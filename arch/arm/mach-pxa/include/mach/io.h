/*
 * arch/arm/mach-pxa/include/mach/io.h
 *
 * Copied from asm/arch/sa1100/io.h
 */
#ifndef __ASM_ARM_ARCH_IO_H
#define __ASM_ARM_ARCH_IO_H

<<<<<<< HEAD
<<<<<<< HEAD
#include <mach/hardware.h>

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#define IO_SPACE_LIMIT 0xffffffff

/*
 * We don't actually have real ISA nor PCI buses, but there is so many
 * drivers out there that might just work if we fake them...
 */
#define __io(a)		__typesafe_io(a)
<<<<<<< HEAD
<<<<<<< HEAD
#define __mem_pci(a)	(a)
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#endif
