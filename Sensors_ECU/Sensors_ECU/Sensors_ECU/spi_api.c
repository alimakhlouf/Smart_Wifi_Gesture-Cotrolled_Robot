/*
 * spi_api.c
 *
 * Created: 6/5/2019 06:27:08 م
 *  Author: hppp
 */ 

#include "spi_api.h"

void spi_send(const uint8_t spi_num, const unint8_t *Str, const unint8_t size)
{
	unint8_t i = 0;
	
	while (i < size)
	{
		spi_exchange(spi_num, Str[i]);
		i++;
	}
}

//keep sending until you reach the marked byte (don't send it)
void spi_send_until(const uint8_t spi_num, const unint8_t *Str, const unint8_t mark)
{
	unint8_t i = 0;
	
	while(Str[i] != mark)
	{
		spi_exchange(spi_num, Str[i]);
		i++;
	}
	
}

void spi_read(const uint8_t spi_num, unint8_t *Str, const unint8_t size)
{
	unsigned char i = 0;
	
	while (i < size)
	{
		Str[i] = spi_exchange(spi_num, 0x00);
		i++;
	}
	
}

//keep reading until you reach the marked byte (and read it too )
void spi_read_until(const uint8_t spi_num, unint8_t *Str, const unint8_t mark)
{
	uint8_t i = 0;
	
	do
	{
		Str[i] = spi_exchange(spi_num, 0x00);
	}
	while (Str[i++] != mark);
	
}




