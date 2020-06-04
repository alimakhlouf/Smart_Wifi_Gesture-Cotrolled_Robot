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
#include "string.h"
#include "uart_config.h"

/****************************************NOTE****************************
 * in this function we have to use floating point arithmetic to calculate the value of UBBR
 * register which is responsible for controlling the baud rate of the peripheral, we use
 *floating point to insure that the number is rounded to the closest integer as there is a big
 * difference in baud rate if we put for example 5 in UBBR register or 6 in UBBR register
 *I used fixed point arithmetic instead of floating point to avoid the massive floating point
 * libraries and long time it takes ,fixed p. is just done by splitting an integer register
 *into an integer bits and a precision bits, here we want just one precision bit because
 * 0.1 in binary = 0.5 in decimal, so by using only one precision bit we get to know if we
 *must round up or down
 *NOTICE THAT IN A FIXED POINT ARITMETIC ALL OPERANDS MUST HAVE THE NUMBER OF PRECISION BITS
 */
/****************************************/



#define UART_PRECISION_BITS 1

#if (DOUBLE_SPEED==1)
#define FIXEDP_FACTOR_8_16 (unint32_t)(8 << UART_PRECISION_BITS)
#else
#define FIXEDP_FACTOR_8_16 (unint32_t)(16 << UART_PRECISION_BITS)
#endif


#define CPU_FREQ_IN_FIXEDP (unint32_t)(F_CPU << UART_PRECISION_BITS)  //F_CPU is split for 1 bit precision and the rest are integer bits




#define FIXEDP_BAUD_RATE (unint32_t)((unint32_t)BAUD_RATE << UART_PRECISION_BITS)//BAUD_RATE is split for 1 bit precision and the rest are integer bits




				
/* ---------------------------------------------------------------------------------------------------------------
[MACRO Name] : MULTIPLY_FIXED_POINT
[Description] : This function is responsible multiplying 2 fixed point numbers
				the fixed point multiplication goes like this x=(y*z)>>PRECISION_BITS (we shift right by the number of precision bits because multiplying 2 fixed numbers THE RESULT HAS DOUBLE THENUMBER OF PRECISIONN BITS SO WE HALF IT BY SHIFTING THE NUMBER OF PROCISION BITS)
[Args] :
[in] -A
this ARG is the first operand in the multiplication
-B
this ARG is the second operand in the multiplication
-PRECISION_BITS
this ARG is the number of the precision bits
---------------------------------------------------------------------------------------------------------------*/
#define MULTIPLY_FIXED_POINT_UART(A, B, precision_bits) (((A) * (B)) >> (precision_bits))


/* ---------------------------------------------------------------------------------------------------------------
[MACRO Name] : DIVIDE_FIXED_POINT
[Description] : This function is responsible dividing 2 fixed point numbers
				the fixed point multiplication goes like this x=((y<<1)*z) (we shift y left by the number of precision bits because dividing 2 fixed numbers , the numbers will remove each ithers precision bits (just like 20/30) so the result won't contain any precision bits and will be integer number we avoid this step by shifting the first operand left by the number of precision bits
[Args] :
[in] -A
this ARG is the first operand(a fixed point number) in the division
-B
this ARG is the second operand(a fixed point number) in the division
-PRECISION_BITS
this ARG is the number of the precision bits
---------------------------------------------------------------------------------------------------------------*/
#define DIVIDE_FIXED_POINT_UART(A, B, precision_bits)(((A) << (precision_bits)) / (B))




/* ---------------------------------------------------------------------------------------------------------------
[Function Name] : UART_init
[Description] : This function is responsible for initializing UART with all of it's characteristics such as it's baud rate
				, double speed , transmit enable and it's interrupt, receive enable and it's interrupt , stop bits , parity bit
				synchronisation mode and data size. all these configurations are set previously by #define and handled at pre-compile time
[Args] :
[int] -all the previous #define configurations
---------------------------------------------------------------------------------------------------------------*/
void UART_init(void);


/* ---------------------------------------------------------------------------------------------------------------
[Function Name] : UART_sendChr
[Description] : This function is responsible for sending a byte of data through UART
[Args] :
[int] -const unint8 data
this ARG is responsible for holding the character that will be displayed
---------------------------------------------------------------------------------------------------------------*/
void UART_sendChr(const unint8_t data);


/* ---------------------------------------------------------------------------------------------------------------
[Function Name] : UART_receiveChr
[Description] : This function is responsible for receiving a byte of data
[Args] :

[Return]: returns the byte received
---------------------------------------------------------------------------------------------------------------*/
unint8_t UART_receiveChr(void);


/* ---------------------------------------------------------------------------------------------------------------
[Function Name] : UART_sendStrByMark
[Description] : This function is responsible for sending a string of data through UART
				the string end is defined by a particular character which the function will stop sending when it sees it

[Args] :
[in] -const char *str
this ARG is responsible for pointing to the first character of the string
-unint8 mark
this ARG is responsible for holding the informing the function about the mark which will identify the end of the string
---------------------------------------------------------------------------------------------------------------*/
void UART_sendStrByMark(const unint8_t *str,unint8_t mark);


/* ---------------------------------------------------------------------------------------------------------------
[Function Name] : UART_sendStrBySize
[Description] : This function is responsible for sending a string of data through UART
				the string size(number of characters it's going to send) is passed to it so the function will stop sending when it reaches the size
				NOTICE THAT THIS FUNCTION SHOULD BE ONLY USED IN THE POLLING MODE (IN INTERRUPT THESENDING OR RECEIVING METHOD IS SO APPLICATION-DEPENDENT AND IT'S IMPOSSIBLE TOGET A STANDARD WAY OF DOING IT  SO I COULDN'T DO STR SEND OR RECEIVE FOR THEM)

[Args] :
[in] -const char *str
this ARG is responsible for pointing to the first character of the string
-unint8 size
this ARG is responsible for holding the informing the function about the size of the string(number of characters that will be sent)
---------------------------------------------------------------------------------------------------------------*/
void UART_sendStrBySize(const unint8_t *Str,unint16_t size); // Receive until certain mark


/* ---------------------------------------------------------------------------------------------------------------
[Function Name] : UART_receiveStrByMark
[Description] : This function is responsible for receiving a string of data through UART
				the string end is defined by a particular character which the function will stop receiving when it sees it and then replace it with null'\0'
				NOTICE THAT THIS FUNCTION SHOULD BE ONLY USED IN THE POLLING MODE (IN INTERRUPT THESENDING OR RECEIVING METHOD IS SO APPLICATION-DEPENDENT AND IT'S IMPOSSIBLE TO GET A STANDARD WAY OF DOING IT  SO I COULDN'T DO STR SEND OR RECEIVE FOR THEM)

[Args] :
[in] -const char *str
this ARG is responsible for pointing to the first character of the string that will be received
-unint8 mark
this ARG is responsible for holding the informing the function about the mark which will identify the end of the string
---------------------------------------------------------------------------------------------------------------*/
void UART_receiveStrByMark(unint8_t *str,unint8_t mark);


/* ---------------------------------------------------------------------------------------------------------------
[Function Name] : UART_receiveStrBySize
[Description] : This function is responsible for receiving a string of data through UART
				the string size(number of characters it's going to received) is passed to it so the function will stop receiving when it reaches the size
				NOTICE THAT THIS FUNCTION SHOULD BE ONLY USED IN THE POLLING MODE (IN INTERRUPT THESENDING OR RECEIVING METHOD IS SO APPLICATION-DEPENDENT AND IT'S IMPOSSIBLE TO GET A STANDARD WAY OF DOING IT  SO I COULDN'T DO STR SEND OR RECEIVE FOR THEM)


[Args] :
[in] -const char *str
this ARG is responsible for pointing to the first character of the string that will be received
-unint8 size
this ARG is responsible for holding the size of the string(number of characters that will be sent)
---------------------------------------------------------------------------------------------------------------*/
void UART_receiveStrBySize(unint8_t *str,unint16_t size);

#if(RECEIVE_COMPLETE_INTERRUP==1)
	extern volatile unint8_t uart_rxc_flag;
#endif

#if(TRANSMIT_COMPLETE_INTERRUP==1)
	extern volatile unint8_t uart_txc_flag;
#endif

#if(DATA_REGISTER_EMPTY_INTERRUP==1)
	extern volatile unint8_t uart_empty_flag;
#endif

#endif



