/* ---------------------------------------------------------------------------------------------------
[FILE NAME]: millis.c
[AUTHOR(S)]: Ali Makhlouf
[DATE CREATED]: 12/5/2017
[DESCRIPTION]: a library c header file for delay using timer0 either using polling or interrupt
			  unlike _delay_ms() ,it can also monitor a specified event or code during the
			  delay and if the action happened it jumps out of the delay
--------------------------------------------------------------------------------------------------- */

#include "millis0.h"




#if(MILLIS_INTERRUPT)

static  unint8_t tt;
ISR(TIMER0_OVF_vect)
{
	if(n_ovf ==0 )
	{}
	else if (n_ovf>1)
	{
		n_ovf--;//decrement the static variable.
		TCNT0=0;
	}
	else if (n_ovf == 1)
	{
		n_ovf--;//decrement the static variable
		TCNT0 = 256 - tt;
	}


}


unint8_t millis_interrupt(unint16_t t)
{

	if ((n_ovf==0) && (TCCR0==0)) //happens only at the beginning
	{
		PORTD=0xff;
		//in the .h file we can have the number of overflows and the number of remaining ticks
		//we put the number of overflows in the static variable plus one for the remaining ticks
		n_ovf= NUMBER_OF_OVFLOWS + 1;

		//initialise timer0 with 1024 prescaler and activate the timer mode
		TCCR0=0x05;
		TIMSK=1;
		//if the number of ovf is 1 meaning there is just the remaining ticks
			//make TCNT0=265-REMAINING_TICKS;
		//else TCNT=0
		tt=REMAINING_TICKS;

		if(n_ovf == 1)
			TCNT0 = 256 - tt ;
		else
			TCNT0=0;


	}


		else if(n_ovf == 0)
		{
			TCCR0=0;//disable the timer and return false
			TIMSK &= ~(1<<0);
			return FALSE;
		}



	return TRUE;
}

#else

 unint8_t millis(unint16_t t)
{
	
	//if it's the first loop ( activate the timer)
	if ((n_ovf == 0) && (TCCR0 == 0))
	{
		//in the .h file we can have the number of overflows and the number of remaining ticks
		//we put the number of overflows in the static variable plus one for the remaining ticks
		n_ovf = NUMBER_OF_OVFLOWS + 1;

		//initialize timer0 with 1024 prescaler and activate the timer mode
		TCCR0 = 0x05;
		//if the number of ovf is 1 meaning there is just the remaining tick
			//make TCNT0=265-REMAINING_TICKS;
		//else TCNT=0 (clear the timer)
		if(n_ovf == 1)
			TCNT0 = 256 - REMAINING_TICKS;
		else
			TCNT0=0;

	}
	else // if it's not the first loop 
	{
		if ((TIFR & (1 << TOV0))) // if the overflow flag was set in this tick
		{
			TIFR |= (1<<TOV0); // clear the flag
			PORTC = (unint8_t)(n_ovf);
			//PORTD= (n_ovf>>8);
			n_ovf--;//decrement the number of overflows left by one static variable.
			
			if(n_ovf == 0) //if the number of overflows left is 0  
			{
				TCCR0=0;
				TCNT0=0;
				
				//because there are no ticks left .. then the time has ended so we return 0
				return FALSE;
			}//if there are remaining OVERFLOWS is one .. this means the last flag has some ticks remaining but not all 256 ticks
			else if (n_ovf == 1)
			{
				TCNT0 = 256 - REMAINING_TICKS;// the last ovf we make the TCNT0 = (256 - remaining ticks)
			}
			else if (n_ovf > 1)//if there are remaining OVERFLOWS .. just clear the timer to start ticking again
			{
				TCNT0=0;
			}

		}

	}

	return TRUE;
}

#endif
