/* ---------------------------------------------------------------------------------------------------
[FILE NAME]: uart.c
[AUTHOR(S)]: Ali Makhlouf
[DATE CREATED]: 15/6/2016
[DESCRIPTION]: library c file for UART serial communication
--------------------------------------------------------------------------------------------------- */


#include "uart.h"
/*this part is interrupt and can be adjusted by the user*/
#if(RECEIVE_COMPLETE_INTERRUP==1)
	volatile unint8_t uart_rxc_flag;
#endif

#if(TRANSMIT_COMPLETE_INTERRUP==1)
	volatile unint8_t uart_txc_flag;
#endif

#if(DATA_REGISTER_EMPTY_INTERRUP==1)
	volatile unint8_t uart_empty_flag;
#endif

#if(RECEIVE_COMPLETE_INTERRUP==1)
ISR(USART_RXC_vect)
{

}
#endif

#if(TRANSMIT_COMPLETE_INTERRUP==1)
ISR(USART_TXC_vect )
{

}
#endif

#if(DATA_REGISTER_EMPTY_INTERRUP==1)
ISR(USART_UDRE_vect)
{

}
#endif

/***************************************************/
void UART_init()
{

	/*calculate UBBR value the formula is (CPU_FREQUENCY / (16_OR_8 * BAUD RATE))  -1 */
	unint16_t UBBR_value = DIVIDE_FIXED_POINT_UART(CPU_FREQ_IN_FIXEDP,
						  	  	  	  	  	  	  MULTIPLY_FIXED_POINT_UART(FIXEDP_FACTOR_8_16,
						  	  	  	  	  	  			  	  	           FIXEDP_BAUD_RATE,
																		   UART_PRECISION_BITS),
											      UART_PRECISION_BITS) - (1<<UART_PRECISION_BITS);
	if((UBBR_value & 1) ) //if the precision bit is 0.1 in binary meaning 0.5 in decimal we should round up
	{
		UBBR_value = (UBBR_value >> 1) +1; //remove the precision bit and round up
	}
	else //if the precision bit is not 0.1 in binary meaning less than 0.5 in decimal we should round down
	{
		UBBR_value >>= UART_PRECISION_BITS; // round down by just removing the precision bit
	}
	
	//PORTB = UBBR_value; just for testing
	//setting the U2X bit to 1 for double speed asynchronous
	UCSRA = (DOUBLE_SPEED<< U2X);	
	/************************** UCSRB Description **************************
	 * RXCIE = 1/0 En/Disable USART RX Complete Interrupt Enable
	 * TXCIE = 1/0 En/Disable USART Tx Complete Interrupt Enable
	 * UDRIE = 1/0 En/Disable USART Data Register Empty Interrupt Enable
	 * RXEN  = 1/0 Receiver En/Disable
	 * TXEN  = 1/0 Transmitter En/DisEnable
	 * UCSZ2 = 0   For 8-bit data mode
	 * RXB8 & TXB8 not used for 8-bit data mode
	 ***********************************************************************/ 
	UCSRB = (TRANSMIT_ENABLE << TXEN)|(RECIEVE_ENABLE<<RXEN) |(TRANSMIT_COMPLETE_INTERRUP<<TXCIE)|(RECEIVE_COMPLETE_INTERRUP<<RXCIE)|(DATA_REGISTER_EMPTY_INTERRUP<<UDRIE);
	
	/************************** UCSRC Description **************************
	 * URSEL   = 1 The URSEL must be one when writing the UCSRC
	 * UMSEL   = 0/1 As/Synchronous Operation
	 * UPM1:0  = 0 Disable parity bit
	 * USBS    = 0 One stop bit
	 * UCSZ1:0 = 11 For 8-bit data mode
	 * UCPOL   = 0 Used with the Synchronous operation only
	 ***********************************************************************/ 
	UCSRC =(1<<URSEL)|(SYNCH_ASYNCH<<UMSEL)|(PARITY<<UPM0)|(STOP_BITS<<USBS)|(DATA_SIZE<<UCSZ0)|(TRANS_SAMPLE_CONFIG<<UCPOL);
 	
	//Put the upper part of the UBBR value here (bits 8 to 11)
	UBRRH = (unint8_t)(UBBR_value >> 8);
	//Put the remaining part of the UBBR value here
	UBRRL = (unint8_t)UBBR_value;
}


void UART_sendChr(const unint8_t byte)
{
	/* UDRE flag is set when the Tx buffer (UDR) is empty and ready for 
	 * transmitting a new byte so wait until this flag is set to one */
	#if(DATA_REGISTER_EMPTY_INTERRUP==0) //if the send interrupt is not activated keep the flag polling
		while (BIT_IS_CLEAR(UCSRA,UDRE)){}
	#endif
	
	/* Put the required data in the UDR register and it also clear the UDRE flag as 
	 * the UDR register is not empty now */	
	UDR=byte;
}

unint8_t UART_receiveChr(void)
{
	/* RXC flag is set when the UART receive data so wait until this 
	 * flag is set to one */
	#if(RECEIVE_COMPLETE_INTERRUP==0) //if the receive interrupt is not activated keep the flag polling
		while(BIT_IS_CLEAR(UCSRA,RXC))
		{

		}
	#endif
	/* Read the received data from the Rx buffer (UDR) and the RXC flag 
	   will be cleared after read this data */	 
    return UDR;		
}

/*notice that this function send the string except the null char */
void UART_sendStrByMark(const unint8_t * str , unint8_t mark)
{
	unint8_t i = 0;
	
	while(str[i] != mark)
	{
		UART_sendChr(str[i]);
		i++;
	}	
	
}


void UART_sendStrBySize(const unint8_t * str,unint16_t size)
{
	unint8_t i = 0;
	while(i < size)
	{
		UART_sendChr(str[i]);
		i++;
	}
}

void UART_receiveStrByMark(unint8_t *str,unint8_t mark)
{
	unint8_t i = -1;
	uint8_t tmp;
	
	str[i] = UART_receiveChr();
	
	do 
	{
		i++;
		str[i] = UART_receiveChr();
	} while (str[i] != mark);
	
		/*decrement because we incremented i once from the null place after exiting the loop so we must go one step back to it*/
	str[i] = '\0';
}

void UART_receiveStrBySize(unint8_t * str,unint16_t size)
{
	unint8_t i = 0;
	
	while(i < size)
	{
		str[i] = UART_receiveChr();
		i++;
	}
	
	str[i] = '\0';
}
