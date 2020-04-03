/*
 * 74HC595.c
 *
 *  Created on: ?? —Ã»° ???? Â‹
 *      Author: hppp
 */



#include "spc.h"


void spc_init()
{
	// assign the pins connected sipo_SPCift_register(74HC595) as output // assign the three pins as output
	DIO_SET_DIR(SH_CP_PIN, DIO_OUTPUT);
	DIO_SET_DIR(DS_PIN, DIO_OUTPUT);
	DIO_SET_DIR(ST_CP_PIN, DIO_OUTPUT);
	
	//SERIAL_DDR |= ((1 << DS) | (1 << SH_CP) | (1 << ST_CP));
	 
	DIO_SET_VAL(SH_CP_PIN, DIO_LOW);
	DIO_SET_VAL(DS_PIN, DIO_LOW);
	DIO_SET_VAL(ST_CP_PIN, DIO_LOW);                                       
	
	//put 0 the pins
	//SERIAL_PORT &= ~((1 << DS) | (1 << SH_CP) | (1 << ST_CP));
}


//the sequence is like this at the beginning clear the shift pulse
//then put the corresponding bit to the DS(data pin) then 
//set the shift pulse
//after doing this 8 times .. set the store pulse then clear it 

void spc_send(uint8_t data, int8_t out_pins_count) // out pins (how many pins are you using from the 8 pins output)
{

	for(int8_t bit_count = out_pins_count - 1; bit_count >= 0 ; bit_count-- )
	{
		
		DIO_SET_VAL(SH_CP_PIN, DIO_LOW);
		
		if (BIT_IS_SET(data, bit_count))
		{
			DIO_SET_VAL(DS_PIN, DIO_HIGH);
		}
		else
		{
			DIO_SET_VAL(DS_PIN, DIO_LOW);
		}
		
		DIO_SET_VAL(SH_CP_PIN, DIO_HIGH);
	}
	
	DIO_SET_VAL(ST_CP_PIN, DIO_LOW);//store the new byte that was received from the MCU
	DIO_SET_VAL(ST_CP_PIN, DIO_HIGH); // clear it again to not make

}



