/*
 * spi_api.c
 *
 * Created: 6/5/2019 06:27:08 م
 *  Author: hppp
 */ 

#include "spi_api.h"

void spi_send_str_mark(const unint8_t *Str, unint8_t mark)
{
	unint8_t i = 0;
	
	while(Str[i] != mark)
	{
		spi_send(Str[i]);
		i++;
	}
	
}

void spi_send_str_size(const unint8_t *Str, unint8_t size)
{
	unint8_t i = 0;
	
	while (i < size)
	{
		spi_send(Str[i]);
		i++;
	}
}

void spi_read_str_mark(unint8_t *Str, unint8_t mark)
{
	unsigned char i = 0;
	
	do
	{
		Str[i] = spi_read();
	}
	while (Str[i++] != mark);
	
	//i--;  /*return back one step to the last cell where we put null*/
	//Str[i] = '\0';
}


void spi_read_str_size(unint8_t *Str, unint8_t size)
{
	unsigned char i = 0;
	
	while (i < size)
	{
		Str[i] = spi_read();
		i++;
	}
	
}


