/*
 * arch/sh/drivers/pci/ops-titan.c
 *
 * Ported to new API by Paul Mundt <lethal@linux-sh.org>
 *
 * Modified from ops-snapgear.c written by  David McCullough
 * Highly leveraged from pci-bigsur.c, written by Dustin McIntire.
 *
 * May be copied or modified under the terms of the GNU General Public
 * License.  See linux/COPYING for more information.
 *
 * PCI initialization for the Titan boards
 */
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/io.h>
#include <mach/titan.h>
#include "pci-sh4.h"

static char titan_irq_tab[] __initdata = {
	TITAN_IRQ_WAN,
	TITAN_IRQ_LAN,
	TITAN_IRQ_MPCIA,
	TITAN_IRQ_MPCIB,
	TITAN_IRQ_USB,
};

<<<<<<< HEAD
<<<<<<< HEAD
int __init pcibios_map_platform_irq(struct pci_dev *pdev, u8 slot, u8 pin)
=======
int __init pcibios_map_platform_irq(const struct pci_dev *pdev, u8 slot, u8 pin)
>>>>>>> refs/remotes/origin/cm-10.0
=======
int __init pcibios_map_platform_irq(const struct pci_dev *pdev, u8 slot, u8 pin)
>>>>>>> refs/remotes/origin/master
{
	int irq = titan_irq_tab[slot];

	printk("PCI: Mapping TITAN IRQ for slot %d, pin %c to irq %d\n",
		slot, pin - 1 + 'A', irq);

	return irq;
}
