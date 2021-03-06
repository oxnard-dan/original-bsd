/*-
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * William Jolitz.
 *
 * %sccs.include.redist.c%
 */

#if defined(SYSLIBC_SCCS) && !defined(lint)
	.asciz "@(#)fork.s	8.1 (Berkeley) 06/04/93"
#endif /* SYSLIBC_SCCS and not lint */

#include "SYS.h"

SYSCALL(fork)
	cmpl	$0,%edx	/* parent, since %edx == 0 in parent, 1 in child */
	je	1f
	movl	$0,%eax
1:
	ret		/* pid = fork(); */
