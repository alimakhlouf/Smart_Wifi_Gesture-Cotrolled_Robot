/*
 * delay_api.h
 *
 * Created: 6/13/2019 03:05:35 م
 *  Author: hppp
 */ 


#ifndef DELAY_API_H_
#define DELAY_API_H_


#include "micro_config.h"
#include "math.h"
#include "delay.h"

static inline void delay_usec(double us_delay)
{
	delay_micro(us_delay);
}

static inline void delay_msec(double ms_delay)
{
	delay_milli(ms_delay);
}


#endif /* DELAY_API_H_ */