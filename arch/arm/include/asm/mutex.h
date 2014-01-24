/*
 * arch/arm/include/asm/mutex.h
 *
 * ARM optimized mutex locking primitives
 *
 * Please look into asm-generic/mutex-xchg.h for a formal definition.
 */
#ifndef _ASM_MUTEX_H
#define _ASM_MUTEX_H
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
/*
 * On pre-ARMv6 hardware this results in a swp-based implementation,
 * which is the most efficient. For ARMv6+, we emit a pair of exclusive
 * accesses instead.
 */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
#include <asm-generic/mutex-xchg.h>
#endif
=======
/*
 * On pre-ARMv6 hardware this results in a swp-based implementation,
 * which is the most efficient. For ARMv6+, we have exclusive memory
 * accessors and use atomic_dec to avoid the extra xchg operations
 * on the locking slowpaths.
 */
#if __LINUX_ARM_ARCH__ < 6
#include <asm-generic/mutex-xchg.h>
#else
#include <asm-generic/mutex-dec.h>
=======
#include <asm-generic/mutex-xchg.h>
>>>>>>> refs/remotes/origin/cm-11.0
#endif
#endif	/* _ASM_MUTEX_H */
>>>>>>> refs/remotes/origin/master
