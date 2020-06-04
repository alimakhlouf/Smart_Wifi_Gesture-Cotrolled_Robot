/*
 * CAN_Program.c
 *
 * Created: 3/14/2019 08:44:01 ص
 * Author : hppp
 */ 

/*
TO DO: 
1- check the lowest delay needed after the transmission of a message in order for the device to response properly it
*/

#include "micro_config.h"
#include "std_types.h"
#include "spi.h"
#include "uart.h"
#include "mcp2515.h"



int main(void)
{
	DDRA = 0xff;
	PORTA = 1; 
	SPI_init();
	UART_init();
	unint8_t data_buff[9] = "tartasha";
	unint8_t receive_buff[9];
	
	uint8_t bin_rep[16];
	uint8_t s[3] = {5};
	uint8_t d;
	


	mcp_init();  
	
	//assign this id to the message on tx0 buffer 
	mcp_tx_id(MCP_TX(0), 0, 0x058f);//000 1111 0111 001 10 00110101 10001111	
	
	//assign this data to the tx0
	mcp_tx_data(MCP_TX(0), data_buff, 8);
	
	//send the data in TX0 buffer 
	mcp_tx_trigger(MCP_TX(0));
	
//the tx0 is going to send to the rx0 
    /* Replace with your application code */
    while (1)
    {
		PORTA ^= 1;	
		PORTA ^= 2;	

		mcp_rx_data(MCP_RX(0), receive_buff);
		UART_sendStrByMark(receive_buff, 0);
		
		UART_sendStrByMark("\r\n", 0);
		UART_sendStrByMark("\r\n", 0);
		
		_delay_ms(2000);
    }
}