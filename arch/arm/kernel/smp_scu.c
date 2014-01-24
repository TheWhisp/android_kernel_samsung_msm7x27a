/*
 *  linux/arch/arm/kernel/smp_scu.c
 *
 *  Copyright (C) 2002 ARM Ltd.
 *  All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/init.h>
#include <linux/io.h>

<<<<<<< HEAD
=======
#include <asm/smp_plat.h>
>>>>>>> refs/remotes/origin/master
#include <asm/smp_scu.h>
#include <asm/cacheflush.h>
#include <asm/cputype.h>

#define SCU_CTRL		0x00
#define SCU_CONFIG		0x04
#define SCU_CPU_STATUS		0x08
#define SCU_INVALIDATE		0x0c
#define SCU_FPGA_REVISION	0x10

<<<<<<< HEAD
<<<<<<< HEAD
=======
#ifdef CONFIG_SMP
>>>>>>> refs/remotes/origin/cm-10.0
=======
#ifdef CONFIG_SMP
>>>>>>> refs/remotes/origin/master
/*
 * Get the number of CPU cores from the SCU configuration
 */
unsigned int __init scu_get_core_count(void __iomem *scu_base)
{
<<<<<<< HEAD
	unsigned int ncores = __raw_readl(scu_base + SCU_CONFIG);
=======
	unsigned int ncores = readl_relaxed(scu_base + SCU_CONFIG);
>>>>>>> refs/remotes/origin/master
	return (ncores & 0x03) + 1;
}

/*
 * Enable the SCU
 */
<<<<<<< HEAD
<<<<<<< HEAD
void __init scu_enable(void __iomem *scu_base)
=======
void scu_enable(void __iomem *scu_base)
>>>>>>> refs/remotes/origin/cm-10.0
=======
void scu_enable(void __iomem *scu_base)
>>>>>>> refs/remotes/origin/master
{
	u32 scu_ctrl;

#ifdef CONFIG_ARM_ERRATA_764369
	/* Cortex-A9 only */
<<<<<<< HEAD
	if ((read_cpuid(CPUID_ID) & 0xff0ffff0) == 0x410fc090) {
		scu_ctrl = __raw_readl(scu_base + 0x30);
		if (!(scu_ctrl & 1))
			__raw_writel(scu_ctrl | 0x1, scu_base + 0x30);
	}
#endif

	scu_ctrl = __raw_readl(scu_base + SCU_CTRL);
=======
	if ((read_cpuid_id() & 0xff0ffff0) == 0x410fc090) {
		scu_ctrl = readl_relaxed(scu_base + 0x30);
		if (!(scu_ctrl & 1))
			writel_relaxed(scu_ctrl | 0x1, scu_base + 0x30);
	}
#endif

	scu_ctrl = readl_relaxed(scu_base + SCU_CTRL);
>>>>>>> refs/remotes/origin/master
	/* already enabled? */
	if (scu_ctrl & 1)
		return;

	scu_ctrl |= 1;
<<<<<<< HEAD
	__raw_writel(scu_ctrl, scu_base + SCU_CTRL);
=======
	writel_relaxed(scu_ctrl, scu_base + SCU_CTRL);
>>>>>>> refs/remotes/origin/master

	/*
	 * Ensure that the data accessed by CPU0 before the SCU was
	 * initialised is visible to the other CPUs.
	 */
	flush_cache_all();
}
<<<<<<< HEAD
<<<<<<< HEAD
=======
#endif
>>>>>>> refs/remotes/origin/cm-10.0
=======
#endif
>>>>>>> refs/remotes/origin/master

/*
 * Set the executing CPUs power mode as defined.  This will be in
 * preparation for it executing a WFI instruction.
 *
 * This function must be called with preemption disabled, and as it
 * has the side effect of disabling coherency, caches must have been
 * flushed.  Interrupts must also have been disabled.
 */
int scu_power_mode(void __iomem *scu_base, unsigned int mode)
{
	unsigned int val;
<<<<<<< HEAD
	int cpu = smp_processor_id();
=======
	int cpu = MPIDR_AFFINITY_LEVEL(cpu_logical_map(smp_processor_id()), 0);
>>>>>>> refs/remotes/origin/master

	if (mode > 3 || mode == 1 || cpu > 3)
		return -EINVAL;

<<<<<<< HEAD
	val = __raw_readb(scu_base + SCU_CPU_STATUS + cpu) & ~0x03;
	val |= mode;
	__raw_writeb(val, scu_base + SCU_CPU_STATUS + cpu);
=======
	val = readb_relaxed(scu_base + SCU_CPU_STATUS + cpu) & ~0x03;
	val |= mode;
	writeb_relaxed(val, scu_base + SCU_CPU_STATUS + cpu);
>>>>>>> refs/remotes/origin/master

	return 0;
}
