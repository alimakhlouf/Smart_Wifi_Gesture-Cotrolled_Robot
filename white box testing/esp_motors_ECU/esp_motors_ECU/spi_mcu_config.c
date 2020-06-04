/* -------------------------------------------------------------------------
[FILE NAME]: millis_mcu_config.c

[AUTHOR]: Ali Makhlouf

[DATE CREATED]: 12/5/2017

[DESCRIPTION]: a library c file for controlling and issuing SPI operations
               it's in the HAL layer
----------------------------------------------------------------------------*/
#include "micro_config.h"
#include "spi_mcu_config.h"


#if defined(ATMEGA8) || defined(ATMEGA16) || defined(ATMEGA32)
static void ( * gp_spi_isr)(void) = NULL;

void CONFIG_SPI_ISR() // finished
{
	//-----------------------------------------------------------------------
	// we need to Global interrupt to prevent nested interrupt .
	cli();
	//-----------------------------------------------------------------------
	
	if (gp_spi_isr != NULL)
	{
		gp_spi_isr();
	}
	

	sei();
}


void spi_init()
{	
	//if it's master set the SPI pins according to that 
#if (SPI_NODE_TYPE == SPI_MASTER)

	DIO_SET_DIR(MOSI, DIO_OUTPUT);
	DIO_SET_DIR(SS, DIO_OUTPUT);
	DIO_SET_DIR(SCK, DIO_OUTPUT);
	DIO_SET_DIR(MISO, DIO_INPUT);
	SPCR = (1 << SPE) | (DATA_ORDER << DORD) | (1 << MSTR) | (POLARITY << CPOL)
	        | (PHASE << CPHA) | (SPR0_1 << SPR0);
	SPSR = (SPI2X << SPI2X);
	   //if it's slave .. set the SPI pins according to that 
#else //(SPI_NODE_TYPE == SPI_SLAVE)

	DIO_SET_DIR(MOSI, DIO_INPUT);
	DIO_SET_DIR(SS, DIO_INPUT);
	DIO_SET_DIR(SCK, DIO_INPUT);
	DIO_SET_DIR(MISO, DIO_OUTPUT);
	SPCR = (1 << SPE)|(DATA_ORDER << DORD)|(POLARITY << CPOL)|(PHASE << CPHA);
#endif

}


void spi_send(unint8_t data)
{
	SPDR = data; //send data by SPI
	
	/*
	[NOTE]: the reason that polling is after the SPI operation not before 
	it(like UART) is that this operation can be used in a read as well(in master mode
	reading from a slave is done by sending a byte to fist it thus their bytes are 
	exchanged so you have to check that the operation(exchange) completed before 
	reading the byte*/ 
	if (false == spi_int_stat())
	{
		
		while(BIT_IS_CLEAR(SPSR, SPIF))
		{
			 //wait until SPI flag becomes 1 (data is sent correctly)
		}
		
	}
	
}

/*[NOTE]: in master mode you can't use this function in the interrupt mode 
the user have to make his own method in the application layer*/
unint8_t spi_read(void)      
{
	//here in the master mode it sends a byte to the slave
#if (SPI_NODE_TYPE == SPI_MASTER)
	spi_send('1');
	//[Note] : we didn't use polling as it's already used in spi_send function
#else  // if it's a slave .. read directly
	
	//if polling method is used execute the polling procedure .. otherwise not
	if (spi_int_stat() == 0)
	{
		
		while(BIT_IS_CLEAR(SPSR,SPIF))
		{
		
		} //wait until SPI interrupt flag=1 (data is received correctly)
		
	}
	
#endif

	return SPDR;
}

void spi_set_int(bool b_state)
{
	//if b_state was true then set the bit otherwise clear it
	SPCR = (b_state ? SPCR | (1 << SPIE) : SPCR & ~(1 << SPIE));
}

void spi_set_isr(void ( * p_spi_function)(void))
{
	/*assign the function that the user wrote to be the SPI ISR to the handler
      function that will be actually called when the interrupt occurs.*/
	gp_spi_isr = p_spi_function;
}

#endif