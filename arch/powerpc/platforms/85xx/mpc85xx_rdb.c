/*
 * MPC85xx RDB Board Setup
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright 2009 Freescale Semiconductor Inc.
=======
 * Copyright 2009,2012 Freescale Semiconductor Inc.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Copyright 2009,2012 Freescale Semiconductor Inc.
>>>>>>> refs/remotes/origin/master
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/stddef.h>
#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/kdev_t.h>
#include <linux/delay.h>
#include <linux/seq_file.h>
#include <linux/interrupt.h>
#include <linux/of_platform.h>

<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/system.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#include <asm/time.h>
#include <asm/machdep.h>
#include <asm/pci-bridge.h>
#include <mm/mmu_decl.h>
#include <asm/prom.h>
#include <asm/udbg.h>
#include <asm/mpic.h>
<<<<<<< HEAD
<<<<<<< HEAD

#include <sysdev/fsl_soc.h>
#include <sysdev/fsl_pci.h>
=======
=======
>>>>>>> refs/remotes/origin/master
#include <asm/qe.h>
#include <asm/qe_ic.h>
#include <asm/fsl_guts.h>

#include <sysdev/fsl_soc.h>
#include <sysdev/fsl_pci.h>
#include "smp.h"

#include "mpc85xx.h"
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#undef DEBUG

#ifdef DEBUG
#define DBG(fmt, args...) printk(KERN_ERR "%s: " fmt, __func__, ## args)
#else
#define DBG(fmt, args...)
#endif


void __init mpc85xx_rdb_pic_init(void)
{
	struct mpic *mpic;
<<<<<<< HEAD
<<<<<<< HEAD
	struct resource r;
	struct device_node *np;
	unsigned long root = of_get_flat_dt_root();

	np = of_find_node_by_type(NULL, "open-pic");
	if (np == NULL) {
		printk(KERN_ERR "Could not find open-pic node\n");
		return;
	}

	if (of_address_to_resource(np, 0, &r)) {
		printk(KERN_ERR "Failed to map mpic register space\n");
		of_node_put(np);
		return;
	}

	if (of_flat_dt_is_compatible(root, "fsl,85XXRDB-CAMP")) {
		mpic = mpic_alloc(np, r.start,
			MPIC_PRIMARY |
			MPIC_BIG_ENDIAN | MPIC_BROKEN_FRR_NIRQS,
			0, 256, " OpenPIC  ");
	} else {
		mpic = mpic_alloc(np, r.start,
		  MPIC_PRIMARY | MPIC_WANTS_RESET |
		  MPIC_BIG_ENDIAN | MPIC_BROKEN_FRR_NIRQS |
=======
=======
>>>>>>> refs/remotes/origin/master
	unsigned long root = of_get_flat_dt_root();

#ifdef CONFIG_QUICC_ENGINE
	struct device_node *np;
#endif

	if (of_flat_dt_is_compatible(root, "fsl,MPC85XXRDB-CAMP")) {
		mpic = mpic_alloc(NULL, 0, MPIC_NO_RESET |
			MPIC_BIG_ENDIAN |
			MPIC_SINGLE_DEST_CPU,
			0, 256, " OpenPIC  ");
	} else {
		mpic = mpic_alloc(NULL, 0,
		  MPIC_BIG_ENDIAN |
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		  MPIC_SINGLE_DEST_CPU,
		  0, 256, " OpenPIC  ");
	}

	BUG_ON(mpic == NULL);
<<<<<<< HEAD
<<<<<<< HEAD
	of_node_put(np);

	mpic_init(mpic);

=======
=======
>>>>>>> refs/remotes/origin/master
	mpic_init(mpic);

#ifdef CONFIG_QUICC_ENGINE
	np = of_find_compatible_node(NULL, NULL, "fsl,qe-ic");
	if (np) {
		qe_ic_init(np, 0, qe_ic_cascade_low_mpic,
				qe_ic_cascade_high_mpic);
		of_node_put(np);

	} else
		pr_err("%s: Could not find qe-ic node\n", __func__);
#endif

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

/*
 * Setup the architecture
 */
<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_SMP
extern void __init mpc85xx_smp_init(void);
#endif
static void __init mpc85xx_rdb_setup_arch(void)
{
#ifdef CONFIG_PCI
=======
static void __init mpc85xx_rdb_setup_arch(void)
{
#if defined(CONFIG_PCI) || defined(CONFIG_QUICC_ENGINE)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void __init mpc85xx_rdb_setup_arch(void)
{
#ifdef CONFIG_QUICC_ENGINE
>>>>>>> refs/remotes/origin/master
	struct device_node *np;
#endif

	if (ppc_md.progress)
		ppc_md.progress("mpc85xx_rdb_setup_arch()", 0);

<<<<<<< HEAD
#ifdef CONFIG_PCI
	for_each_node_by_type(np, "pci") {
		if (of_device_is_compatible(np, "fsl,mpc8548-pcie"))
			fsl_add_bridge(np, 0);
	}

#endif

<<<<<<< HEAD
#ifdef CONFIG_SMP
	mpc85xx_smp_init();
#endif

	printk(KERN_INFO "MPC85xx RDB board from Freescale Semiconductor\n");
}

static struct of_device_id __initdata mpc85xxrdb_ids[] = {
	{ .type = "soc", },
	{ .compatible = "soc", },
	{ .compatible = "simple-bus", },
	{ .compatible = "gianfar", },
	{},
};

static int __init mpc85xxrdb_publish_devices(void)
{
	return of_platform_bus_probe(NULL, mpc85xxrdb_ids, NULL);
}
machine_device_initcall(p2020_rdb, mpc85xxrdb_publish_devices);
machine_device_initcall(p1020_rdb, mpc85xxrdb_publish_devices);
=======
	mpc85xx_smp_init();
=======
	mpc85xx_smp_init();

	fsl_pci_assign_primary();
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_QUICC_ENGINE
	np = of_find_compatible_node(NULL, NULL, "fsl,qe");
	if (!np) {
		pr_err("%s: Could not find Quicc Engine node\n", __func__);
		goto qe_fail;
	}

	qe_reset();
	of_node_put(np);

	np = of_find_node_by_name(NULL, "par_io");
	if (np) {
		struct device_node *ucc;

		par_io_init(np);
		of_node_put(np);

		for_each_node_by_name(ucc, "ucc")
			par_io_of_config(ucc);

	}
#if defined(CONFIG_UCC_GETH) || defined(CONFIG_SERIAL_QE)
	if (machine_is(p1025_rdb)) {

		struct ccsr_guts __iomem *guts;

		np = of_find_node_by_name(NULL, "global-utilities");
		if (np) {
			guts = of_iomap(np, 0);
			if (!guts) {

				pr_err("mpc85xx-rdb: could not map global utilities register\n");

			} else {
			/* P1025 has pins muxed for QE and other functions. To
			* enable QE UEC mode, we need to set bit QE0 for UCC1
			* in Eth mode, QE0 and QE3 for UCC5 in Eth mode, QE9
			* and QE12 for QE MII management singals in PMUXCR
			* register.
			*/
				setbits32(&guts->pmuxcr, MPC85xx_PMUXCR_QE(0) |
						MPC85xx_PMUXCR_QE(3) |
						MPC85xx_PMUXCR_QE(9) |
						MPC85xx_PMUXCR_QE(12));
				iounmap(guts);
			}
			of_node_put(np);
		}

	}
#endif

qe_fail:
#endif	/* CONFIG_QUICC_ENGINE */

	printk(KERN_INFO "MPC85xx RDB board from Freescale Semiconductor\n");
}

<<<<<<< HEAD
machine_device_initcall(p2020_rdb, mpc85xx_common_publish_devices);
machine_device_initcall(p2020_rdb_pc, mpc85xx_common_publish_devices);
machine_device_initcall(p1020_mbg_pc, mpc85xx_common_publish_devices);
machine_device_initcall(p1020_rdb, mpc85xx_common_publish_devices);
machine_device_initcall(p1020_rdb_pc, mpc85xx_common_publish_devices);
machine_device_initcall(p1020_utm_pc, mpc85xx_common_publish_devices);
machine_device_initcall(p1021_rdb_pc, mpc85xx_common_publish_devices);
machine_device_initcall(p1025_rdb, mpc85xx_common_publish_devices);
>>>>>>> refs/remotes/origin/cm-10.0
=======
machine_arch_initcall(p2020_rdb, mpc85xx_common_publish_devices);
machine_arch_initcall(p2020_rdb_pc, mpc85xx_common_publish_devices);
machine_arch_initcall(p1020_mbg_pc, mpc85xx_common_publish_devices);
machine_arch_initcall(p1020_rdb, mpc85xx_common_publish_devices);
machine_arch_initcall(p1020_rdb_pc, mpc85xx_common_publish_devices);
machine_arch_initcall(p1020_rdb_pd, mpc85xx_common_publish_devices);
machine_arch_initcall(p1020_utm_pc, mpc85xx_common_publish_devices);
machine_arch_initcall(p1021_rdb_pc, mpc85xx_common_publish_devices);
machine_arch_initcall(p1025_rdb, mpc85xx_common_publish_devices);
machine_arch_initcall(p1024_rdb, mpc85xx_common_publish_devices);
>>>>>>> refs/remotes/origin/master

/*
 * Called very early, device-tree isn't unflattened
 */
static int __init p2020_rdb_probe(void)
{
	unsigned long root = of_get_flat_dt_root();

	if (of_flat_dt_is_compatible(root, "fsl,P2020RDB"))
		return 1;
	return 0;
}

static int __init p1020_rdb_probe(void)
{
	unsigned long root = of_get_flat_dt_root();

	if (of_flat_dt_is_compatible(root, "fsl,P1020RDB"))
		return 1;
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static int __init p1020_rdb_pc_probe(void)
{
	unsigned long root = of_get_flat_dt_root();

	return of_flat_dt_is_compatible(root, "fsl,P1020RDB-PC");
}

<<<<<<< HEAD
=======
static int __init p1020_rdb_pd_probe(void)
{
	unsigned long root = of_get_flat_dt_root();

	return of_flat_dt_is_compatible(root, "fsl,P1020RDB-PD");
}

>>>>>>> refs/remotes/origin/master
static int __init p1021_rdb_pc_probe(void)
{
	unsigned long root = of_get_flat_dt_root();

	if (of_flat_dt_is_compatible(root, "fsl,P1021RDB-PC"))
		return 1;
	return 0;
}

static int __init p2020_rdb_pc_probe(void)
{
	unsigned long root = of_get_flat_dt_root();

	if (of_flat_dt_is_compatible(root, "fsl,P2020RDB-PC"))
		return 1;
	return 0;
}

static int __init p1025_rdb_probe(void)
{
	unsigned long root = of_get_flat_dt_root();

	return of_flat_dt_is_compatible(root, "fsl,P1025RDB");
}

static int __init p1020_mbg_pc_probe(void)
{
	unsigned long root = of_get_flat_dt_root();

	return of_flat_dt_is_compatible(root, "fsl,P1020MBG-PC");
}

static int __init p1020_utm_pc_probe(void)
{
	unsigned long root = of_get_flat_dt_root();

	return of_flat_dt_is_compatible(root, "fsl,P1020UTM-PC");
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int __init p1024_rdb_probe(void)
{
	unsigned long root = of_get_flat_dt_root();

	return of_flat_dt_is_compatible(root, "fsl,P1024RDB");
}

>>>>>>> refs/remotes/origin/master
define_machine(p2020_rdb) {
	.name			= "P2020 RDB",
	.probe			= p2020_rdb_probe,
	.setup_arch		= mpc85xx_rdb_setup_arch,
	.init_IRQ		= mpc85xx_rdb_pic_init,
#ifdef CONFIG_PCI
	.pcibios_fixup_bus	= fsl_pcibios_fixup_bus,
#endif
	.get_irq		= mpic_get_irq,
	.restart		= fsl_rstcr_restart,
	.calibrate_decr		= generic_calibrate_decr,
	.progress		= udbg_progress,
};

define_machine(p1020_rdb) {
	.name			= "P1020 RDB",
	.probe			= p1020_rdb_probe,
	.setup_arch		= mpc85xx_rdb_setup_arch,
	.init_IRQ		= mpc85xx_rdb_pic_init,
#ifdef CONFIG_PCI
	.pcibios_fixup_bus	= fsl_pcibios_fixup_bus,
#endif
	.get_irq		= mpic_get_irq,
	.restart		= fsl_rstcr_restart,
	.calibrate_decr		= generic_calibrate_decr,
	.progress		= udbg_progress,
};
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master

define_machine(p1021_rdb_pc) {
	.name			= "P1021 RDB-PC",
	.probe			= p1021_rdb_pc_probe,
	.setup_arch		= mpc85xx_rdb_setup_arch,
	.init_IRQ		= mpc85xx_rdb_pic_init,
#ifdef CONFIG_PCI
	.pcibios_fixup_bus	= fsl_pcibios_fixup_bus,
#endif
	.get_irq		= mpic_get_irq,
	.restart		= fsl_rstcr_restart,
	.calibrate_decr		= generic_calibrate_decr,
	.progress		= udbg_progress,
};

define_machine(p2020_rdb_pc) {
	.name			= "P2020RDB-PC",
	.probe			= p2020_rdb_pc_probe,
	.setup_arch		= mpc85xx_rdb_setup_arch,
	.init_IRQ		= mpc85xx_rdb_pic_init,
#ifdef CONFIG_PCI
	.pcibios_fixup_bus	= fsl_pcibios_fixup_bus,
#endif
	.get_irq		= mpic_get_irq,
	.restart		= fsl_rstcr_restart,
	.calibrate_decr		= generic_calibrate_decr,
	.progress		= udbg_progress,
};

define_machine(p1025_rdb) {
	.name			= "P1025 RDB",
	.probe			= p1025_rdb_probe,
	.setup_arch		= mpc85xx_rdb_setup_arch,
	.init_IRQ		= mpc85xx_rdb_pic_init,
#ifdef CONFIG_PCI
	.pcibios_fixup_bus	= fsl_pcibios_fixup_bus,
#endif
	.get_irq		= mpic_get_irq,
	.restart		= fsl_rstcr_restart,
	.calibrate_decr		= generic_calibrate_decr,
	.progress		= udbg_progress,
};

define_machine(p1020_mbg_pc) {
	.name			= "P1020 MBG-PC",
	.probe			= p1020_mbg_pc_probe,
	.setup_arch		= mpc85xx_rdb_setup_arch,
	.init_IRQ		= mpc85xx_rdb_pic_init,
#ifdef CONFIG_PCI
	.pcibios_fixup_bus	= fsl_pcibios_fixup_bus,
#endif
	.get_irq		= mpic_get_irq,
	.restart		= fsl_rstcr_restart,
	.calibrate_decr		= generic_calibrate_decr,
	.progress		= udbg_progress,
};

define_machine(p1020_utm_pc) {
	.name			= "P1020 UTM-PC",
	.probe			= p1020_utm_pc_probe,
	.setup_arch		= mpc85xx_rdb_setup_arch,
	.init_IRQ		= mpc85xx_rdb_pic_init,
#ifdef CONFIG_PCI
	.pcibios_fixup_bus	= fsl_pcibios_fixup_bus,
#endif
	.get_irq		= mpic_get_irq,
	.restart		= fsl_rstcr_restart,
	.calibrate_decr		= generic_calibrate_decr,
	.progress		= udbg_progress,
};

define_machine(p1020_rdb_pc) {
	.name			= "P1020RDB-PC",
	.probe			= p1020_rdb_pc_probe,
	.setup_arch		= mpc85xx_rdb_setup_arch,
	.init_IRQ		= mpc85xx_rdb_pic_init,
#ifdef CONFIG_PCI
	.pcibios_fixup_bus	= fsl_pcibios_fixup_bus,
#endif
	.get_irq		= mpic_get_irq,
	.restart		= fsl_rstcr_restart,
	.calibrate_decr		= generic_calibrate_decr,
	.progress		= udbg_progress,
};
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======

define_machine(p1020_rdb_pd) {
	.name			= "P1020RDB-PD",
	.probe			= p1020_rdb_pd_probe,
	.setup_arch		= mpc85xx_rdb_setup_arch,
	.init_IRQ		= mpc85xx_rdb_pic_init,
#ifdef CONFIG_PCI
	.pcibios_fixup_bus	= fsl_pcibios_fixup_bus,
#endif
	.get_irq		= mpic_get_irq,
	.restart		= fsl_rstcr_restart,
	.calibrate_decr		= generic_calibrate_decr,
	.progress		= udbg_progress,
};

define_machine(p1024_rdb) {
	.name			= "P1024 RDB",
	.probe			= p1024_rdb_probe,
	.setup_arch		= mpc85xx_rdb_setup_arch,
	.init_IRQ		= mpc85xx_rdb_pic_init,
#ifdef CONFIG_PCI
	.pcibios_fixup_bus	= fsl_pcibios_fixup_bus,
#endif
	.get_irq		= mpic_get_irq,
	.restart		= fsl_rstcr_restart,
	.calibrate_decr		= generic_calibrate_decr,
	.progress		= udbg_progress,
};
>>>>>>> refs/remotes/origin/master
