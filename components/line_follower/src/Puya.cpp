#include "Puya.hpp"

Puya::Puya(gpio_num_t tx, gpio_num_t rx, uart_port_t uart_num = UART_NUM_1) : num(uart_num)
{
    esp_log_level_set(TAG, ESP_LOG_INFO);

    /* Configure this->parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    // Install UART driver, and get the queue.
    uart_driver_install(this->num, 1024 * 2, 1024 * 2, 200, &this->uart0_queue, 0);
    uart_param_config(this->num, &uart_config);

    // Set UART log level
    esp_log_level_set(TAG, ESP_LOG_INFO);
    // Set UART pins (using UART0 default pins ie no changes.)
    uart_set_pin(this->num, tx, rx, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    // Set uart pattern detect function.
    uart_enable_pattern_det_baud_intr(this->num, '+', 3, 9, 0, 0);
    // Reset the pattern queue length to record at most 20 pattern positions.
    uart_pattern_queue_reset(this->num, 20);

    // // Create a task to handler UART this->event from ISR
    // xTaskCreate(uart_event_task, "uart_event_task", 2048, NULL, 12, NULL);
}

Puya::~Puya()
{
}

sensor_val_t Puya::read()
{
    if (xQueueReceive(this->uart0_queue, (void *)&this->event, (TickType_t)portMAX_DELAY))
    {
        bzero(dtmp, 1024);
        ESP_LOGI(TAG, "uart[%d] event: [%d]", this->num, this->event.type);
        if (this->event.type == UART_DATA)
        {
            uart_read_bytes(this->num, dtmp, this->event.size, portMAX_DELAY);
            ESP_LOGI(TAG, "[UART DATA]: size : [%d]", this->event.size);
            ESP_LOG_BUFFER_HEX(TAG, dtmp, this->event.size);
            if ((dtmp[0] == 'L') && (dtmp[11] == 'F'))
            {
                this->value.sen1 =  ((dtmp[1] << 8) | dtmp[2]);
                this->value.sen2 =  ((dtmp[3] << 8) | dtmp[4]);
                this->value.sen3 =  ((dtmp[5] << 8) | dtmp[6]);
                this->value.sen4 =  ((dtmp[7] << 8) | dtmp[8]);
                this->value.sen5 =  ((dtmp[9] << 8) | dtmp[10]);


                ESP_LOGI(TAG, "%d %d %d %d %d", this->value.sen1,this->value.sen2,this->value.sen3,this->value.sen4,this->value.sen5);
                
            }
            return this->value;

        }
        else if (this->event.type == UART_BUFFER_FULL)
        {
            uart_flush_input(this->num);
            xQueueReset(this->uart0_queue);
        }

        
    }

    return this->value;
}