/*
 * linux/driver/usb/host/ehci-w90x900.c
 *
 * Copyright (c) 2008 Nuvoton technology corporation.
 *
 * Wan ZongShun <mcuos.com@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation;version 2 of the License.
 *
 */

<<<<<<< HEAD
#include <linux/platform_device.h>

/*ebable phy0 and phy1 for w90p910*/
=======
#include <linux/dma-mapping.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/usb.h>
#include <linux/usb/hcd.h>

#include "ehci.h"

/* enable phy0 and phy1 for w90p910 */
>>>>>>> refs/remotes/origin/master
#define	ENPHY		(0x01<<8)
#define PHY0_CTR	(0xA4)
#define PHY1_CTR	(0xA8)

<<<<<<< HEAD
static int __devinit usb_w90x900_probe(const struct hc_driver *driver,
=======
#define DRIVER_DESC "EHCI w90x900 driver"

static const char hcd_name[] = "ehci-w90x900 ";

static struct hc_driver __read_mostly ehci_w90x900_hc_driver;

static int usb_w90x900_probe(const struct hc_driver *driver,
>>>>>>> refs/remotes/origin/master
		      struct platform_device *pdev)
{
	struct usb_hcd *hcd;
	struct ehci_hcd *ehci;
	struct resource *res;
	int retval = 0, irq;
	unsigned long val;


	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		retval = -ENXIO;
		goto err1;
	}

	hcd = usb_create_hcd(driver, &pdev->dev, "w90x900 EHCI");
	if (!hcd) {
		retval = -ENOMEM;
		goto err1;
	}

	hcd->rsrc_start = res->start;
<<<<<<< HEAD
<<<<<<< HEAD
	hcd->rsrc_len = res->end - res->start + 1;
=======
	hcd->rsrc_len = resource_size(res);
>>>>>>> refs/remotes/origin/cm-10.0

	if (!request_mem_region(hcd->rsrc_start, hcd->rsrc_len, hcd_name)) {
		retval = -EBUSY;
		goto err2;
	}

	hcd->regs = ioremap(hcd->rsrc_start, hcd->rsrc_len);
	if (hcd->regs == NULL) {
		retval = -EFAULT;
		goto err3;
	}

=======
	hcd->rsrc_len = resource_size(res);

	hcd->regs = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(hcd->regs)) {
		retval = PTR_ERR(hcd->regs);
		goto err2;
	}

>>>>>>> refs/remotes/origin/master
	ehci = hcd_to_ehci(hcd);
	ehci->caps = hcd->regs;
	ehci->regs = hcd->regs +
		HC_LENGTH(ehci, ehci_readl(ehci, &ehci->caps->hc_capbase));

	/* enable PHY 0,1,the regs only apply to w90p910
	*  0xA4,0xA8 were offsets of PHY0 and PHY1 controller of
	*  w90p910 IC relative to ehci->regs.
	*/
	val = __raw_readl(ehci->regs+PHY0_CTR);
	val |= ENPHY;
	__raw_writel(val, ehci->regs+PHY0_CTR);

	val = __raw_readl(ehci->regs+PHY1_CTR);
	val |= ENPHY;
	__raw_writel(val, ehci->regs+PHY1_CTR);

<<<<<<< HEAD
	ehci->hcs_params = ehci_readl(ehci, &ehci->caps->hcs_params);
	ehci->sbrn = 0x20;

	irq = platform_get_irq(pdev, 0);
	if (irq < 0)
		goto err4;

<<<<<<< HEAD
=======
	ehci_reset(ehci);

>>>>>>> refs/remotes/origin/cm-10.0
	retval = usb_add_hcd(hcd, irq, IRQF_SHARED);
	if (retval != 0)
		goto err4;

	ehci_writel(ehci, 1, &ehci->regs->configured_flag);

	return retval;
err4:
	iounmap(hcd->regs);
err3:
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
=======
	irq = platform_get_irq(pdev, 0);
	if (irq < 0)
		goto err2;

	retval = usb_add_hcd(hcd, irq, IRQF_SHARED);
	if (retval != 0)
		goto err2;

	device_wakeup_enable(hcd->self.controller);
	return retval;
>>>>>>> refs/remotes/origin/master
err2:
	usb_put_hcd(hcd);
err1:
	return retval;
}

<<<<<<< HEAD
static
void usb_w90x900_remove(struct usb_hcd *hcd, struct platform_device *pdev)
{
	usb_remove_hcd(hcd);
	iounmap(hcd->regs);
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
	usb_put_hcd(hcd);
}

static const struct hc_driver ehci_w90x900_hc_driver = {
	.description = hcd_name,
	.product_desc = "Nuvoton w90x900 EHCI Host Controller",
	.hcd_priv_size = sizeof(struct ehci_hcd),

	/*
	 * generic hardware linkage
	 */
	.irq = ehci_irq,
	.flags = HCD_USB2|HCD_MEMORY,

	/*
	 * basic lifecycle operations
	 */
	.reset = ehci_init,
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
#ifdef	CONFIG_PM
	.bus_suspend = ehci_bus_suspend,
	.bus_resume = ehci_bus_resume,
#endif
	.relinquish_port	= ehci_relinquish_port,
	.port_handed_over	= ehci_port_handed_over,

	.clear_tt_buffer_complete = ehci_clear_tt_buffer_complete,
};

static int __devinit ehci_w90x900_probe(struct platform_device *pdev)
=======
static void usb_w90x900_remove(struct usb_hcd *hcd,
			struct platform_device *pdev)
{
	usb_remove_hcd(hcd);
	usb_put_hcd(hcd);
}

static int ehci_w90x900_probe(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	if (usb_disabled())
		return -ENODEV;

	return usb_w90x900_probe(&ehci_w90x900_hc_driver, pdev);
}

<<<<<<< HEAD
static int __devexit ehci_w90x900_remove(struct platform_device *pdev)
=======
static int ehci_w90x900_remove(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	struct usb_hcd *hcd = platform_get_drvdata(pdev);

	usb_w90x900_remove(hcd, pdev);

	return 0;
}

static struct platform_driver ehci_hcd_w90x900_driver = {
	.probe  = ehci_w90x900_probe,
<<<<<<< HEAD
	.remove = __devexit_p(ehci_w90x900_remove),
=======
	.remove = ehci_w90x900_remove,
>>>>>>> refs/remotes/origin/master
	.driver = {
		.name = "w90x900-ehci",
		.owner = THIS_MODULE,
	},
};

<<<<<<< HEAD
MODULE_AUTHOR("Wan ZongShun <mcuos.com@gmail.com>");
MODULE_DESCRIPTION("w90p910 usb ehci driver!");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:w90p910-ehci");
=======
static int __init ehci_w90X900_init(void)
{
	if (usb_disabled())
		return -ENODEV;

	pr_info("%s: " DRIVER_DESC "\n", hcd_name);

	ehci_init_driver(&ehci_w90x900_hc_driver, NULL);
	return platform_driver_register(&ehci_hcd_w90x900_driver);
}
module_init(ehci_w90X900_init);

static void __exit ehci_w90X900_cleanup(void)
{
	platform_driver_unregister(&ehci_hcd_w90x900_driver);
}
module_exit(ehci_w90X900_cleanup);

MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_AUTHOR("Wan ZongShun <mcuos.com@gmail.com>");
MODULE_ALIAS("platform:w90p910-ehci");
MODULE_LICENSE("GPL v2");
>>>>>>> refs/remotes/origin/master
