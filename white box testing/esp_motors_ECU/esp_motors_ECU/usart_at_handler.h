/* ----------------------------------------------------------------------------
[FILE NAME]: uart_at_handler.h

[AUTHOR]: Ali Makhlouf

[DATE CREATED]:  6/14/2019 10:58:11 ص

[DESCRIPTION]: a library c file for initiating and operating PWM peripherals
-------------------------------------------------------------------------------*/

#ifndef UART_AT_HANDLER_H_
#define UART_AT_HANDLER_H_

#include "micro_config.h"
#include "millis_api.h"
#include "uart_api.h"
#include <stdarg.h> 
#include <stdbool.h>

/* ----------------------------------------------------------------------------
[Function Name] : wait_to_finish
[Description] : This function flushes out any stream of uart data that're 
                arriving until the end of this stream
[Args] :
[int] -uint16 a_time
	this ARG indicates timeout in msec ((it resets after each frame received so the 
	delay is suitable for the time between two consecutive frames(atmost 5 msec)
-----------------------------------------------------------------------------*/
void wait_to_finish(unint8_t a_time);



/* ----------------------------------------------------------------------------
[Function Name] : get_str_until
[Description] : This function keeps storing any arriving data until a specific
string is found, it exits when str is found or timeout 
[Args] :
[out]-char * ap_dest_str
	this ARG indicates the location where the stream of data between the 2 strings
	will be stored
[in] -char * ap_afterstr
	this ARG indicates the end string which the function finishes storing and
	returns upon finding it 
[NOTE]: passing a NULL in this argument makes the function keeps storing until 
		timeout
[in] -uint16 a_time
	this ARG indicates delay time in milliseconds for this function to keep 
	storing and it
[NOTE] :
	the timeout is for (((the whole stream, meaning that it doesn't
	restart timing after each frame received but it's one count for all the stream
	unlike the wait_to_finish function, so the delay might be in range of hundreds
	of milliseconds))
-----------------------------------------------------------------------------*/
bool get_str_until(char * ap_dest_str, char * ap_afterstr, unint16_t a_time);

/* ----------------------------------------------------------------------------
[Function Name] : get_string
[Description] : This function deals with arriving uart frames and stores a 
stream of a data existing between 2 strings and it returns upon either finding 
the ending string or timeout
[Args] :
[out]-char * ap_dest_str
	this ARG indicates the location where the stream of data between the 2 string
	will be stored 
[in] -char * ap_beforestr
	this ARG indicates the end string which the function starts storing and upon 
	finding it
[in] -char * ap_afterstr
	this ARG indicates the end string which the function finishes storing and
	returns upon finding it
[NOTE]: passing a NULL in this argument makes the function keeps storing until
timeout
[in] -uint16 a_time
	this ARG indicates timeout in milliseconds
[Returns]: this return indicates whether the storing operation was successful
		   or not 
[NOTE] : 
	the delay time is for (((the whole stream, meaning that it doesn't
	restart timing after each frame received but it's one count for all the stream
	unlike the wait_to_finish function, so the delay might be in range of tens 
	or hundreds of milliseconds))
-----------------------------------------------------------------------------*/
bool get_string(char * ap_dest_str, char * ap_beforestr, char * afterstr, 
                        unint16_t a_time);

/* ----------------------------------------------------------------------------
[Function Name] : is_str_reached
[Description] : This function is responsible for checking whether the currently
received frame signifies that the string was reached or is not reached yet 
[Args] :
[in] -char * ap_str
	this ARG indicates the string to be checked
[in] -unint8_t a_tmp1
	this ARG holds the char that will be compared to the string characters
[in] -unint8_t * a_tmp
	this ARG tracks the matching between the string and the stream of arriving data
	every time a char matches it increments until it points at the NULL character
	which means the end of the string(match occurred) 
[Returns]: this return indicates whether the string was reached or not
true : string found
false: string not found yet
[NOTE] : 
	the delay time is for (((the whole stream, meaning that it doesn't
	restart timing after each frame received but it's one count for all the stream
	unlike the wait_to_finish function, so the delay might be in range of tens
	or hundreds of milliseconds))
-----------------------------------------------------------------------------*/
bool is_str_reached(char * ap_str, unint8_t a_tmp1, unint8_t * a_tmp);

/* ----------------------------------------------------------------------------
[Function Name] : check_strings
[Description] : This function checks for one or more strings whether they exist 
	in the arriving data or not. Once it finds a string it returns true immediately	
	otherwise it continues until timeout.
[NOTE]: this function is a variable parameter function
[Args] :
[in] -uint16 a_time
	this ARG indicates delay time in milliseconds
[in] -int16_t a_time
	this ARG indicates the number of strings to be checked
[in] -variable parameters
	the strings are entered here one by one separated by a comma
[Returns] : this returns the state or the result of the process 
	0 : if timeout and no string found
	otherwise the number of the string that's been found(the number is it's order
	in the arguments starting from 1) 
[NOTE] : 
	the delay time is for (((the whole stream, meaning that it doesn't
	restart timing after each frame received but it's one count for all the stream
	unlike the wait_to_finish function, so the delay might be in range of hundreds
	of milliseconds))
-----------------------------------------------------------------------------*/
uint8_t check_strings (unint16_t a_time, int16_t a_str_count, ...);



#endif /* UART_AT_HANDLER_H_ */