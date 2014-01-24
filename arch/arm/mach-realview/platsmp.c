/*
 *  linux/arch/arm/mach-realview/platsmp.c
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
#include <linux/smp.h>
#include <linux/io.h>

#include <mach/hardware.h>
<<<<<<< HEAD
#include <asm/hardware/gic.h>
#include <asm/mach-types.h>
#include <asm/smp_scu.h>
<<<<<<< HEAD
#include <asm/unified.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <asm/mach-types.h>
#include <asm/smp_scu.h>
>>>>>>> refs/remotes/origin/master

#include <mach/board-eb.h>
#include <mach/board-pb11mp.h>
#include <mach/board-pbx.h>

<<<<<<< HEAD
#include "core.h"

extern void versatile_secondary_startup(void);
=======
#include <plat/platsmp.h>

#include "core.h"
>>>>>>> refs/remotes/origin/master

static void __iomem *scu_base_addr(void)
{
	if (machine_is_realview_eb_mp())
		return __io_address(REALVIEW_EB11MP_SCU_BASE);
	else if (machine_is_realview_pb11mp())
		return __io_address(REALVIEW_TC11MP_SCU_BASE);
	else if (machine_is_realview_pbx() &&
		 (core_tile_pbx11mp() || core_tile_pbxa9mp()))
		return __io_address(REALVIEW_PBX_TILE_SCU_BASE);
	else
		return (void __iomem *)0;
}

/*
 * Initialise the CPU possible map early - this describes the CPUs
 * which may be present or become present in the system.
 */
<<<<<<< HEAD
void __init smp_init_cpus(void)
=======
static void __init realview_smp_init_cpus(void)
>>>>>>> refs/remotes/origin/master
{
	void __iomem *scu_base = scu_base_addr();
	unsigned int i, ncores;

	ncores = scu_base ? scu_get_core_count(scu_base) : 1;

	/* sanity check */
<<<<<<< HEAD
<<<<<<< HEAD
	if (ncores > NR_CPUS) {
		printk(KERN_WARNING
		       "Realview: no. of cores (%d) greater than configured "
		       "maximum of %d - clipping\n",
		       ncores, NR_CPUS);
		ncores = NR_CPUS;
=======
=======
>>>>>>> refs/remotes/origin/master
	if (ncores > nr_cpu_ids) {
		pr_warn("SMP: %u cores greater than maximum (%u), clipping\n",
			ncores, nr_cpu_ids);
		ncores = nr_cpu_ids;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	for (i = 0; i < ncores; i++)
		set_cpu_possible(i, true);
<<<<<<< HEAD

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
=======
}

static void __init realview_smp_prepare_cpus(unsigned int max_cpus)
{
>>>>>>> refs/remotes/origin/master

	scu_enable(scu_base_addr());

	/*
	 * Write the address of secondary startup into the
	 * system-wide flags register. The BootMonitor waits
	 * until it receives a soft interrupt, and then the
	 * secondary CPU branches to this address.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	__raw_writel(BSYM(virt_to_phys(versatile_secondary_startup)),
=======
	__raw_writel(virt_to_phys(versatile_secondary_startup),
>>>>>>> refs/remotes/origin/cm-10.0
		     __io_address(REALVIEW_SYS_FLAGSSET));
}
=======
	__raw_writel(virt_to_phys(versatile_secondary_startup),
		     __io_address(REALVIEW_SYS_FLAGSSET));
}

struct smp_operations realview_smp_ops __initdata = {
	.smp_init_cpus		= realview_smp_init_cpus,
	.smp_prepare_cpus	= realview_smp_prepare_cpus,
	.smp_secondary_init	= versatile_secondary_init,
	.smp_boot_secondary	= versatile_boot_secondary,
#ifdef CONFIG_HOTPLUG_CPU
	.cpu_die		= realview_cpu_die,
#endif
};
>>>>>>> refs/remotes/origin/master
