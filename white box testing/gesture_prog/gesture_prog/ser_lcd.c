/* --------------------------------------------------------------------------
[FILE NAME]:     ser_lcd.c

[AUTHOR]:        Ali MAkhlouf

[DATE CREATED]:  5/7/2019 03:24:39 PM

[DESCRIPTION]:   a library C file for controlling LCD screen using sipo
shift register 74HC595
----------------------------------------------------------------------------*/


#include "ser_lcd.h"

uint8_t lcd_reg = 0; 

void ser_lcd_init()
{
	//initialize shift register
	spc_init();
	
	_delay_ms(18);
	//lcd configuration commands
	ser_lcd_send(COMMAND, CURSOR_SCREEN_INIT);
	ser_lcd_send(COMMAND, TWO_LINE_LCD_FOUR_BIT_MODE);
	ser_lcd_send(COMMAND, CURSOR_ON);
	ser_lcd_send(COMMAND, CLEAR_SCREEN);//clear screen and move cursor to zero 
	ser_lcd_send(COMMAND, INC_CURSOR_RIGHT_NO_SC_SHIFTING);
}

void ser_lcd_send(uint8_t a_type, uint8_t a_byte)
{
	//putting the upper bits on the data pins
	uint8_t data_pins= MOVE_UPPER_4BITS_TO_DATAPINS(a_byte);
	//indicates type (command or data)
	lcd_reg =  (a_type? lcd_reg | (1 << RS) : lcd_reg & ~(1 << RS)); 
	spc_send(lcd_reg, 8);
	_delay_us(1);// delay for processing Tas = 50ns
	lcd_reg |= 1 << E;
	spc_send(lcd_reg, 8);
	_delay_us(1); // delay for processing Tpw - Tdws = 190ns
	//put the highest 4 bits of the required command to the data bus D4 --> D7
	//clear the data pins and OR them with the upper part of the data
	lcd_reg = (lcd_reg & ~(DATA_PINS_HIGH)) | data_pins ;
	spc_send(lcd_reg, 8);
	_delay_us(1);/* delay for processing Tdsw = 100ns */
	lcd_reg &= ~(1 << E);
	spc_send(lcd_reg, 8);
	
	_delay_us(1);/* delay for processing Th = 13ns */
	
	// adjusting the lower bits to the data pins
	data_pins= MOVE_LOWER_4BITS_TO_DATAPINS(a_byte);
	lcd_reg |= 1 << E;
	spc_send(lcd_reg, 8);
	_delay_us(1); /* delay for processing Tpw - Tdws = 190ns */
	//put the lowest 4 bits of the required command to the data bus D4 --> D7
	//clear the data pins and OR them with the lower part of the data
	lcd_reg = (lcd_reg & ~(DATA_PINS_HIGH)) | data_pins ;
	spc_send(lcd_reg, 8);
	_delay_us(1);//delay for processing Tdsw = 100ns
	lcd_reg &= ~(1 << E);
	spc_send(lcd_reg, 8);
	
	//those 2 commands require 1.64 msec delay for executing
	if (COMMAND == a_type)
	{
		
		if ((CLEAR_SCREEN == a_byte) ||(CURSOR_SCREEN_INIT == a_byte))
		{
			_delay_ms(2);
		}
		else // other commands require 40 us delay
		{
			_delay_us(40);
		}
		
	}
	else //if data operation
	{
		//delay for processing = 100ns
		_delay_us(1);
	}
	
}


void ser_lcd_gotoxy (uint16_t a_x , uint16_t a_y)
{
	uint8_t Address = 0;

	/* first of all calculate the required address */
	switch(a_y)
	{
		case 0:
				Address = a_x;
				break;
		case 1:
				Address = a_x | 0x40;
				break;
		case 2:
				Address = a_x | 0x10;
				break;
		case 3:
				Address = a_x | 0x50;
				break;
		default:
				break;
	}
	/* to write to a specific address in the LCD
	 * we need to apply the corresponding command 0b10000000+Address */
	ser_lcd_send(COMMAND, Address | 0x80);
}

void ser_lcd_str_add(const char * ap_string_ptr , uint8_t a_x , uint8_t a_y)
{   
	uint8_t i = 0;
	ser_lcd_gotoxy (a_x, a_y);
	
	while (ap_string_ptr[i] != 0)
	{
		ser_lcd_send(DATA, ap_string_ptr[i]);
		i++;
	}
	
}

void ser_lcd_str(const char * ap_string_ptr)
{
	uint8_t i = 0;
	
	while (ap_string_ptr[i] != 0)
	{
		ser_lcd_send(DATA, ap_string_ptr[i]);
		i++;
	}
	
}

void  ser_lcd_chr_add(uint8_t a_character,uint8_t a_x, uint8_t a_y)
{
	ser_lcd_gotoxy(a_x, a_y);
	ser_lcd_send(DATA, a_character);
}

void ser_lcd_chr(uint8_t a_character)
{
	ser_lcd_send(DATA, a_character);
}

void ser_lcd_shift_cursor(uint8_t a_direction ,uint8_t a_no_of_steps)
{
	
	while (a_no_of_steps)  //avoid using any mathematics inside the test to increase it's speed
	{
		ser_lcd_send(COMMAND, 0x10 | (a_direction << 2));
		a_no_of_steps-- ;
	}
	
}

void ser_lcd_shift_screen(uint8_t a_direction ,uint8_t a_no_of_steps)
{
	
	while(a_no_of_steps)  //avoid using any mathematics inside the test to increase it's speed
	{
		ser_lcd_send(COMMAND, 0x18 |(a_direction << 2));
		a_no_of_steps-- ;
	}
	
}

void ser_lcd_itoa(uint16_t a_number, uint8_t a_x, uint8_t a_y)
{
	char buff[7]; /* String to hold the ascii result */
	
	itoa(a_number, buff, 10);
	ser_lcd_gotoxy (a_x, a_y);
	
	uint8_t i = 0;
	while (buff[i] != 0)   
	{	
		ser_lcd_send(DATA, buff[i]);
		i++;
	}
	 
}
