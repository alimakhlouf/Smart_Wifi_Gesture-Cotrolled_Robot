/*
 * 74HC595.c
 *
 *  Created on: ?? —Ã»° ???? Â‹
 *      Author: hppp
 */


#include "common_macros.h"
#include "std_types.h"
#include "micro_config.h"
#include "spc.h"

void spc_init()
{
	// assign the pins connected sipo_SPCift_register(74HC595) as output // assign the three pins as output
	SERIAL_DDR |= ((1 << DS) | (1 << SH_CP) | (1 << ST_CP));
                                                            
	//put 0 the pins
	SERIAL_PORT &= ~((1 << DS) | (1 << SH_CP) | (1 << ST_CP));
}


//the sequence is like this at the beginning clear the shift pulse
//then put the corresponding bit to the DS(data pin) then 
//set the shift pulse
//after doing this 8 times .. set the store pulse then clear it 

void spc_send(uint8_t data, uint8_t out_pins_count) // out pins (how many pins are you using from the 8 pins output)
{

	for(int8_t bit_count = out_pins_count - 1; bit_count >= 0 ; bit_count-- )
	{
		CLEAR_BIT(SERIAL_PORT, SH_CP); //zero the clock pulse as every bit is sent on the rising edge

		if (BIT_IS_SET(data, bit_count))
		{
			SET_BIT(SERIAL_PORT, DS);
		}
		else
		{
			CLEAR_BIT(SERIAL_PORT, DS);
		}

		SET_BIT(SERIAL_PORT, SH_CP);//activate the clock pulse to make74HC595 latch the bit from MCU
	}
	SET_BIT(SERIAL_PORT, ST_CP);//store the new byte that was received from the MCU
	CLEAR_BIT(SERIAL_PORT, ST_CP); // clear it again to not make

}



