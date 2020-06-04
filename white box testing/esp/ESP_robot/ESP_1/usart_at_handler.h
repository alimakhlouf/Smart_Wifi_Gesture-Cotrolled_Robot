/*
 * uart_at_handler.h
 *
 * Created: 6/14/2019 10:58:11 ص
 *  Author: hppp
 */ 


#ifndef UART_AT_HANDLER_H_
#define UART_AT_HANDLER_H_

#include "micro_config.h"
#include "millis_api.h"
#include "uart_api.h"
#include <stdarg.h> 

void wait_to_finish(unint8_t t);

char check_string(char * str, unint16_t millis_time);

unint8_t get_str_until(char * dest_str, char * afterstr, unint16_t delay_time);

unint8_t get_string(char * dest_str, char * beforestr, char * afterstr, unint16_t time_d);

unint8_t is_str_reached(char * str, unint8_t tmp1, unint8_t * tmp);

uint8_t check_strings (unint16_t millis_time, int16_t str_count, ...);



#endif /* UART_AT_HANDLER_H_ */