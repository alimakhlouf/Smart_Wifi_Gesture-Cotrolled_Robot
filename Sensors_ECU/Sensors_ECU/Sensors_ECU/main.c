/*
 * SENSORS_ECU.c
 *
 * Created: 4/8/2019 06:08:48 ص
 * Author : hppp
 */ 

//to do:
//try to remove the id_type from the mcp_remote handler function .. and find out the type by itself from reading the received rx 
//change the mcp_tx_data so that (make a separate one for the remote frame as the remote is not suitable for the function 
//make the mcp_rx_read more efficient so that I don't have to send only 7 bytes and the 8th one is zero
 
#include "micro_config.h" 
#include "dio_api.h"
#include "uart_api.h"
#include "DHT11.h"                                              
#include "MQ_2.h"
#include "mcp2515.h"
#include "adc_api.h"
#include "hcsr04.h"
#include "delay_api.h"

#define MCP_ACTIVATED 1

void alarm();
void store_dht ();

void store_mq2 ();

int main(void)
{
	DIO_SET_DIR(1, DIO_OUTPUT);
	DIO_SET_DIR(2, DIO_OUTPUT);
	DIO_SET_DIR(13, DIO_PULLUP);
	
	uint16_t hcs_cm = 0;
	uint8_t buff[5];
	char DHT11_data[5] = {0};
	char smoke[10]  = {0};
	char t_h[5]  = {0};
	uint16_t mq2_result = 0;
	uint8_t state = 0;
	uint8_t break_flag = 0;
	
	usart_init(USART0, 9600, DATA_SIZE_8, TX_RX, NO_PARITY, ONE_STOP_BIT, TX_RISE);
	
	DHT_init();
	
	MQ2_init();
#if  MCP_ACTIVATED == 1
	mcp_init();
	mcp_tx_id(MCP_TX(0), 0, 0x058f); //000 1111 0111 001 10 00110101 10001111
	//mcp_tx_data(MCP_TX(0), DATA_FRAME, "ALIBE", 5);
	
	mcp_tx_id(MCP_TX(1), 0, 0x057f); //000 1111 0111 001 10 00110101 10001111
	//mcp_tx_data(MCP_TX(1), DATA_FRAME, "ALIST", 5);
	
	mcp_tx_id(MCP_TX(2), 0, 0x056f); //000 1111 0111 001 10 00110101 10001111
	//mcp_tx_data(MCP_TX(2), DATA_FRAME, "ALI MAKH", 8);
#endif
	
 	DIO_TOGGLE(1);
	
	hcsr04_init();
	
	sei();

	usart_write_str_mark(USART0, "Triggering", 0);
	hcsr04_trigger();
    /* Replace with your application code */
	
#if 0
	while(1)
	{
		
		if (DIO_READ(13))
		{
			DIO_SET_VAL(2, DIO_HIGH);
			_delay_ms(500);
		}
		 if (!(DIO_READ(13)))
		 {
			 DIO_SET_VAL(2, DIO_LOW);
		 }
		 
	}
#endif


	
    while (1) 
    {
#if MCP_ACTIVATED == 1

		
		
		state = mcp_status();
		
		if (mcp_status_rx(state, MCP_STAT_NO_RX) == 0)
		{
			uint8_t rx_state = mcp_rx_status();
			
			if (mcp_rx_status_frame(rx_state, RX_STATUS_SREMOTE))
			{
				
				if (mcp_rx_status_filter(rx_state, RX_STATUS_RXF(0)))
				{
					store_dht();
					mcp_tx_trigger(MCP_TX(0));
					
					mcp_clear_flag(RX0IF_BIT);
					
				}
				
				if (mcp_rx_status_filter(rx_state, RX_STATUS_RXF(2)))
				{
					store_mq2();
					mcp_tx_trigger(MCP_TX(1));
					
					mcp_clear_flag(RX1IF_BIT);
					
				}
				
			}
			
		}
		
		if (mcp_status_tx(state, MCP_STAT_TX0))
		{
			mcp_clear_flag(TX0IF_BIT);
		}
		
		if (mcp_status_tx(state, MCP_STAT_TX1))
		{			
			mcp_clear_flag(TX1IF_BIT);
		}
		
		if (mcp_status_tx(state, MCP_STAT_TX2))
		{
			mcp_clear_flag(TX2IF_BIT);
		}
		
		if ( hcsr04_data_ready())
		{
			usart_write_str_mark(USART0, "data_ready", 0);
			hcs_cm = hcsr04_cm();


			if ((hcs_cm < 5) && (break_flag == 0))
			{
				DIO_TOGGLE(1);
				//DIO_TOGGLE(2);
				mcp_send_dataframe(MCP_TX(2), "B1", 2);
				break_flag = 1;
			}
			else if ((hcs_cm >= 5) && (break_flag == 1))
			{
				DIO_TOGGLE(1);
				//DIO_TOGGLE(2);
				mcp_send_dataframe(MCP_TX(2), "B0", 2);
				break_flag = 0;
			}
			
			
			
		}
		
		if (millis(500) == 0)
		{
			//DIO_TOGGLE(1);
			DIO_TOGGLE(2);
			hcsr04_trigger();
		}

#else
		
		char t_h[7]  = {0};
				
		DHT11_read(DHT11_data);
			
		itoa(DHT11_data[0], t_h, 10);
		usart_write_str_mark(USART0, "H2O: ", 0);
		usart_write_str_mark(USART0, t_h, 0);
		usart_write(USART0, 13);
		usart_write(USART0, 10);
			
		itoa(DHT11_data[2], t_h, 10);
		usart_write_str_mark(USART0, "C: ", 0);
		usart_write_str_mark(USART0, t_h, 0);
		usart_write(USART0, 13);
		usart_write(USART0, 10);
				
		mq2_result = MQ2_get_result();
		itoa(mq2_result, t_h, 10);
		usart_write_str_mark(USART0, "ppm: ", 0);
		usart_write_str_mark(USART0, t_h, 0);
		usart_write(USART0, 13);
		usart_write(USART0, 10);
			
		if ( hcsr04_data_ready())
		{
			//usart_write_str_mark(USART0, "data_ready", 0);
			hcs_cm = hcsr04_cm();
			usart_write_str_mark(USART0, "cm: ", 0);
			ltoa(hcs_cm , buff, 10);
			usart_write_str_mark(USART0, buff, 0);
			usart_write_str_mark(USART0, "\r\n", 0);
			hcsr04_trigger();
		}
			
		_delay_ms(500);
		
			
#endif		
	
	}
	
			
	
}
		 


#if MCP_ACTIVATED == 1
void store_dht() // stores dht information in the TX0 data
{
	char DHT11_data[5] = {0};
	char t_h[5]  = {0};
	uint8_t buff[8];
	
	DHT11_read(DHT11_data);
	
	itoa(DHT11_data[0], t_h, 10);
	
	buff[0] = t_h[0];
	buff[1] = t_h[1];

	itoa(DHT11_data[2], t_h, 10);
	buff[2] = t_h[0];
	buff[3] = t_h[1];
	buff[4] = t_h[2];
	buff[5] = 0;
	mcp_tx_data(MCP_TX(0), DATA_FRAME, buff, 5);
	//mcp_send_dataframe(MCP_TX(0), buff, 8);
}

void store_mq2()
{
	uint16_t mq2_result = 0;
	uint8_t buff[8];
	mq2_result = MQ2_get_result();
	itoa(mq2_result, buff, 10);
	//usart_write_str_mark(USART0, buff, 0);
	
	mcp_tx_data(MCP_TX(1), DATA_FRAME, buff, 8);
}

void alarm()
{
	for(int i = 0; i < 150; i++)
	{
		DIO_TOGGLE(2);
		delay_msec(100);
	}
	
}

#endif