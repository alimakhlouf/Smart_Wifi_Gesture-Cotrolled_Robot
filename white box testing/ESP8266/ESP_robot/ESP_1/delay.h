/* ----------------------------------------------------------------------------
[FILE NAME]:    delay.h

[AUTHOR]:       Ali MAkhlouf

[DATE CREATED]: 6/13/2019 03:04:00

[DESCRIPTION]:  this file contains functions that perform software delay, it
                has different version of the same functinos for different
				 micro controllers
------------------------------------------------------------------------------*/


#ifndef DELAY_H_
#define DELAY_H_


#include "micro_config.h"
#include "math.h"

//if the mcu type is either atmega16 or atmega32 or atmega8
#if defined(ATMEGA16) || defined(ATMEGA32) || defined(ATMEGA8)

//if no code optimization enabled ... display a warning 
#ifndef __OPTIMIZE__
#warning you need to enable optimization in order to get accurate delay periods
#endif

/* ----------------------------------------------------------------------------
[Function Name] : delay_micro
[Description] : -This function is responsible performing a software delay in
microseconds, it works in the the the HAL layer
-there are different versions of code of this function for
different MCUs.
[Args] :
[in] double a__us :
This Arg shall indicate the delay time in microsseconds
---------------------------------------------------------------------------- */
static inline void delay_micro(double a__us) __attribute__((always_inline));

/* ----------------------------------------------------------------------------
[Function Name] : delay_milli
[Description] : -This function is responsible performing a software delay in
milliseconds, it works in the the the HAL layer
-there are different versions of code of this function for 
 different MCUs.
[Args] :
[in] double a__ms:
This Arg shall indicate the delay time in milliseconds
[NOTE]: the maximum delay achievable by this function is 65535 milliseconds. if
you go higher than that the resulted delay will be wrong(much shorter than the
actual time  
---------------------------------------------------------------------------- */
static inline void delay_milli(double a__ms) __attribute__((always_inline));

/* ----------------------------------------------------------------------------
[Function Name] : _delay_loop1
[Description] : This function is responsible performing an avr loop each loop
takes 3 cycles .. it's accurate so it's usually called by the delay_micro  
function as it takes a very few cycles to execute
                
[Args] : 
[in] uint8_t a__count :
This Arg shall indicate the number of loops to be performed 
 
NOTE: we choose an byte input in order to reduce the time of receiving the
argument to one cycle

-__attribute__((always_inline)): It forces the compiler to inline the function
 even if optimizations are disabled 
---------------------------------------------------------------------------- */
static inline void _delay_loop1(uint8_t a__count) __attribute__((always_inline));

/* ----------------------------------------------------------------------------
[Function Name] : _delay_loop2
[Description] : -his function is responsible performing an avr loop, each loop
takes 4 cycles
-it's called by the delay_micro function as it takes a very few cycles to 
execute(4 cycles)

[Args] :
[in] uint16_t a__count :
This Arg shall indicate the number of loops to be performed 

NOTE: we choose an 2-byte (unlike the _delay_loop1 function) because:
1-it's used in  less critical delays times that required less accurate time 
2-to achieve a higher number of loops so we can make a longer delay per loop and
eventually a longer daely time, so at 1 Mhz frequency the maximal delay for one
call of _delay_loop2 is 262 milliseconds for delay_milli and 262 microseconds
can achieve a longer number of
--__attribute__((always_inline)): It forces the compiler to inline the function
even if optimizations are disabled
---------------------------------------------------------------------------- */
static inline void _delay_loop2(uint16_t a__count) __attribute__((always_inline));


void _delay_loop1(uint8_t a__count)
{
	//to write an assemble code, it needs to be put in this form of block 
	__asm__ volatile (
	"1: dec %0" "\n\t" // decrement the first argument which is the counter
	"brne 1b"          // branch when this counter reaches zero to 1:
	: "=r" (a__count)
	: "0" (a__count)
	);
}

void _delay_loop2(uint16_t __count)
{
	//to write an assemble code, it needs to be put in this form of block 
	__asm__ volatile ( 
	"1: sbiw %0,1" "\n\t" //subtract the first argument(which is a word)
	"brne 1b"             // branch when this counter reaches zero to 1:
	: "=w" (__count)
	: "0" (__count)
	);
}

void delay_milli(double a_ms_time)
{
	/*calculate the number of loops needed to be make the time delay in ms
	* a loop is 4 cycles so to calculate the number of loops is to know
	* how many cycles it takes to generate the delay time by dividing the CPU
	* frequency by 1000 to get number of cycles for 1 ms.. and then multiply it
	* by the number of milliseconds required(ms_time) .. and to get the num of
	* loops we divide it by the the number of cycles the loop takes(4 cycles) 
	*/
	double tmp = (((F_CPU) / (4 * 1000)) * a_ms_time);//4 is num cycles per loop
	
	//this variable holds the counter that will be passed to the loop function
	uint16_t loop_count = 0;
	
	// if the number of loops is less than one 
	if (tmp < 1.0) 
	{
		loop_count = 1;//make it loop once
		_delay_loop2(loop_count);
	}//else if the number of loops exceeds(the ticks)an ovf will happen.To 
	//avoid this we making the loop function perform a 1/10 ms delay time and 
	//then repeat it by the number of ms delay required * 10.Now we've achieved
	//the delay time in another way but it's less accurate 
	//it's maximum time is 6.5535 seconds 
	
	else if (tmp > 65535)//dividing the needed loops into 10  and then run
	{  //the loop 10 times in each one only do 1/10 of the actual number of loops
		loop_count = (uint16_t)(a_ms_time * 10.0);//
		
		while(loop_count)//repeat a_ms_time*10 times each time make a 1/10 ms delay
		{
			_delay_loop2(((F_CPU) / (4 * 1000)) / 10);
			loop_count-- ;
		}
		
	}
	else //(tmp > 65535 && tmp > 1)
	{
		//put the number of loops in the function as it is
		loop_count = (uint16_t)tmp; 
		_delay_loop2(loop_count);
	}
	

}

void delay_micro(double a_us_time)
{
	/*calculate the number of loops needed to be make the time delay in us using
	*2 methods one that takes 3 cycles per loop(using _delay_us_loop which is more
	*accurate but uses 4 loop that repeats only up to 255 counts) and the other
	*that takes 4 cycles(using _delay_us_loop which is less accurate but could  
	*repeat up to 65535 counts so it can ake longer delays).So if the num of  
	*loop count exceeds 255 it use the 4 cycles loop function. And if the count
	*exceeds 65535 loops call the delay_milli function instead
	*/
	
	//calculating the num loops required( using a 3 cycle loop)
	double tmp_8 = (((F_CPU) / (3 * 1000000)) * a_us_time);
	//calculating the num loops required( using a 4 cycle loop)
	double tmp_16  = (((F_CPU) / (4 * 1000000)) * a_us_time);
	
	//holds the final value of number of loops that will be passed to the function
	uint8_t num_loops_8_bits;
	
	if (tmp_8 < 1.0)
	{
		num_loops_8_bits = 1;
		_delay_loop1(num_loops_8_bits);
	}
	else if (tmp_16 > 65535)
	{
		delay_milli(a_us_time / 1000.0);
	}
	else if (tmp_8 > 255)//if the num of loops required is between 255 and 65535
	{
		
		uint16_t num_loops_16_bits = (uint16_t)tmp_16;
		_delay_loop2(num_loops_16_bits);
		return;
	}
	else//if the number of loops is within 255, you can use the 255 
	{   //loops function _delay_us_loop with 3 cycles which is more accurate
		num_loops_8_bits = (uint8_t)tmp_8;
		_delay_loop1(num_loops_8_bits);
	}
	
	
}

#endif //defined(ATMEGA16) || defined(ATMEGA32) || defined(ATMEGA8)


#endif /* DELAY_H_ */