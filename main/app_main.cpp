#include "Encoder.hpp"
#include "Motor.hpp"
#include "Pid.hpp"
#include "Sensor.hpp"
#include <stdio.h>
#include "Puya.hpp"
#include "Utility.hpp"

#define TAG "LINE_FOLLOWER"

Encoder *encoder1 = nullptr;
Encoder *encoder2 = nullptr;

Motor *motor1 = nullptr;
Motor *motor2 = nullptr;

Sensor *sensor1 = nullptr;
Sensor *sensor2 = nullptr;
Sensor *sensor3 = nullptr;
Sensor *sensor4 = nullptr;
Sensor *sensor5 = nullptr;

Pid *pid = nullptr;

Puya *puya = nullptr;

static volatile int minValues[6], maxValues[6], threshold[6];

int P, D, I, previousError, PIDvalue, error;
int lsp, rsp;
int lfspeed = 40;

float Kp = 0;
float Kd = 0;
float Ki = 0;

pid_ctrl_parameter_t pid_para = {
    .kp = 0.6,
    .ki = 0.4,
    .kd = 0.2,
    .cal_type = PID_CAL_TYPE_INCREMENTAL,
    .max_output   = 254,
    .min_output   = 0,
    .max_integral = 254,
    .min_integral = -0,

}

void encoder_init()
{
    pcnt_config_t conf1 = {
        .ch0 = {
            .edge_gpio_num = 10,
            .level_gpio_num = 11,
        },
        .ch1 = {
            .edge_gpio_num = 11,
            .level_gpio_num = 10,
        },
    };
    encoder1 = new Encoder(conf1);

    pcnt_config_t conf2 = {
        .ch0 = {
            .edge_gpio_num = 13,
            .level_gpio_num = 12,
        },
        .ch1 = {
            .edge_gpio_num = 12,
            .level_gpio_num = 13,
        },
    };

    encoder2 = new Encoder(conf2);

    encoder1->enable();
    encoder2->enable();
    encoder1->clear_count();
    encoder2->clear_count();
    encoder1->start();
    encoder2->start();
}


void claibrate()
{
    ESP_LOGI(TAG, "INSIDE CALIBRATION");
    for (int i = 1; i < 6; i++)
    {
        minValues[i] = puya->read(i);
        maxValues[i] = puya->read(i);
    }

    for (int i = 0; i < 3000; i++)
    {
        motor1->drive(50);
        motor2->drive(-50);

        for (int i = 1; i < 6; i++)
        {
            if (puya->read(i) < minValues[i])
            {
                minValues[i] = puya->read(i);
            }
            if (puya->read(i) > maxValues[i])
            {
                maxValues[i] = puya->read(i);
            }
        }
    }

    for (int i = 1; i < 6; i++)
    {
        threshold[i] = (minValues[i] + maxValues[i]) / 2;
        ESP_LOGI(TAG, "THR[i] %d", threshold[i]);
    }

    motor1->drive(0);
    motor2->drive(0);
}

void linefollow(void *args)
{

    pid_ctrl_config_t conf ={
        .init_param =
    };
    pid = new Pid();

    while (1)
    {
        if (puya->read(1) > threshold[1] && puya->read(5) < threshold[5])
        {
            lsp = 0;
            rsp = lfspeed;
            motor1->drive(0);
            motor2->drive(lfspeed);
        }

        else if (puya->read(5) > threshold[5] && puya->read(1) < threshold[1])
        {
            lsp = lfspeed;
            rsp = 0;
            motor1->drive(lfspeed);
            motor2->drive(0);
        }
        else if (puya->read(3) > threshold[3])
        {
            Kp = 0.0006 * (1000 - puya->read(3));
            Kd = 10 * Kp;
            int error = (puya->read(2) - puya->read(4));

            P = error;
            I = I + error;
            D = error - previousError;

            PIDvalue = (Kp * P) + (Ki * I) + (Kd * D);
            previousError = error;

            lsp = lfspeed - PIDvalue;
            rsp = lfspeed + PIDvalue;

            lsp = clamp(lsp, 0, 254);
            rsp = clamp(rsp, 0, 254);

            motor1->drive(lsp);
            motor2->drive(rsp);
        }
        vTaskDelay(1);
    }
}

void wait_for_start()
{
    esp_rom_gpio_pad_select_gpio(GPIO_NUM_36);
    gpio_set_direction(GPIO_NUM_36, GPIO_MODE_INPUT);

    while (!gpio_get_level(GPIO_NUM_36))
    {
        ESP_LOGI(TAG, "WAIT TO WAKE UP");
        vTaskDelay(1);
    }
}

extern "C" void app_main()
{
    motor1 = new Motor(GPIO_NUM_41, GPIO_NUM_37, 0);
    motor2 = new Motor(GPIO_NUM_21, GPIO_NUM_35, 0);

    motor1->enable();
    motor2->enable();

    puya = new Puya(GPIO_NUM_17, GPIO_NUM_16, UART_NUM_1);
    wait_for_start();
    claibrate();
    xTaskCreatePinnedToCore(linefollow, "linefollow", 4096, NULL, 20, NULL, 1);

    // puya = new Puya(GPIO_NUM_17, GPIO_NUM_16, UART_NUM_1);

    // while (1)
    // {
    //     ESP_LOGI(TAG,"[%d] [%d] [%d] [%d] [%d]",puya->read(1),puya->read(2),puya->read(3),puya->read(4),puya->read(5));
    //     vTaskDelay(1);
    // }
}