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

/*************error codes***************/
#define SENSOR_ERROR 255

void alarm();
uint8_t store_dht ();

void store_mq2 ();
void send_all_readings();

int main(void)
{
	DIO_SET_DIR(1, DIO_OUTPUT);
	DIO_SET_DIR(2, DIO_OUTPUT);
	DIO_SET_DIR(13, DIO_PULLUP);
	DIO_SET_DIR(30, DIO_OUTPUT);
	
	volatile uint16_t mq2_result = 0;
	volatile uint16_t hcs_cm = 0;
	uint8_t buff[5];

	
	volatile uint8_t state = 0;
	volatile uint8_t break_flag = 0;
	
	uart_init(UART0, 9600, DATA_BITS_8, TX_RX, PARITY_NO, STOP_BITS_1, TX_R_RX_F);
	
	DHT_init();
	
	MQ2_init();
#if  MCP_ACTIVATED == 1
	if (!mcp_init())
	{
		for (int j = 0; j < 20; j++)
		{
			DIO_TOGGLE(1);
			delay_msec(100);
			
		}
	}
	mcp_set_tx_id(MCP_TX(0), 0, 0x058f); //000 1111 0111 001 10 00110101 10001111
	//mcp_tx_data(MCP_TX(0), DATA_FRAME, "ALIBE", 5);
	
	mcp_set_tx_id(MCP_TX(1), 0, 0x057f); //000 1111 0111 001 10 00110101 10001111
	//mcp_tx_data(MCP_TX(1), DATA_FRAME, "ALIST", 5);
	
	mcp_set_tx_id(MCP_TX(2), 0, 0x056f); //000 1111 0111 001 10 00110101 10001111
	//mcp_tx_data(MCP_TX(2), DATA_FRAME, "ALI MAKH", 8);
#endif
	
 	DIO_TOGGLE(1);
	
	hcsr04_init();
	
	sei();

	//uart_send_until(UART0, "Triggering", 0);
	hcsr04_trigger();
    /* Replace with your application code */
	
	
	//just testing the dio library 
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
		state = mcp_get_status();
		
		//if it has received a frame
		if (mcp_get_status_rx(state, MCP_STAT_NO_RX) == 0)
		{
// 			for (int j = 0; j < 20; j++)
// 			{
// 				DIO_TOGGLE(1);
// 				delay_msec(100);
// 				
// 			}
// 			
			volatile uint8_t rx_state = mcp_get_rx_status();
			
			//if the received frame is a remote frame
			if (mcp_get_rx_status_frame(rx_state, RX_STATUS_SREMOTE))
			{
				
// 				for (int j = 0; j < 5; j++)
// 				{
// 					DIO_TOGGLE(1);
// 					delay_msec(500);
// 					
// 				}
				
				//if it's the first filter in the RXB0
				if (mcp_get_rx_status_filters(rx_state, RX_STATUS_RXF(0)))
				{
// 					for (int j = 0; j < 10; j++)
// 					{
// 						DIO_TOGGLE(1);
// 						delay_msec(100);
// 						
// 					}
					
					if (store_dht() == SENSOR_ERROR)
					{
						uart_send_str(UART0, "\r\nw DHT SENSOR error\r\n");
						for (int i = 0; i < 20; i++)
						{
							DIO_TOGGLE(30);
							delay_msec(100);
						}
						while(1) ;
					}
					
// 					for (int j = 0; j < 9; j++)
// 					{
// 						DIO_TOGGLE(1);
// 						delay_msec(300);
// 					}
					
					mcp_tx_trigger(MCP_TX(0));
					mcp_clear_flag(RX0IF_BIT);
					
// 					for (int j = 0; j < 4; j++)
// 					{
// 						DIO_TOGGLE(1);
// 						delay_msec(500);
// 					}
				}
				
				//if it's the first filter in RXB1
				if (mcp_get_rx_status_filters(rx_state, RX_STATUS_RXF(2)))
				{
					store_mq2();
					mcp_tx_trigger(MCP_TX(1));
					mcp_clear_flag(RX1IF_BIT);
				}
				
			}
			
		}
		
		if (mcp_get_status_tx(state, MCP_STAT_TX0))
		{
			mcp_clear_flag(TX0IF_BIT);
		}
		
		if (mcp_get_status_tx(state, MCP_STAT_TX1))
		{			
			mcp_clear_flag(TX1IF_BIT);
		}
		
		if (mcp_get_status_tx(state, MCP_STAT_TX2))
		{
			mcp_clear_flag(TX2IF_BIT);
		}
		
#endif
		
		if (millis(500) == 0)
		{
			
			if ( hcsr04_data_ready())
			{
				//uart_write_str_mark(UART0, "data_ready", 0);
				//DIO_TOGGLE(2);
				hcs_cm = hcsr04_cm();
// 				uart_write_str_mark(UART0, "cm: ", 0);
// 				ltoa(hcs_cm , buff, 10);
// 				uart_write_str_mark(UART0, buff, 0);
// 				uart_write_str_mark(UART0, "\r\n", 0);


				if ((hcs_cm < 5) && (break_flag == 0))
				{
					DIO_TOGGLE(1);
					//DIO_TOGGLE(2);
					
					#if MCP_ACTIVATED == 1
					mcp_send_dataframe(MCP_TX(2), "B1", 3);
					#endif
					
					break_flag = 1;
				}
				else if ((hcs_cm >= 5) && (break_flag == 1))
				{
					DIO_TOGGLE(1);
					//DIO_TOGGLE(2);
					
					#if MCP_ACTIVATED == 1
					mcp_send_dataframe(MCP_TX(2), "B0", 3);
					#endif
					
					break_flag = 0;
				}
				
			}
			//DIO_TOGGLE(1);
			DIO_TOGGLE(2);
// 			uart_write_str_mark(UART0, "cm: ", 0);
			hcsr04_trigger();
		}
		

#if 0
		send_all_readings();	
		_delay_ms(500);
#endif

	}
	
			
	
}


		 


#if MCP_ACTIVATED == 1
uint8_t store_dht() // stores dht information in the TX0 data
{
	char DHT11_data[5] = {0};
	char t_h[5]  = {0};
	uint8_t buff[8];
	
// 	for (int j = 0; j < 4; j++)
// 	{
// 		DIO_TOGGLE(1);
// 		delay_msec(500);
// 	}
	
	if (DHT11_read(DHT11_data) != SUCCESS) 
	{
		return SENSOR_ERROR;
	}
	
// 	for (int j = 0; j < 9; j++)
// 	{
// 		DIO_TOGGLE(1);
// 		delay_msec(300);
// 	}
	
	//the humidity data
	itoa(DHT11_data[0], t_h, 10);
	
	buff[0] = t_h[0];
	buff[1] = t_h[1];
	
	//the temp data
	itoa(DHT11_data[2], t_h, 10);
	buff[2] = t_h[0];
	buff[3] = t_h[1];
	buff[4] = t_h[2];
	buff[5] = 0;
	mcp_set_tx_data(MCP_TX(0), DATA_FRAME, buff, 6);
	
	return SUCCESS;
}

void store_mq2()
{
	uint16_t mq2_result = 0;
	uint8_t buff[8];
	mq2_result = MQ2_get_result();
	itoa(mq2_result, buff, 10);
	//uart_write_str_mark(UART0, buff, 0);
	
	mcp_set_tx_data(MCP_TX(1), DATA_FRAME, buff, 8);
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

void send_all_readings()
{
	uint8_t buff[5];
	char DHT11_data[5] = {0};
	char smoke[10]  = {0};
	char t_h[7]  = {0};
	uint16_t mq2_result = 0;
	uint16_t hcs_cm = 0;
	
	DHT11_read(DHT11_data);
	
	itoa(DHT11_data[0], t_h, 10);
	uart_send_str(UART0, "H2O: ");
	uart_send_str(UART0, t_h);
	uart_send(UART0, "\r", 1);
	uart_send(UART0, "\n", 1);
	
	itoa(DHT11_data[2], t_h, 10);
	uart_send_str(UART0, "C: ");
	uart_send_str(UART0, t_h);
	uart_send(UART0, "\r", 1);
	uart_send(UART0, "\n", 1);
	
	mq2_result = MQ2_get_result();
	itoa(mq2_result, t_h, 10);
	uart_send_str(UART0, "ppm: ");
	uart_send_str(UART0, t_h);
	uart_send(UART0, "\r", 1);
	uart_send(UART0, "\n", 1);
	
	if ( hcsr04_data_ready())
	{
		//uart_write_str_mark(UART0, "data_ready", 0);
		hcs_cm = hcsr04_cm();
		uart_send_str(UART0, "cm: ");
		ltoa(hcs_cm , buff, 10);
		uart_send_str(UART0, buff);
		uart_send_str(UART0, "\r\n");
		hcsr04_trigger();
	}
}