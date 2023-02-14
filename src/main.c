#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "ports.h"
#include "bpm_sensor.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "freertos/semphr.h"
#include "wifi.h"
#include "mqtt.h"
#include "driver/gpio.h"
#include "gpio_setup.h"

#define RED_LED_PIN 5
#define YELLOW_LED_PIN 18
#define PUSH_BUTTON_PIN 0

int heartbeat = 0;
float isOn = false;

SemaphoreHandle_t wifiConnectionSemaphore;
SemaphoreHandle_t mqttConnectionSemaphore;
SemaphoreHandle_t heartbeatSensorSemaphore;

void turnOff()
{
  gpio_set_level(RED_LED_PIN, 1);
  gpio_set_level(YELLOW_LED_PIN, 0);
  gpio_set_level(GPIO_NUM_2, 0);
  isOn = false;
  mqtt_stop();
  wifi_stop();
}

void initHeartbeatRoutine(void * params)
{    
  setup(HEARTBEAT_SENSOR);
  vTaskDelay(1);
  while (isOn)
  {
    monitorBPM();
  }
}

void wifiConnected(void * params)
{
  while(true)
  {
    if(xSemaphoreTake(wifiConnectionSemaphore, portMAX_DELAY))
    {
      mqtt_start();
    }
  }
}

void turnOn() {
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  isOn = true;

  gpio_set_level(RED_LED_PIN, 0);
  gpio_set_level(YELLOW_LED_PIN, 1);
  gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
  gpio_set_level(GPIO_NUM_2, 1);
    
  wifiConnectionSemaphore = xSemaphoreCreateBinary();
  mqttConnectionSemaphore = xSemaphoreCreateBinary();
  wifi_start();
  
  xTaskCreate(&wifiConnected, "Conexão Wi-fi", 2048, NULL, 1, NULL);
  xTaskCreate(&initHeartbeatRoutine, "Conexão MQTT", 2048, NULL, 1, NULL);
}

void handleButton(void * params)
{
  gpio_set_direction(RED_LED_PIN, GPIO_MODE_OUTPUT);
  gpio_set_direction(YELLOW_LED_PIN, GPIO_MODE_OUTPUT);
  gpio_set_level(RED_LED_PIN, 1);
  gpio_set_level(YELLOW_LED_PIN, 0);

while (true)
{
  if (gpio_get_level(PUSH_BUTTON_PIN) == 0 && isOn == false) {
    turnOn();
  } else if (gpio_get_level(PUSH_BUTTON_PIN) == 1 && isOn == true) {
    turnOff();
  }
  vTaskDelay(1);
  }
}

void app_main()
{
  xTaskCreate(&handleButton, "Inicia Placa", 4096, NULL, 1, NULL);
}