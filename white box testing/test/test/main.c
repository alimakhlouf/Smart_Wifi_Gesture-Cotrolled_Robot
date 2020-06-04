/*
 * test.c
 *
 * Created: 2/25/2019 06:23:10 ص
 * Author : hppp
 */ 

#include "micro_config.h"
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include <string.h>

int main(void)
{
	//char aa[10];
	DDRA = 0xFF;
	PORTB |= 1;
    UART_init();
	//char i = 0;
	//char * str1 = "str\r\n";
	//char t;
	char str3[3];
	
	_delay_ms(5000);
	_delay_ms(1500);
	
	/* Replace with your application code */
    while (1) 
    {		
		PORTA ^= 1;
		_delay_ms(1000);
		
		
    }
	
	
}


/*
if (strlen(str1) == 5)
{
	
	for (int i = 0; i < 10; i++)
	{
		PORTB ^= 1;
		_delay_ms(500);
	}
	
}
else if (strlen(str1) == 7)
{
	for (int i = 0; i < 6; i++)
	{
		PORTB ^= 1;
		_delay_ms(2000);
	}
}

UART_sendStrByMark("strfv\r\n", 0);
_delay_ms(5000);
*/
