/*
 * Debugging macro include header
 *
 *  Copyright (C) 1994-1999 Russell King
 *  Moved from linux/arch/arm/kernel/debug.S by Ben Dooks
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
*/

<<<<<<< HEAD
		.macro	addruart, rp, rv
=======
		.macro	addruart, rp, rv, tmp
>>>>>>> refs/remotes/origin/cm-10.0
		mov	\rp, #0x00100000
		add	\rp, \rp, #0x000fb000
		add	\rv, \rp, #0xf0000000	@ virtual base
		add	\rp, \rp, #0x10000000	@ physical base address
		.endm

#include <asm/hardware/debug-pl01x.S>
