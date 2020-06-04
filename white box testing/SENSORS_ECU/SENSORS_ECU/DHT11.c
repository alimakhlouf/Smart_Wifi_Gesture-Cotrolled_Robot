/* ----------------------------------------------------------------------------
[FILE NAME]:    DHT11.c

[AUTHOR]:       Ali MAkhlouf

[DATE CREATED]: 1/3/2015 11:28:01 PM

[DESCRIPTION]:   a library C file for communicating and reading sensor
				 values from a DHT11 sensor
-----------------------------------------------------------------------------*/
#include "DHT11.h"

void DHT_init()
{
	//initialize the pin connected to the DJT11 sensor. It's the idle state
	DIO_SET_STATE(DHT_PIN, DIO_OUTPUT);
	DIO_SET_VAL(DHT_PIN, DIO_HIGH);
}


void DHT11_read(char *arr)
{
	//first send start signal(low signal for 18 ms)
	DIO_SET_STATE(DHT_PIN, DIO_OUTPUT);
	DIO_SET_VAL(DHT_PIN, DIO_LOW);
	delay_msec(18);
	//pull up the pin
	DIO_SET_STATE(DHT_PIN, DIO_PULLUP);
	
	/*before data arrives .. a sequence of signals arrives(high - low - high)
	//so we have to skip them in order to move on to the data signals */
	//skipping the first high signal
	while (DIO_READ(DHT_PIN));
	//skipping the low signal
	while (!(DIO_READ(DHT_PIN)));
	//skipping the second high signal
	while (DIO_READ(DHT_PIN));
	// the data consists of 5 bytes 
	
    //looping 5 .. to get the 5 bytes 
	for (unint8_t arr_count = 0; arr_count < 5; arr_count++ )		
	{	
		//clear the bit from the previous reading
		arr[arr_count] = 0;					
		
		//looping 8 times for the bits of each byte
		for (int8_t bit_count = 0; bit_count < 8; bit_count++ )			
		{
			//for every bit the  SIGNAL starts with a low signal .. then a high
			//signal. if the high signal was more than about 30 usec. then the
			//bit is 0. If it's about 70 usec, then the bit is 1
			
			//skip the low signal part
			while (!(DIO_READ(DHT_PIN)));
			
			//skip the first 40 usec of the high signal
			delay_usec(40);  
			
			//then check if it's still high (so bit = 1) or low(so bit = 0) 
			//if it's high
			if(DIO_READ(DHT_PIN))
			{	
				//put 1 to the 
				arr[arr_count] |= (1 << (7 - bit_count)); 
				
				//waiting for the rest of high signal(70us) to finish 
				while(DIO_READ(DHT_PIN)); 
								 
			}
			//otherwise the bit is initially cleared so you don't have to clear it.
			
		}// for (bit_count = 0
				
	} //for (arr_count = 0;
	
} //void DHT
