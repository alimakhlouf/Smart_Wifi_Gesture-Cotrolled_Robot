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
#define ESP_SEND_FAIL 3
#define ESP_ALREADY_CONNECTED 3  
#define ESP_CONNECTED '2'
#define ESP_NOT_CONNECTED '5' 
#define ESP_SINGLE_CONN 0
#define ESP_MULTIPLE_CONN 1
#define ESP_NORMAL_MODE 0
#define ESP_PASSTHROUGH_MODE 1
#define ESP_STATION 1
#define ESP_AP 2
#define ESP_STATION_AP 3
#define ESP_TCP 1
#define ESP_UDP 2

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

/*
  ESP_STATION
  ESP_AP
  ESP_STATION_AP
*/
#define WIFI_MODE ESP_STATION

//ESP functions declarations 
uint8_t esp_connect (uint8_t * ssid, uint8_t * password);
uint8_t esp_is_connected ();
uint8_t esp_mult_conn (bool af_is_mult);
uint8_t esp_init ();
uint8_t esp_max_clients (uint8_t max_cli);
uint8_t esp_transm_mode (uint8_t a_transmission_mode);
uint8_t esp_max_clients (uint8_t max_cli);
uint8_t esp_create_server (uint16_t a_port);
uint8_t esp_connect_site (uint8_t * site_name, uint8_t conn_type, uint16_t port_num,  uint8_t id_chr);
uint8_t esp_conn_timeout (uint16_t a_minutes);
uint8_t esp_send_get (uint8_t * site_name, uint8_t * page_name, uint8_t * get_req,
						  uint8_t conn_type, uint16_t port_num,  uint8_t id_chr);

//*********************************************


int main(void)
{
	
	
	
	uint8_t esp_response = 0;
	
	_delay_ms(5000);
	
	esp_response = esp_init();
	
	if (esp_response == ESP_OK)
	{
		uart_send_until(UART0, "esp init OK \r\n", 0);
	}
	else if (esp_response == ESP_ERROR)
	{
		uart_send_until(UART0, "esp init ERROR \r\n", 0);
	}
	else if (esp_response == ESP_NO_RESPONSE)
	{
		uart_send_until(UART0, "esp init NO RESPONSE \r\n", 0);
	}
	
	_delay_ms(100);
	
	esp_response = esp_is_connected();
	
	if (esp_response == ESP_NOT_CONNECTED)
	{
		esp_response = esp_connect("tedata2", "3#Tm6d#W4s");
		
		if (esp_response == ESP_OK)
		{
			//uart_send_until(UART0, "connect_ok\r\n", 0);
		}
		else if (esp_response == ESP_ERROR)
		{
			uart_send_until(UART0, "connect_error\r\n", 0);
		}
		else if (esp_response == ESP_NO_RESPONSE)
		{
			uart_send_until(UART0, "connect_no_resp\r\n", 0);
		}
		
	}
	else if (esp_response == ESP_CONNECTED)
	{
		//uart_send_until(UART0, "already connected \r\n", 0);
	}
	else
	{
		uart_send_until(UART0, "connection response error\r\n", 0) ;
	}
	
	_delay_ms(100);
		
	#if 0
esp_response = esp_max_clients(2);
	
	if (esp_response == ESP_OK)
	{
		//uart_send_until(UART0, "esp max cli OK \r\n", 0);
	}
	else if (esp_response == ESP_ERROR)
	{
		uart_send_until(UART0, "esp max cli error \r\n", 0);
	}
	else if (esp_response == ESP_NO_RESPONSE)
	{
		uart_send_until(UART0, "esp max cli no response \r\n", 0);
	}
	
	_delay_ms(100);
#endif
	
	esp_response = esp_create_server(80);
	if (esp_response == ESP_OK)
	{
		//uart_send_until(UART0, "server OK \r\n", 0);
	}
	else if (esp_response == ESP_ERROR)
	{
		uart_send_until(UART0, "server error \r\n", 0);
	}
	else if (esp_response == ESP_NO_RESPONSE)
	{
		uart_send_until(UART0, "server no response \r\n", 0);
	}
	
	_delay_ms(100);
	
	esp_response = esp_conn_timeout(1000);
	if (esp_response == ESP_OK)
	{
		//uart_send_until(UART0, "timeout OK \r\n", 0);
	}
	else if (esp_response == ESP_ERROR)
	{
		uart_send_until(UART0, "timeout error \r\n", 0);
	}
	else if (esp_response == ESP_NO_RESPONSE)
	{
		uart_send_until(UART0, "timeout no response \r\n", 0);
	}
	
	esp_response = esp_send_get("embiotesp8266.000webhostapp.com","saveout.php",
								    "apples=40&oranges=40", ESP_TCP, 80, '0');
	if (esp_response == ESP_OK)
	{
		//uart_send_until(UART0, "website connection successful\r\n", 0);
	}
	else if (esp_response == ESP_ERROR)
	{
		uart_send_until(UART0, "website connection error\r\n", 0);
		return esp_response;
	}
	else if(esp_response == ESP_NO_RESPONSE)
	{
		uart_send_until(UART0, "website connection no response\r\n", 0);
		return esp_response;
	}
	else
	{
		
	}
	
	
	while(1);

	
}






uint8_t esp_receive_conn(uint8_t a_id, uint16_t a_minutes)
{	
	uint8_t recv_id_str[3];
	check_strings (a_minutes, 1, "CONNECT");
	
	if (get_string(recv_id_str, "+IPD,", ",", 1000) == TRUE)
	{
		if (recv_id_str[0] == a_id)
		{
			return ESP_OK;
		}
		else
	}
}


uint8_t esp_send_get(uint8_t * site_name, uint8_t * page_name, uint8_t * get_req,
						 uint8_t conn_type, uint16_t port_num,  uint8_t id_chr)
{
	
	//we have to send CIPSEND later and we must pass the length of the request as a parameter to it
	uint16_t req_length = 0;
	/*the request is in this form
	  GET /(page_addr)?(get_req) HTTP/1.0\r\nHOST: (site_adr)\r\n\r\n\r\n   (the brackets are excluded)
	*/
	req_length += 27; //  for characters for in the request except for 
					  //site_addr, page_addr, ger_req
	req_length += strlen(page_name);
	req_length += strlen(site_name);
	req_length += strlen(get_req);
	
	//converting the length number to string
	uint8_t length_str[5];
	itoa(req_length, length_str, 10);
	
	
	//we connect to the website first 
	//I didn't call esp_connect_site in this function as it will take all
	//the site addr as a parameter again which will consume a lot of additional
	// RAM so I exchanged the additional RAM with additional ROM and copied the
	//function body  
	
	uint8_t port_str [5];
	itoa(port_num, port_str, 10);
	
	uart_send_until(UART0, "AT+CIPSTART=", 0);
	
	#if ESP_MULTIPLE_OR_SINGLE_CONN == ESP_MULTIPLE_CONN
	uart_send(UART0, &id_chr, 1);
	uart_send(UART0, ",", 1);
	#endif
	
	if (conn_type == ESP_TCP)
	{
		uart_send_until(UART0, "\"TCP\",", 0);
	}
	else if (conn_type == ESP_UDP)
	{
		uart_send_until(UART0, "\"UDP\",", 0);
	}
	else
	{
		return PARAMETER_ERROR;
	}
	uart_send_until(UART0, "\"", 0);
	uart_send_until(UART0, site_name, 0);
	uart_send_until(UART0, "\"", 0);
	uart_send_until(UART0, ",", 0);
	uart_send_until(UART0, port_str, 0);
	uart_send_until(UART0, "\r\n", 0);
	
	uint8_t esp_response = check_strings(500, 3, "OK\r\n", "RROR\r\n", "ALREADY");
	
	if (esp_response == ESP_ALREADY_CONNECTED) // 3 :already connected
	{
		//it's already connected so wait till the response ends and continue normally
		_delay_ms(20); 
	}// else if the response is not OK then return the function with the response
	else if (esp_response != ESP_OK)
	{
		return esp_response;
		//uart_send_until(UART0, "website connection successful\r\n", 0);
	}
	
	//here the connection is connected to the website
	//now we have to send CIPSEND 
	uart_send_until(UART0, "AT+CIPSEND=", 0);
	
	#if ESP_MULTIPLE_OR_SINGLE_CONN == ESP_MULTIPLE_CONN
	uart_send(UART0, &id_chr, 1);
	uart_send(UART0, ",", 1);
	#endif
	
	uart_send_until(UART0, length_str, 0);
	uart_send_until(UART0, "\r\n", 0);
	esp_response = check_strings(1000, 2, "OK\r\n", "ERROR\r\n");
	
	if (esp_response != ESP_OK)
	{
		return esp_response;
		//uart_send_until(UART0, "website connection successful\r\n", 0);
	}
	
	//GET /(page_name)?(get_req) HTTP/1.0\r\nHOST: (site_name)\r\n\r\n\r\n   (the brackets are excluded)
	uart_send_until(UART0, "GET /", 0);
	uart_send_until(UART0, page_name, 0);
	uart_send_until(UART0,"?", 0);
	uart_send_until(UART0, get_req, 0);
	uart_send_until(UART0, " HTTP/1.0\r\nHOST: ", 0);
	uart_send_until(UART0, site_name, 0);
	uart_send_until(UART0, "\r\n\r\n\r\n", 0);
	
	esp_response = check_strings(500, 3, "SEND OK\r\n", "ERROR\r\n", "SEND FAIL\r\n");
	if (esp_response != ESP_OK) //SEND OK	
	{
		return esp_response;
	}
	
	//if everything works fine
	return ESP_OK;
}



uint8_t esp_connect_site(uint8_t * site_name, uint8_t conn_type, uint16_t port_num,  uint8_t id_chr)
{
	
	uint8_t port_str[5]; 
	itoa (port_num, port_str, 10);
	uart_send_until(UART0, "AT+CIPSTART=", 0);
	
	#if ESP_MULTIPLE_OR_SINGLE_CONN == ESP_MULTIPLE_CONN
	uart_send(UART0, &id_chr, 1);
	uart_send(UART0, ",", 1);
	#endif
	
	if (conn_type == ESP_TCP)
	{
		uart_send_until(UART0, "\"TCP\",", 0);
	}
	else if (conn_type == ESP_UDP)
	{
		uart_send_until(UART0, "\"UDP\",", 0);
	}
	else
	{
		return PARAMETER_ERROR;
	}
	uart_send_until(UART0, "\"", 0);
	uart_send_until(UART0, site_name, 0);
	uart_send_until(UART0, "\"", 0);
	uart_send_until(UART0, ",", 0);
	uart_send_until(UART0, port_str, 0);
	uart_send_until(UART0, "\r\n", 0);
	uint8_t esp_response = check_strings(3000, 3, "OK\r\n", "RROR\r\n", "ALREADY");
	
	if (esp_response != ESP_OK)
	{
		return esp_response;
		//uart_send_until(UART0, "website connection successful\r\n", 0);
	}
	
	return ESP_OK;
}


uint8_t esp_init()
{
	uint8_t conf_reg = 0, esp_response = 0;
	
	
	uart_init(UART0, 9600, DATA_BITS_8, TX_RX, PARITY_NO, STOP_BITS_1, TX_R_RX_F);
	uart_set_rx_int(UART0, false);
	uart_set_tx_int(UART0, false);
	
	conf_reg = ESP_TRANSMISSION_MODE + 48 ; // convert the digit to ascii code
	uart_send_until(UART0, "AT+CIPMODE=", 0);
	uart_send(UART0, &conf_reg, 1);
	uart_send_until(UART0, "\r\n", 0);
	esp_response = check_strings(500, 2, "OK\r\n", "ERROR\r\n", 0);
	
	if (esp_response != ESP_OK)
	{
		return esp_response;
	}
	
	conf_reg = ESP_MULTIPLE_OR_SINGLE_CONN + 48;
	uart_send_until(UART0, "AT+CIPMUX=", 0);
	uart_send(UART0, &conf_reg, 1);
	uart_send_until(UART0, "\r\n", 0);
	esp_response = check_strings(500, 2, "OK\r\n", "ERROR\r\n");
	
	if (esp_response != ESP_OK)
	{
		return esp_response;
	}
	
	conf_reg = WIFI_MODE + 48;
	uart_send_until(UART0, "AT+CWMODE=", 0);
	uart_send(UART0, &conf_reg, 1);
	uart_send_until(UART0, "\r\n", 0);
	esp_response = check_strings(500, 2, "OK\r\n", "ERROR\r\n");

	if (esp_response != ESP_OK)
	{
		return esp_response;
	}
	
	return ESP_OK;
}


uint8_t esp_connect(uint8_t * ssid, uint8_t * password)
{
	uart_send_until(UART0, "AT+CWJAP=\"", 0);
	uart_send_until(UART0, ssid, 0);
	uart_send_until(UART0, "\",\"", 0);
	uart_send_until(UART0, password, 0);
	uart_send_until(UART0, "\"\r\n", 0);
	
	return check_strings(5000, 2, "OK\r\n", "ERROR\r\n");
}

uint8_t esp_is_connected()
{
	uint8_t str[5];
	uart_send_until(UART0, "AT+CIPSTATUS\r\n", 0);
	
	if (get_string(str, "S:", "\r", 500) == TRUE)
	{
		return str[0];
	}
	else
	{
		return ESP_ERROR;
	}
	
}




//the maximum number of clients allowed to connect to the TCP or SSL server, range: [1, 5]
// you should call the command before creating a server.
//IT'S NOT WORKING FOR SOME REASON
uint8_t esp_max_clients(uint8_t max_cli)
{
	max_cli += 48; //convert the digit to ascii
	uart_send_until(UART0, "AT+CIPSERVERMAXCONN=", 0); 
	uart_send(UART0, &max_cli, 1);
	uart_send_until(UART0, "\r\n", 0);
	
	return check_strings(500, 2, "OK\r\n", "ERROR\r\n");
}

uint8_t esp_create_server(uint16_t a_port)
{
	uint8_t port_str[7];
	itoa(a_port, port_str, 10);
	uart_send_until(UART0, "AT+CIPSERVER=1,", 0);
	uart_send_until(UART0, port_str, 0);
	uart_send_until(UART0, "\r\n", 0);
	
	return check_strings(500, 2, "OK\r\n", "ERROR\r\n");
}


//must be used only after interring the server mode
uint8_t esp_conn_timeout(uint16_t a_minutes)
{
	uint8_t minutes_str[7];
	uart_send_until(UART0, "AT+CIPSTO=", 0);
	itoa(a_minutes, minutes_str, 10);
	uart_send_until(UART0, minutes_str, 0);
	uart_send_until(UART0, "\r\n", 0);
	return check_strings(500, 2, "OK\r\n", "ERROR\r\n");
}



#if 0
uint8_t esp_mult_conn(bool af_is_mult)
{
	
	if (af_is_mult == true)
	{
		uart_send_until(UART0, "AT+CIPMUX=1\r\n", 0);
	}
	else
	{
		uart_send_until(UART0, "AT+CIPMUX=0\r\n" , 0);
	}
	
	return check_strings(500, 2, "OK\r\n", "ERROR\r\n");
	
}

uint8_t esp_transm_mode(uint8_t a_transmission_mode)
{
	
	if (a_transmission_mode == ESP_NORMAL_MODE)
	{
		uart_send_until(UART0, "AT+CIPMODE=0\r\n", 0);
	}
	else if (a_transmission_mode == ESP_PASSTHROUGH_MODE)
	{
		uart_send_until(UART0, "AT+CIPMODE=1\r\n", 0);
	}
	
	return check_strings(500, 2, "OK\r\n", "ERROR\r\n");
	
}
#endif

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

#if 0
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
#endif

#if 0
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
#endif









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