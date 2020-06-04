/*
 * abstract interrupt.c
 *
 * Created: 5/12/2019 11:23:24 ص
 * Author : hppp
 */ 

#include "micro_config.h"
#include "uart.h"
#include "millis0.h"
#include "uart_config.h"

uint8_t tmp;
void receive_isr()
{
	PORTC ^= 1 << 5;
	tmp = UDR;
}

#if 0 
ISR(USART_RXC_vect)
{
	PORTC ^= 1 << 5;
	tmp = UDR;
}
#endif

int main(void)
{
	DDRC |= 1 << 5;
	
	UART_init();
	
	USART_RXC_ISR = receive_isr;

	sei();
	
    /* Replace with your application code */
    while (1) 
    {
		
    }
}