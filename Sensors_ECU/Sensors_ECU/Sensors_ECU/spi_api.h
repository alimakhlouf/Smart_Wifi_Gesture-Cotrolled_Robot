/*
 * spi_api.h
 *
 * Created: 6/5/2019 06:26:53 م
 *  Author: hppp
 */ 


#ifndef SPI_API_H_
#define SPI_API_H_

#include "micro_config.h"
#include "std_types.h"

#include "common_macros.h"
#include "stdbool.h"
#include "dio_api.h"
#include "spi_mcu_config.h"


/*********************NODE TYPE************************
SPI_MASTER
SPI_SLAVE
************************************/
#define SPI_NODE_TYPE SPI_MASTER

#if SPI_NODE_TYPE == SPI_MASTER
/************* SPI BIT RATE****************/
#define SPI_BIT_RATE 2000000UL
#endif

/*******CLOCK PHASE *********/
//SAMPLE_AT_RISING 0
//SHIFT_AT_RISING  1
#define PHASE SAMPLE_AT_RISE_E



/*******CLOCK POLARITY *********/
//HIGH_EDGE_LEAD
//LOW_EDGE_LEAD
#define POLARITY HIGH_EDGE_LEAD

/*******DATA ORDER *********/
//SPI_MSB_FIRST
//SPI_LSB_FIRST
#define DATA_ORDER MSB_FIRST

/****************************END OF CONFIGURATIONS********************************/





uint8_t spi_exchange(uint8_t spi_num, uint8_t sent_byte);
void spi_send_until (uint8_t spi_num, const unint8_t *Str, unint8_t mark);
void spi_send (uint8_t spi_num, const unint8_t *Str, unint8_t size);
void spi_read_until (uint8_t spi_num, unint8_t *Str, unint8_t mark);
void spi_read (uint8_t spi_num, unint8_t *Str, unint8_t size);
void spi_set_int (uint8_t spi_num, bool int_state);
void spi_set_isr (uint8_t spi_num, void ( * p_spi_function)(void));

#endif /* SPI_API_H_ */