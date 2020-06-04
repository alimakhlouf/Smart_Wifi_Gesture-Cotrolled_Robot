/* ----------------------------------------------------------------------------
[FILE NAME]: uart_mcu_config.h

[AUTHOR(S)]: Ali Makhlouf

[DATE CREATED]: 15/6/2016

[DESCRIPTION]: library header file for UART serial communication in the 
               HAL layer
----------------------------------------------------------------------------- */


#ifndef UART_LIBRARY
#define UART_LIBRARY

#include "common_macros.h"
#include "micro_config.h"
#include "std_types.h"

#include "string.h"
#include "stdbool.h"
#include "fixed_point.h"
#include "uart_api.h" 

/****************************************NOTE**********************************
 * in this function we have to use floating point arithmetic to calculate the 
 * value of UBBR register which is responsible for controlling the baud rate of
 * the peripheral, we use *floating point to insure that the number is rounded
 * to the closest integer as there is a big difference in baud rate if we put
 * for example 5 in UBBR register or 6 in UBBR register I used fixed point 
 * arithmetic instead of floating point to a void the massive floating point
 * libraries and long time it takes ,fixed p. is just done by splitting an 
 * integer register into an integer bits and a precision bits, here we want just
 * one precision bit because 0.1 in binary = 0.5 in decimal, so by using only 
 * one precision bit we get to know if we must round up or down
 * NOTICE THAT IN A FIXED POINT ARITMETIC ALL OPERANDS MUST HAVE THE SAME NUMBER
 * OF PRECISION BITS
 */
/****************************************/




// the following version of configurations for ATMEGA32-16-8
#if defined(ATMEGA8) || defined(ATMEGA32) || defined(ATMEGA16)
/* BIT 1*/
/* 1 for double speed - 0 to disable double speed*/
#define DOUBLE_SPEED 1

#define UART_PRECISION_BITS 1

//CPU frequency is split for 1 bit precision and the rest are integer bits
#define FIXP_FREQ (unint32_t)FIXP_CONVERT(F_CPU, UART_PRECISION_BITS)  

//BAUD_RATE is split for 1 bit precision and the rest are integer bits
#define FIXP_BAUD FIXP_CONVERT(a_baud_rate, UART_PRECISION_BITS)

#if (DOUBLE_SPEED == 1)
#define FIXP_8_16 (unint32_t)FIXP_CONVERT(8, UART_PRECISION_BITS)
#else
#define FIXP_8_16 (unint32_t)FIXP_CONVERT(16, UART_PRECISION_BITS)
#endif

//used #define indicates which uart to address
#define UART0  0
  
//used #define indicates TX-RX status
#define UART_TX 1
#define UART_RX 2
#define TX_RX   3

//#defines that indicates PARITY options
#define NO_PARITY   0
#define EVEN_PARITY 2
#define ODD_PARITY  3

//used #define indicates stop_bits options
#define ONE_STOP_BIT 0
#define TWO_STOP_BIT 1

//indicates data_size options
#define DATA_SIZE_5 0
#define DATA_SIZE_6 1
#define DATA_SIZE_7 2
#define DATA_SIZE_8 3
#define DATA_SIZE_9 7
        
//indicates sample phases options		
#define TX_RISE_RX_FALL 0
#define TX_FALL_RX_RISE 1

//uart_modes options
#define ASYNCH  0
#define SYNCH   1

#if 0
// UASRT_REGISTERS
//=============================================================================
#define UDR    *((volatile uint8 *)(0x2C))	//	 UASRT DATA REGISTER
#define UCSRA  *((volatile uint8 *)(0x5B))	//	 UASRT
#define UCSRB  *((volatile uint8 *)(0x2A))  
#define UCSRC  *((volatile uint8 *)(0x40))  // same as UBRRH
#define UBRRH  *((volatile uint8 *)(0x40))  // UART BAUD RATE REGISTER.
#define UBRRL  *((volatile uint8 *)(0x29))
//==============================================================================
#endif
// UCSRA Bits configuration .
//------------------------------------------------------------------------------


  
#if 0
#define RXC  7
#define TXC  6
#define UDRE 5
#define FE   4
#define DOR  3
#define PE   2
#define U2X  1
#endif
#if 0
#define RXC_BIT				(uint8_t)(get_bit(UCSRA,7)) // UART Receive Complete flag
#define RXC(value)			assig_bit(UCSRA,7,value)
#define TXC_BIT				(uint8_t)(get_bit(UCSRA,6)) // UART Transmit Complete flag
#define TXC(value)			assig_bit(UCSRA,6,value)
#define UDRE_BIT			(uint8_t)get_bit(UCSRA,5) // UART Data Register Empty flag
#define FE_BIT              (uint8_t)(get_bit(UCSRA,4)) // FE: Frame Error flag
#define DOR_BIT             (uint8_t)(get_bit(UCSRA,3)) // DOR: DataOverRun          flag
#define PE_BIT              (uint8_t)(get_bit(UCSRA,2)) // PE: Parity Error          flag
#define U2X(value)          assig_bit(UCSRA,1,value)  // U2X: Double the UART Transmission Speed enable
#define MPCM(value)			assig_bit(UCSRA,0,value)  // MPCM: Multi-processor Communication Mode enable
#endif
//==================================================================================
// UCSRB Bits configuration .
//----------------------------------------------------------------------------------
#define RXEN_TXEN 3

#if 0
#define RXCIE 7
#define TXCIE 6
#define UDRIE 5
#define RXEN  4
#define TXEN  3
#define UCSZ2 2
#define RXB8  1
#define TXB8  0
#endif

#if 0
#define RXCIE(value)          assig_bit(UCSRB,7,value) // RX Complete Interrupt Enable
#define	RXCIE_BIT			  (uint8_t)(get_bit(UCSRB,7))
#define TXCIE(value)          assig_bit(UCSRB,6,value) // TX Complete Interrupt Enable
#define TXCIE_BIT	          (uint8_t)(get_bit(UCSRB,6))
#define UDRIE(value)          assig_bit(UCSRB,5,value) // UART Data Register Empty Interrupt Enable
#define UDRIE_BIT			  (uint8_t)(get_bit(UCSRB,5))
#define RXEN(value)           assig_bit(UCSRB,4,value) // UART Receive Enable
#define TXEN(value)           assig_bit(UCSRB,3,value) // UART Transmit Enable
#define UCSZ2(value)		  assig_bit(UCSRB,2,value) // UCSZ2: Character Size (will be used with UCSZ1:0  at UCSRC)
#define RXB8_BIT		      (uint8_t)(get_bit(UCSRB,1))// RXB8: Received Data Bit 8
#define TXB8(value)		      assig_bit(UCSRB,0,value) // TXB8: Transmitted Data Bit 8
#endif
//===================================================================================
// UCSRC Bits configuration .
//-----------------------------------------------------------------------------------
#define UCSZ_0_1 1
#if 0

#define URSEL    7
#define UMSEL    6
#define UPM1     5
#define UPM0     4
#define USBS     3
#define UCSZ1    2
#define UCSZ0    1
#define UCPOL    0
#endif

#define UPM_0_1  4

#if 0
#define URSEL(value)          assig_bit(UCSRC_REG,7,value) // URSEL: Register Select (UBRRH OR UCSRC)
#define UMSEL(value)		  assig_bit(UCSRC_REG,6,value) // UMSEL: UART Mode Select
#define UPM1(value)		      assig_bit(UCSRC_REG,5,value) // UPM1 : Parity Mode
#define UPM0(value)		      assig_bit(UCSRC_REG,4,value) // UPM0 : Parity Mode
#define USBS(value)		      assig_bit(UCSRC_REG,3,value) // USBS : Stop Bit Select
#define UCSZ1(value)		  assig_bit(UCSRC_REG,2,value) // UCSZ1: Character Size (will be used with UCSZ0  at UCSRC,UCSZ2  at UCSRB)
#define UCSZ0(value)		  assig_bit(UCSRC_REG,1,value) // UCSZ0: Character Size (will be used with UCSZ1  at UCSRC,UCSZ2  at UCSRB)
#define UCPOL(value)		  assig_bit(UCSRC_REG,0,value) // UCPOL: Clock Polarity
#endif

/**************************************/

#ifdef ATMEGA8
//define CONFIG_TXC_ISR as TX complete interrupt handler
#define CONFIG_TXC_ISR   __vector_13
void CONFIG_TXC_ISR(void) __attribute__((signal, __INTR_ATTRS));

//define CONFIG_RXC_ISR as RX complete interrupt handler
//-----------------------------------------------------------------------------
#define CONFIG_RXC_ISR   __vector_11
void CONFIG_RXC_ISR(void) __attribute__((signal, __INTR_ATTRS));
//-----------------------------------------------------------------------------
//define CONFIG_UDRE_ISR as UDRE complete interrupt handler
#define CONFIG_UDRE_ISR   __vector_12
void CONFIG_UDRE_ISR(void) __attribute__((signal, __INTR_ATTRS));
//------------------------------------------------------

#elif defined(ATMEGA32)
//define CONFIG_TXC_ISR as TX complete interrupt handler
#define CONFIG_TXC_ISR   __vector_15
void CONFIG_TXC_ISR(void) __attribute__((signal, __INTR_ATTRS));
// Receive complete interrupt.
//-----------------------------------------------------------------------------
//define CONFIG_RXC_ISR as RX complete interrupt handler
#define CONFIG_RXC_ISR   __vector_13
void CONFIG_RXC_ISR(void) __attribute__((signal, __INTR_ATTRS));
//-----------------------------------------------------------------------------
//define CONFIG_UDRE_ISR as UDRE complete interrupt handler
#define CONFIG_UDRE_ISR   __vector_14
void CONFIG_RXC_ISR(void) __attribute__((signal, __INTR_ATTRS));
//-----------------------------------------------------------------------------

#elif defined(ATMEGA16)
//define CONFIG_TXC_ISR as TX complete interrupt handler
#define CONFIG_TXC_ISR   __vector_13
void CONFIG_TXC_ISR(void) __attribute__((signal, __INTR_ATTRS));
//-----------------------------------------------------------------------------
//define CONFIG_RXC_ISR as the RX complete interrupt handler
#define CONFIG_RXC_ISR   __vector_11
void CONFIG_RXC_ISR(void) __attribute__((signal, __INTR_ATTRS));
//-----------------------------------------------------------------------------
//define CONFIG_UDRE_ISR as UDRE complete interrupt handler
#define CONFIG_UDRE_ISR   __vector_12
void CONFIG_UDRE_ISR(void) __attribute__((signal, __INTR_ATTRS));
//------------------------------------------------------
#endif

/* -----------------------------------------------------------------------------
[Macro Name] : UART0_DATA_READY
[Description] : This Macro indicates whether data has been received on UART 
				peripheral number 0(which in these controllers it's the only one
				available in the case of the speicified controllers)
------------------------------------------------------------------------------*/
#define UART0_DATA_READY() (BIT_IS_SET(UCSRA, RXC)) 

/* -----------------------------------------------------------------------------
[Macro Name] : UART0_DATA_READY
[Description] : This Macro indicates whether data has been received on UART 
				peripheral number 0
------------------------------------------------------------------------------*/
/* -----------------------------------------------------------------------------
[Macro Name] : UART0_RX
[Description] : This Macro is responsible for getting a received frame directly
	without polling from UART peripheral number 0
------------------------------------------------------------------------------*/
#define UART0_RX() (UDR)
 
//=============================================================================
#endif //#if defined(ATMEGA8) || defined(ATMEGA16) || defined(ATMEGA32)

/* -----------------------------------------------------------------------------
[Function Name] : uart_data_ready
[Description] : This function indicates whether data has been received or not
[Args] :
[in] -unint8_t a_uart_no
this argument shall indicate which UART peripheral that's being initialized.
it's in this form: UARTn (n is the number of peripheral starting from 0)
[in] -uint8_t a_data
this argument is responsible for holding the character that will be displayed
------------------------------------------------------------------------------*/
bool uart_data_ready(unint8_t uart_no);

/* ----------------------------------------------------------------------------
[Function Name] : UART_init
[Description] : This function is responsible for initializing UART with all of 
                it's characteristics such as it's baud rate, receive enable, 
				transmit enable sampling phase, receive enable and it's 
				interrupt , stop bits , parity bit synchronizations mode and 
				data size.
[Args] :
[in] -unint8_t a_uart_no
	this argument shall indicate which UART peripheral that's being initialized.
	it's in this form: UARTn (n is the number of peripheral starting from 0)
[in] -uint32_t a_baud_rate
	this argument indicates the baud rate that the UART peripheral will work on.
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
void uart_init (uint8_t uart_no, uint32_t baud_rate, uint8_t data_size, 
                    uint8_t a_tx_or_rx, uint8_t a_parity, uint8_t a_stop_bits,
				        uint8_t a_sample_phase);

/* -----------------------------------------------------------------------------
[Function Name] : uart_write
[Description] : This function is responsible for sending a byte of data through
UART
[Args] :
[in] -unint8_t a_uart_no
	this argument shall indicate which UART peripheral that's being initialized.
	it's in this form: UARTn (n is the number of peripheral starting from 0)
[in] -uint8_t a_data
	this argument is responsible for holding the character that will be displayed
------------------------------------------------------------------------------*/
void uart_write (uint8_t a_uart_no, uint8_t a_data);

/* -----------------------------------------------------------------------------
[Function Name] : uart_read
[Description] : This function is responsible for receiving a byte of data
[Args] :
[in] -unint8_t a_uart_no
	this argument shall indicate which UART peripheral that's being initialized.
	it's in this form: UARTn (n is the number of peripheral starting from 0)
[Return]: returns the data_bits received
------------------------------------------------------------------------------*/
uint8_t uart_read (uint8_t uart_no);

/* ----------------------------------------------------------------------------
[Function Name] : uart_set_rx_int
[Description] : This function enables or disables the RX interrupt
[Args] :
[in] -unint8_t a_uart_no
	this argument shall indicate which UART peripheral that's being initialized.
	it's in this form: UARTn (n is the number of peripheral starting from 0)
[in] -bool * b_state
	this argument indicates the interrupt state
	options : true - false
-----------------------------------------------------------------------------*/
void uart_set_rx_int (uint8_t uart_no, bool state);

/* ----------------------------------------------------------------------------
[Function Name] : uart_set_tx_int
[Description] : This function enables or disables the RX interrupt
[Args] :
[in] -unint8_t a_uart_no
	this argument shall indicate which UART peripheral that's being initialized.
	it's in this form: UARTn (n is the number of peripheral starting from 0)
[in] -bool * b_state
	this argument indicates the RX interrupt state
	options : true - false
-----------------------------------------------------------------------------*/
void uart_set_tx_int (uint8_t uart_no, bool state);

/* ----------------------------------------------------------------------------
[Function Name] : uart_set_udre_int
[Description] : This function enables or disables the UDR interrupt
[NOTE] : using this function might remove the application abstraction feature as
UDRE is not common among all MCUs
[Args] :
[in] -unint8_t a_uart_no
	this argument shall indicate which UART peripheral that's being initialized.
	it's in this form: UARTn (n is the number of peripheral starting from 0)
[in] -bool * b_state
	this argument indicates the UDR interrupt state
	options : true - false
-----------------------------------------------------------------------------*/
void uart_set_udre_int (uint8_t a_uart_no, bool b_state);

/* -----------------------------------------------------------------------------
[Function Name] : uart_set_rx_isr
[Description] : This function assigns a handler to the RX complete interrupt
[Args] :
[in] -unint8_t a_uart_no
	this argument shall indicate which UART peripheral that's being initialized.
	it's in this form: UARTn (n is the number of peripheral starting from 0)
[in] -bool * b_state
	this argument indicates the RX interrupt state
	options : true - false
[in] -void ( * p_uart_rx_function)(void)
	this argument holds the Rx interrupt handler that will be called upon
	RX complete interrupt
-----------------------------------------------------------------------------*/
void uart_set_rx_isr (uint8_t uart_no, void ( * p_uart_rx_function)(void));

/* -----------------------------------------------------------------------------
[Function Name] : uart_set_udre_isr
[Description] : This function assigns a handler to the UDRE complete interrupt
[NOTE] : using this function might remove the application abstraction feature as
UDRE is not common among all MCUs
[Args] :
[in] -unint8_t a_uart_no
	this argument shall indicate which UART peripheral that's being initialized.
	it's in this form: UARTn (n is the number of peripheral starting from 0)
[in] -void ( * p_uart_udre_function)(void)
	this argument holds the UDRE complete interrupt handler that will be called upon
	the interrupt
-----------------------------------------------------------------------------*/
void uart_set_udr_isr (uint8_t uart_no, void ( * p_uart_tx_function)(void));

#endif

