/* --------------------------------------------------------------------------
[FILE NAME]:     74HC595.h

[AUTHOR]:        Ali MAkhlouf

[DATE CREATED]:  6/4/2019 02:16:18 PM

[DESCRIPTION]:   a library header file for controlling sipo register 74HC595
----------------------------------------------------------------------------*/

#ifndef SIPO_H
#define SIPO_H

#include "common_macros.h"
#include "std_types.h"
#include "micro_config.h"
#include "dio_api.h"

#define SH_CP_PIN 9 // shift pin
#define DS_PIN	  11 // data pin
#define ST_CP_PIN 10 // store pin


#if 0
#define SERIAL_PORT PORTC
#define SERIAL_DDR DDRC
#define SH_CP 0 //shift clock pulse
#define DS	2 // data pin
#define ST_CP 1 //store clock pulse
#endif

/* --------------------------------------------------------------------------
[Function Name] : spc_init
[Description] : This function is responsible for initializing pins connected  
	to 74HC595 register
-----------------------------------------------------------------------------*/
void spc_init ();

/* ----------------------------------------------------------------------------
[Function Name] : spc_send
[Description] : This function is responsible for initializing pins connected to 
	74HC595 register
[Args] : 
[in] -uint8_t data
	this ARG shall indicate the data that shall be appear on the register output
[in] -uint8_t out_pins_count
	this ARG shall indicate how many pins are you using from the 8 pins output 
	pins
[NOTE] : when using less than 8 pins (eg.6) .. the bits must be the least 6 pins
	you mustn't skip some pins in the first and use some higher pins 
-------------------------------------------------------------------------------*/
void spc_send (uint8_t data, uint8_t out_pins_count);


#endif /* 74HC595_H_ */
