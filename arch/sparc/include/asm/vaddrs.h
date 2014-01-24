#ifndef _SPARC_VADDRS_H
#define _SPARC_VADDRS_H

#include <asm/head.h>

/*
 * asm/vaddrs.h:  Here we define the virtual addresses at
 *                      which important things will be mapped.
 *
 * Copyright (C) 1995 David S. Miller (davem@caip.rutgers.edu)
 * Copyright (C) 2000 Anton Blanchard (anton@samba.org)
 */

#define SRMMU_MAXMEM		0x0c000000

#define SRMMU_NOCACHE_VADDR	(KERNBASE + SRMMU_MAXMEM)
				/* = 0x0fc000000 */
/* XXX Empiricals - this needs to go away - KMW */
#define SRMMU_MIN_NOCACHE_PAGES (550)
#define SRMMU_MAX_NOCACHE_PAGES	(1280)

/* The following constant is used in mm/srmmu.c::srmmu_nocache_calcsize()
 * to determine the amount of memory that will be reserved as nocache:
 *
 * 256 pages will be taken as nocache per each
 * SRMMU_NOCACHE_ALCRATIO MB of system memory.
 *
 * limits enforced:	nocache minimum = 256 pages
 *			nocache maximum = 1280 pages
 */
#define SRMMU_NOCACHE_ALCRATIO	64	/* 256 pages per 64MB of system RAM */

<<<<<<< HEAD
=======
#ifndef __ASSEMBLY__
#include <asm/kmap_types.h>

enum fixed_addresses {
	FIX_HOLE,
#ifdef CONFIG_HIGHMEM
	FIX_KMAP_BEGIN,
	FIX_KMAP_END = (KM_TYPE_NR * NR_CPUS),
#endif
	__end_of_fixed_addresses
};
#endif

/* Leave one empty page between IO pages at 0xfd000000 and
 * the top of the fixmap.
 */
#define FIXADDR_TOP		(0xfcfff000UL)
#define FIXADDR_SIZE		((FIX_KMAP_END + 1) << PAGE_SHIFT)
#define FIXADDR_START		(FIXADDR_TOP - FIXADDR_SIZE)

#define __fix_to_virt(x)        (FIXADDR_TOP - ((x) << PAGE_SHIFT))

>>>>>>> refs/remotes/origin/master
#define SUN4M_IOBASE_VADDR	0xfd000000 /* Base for mapping pages */
#define IOBASE_VADDR		0xfe000000
#define IOBASE_END		0xfe600000

<<<<<<< HEAD
/*
 * On the sun4/4c we need a place
 * to reliably map locked down kernel data.  This includes the
 * task_struct and kernel stack pages of each process plus the
 * scsi buffers during dvma IO transfers, also the floppy buffers
 * during pseudo dma which runs with traps off (no faults allowed).
 * Some quick calculations yield:
 *       NR_TASKS <512> * (3 * PAGE_SIZE) == 0x600000
 * Subtract this from 0xc00000 and you get 0x927C0 of vm left
 * over to map SCSI dvma + floppy pseudo-dma buffers.  So be
 * careful if you change NR_TASKS or else there won't be enough
 * room for it all.
 */
#define SUN4C_LOCK_VADDR	0xff000000
#define SUN4C_LOCK_END		0xffc00000

=======
>>>>>>> refs/remotes/origin/master
#define KADB_DEBUGGER_BEGVM	0xffc00000 /* Where kern debugger is in virt-mem */
#define KADB_DEBUGGER_ENDVM	0xffd00000
#define DEBUG_FIRSTVADDR	KADB_DEBUGGER_BEGVM
#define DEBUG_LASTVADDR		KADB_DEBUGGER_ENDVM

#define LINUX_OPPROM_BEGVM	0xffd00000
#define LINUX_OPPROM_ENDVM	0xfff00000

#define DVMA_VADDR		0xfff00000 /* Base area of the DVMA on suns */
#define DVMA_END		0xfffc0000

#endif /* !(_SPARC_VADDRS_H) */
