/*
 * button.h
 *
 * Created: 5/11/2019 04:20:45 م
 *  Author: hppp
 */ 


#ifndef BUTTON_H_
#define BUTTON_H_

#include "micro_config.h"
#include "DIO.h"

#define PULL_UP 0
#define PULL_DOWN 1


uint8_t is_port_pin_pd_pressed (volatile uint8_t * pin_reg, uint8_t pin_no);
uint8_t is_port_pin_pu_pressed (volatile uint8_t * pin_reg, uint8_t pin_no);

uint8_t button_state (uint8_t PU_PD, uint8_t dio_pin_no);

#endif /* BUTTON_H_ */