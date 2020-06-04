
#include "i2c.h"

#if(I2C_INTERRUPT == 1)
	ISR(TWI_vect)
	{
		/* interrupt code here*/
		
	

		TWCR &= ~((1<<TWIE)|(1<<TWINT));/*this statement is used to disable the interrupt in the i2c(normally is put before
										leaving the ISR)as the TWINT will be set and an interrupt will happen continuously
										unless we clear it before leaving ISR(if you removed the (1<<twint)part the master will
										accidentally start sending data*/ 
		
	}
#endif
void TWI_init(void)
{   
	long freq;
	
	#if(SCL_FREQ >= MINIMUM_SCL_FREQ_WHEN_TWPS_EQUALS_0) //if the goal FREQ is >= than the minimum frequency with the prescaler is 1
		TWSR |= (0<<TWPS0); 
	#elif(SCL_FREQ >= MINIMUM_SCL_FREQ_WHEN_TWPS_EQUALS_1)//WHEN the goal FREQ is >= than the minimum frequency with THE PRESCALER IS 4^1= 4
		TWSR |= (1<<TWPS0);
	#elif(SCL_FREQ >= MINIMUM_SCL_FREQ_WHEN_TWPS_EQUALS_2)//WHEN the goal FREQ is >= than the minimum frequency with THE PRESCALER IS 4^2=16
		TWSR |= (2<<TWPS0);
	#else		//WHEN the goal FREQ is >= than the minimum frequency with THE PRESCALER IS 4^3= 64
		TWSR |= (3<<TWPS0);
	#endif

	freq=lrint(((F_CPU/(float)SCL_FREQ)-16)/(pow(4,TWPS_VALUE)*2)); // THE SECOND ARG. IN POW IS THE VALUE
	TWBR=(int)freq;
	TWAR = TWI_ADDRESS; // my address 
	TWCR=(1<<TWEN); //ENABLE I2C PERIPHERAL
}

uint8_t TWI_getStatus(void)
{
    uint8_t status;
    /* masking to eliminate first 3 bits and get the last 5 bits (status bits) */
    status = TWSR & 0xF8;
    return status;
}



#if(TWI_NTERRUPT ==0)
void TWI_start(void)
{
	/* 
	 * Clear the TWINT flag before sending the start bit TWINT=1
	 * send the start bit by TWSTA=1
	 * Enable TWI Module TWEN=1 
	 */
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    /* Wait for TWINT flag set in TWCR Register (start bit is send successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
}

void TWI_stop(void)
{
    /* 
	 * Clear the TWINT flag before sending the stop bit TWINT=1
	 * send the stop bit by TWSTO=1
	 * Enable TWI Module TWEN=1 
	 */
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}


void TWI_write(uint8_t data)
{
    /* Put data On TWI data Register */
    TWDR = data;
    /* 
	 * Clear the TWINT flag before sending the data TWINT=1
	 * Enable TWI Module TWEN=1 
	 */ 
    TWCR = (1 << TWINT) | (1 << TWEN);
	
	//this is for polling
    /* Wait for TWINT flag set in TWCR Register (data is send successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
}


uint8_t TWI_readWith_ACK(void) //this function returns the byte read not the state of the operation
{
	/* 
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable sending ACK after reading or receiving data TWEA=1
	 * Enable TWI Module TWEN=1 
	 */ 
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
    /* Read Data */
    return TWDR;
}


uint8_t TWI_readWith_NACK(void) //this function returns the byte read not the state of the operation
{
	/* 
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable TWI Module TWEN=1 
	 */
    TWCR = (1 << TWINT) | (1 << TWEN);
    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
    /* Read Data */
    return TWDR;
}
#else 
/***********************************************************/
/****************IF WE ARE USING INTERRUPTS*******************/
	
void TWI_start(uint8_t interrupt)
{
	/* 
	 * Clear the TWINT flag before sending the start bit TWINT=1
	 * send the start bit by TWSTA=1
	 * Enable TWI Module TWEN=1 
	 */
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (interrupt<<TWIE);
}


void TWI_stop(uint8_t interrupt)
{
    /* 
	 * Clear the TWINT flag before sending the stop bit TWINT=1
	 * send the stop bit by TWSTO=1
	 * Enable TWI Module TWEN=1 
	 */
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN) | (interrupt<<TWIE);
}


void TWI_write(uint8_t data,uint8_t interrupt)
{
    /* Put data On TWI data Register */
    TWDR = data;
    /* 
	 * Clear the TWINT flag before sending the data TWINT=1
	 * Enable TWI Module TWEN=1 
	 */ 
    TWCR = (1 << TWINT) | (1 << TWEN) | (interrupt<<TWIE);
}


void TWI_readWith_ACK(uint8_t interrupt)
{
	/* 
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable sending ACK after reading or receiving data TWEA=1
	 * Enable TWI Module TWEN=1 
	 */ 
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA) | (interrupt<<TWIE);

}


void TWI_readWith_NACK(uint8_t interrupt)
{
	/* 
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable TWI Module TWEN=1 
	 */
    TWCR = (1 << TWINT) | (1 << TWEN) | (interrupt<<TWIE);
}
#endif
