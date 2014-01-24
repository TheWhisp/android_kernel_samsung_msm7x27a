/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1994, 1995, 1996, 1999 by Ralf Baechle
 * Copyright (C) 2008 Wind River Systems,
 *   written by Ralf Baechle
 * Copyright (C) 1999 Silicon Graphics, Inc.
 */
#ifndef _ASM_TYPES_H
#define _ASM_TYPES_H

<<<<<<< HEAD
/*
 * We don't use int-l64.h for the kernel anymore but still use it for
 * userspace to avoid code changes.
 */
<<<<<<< HEAD
#if (_MIPS_SZLONG == 64) && !defined(__KERNEL__)
# include <asm-generic/int-l64.h>
#else
# include <asm-generic/int-ll64.h>
#endif

#ifndef __ASSEMBLY__

typedef unsigned short umode_t;

#endif /* __ASSEMBLY__ */

=======
#ifdef __KERNEL__
# include <asm-generic/int-ll64.h>
#else
# if _MIPS_SZLONG == 64
#  include <asm-generic/int-l64.h>
# else
#  include <asm-generic/int-ll64.h>
# endif
#endif

>>>>>>> refs/remotes/origin/cm-10.0
/*
 * These aren't exported outside the kernel to avoid name space clashes
 */
#ifdef __KERNEL__
=======
# include <asm-generic/int-ll64.h>
#include <uapi/asm/types.h>

/*
 * These aren't exported outside the kernel to avoid name space clashes
 */
>>>>>>> refs/remotes/origin/master
#ifndef __ASSEMBLY__

/*
 * Don't use phys_t.  You've been warned.
 */
#ifdef CONFIG_64BIT_PHYS_ADDR
typedef unsigned long long phys_t;
#else
typedef unsigned long phys_t;
#endif

#endif /* __ASSEMBLY__ */

<<<<<<< HEAD
#endif /* __KERNEL__ */

=======
>>>>>>> refs/remotes/origin/master
#endif /* _ASM_TYPES_H */
