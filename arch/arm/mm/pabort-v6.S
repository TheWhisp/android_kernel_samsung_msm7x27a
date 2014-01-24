#include <linux/linkage.h>
#include <asm/assembler.h>

/*
 * Function: v6_pabort
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * Params  : r0 = address of aborted instruction
 *
 * Returns : r0 = address of abort
 *	   : r1 = IFSR
=======
=======
>>>>>>> refs/remotes/origin/master
 * Params  : r2 = pt_regs
 *	   : r4 = address of aborted instruction
 *	   : r5 = psr for parent context
 *
 * Returns : r4 - r11, r13 preserved
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 *
 * Purpose : obtain information about current prefetch abort.
 */

	.align	5
ENTRY(v6_pabort)
<<<<<<< HEAD
<<<<<<< HEAD
	mrc	p15, 0, r1, c5, c0, 1		@ get IFSR
	mov	pc, lr
=======
	mov	r0, r4
	mrc	p15, 0, r1, c5, c0, 1		@ get IFSR
	b	do_PrefetchAbort
>>>>>>> refs/remotes/origin/cm-10.0
=======
	mov	r0, r4
	mrc	p15, 0, r1, c5, c0, 1		@ get IFSR
	b	do_PrefetchAbort
>>>>>>> refs/remotes/origin/master
ENDPROC(v6_pabort)
