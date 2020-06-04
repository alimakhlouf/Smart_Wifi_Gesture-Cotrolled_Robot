/*

 * DIO_API.c
 *
 *  Created on: ??þ/??þ/????
 *      Author: Moaaz
 */
#include "dio_api.h"
//#include "dio_mcu_config.h"


#if defined(ATMEGA8) || defined(ATMEGA32) || defined(ATMEGA16)


void DIO_SetPinDir(uint8_t Pin_No,uint8_t PinDir)
{
	
	uint8_t PinPort= Pin_No / No_PinPerPort;
	uint8_t PinNo_InPort = MODULO(Pin_No, No_PinPerPort);
	
	if (PinDir != DIO_PULLUP)
	{
		assig_bit(*(DDR_REG_BASE + PORT_OFFSET(PinPort)), PinNo_InPort, PinDir);
	}
	else //if pull up then make it INPUT  and put 1 to the PORT reg
	{
		assig_bit(*(DDR_REG_BASE + PORT_OFFSET(PinPort)), PinNo_InPort, 0); // assign pin as input
		assig_bit(*(PORT_REG_BASE + PORT_OFFSET(PinPort)), PinNo_InPort, 1); // put 1 to port pin to make it pull up
	}
	
}

uint8_t DIO_GetPin(uint8_t Pin_No)
{
	uint8_t PinPort= Pin_No / No_PinPerPort;
	uint8_t PinNo_InPort = MODULO(Pin_No, No_PinPerPort);
	//uint8_t Pin_Reading=0;
	
	//Pin_Reading = get_bit(*(ATMEGA8_PIN_REG_BASE + PORT_OFFSET(PinPort)), PinNo_InPort);
	
	return (get_bit(*(PIN_REG_BASE + PORT_OFFSET(PinPort)), PinNo_InPort));
}


void DIO_SetPinVal(uint8_t Pin_No, uint8_t PinVal)
{
	uint8_t PinPort = Pin_No / No_PinPerPort;
	uint8_t PinNo_InPort = MODULO(Pin_No, No_PinPerPort);
	
	assig_bit(*(PORT_REG_BASE + PORT_OFFSET(PinPort)), PinNo_InPort, PinVal);

}

void DIO_ToggPinVal(uint8_t Pin_No)
{
	uint8_t PinPort = Pin_No / No_PinPerPort;
	uint8_t PinNo_InPort = MODULO(Pin_No, No_PinPerPort);
	
	TOGGLE_BIT(*(PORT_REG_BASE + PORT_OFFSET(PinPort)), PinNo_InPort);

}
#endif
