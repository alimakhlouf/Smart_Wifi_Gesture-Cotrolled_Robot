/*
 * micro_config.h
 *
 * Created: 1/3/2015 11:28:01 PM
 *  Author: Mohamed Tarek
 */ 


#ifndef MICRO_CONFIG_H_
#define MICRO_CONFIG_H_



#define F_CPU 8000000UL //1MHz Clock frequency

#ifndef SREG
#define I 7
#define SREG *((volatile uint8_t *)(0x5F))
#endif

#define ATMEGA32

#define ERROR 0
#define SUCCESS 1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#endif /* MICRO_CONFIG_H_ */
