/*-
 * Copyright (c) 1983, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * %sccs.include.proprietary.c%
 */

#ifndef lint
static char sccsid[] = "@(#)move.c	8.1 (Berkeley) 06/04/93";
#endif /* not lint */

move(xi,yi){
	putch(035);
	cont(xi,yi);
}
