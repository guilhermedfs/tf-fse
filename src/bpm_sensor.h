#ifndef _BPM_SENSOR_
#define _BPM_SENSOR_

#include "adc_module.h"

#define SAMP_SIZE 20
#define RISE_THRESHOLD 10
#define RISE_LIMIT 10
#define BEATS_SIZE 15
#define BPM_AVG_SIZE 4

void setup(int channel);
void monitorBPM();

#endif