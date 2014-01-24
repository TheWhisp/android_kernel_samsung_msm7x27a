/*
 * ehci-omap.c - driver for USBHOST on OMAP3/4 processors
 *
 * Bus Glue for the EHCI controllers in OMAP3/4
 * Tested on several OMAP3 boards, and OMAP4 Pandaboard
 *
<<<<<<< HEAD
 * Copyright (C) 2007-2011 Texas Instruments, Inc.
 *	Author: Vikram Pandita <vikram.pandita@ti.com>
 *	Author: Anand Gadiyar <gadiyar@ti.com>
 *	Author: Keshava Munegowda <keshava_mgowda@ti.com>
=======
 * Copyright (C) 2007-2013 Texas Instruments, Inc.
 *	Author: Vikram Pandita <vikram.pandita@ti.com>
 *	Author: Anand Gadiyar <gadiyar@ti.com>
 *	Author: Keshava Munegowda <keshava_mgowda@ti.com>
 *	Author: Roger Quadros <rogerq@ti.com>
>>>>>>> refs/remotes/origin/master
 *
 * Copyright (C) 2009 Nokia Corporation
 *	Contact: Felipe Balbi <felipe.balbi@nokia.com>
 *
 * Based on "ehci-fsl.c" and "ehci-au1xxx.c" ehci glue layers
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
<<<<<<< HEAD
 * TODO (last updated Feb 27, 2010):
 *	- add kernel-doc
 *	- enable AUTOIDLE
 *	- add suspend/resume
 *	- add HSIC and TLL support
 *	- convert to use hwmod and runtime PM
 */

#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/usb/ulpi.h>
#include <plat/usb.h>
#include <linux/regulator/consumer.h>
<<<<<<< HEAD
=======
#include <linux/pm_runtime.h>
#include <linux/gpio.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/usb/ulpi.h>
#include <linux/pm_runtime.h>
#include <linux/gpio.h>
#include <linux/clk.h>
#include <linux/usb.h>
#include <linux/usb/hcd.h>
#include <linux/of.h>
#include <linux/dma-mapping.h>

#include "ehci.h"

#include <linux/platform_data/usb-omap.h>
>>>>>>> refs/remotes/origin/master

/* EHCI Register Set */
#define EHCI_INSNREG04					(0xA0)
#define EHCI_INSNREG04_DISABLE_UNSUSPEND		(1 << 5)
#define	EHCI_INSNREG05_ULPI				(0xA4)
#define	EHCI_INSNREG05_ULPI_CONTROL_SHIFT		31
#define	EHCI_INSNREG05_ULPI_PORTSEL_SHIFT		24
#define	EHCI_INSNREG05_ULPI_OPSEL_SHIFT			22
#define	EHCI_INSNREG05_ULPI_REGADD_SHIFT		16
#define	EHCI_INSNREG05_ULPI_EXTREGADD_SHIFT		8
#define	EHCI_INSNREG05_ULPI_WRDATA_SHIFT		0

<<<<<<< HEAD
/*-------------------------------------------------------------------------*/

static const struct hc_driver ehci_omap_hc_driver;

=======
#define DRIVER_DESC "OMAP-EHCI Host Controller driver"

static const char hcd_name[] = "ehci-omap";

/*-------------------------------------------------------------------------*/

struct omap_hcd {
	struct usb_phy *phy[OMAP3_HS_USB_PORTS]; /* one PHY for each port */
	int nports;
};
>>>>>>> refs/remotes/origin/master

static inline void ehci_write(void __iomem *base, u32 reg, u32 val)
{
	__raw_writel(val, base + reg);
}

static inline u32 ehci_read(void __iomem *base, u32 reg)
{
	return __raw_readl(base + reg);
}

<<<<<<< HEAD
static void omap_ehci_soft_phy_reset(struct platform_device *pdev, u8 port)
{
	struct usb_hcd	*hcd = dev_get_drvdata(&pdev->dev);
	unsigned long timeout = jiffies + msecs_to_jiffies(1000);
	unsigned reg = 0;

	reg = ULPI_FUNC_CTRL_RESET
		/* FUNCTION_CTRL_SET register */
		| (ULPI_SET(ULPI_FUNC_CTRL) << EHCI_INSNREG05_ULPI_REGADD_SHIFT)
		/* Write */
		| (2 << EHCI_INSNREG05_ULPI_OPSEL_SHIFT)
		/* PORTn */
		| ((port + 1) << EHCI_INSNREG05_ULPI_PORTSEL_SHIFT)
		/* start ULPI access*/
		| (1 << EHCI_INSNREG05_ULPI_CONTROL_SHIFT);

	ehci_write(hcd->regs, EHCI_INSNREG05_ULPI, reg);

	/* Wait for ULPI access completion */
	while ((ehci_read(hcd->regs, EHCI_INSNREG05_ULPI)
			& (1 << EHCI_INSNREG05_ULPI_CONTROL_SHIFT))) {
		cpu_relax();

		if (time_after(jiffies, timeout)) {
			dev_dbg(&pdev->dev, "phy reset operation timed out\n");
			break;
		}
	}
}

<<<<<<< HEAD
=======
static void disable_put_regulator(
		struct ehci_hcd_omap_platform_data *pdata)
{
	int i;

	for (i = 0 ; i < OMAP3_HS_USB_PORTS ; i++) {
		if (pdata->regulator[i]) {
			regulator_disable(pdata->regulator[i]);
			regulator_put(pdata->regulator[i]);
		}
	}
}
>>>>>>> refs/remotes/origin/cm-10.0

/* configure so an HC device and id are always provided */
/* always called with process context; sleeping is OK */

=======
/* configure so an HC device and id are always provided */
/* always called with process context; sleeping is OK */

static struct hc_driver __read_mostly ehci_omap_hc_driver;

static const struct ehci_driver_overrides ehci_omap_overrides __initdata = {
	.extra_priv_size = sizeof(struct omap_hcd),
};

>>>>>>> refs/remotes/origin/master
/**
 * ehci_hcd_omap_probe - initialize TI-based HCDs
 *
 * Allocates basic resources for this USB host controller, and
 * then invokes the start() method for the HCD associated with it
 * through the hotplug entry's driver_data.
 */
static int ehci_hcd_omap_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct device				*dev = &pdev->dev;
	struct ehci_hcd_omap_platform_data	*pdata = dev->platform_data;
	struct resource				*res;
	struct usb_hcd				*hcd;
	void __iomem				*regs;
	struct ehci_hcd				*omap_ehci;
	int					ret = -ENODEV;
	int					irq;
	int					i;
	char					supply[7];
=======
	struct device *dev = &pdev->dev;
	struct usbhs_omap_platform_data *pdata = dev_get_platdata(dev);
	struct resource	*res;
	struct usb_hcd	*hcd;
	void __iomem *regs;
	int ret;
	int irq;
	int i;
	struct omap_hcd	*omap;
>>>>>>> refs/remotes/origin/master

	if (usb_disabled())
		return -ENODEV;

	if (!dev->parent) {
		dev_err(dev, "Missing parent device\n");
		return -ENODEV;
	}

<<<<<<< HEAD
	irq = platform_get_irq_byname(pdev, "ehci-irq");
	if (irq < 0) {
		dev_err(dev, "EHCI irq failed\n");
		return -ENODEV;
	}

	res =  platform_get_resource_byname(pdev,
				IORESOURCE_MEM, "ehci");
	if (!res) {
		dev_err(dev, "UHH EHCI get resource failed\n");
		return -ENODEV;
	}

	regs = ioremap(res->start, resource_size(res));
	if (!regs) {
		dev_err(dev, "UHH EHCI ioremap failed\n");
		return -ENOMEM;
	}

	hcd = usb_create_hcd(&ehci_omap_hc_driver, dev,
			dev_name(dev));
	if (!hcd) {
		dev_err(dev, "failed to create hcd with err %d\n", ret);
		ret = -ENOMEM;
		goto err_io;
=======
	/* For DT boot, get platform data from parent. i.e. usbhshost */
	if (dev->of_node) {
		pdata = dev_get_platdata(dev->parent);
		dev->platform_data = pdata;
	}

	if (!pdata) {
		dev_err(dev, "Missing platform data\n");
		return -ENODEV;
	}

	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		dev_err(dev, "EHCI irq failed\n");
		return -ENODEV;
	}

	res =  platform_get_resource(pdev, IORESOURCE_MEM, 0);
	regs = devm_ioremap_resource(dev, res);
	if (IS_ERR(regs))
		return PTR_ERR(regs);

	/*
	 * Right now device-tree probed devices don't get dma_mask set.
	 * Since shared usb code relies on it, set it here for now.
	 * Once we have dma capability bindings this can go away.
	 */
	ret = dma_coerce_mask_and_coherent(dev, DMA_BIT_MASK(32));
	if (ret)
		return ret;

	ret = -ENODEV;
	hcd = usb_create_hcd(&ehci_omap_hc_driver, dev,
			dev_name(dev));
	if (!hcd) {
		dev_err(dev, "Failed to create HCD\n");
		return -ENOMEM;
>>>>>>> refs/remotes/origin/master
	}

	hcd->rsrc_start = res->start;
	hcd->rsrc_len = resource_size(res);
	hcd->regs = regs;
<<<<<<< HEAD

	/* get ehci regulator and enable */
	for (i = 0 ; i < OMAP3_HS_USB_PORTS ; i++) {
		if (pdata->port_mode[i] != OMAP_EHCI_PORT_MODE_PHY) {
			pdata->regulator[i] = NULL;
			continue;
		}
		snprintf(supply, sizeof(supply), "hsusb%d", i);
		pdata->regulator[i] = regulator_get(dev, supply);
		if (IS_ERR(pdata->regulator[i])) {
			pdata->regulator[i] = NULL;
			dev_dbg(dev,
			"failed to get ehci port%d regulator\n", i);
		} else {
			regulator_enable(pdata->regulator[i]);
		}
	}

<<<<<<< HEAD
	ret = omap_usbhs_enable(dev);
	if (ret) {
		dev_err(dev, "failed to start usbhs with err %d\n", ret);
		goto err_enable;
	}

=======
	if (pdata->phy_reset) {
		if (gpio_is_valid(pdata->reset_gpio_port[0]))
			gpio_request_one(pdata->reset_gpio_port[0],
					 GPIOF_OUT_INIT_LOW, "USB1 PHY reset");

		if (gpio_is_valid(pdata->reset_gpio_port[1]))
			gpio_request_one(pdata->reset_gpio_port[1],
					 GPIOF_OUT_INIT_LOW, "USB2 PHY reset");

		/* Hold the PHY in RESET for enough time till DIR is high */
		udelay(10);
=======
	hcd_to_ehci(hcd)->caps = regs;

	omap = (struct omap_hcd *)hcd_to_ehci(hcd)->priv;
	omap->nports = pdata->nports;

	platform_set_drvdata(pdev, hcd);

	/* get the PHY devices if needed */
	for (i = 0 ; i < omap->nports ; i++) {
		struct usb_phy *phy;

		/* get the PHY device */
		if (dev->of_node)
			phy = devm_usb_get_phy_by_phandle(dev, "phys", i);
		else
			phy = devm_usb_get_phy_dev(dev, i);
		if (IS_ERR(phy)) {
			/* Don't bail out if PHY is not absolutely necessary */
			if (pdata->port_mode[i] != OMAP_EHCI_PORT_MODE_PHY)
				continue;

			ret = PTR_ERR(phy);
			dev_err(dev, "Can't get PHY device for port %d: %d\n",
					i, ret);
			goto err_phy;
		}

		omap->phy[i] = phy;

		if (pdata->port_mode[i] == OMAP_EHCI_PORT_MODE_PHY) {
			usb_phy_init(omap->phy[i]);
			/* bring PHY out of suspend */
			usb_phy_set_suspend(omap->phy[i], 0);
		}
>>>>>>> refs/remotes/origin/master
	}

	pm_runtime_enable(dev);
	pm_runtime_get_sync(dev);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * An undocumented "feature" in the OMAP3 EHCI controller,
	 * causes suspended ports to be taken out of suspend when
	 * the USBCMD.Run/Stop bit is cleared (for example when
	 * we do ehci_bus_suspend).
	 * This breaks suspend-resume if the root-hub is allowed
	 * to suspend. Writing 1 to this undocumented register bit
	 * disables this feature and restores normal behavior.
	 */
	ehci_write(regs, EHCI_INSNREG04,
				EHCI_INSNREG04_DISABLE_UNSUSPEND);

<<<<<<< HEAD
	/* Soft reset the PHY using PHY reset command over ULPI */
	if (pdata->port_mode[0] == OMAP_EHCI_PORT_MODE_PHY)
		omap_ehci_soft_phy_reset(pdev, 0);
	if (pdata->port_mode[1] == OMAP_EHCI_PORT_MODE_PHY)
		omap_ehci_soft_phy_reset(pdev, 1);

	omap_ehci = hcd_to_ehci(hcd);
	omap_ehci->sbrn = 0x20;

	/* we know this is the memory we want, no need to ioremap again */
	omap_ehci->caps = hcd->regs;
	omap_ehci->regs = hcd->regs
		+ HC_LENGTH(ehci, readl(&omap_ehci->caps->hc_capbase));

	dbg_hcs_params(omap_ehci, "reset");
	dbg_hcc_params(omap_ehci, "reset");

	/* cache this readonly data; minimize chip reads */
	omap_ehci->hcs_params = readl(&omap_ehci->caps->hcs_params);

<<<<<<< HEAD
	ret = usb_add_hcd(hcd, irq, IRQF_DISABLED | IRQF_SHARED);
=======
	ehci_reset(omap_ehci);

	ret = usb_add_hcd(hcd, irq, IRQF_SHARED);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret) {
		dev_err(dev, "failed to add hcd with err %d\n", ret);
		goto err_add_hcd;
	}

	/* root ports should always stay powered */
	ehci_port_power(omap_ehci, 1);

<<<<<<< HEAD
	return 0;

err_add_hcd:
	omap_usbhs_disable(dev);

err_enable:
	usb_put_hcd(hcd);

err_io:
=======
	if (pdata->phy_reset) {
		/* Hold the PHY in RESET for enough time till
		 * PHY is settled and ready
		 */
		udelay(10);

		if (gpio_is_valid(pdata->reset_gpio_port[0]))
			gpio_set_value(pdata->reset_gpio_port[0], 1);

		if (gpio_is_valid(pdata->reset_gpio_port[1]))
			gpio_set_value(pdata->reset_gpio_port[1], 1);
=======
	ret = usb_add_hcd(hcd, irq, IRQF_SHARED);
	if (ret) {
		dev_err(dev, "failed to add hcd with err %d\n", ret);
		goto err_pm_runtime;
	}
	device_wakeup_enable(hcd->self.controller);

	/*
	 * Bring PHYs out of reset for non PHY modes.
	 * Even though HSIC mode is a PHY-less mode, the reset
	 * line exists between the chips and can be modelled
	 * as a PHY device for reset control.
	 */
	for (i = 0; i < omap->nports; i++) {
		if (!omap->phy[i] ||
		     pdata->port_mode[i] == OMAP_EHCI_PORT_MODE_PHY)
			continue;

		usb_phy_init(omap->phy[i]);
		/* bring PHY out of suspend */
		usb_phy_set_suspend(omap->phy[i], 0);
>>>>>>> refs/remotes/origin/master
	}

	return 0;

<<<<<<< HEAD
err_add_hcd:
	disable_put_regulator(pdata);
	pm_runtime_put_sync(dev);

err_io:
	iounmap(regs);
>>>>>>> refs/remotes/origin/cm-10.0
=======
err_pm_runtime:
	pm_runtime_put_sync(dev);

err_phy:
	for (i = 0; i < omap->nports; i++) {
		if (omap->phy[i])
			usb_phy_shutdown(omap->phy[i]);
	}

	usb_put_hcd(hcd);

>>>>>>> refs/remotes/origin/master
	return ret;
}


/**
 * ehci_hcd_omap_remove - shutdown processing for EHCI HCDs
 * @pdev: USB Host Controller being removed
 *
 * Reverses the effect of usb_ehci_hcd_omap_probe(), first invoking
 * the HCD's stop() method.  It is always called from a thread
 * context, normally "rmmod", "apmd", or something similar.
 */
static int ehci_hcd_omap_remove(struct platform_device *pdev)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct device *dev	= &pdev->dev;
	struct usb_hcd *hcd	= dev_get_drvdata(dev);

	usb_remove_hcd(hcd);
	omap_usbhs_disable(dev);
	usb_put_hcd(hcd);
=======
	struct device *dev				= &pdev->dev;
	struct usb_hcd *hcd				= dev_get_drvdata(dev);
	struct ehci_hcd_omap_platform_data *pdata	= dev->platform_data;

	usb_remove_hcd(hcd);
	disable_put_regulator(dev->platform_data);
	iounmap(hcd->regs);
=======
	struct device *dev = &pdev->dev;
	struct usb_hcd *hcd = dev_get_drvdata(dev);
	struct omap_hcd *omap = (struct omap_hcd *)hcd_to_ehci(hcd)->priv;
	int i;

	usb_remove_hcd(hcd);

	for (i = 0; i < omap->nports; i++) {
		if (omap->phy[i])
			usb_phy_shutdown(omap->phy[i]);
	}

>>>>>>> refs/remotes/origin/master
	usb_put_hcd(hcd);
	pm_runtime_put_sync(dev);
	pm_runtime_disable(dev);

<<<<<<< HEAD
	if (pdata->phy_reset) {
		if (gpio_is_valid(pdata->reset_gpio_port[0]))
			gpio_free(pdata->reset_gpio_port[0]);

		if (gpio_is_valid(pdata->reset_gpio_port[1]))
			gpio_free(pdata->reset_gpio_port[1]);
	}
>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
}

static void ehci_hcd_omap_shutdown(struct platform_device *pdev)
{
	struct usb_hcd *hcd = dev_get_drvdata(&pdev->dev);

	if (hcd->driver->shutdown)
		hcd->driver->shutdown(hcd);
}
=======
	return 0;
}

static const struct of_device_id omap_ehci_dt_ids[] = {
	{ .compatible = "ti,ehci-omap" },
	{ }
};

MODULE_DEVICE_TABLE(of, omap_ehci_dt_ids);
>>>>>>> refs/remotes/origin/master

static struct platform_driver ehci_hcd_omap_driver = {
	.probe			= ehci_hcd_omap_probe,
	.remove			= ehci_hcd_omap_remove,
<<<<<<< HEAD
	.shutdown		= ehci_hcd_omap_shutdown,
	/*.suspend		= ehci_hcd_omap_suspend, */
	/*.resume		= ehci_hcd_omap_resume, */
	.driver = {
		.name		= "ehci-omap",
=======
	.shutdown		= usb_hcd_platform_shutdown,
	/*.suspend		= ehci_hcd_omap_suspend, */
	/*.resume		= ehci_hcd_omap_resume, */
	.driver = {
		.name		= hcd_name,
		.of_match_table = omap_ehci_dt_ids,
>>>>>>> refs/remotes/origin/master
	}
};

/*-------------------------------------------------------------------------*/

<<<<<<< HEAD
static const struct hc_driver ehci_omap_hc_driver = {
	.description		= hcd_name,
	.product_desc		= "OMAP-EHCI Host Controller",
	.hcd_priv_size		= sizeof(struct ehci_hcd),

	/*
	 * generic hardware linkage
	 */
	.irq			= ehci_irq,
	.flags			= HCD_MEMORY | HCD_USB2,

	/*
	 * basic lifecycle operations
	 */
	.reset			= ehci_init,
	.start			= ehci_run,
	.stop			= ehci_stop,
	.shutdown		= ehci_shutdown,

	/*
	 * managing i/o requests and associated device resources
	 */
	.urb_enqueue		= ehci_urb_enqueue,
	.urb_dequeue		= ehci_urb_dequeue,
	.endpoint_disable	= ehci_endpoint_disable,
	.endpoint_reset		= ehci_endpoint_reset,

	/*
	 * scheduling support
	 */
	.get_frame_number	= ehci_get_frame,

	/*
	 * root hub support
	 */
	.hub_status_data	= ehci_hub_status_data,
	.hub_control		= ehci_hub_control,
	.bus_suspend		= ehci_bus_suspend,
	.bus_resume		= ehci_bus_resume,

	.clear_tt_buffer_complete = ehci_clear_tt_buffer_complete,
};
=======
static int __init ehci_omap_init(void)
{
	if (usb_disabled())
		return -ENODEV;

	pr_info("%s: " DRIVER_DESC "\n", hcd_name);

<<<<<<< HEAD
	ehci_init_driver(&ehci_omap_hc_driver, &ehci_omap_overrides);
	return platform_driver_register(&ehci_hcd_omap_driver);
}
module_init(ehci_omap_init);

static void __exit ehci_omap_cleanup(void)
{
	platform_driver_unregister(&ehci_hcd_omap_driver);
}
module_exit(ehci_omap_cleanup);
>>>>>>> refs/remotes/origin/master

=======
>>>>>>> refs/remotes/origin/cm-11.0
MODULE_ALIAS("platform:ehci-omap");
MODULE_AUTHOR("Texas Instruments, Inc.");
MODULE_AUTHOR("Felipe Balbi <felipe.balbi@nokia.com>");
<<<<<<< HEAD

=======
MODULE_AUTHOR("Roger Quadros <rogerq@ti.com>");

MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");
>>>>>>> refs/remotes/origin/master
