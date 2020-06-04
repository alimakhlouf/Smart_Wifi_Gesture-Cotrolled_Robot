/* --------------------------------------------------------------------------
[FILE NAME]:    spi_api.c

[AUTHOR]:       Ali MAkhlouf

[DATE CREATED]:  6/5/2019 06:27:08 م

[DESCRIPTION]:  this file contains function definitions at the application 
                layer for handling SPI peripheral
----------------------------------------------------------------------------*/

#include "spi_api.h"

void spi_send_str_mark(const unint8_t * ap_str, unint8_t a_mark)
{
	//used for accessing string elements one by one
	unint8_t count = 0;
	
	//while we didn't reach end of ap_str character
	while(ap_str[count] != a_mark)
	{
		spi_send(ap_str[count]);
		 count++;
	}
	
}

void spi_send_str_size(const unint8_t * ap_str, unint8_t a_size)
{
	//used for accessing string elements one by one
	unint8_t count = 0;
	
	//while the loop counter didn't reach the a_size of the ap_string
	while (count < a_size)
	{
		spi_send(ap_str[count]);
		 count++;
	}
}

void spi_read_str_mark(unint8_t * ap_str, unint8_t a_mark)
{
	//used for accessing string elements one by one 
	unint8_t count = 0;
	
	do
	{
		
		ap_str[count] = spi_read();
	}//while we didn't reach end of ap_str character
	while (ap_str[ count++] != a_mark);
	
	//i--;  /*return back one step to the last cell where we put null*/
	//ap_str[count] = '\0';
}


void spi_read_str_size(unint8_t * ap_str, unint8_t a_size)
{
	//used for accessing ap_string elements one by one 
	unint8_t count = 0;
	
	//while the loop counter didn't reach the a_size of the ap_string
	while (count < a_size)
	{
		ap_str[count] = spi_read();
		count++;
	}
	
}


