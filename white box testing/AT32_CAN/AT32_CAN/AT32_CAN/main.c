/*
 * AT32_CAN.c
 *
 * Created: 4/5/2019 07:26:34 ص
 * Author : hppp
 */ 

#include "micro_config.h"
#include "std_types.h"
//#include "spi.h"
#include "spi_api.h"
#include "uart.h"
#include "mcp2515.h"
#include "dio_api.h"



int main(void)
{
	DDRB &= ~(1 << 0);
	DDRB = 0xff;
	PORTB = 1;
// 	spi_init();
	UART_init();
	unint8_t data_buff[9] = "ALIBEAST";
	unint8_t receive_buff[9];
	
	uint8_t bit_rep[16];
	uint8_t s[3] = {5};
	uint8_t d;
	uint16_t m[3] = {0x058f, 0x057f, 0x056f};
	uint8_t i = 0;
	
	_delay_ms(500);
	PORTB ^= 1;                                                                                              


	mcp_init();
	
	mcp_tx_id(MCP_TX(0), 0, 0x058f);//000 1111 0111 001 10 00110101 10001111
	mcp_tx_id(MCP_TX(1), 0, 0x057f);//000 1111 0111 001 10 00110101 10001111
	mcp_tx_id(MCP_TX(2), 0, 0x056f);//000 1111 0111 001 10 00110101 10001111
	_delay_ms(2000);
	mcp_send_remoteframe(MCP_TX(0), 8);
	
	

	while (1)
	{

#if 1   
		if (mcp_check_flag(TX0IF_BIT))
		{
			UART_sendStrByMark("SENT0\r\n", 0);
			mcp_clear_flag(TX0IF_BIT);
		}

		if (mcp_check_flag(TX1IF_BIT))
		{
			UART_sendStrByMark("SENT1\r\n", 0);
			mcp_clear_flag(TX1IF_BIT);
		}

		if (mcp_check_flag(TX2IF_BIT))
		{
			UART_sendStrByMark("SENT2\r\n", 0);
			mcp_clear_flag(TX2IF_BIT);
		}
                                                                                                                                                                                             
		if (mcp_check_flag(RX0IF_BIT))
		{
			UART_sendStrByMark("RECEIVED1 DATA:\r\n", 0);
			PORTB ^= 1;
			mcp_rx_data(MCP_RX(0), receive_buff);
			UART_sendStrBySize(receive_buff, 8);
			UART_sendStrByMark("\r\n", 0);

			mcp_clear_flag(RX0IF_BIT);
			mcp_send_remoteframe(MCP_TX(1), 8);
			i++;
			
			PORTB ^= 1;
		}
		
		if (mcp_check_flag(RX1IF_BIT))
		{
			
			if (i == 1)
			{
				UART_sendStrByMark("RECEIVED2 DATA:\r\n", 0);
			}
			else if (i == 2)
			{
				UART_sendStrByMark("RECEIVED3 DATA:\r\n", 0);
			}
			
			PORTB ^= 1;
			mcp_rx_data(MCP_RX(1), receive_buff);
			UART_sendStrBySize(receive_buff, 8);
			UART_sendStrByMark("\r\n", 0);			
			mcp_clear_flag(RX1IF_BIT);
			
			if (i == 1)
			{
				mcp_send_remoteframe(MCP_TX(2), 8);
				i++;
			}
			else if (i == 2)
			{
				
			}
			
			PORTB ^= 1;
		}
		
		
#else

		mcp_send_remoteframe(MCP_TX(0), 8);
		
		while (mcp_check_flag(TX0IF_BIT) == 0)
		{
			PORTB ^= 2:
			_delay_ms(100);
		}
		
		UART_sendStrByMark("SENT0\r\n", 0);
#endif
		
		
	}
	
	return 0;
}