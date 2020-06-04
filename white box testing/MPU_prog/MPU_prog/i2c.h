#ifndef i2c_receive
#define i2c_receive


#include <math.h>
#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

/* I2C Status Bits in the TWSR Register */
#define TW_START         0x08 // start has been sent
#define TW_REP_START     0x10 // repeated start
#define TW_MT_SLA_W_ACK  0x18 // Master transmit ( slave address + Write request ) to slave + Ack received from slave
#define TW_MT_SLA_R_ACK  0x40 // Master transmit ( slave address + Read request ) to slave + Ack received from slave
#define TW_MT_DATA_ACK   0x28 // Master transmit data and ACK has been received from Slave.
#define TW_MR_DATA_ACK   0x50 // Master received data and send ACK to slave
#define TW_MR_DATA_NACK  0x58 // Master received data but doesn't send ACK to slave

#define MINIMUM_SCL_FREQ_WHEN_TWPS_EQUALS_0  F_CPU/(16+2*255) /*means that the minimum SCL freq reached when applying the 
															   minimum precscaler so that if the goal freq is less the result we
															   increase the prescaler*/

#define MINIMUM_SCL_FREQ_WHEN_TWPS_EQUALS_1  F_CPU/(16+2*255*4)
#define MINIMUM_SCL_FREQ_WHEN_TWPS_EQUALS_2  F_CPU/(16+2*255*16)																   
#define MINIMUM_SCL_FREQ_WHEN_TWPS_EQUALS_3  F_CPU/(16+2*255*64)																   
#define TWPS_VALUE (TWSR &0X03) 	
   
/**********ENTER THE SCL FEQUENCY(IN BYTES)*********/
#define SCL_FREQ 50000  //means 4000000 Kb/s
/*****************************/


/***apply device I2c address (even if master)*****/
#define TWI_ADDRESS 00000010 


/****INTERRUPT COFIG***
 DISABLE = 0
 ENABLE  = 1
*/
#define TWI_INTERRUPT 0

/*BECAUSE THERE IS NO STANDARD SEQUENCE OF WRITING A BYTE OR STRING WE ARE NOT GOING TO MAKE
 * FUNCTIONS OF THEM INSTEAD THESE FUNCTIONS SHOULD BE PUT IN THE DEVICES(OR MODULES) CODE SO IT
 *  CAN APPLY IT'S OWN SEQUENCE OF OPERATIONS IN IT */

/*also the acknowledgment of the operations can't be put inside the I2c.c functions as
 *there are more that one status for every operation depending on the sequence*/


/*************PROTOTYPES***************************/
#if(TWI_INTERRUPT == 0)
	void TWI_init(void);
	void TWI_start(void); /*returns 1 if success and 0 if failed*/
	void TWI_stop(void);
	void TWI_write(uint8_t data); /*returns 1 if success and 0 if failed*/
	uint8_t TWI_readWith_ACK(void);/*read with send Ack,this,the function returns the byte read not the state of the operation*/
	uint8_t TWI_readWith_NACK(void);/*read without send NAck,the function returns the byte read not the state of the operation*/
	uint8_t TWI_getStatus(void);

#else
	void TWI_Init(void);
	void TWI_Start(uint8_t interrupt);
	void TWI_Stop(uint8_t interrupt);
	void TWI_write(uint8_t data,uint8_t interrupt);
	void TWI_readWith_ACK(uint8_t interrupt); /*read with send Ack*/
	void TWI_readWith_NACK(uint8_t interrupt); /*read without send Ack*/
	uint8_t TWI_getStatus(void);	
	uint8_t TWI_Get_Status(void);
#endif
	
	
	
#endif
