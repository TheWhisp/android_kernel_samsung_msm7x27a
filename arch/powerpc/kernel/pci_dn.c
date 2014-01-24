/*
 * pci_dn.c
 *
 * Copyright (C) 2001 Todd Inglett, IBM Corporation
 *
 * PCI manipulation via device_nodes.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *    
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */
#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/string.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master
#include <linux/init.h>
#include <linux/gfp.h>

#include <asm/io.h>
#include <asm/prom.h>
#include <asm/pci-bridge.h>
#include <asm/ppc-pci.h>
#include <asm/firmware.h>

<<<<<<< HEAD
=======
struct pci_dn *pci_get_pdn(struct pci_dev *pdev)
{
	struct device_node *dn = pci_device_to_OF_node(pdev);
	if (!dn)
		return NULL;
	return PCI_DN(dn);
}

>>>>>>> refs/remotes/origin/master
/*
 * Traverse_func that inits the PCI fields of the device node.
 * NOTE: this *must* be done before read/write config to the device.
 */
<<<<<<< HEAD
void * __devinit update_dn_pci_info(struct device_node *dn, void *data)
{
	struct pci_controller *phb = data;
	const int *type =
		of_get_property(dn, "ibm,pci-config-space-type", NULL);
	const u32 *regs;
=======
void *update_dn_pci_info(struct device_node *dn, void *data)
{
	struct pci_controller *phb = data;
	const __be32 *type = of_get_property(dn, "ibm,pci-config-space-type", NULL);
	const __be32 *regs;
>>>>>>> refs/remotes/origin/master
	struct pci_dn *pdn;

	pdn = zalloc_maybe_bootmem(sizeof(*pdn), GFP_KERNEL);
	if (pdn == NULL)
		return NULL;
	dn->data = pdn;
	pdn->node = dn;
	pdn->phb = phb;
<<<<<<< HEAD
<<<<<<< HEAD
=======
#ifdef CONFIG_PPC_POWERNV
	pdn->pe_number = IODA_INVALID_PE;
#endif
>>>>>>> refs/remotes/origin/cm-10.0
	regs = of_get_property(dn, "reg", NULL);
	if (regs) {
		/* First register entry is addr (00BBSS00)  */
		pdn->busno = (regs[0] >> 16) & 0xff;
		pdn->devfn = (regs[0] >> 8) & 0xff;
	}

	pdn->pci_ext_config_space = (type && *type == 1);
=======
#ifdef CONFIG_PPC_POWERNV
	pdn->pe_number = IODA_INVALID_PE;
#endif
	regs = of_get_property(dn, "reg", NULL);
	if (regs) {
		u32 addr = of_read_number(regs, 1);

		/* First register entry is addr (00BBSS00)  */
		pdn->busno = (addr >> 16) & 0xff;
		pdn->devfn = (addr >> 8) & 0xff;
	}

	pdn->pci_ext_config_space = (type && of_read_number(type, 1) == 1);
>>>>>>> refs/remotes/origin/master
	return NULL;
}

/*
 * Traverse a device tree stopping each PCI device in the tree.
 * This is done depth first.  As each node is processed, a "pre"
 * function is called and the children are processed recursively.
 *
 * The "pre" func returns a value.  If non-zero is returned from
 * the "pre" func, the traversal stops and this value is returned.
 * This return value is useful when using traverse as a method of
 * finding a device.
 *
 * NOTE: we do not run the func for devices that do not appear to
 * be PCI except for the start node which we assume (this is good
 * because the start node is often a phb which may be missing PCI
 * properties).
 * We use the class-code as an indicator. If we run into
 * one of these nodes we also assume its siblings are non-pci for
 * performance.
 */
void *traverse_pci_devices(struct device_node *start, traverse_func pre,
		void *data)
{
	struct device_node *dn, *nextdn;
	void *ret;

	/* We started with a phb, iterate all childs */
	for (dn = start->child; dn; dn = nextdn) {
<<<<<<< HEAD
		const u32 *classp;
		u32 class;

		nextdn = NULL;
		classp = of_get_property(dn, "class-code", NULL);
		class = classp ? *classp : 0;
=======
		const __be32 *classp;
		u32 class = 0;

		nextdn = NULL;
		classp = of_get_property(dn, "class-code", NULL);
		if (classp)
			class = of_read_number(classp, 1);
>>>>>>> refs/remotes/origin/master

		if (pre && ((ret = pre(dn, data)) != NULL))
			return ret;

		/* If we are a PCI bridge, go down */
		if (dn->child && ((class >> 8) == PCI_CLASS_BRIDGE_PCI ||
				  (class >> 8) == PCI_CLASS_BRIDGE_CARDBUS))
			/* Depth first...do children */
			nextdn = dn->child;
		else if (dn->sibling)
			/* ok, try next sibling instead. */
			nextdn = dn->sibling;
		if (!nextdn) {
			/* Walk up to next valid sibling. */
			do {
				dn = dn->parent;
				if (dn == start)
					return NULL;
			} while (dn->sibling == NULL);
			nextdn = dn->sibling;
		}
	}
	return NULL;
}

/** 
 * pci_devs_phb_init_dynamic - setup pci devices under this PHB
 * phb: pci-to-host bridge (top-level bridge connecting to cpu)
 *
 * This routine is called both during boot, (before the memory
 * subsystem is set up, before kmalloc is valid) and during the 
 * dynamic lpar operation of adding a PHB to a running system.
 */
<<<<<<< HEAD
void __devinit pci_devs_phb_init_dynamic(struct pci_controller *phb)
=======
void pci_devs_phb_init_dynamic(struct pci_controller *phb)
>>>>>>> refs/remotes/origin/master
{
	struct device_node *dn = phb->dn;
	struct pci_dn *pdn;

	/* PHB nodes themselves must not match */
	update_dn_pci_info(dn, phb);
	pdn = dn->data;
	if (pdn) {
		pdn->devfn = pdn->busno = -1;
		pdn->phb = phb;
	}

	/* Update dn->phb ptrs for new phb and children devices */
	traverse_pci_devices(dn, update_dn_pci_info, phb);
}

<<<<<<< HEAD
<<<<<<< HEAD
/*
 * Traversal func that looks for a <busno,devfcn> value.
 * If found, the pci_dn is returned (thus terminating the traversal).
 */
static void *is_devfn_node(struct device_node *dn, void *data)
{
	int busno = ((unsigned long)data >> 8) & 0xff;
	int devfn = ((unsigned long)data) & 0xff;
	struct pci_dn *pci = dn->data;

	if (pci && (devfn == pci->devfn) && (busno == pci->busno))
		return dn;
	return NULL;
}

/*
 * This is the "slow" path for looking up a device_node from a
 * pci_dev.  It will hunt for the device under its parent's
 * phb and then update of_node pointer.
 *
 * It may also do fixups on the actual device since this happens
 * on the first read/write.
 *
 * Note that it also must deal with devices that don't exist.
 * In this case it may probe for real hardware ("just in case")
 * and add a device_node to the device tree if necessary.
 *
 * Is this function necessary anymore now that dev->dev.of_node is
 * used to store the node pointer?
 *
 */
struct device_node *fetch_dev_dn(struct pci_dev *dev)
{
	struct pci_controller *phb = dev->sysdata;
	struct device_node *dn;
	unsigned long searchval = (dev->bus->number << 8) | dev->devfn;

	if (WARN_ON(!phb))
		return NULL;

	dn = traverse_pci_devices(phb->dn, is_devfn_node, (void *)searchval);
	if (dn)
		dev->dev.of_node = dn;
	return dn;
}
EXPORT_SYMBOL(fetch_dev_dn);

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/** 
 * pci_devs_phb_init - Initialize phbs and pci devs under them.
 * 
 * This routine walks over all phb's (pci-host bridges) on the
 * system, and sets up assorted pci-related structures 
 * (including pci info in the device node structs) for each
 * pci device found underneath.  This routine runs once,
 * early in the boot sequence.
 */
void __init pci_devs_phb_init(void)
{
	struct pci_controller *phb, *tmp;

	/* This must be done first so the device nodes have valid pci info! */
	list_for_each_entry_safe(phb, tmp, &hose_list, list_node)
		pci_devs_phb_init_dynamic(phb);
}
