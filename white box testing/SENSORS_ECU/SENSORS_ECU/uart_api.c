/*
 * CFile1.c
 *
 * Created: 5/30/2019 10:08:17 ص
 *  Author: hppp
 */ 
#include "uart_api.h"

#if 0


extern inline bool usart_is_data_ready(unint8_t usart_no);
#endif


void usart_write_str_mark(unint8_t usart_no, unint8_t * str, unint8_t mark)
{
	unint8_t i = 0;
	
	while(str[i] != mark)
	{
		usart_write(usart_no, str[i]);
		i++;
	}
}


void usart_write_str_size(unint8_t usart_no, unint8_t * str, unint8_t size)
{
	unint8_t i = 0;
	
	while(i < size)
	{
		usart_write(usart_no, str[i]);
		i++;
	}
}

void usart_read_str_mark(unint8_t usart_no, unint8_t * str, unint8_t mark)
{
	unint8_t i = 0;
	do
	{
		str[i] = usart_read(usart_no);
	}
	while(str[i++] != mark);
	
	i--;	/*decrement because we incremented i once from the null place after exiting the loop so we must go one step back to it*/
	str[i] = '\0';
}

void usart_read_str_size(unint8_t usart_no, unint8_t * str, unint8_t size)
{
	unint8_t i = 0;
	
	while(i < size)
	{
		str[i] = usart_read(usart_no);
		i++;
	}
	
	str[i] = '\0';
}
