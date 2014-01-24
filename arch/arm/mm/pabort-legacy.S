#include <linux/linkage.h>
#include <asm/assembler.h>

/*
 * Function: legacy_pabort
 *
<<<<<<< HEAD
 * Params  : r0 = address of aborted instruction
 *
 * Returns : r0 = address of abort
 *	   : r1 = Simulated IFSR with section translation fault status
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
ENTRY(legacy_pabort)
<<<<<<< HEAD
	mov	r1, #5
	mov	pc, lr
=======
	mov	r0, r4
	mov	r1, #5
	b	do_PrefetchAbort
>>>>>>> refs/remotes/origin/cm-10.0
ENDPROC(legacy_pabort)
