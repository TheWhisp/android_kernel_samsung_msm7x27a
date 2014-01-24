/*
* OHCI HCD (Host Controller Driver) for USB.
*
* Copyright (C) 2010 ST Microelectronics.
* Deepak Sikri<deepak.sikri@st.com>
*
* Based on various ohci-*.c drivers
*
* This file is licensed under the terms of the GNU General Public
* License version 2. This program is licensed "as is" without any
* warranty of any kind, whether express or implied.
*/

<<<<<<< HEAD
#include <linux/signal.h>
#include <linux/platform_device.h>
#include <linux/clk.h>

struct spear_ohci {
	struct ohci_hcd ohci;
	struct clk *clk;
};

#define to_spear_ohci(hcd)	(struct spear_ohci *)hcd_to_ohci(hcd)

static void spear_start_ohci(struct spear_ohci *ohci)
{
	clk_enable(ohci->clk);
}

static void spear_stop_ohci(struct spear_ohci *ohci)
{
	clk_disable(ohci->clk);
}

static int __devinit ohci_spear_start(struct usb_hcd *hcd)
{
	struct ohci_hcd *ohci = hcd_to_ohci(hcd);
	int ret;

	ret = ohci_init(ohci);
	if (ret < 0)
		return ret;
	ohci->regs = hcd->regs;

	ret = ohci_run(ohci);
	if (ret < 0) {
		dev_err(hcd->self.controller, "can't start\n");
		ohci_stop(hcd);
		return ret;
	}

	create_debug_files(ohci);

#ifdef DEBUG
	ohci_dump(ohci, 1);
#endif
	return 0;
}

static const struct hc_driver ohci_spear_hc_driver = {
	.description		= hcd_name,
	.product_desc		= "SPEAr OHCI",
	.hcd_priv_size		= sizeof(struct spear_ohci),

	/* generic hardware linkage */
	.irq			= ohci_irq,
	.flags			= HCD_USB11 | HCD_MEMORY,

	/* basic lifecycle operations */
	.start			= ohci_spear_start,
	.stop			= ohci_stop,
	.shutdown		= ohci_shutdown,
#ifdef	CONFIG_PM
	.bus_suspend		= ohci_bus_suspend,
	.bus_resume		= ohci_bus_resume,
#endif

	/* managing i/o requests and associated device resources */
	.urb_enqueue		= ohci_urb_enqueue,
	.urb_dequeue		= ohci_urb_dequeue,
	.endpoint_disable	= ohci_endpoint_disable,

	/* scheduling support */
	.get_frame_number	= ohci_get_frame,

	/* root hub support */
	.hub_status_data	= ohci_hub_status_data,
	.hub_control		= ohci_hub_control,

	.start_port_reset	= ohci_start_port_reset,
};
=======
#include <linux/clk.h>
#include <linux/dma-mapping.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/signal.h>
#include <linux/usb.h>
#include <linux/usb/hcd.h>

#include "ohci.h"

#define DRIVER_DESC "OHCI SPEAr driver"

static const char hcd_name[] = "SPEAr-ohci";
struct spear_ohci {
	struct clk *clk;
};

#define to_spear_ohci(hcd)     (struct spear_ohci *)(hcd_to_ohci(hcd)->priv)

static struct hc_driver __read_mostly ohci_spear_hc_driver;
>>>>>>> refs/remotes/origin/master

static int spear_ohci_hcd_drv_probe(struct platform_device *pdev)
{
	const struct hc_driver *driver = &ohci_spear_hc_driver;
<<<<<<< HEAD
	struct usb_hcd *hcd = NULL;
	struct clk *usbh_clk;
	struct spear_ohci *ohci_p;
	struct resource *res;
	int retval, irq;
	int *pdata = pdev->dev.platform_data;
	char clk_name[20] = "usbh_clk";

	if (pdata == NULL)
		return -EFAULT;
=======
	struct ohci_hcd *ohci;
	struct usb_hcd *hcd = NULL;
	struct clk *usbh_clk;
	struct spear_ohci *sohci_p;
	struct resource *res;
	int retval, irq;
>>>>>>> refs/remotes/origin/master

	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		retval = irq;
<<<<<<< HEAD
		goto fail_irq_get;
	}

	if (*pdata >= 0)
		sprintf(clk_name, "usbh.%01d_clk", *pdata);

	usbh_clk = clk_get(NULL, clk_name);
	if (IS_ERR(usbh_clk)) {
		dev_err(&pdev->dev, "Error getting interface clock\n");
		retval = PTR_ERR(usbh_clk);
		goto fail_get_usbh_clk;
=======
		goto fail;
	}

	/*
	 * Right now device-tree probed devices don't get dma_mask set.
	 * Since shared usb code relies on it, set it here for now.
	 * Once we have dma capability bindings this can go away.
	 */
	retval = dma_coerce_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(32));
	if (retval)
		goto fail;

	usbh_clk = devm_clk_get(&pdev->dev, NULL);
	if (IS_ERR(usbh_clk)) {
		dev_err(&pdev->dev, "Error getting interface clock\n");
		retval = PTR_ERR(usbh_clk);
		goto fail;
>>>>>>> refs/remotes/origin/master
	}

	hcd = usb_create_hcd(driver, &pdev->dev, dev_name(&pdev->dev));
	if (!hcd) {
		retval = -ENOMEM;
<<<<<<< HEAD
		goto fail_create_hcd;
=======
		goto fail;
>>>>>>> refs/remotes/origin/master
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		retval = -ENODEV;
<<<<<<< HEAD
		goto fail_request_resource;
=======
		goto err_put_hcd;
>>>>>>> refs/remotes/origin/master
	}

	hcd->rsrc_start = pdev->resource[0].start;
	hcd->rsrc_len = resource_size(res);
<<<<<<< HEAD
	if (!request_mem_region(hcd->rsrc_start, hcd->rsrc_len, hcd_name)) {
		dev_dbg(&pdev->dev, "request_mem_region failed\n");
		retval = -EBUSY;
		goto fail_request_resource;
	}

	hcd->regs = ioremap(hcd->rsrc_start, hcd->rsrc_len);
	if (!hcd->regs) {
		dev_dbg(&pdev->dev, "ioremap failed\n");
		retval = -ENOMEM;
		goto fail_ioremap;
	}

	ohci_p = (struct spear_ohci *)hcd_to_ohci(hcd);
	ohci_p->clk = usbh_clk;
	spear_start_ohci(ohci_p);
	ohci_hcd_init(hcd_to_ohci(hcd));

<<<<<<< HEAD
	retval = usb_add_hcd(hcd, platform_get_irq(pdev, 0), IRQF_DISABLED);
=======
	retval = usb_add_hcd(hcd, platform_get_irq(pdev, 0), 0);
>>>>>>> refs/remotes/origin/cm-10.0
	if (retval == 0)
		return retval;

	spear_stop_ohci(ohci_p);
	iounmap(hcd->regs);
fail_ioremap:
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
fail_request_resource:
	usb_put_hcd(hcd);
fail_create_hcd:
	clk_put(usbh_clk);
fail_get_usbh_clk:
fail_irq_get:
=======

	hcd->regs = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(hcd->regs)) {
		retval = PTR_ERR(hcd->regs);
		goto err_put_hcd;
	}

	sohci_p = to_spear_ohci(hcd);
	sohci_p->clk = usbh_clk;

	clk_prepare_enable(sohci_p->clk);

	ohci = hcd_to_ohci(hcd);

	retval = usb_add_hcd(hcd, platform_get_irq(pdev, 0), 0);
	if (retval == 0) {
		device_wakeup_enable(hcd->self.controller);
		return retval;
	}

	clk_disable_unprepare(sohci_p->clk);
err_put_hcd:
	usb_put_hcd(hcd);
fail:
>>>>>>> refs/remotes/origin/master
	dev_err(&pdev->dev, "init fail, %d\n", retval);

	return retval;
}

static int spear_ohci_hcd_drv_remove(struct platform_device *pdev)
{
	struct usb_hcd *hcd = platform_get_drvdata(pdev);
<<<<<<< HEAD
	struct spear_ohci *ohci_p = to_spear_ohci(hcd);

	usb_remove_hcd(hcd);
	if (ohci_p->clk)
		spear_stop_ohci(ohci_p);

	iounmap(hcd->regs);
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
	usb_put_hcd(hcd);

	if (ohci_p->clk)
		clk_put(ohci_p->clk);
	platform_set_drvdata(pdev, NULL);
=======
	struct spear_ohci *sohci_p = to_spear_ohci(hcd);

	usb_remove_hcd(hcd);
	if (sohci_p->clk)
		clk_disable_unprepare(sohci_p->clk);

	usb_put_hcd(hcd);
>>>>>>> refs/remotes/origin/master
	return 0;
}

#if defined(CONFIG_PM)
<<<<<<< HEAD
static int spear_ohci_hcd_drv_suspend(struct platform_device *dev,
		pm_message_t message)
{
	struct usb_hcd *hcd = platform_get_drvdata(dev);
	struct ohci_hcd	*ohci = hcd_to_ohci(hcd);
	struct spear_ohci *ohci_p = to_spear_ohci(hcd);
=======
static int spear_ohci_hcd_drv_suspend(struct platform_device *pdev,
		pm_message_t message)
{
	struct usb_hcd *hcd = platform_get_drvdata(pdev);
	struct ohci_hcd	*ohci = hcd_to_ohci(hcd);
	struct spear_ohci *sohci_p = to_spear_ohci(hcd);
	bool do_wakeup = device_may_wakeup(&pdev->dev);
	int ret;
>>>>>>> refs/remotes/origin/master

	if (time_before(jiffies, ohci->next_statechange))
		msleep(5);
	ohci->next_statechange = jiffies;

<<<<<<< HEAD
	spear_stop_ohci(ohci_p);
<<<<<<< HEAD
	ohci_to_hcd(ohci)->state = HC_STATE_SUSPENDED;
=======
>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
=======
	ret = ohci_suspend(hcd, do_wakeup);
	if (ret)
		return ret;

	clk_disable_unprepare(sohci_p->clk);

	return ret;
>>>>>>> refs/remotes/origin/master
}

static int spear_ohci_hcd_drv_resume(struct platform_device *dev)
{
	struct usb_hcd *hcd = platform_get_drvdata(dev);
	struct ohci_hcd	*ohci = hcd_to_ohci(hcd);
<<<<<<< HEAD
	struct spear_ohci *ohci_p = to_spear_ohci(hcd);
=======
	struct spear_ohci *sohci_p = to_spear_ohci(hcd);
>>>>>>> refs/remotes/origin/master

	if (time_before(jiffies, ohci->next_statechange))
		msleep(5);
	ohci->next_statechange = jiffies;

<<<<<<< HEAD
	spear_start_ohci(ohci_p);
	ohci_finish_controller_resume(hcd);
=======
	clk_prepare_enable(sohci_p->clk);
	ohci_resume(hcd, false);
>>>>>>> refs/remotes/origin/master
	return 0;
}
#endif

<<<<<<< HEAD
=======
static struct of_device_id spear_ohci_id_table[] = {
	{ .compatible = "st,spear600-ohci", },
	{ },
};

>>>>>>> refs/remotes/origin/master
/* Driver definition to register with the platform bus */
static struct platform_driver spear_ohci_hcd_driver = {
	.probe =	spear_ohci_hcd_drv_probe,
	.remove =	spear_ohci_hcd_drv_remove,
#ifdef CONFIG_PM
	.suspend =	spear_ohci_hcd_drv_suspend,
	.resume =	spear_ohci_hcd_drv_resume,
#endif
	.driver = {
		.owner = THIS_MODULE,
		.name = "spear-ohci",
<<<<<<< HEAD
	},
};

=======
		.of_match_table = spear_ohci_id_table,
	},
};

static const struct ohci_driver_overrides spear_overrides __initconst = {
	.extra_priv_size = sizeof(struct spear_ohci),
};
static int __init ohci_spear_init(void)
{
	if (usb_disabled())
		return -ENODEV;

	pr_info("%s: " DRIVER_DESC "\n", hcd_name);

	ohci_init_driver(&ohci_spear_hc_driver, &spear_overrides);
	return platform_driver_register(&spear_ohci_hcd_driver);
}
module_init(ohci_spear_init);

static void __exit ohci_spear_cleanup(void)
{
	platform_driver_unregister(&spear_ohci_hcd_driver);
}
module_exit(ohci_spear_cleanup);

MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_AUTHOR("Deepak Sikri");
MODULE_LICENSE("GPL v2");
>>>>>>> refs/remotes/origin/master
MODULE_ALIAS("platform:spear-ohci");
