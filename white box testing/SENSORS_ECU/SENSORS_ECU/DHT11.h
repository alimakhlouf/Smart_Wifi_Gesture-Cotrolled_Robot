/* ----------------------------------------------------------------------------
[FILE NAME]:     DHT11.h

[AUTHOR]:        Ali MAkhlouf

[DATE CREATED]:  1/3/2015 11:28:01 PM

[DESCRIPTION]:   a library header file for communicating and reading sensor
				 values from a DHT11 sensor
-----------------------------------------------------------------------------*/

#ifndef  DHT11_library
#define  DHT11_library

#include "micro_config.h"
#include "std_types.h"
#include "dio_api.h"
#include "delay_api.h"



/***************************choose port and pin***********/
#define DHT_PIN  0
/*********************************************************/
/* ----------------------------------------------------------------------------
[Function Name] : dht11_init
[Description] : This function is responsible for initializing DHT11 sensor
-------------------------------------------------------------------------------*/
void dht11_init (); //

/* ----------------------------------------------------------------------------
[Function Name] : dht11_read
[Description] : This function is responsible for getting the sensor readings
[Args] :
[out] -char *arr
	this argument indicates the location where the reading of the sensor will be 
	stored
[NOTR]: it consist of 5 bytes
-------------------------------------------------------------------------------*/
void dht11_read (char *arr);

	
#endif