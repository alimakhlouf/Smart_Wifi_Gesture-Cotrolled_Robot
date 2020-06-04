/* ---------------------------------------------------------------------------------------------------
[FILE NAME]: lcd_4bit.c
[AUTHOR(S)]: Ali Makhlouf
[DATE CREATED]: 1/6/2016
[DESCRIPTION]: library c file for LCD with maximum 4 raws
--------------------------------------------------------------------------------------------------- */


#include "lcd_4bit.h"


void LCD_init()
{
	_delay_ms(15);
	LCD_CTR_PORT_DIR  |= (1<<E)|(1<<RS) ;
	LCD_DATA_PORT_DIR |= DATA_PINS_HIGH ;
	LCD_commandSend (CURSOR_SCREEN_INIT);
	LCD_commandSend (TWO_LINE_LCD_Four_BIT_MODE);
	LCD_commandSend (CURSOR_ON);
	LCD_commandSend (CLEAR_SCREEN);  //clear screen and move cursor to zero
}


void LCD_commandSend(uint8_t a_command)
{
	// adjusting the upper bits to the data pins
	uint8_t data_pins = MOVE_UPPER_4BITS_TO_DATAPINS(a_command);
	LCD_CTR_PORT &= ~(1<<RS);
	_delay_us(1); /* delay for processing Tas = 50ns */
	LCD_CTR_PORT |= 1<<E;
	_delay_us(1); /* delay for processing Tpw - Tdws = 190ns */
	/* out the highest 4 bits of the required command to the data bus D4 --> D7 */
	LCD_DATA_PORT=(LCD_DATA_PORT & ~(DATA_PINS_HIGH)) | data_pins ;  //clear the data pins and OR  them with the upper part of the data
	_delay_us(1);/* delay for processing Tdsw = 100ns */
	LCD_CTR_PORT &= ~(1<<E);

	_delay_us(1);/* delay for processing Th = 13ns */

	// adjusting the lower bits to the data pins
	data_pins= MOVE_LOWER_4BITS_TO_DATAPINS(a_command);
	LCD_CTR_PORT |= 1<<E;
	_delay_us(1); /* delay for processing Tpw - Tdws = 190ns */
	/* out the lowest 4 bits of the required command to the data bus D4 --> D7 */
	LCD_DATA_PORT=(LCD_DATA_PORT & ~(DATA_PINS_HIGH)) | data_pins ;  ////clear the data pins and OR them with the lower part of the data
	_delay_us(1);/* delay for processing Tdsw = 100ns */
	LCD_CTR_PORT &= ~(1<<E);
	if ((a_command == CLEAR_SCREEN) ||(a_command == CURSOR_SCREEN_INIT)) //those 2 commands require 1.64 msec delay for executing
	{
		_delay_us(1640);
	}
	else // other commands require 40 us delay
	{
		_delay_us(40);
	}
}



void LCD_dataSend( uint8_t data )
{
	// adjusting the upper bits to the data pins
	uint8_t data_pins= MOVE_UPPER_4BITS_TO_DATAPINS(data);
	LCD_CTR_PORT|= (1<<RS);
	_delay_us(1);/* delay for processing Tas = 50ns */
	LCD_CTR_PORT |= 1<<E;
	_delay_us(1); /* delay for processing Tpw - Tdws = 190ns */
	/* out the highest 4 bits of the required command to the data bus D4 --> D7 */
	LCD_DATA_PORT=(LCD_DATA_PORT & ~(DATA_PINS_HIGH)) | data_pins ;  //clear the data pins and OR  them with the upper part of the data
	_delay_us(1);/* delay for processing Tdsw = 100ns */
	LCD_CTR_PORT &= ~(1<<E);

	_delay_us(1);/* delay for processing Th = 13ns */

	// adjusting the lower bits to the data pins
	data_pins= MOVE_LOWER_4BITS_TO_DATAPINS(data);
	LCD_CTR_PORT |= 1<<E;
	_delay_us(1); /* delay for processing Tpw - Tdws = 190ns */
	/* out the lowest 4 bits of the required command to the data bus D4 --> D7 */
	LCD_DATA_PORT=(LCD_DATA_PORT & ~(DATA_PINS_HIGH)) | data_pins;  ////clear the data pins and OR them with the lower part of the data
	_delay_us(1);/* delay for processing Tdsw = 100ns */
	LCD_CTR_PORT &= ~(1<<E);
	_delay_us(40);/* delay for processing = 100ns */
}






void LCD_gotoxy (uint8_t x , uint8_t y)
{
	uint8_t Address = 0;

	/* first of all calculate the required address */
	switch(y)
	{
		case 0:
				Address=x;
				break;
		case 1:
				Address=x+0x40;
				break;
		case 2:
				Address=x+0x10;
				break;
		case 3:
				Address=x+0x50;
				break;
		default:
				break;
	}
	/* to write to a specific address in the LCD
	 * we need to apply the corresponding command 0b10000000+Address */
	LCD_commandSend (Address |0x80);
}




void LCD_strByLocation(const char *string_ptr , uint8_t x , uint8_t y)
{   uint8_t i=0;
	LCD_gotoxy (x,y);
	while (string_ptr[i] != 0)
	{
		LCD_dataSend(string_ptr[i]);
		i++;
	}
}

void LCD_str(const char *string_ptr)
{
	uint8_t i=0;
	while (string_ptr[i] != 0)
	{
		LCD_dataSend(string_ptr[i]);
		i++;
	}
}

void  LCD_chrByLocation (uint8_t character,uint8_t x,uint8_t y )
{
	LCD_gotoxy( x , y);
	LCD_dataSend(character);
}

void LCD_chr (uint8_t character)
{
	LCD_dataSend(character);
}

void LCD_shiftCursor (uint8_t direction ,uint8_t no_of_steps)
{
	while(no_of_steps)  //avoid using any mathematics inside the test to increase it's speed
	{
		LCD_commandSend(0x10 |(direction<<2));
		no_of_steps--;
	}
}



void LCD_shiftScreen(uint8_t direction ,uint8_t no_of_steps)
{
	while(no_of_steps)  //avoid using any mathematics inside the test to increase it's speed
	{
		LCD_commandSend(0x18 |(direction<<2));
		no_of_steps--;
	}
}



 void LCD_itoa(uint16_t number,uint8_t x,uint8_t y)
 {
	 char buff[16]; /* String to hold the ascii result */
	 uint8_t i=0;
	 itoa(number,buff,10);

	 LCD_gotoxy (x,y);
	 while (buff[i] != 0)   // from the lcd_str function
	 {	LCD_dataSend(buff[i]);
		i++;
	 }
 }
