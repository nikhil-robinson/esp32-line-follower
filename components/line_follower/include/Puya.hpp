#pragma once

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include <iostream>
#include <sstream>
#include <string>

#define TAG "puya"


typedef struct
{
  int sen1;
  int sen2;
  int sen3;
  int sen4;
  int sen5;
} sensor_val_t;

class Puya
{
private:
    uart_port_t num = UART_NUM_1;
    uart_event_t event;
    QueueHandle_t uart0_queue;
    size_t buffered_size;
    uint8_t *dtmp = (uint8_t *)malloc(1024);
    sensor_val_t value ={0};

public:
    Puya(gpio_num_t tx, gpio_num_t rx, uart_port_t uart_num);
    ~Puya();
    sensor_val_t read();
    int read(uint8_t id);
};
