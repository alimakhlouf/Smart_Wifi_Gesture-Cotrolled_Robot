#include "uart_get_word.h"

void activate_tmr0(char presc)
{
	TCCR0=presc;							 //pre scaler 1024
	//TIMSK|=(1<<TOIE0);
	TCNT0=0;
	//cnt=0;
}

void disable_tmr0()
{
	TCCR0=0;					 //DISABLE TIMER0
	//TIMSK &= ~(1<<TOIE0);		 //DISABLE TMR0 OVERFLOW INTERRUPT
	//cnt=0;
}
//-----------------------------------------------------------------------------
char uart_get_1sec( )
{	char cnt=0;
	activate_tmr0(5);
	
	while ( !(UCSRA & (1<<RXC)))
	{	//PORTC=cnt;
		if(cnt>=30)				//if no uart received during 2 seconds  this means end of string
		{	disable_tmr0();
			//PORTB^=2;
			return 0;
		}
		
		if ((TIFR &(1<<TOV0)))
		{
			TIFR |= (1<<TOV0); // clear the flag
			cnt++;
		}
		
		_delay_us(1);
	}
	
	disable_tmr0();
	
	return 1;
}
//-------------------------------------------------------------------------
void wait_uart_tofinish()
{	
	char v,cnt=0;
	
	activate_tmr0(5);
	
	while (cnt < 30)
	{	
		
		if (UCSRA & (1<<RXC))
		{	cnt=0;
			v=UDR;		//just reading the register in order to empty the receiving buffer and clear rxc flag
		}
		
		if ((TIFR &(1<<TOV0)))
		{
			TIFR |= (1<<TOV0);		// clear the flag
			cnt++;
		}
		
		_delay_us(1);
	}
	
	disable_tmr0();					 //RESET THE COUNTER
	
}


//---------------------------------------------------------------------
void get_whole_uart(char *direction_string)
{	
	unsigned char j=0;
	
	while ( !(UCSRA & (1<<RXC)));
	
	direction_string[j++] = UDR;

	while (1)
	{
		if (uart_get_1sec( ))   direction_string[j++] = UDR;
		else  break;
	}
	
}

//------------------------------------------------------------------------------
#if 0

void wait_to_finish(unint8_t t)
{
	volatile char tmp;
	
	while(millis(t))
	{	
		if (BIT_IS_SET(UCSRA, RXC))
		{
			tmp = UDR;
			RESET_TICKS;
		}
	}
	
	////STOP;
	
}

char check_string(char * str, unint8_t length, unint16_t millis_time)
{
	unint8_t event_flag = 0; //determines if the chosen word is reached
	unint8_t tmp = 0; //used for deciding the next position in the tmp_str for storing the received char from UART
	char tmp_str[length]; //buffer used for checking whether the current word in UART string matches the chosen word
	
	while (millis(millis_time) && (event_flag == 0)) // keep looping until either timeout or word found 
	{
		
		if (BIT_IS_SET(UCSRA, RXC)) // if receive flag is activated
		{
			tmp_str[tmp] = UDR; // receive the byte in the tmp_str buffer on the specified address
			//UART_sendChr(tmp_str[0]);
			//UART_sendChr(tmp_str[1]);	
			
			if (tmp_str[tmp] == str[tmp])//if it matches the corresponding char in the str buffer 
			{
				UART_sendChr('1');
				tmp++; //move to the next position
				
				if (tmp == length) // if the word is reached .. then set the flag
				{
					UART_sendChr('0');
					event_flag = 1;
				}
					
			}
			else //if the character is different then the word in the buffer is wrong
			{
				//UART_sendChr('3');
				//check if the char in the tmp in tmp_str position is the same as the first character in str
				//if so then store it in the first position and then make tmp = 1
				if (tmp_str[tmp] == str[0])
				{
					tmp_str[0] = tmp_str[tmp];
					tmp = 1;
				}
				else//if not the same as the first char in str position then omit it and start saving again from the beginning
				{
					tmp = 0;
				}

			}
			
		}
		
	}
	
	//STOP;
	
	return event_flag;
}
#endif
char get_check_word (char *direction_string ,char get1_check0, char *word1, char *word2)
{
	unsigned char j, first_size, last_size, l_chr = 0;
	
	for (first_size = 0 ; word1[first_size] != 0 ; first_size++ );
	for (last_size = 0 ; word2[last_size] != 0; last_size++ );
	char last_array[last_size];
	
	
	while (j < first_size)		// while the first word isn't found yet
	{
		if ((UCSRA & (1<<RXC)))				//check for uart message
		{	direction_string[0]=UDR;		// get the first char
			
			for (j = 1 ; j < first_size ; j++)				//because the last part is '\0'
			{
				if (uart_get_1sec( ))   direction_string[j] = UDR;
				//else    {uart_string("first word not found \r\n"); return 0;}
			}
			j=0;
			while (j < first_size)		// while the first word isn't found yet
			{
				for ( j=0; (j<first_size) && (direction_string[j]==word1[j]) ; j++ ) ;

				if (j<first_size)		//if the first word is not reached yet
				{
					for (j=0; j<first_size-1 ; j++)		// roll the array (distination_string) one step back
					{
						direction_string[j]=direction_string[j+1];
					}
					PORTC |=(1<<2);
					if (uart_get_1sec( ))   direction_string[j] = UDR;
					//else    {uart_string("first word not found \r\n"); return 0;}
					
					
				}// if (j<first_size)
			}// end of the  while (j<(first_size))
			
			if (!get1_check0)
			return 1;
			else
			{
				
				//-------------------- ------------------------------------------------------------
				// the last word part
			
			
			
				// start saving from the beginning of the destination_string
				for (j=0; j<last_size ; j++)				//because the last part is '\0'
				{
					if (uart_get_1sec( ))   last_array[j] = UDR;
					//else   {uart_string("last word not found \r\n"); return 0;}
				}
			
				j=0;
				
				while (j<last_size)
				{
					for ( j=0; (j<last_size) && (last_array[j]==word2[j]) ; j++);
				
					if (j<last_size)
					{
						direction_string[l_chr]=last_array[0];
					
						for (j=0; j<last_size-1 ; j++)
						{
							last_array[j]=last_array[j+1];
						}
					
						if (uart_get_1sec( ))   last_array[j] = UDR;		// add a char
						//else  {uart_string("last word not found \r\n"); return 0;}
						l_chr++;
					}
				
				}
				direction_string[l_chr]=0;		//add the null
				return 1;
			}
			
		}// end of if statement
		
	}//end of the first while 1
	
	
	return 1;  //( not used)
}// end of function