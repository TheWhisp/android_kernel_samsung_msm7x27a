/*
 * EHCI HCD glue for Cavium Octeon II SOCs.
 *
 * Loosely based on ehci-au1xxx.c
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2010 Cavium Networks
 *
 */

#include <linux/platform_device.h>

#include <asm/octeon/octeon.h>
#include <asm/octeon/cvmx-uctlx-defs.h>

#define OCTEON_OHCI_HCD_NAME "octeon-ohci"

/* Common clock init code.  */
void octeon2_usb_clocks_start(void);
void octeon2_usb_clocks_stop(void);

static void ohci_octeon_hw_start(void)
{
	union cvmx_uctlx_ohci_ctl ohci_ctl;

	octeon2_usb_clocks_start();

	ohci_ctl.u64 = cvmx_read_csr(CVMX_UCTLX_OHCI_CTL(0));
	ohci_ctl.s.l2c_addr_msb = 0;
	ohci_ctl.s.l2c_buff_emod = 1; /* Byte swapped. */
	ohci_ctl.s.l2c_desc_emod = 1; /* Byte swapped. */
	cvmx_write_csr(CVMX_UCTLX_OHCI_CTL(0), ohci_ctl.u64);

}

static void ohci_octeon_hw_stop(void)
{
	/* Undo ohci_octeon_start() */
	octeon2_usb_clocks_stop();
}

<<<<<<< HEAD
static int __devinit ohci_octeon_start(struct usb_hcd *hcd)
=======
static int ohci_octeon_start(struct usb_hcd *hcd)
>>>>>>> refs/remotes/origin/master
{
	struct ohci_hcd	*ohci = hcd_to_ohci(hcd);
	int ret;

	ret = ohci_init(ohci);

	if (ret < 0)
		return ret;

	ret = ohci_run(ohci);

	if (ret < 0) {
		ohci_err(ohci, "can't start %s", hcd->self.bus_name);
		ohci_stop(hcd);
		return ret;
	}

	return 0;
}

static const struct hc_driver ohci_octeon_hc_driver = {
	.description		= hcd_name,
	.product_desc		= "Octeon OHCI",
	.hcd_priv_size		= sizeof(struct ohci_hcd),

	/*
	 * generic hardware linkage
	 */
	.irq =			ohci_irq,
	.flags =		HCD_USB11 | HCD_MEMORY,

	/*
	 * basic lifecycle operations
	 */
	.start =		ohci_octeon_start,
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

	.start_port_reset =	ohci_start_port_reset,
};

static int ohci_octeon_drv_probe(struct platform_device *pdev)
{
	struct usb_hcd *hcd;
	struct ohci_hcd *ohci;
	void *reg_base;
	struct resource *res_mem;
	int irq;
	int ret;

	if (usb_disabled())
		return -ENODEV;

	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		dev_err(&pdev->dev, "No irq assigned\n");
		return -ENODEV;
	}

	res_mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (res_mem == NULL) {
		dev_err(&pdev->dev, "No register space assigned\n");
		return -ENODEV;
	}

	/* Ohci is a 32-bit device. */
<<<<<<< HEAD
	pdev->dev.coherent_dma_mask = DMA_BIT_MASK(32);
	pdev->dev.dma_mask = &pdev->dev.coherent_dma_mask;
=======
	ret = dma_coerce_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(32));
	if (ret)
		return ret;
>>>>>>> refs/remotes/origin/master

	hcd = usb_create_hcd(&ohci_octeon_hc_driver, &pdev->dev, "octeon");
	if (!hcd)
		return -ENOMEM;

	hcd->rsrc_start = res_mem->start;
<<<<<<< HEAD
<<<<<<< HEAD
	hcd->rsrc_len = res_mem->end - res_mem->start + 1;
=======
	hcd->rsrc_len = resource_size(res_mem);
>>>>>>> refs/remotes/origin/cm-10.0

	if (!request_mem_region(hcd->rsrc_start, hcd->rsrc_len,
				OCTEON_OHCI_HCD_NAME)) {
		dev_err(&pdev->dev, "request_mem_region failed\n");
		ret = -EBUSY;
		goto err1;
	}

	reg_base = ioremap(hcd->rsrc_start, hcd->rsrc_len);
	if (!reg_base) {
		dev_err(&pdev->dev, "ioremap failed\n");
		ret = -ENOMEM;
		goto err2;
	}

=======
	hcd->rsrc_len = resource_size(res_mem);

	reg_base = devm_ioremap_resource(&pdev->dev, res_mem);
	if (IS_ERR(reg_base)) {
		ret = PTR_ERR(reg_base);
		goto err1;
	}

>>>>>>> refs/remotes/origin/master
	ohci_octeon_hw_start();

	hcd->regs = reg_base;

	ohci = hcd_to_ohci(hcd);

	/* Octeon OHCI matches CPU endianness. */
#ifdef __BIG_ENDIAN
	ohci->flags |= OHCI_QUIRK_BE_MMIO;
#endif

	ohci_hcd_init(ohci);

<<<<<<< HEAD
<<<<<<< HEAD
	ret = usb_add_hcd(hcd, irq, IRQF_DISABLED | IRQF_SHARED);
=======
	ret = usb_add_hcd(hcd, irq, IRQF_SHARED);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret) {
		dev_dbg(&pdev->dev, "failed to add hcd with err %d\n", ret);
		goto err3;
	}

=======
	ret = usb_add_hcd(hcd, irq, IRQF_SHARED);
	if (ret) {
		dev_dbg(&pdev->dev, "failed to add hcd with err %d\n", ret);
		goto err2;
	}

	device_wakeup_enable(hcd->self.controller);

>>>>>>> refs/remotes/origin/master
	platform_set_drvdata(pdev, hcd);

	return 0;

<<<<<<< HEAD
err3:
	ohci_octeon_hw_stop();

	iounmap(hcd->regs);
err2:
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
=======
err2:
	ohci_octeon_hw_stop();

>>>>>>> refs/remotes/origin/master
err1:
	usb_put_hcd(hcd);
	return ret;
}

static int ohci_octeon_drv_remove(struct platform_device *pdev)
{
	struct usb_hcd *hcd = platform_get_drvdata(pdev);

	usb_remove_hcd(hcd);

	ohci_octeon_hw_stop();
<<<<<<< HEAD
	iounmap(hcd->regs);
	release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
	usb_put_hcd(hcd);

	platform_set_drvdata(pdev, NULL);

=======
	usb_put_hcd(hcd);

>>>>>>> refs/remotes/origin/master
	return 0;
}

static struct platform_driver ohci_octeon_driver = {
	.probe		= ohci_octeon_drv_probe,
	.remove		= ohci_octeon_drv_remove,
	.shutdown	= usb_hcd_platform_shutdown,
	.driver = {
		.name	= OCTEON_OHCI_HCD_NAME,
		.owner	= THIS_MODULE,
	}
};

MODULE_ALIAS("platform:" OCTEON_OHCI_HCD_NAME);
