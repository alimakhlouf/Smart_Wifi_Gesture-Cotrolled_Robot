/* ---------------------------------------------------------------------------------------------------
[FILE NAME]: pwm1.c
[AUTHOR(S)]: Ali Makhlouf
[DATE CREATED]: 10/5/2017
[DESCRIPTION]: timer1 PWM library for both OCR1A and OCR1A pins
--------------------------------------------------------------------------------------------------- */

#include "pwm1.h"

#if defined(ATMEGA32) || defined(ATMEGA16)
void pwm_init(uint8_t pwm_no, uint16_t freq) // sets the frequency ..and setes the timer as 
{
	if (pwm_no & PWM0)
	{
		TCCR0 &= ~(0x07); // clear any previous prescalers
		
		if(NUM_REQUIRED_TICKS_NO_PRE < 255) //apply the frequency formula on prescaler = 1 and get ICR1 value if it's larger than what ICR1 register can hold(this means that the frequency we want too low to use prescaler=1 for it) we have to increase the prescaler and apply the formulA again
		{
			TCCR0 = (1<<CS00);
		}
		else if(NUM_REQUIRED_TICKS_8_PRE < 255) //apply the frequency formula on prescaler = 8 and get ICR1 value if it's larger than what ICR1 register can hold(this means that the frequency we want too low to use prescaler = 8 for it) we have to increase the prescaler and apply the formulA again
		{
			TCCR0 |= (1<<CS01); //
		}
		else if(NUM_REQUIRED_TICKS_64_PRE < 255)//apply the frequency formula on prescaler = 64 and get ICR1 value if it's larger than what ICR1 register can hold(this means that the frequency we want too low to use prescaler=64 for it) we have to increase the prescaler and apply the formulA again
		{
			TCCR0 |= (1<<CS10)|(1<<CS11); //
		}
		else if(NUM_REQUIRED_TICKS_256_PRE < 255)//apply the frequency formula on prescaler = 256 and get ICR1 value if it's larger than what ICR1 register can hold(this means that the frequency we want too low to use prescaler=256 for it) we have to increase the prescaler and apply the formulA again
		{
			TCCR0 |= (1<<CS02); //
		}
		else if(NUM_REQUIRED_TICKS_1024_PRE < 255)//apply the frequency formulA on prescaler=1 and get ICR1 value if it's larger than what ICR1 register can hold(this means that the frequency we want too low to use prescaler=1 for it) we have to increase the prescaler and apply the formulA again
		{
			TCCR0 |= (1<<CS00)|(1<<CS02);
		}

		TCCR0 |= (1<<WGM00) | (1<<WGM01); // SET THE BIT (FAST PWM WITH ICR1 AS TOP)
	}
	
	if ((pwm_no & (PWM1)) || (pwm_no & (PWM2)))
	{
		TCCR1A &= ~(0x07); // clear any previous pre scalers
		uint16_t ICR1_buffer = NUM_REQUIRED_TICKS_NO_PRE; // default value
		if(NUM_REQUIRED_TICKS_NO_PRE < (uint32_t)65535) //apply the frequency formula on prescaler = 1 and get ICR1 value if it's larger than what ICR1 register can hold(this means that the frequency we want too low to use prescaler=1 for it) we have to increase the prescaler and apply the formulA again
		{
			TCCR1B = (1<<CS10);
			ICR1_buffer = NUM_REQUIRED_TICKS_NO_PRE;
		}
		else if(NUM_REQUIRED_TICKS_8_PRE < (uint32_t)65535) //apply the frequency formulA on prescaler = 8 and get ICR1 value if it's larger than what ICR1 register can hold(this means that the frequency we want too low to use prescaler = 8 for it) we have to increase the prescaler and apply the formulA again
		{
			TCCR1B |= (1<<CS11); //
			ICR1_buffer = NUM_REQUIRED_TICKS_8_PRE;
		}
		else if(NUM_REQUIRED_TICKS_64_PRE < (uint32_t)65535)//apply the frequency formulA on prescaler = 64 and get ICR1 value if it's larger than what ICR1 register can hold(this means that the frequency we want too low to use prescaler=64 for it) we have to increase the prescaler and apply the formulA again
		{
			TCCR1B |= (1<<CS10)|(1<<CS11); //
			ICR1_buffer = NUM_REQUIRED_TICKS_64_PRE;
		}
		else if(NUM_REQUIRED_TICKS_256_PRE < (uint32_t)65535)//apply the frequency formulA on prescaler=256 and get ICR1 value if it's larger than what ICR1 register can hold(this means that the frequency we want too low to use prescaler=256 for it) we have to increase the prescaler and apply the formulA again
		{
			TCCR1B |= (1<<CS12); //
			ICR1_buffer = NUM_REQUIRED_TICKS_256_PRE;
		}
		else if(NUM_REQUIRED_TICKS_1024_PRE < (uint16_t)65535)//apply the frequency formulA on prescaler=1 and get ICR1 value if it's larger than what ICR1 register can hold(this means that the frequency we want too low to use prescaler=1 for it) we have to increase the prescaler and apply the formulA again
		{
			TCCR1B |= (1<<CS10)|(1<<CS12); //
			ICR1_buffer = NUM_REQUIRED_TICKS_1024_PRE;
		}

		ICR1H=ICR1_buffer >> 8;
		ICR1L=(ICR1_buffer & 0x00ff);
		TCCR1B |= (1<<WGM12)|(1<<WGM13); // SET THE BIT (FAST PWM WITH ICR1 AS TOP)
		TCCR1A |= (1<<WGM11);
		TCCR1A &= ~(1 << WGM10);
	}
	
	
	
	if (pwm_no & PWM3)
	{
		TCCR2 &= ~(0x07); // clear any previous pre scalers
		
		if(NUM_REQUIRED_TICKS_NO_PRE < 255) //apply the frequency formula on prescaler = 1 and get ICR1 value if it's larger than what ICR1 register can hold(this means that the frequency we want too low to use prescaler=1 for it) we have to increase the prescaler and apply the formulA again
		{
			TCCR2 = (1 << CS20);
		}
		else if(NUM_REQUIRED_TICKS_8_PRE < 255) //apply the frequency formula on prescaler = 8 and get ICR1 value if it's larger than what ICR1 register can hold(this means that the frequency we want too low to use prescaler = 8 for it) we have to increase the prescaler and apply the formulA again
		{
			TCCR2 |= (1<<CS21); //
		}
		else if(NUM_REQUIRED_TICKS_32_PRE < 255) //apply the frequency formula on prescaler = 8 and get ICR1 value if it's larger than what ICR1 register can hold(this means that the frequency we want too low to use prescaler = 8 for it) we have to increase the prescaler and apply the formulA again
		{
			TCCR2 |= (1 << CS20) | (1 << CS21); //
		}
		else if(NUM_REQUIRED_TICKS_64_PRE < 255)//apply the frequency formula on prescaler = 64 and get ICR1 value if it's larger than what ICR1 register can hold(this means that the frequency we want too low to use prescaler=64 for it) we have to increase the prescaler and apply the formulA again
		{
			TCCR2 |= (1 << CS22); //
		}
		else if(NUM_REQUIRED_TICKS_128_PRE < 255)//apply the frequency formula on prescaler = 256 and get ICR1 value if it's larger than what ICR1 register can hold(this means that the frequency we want too low to use prescaler=256 for it) we have to increase the prescaler and apply the formulA again
		{
			TCCR2 |= (1 << CS20) | (1 << CS22); //

		}
		else if(NUM_REQUIRED_TICKS_256_PRE < 255)//apply the frequency formula on prescaler = 256 and get ICR1 value if it's larger than what ICR1 register can hold(this means that the frequency we want too low to use prescaler=256 for it) we have to increase the prescaler and apply the formulA again
		{
			TCCR2 |= (1 << CS21) | (1 << CS22); //
		}
		else if(NUM_REQUIRED_TICKS_1024_PRE < 255)//apply the frequency formulA on prescaler=1 and get ICR1 value if it's larger than what ICR1 register can hold(this means that the frequency we want too low to use prescaler=1 for it) we have to increase the prescaler and apply the formulA again
		{
			TCCR2 |= (1 << CS20) | (1 << CS22) | (1 << CS21);    
		}

		TCCR2 |= (1<<WGM20) | (1<<WGM21); // SET THE BIT (FAST PWM WITH ICR1 AS TOP)
	}
	
}

void pwm_start(uint8_t pwm_no)
{
	if (pwm_no & PWM0)
	{
		DIO_SET_DIR(DIO_OC0, DIO_OUTPUT);
		TCCR0 |= (1 << COM01);
		TCCR0 &= ~(1<<COM00);
	}
	
	if (pwm_no & PWM1)
	{
		DIO_SET_DIR(DIO_OC1A, DIO_OUTPUT);
		TCCR1A &= ~(1 << COM1A0);
		TCCR1A |= (1 << COM1A1); // SET THE BIT( START CLEAR ON COMPARE) (FAST PWM WITH ICR1 AS TOP) respectively
	}
	
	if (pwm_no & PWM2)
	{
		DIO_SET_DIR(DIO_OC1B, DIO_OUTPUT);
		TCCR1A &= ~(1 << COM1B0);
		TCCR1A |= (1 << COM1B1); // SET THE BIT ON (SET ON START, CLEAR ON COMPARE) (FAST PWM WITH ICR1 AS TOP)
	}
	
	if (pwm_no & PWM3)
	{
		DIO_SET_DIR(DIO_OC2, DIO_OUTPUT);
		DDRD |= (1 << 7);
		TCCR2 |= (1 << COM21);
		TCCR2 &= ~(1 << COM20);
	}
	
	
}

void pwm_setDuty(uint8_t pwn_no, uint8_t duty_percent)
{
	if (pwn_no & PWM0)
	{
		if (duty_percent == 100) // if the duty cycle is 100% putting OCR1A the same number as as top value will cause A compare and overflow at the same time which only one of the is going to be activated so we put it equal TOP value( which held in ICR1)-1
			OCR0 = 255;
		else
			OCR0 = (255 * (uint16_t)duty_percent) / 100;
	}
	
	if (pwn_no & PWM1)    
	{
		if (duty_percent == 100) // if the duty cycle is 100% putting OCR1A the same number as as top value will cause A compare and overflow at the same time which only one of the is going to be activated so we put it equal TOP value( which held in ICR1)-1
			OCR1A = ((ICR1L) + ((uint16_t)ICR1H << 8)) - 1;
		else
			OCR1A = ((uint32_t)((ICR1L) + ((uint16_t)ICR1H << 8)) * duty_percent) / 100;
	}
	
	if (pwn_no & PWM2)    
	{
		if (duty_percent == 100)// if the duty cycle is 100% putting OCR1A the same number as as top value will cause A compare and overflow at the same time which only one of the is going to be activated so we put it equal TOP value( which held in ICR1)-1
			OCR1B = ((ICR1L) + ((uint16_t)ICR1H << 8)) - 1;
		else
			OCR1B =((uint32_t)((ICR1L) + ((uint16_t)ICR1H << 8))  * duty_percent) / 100;
	}
	
	if (pwn_no & PWM3)
	{
		if (duty_percent == 100) // if the duty cycle is 100% putting OCR1A the same number as as top value will cause A compare and overflow at the same time which only one of the is going to be activated so we put it equal TOP value( which held in ICR1)-1
			OCR2 = 255;
		else
			OCR2 = (255 * (uint16_t)duty_percent) / 100;
	}

}


void pwm_stop(uint8_t pwm_no)
{
	if (pwm_no & PWM0)
	{
		TCCR0 &= ~((1 << COM00) | (1 << COM01)); 
		DIO_SET_VAL(DIO_OC1A, DIO_LOW); 
	}
	
	if (pwm_no & PWM1)
	{
		TCCR1A &= ~((1 << COM1A0) | (1 << COM1A1));
		DIO_SET_VAL(DIO_OC1A, DIO_LOW);
	}
	
	if (pwm_no & PWM2)
	{
		TCCR1A &= ~((1 << COM1B0) | (1 << COM1B1));
		DIO_SET_VAL(DIO_OC1B, DIO_LOW);
	}
	
	if (pwm_no & PWM3)
	{
		TCCR2 &= ~((1 << COM20) | (1 << COM21));
		DIO_SET_VAL(DIO_OC2, DIO_LOW);
	}
	else
	{
		
	}
	
}

void pwm_changeFreq(uint8_t pwm_no, uint16_t freq)
{
	if ((pwm_no & PWM1) || (pwm_no & PWM2))
	{
		float A_value = (float)OCR1A/ ((ICR1L) + ((uint32_t)ICR1H << 8));
		float B_value = (float)OCR1B/ ((ICR1L) + ((uint32_t)ICR1H << 8));
		pwm_init(pwm_no, freq);
		OCR1A = (uint16_t)(A_value*((ICR1L) + ((uint32_t)ICR1H << 8)));
		OCR1B = (uint16_t)(B_value*((ICR1L) + ((uint32_t)ICR1H << 8)));
	}
	
	if ((pwm_no & PWM0) || (pwm_no & PWM3));
	{
		pwm_init(pwm_no, freq);
	}
	
}







#elif defined(ATMEGA8)
void pwm_init(uint8_t pwm_no, uint16_t freq) // sets the frequency ..and setes the timer as
{
		
	if ((pwm_no & (PWM0)) || (pwm_no & (PWM1)))
	{
		TCCR1A &= ~(0x07); // clear any previous pre scalers
		uint16_t ICR1_buffer = NUM_REQUIRED_TICKS_NO_PRE; // default value
		if(NUM_REQUIRED_TICKS_NO_PRE < (uint32_t)65535) //apply the frequency formula on prescaler = 1 and get ICR1 value if it's larger than what ICR1 register can hold(this means that the frequency we want too low to use prescaler=1 for it) we have to increase the prescaler and apply the formulA again
		{
			TCCR1B = (1<<CS10);
			ICR1_buffer = NUM_REQUIRED_TICKS_NO_PRE;
		}
		else if(NUM_REQUIRED_TICKS_8_PRE < (uint32_t)65535) //apply the frequency formulA on prescaler = 8 and get ICR1 value if it's larger than what ICR1 register can hold(this means that the frequency we want too low to use prescaler = 8 for it) we have to increase the prescaler and apply the formulA again
		{
			TCCR1B |= (1<<CS11); //
			ICR1_buffer = NUM_REQUIRED_TICKS_8_PRE;
		}
		else if(NUM_REQUIRED_TICKS_64_PRE < (uint32_t)65535)//apply the frequency formulA on prescaler = 64 and get ICR1 value if it's larger than what ICR1 register can hold(this means that the frequency we want too low to use prescaler=64 for it) we have to increase the prescaler and apply the formulA again
		{
			TCCR1B |= (1<<CS10)|(1<<CS11); //
			ICR1_buffer = NUM_REQUIRED_TICKS_64_PRE;
		}
		else if(NUM_REQUIRED_TICKS_256_PRE < (uint32_t)65535)//apply the frequency formulA on prescaler=256 and get ICR1 value if it's larger than what ICR1 register can hold(this means that the frequency we want too low to use prescaler=256 for it) we have to increase the prescaler and apply the formulA again
		{
			TCCR1B |= (1<<CS12); //
			ICR1_buffer = NUM_REQUIRED_TICKS_256_PRE;
		}
		else if(NUM_REQUIRED_TICKS_1024_PRE < (uint16_t)65535)//apply the frequency formulA on prescaler=1 and get ICR1 value if it's larger than what ICR1 register can hold(this means that the frequency we want too low to use prescaler=1 for it) we have to increase the prescaler and apply the formulA again
		{
			TCCR1B |= (1<<CS10)|(1<<CS12); //
			ICR1_buffer = NUM_REQUIRED_TICKS_1024_PRE;
		}

		ICR1H=ICR1_buffer >> 8;
		ICR1L=(ICR1_buffer & 0x00ff);
		TCCR1B |= (1<<WGM12)|(1<<WGM13); // SET THE BIT (FAST PWM WITH ICR1 AS TOP)
		TCCR1A |= (1<<WGM11);
		TCCR1A &= ~(1 << WGM10);
	}
	
	
	
	if (pwm_no & PWM2)
	{
		TCCR2 &= ~(0x07); // clear any previous pre scalers
		
		if(NUM_REQUIRED_TICKS_NO_PRE < 255) //apply the frequency formula on prescaler = 1 and get ICR1 value if it's larger than what ICR1 register can hold(this means that the frequency we want too low to use prescaler=1 for it) we have to increase the prescaler and apply the formulA again
		{
			TCCR2 = (1 << CS20);
		}
		else if(NUM_REQUIRED_TICKS_8_PRE < 255) //apply the frequency formula on prescaler = 8 and get ICR1 value if it's larger than what ICR1 register can hold(this means that the frequency we want too low to use prescaler = 8 for it) we have to increase the prescaler and apply the formulA again
		{
			TCCR2 |= (1<<CS21); //
		}
		else if(NUM_REQUIRED_TICKS_32_PRE < 255) //apply the frequency formula on prescaler = 8 and get ICR1 value if it's larger than what ICR1 register can hold(this means that the frequency we want too low to use prescaler = 8 for it) we have to increase the prescaler and apply the formulA again
		{
			TCCR2 |= (1 << CS20) | (1 << CS21); //
		}
		else if(NUM_REQUIRED_TICKS_64_PRE < 255)//apply the frequency formula on prescaler = 64 and get ICR1 value if it's larger than what ICR1 register can hold(this means that the frequency we want too low to use prescaler=64 for it) we have to increase the prescaler and apply the formulA again
		{
			TCCR2 |= (1 << CS22); //
		}
		else if(NUM_REQUIRED_TICKS_128_PRE < 255)//apply the frequency formula on prescaler = 256 and get ICR1 value if it's larger than what ICR1 register can hold(this means that the frequency we want too low to use prescaler=256 for it) we have to increase the prescaler and apply the formulA again
		{
			TCCR2 |= (1 << CS20) | (1 << CS22); //

		}
		else if(NUM_REQUIRED_TICKS_256_PRE < 255)//apply the frequency formula on prescaler = 256 and get ICR1 value if it's larger than what ICR1 register can hold(this means that the frequency we want too low to use prescaler=256 for it) we have to increase the prescaler and apply the formulA again
		{
			TCCR2 |= (1 << CS21) | (1 << CS22); //
		}
		else if(NUM_REQUIRED_TICKS_1024_PRE < 255)//apply the frequency formulA on prescaler=1 and get ICR1 value if it's larger than what ICR1 register can hold(this means that the frequency we want too low to use prescaler=1 for it) we have to increase the prescaler and apply the formulA again
		{
			TCCR2 |= (1 << CS20) | (1 << CS22) | (1 << CS21);
		}

		TCCR2 |= (1<<WGM20) | (1<<WGM21); // SET THE BIT (FAST PWM WITH ICR1 AS TOP)
	}
	
}

void pwm_start(uint8_t pwm_no)
{
	
	if (pwm_no & PWM0)
	{
		DIO_SET_DIR(DIO_OC1A, DIO_OUTPUT);
		TCCR1A &= ~(1 << COM1A0);
		TCCR1A |= (1 << COM1A1); // SET THE BIT( START CLEAR ON COMPARE) (FAST PWM WITH ICR1 AS TOP) respectively
	}
	
	if (pwm_no & PWM1)
	{
		DIO_SET_DIR(DIO_OC1B, DIO_OUTPUT);
		TCCR1A &= ~(1 << COM1B0);
		TCCR1A |= (1 << COM1B1); // SET THE BIT ON (SET ON START, CLEAR ON COMPARE) (FAST PWM WITH ICR1 AS TOP)
	}
	
	if (pwm_no & PWM2)
	{
		DIO_SET_DIR(DIO_OC2, DIO_OUTPUT);;
		TCCR2 |= (1 << COM21);
		TCCR2 &= ~(1 << COM20);
	}
	
	
}

void pwm_setDuty(uint8_t pwn_no, uint8_t duty_percent)
{
	
	if (pwn_no & PWM0)
	{
		if (duty_percent == 100) // if the duty cycle is 100% putting OCR1A the same number as as top value will cause A compare and overflow at the same time which only one of the is going to be activated so we put it equal TOP value( which held in ICR1)-1
		OCR1A = ((ICR1L) + ((uint16_t)ICR1H << 8)) - 1;
		else
		OCR1A = ((uint32_t)((ICR1L) + ((uint16_t)ICR1H << 8)) * duty_percent) / 100;
	}
	
	if (pwn_no & PWM1)
	{
		if (duty_percent == 100)// if the duty cycle is 100% putting OCR1A the same number as as top value will cause A compare and overflow at the same time which only one of the is going to be activated so we put it equal TOP value( which held in ICR1)-1
		OCR1B = ((ICR1L) + ((uint16_t)ICR1H << 8)) - 1;
		else
		OCR1B =((uint32_t)((ICR1L) + ((uint16_t)ICR1H << 8))  * duty_percent) / 100;
	}
	
	if (pwn_no & PWM2)
	{
		if (duty_percent == 100) // if the duty cycle is 100% putting OCR1A the same number as as top value will cause A compare and overflow at the same time which only one of the is going to be activated so we put it equal TOP value( which held in ICR1)-1
		OCR2 = 255;
		else
		OCR2 = (255 * (uint16_t)duty_percent) / 100;
	}

}


void pwm_stop(uint8_t pwm_no)
{

	if (pwm_no & PWM0)
	{
		TCCR1A &= ~((1 << COM1A0) | (1 << COM1A1));
		DIO_SET_VAL(DIO_OC1A, DIO_LOW);
	}
	
	if (pwm_no & PWM1)
	{
		TCCR1A &= ~((1 << COM1B0) | (1 << COM1B1));
		DIO_SET_VAL(DIO_OC1B, DIO_LOW);
	}
	
	if (pwm_no & PWM2)
	{
		TCCR2 &= ~((1 << COM20) | (1 << COM21));
		DIO_SET_VAL(DIO_OC2, DIO_LOW);
	}
	else
	{
		
	}
	
}

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