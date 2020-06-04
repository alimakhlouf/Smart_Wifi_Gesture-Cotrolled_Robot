/*
 * buttons.h
 *
 *  Created on: ?? ‘⁄»«‰° ???? Â‹
 *      Author: hppp
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"


/*NOTICE THAT THESE FUNCTIONS ARE ONLY USED FOR BUTTONS THAT ACT LIKE SEQUENTIAL INPUTS BECAUSE
COMBINANTIAL INPUTS THEY ONLY REQUIRE SETING THE STATE OF THE INPUT ONLY
*/
#define TRUE 1
#define FALSE 0

uint8 BUTTON_PDCheck_ON(uint8 port_pin_reg, uint8 pin_n, uint8 *flag);
uint8 BUTTON_PDCheck_OFF(uint8 port_pin_reg, uint8 pin_n, uint8 *flag);
uint8 BUTTON_PUCheck_ON(uint8 port_pin_reg, uint8 pin_n, uint8 *flag);
uint8 BUTTON_PUCheck_OFF(uint8 port_pin_reg, uint8 pin_n, uint8 *flag);


#endif /* BUTTONS_H_ */
