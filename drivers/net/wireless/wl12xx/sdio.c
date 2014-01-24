/*
 * This file is part of wl1271
 *
 * Copyright (C) 2009-2010 Nokia Corporation
 *
 * Contact: Luciano Coelho <luciano.coelho@nokia.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

#include <linux/irq.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <linux/crc7.h>
#include <linux/vmalloc.h>
=======
#include <linux/vmalloc.h>
#include <linux/platform_device.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/mmc/sdio_func.h>
#include <linux/mmc/sdio_ids.h>
#include <linux/mmc/card.h>
#include <linux/mmc/host.h>
#include <linux/gpio.h>
#include <linux/wl12xx.h>
#include <linux/pm_runtime.h>

#include "wl12xx.h"
#include "wl12xx_80211.h"
#include "io.h"

#ifndef SDIO_VENDOR_ID_TI
#define SDIO_VENDOR_ID_TI		0x0097
#endif

#ifndef SDIO_DEVICE_ID_TI_WL1271
#define SDIO_DEVICE_ID_TI_WL1271	0x4076
#endif

<<<<<<< HEAD
static const struct sdio_device_id wl1271_devices[] = {
=======
struct wl12xx_sdio_glue {
	struct device *dev;
	struct platform_device *core;
};

static const struct sdio_device_id wl1271_devices[] __devinitconst = {
>>>>>>> refs/remotes/origin/cm-10.0
	{ SDIO_DEVICE(SDIO_VENDOR_ID_TI, SDIO_DEVICE_ID_TI_WL1271) },
	{}
};
MODULE_DEVICE_TABLE(sdio, wl1271_devices);

<<<<<<< HEAD
static void wl1271_sdio_set_block_size(struct wl1271 *wl, unsigned int blksz)
{
	sdio_claim_host(wl->if_priv);
	sdio_set_block_size(wl->if_priv, blksz);
	sdio_release_host(wl->if_priv);
}

static inline struct sdio_func *wl_to_func(struct wl1271 *wl)
{
	return wl->if_priv;
}

static struct device *wl1271_sdio_wl_to_dev(struct wl1271 *wl)
{
	return &(wl_to_func(wl)->dev);
}

static irqreturn_t wl1271_hardirq(int irq, void *cookie)
{
	struct wl1271 *wl = cookie;
	unsigned long flags;

	wl1271_debug(DEBUG_IRQ, "IRQ");

	/* complete the ELP completion */
	spin_lock_irqsave(&wl->wl_lock, flags);
	set_bit(WL1271_FLAG_IRQ_RUNNING, &wl->flags);
	if (wl->elp_compl) {
		complete(wl->elp_compl);
		wl->elp_compl = NULL;
	}

	if (test_bit(WL1271_FLAG_SUSPENDED, &wl->flags)) {
		/* don't enqueue a work right now. mark it as pending */
		set_bit(WL1271_FLAG_PENDING_WORK, &wl->flags);
		wl1271_debug(DEBUG_IRQ, "should not enqueue work");
		disable_irq_nosync(wl->irq);
		pm_wakeup_event(wl1271_sdio_wl_to_dev(wl), 0);
		spin_unlock_irqrestore(&wl->wl_lock, flags);
		return IRQ_HANDLED;
	}
	spin_unlock_irqrestore(&wl->wl_lock, flags);

	return IRQ_WAKE_THREAD;
}

static void wl1271_sdio_disable_interrupts(struct wl1271 *wl)
{
	disable_irq(wl->irq);
}

static void wl1271_sdio_enable_interrupts(struct wl1271 *wl)
{
	enable_irq(wl->irq);
}

static void wl1271_sdio_reset(struct wl1271 *wl)
{
}

static void wl1271_sdio_init(struct wl1271 *wl)
{
}

static void wl1271_sdio_raw_read(struct wl1271 *wl, int addr, void *buf,
				 size_t len, bool fixed)
{
	int ret;
	struct sdio_func *func = wl_to_func(wl);

	if (unlikely(addr == HW_ACCESS_ELP_CTRL_REG_ADDR)) {
		((u8 *)buf)[0] = sdio_f0_readb(func, addr, &ret);
		wl1271_debug(DEBUG_SDIO, "sdio read 52 addr 0x%x, byte 0x%02x",
			     addr, ((u8 *)buf)[0]);
=======
static void wl1271_sdio_set_block_size(struct device *child,
				       unsigned int blksz)
{
	struct wl12xx_sdio_glue *glue = dev_get_drvdata(child->parent);
	struct sdio_func *func = dev_to_sdio_func(glue->dev);

	sdio_claim_host(func);
	sdio_set_block_size(func, blksz);
	sdio_release_host(func);
}

static void wl12xx_sdio_raw_read(struct device *child, int addr, void *buf,
				 size_t len, bool fixed)
{
	int ret;
	struct wl12xx_sdio_glue *glue = dev_get_drvdata(child->parent);
	struct sdio_func *func = dev_to_sdio_func(glue->dev);

	sdio_claim_host(func);

	if (unlikely(addr == HW_ACCESS_ELP_CTRL_REG_ADDR)) {
		((u8 *)buf)[0] = sdio_f0_readb(func, addr, &ret);
		dev_dbg(child->parent, "sdio read 52 addr 0x%x, byte 0x%02x\n",
			addr, ((u8 *)buf)[0]);
>>>>>>> refs/remotes/origin/cm-10.0
	} else {
		if (fixed)
			ret = sdio_readsb(func, buf, addr, len);
		else
			ret = sdio_memcpy_fromio(func, buf, addr, len);

<<<<<<< HEAD
		wl1271_debug(DEBUG_SDIO, "sdio read 53 addr 0x%x, %zu bytes",
			     addr, len);
		wl1271_dump_ascii(DEBUG_SDIO, "data: ", buf, len);
	}

	if (ret)
		wl1271_error("sdio read failed (%d)", ret);
}

static void wl1271_sdio_raw_write(struct wl1271 *wl, int addr, void *buf,
				  size_t len, bool fixed)
{
	int ret;
	struct sdio_func *func = wl_to_func(wl);

	if (unlikely(addr == HW_ACCESS_ELP_CTRL_REG_ADDR)) {
		sdio_f0_writeb(func, ((u8 *)buf)[0], addr, &ret);
		wl1271_debug(DEBUG_SDIO, "sdio write 52 addr 0x%x, byte 0x%02x",
			     addr, ((u8 *)buf)[0]);
	} else {
		wl1271_debug(DEBUG_SDIO, "sdio write 53 addr 0x%x, %zu bytes",
			     addr, len);
		wl1271_dump_ascii(DEBUG_SDIO, "data: ", buf, len);
=======
		dev_dbg(child->parent, "sdio read 53 addr 0x%x, %zu bytes\n",
			addr, len);
	}

	sdio_release_host(func);

	if (ret)
		dev_err(child->parent, "sdio read failed (%d)\n", ret);
}

static void wl12xx_sdio_raw_write(struct device *child, int addr, void *buf,
				  size_t len, bool fixed)
{
	int ret;
	struct wl12xx_sdio_glue *glue = dev_get_drvdata(child->parent);
	struct sdio_func *func = dev_to_sdio_func(glue->dev);

	sdio_claim_host(func);

	if (unlikely(addr == HW_ACCESS_ELP_CTRL_REG_ADDR)) {
		sdio_f0_writeb(func, ((u8 *)buf)[0], addr, &ret);
		dev_dbg(child->parent, "sdio write 52 addr 0x%x, byte 0x%02x\n",
			addr, ((u8 *)buf)[0]);
	} else {
		dev_dbg(child->parent, "sdio write 53 addr 0x%x, %zu bytes\n",
			addr, len);
>>>>>>> refs/remotes/origin/cm-10.0

		if (fixed)
			ret = sdio_writesb(func, addr, buf, len);
		else
			ret = sdio_memcpy_toio(func, addr, buf, len);
	}

<<<<<<< HEAD
	if (ret)
		wl1271_error("sdio write failed (%d)", ret);
}

static int wl1271_sdio_power_on(struct wl1271 *wl)
{
	struct sdio_func *func = wl_to_func(wl);
	int ret;

	/* Make sure the card will not be powered off by runtime PM */
	ret = pm_runtime_get_sync(&func->dev);
	if (ret < 0)
		goto out;

	/* Runtime PM might be disabled, so power up the card manually */
	ret = mmc_power_restore_host(func->card->host);
	if (ret < 0)
		goto out;

	sdio_claim_host(func);
	sdio_enable_func(func);
=======
	sdio_release_host(func);

	if (ret)
		dev_err(child->parent, "sdio write failed (%d)\n", ret);
}

static int wl12xx_sdio_power_on(struct wl12xx_sdio_glue *glue)
{
	int ret;
	struct sdio_func *func = dev_to_sdio_func(glue->dev);

	/* If enabled, tell runtime PM not to power off the card */
	if (pm_runtime_enabled(&func->dev)) {
		ret = pm_runtime_get_sync(&func->dev);
		if (ret < 0)
			goto out;
	} else {
		/* Runtime PM is disabled: power up the card manually */
		ret = mmc_power_restore_host(func->card->host);
		if (ret < 0)
			goto out;
	}

	sdio_claim_host(func);
	sdio_enable_func(func);
	sdio_release_host(func);
>>>>>>> refs/remotes/origin/cm-10.0

out:
	return ret;
}

<<<<<<< HEAD
static int wl1271_sdio_power_off(struct wl1271 *wl)
{
	struct sdio_func *func = wl_to_func(wl);
	int ret;

	sdio_disable_func(func);
	sdio_release_host(func);

	/* Runtime PM might be disabled, so power off the card manually */
=======
static int wl12xx_sdio_power_off(struct wl12xx_sdio_glue *glue)
{
	int ret;
	struct sdio_func *func = dev_to_sdio_func(glue->dev);

	sdio_claim_host(func);
	sdio_disable_func(func);
	sdio_release_host(func);

	/* Power off the card manually, even if runtime PM is enabled. */
>>>>>>> refs/remotes/origin/cm-10.0
	ret = mmc_power_save_host(func->card->host);
	if (ret < 0)
		return ret;

<<<<<<< HEAD
	/* Let runtime PM know the card is powered off */
	return pm_runtime_put_sync(&func->dev);
}

static int wl1271_sdio_set_power(struct wl1271 *wl, bool enable)
{
	if (enable)
		return wl1271_sdio_power_on(wl);
	else
		return wl1271_sdio_power_off(wl);
}

static struct wl1271_if_operations sdio_ops = {
	.read		= wl1271_sdio_raw_read,
	.write		= wl1271_sdio_raw_write,
	.reset		= wl1271_sdio_reset,
	.init		= wl1271_sdio_init,
	.power		= wl1271_sdio_set_power,
	.dev		= wl1271_sdio_wl_to_dev,
	.enable_irq	= wl1271_sdio_enable_interrupts,
	.disable_irq	= wl1271_sdio_disable_interrupts,
=======
	/* If enabled, let runtime PM know the card is powered off */
	if (pm_runtime_enabled(&func->dev))
		ret = pm_runtime_put_sync(&func->dev);

	return ret;
}

static int wl12xx_sdio_set_power(struct device *child, bool enable)
{
	struct wl12xx_sdio_glue *glue = dev_get_drvdata(child->parent);

	if (enable)
		return wl12xx_sdio_power_on(glue);
	else
		return wl12xx_sdio_power_off(glue);
}

static struct wl1271_if_operations sdio_ops = {
	.read		= wl12xx_sdio_raw_read,
	.write		= wl12xx_sdio_raw_write,
	.power		= wl12xx_sdio_set_power,
>>>>>>> refs/remotes/origin/cm-10.0
	.set_block_size = wl1271_sdio_set_block_size,
};

static int __devinit wl1271_probe(struct sdio_func *func,
				  const struct sdio_device_id *id)
{
<<<<<<< HEAD
	struct ieee80211_hw *hw;
	const struct wl12xx_platform_data *wlan_data;
	struct wl1271 *wl;
	unsigned long irqflags;
	mmc_pm_flag_t mmcflags;
	int ret;
=======
	struct wl12xx_platform_data *wlan_data;
	struct wl12xx_sdio_glue *glue;
	struct resource res[1];
	mmc_pm_flag_t mmcflags;
	int ret = -ENOMEM;
>>>>>>> refs/remotes/origin/cm-10.0

	/* We are only able to handle the wlan function */
	if (func->num != 0x02)
		return -ENODEV;

<<<<<<< HEAD
	hw = wl1271_alloc_hw();
	if (IS_ERR(hw))
		return PTR_ERR(hw);

	wl = hw->priv;

	wl->if_priv = func;
	wl->if_ops = &sdio_ops;
=======
	glue = kzalloc(sizeof(*glue), GFP_KERNEL);
	if (!glue) {
		dev_err(&func->dev, "can't allocate glue\n");
		goto out;
	}

	glue->dev = &func->dev;
>>>>>>> refs/remotes/origin/cm-10.0

	/* Grab access to FN0 for ELP reg. */
	func->card->quirks |= MMC_QUIRK_LENIENT_FN0;

	/* Use block mode for transferring over one block size of data */
	func->card->quirks |= MMC_QUIRK_BLKSZ_FOR_BYTE_MODE;

	wlan_data = wl12xx_get_platform_data();
	if (IS_ERR(wlan_data)) {
		ret = PTR_ERR(wlan_data);
<<<<<<< HEAD
		wl1271_error("missing wlan platform data: %d", ret);
		goto out_free;
	}

	wl->irq = wlan_data->irq;
	wl->ref_clock = wlan_data->board_ref_clock;
	wl->tcxo_clock = wlan_data->board_tcxo_clock;
	wl->platform_quirks = wlan_data->platform_quirks;

	if (wl->platform_quirks & WL12XX_PLATFORM_QUIRK_EDGE_IRQ)
		irqflags = IRQF_TRIGGER_RISING;
	else
		irqflags = IRQF_TRIGGER_HIGH | IRQF_ONESHOT;

	ret = request_threaded_irq(wl->irq, wl1271_hardirq, wl1271_irq,
				   irqflags,
				   DRIVER_NAME, wl);
	if (ret < 0) {
		wl1271_error("request_irq() failed: %d", ret);
		goto out_free;
	}

	enable_irq_wake(wl->irq);
	device_init_wakeup(wl1271_sdio_wl_to_dev(wl), 1);

	disable_irq(wl->irq);

	/* if sdio can keep power while host is suspended, enable wow */
	mmcflags = sdio_get_host_pm_caps(func);
	wl1271_debug(DEBUG_SDIO, "sdio PM caps = 0x%x", mmcflags);

	if (mmcflags & MMC_PM_KEEP_POWER)
		hw->wiphy->wowlan.flags = WIPHY_WOWLAN_ANY;

	ret = wl1271_init_ieee80211(wl);
	if (ret)
		goto out_irq;

	ret = wl1271_register_hw(wl);
	if (ret)
		goto out_irq;

	sdio_set_drvdata(func, wl);
=======
		dev_err(glue->dev, "missing wlan platform data: %d\n", ret);
		goto out_free_glue;
	}

	/* if sdio can keep power while host is suspended, enable wow */
	mmcflags = sdio_get_host_pm_caps(func);
	dev_dbg(glue->dev, "sdio PM caps = 0x%x\n", mmcflags);

	if (mmcflags & MMC_PM_KEEP_POWER)
		wlan_data->pwr_in_suspend = true;

	wlan_data->ops = &sdio_ops;

	sdio_set_drvdata(func, glue);
>>>>>>> refs/remotes/origin/cm-10.0

	/* Tell PM core that we don't need the card to be powered now */
	pm_runtime_put_noidle(&func->dev);

<<<<<<< HEAD
	wl1271_notice("initialized");

	return 0;

 out_irq:
	free_irq(wl->irq, wl);

 out_free:
	wl1271_free_hw(wl);

=======
	glue->core = platform_device_alloc("wl12xx", -1);
	if (!glue->core) {
		dev_err(glue->dev, "can't allocate platform_device");
		ret = -ENOMEM;
		goto out_free_glue;
	}

	glue->core->dev.parent = &func->dev;

	memset(res, 0x00, sizeof(res));

	res[0].start = wlan_data->irq;
	res[0].flags = IORESOURCE_IRQ;
	res[0].name = "irq";

	ret = platform_device_add_resources(glue->core, res, ARRAY_SIZE(res));
	if (ret) {
		dev_err(glue->dev, "can't add resources\n");
		goto out_dev_put;
	}

	ret = platform_device_add_data(glue->core, wlan_data,
				       sizeof(*wlan_data));
	if (ret) {
		dev_err(glue->dev, "can't add platform data\n");
		goto out_dev_put;
	}

	ret = platform_device_add(glue->core);
	if (ret) {
		dev_err(glue->dev, "can't add platform device\n");
		goto out_dev_put;
	}
	return 0;

out_dev_put:
	platform_device_put(glue->core);

out_free_glue:
	kfree(glue);

out:
>>>>>>> refs/remotes/origin/cm-10.0
	return ret;
}

static void __devexit wl1271_remove(struct sdio_func *func)
{
<<<<<<< HEAD
	struct wl1271 *wl = sdio_get_drvdata(func);
=======
	struct wl12xx_sdio_glue *glue = sdio_get_drvdata(func);
>>>>>>> refs/remotes/origin/cm-10.0

	/* Undo decrement done above in wl1271_probe */
	pm_runtime_get_noresume(&func->dev);

<<<<<<< HEAD
	wl1271_unregister_hw(wl);
	device_init_wakeup(wl1271_sdio_wl_to_dev(wl), 0);
	disable_irq_wake(wl->irq);
	free_irq(wl->irq, wl);
	wl1271_free_hw(wl);
=======
	platform_device_del(glue->core);
	platform_device_put(glue->core);
	kfree(glue);
>>>>>>> refs/remotes/origin/cm-10.0
}

#ifdef CONFIG_PM
static int wl1271_suspend(struct device *dev)
{
	/* Tell MMC/SDIO core it's OK to power down the card
	 * (if it isn't already), but not to remove it completely */
	struct sdio_func *func = dev_to_sdio_func(dev);
<<<<<<< HEAD
	struct wl1271 *wl = sdio_get_drvdata(func);
	mmc_pm_flag_t sdio_flags;
	int ret = 0;

	wl1271_debug(DEBUG_MAC80211, "wl1271 suspend. wow_enabled: %d",
		     wl->wow_enabled);
=======
	struct wl12xx_sdio_glue *glue = sdio_get_drvdata(func);
	struct wl1271 *wl = platform_get_drvdata(glue->core);
	mmc_pm_flag_t sdio_flags;
	int ret = 0;

	dev_dbg(dev, "wl1271 suspend. wow_enabled: %d\n",
		wl->wow_enabled);
>>>>>>> refs/remotes/origin/cm-10.0

	/* check whether sdio should keep power */
	if (wl->wow_enabled) {
		sdio_flags = sdio_get_host_pm_caps(func);

		if (!(sdio_flags & MMC_PM_KEEP_POWER)) {
<<<<<<< HEAD
			wl1271_error("can't keep power while host "
				     "is suspended");
=======
			dev_err(dev, "can't keep power while host "
				     "is suspended\n");
>>>>>>> refs/remotes/origin/cm-10.0
			ret = -EINVAL;
			goto out;
		}

		/* keep power while host suspended */
		ret = sdio_set_host_pm_flags(func, MMC_PM_KEEP_POWER);
		if (ret) {
<<<<<<< HEAD
			wl1271_error("error while trying to keep power");
			goto out;
		}

		/* release host */
		sdio_release_host(func);
=======
			dev_err(dev, "error while trying to keep power\n");
			goto out;
		}
>>>>>>> refs/remotes/origin/cm-10.0
	}
out:
	return ret;
}

static int wl1271_resume(struct device *dev)
{
<<<<<<< HEAD
	struct sdio_func *func = dev_to_sdio_func(dev);
	struct wl1271 *wl = sdio_get_drvdata(func);

	wl1271_debug(DEBUG_MAC80211, "wl1271 resume");
	if (wl->wow_enabled) {
		/* claim back host */
		sdio_claim_host(func);
	}
=======
	dev_dbg(dev, "wl1271 resume\n");
>>>>>>> refs/remotes/origin/cm-10.0

	return 0;
}

static const struct dev_pm_ops wl1271_sdio_pm_ops = {
	.suspend	= wl1271_suspend,
	.resume		= wl1271_resume,
};
#endif

static struct sdio_driver wl1271_sdio_driver = {
	.name		= "wl1271_sdio",
	.id_table	= wl1271_devices,
	.probe		= wl1271_probe,
	.remove		= __devexit_p(wl1271_remove),
#ifdef CONFIG_PM
	.drv = {
		.pm = &wl1271_sdio_pm_ops,
	},
#endif
};

static int __init wl1271_init(void)
{
<<<<<<< HEAD
	int ret;

	ret = sdio_register_driver(&wl1271_sdio_driver);
	if (ret < 0) {
		wl1271_error("failed to register sdio driver: %d", ret);
		goto out;
	}

out:
	return ret;
=======
	return sdio_register_driver(&wl1271_sdio_driver);
>>>>>>> refs/remotes/origin/cm-10.0
}

static void __exit wl1271_exit(void)
{
	sdio_unregister_driver(&wl1271_sdio_driver);
<<<<<<< HEAD

	wl1271_notice("unloaded");
=======
>>>>>>> refs/remotes/origin/cm-10.0
}

module_init(wl1271_init);
module_exit(wl1271_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Luciano Coelho <coelho@ti.com>");
MODULE_AUTHOR("Juuso Oikarinen <juuso.oikarinen@nokia.com>");
<<<<<<< HEAD
MODULE_FIRMWARE(WL1271_FW_NAME);
MODULE_FIRMWARE(WL128X_FW_NAME);
MODULE_FIRMWARE(WL127X_AP_FW_NAME);
MODULE_FIRMWARE(WL128X_AP_FW_NAME);
=======
MODULE_FIRMWARE(WL127X_FW_NAME_SINGLE);
MODULE_FIRMWARE(WL127X_FW_NAME_MULTI);
MODULE_FIRMWARE(WL127X_PLT_FW_NAME);
MODULE_FIRMWARE(WL128X_FW_NAME_SINGLE);
MODULE_FIRMWARE(WL128X_FW_NAME_MULTI);
MODULE_FIRMWARE(WL128X_PLT_FW_NAME);
>>>>>>> refs/remotes/origin/cm-10.0
