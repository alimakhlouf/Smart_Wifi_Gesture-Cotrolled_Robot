/* --------------------------------------------------------------------------
[FILE NAME]:    uart_api.h

[AUTHOR]:       Ali MAkhlouf

[DATE CREATED]:  4/18/2019 12:07:19 م

[DESCRIPTION]:  a library header file for controlling and issuing UART 
                operations, it's in the application layer
----------------------------------------------------------------------------*/

#ifndef UART_CONFIG_H_
#define UART_CONFIG_H_


#include "micro_config.h"
#include "uart_mcu_config.h"
#include "dio_mcu_config.h"
#include "stdbool.h"
#include "std_types.h"
#include <stdbool.h>

//PERIPHERAL SETTINGS
#if 0

/*****enter the baud rate****/
#define BAUD_RATE 9600


/*use TX ? RX? OR BOTH
UART_TX 1
UART_RX 2
TX_RX   3
*/
#define UART_TX_RX UART_BOTH
 
/*PARITY BIT 
 NO_PARITY disable   0 
 EVEN_PARITY even parity 2
 ODD_PARITY odd parity   3
 */
#define PARITY NO_PARITY

/* NUMBER OF STOP BITS
ONE_STOP_BIT for one stop bit mode  0
TWO_STOP_BITS for two stop bit mode  1
*/
#define STOP_BITS ONE_STOP_BIT


/*CHOOSE DATABITS SIZE
5_DATA_BITS 0 FOR 5 BITS
6_DATA_BITS 1 FOR 6 BITS
7_DATA_BITS 2 FOR 7 BITS
8_DATA_BITS 3 FOR 8 BITS
9_DATA_BITS 7
*/
#define DATA_SIZE 8_DATA_BITS


/* CLOCK POLARITY -USED FOR SYNCH ONLY
TX_RISE_RX_FALL 0  TRANSMIT CHANGE ON RISING EDGE ,RECEIVE SAMPLE ON FALLING EDGE 
TX_RISE_RX_FALL 1  REANSMIT CHANGE ON FALLING EDGE ,RECEIVE SAMPLE ON RISING EDGE 
*/
#define TRANS_SAMPLE_PHASE TX_RISE_RX_FALL 


/*transmit complete interrupt*/
/* ENABLE 1  - DISABLE 0  */
#define INITIAL_RX_INT ENABLE
#define INITIAL_TX_INT DISABLE

/*UART MODE
ASYNCH for Asynchronous mode
SYNCH for synchronous mode
*/
#define SYNCH_ASYNCH ASYNCH

#if SYNCH_ASYNCH == SYNCH
/*UART NODE TYPE
MASTER 1
SLAVE 0
*/
#    define UART_NODE_TYPE 
#endif


#endif


/* ----------------------------------------------------------------------------
[Function Name] : UART_init
[Description] : This function is responsible for initializing UART with all of 
                it's characteristics such as it's baud rate, receive enable, 
				transmit enable sampling phase, receive enable and it's 
				interrupt , stop bits , parity bit synchronizations mode and 
				data size.
[Args] :
[in] -unint8_t a_usart_no
	this argument shall indicate which USART peripheral that's being initialized.
	it's in this form: USARTn (n is the number of peripheral starting from 0)
[in] -uint32_t a_baud_rate
	this argument indicates the baud rate that the USART peripheral will work on.
[in] -unint8_t a_data_size
	this argument indicates the data size that's going to be send within each frame.
	it's in this form: DATA_SIZE_n (n is the size). 
[in] -unint8_t a_tx_or_rx
	this argument indicates whether TX or RX or both are activated.
	it's in this form: -TX  -RX -TX_RX 
[in] -unint8_t a_parity
	this argument indicates whether parity check is used or not.
	it's in this form: NO_PARITY -EVEN_PARITY - ODD_PARITY
[in] -unint8_t a_stop_bits
	this argument indicates the stop bits configurations.
	it's in this form: ONE_STOP_BITS-ONE_HALF_STOP_BITS-TWO_STOP_BITS and so on
[in] -unint8_t a_sample_phase
	this argument indicates which edge to sample and which to transfer. 
	options are: TX_RISE_RX_FALL - TX_FALL_RX_RISE
-----------------------------------------------------------------------------*/
extern void usart_init (unint8_t a_usart_no, uint32_t a_baud_rate, unint8_t a_data_size,
                            unint8_t a_tx_or_rx, unint8_t a_parity, unint8_t a_stop_bits,
							     unint8_t a_sample_phase);

/* ----------------------------------------------------------------------------
[Function Name] : uart_set_usart
[Description] : This function is responsible for sending a byte of data through 
UART.
[Args] :
[in] -unint8_t a_usart_no
	this argument shall indicate which USART peripheral that's being initialized.
	it's in this form: USARTn (n is the number of peripheral starting from 0)
[in] -uint8_t a_mode
	this argument determines whether synchronous or asynchronous.
	the options are: ASYNCH - SYNCH
------------------------------------------------------------------------------*/
extern void uart_set_usart (unint8_t usart_no, unint8_t a_mode);

/* -----------------------------------------------------------------------------
[Function Name] : usart_write
[Description] : This function is responsible for sending a frame of data through
UART
[Args] :
[in] -unint8_t a_usart_no
	this argument shall indicate which USART peripheral that's being initialized.
	it's in this form: USARTn (n is the number of peripheral starting from 0)
[in] -uint8_t a_data
	this argument is responsible for holding the character that will be sent
------------------------------------------------------------------------------*/
extern void usart_write (unint8_t usart_no, unint8_t a_data);

/* -----------------------------------------------------------------------------
[Function Name] : usart_read
[Description] : This function is responsible for receiving a frame of data
[Args] :
[in] -unint8_t a_usart_no
	this argument shall indicate which USART peripheral that's being initialized.
	it's in this form: USARTn (n is the number of peripheral starting from 0)
[Return]: returns the data_bits received
------------------------------------------------------------------------------*/
extern unint8_t usart_read (unint8_t usart_no);

/* ----------------------------------------------------------------------------
[Function Name] : usart_read_str_size
[Description] : This function is responsible for receiving a string of data 
through UART
the string size(number of characters it's going to received) is passed to it so
-the function will stop receiving when it reaches the size
-[NOTE] THAT THIS FUNCTION SHOULD BE ONLY USED IN THE POLLING MODE
(IN INTERRUPT THESENDING OR RECEIVING METHOD IS SO APPLICATION-DEPENDENT AND 
IT'S DIFFICULT TO GET A STANDARD WAY OF DOING IT)
[Args] :
[in] -unint8_t a_usart_no
	this argument shall indicate which USART peripheral that's being initialized.
	it's in this form: USARTn (n is the number of peripheral starting from 0)
[in] -uint8_t a_str_size
	this ARG is responsible for holding the informing the function about the size 
	of the string(number of characters that will be sent)
[out] -unint8_t * ap_str
	this ARG is responsible for pointing to the first character of the string that
	will be received
-----------------------------------------------------------------------------*/
void usart_read_str_size (unint8_t a_usart_no, unint8_t * ap_str,
                                     unint8_t a_str_size);
									  
	
/* ----------------------------------------------------------------------------
[Function Name] : usart_read_str_mark
[Description] : This function is responsible for receiving a string of data 
through UART
-the string end is defined by a particular character which the function will
stop receiving when it sees it and then replaces it with null'\0'
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
extern void usart_read_str_mark (unint8_t a_usart_no, unint8_t * ap_str, 
                                     unint8_t a_mark);

/* ----------------------------------------------------------------------------
[Function Name] : usart_write_str_size
[Description] : This function sends a string of data through UART
[NOTE] THAT THIS FUNCTION SHOULD BE ONLY USED IN THE POLLING MODE
(IN INTERRUPT THESENDING OR RECEIVING METHOD IS SO APPLICATION-DEPENDENT AND
IT'S DIFFICULT TO GET A STANDARD WAY OF DOING IT)
[Args] :
[in] -unint8_t a_usart_no
	this argument shall indicate which USART peripheral that's being initialized.
	it's in this form: USARTn (n is the number of peripheral starting from 0)
[in] -const uint8_t * ap_str
	this ARG is responsible for pointing to the first character of the string
[in] -uint8_t a_size
	this ARG is responsible for holding the informing the function about the size
	of the string(number of characters that will be sent).
----------------------------------------------------------------------------*/
extern void usart_write_str_size (unint8_t usart_no, const unint8_t * ap_str,
                                      unint8_t a_size);

/* -----------------------------------------------------------------------------
[Function Name] : usart_write_str_mark
[Description] : This function sends a string of data through UART
the string end is defined by a particular character which the function will stop
sending (after) sending this character
[NOTE] -THAT THIS FUNCTION SHOULD BE ONLY USED IN THE POLLING MODE
(IN INTERRUPT THESENDING OR RECEIVING METHOD IS SO APPLICATION-DEPENDENT AND
IT'S DIFFICULT TO GET A STANDARD WAY OF DOING IT)
[Args] :
[in] -unint8_t a_usart_no
	this argument shall indicate which USART peripheral that's being initialized.
	it's in this form: USARTn (n is the number of peripheral starting from 0)
[in] -conts unint8_t * ap_str
	this ARG points at the first character of the string
[in] -uint8_t a_mark
	this ARG is  holds the informing of the function about the mark which will 
	identify the end of the string.
-----------------------------------------------------------------------------*/
extern void usart_write_str_mark (unint8_t usart_no, const unint8_t * str, unint8_t mark);

/* ----------------------------------------------------------------------------
[Function Name] : usart_set_rx_int
[Description] : This function enables or disables the RX interrupt 
[Args] :
[in] -unint8_t a_usart_no
	this argument shall indicate which USART peripheral that's being initialized.
	it's in this form: USARTn (n is the number of peripheral starting from 0)
[in] -bool * b_state
	this argument indicates the interrupt new state that'll be assigned
	options : true - false
-----------------------------------------------------------------------------*/
extern void usart_set_rx_int (unint8_t a_usart_no, bool b_state);

/* ----------------------------------------------------------------------------
[Function Name] : usart_set_tx_int
[Description] : This function enables or disables the RX interrupt
[Args] :
[in] -unint8_t a_usart_no
	this argument shall indicate which USART peripheral that's being initialized.
	it's in this form: USARTn (n is the number of peripheral starting from 0)
[in] -bool * b_state
	this argument indicates the RX interrupt new state that'll be assigned
	options : true - false
-----------------------------------------------------------------------------*/
extern void usart_set_tx_int (unint8_t usart_no, bool b_state);

/* ----------------------------------------------------------------------------
[Function Name] : usart_set_udre_int
[Description] : This function enables or disables the UDR interrupt
[NOTE] : using this function might remove the application abstraction feature as
UDRE is not common among all MCUs
[Args] :
[in] -unint8_t a_usart_no
	this argument shall indicate which USART peripheral that's being initialized.
	it's in this form: USARTn (n is the number of peripheral starting from 0)
[in] -bool * b_state
	this argument indicates the UDR interrupt new state that'll be assigned
	options : true - false
-----------------------------------------------------------------------------*/
void usart_set_udre_int (uint8_t a_usart_no, bool b_state);

/* -----------------------------------------------------------------------------
[Function Name] : usart_set_tx_isr
[Description] : This function assigns a handler to the UDRE complete interrupt
[NOTE] : using this function might remove the application abstraction feature as
UDRE is not common among all MCUs
[Args] :
[in] -unint8_t a_usart_no
	this argument shall indicate which USART peripheral that's being initialized.
	it's in this form: USARTn (n is the number of peripheral starting from 0)
[in] -void ( * p_usart_udre_function)(void)
	this argument holds the UDRE complete interrupt handler that will be called upon
	the interrupt
-----------------------------------------------------------------------------*/
void usart_set_udr_isr (uint8_t usart_no, void ( * p_usart_tx_function)(void));

/* -----------------------------------------------------------------------------
[Function Name] : usart_set_rx_isr
[Description] : This function assigns a handler to the RX complete interrupt
[Args] :
[in] -unint8_t a_usart_no
	this argument shall indicate which USART peripheral that's being initialized.
	it's in this form: USARTn (n is the number of peripheral starting from 0)
[in] -bool * b_state
	this argument indicates the RX interrupt state
	options : true - false
[in] -void ( * p_usart_rx_function)(void)
	this argument holds the Rx interrupt handler that will be called upon
	RX complete interrupt 
-----------------------------------------------------------------------------*/
extern void usart_set_rx_isr (unint8_t usart_no, void ( * p_usart_rx_function)(void));

/* -----------------------------------------------------------------------------
[Function Name] : usart_set_tx_isr
[Description] : This function assigns a handler to the TX complete interrupt
[Args] :
[in] -unint8_t a_usart_no
	this argument shall indicate which USART peripheral that's being initialized.
	it's in this form: USARTn (n is the number of peripheral starting from 0)
[in] -void ( * p_usart_tx_function)(void)
	this argument holds the Rx interrupt handler that will be called upon
	TX complete interrupt
-----------------------------------------------------------------------------*/
extern void usart_set_tx_isr (unint8_t usart_no, void ( * p_usart_tx_function)(void));

/* -----------------------------------------------------------------------------
[Macro Name] : USART_DATA_READY
[Description] : This Macro detects whether data has been received or not
[Args] :
[in] -unint8_t usart_no
	this argument shall indicate which USART peripheral that's being initialized.
	it's form: USARTn (n is the number of peripheral starting from 0)
------------------------------------------------------------------------------*/
#define USART_DATA_READY(usart_no)  usart_no ## _DATA_READY()//CAT(usart_no, _DATA_READY())

/* -----------------------------------------------------------------------------
[Macro Name] : USART_RX_BUF
[Description] : This Macro is responsible for getting the received frame directly
	without polling 
[Args] :
[in] -unint8_t usart_no
	this argument shall indicate which USART peripheral we want to read its frame
------------------------------------------------------------------------------*/
#define USART_RX_BUF(usart_no)  usart_no ## _RX()//CAT(USART0, _RX())


#endif /* UART_CONFIG_H_ */