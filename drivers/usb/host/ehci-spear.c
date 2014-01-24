/*
<<<<<<< HEAD
* Driver for EHCI HCD on SPEAR SOC
=======
* Driver for EHCI HCD on SPEAr SOC
>>>>>>> refs/remotes/origin/master
*
* Copyright (C) 2010 ST Micro Electronics,
* Deepak Sikri <deepak.sikri@st.com>
*
* Based on various ehci-*.c drivers
*
* This file is subject to the terms and conditions of the GNU General Public
* License. See the file COPYING in the main directory of this archive for
* more details.
*/

<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/platform_device.h>
#include <linux/clk.h>
=======
#include <linux/clk.h>
#include <linux/jiffies.h>
#include <linux/platform_device.h>
#include <linux/pm.h>
>>>>>>> refs/remotes/origin/cm-10.0

struct spear_ehci {
	struct ehci_hcd ehci;
	struct clk *clk;
};

#define to_spear_ehci(hcd)	(struct spear_ehci *)hcd_to_ehci(hcd)

static void spear_start_ehci(struct spear_ehci *ehci)
{
	clk_enable(ehci->clk);
}

static void spear_stop_ehci(struct spear_ehci *ehci)
{
	clk_disable(ehci->clk);
}

static int ehci_spear_setup(struct usb_hcd *hcd)
{
	struct ehci_hcd *ehci = hcd_to_ehci(hcd);
	int retval = 0;

	/* registers start at offset 0x0 */
	ehci->caps = hcd->regs;
	ehci->regs = hcd->regs + HC_LENGTH(ehci, ehci_readl(ehci,
				&ehci->caps->hc_capbase));
	/* cache this readonly data; minimize chip reads */
	ehci->hcs_params = ehci_readl(ehci, &ehci->caps->hcs_params);
	retval = ehci_halt(ehci);
	if (retval)
		return retval;

	retval = ehci_init(hcd);
	if (retval)
		return retval;

	ehci_reset(ehci);
	ehci_port_power(ehci, 0);

	return retval;
}

static const struct hc_driver ehci_spear_hc_driver = {
	.description			= hcd_name,
	.product_desc			= "SPEAr EHCI",
	.hcd_priv_size			= sizeof(struct spear_ehci),

	/* generic hardware linkage */
	.irq				= ehci_irq,
	.flags				= HCD_MEMORY | HCD_USB2,

	/* basic lifecycle operations */
	.reset				= ehci_spear_setup,
	.start				= ehci_run,
	.stop				= ehci_stop,
	.shutdown			= ehci_shutdown,

	/* managing i/o requests and associated device resources */
	.urb_enqueue			= ehci_urb_enqueue,
	.urb_dequeue			= ehci_urb_dequeue,
	.endpoint_disable		= ehci_endpoint_disable,
	.endpoint_reset			= ehci_endpoint_reset,

	/* scheduling support */
	.get_frame_number		= ehci_get_frame,

	/* root hub support */
	.hub_status_data		= ehci_hub_status_data,
	.hub_control			= ehci_hub_control,
	.bus_suspend			= ehci_bus_suspend,
	.bus_resume			= ehci_bus_resume,
	.relinquish_port		= ehci_relinquish_port,
	.port_handed_over		= ehci_port_handed_over,
	.clear_tt_buffer_complete	= ehci_clear_tt_buffer_complete,
};

<<<<<<< HEAD
=======
#ifdef CONFIG_PM
static int ehci_spear_drv_suspend(struct device *dev)
{
	struct usb_hcd *hcd = dev_get_drvdata(dev);
	struct ehci_hcd *ehci = hcd_to_ehci(hcd);
	unsigned long flags;
	int rc = 0;

	if (time_before(jiffies, ehci->next_statechange))
		msleep(10);

	/*
	 * Root hub was already suspended. Disable irq emission and mark HW
	 * unaccessible. The PM and USB cores make sure that the root hub is
	 * either suspended or stopped.
	 */
	spin_lock_irqsave(&ehci->lock, flags);
	ehci_prepare_ports_for_controller_suspend(ehci, device_may_wakeup(dev));
	ehci_writel(ehci, 0, &ehci->regs->intr_enable);
	ehci_readl(ehci, &ehci->regs->intr_enable);
	spin_unlock_irqrestore(&ehci->lock, flags);

	return rc;
=======
#include <linux/clk.h>
#include <linux/dma-mapping.h>
#include <linux/io.h>
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/pm.h>
#include <linux/usb.h>
#include <linux/usb/hcd.h>

#include "ehci.h"

#define DRIVER_DESC "EHCI SPEAr driver"

static const char hcd_name[] = "SPEAr-ehci";

struct spear_ehci {
	struct clk *clk;
};

#define to_spear_ehci(hcd)	(struct spear_ehci *)(hcd_to_ehci(hcd)->priv)

static struct hc_driver __read_mostly ehci_spear_hc_driver;

#ifdef CONFIG_PM_SLEEP
static int ehci_spear_drv_suspend(struct device *dev)
{
	struct usb_hcd *hcd = dev_get_drvdata(dev);
	bool do_wakeup = device_may_wakeup(dev);

	return ehci_suspend(hcd, do_wakeup);
>>>>>>> refs/remotes/origin/master
}

static int ehci_spear_drv_resume(struct device *dev)
{
	struct usb_hcd *hcd = dev_get_drvdata(dev);
<<<<<<< HEAD
	struct ehci_hcd *ehci = hcd_to_ehci(hcd);

	if (time_before(jiffies, ehci->next_statechange))
		msleep(100);

	if (ehci_readl(ehci, &ehci->regs->configured_flag) == FLAG_CF) {
		int mask = INTR_MASK;

		ehci_prepare_ports_for_controller_resume(ehci);

		if (!hcd->self.root_hub->do_remote_wakeup)
			mask &= ~STS_PCD;

		ehci_writel(ehci, mask, &ehci->regs->intr_enable);
		ehci_readl(ehci, &ehci->regs->intr_enable);
		return 0;
	}

	usb_root_hub_lost_power(hcd->self.root_hub);

	/*
	 * Else reset, to cope with power loss or flush-to-storage style
	 * "resume" having let BIOS kick in during reboot.
	 */
	ehci_halt(ehci);
	ehci_reset(ehci);

	/* emptying the schedule aborts any urbs */
	spin_lock_irq(&ehci->lock);
	if (ehci->reclaim)
		end_unlink_async(ehci);

	ehci_work(ehci);
	spin_unlock_irq(&ehci->lock);

	ehci_writel(ehci, ehci->command, &ehci->regs->command);
	ehci_writel(ehci, FLAG_CF, &ehci->regs->configured_flag);
	ehci_readl(ehci, &ehci->regs->command);	/* unblock posted writes */

	/* here we "know" root ports should always stay powered */
	ehci_port_power(ehci, 1);
	return 0;
}
#endif /* CONFIG_PM */
=======

	ehci_resume(hcd, false);
	return 0;
}
#endif /* CONFIG_PM_SLEEP */
>>>>>>> refs/remotes/origin/master

static SIMPLE_DEV_PM_OPS(ehci_spear_pm_ops, ehci_spear_drv_suspend,
		ehci_spear_drv_resume);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
static int spear_ehci_hcd_drv_probe(struct platform_device *pdev)
{
	struct usb_hcd *hcd ;
	struct spear_ehci *ehci;
	struct resource *res;
	struct clk *usbh_clk;
	const struct hc_driver *driver = &ehci_spear_hc_driver;
	int *pdata = pdev->dev.platform_data;
	int irq, retval;
	char clk_name[20] = "usbh_clk";

	if (pdata == NULL)
		return -EFAULT;
=======
static int spear_ehci_hcd_drv_probe(struct platform_device *pdev)
{
	struct usb_hcd *hcd ;
	struct spear_ehci *sehci;
	struct resource *res;
	struct clk *usbh_clk;
	const struct hc_driver *driver = &ehci_spear_hc_driver;
	int irq, retval;
>>>>>>> refs/remotes/origin/master

	if (usb_disabled())
		return -ENODEV;

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

	hcd->rsrc_start = res->start;
	hcd->rsrc_len = resource_size(res);
<<<<<<< HEAD
	if (!request_mem_region(hcd->rsrc_start, hcd->rsrc_len,
				driver->description)) {
		retval = -EBUSY;
		goto fail_request_resource;
	}

	hcd->regs = ioremap(hcd->rsrc_start, hcd->rsrc_len);
	if (hcd->regs == NULL) {
		dev_dbg(&pdev->dev, "error mapping memory\n");
		retval = -ENOMEM;
		goto fail_ioremap;
	}

	ehci = (struct spear_ehci *)hcd_to_ehci(hcd);
	ehci->clk = usbh_clk;

	spear_start_ehci(ehci);
<<<<<<< HEAD
	retval = usb_add_hcd(hcd, irq, IRQF_SHARED | IRQF_DISABLED);
=======
	retval = usb_add_hcd(hcd, irq, IRQF_SHARED);
>>>>>>> refs/remotes/origin/cm-10.0
	if (retval)
		goto fail_add_hcd;

	return retval;

fail_add_hcd:
	spear_stop_ehci(ehci);
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
	if (!devm_request_mem_region(&pdev->dev, hcd->rsrc_start, hcd->rsrc_len,
				driver->description)) {
		retval = -EBUSY;
		goto err_put_hcd;
	}

	hcd->regs = devm_ioremap(&pdev->dev, hcd->rsrc_start, hcd->rsrc_len);
	if (hcd->regs == NULL) {
		dev_dbg(&pdev->dev, "error mapping memory\n");
		retval = -ENOMEM;
		goto err_put_hcd;
	}

	sehci = to_spear_ehci(hcd);
	sehci->clk = usbh_clk;

	/* registers start at offset 0x0 */
	hcd_to_ehci(hcd)->caps = hcd->regs;

	clk_prepare_enable(sehci->clk);
	retval = usb_add_hcd(hcd, irq, IRQF_SHARED);
	if (retval)
		goto err_stop_ehci;

	device_wakeup_enable(hcd->self.controller);
	return retval;

err_stop_ehci:
	clk_disable_unprepare(sehci->clk);
err_put_hcd:
	usb_put_hcd(hcd);
fail:
>>>>>>> refs/remotes/origin/master
	dev_err(&pdev->dev, "init fail, %d\n", retval);

	return retval ;
}

static int spear_ehci_hcd_drv_remove(struct platform_device *pdev)
{
	struct usb_hcd *hcd = platform_get_drvdata(pdev);
<<<<<<< HEAD
	struct spear_ehci *ehci_p = to_spear_ehci(hcd);

	if (!hcd)
		return 0;
	if (in_interrupt())
		BUG();
	usb_remove_hcd(hcd);

	if (ehci_p->clk)
		spear_stop_ehci(ehci_p);
	iounmap(hcd->regs);
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
	usb_put_hcd(hcd);

	if (ehci_p->clk)
		clk_put(ehci_p->clk);

	return 0;
}

=======
	struct spear_ehci *sehci = to_spear_ehci(hcd);

	usb_remove_hcd(hcd);

	if (sehci->clk)
		clk_disable_unprepare(sehci->clk);
	usb_put_hcd(hcd);

	return 0;
}

static struct of_device_id spear_ehci_id_table[] = {
	{ .compatible = "st,spear600-ehci", },
	{ },
};

>>>>>>> refs/remotes/origin/master
static struct platform_driver spear_ehci_hcd_driver = {
	.probe		= spear_ehci_hcd_drv_probe,
	.remove		= spear_ehci_hcd_drv_remove,
	.shutdown	= usb_hcd_platform_shutdown,
	.driver		= {
		.name = "spear-ehci",
<<<<<<< HEAD
<<<<<<< HEAD
		.bus = &platform_bus_type
=======
		.bus = &platform_bus_type,
		.pm = &ehci_spear_pm_ops,
>>>>>>> refs/remotes/origin/cm-10.0
	}
};

MODULE_ALIAS("platform:spear-ehci");
=======
		.bus = &platform_bus_type,
		.pm = &ehci_spear_pm_ops,
		.of_match_table = spear_ehci_id_table,
	}
};

static const struct ehci_driver_overrides spear_overrides __initdata = {
	.extra_priv_size = sizeof(struct spear_ehci),
};

static int __init ehci_spear_init(void)
{
	if (usb_disabled())
		return -ENODEV;

	pr_info("%s: " DRIVER_DESC "\n", hcd_name);

	ehci_init_driver(&ehci_spear_hc_driver, &spear_overrides);
	return platform_driver_register(&spear_ehci_hcd_driver);
}
module_init(ehci_spear_init);

static void __exit ehci_spear_cleanup(void)
{
	platform_driver_unregister(&spear_ehci_hcd_driver);
}
module_exit(ehci_spear_cleanup);

MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_ALIAS("platform:spear-ehci");
MODULE_AUTHOR("Deepak Sikri");
MODULE_LICENSE("GPL");
>>>>>>> refs/remotes/origin/master
