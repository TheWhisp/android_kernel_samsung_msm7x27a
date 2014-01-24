/*
 *  linux/arch/arm/mach-tegra/platsmp.c
 *
 *  Copyright (C) 2002 ARM Ltd.
 *  All Rights Reserved
 *
 *  Copyright (C) 2009 Palm
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
<<<<<<< HEAD

#include <asm/cacheflush.h>
#include <asm/hardware/gic.h>
<<<<<<< HEAD
#include <mach/hardware.h>
#include <asm/mach-types.h>
#include <asm/smp_scu.h>

#include <mach/iomap.h>

extern void tegra_secondary_startup(void);

static DEFINE_SPINLOCK(boot_lock);
=======
#include <asm/mach-types.h>
#include <asm/smp_scu.h>

#include <mach/clk.h>
#include <mach/iomap.h>
#include <mach/powergate.h>
=======
#include <linux/clk/tegra.h>

#include <asm/cacheflush.h>
#include <asm/mach-types.h>
#include <asm/smp_scu.h>
#include <asm/smp_plat.h>
>>>>>>> refs/remotes/origin/master

#include "fuse.h"
#include "flowctrl.h"
#include "reset.h"
<<<<<<< HEAD

extern void tegra_secondary_startup(void);

>>>>>>> refs/remotes/origin/cm-10.0
static void __iomem *scu_base = IO_ADDRESS(TEGRA_ARM_PERIF_BASE);

#define EVP_CPU_RESET_VECTOR \
	(IO_ADDRESS(TEGRA_EXCEPTION_VECTORS_BASE) + 0x100)
#define CLK_RST_CONTROLLER_CLK_CPU_CMPLX \
	(IO_ADDRESS(TEGRA_CLK_RESET_BASE) + 0x4c)
<<<<<<< HEAD
#define CLK_RST_CONTROLLER_RST_CPU_CMPLX_CLR \
	(IO_ADDRESS(TEGRA_CLK_RESET_BASE) + 0x344)
=======
#define CLK_RST_CONTROLLER_RST_CPU_CMPLX_SET \
	(IO_ADDRESS(TEGRA_CLK_RESET_BASE) + 0x340)
#define CLK_RST_CONTROLLER_RST_CPU_CMPLX_CLR \
	(IO_ADDRESS(TEGRA_CLK_RESET_BASE) + 0x344)
#define CLK_RST_CONTROLLER_CLK_CPU_CMPLX_CLR \
	(IO_ADDRESS(TEGRA_CLK_RESET_BASE) + 0x34c)

#define CPU_CLOCK(cpu)	(0x1<<(8+cpu))
#define CPU_RESET(cpu)	(0x1111ul<<(cpu))
>>>>>>> refs/remotes/origin/cm-10.0

void __cpuinit platform_secondary_init(unsigned int cpu)
{
	/*
	 * if any interrupts are already enabled for the primary
	 * core (e.g. timer irq), then they will not have been enabled
	 * for us: do so
	 */
	gic_secondary_init(0);

<<<<<<< HEAD
	/*
	 * Synchronise with the boot thread.
	 */
	spin_lock(&boot_lock);
	spin_unlock(&boot_lock);
}

int __cpuinit boot_secondary(unsigned int cpu, struct task_struct *idle)
{
	unsigned long old_boot_vector;
	unsigned long boot_vector;
	unsigned long timeout;
	u32 reg;

	/*
	 * set synchronisation state between this boot processor
	 * and the secondary one
	 */
	spin_lock(&boot_lock);


	/* set the reset vector to point to the secondary_startup routine */

	boot_vector = virt_to_phys(tegra_secondary_startup);
	old_boot_vector = readl(EVP_CPU_RESET_VECTOR);
	writel(boot_vector, EVP_CPU_RESET_VECTOR);

	/* enable cpu clock on cpu1 */
	reg = readl(CLK_RST_CONTROLLER_CLK_CPU_CMPLX);
	writel(reg & ~(1<<9), CLK_RST_CONTROLLER_CLK_CPU_CMPLX);

	reg = (1<<13) | (1<<9) | (1<<5) | (1<<1);
	writel(reg, CLK_RST_CONTROLLER_RST_CPU_CMPLX_CLR);

	smp_wmb();
	flush_cache_all();

	/* unhalt the cpu */
	writel(0, IO_ADDRESS(TEGRA_FLOW_CTRL_BASE) + 0x14);

	timeout = jiffies + (1 * HZ);
	while (time_before(jiffies, timeout)) {
		if (readl(EVP_CPU_RESET_VECTOR) != boot_vector)
			break;
		udelay(10);
	}

	/* put the old boot vector back */
	writel(old_boot_vector, EVP_CPU_RESET_VECTOR);

	/*
	 * now the secondary core is starting up let it run its
	 * calibrations, then wait for it to finish
	 */
	spin_unlock(&boot_lock);

	return 0;
=======
}

static int tegra20_power_up_cpu(unsigned int cpu)
{
	u32 reg;

	/* Enable the CPU clock. */
	reg = readl(CLK_RST_CONTROLLER_CLK_CPU_CMPLX);
	writel(reg & ~CPU_CLOCK(cpu), CLK_RST_CONTROLLER_CLK_CPU_CMPLX);
	barrier();
	reg = readl(CLK_RST_CONTROLLER_CLK_CPU_CMPLX);

	/* Clear flow controller CSR. */
	flowctrl_write_cpu_csr(cpu, 0);

	return 0;
}

static int tegra30_power_up_cpu(unsigned int cpu)
{
	u32 reg;
	int ret, pwrgateid;
	unsigned long timeout;

	pwrgateid = tegra_cpu_powergate_id(cpu);
	if (pwrgateid < 0)
		return pwrgateid;

	/* If this is the first boot, toggle powergates directly. */
	if (!tegra_powergate_is_powered(pwrgateid)) {
		ret = tegra_powergate_power_on(pwrgateid);
=======
#include "pmc.h"

#include "common.h"
#include "iomap.h"

static cpumask_t tegra_cpu_init_mask;

static void tegra_secondary_init(unsigned int cpu)
{
	cpumask_set_cpu(cpu, &tegra_cpu_init_mask);
}


static int tegra20_boot_secondary(unsigned int cpu, struct task_struct *idle)
{
	cpu = cpu_logical_map(cpu);

	/*
	 * Force the CPU into reset. The CPU must remain in reset when
	 * the flow controller state is cleared (which will cause the
	 * flow controller to stop driving reset if the CPU has been
	 * power-gated via the flow controller). This will have no
	 * effect on first boot of the CPU since it should already be
	 * in reset.
	 */
	tegra_put_cpu_in_reset(cpu);

	/*
	 * Unhalt the CPU. If the flow controller was used to
	 * power-gate the CPU this will cause the flow controller to
	 * stop driving reset. The CPU will remain in reset because the
	 * clock and reset block is now driving reset.
	 */
	flowctrl_write_cpu_halt(cpu, 0);

	tegra_enable_cpu_clock(cpu);
	flowctrl_write_cpu_csr(cpu, 0); /* Clear flow controller CSR. */
	tegra_cpu_out_of_reset(cpu);
	return 0;
}

static int tegra30_boot_secondary(unsigned int cpu, struct task_struct *idle)
{
	int ret;
	unsigned long timeout;

	cpu = cpu_logical_map(cpu);
	tegra_put_cpu_in_reset(cpu);
	flowctrl_write_cpu_halt(cpu, 0);

	/*
	 * The power up sequence of cold boot CPU and warm boot CPU
	 * was different.
	 *
	 * For warm boot CPU that was resumed from CPU hotplug, the
	 * power will be resumed automatically after un-halting the
	 * flow controller of the warm boot CPU. We need to wait for
	 * the confirmaiton that the CPU is powered then removing
	 * the IO clamps.
	 * For cold boot CPU, do not wait. After the cold boot CPU be
	 * booted, it will run to tegra_secondary_init() and set
	 * tegra_cpu_init_mask which influences what tegra30_boot_secondary()
	 * next time around.
	 */
	if (cpumask_test_cpu(cpu, &tegra_cpu_init_mask)) {
		timeout = jiffies + msecs_to_jiffies(50);
		do {
			if (tegra_pmc_cpu_is_powered(cpu))
				goto remove_clamps;
			udelay(10);
		} while (time_before(jiffies, timeout));
	}

	/*
	 * The power status of the cold boot CPU is power gated as
	 * default. To power up the cold boot CPU, the power should
	 * be un-gated by un-toggling the power gate register
	 * manually.
	 */
	if (!tegra_pmc_cpu_is_powered(cpu)) {
		ret = tegra_pmc_cpu_power_on(cpu);
>>>>>>> refs/remotes/origin/master
		if (ret)
			return ret;

		/* Wait for the power to come up. */
<<<<<<< HEAD
		timeout = jiffies + 10*HZ;
		while (tegra_powergate_is_powered(pwrgateid)) {
=======
		timeout = jiffies + msecs_to_jiffies(100);
		while (tegra_pmc_cpu_is_powered(cpu)) {
>>>>>>> refs/remotes/origin/master
			if (time_after(jiffies, timeout))
				return -ETIMEDOUT;
			udelay(10);
		}
	}

<<<<<<< HEAD
	/* CPU partition is powered. Enable the CPU clock. */
	writel(CPU_CLOCK(cpu), CLK_RST_CONTROLLER_CLK_CPU_CMPLX_CLR);
	reg = readl(CLK_RST_CONTROLLER_CLK_CPU_CMPLX_CLR);
	udelay(10);

	/* Remove I/O clamps. */
	ret = tegra_powergate_remove_clamping(pwrgateid);
	udelay(10);

	/* Clear flow controller CSR. */
	flowctrl_write_cpu_csr(cpu, 0);

	return 0;
}

int __cpuinit boot_secondary(unsigned int cpu, struct task_struct *idle)
{
	int status;

	/*
	 * Force the CPU into reset. The CPU must remain in reset when the
	 * flow controller state is cleared (which will cause the flow
	 * controller to stop driving reset if the CPU has been power-gated
	 * via the flow controller). This will have no effect on first boot
	 * of the CPU since it should already be in reset.
	 */
	writel(CPU_RESET(cpu), CLK_RST_CONTROLLER_RST_CPU_CMPLX_SET);
	dmb();

	/*
	 * Unhalt the CPU. If the flow controller was used to power-gate the
	 * CPU this will cause the flow controller to stop driving reset.
	 * The CPU will remain in reset because the clock and reset block
	 * is now driving reset.
	 */
	flowctrl_write_cpu_halt(cpu, 0);

	switch (tegra_chip_id) {
	case TEGRA20:
		status = tegra20_power_up_cpu(cpu);
		break;
	case TEGRA30:
		status = tegra30_power_up_cpu(cpu);
		break;
	default:
		status = -EINVAL;
		break;
	}

	if (status)
		goto done;

	/* Take the CPU out of reset. */
	writel(CPU_RESET(cpu), CLK_RST_CONTROLLER_RST_CPU_CMPLX_CLR);
	wmb();
done:
	return status;
>>>>>>> refs/remotes/origin/cm-10.0
}

/*
 * Initialise the CPU possible map early - this describes the CPUs
 * which may be present or become present in the system.
 */
void __init smp_init_cpus(void)
{
	unsigned int i, ncores = scu_get_core_count(scu_base);

<<<<<<< HEAD
	if (ncores > NR_CPUS) {
		printk(KERN_ERR "Tegra: no. of cores (%u) greater than configured (%u), clipping\n",
			ncores, NR_CPUS);
		ncores = NR_CPUS;
	}

	for (i = 0; i < ncores; i++)
		cpu_set(i, cpu_possible_map);
=======
	if (ncores > nr_cpu_ids) {
		pr_warn("SMP: %u cores greater than maximum (%u), clipping\n",
			ncores, nr_cpu_ids);
		ncores = nr_cpu_ids;
	}

	for (i = 0; i < ncores; i++)
		set_cpu_possible(i, true);
>>>>>>> refs/remotes/origin/cm-10.0

	set_smp_cross_call(gic_raise_softirq);
}

void __init platform_smp_prepare_cpus(unsigned int max_cpus)
{
<<<<<<< HEAD
	int i;

	/*
	 * Initialise the present map, which describes the set of CPUs
	 * actually populated at the present time.
	 */
	for (i = 0; i < max_cpus; i++)
		set_cpu_present(i, true);

=======
	tegra_cpu_reset_handler_init();
>>>>>>> refs/remotes/origin/cm-10.0
	scu_enable(scu_base);
}
=======
remove_clamps:
	/* CPU partition is powered. Enable the CPU clock. */
	tegra_enable_cpu_clock(cpu);
	udelay(10);

	/* Remove I/O clamps. */
	ret = tegra_pmc_cpu_remove_clamping(cpu);
	if (ret)
		return ret;

	udelay(10);

	flowctrl_write_cpu_csr(cpu, 0); /* Clear flow controller CSR. */
	tegra_cpu_out_of_reset(cpu);
	return 0;
}

static int tegra114_boot_secondary(unsigned int cpu, struct task_struct *idle)
{
	int ret = 0;

	cpu = cpu_logical_map(cpu);

	if (cpumask_test_cpu(cpu, &tegra_cpu_init_mask)) {
		/*
		 * Warm boot flow
		 * The flow controller in charge of the power state and
		 * control for each CPU.
		 */
		/* set SCLK as event trigger for flow controller */
		flowctrl_write_cpu_csr(cpu, 1);
		flowctrl_write_cpu_halt(cpu,
				FLOW_CTRL_WAITEVENT | FLOW_CTRL_SCLK_RESUME);
	} else {
		/*
		 * Cold boot flow
		 * The CPU is powered up by toggling PMC directly. It will
		 * also initial power state in flow controller. After that,
		 * the CPU's power state is maintained by flow controller.
		 */
		ret = tegra_pmc_cpu_power_on(cpu);
	}

	return ret;
}

static int tegra_boot_secondary(unsigned int cpu,
					  struct task_struct *idle)
{
	if (IS_ENABLED(CONFIG_ARCH_TEGRA_2x_SOC) && tegra_chip_id == TEGRA20)
		return tegra20_boot_secondary(cpu, idle);
	if (IS_ENABLED(CONFIG_ARCH_TEGRA_3x_SOC) && tegra_chip_id == TEGRA30)
		return tegra30_boot_secondary(cpu, idle);
	if (IS_ENABLED(CONFIG_ARCH_TEGRA_114_SOC) && tegra_chip_id == TEGRA114)
		return tegra114_boot_secondary(cpu, idle);
	if (IS_ENABLED(CONFIG_ARCH_TEGRA_124_SOC) && tegra_chip_id == TEGRA124)
		return tegra114_boot_secondary(cpu, idle);

	return -EINVAL;
}

static void __init tegra_smp_prepare_cpus(unsigned int max_cpus)
{
	/* Always mark the boot CPU (CPU0) as initialized. */
	cpumask_set_cpu(0, &tegra_cpu_init_mask);

	if (scu_a9_has_base())
		scu_enable(IO_ADDRESS(scu_a9_get_base()));
}

struct smp_operations tegra_smp_ops __initdata = {
	.smp_prepare_cpus	= tegra_smp_prepare_cpus,
	.smp_secondary_init	= tegra_secondary_init,
	.smp_boot_secondary	= tegra_boot_secondary,
#ifdef CONFIG_HOTPLUG_CPU
	.cpu_kill		= tegra_cpu_kill,
	.cpu_die		= tegra_cpu_die,
#endif
};
>>>>>>> refs/remotes/origin/master
