/*
 * Copyright (c) 1991 University of Utah.
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * the Systems Programming Group of the University of Utah Computer
 * Science Department.
 *
 * %sccs.include.redist.c%
 *
 * from: Utah $Hdr: grf_conf.c 1.2 92/01/22$
 *
 *	@(#)grf_conf.c	7.2 (Berkeley) 10/11/92
 */

/*
 * XXX this information could be generated by config.
 */
#include "grf.h"
#if NGRF > 0

#include <sys/types.h>

#include <hp/dev/device.h>
#include <hp/dev/grfioctl.h>
#include <hp/dev/grfvar.h>
#include <hp/dev/grfreg.h>

extern	int tc_init(), tc_mode();
extern	int gb_init(), gb_mode();
extern	int rb_init(), rb_mode();
extern	int dv_init(), dv_mode();
extern	int hy_init(), hy_mode();

struct grfsw grfsw[] = {
	GID_TOPCAT,	GRFBOBCAT,	"topcat",	  tc_init, tc_mode,
	GID_GATORBOX,	GRFGATOR,	"gatorbox",	  gb_init, gb_mode,
	GID_RENAISSANCE,GRFRBOX,	"renaissance",	  rb_init, rb_mode,
	GID_LRCATSEYE,	GRFCATSEYE,	"lo-res catseye", tc_init, tc_mode,
	GID_HRCCATSEYE,	GRFCATSEYE,	"hi-res catseye", tc_init, tc_mode,
	GID_HRMCATSEYE,	GRFCATSEYE,	"hi-res catseye", tc_init, tc_mode,
	GID_DAVINCI,	GRFDAVINCI,	"davinci",	  dv_init, dv_mode,
	GID_HYPERION,	GRFHYPERION,	"hyperion",	  hy_init, hy_mode,
};
int	ngrfsw = sizeof(grfsw) / sizeof(grfsw[0]);

#include "ite.h"
#if NITE > 0

#include <hp/dev/itevar.h>

extern	u_char ite_readbyte();
extern	int ite_writeglyph();
extern	int topcat_scroll(), topcat_init(), topcat_deinit();
extern	int topcat_clear(), topcat_putc(), topcat_cursor();
extern	int gbox_scroll(), gbox_init(), gbox_deinit();
extern	int gbox_clear(), gbox_putc(), gbox_cursor();
extern	int rbox_scroll(), rbox_init(), rbox_deinit();
extern	int rbox_clear(), rbox_putc(), rbox_cursor();
extern	int dvbox_scroll(), dvbox_init(), dvbox_deinit();
extern	int dvbox_clear(), dvbox_putc(), dvbox_cursor();
extern	int hyper_scroll(), hyper_init(), hyper_deinit();
extern	int hyper_clear(), hyper_putc(), hyper_cursor();

struct itesw itesw[] = {
	GID_TOPCAT,
	topcat_init,	topcat_deinit,	topcat_clear,	topcat_putc,
	topcat_cursor,	topcat_scroll,	ite_readbyte,	ite_writeglyph,
	GID_GATORBOX,
	gbox_init,	gbox_deinit,	gbox_clear,	gbox_putc,
	gbox_cursor,	gbox_scroll,	ite_readbyte,	ite_writeglyph,
	GID_RENAISSANCE,
	rbox_init,	rbox_deinit,	rbox_clear,	rbox_putc,
	rbox_cursor,	rbox_scroll,	ite_readbyte,	ite_writeglyph,
	GID_LRCATSEYE,
	topcat_init,	topcat_deinit,	topcat_clear,	topcat_putc,
	topcat_cursor,	topcat_scroll,	ite_readbyte,	ite_writeglyph,
	GID_HRCCATSEYE,
	topcat_init,	topcat_deinit,	topcat_clear,	topcat_putc,
	topcat_cursor,	topcat_scroll,	ite_readbyte,	ite_writeglyph,
	GID_HRMCATSEYE,
	topcat_init,	topcat_deinit,	topcat_clear,	topcat_putc,
	topcat_cursor,	topcat_scroll,	ite_readbyte,	ite_writeglyph,
	GID_DAVINCI,
	dvbox_init,	dvbox_deinit,	dvbox_clear,	dvbox_putc,
	dvbox_cursor,	dvbox_scroll,	ite_readbyte,	ite_writeglyph,
	GID_HYPERION,
	hyper_init,	hyper_deinit,	hyper_clear,	hyper_putc,
	hyper_cursor,	hyper_scroll,	ite_readbyte,	ite_writeglyph,
};
int	nitesw = sizeof(itesw) / sizeof(itesw[0]);
#endif
#endif
