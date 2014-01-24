/*
 *  arch/mips/include/asm/prom.h
 *
 *  Copyright (C) 2010 Cisco Systems Inc. <dediao@cisco.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
<<<<<<< HEAD
<<<<<<< HEAD
#ifndef __ASM_MIPS_PROM_H
#define __ASM_MIPS_PROM_H
=======
#ifndef __ASM_PROM_H
#define __ASM_PROM_H
>>>>>>> refs/remotes/origin/cm-10.0

#ifdef CONFIG_OF
#include <asm/bootinfo.h>

extern int early_init_dt_scan_memory_arch(unsigned long node,
	const char *uname, int depth, void *data);

extern int reserve_mem_mach(unsigned long addr, unsigned long size);
extern void free_mem_mach(unsigned long addr, unsigned long size);

extern void device_tree_init(void);
=======
#ifndef __ASM_PROM_H
#define __ASM_PROM_H

#ifdef CONFIG_OF
#include <linux/bug.h>
#include <linux/io.h>
#include <linux/types.h>
#include <asm/bootinfo.h>

extern void device_tree_init(void);

struct boot_param_header;

extern void __dt_setup_arch(struct boot_param_header *bph);

#define dt_setup_arch(sym)						\
({									\
	extern struct boot_param_header __dtb_##sym##_begin;		\
									\
	__dt_setup_arch(&__dtb_##sym##_begin);				\
})

>>>>>>> refs/remotes/origin/master
#else /* CONFIG_OF */
static inline void device_tree_init(void) { }
#endif /* CONFIG_OF */

<<<<<<< HEAD
<<<<<<< HEAD
#endif /* _ASM_MIPS_PROM_H */
=======
#endif /* __ASM_PROM_H */
>>>>>>> refs/remotes/origin/cm-10.0
=======
extern char *mips_get_machine_name(void);
extern void mips_set_machine_name(const char *name);

#endif /* __ASM_PROM_H */
>>>>>>> refs/remotes/origin/master
