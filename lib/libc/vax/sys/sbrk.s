/*
 * Copyright (c) 1983, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * %sccs.include.redist.c%
 */

#if defined(SYSLIBC_SCCS) && !defined(lint)
	.asciz "@(#)sbrk.s	8.1 (Berkeley) 06/04/93"
#endif /* SYSLIBC_SCCS and not lint */

#include "SYS.h"

#define	SYS_brk		17

	.globl	_end
	.globl	minbrk
	.globl	curbrk

	.data
minbrk: .long	_end
curbrk:	.long	_end
	.text

ENTRY(sbrk)
	addl3	curbrk,4(ap),-(sp)
	pushl	$1
	movl	ap,r3
	movl	sp,ap
	chmk	$SYS_brk
	jcs 	err
	movl	curbrk,r0
	addl2	4(r3),curbrk
	ret
err:
	jmp	cerror
