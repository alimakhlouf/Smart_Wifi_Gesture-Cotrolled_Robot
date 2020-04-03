#ifndef  DHT11_library
#define  DHT11_library

#include "micro_config.h"
#include "std_types.h"
#include "dio_api.h"
#include "delay_api.h"

/***************************choose port and pin***********/
#define DHT_PIN  0
/*********************************************************/



void DHT_init(); //
void DHT11_read(char *arr); //sends an array of 5 cells , reads the module and puts the values in the array

	
#endif
