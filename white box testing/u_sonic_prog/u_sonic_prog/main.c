/*
 * u_sonic_prog.c
 *
 * Created: 4/14/2019 08:07:31 ص
 * Author : hppp
 */ 

#include "micro_config.h"
#include "uart.h"
#include "hcsr04.h"
#include "millis0.h"

#define ACTIVATE_ICP (TCCR1B |= (0x02))
#define DEACTIVATE_ICP (TCCR1B &= ~(0x07))


volatile uint32_t ovf_counter = 0;
volatile uint8_t  echoing = 0;


ISR (TIMER1_OVF_vect)
{
	ovf_counter++ ;
}

ISR (TIMER1_CAPT_vect)
{
	TCNT1H = 0;
	TCNT1L = 5;
	
	if (echoing == 0) // 0 means that it was sending the wave and there is no echo yet
					  //so when the edge interrupt happens it means that the echo has arrived (indicated 
					  //by the high bit that appeared on the icp pin which activated the interrupt) .... now we 
					  //are going to start calculating the period and make the interrupt edge to be 0 (in order to
					  //capture the end of the echo) so that when the echo ends the interrupt activates and
					  //goes to the (else) part and deactivates the interrupt.
	{	
		
		TCCR1B &= ~(1 << ICES1); //make the edge of the input capture to be low so that win the 
		echoing = 1;
	}
	else //if its the end of the signal echoing = 1
	{
		DEACTIVATE_ICP; //deactivate the timer(input capture timer) ..
		// we will activate it again when we send the trigger   
		TCCR1B |= (1 << ICES1);
		echoing = 2; // means that the echo signal is finished and now we must calculate and
		             // get the result and return to the first state which is echoing = 0 so 
					 //that we are waiting to send another wave 
					 // going 
	}

	
}



void hcsr04_trigger ();
void hcsr04_init ();


int main(void)
{
	DDRB |= 3;
	volatile uint32_t cpt_result;
	volatile uint32_t final_r = 0;
	uint8_t buff[19];
	
	UART_init();
	hcsr04_init();
	
	sei();
	
	PORTB |= 1;
	_delay_ms(1000);
	
	//hcsr04_trigger();
	
    while (1) 
    {
		
		if (echoing == 2)
		{
			cpt_result = ICR1L;
			cpt_result |= ((uint16_t)ICR1H << 8);
			final_r = (uint32_t)cpt_result + (ovf_counter << 16); 
			final_r /= 58;
			
			UART_sendStrByMark("cm: ", 0);
			ltoa(final_r , buff, 10);
			UART_sendStrByMark(buff, 0);
			UART_sendStrByMark("\r\n", 0);
			
			echoing = 0;
			ovf_counter = 0;
			
			if (final_r < 5)
			{
				PORTB |= 2; 
			}
			else
			{
				PORTB &= ~2;
			}
            
		}
		
		// if one second has passed then send another ultra sonic wave
		if (millis(500) == 0)
		{
			ltoa(ovf_counter , buff, 10);
			UART_sendStrByMark(buff, 0);
			UART_sendStrByMark("\r\n", 0);
			
			
			
			PORTB ^= 1;
			hcsr04_trigger();
		}

// 		PORTB ^= 1;
// 		hcsr04_trigger();
		
    }
	
}

//NOTE: the timer tick must be 1us so the initialization isn't generic and depends on the clock freq
//and you have to set the prescaler such that the tick becomes  1us (the following initialization for
// 8Mhz clock freq)
void hcsr04_trigger()
{
	HCSR04_TR_PORT |= (1 << HCSR04_TR_PIN);
	_delay_us(10);
	HCSR04_TR_PORT &= ~(1 << HCSR04_TR_PIN);
	
	//activate the ICP(input capture peripheral)  .. and the edge was and still at the rising edge
	TCCR1B |= (1 << ICES1);
	ACTIVATE_ICP;
}

void hcsr04_init()
{
	HCSR04_TR_DIR |= (1 << HCSR04_TR_PIN);
	HCSR04_TR_PORT &= ~(1 << HCSR04_TR_PIN);
	TCNT1L = 0;
	TCNT1H = 0;
	TIMSK  |=  (1 << TICIE1) | (1 << TOIE1);
	TCCR1B |= (1 << ICES1); // pre scaler of 8 ( to make every count = 1us  --- positive edge trigger  // do not start timer yet
}


