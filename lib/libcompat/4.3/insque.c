/*
 * Copyright (c) 1987, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * %sccs.include.redist.c%
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)insque.c	8.1 (Berkeley) 06/04/93";
#endif /* LIBC_SCCS and not lint */

/*
 * insque -- vax insque instruction
 *
 * NOTE: this implementation is non-atomic!!
 */

struct vaxque {		/* queue format expected by VAX queue instructions */
	struct vaxque	*vq_next;
	struct vaxque	*vq_prev;
};

insque(e, prev)
	register struct vaxque *e, *prev;
{
	e->vq_prev = prev;
	e->vq_next = prev->vq_next;
	prev->vq_next->vq_prev = e;
	prev->vq_next = e;
}
