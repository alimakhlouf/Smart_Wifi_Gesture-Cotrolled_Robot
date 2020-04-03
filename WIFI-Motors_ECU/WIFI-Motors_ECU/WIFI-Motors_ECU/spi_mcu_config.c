
#include "micro_config.h"
#include "spi_mcu_config.h"

static void ( * gp_spi_isr)(void);

void CONFIG_SPI_ISR() // finished
{
	//-----------------------------------------------------------------------
	// we need to Global interrupt to prevent nested interrupt .
	cli();
	//-----------------------------------------------------------------------

	gp_spi_isr();

	sei();
}


void spi_init()
{	
#if (SPI_NODE_TYPE == SPI_MASTER)
	DIO_SET_DIR(MOSI, DIO_OUTPUT);
	DIO_SET_DIR(SS, DIO_OUTPUT);
	DIO_SET_DIR(SCK, DIO_OUTPUT);
	DIO_SET_DIR(MISO, DIO_INPUT);
	
	//SPI_DIR  |= (1 << MOSI) | (1 << SCK) | (1 << SS);
	//SPI_DIR &= ~(1 << MISO);
	SPCR = (1 << SPE) | (DATA_ORDER << DORD) | 
	       (1 << MSTR) | (POLARITY << CPOL) | (PHASE << CPHA) | (SPR0_1 << SPR0);
	SPSR = (SPI2X << SPI2X);
#else //(SPI_NODE_TYPE == SPI_SLAVE)

	DIO_SET_DIR(MOSI, DIO_INPUT);
	DIO_SET_DIR(SS, DIO_INPUT);
	DIO_SET_DIR(SCK, DIO_INPUT);
	DIO_SET_DIR(MISO, DIO_OUTPUT);
	
	//SPI_DIR  &= ~((1 << MOSI)|(1<<SCK)|(1<<SS));
	//SPI_DIR |= (1 << MISO);
	SPCR = (1 << SPE)|(DATA_ORDER << DORD)|(POLARITY << CPOL)|(PHASE << CPHA);
#endif

}


void spi_send(unint8_t data)
{
	SPDR = data; //send datA by SPI
	
 	if (spi_int_stat() == false)
 	{
		while(BIT_IS_CLEAR(SPSR,SPIF))
		{
			
		} //wait until SPI flag becomes 1 (data is sent correctly)
	}
	
}


unint8_t spi_read(void)   /*here the slave must write the byte after that the master calls this function*/   
{
	
#if (SPI_NODE_TYPE == SPI_MASTER)
	spi_send('1');
#else 
	if (spi_int_stat() == 0)
	{
		
		while(BIT_IS_CLEAR(SPSR,SPIF))
		{
		
		} //wait until SPI interrupt flag=1 (datA is sent correctly)
		
	}
	
#endif

	return SPDR;
}

void spi_set_int(bool b_state)
{
	SPCR = (b_state ? SPCR | (1 << SPIE) : SPCR & ~(1 << SPIE));
}

void spi_set_isr(void ( * p_spi_function)(void))
{
	gp_spi_isr = p_spi_function;
}
