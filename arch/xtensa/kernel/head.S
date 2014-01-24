/*
 * arch/xtensa/kernel/head.S
 *
 * Xtensa Processor startup code.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
<<<<<<< HEAD
 * Copyright (C) 2001 - 2005 Tensilica Inc.
=======
 * Copyright (C) 2001 - 2008 Tensilica Inc.
>>>>>>> refs/remotes/origin/master
 *
 * Chris Zankel <chris@zankel.net>
 * Marc Gauthier <marc@tensilica.com, marc@alumni.uwaterloo.ca>
 * Joe Taylor <joe@tensilica.com, joetylr@yahoo.com>
 * Kevin Chea
 */

#include <asm/processor.h>
#include <asm/page.h>
#include <asm/cacheasm.h>
<<<<<<< HEAD
=======
#include <asm/initialize_mmu.h>
>>>>>>> refs/remotes/origin/master

#include <linux/init.h>
#include <linux/linkage.h>

/*
 * This module contains the entry code for kernel images. It performs the
 * minimal setup needed to call the generic C routines.
 *
 * Prerequisites:
 *
 * - The kernel image has been loaded to the actual address where it was
 *   compiled to.
 * - a2 contains either 0 or a pointer to a list of boot parameters.
 *   (see setup.c for more details)
 *
 */

/*
 *  _start
 *
 *  The bootloader passes a pointer to a list of boot parameters in a2.
 */

	/* The first bytes of the kernel image must be an instruction, so we
	 * manually allocate and define the literal constant we need for a jx
	 * instruction.
	 */

	__HEAD
<<<<<<< HEAD
	.globl _start
_start:	_j	2f
	.align	4
1:	.word	_startup
2:	l32r	a0, 1b
	jx	a0

	.section .init.text, "ax"
	.align 4
_startup:

	/* Disable interrupts and exceptions. */

	movi	a0, LOCKLEVEL
	wsr	a0, PS

	/* Preserve the pointer to the boot parameter list in EXCSAVE_1 */

	wsr	a2, EXCSAVE_1
=======
	.begin	no-absolute-literals

ENTRY(_start)

	/* Preserve the pointer to the boot parameter list in EXCSAVE_1 */
	wsr     a2, excsave1
	_j	_SetupMMU

	.align	4
	.literal_position
.Lstartup:
	.word	_startup

	.align	4
	.global _SetupMMU
_SetupMMU:
	Offset = _SetupMMU - _start

#ifdef CONFIG_INITIALIZE_XTENSA_MMU_INSIDE_VMLINUX
	initialize_mmu
#if defined(CONFIG_MMU) && XCHAL_HAVE_PTP_MMU && XCHAL_HAVE_SPANNING_WAY
	rsr	a2, excsave1
	movi	a3, 0x08000000
	bgeu	a2, a3, 1f
	movi	a3, 0xd0000000
	add	a2, a2, a3
	wsr	a2, excsave1
1:
#endif
#endif
	.end	no-absolute-literals

	l32r	a0, .Lstartup
	jx	a0

ENDPROC(_start)

	__INIT
	.literal_position

ENTRY(_startup)

	/* Disable interrupts and exceptions. */

	movi	a0, LOCKLEVEL
	wsr	a0, ps
>>>>>>> refs/remotes/origin/master

	/* Start with a fresh windowbase and windowstart.  */

	movi	a1, 1
	movi	a0, 0
<<<<<<< HEAD
	wsr	a1, WINDOWSTART
	wsr	a0, WINDOWBASE
=======
	wsr	a1, windowstart
	wsr	a0, windowbase
>>>>>>> refs/remotes/origin/master
	rsync

	/* Set a0 to 0 for the remaining initialization. */

	movi	a0, 0

	/* Clear debugging registers. */

#if XCHAL_HAVE_DEBUG
<<<<<<< HEAD
	wsr	a0, IBREAKENABLE
	wsr	a0, ICOUNT
	movi	a1, 15
	wsr	a0, ICOUNTLEVEL

	.set	_index, 0
	.rept	XCHAL_NUM_DBREAK - 1
	wsr	a0, DBREAKC + _index
=======
#if XCHAL_NUM_IBREAK > 0
	wsr	a0, ibreakenable
#endif
	wsr	a0, icount
	movi	a1, 15
	wsr	a0, icountlevel

	.set	_index, 0
	.rept	XCHAL_NUM_DBREAK - 1
	wsr	a0, SREG_DBREAKC + _index
>>>>>>> refs/remotes/origin/master
	.set	_index, _index + 1
	.endr
#endif

	/* Clear CCOUNT (not really necessary, but nice) */

<<<<<<< HEAD
	wsr	a0, CCOUNT	# not really necessary, but nice
=======
	wsr	a0, ccount	# not really necessary, but nice
>>>>>>> refs/remotes/origin/master

	/* Disable zero-loops. */

#if XCHAL_HAVE_LOOPS
<<<<<<< HEAD
	wsr	a0, LCOUNT
=======
	wsr	a0, lcount
>>>>>>> refs/remotes/origin/master
#endif

	/* Disable all timers. */

	.set	_index, 0
<<<<<<< HEAD
	.rept	XCHAL_NUM_TIMERS - 1
	wsr	a0, CCOMPARE + _index
=======
	.rept	XCHAL_NUM_TIMERS
	wsr	a0, SREG_CCOMPARE + _index
>>>>>>> refs/remotes/origin/master
	.set	_index, _index + 1
	.endr

	/* Interrupt initialization. */

	movi	a2, XCHAL_INTTYPE_MASK_SOFTWARE | XCHAL_INTTYPE_MASK_EXTERN_EDGE
<<<<<<< HEAD
	wsr	a0, INTENABLE
	wsr	a2, INTCLEAR

	/* Disable coprocessors. */

#if XCHAL_CP_NUM > 0
	wsr	a0, CPENABLE
#endif

	/* Set PS.INTLEVEL=1, PS.WOE=0, kernel stack, PS.EXCM=0
=======
	wsr	a0, intenable
	wsr	a2, intclear

	/* Disable coprocessors. */

#if XCHAL_HAVE_CP
	wsr	a0, cpenable
#endif

	/* Set PS.INTLEVEL=LOCKLEVEL, PS.WOE=0, kernel stack, PS.EXCM=0
>>>>>>> refs/remotes/origin/master
	 *
	 * Note: PS.EXCM must be cleared before using any loop
	 *	 instructions; otherwise, they are silently disabled, and
	 * 	 at most one iteration of the loop is executed.
	 */

<<<<<<< HEAD
	movi	a1, 1
	wsr	a1, PS
=======
	movi	a1, LOCKLEVEL
	wsr	a1, ps
>>>>>>> refs/remotes/origin/master
	rsync

	/*  Initialize the caches.
	 *  a2, a3 are just working registers (clobbered).
	 */

#if XCHAL_DCACHE_LINE_LOCKABLE
	___unlock_dcache_all a2 a3
#endif

#if XCHAL_ICACHE_LINE_LOCKABLE
	___unlock_icache_all a2 a3
#endif

	___invalidate_dcache_all a2 a3
	___invalidate_icache_all a2 a3

	isync

	/* Unpack data sections
	 *
	 * The linker script used to build the Linux kernel image
	 * creates a table located at __boot_reloc_table_start
	 * that contans the information what data needs to be unpacked.
	 *
	 * Uses a2-a7.
	 */

	movi	a2, __boot_reloc_table_start
	movi	a3, __boot_reloc_table_end

1:	beq	a2, a3, 3f	# no more entries?
	l32i	a4, a2, 0	# start destination (in RAM)
	l32i	a5, a2, 4	# end desination (in RAM)
	l32i	a6, a2, 8	# start source (in ROM)
	addi	a2, a2, 12	# next entry
	beq	a4, a5, 1b	# skip, empty entry
	beq	a4, a6, 1b	# skip, source and dest. are the same

2:	l32i	a7, a6, 0	# load word
	addi	a6, a6, 4
	s32i	a7, a4, 0	# store word
	addi	a4, a4, 4
	bltu	a4, a5, 2b
	j	1b

3:
	/* All code and initialized data segments have been copied.
	 * Now clear the BSS segment.
	 */

	movi	a2, __bss_start	# start of BSS
	movi	a3, __bss_stop	# end of BSS

	__loopt	a2, a3, a4, 2
	s32i	a0, a2, 0
	__endla	a2, a4, 4

#if XCHAL_DCACHE_IS_WRITEBACK

	/* After unpacking, flush the writeback cache to memory so the
	 * instructions/data are available.
	 */

	___flush_dcache_all a2 a3
#endif
<<<<<<< HEAD
=======
	memw
	isync
	___invalidate_icache_all a2 a3
	isync
>>>>>>> refs/remotes/origin/master

	/* Setup stack and enable window exceptions (keep irqs disabled) */

	movi	a1, init_thread_union
	addi	a1, a1, KERNEL_STACK_SIZE

<<<<<<< HEAD
	movi	a2, 0x00040001		# WOE=1, INTLEVEL=1, UM=0
	wsr	a2, PS			# (enable reg-windows; progmode stack)
=======
	movi	a2, (1 << PS_WOE_BIT) | LOCKLEVEL
					# WOE=1, INTLEVEL=LOCKLEVEL, UM=0
	wsr	a2, ps			# (enable reg-windows; progmode stack)
>>>>>>> refs/remotes/origin/master
	rsync

	/* Set up EXCSAVE[DEBUGLEVEL] to point to the Debug Exception Handler.*/

	movi	a2, debug_exception
<<<<<<< HEAD
	wsr	a2, EXCSAVE + XCHAL_DEBUGLEVEL
=======
	wsr	a2, SREG_EXCSAVE + XCHAL_DEBUGLEVEL
>>>>>>> refs/remotes/origin/master

	/* Set up EXCSAVE[1] to point to the exc_table. */

	movi	a6, exc_table
<<<<<<< HEAD
	xsr	a6, EXCSAVE_1
=======
	xsr	a6, excsave1
>>>>>>> refs/remotes/origin/master

	/* init_arch kick-starts the linux kernel */

	movi	a4, init_arch
	callx4	a4

	movi	a4, start_kernel
	callx4	a4

should_never_return:
	j	should_never_return

<<<<<<< HEAD
=======
ENDPROC(_startup)
>>>>>>> refs/remotes/origin/master

/*
 * BSS section
 */
	
__PAGE_ALIGNED_BSS
#ifdef CONFIG_MMU
ENTRY(swapper_pg_dir)
	.fill	PAGE_SIZE, 1, 0
<<<<<<< HEAD
#endif
ENTRY(empty_zero_page)
	.fill	PAGE_SIZE, 1, 0
=======
END(swapper_pg_dir)
#endif
ENTRY(empty_zero_page)
	.fill	PAGE_SIZE, 1, 0
END(empty_zero_page)
>>>>>>> refs/remotes/origin/master
