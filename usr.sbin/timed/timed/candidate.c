/*-
 * Copyright (c) 1985, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * %sccs.include.redist.c%
 */

#ifndef lint
static char sccsid[] = "@(#)candidate.c	8.1 (Berkeley) 06/06/93";
#endif /* not lint */

#ifdef sgi
#ident "$Revision: 1.9 $"
#endif

#include "globals.h"

/*
 * `election' candidates a host as master: it is called by a slave
 * which runs with the -M option set when its election timeout expires.
 * Note the conservative approach: if a new timed comes up, or another
 * candidate sends an election request, the candidature is withdrawn.
 */
int
election(net)
	struct netinfo *net;
{
	struct tsp *resp, msg;
	struct timeval then, wait;
	struct tsp *answer;
	struct hosttbl *htp;
	char loop_lim = 0;

/* This code can get totally confused if it gets slightly behind.  For
 *	example, if readmsg() has some QUIT messages waiting from the last
 *	round, we would send an ELECTION message, get the stale QUIT,
 *	and give up.  This results in network storms when several machines
 *	do it at once.
 */
	wait.tv_sec = 0;
	wait.tv_usec = 0;
	while (0 != readmsg(TSP_REFUSE, ANYADDR, &wait, net)) {
		if (trace)
			fprintf(fd, "election: discarded stale REFUSE\n");
	}
	while (0 != readmsg(TSP_QUIT, ANYADDR, &wait, net)) {
		if (trace)
			fprintf(fd, "election: discarded stale QUIT\n");
	}

again:
	syslog(LOG_INFO, "This machine is a candidate time master");
	if (trace)
		fprintf(fd, "This machine is a candidate time master\n");
	msg.tsp_type = TSP_ELECTION;
	msg.tsp_vers = TSPVERSION;
	(void)strcpy(msg.tsp_name, hostname);
	bytenetorder(&msg);
	if (sendto(sock, (char *)&msg, sizeof(struct tsp), 0,
		   (struct sockaddr*)&net->dest_addr,
		   sizeof(struct sockaddr)) < 0) {
		trace_sendto_err(net->dest_addr.sin_addr);
		return(SLAVE);
	}

	(void)gettimeofday(&then, 0);
	then.tv_sec += 3;
	for (;;) {
		(void)gettimeofday(&wait, 0);
		timevalsub(&wait,&then,&wait);
		resp = readmsg(TSP_ANY, ANYADDR, &wait, net);
		if (!resp)
			return(MASTER);

		switch (resp->tsp_type) {

		case TSP_ACCEPT:
			(void)addmach(resp->tsp_name, &from,fromnet);
			break;

		case TSP_MASTERUP:
		case TSP_MASTERREQ:
			/*
			 * If another timedaemon is coming up at the same
			 * time, give up, and let it be the master.
			 */
			if (++loop_lim < 5
			    && !good_host_name(resp->tsp_name)) {
				(void)addmach(resp->tsp_name, &from,fromnet);
				suppress(&from, resp->tsp_name, net);
				goto again;
			}
			rmnetmachs(net);
			return(SLAVE);

		case TSP_QUIT:
		case TSP_REFUSE:
			/*
			 * Collision: change value of election timer
			 * using exponential backoff. 
			 *
			 *  Fooey.
			 * An exponential backoff on a delay starting at
			 * 6 to 15 minutes for a process that takes
			 * milliseconds is silly.  It is particularly
			 * strange that the original code would increase
			 * the backoff without bound.
			 */
			rmnetmachs(net);
			return(SLAVE);

		case TSP_ELECTION:
			/* no master for another round */
			htp = addmach(resp->tsp_name,&from,fromnet);
			msg.tsp_type = TSP_REFUSE;
			(void)strcpy(msg.tsp_name, hostname);
			answer = acksend(&msg, &htp->addr, htp->name,
					 TSP_ACK, 0, htp->noanswer);
			if (!answer) {
				syslog(LOG_ERR, "error in election from %s",
				       htp->name);
			}
			break;

		case TSP_SLAVEUP:
			(void)addmach(resp->tsp_name, &from,fromnet);
			break;

		case TSP_SETDATE:
		case TSP_SETDATEREQ:
			break;

		default:
			if (trace) {
				fprintf(fd, "candidate: ");
				print(resp, &from);
			}
			break;
		}
	}
}
