#pragma once
#include "esp_err.h"
#include "pid_ctrl.h"

class Pid
{
private:
  pid_ctrl_block_handle_t pid;

public:
  Pid(const pid_ctrl_config_t *config);
  ~Pid();
  esp_err_t update(const pid_ctrl_parameter_t *params);
  float compute(float input_error);
  esp_err_t reset();
};
