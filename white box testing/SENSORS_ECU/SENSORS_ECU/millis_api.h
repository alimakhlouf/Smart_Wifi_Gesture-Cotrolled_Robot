/*
 * millis_api.h
 *
 * Created: 6/4/2019 02:16:18 م
 *  Author: hppp
 */ 


#ifndef MILLIS_API_H_
#define MILLIS_API_H_

#include "millis_mcu_config.h"


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


/* ---------------------------------------------------------------------------------------------------------------
[MACRO Name] : DISABLE_TIMER0
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
void millis_disable_tmr ();

void millis_reset_ticks ();

void millis_reset_tmr (uint16_t t);

void millis_add_ovf_isr (void ( * additional_tmr_ovf_isr)(void));



#endif /* MILLIS_API_H_ */