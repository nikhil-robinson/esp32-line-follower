#include "Encoder.hpp"

Encoder::Encoder(pcnt_config_t conf)
{
  pcnt_unit_config_t unit_config = {
      .low_limit = BDC_ENCODER_PCNT_LOW_LIMIT,
      .high_limit = BDC_ENCODER_PCNT_HIGH_LIMIT,

      .flags = {
          .accum_count = true, // enable counter accumulation
      }};

  ESP_ERROR_CHECK(pcnt_new_unit(&unit_config, &pcnt_unit));

  pcnt_glitch_filter_config_t filter_config = {
      .max_glitch_ns = 1000,
  };

  ESP_ERROR_CHECK(pcnt_unit_set_glitch_filter(pcnt_unit, &filter_config));

  ESP_ERROR_CHECK(pcnt_new_channel(pcnt_unit, &conf.ch0, &pcnt_chan_a));
  ESP_ERROR_CHECK(pcnt_new_channel(pcnt_unit, &conf.ch1, &pcnt_chan_b));

  ESP_ERROR_CHECK(pcnt_channel_set_edge_action(
      pcnt_chan_a, PCNT_CHANNEL_EDGE_ACTION_DECREASE,
      PCNT_CHANNEL_EDGE_ACTION_INCREASE));
  ESP_ERROR_CHECK(
      pcnt_channel_set_level_action(pcnt_chan_a, PCNT_CHANNEL_LEVEL_ACTION_KEEP,
                                    PCNT_CHANNEL_LEVEL_ACTION_INVERSE));
  ESP_ERROR_CHECK(pcnt_channel_set_edge_action(
      pcnt_chan_b, PCNT_CHANNEL_EDGE_ACTION_INCREASE,
      PCNT_CHANNEL_EDGE_ACTION_DECREASE));
  ESP_ERROR_CHECK(
      pcnt_channel_set_level_action(pcnt_chan_b, PCNT_CHANNEL_LEVEL_ACTION_KEEP,
                                    PCNT_CHANNEL_LEVEL_ACTION_INVERSE));

  ESP_ERROR_CHECK(
      pcnt_unit_add_watch_point(pcnt_unit, BDC_ENCODER_PCNT_HIGH_LIMIT));
  ESP_ERROR_CHECK(
      pcnt_unit_add_watch_point(pcnt_unit, BDC_ENCODER_PCNT_LOW_LIMIT));
}

Encoder::~Encoder()
{
  pcnt_del_unit(this->pcnt_unit);
  pcnt_del_channel(this->pcnt_chan_a);
  pcnt_del_channel(this->pcnt_chan_b);
}

esp_err_t Encoder::enable() { return pcnt_unit_enable(this->pcnt_unit); }

esp_err_t Encoder::disable() { return pcnt_unit_disable(this->pcnt_unit); }

esp_err_t Encoder::clear_count()
{
  return pcnt_unit_clear_count(this->pcnt_unit);
}

esp_err_t Encoder::start() { return pcnt_unit_start(this->pcnt_unit); }

esp_err_t Encoder::stop() { return pcnt_unit_start(this->pcnt_unit); }

int Encoder::get_count()
{
  int count = 0;
  ESP_ERROR_CHECK(pcnt_unit_get_count(pcnt_unit, &count));
  return count;
}

esp_err_t Encoder::add_watch_point(int watch_point)
{
  return pcnt_unit_add_watch_point(this->pcnt_unit, watch_point);
}

esp_err_t Encoder::remove_watch_point(int watch_point)
{
  return pcnt_unit_remove_watch_point(this->pcnt_unit, watch_point);
}