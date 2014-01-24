/* linux/arch/arm/mach-exynos4/platsmp.c
 *
 * Copyright (c) 2010-2011 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com
 *
 * Cloned from linux/arch/arm/mach-vexpress/platsmp.c
 *
 *  Copyright (C) 2002 ARM Ltd.
 *  All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/init.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/jiffies.h>
#include <linux/smp.h>
#include <linux/io.h>

#include <asm/cacheflush.h>
<<<<<<< HEAD
#include <asm/hardware/gic.h>
#include <asm/smp_plat.h>
#include <asm/smp_scu.h>
=======
#include <asm/smp_plat.h>
#include <asm/smp_scu.h>
#include <asm/firmware.h>
>>>>>>> refs/remotes/origin/master

#include <mach/hardware.h>
#include <mach/regs-clock.h>
#include <mach/regs-pmu.h>

#include <plat/cpu.h>

<<<<<<< HEAD
extern void exynos4_secondary_startup(void);

#define CPU1_BOOT_REG		(samsung_rev() == EXYNOS4210_REV_1_1 ? \
				S5P_INFORM5 : S5P_VA_SYSRAM)

/*
 * control for which core is the next to come out of the secondary
 * boot "holding pen"
 */

volatile int __cpuinitdata pen_release = -1;
=======
#include "common.h"

extern void exynos4_secondary_startup(void);

static inline void __iomem *cpu_boot_reg_base(void)
{
	if (soc_is_exynos4210() && samsung_rev() == EXYNOS4210_REV_1_1)
		return S5P_INFORM5;
	return S5P_VA_SYSRAM;
}

static inline void __iomem *cpu_boot_reg(int cpu)
{
	void __iomem *boot_reg;

	boot_reg = cpu_boot_reg_base();
	if (soc_is_exynos4412())
		boot_reg += 4*cpu;
	else if (soc_is_exynos5420())
		boot_reg += 4;
	return boot_reg;
}
>>>>>>> refs/remotes/origin/master

/*
 * Write pen_release in a way that is guaranteed to be visible to all
 * observers, irrespective of whether they're taking part in coherency
 * or not.  This is necessary for the hotplug code to work reliably.
 */
static void write_pen_release(int val)
{
	pen_release = val;
	smp_wmb();
	__cpuc_flush_dcache_area((void *)&pen_release, sizeof(pen_release));
	outer_clean_range(__pa(&pen_release), __pa(&pen_release + 1));
}

static void __iomem *scu_base_addr(void)
{
	return (void __iomem *)(S5P_VA_SCU);
}

static DEFINE_SPINLOCK(boot_lock);

<<<<<<< HEAD
void __cpuinit platform_secondary_init(unsigned int cpu)
{
	/*
	 * if any interrupts are already enabled for the primary
	 * core (e.g. timer irq), then they will not have been enabled
	 * for us: do so
	 */
	gic_secondary_init(0);

	/*
=======
static void exynos_secondary_init(unsigned int cpu)
{
	/*
>>>>>>> refs/remotes/origin/master
	 * let the primary processor know we're out of the
	 * pen, then head off into the C entry point
	 */
	write_pen_release(-1);

	/*
	 * Synchronise with the boot thread.
	 */
	spin_lock(&boot_lock);
	spin_unlock(&boot_lock);
}

<<<<<<< HEAD
int __cpuinit boot_secondary(unsigned int cpu, struct task_struct *idle)
{
	unsigned long timeout;
=======
static int exynos_boot_secondary(unsigned int cpu, struct task_struct *idle)
{
	unsigned long timeout;
	unsigned long phys_cpu = cpu_logical_map(cpu);
>>>>>>> refs/remotes/origin/master

	/*
	 * Set synchronisation state between this boot processor
	 * and the secondary one
	 */
	spin_lock(&boot_lock);

	/*
	 * The secondary processor is waiting to be released from
	 * the holding pen - release it, then wait for it to flag
	 * that it has been released by resetting pen_release.
	 *
	 * Note that "pen_release" is the hardware CPU ID, whereas
	 * "cpu" is Linux's internal ID.
	 */
<<<<<<< HEAD
	write_pen_release(cpu_logical_map(cpu));
=======
	write_pen_release(phys_cpu);
>>>>>>> refs/remotes/origin/master

	if (!(__raw_readl(S5P_ARM_CORE1_STATUS) & S5P_CORE_LOCAL_PWR_EN)) {
		__raw_writel(S5P_CORE_LOCAL_PWR_EN,
			     S5P_ARM_CORE1_CONFIGURATION);

		timeout = 10;

		/* wait max 10 ms until cpu1 is on */
		while ((__raw_readl(S5P_ARM_CORE1_STATUS)
			& S5P_CORE_LOCAL_PWR_EN) != S5P_CORE_LOCAL_PWR_EN) {
			if (timeout-- == 0)
				break;

			mdelay(1);
		}

		if (timeout == 0) {
			printk(KERN_ERR "cpu1 power enable failed");
			spin_unlock(&boot_lock);
			return -ETIMEDOUT;
		}
	}
	/*
	 * Send the secondary CPU a soft interrupt, thereby causing
	 * the boot monitor to read the system wide flags register,
	 * and branch to the address found there.
	 */

	timeout = jiffies + (1 * HZ);
	while (time_before(jiffies, timeout)) {
<<<<<<< HEAD
		smp_rmb();

		__raw_writel(virt_to_phys(exynos4_secondary_startup),
			CPU1_BOOT_REG);
		gic_raise_softirq(cpumask_of(cpu), 1);
=======
		unsigned long boot_addr;

		smp_rmb();

		boot_addr = virt_to_phys(exynos4_secondary_startup);

		/*
		 * Try to set boot address using firmware first
		 * and fall back to boot register if it fails.
		 */
		if (call_firmware_op(set_cpu_boot_addr, phys_cpu, boot_addr))
			__raw_writel(boot_addr, cpu_boot_reg(phys_cpu));

		call_firmware_op(cpu_boot, phys_cpu);

		arch_send_wakeup_ipi_mask(cpumask_of(cpu));
>>>>>>> refs/remotes/origin/master

		if (pen_release == -1)
			break;

		udelay(10);
	}

	/*
	 * now the secondary core is starting up let it run its
	 * calibrations, then wait for it to finish
	 */
	spin_unlock(&boot_lock);

	return pen_release != -1 ? -ENOSYS : 0;
}

/*
 * Initialise the CPU possible map early - this describes the CPUs
 * which may be present or become present in the system.
 */

<<<<<<< HEAD
void __init smp_init_cpus(void)
=======
static void __init exynos_smp_init_cpus(void)
>>>>>>> refs/remotes/origin/master
{
	void __iomem *scu_base = scu_base_addr();
	unsigned int i, ncores;

<<<<<<< HEAD
	if (soc_is_exynos5250())
		ncores = 2;
	else
		ncores = scu_base ? scu_get_core_count(scu_base) : 1;
=======
	if (read_cpuid_part_number() == ARM_CPU_PART_CORTEX_A9)
		ncores = scu_base ? scu_get_core_count(scu_base) : 1;
	else
		/*
		 * CPU Nodes are passed thru DT and set_cpu_possible
		 * is set by "arm_dt_init_cpu_maps".
		 */
		return;
>>>>>>> refs/remotes/origin/master

	/* sanity check */
	if (ncores > nr_cpu_ids) {
		pr_warn("SMP: %u cores greater than maximum (%u), clipping\n",
			ncores, nr_cpu_ids);
		ncores = nr_cpu_ids;
	}

	for (i = 0; i < ncores; i++)
		set_cpu_possible(i, true);
<<<<<<< HEAD

	set_smp_cross_call(gic_raise_softirq);
}

void __init platform_smp_prepare_cpus(unsigned int max_cpus)
{
	if (!soc_is_exynos5250())
=======
}

static void __init exynos_smp_prepare_cpus(unsigned int max_cpus)
{
	int i;

	if (read_cpuid_part_number() == ARM_CPU_PART_CORTEX_A9)
>>>>>>> refs/remotes/origin/master
		scu_enable(scu_base_addr());

	/*
	 * Write the address of secondary startup into the
	 * system-wide flags register. The boot monitor waits
	 * until it receives a soft interrupt, and then the
	 * secondary CPU branches to this address.
<<<<<<< HEAD
	 */
	__raw_writel(virt_to_phys(exynos4_secondary_startup),
			CPU1_BOOT_REG);
}
=======
	 *
	 * Try using firmware operation first and fall back to
	 * boot register if it fails.
	 */
	for (i = 1; i < max_cpus; ++i) {
		unsigned long phys_cpu;
		unsigned long boot_addr;

		phys_cpu = cpu_logical_map(i);
		boot_addr = virt_to_phys(exynos4_secondary_startup);

		if (call_firmware_op(set_cpu_boot_addr, phys_cpu, boot_addr))
			__raw_writel(boot_addr, cpu_boot_reg(phys_cpu));
	}
}

struct smp_operations exynos_smp_ops __initdata = {
	.smp_init_cpus		= exynos_smp_init_cpus,
	.smp_prepare_cpus	= exynos_smp_prepare_cpus,
	.smp_secondary_init	= exynos_secondary_init,
	.smp_boot_secondary	= exynos_boot_secondary,
#ifdef CONFIG_HOTPLUG_CPU
	.cpu_die		= exynos_cpu_die,
#endif
};
>>>>>>> refs/remotes/origin/master
