/*
 * DIO_prog.c
 *
 * Created: 5/15/2019 03:35:22 م
 * Author : hppp
 */ 

#include "micro_config.h"
#include "DIO.h"
#include "button.h"


int main(void)
{
	
	DDRB |= 0b00000010;

	#if 0
DIO_SetPinDir(DIO_PIN13, DIO_OUTPUT);
	
	DIO_SetPinDir(DIO_PIN12, DIO_INPUT);
	DIO_SetPinVal(DIO_PIN12, DIO_HIGH);

	
	_delay_ms(1000);
	DIO_SetPinVal(DIO_PIN13, DIO_HIGH);
	_delay_ms(1000);
	DIO_SetPinVal(DIO_PIN13, DIO_LOW);
	_delay_ms(1000);
	DIO_SetPinVal(DIO_PIN13, DIO_HIGH);
	_delay_ms(1000);
	DIO_SetPinVal(DIO_PIN13, DIO_LOW);
	_delay_ms(1000);
	DIO_SetPinVal(DIO_PIN13, DIO_HIGH);
	_delay_ms(1000);
	DIO_SetPinVal(DIO_PIN13, DIO_LOW);
	_delay_ms(1000);
	DIO_SetPinVal(DIO_PIN13, DIO_HIGH);
	_delay_ms(1000);
	DIO_SetPinVal(DIO_PIN13, DIO_LOW);
#endif
	
    /* Replace with your application code */
    while (1) 
    {
		PORTB |= 0b00000010;
		_delay_ms(1000);
		PORTB &= 0b11111101;
		_delay_ms(1000);
		
// 		if (button_state (PULL_UP, DIO_PIN12)) //is_port_pin_pu_pressed  //button_state (PULL_UP, DIO_PIN12)
// 		{
// 			DIO_SetPinVal(DIO_PIN13, DIO_HIGH);
// 		}
// 		else
// 		{
// 			DIO_SetPinVal(DIO_PIN13, DIO_LOW);
// 		}
		
    }
}

