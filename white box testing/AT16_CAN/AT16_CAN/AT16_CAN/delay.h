/*
 * delay.h
 *
 * Created: 6/13/2019 03:04:00 م
 *  Author: hppp
 */ 


#ifndef DELAY_H_
#define DELAY_H_


#include "micro_config.h"
#include "math.h"


#if defined(ATMEGA16) || defined(ATMEGA32) || defined(ATMEGA8)

#ifndef __OPTIMIZE__
#warning you need to enable optimization in order to get accurate delay periods
#endif

static inline void delay_micro(double __us) __attribute__((always_inline));
static inline void delay_milli(double __ms) __attribute__((always_inline));
static inline void _delay_us_loop(uint8_t __count) __attribute__((always_inline));
static inline void _delay_ms_loop(uint16_t __count) __attribute__((always_inline));


void _delay_us_loop(uint8_t __count)
{
	__asm__ volatile (
	"1: dec %0" "\n\t"
	"brne 1b"
	: "=r" (__count)
	: "0" (__count)
	);
}

void _delay_ms_loop(uint16_t __count)
{
	__asm__ volatile (
	"1: sbiw %0,1" "\n\t"
	"brne 1b"
	: "=w" (__count)
	: "0" (__count)
	);
}

void delay_milli(double ms_time)
{
	double tmp = (((F_CPU) / (4 * 1000)) * ms_time);
	uint16_t ticks;
	if (tmp < 1.0)
	{
		ticks = 1;
	}
	else if (tmp > 65535)
	{
		ticks = (uint16_t)(ms_time * 10.0);
		
		while(ticks)
		{
			_delay_ms_loop(((F_CPU) / (4 * 1000)) / 10);
			ticks-- ;
		}
		return;
		
	}
	else
	{
		ticks = (uint16_t)tmp;
	}
	
	_delay_ms_loop(ticks);

}

void delay_micro(double us_time)
{
	double tmp_16  = (((F_CPU) / (4 * 1000000)) * us_time);
	double tmp_8 = (((F_CPU) / (3 * 1000000)) * us_time);
	uint8_t ticks;
	
	if (tmp_8 < 1.0)
	{
		ticks = 1;
	}
	else if (tmp_16 > 65535)
	{
		delay_milli(us_time / 1000.0);
	}
	else if (tmp_8 > 255)
	{
		uint16_t ticks = (uint16_t)tmp_16;
		_delay_ms_loop(ticks);
		return;
	}
	else
	{
		ticks = (uint8_t)tmp_8;
	}
	
	_delay_us_loop(ticks);
}

#endif //defined(ATMEGA16) || defined(ATMEGA32) || defined(ATMEGA8)


#endif /* DELAY_H_ */