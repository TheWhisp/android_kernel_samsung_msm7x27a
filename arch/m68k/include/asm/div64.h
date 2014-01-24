#ifndef _M68K_DIV64_H
#define _M68K_DIV64_H

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_MMU
=======
#ifdef CONFIG_CPU_HAS_NO_MULDIV64
#include <asm-generic/div64.h>
#else
>>>>>>> refs/remotes/origin/cm-10.0
=======
#ifdef CONFIG_CPU_HAS_NO_MULDIV64
#include <asm-generic/div64.h>
#else
>>>>>>> refs/remotes/origin/master

#include <linux/types.h>

/* n = n / base; return rem; */

#define do_div(n, base) ({					\
	union {							\
		unsigned long n32[2];				\
		unsigned long long n64;				\
	} __n;							\
	unsigned long __rem, __upper;				\
	unsigned long __base = (base);				\
								\
	__n.n64 = (n);						\
	if ((__upper = __n.n32[0])) {				\
		asm ("divul.l %2,%1:%0"				\
		     : "=d" (__n.n32[0]), "=d" (__upper)	\
		     : "d" (__base), "0" (__n.n32[0]));		\
	}							\
	asm ("divu.l %2,%1:%0"					\
	     : "=d" (__n.n32[1]), "=d" (__rem)			\
	     : "d" (__base), "1" (__upper), "0" (__n.n32[1]));	\
	(n) = __n.n64;						\
	__rem;							\
})

<<<<<<< HEAD
<<<<<<< HEAD
#else
#include <asm-generic/div64.h>
#endif /* CONFIG_MMU */
=======
#endif /* CONFIG_CPU_HAS_NO_MULDIV64 */
>>>>>>> refs/remotes/origin/cm-10.0
=======
#endif /* CONFIG_CPU_HAS_NO_MULDIV64 */
>>>>>>> refs/remotes/origin/master

#endif /* _M68K_DIV64_H */
