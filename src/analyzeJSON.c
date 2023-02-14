#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esp_log.h"
#include "analyzeJSON.h"

#define TAG "JSON_PARSER"

int json_parse_return_comm(const char *const json)
{
  if (strstr(json, "turnOnSystem"))
  {
    return TURN_ON_SYSTEM;
  }

  if (strstr(json, "turnOffSystem"))
  {
    return TURN_OFF_SYSTEM;
  }

  return -1;
}
