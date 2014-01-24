/*
 * Copyright 2008-2011, IBM Corporation
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/delay.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/of.h>
#include <linux/smp.h>
<<<<<<< HEAD
<<<<<<< HEAD

#include <asm/machdep.h>
#include <asm/system.h>
#include <asm/time.h>
=======
#include <linux/time.h>

#include <asm/machdep.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/time.h>
#include <linux/of_fdt.h>

#include <asm/machdep.h>
>>>>>>> refs/remotes/origin/master
#include <asm/udbg.h>

#include "ics.h"
#include "wsp.h"


static void psr2_spin(void)
{
	hard_irq_disable();
<<<<<<< HEAD
<<<<<<< HEAD
	for (;;) ;
=======
	for (;;)
		continue;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	for (;;)
		continue;
>>>>>>> refs/remotes/origin/master
}

static void psr2_restart(char *cmd)
{
	psr2_spin();
}

<<<<<<< HEAD
<<<<<<< HEAD
static int psr2_probe_devices(void)
{
	struct device_node *np;

	/* Our RTC is a ds1500. It seems to be programatically compatible
	 * with the ds1511 for which we have a driver so let's use that
	 */
	np = of_find_compatible_node(NULL, NULL, "dallas,ds1500");
	if (np != NULL) {
		struct resource res;
		if (of_address_to_resource(np, 0, &res) == 0)
			platform_device_register_simple("ds1511", 0, &res, 1);
	}
	return 0;
}
machine_arch_initcall(psr2_md, psr2_probe_devices);

static void __init psr2_setup_arch(void)
{
	/* init to some ~sane value until calibrate_delay() runs */
	loops_per_jiffy = 50000000;

	scom_init_wsp();

	/* Setup SMP callback */
#ifdef CONFIG_SMP
	a2_setup_smp();
#endif
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static int __init psr2_probe(void)
{
	unsigned long root = of_get_flat_dt_root();

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	if (of_flat_dt_is_compatible(root, "ibm,wsp-chroma")) {
		/* chroma systems also claim they are psr2s */
		return 0;
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (!of_flat_dt_is_compatible(root, "ibm,psr2"))
		return 0;

	return 1;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void __init psr2_init_irq(void)
{
	wsp_init_irq();
	opb_pic_init();
}

define_machine(psr2_md) {
	.name			= "PSR2 A2",
	.probe			= psr2_probe,
	.setup_arch		= psr2_setup_arch,
=======
=======
>>>>>>> refs/remotes/origin/master
define_machine(psr2_md) {
	.name			= "PSR2 A2",
	.probe			= psr2_probe,
	.setup_arch		= wsp_setup_arch,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	.restart		= psr2_restart,
	.power_off		= psr2_spin,
	.halt			= psr2_spin,
	.calibrate_decr		= generic_calibrate_decr,
<<<<<<< HEAD
<<<<<<< HEAD
	.init_IRQ		= psr2_init_irq,
	.progress		= udbg_progress,
	.power_save		= book3e_idle,
};
=======
=======
>>>>>>> refs/remotes/origin/master
	.init_IRQ		= wsp_setup_irq,
	.progress		= udbg_progress,
	.power_save		= book3e_idle,
};

machine_arch_initcall(psr2_md, wsp_probe_devices);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
