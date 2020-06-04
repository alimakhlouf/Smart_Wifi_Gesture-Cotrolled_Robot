/* ----------------------------------------------------------------------------
[FILE NAME]:    uart_api.c

[AUTHOR]:       Ali MAkhlouf

[DATE CREATED]: 5/30/2019 10:08:17 ص

[DESCRIPTION]:  this file contains functions in the application layer that 
                controls and issue uart operations.
-----------------------------------------------------------------------------*/
#include "uart_api.h"

void usart_write_str_mark(unint8_t a_usart_no, const unint8_t * ap_str, unint8_t a_mark)
{
	//used for accessing string elements one by one 
	unint8_t count = 0;
	
	//while we didn't reach end of ap_str character
	while(ap_str[count] != a_mark)
	{
		usart_write(a_usart_no, ap_str[count]);
		count++;
	}
}

void usart_write_str_size(unint8_t a_usart_no, const unint8_t * ap_str, unint8_t a_size)
{
	//used for accessing string elements one by one 
	unint8_t count = 0;
	
	//while the loop counter didn't reach the a_size of the ap_string
	while(count < a_size)
	{
		usart_write(a_usart_no, ap_str[count]);
		count++;
	}
}

void usart_read_str_mark(unint8_t a_usart_no, unint8_t * ap_str, unint8_t a_mark)
{
	//used for accessing string elements one by one 
	unint8_t count = 0;
	
	do
	{
		ap_str[count] = usart_read(a_usart_no);
	}
	while(ap_str[count++] != a_mark);//while we didn't reach end of ap_str character
	
	count--;	/*decrement because we incremented i once from the null place 
	              after exiting the loop so we must go one step back to it*/
	ap_str[count] = '\0';
}

void usart_read_str_size(unint8_t a_usart_no, unint8_t * ap_str, unint8_t a_size)
{
	//used for accessing ap_string elements one by one 
	unint8_t count = 0;
	
	//while the loop counter didn't reach the a_size of the ap_string
	while(count < a_size)
	{
		ap_str[count] = usart_read(a_usart_no);
		count++;
	}
	
	
	ap_str[count] = '\0';
}
