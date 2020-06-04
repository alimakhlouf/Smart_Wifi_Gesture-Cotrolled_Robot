/* ---------------------------------------------------------------------------------------------------
[FILE NAME]: millis.c
[AUTHOR(S)]: Ali Makhlouf
[DATE CREATED]: 12/5/2017
[DESCRIPTION]: a library c header file for delay using timer0 either using polling or interrupt
			  unlike _delay_ms() ,it can also monitor a specified event or code during the
			  delay and if the action happened it jumps out of the delay
--------------------------------------------------------------------------------------------------- */

#include "millis0.h"


#if MILLIS_INTERRUPT == 1
static volatile  unint8_t tt;
ISR(TIMER0_OVF_vect)
{
	//NOTE: the reason I am checking the overflow counter rather than the checking a flag is that by decrementing the 
	//n_ovf in ISR then I don't have to guarantee that millis will be checked every 1024 cycles(because the program might be is too big
	//so that I can't check this function every 1024 cycles
	
	
	
	if (n_ovf)
	{
		n_ovf-- ;
		
		// if (n_ovf == 1) then there are some remaining ticks wich will be out in TCNT0 .. otherwise(n_ovf > 1) clear TCNT0
		if (n_ovf == 1)
		{
			TCNT0 = 256 - tt;
		}
		else
		{
			
		}
		
		//TCNT0 = ((n_ovf == 1)? (256 - tt) : (0)); //can't put this statement in the main function because timer zero
															   //is only reinitialized when an overflow occurs. 
		//
	}
	
}
#endif



unint8_t millis(unint16_t t)
{
	
	if ((n_ovf == 0) && (TCCR0 == 0)) //happens only at the beginning
	{
		/*initialize n_ovf to hold the number of overflows so that the counter will keep working and thus decreasing this number with
		//every overflow occur until this number reaches zero then the timer will recognize that it's timeout
		//in the .h file we can have the number of overflows and the number of remaining ticks(calculated automatically from the time
		//parameter
		//we put the number of overflows plus one(because the required number of ticks are not exactly multiples of 256 so there will be
        //some ticks less that 256 remaining at the end ticks in a static variable plus one for the remaining ticks.*/
		n_ovf = NUMBER_OF_OVFLOWS + 1;

		//initialize timer0 with 1024 pre-scaler and activate the timer mode
		TCCR0 = 0x05;
		
		#if MILLIS_INTERRUPT == 1
		TIMSK |= 1; //enable timer0 interrupt
		//if the number of ovf is 1 meaning there is just the remaining ticks
		//make TCNT0=265-REMAINING_TICKS;
		//else TCNT=0
		tt = REMAINING_TICKS;
		#endif
		
		
		if(n_ovf == 1)
			TCNT0 = 256 - REMAINING_TICKS;
		else if (n_ovf > 1) 
			TCNT0 = 0;


	}// the below part is different between interrupt millis and polling millis   
	#if MILLIS_INTERRUPT == 1
	else if (n_ovf == 0) // we only check for the timeout state(n_ovf reaches 0) other states are handled in the OVF ISR
	{
		TCCR0 = 0;//disable the timer and return false
		TCNT0 = 0;
		TIMSK &= ~(1 << 0);
		return FALSE;
	}
	#endif
	#if MILLIS_INTERRUPT == 0 //here we have to poll the flag .. if set check for each state of n_ovf 
	else // if it's not the first loop
	{
		if ((TIFR & (1 << TOV0))) // if the overflow flag was set in this tick
		{
			TIFR |= (1 << TOV0); // clear the flag
			
			n_ovf--;//decrement the number of overflows left by one static variable.
			
			if (n_ovf == 0) //if the number of overflows left is 0 (timeout) ..disable the timer and return false
			{
				TCCR0 = 0;
				TCNT0 = 0;
				
				//because there are no ticks left .. then the time has ended so we return 0
				return FALSE;
			}//if there are remaining OVERFLOWS is one .. this means the last flag has some ticks remaining but complete 256 ticks
			else if (n_ovf == 1)
			{
				TCNT0 = 256 - REMAINING_TICKS;// the last ovf we make the TCNT0 = (256 - remaining ticks)
			}
			else if (n_ovf > 1)//if there are remaining OVERFLOWS .. just clear the counter timer and continue ticking again
			{
				//do not need to reset TCNT0;
			}

		}

	}
	#endif
	
	return TRUE;
}

