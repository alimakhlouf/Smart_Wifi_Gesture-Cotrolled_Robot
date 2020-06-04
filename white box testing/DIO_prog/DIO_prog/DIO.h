/*
 * DIO_API.h

 *
 *  Created on: Mar 17, 2019
 *      Author: MoaazSherif
 */
//==============================================================
/* this file will be included in all other peripherals drivers.*/
//==============================================================
#ifndef DIO_API_H_
#define DIO_API_H_

//===============================================
#include"std_types.h"
#include"common_macros.h"
#include "micro_config.h"
//===============================================
#define DIO_PIN0 0
#define DIO_PIN1 1
#define DIO_PIN2 2
#define DIO_PIN3 3
#define DIO_PIN4 4
#define DIO_PIN5 5
#define DIO_PIN6 6
#define DIO_PIN7 7
#define DIO_PIN8 8
#define DIO_PIN9 9
#define DIO_PIN10 10
#define DIO_PIN11 11
#define DIO_PIN12 12
#define DIO_PIN13 13
#define DIO_PIN14 14
#define DIO_PIN15 15
#define DIO_PIN16 16
#define DIO_PIN17 17
#define DIO_PIN18 18
#define DIO_PIN19 19
#define DIO_PIN20 20
#define DIO_PIN21 21
#define DIO_PIN22 22
#define DIO_PIN23 23
#define DIO_PIN24 24
#define DIO_PIN25 25
#define DIO_PIN26 26
#define DIO_PIN27 27
#define DIO_PIN28 28
#define DIO_PIN29 29
#define DIO_PIN30 30
#define DIO_PIN31 31
//==============================================
#define DIO_OUTPUT  1
#define DIO_INPUT  	0
#define DIO_PULLUP  2
//==============================================
#define DIO_HIGH    1
#define DIO_LOW     0
//==============================================

//atmega32 io .h
#define No_Ports 4
#define No_PinPerPort 8


extern void DIO_SetPinDir(uint8_t Pin_No,uint8_t PinDir);
extern uint8_t DIO_GetPin(uint8_t Pin_No);
extern void DIO_SetPinVal(uint8_t Pin_No,uint8_t PinVal);

#endif /* DIO_API_H_ */
