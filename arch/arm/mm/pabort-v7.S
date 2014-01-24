#include <linux/linkage.h>
#include <asm/assembler.h>

/*
<<<<<<< HEAD
 * Function: v6_pabort
 *
 * Params  : r0 = address of aborted instruction
 *
 * Returns : r0 = address of abort
 *	   : r1 = IFSR
=======
 * Function: v7_pabort
 *
 * Params  : r2 = pt_regs
 *	   : r4 = address of aborted instruction
 *	   : r5 = psr for parent context
 *
 * Returns : r4 - r11, r13 preserved
>>>>>>> refs/remotes/origin/cm-10.0
 *
 * Purpose : obtain information about current prefetch abort.
 */

	.align	5
ENTRY(v7_pabort)
	mrc	p15, 0, r0, c6, c0, 2		@ get IFAR
	mrc	p15, 0, r1, c5, c0, 1		@ get IFSR
<<<<<<< HEAD
	mov	pc, lr
=======
	b	do_PrefetchAbort
>>>>>>> refs/remotes/origin/cm-10.0
ENDPROC(v7_pabort)
