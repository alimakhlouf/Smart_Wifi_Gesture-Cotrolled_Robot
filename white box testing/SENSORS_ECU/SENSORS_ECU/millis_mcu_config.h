/* ---------------------------------------------------------------------------------------------------
[FILE NAME]: millis.h
[AUTHOR(S)]: Ali Makhlouf
[DATE CREATED]: 12/5/2017
[DESCRIPTION]: a library c header file for delay using timer0 either using polling or interrupt
			  unlike _delay_ms() ,it can also monitor a specified event or code during the
			  delay and if the action happened it jumps out of the delay
--------------------------------------------------------------------------------------------------- */

#ifndef MILLIS0_H_
#define MILLIS0_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"


//****************************LIBRARY SETTINGS******************** /
/****CHOOSE MILLIS FUNCTION INTERRUPT OR POLLING********
 * 0 POLLING
 * 1 INTERRUPT
 */

#define MILLIS_INTERRUPT 0

/********************************************************/




#define FALSE 0
#define TRUE 1

#define DELAY_TIME_IN_US(t) ((unint32_t)t * 1000) /*it takes the same argument of millis(uint16 t) which holds the needed delay time 
												    we multiply the delay time by 1000 to convert it to microseconds*/


#if defined(ATMEGA8) || defined(ATMEGA16) || defined(ATMEGA32)

//the following configurations is related to the timer in microseconds
#define TICK_TIME_1024_PRESCALER (( (unint32_t)1024 * 1000000 / F_CPU)) /*the time between each timer clock tick and the next one in
																		 microseconds (the same meaning as the time of 1024 cycles in
																		  microseconds because we are choosing 1024 prescaler)
																		*/
//the following macros perform the same function as millis() but on us(you can call it micros() :)
/*there are several reasons to choose the 1024 pre scaler:
 *1- because between each tick the program is going to execute some code so
 *taking 1024 pre scaler insures that it can monitor a code up to 1024 cycles between each tick
 *and the other
 *
 *2- because this reduces the ranges of the results in the timer configuration formulas we use (dividing by 1024)
 */
#define TIME_OF_ONE_OVF ((unint32_t)TICK_TIME_1024_PRESCALER * 256) // the time from TCNT0=0 to overflow in microseconds


////****************************************************************************************


#define NUMBER_OF_OVFLOWS (DELAY_TIME_IN_US(t) / TIME_OF_ONE_OVF )//number of overflows to finish the delay time

#define REMAINING_TICKS ((DELAY_TIME_IN_US(t) % TIME_OF_ONE_OVF) / (TICK_TIME_1024_PRESCALER)) //after finishing all the overflow there will be some remaining ticks which come from the fraction of the previous macro (NO_OF_OVF)







/* ---------------------------------------------------------------------------------------------------------------
[Function Name] : millis_interrupt
[Description] : This function is responsible for setting timer0 to keep counting(and returning true indicating not finished yet) 
				until it finishes the delay time then it stops(and returns zero indicating finished)
				while delaying you can monitor some code(action or event) that if they happen get out of the delay and continue
				this uses interrupt instead of polling(even that their functions are identical  exactly doing the same)
				but the only problem of the polling is that the code they monitor must not exceed 1024 CPU cycle time because if so 
				it will ignore some overflows because the code will take time longer than time between a clock tick and the next one
				so if you are using delays in the monitored code or any thing that will exceeds the 1024 CPU cycle time use this 
				function
[Args] :
[int] -uint16 t
this ARG is responsible for holding delay time in milliseconds

[Returns] :TRUE if the timer is still working ,FALSE if the timer finished counting
---------------------------------------------------------------------------------------------------------------*/


#ifdef ATMEGA8
#define CONFIG_TMR0_OVF_ISR   __vector_9
void CONFIG_TMR0_OVF_ISR(void) __attribute__((signal,__INTR_ATTRS));

#elif defined(ATMEGA32)
#define CONFIG_TMR0_OVF_ISR   __vector_11
void CONFIG_TMR0_OVF_ISR(void) __attribute__((signal,__INTR_ATTRS));

#elif defined(ATMEGA16)
#define CONFIG_TMR0_OVF_ISR   __vector_9
void CONFIG_TMR0_OVF_ISR(void) __attribute__((signal,__INTR_ATTRS));
#endif
	


#endif

/* ---------------------------------------------------------------------------------------------------------------
[Function Name] : millis_interrupt
[Description] : This function is responsible for setting timer0 to keep counting(and returning true indicating not finished yet) 
				until it finishes the delay time then it stops(and returns false indicating finished)
				while delaying you can monitor some code(action or event) that if the event occured get out of the millis function 
				and continue your program
				this uses polling instead of interrupt(their functions are identical exactly doing the same)
				but the only problem of the polling is that the code they monitor must not exceed 1024 CPU cycle time because if so
				it will ignore some overflows because the code will take time longer than time between a clock tick and the next one
				so if you are using delays in the monitored code or any thing that will exceeds the 1024 CPU cycle time use the
				interrupt function
				but this function is more accurate than the interrupt function as there will be no interrupt latency
[Args] :
[int] -uint16 t
this ARG is responsible for holding delay time in milliseconds

[Returns] :TRUE if the timer is still working ,FALSE if the timer finished counting
---------------------------------------------------------------------------------------------------------------*/
unint8_t millis (unint16_t t); //USES 2BYTE STATIC VARIABLE



void reset_timer (uint16_t t);
void millis_disable_tmr ();
void millis_reset_ticks ();
void millis_reset_tmr (uint16_t t);
void millis_add_ovf_isr (void ( * additional_tmr_ovf_isr)(void));

unint16_t n_ovf;



#endif /* MILLIS0_H_ */