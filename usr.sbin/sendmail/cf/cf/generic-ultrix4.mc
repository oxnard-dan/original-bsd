divert(-1)
#
# Copyright (c) 1983 Eric P. Allman
# Copyright (c) 1988, 1993
#	The Regents of the University of California.  All rights reserved.
#
# %sccs.include.redist.sh%
#

#
#  This is a generic configuration file for Ultrix 4.x.
#  It has support for local and SMTP mail only.  If you want to
#  customize it, copy it to a name appropriate for your environment
#  and do the modifications there.
#

include(`../m4/cf.m4')
VERSIONID(`@(#)generic-ultrix4.mc	8.1 (Berkeley) 04/21/95')
OSTYPE(ultrix4)dnl
DOMAIN(generic)dnl
MAILER(local)dnl
MAILER(smtp)dnl
