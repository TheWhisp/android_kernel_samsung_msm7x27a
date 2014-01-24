/*
 * Copyright (C) 2004-2006 Atmel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __ASM_AVR32_TYPES_H
#define __ASM_AVR32_TYPES_H

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
#endif /* __ASM_AVR32_TYPES_H */
