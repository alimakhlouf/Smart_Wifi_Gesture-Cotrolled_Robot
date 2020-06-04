/* --------------------------------------------------------------------------
[FILE NAME]:     button.c

[AUTHOR]:        Ali MAkhlouf

[DATE CREATED]:  5/11/2019 04:20:27 PM

[DESCRIPTION]:   a library C file for checking Button status and avoiding
	button debounce using software(no delays)
----------------------------------------------------------------------------*/

#include "button.h"

/* ----------------------------------------------------------------------------
[Function Name] : is_port_pin_pd_pressed
[Description] : This function is getting the state of the button(pushed_released) 
	in a pull down mode
[Args] :
[in] -volatile uint8_t * ap_pin_reg
	this ARG shall indicate the address of the port's PIN register which the button
	is connected to one of it's registers
[in] -uint8_t a_pin_no
	this ARG shall indicate the pin no in the port which the button is connected to
[Returns] : this return shall indicate the state of the button(pushed_released)
------------------------------------------------------------------------------*/
static bool is_port_pin_pd_pressed (volatile uint8_t * ap_pin_reg, uint8_t a_pin_no);

/* ----------------------------------------------------------------------------
[Function Name] : is_port_pin_pu_pressed
[Description] : This function is getting the state of the button(pushed_released) 
	in a pull up mode
[Args] :
[in] -volatile uint8_t * ap_pin_reg
	this ARG shall indicate the address of the port's PIN register which the button
	is connected to one of it's registers
[in] -uint8_t a_pin_no
	this ARG shall indicate the pin no in the port which the button is connected to
[Returns] : this return shall indicate the state of the button(pushed_released)
------------------------------------------------------------------------------*/
static bool is_port_pin_pu_pressed (volatile uint8_t * ap_pin_reg, uint8_t a_pin_no);


bool button_state(uint8_t a_pu_pd, uint8_t a_dio_pin_no)
{
	
	//get the port and the pin location of the port from the dio pin value 
	uint8_t port_no = a_dio_pin_no / NUM_PIN_PER_PORT;
	uint8_t pin_no_in_port = MODULO(a_dio_pin_no, NUM_PIN_PER_PORT);
	
	//get the port address
	volatile uint8_t * port_address = PIN_REG_BASE + PIN_REG_OFFSET(port_no);
	
	bool button_state = false;
	if (a_pu_pd == PULL_DOWN)
	{
		button_state = is_port_pin_pd_pressed(port_address, pin_no_in_port);
	}
	else
	{
		button_state = is_port_pin_pu_pressed(port_address, pin_no_in_port);
	}
	
	return button_state; 
}


static bool is_port_pin_pd_pressed(volatile uint8_t * port_reg, uint8_t pin_no)
{
	//just an initial number represents neither 1(true) nor 0(false)
	uint8_t pressed = 5;
	
	uint16_t pressed_confidence_level = 0;
	uint16_t released_confidence_level = 0;
	
	while (pressed == 5) // while didn't reach any confidence level
	{
		/*if detected a high bit, then (in pd) this means a step closer to 
		 the pressed state, so inc pressed confidence level and clear released   increase the pressed
		 confidence level*/
		if (BIT_IS_SET(*port_reg, pin_no))//in case of high value detected
		{
			released_confidence_level = 0;
			pressed_confidence_level++ ;
			
			//if the ones exceeded the confidence level then the pin is pressed 
			if (pressed_confidence_level > CONFIDENCE_LEVEL)
			{
				pressed = 1;
				pressed_confidence_level = 0;
			}
			
		}
		else//in case of low value detected it's the reverse of the high value
		{
			released_confidence_level++ ;
			pressed_confidence_level = 0;
			
			if (released_confidence_level > CONFIDENCE_LEVEL)
			{
				pressed = 0;
				released_confidence_level = 0;
			}
			
		}
		
	}	
	
	return (pressed != 0);
}

//the complete reverse of pull down function above
static bool is_port_pin_pu_pressed(volatile uint8_t * port_reg, uint8_t pin_no)
{
	uint8_t pressed = 5; //any default value
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
	
	return (pressed != 0);
	
}