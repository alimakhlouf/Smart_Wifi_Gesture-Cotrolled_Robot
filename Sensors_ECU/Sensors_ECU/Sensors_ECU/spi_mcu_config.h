#ifndef SPI_LIBB
#define SPI_LIBB

#include "micro_config.h"
#include "std_types.h"

#include "common_macros.h"
#include "stdbool.h"
#include "dio_api.h"
#include "spi_api.h"



/*******************************************CONFIGURATIONS******************************************************/
#if defined(ATMEGA32) || defined(ATMEGA16)


#define SS   4
#define MOSI 5
#define MISO 6
#define SCK  7

#define SPI_MASTER  1
#define SPI_SLAVE 0

#define SAMPLE_AT_RISING 0
#define SHIFT_AT_RISING  1

#define HIGH_EDGE_LEAD 0
#define LOW_EDGE_LEAD  1

#define SPI_MSB_FIRST  0
#define SPI_LSB_FIRST  1





#if SPI_NODE_TYPE == SPI_MASTER

#if (SPI_BIT_RATE == (F_CPU / 2))
#define SPR0_1 0
#define SPI2X  1
#elif (SPI_BIT_RATE == (F_CPU / 4))
#define SPR0_1 0
#define SPI2X  0
#elif (SPI_BIT_RATE == (F_CPU / 8))
#define SPR0_1 1
#define SPI2X  1
#elif (SPI_BIT_RATE == (F_CPU / 16))
#define SPR0_1 1
#define SPI2X  0
#elif (SPI_BIT_RATE == (F_CPU / 32))
#define SPR0_1 2
#define SPI2X  1
#elif (SPI_BIT_RATE == (F_CPU / 64))
#define SPR0_1 2
#define SPI2X  0
#elif (SPI_BIT_RATE == (F_CPU / 128))
#define SPR0_1 3
#define SPI2X  0
#else
#error "spi bit rate connot be reached, please choose another bit rate or cpu frequency"
#endif //if SPI_BIT_RATE

#endif  //if SPI_TYPE == SPI MASTER



inline bool spi_int_stat()
{
    return ((BIT_IS_SET(SREG, I)) && (BIT_IS_SET(SPCR, SPIE)));
}

#if defined(ATMEGA32) || defined(ATMEGA16)
// Transmit complete interrupt.
#define CONFIG_SPI_ISR   __vector_12
void CONFIG_SPI_ISR(void) __attribute__((signal, __INTR_ATTRS));
#endif //defined(ATMEGA32) || defined(ATMEGA16)


#endif // if (defined(ATMEGA32) || defined(ATMEGA16)


void spi_init (void); 
void spi_send (unint8_t data);
unint8_t spi_read ();
void spi_set_int (bool b_state);
void spi_set_isr (void ( * p_spi_function)(void));



#endif // if 
