/*-
 * Copyright (c) 1980 The Regents of the University of California.
 * All rights reserved.
 *
 * %sccs.include.redist.c%
 *
 *	@(#)pcwhoami.h	5.3 (Berkeley) 04/16/91
 */

/*
 *	am i generating an obj file (OBJ),
 *	postfix binary input to the 2nd pass of the portable c compiler (PC),
 *	or pTrees (PTREE)?
 */
#undef	OBJ
#define	PC
#undef	PTREE

/*
 *	we assume one of the following will be defined by the preprocessor:
 *	vax	for vaxes
 *	pdp11	for pdp11's
 *	mc68000	for motorola mc68000's
 *	tahoe	for cci power 6/32
 */

/*
 *	hardware characteristics:
 *	address size (16 or 32 bits) and byte ordering (normal or dec11 family).
 */
#ifdef vax
#   undef	ADDR16
#   define	ADDR32
#   define	DEC11
#endif vax
#ifdef pdp11
#   define	ADDR16
#   undef	ADDR32
#   define	DEC11
#endif vax
#ifdef mc68000
#   undef	ADDR16
#   define	ADDR32
#   undef	DEC11
#endif mc68000
#ifdef tahoe
#   undef	ADDR16
#   define	ADDR32
#   undef	DEC11
#endif tahoe
#ifdef z8000
#   define	ADDR16
#   undef	ADDR32
#   undef	DEC11
#endif z8000

/*
 *	am i pi or pxp?
 */
#define PI
#undef	PXP

/*
 *	am i both passes, or am i only one of the two passes pi0 or pi1?
 */
#define	PI01
#undef	PI0
#undef	PI1
