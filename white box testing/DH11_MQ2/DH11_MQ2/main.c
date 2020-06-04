/*
 * sensors_main.c
 *
 *  Created on: ?? جمادى الآخرة، ???? هـ
 *      Author: Ali Makhlouf
 */

#include "micro_config.h"
#include "uart.h"
#include "std_types.h"
#include "MQ_2.h"
#include <stdlib.h>
#include <math.h>

int main()
{
	
	char DHT11_data[5] = {0};
	char arr[10];
		
	_delay_ms(1000);
	UART_init();
	
	UART_sendStrByMark("hello ali \r\n", 0);	
	
	MQ2_init();
	DHT_init();

	unint16_t ppm;
	DDRB |= 2;
	PORTB |= 2;
	PORTB |= 1;
	
	UART_sendStrByMark("hello ali \r\n", 0);

	while (1)
	{
		
		ppm = MQ2_get_result();
		itoa(ppm, arr, 10);
		UART_sendStrByMark("ppm: ", 0);
		UART_sendStrByMark(arr, 0);
		UART_sendChr(13);
		UART_sendChr(10);
		
		DHT11_read(DHT11_data);
		
		itoa(DHT11_data[0], arr, 10);
		UART_sendStrByMark("H2O: ", 0);
		UART_sendStrByMark(arr, 0);
		UART_sendChr(13);
		UART_sendChr(10);
		
		itoa(DHT11_data[2], arr, 10);
		UART_sendStrByMark("C: ", 0);
		UART_sendStrByMark(arr, 0);
		UART_sendChr(13);
		UART_sendChr(10);
				
		_delay_ms(2000);
	}

	return 0;
}