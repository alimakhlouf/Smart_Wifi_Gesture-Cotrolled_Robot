/* ----------------------------------------------------------------------------
[FILE NAME]:    common_macros.h

[AUTHOR]:       Ali MAkhlouf

[DATE CREATED]: 4/2/2014 7:47:21 PM

[DESCRIPTION]:   a library header file containing MACROs an 
				 functions for performing fixed common bit and register 
				 manipulation operations
-----------------------------------------------------------------------------*/
#ifndef COMMON_MACROS
#define COMMON_MACROS
/*
[NOTE] :
	despite the fact that inline functions are preferable in barcoding
	standard, some procedures where written as macros instead
	because functions requires specific size for the operands which is
	not needed in the the case MACROs, so we use MACROs to preform to
	define procedures with variable operands sizes.
*/
#include "micro_config.h"
#include <stdbool.h>

/* -----------------------------------------------------------------------
[MACRO Name] : SET_BIT
[Description] : This function is responsible setting a certain bit in a 
	certain register
[NOTE] :
	I didn't make this macro an inline function(which is preferable in
	barcoding standard) because the function will require specific size for
	the operands which is not needed in the macro	
[Args] :
[in] -reg
	this ARG shall indicate the register that contains the bit
[in] -bit
	this ARG shall indicate the bit location in the register
-----------------------------------------------------------------------------*/
#define SET_BIT(reg, bit) ((reg) |= (1 << (bit)))

/* -----------------------------------------------------------------------
[MACRO Name] : CLEAR_BIT
[Description] : This function is responsible for clearing a certain bit in a 
	certain register
[Args] :
[in] -reg
	this ARG shall indicate the register that contains the bit
[in] -bit
	this ARG shall indicate the bit location in the register
-----------------------------------------------------------------------------*/
#define CLEAR_BIT(reg, bit) ((reg) &= ( ~(1 << (bit))))

/* ----------------------------------------------------------------------------
[MACRO Name] : TOGGLE_BIT
[Description] : This function is responsible for toggling a certain bit in a 
	certain register
[Args] :
[in] -reg
	this ARG shall indicate the register that contains the bit
[in] -bit
	this ARG shall indicate the bit location in the register
-----------------------------------------------------------------------------*/
#define TOGGLE_BIT(reg, bit) ((reg) ^= (1 << (bit)))

/* ----------------------------------------------------------------------------
[function Name] : ror_8bit
[Description] : This function is responsible for performing a right rotation for
	a register a specified number of times
[NOTE] : 
	I used is as inline as it's more preferable(according to barcoding standards)
	as long as the use of macro is not a necessity
[Args] :
[in] -uint8_t a_reg
	this ARG shall indicate the register that'll be rotated
[in] -uint8_t a_num
	this ARG shall indicate the number of rotations
-----------------------------------------------------------------------------*/
inline void ror_8bit(uint8_t a_reg, uint8_t a_num)
{
	a_reg = (a_reg >> a_num) | (a_reg << (8 - a_num));
}
/* ----------------------------------------------------------------------------
[function Name] : rol_8bit
[Description] : This function is responsible for performing a left rotation for
	a register a specified number of times
[Args] :
[in] -reg
	this ARG shall indicate the register that'll be rotated
[in] -num
	this ARG shall indicate the number of rotations
-----------------------------------------------------------------------------*/
inline void rol_8bit(uint8_t a_reg, uint8_t a_num)
{
	a_reg = (a_reg << a_num) | (a_reg >> (8 - a_num));
}

/* ----------------------------------------------------------------------------
[MACRO Name] : BIT_IS_SET
[Description] : This MACRO is responsible for for checking whether a specified
	bit in the string is set or not
[Args] :
[in] -a_reg
	this ARG shall indicate the register that contains the bit
[in] -a_bit
	the location of the bit in the register
[Returns] : this return shall indicate whether the bit is set or not
	non-zero :if it's set
	zero     :if not
-----------------------------------------------------------------------------*/
#define BIT_IS_SET(reg, bit) (((reg) & (1 << (bit))))

/* ----------------------------------------------------------------------------
[MACRO Name] : BIT_IS_CLEAR
[Description] : This MACRO is responsible for for checking whether a specified
	bit in the string is cleared or not
[Args] :
[in] -reg
	this ARG shall indicate the register that contains the bit
[in] -bit
	the location of the bit in the register
[Returns] : this return shall indicate whether the bit is cleared or not
	non-zero :if it's cleared
	zero     :if not
-----------------------------------------------------------------------------*/
#define BIT_IS_CLEAR(reg, bit) ( !((reg) & (1 << (bit))))

/* ----------------------------------------------------------------------------
[Macro Name] : MODULO
[Description] : This function is responsible calculating the remainder resulted
	from a division operation
[Args] :
[in] -dividend
	this ARG shall indicate the dividend operator
[in] -divisor
	the location of the bit in the register.
[Returns] : this return shall indicate the remainder of the division operation.
-----------------------------------------------------------------------------*/
#define MODULO(dividend, divisor) ((dividend) - ((divisor) * ((dividend) / (divisor))))


/* ----------------------------------------------------------------------------
[Macro Name] : CAT
[Description] : This function is responsible concatenating to text with each 
	other, so the compiler will process the resulted combination between them.
[Args] :
[in] -x
	this ARG shall indicate the left side text.
[in] -y
	this ARG shall indicate the right side text.
-----------------------------------------------------------------------------*/
#define CAT(x,y) x ## y 

/* -----------------------------------------------------------------------
[MACRO Name] : ASSIGN_BIT
[Description] : This function is responsible assigning a certain state to
	a specific bit in a register.
[Args] :
[in] -reg
	this ARG shall indicate the register that contains the bit.
[in] -bit
	this ARG shall indicate the bit location in the register.
[in] -state
this ARG shall indicate the state which will be assigned to the register.
-----------------------------------------------------------------------------*/
#define ASSIGN_BIT(reg, bit, state) (reg) = (((reg) & ( ~(1 << (bit)))) | ((state) << (bit)))


#endif
