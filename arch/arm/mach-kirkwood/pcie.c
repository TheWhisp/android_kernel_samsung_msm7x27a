/*
 * arch/arm/mach-kirkwood/pcie.c
 *
 * PCIe functions for Marvell Kirkwood SoCs
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/slab.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/mbus.h>
=======
#include <video/vga.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/clk.h>
#include <linux/mbus.h>
#include <video/vga.h>
>>>>>>> refs/remotes/origin/master
#include <asm/irq.h>
#include <asm/mach/pci.h>
#include <plat/pcie.h>
#include <mach/bridge-regs.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <plat/addr-map.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include "common.h"

=======
#include "common.h"

/* These can go away once Kirkwood uses the mvebu-mbus DT binding */
#define KIRKWOOD_MBUS_PCIE0_MEM_TARGET    0x4
#define KIRKWOOD_MBUS_PCIE0_MEM_ATTR      0xe8
#define KIRKWOOD_MBUS_PCIE0_IO_TARGET     0x4
#define KIRKWOOD_MBUS_PCIE0_IO_ATTR       0xe0
#define KIRKWOOD_MBUS_PCIE1_MEM_TARGET    0x4
#define KIRKWOOD_MBUS_PCIE1_MEM_ATTR      0xd8
#define KIRKWOOD_MBUS_PCIE1_IO_TARGET     0x4
#define KIRKWOOD_MBUS_PCIE1_IO_ATTR       0xd0

static void kirkwood_enable_pcie_clk(const char *port)
{
	struct clk *clk;

	clk = clk_get_sys("pcie", port);
	if (IS_ERR(clk)) {
		pr_err("PCIE clock %s missing\n", port);
		return;
	}
	clk_prepare_enable(clk);
	clk_put(clk);
}

/* This function is called very early in the boot when probing the
   hardware to determine what we actually are, and what rate tclk is
   ticking at. Hence calling kirkwood_enable_pcie_clk() is not
   possible since the clk tree has not been created yet. */
>>>>>>> refs/remotes/origin/master
void kirkwood_enable_pcie(void)
{
	u32 curr = readl(CLOCK_GATING_CTRL);
	if (!(curr & CGC_PEX0))
		writel(curr | CGC_PEX0, CLOCK_GATING_CTRL);
}

<<<<<<< HEAD
void __init kirkwood_pcie_id(u32 *dev, u32 *rev)
{
	kirkwood_enable_pcie();
	*dev = orion_pcie_dev_id((void __iomem *)PCIE_VIRT_BASE);
	*rev = orion_pcie_rev((void __iomem *)PCIE_VIRT_BASE);
=======
void kirkwood_pcie_id(u32 *dev, u32 *rev)
{
	kirkwood_enable_pcie();
	*dev = orion_pcie_dev_id(PCIE_VIRT_BASE);
	*rev = orion_pcie_rev(PCIE_VIRT_BASE);
>>>>>>> refs/remotes/origin/master
}

struct pcie_port {
	u8			root_bus_nr;
	void __iomem		*base;
	spinlock_t		conf_lock;
	int			irq;
<<<<<<< HEAD
	struct resource		res[2];
=======
	struct resource		res;
>>>>>>> refs/remotes/origin/master
};

static int pcie_port_map[2];
static int num_pcie_ports;

<<<<<<< HEAD
static inline struct pcie_port *bus_to_port(struct pci_bus *bus)
{
	struct pci_sys_data *sys = bus->sysdata;
	return sys->private_data;
}

=======
>>>>>>> refs/remotes/origin/master
static int pcie_valid_config(struct pcie_port *pp, int bus, int dev)
{
	/*
	 * Don't go out when trying to access --
	 * 1. nonexisting device on local bus
	 * 2. where there's no device connected (no link)
	 */
	if (bus == pp->root_bus_nr && dev == 0)
		return 1;

	if (!orion_pcie_link_up(pp->base))
		return 0;

	if (bus == pp->root_bus_nr && dev != 1)
		return 0;

	return 1;
}


/*
 * PCIe config cycles are done by programming the PCIE_CONF_ADDR register
 * and then reading the PCIE_CONF_DATA register. Need to make sure these
 * transactions are atomic.
 */

static int pcie_rd_conf(struct pci_bus *bus, u32 devfn, int where,
			int size, u32 *val)
{
<<<<<<< HEAD
	struct pcie_port *pp = bus_to_port(bus);
=======
	struct pci_sys_data *sys = bus->sysdata;
	struct pcie_port *pp = sys->private_data;
>>>>>>> refs/remotes/origin/master
	unsigned long flags;
	int ret;

	if (pcie_valid_config(pp, bus->number, PCI_SLOT(devfn)) == 0) {
		*val = 0xffffffff;
		return PCIBIOS_DEVICE_NOT_FOUND;
	}

	spin_lock_irqsave(&pp->conf_lock, flags);
	ret = orion_pcie_rd_conf(pp->base, bus, devfn, where, size, val);
	spin_unlock_irqrestore(&pp->conf_lock, flags);

	return ret;
}

static int pcie_wr_conf(struct pci_bus *bus, u32 devfn,
			int where, int size, u32 val)
{
<<<<<<< HEAD
	struct pcie_port *pp = bus_to_port(bus);
=======
	struct pci_sys_data *sys = bus->sysdata;
	struct pcie_port *pp = sys->private_data;
>>>>>>> refs/remotes/origin/master
	unsigned long flags;
	int ret;

	if (pcie_valid_config(pp, bus->number, PCI_SLOT(devfn)) == 0)
		return PCIBIOS_DEVICE_NOT_FOUND;

	spin_lock_irqsave(&pp->conf_lock, flags);
	ret = orion_pcie_wr_conf(pp->base, bus, devfn, where, size, val);
	spin_unlock_irqrestore(&pp->conf_lock, flags);

	return ret;
}

static struct pci_ops pcie_ops = {
	.read = pcie_rd_conf,
	.write = pcie_wr_conf,
};

static void __init pcie0_ioresources_init(struct pcie_port *pp)
{
<<<<<<< HEAD
	pp->base = (void __iomem *)PCIE_VIRT_BASE;
	pp->irq	= IRQ_KIRKWOOD_PCIE;

	/*
	 * IORESOURCE_IO
	 */
	pp->res[0].name = "PCIe 0 I/O Space";
	pp->res[0].start = KIRKWOOD_PCIE_IO_BUS_BASE;
	pp->res[0].end = pp->res[0].start + KIRKWOOD_PCIE_IO_SIZE - 1;
	pp->res[0].flags = IORESOURCE_IO;

	/*
	 * IORESOURCE_MEM
	 */
	pp->res[1].name = "PCIe 0 MEM";
	pp->res[1].start = KIRKWOOD_PCIE_MEM_PHYS_BASE;
	pp->res[1].end = pp->res[1].start + KIRKWOOD_PCIE_MEM_SIZE - 1;
	pp->res[1].flags = IORESOURCE_MEM;
=======
	pp->base = PCIE_VIRT_BASE;
	pp->irq	= IRQ_KIRKWOOD_PCIE;

	/*
	 * IORESOURCE_MEM
	 */
	pp->res.name = "PCIe 0 MEM";
	pp->res.start = KIRKWOOD_PCIE_MEM_PHYS_BASE;
	pp->res.end = pp->res.start + KIRKWOOD_PCIE_MEM_SIZE - 1;
	pp->res.flags = IORESOURCE_MEM;
>>>>>>> refs/remotes/origin/master
}

static void __init pcie1_ioresources_init(struct pcie_port *pp)
{
<<<<<<< HEAD
	pp->base = (void __iomem *)PCIE1_VIRT_BASE;
	pp->irq	= IRQ_KIRKWOOD_PCIE1;

	/*
	 * IORESOURCE_IO
	 */
	pp->res[0].name = "PCIe 1 I/O Space";
	pp->res[0].start = KIRKWOOD_PCIE1_IO_BUS_BASE;
	pp->res[0].end = pp->res[0].start + KIRKWOOD_PCIE1_IO_SIZE - 1;
	pp->res[0].flags = IORESOURCE_IO;

	/*
	 * IORESOURCE_MEM
	 */
	pp->res[1].name = "PCIe 1 MEM";
	pp->res[1].start = KIRKWOOD_PCIE1_MEM_PHYS_BASE;
	pp->res[1].end = pp->res[1].start + KIRKWOOD_PCIE1_MEM_SIZE - 1;
	pp->res[1].flags = IORESOURCE_MEM;
=======
	pp->base = PCIE1_VIRT_BASE;
	pp->irq	= IRQ_KIRKWOOD_PCIE1;

	/*
	 * IORESOURCE_MEM
	 */
	pp->res.name = "PCIe 1 MEM";
	pp->res.start = KIRKWOOD_PCIE1_MEM_PHYS_BASE;
	pp->res.end = pp->res.start + KIRKWOOD_PCIE1_MEM_SIZE - 1;
	pp->res.flags = IORESOURCE_MEM;
>>>>>>> refs/remotes/origin/master
}

static int __init kirkwood_pcie_setup(int nr, struct pci_sys_data *sys)
{
<<<<<<< HEAD
	extern unsigned int kirkwood_clk_ctrl;
=======
>>>>>>> refs/remotes/origin/master
	struct pcie_port *pp;
	int index;

	if (nr >= num_pcie_ports)
		return 0;

	index = pcie_port_map[nr];
<<<<<<< HEAD
	printk(KERN_INFO "PCI: bus%d uses PCIe port %d\n", sys->busnr, index);
=======
	pr_info("PCI: bus%d uses PCIe port %d\n", sys->busnr, index);
>>>>>>> refs/remotes/origin/master

	pp = kzalloc(sizeof(*pp), GFP_KERNEL);
	if (!pp)
		panic("PCIe: failed to allocate pcie_port data");
	sys->private_data = pp;
	pp->root_bus_nr = sys->busnr;
	spin_lock_init(&pp->conf_lock);

	switch (index) {
	case 0:
<<<<<<< HEAD
		kirkwood_clk_ctrl |= CGC_PEX0;
		pcie0_ioresources_init(pp);
		break;
	case 1:
		kirkwood_clk_ctrl |= CGC_PEX1;
		pcie1_ioresources_init(pp);
=======
		kirkwood_enable_pcie_clk("0");
		pcie0_ioresources_init(pp);
		pci_ioremap_io(SZ_64K * sys->busnr, KIRKWOOD_PCIE_IO_PHYS_BASE);
		break;
	case 1:
		kirkwood_enable_pcie_clk("1");
		pcie1_ioresources_init(pp);
		pci_ioremap_io(SZ_64K * sys->busnr,
			       KIRKWOOD_PCIE1_IO_PHYS_BASE);
>>>>>>> refs/remotes/origin/master
		break;
	default:
		panic("PCIe setup: invalid controller %d", index);
	}

<<<<<<< HEAD
	if (request_resource(&ioport_resource, &pp->res[0]))
		panic("Request PCIe%d IO resource failed\n", index);
	if (request_resource(&iomem_resource, &pp->res[1]))
		panic("Request PCIe%d Memory resource failed\n", index);

<<<<<<< HEAD
	sys->resource[0] = &pp->res[0];
	sys->resource[1] = &pp->res[1];
	sys->resource[2] = NULL;
	sys->io_offset = 0;
=======
	sys->io_offset = 0;
	pci_add_resource_offset(&sys->resources, &pp->res[0], sys->io_offset);
	pci_add_resource_offset(&sys->resources, &pp->res[1], sys->mem_offset);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (request_resource(&iomem_resource, &pp->res))
		panic("Request PCIe%d Memory resource failed\n", index);

	pci_add_resource_offset(&sys->resources, &pp->res, sys->mem_offset);
>>>>>>> refs/remotes/origin/master

	/*
	 * Generic PCIe unit setup.
	 */
	orion_pcie_set_local_bus_nr(pp->base, sys->busnr);

<<<<<<< HEAD
<<<<<<< HEAD
	orion_pcie_setup(pp->base, &kirkwood_mbus_dram_info);
=======
	orion_pcie_setup(pp->base);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	orion_pcie_setup(pp->base);
>>>>>>> refs/remotes/origin/master

	return 1;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void __devinit rc_pci_fixup(struct pci_dev *dev)
{
	/*
	 * Prevent enumeration of root complex.
	 */
	if (dev->bus->parent == NULL && dev->devfn == 0) {
		int i;

=======
=======
>>>>>>> refs/remotes/origin/master
/*
 * The root complex has a hardwired class of PCI_CLASS_MEMORY_OTHER, when it
 * is operating as a root complex this needs to be switched to
 * PCI_CLASS_BRIDGE_HOST or Linux will errantly try to process the BAR's on
 * the device. Decoding setup is handled by the orion code.
 */
<<<<<<< HEAD
static void __devinit rc_pci_fixup(struct pci_dev *dev)
=======
static void rc_pci_fixup(struct pci_dev *dev)
>>>>>>> refs/remotes/origin/master
{
	if (dev->bus->parent == NULL && dev->devfn == 0) {
		int i;

		dev->class &= 0xff;
		dev->class |= PCI_CLASS_BRIDGE_HOST << 8;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		for (i = 0; i < DEVICE_COUNT_RESOURCE; i++) {
			dev->resource[i].start = 0;
			dev->resource[i].end   = 0;
			dev->resource[i].flags = 0;
		}
	}
}
DECLARE_PCI_FIXUP_HEADER(PCI_VENDOR_ID_MARVELL, PCI_ANY_ID, rc_pci_fixup);

<<<<<<< HEAD
static struct pci_bus __init *
kirkwood_pcie_scan_bus(int nr, struct pci_sys_data *sys)
{
	struct pci_bus *bus;

	if (nr < num_pcie_ports) {
<<<<<<< HEAD
		bus = pci_scan_bus(sys->busnr, &pcie_ops, sys);
=======
		bus = pci_scan_root_bus(NULL, sys->busnr, &pcie_ops, sys,
					&sys->resources);
>>>>>>> refs/remotes/origin/cm-10.0
	} else {
		bus = NULL;
		BUG();
	}

	return bus;
}

<<<<<<< HEAD
static int __init kirkwood_pcie_map_irq(struct pci_dev *dev, u8 slot, u8 pin)
=======
static int __init kirkwood_pcie_map_irq(const struct pci_dev *dev, u8 slot,
	u8 pin)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct pcie_port *pp = bus_to_port(dev->bus);
=======
static int __init kirkwood_pcie_map_irq(const struct pci_dev *dev, u8 slot,
	u8 pin)
{
	struct pci_sys_data *sys = dev->sysdata;
	struct pcie_port *pp = sys->private_data;
>>>>>>> refs/remotes/origin/master

	return pp->irq;
}

static struct hw_pci kirkwood_pci __initdata = {
<<<<<<< HEAD
	.swizzle	= pci_std_swizzle,
	.setup		= kirkwood_pcie_setup,
	.scan		= kirkwood_pcie_scan_bus,
	.map_irq	= kirkwood_pcie_map_irq,
};

static void __init add_pcie_port(int index, unsigned long base)
{
	printk(KERN_INFO "Kirkwood PCIe port %d: ", index);

	if (orion_pcie_link_up((void __iomem *)base)) {
		printk(KERN_INFO "link up\n");
		pcie_port_map[num_pcie_ports++] = index;
	} else
		printk(KERN_INFO "link down, ignoring\n");
=======
	.setup		= kirkwood_pcie_setup,
	.map_irq	= kirkwood_pcie_map_irq,
	.ops            = &pcie_ops,
};

static void __init add_pcie_port(int index, void __iomem *base)
{
	pcie_port_map[num_pcie_ports++] = index;
	pr_info("Kirkwood PCIe port %d: link %s\n", index,
		orion_pcie_link_up(base) ? "up" : "down");
>>>>>>> refs/remotes/origin/master
}

void __init kirkwood_pcie_init(unsigned int portmask)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
	vga_base = KIRKWOOD_PCIE_MEM_PHYS_BASE;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	mvebu_mbus_add_window_remap_by_id(KIRKWOOD_MBUS_PCIE0_IO_TARGET,
					  KIRKWOOD_MBUS_PCIE0_IO_ATTR,
					  KIRKWOOD_PCIE_IO_PHYS_BASE,
					  KIRKWOOD_PCIE_IO_SIZE,
					  KIRKWOOD_PCIE_IO_BUS_BASE);
	mvebu_mbus_add_window_by_id(KIRKWOOD_MBUS_PCIE0_MEM_TARGET,
				    KIRKWOOD_MBUS_PCIE0_MEM_ATTR,
				    KIRKWOOD_PCIE_MEM_PHYS_BASE,
				    KIRKWOOD_PCIE_MEM_SIZE);
	mvebu_mbus_add_window_remap_by_id(KIRKWOOD_MBUS_PCIE1_IO_TARGET,
					  KIRKWOOD_MBUS_PCIE1_IO_ATTR,
					  KIRKWOOD_PCIE1_IO_PHYS_BASE,
					  KIRKWOOD_PCIE1_IO_SIZE,
					  KIRKWOOD_PCIE1_IO_BUS_BASE);
	mvebu_mbus_add_window_by_id(KIRKWOOD_MBUS_PCIE1_MEM_TARGET,
				    KIRKWOOD_MBUS_PCIE1_MEM_ATTR,
				    KIRKWOOD_PCIE1_MEM_PHYS_BASE,
				    KIRKWOOD_PCIE1_MEM_SIZE);

	vga_base = KIRKWOOD_PCIE_MEM_PHYS_BASE;

>>>>>>> refs/remotes/origin/master
	if (portmask & KW_PCIE0)
		add_pcie_port(0, PCIE_VIRT_BASE);

	if (portmask & KW_PCIE1)
		add_pcie_port(1, PCIE1_VIRT_BASE);

	kirkwood_pci.nr_controllers = num_pcie_ports;
	pci_common_init(&kirkwood_pci);
}
