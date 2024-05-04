#pragma once

#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/pulse_cnt.h"

#define BDC_ENCODER_PCNT_HIGH_LIMIT   1000
#define BDC_ENCODER_PCNT_LOW_LIMIT    -1000


typedef struct {
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
    esp_err_t get_count();

};

Encoder::Encoder(pcnt_config_t conf)
{
    pcnt_unit_config_t unit_config = {
        .high_limit = BDC_ENCODER_PCNT_HIGH_LIMIT,
        .low_limit = BDC_ENCODER_PCNT_LOW_LIMIT,
        .flags = {
            .accum_count = true, // enable counter accumulation
        }
    };

    ESP_ERROR_CHECK(pcnt_new_unit(&unit_config, &pcnt_unit));

    pcnt_glitch_filter_config_t filter_config = {
        .max_glitch_ns = 1000,
    };

    ESP_ERROR_CHECK(pcnt_unit_set_glitch_filter(pcnt_unit, &filter_config));

    ESP_ERROR_CHECK(pcnt_new_channel(pcnt_unit, &conf.ch0, &pcnt_chan_a));
    ESP_ERROR_CHECK(pcnt_new_channel(pcnt_unit, &conf.ch1, &pcnt_chan_b));

    ESP_ERROR_CHECK(pcnt_channel_set_edge_action(pcnt_chan_a, PCNT_CHANNEL_EDGE_ACTION_DECREASE, PCNT_CHANNEL_EDGE_ACTION_INCREASE));
    ESP_ERROR_CHECK(pcnt_channel_set_level_action(pcnt_chan_a, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE));
    ESP_ERROR_CHECK(pcnt_channel_set_edge_action(pcnt_chan_b, PCNT_CHANNEL_EDGE_ACTION_INCREASE, PCNT_CHANNEL_EDGE_ACTION_DECREASE));
    ESP_ERROR_CHECK(pcnt_channel_set_level_action(pcnt_chan_b, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE));
    ESP_ERROR_CHECK(pcnt_unit_add_watch_point(pcnt_unit, BDC_ENCODER_PCNT_HIGH_LIMIT));
    ESP_ERROR_CHECK(pcnt_unit_add_watch_point(pcnt_unit, BDC_ENCODER_PCNT_LOW_LIMIT));
    
}



Encoder::~Encoder()
{
}
