#ifdef __ASSEMBLY__

#include <asm/asm.h>

#ifdef CONFIG_SMP
	.macro LOCK_PREFIX
<<<<<<< HEAD
1:	lock
	.section .smp_locks,"a"
	.balign 4
	.long 1b - .
=======
672:	lock
	.section .smp_locks,"a"
	.balign 4
	.long 672b - .
>>>>>>> refs/remotes/origin/cm-10.0
	.previous
	.endm
#else
	.macro LOCK_PREFIX
	.endm
#endif

.macro altinstruction_entry orig alt feature orig_len alt_len
<<<<<<< HEAD
	.align 8
	.quad \orig
	.quad \alt
=======
	.long \orig - .
	.long \alt - .
>>>>>>> refs/remotes/origin/cm-10.0
	.word \feature
	.byte \orig_len
	.byte \alt_len
.endm

#endif  /*  __ASSEMBLY__  */
