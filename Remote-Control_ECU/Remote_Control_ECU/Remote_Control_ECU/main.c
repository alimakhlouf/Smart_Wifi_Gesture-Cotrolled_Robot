/*
 * gesture_prog.c
 *
 * Created: 4/29/2019 05:19:37 م
 * Author : hppp
 */ 


#include "Queue.h"
#include "micro_config.h"
#include "uart_api.h"
#include "millis_api.h"
#include "ser_lcd.h"
#include "button.h"
#include "dio_api.h"
#include "delay_api.h"
#include "usart_at_handler.h"



#define L 1
#define R 2
#define F 3
#define B 4
#define S 0

#define MENU 0
#define DATA 1

#define S_OP 0
#define H_OP 1
#define T_OP 2

#define NOBREAK 'N'
#define BREAK 'B'

//NOTE: should be put in at library
#define AT_OK 1
#define AT_ERROR 2

void alarm();
void esp_init();
void esp_send(uint8_t * str, uint8_t data_size_str);
void lcd_data(uint8_t break_f);	
void lcd_menu();
void uart_print_list();
void store_data();

typedef struct element{
	char * str;	
	char request_type; 
}element_t;

uint8_t t_arr[4] = "1";
uint8_t h_arr[4] = "2";
uint8_t s_arr[6] = "3";

start_t * list;
element_t e;
element_t * e_ptr = &e;

uint8_t arrow_pos = S_OP; // which option the arrow is pointing at at the moment  


volatile int8_t receive_flag = -2;
volatile int8_t string_counter = -1; //-1 indicating nothing to receive yet
volatile uint8_t temp = 0;
volatile uint8_t str_reached = FALSE; 
volatile uint8_t req_type = 0;
volatile uint16_t counter = 0; 

volatile char buffer[10];
volatile uint8_t rx_end_flag = 0; // 0-indicates that it has ended ... 1-indicates that it's starting

void uart_rx_handler()
{
	rx_end_flag = 1;
	// the states are: initialize the flag to -2(not receiving) .. and the string counter to 0
	//1-if it's not receiving anything.. make the flag = -2( this may not be implemented in the ISR part .. but rather in the millis in the TMR_OVF interrupt )
	//2- if receiving but the begin symbol is not reached yet .. make the flag = -1 : and you must
	//not start any operation with the esp until the flag is returned to -2
	//3- if the begin symbol is reached
	//4- if receiving and the flag is 0 then start storing in the buffer in order by the queue (maximum size for each buffer is 10)
	// I'll have to find a way to make the receiver get the size of the buffer equals the number of characters received
	//5- if the string counter exceeded 10 and the end wasn't reached yet put the flag into -1 .. and the the string counter = -1
	//6- if the end is reached, if the flag = 0 then: the flag = 1, store the resulting string in the queue and clear the counter
	// .. otherwise (if the flag isn't 0) . then don't change any thing
	//any time it receives any thin it clears the timer OVF counter
	
	
	temp = UDR;
	
	if (receive_flag == -2) //if it's the first char to receive
	{
		millis(3);// activate the timer at the beginning of the reception
		receive_flag = -1; // make the flag in state 2(indicating currently receiving but didn't find the begin char yet
		
		if (temp == '{') // if it's the begin character
		{
			//make the counter = 0;  and the flag = 0 indicating storing the next data coming
			receive_flag = 0;
			string_counter = 0;
		}
	}
	else if (receive_flag == -1) // if it's receiving but didn't find the char yet
	{
		 
		if (temp == '{') // if it's the begin character
		{
			//make the counter = 0;  and the flag = 0 indicating storing the next data coming
			receive_flag = 0;
			string_counter = 0;
		}
		
	}
	else if (receive_flag == 0) // if it has found the begin character .. then the next char holds the request type
	{
		req_type = temp; // store the next char in the req_type and 
		receive_flag = 1;//update the state to start putting the data in the buffer
	}
	else if (receive_flag == 1)// //if the first char is read(holds the request type)
	{
		
		if (temp == '}') // if the next char is the terminating char
		{
			
			buffer[string_counter] = 0; // put the null at the end of the received chars
			//change the state to -1 indicating that it has ended .. but not sure if the stream of received data has ended or not 
			//it's assigned to -2 in the millis function
			

			receive_flag = -1;
			
			//TODO: allocate an array in the size of string_counter and store the string in it and pass it to the queue
			//get the counter to the initial state
			string_counter = -1;
			//put the str reached flag to 1
			str_reached = TRUE;
		}
		else
		{
			//keep storing the incoming chars into the buffer .. increment the buffer counter
			buffer[string_counter] = temp;
			string_counter++ ;
			
			if(string_counter > 10) // if it exceeded its limits .. then this is a false operation
			{
				string_counter = -1;
				receive_flag = -1; // not -2 because it's still receiving
			}
			
		}
		
	}
		
}


void timer0_ovf_handler()
{	
	if (receive_flag != -2) // timer is working for a received request
	{
		//if it's still receiving then clear the flag .. and after 3 ms check again and if the flag is still 0 then it means that
		//it it's not receiving any more
		if (rx_end_flag == 1)
		{
			rx_end_flag = 0;
			millis_reset_tmr(3);
			counter++ ;
		}
		else if (rx_end_flag == 0) // if it's zero then there hasn't been any UART receive operation for the last 5 millis which means
		{
			//int8_t arr[10];
			//stop the timer
			millis_disable_tmr();
			//DISABLE_TIMER0;
			//put the receive flag to -2 indicating reception process has stopped
			receive_flag = -2; //default value
			string_counter = -1;// default value
			//itoa(counter, arr, 10);                                          
			//usart_write_str_mark(USART0, arr, 0);
			//usart_write_str_mark(USART0, "\r\n", 0);
			counter = 0;
			
			store_data();
		}
		
	}

}

//NOTE any esp response(from a command not from another esp or server) is
// going to be polling so I have to disable the receive interrupt to prevent
// any received interrupt messages except the response to prevent data 
//collision .. for the sending it's OK because any received data will be
// in a queue waiting to be processed not directly . but the problem is
// that I know that after sending there will be response so mostly if data is
// being received while sending a message it will be neglected .. so I have to
// stop sending while data is being received--knowing data is being received or
// not is by checking by the receive flag becoming -1 or higher--(the esp waits
// before sending the \n and check if it's receiving any data then if it's then 
// wait until all data is received ant then disable the receive interrupt and 
//send the \n char and wait for the response to finish and the reactivate the 
//receive interrupt again .. and so on) .. but I'll do that at the end not now



int main(void)
{
	
	DIO_SET_DIR(16, DIO_OUTPUT);

	//screen manipulation pines
	DIO_SET_DIR(0, DIO_PULLUP); // change option
	DIO_SET_DIR(3, DIO_PULLUP); // choose option
	DIO_SET_DIR(5, DIO_PULLUP); // change screen 
		
	//directions pins
	DIO_SET_DIR(12, DIO_PULLUP); //
	DIO_SET_DIR(14, DIO_PULLUP);
	DIO_SET_DIR(15, DIO_PULLUP);
	DIO_SET_DIR(17, DIO_PULLUP);
	DIO_SET_DIR(19, DIO_PULLUP);
	
	uint8_t esp_data;
	uint8_t lcd_state      = MENU;
	uint8_t break_f        = 0;
	uint8_t direction      = 0;
	uint8_t option_changed = 0;
	uint8_t screen_changed = 0;
	
	sei();

	list = create_list();
	
	
	usart_set_rx_isr(USART0, uart_rx_handler);
	
	millis_add_ovf_isr(timer0_ovf_handler);
    
	usart_init(USART0, 9600, DATA_SIZE_8, TX_RX, NO_PARITY, ONE_STOP_BIT, TX_RISE);
	
	//usart_write_str_mark(USART0, "ALI MAK\r\n", 0);
	_delay_ms(1000);
	
	esp_init();
	LCD_init();	
	LCD_gotoxy(0, 0);
	LCD_str("welcome");
	LCD_gotoxy(0, 1);
	LCD_str("ALI");
	
	_delay_ms(1000);
	
	lcd_menu();


	usart_set_rx_int(USART0, true);
	
	/* Replace with your application code */
    while (1) 
    {
		
		if (list->size > 0) // if there is a request in the Queue
		{
			
			pop(list, e_ptr, sizeof(element_t));
			
			
			if (e_ptr->request_type == 'S') //the format is this .. -- R-data if needed 
			{
				
				memcpy(s_arr, e_ptr->str, strlen(e_ptr->str) + 1);
				usart_write(USART0, 'S');
				usart_write_str_mark(USART0, s_arr, 0); //display the smoke value
				usart_write_str_mark(USART0, "\r\n", 0);
			}
			else if (e_ptr->request_type == 'T')
			{
				usart_write(USART0, 'T');
				memcpy(t_arr, e_ptr->str, strlen(e_ptr->str) + 1);
				usart_write_str_mark(USART0, t_arr, 0); //display the temperature value
				usart_write_str_mark(USART0, "\r\n", 0);
			}					   
			else if (e_ptr->request_type == 'H')
			{
				usart_write(USART0, 'H');
				memcpy(h_arr, e_ptr->str, strlen(e_ptr->str) + 1);
				usart_write_str_mark(USART0, h_arr, 0);	//display the humidity value	
				usart_write_str_mark(USART0, "\r\n", 0);
			}
			else if (e_ptr->request_type == 'B')
			{
				if (buffer[0] == 'B')
				{
					PORTC |= (1 << 5); //activate a led
					break_f = 1; // set the break flag
				}
				else if (buffer[0] == 'N')
				{
					PORTC &= ~(1 << 5);
					break_f = 0;
				}
			}
			
			str_reached = 0; 

		}
		
		
		if (button_state(PULL_UP, DIO_PIN0)) // moves between menu options BIT_IS_CLEAR(PIND, 2)
		{
			if (!option_changed)
			{
				if (lcd_state == MENU)
				{
					
					if (arrow_pos == S_OP)
					{
						LCD_gotoxy(0,0);
						LCD_chr(' ');
						LCD_gotoxy(0, 1);
						LCD_chr('>');
						arrow_pos = T_OP;
						
					}
					else if (arrow_pos == T_OP)
					{
						LCD_gotoxy(0, 1);
						LCD_chr(' ');
						LCD_gotoxy(10, 1);
						LCD_chr('>');
						arrow_pos = H_OP;
					}
					else if (arrow_pos == H_OP)
					{
						LCD_gotoxy(10, 1);
						LCD_chr(' ');
						LCD_gotoxy(0, 0);
						LCD_chr('>');
						arrow_pos = S_OP;
					}
					
				}
				
				option_changed = 1;
			}
			
		}
		else
		{
			option_changed = 0;
		}
		
		
		if (button_state(PULL_UP, DIO_PIN3)) // choose the option  .. get the data and then update the string and then change the value BIT_IS_CLEAR(PIND, 3)
		{
			
			if (lcd_state == MENU)
			{
				
				if (arrow_pos == S_OP)
				{
					esp_send(":S", 2);
					s_arr[0]++;
				}
				else if (arrow_pos == T_OP)
				{
					esp_send(":T", 2);
					t_arr[0]++;
				}
				else if (arrow_pos == H_OP)
				{
					esp_send(":H", 2);
					h_arr[0]++;
				}
				
				lcd_state = DATA;
				lcd_data(break_f);	
			}

			
		}
		
		if (button_state(PULL_UP, DIO_PIN5)) // change screen BIT_IS_CLEAR(PIND, 4)
		{
			if (!screen_changed)
			{
				if (lcd_state == MENU)
				{
					lcd_state = DATA;
					lcd_data(break_f);
				}
				else if (lcd_state == DATA)
				{
					lcd_state = MENU;
					lcd_menu();
				}
				
				screen_changed = 1;
			}
			
		}
		else
		{
			screen_changed = 0;
		}
		
// 		if (button_state(PULL_UP, DIO_PIN12))// move right //B1
// 		{ 
// 					
// 			if (direction != R)
// 			{
// 				esp_send("{DR}", 4);
// 				direction  = R;
// 			}
// 					
// 		}
		if (button_state(PULL_UP, DIO_PIN14)) //FORWARD button .. B2
		{
					
			if (direction != F)
			{
				esp_send("{DF}", 4);
				direction  = F;
			}
					
		}
		else if (button_state(PULL_UP, DIO_PIN15)) //back ward button  B3
		{
					
			if (direction != B)
			{
				esp_send("{DB}", 4);
				direction  = B;
			}
					
		}
		else if (button_state(PULL_UP, DIO_PIN17)) ////left button
		{
					
			if (direction != L)
			{
				esp_send("{DL}", 4);
				direction  = L;
			}
					
		}
		else if (button_state(PULL_UP, DIO_PIN19))// stop button
		{
					
			if (direction != S)
			{
				esp_send("{DS}", 4);
				direction  = S;
			}
					
		}
		
    }
	
}


//==================================================================================================================================
//=================================================================================================================================
//==================================================================================================================================
//=================================================================================================================================
//==================================================================================================================================
//=================================================================================================================================
//==================================================================================================================================
//=================================================================================================================================




void store_data()
{
	if (list->size < 4) //maximum number of queued requests = 5
	{
		//the problem of containing a pointer in the element structure is that when you push it to the Queue
		//so when copying the pointer .. it's not going to copy the actual data but rather copy only the pointer itself so
		// whenever the data that is being pointed to(buffer in this case) changes .. the node data in the queue will
		//change automatically despite the fact that we didn't want that .. so the right move is
		//to not but the pointer point to the buffer but instead copy the buffer data to the pointer and then pass the pointer
		//array instead of a pointer because in this time th element structure itself is going to contain the actual
		//data rather than a pointer to this data and when you push it you're ...
		e_ptr->str = malloc(strlen(buffer) + 1); 
		memcpy(e_ptr->str, buffer, strlen(buffer) + 1); 
		e_ptr->request_type = req_type;
		push(list, e_ptr, sizeof(element_t));// push the request in the Queue 
#if 0
		usart_write_str_mark(USART0, "\r\n size = ", 0); 
		usart_write(USART0, list->size + 48); 
		usart_write_str_mark(USART0, "\r\n", 0); 
#endif
	}
	
#if 0
	if (list->size == 4)
	{
		uart_print_list();
	}
#endif
}


void uart_print_list()
{
	element_t entry;
	usart_write_str_mark(USART0, "\r\n", 0);
	
	while(list->size != 0)
	{
		pop(list, &entry, sizeof(element_t));
		usart_write_str_mark(USART0, entry.str, 0);
		usart_write_str_mark(USART0, "   size = ", 0);
		usart_write(USART0, list->size + 48);
		usart_write_str_mark(USART0, "\r\n", 0);
		
		
		//free the string in the element that was previously allocated 
		free(entry.str);
	}
	
}


void no_data_alarm()
{
	
	for (int i = 0; i < 4; i++)
	{
		DIO_SET_VAL(16, DIO_HIGH);
		_delay_ms(100);
		DIO_SET_VAL(16, DIO_LOW);
		_delay_ms(100);
		
	}
	
}

void error_alarm()
{
	
	for (int i = 0; i < 12; i++)
	{
		DIO_SET_VAL(16, DIO_HIGH);
		_delay_ms(10);
		DIO_SET_VAL(16, DIO_LOW);
		_delay_ms(10);
	}
	
}


void lcd_menu()
{
	LCD_commandSend (CLEAR_SCREEN);
	LCD_strByLocation("smoke:", 1,0);
	LCD_strByLocation("temp:", 1,1);
	LCD_strByLocation("H2O:", 11,1);
	
	if (arrow_pos == S_OP)
	{
		LCD_gotoxy(0, 0);
	}
	else if (arrow_pos == T_OP)
	{
		LCD_gotoxy(0, 1);
	}
	else if (arrow_pos == H_OP)
	{
		LCD_gotoxy(10, 1);
	}
	
	LCD_chr('>');
		
}

void lcd_data(uint8_t break_f)
{
	LCD_commandSend (CLEAR_SCREEN);
	LCD_strByLocation("S: ", 0,0);
	LCD_str(s_arr);
	LCD_str("ppm");
	
	LCD_strByLocation("T: ", 1,1);
	LCD_str(t_arr);
	LCD_str("c");
	
	LCD_strByLocation("H: ", 11,1);
	LCD_str(h_arr);
	LCD_str("%");
	
	if (break_f == 1)
	{
		LCD_strByLocation("BREAK", 9, 0);
	}
	else
	{
		LCD_strByLocation("NOBREAK", 9, 0);
	}
	
	
}

void esp_init()
{
	uint8_t at_flag = 0;
	
	usart_write_str_mark(USART0, "AT", 0);
	while (receive_flag != -2); // while there is some request coming from the esp .. do not send your request in order to avoid
	//collision of data between the request and the response
	usart_set_rx_int(USART0, false); // disable the interrupt .. to do not listen for any request and listen only for the response of command
	
	usart_write_str_mark(USART0, "\r\n", 0);// send the request terminator in order of it to be processed
	at_flag = check_strings(100, 2, "OK\r\n", "ERROR\r\n");
	if (!at_flag)
	{
		no_data_alarm();
	}
	else if (at_flag == AT_ERROR)
	{
		error_alarm();
	}

	
	usart_write_str_mark(USART0, "AT+CWMODE=0\r\n", 0);
	at_flag = check_strings(100, 2, "OK\r\n", "ERROR\r\n");
	if (!at_flag)
	{
		no_data_alarm();
	}
	else if (at_flag == AT_ERROR)
	{
		error_alarm();
	}
	
	
	usart_write_str_mark(USART0, "AT+CIPMUX=1\r\n", 0);
	at_flag = check_strings(100, 2, "OK\r\n", "ERROR\r\n");
	if (!at_flag)
	{
		no_data_alarm();
	}
	else if (at_flag == AT_ERROR)
	{
		error_alarm();
	}
	
	
	usart_write_str_mark(USART0, "AT+CIFSR\r\n", 0);
	at_flag = check_strings(100, 2, "OK\r\n", "ERROR\r\n");
	if (!at_flag)
	{
		no_data_alarm();
	}
	else if (at_flag == AT_ERROR)
	{
		error_alarm();
	}
	
	usart_write_str_mark(USART0, "AT+CIPSTART=\"TCP\",\"192.168.1.108\",80\r\n", 0);
	at_flag = check_strings(100, 2, "OK\r\n", "ERROR\r\n");
	if (!at_flag)
	{
		no_data_alarm();
	}
	else if (at_flag == AT_ERROR)
	{
		error_alarm();
	}
	
	usart_set_rx_int(USART0, true);
	
}


void esp_send(uint8_t * str, uint8_t data_size_str)
{
	int8_t arr[4]; 
	uint8_t at_flag = 0;
	
	usart_write_str_mark(USART0, "AT+CIPSEND=", 0);
	itoa(data_size_str, arr, 10);
	usart_write_str_mark(USART0, arr, 0);
	while (receive_flag != -2); // while there is some request coming from the esp .. do not send your request in order to avoid
	//collision of data between the request and the response
	usart_set_rx_int(USART0, false); // disable the interrupt .. to do not listen for any request and listen only for the response of command
	
	usart_write_str_mark(USART0, "\r\n", 0);// send the request terminator in order of it to be processed 
	at_flag = check_strings(500, 2, "OK\r\n", "ERROR\r\n");
	if (!at_flag)
	{
		no_data_alarm();
	}
	else if (at_flag == AT_ERROR)
	{
		error_alarm();
	}
	
	usart_set_rx_int(USART0, true); //after getting the response activate the interrupt again to listen for any coming requests
	
	usart_write_str_size(USART0, str, data_size_str);
	usart_write_str_mark(USART0, "\r\n", 0);
}











void process_yrequest()
{
#if 0
			if (BIT_IS_SET(UCSRA,RXC))
			{
				
				if (check_string("::", 100))
				{
					req_type = usart_read(USART0);
					//then it's an esp request
					if (get_str_until(esp_data, NULL, 100))
					{
						
						if (req_type == 'S')
						{
							
							if (get_str_until(s_arr, NULL, 100))
							{
								
							}
							
						}
						else if (req_type == 'T')
						{
							
							if (get_str_until(t_arr, NULL, 100))
							{
								
							}
							
						}
						else if (req_type == 'H')
						{
							
							if (get_str_until(h_arr, NULL, 100))
							{
								
							}
							
						}
						else if (req_type == 'B')
						{
							
							b_state = usart_read(USART0);
							
						}
						
					}

					
				}
				
				
			}
			
			
			if (BIT_IS_CLEAR(PIND, 2)) // moves between menu options
			{
				if (lcd_state == MENU)
				{
					
					if (arrow_pos == S_OP)
					{
						LCD_gotoxy(0,0);
						LCD_chr(' ');
						LCD_gotoxy(0, 1);
						LCD_chr('>');
						arrow_pos = T_OP;
						
					}
					else if (arrow_pos == T_OP)
					{
						LCD_gotoxy(0, 1);
						LCD_chr(' ');
						LCD_gotoxy(10, 1);
						LCD_chr('>');
						arrow_pos = H_OP;
					}
					else if (arrow_pos == H_OP)
					{
						LCD_gotoxy(10, 1);
						LCD_chr(' ');
						LCD_gotoxy(0, 0);
						LCD_chr('>');
						arrow_pos = S_OP;
					}
					
				}
				
				_delay_ms(30);
				while(BIT_IS_CLEAR(PIND, 2));
				
			}
			else if (BIT_IS_CLEAR(PIND, 3)) // choose the option  .. get the data and then update the string and then change the value
			{
				
				if (lcd_state == MENU)
				{
					
					if (arrow_pos == S_OP)
					{
						esp_send(":S", 2);
						s_arr[0]++;
					}
					else if (arrow_pos == T_OP)
					{
						esp_send(":T", 2);
						t_arr[0]++;
					}
					else if (arrow_pos == H_OP)
					{
						esp_send(":H", 2);
						h_arr[0]++;
					}
					
					lcd_state = DATA;
					lcd_data();
				}

				
			}
			else if (BIT_IS_CLEAR(PIND, 4)) // change screen
			{
				
				if (lcd_state == MENU)
				{
					lcd_state = DATA;
					lcd_data();
				}
				else if (lcd_state == DATA)
				{
					lcd_state = MENU;
					lcd_menu();
				}
				
				_delay_ms(30);
				while(BIT_IS_CLEAR(PIND, 4));
			}
			else if (BIT_IS_CLEAR(PINB, 1))// move right
			{
				
				if (direction != R)
				{
					esp_send("{DR}", 4);
				}
				
			}
			else if (BIT_IS_CLEAR(PINB, 2)) //FORWARD button
			{
				
				if (direction != F)
				{
					esp_send("{DF}", 4);
				}
				
			}
			else if (BIT_IS_CLEAR(PINB, 3)) //back ward button
			{
				
				if (direction != B)
				{
					esp_send("{DB}", 4);
				}
				
			}
			else if (BIT_IS_CLEAR(PINB, 4)) ////left button
			{
				
				if (direction != L)
				{
					esp_send("{DL}", 4);
				}
				
			}
			else if (BIT_IS_CLEAR(PINB, 5))// stop button
			{
				
				if (direction != S)
				{
					esp_send("{DS}", 4);
				}
				
			}
			
		}
#endif
}