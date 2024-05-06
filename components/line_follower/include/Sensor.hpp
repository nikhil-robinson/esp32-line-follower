#pragma once

#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/soc_caps.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class Sensor
{
private:
  adc_oneshot_unit_handle_t adc_handle;
  adc_channel_t _chan;

public:
  Sensor(adc_channel_t chan, adc_unit_t unit);
  ~Sensor();

  int read();
};
