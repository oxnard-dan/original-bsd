/*-
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Chris Torek.
 *
 * %sccs.include.redist.c%
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)vprintf.c	5.7 (Berkeley) 06/23/92";
#endif /* LIBC_SCCS and not lint */

#include <stdio.h>

vprintf(fmt, ap)
	char const *fmt;
	_BSD_VA_LIST_ ap;
{
	return (vfprintf(stdout, fmt, ap));
}
