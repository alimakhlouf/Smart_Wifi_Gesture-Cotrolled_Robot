#include "spi.h"

#if(SPI_INTERRUPT == 1)
volatile unint8_t spi_flag;
ISR(SPI_STC_vect)
{
	
}
#endif



void SPI_init()
{	
#if(SPI_NODE_TYPE == SPI_MASTER)
	SPI_DIR  = (1 << MOSI) | (1 << SCK) | (1 << SS);
	SPI_DIR &= ~(1 << MISO);
	SPCR = (SPI_INTERRUPT << SPIE) | (1 << SPE) | (DATA_ORDER << DORD) |
	(1 << MSTR) | (POLARITY << CPOL) | (PHASE << CPHA) | (SPR1_0_PRESCALER << SPR0);
	SPSR = (SPI2X_PRESCALER << SPI2X);
#else //(SPI_NODE_TYPE == SPI_SLAVE)
	SPI_DIR  &= ~((1 << MOSI)|(1<<SCK)|(1<<SS));
	SPI_DIR = (1<<MISO);
	SPCR = (SPI_INTERRUPT << SPIE)|(1 << SPE)|(DATA_ORDER << DORD)|(POLARITY << CPOL)|(PHASE << CPHA);
#endif

}


void SPI_sendChr(unint8_t data)
{
	SPDR = data; //send data by SPI
	while(BIT_IS_CLEAR(SPSR,SPIF)){} //wait until SPI interrupt flag=1 (data is sent correctly)
}



void SPI_sendStrByMark(const unint8_t *Str, unint8_t mark)
{
	unint8_t i = 0;
	while(Str[i] != mark)
	{
		SPI_sendChr(Str[i]);
		i++;
	}
}

void SPI_sendStrBySize(const unint8_t *Str, unint8_t size)
{
	unint8_t i = 0;
	
	while (i < size)
	{
		SPI_sendChr(Str[i]);
		i++;
	}
}

void SPI_receiveStrByMark(unint8_t *Str, unint8_t mark)
{
	unsigned char i = 0;
	
	do
	{
		Str[i] = SPI_receiveChr();
	}
	while (Str[i++] != mark);
	
	i--;  /*return back one step to the last cell where we put null*/
	Str[i] = '\0';
}


void SPI_receiveStrBySize(unint8_t *Str, unint8_t size)
{
	unsigned char i = 0;
	
	while (i < size)
	{
		Str[i] = SPI_receiveChr();
		i++;
	}
	
	Str[i] = '\0';
}


unint8_t SPI_receiveChr(void)   /*here the slave must write the byte after that the master calls this function*/   
{
	
#if (SPI_NODE_TYPE == SPI_MASTER)
	SPI_sendChr('1');
#else 
	while(BIT_IS_CLEAR(SPSR,SPIF)){} //wait until SPI interrupt flag=1 (data is sent correctly)
#endif

	return SPDR;
}