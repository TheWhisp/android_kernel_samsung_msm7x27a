/*
 * Copyright (C) 2007-2009 Michal Simek <monstr@monstr.eu>
 * Copyright (C) 2007-2009 PetaLogix
 * Copyright (C) 2006 Atmark Techno, Inc.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file "COPYING" in the main directory of this archive
 * for more details.
 */
<<<<<<< HEAD

#ifndef _ASM_MICROBLAZE_SETUP_H
#define _ASM_MICROBLAZE_SETUP_H

#define COMMAND_LINE_SIZE	256

# ifndef __ASSEMBLY__

#  ifdef __KERNEL__
=======
#ifndef _ASM_MICROBLAZE_SETUP_H
#define _ASM_MICROBLAZE_SETUP_H

#include <uapi/asm/setup.h>

# ifndef __ASSEMBLY__
>>>>>>> refs/remotes/origin/master
extern unsigned int boot_cpuid; /* move to smp.h */

extern char cmd_line[COMMAND_LINE_SIZE];

<<<<<<< HEAD
<<<<<<< HEAD
void early_printk(const char *fmt, ...);

int setup_early_printk(char *opt);
void disable_early_printk(void);

#if defined(CONFIG_EARLY_PRINTK)
#define eprintk early_printk
#else
#define eprintk printk
#endif

=======
=======
>>>>>>> refs/remotes/origin/master
extern char *klimit;

void early_printk(const char *fmt, ...);

int setup_early_printk(char *opt);
void remap_early_printk(void);
void disable_early_printk(void);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
void heartbeat(void);
void setup_heartbeat(void);

#   ifdef CONFIG_MMU
extern void mmu_reset(void);
extern void early_console_reg_tlb_alloc(unsigned int addr);
#   endif /* CONFIG_MMU */

extern void of_platform_reset_gpio_probe(void);

void time_init(void);
void init_IRQ(void);
void machine_early_init(const char *cmdline, unsigned int ram,
<<<<<<< HEAD
<<<<<<< HEAD
			unsigned int fdt, unsigned int msr);
=======
		unsigned int fdt, unsigned int msr, unsigned int tlb0,
		unsigned int tlb1);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		unsigned int fdt, unsigned int msr, unsigned int tlb0,
		unsigned int tlb1);
>>>>>>> refs/remotes/origin/master

void machine_restart(char *cmd);
void machine_shutdown(void);
void machine_halt(void);
void machine_power_off(void);

<<<<<<< HEAD
<<<<<<< HEAD
=======
void free_init_pages(char *what, unsigned long begin, unsigned long end);
extern void *alloc_maybe_bootmem(size_t size, gfp_t mask);
extern void *zalloc_maybe_bootmem(size_t size, gfp_t mask);

>>>>>>> refs/remotes/origin/cm-10.0
#  endif/* __KERNEL__ */
=======
extern void *alloc_maybe_bootmem(size_t size, gfp_t mask);
extern void *zalloc_maybe_bootmem(size_t size, gfp_t mask);

>>>>>>> refs/remotes/origin/master
# endif /* __ASSEMBLY__ */
#endif /* _ASM_MICROBLAZE_SETUP_H */
