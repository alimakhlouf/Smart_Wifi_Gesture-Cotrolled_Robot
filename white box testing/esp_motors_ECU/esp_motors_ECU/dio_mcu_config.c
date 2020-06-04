/* ----------------------------------------------------------------------------
[FILE NAME]:    dio_mcu_config.c

[AUTHOR]:       Ali MAkhlouf

[DATE CREATED]: 5/29/2019 04:30:01

[DESCRIPTION]:  this file contains functions that handles the MCU
                digital I/O pins .. it exists in the HAL layer and contains 
				different versions of code for different MCUs
-----------------------------------------------------------------------------*/
#include "dio_mcu_config.h"

// the following version of code for ATMEGA32-16-8
#if defined(ATMEGA8) || defined(ATMEGA32) || defined(ATMEGA16)

void dio_set_pin_dir(uint8_t a_pin_no, uint8_t a_pin_dir)
{
	//determine the port that was addressed from the dio_pin number 
	uint8_t pin_port= a_pin_no / NUM_PIN_PER_PORT;
	//determine which the port that was addressed from the dio_pin number
	uint8_t Pin_no_in_port = MODULO(a_pin_no, NUM_PIN_PER_PORT);
	
	//of the direction is either input or output configure it
	if (a_pin_dir != DIO_PULLUP)
	{
		//put this direction in the specific pin_no in the specific port 
		ASSIGN_BIT(*(DDR_REG_BASE + PORT_OFFSET(pin_port)), Pin_no_in_port, 
		             a_pin_dir);
	}
	else //if pull up then make it input and put high to the PORT reg
	{ 
		// assign pin as input
		ASSIGN_BIT(*(DDR_REG_BASE + PORT_OFFSET(pin_port)), Pin_no_in_port, 0); 
		// put 1 to port pin to make it pull up
		ASSIGN_BIT(*(PORT_REG_BASE + PORT_OFFSET(pin_port)), Pin_no_in_port, 1); 
	}
	
}

bool dio_get_pin(uint8_t a_pin_no)
{
	//determine the port that was addressed from the dio_pin number 
	uint8_t pin_port= a_pin_no / NUM_PIN_PER_PORT;
	//determine which the port that was addressed from the dio_pin number
	uint8_t Pin_no_in_port = MODULO(a_pin_no, NUM_PIN_PER_PORT);
	//return the state of the pin 	
	return (BIT_IS_SET(*(PIN_REG_BASE + PORT_OFFSET(pin_port)), Pin_no_in_port));
}


void dio_set_pin_val(uint8_t a_pin_no, uint8_t PinVal)
{
	//determine the port that was addressed from the dio_pin number
	uint8_t pin_port = a_pin_no / NUM_PIN_PER_PORT;
	//determine which the port that was addressed from the dio_pin number
	uint8_t Pin_no_in_port = MODULO(a_pin_no, NUM_PIN_PER_PORT);
	
	//assign the value to the specific pin_no in the specific port 
	ASSIGN_BIT(*(PORT_REG_BASE + PORT_OFFSET(pin_port)), Pin_no_in_port, PinVal);

}

void dio_togg_pin(uint8_t a_pin_no)
{
	//determine the port that was addressed from the dio_pin number
	uint8_t pin_port = a_pin_no / NUM_PIN_PER_PORT;
	//determine which the port that was addressed from the dio_pin number
	uint8_t Pin_no_in_port = MODULO(a_pin_no, NUM_PIN_PER_PORT);
	
	//toggle the specific pin_no in the specific port 
	TOGGLE_BIT(*(PORT_REG_BASE + PORT_OFFSET(pin_port)), Pin_no_in_port);

}
#endif
