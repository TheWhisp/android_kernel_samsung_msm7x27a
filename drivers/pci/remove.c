#include <linux/pci.h>
#include <linux/module.h>
#include <linux/pci-aspm.h>
#include "pci.h"

static void pci_free_resources(struct pci_dev *dev)
{
	int i;

<<<<<<< HEAD
 	msi_remove_pci_irq_vectors(dev);
=======
	msi_remove_pci_irq_vectors(dev);
>>>>>>> refs/remotes/origin/master

	pci_cleanup_rom(dev);
	for (i = 0; i < PCI_NUM_RESOURCES; i++) {
		struct resource *res = dev->resource + i;
		if (res->parent)
			release_resource(res);
	}
}

static void pci_stop_dev(struct pci_dev *dev)
{
	pci_pme_active(dev, false);

	if (dev->is_added) {
		pci_proc_detach_device(dev);
		pci_remove_sysfs_dev_files(dev);
<<<<<<< HEAD
		device_unregister(&dev->dev);
=======
		device_release_driver(&dev->dev);
>>>>>>> refs/remotes/origin/master
		dev->is_added = 0;
	}

	if (dev->bus->self)
		pcie_aspm_exit_link_state(dev);
}

static void pci_destroy_dev(struct pci_dev *dev)
{
<<<<<<< HEAD
	/* Remove the device from the device lists, and prevent any further
	 * list accesses from this device */
	down_write(&pci_bus_sem);
	list_del(&dev->bus_list);
	dev->bus_list.next = dev->bus_list.prev = NULL;
	up_write(&pci_bus_sem);

	pci_free_resources(dev);
	pci_dev_put(dev);
}

/**
 * pci_remove_device_safe - remove an unused hotplug device
 * @dev: the device to remove
 *
 * Delete the device structure from the device lists and 
 * notify userspace (/sbin/hotplug), but only if the device
 * in question is not being used by a driver.
 * Returns 0 on success.
 */
#if 0
int pci_remove_device_safe(struct pci_dev *dev)
{
	if (pci_dev_driver(dev))
		return -EBUSY;
	pci_destroy_dev(dev);
	return 0;
}
#endif  /*  0  */

void pci_remove_bus(struct pci_bus *pci_bus)
{
	pci_proc_detach_bus(pci_bus);

	down_write(&pci_bus_sem);
	list_del(&pci_bus->node);
	up_write(&pci_bus_sem);
	if (!pci_bus->is_added)
		return;

	pci_remove_legacy_files(pci_bus);
	device_unregister(&pci_bus->dev);
}
EXPORT_SYMBOL(pci_remove_bus);

<<<<<<< HEAD
/**
 * pci_remove_bus_device - remove a PCI device and any children
=======
static void __pci_remove_behind_bridge(struct pci_dev *dev);
/**
 * pci_stop_and_remove_bus_device - remove a PCI device and any children
>>>>>>> refs/remotes/origin/cm-10.0
=======
	device_del(&dev->dev);

	down_write(&pci_bus_sem);
	list_del(&dev->bus_list);
	up_write(&pci_bus_sem);

	pci_free_resources(dev);
	put_device(&dev->dev);
}

void pci_remove_bus(struct pci_bus *bus)
{
	pci_proc_detach_bus(bus);

	down_write(&pci_bus_sem);
	list_del(&bus->node);
	pci_bus_release_busn_res(bus);
	up_write(&pci_bus_sem);
	pci_remove_legacy_files(bus);
	pcibios_remove_bus(bus);
	device_unregister(&bus->dev);
}
EXPORT_SYMBOL(pci_remove_bus);

static void pci_stop_bus_device(struct pci_dev *dev)
{
	struct pci_bus *bus = dev->subordinate;
	struct pci_dev *child, *tmp;

	/*
	 * Stopping an SR-IOV PF device removes all the associated VFs,
	 * which will update the bus->devices list and confuse the
	 * iterator.  Therefore, iterate in reverse so we remove the VFs
	 * first, then the PF.
	 */
	if (bus) {
		list_for_each_entry_safe_reverse(child, tmp,
						 &bus->devices, bus_list)
			pci_stop_bus_device(child);
	}

	pci_stop_dev(dev);
}

static void pci_remove_bus_device(struct pci_dev *dev)
{
	struct pci_bus *bus = dev->subordinate;
	struct pci_dev *child, *tmp;

	if (bus) {
		list_for_each_entry_safe(child, tmp,
					 &bus->devices, bus_list)
			pci_remove_bus_device(child);

		pci_remove_bus(bus);
		dev->subordinate = NULL;
	}

	pci_destroy_dev(dev);
}

/**
 * pci_stop_and_remove_bus_device - remove a PCI device and any children
>>>>>>> refs/remotes/origin/master
 * @dev: the device to remove
 *
 * Remove a PCI device from the device lists, informing the drivers
 * that the device has been removed.  We also remove any subordinate
 * buses and children in a depth-first manner.
 *
 * For each device we remove, delete the device structure from the
 * device lists, remove the /proc entry, and notify userspace
 * (/sbin/hotplug).
 */
<<<<<<< HEAD
<<<<<<< HEAD
void pci_remove_bus_device(struct pci_dev *dev)
{
	pci_stop_bus_device(dev);
	if (dev->subordinate) {
		struct pci_bus *b = dev->subordinate;

		pci_remove_behind_bridge(dev);
=======
void __pci_remove_bus_device(struct pci_dev *dev)
{
	if (dev->subordinate) {
		struct pci_bus *b = dev->subordinate;

		__pci_remove_behind_bridge(dev);
>>>>>>> refs/remotes/origin/cm-10.0
		pci_remove_bus(b);
		dev->subordinate = NULL;
	}

	pci_destroy_dev(dev);
}
<<<<<<< HEAD

/**
 * pci_remove_behind_bridge - remove all devices behind a PCI bridge
=======
EXPORT_SYMBOL(__pci_remove_bus_device);

void pci_stop_and_remove_bus_device(struct pci_dev *dev)
{
	pci_stop_bus_device(dev);
	__pci_remove_bus_device(dev);
}

static void __pci_remove_behind_bridge(struct pci_dev *dev)
{
	struct list_head *l, *n;

	if (dev->subordinate)
		list_for_each_safe(l, n, &dev->subordinate->devices)
			__pci_remove_bus_device(pci_dev_b(l));
}

static void pci_stop_behind_bridge(struct pci_dev *dev)
{
	struct list_head *l, *n;

	if (dev->subordinate)
		list_for_each_safe(l, n, &dev->subordinate->devices)
			pci_stop_bus_device(pci_dev_b(l));
}

/**
 * pci_stop_and_remove_behind_bridge - stop and remove all devices behind
 *					 a PCI bridge
>>>>>>> refs/remotes/origin/cm-10.0
 * @dev: PCI bridge device
 *
 * Remove all devices on the bus, except for the parent bridge.
 * This also removes any child buses, and any devices they may
 * contain in a depth-first manner.
 */
<<<<<<< HEAD
void pci_remove_behind_bridge(struct pci_dev *dev)
{
	struct list_head *l, *n;

	if (dev->subordinate)
		list_for_each_safe(l, n, &dev->subordinate->devices)
			pci_remove_bus_device(pci_dev_b(l));
=======
void pci_stop_and_remove_behind_bridge(struct pci_dev *dev)
{
	pci_stop_behind_bridge(dev);
	__pci_remove_behind_bridge(dev);
>>>>>>> refs/remotes/origin/cm-10.0
}

static void pci_stop_bus_devices(struct pci_bus *bus)
{
	struct list_head *l, *n;

<<<<<<< HEAD
	list_for_each_safe(l, n, &bus->devices) {
=======
	/*
	 * VFs could be removed by pci_stop_and_remove_bus_device() in the
	 *  pci_stop_bus_devices() code path for PF.
	 *  aka, bus->devices get updated in the process.
	 * but VFs are inserted after PFs when SRIOV is enabled for PF,
	 * We can iterate the list backwards to get prev valid PF instead
	 *  of removed VF.
	 */
	list_for_each_prev_safe(l, n, &bus->devices) {
>>>>>>> refs/remotes/origin/cm-10.0
		struct pci_dev *dev = pci_dev_b(l);
		pci_stop_bus_device(dev);
	}
}

/**
 * pci_stop_bus_device - stop a PCI device and any children
 * @dev: the device to stop
 *
 * Stop a PCI device (detach the driver, remove from the global list
 * and so on). This also stop any subordinate buses and children in a
 * depth-first manner.
 */
void pci_stop_bus_device(struct pci_dev *dev)
{
	if (dev->subordinate)
		pci_stop_bus_devices(dev->subordinate);

	pci_stop_dev(dev);
}

<<<<<<< HEAD
EXPORT_SYMBOL(pci_remove_bus_device);
EXPORT_SYMBOL(pci_remove_behind_bridge);
=======
EXPORT_SYMBOL(pci_stop_and_remove_bus_device);
EXPORT_SYMBOL(pci_stop_and_remove_behind_bridge);
>>>>>>> refs/remotes/origin/cm-10.0
EXPORT_SYMBOL_GPL(pci_stop_bus_device);
=======
void pci_stop_and_remove_bus_device(struct pci_dev *dev)
{
	pci_stop_bus_device(dev);
	pci_remove_bus_device(dev);
}
EXPORT_SYMBOL(pci_stop_and_remove_bus_device);

void pci_stop_root_bus(struct pci_bus *bus)
{
	struct pci_dev *child, *tmp;
	struct pci_host_bridge *host_bridge;

	if (!pci_is_root_bus(bus))
		return;

	host_bridge = to_pci_host_bridge(bus->bridge);
	list_for_each_entry_safe_reverse(child, tmp,
					 &bus->devices, bus_list)
		pci_stop_bus_device(child);

	/* stop the host bridge */
	device_del(&host_bridge->dev);
}

void pci_remove_root_bus(struct pci_bus *bus)
{
	struct pci_dev *child, *tmp;
	struct pci_host_bridge *host_bridge;

	if (!pci_is_root_bus(bus))
		return;

	host_bridge = to_pci_host_bridge(bus->bridge);
	list_for_each_entry_safe(child, tmp,
				 &bus->devices, bus_list)
		pci_remove_bus_device(child);
	pci_remove_bus(bus);
	host_bridge->bus = NULL;

	/* remove the host bridge */
	put_device(&host_bridge->dev);
}
>>>>>>> refs/remotes/origin/master
