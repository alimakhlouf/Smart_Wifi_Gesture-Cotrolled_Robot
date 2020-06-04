/*
 * MPU_prog.c
 *
 * Created: 4/15/2019 07:40:05 م
 * Author : hppp
 */ 

#include "micro_config.h"
#include "i2c.h" 
#include "uart.h"
#include "spc.h"
#include "millis0.h"
#include "lcd_4bit.h"
#include "buttons.h"


#define PWR_MGMT_1  107
#define PWR_MGMT_2  108
#define INT_PIN_CFG 55
#define ACCEL_CONFIG 28
#define INT_ENABLE 56
#define INT_STATUS 58
#define ACCEL_XOUT_H 59

#define L 1
#define R 2
#define F 3
#define B 4
#define S 0



void alarm();
void esp_init();

void mpu_write_byte(uint8_t reg_add, uint8_t data);
void mpu_read_burst(uint8_t reg_add, uint8_t * data, uint8_t size);
uint8_t mpu_read_byte(uint8_t reg_add);

int main(void)
{
	DDRB &= ~(0b00111110);
	PORTB |=  0b00111110; 
	
	uint8_t direction = S;
	
	
	
	
#if 0
	uint8_t accel[6];
	uint16_t accel_y;
	uint16_t accel_z;
	uint16_t accel_x;
	
	DDRB &= ~(1 << 0);
	
	TWI_init();
	
	mpu_write_byte(PWR_MGMT_1, 0b00100000); //disable sleep, temperature ... and enable cycle
	
	mpu_write_byte(PWR_MGMT_2, 0b11000000); // wake up 40 times a second ... and activate every sensor
	
	mpu_write_byte(INT_PIN_CFG, 0b00100000); //configuring the int interrupt to be active high .. push-pull ... held high
											 // and cleared by reading the int status reg
	
	mpu_write_byte(ACCEL_CONFIG , 0); //configuring the full scale to +-2g
	
	mpu_write_byte(INT_ENABLE, 1); //activate ready
#endif
	
    /* Replace with your application code */
    while (1) 
    {
		
		if (BIT_IS_CLEAR(PINB, 1))
		{
			
			if (direction != R)
			{
				esp_send(":DR", 3);
			}
			
		}
		else if (BIT_IS_CLEAR(PINB, 2))
		{
			
			if (direction != F)
			{
				esp_send(":DF", 3);
			}
			
		}
		else if (BIT_IS_CLEAR(PINB, 3))
		{
			
			if (direction != B)
			{
				esp_send(":DB", 3);
			}
			
		}
		else if (BIT_IS_CLEAR(PINB, 4))
		{
			
			if (direction != L)
			{
				esp_send(":DL", 3);
			}
			
		}
		else if (BIT_IS_CLEAR(PINB, 5))
		{
			
			if (direction != S)
			{
				esp_send(":DS", 3);
			}
			
		}
		
#if 0
		if (PINB & (1 << 0))
		{
			mpu_read_byte(INT_STATUS);
			mpu_read_burst(ACCEL_XOUT_H, accel, 6);
			UART_sendStrByMark("accelerometer is read", 0);
			accel_x = (accel[1]) | ((uint16_t)accel[0] << 8);
			accel_y = (accel[3]) | ((uint16_t)accel[2] << 8);
			accel_z = (accel[5]) | ((uint16_t)accel[4] << 8);
		}
#endif
		
		
		
    }
	
}

void mpu_write_byte(uint8_t reg_add, uint8_t data)
{
	TWI_start();
	TWI_write(0b11010000); 
	TWI_write(reg_add);
	TWI_write(data); 
	TWI_stop();
}

uint8_t mpu_read_byte(uint8_t reg_add)
{
	uint8_t temp;
	TWI_start();
	TWI_write(0b11010000);
	TWI_write((reg_add << 1));
	TWI_start();
	TWI_write(0b11010001);
	temp = TWI_readWith_NACK();
	TWI_stop();	
	
	return temp;
}

void mpu_read_burst(uint8_t reg_add, uint8_t * data, uint8_t size)
{
	uint8_t i = 0;
	TWI_start();
	TWI_write(0b11010000);
	TWI_write((reg_add << 1));
	TWI_start();
	TWI_write(0b11010001);
	
	while (i < (size - 1))
	{
		data[i] = TWI_readWith_ACK();
		i++ ;
	}
	
	data[i] = TWI_readWith_NACK();
	
	TWI_stop();
}


void esp_init()
{
	UART_sendStrByMark("AT\r\n", 0);
	
	if (check_string("OK\r\n", 2000))
	{
		PORTB ^= 1;
		_delay_ms(5);
		
	}
	else
	{
		alarm();
	}
	
	
	UART_sendStrByMark("AT+CWMODE=0\r\n", 0);
	
	if (check_string("OK\r\n", 2000))
	{
		PORTB ^= 1;
		_delay_ms(5);
	}
	else
	{
		alarm();
	}
	
	
	UART_sendStrByMark("AT+CIPMUX=1\r\n", 0);
	if (check_string("OK\r\n", 2000))
	{
		PORTB ^= 1;
		_delay_ms(5);
	}
	else
	{
		alarm();
	}
	
	
	UART_sendStrByMark("AT+CIFSR\r\n", 0);
	
	if (check_string("OK\r\n", 2000))
	{
		PORTB ^= 1;
		_delay_ms(5);
	}
	else
	{
		alarm();
	}
	
	UART_sendStrByMark("AT+CIPSTART=\"TCP\",\"192.168.1.108\",80\r\n");
	
	if (check_string("OK\r\n", 2000))
	{
		PORTB ^= 1;
		_delay_ms(5);
	}
	else
	{
		alarm();
	}
	
}

void alarm()
{
	
	for (int i = 0; i < 30; i++)
	{
		PORTB ^= 1;
		_delay_ms(200);
	}
	
}

void esp_send(uint8_t * str, uint8_t data_size_str)
{
	UART_sendStrByMark("AT+CIPSEND=", 0);
	UART_sendStrByMark(data_size_str, 0);
	UART_sendStrByMark("\r\n", 0);
	
	if (check_string("OK\r\n", 2000))
	{
		//PORTB ^= 1;
		_delay_ms(5);
		
	}
	else
	{
		alarm();
	}
	

	UART_sendStrBySize(str, data_size_str);
	UART_sendStrByMark("\r\n", 0);
}

