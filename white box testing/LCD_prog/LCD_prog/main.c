/*
 * LCD_prog.c
 *
 * Created: 5/1/2019 05:36:43 م
 * Author : hppp
 */ 

#include "micro_config.h"
#include "ser_lcd.h"
#include "dio_api.h"




int main(void)
{
	_delay_ms(1000);
	LCD_init();
	
	

	LCD_gotoxy(1, 0);
	LCD_str("hello ALI MAKHLO");
	
    /* Replace with your application code */
    while (1) 
    {
		
    }
}