/*	Acvt.c	1.2	90/12/04	*/

#include "align.h"
cvt(infop) 	process_info *infop;
/*
/*	Convert , checks overflow
/*
/****************************************/
{
	register long result;

	result = operand(infop,0)->data;
	if (result < 0 )  negative_1 ; else negative_0;
	if (result == 0 )  zero_1 ; else zero_0;
	carry_1; overflow_0;
		/* Overflow may be set by writing back */
	write_back (infop, result, operand(infop,1) );
}
