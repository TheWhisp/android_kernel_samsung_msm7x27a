/*
 * arch/arm/mach-orion5x/include/mach/debug-macro.S
 *
 * Debugging macro include header
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <mach/orion5x.h>

<<<<<<< HEAD
	.macro  addruart, rp, rv
=======
	.macro  addruart, rp, rv, tmp
>>>>>>> refs/remotes/origin/cm-10.0
	ldr	\rp, =ORION5X_REGS_PHYS_BASE
	ldr	\rv, =ORION5X_REGS_VIRT_BASE
	orr	\rp, \rp, #0x00012000
	orr	\rv, \rv, #0x00012000
	.endm

#define UART_SHIFT	2
#include <asm/hardware/debug-8250.S>
