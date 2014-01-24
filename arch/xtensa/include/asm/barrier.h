/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
<<<<<<< HEAD
 * Copyright (C) 2001 - 2005 Tensilica Inc.
=======
 * Copyright (C) 2001 - 2012 Tensilica Inc.
>>>>>>> refs/remotes/origin/master
 */

#ifndef _XTENSA_SYSTEM_H
#define _XTENSA_SYSTEM_H

<<<<<<< HEAD
#define smp_read_barrier_depends() do { } while(0)
#define read_barrier_depends() do { } while(0)

#define mb()  barrier()
#define rmb() mb()
=======
#define mb()  ({ __asm__ __volatile__("memw" : : : "memory"); })
#define rmb() barrier()
>>>>>>> refs/remotes/origin/master
#define wmb() mb()

#ifdef CONFIG_SMP
#error smp_* not defined
<<<<<<< HEAD
#else
#define smp_mb()	barrier()
#define smp_rmb()	barrier()
#define smp_wmb()	barrier()
#endif

#define set_mb(var, value)	do { var = value; mb(); } while (0)
=======
#endif

#include <asm-generic/barrier.h>
>>>>>>> refs/remotes/origin/master

#endif /* _XTENSA_SYSTEM_H */
