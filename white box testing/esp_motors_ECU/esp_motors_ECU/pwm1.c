/* ----------------------------------------------------------------------------
[FILE NAME]: pwm1.h

[AUTHOR]: Ali Makhlouf

[DATE CREATED]: 10/5/2017

[DESCRIPTION]: a library c file for initiating and operating PWM peripherals
-------------------------------------------------------------------------------*/
#include "pwm1.h"

#if defined(ATMEGA32) || defined(ATMEGA16)


void pwm_init(uint8_t pwm_no, uint16_t freq) 
{
	if (pwm_no & PWM0)
	{
		TCCR0 &= ~(0x07); // clear any previous pre scalers
		
		/*get number of ticks for required for the delay at 1 prescaler
		 if it overflowed the TOP(255) increase the prescaler */
		if(NUM_REQUIRED_TICKS_NO_PRE < 255) 
		{
			//if no overflow select prescaler 1
			TCCR0 = (1<<CS00);
		}//do the same for the 8 prescaler 
		else if(NUM_REQUIRED_TICKS_8_PRE < 255) 
		{
			TCCR0 |= (1<<CS01); //
		}//do the same for the 64 prescaler 
		else if(NUM_REQUIRED_TICKS_64_PRE < 255)
		{
			TCCR0 |= (1<<CS10)|(1<<CS11); //
		}//do the same for the 256 prescaler 
		else if(NUM_REQUIRED_TICKS_256_PRE < 255)
		{
			TCCR0 |= (1<<CS02); //
		}//do the same for the 1024 prescaler 
		else if(NUM_REQUIRED_TICKS_1024_PRE < 255)
		{
			TCCR0 |= (1<<CS00)|(1<<CS02);
		}
		else
		{
			//barcoding standard
		}
		//set the bits for FAST PWM
		TCCR0 |= (1<<WGM00) | (1<<WGM01); 
	}
	
	if ((pwm_no & (PWM1)) || (pwm_no & (PWM2)))
	{
		TCCR1A &= ~(0x07); // clear any previous pre scalers
		
		uint16_t ICR1_buffer = NUM_REQUIRED_TICKS_NO_PRE; // default value
		
		/*get number of ticks for required for the delay at 1 prescaler if it 
		 overflowed ICR1(66635) increase the prescaler 
		*/
		if(NUM_REQUIRED_TICKS_NO_PRE < (uint32_t)65535)
		{
			//if no overflow select prescaler 1
			TCCR1B = (1<<CS10);
			//put number of ticks needed to ICR1
			ICR1_buffer = NUM_REQUIRED_TICKS_NO_PRE;
		}//do the same for the 8 prescaler 
		else if(NUM_REQUIRED_TICKS_8_PRE < (uint32_t)65535)
		{
			TCCR1B |= (1<<CS11);	
			ICR1_buffer = NUM_REQUIRED_TICKS_8_PRE; 
		}//do the same for the 64 prescaler 
		else if(NUM_REQUIRED_TICKS_64_PRE < (uint32_t)65535)
		{
			TCCR1B |= (1<<CS10)|(1<<CS11); //
			ICR1_buffer = NUM_REQUIRED_TICKS_64_PRE;
		}//do the same for the 256 prescaler 
		else if(NUM_REQUIRED_TICKS_256_PRE < (uint32_t)65535)//apply the frequency formulA on prescaler=256 and get ICR1 value if it's larger than what ICR1 register can hold(this means that the frequency we want too low to use prescaler=256 for it) we have to increase the prescaler and apply the formulA again
		{
			TCCR1B |= (1<<CS12); //
			ICR1_buffer = NUM_REQUIRED_TICKS_256_PRE;
		}//do the same for the 1024 prescaler 
		else if(NUM_REQUIRED_TICKS_1024_PRE < (uint16_t)65535)//apply the frequency formulA on prescaler=1 and get ICR1 value if it's larger than what ICR1 register can hold(this means that the frequency we want too low to use prescaler=1 for it) we have to increase the prescaler and apply the formulA again
		{
			TCCR1B |= (1<<CS10)|(1<<CS12); //
			ICR1_buffer = NUM_REQUIRED_TICKS_1024_PRE;
		}
		else
		{
			//barcoding standard
		}
		

		ICR1H = ICR1_buffer >> 8;
		ICR1L = (ICR1_buffer & 0x00ff);
		
		// SET THE BIT (FAST PWM WITH ICR1 AS TOP)
		TCCR1B |= (1<<WGM12)|(1<<WGM13); 
		TCCR1A |= (1<<WGM11);
		TCCR1A &= ~(1 << WGM10);
	}
	
	
	
	if (pwm_no & PWM3)
	{
		TCCR2 &= ~(0x07); // clear any previous pre scalers
		
		/*get number of ticks for required for the delay at 1 prescaler
		 if it overflowed the TOP(255) increase the prescaler*/
		if(NUM_REQUIRED_TICKS_NO_PRE < 255)
		{
			//if no overflow select prescaler 1
			TCCR2 = (1 << CS20);//if no overflow assign prescaler 1 to the PWM time
		}//do the same for the 8 prescaler 
		else if(NUM_REQUIRED_TICKS_8_PRE < 255)
		{
			TCCR2 |= (1<<CS21); //
		}//do the same for the 32 prescaler 
		else if(NUM_REQUIRED_TICKS_32_PRE < 255) 
		{
			TCCR2 |= (1 << CS20) | (1 << CS21);
		}//do the same for the 64 prescaler 
		else if(NUM_REQUIRED_TICKS_64_PRE < 255)
		{
			TCCR2 |= (1 << CS22); //
		}//do the same for the 128 prescaler 
		else if(NUM_REQUIRED_TICKS_128_PRE < 255)
		{
			TCCR2 |= (1 << CS20) | (1 << CS22); 
		}//do the same for the 256 prescaler 
		else if(NUM_REQUIRED_TICKS_256_PRE < 255)
		{
			TCCR2 |= (1 << CS21) | (1 << CS22); //
		}//do the same for the 1024 prescaler 
		else if(NUM_REQUIRED_TICKS_1024_PRE < 255)//apply the frequency formulA on prescaler=1 and get ICR1 value if it's larger than what ICR1 register can hold(this means that the frequency we want too low to use prescaler=1 for it) we have to increase the prescaler and apply the formulA again
		{
			TCCR2 |= (1 << CS20) | (1 << CS22) | (1 << CS21);    
		}

		//set the bits for FAST PWM
		TCCR2 |= (1<<WGM20) | (1<<WGM21); 
	}
	
}

void pwm_start(uint8_t pwm_no)
{
	if (pwm_no & PWM0)
	{
		//set the pin OC0 as output
		DIO_SET_DIR(DIO_OC0, DIO_OUTPUT);
		//set OC0 pin to fast PWM mode
		TCCR0 |= (1 << COM01);
		TCCR0 &= ~(1 << COM00);
	}
	
	if (pwm_no & PWM1)
	{
		//set the pin OC1A as output
		DIO_SET_DIR(DIO_OC1A, DIO_OUTPUT);
		//set OC1A pin to fast PWM mode
		TCCR1A &= ~(1 << COM1A0);
		TCCR1A |= (1 << COM1A1); // SET THE BIT( CLEAR ON COMPARE)
	}
	
	if (pwm_no & PWM2)
	{
		//set the pin OC1B as output
		DIO_SET_DIR(DIO_OC1B, DIO_OUTPUT);
		//set OC1B pin to fast PWM mode
		TCCR1A &= ~(1 << COM1B0);
		TCCR1A |= (1 << COM1B1); 
	}
	
	if (pwm_no & PWM3)
	{
		//set the pin OC2 as output
		DIO_SET_DIR(DIO_OC2, DIO_OUTPUT);
		//set OC2 pin to fast PWM mode
		TCCR2 |= (1 << COM21);
		TCCR2 &= ~(1 << COM20);
	}
	
}

void pwm_setDuty(uint8_t pwn_no, uint8_t duty_percent)
{
	if (pwn_no & PWM0)
	{   
		/*[NOTE] :if the duty cycle is 100% putting the same number as 
		top value(256) will cause A compare and overflow at the same time which 
		it will be always either set or clear for the whole time so we put it
		 equal to TOP value 255
		*/
		if (duty_percent == 100) 
		{
			OCR0 = 255;
		}
		else
		{
			//assign OCR0(holds the compare value) to the duty cycle percentage 
			OCR0 = (255 * (uint16_t)duty_percent) / 100;
		}
			
	}
	
	/*[NOTE] :if the duty cycle is 100% putting the same number as 
		ICR1 value will cause A compare and overflow at the same time which 
		it will be always either set or clear for the whole time so we put it
		 equal to ICR1 -1
		*/
	if (pwn_no & PWM1)    
	{
		if (duty_percent == 100)
		{
			OCR1A = ((ICR1L) + ((uint16_t)ICR1H << 8)) - 1;
		}
		else
		{
			//assign OCR1A(holds the compare value) to the duty cycle percentage 
			OCR1A = ((uint32_t)((ICR1L) + ((uint16_t)ICR1H << 8)) * duty_percent) / 100;	
		}
			
	}
	
	//[NOTE]: same as PWM1
	if (pwn_no & PWM2)    
	{
		if (duty_percent == 100)
		{
			OCR1B = ((ICR1L) + ((uint16_t)ICR1H << 8)) - 1;
		}
		else
		{
			//assign OCR1B(holds the compare value) to the duty cycle percentage 
			OCR1B =((uint32_t)((ICR1L) + ((uint16_t)ICR1H << 8))  * duty_percent) / 100;
		}
	}
	
	//[NOTE]: same as PWM0
	if (pwn_no & PWM3)
	{
		if (duty_percent == 100) 
		{
			OCR2 = 255;
		}
		else
		{
			//assign OCR2(holds the compare value) to the duty cycle percentage
			OCR2 = (255 * (uint16_t)duty_percent) / 100;
		}
			
	}

}


void pwm_stop(uint8_t pwm_no)
{
	if (pwm_no & PWM0)
	{
		//disconnect OCR0 from fast PWM mode
		TCCR0 &= ~((1 << COM00) | (1 << COM01)); 
		DIO_SET_VAL(DIO_OC1A, DIO_LOW); 
	}
	
	if (pwm_no & PWM1)
	{
		//disconnect OCR1A from fast PWM mode
		TCCR1A &= ~((1 << COM1A0) | (1 << COM1A1));
		DIO_SET_VAL(DIO_OC1A, DIO_LOW);
	}
	
	if (pwm_no & PWM2)
	{
		//disconnect OCR1B from fast PWM mode
		TCCR1A &= ~((1 << COM1B0) | (1 << COM1B1));
		DIO_SET_VAL(DIO_OC1B, DIO_LOW);
	}
	
	if (pwm_no & PWM3)
	{
		//disconnect OCR2 from fast PWM mode
		TCCR2 &= ~((1 << COM20) | (1 << COM21));
		DIO_SET_VAL(DIO_OC2, DIO_LOW);
	}

	
}

#if 0
void pwm_changeFreq(uint8_t pwm_no, uint16_t freq)
{
	if ((pwm_no & PWM1) || (pwm_no & PWM2))
	{
		//you need to hold the values of  
		//a value and b_value are the duty cycles ..I should store them to
		//apply them when I
		
		uint64_t 
		float A_value = (float)OCR1A / ((ICR1L) + ((uint32_t)ICR1H << 8));
		float B_value = (float)OCR1B/ ((ICR1L) + ((uint32_t)ICR1H << 8));
		pwm_init(pwm_no, freq);
		OCR1A = (uint16_t)(A_value * ((ICR1L) + ((uint32_t)ICR1H << 8)));
		OCR1B = (uint16_t)(B_value * ((ICR1L) + ((uint32_t)ICR1H << 8)));
	}
	
	if ((pwm_no & PWM0) || (pwm_no & PWM3));
	{
		pwm_init(pwm_no, freq);
	}
	
}
#endif







#elif defined(ATMEGA8)
void pwm_init(uint8_t pwm_no, uint16_t freq) // sets the frequency ..and setes the timer as
{
	
	if ((pwm_no & (PWM0)) || (pwm_no & (PWM1)))
	{
		TCCR1A &= ~(0x07); // clear any previous pre scalers
		uint16_t ICR1_buffer = NUM_REQUIRED_TICKS_NO_PRE; // default value
		
		/*get number of ticks for required for the delay at 1 prescaler if it 
		 overflowed ICR1(66635) increase the prescaler 
		*/
		if(NUM_REQUIRED_TICKS_NO_PRE < (uint32_t)65535)
		{
			//if no overflow select prescaler 1
			TCCR1B = (1<<CS10);
			//put number of ticks needed to ICR1
			ICR1_buffer = NUM_REQUIRED_TICKS_NO_PRE;
		}//do the same for the 8 prescaler 
		else if(NUM_REQUIRED_TICKS_8_PRE < (uint32_t)65535)
		{
			TCCR1B |= (1<<CS11);	
			ICR1_buffer = NUM_REQUIRED_TICKS_8_PRE; 
		}//do the same for the 64 prescaler 
		else if(NUM_REQUIRED_TICKS_64_PRE < (uint32_t)65535)
		{
			TCCR1B |= (1<<CS10)|(1<<CS11); //
			ICR1_buffer = NUM_REQUIRED_TICKS_64_PRE;
		}//do the same for the 256 prescaler 
		else if(NUM_REQUIRED_TICKS_256_PRE < (uint32_t)65535)
		{
			TCCR1B |= (1<<CS12);
			ICR1_buffer = NUM_REQUIRED_TICKS_256_PRE;
		}//do the same for the 1024 prescaler 
		else if(NUM_REQUIRED_TICKS_1024_PRE < (uint16_t)65535)
		{
			TCCR1B |= (1<<CS10)|(1<<CS12);
			ICR1_buffer = NUM_REQUIRED_TICKS_1024_PRE;
		}
		else
		{
			//barcoding standard
		}
		

		ICR1H = ICR1_buffer >> 8;
		ICR1L = (ICR1_buffer & 0x00ff);
		
		//set the (FAST PWM WITH ICR1 AS TOP)
		TCCR1B |= (1<<WGM12)|(1<<WGM13); 
		TCCR1A |= (1<<WGM11);
		TCCR1A &= ~(1 << WGM10);
	}
	
	
	
	if (pwm_no & PWM2)
	{
		TCCR2 &= ~(0x07); // clear any previous pre scalers
		
		/*get number of ticks for required for the delay at 1 prescaler
		 if it overflowed the TOP(255) increase the prescaler*/
		if(NUM_REQUIRED_TICKS_NO_PRE < 255)
		{
			//if no overflow select prescaler 1
			TCCR2 = (1 << CS20);//if no overflow assign prescaler 1 to the PWM time
		}//do the same for the 8 prescaler 
		else if(NUM_REQUIRED_TICKS_8_PRE < 255)
		{
			TCCR2 |= (1<<CS21); //
		}//do the same for the 32 prescaler 
		else if(NUM_REQUIRED_TICKS_32_PRE < 255) 
		{
			TCCR2 |= (1 << CS20) | (1 << CS21);
		}//do the same for the 64 prescaler 
		else if(NUM_REQUIRED_TICKS_64_PRE < 255)
		{
			TCCR2 |= (1 << CS22); //
		}//do the same for the 128 prescaler 
		else if(NUM_REQUIRED_TICKS_128_PRE < 255)
		{
			TCCR2 |= (1 << CS20) | (1 << CS22); 
		}//do the same for the 256 prescaler 
		else if(NUM_REQUIRED_TICKS_256_PRE < 255)
		{
			TCCR2 |= (1 << CS21) | (1 << CS22); //
		}//do the same for the 1024 prescaler 
		else if(NUM_REQUIRED_TICKS_1024_PRE < 255)
		{
			TCCR2 |= (1 << CS20) | (1 << CS22) | (1 << CS21);    
		}

		//set the bits for FAST PWM
		TCCR2 |= (1<<WGM20) | (1<<WGM21);
	}
	
}

void pwm_start(uint8_t pwm_no)
{
	
	if (pwm_no & PWM0)
	{
		//set the pin OC1A as output
		DIO_SET_DIR(DIO_OC1A, DIO_OUTPUT);
		//set OC1A pin to fast PWM mode
		TCCR1A &= ~(1 << COM1A0);
		TCCR1A |= (1 << COM1A1); 
	}
	
	if (pwm_no & PWM1)
	{
		//set the pin OC1B as output
		DIO_SET_DIR(DIO_OC1B, DIO_OUTPUT);
		//set OC1B pin to fast PWM mode
		TCCR1A &= ~(1 << COM1B0);
		TCCR1A |= (1 << COM1B1);
	}
	
	if (pwm_no & PWM2)
	{
		//set the pin OC2 as output
		DIO_SET_DIR(DIO_OC2, DIO_OUTPUT);
		//set OC2 pin to fast PWM mode
		TCCR2 |= (1 << COM21);
		TCCR2 &= ~(1 << COM20);
	}
	
	
	
}

void pwm_setDuty(uint8_t pwn_no, uint8_t duty_percent)
{
	/*[NOTE] :if the duty cycle is 100% putting the same number as 
		ICR1 value will cause A compare and overflow at the same time which 
		it will be always either set or clear for the whole time so we put it
	    equal to ICR1 -1
	*/
	if (pwn_no & PWM0)
	{
		
		if (duty_percent == 100)
		{
			OCR1A = ((ICR1L) + ((uint16_t)ICR1H << 8)) - 1;
		}
		else
		{
			//assign OCR1A(holds the compare value) to the duty cycle percentage
			OCR1A = ((uint32_t)((ICR1L) + ((uint16_t)ICR1H << 8)) * duty_percent)
			           / 100;
		}
		
	}
	
	//[NOTE]: same as PWM0
	if (pwn_no & PWM1)
	{
		
		if (duty_percent == 100)
		{
			OCR1B = ((ICR1L) + ((uint16_t)ICR1H << 8)) - 1;
		}
		else
		{
			//assign OCR1B(holds the compare value) to the duty cycle percentage
			OCR1B =((uint32_t)((ICR1L) + ((uint16_t)ICR1H << 8))  * duty_percent) 
			           / 100;
		}
		
	}
	
	/*[NOTE] :if the duty cycle is 100% putting the same number as 
		top value(256) will cause A compare and overflow at the same time which 
		it will be always either set or clear for the whole time so we put it
	    equal to TOP value 255
	*/
	if (pwn_no & PWM2)
	{
		
		if (duty_percent == 100)
		{
			OCR2 = 255;
		}
		else
		{
			//assign OCR2(holds the compare value) to the duty cycle percentage
			OCR2 = (255 * (uint16_t)duty_percent) / 100;
		}
		
	}

}


void pwm_stop(uint8_t pwm_no)
{

	if (pwm_no & PWM0)
	{
		//disconnect OCR1A from fast PWM mode
		TCCR1A &= ~((1 << COM1A0) | (1 << COM1A1));
		DIO_SET_VAL(DIO_OC1A, DIO_LOW);
	}
	
	if (pwm_no & PWM1)
	{
		//disconnect OCR1B from fast PWM mode
		TCCR1A &= ~((1 << COM1B0) | (1 << COM1B1));
		DIO_SET_VAL(DIO_OC1B, DIO_LOW);
	}
	
	if (pwm_no & PWM2)
	{
		//disconnect OCR2 from fast PWM mode
		TCCR2 &= ~((1 << COM20) | (1 << COM21));
		DIO_SET_VAL(DIO_OC2, DIO_LOW);
	}
	
}

#if 0
void pwm_changeFreq(uint8_t pwm_no, uint16_t freq)
{
	if ((pwm_no & PWM0) || (pwm_no & PWM1))
	{
		float A_value = (float)OCR1A/ ((ICR1L) + ((uint32_t)ICR1H << 8));
		float B_value = (float)OCR1B/ ((ICR1L) + ((uint32_t)ICR1H << 8));
		pwm_init(pwm_no, freq);
		OCR1A = (uint16_t)(A_value*((ICR1L) + ((uint32_t)ICR1H << 8)));
		OCR1B = (uint16_t)(B_value*((ICR1L) + ((uint32_t)ICR1H << 8)));
	}
	else if (pwm_no & PWM2)
	{
		pwm_init(pwm_no, freq);
	}
	
}
#endif

#endif //defined(ATMEGA8)

/*
uint8_t PWM1_setFreqReg(uint16_t fr,uint16_t prescaler)
{
	uint16_t b=0,t=65535,n;
	if(fr < t)
	{

		n=GET_HALF;
		for (;;)
		{
			if(fr < n) //meaning we have to go to the b0ttom half
			{
				t=n;
				n=GET_HALF;
				PORTC = (uint8_t)n;
				PORTD = n>>8;
				delay_msec(250);
			}
			else // meaning we have to go to the top half
			{
				b=n;
				n=GET_HALF;
				PORTC = (uint8_t)n;
				PORTD = n>>8;
				delay_msec(250);
			}
		}

	}

	return FALSE;s
}*/