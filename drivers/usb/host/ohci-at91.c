/*
 * OHCI HCD (Host Controller Driver) for USB.
 *
 *  Copyright (C) 2004 SAN People (Pty) Ltd.
 *  Copyright (C) 2005 Thibaut VARENE <varenet@parisc-linux.org>
 *
 * AT91 Bus Glue
 *
 * Based on fragments of 2.4 driver by Rick Bronson.
 * Based on ohci-omap.c
 *
 * This file is licenced under the GPL.
 */

#include <linux/clk.h>
<<<<<<< HEAD
#include <linux/platform_device.h>
<<<<<<< HEAD
=======
#include <linux/of_platform.h>
#include <linux/of_gpio.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/dma-mapping.h>
#include <linux/of_platform.h>
#include <linux/of_gpio.h>
#include <linux/platform_device.h>
#include <linux/platform_data/atmel.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/usb.h>
#include <linux/usb/hcd.h>
>>>>>>> refs/remotes/origin/master

#include <mach/hardware.h>
#include <asm/gpio.h>

<<<<<<< HEAD
#include <mach/board.h>
#include <mach/cpu.h>

#ifndef CONFIG_ARCH_AT91
#error "CONFIG_ARCH_AT91 must be defined."
#endif

<<<<<<< HEAD
=======
=======
#include <mach/cpu.h>


#include "ohci.h"

>>>>>>> refs/remotes/origin/master
#define valid_port(index)	((index) >= 0 && (index) < AT91_MAX_USBH_PORTS)
#define at91_for_each_port(index)	\
		for ((index) = 0; (index) < AT91_MAX_USBH_PORTS; (index)++)

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
/* interface and function clocks; sometimes also an AHB clock */
static struct clk *iclk, *fclk, *hclk;
static int clocked;
=======
/* interface, function and usb clocks; sometimes also an AHB clock */
static struct clk *iclk, *fclk, *uclk, *hclk;
/* interface and function clocks; sometimes also an AHB clock */

#define DRIVER_DESC "OHCI Atmel driver"

static const char hcd_name[] = "ohci-atmel";

static struct hc_driver __read_mostly ohci_at91_hc_driver;
static int clocked;
static int (*orig_ohci_hub_control)(struct usb_hcd  *hcd, u16 typeReq,
			u16 wValue, u16 wIndex, char *buf, u16 wLength);
static int (*orig_ohci_hub_status_data)(struct usb_hcd *hcd, char *buf);
>>>>>>> refs/remotes/origin/master

extern int usb_disabled(void);

/*-------------------------------------------------------------------------*/

static void at91_start_clock(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	if (cpu_is_at91sam9261() || cpu_is_at91sam9g10())
		clk_enable(hclk);
=======
	clk_enable(hclk);
>>>>>>> refs/remotes/origin/cm-10.0
	clk_enable(iclk);
	clk_enable(fclk);
=======
	if (IS_ENABLED(CONFIG_COMMON_CLK)) {
		clk_set_rate(uclk, 48000000);
		clk_prepare_enable(uclk);
	}
	clk_prepare_enable(hclk);
	clk_prepare_enable(iclk);
	clk_prepare_enable(fclk);
>>>>>>> refs/remotes/origin/master
	clocked = 1;
}

static void at91_stop_clock(void)
{
<<<<<<< HEAD
	clk_disable(fclk);
	clk_disable(iclk);
<<<<<<< HEAD
	if (cpu_is_at91sam9261() || cpu_is_at91sam9g10())
		clk_disable(hclk);
=======
	clk_disable(hclk);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	clk_disable_unprepare(fclk);
	clk_disable_unprepare(iclk);
	clk_disable_unprepare(hclk);
	if (IS_ENABLED(CONFIG_COMMON_CLK))
		clk_disable_unprepare(uclk);
>>>>>>> refs/remotes/origin/master
	clocked = 0;
}

static void at91_start_hc(struct platform_device *pdev)
{
	struct usb_hcd *hcd = platform_get_drvdata(pdev);
	struct ohci_regs __iomem *regs = hcd->regs;

	dev_dbg(&pdev->dev, "start\n");

	/*
	 * Start the USB clocks.
	 */
	at91_start_clock();

	/*
	 * The USB host controller must remain in reset.
	 */
	writel(0, &regs->control);
}

static void at91_stop_hc(struct platform_device *pdev)
{
	struct usb_hcd *hcd = platform_get_drvdata(pdev);
	struct ohci_regs __iomem *regs = hcd->regs;

	dev_dbg(&pdev->dev, "stop\n");

	/*
	 * Put the USB host controller into reset.
	 */
	writel(0, &regs->control);

	/*
	 * Stop the USB clocks.
	 */
	at91_stop_clock();
}


/*-------------------------------------------------------------------------*/

<<<<<<< HEAD
<<<<<<< HEAD
static void usb_hcd_at91_remove (struct usb_hcd *, struct platform_device *);
=======
static void __devexit usb_hcd_at91_remove (struct usb_hcd *, struct platform_device *);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void usb_hcd_at91_remove (struct usb_hcd *, struct platform_device *);
>>>>>>> refs/remotes/origin/master

/* configure so an HC device and id are always provided */
/* always called with process context; sleeping is OK */


/**
 * usb_hcd_at91_probe - initialize AT91-based HCDs
 * Context: !in_interrupt()
 *
 * Allocates basic resources for this USB host controller, and
 * then invokes the start() method for the HCD associated with it
 * through the hotplug entry's driver_data.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static int usb_hcd_at91_probe(const struct hc_driver *driver,
=======
static int __devinit usb_hcd_at91_probe(const struct hc_driver *driver,
>>>>>>> refs/remotes/origin/cm-10.0
			struct platform_device *pdev)
{
	int retval;
	struct usb_hcd *hcd = NULL;

	if (pdev->num_resources != 2) {
		pr_debug("hcd probe: invalid num_resources");
		return -ENODEV;
	}

	if ((pdev->resource[0].flags != IORESOURCE_MEM)
			|| (pdev->resource[1].flags != IORESOURCE_IRQ)) {
		pr_debug("hcd probe: invalid resource type\n");
		return -ENODEV;
	}

	hcd = usb_create_hcd(driver, &pdev->dev, "at91");
	if (!hcd)
		return -ENOMEM;
	hcd->rsrc_start = pdev->resource[0].start;
	hcd->rsrc_len = pdev->resource[0].end - pdev->resource[0].start + 1;

	if (!request_mem_region(hcd->rsrc_start, hcd->rsrc_len, hcd_name)) {
		pr_debug("request_mem_region failed\n");
		retval = -EBUSY;
		goto err1;
	}

	hcd->regs = ioremap(hcd->rsrc_start, hcd->rsrc_len);
	if (!hcd->regs) {
		pr_debug("ioremap failed\n");
		retval = -EIO;
		goto err2;
	}

	iclk = clk_get(&pdev->dev, "ohci_clk");
<<<<<<< HEAD
	fclk = clk_get(&pdev->dev, "uhpck");
	if (cpu_is_at91sam9261() || cpu_is_at91sam9g10())
		hclk = clk_get(&pdev->dev, "hck0");
=======
	if (IS_ERR(iclk)) {
		dev_err(&pdev->dev, "failed to get ohci_clk\n");
		retval = PTR_ERR(iclk);
		goto err3;
	}
	fclk = clk_get(&pdev->dev, "uhpck");
	if (IS_ERR(fclk)) {
		dev_err(&pdev->dev, "failed to get uhpck\n");
		retval = PTR_ERR(fclk);
		goto err4;
	}
	hclk = clk_get(&pdev->dev, "hclk");
	if (IS_ERR(hclk)) {
		dev_err(&pdev->dev, "failed to get hclk\n");
		retval = PTR_ERR(hclk);
		goto err5;
	}
>>>>>>> refs/remotes/origin/cm-10.0

	at91_start_hc(pdev);
	ohci_hcd_init(hcd_to_ohci(hcd));

	retval = usb_add_hcd(hcd, pdev->resource[1].start, IRQF_SHARED);
	if (retval == 0)
		return retval;
=======
static int usb_hcd_at91_probe(const struct hc_driver *driver,
			struct platform_device *pdev)
{
	struct at91_usbh_data *board;
	struct ohci_hcd *ohci;
	int retval;
	struct usb_hcd *hcd = NULL;
	struct device *dev = &pdev->dev;
	struct resource *res;
	int irq;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_dbg(dev, "hcd probe: missing memory resource\n");
		return -ENXIO;
	}

	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		dev_dbg(dev, "hcd probe: missing irq resource\n");
		return irq;
	}

	hcd = usb_create_hcd(driver, dev, "at91");
	if (!hcd)
		return -ENOMEM;
	hcd->rsrc_start = res->start;
	hcd->rsrc_len = resource_size(res);

	hcd->regs = devm_ioremap_resource(dev, res);
	if (IS_ERR(hcd->regs)) {
		retval = PTR_ERR(hcd->regs);
		goto err;
	}

	iclk = devm_clk_get(dev, "ohci_clk");
	if (IS_ERR(iclk)) {
		dev_err(dev, "failed to get ohci_clk\n");
		retval = PTR_ERR(iclk);
		goto err;
	}
	fclk = devm_clk_get(dev, "uhpck");
	if (IS_ERR(fclk)) {
		dev_err(dev, "failed to get uhpck\n");
		retval = PTR_ERR(fclk);
		goto err;
	}
	hclk = devm_clk_get(dev, "hclk");
	if (IS_ERR(hclk)) {
		dev_err(dev, "failed to get hclk\n");
		retval = PTR_ERR(hclk);
		goto err;
	}
	if (IS_ENABLED(CONFIG_COMMON_CLK)) {
		uclk = devm_clk_get(dev, "usb_clk");
		if (IS_ERR(uclk)) {
			dev_err(dev, "failed to get uclk\n");
			retval = PTR_ERR(uclk);
			goto err;
		}
	}

	board = hcd->self.controller->platform_data;
	ohci = hcd_to_ohci(hcd);
	ohci->num_ports = board->ports;
	at91_start_hc(pdev);

	retval = usb_add_hcd(hcd, irq, IRQF_SHARED);
	if (retval == 0) {
		device_wakeup_enable(hcd->self.controller);
		return retval;
	}
>>>>>>> refs/remotes/origin/master

	/* Error handling */
	at91_stop_hc(pdev);

<<<<<<< HEAD
<<<<<<< HEAD
	if (cpu_is_at91sam9261() || cpu_is_at91sam9g10())
		clk_put(hclk);
	clk_put(fclk);
	clk_put(iclk);

=======
	clk_put(hclk);
 err5:
	clk_put(fclk);
 err4:
	clk_put(iclk);

 err3:
>>>>>>> refs/remotes/origin/cm-10.0
	iounmap(hcd->regs);

 err2:
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);

 err1:
=======
 err:
>>>>>>> refs/remotes/origin/master
	usb_put_hcd(hcd);
	return retval;
}


/* may be called with controller, bus, and devices active */

/**
 * usb_hcd_at91_remove - shutdown processing for AT91-based HCDs
 * @dev: USB Host Controller being removed
 * Context: !in_interrupt()
 *
 * Reverses the effect of usb_hcd_at91_probe(), first invoking
 * the HCD's stop() method.  It is always called from a thread
 * context, "rmmod" or something similar.
 *
 */
<<<<<<< HEAD
<<<<<<< HEAD
static void usb_hcd_at91_remove(struct usb_hcd *hcd,
=======
static void __devexit usb_hcd_at91_remove(struct usb_hcd *hcd,
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void usb_hcd_at91_remove(struct usb_hcd *hcd,
>>>>>>> refs/remotes/origin/master
				struct platform_device *pdev)
{
	usb_remove_hcd(hcd);
	at91_stop_hc(pdev);
<<<<<<< HEAD
	iounmap(hcd->regs);
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
	usb_put_hcd(hcd);

<<<<<<< HEAD
	if (cpu_is_at91sam9261() || cpu_is_at91sam9g10())
		clk_put(hclk);
=======
	clk_put(hclk);
>>>>>>> refs/remotes/origin/cm-10.0
	clk_put(fclk);
	clk_put(iclk);
	fclk = iclk = hclk = NULL;

	dev_set_drvdata(&pdev->dev, NULL);
}

/*-------------------------------------------------------------------------*/

static int __devinit
ohci_at91_start (struct usb_hcd *hcd)
{
	struct at91_usbh_data	*board = hcd->self.controller->platform_data;
	struct ohci_hcd		*ohci = hcd_to_ohci (hcd);
	int			ret;

	if ((ret = ohci_init(ohci)) < 0)
		return ret;

	ohci->num_ports = board->ports;

	if ((ret = ohci_run(ohci)) < 0) {
		err("can't start %s", hcd->self.bus_name);
		ohci_stop(hcd);
		return ret;
	}
	return 0;
}

<<<<<<< HEAD
=======
=======
	usb_put_hcd(hcd);
}

/*-------------------------------------------------------------------------*/
>>>>>>> refs/remotes/origin/master
static void ohci_at91_usb_set_power(struct at91_usbh_data *pdata, int port, int enable)
{
	if (!valid_port(port))
		return;

	if (!gpio_is_valid(pdata->vbus_pin[port]))
		return;

	gpio_set_value(pdata->vbus_pin[port],
		       pdata->vbus_pin_active_low[port] ^ enable);
}

static int ohci_at91_usb_get_power(struct at91_usbh_data *pdata, int port)
{
	if (!valid_port(port))
		return -EINVAL;

	if (!gpio_is_valid(pdata->vbus_pin[port]))
		return -EINVAL;

	return gpio_get_value(pdata->vbus_pin[port]) ^
		pdata->vbus_pin_active_low[port];
}

/*
 * Update the status data from the hub with the over-current indicator change.
 */
static int ohci_at91_hub_status_data(struct usb_hcd *hcd, char *buf)
{
	struct at91_usbh_data *pdata = hcd->self.controller->platform_data;
<<<<<<< HEAD
	int length = ohci_hub_status_data(hcd, buf);
=======
	int length = orig_ohci_hub_status_data(hcd, buf);
>>>>>>> refs/remotes/origin/master
	int port;

	at91_for_each_port(port) {
		if (pdata->overcurrent_changed[port]) {
			if (!length)
				length = 1;
			buf[0] |= 1 << (port + 1);
		}
	}

	return length;
}

/*
 * Look at the control requests to the root hub and see if we need to override.
 */
static int ohci_at91_hub_control(struct usb_hcd *hcd, u16 typeReq, u16 wValue,
				 u16 wIndex, char *buf, u16 wLength)
{
<<<<<<< HEAD
	struct at91_usbh_data *pdata = hcd->self.controller->platform_data;
=======
	struct at91_usbh_data *pdata = dev_get_platdata(hcd->self.controller);
>>>>>>> refs/remotes/origin/master
	struct usb_hub_descriptor *desc;
	int ret = -EINVAL;
	u32 *data = (u32 *)buf;

	dev_dbg(hcd->self.controller,
		"ohci_at91_hub_control(%p,0x%04x,0x%04x,0x%04x,%p,%04x)\n",
		hcd, typeReq, wValue, wIndex, buf, wLength);

	wIndex--;

	switch (typeReq) {
	case SetPortFeature:
		if (wValue == USB_PORT_FEAT_POWER) {
			dev_dbg(hcd->self.controller, "SetPortFeat: POWER\n");
			if (valid_port(wIndex)) {
				ohci_at91_usb_set_power(pdata, wIndex, 1);
				ret = 0;
			}

			goto out;
		}
		break;

	case ClearPortFeature:
		switch (wValue) {
		case USB_PORT_FEAT_C_OVER_CURRENT:
			dev_dbg(hcd->self.controller,
				"ClearPortFeature: C_OVER_CURRENT\n");

			if (valid_port(wIndex)) {
				pdata->overcurrent_changed[wIndex] = 0;
				pdata->overcurrent_status[wIndex] = 0;
			}

			goto out;

		case USB_PORT_FEAT_OVER_CURRENT:
			dev_dbg(hcd->self.controller,
				"ClearPortFeature: OVER_CURRENT\n");

			if (valid_port(wIndex))
				pdata->overcurrent_status[wIndex] = 0;

			goto out;

		case USB_PORT_FEAT_POWER:
			dev_dbg(hcd->self.controller,
				"ClearPortFeature: POWER\n");

			if (valid_port(wIndex)) {
				ohci_at91_usb_set_power(pdata, wIndex, 0);
				return 0;
			}
		}
		break;
	}

<<<<<<< HEAD
	ret = ohci_hub_control(hcd, typeReq, wValue, wIndex + 1, buf, wLength);
=======
	ret = orig_ohci_hub_control(hcd, typeReq, wValue, wIndex + 1,
				buf, wLength);
>>>>>>> refs/remotes/origin/master
	if (ret)
		goto out;

	switch (typeReq) {
	case GetHubDescriptor:

		/* update the hub's descriptor */

		desc = (struct usb_hub_descriptor *)buf;

		dev_dbg(hcd->self.controller, "wHubCharacteristics 0x%04x\n",
			desc->wHubCharacteristics);

		/* remove the old configurations for power-switching, and
		 * over-current protection, and insert our new configuration
		 */

		desc->wHubCharacteristics &= ~cpu_to_le16(HUB_CHAR_LPSM);
		desc->wHubCharacteristics |= cpu_to_le16(0x0001);

		if (pdata->overcurrent_supported) {
			desc->wHubCharacteristics &= ~cpu_to_le16(HUB_CHAR_OCPM);
			desc->wHubCharacteristics |=  cpu_to_le16(0x0008|0x0001);
		}

		dev_dbg(hcd->self.controller, "wHubCharacteristics after 0x%04x\n",
			desc->wHubCharacteristics);

		return ret;

	case GetPortStatus:
		/* check port status */

		dev_dbg(hcd->self.controller, "GetPortStatus(%d)\n", wIndex);

		if (valid_port(wIndex)) {
			if (!ohci_at91_usb_get_power(pdata, wIndex))
				*data &= ~cpu_to_le32(RH_PS_PPS);

			if (pdata->overcurrent_changed[wIndex])
				*data |= cpu_to_le32(RH_PS_OCIC);

			if (pdata->overcurrent_status[wIndex])
				*data |= cpu_to_le32(RH_PS_POCI);
		}
	}

 out:
	return ret;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
/*-------------------------------------------------------------------------*/

static const struct hc_driver ohci_at91_hc_driver = {
	.description =		hcd_name,
	.product_desc =		"AT91 OHCI",
	.hcd_priv_size =	sizeof(struct ohci_hcd),

	/*
	 * generic hardware linkage
	 */
	.irq =			ohci_irq,
	.flags =		HCD_USB11 | HCD_MEMORY,

	/*
	 * basic lifecycle operations
	 */
	.start =		ohci_at91_start,
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
<<<<<<< HEAD
	.hub_status_data =	ohci_hub_status_data,
	.hub_control =		ohci_hub_control,
=======
	.hub_status_data =	ohci_at91_hub_status_data,
	.hub_control =		ohci_at91_hub_control,
>>>>>>> refs/remotes/origin/cm-10.0
#ifdef CONFIG_PM
	.bus_suspend =		ohci_bus_suspend,
	.bus_resume =		ohci_bus_resume,
#endif
	.start_port_reset =	ohci_start_port_reset,
};

/*-------------------------------------------------------------------------*/

<<<<<<< HEAD
static int ohci_hcd_at91_drv_probe(struct platform_device *pdev)
{
	struct at91_usbh_data	*pdata = pdev->dev.platform_data;
	int			i;

	if (pdata) {
		/* REVISIT make the driver support per-port power switching,
		 * and also overcurrent detection.  Here we assume the ports
		 * are always powered while this driver is active, and use
		 * active-low power switches.
		 */
		for (i = 0; i < ARRAY_SIZE(pdata->vbus_pin); i++) {
			if (pdata->vbus_pin[i] <= 0)
				continue;
			gpio_request(pdata->vbus_pin[i], "ohci_vbus");
			gpio_direction_output(pdata->vbus_pin[i], 0);
=======
static irqreturn_t ohci_hcd_at91_overcurrent_irq(int irq, void *data)
{
	struct platform_device *pdev = data;
	struct at91_usbh_data *pdata = pdev->dev.platform_data;
=======
/*-------------------------------------------------------------------------*/

static irqreturn_t ohci_hcd_at91_overcurrent_irq(int irq, void *data)
{
	struct platform_device *pdev = data;
	struct at91_usbh_data *pdata = dev_get_platdata(&pdev->dev);
>>>>>>> refs/remotes/origin/master
	int val, gpio, port;

	/* From the GPIO notifying the over-current situation, find
	 * out the corresponding port */
	at91_for_each_port(port) {
		if (gpio_is_valid(pdata->overcurrent_pin[port]) &&
				gpio_to_irq(pdata->overcurrent_pin[port]) == irq) {
			gpio = pdata->overcurrent_pin[port];
			break;
		}
	}

	if (port == AT91_MAX_USBH_PORTS) {
		dev_err(& pdev->dev, "overcurrent interrupt from unknown GPIO\n");
		return IRQ_HANDLED;
	}

	val = gpio_get_value(gpio);

	/* When notified of an over-current situation, disable power
	   on the corresponding port, and mark this port in
	   over-current. */
	if (!val) {
		ohci_at91_usb_set_power(pdata, port, 0);
		pdata->overcurrent_status[port]  = 1;
		pdata->overcurrent_changed[port] = 1;
	}

	dev_dbg(& pdev->dev, "overcurrent situation %s\n",
		val ? "exited" : "notified");

	return IRQ_HANDLED;
}

#ifdef CONFIG_OF
static const struct of_device_id at91_ohci_dt_ids[] = {
	{ .compatible = "atmel,at91rm9200-ohci" },
	{ /* sentinel */ }
};

MODULE_DEVICE_TABLE(of, at91_ohci_dt_ids);

<<<<<<< HEAD
static u64 at91_ohci_dma_mask = DMA_BIT_MASK(32);

static int __devinit ohci_at91_of_init(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
	int i, gpio;
=======
static int ohci_at91_of_init(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
	int i, gpio, ret;
>>>>>>> refs/remotes/origin/master
	enum of_gpio_flags flags;
	struct at91_usbh_data	*pdata;
	u32 ports;

	if (!np)
		return 0;

	/* Right now device-tree probed devices don't get dma_mask set.
	 * Since shared usb code relies on it, set it here for now.
	 * Once we have dma capability bindings this can go away.
	 */
<<<<<<< HEAD
	if (!pdev->dev.dma_mask)
		pdev->dev.dma_mask = &at91_ohci_dma_mask;
=======
	ret = dma_coerce_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(32));
	if (ret)
		return ret;
>>>>>>> refs/remotes/origin/master

	pdata = devm_kzalloc(&pdev->dev, sizeof(*pdata), GFP_KERNEL);
	if (!pdata)
		return -ENOMEM;

	if (!of_property_read_u32(np, "num-ports", &ports))
		pdata->ports = ports;

	at91_for_each_port(i) {
		gpio = of_get_named_gpio_flags(np, "atmel,vbus-gpio", i, &flags);
		pdata->vbus_pin[i] = gpio;
		if (!gpio_is_valid(gpio))
			continue;
		pdata->vbus_pin_active_low[i] = flags & OF_GPIO_ACTIVE_LOW;
	}

	at91_for_each_port(i)
		pdata->overcurrent_pin[i] =
			of_get_named_gpio_flags(np, "atmel,oc-gpio", i, &flags);

	pdev->dev.platform_data = pdata;

	return 0;
}
#else
<<<<<<< HEAD
static int __devinit ohci_at91_of_init(struct platform_device *pdev)
=======
static int ohci_at91_of_init(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	return 0;
}
#endif

/*-------------------------------------------------------------------------*/

<<<<<<< HEAD
static int __devinit ohci_hcd_at91_drv_probe(struct platform_device *pdev)
=======
static int ohci_hcd_at91_drv_probe(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	struct at91_usbh_data	*pdata;
	int			i;
	int			gpio;
	int			ret;

	ret = ohci_at91_of_init(pdev);
	if (ret)
		return ret;

<<<<<<< HEAD
	pdata = pdev->dev.platform_data;
=======
	pdata = dev_get_platdata(&pdev->dev);
>>>>>>> refs/remotes/origin/master

	if (pdata) {
		at91_for_each_port(i) {
			/*
			 * do not configure PIO if not in relation with
			 * real USB port on board
			 */
			if (i >= pdata->ports) {
				pdata->vbus_pin[i] = -EINVAL;
				pdata->overcurrent_pin[i] = -EINVAL;
				break;
			}

			if (!gpio_is_valid(pdata->vbus_pin[i]))
				continue;
			gpio = pdata->vbus_pin[i];

			ret = gpio_request(gpio, "ohci_vbus");
			if (ret) {
				dev_err(&pdev->dev,
					"can't request vbus gpio %d\n", gpio);
				continue;
			}
			ret = gpio_direction_output(gpio,
						!pdata->vbus_pin_active_low[i]);
			if (ret) {
				dev_err(&pdev->dev,
					"can't put vbus gpio %d as output %d\n",
					gpio, !pdata->vbus_pin_active_low[i]);
				gpio_free(gpio);
				continue;
			}

			ohci_at91_usb_set_power(pdata, i, 1);
		}

		at91_for_each_port(i) {
			if (!gpio_is_valid(pdata->overcurrent_pin[i]))
				continue;
			gpio = pdata->overcurrent_pin[i];

			ret = gpio_request(gpio, "ohci_overcurrent");
			if (ret) {
				dev_err(&pdev->dev,
					"can't request overcurrent gpio %d\n",
					gpio);
				continue;
			}

			ret = gpio_direction_input(gpio);
			if (ret) {
				dev_err(&pdev->dev,
					"can't configure overcurrent gpio %d as input\n",
					gpio);
				gpio_free(gpio);
				continue;
			}

			ret = request_irq(gpio_to_irq(gpio),
					  ohci_hcd_at91_overcurrent_irq,
					  IRQF_SHARED, "ohci_overcurrent", pdev);
			if (ret) {
				gpio_free(gpio);
				dev_err(&pdev->dev,
					"can't get gpio IRQ for overcurrent\n");
			}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		}
	}

	device_init_wakeup(&pdev->dev, 1);
	return usb_hcd_at91_probe(&ohci_at91_hc_driver, pdev);
}

<<<<<<< HEAD
<<<<<<< HEAD
static int ohci_hcd_at91_drv_remove(struct platform_device *pdev)
=======
static int __devexit ohci_hcd_at91_drv_remove(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct at91_usbh_data	*pdata = pdev->dev.platform_data;
	int			i;

	if (pdata) {
<<<<<<< HEAD
		for (i = 0; i < ARRAY_SIZE(pdata->vbus_pin); i++) {
			if (pdata->vbus_pin[i] <= 0)
				continue;
			gpio_direction_output(pdata->vbus_pin[i], 1);
			gpio_free(pdata->vbus_pin[i]);
		}
=======
=======
static int ohci_hcd_at91_drv_remove(struct platform_device *pdev)
{
	struct at91_usbh_data	*pdata = dev_get_platdata(&pdev->dev);
	int			i;

	if (pdata) {
>>>>>>> refs/remotes/origin/master
		at91_for_each_port(i) {
			if (!gpio_is_valid(pdata->vbus_pin[i]))
				continue;
			ohci_at91_usb_set_power(pdata, i, 0);
			gpio_free(pdata->vbus_pin[i]);
		}

		at91_for_each_port(i) {
			if (!gpio_is_valid(pdata->overcurrent_pin[i]))
				continue;
			free_irq(gpio_to_irq(pdata->overcurrent_pin[i]), pdev);
			gpio_free(pdata->overcurrent_pin[i]);
		}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	device_init_wakeup(&pdev->dev, 0);
	usb_hcd_at91_remove(platform_get_drvdata(pdev), pdev);
	return 0;
}

#ifdef CONFIG_PM

static int
ohci_hcd_at91_drv_suspend(struct platform_device *pdev, pm_message_t mesg)
{
	struct usb_hcd	*hcd = platform_get_drvdata(pdev);
	struct ohci_hcd	*ohci = hcd_to_ohci(hcd);
<<<<<<< HEAD

	if (device_may_wakeup(&pdev->dev))
		enable_irq_wake(hcd->irq);

=======
	bool		do_wakeup = device_may_wakeup(&pdev->dev);
	int		ret;

	if (do_wakeup)
		enable_irq_wake(hcd->irq);

	ret = ohci_suspend(hcd, do_wakeup);
	if (ret) {
		disable_irq_wake(hcd->irq);
		return ret;
	}
>>>>>>> refs/remotes/origin/master
	/*
	 * The integrated transceivers seem unable to notice disconnect,
	 * reconnect, or wakeup without the 48 MHz clock active.  so for
	 * correctness, always discard connection state (using reset).
	 *
	 * REVISIT: some boards will be able to turn VBUS off...
	 */
	if (at91_suspend_entering_slow_clock()) {
<<<<<<< HEAD
		ohci_usb_reset (ohci);
=======
		ohci->hc_control = ohci_readl(ohci, &ohci->regs->control);
		ohci->hc_control &= OHCI_CTRL_RWC;
		ohci_writel(ohci, ohci->hc_control, &ohci->regs->control);
		ohci->rh_state = OHCI_RH_HALTED;

>>>>>>> refs/remotes/origin/master
		/* flush the writes */
		(void) ohci_readl (ohci, &ohci->regs->control);
		at91_stop_clock();
	}

<<<<<<< HEAD
	return 0;
=======
	return ret;
>>>>>>> refs/remotes/origin/master
}

static int ohci_hcd_at91_drv_resume(struct platform_device *pdev)
{
	struct usb_hcd	*hcd = platform_get_drvdata(pdev);

	if (device_may_wakeup(&pdev->dev))
		disable_irq_wake(hcd->irq);

	if (!clocked)
		at91_start_clock();

<<<<<<< HEAD
	ohci_finish_controller_resume(hcd);
=======
	ohci_resume(hcd, false);
>>>>>>> refs/remotes/origin/master
	return 0;
}
#else
#define ohci_hcd_at91_drv_suspend NULL
#define ohci_hcd_at91_drv_resume  NULL
#endif

<<<<<<< HEAD
MODULE_ALIAS("platform:at91_ohci");

static struct platform_driver ohci_hcd_at91_driver = {
	.probe		= ohci_hcd_at91_drv_probe,
<<<<<<< HEAD
	.remove		= ohci_hcd_at91_drv_remove,
=======
	.remove		= __devexit_p(ohci_hcd_at91_drv_remove),
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct platform_driver ohci_hcd_at91_driver = {
	.probe		= ohci_hcd_at91_drv_probe,
	.remove		= ohci_hcd_at91_drv_remove,
>>>>>>> refs/remotes/origin/master
	.shutdown	= usb_hcd_platform_shutdown,
	.suspend	= ohci_hcd_at91_drv_suspend,
	.resume		= ohci_hcd_at91_drv_resume,
	.driver		= {
		.name	= "at91_ohci",
		.owner	= THIS_MODULE,
<<<<<<< HEAD
<<<<<<< HEAD
=======
		.of_match_table	= of_match_ptr(at91_ohci_dt_ids),
>>>>>>> refs/remotes/origin/cm-10.0
	},
};
=======
		.of_match_table	= of_match_ptr(at91_ohci_dt_ids),
	},
};

static int __init ohci_at91_init(void)
{
	if (usb_disabled())
		return -ENODEV;

	pr_info("%s: " DRIVER_DESC "\n", hcd_name);
	ohci_init_driver(&ohci_at91_hc_driver, NULL);

	/*
	 * The Atmel HW has some unusual quirks, which require Atmel-specific
	 * workarounds. We override certain hc_driver functions here to
	 * achieve that. We explicitly do not enhance ohci_driver_overrides to
	 * allow this more easily, since this is an unusual case, and we don't
	 * want to encourage others to override these functions by making it
	 * too easy.
	 */

	orig_ohci_hub_control = ohci_at91_hc_driver.hub_control;
	orig_ohci_hub_status_data = ohci_at91_hc_driver.hub_status_data;

	ohci_at91_hc_driver.hub_status_data	= ohci_at91_hub_status_data;
	ohci_at91_hc_driver.hub_control		= ohci_at91_hub_control;

	return platform_driver_register(&ohci_hcd_at91_driver);
}
module_init(ohci_at91_init);

static void __exit ohci_at91_cleanup(void)
{
	platform_driver_unregister(&ohci_hcd_at91_driver);
}
module_exit(ohci_at91_cleanup);

MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:at91_ohci");
>>>>>>> refs/remotes/origin/master
