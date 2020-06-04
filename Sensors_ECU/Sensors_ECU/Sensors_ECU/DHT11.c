#include "DHT11.h"

//NOTE:all the statement comments below are extra that are used for checking the work of the module by debugging it oscilliscope

void DHT_init()
{
	DIO_SET_DIR(DHT_PIN, DIO_OUTPUT);
	DIO_SET_VAL(DHT_PIN, DIO_HIGH);

}


uint8_t DHT11_read(char *arr)
{
	DIO_SET_DIR(DHT_PIN, DIO_OUTPUT);
	DIO_SET_VAL(DHT_PIN, DIO_LOW);   //first send start signal(low signal for 18 ms)
	 
	int8_t sw_timer = 6; 
	delay_msec(18);
	
	DIO_SET_DIR(DHT_PIN, DIO_PULLUP);//turn to PU and wait for DHT 11 response
	
	//wait 75 us for response (module outputting low signal) (high signal between 20-40 usec) 
	while ((--sw_timer) && (DIO_READ(DHT_PIN)))
	{
		delay_usec(15);
	}
	
	if (!sw_timer)
	{
		return DHT11_NO_LOW_80US_RESPONSE;
	}
	
	//wait 120 usec for response -DHT outputting high signal- (low signal lasts for 80usec) 
	sw_timer = 7;
	while ((--sw_timer) && !(DIO_READ(DHT_PIN)))
	{
		delay_usec(20);
	}
	
	if (!sw_timer)
	{
		return DHT11_NO_HIGH_80US_RESPONSE;
	}
	
	sw_timer = 8;
	while (DIO_READ(DHT_PIN))
	{
		delay_usec(15);
	}
	
	if (!sw_timer)
	{
		return DHT11_NO_FIRST_DATA;
	}
	
		
	for (unint8_t cnt = 0; cnt < 5; cnt++ )		//5 times for each 8 bit of the 40-bit data
	{	
		arr[cnt] = 0;					//clear the bit from the previous reading
		
		for (int8_t j = 0; j < 8; j++ )			//8 times to save the the bits of each byte of data
		{
			/*if the module has reached this stated then it's assumed that it's working properly and there is no need
			to create a sw timer for any further polling routines*/
			
			//sw timer to wait 70 usec for action - low signal- (repsonse takes 50 usec)
			sw_timer = 35;
			while ((--sw_timer) && !(DIO_READ(DHT_PIN))) //wait until the high wave comes then start testing
			{
				delay_usec(2);
			}
			
			if (!sw_timer)
			{
				return DHT11_DATA_LOW_ERROR;
			}
			

			delay_usec(40); // go th the part of the signal where you check for the signal part 
			
			
			
			if(DIO_READ(DHT_PIN)) //DIO_READ(DHT_PIN)//if the high signal is more than 40us (means high bit) (70us)
			{	
				arr[cnt] |= (1 << (7 - j)); //put 1 to the 
				
				//mostly there will be atmost 30 usec left until this high signal finishes 
				sw_timer = 20;
				while((--sw_timer) && (DIO_READ(DHT_PIN))) //DIO_READ(DHT_PIN)
				{
					delay_usec(2);
				}
				
				if (!sw_timer)
				{
					return DHT11_DATA_HIGH_ERROR; 
				}
				
				//while (DHT_PIN_STATUS & (1 << DHT_PIN)); //waiting for the high signal to finish (in the case of the 70us) 
			}


			
		}// for (j = 0
				
	} //for (cnt = 0;
	
	return SUCCESS;
} //void DHT
