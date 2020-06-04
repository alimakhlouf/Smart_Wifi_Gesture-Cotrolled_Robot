/* ---------------------------------------------------------------------------------------------------
[FILE NAME]: uart.h
[AUTHOR(S)]: Ali Makhlouf
[DATE CREATED]: 15/6/2016
[DESCRIPTION]: library header file for UART serial communication
--------------------------------------------------------------------------------------------------- */


#ifndef UART_LIBRARY
#define UART_LIBRARY



#include "common_macros.h"
#include "micro_config.h"
#include "std_types.h"
#include "millis_api.h"
#include "string.h"
#include "stdbool.h"
#include "fixed_point.h"
#include "uart_api.h" 



/****************************************NOTE****************************
 * in this function we have to use floating point arithmetic to calculate the value of UBBR
 * register which is responsible for controlling the baud rate of the peripheral, we use
 *floating point to insure that the number is rounded to the closest integer as there is a big
 * difference in baud rate if we put for example 5 in UBBR register or 6 in UBBR register
 *I used fixed point arithmetic instead of floating point to a void the massive floating point
 * libraries and long time it takes ,fixed p. is just done by splitting an integer register
 *into an integer bits and a precision bits, here we want just one precision bit because
 * 0.1 in binary = 0.5 in decimal, so by using only one precision bit we get to know if we
 *must round up or down
 *NOTICE THAT IN A FIXED POINT ARITMETIC ALL OPERANDS MUST HAVE THE NUMBER OF PRECISION BITS
 */
/****************************************/



/* BIT 1*/
/* 1 for double speed - 0 to disable double speed*/
#define DOUBLE_SPEED 1



#define UART_PRECISION_BITS 1


#define FIXP_FREQ (unint32_t)FIXP_CONVERT(F_CPU, UART_PRECISION_BITS)  //F_CPU is split for 1 bit precision and the rest are integer bits




#define FIXP_BAUD FIXP_CONVERT(baud_rate, UART_PRECISION_BITS)//BAUD_RATE is split for 1 bit precision and the rest are integer bits

#if (DOUBLE_SPEED == 1)
#define FIXP_8_16 (unint32_t)FIXP_CONVERT(8, UART_PRECISION_BITS)
#else
#define FIXP_8_16 (unint32_t)FIXP_CONVERT(16, UART_PRECISION_BITS)
#endif


#if defined(ATMEGA8) || defined(ATMEGA32) || defined(ATMEGA16)
#define NUM_USARTS 1
        
#define USART0  0
  
#define UART_TX 1
#define UART_RX 2
#define TX_RX   3

#define NO_PARITY   0
#define EVEN_PARITY 2
#define ODD_PARITY  3

#define ONE_STOP_BIT 0
#define TWO_STOP_BIT 1

#define DATA_SIZE_5 0
#define DATA_SIZE_6 1
#define DATA_SIZE_7 2
#define DATA_SIZE_8 3
#define DATA_SIZE_9 7

#define TX_RISE 0
#define TX_FALL 1

#define ASYNCH  0
#define SYNCH   1

#if 0
// UASRT_REGISTERS
//================================================================================
#define UDR    *((volatile uint8 *)(0x2C))	//	 UASRT DATA REGISTER
#define UCSRA  *((volatile uint8 *)(0x5B))	//	 UASRT
#define UCSRB  *((volatile uint8 *)(0x2A))
#define UCSRC  *((volatile uint8 *)(0x40))  // same as UBRRH
#define UBRRH  *((volatile uint8 *)(0x40))  // USART BAUD RATE REGISTER.
#define UBRRL  *((volatile uint8 *)(0x29))
//=================================================================================
#endif
// UCSRA Bits configuration .
//---------------------------------------------------------------------------------


  
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
#define RXC_BIT				(uint8_t)(get_bit(UCSRA,7)) // USART Receive Complete    flag
#define RXC(value)			assig_bit(UCSRA,7,value)
#define TXC_BIT				(uint8_t)(get_bit(UCSRA,6)) // USART Transmit Complete   flag
#define TXC(value)			assig_bit(UCSRA,6,value)
#define UDRE_BIT			(uint8_t)get_bit(UCSRA,5) // USART Data Register Empty flag
#define FE_BIT              (uint8_t)(get_bit(UCSRA,4)) // FE: Frame Error           flag
#define DOR_BIT             (uint8_t)(get_bit(UCSRA,3)) // DOR: DataOverRun          flag
#define PE_BIT              (uint8_t)(get_bit(UCSRA,2)) // PE: Parity Error          flag
#define U2X(value)          assig_bit(UCSRA,1,value)  // U2X: Double the USART Transmission Speed enable
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
#define UDRIE(value)          assig_bit(UCSRB,5,value) // USART Data Register Empty Interrupt Enable
#define UDRIE_BIT			  (uint8_t)(get_bit(UCSRB,5))
#define RXEN(value)           assig_bit(UCSRB,4,value) // USART Receive Enable
#define TXEN(value)           assig_bit(UCSRB,3,value) // USART Transmit Enable
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
#define UMSEL(value)		  assig_bit(UCSRC_REG,6,value) // UMSEL: USART Mode Select

#define UPM1(value)		      assig_bit(UCSRC_REG,5,value) // UPM1 : Parity Mode
#define UPM0(value)		      assig_bit(UCSRC_REG,4,value) // UPM0 : Parity Mode
#define USBS(value)		      assig_bit(UCSRC_REG,3,value) // USBS : Stop Bit Select
#define UCSZ1(value)		  assig_bit(UCSRC_REG,2,value) // UCSZ1: Character Size (will be used with UCSZ0  at UCSRC,UCSZ2  at UCSRB)
#define UCSZ0(value)		  assig_bit(UCSRC_REG,1,value) // UCSZ0: Character Size (will be used with UCSZ1  at UCSRC,UCSZ2  at UCSRB)
#define UCPOL(value)		  assig_bit(UCSRC_REG,0,value) // UCPOL: Clock Polarity
#endif

/***************************************/
#ifdef ATMEGA8
// Transmit complete interrupt.
#define CONFIG_TXC_ISR   __vector_13
void CONFIG_TXC_ISR(void) __attribute__((signal, __INTR_ATTRS));

// Receive complete interrupt.
//-----------------------------------------------------------------------------------
#define CONFIG_RXC_ISR   __vector_11
void CONFIG_RXC_ISR(void) __attribute__((signal, __INTR_ATTRS));
//-----------------------------------------------------------------------------------

#elif defined(ATMEGA32)
// Transmit complete interrupt.
#define CONFIG_TXC_ISR   __vector_15
void CONFIG_TXC_ISR(void) __attribute__((signal, __INTR_ATTRS));
// Receive complete interrupt.
//-----------------------------------------------------------------------------------
#define CONFIG_RXC_ISR   __vector_13
void CONFIG_RXC_ISR(void) __attribute__((signal, __INTR_ATTRS));
//-----------------------------------------------------------------------------------

#elif defined(ATMEGA16)
// Transmit complete interrupt.
#define CONFIG_TXC_ISR   __vector_13
void CONFIG_TXC_ISR(void) __attribute__((signal, __INTR_ATTRS));
// Receive complete interrupt.
//-----------------------------------------------------------------------------------
#define CONFIG_RXC_ISR   __vector_11
void CONFIG_RXC_ISR(void) __attribute__((signal, __INTR_ATTRS));
//-----------------------------------------------------------------------------------
#endif


//===================================================================================
#endif //#if MCU_TPE == ATMEGA8 .... etc

inline bool usart_data_ready(unint8_t usart_no)
{
	uint8_t result = 0;
	
	if (USART0 == usart_no)
	{
		result = BIT_IS_SET(UCSRA, RXC);
	}
	
	return result;
}

void usart_init (uint8_t usart_no, uint32_t baud_rate, uint8_t data_size,uint8_t tx_or_rx, uint8_t parity, uint8_t stop_bits,                    uint8_t sample_phase);
void usart_write (uint8_t usart_no, uint8_t data);
uint8_t usart_read (uint8_t usart_no);
void usart_set_rx_int (uint8_t usart_no, bool state);
void usart_set_tx_int (uint8_t usart_no, bool state);
void usart_set_rx_isr (uint8_t usart_no, void ( * p_usart_rx_function)(void));
void usart_set_tx_isr (uint8_t usart_no, void ( * p_usart_tx_function)(void));


//bool usart_data_ready(unint8_t usart_no);


void wait_to_finish(unint8_t t);

char check_string(char * str, unint16_t millis_time);

unint8_t get_str_until(char * dest_str, char * afterstr, unint16_t delay_time);

unint8_t get_string(char * dest_str, char * beforestr, char * afterstr, unint16_t time_d);

unint8_t is_str_reached(char * str, unint8_t tmp1, unint8_t * tmp);

uint8_t check_strings (unint16_t millis_time, int16_t str_count, ...);


#endif



//the fixed point multiplication goes like this x=(y*z)>>PRECISION_BITS (we shift right by the number of precision bits because multiplying 2 fixed numbers
