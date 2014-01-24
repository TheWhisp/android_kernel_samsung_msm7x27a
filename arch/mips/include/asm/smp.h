/*
 * This file is subject to the terms and conditions of the GNU General
 * Public License.  See the file "COPYING" in the main directory of this
 * archive for more details.
 *
 * Copyright (C) 2000 - 2001 by Kanoj Sarcar (kanoj@sgi.com)
 * Copyright (C) 2000 - 2001 by Silicon Graphics, Inc.
 * Copyright (C) 2000, 2001, 2002 Ralf Baechle
 * Copyright (C) 2000, 2001 Broadcom Corporation
 */
#ifndef __ASM_SMP_H
#define __ASM_SMP_H

#include <linux/bitops.h>
#include <linux/linkage.h>
#include <linux/smp.h>
#include <linux/threads.h>
#include <linux/cpumask.h>

<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/atomic.h>
=======
#include <linux/atomic.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/atomic.h>
>>>>>>> refs/remotes/origin/master
#include <asm/smp-ops.h>

extern int smp_num_siblings;
extern cpumask_t cpu_sibling_map[];

#define raw_smp_processor_id() (current_thread_info()->cpu)

/* Map from cpu id to sequential logical cpu number.  This will only
<<<<<<< HEAD
   not be idempotent when cpus failed to come on-line.  */
=======
   not be idempotent when cpus failed to come on-line.	*/
>>>>>>> refs/remotes/origin/master
extern int __cpu_number_map[NR_CPUS];
#define cpu_number_map(cpu)  __cpu_number_map[cpu]

/* The reverse map from sequential logical cpu number to cpu id.  */
extern int __cpu_logical_map[NR_CPUS];
#define cpu_logical_map(cpu)  __cpu_logical_map[cpu]

#define NO_PROC_ID	(-1)

<<<<<<< HEAD
#define SMP_RESCHEDULE_YOURSELF	0x1	/* XXX braindead */
#define SMP_CALL_FUNCTION	0x2
/* Octeon - Tell another core to flush its icache */
#define SMP_ICACHE_FLUSH	0x4
=======
#define SMP_RESCHEDULE_YOURSELF 0x1	/* XXX braindead */
#define SMP_CALL_FUNCTION	0x2
/* Octeon - Tell another core to flush its icache */
#define SMP_ICACHE_FLUSH	0x4
/* Used by kexec crashdump to save all cpu's state */
#define SMP_DUMP		0x8
>>>>>>> refs/remotes/origin/master

extern volatile cpumask_t cpu_callin_map;

extern void asmlinkage smp_bootstrap(void);

/*
 * this function sends a 'reschedule' IPI to another CPU.
 * it goes straight through and wastes no time serializing
 * anything. Worst case is that we lose a reschedule ...
 */
static inline void smp_send_reschedule(int cpu)
{
	extern struct plat_smp_ops *mp_ops;	/* private */

	mp_ops->send_ipi_single(cpu, SMP_RESCHEDULE_YOURSELF);
}

#ifdef CONFIG_HOTPLUG_CPU
static inline int __cpu_disable(void)
{
<<<<<<< HEAD
	extern struct plat_smp_ops *mp_ops;     /* private */
=======
	extern struct plat_smp_ops *mp_ops;	/* private */
>>>>>>> refs/remotes/origin/master

	return mp_ops->cpu_disable();
}

static inline void __cpu_die(unsigned int cpu)
{
<<<<<<< HEAD
	extern struct plat_smp_ops *mp_ops;     /* private */
=======
	extern struct plat_smp_ops *mp_ops;	/* private */
>>>>>>> refs/remotes/origin/master

	mp_ops->cpu_die(cpu);
}

extern void play_dead(void);
#endif

extern asmlinkage void smp_call_function_interrupt(void);

static inline void arch_send_call_function_single_ipi(int cpu)
{
<<<<<<< HEAD
	extern struct plat_smp_ops *mp_ops;     /* private */
=======
	extern struct plat_smp_ops *mp_ops;	/* private */
>>>>>>> refs/remotes/origin/master

	mp_ops->send_ipi_mask(&cpumask_of_cpu(cpu), SMP_CALL_FUNCTION);
}

static inline void arch_send_call_function_ipi_mask(const struct cpumask *mask)
{
<<<<<<< HEAD
	extern struct plat_smp_ops *mp_ops;     /* private */
=======
	extern struct plat_smp_ops *mp_ops;	/* private */
>>>>>>> refs/remotes/origin/master

	mp_ops->send_ipi_mask(mask, SMP_CALL_FUNCTION);
}

<<<<<<< HEAD
=======
#if defined(CONFIG_KEXEC)
extern void (*dump_ipi_function_ptr)(void *);
void dump_send_ipi(void (*dump_ipi_callback)(void *));
#endif
>>>>>>> refs/remotes/origin/master
#endif /* __ASM_SMP_H */
