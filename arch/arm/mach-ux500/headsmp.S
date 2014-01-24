/*
 *  Copyright (c) 2009 ST-Ericsson
 *	This file is based  ARM Realview platform
 *  Copyright (c) 2003 ARM Limited
 *  All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/linkage.h>
#include <linux/init.h>

<<<<<<< HEAD
	__INIT

=======
>>>>>>> refs/remotes/origin/master
/*
 * U8500 specific entry point for secondary CPUs.
 */
ENTRY(u8500_secondary_startup)
	mrc	p15, 0, r0, c0, c0, 5
	and	r0, r0, #15
	adr	r4, 1f
	ldmia	r4, {r5, r6}
	sub	r4, r4, r5
	add	r6, r6, r4
pen:	ldr	r7, [r6]
	cmp	r7, r0
	bne	pen

	/*
	 * we've been released from the holding pen: secondary_stack
	 * should now contain the SVC stack for this core
	 */
	b	secondary_startup
<<<<<<< HEAD
<<<<<<< HEAD

=======
ENDPROC(u8500_secondary_startup)

	.align 2
>>>>>>> refs/remotes/origin/cm-10.0
=======
ENDPROC(u8500_secondary_startup)

	.align 2
>>>>>>> refs/remotes/origin/master
1:	.long	.
	.long	pen_release
