/*
 * File:	pci-acpi.c
 * Purpose:	Provide PCI support in ACPI
 *
 * Copyright (C) 2005 David Shaohua Li <shaohua.li@intel.com>
 * Copyright (C) 2004 Tom Long Nguyen <tom.l.nguyen@intel.com>
 * Copyright (C) 2004 Intel Corp.
 */

#include <linux/delay.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/module.h>
#include <linux/pci-aspm.h>
#include <acpi/acpi.h>
#include <acpi/acpi_bus.h>

#include <linux/pci-acpi.h>
#include <linux/pm_runtime.h>
<<<<<<< HEAD
#include "pci.h"

static DEFINE_MUTEX(pci_acpi_pm_notify_mtx);

=======
#include <linux/pm_qos.h>
#include "pci.h"

>>>>>>> refs/remotes/origin/master
/**
 * pci_acpi_wake_bus - Wake-up notification handler for root buses.
 * @handle: ACPI handle of a device the notification is for.
 * @event: Type of the signaled event.
 * @context: PCI root bus to wake up devices on.
 */
static void pci_acpi_wake_bus(acpi_handle handle, u32 event, void *context)
{
	struct pci_bus *pci_bus = context;

	if (event == ACPI_NOTIFY_DEVICE_WAKE && pci_bus)
		pci_pme_wakeup_bus(pci_bus);
}

/**
 * pci_acpi_wake_dev - Wake-up notification handler for PCI devices.
 * @handle: ACPI handle of a device the notification is for.
 * @event: Type of the signaled event.
 * @context: PCI device object to wake up.
 */
static void pci_acpi_wake_dev(acpi_handle handle, u32 event, void *context)
{
	struct pci_dev *pci_dev = context;

<<<<<<< HEAD
<<<<<<< HEAD
	if (event == ACPI_NOTIFY_DEVICE_WAKE && pci_dev) {
		pci_wakeup_event(pci_dev);
		pci_check_pme_status(pci_dev);
		pm_runtime_resume(&pci_dev->dev);
		if (pci_dev->subordinate)
			pci_pme_wakeup_bus(pci_dev->subordinate);
	}
=======
	if (event != ACPI_NOTIFY_DEVICE_WAKE || !pci_dev)
		return;

	if (!pci_dev->pm_cap || !pci_dev->pme_support
	     || pci_check_pme_status(pci_dev)) {
		if (pci_dev->pme_poll)
			pci_dev->pme_poll = false;

		pci_wakeup_event(pci_dev);
		pm_runtime_resume(&pci_dev->dev);
	}

	if (pci_dev->subordinate)
		pci_pme_wakeup_bus(pci_dev->subordinate);
>>>>>>> refs/remotes/origin/cm-10.0
}

/**
 * add_pm_notifier - Register PM notifier for given ACPI device.
 * @dev: ACPI device to add the notifier for.
 * @context: PCI device or bus to check for PME status if an event is signaled.
 *
 * NOTE: @dev need not be a run-wake or wake-up device to be a valid source of
 * PM wake-up events.  For example, wake-up events may be generated for bridges
 * if one of the devices below the bridge is signaling PME, even if the bridge
 * itself doesn't have a wake-up GPE associated with it.
 */
static acpi_status add_pm_notifier(struct acpi_device *dev,
				   acpi_notify_handler handler,
				   void *context)
{
	acpi_status status = AE_ALREADY_EXISTS;

	mutex_lock(&pci_acpi_pm_notify_mtx);

	if (dev->wakeup.flags.notifier_present)
		goto out;

	status = acpi_install_notify_handler(dev->handle,
					     ACPI_SYSTEM_NOTIFY,
					     handler, context);
	if (ACPI_FAILURE(status))
		goto out;

	dev->wakeup.flags.notifier_present = true;

 out:
	mutex_unlock(&pci_acpi_pm_notify_mtx);
	return status;
}

/**
 * remove_pm_notifier - Unregister PM notifier from given ACPI device.
 * @dev: ACPI device to remove the notifier from.
 */
static acpi_status remove_pm_notifier(struct acpi_device *dev,
				      acpi_notify_handler handler)
{
	acpi_status status = AE_BAD_PARAMETER;

	mutex_lock(&pci_acpi_pm_notify_mtx);

	if (!dev->wakeup.flags.notifier_present)
		goto out;

	status = acpi_remove_notify_handler(dev->handle,
					    ACPI_SYSTEM_NOTIFY,
					    handler);
	if (ACPI_FAILURE(status))
		goto out;

	dev->wakeup.flags.notifier_present = false;

 out:
	mutex_unlock(&pci_acpi_pm_notify_mtx);
	return status;
=======
	if (event != ACPI_NOTIFY_DEVICE_WAKE || !pci_dev)
		return;

	if (pci_dev->pme_poll)
		pci_dev->pme_poll = false;

	if (pci_dev->current_state == PCI_D3cold) {
		pci_wakeup_event(pci_dev);
		pm_runtime_resume(&pci_dev->dev);
		return;
	}

	/* Clear PME Status if set. */
	if (pci_dev->pme_support)
		pci_check_pme_status(pci_dev);

	pci_wakeup_event(pci_dev);
	pm_runtime_resume(&pci_dev->dev);

	if (pci_dev->subordinate)
		pci_pme_wakeup_bus(pci_dev->subordinate);
>>>>>>> refs/remotes/origin/master
}

/**
 * pci_acpi_add_bus_pm_notifier - Register PM notifier for given PCI bus.
 * @dev: ACPI device to add the notifier for.
 * @pci_bus: PCI bus to walk checking for PME status if an event is signaled.
 */
acpi_status pci_acpi_add_bus_pm_notifier(struct acpi_device *dev,
					 struct pci_bus *pci_bus)
{
<<<<<<< HEAD
	return add_pm_notifier(dev, pci_acpi_wake_bus, pci_bus);
=======
	return acpi_add_pm_notifier(dev, pci_acpi_wake_bus, pci_bus);
>>>>>>> refs/remotes/origin/master
}

/**
 * pci_acpi_remove_bus_pm_notifier - Unregister PCI bus PM notifier.
 * @dev: ACPI device to remove the notifier from.
 */
acpi_status pci_acpi_remove_bus_pm_notifier(struct acpi_device *dev)
{
<<<<<<< HEAD
	return remove_pm_notifier(dev, pci_acpi_wake_bus);
=======
	return acpi_remove_pm_notifier(dev, pci_acpi_wake_bus);
>>>>>>> refs/remotes/origin/master
}

/**
 * pci_acpi_add_pm_notifier - Register PM notifier for given PCI device.
 * @dev: ACPI device to add the notifier for.
 * @pci_dev: PCI device to check for the PME status if an event is signaled.
 */
acpi_status pci_acpi_add_pm_notifier(struct acpi_device *dev,
				     struct pci_dev *pci_dev)
{
<<<<<<< HEAD
	return add_pm_notifier(dev, pci_acpi_wake_dev, pci_dev);
=======
	return acpi_add_pm_notifier(dev, pci_acpi_wake_dev, pci_dev);
>>>>>>> refs/remotes/origin/master
}

/**
 * pci_acpi_remove_pm_notifier - Unregister PCI device PM notifier.
 * @dev: ACPI device to remove the notifier from.
 */
acpi_status pci_acpi_remove_pm_notifier(struct acpi_device *dev)
{
<<<<<<< HEAD
	return remove_pm_notifier(dev, pci_acpi_wake_dev);
=======
	return acpi_remove_pm_notifier(dev, pci_acpi_wake_dev);
}

phys_addr_t acpi_pci_root_get_mcfg_addr(acpi_handle handle)
{
	acpi_status status = AE_NOT_EXIST;
	unsigned long long mcfg_addr;

	if (handle)
		status = acpi_evaluate_integer(handle, METHOD_NAME__CBA,
					       NULL, &mcfg_addr);
	if (ACPI_FAILURE(status))
		return 0;

	return (phys_addr_t)mcfg_addr;
>>>>>>> refs/remotes/origin/master
}

/*
 * _SxD returns the D-state with the highest power
 * (lowest D-state number) supported in the S-state "x".
 *
 * If the devices does not have a _PRW
 * (Power Resources for Wake) supporting system wakeup from "x"
 * then the OS is free to choose a lower power (higher number
 * D-state) than the return value from _SxD.
 *
 * But if _PRW is enabled at S-state "x", the OS
 * must not choose a power lower than _SxD --
 * unless the device has an _SxW method specifying
 * the lowest power (highest D-state number) the device
 * may enter while still able to wake the system.
 *
 * ie. depending on global OS policy:
 *
 * if (_PRW at S-state x)
 *	choose from highest power _SxD to lowest power _SxW
 * else // no _PRW at S-state x
<<<<<<< HEAD
 * 	choose highest power _SxD or any lower power
=======
 *	choose highest power _SxD or any lower power
>>>>>>> refs/remotes/origin/master
 */

static pci_power_t acpi_pci_choose_state(struct pci_dev *pdev)
{
<<<<<<< HEAD
	int acpi_state;

	acpi_state = acpi_pm_device_sleep_state(&pdev->dev, NULL);
=======
	int acpi_state, d_max;

	if (pdev->no_d3cold)
		d_max = ACPI_STATE_D3_HOT;
	else
		d_max = ACPI_STATE_D3_COLD;
	acpi_state = acpi_pm_device_sleep_state(&pdev->dev, NULL, d_max);
>>>>>>> refs/remotes/origin/master
	if (acpi_state < 0)
		return PCI_POWER_ERROR;

	switch (acpi_state) {
	case ACPI_STATE_D0:
		return PCI_D0;
	case ACPI_STATE_D1:
		return PCI_D1;
	case ACPI_STATE_D2:
		return PCI_D2;
<<<<<<< HEAD
<<<<<<< HEAD
	case ACPI_STATE_D3:
=======
	case ACPI_STATE_D3_HOT:
>>>>>>> refs/remotes/origin/cm-10.0
=======
	case ACPI_STATE_D3_HOT:
>>>>>>> refs/remotes/origin/master
		return PCI_D3hot;
	case ACPI_STATE_D3_COLD:
		return PCI_D3cold;
	}
	return PCI_POWER_ERROR;
}

static bool acpi_pci_power_manageable(struct pci_dev *dev)
{
<<<<<<< HEAD
	acpi_handle handle = DEVICE_ACPI_HANDLE(&dev->dev);
=======
	acpi_handle handle = ACPI_HANDLE(&dev->dev);
>>>>>>> refs/remotes/origin/master

	return handle ? acpi_bus_power_manageable(handle) : false;
}

static int acpi_pci_set_power_state(struct pci_dev *dev, pci_power_t state)
{
<<<<<<< HEAD
	acpi_handle handle = DEVICE_ACPI_HANDLE(&dev->dev);
	acpi_handle tmp;
=======
	acpi_handle handle = ACPI_HANDLE(&dev->dev);
>>>>>>> refs/remotes/origin/master
	static const u8 state_conv[] = {
		[PCI_D0] = ACPI_STATE_D0,
		[PCI_D1] = ACPI_STATE_D1,
		[PCI_D2] = ACPI_STATE_D2,
<<<<<<< HEAD
		[PCI_D3hot] = ACPI_STATE_D3,
		[PCI_D3cold] = ACPI_STATE_D3
=======
		[PCI_D3hot] = ACPI_STATE_D3_COLD,
		[PCI_D3cold] = ACPI_STATE_D3_COLD,
>>>>>>> refs/remotes/origin/master
	};
	int error = -EINVAL;

	/* If the ACPI device has _EJ0, ignore the device */
<<<<<<< HEAD
	if (!handle || ACPI_SUCCESS(acpi_get_handle(handle, "_EJ0", &tmp)))
		return -ENODEV;

	switch (state) {
=======
	if (!handle || acpi_has_method(handle, "_EJ0"))
		return -ENODEV;

	switch (state) {
	case PCI_D3cold:
		if (dev_pm_qos_flags(&dev->dev, PM_QOS_FLAG_NO_POWER_OFF) ==
				PM_QOS_FLAGS_ALL) {
			error = -EBUSY;
			break;
		}
>>>>>>> refs/remotes/origin/master
	case PCI_D0:
	case PCI_D1:
	case PCI_D2:
	case PCI_D3hot:
<<<<<<< HEAD
	case PCI_D3cold:
=======
>>>>>>> refs/remotes/origin/master
		error = acpi_bus_set_power(handle, state_conv[state]);
	}

	if (!error)
<<<<<<< HEAD
		dev_printk(KERN_INFO, &dev->dev,
				"power state changed by ACPI to D%d\n", state);
=======
		dev_dbg(&dev->dev, "power state changed by ACPI to %s\n",
			 acpi_power_state_string(state_conv[state]));
>>>>>>> refs/remotes/origin/master

	return error;
}

static bool acpi_pci_can_wakeup(struct pci_dev *dev)
{
<<<<<<< HEAD
	acpi_handle handle = DEVICE_ACPI_HANDLE(&dev->dev);
=======
	acpi_handle handle = ACPI_HANDLE(&dev->dev);
>>>>>>> refs/remotes/origin/master

	return handle ? acpi_bus_can_wakeup(handle) : false;
}

static void acpi_pci_propagate_wakeup_enable(struct pci_bus *bus, bool enable)
{
	while (bus->parent) {
		if (!acpi_pm_device_sleep_wake(&bus->self->dev, enable))
			return;
		bus = bus->parent;
	}

	/* We have reached the root bus. */
	if (bus->bridge)
		acpi_pm_device_sleep_wake(bus->bridge, enable);
}

static int acpi_pci_sleep_wake(struct pci_dev *dev, bool enable)
{
	if (acpi_pci_can_wakeup(dev))
		return acpi_pm_device_sleep_wake(&dev->dev, enable);

	acpi_pci_propagate_wakeup_enable(dev->bus, enable);
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
/**
 * acpi_dev_run_wake - Enable/disable wake-up for given device.
 * @phys_dev: Device to enable/disable the platform to wake-up the system for.
 * @enable: Whether enable or disable the wake-up functionality.
 *
 * Find the ACPI device object corresponding to @pci_dev and try to
 * enable/disable the GPE associated with it.
 */
static int acpi_dev_run_wake(struct device *phys_dev, bool enable)
{
	struct acpi_device *dev;
	acpi_handle handle;
	int error = -ENODEV;

	if (!device_run_wake(phys_dev))
		return -EINVAL;

	handle = DEVICE_ACPI_HANDLE(phys_dev);
	if (!handle || ACPI_FAILURE(acpi_bus_get_device(handle, &dev))) {
		dev_dbg(phys_dev, "ACPI handle has no context in %s!\n",
			__func__);
		return -ENODEV;
	}

	if (enable) {
		acpi_enable_wakeup_device_power(dev, ACPI_STATE_S0);
		acpi_enable_gpe(dev->wakeup.gpe_device, dev->wakeup.gpe_number);
	} else {
		acpi_disable_gpe(dev->wakeup.gpe_device, dev->wakeup.gpe_number);
		acpi_disable_wakeup_device_power(dev);
	}

	return error;
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static void acpi_pci_propagate_run_wake(struct pci_bus *bus, bool enable)
{
	while (bus->parent) {
		struct pci_dev *bridge = bus->self;

		if (bridge->pme_interrupt)
			return;
<<<<<<< HEAD
<<<<<<< HEAD
		if (!acpi_dev_run_wake(&bridge->dev, enable))
=======
		if (!acpi_pm_device_run_wake(&bridge->dev, enable))
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (!acpi_pm_device_run_wake(&bridge->dev, enable))
>>>>>>> refs/remotes/origin/master
			return;
		bus = bus->parent;
	}

	/* We have reached the root bus. */
	if (bus->bridge)
<<<<<<< HEAD
<<<<<<< HEAD
		acpi_dev_run_wake(bus->bridge, enable);
=======
		acpi_pm_device_run_wake(bus->bridge, enable);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		acpi_pm_device_run_wake(bus->bridge, enable);
>>>>>>> refs/remotes/origin/master
}

static int acpi_pci_run_wake(struct pci_dev *dev, bool enable)
{
<<<<<<< HEAD
	if (dev->pme_interrupt)
		return 0;

<<<<<<< HEAD
	if (!acpi_dev_run_wake(&dev->dev, enable))
=======
	if (!acpi_pm_device_run_wake(&dev->dev, enable))
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/*
	 * Per PCI Express Base Specification Revision 2.0 section
	 * 5.3.3.2 Link Wakeup, platform support is needed for D3cold
	 * waking up to power on the main link even if there is PME
	 * support for D3cold
	 */
	if (dev->pme_interrupt && !dev->runtime_d3cold)
		return 0;

	if (!acpi_pm_device_run_wake(&dev->dev, enable))
>>>>>>> refs/remotes/origin/master
		return 0;

	acpi_pci_propagate_run_wake(dev->bus, enable);
	return 0;
}

static struct pci_platform_pm_ops acpi_pci_platform_pm = {
	.is_manageable = acpi_pci_power_manageable,
	.set_state = acpi_pci_set_power_state,
	.choose_state = acpi_pci_choose_state,
<<<<<<< HEAD
	.can_wakeup = acpi_pci_can_wakeup,
=======
>>>>>>> refs/remotes/origin/master
	.sleep_wake = acpi_pci_sleep_wake,
	.run_wake = acpi_pci_run_wake,
};

<<<<<<< HEAD
/* ACPI bus type */
static int acpi_pci_find_device(struct device *dev, acpi_handle *handle)
{
	struct pci_dev * pci_dev;
	u64	addr;

	pci_dev = to_pci_dev(dev);
	/* Please ref to ACPI spec for the syntax of _ADR */
	addr = (PCI_SLOT(pci_dev->devfn) << 16) | PCI_FUNC(pci_dev->devfn);
	*handle = acpi_get_child(DEVICE_ACPI_HANDLE(dev->parent), addr);
=======
void acpi_pci_add_bus(struct pci_bus *bus)
{
	if (acpi_pci_disabled || !bus->bridge)
		return;

	acpi_pci_slot_enumerate(bus);
	acpiphp_enumerate_slots(bus);
}

void acpi_pci_remove_bus(struct pci_bus *bus)
{
	if (acpi_pci_disabled || !bus->bridge)
		return;

	acpiphp_remove_slots(bus);
	acpi_pci_slot_remove(bus);
}

/* ACPI bus type */
static int acpi_pci_find_device(struct device *dev, acpi_handle *handle)
{
	struct pci_dev *pci_dev = to_pci_dev(dev);
	bool is_bridge;
	u64 addr;

	/*
	 * pci_is_bridge() is not suitable here, because pci_dev->subordinate
	 * is set only after acpi_pci_find_device() has been called for the
	 * given device.
	 */
	is_bridge = pci_dev->hdr_type == PCI_HEADER_TYPE_BRIDGE
			|| pci_dev->hdr_type == PCI_HEADER_TYPE_CARDBUS;
	/* Please ref to ACPI spec for the syntax of _ADR */
	addr = (PCI_SLOT(pci_dev->devfn) << 16) | PCI_FUNC(pci_dev->devfn);
	*handle = acpi_find_child(ACPI_HANDLE(dev->parent), addr, is_bridge);
>>>>>>> refs/remotes/origin/master
	if (!*handle)
		return -ENODEV;
	return 0;
}

<<<<<<< HEAD
static int acpi_pci_find_root_bridge(struct device *dev, acpi_handle *handle)
{
	int num;
	unsigned int seg, bus;

	/*
	 * The string should be the same as root bridge's name
	 * Please look at 'pci_scan_bus_parented'
	 */
	num = sscanf(dev_name(dev), "pci%04x:%02x", &seg, &bus);
	if (num != 2)
		return -ENODEV;
	*handle = acpi_get_pci_rootbridge_handle(seg, bus);
	if (!*handle)
		return -ENODEV;
	return 0;
}

static struct acpi_bus_type acpi_pci_bus = {
	.bus = &pci_bus_type,
	.find_device = acpi_pci_find_device,
	.find_bridge = acpi_pci_find_root_bridge,
=======
static void pci_acpi_setup(struct device *dev)
{
	struct pci_dev *pci_dev = to_pci_dev(dev);
	struct acpi_device *adev = ACPI_COMPANION(dev);

	if (!adev)
		return;

	pci_acpi_add_pm_notifier(adev, pci_dev);
	if (!adev->wakeup.flags.valid)
		return;

	device_set_wakeup_capable(dev, true);
	acpi_pci_sleep_wake(pci_dev, false);
	if (adev->wakeup.flags.run_wake)
		device_set_run_wake(dev, true);
}

static void pci_acpi_cleanup(struct device *dev)
{
	struct acpi_device *adev = ACPI_COMPANION(dev);

	if (!adev)
		return;

	pci_acpi_remove_pm_notifier(adev);
	if (adev->wakeup.flags.valid) {
		device_set_wakeup_capable(dev, false);
		device_set_run_wake(dev, false);
	}
}

static bool pci_acpi_bus_match(struct device *dev)
{
	return dev->bus == &pci_bus_type;
}

static struct acpi_bus_type acpi_pci_bus = {
	.name = "PCI",
	.match = pci_acpi_bus_match,
	.find_device = acpi_pci_find_device,
	.setup = pci_acpi_setup,
	.cleanup = pci_acpi_cleanup,
>>>>>>> refs/remotes/origin/master
};

static int __init acpi_pci_init(void)
{
	int ret;

	if (acpi_gbl_FADT.boot_flags & ACPI_FADT_NO_MSI) {
<<<<<<< HEAD
		printk(KERN_INFO"ACPI FADT declares the system doesn't support MSI, so disable it\n");
=======
		pr_info("ACPI FADT declares the system doesn't support MSI, so disable it\n");
>>>>>>> refs/remotes/origin/master
		pci_no_msi();
	}

	if (acpi_gbl_FADT.boot_flags & ACPI_FADT_NO_ASPM) {
<<<<<<< HEAD
		printk(KERN_INFO"ACPI FADT declares the system doesn't support PCIe ASPM, so disable it\n");
=======
		pr_info("ACPI FADT declares the system doesn't support PCIe ASPM, so disable it\n");
>>>>>>> refs/remotes/origin/master
		pcie_no_aspm();
	}

	ret = register_acpi_bus_type(&acpi_pci_bus);
	if (ret)
		return 0;
<<<<<<< HEAD
	pci_set_platform_pm(&acpi_pci_platform_pm);
=======

	pci_set_platform_pm(&acpi_pci_platform_pm);
	acpi_pci_slot_init();
	acpiphp_init();

>>>>>>> refs/remotes/origin/master
	return 0;
}
arch_initcall(acpi_pci_init);
