#pragma once

#include "bdc_motor.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "sdkconfig.h"
#include <stdio.h>


#define BDC_MCPWM_TIMER_RESOLUTION_HZ   CONFIG_BDC_MCPWM_TIMER_RESOLUTION_HZ
#define BDC_MCPWM_FREQ_HZ               CONFIG_BDC_MCPWM_TIMER_RESOLUTION_HZ
#define BDC_MCPWM_DUTY_TICK_MAX         (BDC_MCPWM_TIMER_RESOLUTION_HZ / BDC_MCPWM_FREQ_HZ)

class Motor {
private:

  bdc_motor_handle_t motor = NULL;

public:

/**
 * @brief Create BDC Motor based on MCPWM peripheral
 *
 * @param pin_a: BDC pin a
 * @param pin_b: BDC pin b
 * @param ret_motor Returned BDC Motor handle
 * @return
 *      - ESP_OK: Create BDC Motor handle successfully
 *      - ESP_ERR_INVALID_ARG: Create BDC Motor handle failed because of invalid argument
 *      - ESP_ERR_NO_MEM: Create BDC Motor handle failed because of out of memory
 *      - ESP_FAIL: Create BDC Motor handle failed because some other error
 */
  Motor(gpio_num_t pin_a, gpio_num_t pin_b, int group_id = 0);
  ~Motor();

  /**
   * @brief Enable BDC motor
   *
   *
   * @return
   *      - ESP_OK: Enable motor successfully
   *      - ESP_ERR_INVALID_ARG: Enable motor failed because of invalid
   * parameters
   *      - ESP_FAIL: Enable motor failed because other error occurred
   */
  esp_err_t enable();

  /**
   * @brief Disable BDC motor
   *
   *
   * @return
   *      - ESP_OK: Disable motor successfully
   *      - ESP_ERR_INVALID_ARG: Disable motor failed because of invalid
   * parameters
   *      - ESP_FAIL: Disable motor failed because other error occurred
   */
  esp_err_t disable();

  /**
   * @brief Set speed for bdc motor
   *
   * @param speed: BDC speed
   *
   * @return
   *      - ESP_OK: Set motor speed successfully
   *      - ESP_ERR_INVALID_ARG: Set motor speed failed because of invalid
   * parameters
   *      - ESP_FAIL: Set motor speed failed because other error occurred
   */
  esp_err_t set_speed(uint32_t speed);

  /**
   * @brief Forward BDC motor
   *
   *
   * @return
   *      - ESP_OK: Forward motor successfully
   *      - ESP_FAIL: Forward motor failed because some other error occurred
   */
  esp_err_t forward();

  /**
   * @brief Reverse BDC Motor
   *
   *
   * @return
   *      - ESP_OK: Reverse motor successfully
   *      - ESP_FAIL: Reverse motor failed because some other error occurred
   */
  esp_err_t reverse();

  /**
   * @brief Stop motor in a coast way (a.k.a Fast Decay)
   *
   *
   * @return
   *      - ESP_OK: Stop motor successfully
   *      - ESP_FAIL: Stop motor failed because some other error occurred
   */
  esp_err_t coast();

  /**
   * @brief Stop motor in a brake way (a.k.a Slow Decay)
   *
   *
   * @return
   *      - ESP_OK: Stop motor successfully
   *      - ESP_FAIL: Stop motor failed because some other error occurred
   */
  esp_err_t stop();
};
