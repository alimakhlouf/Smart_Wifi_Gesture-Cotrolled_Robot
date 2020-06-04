/*
 * common_macros.h
 *
 * Created: 4/2/2014 7:47:21 PM
 * Author: mohamed tarek 
 */ 

#ifndef COMMON_MACROS
#define COMMON_MACROS

#define assig_bit(reg, bit_no, state) (reg) = (((reg) & ( ~(1 << (bit_no)))) | ((state) << (bit_no)))

/* Set a certain bit in any register */
#define SET_BIT(REG, BIT) ((REG) |= (1 << (BIT)))

/* Clear a certain bit in any register */
#define CLEAR_BIT(REG, BIT) ((REG) &= ( ~(1 << (BIT))))

/* Toggle a certain bit in any register */
#define TOGGLE_BIT(REG, BIT) ((REG) ^= (1 << (BIT)))

/* Rotate right the register value with specific number of rotates */
#define ROR(REG, num) ((REG) = ((REG) >> (num)) | ((REG) << (8 - (num))) )

/* Rotate left the register value with specific number of rotates */
#define ROL(REG, num) ((REG) = ((REG) << (num)) | ((REG) >> (8 - (num))) )

/* Check if a specific bit is set in any register and return true if yes */
#define BIT_IS_SET(REG, BIT) ((REG) & (1 << (BIT)))

/* Check if a specific bit is cleared in any register and return true if yes */
#define BIT_IS_CLEAR(REG, BIT) ( !((REG) & (1 << (BIT))) )

#define get_bit(reg, bit_no) (((reg) & ((0x01) << (bit_no))) >> (bit_no))

#define MODULO(divident, divisor) ((divident) - ((divisor) * ((divident) / (divisor)))) 

#endif
