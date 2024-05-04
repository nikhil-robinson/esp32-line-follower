#include "Motor.hpp"


Motor::Motor(gpio_num_t pin_a, gpio_num_t pin_b, int group_id = 0) {

  bdc_motor_config_t motor_config = {
      .pwm_freq_hz = BDC_MCPWM_FREQ_HZ,
      .pwma_gpio_num = pin_a,
      .pwmb_gpio_num = pin_b,
  };
  bdc_motor_mcpwm_config_t mcpwm_config = {
        .group_id = group_id,
        .resolution_hz = BDC_MCPWM_TIMER_RESOLUTION_HZ,
    };
  ESP_ERROR_CHECK(bdc_motor_new_mcpwm_device(&motor_config, &mcpwm_config, &motor));
}

Motor::~Motor() {

    bdc_motor_del(this->motor);
}

esp_err_t Motor::enable()
{
    return bdc_motor_enable(this->motor);
}
esp_err_t Motor::disable()
{
    return bdc_motor_disable(this->motor);
}
esp_err_t Motor::set_speed(uint32_t speed)
{
    return bdc_motor_set_speed(this->motor,speed);

}
esp_err_t Motor::forward()
{
    return bdc_motor_forward(this->motor);
}
esp_err_t Motor::reverse()
{
    return bdc_motor_reverse(this->motor);

}
esp_err_t Motor::coast()
{
    return bdc_motor_coast(this->motor);
}
esp_err_t Motor::stop()
{
    return bdc_motor_brake(this->motor);
}