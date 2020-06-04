/*
 * get_string.c
 *
 * Created: 2/28/2019 10:46:01 ص
 * Author : hppp
 */ 

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"
#include "uart.h"
#include "millis0.h"
#include "uart_get_word.h"
#include "string.h"

int main(void)
{
	UART_init();
	DDRB = 0xFF;
	
	char str[30];
	
    /* Replace with your application code */
    while (1) 
    {
		PORTB ^= 1;
		
		if (get_str_until(str, "123", 2000))
		{
			UART_sendStrByMark(str,0);
			
		}
		else
		{
			UART_sendChr('N');
		}
		
    }
	
}