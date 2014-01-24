/*
 * OMAP4 SMP cpu-hotplug support
 *
 * Copyright (C) 2010 Texas Instruments, Inc.
 * Author:
 *      Santosh Shilimkar <santosh.shilimkar@ti.com>
 *
 * Platform file needed for the OMAP4 SMP. This file is based on arm
 * realview smp platform.
 * Copyright (c) 2002 ARM Limited.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/smp.h>
<<<<<<< HEAD

#include <asm/cacheflush.h>
<<<<<<< HEAD
#include <mach/omap4-common.h>
=======

#include "common.h"

#include "powerdomain.h"
>>>>>>> refs/remotes/origin/cm-10.0

int platform_cpu_kill(unsigned int cpu)
{
	return 1;
}
=======
#include <linux/io.h>

#include "omap-wakeupgen.h"
#include "common.h"
#include "powerdomain.h"
>>>>>>> refs/remotes/origin/master

/*
 * platform-specific code to shutdown a CPU
 * Called with IRQs disabled
 */
<<<<<<< HEAD
<<<<<<< HEAD
void platform_cpu_die(unsigned int cpu)
{
=======
void __ref platform_cpu_die(unsigned int cpu)
{
	unsigned int this_cpu;

>>>>>>> refs/remotes/origin/cm-10.0
	flush_cache_all();
	dsb();
=======
void __ref omap4_cpu_die(unsigned int cpu)
{
	unsigned int boot_cpu = 0;
	void __iomem *base = omap_get_wakeupgen_base();
>>>>>>> refs/remotes/origin/master

	/*
	 * we're ready for shutdown now, so do it
	 */
<<<<<<< HEAD
	if (omap_modify_auxcoreboot0(0x0, 0x200) != 0x0)
<<<<<<< HEAD
		printk(KERN_CRIT "Secure clear status failed\n");

	for (;;) {
		/*
		 * Execute WFI
		 */
		do_wfi();

		if (omap_read_auxcoreboot0() == cpu) {
=======
		pr_err("Secure clear status failed\n");
=======
	if (omap_secure_apis_support()) {
		if (omap_modify_auxcoreboot0(0x0, 0x200) != 0x0)
			pr_err("Secure clear status failed\n");
	} else {
		__raw_writel(0, base + OMAP_AUX_CORE_BOOT_0);
	}

>>>>>>> refs/remotes/origin/master

	for (;;) {
		/*
		 * Enter into low power state
		 */
		omap4_hotplug_cpu(cpu, PWRDM_POWER_OFF);
<<<<<<< HEAD
		this_cpu = smp_processor_id();
		if (omap_read_auxcoreboot0() == this_cpu) {
>>>>>>> refs/remotes/origin/cm-10.0
=======

		if (omap_secure_apis_support())
			boot_cpu = omap_read_auxcoreboot0();
		else
			boot_cpu =
				__raw_readl(base + OMAP_AUX_CORE_BOOT_0) >> 5;

		if (boot_cpu == smp_processor_id()) {
>>>>>>> refs/remotes/origin/master
			/*
			 * OK, proper wakeup, we're done
			 */
			break;
		}
		pr_debug("CPU%u: spurious wakeup call\n", cpu);
	}
}
<<<<<<< HEAD

int platform_cpu_disable(unsigned int cpu)
{
	/*
	 * we don't allow CPU 0 to be shutdown (it is still too special
	 * e.g. clock tick interrupts)
	 */
	return cpu == 0 ? -EPERM : 0;
}
=======
>>>>>>> refs/remotes/origin/master
