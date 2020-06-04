/*
 * AT16_CAN.c
 *
 * Created: 4/5/2019 07:37:04 ص
 * Author : hppp
 */ 

#include "micro_config.h"
#include "std_types.h"
#include "spi_api.h"
#include "delay_api.h"
#include "dio_api.h"
#include "uart.h"
#include "mcp2515.h"



int main(void)
{
	DDRA = 0xff;
	//SPI_init();
	UART_init();
	uint8_t d;
	uint8_t bit_rep[16];
	uint8_t receive_buff[9];
	uint8_t state;

	mcp_init();
	
	mcp_tx_id(MCP_TX(0), 0, 0x058f); //000 1111 0111 001 10 00110101 10001111
	mcp_tx_data(MCP_TX(0), DATA_FRAME, "ALIBEAST", 8);
	
	mcp_tx_id(MCP_TX(1), 0, 0x057f); //000 1111 0111 001 10 00110101 10001111
	mcp_tx_data(MCP_TX(1), DATA_FRAME, "ALISTARK", 8);
	
	mcp_tx_id(MCP_TX(2), 0, 0x056f); //000 1111 0111 001 10 00110101 10001111
	mcp_tx_data(MCP_TX(2), DATA_FRAME, "ALI MAKH", 8);

	

	

	while (1)
	{
		state = mcp_status();
		
		if (mcp_status_rx(state, MCP_STAT_NO_RX) == 0)
		{
				uint8_t rx_status = mcp_rx_status();
				
				if (mcp_rx_status_frame(rx_status, RX_STATUS_SREMOTE))
				{
					
					if (mcp_status_rx(state, MCP_STAT_RX0))
					{
						
						//here it should send ALIBEAST
						mcp_remote_handler(MCP_RX(0), SID);
						
						
						//just an indicator that it has reached this part of code
						for(int i = 0; i < 6; i++)
						{
							PORTA ^= 2;
							_delay_ms(500);
						}
						
						mcp_clear_flag(RX0IF_BIT);
						
					}
					else if (mcp_check_flag(RX1IF_BIT))//mcp_status_rx(state, MCP_STAT_RX1)
					{
						
						//here it should send ALISTARK
						mcp_remote_handler(MCP_RX(1), SID);
						
						for(int i = 0; i < 10; i++)
						{
							PORTA ^= 2;
							_delay_ms(200);
						}
						
						mcp_clear_flag(RX1IF_BIT);
					}

					
				}
				else
				{
	
					for(int i = 0; i < 4; i++)
					{
						PORTA ^= 2;
						_delay_ms(1500);
					}
					
				}	
				
		}
		
		if (mcp_status_tx(state, MCP_STAT_NO_TX) == 0)
		{
			
			UART_sendStrByMark("entered\r\n", 0);
			if (mcp_status_tx(state, MCP_STAT_TX0))
			{
				UART_sendStrByMark("sent0\r\n", 0);
				_delay_ms(1000);
				mcp_clear_flag(TX0IF_BIT);
			}
			
			if (mcp_status_tx(state, MCP_STAT_TX1))
			{
// 				UART_sendStrByMark("sent1\r\n", 0);
// 				_delay_ms(1000);
				mcp_clear_flag(TX1IF_BIT);
			}
			
			if (mcp_status_tx(state, MCP_STAT_TX2))
			{
				UART_sendStrByMark("sent2\r\n", 0);
				_delay_ms(1000);
				mcp_clear_flag(TX2IF_BIT);
			}
			
		}
						
		
	}
}

