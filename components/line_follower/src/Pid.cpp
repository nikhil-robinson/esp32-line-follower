#include "Pid.hpp"

Pid::Pid(const pid_ctrl_config_t *config)
{
  ESP_ERROR_CHECK(pid_new_control_block(config, &this->pid));
}

Pid::~Pid() { ESP_ERROR_CHECK(pid_del_control_block(this->pid)); }

esp_err_t Pid::update(const pid_ctrl_parameter_t *params)
{
  return pid_update_parameters(this->pid, params);
}

float Pid::compute(float input_error)
{
  float ret_result = 0.0;
  ESP_ERROR_CHECK(pid_compute(this->pid, input_error, &ret_result));
  return ret_result;
}

esp_err_t Pid::reset() { return pid_reset_ctrl_block(this->pid); }