/* ----------------------------------------------------------------------------
[FILE NAME]: pwm1.h

[AUTHOR]: Ali Makhlouf

[DATE CREATED]: 10/5/2017

[DESCRIPTION]: a library header file for initiating and operating PWM peripherals
-------------------------------------------------------------------------------*/

#ifndef PWM1_H_
#define PWM1_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"
#include "dio_api.h"
#include "fixed_point.h"

#define FALSE 0
#define TRUE 1


#if defined(ATMEGA32) || defined(ATMEGA16) || defined(ATMEGA8)

#define PWM0 1
#define PWM1 2
#define PWM2 4
#define PWM3 8

#if defined(ATMEGA32) || defined(ATMEGA16)
#define DIO_OC0  3
#define DIO_OC1A 13
#define DIO_OC1B 12
#define DIO_OC2  15

#elif defined(ATMEGA8)
#define DIO_OC1A 12
#define DIO_OC1B 13
#define DIO_OC2  14
#endif

//the formula of getting the number of ticks required for performing the
//required delay for a 1 prescaler 
#define NUM_REQUIRED_TICKS_NO_PRE (F_CPU / ((uint32_t)1 * freq))


//the formula of getting the number of ticks required for performing the
//required delay for a 8 prescaler
#define NUM_REQUIRED_TICKS_8_PRE  (F_CPU / ((uint32_t)8 * freq)) 
//the formula of getting the number of ticks required for performing the
//required delay for a 32 prescaler
#define NUM_REQUIRED_TICKS_32_PRE (F_CPU / ((uint32_t)32 * freq))
//the formula of getting the number of ticks required for performing the
//required delay for a 64 prescaler
#define NUM_REQUIRED_TICKS_64_PRE (F_CPU / ((uint32_t)64 * freq)) 
//the formula of getting the number of ticks required for performing the
//required delay for a 128 prescaler
#define NUM_REQUIRED_TICKS_128_PRE (F_CPU / ((uint32_t)128 * freq))
//the formula of getting the number of ticks required for performing the
//required delay for a 256 prescaler
#define NUM_REQUIRED_TICKS_256_PRE (F_CPU / ((uint32_t)256 * freq))
//the formula of getting the number of ticks required for performing the
//required delay for a 1024 prescaler
#define NUM_REQUIRED_TICKS_1024_PRE (F_CPU / ((uint32_t)1024 * freq))

#endif // if defined(ATMEGA32) || defined || defined


/* ----------------------------------------------------------------------------
[Function Name] : pwm_init
[Description] : This function is responsible for initializing one or more PWM
peripherals
[Args] :
[in] -uint8_t pwm_no
	this argument indicates which PWM which be used
	it's form: PWMn (n is the number of the PWM peripheral)
[in] -uint16_t freq
	this argument indicates the frequency of the PWM
-------------------------------------------------------------------------------*/
void pwm_init(uint8_t pwm_no, uint16_t freq); 


/* ----------------------------------------------------------------------------
[Function Name] : pwm_start
[Description] : This function is responsible for starting the PWM 
[Args] :
[in] -uint8_t pwm_no
	this argument indicates which PWM which be used
	it's form: PWMn (n is the number of the PWM peripheral)
-----------------------------------------------------------------------------*/
void pwm_start(uint8_t pwm_no);


/* --------------------------------------------------------------------------
[Function Name] : pwm_changeFreq
[Description] : This function is responsible for changing the frequency of 
the PWM
[Args] :
[in] -uint8_t pwm_no
	this argument indicates which PWM which be used
	it's form: PWMn (n is the number of the PWM peripheral)
[in] -uint16_t freq
	this argument indicates the frequency of the PWM
-----------------------------------------------------------------------------*/
//void pwm_changeFreq(uint8_t pwm_no, uint16_t freq);



/* ----------------------------------------------------------------------------
[Function Name] : PWM1A_setDuty
[Description] : This function is responsible for setting the duty cycle of PWM1

[Args] :
[in] -uint8_t pwm_no
	this argument indicates which PWM which be used
	it's form: PWMn (n is the number of the PWM peripheral)
[in] -uint16_t freq
	this argument indicates the duty cycle
-----------------------------------------------------------------------------*/
void pwm_setDuty(uint8_t pwn_no, uint8_t duty_percent);

/* ---------------------------------------------------------------------------------------------------------------
[Function Name] : pwm_stop
[Description] : This function is responsible for stopping PWM by 
[Args] :
[in] -uint8_t pwm_no
	this argument indicates which PWM which be used
	it's form: PWMn (n is the number of the PWM peripheral)
---------------------------------------------------------------------------------------------------------------*/
void pwm_stop(uint8_t pwn_no);



#endif /* PWM1_H_ */
