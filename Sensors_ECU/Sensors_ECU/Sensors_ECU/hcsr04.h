/*
 * HCSR04.h
 *
 * Created: 4/15/2019 07:12:54 ص
 *  Author: hppp
 */ 


#ifndef HCSR04_H_
#define HCSR04_H_

#include "micro_config.h"
#include "dio_api.h"
#include "stdbool.h"
#include "delay_api.h"

//#define HCSR04_TR_PIN 0

#define HCSR04_TR_PIN 16

#define HCSR04_DATA_READY (echoing  == 2)

#ifdef ATMEGA8
#define CONFIG_TMR1_CAPT_ISR   __vector_5
void CONFIG_TMR1_CAPT_ISR(void) __attribute__((signal,__INTR_ATTRS));
#define CONFIG_TMR1_OVF_ISR   __vector_8
void CONFIG_TMR1_OVF_ISR(void) __attribute__((signal,__INTR_ATTRS));

#elif defined(ATMEGA32)
#define CONFIG_TMR1_CAPT_ISR   __vector_6
void CONFIG_TMR1_CAPT_ISR(void) __attribute__((signal,__INTR_ATTRS));
#define CONFIG_TMR1_OVF_ISR   __vector_9
void CONFIG_TMR1_OVF_ISR(void) __attribute__((signal,__INTR_ATTRS));

#elif defined(ATMEGA16)
#define CONFIG_TMR1_CAPT_ISR   __vector_5
void CONFIG_TMR1_CAPT_ISR(void) __attribute__((signal,__INTR_ATTRS));
#define CONFIG_TMR1_OVF_ISR   __vector_8
void CONFIG_TMR1_OVF_ISR(void) __attribute__((signal,__INTR_ATTRS));
#endif

uint16_t hcsr04_cm ();
void hcsr04_trigger ();
void hcsr04_init ();
bool hcsr04_data_ready ();


#endif /* HCSR04_H_ */