/*
 * buttons.c
 *
 *  Created on: ?? ‘⁄»«‰° ???? Â‹
 *      Author: hppp
 */
#include "buttons.h"

uint8 BUTTON_PDCheck_ON(uint8 port_pin_reg,uint8 pin_n, uint8 *flag)
{
	if (*flag == 0)
		if (port_pin_reg & (1 << pin_n))
		{
			_delay_ms(30);
			if (port_pin_reg & (1 << pin_n))
			{
				*flag=1;
				return TRUE;
			}
		}

	return FALSE;
}


uint8 BUTTON_PDCheck_OFF(uint8 port_pin_reg, uint8 pin_n, uint8 *flag)
{
	if (*flag == 1)
		if (!(port_pin_reg & (1 << pin_n)))
		{
			_delay_ms(30);
			if (!(port_pin_reg & (1 << pin_n)))
			{
				*flag = 0;
				return TRUE;

			}
		}

	return FALSE;
}




uint8 BUTTON_PUCheck_ON(uint8 port_pin_reg,uint8 pin_n, uint8 *flag)
{
	if (*flag == 0)
		if (!(port_pin_reg & (1 << pin_n)))
		{
			_delay_ms(30);
			if (!(port_pin_reg & (1 << pin_n)))
			{
				*flag=1;
				return TRUE;
			}
		}

	return FALSE;
}

uint8 BUTTON_PUCheck_OFF(uint8 port_pin_reg, uint8 pin_n, uint8 *flag)
{
	if (*flag == 1)
		if (port_pin_reg & (1 << pin_n))
		{
			_delay_ms(30);
			if (port_pin_reg & (1 << pin_n))
			{
				*flag = 0;
				return TRUE;

			}
		}

	return FALSE;
}


