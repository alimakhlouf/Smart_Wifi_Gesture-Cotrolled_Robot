/*
 * uart_at_handler.c
 *
 * Created: 6/14/2019 10:58:31 ص
 *  Author: hppp
 */ 
#include "usart_at_handler.h"

void wait_to_finish(unint8_t t)
{
	volatile char tmp; //volatile because it the compiler might see that it's not used anywhere .. but actually reading the
	//UDR register is necessary to clear it and clear the RXC flag
	
	while(millis(t))
	{
		if (BIT_IS_SET(UCSRA, RXC))
		{
			tmp = UDR;
			millis_reset_ticks();
			//RESET_TICKS;
		}
	}
	
	millis_disable_tmr();
	//DISABLE_TIMER0;
	
}

unint8_t is_str_reached(char * str, unint8_t tmp1, unint8_t * tmp)
{
	//unint8_t tmp1; //stores the char in UDR then checks whether it's in the word----------------------------
	unint8_t b_found = 0; //determines if the chosen word is reached
	
	if (tmp1 == str[*tmp])//if it matches the corresponding char in the str buffer
	{

		*tmp = *tmp + 1; //move to the next position
		
		if (str[*tmp] == 0) // if the word is reached .. then set the flag
		{
			b_found = 1;
		}
		
	}
	else //if the character is different then the word in the buffer is wrong
	{
		//check if the char in the tmp in tmp_str position is the same as the first character in str
		//if so then store it in the first position and then make tmp = 1
		if (tmp1 == str[0])
		{
			*tmp = 1;
		}
		else//if not the same as the first char in str position then omit it and start saving again from the beginning
		{
			*tmp = 0;
		}

	}
	
	
	return b_found;
}

//[TO DO]: change the algorithm to be like the one in the get >>>> done


unint8_t get_str_until(char * dest_str, char * afterstr, unint16_t delay_time)
{
	unint8_t get_flag = 0;
	char str[30] = {0};
	unint8_t i = 0;
	unint8_t after_str_length = strlen(afterstr);
	unint8_t tmp = 0; //used for deciding the next position in the tmp_str for storing the received char from UART
	
	while((millis(delay_time)) && (get_flag == 0))// if the first word is reached // then start getting the world
	{

		if (BIT_IS_SET(UCSRA, RXC)) // if receive flag is activated
		{
			str[i] = UDR; // receive the byte in the tmp_str buffer on the specified address
			
			if (afterstr == NULL)
			{
				i++;
			}
			else
			{
				
				if (is_str_reached(afterstr, str[i], &tmp))//check if a received char is the last char in str
				{
					get_flag = 1; //set the flag indicating after_str_reached
					str[i - (after_str_length - 1)] = 0;//remove afterstr from str[30] by ending the string(putting
					//'\0' at the first address of afterstr stored in str[30]
					strcpy(dest_str, str);
					dest_str[i - (after_str_length - 1)] = 0;
				}
				else
				{
					i++;
				}
				
			}
			
		}
		
	}
	
	millis_disable_tmr();
	//DISABLE_TIMER0; //disable timer0 . responsible for millis function
	
	if (afterstr == NULL)
	{
		strcpy(dest_str, str);
		get_flag = 1;
	}
	
	return get_flag;
	
}

unint8_t get_string(char * dest_str, char * beforestr, char * afterstr, unint16_t delay_time)
{
	unint8_t get_result = FALSE;
	
	if (check_string(beforestr, delay_time))
	{
		
		if (get_str_until(dest_str, afterstr, delay_time))
		{
			get_result = TRUE;
		}
		
	}

	return get_result;
}



uint8_t check_strings(unint16_t millis_time, int16_t str_count, ...)
{
	char * str_list[str_count];
	uint8_t str_flags[str_count];
	va_list ap;
	va_start(ap, str_count);
	
	
	for (int i = 0; i < str_count; i++ )
	{
		str_list[i] = va_arg(ap, char *);
	}
	
	uint8_t event_flag = 0; //determines if the chosen word is reached
	unint8_t tmp = 0; //used for deciding the next position in the tmp_str for storing the received char from UART
	
	while (millis(millis_time) && (!event_flag)) // keep looping until either timeout or word found
	{
		
		if (BIT_IS_SET(UCSRA, RXC)) // if receive flag is activated
		{
			tmp = UDR;
			for (int i = 0; i < str_count; i++)
			{
				if (is_str_reached(str_list[i], tmp, &str_flags[i])) // check if the received char is the last char in str
				{
					event_flag = i + 1;
					break;
				}
			}
			
			
		}
		
	}
	
	millis_disable_tmr();
	//DISABLE_TIMER0;
	return event_flag;
}

