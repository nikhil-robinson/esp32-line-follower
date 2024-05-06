#include "Encoder.hpp"
#include "Motor.hpp"
#include "Pid.hpp"
#include "Sensor.hpp"
#include <stdio.h>
#include "Puya.hpp"


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




void motor_task(void *args)
{

    motor1 = new Motor(GPIO_NUM_41, GPIO_NUM_37,  0);
    motor2 = new Motor(GPIO_NUM_21, GPIO_NUM_35, 0);

    motor1->enable();
    motor2->enable();

    motor1->set_speed(400);
    motor2->set_speed(400);

    while (true)
    {
        motor1->forward();
        motor2->forward();
        motor1->set_speed(400);
        motor2->set_speed(400);
        vTaskDelay(1);
    }
}

void encoder_task(void *args)
{
    pcnt_config_t conf1 = {
        .ch0 = {
            .edge_gpio_num = 10,
            .level_gpio_num =11,
        },
        .ch1 = {
            .edge_gpio_num = 11,
            .level_gpio_num =10,
        },
    };
    encoder1 = new Encoder(conf1);

    pcnt_config_t conf2 = {
        .ch0 = {
            .edge_gpio_num = 13,
            .level_gpio_num =12,
        },
        .ch1 = {
            .edge_gpio_num = 12,
            .level_gpio_num =13,
        },
    };

    encoder2 = new Encoder(conf2);


    encoder1->enable();
    encoder2->enable();
    encoder1->clear_count();
    encoder2->clear_count();
    encoder1->start();
    encoder2->start();

    while (true)
    {
        ESP_LOGI(TAG,"EN1 count [%d] EN2 count [%d]",encoder1->get_count(),encoder2->get_count());
        vTaskDelay(1);
    }
}

extern "C" void app_main()
{

    // xTaskCreatePinnedToCore(encoder_task, "encoder_task", 4096, NULL, 20, NULL,1);
    // xTaskCreatePinnedToCore(motor_task, "motor_task", 4096, NULL, 20, NULL,0);

    puya = new Puya(GPIO_NUM_17,GPIO_NUM_16,UART_NUM_1);

    while (true)
    {
        sensor_val_t val = puya->read();
        vTaskDelay(1);
    }
    


}