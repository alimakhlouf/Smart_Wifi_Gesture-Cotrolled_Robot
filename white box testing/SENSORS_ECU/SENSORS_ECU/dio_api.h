/*
 * DIO_API.h

 *
 *  Created on: Mar 17, 2019
 *      Author: MoaazSherif
 */
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
#define DIO_SET_STATE(dio_no, dio_state) DIO_SetPinDir(CAT(DIO_PIN, dio_no), dio_state)

#define DIO_SET_VAL(dio_no, dio_state) DIO_SetPinVal(CAT(DIO_PIN, dio_no), dio_state)
								       
#define DIO_READ(dio_no) DIO_GetPin(CAT(DIO_PIN, dio_no))

#define DIO_TOGGLE(dio_no) DIO_ToggPinVal(CAT(DIO_PIN, dio_no))								   

inline void dio_set_state(uint8_t dio_no,uint8_t dio_state)
{
	DIO_SetPinDir(dio_no, dio_state);
}

inline uint8_t dio_read(uint8_t dio_no)
{
	return DIO_GetPin(dio_no);
}

inline void dio_set_val(uint8_t dio_no, uint8_t dio_val)
{
	DIO_SetPinVal(dio_no, dio_val);
}
inline void dio_toggle(uint8_t dio_no)
{
	DIO_ToggPinVal(dio_no);
}


#endif /* DIO_API_H_ */
