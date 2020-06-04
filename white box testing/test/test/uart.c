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
void UART_sendStrByMark(const char *str , unint8_t mark)
{
	unint8_t i = 0;
	while(str[i] != mark)
	{
		UART_sendChr(str[i]);
		i++;
	}	
}


void UART_sendStrBySize(const char *str,unint16_t size)
{
	unint8_t i = 0;
	while(i < size)
	{
		UART_sendChr(str[i]);
		i++;
	}
}

void UART_receiveStrByMark(char *str,unint8_t mark)
{
	unint8_t i = 0;
	do
	{
		str[i] = UART_receiveChr();
	}
	while(str[i++] != mark);
	
	i--;	/*decrement because we incremented i once from the null place after exiting the loop so we must go one step back to it*/
	str[i] = '\0';
}

void UART_receiveStrBySize(char *str,unint16_t size)
{
	unint8_t i = 0;
	while(i < size)
	{
		str[i] = UART_receiveChr();
		i++;
	}
	str[i] = '\0';
}

void wait_to_finish(unint8_t t)
{
	volatile char tmp; //volatile because it the compiler might see that it's not used anywhere .. but actually reading the 
					   //UDR register is necessary to clear it and clear the RXC flag
	
	while(millis(t))
	{
		if (BIT_IS_SET(UCSRA, RXC))
		{
			tmp = UDR;
			RESET_TICKS;
		}
	}
	
	DISABLE_TIMER0;
	
}

unint8_t is_str_reached(char * str, unint8_t length, unint8_t tmp1, unint8_t * tmp)
{
	//unint8_t tmp1; //stores the char in UDR then checks whether it's in the word----------------------------
	unint8_t b_found = 0; //determines if the chosen word is reached
	
	if (tmp1 == str[*tmp])//if it matches the corresponding char in the str buffer
	{

		*tmp = *tmp + 1; //move to the next position
			
		if (*tmp == length) // if the word is reached .. then set the flag
		{
			b_found = 1;
		}
			
	}
	else //if the character is different then the word in the buffer is wrong
	{
		//check if the char in the tmp in tmp_str position is the same as the first character in str
		//if so then store it in the first position and then make tmp = 1
		if (tmp1 == str[0])
		{
			*tmp = 1;
		}
		else//if not the same as the first char in str position then omit it and start saving again from the beginning
		{
			*tmp = 0;
		}

	}
		
	
	return b_found;
}




//[TO DO]: change the algorithm to be like the one in the get
char check_string(char * str, unint16_t millis_time)
{
	unint8_t length = strlen(str); 
	unint8_t event_flag = 0; //determines if the chosen word is reached
	unint8_t tmp = 0; //used for deciding the next position in the tmp_str for storing the received char from UART
		
	while (millis(millis_time) && (event_flag == 0)) // keep looping until either timeout or word found
	{
		
		if (BIT_IS_SET(UCSRA, RXC)) // if receive flag is activated
		{
			
			if (is_str_reached(str, length, UDR, &tmp)) // check if the received char is the last char in str
			{
				event_flag = 1;
			}
			
		}
		
	}
	
	DISABLE_TIMER0;
	return event_flag;
}

unint8_t get_string(char * dest_str, char * beforestr, char * afterstr, unint16_t delay_time)
{
	char str[30] = {0};
	unint8_t i = 0; // traversing over the buffer
	unint8_t tmp = 0; // storing the received UART char
	unint8_t get_flag = 0;

	unint8_t after_str_length = strlen(afterstr);

	if (check_string(beforestr, delay_time))  
	{

		
		while((millis(delay_time)) && (get_flag == 0))// if the first word is reached // then start getting the world
		{

			if (BIT_IS_SET(UCSRA, RXC)) // if receive flag is activated
			{
				str[i] = UDR; // receive the byte in the tmp_str buffer on the specified address
				if (is_str_reached(afterstr, after_str_length, str[i], &tmp))//check if a received char is the last char in str
				{

					get_flag = 1; //set the flag indicating after_str_reached
					str[i - (after_str_length - 1)] = 0;//remove afterstr from str[30] by ending the string(putting 
														//'\0' at the first address of afterstr stored in str[30]
					strcpy(dest_str, str);
					dest_str[i - (after_str_length - 1)] = 0;
					get_flag = 1;
									
				}
				else
				{
					i++;
				}
						
			}
			
			
			
		}
		DISABLE_TIMER0; //disable timer0 . responsible for millis function
	}
  
  return get_flag;
}