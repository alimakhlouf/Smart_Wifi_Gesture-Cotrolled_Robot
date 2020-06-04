/* ----------------------------------------------------------------------------
[FILE NAME]: spi_mcu_config.h

[AUTHOR]: Ali Makhlouf

[DATE CREATED]: 12/5/2017

[DESCRIPTION]:  a library header file for controlling and issuing SPI operations
                it's in the HAL layer
-----------------------------------------------------------------------------*/

#ifndef SPI_LIBB
#define SPI_LIBB

#include "micro_config.h"
#include "std_types.h"

#include "common_macros.h"
#include "stdbool.h"
#include "dio_api.h"
#include "spi_api.h"



// the following version of configurations for ATMEGA32-16-8
#if defined(ATMEGA8) || defined(ATMEGA32) || defined(ATMEGA16)


#if defined(ATMEGA32) || defined(ATMEGA16)
//SPI pins locations in ATMEGA16-32 
#define SS   4
#define MOSI 5
#define MISO 6
#define SCK  7
#endif 

//spi modes options
#define SPI_MASTER  1
#define SPI_SLAVE 0

//spi phase options
#define SAMPLE_AT_RISING 0
#define SHIFT_AT_RISING  1

//SPI polarity options
#define HIGH_EDGE_LEAD 0
#define LOW_EDGE_LEAD  1

//SPI byte order options
#define SPI_MSB_FIRST  0
#define SPI_LSB_FIRST  1

//bit rate is only assigned when SPI is in MASTER mode
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


/* -----------------------------------------------------------------------------
[Function Name] : spi_int_stat
[Description] : This function indicates whether an SPI operation has been 
completed or not

[Args] :
[in] -uint8_t a_data
	this argument is responsible for holding the character that will be sent

[Return] :returns true if data received, false if not
------------------------------------------------------------------------------*/
inline bool spi_int_stat()
{
    return ((BIT_IS_SET(SREG, I)) && (BIT_IS_SET(SPCR, SPIE)));
}


#if defined(ATMEGA32)
//define CONFIG_TXC_ISR as SPI complete interrupt handler
#define CONFIG_SPI_ISR   __vector_12
void CONFIG_SPI_ISR(void) __attribute__((signal, __INTR_ATTRS));
#elif defined(ATMEGA16)
//define CONFIG_TXC_ISR as SPI complete interrupt handler
#define CONFIG_SPI_ISR   __vector_10
void CONFIG_SPI_ISR(void) __attribute__((signal, __INTR_ATTRS));
#endif //defined(ATMEGA32) || defined(ATMEGA16)

#endif // if defined(ATMEGA32) || defined(ATMEGA16) || defined(ATMEAG8)

/* ----------------------------------------------------------------------------
[Function Name] : UART_init
[Description] : This function is responsible for initializing SPI with all of 
                it's characteristics such as it's bit rate, master_slave_type 
				phase, polarity. all these configurations are entered by the  
				user in their corresponding #defines and handled at pre-compile
				time.
-----------------------------------------------------------------------------*/
void spi_init (void); 

/* ----------------------------------------------------------------------------
[Function Name] : spi_send
[Description] : This function is responsible for sending a byte of data through
SPI
[Args] :
[in] -uint8_t a_data
	this argument is responsible for holding the character that will be sent
------------------------------------------------------------------------------*/
void spi_send (unint8_t data);

/* -----------------------------------------------------------------------------
[Function Name] : spi_read
[Description] : This function is responsible for receiving a byte of data
[NOTE]: in master mode you can't use this function in the interrupt mode
		the user have to make his own method in the application layer
[Return]: returns the byte that's been received
-----------------------------------------------------------------------------*/
unint8_t spi_read ();

/* ----------------------------------------------------------------------------
[Function Name] : usart_set_int
[Description] : This function enables or disables the SPI interrupt
[Args] :
[in] -bool * b_state
	this argument indicates the interrupt new state that'll be assigned
	options : true - false
-----------------------------------------------------------------------------*/
void spi_set_int (bool b_state);

/* ----------------------------------------------------------------------------
[Function Name] : spi_set_isr
[Description] : This function assigns a function to the SPI interrupt handler
[Args] :
[in] -void ( * ap_spi_function)(void)
	this argument holds the function that will be assigned to the SPI interrupt 
	handler
-----------------------------------------------------------------------------*/
void spi_set_isr (void ( * p_spi_function)(void));

#endif // if 
