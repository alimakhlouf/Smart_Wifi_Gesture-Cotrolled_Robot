#include "DHT11.h"

//NOTE:all the statement comments below are extra that are used for checking the work of the module by debugging it oscilliscope

void DHT_init()
{
	DIO_SET_DIR(DHT_PIN, DIO_OUTPUT);
	DIO_SET_VAL(DHT_PIN, DIO_HIGH);

}


void DHT11_read(char *arr)
{
	DIO_SET_DIR(DHT_PIN, DIO_OUTPUT);
	DIO_SET_VAL(DHT_PIN, DIO_LOW);
	
// 	DHTDIR |= (1 << DHT_PIN);
// 	DHTPORT &= ~(1 << DHT_PIN);      //first send start signal(low signal for 18 ms)
	delay_msec(18);
	
	DIO_SET_DIR(DHT_PIN, DIO_PULLUP);
// 	DHTPORT |= (1 << DHT_PIN);       //pull up
// 	DHTDIR &= ~(1 << DHT_PIN);	    //turn to input and wait for DHT 11 response
	
	while (DIO_READ(DHT_PIN));
	
	while (!(DIO_READ(DHT_PIN)));
	
	while (DIO_READ(DHT_PIN));
	
	
	//while ((DHT_PIN_STATUS & (1 << DHT_PIN)));// checking for the ending of the high signal( beginning of response )(20-40us)
	
	//while (!(DHT_PIN_STATUS & (1 << DHT_PIN)));//wait until the end of a low signal from DHT(80 us)


	//while (DHT_PIN_STATUS & (1 << DHT_PIN));//wait until the end of a high signal from DHT(80 us)

	
	for (unint8_t cnt = 0; cnt < 5; cnt++ )		//5 times for each 8 bit of the 40-bit data
	{	
		arr[cnt] = 0;					//clear the bit from the previous reading
		
		for (int8_t j = 0; j < 8; j++ )			//8 times to save the the bits of each byte of data
		{
			while (!(DIO_READ(DHT_PIN))); // !(DIO_READ(DIO_OUTPUT))
			
			//while (!(DHT_PIN_STATUS & (1 << DHT_PIN))); //wait for the 50us low signal before the data finishes

			delay_usec(40); // go th the part of the signal where you check for the signal part 
			
			
			
			if(DIO_READ(DHT_PIN)) //DIO_READ(DHT_PIN)//if the high signal is more than 40us (means high bit) (70us)
			{	
				arr[cnt] |= (1 << (7 - j)); //put 1 to the 
				
				while(DIO_READ(DHT_PIN)); //DIO_READ(DHT_PIN)
				
				//while (DHT_PIN_STATUS & (1 << DHT_PIN)); //waiting for the high signal to finish (in the case of the 70us) 
			}


			
		}// for (j = 0
				
	} //for (cnt = 0;
	
} //void DHT
