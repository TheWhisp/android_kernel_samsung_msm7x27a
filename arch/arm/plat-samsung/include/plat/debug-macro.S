/* arch/arm/plat-samsung/include/plat/debug-macro.S
 *
 * Copyright 2005, 2007 Simtec Electronics
 *	http://armlinux.simtec.co.uk/
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <plat/regs-serial.h>

/* The S5PV210/S5PC110 implementations are as belows. */

	.macro fifo_level_s5pv210 rd, rx
<<<<<<< HEAD
<<<<<<< HEAD
		ldr	\rd, [ \rx, # S3C2410_UFSTAT ]
=======
		ldr	\rd, [\rx, # S3C2410_UFSTAT]
>>>>>>> refs/remotes/origin/cm-10.0
=======
		ldr	\rd, [\rx, # S3C2410_UFSTAT]
>>>>>>> refs/remotes/origin/cm-11.0
		and	\rd, \rd, #S5PV210_UFSTAT_TXMASK
	.endm

	.macro  fifo_full_s5pv210 rd, rx
<<<<<<< HEAD
<<<<<<< HEAD
		ldr	\rd, [ \rx, # S3C2410_UFSTAT ]
=======
		ldr	\rd, [\rx, # S3C2410_UFSTAT]
>>>>>>> refs/remotes/origin/cm-10.0
=======
		ldr	\rd, [\rx, # S3C2410_UFSTAT]
>>>>>>> refs/remotes/origin/cm-11.0
		tst	\rd, #S5PV210_UFSTAT_TXFULL
	.endm

/* The S3C2440 implementations are used by default as they are the
 * most widely re-used */

	.macro fifo_level_s3c2440 rd, rx
<<<<<<< HEAD
<<<<<<< HEAD
		ldr	\rd, [ \rx, # S3C2410_UFSTAT ]
=======
		ldr	\rd, [\rx, # S3C2410_UFSTAT]
>>>>>>> refs/remotes/origin/cm-10.0
=======
		ldr	\rd, [\rx, # S3C2410_UFSTAT]
>>>>>>> refs/remotes/origin/cm-11.0
		and	\rd, \rd, #S3C2440_UFSTAT_TXMASK
	.endm

#ifndef fifo_level
#define fifo_level fifo_level_s3c2440
#endif

	.macro  fifo_full_s3c2440 rd, rx
<<<<<<< HEAD
<<<<<<< HEAD
		ldr	\rd, [ \rx, # S3C2410_UFSTAT ]
=======
		ldr	\rd, [\rx, # S3C2410_UFSTAT]
>>>>>>> refs/remotes/origin/cm-10.0
=======
		ldr	\rd, [\rx, # S3C2410_UFSTAT]
>>>>>>> refs/remotes/origin/cm-11.0
		tst	\rd, #S3C2440_UFSTAT_TXFULL
	.endm

#ifndef fifo_full
#define fifo_full fifo_full_s3c2440
#endif

	.macro	senduart,rd,rx
<<<<<<< HEAD
<<<<<<< HEAD
		strb 	\rd, [\rx, # S3C2410_UTXH ]
	.endm

	.macro	busyuart, rd, rx
		ldr	\rd, [ \rx, # S3C2410_UFCON ]
=======
		strb 	\rd, [\rx, # S3C2410_UTXH]
	.endm

	.macro	busyuart, rd, rx
		ldr	\rd, [\rx, # S3C2410_UFCON]
>>>>>>> refs/remotes/origin/cm-10.0
=======
		strb 	\rd, [\rx, # S3C2410_UTXH]
	.endm

	.macro	busyuart, rd, rx
		ldr	\rd, [\rx, # S3C2410_UFCON]
>>>>>>> refs/remotes/origin/cm-11.0
		tst	\rd, #S3C2410_UFCON_FIFOMODE	@ fifo enabled?
		beq	1001f				@
		@ FIFO enabled...
1003:
		fifo_full \rd, \rx
		bne	1003b
		b	1002f

1001:
		@ busy waiting for non fifo
<<<<<<< HEAD
<<<<<<< HEAD
		ldr	\rd, [ \rx, # S3C2410_UTRSTAT ]
=======
		ldr	\rd, [\rx, # S3C2410_UTRSTAT]
>>>>>>> refs/remotes/origin/cm-10.0
=======
		ldr	\rd, [\rx, # S3C2410_UTRSTAT]
>>>>>>> refs/remotes/origin/cm-11.0
		tst	\rd, #S3C2410_UTRSTAT_TXFE
		beq	1001b

1002:		@ exit busyuart
	.endm

	.macro	waituart,rd,rx
<<<<<<< HEAD
<<<<<<< HEAD
		ldr	\rd, [ \rx, # S3C2410_UFCON ]
=======
		ldr	\rd, [\rx, # S3C2410_UFCON]
>>>>>>> refs/remotes/origin/cm-10.0
=======
		ldr	\rd, [\rx, # S3C2410_UFCON]
>>>>>>> refs/remotes/origin/cm-11.0
		tst	\rd, #S3C2410_UFCON_FIFOMODE	@ fifo enabled?
		beq	1001f				@
		@ FIFO enabled...
1003:
		fifo_level \rd, \rx
		teq	\rd, #0
		bne	1003b
		b	1002f
1001:
		@ idle waiting for non fifo
<<<<<<< HEAD
<<<<<<< HEAD
		ldr	\rd, [ \rx, # S3C2410_UTRSTAT ]
=======
		ldr	\rd, [\rx, # S3C2410_UTRSTAT]
>>>>>>> refs/remotes/origin/cm-10.0
=======
		ldr	\rd, [\rx, # S3C2410_UTRSTAT]
>>>>>>> refs/remotes/origin/cm-11.0
		tst	\rd, #S3C2410_UTRSTAT_TXFE
		beq	1001b

1002:		@ exit busyuart
	.endm
