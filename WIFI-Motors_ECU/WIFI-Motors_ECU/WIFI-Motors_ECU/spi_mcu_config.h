#ifndef SPI_LIBB
#define SPI_LIBB

#include "micro_config.h"
#include "std_types.h"

#include "common_macros.h"
#include "stdbool.h"
#include "dio_api.h"
#include "spi_api.h"



/*******************************************CONFIGURATIONS******************************************************/
#if defined(ATMEGA8) || defined(ATMEGA32) || defined(ATMEGA16)

#define SPI0 0

#define SS   4
#define MOSI 5
#define MISO 6
#define SCK  7

#define SPI_MASTER  1
#define SPI_SLAVE 0

#define SAMPLE_AT_RISE_E 0
#define SHIFT_AT_RISE_E  1

#define HIGH_EDGE_LEAD 0
#define LOW_EDGE_LEAD  1

#define MSB_FIRST  0
#define LSB_FIRST  1



inline bool spi_int_stat(uint8_t spi_num)
{
	if (spi_num == SPI0)
	{
		return ((BIT_IS_SET(SREG, I)) && (BIT_IS_SET(SPCR, SPIE)));
	}
	else
	{
		
	}
	
}

#if defined(ATMEGA32)
// Transmit complete interrupt.
#define CONFIG_SPI_ISR   __vector_12
void CONFIG_SPI_ISR(void) __attribute__((signal, __INTR_ATTRS));
#elif defined(ATMEGA16)
#define CONFIG_SPI_ISR   __vector_10
void CONFIG_SPI_ISR(void) __attribute__((signal, __INTR_ATTRS));
#endif //defined(ATMEGA32) || defined(ATMEGA16)


#endif // if (defined(ATMEGA32) || defined(ATMEGA16)


uint8_t spi_init_master(uint8_t spi_num, uint32_t bit_rate, uint8_t phase, uint8_t polarity, uint8_t data_order);
void spi_init_slave (uint8_t spi_num, uint8_t phase, uint8_t polarity, uint8_t data_order);
void spi_set_int (uint8_t spi_num, bool b_state);
void spi_set_isr (uint8_t spi_num, void ( * p_spi_function)(void));



#endif // if 
