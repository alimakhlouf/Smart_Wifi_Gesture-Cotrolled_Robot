/*
 * ser_lcd.h
 *
 * Created: 5/7/2019 03:25:00 م
 *  Author: hppp
 */ 


#ifndef SER_LCD_H_
#define SER_LCD_H_

// 
// 
// #ifndef lcd_4bit_library
// #define lcd_4bit_library

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"
#include "spc.h"


#define RS 6
#define E  5
#define D4 1
#define D5 2
#define D6 3
#define D7 4


#define DATA_PINS_HIGH ((1<<D4)|(1<<D5)|(1<<D6)|(1<<D7))

#define RIGHT 1
#define LEFT 0
#define CLEAR_SCREEN 0x01
#define CURSOR_SCREEN_INIT 0x02
#define TWO_LINE_LCD_Four_BIT_MODE 0x28  //0 0 0 1 DL N F x x --> N=1:2 lines ----DL=0: 4-bit data --F=0:font 5x8 dots font
#define TWO_LINE_LCD_Eight_BIT_MODE 0x38 //0 0 0 1 DL N F x x --> N=1:2 lines ----DL=1: 8-bit data --F=0:font 5x8 dots font
#define CURSOR_OFF 0x0C
#define CURSOR_ON 0x0E
#define SET_CURSOR_LOCATION 0x80
#define CURSOR_CONFIG 0xc0
#define INC_CURSOR_RIGHT_NO_SC_SHIFTING 0x06 //0 0 0 0 0 1 I/D S -->I/D=1 :increment cursor direction(moves right)--- S=0: no screen shift shifting


/* ---------------------------------------------------------------------------------------------------------------
[macro Name] : MOVE_UPPER_4BITS_TO_DATAPINS
[Description] : This macro is responsible for putting the upper nibble(4 bits) of the byte(data or command) to the LCD_pins of the MCU in order to send them to the LCD

[Args] :
[int] -uint8_t data_command
this ARG is responsible for holding the byte(data_command) that will be sent on the LCD screen
--------------------------------------------------------------------------------------------------------------*/
#define MOVE_UPPER_4BITS_TO_DATAPINS(data_command) ((((data_command) & 0b10000000)>>7)<<D7) |  ((((data_command) & 0b01000000)>>6)<<D6) | ((((data_command) & 0b00100000)>>5)<<D5) | ((((data_command) & 0b00010000)>>4)<<D4)


/* ---------------------------------------------------------------------------------------------------------------
[macro Name] : MOVE_LOWER_4BITS_TO_DATAPINS
[Description] : This macro is responsible for putting the lower nibble(4 bits) of the byte(data or command) to the LCD_pins of the MCU in order to send them to the LCD

[Args] :
[int] -uint8_t data_command
this ARG is responsible for holding the byte(data_command) that will be sent on the LCD screen
---------------------------------------------------------------------------------------------------------------*/
#define MOVE_LOWER_4BITS_TO_DATAPINS(data_command) ((((data_command) & 0b00001000)>>3)<<D7) |  ((((data_command) & 0b00000100)>>2)<<D6) | ((((data_command) & 0b00000010)>>1)<<D5) | (((data_command) & 0b00000001)<<D4)



/* ---------------------------------------------------------------------------------------------------------------
[Function Name] : LCD_init
[Description] : This function is responsible for initializing the LCD doing these steps:
				1-setting the LCD pins high as well as the control pins
				2-initializing it's cursor and screen and setting it in 4-bit mode
				3-show the cursor
---------------------------------------------------------------------------------------------------------------*/
void LCD_init();



/* ---------------------------------------------------------------------------------------------------------------
[Function Name] : LCD_commandSend
[Description] : This function is responsible for sending a command byte to the LCD in 4-bit mode(it sends the upper 4 bits first then the lower 4 bits)

[Args] :
[int] -uint8_t a_command
this ARG is responsible for holding the command that will be received and processed by the LCD
---------------------------------------------------------------------------------------------------------------*/
void LCD_commandSend(uint8_t a_command);


/* ---------------------------------------------------------------------------------------------------------------
[Function Name] : LCD_dataSend
[Description] : This function is responsible for sending a data byte(character) to the LCD in 4-bit mode to be display on it's screen(it sends the upper 4 bits first then the lower 4 bits)

[Args] :
[int] -uint8_t a_data
this ARG is responsible for holding the character that will be displayed on the LCD screen
---------------------------------------------------------------------------------------------------------------*/
void LCD_dataSend(uint8_t a_data );


/* ---------------------------------------------------------------------------------------------------------------
[Function Name] : LCD_gotoxy(goto x,y coordinates in the lcd_screen)
[Description] : This function is responsible for putting the cursor in particular coordinate in the LCD screen
[Args] :
[int] -uint8_t x
this ARG is responsible for holding the column number of the place which pointer will be set at
-uint8_t y
this ARG is responsible for holding the row number of the place which pointer will be set at
---------------------------------------------------------------------------------------------------------------*/
void LCD_gotoxy (uint16_t x ,uint16_t y);



/* ---------------------------------------------------------------------------------------------------------------
[Function Name] : LCD_str
[Description] : This function is responsible for displaying a string starting from the current place of the cursor
[Args] :
[int] -const char *string_ptr
this ARG is responsible for pointing at the first character of the string that will be displayed
---------------------------------------------------------------------------------------------------------------*/
void LCD_str(const char *string_ptr);


/* ---------------------------------------------------------------------------------------------------------------
[Function Name] : LCD_strByLocation
[Description] : This function is responsible for displaying a string at a location we specify
[Args] :
[int] -const char *string_ptr
this ARG is responsible for pointing at the first character of the string that will be displayed

-uint8_t x
this ARG is responsible for holding the column number of the place which the cursor will be set at
-uint8_t y
this ARG is responsible for holding the row number of the place which the cursor will be set at
---------------------------------------------------------------------------------------------------------------*/
void LCD_strByLocation(const char *string_ptr , uint8_t x , uint8_t y);


/* ---------------------------------------------------------------------------------------------------------------
[Function Name] : LCD_chr
[Description] : This function is responsible for displaying a character at the current place of the cursor
[Args] :
[int] -uint8_t character
this ARG is responsible for holding the character that will be displayed
---------------------------------------------------------------------------------------------------------------*/
void LCD_chr (uint8_t character);


/* ---------------------------------------------------------------------------------------------------------------
[Function Name] : LCD_chrByLocation
[Description] : This function is responsible for displaying a character at a location we specify

[Args] :
[int] -uint8_t character
this ARG is responsible for holding the character that will be displayed

-uint8_t x
this ARG is responsible for holding the column number of the place which the cursor will be set at
-uint8_t y
this ARG is responsible for holding the row number of the place which the cursor will be set at
---------------------------------------------------------------------------------------------------------------*/
void LCD_chrByLocation (uint8_t character,uint8_t x,uint8_t y );


/* ---------------------------------------------------------------------------------------------------------------
[Function Name] : LCD_shiftCursor
[Description] : This function is responsible for moving the cursor from the current place a specified number of steps right or left
[Args] :
[int] -uint8_t direction
this ARG is responsible for informing the function which direction the cursor will be shifted ,you write either RIGHT(defined as 1) or LEFT(defined as 0)
-uint8_t no_of_steps
this ARG is responsible for informing the function how many steps the cursor will move in the specified direction
---------------------------------------------------------------------------------------------------------------*/
void LCD_shiftCursor (uint8_t direction ,uint8_t no_of_steps);

/* ---------------------------------------------------------------------------------------------------------------
[Function Name] : LCD_shiftScreen
[Description] : This function is responsible for moving the displayed characters on the screen from the current place a specified number of steps right or left
[Args] :
[int] -uint8_t direction
this ARG is responsible for informing the function which direction the screen will be shifted ,you write either RIGHT(defined as 1) or LEFT(defined as 0)
-uint8_t no_of_steps
this ARG is responsible for informing the function how many steps the screen will move in the specified direction
---------------------------------------------------------------------------------------------------------------*/
void LCD_shiftScreen(uint8_t direction ,uint8_t no_of_steps);


/* ---------------------------------------------------------------------------------------------------------------
[Function Name] : LCD_itoa(integer to ASCII)
[Description] : This function is responsible 2 things:
 	 	 	 	1-converting a number into it's corresponding ASCII representation using standard itoa function
				2-displaying the converted number in the LCD at a location we specify

[Args] :
[int] -uint8_t character
this ARG is responsible for holding the character that will be displayed

-uint8_t x
this ARG is responsible for holding the column number of the place which the cursor will be set at
-uint8_t y
this ARG is responsible for holding the row number of the place which the cursor will be set at
---------------------------------------------------------------------------------------------------------------*/
void LCD_itoa(uint16_t number,uint8_t x,uint8_t y);


#endif /* SER_LCD_H_ */