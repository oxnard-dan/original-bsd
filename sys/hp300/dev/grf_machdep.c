/*
 * Copyright (c) 1991 University of Utah.
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * the Systems Programming Group of the University of Utah Computer
 * Science Department.
 *
 * %sccs.include.redist.c%
 *
 * from: Utah $Hdr: grf_machdep.c 1.1 92/01/21
 *
 *	@(#)grf_machdep.c	8.2 (Berkeley) 01/12/94
 */

/*
 * Graphics display driver for the HP300/400 DIO/DIO-II based machines.
 * This is the hardware-dependent configuration portion of the driver.
 */

#include "grf.h"
#if NGRF > 0

#include <sys/param.h>

#include <hp/dev/device.h>
#include <hp/dev/grfioctl.h>
#include <hp/dev/grfvar.h>
#include <hp/dev/grfreg.h>

int grfprobe();
struct	driver grfdriver = { grfprobe, "grf" };

/*
 * XXX called from ite console init routine.
 * Does just what configure will do later but without printing anything.
 */
grfconfig()
{
	register caddr_t addr;
	register struct hp_hw *hw;
	register struct hp_device *hd, *nhd;

	for (hw = sc_table; hw->hw_type; hw++) {
	        if (!HW_ISDEV(hw, D_BITMAP))
			continue;
		/*
		 * Found one, now match up with a logical unit number
		 */
		nhd = NULL;		
		addr = hw->hw_kva;
		for (hd = hp_dinit; hd->hp_driver; hd++) {
			if (hd->hp_driver != &grfdriver || hd->hp_alive)
				continue;
			/*
			 * Wildcarded.  If first, remember as possible match.
			 */
			if (hd->hp_addr == NULL) {
				if (nhd == NULL)
					nhd = hd;
				continue;
			}
			/*
			 * Not wildcarded.
			 * If exact match done searching, else keep looking.
			 */
			if (sctova(hd->hp_addr) == addr) {
				nhd = hd;
				break;
			}
		}
		/*
		 * Found a match, initialize
		 */
		if (nhd && grfinit(addr, nhd->hp_unit))
			nhd->hp_addr = addr;
	}
}

/*
 * Normal init routine called by configure() code
 */
grfprobe(hd)
	struct hp_device *hd;
{
	struct grf_softc *gp = &grf_softc[hd->hp_unit];

	if ((gp->g_flags & GF_ALIVE) == 0 &&
	    !grfinit(hd->hp_addr, hd->hp_unit))
		return(0);
	printf("grf%d: %d x %d ", hd->hp_unit,
	       gp->g_display.gd_dwidth, gp->g_display.gd_dheight);
	if (gp->g_display.gd_colors == 2)
		printf("monochrome");
	else
		printf("%d color", gp->g_display.gd_colors);
	printf(" %s display\n", gp->g_sw->gd_desc);
	return(1);
}

grfinit(addr, unit)
	caddr_t addr;
	int unit;
{
	struct grf_softc *gp = &grf_softc[unit];
	register struct grfsw *gsw;
	struct grfreg *gr;

	gr = (struct grfreg *) addr;
	if (gr->gr_id != GRFHWID)
		return(0);
	for (gsw = grfsw; gsw < &grfsw[ngrfsw]; gsw++)
		if (gsw->gd_hwid == gr->gr_id2)
			break;
	if (gsw < &grfsw[ngrfsw] && (*gsw->gd_init)(gp, addr)) {
		gp->g_sw = gsw;
		gp->g_display.gd_id = gsw->gd_swid;
		gp->g_flags = GF_ALIVE;
		return(1);
	}
	return(0);
}
#endif
