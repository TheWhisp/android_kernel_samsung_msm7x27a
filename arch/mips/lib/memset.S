/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1998, 1999, 2000 by Ralf Baechle
 * Copyright (C) 1999, 2000 Silicon Graphics, Inc.
<<<<<<< HEAD
 * Copyright (C) 2007  Maciej W. Rozycki
=======
 * Copyright (C) 2007 by Maciej W. Rozycki
 * Copyright (C) 2011, 2012 MIPS Technologies, Inc.
>>>>>>> refs/remotes/origin/master
 */
#include <asm/asm.h>
#include <asm/asm-offsets.h>
#include <asm/regdef.h>

#if LONGSIZE == 4
#define LONG_S_L swl
#define LONG_S_R swr
#else
#define LONG_S_L sdl
#define LONG_S_R sdr
#endif

<<<<<<< HEAD
#define EX(insn,reg,addr,handler)			\
9:	insn	reg, addr;				\
	.section __ex_table,"a"; 			\
	PTR	9b, handler; 				\
	.previous

	.macro	f_fill64 dst, offset, val, fixup
	EX(LONG_S, \val, (\offset +  0 * LONGSIZE)(\dst), \fixup)
	EX(LONG_S, \val, (\offset +  1 * LONGSIZE)(\dst), \fixup)
	EX(LONG_S, \val, (\offset +  2 * LONGSIZE)(\dst), \fixup)
	EX(LONG_S, \val, (\offset +  3 * LONGSIZE)(\dst), \fixup)
	EX(LONG_S, \val, (\offset +  4 * LONGSIZE)(\dst), \fixup)
	EX(LONG_S, \val, (\offset +  5 * LONGSIZE)(\dst), \fixup)
	EX(LONG_S, \val, (\offset +  6 * LONGSIZE)(\dst), \fixup)
	EX(LONG_S, \val, (\offset +  7 * LONGSIZE)(\dst), \fixup)
#if LONGSIZE == 4
	EX(LONG_S, \val, (\offset +  8 * LONGSIZE)(\dst), \fixup)
	EX(LONG_S, \val, (\offset +  9 * LONGSIZE)(\dst), \fixup)
	EX(LONG_S, \val, (\offset + 10 * LONGSIZE)(\dst), \fixup)
	EX(LONG_S, \val, (\offset + 11 * LONGSIZE)(\dst), \fixup)
	EX(LONG_S, \val, (\offset + 12 * LONGSIZE)(\dst), \fixup)
	EX(LONG_S, \val, (\offset + 13 * LONGSIZE)(\dst), \fixup)
	EX(LONG_S, \val, (\offset + 14 * LONGSIZE)(\dst), \fixup)
	EX(LONG_S, \val, (\offset + 15 * LONGSIZE)(\dst), \fixup)
=======
#ifdef CONFIG_CPU_MICROMIPS
#define STORSIZE (LONGSIZE * 2)
#define STORMASK (STORSIZE - 1)
#define FILL64RG t8
#define FILLPTRG t7
#undef  LONG_S
#define LONG_S LONG_SP
#else
#define STORSIZE LONGSIZE
#define STORMASK LONGMASK
#define FILL64RG a1
#define FILLPTRG t0
#endif

#define EX(insn,reg,addr,handler)			\
9:	insn	reg, addr;				\
	.section __ex_table,"a";			\
	PTR	9b, handler;				\
	.previous

	.macro	f_fill64 dst, offset, val, fixup
	EX(LONG_S, \val, (\offset +  0 * STORSIZE)(\dst), \fixup)
	EX(LONG_S, \val, (\offset +  1 * STORSIZE)(\dst), \fixup)
	EX(LONG_S, \val, (\offset +  2 * STORSIZE)(\dst), \fixup)
	EX(LONG_S, \val, (\offset +  3 * STORSIZE)(\dst), \fixup)
#if ((defined(CONFIG_CPU_MICROMIPS) && (LONGSIZE == 4)) || !defined(CONFIG_CPU_MICROMIPS))
	EX(LONG_S, \val, (\offset +  4 * STORSIZE)(\dst), \fixup)
	EX(LONG_S, \val, (\offset +  5 * STORSIZE)(\dst), \fixup)
	EX(LONG_S, \val, (\offset +  6 * STORSIZE)(\dst), \fixup)
	EX(LONG_S, \val, (\offset +  7 * STORSIZE)(\dst), \fixup)
#endif
#if (!defined(CONFIG_CPU_MICROMIPS) && (LONGSIZE == 4))
	EX(LONG_S, \val, (\offset +  8 * STORSIZE)(\dst), \fixup)
	EX(LONG_S, \val, (\offset +  9 * STORSIZE)(\dst), \fixup)
	EX(LONG_S, \val, (\offset + 10 * STORSIZE)(\dst), \fixup)
	EX(LONG_S, \val, (\offset + 11 * STORSIZE)(\dst), \fixup)
	EX(LONG_S, \val, (\offset + 12 * STORSIZE)(\dst), \fixup)
	EX(LONG_S, \val, (\offset + 13 * STORSIZE)(\dst), \fixup)
	EX(LONG_S, \val, (\offset + 14 * STORSIZE)(\dst), \fixup)
	EX(LONG_S, \val, (\offset + 15 * STORSIZE)(\dst), \fixup)
>>>>>>> refs/remotes/origin/master
#endif
	.endm

/*
 * memset(void *s, int c, size_t n)
 *
 * a0: start of area to clear
 * a1: char to fill with
 * a2: size of area to clear
 */
	.set	noreorder
	.align	5
LEAF(memset)
	beqz		a1, 1f
	 move		v0, a0			/* result */

	andi		a1, 0xff		/* spread fillword */
	LONG_SLL		t1, a1, 8
	or		a1, t1
	LONG_SLL		t1, a1, 16
#if LONGSIZE == 8
	or		a1, t1
	LONG_SLL		t1, a1, 32
#endif
	or		a1, t1
1:

FEXPORT(__bzero)
<<<<<<< HEAD
	sltiu		t0, a2, LONGSIZE	/* very small region? */
	bnez		t0, .Lsmall_memset
	 andi		t0, a0, LONGMASK	/* aligned? */

#ifndef CONFIG_CPU_DADDI_WORKAROUNDS
	beqz		t0, 1f
	 PTR_SUBU	t0, LONGSIZE		/* alignment in bytes */
#else
	.set		noat
	li		AT, LONGSIZE
=======
	sltiu		t0, a2, STORSIZE	/* very small region? */
	bnez		t0, .Lsmall_memset
	 andi		t0, a0, STORMASK	/* aligned? */

#ifdef CONFIG_CPU_MICROMIPS
	move		t8, a1			/* used by 'swp' instruction */
	move		t9, a1
#endif
#ifndef CONFIG_CPU_DADDI_WORKAROUNDS
	beqz		t0, 1f
	 PTR_SUBU	t0, STORSIZE		/* alignment in bytes */
#else
	.set		noat
	li		AT, STORSIZE
>>>>>>> refs/remotes/origin/master
	beqz		t0, 1f
	 PTR_SUBU	t0, AT			/* alignment in bytes */
	.set		at
#endif

	R10KCBARRIER(0(ra))
#ifdef __MIPSEB__
	EX(LONG_S_L, a1, (a0), .Lfirst_fixup)	/* make word/dword aligned */
#endif
#ifdef __MIPSEL__
	EX(LONG_S_R, a1, (a0), .Lfirst_fixup)	/* make word/dword aligned */
#endif
	PTR_SUBU	a0, t0			/* long align ptr */
	PTR_ADDU	a2, t0			/* correct size */

1:	ori		t1, a2, 0x3f		/* # of full blocks */
	xori		t1, 0x3f
	beqz		t1, .Lmemset_partial	/* no block to fill */
<<<<<<< HEAD
	 andi		t0, a2, 0x40-LONGSIZE
=======
	 andi		t0, a2, 0x40-STORSIZE
>>>>>>> refs/remotes/origin/master

	PTR_ADDU	t1, a0			/* end address */
	.set		reorder
1:	PTR_ADDIU	a0, 64
	R10KCBARRIER(0(ra))
<<<<<<< HEAD
	f_fill64 a0, -64, a1, .Lfwd_fixup
=======
	f_fill64 a0, -64, FILL64RG, .Lfwd_fixup
>>>>>>> refs/remotes/origin/master
	bne		t1, a0, 1b
	.set		noreorder

.Lmemset_partial:
	R10KCBARRIER(0(ra))
	PTR_LA		t1, 2f			/* where to start */
<<<<<<< HEAD
#if LONGSIZE == 4
	PTR_SUBU	t1, t0
#else
	.set		noat
	LONG_SRL		AT, t0, 1
=======
#ifdef CONFIG_CPU_MICROMIPS
	LONG_SRL	t7, t0, 1
#endif
#if LONGSIZE == 4
	PTR_SUBU	t1, FILLPTRG
#else
	.set		noat
	LONG_SRL	AT, FILLPTRG, 1
>>>>>>> refs/remotes/origin/master
	PTR_SUBU	t1, AT
	.set		at
#endif
	jr		t1
	 PTR_ADDU	a0, t0			/* dest ptr */

	.set		push
	.set		noreorder
	.set		nomacro
<<<<<<< HEAD
	f_fill64 a0, -64, a1, .Lpartial_fixup	/* ... but first do longs ... */
2:	.set		pop
	andi		a2, LONGMASK		/* At most one long to go */
=======
	f_fill64 a0, -64, FILL64RG, .Lpartial_fixup	/* ... but first do longs ... */
2:	.set		pop
	andi		a2, STORMASK		/* At most one long to go */
>>>>>>> refs/remotes/origin/master

	beqz		a2, 1f
	 PTR_ADDU	a0, a2			/* What's left */
	R10KCBARRIER(0(ra))
#ifdef __MIPSEB__
	EX(LONG_S_R, a1, -1(a0), .Llast_fixup)
#endif
#ifdef __MIPSEL__
	EX(LONG_S_L, a1, -1(a0), .Llast_fixup)
#endif
1:	jr		ra
	 move		a2, zero

.Lsmall_memset:
	beqz		a2, 2f
	 PTR_ADDU	t1, a0, a2

1:	PTR_ADDIU	a0, 1			/* fill bytewise */
	R10KCBARRIER(0(ra))
	bne		t1, a0, 1b
	 sb		a1, -1(a0)

2:	jr		ra			/* done */
	 move		a2, zero
	END(memset)

.Lfirst_fixup:
	jr	ra
	 nop

.Lfwd_fixup:
	PTR_L		t0, TI_TASK($28)
	andi		a2, 0x3f
	LONG_L		t0, THREAD_BUADDR(t0)
	LONG_ADDU	a2, t1
	jr		ra
	 LONG_SUBU	a2, t0

.Lpartial_fixup:
	PTR_L		t0, TI_TASK($28)
<<<<<<< HEAD
	andi		a2, LONGMASK
=======
	andi		a2, STORMASK
>>>>>>> refs/remotes/origin/master
	LONG_L		t0, THREAD_BUADDR(t0)
	LONG_ADDU	a2, t1
	jr		ra
	 LONG_SUBU	a2, t0

.Llast_fixup:
	jr		ra
<<<<<<< HEAD
	 andi		v1, a2, LONGMASK
=======
	 andi		v1, a2, STORMASK
>>>>>>> refs/remotes/origin/master
