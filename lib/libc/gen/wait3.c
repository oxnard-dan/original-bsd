/*
 * Copyright (c) 1988 The Regents of the University of California.
 * All rights reserved.
 *
 * %sccs.include.redist.c%
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)wait3.c	5.4 (Berkeley) 06/01/90";
#endif /* LIBC_SCCS and not lint */

#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/resource.h>

wait3(pstat, options, rup)
	union wait *pstat;
	int options;
	struct rusage *rup;
{

	return (wait4(WAIT_ANY, pstat, options, rup));
}
