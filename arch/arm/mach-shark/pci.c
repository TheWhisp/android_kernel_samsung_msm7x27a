/*
 *  linux/arch/arm/mach-shark/pci.c
 *
 *  PCI bios-type initialisation for PCI machines
 *
 *  Bits taken from various places.
 */
#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/init.h>
<<<<<<< HEAD
=======
#include <video/vga.h>
>>>>>>> refs/remotes/origin/cm-10.0

#include <asm/irq.h>
#include <asm/mach/pci.h>
#include <asm/mach-types.h>

<<<<<<< HEAD
static int __init shark_map_irq(struct pci_dev *dev, u8 slot, u8 pin)
=======
static int __init shark_map_irq(const struct pci_dev *dev, u8 slot, u8 pin)
>>>>>>> refs/remotes/origin/cm-10.0
{
	if (dev->bus->number == 0)
		if (dev->devfn == 0)
			return 255;
		else
			return 11;
	else
		return 255;
}

extern void __init via82c505_preinit(void);

static struct hw_pci shark_pci __initdata = {
	.setup		= via82c505_setup,
	.swizzle	= pci_std_swizzle,
	.map_irq	= shark_map_irq,
	.nr_controllers = 1,
	.scan		= via82c505_scan_bus,
	.preinit	= via82c505_preinit,
};

static int __init shark_pci_init(void)
{
<<<<<<< HEAD
	if (machine_is_shark())
		pci_common_init(&shark_pci);
=======
	if (!machine_is_shark())
		return;

	pcibios_min_io = 0x6000;
	pcibios_min_mem = 0x50000000;
	vga_base = 0xe8000000;

	pci_common_init(&shark_pci);

>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
}

subsys_initcall(shark_pci_init);
