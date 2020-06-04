/* ---------------------------------------------------------------------------------------------------
[FILE NAME]: uart.c
[AUTHOR(S)]: Ali Makhlouf
[DATE CREATED]: 15/6/2016
[DESCRIPTION]: library c file for UART serial communication
--------------------------------------------------------------------------------------------------- */


#include "uart_mcu_config.h"
#include <stdarg.h> 



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

























/*notice that this function send the string except the null char */

void wait_to_finish(unint8_t t)
{
	volatile char tmp; //volatile because it the compiler might see that it's not used anywhere .. but actually reading the 
					   //UDR register is necessary to clear it and clear the RXC flag
	
	while(millis(t))
	{
		if (BIT_IS_SET(UCSRA, RXC))
		{
			tmp = UDR;
			millis_reset_ticks();
			//RESET_TICKS;
		}
	}
	
	millis_disable_tmr();
	//DISABLE_TIMER0;
	
}

unint8_t is_str_reached(char * str, unint8_t tmp1, unint8_t * tmp)
{
	//unint8_t tmp1; //stores the char in UDR then checks whether it's in the word----------------------------
	unint8_t b_found = 0; //determines if the chosen word is reached
	
	if (tmp1 == str[*tmp])//if it matches the corresponding char in the str buffer
	{

		*tmp = *tmp + 1; //move to the next position
			
		if (str[*tmp] == 0) // if the word is reached .. then set the flag
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

//[TO DO]: change the algorithm to be like the one in the get >>>> done


unint8_t get_str_until(char * dest_str, char * afterstr, unint16_t delay_time)
{
	unint8_t get_flag = 0;
	char str[30] = {0};
	unint8_t i = 0;
	unint8_t after_str_length = strlen(afterstr);
	unint8_t tmp = 0; //used for deciding the next position in the tmp_str for storing the received char from UART
	
	while((millis(delay_time)) && (get_flag == 0))// if the first word is reached // then start getting the world
	{

		if (BIT_IS_SET(UCSRA, RXC)) // if receive flag is activated
		{
			str[i] = UDR; // receive the byte in the tmp_str buffer on the specified address
			
			if (afterstr == NULL)
			{
				i++;
			}
			else
			{
				
				if (is_str_reached(afterstr, str[i], &tmp))//check if a received char is the last char in str
				{
					get_flag = 1; //set the flag indicating after_str_reached
					str[i - (after_str_length - 1)] = 0;//remove afterstr from str[30] by ending the string(putting
					//'\0' at the first address of afterstr stored in str[30]
					strcpy(dest_str, str);
					dest_str[i - (after_str_length - 1)] = 0;
				}
				else
				{
					i++;
				}
				
			}
			
		}
		
	}
	
	millis_disable_tmr();
	//DISABLE_TIMER0; //disable timer0 . responsible for millis function
	
	if (afterstr == NULL)
	{
		strcpy(dest_str, str);
		get_flag = 1;
	}
	
	return get_flag;
	
}

unint8_t get_string(char * dest_str, char * beforestr, char * afterstr, unint16_t delay_time)
{
	unint8_t get_result = 0;
	
	if (check_string(beforestr, delay_time))  
	{
		
		if (get_str_until(dest_str, afterstr, delay_time))
		{
			get_result = 1;
		}
		
	}

	return get_result;
}



uint8_t check_strings(unint16_t millis_time, int16_t str_count, ...)
{
	char * str_list[str_count];
	uint8_t str_flags[str_count];
	va_list ap;
	va_start(ap, str_count);
	
	
	for (int i = 0; i < str_count; i++ )
	{
		str_list[i] = va_arg(ap, char *);
	}	
	
	uint8_t event_flag = 0; //determines if the chosen word is reached
	unint8_t tmp = 0; //used for deciding the next position in the tmp_str for storing the received char from UART
	
	while (millis(millis_time) && (!event_flag)) // keep looping until either timeout or word found
	{
		
		if (BIT_IS_SET(UCSRA, RXC)) // if receive flag is activated
		{
			tmp = UDR;
			for (int i = 0; i < str_count; i++)
			{
				if (is_str_reached(str_list[i], tmp, &str_flags[i])) // check if the received char is the last char in str
				{
					event_flag = i + 1;
					break;
				}
			}
			
			
		}
		
	}
	
	millis_disable_tmr();
	//DISABLE_TIMER0;
	return event_flag;
}

