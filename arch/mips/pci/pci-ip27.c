/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2003 Christoph Hellwig (hch@lst.de)
 * Copyright (C) 1999, 2000, 04 Ralf Baechle (ralf@linux-mips.org)
 * Copyright (C) 1999, 2000 Silicon Graphics, Inc.
 */
#include <linux/init.h>
#include <linux/kernel.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master
#include <linux/pci.h>
#include <linux/smp.h>
#include <asm/sn/arch.h>
#include <asm/pci/bridge.h>
#include <asm/paccess.h>
#include <asm/sn/intr.h>
#include <asm/sn/sn0/hub.h>

/*
 * Max #PCI busses we can handle; ie, max #PCI bridges.
 */
#define MAX_PCI_BUSSES		40

/*
 * Max #PCI devices (like scsi controllers) we handle on a bus.
 */
#define MAX_DEVICES_PER_PCIBUS	8

/*
 * XXX: No kmalloc available when we do our crosstalk scan,
<<<<<<< HEAD
 * 	we should try to move it later in the boot process.
=======
 *	we should try to move it later in the boot process.
>>>>>>> refs/remotes/origin/master
 */
static struct bridge_controller bridges[MAX_PCI_BUSSES];

/*
 * Translate from irq to software PCI bus number and PCI slot.
 */
struct bridge_controller *irq_to_bridge[MAX_PCI_BUSSES * MAX_DEVICES_PER_PCIBUS];
int irq_to_slot[MAX_PCI_BUSSES * MAX_DEVICES_PER_PCIBUS];

extern struct pci_ops bridge_pci_ops;

<<<<<<< HEAD
int __cpuinit bridge_probe(nasid_t nasid, int widget_id, int masterwid)
=======
int bridge_probe(nasid_t nasid, int widget_id, int masterwid)
>>>>>>> refs/remotes/origin/master
{
	unsigned long offset = NODE_OFFSET(nasid);
	struct bridge_controller *bc;
	static int num_bridges = 0;
	bridge_t *bridge;
	int slot;

<<<<<<< HEAD
<<<<<<< HEAD
	pci_probe_only = 1;
=======
	pci_set_flags(PCI_PROBE_ONLY);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pci_set_flags(PCI_PROBE_ONLY);
>>>>>>> refs/remotes/origin/master

	printk("a bridge\n");

	/* XXX: kludge alert.. */
	if (!num_bridges)
		ioport_resource.end = ~0UL;

	bc = &bridges[num_bridges];

	bc->pc.pci_ops		= &bridge_pci_ops;
	bc->pc.mem_resource	= &bc->mem;
	bc->pc.io_resource	= &bc->io;

	bc->pc.index		= num_bridges;

	bc->mem.name		= "Bridge PCI MEM";
	bc->pc.mem_offset	= offset;
	bc->mem.start		= 0;
	bc->mem.end		= ~0UL;
	bc->mem.flags		= IORESOURCE_MEM;

	bc->io.name		= "Bridge IO MEM";
	bc->pc.io_offset	= offset;
	bc->io.start		= 0UL;
	bc->io.end		= ~0UL;
	bc->io.flags		= IORESOURCE_IO;

	bc->irq_cpu = smp_processor_id();
	bc->widget_id = widget_id;
	bc->nasid = nasid;

	bc->baddr = (u64)masterwid << 60 | PCI64_ATTR_BAR;

	/*
	 * point to this bridge
	 */
	bridge = (bridge_t *) RAW_NODE_SWIN_BASE(nasid, widget_id);

	/*
	 * Clear all pending interrupts.
	 */
	bridge->b_int_rst_stat = BRIDGE_IRR_ALL_CLR;

	/*
	 * Until otherwise set up, assume all interrupts are from slot 0
	 */
	bridge->b_int_device = 0x0;

	/*
	 * swap pio's to pci mem and io space (big windows)
	 */
	bridge->b_wid_control |= BRIDGE_CTRL_IO_SWAP |
<<<<<<< HEAD
	                         BRIDGE_CTRL_MEM_SWAP;
=======
				 BRIDGE_CTRL_MEM_SWAP;
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_PAGE_SIZE_4KB
	bridge->b_wid_control &= ~BRIDGE_CTRL_PAGE_SIZE;
#else /* 16kB or larger */
	bridge->b_wid_control |= BRIDGE_CTRL_PAGE_SIZE;
#endif

	/*
	 * Hmm...  IRIX sets additional bits in the address which
	 * are documented as reserved in the bridge docs.
	 */
	bridge->b_wid_int_upper = 0x8000 | (masterwid << 16);
	bridge->b_wid_int_lower = 0x01800090;	/* PI_INT_PEND_MOD off*/
	bridge->b_dir_map = (masterwid << 20);	/* DMA */
	bridge->b_int_enable = 0;

	for (slot = 0; slot < 8; slot ++) {
		bridge->b_device[slot].reg |= BRIDGE_DEV_SWAP_DIR;
		bc->pci_int[slot] = -1;
	}
<<<<<<< HEAD
	bridge->b_wid_tflush;     /* wait until Bridge PIO complete */
=======
	bridge->b_wid_tflush;	  /* wait until Bridge PIO complete */
>>>>>>> refs/remotes/origin/master

	bc->base = bridge;

	register_pci_controller(&bc->pc);

	num_bridges++;

	return 0;
}

/*
 * All observed requests have pin == 1. We could have a global here, that
 * gets incremented and returned every time - unfortunately, pci_map_irq
 * may be called on the same device over and over, and need to return the
 * same value. On O2000, pin can be 0 or 1, and PCI slots can be [0..7].
 *
 * A given PCI device, in general, should be able to intr any of the cpus
 * on any one of the hubs connected to its xbow.
 */
<<<<<<< HEAD
int __devinit pcibios_map_irq(const struct pci_dev *dev, u8 slot, u8 pin)
=======
int pcibios_map_irq(const struct pci_dev *dev, u8 slot, u8 pin)
>>>>>>> refs/remotes/origin/master
{
	return 0;
}

static inline struct pci_dev *bridge_root_dev(struct pci_dev *dev)
{
	while (dev->bus->parent) {
		/* Move up the chain of bridges. */
		dev = dev->bus->self;
	}

	return dev;
}

/* Do platform specific device initialization at pci_enable_device() time */
int pcibios_plat_dev_init(struct pci_dev *dev)
{
	struct bridge_controller *bc = BRIDGE_CONTROLLER(dev->bus);
	struct pci_dev *rdev = bridge_root_dev(dev);
	int slot = PCI_SLOT(rdev->devfn);
	int irq;

	irq = bc->pci_int[slot];
	if (irq == -1) {
		irq = request_bridge_irq(bc);
		if (irq < 0)
			return irq;

		bc->pci_int[slot] = irq;
	}

	irq_to_bridge[irq] = bc;
	irq_to_slot[irq] = slot;

	dev->irq = irq;

	return 0;
}

/*
<<<<<<< HEAD
 * Device might live on a subordinate PCI bus.  XXX Walk up the chain of buses
=======
 * Device might live on a subordinate PCI bus.	XXX Walk up the chain of buses
>>>>>>> refs/remotes/origin/master
 * to find the slot number in sense of the bridge device register.
 * XXX This also means multiple devices might rely on conflicting bridge
 * settings.
 */

static inline void pci_disable_swapping(struct pci_dev *dev)
{
	struct bridge_controller *bc = BRIDGE_CONTROLLER(dev->bus);
	bridge_t *bridge = bc->base;
	int slot = PCI_SLOT(dev->devfn);

	/* Turn off byte swapping */
	bridge->b_device[slot].reg &= ~BRIDGE_DEV_SWAP_DIR;
	bridge->b_widget.w_tflush;	/* Flush */
}

static inline void pci_enable_swapping(struct pci_dev *dev)
{
	struct bridge_controller *bc = BRIDGE_CONTROLLER(dev->bus);
	bridge_t *bridge = bc->base;
	int slot = PCI_SLOT(dev->devfn);

	/* Turn on byte swapping */
	bridge->b_device[slot].reg |= BRIDGE_DEV_SWAP_DIR;
	bridge->b_widget.w_tflush;	/* Flush */
}

<<<<<<< HEAD
static void __init pci_fixup_ioc3(struct pci_dev *d)
=======
static void pci_fixup_ioc3(struct pci_dev *d)
>>>>>>> refs/remotes/origin/master
{
	pci_disable_swapping(d);
}

<<<<<<< HEAD
=======
#ifdef CONFIG_NUMA
>>>>>>> refs/remotes/origin/master
int pcibus_to_node(struct pci_bus *bus)
{
	struct bridge_controller *bc = BRIDGE_CONTROLLER(bus);

	return bc->nasid;
}
EXPORT_SYMBOL(pcibus_to_node);
<<<<<<< HEAD
=======
#endif /* CONFIG_NUMA */
>>>>>>> refs/remotes/origin/master

DECLARE_PCI_FIXUP_HEADER(PCI_VENDOR_ID_SGI, PCI_DEVICE_ID_SGI_IOC3,
	pci_fixup_ioc3);
