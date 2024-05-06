#pragma once

#include "driver/pulse_cnt.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include <stdio.h>

#define BDC_ENCODER_PCNT_HIGH_LIMIT 1000
#define BDC_ENCODER_PCNT_LOW_LIMIT -1000

typedef struct
{
  pcnt_chan_config_t ch0;
  pcnt_chan_config_t ch1;
} pcnt_config_t;

class Encoder
{
private:
  pcnt_unit_handle_t pcnt_unit = NULL;
  pcnt_channel_handle_t pcnt_chan_a = NULL;
  pcnt_channel_handle_t pcnt_chan_b = NULL;

public:
  Encoder(pcnt_config_t conf);
  ~Encoder();

  esp_err_t enable();
  esp_err_t disable();
  esp_err_t clear_count();
  esp_err_t start();
  esp_err_t stop();
  int get_count();
  esp_err_t add_watch_point(int watch_point);
  esp_err_t remove_watch_point(int watch_point);
};
