/*
 *  arch/arm/include/asm/proc-fns.h
 *
 *  Copyright (C) 1997-1999 Russell King
 *  Copyright (C) 2000 Deep Blue Solutions Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __ASM_PROCFNS_H
#define __ASM_PROCFNS_H

#ifdef __KERNEL__

#include <asm/glue-proc.h>
#include <asm/page.h>

#ifndef __ASSEMBLY__

struct mm_struct;

/*
 * Don't change this structure - ASM code relies on it.
 */
extern struct processor {
	/* MISC
	 * get data abort address/flags
	 */
	void (*_data_abort)(unsigned long pc);
	/*
	 * Retrieve prefetch fault address
	 */
	unsigned long (*_prefetch_abort)(unsigned long lr);
	/*
	 * Set up any processor specifics
	 */
	void (*_proc_init)(void);
	/*
	 * Disable any processor specifics
	 */
	void (*_proc_fin)(void);
	/*
	 * Special stuff for a reset
	 */
	void (*reset)(unsigned long addr) __attribute__((noreturn));
	/*
	 * Idle the processor
	 */
	int (*_do_idle)(void);
	/*
	 * Processor architecture specific
	 */
	/*
	 * clean a virtual address range from the
	 * D-cache without flushing the cache.
	 */
	void (*dcache_clean_area)(void *addr, int size);

	/*
	 * Set the page table
	 */
<<<<<<< HEAD
	void (*switch_mm)(unsigned long pgd_phys, struct mm_struct *mm);
=======
	void (*switch_mm)(phys_addr_t pgd_phys, struct mm_struct *mm);
>>>>>>> refs/remotes/origin/master
	/*
	 * Set a possibly extended PTE.  Non-extended PTEs should
	 * ignore 'ext'.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	void (*set_pte_ext)(pte_t *ptep, pte_t pte, unsigned int ext);
=======
=======
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_ARM_LPAE
	void (*set_pte_ext)(pte_t *ptep, pte_t pte);
#else
	void (*set_pte_ext)(pte_t *ptep, pte_t pte, unsigned int ext);
#endif
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	/* Suspend/resume */
	unsigned int suspend_size;
	void (*do_suspend)(void *);
	void (*do_resume)(void *);
} processor;

#ifndef MULTI_CPU
extern void cpu_proc_init(void);
extern void cpu_proc_fin(void);
extern int cpu_do_idle(void);
extern void cpu_dcache_clean_area(void *, int);
<<<<<<< HEAD
extern void cpu_do_switch_mm(unsigned long pgd_phys, struct mm_struct *mm);
<<<<<<< HEAD
extern void cpu_set_pte_ext(pte_t *ptep, pte_t pte, unsigned int ext);
extern void cpu_reset(unsigned long addr) __attribute__((noreturn));
#else
#define cpu_proc_init()			processor._proc_init()
#define cpu_proc_fin()			processor._proc_fin()
#define cpu_reset(addr)			processor.reset(addr)
#define cpu_do_idle()			processor._do_idle()
#define cpu_dcache_clean_area(addr,sz)	processor.dcache_clean_area(addr,sz)
#define cpu_set_pte_ext(ptep,pte,ext)	processor.set_pte_ext(ptep,pte,ext)
#define cpu_do_switch_mm(pgd,mm)	processor.switch_mm(pgd,mm)
=======
=======
extern void cpu_do_switch_mm(phys_addr_t pgd_phys, struct mm_struct *mm);
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_ARM_LPAE
extern void cpu_set_pte_ext(pte_t *ptep, pte_t pte);
#else
extern void cpu_set_pte_ext(pte_t *ptep, pte_t pte, unsigned int ext);
#endif
extern void cpu_reset(unsigned long addr) __attribute__((noreturn));

/* These three are private to arch/arm/kernel/suspend.c */
extern void cpu_do_suspend(void *);
extern void cpu_do_resume(void *);
#else
#define cpu_proc_init			processor._proc_init
#define cpu_proc_fin			processor._proc_fin
#define cpu_reset			processor.reset
#define cpu_do_idle			processor._do_idle
#define cpu_dcache_clean_area		processor.dcache_clean_area
#define cpu_set_pte_ext			processor.set_pte_ext
#define cpu_do_switch_mm		processor.switch_mm

/* These three are private to arch/arm/kernel/suspend.c */
#define cpu_do_suspend			processor.do_suspend
#define cpu_do_resume			processor.do_resume
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#endif

extern void cpu_resume(void);

#include <asm/memory.h>

#ifdef CONFIG_MMU

#define cpu_switch_mm(pgd,mm) cpu_do_switch_mm(virt_to_phys(pgd),mm)

<<<<<<< HEAD
<<<<<<< HEAD
=======
#ifdef CONFIG_ARM_LPAE
#define cpu_get_pgd()	\
	({						\
		unsigned long pg, pg2;			\
		__asm__("mrrc	p15, 0, %0, %1, c2"	\
			: "=r" (pg), "=r" (pg2)		\
			:				\
			: "cc");			\
=======
#ifdef CONFIG_ARM_LPAE

#define cpu_get_ttbr(nr)					\
	({							\
		u64 ttbr;					\
		__asm__("mrrc	p15, " #nr ", %Q0, %R0, c2"	\
			: "=r" (ttbr));				\
		ttbr;						\
	})

#define cpu_set_ttbr(nr, val)					\
	do {							\
		u64 ttbr = val;					\
		__asm__("mcrr	p15, " #nr ", %Q0, %R0, c2"	\
			: : "r" (ttbr));			\
	} while (0)

#define cpu_get_pgd()	\
	({						\
		u64 pg = cpu_get_ttbr(0);		\
>>>>>>> refs/remotes/origin/master
		pg &= ~(PTRS_PER_PGD*sizeof(pgd_t)-1);	\
		(pgd_t *)phys_to_virt(pg);		\
	})
#else
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#define cpu_get_pgd()	\
	({						\
		unsigned long pg;			\
		__asm__("mrc	p15, 0, %0, c2, c0, 0"	\
			 : "=r" (pg) : : "cc");		\
		pg &= ~0x3fff;				\
		(pgd_t *)phys_to_virt(pg);		\
	})
<<<<<<< HEAD
<<<<<<< HEAD
=======
#endif
>>>>>>> refs/remotes/origin/cm-10.0
=======
#endif

#else	/*!CONFIG_MMU */

#define cpu_switch_mm(pgd,mm)	{ }
>>>>>>> refs/remotes/origin/master

#endif

#endif /* __ASSEMBLY__ */
#endif /* __KERNEL__ */
#endif /* __ASM_PROCFNS_H */
