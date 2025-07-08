#include "adc_config.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_log.h"

#define ADC_ATTEN   ADC_ATTEN_DB_12 // Atenuarea ADC-ului, 12 dB, 0 - ~3.6V
#define DEFAULT_VREF 3300 // Vref ADC este 3.3V

static const char *TAG = "ADC";

static adc_cali_handle_t cali_handle = NULL;

// Inițializare ADC pentru un canal dat
void configure_adc(adc_oneshot_unit_handle_t *adc_handle, adc_channel_t channel) {
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_1
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, adc_handle));

    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT, // Setează bitwidth-ul ADC-ului
        .atten = ADC_ATTEN // Setează atenuarea ADC-ului
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(*adc_handle, channel, &config));

  // First trying curve fitting calibration scheme, if supported
  // If curve fitting is not supported, it will fall back to line fitting calibration scheme
  #if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
  adc_cali_curve_fitting_config_t cali_config_cf = {
      .unit_id = ADC_UNIT_1,
      .chan = channel,
      .atten = ADC_ATTEN,
      .bitwidth = ADC_BITWIDTH_DEFAULT
  };
  if (adc_cali_create_scheme_curve_fitting(&cali_config_cf, &cali_handle) == ESP_OK) {
      ESP_LOGI(TAG, "ADC calibration enabled (curve fitting)");
      return;
  }
#endif

    // If curve fitting calibration scheme is not supported or failed, try line fitting calibration scheme
#if ADC_CALI_SCHEME_LINE_FITTING_SUPPORTED
    adc_cali_line_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT_1,
        .atten = ADC_ATTEN,
        .bitwidth = ADC_BITWIDTH_DEFAULT
    };
    if (adc_cali_create_scheme_line_fitting(&cali_config, &cali_handle) == ESP_OK) {
        ESP_LOGI(TAG, "ADC calibration enabled");
    } else {
        ESP_LOGW(TAG, "ADC calibration not available, using manual conversion");
        // Dacă calibrarea nu este disponibilă, setează cali_handle la NULL
        cali_handle = NULL;
    }
#endif
}

// Citirea tensiunii (în mV) de pe un canal ADC
int read_adc_voltage(adc_oneshot_unit_handle_t adc_handle, adc_channel_t channel) {
    int raw = 0;
    ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, channel, &raw));
    ESP_LOGI(TAG, "ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_1 + 1, channel, raw);
    if (cali_handle) {
        int voltage = 0;
        ESP_ERROR_CHECK(adc_cali_raw_to_voltage(cali_handle, raw, &voltage));
        return voltage;
    } else {
        float voltage = (raw / 4095.0) * DEFAULT_VREF; // Conversie la mV 
        return (int)voltage;// Dacă nu e calibrare, întoarce valoarea in mV manual
    }
}
