/* ehci-msm.c - HSUSB Host Controller Driver Implementation
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright (c) 2008-2011, The Linux Foundation. All rights reserved.
=======
 * Copyright (c) 2008-2012, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Copyright (c) 2008-2011, Code Aurora Forum. All rights reserved.
>>>>>>> refs/remotes/origin/master
 *
 * Partly derived from ehci-fsl.c and ehci-hcd.c
 * Copyright (c) 2000-2004 by David Brownell
 * Copyright (c) 2005 MontaVista Software
 *
 * All source code in this file is licensed under the following license except
 * where indicated.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you can find it at http://www.fsf.org
 */

<<<<<<< HEAD
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/pm_runtime.h>

#include <linux/usb/otg.h>
#include <linux/usb/msm_hsusb_hw.h>

#define MSM_USB_BASE (hcd->regs)

<<<<<<< HEAD
static struct otg_transceiver *otg;
=======
static struct usb_phy *phy;
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/usb/otg.h>
#include <linux/usb/msm_hsusb_hw.h>
#include <linux/usb.h>
#include <linux/usb/hcd.h>

#include "ehci.h"

#define MSM_USB_BASE (hcd->regs)

#define DRIVER_DESC "Qualcomm On-Chip EHCI Host Controller"

static const char hcd_name[] = "ehci-msm";
static struct hc_driver __read_mostly msm_hc_driver;
>>>>>>> refs/remotes/origin/master

static int ehci_msm_reset(struct usb_hcd *hcd)
{
	struct ehci_hcd *ehci = hcd_to_ehci(hcd);
	int retval;

	ehci->caps = USB_CAPLENGTH;
<<<<<<< HEAD
<<<<<<< HEAD
	ehci->regs = USB_CAPLENGTH +
		HC_LENGTH(ehci, ehci_readl(ehci, &ehci->caps->hc_capbase));
	dbg_hcs_params(ehci, "reset");
	dbg_hcc_params(ehci, "reset");

	/* cache the data to minimize the chip reads*/
	ehci->hcs_params = ehci_readl(ehci, &ehci->caps->hcs_params);

	hcd->has_tt = 1;
	ehci->sbrn = HCD_USB2;

	retval = ehci_halt(ehci);
	if (retval)
		return retval;

	/* data structure init */
	retval = ehci_init(hcd);
	if (retval)
		return retval;

	retval = ehci_reset(ehci);
=======
	hcd->has_tt = 1;

	retval = ehci_setup(hcd);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	hcd->has_tt = 1;

	retval = ehci_setup(hcd);
>>>>>>> refs/remotes/origin/master
	if (retval)
		return retval;

	/* bursts of unspecified length. */
	writel(0, USB_AHBBURST);
	/* Use the AHB transactor */
<<<<<<< HEAD
<<<<<<< HEAD
	writel(0, USB_AHBMODE);
=======
	writel_relaxed(0x08, USB_AHBMODE);
>>>>>>> refs/remotes/origin/cm-10.0
	/* Disable streaming mode and select host mode */
	writel(0x13, USB_USBMODE);

	ehci_port_power(ehci, 1);
	return 0;
}

static struct hc_driver msm_hc_driver = {
	.description		= hcd_name,
	.product_desc		= "Qualcomm On-Chip EHCI Host Controller",
	.hcd_priv_size		= sizeof(struct ehci_hcd),

	/*
	 * generic hardware linkage
	 */
	.irq			= ehci_irq,
	.flags			= HCD_USB2 | HCD_MEMORY,

	.reset			= ehci_msm_reset,
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
	.clear_tt_buffer_complete = ehci_clear_tt_buffer_complete,

	/*
	 * scheduling support
	 */
	.get_frame_number	= ehci_get_frame,

	/*
	 * root hub support
	 */
	.hub_status_data	= ehci_hub_status_data,
	.hub_control		= ehci_hub_control,
	.relinquish_port	= ehci_relinquish_port,
	.port_handed_over	= ehci_port_handed_over,

	/*
	 * PM support
	 */
	.bus_suspend		= ehci_bus_suspend,
	.bus_resume		= ehci_bus_resume,
};

=======
	writel(0, USB_AHBMODE);
	/* Disable streaming mode and select host mode */
	writel(0x13, USB_USBMODE);

	return 0;
}

>>>>>>> refs/remotes/origin/master
static int ehci_msm_probe(struct platform_device *pdev)
{
	struct usb_hcd *hcd;
	struct resource *res;
<<<<<<< HEAD
=======
	struct usb_phy *phy;
>>>>>>> refs/remotes/origin/master
	int ret;

	dev_dbg(&pdev->dev, "ehci_msm proble\n");

	hcd = usb_create_hcd(&msm_hc_driver, &pdev->dev, dev_name(&pdev->dev));
	if (!hcd) {
		dev_err(&pdev->dev, "Unable to create HCD\n");
		return  -ENOMEM;
	}

	hcd->irq = platform_get_irq(pdev, 0);
	if (hcd->irq < 0) {
		dev_err(&pdev->dev, "Unable to get IRQ resource\n");
		ret = hcd->irq;
		goto put_hcd;
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev, "Unable to get memory resource\n");
		ret = -ENODEV;
		goto put_hcd;
	}

	hcd->rsrc_start = res->start;
	hcd->rsrc_len = resource_size(res);
<<<<<<< HEAD
	hcd->regs = ioremap(hcd->rsrc_start, hcd->rsrc_len);
=======
	hcd->regs = devm_ioremap(&pdev->dev, hcd->rsrc_start, hcd->rsrc_len);
>>>>>>> refs/remotes/origin/master
	if (!hcd->regs) {
		dev_err(&pdev->dev, "ioremap failed\n");
		ret = -ENOMEM;
		goto put_hcd;
	}

	/*
	 * OTG driver takes care of PHY initialization, clock management,
	 * powering up VBUS, mapping of registers address space and power
	 * management.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	otg = otg_get_transceiver();
	if (!otg) {
=======
	phy = usb_get_transceiver();
	if (!phy) {
>>>>>>> refs/remotes/origin/cm-10.0
		dev_err(&pdev->dev, "unable to find transceiver\n");
		ret = -ENODEV;
		goto unmap;
	}

<<<<<<< HEAD
	ret = otg_set_host(otg, &hcd->self);
=======
	ret = otg_set_host(phy->otg, &hcd->self);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret < 0) {
		dev_err(&pdev->dev, "unable to register with transceiver\n");
		goto put_transceiver;
	}

<<<<<<< HEAD
=======
	hcd_to_ehci(hcd)->transceiver = phy;
>>>>>>> refs/remotes/origin/cm-10.0
	device_init_wakeup(&pdev->dev, 1);
	pm_runtime_enable(&pdev->dev);

	return 0;

put_transceiver:
<<<<<<< HEAD
	otg_put_transceiver(otg);
=======
	usb_put_transceiver(phy);
>>>>>>> refs/remotes/origin/cm-10.0
unmap:
	iounmap(hcd->regs);
=======
	if (pdev->dev.of_node)
		phy = devm_usb_get_phy_by_phandle(&pdev->dev, "usb-phy", 0);
	else
		phy = devm_usb_get_phy(&pdev->dev, USB_PHY_TYPE_USB2);

	if (IS_ERR(phy)) {
		dev_err(&pdev->dev, "unable to find transceiver\n");
		ret = -EPROBE_DEFER;
		goto put_hcd;
	}

	ret = otg_set_host(phy->otg, &hcd->self);
	if (ret < 0) {
		dev_err(&pdev->dev, "unable to register with transceiver\n");
		goto put_hcd;
	}

	hcd->phy = phy;
	device_init_wakeup(&pdev->dev, 1);
	/*
	 * OTG device parent of HCD takes care of putting
	 * hardware into low power mode.
	 */
	pm_runtime_no_callbacks(&pdev->dev);
	pm_runtime_enable(&pdev->dev);

	/* FIXME: need to call usb_add_hcd() here? */

	return 0;

>>>>>>> refs/remotes/origin/master
put_hcd:
	usb_put_hcd(hcd);

	return ret;
}

<<<<<<< HEAD
static int __devexit ehci_msm_remove(struct platform_device *pdev)
=======
static int ehci_msm_remove(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	struct usb_hcd *hcd = platform_get_drvdata(pdev);

	device_init_wakeup(&pdev->dev, 0);
	pm_runtime_disable(&pdev->dev);
	pm_runtime_set_suspended(&pdev->dev);

<<<<<<< HEAD
<<<<<<< HEAD
	otg_set_host(otg, NULL);
	otg_put_transceiver(otg);
=======
	hcd_to_ehci(hcd)->transceiver = NULL;
	otg_set_host(phy->otg, NULL);
	usb_put_transceiver(phy);
>>>>>>> refs/remotes/origin/cm-10.0

	usb_put_hcd(hcd);

	return 0;
}

#ifdef CONFIG_PM_RUNTIME
static int ehci_msm_runtime_idle(struct device *dev)
{
	dev_dbg(dev, "ehci runtime idle\n");
	return 0;
}

static int ehci_msm_runtime_suspend(struct device *dev)
{
	dev_dbg(dev, "ehci runtime suspend\n");
	/*
	 * Notify OTG about suspend.  It takes care of
	 * putting the hardware in LPM.
	 */
<<<<<<< HEAD
	return otg_set_suspend(otg, 1);
=======
	return usb_phy_set_suspend(phy, 1);
>>>>>>> refs/remotes/origin/cm-10.0
}

static int ehci_msm_runtime_resume(struct device *dev)
{
	dev_dbg(dev, "ehci runtime resume\n");
<<<<<<< HEAD
	return otg_set_suspend(otg, 0);
=======
	return usb_phy_set_suspend(phy, 0);
>>>>>>> refs/remotes/origin/cm-10.0
}
#endif

#ifdef CONFIG_PM_SLEEP
static int ehci_msm_pm_suspend(struct device *dev)
{
	struct usb_hcd *hcd = dev_get_drvdata(dev);
	bool wakeup = device_may_wakeup(dev);

	dev_dbg(dev, "ehci-msm PM suspend\n");

	if (!hcd->rh_registered)
		return 0;

	/*
	 * EHCI helper function has also the same check before manipulating
	 * port wakeup flags.  We do check here the same condition before
	 * calling the same helper function to avoid bringing hardware
	 * from Low power mode when there is no need for adjusting port
	 * wakeup flags.
	 */
	if (hcd->self.root_hub->do_remote_wakeup && !wakeup) {
		pm_runtime_resume(dev);
		ehci_prepare_ports_for_controller_suspend(hcd_to_ehci(hcd),
				wakeup);
	}

<<<<<<< HEAD
	return otg_set_suspend(otg, 1);
=======
	return usb_phy_set_suspend(phy, 1);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	otg_set_host(hcd->phy->otg, NULL);

	/* FIXME: need to call usb_remove_hcd() here? */

	usb_put_hcd(hcd);

	return 0;
}

#ifdef CONFIG_PM
static int ehci_msm_pm_suspend(struct device *dev)
{
	struct usb_hcd *hcd = dev_get_drvdata(dev);
	bool do_wakeup = device_may_wakeup(dev);

	dev_dbg(dev, "ehci-msm PM suspend\n");

	return ehci_suspend(hcd, do_wakeup);
>>>>>>> refs/remotes/origin/master
}

static int ehci_msm_pm_resume(struct device *dev)
{
	struct usb_hcd *hcd = dev_get_drvdata(dev);

	dev_dbg(dev, "ehci-msm PM resume\n");
<<<<<<< HEAD

	if (!hcd->rh_registered)
		return 0;

	ehci_prepare_ports_for_controller_resume(hcd_to_ehci(hcd));

<<<<<<< HEAD
	return otg_set_suspend(otg, 0);
=======
	return usb_phy_set_suspend(phy, 0);
>>>>>>> refs/remotes/origin/cm-10.0
}
#endif

static const struct dev_pm_ops ehci_msm_dev_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(ehci_msm_pm_suspend, ehci_msm_pm_resume)
	SET_RUNTIME_PM_OPS(ehci_msm_runtime_suspend, ehci_msm_runtime_resume,
				ehci_msm_runtime_idle)
};

static struct platform_driver ehci_msm_driver = {
	.probe	= ehci_msm_probe,
	.remove	= __devexit_p(ehci_msm_remove),
	.driver = {
		   .name = "msm_hsusb_host",
		   .pm = &ehci_msm_dev_pm_ops,
	},
};
=======
	ehci_resume(hcd, false);

	return 0;
}
#else
#define ehci_msm_pm_suspend	NULL
#define ehci_msm_pm_resume	NULL
#endif

static const struct dev_pm_ops ehci_msm_dev_pm_ops = {
	.suspend         = ehci_msm_pm_suspend,
	.resume          = ehci_msm_pm_resume,
};

static struct of_device_id msm_ehci_dt_match[] = {
	{ .compatible = "qcom,ehci-host", },
	{}
};
MODULE_DEVICE_TABLE(of, msm_ehci_dt_match);

static struct platform_driver ehci_msm_driver = {
	.probe	= ehci_msm_probe,
	.remove	= ehci_msm_remove,
	.driver = {
		   .name = "msm_hsusb_host",
		   .pm = &ehci_msm_dev_pm_ops,
		   .of_match_table = msm_ehci_dt_match,
	},
};

static const struct ehci_driver_overrides msm_overrides __initdata = {
	.reset = ehci_msm_reset,
};

static int __init ehci_msm_init(void)
{
	if (usb_disabled())
		return -ENODEV;

	pr_info("%s: " DRIVER_DESC "\n", hcd_name);
	ehci_init_driver(&msm_hc_driver, &msm_overrides);
	return platform_driver_register(&ehci_msm_driver);
}
module_init(ehci_msm_init);

static void __exit ehci_msm_cleanup(void)
{
	platform_driver_unregister(&ehci_msm_driver);
}
module_exit(ehci_msm_cleanup);

MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_ALIAS("platform:msm-ehci");
MODULE_LICENSE("GPL");
>>>>>>> refs/remotes/origin/master
