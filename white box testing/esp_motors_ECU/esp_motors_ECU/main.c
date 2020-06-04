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

#include <stdbool.h>

#define R_FORWARD  0
#define R_BACKWARD 1
#define L_FORWARD  2
#define L_BACKWARD 3

#define AT_OK 1
#define AT_ERROR 2

#define MCP 1

#define NOT_RECEIVING -2
#define RECEIVING -1
#define REQUEST_DETECTED 0
#define REQUEST_STORING 1

#define RESET_STR_COUNTER() string_counter = -1 


typedef struct element{
	char * str;
	char request_type;
}element_t;

q_list_t * list;
element_t e;
element_t * e_ptr = &e;
char esp_buff[9] = "{        ";

void esp_init();
void esp_send_temp(unint8_t ch_num);
void esp_send_smoke(unint8_t ch_num);
void esp_send_H2O(unint8_t ch_num);
void alarm();
void esp_send(const char * str, unint8_t ch_num);
void esp_send_alert (char * alert, uint8_t ch_num);
void sensor_to_esp_buff(char * sensor_str, uint8_t info_type);
void store_data();
void no_data_alarm ();
void error_alarm ();

void motor_right()
{
	DIO_SET_VAL(L_FORWARD, DIO_LOW);
	DIO_SET_VAL(L_BACKWARD, DIO_HIGH);
	DIO_SET_VAL(R_FORWARD, DIO_HIGH);
	DIO_SET_VAL(R_BACKWARD, DIO_LOW);
}

inline void motor_stop()
{
	DIO_SET_VAL(L_FORWARD, DIO_LOW);
	DIO_SET_VAL(L_BACKWARD, DIO_LOW);
	DIO_SET_VAL(R_FORWARD, DIO_LOW);
	DIO_SET_VAL(R_BACKWARD, DIO_LOW);
}

inline void motor_forward()
{
	DIO_SET_VAL(R_FORWARD, DIO_HIGH);
	DIO_SET_VAL(R_BACKWARD, DIO_LOW);
	DIO_SET_VAL(L_FORWARD, DIO_HIGH);
	DIO_SET_VAL(L_BACKWARD, DIO_LOW);
}

void motor_backward()
{
	DIO_SET_VAL(R_FORWARD, DIO_LOW);
	DIO_SET_VAL(R_BACKWARD, DIO_HIGH);
	DIO_SET_VAL(L_FORWARD, DIO_LOW);
	DIO_SET_VAL(L_BACKWARD, DIO_HIGH);
}

void motor_left()
{
	DIO_SET_VAL(R_FORWARD, DIO_HIGH);
	DIO_SET_VAL(R_BACKWARD, DIO_LOW);
	DIO_SET_VAL(L_FORWARD, DIO_LOW);
	DIO_SET_VAL(L_BACKWARD, DIO_HIGH);
}

void  motors_init()
{
	DIO_SET_DIR(R_FORWARD, DIO_OUTPUT);  //RF
	DIO_SET_DIR(R_BACKWARD, DIO_OUTPUT); //RB
	DIO_SET_DIR(L_FORWARD, DIO_OUTPUT);  //LF
	DIO_SET_DIR(L_BACKWARD, DIO_OUTPUT); //LB

	
 	pwm_init(PWM1 | PWM2, 1000);  
 	pwm_start(PWM1 | PWM2);       
	pwm_setDuty(PWM1 | PWM2, 80); 
}

volatile int8_t receive_flag = NOT_RECEIVING;
volatile int8_t string_counter = -1; //-1 indicating nothing to receive yet
volatile uint8_t temp = 0;
volatile bool b_str_reached = FALSE; 
volatile uint8_t req_type = 0;
volatile uint16_t counter = 0; 
// 0:indicates that it has ended ... 1: indicates that it's starting
uint8_t rx_end_flag = 0;
volatile int8_t buffer[10];

void uart_rx_handler(void)
{
	rx_end_flag = 1;
	/*[NOTE] : the receiving mechanism is as follows: 
	the algorithms is: initialize the flag to NOT_RECEIVING .. and the string 
	counter to 0
	1-if it's not receiving anything.. make the flag = NOT_RECEIVING(this is
	not implemented in this ISR , but rather in the millis TIMER ISR in the 
	which detects if nothing was received for a period of time)
	2- if receiving but the begin symbol is not reached yet, make
	receive_flag = RECEIVING : and you must not send any commands to the esp
	until the flag returns to NOT_RECEVING to avoid collision.
	3- if the begin symbol is reached, make receive_flag = REQUEST_DETECTED
	4- if receiving and the flag is REQUEST_DETECTED then store the first char
	arrives(which shall indicate the request type) in the req_type and 
	make receive_flag = REQUENST_STORING
	5- if receive_flag = REQUENST_STORING start storing in the buffer in order
	 by the queue(maximum size for each request is 10 chars
	6- if the string counter exceeded 10 and the end wasn't reached yet make 
	 receive_flag = REQUENST_STORING(which makes you omit the current request 
	 and start searching for the begin symbol again .. and reset str counter
	7- if the end symbol has been reached, ,set the b_str_reached flag, store
	 the resulting string in the queue and clear the str counter, if not reached
	 yet, keep storing the request */
	 
	//get the arrived char
	temp = USART_RX_BUF(USART0);
	
	if (NOT_RECEIVING == receive_flag) //if it's the first char to receive
	{
		millis(3);// activate the timer at the beginning of the reception
		receive_flag = RECEIVING; // make the flag in state -1(indicating currently receiving but didn't find the begin char yet
		
		if (temp == '{') // if it's the begin character
		{
			//make the counter = 0;  and the flag = 0 indicating storing the next data coming
			receive_flag = REQUEST_DETECTED;
			string_counter = 0;
		}
	}
	else if (RECEIVING == receive_flag) // if it's receiving but didn't find the char yet
	{
			
		if (temp == '{') // if it's the begin character
		{
			//make the counter = 0;  and the flag = 0 indicating storing the next data coming
			receive_flag = REQUEST_DETECTED;
			string_counter = 0;
		}
				
	}
	else if (REQUEST_DETECTED == receive_flag) // if it has found the begin character .. then the next char holds the request type
	{
		req_type = temp; // store the next char in the req_type and
		receive_flag = REQUEST_STORING;//update the state to start putting the data in the buffer
	}
	else if (REQUEST_STORING == receive_flag)//if the first char is read(holds the request type)
	{
				
		if (temp == '}')//if the end symbol has been reached
		{
			// put the null at the end of the received request
			buffer[string_counter] = 0;
			
			//change the state to RECEIVING indicating that it has ended, but 
			//not sure if the stream of received data has ended or not
			//only in the millis function(where the end of data stream can be
			//detected)received_flag assigned to NOT_RECEIVING 
			receive_flag = RECEIVING;
				
			//TODO: allocate an array in the size of string_counter and store the string in it and pass it to the queue
			//get the counter to the initial state
			RESET_STR_COUNTER();
			//put the str reached flag to 1
			b_str_reached = TRUE;
		}
		else//if the end symbol hasn't been reached yet
		{
			//keep storing the incoming chars into the buffer .. increment the buffer counter
			buffer[string_counter] = temp;
			string_counter++ ;
				
			if(string_counter > 10) // if it exceeded its limits .. then this is a false operation
			{
				RESET_STR_COUNTER();
				// not -2 because it might be still receiving
				receive_flag = RECEIVING; 
			}
				
		}
			
	}		
		
}

void timer0_ovf_handler()
{	
	// timer is working for a received request
	if (receive_flag != NOT_RECEIVING) 
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
			receive_flag = NOT_RECEIVING; //default value
			string_counter = -1;// default value
			//itoa(counter, arr, 10); 
			//UART_sendStrByMark(arr, 0);
			//UART_sendStrByMark("\r\n", 0);
			counter = 0;
			
			store_data();
		}
		
	}
	else // if it's working on a received response
	{
		
	}

}

int main(void)
{
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
	DIO_SET_DIR(15, DIO_OUTPUT);	
	DIO_SET_DIR(30, DIO_OUTPUT);
	
	list = q_create_list(); // create the queue that is going to hold the esp requests
	
	usart_init(USART0, 9600, DATA_SIZE_8, TX_RX, NO_PARITY, ONE_STOP_BIT, 
	               TX_RISE_RX_FALL); 
	
	usart_set_rx_isr(USART0, uart_rx_handler);
	millis_add_ovf_isr(timer0_ovf_handler);
	usart_set_rx_int(USART0, false);
	
	//esp_init();	
	
#if MCP == 1
	mcp_init();

	mcp_tx_id(MCP_TX(0), 0, 0x058f);//000 1111 0111 001 10 00110101 10001111
	mcp_tx_id(MCP_TX(1), 0, 0x057f);//000 1111 0111 001 10 00110101 10001111
	mcp_tx_id(MCP_TX(2), 0, 0x056f);//000 1111 0111 001 10 00110101 10001111
#endif

	#if 0
	while (false == (USART_DATA_READY(USART0)))//USART_DATA_READY(USART0))
	{
		usart_write(USART0, 'W');
		_delay_ms(1000);
	}
	#endif
	
	usart_write_str_mark(USART0, "READY", 0);
	
	
	
	/* Replace with your application code */
	motors_init();                       
	usart_set_rx_int(USART0, true);		
	motor_backward();	   

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
				
				DIO_TOGGLE(29);
				//PORTA ^= (1 << 2);
				mcp_clear_flag(RX1IF_BIT);
				mcp_rx_data(MCP_RX(1), break_f);
				
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
				
				q_pop(list, e_ptr, sizeof(element_t));

				//then it's an esp request
				if (e_ptr->request_type == 'S')
				{
					esp_send_smoke(ch_num[0]);
				}
				else if (e_ptr->request_type == 'T')
				{
					esp_send_temp(ch_num[0]);
				}
				else if (e_ptr->request_type == 'H')
				{
					esp_send_H2O(ch_num[0]);
				}
				else if (e_ptr->request_type == 'D')
				{
					if ( e_ptr->str[0] == 'R')
					{
					motor_right();
					}
					else if ( e_ptr->str[0] == 'L')
					{
						motor_left();
					}
					else if ( e_ptr->str[0] == 'F')
					{
						if (!barrier_flag)
						{
							motor_forward();
						}
						
					}
					else if ( e_ptr->str[0] == 'B')
					{
						motor_backward();
					}
					else if ( e_ptr->str[0] == 'S')
					{
						motor_stop();
					}
					
				}
				
				alarm();
			}
			else // no +IPD, , found
			{
				DIO_TOGGLE(31);
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
		//whenever the data that is being pointed to(buffer in this case) changes .. the node data in the queue will
		//change automatically despite the fact that we didn't want that .. so the right move is
		//to not but the pointer point to the buffer but instead copy the buffer data to the pointer and then pass the pointer
		//array instead of a pointer because in this time th element structure itself is going to contain the actual
		//data rather than a pointer to this data and when you push it you're ...
		e_ptr->str = malloc(strlen(buffer) + 1);
		memcpy(e_ptr->str, buffer, strlen(buffer) + 1);
		e_ptr->request_type = req_type;
		q_push(list, e_ptr, sizeof(element_t));// push the request in the Queue
		#if 0
		UART_sendStrByMark("\r\n size = ", 0);
		UART_sendChr(list->size + 48);
		UART_sendStrByMark("\r\n", 0);
		#endif
	}


}

//this construct a string that contains the sensor type and its reading
//surrounded by brackets(esp_message format and stores this string
//into esp_buff to be sent
void sensor_to_esp_buff(char * sensor_str, uint8_t info_type)
{
	uint8_t i = 0;
	esp_buff[0] = '{'; //can be moved to a a place where it's executed only once instead of getting executed every time 
	esp_buff[1] = info_type;
	 
	while (sensor_str[i] != 0)
	{
		esp_buff[i + 2] = sensor_str[i]; // i + 1 because the first element cantains a bracket({)
		i++ ;
	}
	
	esp_buff[i + 2] = '}'; // indicates the ending of the esp message
	i++ ;
	esp_buff[i + 2] = 0;
}

void esp_send_smoke(unint8_t ch_num)
{
	char smoke[7] = "1234";
	
#if MCP == 1
	mcp_send_remoteframe(MCP_TX(1), 8);
	
	while (mcp_check_flag(TX1IF_BIT) == 0)
	{
		delay_msec(500);
		DIO_TOGGLE(31);
	}
	
	mcp_clear_flag(TX1IF_BIT);
	
	while(mcp_check_flag(RX1IF_BIT) == 0)
	{
		delay_msec(100);
		DIO_TOGGLE(31);
		//PORTA ^= 1;
	}
	
	mcp_clear_flag(RX1IF_BIT);
	
	mcp_rx_data(MCP_RX(1), smoke);
#endif
	sensor_to_esp_buff(smoke, 'S');
	esp_send(esp_buff, ch_num);
}

void esp_send_temp(unint8_t ch_num)
{
	char temp[4] = "12";
	char receive_buff[9];
	
#if MCP == 1
	mcp_send_remoteframe(MCP_TX(0), 5);
	
	while (mcp_check_flag(TX0IF_BIT) == 0);
#if 0
	{
		delay_msec(500);
		DIO_TOGGLE(31);
		//PORTA ^= 1;
	}
#endif
	
	 
	mcp_clear_flag(TX0IF_BIT); 
	
	while(mcp_check_flag(RX0IF_BIT) == 0)
	{
		delay_msec(100);
		DIO_TOGGLE(31);
		//PORTA ^= 1;
	}
	
	mcp_clear_flag(RX0IF_BIT);
	
	mcp_rx_data(MCP_RX(0), receive_buff);
	
	
	temp[0] = receive_buff[2];
	temp[1] = receive_buff[3];
	temp[2] = receive_buff[4];
	temp[3] = 0;
	DIO_TOGGLE(31);
	//PORTB ^= 1;
#endif
	sensor_to_esp_buff(temp, 'T');
	esp_send(esp_buff, ch_num);
	
}

void esp_send_H2O(unint8_t ch_num)
{
	char h2o[3] = "89";
	char receive_buff[9];
	
#if MCP == 1
	mcp_send_remoteframe(MCP_TX(0), 5);
	
	while (mcp_check_flag(TX0IF_BIT) == 0)
	{
		delay_msec(500);
		DIO_TOGGLE(31);
		//PORTA ^= 1;
	}
	
	mcp_clear_flag(TX0IF_BIT);
	
	while(mcp_check_flag(RX0IF_BIT) == 0)
	{
		delay_msec(100);
		DIO_TOGGLE(31);
	}
	
	mcp_clear_flag(RX0IF_BIT);
	
	mcp_rx_data(MCP_RX(0), receive_buff);
	
	h2o[0] = receive_buff[0];
	h2o[1] = receive_buff[1];
	h2o[2] = 0;
	
	DIO_TOGGLE(0);
#endif
	sensor_to_esp_buff(h2o, 'H');
	esp_send(esp_buff, ch_num);
	
}



void esp_send(const char * str, unint8_t ch_num)
{
	uint8_t at_flag;
	char data_size_str[4] = {0};
		
	itoa(strlen(str), data_size_str, 10);
	
	usart_write_str_mark(USART0, "AT+CIPSEND=", 0);
	usart_write(USART0, ch_num + 48); 
	usart_write(USART0, ',');
	usart_write_str_mark(USART0, data_size_str, 0);
	//wait while the esp is receiving some message and then send the \r\n order to make the esp listen to your response
	while (receive_flag != -2);
	//collision of data between the request and the response
	usart_set_rx_int(USART0, false); // disable the interrupt .. to do not listen for any request and listen only for the response of command
	usart_write_str_mark(USART0, "\r\n", 0);
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
	
	usart_write_str_mark(USART0, str, 0);
	usart_write_str_mark(USART0, "\r\n", 0);
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
		delay_msec(100);
	}
	
}

void esp_init()
{
	uint8_t at_flag;
	
	usart_write_str_mark(USART0, "AT\r\n", 0);
	at_flag = check_strings(2000, 2, "OK\r\n", "ERROR\r\n");
	
	if ( !at_flag)
	{
		no_data_alarm();
	}
	else if (at_flag == AT_ERROR)
	{
		error_alarm();
	}
	else
	{
		alarm();
	}
	
	usart_write_str_mark(USART0, "AT+CWMODE=1\r\n", 0);
	
	at_flag = check_strings(500, 2, "OK\r\n", "ERROR\r\n");
	if (!at_flag)
	{
		no_data_alarm();
	}
	else if (at_flag == AT_ERROR)
	{
		error_alarm();
	}
	
	usart_write_str_mark(USART0, "AT+CIPMUX=1\r\n", 0);
	at_flag = check_strings(500, 2, "OK\r\n", "ERROR\r\n");
	
	if (!at_flag)
	{
		no_data_alarm();
	}
	else if (at_flag == AT_ERROR)
	{
		error_alarm();
	}
	
	
	usart_write_str_mark(USART0, "AT+CIFSR\r\n", 0);
	at_flag = check_strings(500, 2, "OK\r\n", "ERROR\r\n");
	if (!at_flag)
	{
		no_data_alarm();
	}
	else if (at_flag == AT_ERROR)
	{
		error_alarm();
	}
	
	usart_write_str_mark(USART0, "AT+CIPSERVER=1,80\r\n", 0);
	at_flag = check_strings(500, 2, "OK\r\n", "ERROR\r\n");
	if (!at_flag)
	{
		no_data_alarm();
	}
	else if (at_flag == AT_ERROR)
	{
		error_alarm();
	}
	
	usart_write_str_mark(USART0, "AT+CIPSTO=1000\r\n", 0);
	at_flag = check_strings(500, 2, "OK\r\n", "ERROR\r\n");
	if (!at_flag)
	{
		no_data_alarm();
	}
	else if (at_flag == AT_ERROR)
	{
		error_alarm();
	}
	
}

void no_data_alarm()
{
	
	for (int i = 0; i < 5; i++)
	{
		DIO_TOGGLE(31);
		delay_msec(200);
	}
	
}

void error_alarm()
{
	
	for (int i = 0; i < 10; i++)
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