/*
 * uart_config.h
 *
 * Created: 4/18/2019 12:07:19 م
 *  Author: hppp
 */ 


#ifndef UART_CONFIG_H_
#define UART_CONFIG_H_


#include "micro_config.h"
#include "uart_mcu_config.h"
#include "dio_mcu_config.h"
#include "stdbool.h"
#include "std_types.h"
#include <stdbool.h>

#define  USART0 0
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

#if 0
// empty Transmitter data register complete .
//-----------------------------------------------------------------------------------
#define CONFIG_UDRE_ISR   __vector_12
void CONFIG_UDRE_ISR(void) __attribute__((signal, __INTR_ATTRS));
#endif


/*use TX ? RX? OR BOTH
UART_TX 1
UART_RX 2
TX_RX   3

PARITY BIT 
 NO_PARITY disable   0 
 EVEN_PARITY even parity 2
 ODD_PARITY odd parity   3

NUMBER OF STOP BITS
 ONE_STOP_BIT for one stop bit mode  0
 TWO_STOP_BITS for two stop bit mode  1

CHOOSE DATABITS SIZE
 5_DATA_BITS 0 FOR 5 BITS
 6_DATA_BITS 1 FOR 6 BITS
 7_DATA_BITS 2 FOR 7 BITS
 8_DATA_BITS 3 FOR 8 BITS
 9_DATA_BITS 7

CLOCK POLARITY -USED FOR SYNCH ONLY
 TX_RISE_RX_FALL 0  TRANSMIT CHANGE ON RISING EDGE ,RECEIVE SAMPLE ON FALLING EDGE
 TX_RISE_RX_FALL 1  REANSMIT CHANGE ON FALLING EDGE ,RECEIVE SAMPLE ON RISING EDGE 

UART MODE
 ASYNCH for Asynchronous mode
 SYNCH for synchronous mode
*/



extern void usart_init (unint8_t usart_no, uint32_t baud_rate, unint8_t data_size,unint8_t tx_or_rx, unint8_t parity, unint8_t stop_bits, unint8_t sample_phase);



extern void uart_set_usart (unint8_t usart_no, unint8_t mode);
extern void usart_write (unint8_t usart_no, unint8_t data);
extern unint8_t usart_read (unint8_t usart_no);
extern void usart_read_str_size (unint8_t usart_no, unint8_t * str, unint8_t length);
extern void usart_read_str_mark (unint8_t usart_no, unint8_t * str, unint8_t mark);
extern void usart_write_str_size (unint8_t usart_no, unint8_t * str, unint8_t length);
extern void usart_write_str_mark (unint8_t usart_no, unint8_t * str, unint8_t mark);
extern void usart_set_rx_int (unint8_t usart_no, bool b_state);
extern void usart_set_tx_int (unint8_t usart_no, bool b_state);
extern void usart_set_rx_isr (unint8_t usart_no, void ( * p_usart_rx_function)(void));
extern void usart_set_tx_isr (unint8_t usart_no, void ( * p_usart_tx_function)(void));
#define USART_DATA_READY(usart_no) usart_data_ready((usart_no))
								   




#endif /* UART_CONFIG_H_ */