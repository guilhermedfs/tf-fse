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

SemaphoreHandle_t wifiConnectionSemaphore;
SemaphoreHandle_t mqttConnectionSemaphore;

void bpmTask(void * params)
{
  monitorBPM();
}

void handleServerConnection(void * params)
{
  char message[50];
  if(xSemaphoreTake(mqttConnectionSemaphore, portMAX_DELAY))
  {
    while(true)
    {
      float temperature = 30.2;
      sprintf(message, "{\"temperature\": %f}", temperature);
      mqtt_send_message(MQTT_TELEMETRY, message);
      printf("enviado \n");
      vTaskDelay(3000 / portTICK_PERIOD_MS);

    }
  }
}

void wifiConnected(void * params)
{
  while(true)
  {
    if(xSemaphoreTake(wifiConnectionSemaphore, portMAX_DELAY))
    {
      // Processamento Internet
      mqtt_start();
    }
  }
}

void app_main()
{

  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);
    
  wifiConnectionSemaphore = xSemaphoreCreateBinary();
  mqttConnectionSemaphore = xSemaphoreCreateBinary();
  wifi_start();

  // setup(HEARTBEAT_SENSOR);
  // xTaskCreate(&bpmTask,  "BPM Task", 2048, NULL, 1, NULL);

  // xTaskCreate(&wifiConnected, "Conexão MQTT", 2048, NULL, 1, NULL);
  // xTaskCreate(&handleServerConnection, "Conexão MQTT", 2048, NULL, 1, NULL);
}