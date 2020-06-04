/* --------------------------------------------------------------------------
[FILE NAME]:     hcsr04.h

[AUTHOR]:        Ali MAkhlouf

[DATE CREATED]:  4/15/2019 07:12:54 AM

[DESCRIPTION]:   a library header file for controlling and reading sensor
				 values from a MQ2 sensor
-----------------------------------------------------------------------------*/

#ifndef HCSR04_H_
#define HCSR04_H_

#include "micro_config.h"
#include "dio_api.h"
#include "stdbool.h"
#include "delay_api.h"

//#define HCSR04_TR_PIN 0

#define HCSR04_TR_PIN 16

#define HCSR04_DATA_READY (echoing  == 2)

#define HCSR04_NO_OP 0
#define HCSR04_START_OP 1
#define HCSR04_COMPLETE_OP 2

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

/* ----------------------------------------------------------------------------
[Function Name] : hcsr04_init
[Description] : This function is responsible initializing the MCU for the module
by configuring the trigger pin in MCU and initializing input capture of a timer. 
-------------------------------------------------------------------------------*/
void hcsr04_init ();

/* ----------------------------------------------------------------------------
[Function Name] : hcsr04_cm
[Description] : This function is responsible for measuring the distance in cm unit.
[Returns] : this return shall indicate the distance in cm
-------------------------------------------------------------------------------*/
uint16_t hcsr04_cm ();

/* ----------------------------------------------------------------------------
[Function Name] : hcsr04_trigger
[Description] : This function is responsible triggering the ultrasonic signal 
-------------------------------------------------------------------------------*/
void hcsr04_trigger ();

/* ----------------------------------------------------------------------------
[Function Name] : hcsr04_trigger
[Description] : This function is responsible triggering the ultrasonic signal
-------------------------------------------------------------------------------*/
bool hcsr04_data_ready ();


#endif /* HCSR04_H_ */