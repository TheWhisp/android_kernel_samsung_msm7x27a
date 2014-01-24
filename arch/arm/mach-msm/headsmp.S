/*
<<<<<<< HEAD
 *  Copyright (c) 2003 ARM Limited
 *  All Rights Reserved
<<<<<<< HEAD
 *  Copyright (c) 2010, The Linux Foundation. All rights reserved.
=======
 *  Copyright (c) 2010, 2012 The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 *  linux/arch/arm/mach-realview/headsmp.S
 *
 *  Copyright (c) 2003 ARM Limited
 *  All Rights Reserved
>>>>>>> refs/remotes/origin/master
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/linkage.h>
#include <linux/init.h>

<<<<<<< HEAD
<<<<<<< HEAD
=======
__CPUINIT

>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * MSM specific entry point for secondary CPUs.  This provides
 * a "holding pen" into which all secondary cores are held until we're
 * ready for them to initialise.
<<<<<<< HEAD
 *
 * This is executing in physical space with cache's off.
 */
ENTRY(msm_secondary_startup)
	mrc	p15, 0, r0, c0, c0, 5 	@ MPIDR
	and	r0, r0, #15		@ What CPU am I
	adr	r4, 1f			@ address of
	ldmia	r4, {r5, r6}		@ load curr addr and pen_rel addr
	sub	r4, r4, r5		@ determine virtual/phys offsets
	add	r6, r6, r4		@ apply
pen:
<<<<<<< HEAD
	wfe
	dsb				@ ensure subsequent access is
					@ after event

=======
>>>>>>> refs/remotes/origin/cm-10.0
	ldr	r7, [r6]		@ pen_rel has cpu to remove from reset
	cmp	r7, r0			@ are we lucky?
=======
 */
ENTRY(msm_secondary_startup)
	mrc	p15, 0, r0, c0, c0, 5
	and	r0, r0, #15
	adr	r4, 1f
	ldmia	r4, {r5, r6}
	sub	r4, r4, r5
	add	r6, r6, r4
pen:	ldr	r7, [r6]
	cmp	r7, r0
>>>>>>> refs/remotes/origin/master
	bne	pen

	/*
	 * we've been released from the holding pen: secondary_stack
	 * should now contain the SVC stack for this core
	 */
<<<<<<< HEAD
	mvn	r7, #0			@ -1 to registers
	str r7,[r6]			@ back to the pen for ack
	b	secondary_startup
<<<<<<< HEAD
=======
ENDPROC(msm_secondary_startup)
>>>>>>> refs/remotes/origin/cm-10.0

=======
	b	secondary_startup
ENDPROC(msm_secondary_startup)

	.align
>>>>>>> refs/remotes/origin/master
1:	.long	.
	.long	pen_release
