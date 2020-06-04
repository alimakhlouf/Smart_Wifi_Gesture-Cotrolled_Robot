#ifndef  DHT11_library
#define  DHT11_library

#include "micro_config.h"
#include "std_types.h"
#include "dio_api.h"
#include "delay_api.h"

/***************************choose port and pin***********/
#define DHT_PIN  0
/*********************************************************/


//available errors
#define DHT11_NO_LOW_80US_RESPONSE 255
#define DHT11_NO_HIGH_80US_RESPONSE 254
#define DHT11_NO_FIRST_DATA 253
#define DHT11_DATA_LOW_ERROR 252
#define DHT11_DATA_HIGH_ERROR 251

void DHT_init(); //
uint8_t DHT11_read(char *arr); //sends an array of 5 cells, reads the module and puts the values in the array

	
#endif
