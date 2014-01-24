/* Core PCI functionality used only by PCI hotplug */

#include <linux/pci.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include "pci.h"


unsigned int __devinit pci_do_scan_bus(struct pci_bus *bus)
{
	unsigned int max;

	max = pci_scan_child_bus(bus);

	/*
	 * Make the discovered devices available.
	 */
	pci_bus_add_devices(bus);

	return max;
}
EXPORT_SYMBOL(pci_do_scan_bus);
=======
#include <linux/export.h>
#include "pci.h"

int __ref pci_hp_add_bridge(struct pci_dev *dev)
{
	struct pci_bus *parent = dev->bus;
	int pass, busnr, start = parent->busn_res.start;
	int end = parent->busn_res.end;

	for (busnr = start; busnr <= end; busnr++) {
		if (!pci_find_bus(pci_domain_nr(parent), busnr))
			break;
	}
	if (busnr-- > end) {
		printk(KERN_ERR "No bus number available for hot-added bridge %s\n",
				pci_name(dev));
		return -1;
	}
	for (pass = 0; pass < 2; pass++)
		busnr = pci_scan_bridge(parent, dev, busnr, pass);
	if (!dev->subordinate)
		return -1;

	return 0;
}
EXPORT_SYMBOL_GPL(pci_hp_add_bridge);
>>>>>>> refs/remotes/origin/master
