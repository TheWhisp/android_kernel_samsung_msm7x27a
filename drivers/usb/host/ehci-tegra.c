/*
 * EHCI-compliant USB host controller driver for NVIDIA Tegra SoCs
 *
 * Copyright (C) 2010 Google, Inc.
<<<<<<< HEAD
 * Copyright (C) 2009 NVIDIA Corporation
=======
 * Copyright (C) 2009 - 2013 NVIDIA Corporation
>>>>>>> refs/remotes/origin/master
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 */

#include <linux/clk.h>
<<<<<<< HEAD
#include <linux/platform_device.h>
#include <linux/platform_data/tegra_usb.h>
#include <linux/irq.h>
#include <linux/usb/otg.h>
<<<<<<< HEAD
#include <mach/usb_phy.h>
=======
#include <linux/gpio.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/pm_runtime.h>

#include <mach/usb_phy.h>
#include <mach/iomap.h>
>>>>>>> refs/remotes/origin/cm-10.0

#define TEGRA_USB_DMA_ALIGN 32

struct tegra_ehci_hcd {
	struct ehci_hcd *ehci;
	struct tegra_usb_phy *phy;
	struct clk *clk;
	struct clk *emc_clk;
<<<<<<< HEAD
	struct otg_transceiver *transceiver;
	int host_resumed;
	int bus_suspended;
	int port_resuming;
	int power_down_on_bus_suspend;
=======
	struct usb_phy *transceiver;
	int host_resumed;
	int port_resuming;
>>>>>>> refs/remotes/origin/cm-10.0
	enum tegra_usb_phy_port_speed port_speed;
};

static void tegra_ehci_power_up(struct usb_hcd *hcd)
{
	struct tegra_ehci_hcd *tegra = dev_get_drvdata(hcd->self.controller);

	clk_enable(tegra->emc_clk);
	clk_enable(tegra->clk);
	tegra_usb_phy_power_on(tegra->phy);
	tegra->host_resumed = 1;
}

static void tegra_ehci_power_down(struct usb_hcd *hcd)
{
	struct tegra_ehci_hcd *tegra = dev_get_drvdata(hcd->self.controller);

	tegra->host_resumed = 0;
	tegra_usb_phy_power_off(tegra->phy);
	clk_disable(tegra->clk);
	clk_disable(tegra->emc_clk);
}

=======
#include <linux/clk/tegra.h>
#include <linux/dma-mapping.h>
#include <linux/err.h>
#include <linux/gpio.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/slab.h>
#include <linux/usb/ehci_def.h>
#include <linux/usb/tegra_usb_phy.h>
#include <linux/usb.h>
#include <linux/usb/hcd.h>
#include <linux/usb/otg.h>

#include "ehci.h"

#define TEGRA_USB_BASE			0xC5000000
#define TEGRA_USB2_BASE			0xC5004000
#define TEGRA_USB3_BASE			0xC5008000

#define PORT_WAKE_BITS (PORT_WKOC_E|PORT_WKDISC_E|PORT_WKCONN_E)

#define TEGRA_USB_DMA_ALIGN 32

#define DRIVER_DESC "Tegra EHCI driver"
#define DRV_NAME "tegra-ehci"

static struct hc_driver __read_mostly tegra_ehci_hc_driver;

struct tegra_ehci_soc_config {
	bool has_hostpc;
};

static int (*orig_hub_control)(struct usb_hcd *hcd,
				u16 typeReq, u16 wValue, u16 wIndex,
				char *buf, u16 wLength);

struct tegra_ehci_hcd {
	struct tegra_usb_phy *phy;
	struct clk *clk;
	int port_resuming;
	bool needs_double_reset;
	enum tegra_usb_phy_port_speed port_speed;
};

>>>>>>> refs/remotes/origin/master
static int tegra_ehci_internal_port_reset(
	struct ehci_hcd	*ehci,
	u32 __iomem	*portsc_reg
)
{
	u32		temp;
	unsigned long	flags;
	int		retval = 0;
	int		i, tries;
	u32		saved_usbintr;

	spin_lock_irqsave(&ehci->lock, flags);
	saved_usbintr = ehci_readl(ehci, &ehci->regs->intr_enable);
	/* disable USB interrupt */
	ehci_writel(ehci, 0, &ehci->regs->intr_enable);
	spin_unlock_irqrestore(&ehci->lock, flags);

	/*
	 * Here we have to do Port Reset at most twice for
	 * Port Enable bit to be set.
	 */
	for (i = 0; i < 2; i++) {
		temp = ehci_readl(ehci, portsc_reg);
		temp |= PORT_RESET;
		ehci_writel(ehci, temp, portsc_reg);
		mdelay(10);
		temp &= ~PORT_RESET;
		ehci_writel(ehci, temp, portsc_reg);
		mdelay(1);
		tries = 100;
		do {
			mdelay(1);
			/*
			 * Up to this point, Port Enable bit is
			 * expected to be set after 2 ms waiting.
			 * USB1 usually takes extra 45 ms, for safety,
			 * we take 100 ms as timeout.
			 */
			temp = ehci_readl(ehci, portsc_reg);
		} while (!(temp & PORT_PE) && tries--);
		if (temp & PORT_PE)
			break;
	}
	if (i == 2)
		retval = -ETIMEDOUT;

	/*
	 * Clear Connect Status Change bit if it's set.
	 * We can't clear PORT_PEC. It will also cause PORT_PE to be cleared.
	 */
	if (temp & PORT_CSC)
		ehci_writel(ehci, PORT_CSC, portsc_reg);

	/*
	 * Write to clear any interrupt status bits that might be set
	 * during port reset.
	 */
	temp = ehci_readl(ehci, &ehci->regs->status);
	ehci_writel(ehci, temp, &ehci->regs->status);

	/* restore original interrupt enable bits */
	ehci_writel(ehci, saved_usbintr, &ehci->regs->intr_enable);
	return retval;
}

static int tegra_ehci_hub_control(
	struct usb_hcd	*hcd,
	u16		typeReq,
	u16		wValue,
	u16		wIndex,
	char		*buf,
	u16		wLength
)
{
<<<<<<< HEAD
	struct ehci_hcd	*ehci = hcd_to_ehci(hcd);
	struct tegra_ehci_hcd *tegra = dev_get_drvdata(hcd->self.controller);
=======
	struct ehci_hcd *ehci = hcd_to_ehci(hcd);
	struct tegra_ehci_hcd *tegra = (struct tegra_ehci_hcd *)ehci->priv;
>>>>>>> refs/remotes/origin/master
	u32 __iomem	*status_reg;
	u32		temp;
	unsigned long	flags;
	int		retval = 0;

	status_reg = &ehci->regs->port_status[(wIndex & 0xff) - 1];

	spin_lock_irqsave(&ehci->lock, flags);

<<<<<<< HEAD
	/*
	 * In ehci_hub_control() for USB_PORT_FEAT_ENABLE clears the other bits
	 * that are write on clear, by writing back the register read value, so
	 * USB_PORT_FEAT_ENABLE is handled by masking the set on clear bits
	 */
	if (typeReq == ClearPortFeature && wValue == USB_PORT_FEAT_ENABLE) {
		temp = ehci_readl(ehci, status_reg) & ~PORT_RWC_BITS;
		ehci_writel(ehci, temp & ~PORT_PE, status_reg);
		goto done;
	}

	else if (typeReq == GetPortStatus) {
=======
	if (typeReq == GetPortStatus) {
>>>>>>> refs/remotes/origin/master
		temp = ehci_readl(ehci, status_reg);
		if (tegra->port_resuming && !(temp & PORT_SUSPEND)) {
			/* Resume completed, re-enable disconnect detection */
			tegra->port_resuming = 0;
<<<<<<< HEAD
			tegra_usb_phy_postresume(tegra->phy);
=======
			tegra_usb_phy_postresume(hcd->phy);
>>>>>>> refs/remotes/origin/master
		}
	}

	else if (typeReq == SetPortFeature && wValue == USB_PORT_FEAT_SUSPEND) {
		temp = ehci_readl(ehci, status_reg);
		if ((temp & PORT_PE) == 0 || (temp & PORT_RESET) != 0) {
			retval = -EPIPE;
			goto done;
		}

<<<<<<< HEAD
		temp &= ~PORT_WKCONN_E;
=======
		temp &= ~(PORT_RWC_BITS | PORT_WKCONN_E);
>>>>>>> refs/remotes/origin/master
		temp |= PORT_WKDISC_E | PORT_WKOC_E;
		ehci_writel(ehci, temp | PORT_SUSPEND, status_reg);

		/*
		 * If a transaction is in progress, there may be a delay in
		 * suspending the port. Poll until the port is suspended.
		 */
<<<<<<< HEAD
		if (handshake(ehci, status_reg, PORT_SUSPEND,
=======
		if (ehci_handshake(ehci, status_reg, PORT_SUSPEND,
>>>>>>> refs/remotes/origin/master
						PORT_SUSPEND, 5000))
			pr_err("%s: timeout waiting for SUSPEND\n", __func__);

		set_bit((wIndex & 0xff) - 1, &ehci->suspended_ports);
		goto done;
	}

	/* For USB1 port we need to issue Port Reset twice internally */
<<<<<<< HEAD
	if (tegra->phy->instance == 0 &&
=======
	if (tegra->needs_double_reset &&
>>>>>>> refs/remotes/origin/master
	   (typeReq == SetPortFeature && wValue == USB_PORT_FEAT_RESET)) {
		spin_unlock_irqrestore(&ehci->lock, flags);
		return tegra_ehci_internal_port_reset(ehci, status_reg);
	}

	/*
	 * Tegra host controller will time the resume operation to clear the bit
	 * when the port control state switches to HS or FS Idle. This behavior
	 * is different from EHCI where the host controller driver is required
	 * to set this bit to a zero after the resume duration is timed in the
	 * driver.
	 */
	else if (typeReq == ClearPortFeature &&
					wValue == USB_PORT_FEAT_SUSPEND) {
		temp = ehci_readl(ehci, status_reg);
		if ((temp & PORT_RESET) || !(temp & PORT_PE)) {
			retval = -EPIPE;
			goto done;
		}

		if (!(temp & PORT_SUSPEND))
			goto done;

		/* Disable disconnect detection during port resume */
<<<<<<< HEAD
		tegra_usb_phy_preresume(tegra->phy);
=======
		tegra_usb_phy_preresume(hcd->phy);
>>>>>>> refs/remotes/origin/master

		ehci->reset_done[wIndex-1] = jiffies + msecs_to_jiffies(25);

		temp &= ~(PORT_RWC_BITS | PORT_WAKE_BITS);
		/* start resume signalling */
		ehci_writel(ehci, temp | PORT_RESUME, status_reg);
<<<<<<< HEAD
<<<<<<< HEAD
=======
		set_bit(wIndex-1, &ehci->resuming_ports);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		set_bit(wIndex-1, &ehci->resuming_ports);
>>>>>>> refs/remotes/origin/master

		spin_unlock_irqrestore(&ehci->lock, flags);
		msleep(20);
		spin_lock_irqsave(&ehci->lock, flags);

		/* Poll until the controller clears RESUME and SUSPEND */
<<<<<<< HEAD
		if (handshake(ehci, status_reg, PORT_RESUME, 0, 2000))
			pr_err("%s: timeout waiting for RESUME\n", __func__);
		if (handshake(ehci, status_reg, PORT_SUSPEND, 0, 2000))
			pr_err("%s: timeout waiting for SUSPEND\n", __func__);

		ehci->reset_done[wIndex-1] = 0;
<<<<<<< HEAD
=======
		clear_bit(wIndex-1, &ehci->resuming_ports);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (ehci_handshake(ehci, status_reg, PORT_RESUME, 0, 2000))
			pr_err("%s: timeout waiting for RESUME\n", __func__);
		if (ehci_handshake(ehci, status_reg, PORT_SUSPEND, 0, 2000))
			pr_err("%s: timeout waiting for SUSPEND\n", __func__);

		ehci->reset_done[wIndex-1] = 0;
		clear_bit(wIndex-1, &ehci->resuming_ports);
>>>>>>> refs/remotes/origin/master

		tegra->port_resuming = 1;
		goto done;
	}

	spin_unlock_irqrestore(&ehci->lock, flags);

	/* Handle the hub control events here */
<<<<<<< HEAD
	return ehci_hub_control(hcd, typeReq, wValue, wIndex, buf, wLength);
=======
	return orig_hub_control(hcd, typeReq, wValue, wIndex, buf, wLength);

>>>>>>> refs/remotes/origin/master
done:
	spin_unlock_irqrestore(&ehci->lock, flags);
	return retval;
}

<<<<<<< HEAD
static void tegra_ehci_restart(struct usb_hcd *hcd)
{
	struct ehci_hcd *ehci = hcd_to_ehci(hcd);

	ehci_reset(ehci);

	/* setup the frame list and Async q heads */
	ehci_writel(ehci, ehci->periodic_dma, &ehci->regs->frame_list);
	ehci_writel(ehci, (u32)ehci->async->qh_dma, &ehci->regs->async_next);
	/* setup the command register and set the controller in RUN mode */
	ehci->command &= ~(CMD_LRESET|CMD_IAAD|CMD_PSE|CMD_ASE|CMD_RESET);
	ehci->command |= CMD_RUN;
	ehci_writel(ehci, ehci->command, &ehci->regs->command);

	down_write(&ehci_cf_port_reset_rwsem);
	ehci_writel(ehci, FLAG_CF, &ehci->regs->configured_flag);
	/* flush posted writes */
	ehci_readl(ehci, &ehci->regs->command);
	up_write(&ehci_cf_port_reset_rwsem);
}

<<<<<<< HEAD
static int tegra_usb_suspend(struct usb_hcd *hcd)
{
	struct tegra_ehci_hcd *tegra = dev_get_drvdata(hcd->self.controller);
	struct ehci_regs __iomem *hw = tegra->ehci->regs;
	unsigned long flags;

	spin_lock_irqsave(&tegra->ehci->lock, flags);

	tegra->port_speed = (readl(&hw->port_status[0]) >> 26) & 0x3;
	ehci_halt(tegra->ehci);
	clear_bit(HCD_FLAG_HW_ACCESSIBLE, &hcd->flags);

	spin_unlock_irqrestore(&tegra->ehci->lock, flags);

	tegra_ehci_power_down(hcd);
	return 0;
}

static int tegra_usb_resume(struct usb_hcd *hcd)
{
	struct tegra_ehci_hcd *tegra = dev_get_drvdata(hcd->self.controller);
	struct ehci_hcd	*ehci = hcd_to_ehci(hcd);
	struct ehci_regs __iomem *hw = ehci->regs;
	unsigned long val;

	set_bit(HCD_FLAG_HW_ACCESSIBLE, &hcd->flags);
	tegra_ehci_power_up(hcd);

	if (tegra->port_speed > TEGRA_USB_PHY_PORT_SPEED_HIGH) {
		/* Wait for the phy to detect new devices
		 * before we restart the controller */
		msleep(10);
		goto restart;
	}

	/* Force the phy to keep data lines in suspend state */
	tegra_ehci_phy_restore_start(tegra->phy, tegra->port_speed);

	/* Enable host mode */
	tdi_reset(ehci);

	/* Enable Port Power */
	val = readl(&hw->port_status[0]);
	val |= PORT_POWER;
	writel(val, &hw->port_status[0]);
	udelay(10);

	/* Check if the phy resume from LP0. When the phy resume from LP0
	 * USB register will be reset. */
	if (!readl(&hw->async_next)) {
		/* Program the field PTC based on the saved speed mode */
		val = readl(&hw->port_status[0]);
		val &= ~PORT_TEST(~0);
		if (tegra->port_speed == TEGRA_USB_PHY_PORT_SPEED_HIGH)
			val |= PORT_TEST_FORCE;
		else if (tegra->port_speed == TEGRA_USB_PHY_PORT_SPEED_FULL)
			val |= PORT_TEST(6);
		else if (tegra->port_speed == TEGRA_USB_PHY_PORT_SPEED_LOW)
			val |= PORT_TEST(7);
		writel(val, &hw->port_status[0]);
		udelay(10);

		/* Disable test mode by setting PTC field to NORMAL_OP */
		val = readl(&hw->port_status[0]);
		val &= ~PORT_TEST(~0);
		writel(val, &hw->port_status[0]);
		udelay(10);
	}

	/* Poll until CCS is enabled */
	if (handshake(ehci, &hw->port_status[0], PORT_CONNECT,
						 PORT_CONNECT, 2000)) {
		pr_err("%s: timeout waiting for PORT_CONNECT\n", __func__);
		goto restart;
	}

	/* Poll until PE is enabled */
	if (handshake(ehci, &hw->port_status[0], PORT_PE,
						 PORT_PE, 2000)) {
		pr_err("%s: timeout waiting for USB_PORTSC1_PE\n", __func__);
		goto restart;
	}

	/* Clear the PCI status, to avoid an interrupt taken upon resume */
	val = readl(&hw->status);
	val |= STS_PCD;
	writel(val, &hw->status);

	/* Put controller in suspend mode by writing 1 to SUSP bit of PORTSC */
	val = readl(&hw->port_status[0]);
	if ((val & PORT_POWER) && (val & PORT_PE)) {
		val |= PORT_SUSPEND;
		writel(val, &hw->port_status[0]);

		/* Wait until port suspend completes */
		if (handshake(ehci, &hw->port_status[0], PORT_SUSPEND,
							 PORT_SUSPEND, 1000)) {
			pr_err("%s: timeout waiting for PORT_SUSPEND\n",
								__func__);
			goto restart;
		}
	}

	tegra_ehci_phy_restore_end(tegra->phy);
	return 0;

restart:
	if (tegra->port_speed <= TEGRA_USB_PHY_PORT_SPEED_HIGH)
		tegra_ehci_phy_restore_end(tegra->phy);

	tegra_ehci_restart(hcd);
	return 0;
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
static void tegra_ehci_shutdown(struct usb_hcd *hcd)
{
	struct tegra_ehci_hcd *tegra = dev_get_drvdata(hcd->self.controller);

	/* ehci_shutdown touches the USB controller registers, make sure
	 * controller has clocks to it */
	if (!tegra->host_resumed)
		tegra_ehci_power_up(hcd);

	ehci_shutdown(hcd);
}

static int tegra_ehci_setup(struct usb_hcd *hcd)
{
	struct ehci_hcd *ehci = hcd_to_ehci(hcd);
	int retval;

	/* EHCI registers start at offset 0x100 */
	ehci->caps = hcd->regs + 0x100;
	ehci->regs = hcd->regs + 0x100 +
		HC_LENGTH(ehci, readl(&ehci->caps->hc_capbase));

	dbg_hcs_params(ehci, "reset");
	dbg_hcc_params(ehci, "reset");

	/* cache this readonly data; minimize chip reads */
	ehci->hcs_params = readl(&ehci->caps->hcs_params);

	/* switch to host mode */
	hcd->has_tt = 1;
	ehci_reset(ehci);

	retval = ehci_halt(ehci);
	if (retval)
		return retval;

	/* data structure init */
	retval = ehci_init(hcd);
	if (retval)
		return retval;

	ehci->sbrn = 0x20;

	ehci_port_power(ehci, 1);
	return retval;
}

<<<<<<< HEAD
#ifdef CONFIG_PM
static int tegra_ehci_bus_suspend(struct usb_hcd *hcd)
{
	struct tegra_ehci_hcd *tegra = dev_get_drvdata(hcd->self.controller);
	int error_status = 0;

	error_status = ehci_bus_suspend(hcd);
	if (!error_status && tegra->power_down_on_bus_suspend) {
		tegra_usb_suspend(hcd);
		tegra->bus_suspended = 1;
	}

	return error_status;
}

static int tegra_ehci_bus_resume(struct usb_hcd *hcd)
{
	struct tegra_ehci_hcd *tegra = dev_get_drvdata(hcd->self.controller);

	if (tegra->bus_suspended && tegra->power_down_on_bus_suspend) {
		tegra_usb_resume(hcd);
		tegra->bus_suspended = 0;
	}

	tegra_usb_phy_preresume(tegra->phy);
	tegra->port_resuming = 1;
	return ehci_bus_resume(hcd);
}
#endif

=======
>>>>>>> refs/remotes/origin/cm-10.0
struct temp_buffer {
=======
struct dma_aligned_buffer {
>>>>>>> refs/remotes/origin/master
	void *kmalloc_ptr;
	void *old_xfer_buffer;
	u8 data[0];
};

<<<<<<< HEAD
static void free_temp_buffer(struct urb *urb)
{
	enum dma_data_direction dir;
	struct temp_buffer *temp;
=======
static void free_dma_aligned_buffer(struct urb *urb)
{
	struct dma_aligned_buffer *temp;
>>>>>>> refs/remotes/origin/master

	if (!(urb->transfer_flags & URB_ALIGNED_TEMP_BUFFER))
		return;

<<<<<<< HEAD
	dir = usb_urb_dir_in(urb) ? DMA_FROM_DEVICE : DMA_TO_DEVICE;

	temp = container_of(urb->transfer_buffer, struct temp_buffer,
			    data);

	if (dir == DMA_FROM_DEVICE)
=======
	temp = container_of(urb->transfer_buffer,
		struct dma_aligned_buffer, data);

	if (usb_urb_dir_in(urb))
>>>>>>> refs/remotes/origin/master
		memcpy(temp->old_xfer_buffer, temp->data,
		       urb->transfer_buffer_length);
	urb->transfer_buffer = temp->old_xfer_buffer;
	kfree(temp->kmalloc_ptr);

	urb->transfer_flags &= ~URB_ALIGNED_TEMP_BUFFER;
}

<<<<<<< HEAD
static int alloc_temp_buffer(struct urb *urb, gfp_t mem_flags)
{
	enum dma_data_direction dir;
	struct temp_buffer *temp, *kmalloc_ptr;
=======
static int alloc_dma_aligned_buffer(struct urb *urb, gfp_t mem_flags)
{
	struct dma_aligned_buffer *temp, *kmalloc_ptr;
>>>>>>> refs/remotes/origin/master
	size_t kmalloc_size;

	if (urb->num_sgs || urb->sg ||
	    urb->transfer_buffer_length == 0 ||
	    !((uintptr_t)urb->transfer_buffer & (TEGRA_USB_DMA_ALIGN - 1)))
		return 0;

<<<<<<< HEAD
	dir = usb_urb_dir_in(urb) ? DMA_FROM_DEVICE : DMA_TO_DEVICE;

	/* Allocate a buffer with enough padding for alignment */
	kmalloc_size = urb->transfer_buffer_length +
		sizeof(struct temp_buffer) + TEGRA_USB_DMA_ALIGN - 1;
=======
	/* Allocate a buffer with enough padding for alignment */
	kmalloc_size = urb->transfer_buffer_length +
		sizeof(struct dma_aligned_buffer) + TEGRA_USB_DMA_ALIGN - 1;
>>>>>>> refs/remotes/origin/master

	kmalloc_ptr = kmalloc(kmalloc_size, mem_flags);
	if (!kmalloc_ptr)
		return -ENOMEM;

<<<<<<< HEAD
	/* Position our struct temp_buffer such that data is aligned */
	temp = PTR_ALIGN(kmalloc_ptr + 1, TEGRA_USB_DMA_ALIGN) - 1;

	temp->kmalloc_ptr = kmalloc_ptr;
	temp->old_xfer_buffer = urb->transfer_buffer;
	if (dir == DMA_TO_DEVICE)
=======
	/* Position our struct dma_aligned_buffer such that data is aligned */
	temp = PTR_ALIGN(kmalloc_ptr + 1, TEGRA_USB_DMA_ALIGN) - 1;
	temp->kmalloc_ptr = kmalloc_ptr;
	temp->old_xfer_buffer = urb->transfer_buffer;
	if (usb_urb_dir_out(urb))
>>>>>>> refs/remotes/origin/master
		memcpy(temp->data, urb->transfer_buffer,
		       urb->transfer_buffer_length);
	urb->transfer_buffer = temp->data;

	urb->transfer_flags |= URB_ALIGNED_TEMP_BUFFER;

	return 0;
}

static int tegra_ehci_map_urb_for_dma(struct usb_hcd *hcd, struct urb *urb,
				      gfp_t mem_flags)
{
	int ret;

<<<<<<< HEAD
	ret = alloc_temp_buffer(urb, mem_flags);
=======
	ret = alloc_dma_aligned_buffer(urb, mem_flags);
>>>>>>> refs/remotes/origin/master
	if (ret)
		return ret;

	ret = usb_hcd_map_urb_for_dma(hcd, urb, mem_flags);
	if (ret)
<<<<<<< HEAD
		free_temp_buffer(urb);
=======
		free_dma_aligned_buffer(urb);
>>>>>>> refs/remotes/origin/master

	return ret;
}

static void tegra_ehci_unmap_urb_for_dma(struct usb_hcd *hcd, struct urb *urb)
{
	usb_hcd_unmap_urb_for_dma(hcd, urb);
<<<<<<< HEAD
	free_temp_buffer(urb);
}

static const struct hc_driver tegra_ehci_hc_driver = {
	.description		= hcd_name,
	.product_desc		= "Tegra EHCI Host Controller",
	.hcd_priv_size		= sizeof(struct ehci_hcd),

	.flags			= HCD_USB2 | HCD_MEMORY,

	.reset			= tegra_ehci_setup,
	.irq			= ehci_irq,

	.start			= ehci_run,
	.stop			= ehci_stop,
	.shutdown		= tegra_ehci_shutdown,
	.urb_enqueue		= ehci_urb_enqueue,
	.urb_dequeue		= ehci_urb_dequeue,
	.map_urb_for_dma	= tegra_ehci_map_urb_for_dma,
	.unmap_urb_for_dma	= tegra_ehci_unmap_urb_for_dma,
	.endpoint_disable	= ehci_endpoint_disable,
	.endpoint_reset		= ehci_endpoint_reset,
	.get_frame_number	= ehci_get_frame,
	.hub_status_data	= ehci_hub_status_data,
	.hub_control		= tegra_ehci_hub_control,
	.clear_tt_buffer_complete = ehci_clear_tt_buffer_complete,
#ifdef CONFIG_PM
<<<<<<< HEAD
	.bus_suspend		= tegra_ehci_bus_suspend,
	.bus_resume		= tegra_ehci_bus_resume,
=======
	.bus_suspend		= ehci_bus_suspend,
	.bus_resume		= ehci_bus_resume,
>>>>>>> refs/remotes/origin/cm-10.0
#endif
	.relinquish_port	= ehci_relinquish_port,
	.port_handed_over	= ehci_port_handed_over,
};

<<<<<<< HEAD
=======
static int setup_vbus_gpio(struct platform_device *pdev)
{
	int err = 0;
	int gpio;

	if (!pdev->dev.of_node)
		return 0;

	gpio = of_get_named_gpio(pdev->dev.of_node, "nvidia,vbus-gpio", 0);
	if (!gpio_is_valid(gpio))
		return 0;

	err = gpio_request(gpio, "vbus_gpio");
	if (err) {
		dev_err(&pdev->dev, "can't request vbus gpio %d", gpio);
		return err;
	}
	err = gpio_direction_output(gpio, 1);
	if (err) {
		dev_err(&pdev->dev, "can't enable vbus\n");
		return err;
	}

	return err;
}

#ifdef CONFIG_PM

static int controller_suspend(struct device *dev)
{
	struct tegra_ehci_hcd *tegra =
			platform_get_drvdata(to_platform_device(dev));
	struct ehci_hcd	*ehci = tegra->ehci;
	struct usb_hcd *hcd = ehci_to_hcd(ehci);
	struct ehci_regs __iomem *hw = ehci->regs;
	unsigned long flags;

	if (time_before(jiffies, ehci->next_statechange))
		msleep(10);

	spin_lock_irqsave(&ehci->lock, flags);

	tegra->port_speed = (readl(&hw->port_status[0]) >> 26) & 0x3;
	ehci_halt(ehci);
	clear_bit(HCD_FLAG_HW_ACCESSIBLE, &hcd->flags);

	spin_unlock_irqrestore(&ehci->lock, flags);

	tegra_ehci_power_down(hcd);
	return 0;
}

static int controller_resume(struct device *dev)
{
	struct tegra_ehci_hcd *tegra =
			platform_get_drvdata(to_platform_device(dev));
	struct ehci_hcd	*ehci = tegra->ehci;
	struct usb_hcd *hcd = ehci_to_hcd(ehci);
	struct ehci_regs __iomem *hw = ehci->regs;
	unsigned long val;

	set_bit(HCD_FLAG_HW_ACCESSIBLE, &hcd->flags);
	tegra_ehci_power_up(hcd);

	if (tegra->port_speed > TEGRA_USB_PHY_PORT_SPEED_HIGH) {
		/* Wait for the phy to detect new devices
		 * before we restart the controller */
		msleep(10);
		goto restart;
	}

	/* Force the phy to keep data lines in suspend state */
	tegra_ehci_phy_restore_start(tegra->phy, tegra->port_speed);

	/* Enable host mode */
	tdi_reset(ehci);

	/* Enable Port Power */
	val = readl(&hw->port_status[0]);
	val |= PORT_POWER;
	writel(val, &hw->port_status[0]);
	udelay(10);

	/* Check if the phy resume from LP0. When the phy resume from LP0
	 * USB register will be reset. */
	if (!readl(&hw->async_next)) {
		/* Program the field PTC based on the saved speed mode */
		val = readl(&hw->port_status[0]);
		val &= ~PORT_TEST(~0);
		if (tegra->port_speed == TEGRA_USB_PHY_PORT_SPEED_HIGH)
			val |= PORT_TEST_FORCE;
		else if (tegra->port_speed == TEGRA_USB_PHY_PORT_SPEED_FULL)
			val |= PORT_TEST(6);
		else if (tegra->port_speed == TEGRA_USB_PHY_PORT_SPEED_LOW)
			val |= PORT_TEST(7);
		writel(val, &hw->port_status[0]);
		udelay(10);

		/* Disable test mode by setting PTC field to NORMAL_OP */
		val = readl(&hw->port_status[0]);
		val &= ~PORT_TEST(~0);
		writel(val, &hw->port_status[0]);
		udelay(10);
	}

	/* Poll until CCS is enabled */
	if (handshake(ehci, &hw->port_status[0], PORT_CONNECT,
						 PORT_CONNECT, 2000)) {
		pr_err("%s: timeout waiting for PORT_CONNECT\n", __func__);
		goto restart;
	}

	/* Poll until PE is enabled */
	if (handshake(ehci, &hw->port_status[0], PORT_PE,
						 PORT_PE, 2000)) {
		pr_err("%s: timeout waiting for USB_PORTSC1_PE\n", __func__);
		goto restart;
	}

	/* Clear the PCI status, to avoid an interrupt taken upon resume */
	val = readl(&hw->status);
	val |= STS_PCD;
	writel(val, &hw->status);

	/* Put controller in suspend mode by writing 1 to SUSP bit of PORTSC */
	val = readl(&hw->port_status[0]);
	if ((val & PORT_POWER) && (val & PORT_PE)) {
		val |= PORT_SUSPEND;
		writel(val, &hw->port_status[0]);

		/* Wait until port suspend completes */
		if (handshake(ehci, &hw->port_status[0], PORT_SUSPEND,
							 PORT_SUSPEND, 1000)) {
			pr_err("%s: timeout waiting for PORT_SUSPEND\n",
								__func__);
			goto restart;
		}
	}

	tegra_ehci_phy_restore_end(tegra->phy);
	goto done;

 restart:
	if (tegra->port_speed <= TEGRA_USB_PHY_PORT_SPEED_HIGH)
		tegra_ehci_phy_restore_end(tegra->phy);

	tegra_ehci_restart(hcd);

 done:
	tegra_usb_phy_preresume(tegra->phy);
	tegra->port_resuming = 1;
	return 0;
}

static int tegra_ehci_suspend(struct device *dev)
{
	struct tegra_ehci_hcd *tegra =
			platform_get_drvdata(to_platform_device(dev));
	struct usb_hcd *hcd = ehci_to_hcd(tegra->ehci);
	int rc = 0;

	/*
	 * When system sleep is supported and USB controller wakeup is
	 * implemented: If the controller is runtime-suspended and the
	 * wakeup setting needs to be changed, call pm_runtime_resume().
	 */
	if (HCD_HW_ACCESSIBLE(hcd))
		rc = controller_suspend(dev);
	return rc;
}

static int tegra_ehci_resume(struct device *dev)
{
	int rc;

	rc = controller_resume(dev);
	if (rc == 0) {
		pm_runtime_disable(dev);
		pm_runtime_set_active(dev);
		pm_runtime_enable(dev);
	}
	return rc;
}

static int tegra_ehci_runtime_suspend(struct device *dev)
{
	return controller_suspend(dev);
}

static int tegra_ehci_runtime_resume(struct device *dev)
{
	return controller_resume(dev);
}

static const struct dev_pm_ops tegra_ehci_pm_ops = {
	.suspend	= tegra_ehci_suspend,
	.resume		= tegra_ehci_resume,
	.runtime_suspend = tegra_ehci_runtime_suspend,
	.runtime_resume	= tegra_ehci_runtime_resume,
};

#endif

static u64 tegra_ehci_dma_mask = DMA_BIT_MASK(32);

>>>>>>> refs/remotes/origin/cm-10.0
static int tegra_ehci_probe(struct platform_device *pdev)
{
	struct resource *res;
	struct usb_hcd *hcd;
	struct tegra_ehci_hcd *tegra;
	struct tegra_ehci_platform_data *pdata;
	int err = 0;
	int irq;
	int instance = pdev->id;

	pdata = pdev->dev.platform_data;
	if (!pdata) {
		dev_err(&pdev->dev, "Platform data missing\n");
		return -EINVAL;
	}

<<<<<<< HEAD
=======
=======
	free_dma_aligned_buffer(urb);
}

static const struct tegra_ehci_soc_config tegra30_soc_config = {
	.has_hostpc = true,
};

static const struct tegra_ehci_soc_config tegra20_soc_config = {
	.has_hostpc = false,
};

static struct of_device_id tegra_ehci_of_match[] = {
	{ .compatible = "nvidia,tegra30-ehci", .data = &tegra30_soc_config },
	{ .compatible = "nvidia,tegra20-ehci", .data = &tegra20_soc_config },
	{ },
};

static int tegra_ehci_probe(struct platform_device *pdev)
{
	const struct of_device_id *match;
	const struct tegra_ehci_soc_config *soc_config;
	struct resource *res;
	struct usb_hcd *hcd;
	struct ehci_hcd *ehci;
	struct tegra_ehci_hcd *tegra;
	int err = 0;
	int irq;
	struct usb_phy *u_phy;

	match = of_match_device(tegra_ehci_of_match, &pdev->dev);
	if (!match) {
		dev_err(&pdev->dev, "Error: No device match found\n");
		return -ENODEV;
	}
	soc_config = match->data;

>>>>>>> refs/remotes/origin/master
	/* Right now device-tree probed devices don't get dma_mask set.
	 * Since shared usb code relies on it, set it here for now.
	 * Once we have dma capability bindings this can go away.
	 */
<<<<<<< HEAD
	if (!pdev->dev.dma_mask)
		pdev->dev.dma_mask = &tegra_ehci_dma_mask;

	setup_vbus_gpio(pdev);

>>>>>>> refs/remotes/origin/cm-10.0
	tegra = kzalloc(sizeof(struct tegra_ehci_hcd), GFP_KERNEL);
	if (!tegra)
		return -ENOMEM;
=======
	err = dma_coerce_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(32));
	if (err)
		return err;
>>>>>>> refs/remotes/origin/master

	hcd = usb_create_hcd(&tegra_ehci_hc_driver, &pdev->dev,
					dev_name(&pdev->dev));
	if (!hcd) {
		dev_err(&pdev->dev, "Unable to create HCD\n");
<<<<<<< HEAD
		err = -ENOMEM;
		goto fail_hcd;
	}

	platform_set_drvdata(pdev, tegra);

	tegra->clk = clk_get(&pdev->dev, NULL);
	if (IS_ERR(tegra->clk)) {
		dev_err(&pdev->dev, "Can't get ehci clock\n");
		err = PTR_ERR(tegra->clk);
		goto fail_clk;
	}

	err = clk_enable(tegra->clk);
	if (err)
		goto fail_clken;

	tegra->emc_clk = clk_get(&pdev->dev, "emc");
	if (IS_ERR(tegra->emc_clk)) {
		dev_err(&pdev->dev, "Can't get emc clock\n");
		err = PTR_ERR(tegra->emc_clk);
		goto fail_emc_clk;
	}

	clk_enable(tegra->emc_clk);
	clk_set_rate(tegra->emc_clk, 400000000);
=======
		return -ENOMEM;
	}
	platform_set_drvdata(pdev, hcd);
	ehci = hcd_to_ehci(hcd);
	tegra = (struct tegra_ehci_hcd *)ehci->priv;

	hcd->has_tt = 1;

	tegra->clk = devm_clk_get(&pdev->dev, NULL);
	if (IS_ERR(tegra->clk)) {
		dev_err(&pdev->dev, "Can't get ehci clock\n");
		err = PTR_ERR(tegra->clk);
		goto cleanup_hcd_create;
	}

	err = clk_prepare_enable(tegra->clk);
	if (err)
		goto cleanup_hcd_create;

	tegra_periph_reset_assert(tegra->clk);
	udelay(1);
	tegra_periph_reset_deassert(tegra->clk);

	u_phy = devm_usb_get_phy_by_phandle(&pdev->dev, "nvidia,phy", 0);
	if (IS_ERR(u_phy)) {
		err = PTR_ERR(u_phy);
		goto cleanup_clk_en;
	}
	hcd->phy = u_phy;

	tegra->needs_double_reset = of_property_read_bool(pdev->dev.of_node,
		"nvidia,needs-double-reset");
>>>>>>> refs/remotes/origin/master

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev, "Failed to get I/O memory\n");
		err = -ENXIO;
<<<<<<< HEAD
		goto fail_io;
	}
	hcd->rsrc_start = res->start;
	hcd->rsrc_len = resource_size(res);
	hcd->regs = ioremap(res->start, resource_size(res));
	if (!hcd->regs) {
		dev_err(&pdev->dev, "Failed to remap I/O memory\n");
		err = -ENOMEM;
		goto fail_io;
	}

<<<<<<< HEAD
=======
	/* This is pretty ugly and needs to be fixed when we do only
	 * device-tree probing. Old code relies on the platform_device
	 * numbering that we lack for device-tree-instantiated devices.
	 */
	if (instance < 0) {
		switch (res->start) {
		case TEGRA_USB_BASE:
			instance = 0;
			break;
		case TEGRA_USB2_BASE:
			instance = 1;
			break;
		case TEGRA_USB3_BASE:
			instance = 2;
			break;
		default:
			err = -ENODEV;
			dev_err(&pdev->dev, "unknown usb instance\n");
			goto fail_phy;
		}
	}

>>>>>>> refs/remotes/origin/cm-10.0
	tegra->phy = tegra_usb_phy_open(instance, hcd->regs, pdata->phy_config,
						TEGRA_USB_PHY_MODE_HOST);
	if (IS_ERR(tegra->phy)) {
		dev_err(&pdev->dev, "Failed to open USB phy\n");
		err = -ENXIO;
		goto fail_phy;
	}

	err = tegra_usb_phy_power_on(tegra->phy);
	if (err) {
		dev_err(&pdev->dev, "Failed to power on the phy\n");
		goto fail;
	}

	tegra->host_resumed = 1;
<<<<<<< HEAD
	tegra->power_down_on_bus_suspend = pdata->power_down_on_bus_suspend;
=======
>>>>>>> refs/remotes/origin/cm-10.0
	tegra->ehci = hcd_to_ehci(hcd);

=======
		goto cleanup_clk_en;
	}
	hcd->rsrc_start = res->start;
	hcd->rsrc_len = resource_size(res);
	hcd->regs = devm_ioremap(&pdev->dev, res->start, resource_size(res));
	if (!hcd->regs) {
		dev_err(&pdev->dev, "Failed to remap I/O memory\n");
		err = -ENOMEM;
		goto cleanup_clk_en;
	}
	ehci->caps = hcd->regs + 0x100;
	ehci->has_hostpc = soc_config->has_hostpc;

	err = usb_phy_init(hcd->phy);
	if (err) {
		dev_err(&pdev->dev, "Failed to initialize phy\n");
		goto cleanup_clk_en;
	}

	u_phy->otg = devm_kzalloc(&pdev->dev, sizeof(struct usb_otg),
			     GFP_KERNEL);
	if (!u_phy->otg) {
		dev_err(&pdev->dev, "Failed to alloc memory for otg\n");
		err = -ENOMEM;
		goto cleanup_phy;
	}
	u_phy->otg->host = hcd_to_bus(hcd);

	err = usb_phy_set_suspend(hcd->phy, 0);
	if (err) {
		dev_err(&pdev->dev, "Failed to power on the phy\n");
		goto cleanup_phy;
	}

>>>>>>> refs/remotes/origin/master
	irq = platform_get_irq(pdev, 0);
	if (!irq) {
		dev_err(&pdev->dev, "Failed to get IRQ\n");
		err = -ENODEV;
<<<<<<< HEAD
		goto fail;
	}
<<<<<<< HEAD
	set_irq_flags(irq, IRQF_VALID);

#ifdef CONFIG_USB_OTG_UTILS
	if (pdata->operating_mode == TEGRA_USB_OTG) {
		tegra->transceiver = otg_get_transceiver();
		if (tegra->transceiver)
			otg_set_host(tegra->transceiver, &hcd->self);
	}
#endif

	err = usb_add_hcd(hcd, irq, IRQF_DISABLED | IRQF_SHARED);
=======

#ifdef CONFIG_USB_OTG_UTILS
	if (pdata->operating_mode == TEGRA_USB_OTG) {
		tegra->transceiver = usb_get_transceiver();
		if (tegra->transceiver)
			otg_set_host(tegra->transceiver->otg, &hcd->self);
	}
#endif

	err = usb_add_hcd(hcd, irq, IRQF_SHARED);
>>>>>>> refs/remotes/origin/cm-10.0
	if (err) {
		dev_err(&pdev->dev, "Failed to add USB HCD\n");
		goto fail;
	}

<<<<<<< HEAD
=======
	pm_runtime_set_active(&pdev->dev);
	pm_runtime_get_noresume(&pdev->dev);

	/* Don't skip the pm_runtime_forbid call if wakeup isn't working */
	/* if (!pdata->power_down_on_bus_suspend) */
		pm_runtime_forbid(&pdev->dev);
	pm_runtime_enable(&pdev->dev);
	pm_runtime_put_sync(&pdev->dev);
>>>>>>> refs/remotes/origin/cm-10.0
	return err;

fail:
#ifdef CONFIG_USB_OTG_UTILS
	if (tegra->transceiver) {
<<<<<<< HEAD
		otg_set_host(tegra->transceiver, NULL);
		otg_put_transceiver(tegra->transceiver);
=======
		otg_set_host(tegra->transceiver->otg, NULL);
		usb_put_transceiver(tegra->transceiver);
>>>>>>> refs/remotes/origin/cm-10.0
	}
#endif
	tegra_usb_phy_close(tegra->phy);
fail_phy:
	iounmap(hcd->regs);
fail_io:
	clk_disable(tegra->emc_clk);
	clk_put(tegra->emc_clk);
fail_emc_clk:
	clk_disable(tegra->clk);
fail_clken:
	clk_put(tegra->clk);
fail_clk:
	usb_put_hcd(hcd);
fail_hcd:
	kfree(tegra);
	return err;
}

<<<<<<< HEAD
#ifdef CONFIG_PM
static int tegra_ehci_resume(struct platform_device *pdev)
{
	struct tegra_ehci_hcd *tegra = platform_get_drvdata(pdev);
	struct usb_hcd *hcd = ehci_to_hcd(tegra->ehci);

	if (tegra->bus_suspended)
		return 0;

	return tegra_usb_resume(hcd);
}

static int tegra_ehci_suspend(struct platform_device *pdev, pm_message_t state)
{
	struct tegra_ehci_hcd *tegra = platform_get_drvdata(pdev);
	struct usb_hcd *hcd = ehci_to_hcd(tegra->ehci);

	if (tegra->bus_suspended)
		return 0;

	if (time_before(jiffies, tegra->ehci->next_statechange))
		msleep(10);

	return tegra_usb_suspend(hcd);
}
#endif

=======
>>>>>>> refs/remotes/origin/cm-10.0
static int tegra_ehci_remove(struct platform_device *pdev)
{
	struct tegra_ehci_hcd *tegra = platform_get_drvdata(pdev);
	struct usb_hcd *hcd = ehci_to_hcd(tegra->ehci);

	if (tegra == NULL || hcd == NULL)
		return -EINVAL;

<<<<<<< HEAD
#ifdef CONFIG_USB_OTG_UTILS
	if (tegra->transceiver) {
		otg_set_host(tegra->transceiver, NULL);
		otg_put_transceiver(tegra->transceiver);
=======
	pm_runtime_get_sync(&pdev->dev);
	pm_runtime_disable(&pdev->dev);
	pm_runtime_put_noidle(&pdev->dev);

#ifdef CONFIG_USB_OTG_UTILS
	if (tegra->transceiver) {
		otg_set_host(tegra->transceiver->otg, NULL);
		usb_put_transceiver(tegra->transceiver);
>>>>>>> refs/remotes/origin/cm-10.0
	}
#endif

	usb_remove_hcd(hcd);
	usb_put_hcd(hcd);

	tegra_usb_phy_close(tegra->phy);
	iounmap(hcd->regs);

	clk_disable(tegra->clk);
	clk_put(tegra->clk);

	clk_disable(tegra->emc_clk);
	clk_put(tegra->emc_clk);

	kfree(tegra);
=======
		goto cleanup_phy;
	}

	otg_set_host(u_phy->otg, &hcd->self);

	err = usb_add_hcd(hcd, irq, IRQF_SHARED);
	if (err) {
		dev_err(&pdev->dev, "Failed to add USB HCD\n");
		goto cleanup_otg_set_host;
	}
	device_wakeup_enable(hcd->self.controller);

	return err;

cleanup_otg_set_host:
	otg_set_host(u_phy->otg, NULL);
cleanup_phy:
	usb_phy_shutdown(hcd->phy);
cleanup_clk_en:
	clk_disable_unprepare(tegra->clk);
cleanup_hcd_create:
	usb_put_hcd(hcd);
	return err;
}

static int tegra_ehci_remove(struct platform_device *pdev)
{
	struct usb_hcd *hcd = platform_get_drvdata(pdev);
	struct tegra_ehci_hcd *tegra =
		(struct tegra_ehci_hcd *)hcd_to_ehci(hcd)->priv;

	otg_set_host(hcd->phy->otg, NULL);

	usb_phy_shutdown(hcd->phy);
	usb_remove_hcd(hcd);
	usb_put_hcd(hcd);

	clk_disable_unprepare(tegra->clk);

>>>>>>> refs/remotes/origin/master
	return 0;
}

static void tegra_ehci_hcd_shutdown(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct tegra_ehci_hcd *tegra = platform_get_drvdata(pdev);
	struct usb_hcd *hcd = ehci_to_hcd(tegra->ehci);
=======
	struct usb_hcd *hcd = platform_get_drvdata(pdev);
>>>>>>> refs/remotes/origin/master

	if (hcd->driver->shutdown)
		hcd->driver->shutdown(hcd);
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct platform_driver tegra_ehci_driver = {
	.probe		= tegra_ehci_probe,
	.remove		= tegra_ehci_remove,
#ifdef CONFIG_PM
	.suspend	= tegra_ehci_suspend,
	.resume		= tegra_ehci_resume,
#endif
	.shutdown	= tegra_ehci_hcd_shutdown,
	.driver		= {
		.name	= "tegra-ehci",
=======
static struct of_device_id tegra_ehci_of_match[] __devinitdata = {
	{ .compatible = "nvidia,tegra20-ehci", },
	{ },
};

static struct platform_driver tegra_ehci_driver = {
	.probe		= tegra_ehci_probe,
	.remove		= tegra_ehci_remove,
	.shutdown	= tegra_ehci_hcd_shutdown,
	.driver		= {
		.name	= "tegra-ehci",
		.of_match_table = tegra_ehci_of_match,
#ifdef CONFIG_PM
		.pm	= &tegra_ehci_pm_ops,
#endif
>>>>>>> refs/remotes/origin/cm-10.0
	}
};
=======
static struct platform_driver tegra_ehci_driver = {
	.probe		= tegra_ehci_probe,
	.remove		= tegra_ehci_remove,
	.shutdown	= tegra_ehci_hcd_shutdown,
	.driver		= {
		.name	= DRV_NAME,
		.of_match_table = tegra_ehci_of_match,
	}
};

static const struct ehci_driver_overrides tegra_overrides __initconst = {
	.extra_priv_size	= sizeof(struct tegra_ehci_hcd),
};

static int __init ehci_tegra_init(void)
{
	if (usb_disabled())
		return -ENODEV;

	pr_info(DRV_NAME ": " DRIVER_DESC "\n");

	ehci_init_driver(&tegra_ehci_hc_driver, &tegra_overrides);

	/*
	 * The Tegra HW has some unusual quirks, which require Tegra-specific
	 * workarounds. We override certain hc_driver functions here to
	 * achieve that. We explicitly do not enhance ehci_driver_overrides to
	 * allow this more easily, since this is an unusual case, and we don't
	 * want to encourage others to override these functions by making it
	 * too easy.
	 */

	orig_hub_control = tegra_ehci_hc_driver.hub_control;

	tegra_ehci_hc_driver.map_urb_for_dma = tegra_ehci_map_urb_for_dma;
	tegra_ehci_hc_driver.unmap_urb_for_dma = tegra_ehci_unmap_urb_for_dma;
	tegra_ehci_hc_driver.hub_control = tegra_ehci_hub_control;

	return platform_driver_register(&tegra_ehci_driver);
}
module_init(ehci_tegra_init);

static void __exit ehci_tegra_cleanup(void)
{
	platform_driver_unregister(&tegra_ehci_driver);
}
module_exit(ehci_tegra_cleanup);

MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:" DRV_NAME);
MODULE_DEVICE_TABLE(of, tegra_ehci_of_match);
>>>>>>> refs/remotes/origin/master
