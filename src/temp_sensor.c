/*
 * MIT License
 *
 * Copyright (c) 2017 David Antliff
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "temp_sensor.h"
#include "dht11.h"
#include "mqtt.h"

#define GPIO_DS18B20_0 4

void close_temp()
{
  return;
}

void temp_read()
{
  char message[50];
  struct dht11_reading data;
  struct dht11_reading last_data = {
    .humidity = 0,
    .temperature = 0,
  };
  while (1)
  {
    data = DHT11_read();
    if (data.temperature != -1)
    {
      last_data.temperature = data.temperature;
    }
    if (data.humidity != -1)
    {
      last_data.humidity = data.humidity;
    }
    sprintf(message, "{\"temperature\":\"%d\"}", last_data.temperature);
    printf("%s\n", message);
    mqtt_send_message(MQTT_TELEMETRY, message);
    sprintf(message, "{\"humidity\":\"%d\"}", last_data.humidity);
    mqtt_send_message(MQTT_TELEMETRY, message);
    printf("%s\n", message);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

int temp_init()
{
  DHT11_init(GPIO_DS18B20_0);

  return 1;
}