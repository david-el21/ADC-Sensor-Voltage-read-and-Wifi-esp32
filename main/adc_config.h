#ifndef ADC_CONFIG_H
#define ADC_CONFIG_H

#include "esp_adc/adc_oneshot.h"

void configure_adc(adc_oneshot_unit_handle_t *adc_handle, adc_channel_t channel);
int read_adc_voltage(adc_oneshot_unit_handle_t adc_handle, adc_channel_t channel);

#endif // ADC_CONFIG_H
