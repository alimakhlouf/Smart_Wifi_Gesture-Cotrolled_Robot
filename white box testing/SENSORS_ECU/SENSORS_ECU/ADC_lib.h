/* --------------------------------------------------------------------------
[FILE NAME]:     ADC_lib.h

[AUTHOR]:        Ali MAkhlouf

[DATE CREATED]:  2/3/2015 11:28:01 AM

[DESCRIPTION]:   a library header file for controlling and operating ADC 
				 peripheral
-----------------------------------------------------------------------------*/

#ifndef ADC_LIB_H_
#define ADC_LIB_H_

#include "micro_config.h"
#include "std_types.h"
#include "adc_api.h"
#include "common_macros.h"
#include "stdbool.h"



#if defined(ATMEGA32) || defined(ATMEGA16) || defined(ATMEGA8)
#define AREF_PIN 0
#define AVCC_PIN 1
#define INTERNAL_SOURCE 3

/*CHOOSE THE RESULT BIT ALIGNMENT
 *LEFT_ADJUSTED_RESULT 1
 *RIGHT_ADJSUTED_RESULT 0
 *CHOOSE ONE OF THE ABOVE IN THE FOLLOWING CONFIGURAION
 */
#define ADJUST_RESULT_BITS 0


/* 1 for enable ---0 for disable*/

//************************* END ***********************************

//THE FREQUENCY PRESCALER
#if (F_CPU > 12000000)
#define FREQ_PRESCALER 7
#elif (F_CPU > 6000000)
#define FREQ_PRESCALER 6
#elif (F_CPU > 3000000)
#define FREQ_PRESCALER 5
#elif (F_CPU > 1000000)
#define FREQ_PRESCALER 4
#elif (F_CPU == 1000000)
#define FREQ_PRESCALER 3
#endif

/*TO STANDARDISE THE ADC_ADJUSTMENT IN ORDER TO READ FROM THEM
  AFTER THE CONVERSION PROCESS HAPPENS*/
#if (ADJUST_RESULT_BITS == 0) /*right adjusted*/
#define GET_ADC_RESULT (ADCL + ((unint16_t)ADCH << 8))
#else /*LEFT ADJUSTED*/
#define GET_ADC_RESULT ((ADCL >> 6) + ((unint16_t)ADCH << 2))
#endif

#if defined(ATMEGA16)
#define CONFIG_ADC_ISR   __vector_14
void CONFIG_ADC_ISR(void) __attribute__((signal,__INTR_ATTRS));
#endif

inline bool adc_int_state()
{
	return ((BIT_IS_SET(SREG, I)) && (BIT_IS_SET(ADCSRA, ADIE)));
}

#endif

/*
 * Description :
 * Function responsible for initialize the ADC driver.
 */
/* ----------------------------------------------------------------------------
[Function Name] : adc_init
[Description] : This function is responsible initializing the ADC peripheral
-------------------------------------------------------------------------------*/
void adc_init(void);

/*
 * Description :
 * Function responsible for read analog data from a certain ADC channel
 * and convert it to digital using the ADC driver.
 */
/* ----------------------------------------------------------------------------
[Function Name] : adc_convert
[Description] : Function responsible for performing an analog conversion without
	polling for returning the output .. usually used in interrupt mode to launch
	a conversion and then at the isr the result is read directly  
[Args] : 
[in] -unint8_t channel_num
	this ARG shall indicate the analog argument
-----------------------------------------------------------------------------*/
void adc_convert (unint8_t channel_num);

/* ----------------------------------------------------------------------------
[Function Name] : adc_read
[Description] : Function responsible for performing an analog conversion without
	polling or returning the output .. usually used in interrupt mode to launch
	a conversion and then at the isr the result is read directly  

-----------------------------------------------------------------------------*/
uint16_t adc_read (uint8_t channel_num);

void adc_set_isr(void ( * adc_int_function)(void));

void adc_set_int (bool b_state);

#endif /* ADC_LIB_H_ */
