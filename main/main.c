#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "gpio_config.h"
#include "adc_config.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "ws2812_control.h"

//  WIFI - SETTINGS
#define SSID "INC3"
#define PASSWORD "cyber3INC"
//  WiFi will be used to connect to the network and send data.


// GPIO / CHANNEL SETTINGS
#define ADC_CHANNEL ADC_CHANNEL_4   // GPIO 5
#define BUTTON_GPIO 4               // GPIO pentru buton
#define LED_GPIO    46               // GPIO pentru LED

// Declaram functia wifi_connect care va fi folosita pentru a conecta ESP32 la WiFi
void wifi_connect(const char *ssid, const char *password);

void app_main(void)
{    
    configure_LED_output(LED_GPIO); // Configuram LED-ul ca iesire
    configure_BUTTON_input(BUTTON_GPIO); // Configuram butonul ca intrare

    adc_oneshot_unit_handle_t adc_handle; // Declararea unui handle pentru ADC
    adc_channel_t channel = ADC_CHANNEL;  // GPIO 5
    configure_adc(&adc_handle, channel); // Configuram ADC-ul pentru canalul specificat

    // Verificam daca partitia NVS este initializata, daca nu, o initializam
    // NVS (Non-Volatile Storage) este folosit pentru a stoca date
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase()); // Stergem partitia NVS
        ret = nvs_flash_init(); // Reinitializam partitia NVS
    }
    ESP_ERROR_CHECK(ret);

    // Initializam ssid si pass cu detaliile SSID-ului, precum parola de la router (Wifi)
    const char *my_ssid = SSID;
    const char *my_pass = PASSWORD;
   wifi_connect(my_ssid, my_pass); // Conectam la WiFi folosind SSID-ul si parola specificate

    while (1) {
        int btn_state = gpio_get_level(BUTTON_GPIO);
        if (btn_state == 0) {
            vTaskDelay(pdMS_TO_TICKS(50)); // debounce
            if (btn_state == 0) {
                int voltage = read_adc_voltage(adc_handle, channel);
                printf("Tensiune ADC: %d mV\n", voltage);

                gpio_set_level(LED_GPIO, 1);
                vTaskDelay(pdMS_TO_TICKS(500));
                gpio_set_level(LED_GPIO, 0);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
