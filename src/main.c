#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "ports.h"
#include "bpm_sensor.h"

void bpmTask(void * params)
{
  monitorBPM();
}

void app_main()
{
  setup(HEARTBEAT_SENSOR);
  xTaskCreate(&bpmTask,  "BPM Task", 4096, NULL, 1, NULL);
}