/* ----------------------------------------------------------------------------
[FILE NAME]:    uart_api.c

[AUTHOR]:       Ali MAkhlouf

[DATE CREATED]: 5/30/2019 10:08:17 ص

[DESCRIPTION]:  this file contains functions in the application layer that 
                controls and issue uart operations.
-----------------------------------------------------------------------------*/
#include "uart_api.h"

inline static void uart_send_chr(unint8_t a_uart_no, const void * ap_str, unint16_t a_mark))
{
	
}

void uart_send_until(unint8_t a_uart_no, const unint8_t * ap_str, unint16_t a_mark)
{
	//used for accessing string elements one by one 
	unint8_t count = 0;
	
	//while we didn't reach end of ap_str character
	while((CAT(BIT_IS_CLEAR(UCSRB, UCSZ2) ? uint8 : uint16, _t))ap_str[count] != a_mark)
	{
		/* UDRE flag is set when the TX buffer (UDR) is empty and ready for 
		 * transmitting a new byte so wait until this flag is set to one 
		 */
		//if transmit interrupt is disabled activate polling
		//we check the interrupt on the TXC flag .. but poll the UDR flag because
		//the UDR is more efficient for polling .. but the interrupt is more efficient for 
		if (BIT_IS_CLEAR(UCSRB, UDRIE))
		{
			
			while (BIT_IS_CLEAR(UCSRA,UDRE))
			{
			
			}
			
		}
		
		//if the ninth bit is used
		if (BIT_IS_CLEAR(UCSRB, UCSZ2))
		{
			//put the ninth bit value in its specified bit in UCSRB register
			UCSRB = (((uint16_t *)ap_str[count] & (1 << 8)) ? UCSRB | (1 << UDRIE) :
			UCSRB & ~(1 << UDRIE));
			UDR = ((uint16_t *)ap_str[count] & 0xff);
		}
		else // if the ninth bit is not used
		{
			UDR = (uint8_t *)ap_str[count];
		}
		
		count++;
	}
}

void uart_send(unint8_t a_uart_no, const void * ap_str, unint16_t a_size)
{
	//used for accessing string elements one by one 
	unint8_t count = 0;
	
	//while the loop counter didn't reach the a_size of the ap_string
	while(count < a_size)
	{
		 /* UDRE flag is set when the TX buffer (UDR) is empty and ready for 
		 * transmitting a new byte so wait until this flag is set to one 
		 */
		//#if(DATA_REGISTER_EMPTY_INTERRUP==0)
		//if the send interrupt is not activated keep the flag polling
		if (BIT_IS_CLEAR(UCSRB, UDRIE))
		{
			
			while (BIT_IS_CLEAR(UCSRA,UDRE))
			{
			
			}
			
		}
		
		//if the ninth bit is used
		if (BIT_IS_CLEAR(UCSRB, UCSZ2))
		{
			//put the ninth bit value in its specified bit in UCSRB register
			UCSRB = (((uint16_t *)ap_str[count] & (1 << 8)) ? UCSRB | (1 << UDRIE) :
															 UCSRB & ~(1 << UDRIE));
			UDR = ((uint16_t *)ap_str[count] & 0xff);
		}
		else // if the ninth bit is not used
		{
			UDR = (uint8_t *)ap_str[count];
		}
	
		/* Put the required data in the UDR register and it automatically 
		 * clears the UDRE flag as the UDR register is not empty now 
		*/	
		//UDR = ap_str[count];
		
		count++;
	}
}

void uart_receive_until(unint8_t a_uart_no, unint8_t * ap_str, unint16_t a_mark)
{
	//used for accessing string elements one by one 
	unint16_t count = 0;
	
	do
	{
		//if interrupt is disabled poll the flag
		if (BIT_IS_CLEAR(UCSRB, RXCIE))
		{
			
			while(BIT_IS_CLEAR(UCSRA,RXC))
			{

			}
			
		}
		
		//if the ninth bit is used
		if (BIT_IS_CLEAR(UCSRB, UCSZ2))
		{
			//clear the flag from previous data 
			(uint16_t)ap_str[count] = 0;
			
			//if the ninth bit is used
			if (BIT_IS_CLEAR(UCSRB, UCSZ2))
			{
				(uint8_t *)ap_str[count] = UDR;
			}
			else // if the ninth bit is not used
			{
				//clear the flag from previous data
				(uint16_t *)ap_str[count] = 0;
				
				//if the received ninth bit is set .. set it in the buffer
				(uint16_t *)ap_str[count] |= (BIT_IS_SET(UCSRB, TXB8) << 8);
				
				
				//put the first 8 bits in the buffer
				(uint16_t *)ap_str[count] |= UDR ;
			}
					
	}//while we didn't reach the mark
	while((CAT(BIT_IS_CLEAR(UCSRB, UCSZ2) ? uint8 : uint16, _t)) != a_mark);
	
	count--;	/*decrement because we incremented i once from the null place 
	              after exiting the loop so we must go one step back to it*/
	ap_str[count] = '\0';
}

void uart_receive(unint8_t a_uart_no, void * ap_str, unint16_t a_size)
{
	//used for accessing ap_string elements one by one 
	unint8_t count = 0;
	
	//while the loop counter didn't reach the a_size of the ap_string
	while(count < a_size)
	{
		
		/* RXC flag is set when the UART receive data so wait until this 
	     *flag is set to one */
		//if the receive interrupt is not activated keep the flag polling
		//if interrupt is disabled poll the flag
		if (BIT_IS_CLEAR(UCSRB, RXCIE))
		{
			
			while(BIT_IS_CLEAR(UCSRA,RXC))
			{

			}
			
		}
		
		
		//if the ninth bit is used
		if (BIT_IS_CLEAR(UCSRB, UCSZ2))
		{
			//clear the flag from previous data
			(uint16_t *)ap_str[count] = 0;
			
			//if the received ninth bit is set .. set it in the buffer
			(uint16_t *)ap_str[count] |= (BIT_IS_SET(UCSRB, TXB8) << 8);
			
			
			//put the first 8 bits in the buffer
			(uint16_t *)ap_str[count] |= UDR ;
			
		}
		else // if the ninth bit is not used
		{
			(uint8_t *)ap_str[count] = UDR;
		}
		
		/* Read the received data from the Rx buffer (UDR) and the RXC flag 
	   will be cleared after read this data */	 
				
		count++;
	}
	
}
