/*-
 * Copyright (c) 1980, 1991 The Regents of the University of California.
 * All rights reserved.
 *
 * %sccs.include.redist.c%
 */

#ifndef lint
static char sccsid[] = "@(#)init.c	5.14 (Berkeley) 07/28/91";
#endif /* not lint */

#if __STDC__
# include <stdarg.h>
#else
# include <varargs.h>
#endif

#include "csh.h"
#include "extern.h"

#define	INF	1000

struct biltins bfunc[] =
{
    "@", 	dolet, 		0, INF,
    "alias", 	doalias, 	0, INF,
    "alloc", 	showall, 	0, 1,
    "bg", 	dobg, 		0, INF,
    "break", 	dobreak, 	0, 0,
    "breaksw", 	doswbrk, 	0, 0,
    "case", 	dozip, 		0, 1,
    "cd", 	dochngd, 	0, INF,
    "chdir", 	dochngd, 	0, INF,
    "continue", docontin, 	0, 0,
    "default", 	dozip, 		0, 0,
    "dirs", 	dodirs,		0, INF,
    "echo", 	doecho,		0, INF,
    "else", 	doelse,		0, INF,
    "end", 	doend, 		0, 0,
    "endif", 	dozip, 		0, 0,
    "endsw", 	dozip, 		0, 0,
    "eval", 	doeval, 	0, INF,
    "exec", 	execash, 	1, INF,
    "exit", 	doexit, 	0, INF,
    "fg", 	dofg, 		0, INF,
    "foreach", 	doforeach, 	3, INF,
    "glob", 	doglob, 	0, INF,
    "goto", 	dogoto, 	1, 1,
    "hashstat", hashstat, 	0, 0,
    "history", 	dohist, 	0, 2,
    "if", 	doif, 		1, INF,
    "jobs", 	dojobs, 	0, 1,
    "kill", 	dokill, 	1, INF,
    "limit", 	dolimit, 	0, 3,
    "linedit", 	doecho, 	0, INF,
    "login", 	dologin, 	0, 1,
    "logout", 	dologout, 	0, 0,
    "nice", 	donice, 	0, INF,
    "nohup", 	donohup, 	0, INF,
    "notify", 	donotify, 	0, INF,
    "onintr", 	doonintr, 	0, 2,
    "popd", 	dopopd, 	0, INF,
    "printf",	doprintf,	1, INF,
    "pushd", 	dopushd, 	0, INF,
    "rehash", 	dohash, 	0, 0,
    "repeat", 	dorepeat, 	2, INF,
    "set", 	doset, 		0, INF,
    "setenv", 	dosetenv, 	0, 2,
    "shift", 	shift, 		0, 1,
    "source", 	dosource, 	1, 2,
    "stop", 	dostop, 	1, INF,
    "suspend", 	dosuspend, 	0, 0,
    "switch", 	doswitch, 	1, INF,
    "time", 	dotime, 	0, INF,
    "umask", 	doumask, 	0, 1,
    "unalias", 	unalias, 	1, INF,
    "unhash", 	dounhash, 	0, 0,
    "unlimit", 	dounlimit, 	0, INF,
    "unset", 	unset, 		1, INF,
    "unsetenv", dounsetenv, 	1, INF,
    "wait", 	dowait, 	0, 0,
    "which", 	dowhich, 	1, INF,
    "while", 	dowhile, 	1, INF,
};
int     nbfunc = sizeof bfunc / sizeof *bfunc;

struct srch srchn[] =
{
    "@", 	T_LET,
    "break", 	T_BREAK,
    "breaksw", 	T_BRKSW,
    "case", 	T_CASE,
    "default", 	T_DEFAULT,
    "else", 	T_ELSE,
    "end", 	T_END,
    "endif", 	T_ENDIF,
    "endsw", 	T_ENDSW,
    "exit", 	T_EXIT,
    "foreach", 	T_FOREACH,
    "goto", 	T_GOTO,
    "if", 	T_IF,
    "label", 	T_LABEL,
    "set", 	T_SET,
    "switch", 	T_SWITCH,
    "while", 	T_WHILE,
};
int     nsrchn = sizeof srchn / sizeof *srchn;

struct mesg mesg[] =
{
     /*  0 */ 	0, 		"",
     /*  1 */ 	"HUP", 		"Hangup",
     /*  2 */ 	"INT", 		"Interrupt",
     /*  3 */ 	"QUIT", 	"Quit",
     /*  4 */ 	"ILL", 		"Illegal instruction",
     /*  5 */ 	"TRAP", 	"Trace/BPT trap",
     /*  6 */ 	"IOT", 		"IOT trap",
     /*  7 */ 	"EMT", 		"EMT trap",
     /*  8 */ 	"FPE", 		"Floating exception",
     /*  9 */ 	"KILL", 	"Killed",
     /* 10 */ 	"BUS", 		"Bus error",
     /* 11 */ 	"SEGV", 	"Segmentation fault",
     /* 12 */ 	"SYS", 		"Bad system call",
     /* 13 */ 	"PIPE", 	"Broken pipe",
     /* 14 */ 	"ALRM", 	"Alarm clock",
     /* 15 */ 	"TERM", 	"Terminated",
     /* 16 */ 	"URG", 		"Urgent condition on IO channel",
     /* 17 */ 	"STOP", 	"Suspended (signal)",
     /* 18 */ 	"TSTP", 	"Suspended",
     /* 19 */ 	"CONT", 	"Continued",
     /* 20 */ 	"CHLD", 	"Child exited",
     /* 21 */ 	"TTIN", 	"Suspended (tty input)",
     /* 22 */ 	"TTOU", 	"Suspended (tty output)",
     /* 23 */ 	"IO", 		"IO possible interrupt",
     /* 24 */ 	"XCPU", 	"Cputime limit exceeded",
     /* 25 */ 	"XFSZ", 	"Filesize limit exceeded",
     /* 26 */ 	"VTALRM", 	"Virtual time alarm",
     /* 27 */ 	"PROF", 	"Profiling time alarm",
     /* 28 */ 	"WINCH", 	"Window changed",
     /* 29 */ 	"INFO", 	"Information request",
     /* 30 */ 	"USR1", 	"User signal 1",
     /* 31 */ 	"USR2", 	"User signal 2",
     /* 32 */ 	0, 		"Signal 32",
};
