/*-
 * Copyright (c) 1979, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * %sccs.include.redist.c%
 */

#ifndef lint
static char sccsid[] = "@(#)RELTGT.c	8.1 (Berkeley) 06/06/93";
#endif /* not lint */

#include "h00vars.h"

bool
RELTGT(bytecnt, left, right)

	long		bytecnt;
	register long	*left;
	register long	*right;
{
	register int	longcnt;

	longcnt = bytecnt >> 2;
	do	{
		if ((*right & ~*left) != 0)
			return FALSE;
		if ((*left++ & ~*right++) != 0)
			goto geq;
	} while (--longcnt);
	return FALSE;
geq:
	while (--longcnt) {
		if ((*right++ & ~*left++) != 0) 
			return FALSE;
	}
	return TRUE;
}
