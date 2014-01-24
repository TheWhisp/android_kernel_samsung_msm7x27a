/*
 * Copyright 2010 Tilera Corporation. All Rights Reserved.
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation, version 2.
 *
 *   This program is distributed in the hope that it will be useful, but
 *   WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
 *   NON INFRINGEMENT.  See the GNU General Public License for
 *   more details.
 */
<<<<<<< HEAD

#ifndef _ASM_TILE_SETUP_H
#define _ASM_TILE_SETUP_H

#define COMMAND_LINE_SIZE	2048

#ifdef __KERNEL__

#include <linux/pfn.h>
#include <linux/init.h>
=======
#ifndef _ASM_TILE_SETUP_H
#define _ASM_TILE_SETUP_H


#include <linux/pfn.h>
#include <linux/init.h>
#include <uapi/asm/setup.h>
>>>>>>> refs/remotes/origin/master

/*
 * Reserved space for vmalloc and iomap - defined in asm/page.h
 */
#define MAXMEM_PFN	PFN_DOWN(MAXMEM)

<<<<<<< HEAD
void early_panic(const char *fmt, ...);
void warn_early_printk(void);
void __init disable_early_printk(void);

<<<<<<< HEAD
=======
=======
int tile_console_write(const char *buf, int count);
void early_panic(const char *fmt, ...);

>>>>>>> refs/remotes/origin/master
/* Init-time routine to do tile-specific per-cpu setup. */
void setup_cpu(int boot);

/* User-level DMA management functions */
void grant_dma_mpls(void);
void restrict_dma_mpls(void);

#ifdef CONFIG_HARDWALL
/* User-level network management functions */
void reset_network_state(void);
<<<<<<< HEAD
void grant_network_mpls(void);
void restrict_network_mpls(void);
struct task_struct;
int hardwall_deactivate(struct task_struct *task);

/* Hook hardwall code into changes in affinity. */
#define arch_set_cpus_allowed(p, new_mask) do { \
	if (p->thread.hardwall && !cpumask_equal(&p->cpus_allowed, new_mask)) \
		hardwall_deactivate(p); \
} while (0)
#endif

>>>>>>> refs/remotes/origin/cm-10.0
#endif /* __KERNEL__ */

=======
struct task_struct;
void hardwall_switch_tasks(struct task_struct *prev, struct task_struct *next);
void hardwall_deactivate_all(struct task_struct *task);
int hardwall_ipi_valid(int cpu);

/* Hook hardwall code into changes in affinity. */
#define arch_set_cpus_allowed(p, new_mask) do { \
	if (!cpumask_equal(&p->cpus_allowed, new_mask)) \
		hardwall_deactivate_all(p); \
} while (0)
#endif

>>>>>>> refs/remotes/origin/master
#endif /* _ASM_TILE_SETUP_H */
