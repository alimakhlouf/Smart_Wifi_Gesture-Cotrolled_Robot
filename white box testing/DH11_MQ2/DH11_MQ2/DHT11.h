#ifndef  DHT11_library
#define  DHT11_library

#include "micro_config.h"
#include "std_types.h"

/***************************choose port and pin***********/
#define HDTDIR DDRB
#define HDTPORT PORTB
#define HDTPIN_STATUS PINB
#define HDTPIN  0
/*********************************************************/



void DHT_init(); //
void DHT11_read(char *arr); //sends an array of 5 cells , reads the module and puts the values in the array

	
#endif
