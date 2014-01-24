#ifndef __ASM_SH_TYPES_H
#define __ASM_SH_TYPES_H

<<<<<<< HEAD
#include <asm-generic/types.h>
=======
#include <uapi/asm/types.h>
>>>>>>> refs/remotes/origin/master

/*
 * These aren't exported outside the kernel to avoid name space clashes
 */
<<<<<<< HEAD
#ifdef __KERNEL__
=======
>>>>>>> refs/remotes/origin/master
#ifndef __ASSEMBLY__

#ifdef CONFIG_SUPERH32
typedef u16 insn_size_t;
typedef u32 reg_size_t;
#else
typedef u32 insn_size_t;
typedef u64 reg_size_t;
#endif

#endif /* __ASSEMBLY__ */
<<<<<<< HEAD
#endif /* __KERNEL__ */

=======
>>>>>>> refs/remotes/origin/master
#endif /* __ASM_SH_TYPES_H */
