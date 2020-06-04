/*
 * uart_generic.c
 *
 * Created: 8/5/2019 12:58:13 ص
 * Author : hppp
 */ 

#include "micro_config.h"
#include "uart_api.h"

int main(void)
{
	uint8_t data[50] = {'H', 'E', 'L', 'L', 'O'}; 
	uart_init(UART0, 9600, DATA_BITS_8, TX_RX, PARITY_NO, STOP_BITS_1, TX_R_RX_F);
	
	_delay_ms(1000);
	uart_send(UART0, "hell\r\n", 7);
	uart_send_until(UART0, " My name is Ali Makhlouf" , 0);
	
	while (1)
	{
		uart_read_until(UART0, data, '#');
		uart_send_until(UART0, data, '#');
	}
	
	
}