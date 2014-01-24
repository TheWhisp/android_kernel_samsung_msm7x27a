#include <linux/linkage.h>
#include <asm/assembler.h>

/*
 * Function: v6_pabort
 *
<<<<<<< HEAD
 * Params  : r0 = address of aborted instruction
 *
 * Returns : r0 = address of abort
 *	   : r1 = IFSR
=======
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
ENTRY(v6_pabort)
<<<<<<< HEAD
	mrc	p15, 0, r1, c5, c0, 1		@ get IFSR
	mov	pc, lr
=======
	mov	r0, r4
	mrc	p15, 0, r1, c5, c0, 1		@ get IFSR
	b	do_PrefetchAbort
>>>>>>> refs/remotes/origin/cm-10.0
ENDPROC(v6_pabort)
