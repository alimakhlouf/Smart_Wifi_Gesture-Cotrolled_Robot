/*
 * ADC_lib.c
 *
 *  Created on: ? ‘⁄»«‰° ???? Â‹
 *      Author: hppp
 */
#include "ADC_lib.h"

static void( * adc_isr)(void) = {NULL}; 

void CONFIG_ADC_ISR()
{
	cli();
	
	if (adc_isr != NULL)
	{
		adc_isr();
	}
	
	sei();
}

/*=====================================================================
 * ====================================================================
 */

void ADC_init(void)
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
	 * ADPS2:0 = 011 to choose ADC_Clock=F_CPU/8=1Mhz/8=125Khz --> ADC must operate in range 50-200Khz
	 */
	ADCSRA = (1 << ADEN) | (FREQ_PRESCALER << ADPS0);
}


unint16_t ADC_read(unint8_t channel_num)
{
	if (adc_int_state() == false)
	{
		DDRA &= ~(1 << channel_num); // assign the pin as input
		channel_num &= 0x07; /* channel number must be from 0 --> 7 */
		/*clear first 5 bits in the ADMUX (unchoose any previously chosen channel)
		 * (channel number MUX4:0 bits) before setting the required channel
		 * */
		ADMUX &= 0xE0;
		ADMUX |= channel_num; /* choose the correct channel by setting the channel number in MUX4:0 bits */
		SET_BIT(ADCSRA,ADSC); /* start conversion process by writing '1' to ADSC */
	
	
		while (BIT_IS_CLEAR(ADCSRA,ADIF)); /* wait for conversion to complete ADIF becomes '1' */
		SET_BIT(ADCSRA,ADIF); /* clear ADIF by write '1' to it :) */
	}
	
	
	ADMUX &= 0xE0; //unchoose the channel again
	return GET_ADC_RESULT; /* return the data register */
}

void ADC_convert(unint8_t channel_num)
{
	DDRA &= ~(1 << channel_num); // assign the pin as input
	channel_num &= 0x07; /* channel number must be from 0 --> 7 */
	ADMUX &= 0xE0; /* clear first 5 bits in the ADMUX (channel number MUX4:0 bits) before set the required channel */
	ADMUX = ADMUX | channel_num; /* choose the correct channel by setting the channel number in MUX4:0 bits */
	SET_BIT(ADCSRA,ADSC); /* start conversion write '1' to ADSC */
}

void adc_set_isr(void ( * adc_int_function)(void))
{
	adc_isr = adc_int_function;
}

void adc_set_int(bool b_state)
{
	ADCSRA = (b_state ? ADCSRA | (1 << ADIE) : ADCSRA & ~(1 << ADIE));
}

