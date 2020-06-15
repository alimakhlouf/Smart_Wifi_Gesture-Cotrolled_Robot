/* ---------------------------------------------------------------------------------------------------
[FILE NAME]: uart.c

[AUTHOR(S)]: Ali Makhlouf

[DATE CREATED]: 15/6/2016

[DESCRIPTION]: library c file for UART serial in the HAL layer
--------------------------------------------------------------------------------------------------- */


#include "uart_mcu_config.h"

#if defined(ATMEGA32) || defined(ATMEGA16) || defined(ATMEGA8)

#define CURRENT_SYMBOL_IN_STR(count) (BIT_IS_CLEAR(UCSRB, UCSZ2) ? *((uint8_t *)ap_str + (count)) :\
                                                           *((uint16_t *)ap_str + (count)) ) 
static void ( * gp_uart_txc_isr)(void)  = NULL;
static void ( * gp_uart_rxc_isr)(void)  = NULL;
static void ( * gp_uart_udre_isr)(void) = NULL;

void CONFIG_TXC_ISR() // finished
{
	//-----------------------------------------------------------------------
	// we need to Global interrupt to prevent nested interrupt .
	cli();
	//-----------------------------------------------------------------------
	
	if (gp_uart_txc_isr != NULL)
	{
		gp_uart_txc_isr();
	}

	sei();
}
//=============================================================================
void CONFIG_RXC_ISR()
{
	
	//-------------------------------------------------------------------
	// we need to Global interrupt to prevent nested interrupt .
	cli();
	//-------------------------------------------------------------------
	
	if (gp_uart_rxc_isr != NULL)
	{
		gp_uart_rxc_isr();
	}
	
	sei();

}
//=============================================================================
/*
[NOTE] : using this function might remove the application abstraction feature 
as UDRE is not common among all MCUs
*/
void CONFIG_UDRE_ISR() // finished
{
	//---------------------------------------------------------
	// we need to Global interrupt to prevent nested interrupt .
	cli();
	//---------------------------------------------------------
	//thus we clear it before each transmission that when the transmission
	//completes UDRE = 1, we clear it before each reception
	SET_BIT(UCSRA, UDRE); 
	//------------------------------------------------------------
	// API ISR:
	//------------------------------------------------------------
	
	if (gp_uart_udre_isr != NULL)
	{
		gp_uart_udre_isr();
	}
	

	sei();
}



/***************************************************/
void uart_init (uint8_t a_uart_no, uint32_t a_baud_rate, uint8_t a_data_size,
                      uint8_t a_tx_or_rx, uint8_t a_parity, uint8_t a_stop_bits, 
					         uint8_t a_sample_phase)
{
    if (UART0 == a_uart_no)
	{
		/*calculate UBBR value the formula is 
		(CPU_FREQUENCY / (16_OR_8 * BAUD RATE))-1 
		*/
		unint16_t UBBR_value = FIXP_DIV(FIXP_FREQ, FIXP_MUL(FIXP_8_16, FIXP_BAUD, 
		                                                     UART_PRECISION_BITS),
										UART_PRECISION_BITS) 
							   - (1 << UART_PRECISION_BITS);
											
		/*if the precision bit is 0.1 in binary meaning 0.5 in decimal we 
		  should round up
		*/									
		if ((UBBR_value & 1)) 
		{
			//remove the precision bit and round up
			UBBR_value = (UBBR_value >> 1) + 1;
		}  //else if the precision bit is not 0.1 in binary meaning less than 0.5 
		else //in decimal we should round down
		    
		{
			/*round down by just removing the precision bit*/
			UBBR_value >>= UART_PRECISION_BITS; 
		}                                        
	
		//PORTB = UBBR_value; just for testing
		//setting the U2X bit to 1 for double speed asynchronous
		UCSRA = (DOUBLE_SPEED << U2X);	
		/************************** UCSRB Description **************************
			* RXCIE = 1/0 En/Disable UART RX Complete Interrupt Enable
			* TXCIE = 1/0 En/Disable UART Tx Complete Interrupt Enable
			* UDRIE = 1/0 En/Disable UART Data Register Empty Interrupt Enable
			* RXEN  = 1/0 Receiver En/Disable
			* TXEN  = 1/0 Transmitter En/DisEnable
			* UCSZ2 = 0   For 8-bit data mode
			* RXB8 & TXB8 not used for 8-bit data mode
			*******************************************************************/ 
		UCSRB = (a_tx_or_rx << RXEN_TXEN) | (a_data_size & (1 << 2));
	
		/************************** UCSRC Description **************************
			* URSEL   = 1 The URSEL must be one when writing the UCSRC
			* UMSEL   = 0/1 As/Synchronous Operation
			* UPM1:0  = 0 Disable parity bit
			* USBS    = 0 One stop bit
			* UCSZ1:0 = 11 For 8-bit data mode
			* UCPOL   = 0 Used with the Synchronous operation only
			***********************************************************************/ 
		UCSRC = (1 << URSEL) | (a_parity << UPM_0_1) | (a_stop_bits << USBS) | 
		             ((a_data_size & 0x03) << UCSZ_0_1) | (a_sample_phase << UCPOL);
 	
		//Put the upper part of the UBBR value here (bits 8 to 11)
		UBRRH = (unint8_t)(UBBR_value >> 8);
		//Put the remaining part of the UBBR value here
		UBRRL = (unint8_t)UBBR_value;
	}

}

bool uart_data_ready(unint8_t a_uart_no)
{
	//this will hold the state of the flag whether data's been received or not
	uint8_t result = 0;
	
	if (UART0 == a_uart_no)
	{
		//if b_state was true then set the UDRIE bit otherwise clear it
		result = BIT_IS_SET(UCSRA, RXC);
	}
	
	return result;
}


//NOTE: in interrupt mode you can only use it to send one symbol(e.g.char) at
// a time .. sending a stream won't work
uint8_t uart_send(unint8_t a_uart_no, const void * ap_str, unint16_t a_size)
{
	//used for accessing string elements one by one 
	unint8_t count = 0;
	
	//while the loop counter didn't reach the a_size of the ap_string
	while(count < a_size)
	{
		//make a sw timer delay for half a second 1000 * 500 usec
		int16_t timer = 1000;
		 /* UDRE flag is set when the TX buffer (UDR) is empty and ready for 
		 * transmitting a new byte so wait until this flag is set to one 
		 */
		//if the send interrupt is not activated keep the flag polling
		if (BIT_IS_CLEAR(UCSRB, UDRIE))
		{
			
			while (BIT_IS_CLEAR(UCSRA,UDRE) && --timer)
			{
				delay_usec(500);
			}
			
			if (!timer)
			{
				return ERROR;
			}
			
		}//if in interrupt mode and the size is not set to 1
		else
		{
			return ERROR; //because sending in interrupt mode is completed in 
			              //the isr so we can't send multiple of them 
		}
		
		
		
		//if the ninth bit mode is not used
		if (BIT_IS_CLEAR(UCSRB, UCSZ2))
		{
			/* Put the required data in the UDR register and it automatically
			* clears the UDRE flag as the UDR register is not empty now
			*/
			UDR = *((uint8_t *)ap_str + count);
		}
		else // if the ninth bit is  used
		{//put the ninth bit of the sent frame in its specified bit in UCSRB register
			UCSRB = (( *((uint16_t *)ap_str + count) & (1 << 8)) ? UCSRB | (1 << TXB8) :
																	 UCSRB & ~(1 << TXB8));
																	 
			UDR = ( *((uint16_t *)ap_str + count) & 0xff);
			
		}
	
		
		//UDR = ap_str[count];
		
		count++;
	}
	
}

//this function sends a stream of data except the marked symbol
//NOTE: this function does not work in interrupt mode 
uint8_t uart_send_until(unint8_t a_uart_no, const void * ap_str, unint16_t a_mark)
{
	//used for accessing string elements one by one 
	unint8_t count = 0;
	
	//while we didn't reach end of ap_str character
	//	while( *((uint8_t *)ap_str + count) != a_mark)
	while(CURRENT_SYMBOL_IN_STR(count) != a_mark)
	{
		
		int16_t timer = 1000;
		/* UDRE flag is set when the TX buffer (UDR) is empty and ready for 
		 * transmitting a new byte so wait until this flag is set to one 
		 */
		//if transmit interrupt is disabled activate polling
		//we check the interrupt on the TXC flag .. but poll the UDR flag because
		//the UDR is more efficient than TXC
		while (BIT_IS_CLEAR(UCSRA,UDRE) && --timer)
		{
			delay_usec(500);
		}
		
		if (!timer)
		{
			return ERROR;
		}
		
		//if the ninth bit is not used
		if (BIT_IS_CLEAR(UCSRB, UCSZ2))
		{
			UDR = *((uint8_t *)ap_str + count);			
		}
		else // if the ninth bit is used
		{
			//put the ninth bit value in its specified bit in UCSRB register
			UCSRB = (( *((uint16_t *)ap_str + count) & (1 << 8)) ? UCSRB | (1 << TXB8) :
																		UCSRB & ~(1 << TXB8));
			UDR = ( *((uint16_t *)ap_str + count) & 0xff);
		}
		
		count++;
	}
	
	return SUCCESS;
	
}

uint8_t  uart_send_str(unint8_t a_uart_no, const void * ap_str)
{
	 if (uart_send_until(a_uart_no, ap_str, 0) == ERROR)
	 {
		 return ERROR;
	 }
	 
	 int16_t timer = 1000;
	 while (BIT_IS_CLEAR(UCSRA,UDRE) && --timer)
	 {
		  delay_usec(500);
	 }
	 
	 if (!timer)
	 {
		 return ERROR;
	 }
	 
	 //we are going to send the string nullification (0) so we clear TXB8 and UDR
	 UCSRB &= ~(1 << TXB8);
	 UDR = 0;
	 
	 return SUCCESS; 
}

//NOTE: in interrupt mode you can only use it to get a single received symbol at
// a time (mostly this function is placed in the RX ISR) .. receiving a stream won't work
uint8_t uart_read(unint8_t a_uart_no, void * ap_str, unint16_t a_size)
{
	//used as an index in ap_string array 
	unint8_t count = 0;
	
	
	//while the loop counter didn't reach the a_size of the ap_string
	while(count < a_size)
	{
		//sw delay timer for 50 ms 
		int16_t sw_timer = 100;
		/* RXC flag is set when the UART receive data so wait until this 
	     *flag is set to one */
		//if the receive interrupt is not activated keep the flag polling
		//if interrupt is disabled poll the flag
		if (BIT_IS_CLEAR(UCSRB, RXCIE))
		{
			//SW timer for 1000 sec
			while(BIT_IS_CLEAR(UCSRA,RXC) && --sw_timer)
			{
				delay_usec(500);
			}
			
			if (!sw_timer)
			{
				return ERROR;
			}
		}//else if it's in interrupt mode and the size is not set to one then return an error 
		else if (a_size != 1)
		{
			return ERROR; 
		}
		
		
		//if the ninth bit is not used
		if (BIT_IS_CLEAR(UCSRB, UCSZ2))
		{
			*((uint8_t *)ap_str + count) = UDR;		
		}
		else // if the ninth bit is used
		{
			//clear the receiving buffer from old data ( because we are going
			// to perform OR operation)
			*((uint16_t *)ap_str + count) = 0;
			
			//if the received ninth bit is set .. set it in the buffer
			*((uint16_t *)ap_str + count) |= (BIT_IS_SET(UCSRB, RXB8) << 8);
			
			//put the first 8 bits in the buffer
			*((uint16_t *)ap_str + count) |= UDR ;
		}
		
		/*  RXC flag is automatically cleared after reading this data */	 
				
		count++;
	}
	
	return SUCCESS; 
	
}

//this function keeps receiving a stream of data until it reaches a specific symbol (the marked symbol is received too)
//NOTE this function does not work in interrupt mode 
uint8_t uart_read_until(unint8_t a_uart_no, void * ap_str, unint16_t a_mark)
{
	//used for accessing string elements one by one 
	int16_t count = -1;
	
	//sw delay timer for 50 ms
	int16_t sw_timer = 100;
	
	do
	{
		count++;
		
		while(BIT_IS_CLEAR(UCSRA,RXC) && --sw_timer)
		{
			delay_usec(500);
		}
		
		if (!sw_timer)
		{
			return ERROR;
		}
		
		//if the ninth bit is not used
		if (BIT_IS_CLEAR(UCSRB, UCSZ2))
		{
			*((uint8_t *)ap_str + count) = UDR;
		}
		else // if the ninth bit is used
		{
			//clear the receiving buffer from old data ( because we are going
			// to perform OR operation)
			*((uint16_t *)ap_str + count) = 0;
				
			//if the received ninth bit is set .. set it in the buffer
			*((uint8_t *)ap_str + count) |= (BIT_IS_SET(UCSRB, RXB8) << 8);
				
				
			//put the first 8 bits in the buffer
			*((uint8_t *)ap_str + count) |= UDR ;
		}
		
	//while we didn't reach the mark				
	}while( CURRENT_SYMBOL_IN_STR(count) != a_mark);
	
	return SUCCESS;
}

uint8_t uart_read_str(unint8_t a_uart_no, void * ap_str)
{
	return uart_read_until(a_uart_no, ap_str, 0);
}




void uart_set_rx_int (unint8_t a_uart_no, bool b_state)
{
	
	if (UART0 == a_uart_no)
	{
		//if b_state was true then set the bit otherwise clear it
		UCSRB = ((b_state) ? UCSRB | (1 << RXCIE) : UCSRB & ~(1 << RXCIE));
	}
	
}

void uart_set_tx_int (uint8_t a_uart_no, bool b_state)
{
	
	if (UART0 == a_uart_no)
	{
		//if b_state was true then set the bit otherwise clear it
		UCSRB = (b_state ? UCSRB | (1 << TXCIE) : UCSRB & ~(1 << TXCIE));
	}

}

void uart_set_udr_int (uint8_t a_uart_no, bool ab_state)
{
	
	if (UART0 == a_uart_no)
	{
		//if b_state was true then set the bit otherwise clear it
		UCSRB = (ab_state ? UCSRB | (1 << UDRIE) : UCSRB & ~(1 << UDRIE));
	}

}



void uart_set_rx_isr (unint8_t a_uart_no, void ( * ap_uart_rx_function)(void))
{
	/*
	  assign the function that the user wrote to be the RX ISR to the function
	  that will be actually called when the interrupt occurs.
	*/
	gp_uart_rxc_isr = ap_uart_rx_function;
}

void uart_set_tx_isr (unint8_t a_uart_no, void ( * ap_uart_tx_function)(void))
{
	/*
	  assign the function that the user wrote to be the TX ISR to the function
	  that will be actually called when the interrupt occurs.
	*/
	gp_uart_txc_isr  = ap_uart_tx_function;
}

void uart_set_udr_isr (unint8_t a_uart_no, void ( * ap_uart_udr_function)(void))
{
	/*
	  assign the function that the user wrote to be the TX ISR to the function
	  that will be actually called when the interrupt occurs.
	*/
	gp_uart_udre_isr = ap_uart_udr_function;
}

#endif // defined (ATMEGA32) || ...