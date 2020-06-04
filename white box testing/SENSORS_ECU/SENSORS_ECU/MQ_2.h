/* --------------------------------------------------------------------------
[FILE NAME]:     MQ2.h

[AUTHOR]:        Ali MAkhlouf

[DATE CREATED]:  1/3/2015 11:28:01 PM

[DESCRIPTION]:   a library header file for communicating and reading sensor
				 values from a MQ2 sensor
-----------------------------------------------------------------------------*/
#ifndef MQ2_ibrary
#define MQ2_library

#include "ADC_lib.h"
#include "micro_config.h"
#include "std_types.h"
#include <math.h>
#include "stdlib.h"
#include "uart_api.h"
#include "delay_api.h"


/******************choose the adc channel(pin)*********************/
#define MQ2_PIN  0
/******************************************************************/


#define	RL_VALUE  (10)  //define the load resistance on the board, in kilo ohms

#define NO_OF_CALIBARAIONS (30)// NO OF TIMES TAKEN IN A CALIBRATION   
//define how many samples you are going to take in the calibration phase 
#define CALIBARAION_SAMPLE_TIMES (10)     
//define the time interval(in msec) between each two samples 
#define CALIBRATION_SAMPLE_INTERVAL (500)   
//define the time interval (in msec) between each samples in
#define READ_SAMPLE_INTERVAL (10)
//define how many samples you are going to take in normal operation
#define READ_SAMPLE_TIMES (50) 

//-------------------------------------------------------------------------------------
//Constants
/*RO_CLEAR_AIR_FACTOR=(Sensor resistance in clean air)/RO,
//which is derived from the chart in datasheet*/
#define         RO_CLEAN_AIR_FACTOR          (9.83)
  
//numbering different types of gases in the sensor
//#define         GAS_LPG                    (0)
//#define         GAS_CO                     (1)
#define         GAS_SMOKE                    (2)



//Y = MX + N{2.3,0.53
#define			SMOKE_SLOPE					-0.4434025702  //the log slope
#define			SMOKE_Y						0.53	  // the intercept value(b)
#define		    SMOKE_X						2.3                   
//------------------------------------------------------------------------------------

#define MQ2_CALC_R_VALUE(adc_result) ((1023.0 - ((float)adc_result)) / ((float)adc_result))


/* ----------------------------------------------------------------------------
[Function Name] : mq2_get_result
[Description] : This function is responsible for calculating the ppm of the 
	target gas

[NOTE]: it consist of 5 bytes
[Returns] : the final result of the sensor in PPM
-----------------------------------------------------------------------------*/
unint16_t mq2_get_result ();


//the function that gets the PPM value of various gases  from the RS/RO ratio
//unsigned int mq2_GetGasPercentage(float y, char gas_id);
/* ----------------------------------------------------------------------------
[Function Name] :  mq2_init
[Description] : This function is responsible for initializing MQ2 sensor
	it initializes the ADC peripheral and performs calibration to get the internal
	resistance of the MQ2 sensor(R0) in clean air.
-------------------------------------------------------------------------------*/
void mq2_init ();

#endif