#include <linux/linkage.h>
#include <asm/assembler.h>
/*
 * Function: v4_early_abort
 *
<<<<<<< HEAD
 * Params  : r2 = address of aborted instruction
 *         : r3 = saved SPSR
 *
 * Returns : r0 = address of abort
 *	   : r1 = FSR, bit 11 = write
 *	   : r2-r8 = corrupted
 *	   : r9 = preserved
 *	   : sp = pointer to registers
=======
 * Params  : r2 = pt_regs
 *	   : r4 = aborted context pc
 *	   : r5 = aborted context psr
 *
 * Returns : r4 - r11, r13 preserved
>>>>>>> refs/remotes/origin/cm-10.0
 *
 * Purpose : obtain information about current aborted instruction.
 * Note: we read user space.  This means we might cause a data
 * abort here if the I-TLB and D-TLB aren't seeing the same
 * picture.  Unfortunately, this does happen.  We live with it.
 */
	.align	5
ENTRY(v4_early_abort)
	mrc	p15, 0, r1, c5, c0, 0		@ get FSR
	mrc	p15, 0, r0, c6, c0, 0		@ get FAR
<<<<<<< HEAD
	ldr	r3, [r2]			@ read aborted ARM instruction
	bic	r1, r1, #1 << 11 | 1 << 10	@ clear bits 11 and 10 of FSR
	tst	r3, #1 << 20			@ L = 1 -> write?
	orreq	r1, r1, #1 << 11		@ yes.
	mov	pc, lr


=======
	ldr	r3, [r4]			@ read aborted ARM instruction
	bic	r1, r1, #1 << 11 | 1 << 10	@ clear bits 11 and 10 of FSR
	tst	r3, #1 << 20			@ L = 1 -> write?
	orreq	r1, r1, #1 << 11		@ yes.
	b	do_DataAbort
>>>>>>> refs/remotes/origin/cm-10.0
