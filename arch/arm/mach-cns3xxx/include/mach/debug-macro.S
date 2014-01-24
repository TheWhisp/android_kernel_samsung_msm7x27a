/*
 * Debugging macro include header
 *
 * Copyright 1994-1999 Russell King
 * Copyright 2008 Cavium Networks
 * Moved from linux/arch/arm/kernel/debug.S by Ben Dooks
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, Version 2, as
 * published by the Free Software Foundation.
 */

<<<<<<< HEAD
		.macro	addruart,rp,rv
=======
		.macro	addruart,rp,rv,tmp
>>>>>>> refs/remotes/origin/cm-10.0
		mov	\rp, #0x00009000
		orr	\rv, \rp, #0xf0000000	@ virtual base
		orr	\rp, \rp, #0x10000000
		.endm

#include <asm/hardware/debug-pl01x.S>
