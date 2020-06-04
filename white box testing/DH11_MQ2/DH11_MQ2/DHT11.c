#include "DHT11.h"

//NOTE:all the statement comments below are extra that are used for checking the work of the module by debugging it oscilliscope

void DHT_init()
{
	HDTDIR  |= (1<<HDTPIN);
	HDTPORT |= (1<<HDTPIN);
}


void DHT11_read(char *arr)
{
	
	HDTDIR |= (1 << HDTPIN);
	HDTPORT &= ~(1 << HDTPIN);      //first send start signal(low signal for 18 ms)
	_delay_ms(18);
	HDTPORT |= (1 << HDTPIN);       //pull up
	HDTDIR &= ~(1 << HDTPIN);	    //turn to input and wait for HDT 11 response
	
	while ((HDTPIN_STATUS & (1 << HDTPIN)));// checking for the ending of the high signal( beginning of response )(20-40us)
	
	while (!(HDTPIN_STATUS & (1 << HDTPIN)));//wait until the end of a low signal from DHT(80 us)

	while (HDTPIN_STATUS & (1 << HDTPIN));//wait until the end of a high signal from DHT(80 us)
	
	for (unint8_t cnt = 0; cnt < 5; cnt++ )		//5 times for each 8 bit of the 40-bit data
	{	
		arr[cnt] = 0;					//clear the bit from the previous reading
		
		for (int8_t j = 0; j < 8; j++ )			//8 times to save the the bits of each byte of data
		{
			
			while (!(HDTPIN_STATUS & (1 << HDTPIN))) //wait for the 50us low signal before the data finishes
			{
				
			}
			
			_delay_us(40); // go th the part of the signal where you check for the signal part 
			
			if(HDTPIN_STATUS & (1 << HDTPIN)) //if the high signal is more than 40us (means high bit) (70us)
			{	
				arr[cnt] |= (1 << (7 - j)); //put 1 to the 
				
				while (HDTPIN_STATUS & (1 << HDTPIN)) //waiting for the high signal to finish (in the case of the 70us) 
				{
					
				}
				
			}
				
		}		
	
	}
	
}
