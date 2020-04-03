#ifndef MQ2_library
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
#define NO_OF_CALIBARAIONS (30)    // NO OF TIMES TAKEN IN A CALIBRATION
#define CALIBARAION_SAMPLE_TIMES (10)     //define how many samples you are going to take in the calibration phase
#define CALIBRATION_SAMPLE_INTERVAL (500)   //define the time interal(in milisecond) between each samples in the
//cablibration phase
#define READ_SAMPLE_INTERVAL (10)    //define the time interval (in milliseconds) between each samples in
#define READ_SAMPLE_TIMES (50)     //define how many samples you are going to take in normal operation
//normal operator


//-------------------------------------------------------------------------------------
//constant #defines
#define         RO_CLEAN_AIR_FACTOR          (9.83)  //RO_CLEAR_AIR_FACTOR=(Sensor resistance in clean air)/RO,
//which is derived from the chart in datasheet


//#define         GAS_LPG                    (0)
//#define         GAS_CO                     (1)
#define         GAS_SMOKE                    (2)



//Y=MX+N{2.3,0.53
#define			SMOKE_SLOPE					-0.4434025702  //the log slope
#define			SMOKE_Y						0.53	  // the intercept value(b)
#define		    SMOKE_X						2.3                   
//------------------------------------------------------------------------------------





//void MQ_init(char pin);


#define MQ2_CALC_R_VALUE(adc_result) ((1023.0 - ((float)adc_result)) / ((float)adc_result))

//it derives the resistance Rs from the analog read
//float MQ2_ResistanceCalculation(unsigned int raw_adc);




//  reads the analog value then derives the rs value
//float MQ2_get_res();


unint16_t MQ2_get_result();


//the function that gets the PPM value of various gases  from the RS/RO ratio
//unsigned int MQ2_GetGasPercentage(float y, char gas_id);

void MQ2_init();

#endif