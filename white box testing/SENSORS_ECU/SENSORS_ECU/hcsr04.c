/* --------------------------------------------------------------------------
[FILE NAME]:     hcsr04.c

[AUTHOR]:        Ali MAkhlouf

[DATE CREATED]:  4/19/2019 01:49:03 PM

[DESCRIPTION]:   a library header file for controlling and reading sensor
				 values from a MQ2 sensor
-----------------------------------------------------------------------------*/

#include "hcsr04.h"

#if defined(ATMEGA8) || defined(ATMEGA32) || defined(ATMEGA16) 
//used in timer1 overflow counter   
static uint16_t ovf_counter = 0;

//0 means no operation started -- 1 means operation -- 2 operation finished
static uint8_t  op_state = HCSR04_NO_OP;

//activates input capture unit
#define ACTIVATE_ICP() (TCCR1B |= (0x01))
//deactivates input capture unit
#define DEACTIVATE_ICP() (TCCR1B &= ~(0x07))


void CONFIG_TMR1_OVF_ISR()
{
	ovf_counter++ ;
}

void CONFIG_TMR1_CAPT_ISR()
{
	TCNT1H = 0;
	TCNT1L = 0;
	
	//if the previous state is no operation .. change the state to operating
	//configure the capture unit to detecting a falling edge of the echo
	if (op_state == HCSR04_NO_OP)  
	{
		/*make the capture at the falling edge to detect the end of the signal 
		and start counting the time*/
		TCCR1B &= ~(1 << ICES1);
		// meaning start of the echo signal and counting the time
		op_state = HCSR04_START_OP;
	}
	else  //if its the end of the signal
	{
		//deactivate the timer,will be activated again when we send the trigger
		DEACTIVATE_ICP(); 
		/*make the edge rising again in order for when the next signal occur 
		it captures its rising signal*/ 
		TCCR1B |= (1 << ICES1);
		//change the state to operation completed
		op_state = HCSR04_COMPLETE_OP;
	}

}


void hcsr04_init()
{
	//set trigger pin to output .. low 
	DIO_SET_STATE(HCSR04_TR_PIN, DIO_OUTPUT);
	DIO_SET_VAL(HCSR04_TR_PIN, DIO_LOW);
	
	//clear the timer counter
	TCNT1L = 1;
	TCNT1H = 0;
	
	// pre scaler of 8 --- positive edge trigger
	TIMSK  |=  (1 << TICIE1) | (1 << TOIE1);
	TCCR1B |= (1 << ICES1);  
	//[NOTE]: the timer doesn't start yet 
}

void hcsr04_trigger()
{
	//set the trigger pin for 10 usec, then activate timer input capture
	DIO_SET_VAL(HCSR04_TR_PIN, DIO_HIGH);
	_delay_us(10);
	DIO_SET_VAL(HCSR04_TR_PIN, DIO_LOW);
	ACTIVATE_ICP();
}

uint16_t hcsr04_cm()
{
	/*holds the value of the ICR1 register(which holds the captured value of
	 the timer).*/
	uint16_t icr_result;
	
	//holds the resulted distance in cm. 
	uint32_t hcs_cm;
	
	//copy the ICR1H - ICR1L to icr_result
	icr_result = ICR1L;
	icr_result |= ((uint16_t)ICR1H << 8);
	
	//apply the formula to get the distance in cm
	hcs_cm = (uint32_t)icr_result + ((uint32_t)ovf_counter << 16);
	hcs_cm /= 58;
	
	//after completion .. set the module state to no operation
	op_state = HCSR04_NO_OP;
	
	ovf_counter = 0;
	
	return (uint16_t)hcs_cm;
}

#endif



bool hcsr04_data_ready()
{
	//return the module state
	return (HCSR04_COMPLETE_OP == op_state);
}
