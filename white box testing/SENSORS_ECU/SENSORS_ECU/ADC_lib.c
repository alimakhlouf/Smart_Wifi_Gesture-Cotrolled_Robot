/* --------------------------------------------------------------------------
[FILE NAME]:     ADC_lib.c

[AUTHOR]:        Ali MAkhlouf

[DATE CREATED]:  2/3/2015 11:28:01 AM

[DESCRIPTION]:   a library C file for controlling and operating ADC 
				 peripheral
-----------------------------------------------------------------------------*/
#include "ADC_lib.h"

static void( * adc_isr)(void); 

void CONFIG_TMR1_OVF_ISR()
{
	adc_isr();
}

/*=====================================================================
 * ====================================================================
 */

void adc_init(void)
{
	/* ADMUX Register Bits Description:
	 * REFS1:0 = 00 to choose to connect external reference voltage by input this voltage through AREF pin
	 * ADLAR   = 0 right adjusted
	 * MUX4:0  = 00000 to choose channel 0 as initialization
	 */
	ADMUX =(UPPER_LIMIT_SOURCE << REFS0) | (ADJUST_RESULT_BITS << ADLAR);

	/* ADCSRA Register Bits Description:
	 * ADEN    = 1 Enable ADC
	 * ADIE    = 0 Disable ADC Interrupt
	 * ADPS2:0 = 011 to choose ADC_Clock=F_CPU/8=1Mhz/8=125Khz --> 
	 * ADC must operate in range 50-200Khz
	 */
	ADCSRA = (1 << ADEN) | (FREQ_PRESCALER << ADPS0);
}


uint16_t adc_read(uint8_t channel_num)
{
	if (adc_int_state() == false)
	{
		DDRA &= ~(1 << channel_num); // assign the pin as input
		channel_num &= 0x07; /* channel number must be from 0 --> 7 */
		/*clear first 5 bits in the ADMUX (choose any previously chosen channel)
		 * (channel number MUX4:0 bits) before setting the required channel
		 * */
		ADMUX &= 0xE0;
		//choose a specific channel by setting the channel number in MUX4:0 bits
		ADMUX |= channel_num; 
		SET_BIT(ADCSRA,ADSC); /* start conversion process by writing '1' to ADSC */
	
	    //wait for conversion to complete ADIF becomes '1'
		while (BIT_IS_CLEAR(ADCSRA,ADIF)); 
		SET_BIT(ADCSRA,ADIF); /* clear ADIF by write '1' to it :) */
	}
	
	
	ADMUX &= 0xE0; //clear the chosen channel again
	return GET_ADC_RESULT; /* return the data register */
}

void adc_convert(unint8_t channel_num)
{
	DDRA &= ~(1 << channel_num); // assign the pin as input
	channel_num &= 0x07; /* channel number must be from 0 --> 7 */
	/* clear first 5 bits in the ADMUX (channel number MUX4:0
	   bits) before set the required channel */
	ADMUX &= 0xE0; 
				      
	//choose the correct channel by setting the channel number in MUX4:0bits
	ADMUX = ADMUX | channel_num; 
	                               
	SET_BIT(ADCSRA,ADSC);//start conversion write '1' to ADSC
}

void adc_set_isr(void ( * adc_int_function)(void))
{
	
	adc_isr = adc_int_function;
}

void adc_set_int(bool b_state)
{
	ADCSRA = (b_state ? ADCSRA | (1 << ADIE) : ADCSRA & ~(1 << ADIE));
}

