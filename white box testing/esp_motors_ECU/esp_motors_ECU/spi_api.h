/* --------------------------------------------------------------------------
[FILE NAME]:    spi_api.h

[AUTHOR]:       Ali MAkhlouf

[DATE CREATED]:  6/5/2019 06:26:53 م

[DESCRIPTION]:  a library header file for controlling and issuing SPI operations
                it's in the application layer
----------------------------------------------------------------------------*/

#ifndef SPI_API_H_
#define SPI_API_H_

#include "micro_config.h"
#include "std_types.h"

#include "common_macros.h"
#include "stdbool.h"
#include "dio_api.h"
#include "spi_mcu_config.h"


/*
[NOTE] : here SPI features are configured as #defines instead of parameters
because most 8-bit MCUs have one SPI peripheral so I can use this method
as I'll be always having 1 SPI and so the generic feature is maintained
( in case of USART some MCUs have more than one USART so I can't use this
method there as in the api library I don't know how many UART I am dealing with
so how many #defines will be needed)
*/
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
#define PHASE SAMPLE_AT_RISING



/*******CLOCK POLARITY *********/
//HIGH_EDGE_LEAD
//LOW_EDGE_LEAD
#define POLARITY HIGH_EDGE_LEAD

/*******DATA ORDER *********/
//SPI_MSB_FIRST
//SPI_LSB_FIRST
#define DATA_ORDER SPI_MSB_FIRST

/****************************END OF CONFIGURATIONS***************************/

/* ----------------------------------------------------------------------------
[Function Name] : UART_init
[Description] : This function is responsible for initializing SPI with all of 
                it's characteristics such as it's bit rate, master_slave_type 
				phase, polarity. all these configurations are entered by the  
				user in their corresponding #defines and handled at pre-compile
				time.
-----------------------------------------------------------------------------*/
extern void spi_init ();

/* -----------------------------------------------------------------------------
[Function Name] : spi_send
[Description] : This function is responsible for sending a byte of data through
SPI
[Args] :
[in] -uint8_t a_data
this argument is responsible for holding the character that will be sent
------------------------------------------------------------------------------*/
void spi_send (unint8_t a_data);

/* -----------------------------------------------------------------------------
[Function Name] : spi_read
[Description] : This function is responsible for receiving a byte of data
[NOTE]: in master mode you can't use this function in the interrupt mode
the user have to make his own method in the application layer
[Return]: returns the byte that's been received
------------------------------------------------------------------------------*/
unint8_t spi_read (void);  

/* ----------------------------------------------------------------------------
[Function Name] : spi_send_str_mark
[Description] : This function is responsible for sending a string of data
through SPI
the string end is defined by a particular character which the function will stop
sending (before) sending this character
[NOTE] THAT THIS FUNCTION SHOULD BE ONLY USED IN THE POLLING MODE(IN INTERRUPT IT
IS SO APPLICATION-DEPENDENT AND IT'S DIFFICULT TO GET A STANDARD WAY OF DOING IT)
[Args] :
[in] -unint8 a_mark
this argument indicates the char that identifies the end of the sent string
[in] -const unint8_t * ap_str.
this ARG is pointing to the first character of the string that will be sent
-----------------------------------------------------------------------------*/
void spi_send_str_mark (const unint8_t * ap_str, unint8_t a_mark);


/* ----------------------------------------------------------------------------
[Function Name] : spi_send_str_size
[Description] : This function sends a string of data through SPI
[NOTE] THAT THIS FUNCTION SHOULD BE ONLY USED IN THE POLLING MODE(IN INTERRUPT IT 
IS SO APPLICATION-DEPENDENT AND IT'S DIFFICULT TO GET A STANDARD WAY OF DOING IT)
[Args] :
[in] -const unint8_t * ap_str
this ARG is responsible for pointing to the first character of the string
[in] -uint8_t a_size
this ARG is responsible for holding the informing the function about the size
of the string(number of characters that will be sent).
----------------------------------------------------------------------------*/
void spi_send_str_size (const unint8_t * ap_str, unint8_t a_size);

/* ----------------------------------------------------------------------------
[Function Name] : usart_read_str_mark
[Description] : This function is responsible for receiving a string of data
through UART
-the string end is defined by a particular character which the function will
stop receiving when it sees it and then replace it with null'\0'
[NOTE] THAT THIS FUNCTION SHOULD BE ONLY USED IN THE POLLING MODE(IN INTERRUPT IT
IS SO APPLICATION-DEPENDENT AND IT'S DIFFICULT TO GET A STANDARD WAY OF DOING IT)
[Args] :
[in] -unint8_t a_usart_no
this argument shall indicate which USART peripheral that's being initialized.
it's in this form: USARTn (n is the number of peripheral starting from 0)
[in] -uint8 a_mark
this argument indicates the char that identifies the end of the received string
[out] -const char * ap_str
this ARG is pointing to the first character of the string that will be received
-----------------------------------------------------------------------------*/
void spi_read_str_mark (unint8_t *Str, unint8_t mark);

/* ----------------------------------------------------------------------------
[Function Name] : spi_read_str_size
[Description] : This function is responsible for receiving a string of data 
through UART
the string size(number of characters it's going to received) is passed to it so
-the function will stop receiving when it reaches the size
[NOTE] THAT THIS FUNCTION SHOULD BE ONLY USED IN THE POLLING MODE(IN INTERRUPT IT
IS SO APPLICATION-DEPENDENT AND IT'S DIFFICULT TO GET A STANDARD WAY OF DOING IT)

[Args] :
[in] -uint8_t a_str_size
this ARG is responsible for holding the informing the function about the size
of the string(number of characters that will be sent)
[out] -unint8_t * ap_str
this ARG is responsible for pointing to the first character of the string that
will be received
-----------------------------------------------------------------------------*/
void spi_read_str_size (unint8_t *ap_str, unint8_t a_str_size);

/* ----------------------------------------------------------------------------
[Function Name] : usart_set_int
[Description] : This function enables or disables the SPI interrupt
[Args] :
[in] -bool * b_state
this argument indicates the interrupt new state that'll be assigned
options : true - false
-----------------------------------------------------------------------------*/
void spi_set_int (bool int_state);

/* ----------------------------------------------------------------------------
[Function Name] : spi_set_isr
[Description] : This function assigns a function to the SPI interrupt handler
[Args] :
[in] -void ( * ap_spi_function)(void)
this argument holds the function that will be assigned to the SPI interrupt
handler
-----------------------------------------------------------------------------*/
void spi_set_isr (void ( * ap_spi_function)(void));

#endif /* SPI_API_H_ */