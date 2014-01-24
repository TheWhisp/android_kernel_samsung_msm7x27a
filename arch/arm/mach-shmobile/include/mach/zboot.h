#ifndef ZBOOT_H
#define ZBOOT_H

<<<<<<< HEAD
#include <asm/mach-types.h>
=======
>>>>>>> refs/remotes/origin/master
#include <mach/zboot_macros.h>

/**************************************************
 *
 *		board specific settings
 *
 **************************************************/

<<<<<<< HEAD
#ifdef CONFIG_MACH_AP4EVB
#define MACH_TYPE	MACH_TYPE_AP4EVB
#include "mach/head-ap4evb.txt"
#elif defined(CONFIG_MACH_MACKEREL)
#define MACH_TYPE	MACH_TYPE_MACKEREL
=======
#ifdef CONFIG_MACH_MACKEREL
#define MEMORY_START	0x40000000
>>>>>>> refs/remotes/origin/master
#include "mach/head-mackerel.txt"
#else
#error "unsupported board."
#endif

#endif /* ZBOOT_H */
