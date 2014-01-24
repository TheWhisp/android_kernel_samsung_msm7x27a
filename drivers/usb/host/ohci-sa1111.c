/*
 * OHCI HCD (Host Controller Driver) for USB.
 *
 * (C) Copyright 1999 Roman Weissgaerber <weissg@vienna.at>
 * (C) Copyright 2000-2002 David Brownell <dbrownell@users.sourceforge.net>
 * (C) Copyright 2002 Hewlett-Packard Company
 *
 * SA1111 Bus Glue
 *
 * Written by Christopher Hoover <ch@hpl.hp.com>
 * Based on fragments of previous driver by Russell King et al.
 *
 * This file is licenced under the GPL.
 */

#include <mach/hardware.h>
#include <asm/mach-types.h>
#include <mach/assabet.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <mach/badge4.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#include <asm/hardware/sa1111.h>

#ifndef CONFIG_SA1111
#error "This file is SA-1111 bus glue.  CONFIG_SA1111 must be defined."
#endif

<<<<<<< HEAD
<<<<<<< HEAD
extern int usb_disabled(void);

/*-------------------------------------------------------------------------*/

static void sa1111_start_hc(struct sa1111_dev *dev)
{
	unsigned int usb_rst = 0;

	printk(KERN_DEBUG "%s: starting SA-1111 OHCI USB Controller\n",
	       __FILE__);

#ifdef CONFIG_SA1100_BADGE4
	if (machine_is_badge4()) {
		badge4_set_5V(BADGE4_5V_USB, 1);
	}
#endif
=======
=======
>>>>>>> refs/remotes/origin/master
#define USB_STATUS	0x0118
#define USB_RESET	0x011c
#define USB_IRQTEST	0x0120

#define USB_RESET_FORCEIFRESET	(1 << 0)
#define USB_RESET_FORCEHCRESET	(1 << 1)
#define USB_RESET_CLKGENRESET	(1 << 2)
#define USB_RESET_SIMSCALEDOWN	(1 << 3)
#define USB_RESET_USBINTTEST	(1 << 4)
#define USB_RESET_SLEEPSTBYEN	(1 << 5)
#define USB_RESET_PWRSENSELOW	(1 << 6)
#define USB_RESET_PWRCTRLLOW	(1 << 7)

#define USB_STATUS_IRQHCIRMTWKUP  (1 <<  7)
#define USB_STATUS_IRQHCIBUFFACC  (1 <<  8)
#define USB_STATUS_NIRQHCIM       (1 <<  9)
#define USB_STATUS_NHCIMFCLR      (1 << 10)
#define USB_STATUS_USBPWRSENSE    (1 << 11)

#if 0
static void dump_hci_status(struct usb_hcd *hcd, const char *label)
{
	unsigned long status = sa1111_readl(hcd->regs + USB_STATUS);

<<<<<<< HEAD
	dbg("%s USB_STATUS = { %s%s%s%s%s}", label,
=======
	printk(KERN_DEBUG "%s USB_STATUS = { %s%s%s%s%s}\n", label,
>>>>>>> refs/remotes/origin/master
	     ((status & USB_STATUS_IRQHCIRMTWKUP) ? "IRQHCIRMTWKUP " : ""),
	     ((status & USB_STATUS_IRQHCIBUFFACC) ? "IRQHCIBUFFACC " : ""),
	     ((status & USB_STATUS_NIRQHCIM) ? "" : "IRQHCIM "),
	     ((status & USB_STATUS_NHCIMFCLR) ? "" : "HCIMFCLR "),
	     ((status & USB_STATUS_USBPWRSENSE) ? "USBPWRSENSE " : ""));
}
#endif

static int ohci_sa1111_reset(struct usb_hcd *hcd)
{
	struct ohci_hcd *ohci = hcd_to_ohci(hcd);

	ohci_hcd_init(ohci);
	return ohci_init(ohci);
}

<<<<<<< HEAD
static int __devinit ohci_sa1111_start(struct usb_hcd *hcd)
=======
static int ohci_sa1111_start(struct usb_hcd *hcd)
>>>>>>> refs/remotes/origin/master
{
	struct ohci_hcd	*ohci = hcd_to_ohci(hcd);
	int ret;

	ret = ohci_run(ohci);
	if (ret < 0) {
		ohci_err(ohci, "can't start\n");
		ohci_stop(hcd);
	}
	return ret;
}

static const struct hc_driver ohci_sa1111_hc_driver = {
	.description =		hcd_name,
	.product_desc =		"SA-1111 OHCI",
	.hcd_priv_size =	sizeof(struct ohci_hcd),

	/*
	 * generic hardware linkage
	 */
	.irq =			ohci_irq,
	.flags =		HCD_USB11 | HCD_MEMORY,

	/*
	 * basic lifecycle operations
	 */
	.reset =		ohci_sa1111_reset,
	.start =		ohci_sa1111_start,
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
	.hub_status_data =	ohci_hub_status_data,
	.hub_control =		ohci_hub_control,
#ifdef	CONFIG_PM
	.bus_suspend =		ohci_bus_suspend,
	.bus_resume =		ohci_bus_resume,
#endif
	.start_port_reset =	ohci_start_port_reset,
};

static int sa1111_start_hc(struct sa1111_dev *dev)
{
	unsigned int usb_rst = 0;
	int ret;

	dev_dbg(&dev->dev, "starting SA-1111 OHCI USB Controller\n");
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	if (machine_is_xp860() ||
	    machine_has_neponset() ||
	    machine_is_pfs168() ||
	    machine_is_badge4())
		usb_rst = USB_RESET_PWRSENSELOW | USB_RESET_PWRCTRLLOW;

	/*
	 * Configure the power sense and control lines.  Place the USB
	 * host controller in reset.
	 */
	sa1111_writel(usb_rst | USB_RESET_FORCEIFRESET | USB_RESET_FORCEHCRESET,
<<<<<<< HEAD
<<<<<<< HEAD
		      dev->mapbase + SA1111_USB_RESET);
=======
		      dev->mapbase + USB_RESET);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		      dev->mapbase + USB_RESET);
>>>>>>> refs/remotes/origin/master

	/*
	 * Now, carefully enable the USB clock, and take
	 * the USB host controller out of reset.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	sa1111_enable_device(dev);
	udelay(11);
	sa1111_writel(usb_rst, dev->mapbase + SA1111_USB_RESET);
=======
=======
>>>>>>> refs/remotes/origin/master
	ret = sa1111_enable_device(dev);
	if (ret == 0) {
		udelay(11);
		sa1111_writel(usb_rst, dev->mapbase + USB_RESET);
	}

	return ret;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static void sa1111_stop_hc(struct sa1111_dev *dev)
{
	unsigned int usb_rst;
<<<<<<< HEAD
<<<<<<< HEAD
	printk(KERN_DEBUG "%s: stopping SA-1111 OHCI USB Controller\n",
	       __FILE__);
=======

	dev_dbg(&dev->dev, "stopping SA-1111 OHCI USB Controller\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======

	dev_dbg(&dev->dev, "stopping SA-1111 OHCI USB Controller\n");
>>>>>>> refs/remotes/origin/master

	/*
	 * Put the USB host controller into reset.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	usb_rst = sa1111_readl(dev->mapbase + SA1111_USB_RESET);
	sa1111_writel(usb_rst | USB_RESET_FORCEIFRESET | USB_RESET_FORCEHCRESET,
		      dev->mapbase + SA1111_USB_RESET);
=======
	usb_rst = sa1111_readl(dev->mapbase + USB_RESET);
	sa1111_writel(usb_rst | USB_RESET_FORCEIFRESET | USB_RESET_FORCEHCRESET,
		      dev->mapbase + USB_RESET);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	usb_rst = sa1111_readl(dev->mapbase + USB_RESET);
	sa1111_writel(usb_rst | USB_RESET_FORCEIFRESET | USB_RESET_FORCEHCRESET,
		      dev->mapbase + USB_RESET);
>>>>>>> refs/remotes/origin/master

	/*
	 * Stop the USB clock.
	 */
	sa1111_disable_device(dev);
<<<<<<< HEAD
<<<<<<< HEAD

#ifdef CONFIG_SA1100_BADGE4
	if (machine_is_badge4()) {
		/* Disable power to the USB bus */
		badge4_set_5V(BADGE4_5V_USB, 0);
	}
#endif
}


/*-------------------------------------------------------------------------*/

#if 0
static void dump_hci_status(struct usb_hcd *hcd, const char *label)
{
	unsigned long status = sa1111_readl(hcd->regs + SA1111_USB_STATUS);

	dbg ("%s USB_STATUS = { %s%s%s%s%s}", label,
	     ((status & USB_STATUS_IRQHCIRMTWKUP) ? "IRQHCIRMTWKUP " : ""),
	     ((status & USB_STATUS_IRQHCIBUFFACC) ? "IRQHCIBUFFACC " : ""),
	     ((status & USB_STATUS_NIRQHCIM) ? "" : "IRQHCIM "),
	     ((status & USB_STATUS_NHCIMFCLR) ? "" : "HCIMFCLR "),
	     ((status & USB_STATUS_USBPWRSENSE) ? "USBPWRSENSE " : ""));
}
#endif

/*-------------------------------------------------------------------------*/

/* configure so an HC device and id are always provided */
/* always called with process context; sleeping is OK */


/**
 * usb_hcd_sa1111_probe - initialize SA-1111-based HCDs
 * Context: !in_interrupt()
 *
 * Allocates basic resources for this USB host controller, and
 * then invokes the start() method for the HCD associated with it
 * through the hotplug entry's driver_data.
 *
 * Store this function in the HCD's struct pci_driver as probe().
 */
int usb_hcd_sa1111_probe (const struct hc_driver *driver,
			  struct sa1111_dev *dev)
{
	struct usb_hcd *hcd;
	int retval;

	hcd = usb_create_hcd (driver, &dev->dev, "sa1111");
	if (!hcd)
		return -ENOMEM;
	hcd->rsrc_start = dev->res.start;
	hcd->rsrc_len = dev->res.end - dev->res.start + 1;

	if (!request_mem_region(hcd->rsrc_start, hcd->rsrc_len, hcd_name)) {
		dbg("request_mem_region failed");
		retval = -EBUSY;
		goto err1;
	}
	hcd->regs = dev->mapbase;

	sa1111_start_hc(dev);
	ohci_hcd_init(hcd_to_ohci(hcd));

	retval = usb_add_hcd(hcd, dev->irq[1], IRQF_DISABLED);
	if (retval == 0)
		return retval;

	sa1111_stop_hc(dev);
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
 err1:
	usb_put_hcd(hcd);
	return retval;
}


/* may be called without controller electrically present */
/* may be called with controller, bus, and devices active */

/**
 * usb_hcd_sa1111_remove - shutdown processing for SA-1111-based HCDs
 * @dev: USB Host Controller being removed
 * Context: !in_interrupt()
 *
 * Reverses the effect of usb_hcd_sa1111_probe(), first invoking
 * the HCD's stop() method.  It is always called from a thread
 * context, normally "rmmod", "apmd", or something similar.
 *
 */
void usb_hcd_sa1111_remove (struct usb_hcd *hcd, struct sa1111_dev *dev)
{
=======
=======
>>>>>>> refs/remotes/origin/master
}

/**
 * ohci_hcd_sa1111_probe - initialize SA-1111-based HCDs
 *
 * Allocates basic resources for this USB host controller, and
 * then invokes the start() method for the HCD associated with it.
 */
static int ohci_hcd_sa1111_probe(struct sa1111_dev *dev)
{
	struct usb_hcd *hcd;
	int ret;

	if (usb_disabled())
		return -ENODEV;

<<<<<<< HEAD
=======
	/*
	 * We don't call dma_set_mask_and_coherent() here because the
	 * DMA mask has already been appropraitely setup by the core
	 * SA-1111 bus code (which includes bug workarounds.)
	 */

>>>>>>> refs/remotes/origin/master
	hcd = usb_create_hcd(&ohci_sa1111_hc_driver, &dev->dev, "sa1111");
	if (!hcd)
		return -ENOMEM;

	hcd->rsrc_start = dev->res.start;
	hcd->rsrc_len = resource_size(&dev->res);

	if (!request_mem_region(hcd->rsrc_start, hcd->rsrc_len, hcd_name)) {
<<<<<<< HEAD
		dbg("request_mem_region failed");
=======
		dev_dbg(&dev->dev, "request_mem_region failed\n");
>>>>>>> refs/remotes/origin/master
		ret = -EBUSY;
		goto err1;
	}

	hcd->regs = dev->mapbase;

	ret = sa1111_start_hc(dev);
	if (ret)
		goto err2;

	ret = usb_add_hcd(hcd, dev->irq[1], 0);
<<<<<<< HEAD
	if (ret == 0)
		return ret;
=======
	if (ret == 0) {
		device_wakeup_enable(hcd->self.controller);
		return ret;
	}
>>>>>>> refs/remotes/origin/master

	sa1111_stop_hc(dev);
 err2:
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
 err1:
	usb_put_hcd(hcd);
	return ret;
}

/**
 * ohci_hcd_sa1111_remove - shutdown processing for SA-1111-based HCDs
 * @dev: USB Host Controller being removed
 *
 * Reverses the effect of ohci_hcd_sa1111_probe(), first invoking
 * the HCD's stop() method.
 */
static int ohci_hcd_sa1111_remove(struct sa1111_dev *dev)
{
	struct usb_hcd *hcd = sa1111_get_drvdata(dev);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	usb_remove_hcd(hcd);
	sa1111_stop_hc(dev);
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
	usb_put_hcd(hcd);
<<<<<<< HEAD
<<<<<<< HEAD
}

/*-------------------------------------------------------------------------*/

static int __devinit
ohci_sa1111_start (struct usb_hcd *hcd)
{
	struct ohci_hcd	*ohci = hcd_to_ohci (hcd);
	int		ret;

	if ((ret = ohci_init(ohci)) < 0)
		return ret;

	if ((ret = ohci_run (ohci)) < 0) {
		err ("can't start %s", hcd->self.bus_name);
		ohci_stop (hcd);
		return ret;
	}
	return 0;
}

/*-------------------------------------------------------------------------*/

static const struct hc_driver ohci_sa1111_hc_driver = {
	.description =		hcd_name,
	.product_desc =		"SA-1111 OHCI",
	.hcd_priv_size =	sizeof(struct ohci_hcd),

	/*
	 * generic hardware linkage
	 */
	.irq =			ohci_irq,
	.flags =		HCD_USB11 | HCD_MEMORY,

	/*
	 * basic lifecycle operations
	 */
	.start =		ohci_sa1111_start,
	.stop =			ohci_stop,

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
	.hub_status_data =	ohci_hub_status_data,
	.hub_control =		ohci_hub_control,
#ifdef	CONFIG_PM
	.bus_suspend =		ohci_bus_suspend,
	.bus_resume =		ohci_bus_resume,
#endif
	.start_port_reset =	ohci_start_port_reset,
};

/*-------------------------------------------------------------------------*/

static int ohci_hcd_sa1111_drv_probe(struct sa1111_dev *dev)
{
	int ret;

	if (usb_disabled())
		return -ENODEV;

	ret = usb_hcd_sa1111_probe(&ohci_sa1111_hc_driver, dev);
	return ret;
}

static int ohci_hcd_sa1111_drv_remove(struct sa1111_dev *dev)
{
	struct usb_hcd *hcd = sa1111_get_drvdata(dev);

	usb_hcd_sa1111_remove(hcd, dev);
	return 0;
=======
=======
>>>>>>> refs/remotes/origin/master

	return 0;
}

static void ohci_hcd_sa1111_shutdown(struct sa1111_dev *dev)
{
	struct usb_hcd *hcd = sa1111_get_drvdata(dev);

	if (test_bit(HCD_FLAG_HW_ACCESSIBLE, &hcd->flags)) {
		hcd->driver->shutdown(hcd);
		sa1111_stop_hc(dev);
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static struct sa1111_driver ohci_hcd_sa1111_driver = {
	.drv = {
		.name	= "sa1111-ohci",
<<<<<<< HEAD
<<<<<<< HEAD
	},
	.devid		= SA1111_DEVID_USB,
	.probe		= ohci_hcd_sa1111_drv_probe,
	.remove		= ohci_hcd_sa1111_drv_remove,
};

=======
=======
>>>>>>> refs/remotes/origin/master
		.owner	= THIS_MODULE,
	},
	.devid		= SA1111_DEVID_USB,
	.probe		= ohci_hcd_sa1111_probe,
	.remove		= ohci_hcd_sa1111_remove,
	.shutdown	= ohci_hcd_sa1111_shutdown,
};
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
