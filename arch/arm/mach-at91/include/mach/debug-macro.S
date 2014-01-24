/*
 * arch/arm/mach-at91/include/mach/debug-macro.S
 *
 *  Copyright (C) 2003-2005 SAN People
 *
 * Debugging macro include header
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
*/

#include <mach/hardware.h>
#include <mach/at91_dbgu.h>

<<<<<<< HEAD
	.macro	addruart, rp, rv
	ldr	\rp, =(AT91_BASE_SYS + AT91_DBGU)		@ System peripherals (phys address)
	ldr	\rv, =(AT91_VA_BASE_SYS	+ AT91_DBGU)		@ System peripherals (virt address)
	.endm

	.macro	senduart,rd,rx
	strb	\rd, [\rx, #(AT91_DBGU_THR - AT91_DBGU)]	@ Write to Transmitter Holding Register
	.endm

	.macro	waituart,rd,rx
1001:	ldr	\rd, [\rx, #(AT91_DBGU_SR - AT91_DBGU)]		@ Read Status Register
	tst	\rd, #AT91_DBGU_TXRDY				@ DBGU_TXRDY = 1 when ready to transmit
=======
#if defined(CONFIG_AT91_DEBUG_LL_DBGU0)
#define AT91_DBGU AT91_BASE_DBGU0
#else
#define AT91_DBGU AT91_BASE_DBGU1
#endif

	.macro	addruart, rp, rv, tmp
	ldr	\rp, =AT91_DBGU				@ System peripherals (phys address)
	ldr	\rv, =AT91_IO_P2V(AT91_DBGU)		@ System peripherals (virt address)
	.endm

	.macro	senduart,rd,rx
	strb	\rd, [\rx, #(AT91_DBGU_THR)]		@ Write to Transmitter Holding Register
	.endm

	.macro	waituart,rd,rx
1001:	ldr	\rd, [\rx, #(AT91_DBGU_SR)]		@ Read Status Register
	tst	\rd, #AT91_DBGU_TXRDY			@ DBGU_TXRDY = 1 when ready to transmit
>>>>>>> refs/remotes/origin/cm-10.0
	beq	1001b
	.endm

	.macro	busyuart,rd,rx
<<<<<<< HEAD
1001:	ldr	\rd, [\rx, #(AT91_DBGU_SR - AT91_DBGU)]		@ Read Status Register
	tst	\rd, #AT91_DBGU_TXEMPTY				@ DBGU_TXEMPTY = 1 when transmission complete
=======
1001:	ldr	\rd, [\rx, #(AT91_DBGU_SR)]		@ Read Status Register
	tst	\rd, #AT91_DBGU_TXEMPTY			@ DBGU_TXEMPTY = 1 when transmission complete
>>>>>>> refs/remotes/origin/cm-10.0
	beq	1001b
	.endm

