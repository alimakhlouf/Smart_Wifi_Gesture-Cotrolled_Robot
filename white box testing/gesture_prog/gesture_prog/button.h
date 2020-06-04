/* --------------------------------------------------------------------------
[FILE NAME]:     button.h

[AUTHOR]:        Ali MAkhlouf

[DATE CREATED]:  5/11/2019 04:20:45 PM

[DESCRIPTION]:   a library header file for checking Button status and avoiding
	button debounce using software(no delays)

----------------------------------------------------------------------------*/


#ifndef BUTTON_H_
#define BUTTON_H_

#include "micro_config.h"
#include "dio_api.h"
#include <stdbool.h>

/*[NOTE]:
confidence level tracks how many consecutive similar bits of the either
ones or zeros appeared and if exceeds a certain value then this value is
the real value of the pin. This solves the debounce problems in buttons
as debounce keeps flipping between high and low so it won't reach the
confidence level until it passes*/
#define CONFIDENCE_LEVEL 500

#ifdef ATMEGA8
#define NUM_PIN_PER_PORT 8
#define PIN_REG_BASE (volatile uint8_t *)(0x36)
#elif defined(ATMEGA32) || defined(ATMEGA16)
// the base address of the PIN registers (which is PINA)
#define PIN_REG_BASE (volatile uint8_t *)(0x39)
#define NUM_PIN_PER_PORT 8
#endif

#define PIN_REG_OFFSET(n) (((n) * -3))
#define PULL_UP 0
#define PULL_DOWN 1

/* ----------------------------------------------------------------------------
[Macro Name] : BUTTON_STATUS
[Description] : This function is getting the state of the button(pushed_released) 
	it calls the function button_state
[Args] :
[in] -uint8_t pull_type
	this ARG shall indicate the pull type(pull up or pull down) 
[in] -uint8_t dio_no
	this ARG shall indicate the pin that the button is connected to
	you enter dio number directly
[Returns] : this return shall indicate the  state of the button(pushed_released)
------------------------------------------------------------------------------*/
#define BUTTON_STATUS(pull_type, dio_no) (button_state(pull_type, \
														 CAT(DIO_PIN, dio_no)))

/* ----------------------------------------------------------------------------
[Function Name] : button_state
[Description] : This function is getting the state of the button(pushed_released) 
[Args] :
[in] -uint8_t a_pull_type
	this ARG shall indicate the pull type(pull up or pull down) 
[in] -uint8_t a_dio_pin_no
	this ARG shall indicate the pin that the button is connected to
	the form is: DIO_PINn (n is the number of dio pin)
[Returns] : this return shall indicate the  state of the button(pushed_released)
------------------------------------------------------------------------------*/
bool button_state (uint8_t a_pull_type, uint8_t a_dio_pin_no);



#endif /* BUTTON_H_ */