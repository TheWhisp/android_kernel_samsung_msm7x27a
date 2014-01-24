/*
 *  linux/drivers/mmc/core/bus.c
 *
 *  Copyright (C) 2003 Russell King, All Rights Reserved.
 *  Copyright (C) 2007 Pierre Ossman
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  MMC card bus driver model
 */

<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/device.h>
#include <linux/err.h>
#include <linux/slab.h>
=======
=======
>>>>>>> refs/remotes/origin/master
#include <linux/export.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/stat.h>
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#include <linux/pm_runtime.h>

#include <linux/mmc/card.h>
#include <linux/mmc/host.h>

#include "core.h"
#include "sdio_cis.h"
#include "bus.h"

#define to_mmc_driver(d)	container_of(d, struct mmc_driver, drv)

<<<<<<< HEAD
static ssize_t mmc_type_show(struct device *dev,
=======
static ssize_t type_show(struct device *dev,
>>>>>>> refs/remotes/origin/master
	struct device_attribute *attr, char *buf)
{
	struct mmc_card *card = mmc_dev_to_card(dev);

	switch (card->type) {
	case MMC_TYPE_MMC:
		return sprintf(buf, "MMC\n");
	case MMC_TYPE_SD:
		return sprintf(buf, "SD\n");
	case MMC_TYPE_SDIO:
		return sprintf(buf, "SDIO\n");
	case MMC_TYPE_SD_COMBO:
		return sprintf(buf, "SDcombo\n");
	default:
		return -EFAULT;
	}
}
<<<<<<< HEAD

static struct device_attribute mmc_dev_attrs[] = {
	__ATTR(type, S_IRUGO, mmc_type_show, NULL),
	__ATTR_NULL,
};
=======
static DEVICE_ATTR_RO(type);

static struct attribute *mmc_dev_attrs[] = {
	&dev_attr_type.attr,
	NULL,
};
ATTRIBUTE_GROUPS(mmc_dev);
>>>>>>> refs/remotes/origin/master

/*
 * This currently matches any MMC driver to any MMC card - drivers
 * themselves make the decision whether to drive this card in their
 * probe method.
 */
static int mmc_bus_match(struct device *dev, struct device_driver *drv)
{
	return 1;
}

static int
mmc_bus_uevent(struct device *dev, struct kobj_uevent_env *env)
{
	struct mmc_card *card = mmc_dev_to_card(dev);
	const char *type;
	int retval = 0;

	switch (card->type) {
	case MMC_TYPE_MMC:
		type = "MMC";
		break;
	case MMC_TYPE_SD:
		type = "SD";
		break;
	case MMC_TYPE_SDIO:
		type = "SDIO";
		break;
	case MMC_TYPE_SD_COMBO:
		type = "SDcombo";
		break;
	default:
		type = NULL;
	}

	if (type) {
		retval = add_uevent_var(env, "MMC_TYPE=%s", type);
		if (retval)
			return retval;
	}

	retval = add_uevent_var(env, "MMC_NAME=%s", mmc_card_name(card));
	if (retval)
		return retval;

	/*
	 * Request the mmc_block device.  Note: that this is a direct request
	 * for the module it carries no information as to what is inserted.
	 */
	retval = add_uevent_var(env, "MODALIAS=mmc:block");

	return retval;
}

static int mmc_bus_probe(struct device *dev)
{
	struct mmc_driver *drv = to_mmc_driver(dev->driver);
	struct mmc_card *card = mmc_dev_to_card(dev);

	return drv->probe(card);
}

static int mmc_bus_remove(struct device *dev)
{
	struct mmc_driver *drv = to_mmc_driver(dev->driver);
	struct mmc_card *card = mmc_dev_to_card(dev);

	drv->remove(card);

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
static int mmc_bus_pm_suspend(struct device *dev)
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
#ifdef CONFIG_PM_SLEEP
static int mmc_bus_suspend(struct device *dev)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct mmc_driver *drv = to_mmc_driver(dev->driver);
	struct mmc_card *card = mmc_dev_to_card(dev);
	int ret = 0;

	if (dev->driver && drv->suspend)
		ret = drv->suspend(card);
	return ret;
}

<<<<<<< HEAD
static int mmc_bus_pm_resume(struct device *dev)
=======
static int mmc_bus_resume(struct device *dev)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct mmc_driver *drv = to_mmc_driver(dev->driver);
	struct mmc_card *card = mmc_dev_to_card(dev);
	int ret = 0;

	if (dev->driver && drv->resume)
		ret = drv->resume(card);
	return ret;
}
<<<<<<< HEAD
<<<<<<< HEAD

#ifdef CONFIG_PM_RUNTIME
=======
#else
#define mmc_bus_suspend NULL
#define mmc_bus_resume NULL
=======
static void mmc_bus_shutdown(struct device *dev)
{
	struct mmc_driver *drv = to_mmc_driver(dev->driver);
	struct mmc_card *card = mmc_dev_to_card(dev);
	struct mmc_host *host = card->host;
	int ret;

	if (dev->driver && drv->shutdown)
		drv->shutdown(card);

	if (host->bus_ops->shutdown) {
		ret = host->bus_ops->shutdown(host);
		if (ret)
			pr_warn("%s: error %d during shutdown\n",
				mmc_hostname(host), ret);
	}
}

#ifdef CONFIG_PM_SLEEP
static int mmc_bus_suspend(struct device *dev)
{
	struct mmc_driver *drv = to_mmc_driver(dev->driver);
	struct mmc_card *card = mmc_dev_to_card(dev);
	struct mmc_host *host = card->host;
	int ret;

	if (dev->driver && drv->suspend) {
		ret = drv->suspend(card);
		if (ret)
			return ret;
	}

	ret = host->bus_ops->suspend(host);
	return ret;
}

static int mmc_bus_resume(struct device *dev)
{
	struct mmc_driver *drv = to_mmc_driver(dev->driver);
	struct mmc_card *card = mmc_dev_to_card(dev);
	struct mmc_host *host = card->host;
	int ret;

	ret = host->bus_ops->resume(host);
	if (ret)
		pr_warn("%s: error %d during resume (card was removed?)\n",
			mmc_hostname(host), ret);

	if (dev->driver && drv->resume)
		ret = drv->resume(card);

	return ret;
}
>>>>>>> refs/remotes/origin/master
=======
#else
#define mmc_bus_suspend NULL
#define mmc_bus_resume NULL
>>>>>>> refs/remotes/origin/cm-11.0
#endif

#ifdef CONFIG_PM_RUNTIME

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
static int mmc_runtime_suspend(struct device *dev)
{
	struct mmc_card *card = mmc_dev_to_card(dev);

	return mmc_power_save_host(card->host);
=======
static int mmc_runtime_suspend(struct device *dev)
{
	struct mmc_card *card = mmc_dev_to_card(dev);
	struct mmc_host *host = card->host;
	int ret = 0;

	if (host->bus_ops->runtime_suspend)
		ret = host->bus_ops->runtime_suspend(host);

	return ret;
>>>>>>> refs/remotes/origin/master
}

static int mmc_runtime_resume(struct device *dev)
{
	struct mmc_card *card = mmc_dev_to_card(dev);
<<<<<<< HEAD

	return mmc_power_restore_host(card->host);
=======
	struct mmc_host *host = card->host;
	int ret = 0;

	if (host->bus_ops->runtime_resume)
		ret = host->bus_ops->runtime_resume(host);

	return ret;
>>>>>>> refs/remotes/origin/master
}

static int mmc_runtime_idle(struct device *dev)
{
<<<<<<< HEAD
	return pm_runtime_suspend(dev);
}
<<<<<<< HEAD
#else /* !CONFIG_PM_RUNTIME */
#define mmc_runtime_suspend	NULL
#define mmc_runtime_resume	NULL
#define mmc_runtime_idle	NULL
#endif /* !CONFIG_PM_RUNTIME */

static const struct dev_pm_ops mmc_bus_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(mmc_bus_pm_suspend, mmc_bus_pm_resume)
	SET_RUNTIME_PM_OPS(mmc_runtime_suspend, mmc_runtime_resume, mmc_runtime_idle)
=======
=======
	return 0;
}
>>>>>>> refs/remotes/origin/master

#endif /* !CONFIG_PM_RUNTIME */

static const struct dev_pm_ops mmc_bus_pm_ops = {
	SET_RUNTIME_PM_OPS(mmc_runtime_suspend, mmc_runtime_resume,
			mmc_runtime_idle)
	SET_SYSTEM_SLEEP_PM_OPS(mmc_bus_suspend, mmc_bus_resume)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};

static struct bus_type mmc_bus_type = {
	.name		= "mmc",
<<<<<<< HEAD
	.dev_attrs	= mmc_dev_attrs,
=======
	.dev_groups	= mmc_dev_groups,
>>>>>>> refs/remotes/origin/master
	.match		= mmc_bus_match,
	.uevent		= mmc_bus_uevent,
	.probe		= mmc_bus_probe,
	.remove		= mmc_bus_remove,
<<<<<<< HEAD
=======
	.shutdown	= mmc_bus_shutdown,
>>>>>>> refs/remotes/origin/master
	.pm		= &mmc_bus_pm_ops,
};

int mmc_register_bus(void)
{
	return bus_register(&mmc_bus_type);
}

void mmc_unregister_bus(void)
{
	bus_unregister(&mmc_bus_type);
}

/**
 *	mmc_register_driver - register a media driver
 *	@drv: MMC media driver
 */
int mmc_register_driver(struct mmc_driver *drv)
{
	drv->drv.bus = &mmc_bus_type;
	return driver_register(&drv->drv);
}

EXPORT_SYMBOL(mmc_register_driver);

/**
 *	mmc_unregister_driver - unregister a media driver
 *	@drv: MMC media driver
 */
void mmc_unregister_driver(struct mmc_driver *drv)
{
	drv->drv.bus = &mmc_bus_type;
	driver_unregister(&drv->drv);
}

EXPORT_SYMBOL(mmc_unregister_driver);

static void mmc_release_card(struct device *dev)
{
	struct mmc_card *card = mmc_dev_to_card(dev);

	sdio_free_common_cis(card);

<<<<<<< HEAD
	if (card->info)
		kfree(card->info);
=======
	kfree(card->info);
>>>>>>> refs/remotes/origin/master

	kfree(card);
}

/*
 * Allocate and initialise a new MMC card structure.
 */
struct mmc_card *mmc_alloc_card(struct mmc_host *host, struct device_type *type)
{
	struct mmc_card *card;

	card = kzalloc(sizeof(struct mmc_card), GFP_KERNEL);
	if (!card)
		return ERR_PTR(-ENOMEM);

	card->host = host;

	device_initialize(&card->dev);

	card->dev.parent = mmc_classdev(host);
	card->dev.bus = &mmc_bus_type;
	card->dev.release = mmc_release_card;
	card->dev.type = type;

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
	spin_lock_init(&card->wr_pack_stats.lock);

>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
	spin_lock_init(&card->wr_pack_stats.lock);

>>>>>>> refs/remotes/origin/cm-11.0
	return card;
}

/*
 * Register a new MMC card with the driver model.
 */
int mmc_add_card(struct mmc_card *card)
{
	int ret;
	const char *type;
	const char *uhs_bus_speed_mode = "";
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	static const char *const uhs_speeds[] = {
		[UHS_SDR12_BUS_SPEED] = "SDR12 ",
		[UHS_SDR25_BUS_SPEED] = "SDR25 ",
		[UHS_SDR50_BUS_SPEED] = "SDR50 ",
		[UHS_SDR104_BUS_SPEED] = "SDR104 ",
		[UHS_DDR50_BUS_SPEED] = "DDR50 ",
	};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	dev_set_name(&card->dev, "%s:%04x", mmc_hostname(card->host), card->rca);

	switch (card->type) {
	case MMC_TYPE_MMC:
		type = "MMC";
		break;
	case MMC_TYPE_SD:
		type = "SD";
		if (mmc_card_blockaddr(card)) {
			if (mmc_card_ext_capacity(card))
				type = "SDXC";
			else
				type = "SDHC";
		}
		break;
	case MMC_TYPE_SDIO:
		type = "SDIO";
		break;
	case MMC_TYPE_SD_COMBO:
		type = "SD-combo";
		if (mmc_card_blockaddr(card))
			type = "SDHC-combo";
		break;
	default:
		type = "?";
		break;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	if (mmc_sd_card_uhs(card)) {
		switch (card->sd_bus_speed) {
		case UHS_SDR104_BUS_SPEED:
			uhs_bus_speed_mode = "SDR104 ";
			break;
		case UHS_SDR50_BUS_SPEED:
			uhs_bus_speed_mode = "SDR50 ";
			break;
		case UHS_DDR50_BUS_SPEED:
			uhs_bus_speed_mode = "DDR50 ";
			break;
		case UHS_SDR25_BUS_SPEED:
			uhs_bus_speed_mode = "SDR25 ";
			break;
		case UHS_SDR12_BUS_SPEED:
			uhs_bus_speed_mode = "SDR12 ";
			break;
		default:
			uhs_bus_speed_mode = "";
			break;
		}
	}
	if (mmc_host_is_spi(card->host)) {
		printk(KERN_INFO "%s: new %s%s%s card on SPI\n",
=======
	if (mmc_sd_card_uhs(card) &&
=======
	if (mmc_card_uhs(card) &&
>>>>>>> refs/remotes/origin/master
		(card->sd_bus_speed < ARRAY_SIZE(uhs_speeds)))
		uhs_bus_speed_mode = uhs_speeds[card->sd_bus_speed];

	if (mmc_host_is_spi(card->host)) {
		pr_info("%s: new %s%s%s card on SPI\n",
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			mmc_hostname(card->host),
			mmc_card_highspeed(card) ? "high speed " : "",
			mmc_card_ddr_mode(card) ? "DDR " : "",
			type);
	} else {
<<<<<<< HEAD
<<<<<<< HEAD
		pr_info("%s: new %s%s%s%s card at address %04x\n",
			mmc_hostname(card->host),
			mmc_sd_card_uhs(card) ? "ultra high speed " :
			(mmc_card_highspeed(card) ? "high speed " : ""),
			mmc_card_ddr_mode(card) ? "DDR " : "",
			uhs_bus_speed_mode,
			type, card->rca);
=======
=======
>>>>>>> refs/remotes/origin/master
		pr_info("%s: new %s%s%s%s%s card at address %04x\n",
			mmc_hostname(card->host),
			mmc_card_uhs(card) ? "ultra high speed " :
			(mmc_card_highspeed(card) ? "high speed " : ""),
			(mmc_card_hs200(card) ? "HS200 " : ""),
			mmc_card_ddr_mode(card) ? "DDR " : "",
			uhs_bus_speed_mode, type, card->rca);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

#ifdef CONFIG_DEBUG_FS
	mmc_add_card_debugfs(card);
#endif
<<<<<<< HEAD
=======
	mmc_init_context_info(card->host);
>>>>>>> refs/remotes/origin/master

	ret = device_add(&card->dev);
	if (ret)
		return ret;

	mmc_card_set_present(card);

	return 0;
}

/*
 * Unregister a new MMC card with the driver model, and
 * (eventually) free it.
 */
void mmc_remove_card(struct mmc_card *card)
{
#ifdef CONFIG_DEBUG_FS
	mmc_remove_card_debugfs(card);
#endif

	if (mmc_card_present(card)) {
		if (mmc_host_is_spi(card->host)) {
<<<<<<< HEAD
<<<<<<< HEAD
			printk(KERN_INFO "%s: SPI card removed\n",
				mmc_hostname(card->host));
		} else {
			printk(KERN_INFO "%s: card %04x removed\n",
=======
=======
>>>>>>> refs/remotes/origin/master
			pr_info("%s: SPI card removed\n",
				mmc_hostname(card->host));
		} else {
			pr_info("%s: card %04x removed\n",
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				mmc_hostname(card->host), card->rca);
		}
		device_del(&card->dev);
	}

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
	kfree(card->wr_pack_stats.packing_events);

>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
	kfree(card->wr_pack_stats.packing_events);

>>>>>>> refs/remotes/origin/cm-11.0
	put_device(&card->dev);
}

