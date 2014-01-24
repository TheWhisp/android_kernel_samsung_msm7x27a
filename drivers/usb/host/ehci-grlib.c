/*
 * Driver for Aeroflex Gaisler GRLIB GRUSBHC EHCI host controller
 *
 * GRUSBHC is typically found on LEON/GRLIB SoCs
 *
 * (c) Jan Andersson <jan@gaisler.com>
 *
 * Based on ehci-ppc-of.c which is:
 * (c) Valentine Barshak <vbarshak@ru.mvista.com>
 * and in turn based on "ehci-ppc-soc.c" by Stefan Roese <sr@denx.de>
 * and "ohci-ppc-of.c" by Sylvain Munaut <tnt@246tNt.com>
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
#include <linux/err.h>
>>>>>>> refs/remotes/origin/master
#include <linux/signal.h>

#include <linux/of_irq.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>

#define GRUSBHC_HCIVERSION 0x0100 /* Known value of cap. reg. HCIVERSION */

<<<<<<< HEAD
/* called during probe() after chip reset completes */
static int ehci_grlib_setup(struct usb_hcd *hcd)
{
	struct ehci_hcd	*ehci = hcd_to_ehci(hcd);
	int		retval;

	retval = ehci_halt(ehci);
	if (retval)
		return retval;

	retval = ehci_init(hcd);
	if (retval)
		return retval;

	ehci->sbrn = 0x20;
	ehci_port_power(ehci, 1);

	return ehci_reset(ehci);
}


=======
>>>>>>> refs/remotes/origin/master
static const struct hc_driver ehci_grlib_hc_driver = {
	.description		= hcd_name,
	.product_desc		= "GRLIB GRUSBHC EHCI",
	.hcd_priv_size		= sizeof(struct ehci_hcd),

	/*
	 * generic hardware linkage
	 */
	.irq			= ehci_irq,
<<<<<<< HEAD
	.flags			= HCD_MEMORY | HCD_USB2,
=======
	.flags			= HCD_MEMORY | HCD_USB2 | HCD_BH,
>>>>>>> refs/remotes/origin/master

	/*
	 * basic lifecycle operations
	 */
<<<<<<< HEAD
	.reset			= ehci_grlib_setup,
=======
	.reset			= ehci_setup,
>>>>>>> refs/remotes/origin/master
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
#ifdef	CONFIG_PM
	.bus_suspend		= ehci_bus_suspend,
	.bus_resume		= ehci_bus_resume,
#endif
	.relinquish_port	= ehci_relinquish_port,
	.port_handed_over	= ehci_port_handed_over,

	.clear_tt_buffer_complete	= ehci_clear_tt_buffer_complete,
};


<<<<<<< HEAD
static int __devinit ehci_hcd_grlib_probe(struct platform_device *op)
=======
static int ehci_hcd_grlib_probe(struct platform_device *op)
>>>>>>> refs/remotes/origin/master
{
	struct device_node *dn = op->dev.of_node;
	struct usb_hcd *hcd;
	struct ehci_hcd	*ehci = NULL;
	struct resource res;
	u32 hc_capbase;
	int irq;
	int rv;

	if (usb_disabled())
		return -ENODEV;

	dev_dbg(&op->dev, "initializing GRUSBHC EHCI USB Controller\n");

	rv = of_address_to_resource(dn, 0, &res);
	if (rv)
		return rv;

	/* usb_create_hcd requires dma_mask != NULL */
	op->dev.dma_mask = &op->dev.coherent_dma_mask;
	hcd = usb_create_hcd(&ehci_grlib_hc_driver, &op->dev,
			"GRUSBHC EHCI USB");
	if (!hcd)
		return -ENOMEM;

	hcd->rsrc_start = res.start;
<<<<<<< HEAD
<<<<<<< HEAD
	hcd->rsrc_len = res.end - res.start + 1;
=======
	hcd->rsrc_len = resource_size(&res);
>>>>>>> refs/remotes/origin/cm-10.0

	if (!request_mem_region(hcd->rsrc_start, hcd->rsrc_len, hcd_name)) {
		printk(KERN_ERR "%s: request_mem_region failed\n", __FILE__);
		rv = -EBUSY;
		goto err_rmr;
	}

	irq = irq_of_parse_and_map(dn, 0);
	if (irq == NO_IRQ) {
		printk(KERN_ERR "%s: irq_of_parse_and_map failed\n", __FILE__);
=======
	hcd->rsrc_len = resource_size(&res);

	irq = irq_of_parse_and_map(dn, 0);
	if (irq == NO_IRQ) {
		dev_err(&op->dev, "%s: irq_of_parse_and_map failed\n",
			__FILE__);
>>>>>>> refs/remotes/origin/master
		rv = -EBUSY;
		goto err_irq;
	}

<<<<<<< HEAD
	hcd->regs = ioremap(hcd->rsrc_start, hcd->rsrc_len);
	if (!hcd->regs) {
		printk(KERN_ERR "%s: ioremap failed\n", __FILE__);
		rv = -ENOMEM;
=======
	hcd->regs = devm_ioremap_resource(&op->dev, &res);
	if (IS_ERR(hcd->regs)) {
		rv = PTR_ERR(hcd->regs);
>>>>>>> refs/remotes/origin/master
		goto err_ioremap;
	}

	ehci = hcd_to_ehci(hcd);

	ehci->caps = hcd->regs;

	/* determine endianness of this implementation */
	hc_capbase = ehci_readl(ehci, &ehci->caps->hc_capbase);
	if (HC_VERSION(ehci, hc_capbase) != GRUSBHC_HCIVERSION) {
		ehci->big_endian_mmio = 1;
		ehci->big_endian_desc = 1;
		ehci->big_endian_capbase = 1;
	}

<<<<<<< HEAD
	ehci->regs = hcd->regs +
		HC_LENGTH(ehci, ehci_readl(ehci, &ehci->caps->hc_capbase));

	/* cache this readonly data; minimize chip reads */
	ehci->hcs_params = ehci_readl(ehci, &ehci->caps->hcs_params);

	rv = usb_add_hcd(hcd, irq, 0);
	if (rv)
		goto err_ehci;

	return 0;

err_ehci:
	iounmap(hcd->regs);
err_ioremap:
	irq_dispose_mapping(irq);
err_irq:
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
err_rmr:
=======
	rv = usb_add_hcd(hcd, irq, 0);
	if (rv)
		goto err_ioremap;

	device_wakeup_enable(hcd->self.controller);
	return 0;

err_ioremap:
	irq_dispose_mapping(irq);
err_irq:
>>>>>>> refs/remotes/origin/master
	usb_put_hcd(hcd);

	return rv;
}


static int ehci_hcd_grlib_remove(struct platform_device *op)
{
<<<<<<< HEAD
	struct usb_hcd *hcd = dev_get_drvdata(&op->dev);

	dev_set_drvdata(&op->dev, NULL);
=======
	struct usb_hcd *hcd = platform_get_drvdata(op);
>>>>>>> refs/remotes/origin/master

	dev_dbg(&op->dev, "stopping GRLIB GRUSBHC EHCI USB Controller\n");

	usb_remove_hcd(hcd);

<<<<<<< HEAD
	iounmap(hcd->regs);
	irq_dispose_mapping(hcd->irq);
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
=======
	irq_dispose_mapping(hcd->irq);
>>>>>>> refs/remotes/origin/master

	usb_put_hcd(hcd);

	return 0;
}


<<<<<<< HEAD
static void ehci_hcd_grlib_shutdown(struct platform_device *op)
{
	struct usb_hcd *hcd = dev_get_drvdata(&op->dev);

	if (hcd->driver->shutdown)
		hcd->driver->shutdown(hcd);
}


=======
>>>>>>> refs/remotes/origin/master
static const struct of_device_id ehci_hcd_grlib_of_match[] = {
	{
		.name = "GAISLER_EHCI",
	 },
	{
		.name = "01_026",
	 },
	{},
};
MODULE_DEVICE_TABLE(of, ehci_hcd_grlib_of_match);


static struct platform_driver ehci_grlib_driver = {
	.probe		= ehci_hcd_grlib_probe,
	.remove		= ehci_hcd_grlib_remove,
<<<<<<< HEAD
	.shutdown	= ehci_hcd_grlib_shutdown,
=======
	.shutdown	= usb_hcd_platform_shutdown,
>>>>>>> refs/remotes/origin/master
	.driver = {
		.name = "grlib-ehci",
		.owner = THIS_MODULE,
		.of_match_table = ehci_hcd_grlib_of_match,
	},
};
