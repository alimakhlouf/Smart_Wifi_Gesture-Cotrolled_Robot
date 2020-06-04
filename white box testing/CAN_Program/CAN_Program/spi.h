#ifndef SPI_LIB
#define SPI_LIB 

#include "std_types.h"
#include "micro_config.h"
#include "common_macros.h"

#define SPI_MASTER  1
#define SPI_SLAVE 0

/*******************************************CONFIGURATIONS******************************************************/
/**********PIN CONFIGURATION*****************/
#define SS 4
#define MOSI 5
#define MISO 6
#define SCK 7
#define SPI_PORT PORTB
#define SPI_DIR DDRB

/*********************NODE TYPE************************
MASTER = 1
SLAVE = 0
************************************/
#define SPI_NODE_TYPE 1

/*****************PRESCALER COFIGURATION********************
FREQ/4   : SPR1:<0> = 0 --- SPI2X = 0
FREQ/16  : SPR1:<0> = 1 --- SPI2X = 0
FREQ/64  : SPR1:<0> = 2 --- SPI2X = 0
FREQ/128 : SPR1:<0> = 3 --- SPI2X = 0
FREQ/2   : SPR1:<0> = 0 --- SPI2X = 1 (NOT RECOMMENDED)
FREQ/8   : SPR1:<0> = 1 --- SPI2X = 1
FREQ/32  : SPR1:<0> = 2 --- SPI2X = 1
FREQ/64  : SPR1:<0> = 3 --- SPI2X = 1
*/
#define SPR1_0_PRESCALER 0  
#define SPI2X_PRESCALER 0
/**************************/

/*******CLOCK PHASE *********
SAMPLE AT LEADING EDGE = 0
SHIFT OUT(SETUP) AT LEADING EDGE = 1
*/
#define PHASE 0

/*******CLOCK POLARITY *********
HIGH LEADING EDGE - LOW TRAILING EDGE = 0
LOW LEADING EDGE - HIGH TRAILING EDGE = 1
*/
#define POLARITY 0

/*******DATA ORDER *********
SEND MSB FIRST = 0
SEND LSB FIRST = 1
*/
#define DATA_ORDER 0

/***SPI interrupt****
disable =0
ENABLE  =1
*/
#define SPI_INTERRUPT 0

/****************************END OF CONFIGURATIONS********************************/



void SPI_init (void); 
void SPI_sendChr (unint8_t data); 
unint8_t SPI_receiveChr ();
void SPI_sendString (unint8_t *Str);
void SPI_recieveStrByMark (unint8_t *Str, unint8_t mark);
void SPI_receiveStrBySize (unint8_t *Str, unint8_t size);
void SPI_sendStrBySize (const unint8_t *Str, unint8_t size);
void SPI_sendStrByMark (const unint8_t *Str, unint8_t mark);



#if (SPI_INTERRUPT == 1)
extern volatile unint8_t spi_flag;
#endif

#endif
