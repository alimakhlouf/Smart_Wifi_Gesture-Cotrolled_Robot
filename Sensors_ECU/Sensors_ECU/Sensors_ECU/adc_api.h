/*
 * adc_api.h
 *
 * Created: 6/13/2019 04:20:29 ص
 *  Author: hppp
 */ 


#ifndef ADC_API_H_
#define ADC_API_H_
#include "micro_config.h"
#include "std_types.h"
#include "ADC_lib.h"
#include "stdbool.h"



/*ANALOG SIGNAL UPPER LIMIT SOURCE
AREF_PIN 
AVCC_PIN 
INTERNAL_SOURCE 
CHOOSE ONE OF THE ABOVE IN THE FOLLOWING CONFIGURAION*/
#define UPPER_LIMIT_SOURCE AVCC_PIN

void ADC_init(void);

void adc_set_isr(void ( * adc_int_function)(void));

void adc_convert (unint8_t channel_num);

unint16_t ADC_read (unint8_t channel_num);

void adc_set_int (bool b_state);

void adc_read();

#endif /* ADC_API_H_ */