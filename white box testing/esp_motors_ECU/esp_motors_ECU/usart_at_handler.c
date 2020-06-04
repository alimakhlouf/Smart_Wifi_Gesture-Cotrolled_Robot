/* ----------------------------------------------------------------------------
[FILE NAME]: uart_at_handler.c

[AUTHOR]: Ali Makhlouf

[DATE CREATED]: 6/14/2019 10:58:31 ص

[DESCRIPTION]: a library c file for initiating and operating PWM peripherals
-------------------------------------------------------------------------------*/
#include "usart_at_handler.h"

void wait_to_finish(unint8_t a_t)
{
	/*volatile because it the compiler might see that it's not used anywhere
	  but actually reading the UDR register is necessary to clear it and 
	  clear the RXC flag
	*/
	volatile char tmp; 
	
	//keep looping until and garbaging any coming data until the time passes 
	while(millis(a_t))
	{
		if (BIT_IS_SET(UCSRA, RXC))
		{
			//read the register to clear the flag and empty UDR
			tmp = UDR;
			
			//when receiving a frame reset the timer
			millis_reset_ticks();
		}
	}
	
	
	millis_disable_tmr();
	
	
}


bool is_str_reached(char * ap_str, unint8_t a_tmp1, unint8_t * ap_tmp)
{
	//determines if the specified word is reached
	bool b_found = false; 
	
	//if it matches the corresponding char in the str buffer
	if (a_tmp1 == ap_str[*ap_tmp])
	{
		*ap_tmp = *ap_tmp + 1; //move to the next position
		
		if (ap_str[*ap_tmp] == 0) // if the null is reached(the word is found)
		{
			//set the found flag 
			b_found = true;
		}
		
	}//if the character is different then the word in the buffer is wrong
	else 
	{
		/*check if the char in the tmp in tmp_str position is the same as
		  the first character in str if so then store it in the first 
		  position and then make tmp = 1*/
		if (a_tmp1 == ap_str[0])
		{
			*ap_tmp = 1;
		}//if not the same as the first char in str position then omit it
		else 
		{
			//start saving again from the beginning
			*ap_tmp = 0;
		}

	}
	
	return b_found;
}

//[TO DO]: change the algorithm to be like the one in the get >>>> done


bool get_str_until(char * dest_str, char * afterstr, unint16_t delay_time)
{
	
	bool b_found = false;//indicates whether terminating str's been reached 
	char str[30] = {0};//will contain the received data
	unint8_t count = 0;//index elements in the str
	
	
	unint8_t after_str_length = strlen(afterstr);
	/*
	used for deciding the next position in the tmp_str for storing the 
	received char from UART
	*/
	unint8_t tmp = 0; 
	
	// if the first word is reached .. then start getting the world
	while((millis(delay_time)) && (b_found == false))
	{

		if (BIT_IS_SET(UCSRA, RXC)) //if receive flag is activated
		{
			//receive the byte in the tmp_str buffer on the specified address
			str[count] = UDR; 
			
			//receive all the message till the end
			if (afterstr == NULL)
			{
				count++;
			}//if a specified str identifies the end of receiving 
			else
			{
				//check if a received char is the last char in str
				if (is_str_reached(afterstr, str[count], &tmp))
				{
					//set the flag indicating after_str_reached
					b_found = true; 
					/*remove afterstr from str[30] by ending the string(putting
					  '\0' at the first address of afterstr stored in str[30]
					*/
					str[count - (after_str_length - 1)] = 0;
					
					strcpy(dest_str, str);
					dest_str[count - (after_str_length - 1)] = 0;
				}//if the char is not the last char in str, keep receiving
				else
				{
					count++;
				}
				
			}
			
		}
		
	}
	
	millis_disable_tmr();
	
	/*if there was no ending character then the function kept receiving until 
	  timeout, in this case the function succeeded but b_found is false so we
	  have to true it */
	if (afterstr == NULL)
	{
		strcpy(dest_str, str);
		b_found = true;
	}
	
	return b_found;
	
}

bool get_string(char * ap_dest_str, char * ap_beforestr, char * ap_afterstr, 
                       unint16_t a_delay_time)
{
	/*indicates whether the operation of collecting the data
	  has been found or not*/
	bool b_data_collected = false;
	
	//keep skipping frames until the before str is reached
	if (check_strings(a_delay_time, 1, ap_beforestr))
	{
		//keep storing upcoming data until the after_str is reached 
		if (get_str_until(ap_dest_str, ap_afterstr, a_delay_time))
		{
			//when it's reached set b_found 
			b_data_collected = true;
		}
		
	}

	return b_data_collected;
}



uint8_t check_strings(unint16_t millis_time, int16_t str_count, ...)
{
	//the list of strings to be checked
	char * str_list[str_count];
	//holds counters  of the strings .. needed for is_str_reached function 
	uint8_t str_counters[str_count];
	//needed for the variable number of parameters procedure
	//it's like a token identifying a specific variable parameters list 
	va_list ap;
	//assigning str_count to be the variable parameter counter of ap
	va_start(ap, str_count);
	
	for (uint8_t i = 0; i < str_count; i++ )
	{
		//putting each string parameter in the str_list string by it's order
		str_list[i] = va_arg(ap, char *);
	}
	
	//determines if the chosen word is reached
	uint8_t b_found = 0; 
	/*used for deciding the next position in the tmp_str for storing the 
	  received char from UART*/ 
	unint8_t tmp = 0;       
	
	//keep looping until either timeout or word found
	while ((millis(millis_time)) && (!b_found)) 
	{
		//there is data to receive
		if (BIT_IS_SET(UCSRA, RXC)) 
		{
			tmp = UDR;          
			
			for (uint8_t i = 0; i < str_count; i++ )
			{
				// check if the received char is the last char in str
				if (is_str_reached(str_list[i], tmp, &str_counters[i])) 
				{
					b_found = i + 1;
					
					break;
				}
				
			}
			
		}
		
	}
	
	millis_disable_tmr();
	
	return b_found;
}

