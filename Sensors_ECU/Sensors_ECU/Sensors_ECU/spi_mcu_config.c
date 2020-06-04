
#include "micro_config.h"
#include "spi_mcu_config.h"

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


uint8_t spi_init_master(uint8_t spi_num, uint32_t bit_rate, uint8_t phase, uint8_t polarity, uint8_t data_order)
{	
	if (spi_num == SPI0)
	{
		
		if (bit_rate == (F_CPU/2))
		{
			SPCR &= ~(3 << SPR0);
			SPSR |= 1 << SPI2X;
		}
		else if (bit_rate == (F_CPU/4))
		{
			SPCR &= ~(3 << SPR0);
			SPSR &= ~(1 << SPI2X);
		}
		else if (bit_rate == (F_CPU/8))
		{
			SPCR &= ~(1 << SPR1);
			SPCR |= (1 << SPR0);
			SPSR |= 1 << SPI2X;
		}
		else if (bit_rate == (F_CPU/16))
		{
			SPCR &= ~(1 << SPR1);
			SPCR |= (1 << SPR0);
			SPSR &= ~(1 << SPI2X);
		}
		else if (bit_rate == (F_CPU/32))
		{
			SPCR &= ~(1 << SPR0);
			SPCR |= (1 << SPR1);
			SPSR |= (1 << SPI2X);
		}
		else if (bit_rate == (F_CPU/64))
		{
			SPCR &= ~(1 << SPR0);
			SPCR |= (1 << SPR1);
			SPSR &= ~(1 << SPI2X);
		}
		else if (bit_rate == (F_CPU/64))
		{
			SPCR |= (3 << SPR0);
			SPSR &= ~(1 << SPI2X);
		}
		else
		{
			return ERROR;
		}
		
		DIO_SET_DIR(MOSI, DIO_OUTPUT);
		DIO_SET_DIR(SS, DIO_OUTPUT);
		DIO_SET_DIR(SCK, DIO_OUTPUT);
		DIO_SET_DIR(MISO, DIO_INPUT);
		
		//SPI_DIR  |= (1 << MOSI) | (1 << SCK) | (1 << SS);
		//SPI_DIR &= ~(1 << MISO);
		SPCR = (1 << SPE) | (data_order << DORD) | (1 << MSTR) | 
	         	(polarity << CPOL) | (phase << CPHA);
	}
	else
	{
		return ERROR;
	}
	
	return SUCCESS;


#if 0
	DIO_SET_DIR(MOSI, DIO_INPUT);
	DIO_SET_DIR(SS, DIO_INPUT);
	DIO_SET_DIR(SCK, DIO_INPUT);
	DIO_SET_DIR(MISO, DIO_OUTPUT);
	
	//SPI_DIR  &= ~((1 << MOSI)|(1<<SCK)|(1<<SS));
	//SPI_DIR |= (1 << MISO);
	SPCR = (1 << SPE)|(DATA_ORDER << DORD)|(POLARITY << CPOL)|(PHASE << CPHA);
#endif

}

void spi_init_slave(uint8_t spi_num, uint8_t phase, uint8_t polarity, uint8_t data_order)
{
	DIO_SET_DIR(MOSI, DIO_INPUT);
	DIO_SET_DIR(SS, DIO_INPUT);
	DIO_SET_DIR(SCK, DIO_INPUT);
	DIO_SET_DIR(MISO, DIO_OUTPUT);
	
	SPCR = (1 << SPE) | (data_order << DORD) | (polarity << CPOL) | (phase << CPHA);
}

uint8_t spi_exchange(uint8_t spi_num, uint8_t sent_byte)
{
	SPDR = sent_byte; //send datA by SPI
	
	if (spi_int_stat(SPI0) == false)
	{
		while(BIT_IS_CLEAR(SPSR,SPIF))
		{
			
		} //wait until SPI flag becomes 1 (data is sent correctly)
		
		return SPDR;
	}
	
	return SUCCESS;
}



void spi_set_int(uint8_t spi_num, bool b_state)
{
	if (spi_num == SPI0)
	{
		SPCR = (b_state ? SPCR | (1 << SPIE) : SPCR & ~(1 << SPIE));
	}
	else
	{
		
	}
	
}

void spi_set_isr(uint8_t spi_num, void ( * p_spi_function)(void))
{
	
	if (spi_num == SPI0)
	{
		gp_spi_isr = p_spi_function;
	}
	else
	{
		
	}
	
}
