set(pr_req  driver
            esp_timer
            nvs_flash
            esp_adc
            pid_ctrl
            bdc_motor)
            
idf_component_register(SRC_DIRS src INCLUDE_DIRS include PRIV_REQUIRES ${pr_req})