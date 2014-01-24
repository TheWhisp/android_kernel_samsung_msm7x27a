/*
 * SMP support for R-Mobile / SH-Mobile
 *
 * Copyright (C) 2010  Magnus Damm
 * Copyright (C) 2010  Takashi Yoshii
 *
 * Based on vexpress, Copyright (c) 2003 ARM Limited, All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/linkage.h>
#include <linux/init.h>
#include <asm/memory.h>

<<<<<<< HEAD
<<<<<<< HEAD
	__INIT
=======
	__CPUINIT

/* Cache invalidation nicked from arch/arm/mach-imx/head-v7.S, thanks!
 *
 * The secondary kernel init calls v7_flush_dcache_all before it enables
 * the L1; however, the L1 comes out of reset in an undefined state, so
 * the clean + invalidate performed by v7_flush_dcache_all causes a bunch
 * of cache lines with uninitialized data and uninitialized tags to get
 * written out to memory, which does really unpleasant things to the main
 * processor.  We fix this by performing an invalidate, rather than a
 * clean + invalidate, before jumping into the kernel.
 *
 * This funciton is cloned from arch/arm/mach-tegra/headsmp.S, and needs
 * to be called for both secondary cores startup and primary core resume
 * procedures.  Ideally, it should be moved into arch/arm/mm/cache-v7.S.
 */
ENTRY(v7_invalidate_l1)
	mov	r0, #0
	mcr	p15, 0, r0, c7, c5, 0	@ invalidate I cache
	mcr	p15, 2, r0, c0, c0, 0
	mrc	p15, 1, r0, c0, c0, 0

	ldr	r1, =0x7fff
	and	r2, r1, r0, lsr #13

	ldr	r1, =0x3ff

	and	r3, r1, r0, lsr #3	@ NumWays - 1
	add	r2, r2, #1		@ NumSets

	and	r0, r0, #0x7
	add	r0, r0, #4	@ SetShift

	clz	r1, r3		@ WayShift
	add	r4, r3, #1	@ NumWays
1:	sub	r2, r2, #1	@ NumSets--
	mov	r3, r4		@ Temp = NumWays
2:	subs	r3, r3, #1	@ Temp--
	mov	r5, r3, lsl r1
	mov	r6, r2, lsl r0
	orr	r5, r5, r6	@ Reg = (Temp<<WayShift)|(NumSets<<SetShift)
	mcr	p15, 0, r5, c7, c6, 2
	bgt	2b
	cmp	r2, #0
	bgt	1b
	dsb
	isb
	mov	pc, lr
ENDPROC(v7_invalidate_l1)

=======
>>>>>>> refs/remotes/origin/master
ENTRY(shmobile_invalidate_start)
	bl	v7_invalidate_l1
	b	secondary_startup
ENDPROC(shmobile_invalidate_start)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

/*
 * Reset vector for secondary CPUs.
 * This will be mapped at address 0 by SBAR register.
 * We need _long_ jump to the physical address.
 */
<<<<<<< HEAD
	.align  12
ENTRY(shmobile_secondary_vector)
	ldr     pc, 1f
<<<<<<< HEAD
1:	.long   secondary_startup - PAGE_OFFSET + PLAT_PHYS_OFFSET
=======
1:	.long   shmobile_invalidate_start - PAGE_OFFSET + PLAT_PHYS_OFFSET
ENDPROC(shmobile_secondary_vector)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.arm
	.align  12
ENTRY(shmobile_boot_vector)
	ldr     r0, 2f
	ldr     r1, 1f
	bx	r1

ENDPROC(shmobile_boot_vector)

	.align	2
	.globl	shmobile_boot_fn
shmobile_boot_fn:
1:	.space	4
	.globl	shmobile_boot_arg
shmobile_boot_arg:
2:	.space	4
	.globl	shmobile_boot_size
shmobile_boot_size:
	.long	. - shmobile_boot_vector

/*
 * Per-CPU SMP boot function/argument selection code based on MPIDR
 */

ENTRY(shmobile_smp_boot)
						@ r0 = MPIDR_HWID_BITMASK
	mrc	p15, 0, r1, c0, c0, 5		@ r1 = MPIDR
	and	r0, r1, r0			@ r0 = cpu_logical_map() value
	mov	r1, #0				@ r1 = CPU index
	adr	r5, 1f				@ array of per-cpu mpidr values
	adr	r6, 2f				@ array of per-cpu functions
	adr	r7, 3f				@ array of per-cpu arguments

shmobile_smp_boot_find_mpidr:
	ldr	r8, [r5, r1, lsl #2]
	cmp	r8, r0
	bne	shmobile_smp_boot_next

	ldr	r9, [r6, r1, lsl #2]
	cmp	r9, #0
	bne	shmobile_smp_boot_found

shmobile_smp_boot_next:
	add	r1, r1, #1
	cmp	r1, #CONFIG_NR_CPUS
	blo	shmobile_smp_boot_find_mpidr

	b	shmobile_smp_sleep

shmobile_smp_boot_found:
	ldr	r0, [r7, r1, lsl #2]
	mov	pc, r9
ENDPROC(shmobile_smp_boot)

ENTRY(shmobile_smp_sleep)
	wfi
	b	shmobile_smp_boot
ENDPROC(shmobile_smp_sleep)

	.globl	shmobile_smp_mpidr
shmobile_smp_mpidr:
1:	.space	CONFIG_NR_CPUS * 4
	.globl	shmobile_smp_fn
shmobile_smp_fn:
2:	.space	CONFIG_NR_CPUS * 4
	.globl	shmobile_smp_arg
shmobile_smp_arg:
3:	.space	CONFIG_NR_CPUS * 4
>>>>>>> refs/remotes/origin/master
