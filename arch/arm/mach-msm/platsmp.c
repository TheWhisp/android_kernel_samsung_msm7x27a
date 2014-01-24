/*
 *  Copyright (C) 2002 ARM Ltd.
 *  All Rights Reserved
 *  Copyright (c) 2010-2012, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/cpumask.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/io.h>

#include <asm/hardware/gic.h>
#include <asm/cacheflush.h>
#include <asm/cputype.h>
#include <asm/mach-types.h>
<<<<<<< HEAD

#include <mach/socinfo.h>
#include <mach/smp.h>
=======
#include <asm/smp_plat.h>

#include <mach/socinfo.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <mach/hardware.h>
#include <mach/msm_iomap.h>

#include "pm.h"
#include "scm-boot.h"
#include "spm.h"

<<<<<<< HEAD
int pen_release = -1;

/* Initialize the present map (cpu_set(i, cpu_present_map)). */
void __init platform_smp_prepare_cpus(unsigned int max_cpus)
{
	int i;

	for (i = 0; i < max_cpus; i++)
		cpu_set(i, cpu_present_map);
}

void __init smp_init_cpus(void)
{
	unsigned int i, ncores = get_core_count();

	for (i = 0; i < ncores; i++)
		cpu_set(i, cpu_possible_map);

	set_smp_cross_call(gic_raise_softirq);
=======
#define VDD_SC1_ARRAY_CLAMP_GFS_CTL 0x15A0
#define SCSS_CPU1CORE_RESET 0xD80
#define SCSS_DBG_STATUS_CORE_PWRDUP 0xE64

extern void msm_secondary_startup(void);

/*
 * control for which core is the next to come out of the secondary
 * boot "holding pen".
 */
volatile int pen_release = -1;

static DEFINE_SPINLOCK(boot_lock);

void __cpuinit platform_secondary_init(unsigned int cpu)
{
	WARN_ON(msm_platform_secondary_init(cpu));

	/*
	 * if any interrupts are already enabled for the primary
	 * core (e.g. timer irq), then they will not have been enabled
	 * for us: do so
	 */
	gic_secondary_init(0);

	/*
	 * Synchronise with the boot thread.
	 */
	spin_lock(&boot_lock);
	spin_unlock(&boot_lock);
>>>>>>> refs/remotes/origin/cm-10.0
}

static int __cpuinit scorpion_release_secondary(void)
{
	void *base_ptr = ioremap_nocache(0x00902000, SZ_4K*2);
	if (!base_ptr)
		return -EINVAL;

<<<<<<< HEAD
	writel_relaxed(0x0, base_ptr+0x15A0);
	dmb();
	writel_relaxed(0x0, base_ptr+0xD80);
	writel_relaxed(0x3, base_ptr+0xE64);
=======
	writel_relaxed(0, base_ptr + VDD_SC1_ARRAY_CLAMP_GFS_CTL);
	dmb();
	writel_relaxed(0, base_ptr + SCSS_CPU1CORE_RESET);
	writel_relaxed(3, base_ptr + SCSS_DBG_STATUS_CORE_PWRDUP);
>>>>>>> refs/remotes/origin/cm-10.0
	mb();
	iounmap(base_ptr);

	return 0;
}

<<<<<<< HEAD
static int __cpuinit krait_release_secondary_sim(int cpu)
{
	void *base_ptr = ioremap_nocache(0x02088000 + (cpu * 0x10000), SZ_4K);
=======
static int __cpuinit krait_release_secondary_sim(unsigned long base, int cpu)
{
	void *base_ptr = ioremap_nocache(base + (cpu * 0x10000), SZ_4K);
>>>>>>> refs/remotes/origin/cm-10.0
	if (!base_ptr)
		return -ENODEV;

	if (machine_is_msm8960_sim() || machine_is_msm8960_rumi3()) {
		writel_relaxed(0x10, base_ptr+0x04);
		writel_relaxed(0x80, base_ptr+0x04);
	}

	if (machine_is_apq8064_sim())
		writel_relaxed(0xf0000, base_ptr+0x04);

<<<<<<< HEAD
=======
	if (machine_is_msm8974_sim()) {
		writel_relaxed(0x800, base_ptr+0x04);
		writel_relaxed(0x3FFF, base_ptr+0x14);
	}

>>>>>>> refs/remotes/origin/cm-10.0
	mb();
	iounmap(base_ptr);
	return 0;
}

<<<<<<< HEAD
static int __cpuinit krait_release_secondary(int cpu)
{
	void *base_ptr = ioremap_nocache(0x02088000 + (cpu * 0x10000), SZ_4K);
=======
static int __cpuinit krait_release_secondary(unsigned long base, int cpu)
{
	void *base_ptr = ioremap_nocache(base + (cpu * 0x10000), SZ_4K);
>>>>>>> refs/remotes/origin/cm-10.0
	if (!base_ptr)
		return -ENODEV;

	msm_spm_turn_on_cpu_rail(cpu);

	writel_relaxed(0x109, base_ptr+0x04);
	writel_relaxed(0x101, base_ptr+0x04);
	ndelay(300);

	writel_relaxed(0x121, base_ptr+0x04);
	udelay(2);

	writel_relaxed(0x020, base_ptr+0x04);
	udelay(2);

	writel_relaxed(0x000, base_ptr+0x04);
	udelay(100);

	writel_relaxed(0x080, base_ptr+0x04);
	mb();
	iounmap(base_ptr);
	return 0;
}

static int __cpuinit release_secondary(unsigned int cpu)
{
	BUG_ON(cpu >= get_core_count());

	if (cpu_is_msm8x60())
		return scorpion_release_secondary();

	if (machine_is_msm8960_sim() || machine_is_msm8960_rumi3() ||
	    machine_is_apq8064_sim())
<<<<<<< HEAD
		return krait_release_secondary_sim(cpu);

	if (cpu_is_msm8960() || cpu_is_msm8930() || cpu_is_apq8064())
		return krait_release_secondary(cpu);
=======
		return krait_release_secondary_sim(0x02088000, cpu);

	if (machine_is_msm8974_sim())
		return krait_release_secondary_sim(0xf9088000, cpu);

	if (cpu_is_msm8960() || cpu_is_msm8930() || cpu_is_msm8930aa() ||
	    cpu_is_apq8064() || cpu_is_msm8627())
		return krait_release_secondary(0x02088000, cpu);
>>>>>>> refs/remotes/origin/cm-10.0

	WARN(1, "unknown CPU case in release_secondary\n");
	return -EINVAL;
}

DEFINE_PER_CPU(int, cold_boot_done);
static int cold_boot_flags[] = {
	0,
	SCM_FLAG_COLDBOOT_CPU1,
	SCM_FLAG_COLDBOOT_CPU2,
	SCM_FLAG_COLDBOOT_CPU3,
};

<<<<<<< HEAD
/* Executed by primary CPU, brings other CPUs out of reset. Called at boot
   as well as when a CPU is coming out of shutdown induced by echo 0 >
   /sys/devices/.../cpuX.
*/
int __cpuinit boot_secondary(unsigned int cpu, struct task_struct *idle)
{
	int cnt = 0;
	int ret;
	int flag = 0;
=======
int __cpuinit boot_secondary(unsigned int cpu, struct task_struct *idle)
{
	int ret;
	int flag = 0;
	unsigned long timeout;
>>>>>>> refs/remotes/origin/cm-10.0

	pr_debug("Starting secondary CPU %d\n", cpu);

	/* Set preset_lpj to avoid subsequent lpj recalculations */
	preset_lpj = loops_per_jiffy;

	if (cpu > 0 && cpu < ARRAY_SIZE(cold_boot_flags))
		flag = cold_boot_flags[cpu];
	else
		__WARN();

	if (per_cpu(cold_boot_done, cpu) == false) {
		ret = scm_set_boot_addr((void *)
					virt_to_phys(msm_secondary_startup),
					flag);
		if (ret == 0)
			release_secondary(cpu);
		else
			printk(KERN_DEBUG "Failed to set secondary core boot "
					  "address\n");
		per_cpu(cold_boot_done, cpu) = true;
	}

<<<<<<< HEAD
	pen_release = cpu;
	dmac_flush_range((void *)&pen_release,
			 (void *)(&pen_release + sizeof(pen_release)));
	__asm__("sev");
	mb();

	/* Use smp_cross_call() to send a soft interrupt to wake up
	 * the other core.
	 */
	gic_raise_softirq(cpumask_of(cpu), 1);

	while (pen_release != 0xFFFFFFFF) {
		dmac_inv_range((void *)&pen_release,
			       (void *)(&pen_release+sizeof(pen_release)));
		usleep(500);
		if (cnt++ >= 10)
			break;
	}

	return 0;
}

/* Initialization routine for secondary CPUs after they are brought out of
 * reset.
*/
void __cpuinit platform_secondary_init(unsigned int cpu)
{
	pr_debug("CPU%u: Booted secondary processor\n", cpu);

	WARN_ON(msm_platform_secondary_init(cpu));

	trace_hardirqs_off();

	/* Edge trigger PPIs except AVS_SVICINT and AVS_SVICINTSWDONE */
	writel(0xFFFFD7FF, MSM_QGIC_DIST_BASE + GIC_DIST_CONFIG + 4);

	/* RUMI does not adhere to GIC spec by enabling STIs by default.
	 * Enable/clear is supposed to be RO for STIs, but is RW on RUMI.
	 */
	if (!machine_is_msm8x60_sim())
		writel(0x0000FFFF, MSM_QGIC_DIST_BASE + GIC_DIST_ENABLE_SET);

	gic_secondary_init(0);
=======
	/*
	 * set synchronisation state between this boot processor
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
	pen_release = cpu_logical_map(cpu);
	__cpuc_flush_dcache_area((void *)&pen_release, sizeof(pen_release));
	outer_clean_range(__pa(&pen_release), __pa(&pen_release + 1));

	/*
	 * Send the secondary CPU a soft interrupt, thereby causing
	 * the boot monitor to read the system wide flags register,
	 * and branch to the address found there.
	 */
	gic_raise_softirq(cpumask_of(cpu), 1);

	timeout = jiffies + (1 * HZ);
	while (time_before(jiffies, timeout)) {
		smp_rmb();
		if (pen_release == -1)
			break;

		dmac_inv_range((void *)&pen_release,
			       (void *)(&pen_release+sizeof(pen_release)));
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
void __init smp_init_cpus(void)
{
	unsigned int i, ncores = get_core_count();

	if (ncores > nr_cpu_ids) {
		pr_warn("SMP: %u cores greater than maximum (%u), clipping\n",
			ncores, nr_cpu_ids);
		ncores = nr_cpu_ids;
	}

	for (i = 0; i < ncores; i++)
		set_cpu_possible(i, true);

	set_smp_cross_call(gic_raise_softirq);
}

void __init platform_smp_prepare_cpus(unsigned int max_cpus)
{
>>>>>>> refs/remotes/origin/cm-10.0
}
