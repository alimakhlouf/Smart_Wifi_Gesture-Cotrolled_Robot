/*
 * ADC_lib.h
 *
 *  Created on: ? ÔÚÈÇä¡ ???? åÜ
 *      Author: hppp
 */

#ifndef ADC_LIB_H_
#define ADC_LIB_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"


//********************ADC CONFIGURATIONS**************************
/*ANALOG SIGNAL UPPER LIMIT SOURCE
AREF_PIN 0
AVCC_PIN 1
INTERNAL_SOURCE_2.56V 3
CHOOSE ONE OF THE ABOVE IN THE FOLLOWING CONFIGURAION*/
#define UPPER_LIMIT_SOURCE  1

/*CHOOSE THE RESULT BIT ALIGNMENT
 *LEFT_ADJUSTED_RESULT 1
 *RIGHT_ADJSUTED_RESULT 0
 *CHOOSE ONE OF THE ABOVE IN THE FOLLOWING CONFIGURAION
 */
#define ADJUST_RESULT_BITS 0


/* 1 for enable ---0 for disable*/
#define ADC_INT 0
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
#if(ADJUST_RESULT_BITS == 0) /*right adjusted*/
#define GET_ADC_RESULT (ADCL + ((unint16_t)ADCH << 8))
#else /*LEFT ADJUSTED*/
#define GET_ADC_RESULT ((ADCL >> 6) + ((unint16_t)ADCH << 2))
#endif

/*
 * Description :
 * Function responsible for initialize the ADC driver.
 */
void ADC_init(void);

/*
 * Description :
 * Function responsible for read analog data from a certain ADC channel
 * and convert it to digital using the ADC driver.
 */
#if(ADC_INT == 0) //if an interrupt is used
unint16_t ADC_read(unint8_t channel_num);
#else
unint16_t ADC_read(unint8_t channel_num);
volatile extern unint8_t adc_flag; /*flag indicating ADC interrupt happend*/
#endif



#endif /* ADC_LIB_H_ */
