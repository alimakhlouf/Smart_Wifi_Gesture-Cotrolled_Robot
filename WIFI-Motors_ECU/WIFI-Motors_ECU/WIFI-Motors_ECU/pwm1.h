/* ---------------------------------------------------------------------------------------------------
[FILE NAME]: pwm1.h
[AUTHOR(S)]: Ali Makhlouf
[DATE CREATED]: 10/5/2017
[DESCRIPTION]: timer1 PWM library for both OCR1A and OCR1A pins
--------------------------------------------------------------------------------------------------- */

#ifndef PWM1_H_
#define PWM1_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"
#include "dio_api.h"

#define FALSE 0
#define TRUE 1


#if defined(ATMEGA32) || defined(ATMEGA16) || defined(ATMEGA8)

#define PWM0 1  
#define PWM1 2
#define PWM2 4
#define PWM3 8

#if defined(ATMEGA32) || defined(ATMEGA16)
#define DIO_OC0  3  //PWM0
#define DIO_OC1A 13 //PWM1
#define DIO_OC1B 12 //PWM2
#define DIO_OC2  15 //PWM3

#elif defined(ATMEGA8)
#define DIO_OC1A 12
#define DIO_OC1B 13
#define DIO_OC2  14
#endif

//the formulA of getting ICR1 value is (F_CPU / ((uint32)prescaler * freq))
#define NUM_REQUIRED_TICKS_NO_PRE (F_CPU / ((uint32_t)1 * freq))
//here every time we use one of these macros we assign it's value to ICR1_buffer so we move from prescaler to the next by dividing (prescaler of the macro/previous prescaler)
#define NUM_REQUIRED_TICKS_8_PRE  (F_CPU / ((uint32_t)8 * freq)) //THE VALUE OF ICR1_buffer(NUM_REQUIRED_TICKS_NO_PRE) divided by 8(8/1=8)
#define NUM_REQUIRED_TICKS_32_PRE (F_CPU / ((uint32_t)32 * freq))//THE VALUE OF ICR1_buffer(NUM_REQUIRED_TICKS_NO_PRE) divided by 256(256/1=256)
#define NUM_REQUIRED_TICKS_64_PRE (F_CPU / ((uint32_t)64 * freq)) //THE VALUE OF ICR1_buffer(NUM_REQUIRED_TICKS_NO_PRE) divided by 64(64/1=64)
#define NUM_REQUIRED_TICKS_128_PRE (F_CPU / ((uint32_t)128 * freq))//THE VALUE OF ICR1_buffer(NUM_REQUIRED_TICKS_NO_PRE) divided by 256(256/1=256)
#define NUM_REQUIRED_TICKS_256_PRE (F_CPU / ((uint32_t)256 * freq))//THE VALUE OF ICR1_buffer(NUM_REQUIRED_TICKS_NO_PRE) divided by 256(256/1=256)
#define NUM_REQUIRED_TICKS_1024_PRE (F_CPU / ((uint32_t)1024 * freq))//THE VALUE OF ICR1_buffer(NUM_REQUIRED_TICKS_NO_PRE) divided by 1024(1024/1=1024)



/* ---------------------------------------------------------------------------------------------------------------
[Function Name] : pwm_init
[Description] : This function is responsible for initialising PWM1 by putting OCR1A as output direction
				and set TIMER1 peripheral as A fast PWM (with ICR1 as the top value in order to help us set the
				system match the entered frequency accurately
---------------------------------------------------------------------------------------------------------------*/
void pwm_init(uint8_t pwm_no, uint16_t freq);



/* ---------------------------------------------------------------------------------------------------------------
[Function Name] : PWM1B_init
[Description] : This function is responsible for initialising PWM1 by putting OCR1A as output direction
				and set TIMER1 peripheral as A fast PWM (with ICR1 as the top value in order to help us make the
				PWM frequency match the entered frequency accurately
---------------------------------------------------------------------------------------------------------------*/
//void PWM1B_init();


/* ---------------------------------------------------------------------------------------------------------------
[Function Name] : PWM1_start
[Description] : This function is responsible for starting the PWM by putting the bits in the prescaler this
				happens after it calculates the value that must be put in ICR1(which has the TOP value that the
				timer will cause an overflow there ,and prescaler  in order the produce
				the specified frequency and
---------------------------------------------------------------------------------------------------------------*/
void pwm_start(uint8_t pwm_no);


/* ---------------------------------------------------------------------------------------------------------------
[Function Name] : PWM1_changeFreq
[Description] : This function is responsible for changing the frequency of the PWM

[out]: ICR1 register
this argument is responsible for holding the TOP value that the timer will cause an overflow when TCNT1 reaches it

TCCR1A :
this argument contains the prescaler bits
---------------------------------------------------------------------------------------------------------------*/
void pwm_changeFreq(uint8_t pwm_no, uint16_t freq);



/* ---------------------------------------------------------------------------------------------------------------
[Function Name] : PWM1A_setDuty
[Description] : This function is responsible for setting the duty cycle of PWM1

[out]: OCR1A register
this argument is responsible for holding the TOP value that the timer will cause an overflow when TCNT1 reaches it
---------------------------------------------------------------------------------------------------------------*/
void pwm_setDuty(uint8_t pwn_no, uint8_t duty_percent);
//NEVER CALL THE DUTY FUNCTION BEFORE START FUNCTION BECAUSE IN the duty depends on  the top value(ICR1) which is assigned in the start function
//THIS CASE WE DON'T KNOW THE TOP VALUE OF THE ICR1 REG



/* ---------------------------------------------------------------------------------------------------------------
[Function Name] : PWM1B_setDuty
[Description] : This function is responsible for setting the duty cycle of PWM1

[out]: OCR1A register
this argument is responsible for holding the TOP value that the timer will cause an overflow when TCNT1 reaches it
---------------------------------------------------------------------------------------------------------------*/
//void PWM1B_setDuty(uint8_t duty_percent);
//NEVER CALL THE DUTY FUNCTION BEFORE START FUNCTION BECAUSE IN  duty depends on  the top value(ICR1) which is assigned in the start function
//THIS CASE WE DON'T KNOW THE TOP VALUE OF THE ICR1 REG 



/* ---------------------------------------------------------------------------------------------------------------
[Function Name] : PWM1_stop
[Description] : This function is responsible for stopping TIMER1 the PWM by clearing the bits in the prescaler this

[out]: TCCR1B
this argument contains the prescaler bits which are cleared to stop TIMER1
---------------------------------------------------------------------------------------------------------------*/
void pwm_stop(uint8_t pwn_no);

#endif // if defined(ATMEGA32) || defined || defined

#endif /* PWM1_H_ */
