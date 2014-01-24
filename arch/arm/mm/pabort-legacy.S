#include <linux/linkage.h>
#include <asm/assembler.h>

/*
 * Function: legacy_pabort
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * Params  : r0 = address of aborted instruction
 *
 * Returns : r0 = address of abort
 *	   : r1 = Simulated IFSR with section translation fault status
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
ENTRY(legacy_pabort)
<<<<<<< HEAD
<<<<<<< HEAD
	mov	r1, #5
	mov	pc, lr
=======
	mov	r0, r4
	mov	r1, #5
	b	do_PrefetchAbort
>>>>>>> refs/remotes/origin/cm-10.0
=======
	mov	r0, r4
	mov	r1, #5
	b	do_PrefetchAbort
>>>>>>> refs/remotes/origin/master
ENDPROC(legacy_pabort)
