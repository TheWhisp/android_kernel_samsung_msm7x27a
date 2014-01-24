/*
 * OMAP4 SMP source file. It contains platform specific fucntions
 * needed for the linux smp kernel.
 *
 * Copyright (C) 2009 Texas Instruments, Inc.
 *
 * Author:
 *      Santosh Shilimkar <santosh.shilimkar@ti.com>
 *
 * Platform file needed for the OMAP4 SMP. This file is based on arm
 * realview smp platform.
 * * Copyright (c) 2002 ARM Limited.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/init.h>
#include <linux/device.h>
#include <linux/smp.h>
#include <linux/io.h>

#include <asm/cacheflush.h>
#include <asm/hardware/gic.h>
#include <asm/smp_scu.h>
<<<<<<< HEAD
#include <mach/hardware.h>
#include <mach/omap4-common.h>
=======

#include <mach/hardware.h>
#include <mach/omap-secure.h>

#include "iomap.h"
#include "common.h"
#include "clockdomain.h"
>>>>>>> refs/remotes/origin/cm-10.0

/* SCU base address */
static void __iomem *scu_base;

static DEFINE_SPINLOCK(boot_lock);

<<<<<<< HEAD
void __cpuinit platform_secondary_init(unsigned int cpu)
{
	/*
=======
void __iomem *omap4_get_scu_base(void)
{
	return scu_base;
}

void __cpuinit platform_secondary_init(unsigned int cpu)
{
	/*
	 * Configure ACTRL and enable NS SMP bit access on CPU1 on HS device.
	 * OMAP44XX EMU/HS devices - CPU0 SMP bit access is enabled in PPA
	 * init and for CPU1, a secure PPA API provided. CPU0 must be ON
	 * while executing NS_SMP API on CPU1 and PPA version must be 1.4.0+.
	 * OMAP443X GP devices- SMP bit isn't accessible.
	 * OMAP446X GP devices - SMP bit access is enabled on both CPUs.
	 */
	if (cpu_is_omap443x() && (omap_type() != OMAP2_DEVICE_TYPE_GP))
		omap_secure_dispatcher(OMAP4_PPA_CPU_ACTRL_SMP_INDEX,
							4, 0, 0, 0, 0, 0);

	/*
>>>>>>> refs/remotes/origin/cm-10.0
	 * If any interrupts are already enabled for the primary
	 * core (e.g. timer irq), then they will not have been enabled
	 * for us: do so
	 */
	gic_secondary_init(0);

	/*
	 * Synchronise with the boot thread.
	 */
	spin_lock(&boot_lock);
	spin_unlock(&boot_lock);
}

int __cpuinit boot_secondary(unsigned int cpu, struct task_struct *idle)
{
<<<<<<< HEAD
=======
	static struct clockdomain *cpu1_clkdm;
	static bool booted;
>>>>>>> refs/remotes/origin/cm-10.0
	/*
	 * Set synchronisation state between this boot processor
	 * and the secondary one
	 */
	spin_lock(&boot_lock);

	/*
	 * Update the AuxCoreBoot0 with boot state for secondary core.
	 * omap_secondary_startup() routine will hold the secondary core till
	 * the AuxCoreBoot1 register is updated with cpu state
	 * A barrier is added to ensure that write buffer is drained
	 */
	omap_modify_auxcoreboot0(0x200, 0xfffffdff);
	flush_cache_all();
	smp_wmb();
<<<<<<< HEAD
=======

	if (!cpu1_clkdm)
		cpu1_clkdm = clkdm_lookup("mpu1_clkdm");

	/*
	 * The SGI(Software Generated Interrupts) are not wakeup capable
	 * from low power states. This is known limitation on OMAP4 and
	 * needs to be worked around by using software forced clockdomain
	 * wake-up. To wakeup CPU1, CPU0 forces the CPU1 clockdomain to
	 * software force wakeup. The clockdomain is then put back to
	 * hardware supervised mode.
	 * More details can be found in OMAP4430 TRM - Version J
	 * Section :
	 *	4.3.4.2 Power States of CPU0 and CPU1
	 */
	if (booted) {
		clkdm_wakeup(cpu1_clkdm);
		clkdm_allow_idle(cpu1_clkdm);
	} else {
		dsb_sev();
		booted = true;
	}

>>>>>>> refs/remotes/origin/cm-10.0
	gic_raise_softirq(cpumask_of(cpu), 1);

	/*
	 * Now the secondary core is starting up let it run its
	 * calibrations, then wait for it to finish
	 */
	spin_unlock(&boot_lock);

	return 0;
}

static void __init wakeup_secondary(void)
{
	/*
	 * Write the address of secondary startup routine into the
	 * AuxCoreBoot1 where ROM code will jump and start executing
	 * on secondary core once out of WFE
	 * A barrier is added to ensure that write buffer is drained
	 */
	omap_auxcoreboot_addr(virt_to_phys(omap_secondary_startup));
	smp_wmb();

	/*
	 * Send a 'sev' to wake the secondary core from WFE.
	 * Drain the outstanding writes to memory
	 */
	dsb_sev();
	mb();
}

/*
 * Initialise the CPU possible map early - this describes the CPUs
 * which may be present or become present in the system.
 */
void __init smp_init_cpus(void)
{
	unsigned int i, ncores;

<<<<<<< HEAD
	/* Never released */
	scu_base = ioremap(OMAP44XX_SCU_BASE, SZ_256);
=======
	/*
	 * Currently we can't call ioremap here because
	 * SoC detection won't work until after init_early.
	 */
	scu_base =  OMAP2_L4_IO_ADDRESS(OMAP44XX_SCU_BASE);
>>>>>>> refs/remotes/origin/cm-10.0
	BUG_ON(!scu_base);

	ncores = scu_get_core_count(scu_base);

	/* sanity check */
<<<<<<< HEAD
	if (ncores > NR_CPUS) {
		printk(KERN_WARNING
		       "OMAP4: no. of cores (%d) greater than configured "
		       "maximum of %d - clipping\n",
		       ncores, NR_CPUS);
		ncores = NR_CPUS;
=======
	if (ncores > nr_cpu_ids) {
		pr_warn("SMP: %u cores greater than maximum (%u), clipping\n",
			ncores, nr_cpu_ids);
		ncores = nr_cpu_ids;
>>>>>>> refs/remotes/origin/cm-10.0
	}

	for (i = 0; i < ncores; i++)
		set_cpu_possible(i, true);

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
>>>>>>> refs/remotes/origin/cm-10.0

	/*
	 * Initialise the SCU and wake up the secondary core using
	 * wakeup_secondary().
	 */
	scu_enable(scu_base);
	wakeup_secondary();
}
