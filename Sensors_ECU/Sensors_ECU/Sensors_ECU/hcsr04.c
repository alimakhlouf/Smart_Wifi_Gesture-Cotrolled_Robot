/*
 * hcsr04.c
 *
 * Created: 4/19/2019 01:49:03 م
 *  Author: hppp
 */ 

#include "hcsr04.h"

static volatile uint16_t ovf_counter = 0;
static volatile uint8_t  echoing = 0;


#if defined(ATMEGA8) || defined(ATMEGA32) || defined(ATMEGA16) 

#define ACTIVATE_ICP() TCCR1B |= (0x02)
#define DEACTIVATE_ICP() TCCR1B &= ~(0x07)


void CONFIG_TMR1_OVF_ISR()
{
	ovf_counter++ ;
}

void CONFIG_TMR1_CAPT_ISR()
{
	TCNT1H = 0;
	TCNT1L = 0;
	
	if (echoing == 0) // if its the start of the signal .. meaning if the capture detected a rusing edge of the echo 
	{
		TCCR1B &= ~(1 << ICES1); //make the capture at the falling edge to detect the end of the signal and start counting the time
		echoing = 1; // meaning start of the echo signal and counting the time
	}
	else  //if its the end of the signal
	{
		DEACTIVATE_ICP(); //deactivate the timer .. we will activate it again when we send the trigger
		
		TCCR1B |= (1 << ICES1);//make the edge rising again in order for when the next signal occur it captures its rising signal 
		echoing = 2;
	}

}

void hcsr04_init()
{
	DIO_SET_DIR(HCSR04_TR_PIN, DIO_OUTPUT);
	DIO_SET_VAL(HCSR04_TR_PIN, DIO_LOW);
	
// 	HCSR04_TR_DIR |= (1 << HCSR04_TR_PIN);
// 	HCSR04_TR_PORT &= ~(1 << HCSR04_TR_PIN);
	TCNT1L = 0;
	TCNT1H = 0;
	TIMSK  |=  (1 << TICIE1) | (1 << TOIE1);
	TCCR1B |= (1 << ICES1); // pre scaler of 8 --- positive edge trigger  // do not start timer yet
}


void hcsr04_trigger()
{
	DIO_SET_VAL(HCSR04_TR_PIN, DIO_HIGH);
	
	//HCSR04_TR_PORT |= (1 << HCSR04_TR_PIN);
	_delay_us(10);
	DIO_SET_VAL(HCSR04_TR_PIN, DIO_LOW);
	//HCSR04_TR_PORT &= ~(1 << HCSR04_TR_PIN);
	ACTIVATE_ICP();
}

uint16_t hcsr04_cm()
{
	uint16_t icr_result;
	uint32_t hcs_cm;
	
	icr_result = ICR1L;
	icr_result |= ((uint16_t)ICR1H << 8);
	
	
	hcs_cm = (uint32_t)icr_result + ((uint32_t)ovf_counter << 16);
	hcs_cm /= 58;
	echoing = 0;
	ovf_counter = 0;
	
	return (uint16_t)hcs_cm;
}

#endif



bool hcsr04_data_ready()
{
	return (echoing == 2);
}
