/*
 * MPC85xx setup and early boot code plus other random bits.
 *
 * Maintained by Kumar Gala (see MAINTAINERS for contact information)
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright 2005 Freescale Semiconductor Inc.
=======
 * Copyright 2005, 2011-2012 Freescale Semiconductor Inc.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Copyright 2005, 2011-2012 Freescale Semiconductor Inc.
>>>>>>> refs/remotes/origin/master
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
#include <linux/initrd.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/module.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#include <linux/interrupt.h>
#include <linux/fsl_devices.h>
#include <linux/of_platform.h>

<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/system.h>
#include <asm/pgtable.h>
#include <asm/page.h>
#include <asm/atomic.h>
=======
#include <asm/pgtable.h>
#include <asm/page.h>
#include <linux/atomic.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <asm/pgtable.h>
#include <asm/page.h>
#include <linux/atomic.h>
>>>>>>> refs/remotes/origin/master
#include <asm/time.h>
#include <asm/io.h>
#include <asm/machdep.h>
#include <asm/ipic.h>
#include <asm/pci-bridge.h>
#include <asm/irq.h>
#include <mm/mmu_decl.h>
#include <asm/prom.h>
#include <asm/udbg.h>
#include <asm/mpic.h>
#include <asm/i8259.h>

#include <sysdev/fsl_soc.h>
#include <sysdev/fsl_pci.h>

<<<<<<< HEAD
<<<<<<< HEAD
/* CADMUS info */
/* xxx - galak, move into device tree */
#define CADMUS_BASE (0xf8004000)
#define CADMUS_SIZE (256)
#define CM_VER	(0)
#define CM_CSR	(1)
#define CM_RST	(2)


static int cds_pci_slot = 2;
static volatile u8 *cadmus;
=======
=======
>>>>>>> refs/remotes/origin/master
#include "mpc85xx.h"

/*
 * The CDS board contains an FPGA/CPLD called "Cadmus", which collects
 * various logic and performs system control functions.
 * Here is the FPGA/CPLD register map.
 */
struct cadmus_reg {
	u8 cm_ver;		/* Board version */
	u8 cm_csr;		/* General control/status */
	u8 cm_rst;		/* Reset control */
	u8 cm_hsclk;	/* High speed clock */
	u8 cm_hsxclk;	/* High speed clock extended */
	u8 cm_led;		/* LED data */
	u8 cm_pci;		/* PCI control/status */
	u8 cm_dma;		/* DMA control */
	u8 res[248];	/* Total 256 bytes */
};

static struct cadmus_reg *cadmus;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_PCI

#define ARCADIA_HOST_BRIDGE_IDSEL	17
#define ARCADIA_2ND_BRIDGE_IDSEL	3

static int mpc85xx_exclude_device(struct pci_controller *hose,
				  u_char bus, u_char devfn)
{
	/* We explicitly do not go past the Tundra 320 Bridge */
	if ((bus == 1) && (PCI_SLOT(devfn) == ARCADIA_2ND_BRIDGE_IDSEL))
		return PCIBIOS_DEVICE_NOT_FOUND;
	if ((bus == 0) && (PCI_SLOT(devfn) == ARCADIA_2ND_BRIDGE_IDSEL))
		return PCIBIOS_DEVICE_NOT_FOUND;
	else
		return PCIBIOS_SUCCESSFUL;
}

static void mpc85xx_cds_restart(char *cmd)
{
	struct pci_dev *dev;
	u_char tmp;

	if ((dev = pci_get_device(PCI_VENDOR_ID_VIA, PCI_DEVICE_ID_VIA_82C686,
					NULL))) {

		/* Use the VIA Super Southbridge to force a PCI reset */
		pci_read_config_byte(dev, 0x47, &tmp);
		pci_write_config_byte(dev, 0x47, tmp | 1);

		/* Flush the outbound PCI write queues */
		pci_read_config_byte(dev, 0x47, &tmp);

		/*
		 *  At this point, the harware reset should have triggered.
		 *  However, if it doesn't work for some mysterious reason,
		 *  just fall through to the default reset below.
		 */

		pci_dev_put(dev);
	}

	/*
	 *  If we can't find the VIA chip (maybe the P2P bridge is disabled)
	 *  or the VIA chip reset didn't work, just use the default reset.
	 */
	fsl_rstcr_restart(NULL);
}

static void __init mpc85xx_cds_pci_irq_fixup(struct pci_dev *dev)
{
	u_char c;
	if (dev->vendor == PCI_VENDOR_ID_VIA) {
		switch (dev->device) {
		case PCI_DEVICE_ID_VIA_82C586_1:
			/*
			 * U-Boot does not set the enable bits
			 * for the IDE device. Force them on here.
			 */
			pci_read_config_byte(dev, 0x40, &c);
			c |= 0x03; /* IDE: Chip Enable Bits */
			pci_write_config_byte(dev, 0x40, c);

			/*
			 * Since only primary interface works, force the
			 * IDE function to standard primary IDE interrupt
			 * w/ 8259 offset
			 */
			dev->irq = 14;
			pci_write_config_byte(dev, PCI_INTERRUPT_LINE, dev->irq);
			break;
		/*
		 * Force legacy USB interrupt routing
		 */
		case PCI_DEVICE_ID_VIA_82C586_2:
		/* There are two USB controllers.
		 * Identify them by functon number
		 */
			if (PCI_FUNC(dev->devfn) == 3)
				dev->irq = 11;
			else
				dev->irq = 10;
			pci_write_config_byte(dev, PCI_INTERRUPT_LINE, dev->irq);
		default:
			break;
		}
	}
}

<<<<<<< HEAD
static void __devinit skip_fake_bridge(struct pci_dev *dev)
=======
static void skip_fake_bridge(struct pci_dev *dev)
>>>>>>> refs/remotes/origin/master
{
	/* Make it an error to skip the fake bridge
	 * in pci_setup_device() in probe.c */
	dev->hdr_type = 0x7f;
}
DECLARE_PCI_FIXUP_EARLY(0x1957, 0x3fff, skip_fake_bridge);
DECLARE_PCI_FIXUP_EARLY(0x3fff, 0x1957, skip_fake_bridge);
DECLARE_PCI_FIXUP_EARLY(0xff3f, 0x5719, skip_fake_bridge);

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
#define PCI_DEVICE_ID_IDT_TSI310	0x01a7

/*
 * Fix Tsi310 PCI-X bridge resource.
 * Force the bridge to open a window from 0x0000-0x1fff in PCI I/O space.
 * This allows legacy I/O(i8259, etc) on the VIA southbridge to be accessed.
 */
void mpc85xx_cds_fixup_bus(struct pci_bus *bus)
{
	struct pci_dev *dev = bus->self;
	struct resource *res = bus->resource[0];

	if (dev != NULL &&
	    dev->vendor == PCI_VENDOR_ID_IBM &&
	    dev->device == PCI_DEVICE_ID_IDT_TSI310) {
		if (res) {
			res->start = 0;
			res->end   = 0x1fff;
			res->flags = IORESOURCE_IO;
			pr_info("mpc85xx_cds: PCI bridge resource fixup applied\n");
			pr_info("mpc85xx_cds: %pR\n", res);
		}
	}

	fsl_pcibios_fixup_bus(bus);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_PPC_I8259
static void mpc85xx_8259_cascade_handler(unsigned int irq,
					 struct irq_desc *desc)
{
	unsigned int cascade_irq = i8259_irq();

	if (cascade_irq != NO_IRQ)
		/* handle an interrupt from the 8259 */
		generic_handle_irq(cascade_irq);

	/* check for any interrupts from the shared IRQ line */
	handle_fasteoi_irq(irq, desc);
}

static irqreturn_t mpc85xx_8259_cascade_action(int irq, void *dev_id)
{
	return IRQ_HANDLED;
}

static struct irqaction mpc85xxcds_8259_irqaction = {
	.handler = mpc85xx_8259_cascade_action,
<<<<<<< HEAD
<<<<<<< HEAD
	.flags = IRQF_SHARED,
=======
	.flags = IRQF_SHARED | IRQF_NO_THREAD,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.flags = IRQF_SHARED | IRQF_NO_THREAD,
>>>>>>> refs/remotes/origin/master
	.name = "8259 cascade",
};
#endif /* PPC_I8259 */
#endif /* CONFIG_PCI */

static void __init mpc85xx_cds_pic_init(void)
{
	struct mpic *mpic;
<<<<<<< HEAD
<<<<<<< HEAD
	struct resource r;
	struct device_node *np = NULL;

	np = of_find_node_by_type(np, "open-pic");

	if (np == NULL) {
		printk(KERN_ERR "Could not find open-pic node\n");
		return;
	}

	if (of_address_to_resource(np, 0, &r)) {
		printk(KERN_ERR "Failed to map mpic register space\n");
		of_node_put(np);
		return;
	}

	mpic = mpic_alloc(np, r.start,
			MPIC_PRIMARY | MPIC_WANTS_RESET | MPIC_BIG_ENDIAN,
			0, 256, " OpenPIC  ");
	BUG_ON(mpic == NULL);

	/* Return the mpic node */
	of_node_put(np);

=======
	mpic = mpic_alloc(NULL, 0, MPIC_BIG_ENDIAN,
			0, 256, " OpenPIC  ");
	BUG_ON(mpic == NULL);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	mpic = mpic_alloc(NULL, 0, MPIC_BIG_ENDIAN,
			0, 256, " OpenPIC  ");
	BUG_ON(mpic == NULL);
>>>>>>> refs/remotes/origin/master
	mpic_init(mpic);
}

#if defined(CONFIG_PPC_I8259) && defined(CONFIG_PCI)
static int mpc85xx_cds_8259_attach(void)
{
	int ret;
	struct device_node *np = NULL;
	struct device_node *cascade_node = NULL;
	int cascade_irq;

	/* Initialize the i8259 controller */
	for_each_node_by_type(np, "interrupt-controller")
		if (of_device_is_compatible(np, "chrp,iic")) {
			cascade_node = np;
			break;
		}

	if (cascade_node == NULL) {
		printk(KERN_DEBUG "Could not find i8259 PIC\n");
		return -ENODEV;
	}

	cascade_irq = irq_of_parse_and_map(cascade_node, 0);
	if (cascade_irq == NO_IRQ) {
		printk(KERN_ERR "Failed to map cascade interrupt\n");
		return -ENXIO;
	}

	i8259_init(cascade_node, 0);
	of_node_put(cascade_node);

	/*
	 *  Hook the interrupt to make sure desc->action is never NULL.
	 *  This is required to ensure that the interrupt does not get
	 *  disabled when the last user of the shared IRQ line frees their
	 *  interrupt.
	 */
	if ((ret = setup_irq(cascade_irq, &mpc85xxcds_8259_irqaction))) {
		printk(KERN_ERR "Failed to setup cascade interrupt\n");
		return ret;
	}

	/* Success. Connect our low-level cascade handler. */
	irq_set_handler(cascade_irq, mpc85xx_8259_cascade_handler);

	return 0;
}
machine_device_initcall(mpc85xx_cds, mpc85xx_cds_8259_attach);

#endif /* CONFIG_PPC_I8259 */

<<<<<<< HEAD
=======
static void mpc85xx_cds_pci_assign_primary(void)
{
#ifdef CONFIG_PCI
	struct device_node *np;

	if (fsl_pci_primary)
		return;

	/*
	 * MPC85xx_CDS has ISA bridge but unfortunately there is no
	 * isa node in device tree. We now looking for i8259 node as
	 * a workaround for such a broken device tree. This routine
	 * is for complying to all device trees.
	 */
	np = of_find_node_by_name(NULL, "i8259");
	while ((fsl_pci_primary = of_get_parent(np))) {
		of_node_put(np);
		np = fsl_pci_primary;

		if ((of_device_is_compatible(np, "fsl,mpc8540-pci") ||
		    of_device_is_compatible(np, "fsl,mpc8548-pcie")) &&
		    of_device_is_available(np))
			return;
	}
#endif
}

>>>>>>> refs/remotes/origin/master
/*
 * Setup the architecture
 */
static void __init mpc85xx_cds_setup_arch(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_PCI
	struct device_node *np;
#endif
=======
	struct device_node *np;
	int cds_pci_slot;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct device_node *np;
	int cds_pci_slot;
>>>>>>> refs/remotes/origin/master

	if (ppc_md.progress)
		ppc_md.progress("mpc85xx_cds_setup_arch()", 0);

<<<<<<< HEAD
<<<<<<< HEAD
	cadmus = ioremap(CADMUS_BASE, CADMUS_SIZE);
	cds_pci_slot = ((cadmus[CM_CSR] >> 6) & 0x3) + 1;

	if (ppc_md.progress) {
		char buf[40];
		snprintf(buf, 40, "CDS Version = 0x%x in slot %d\n",
				cadmus[CM_VER], cds_pci_slot);
=======
=======
>>>>>>> refs/remotes/origin/master
	np = of_find_compatible_node(NULL, NULL, "fsl,mpc8548cds-fpga");
	if (!np) {
		pr_err("Could not find FPGA node.\n");
		return;
	}

	cadmus = of_iomap(np, 0);
	of_node_put(np);
	if (!cadmus) {
		pr_err("Fail to map FPGA area.\n");
		return;
	}

	if (ppc_md.progress) {
		char buf[40];
		cds_pci_slot = ((in_8(&cadmus->cm_csr) >> 6) & 0x3) + 1;
		snprintf(buf, 40, "CDS Version = 0x%x in slot %d\n",
				in_8(&cadmus->cm_ver), cds_pci_slot);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		ppc_md.progress(buf, 0);
	}

#ifdef CONFIG_PCI
<<<<<<< HEAD
	for_each_node_by_type(np, "pci") {
		if (of_device_is_compatible(np, "fsl,mpc8540-pci") ||
		    of_device_is_compatible(np, "fsl,mpc8548-pcie")) {
			struct resource rsrc;
			of_address_to_resource(np, 0, &rsrc);
			if ((rsrc.start & 0xfffff) == 0x8000)
				fsl_add_bridge(np, 1);
			else
				fsl_add_bridge(np, 0);
		}
	}

	ppc_md.pci_irq_fixup = mpc85xx_cds_pci_irq_fixup;
	ppc_md.pci_exclude_device = mpc85xx_exclude_device;
#endif
=======
	ppc_md.pci_irq_fixup = mpc85xx_cds_pci_irq_fixup;
	ppc_md.pci_exclude_device = mpc85xx_exclude_device;
#endif

	mpc85xx_cds_pci_assign_primary();
	fsl_pci_assign_primary();
>>>>>>> refs/remotes/origin/master
}

static void mpc85xx_cds_show_cpuinfo(struct seq_file *m)
{
	uint pvid, svid, phid1;

	pvid = mfspr(SPRN_PVR);
	svid = mfspr(SPRN_SVR);

	seq_printf(m, "Vendor\t\t: Freescale Semiconductor\n");
<<<<<<< HEAD
<<<<<<< HEAD
	seq_printf(m, "Machine\t\t: MPC85xx CDS (0x%x)\n", cadmus[CM_VER]);
=======
	seq_printf(m, "Machine\t\t: MPC85xx CDS (0x%x)\n",
			in_8(&cadmus->cm_ver));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	seq_printf(m, "Machine\t\t: MPC85xx CDS (0x%x)\n",
			in_8(&cadmus->cm_ver));
>>>>>>> refs/remotes/origin/master
	seq_printf(m, "PVR\t\t: 0x%x\n", pvid);
	seq_printf(m, "SVR\t\t: 0x%x\n", svid);

	/* Display cpu Pll setting */
	phid1 = mfspr(SPRN_HID1);
	seq_printf(m, "PLL setting\t: 0x%x\n", ((phid1 >> 24) & 0x3f));
}


/*
 * Called very early, device-tree isn't unflattened
 */
static int __init mpc85xx_cds_probe(void)
{
        unsigned long root = of_get_flat_dt_root();

        return of_flat_dt_is_compatible(root, "MPC85xxCDS");
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct of_device_id __initdata of_bus_ids[] = {
	{ .type = "soc", },
	{ .compatible = "soc", },
	{ .compatible = "simple-bus", },
	{ .compatible = "gianfar", },
	{},
};

static int __init declare_of_platform_devices(void)
{
	return of_platform_bus_probe(NULL, of_bus_ids, NULL);
}
machine_device_initcall(mpc85xx_cds, declare_of_platform_devices);
=======
machine_device_initcall(mpc85xx_cds, mpc85xx_common_publish_devices);
>>>>>>> refs/remotes/origin/cm-10.0
=======
machine_arch_initcall(mpc85xx_cds, mpc85xx_common_publish_devices);
>>>>>>> refs/remotes/origin/master

define_machine(mpc85xx_cds) {
	.name		= "MPC85xx CDS",
	.probe		= mpc85xx_cds_probe,
	.setup_arch	= mpc85xx_cds_setup_arch,
	.init_IRQ	= mpc85xx_cds_pic_init,
	.show_cpuinfo	= mpc85xx_cds_show_cpuinfo,
	.get_irq	= mpic_get_irq,
#ifdef CONFIG_PCI
	.restart	= mpc85xx_cds_restart,
<<<<<<< HEAD
<<<<<<< HEAD
	.pcibios_fixup_bus	= fsl_pcibios_fixup_bus,
=======
	.pcibios_fixup_bus	= mpc85xx_cds_fixup_bus,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.pcibios_fixup_bus	= mpc85xx_cds_fixup_bus,
>>>>>>> refs/remotes/origin/master
#else
	.restart	= fsl_rstcr_restart,
#endif
	.calibrate_decr = generic_calibrate_decr,
	.progress	= udbg_progress,
};
