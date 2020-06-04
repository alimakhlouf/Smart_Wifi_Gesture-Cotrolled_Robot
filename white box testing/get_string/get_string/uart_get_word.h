#ifndef get_word_library
#define get_word_library

//tis library uses timer 0 with the interrupt and uart with polling but it doesn't conntain the initializaions

#define F_CPU 8000000UL



#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"
#include "uart.h"
#include "millis0.h"

//void wait_to_finish(unint8_t t);
//char check_string(char * str, unint8_t length, unint16_t millis_time);
char uart_get_1sec();
void disable_tmr0();
void activate_tmr0(char presc);
void wait_uart_tofinish();
char get_check_word (char *string ,char get1_check0, char *word1, char *word2);// word2 is the last word wihch is not used when checking
void get_whole_uart(char *direction_string);


/*EXAMPLE:
	while (1)
	{
		if (get_check_word(str,0,"is ",NULL))
		{
			uart_string(str);
			uart_string("\r\n");
			wait_uart_tofinish();
			uart_string("ready for data\r\n");
			
		}
		else
		{
			uart_string("noooo\r\n");
			uart_string("ready for data\r\n");
			
		}
	}*/



#endif