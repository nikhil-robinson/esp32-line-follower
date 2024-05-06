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
    esp_err_t compute(float input_error, float *ret_result);
    esp_err_t reset();
};

Pid::Pid(const pid_ctrl_config_t *config)
{
    pid_new_control_block(config, &this->ret_pid);
}

Pid::~Pid()
{
    pid_del_control_block(this->pid);
}


esp_err_t Pid::update(const pid_ctrl_parameter_t *params)
{
    return pid_update_parameters(this->pid, params);
}
esp_err_t Pid::compute(float input_error, float *ret_result)
{
    return pid_compute(this->pid, input_error, ret_result);

}
esp_err_t Pid::reset()
{
    return pid_reset_ctrl_block(this->pid);
}