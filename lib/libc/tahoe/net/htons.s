/*
 * Copyright (c) 1988, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * %sccs.include.redist.c%
 */

#if defined(LIBC_SCCS) && !defined(lint)
	.asciz "@(#)htons.s	8.1 (Berkeley) 06/04/93"
#endif /* LIBC_SCCS and not lint */

/* hostorder = htons(netorder) */

#include "DEFS.h"

ENTRY(htons, 0)
	movzwl	6(fp),r0
	ret
