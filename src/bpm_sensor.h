#ifndef _BPM_SENSOR_
#define _BPM_SENSOR_

#include "adc_module.h"

#define SAMP_SIZE 4
#define RISE_THRESHOLD 4

void setup(int channel);
void monitorBPM();

#endif