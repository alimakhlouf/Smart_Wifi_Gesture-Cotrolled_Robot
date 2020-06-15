/* ---------------------------------------------------------------------------------------------------
[FILE NAME]: millis.h
[AUTHOR(S)]: Ali Makhlouf
[DATE CREATED]: 12/5/2017
[DESCRIPTION]: a library c header file for delay using timer0 either using polling or interrupt
			  unlike delay_msec() ,it can also monitor a specified event or code during the
			  delay and if the action happened it jumps out of the delay
--------------------------------------------------------------------------------------------------- */

#ifndef MILLIS0_H_
#define MILLIS0_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"
#include "stdbool.h"
#include "dio_api.h"
#include "millis_api.h"

#define FALSE 0
#define TRUE 1



#define DELAY_TIME_IN_US(a_t) ((unint32_t)(a_t) * 1000) /*it takes the same argument of millis(uint16 t) which holds the needed delay time 
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

/* ----------------------------------------------------------------------------
[Function Name] : millis
[Description] : This function is responsible for utilizing a timer to keep
counting(and returning true indicating not finished yet) until it finishes the
delay time then it stops(and returns false indicating finished) while delaying
you can monitor some code(action or event) that if the event occurred get out
of the millis function and continue your program.You can use either polling or
interrupt(their functions are identical exactly doing the same)
polling is more accurate than the interrupt function as there will be no
interrupt latency, but sometimes an interrupt may be needed for in some
applications

[Args] :
[int] -uint16 a_t
this ARG is responsible for holding delay time in milliseconds

[Returns] :true if the timer is still working ,false if the timer finished
counting
-----------------------------------------------------------------------------*/
bool millis (unint16_t t); //USES 2BYTE STATIC VARIABLE


/* ----------------------------------------------------------------------------
[MACRO Name] : millis_disable_tmr
[Description] : This function is responsible for stopping the timer from
counting, clearing the timer register and clearing the overflow counter(n_ovf)

[NOTE]: typically it's used when the the event that the timer is monitoring
happens so you quit the millis function and then you have to disable the
counting process, this is when you use this function.

-----------------------------------------------------------------------------*/
void millis_disable_tmr ();

/* ----------------------------------------------------------------------------
[MACRO Name] : millis_reset_ticks
[Description] : This function is responsible for reseting the timer counter
register ticks of the current n_ovf counter
------------------------------------------------------------------------------*/
void millis_reset_ticks ();

/* ----------------------------------------------------------------------------
[Function Name] : millis_reset_tmr
[Description] : This function is responsible for making the timer start
counting from the start

[Args] :
[int] -uint16 a_t
this ARG is responsible for holding delay time in milliseconds(it's needed)
-----------------------------------------------------------------------------*/
void millis_reset_tmr (uint16_t a_t);

/* ----------------------------------------------------------------------------
[MACRO Name] : millis_add_ovf_isr
[Description] :This function is responsible adding an action that happens when
the when the timer overflow interrupt happens(in addition to the millis interrupt
handler that is already in the ISR). As when using this library you can't make a
separate overflow ISR handler(because it's used in the millis interrupt), what
you can do is add a function that can be called whenever an the overflow happens
and adjust your program algorithm based on that
------------------------------------------------------------------------------*/
void millis_add_ovf_isr (void ( * additional_tmr_ovf_isr)(void));





#endif /* MILLIS0_H_ */