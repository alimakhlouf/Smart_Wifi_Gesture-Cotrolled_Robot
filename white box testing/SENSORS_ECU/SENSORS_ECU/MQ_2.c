

/* ----------------------------------------------------------------------------
[FILE NAME]:    MQ_2.c

[AUTHOR]:       Ali MAkhlouf

[DATE CREATED]: 1/3/2015 11:28:01 PM

[DESCRIPTION]:   a library C file for communicating and reading sensor
				 values from a MQ2 sensor
-----------------------------------------------------------------------------*/
#include "MQ_2.h"

//it's the internal resistance of the MQ2 sensor in clean air
static float mq2_r0;

/*****************************  MQGetGasPercentage **********************************
Input:   rs_ro_ratio - Rs divided by Ro
         gas_id      - target gas type
Output:  ppm of the target gas
Remarks: This function passes different curves to the MQGetPercentage function which 
         calculates the ppm (parts per million) of the target gas.
************************************************************************************/ 

/* ----------------------------------------------------------------------------
[Function Name] : mq2_Calibration
[Description] : This function is responsible for getting calibration to get the
	internal resistance of the MQ2 sensor(R0) in clean air.

[Returns] : this return indicates the value of the resistance R0 at clean air
-------------------------------------------------------------------------------*/
static float mq2_Calibration ();

/* ----------------------------------------------------------------------------
[Function Name] : mq2_get_res
[Description] : This function is responsible for getting calibration to get the
	current internal resistance of the MQ2 sensor(R0).

[Returns] : this return indicates the current value of the resistance R0
-------------------------------------------------------------------------------*/
static float mq2_get_res();


void mq2_init()
{
	adc_init();
	mq2_r0 = mq2_Calibration();
}

unint16_t mq2_get_result()
{
	float ratio = mq2_get_res();
	
	return (unint16_t)(pow(10, (((log10((ratio / mq2_r0)) - SMOKE_Y) /
		                            SMOKE_SLOPE) + SMOKE_X)));
}

static float mq2_Calibration()
{
	char arr[10];
	
	unsigned char i;
	float val = 0;
	unint16_t adc_result;
	
	//adc_result = ADC_read(MQ2_PIN);
	//val = MQ2_CALC_R_VALUE(adc_result);
	
	for (uint16_t i = 0; i < CALIBARAION_SAMPLE_TIMES; i++ ) 
	{   
		//take multiple samples
		//it derives the resistance Rs from the analog read
		
		adc_result = adc_read(MQ2_PIN);
		//applying the voltage divider formula to get R value
		val += MQ2_CALC_R_VALUE(adc_result);
		delay_msec(CALIBRATION_SAMPLE_INTERVAL);
	}
	//calculate the average value
	val = val / CALIBARAION_SAMPLE_TIMES;
		
	/*itoa((int)(val), arr, 10);
	
	UART_sendStrByMark("val value is", 0);
	UART_sendStrByMark(arr, 0);
	UART_sendChr(13);
	UART_sendChr(10);*/


	
	
	val = val / RO_CLEAN_AIR_FACTOR;//divided by CLEAN_AIR_FACTOR result to R0
	//according to the chart in the datasheet
	/*itoa((int)(val), arr, 10);
	UART_sendStrByMark("val  after is:", 0);
	UART_sendStrByMark(arr, 0);
	UART_sendChr(13);
	UART_sendChr(10);*/

	return val;
}

//-----------------------------------------------------
//reads the analog value then derives the Resistance value
static float mq2_get_res()
{
	char arr[10];
	unsigned char i;
	float rs = 0;
	unint16_t adc_result;
	
	//adc_result = ADC_read(MQ2_PIN);
	//rs = MQ2_CALC_R_VALUE(adc_result);
	
	
	for (i = 0; i < READ_SAMPLE_TIMES; i++ )
	{
		adc_result = adc_read(MQ2_PIN);
		rs += MQ2_CALC_R_VALUE(adc_result);
		delay_msec(READ_SAMPLE_INTERVAL);
	}
	
	rs /= READ_SAMPLE_TIMES;
	
	//itoa((int)(rs), arr, 10);
	
	/*sprintf(arr,"%.3f",rs);
	
	UART_sendStrByMark("RS value is", 0);
	UART_sendStrByMark(arr, 0);
	UART_sendChr(13);
	UART_sendChr(10);
	
	itoa(adc_result, arr, 10);
	UART_sendStrByMark("V:  ", 0);
	UART_sendStrByMark(arr, 0);
	UART_sendChr(13);
	UART_sendChr(10);
*/
	
	return rs;
}



static uint16_t mq2_GetPercentage(float ratio, float m, float y, float x)
{
#if 0
	char arr[10];
	double temp;
	temp = log10(ratio);
		/*itoa((int)(temp), arr, 10);
		
		UART_sendStrByMark("log10 value is", 0);
		UART_sendStrByMark(arr, 0);
		UART_sendChr(13);
		UART_sendChr(10);*/
		
	temp = (((temp - y) / m) + x);
	
			/*itoa((int)(temp), arr, 10);
			
			UART_sendStrByMark("log10 form", 0);
			UART_sendStrByMark(arr, 0);
			UART_sendChr(13);
			UART_sendChr(10);*/
			
		double power = pow(10, temp);
		
		/*itoa((int)(power), arr, 10);
					
		UART_sendStrByMark("POWER:", 0);
		UART_sendStrByMark(arr, 0);
		UART_sendChr(13);
		UART_sendChr(10);*/
#endif
				
		
	return (unint16_t)(pow(10, (((log10(ratio) - y) / m) + x)));
}


//the function that gets the PPM value of various gases  from the RS/RO ratio
/*unsigned int MQ2_GetGasPercentage(float y, char gas_id)
{
	if (gas_id == GAS_SMOKE)
	{
		return MQ2_GetPercentage(y, SMOKE_SLOPE, SMOKE_B);
	}
	else
	{
		return 0;
	}

}*/
