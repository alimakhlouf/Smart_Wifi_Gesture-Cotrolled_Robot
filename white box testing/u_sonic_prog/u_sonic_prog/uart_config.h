/*
 * uart_config.h
 *
 * Created: 4/18/2019 12:07:19 م
 *  Author: hppp
 */ 


#ifndef UART_CONFIG_H_
#define UART_CONFIG_H_

//PERIPHERAL SETTINGS

/*****enter the baud rate****/
#define BAUD_RATE 9600
/****************************/



/*****************UCSRA CONFIGURATIONS**********/

/* BIT 1*/
/* 1 for double speed - 0 to disable double speed*/
#define DOUBLE_SPEED 1


/************************************************/



/**************UCSRB CONFIGURATIONS*************/
/*Transmit enable*/
/* 1 enable - 0 disable */
#define TRANSMIT_ENABLE 1


/*receive enable*/
/* 1 enable - 0 disable */
#define RECIEVE_ENABLE 1

/*data register empty interrupt */
/* 1 enable - 0 disable */
#define DATA_REGISTER_EMPTY_INTERRUP 0


/*transmit complete interrupt*/
/* 1 enable - 0 disable */
#define TRANSMIT_COMPLETE_INTERRUP 0

/*receive complete interrupt*/
/* 1 enable - 0 disable */
#define RECEIVE_COMPLETE_INTERRUP 0
/*********************************************************/


/*******************UCSRC CONFIGURATIONS********************/
/*UART MODE
0 for Asynchronous mode
1 for synchronous mode
*/
#define SYNCH_ASYNCH 0


/*PARITY BIT 
 0 disable
 2 even parity
 3 odd parity 
 */
#define PARITY 0


/* NUMBER OF STOP BITS
0 for one stop bit mode 
1 for two stop bits mode 
*/
#define STOP_BITS 0


/*CHOOSE DATABITS SIZE
0 FOR 5 BITS
1 FOR 6 BITS
2 FOR 7 BITS
3 FOR 8 BITS
*/
#define DATA_SIZE 3


/* CLOCK POLARITY -USED FOR SYNCH ONLY
0  TRANSMIT CHANGE ON RISING EDGE ,RECEIVE SAMPLE ON FALLING EDGE 
1  REANSMIT CHANGE ON FALLING EDGE ,RECEIVE SAMPLE ON RISING EDGE 
*/
#define TRANS_SAMPLE_CONFIG 0
/***************************************/




#endif /* UART_CONFIG_H_ */