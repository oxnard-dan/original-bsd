PUSHDIVERT(-1)
#
# Copyright (c) 1983 Eric P. Allman
# Copyright (c) 1988, 1993
#	The Regents of the University of California.  All rights reserved.
#
# %sccs.include.redist.sh%
#

ifdef(`USENET_MAILER_PATH',, `define(`USENET_MAILER_PATH', /usr/lib/news/inews)')
ifdef(`USENET_MAILER_FLAGS',, `define(`USENET_MAILER_FLAGS', `rlsDFMmn')')
ifdef(`USENET_MAILER_ARGS',, `define(`USENET_MAILER_ARGS', `inews -m -h -n')')
POPDIVERT
####################################
###  USENET Mailer specification ###
####################################

VERSIONID(`@(#)usenet.m4	8.5 (Berkeley) 04/26/95')

Musenet,	P=USENET_MAILER_PATH, F=USENET_MAILER_FLAGS, S=10, R=20,
		_OPTINS(`USENET_MAILER_MAX', `M=', `, ')T=X-Usenet/X-Usenet/X-Unix,
		A=USENET_MAILER_ARGS $u
