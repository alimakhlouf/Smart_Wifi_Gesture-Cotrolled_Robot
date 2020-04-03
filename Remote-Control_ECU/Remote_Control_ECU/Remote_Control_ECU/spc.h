/*
 * 74HC595.h
 *
 *  Created on: ?? —Ã»° ???? Â‹
 *      Author: hppp
 */

#ifndef SIPO_H
#define SIPO_H

#include "common_macros.h"
#include "std_types.h"
#include "micro_config.h"
#include "dio_api.h"

#define SH_CP_PIN 29 //shift clock pulse
#define DS_PIN	  31 // data pin
#define ST_CP_PIN 30 //store clock pulse


#if 0
#define SERIAL_PORT PORTC
#define SERIAL_DDR DDRC
#define SH_CP 0 //shift clock pulse
#define DS	2 // data pin
#define ST_CP 1 //store clock pulse
#endif

void spc_send (uint8_t , int8_t );
void spc_init ();

#endif /* 74HC595_H_ */
