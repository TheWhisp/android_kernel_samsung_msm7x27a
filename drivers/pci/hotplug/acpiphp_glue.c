/*
 * ACPI PCI HotPlug glue functions to ACPI CA subsystem
 *
 * Copyright (C) 2002,2003 Takayoshi Kochi (t-kochi@bq.jp.nec.com)
 * Copyright (C) 2002 Hiroshi Aono (h-aono@ap.jp.nec.com)
 * Copyright (C) 2002,2003 NEC Corporation
 * Copyright (C) 2003-2005 Matthew Wilcox (matthew.wilcox@hp.com)
 * Copyright (C) 2003-2005 Hewlett Packard
 * Copyright (C) 2005 Rajesh Shah (rajesh.shah@intel.com)
 * Copyright (C) 2005 Intel Corporation
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
 * Send feedback to <kristen.c.accardi@intel.com>
 *
 */

/*
 * Lifetime rules for pci_dev:
 *  - The one in acpiphp_bridge has its refcount elevated by pci_get_slot()
 *    when the bridge is scanned and it loses a refcount when the bridge
 *    is removed.
 *  - When a P2P bridge is present, we elevate the refcount on the subordinate
 *    bus. It loses the refcount when the the driver unloads.
 */

<<<<<<< HEAD
=======
#define pr_fmt(fmt) "acpiphp_glue: " fmt

>>>>>>> refs/remotes/origin/master
#include <linux/init.h>
#include <linux/module.h>

#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/pci_hotplug.h>
#include <linux/pci-acpi.h>
<<<<<<< HEAD
#include <linux/mutex.h>
#include <linux/slab.h>
<<<<<<< HEAD
=======
#include <linux/acpi.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/pm_runtime.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/acpi.h>
>>>>>>> refs/remotes/origin/master

#include "../pci.h"
#include "acpiphp.h"

static LIST_HEAD(bridge_list);
<<<<<<< HEAD

#define MY_NAME "acpiphp_glue"

static void handle_hotplug_event_bridge (acpi_handle, u32, void *);
static void acpiphp_sanitize_bus(struct pci_bus *bus);
static void acpiphp_set_hpp_values(struct pci_bus *bus);
static void handle_hotplug_event_func(acpi_handle handle, u32 type, void *context);

/* callback routine to check for the existence of a pci dock device */
static acpi_status
is_pci_dock_device(acpi_handle handle, u32 lvl, void *context, void **rv)
{
	int *count = (int *)context;

	if (is_dock_device(handle)) {
		(*count)++;
		return AE_CTRL_TERMINATE;
	} else {
		return AE_OK;
	}
=======
static DEFINE_MUTEX(bridge_mutex);
static DEFINE_MUTEX(acpiphp_context_lock);

static void handle_hotplug_event(acpi_handle handle, u32 type, void *data);
static void acpiphp_sanitize_bus(struct pci_bus *bus);
static void acpiphp_set_hpp_values(struct pci_bus *bus);
static void hotplug_event(acpi_handle handle, u32 type, void *data);
static void free_bridge(struct kref *kref);

static void acpiphp_context_handler(acpi_handle handle, void *context)
{
	/* Intentionally empty. */
}

/**
 * acpiphp_init_context - Create hotplug context and grab a reference to it.
 * @handle: ACPI object handle to create the context for.
 *
 * Call under acpiphp_context_lock.
 */
static struct acpiphp_context *acpiphp_init_context(acpi_handle handle)
{
	struct acpiphp_context *context;
	acpi_status status;

	context = kzalloc(sizeof(*context), GFP_KERNEL);
	if (!context)
		return NULL;

	context->handle = handle;
	context->refcount = 1;
	status = acpi_attach_data(handle, acpiphp_context_handler, context);
	if (ACPI_FAILURE(status)) {
		kfree(context);
		return NULL;
	}
	return context;
}

/**
 * acpiphp_get_context - Get hotplug context and grab a reference to it.
 * @handle: ACPI object handle to get the context for.
 *
 * Call under acpiphp_context_lock.
 */
static struct acpiphp_context *acpiphp_get_context(acpi_handle handle)
{
	struct acpiphp_context *context = NULL;
	acpi_status status;
	void *data;

	status = acpi_get_data(handle, acpiphp_context_handler, &data);
	if (ACPI_SUCCESS(status)) {
		context = data;
		context->refcount++;
	}
	return context;
}

/**
 * acpiphp_put_context - Drop a reference to ACPI hotplug context.
 * @handle: ACPI object handle to put the context for.
 *
 * The context object is removed if there are no more references to it.
 *
 * Call under acpiphp_context_lock.
 */
static void acpiphp_put_context(struct acpiphp_context *context)
{
	if (--context->refcount)
		return;

	WARN_ON(context->bridge);
	acpi_detach_data(context->handle, acpiphp_context_handler);
	kfree(context);
}

static inline void get_bridge(struct acpiphp_bridge *bridge)
{
	kref_get(&bridge->ref);
}

static inline void put_bridge(struct acpiphp_bridge *bridge)
{
	kref_put(&bridge->ref, free_bridge);
}

static void free_bridge(struct kref *kref)
{
	struct acpiphp_context *context;
	struct acpiphp_bridge *bridge;
	struct acpiphp_slot *slot, *next;
	struct acpiphp_func *func, *tmp;

	mutex_lock(&acpiphp_context_lock);

	bridge = container_of(kref, struct acpiphp_bridge, ref);

	list_for_each_entry_safe(slot, next, &bridge->slots, node) {
		list_for_each_entry_safe(func, tmp, &slot->funcs, sibling)
			acpiphp_put_context(func_to_context(func));

		kfree(slot);
	}

	context = bridge->context;
	/* Root bridges will not have hotplug context. */
	if (context) {
		/* Release the reference taken by acpiphp_enumerate_slots(). */
		put_bridge(context->func.parent);
		context->bridge = NULL;
		acpiphp_put_context(context);
	}

	put_device(&bridge->pci_bus->dev);
	pci_dev_put(bridge->pci_dev);
	kfree(bridge);

	mutex_unlock(&acpiphp_context_lock);
>>>>>>> refs/remotes/origin/master
}

/*
 * the _DCK method can do funny things... and sometimes not
 * hah-hah funny.
 *
 * TBD - figure out a way to only call fixups for
 * systems that require them.
 */
<<<<<<< HEAD
static int post_dock_fixups(struct notifier_block *nb, unsigned long val,
	void *v)
{
	struct acpiphp_func *func = container_of(nb, struct acpiphp_func, nb);
	struct pci_bus *bus = func->slot->bridge->pci_bus;
	u32 buses;

	if (!bus->self)
		return  NOTIFY_OK;
=======
static void post_dock_fixups(acpi_handle not_used, u32 event, void *data)
{
	struct acpiphp_context *context = data;
	struct pci_bus *bus = context->func.slot->bus;
	u32 buses;

	if (!bus->self)
		return;
>>>>>>> refs/remotes/origin/master

	/* fixup bad _DCK function that rewrites
	 * secondary bridge on slot
	 */
	pci_read_config_dword(bus->self,
			PCI_PRIMARY_BUS,
			&buses);

<<<<<<< HEAD
	if (((buses >> 8) & 0xff) != bus->secondary) {
		buses = (buses & 0xff000000)
			| ((unsigned int)(bus->primary)     <<  0)
			| ((unsigned int)(bus->secondary)   <<  8)
			| ((unsigned int)(bus->subordinate) << 16);
		pci_write_config_dword(bus->self, PCI_PRIMARY_BUS, buses);
	}
	return NOTIFY_OK;
}


<<<<<<< HEAD
static struct acpi_dock_ops acpiphp_dock_ops = {
=======
static const struct acpi_dock_ops acpiphp_dock_ops = {
>>>>>>> refs/remotes/origin/cm-10.0
	.handler = handle_hotplug_event_func,
};

/* callback routine to register each ACPI PCI slot object */
static acpi_status
register_slot(acpi_handle handle, u32 lvl, void *context, void **rv)
{
	struct acpiphp_bridge *bridge = (struct acpiphp_bridge *)context;
	struct acpiphp_slot *slot;
	struct acpiphp_func *newfunc;
	acpi_handle tmp;
	acpi_status status = AE_OK;
	unsigned long long adr, sun;
	int device, function, retval;
	struct pci_bus *pbus = bridge->pci_bus;
	struct pci_dev *pdev;

	if (!acpi_pci_check_ejectable(pbus, handle) && !is_dock_device(handle))
=======
	if (((buses >> 8) & 0xff) != bus->busn_res.start) {
		buses = (buses & 0xff000000)
			| ((unsigned int)(bus->primary)     <<  0)
			| ((unsigned int)(bus->busn_res.start)   <<  8)
			| ((unsigned int)(bus->busn_res.end) << 16);
		pci_write_config_dword(bus->self, PCI_PRIMARY_BUS, buses);
	}
}


static const struct acpi_dock_ops acpiphp_dock_ops = {
	.fixup = post_dock_fixups,
	.handler = hotplug_event,
};

/* Check whether the PCI device is managed by native PCIe hotplug driver */
static bool device_is_managed_by_native_pciehp(struct pci_dev *pdev)
{
	u32 reg32;
	acpi_handle tmp;
	struct acpi_pci_root *root;

	/* Check whether the PCIe port supports native PCIe hotplug */
	if (pcie_capability_read_dword(pdev, PCI_EXP_SLTCAP, &reg32))
		return false;
	if (!(reg32 & PCI_EXP_SLTCAP_HPC))
		return false;

	/*
	 * Check whether native PCIe hotplug has been enabled for
	 * this PCIe hierarchy.
	 */
	tmp = acpi_find_root_bridge_handle(pdev);
	if (!tmp)
		return false;
	root = acpi_pci_find_root(tmp);
	if (!root)
		return false;
	if (!(root->osc_control_set & OSC_PCI_EXPRESS_NATIVE_HP_CONTROL))
		return false;

	return true;
}

static void acpiphp_dock_init(void *data)
{
	struct acpiphp_context *context = data;

	get_bridge(context->func.parent);
}

static void acpiphp_dock_release(void *data)
{
	struct acpiphp_context *context = data;

	put_bridge(context->func.parent);
}

/* callback routine to register each ACPI PCI slot object */
static acpi_status register_slot(acpi_handle handle, u32 lvl, void *data,
				 void **rv)
{
	struct acpiphp_bridge *bridge = data;
	struct acpiphp_context *context;
	struct acpiphp_slot *slot;
	struct acpiphp_func *newfunc;
	acpi_status status = AE_OK;
	unsigned long long adr;
	int device, function;
	struct pci_bus *pbus = bridge->pci_bus;
	struct pci_dev *pdev = bridge->pci_dev;
	u32 val;

	if (pdev && device_is_managed_by_native_pciehp(pdev))
>>>>>>> refs/remotes/origin/master
		return AE_OK;

	status = acpi_evaluate_integer(handle, "_ADR", NULL, &adr);
	if (ACPI_FAILURE(status)) {
<<<<<<< HEAD
<<<<<<< HEAD
		warn("can't evaluate _ADR (%#x)\n", status);
=======
		if (status != AE_NOT_FOUND)
			acpi_handle_warn(handle,
				"can't evaluate _ADR (%#x)\n", status);
>>>>>>> refs/remotes/origin/master
=======
		warn("can't evaluate _ADR (%#x)\n", status);
>>>>>>> refs/remotes/origin/cm-11.0
		return AE_OK;
	}

	device = (adr >> 16) & 0xffff;
	function = adr & 0xffff;

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	pdev = pbus->self;
	if (pdev && pci_is_pcie(pdev)) {
		tmp = acpi_find_root_bridge_handle(pdev);
		if (tmp) {
			struct acpi_pci_root *root = acpi_pci_find_root(tmp);

			if (root && (root->osc_control_set &
					OSC_PCI_EXPRESS_NATIVE_HP_CONTROL))
				return AE_OK;
		}
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
	newfunc = kzalloc(sizeof(struct acpiphp_func), GFP_KERNEL);
	if (!newfunc)
		return AE_NO_MEMORY;

	INIT_LIST_HEAD(&newfunc->sibling);
	newfunc->handle = handle;
	newfunc->function = function;

	if (ACPI_SUCCESS(acpi_get_handle(handle, "_EJ0", &tmp)))
		newfunc->flags = FUNC_HAS_EJ0;

	if (ACPI_SUCCESS(acpi_get_handle(handle, "_STA", &tmp)))
		newfunc->flags |= FUNC_HAS_STA;

	if (ACPI_SUCCESS(acpi_get_handle(handle, "_PS0", &tmp)))
		newfunc->flags |= FUNC_HAS_PS0;

	if (ACPI_SUCCESS(acpi_get_handle(handle, "_PS3", &tmp)))
		newfunc->flags |= FUNC_HAS_PS3;

	if (ACPI_SUCCESS(acpi_get_handle(handle, "_DCK", &tmp)))
		newfunc->flags |= FUNC_HAS_DCK;

	status = acpi_evaluate_integer(handle, "_SUN", NULL, &sun);
	if (ACPI_FAILURE(status)) {
		/*
		 * use the count of the number of slots we've found
		 * for the number of the slot
		 */
		sun = bridge->nr_slots+1;
	}

	/* search for objects that share the same slot */
	for (slot = bridge->slots; slot; slot = slot->next)
		if (slot->device == device) {
			if (slot->sun != sun)
				warn("sibling found, but _SUN doesn't match!\n");
			break;
		}

	if (!slot) {
		slot = kzalloc(sizeof(struct acpiphp_slot), GFP_KERNEL);
		if (!slot) {
			kfree(newfunc);
			return AE_NO_MEMORY;
		}

		slot->bridge = bridge;
		slot->device = device;
		slot->sun = sun;
		INIT_LIST_HEAD(&slot->funcs);
		mutex_init(&slot->crit_sect);

		slot->next = bridge->slots;
		bridge->slots = slot;

		bridge->nr_slots++;

		dbg("found ACPI PCI Hotplug slot %llu at PCI %04x:%02x:%02x\n",
		    slot->sun, pci_domain_nr(pbus), pbus->number, device);
		retval = acpiphp_register_hotplug_slot(slot);
		if (retval) {
			if (retval == -EBUSY)
				warn("Slot %llu already registered by another "
					"hotplug driver\n", slot->sun);
			else
				warn("acpiphp_register_hotplug_slot failed "
					"(err code = 0x%x)\n", retval);
			goto err_exit;
		}
	}

	newfunc->slot = slot;
	list_add_tail(&newfunc->sibling, &slot->funcs);

	pdev = pci_get_slot(pbus, PCI_DEVFN(device, function));
	if (pdev) {
<<<<<<< HEAD
		pdev->current_state = PCI_D0;
=======
>>>>>>> refs/remotes/origin/cm-10.0
		slot->flags |= (SLOT_ENABLED | SLOT_POWEREDON);
		pci_dev_put(pdev);
	}
=======
	mutex_lock(&acpiphp_context_lock);
	context = acpiphp_init_context(handle);
	if (!context) {
		mutex_unlock(&acpiphp_context_lock);
		acpi_handle_err(handle, "No hotplug context\n");
		return AE_NOT_EXIST;
	}
	newfunc = &context->func;
	newfunc->function = function;
	newfunc->parent = bridge;
	mutex_unlock(&acpiphp_context_lock);

	if (acpi_has_method(handle, "_EJ0"))
		newfunc->flags = FUNC_HAS_EJ0;

	if (acpi_has_method(handle, "_STA"))
		newfunc->flags |= FUNC_HAS_STA;

	if (acpi_has_method(handle, "_DCK"))
		newfunc->flags |= FUNC_HAS_DCK;

	/* search for objects that share the same slot */
	list_for_each_entry(slot, &bridge->slots, node)
		if (slot->device == device)
			goto slot_found;

	slot = kzalloc(sizeof(struct acpiphp_slot), GFP_KERNEL);
	if (!slot) {
		status = AE_NO_MEMORY;
		goto err;
	}

	slot->bus = bridge->pci_bus;
	slot->device = device;
	INIT_LIST_HEAD(&slot->funcs);
	mutex_init(&slot->crit_sect);

	list_add_tail(&slot->node, &bridge->slots);

	/* Register slots for ejectable functions only. */
	if (acpi_pci_check_ejectable(pbus, handle)  || is_dock_device(handle)) {
		unsigned long long sun;
		int retval;

		bridge->nr_slots++;
		status = acpi_evaluate_integer(handle, "_SUN", NULL, &sun);
		if (ACPI_FAILURE(status))
			sun = bridge->nr_slots;

		pr_debug("found ACPI PCI Hotplug slot %llu at PCI %04x:%02x:%02x\n",
		    sun, pci_domain_nr(pbus), pbus->number, device);

		retval = acpiphp_register_hotplug_slot(slot, sun);
		if (retval) {
			slot->slot = NULL;
			bridge->nr_slots--;
			if (retval == -EBUSY)
				pr_warn("Slot %llu already registered by another "
					"hotplug driver\n", sun);
			else
				pr_warn("acpiphp_register_hotplug_slot failed "
					"(err code = 0x%x)\n", retval);
		}
		/* Even if the slot registration fails, we can still use it. */
	}

 slot_found:
	newfunc->slot = slot;
	list_add_tail(&newfunc->sibling, &slot->funcs);

	if (pci_bus_read_dev_vendor_id(pbus, PCI_DEVFN(device, function),
				       &val, 60*1000))
		slot->flags |= SLOT_ENABLED;
>>>>>>> refs/remotes/origin/master

	if (is_dock_device(handle)) {
		/* we don't want to call this device's _EJ0
		 * because we want the dock notify handler
		 * to call it after it calls _DCK
		 */
		newfunc->flags &= ~FUNC_HAS_EJ0;
		if (register_hotplug_dock_device(handle,
<<<<<<< HEAD
			&acpiphp_dock_ops, newfunc))
			dbg("failed to register dock device\n");

		/* we need to be notified when dock events happen
		 * outside of the hotplug operation, since we may
		 * need to do fixups before we can hotplug.
		 */
		newfunc->nb.notifier_call = post_dock_fixups;
		if (register_dock_notifier(&newfunc->nb))
			dbg("failed to register a dock notifier");
=======
			&acpiphp_dock_ops, context,
			acpiphp_dock_init, acpiphp_dock_release))
			pr_debug("failed to register dock device\n");
>>>>>>> refs/remotes/origin/master
	}

	/* install notify handler */
	if (!(newfunc->flags & FUNC_HAS_DCK)) {
<<<<<<< HEAD
		status = acpi_install_notify_handler(handle,
					     ACPI_SYSTEM_NOTIFY,
					     handle_hotplug_event_func,
					     newfunc);

		if (ACPI_FAILURE(status))
			err("failed to register interrupt notify handler\n");
	} else
		status = AE_OK;

	return status;

 err_exit:
	bridge->nr_slots--;
	bridge->slots = slot->next;
	kfree(slot);
	kfree(newfunc);

	return AE_OK;
}


/* see if it's worth looking at this bridge */
static int detect_ejectable_slots(acpi_handle handle)
{
	int found = acpi_pci_detect_ejectable(handle);
	if (!found) {
		acpi_walk_namespace(ACPI_TYPE_DEVICE, handle, (u32)1,
				    is_pci_dock_device, NULL, (void *)&found, NULL);
	}
	return found;
}

/* initialize miscellaneous stuff for both root and PCI-to-PCI bridge */
static void init_bridge_misc(struct acpiphp_bridge *bridge)
{
	acpi_status status;

	/* must be added to the list prior to calling register_slot */
	list_add(&bridge->list, &bridge_list);

	/* register all slot objects under this bridge */
	status = acpi_walk_namespace(ACPI_TYPE_DEVICE, bridge->handle, (u32)1,
				     register_slot, NULL, bridge, NULL);
	if (ACPI_FAILURE(status)) {
		list_del(&bridge->list);
		return;
	}

	/* install notify handler */
	if (bridge->type != BRIDGE_TYPE_HOST) {
		if ((bridge->flags & BRIDGE_HAS_EJ0) && bridge->func) {
			status = acpi_remove_notify_handler(bridge->func->handle,
						ACPI_SYSTEM_NOTIFY,
						handle_hotplug_event_func);
			if (ACPI_FAILURE(status))
				err("failed to remove notify handler\n");
		}
		status = acpi_install_notify_handler(bridge->handle,
					     ACPI_SYSTEM_NOTIFY,
					     handle_hotplug_event_bridge,
					     bridge);

		if (ACPI_FAILURE(status)) {
			err("failed to register interrupt notify handler\n");
		}
	}
}


/* find acpiphp_func from acpiphp_bridge */
static struct acpiphp_func *acpiphp_bridge_handle_to_function(acpi_handle handle)
{
	struct acpiphp_bridge *bridge;
	struct acpiphp_slot *slot;
	struct acpiphp_func *func;

	list_for_each_entry(bridge, &bridge_list, list) {
		for (slot = bridge->slots; slot; slot = slot->next) {
			list_for_each_entry(func, &slot->funcs, sibling) {
				if (func->handle == handle)
					return func;
			}
		}
	}

	return NULL;
}


static inline void config_p2p_bridge_flags(struct acpiphp_bridge *bridge)
{
	acpi_handle dummy_handle;

	if (ACPI_SUCCESS(acpi_get_handle(bridge->handle,
					"_STA", &dummy_handle)))
		bridge->flags |= BRIDGE_HAS_STA;

	if (ACPI_SUCCESS(acpi_get_handle(bridge->handle,
					"_EJ0", &dummy_handle)))
		bridge->flags |= BRIDGE_HAS_EJ0;

	if (ACPI_SUCCESS(acpi_get_handle(bridge->handle,
					"_PS0", &dummy_handle)))
		bridge->flags |= BRIDGE_HAS_PS0;

	if (ACPI_SUCCESS(acpi_get_handle(bridge->handle,
					"_PS3", &dummy_handle)))
		bridge->flags |= BRIDGE_HAS_PS3;

	/* is this ejectable p2p bridge? */
	if (bridge->flags & BRIDGE_HAS_EJ0) {
		struct acpiphp_func *func;

		dbg("found ejectable p2p bridge\n");

		/* make link between PCI bridge and PCI function */
		func = acpiphp_bridge_handle_to_function(bridge->handle);
		if (!func)
			return;
		bridge->func = func;
		func->bridge = bridge;
	}
}


/* allocate and initialize host bridge data structure */
static void add_host_bridge(acpi_handle *handle)
{
	struct acpiphp_bridge *bridge;
	struct acpi_pci_root *root = acpi_pci_find_root(handle);

	bridge = kzalloc(sizeof(struct acpiphp_bridge), GFP_KERNEL);
	if (bridge == NULL)
		return;

	bridge->type = BRIDGE_TYPE_HOST;
	bridge->handle = handle;

	bridge->pci_bus = root->bus;

	spin_lock_init(&bridge->res_lock);

	init_bridge_misc(bridge);
}


/* allocate and initialize PCI-to-PCI bridge data structure */
static void add_p2p_bridge(acpi_handle *handle)
{
	struct acpiphp_bridge *bridge;

	bridge = kzalloc(sizeof(struct acpiphp_bridge), GFP_KERNEL);
	if (bridge == NULL) {
		err("out of memory\n");
		return;
	}

	bridge->type = BRIDGE_TYPE_P2P;
	bridge->handle = handle;
	config_p2p_bridge_flags(bridge);

	bridge->pci_dev = acpi_get_pci_dev(handle);
	bridge->pci_bus = bridge->pci_dev->subordinate;
	if (!bridge->pci_bus) {
		err("This is not a PCI-to-PCI bridge!\n");
		goto err;
	}

	/*
	 * Grab a ref to the subordinate PCI bus in case the bus is
	 * removed via PCI core logical hotplug. The ref pins the bus
	 * (which we access during module unload).
	 */
	get_device(&bridge->pci_bus->dev);
	spin_lock_init(&bridge->res_lock);

	init_bridge_misc(bridge);
	return;
 err:
	pci_dev_put(bridge->pci_dev);
	kfree(bridge);
	return;
}


/* callback routine to find P2P bridges */
static acpi_status
find_p2p_bridge(acpi_handle handle, u32 lvl, void *context, void **rv)
{
	acpi_status status;
	struct pci_dev *dev;

	dev = acpi_get_pci_dev(handle);
	if (!dev || !dev->subordinate)
		goto out;

	/* check if this bridge has ejectable slots */
	if ((detect_ejectable_slots(handle) > 0)) {
		dbg("found PCI-to-PCI bridge at PCI %s\n", pci_name(dev));
		add_p2p_bridge(handle);
	}

	/* search P2P bridges under this p2p bridge */
	status = acpi_walk_namespace(ACPI_TYPE_DEVICE, handle, (u32)1,
				     find_p2p_bridge, NULL, NULL, NULL);
	if (ACPI_FAILURE(status))
		warn("find_p2p_bridge failed (error code = 0x%x)\n", status);

 out:
	pci_dev_put(dev);
	return AE_OK;
}


/* find hot-pluggable slots, and then find P2P bridge */
static int add_bridge(acpi_handle handle)
{
	acpi_status status;
	unsigned long long tmp;
	acpi_handle dummy_handle;

	/* if the bridge doesn't have _STA, we assume it is always there */
	status = acpi_get_handle(handle, "_STA", &dummy_handle);
	if (ACPI_SUCCESS(status)) {
		status = acpi_evaluate_integer(handle, "_STA", NULL, &tmp);
		if (ACPI_FAILURE(status)) {
			dbg("%s: _STA evaluation failure\n", __func__);
			return 0;
		}
		if ((tmp & ACPI_STA_FUNCTIONING) == 0)
			/* don't register this object */
			return 0;
	}

	/* check if this bridge has ejectable slots */
	if (detect_ejectable_slots(handle) > 0) {
		dbg("found PCI host-bus bridge with hot-pluggable slots\n");
		add_host_bridge(handle);
	}

	/* search P2P bridges under this host bridge */
	status = acpi_walk_namespace(ACPI_TYPE_DEVICE, handle, (u32)1,
				     find_p2p_bridge, NULL, NULL, NULL);

	if (ACPI_FAILURE(status))
		warn("find_p2p_bridge failed (error code = 0x%x)\n", status);

	return 0;
}

static struct acpiphp_bridge *acpiphp_handle_to_bridge(acpi_handle handle)
{
	struct acpiphp_bridge *bridge;

	list_for_each_entry(bridge, &bridge_list, list)
		if (bridge->handle == handle)
			return bridge;

	return NULL;
}

static void cleanup_bridge(struct acpiphp_bridge *bridge)
{
	struct acpiphp_slot *slot, *next;
	struct acpiphp_func *func, *tmp;
	acpi_status status;
	acpi_handle handle = bridge->handle;

	status = acpi_remove_notify_handler(handle, ACPI_SYSTEM_NOTIFY,
					    handle_hotplug_event_bridge);
	if (ACPI_FAILURE(status))
		err("failed to remove notify handler\n");

	if ((bridge->type != BRIDGE_TYPE_HOST) &&
	    ((bridge->flags & BRIDGE_HAS_EJ0) && bridge->func)) {
		status = acpi_install_notify_handler(bridge->func->handle,
						ACPI_SYSTEM_NOTIFY,
						handle_hotplug_event_func,
						bridge->func);
		if (ACPI_FAILURE(status))
			err("failed to install interrupt notify handler\n");
	}

	slot = bridge->slots;
	while (slot) {
		next = slot->next;
		list_for_each_entry_safe(func, tmp, &slot->funcs, sibling) {
			if (is_dock_device(func->handle)) {
				unregister_hotplug_dock_device(func->handle);
				unregister_dock_notifier(&func->nb);
			}
			if (!(func->flags & FUNC_HAS_DCK)) {
				status = acpi_remove_notify_handler(func->handle,
						ACPI_SYSTEM_NOTIFY,
						handle_hotplug_event_func);
				if (ACPI_FAILURE(status))
					err("failed to remove notify handler\n");
			}
			list_del(&func->sibling);
			kfree(func);
		}
		acpiphp_unregister_hotplug_slot(slot);
		list_del(&slot->funcs);
		kfree(slot);
		slot = next;
	}

	/*
	 * Only P2P bridges have a pci_dev
	 */
	if (bridge->pci_dev)
		put_device(&bridge->pci_bus->dev);

	pci_dev_put(bridge->pci_dev);
	list_del(&bridge->list);
	kfree(bridge);
}

static acpi_status
cleanup_p2p_bridge(acpi_handle handle, u32 lvl, void *context, void **rv)
{
	struct acpiphp_bridge *bridge;

	/* cleanup p2p bridges under this P2P bridge
	   in a depth-first manner */
	acpi_walk_namespace(ACPI_TYPE_DEVICE, handle, (u32)1,
				cleanup_p2p_bridge, NULL, NULL, NULL);

	bridge = acpiphp_handle_to_bridge(handle);
	if (bridge)
		cleanup_bridge(bridge);

	return AE_OK;
}

static void remove_bridge(acpi_handle handle)
{
	struct acpiphp_bridge *bridge;

	/* cleanup p2p bridges under this host bridge
	   in a depth-first manner */
	acpi_walk_namespace(ACPI_TYPE_DEVICE, handle,
				(u32)1, cleanup_p2p_bridge, NULL, NULL, NULL);

	/*
	 * On root bridges with hotplug slots directly underneath (ie,
	 * no p2p bridge between), we call cleanup_bridge(). 
	 *
	 * The else clause cleans up root bridges that either had no
	 * hotplug slots at all, or had a p2p bridge underneath.
	 */
	bridge = acpiphp_handle_to_bridge(handle);
	if (bridge)
		cleanup_bridge(bridge);
	else
		acpi_remove_notify_handler(handle, ACPI_SYSTEM_NOTIFY,
					   handle_hotplug_event_bridge);
}

static int power_on_slot(struct acpiphp_slot *slot)
{
	acpi_status status;
	struct acpiphp_func *func;
	int retval = 0;

	/* if already enabled, just skip */
	if (slot->flags & SLOT_POWEREDON)
		goto err_exit;

	list_for_each_entry(func, &slot->funcs, sibling) {
		if (func->flags & FUNC_HAS_PS0) {
			dbg("%s: executing _PS0\n", __func__);
			status = acpi_evaluate_object(func->handle, "_PS0", NULL, NULL);
			if (ACPI_FAILURE(status)) {
				warn("%s: _PS0 failed\n", __func__);
				retval = -1;
				goto err_exit;
			} else
				break;
		}
	}

	/* TBD: evaluate _STA to check if the slot is enabled */

	slot->flags |= SLOT_POWEREDON;

 err_exit:
	return retval;
}


static int power_off_slot(struct acpiphp_slot *slot)
{
	acpi_status status;
	struct acpiphp_func *func;

	int retval = 0;

	/* if already disabled, just skip */
	if ((slot->flags & SLOT_POWEREDON) == 0)
		goto err_exit;

	list_for_each_entry(func, &slot->funcs, sibling) {
		if (func->flags & FUNC_HAS_PS3) {
			status = acpi_evaluate_object(func->handle, "_PS3", NULL, NULL);
			if (ACPI_FAILURE(status)) {
				warn("%s: _PS3 failed\n", __func__);
				retval = -1;
				goto err_exit;
			} else
				break;
		}
	}

	/* TBD: evaluate _STA to check if the slot is disabled */

	slot->flags &= (~SLOT_POWEREDON);

 err_exit:
	return retval;
}



=======
		status = acpi_install_notify_handler(handle, ACPI_SYSTEM_NOTIFY,
						     handle_hotplug_event,
						     context);
		if (ACPI_FAILURE(status))
			acpi_handle_err(handle,
					"failed to install notify handler\n");
	}

	return AE_OK;

 err:
	mutex_lock(&acpiphp_context_lock);
	acpiphp_put_context(context);
	mutex_unlock(&acpiphp_context_lock);
	return status;
}

static struct acpiphp_bridge *acpiphp_handle_to_bridge(acpi_handle handle)
{
	struct acpiphp_context *context;
	struct acpiphp_bridge *bridge = NULL;

	mutex_lock(&acpiphp_context_lock);
	context = acpiphp_get_context(handle);
	if (context) {
		bridge = context->bridge;
		if (bridge)
			get_bridge(bridge);

		acpiphp_put_context(context);
	}
	mutex_unlock(&acpiphp_context_lock);
	return bridge;
}

static void cleanup_bridge(struct acpiphp_bridge *bridge)
{
	struct acpiphp_slot *slot;
	struct acpiphp_func *func;
	acpi_status status;

	list_for_each_entry(slot, &bridge->slots, node) {
		list_for_each_entry(func, &slot->funcs, sibling) {
			acpi_handle handle = func_to_handle(func);

			if (is_dock_device(handle))
				unregister_hotplug_dock_device(handle);

			if (!(func->flags & FUNC_HAS_DCK)) {
				status = acpi_remove_notify_handler(handle,
							ACPI_SYSTEM_NOTIFY,
							handle_hotplug_event);
				if (ACPI_FAILURE(status))
					pr_err("failed to remove notify handler\n");
			}
		}
		if (slot->slot)
			acpiphp_unregister_hotplug_slot(slot);
	}

	mutex_lock(&bridge_mutex);
	list_del(&bridge->list);
	mutex_unlock(&bridge_mutex);
}

>>>>>>> refs/remotes/origin/master
/**
 * acpiphp_max_busnr - return the highest reserved bus number under the given bus.
 * @bus: bus to start search with
 */
static unsigned char acpiphp_max_busnr(struct pci_bus *bus)
{
	struct list_head *tmp;
	unsigned char max, n;

	/*
	 * pci_bus_max_busnr will return the highest
	 * reserved busnr for all these children.
	 * that is equivalent to the bus->subordinate
	 * value.  We don't want to use the parent's
	 * bus->subordinate value because it could have
	 * padding in it.
	 */
<<<<<<< HEAD
	max = bus->secondary;
=======
	max = bus->busn_res.start;
>>>>>>> refs/remotes/origin/master

	list_for_each(tmp, &bus->children) {
		n = pci_bus_max_busnr(pci_bus_b(tmp));
		if (n > max)
			max = n;
	}
	return max;
}

<<<<<<< HEAD

/**
 * acpiphp_bus_add - add a new bus to acpi subsystem
 * @func: acpiphp_func of the bridge
 */
static int acpiphp_bus_add(struct acpiphp_func *func)
{
	acpi_handle phandle;
	struct acpi_device *device, *pdevice;
	int ret_val;

	acpi_get_parent(func->handle, &phandle);
	if (acpi_bus_get_device(phandle, &pdevice)) {
		dbg("no parent device, assuming NULL\n");
		pdevice = NULL;
	}
	if (!acpi_bus_get_device(func->handle, &device)) {
		dbg("bus exists... trim\n");
		/* this shouldn't be in here, so remove
		 * the bus then re-add it...
		 */
		ret_val = acpi_bus_trim(device, 1);
		dbg("acpi_bus_trim return %x\n", ret_val);
	}

	ret_val = acpi_bus_add(&device, pdevice, func->handle,
		ACPI_BUS_TYPE_DEVICE);
	if (ret_val) {
		dbg("error adding bus, %x\n",
			-ret_val);
		goto acpiphp_bus_add_out;
	}
	ret_val = acpi_bus_start(device);

acpiphp_bus_add_out:
	return ret_val;
}


/**
 * acpiphp_bus_trim - trim a bus from acpi subsystem
 * @handle: handle to acpi namespace
 */
static int acpiphp_bus_trim(acpi_handle handle)
{
	struct acpi_device *device;
	int retval;

	retval = acpi_bus_get_device(handle, &device);
	if (retval) {
		dbg("acpi_device not found\n");
		return retval;
	}

	retval = acpi_bus_trim(device, 1);
	if (retval)
		err("cannot remove from acpi list\n");

	return retval;
=======
/**
 * acpiphp_bus_trim - Trim device objects in an ACPI namespace subtree.
 * @handle: ACPI device object handle to start from.
 */
static void acpiphp_bus_trim(acpi_handle handle)
{
	struct acpi_device *adev = NULL;

	acpi_bus_get_device(handle, &adev);
	if (adev)
		acpi_bus_trim(adev);
}

/**
 * acpiphp_bus_add - Scan ACPI namespace subtree.
 * @handle: ACPI object handle to start the scan from.
 */
static void acpiphp_bus_add(acpi_handle handle)
{
	struct acpi_device *adev = NULL;

	acpi_bus_scan(handle);
	acpi_bus_get_device(handle, &adev);
	if (adev)
		acpi_device_set_power(adev, ACPI_STATE_D0);
>>>>>>> refs/remotes/origin/master
}

static void acpiphp_set_acpi_region(struct acpiphp_slot *slot)
{
	struct acpiphp_func *func;
	union acpi_object params[2];
	struct acpi_object_list arg_list;

	list_for_each_entry(func, &slot->funcs, sibling) {
		arg_list.count = 2;
		arg_list.pointer = params;
		params[0].type = ACPI_TYPE_INTEGER;
		params[0].integer.value = ACPI_ADR_SPACE_PCI_CONFIG;
		params[1].type = ACPI_TYPE_INTEGER;
		params[1].integer.value = 1;
		/* _REG is optional, we don't care about if there is failure */
<<<<<<< HEAD
		acpi_evaluate_object(func->handle, "_REG", &arg_list, NULL);
	}
}

/**
 * enable_device - enable, configure a slot
=======
		acpi_evaluate_object(func_to_handle(func), "_REG", &arg_list,
				     NULL);
	}
}

static void check_hotplug_bridge(struct acpiphp_slot *slot, struct pci_dev *dev)
{
	struct acpiphp_func *func;

	/* quirk, or pcie could set it already */
	if (dev->is_hotplug_bridge)
		return;

	list_for_each_entry(func, &slot->funcs, sibling) {
		if (PCI_FUNC(dev->devfn) == func->function) {
			dev->is_hotplug_bridge = 1;
			break;
		}
	}
}

static int acpiphp_rescan_slot(struct acpiphp_slot *slot)
{
	struct acpiphp_func *func;

	list_for_each_entry(func, &slot->funcs, sibling)
		acpiphp_bus_add(func_to_handle(func));

	return pci_scan_slot(slot->bus, PCI_DEVFN(slot->device, 0));
}

/**
 * enable_slot - enable, configure a slot
>>>>>>> refs/remotes/origin/master
 * @slot: slot to be enabled
 *
 * This function should be called per *physical slot*,
 * not per each slot object in ACPI namespace.
 */
<<<<<<< HEAD
static int __ref enable_device(struct acpiphp_slot *slot)
{
	struct pci_dev *dev;
	struct pci_bus *bus = slot->bridge->pci_bus;
	struct acpiphp_func *func;
	int retval = 0;
	int num, max, pass;
	acpi_status status;

	if (slot->flags & SLOT_ENABLED)
		goto err_exit;

<<<<<<< HEAD
	/* sanity check: dev should be NULL when hot-plugged in */
	dev = pci_get_slot(bus, PCI_DEVFN(slot->device, 0));
	if (dev) {
		/* This case shouldn't happen */
		err("pci_dev structure already exists.\n");
		pci_dev_put(dev);
		retval = -1;
		goto err_exit;
	}

	num = pci_scan_slot(bus, PCI_DEVFN(slot->device, 0));
	if (num == 0) {
		err("No new device found\n");
		retval = -1;
=======
	num = pci_scan_slot(bus, PCI_DEVFN(slot->device, 0));
	if (num == 0) {
		/* Maybe only part of funcs are added. */
		dbg("No new device found\n");
>>>>>>> refs/remotes/origin/cm-10.0
		goto err_exit;
	}

=======
static void __ref enable_slot(struct acpiphp_slot *slot)
{
	struct pci_dev *dev;
	struct pci_bus *bus = slot->bus;
	struct acpiphp_func *func;
	int max, pass;
	LIST_HEAD(add_list);

	acpiphp_rescan_slot(slot);
>>>>>>> refs/remotes/origin/master
	max = acpiphp_max_busnr(bus);
	for (pass = 0; pass < 2; pass++) {
		list_for_each_entry(dev, &bus->devices, bus_list) {
			if (PCI_SLOT(dev->devfn) != slot->device)
				continue;
<<<<<<< HEAD
			if (dev->hdr_type == PCI_HEADER_TYPE_BRIDGE ||
			    dev->hdr_type == PCI_HEADER_TYPE_CARDBUS) {
				max = pci_scan_bridge(bus, dev, max, pass);
				if (pass && dev->subordinate)
					pci_bus_size_bridges(dev->subordinate);
			}
		}
	}

	list_for_each_entry(func, &slot->funcs, sibling)
		acpiphp_bus_add(func);

	pci_bus_assign_resources(bus);
	acpiphp_sanitize_bus(bus);
	acpiphp_set_hpp_values(bus);
	acpiphp_set_acpi_region(slot);
	pci_enable_bridges(bus);
=======

			if (dev->hdr_type == PCI_HEADER_TYPE_BRIDGE ||
			    dev->hdr_type == PCI_HEADER_TYPE_CARDBUS) {
				max = pci_scan_bridge(bus, dev, max, pass);
				if (pass && dev->subordinate) {
					check_hotplug_bridge(slot, dev);
					pcibios_resource_survey_bus(dev->subordinate);
					__pci_bus_size_bridges(dev->subordinate,
							       &add_list);
				}
			}
		}
	}
	__pci_bus_assign_resources(bus, &add_list, NULL);

	acpiphp_sanitize_bus(bus);
	acpiphp_set_hpp_values(bus);
	acpiphp_set_acpi_region(slot);
>>>>>>> refs/remotes/origin/master

	list_for_each_entry(dev, &bus->devices, bus_list) {
		/* Assume that newly added devices are powered on already. */
		if (!dev->is_added)
			dev->current_state = PCI_D0;
	}

	pci_bus_add_devices(bus);

<<<<<<< HEAD
<<<<<<< HEAD
	list_for_each_entry(func, &slot->funcs, sibling) {
		dev = pci_get_slot(bus, PCI_DEVFN(slot->device,
						  func->function));
		if (!dev)
			continue;
=======
=======
>>>>>>> refs/remotes/origin/master
	slot->flags |= SLOT_ENABLED;
	list_for_each_entry(func, &slot->funcs, sibling) {
		dev = pci_get_slot(bus, PCI_DEVFN(slot->device,
						  func->function));
		if (!dev) {
			/* Do not set SLOT_ENABLED flag if some funcs
			   are not added. */
			slot->flags &= (~SLOT_ENABLED);
			continue;
		}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

		if (dev->hdr_type != PCI_HEADER_TYPE_BRIDGE &&
		    dev->hdr_type != PCI_HEADER_TYPE_CARDBUS) {
			pci_dev_put(dev);
			continue;
		}

		status = find_p2p_bridge(func->handle, (u32)1, bus, NULL);
		if (ACPI_FAILURE(status))
			warn("find_p2p_bridge failed (error code = 0x%x)\n",
				status);
		pci_dev_put(dev);
	}

<<<<<<< HEAD
	slot->flags |= SLOT_ENABLED;
=======
>>>>>>> refs/remotes/origin/cm-10.0

 err_exit:
	return retval;
}

static void disable_bridges(struct pci_bus *bus)
{
	struct pci_dev *dev;
	list_for_each_entry(dev, &bus->devices, bus_list) {
		if (dev->subordinate) {
			disable_bridges(dev->subordinate);
			pci_disable_device(dev);
		}
	}
}

/**
 * disable_device - disable a slot
 * @slot: ACPI PHP slot
 */
static int disable_device(struct acpiphp_slot *slot)
{
	struct acpiphp_func *func;
	struct pci_dev *pdev;
<<<<<<< HEAD

	/* is this slot already disabled? */
	if (!(slot->flags & SLOT_ENABLED))
=======
	struct pci_bus *bus = slot->bridge->pci_bus;

	/* The slot will be enabled when func 0 is added, so check
	   func 0 before disable the slot. */
	pdev = pci_get_slot(bus, PCI_DEVFN(slot->device, 0));
	if (!pdev)
>>>>>>> refs/remotes/origin/cm-10.0
		goto err_exit;

	list_for_each_entry(func, &slot->funcs, sibling) {
		if (func->bridge) {
			/* cleanup p2p bridges under this P2P bridge */
			cleanup_p2p_bridge(func->bridge->handle,
						(u32)1, NULL, NULL);
			func->bridge = NULL;
		}

		pdev = pci_get_slot(slot->bridge->pci_bus,
				    PCI_DEVFN(slot->device, func->function));
		if (pdev) {
			pci_stop_bus_device(pdev);
			if (pdev->subordinate) {
				disable_bridges(pdev->subordinate);
				pci_disable_device(pdev);
			}
<<<<<<< HEAD
			pci_remove_bus_device(pdev);
=======
			__pci_remove_bus_device(pdev);
>>>>>>> refs/remotes/origin/cm-10.0
			pci_dev_put(pdev);
		}
	}

	list_for_each_entry(func, &slot->funcs, sibling) {
		acpiphp_bus_trim(func->handle);
	}

	slot->flags &= (~SLOT_ENABLED);

err_exit:
	return 0;
}

=======
	}
}

/* return first device in slot, acquiring a reference on it */
static struct pci_dev *dev_in_slot(struct acpiphp_slot *slot)
{
	struct pci_bus *bus = slot->bus;
	struct pci_dev *dev;
	struct pci_dev *ret = NULL;

	down_read(&pci_bus_sem);
	list_for_each_entry(dev, &bus->devices, bus_list)
		if (PCI_SLOT(dev->devfn) == slot->device) {
			ret = pci_dev_get(dev);
			break;
		}
	up_read(&pci_bus_sem);

	return ret;
}

/**
 * disable_slot - disable a slot
 * @slot: ACPI PHP slot
 */
static void disable_slot(struct acpiphp_slot *slot)
{
	struct acpiphp_func *func;
	struct pci_dev *pdev;

	/*
	 * enable_slot() enumerates all functions in this device via
	 * pci_scan_slot(), whether they have associated ACPI hotplug
	 * methods (_EJ0, etc.) or not.  Therefore, we remove all functions
	 * here.
	 */
	while ((pdev = dev_in_slot(slot))) {
		pci_stop_and_remove_bus_device(pdev);
		pci_dev_put(pdev);
	}

	list_for_each_entry(func, &slot->funcs, sibling)
		acpiphp_bus_trim(func_to_handle(func));

	slot->flags &= (~SLOT_ENABLED);
}

static bool acpiphp_no_hotplug(acpi_handle handle)
{
	struct acpi_device *adev = NULL;

	acpi_bus_get_device(handle, &adev);
	return adev && adev->flags.no_hotplug;
}

static bool slot_no_hotplug(struct acpiphp_slot *slot)
{
	struct acpiphp_func *func;

	list_for_each_entry(func, &slot->funcs, sibling)
		if (acpiphp_no_hotplug(func_to_handle(func)))
			return true;

	return false;
}
>>>>>>> refs/remotes/origin/master

/**
 * get_slot_status - get ACPI slot status
 * @slot: ACPI PHP slot
 *
 * If a slot has _STA for each function and if any one of them
 * returned non-zero status, return it.
 *
 * If a slot doesn't have _STA and if any one of its functions'
 * configuration space is configured, return 0x0f as a _STA.
 *
 * Otherwise return 0.
 */
static unsigned int get_slot_status(struct acpiphp_slot *slot)
{
<<<<<<< HEAD
	acpi_status status;
	unsigned long long sta = 0;
	u32 dvid;
=======
	unsigned long long sta = 0;
>>>>>>> refs/remotes/origin/master
	struct acpiphp_func *func;

	list_for_each_entry(func, &slot->funcs, sibling) {
		if (func->flags & FUNC_HAS_STA) {
<<<<<<< HEAD
			status = acpi_evaluate_integer(func->handle, "_STA", NULL, &sta);
			if (ACPI_SUCCESS(status) && sta)
				break;
		} else {
			pci_bus_read_config_dword(slot->bridge->pci_bus,
=======
			acpi_status status;

			status = acpi_evaluate_integer(func_to_handle(func),
						       "_STA", NULL, &sta);
			if (ACPI_SUCCESS(status) && sta)
				break;
		} else {
			u32 dvid;

			pci_bus_read_config_dword(slot->bus,
>>>>>>> refs/remotes/origin/master
						  PCI_DEVFN(slot->device,
							    func->function),
						  PCI_VENDOR_ID, &dvid);
			if (dvid != 0xffffffff) {
				sta = ACPI_STA_ALL;
				break;
			}
		}
	}

	return (unsigned int)sta;
}

/**
<<<<<<< HEAD
 * acpiphp_eject_slot - physically eject the slot
 * @slot: ACPI PHP slot
 */
int acpiphp_eject_slot(struct acpiphp_slot *slot)
{
	acpi_status status;
	struct acpiphp_func *func;
	struct acpi_object_list arg_list;
	union acpi_object arg;

	list_for_each_entry(func, &slot->funcs, sibling) {
		/* We don't want to call _EJ0 on non-existing functions. */
		if ((func->flags & FUNC_HAS_EJ0)) {
			/* _EJ0 method take one argument */
			arg_list.count = 1;
			arg_list.pointer = &arg;
			arg.type = ACPI_TYPE_INTEGER;
			arg.integer.value = 1;

			status = acpi_evaluate_object(func->handle, "_EJ0", &arg_list, NULL);
			if (ACPI_FAILURE(status)) {
				warn("%s: _EJ0 failed\n", __func__);
				return -1;
			} else
				break;
		}
	}
	return 0;
=======
 * trim_stale_devices - remove PCI devices that are not responding.
 * @dev: PCI device to start walking the hierarchy from.
 */
static void trim_stale_devices(struct pci_dev *dev)
{
	acpi_handle handle = ACPI_HANDLE(&dev->dev);
	struct pci_bus *bus = dev->subordinate;
	bool alive = false;

	if (handle) {
		acpi_status status;
		unsigned long long sta;

		status = acpi_evaluate_integer(handle, "_STA", NULL, &sta);
		alive = (ACPI_SUCCESS(status) && sta == ACPI_STA_ALL)
			|| acpiphp_no_hotplug(handle);
	}
	if (!alive) {
		u32 v;

		/* Check if the device responds. */
		alive = pci_bus_read_dev_vendor_id(dev->bus, dev->devfn, &v, 0);
	}
	if (!alive) {
		pci_stop_and_remove_bus_device(dev);
		if (handle)
			acpiphp_bus_trim(handle);
	} else if (bus) {
		struct pci_dev *child, *tmp;

		/* The device is a bridge. so check the bus below it. */
		pm_runtime_get_sync(&dev->dev);
		list_for_each_entry_safe(child, tmp, &bus->devices, bus_list)
			trim_stale_devices(child);

		pm_runtime_put(&dev->dev);
	}
>>>>>>> refs/remotes/origin/master
}

/**
 * acpiphp_check_bridge - re-enumerate devices
 * @bridge: where to begin re-enumeration
 *
 * Iterate over all slots under this bridge and make sure that if a
 * card is present they are enabled, and if not they are disabled.
 */
<<<<<<< HEAD
static int acpiphp_check_bridge(struct acpiphp_bridge *bridge)
{
	struct acpiphp_slot *slot;
	int retval = 0;
	int enabled, disabled;

	enabled = disabled = 0;

	for (slot = bridge->slots; slot; slot = slot->next) {
		unsigned int status = get_slot_status(slot);
		if (slot->flags & SLOT_ENABLED) {
			if (status == ACPI_STA_ALL)
				continue;
			retval = acpiphp_disable_slot(slot);
			if (retval) {
				err("Error occurred in disabling\n");
				goto err_exit;
			} else {
				acpiphp_eject_slot(slot);
			}
			disabled++;
		} else {
			if (status != ACPI_STA_ALL)
				continue;
			retval = acpiphp_enable_slot(slot);
			if (retval) {
				err("Error occurred in enabling\n");
				goto err_exit;
			}
			enabled++;
		}
	}

	dbg("%s: %d enabled, %d disabled\n", __func__, enabled, disabled);

 err_exit:
	return retval;
=======
static void acpiphp_check_bridge(struct acpiphp_bridge *bridge)
{
	struct acpiphp_slot *slot;

	list_for_each_entry(slot, &bridge->slots, node) {
		struct pci_bus *bus = slot->bus;
		struct pci_dev *dev, *tmp;

		mutex_lock(&slot->crit_sect);
		if (slot_no_hotplug(slot)) {
			; /* do nothing */
		} else if (get_slot_status(slot) == ACPI_STA_ALL) {
			/* remove stale devices if any */
			list_for_each_entry_safe(dev, tmp, &bus->devices,
						 bus_list)
				if (PCI_SLOT(dev->devfn) == slot->device)
					trim_stale_devices(dev);

			/* configure all functions */
			enable_slot(slot);
		} else {
			disable_slot(slot);
		}
		mutex_unlock(&slot->crit_sect);
	}
>>>>>>> refs/remotes/origin/master
}

static void acpiphp_set_hpp_values(struct pci_bus *bus)
{
	struct pci_dev *dev;

	list_for_each_entry(dev, &bus->devices, bus_list)
		pci_configure_slot(dev);
}

/*
 * Remove devices for which we could not assign resources, call
 * arch specific code to fix-up the bus
 */
static void acpiphp_sanitize_bus(struct pci_bus *bus)
{
<<<<<<< HEAD
	struct pci_dev *dev;
	int i;
	unsigned long type_mask = IORESOURCE_IO | IORESOURCE_MEM;

	list_for_each_entry(dev, &bus->devices, bus_list) {
=======
	struct pci_dev *dev, *tmp;
	int i;
	unsigned long type_mask = IORESOURCE_IO | IORESOURCE_MEM;

	list_for_each_entry_safe(dev, tmp, &bus->devices, bus_list) {
>>>>>>> refs/remotes/origin/master
		for (i=0; i<PCI_BRIDGE_RESOURCES; i++) {
			struct resource *res = &dev->resource[i];
			if ((res->flags & type_mask) && !res->start &&
					res->end) {
				/* Could not assign a required resources
				 * for this device, remove it */
<<<<<<< HEAD
<<<<<<< HEAD
				pci_remove_bus_device(dev);
=======
				pci_stop_and_remove_bus_device(dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
				pci_stop_and_remove_bus_device(dev);
>>>>>>> refs/remotes/origin/master
				break;
			}
		}
	}
}

<<<<<<< HEAD
/* Program resources in newly inserted bridge */
static int acpiphp_configure_bridge (acpi_handle handle)
{
	struct pci_bus *bus;

	if (acpi_is_root_bridge(handle)) {
		struct acpi_pci_root *root = acpi_pci_find_root(handle);
		bus = root->bus;
	} else {
		struct pci_dev *pdev = acpi_get_pci_dev(handle);
		bus = pdev->subordinate;
		pci_dev_put(pdev);
	}

	pci_bus_size_bridges(bus);
	pci_bus_assign_resources(bus);
	acpiphp_sanitize_bus(bus);
	acpiphp_set_hpp_values(bus);
	pci_enable_bridges(bus);
	return 0;
}

static void handle_bridge_insertion(acpi_handle handle, u32 type)
{
	struct acpi_device *device, *pdevice;
	acpi_handle phandle;

	if ((type != ACPI_NOTIFY_BUS_CHECK) &&
			(type != ACPI_NOTIFY_DEVICE_CHECK)) {
		err("unexpected notification type %d\n", type);
		return;
	}

	acpi_get_parent(handle, &phandle);
	if (acpi_bus_get_device(phandle, &pdevice)) {
		dbg("no parent device, assuming NULL\n");
		pdevice = NULL;
	}
	if (acpi_bus_add(&device, pdevice, handle, ACPI_BUS_TYPE_DEVICE)) {
		err("cannot add bridge to acpi list\n");
		return;
	}
	if (!acpiphp_configure_bridge(handle) &&
		!acpi_bus_start(device))
		add_bridge(handle);
	else
		err("cannot configure and start bridge\n");

}

=======
>>>>>>> refs/remotes/origin/master
/*
 * ACPI event handlers
 */

<<<<<<< HEAD
static acpi_status
count_sub_bridges(acpi_handle handle, u32 lvl, void *context, void **rv)
{
	int *count = (int *)context;
	struct acpiphp_bridge *bridge;

	bridge = acpiphp_handle_to_bridge(handle);
	if (bridge)
		(*count)++;
	return AE_OK ;
}

static acpi_status
check_sub_bridges(acpi_handle handle, u32 lvl, void *context, void **rv)
{
	struct acpiphp_bridge *bridge;
	char objname[64];
	struct acpi_buffer buffer = { .length = sizeof(objname),
				      .pointer = objname };

	bridge = acpiphp_handle_to_bridge(handle);
	if (bridge) {
		acpi_get_name(handle, ACPI_FULL_PATHNAME, &buffer);
		dbg("%s: re-enumerating slots under %s\n",
			__func__, objname);
		acpiphp_check_bridge(bridge);
	}
	return AE_OK ;
}

<<<<<<< HEAD
/**
 * handle_hotplug_event_bridge - handle ACPI event on bridges
 * @handle: Notify()'ed acpi_handle
 * @type: Notify code
 * @context: pointer to acpiphp_bridge structure
 *
 * Handles ACPI event notification on {host,p2p} bridges.
 */
static void handle_hotplug_event_bridge(acpi_handle handle, u32 type, void *context)
=======
struct acpiphp_hp_work {
	struct work_struct work;
	acpi_handle handle;
	u32 type;
	void *context;
};

static void alloc_acpiphp_hp_work(acpi_handle handle, u32 type,
				  void *context,
				  void (*func)(struct work_struct *work))
{
	struct acpiphp_hp_work *hp_work;
	int ret;

	hp_work = kmalloc(sizeof(*hp_work), GFP_KERNEL);
	if (!hp_work)
		return;

	hp_work->handle = handle;
	hp_work->type = type;
	hp_work->context = context;

	INIT_WORK(&hp_work->work, func);
	ret = queue_work(kacpi_hotplug_wq, &hp_work->work);
	if (!ret)
		kfree(hp_work);
}

static void _handle_hotplug_event_bridge(struct work_struct *work)
>>>>>>> refs/remotes/origin/cm-10.0
{
=======
void acpiphp_check_host_bridge(acpi_handle handle)
{
	struct acpiphp_bridge *bridge;

	bridge = acpiphp_handle_to_bridge(handle);
	if (bridge) {
		acpiphp_check_bridge(bridge);
		put_bridge(bridge);
	}
}

static void hotplug_event(acpi_handle handle, u32 type, void *data)
{
	struct acpiphp_context *context = data;
	struct acpiphp_func *func = &context->func;
>>>>>>> refs/remotes/origin/master
	struct acpiphp_bridge *bridge;
	char objname[64];
	struct acpi_buffer buffer = { .length = sizeof(objname),
				      .pointer = objname };
<<<<<<< HEAD
	struct acpi_device *device;
	int num_sub_bridges = 0;
<<<<<<< HEAD
=======
	struct acpiphp_hp_work *hp_work;
	acpi_handle handle;
	u32 type;

	hp_work = container_of(work, struct acpiphp_hp_work, work);
	handle = hp_work->handle;
	type = hp_work->type;
>>>>>>> refs/remotes/origin/cm-10.0

	if (acpi_bus_get_device(handle, &device)) {
		/* This bridge must have just been physically inserted */
		handle_bridge_insertion(handle, type);
<<<<<<< HEAD
		return;
=======
		goto out;
>>>>>>> refs/remotes/origin/cm-10.0
	}

	bridge = acpiphp_handle_to_bridge(handle);
	if (type == ACPI_NOTIFY_BUS_CHECK) {
		acpi_walk_namespace(ACPI_TYPE_DEVICE, handle, ACPI_UINT32_MAX,
			count_sub_bridges, NULL, &num_sub_bridges, NULL);
	}

	if (!bridge && !num_sub_bridges) {
		err("cannot get bridge info\n");
<<<<<<< HEAD
		return;
=======
		goto out;
>>>>>>> refs/remotes/origin/cm-10.0
	}
=======

	mutex_lock(&acpiphp_context_lock);
	bridge = context->bridge;
	if (bridge)
		get_bridge(bridge);

	mutex_unlock(&acpiphp_context_lock);
>>>>>>> refs/remotes/origin/master

	acpi_get_name(handle, ACPI_FULL_PATHNAME, &buffer);

	switch (type) {
	case ACPI_NOTIFY_BUS_CHECK:
		/* bus re-enumerate */
<<<<<<< HEAD
		dbg("%s: Bus check notify on %s\n", __func__, objname);
		if (bridge) {
			dbg("%s: re-enumerating slots under %s\n",
				__func__, objname);
			acpiphp_check_bridge(bridge);
		}
		if (num_sub_bridges)
			acpi_walk_namespace(ACPI_TYPE_DEVICE, handle,
				ACPI_UINT32_MAX, check_sub_bridges, NULL, NULL, NULL);
=======
		pr_debug("%s: Bus check notify on %s\n", __func__, objname);
		pr_debug("%s: re-enumerating slots under %s\n",
			 __func__, objname);
		if (bridge) {
			acpiphp_check_bridge(bridge);
		} else {
			struct acpiphp_slot *slot = func->slot;

			mutex_lock(&slot->crit_sect);
			enable_slot(slot);
			mutex_unlock(&slot->crit_sect);
		}
>>>>>>> refs/remotes/origin/master
		break;

	case ACPI_NOTIFY_DEVICE_CHECK:
		/* device check */
<<<<<<< HEAD
		dbg("%s: Device check notify on %s\n", __func__, objname);
		acpiphp_check_bridge(bridge);
		break;

	case ACPI_NOTIFY_DEVICE_WAKE:
		/* wake event */
		dbg("%s: Device wake notify on %s\n", __func__, objname);
=======
		pr_debug("%s: Device check notify on %s\n", __func__, objname);
		if (bridge) {
			acpiphp_check_bridge(bridge);
		} else {
			struct acpiphp_slot *slot = func->slot;
			int ret;

			/*
			 * Check if anything has changed in the slot and rescan
			 * from the parent if that's the case.
			 */
			mutex_lock(&slot->crit_sect);
			ret = acpiphp_rescan_slot(slot);
			mutex_unlock(&slot->crit_sect);
			if (ret)
				acpiphp_check_bridge(func->parent);
		}
>>>>>>> refs/remotes/origin/master
		break;

	case ACPI_NOTIFY_EJECT_REQUEST:
		/* request device eject */
<<<<<<< HEAD
		dbg("%s: Device eject notify on %s\n", __func__, objname);
		if ((bridge->type != BRIDGE_TYPE_HOST) &&
		    (bridge->flags & BRIDGE_HAS_EJ0)) {
			struct acpiphp_slot *slot;
			slot = bridge->func->slot;
			if (!acpiphp_disable_slot(slot))
				acpiphp_eject_slot(slot);
		}
		break;

	case ACPI_NOTIFY_FREQUENCY_MISMATCH:
		printk(KERN_ERR "Device %s cannot be configured due"
				" to a frequency mismatch\n", objname);
		break;

	case ACPI_NOTIFY_BUS_MODE_MISMATCH:
		printk(KERN_ERR "Device %s cannot be configured due"
				" to a bus mode mismatch\n", objname);
		break;

	case ACPI_NOTIFY_POWER_FAULT:
		printk(KERN_ERR "Device %s has suffered a power fault\n",
				objname);
		break;

	default:
		warn("notify_handler: unknown event type 0x%x for %s\n", type, objname);
		break;
	}
<<<<<<< HEAD
}

/**
 * handle_hotplug_event_func - handle ACPI event on functions (i.e. slots)
 * @handle: Notify()'ed acpi_handle
 * @type: Notify code
 * @context: pointer to acpiphp_func structure
 *
 * Handles ACPI event notification on slots.
 */
static void handle_hotplug_event_func(acpi_handle handle, u32 type, void *context)
=======

out:
	kfree(hp_work); /* allocated in handle_hotplug_event_bridge */
}

/**
 * handle_hotplug_event_bridge - handle ACPI event on bridges
 * @handle: Notify()'ed acpi_handle
 * @type: Notify code
 * @context: pointer to acpiphp_bridge structure
 *
 * Handles ACPI event notification on {host,p2p} bridges.
 */
static void handle_hotplug_event_bridge(acpi_handle handle, u32 type,
					void *context)
{
	/*
	 * Currently the code adds all hotplug events to the kacpid_wq
	 * queue when it should add hotplug events to the kacpi_hotplug_wq.
	 * The proper way to fix this is to reorganize the code so that
	 * drivers (dock, etc.) do not call acpi_os_execute(), etc.
	 * For now just re-add this work to the kacpi_hotplug_wq so we
	 * don't deadlock on hotplug actions.
	 */
	alloc_acpiphp_hp_work(handle, type, context,
			      _handle_hotplug_event_bridge);
}

static void _handle_hotplug_event_func(struct work_struct *work)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct acpiphp_func *func;
	char objname[64];
	struct acpi_buffer buffer = { .length = sizeof(objname),
				      .pointer = objname };
<<<<<<< HEAD
=======
	struct acpiphp_hp_work *hp_work;
	acpi_handle handle;
	u32 type;
	void *context;

	hp_work = container_of(work, struct acpiphp_hp_work, work);
	handle = hp_work->handle;
	type = hp_work->type;
	context = hp_work->context;
>>>>>>> refs/remotes/origin/cm-10.0

	acpi_get_name(handle, ACPI_FULL_PATHNAME, &buffer);

	func = (struct acpiphp_func *)context;

	switch (type) {
	case ACPI_NOTIFY_BUS_CHECK:
		/* bus re-enumerate */
		dbg("%s: Bus check notify on %s\n", __func__, objname);
		acpiphp_enable_slot(func->slot);
		break;

	case ACPI_NOTIFY_DEVICE_CHECK:
		/* device check : re-enumerate from parent bus */
		dbg("%s: Device check notify on %s\n", __func__, objname);
		acpiphp_check_bridge(func->slot->bridge);
		break;

	case ACPI_NOTIFY_DEVICE_WAKE:
		/* wake event */
		dbg("%s: Device wake notify on %s\n", __func__, objname);
		break;

	case ACPI_NOTIFY_EJECT_REQUEST:
		/* request device eject */
		dbg("%s: Device eject notify on %s\n", __func__, objname);
		if (!(acpiphp_disable_slot(func->slot)))
			acpiphp_eject_slot(func->slot);
		break;

	default:
		warn("notify_handler: unknown event type 0x%x for %s\n", type, objname);
		break;
	}
<<<<<<< HEAD
}

=======

	kfree(hp_work); /* allocated in handle_hotplug_event_func */
}

/**
 * handle_hotplug_event_func - handle ACPI event on functions (i.e. slots)
 * @handle: Notify()'ed acpi_handle
 * @type: Notify code
 * @context: pointer to acpiphp_func structure
 *
 * Handles ACPI event notification on slots.
 */
static void handle_hotplug_event_func(acpi_handle handle, u32 type,
				      void *context)
{
	/*
	 * Currently the code adds all hotplug events to the kacpid_wq
	 * queue when it should add hotplug events to the kacpi_hotplug_wq.
	 * The proper way to fix this is to reorganize the code so that
	 * drivers (dock, etc.) do not call acpi_os_execute(), etc.
	 * For now just re-add this work to the kacpi_hotplug_wq so we
	 * don't deadlock on hotplug actions.
	 */
	alloc_acpiphp_hp_work(handle, type, context,
			      _handle_hotplug_event_func);
}
>>>>>>> refs/remotes/origin/cm-10.0

static acpi_status
find_root_bridges(acpi_handle handle, u32 lvl, void *context, void **rv)
{
	int *count = (int *)context;

<<<<<<< HEAD
	if (acpi_is_root_bridge(handle)) {
		acpi_install_notify_handler(handle, ACPI_SYSTEM_NOTIFY,
				handle_hotplug_event_bridge, NULL);
			(*count)++;
	}
=======
	if (!acpi_is_root_bridge(handle))
		return AE_OK;

	(*count)++;
	acpi_install_notify_handler(handle, ACPI_SYSTEM_NOTIFY,
				    handle_hotplug_event_bridge, NULL);

>>>>>>> refs/remotes/origin/cm-10.0
	return AE_OK ;
}

static struct acpi_pci_driver acpi_pci_hp_driver = {
	.add =		add_bridge,
	.remove =	remove_bridge,
};

/**
 * acpiphp_glue_init - initializes all PCI hotplug - ACPI glue data structures
 */
int __init acpiphp_glue_init(void)
{
	int num = 0;

	acpi_walk_namespace(ACPI_TYPE_DEVICE, ACPI_ROOT_OBJECT,
			ACPI_UINT32_MAX, find_root_bridges, NULL, &num, NULL);

	if (num <= 0)
		return -1;
	else
		acpi_pci_register_driver(&acpi_pci_hp_driver);

	return 0;
}


/**
 * acpiphp_glue_exit - terminates all PCI hotplug - ACPI glue data structures
 *
 * This function frees all data allocated in acpiphp_glue_init().
 */
void  acpiphp_glue_exit(void)
{
	acpi_pci_unregister_driver(&acpi_pci_hp_driver);
}


/**
 * acpiphp_get_num_slots - count number of slots in a system
 */
int __init acpiphp_get_num_slots(void)
{
	struct acpiphp_bridge *bridge;
	int num_slots = 0;

	list_for_each_entry(bridge, &bridge_list, list) {
		dbg("Bus %04x:%02x has %d slot%s\n",
				pci_domain_nr(bridge->pci_bus),
				bridge->pci_bus->number, bridge->nr_slots,
				bridge->nr_slots == 1 ? "" : "s");
		num_slots += bridge->nr_slots;
	}

	dbg("Total %d slots\n", num_slots);
	return num_slots;
}


#if 0
/**
 * acpiphp_for_each_slot - call function for each slot
 * @fn: callback function
 * @data: context to be passed to callback function
 */
static int acpiphp_for_each_slot(acpiphp_callback fn, void *data)
{
	struct list_head *node;
	struct acpiphp_bridge *bridge;
	struct acpiphp_slot *slot;
	int retval = 0;

	list_for_each (node, &bridge_list) {
		bridge = (struct acpiphp_bridge *)node;
		for (slot = bridge->slots; slot; slot = slot->next) {
			retval = fn(slot, data);
			if (!retval)
				goto err_exit;
		}
	}

 err_exit:
	return retval;
}
#endif

=======
		pr_debug("%s: Device eject notify on %s\n", __func__, objname);
		acpiphp_disable_and_eject_slot(func->slot);
		break;
	}

	if (bridge)
		put_bridge(bridge);
}

static void hotplug_event_work(void *data, u32 type)
{
	struct acpiphp_context *context = data;
	acpi_handle handle = context->handle;

	acpi_scan_lock_acquire();

	hotplug_event(handle, type, context);

	acpi_scan_lock_release();
	acpi_evaluate_hotplug_ost(handle, type, ACPI_OST_SC_SUCCESS, NULL);
	put_bridge(context->func.parent);
}

/**
 * handle_hotplug_event - handle ACPI hotplug event
 * @handle: Notify()'ed acpi_handle
 * @type: Notify code
 * @data: pointer to acpiphp_context structure
 *
 * Handles ACPI event notification on slots.
 */
static void handle_hotplug_event(acpi_handle handle, u32 type, void *data)
{
	struct acpiphp_context *context;
	u32 ost_code = ACPI_OST_SC_SUCCESS;

	switch (type) {
	case ACPI_NOTIFY_BUS_CHECK:
	case ACPI_NOTIFY_DEVICE_CHECK:
		break;
	case ACPI_NOTIFY_EJECT_REQUEST:
		ost_code = ACPI_OST_SC_EJECT_IN_PROGRESS;
		acpi_evaluate_hotplug_ost(handle, type, ost_code, NULL);
		break;

	case ACPI_NOTIFY_DEVICE_WAKE:
		return;

	case ACPI_NOTIFY_FREQUENCY_MISMATCH:
		acpi_handle_err(handle, "Device cannot be configured due "
				"to a frequency mismatch\n");
		goto out;

	case ACPI_NOTIFY_BUS_MODE_MISMATCH:
		acpi_handle_err(handle, "Device cannot be configured due "
				"to a bus mode mismatch\n");
		goto out;

	case ACPI_NOTIFY_POWER_FAULT:
		acpi_handle_err(handle, "Device has suffered a power fault\n");
		goto out;

	default:
		acpi_handle_warn(handle, "Unsupported event type 0x%x\n", type);
		ost_code = ACPI_OST_SC_UNRECOGNIZED_NOTIFY;
		goto out;
	}

	mutex_lock(&acpiphp_context_lock);
	context = acpiphp_get_context(handle);
	if (context && !WARN_ON(context->handle != handle)) {
		get_bridge(context->func.parent);
		acpiphp_put_context(context);
		acpi_hotplug_execute(hotplug_event_work, context, type);
		mutex_unlock(&acpiphp_context_lock);
		return;
	}
	mutex_unlock(&acpiphp_context_lock);
	ost_code = ACPI_OST_SC_NON_SPECIFIC_FAILURE;

 out:
	acpi_evaluate_hotplug_ost(handle, type, ost_code, NULL);
}

/*
 * Create hotplug slots for the PCI bus.
 * It should always return 0 to avoid skipping following notifiers.
 */
void acpiphp_enumerate_slots(struct pci_bus *bus)
{
	struct acpiphp_bridge *bridge;
	acpi_handle handle;
	acpi_status status;

	if (acpiphp_disabled)
		return;

	handle = ACPI_HANDLE(bus->bridge);
	if (!handle)
		return;

	bridge = kzalloc(sizeof(struct acpiphp_bridge), GFP_KERNEL);
	if (!bridge) {
		acpi_handle_err(handle, "No memory for bridge object\n");
		return;
	}

	INIT_LIST_HEAD(&bridge->slots);
	kref_init(&bridge->ref);
	bridge->pci_dev = pci_dev_get(bus->self);
	bridge->pci_bus = bus;

	/*
	 * Grab a ref to the subordinate PCI bus in case the bus is
	 * removed via PCI core logical hotplug. The ref pins the bus
	 * (which we access during module unload).
	 */
	get_device(&bus->dev);

	if (!pci_is_root_bus(bridge->pci_bus)) {
		struct acpiphp_context *context;

		/*
		 * This bridge should have been registered as a hotplug function
		 * under its parent, so the context should be there, unless the
		 * parent is going to be handled by pciehp, in which case this
		 * bridge is not interesting to us either.
		 */
		mutex_lock(&acpiphp_context_lock);
		context = acpiphp_get_context(handle);
		if (!context) {
			mutex_unlock(&acpiphp_context_lock);
			put_device(&bus->dev);
			pci_dev_put(bridge->pci_dev);
			kfree(bridge);
			return;
		}
		bridge->context = context;
		context->bridge = bridge;
		/* Get a reference to the parent bridge. */
		get_bridge(context->func.parent);
		mutex_unlock(&acpiphp_context_lock);
	}

	/* must be added to the list prior to calling register_slot */
	mutex_lock(&bridge_mutex);
	list_add(&bridge->list, &bridge_list);
	mutex_unlock(&bridge_mutex);

	/* register all slot objects under this bridge */
	status = acpi_walk_namespace(ACPI_TYPE_DEVICE, handle, 1,
				     register_slot, NULL, bridge, NULL);
	if (ACPI_FAILURE(status)) {
		acpi_handle_err(handle, "failed to register slots\n");
		cleanup_bridge(bridge);
		put_bridge(bridge);
	}
}

/* Destroy hotplug slots associated with the PCI bus */
void acpiphp_remove_slots(struct pci_bus *bus)
{
	struct acpiphp_bridge *bridge;

	if (acpiphp_disabled)
		return;

	mutex_lock(&bridge_mutex);
	list_for_each_entry(bridge, &bridge_list, list)
		if (bridge->pci_bus == bus) {
			mutex_unlock(&bridge_mutex);
			cleanup_bridge(bridge);
			put_bridge(bridge);
			return;
		}

	mutex_unlock(&bridge_mutex);
}
>>>>>>> refs/remotes/origin/master

/**
 * acpiphp_enable_slot - power on slot
 * @slot: ACPI PHP slot
 */
int acpiphp_enable_slot(struct acpiphp_slot *slot)
{
<<<<<<< HEAD
	int retval;

	mutex_lock(&slot->crit_sect);

	/* wake up all functions */
	retval = power_on_slot(slot);
	if (retval)
		goto err_exit;

	if (get_slot_status(slot) == ACPI_STA_ALL) {
		/* configure all functions */
		retval = enable_device(slot);
		if (retval)
			power_off_slot(slot);
	} else {
		dbg("%s: Slot status is not ACPI_STA_ALL\n", __func__);
		power_off_slot(slot);
	}

 err_exit:
	mutex_unlock(&slot->crit_sect);
	return retval;
}

/**
 * acpiphp_disable_slot - power off slot
 * @slot: ACPI PHP slot
 */
int acpiphp_disable_slot(struct acpiphp_slot *slot)
{
=======
	mutex_lock(&slot->crit_sect);
	/* configure all functions */
	if (!(slot->flags & SLOT_ENABLED))
		enable_slot(slot);

	mutex_unlock(&slot->crit_sect);
	return 0;
}

/**
 * acpiphp_disable_and_eject_slot - power off and eject slot
 * @slot: ACPI PHP slot
 */
int acpiphp_disable_and_eject_slot(struct acpiphp_slot *slot)
{
	struct acpiphp_func *func;
>>>>>>> refs/remotes/origin/master
	int retval = 0;

	mutex_lock(&slot->crit_sect);

	/* unconfigure all functions */
<<<<<<< HEAD
	retval = disable_device(slot);
	if (retval)
		goto err_exit;

	/* power off all functions */
	retval = power_off_slot(slot);
	if (retval)
		goto err_exit;

 err_exit:
=======
	disable_slot(slot);

	list_for_each_entry(func, &slot->funcs, sibling)
		if (func->flags & FUNC_HAS_EJ0) {
			acpi_handle handle = func_to_handle(func);

			if (ACPI_FAILURE(acpi_evaluate_ej0(handle)))
				acpi_handle_err(handle, "_EJ0 failed\n");

			break;
		}

>>>>>>> refs/remotes/origin/master
	mutex_unlock(&slot->crit_sect);
	return retval;
}


/*
 * slot enabled:  1
 * slot disabled: 0
 */
u8 acpiphp_get_power_status(struct acpiphp_slot *slot)
{
<<<<<<< HEAD
	return (slot->flags & SLOT_POWEREDON);
=======
	return (slot->flags & SLOT_ENABLED);
>>>>>>> refs/remotes/origin/master
}


/*
 * latch   open:  1
 * latch closed:  0
 */
u8 acpiphp_get_latch_status(struct acpiphp_slot *slot)
{
<<<<<<< HEAD
	unsigned int sta;

	sta = get_slot_status(slot);

	return (sta & ACPI_STA_SHOW_IN_UI) ? 0 : 1;
=======
	return !(get_slot_status(slot) & ACPI_STA_DEVICE_UI);
>>>>>>> refs/remotes/origin/master
}


/*
 * adapter presence : 1
 *          absence : 0
 */
u8 acpiphp_get_adapter_status(struct acpiphp_slot *slot)
{
<<<<<<< HEAD
	unsigned int sta;

	sta = get_slot_status(slot);

	return (sta == 0) ? 0 : 1;
=======
	return !!get_slot_status(slot);
>>>>>>> refs/remotes/origin/master
}
