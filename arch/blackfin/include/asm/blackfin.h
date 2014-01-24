/*
 * Common header file for Blackfin family of processors.
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright 2004-2009 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
=======
 * Copyright 2004-2009 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Copyright 2004-2009 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
>>>>>>> refs/remotes/origin/master
 */

#ifndef _BLACKFIN_H_
#define _BLACKFIN_H_

#include <mach/anomaly.h>

#ifndef __ASSEMBLY__

/* SSYNC implementation for C file */
static inline void SSYNC(void)
{
	int _tmp;
<<<<<<< HEAD
<<<<<<< HEAD
	if (ANOMALY_05000312)
=======
	if (ANOMALY_05000312 || ANOMALY_05000244)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (ANOMALY_05000312 || ANOMALY_05000244)
>>>>>>> refs/remotes/origin/master
		__asm__ __volatile__(
			"cli %0;"
			"nop;"
			"nop;"
<<<<<<< HEAD
<<<<<<< HEAD
=======
			"nop;"
>>>>>>> refs/remotes/origin/cm-10.0
=======
			"nop;"
>>>>>>> refs/remotes/origin/master
			"ssync;"
			"sti %0;"
			: "=d" (_tmp)
		);
<<<<<<< HEAD
<<<<<<< HEAD
	else if (ANOMALY_05000244)
		__asm__ __volatile__(
			"nop;"
			"nop;"
			"nop;"
			"ssync;"
		);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	else
		__asm__ __volatile__("ssync;");
}

/* CSYNC implementation for C file */
static inline void CSYNC(void)
{
	int _tmp;
<<<<<<< HEAD
<<<<<<< HEAD
	if (ANOMALY_05000312)
=======
	if (ANOMALY_05000312 || ANOMALY_05000244)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (ANOMALY_05000312 || ANOMALY_05000244)
>>>>>>> refs/remotes/origin/master
		__asm__ __volatile__(
			"cli %0;"
			"nop;"
			"nop;"
<<<<<<< HEAD
<<<<<<< HEAD
=======
			"nop;"
>>>>>>> refs/remotes/origin/cm-10.0
=======
			"nop;"
>>>>>>> refs/remotes/origin/master
			"csync;"
			"sti %0;"
			: "=d" (_tmp)
		);
<<<<<<< HEAD
<<<<<<< HEAD
	else if (ANOMALY_05000244)
		__asm__ __volatile__(
			"nop;"
			"nop;"
			"nop;"
			"csync;"
		);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	else
		__asm__ __volatile__("csync;");
}

#else  /* __ASSEMBLY__ */

#define LO(con32) ((con32) & 0xFFFF)
#define lo(con32) ((con32) & 0xFFFF)
#define HI(con32) (((con32) >> 16) & 0xFFFF)
#define hi(con32) (((con32) >> 16) & 0xFFFF)

/* SSYNC & CSYNC implementations for assembly files */

#define ssync(x) SSYNC(x)
#define csync(x) CSYNC(x)

<<<<<<< HEAD
<<<<<<< HEAD
#if ANOMALY_05000312
#define SSYNC(scratch) cli scratch; nop; nop; SSYNC; sti scratch;
#define CSYNC(scratch) cli scratch; nop; nop; CSYNC; sti scratch;

#elif ANOMALY_05000244
#define SSYNC(scratch) nop; nop; nop; SSYNC;
#define CSYNC(scratch) nop; nop; nop; CSYNC;
=======
#if ANOMALY_05000312 || ANOMALY_05000244
#define SSYNC(scratch)	\
do {			\
	cli scratch;	\
	nop; nop; nop;	\
	SSYNC;		\
	sti scratch;	\
} while (0)

#define CSYNC(scratch)	\
do {			\
	cli scratch;	\
	nop; nop; nop;	\
	CSYNC;		\
	sti scratch;	\
} while (0)
>>>>>>> refs/remotes/origin/cm-10.0
=======
#if ANOMALY_05000312 || ANOMALY_05000244
#define SSYNC(scratch)	\
	cli scratch;	\
	nop; nop; nop;	\
	SSYNC;		\
	sti scratch;

#define CSYNC(scratch)	\
	cli scratch;	\
	nop; nop; nop;	\
	CSYNC;		\
	sti scratch;
>>>>>>> refs/remotes/origin/master

#else
#define SSYNC(scratch) SSYNC;
#define CSYNC(scratch) CSYNC;
<<<<<<< HEAD
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#endif /* ANOMALY_05000312 & ANOMALY_05000244 handling */

#endif /* __ASSEMBLY__ */

#include <asm/mem_map.h>
#include <mach/blackfin.h>
#include <asm/bfin-global.h>

#endif				/* _BLACKFIN_H_ */
