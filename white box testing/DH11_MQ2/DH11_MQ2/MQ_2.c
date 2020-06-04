#include "MQ_2.h"

static unsigned int MQ2_GetPercentage(float , float ,float b, float);

// it calibrates the sensor to get the value of RO resistor
static float MQ2_Calibration ();

//  reads the analog value then derives the rs value
static float MQ2_get_res();

// it calibrates the sensor to get the value of RO resistor
static float MQ2_Calibration()
{
	char arr[10];
	
	unsigned char i;
	float val = 0;
	unint16_t adc_result;
	
	_delay_ms(5000);
	adc_result = ADC_read(MQ2_PIN);
	itoa((int)(adc_result), arr, 10);
	UART_sendStrByMark(arr, 0);
	UART_sendStrByMark("\r\n", 0);
	val = MQ2_CALC_R_VALUE(adc_result);
	
	
	/*for (i = 0; i < CALIBARAION_SAMPLE_TIMES; i++ ) 
	{   
		//take multiple samples
		//it derives the resistance Rs from the analog read
		//applying the voltage divider formula to get the result
		adc_result = ADC_read(MQ2_PIN);
		val += MQ2_CALC_R_VALUE(adc_result);
		_delay_ms(CALIBRATION_SAMPLE_INTERVAL);
	}
	//calculate the average value
	val = val / CALIBARAION_SAMPLE_TIMES;*/
	
	
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
//reads the analog value then derives the Resistance value value
static float MQ2_get_res()
{
	char arr[10];
	unsigned char i;
	float rs = 0;
	unint16_t adc_result;
	
	adc_result = ADC_read(MQ2_PIN);
	itoa((int)(adc_result), arr, 10);
	UART_sendStrByMark(arr, 0);
	UART_sendStrByMark("\r\n", 0);
	rs = MQ2_CALC_R_VALUE(adc_result);
	
	
	/*for (i = 0; i < READ_SAMPLE_TIMES; i++ )
	{
		adc_result = ADC_read(MQ2_PIN);
		rs += MQ2_CALC_R_VALUE(adc_result);
		_delay_ms(READ_SAMPLE_INTERVAL);
	}
	
	rs /= READ_SAMPLE_TIMES;*/
	
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


void MQ2_init()
{
	ADC_init();
	MQ2_R0 = MQ2_Calibration();
}
/*****************************  MQGetGasPercentage **********************************
Input:   rs_ro_ratio - Rs divided by Ro
         gas_id      - target gas type
Output:  ppm of the target gas
Remarks: This function passes different curves to the MQGetPercentage function which 
         calculates the ppm (parts per million) of the target gas.
************************************************************************************/ 



static unsigned int MQ2_GetPercentage(float ratio, float m, float y, float x)
{
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
				
		
	return (unint16_t)(pow(10,( ((log10(ratio) - y) / m) + x)));
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

unint16_t MQ2_get_result()
{
	unint16_t ppm_result = MQ2_GetPercentage((MQ2_get_res(MQ2_PIN) / MQ2_R0), SMOKE_SLOPE, SMOKE_Y, SMOKE_X); 
	
	return ppm_result;	
}

