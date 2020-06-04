/* --------------------------------------------------------------------------
[FILE NAME]:     ser_lcd.h

[AUTHOR]:        Ali MAkhlouf

[DATE CREATED]:  5/7/2019 03:25:00 PM

[DESCRIPTION]:   a library header file for controlling LCD screen using sipo
	shift register 74HC595
----------------------------------------------------------------------------*/

#ifndef SER_LCD_H_
#define SER_LCD_H_



#ifndef lcd_4bit_library
#define lcd_4bit_library

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"
#include "spc.h"


//pins locations on the sipo register 74HC595 
#define RS 6
#define E  5
#define D4 1
#define D5 2
#define D6 3
#define D7 4

#define DATA_PINS_HIGH ((1 << D4) | (1 << D5) | (1 << D6) | (1 << D7))

#define COMMAND 0
#define DATA 1

#define RIGHT 1
#define LEFT 0

//LCD commands 
#define CLEAR_SCREEN 0x01
#define CURSOR_SCREEN_INIT 0x02

/*
0 0 0 1 DL N F x x
N=1:2 lines
DL=0: 4-bit data
F=0:font 5x8 dots font*/
#define TWO_LINE_LCD_FOUR_BIT_MODE 0x28  

/*
0 0 0 1 DL N F x x 
N=1:2 lines
DL=1: 8-bit data
F=0:font 5x8 dots font
*/
#define TWO_LINE_LCD_EIGHT_BIT_MODE 0x38 
#define CURSOR_OFF 0x0C
#define CURSOR_ON 0x0E
#define SET_CURSOR_LOCATION 0x80
#define CURSOR_CONFIG 0xc0

/*0 0 0 0 0 1 I/D S --> 
//I/D=1 increment cursor direction(move right)
--S=0: no screen shift shifting*/
#define INC_CURSOR_RIGHT_NO_SC_SHIFTING 0x06 


/* ----------------------------------------------------------------------------
[Macro Name] : MOVE_UPPER_4BITS_TO_DATAPINS
[Description] : This macro is responsible for putting the upper nibble(4 bits) 
	of the byte(data or command) to the LCD_pins of the MCU in order to send 
	them to the LCD

[Args] :
[in] -uint8_t data_command                          
	this ARG is responsible for holding the byte(data_command) that will be sent 
	on the LCD screen
------------------------------------------------------------------------------*/
#define MOVE_UPPER_4BITS_TO_DATAPINS(data_command) \
	(((((data_command) & 0b10000000) >> 7) << D7)| \
    ((((data_command) & 0b01000000) >> 6) << D6) | \
	((((data_command) & 0b00100000) >> 5) << D5) | \
	((((data_command) & 0b00010000) >> 4) << D4))\



/* ----------------------------------------------------------------------------
[macro Name] : MOVE_LOWER_4BITS_TO_DATAPINS
[Description] : This macro is responsible for putting the lower nibble(4 bits) of
	the byte(data or command) to the LCD_pins of the MCU in order to send them to
	the LCD

[Args] :
[in] -uint8_t data_command
	this ARG is responsible for holding the byte(data_command) that will be sent  
	on the LCD screen
-----------------------------------------------------------------------------*/
#define MOVE_LOWER_4BITS_TO_DATAPINS(data_command) \
	(((((data_command) & 0b00001000) >> 3) << D7)|\
	((((data_command) & 0b00000100) >> 2) << D6) |\
	((((data_command) & 0b00000010) >> 1) << D5) |\
	(((data_command)  & 0b00000001) << D4))



/* ---------------------------------------------------------------------------
[Function Name] : LCD_init
[Description] : This function is responsible for doing these steps:
				1-setting the LCD pins high as well as the control pins
				2-initializing it's cursor and screen and setting it in 4-bit 
				 mode
				3-show the cursor
-----------------------------------------------------------------------------*/
void ser_lcd_init ();

/* ----------------------------------------------------------------------------
[Function Name] : ser_lcd_send
[Description] : This function is responsible for sending a command or data byte
	to the LCD in 4-bit mode
[Args] :
[in] -uint8_t a_type
	this ARG shall indicate the type(command or data) 
[in] -uint8_t a_byte
	this ARG is responsible for holding the command or data that will be 
	received and processed by the LCD
------------------------------------------------------------------------------*/
void ser_lcd_send (uint8_t a_type, uint8_t a_byte);


/* -----------------------------------------------------------------------------
[Function Name] : ser_lcd_gotoxy (goto x, y coordinates in the lcd_screen)
[Description] : This function is responsible for putting the cursor in particular
	coordinate in the LCD screen
[Args] :
[in] -uint8_t a_x
	this ARG is responsible for holding the column number of the place which 
	pointer will be set at
[in] -uint8_t a_y
	this ARG is responsible for holding the row number of the place which 
	pointer will be set at
------------------------------------------------------------------------------*/
void ser_lcd_gotoxy (uint16_t a_x ,uint16_t a_y);

/* -----------------------------------------------------------------------------
[Function Name] : ser_lcd_str
[Description] : This function is responsible for displaying a string starting 
	from the current place of the cursor
[Args] :
[in] -const char * ap_string_ptr
	this ARG is responsible for pointing at the first character of the string 
	that will be displayed
------------------------------------------------------------------------------*/
void ser_lcd_str(const char * ap_string_ptr);

/* -----------------------------------------------------------------------------
[Function Name] : ser_lcd_str_add
[Description] : This function is responsible for displaying a string at a 
	location we specify
[Args] :
[in] -const char * ap_string_ptr
	this ARG is responsible for pointing at the first character of the string that
	will be displayed
[in] -uint8_t a_x
	this ARG is responsible for holding the column number of the place which the 
	cursor will be set at
[in] -uint8_t a_y
	this ARG is responsible for holding the row number of the place which the 
	cursor will be set at
------------------------------------------------------------------------------*/
void ser_lcd_str_add (const char * ap_string_ptr , uint8_t a_x , uint8_t a_y);


/* -----------------------------------------------------------------------------
[Function Name] : ser_lcd_chr
[Description] : This function is responsible for displaying a character at the 
	current place of the cursor
[Args] :
[in] -uint8_t character
	this ARG is responsible for holding the character that will be displayed
------------------------------------------------------------------------------*/
void ser_lcd_chr (uint8_t character);


/* -----------------------------------------------------------------------------
[Function Name] : ser_lcd_chr_add
[Description] : This function is responsible for displaying a character at a 
	location we specify
[Args] :
[in] -uint8_t character
this ARG is responsible for holding the character that will be displayed
[in] -uint8_t x
	this ARG is responsible for holding the column number of the place which the 
	cursor will be set at
[in] -uint8_t y
	this ARG is responsible for holding the row number of the place which the 
	cursor will be set at
------------------------------------------------------------------------------*/
void ser_lcd_chr_add (uint8_t character,uint8_t x,uint8_t y );


/* -----------------------------------------------------------------------------
[Function Name] : ser_lcd_shift_cursor
[Description] : This function is responsible for moving the cursor from the 
	current place a specified number of steps right or left
[Args] :
[in] -uint8_t a_direction
	this ARG is responsible for informing the function which direction the cursor 
	will be shifted ,you write either RIGHT(defined as 1) or LEFT(defined as 0)
[in] -uint8_t a_no_of_steps
	this ARG is responsible for informing the function how many steps the cursor will
	move in the specified direction
------------------------------------------------------------------------------*/
void ser_lcd_shift_cursor (uint8_t a_direction ,uint8_t a_no_of_steps);

/* -----------------------------------------------------------------------------
[Function Name] : ser_lcd_shift_screen
[Description] : This function is responsible for moving the displayed characters
	 on the screen from the current place a specified number of steps right or left
[Args] :
[in] -uint8_t a_direction
	this ARG is responsible for informing the function which direction the screen 
	will be shifted ,you write either RIGHT(defined as 1) or LEFT(defined as 0)
[in] -uint8_t a_no_of_steps
	this ARG is responsible for informing the function how many steps the screen
	will move in the specified direction
------------------------------------------------------------------------------*/
void ser_lcd_shift_screen (uint8_t a_direction ,uint8_t a_no_of_steps);

/* -----------------------------------------------------------------------------
[Function Name] : LCD_itoa(integer to ASCII)
[Description] : This function is responsible 2 things:
 	 	 	 	1-converting a number into it's corresponding ASCII representation
				using standard itoa function
				2-displaying the converted number in the LCD at a location we 
				specify
[Args] :
[in] -uint8_t number
	this ARG is responsible for holding the character that will be displayed
[in] -uint8_t a_x
	this ARG is responsible for holding the column number of the place which the cursor will be set at
[in] -uint8_t a_y
	this ARG is responsible for holding the row number of the place which the cursor will be set at
------------------------------------------------------------------------------*/
void ser_lcd_itoa (uint16_t a_number, uint8_t a_x, uint8_t a_y);


#endif


#endif /* SER_LCD_H_ */