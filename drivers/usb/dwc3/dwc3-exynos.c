/**
 * dwc3-exynos.c - Samsung EXYNOS DWC3 Specific Glue layer
 *
 * Copyright (c) 2012 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com
 *
 * Author: Anton Tikhomirov <av.tikhomirov@samsung.com>
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
=======
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2  of
 * the License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
>>>>>>> refs/remotes/origin/master
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/platform_data/dwc3-exynos.h>
#include <linux/dma-mapping.h>
<<<<<<< HEAD
#include <linux/module.h>
#include <linux/clk.h>

#include "core.h"

struct dwc3_exynos {
	struct platform_device	*dwc3;
=======
#include <linux/clk.h>
#include <linux/usb/otg.h>
#include <linux/usb/usb_phy_gen_xceiv.h>
#include <linux/of.h>
#include <linux/of_platform.h>

struct dwc3_exynos {
	struct platform_device	*usb2_phy;
	struct platform_device	*usb3_phy;
>>>>>>> refs/remotes/origin/master
	struct device		*dev;

	struct clk		*clk;
};

<<<<<<< HEAD
static int __devinit dwc3_exynos_probe(struct platform_device *pdev)
{
	struct dwc3_exynos_data	*pdata = pdev->dev.platform_data;
	struct platform_device	*dwc3;
	struct dwc3_exynos	*exynos;
	struct clk		*clk;

	int			devid;
	int			ret = -ENOMEM;

	exynos = kzalloc(sizeof(*exynos), GFP_KERNEL);
	if (!exynos) {
		dev_err(&pdev->dev, "not enough memory\n");
		goto err0;
	}

	platform_set_drvdata(pdev, exynos);

	devid = dwc3_get_device_id();
	if (devid < 0)
		goto err1;

	dwc3 = platform_device_alloc("dwc3", devid);
	if (!dwc3) {
		dev_err(&pdev->dev, "couldn't allocate dwc3 device\n");
		goto err2;
	}

	clk = clk_get(&pdev->dev, "usbdrd30");
	if (IS_ERR(clk)) {
		dev_err(&pdev->dev, "couldn't get clock\n");
		ret = -EINVAL;
		goto err3;
	}

	dma_set_coherent_mask(&dwc3->dev, pdev->dev.coherent_dma_mask);

	dwc3->dev.parent = &pdev->dev;
	dwc3->dev.dma_mask = pdev->dev.dma_mask;
	dwc3->dev.dma_parms = pdev->dev.dma_parms;
	exynos->dwc3	= dwc3;
	exynos->dev	= &pdev->dev;
	exynos->clk	= clk;

	clk_enable(exynos->clk);

	/* PHY initialization */
	if (!pdata) {
		dev_dbg(&pdev->dev, "missing platform data\n");
	} else {
		if (pdata->phy_init)
			pdata->phy_init(pdev, pdata->phy_type);
	}

	ret = platform_device_add_resources(dwc3, pdev->resource,
			pdev->num_resources);
	if (ret) {
		dev_err(&pdev->dev, "couldn't add resources to dwc3 device\n");
		goto err4;
	}

	ret = platform_device_add(dwc3);
	if (ret) {
		dev_err(&pdev->dev, "failed to register dwc3 device\n");
		goto err4;
=======
static int dwc3_exynos_register_phys(struct dwc3_exynos *exynos)
{
	struct usb_phy_gen_xceiv_platform_data pdata;
	struct platform_device	*pdev;
	int			ret;

	memset(&pdata, 0x00, sizeof(pdata));

	pdev = platform_device_alloc("usb_phy_gen_xceiv", PLATFORM_DEVID_AUTO);
	if (!pdev)
		return -ENOMEM;

	exynos->usb2_phy = pdev;
	pdata.type = USB_PHY_TYPE_USB2;
	pdata.gpio_reset = -1;

	ret = platform_device_add_data(exynos->usb2_phy, &pdata, sizeof(pdata));
	if (ret)
		goto err1;

	pdev = platform_device_alloc("usb_phy_gen_xceiv", PLATFORM_DEVID_AUTO);
	if (!pdev) {
		ret = -ENOMEM;
		goto err1;
	}

	exynos->usb3_phy = pdev;
	pdata.type = USB_PHY_TYPE_USB3;

	ret = platform_device_add_data(exynos->usb3_phy, &pdata, sizeof(pdata));
	if (ret)
		goto err2;

	ret = platform_device_add(exynos->usb2_phy);
	if (ret)
		goto err2;

	ret = platform_device_add(exynos->usb3_phy);
	if (ret)
		goto err3;

	return 0;

err3:
	platform_device_del(exynos->usb2_phy);

err2:
	platform_device_put(exynos->usb3_phy);

err1:
	platform_device_put(exynos->usb2_phy);

	return ret;
}

static int dwc3_exynos_remove_child(struct device *dev, void *unused)
{
	struct platform_device *pdev = to_platform_device(dev);

	platform_device_unregister(pdev);

	return 0;
}

static int dwc3_exynos_probe(struct platform_device *pdev)
{
	struct dwc3_exynos	*exynos;
	struct clk		*clk;
	struct device		*dev = &pdev->dev;
	struct device_node	*node = dev->of_node;

	int			ret = -ENOMEM;

	exynos = devm_kzalloc(dev, sizeof(*exynos), GFP_KERNEL);
	if (!exynos) {
		dev_err(dev, "not enough memory\n");
		goto err1;
	}

	/*
	 * Right now device-tree probed devices don't get dma_mask set.
	 * Since shared usb code relies on it, set it here for now.
	 * Once we move to full device tree support this will vanish off.
	 */
	ret = dma_coerce_mask_and_coherent(dev, DMA_BIT_MASK(32));
	if (ret)
		goto err1;

	platform_set_drvdata(pdev, exynos);

	ret = dwc3_exynos_register_phys(exynos);
	if (ret) {
		dev_err(dev, "couldn't register PHYs\n");
		goto err1;
	}

	clk = devm_clk_get(dev, "usbdrd30");
	if (IS_ERR(clk)) {
		dev_err(dev, "couldn't get clock\n");
		ret = -EINVAL;
		goto err1;
	}

	exynos->dev	= dev;
	exynos->clk	= clk;

	clk_prepare_enable(exynos->clk);

	if (node) {
		ret = of_platform_populate(node, NULL, NULL, dev);
		if (ret) {
			dev_err(dev, "failed to add dwc3 core\n");
			goto err2;
		}
	} else {
		dev_err(dev, "no device node, failed to add dwc3 core\n");
		ret = -ENODEV;
		goto err2;
>>>>>>> refs/remotes/origin/master
	}

	return 0;

<<<<<<< HEAD
err4:
	if (pdata && pdata->phy_exit)
		pdata->phy_exit(pdev, pdata->phy_type);

	clk_disable(clk);
	clk_put(clk);
err3:
	platform_device_put(dwc3);
err2:
	dwc3_put_device_id(devid);
err1:
	kfree(exynos);
err0:
	return ret;
}

static int __devexit dwc3_exynos_remove(struct platform_device *pdev)
{
	struct dwc3_exynos	*exynos = platform_get_drvdata(pdev);
	struct dwc3_exynos_data *pdata = pdev->dev.platform_data;

	platform_device_unregister(exynos->dwc3);

	dwc3_put_device_id(exynos->dwc3->id);

	if (pdata && pdata->phy_exit)
		pdata->phy_exit(pdev, pdata->phy_type);

	clk_disable(exynos->clk);
	clk_put(exynos->clk);

	kfree(exynos);
=======
err2:
	clk_disable_unprepare(clk);
err1:
	return ret;
}

static int dwc3_exynos_remove(struct platform_device *pdev)
{
	struct dwc3_exynos	*exynos = platform_get_drvdata(pdev);

	device_for_each_child(&pdev->dev, NULL, dwc3_exynos_remove_child);
	platform_device_unregister(exynos->usb2_phy);
	platform_device_unregister(exynos->usb3_phy);

	clk_disable_unprepare(exynos->clk);

	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id exynos_dwc3_match[] = {
	{ .compatible = "samsung,exynos5250-dwusb3" },
	{},
};
MODULE_DEVICE_TABLE(of, exynos_dwc3_match);
#endif

#ifdef CONFIG_PM_SLEEP
static int dwc3_exynos_suspend(struct device *dev)
{
	struct dwc3_exynos *exynos = dev_get_drvdata(dev);

	clk_disable(exynos->clk);

	return 0;
}

static int dwc3_exynos_resume(struct device *dev)
{
	struct dwc3_exynos *exynos = dev_get_drvdata(dev);

	clk_enable(exynos->clk);

	/* runtime set active to reflect active state. */
	pm_runtime_disable(dev);
	pm_runtime_set_active(dev);
	pm_runtime_enable(dev);
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
static struct platform_driver dwc3_exynos_driver = {
	.probe		= dwc3_exynos_probe,
	.remove		= __devexit_p(dwc3_exynos_remove),
	.driver		= {
		.name	= "exynos-dwc3",
=======
static const struct dev_pm_ops dwc3_exynos_dev_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(dwc3_exynos_suspend, dwc3_exynos_resume)
};

#define DEV_PM_OPS	(&dwc3_exynos_dev_pm_ops)
#else
#define DEV_PM_OPS	NULL
#endif /* CONFIG_PM_SLEEP */

static struct platform_driver dwc3_exynos_driver = {
	.probe		= dwc3_exynos_probe,
	.remove		= dwc3_exynos_remove,
	.driver		= {
		.name	= "exynos-dwc3",
		.of_match_table = of_match_ptr(exynos_dwc3_match),
		.pm	= DEV_PM_OPS,
>>>>>>> refs/remotes/origin/master
	},
};

module_platform_driver(dwc3_exynos_driver);

MODULE_ALIAS("platform:exynos-dwc3");
MODULE_AUTHOR("Anton Tikhomirov <av.tikhomirov@samsung.com>");
<<<<<<< HEAD
MODULE_LICENSE("GPL");
=======
MODULE_LICENSE("GPL v2");
>>>>>>> refs/remotes/origin/master
MODULE_DESCRIPTION("DesignWare USB3 EXYNOS Glue Layer");
