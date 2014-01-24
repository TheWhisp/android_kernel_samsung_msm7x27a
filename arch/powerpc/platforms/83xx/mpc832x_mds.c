/*
<<<<<<< HEAD
 * Copyright (C) Freescale Semicondutor, Inc. 2006. All rights reserved.
=======
 * Copyright 2006 Freescale Semiconductor, Inc. All rights reserved.
>>>>>>> refs/remotes/origin/master
 *
 * Description:
 * MPC832xE MDS board specific routines.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/stddef.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/reboot.h>
#include <linux/pci.h>
#include <linux/kdev_t.h>
#include <linux/major.h>
#include <linux/console.h>
#include <linux/delay.h>
#include <linux/seq_file.h>
#include <linux/root_dev.h>
#include <linux/initrd.h>
#include <linux/of_platform.h>
#include <linux/of_device.h>

<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/system.h>
#include <asm/atomic.h>
=======
#include <linux/atomic.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/atomic.h>
>>>>>>> refs/remotes/origin/master
#include <asm/time.h>
#include <asm/io.h>
#include <asm/machdep.h>
#include <asm/ipic.h>
#include <asm/irq.h>
#include <asm/prom.h>
#include <asm/udbg.h>
#include <sysdev/fsl_soc.h>
#include <sysdev/fsl_pci.h>
#include <asm/qe.h>
#include <asm/qe_ic.h>

#include "mpc83xx.h"

#undef DEBUG
#ifdef DEBUG
#define DBG(fmt...) udbg_printf(fmt)
#else
#define DBG(fmt...)
#endif

/* ************************************************************************
 *
 * Setup the architecture
 *
 */
static void __init mpc832x_sys_setup_arch(void)
{
	struct device_node *np;
	u8 __iomem *bcsr_regs = NULL;

	if (ppc_md.progress)
		ppc_md.progress("mpc832x_sys_setup_arch()", 0);

	/* Map BCSR area */
	np = of_find_node_by_name(NULL, "bcsr");
	if (np) {
		struct resource res;

		of_address_to_resource(np, 0, &res);
<<<<<<< HEAD
<<<<<<< HEAD
		bcsr_regs = ioremap(res.start, res.end - res.start +1);
		of_node_put(np);
	}

#ifdef CONFIG_PCI
	for_each_compatible_node(np, "pci", "fsl,mpc8349-pci")
		mpc83xx_add_bridge(np);
#endif
=======
=======
>>>>>>> refs/remotes/origin/master
		bcsr_regs = ioremap(res.start, resource_size(&res));
		of_node_put(np);
	}

	mpc83xx_setup_pci();
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_QUICC_ENGINE
	qe_reset();

	if ((np = of_find_node_by_name(NULL, "par_io")) != NULL) {
		par_io_init(np);
		of_node_put(np);

		for (np = NULL; (np = of_find_node_by_name(np, "ucc")) != NULL;)
			par_io_of_config(np);
	}

	if ((np = of_find_compatible_node(NULL, "network", "ucc_geth"))
			!= NULL){
		/* Reset the Ethernet PHYs */
#define BCSR8_FETH_RST 0x50
		clrbits8(&bcsr_regs[8], BCSR8_FETH_RST);
		udelay(1000);
		setbits8(&bcsr_regs[8], BCSR8_FETH_RST);
		iounmap(bcsr_regs);
		of_node_put(np);
	}
#endif				/* CONFIG_QUICC_ENGINE */
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct of_device_id mpc832x_ids[] = {
	{ .type = "soc", },
	{ .compatible = "soc", },
	{ .compatible = "simple-bus", },
	{ .type = "qe", },
	{ .compatible = "fsl,qe", },
	{},
};

static int __init mpc832x_declare_of_platform_devices(void)
{
	/* Publish the QE devices */
	of_platform_bus_probe(NULL, mpc832x_ids, NULL);

	return 0;
}
machine_device_initcall(mpc832x_mds, mpc832x_declare_of_platform_devices);

static void __init mpc832x_sys_init_IRQ(void)
{
	struct device_node *np;

	np = of_find_node_by_type(NULL, "ipic");
	if (!np)
		return;

	ipic_init(np, 0);

	/* Initialize the default interrupt mapping priorities,
	 * in case the boot rom changed something on us.
	 */
	ipic_set_default_priority();
	of_node_put(np);

#ifdef CONFIG_QUICC_ENGINE
	np = of_find_compatible_node(NULL, NULL, "fsl,qe-ic");
	if (!np) {
		np = of_find_node_by_type(NULL, "qeic");
		if (!np)
			return;
	}
	qe_ic_init(np, 0, qe_ic_cascade_low_ipic, qe_ic_cascade_high_ipic);
	of_node_put(np);
#endif				/* CONFIG_QUICC_ENGINE */
}
=======
machine_device_initcall(mpc832x_mds, mpc83xx_declare_of_platform_devices);
>>>>>>> refs/remotes/origin/cm-10.0
=======
machine_device_initcall(mpc832x_mds, mpc83xx_declare_of_platform_devices);
>>>>>>> refs/remotes/origin/master

/*
 * Called very early, MMU is off, device-tree isn't unflattened
 */
static int __init mpc832x_sys_probe(void)
{
        unsigned long root = of_get_flat_dt_root();

        return of_flat_dt_is_compatible(root, "MPC832xMDS");
}

define_machine(mpc832x_mds) {
	.name 		= "MPC832x MDS",
	.probe 		= mpc832x_sys_probe,
	.setup_arch 	= mpc832x_sys_setup_arch,
<<<<<<< HEAD
<<<<<<< HEAD
	.init_IRQ 	= mpc832x_sys_init_IRQ,
=======
	.init_IRQ	= mpc83xx_ipic_and_qe_init_IRQ,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.init_IRQ	= mpc83xx_ipic_and_qe_init_IRQ,
>>>>>>> refs/remotes/origin/master
	.get_irq 	= ipic_get_irq,
	.restart 	= mpc83xx_restart,
	.time_init 	= mpc83xx_time_init,
	.calibrate_decr	= generic_calibrate_decr,
	.progress 	= udbg_progress,
};
