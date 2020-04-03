/* ---------------------------------------------------------------------------------------------------
[FILE NAME]: millis.c
[AUTHOR(S)]: Ali Makhlouf
[DATE CREATED]: 12/5/2017
[DESCRIPTION]: A library c header file for delay using timer0 either using polling or interrupt
			  unlike delay_msec() ,it can also monitor A specified event or code during the
			  delay and if the action happened it jumps out of the delay
--------------------------------------------------------------------------------------------------- */

#include "millis_mcu_config.h"


#if defined(ATMEGA8) || defined(ATMEGA16) || defined(ATMEGA32)

/*this holds the interrupt handler that the application programmer writes and
  it's called when an interrupt occurs
*/
void ( * gp_tmr0_ovf_isr)(void) = NULL;

/*when deciding the number of overflows needed to generate the needed ticks
 for making the delay time there will be some remaining ticks after the last
  overflow that 'll occur. This variable holds it 
*/
static volatile unint8_t g_last_ticks;

//the number of overflows needed to generate the needed ticks for making the
// delay time
static volatile unint16_t g_ovf_count;

void CONFIG_TMR0_OVF_ISR()
{
	cli();
	/*NOTE: the reason I am checking the overflow counter rather than the checking
	 a flag is that by decrementing the g_ovf_count in ISR then I don't have to 
	 guarantee that millis will be checked every 1024 cycles(because the program 
	 might be is too big so that I can't check this function every 1024 cycles
	 */
	//if there are remaining overflows yet
	if (g_ovf_count)
	{
		/* if (g_ovf_count == 1) then the n_of actual overflows has ended and
		   there are some remaining ticks which will be out in TCNT0 to get all
		   the ticks needed precisely*/
		if (g_ovf_count == 1)
		{
			TCNT0 = 256 - g_last_ticks;
		}
		
		g_ovf_count-- ;
	}
	
	if (gp_tmr0_ovf_isr != NULL)
	{
		gp_tmr0_ovf_isr();
	}
	
	sei();
}


bool millis(unint16_t t)
{
	if ((g_ovf_count == 0) && (TCCR0 == 0)) //happens only at the beginning
	{
		/*initialize g_ovf_count to hold the number of overflows so that the counter will keep working and thus decreasing this number with
		//every overflow occur until this number reaches zero then the timer will recognize that it's timeout
		//in the .h file we can have the number of overflows and the number of remaining ticks(calculated automatically from the time
		//parameter
		//we put the number of overflows plus one(because the required number of ticks are not exactly multiples of 256 so there will be
        //some ticks less that 256 remaining at the end ticks in a static variable plus one for the remaining ticks.*/
		g_ovf_count= NUMBER_OF_OVFLOWS + 1;

		//initialize timer0 with 1024 pre_scaler and activate the timer mode
		TCCR0 = 0x05;
		#if (MILLIS_INTERRUPT == 1)
		TIMSK = (1 << 0); //enable timer0 interrupt
		//if the number of ovf is 1 meaning there is just the remaining ticks
		//make TCNT0=265-REMAINING_TICKS;
		//else TCNT=0
		g_last_ticks = REMAINING_TICKS;
		#endif
		
		
		if(g_ovf_count == 1)
			TCNT0 = 256 - REMAINING_TICKS;
		else if ( g_ovf_count > 1) 
			TCNT0 = 0;


	}// the below part is different between interrupt millis and polling millis   
	#if (MILLIS_INTERRUPT == 1)
	else if (g_ovf_count == 0) // we only check for the timeout state(g_ovf_count reaches 0) other states are handled in the OVF ISR
	{
		TCCR0 = 0;//disable the timer and return false
		TCNT0 = 0;
		TIMSK &= ~(1<<0);
		return false;
	}
	#endif
	#if (MILLIS_INTERRUPT == 0) //here we have to poll the flag .. if set check for each state of g_ovf_count 
	else // if it's not the first loop
	{
		if ((TIFR & (1 << TOV0))) // if the overflow flag was set in this tick
		{
			TIFR |= (1 << TOV0); // clear the flag
			
			g_ovf_count-- ;//decrement the number of overflows left by one static variable.
			
			if(g_ovf_count == 0) //if the number of overflows left is 0 (timeout) ..disable the timer and return false
			{
				TCCR0 = 0;
				TCNT0 = 0;
				
				//because there are no ticks left .. then the time has ended so we return 0
				return false;
			}//if there are remaining OVERFLOWS is one .. this means the last flag has some ticks remaining but complete 256 ticks
			else if (g_ovf_count == 1)
			{
				TCNT0 = 256 - REMAINING_TICKS;// the last ovf we make the TCNT0 = (256 - remaining ticks)
			}
			else if (g_ovf_count > 1)//if there are remaining OVERFLOWS .. just clear the counter timer and continue ticking again
			{
				//do not need to reset TCNT0;
			}

		}

	}
	#endif
	
	return true;
}

void millis_disable_tmr()
{
	TCCR0 = 0;
	TCNT0 = 0;
	g_ovf_count = 0;
}

void millis_reset_ticks()
{
	TCNT0 = 0;
}

void millis_reset_tmr(uint16_t t)
{
	g_ovf_count = NUMBER_OF_OVFLOWS + 1;
#if (MILLIS_INTERRUPT == 1)
	TIMSK = 1; //enable timer0 interrupt
	//if the number of ovf is 1 meaning there is just the remaining ticks
	//make TCNT0=265-REMAINING_TICKS;
	//else TCNT=0
	g_last_ticks = REMAINING_TICKS;
#endif
}

void millis_add_ovf_isr(void ( * additional_tmr_ovf_isr)(void))
{
	gp_tmr0_ovf_isr = additional_tmr_ovf_isr;
}



#endif