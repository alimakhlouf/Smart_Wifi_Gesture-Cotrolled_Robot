/*
 * PWM prog.c
 *
 * Created: 6/14/2019 05:26:28 م
 * Author : hppp
 */ 

#include "micro_config.h"
#include "pwm1.h"
#include "dio_api.h"
#include "delay_api.h"


int main(void)
{
	pwm_init(PWM0 | PWM1 | PWM2 | PWM3, 1000);
	
	pwm_start(PWM0 | PWM1 | PWM2 | PWM3);
	
	pwm_setDuty(PWM0 | PWM1 | PWM2 | PWM3, 30);
	
	
	delay_msec(3000);
	
	for (;;)
	{
		for (int j = 0; j < 3; j++)
		{
			
			for (int i = 0; i < 100; i += 5)
			{
				pwm_setDuty(PWM0, i);
				pwm_setDuty(PWM1 | PWM3, 100 - i);
				pwm_setDuty(PWM2, i / 2);
				_delay_ms(20);
			}
			
		}
		
		pwm_stop(PWM0);
		delay_msec(1000);
		pwm_stop(PWM1);
		delay_msec(1000);
		pwm_stop(PWM2);
		delay_msec(1000);
		pwm_stop(PWM3);
		delay_msec(1000);		
	}
	
}