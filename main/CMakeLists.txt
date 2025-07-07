idf_component_register(SRCS "adc_config.c" "wifi_connect.c" "main.c" "ws2812_control.c" "gpio_config.c"
                    PRIV_REQUIRES esp_wifi nvs_flash led_strip driver esp_adc
                    INCLUDE_DIRS ".")