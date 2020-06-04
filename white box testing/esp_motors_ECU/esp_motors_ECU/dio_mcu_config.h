
/* ----------------------------------------------------------------------------
[FILE NAME]:    dio_mcu_config.h

[AUTHOR]:       Ali MAkhlouf

[DATE CREATED]: 5/28/2019 05:10:01

[DESCRIPTION]:  this file contains functions that handles the MCU
                digital I/O pins .. it exists in the HAL layer and contains 
				different versions of code for different MCUs
-----------------------------------------------------------------------------*/

#ifndef DIO_MCU_CONFIG_H_
#define DIO_MCU_CONFIG_H_

#include "micro_config.h"
#include "dio_api.h"
#include <stdbool.h>

// the following version of configurations for ATMEGA32-16-8
#if defined(ATMEGA8) || defined(ATMEGA32) || defined(ATMEGA16)


#ifdef ATMEGA8
//no of io ports in atmega 8
#define NO_PORTS 3

// the base address of the PIN registers (which is PINB)
#define PIN_REG_BASE (volatile uint8_t *)(0x36)
// the base address of the DDR registers (which is DDRB)
#define DDR_REG_BASE (volatile uint8_t *)(0x37)
// the base address of the PORT registers (which is PORTB)
#define PORT_REG_BASE (volatile uint8_t *)(0x38)

//mapping dio pins to their corresponding order in ATMEGA8 pins
#define MCU_PIN0  14 //C6
#define MCU_PIN1  16 //D0 
#define MCU_PIN2  17 //D1
#define MCU_PIN3  18 //D2
#define MCU_PIN4  19 //D3
#define MCU_PIN5  20 //D4
#define MCU_PIN6  6  //B6
#define MCU_PIN7  7  //B7
#define MCU_PIN8  21 //D5
#define MCU_PIN9  22 //D6
#define MCU_PIN10 23 //D7
#define MCU_PIN11 0  //B0
#define MCU_PIN12 1  //B1
#define MCU_PIN13 2  //B2
#define MCU_PIN14 3  //B3
#define MCU_PIN15 4  //B4
#define MCU_PIN16 5  //B5
#define MCU_PIN17 8  //C0
#define MCU_PIN18 9  //C1
#define MCU_PIN19 10 //C2
#define MCU_PIN20 11 //C3
#define MCU_PIN21 12 //C4
#define MCU_PIN22 13 //C5


#elif defined(ATMEGA32) || defined(ATMEGA16)
//no of io ports in atmega32-16
#define NO_PORTS 4

// the base address of the PIN registers (which is PINA)
#define PIN_REG_BASE (volatile uint8_t *)(0x39)
// the base address of the DDR registers (which is DDRA)
#define DDR_REG_BASE (volatile uint8_t *)(0x3A)
// the base address of the PORT registers (which is PORTA)
#define PORT_REG_BASE (volatile uint8_t *)(0x3B)

//mapping dio pins to their corresponding order in ATMEGA32-16 pins
#define MCU_PIN0  8  //B0
#define MCU_PIN1  9  //B1
#define MCU_PIN2  10 //B2
#define MCU_PIN3  11 //B3
#define MCU_PIN4  12 //B4
#define MCU_PIN5  13 //B5
#define MCU_PIN6  14 //B6
#define MCU_PIN7  15 //B7
#define MCU_PIN8  24 //D0
#define MCU_PIN9  25 //D1
#define MCU_PIN10 26 //D2
#define MCU_PIN11 27 //D3
#define MCU_PIN12 28 //D4
#define MCU_PIN13 29 //D5
#define MCU_PIN14 30 //D6
#define MCU_PIN15 31 //D7
#define MCU_PIN16 16 //C0
#define MCU_PIN17 17 //C1
#define MCU_PIN18 18 //C2
#define MCU_PIN19 19 //C3
#define MCU_PIN20 20 //C4
#define MCU_PIN21 21 //C5
#define MCU_PIN22 22 //C6
#define MCU_PIN23 23 //C7
#define MCU_PIN24 7  //A7
#define MCU_PIN25 6  //A6
#define MCU_PIN26 5  //A5
#define MCU_PIN27 4  //A4
#define MCU_PIN28 3  //A3
#define MCU_PIN29 2  //A2
#define MCU_PIN30 1  //A1
#define MCU_PIN31 0  //A0
#endif


#define NUM_PIN_PER_PORT 8

//it chooses the port number whether A-B-C-D
#define PORT_OFFSET(n) (((n) * -3))

#if 0

#if defined(ATMEGA32) || defined(ATMEGA16)
//=======================================================
#define PORTA *((volatile uint8_t *)(0x3B))
#define DDRA  *((volatile uint8_t *)(0x3A))
#define PINA  *((volatile uint8_t *)(0x39))
//=======================================================
#endif

//=======================================================
#define PORTB *((volatile uint8_t *)(0x38))
#define DDRB  *((volatile uint8_t *)(0x37))
#define PINB  *((volatile uint8_t *)(0x36))
//=======================================================
#define PORTC *((volatile uint8_t *)(0x35))
#define DDRC  *((volatile uint8_t *)(0x34))
#define PINC  *((volatile uint8_t *)(0x33))
//=======================================================
#define PORTD *((volatile uint8 *)(0x32))
#define DDRD  *((volatile uint8 *)(0x31))
#define PIND  *((volatile uint8 *)(0x30))
//=======================================================
#endif // if 0


#endif //ATMEGA32 ATMEGA8 ATMEGA16 
/* ----------------------------------------------------------------------------
[Macro Name] : dio_set_pin_dir
[Description] : -This function is responsible for configuring the direction of
 the pin whether input or output or pullup
different MCUs.
-it exists in HAL layer, and is called by DIO_SET_DIR in the application layer
[Args] :
[in] a_dio_no:
	This Arg shall indicate the pin number to be configured
[in] a_pin_dir:
	This Arg shall indicate the direction assigned to the pin
---------------------------------------------------------------------------- */
void dio_set_pin_dir (uint8_t a_pin_no, uint8_t a_pin_dir);

/* ----------------------------------------------------------------------------
[Macro Name] : dio_set_pin_val
[Description] : -This function is responsible for
assigning the output state of the pin (high output -- low output)
-it exists in HAL layer, and is called by DIO_SET_VAL in the application layer
[Args] :
[in] a_dio_no:
	This Arg shall indicate the pin number to be configured
[in] a_dio_val:
	This Arg shall indicate the value assigned to the pin, it has two
	options:
	DIO_HIGH = 1 -- DIO_LOW = 0
---------------------------------------------------------------------------- */
void dio_set_pin_val (uint8_t a_pin_no, uint8_t a_pin_val);

/* ----------------------------------------------------------------------------
[Macro Name] : dio_get_pin
[Description] : responsible for getting the state(value) applied on the pin
-it exists in HAL layer, and is called by DIO_READ in the application layer

[Args] :
[in] a_dio_no:
	This Arg shall indicate the pin number to be read
[Returns] : this return shal indicate the state of the pin:
	whether ghigh state : true
	or low state: false
---------------------------------------------------------------------------- */
bool dio_get_pin (uint8_t a_pin_no);


/* ----------------------------------------------------------------------------
[Macro Name] : dio_togg_pin
[Description] : -This function is an used in application layer, responsible for
toggling the pin state(value)
-it exists in HAL layer, and is called by DIO_TOGGLE in the application layer

[Args] :
[in] a_dio_no:
	This Arg shall indicate the pin number to be configured
---------------------------------------------------------------------------- */
void dio_togg_pin (uint8_t a_pin_no);


#endif /* DIO_MCU_CONFIG_H_ */