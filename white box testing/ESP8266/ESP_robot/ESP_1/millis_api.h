
/* ----------------------------------------------------------------------------
[FILE NAME]:    millis_api.h

[AUTHOR]:       Ali MAkhlouf

[DATE CREATED]:  6/4/2019 02:16:18 م

[DESCRIPTION]:  this file contains configurations and function prototypes for
                handling the making an event monitoring function using a 
				hardware timer
----------------------------------------------------------------------------*/

#ifndef MILLIS_API_H_
#define MILLIS_API_H_

#include "millis_mcu_config.h"

/****CHOOSE MILLIS FUNCTION INTERRUPT OR POLLING********
 * 0 POLLING
 * 1 INTERRUPT
 */

#define MILLIS_INTERRUPT 0

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
bool millis (unint16_t a_t); //USES 2BYTE STATIC VARIABLE


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
[MACRO Name] : millis_add_ovf_isr
[Description] :This function is responsible adding an action that happens when 
the when the timer overflow interrupt happens(in addition to the millis interrupt
handler that is already in the ISR). As when using this library you can't make a
separate overflow ISR handler(because it's used in the millis interrupt), what 
you can do is add a function that can be called whenever an the overflow happens
and adjust your program algorithm based on that
------------------------------------------------------------------------------*/
void millis_add_ovf_isr (void ( * ap_additional_tmr_ovf_isr)(void));


#endif /* MILLIS_API_H_ */