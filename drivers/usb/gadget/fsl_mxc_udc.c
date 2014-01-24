/*
 * Copyright (C) 2009
 * Guennadi Liakhovetski, DENX Software Engineering, <lg@denx.de>
 *
 * Description:
 * Helper routines for i.MX3x SoCs from Freescale, needed by the fsl_usb2_udc.c
 * driver to function correctly on these systems.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */
#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/fsl_devices.h>
#include <linux/platform_device.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/io.h>
>>>>>>> refs/remotes/origin/cm-10.0

#include <mach/hardware.h>

static struct clk *mxc_ahb_clk;
static struct clk *mxc_usb_clk;

/* workaround ENGcm09152 for i.MX35 */
#define USBPHYCTRL_OTGBASE_OFFSET	0x608
=======
#include <linux/io.h>

static struct clk *mxc_ahb_clk;
static struct clk *mxc_per_clk;
static struct clk *mxc_ipg_clk;

/* workaround ENGcm09152 for i.MX35 */
#define MX35_USBPHYCTRL_OFFSET		0x600
#define USBPHYCTRL_OTGBASE_OFFSET	0x8
>>>>>>> refs/remotes/origin/master
#define USBPHYCTRL_EVDO			(1 << 23)

int fsl_udc_clk_init(struct platform_device *pdev)
{
	struct fsl_usb2_platform_data *pdata;
	unsigned long freq;
	int ret;

<<<<<<< HEAD
	pdata = pdev->dev.platform_data;

	if (!cpu_is_mx35() && !cpu_is_mx25()) {
		mxc_ahb_clk = clk_get(&pdev->dev, "usb_ahb");
		if (IS_ERR(mxc_ahb_clk))
			return PTR_ERR(mxc_ahb_clk);

		ret = clk_enable(mxc_ahb_clk);
		if (ret < 0) {
			dev_err(&pdev->dev, "clk_enable(\"usb_ahb\") failed\n");
			goto eenahb;
		}
	}

	/* make sure USB_CLK is running at 60 MHz +/- 1000 Hz */
	mxc_usb_clk = clk_get(&pdev->dev, "usb");
	if (IS_ERR(mxc_usb_clk)) {
		dev_err(&pdev->dev, "clk_get(\"usb\") failed\n");
		ret = PTR_ERR(mxc_usb_clk);
		goto egusb;
	}

	if (!cpu_is_mx51()) {
		freq = clk_get_rate(mxc_usb_clk);
=======
	pdata = dev_get_platdata(&pdev->dev);

	mxc_ipg_clk = devm_clk_get(&pdev->dev, "ipg");
	if (IS_ERR(mxc_ipg_clk)) {
		dev_err(&pdev->dev, "clk_get(\"ipg\") failed\n");
		return PTR_ERR(mxc_ipg_clk);
	}

	mxc_ahb_clk = devm_clk_get(&pdev->dev, "ahb");
	if (IS_ERR(mxc_ahb_clk)) {
		dev_err(&pdev->dev, "clk_get(\"ahb\") failed\n");
		return PTR_ERR(mxc_ahb_clk);
	}

	mxc_per_clk = devm_clk_get(&pdev->dev, "per");
	if (IS_ERR(mxc_per_clk)) {
		dev_err(&pdev->dev, "clk_get(\"per\") failed\n");
		return PTR_ERR(mxc_per_clk);
	}

	clk_prepare_enable(mxc_ipg_clk);
	clk_prepare_enable(mxc_ahb_clk);
	clk_prepare_enable(mxc_per_clk);

	/* make sure USB_CLK is running at 60 MHz +/- 1000 Hz */
	if (!strcmp(pdev->id_entry->name, "imx-udc-mx27")) {
		freq = clk_get_rate(mxc_per_clk);
>>>>>>> refs/remotes/origin/master
		if (pdata->phy_mode != FSL_USB2_PHY_ULPI &&
		    (freq < 59999000 || freq > 60001000)) {
			dev_err(&pdev->dev, "USB_CLK=%lu, should be 60MHz\n", freq);
			ret = -EINVAL;
			goto eclkrate;
		}
	}

<<<<<<< HEAD
	ret = clk_enable(mxc_usb_clk);
	if (ret < 0) {
		dev_err(&pdev->dev, "clk_enable(\"usb_clk\") failed\n");
		goto eenusb;
	}

	return 0;

eenusb:
eclkrate:
	clk_put(mxc_usb_clk);
	mxc_usb_clk = NULL;
egusb:
	if (!cpu_is_mx35())
		clk_disable(mxc_ahb_clk);
eenahb:
	if (!cpu_is_mx35())
		clk_put(mxc_ahb_clk);
	return ret;
}

void fsl_udc_clk_finalize(struct platform_device *pdev)
{
	struct fsl_usb2_platform_data *pdata = pdev->dev.platform_data;
<<<<<<< HEAD
#if defined(CONFIG_SOC_IMX35)
=======
>>>>>>> refs/remotes/origin/cm-10.0
	if (cpu_is_mx35()) {
		unsigned int v;

		/* workaround ENGcm09152 for i.MX35 */
		if (pdata->workaround & FLS_USB2_WORKAROUND_ENGCM09152) {
			v = readl(MX35_IO_ADDRESS(MX35_USB_BASE_ADDR +
					USBPHYCTRL_OTGBASE_OFFSET));
			writel(v | USBPHYCTRL_EVDO,
				MX35_IO_ADDRESS(MX35_USB_BASE_ADDR +
					USBPHYCTRL_OTGBASE_OFFSET));
		}
	}
<<<<<<< HEAD
#endif
=======
>>>>>>> refs/remotes/origin/cm-10.0

	/* ULPI transceivers don't need usbpll */
	if (pdata->phy_mode == FSL_USB2_PHY_ULPI) {
		clk_disable(mxc_usb_clk);
		clk_put(mxc_usb_clk);
		mxc_usb_clk = NULL;
	}
=======
	return 0;

eclkrate:
	clk_disable_unprepare(mxc_ipg_clk);
	clk_disable_unprepare(mxc_ahb_clk);
	clk_disable_unprepare(mxc_per_clk);
	mxc_per_clk = NULL;
	return ret;
}

int fsl_udc_clk_finalize(struct platform_device *pdev)
{
	struct fsl_usb2_platform_data *pdata = dev_get_platdata(&pdev->dev);
	int ret = 0;

	/* workaround ENGcm09152 for i.MX35 */
	if (pdata->workaround & FLS_USB2_WORKAROUND_ENGCM09152) {
		unsigned int v;
		struct resource *res = platform_get_resource
			(pdev, IORESOURCE_MEM, 0);
		void __iomem *phy_regs = ioremap(res->start +
						MX35_USBPHYCTRL_OFFSET, 512);
		if (!phy_regs) {
			dev_err(&pdev->dev, "ioremap for phy address fails\n");
			ret = -EINVAL;
			goto ioremap_err;
		}

		v = readl(phy_regs + USBPHYCTRL_OTGBASE_OFFSET);
		writel(v | USBPHYCTRL_EVDO,
			phy_regs + USBPHYCTRL_OTGBASE_OFFSET);

		iounmap(phy_regs);
	}


ioremap_err:
	/* ULPI transceivers don't need usbpll */
	if (pdata->phy_mode == FSL_USB2_PHY_ULPI) {
		clk_disable_unprepare(mxc_per_clk);
		mxc_per_clk = NULL;
	}

	return ret;
>>>>>>> refs/remotes/origin/master
}

void fsl_udc_clk_release(void)
{
<<<<<<< HEAD
	if (mxc_usb_clk) {
		clk_disable(mxc_usb_clk);
		clk_put(mxc_usb_clk);
	}
	if (!cpu_is_mx35()) {
		clk_disable(mxc_ahb_clk);
		clk_put(mxc_ahb_clk);
	}
=======
	if (mxc_per_clk)
		clk_disable_unprepare(mxc_per_clk);
	clk_disable_unprepare(mxc_ahb_clk);
	clk_disable_unprepare(mxc_ipg_clk);
>>>>>>> refs/remotes/origin/master
}
