/*
 * OHCI HCD (Host Controller Driver) for USB.
 *
 * (C) Copyright 1999 Roman Weissgaerber <weissg@vienna.at>
 * (C) Copyright 2000-2002 David Brownell <dbrownell@users.sourceforge.net>
 * (C) Copyright 2002 Hewlett-Packard Company
 *
 * USB Bus Glue for Samsung S3C2410
 *
 * Written by Christopher Hoover <ch@hpl.hp.com>
 * Based on fragments of previous driver by Russell King et al.
 *
 * Modified for S3C2410 from ohci-sa1111.c, ohci-omap.c and ohci-lh7a40.c
 *	by Ben Dooks, <ben@simtec.co.uk>
 *	Copyright (C) 2004 Simtec Electronics
 *
 * Thanks to basprog@mail.ru for updates to newer kernels
 *
 * This file is licenced under the GPL.
*/

<<<<<<< HEAD
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <plat/usb-control.h>
=======
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/platform_data/usb-ohci-s3c2410.h>
#include <linux/usb.h>
#include <linux/usb/hcd.h>

#include "ohci.h"

>>>>>>> refs/remotes/origin/master

#define valid_port(idx) ((idx) == 1 || (idx) == 2)

/* clock device associated with the hcd */

<<<<<<< HEAD
=======

#define DRIVER_DESC "OHCI S3C2410 driver"

static const char hcd_name[] = "ohci-s3c2410";

>>>>>>> refs/remotes/origin/master
static struct clk *clk;
static struct clk *usb_clk;

/* forward definitions */

<<<<<<< HEAD
=======
static int (*orig_ohci_hub_control)(struct usb_hcd  *hcd, u16 typeReq,
			u16 wValue, u16 wIndex, char *buf, u16 wLength);
static int (*orig_ohci_hub_status_data)(struct usb_hcd *hcd, char *buf);

>>>>>>> refs/remotes/origin/master
static void s3c2410_hcd_oc(struct s3c2410_hcd_info *info, int port_oc);

/* conversion functions */

static struct s3c2410_hcd_info *to_s3c2410_info(struct usb_hcd *hcd)
{
<<<<<<< HEAD
	return hcd->self.controller->platform_data;
=======
	return dev_get_platdata(hcd->self.controller);
>>>>>>> refs/remotes/origin/master
}

static void s3c2410_start_hc(struct platform_device *dev, struct usb_hcd *hcd)
{
<<<<<<< HEAD
	struct s3c2410_hcd_info *info = dev->dev.platform_data;

	dev_dbg(&dev->dev, "s3c2410_start_hc:\n");

	clk_enable(usb_clk);
	mdelay(2);			/* let the bus clock stabilise */

	clk_enable(clk);
=======
	struct s3c2410_hcd_info *info = dev_get_platdata(&dev->dev);

	dev_dbg(&dev->dev, "s3c2410_start_hc:\n");

	clk_prepare_enable(usb_clk);
	mdelay(2);			/* let the bus clock stabilise */

	clk_prepare_enable(clk);
>>>>>>> refs/remotes/origin/master

	if (info != NULL) {
		info->hcd	= hcd;
		info->report_oc = s3c2410_hcd_oc;

		if (info->enable_oc != NULL)
			(info->enable_oc)(info, 1);
	}
}

static void s3c2410_stop_hc(struct platform_device *dev)
{
<<<<<<< HEAD
	struct s3c2410_hcd_info *info = dev->dev.platform_data;
=======
	struct s3c2410_hcd_info *info = dev_get_platdata(&dev->dev);
>>>>>>> refs/remotes/origin/master

	dev_dbg(&dev->dev, "s3c2410_stop_hc:\n");

	if (info != NULL) {
		info->report_oc = NULL;
		info->hcd	= NULL;

		if (info->enable_oc != NULL)
			(info->enable_oc)(info, 0);
	}

<<<<<<< HEAD
	clk_disable(clk);
	clk_disable(usb_clk);
=======
	clk_disable_unprepare(clk);
	clk_disable_unprepare(usb_clk);
>>>>>>> refs/remotes/origin/master
}

/* ohci_s3c2410_hub_status_data
 *
 * update the status data from the hub with anything that
 * has been detected by our system
*/

static int
ohci_s3c2410_hub_status_data(struct usb_hcd *hcd, char *buf)
{
	struct s3c2410_hcd_info *info = to_s3c2410_info(hcd);
	struct s3c2410_hcd_port *port;
	int orig;
	int portno;

<<<<<<< HEAD
	orig  = ohci_hub_status_data(hcd, buf);
=======
	orig = orig_ohci_hub_status_data(hcd, buf);
>>>>>>> refs/remotes/origin/master

	if (info == NULL)
		return orig;

	port = &info->port[0];

	/* mark any changed port as changed */

	for (portno = 0; portno < 2; port++, portno++) {
		if (port->oc_changed == 1 &&
		    port->flags & S3C_HCDFLG_USED) {
			dev_dbg(hcd->self.controller,
				"oc change on port %d\n", portno);

			if (orig < 1)
				orig = 1;

			buf[0] |= 1<<(portno+1);
		}
	}

	return orig;
}

/* s3c2410_usb_set_power
 *
 * configure the power on a port, by calling the platform device
 * routine registered with the platform device
*/

static void s3c2410_usb_set_power(struct s3c2410_hcd_info *info,
				  int port, int to)
{
	if (info == NULL)
		return;

	if (info->power_control != NULL) {
		info->port[port-1].power = to;
		(info->power_control)(port-1, to);
	}
}

/* ohci_s3c2410_hub_control
 *
 * look at control requests to the hub, and see if we need
 * to take any action or over-ride the results from the
 * request.
*/

static int ohci_s3c2410_hub_control(
	struct usb_hcd	*hcd,
	u16		typeReq,
	u16		wValue,
	u16		wIndex,
	char		*buf,
	u16		wLength)
{
	struct s3c2410_hcd_info *info = to_s3c2410_info(hcd);
	struct usb_hub_descriptor *desc;
	int ret = -EINVAL;
	u32 *data = (u32 *)buf;

	dev_dbg(hcd->self.controller,
		"s3c2410_hub_control(%p,0x%04x,0x%04x,0x%04x,%p,%04x)\n",
		hcd, typeReq, wValue, wIndex, buf, wLength);

	/* if we are only an humble host without any special capabilities
	 * process the request straight away and exit */

	if (info == NULL) {
<<<<<<< HEAD
		ret = ohci_hub_control(hcd, typeReq, wValue,
=======
		ret = orig_ohci_hub_control(hcd, typeReq, wValue,
>>>>>>> refs/remotes/origin/master
				       wIndex, buf, wLength);
		goto out;
	}

	/* check the request to see if it needs handling */

	switch (typeReq) {
	case SetPortFeature:
		if (wValue == USB_PORT_FEAT_POWER) {
			dev_dbg(hcd->self.controller, "SetPortFeat: POWER\n");
			s3c2410_usb_set_power(info, wIndex, 1);
			goto out;
		}
		break;

	case ClearPortFeature:
		switch (wValue) {
		case USB_PORT_FEAT_C_OVER_CURRENT:
			dev_dbg(hcd->self.controller,
				"ClearPortFeature: C_OVER_CURRENT\n");

			if (valid_port(wIndex)) {
				info->port[wIndex-1].oc_changed = 0;
				info->port[wIndex-1].oc_status = 0;
			}

			goto out;

		case USB_PORT_FEAT_OVER_CURRENT:
			dev_dbg(hcd->self.controller,
				"ClearPortFeature: OVER_CURRENT\n");

			if (valid_port(wIndex))
				info->port[wIndex-1].oc_status = 0;

			goto out;

		case USB_PORT_FEAT_POWER:
			dev_dbg(hcd->self.controller,
				"ClearPortFeature: POWER\n");

			if (valid_port(wIndex)) {
				s3c2410_usb_set_power(info, wIndex, 0);
				return 0;
			}
		}
		break;
	}

<<<<<<< HEAD
	ret = ohci_hub_control(hcd, typeReq, wValue, wIndex, buf, wLength);
=======
	ret = orig_ohci_hub_control(hcd, typeReq, wValue, wIndex, buf, wLength);
>>>>>>> refs/remotes/origin/master
	if (ret)
		goto out;

	switch (typeReq) {
	case GetHubDescriptor:

		/* update the hub's descriptor */

		desc = (struct usb_hub_descriptor *)buf;

		if (info->power_control == NULL)
			return ret;

		dev_dbg(hcd->self.controller, "wHubCharacteristics 0x%04x\n",
			desc->wHubCharacteristics);

		/* remove the old configurations for power-switching, and
		 * over-current protection, and insert our new configuration
		 */

		desc->wHubCharacteristics &= ~cpu_to_le16(HUB_CHAR_LPSM);
		desc->wHubCharacteristics |= cpu_to_le16(0x0001);

		if (info->enable_oc) {
			desc->wHubCharacteristics &= ~cpu_to_le16(
				HUB_CHAR_OCPM);
			desc->wHubCharacteristics |=  cpu_to_le16(
				0x0008 |
				0x0001);
		}

		dev_dbg(hcd->self.controller, "wHubCharacteristics after 0x%04x\n",
			desc->wHubCharacteristics);

		return ret;

	case GetPortStatus:
		/* check port status */

		dev_dbg(hcd->self.controller, "GetPortStatus(%d)\n", wIndex);

		if (valid_port(wIndex)) {
			if (info->port[wIndex-1].oc_changed)
				*data |= cpu_to_le32(RH_PS_OCIC);

			if (info->port[wIndex-1].oc_status)
				*data |= cpu_to_le32(RH_PS_POCI);
		}
	}

 out:
	return ret;
}

/* s3c2410_hcd_oc
 *
 * handle an over-current report
*/

static void s3c2410_hcd_oc(struct s3c2410_hcd_info *info, int port_oc)
{
	struct s3c2410_hcd_port *port;
	struct usb_hcd *hcd;
	unsigned long flags;
	int portno;

	if (info == NULL)
		return;

	port = &info->port[0];
	hcd = info->hcd;

	local_irq_save(flags);

	for (portno = 0; portno < 2; port++, portno++) {
		if (port_oc & (1<<portno) &&
		    port->flags & S3C_HCDFLG_USED) {
			port->oc_status = 1;
			port->oc_changed = 1;

			/* ok, once over-current is detected,
			   the port needs to be powered down */
			s3c2410_usb_set_power(info, portno+1, 0);
		}
	}

	local_irq_restore(flags);
}

/* may be called without controller electrically present */
/* may be called with controller, bus, and devices active */

/*
 * usb_hcd_s3c2410_remove - shutdown processing for HCD
 * @dev: USB Host Controller being removed
 * Context: !in_interrupt()
 *
 * Reverses the effect of usb_hcd_3c2410_probe(), first invoking
 * the HCD's stop() method.  It is always called from a thread
 * context, normally "rmmod", "apmd", or something similar.
 *
*/

static void
usb_hcd_s3c2410_remove(struct usb_hcd *hcd, struct platform_device *dev)
{
	usb_remove_hcd(hcd);
	s3c2410_stop_hc(dev);
<<<<<<< HEAD
	iounmap(hcd->regs);
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
=======
>>>>>>> refs/remotes/origin/master
	usb_put_hcd(hcd);
}

/**
 * usb_hcd_s3c2410_probe - initialize S3C2410-based HCDs
 * Context: !in_interrupt()
 *
 * Allocates basic resources for this USB host controller, and
 * then invokes the start() method for the HCD associated with it
 * through the hotplug entry's driver_data.
 *
 */
static int usb_hcd_s3c2410_probe(const struct hc_driver *driver,
				  struct platform_device *dev)
{
	struct usb_hcd *hcd = NULL;
<<<<<<< HEAD
	int retval;

	s3c2410_usb_set_power(dev->dev.platform_data, 1, 1);
	s3c2410_usb_set_power(dev->dev.platform_data, 2, 1);
=======
	struct s3c2410_hcd_info *info = dev_get_platdata(&dev->dev);
	int retval;

	s3c2410_usb_set_power(info, 1, 1);
	s3c2410_usb_set_power(info, 2, 1);
>>>>>>> refs/remotes/origin/master

	hcd = usb_create_hcd(driver, &dev->dev, "s3c24xx");
	if (hcd == NULL)
		return -ENOMEM;

	hcd->rsrc_start = dev->resource[0].start;
	hcd->rsrc_len	= resource_size(&dev->resource[0]);

<<<<<<< HEAD
	if (!request_mem_region(hcd->rsrc_start, hcd->rsrc_len, hcd_name)) {
		dev_err(&dev->dev, "request_mem_region failed\n");
		retval = -EBUSY;
		goto err_put;
	}

	clk = clk_get(&dev->dev, "usb-host");
	if (IS_ERR(clk)) {
		dev_err(&dev->dev, "cannot get usb-host clock\n");
		retval = PTR_ERR(clk);
		goto err_mem;
	}

	usb_clk = clk_get(&dev->dev, "usb-bus-host");
	if (IS_ERR(usb_clk)) {
		dev_err(&dev->dev, "cannot get usb-bus-host clock\n");
		retval = PTR_ERR(usb_clk);
		goto err_clk;
=======
	hcd->regs = devm_ioremap_resource(&dev->dev, &dev->resource[0]);
	if (IS_ERR(hcd->regs)) {
		retval = PTR_ERR(hcd->regs);
		goto err_put;
	}

	clk = devm_clk_get(&dev->dev, "usb-host");
	if (IS_ERR(clk)) {
		dev_err(&dev->dev, "cannot get usb-host clock\n");
		retval = PTR_ERR(clk);
		goto err_put;
	}

	usb_clk = devm_clk_get(&dev->dev, "usb-bus-host");
	if (IS_ERR(usb_clk)) {
		dev_err(&dev->dev, "cannot get usb-bus-host clock\n");
		retval = PTR_ERR(usb_clk);
		goto err_put;
>>>>>>> refs/remotes/origin/master
	}

	s3c2410_start_hc(dev, hcd);

<<<<<<< HEAD
	hcd->regs = ioremap(hcd->rsrc_start, hcd->rsrc_len);
	if (!hcd->regs) {
		dev_err(&dev->dev, "ioremap failed\n");
		retval = -ENOMEM;
		goto err_ioremap;
	}

	ohci_hcd_init(hcd_to_ohci(hcd));

<<<<<<< HEAD
	retval = usb_add_hcd(hcd, dev->resource[1].start, IRQF_DISABLED);
=======
	retval = usb_add_hcd(hcd, dev->resource[1].start, 0);
>>>>>>> refs/remotes/origin/cm-10.0
	if (retval != 0)
		goto err_ioremap;

=======
	retval = usb_add_hcd(hcd, dev->resource[1].start, 0);
	if (retval != 0)
		goto err_ioremap;

	device_wakeup_enable(hcd->self.controller);
>>>>>>> refs/remotes/origin/master
	return 0;

 err_ioremap:
	s3c2410_stop_hc(dev);
<<<<<<< HEAD
	iounmap(hcd->regs);
	clk_put(usb_clk);

 err_clk:
	clk_put(clk);

 err_mem:
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
=======
>>>>>>> refs/remotes/origin/master

 err_put:
	usb_put_hcd(hcd);
	return retval;
}

/*-------------------------------------------------------------------------*/

<<<<<<< HEAD
static int
ohci_s3c2410_start(struct usb_hcd *hcd)
{
	struct ohci_hcd	*ohci = hcd_to_ohci(hcd);
	int ret;

	ret = ohci_init(ohci);
	if (ret < 0)
		return ret;

	ret = ohci_run(ohci);
	if (ret < 0) {
		err("can't start %s", hcd->self.bus_name);
		ohci_stop(hcd);
		return ret;
	}

	return 0;
}


static const struct hc_driver ohci_s3c2410_hc_driver = {
	.description =		hcd_name,
	.product_desc =		"S3C24XX OHCI",
	.hcd_priv_size =	sizeof(struct ohci_hcd),

	/*
	 * generic hardware linkage
	 */
	.irq =			ohci_irq,
	.flags =		HCD_USB11 | HCD_MEMORY,

	/*
	 * basic lifecycle operations
	 */
	.start =		ohci_s3c2410_start,
	.stop =			ohci_stop,
	.shutdown =		ohci_shutdown,

	/*
	 * managing i/o requests and associated device resources
	 */
	.urb_enqueue =		ohci_urb_enqueue,
	.urb_dequeue =		ohci_urb_dequeue,
	.endpoint_disable =	ohci_endpoint_disable,

	/*
	 * scheduling support
	 */
	.get_frame_number =	ohci_get_frame,

	/*
	 * root hub support
	 */
	.hub_status_data =	ohci_s3c2410_hub_status_data,
	.hub_control =		ohci_s3c2410_hub_control,
#ifdef	CONFIG_PM
	.bus_suspend =		ohci_bus_suspend,
	.bus_resume =		ohci_bus_resume,
#endif
	.start_port_reset =	ohci_start_port_reset,
};

/* device driver */

static int __devinit ohci_hcd_s3c2410_drv_probe(struct platform_device *pdev)
=======
static struct hc_driver __read_mostly ohci_s3c2410_hc_driver;

static int ohci_hcd_s3c2410_drv_probe(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	return usb_hcd_s3c2410_probe(&ohci_s3c2410_hc_driver, pdev);
}

<<<<<<< HEAD
static int __devexit ohci_hcd_s3c2410_drv_remove(struct platform_device *pdev)
=======
static int ohci_hcd_s3c2410_drv_remove(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	struct usb_hcd *hcd = platform_get_drvdata(pdev);

	usb_hcd_s3c2410_remove(hcd, pdev);
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_PM
static int ohci_hcd_s3c2410_drv_suspend(struct device *dev)
{
	struct usb_hcd *hcd = dev_get_drvdata(dev);
<<<<<<< HEAD
	struct ohci_hcd *ohci = hcd_to_ohci(hcd);
	struct platform_device *pdev = to_platform_device(dev);
	unsigned long flags;
	int rc = 0;

	/*
	 * Root hub was already suspended. Disable irq emission and
	 * mark HW unaccessible, bail out if RH has been resumed. Use
	 * the spinlock to properly synchronize with possible pending
	 * RH suspend or resume activity.
	 */
	spin_lock_irqsave(&ohci->lock, flags);
	if (ohci->rh_state != OHCI_RH_SUSPENDED) {
		rc = -EINVAL;
		goto bail;
	}

	clear_bit(HCD_FLAG_HW_ACCESSIBLE, &hcd->flags);

	s3c2410_stop_hc(pdev);
bail:
	spin_unlock_irqrestore(&ohci->lock, flags);
=======
	struct platform_device *pdev = to_platform_device(dev);
	bool do_wakeup = device_may_wakeup(dev);
	int rc = 0;

	rc = ohci_suspend(hcd, do_wakeup);
	if (rc)
		return rc;

	s3c2410_stop_hc(pdev);
>>>>>>> refs/remotes/origin/master

	return rc;
}

static int ohci_hcd_s3c2410_drv_resume(struct device *dev)
{
	struct usb_hcd *hcd = dev_get_drvdata(dev);
	struct platform_device *pdev = to_platform_device(dev);

	s3c2410_start_hc(pdev, hcd);

<<<<<<< HEAD
	set_bit(HCD_FLAG_HW_ACCESSIBLE, &hcd->flags);
	ohci_finish_controller_resume(hcd);
=======
	ohci_resume(hcd, false);
>>>>>>> refs/remotes/origin/master

	return 0;
}
#else
#define ohci_hcd_s3c2410_drv_suspend	NULL
#define ohci_hcd_s3c2410_drv_resume	NULL
#endif

static const struct dev_pm_ops ohci_hcd_s3c2410_pm_ops = {
	.suspend	= ohci_hcd_s3c2410_drv_suspend,
	.resume		= ohci_hcd_s3c2410_drv_resume,
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
static struct platform_driver ohci_hcd_s3c2410_driver = {
	.probe		= ohci_hcd_s3c2410_drv_probe,
	.remove		= __devexit_p(ohci_hcd_s3c2410_drv_remove),
	.shutdown	= usb_hcd_platform_shutdown,
<<<<<<< HEAD
	/*.suspend	= ohci_hcd_s3c2410_drv_suspend, */
	/*.resume	= ohci_hcd_s3c2410_drv_resume, */
	.driver		= {
		.owner	= THIS_MODULE,
		.name	= "s3c2410-ohci",
=======
=======
static struct platform_driver ohci_hcd_s3c2410_driver = {
	.probe		= ohci_hcd_s3c2410_drv_probe,
	.remove		= ohci_hcd_s3c2410_drv_remove,
	.shutdown	= usb_hcd_platform_shutdown,
>>>>>>> refs/remotes/origin/master
	.driver		= {
		.owner	= THIS_MODULE,
		.name	= "s3c2410-ohci",
		.pm	= &ohci_hcd_s3c2410_pm_ops,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	},
};

=======
	},
};

static int __init ohci_s3c2410_init(void)
{
	if (usb_disabled())
		return -ENODEV;

	pr_info("%s: " DRIVER_DESC "\n", hcd_name);
	ohci_init_driver(&ohci_s3c2410_hc_driver, NULL);

	/*
	 * The Samsung HW has some unusual quirks, which require
	 * Sumsung-specific workarounds. We override certain hc_driver
	 * functions here to achieve that. We explicitly do not enhance
	 * ohci_driver_overrides to allow this more easily, since this
	 * is an unusual case, and we don't want to encourage others to
	 * override these functions by making it too easy.
	 */

	orig_ohci_hub_control = ohci_s3c2410_hc_driver.hub_control;
	orig_ohci_hub_status_data = ohci_s3c2410_hc_driver.hub_status_data;

	ohci_s3c2410_hc_driver.hub_status_data	= ohci_s3c2410_hub_status_data;
	ohci_s3c2410_hc_driver.hub_control	= ohci_s3c2410_hub_control;

	return platform_driver_register(&ohci_hcd_s3c2410_driver);
}
module_init(ohci_s3c2410_init);

static void __exit ohci_s3c2410_cleanup(void)
{
	platform_driver_unregister(&ohci_hcd_s3c2410_driver);
}
module_exit(ohci_s3c2410_cleanup);

MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");
>>>>>>> refs/remotes/origin/master
MODULE_ALIAS("platform:s3c2410-ohci");
