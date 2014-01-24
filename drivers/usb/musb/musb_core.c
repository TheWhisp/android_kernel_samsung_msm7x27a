/*
 * MUSB OTG driver core code
 *
 * Copyright 2005 Mentor Graphics Corporation
 * Copyright (C) 2005-2006 by Texas Instruments
 * Copyright (C) 2006-2007 Nokia Corporation
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
 * NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*
 * Inventra (Multipoint) Dual-Role Controller Driver for Linux.
 *
 * This consists of a Host Controller Driver (HCD) and a peripheral
 * controller driver implementing the "Gadget" API; OTG support is
 * in the works.  These are normal Linux-USB controller drivers which
 * use IRQs and have no dedicated thread.
 *
 * This version of the driver has only been used with products from
 * Texas Instruments.  Those products integrate the Inventra logic
 * with other DMA, IRQ, and bus modules, as well as other logic that
 * needs to be reflected in this driver.
 *
 *
 * NOTE:  the original Mentor code here was pretty much a collection
 * of mechanisms that don't seem to have been fully integrated/working
 * for *any* Linux kernel version.  This version aims at Linux 2.6.now,
 * Key open issues include:
 *
 *  - Lack of host-side transaction scheduling, for all transfer types.
 *    The hardware doesn't do it; instead, software must.
 *
 *    This is not an issue for OTG devices that don't support external
 *    hubs, but for more "normal" USB hosts it's a user issue that the
 *    "multipoint" support doesn't scale in the expected ways.  That
 *    includes DaVinci EVM in a common non-OTG mode.
 *
 *      * Control and bulk use dedicated endpoints, and there's as
 *        yet no mechanism to either (a) reclaim the hardware when
 *        peripherals are NAKing, which gets complicated with bulk
 *        endpoints, or (b) use more than a single bulk endpoint in
 *        each direction.
 *
 *        RESULT:  one device may be perceived as blocking another one.
 *
 *      * Interrupt and isochronous will dynamically allocate endpoint
 *        hardware, but (a) there's no record keeping for bandwidth;
 *        (b) in the common case that few endpoints are available, there
 *        is no mechanism to reuse endpoints to talk to multiple devices.
 *
 *        RESULT:  At one extreme, bandwidth can be overcommitted in
 *        some hardware configurations, no faults will be reported.
 *        At the other extreme, the bandwidth capabilities which do
 *        exist tend to be severely undercommitted.  You can't yet hook
 *        up both a keyboard and a mouse to an external USB hub.
 */

/*
 * This gets many kinds of configuration information:
 *	- Kconfig for everything user-configurable
 *	- platform_device for addressing, irq, and platform_data
<<<<<<< HEAD
 *	- platform_data is mostly for board-specific informarion
=======
 *	- platform_data is mostly for board-specific information
>>>>>>> refs/remotes/origin/master
 *	  (plus recentrly, SOC or family details)
 *
 * Most of the conditional compilation will (someday) vanish.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <linux/init.h>
=======
>>>>>>> refs/remotes/origin/master
#include <linux/list.h>
#include <linux/kobject.h>
#include <linux/prefetch.h>
#include <linux/platform_device.h>
#include <linux/io.h>
<<<<<<< HEAD
=======
#include <linux/dma-mapping.h>
>>>>>>> refs/remotes/origin/master

#include "musb_core.h"

#define TA_WAIT_BCON(m) max_t(int, (m)->a_wait_bcon, OTG_TIME_A_WAIT_BCON)


#define DRIVER_AUTHOR "Mentor Graphics, Texas Instruments, Nokia"
#define DRIVER_DESC "Inventra Dual-Role USB Controller Driver"

#define MUSB_VERSION "6.0"

#define DRIVER_INFO DRIVER_DESC ", v" MUSB_VERSION

#define MUSB_DRIVER_NAME "musb-hdrc"
const char musb_driver_name[] = MUSB_DRIVER_NAME;

MODULE_DESCRIPTION(DRIVER_INFO);
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:" MUSB_DRIVER_NAME);


/*-------------------------------------------------------------------------*/

static inline struct musb *dev_to_musb(struct device *dev)
{
	return dev_get_drvdata(dev);
}

/*-------------------------------------------------------------------------*/

#ifndef CONFIG_BLACKFIN
<<<<<<< HEAD
<<<<<<< HEAD
static int musb_ulpi_read(struct otg_transceiver *otg, u32 offset)
{
	void __iomem *addr = otg->io_priv;
	int	i = 0;
	u8	r;
	u8	power;
=======
=======
>>>>>>> refs/remotes/origin/master
static int musb_ulpi_read(struct usb_phy *phy, u32 offset)
{
	void __iomem *addr = phy->io_priv;
	int	i = 0;
	u8	r;
	u8	power;
	int	ret;

	pm_runtime_get_sync(phy->io_dev);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	/* Make sure the transceiver is not in low power mode */
	power = musb_readb(addr, MUSB_POWER);
	power &= ~MUSB_POWER_SUSPENDM;
	musb_writeb(addr, MUSB_POWER, power);

	/* REVISIT: musbhdrc_ulpi_an.pdf recommends setting the
	 * ULPICarKitControlDisableUTMI after clearing POWER_SUSPENDM.
	 */

	musb_writeb(addr, MUSB_ULPI_REG_ADDR, (u8)offset);
	musb_writeb(addr, MUSB_ULPI_REG_CONTROL,
			MUSB_ULPI_REG_REQ | MUSB_ULPI_RDN_WR);

	while (!(musb_readb(addr, MUSB_ULPI_REG_CONTROL)
				& MUSB_ULPI_REG_CMPLT)) {
		i++;
<<<<<<< HEAD
<<<<<<< HEAD
		if (i == 10000)
			return -ETIMEDOUT;
=======
=======
>>>>>>> refs/remotes/origin/master
		if (i == 10000) {
			ret = -ETIMEDOUT;
			goto out;
		}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	}
	r = musb_readb(addr, MUSB_ULPI_REG_CONTROL);
	r &= ~MUSB_ULPI_REG_CMPLT;
	musb_writeb(addr, MUSB_ULPI_REG_CONTROL, r);

<<<<<<< HEAD
<<<<<<< HEAD
	return musb_readb(addr, MUSB_ULPI_REG_DATA);
}

static int musb_ulpi_write(struct otg_transceiver *otg,
		u32 offset, u32 data)
{
	void __iomem *addr = otg->io_priv;
	int	i = 0;
	u8	r = 0;
	u8	power;
=======
=======
>>>>>>> refs/remotes/origin/master
	ret = musb_readb(addr, MUSB_ULPI_REG_DATA);

out:
	pm_runtime_put(phy->io_dev);

	return ret;
}

static int musb_ulpi_write(struct usb_phy *phy, u32 offset, u32 data)
{
	void __iomem *addr = phy->io_priv;
	int	i = 0;
	u8	r = 0;
	u8	power;
	int	ret = 0;

	pm_runtime_get_sync(phy->io_dev);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	/* Make sure the transceiver is not in low power mode */
	power = musb_readb(addr, MUSB_POWER);
	power &= ~MUSB_POWER_SUSPENDM;
	musb_writeb(addr, MUSB_POWER, power);

	musb_writeb(addr, MUSB_ULPI_REG_ADDR, (u8)offset);
	musb_writeb(addr, MUSB_ULPI_REG_DATA, (u8)data);
	musb_writeb(addr, MUSB_ULPI_REG_CONTROL, MUSB_ULPI_REG_REQ);

	while (!(musb_readb(addr, MUSB_ULPI_REG_CONTROL)
				& MUSB_ULPI_REG_CMPLT)) {
		i++;
<<<<<<< HEAD
<<<<<<< HEAD
		if (i == 10000)
			return -ETIMEDOUT;
=======
=======
>>>>>>> refs/remotes/origin/master
		if (i == 10000) {
			ret = -ETIMEDOUT;
			goto out;
		}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	r = musb_readb(addr, MUSB_ULPI_REG_CONTROL);
	r &= ~MUSB_ULPI_REG_CMPLT;
	musb_writeb(addr, MUSB_ULPI_REG_CONTROL, r);

<<<<<<< HEAD
<<<<<<< HEAD
	return 0;
=======
=======
>>>>>>> refs/remotes/origin/master
out:
	pm_runtime_put(phy->io_dev);

	return ret;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}
#else
#define musb_ulpi_read		NULL
#define musb_ulpi_write		NULL
#endif

<<<<<<< HEAD
<<<<<<< HEAD
static struct otg_io_access_ops musb_ulpi_access = {
=======
static struct usb_phy_io_ops musb_ulpi_access = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct usb_phy_io_ops musb_ulpi_access = {
>>>>>>> refs/remotes/origin/master
	.read = musb_ulpi_read,
	.write = musb_ulpi_write,
};

/*-------------------------------------------------------------------------*/

#if !defined(CONFIG_USB_MUSB_TUSB6010) && !defined(CONFIG_USB_MUSB_BLACKFIN)

/*
 * Load an endpoint's FIFO
 */
void musb_write_fifo(struct musb_hw_ep *hw_ep, u16 len, const u8 *src)
{
	struct musb *musb = hw_ep->musb;
	void __iomem *fifo = hw_ep->fifo;

<<<<<<< HEAD
=======
	if (unlikely(len == 0))
		return;

>>>>>>> refs/remotes/origin/master
	prefetch((u8 *)src);

	dev_dbg(musb->controller, "%cX ep%d fifo %p count %d buf %p\n",
			'T', hw_ep->epnum, fifo, len, src);

	/* we can't assume unaligned reads work */
	if (likely((0x01 & (unsigned long) src) == 0)) {
		u16	index = 0;

		/* best case is 32bit-aligned source address */
		if ((0x02 & (unsigned long) src) == 0) {
			if (len >= 4) {
<<<<<<< HEAD
				writesl(fifo, src + index, len >> 2);
=======
				iowrite32_rep(fifo, src + index, len >> 2);
>>>>>>> refs/remotes/origin/master
				index += len & ~0x03;
			}
			if (len & 0x02) {
				musb_writew(fifo, 0, *(u16 *)&src[index]);
				index += 2;
			}
		} else {
			if (len >= 2) {
<<<<<<< HEAD
				writesw(fifo, src + index, len >> 1);
=======
				iowrite16_rep(fifo, src + index, len >> 1);
>>>>>>> refs/remotes/origin/master
				index += len & ~0x01;
			}
		}
		if (len & 0x01)
			musb_writeb(fifo, 0, src[index]);
	} else  {
		/* byte aligned */
<<<<<<< HEAD
		writesb(fifo, src, len);
=======
		iowrite8_rep(fifo, src, len);
>>>>>>> refs/remotes/origin/master
	}
}

#if !defined(CONFIG_USB_MUSB_AM35X)
/*
 * Unload an endpoint's FIFO
 */
void musb_read_fifo(struct musb_hw_ep *hw_ep, u16 len, u8 *dst)
{
	struct musb *musb = hw_ep->musb;
	void __iomem *fifo = hw_ep->fifo;

<<<<<<< HEAD
=======
	if (unlikely(len == 0))
		return;

>>>>>>> refs/remotes/origin/master
	dev_dbg(musb->controller, "%cX ep%d fifo %p count %d buf %p\n",
			'R', hw_ep->epnum, fifo, len, dst);

	/* we can't assume unaligned writes work */
	if (likely((0x01 & (unsigned long) dst) == 0)) {
		u16	index = 0;

		/* best case is 32bit-aligned destination address */
		if ((0x02 & (unsigned long) dst) == 0) {
			if (len >= 4) {
<<<<<<< HEAD
				readsl(fifo, dst, len >> 2);
=======
				ioread32_rep(fifo, dst, len >> 2);
>>>>>>> refs/remotes/origin/master
				index = len & ~0x03;
			}
			if (len & 0x02) {
				*(u16 *)&dst[index] = musb_readw(fifo, 0);
				index += 2;
			}
		} else {
			if (len >= 2) {
<<<<<<< HEAD
				readsw(fifo, dst, len >> 1);
=======
				ioread16_rep(fifo, dst, len >> 1);
>>>>>>> refs/remotes/origin/master
				index = len & ~0x01;
			}
		}
		if (len & 0x01)
			dst[index] = musb_readb(fifo, 0);
	} else  {
		/* byte aligned */
<<<<<<< HEAD
		readsb(fifo, dst, len);
=======
		ioread8_rep(fifo, dst, len);
>>>>>>> refs/remotes/origin/master
	}
}
#endif

#endif	/* normal PIO */


/*-------------------------------------------------------------------------*/

/* for high speed test mode; see USB 2.0 spec 7.1.20 */
static const u8 musb_test_packet[53] = {
	/* implicit SYNC then DATA0 to start */

	/* JKJKJKJK x9 */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	/* JJKKJJKK x8 */
	0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
	/* JJJJKKKK x8 */
	0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee,
	/* JJJJJJJKKKKKKK x8 */
	0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	/* JJJJJJJK x8 */
	0x7f, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd,
	/* JKKKKKKK x10, JK */
	0xfc, 0x7e, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd, 0x7e

	/* implicit CRC16 then EOP to end */
};

void musb_load_testpacket(struct musb *musb)
{
	void __iomem	*regs = musb->endpoints[0].regs;

	musb_ep_select(musb->mregs, 0);
	musb_write_fifo(musb->control_ep,
			sizeof(musb_test_packet), musb_test_packet);
	musb_writew(regs, MUSB_CSR0, MUSB_CSR0_TXPKTRDY);
}

/*-------------------------------------------------------------------------*/

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef	CONFIG_USB_MUSB_OTG

=======
>>>>>>> refs/remotes/origin/cm-10.0
/*
 * Handles OTG hnp timeouts, such as b_ase0_brst
 */
void musb_otg_timer_func(unsigned long data)
=======
/*
 * Handles OTG hnp timeouts, such as b_ase0_brst
 */
static void musb_otg_timer_func(unsigned long data)
>>>>>>> refs/remotes/origin/master
{
	struct musb	*musb = (struct musb *)data;
	unsigned long	flags;

	spin_lock_irqsave(&musb->lock, flags);
	switch (musb->xceiv->state) {
	case OTG_STATE_B_WAIT_ACON:
		dev_dbg(musb->controller, "HNP: b_wait_acon timeout; back to b_peripheral\n");
		musb_g_disconnect(musb);
		musb->xceiv->state = OTG_STATE_B_PERIPHERAL;
		musb->is_active = 0;
		break;
	case OTG_STATE_A_SUSPEND:
	case OTG_STATE_A_WAIT_BCON:
		dev_dbg(musb->controller, "HNP: %s timeout\n",
<<<<<<< HEAD
			otg_state_string(musb->xceiv->state));
=======
			usb_otg_state_string(musb->xceiv->state));
>>>>>>> refs/remotes/origin/master
		musb_platform_set_vbus(musb, 0);
		musb->xceiv->state = OTG_STATE_A_WAIT_VFALL;
		break;
	default:
		dev_dbg(musb->controller, "HNP: Unhandled mode %s\n",
<<<<<<< HEAD
			otg_state_string(musb->xceiv->state));
	}
	musb->ignore_disconnect = 0;
=======
			usb_otg_state_string(musb->xceiv->state));
	}
>>>>>>> refs/remotes/origin/master
	spin_unlock_irqrestore(&musb->lock, flags);
}

/*
 * Stops the HNP transition. Caller must take care of locking.
 */
void musb_hnp_stop(struct musb *musb)
{
<<<<<<< HEAD
	struct usb_hcd	*hcd = musb_to_hcd(musb);
	void __iomem	*mbase = musb->mregs;
	u8	reg;

	dev_dbg(musb->controller, "HNP: stop from %s\n", otg_state_string(musb->xceiv->state));
=======
	struct usb_hcd	*hcd = musb->hcd;
	void __iomem	*mbase = musb->mregs;
	u8	reg;

	dev_dbg(musb->controller, "HNP: stop from %s\n",
			usb_otg_state_string(musb->xceiv->state));
>>>>>>> refs/remotes/origin/master

	switch (musb->xceiv->state) {
	case OTG_STATE_A_PERIPHERAL:
		musb_g_disconnect(musb);
		dev_dbg(musb->controller, "HNP: back to %s\n",
<<<<<<< HEAD
			otg_state_string(musb->xceiv->state));
		break;
	case OTG_STATE_B_HOST:
		dev_dbg(musb->controller, "HNP: Disabling HR\n");
		hcd->self.is_b_host = 0;
=======
			usb_otg_state_string(musb->xceiv->state));
		break;
	case OTG_STATE_B_HOST:
		dev_dbg(musb->controller, "HNP: Disabling HR\n");
		if (hcd)
			hcd->self.is_b_host = 0;
>>>>>>> refs/remotes/origin/master
		musb->xceiv->state = OTG_STATE_B_PERIPHERAL;
		MUSB_DEV_MODE(musb);
		reg = musb_readb(mbase, MUSB_POWER);
		reg |= MUSB_POWER_SUSPENDM;
		musb_writeb(mbase, MUSB_POWER, reg);
		/* REVISIT: Start SESSION_REQUEST here? */
		break;
	default:
		dev_dbg(musb->controller, "HNP: Stopping in unknown state %s\n",
<<<<<<< HEAD
			otg_state_string(musb->xceiv->state));
=======
			usb_otg_state_string(musb->xceiv->state));
>>>>>>> refs/remotes/origin/master
	}

	/*
	 * When returning to A state after HNP, avoid hub_port_rebounce(),
	 * which cause occasional OPT A "Did not receive reset after connect"
	 * errors.
	 */
	musb->port1_status &= ~(USB_PORT_STAT_C_CONNECTION << 16);
}

<<<<<<< HEAD
<<<<<<< HEAD
#endif

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * Interrupt Service Routine to record USB "global" interrupts.
 * Since these do not happen often and signify things of
 * paramount importance, it seems OK to check them individually;
 * the order of the tests is specified in the manual
 *
 * @param musb instance pointer
 * @param int_usb register contents
 * @param devctl
 * @param power
 */

static irqreturn_t musb_stage0_irq(struct musb *musb, u8 int_usb,
<<<<<<< HEAD
				u8 devctl, u8 power)
{
<<<<<<< HEAD
=======
	struct usb_otg *otg = musb->xceiv->otg;
>>>>>>> refs/remotes/origin/cm-10.0
	irqreturn_t handled = IRQ_NONE;

	dev_dbg(musb->controller, "<== Power=%02x, DevCtl=%02x, int_usb=0x%x\n", power, devctl,
=======
				u8 devctl)
{
	struct usb_otg *otg = musb->xceiv->otg;
	irqreturn_t handled = IRQ_NONE;

	dev_dbg(musb->controller, "<== DevCtl=%02x, int_usb=0x%x\n", devctl,
>>>>>>> refs/remotes/origin/master
		int_usb);

	/* in host mode, the peripheral may issue remote wakeup.
	 * in peripheral mode, the host may resume the link.
	 * spurious RESUME irqs happen too, paired with SUSPEND.
	 */
	if (int_usb & MUSB_INTR_RESUME) {
		handled = IRQ_HANDLED;
<<<<<<< HEAD
		dev_dbg(musb->controller, "RESUME (%s)\n", otg_state_string(musb->xceiv->state));

		if (devctl & MUSB_DEVCTL_HM) {
<<<<<<< HEAD
#ifdef CONFIG_USB_MUSB_HDRC_HCD
=======
>>>>>>> refs/remotes/origin/cm-10.0
			void __iomem *mbase = musb->mregs;
=======
		dev_dbg(musb->controller, "RESUME (%s)\n", usb_otg_state_string(musb->xceiv->state));

		if (devctl & MUSB_DEVCTL_HM) {
			void __iomem *mbase = musb->mregs;
			u8 power;
>>>>>>> refs/remotes/origin/master

			switch (musb->xceiv->state) {
			case OTG_STATE_A_SUSPEND:
				/* remote wakeup?  later, GetPortStatus
				 * will stop RESUME signaling
				 */

<<<<<<< HEAD
=======
				power = musb_readb(musb->mregs, MUSB_POWER);
>>>>>>> refs/remotes/origin/master
				if (power & MUSB_POWER_SUSPENDM) {
					/* spurious */
					musb->int_usb &= ~MUSB_INTR_SUSPEND;
					dev_dbg(musb->controller, "Spurious SUSPENDM\n");
					break;
				}

				power &= ~MUSB_POWER_SUSPENDM;
				musb_writeb(mbase, MUSB_POWER,
						power | MUSB_POWER_RESUME);

				musb->port1_status |=
						(USB_PORT_STAT_C_SUSPEND << 16)
						| MUSB_PORT_STAT_RESUME;
<<<<<<< HEAD
				musb->rh_timer = jiffies
						+ msecs_to_jiffies(20);

				musb->xceiv->state = OTG_STATE_A_HOST;
				musb->is_active = 1;
				usb_hcd_resume_root_hub(musb_to_hcd(musb));
=======
				schedule_delayed_work(
					&musb->finish_resume_work, 20);

				musb->xceiv->state = OTG_STATE_A_HOST;
				musb->is_active = 1;
				musb_host_resume_root_hub(musb);
>>>>>>> refs/remotes/origin/master
				break;
			case OTG_STATE_B_WAIT_ACON:
				musb->xceiv->state = OTG_STATE_B_PERIPHERAL;
				musb->is_active = 1;
				MUSB_DEV_MODE(musb);
				break;
			default:
				WARNING("bogus %s RESUME (%s)\n",
					"host",
<<<<<<< HEAD
					otg_state_string(musb->xceiv->state));
			}
<<<<<<< HEAD
#endif
		} else {
			switch (musb->xceiv->state) {
#ifdef CONFIG_USB_MUSB_HDRC_HCD
=======
		} else {
			switch (musb->xceiv->state) {
>>>>>>> refs/remotes/origin/cm-10.0
			case OTG_STATE_A_SUSPEND:
				/* possibly DISCONNECT is upcoming */
				musb->xceiv->state = OTG_STATE_A_HOST;
				usb_hcd_resume_root_hub(musb_to_hcd(musb));
				break;
<<<<<<< HEAD
#endif
#ifdef CONFIG_USB_GADGET_MUSB_HDRC
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
					usb_otg_state_string(musb->xceiv->state));
			}
		} else {
			switch (musb->xceiv->state) {
			case OTG_STATE_A_SUSPEND:
				/* possibly DISCONNECT is upcoming */
				musb->xceiv->state = OTG_STATE_A_HOST;
				musb_host_resume_root_hub(musb);
				break;
>>>>>>> refs/remotes/origin/master
			case OTG_STATE_B_WAIT_ACON:
			case OTG_STATE_B_PERIPHERAL:
				/* disconnect while suspended?  we may
				 * not get a disconnect irq...
				 */
				if ((devctl & MUSB_DEVCTL_VBUS)
						!= (3 << MUSB_DEVCTL_VBUS_SHIFT)
						) {
					musb->int_usb |= MUSB_INTR_DISCONNECT;
					musb->int_usb &= ~MUSB_INTR_SUSPEND;
					break;
				}
				musb_g_resume(musb);
				break;
			case OTG_STATE_B_IDLE:
				musb->int_usb &= ~MUSB_INTR_SUSPEND;
				break;
<<<<<<< HEAD
<<<<<<< HEAD
#endif
=======
>>>>>>> refs/remotes/origin/cm-10.0
			default:
				WARNING("bogus %s RESUME (%s)\n",
					"peripheral",
					otg_state_string(musb->xceiv->state));
=======
			default:
				WARNING("bogus %s RESUME (%s)\n",
					"peripheral",
					usb_otg_state_string(musb->xceiv->state));
>>>>>>> refs/remotes/origin/master
			}
		}
	}

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_USB_MUSB_HDRC_HCD
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	/* see manual for the order of the tests */
	if (int_usb & MUSB_INTR_SESSREQ) {
		void __iomem *mbase = musb->mregs;

		if ((devctl & MUSB_DEVCTL_VBUS) == MUSB_DEVCTL_VBUS
				&& (devctl & MUSB_DEVCTL_BDEVICE)) {
			dev_dbg(musb->controller, "SessReq while on B state\n");
			return IRQ_HANDLED;
		}

		dev_dbg(musb->controller, "SESSION_REQUEST (%s)\n",
<<<<<<< HEAD
			otg_state_string(musb->xceiv->state));
=======
			usb_otg_state_string(musb->xceiv->state));
>>>>>>> refs/remotes/origin/master

		/* IRQ arrives from ID pin sense or (later, if VBUS power
		 * is removed) SRP.  responses are time critical:
		 *  - turn on VBUS (with silicon-specific mechanism)
		 *  - go through A_WAIT_VRISE
		 *  - ... to A_WAIT_BCON.
		 * a_wait_vrise_tmout triggers VBUS_ERROR transitions
		 */
		musb_writeb(mbase, MUSB_DEVCTL, MUSB_DEVCTL_SESSION);
		musb->ep0_stage = MUSB_EP0_START;
		musb->xceiv->state = OTG_STATE_A_IDLE;
		MUSB_HST_MODE(musb);
		musb_platform_set_vbus(musb, 1);

		handled = IRQ_HANDLED;
	}

	if (int_usb & MUSB_INTR_VBUSERROR) {
		int	ignore = 0;

		/* During connection as an A-Device, we may see a short
		 * current spikes causing voltage drop, because of cable
		 * and peripheral capacitance combined with vbus draw.
		 * (So: less common with truly self-powered devices, where
		 * vbus doesn't act like a power supply.)
		 *
		 * Such spikes are short; usually less than ~500 usec, max
		 * of ~2 msec.  That is, they're not sustained overcurrent
		 * errors, though they're reported using VBUSERROR irqs.
		 *
		 * Workarounds:  (a) hardware: use self powered devices.
		 * (b) software:  ignore non-repeated VBUS errors.
		 *
		 * REVISIT:  do delays from lots of DEBUG_KERNEL checks
		 * make trouble here, keeping VBUS < 4.4V ?
		 */
		switch (musb->xceiv->state) {
		case OTG_STATE_A_HOST:
			/* recovery is dicey once we've gotten past the
			 * initial stages of enumeration, but if VBUS
			 * stayed ok at the other end of the link, and
			 * another reset is due (at least for high speed,
			 * to redo the chirp etc), it might work OK...
			 */
		case OTG_STATE_A_WAIT_BCON:
		case OTG_STATE_A_WAIT_VRISE:
			if (musb->vbuserr_retry) {
				void __iomem *mbase = musb->mregs;

				musb->vbuserr_retry--;
				ignore = 1;
				devctl |= MUSB_DEVCTL_SESSION;
				musb_writeb(mbase, MUSB_DEVCTL, devctl);
			} else {
				musb->port1_status |=
					  USB_PORT_STAT_OVERCURRENT
					| (USB_PORT_STAT_C_OVERCURRENT << 16);
			}
			break;
		default:
			break;
		}

<<<<<<< HEAD
		dev_dbg(musb->controller, "VBUS_ERROR in %s (%02x, %s), retry #%d, port1 %08x\n",
				otg_state_string(musb->xceiv->state),
=======
		dev_printk(ignore ? KERN_DEBUG : KERN_ERR, musb->controller,
				"VBUS_ERROR in %s (%02x, %s), retry #%d, port1 %08x\n",
				usb_otg_state_string(musb->xceiv->state),
>>>>>>> refs/remotes/origin/master
				devctl,
				({ char *s;
				switch (devctl & MUSB_DEVCTL_VBUS) {
				case 0 << MUSB_DEVCTL_VBUS_SHIFT:
					s = "<SessEnd"; break;
				case 1 << MUSB_DEVCTL_VBUS_SHIFT:
					s = "<AValid"; break;
				case 2 << MUSB_DEVCTL_VBUS_SHIFT:
					s = "<VBusValid"; break;
				/* case 3 << MUSB_DEVCTL_VBUS_SHIFT: */
				default:
					s = "VALID"; break;
<<<<<<< HEAD
				}; s; }),
=======
				} s; }),
>>>>>>> refs/remotes/origin/master
				VBUSERR_RETRY_COUNT - musb->vbuserr_retry,
				musb->port1_status);

		/* go through A_WAIT_VFALL then start a new session */
		if (!ignore)
			musb_platform_set_vbus(musb, 0);
		handled = IRQ_HANDLED;
	}

<<<<<<< HEAD
<<<<<<< HEAD
#endif
=======
>>>>>>> refs/remotes/origin/cm-10.0
	if (int_usb & MUSB_INTR_SUSPEND) {
		dev_dbg(musb->controller, "SUSPEND (%s) devctl %02x power %02x\n",
			otg_state_string(musb->xceiv->state), devctl, power);
		handled = IRQ_HANDLED;

		switch (musb->xceiv->state) {
<<<<<<< HEAD
#ifdef	CONFIG_USB_MUSB_OTG
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (int_usb & MUSB_INTR_SUSPEND) {
		dev_dbg(musb->controller, "SUSPEND (%s) devctl %02x\n",
			usb_otg_state_string(musb->xceiv->state), devctl);
		handled = IRQ_HANDLED;

		switch (musb->xceiv->state) {
>>>>>>> refs/remotes/origin/master
		case OTG_STATE_A_PERIPHERAL:
			/* We also come here if the cable is removed, since
			 * this silicon doesn't report ID-no-longer-grounded.
			 *
			 * We depend on T(a_wait_bcon) to shut us down, and
			 * hope users don't do anything dicey during this
			 * undesired detour through A_WAIT_BCON.
			 */
			musb_hnp_stop(musb);
<<<<<<< HEAD
			usb_hcd_resume_root_hub(musb_to_hcd(musb));
=======
			musb_host_resume_root_hub(musb);
>>>>>>> refs/remotes/origin/master
			musb_root_disconnect(musb);
			musb_platform_try_idle(musb, jiffies
					+ msecs_to_jiffies(musb->a_wait_bcon
						? : OTG_TIME_A_WAIT_BCON));

			break;
<<<<<<< HEAD
<<<<<<< HEAD
#endif
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		case OTG_STATE_B_IDLE:
			if (!musb->is_active)
				break;
		case OTG_STATE_B_PERIPHERAL:
			musb_g_suspend(musb);
<<<<<<< HEAD
			musb->is_active = is_otg_enabled(musb)
<<<<<<< HEAD
					&& musb->xceiv->gadget->b_hnp_enable;
			if (musb->is_active) {
#ifdef	CONFIG_USB_MUSB_OTG
=======
					&& otg->gadget->b_hnp_enable;
			if (musb->is_active) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
			musb->is_active = otg->gadget->b_hnp_enable;
			if (musb->is_active) {
>>>>>>> refs/remotes/origin/master
				musb->xceiv->state = OTG_STATE_B_WAIT_ACON;
				dev_dbg(musb->controller, "HNP: Setting timer for b_ase0_brst\n");
				mod_timer(&musb->otg_timer, jiffies
					+ msecs_to_jiffies(
							OTG_TIME_B_ASE0_BRST));
<<<<<<< HEAD
<<<<<<< HEAD
#endif
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			}
			break;
		case OTG_STATE_A_WAIT_BCON:
			if (musb->a_wait_bcon != 0)
				musb_platform_try_idle(musb, jiffies
					+ msecs_to_jiffies(musb->a_wait_bcon));
			break;
		case OTG_STATE_A_HOST:
			musb->xceiv->state = OTG_STATE_A_SUSPEND;
<<<<<<< HEAD
			musb->is_active = is_otg_enabled(musb)
<<<<<<< HEAD
					&& musb->xceiv->host->b_hnp_enable;
=======
					&& otg->host->b_hnp_enable;
>>>>>>> refs/remotes/origin/cm-10.0
=======
			musb->is_active = otg->host->b_hnp_enable;
>>>>>>> refs/remotes/origin/master
			break;
		case OTG_STATE_B_HOST:
			/* Transition to B_PERIPHERAL, see 6.8.2.6 p 44 */
			dev_dbg(musb->controller, "REVISIT: SUSPEND as B_HOST\n");
			break;
		default:
			/* "should not happen" */
			musb->is_active = 0;
			break;
		}
	}

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_USB_MUSB_HDRC_HCD
=======
>>>>>>> refs/remotes/origin/cm-10.0
	if (int_usb & MUSB_INTR_CONNECT) {
		struct usb_hcd *hcd = musb_to_hcd(musb);

		handled = IRQ_HANDLED;
		musb->is_active = 1;
<<<<<<< HEAD
		set_bit(HCD_FLAG_SAW_IRQ, &hcd->flags);

		musb->ep0_stage = MUSB_EP0_START;

#ifdef CONFIG_USB_MUSB_OTG
=======

		musb->ep0_stage = MUSB_EP0_START;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (int_usb & MUSB_INTR_CONNECT) {
		struct usb_hcd *hcd = musb->hcd;

		handled = IRQ_HANDLED;
		musb->is_active = 1;

		musb->ep0_stage = MUSB_EP0_START;

>>>>>>> refs/remotes/origin/master
		/* flush endpoints when transitioning from Device Mode */
		if (is_peripheral_active(musb)) {
			/* REVISIT HNP; just force disconnect */
		}
<<<<<<< HEAD
		musb_writew(musb->mregs, MUSB_INTRTXE, musb->epmask);
		musb_writew(musb->mregs, MUSB_INTRRXE, musb->epmask & 0xfffe);
		musb_writeb(musb->mregs, MUSB_INTRUSBE, 0xf7);
<<<<<<< HEAD
#endif
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
		musb->intrtxe = musb->epmask;
		musb_writew(musb->mregs, MUSB_INTRTXE, musb->intrtxe);
		musb->intrrxe = musb->epmask & 0xfffe;
		musb_writew(musb->mregs, MUSB_INTRRXE, musb->intrrxe);
		musb_writeb(musb->mregs, MUSB_INTRUSBE, 0xf7);
>>>>>>> refs/remotes/origin/master
		musb->port1_status &= ~(USB_PORT_STAT_LOW_SPEED
					|USB_PORT_STAT_HIGH_SPEED
					|USB_PORT_STAT_ENABLE
					);
		musb->port1_status |= USB_PORT_STAT_CONNECTION
					|(USB_PORT_STAT_C_CONNECTION << 16);

		/* high vs full speed is just a guess until after reset */
		if (devctl & MUSB_DEVCTL_LSDEV)
			musb->port1_status |= USB_PORT_STAT_LOW_SPEED;

		/* indicate new connection to OTG machine */
		switch (musb->xceiv->state) {
		case OTG_STATE_B_PERIPHERAL:
			if (int_usb & MUSB_INTR_SUSPEND) {
				dev_dbg(musb->controller, "HNP: SUSPEND+CONNECT, now b_host\n");
				int_usb &= ~MUSB_INTR_SUSPEND;
				goto b_host;
			} else
				dev_dbg(musb->controller, "CONNECT as b_peripheral???\n");
			break;
		case OTG_STATE_B_WAIT_ACON:
			dev_dbg(musb->controller, "HNP: CONNECT, now b_host\n");
b_host:
			musb->xceiv->state = OTG_STATE_B_HOST;
<<<<<<< HEAD
			hcd->self.is_b_host = 1;
			musb->ignore_disconnect = 0;
=======
			if (musb->hcd)
				musb->hcd->self.is_b_host = 1;
>>>>>>> refs/remotes/origin/master
			del_timer(&musb->otg_timer);
			break;
		default:
			if ((devctl & MUSB_DEVCTL_VBUS)
					== (3 << MUSB_DEVCTL_VBUS_SHIFT)) {
				musb->xceiv->state = OTG_STATE_A_HOST;
<<<<<<< HEAD
				hcd->self.is_b_host = 0;
=======
				if (hcd)
					hcd->self.is_b_host = 0;
>>>>>>> refs/remotes/origin/master
			}
			break;
		}

<<<<<<< HEAD
		/* poke the root hub */
		MUSB_HST_MODE(musb);
		if (hcd->status_urb)
			usb_hcd_poll_rh_status(hcd);
		else
			usb_hcd_resume_root_hub(hcd);

		dev_dbg(musb->controller, "CONNECT (%s) devctl %02x\n",
				otg_state_string(musb->xceiv->state), devctl);
	}
<<<<<<< HEAD
#endif	/* CONFIG_USB_MUSB_HDRC_HCD */
=======
>>>>>>> refs/remotes/origin/cm-10.0

	if ((int_usb & MUSB_INTR_DISCONNECT) && !musb->ignore_disconnect) {
		dev_dbg(musb->controller, "DISCONNECT (%s) as %s, devctl %02x\n",
				otg_state_string(musb->xceiv->state),
=======
		musb_host_poke_root_hub(musb);

		dev_dbg(musb->controller, "CONNECT (%s) devctl %02x\n",
				usb_otg_state_string(musb->xceiv->state), devctl);
	}

	if (int_usb & MUSB_INTR_DISCONNECT) {
		dev_dbg(musb->controller, "DISCONNECT (%s) as %s, devctl %02x\n",
				usb_otg_state_string(musb->xceiv->state),
>>>>>>> refs/remotes/origin/master
				MUSB_MODE(musb), devctl);
		handled = IRQ_HANDLED;

		switch (musb->xceiv->state) {
<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_USB_MUSB_HDRC_HCD
=======
>>>>>>> refs/remotes/origin/cm-10.0
		case OTG_STATE_A_HOST:
		case OTG_STATE_A_SUSPEND:
			usb_hcd_resume_root_hub(musb_to_hcd(musb));
			musb_root_disconnect(musb);
			if (musb->a_wait_bcon != 0 && is_otg_enabled(musb))
				musb_platform_try_idle(musb, jiffies
					+ msecs_to_jiffies(musb->a_wait_bcon));
			break;
<<<<<<< HEAD
#endif	/* HOST */
#ifdef CONFIG_USB_MUSB_OTG
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
		case OTG_STATE_A_HOST:
		case OTG_STATE_A_SUSPEND:
			musb_host_resume_root_hub(musb);
			musb_root_disconnect(musb);
			if (musb->a_wait_bcon != 0)
				musb_platform_try_idle(musb, jiffies
					+ msecs_to_jiffies(musb->a_wait_bcon));
			break;
>>>>>>> refs/remotes/origin/master
		case OTG_STATE_B_HOST:
			/* REVISIT this behaves for "real disconnect"
			 * cases; make sure the other transitions from
			 * from B_HOST act right too.  The B_HOST code
			 * in hnp_stop() is currently not used...
			 */
			musb_root_disconnect(musb);
<<<<<<< HEAD
			musb_to_hcd(musb)->self.is_b_host = 0;
=======
			if (musb->hcd)
				musb->hcd->self.is_b_host = 0;
>>>>>>> refs/remotes/origin/master
			musb->xceiv->state = OTG_STATE_B_PERIPHERAL;
			MUSB_DEV_MODE(musb);
			musb_g_disconnect(musb);
			break;
		case OTG_STATE_A_PERIPHERAL:
			musb_hnp_stop(musb);
			musb_root_disconnect(musb);
			/* FALLTHROUGH */
		case OTG_STATE_B_WAIT_ACON:
			/* FALLTHROUGH */
<<<<<<< HEAD
<<<<<<< HEAD
#endif	/* OTG */
#ifdef CONFIG_USB_GADGET_MUSB_HDRC
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		case OTG_STATE_B_PERIPHERAL:
		case OTG_STATE_B_IDLE:
			musb_g_disconnect(musb);
			break;
<<<<<<< HEAD
<<<<<<< HEAD
#endif	/* GADGET */
=======
>>>>>>> refs/remotes/origin/cm-10.0
		default:
			WARNING("unhandled DISCONNECT transition (%s)\n",
				otg_state_string(musb->xceiv->state));
=======
		default:
			WARNING("unhandled DISCONNECT transition (%s)\n",
				usb_otg_state_string(musb->xceiv->state));
>>>>>>> refs/remotes/origin/master
			break;
		}
	}

	/* mentor saves a bit: bus reset and babble share the same irq.
	 * only host sees babble; only peripheral sees bus reset.
	 */
	if (int_usb & MUSB_INTR_RESET) {
		handled = IRQ_HANDLED;
<<<<<<< HEAD
		if (is_host_capable() && (devctl & MUSB_DEVCTL_HM) != 0) {
=======
		if ((devctl & MUSB_DEVCTL_HM) != 0) {
>>>>>>> refs/remotes/origin/master
			/*
			 * Looks like non-HS BABBLE can be ignored, but
			 * HS BABBLE is an error condition. For HS the solution
			 * is to avoid babble in the first place and fix what
			 * caused BABBLE. When HS BABBLE happens we can only
			 * stop the session.
			 */
			if (devctl & (MUSB_DEVCTL_FSDEV | MUSB_DEVCTL_LSDEV))
				dev_dbg(musb->controller, "BABBLE devctl: %02x\n", devctl);
			else {
				ERR("Stopping host session -- babble\n");
				musb_writeb(musb->mregs, MUSB_DEVCTL, 0);
			}
<<<<<<< HEAD
		} else if (is_peripheral_capable()) {
			dev_dbg(musb->controller, "BUS RESET as %s\n",
				otg_state_string(musb->xceiv->state));
			switch (musb->xceiv->state) {
<<<<<<< HEAD
#ifdef CONFIG_USB_OTG
=======
>>>>>>> refs/remotes/origin/cm-10.0
			case OTG_STATE_A_SUSPEND:
				/* We need to ignore disconnect on suspend
				 * otherwise tusb 2.0 won't reconnect after a
				 * power cycle, which breaks otg compliance.
				 */
				musb->ignore_disconnect = 1;
=======
		} else {
			dev_dbg(musb->controller, "BUS RESET as %s\n",
				usb_otg_state_string(musb->xceiv->state));
			switch (musb->xceiv->state) {
			case OTG_STATE_A_SUSPEND:
>>>>>>> refs/remotes/origin/master
				musb_g_reset(musb);
				/* FALLTHROUGH */
			case OTG_STATE_A_WAIT_BCON:	/* OPT TD.4.7-900ms */
				/* never use invalid T(a_wait_bcon) */
				dev_dbg(musb->controller, "HNP: in %s, %d msec timeout\n",
<<<<<<< HEAD
					otg_state_string(musb->xceiv->state),
=======
					usb_otg_state_string(musb->xceiv->state),
>>>>>>> refs/remotes/origin/master
					TA_WAIT_BCON(musb));
				mod_timer(&musb->otg_timer, jiffies
					+ msecs_to_jiffies(TA_WAIT_BCON(musb)));
				break;
			case OTG_STATE_A_PERIPHERAL:
<<<<<<< HEAD
				musb->ignore_disconnect = 0;
=======
>>>>>>> refs/remotes/origin/master
				del_timer(&musb->otg_timer);
				musb_g_reset(musb);
				break;
			case OTG_STATE_B_WAIT_ACON:
				dev_dbg(musb->controller, "HNP: RESET (%s), to b_peripheral\n",
<<<<<<< HEAD
					otg_state_string(musb->xceiv->state));
				musb->xceiv->state = OTG_STATE_B_PERIPHERAL;
				musb_g_reset(musb);
				break;
<<<<<<< HEAD
#endif
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
					usb_otg_state_string(musb->xceiv->state));
				musb->xceiv->state = OTG_STATE_B_PERIPHERAL;
				musb_g_reset(musb);
				break;
>>>>>>> refs/remotes/origin/master
			case OTG_STATE_B_IDLE:
				musb->xceiv->state = OTG_STATE_B_PERIPHERAL;
				/* FALLTHROUGH */
			case OTG_STATE_B_PERIPHERAL:
				musb_g_reset(musb);
				break;
			default:
				dev_dbg(musb->controller, "Unhandled BUS RESET as %s\n",
<<<<<<< HEAD
					otg_state_string(musb->xceiv->state));
=======
					usb_otg_state_string(musb->xceiv->state));
>>>>>>> refs/remotes/origin/master
			}
		}
	}

#if 0
/* REVISIT ... this would be for multiplexing periodic endpoints, or
 * supporting transfer phasing to prevent exceeding ISO bandwidth
 * limits of a given frame or microframe.
 *
 * It's not needed for peripheral side, which dedicates endpoints;
 * though it _might_ use SOF irqs for other purposes.
 *
 * And it's not currently needed for host side, which also dedicates
 * endpoints, relies on TX/RX interval registers, and isn't claimed
 * to support ISO transfers yet.
 */
	if (int_usb & MUSB_INTR_SOF) {
		void __iomem *mbase = musb->mregs;
		struct musb_hw_ep	*ep;
		u8 epnum;
		u16 frame;

		dev_dbg(musb->controller, "START_OF_FRAME\n");
		handled = IRQ_HANDLED;

		/* start any periodic Tx transfers waiting for current frame */
		frame = musb_readw(mbase, MUSB_FRAME);
		ep = musb->endpoints;
		for (epnum = 1; (epnum < musb->nr_endpoints)
					&& (musb->epmask >= (1 << epnum));
				epnum++, ep++) {
			/*
			 * FIXME handle framecounter wraps (12 bits)
			 * eliminate duplicated StartUrb logic
			 */
			if (ep->dwWaitFrame >= frame) {
				ep->dwWaitFrame = 0;
				pr_debug("SOF --> periodic TX%s on %d\n",
					ep->tx_channel ? " DMA" : "",
					epnum);
				if (!ep->tx_channel)
					musb_h_tx_start(musb, epnum);
				else
					cppi_hostdma_start(musb, epnum);
			}
		}		/* end of for loop */
	}
#endif

	schedule_work(&musb->irq_work);

	return handled;
}

/*-------------------------------------------------------------------------*/

<<<<<<< HEAD
/*
* Program the HDRC to start (enable interrupts, dma, etc.).
*/
void musb_start(struct musb *musb)
{
	void __iomem	*regs = musb->mregs;
	u8		devctl = musb_readb(regs, MUSB_DEVCTL);
=======
static void musb_generic_disable(struct musb *musb)
{
	void __iomem	*mbase = musb->mregs;
	u16	temp;

	/* disable interrupts */
	musb_writeb(mbase, MUSB_INTRUSBE, 0);
	musb->intrtxe = 0;
	musb_writew(mbase, MUSB_INTRTXE, 0);
	musb->intrrxe = 0;
	musb_writew(mbase, MUSB_INTRRXE, 0);

	/* off */
	musb_writeb(mbase, MUSB_DEVCTL, 0);

	/*  flush pending interrupts */
	temp = musb_readb(mbase, MUSB_INTRUSB);
	temp = musb_readw(mbase, MUSB_INTRTX);
	temp = musb_readw(mbase, MUSB_INTRRX);

}

/*
 * Program the HDRC to start (enable interrupts, dma, etc.).
 */
void musb_start(struct musb *musb)
{
	void __iomem    *regs = musb->mregs;
	u8              devctl = musb_readb(regs, MUSB_DEVCTL);
>>>>>>> refs/remotes/origin/master

	dev_dbg(musb->controller, "<== devctl %02x\n", devctl);

	/*  Set INT enable registers, enable interrupts */
<<<<<<< HEAD
	musb_writew(regs, MUSB_INTRTXE, musb->epmask);
	musb_writew(regs, MUSB_INTRRXE, musb->epmask & 0xfffe);
=======
	musb->intrtxe = musb->epmask;
	musb_writew(regs, MUSB_INTRTXE, musb->intrtxe);
	musb->intrrxe = musb->epmask & 0xfffe;
	musb_writew(regs, MUSB_INTRRXE, musb->intrrxe);
>>>>>>> refs/remotes/origin/master
	musb_writeb(regs, MUSB_INTRUSBE, 0xf7);

	musb_writeb(regs, MUSB_TESTMODE, 0);

	/* put into basic highspeed mode and start session */
	musb_writeb(regs, MUSB_POWER, MUSB_POWER_ISOUPDATE
<<<<<<< HEAD
<<<<<<< HEAD
						| MUSB_POWER_SOFTCONN
=======
>>>>>>> refs/remotes/origin/cm-10.0
						| MUSB_POWER_HSENAB
						/* ENSUSPEND wedges tusb */
						/* | MUSB_POWER_ENSUSPEND */
						);
=======
			| MUSB_POWER_HSENAB
			/* ENSUSPEND wedges tusb */
			/* | MUSB_POWER_ENSUSPEND */
		   );
>>>>>>> refs/remotes/origin/master

	musb->is_active = 0;
	devctl = musb_readb(regs, MUSB_DEVCTL);
	devctl &= ~MUSB_DEVCTL_SESSION;

<<<<<<< HEAD
	if (is_otg_enabled(musb)) {
		/* session started after:
		 * (a) ID-grounded irq, host mode;
		 * (b) vbus present/connect IRQ, peripheral mode;
		 * (c) peripheral initiates, using SRP
		 */
		if ((devctl & MUSB_DEVCTL_VBUS) == MUSB_DEVCTL_VBUS)
			musb->is_active = 1;
		else
			devctl |= MUSB_DEVCTL_SESSION;

	} else if (is_host_enabled(musb)) {
		/* assume ID pin is hard-wired to ground */
		devctl |= MUSB_DEVCTL_SESSION;

	} else /* peripheral is enabled */ {
		if ((devctl & MUSB_DEVCTL_VBUS) == MUSB_DEVCTL_VBUS)
			musb->is_active = 1;
	}
=======
	/* session started after:
	 * (a) ID-grounded irq, host mode;
	 * (b) vbus present/connect IRQ, peripheral mode;
	 * (c) peripheral initiates, using SRP
	 */
	if (musb->port_mode != MUSB_PORT_MODE_HOST &&
			(devctl & MUSB_DEVCTL_VBUS) == MUSB_DEVCTL_VBUS) {
		musb->is_active = 1;
	} else {
		devctl |= MUSB_DEVCTL_SESSION;
	}

>>>>>>> refs/remotes/origin/master
	musb_platform_enable(musb);
	musb_writeb(regs, MUSB_DEVCTL, devctl);
}

<<<<<<< HEAD

static void musb_generic_disable(struct musb *musb)
{
	void __iomem	*mbase = musb->mregs;
	u16	temp;

	/* disable interrupts */
	musb_writeb(mbase, MUSB_INTRUSBE, 0);
	musb_writew(mbase, MUSB_INTRTXE, 0);
	musb_writew(mbase, MUSB_INTRRXE, 0);

	/* off */
	musb_writeb(mbase, MUSB_DEVCTL, 0);

	/*  flush pending interrupts */
	temp = musb_readb(mbase, MUSB_INTRUSB);
	temp = musb_readw(mbase, MUSB_INTRTX);
	temp = musb_readw(mbase, MUSB_INTRRX);

}

=======
>>>>>>> refs/remotes/origin/master
/*
 * Make the HDRC stop (disable interrupts, etc.);
 * reversible by musb_start
 * called on gadget driver unregister
 * with controller locked, irqs blocked
 * acts as a NOP unless some role activated the hardware
 */
void musb_stop(struct musb *musb)
{
	/* stop IRQs, timers, ... */
	musb_platform_disable(musb);
	musb_generic_disable(musb);
	dev_dbg(musb->controller, "HDRC disabled\n");

	/* FIXME
	 *  - mark host and/or peripheral drivers unusable/inactive
	 *  - disable DMA (and enable it in HdrcStart)
	 *  - make sure we can musb_start() after musb_stop(); with
	 *    OTG mode, gadget driver module rmmod/modprobe cycles that
	 *  - ...
	 */
	musb_platform_try_idle(musb, 0);
}

static void musb_shutdown(struct platform_device *pdev)
{
	struct musb	*musb = dev_to_musb(&pdev->dev);
	unsigned long	flags;

	pm_runtime_get_sync(musb->controller);
<<<<<<< HEAD
<<<<<<< HEAD
=======

	musb_gadget_cleanup(musb);

>>>>>>> refs/remotes/origin/cm-10.0
=======

	musb_host_cleanup(musb);
	musb_gadget_cleanup(musb);

>>>>>>> refs/remotes/origin/master
	spin_lock_irqsave(&musb->lock, flags);
	musb_platform_disable(musb);
	musb_generic_disable(musb);
	spin_unlock_irqrestore(&musb->lock, flags);

<<<<<<< HEAD
	if (!is_otg_enabled(musb) && is_host_enabled(musb))
		usb_remove_hcd(musb_to_hcd(musb));
=======
>>>>>>> refs/remotes/origin/master
	musb_writeb(musb->mregs, MUSB_DEVCTL, 0);
	musb_platform_exit(musb);

	pm_runtime_put(musb->controller);
	/* FIXME power down */
}


/*-------------------------------------------------------------------------*/

/*
 * The silicon either has hard-wired endpoint configurations, or else
 * "dynamic fifo" sizing.  The driver has support for both, though at this
 * writing only the dynamic sizing is very well tested.   Since we switched
 * away from compile-time hardware parameters, we can no longer rely on
 * dead code elimination to leave only the relevant one in the object file.
 *
 * We don't currently use dynamic fifo setup capability to do anything
 * more than selecting one of a bunch of predefined configurations.
 */
<<<<<<< HEAD
<<<<<<< HEAD
#if defined(CONFIG_USB_MUSB_TUSB6010) || defined(CONFIG_USB_MUSB_OMAP2PLUS) \
	|| defined(CONFIG_USB_MUSB_AM35X)
static ushort __initdata fifo_mode = 4;
#elif defined(CONFIG_USB_MUSB_UX500)
static ushort __initdata fifo_mode = 5;
#else
static ushort __initdata fifo_mode = 2;
=======
=======
>>>>>>> refs/remotes/origin/master
#if defined(CONFIG_USB_MUSB_TUSB6010)			\
	|| defined(CONFIG_USB_MUSB_TUSB6010_MODULE)	\
	|| defined(CONFIG_USB_MUSB_OMAP2PLUS)		\
	|| defined(CONFIG_USB_MUSB_OMAP2PLUS_MODULE)	\
	|| defined(CONFIG_USB_MUSB_AM35X)		\
<<<<<<< HEAD
	|| defined(CONFIG_USB_MUSB_AM35X_MODULE)
static ushort __devinitdata fifo_mode = 4;
#elif defined(CONFIG_USB_MUSB_UX500)			\
	|| defined(CONFIG_USB_MUSB_UX500_MODULE)
static ushort __devinitdata fifo_mode = 5;
#else
static ushort __devinitdata fifo_mode = 2;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	|| defined(CONFIG_USB_MUSB_AM35X_MODULE)	\
	|| defined(CONFIG_USB_MUSB_DSPS)		\
	|| defined(CONFIG_USB_MUSB_DSPS_MODULE)
static ushort fifo_mode = 4;
#elif defined(CONFIG_USB_MUSB_UX500)			\
	|| defined(CONFIG_USB_MUSB_UX500_MODULE)
static ushort fifo_mode = 5;
#else
static ushort fifo_mode = 2;
>>>>>>> refs/remotes/origin/master
#endif

/* "modprobe ... fifo_mode=1" etc */
module_param(fifo_mode, ushort, 0);
MODULE_PARM_DESC(fifo_mode, "initial endpoint configuration");

/*
 * tables defining fifo_mode values.  define more if you like.
 * for host side, make sure both halves of ep1 are set up.
 */

/* mode 0 - fits in 2KB */
<<<<<<< HEAD
<<<<<<< HEAD
static struct musb_fifo_cfg __initdata mode_0_cfg[] = {
=======
static struct musb_fifo_cfg __devinitdata mode_0_cfg[] = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct musb_fifo_cfg mode_0_cfg[] = {
>>>>>>> refs/remotes/origin/master
{ .hw_ep_num = 1, .style = FIFO_TX,   .maxpacket = 512, },
{ .hw_ep_num = 1, .style = FIFO_RX,   .maxpacket = 512, },
{ .hw_ep_num = 2, .style = FIFO_RXTX, .maxpacket = 512, },
{ .hw_ep_num = 3, .style = FIFO_RXTX, .maxpacket = 256, },
{ .hw_ep_num = 4, .style = FIFO_RXTX, .maxpacket = 256, },
};

/* mode 1 - fits in 4KB */
<<<<<<< HEAD
<<<<<<< HEAD
static struct musb_fifo_cfg __initdata mode_1_cfg[] = {
=======
static struct musb_fifo_cfg __devinitdata mode_1_cfg[] = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct musb_fifo_cfg mode_1_cfg[] = {
>>>>>>> refs/remotes/origin/master
{ .hw_ep_num = 1, .style = FIFO_TX,   .maxpacket = 512, .mode = BUF_DOUBLE, },
{ .hw_ep_num = 1, .style = FIFO_RX,   .maxpacket = 512, .mode = BUF_DOUBLE, },
{ .hw_ep_num = 2, .style = FIFO_RXTX, .maxpacket = 512, .mode = BUF_DOUBLE, },
{ .hw_ep_num = 3, .style = FIFO_RXTX, .maxpacket = 256, },
{ .hw_ep_num = 4, .style = FIFO_RXTX, .maxpacket = 256, },
};

/* mode 2 - fits in 4KB */
<<<<<<< HEAD
<<<<<<< HEAD
static struct musb_fifo_cfg __initdata mode_2_cfg[] = {
=======
static struct musb_fifo_cfg __devinitdata mode_2_cfg[] = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct musb_fifo_cfg mode_2_cfg[] = {
>>>>>>> refs/remotes/origin/master
{ .hw_ep_num = 1, .style = FIFO_TX,   .maxpacket = 512, },
{ .hw_ep_num = 1, .style = FIFO_RX,   .maxpacket = 512, },
{ .hw_ep_num = 2, .style = FIFO_TX,   .maxpacket = 512, },
{ .hw_ep_num = 2, .style = FIFO_RX,   .maxpacket = 512, },
{ .hw_ep_num = 3, .style = FIFO_RXTX, .maxpacket = 256, },
{ .hw_ep_num = 4, .style = FIFO_RXTX, .maxpacket = 256, },
};

/* mode 3 - fits in 4KB */
<<<<<<< HEAD
<<<<<<< HEAD
static struct musb_fifo_cfg __initdata mode_3_cfg[] = {
=======
static struct musb_fifo_cfg __devinitdata mode_3_cfg[] = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct musb_fifo_cfg mode_3_cfg[] = {
>>>>>>> refs/remotes/origin/master
{ .hw_ep_num = 1, .style = FIFO_TX,   .maxpacket = 512, .mode = BUF_DOUBLE, },
{ .hw_ep_num = 1, .style = FIFO_RX,   .maxpacket = 512, .mode = BUF_DOUBLE, },
{ .hw_ep_num = 2, .style = FIFO_TX,   .maxpacket = 512, },
{ .hw_ep_num = 2, .style = FIFO_RX,   .maxpacket = 512, },
{ .hw_ep_num = 3, .style = FIFO_RXTX, .maxpacket = 256, },
{ .hw_ep_num = 4, .style = FIFO_RXTX, .maxpacket = 256, },
};

/* mode 4 - fits in 16KB */
<<<<<<< HEAD
<<<<<<< HEAD
static struct musb_fifo_cfg __initdata mode_4_cfg[] = {
=======
static struct musb_fifo_cfg __devinitdata mode_4_cfg[] = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct musb_fifo_cfg mode_4_cfg[] = {
>>>>>>> refs/remotes/origin/master
{ .hw_ep_num =  1, .style = FIFO_TX,   .maxpacket = 512, },
{ .hw_ep_num =  1, .style = FIFO_RX,   .maxpacket = 512, },
{ .hw_ep_num =  2, .style = FIFO_TX,   .maxpacket = 512, },
{ .hw_ep_num =  2, .style = FIFO_RX,   .maxpacket = 512, },
{ .hw_ep_num =  3, .style = FIFO_TX,   .maxpacket = 512, },
{ .hw_ep_num =  3, .style = FIFO_RX,   .maxpacket = 512, },
{ .hw_ep_num =  4, .style = FIFO_TX,   .maxpacket = 512, },
{ .hw_ep_num =  4, .style = FIFO_RX,   .maxpacket = 512, },
{ .hw_ep_num =  5, .style = FIFO_TX,   .maxpacket = 512, },
{ .hw_ep_num =  5, .style = FIFO_RX,   .maxpacket = 512, },
{ .hw_ep_num =  6, .style = FIFO_TX,   .maxpacket = 512, },
{ .hw_ep_num =  6, .style = FIFO_RX,   .maxpacket = 512, },
{ .hw_ep_num =  7, .style = FIFO_TX,   .maxpacket = 512, },
{ .hw_ep_num =  7, .style = FIFO_RX,   .maxpacket = 512, },
{ .hw_ep_num =  8, .style = FIFO_TX,   .maxpacket = 512, },
{ .hw_ep_num =  8, .style = FIFO_RX,   .maxpacket = 512, },
{ .hw_ep_num =  9, .style = FIFO_TX,   .maxpacket = 512, },
{ .hw_ep_num =  9, .style = FIFO_RX,   .maxpacket = 512, },
{ .hw_ep_num = 10, .style = FIFO_TX,   .maxpacket = 256, },
{ .hw_ep_num = 10, .style = FIFO_RX,   .maxpacket = 64, },
{ .hw_ep_num = 11, .style = FIFO_TX,   .maxpacket = 256, },
{ .hw_ep_num = 11, .style = FIFO_RX,   .maxpacket = 64, },
{ .hw_ep_num = 12, .style = FIFO_TX,   .maxpacket = 256, },
{ .hw_ep_num = 12, .style = FIFO_RX,   .maxpacket = 64, },
{ .hw_ep_num = 13, .style = FIFO_RXTX, .maxpacket = 4096, },
{ .hw_ep_num = 14, .style = FIFO_RXTX, .maxpacket = 1024, },
{ .hw_ep_num = 15, .style = FIFO_RXTX, .maxpacket = 1024, },
};

/* mode 5 - fits in 8KB */
<<<<<<< HEAD
<<<<<<< HEAD
static struct musb_fifo_cfg __initdata mode_5_cfg[] = {
=======
static struct musb_fifo_cfg __devinitdata mode_5_cfg[] = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct musb_fifo_cfg mode_5_cfg[] = {
>>>>>>> refs/remotes/origin/master
{ .hw_ep_num =  1, .style = FIFO_TX,   .maxpacket = 512, },
{ .hw_ep_num =  1, .style = FIFO_RX,   .maxpacket = 512, },
{ .hw_ep_num =  2, .style = FIFO_TX,   .maxpacket = 512, },
{ .hw_ep_num =  2, .style = FIFO_RX,   .maxpacket = 512, },
{ .hw_ep_num =  3, .style = FIFO_TX,   .maxpacket = 512, },
{ .hw_ep_num =  3, .style = FIFO_RX,   .maxpacket = 512, },
{ .hw_ep_num =  4, .style = FIFO_TX,   .maxpacket = 512, },
{ .hw_ep_num =  4, .style = FIFO_RX,   .maxpacket = 512, },
{ .hw_ep_num =  5, .style = FIFO_TX,   .maxpacket = 512, },
{ .hw_ep_num =  5, .style = FIFO_RX,   .maxpacket = 512, },
{ .hw_ep_num =  6, .style = FIFO_TX,   .maxpacket = 32, },
{ .hw_ep_num =  6, .style = FIFO_RX,   .maxpacket = 32, },
{ .hw_ep_num =  7, .style = FIFO_TX,   .maxpacket = 32, },
{ .hw_ep_num =  7, .style = FIFO_RX,   .maxpacket = 32, },
{ .hw_ep_num =  8, .style = FIFO_TX,   .maxpacket = 32, },
{ .hw_ep_num =  8, .style = FIFO_RX,   .maxpacket = 32, },
{ .hw_ep_num =  9, .style = FIFO_TX,   .maxpacket = 32, },
{ .hw_ep_num =  9, .style = FIFO_RX,   .maxpacket = 32, },
{ .hw_ep_num = 10, .style = FIFO_TX,   .maxpacket = 32, },
{ .hw_ep_num = 10, .style = FIFO_RX,   .maxpacket = 32, },
{ .hw_ep_num = 11, .style = FIFO_TX,   .maxpacket = 32, },
{ .hw_ep_num = 11, .style = FIFO_RX,   .maxpacket = 32, },
{ .hw_ep_num = 12, .style = FIFO_TX,   .maxpacket = 32, },
{ .hw_ep_num = 12, .style = FIFO_RX,   .maxpacket = 32, },
{ .hw_ep_num = 13, .style = FIFO_RXTX, .maxpacket = 512, },
{ .hw_ep_num = 14, .style = FIFO_RXTX, .maxpacket = 1024, },
{ .hw_ep_num = 15, .style = FIFO_RXTX, .maxpacket = 1024, },
};

/*
 * configure a fifo; for non-shared endpoints, this may be called
 * once for a tx fifo and once for an rx fifo.
 *
 * returns negative errno or offset for next fifo.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static int __init
=======
static int __devinit
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int
>>>>>>> refs/remotes/origin/master
fifo_setup(struct musb *musb, struct musb_hw_ep  *hw_ep,
		const struct musb_fifo_cfg *cfg, u16 offset)
{
	void __iomem	*mbase = musb->mregs;
	int	size = 0;
	u16	maxpacket = cfg->maxpacket;
	u16	c_off = offset >> 3;
	u8	c_size;

	/* expect hw_ep has already been zero-initialized */

	size = ffs(max(maxpacket, (u16) 8)) - 1;
	maxpacket = 1 << size;

	c_size = size - 3;
	if (cfg->mode == BUF_DOUBLE) {
		if ((offset + (maxpacket << 1)) >
				(1 << (musb->config->ram_bits + 2)))
			return -EMSGSIZE;
		c_size |= MUSB_FIFOSZ_DPB;
	} else {
		if ((offset + maxpacket) > (1 << (musb->config->ram_bits + 2)))
			return -EMSGSIZE;
	}

	/* configure the FIFO */
	musb_writeb(mbase, MUSB_INDEX, hw_ep->epnum);

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_USB_MUSB_HDRC_HCD
=======
>>>>>>> refs/remotes/origin/cm-10.0
	/* EP0 reserved endpoint for control, bidirectional;
	 * EP1 reserved for bulk, two unidirection halves.
=======
	/* EP0 reserved endpoint for control, bidirectional;
	 * EP1 reserved for bulk, two unidirectional halves.
>>>>>>> refs/remotes/origin/master
	 */
	if (hw_ep->epnum == 1)
		musb->bulk_ep = hw_ep;
	/* REVISIT error check:  be sure ep0 can both rx and tx ... */
<<<<<<< HEAD
<<<<<<< HEAD
#endif
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	switch (cfg->style) {
	case FIFO_TX:
		musb_write_txfifosz(mbase, c_size);
		musb_write_txfifoadd(mbase, c_off);
		hw_ep->tx_double_buffered = !!(c_size & MUSB_FIFOSZ_DPB);
		hw_ep->max_packet_sz_tx = maxpacket;
		break;
	case FIFO_RX:
		musb_write_rxfifosz(mbase, c_size);
		musb_write_rxfifoadd(mbase, c_off);
		hw_ep->rx_double_buffered = !!(c_size & MUSB_FIFOSZ_DPB);
		hw_ep->max_packet_sz_rx = maxpacket;
		break;
	case FIFO_RXTX:
		musb_write_txfifosz(mbase, c_size);
		musb_write_txfifoadd(mbase, c_off);
		hw_ep->rx_double_buffered = !!(c_size & MUSB_FIFOSZ_DPB);
		hw_ep->max_packet_sz_rx = maxpacket;

		musb_write_rxfifosz(mbase, c_size);
		musb_write_rxfifoadd(mbase, c_off);
		hw_ep->tx_double_buffered = hw_ep->rx_double_buffered;
		hw_ep->max_packet_sz_tx = maxpacket;

		hw_ep->is_shared_fifo = true;
		break;
	}

	/* NOTE rx and tx endpoint irqs aren't managed separately,
	 * which happens to be ok
	 */
	musb->epmask |= (1 << hw_ep->epnum);

	return offset + (maxpacket << ((c_size & MUSB_FIFOSZ_DPB) ? 1 : 0));
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct musb_fifo_cfg __initdata ep0_cfg = {
	.style = FIFO_RXTX, .maxpacket = 64,
};

static int __init ep_config_from_table(struct musb *musb)
=======
static struct musb_fifo_cfg __devinitdata ep0_cfg = {
	.style = FIFO_RXTX, .maxpacket = 64,
};

static int __devinit ep_config_from_table(struct musb *musb)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct musb_fifo_cfg ep0_cfg = {
	.style = FIFO_RXTX, .maxpacket = 64,
};

static int ep_config_from_table(struct musb *musb)
>>>>>>> refs/remotes/origin/master
{
	const struct musb_fifo_cfg	*cfg;
	unsigned		i, n;
	int			offset;
	struct musb_hw_ep	*hw_ep = musb->endpoints;

	if (musb->config->fifo_cfg) {
		cfg = musb->config->fifo_cfg;
		n = musb->config->fifo_cfg_size;
		goto done;
	}

	switch (fifo_mode) {
	default:
		fifo_mode = 0;
		/* FALLTHROUGH */
	case 0:
		cfg = mode_0_cfg;
		n = ARRAY_SIZE(mode_0_cfg);
		break;
	case 1:
		cfg = mode_1_cfg;
		n = ARRAY_SIZE(mode_1_cfg);
		break;
	case 2:
		cfg = mode_2_cfg;
		n = ARRAY_SIZE(mode_2_cfg);
		break;
	case 3:
		cfg = mode_3_cfg;
		n = ARRAY_SIZE(mode_3_cfg);
		break;
	case 4:
		cfg = mode_4_cfg;
		n = ARRAY_SIZE(mode_4_cfg);
		break;
	case 5:
		cfg = mode_5_cfg;
		n = ARRAY_SIZE(mode_5_cfg);
		break;
	}

	printk(KERN_DEBUG "%s: setup fifo_mode %d\n",
			musb_driver_name, fifo_mode);


done:
	offset = fifo_setup(musb, hw_ep, &ep0_cfg, 0);
	/* assert(offset > 0) */

	/* NOTE:  for RTL versions >= 1.400 EPINFO and RAMINFO would
	 * be better than static musb->config->num_eps and DYN_FIFO_SIZE...
	 */

	for (i = 0; i < n; i++) {
		u8	epn = cfg->hw_ep_num;

		if (epn >= musb->config->num_eps) {
			pr_debug("%s: invalid ep %d\n",
					musb_driver_name, epn);
			return -EINVAL;
		}
		offset = fifo_setup(musb, hw_ep + epn, cfg++, offset);
		if (offset < 0) {
			pr_debug("%s: mem overrun, ep %d\n",
					musb_driver_name, epn);
<<<<<<< HEAD
			return -EINVAL;
=======
			return offset;
>>>>>>> refs/remotes/origin/master
		}
		epn++;
		musb->nr_endpoints = max(epn, musb->nr_endpoints);
	}

	printk(KERN_DEBUG "%s: %d/%d max ep, %d/%d memory\n",
			musb_driver_name,
			n + 1, musb->config->num_eps * 2 - 1,
			offset, (1 << (musb->config->ram_bits + 2)));

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_USB_MUSB_HDRC_HCD
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (!musb->bulk_ep) {
		pr_debug("%s: missing bulk\n", musb_driver_name);
		return -EINVAL;
	}
<<<<<<< HEAD
<<<<<<< HEAD
#endif
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return 0;
}


/*
 * ep_config_from_hw - when MUSB_C_DYNFIFO_DEF is false
 * @param musb the controller
 */
<<<<<<< HEAD
<<<<<<< HEAD
static int __init ep_config_from_hw(struct musb *musb)
=======
static int __devinit ep_config_from_hw(struct musb *musb)
>>>>>>> refs/remotes/origin/cm-10.0
{
	u8 epnum = 0;
	struct musb_hw_ep *hw_ep;
	void *mbase = musb->mregs;
=======
static int ep_config_from_hw(struct musb *musb)
{
	u8 epnum = 0;
	struct musb_hw_ep *hw_ep;
	void __iomem *mbase = musb->mregs;
>>>>>>> refs/remotes/origin/master
	int ret = 0;

	dev_dbg(musb->controller, "<== static silicon ep config\n");

	/* FIXME pick up ep0 maxpacket size */

	for (epnum = 1; epnum < musb->config->num_eps; epnum++) {
		musb_ep_select(mbase, epnum);
		hw_ep = musb->endpoints + epnum;

		ret = musb_read_fifosize(musb, hw_ep, epnum);
		if (ret < 0)
			break;

		/* FIXME set up hw_ep->{rx,tx}_double_buffered */

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_USB_MUSB_HDRC_HCD
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		/* pick an RX/TX endpoint for bulk */
		if (hw_ep->max_packet_sz_tx < 512
				|| hw_ep->max_packet_sz_rx < 512)
			continue;

		/* REVISIT:  this algorithm is lazy, we should at least
		 * try to pick a double buffered endpoint.
		 */
		if (musb->bulk_ep)
			continue;
		musb->bulk_ep = hw_ep;
<<<<<<< HEAD
<<<<<<< HEAD
#endif
	}

#ifdef CONFIG_USB_MUSB_HDRC_HCD
=======
	}

>>>>>>> refs/remotes/origin/cm-10.0
=======
	}

>>>>>>> refs/remotes/origin/master
	if (!musb->bulk_ep) {
		pr_debug("%s: missing bulk\n", musb_driver_name);
		return -EINVAL;
	}
<<<<<<< HEAD
<<<<<<< HEAD
#endif
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return 0;
}

enum { MUSB_CONTROLLER_MHDRC, MUSB_CONTROLLER_HDRC, };

/* Initialize MUSB (M)HDRC part of the USB hardware subsystem;
 * configure endpoints, or take their config from silicon
 */
<<<<<<< HEAD
<<<<<<< HEAD
static int __init musb_core_init(u16 musb_type, struct musb *musb)
=======
static int __devinit musb_core_init(u16 musb_type, struct musb *musb)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int musb_core_init(u16 musb_type, struct musb *musb)
>>>>>>> refs/remotes/origin/master
{
	u8 reg;
	char *type;
	char aInfo[90], aRevision[32], aDate[12];
	void __iomem	*mbase = musb->mregs;
	int		status = 0;
	int		i;

	/* log core options (read using indexed model) */
	reg = musb_read_configdata(mbase);

	strcpy(aInfo, (reg & MUSB_CONFIGDATA_UTMIDW) ? "UTMI-16" : "UTMI-8");
	if (reg & MUSB_CONFIGDATA_DYNFIFO) {
		strcat(aInfo, ", dyn FIFOs");
		musb->dyn_fifo = true;
	}
	if (reg & MUSB_CONFIGDATA_MPRXE) {
		strcat(aInfo, ", bulk combine");
		musb->bulk_combine = true;
	}
	if (reg & MUSB_CONFIGDATA_MPTXE) {
		strcat(aInfo, ", bulk split");
		musb->bulk_split = true;
	}
	if (reg & MUSB_CONFIGDATA_HBRXE) {
		strcat(aInfo, ", HB-ISO Rx");
		musb->hb_iso_rx = true;
	}
	if (reg & MUSB_CONFIGDATA_HBTXE) {
		strcat(aInfo, ", HB-ISO Tx");
		musb->hb_iso_tx = true;
	}
	if (reg & MUSB_CONFIGDATA_SOFTCONE)
		strcat(aInfo, ", SoftConn");

	printk(KERN_DEBUG "%s: ConfigData=0x%02x (%s)\n",
			musb_driver_name, reg, aInfo);

	aDate[0] = 0;
	if (MUSB_CONTROLLER_MHDRC == musb_type) {
		musb->is_multipoint = 1;
		type = "M";
	} else {
		musb->is_multipoint = 0;
		type = "";
<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_USB_MUSB_HDRC_HCD
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#ifndef	CONFIG_USB_OTG_BLACKLIST_HUB
		printk(KERN_ERR
			"%s: kernel must blacklist external hubs\n",
			musb_driver_name);
#endif
<<<<<<< HEAD
<<<<<<< HEAD
#endif
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	/* log release info */
	musb->hwvers = musb_read_hwvers(mbase);
	snprintf(aRevision, 32, "%d.%d%s", MUSB_HWVERS_MAJOR(musb->hwvers),
		MUSB_HWVERS_MINOR(musb->hwvers),
		(musb->hwvers & MUSB_HWVERS_RC) ? "RC" : "");
	printk(KERN_DEBUG "%s: %sHDRC RTL version %s %s\n",
			musb_driver_name, type, aRevision, aDate);

	/* configure ep0 */
	musb_configure_ep0(musb);

	/* discover endpoint configuration */
	musb->nr_endpoints = 1;
	musb->epmask = 1;

	if (musb->dyn_fifo)
		status = ep_config_from_table(musb);
	else
		status = ep_config_from_hw(musb);

	if (status < 0)
		return status;

	/* finish init, and print endpoint config */
	for (i = 0; i < musb->nr_endpoints; i++) {
		struct musb_hw_ep	*hw_ep = musb->endpoints + i;

		hw_ep->fifo = MUSB_FIFO_OFFSET(i) + mbase;
<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_USB_MUSB_TUSB6010
=======
#if defined(CONFIG_USB_MUSB_TUSB6010) || defined (CONFIG_USB_MUSB_TUSB6010_MODULE)
>>>>>>> refs/remotes/origin/cm-10.0
=======
#if defined(CONFIG_USB_MUSB_TUSB6010) || defined (CONFIG_USB_MUSB_TUSB6010_MODULE)
>>>>>>> refs/remotes/origin/master
		hw_ep->fifo_async = musb->async + 0x400 + MUSB_FIFO_OFFSET(i);
		hw_ep->fifo_sync = musb->sync + 0x400 + MUSB_FIFO_OFFSET(i);
		hw_ep->fifo_sync_va =
			musb->sync_va + 0x400 + MUSB_FIFO_OFFSET(i);

		if (i == 0)
			hw_ep->conf = mbase - 0x400 + TUSB_EP0_CONF;
		else
			hw_ep->conf = mbase + 0x400 + (((i - 1) & 0xf) << 2);
#endif

		hw_ep->regs = MUSB_EP_OFFSET(i, 0) + mbase;
<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_USB_MUSB_HDRC_HCD
		hw_ep->target_regs = musb_read_target_reg_base(i, mbase);
		hw_ep->rx_reinit = 1;
		hw_ep->tx_reinit = 1;
#endif
=======
		hw_ep->target_regs = musb_read_target_reg_base(i, mbase);
		hw_ep->rx_reinit = 1;
		hw_ep->tx_reinit = 1;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		hw_ep->target_regs = musb_read_target_reg_base(i, mbase);
		hw_ep->rx_reinit = 1;
		hw_ep->tx_reinit = 1;
>>>>>>> refs/remotes/origin/master

		if (hw_ep->max_packet_sz_tx) {
			dev_dbg(musb->controller,
				"%s: hw_ep %d%s, %smax %d\n",
				musb_driver_name, i,
				hw_ep->is_shared_fifo ? "shared" : "tx",
				hw_ep->tx_double_buffered
					? "doublebuffer, " : "",
				hw_ep->max_packet_sz_tx);
		}
		if (hw_ep->max_packet_sz_rx && !hw_ep->is_shared_fifo) {
			dev_dbg(musb->controller,
				"%s: hw_ep %d%s, %smax %d\n",
				musb_driver_name, i,
				"rx",
				hw_ep->rx_double_buffered
					? "doublebuffer, " : "",
				hw_ep->max_packet_sz_rx);
		}
		if (!(hw_ep->max_packet_sz_tx || hw_ep->max_packet_sz_rx))
			dev_dbg(musb->controller, "hw_ep %d not configured\n", i);
	}

	return 0;
}

/*-------------------------------------------------------------------------*/

<<<<<<< HEAD
#if defined(CONFIG_SOC_OMAP2430) || defined(CONFIG_SOC_OMAP3430) || \
<<<<<<< HEAD
	defined(CONFIG_ARCH_OMAP4) || defined(CONFIG_ARCH_U8500) || \
	defined(CONFIG_ARCH_U5500)
=======
	defined(CONFIG_ARCH_OMAP4) || defined(CONFIG_ARCH_U8500)
>>>>>>> refs/remotes/origin/cm-10.0

static irqreturn_t generic_interrupt(int irq, void *__hci)
{
	unsigned long	flags;
	irqreturn_t	retval = IRQ_NONE;
	struct musb	*musb = __hci;

	spin_lock_irqsave(&musb->lock, flags);

	musb->int_usb = musb_readb(musb->mregs, MUSB_INTRUSB);
	musb->int_tx = musb_readw(musb->mregs, MUSB_INTRTX);
	musb->int_rx = musb_readw(musb->mregs, MUSB_INTRRX);

	if (musb->int_usb || musb->int_tx || musb->int_rx)
		retval = musb_interrupt(musb);

	spin_unlock_irqrestore(&musb->lock, flags);

	return retval;
}

#else
#define generic_interrupt	NULL
#endif

=======
>>>>>>> refs/remotes/origin/master
/*
 * handle all the irqs defined by the HDRC core. for now we expect:  other
 * irq sources (phy, dma, etc) will be handled first, musb->int_* values
 * will be assigned, and the irq will already have been acked.
 *
 * called in irq context with spinlock held, irqs blocked
 */
irqreturn_t musb_interrupt(struct musb *musb)
{
	irqreturn_t	retval = IRQ_NONE;
<<<<<<< HEAD
	u8		devctl, power;
=======
	u8		devctl;
>>>>>>> refs/remotes/origin/master
	int		ep_num;
	u32		reg;

	devctl = musb_readb(musb->mregs, MUSB_DEVCTL);
<<<<<<< HEAD
	power = musb_readb(musb->mregs, MUSB_POWER);
=======
>>>>>>> refs/remotes/origin/master

	dev_dbg(musb->controller, "** IRQ %s usb%04x tx%04x rx%04x\n",
		(devctl & MUSB_DEVCTL_HM) ? "host" : "peripheral",
		musb->int_usb, musb->int_tx, musb->int_rx);

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_USB_GADGET_MUSB_HDRC
	if (is_otg_enabled(musb) || is_peripheral_enabled(musb))
		if (!musb->gadget_driver) {
			dev_dbg(musb->controller, "No gadget driver loaded\n");
			return IRQ_HANDLED;
		}
#endif

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	/* the core can interrupt us for multiple reasons; docs have
	 * a generic interrupt flowchart to follow
	 */
	if (musb->int_usb)
		retval |= musb_stage0_irq(musb, musb->int_usb,
<<<<<<< HEAD
				devctl, power);
=======
				devctl);
>>>>>>> refs/remotes/origin/master

	/* "stage 1" is handling endpoint irqs */

	/* handle endpoint 0 first */
	if (musb->int_tx & 1) {
		if (devctl & MUSB_DEVCTL_HM)
			retval |= musb_h_ep0_irq(musb);
		else
			retval |= musb_g_ep0_irq(musb);
	}

	/* RX on endpoints 1-15 */
	reg = musb->int_rx >> 1;
	ep_num = 1;
	while (reg) {
		if (reg & 1) {
			/* musb_ep_select(musb->mregs, ep_num); */
			/* REVISIT just retval = ep->rx_irq(...) */
			retval = IRQ_HANDLED;
<<<<<<< HEAD
			if (devctl & MUSB_DEVCTL_HM) {
				if (is_host_capable())
					musb_host_rx(musb, ep_num);
			} else {
				if (is_peripheral_capable())
					musb_g_rx(musb, ep_num);
			}
=======
			if (devctl & MUSB_DEVCTL_HM)
				musb_host_rx(musb, ep_num);
			else
				musb_g_rx(musb, ep_num);
>>>>>>> refs/remotes/origin/master
		}

		reg >>= 1;
		ep_num++;
	}

	/* TX on endpoints 1-15 */
	reg = musb->int_tx >> 1;
	ep_num = 1;
	while (reg) {
		if (reg & 1) {
			/* musb_ep_select(musb->mregs, ep_num); */
			/* REVISIT just retval |= ep->tx_irq(...) */
			retval = IRQ_HANDLED;
<<<<<<< HEAD
			if (devctl & MUSB_DEVCTL_HM) {
				if (is_host_capable())
					musb_host_tx(musb, ep_num);
			} else {
				if (is_peripheral_capable())
					musb_g_tx(musb, ep_num);
			}
=======
			if (devctl & MUSB_DEVCTL_HM)
				musb_host_tx(musb, ep_num);
			else
				musb_g_tx(musb, ep_num);
>>>>>>> refs/remotes/origin/master
		}
		reg >>= 1;
		ep_num++;
	}

	return retval;
}
EXPORT_SYMBOL_GPL(musb_interrupt);

#ifndef CONFIG_MUSB_PIO_ONLY
<<<<<<< HEAD
<<<<<<< HEAD
static int __initdata use_dma = 1;
=======
static bool __devinitdata use_dma = 1;
>>>>>>> refs/remotes/origin/cm-10.0
=======
static bool use_dma = 1;
>>>>>>> refs/remotes/origin/master

/* "modprobe ... use_dma=0" etc */
module_param(use_dma, bool, 0);
MODULE_PARM_DESC(use_dma, "enable/disable use of DMA");

void musb_dma_completion(struct musb *musb, u8 epnum, u8 transmit)
{
	u8	devctl = musb_readb(musb->mregs, MUSB_DEVCTL);

	/* called with controller lock already held */

	if (!epnum) {
#ifndef CONFIG_USB_TUSB_OMAP_DMA
		if (!is_cppi_enabled()) {
			/* endpoint 0 */
			if (devctl & MUSB_DEVCTL_HM)
				musb_h_ep0_irq(musb);
			else
				musb_g_ep0_irq(musb);
		}
#endif
	} else {
		/* endpoints 1..15 */
		if (transmit) {
<<<<<<< HEAD
			if (devctl & MUSB_DEVCTL_HM) {
				if (is_host_capable())
					musb_host_tx(musb, epnum);
			} else {
				if (is_peripheral_capable())
					musb_g_tx(musb, epnum);
			}
		} else {
			/* receive */
			if (devctl & MUSB_DEVCTL_HM) {
				if (is_host_capable())
					musb_host_rx(musb, epnum);
			} else {
				if (is_peripheral_capable())
					musb_g_rx(musb, epnum);
			}
		}
	}
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(musb_dma_completion);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			if (devctl & MUSB_DEVCTL_HM)
				musb_host_tx(musb, epnum);
			else
				musb_g_tx(musb, epnum);
		} else {
			/* receive */
			if (devctl & MUSB_DEVCTL_HM)
				musb_host_rx(musb, epnum);
			else
				musb_g_rx(musb, epnum);
		}
	}
}
EXPORT_SYMBOL_GPL(musb_dma_completion);
>>>>>>> refs/remotes/origin/master

#else
#define use_dma			0
#endif

/*-------------------------------------------------------------------------*/

<<<<<<< HEAD
#ifdef CONFIG_SYSFS

=======
>>>>>>> refs/remotes/origin/master
static ssize_t
musb_mode_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct musb *musb = dev_to_musb(dev);
	unsigned long flags;
	int ret = -EINVAL;

	spin_lock_irqsave(&musb->lock, flags);
<<<<<<< HEAD
	ret = sprintf(buf, "%s\n", otg_state_string(musb->xceiv->state));
=======
	ret = sprintf(buf, "%s\n", usb_otg_state_string(musb->xceiv->state));
>>>>>>> refs/remotes/origin/master
	spin_unlock_irqrestore(&musb->lock, flags);

	return ret;
}

static ssize_t
musb_mode_store(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t n)
{
	struct musb	*musb = dev_to_musb(dev);
	unsigned long	flags;
	int		status;

	spin_lock_irqsave(&musb->lock, flags);
	if (sysfs_streq(buf, "host"))
		status = musb_platform_set_mode(musb, MUSB_HOST);
	else if (sysfs_streq(buf, "peripheral"))
		status = musb_platform_set_mode(musb, MUSB_PERIPHERAL);
	else if (sysfs_streq(buf, "otg"))
		status = musb_platform_set_mode(musb, MUSB_OTG);
	else
		status = -EINVAL;
	spin_unlock_irqrestore(&musb->lock, flags);

	return (status == 0) ? n : status;
}
static DEVICE_ATTR(mode, 0644, musb_mode_show, musb_mode_store);

static ssize_t
musb_vbus_store(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t n)
{
	struct musb	*musb = dev_to_musb(dev);
	unsigned long	flags;
	unsigned long	val;

	if (sscanf(buf, "%lu", &val) < 1) {
		dev_err(dev, "Invalid VBUS timeout ms value\n");
		return -EINVAL;
	}

	spin_lock_irqsave(&musb->lock, flags);
	/* force T(a_wait_bcon) to be zero/unlimited *OR* valid */
	musb->a_wait_bcon = val ? max_t(int, val, OTG_TIME_A_WAIT_BCON) : 0 ;
	if (musb->xceiv->state == OTG_STATE_A_WAIT_BCON)
		musb->is_active = 0;
	musb_platform_try_idle(musb, jiffies + msecs_to_jiffies(val));
	spin_unlock_irqrestore(&musb->lock, flags);

	return n;
}

static ssize_t
musb_vbus_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct musb	*musb = dev_to_musb(dev);
	unsigned long	flags;
	unsigned long	val;
	int		vbus;

	spin_lock_irqsave(&musb->lock, flags);
	val = musb->a_wait_bcon;
	/* FIXME get_vbus_status() is normally #defined as false...
	 * and is effectively TUSB-specific.
	 */
	vbus = musb_platform_get_vbus_status(musb);
	spin_unlock_irqrestore(&musb->lock, flags);

	return sprintf(buf, "Vbus %s, timeout %lu msec\n",
			vbus ? "on" : "off", val);
}
static DEVICE_ATTR(vbus, 0644, musb_vbus_show, musb_vbus_store);

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_USB_GADGET_MUSB_HDRC

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/* Gadget drivers can't know that a host is connected so they might want
 * to start SRP, but users can.  This allows userspace to trigger SRP.
 */
static ssize_t
musb_srp_store(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t n)
{
	struct musb	*musb = dev_to_musb(dev);
	unsigned short	srp;

	if (sscanf(buf, "%hu", &srp) != 1
			|| (srp != 1)) {
		dev_err(dev, "SRP: Value must be 1\n");
		return -EINVAL;
	}

	if (srp == 1)
		musb_g_wakeup(musb);

	return n;
}
static DEVICE_ATTR(srp, 0644, NULL, musb_srp_store);

<<<<<<< HEAD
<<<<<<< HEAD
#endif /* CONFIG_USB_GADGET_MUSB_HDRC */

static struct attribute *musb_attributes[] = {
	&dev_attr_mode.attr,
	&dev_attr_vbus.attr,
#ifdef CONFIG_USB_GADGET_MUSB_HDRC
	&dev_attr_srp.attr,
#endif
=======
static struct attribute *musb_attributes[] = {
	&dev_attr_mode.attr,
	&dev_attr_vbus.attr,
	&dev_attr_srp.attr,
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct attribute *musb_attributes[] = {
	&dev_attr_mode.attr,
	&dev_attr_vbus.attr,
	&dev_attr_srp.attr,
>>>>>>> refs/remotes/origin/master
	NULL
};

static const struct attribute_group musb_attr_group = {
	.attrs = musb_attributes,
};

<<<<<<< HEAD
#endif	/* sysfs */

=======
>>>>>>> refs/remotes/origin/master
/* Only used to provide driver mode change events */
static void musb_irq_work(struct work_struct *data)
{
	struct musb *musb = container_of(data, struct musb, irq_work);
<<<<<<< HEAD
	static int old_state;

	if (musb->xceiv->state != old_state) {
		old_state = musb->xceiv->state;
=======

	if (musb->xceiv->state != musb->xceiv_old_state) {
		musb->xceiv_old_state = musb->xceiv->state;
>>>>>>> refs/remotes/origin/master
		sysfs_notify(&musb->controller->kobj, NULL, "mode");
	}
}

/* --------------------------------------------------------------------------
 * Init support
 */

<<<<<<< HEAD
<<<<<<< HEAD
static struct musb *__init
=======
static struct musb *__devinit
>>>>>>> refs/remotes/origin/cm-10.0
allocate_instance(struct device *dev,
=======
static struct musb *allocate_instance(struct device *dev,
>>>>>>> refs/remotes/origin/master
		struct musb_hdrc_config *config, void __iomem *mbase)
{
	struct musb		*musb;
	struct musb_hw_ep	*ep;
	int			epnum;
<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_USB_MUSB_HDRC_HCD
=======
>>>>>>> refs/remotes/origin/cm-10.0
	struct usb_hcd	*hcd;

	hcd = usb_create_hcd(&musb_hc_driver, dev, dev_name(dev));
	if (!hcd)
		return NULL;
	/* usbcore sets dev->driver_data to hcd, and sometimes uses that... */

	musb = hcd_to_musb(hcd);
=======
	int			ret;

	musb = devm_kzalloc(dev, sizeof(*musb), GFP_KERNEL);
	if (!musb)
		return NULL;

>>>>>>> refs/remotes/origin/master
	INIT_LIST_HEAD(&musb->control);
	INIT_LIST_HEAD(&musb->in_bulk);
	INIT_LIST_HEAD(&musb->out_bulk);

<<<<<<< HEAD
	hcd->uses_new_polling = 1;
	hcd->has_tt = 1;

	musb->vbuserr_retry = VBUSERR_RETRY_COUNT;
	musb->a_wait_bcon = OTG_TIME_A_WAIT_BCON;
<<<<<<< HEAD
#else
	musb = kzalloc(sizeof *musb, GFP_KERNEL);
	if (!musb)
		return NULL;

#endif
=======
>>>>>>> refs/remotes/origin/cm-10.0
	dev_set_drvdata(dev, musb);
=======
	musb->vbuserr_retry = VBUSERR_RETRY_COUNT;
	musb->a_wait_bcon = OTG_TIME_A_WAIT_BCON;
>>>>>>> refs/remotes/origin/master
	musb->mregs = mbase;
	musb->ctrl_base = mbase;
	musb->nIrq = -ENODEV;
	musb->config = config;
	BUG_ON(musb->config->num_eps > MUSB_C_NUM_EPS);
	for (epnum = 0, ep = musb->endpoints;
			epnum < musb->config->num_eps;
			epnum++, ep++) {
		ep->musb = musb;
		ep->epnum = epnum;
	}

	musb->controller = dev;

<<<<<<< HEAD
	return musb;
=======
	ret = musb_host_alloc(musb);
	if (ret < 0)
		goto err_free;

	dev_set_drvdata(dev, musb);

	return musb;

err_free:
	return NULL;
>>>>>>> refs/remotes/origin/master
}

static void musb_free(struct musb *musb)
{
	/* this has multiple entry modes. it handles fault cleanup after
	 * probe(), where things may be partially set up, as well as rmmod
	 * cleanup after everything's been de-activated.
	 */

#ifdef CONFIG_SYSFS
	sysfs_remove_group(&musb->controller->kobj, &musb_attr_group);
#endif

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_USB_GADGET_MUSB_HDRC
	musb_gadget_cleanup(musb);
#endif

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (musb->nIrq >= 0) {
		if (musb->irq_wake)
			disable_irq_wake(musb->nIrq);
		free_irq(musb->nIrq, musb);
	}
<<<<<<< HEAD
	if (is_dma_capable() && musb->dma_controller) {
		struct dma_controller	*c = musb->dma_controller;

		(void) c->stop(c);
		dma_controller_destroy(c);
	}

<<<<<<< HEAD
#ifdef CONFIG_USB_MUSB_HDRC_HCD
	usb_put_hcd(musb_to_hcd(musb));
#else
	kfree(musb);
#endif
=======
	kfree(musb);
>>>>>>> refs/remotes/origin/cm-10.0
=======

	musb_host_free(musb);
}

static void musb_deassert_reset(struct work_struct *work)
{
	struct musb *musb;
	unsigned long flags;

	musb = container_of(work, struct musb, deassert_reset_work.work);

	spin_lock_irqsave(&musb->lock, flags);

	if (musb->port1_status & USB_PORT_STAT_RESET)
		musb_port_reset(musb, false);

	spin_unlock_irqrestore(&musb->lock, flags);
>>>>>>> refs/remotes/origin/master
}

/*
 * Perform generic per-controller initialization.
 *
<<<<<<< HEAD
 * @pDevice: the controller (already clocked, etc)
 * @nIrq: irq
 * @mregs: virtual address of controller registers,
 *	not yet corrected for platform-specific offsets
 */
<<<<<<< HEAD
static int __init
=======
static int __devinit
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * @dev: the controller (already clocked, etc)
 * @nIrq: IRQ number
 * @ctrl: virtual address of controller registers,
 *	not yet corrected for platform-specific offsets
 */
static int
>>>>>>> refs/remotes/origin/master
musb_init_controller(struct device *dev, int nIrq, void __iomem *ctrl)
{
	int			status;
	struct musb		*musb;
<<<<<<< HEAD
	struct musb_hdrc_platform_data *plat = dev->platform_data;
=======
	struct musb_hdrc_platform_data *plat = dev_get_platdata(dev);
>>>>>>> refs/remotes/origin/master

	/* The driver might handle more features than the board; OK.
	 * Fail when the board needs a feature that's not enabled.
	 */
	if (!plat) {
		dev_dbg(dev, "no platform_data?\n");
		status = -ENODEV;
		goto fail0;
	}

	/* allocate */
	musb = allocate_instance(dev, plat->config, ctrl);
	if (!musb) {
		status = -ENOMEM;
		goto fail0;
	}

	pm_runtime_use_autosuspend(musb->controller);
	pm_runtime_set_autosuspend_delay(musb->controller, 200);
	pm_runtime_enable(musb->controller);

	spin_lock_init(&musb->lock);
<<<<<<< HEAD
	musb->board_mode = plat->mode;
	musb->board_set_power = plat->set_power;
	musb->min_power = plat->min_power;
	musb->ops = plat->platform_ops;

	/* The musb_platform_init() call:
	 *   - adjusts musb->mregs and musb->isr if needed,
	 *   - may initialize an integrated tranceiver
	 *   - initializes musb->xceiv, usually by otg_get_transceiver()
	 *   - stops powering VBUS
	 *
<<<<<<< HEAD
	 * There are various transciever configurations.  Blackfin,
=======
	 * There are various transceiver configurations.  Blackfin,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	musb->board_set_power = plat->set_power;
	musb->min_power = plat->min_power;
	musb->ops = plat->platform_ops;
	musb->port_mode = plat->mode;

	/* The musb_platform_init() call:
	 *   - adjusts musb->mregs
	 *   - sets the musb->isr
	 *   - may initialize an integrated transceiver
	 *   - initializes musb->xceiv, usually by otg_get_phy()
	 *   - stops powering VBUS
	 *
	 * There are various transceiver configurations.  Blackfin,
>>>>>>> refs/remotes/origin/master
	 * DaVinci, TUSB60x0, and others integrate them.  OMAP3 uses
	 * external/discrete ones in various flavors (twl4030 family,
	 * isp1504, non-OTG, etc) mostly hooking up through ULPI.
	 */
<<<<<<< HEAD
	musb->isr = generic_interrupt;
=======
>>>>>>> refs/remotes/origin/master
	status = musb_platform_init(musb);
	if (status < 0)
		goto fail1;

	if (!musb->isr) {
		status = -ENODEV;
<<<<<<< HEAD
<<<<<<< HEAD
		goto fail3;
	}

	if (!musb->xceiv->io_ops) {
=======
=======
>>>>>>> refs/remotes/origin/master
		goto fail2;
	}

	if (!musb->xceiv->io_ops) {
		musb->xceiv->io_dev = musb->controller;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		musb->xceiv->io_priv = musb->mregs;
		musb->xceiv->io_ops = &musb_ulpi_access;
	}

<<<<<<< HEAD
<<<<<<< HEAD
=======
	pm_runtime_get_sync(musb->controller);

>>>>>>> refs/remotes/origin/cm-10.0
#ifndef CONFIG_MUSB_PIO_ONLY
	if (use_dma && dev->dma_mask) {
		struct dma_controller	*c;

		c = dma_controller_create(musb, musb->mregs);
		musb->dma_controller = c;
		if (c)
			(void) c->start(c);
	}
#endif
	/* ideally this would be abstracted in platform setup */
	if (!is_dma_capable() || !musb->dma_controller)
		dev->dma_mask = NULL;
=======
	pm_runtime_get_sync(musb->controller);

	if (use_dma && dev->dma_mask) {
		musb->dma_controller = dma_controller_create(musb, musb->mregs);
		if (IS_ERR(musb->dma_controller)) {
			status = PTR_ERR(musb->dma_controller);
			goto fail2_5;
		}
	}
>>>>>>> refs/remotes/origin/master

	/* be sure interrupts are disabled before connecting ISR */
	musb_platform_disable(musb);
	musb_generic_disable(musb);

<<<<<<< HEAD
=======
	/* Init IRQ workqueue before request_irq */
	INIT_WORK(&musb->irq_work, musb_irq_work);
	INIT_DELAYED_WORK(&musb->deassert_reset_work, musb_deassert_reset);
	INIT_DELAYED_WORK(&musb->finish_resume_work, musb_host_finish_resume);

>>>>>>> refs/remotes/origin/master
	/* setup musb parts of the core (especially endpoints) */
	status = musb_core_init(plat->config->multipoint
			? MUSB_CONTROLLER_MHDRC
			: MUSB_CONTROLLER_HDRC, musb);
	if (status < 0)
		goto fail3;

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_USB_MUSB_OTG
	setup_timer(&musb->otg_timer, musb_otg_timer_func, (unsigned long) musb);
#endif
=======
	setup_timer(&musb->otg_timer, musb_otg_timer_func, (unsigned long) musb);
>>>>>>> refs/remotes/origin/cm-10.0

	/* Init IRQ workqueue before request_irq */
	INIT_WORK(&musb->irq_work, musb_irq_work);
=======
	setup_timer(&musb->otg_timer, musb_otg_timer_func, (unsigned long) musb);
>>>>>>> refs/remotes/origin/master

	/* attach to the IRQ */
	if (request_irq(nIrq, musb->isr, 0, dev_name(dev), musb)) {
		dev_err(dev, "request_irq %d failed!\n", nIrq);
		status = -ENODEV;
		goto fail3;
	}
	musb->nIrq = nIrq;
<<<<<<< HEAD
/* FIXME this handles wakeup irqs wrong */
=======
	/* FIXME this handles wakeup irqs wrong */
>>>>>>> refs/remotes/origin/master
	if (enable_irq_wake(nIrq) == 0) {
		musb->irq_wake = 1;
		device_init_wakeup(dev, 1);
	} else {
		musb->irq_wake = 0;
	}

<<<<<<< HEAD
	/* host side needs more setup */
	if (is_host_enabled(musb)) {
		struct usb_hcd	*hcd = musb_to_hcd(musb);

<<<<<<< HEAD
		otg_set_host(musb->xceiv, &hcd->self);

		if (is_otg_enabled(musb))
			hcd->self.otg_port = 1;
		musb->xceiv->host = &hcd->self;
=======
		otg_set_host(musb->xceiv->otg, &hcd->self);

		if (is_otg_enabled(musb))
			hcd->self.otg_port = 1;
		musb->xceiv->otg->host = &hcd->self;
>>>>>>> refs/remotes/origin/cm-10.0
		hcd->power_budget = 2 * (plat->power ? : 250);

		/* program PHY to use external vBus if required */
		if (plat->extvbus) {
			u8 busctl = musb_read_ulpi_buscontrol(musb->mregs);
			busctl |= MUSB_ULPI_USE_EXTVBUS;
			musb_write_ulpi_buscontrol(musb->mregs, busctl);
		}
	}

	/* For the host-only role, we can activate right away.
	 * (We expect the ID pin to be forcibly grounded!!)
	 * Otherwise, wait till the gadget driver hooks up.
	 */
	if (!is_otg_enabled(musb) && is_host_enabled(musb)) {
		struct usb_hcd	*hcd = musb_to_hcd(musb);

		MUSB_HST_MODE(musb);
<<<<<<< HEAD
		musb->xceiv->default_a = 1;
		musb->xceiv->state = OTG_STATE_A_IDLE;

		status = usb_add_hcd(musb_to_hcd(musb), -1, 0);
=======
		musb->xceiv->otg->default_a = 1;
		musb->xceiv->state = OTG_STATE_A_IDLE;

		status = usb_add_hcd(musb_to_hcd(musb), 0, 0);
>>>>>>> refs/remotes/origin/cm-10.0

		hcd->self.uses_pio_for_control = 1;
		dev_dbg(musb->controller, "%s mode, status %d, devctl %02x %c\n",
			"HOST", status,
			musb_readb(musb->mregs, MUSB_DEVCTL),
			(musb_readb(musb->mregs, MUSB_DEVCTL)
					& MUSB_DEVCTL_BDEVICE
				? 'B' : 'A'));

	} else /* peripheral is enabled */ {
		MUSB_DEV_MODE(musb);
<<<<<<< HEAD
		musb->xceiv->default_a = 0;
=======
		musb->xceiv->otg->default_a = 0;
>>>>>>> refs/remotes/origin/cm-10.0
		musb->xceiv->state = OTG_STATE_B_IDLE;

		status = musb_gadget_setup(musb);

		dev_dbg(musb->controller, "%s mode, status %d, dev%02x\n",
			is_otg_enabled(musb) ? "OTG" : "PERIPHERAL",
			status,
			musb_readb(musb->mregs, MUSB_DEVCTL));

	}
=======
	/* program PHY to use external vBus if required */
	if (plat->extvbus) {
		u8 busctl = musb_read_ulpi_buscontrol(musb->mregs);
		busctl |= MUSB_ULPI_USE_EXTVBUS;
		musb_write_ulpi_buscontrol(musb->mregs, busctl);
	}

	if (musb->xceiv->otg->default_a) {
		MUSB_HST_MODE(musb);
		musb->xceiv->state = OTG_STATE_A_IDLE;
	} else {
		MUSB_DEV_MODE(musb);
		musb->xceiv->state = OTG_STATE_B_IDLE;
	}

	switch (musb->port_mode) {
	case MUSB_PORT_MODE_HOST:
		status = musb_host_setup(musb, plat->power);
		if (status < 0)
			goto fail3;
		status = musb_platform_set_mode(musb, MUSB_HOST);
		break;
	case MUSB_PORT_MODE_GADGET:
		status = musb_gadget_setup(musb);
		if (status < 0)
			goto fail3;
		status = musb_platform_set_mode(musb, MUSB_PERIPHERAL);
		break;
	case MUSB_PORT_MODE_DUAL_ROLE:
		status = musb_host_setup(musb, plat->power);
		if (status < 0)
			goto fail3;
		status = musb_gadget_setup(musb);
		if (status) {
			musb_host_cleanup(musb);
			goto fail3;
		}
		status = musb_platform_set_mode(musb, MUSB_OTG);
		break;
	default:
		dev_err(dev, "unsupported port mode %d\n", musb->port_mode);
		break;
	}

>>>>>>> refs/remotes/origin/master
	if (status < 0)
		goto fail3;

	status = musb_init_debugfs(musb);
	if (status < 0)
		goto fail4;

<<<<<<< HEAD
#ifdef CONFIG_SYSFS
	status = sysfs_create_group(&musb->controller->kobj, &musb_attr_group);
	if (status)
		goto fail5;
#endif

<<<<<<< HEAD
=======
	pm_runtime_put(musb->controller);

>>>>>>> refs/remotes/origin/cm-10.0
	dev_info(dev, "USB %s mode controller at %p using %s, IRQ %d\n",
			({char *s;
			 switch (musb->board_mode) {
			 case MUSB_HOST:		s = "Host"; break;
			 case MUSB_PERIPHERAL:	s = "Peripheral"; break;
			 default:		s = "OTG"; break;
			 }; s; }),
			ctrl,
			(is_dma_capable() && musb->dma_controller)
			? "DMA" : "PIO",
			musb->nIrq);

=======
	status = sysfs_create_group(&musb->controller->kobj, &musb_attr_group);
	if (status)
		goto fail5;

	pm_runtime_put(musb->controller);

>>>>>>> refs/remotes/origin/master
	return 0;

fail5:
	musb_exit_debugfs(musb);

fail4:
<<<<<<< HEAD
	if (!is_otg_enabled(musb) && is_host_enabled(musb))
		usb_remove_hcd(musb_to_hcd(musb));
	else
		musb_gadget_cleanup(musb);

fail3:
<<<<<<< HEAD
=======
	pm_runtime_put_sync(musb->controller);

fail2:
>>>>>>> refs/remotes/origin/cm-10.0
=======
	musb_gadget_cleanup(musb);
	musb_host_cleanup(musb);

fail3:
	cancel_work_sync(&musb->irq_work);
	cancel_delayed_work_sync(&musb->finish_resume_work);
	cancel_delayed_work_sync(&musb->deassert_reset_work);
	if (musb->dma_controller)
		dma_controller_destroy(musb->dma_controller);
fail2_5:
	pm_runtime_put_sync(musb->controller);

fail2:
>>>>>>> refs/remotes/origin/master
	if (musb->irq_wake)
		device_init_wakeup(dev, 0);
	musb_platform_exit(musb);

fail1:
<<<<<<< HEAD
=======
	pm_runtime_disable(musb->controller);
>>>>>>> refs/remotes/origin/master
	dev_err(musb->controller,
		"musb_init_controller failed with status %d\n", status);

	musb_free(musb);

fail0:

	return status;

}

/*-------------------------------------------------------------------------*/

/* all implementations (PCI bridge to FPGA, VLYNQ, etc) should just
 * bridge to a platform device; this driver then suffices.
 */
<<<<<<< HEAD

#ifndef CONFIG_MUSB_PIO_ONLY
static u64	*orig_dma_mask;
#endif

<<<<<<< HEAD
static int __init musb_probe(struct platform_device *pdev)
=======
static int __devinit musb_probe(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct device	*dev = &pdev->dev;
	int		irq = platform_get_irq_byname(pdev, "mc");
	int		status;
=======
static int musb_probe(struct platform_device *pdev)
{
	struct device	*dev = &pdev->dev;
	int		irq = platform_get_irq_byname(pdev, "mc");
>>>>>>> refs/remotes/origin/master
	struct resource	*iomem;
	void __iomem	*base;

	iomem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!iomem || irq <= 0)
		return -ENODEV;

<<<<<<< HEAD
	base = ioremap(iomem->start, resource_size(iomem));
	if (!base) {
		dev_err(dev, "ioremap failed\n");
		return -ENOMEM;
	}

#ifndef CONFIG_MUSB_PIO_ONLY
	/* clobbered by use_dma=n */
	orig_dma_mask = dev->dma_mask;
#endif
	status = musb_init_controller(dev, irq, base);
	if (status < 0)
		iounmap(base);

	return status;
}

<<<<<<< HEAD
static int __exit musb_remove(struct platform_device *pdev)
=======
static int __devexit musb_remove(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct musb	*musb = dev_to_musb(&pdev->dev);
	void __iomem	*ctrl_base = musb->ctrl_base;
=======
	base = devm_ioremap_resource(dev, iomem);
	if (IS_ERR(base))
		return PTR_ERR(base);

	return musb_init_controller(dev, irq, base);
}

static int musb_remove(struct platform_device *pdev)
{
	struct device	*dev = &pdev->dev;
	struct musb	*musb = dev_to_musb(dev);
>>>>>>> refs/remotes/origin/master

	/* this gets called on rmmod.
	 *  - Host mode: host may still be active
	 *  - Peripheral mode: peripheral is deactivated (or never-activated)
	 *  - OTG mode: both roles are deactivated (or never-activated)
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	pm_runtime_get_sync(musb->controller);
	musb_exit_debugfs(musb);
	musb_shutdown(pdev);

	pm_runtime_put(musb->controller);
=======
	musb_exit_debugfs(musb);
	musb_shutdown(pdev);

>>>>>>> refs/remotes/origin/cm-10.0
	musb_free(musb);
	iounmap(ctrl_base);
	device_init_wakeup(&pdev->dev, 0);
#ifndef CONFIG_MUSB_PIO_ONLY
	pdev->dev.dma_mask = orig_dma_mask;
#endif
=======
	musb_exit_debugfs(musb);
	musb_shutdown(pdev);

	if (musb->dma_controller)
		dma_controller_destroy(musb->dma_controller);

	cancel_work_sync(&musb->irq_work);
	cancel_delayed_work_sync(&musb->finish_resume_work);
	cancel_delayed_work_sync(&musb->deassert_reset_work);
	musb_free(musb);
	device_init_wakeup(dev, 0);
>>>>>>> refs/remotes/origin/master
	return 0;
}

#ifdef	CONFIG_PM

static void musb_save_context(struct musb *musb)
{
	int i;
	void __iomem *musb_base = musb->mregs;
	void __iomem *epio;

<<<<<<< HEAD
	if (is_host_enabled(musb)) {
		musb->context.frame = musb_readw(musb_base, MUSB_FRAME);
		musb->context.testmode = musb_readb(musb_base, MUSB_TESTMODE);
		musb->context.busctl = musb_read_ulpi_buscontrol(musb->mregs);
	}
	musb->context.power = musb_readb(musb_base, MUSB_POWER);
	musb->context.intrtxe = musb_readw(musb_base, MUSB_INTRTXE);
	musb->context.intrrxe = musb_readw(musb_base, MUSB_INTRRXE);
=======
	musb->context.frame = musb_readw(musb_base, MUSB_FRAME);
	musb->context.testmode = musb_readb(musb_base, MUSB_TESTMODE);
	musb->context.busctl = musb_read_ulpi_buscontrol(musb->mregs);
	musb->context.power = musb_readb(musb_base, MUSB_POWER);
>>>>>>> refs/remotes/origin/master
	musb->context.intrusbe = musb_readb(musb_base, MUSB_INTRUSBE);
	musb->context.index = musb_readb(musb_base, MUSB_INDEX);
	musb->context.devctl = musb_readb(musb_base, MUSB_DEVCTL);

	for (i = 0; i < musb->config->num_eps; ++i) {
<<<<<<< HEAD
<<<<<<< HEAD
		epio = musb->endpoints[i].regs;
=======
=======
>>>>>>> refs/remotes/origin/master
		struct musb_hw_ep	*hw_ep;

		hw_ep = &musb->endpoints[i];
		if (!hw_ep)
			continue;

		epio = hw_ep->regs;
		if (!epio)
			continue;

		musb_writeb(musb_base, MUSB_INDEX, i);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		musb->context.index_regs[i].txmaxp =
			musb_readw(epio, MUSB_TXMAXP);
		musb->context.index_regs[i].txcsr =
			musb_readw(epio, MUSB_TXCSR);
		musb->context.index_regs[i].rxmaxp =
			musb_readw(epio, MUSB_RXMAXP);
		musb->context.index_regs[i].rxcsr =
			musb_readw(epio, MUSB_RXCSR);

		if (musb->dyn_fifo) {
			musb->context.index_regs[i].txfifoadd =
					musb_read_txfifoadd(musb_base);
			musb->context.index_regs[i].rxfifoadd =
					musb_read_rxfifoadd(musb_base);
			musb->context.index_regs[i].txfifosz =
					musb_read_txfifosz(musb_base);
			musb->context.index_regs[i].rxfifosz =
					musb_read_rxfifosz(musb_base);
		}
<<<<<<< HEAD
		if (is_host_enabled(musb)) {
			musb->context.index_regs[i].txtype =
				musb_readb(epio, MUSB_TXTYPE);
			musb->context.index_regs[i].txinterval =
				musb_readb(epio, MUSB_TXINTERVAL);
			musb->context.index_regs[i].rxtype =
				musb_readb(epio, MUSB_RXTYPE);
			musb->context.index_regs[i].rxinterval =
				musb_readb(epio, MUSB_RXINTERVAL);

			musb->context.index_regs[i].txfunaddr =
				musb_read_txfunaddr(musb_base, i);
			musb->context.index_regs[i].txhubaddr =
				musb_read_txhubaddr(musb_base, i);
			musb->context.index_regs[i].txhubport =
				musb_read_txhubport(musb_base, i);

			musb->context.index_regs[i].rxfunaddr =
				musb_read_rxfunaddr(musb_base, i);
			musb->context.index_regs[i].rxhubaddr =
				musb_read_rxhubaddr(musb_base, i);
			musb->context.index_regs[i].rxhubport =
				musb_read_rxhubport(musb_base, i);
		}
=======

		musb->context.index_regs[i].txtype =
			musb_readb(epio, MUSB_TXTYPE);
		musb->context.index_regs[i].txinterval =
			musb_readb(epio, MUSB_TXINTERVAL);
		musb->context.index_regs[i].rxtype =
			musb_readb(epio, MUSB_RXTYPE);
		musb->context.index_regs[i].rxinterval =
			musb_readb(epio, MUSB_RXINTERVAL);

		musb->context.index_regs[i].txfunaddr =
			musb_read_txfunaddr(musb_base, i);
		musb->context.index_regs[i].txhubaddr =
			musb_read_txhubaddr(musb_base, i);
		musb->context.index_regs[i].txhubport =
			musb_read_txhubport(musb_base, i);

		musb->context.index_regs[i].rxfunaddr =
			musb_read_rxfunaddr(musb_base, i);
		musb->context.index_regs[i].rxhubaddr =
			musb_read_rxhubaddr(musb_base, i);
		musb->context.index_regs[i].rxhubport =
			musb_read_rxhubport(musb_base, i);
>>>>>>> refs/remotes/origin/master
	}
}

static void musb_restore_context(struct musb *musb)
{
	int i;
	void __iomem *musb_base = musb->mregs;
	void __iomem *ep_target_regs;
	void __iomem *epio;

<<<<<<< HEAD
	if (is_host_enabled(musb)) {
		musb_writew(musb_base, MUSB_FRAME, musb->context.frame);
		musb_writeb(musb_base, MUSB_TESTMODE, musb->context.testmode);
		musb_write_ulpi_buscontrol(musb->mregs, musb->context.busctl);
	}
	musb_writeb(musb_base, MUSB_POWER, musb->context.power);
	musb_writew(musb_base, MUSB_INTRTXE, musb->context.intrtxe);
	musb_writew(musb_base, MUSB_INTRRXE, musb->context.intrrxe);
=======
	musb_writew(musb_base, MUSB_FRAME, musb->context.frame);
	musb_writeb(musb_base, MUSB_TESTMODE, musb->context.testmode);
	musb_write_ulpi_buscontrol(musb->mregs, musb->context.busctl);
	musb_writeb(musb_base, MUSB_POWER, musb->context.power);
	musb_writew(musb_base, MUSB_INTRTXE, musb->intrtxe);
	musb_writew(musb_base, MUSB_INTRRXE, musb->intrrxe);
>>>>>>> refs/remotes/origin/master
	musb_writeb(musb_base, MUSB_INTRUSBE, musb->context.intrusbe);
	musb_writeb(musb_base, MUSB_DEVCTL, musb->context.devctl);

	for (i = 0; i < musb->config->num_eps; ++i) {
<<<<<<< HEAD
<<<<<<< HEAD
		epio = musb->endpoints[i].regs;
=======
=======
>>>>>>> refs/remotes/origin/master
		struct musb_hw_ep	*hw_ep;

		hw_ep = &musb->endpoints[i];
		if (!hw_ep)
			continue;

		epio = hw_ep->regs;
		if (!epio)
			continue;

		musb_writeb(musb_base, MUSB_INDEX, i);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		musb_writew(epio, MUSB_TXMAXP,
			musb->context.index_regs[i].txmaxp);
		musb_writew(epio, MUSB_TXCSR,
			musb->context.index_regs[i].txcsr);
		musb_writew(epio, MUSB_RXMAXP,
			musb->context.index_regs[i].rxmaxp);
		musb_writew(epio, MUSB_RXCSR,
			musb->context.index_regs[i].rxcsr);

		if (musb->dyn_fifo) {
			musb_write_txfifosz(musb_base,
				musb->context.index_regs[i].txfifosz);
			musb_write_rxfifosz(musb_base,
				musb->context.index_regs[i].rxfifosz);
			musb_write_txfifoadd(musb_base,
				musb->context.index_regs[i].txfifoadd);
			musb_write_rxfifoadd(musb_base,
				musb->context.index_regs[i].rxfifoadd);
		}

<<<<<<< HEAD
		if (is_host_enabled(musb)) {
			musb_writeb(epio, MUSB_TXTYPE,
				musb->context.index_regs[i].txtype);
			musb_writeb(epio, MUSB_TXINTERVAL,
				musb->context.index_regs[i].txinterval);
			musb_writeb(epio, MUSB_RXTYPE,
				musb->context.index_regs[i].rxtype);
			musb_writeb(epio, MUSB_RXINTERVAL,

			musb->context.index_regs[i].rxinterval);
			musb_write_txfunaddr(musb_base, i,
				musb->context.index_regs[i].txfunaddr);
			musb_write_txhubaddr(musb_base, i,
				musb->context.index_regs[i].txhubaddr);
			musb_write_txhubport(musb_base, i,
				musb->context.index_regs[i].txhubport);

			ep_target_regs =
				musb_read_target_reg_base(i, musb_base);

			musb_write_rxfunaddr(ep_target_regs,
				musb->context.index_regs[i].rxfunaddr);
			musb_write_rxhubaddr(ep_target_regs,
				musb->context.index_regs[i].rxhubaddr);
			musb_write_rxhubport(ep_target_regs,
				musb->context.index_regs[i].rxhubport);
		}
=======
		musb_writeb(epio, MUSB_TXTYPE,
				musb->context.index_regs[i].txtype);
		musb_writeb(epio, MUSB_TXINTERVAL,
				musb->context.index_regs[i].txinterval);
		musb_writeb(epio, MUSB_RXTYPE,
				musb->context.index_regs[i].rxtype);
		musb_writeb(epio, MUSB_RXINTERVAL,

				musb->context.index_regs[i].rxinterval);
		musb_write_txfunaddr(musb_base, i,
				musb->context.index_regs[i].txfunaddr);
		musb_write_txhubaddr(musb_base, i,
				musb->context.index_regs[i].txhubaddr);
		musb_write_txhubport(musb_base, i,
				musb->context.index_regs[i].txhubport);

		ep_target_regs =
			musb_read_target_reg_base(i, musb_base);

		musb_write_rxfunaddr(ep_target_regs,
				musb->context.index_regs[i].rxfunaddr);
		musb_write_rxhubaddr(ep_target_regs,
				musb->context.index_regs[i].rxhubaddr);
		musb_write_rxhubport(ep_target_regs,
				musb->context.index_regs[i].rxhubport);
>>>>>>> refs/remotes/origin/master
	}
	musb_writeb(musb_base, MUSB_INDEX, musb->context.index);
}

static int musb_suspend(struct device *dev)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct platform_device *pdev = to_platform_device(dev);
	unsigned long	flags;
	struct musb	*musb = dev_to_musb(&pdev->dev);
=======
	struct musb	*musb = dev_to_musb(dev);
	unsigned long	flags;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct musb	*musb = dev_to_musb(dev);
	unsigned long	flags;
>>>>>>> refs/remotes/origin/master

	spin_lock_irqsave(&musb->lock, flags);

	if (is_peripheral_active(musb)) {
		/* FIXME force disconnect unless we know USB will wake
		 * the system up quickly enough to respond ...
		 */
	} else if (is_host_active(musb)) {
		/* we know all the children are suspended; sometimes
		 * they will even be wakeup-enabled.
		 */
	}

<<<<<<< HEAD
<<<<<<< HEAD
	musb_save_context(musb);

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	musb_save_context(musb);

>>>>>>> refs/remotes/origin/master
	spin_unlock_irqrestore(&musb->lock, flags);
	return 0;
}

static int musb_resume_noirq(struct device *dev)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct platform_device *pdev = to_platform_device(dev);
	struct musb	*musb = dev_to_musb(&pdev->dev);

	musb_restore_context(musb);

=======
>>>>>>> refs/remotes/origin/cm-10.0
	/* for static cmos like DaVinci, register values were preserved
	 * unless for some reason the whole soc powered down or the USB
	 * module got reset through the PSC (vs just being disabled).
	 */
=======
	struct musb	*musb = dev_to_musb(dev);

	/*
	 * For static cmos like DaVinci, register values were preserved
	 * unless for some reason the whole soc powered down or the USB
	 * module got reset through the PSC (vs just being disabled).
	 *
	 * For the DSPS glue layer though, a full register restore has to
	 * be done. As it shouldn't harm other platforms, we do it
	 * unconditionally.
	 */

	musb_restore_context(musb);

>>>>>>> refs/remotes/origin/master
	return 0;
}

static int musb_runtime_suspend(struct device *dev)
{
	struct musb	*musb = dev_to_musb(dev);

	musb_save_context(musb);

	return 0;
}

static int musb_runtime_resume(struct device *dev)
{
	struct musb	*musb = dev_to_musb(dev);
	static int	first = 1;

	/*
	 * When pm_runtime_get_sync called for the first time in driver
	 * init,  some of the structure is still not initialized which is
	 * used in restore function. But clock needs to be
	 * enabled before any register access, so
	 * pm_runtime_get_sync has to be called.
	 * Also context restore without save does not make
	 * any sense
	 */
	if (!first)
		musb_restore_context(musb);
	first = 0;

	return 0;
}

static const struct dev_pm_ops musb_dev_pm_ops = {
	.suspend	= musb_suspend,
	.resume_noirq	= musb_resume_noirq,
	.runtime_suspend = musb_runtime_suspend,
	.runtime_resume = musb_runtime_resume,
};

#define MUSB_DEV_PM_OPS (&musb_dev_pm_ops)
#else
#define	MUSB_DEV_PM_OPS	NULL
#endif

static struct platform_driver musb_driver = {
	.driver = {
		.name		= (char *)musb_driver_name,
		.bus		= &platform_bus_type,
		.owner		= THIS_MODULE,
		.pm		= MUSB_DEV_PM_OPS,
	},
<<<<<<< HEAD
<<<<<<< HEAD
	.remove		= __exit_p(musb_remove),
=======
	.probe		= musb_probe,
	.remove		= __devexit_p(musb_remove),
>>>>>>> refs/remotes/origin/cm-10.0
	.shutdown	= musb_shutdown,
};

/*-------------------------------------------------------------------------*/

static int __init musb_init(void)
{
<<<<<<< HEAD
#ifdef CONFIG_USB_MUSB_HDRC_HCD
	if (usb_disabled())
		return 0;
#endif

<<<<<<< HEAD
	pr_info("%s: version " MUSB_VERSION ", "
#ifdef CONFIG_MUSB_PIO_ONLY
		"pio"
#elif defined(CONFIG_USB_TI_CPPI_DMA)
		"cppi-dma"
#elif defined(CONFIG_USB_INVENTRA_DMA)
		"musb-dma"
#elif defined(CONFIG_USB_TUSB_OMAP_DMA)
		"tusb-omap-dma"
#elif defined(CONFIG_USB_UX500_DMA)
		"ux500-dma"
#else
		"?dma?"
#endif
		", "
#ifdef CONFIG_USB_MUSB_OTG
		"otg (peripheral+host)"
#elif defined(CONFIG_USB_GADGET_MUSB_HDRC)
		"peripheral"
#elif defined(CONFIG_USB_MUSB_HDRC_HCD)
		"host"
#endif
		,
		musb_driver_name);
	return platform_driver_probe(&musb_driver, musb_probe);
}

/* make us init after usbcore and i2c (transceivers, regulators, etc)
 * and before usb gadget and host-side drivers start to register
 */
fs_initcall(musb_init);
=======
	if (usb_disabled())
		return 0;

=======
>>>>>>> refs/remotes/origin/cm-11.0
	pr_info("%s: version " MUSB_VERSION ", ?dma?, otg (peripheral+host)\n",
		musb_driver_name);
	return platform_driver_register(&musb_driver);
}
module_init(musb_init);
>>>>>>> refs/remotes/origin/cm-10.0

static void __exit musb_cleanup(void)
{
	platform_driver_unregister(&musb_driver);
}
module_exit(musb_cleanup);
=======
	.probe		= musb_probe,
	.remove		= musb_remove,
	.shutdown	= musb_shutdown,
};

module_platform_driver(musb_driver);
>>>>>>> refs/remotes/origin/master
