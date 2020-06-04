/*
 * pwm_api.h
 *
 * Created: 6/14/2019 11:22:48 ص
 *  Author: hppp
 */ 


#ifndef PWM_API_H_
#define PWM_API_H_


#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

#define FALSE 0
#define TRUE 1






/* ---------------------------------------------------------------------------------------------------------------
[Function Name] : PWM1A_init
[Description] : This function is responsible for initialising PWM1 by putting OCR1A as output direction
				and set TIMER1 peripheral as A fast PWM (with ICR1 as the top value in order to help us set the
				system match the entered frequency accurately
---------------------------------------------------------------------------------------------------------------*/
void PWM1A_init();



/* ---------------------------------------------------------------------------------------------------------------
[Function Name] : PWM1B_init
[Description] : This function is responsible for initialising PWM1 by putting OCR1A as output direction
				and set TIMER1 peripheral as A fast PWM (with ICR1 as the top value in order to help us make the
				PWM frequency match the entered frequency accurately
---------------------------------------------------------------------------------------------------------------*/
void PWM1B_init();


/* ---------------------------------------------------------------------------------------------------------------
[Function Name] : PWM1_start
[Description] : This function is responsible for starting the PWM by putting the bits in the prescaler this
				happens after it calculates the value that must be put in ICR1(which has the TOP value that the
				timer will cause an overflow there ,and prescaler  in order the produce
				the specified frequency and
---------------------------------------------------------------------------------------------------------------*/
void PWM1_start(uint16_t freq);


/* ---------------------------------------------------------------------------------------------------------------
[Function Name] : PWM1_changeFreq
[Description] : This function is responsible for changing the frequency of the PWM

[out]: ICR1 register
this argument is responsible for holding the TOP value that the timer will cause an overflow when TCNT1 reaches it

TCCR1A :
this argument contains the prescaler bits
---------------------------------------------------------------------------------------------------------------*/
void PWM1_changeFreq(uint16_t freq);



/* ---------------------------------------------------------------------------------------------------------------
[Function Name] : PWM1A_setDuty
[Description] : This function is responsible for setting the duty cycle of PWM1

[out]: OCR1A register
this argument is responsible for holding the TOP value that the timer will cause an overflow when TCNT1 reaches it
---------------------------------------------------------------------------------------------------------------*/
void PWM1A_setDuty(uint8_t duty_percent);
//NEVER CALL THE DUTY FUNCTION BEFORE START FUNCTION BECAUSE IN the duty depends on  the top value(ICR1) which is assigned in the start function
//THIS CASE WE DON'T KNOW THE TOP VALUE OF THE ICR1 REG



/* ---------------------------------------------------------------------------------------------------------------
[Function Name] : PWM1B_setDuty
[Description] : This function is responsible for setting the duty cycle of PWM1

[out]: OCR1A register
this argument is responsible for holding the TOP value that the timer will cause an overflow when TCNT1 reaches it
---------------------------------------------------------------------------------------------------------------*/
void PWM1B_setDuty(uint8_t duty_percent);
//NEVER CALL THE DUTY FUNCTION BEFORE START FUNCTION BECAUSE IN  duty depends on  the top value(ICR1) which is assigned in the start function
//THIS CASE WE DON'T KNOW THE TOP VALUE OF THE ICR1 REG 



/* ---------------------------------------------------------------------------------------------------------------
[Function Name] : PWM1_stop
[Description] : This function is responsible for stopping TIMER1 the PWM by clearing the bits in the prescaler this

[out]: TCCR1B
this argument contains the prescaler bits which are cleared to stop TIMER1
---------------------------------------------------------------------------------------------------------------*/
void PWM1_stop();

void pwm1a_start();
void pwm1b_start();
void pwm1_init(uint16_t freq);




#endif /* PWM_API_H_ */