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

SemaphoreHandle_t conexaoWifiSemaphore;

void bpmTask(void * params)
{
  monitorBPM();
}

void connectedWifi(void * params)
{
  while(true)
  {
    if(xSemaphoreTake(conexaoWifiSemaphore, portMAX_DELAY))
    {
      
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
    
    conexaoWifiSemaphore = xSemaphoreCreateBinary();

    wifi_start();

    xTaskCreate(&connectedWifi,  "Conexão com WIFI", 4096, NULL, 1, NULL);

  setup(HEARTBEAT_SENSOR);
  xTaskCreate(&bpmTask,  "BPM Task", 4096, NULL, 1, NULL);
}