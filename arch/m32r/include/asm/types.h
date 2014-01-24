#ifndef _ASM_M32R_TYPES_H
#define _ASM_M32R_TYPES_H

<<<<<<< HEAD
#include <asm-generic/int-ll64.h>

<<<<<<< HEAD
#ifndef __ASSEMBLY__

typedef unsigned short umode_t;

#endif /* __ASSEMBLY__ */

=======
>>>>>>> refs/remotes/origin/cm-10.0
/*
 * These aren't exported outside the kernel to avoid name space clashes
 */
#ifdef __KERNEL__

#define BITS_PER_LONG 32

#endif /* __KERNEL__ */

=======
#include <uapi/asm/types.h>

/*
 * These aren't exported outside the kernel to avoid name space clashes
 */

#define BITS_PER_LONG 32

>>>>>>> refs/remotes/origin/master
#endif /* _ASM_M32R_TYPES_H */
