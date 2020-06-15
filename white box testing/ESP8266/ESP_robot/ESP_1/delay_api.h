/* ----------------------------------------------------------------------------
[FILE NAME]:    delay_api.h

[AUTHOR]:       Ali MAkhlouf

[DATE CREATED]: 6/13/2019 03:05:35

[DESCRIPTION]:  this file contains functions that perform software delay, both
                in microseconds, milliseconds
------------------------------------------------------------------------------*/
 
#ifndef DELAY_API_H_
#define DELAY_API_H_


#include "micro_config.h"
#include "math.h"
#include "delay.h"

/* ----------------------------------------------------------------------------
[Function Name] : delay_usec
[Description] : This function is responsible performing a software delay in 
micro seconds.
[Args] :
[in] double a_us_delay :
This Arg shall indicate the delay time in micro seconds
---------------------------------------------------------------------------- */
static inline void delay_usec(double a_us_delay)
{
	// call the delay micro function that performs the actual delay 
	//it's has a specific copies codes for different MCU architectures  
 	delay_micro(a_us_delay);
}

/* ----------------------------------------------------------------------------
[Function Name] : delay_msec
[Description] : This function is responsible performing a software delay in 
                milliseconds.
[Args] :
[in] double a_ms_delay :
This Arg shall indicate the delay time in milliseconds
[NOTE]: the maximum delay achievable by this function is 65535 milliseconds. if
you go higher than that the resulted delay will be wrong(much shorter than the
actual time
---------------------------------------------------------------------------- */
static inline void delay_msec(double a_ms_delay)
{
	// call the delay milliseconds function that performs the actual delay
	//it's has a specific copies codes for different MCU architectures
	delay_milli(a_ms_delay);
}


#endif /* DELAY_API_H_ */