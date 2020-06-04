/* ---------------------------------------------------------------------------------------------------
[FILE NAME]: uart.c
[AUTHOR(S)]: Ali Makhlouf
[DATE CREATED]: 15/6/2016
[DESCRIPTION]: library c file for UART serial communication
--------------------------------------------------------------------------------------------------- */


#include "uart_mcu_config.h"




static void ( * USART_TXC_ISR[NUM_USARTS])(void);
static void ( * USART_RXC_ISR[NUM_USARTS])(void);

void CONFIG_TXC_ISR() // finished
{
	//-----------------------------------------------------------------------
	// we need to Global interrupt to prevent nested interrupt .
	cli();
	//-----------------------------------------------------------------------

	USART_TXC_ISR[0]();

	sei();
}
//======================================================================================
void CONFIG_RXC_ISR()
{
	//-------------------------------------------------------------------
	// we need to Global interrupt to prevent nested interrupt .
	cli();
	//-------------------------------------------------------------------
	

	USART_RXC_ISR[0]();

	
	sei();

}
//======================================================================================
#if 0
void CONFIG_UDRE_ISR() // finished
{
	//---------------------------------------------------------
	// we need to Global interrupt to prevent nested interrupt .
	cli();
	//---------------------------------------------------------
	
	SET_BIT(UCSRA, UDRE); // thus we clear it before each transmission that when the transmission completes TXC=1,we clear it before each transmission
	//------------------------------------------------------------
	// API ISR:
	//------------------------------------------------------------

	//USART_UDRIE_ISR();


	sei();
}
#endif

//

extern inline bool usart_data_ready(unint8_t usart_no);


void usart_set_rx_isr (unint8_t usart_no, void ( * p_usart_rx_function)(void))
{
	USART_RXC_ISR[0] = p_usart_rx_function;
}

void usart_set_tx_isr (unint8_t usart_no, void ( * p_usart_tx_function)(void))
{
	USART_TXC_ISR[0] = p_usart_tx_function;
}

/***************************************************/
void usart_init (uint8_t usart_no, uint32_t baud_rate, uint8_t data_size, uint8_t tx_or_rx, uint8_t parity, uint8_t stop_bits, uint8_t sample_phase)
{
    if (USART0 == usart_no)
	{
		/*calculate UBBR value the formula is (CPU_FREQUENCY / (16_OR_8 * BAUD RATE))  -1 */
		unint16_t UBBR_value = FIXP_DIV(FIXP_FREQ, FIXP_MUL(FIXP_8_16, FIXP_BAUD, UART_PRECISION_BITS),
										    UART_PRECISION_BITS) - (1<<UART_PRECISION_BITS);
		if ((UBBR_value & 1)) //if the precision bit is 0.1 in binary meaning 0.5 in decimal we should round up
		{
			UBBR_value = (UBBR_value >> 1) + 1; //remove the precision bit and round up
		}
		else //if the precision bit is not 0.1 in binary meaning less than 0.5 in decimal we should round down
		{
			UBBR_value >>= UART_PRECISION_BITS; // round down by just removing the precision bit
		}
	
		//PORTB = UBBR_value; just for testing
		//setting the U2X bit to 1 for double speed asynchronous
		UCSRA = (DOUBLE_SPEED << U2X);	
		/************************** UCSRB Description **************************
			* RXCIE = 1/0 En/Disable USART RX Complete Interrupt Enable
			* TXCIE = 1/0 En/Disable USART Tx Complete Interrupt Enable
			* UDRIE = 1/0 En/Disable USART Data Register Empty Interrupt Enable
			* RXEN  = 1/0 Receiver En/Disable
			* TXEN  = 1/0 Transmitter En/DisEnable
			* UCSZ2 = 0   For 8-bit data mode
			* RXB8 & TXB8 not used for 8-bit data mode
			***********************************************************************/ 
		UCSRB = (tx_or_rx << RXEN_TXEN) | (data_size & (1 << 2));
	
		/************************** UCSRC Description **************************
			* URSEL   = 1 The URSEL must be one when writing the UCSRC
			* UMSEL   = 0/1 As/Synchronous Operation
			* UPM1:0  = 0 Disable parity bit
			* USBS    = 0 One stop bit
			* UCSZ1:0 = 11 For 8-bit data mode
			* UCPOL   = 0 Used with the Synchronous operation only
			***********************************************************************/ 
		UCSRC = (1 << URSEL) | (parity << UPM_0_1) | (stop_bits << USBS) | ((data_size & 0x03) << UCSZ_0_1) | (sample_phase << UCPOL);
 	
		//Put the upper part of the UBBR value here (bits 8 to 11)
		UBRRH = (unint8_t)(UBBR_value >> 8);
		//Put the remaining part of the UBBR value here
		UBRRL = (unint8_t)UBBR_value;
	}

}


void usart_write(unint8_t usart_no, unint8_t data)
{
	/* UDRE flag is set when the Tx buffer (UDR) is empty and ready for 
	 * transmitting a new byte so wait until this flag is set to one */
	//#if(DATA_REGISTER_EMPTY_INTERRUP==0) //if the send interrupt is not activated keep the flag polling
	if (BIT_IS_CLEAR(UCSRB, UDRIE))
	{
		while (BIT_IS_CLEAR(UCSRA,UDRE))
		{
			
		}
	}

	
	/* Put the required data in the UDR register and it also clear the UDRE flag as 
	 * the UDR register is not empty now */	
	UDR = data;
}

unint8_t usart_read(unint8_t usart_no)
{
	/* RXC flag is set when the UART receive data so wait until this 
	 * flag is set to one */
	//#if(RECEIVE_COMPLETE_INTERRUP==0) //if the receive interrupt is not activated keep the flag polling
	if (BIT_IS_CLEAR(UCSRB, TXCIE))
	{
		while(BIT_IS_CLEAR(UCSRA,RXC))
		{

		}
	}
		
	/* Read the received data from the Rx buffer (UDR) and the RXC flag 
	   will be cleared after read this data */	 
    return UDR;		
}





void usart_set_rx_int (unint8_t usart_no, bool b_state)
{
	
	if (USART0 == usart_no)
	{
		UCSRB = (b_state ? UCSRB | (1 << RXCIE) : UCSRB & ~(1 << RXCIE));
	}
	
}

void usart_set_tx_int (uint8_t usart_no, bool b_state)
{
	
	if (USART0 == usart_no)
	{
		UCSRB = (b_state ? UCSRB | (1 << TXCIE) : UCSRB & ~(1 << TXCIE));
	}

}

void uart_reg_empty_int(uint8_t state)
{
	if (state)
	{
		UCSRB |= (1 << UDRIE);
	}
	else
	{
		UCSRB &= ~(1 << UDRIE);
	}
}
