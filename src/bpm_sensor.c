#include "bpm_sensor.h"
#include <float.h>
#include "gpio_setup.h"
#include "esp_timer.h"
#include "adc_module.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mqtt.h"

float readings[SAMP_SIZE], first, second, third;
float readingsSum, currentReading, sensorValue, startTime, lastReading, printValue, bpm;
long int currentTime, readingsIndex, bpmListIndex, lastBeat;
int riseCount, n;
bool rising;
int HBChannel;

void setup(int channel) 
{
    adc_init(ADC_UNIT_1);
    pinMode(channel, GPIO_ANALOG);
    HBChannel = channel;

	for(int i = 0; i < SAMP_SIZE; i++)
		readings[i] = 0;
	readingsSum = 0;
	readingsIndex = 0;
	bpmListIndex = 0;
}

void sendHeartbeatMQTTMessage(float heartbeat)
{
  char message[50];

  sprintf(message, "{\"heartbeat\": %f}", heartbeat);
  mqtt_send_message(MQTT_TELEMETRY, message);
  printf("enviado \n");
}

void monitorBPM()
{
  while (true)
  {
    n = 0;
    startTime = esp_timer_get_time();
    sensorValue = 0;
    do{
      sensorValue += analogRead(HBChannel);
      n++;
      currentTime = esp_timer_get_time();
    }while(currentTime < startTime + 20);  
    sensorValue /= n;
    
    readingsSum -= readings[readingsIndex];
    readingsSum += sensorValue;
    readings[readingsIndex] = sensorValue;
    currentReading = readingsSum / SAMP_SIZE;

    if (currentReading > lastReading) {
        riseCount++;
        if (!rising && riseCount > RISE_THRESHOLD) {
          //   Ok, we have detected a rising curve, which implies a heartbeat.
          //   Record the time since last beat, keep track of the two previous
          //   times (first, second, third) to get a weighed average.
          // The rising   flag prevents us from detecting the same rise more than once.
          rising   = true;
          first = esp_timer_get_time() - lastBeat;
          lastBeat = esp_timer_get_time();

           // Calculate the weighed average of heartbeat rate
          // according   to the three last beats
          bpm = (60000. / (0.4 * first + 0.3 *   second + 0.3 * third))*10000;
          
          printf("%f\n", bpm);
          
          third = second;
          second   = first;
          
        }
      } else {
        //   Ok, the curve is falling
        rising = false;
        riseCount = 0;
      }
      
    lastReading = currentReading;
    readingsIndex++;
    readingsIndex %= SAMP_SIZE;
    sendHeartbeatMQTTMessage(bpm);
    vTaskDelay(500/portTICK_PERIOD_MS);
  }
}