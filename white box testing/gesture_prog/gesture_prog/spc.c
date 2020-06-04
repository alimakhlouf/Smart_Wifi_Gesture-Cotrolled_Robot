
/* --------------------------------------------------------------------------
[FILE NAME]:     74HC595.c

[AUTHOR]:        Ali MAkhlouf

[DATE CREATED]:  6/4/2019 02:16:18 PM

[DESCRIPTION]:   a library C file for controlling sipo register 74HC595
----------------------------------------------------------------------------*/



#include "spc.h"


void spc_init()
{
	// assign the pins connected sipo_shift_register(74HC595) as output
	DIO_SET_DIR(SH_CP_PIN, DIO_OUTPUT);
	DIO_SET_DIR(DS_PIN, DIO_OUTPUT);
	DIO_SET_DIR(ST_CP_PIN, DIO_OUTPUT);
	
	 
	DIO_SET_VAL(SH_CP_PIN, DIO_LOW);
	DIO_SET_VAL(DS_PIN, DIO_LOW);
	DIO_SET_VAL(ST_CP_PIN, DIO_LOW);                                       
	
}


/*the algorithm:at the beginning clear the shift pin then put the corresponding
 *bit to the DS(data pin) then set the shift pin after doing this 8 times
 * then set the store pin to activate store operation then clear it*/
void spc_send(uint8_t data, uint8_t out_pins_count)
{

	for(int8_t bit_count = out_pins_count - 1; bit_count >= 0 ; bit_count-- )
	{
		
		//zero the clock pin as every bit is sent on the rising edge
		DIO_SET_VAL(SH_CP_PIN, DIO_LOW);
		
		if (BIT_IS_SET(data, bit_count))
		{
			DIO_SET_VAL(DS_PIN, DIO_HIGH);
			
		}
		else
		{
			DIO_SET_VAL(DS_PIN, DIO_LOW);
			
		}
		
		//activate the clock pulse to make 74HC595 latch the bit to the reg.
		DIO_SET_VAL(SH_CP_PIN, DIO_HIGH);
		
	}
	
	//store the new byte that was received from the MCU
	DIO_SET_VAL(ST_CP_PIN, DIO_HIGH);
	DIO_SET_VAL(ST_CP_PIN, DIO_LOW); // clear it again

}


