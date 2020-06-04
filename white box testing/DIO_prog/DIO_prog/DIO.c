/*

 * DIO_API.c
 *
 *  Created on: ??þ/??þ/????
 *      Author: Moaaz
 */
#include"DIO.h"
//#include"DIO_Cfg.h"

void DIO_voidInitial()
{

}

void DIO_SetPinDir(uint8_t Pin_No,uint8_t PinDir)
{
	if (Pin_No >= 15)
	{
		Pin_No++ ; // because there is no PC7(pin15) .. so we move directly to PORTD
	}
	
	uint8_t PinPort= Pin_No / No_PinPerPort;
	uint8_t PinNo_InPort = MODULO(Pin_No, No_PinPerPort);
	
	switch(PinPort)
	{
		case 0 :
			assig_bit(DDRB,PinNo_InPort,PinDir);
			break;
		case 1 :
			assig_bit(DDRC,PinNo_InPort,PinDir);
			break;
		case 2 :
			assig_bit(DDRD,PinNo_InPort,PinDir);
			break;
	}
}
uint8_t DIO_GetPin(uint8_t Pin_No)
{
	if (Pin_No >= 15)
	{
		Pin_No++ ; // because there is no PC7 .. so we move directly to PORTD 
	}
	
	uint8_t PinPort= Pin_No/No_PinPerPort;
	uint8_t PinNo_InPort = MODULO(Pin_No, No_PinPerPort);
	uint8_t Pin_Reading=0;
	
	switch(PinPort)
	{
		case 0 :
			Pin_Reading=get_bit(PINB,PinNo_InPort);
			break;
		case 1 :
			Pin_Reading=get_bit(PINC,PinNo_InPort);
			break;
		case 2 :
			Pin_Reading=get_bit(PIND,PinNo_InPort);
			break;
	}
	
	return Pin_Reading;
}

void DIO_SetPinVal(uint8_t Pin_No,uint8_t uint8_tPinVal)
{
	if (Pin_No >= 15)
	{
		Pin_No++ ; // because there is no PC7 .. so we move directly to PORTD
	}
	
	uint8_t PinPort = Pin_No / No_PinPerPort;
	uint8_t PinNo_InPort = MODULO(Pin_No, No_PinPerPort);
	
	switch(PinPort)
			{
				case 0 :
					assig_bit(PORTB,PinNo_InPort,uint8_tPinVal);
					break;
				case 1 :
					assig_bit(PORTC,PinNo_InPort,uint8_tPinVal);
					break;
				case 2 :
					assig_bit(PORTD,PinNo_InPort,uint8_tPinVal);
					break;
			}

}
