<<<<<<< HEAD
=======
#ifndef _ASM_X86_ALTERNATIVE_ASM_H
#define _ASM_X86_ALTERNATIVE_ASM_H

>>>>>>> refs/remotes/origin/master
#ifdef __ASSEMBLY__

#include <asm/asm.h>

#ifdef CONFIG_SMP
	.macro LOCK_PREFIX
<<<<<<< HEAD
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
=======
672:	lock
	.pushsection .smp_locks,"a"
	.balign 4
	.long 672b - .
	.popsection
>>>>>>> refs/remotes/origin/master
	.endm
#else
	.macro LOCK_PREFIX
	.endm
#endif

.macro altinstruction_entry orig alt feature orig_len alt_len
<<<<<<< HEAD
<<<<<<< HEAD
	.align 8
	.quad \orig
	.quad \alt
=======
	.long \orig - .
	.long \alt - .
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.long \orig - .
	.long \alt - .
>>>>>>> refs/remotes/origin/master
	.word \feature
	.byte \orig_len
	.byte \alt_len
.endm

#endif  /*  __ASSEMBLY__  */
<<<<<<< HEAD
=======

#endif /* _ASM_X86_ALTERNATIVE_ASM_H */
>>>>>>> refs/remotes/origin/master
