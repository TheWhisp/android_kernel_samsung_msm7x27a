<<<<<<< HEAD
<<<<<<< HEAD
/*
 *  Definitions for use by exception code on Book3-E
 *
 *  Copyright (C) 2008 Ben. Herrenschmidt (benh@kernel.crashing.org), IBM Corp.
=======
=======
>>>>>>> refs/remotes/origin/master
#ifndef _ASM_POWERPC_EXCEPTION_H
#define _ASM_POWERPC_EXCEPTION_H
/*
 * Extracted from head_64.S
 *
 *  PowerPC version
 *    Copyright (C) 1995-1996 Gary Thomas (gdt@linuxppc.org)
 *
 *  Rewritten by Cort Dougan (cort@cs.nmt.edu) for PReP
 *    Copyright (C) 1996 Cort Dougan <cort@cs.nmt.edu>
 *  Adapted for Power Macintosh by Paul Mackerras.
 *  Low-level exception handlers and MMU support
 *  rewritten by Paul Mackerras.
 *    Copyright (C) 1996 Paul Mackerras.
 *
 *  Adapted for 64bit PowerPC by Dave Engebretsen, Peter Bergner, and
 *    Mike Corrigan {engebret|bergner|mikejc}@us.ibm.com
 *
 *  This file contains the low-level support and setup for the
 *  PowerPC-64 platform, including trap and interrupt dispatch.
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version
 *  2 of the License, or (at your option) any later version.
 */
<<<<<<< HEAD
<<<<<<< HEAD
#ifndef _ASM_POWERPC_EXCEPTION_64E_H
#define _ASM_POWERPC_EXCEPTION_64E_H

/*
 * SPRGs usage an other considerations...
 *
 * Since TLB miss and other standard exceptions can be interrupted by
 * critical exceptions which can themselves be interrupted by machine
 * checks, and since the two later can themselves cause a TLB miss when
 * hitting the linear mapping for the kernel stacks, we need to be a bit
 * creative on how we use SPRGs.
 *
 * The base idea is that we have one SRPG reserved for critical and one
 * for machine check interrupts. Those are used to save a GPR that can
 * then be used to get the PACA, and store as much context as we need
 * to save in there. That includes saving the SPRGs used by the TLB miss
 * handler for linear mapping misses and the associated SRR0/1 due to
 * the above re-entrancy issue.
 *
 * So here's the current usage pattern. It's done regardless of which
 * SPRGs are user-readable though, thus we might have to change some of
 * this later. In order to do that more easily, we use special constants
 * for naming them
 *
 * WARNING: Some of these SPRGs are user readable. We need to do something
 * about it as some point by making sure they can't be used to leak kernel
 * critical data
 */


/* We are out of SPRGs so we save some things in the PACA. The normal
 * exception frame is smaller than the CRIT or MC one though
 */
#define EX_R1		(0 * 8)
#define EX_CR		(1 * 8)
#define EX_R10		(2 * 8)
#define EX_R11		(3 * 8)
#define EX_R14		(4 * 8)
#define EX_R15		(5 * 8)

/*
 * The TLB miss exception uses different slots.
 *
 * The bolted variant uses only the first six fields,
 * which in combination with pgd and kernel_pgd fits in
 * one 64-byte cache line.
 */

#define EX_TLB_R10	( 0 * 8)
#define EX_TLB_R11	( 1 * 8)
#define EX_TLB_R14	( 2 * 8)
#define EX_TLB_R15	( 3 * 8)
#define EX_TLB_R16	( 4 * 8)
#define EX_TLB_CR	( 5 * 8)
#define EX_TLB_R12	( 6 * 8)
#define EX_TLB_R13	( 7 * 8)
#define EX_TLB_DEAR	( 8 * 8) /* Level 0 and 2 only */
#define EX_TLB_ESR	( 9 * 8) /* Level 0 and 2 only */
#define EX_TLB_SRR0	(10 * 8)
#define EX_TLB_SRR1	(11 * 8)
#ifdef CONFIG_BOOK3E_MMU_TLB_STATS
#define EX_TLB_R8	(12 * 8)
#define EX_TLB_R9	(13 * 8)
#define EX_TLB_LR	(14 * 8)
#define EX_TLB_SIZE	(15 * 8)
#else
#define EX_TLB_SIZE	(12 * 8)
#endif

#define	START_EXCEPTION(label)						\
	.globl exc_##label##_book3e;					\
exc_##label##_book3e:

/* TLB miss exception prolog
 *
 * This prolog handles re-entrancy (up to 3 levels supported in the PACA
 * though we currently don't test for overflow). It provides you with a
 * re-entrancy safe working space of r10...r16 and CR with r12 being used
 * as the exception area pointer in the PACA for that level of re-entrancy
 * and r13 containing the PACA pointer.
 *
 * SRR0 and SRR1 are saved, but DEAR and ESR are not, since they don't apply
 * as-is for instruction exceptions. It's up to the actual exception code
 * to save them as well if required.
 */
#define TLB_MISS_PROLOG							    \
	mtspr	SPRN_SPRG_TLB_SCRATCH,r12;				    \
	mfspr	r12,SPRN_SPRG_TLB_EXFRAME;				    \
	std	r10,EX_TLB_R10(r12);					    \
	mfcr	r10;							    \
	std	r11,EX_TLB_R11(r12);					    \
	mfspr	r11,SPRN_SPRG_TLB_SCRATCH;				    \
	std	r13,EX_TLB_R13(r12);					    \
	mfspr	r13,SPRN_SPRG_PACA;					    \
	std	r14,EX_TLB_R14(r12);					    \
	addi	r14,r12,EX_TLB_SIZE;					    \
	std	r15,EX_TLB_R15(r12);					    \
	mfspr	r15,SPRN_SRR1;						    \
	std	r16,EX_TLB_R16(r12);					    \
	mfspr	r16,SPRN_SRR0;						    \
	std	r10,EX_TLB_CR(r12);					    \
	std	r11,EX_TLB_R12(r12);					    \
	mtspr	SPRN_SPRG_TLB_EXFRAME,r14;				    \
	std	r15,EX_TLB_SRR1(r12);					    \
	std	r16,EX_TLB_SRR0(r12);					    \
	TLB_MISS_PROLOG_STATS

/* And these are the matching epilogs that restores things
 *
 * There are 3 epilogs:
 *
 * - SUCCESS       : Unwinds one level
 * - ERROR         : restore from level 0 and reset
 * - ERROR_SPECIAL : restore from current level and reset
 *
 * Normal errors use ERROR, that is, they restore the initial fault context
 * and trigger a fault. However, there is a special case for linear mapping
 * errors. Those should basically never happen, but if they do happen, we
 * want the error to point out the context that did that linear mapping
 * fault, not the initial level 0 (basically, we got a bogus PGF or something
 * like that). For userland errors on the linear mapping, there is no
 * difference since those are always level 0 anyway
 */

#define TLB_MISS_RESTORE(freg)						    \
	ld	r14,EX_TLB_CR(r12);					    \
	ld	r10,EX_TLB_R10(r12);					    \
	ld	r15,EX_TLB_SRR0(r12);					    \
	ld	r16,EX_TLB_SRR1(r12);					    \
	mtspr	SPRN_SPRG_TLB_EXFRAME,freg;				    \
	ld	r11,EX_TLB_R11(r12);					    \
	mtcr	r14;							    \
	ld	r13,EX_TLB_R13(r12);					    \
	ld	r14,EX_TLB_R14(r12);					    \
	mtspr	SPRN_SRR0,r15;						    \
	ld	r15,EX_TLB_R15(r12);					    \
	mtspr	SPRN_SRR1,r16;						    \
	TLB_MISS_RESTORE_STATS						    \
	ld	r16,EX_TLB_R16(r12);					    \
	ld	r12,EX_TLB_R12(r12);					    \

#define TLB_MISS_EPILOG_SUCCESS						    \
	TLB_MISS_RESTORE(r12)

#define TLB_MISS_EPILOG_ERROR						    \
	addi	r12,r13,PACA_EXTLB;					    \
	TLB_MISS_RESTORE(r12)

#define TLB_MISS_EPILOG_ERROR_SPECIAL					    \
	addi	r11,r13,PACA_EXTLB;					    \
	TLB_MISS_RESTORE(r11)

#ifdef CONFIG_BOOK3E_MMU_TLB_STATS
#define TLB_MISS_PROLOG_STATS						    \
	mflr	r10;							    \
	std	r8,EX_TLB_R8(r12);					    \
	std	r9,EX_TLB_R9(r12);					    \
	std	r10,EX_TLB_LR(r12);
#define TLB_MISS_RESTORE_STATS					            \
	ld	r16,EX_TLB_LR(r12);					    \
	ld	r9,EX_TLB_R9(r12);					    \
	ld	r8,EX_TLB_R8(r12);					    \
	mtlr	r16;
#define TLB_MISS_PROLOG_STATS_BOLTED						    \
	mflr	r10;							    \
	std	r8,PACA_EXTLB+EX_TLB_R8(r13);				    \
	std	r9,PACA_EXTLB+EX_TLB_R9(r13);				    \
	std	r10,PACA_EXTLB+EX_TLB_LR(r13);
#define TLB_MISS_RESTORE_STATS_BOLTED					            \
	ld	r16,PACA_EXTLB+EX_TLB_LR(r13);				    \
	ld	r9,PACA_EXTLB+EX_TLB_R9(r13);				    \
	ld	r8,PACA_EXTLB+EX_TLB_R8(r13);				    \
	mtlr	r16;
#define TLB_MISS_STATS_D(name)						    \
	addi	r9,r13,MMSTAT_DSTATS+name;				    \
	bl	.tlb_stat_inc;
#define TLB_MISS_STATS_I(name)						    \
	addi	r9,r13,MMSTAT_ISTATS+name;				    \
	bl	.tlb_stat_inc;
#define TLB_MISS_STATS_X(name)						    \
	ld	r8,PACA_EXTLB+EX_TLB_ESR(r13);				    \
	cmpdi	cr2,r8,-1;						    \
	beq	cr2,61f;						    \
	addi	r9,r13,MMSTAT_DSTATS+name;				    \
	b	62f;							    \
61:	addi	r9,r13,MMSTAT_ISTATS+name;				    \
62:	bl	.tlb_stat_inc;
#define TLB_MISS_STATS_SAVE_INFO					    \
	std	r14,EX_TLB_ESR(r12);	/* save ESR */
#define TLB_MISS_STATS_SAVE_INFO_BOLTED					    \
	std	r14,PACA_EXTLB+EX_TLB_ESR(r13);	/* save ESR */
#else
#define TLB_MISS_PROLOG_STATS
#define TLB_MISS_RESTORE_STATS
#define TLB_MISS_PROLOG_STATS_BOLTED
#define TLB_MISS_RESTORE_STATS_BOLTED
#define TLB_MISS_STATS_D(name)
#define TLB_MISS_STATS_I(name)
#define TLB_MISS_STATS_X(name)
#define TLB_MISS_STATS_Y(name)
#define TLB_MISS_STATS_SAVE_INFO
#define TLB_MISS_STATS_SAVE_INFO_BOLTED
#endif

#define SET_IVOR(vector_number, vector_offset)	\
	li	r3,vector_offset@l; 		\
	ori	r3,r3,interrupt_base_book3e@l;	\
	mtspr	SPRN_IVOR##vector_number,r3;

#endif /* _ASM_POWERPC_EXCEPTION_64E_H */

=======
=======
>>>>>>> refs/remotes/origin/master
/*
 * The following macros define the code that appears as
 * the prologue to each of the exception handlers.  They
 * are split into two parts to allow a single kernel binary
 * to be used for pSeries and iSeries.
 *
 * We make as much of the exception code common between native
 * exception handlers (including pSeries LPAR) and iSeries LPAR
 * implementations as possible.
 */

#define EX_R9		0
#define EX_R10		8
#define EX_R11		16
#define EX_R12		24
#define EX_R13		32
#define EX_SRR0		40
#define EX_DAR		48
#define EX_DSISR	56
#define EX_CCR		60
#define EX_R3		64
#define EX_LR		72
#define EX_CFAR		80
<<<<<<< HEAD
=======
#define EX_PPR		88	/* SMT thread status register (priority) */
#define EX_CTR		96

#ifdef CONFIG_RELOCATABLE
#define __EXCEPTION_RELON_PROLOG_PSERIES_1(label, h)			\
	ld	r12,PACAKBASE(r13);	/* get high part of &label */	\
	mfspr	r11,SPRN_##h##SRR0;	/* save SRR0 */			\
	LOAD_HANDLER(r12,label);					\
	mtctr	r12;							\
	mfspr	r12,SPRN_##h##SRR1;	/* and SRR1 */			\
	li	r10,MSR_RI;						\
	mtmsrd 	r10,1;			/* Set RI (EE=0) */		\
	bctr;
#else
/* If not relocatable, we can jump directly -- and save messing with LR */
#define __EXCEPTION_RELON_PROLOG_PSERIES_1(label, h)			\
	mfspr	r11,SPRN_##h##SRR0;	/* save SRR0 */			\
	mfspr	r12,SPRN_##h##SRR1;	/* and SRR1 */			\
	li	r10,MSR_RI;						\
	mtmsrd 	r10,1;			/* Set RI (EE=0) */		\
	b	label;
#endif
#define EXCEPTION_RELON_PROLOG_PSERIES_1(label, h)			\
	__EXCEPTION_RELON_PROLOG_PSERIES_1(label, h)			\

/*
 * As EXCEPTION_PROLOG_PSERIES(), except we've already got relocation on
 * so no need to rfid.  Save lr in case we're CONFIG_RELOCATABLE, in which
 * case EXCEPTION_RELON_PROLOG_PSERIES_1 will be using lr.
 */
#define EXCEPTION_RELON_PROLOG_PSERIES(area, label, h, extra, vec)	\
	EXCEPTION_PROLOG_0(area);					\
	EXCEPTION_PROLOG_1(area, extra, vec);				\
	EXCEPTION_RELON_PROLOG_PSERIES_1(label, h)
>>>>>>> refs/remotes/origin/master

/*
 * We're short on space and time in the exception prolog, so we can't
 * use the normal SET_REG_IMMEDIATE macro. Normally we just need the
 * low halfword of the address, but for Kdump we need the whole low
 * word.
 */
#define LOAD_HANDLER(reg, label)					\
<<<<<<< HEAD
	addi	reg,reg,(label)-_stext;	/* virt addr of handler ... */
=======
	/* Handlers must be within 64K of kbase, which must be 64k aligned */ \
	ori	reg,reg,(label)-_stext;	/* virt addr of handler ... */
>>>>>>> refs/remotes/origin/master

/* Exception register prefixes */
#define EXC_HV	H
#define EXC_STD

<<<<<<< HEAD
#define __EXCEPTION_PROLOG_1(area, extra, vec)				\
	GET_PACA(r13);							\
	std	r9,area+EX_R9(r13);	/* save r9 - r12 */		\
	std	r10,area+EX_R10(r13);					\
	BEGIN_FTR_SECTION_NESTED(66);					\
	mfspr	r10,SPRN_CFAR;						\
	std	r10,area+EX_CFAR(r13);					\
	END_FTR_SECTION_NESTED(CPU_FTR_CFAR, CPU_FTR_CFAR, 66);		\
=======
#if defined(CONFIG_RELOCATABLE)
/*
 * If we support interrupts with relocation on AND we're a relocatable kernel,
 * we need to use CTR to get to the 2nd level handler.  So, save/restore it
 * when required.
 */
#define SAVE_CTR(reg, area)	mfctr	reg ; 	std	reg,area+EX_CTR(r13)
#define GET_CTR(reg, area) 			ld	reg,area+EX_CTR(r13)
#define RESTORE_CTR(reg, area)	ld	reg,area+EX_CTR(r13) ; mtctr reg
#else
/* ...else CTR is unused and in register. */
#define SAVE_CTR(reg, area)
#define GET_CTR(reg, area) 	mfctr	reg
#define RESTORE_CTR(reg, area)
#endif

/*
 * PPR save/restore macros used in exceptions_64s.S  
 * Used for P7 or later processors
 */
#define SAVE_PPR(area, ra, rb)						\
BEGIN_FTR_SECTION_NESTED(940)						\
	ld	ra,PACACURRENT(r13);					\
	ld	rb,area+EX_PPR(r13);	/* Read PPR from paca */	\
	std	rb,TASKTHREADPPR(ra);					\
END_FTR_SECTION_NESTED(CPU_FTR_HAS_PPR,CPU_FTR_HAS_PPR,940)

#define RESTORE_PPR_PACA(area, ra)					\
BEGIN_FTR_SECTION_NESTED(941)						\
	ld	ra,area+EX_PPR(r13);					\
	mtspr	SPRN_PPR,ra;						\
END_FTR_SECTION_NESTED(CPU_FTR_HAS_PPR,CPU_FTR_HAS_PPR,941)

/*
 * Increase the priority on systems where PPR save/restore is not
 * implemented/ supported.
 */
#define HMT_MEDIUM_PPR_DISCARD						\
BEGIN_FTR_SECTION_NESTED(942)						\
	HMT_MEDIUM;							\
END_FTR_SECTION_NESTED(CPU_FTR_HAS_PPR,0,942)  /*non P7*/		

/*
 * Get an SPR into a register if the CPU has the given feature
 */
#define OPT_GET_SPR(ra, spr, ftr)					\
BEGIN_FTR_SECTION_NESTED(943)						\
	mfspr	ra,spr;							\
END_FTR_SECTION_NESTED(ftr,ftr,943)

/*
 * Save a register to the PACA if the CPU has the given feature
 */
#define OPT_SAVE_REG_TO_PACA(offset, ra, ftr)				\
BEGIN_FTR_SECTION_NESTED(943)						\
	std	ra,offset(r13);						\
END_FTR_SECTION_NESTED(ftr,ftr,943)

#define EXCEPTION_PROLOG_0(area)					\
	GET_PACA(r13);							\
	std	r9,area+EX_R9(r13);	/* save r9 */			\
	OPT_GET_SPR(r9, SPRN_PPR, CPU_FTR_HAS_PPR);			\
	HMT_MEDIUM;							\
	std	r10,area+EX_R10(r13);	/* save r10 - r12 */		\
	OPT_GET_SPR(r10, SPRN_CFAR, CPU_FTR_CFAR)

#define __EXCEPTION_PROLOG_1(area, extra, vec)				\
	OPT_SAVE_REG_TO_PACA(area+EX_PPR, r9, CPU_FTR_HAS_PPR);		\
	OPT_SAVE_REG_TO_PACA(area+EX_CFAR, r10, CPU_FTR_CFAR);		\
	SAVE_CTR(r10, area);						\
>>>>>>> refs/remotes/origin/master
	mfcr	r9;							\
	extra(vec);							\
	std	r11,area+EX_R11(r13);					\
	std	r12,area+EX_R12(r13);					\
	GET_SCRATCH0(r10);						\
	std	r10,area+EX_R13(r13)
#define EXCEPTION_PROLOG_1(area, extra, vec)				\
	__EXCEPTION_PROLOG_1(area, extra, vec)

#define __EXCEPTION_PROLOG_PSERIES_1(label, h)				\
	ld	r12,PACAKBASE(r13);	/* get high part of &label */	\
	ld	r10,PACAKMSR(r13);	/* get MSR value for kernel */	\
	mfspr	r11,SPRN_##h##SRR0;	/* save SRR0 */			\
	LOAD_HANDLER(r12,label)						\
	mtspr	SPRN_##h##SRR0,r12;					\
	mfspr	r12,SPRN_##h##SRR1;	/* and SRR1 */			\
	mtspr	SPRN_##h##SRR1,r10;					\
	h##rfid;							\
	b	.	/* prevent speculative execution */
#define EXCEPTION_PROLOG_PSERIES_1(label, h)				\
	__EXCEPTION_PROLOG_PSERIES_1(label, h)

#define EXCEPTION_PROLOG_PSERIES(area, label, h, extra, vec)		\
<<<<<<< HEAD
=======
	EXCEPTION_PROLOG_0(area);					\
>>>>>>> refs/remotes/origin/master
	EXCEPTION_PROLOG_1(area, extra, vec);				\
	EXCEPTION_PROLOG_PSERIES_1(label, h);

#define __KVMTEST(n)							\
	lbz	r10,HSTATE_IN_GUEST(r13);			\
	cmpwi	r10,0;							\
	bne	do_kvm_##n

<<<<<<< HEAD
#define __KVM_HANDLER(area, h, n)					\
do_kvm_##n:								\
	ld	r10,area+EX_R10(r13);					\
	stw	r9,HSTATE_SCRATCH1(r13);			\
	ld	r9,area+EX_R9(r13);					\
	std	r12,HSTATE_SCRATCH0(r13);			\
=======
#ifdef CONFIG_KVM_BOOK3S_HV_POSSIBLE
/*
 * If hv is possible, interrupts come into to the hv version
 * of the kvmppc_interrupt code, which then jumps to the PR handler,
 * kvmppc_interrupt_pr, if the guest is a PR guest.
 */
#define kvmppc_interrupt kvmppc_interrupt_hv
#else
#define kvmppc_interrupt kvmppc_interrupt_pr
#endif

#define __KVM_HANDLER(area, h, n)					\
do_kvm_##n:								\
	BEGIN_FTR_SECTION_NESTED(947)					\
	ld	r10,area+EX_CFAR(r13);					\
	std	r10,HSTATE_CFAR(r13);					\
	END_FTR_SECTION_NESTED(CPU_FTR_CFAR,CPU_FTR_CFAR,947);		\
	BEGIN_FTR_SECTION_NESTED(948)					\
	ld	r10,area+EX_PPR(r13);					\
	std	r10,HSTATE_PPR(r13);					\
	END_FTR_SECTION_NESTED(CPU_FTR_HAS_PPR,CPU_FTR_HAS_PPR,948);	\
	ld	r10,area+EX_R10(r13);					\
	stw	r9,HSTATE_SCRATCH1(r13);				\
	ld	r9,area+EX_R9(r13);					\
	std	r12,HSTATE_SCRATCH0(r13);				\
>>>>>>> refs/remotes/origin/master
	li	r12,n;							\
	b	kvmppc_interrupt

#define __KVM_HANDLER_SKIP(area, h, n)					\
do_kvm_##n:								\
	cmpwi	r10,KVM_GUEST_MODE_SKIP;				\
	ld	r10,area+EX_R10(r13);					\
	beq	89f;							\
	stw	r9,HSTATE_SCRATCH1(r13);			\
<<<<<<< HEAD
=======
	BEGIN_FTR_SECTION_NESTED(948)					\
	ld	r9,area+EX_PPR(r13);					\
	std	r9,HSTATE_PPR(r13);					\
	END_FTR_SECTION_NESTED(CPU_FTR_HAS_PPR,CPU_FTR_HAS_PPR,948);	\
>>>>>>> refs/remotes/origin/master
	ld	r9,area+EX_R9(r13);					\
	std	r12,HSTATE_SCRATCH0(r13);			\
	li	r12,n;							\
	b	kvmppc_interrupt;					\
89:	mtocrf	0x80,r9;						\
	ld	r9,area+EX_R9(r13);					\
	b	kvmppc_skip_##h##interrupt

#ifdef CONFIG_KVM_BOOK3S_64_HANDLER
#define KVMTEST(n)			__KVMTEST(n)
#define KVM_HANDLER(area, h, n)		__KVM_HANDLER(area, h, n)
#define KVM_HANDLER_SKIP(area, h, n)	__KVM_HANDLER_SKIP(area, h, n)

#else
#define KVMTEST(n)
#define KVM_HANDLER(area, h, n)
#define KVM_HANDLER_SKIP(area, h, n)
#endif

<<<<<<< HEAD
#ifdef CONFIG_KVM_BOOK3S_PR
=======
#ifdef CONFIG_KVM_BOOK3S_PR_POSSIBLE
>>>>>>> refs/remotes/origin/master
#define KVMTEST_PR(n)			__KVMTEST(n)
#define KVM_HANDLER_PR(area, h, n)	__KVM_HANDLER(area, h, n)
#define KVM_HANDLER_PR_SKIP(area, h, n)	__KVM_HANDLER_SKIP(area, h, n)

#else
#define KVMTEST_PR(n)
#define KVM_HANDLER_PR(area, h, n)
#define KVM_HANDLER_PR_SKIP(area, h, n)
#endif

#define NOTEST(n)

/*
 * The common exception prolog is used for all except a few exceptions
 * such as a segment miss on a kernel address.  We have to be prepared
 * to take another exception from the point where we first touch the
 * kernel stack onwards.
 *
 * On entry r13 points to the paca, r9-r13 are saved in the paca,
 * r9 contains the saved CR, r11 and r12 contain the saved SRR0 and
 * SRR1, and relocation is on.
 */
#define EXCEPTION_PROLOG_COMMON(n, area)				   \
	andi.	r10,r12,MSR_PR;		/* See if coming from user	*/ \
	mr	r10,r1;			/* Save r1			*/ \
	subi	r1,r1,INT_FRAME_SIZE;	/* alloc frame on kernel stack	*/ \
	beq-	1f;							   \
	ld	r1,PACAKSAVE(r13);	/* kernel stack to use		*/ \
<<<<<<< HEAD
1:	cmpdi	cr1,r1,0;		/* check if r1 is in userspace	*/ \
=======
1:	cmpdi	cr1,r1,-INT_FRAME_SIZE;	/* check if r1 is in userspace	*/ \
>>>>>>> refs/remotes/origin/master
	blt+	cr1,3f;			/* abort if it is		*/ \
	li	r1,(n);			/* will be reloaded later	*/ \
	sth	r1,PACA_TRAP_SAVE(r13);					   \
	std	r3,area+EX_R3(r13);					   \
	addi	r3,r13,area;		/* r3 -> where regs are saved*/	   \
<<<<<<< HEAD
=======
	RESTORE_CTR(r1, area);						   \
>>>>>>> refs/remotes/origin/master
	b	bad_stack;						   \
3:	std	r9,_CCR(r1);		/* save CR in stackframe	*/ \
	std	r11,_NIP(r1);		/* save SRR0 in stackframe	*/ \
	std	r12,_MSR(r1);		/* save SRR1 in stackframe	*/ \
	std	r10,0(r1);		/* make stack chain pointer	*/ \
	std	r0,GPR0(r1);		/* save r0 in stackframe	*/ \
	std	r10,GPR1(r1);		/* save r1 in stackframe	*/ \
<<<<<<< HEAD
	ACCOUNT_CPU_USER_ENTRY(r9, r10);				   \
	std	r2,GPR2(r1);		/* save r2 in stackframe	*/ \
=======
	beq	4f;			/* if from kernel mode		*/ \
	ACCOUNT_CPU_USER_ENTRY(r9, r10);				   \
	SAVE_PPR(area, r9, r10);					   \
4:	std	r2,GPR2(r1);		/* save r2 in stackframe	*/ \
>>>>>>> refs/remotes/origin/master
	SAVE_4GPRS(3, r1);		/* save r3 - r6 in stackframe	*/ \
	SAVE_2GPRS(7, r1);		/* save r7, r8 in stackframe	*/ \
	ld	r9,area+EX_R9(r13);	/* move r9, r10 to stackframe	*/ \
	ld	r10,area+EX_R10(r13);					   \
	std	r9,GPR9(r1);						   \
	std	r10,GPR10(r1);						   \
	ld	r9,area+EX_R11(r13);	/* move r11 - r13 to stackframe	*/ \
	ld	r10,area+EX_R12(r13);					   \
	ld	r11,area+EX_R13(r13);					   \
	std	r9,GPR11(r1);						   \
	std	r10,GPR12(r1);						   \
	std	r11,GPR13(r1);						   \
	BEGIN_FTR_SECTION_NESTED(66);					   \
	ld	r10,area+EX_CFAR(r13);					   \
	std	r10,ORIG_GPR3(r1);					   \
	END_FTR_SECTION_NESTED(CPU_FTR_CFAR, CPU_FTR_CFAR, 66);		   \
<<<<<<< HEAD
	ld	r2,PACATOC(r13);	/* get kernel TOC into r2	*/ \
	mflr	r9;			/* save LR in stackframe	*/ \
	std	r9,_LINK(r1);						   \
	mfctr	r10;			/* save CTR in stackframe	*/ \
=======
	mflr	r9;			/* Get LR, later save to stack	*/ \
	ld	r2,PACATOC(r13);	/* get kernel TOC into r2	*/ \
	std	r9,_LINK(r1);						   \
	GET_CTR(r10, area);						   \
>>>>>>> refs/remotes/origin/master
	std	r10,_CTR(r1);						   \
	lbz	r10,PACASOFTIRQEN(r13);				   \
	mfspr	r11,SPRN_XER;		/* save XER in stackframe	*/ \
	std	r10,SOFTE(r1);						   \
	std	r11,_XER(r1);						   \
	li	r9,(n)+1;						   \
	std	r9,_TRAP(r1);		/* set trap number		*/ \
	li	r10,0;							   \
	ld	r11,exception_marker@toc(r2);				   \
	std	r10,RESULT(r1);		/* clear regs->result		*/ \
	std	r11,STACK_FRAME_OVERHEAD-16(r1); /* mark the frame	*/ \
	ACCOUNT_STOLEN_TIME

/*
 * Exception vectors.
 */
#define STD_EXCEPTION_PSERIES(loc, vec, label)		\
	. = loc;					\
	.globl label##_pSeries;				\
label##_pSeries:					\
<<<<<<< HEAD
	HMT_MEDIUM;					\
=======
	HMT_MEDIUM_PPR_DISCARD;				\
>>>>>>> refs/remotes/origin/master
	SET_SCRATCH0(r13);		/* save r13 */		\
	EXCEPTION_PROLOG_PSERIES(PACA_EXGEN, label##_common,	\
				 EXC_STD, KVMTEST_PR, vec)

<<<<<<< HEAD
=======
/* Version of above for when we have to branch out-of-line */
#define STD_EXCEPTION_PSERIES_OOL(vec, label)			\
	.globl label##_pSeries;					\
label##_pSeries:						\
	EXCEPTION_PROLOG_1(PACA_EXGEN, KVMTEST_PR, vec);	\
	EXCEPTION_PROLOG_PSERIES_1(label##_common, EXC_STD)

>>>>>>> refs/remotes/origin/master
#define STD_EXCEPTION_HV(loc, vec, label)		\
	. = loc;					\
	.globl label##_hv;				\
label##_hv:						\
<<<<<<< HEAD
	HMT_MEDIUM;					\
=======
	HMT_MEDIUM_PPR_DISCARD;				\
>>>>>>> refs/remotes/origin/master
	SET_SCRATCH0(r13);	/* save r13 */			\
	EXCEPTION_PROLOG_PSERIES(PACA_EXGEN, label##_common,	\
				 EXC_HV, KVMTEST, vec)

<<<<<<< HEAD
=======
/* Version of above for when we have to branch out-of-line */
#define STD_EXCEPTION_HV_OOL(vec, label)		\
	.globl label##_hv;				\
label##_hv:						\
	EXCEPTION_PROLOG_1(PACA_EXGEN, KVMTEST, vec);	\
	EXCEPTION_PROLOG_PSERIES_1(label##_common, EXC_HV)

#define STD_RELON_EXCEPTION_PSERIES(loc, vec, label)	\
	. = loc;					\
	.globl label##_relon_pSeries;			\
label##_relon_pSeries:					\
	HMT_MEDIUM_PPR_DISCARD;				\
	/* No guest interrupts come through here */	\
	SET_SCRATCH0(r13);		/* save r13 */	\
	EXCEPTION_RELON_PROLOG_PSERIES(PACA_EXGEN, label##_common, \
				       EXC_STD, NOTEST, vec)

#define STD_RELON_EXCEPTION_PSERIES_OOL(vec, label)		\
	.globl label##_relon_pSeries;				\
label##_relon_pSeries:						\
	EXCEPTION_PROLOG_1(PACA_EXGEN, NOTEST, vec);		\
	EXCEPTION_RELON_PROLOG_PSERIES_1(label##_common, EXC_STD)

#define STD_RELON_EXCEPTION_HV(loc, vec, label)		\
	. = loc;					\
	.globl label##_relon_hv;			\
label##_relon_hv:					\
	HMT_MEDIUM_PPR_DISCARD;				\
	/* No guest interrupts come through here */	\
	SET_SCRATCH0(r13);	/* save r13 */		\
	EXCEPTION_RELON_PROLOG_PSERIES(PACA_EXGEN, label##_common, \
				       EXC_HV, NOTEST, vec)

#define STD_RELON_EXCEPTION_HV_OOL(vec, label)			\
	.globl label##_relon_hv;				\
label##_relon_hv:						\
	EXCEPTION_PROLOG_1(PACA_EXGEN, NOTEST, vec);		\
	EXCEPTION_RELON_PROLOG_PSERIES_1(label##_common, EXC_HV)

>>>>>>> refs/remotes/origin/master
/* This associate vector numbers with bits in paca->irq_happened */
#define SOFTEN_VALUE_0x500	PACA_IRQ_EE
#define SOFTEN_VALUE_0x502	PACA_IRQ_EE
#define SOFTEN_VALUE_0x900	PACA_IRQ_DEC
#define SOFTEN_VALUE_0x982	PACA_IRQ_DEC
<<<<<<< HEAD
=======
#define SOFTEN_VALUE_0xa00	PACA_IRQ_DBELL
#define SOFTEN_VALUE_0xe80	PACA_IRQ_DBELL
#define SOFTEN_VALUE_0xe82	PACA_IRQ_DBELL
>>>>>>> refs/remotes/origin/master

#define __SOFTEN_TEST(h, vec)						\
	lbz	r10,PACASOFTIRQEN(r13);					\
	cmpwi	r10,0;							\
	li	r10,SOFTEN_VALUE_##vec;					\
	beq	masked_##h##interrupt
#define _SOFTEN_TEST(h, vec)	__SOFTEN_TEST(h, vec)

#define SOFTEN_TEST_PR(vec)						\
	KVMTEST_PR(vec);						\
	_SOFTEN_TEST(EXC_STD, vec)

#define SOFTEN_TEST_HV(vec)						\
	KVMTEST(vec);							\
	_SOFTEN_TEST(EXC_HV, vec)

#define SOFTEN_TEST_HV_201(vec)						\
	KVMTEST(vec);							\
	_SOFTEN_TEST(EXC_STD, vec)

<<<<<<< HEAD
#define __MASKABLE_EXCEPTION_PSERIES(vec, label, h, extra)		\
	HMT_MEDIUM;							\
	SET_SCRATCH0(r13);    /* save r13 */				\
	__EXCEPTION_PROLOG_1(PACA_EXGEN, extra, vec);		\
	EXCEPTION_PROLOG_PSERIES_1(label##_common, h);
=======
#define SOFTEN_NOTEST_PR(vec)		_SOFTEN_TEST(EXC_STD, vec)
#define SOFTEN_NOTEST_HV(vec)		_SOFTEN_TEST(EXC_HV, vec)

#define __MASKABLE_EXCEPTION_PSERIES(vec, label, h, extra)		\
	SET_SCRATCH0(r13);    /* save r13 */				\
	EXCEPTION_PROLOG_0(PACA_EXGEN);					\
	__EXCEPTION_PROLOG_1(PACA_EXGEN, extra, vec);			\
	EXCEPTION_PROLOG_PSERIES_1(label##_common, h);

>>>>>>> refs/remotes/origin/master
#define _MASKABLE_EXCEPTION_PSERIES(vec, label, h, extra)		\
	__MASKABLE_EXCEPTION_PSERIES(vec, label, h, extra)

#define MASKABLE_EXCEPTION_PSERIES(loc, vec, label)			\
	. = loc;							\
	.globl label##_pSeries;						\
label##_pSeries:							\
<<<<<<< HEAD
=======
	HMT_MEDIUM_PPR_DISCARD;						\
>>>>>>> refs/remotes/origin/master
	_MASKABLE_EXCEPTION_PSERIES(vec, label,				\
				    EXC_STD, SOFTEN_TEST_PR)

#define MASKABLE_EXCEPTION_HV(loc, vec, label)				\
	. = loc;							\
	.globl label##_hv;						\
label##_hv:								\
	_MASKABLE_EXCEPTION_PSERIES(vec, label,				\
				    EXC_HV, SOFTEN_TEST_HV)

<<<<<<< HEAD
=======
#define MASKABLE_EXCEPTION_HV_OOL(vec, label)				\
	.globl label##_hv;						\
label##_hv:								\
	EXCEPTION_PROLOG_1(PACA_EXGEN, SOFTEN_TEST_HV, vec);		\
	EXCEPTION_PROLOG_PSERIES_1(label##_common, EXC_HV);

#define __MASKABLE_RELON_EXCEPTION_PSERIES(vec, label, h, extra)	\
	HMT_MEDIUM_PPR_DISCARD;						\
	SET_SCRATCH0(r13);    /* save r13 */				\
	EXCEPTION_PROLOG_0(PACA_EXGEN);					\
	__EXCEPTION_PROLOG_1(PACA_EXGEN, extra, vec);		\
	EXCEPTION_RELON_PROLOG_PSERIES_1(label##_common, h);
#define _MASKABLE_RELON_EXCEPTION_PSERIES(vec, label, h, extra)	\
	__MASKABLE_RELON_EXCEPTION_PSERIES(vec, label, h, extra)

#define MASKABLE_RELON_EXCEPTION_PSERIES(loc, vec, label)		\
	. = loc;							\
	.globl label##_relon_pSeries;					\
label##_relon_pSeries:							\
	_MASKABLE_RELON_EXCEPTION_PSERIES(vec, label,			\
					  EXC_STD, SOFTEN_NOTEST_PR)

#define MASKABLE_RELON_EXCEPTION_HV(loc, vec, label)			\
	. = loc;							\
	.globl label##_relon_hv;					\
label##_relon_hv:							\
	_MASKABLE_RELON_EXCEPTION_PSERIES(vec, label,			\
					  EXC_HV, SOFTEN_NOTEST_HV)

#define MASKABLE_RELON_EXCEPTION_HV_OOL(vec, label)			\
	.globl label##_relon_hv;					\
label##_relon_hv:							\
	EXCEPTION_PROLOG_1(PACA_EXGEN, SOFTEN_NOTEST_HV, vec);		\
	EXCEPTION_PROLOG_PSERIES_1(label##_common, EXC_HV);

>>>>>>> refs/remotes/origin/master
/*
 * Our exception common code can be passed various "additions"
 * to specify the behaviour of interrupts, whether to kick the
 * runlatch, etc...
 */

/* Exception addition: Hard disable interrupts */
<<<<<<< HEAD
#define DISABLE_INTS	SOFT_DISABLE_INTS(r10,r11)
=======
#define DISABLE_INTS	RECONCILE_IRQ_STATE(r10,r11)
>>>>>>> refs/remotes/origin/master

#define ADD_NVGPRS				\
	bl	.save_nvgprs

#define RUNLATCH_ON				\
BEGIN_FTR_SECTION				\
<<<<<<< HEAD
	clrrdi	r3,r1,THREAD_SHIFT;		\
=======
	CURRENT_THREAD_INFO(r3, r1);		\
>>>>>>> refs/remotes/origin/master
	ld	r4,TI_LOCAL_FLAGS(r3);		\
	andi.	r0,r4,_TLF_RUNLATCH;		\
	beql	ppc64_runlatch_on_trampoline;	\
END_FTR_SECTION_IFSET(CPU_FTR_CTRL)

#define EXCEPTION_COMMON(trap, label, hdlr, ret, additions)	\
	.align	7;						\
	.globl label##_common;					\
label##_common:							\
	EXCEPTION_PROLOG_COMMON(trap, PACA_EXGEN);		\
	additions;						\
	addi	r3,r1,STACK_FRAME_OVERHEAD;			\
	bl	hdlr;						\
	b	ret

#define STD_EXCEPTION_COMMON(trap, label, hdlr)			\
	EXCEPTION_COMMON(trap, label, hdlr, ret_from_except,	\
			 ADD_NVGPRS;DISABLE_INTS)

/*
 * Like STD_EXCEPTION_COMMON, but for exceptions that can occur
 * in the idle task and therefore need the special idle handling
 * (finish nap and runlatch)
 */
#define STD_EXCEPTION_COMMON_ASYNC(trap, label, hdlr)		  \
	EXCEPTION_COMMON(trap, label, hdlr, ret_from_except_lite, \
			 FINISH_NAP;DISABLE_INTS;RUNLATCH_ON)

/*
 * When the idle code in power4_idle puts the CPU into NAP mode,
 * it has to do so in a loop, and relies on the external interrupt
 * and decrementer interrupt entry code to get it out of the loop.
 * It sets the _TLF_NAPPING bit in current_thread_info()->local_flags
 * to signal that it is in the loop and needs help to get out.
 */
#ifdef CONFIG_PPC_970_NAP
#define FINISH_NAP				\
BEGIN_FTR_SECTION				\
<<<<<<< HEAD
	clrrdi	r11,r1,THREAD_SHIFT;		\
=======
	CURRENT_THREAD_INFO(r11, r1);		\
>>>>>>> refs/remotes/origin/master
	ld	r9,TI_LOCAL_FLAGS(r11);		\
	andi.	r10,r9,_TLF_NAPPING;		\
	bnel	power4_fixup_nap;		\
END_FTR_SECTION_IFSET(CPU_FTR_CAN_NAP)
#else
#define FINISH_NAP
#endif

#endif	/* _ASM_POWERPC_EXCEPTION_H */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
