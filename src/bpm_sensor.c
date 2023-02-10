#include "bpm_sensor.h"
#include <float.h>
#include "gpio_setup.h"
#include "esp_timer.h"
#include "adc_module.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

float readings[SAMP_SIZE], beats[BEATS_SIZE], bpmList[BPM_AVG_SIZE];
float readingsSum, bpmListSum, currentReading, sensorValue, startTime, lastReading, printValue, bpm;
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
	for(int i = 0; i < BPM_AVG_SIZE; i++)
		bpmList[i] = 0;
	for(int i = 0; i < BEATS_SIZE; i++)
		beats[i] = FLT_MAX;
	readingsSum = 0;
	readingsIndex = 0;
	bpmListSum = 0;
	bpmListIndex = 0;
}

void monitorBPM()
{
    while (true)
  {
    n = 0;
    startTime = esp_timer_get_time() / 1000;
    sensorValue = 0.;
    do{
      sensorValue += analogRead(HBChannel);
      n++;
      currentTime = esp_timer_get_time() / 1000;
    }while(currentTime < startTime + 20);  
    sensorValue /= n;
    
    readingsSum -= readings[readingsIndex];
    readingsSum += sensorValue;
    readings[readingsIndex] = sensorValue;
    
    currentReading = readingsSum / SAMP_SIZE;
    
    if (currentReading > lastReading && currentReading-lastReading < RISE_LIMIT){
      riseCount++;
      if (!rising && riseCount > RISE_THRESHOLD){
        rising = true;
        beats[0] = (esp_timer_get_time() / 1000) - lastBeat;
        lastBeat = esp_timer_get_time() / 1000;

        float beatsSum = 0;
        for(int i = 0; i < BEATS_SIZE; i++) beatsSum += beats[i];
        bpm = BEATS_SIZE*60000.0/beatsSum;

        bpmListSum -= bpmList[bpmListIndex];
        bpmListSum += bpm;
        bpmList[bpmListIndex++] = bpm;
        bpmListIndex %= BPM_AVG_SIZE;
        printValue = bpmListSum/BPM_AVG_SIZE;
        
        printf("%f\n", printValue);
        
        for(int i = BEATS_SIZE-1; i>0; i--) beats[i] = beats[i-1];
      }
    }
    else{
      rising = false;
      riseCount = 0;
    }
    lastReading = currentReading;
    readingsIndex++;
    readingsIndex %= SAMP_SIZE;
    vTaskDelay(1);
  }
}