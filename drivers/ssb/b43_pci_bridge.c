/*
 * Broadcom 43xx PCI-SSB bridge module
 *
 * This technically is a separate PCI driver module, but
 * because of its small size we include it in the SSB core
 * instead of creating a standalone module.
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright 2007  Michael Buesch <mb@bu3sch.de>
=======
 * Copyright 2007  Michael Buesch <m@bues.ch>
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Copyright 2007  Michael Buesch <m@bues.ch>
>>>>>>> refs/remotes/origin/master
 *
 * Licensed under the GNU/GPL. See COPYING for details.
 */

#include <linux/pci.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/master
#include <linux/ssb/ssb.h>

#include "ssb_private.h"


static const struct pci_device_id b43_pci_bridge_tbl[] = {
	{ PCI_DEVICE(PCI_VENDOR_ID_BROADCOM, 0x4301) },
	{ PCI_DEVICE(PCI_VENDOR_ID_BROADCOM, 0x4306) },
	{ PCI_DEVICE(PCI_VENDOR_ID_BROADCOM, 0x4307) },
	{ PCI_DEVICE(PCI_VENDOR_ID_BROADCOM, 0x4311) },
	{ PCI_DEVICE(PCI_VENDOR_ID_BROADCOM, 0x4312) },
	{ PCI_DEVICE(PCI_VENDOR_ID_BROADCOM, 0x4315) },
	{ PCI_DEVICE(PCI_VENDOR_ID_BROADCOM, 0x4318) },
	{ PCI_DEVICE(PCI_VENDOR_ID_BCM_GVC,  0x4318) },
	{ PCI_DEVICE(PCI_VENDOR_ID_BROADCOM, 0x4319) },
	{ PCI_DEVICE(PCI_VENDOR_ID_BROADCOM, 0x4320) },
	{ PCI_DEVICE(PCI_VENDOR_ID_BROADCOM, 0x4321) },
<<<<<<< HEAD
=======
	{ PCI_DEVICE(PCI_VENDOR_ID_BROADCOM, 0x4322) },
	{ PCI_DEVICE(PCI_VENDOR_ID_BROADCOM, 43222) },
>>>>>>> refs/remotes/origin/master
	{ PCI_DEVICE(PCI_VENDOR_ID_BROADCOM, 0x4324) },
	{ PCI_DEVICE(PCI_VENDOR_ID_BROADCOM, 0x4325) },
	{ PCI_DEVICE(PCI_VENDOR_ID_BROADCOM, 0x4328) },
	{ PCI_DEVICE(PCI_VENDOR_ID_BROADCOM, 0x4329) },
	{ PCI_DEVICE(PCI_VENDOR_ID_BROADCOM, 0x432b) },
<<<<<<< HEAD
=======
	{ PCI_DEVICE(PCI_VENDOR_ID_BROADCOM, 0x432c) },
	{ PCI_DEVICE(PCI_VENDOR_ID_BROADCOM, 0x4350) },
>>>>>>> refs/remotes/origin/master
	{ 0, },
};
MODULE_DEVICE_TABLE(pci, b43_pci_bridge_tbl);

static struct pci_driver b43_pci_bridge_driver = {
	.name = "b43-pci-bridge",
	.id_table = b43_pci_bridge_tbl,
};


int __init b43_pci_ssb_bridge_init(void)
{
	return ssb_pcihost_register(&b43_pci_bridge_driver);
}

void __exit b43_pci_ssb_bridge_exit(void)
{
	ssb_pcihost_unregister(&b43_pci_bridge_driver);
}
