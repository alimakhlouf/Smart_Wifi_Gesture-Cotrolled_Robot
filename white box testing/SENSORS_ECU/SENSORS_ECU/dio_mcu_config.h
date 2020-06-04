/*
 * dio_mcu_config.h
 *
 * Created: 5/28/2019 05:10:01 م
 *  Author: hppp
 */ 


#ifndef DIO_MCU_CONFIG_H_
#define DIO_MCU_CONFIG_H_

#include "micro_config.h"

#if defined(ATMEGA8) || defined(ATMEGA32) || defined(ATMEGA16)

#ifdef ATMEGA8
#define No_Ports 3

#define PIN_REG_BASE (volatile uint8_t *)(0x39)
#define DDR_REG_BASE (volatile uint8_t *)(0x38)
#define PORT_REG_BASE (volatile uint8_t *)(0x37)

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
#define No_Ports 4

#define PIN_REG_BASE (volatile uint8_t *)(0x39)
#define DDR_REG_BASE (volatile uint8_t *)(0x3A)
#define PORT_REG_BASE (volatile uint8_t *)(0x3B)
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


#define No_PinPerPort 8
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

void DIO_SetPinDir (uint8_t Pin_No, uint8_t PinDir);
uint8_t DIO_GetPin (uint8_t Pin_No);
void DIO_SetPinVal (uint8_t Pin_No, uint8_t PinVal);
void DIO_ToggPinVal (uint8_t Pin_No);


#endif /* DIO_MCU_CONFIG_H_ */