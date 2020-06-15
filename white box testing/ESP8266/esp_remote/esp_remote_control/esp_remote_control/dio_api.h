/* ----------------------------------------------------------------------------
[FILE NAME]:    dio_api.h

[AUTHOR]:       Ali MAkhlouf

[DATE CREATED]: 6/01/2019 03:04:00

[DESCRIPTION]:  this file contains abstract MACROs that handles the MCU
                digital I/O pins .. it's used in the application layer
-----------------------------------------------------------------------------*/

//==============================================================
/* this file will be included in all other peripherals drivers.*/
//==============================================================
#ifndef DIO_API_H_
#define DIO_API_H_

//===============================================
#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"
#include "dio_mcu_config.h"
//===============================================

//the user uses the DIO_PINn where n is the number of the pin
#define DIO_PIN0  MCU_PIN0
#define DIO_PIN1  MCU_PIN1
#define DIO_PIN2  MCU_PIN2
#define DIO_PIN3  MCU_PIN3
#define DIO_PIN4  MCU_PIN4
#define DIO_PIN5  MCU_PIN5
#define DIO_PIN6  MCU_PIN6
#define DIO_PIN7  MCU_PIN7
#define DIO_PIN8  MCU_PIN8
#define DIO_PIN9  MCU_PIN9
#define DIO_PIN10 MCU_PIN10
#define DIO_PIN11 MCU_PIN11
#define DIO_PIN12 MCU_PIN12
#define DIO_PIN13 MCU_PIN13
#define DIO_PIN14 MCU_PIN14
#define DIO_PIN15 MCU_PIN15
#define DIO_PIN16 MCU_PIN16
#define DIO_PIN17 MCU_PIN17
#define DIO_PIN18 MCU_PIN18
#define DIO_PIN19 MCU_PIN19
#define DIO_PIN20 MCU_PIN20
#define DIO_PIN21 MCU_PIN21
#define DIO_PIN22 MCU_PIN22
#define DIO_PIN23 MCU_PIN23
#define DIO_PIN24 MCU_PIN24
#define DIO_PIN25 MCU_PIN25
#define DIO_PIN26 MCU_PIN26
#define DIO_PIN27 MCU_PIN27
#define DIO_PIN28 MCU_PIN28
#define DIO_PIN29 MCU_PIN29
#define DIO_PIN30 MCU_PIN30
#define DIO_PIN31 MCU_PIN31

//==============================================
#define DIO_INPUT  	0
#define DIO_OUTPUT  1
#define DIO_PULLUP  2
//==============================================
#define DIO_HIGH    1
#define DIO_LOW     0
//==============================================

/* ----------------------------------------------------------------------------
[Macro Name] : DIO_SET_DIR
[Description] : -This function is used in application layer, responsible for 
configuring the direction of the pin whether input or output or pullup
-it calls a function that performs this action in HAL layer
[Args] : 
[in] a_dio_no: 
This Arg shall indicate the pin number to be configured
[in] a_dio_dir: 
This Arg shall indicate the direction assigned to the pin, it has three
options:
DIO_INPUT = 0 -- DIO_OUTPUT = 1 -- DIO_PULLUP = 2
---------------------------------------------------------------------------- */
#define DIO_SET_DIR(a_dio_no, a_dio_dir) dio_set_pin_dir(CAT(DIO_PIN, a_dio_no), \
                                                        a_dio_dir)

/* ----------------------------------------------------------------------------
[Macro Name] : DIO_SET_VAL
[Description] : -This function is an used in application layer, responsible for
assigning the output state of the pin (high output -- low output)
-it calls a function that performs this operation in HAL layer
[Args] :
[in] a_dio_no:
This Arg shall indicate the pin number to be configured
[in] a_dio_val:
This Arg shall indicate the value assigned to the pin, it has two
options:
DIO_HIGH = 1 -- DIO_LOW = 0
---------------------------------------------------------------------------- */
#define DIO_SET_VAL(a_dio_no, a_dio_val) dio_set_pin_val(CAT(DIO_PIN, a_dio_no),\
                                                         a_dio_val)

/* ----------------------------------------------------------------------------
[Macro Name] : DIO_READ
[Description] : -This function is an used in application layer, responsible for
getting the state(value) applied on the pin  
-it calls a function that performs this operation in HAL layer

[Args] :
[in] a_dio_no:
This Arg shall indicate the pin number to be read
[NOTE]:you can't use variable, you can use either number, #define or DEFINE_PIN 
---------------------------------------------------------------------------- */								       
#define DIO_READ(a_dio_no) dio_get_pin(CAT(DIO_PIN, a_dio_no))

/* ----------------------------------------------------------------------------
[Macro Name] : DIO_TOGGLE
[Description] : -This function is an used in application layer, responsible for
toggling the pin state(value) 
-it calls a function that performs this operation in HAL layer

[Args] :
[in] a_dio_no:
This Arg shall indicate the pin number to be configured
[NOTE]:you can't use variable, you can use either number, #define or DEFINE_PIN
---------------------------------------------------------------------------- */
#define DIO_TOGGLE(a_dio_no) dio_togg_pin(CAT(DIO_PIN, a_dio_no))								   


#endif /* DIO_API_H_ */
