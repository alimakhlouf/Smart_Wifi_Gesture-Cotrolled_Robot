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

#define MILLIS_INTERRUPT 1

/********************************************************/




#define FALSE 0
#define TRUE 1

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

#define DELAY_TIME_IN_US(t) ((unint32_t)t * 1000) /*it takes the same argument of millis(uint16 t) which holds the needed delay time 
												    we multiply the delay time by 1000 to convert it to microseconds*/
////****************************************************************************************


#define NUMBER_OF_OVFLOWS  ( DELAY_TIME_IN_US(t) / TIME_OF_ONE_OVF )//number of overflows needed to reach the delay time

#define REMAINING_TICKS ((DELAY_TIME_IN_US(t) % TIME_OF_ONE_OVF) / (TICK_TIME_1024_PRESCALER)) //after finishing all the overflow there will be some remaining ticks which come from the fraction of the previous macro (NO_OF_OVF)




/* ---------------------------------------------------------------------------------------------------------------
[MACRO Name] : STOR_TIMER0
[Description] : This function is responsible for stopping the timer from counting (by clearing TCCR0 register), clearing the timer register(TCNT0) and clearing the overflow counter(n_ovf)
				normally when the function finishes it time the registers in this functions are cleared by default but if the action the delay function is monitoring while delaying activates the function will finish before clearing any thing
				so this function must be called after the mills function if there is some code to be monitored that may cause exit the function before the time finishes
				,in other cases when millis is used as a normal delay function without any code to monitor you don't have to use it as the registers will be cleared after the time finishes
[Args] :
[out]:-(*(volatile uint8_t *)((0x33) + 0x20)) TCCR0
this ARG is the control register of timer 0 and when cleared (especially the prescaler bits in it) the timer stop(is disabled)
-(*(volatile uint8_t *)((0x32) + 0x20)) TCNT0
this ARG is the timer register of timer 0 and is cleared to reset for next delay functions
-uint8 n_ovf
this ARG is the overflow counter which is responsible of holding the number of remaining overflows until it reached zero then the counter stop but in this
---------------------------------------------------------------------------------------------------------------*/
#define DISABLE_TIMER0 {\
						TCCR0 = 0; \
						TCNT0 = 0; \
						n_ovf = 0; \
					}


#define RESET_TICKS (TCNT0 = 0)


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
//unint8_t millis_interrupt(unint16_t t);//USES 3 BYTE STATIC VARIABLES
								 //THIS FUNCTION YOU CAN USE DELAYS in the code that is monitored
								 // DON'T FORGET TO ACTIVATE THE I BIT .. externally
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
unint8_t millis(unint16_t t); //USES 2BYTE STATIC VARIABLE
	
	
volatile unint16_t n_ovf;

#endif /* MILLIS0_H_ */