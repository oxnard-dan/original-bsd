/*
 * Copyright (c) 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * %sccs.include.redist.c%
 */

#ifndef lint
static char sccsid[] = "@(#)krcmd.c	8.1 (Berkeley) 06/06/93";
#endif /* not lint */

/*
 *	$Source: /usr/src/usr.bin/rlogin/RCS/krcmd.c,v $
 *	$Header: /mit/kerberos/ucb/mit/kcmd/RCS/krcmd.c,v 5.1
 *		89/07/25 15:38:44 kfall Exp Locker: kfall $
 * static char *rcsid_kcmd_c =
 * "$Header: /mit/kerberos/ucb/mit/kcmd/RCS/krcmd.c,v 5.1 89/07/25 15:38:44
 *	kfall Exp Locker: kfall $";
 */

#ifdef KERBEROS
#include <sys/types.h>
#ifdef CRYPT
#include <sys/socket.h>
#endif

#include <netinet/in.h>

#include <kerberosIV/des.h>
#include <kerberosIV/krb.h>

#include <stdio.h>

#define	SERVICE_NAME	"rcmd"

int	kcmd __P((int *, char **, u_short, char *, char *, char *, int *,
	    KTEXT, char *, char *, CREDENTIALS *, Key_schedule, MSG_DAT *,
	    struct sockaddr_in *, struct sockaddr_in *, long));

/*
 * krcmd: simplified version of Athena's "kcmd"
 *	returns a socket attached to the destination, -1 or krb error on error 
 *	if fd2p is non-NULL, another socket is filled in for it
 */

int
krcmd(ahost, rport, remuser, cmd, fd2p, realm)
	char	**ahost;
	u_short	rport;
	char	*remuser, *cmd;
	int	*fd2p;
	char	*realm;
{
	int		sock = -1, err = 0;
	KTEXT_ST	ticket;
	long		authopts = 0L;

	err = kcmd(
		&sock,
		ahost,
		rport,
		NULL,	/* locuser not used */
		remuser,
		cmd,
		fd2p,
		&ticket,
		SERVICE_NAME,
		realm,
		(CREDENTIALS *)  NULL,		/* credentials not used */
		(bit_64 *) NULL,		/* key schedule not used */
		(MSG_DAT *) NULL,		/* MSG_DAT not used */
		(struct sockaddr_in *) NULL,	/* local addr not used */
		(struct sockaddr_in *) NULL,	/* foreign addr not used */
		authopts
	);

	if (err > KSUCCESS && err < MAX_KRB_ERRORS) {
		fprintf(stderr, "krcmd: %s\n", krb_err_txt[err]);
		return(-1);
	}
	if (err < 0)
		return(-1);
	return(sock);
}

#ifdef CRYPT
int
krcmd_mutual(ahost, rport, remuser, cmd, fd2p, realm, cred, sched)
	char		**ahost;
	u_short		rport;
	char		*remuser, *cmd;
	int		*fd2p;
	char		*realm;
	CREDENTIALS	*cred;
	Key_schedule	sched;
{
	int		sock, err;
	KTEXT_ST	ticket;
	MSG_DAT		msg_dat;
	struct sockaddr_in	laddr, faddr;
	long authopts = KOPT_DO_MUTUAL;

	err = kcmd(
		&sock,
		ahost,
		rport,
		NULL,	/* locuser not used */
		remuser,
		cmd,
		fd2p,
		&ticket,
		SERVICE_NAME,
		realm,
		cred,		/* filled in */
		sched,		/* filled in */
		&msg_dat,	/* filled in */
		&laddr,		/* filled in */
		&faddr,		/* filled in */
		authopts
	);

	if (err > KSUCCESS && err < MAX_KRB_ERRORS) {
		fprintf(stderr, "krcmd_mutual: %s\n", krb_err_txt[err]);
		return(-1);
	}

	if (err < 0)
		return (-1);
	return(sock);
}
#endif /* CRYPT */
#endif /* KERBEROS */
