/*
 * Copyright (c) 2008 Sascha Hauer <s.hauer@pengutronix.de>, Pengutronix
 * Copyright (c) 2009 Daniel Mack <daniel@caiaq.de>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

<<<<<<< HEAD
=======
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/io.h>
>>>>>>> refs/remotes/origin/master
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/usb/otg.h>
#include <linux/usb/ulpi.h>
#include <linux/slab.h>
<<<<<<< HEAD

<<<<<<< HEAD
=======
#include <mach/hardware.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <mach/mxc_ehci.h>

#include <asm/mach-types.h>
=======
#include <linux/usb.h>
#include <linux/usb/hcd.h>
#include <linux/platform_data/usb-ehci-mxc.h>
#include "ehci.h"

#define DRIVER_DESC "Freescale On-Chip EHCI Host driver"

static const char hcd_name[] = "ehci-mxc";
>>>>>>> refs/remotes/origin/master

#define ULPI_VIEWPORT_OFFSET	0x170

struct ehci_mxc_priv {
<<<<<<< HEAD
	struct clk *usbclk, *ahbclk, *phy1clk;
	struct usb_hcd *hcd;
};

/* called during probe() after chip reset completes */
static int ehci_mxc_setup(struct usb_hcd *hcd)
{
	struct ehci_hcd *ehci = hcd_to_ehci(hcd);
	int retval;

	dbg_hcs_params(ehci, "reset");
	dbg_hcc_params(ehci, "reset");

	/* cache this readonly data; minimize chip reads */
	ehci->hcs_params = ehci_readl(ehci, &ehci->caps->hcs_params);

	hcd->has_tt = 1;

	retval = ehci_halt(ehci);
	if (retval)
		return retval;

	/* data structure init */
	retval = ehci_init(hcd);
	if (retval)
		return retval;

	ehci->sbrn = 0x20;

	ehci_reset(ehci);

	ehci_port_power(ehci, 0);
	return 0;
}

static const struct hc_driver ehci_mxc_hc_driver = {
	.description = hcd_name,
	.product_desc = "Freescale On-Chip EHCI Host Controller",
	.hcd_priv_size = sizeof(struct ehci_hcd),

	/*
	 * generic hardware linkage
	 */
	.irq = ehci_irq,
	.flags = HCD_USB2 | HCD_MEMORY,

	/*
	 * basic lifecycle operations
	 */
	.reset = ehci_mxc_setup,
	.start = ehci_run,
	.stop = ehci_stop,
	.shutdown = ehci_shutdown,

	/*
	 * managing i/o requests and associated device resources
	 */
	.urb_enqueue = ehci_urb_enqueue,
	.urb_dequeue = ehci_urb_dequeue,
	.endpoint_disable = ehci_endpoint_disable,
	.endpoint_reset = ehci_endpoint_reset,

	/*
	 * scheduling support
	 */
	.get_frame_number = ehci_get_frame,

	/*
	 * root hub support
	 */
	.hub_status_data = ehci_hub_status_data,
	.hub_control = ehci_hub_control,
	.bus_suspend = ehci_bus_suspend,
	.bus_resume = ehci_bus_resume,
	.relinquish_port = ehci_relinquish_port,
	.port_handed_over = ehci_port_handed_over,

	.clear_tt_buffer_complete = ehci_clear_tt_buffer_complete,
=======
	struct clk *usbclk, *ahbclk, *phyclk;
};

static struct hc_driver __read_mostly ehci_mxc_hc_driver;

static const struct ehci_driver_overrides ehci_mxc_overrides __initconst = {
	.extra_priv_size =	sizeof(struct ehci_mxc_priv),
>>>>>>> refs/remotes/origin/master
};

static int ehci_mxc_drv_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct mxc_usbh_platform_data *pdata = pdev->dev.platform_data;
	struct usb_hcd *hcd;
	struct resource *res;
	int irq, ret;
	unsigned int flags;
=======
	struct mxc_usbh_platform_data *pdata = dev_get_platdata(&pdev->dev);
	struct usb_hcd *hcd;
	struct resource *res;
	int irq, ret;
>>>>>>> refs/remotes/origin/master
	struct ehci_mxc_priv *priv;
	struct device *dev = &pdev->dev;
	struct ehci_hcd *ehci;

<<<<<<< HEAD
	dev_info(&pdev->dev, "initializing i.MX USB Controller\n");

=======
>>>>>>> refs/remotes/origin/master
	if (!pdata) {
		dev_err(dev, "No platform data given, bailing out.\n");
		return -EINVAL;
	}

	irq = platform_get_irq(pdev, 0);

	hcd = usb_create_hcd(&ehci_mxc_hc_driver, dev, dev_name(dev));
	if (!hcd)
		return -ENOMEM;

<<<<<<< HEAD
	priv = kzalloc(sizeof(*priv), GFP_KERNEL);
	if (!priv) {
		ret = -ENOMEM;
		goto err_alloc;
	}

=======
>>>>>>> refs/remotes/origin/master
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(dev, "Found HC with no register addr. Check setup!\n");
		ret = -ENODEV;
<<<<<<< HEAD
		goto err_get_resource;
=======
		goto err_alloc;
>>>>>>> refs/remotes/origin/master
	}

	hcd->rsrc_start = res->start;
	hcd->rsrc_len = resource_size(res);

<<<<<<< HEAD
	if (!request_mem_region(hcd->rsrc_start, hcd->rsrc_len, hcd_name)) {
		dev_dbg(dev, "controller already in use\n");
		ret = -EBUSY;
		goto err_request_mem;
	}

	hcd->regs = ioremap(hcd->rsrc_start, hcd->rsrc_len);
	if (!hcd->regs) {
		dev_err(dev, "error mapping memory\n");
		ret = -EFAULT;
		goto err_ioremap;
	}

	/* enable clocks */
	priv->usbclk = clk_get(dev, "usb");
	if (IS_ERR(priv->usbclk)) {
		ret = PTR_ERR(priv->usbclk);
		goto err_clk;
	}
	clk_enable(priv->usbclk);

	if (!cpu_is_mx35() && !cpu_is_mx25()) {
		priv->ahbclk = clk_get(dev, "usb_ahb");
		if (IS_ERR(priv->ahbclk)) {
			ret = PTR_ERR(priv->ahbclk);
			goto err_clk_ahb;
		}
		clk_enable(priv->ahbclk);
	}

	/* "dr" device has its own clock on i.MX51 */
	if (cpu_is_mx51() && (pdev->id == 0)) {
		priv->phy1clk = clk_get(dev, "usb_phy1");
		if (IS_ERR(priv->phy1clk)) {
			ret = PTR_ERR(priv->phy1clk);
			goto err_clk_phy;
		}
		clk_enable(priv->phy1clk);
	}
=======
	hcd->regs = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(hcd->regs)) {
		ret = PTR_ERR(hcd->regs);
		goto err_alloc;
	}

	hcd->has_tt = 1;
	ehci = hcd_to_ehci(hcd);
	priv = (struct ehci_mxc_priv *) ehci->priv;

	/* enable clocks */
	priv->usbclk = devm_clk_get(&pdev->dev, "ipg");
	if (IS_ERR(priv->usbclk)) {
		ret = PTR_ERR(priv->usbclk);
		goto err_alloc;
	}
	clk_prepare_enable(priv->usbclk);

	priv->ahbclk = devm_clk_get(&pdev->dev, "ahb");
	if (IS_ERR(priv->ahbclk)) {
		ret = PTR_ERR(priv->ahbclk);
		goto err_clk_ahb;
	}
	clk_prepare_enable(priv->ahbclk);

	/* "dr" device has its own clock on i.MX51 */
	priv->phyclk = devm_clk_get(&pdev->dev, "phy");
	if (IS_ERR(priv->phyclk))
		priv->phyclk = NULL;
	if (priv->phyclk)
		clk_prepare_enable(priv->phyclk);
>>>>>>> refs/remotes/origin/master


	/* call platform specific init function */
	if (pdata->init) {
		ret = pdata->init(pdev);
		if (ret) {
			dev_err(dev, "platform init failed\n");
			goto err_init;
		}
		/* platforms need some time to settle changed IO settings */
		mdelay(10);
	}

<<<<<<< HEAD
	ehci = hcd_to_ehci(hcd);

=======
>>>>>>> refs/remotes/origin/master
	/* EHCI registers start at offset 0x100 */
	ehci->caps = hcd->regs + 0x100;
	ehci->regs = hcd->regs + 0x100 +
		HC_LENGTH(ehci, ehci_readl(ehci, &ehci->caps->hc_capbase));

	/* set up the PORTSCx register */
	ehci_writel(ehci, pdata->portsc, &ehci->regs->port_status[0]);

	/* is this really needed? */
	msleep(10);

	/* Initialize the transceiver */
	if (pdata->otg) {
		pdata->otg->io_priv = hcd->regs + ULPI_VIEWPORT_OFFSET;
<<<<<<< HEAD
<<<<<<< HEAD
		ret = otg_init(pdata->otg);
=======
		ret = usb_phy_init(pdata->otg);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		ret = usb_phy_init(pdata->otg);
>>>>>>> refs/remotes/origin/master
		if (ret) {
			dev_err(dev, "unable to init transceiver, probably missing\n");
			ret = -ENODEV;
			goto err_add;
		}
<<<<<<< HEAD
<<<<<<< HEAD
		ret = otg_set_vbus(pdata->otg, 1);
=======
		ret = otg_set_vbus(pdata->otg->otg, 1);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		ret = otg_set_vbus(pdata->otg->otg, 1);
>>>>>>> refs/remotes/origin/master
		if (ret) {
			dev_err(dev, "unable to enable vbus on transceiver\n");
			goto err_add;
		}
	}

<<<<<<< HEAD
	priv->hcd = hcd;
	platform_set_drvdata(pdev, priv);

<<<<<<< HEAD
	ret = usb_add_hcd(hcd, irq, IRQF_DISABLED | IRQF_SHARED);
=======
	ret = usb_add_hcd(hcd, irq, IRQF_SHARED);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret)
		goto err_add;

	if (pdata->otg) {
		/*
		 * efikamx and efikasb have some hardware bug which is
		 * preventing usb to work unless CHRGVBUS is set.
		 * It's in violation of USB specs
		 */
		if (machine_is_mx51_efikamx() || machine_is_mx51_efikasb()) {
<<<<<<< HEAD
			flags = otg_io_read(pdata->otg, ULPI_OTG_CTRL);
			flags |= ULPI_OTG_CTRL_CHRGVBUS;
			ret = otg_io_write(pdata->otg, flags, ULPI_OTG_CTRL);
=======
			flags = usb_phy_io_read(pdata->otg,
							ULPI_OTG_CTRL);
			flags |= ULPI_OTG_CTRL_CHRGVBUS;
			ret = usb_phy_io_write(pdata->otg, flags,
							ULPI_OTG_CTRL);
>>>>>>> refs/remotes/origin/cm-10.0
			if (ret) {
				dev_err(dev, "unable to set CHRVBUS\n");
				goto err_add;
			}
		}
	}

=======
	platform_set_drvdata(pdev, hcd);

	ret = usb_add_hcd(hcd, irq, IRQF_SHARED);
	if (ret)
		goto err_add;

	device_wakeup_enable(hcd->self.controller);
>>>>>>> refs/remotes/origin/master
	return 0;

err_add:
	if (pdata && pdata->exit)
		pdata->exit(pdev);
err_init:
<<<<<<< HEAD
	if (priv->phy1clk) {
		clk_disable(priv->phy1clk);
		clk_put(priv->phy1clk);
	}
err_clk_phy:
	if (priv->ahbclk) {
		clk_disable(priv->ahbclk);
		clk_put(priv->ahbclk);
	}
err_clk_ahb:
	clk_disable(priv->usbclk);
	clk_put(priv->usbclk);
err_clk:
	iounmap(hcd->regs);
err_ioremap:
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
err_request_mem:
err_get_resource:
	kfree(priv);
=======
	if (priv->phyclk)
		clk_disable_unprepare(priv->phyclk);

	clk_disable_unprepare(priv->ahbclk);
err_clk_ahb:
	clk_disable_unprepare(priv->usbclk);
>>>>>>> refs/remotes/origin/master
err_alloc:
	usb_put_hcd(hcd);
	return ret;
}

<<<<<<< HEAD
static int __exit ehci_mxc_drv_remove(struct platform_device *pdev)
{
	struct mxc_usbh_platform_data *pdata = pdev->dev.platform_data;
	struct ehci_mxc_priv *priv = platform_get_drvdata(pdev);
	struct usb_hcd *hcd = priv->hcd;
=======
static int ehci_mxc_drv_remove(struct platform_device *pdev)
{
	struct mxc_usbh_platform_data *pdata = dev_get_platdata(&pdev->dev);
	struct usb_hcd *hcd = platform_get_drvdata(pdev);
	struct ehci_hcd *ehci = hcd_to_ehci(hcd);
	struct ehci_mxc_priv *priv = (struct ehci_mxc_priv *) ehci->priv;

	usb_remove_hcd(hcd);
>>>>>>> refs/remotes/origin/master

	if (pdata && pdata->exit)
		pdata->exit(pdev);

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	if (pdata->otg)
		otg_shutdown(pdata->otg);
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (pdata && pdata->otg)
		usb_phy_shutdown(pdata->otg);
>>>>>>> refs/remotes/origin/cm-10.0

	usb_remove_hcd(hcd);
	iounmap(hcd->regs);
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
	usb_put_hcd(hcd);
	platform_set_drvdata(pdev, NULL);

	clk_disable(priv->usbclk);
	clk_put(priv->usbclk);
	if (priv->ahbclk) {
		clk_disable(priv->ahbclk);
		clk_put(priv->ahbclk);
	}
	if (priv->phy1clk) {
		clk_disable(priv->phy1clk);
		clk_put(priv->phy1clk);
	}

	kfree(priv);

	return 0;
}

static void ehci_mxc_drv_shutdown(struct platform_device *pdev)
{
	struct ehci_mxc_priv *priv = platform_get_drvdata(pdev);
	struct usb_hcd *hcd = priv->hcd;

	if (hcd->driver->shutdown)
		hcd->driver->shutdown(hcd);
}

=======
	if (pdata && pdata->otg)
		usb_phy_shutdown(pdata->otg);

	clk_disable_unprepare(priv->usbclk);
	clk_disable_unprepare(priv->ahbclk);

	if (priv->phyclk)
		clk_disable_unprepare(priv->phyclk);

	usb_put_hcd(hcd);
	return 0;
}

>>>>>>> refs/remotes/origin/master
MODULE_ALIAS("platform:mxc-ehci");

static struct platform_driver ehci_mxc_driver = {
	.probe = ehci_mxc_drv_probe,
<<<<<<< HEAD
	.remove = __exit_p(ehci_mxc_drv_remove),
	.shutdown = ehci_mxc_drv_shutdown,
=======
	.remove = ehci_mxc_drv_remove,
	.shutdown = usb_hcd_platform_shutdown,
>>>>>>> refs/remotes/origin/master
	.driver = {
		   .name = "mxc-ehci",
	},
};
<<<<<<< HEAD
=======

static int __init ehci_mxc_init(void)
{
	if (usb_disabled())
		return -ENODEV;

	pr_info("%s: " DRIVER_DESC "\n", hcd_name);

	ehci_init_driver(&ehci_mxc_hc_driver, &ehci_mxc_overrides);
	return platform_driver_register(&ehci_mxc_driver);
}
module_init(ehci_mxc_init);

static void __exit ehci_mxc_cleanup(void)
{
	platform_driver_unregister(&ehci_mxc_driver);
}
module_exit(ehci_mxc_cleanup);

MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_AUTHOR("Sascha Hauer");
MODULE_LICENSE("GPL");
>>>>>>> refs/remotes/origin/master
