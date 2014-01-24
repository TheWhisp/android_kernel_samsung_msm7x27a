/*
 * PCI Error Recovery Driver for RPA-compliant PPC64 platform.
 * Copyright IBM Corp. 2004 2005
 * Copyright Linas Vepstas <linas@linas.org> 2004, 2005
 *
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
 * NON INFRINGEMENT.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * Send comments and feedback to Linas Vepstas <linas@austin.ibm.com>
 */
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
<<<<<<< HEAD
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/pci.h>
#include <asm/eeh.h>
#include <asm/eeh_event.h>
#include <asm/ppc-pci.h>
#include <asm/pci-bridge.h>
#include <asm/prom.h>
#include <asm/rtas.h>

<<<<<<< HEAD

static inline const char * pcid_name (struct pci_dev *pdev)
=======
/**
 * eeh_pcid_name - Retrieve name of PCI device driver
 * @pdev: PCI device
 *
 * This routine is used to retrieve the name of PCI device driver
 * if that's valid.
 */
static inline const char *eeh_pcid_name(struct pci_dev *pdev)
>>>>>>> refs/remotes/origin/cm-10.0
{
	if (pdev && pdev->dev.driver)
		return pdev->dev.driver->name;
	return "";
}

<<<<<<< HEAD
=======
/**
 * eeh_pcid_get - Get the PCI device driver
 * @pdev: PCI device
 *
 * The function is used to retrieve the PCI device driver for
 * the indicated PCI device. Besides, we will increase the reference
 * of the PCI device driver to prevent that being unloaded on
 * the fly. Otherwise, kernel crash would be seen.
 */
static inline struct pci_driver *eeh_pcid_get(struct pci_dev *pdev)
{
	if (!pdev || !pdev->driver)
		return NULL;

	if (!try_module_get(pdev->driver->driver.owner))
		return NULL;

	return pdev->driver;
}

/**
 * eeh_pcid_put - Dereference on the PCI device driver
 * @pdev: PCI device
 *
 * The function is called to do dereference on the PCI device
 * driver of the indicated PCI device.
 */
static inline void eeh_pcid_put(struct pci_dev *pdev)
{
	if (!pdev || !pdev->driver)
		return;

	module_put(pdev->driver->driver.owner);
}

>>>>>>> refs/remotes/origin/cm-10.0
#if 0
static void print_device_node_tree(struct pci_dn *pdn, int dent)
{
	int i;
	struct device_node *pc;

	if (!pdn)
		return;
	for (i = 0; i < dent; i++)
		printk(" ");
	printk("dn=%s mode=%x \tcfg_addr=%x pe_addr=%x \tfull=%s\n",
		pdn->node->name, pdn->eeh_mode, pdn->eeh_config_addr,
		pdn->eeh_pe_config_addr, pdn->node->full_name);
	dent += 3;
	pc = pdn->node->child;
	while (pc) {
		print_device_node_tree(PCI_DN(pc), dent);
		pc = pc->sibling;
	}
}
#endif

/**
<<<<<<< HEAD
 * eeh_disable_irq - disable interrupt for the recovering device
 */
static void eeh_disable_irq(struct pci_dev *dev)
{
	struct device_node *dn = pci_device_to_OF_node(dev);
=======
 * eeh_disable_irq - Disable interrupt for the recovering device
 * @dev: PCI device
 *
 * This routine must be called when reporting temporary or permanent
 * error to the particular PCI device to disable interrupt of that
 * device. If the device has enabled MSI or MSI-X interrupt, we needn't
 * do real work because EEH should freeze DMA transfers for those PCI
 * devices encountering EEH errors, which includes MSI or MSI-X.
 */
static void eeh_disable_irq(struct pci_dev *dev)
{
	struct eeh_dev *edev = pci_dev_to_eeh_dev(dev);
>>>>>>> refs/remotes/origin/cm-10.0

	/* Don't disable MSI and MSI-X interrupts. They are
	 * effectively disabled by the DMA Stopped state
	 * when an EEH error occurs.
<<<<<<< HEAD
	*/
=======
	 */
>>>>>>> refs/remotes/origin/cm-10.0
	if (dev->msi_enabled || dev->msix_enabled)
		return;

	if (!irq_has_action(dev->irq))
		return;

<<<<<<< HEAD
	PCI_DN(dn)->eeh_mode |= EEH_MODE_IRQ_DISABLED;
=======
	edev->mode |= EEH_MODE_IRQ_DISABLED;
>>>>>>> refs/remotes/origin/cm-10.0
	disable_irq_nosync(dev->irq);
}

/**
<<<<<<< HEAD
 * eeh_enable_irq - enable interrupt for the recovering device
 */
static void eeh_enable_irq(struct pci_dev *dev)
{
	struct device_node *dn = pci_device_to_OF_node(dev);

	if ((PCI_DN(dn)->eeh_mode) & EEH_MODE_IRQ_DISABLED) {
		PCI_DN(dn)->eeh_mode &= ~EEH_MODE_IRQ_DISABLED;
=======
 * eeh_enable_irq - Enable interrupt for the recovering device
 * @dev: PCI device
 *
 * This routine must be called to enable interrupt while failed
 * device could be resumed.
 */
static void eeh_enable_irq(struct pci_dev *dev)
{
	struct eeh_dev *edev = pci_dev_to_eeh_dev(dev);

	if ((edev->mode) & EEH_MODE_IRQ_DISABLED) {
		edev->mode &= ~EEH_MODE_IRQ_DISABLED;
>>>>>>> refs/remotes/origin/cm-10.0
		enable_irq(dev->irq);
	}
}

<<<<<<< HEAD
/* ------------------------------------------------------- */
/**
 * eeh_report_error - report pci error to each device driver
=======
/**
 * eeh_report_error - Report pci error to each device driver
 * @dev: PCI device
 * @userdata: return value
>>>>>>> refs/remotes/origin/cm-10.0
 * 
 * Report an EEH error to each device driver, collect up and 
 * merge the device driver responses. Cumulative response 
 * passed back in "userdata".
 */
<<<<<<< HEAD

static int eeh_report_error(struct pci_dev *dev, void *userdata)
{
	enum pci_ers_result rc, *res = userdata;
	struct pci_driver *driver = dev->driver;

	dev->error_state = pci_channel_io_frozen;

	if (!driver)
		return 0;
=======
static int eeh_report_error(struct pci_dev *dev, void *userdata)
{
	enum pci_ers_result rc, *res = userdata;
	struct pci_driver *driver;

	dev->error_state = pci_channel_io_frozen;

	driver = eeh_pcid_get(dev);
	if (!driver) return 0;
>>>>>>> refs/remotes/origin/cm-10.0

	eeh_disable_irq(dev);

	if (!driver->err_handler ||
<<<<<<< HEAD
	    !driver->err_handler->error_detected)
		return 0;

	rc = driver->err_handler->error_detected (dev, pci_channel_io_frozen);
=======
	    !driver->err_handler->error_detected) {
		eeh_pcid_put(dev);
		return 0;
	}

	rc = driver->err_handler->error_detected(dev, pci_channel_io_frozen);
>>>>>>> refs/remotes/origin/cm-10.0

	/* A driver that needs a reset trumps all others */
	if (rc == PCI_ERS_RESULT_NEED_RESET) *res = rc;
	if (*res == PCI_ERS_RESULT_NONE) *res = rc;

<<<<<<< HEAD
=======
	eeh_pcid_put(dev);
>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
}

/**
<<<<<<< HEAD
 * eeh_report_mmio_enabled - tell drivers that MMIO has been enabled
=======
 * eeh_report_mmio_enabled - Tell drivers that MMIO has been enabled
 * @dev: PCI device
 * @userdata: return value
>>>>>>> refs/remotes/origin/cm-10.0
 *
 * Tells each device driver that IO ports, MMIO and config space I/O
 * are now enabled. Collects up and merges the device driver responses.
 * Cumulative response passed back in "userdata".
 */
<<<<<<< HEAD

static int eeh_report_mmio_enabled(struct pci_dev *dev, void *userdata)
{
	enum pci_ers_result rc, *res = userdata;
	struct pci_driver *driver = dev->driver;

	if (!driver ||
	    !driver->err_handler ||
	    !driver->err_handler->mmio_enabled)
		return 0;

	rc = driver->err_handler->mmio_enabled (dev);
=======
static int eeh_report_mmio_enabled(struct pci_dev *dev, void *userdata)
{
	enum pci_ers_result rc, *res = userdata;
	struct pci_driver *driver;

	driver = eeh_pcid_get(dev);
	if (!driver) return 0;

	if (!driver->err_handler ||
	    !driver->err_handler->mmio_enabled) {
		eeh_pcid_put(dev);
		return 0;
	}

	rc = driver->err_handler->mmio_enabled(dev);
>>>>>>> refs/remotes/origin/cm-10.0

	/* A driver that needs a reset trumps all others */
	if (rc == PCI_ERS_RESULT_NEED_RESET) *res = rc;
	if (*res == PCI_ERS_RESULT_NONE) *res = rc;

<<<<<<< HEAD
=======
	eeh_pcid_put(dev);
>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
}

/**
<<<<<<< HEAD
 * eeh_report_reset - tell device that slot has been reset
 */

static int eeh_report_reset(struct pci_dev *dev, void *userdata)
{
	enum pci_ers_result rc, *res = userdata;
	struct pci_driver *driver = dev->driver;

	if (!driver)
		return 0;

	dev->error_state = pci_channel_io_normal;

	eeh_enable_irq(dev);

	if (!driver->err_handler ||
	    !driver->err_handler->slot_reset)
		return 0;
=======
 * eeh_report_reset - Tell device that slot has been reset
 * @dev: PCI device
 * @userdata: return value
 *
 * This routine must be called while EEH tries to reset particular
 * PCI device so that the associated PCI device driver could take
 * some actions, usually to save data the driver needs so that the
 * driver can work again while the device is recovered.
 */
static int eeh_report_reset(struct pci_dev *dev, void *userdata)
{
	enum pci_ers_result rc, *res = userdata;
	struct pci_driver *driver;

	dev->error_state = pci_channel_io_normal;

	driver = eeh_pcid_get(dev);
	if (!driver) return 0;

	eeh_enable_irq(dev);

	if (!driver->err_handler ||
	    !driver->err_handler->slot_reset) {
		eeh_pcid_put(dev);
		return 0;
	}
>>>>>>> refs/remotes/origin/cm-10.0

	rc = driver->err_handler->slot_reset(dev);
	if ((*res == PCI_ERS_RESULT_NONE) ||
	    (*res == PCI_ERS_RESULT_RECOVERED)) *res = rc;
	if (*res == PCI_ERS_RESULT_DISCONNECT &&
	     rc == PCI_ERS_RESULT_NEED_RESET) *res = rc;

<<<<<<< HEAD
=======
	eeh_pcid_put(dev);
>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
}

/**
<<<<<<< HEAD
 * eeh_report_resume - tell device to resume normal operations
 */

static int eeh_report_resume(struct pci_dev *dev, void *userdata)
{
	struct pci_driver *driver = dev->driver;

	dev->error_state = pci_channel_io_normal;

	if (!driver)
		return 0;
=======
 * eeh_report_resume - Tell device to resume normal operations
 * @dev: PCI device
 * @userdata: return value
 *
 * This routine must be called to notify the device driver that it
 * could resume so that the device driver can do some initialization
 * to make the recovered device work again.
 */
static int eeh_report_resume(struct pci_dev *dev, void *userdata)
{
	struct pci_driver *driver;

	dev->error_state = pci_channel_io_normal;

	driver = eeh_pcid_get(dev);
	if (!driver) return 0;
>>>>>>> refs/remotes/origin/cm-10.0

	eeh_enable_irq(dev);

	if (!driver->err_handler ||
<<<<<<< HEAD
	    !driver->err_handler->resume)
		return 0;

	driver->err_handler->resume(dev);

=======
	    !driver->err_handler->resume) {
		eeh_pcid_put(dev);
		return 0;
	}

	driver->err_handler->resume(dev);

	eeh_pcid_put(dev);
>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
}

/**
<<<<<<< HEAD
 * eeh_report_failure - tell device driver that device is dead.
=======
 * eeh_report_failure - Tell device driver that device is dead.
 * @dev: PCI device
 * @userdata: return value
>>>>>>> refs/remotes/origin/cm-10.0
 *
 * This informs the device driver that the device is permanently
 * dead, and that no further recovery attempts will be made on it.
 */
<<<<<<< HEAD

static int eeh_report_failure(struct pci_dev *dev, void *userdata)
{
	struct pci_driver *driver = dev->driver;

	dev->error_state = pci_channel_io_perm_failure;

	if (!driver)
		return 0;
=======
static int eeh_report_failure(struct pci_dev *dev, void *userdata)
{
	struct pci_driver *driver;

	dev->error_state = pci_channel_io_perm_failure;

	driver = eeh_pcid_get(dev);
	if (!driver) return 0;
>>>>>>> refs/remotes/origin/cm-10.0

	eeh_disable_irq(dev);

	if (!driver->err_handler ||
<<<<<<< HEAD
	    !driver->err_handler->error_detected)
		return 0;

	driver->err_handler->error_detected(dev, pci_channel_io_perm_failure);

	return 0;
}

/* ------------------------------------------------------- */
/**
 * handle_eeh_events -- reset a PCI device after hard lockup.
 *
 * pSeries systems will isolate a PCI slot if the PCI-Host
 * bridge detects address or data parity errors, DMA's
 * occurring to wild addresses (which usually happen due to
 * bugs in device drivers or in PCI adapter firmware).
 * Slot isolations also occur if #SERR, #PERR or other misc
 * PCI-related errors are detected.
 *
 * Recovery process consists of unplugging the device driver
 * (which generated hotplug events to userspace), then issuing
 * a PCI #RST to the device, then reconfiguring the PCI config
 * space for all bridges & devices under this slot, and then
 * finally restarting the device drivers (which cause a second
 * set of hotplug events to go out to userspace).
 */

/**
 * eeh_reset_device() -- perform actual reset of a pci slot
 * @bus: pointer to the pci bus structure corresponding
 *            to the isolated slot. A non-null value will
 *            cause all devices under the bus to be removed
 *            and then re-added.
 * @pe_dn: pointer to a "Partionable Endpoint" device node.
 *            This is the top-level structure on which pci
 *            bus resets can be performed.
 */

static int eeh_reset_device (struct pci_dn *pe_dn, struct pci_bus *bus)
=======
	    !driver->err_handler->error_detected) {
		eeh_pcid_put(dev);
		return 0;
	}

	driver->err_handler->error_detected(dev, pci_channel_io_perm_failure);

	eeh_pcid_put(dev);
	return 0;
}

/**
 * eeh_reset_device - Perform actual reset of a pci slot
 * @edev: PE associated EEH device
 * @bus: PCI bus corresponding to the isolcated slot
 *
 * This routine must be called to do reset on the indicated PE.
 * During the reset, udev might be invoked because those affected
 * PCI devices will be removed and then added.
 */
static int eeh_reset_device(struct eeh_dev *edev, struct pci_bus *bus)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct device_node *dn;
	int cnt, rc;

	/* pcibios will clear the counter; save the value */
<<<<<<< HEAD
	cnt = pe_dn->eeh_freeze_count;
=======
	cnt = edev->freeze_count;
>>>>>>> refs/remotes/origin/cm-10.0

	if (bus)
		pcibios_remove_pci_devices(bus);

	/* Reset the pci controller. (Asserts RST#; resets config space).
	 * Reconfigure bridges and devices. Don't try to bring the system
<<<<<<< HEAD
	 * up if the reset failed for some reason. */
	rc = rtas_set_slot_reset(pe_dn);
	if (rc)
		return rc;

	/* Walk over all functions on this device.  */
	dn = pe_dn->node;
	if (!pcibios_find_pci_bus(dn) && PCI_DN(dn->parent))
		dn = dn->parent->child;

	while (dn) {
		struct pci_dn *ppe = PCI_DN(dn);
		/* On Power4, always true because eeh_pe_config_addr=0 */
		if (pe_dn->eeh_pe_config_addr == ppe->eeh_pe_config_addr) {
			rtas_configure_bridge(ppe);
			eeh_restore_bars(ppe);
=======
	 * up if the reset failed for some reason.
	 */
	rc = eeh_reset_pe(edev);
	if (rc)
		return rc;

	/* Walk over all functions on this device. */
	dn = eeh_dev_to_of_node(edev);
	if (!pcibios_find_pci_bus(dn) && of_node_to_eeh_dev(dn->parent))
		dn = dn->parent->child;

	while (dn) {
		struct eeh_dev *pedev = of_node_to_eeh_dev(dn);

		/* On Power4, always true because eeh_pe_config_addr=0 */
		if (edev->pe_config_addr == pedev->pe_config_addr) {
			eeh_ops->configure_bridge(dn);
			eeh_restore_bars(pedev);
>>>>>>> refs/remotes/origin/cm-10.0
 		}
		dn = dn->sibling;
	}

	/* Give the system 5 seconds to finish running the user-space
	 * hotplug shutdown scripts, e.g. ifdown for ethernet.  Yes, 
	 * this is a hack, but if we don't do this, and try to bring 
	 * the device up before the scripts have taken it down, 
	 * potentially weird things happen.
	 */
	if (bus) {
<<<<<<< HEAD
		ssleep (5);
		pcibios_add_pci_devices(bus);
	}
	pe_dn->eeh_freeze_count = cnt;
=======
		ssleep(5);
		pcibios_add_pci_devices(bus);
	}
	edev->freeze_count = cnt;
>>>>>>> refs/remotes/origin/cm-10.0

	return 0;
}

/* The longest amount of time to wait for a pci device
 * to come back on line, in seconds.
 */
#define MAX_WAIT_FOR_RECOVERY 150

<<<<<<< HEAD
struct pci_dn * handle_eeh_events (struct eeh_event *event)
{
	struct device_node *frozen_dn;
	struct pci_dn *frozen_pdn;
=======
/**
 * eeh_handle_event - Reset a PCI device after hard lockup.
 * @event: EEH event
 *
 * While PHB detects address or data parity errors on particular PCI
 * slot, the associated PE will be frozen. Besides, DMA's occurring
 * to wild addresses (which usually happen due to bugs in device
 * drivers or in PCI adapter firmware) can cause EEH error. #SERR,
 * #PERR or other misc PCI-related errors also can trigger EEH errors.
 *
 * Recovery process consists of unplugging the device driver (which
 * generated hotplug events to userspace), then issuing a PCI #RST to
 * the device, then reconfiguring the PCI config space for all bridges
 * & devices under this slot, and then finally restarting the device
 * drivers (which cause a second set of hotplug events to go out to
 * userspace).
 */
struct eeh_dev *handle_eeh_events(struct eeh_event *event)
{
	struct device_node *frozen_dn;
	struct eeh_dev *frozen_edev;
>>>>>>> refs/remotes/origin/cm-10.0
	struct pci_bus *frozen_bus;
	int rc = 0;
	enum pci_ers_result result = PCI_ERS_RESULT_NONE;
	const char *location, *pci_str, *drv_str, *bus_pci_str, *bus_drv_str;

<<<<<<< HEAD
	frozen_dn = find_device_pe(event->dn);
	if (!frozen_dn) {

		location = of_get_property(event->dn, "ibm,loc-code", NULL);
		location = location ? location : "unknown";
		printk(KERN_ERR "EEH: Error: Cannot find partition endpoint "
		                "for location=%s pci addr=%s\n",
		        location, eeh_pci_name(event->dev));
=======
	frozen_dn = eeh_find_device_pe(eeh_dev_to_of_node(event->edev));
	if (!frozen_dn) {
		location = of_get_property(eeh_dev_to_of_node(event->edev), "ibm,loc-code", NULL);
		location = location ? location : "unknown";
		printk(KERN_ERR "EEH: Error: Cannot find partition endpoint "
		                "for location=%s pci addr=%s\n",
			location, eeh_pci_name(eeh_dev_to_pci_dev(event->edev)));
>>>>>>> refs/remotes/origin/cm-10.0
		return NULL;
	}

	frozen_bus = pcibios_find_pci_bus(frozen_dn);
	location = of_get_property(frozen_dn, "ibm,loc-code", NULL);
	location = location ? location : "unknown";

	/* There are two different styles for coming up with the PE.
	 * In the old style, it was the highest EEH-capable device
	 * which was always an EADS pci bridge.  In the new style,
	 * there might not be any EADS bridges, and even when there are,
	 * the firmware marks them as "EEH incapable". So another
<<<<<<< HEAD
	 * two-step is needed to find the pci bus.. */
	if (!frozen_bus)
		frozen_bus = pcibios_find_pci_bus (frozen_dn->parent);
=======
	 * two-step is needed to find the pci bus..
	 */
	if (!frozen_bus)
		frozen_bus = pcibios_find_pci_bus(frozen_dn->parent);
>>>>>>> refs/remotes/origin/cm-10.0

	if (!frozen_bus) {
		printk(KERN_ERR "EEH: Cannot find PCI bus "
		        "for location=%s dn=%s\n",
		        location, frozen_dn->full_name);
		return NULL;
	}

<<<<<<< HEAD
	frozen_pdn = PCI_DN(frozen_dn);
	frozen_pdn->eeh_freeze_count++;

	pci_str = eeh_pci_name(event->dev);
	drv_str = pcid_name(event->dev);
	
	if (frozen_pdn->eeh_freeze_count > EEH_MAX_ALLOWED_FREEZES)
=======
	frozen_edev = of_node_to_eeh_dev(frozen_dn);
	frozen_edev->freeze_count++;
	pci_str = eeh_pci_name(eeh_dev_to_pci_dev(event->edev));
	drv_str = eeh_pcid_name(eeh_dev_to_pci_dev(event->edev));

	if (frozen_edev->freeze_count > EEH_MAX_ALLOWED_FREEZES)
>>>>>>> refs/remotes/origin/cm-10.0
		goto excess_failures;

	printk(KERN_WARNING
	   "EEH: This PCI device has failed %d times in the last hour:\n",
<<<<<<< HEAD
		frozen_pdn->eeh_freeze_count);

	if (frozen_pdn->pcidev) {
		bus_pci_str = pci_name(frozen_pdn->pcidev);
		bus_drv_str = pcid_name(frozen_pdn->pcidev);
=======
		frozen_edev->freeze_count);

	if (frozen_edev->pdev) {
		bus_pci_str = pci_name(frozen_edev->pdev);
		bus_drv_str = eeh_pcid_name(frozen_edev->pdev);
>>>>>>> refs/remotes/origin/cm-10.0
		printk(KERN_WARNING
			"EEH: Bus location=%s driver=%s pci addr=%s\n",
			location, bus_drv_str, bus_pci_str);
	}

	printk(KERN_WARNING
		"EEH: Device location=%s driver=%s pci addr=%s\n",
		location, drv_str, pci_str);

	/* Walk the various device drivers attached to this slot through
	 * a reset sequence, giving each an opportunity to do what it needs
	 * to accomplish the reset.  Each child gets a report of the
	 * status ... if any child can't handle the reset, then the entire
	 * slot is dlpar removed and added.
	 */
	pci_walk_bus(frozen_bus, eeh_report_error, &result);

	/* Get the current PCI slot state. This can take a long time,
<<<<<<< HEAD
	 * sometimes over 3 seconds for certain systems. */
	rc = eeh_wait_for_slot_status (frozen_pdn, MAX_WAIT_FOR_RECOVERY*1000);
	if (rc < 0) {
=======
	 * sometimes over 3 seconds for certain systems.
	 */
	rc = eeh_ops->wait_state(eeh_dev_to_of_node(frozen_edev), MAX_WAIT_FOR_RECOVERY*1000);
	if (rc < 0 || rc == EEH_STATE_NOT_SUPPORT) {
>>>>>>> refs/remotes/origin/cm-10.0
		printk(KERN_WARNING "EEH: Permanent failure\n");
		goto hard_fail;
	}

	/* Since rtas may enable MMIO when posting the error log,
	 * don't post the error log until after all dev drivers
	 * have been informed.
	 */
<<<<<<< HEAD
	eeh_slot_error_detail(frozen_pdn, EEH_LOG_TEMP_FAILURE);
=======
	eeh_slot_error_detail(frozen_edev, EEH_LOG_TEMP);
>>>>>>> refs/remotes/origin/cm-10.0

	/* If all device drivers were EEH-unaware, then shut
	 * down all of the device drivers, and hope they
	 * go down willingly, without panicing the system.
	 */
	if (result == PCI_ERS_RESULT_NONE) {
<<<<<<< HEAD
		rc = eeh_reset_device(frozen_pdn, frozen_bus);
=======
		rc = eeh_reset_device(frozen_edev, frozen_bus);
>>>>>>> refs/remotes/origin/cm-10.0
		if (rc) {
			printk(KERN_WARNING "EEH: Unable to reset, rc=%d\n", rc);
			goto hard_fail;
		}
	}

	/* If all devices reported they can proceed, then re-enable MMIO */
	if (result == PCI_ERS_RESULT_CAN_RECOVER) {
<<<<<<< HEAD
		rc = rtas_pci_enable(frozen_pdn, EEH_THAW_MMIO);
=======
		rc = eeh_pci_enable(frozen_edev, EEH_OPT_THAW_MMIO);
>>>>>>> refs/remotes/origin/cm-10.0

		if (rc < 0)
			goto hard_fail;
		if (rc) {
			result = PCI_ERS_RESULT_NEED_RESET;
		} else {
			result = PCI_ERS_RESULT_NONE;
			pci_walk_bus(frozen_bus, eeh_report_mmio_enabled, &result);
		}
	}

	/* If all devices reported they can proceed, then re-enable DMA */
	if (result == PCI_ERS_RESULT_CAN_RECOVER) {
<<<<<<< HEAD
		rc = rtas_pci_enable(frozen_pdn, EEH_THAW_DMA);
=======
		rc = eeh_pci_enable(frozen_edev, EEH_OPT_THAW_DMA);
>>>>>>> refs/remotes/origin/cm-10.0

		if (rc < 0)
			goto hard_fail;
		if (rc)
			result = PCI_ERS_RESULT_NEED_RESET;
		else
			result = PCI_ERS_RESULT_RECOVERED;
	}

	/* If any device has a hard failure, then shut off everything. */
	if (result == PCI_ERS_RESULT_DISCONNECT) {
		printk(KERN_WARNING "EEH: Device driver gave up\n");
		goto hard_fail;
	}

	/* If any device called out for a reset, then reset the slot */
	if (result == PCI_ERS_RESULT_NEED_RESET) {
<<<<<<< HEAD
		rc = eeh_reset_device(frozen_pdn, NULL);
=======
		rc = eeh_reset_device(frozen_edev, NULL);
>>>>>>> refs/remotes/origin/cm-10.0
		if (rc) {
			printk(KERN_WARNING "EEH: Cannot reset, rc=%d\n", rc);
			goto hard_fail;
		}
		result = PCI_ERS_RESULT_NONE;
		pci_walk_bus(frozen_bus, eeh_report_reset, &result);
	}

	/* All devices should claim they have recovered by now. */
	if ((result != PCI_ERS_RESULT_RECOVERED) &&
	    (result != PCI_ERS_RESULT_NONE)) {
		printk(KERN_WARNING "EEH: Not recovered\n");
		goto hard_fail;
	}

	/* Tell all device drivers that they can resume operations */
	pci_walk_bus(frozen_bus, eeh_report_resume, NULL);

<<<<<<< HEAD
	return frozen_pdn;
=======
	return frozen_edev;
>>>>>>> refs/remotes/origin/cm-10.0
	
excess_failures:
	/*
	 * About 90% of all real-life EEH failures in the field
	 * are due to poorly seated PCI cards. Only 10% or so are
	 * due to actual, failed cards.
	 */
	printk(KERN_ERR
	   "EEH: PCI device at location=%s driver=%s pci addr=%s\n"
		"has failed %d times in the last hour "
		"and has been permanently disabled.\n"
		"Please try reseating this device or replacing it.\n",
<<<<<<< HEAD
		location, drv_str, pci_str, frozen_pdn->eeh_freeze_count);
=======
		location, drv_str, pci_str, frozen_edev->freeze_count);
>>>>>>> refs/remotes/origin/cm-10.0
	goto perm_error;

hard_fail:
	printk(KERN_ERR
	   "EEH: Unable to recover from failure of PCI device "
	   "at location=%s driver=%s pci addr=%s\n"
	   "Please try reseating this device or replacing it.\n",
		location, drv_str, pci_str);

perm_error:
<<<<<<< HEAD
	eeh_slot_error_detail(frozen_pdn, EEH_LOG_PERM_FAILURE);
=======
	eeh_slot_error_detail(frozen_edev, EEH_LOG_PERM);
>>>>>>> refs/remotes/origin/cm-10.0

	/* Notify all devices that they're about to go down. */
	pci_walk_bus(frozen_bus, eeh_report_failure, NULL);

	/* Shut down the device drivers for good. */
	pcibios_remove_pci_devices(frozen_bus);

	return NULL;
}

<<<<<<< HEAD
/* ---------- end of file ---------- */
=======
>>>>>>> refs/remotes/origin/cm-10.0
