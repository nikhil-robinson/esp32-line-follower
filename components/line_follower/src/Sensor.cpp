#include "Sensor.hpp"

Sensor::Sensor(adc_channel_t chan, adc_unit_t unit) : _chan(chan)
{
  adc_oneshot_unit_init_cfg_t init_config1 = {
      .unit_id = unit,
  };
  ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &this->adc_handle));

  adc_oneshot_chan_cfg_t config = {
      .atten = ADC_ATTEN_DB_12,
      .bitwidth = ADC_BITWIDTH_DEFAULT,

  };
  ESP_ERROR_CHECK(adc_oneshot_config_channel(this->adc_handle, chan, &config));
}

Sensor::~Sensor() { ESP_ERROR_CHECK(adc_oneshot_del_unit(this->adc_handle)); }

int Sensor::read()
{
  int val = 0;
  ESP_ERROR_CHECK(adc_oneshot_read(this->adc_handle, this->_chan, &val));
}
