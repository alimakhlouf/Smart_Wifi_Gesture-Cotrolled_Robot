/*
 * ESP_1.c
 *
 * Created: 2/25/2019 07:45:26 ص
 * Author : hppp
 */ 

#include "micro_config.h"
#include "uart_api.h"
#include "string.h"
#include <math.h>
#include "millis_api.h"
#include "usart_at_handler.h"
#include "stdbool.h"


void esp_send(char info_type , const char * str, unint8_t ch_num);
void send_hello(unint8_t ch_num);
void send_back(unint8_t ch_num, char * esp_data);
void esp_send_temp(unint8_t ch_num);
void esp_send_smoke(unint8_t ch_num);
void esp_send_H2O(unint8_t ch_num);
void alarm();

//ESP MODULE MACROS ***************************
#define ESP_ERROR 2
#define ESP_OK 1
#define ESP_NO_RESPONSE 0 
#define ESP_CONNECTED '2'
#define ESP_NOT_CONNECTED '5' 
#define ESP_MULTIPLE_CONN 1
#define ESP_SINGLE_CONN 0
#define ESP_NORMAL_MODE 0
#define ESP_PASSTHROUGH_MODE 1

//ESP CONFIGURATIONS

/*ESP_NORMAL_MODE: 
(multiple connections can only be activated in here) 
  
  ESP_PASSTHROUGH: 
(During the UART-Wi-Fi passthrough transmission, if the TCP connection breaks,
ESP8266 will keep trying to reconnect until +++ is input to exit the 
transmission. If it is a normal TCP transmission and the TCP connection breaks,
ESP8266 will give a prompt and will not attempt to reconnect.
*/
#define ESP_TRANSMISSION_MODE ESP_NORMAL_MODE

//ESP_MULTIPLE_CONN 
//ESP_SINGLE_CONN 
#define ESP_MULTIPLE_OR_SINGLE_CONN ESP_MULTIPLE_CONN

/*ESP_AP
  ESP_STATION
  ESP_AP_STATION
*/
#define ESP_MODE ESP_STATION


//TRUE
//FALSE
#define ESP_SERVER_MODE TRUE

#if (ESP_SERVER_MODE == TRUE)
#define ESP_SERVER_PORT 80
#define ESP_SERVER_CONN_TOUT 1000
#define ESP_SERVER_MAX_CLIENTS 1
#endif



//ESP_

#define 

//**********************************************

uint8_t ssid[11]     = "tedata2";
uint8_t password[11] = "3#Tm6d#W4s";


int main(void)
{
	DDRB = 0x03;
	PORTB |= 1;
	
    esp_init();
	uint8_t esp_response = 0; 
	_delay_ms(5000);
	
	esp_response = esp_mult_conn(true);
	
	if (esp_response == ESP_OK)
	{
		uart_send_str(UART0, "mult_resp_ok\r\n");
	}
	else if (esp_response == ESP_ERROR)
	{
		uart_send_str(UART0, "mult_resp_error\r\n");
	}
	else if (esp_response == ESP_NO_RESPONSE)
	{
		uart_send_str(UART0, "mult_resp_resp\r\n");
	}
	
	
	
	if (esp_is_connected() == ESP_NOT_CONNECTED)
	{ 
		esp_response = esp_connect(ssid, password);
		
		if (esp_response == ESP_OK)
		{
			uart_send_str(UART0, "connect_ok\r\n");
		}
		else if (esp_response == ESP_ERROR)
		{
			uart_send_str(UART0, "connect_error\r\n");
		}
		else if (esp_response == ESP_NO_RESPONSE)
		{
			uart_send_str(UART0, "connect_no_resp\r\n");
		}
		
	}
	
	
	
}


void esp_init()
{
	uint8_t conf_reg;
	
	uart_init(UART0, 9600, DATA_BITS_8, TX_RX, PARITY_NO, STOP_BITS_1, TX_R_RX_F);
	
	uart_set_rx_int(UART0, false);
	
	conf_reg = ESP_TRANSMISSION_MODE + 49 ; // convert the digit to ascii code
	uart_send_str(UART0, "AT+CIPMODE=");
	uart_send(&conf_reg, 1);
	
}

uint8_t esp_connect(uint8_t * ssid, uint8_t * password)
{
	uart_send_str(UART0, "AT+CWJAP=\"");
	uart_send_str(UART0, ssid);
	uart_send_str(UART0, "\",\"");
	uart_send_str(UART0, password);
	uart_send_str(UART0, "\"\r\n");
	
	return check_strings(5000, 2, "OK\r\n", "ERROR\r\n");
}

uint8_t esp_is_connected()
{
	uint8_t str[5];
	uart_send_until(UART0, "AT+CIPSTATUS\r\n", '\0');
	
	if (get_string(str, "S:", "\r", 500) == TRUE)
	{
		return str[0];
	}
	else
	{
		return ESP_ERROR;
	}
	
}

uint8_t esp_mult_conn(bool af_is_mult)
{
	
	if (af_is_mult == true)
	{
		uart_send_str(UART0, "AT+CIPMUX=1\r\n");
	}
	else
	{
		uart_send_str(UART0, "AT+CIPMUX=0\r\n");
	}
	
	return check_strings(500, 2, "OK\r\n", "ERROR\r\n");
	
// 	if (!at_flag)
// 	{
// 		//no_data_alarm();
// 		
// 	}
// 	else if (at_flag == ESP_ERROR)
// 	{
// 		//error_alarm();
// 		
// 	}
// 	else if ()
	
}

uint8_t esp_transm_mode(uint8_t a_transmission_mode)
{
	
	if (a_transmission_mode == ESP_NORMAL_MODE)
	{
		uart_send_str(UART0, "AT+CIPMODE=0\r\n");
	}
	else if (a_transmission_mode == ESP_PASSTHROUGH_MODE)
	{
		uart_send_str(UART0, "AT+CIPMODE=1\r\n");
	}
	
	return check_strings(500, 2, "OK\r\n", "ERROR\r\n");
	
}

uint8_t esp_create_server(uint16_t a_port)
{
	uint8_t port_str[7];
	itoa(a_port, port_str, 10);
	uart_send_str(UART0, "AT+CIPSERVER=1,");
	uart_send_str(UART0, port_str);
	uart_send_str(UART0, "\r\n");
}

uint8_t esp_conn_timeout(uint8_t a_minutes)
{
	uint8_t minutes_str[7];
	uart_send_str(UART0, "AT+CIPSTO=");
	itoa(a_minutes, minutes_str, 10);
	uart_send_str(UART0, minutes_str);
	uart_send_str(UART0, "\r\n");
	return = check_strings(500, 2, "OK\r\n", "ERROR\r\n");
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


////////////////////////////////////////////////////////////////////////////////////

void send_back(unint8_t ch_num, char * esp_data)
{
	if (get_string(esp_data, ":", NULL, 10))
	{
		char data_size[4] = {0};
		itoa((strlen(esp_data)) , data_size, 10);
		UART_sendStrByMark("AT+CIPSEND=", 0);
		UART_sendChr(ch_num);
		UART_sendChr(',');
		UART_sendStrByMark(data_size, 0);
		UART_sendStrByMark("\r\n", 0);
		
		if (check_string("OK\r\n", 2000))
		{
			//PORTB ^= 1;
			_delay_ms(5);
			
		}
		else
		{
			alarm();
		}
		
		UART_sendStrByMark(esp_data, 0);
		UART_sendStrByMark("\r\n", 0);
		
		if (check_string("SEND OK\r\n", 2000))
		{
			PORTB ^= 1;
			_delay_ms(5);
		}
		else
		{
			alarm();
		}
		
	}
}

void send_smoke()
{
	unint16_t ppm = 0;
	char arr[6];
	
	ppm = MQ2_get_result();
	itoa(ppm, arr, 10);
	UART_sendStrByMark("ppm: ", 0);
	UART_sendStrByMark(arr, 0);
	UART_sendChr(13);
	UART_sendChr(10);
}









//
//
//
//
// void send_hello(unint8_t ch_num)
// {
// 	unint16_t ppm = 0;
// 	char smoke[6];
// 	char temp[4];
// 	char H2O[4];
//
//
// 	unint16_t datasize_int;
// 	char datasize[6];
//
// 	ppm = MQ2_get_result();
// 	itoa(ppm, smoke, 10);
//
// 	DHT11_read();
// 	itoa(DHT11_data[2], temp, 10);
// 	itoa(DHT11_data[0], H2O, 10);
// 	datasize_int = 72 + strlen(smoke) + strlen(temp) + strlen(H2O);
//
// 	itoa(datasize_int, datasize, 10);
// 	UART_sendStrByMark("AT+CIPSEND=", 0);
// 	UART_sendChr(ch_num);
// 	UART_sendStrByMark(",", 0);
// 	UART_sendStrByMark(datasize, 0);
// 	UART_sendStrByMark("\r\n", 0);
//
//
// 	if (check_string("OK\r\n", 2000)) // check the response of the cipsend command
// 	{
// 		//PORTB ^= 1;
// 		_delay_ms(5);
// 	}
// 	else
// 	{
// 		alarm();
// 	}
//
// 	UART_sendStrByMark("<h1>Welcome ALI", 0);
// 	UART_sendStrByMark("</h1><h2>SMOKE: ", 0);
// 	UART_sendStrByMark(smoke, 0);
// 	UART_sendStrByMark(" ppm</h2><h3>TEMP: ", 0);
// 	UART_sendStrByMark(temp, 0);
// 	UART_sendStrByMark(" C</h3><h4>H2O: ", 0);
// 	UART_sendStrByMark(H2O, 0);
// 	UART_sendStrByMark("%</h4>\r\n", 0); //57
//
//
// 	if (check_string("SEND OK\r\n", 2000))
// 	{
// 		PORTB ^= 1;
// 		_delay_ms(5);
// 	}
// 	else
// 	{
// 		alarm();
// 	}
//
// }
//
// void esp_send_smoke(unint8_t ch_num)
// {
// 	unint16_t ppm;
// 	char smoke[8] = "1234";
//
//
// #if 0
// 	ppm = MQ2_get_result();
// 	itoa(ppm, smoke, 10);
// #endif
//
// 	esp_send('S', smoke, ch_num);
// }
//
// void esp_send_temp(unint8_t ch_num)
// {
// 	char temp[3] = "25";
//
// #if 0
// 	DHT11_read();
// 	itoa(DHT11_data[2], temp, 10);
// #endif
//
// 	esp_send('T', temp, ch_num);
// }
//
// void esp_send_H2O(unint8_t ch_num)
// {
// 	char H2O[3] = "48";
//
// #if 0
// 	DHT11_read();
// 	itoa(DHT11_data[0], H2O, 10);
// #endif
//
// 	esp_send('H', H2O, ch_num);
// }
//
// void esp_send(char info_type , const char * str, unint8_t ch_num)
// {
// 	char data_size_str[4] = {0};
//
// 	if (info_type)
// 	{
// 		itoa((strlen(str) + 1), data_size_str, 10);// we are adding a char indicating the type of data we are getting
// 	}
// 	else
// 	{
// 		itoa(strlen(str), data_size_str, 10);// we are adding a char indicating the type of data we are getting
// 	}
//
// 	UART_sendStrByMark("AT+CIPSEND=", 0);
// 	UART_sendChr(ch_num);
// 	UART_sendChr(',');
// 	UART_sendStrByMark(data_size_str, 0);
// 	UART_sendStrByMark("\r\n", 0);
//
// 	if (check_string("OK\r\n", 2000))
// 	{
// 		//PORTB ^= 1;
// 		_delay_ms(5);
//
// 	}
// 	else
// 	{
// 		alarm();
// 	}
//
// 	if (info_type)
// 	{
// 		UART_sendChr(info_type);
// 	}
//
// 	UART_sendStrByMark(str, 0);
// 	UART_sendStrByMark("\r\n", 0);
// }