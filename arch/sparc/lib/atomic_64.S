/* atomic.S: These things are too big to do inline.
 *
<<<<<<< HEAD
 * Copyright (C) 1999, 2007 David S. Miller (davem@davemloft.net)
 */

=======
 * Copyright (C) 1999, 2007 2012 David S. Miller (davem@davemloft.net)
 */

#include <linux/linkage.h>
>>>>>>> refs/remotes/origin/master
#include <asm/asi.h>
#include <asm/backoff.h>

	.text

	/* Two versions of the atomic routines, one that
	 * does not return a value and does not perform
	 * memory barriers, and a second which returns
	 * a value and does the barriers.
	 */
<<<<<<< HEAD
	.globl	atomic_add
	.type	atomic_add,#function
atomic_add: /* %o0 = increment, %o1 = atomic_ptr */
=======
ENTRY(atomic_add) /* %o0 = increment, %o1 = atomic_ptr */
>>>>>>> refs/remotes/origin/master
	BACKOFF_SETUP(%o2)
1:	lduw	[%o1], %g1
	add	%g1, %o0, %g7
	cas	[%o1], %g1, %g7
	cmp	%g1, %g7
	bne,pn	%icc, BACKOFF_LABEL(2f, 1b)
	 nop
	retl
	 nop
2:	BACKOFF_SPIN(%o2, %o3, 1b)
<<<<<<< HEAD
	.size	atomic_add, .-atomic_add

	.globl	atomic_sub
	.type	atomic_sub,#function
atomic_sub: /* %o0 = decrement, %o1 = atomic_ptr */
=======
ENDPROC(atomic_add)

ENTRY(atomic_sub) /* %o0 = decrement, %o1 = atomic_ptr */
>>>>>>> refs/remotes/origin/master
	BACKOFF_SETUP(%o2)
1:	lduw	[%o1], %g1
	sub	%g1, %o0, %g7
	cas	[%o1], %g1, %g7
	cmp	%g1, %g7
	bne,pn	%icc, BACKOFF_LABEL(2f, 1b)
	 nop
	retl
	 nop
2:	BACKOFF_SPIN(%o2, %o3, 1b)
<<<<<<< HEAD
	.size	atomic_sub, .-atomic_sub

	.globl	atomic_add_ret
	.type	atomic_add_ret,#function
atomic_add_ret: /* %o0 = increment, %o1 = atomic_ptr */
=======
ENDPROC(atomic_sub)

ENTRY(atomic_add_ret) /* %o0 = increment, %o1 = atomic_ptr */
>>>>>>> refs/remotes/origin/master
	BACKOFF_SETUP(%o2)
1:	lduw	[%o1], %g1
	add	%g1, %o0, %g7
	cas	[%o1], %g1, %g7
	cmp	%g1, %g7
	bne,pn	%icc, BACKOFF_LABEL(2f, 1b)
	 add	%g1, %o0, %g1
	retl
	 sra	%g1, 0, %o0
2:	BACKOFF_SPIN(%o2, %o3, 1b)
<<<<<<< HEAD
	.size	atomic_add_ret, .-atomic_add_ret

	.globl	atomic_sub_ret
	.type	atomic_sub_ret,#function
atomic_sub_ret: /* %o0 = decrement, %o1 = atomic_ptr */
=======
ENDPROC(atomic_add_ret)

ENTRY(atomic_sub_ret) /* %o0 = decrement, %o1 = atomic_ptr */
>>>>>>> refs/remotes/origin/master
	BACKOFF_SETUP(%o2)
1:	lduw	[%o1], %g1
	sub	%g1, %o0, %g7
	cas	[%o1], %g1, %g7
	cmp	%g1, %g7
	bne,pn	%icc, BACKOFF_LABEL(2f, 1b)
	 sub	%g1, %o0, %g1
	retl
	 sra	%g1, 0, %o0
2:	BACKOFF_SPIN(%o2, %o3, 1b)
<<<<<<< HEAD
	.size	atomic_sub_ret, .-atomic_sub_ret

	.globl	atomic64_add
	.type	atomic64_add,#function
atomic64_add: /* %o0 = increment, %o1 = atomic_ptr */
=======
ENDPROC(atomic_sub_ret)

ENTRY(atomic64_add) /* %o0 = increment, %o1 = atomic_ptr */
>>>>>>> refs/remotes/origin/master
	BACKOFF_SETUP(%o2)
1:	ldx	[%o1], %g1
	add	%g1, %o0, %g7
	casx	[%o1], %g1, %g7
	cmp	%g1, %g7
	bne,pn	%xcc, BACKOFF_LABEL(2f, 1b)
	 nop
	retl
	 nop
2:	BACKOFF_SPIN(%o2, %o3, 1b)
<<<<<<< HEAD
	.size	atomic64_add, .-atomic64_add

	.globl	atomic64_sub
	.type	atomic64_sub,#function
atomic64_sub: /* %o0 = decrement, %o1 = atomic_ptr */
=======
ENDPROC(atomic64_add)

ENTRY(atomic64_sub) /* %o0 = decrement, %o1 = atomic_ptr */
>>>>>>> refs/remotes/origin/master
	BACKOFF_SETUP(%o2)
1:	ldx	[%o1], %g1
	sub	%g1, %o0, %g7
	casx	[%o1], %g1, %g7
	cmp	%g1, %g7
	bne,pn	%xcc, BACKOFF_LABEL(2f, 1b)
	 nop
	retl
	 nop
2:	BACKOFF_SPIN(%o2, %o3, 1b)
<<<<<<< HEAD
	.size	atomic64_sub, .-atomic64_sub

	.globl	atomic64_add_ret
	.type	atomic64_add_ret,#function
atomic64_add_ret: /* %o0 = increment, %o1 = atomic_ptr */
=======
ENDPROC(atomic64_sub)

ENTRY(atomic64_add_ret) /* %o0 = increment, %o1 = atomic_ptr */
>>>>>>> refs/remotes/origin/master
	BACKOFF_SETUP(%o2)
1:	ldx	[%o1], %g1
	add	%g1, %o0, %g7
	casx	[%o1], %g1, %g7
	cmp	%g1, %g7
	bne,pn	%xcc, BACKOFF_LABEL(2f, 1b)
	 nop
	retl
	 add	%g1, %o0, %o0
2:	BACKOFF_SPIN(%o2, %o3, 1b)
<<<<<<< HEAD
	.size	atomic64_add_ret, .-atomic64_add_ret

	.globl	atomic64_sub_ret
	.type	atomic64_sub_ret,#function
atomic64_sub_ret: /* %o0 = decrement, %o1 = atomic_ptr */
=======
ENDPROC(atomic64_add_ret)

ENTRY(atomic64_sub_ret) /* %o0 = decrement, %o1 = atomic_ptr */
>>>>>>> refs/remotes/origin/master
	BACKOFF_SETUP(%o2)
1:	ldx	[%o1], %g1
	sub	%g1, %o0, %g7
	casx	[%o1], %g1, %g7
	cmp	%g1, %g7
	bne,pn	%xcc, BACKOFF_LABEL(2f, 1b)
	 nop
	retl
	 sub	%g1, %o0, %o0
2:	BACKOFF_SPIN(%o2, %o3, 1b)
<<<<<<< HEAD
	.size	atomic64_sub_ret, .-atomic64_sub_ret
=======
ENDPROC(atomic64_sub_ret)

ENTRY(atomic64_dec_if_positive) /* %o0 = atomic_ptr */
	BACKOFF_SETUP(%o2)
1:	ldx	[%o0], %g1
	brlez,pn %g1, 3f
	 sub	%g1, 1, %g7
	casx	[%o0], %g1, %g7
	cmp	%g1, %g7
	bne,pn	%xcc, BACKOFF_LABEL(2f, 1b)
	 nop
3:	retl
	 sub	%g1, 1, %o0
2:	BACKOFF_SPIN(%o2, %o3, 1b)
ENDPROC(atomic64_dec_if_positive)
>>>>>>> refs/remotes/origin/master
