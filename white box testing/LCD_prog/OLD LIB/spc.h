/*
 * 74HC595.h
 *
 *  Created on: ?? ��ȡ ???? ��
 *      Author: hppp
 */

#ifndef SIPO_H
#define SIPO_H


#define SERIAL_PORT PORTC
#define SERIAL_DDR DDRC
#define SH_CP 1 //shift clock pulse
#define DS	2 // data pin
#define ST_CP 3 //store clock pulse

void spc_send (uint8_t , uint8_t );
void spc_init ();

#endif /* 74HC595_H_ */
