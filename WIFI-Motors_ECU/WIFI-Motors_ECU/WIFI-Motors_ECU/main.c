/*
 * esp_motors_ECU.c
 *
 * Created: 4/8/2019 09:54:20 ص
 * Author : hppp
 */ 



#include "micro_config.h"
#include "dio_api.h"
#include "mcp2515.h"
#include "pwm1.h"
#include "Queue.h"
#include "uart_api.h"
#include "spi_api.h"
#include "delay_api.h"
#include "millis_api.h"
#include "usart_at_handler.h"

#define R_FORWARD  0
#define R_BACKWARD 1
#define L_FORWARD  2
#define L_BACKWARD 3

#define AT_OK 1
#define AT_ERROR 2

#define MCP 1

typedef struct element{
	volatile char * str;
	volatile char request_type;
}element_t;



volatile start_t * list;
element_t e;
element_t * e_ptr = &e;
char esp_buff[10] = "{        ";


void esp_init ();
uint8_t esp_send_temp (unint8_t ch_num);
uint8_t esp_send_smoke (unint8_t ch_num);
uint8_t esp_send_H2O (unint8_t ch_num);
void alarm ();
void esp_send (const char * str, unint8_t ch_num);
void esp_send_alert (char * alert, uint8_t ch_num);
void sensor_to_esp_buff (char * sensor_str, uint8_t info_type);
void store_data ();
void no_data_alarm ();
void error_alarm ();

void turn_right ()
{
	//PORTB = 0b00000110; // the first two bits for the right motor .. the second two bits for the left motor
	DIO_SET_VAL(L_FORWARD, DIO_HIGH);
	DIO_SET_VAL(L_BACKWARD, DIO_LOW);
	DIO_SET_VAL(R_FORWARD, DIO_LOW);
	DIO_SET_VAL(R_BACKWARD, DIO_HIGH);
}

inline void motors_stop()
{
	DIO_SET_VAL(L_FORWARD, DIO_LOW);
	DIO_SET_VAL(L_BACKWARD, DIO_LOW);
	DIO_SET_VAL(R_FORWARD, DIO_LOW);
	DIO_SET_VAL(R_BACKWARD, DIO_LOW);
}

inline void move_forward()
{
	DIO_SET_VAL(R_FORWARD, DIO_HIGH);
	DIO_SET_VAL(R_BACKWARD, DIO_LOW);
	DIO_SET_VAL(L_FORWARD, DIO_HIGH);
	DIO_SET_VAL(L_BACKWARD, DIO_LOW);
}

void move_backward()
{
	DIO_SET_VAL(R_FORWARD, DIO_LOW);
	DIO_SET_VAL(R_BACKWARD, DIO_HIGH);
	DIO_SET_VAL(L_FORWARD, DIO_LOW);
	DIO_SET_VAL(L_BACKWARD, DIO_HIGH);
}

void turn_left()
{
	DIO_SET_VAL(R_FORWARD, DIO_HIGH);
	DIO_SET_VAL(R_BACKWARD, DIO_LOW);
	DIO_SET_VAL(L_FORWARD, DIO_LOW);
	DIO_SET_VAL(L_BACKWARD, DIO_HIGH);
}

void  motors_init()
{
	DIO_SET_DIR(R_FORWARD, DIO_OUTPUT); //RF
	DIO_SET_DIR(R_BACKWARD, DIO_OUTPUT); //RB
	DIO_SET_DIR(L_FORWARD, DIO_OUTPUT); //LF
	DIO_SET_DIR(L_BACKWARD, DIO_OUTPUT); //LB
	PORTB = 0;
	
	pwm_init(PWM1 | PWM2, 1000);
	pwm_start(PWM1 | PWM2);
	pwm_setDuty(PWM1, 10);
	pwm_setDuty(PWM2, 90);
}

volatile int8_t receive_flag = -2;
volatile int8_t string_counter = -1; //-1 indicating nothing to receive yet
volatile uint8_t temp = 0;
volatile uint8_t str_reached = FALSE; 
volatile uint8_t req_type = 0;
volatile uint16_t counter = 0; 
volatile uint8_t rx_end_flag = 0; // 0:indicates that it has ended ... 1: indicates that it's starting
volatile int8_t buffer[10];

void uart_rx_handler(void)
{
	
	rx_end_flag = 1; // flag indicates whether a byte has been received recently by uart or not (1 means yes) 
	// the states are: initialize the flag to -2(not receiving) .. and the string counter to 0
	//1-if it's not receiving anything.. make the flag = -2( this may not be implemented in the ISR part .. 
	//but rather in the millis (TMR_OVF interrupt )
	//2- if receiving but the begin symbol (indicated here by { ) is not reached yet .. make
    // the flag = -1(receiving but has found a request start yet : and you must
	//not start any operation with the esp until the flag is returned to -2 (because when sending commands
	//to esp we flip the uart to the polling mode because we send an at command and we wait(poll) for a response 
	//3- if the begin symbol is reached turn the flag o 0(indicating that the a request start has been detected
	//4- if receiving and the flag is 0 then start storing in the buffer in order by the queue (maximum size for
	// each buffer-request- is 10)
	// I'll have to find a way to make the receiver get the size of the buffer equals the number of characters received
	//5- if the string counter exceeded 10 and the end symbol(indicated here by } ) hasn't been reached yet put the
	// flag into -1(indicating receiving but no request found yet) .. and the the string counter = -1(indicating 
	//empty)
	//6-if the end symbol has been found, if the flag = 0 then: the flag = 1(indicating a request has received
	//completely, store the resulting string in the queue and clear the counter
	// .. otherwise (if the flag isn't 0) . then don't change any thing (because there was no request found 
	//at the first place)
	//any time it receives any thing it clears the timer OVF counter(responsible for detecting if the incoming
	//stream is still going or stopped)
	
	//dio_set_val(DIO_PIN31, DIO_HIGH); (indicates a request has been received and stored)
	temp = UDR;
	
	if (receive_flag == -2) //if it's the first char to receive
	{
		//DIO_SET_VAL(31, DIO_LOW);
		millis(3); // activate the timer at the beginning of the reception
		
		receive_flag = -1;//make the flag in state 2(indicating currently receiving but didn't find the begin char yet
		
		if (temp == '{') // if it's the begin character
		{
			//DIO_SET_VAL(0, DIO_HIGH);
			//make the counter = 0; and the flag = 0 indicating storing the next data coming
			receive_flag = 0;
			string_counter = 0;
		}
	}
	else if (receive_flag == -1) // if it's receiving but didn't find the char yet
	{
		//DIO_SET_VAL(1, DIO_HIGH);
		
		if (temp == '{') // if it's the begin character
		{
			//make the counter = 0;  and the flag = 0 indicating storing the next data coming
			receive_flag = 0;
			string_counter = 0;
		}
				
	}
	else if (receive_flag == 0) // if it has found the begin character .. then the next char holds the request type
	{
		//DIO_SET_VAL(2, DIO_HIGH);
		req_type = temp; // store the next char in the req_type and
		receive_flag = 1;//update the state to start putting the data in the buffer
	}
	else if (receive_flag == 1)// //if the first char is read(holds the request type)
	{	
		//DIO_SET_VAL(3, DIO_HIGH);
		
		if (temp == '}') // if the next char is the terminating char
		{
			
			buffer[string_counter] = 0; // put the null at the end of the received chars
			//change the state to -1 indicating that it has ended .. but not sure if the stream of received
			// data has ended or not (it's assigned to -2 in the millis function)
			
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
				
			if(string_counter > 10) // if it exceeded its limits .. then this is an invalid request
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
			rx_end_flag = 0; // clear it and wait for the uart rx interrupt to set it if a byte was received
			                 //by uart              
			millis_reset_tmr(3);          
			counter++ ;                   
		}
		else if (rx_end_flag == 0) // if it's zero then there hasn't been any UART receive operation for the last 5 millis which means
		{
			//int8_t arr[10];
			//stop the timer
			millis_disable_tmr(); // because we don't need it any more as the uart stream has stopped 
			// and we don't need to track whether another byte will be received within specified time or not 
			//DISABLE_TIMER0;
			//put the receive flag to -2 indicating reception process has stopped
			receive_flag = -2; //default value
			string_counter = -1;// default value
			//itoa(counter, arr, 10);
			//UART_sendStrByMark(arr, 0);
			//UART_sendStrByMark("\r\n", 0);
			counter = 0;
			
			
			if (TRUE == str_reached)
			{
				
				store_data();
				str_reached = FALSE;
			}
			
		}
		
	}
	else // if it's working on a received response
	{
		
	}

}


//********************************************************************************************************
//********************************************************************************************************
//********************************************************************************************************
//********************************************************************************************************
//********************************************************************************************************

int main(void)
{
	uint8_t f_st = SUCCESS;
	uint8_t receive_buff[9];
	uint8_t i = 0;
	uint8_t h2o[3] = {0};
	uint8_t temp[4] = {0};
	char ch_num[2] = {0};
		
	char esp_data[12] = {0};
	char break_f[6];
	uint8_t barrier_flag = 0;
	sei();

	DIO_SET_DIR(31, DIO_OUTPUT);
	DIO_SET_DIR(29, DIO_OUTPUT);
	DIO_SET_DIR(0, DIO_OUTPUT);
	DIO_SET_DIR(15, DIO_OUTPUT);	
	DIO_SET_DIR(30, DIO_OUTPUT);	

	list = create_list(); // create the queue that is going to hold the esp requests
	
	motors_init();
	
	//move_forward();
	
	uart_init(UART0, 9600, DATA_BITS_8, TX_RX, PARITY_NO, STOP_BITS_1, TX_R_RX_F);
	
	uart_set_rx_int(UART0, true);
	uart_set_rx_isr(UART0, uart_rx_handler);
	uart_send_until(UART0, "hello\r\n", 0);
	
	millis_add_ovf_isr(timer0_ovf_handler);
	
	esp_init();

	
#if MCP == 1
	if (mcp_init() == ERROR)
	{
		uart_send_str(UART0, "SPI ERROR");	
		while (1);
	}
	
	mcp_set_tx_id(MCP_TX(0), 0, 0x058f);//000 1111 0111 001 10 00110101 10001111
	mcp_set_tx_id(MCP_TX(1), 0, 0x057f);//000 1111 0111 001 10 00110101 10001111
	mcp_set_tx_id(MCP_TX(2), 0, 0x056f);//000 1111 0111 001 10 00110101 10001111
#endif

	 uart_send_until(UART0, "READY", 0);
		
		/* Replace with your application code */
		while (1)
		{

#if MCP == 1
			if (mcp_check_flag(TX1IF_BIT))
			{
				//UART_sendStrByMark("SENT1\r\n", 0);
				mcp_clear_flag(TX1IF_BIT);
			}
			
			if (mcp_check_flag(TX2IF_BIT))
			{
				//UART_sendStrByMark("SENT2\r\n", 0);
				mcp_clear_flag(TX2IF_BIT);
			}
			
			if (mcp_check_flag(RX1IF_BIT))
			{
				
				DIO_TOGGLE(31);
				//PORTA ^= (1 << 2);
				mcp_clear_flag(RX1IF_BIT);
				mcp_get_rx_data(MCP_RX(1), break_f);
				
				if (break_f[0] == 'B')
				{
					
					if (break_f[1] == '1')
					{
						barrier_flag = 1;
						esp_send("{BB}", ch_num);
					}
					else if (break_f[1] == '0')
					{
						barrier_flag = 0;
						esp_send("{BN}", ch_num);
					}
					
				}
				//esp_send_alert(break_f, ch_num);		
			}
#endif
			
			//ESP CODE
			if (list->size > 0)
			{
				
				pop(list, e_ptr, sizeof(element_t));
				uart_send_until(UART0, "\r\nsize > 1 \r\n", 0);
/*				DIO_TOGGLE(0);*/
				
				//then it's an esp request
				if (e_ptr->request_type == 'S')
				{
					f_st = esp_send_smoke(ch_num[0]);
					
					if (f_st == TX1_FAIL)
					{
						uart_send_str(UART0, "\r\nSMOKE CAN TX ERROR\r\n");
					}
					else if (f_st == RX1_FAIL)
					{
						uart_send_str(UART0, "\r\nSMOKE CAN RX ERROR\r\n");
					}
					
				}
				else if (e_ptr->request_type == 'T')
				{
					f_st = esp_send_temp(ch_num[0]);
					
					if (f_st == TX0_FAIL)
					{
						uart_send_str(UART0, "\r\nTEMP CAN TX ERROR\r\n");
					}
					else if (f_st == RX0_FAIL)
					{
						uart_send_str(UART0, "\r\nTEMP CAN RX ERROR\r\n");
					}
				}
				else if (e_ptr->request_type == 'H')
				{
					esp_send_H2O(ch_num[0]);
					
					if (f_st == TX0_FAIL)
					{
						uart_send_str(UART0, "\r\nhumidity CAN TX ERROR\r\n");
					}
					else if (f_st == RX0_FAIL)
					{
						uart_send_str(UART0, "\r\nhumidity CAN RX ERROR\r\n");
					}
				}
				else if (e_ptr->request_type == 'D')
				{
					if ( e_ptr->str[0] == 'R')
					{
						turn_right();
					}
					else if ( e_ptr->str[0] == 'L')
					{
						turn_left();
					}
					else if ( e_ptr->str[0] == 'F')
					{
						
						if (!barrier_flag)
						{
							move_forward();
						}
						
					}
					else if ( e_ptr->str[0] == 'B')
					{
						move_backward();
					}
					else if ( e_ptr->str[0] == 'S')
					{
						motors_stop();
					}
					
				}
				
				alarm();
			}
			else // no +IPD, , found
			{
// 				DIO_TOGGLE(31);
			}

		}
	
	
}

//===============================================================================================================================
//===============================================================================================================================
//===============================================================================================================================
//===============================================================================================================================
//===============================================================================================================================
//===============================================================================================================================
//===============================================================================================================================

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
		uart_send_until(UART0, "\r\n size = ", 0);
		UART_send(list->size + 48);
		uart_send_until(UART0, "\r\n", 0);
		#endif
	}


}

// this construct a string that contains the sensor type and its reading surrounded by brackets(esp_message format and stores 
//this string into esp_buff to be sent 
void sensor_to_esp_buff(char * sensor_str, uint8_t info_type)
{
	
	esp_buff[0] = '{'; //can be moved to a a place where it's executed only once instead of getting executed every time 
	esp_buff[1] = info_type;
	
	uint8_t i = 0;
	while (sensor_str[i] != 0)
	{
		esp_buff[i + 2] = sensor_str[i]; // i + 1 because the first element cantains a bracket({)
		i++ ;
	}
	
	esp_buff[i + 2] = '}'; // indicates the ending of the esp message
	i++ ;
	esp_buff[i + 2] = 0;
}

uint8_t esp_send_smoke(unint8_t ch_num)
{
	char smoke[7] = "1234";
	
#if MCP == 1
	mcp_send_remoteframe(MCP_TX(1), 8);
	volatile int16_t sw_timer = 1500;
	//wait for the flag for approximately 2 sec
	while ((mcp_check_flag(TX1IF_BIT) == 0) && (--sw_timer))
	{
		delay_msec(1);
	}
	
	if (!sw_timer)
	{
		return TX1_FAIL;
	}
	
	mcp_clear_flag(TX1IF_BIT);
	
	//wait for 5 seconds for response
	sw_timer = 2500;
	
	while ((mcp_check_flag(RX1IF_BIT) == 0) && (--sw_timer))
	{
		delay_msec(2);
	}
	
	if (!sw_timer)
	{
		return RX1_FAIL;
	}
	
	mcp_clear_flag(RX1IF_BIT);
	
	mcp_get_rx_data(MCP_RX(1), smoke);
#endif
	sensor_to_esp_buff(smoke, 'S');
	esp_send(esp_buff, ch_num);
	
	return SUCCESS;
}



uint8_t esp_send_temp(unint8_t ch_num)
{
	char temp[4] = "12";
	char receive_buff[9];
	
#if MCP == 1
	mcp_send_remoteframe(MCP_TX(0), 5);
	volatile int16_t sw_timer = 1500;
	
	while ((mcp_check_flag(TX0IF_BIT) == 0) && (--sw_timer))
	{
		_delay_ms(1);
	}
	
	if (!sw_timer)
	{
		return TX0_FAIL;
	}
	 
	mcp_clear_flag(TX0IF_BIT);

	sw_timer = 1500;
	
	while((mcp_check_flag(RX0IF_BIT) == 0) && (--sw_timer))
	{
		delay_msec(1);
	}
	
	if (!sw_timer)
	{
		return RX0_FAIL;
	}
	
	mcp_clear_flag(RX0IF_BIT);
	
	mcp_get_rx_data(MCP_RX(0), receive_buff);
	
	
	temp[0] = receive_buff[2];
	temp[1] = receive_buff[3];
	temp[2] = receive_buff[4];
	temp[3] = 0;
/*	DIO_TOGGLE(0);*/
	//PORTB ^= 1;
#endif
	sensor_to_esp_buff(temp, 'T');
	esp_send(esp_buff, ch_num);
	
	return SUCCESS;
}

uint8_t esp_send_H2O(unint8_t ch_num)
{
	char h2o[3] = "89";
	char receive_buff[9];
	
#if MCP == 1
	mcp_send_remoteframe(MCP_TX(0), 8);
	
	volatile int16_t sw_timer = 1500;
	
	while ((mcp_check_flag(TX0IF_BIT) == 0) && (--sw_timer))
	{
		delay_msec(1);
	}
	
	if (!sw_timer)
	{
		return TX0_FAIL;
	}
	
	mcp_clear_flag(TX0IF_BIT);
	
	sw_timer = 1500;
	
	while((mcp_check_flag(RX0IF_BIT) == 0) && (--sw_timer))
	{
		delay_msec(1);
	}
	
	if (!sw_timer)
	{
		return RX0_FAIL;
	}
	
	mcp_clear_flag(RX0IF_BIT);
	
	mcp_get_rx_data(MCP_RX(0), receive_buff);
	
	h2o[0] = receive_buff[0];
	h2o[1] = receive_buff[1];
	h2o[2] = 0;
	
/*	DIO_TOGGLE(0);*/
#endif
	sensor_to_esp_buff(h2o, 'H');
	esp_send(esp_buff, ch_num);
	
	return SUCCESS;
}



void esp_send(const char * str, unint8_t ch_num)
{
	uint8_t at_flag;
	char data_size_str[4] = {0};
		
	itoa(strlen(str), data_size_str, 10);
	 ch_num += 48;
	uart_send_until(UART0, "AT+CIPSEND=", 0);
	uart_send(UART0, &ch_num, 1); 
	uart_send(UART0, ",", 1);
	uart_send_until(UART0, data_size_str, 0);
	//wait while the esp is receiving some message and then send the \r\n order to make the esp listen to your response
	while (receive_flag != -2);
	//collision of data between the request and the response
	uart_set_rx_int(UART0, false); // disable the interrupt .. to do not listen for any request and listen only
	//for the response of command
	uart_send_until(UART0, "\r\n", 0);
	at_flag = check_strings(500, 2, "OK\r\n", "ERROR\r\n");
	if (!at_flag)
	{
		no_data_alarm();
	}
	else if (at_flag == AT_ERROR)
	{
		error_alarm();
	}

	uart_set_rx_int(UART0, true); //after getting the response activate the interrupt again to listen for any coming requests
	
	uart_send_until(UART0, str, 0);
	uart_send_until(UART0, "\r\n", 0);
}

void no_data_alarm()
{
	
	for (int i = 0; i < 3; i++)
	{
		//PORTC ^= 1 << 5;
		DIO_TOGGLE(31);
		delay_msec(200);
	}
	
}



void esp_send_alert(char * alert, uint8_t ch_num)
{
	if (alert[0] == 'B')
	{
		
		if (alert[1] == '1')
		{
			esp_send("{BB}", ch_num);
		}
		else if (alert[1] == '0')
		{
			esp_send("{BN}", ch_num);
		}
		
	}
	else if (alert[0] == 'M')
	{
		
		if (alert[1] == '1')
		{
			esp_send(": SMOKE", ch_num);
		}
		else if (alert[1] == '0')
		{
			esp_send(": NOBREAK!", ch_num);
		}
		
	}
	
}

void alarm()
{
	
	for (int i = 0; i < 5; i++)
	{
		DIO_TOGGLE(31);
		_delay_ms(100);
	}
	
}




void error_alarm()
{
	
	for (int i = 0; i < 5; i++)
	{
		DIO_TOGGLE(31);
		delay_msec(100);
	}
	
}




#if 0
void get_req()
{
	if (get_string(ch_num, "+IPD,", ",", 500))
	{
					
					
		if (check_strings("::", 100))
		{
						
			//then it's an esp request
			if (get_str_until(esp_data, NULL, 100))
			{
							
				if (esp_data[0] == 'S')
				{
					esp_send_smoke(ch_num[0]);
				}
				else if (esp_data[0] == 'T')
				{
					esp_send_temp(ch_num[0]);
				}
				else if (esp_data[0] == 'H')
				{
					esp_send_H2O(ch_num[0]);
				}
				else if (esp_data[0] == 'D')
				{
					if (esp_data[1] == 'R')
					{
						turn_right();
					}
					else if (esp_data[1] == 'L')
					{
						turn_left();
					}
					else if (esp_data[1] == 'F')
					{
						if (!barrier_flag)
						{
							move_forward();
						}
									
					}
					else if (esp_data[1] == 'B')
					{
						move_backward();
					}
					else if (esp_data[1] == 'S')
					{
						motors_stop();
					}
								
				}
							
			}
						
		}
		
}
#endif