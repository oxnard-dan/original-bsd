/*
 * Copyright (c) 1982, 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * %sccs.include.redist.c%
 *
 *	@(#)samachdep.h	7.2 (Berkeley) 05/07/91
 */

#define	NHPIB		4
#define NITE		4
#define	NSCSI		2
#define NRD		(NHPIB * 8)
#define NCT		(NHPIB * 8)
#define NSD		(NSCSI * 8)

/* from cpu.h */
#define IIOV(x)		(x)
#define DIOBASE		(0x600000)
#define	DIOCSIZE	(0x10000)
#define DIOIIBASE	(0x01000000)
#define DIOIICSIZE	(0x00400000)

#define HP_320		0	/* 16Mhz 68020+HP MMU+16K external cache */
#define HP_330		1	/* 16Mhz 68020+68851 MMU */
#define HP_350		2	/* 25Mhz 68020+HP MMU+32K external cache */
#define HP_360		3	/* 25Mhz 68030 */
#define HP_370		4	/* 33Mhz 68030+64K external cache */
#define HP_340		5	/* 16Mhz 68030 */
#define HP_375		6	/* 50Mhz 68030+32K external cache */

extern	int howto, devtype;

/* bogon grfinfo structure to keep grf_softc happy */
struct grfinfo {
	int	grf_foo;
};
