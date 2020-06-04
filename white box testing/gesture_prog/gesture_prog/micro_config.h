/* ----------------------------------------------------------------------------
[FILE NAME]:    micro_config.h

[AUTHOR]:       Ali MAkhlouf

[DATE CREATED]: 1/3/2015 11:28:01 PM

[DESCRIPTION]:    library header file for entering general MCU configurations
				  and also contains their features
				  
-----------------------------------------------------------------------------*/

#ifndef MICRO_CONFIG_H_
#define MICRO_CONFIG_H_


//this MACRO indicates the frequency of the CPU 
#define F_CPU 8000000UL //1MHz Clock frequency
//SREG is not defined in th avr/io library so i defined it here
#ifndef SREG
//global interrupt enable location in the SREG register
#define I 7
//defining SRG 
#define SREG *((volatile uint8_t *)(0x5F))

#endif

//Type the MCU name you're using
#define ATMEGA8

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>









#endif /* MICRO_CONFIG_H_ */
