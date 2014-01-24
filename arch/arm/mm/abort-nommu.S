#include <linux/linkage.h>
#include <asm/assembler.h>
/*
 * Function: nommu_early_abort
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * Params  : r2 = address of aborted instruction
 *         : r3 = saved SPSR
 *
 * Returns : r0 = 0 (abort address)
 *	   : r1 = 0 (FSR)
=======
=======
>>>>>>> refs/remotes/origin/master
 * Params  : r2 = pt_regs
 *	   : r4 = aborted context pc
 *	   : r5 = aborted context psr
 *
 * Returns : r4 - r11, r13 preserved
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 *
 * Note: There is no FSR/FAR on !CPU_CP15_MMU cores.
 *       Just fill zero into the registers.
 */
	.align	5
ENTRY(nommu_early_abort)
	mov	r0, #0				@ clear r0, r1 (no FSR/FAR)
	mov	r1, #0
<<<<<<< HEAD
<<<<<<< HEAD
	mov	pc, lr
=======
	b	do_DataAbort
>>>>>>> refs/remotes/origin/cm-10.0
=======
	b	do_DataAbort
>>>>>>> refs/remotes/origin/master
ENDPROC(nommu_early_abort)
