/*
 * drivers/base/power/clock_ops.c - Generic clock manipulation PM callbacks
 *
 * Copyright (c) 2011 Rafael J. Wysocki <rjw@sisk.pl>, Renesas Electronics Corp.
 *
 * This file is released under the GPLv2.
 */

#include <linux/init.h>
#include <linux/kernel.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/io.h>
#include <linux/pm.h>
#include <linux/pm_runtime.h>
=======
=======
>>>>>>> refs/remotes/origin/master
#include <linux/device.h>
#include <linux/io.h>
#include <linux/pm.h>
#include <linux/pm_clock.h>
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#include <linux/clk.h>
#include <linux/slab.h>
#include <linux/err.h>

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_PM_RUNTIME

struct pm_runtime_clk_data {
	struct list_head clock_list;
	struct mutex lock;
};
=======
#ifdef CONFIG_PM
>>>>>>> refs/remotes/origin/cm-10.0
=======
#ifdef CONFIG_PM
>>>>>>> refs/remotes/origin/master

enum pce_status {
	PCE_STATUS_NONE = 0,
	PCE_STATUS_ACQUIRED,
	PCE_STATUS_ENABLED,
	PCE_STATUS_ERROR,
};

struct pm_clock_entry {
	struct list_head node;
	char *con_id;
	struct clk *clk;
	enum pce_status status;
};

<<<<<<< HEAD
<<<<<<< HEAD
static struct pm_runtime_clk_data *__to_prd(struct device *dev)
{
	return dev ? dev->power.subsys_data : NULL;
}

/**
 * pm_runtime_clk_add - Start using a device clock for runtime PM.
 * @dev: Device whose clock is going to be used for runtime PM.
 * @con_id: Connection ID of the clock.
 *
 * Add the clock represented by @con_id to the list of clocks used for
 * the runtime PM of @dev.
 */
int pm_runtime_clk_add(struct device *dev, const char *con_id)
{
	struct pm_runtime_clk_data *prd = __to_prd(dev);
	struct pm_clock_entry *ce;

	if (!prd)
=======
=======
>>>>>>> refs/remotes/origin/master
/**
 * pm_clk_acquire - Acquire a device clock.
 * @dev: Device whose clock is to be acquired.
 * @ce: PM clock entry corresponding to the clock.
 */
static void pm_clk_acquire(struct device *dev, struct pm_clock_entry *ce)
{
	ce->clk = clk_get(dev, ce->con_id);
	if (IS_ERR(ce->clk)) {
		ce->status = PCE_STATUS_ERROR;
	} else {
		ce->status = PCE_STATUS_ACQUIRED;
		dev_dbg(dev, "Clock %s managed by runtime PM.\n", ce->con_id);
	}
}

/**
 * pm_clk_add - Start using a device clock for power management.
 * @dev: Device whose clock is going to be used for power management.
 * @con_id: Connection ID of the clock.
 *
 * Add the clock represented by @con_id to the list of clocks used for
 * the power management of @dev.
 */
int pm_clk_add(struct device *dev, const char *con_id)
{
	struct pm_subsys_data *psd = dev_to_psd(dev);
	struct pm_clock_entry *ce;

	if (!psd)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		return -EINVAL;

	ce = kzalloc(sizeof(*ce), GFP_KERNEL);
	if (!ce) {
		dev_err(dev, "Not enough memory for clock entry.\n");
		return -ENOMEM;
	}

	if (con_id) {
		ce->con_id = kstrdup(con_id, GFP_KERNEL);
		if (!ce->con_id) {
			dev_err(dev,
				"Not enough memory for clock connection ID.\n");
			kfree(ce);
			return -ENOMEM;
		}
	}

<<<<<<< HEAD
<<<<<<< HEAD
	mutex_lock(&prd->lock);
	list_add_tail(&ce->node, &prd->clock_list);
	mutex_unlock(&prd->lock);
=======
=======
>>>>>>> refs/remotes/origin/master
	pm_clk_acquire(dev, ce);

	spin_lock_irq(&psd->lock);
	list_add_tail(&ce->node, &psd->clock_list);
	spin_unlock_irq(&psd->lock);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return 0;
}

/**
<<<<<<< HEAD
<<<<<<< HEAD
 * __pm_runtime_clk_remove - Destroy runtime PM clock entry.
 * @ce: Runtime PM clock entry to destroy.
 *
 * This routine must be called under the mutex protecting the runtime PM list
 * of clocks corresponding the the @ce's device.
 */
static void __pm_runtime_clk_remove(struct pm_clock_entry *ce)
=======
=======
>>>>>>> refs/remotes/origin/master
 * __pm_clk_remove - Destroy PM clock entry.
 * @ce: PM clock entry to destroy.
 */
static void __pm_clk_remove(struct pm_clock_entry *ce)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	if (!ce)
		return;

<<<<<<< HEAD
<<<<<<< HEAD
	list_del(&ce->node);

=======
>>>>>>> refs/remotes/origin/cm-10.0
	if (ce->status < PCE_STATUS_ERROR) {
		if (ce->status == PCE_STATUS_ENABLED)
			clk_disable(ce->clk);
=======
	if (ce->status < PCE_STATUS_ERROR) {
		if (ce->status == PCE_STATUS_ENABLED)
			clk_disable_unprepare(ce->clk);
>>>>>>> refs/remotes/origin/master

		if (ce->status >= PCE_STATUS_ACQUIRED)
			clk_put(ce->clk);
	}

<<<<<<< HEAD
<<<<<<< HEAD
	if (ce->con_id)
		kfree(ce->con_id);

=======
	kfree(ce->con_id);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	kfree(ce->con_id);
>>>>>>> refs/remotes/origin/master
	kfree(ce);
}

/**
<<<<<<< HEAD
<<<<<<< HEAD
 * pm_runtime_clk_remove - Stop using a device clock for runtime PM.
 * @dev: Device whose clock should not be used for runtime PM any more.
 * @con_id: Connection ID of the clock.
 *
 * Remove the clock represented by @con_id from the list of clocks used for
 * the runtime PM of @dev.
 */
void pm_runtime_clk_remove(struct device *dev, const char *con_id)
{
	struct pm_runtime_clk_data *prd = __to_prd(dev);
	struct pm_clock_entry *ce;

	if (!prd)
		return;

	mutex_lock(&prd->lock);

	list_for_each_entry(ce, &prd->clock_list, node) {
		if (!con_id && !ce->con_id) {
			__pm_runtime_clk_remove(ce);
			break;
		} else if (!con_id || !ce->con_id) {
			continue;
		} else if (!strcmp(con_id, ce->con_id)) {
			__pm_runtime_clk_remove(ce);
			break;
		}
	}

	mutex_unlock(&prd->lock);
}

/**
 * pm_runtime_clk_init - Initialize a device's list of runtime PM clocks.
 * @dev: Device to initialize the list of runtime PM clocks for.
 *
 * Allocate a struct pm_runtime_clk_data object, initialize its lock member and
 * make the @dev's power.subsys_data field point to it.
 */
int pm_runtime_clk_init(struct device *dev)
{
	struct pm_runtime_clk_data *prd;

	prd = kzalloc(sizeof(*prd), GFP_KERNEL);
	if (!prd) {
		dev_err(dev, "Not enough memory fo runtime PM data.\n");
		return -ENOMEM;
	}

	INIT_LIST_HEAD(&prd->clock_list);
	mutex_init(&prd->lock);
	dev->power.subsys_data = prd;
	return 0;
}

/**
 * pm_runtime_clk_destroy - Destroy a device's list of runtime PM clocks.
 * @dev: Device to destroy the list of runtime PM clocks for.
 *
 * Clear the @dev's power.subsys_data field, remove the list of clock entries
 * from the struct pm_runtime_clk_data object pointed to by it before and free
 * that object.
 */
void pm_runtime_clk_destroy(struct device *dev)
{
	struct pm_runtime_clk_data *prd = __to_prd(dev);
	struct pm_clock_entry *ce, *c;

	if (!prd)
		return;

	dev->power.subsys_data = NULL;

	mutex_lock(&prd->lock);

	list_for_each_entry_safe_reverse(ce, c, &prd->clock_list, node)
		__pm_runtime_clk_remove(ce);

	mutex_unlock(&prd->lock);

	kfree(prd);
}

/**
 * pm_runtime_clk_acquire - Acquire a device clock.
 * @dev: Device whose clock is to be acquired.
 * @con_id: Connection ID of the clock.
 */
static void pm_runtime_clk_acquire(struct device *dev,
				    struct pm_clock_entry *ce)
{
	ce->clk = clk_get(dev, ce->con_id);
	if (IS_ERR(ce->clk)) {
		ce->status = PCE_STATUS_ERROR;
	} else {
		ce->status = PCE_STATUS_ACQUIRED;
		dev_dbg(dev, "Clock %s managed by runtime PM.\n", ce->con_id);
	}
}

/**
 * pm_runtime_clk_suspend - Disable clocks in a device's runtime PM clock list.
 * @dev: Device to disable the clocks for.
 */
int pm_runtime_clk_suspend(struct device *dev)
{
	struct pm_runtime_clk_data *prd = __to_prd(dev);
	struct pm_clock_entry *ce;

	dev_dbg(dev, "%s()\n", __func__);

	if (!prd)
		return 0;

	mutex_lock(&prd->lock);

	list_for_each_entry_reverse(ce, &prd->clock_list, node) {
		if (ce->status == PCE_STATUS_NONE)
			pm_runtime_clk_acquire(dev, ce);

		if (ce->status < PCE_STATUS_ERROR) {
			clk_disable(ce->clk);
=======
=======
>>>>>>> refs/remotes/origin/master
 * pm_clk_remove - Stop using a device clock for power management.
 * @dev: Device whose clock should not be used for PM any more.
 * @con_id: Connection ID of the clock.
 *
 * Remove the clock represented by @con_id from the list of clocks used for
 * the power management of @dev.
 */
void pm_clk_remove(struct device *dev, const char *con_id)
{
	struct pm_subsys_data *psd = dev_to_psd(dev);
	struct pm_clock_entry *ce;

	if (!psd)
		return;

	spin_lock_irq(&psd->lock);

	list_for_each_entry(ce, &psd->clock_list, node) {
		if (!con_id && !ce->con_id)
			goto remove;
		else if (!con_id || !ce->con_id)
			continue;
		else if (!strcmp(con_id, ce->con_id))
			goto remove;
	}

	spin_unlock_irq(&psd->lock);
	return;

 remove:
	list_del(&ce->node);
	spin_unlock_irq(&psd->lock);

	__pm_clk_remove(ce);
}

/**
 * pm_clk_init - Initialize a device's list of power management clocks.
 * @dev: Device to initialize the list of PM clocks for.
 *
 * Initialize the lock and clock_list members of the device's pm_subsys_data
 * object.
 */
void pm_clk_init(struct device *dev)
{
	struct pm_subsys_data *psd = dev_to_psd(dev);
	if (psd)
		INIT_LIST_HEAD(&psd->clock_list);
}

/**
 * pm_clk_create - Create and initialize a device's list of PM clocks.
 * @dev: Device to create and initialize the list of PM clocks for.
 *
 * Allocate a struct pm_subsys_data object, initialize its lock and clock_list
 * members and make the @dev's power.subsys_data field point to it.
 */
int pm_clk_create(struct device *dev)
{
<<<<<<< HEAD
	int ret = dev_pm_get_subsys_data(dev);
	return ret < 0 ? ret : 0;
=======
	return dev_pm_get_subsys_data(dev);
>>>>>>> refs/remotes/origin/master
}

/**
 * pm_clk_destroy - Destroy a device's list of power management clocks.
 * @dev: Device to destroy the list of PM clocks for.
 *
 * Clear the @dev's power.subsys_data field, remove the list of clock entries
 * from the struct pm_subsys_data object pointed to by it before and free
 * that object.
 */
void pm_clk_destroy(struct device *dev)
{
	struct pm_subsys_data *psd = dev_to_psd(dev);
	struct pm_clock_entry *ce, *c;
	struct list_head list;

	if (!psd)
		return;

	INIT_LIST_HEAD(&list);

	spin_lock_irq(&psd->lock);

	list_for_each_entry_safe_reverse(ce, c, &psd->clock_list, node)
		list_move(&ce->node, &list);

	spin_unlock_irq(&psd->lock);

	dev_pm_put_subsys_data(dev);

	list_for_each_entry_safe_reverse(ce, c, &list, node) {
		list_del(&ce->node);
		__pm_clk_remove(ce);
	}
}

#endif /* CONFIG_PM */

#ifdef CONFIG_PM_RUNTIME

/**
 * pm_clk_suspend - Disable clocks in a device's PM clock list.
 * @dev: Device to disable the clocks for.
 */
int pm_clk_suspend(struct device *dev)
{
	struct pm_subsys_data *psd = dev_to_psd(dev);
	struct pm_clock_entry *ce;
	unsigned long flags;

	dev_dbg(dev, "%s()\n", __func__);

	if (!psd)
		return 0;

	spin_lock_irqsave(&psd->lock, flags);

	list_for_each_entry_reverse(ce, &psd->clock_list, node) {
		if (ce->status < PCE_STATUS_ERROR) {
			if (ce->status == PCE_STATUS_ENABLED)
				clk_disable(ce->clk);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			ce->status = PCE_STATUS_ACQUIRED;
		}
	}

<<<<<<< HEAD
<<<<<<< HEAD
	mutex_unlock(&prd->lock);
=======
	spin_unlock_irqrestore(&psd->lock, flags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	spin_unlock_irqrestore(&psd->lock, flags);
>>>>>>> refs/remotes/origin/master

	return 0;
}

/**
<<<<<<< HEAD
<<<<<<< HEAD
 * pm_runtime_clk_resume - Enable clocks in a device's runtime PM clock list.
 * @dev: Device to enable the clocks for.
 */
int pm_runtime_clk_resume(struct device *dev)
{
	struct pm_runtime_clk_data *prd = __to_prd(dev);
	struct pm_clock_entry *ce;

	dev_dbg(dev, "%s()\n", __func__);

	if (!prd)
		return 0;

	mutex_lock(&prd->lock);

	list_for_each_entry(ce, &prd->clock_list, node) {
		if (ce->status == PCE_STATUS_NONE)
			pm_runtime_clk_acquire(dev, ce);

=======
=======
>>>>>>> refs/remotes/origin/master
 * pm_clk_resume - Enable clocks in a device's PM clock list.
 * @dev: Device to enable the clocks for.
 */
int pm_clk_resume(struct device *dev)
{
	struct pm_subsys_data *psd = dev_to_psd(dev);
	struct pm_clock_entry *ce;
	unsigned long flags;

	dev_dbg(dev, "%s()\n", __func__);

	if (!psd)
		return 0;

	spin_lock_irqsave(&psd->lock, flags);

	list_for_each_entry(ce, &psd->clock_list, node) {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		if (ce->status < PCE_STATUS_ERROR) {
			clk_enable(ce->clk);
			ce->status = PCE_STATUS_ENABLED;
		}
	}

<<<<<<< HEAD
<<<<<<< HEAD
	mutex_unlock(&prd->lock);
=======
	spin_unlock_irqrestore(&psd->lock, flags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	spin_unlock_irqrestore(&psd->lock, flags);
>>>>>>> refs/remotes/origin/master

	return 0;
}

/**
<<<<<<< HEAD
<<<<<<< HEAD
 * pm_runtime_clk_notify - Notify routine for device addition and removal.
=======
 * pm_clk_notify - Notify routine for device addition and removal.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * pm_clk_notify - Notify routine for device addition and removal.
>>>>>>> refs/remotes/origin/master
 * @nb: Notifier block object this function is a member of.
 * @action: Operation being carried out by the caller.
 * @data: Device the routine is being run for.
 *
 * For this function to work, @nb must be a member of an object of type
 * struct pm_clk_notifier_block containing all of the requisite data.
<<<<<<< HEAD
<<<<<<< HEAD
 * Specifically, the pwr_domain member of that object is copied to the device's
 * pwr_domain field and its con_ids member is used to populate the device's list
 * of runtime PM clocks, depending on @action.
 *
 * If the device's pwr_domain field is already populated with a value different
 * from the one stored in the struct pm_clk_notifier_block object, the function
 * does nothing.
 */
static int pm_runtime_clk_notify(struct notifier_block *nb,
=======
=======
>>>>>>> refs/remotes/origin/master
 * Specifically, the pm_domain member of that object is copied to the device's
 * pm_domain field and its con_ids member is used to populate the device's list
 * of PM clocks, depending on @action.
 *
 * If the device's pm_domain field is already populated with a value different
 * from the one stored in the struct pm_clk_notifier_block object, the function
 * does nothing.
 */
static int pm_clk_notify(struct notifier_block *nb,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				 unsigned long action, void *data)
{
	struct pm_clk_notifier_block *clknb;
	struct device *dev = data;
	char **con_id;
	int error;

	dev_dbg(dev, "%s() %ld\n", __func__, action);

	clknb = container_of(nb, struct pm_clk_notifier_block, nb);

	switch (action) {
	case BUS_NOTIFY_ADD_DEVICE:
<<<<<<< HEAD
<<<<<<< HEAD
		if (dev->pwr_domain)
			break;

		error = pm_runtime_clk_init(dev);
		if (error)
			break;

		dev->pwr_domain = clknb->pwr_domain;
		if (clknb->con_ids[0]) {
			for (con_id = clknb->con_ids; *con_id; con_id++)
				pm_runtime_clk_add(dev, *con_id);
		} else {
			pm_runtime_clk_add(dev, NULL);
=======
=======
>>>>>>> refs/remotes/origin/master
		if (dev->pm_domain)
			break;

		error = pm_clk_create(dev);
		if (error)
			break;

		dev->pm_domain = clknb->pm_domain;
		if (clknb->con_ids[0]) {
			for (con_id = clknb->con_ids; *con_id; con_id++)
				pm_clk_add(dev, *con_id);
		} else {
			pm_clk_add(dev, NULL);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		}

		break;
	case BUS_NOTIFY_DEL_DEVICE:
<<<<<<< HEAD
<<<<<<< HEAD
		if (dev->pwr_domain != clknb->pwr_domain)
			break;

		dev->pwr_domain = NULL;
		pm_runtime_clk_destroy(dev);
=======
=======
>>>>>>> refs/remotes/origin/master
		if (dev->pm_domain != clknb->pm_domain)
			break;

		dev->pm_domain = NULL;
		pm_clk_destroy(dev);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		break;
	}

	return 0;
}

#else /* !CONFIG_PM_RUNTIME */

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_PM

/**
 * pm_clk_suspend - Disable clocks in a device's PM clock list.
 * @dev: Device to disable the clocks for.
 */
int pm_clk_suspend(struct device *dev)
{
	struct pm_subsys_data *psd = dev_to_psd(dev);
	struct pm_clock_entry *ce;
	unsigned long flags;

	dev_dbg(dev, "%s()\n", __func__);

	/* If there is no driver, the clocks are already disabled. */
	if (!psd || !dev->driver)
		return 0;

	spin_lock_irqsave(&psd->lock, flags);

	list_for_each_entry_reverse(ce, &psd->clock_list, node)
		clk_disable(ce->clk);

	spin_unlock_irqrestore(&psd->lock, flags);

	return 0;
}

/**
 * pm_clk_resume - Enable clocks in a device's PM clock list.
 * @dev: Device to enable the clocks for.
 */
int pm_clk_resume(struct device *dev)
{
	struct pm_subsys_data *psd = dev_to_psd(dev);
	struct pm_clock_entry *ce;
	unsigned long flags;

	dev_dbg(dev, "%s()\n", __func__);

	/* If there is no driver, the clocks should remain disabled. */
	if (!psd || !dev->driver)
		return 0;

	spin_lock_irqsave(&psd->lock, flags);

	list_for_each_entry(ce, &psd->clock_list, node)
		clk_enable(ce->clk);

	spin_unlock_irqrestore(&psd->lock, flags);

	return 0;
}

#endif /* CONFIG_PM */

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/**
 * enable_clock - Enable a device clock.
 * @dev: Device whose clock is to be enabled.
 * @con_id: Connection ID of the clock.
 */
static void enable_clock(struct device *dev, const char *con_id)
{
	struct clk *clk;

	clk = clk_get(dev, con_id);
	if (!IS_ERR(clk)) {
<<<<<<< HEAD
		clk_enable(clk);
=======
		clk_prepare_enable(clk);
>>>>>>> refs/remotes/origin/master
		clk_put(clk);
		dev_info(dev, "Runtime PM disabled, clock forced on.\n");
	}
}

/**
 * disable_clock - Disable a device clock.
 * @dev: Device whose clock is to be disabled.
 * @con_id: Connection ID of the clock.
 */
static void disable_clock(struct device *dev, const char *con_id)
{
	struct clk *clk;

	clk = clk_get(dev, con_id);
	if (!IS_ERR(clk)) {
<<<<<<< HEAD
		clk_disable(clk);
=======
		clk_disable_unprepare(clk);
>>>>>>> refs/remotes/origin/master
		clk_put(clk);
		dev_info(dev, "Runtime PM disabled, clock forced off.\n");
	}
}

/**
<<<<<<< HEAD
<<<<<<< HEAD
 * pm_runtime_clk_notify - Notify routine for device addition and removal.
=======
 * pm_clk_notify - Notify routine for device addition and removal.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * pm_clk_notify - Notify routine for device addition and removal.
>>>>>>> refs/remotes/origin/master
 * @nb: Notifier block object this function is a member of.
 * @action: Operation being carried out by the caller.
 * @data: Device the routine is being run for.
 *
 * For this function to work, @nb must be a member of an object of type
 * struct pm_clk_notifier_block containing all of the requisite data.
 * Specifically, the con_ids member of that object is used to enable or disable
 * the device's clocks, depending on @action.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static int pm_runtime_clk_notify(struct notifier_block *nb,
=======
static int pm_clk_notify(struct notifier_block *nb,
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int pm_clk_notify(struct notifier_block *nb,
>>>>>>> refs/remotes/origin/master
				 unsigned long action, void *data)
{
	struct pm_clk_notifier_block *clknb;
	struct device *dev = data;
	char **con_id;

	dev_dbg(dev, "%s() %ld\n", __func__, action);

	clknb = container_of(nb, struct pm_clk_notifier_block, nb);

	switch (action) {
	case BUS_NOTIFY_BIND_DRIVER:
		if (clknb->con_ids[0]) {
			for (con_id = clknb->con_ids; *con_id; con_id++)
				enable_clock(dev, *con_id);
		} else {
			enable_clock(dev, NULL);
		}
		break;
	case BUS_NOTIFY_UNBOUND_DRIVER:
		if (clknb->con_ids[0]) {
			for (con_id = clknb->con_ids; *con_id; con_id++)
				disable_clock(dev, *con_id);
		} else {
			disable_clock(dev, NULL);
		}
		break;
	}

	return 0;
}

#endif /* !CONFIG_PM_RUNTIME */

/**
<<<<<<< HEAD
<<<<<<< HEAD
 * pm_runtime_clk_add_notifier - Add bus type notifier for runtime PM clocks.
=======
 * pm_clk_add_notifier - Add bus type notifier for power management clocks.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * pm_clk_add_notifier - Add bus type notifier for power management clocks.
>>>>>>> refs/remotes/origin/master
 * @bus: Bus type to add the notifier to.
 * @clknb: Notifier to be added to the given bus type.
 *
 * The nb member of @clknb is not expected to be initialized and its
<<<<<<< HEAD
<<<<<<< HEAD
 * notifier_call member will be replaced with pm_runtime_clk_notify().  However,
 * the remaining members of @clknb should be populated prior to calling this
 * routine.
 */
void pm_runtime_clk_add_notifier(struct bus_type *bus,
=======
=======
>>>>>>> refs/remotes/origin/master
 * notifier_call member will be replaced with pm_clk_notify().  However,
 * the remaining members of @clknb should be populated prior to calling this
 * routine.
 */
void pm_clk_add_notifier(struct bus_type *bus,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				 struct pm_clk_notifier_block *clknb)
{
	if (!bus || !clknb)
		return;

<<<<<<< HEAD
<<<<<<< HEAD
	clknb->nb.notifier_call = pm_runtime_clk_notify;
=======
	clknb->nb.notifier_call = pm_clk_notify;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	clknb->nb.notifier_call = pm_clk_notify;
>>>>>>> refs/remotes/origin/master
	bus_register_notifier(bus, &clknb->nb);
}
