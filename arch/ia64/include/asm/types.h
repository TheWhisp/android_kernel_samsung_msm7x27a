<<<<<<< HEAD
#ifndef _ASM_IA64_TYPES_H
#define _ASM_IA64_TYPES_H

=======
>>>>>>> refs/remotes/origin/master
/*
 * This file is never included by application software unless explicitly
 * requested (e.g., via linux/types.h) in which case the application is
 * Linux specific so (user-) name space pollution is not a major issue.
 * However, for interoperability, libraries still need to be careful to
 * avoid naming clashes.
 *
 * Based on <asm-alpha/types.h>.
 *
 * Modified 1998-2000, 2002
 *	David Mosberger-Tang <davidm@hpl.hp.com>, Hewlett-Packard Co
 */
<<<<<<< HEAD

#ifdef __KERNEL__
#include <asm-generic/int-ll64.h>
#else
#include <asm-generic/int-l64.h>
#endif

#ifdef __ASSEMBLY__
# define __IA64_UL(x)		(x)
# define __IA64_UL_CONST(x)	x

#else
# define __IA64_UL(x)		((unsigned long)(x))
# define __IA64_UL_CONST(x)	x##UL

<<<<<<< HEAD
typedef unsigned int umode_t;

=======
>>>>>>> refs/remotes/origin/cm-10.0
/*
 * These aren't exported outside the kernel to avoid name space clashes
 */
# ifdef __KERNEL__
=======
#ifndef _ASM_IA64_TYPES_H
#define _ASM_IA64_TYPES_H

#include <asm-generic/int-ll64.h>
#include <uapi/asm/types.h>

#ifdef __ASSEMBLY__
#else
/*
 * These aren't exported outside the kernel to avoid name space clashes
 */
>>>>>>> refs/remotes/origin/master

struct fnptr {
	unsigned long ip;
	unsigned long gp;
};

<<<<<<< HEAD
# endif /* __KERNEL__ */
#endif /* !__ASSEMBLY__ */

=======
#endif /* !__ASSEMBLY__ */
>>>>>>> refs/remotes/origin/master
#endif /* _ASM_IA64_TYPES_H */
