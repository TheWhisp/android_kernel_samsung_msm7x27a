/**
 * core.c - DesignWare USB3 DRD Controller Core file
 *
 * Copyright (C) 2010-2011 Texas Instruments Incorporated - http://www.ti.com
 *
 * Authors: Felipe Balbi <balbi@ti.com>,
 *	    Sebastian Andrzej Siewior <bigeasy@linutronix.de>
 *
<<<<<<< HEAD
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer,
 *    without modification.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The names of the above-listed copyright holders may not be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * ALTERNATIVELY, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2, as published by the Free
 * Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
=======
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2  of
 * the License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
>>>>>>> refs/remotes/origin/master
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/io.h>
#include <linux/list.h>
#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <linux/of.h>

#include <linux/usb/ch9.h>
#include <linux/usb/gadget.h>
<<<<<<< HEAD

=======
#include <linux/usb/of.h>
#include <linux/usb/otg.h>

#include "platform_data.h"
>>>>>>> refs/remotes/origin/master
#include "core.h"
#include "gadget.h"
#include "io.h"

#include "debug.h"

<<<<<<< HEAD
static char *maximum_speed = "super";
module_param(maximum_speed, charp, 0);
MODULE_PARM_DESC(maximum_speed, "Maximum supported speed.");

/* -------------------------------------------------------------------------- */

#define DWC3_DEVS_POSSIBLE	32

static DECLARE_BITMAP(dwc3_devs, DWC3_DEVS_POSSIBLE);

int dwc3_get_device_id(void)
{
	int		id;

again:
	id = find_first_zero_bit(dwc3_devs, DWC3_DEVS_POSSIBLE);
	if (id < DWC3_DEVS_POSSIBLE) {
		int old;

		old = test_and_set_bit(id, dwc3_devs);
		if (old)
			goto again;
	} else {
		pr_err("dwc3: no space for new device\n");
		id = -ENOMEM;
	}

	return id;
}
EXPORT_SYMBOL_GPL(dwc3_get_device_id);

void dwc3_put_device_id(int id)
{
	int			ret;

	if (id < 0)
		return;

	ret = test_bit(id, dwc3_devs);
	WARN(!ret, "dwc3: ID %d not in use\n", id);
	clear_bit(id, dwc3_devs);
}
EXPORT_SYMBOL_GPL(dwc3_put_device_id);

=======
/* -------------------------------------------------------------------------- */

>>>>>>> refs/remotes/origin/master
void dwc3_set_mode(struct dwc3 *dwc, u32 mode)
{
	u32 reg;

	reg = dwc3_readl(dwc->regs, DWC3_GCTL);
	reg &= ~(DWC3_GCTL_PRTCAPDIR(DWC3_GCTL_PRTCAP_OTG));
	reg |= DWC3_GCTL_PRTCAPDIR(mode);
	dwc3_writel(dwc->regs, DWC3_GCTL, reg);
}

/**
 * dwc3_core_soft_reset - Issues core soft reset and PHY reset
 * @dwc: pointer to our context structure
 */
static void dwc3_core_soft_reset(struct dwc3 *dwc)
{
	u32		reg;

	/* Before Resetting PHY, put Core in Reset */
	reg = dwc3_readl(dwc->regs, DWC3_GCTL);
	reg |= DWC3_GCTL_CORESOFTRESET;
	dwc3_writel(dwc->regs, DWC3_GCTL, reg);

	/* Assert USB3 PHY reset */
	reg = dwc3_readl(dwc->regs, DWC3_GUSB3PIPECTL(0));
	reg |= DWC3_GUSB3PIPECTL_PHYSOFTRST;
	dwc3_writel(dwc->regs, DWC3_GUSB3PIPECTL(0), reg);

	/* Assert USB2 PHY reset */
	reg = dwc3_readl(dwc->regs, DWC3_GUSB2PHYCFG(0));
	reg |= DWC3_GUSB2PHYCFG_PHYSOFTRST;
	dwc3_writel(dwc->regs, DWC3_GUSB2PHYCFG(0), reg);

<<<<<<< HEAD
=======
	usb_phy_init(dwc->usb2_phy);
	usb_phy_init(dwc->usb3_phy);
>>>>>>> refs/remotes/origin/master
	mdelay(100);

	/* Clear USB3 PHY reset */
	reg = dwc3_readl(dwc->regs, DWC3_GUSB3PIPECTL(0));
	reg &= ~DWC3_GUSB3PIPECTL_PHYSOFTRST;
	dwc3_writel(dwc->regs, DWC3_GUSB3PIPECTL(0), reg);

	/* Clear USB2 PHY reset */
	reg = dwc3_readl(dwc->regs, DWC3_GUSB2PHYCFG(0));
	reg &= ~DWC3_GUSB2PHYCFG_PHYSOFTRST;
	dwc3_writel(dwc->regs, DWC3_GUSB2PHYCFG(0), reg);

<<<<<<< HEAD
=======
	mdelay(100);

>>>>>>> refs/remotes/origin/master
	/* After PHYs are stable we can take Core out of reset state */
	reg = dwc3_readl(dwc->regs, DWC3_GCTL);
	reg &= ~DWC3_GCTL_CORESOFTRESET;
	dwc3_writel(dwc->regs, DWC3_GCTL, reg);
}

/**
 * dwc3_free_one_event_buffer - Frees one event buffer
 * @dwc: Pointer to our controller context structure
 * @evt: Pointer to event buffer to be freed
 */
static void dwc3_free_one_event_buffer(struct dwc3 *dwc,
		struct dwc3_event_buffer *evt)
{
	dma_free_coherent(dwc->dev, evt->length, evt->buf, evt->dma);
<<<<<<< HEAD
	kfree(evt);
=======
>>>>>>> refs/remotes/origin/master
}

/**
 * dwc3_alloc_one_event_buffer - Allocates one event buffer structure
 * @dwc: Pointer to our controller context structure
 * @length: size of the event buffer
 *
 * Returns a pointer to the allocated event buffer structure on success
 * otherwise ERR_PTR(errno).
 */
<<<<<<< HEAD
static struct dwc3_event_buffer *__devinit
dwc3_alloc_one_event_buffer(struct dwc3 *dwc, unsigned length)
{
	struct dwc3_event_buffer	*evt;

	evt = kzalloc(sizeof(*evt), GFP_KERNEL);
=======
static struct dwc3_event_buffer *dwc3_alloc_one_event_buffer(struct dwc3 *dwc,
		unsigned length)
{
	struct dwc3_event_buffer	*evt;

	evt = devm_kzalloc(dwc->dev, sizeof(*evt), GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (!evt)
		return ERR_PTR(-ENOMEM);

	evt->dwc	= dwc;
	evt->length	= length;
	evt->buf	= dma_alloc_coherent(dwc->dev, length,
			&evt->dma, GFP_KERNEL);
<<<<<<< HEAD
	if (!evt->buf) {
		kfree(evt);
		return ERR_PTR(-ENOMEM);
	}
=======
	if (!evt->buf)
		return ERR_PTR(-ENOMEM);
>>>>>>> refs/remotes/origin/master

	return evt;
}

/**
 * dwc3_free_event_buffers - frees all allocated event buffers
 * @dwc: Pointer to our controller context structure
 */
static void dwc3_free_event_buffers(struct dwc3 *dwc)
{
	struct dwc3_event_buffer	*evt;
	int i;

	for (i = 0; i < dwc->num_event_buffers; i++) {
		evt = dwc->ev_buffs[i];
		if (evt)
			dwc3_free_one_event_buffer(dwc, evt);
	}
<<<<<<< HEAD

	kfree(dwc->ev_buffs);
=======
>>>>>>> refs/remotes/origin/master
}

/**
 * dwc3_alloc_event_buffers - Allocates @num event buffers of size @length
 * @dwc: pointer to our controller context structure
 * @length: size of event buffer
 *
 * Returns 0 on success otherwise negative errno. In the error case, dwc
 * may contain some buffers allocated but not all which were requested.
 */
<<<<<<< HEAD
static int __devinit dwc3_alloc_event_buffers(struct dwc3 *dwc, unsigned length)
=======
static int dwc3_alloc_event_buffers(struct dwc3 *dwc, unsigned length)
>>>>>>> refs/remotes/origin/master
{
	int			num;
	int			i;

	num = DWC3_NUM_INT(dwc->hwparams.hwparams1);
	dwc->num_event_buffers = num;

<<<<<<< HEAD
	dwc->ev_buffs = kzalloc(sizeof(*dwc->ev_buffs) * num, GFP_KERNEL);
=======
	dwc->ev_buffs = devm_kzalloc(dwc->dev, sizeof(*dwc->ev_buffs) * num,
			GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (!dwc->ev_buffs) {
		dev_err(dwc->dev, "can't allocate event buffers array\n");
		return -ENOMEM;
	}

	for (i = 0; i < num; i++) {
		struct dwc3_event_buffer	*evt;

		evt = dwc3_alloc_one_event_buffer(dwc, length);
		if (IS_ERR(evt)) {
			dev_err(dwc->dev, "can't allocate event buffer\n");
			return PTR_ERR(evt);
		}
		dwc->ev_buffs[i] = evt;
	}

	return 0;
}

/**
 * dwc3_event_buffers_setup - setup our allocated event buffers
 * @dwc: pointer to our controller context structure
 *
 * Returns 0 on success otherwise negative errno.
 */
<<<<<<< HEAD
static int __devinit dwc3_event_buffers_setup(struct dwc3 *dwc)
=======
static int dwc3_event_buffers_setup(struct dwc3 *dwc)
>>>>>>> refs/remotes/origin/master
{
	struct dwc3_event_buffer	*evt;
	int				n;

	for (n = 0; n < dwc->num_event_buffers; n++) {
		evt = dwc->ev_buffs[n];
		dev_dbg(dwc->dev, "Event buf %p dma %08llx length %d\n",
				evt->buf, (unsigned long long) evt->dma,
				evt->length);

<<<<<<< HEAD
=======
		evt->lpos = 0;

>>>>>>> refs/remotes/origin/master
		dwc3_writel(dwc->regs, DWC3_GEVNTADRLO(n),
				lower_32_bits(evt->dma));
		dwc3_writel(dwc->regs, DWC3_GEVNTADRHI(n),
				upper_32_bits(evt->dma));
		dwc3_writel(dwc->regs, DWC3_GEVNTSIZ(n),
<<<<<<< HEAD
				evt->length & 0xffff);
=======
				DWC3_GEVNTSIZ_SIZE(evt->length));
>>>>>>> refs/remotes/origin/master
		dwc3_writel(dwc->regs, DWC3_GEVNTCOUNT(n), 0);
	}

	return 0;
}

static void dwc3_event_buffers_cleanup(struct dwc3 *dwc)
{
	struct dwc3_event_buffer	*evt;
	int				n;

	for (n = 0; n < dwc->num_event_buffers; n++) {
		evt = dwc->ev_buffs[n];
<<<<<<< HEAD
		dwc3_writel(dwc->regs, DWC3_GEVNTADRLO(n), 0);
		dwc3_writel(dwc->regs, DWC3_GEVNTADRHI(n), 0);
		dwc3_writel(dwc->regs, DWC3_GEVNTSIZ(n), 0);
=======

		evt->lpos = 0;

		dwc3_writel(dwc->regs, DWC3_GEVNTADRLO(n), 0);
		dwc3_writel(dwc->regs, DWC3_GEVNTADRHI(n), 0);
		dwc3_writel(dwc->regs, DWC3_GEVNTSIZ(n), DWC3_GEVNTSIZ_INTMASK
				| DWC3_GEVNTSIZ_SIZE(0));
>>>>>>> refs/remotes/origin/master
		dwc3_writel(dwc->regs, DWC3_GEVNTCOUNT(n), 0);
	}
}

<<<<<<< HEAD
static void __devinit dwc3_cache_hwparams(struct dwc3 *dwc)
=======
static void dwc3_core_num_eps(struct dwc3 *dwc)
{
	struct dwc3_hwparams	*parms = &dwc->hwparams;

	dwc->num_in_eps = DWC3_NUM_IN_EPS(parms);
	dwc->num_out_eps = DWC3_NUM_EPS(parms) - dwc->num_in_eps;

	dev_vdbg(dwc->dev, "found %d IN and %d OUT endpoints\n",
			dwc->num_in_eps, dwc->num_out_eps);
}

static void dwc3_cache_hwparams(struct dwc3 *dwc)
>>>>>>> refs/remotes/origin/master
{
	struct dwc3_hwparams	*parms = &dwc->hwparams;

	parms->hwparams0 = dwc3_readl(dwc->regs, DWC3_GHWPARAMS0);
	parms->hwparams1 = dwc3_readl(dwc->regs, DWC3_GHWPARAMS1);
	parms->hwparams2 = dwc3_readl(dwc->regs, DWC3_GHWPARAMS2);
	parms->hwparams3 = dwc3_readl(dwc->regs, DWC3_GHWPARAMS3);
	parms->hwparams4 = dwc3_readl(dwc->regs, DWC3_GHWPARAMS4);
	parms->hwparams5 = dwc3_readl(dwc->regs, DWC3_GHWPARAMS5);
	parms->hwparams6 = dwc3_readl(dwc->regs, DWC3_GHWPARAMS6);
	parms->hwparams7 = dwc3_readl(dwc->regs, DWC3_GHWPARAMS7);
	parms->hwparams8 = dwc3_readl(dwc->regs, DWC3_GHWPARAMS8);
}

/**
 * dwc3_core_init - Low-level initialization of DWC3 Core
 * @dwc: Pointer to our controller context structure
 *
 * Returns 0 on success otherwise negative errno.
 */
<<<<<<< HEAD
static int __devinit dwc3_core_init(struct dwc3 *dwc)
=======
static int dwc3_core_init(struct dwc3 *dwc)
>>>>>>> refs/remotes/origin/master
{
	unsigned long		timeout;
	u32			reg;
	int			ret;

	reg = dwc3_readl(dwc->regs, DWC3_GSNPSID);
	/* This should read as U3 followed by revision number */
	if ((reg & DWC3_GSNPSID_MASK) != 0x55330000) {
		dev_err(dwc->dev, "this is not a DesignWare USB3 DRD Core\n");
		ret = -ENODEV;
		goto err0;
	}
	dwc->revision = reg;

<<<<<<< HEAD
	dwc3_core_soft_reset(dwc);

=======
>>>>>>> refs/remotes/origin/master
	/* issue device SoftReset too */
	timeout = jiffies + msecs_to_jiffies(500);
	dwc3_writel(dwc->regs, DWC3_DCTL, DWC3_DCTL_CSFTRST);
	do {
		reg = dwc3_readl(dwc->regs, DWC3_DCTL);
		if (!(reg & DWC3_DCTL_CSFTRST))
			break;

		if (time_after(jiffies, timeout)) {
			dev_err(dwc->dev, "Reset Timed Out\n");
			ret = -ETIMEDOUT;
			goto err0;
		}

		cpu_relax();
	} while (true);

<<<<<<< HEAD
	dwc3_cache_hwparams(dwc);
=======
	dwc3_core_soft_reset(dwc);
>>>>>>> refs/remotes/origin/master

	reg = dwc3_readl(dwc->regs, DWC3_GCTL);
	reg &= ~DWC3_GCTL_SCALEDOWN_MASK;
	reg &= ~DWC3_GCTL_DISSCRAMBLE;

	switch (DWC3_GHWPARAMS1_EN_PWROPT(dwc->hwparams.hwparams1)) {
	case DWC3_GHWPARAMS1_EN_PWROPT_CLK:
		reg &= ~DWC3_GCTL_DSBLCLKGTNG;
		break;
	default:
		dev_dbg(dwc->dev, "No power optimization available\n");
	}

	/*
	 * WORKAROUND: DWC3 revisions <1.90a have a bug
	 * where the device can fail to connect at SuperSpeed
	 * and falls back to high-speed mode which causes
	 * the device to enter a Connect/Disconnect loop
	 */
	if (dwc->revision < DWC3_REVISION_190A)
		reg |= DWC3_GCTL_U2RSTECN;

<<<<<<< HEAD
	dwc3_writel(dwc->regs, DWC3_GCTL, reg);

	/*
	 * Currently, the default and the recommended value for GUSB3PIPECTL
	 * [21:19] in the RTL is 3'b100 or 32 consecutive errors. Based on
	 * analysis and experiments in the lab, it is found that there is a
	 * relatively low probability of getting 32 consecutive word errors
	 * in the presence of random recovered noise (during electrical idle).
	 * This can delay the entry to a low power state such that for
	 * applications where the link stays in a non-U0 state for a short
	 * duration (< 1 microsecond), the local PHY does not enter the low
	 * power state prior to receiving a potential LFPS wakeup. This causes
	 * the PHY CDR (Clock and Data Recovery) operation to be unstable for
	 * some Synopsys PHYs.
	 *
	 * The proposal now is to change the default and the recommended value
	 * for GUSB3PIPECTL[21:19] in the RTL from 3'b100 to a minimum of
	 * 3'b001. Perform the same in software for controllers prior to 2.30a
	 * revision.
	 */

	if (dwc->revision < DWC3_REVISION_230A) {
		reg = dwc3_readl(dwc->regs, DWC3_GUSB3PIPECTL(0));
		reg &= ~DWC3_GUSB3PIPECTL_DELAY_P1P2P3;
		reg |= 1 << __ffs(DWC3_GUSB3PIPECTL_DELAY_P1P2P3);
		dwc3_writel(dwc->regs, DWC3_GUSB3PIPECTL(0), reg);
	}

	ret = dwc3_alloc_event_buffers(dwc, DWC3_EVENT_BUFFERS_SIZE);
	if (ret) {
		dev_err(dwc->dev, "failed to allocate event buffers\n");
		ret = -ENOMEM;
		goto err1;
	}

	ret = dwc3_event_buffers_setup(dwc);
	if (ret) {
		dev_err(dwc->dev, "failed to setup event buffers\n");
		goto err1;
	}

	return 0;

err1:
	dwc3_free_event_buffers(dwc);

=======
	dwc3_core_num_eps(dwc);

	dwc3_writel(dwc->regs, DWC3_GCTL, reg);

	return 0;

>>>>>>> refs/remotes/origin/master
err0:
	return ret;
}

static void dwc3_core_exit(struct dwc3 *dwc)
{
<<<<<<< HEAD
	dwc3_event_buffers_cleanup(dwc);
	dwc3_free_event_buffers(dwc);
=======
	usb_phy_shutdown(dwc->usb2_phy);
	usb_phy_shutdown(dwc->usb3_phy);
>>>>>>> refs/remotes/origin/master
}

#define DWC3_ALIGN_MASK		(16 - 1)

<<<<<<< HEAD
static int __devinit dwc3_probe(struct platform_device *pdev)
{
	struct device_node	*node = pdev->dev.of_node;
	struct resource		*res;
	struct dwc3		*dwc;
	struct device		*dev = &pdev->dev;
=======
static int dwc3_probe(struct platform_device *pdev)
{
	struct device		*dev = &pdev->dev;
	struct dwc3_platform_data *pdata = dev_get_platdata(dev);
	struct device_node	*node = dev->of_node;
	struct resource		*res;
	struct dwc3		*dwc;
>>>>>>> refs/remotes/origin/master

	int			ret = -ENOMEM;

	void __iomem		*regs;
	void			*mem;

<<<<<<< HEAD
	u8			mode;

=======
>>>>>>> refs/remotes/origin/master
	mem = devm_kzalloc(dev, sizeof(*dwc) + DWC3_ALIGN_MASK, GFP_KERNEL);
	if (!mem) {
		dev_err(dev, "not enough memory\n");
		return -ENOMEM;
	}
	dwc = PTR_ALIGN(mem, DWC3_ALIGN_MASK + 1);
	dwc->mem = mem;

	res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (!res) {
		dev_err(dev, "missing IRQ\n");
		return -ENODEV;
	}
<<<<<<< HEAD
<<<<<<< HEAD
	dwc->xhci_resources[1] = *res;
=======
	dwc->xhci_resources[1].start = res->start;
	dwc->xhci_resources[1].end = res->end;
	dwc->xhci_resources[1].flags = res->flags;
	dwc->xhci_resources[1].name = res->name;
>>>>>>> refs/remotes/origin/master
=======
	dwc->xhci_resources[1] = *res;
>>>>>>> refs/remotes/origin/cm-11.0

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(dev, "missing memory resource\n");
		return -ENODEV;
	}
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	dwc->xhci_resources[0] = *res;
	dwc->xhci_resources[0].end = dwc->xhci_resources[0].start +
					DWC3_XHCI_REGS_END;

	 /*
	  * Request memory region but exclude xHCI regs,
	  * since it will be requested by the xhci-plat driver.
	  */
	res = devm_request_mem_region(dev, res->start + DWC3_GLOBALS_REGS_START,
			resource_size(res) - DWC3_GLOBALS_REGS_START,
			dev_name(dev));

	if (!res) {
		dev_err(dev, "can't request mem region\n");
		return -ENOMEM;
	}

	regs = devm_ioremap(dev, res->start, resource_size(res));
	if (!regs) {
		dev_err(dev, "ioremap failed\n");
		return -ENOMEM;
	}

<<<<<<< HEAD
=======

	if (node) {
		dwc->maximum_speed = of_usb_get_maximum_speed(node);

		dwc->usb2_phy = devm_usb_get_phy_by_phandle(dev, "usb-phy", 0);
		dwc->usb3_phy = devm_usb_get_phy_by_phandle(dev, "usb-phy", 1);

		dwc->needs_fifo_resize = of_property_read_bool(node, "tx-fifo-resize");
		dwc->dr_mode = of_usb_get_dr_mode(node);
	} else if (pdata) {
		dwc->maximum_speed = pdata->maximum_speed;

		dwc->usb2_phy = devm_usb_get_phy(dev, USB_PHY_TYPE_USB2);
		dwc->usb3_phy = devm_usb_get_phy(dev, USB_PHY_TYPE_USB3);

		dwc->needs_fifo_resize = pdata->tx_fifo_resize;
		dwc->dr_mode = pdata->dr_mode;
	} else {
		dwc->usb2_phy = devm_usb_get_phy(dev, USB_PHY_TYPE_USB2);
		dwc->usb3_phy = devm_usb_get_phy(dev, USB_PHY_TYPE_USB3);
	}

	/* default to superspeed if no maximum_speed passed */
	if (dwc->maximum_speed == USB_SPEED_UNKNOWN)
		dwc->maximum_speed = USB_SPEED_SUPER;

	if (IS_ERR(dwc->usb2_phy)) {
		ret = PTR_ERR(dwc->usb2_phy);

		/*
		 * if -ENXIO is returned, it means PHY layer wasn't
		 * enabled, so it makes no sense to return -EPROBE_DEFER
		 * in that case, since no PHY driver will ever probe.
		 */
		if (ret == -ENXIO)
			return ret;

		dev_err(dev, "no usb2 phy configured\n");
		return -EPROBE_DEFER;
	}

	if (IS_ERR(dwc->usb3_phy)) {
		ret = PTR_ERR(dwc->usb3_phy);

		/*
		 * if -ENXIO is returned, it means PHY layer wasn't
		 * enabled, so it makes no sense to return -EPROBE_DEFER
		 * in that case, since no PHY driver will ever probe.
		 */
		if (ret == -ENXIO)
			return ret;

		dev_err(dev, "no usb3 phy configured\n");
		return -EPROBE_DEFER;
	}

	dwc->xhci_resources[0].start = res->start;
	dwc->xhci_resources[0].end = dwc->xhci_resources[0].start +
					DWC3_XHCI_REGS_END;
	dwc->xhci_resources[0].flags = res->flags;
	dwc->xhci_resources[0].name = res->name;

	res->start += DWC3_GLOBALS_REGS_START;

	/*
	 * Request memory region but exclude xHCI regs,
	 * since it will be requested by the xhci-plat driver.
	 */
	regs = devm_ioremap_resource(dev, res);
	if (IS_ERR(regs))
		return PTR_ERR(regs);

>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	spin_lock_init(&dwc->lock);
	platform_set_drvdata(pdev, dwc);

	dwc->regs	= regs;
	dwc->regs_size	= resource_size(res);
	dwc->dev	= dev;

<<<<<<< HEAD
	if (!strncmp("super", maximum_speed, 5))
		dwc->maximum_speed = DWC3_DCFG_SUPERSPEED;
	else if (!strncmp("high", maximum_speed, 4))
		dwc->maximum_speed = DWC3_DCFG_HIGHSPEED;
	else if (!strncmp("full", maximum_speed, 4))
		dwc->maximum_speed = DWC3_DCFG_FULLSPEED1;
	else if (!strncmp("low", maximum_speed, 3))
		dwc->maximum_speed = DWC3_DCFG_LOWSPEED;
	else
		dwc->maximum_speed = DWC3_DCFG_SUPERSPEED;

	if (of_get_property(node, "tx-fifo-resize", NULL))
		dwc->needs_fifo_resize = true;
=======
	dev->dma_mask	= dev->parent->dma_mask;
	dev->dma_parms	= dev->parent->dma_parms;
	dma_set_coherent_mask(dev, dev->parent->coherent_dma_mask);
>>>>>>> refs/remotes/origin/master

	pm_runtime_enable(dev);
	pm_runtime_get_sync(dev);
	pm_runtime_forbid(dev);

<<<<<<< HEAD
	ret = dwc3_core_init(dwc);
	if (ret) {
		dev_err(dev, "failed to initialize core\n");
		return ret;
	}

	mode = DWC3_MODE(dwc->hwparams.hwparams0);

	switch (mode) {
	case DWC3_MODE_DEVICE:
=======
	dwc3_cache_hwparams(dwc);

	ret = dwc3_alloc_event_buffers(dwc, DWC3_EVENT_BUFFERS_SIZE);
	if (ret) {
		dev_err(dwc->dev, "failed to allocate event buffers\n");
		ret = -ENOMEM;
		goto err0;
	}

	ret = dwc3_core_init(dwc);
	if (ret) {
		dev_err(dev, "failed to initialize core\n");
		goto err0;
	}

	usb_phy_set_suspend(dwc->usb2_phy, 0);
	usb_phy_set_suspend(dwc->usb3_phy, 0);

	ret = dwc3_event_buffers_setup(dwc);
	if (ret) {
		dev_err(dwc->dev, "failed to setup event buffers\n");
		goto err1;
	}

	if (IS_ENABLED(CONFIG_USB_DWC3_HOST))
		dwc->dr_mode = USB_DR_MODE_HOST;
	else if (IS_ENABLED(CONFIG_USB_DWC3_GADGET))
		dwc->dr_mode = USB_DR_MODE_PERIPHERAL;

	if (dwc->dr_mode == USB_DR_MODE_UNKNOWN)
		dwc->dr_mode = USB_DR_MODE_OTG;

	switch (dwc->dr_mode) {
	case USB_DR_MODE_PERIPHERAL:
>>>>>>> refs/remotes/origin/master
		dwc3_set_mode(dwc, DWC3_GCTL_PRTCAP_DEVICE);
		ret = dwc3_gadget_init(dwc);
		if (ret) {
			dev_err(dev, "failed to initialize gadget\n");
<<<<<<< HEAD
			goto err1;
		}
		break;
	case DWC3_MODE_HOST:
=======
			goto err2;
		}
		break;
	case USB_DR_MODE_HOST:
>>>>>>> refs/remotes/origin/master
		dwc3_set_mode(dwc, DWC3_GCTL_PRTCAP_HOST);
		ret = dwc3_host_init(dwc);
		if (ret) {
			dev_err(dev, "failed to initialize host\n");
<<<<<<< HEAD
			goto err1;
		}
		break;
	case DWC3_MODE_DRD:
		dwc3_set_mode(dwc, DWC3_GCTL_PRTCAP_OTG);
		ret = dwc3_otg_init(dwc);
		if (ret) {
			dev_err(dev, "failed to initialize otg\n");
			goto err1;
		}

		ret = dwc3_host_init(dwc);
		if (ret) {
			dev_err(dev, "failed to initialize host\n");
			dwc3_otg_exit(dwc);
			goto err1;
=======
			goto err2;
		}
		break;
	case USB_DR_MODE_OTG:
		dwc3_set_mode(dwc, DWC3_GCTL_PRTCAP_OTG);
		ret = dwc3_host_init(dwc);
		if (ret) {
			dev_err(dev, "failed to initialize host\n");
			goto err2;
>>>>>>> refs/remotes/origin/master
		}

		ret = dwc3_gadget_init(dwc);
		if (ret) {
			dev_err(dev, "failed to initialize gadget\n");
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
			dwc3_host_exit(dwc);
			dwc3_otg_exit(dwc);
			goto err1;
		}
		break;
	default:
		dev_err(dev, "Unsupported mode of operation %d\n", mode);
		goto err1;
	}
	dwc->mode = mode;
=======
			goto err2;
		}
		break;
	default:
		dev_err(dev, "Unsupported mode of operation %d\n", dwc->dr_mode);
		goto err2;
	}
>>>>>>> refs/remotes/origin/master

	ret = dwc3_debugfs_init(dwc);
	if (ret) {
		dev_err(dev, "failed to initialize debugfs\n");
<<<<<<< HEAD
		goto err2;
=======
		goto err3;
>>>>>>> refs/remotes/origin/master
	}

	pm_runtime_allow(dev);

	return 0;

<<<<<<< HEAD
err2:
	switch (mode) {
	case DWC3_MODE_DEVICE:
		dwc3_gadget_exit(dwc);
		break;
	case DWC3_MODE_HOST:
		dwc3_host_exit(dwc);
		break;
	case DWC3_MODE_DRD:
<<<<<<< HEAD
		dwc3_gadget_exit(dwc);
		dwc3_host_exit(dwc);
		dwc3_otg_exit(dwc);
=======
err3:
	switch (dwc->dr_mode) {
	case USB_DR_MODE_PERIPHERAL:
=======
>>>>>>> refs/remotes/origin/cm-11.0
		dwc3_gadget_exit(dwc);
		dwc3_host_exit(dwc);
		dwc3_otg_exit(dwc);
		break;
	case USB_DR_MODE_HOST:
		dwc3_host_exit(dwc);
		break;
	case USB_DR_MODE_OTG:
		dwc3_host_exit(dwc);
		dwc3_gadget_exit(dwc);
>>>>>>> refs/remotes/origin/master
		break;
	default:
		/* do nothing */
		break;
	}

<<<<<<< HEAD
err1:
	dwc3_core_exit(dwc);

	return ret;
}

static int __devexit dwc3_remove(struct platform_device *pdev)
{
	struct dwc3	*dwc = platform_get_drvdata(pdev);
	struct resource	*res;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);

	pm_runtime_put(&pdev->dev);
=======
err2:
	dwc3_event_buffers_cleanup(dwc);

err1:
	usb_phy_set_suspend(dwc->usb2_phy, 1);
	usb_phy_set_suspend(dwc->usb3_phy, 1);
	dwc3_core_exit(dwc);

err0:
	dwc3_free_event_buffers(dwc);

	return ret;
}

static int dwc3_remove(struct platform_device *pdev)
{
	struct dwc3	*dwc = platform_get_drvdata(pdev);

	usb_phy_set_suspend(dwc->usb2_phy, 1);
	usb_phy_set_suspend(dwc->usb3_phy, 1);

	pm_runtime_put_sync(&pdev->dev);
>>>>>>> refs/remotes/origin/master
	pm_runtime_disable(&pdev->dev);

	dwc3_debugfs_exit(dwc);

<<<<<<< HEAD
	switch (dwc->mode) {
	case DWC3_MODE_DEVICE:
		dwc3_gadget_exit(dwc);
		break;
	case DWC3_MODE_HOST:
		dwc3_host_exit(dwc);
		break;
	case DWC3_MODE_DRD:
<<<<<<< HEAD
		dwc3_gadget_exit(dwc);
		dwc3_host_exit(dwc);
		dwc3_otg_exit(dwc);
=======
	switch (dwc->dr_mode) {
	case USB_DR_MODE_PERIPHERAL:
=======
>>>>>>> refs/remotes/origin/cm-11.0
		dwc3_gadget_exit(dwc);
		dwc3_host_exit(dwc);
		dwc3_otg_exit(dwc);
		break;
	case USB_DR_MODE_HOST:
		dwc3_host_exit(dwc);
		break;
	case USB_DR_MODE_OTG:
		dwc3_host_exit(dwc);
		dwc3_gadget_exit(dwc);
>>>>>>> refs/remotes/origin/master
		break;
	default:
		/* do nothing */
		break;
	}

<<<<<<< HEAD
=======
	dwc3_event_buffers_cleanup(dwc);
	dwc3_free_event_buffers(dwc);
>>>>>>> refs/remotes/origin/master
	dwc3_core_exit(dwc);

	return 0;
}

<<<<<<< HEAD
static struct platform_driver dwc3_driver = {
	.probe		= dwc3_probe,
	.remove		= __devexit_p(dwc3_remove),
	.driver		= {
		.name	= "dwc3",
=======
#ifdef CONFIG_PM_SLEEP
static int dwc3_prepare(struct device *dev)
{
	struct dwc3	*dwc = dev_get_drvdata(dev);
	unsigned long	flags;

	spin_lock_irqsave(&dwc->lock, flags);

	switch (dwc->dr_mode) {
	case USB_DR_MODE_PERIPHERAL:
	case USB_DR_MODE_OTG:
		dwc3_gadget_prepare(dwc);
		/* FALLTHROUGH */
	case USB_DR_MODE_HOST:
	default:
		dwc3_event_buffers_cleanup(dwc);
		break;
	}

	spin_unlock_irqrestore(&dwc->lock, flags);

	return 0;
}

static void dwc3_complete(struct device *dev)
{
	struct dwc3	*dwc = dev_get_drvdata(dev);
	unsigned long	flags;

	spin_lock_irqsave(&dwc->lock, flags);

	switch (dwc->dr_mode) {
	case USB_DR_MODE_PERIPHERAL:
	case USB_DR_MODE_OTG:
		dwc3_gadget_complete(dwc);
		/* FALLTHROUGH */
	case USB_DR_MODE_HOST:
	default:
		dwc3_event_buffers_setup(dwc);
		break;
	}

	spin_unlock_irqrestore(&dwc->lock, flags);
}

static int dwc3_suspend(struct device *dev)
{
	struct dwc3	*dwc = dev_get_drvdata(dev);
	unsigned long	flags;

	spin_lock_irqsave(&dwc->lock, flags);

	switch (dwc->dr_mode) {
	case USB_DR_MODE_PERIPHERAL:
	case USB_DR_MODE_OTG:
		dwc3_gadget_suspend(dwc);
		/* FALLTHROUGH */
	case USB_DR_MODE_HOST:
	default:
		/* do nothing */
		break;
	}

	dwc->gctl = dwc3_readl(dwc->regs, DWC3_GCTL);
	spin_unlock_irqrestore(&dwc->lock, flags);

	usb_phy_shutdown(dwc->usb3_phy);
	usb_phy_shutdown(dwc->usb2_phy);

	return 0;
}

static int dwc3_resume(struct device *dev)
{
	struct dwc3	*dwc = dev_get_drvdata(dev);
	unsigned long	flags;

	usb_phy_init(dwc->usb3_phy);
	usb_phy_init(dwc->usb2_phy);

	spin_lock_irqsave(&dwc->lock, flags);

	dwc3_writel(dwc->regs, DWC3_GCTL, dwc->gctl);

	switch (dwc->dr_mode) {
	case USB_DR_MODE_PERIPHERAL:
	case USB_DR_MODE_OTG:
		dwc3_gadget_resume(dwc);
		/* FALLTHROUGH */
	case USB_DR_MODE_HOST:
	default:
		/* do nothing */
		break;
	}

	spin_unlock_irqrestore(&dwc->lock, flags);

	pm_runtime_disable(dev);
	pm_runtime_set_active(dev);
	pm_runtime_enable(dev);

	return 0;
}

static const struct dev_pm_ops dwc3_dev_pm_ops = {
	.prepare	= dwc3_prepare,
	.complete	= dwc3_complete,

	SET_SYSTEM_SLEEP_PM_OPS(dwc3_suspend, dwc3_resume)
};

#define DWC3_PM_OPS	&(dwc3_dev_pm_ops)
#else
#define DWC3_PM_OPS	NULL
#endif

#ifdef CONFIG_OF
static const struct of_device_id of_dwc3_match[] = {
	{
		.compatible = "snps,dwc3"
	},
	{
		.compatible = "synopsys,dwc3"
	},
	{ },
};
MODULE_DEVICE_TABLE(of, of_dwc3_match);
#endif

static struct platform_driver dwc3_driver = {
	.probe		= dwc3_probe,
	.remove		= dwc3_remove,
	.driver		= {
		.name	= "dwc3",
		.of_match_table	= of_match_ptr(of_dwc3_match),
		.pm	= DWC3_PM_OPS,
>>>>>>> refs/remotes/origin/master
	},
};

module_platform_driver(dwc3_driver);

MODULE_ALIAS("platform:dwc3");
MODULE_AUTHOR("Felipe Balbi <balbi@ti.com>");
<<<<<<< HEAD
MODULE_LICENSE("Dual BSD/GPL");
=======
MODULE_LICENSE("GPL v2");
>>>>>>> refs/remotes/origin/master
MODULE_DESCRIPTION("DesignWare USB3 DRD Controller Driver");
