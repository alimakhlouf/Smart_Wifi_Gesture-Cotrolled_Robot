/*
 * button.c
 *
 * Created: 5/11/2019 04:20:27 م
 *  Author: hppp
 */ 

#include "button.h"

//static uint8_t is_port_pin_pd_pressed (volatile uint8_t * pin_reg, uint8_t pin_no);
//static uint8_t is_port_pin_pu_pressed (volatile uint8_t * pin_reg, uint8_t pin_no);

uint8_t button_state(uint8_t PU_PD, uint8_t dio_pin_no)
{
	
	if (dio_pin_no >= 15)
	{
		dio_pin_no++ ; // because there is no PC7 .. so we move directly to PORTD
	}
	
	uint8_t PinPort = dio_pin_no / No_PinPerPort;
	uint8_t PinNo_InPort = MODULO(dio_pin_no, No_PinPerPort);
	uint8_t button_state = 0;
	
	switch(PinPort)
	{
		case 0 :
			button_state = (PU_PD == PULL_DOWN)? is_port_pin_pd_pressed(&PINB, PinNo_InPort) : is_port_pin_pu_pressed(&PINB, PinNo_InPort);
			break;
		case 1 :
#if 0
			if (PU_PD == PULL_DOWN)
			{
				button_state = is_port_pin_pd_pressed(&PINC, PinNo_InPort);
				
			}
			else
			{
				button_state = is_port_pin_pu_pressed(&PINC, PinNo_InPort);
			}
#endif
			button_state = (PU_PD == PULL_DOWN)? is_port_pin_pd_pressed(&PINC, PinNo_InPort) : is_port_pin_pu_pressed(&PINC, PinNo_InPort);
			break;
		case 2 :
			
			button_state = (PU_PD == PULL_DOWN)? is_port_pin_pd_pressed(&PIND, PinNo_InPort) : is_port_pin_pu_pressed(&PIND, PinNo_InPort);
			break;
	}
	
	return button_state; 
}

 uint8_t is_port_pin_pd_pressed(volatile uint8_t * port_reg, uint8_t pin_no)
{
	uint8_t pressed = 5;
	uint16_t pressed_confidence_level = 0;
	uint16_t released_confidence_level = 0;
	
	while (pressed == 5)
	{
		
		if (BIT_IS_SET(*port_reg, pin_no))
		{
			released_confidence_level = 0;
			pressed_confidence_level++ ;
			
			if (pressed_confidence_level > 500)
			{
				pressed = 1;
				pressed_confidence_level = 0;
			}
			
		}
		else
		{
			released_confidence_level++ ;
			pressed_confidence_level = 0;
			
			if (released_confidence_level > 500)
			{
				pressed = 0;
				released_confidence_level = 0;
			}
			
		}
		
	}	
	
	return pressed;
	
}

uint8_t is_port_pin_pu_pressed(volatile uint8_t * port_reg, uint8_t pin_no)
{
	uint8_t pressed = 5; // any default value
	uint16_t pressed_confidence_level = 0;
	uint16_t released_confidence_level = 0;
	
	
	
	while (pressed == 5)
	{
		if (BIT_IS_CLEAR(*port_reg, pin_no))
		{
			released_confidence_level = 0;
			pressed_confidence_level++ ;
			if (pressed_confidence_level > 500)
			{	
				pressed = 1;
				pressed_confidence_level = 0;
			}
		}
		else
		{
			released_confidence_level++ ;
			pressed_confidence_level = 0;
			
			if (released_confidence_level > 500)
			{
				pressed = 0;
				released_confidence_level = 0;
			}
		}
	}
	
	return pressed;
	
}