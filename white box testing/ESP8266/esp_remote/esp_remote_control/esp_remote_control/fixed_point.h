/*
 * fixed_point.h
 *
 * Created: 6/4/2019 10:35:19 ص
 *  Author: hppp
 */ 


#ifndef FIXED_POINT_H_
#define FIXED_POINT_H_

/* ---------------------------------------------------------------------------------------------------------------
[MACRO Name] : MULTIPLY_FIXED_POINT
[Description] : This function is responsible multiplying 2 fixed point numbers
				the fixed point multiplication goes like this x=(y*z)>>PRECISION_BITS (we shift right by the number of precision bits because multiplying 2 fixed numbers THE RESULT HAS DOUBLE THENUMBER OF PRECISIONN BITS SO WE HALF IT BY SHIFTING THE NUMBER OF PROCISION BITS)
[Args] :
[in] -A
this ARG is the first operand in the multiplication
-B
this ARG is the second operand in the multiplication
-PRECISION_BITS
this ARG is the number of the precision bits
---------------------------------------------------------------------------------------------------------------*/
#define FIXP_MUL(A, B, precision_bits) (((A) * (B)) >> (precision_bits))

/* ---------------------------------------------------------------------------------------------------------------
[MACRO Name] : DIVIDE_FIXED_POINT
[Description] : This function is responsible dividing 2 fixed point numbers
				the fixed point multiplication goes like this x=((y<<1)*z) (we shift y left by the number of precision bits because dividing 2 fixed numbers , the numbers will remove each ithers precision bits (just like 20/30) so the result won't contain any precision bits and will be integer number we avoid this step by shifting the first operand left by the number of precision bits
[Args] :
[in] -A
this ARG is the first operand(a fixed point number) in the division
-B
this ARG is the second operand(a fixed point number) in the division
-PRECISION_BITS
this ARG is the number of the precision bits
---------------------------------------------------------------------------------------------------------------*/
#define FIXP_DIV(A, B, precision_bits) (((A) << (precision_bits)) / (B))

#define FIXP_CONVERT(number, precision) ((number) << (precision))


#endif /* FIXED_POINT_H_ */