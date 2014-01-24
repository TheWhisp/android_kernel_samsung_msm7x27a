/*
 * MUSB OTG driver virtual root hub support
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

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/errno.h>
<<<<<<< HEAD
#include <linux/init.h>
=======
>>>>>>> refs/remotes/origin/master
#include <linux/time.h>
#include <linux/timer.h>

#include <asm/unaligned.h>

#include "musb_core.h"

<<<<<<< HEAD

static void musb_port_suspend(struct musb *musb, bool do_suspend)
{
<<<<<<< HEAD
=======
	struct usb_otg	*otg = musb->xceiv->otg;
>>>>>>> refs/remotes/origin/cm-10.0
=======
void musb_host_finish_resume(struct work_struct *work)
{
	struct musb *musb;
	unsigned long flags;
	u8 power;

	musb = container_of(work, struct musb, finish_resume_work.work);

	spin_lock_irqsave(&musb->lock, flags);

	power = musb_readb(musb->mregs, MUSB_POWER);
	power &= ~MUSB_POWER_RESUME;
	dev_dbg(musb->controller, "root port resume stopped, power %02x\n",
		power);
	musb_writeb(musb->mregs, MUSB_POWER, power);

	/*
	 * ISSUE:  DaVinci (RTL 1.300) disconnects after
	 * resume of high speed peripherals (but not full
	 * speed ones).
	 */
	musb->is_active = 1;
	musb->port1_status &= ~(USB_PORT_STAT_SUSPEND | MUSB_PORT_STAT_RESUME);
	musb->port1_status |= USB_PORT_STAT_C_SUSPEND << 16;
	usb_hcd_poll_rh_status(musb->hcd);
	/* NOTE: it might really be A_WAIT_BCON ... */
	musb->xceiv->state = OTG_STATE_A_HOST;

	spin_unlock_irqrestore(&musb->lock, flags);
}

void musb_port_suspend(struct musb *musb, bool do_suspend)
{
	struct usb_otg	*otg = musb->xceiv->otg;
>>>>>>> refs/remotes/origin/master
	u8		power;
	void __iomem	*mbase = musb->mregs;

	if (!is_host_active(musb))
		return;

	/* NOTE:  this doesn't necessarily put PHY into low power mode,
	 * turning off its clock; that's a function of PHY integration and
	 * MUSB_POWER_ENSUSPEND.  PHY may need a clock (sigh) to detect
	 * SE0 changing to connect (J) or wakeup (K) states.
	 */
	power = musb_readb(mbase, MUSB_POWER);
	if (do_suspend) {
		int retries = 10000;

		power &= ~MUSB_POWER_RESUME;
		power |= MUSB_POWER_SUSPENDM;
		musb_writeb(mbase, MUSB_POWER, power);

		/* Needed for OPT A tests */
		power = musb_readb(mbase, MUSB_POWER);
		while (power & MUSB_POWER_SUSPENDM) {
			power = musb_readb(mbase, MUSB_POWER);
			if (retries-- < 1)
				break;
		}

		dev_dbg(musb->controller, "Root port suspended, power %02x\n", power);

		musb->port1_status |= USB_PORT_STAT_SUSPEND;
		switch (musb->xceiv->state) {
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
			if (musb->is_active)
				mod_timer(&musb->otg_timer, jiffies
					+ msecs_to_jiffies(
						OTG_TIME_A_AIDL_BDIS));
			musb_platform_try_idle(musb, 0);
			break;
<<<<<<< HEAD
<<<<<<< HEAD
#ifdef	CONFIG_USB_MUSB_OTG
		case OTG_STATE_B_HOST:
			musb->xceiv->state = OTG_STATE_B_WAIT_ACON;
			musb->is_active = is_otg_enabled(musb)
					&& musb->xceiv->host->b_hnp_enable;
			musb_platform_try_idle(musb, 0);
			break;
#endif
=======
		case OTG_STATE_B_HOST:
			musb->xceiv->state = OTG_STATE_B_WAIT_ACON;
			musb->is_active = is_otg_enabled(musb)
					&& otg->host->b_hnp_enable;
			musb_platform_try_idle(musb, 0);
			break;
>>>>>>> refs/remotes/origin/cm-10.0
		default:
			dev_dbg(musb->controller, "bogus rh suspend? %s\n",
				otg_state_string(musb->xceiv->state));
=======
		case OTG_STATE_B_HOST:
			musb->xceiv->state = OTG_STATE_B_WAIT_ACON;
			musb->is_active = otg->host->b_hnp_enable;
			musb_platform_try_idle(musb, 0);
			break;
		default:
			dev_dbg(musb->controller, "bogus rh suspend? %s\n",
				usb_otg_state_string(musb->xceiv->state));
>>>>>>> refs/remotes/origin/master
		}
	} else if (power & MUSB_POWER_SUSPENDM) {
		power &= ~MUSB_POWER_SUSPENDM;
		power |= MUSB_POWER_RESUME;
		musb_writeb(mbase, MUSB_POWER, power);

		dev_dbg(musb->controller, "Root port resuming, power %02x\n", power);

		/* later, GetPortStatus will stop RESUME signaling */
		musb->port1_status |= MUSB_PORT_STAT_RESUME;
<<<<<<< HEAD
		musb->rh_timer = jiffies + msecs_to_jiffies(20);
	}
}

static void musb_port_reset(struct musb *musb, bool do_reset)
=======
		schedule_delayed_work(&musb->finish_resume_work, 20);
	}
}

void musb_port_reset(struct musb *musb, bool do_reset)
>>>>>>> refs/remotes/origin/master
{
	u8		power;
	void __iomem	*mbase = musb->mregs;

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_USB_MUSB_OTG
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (musb->xceiv->state == OTG_STATE_B_IDLE) {
		dev_dbg(musb->controller, "HNP: Returning from HNP; no hub reset from b_idle\n");
		musb->port1_status &= ~USB_PORT_STAT_RESET;
		return;
	}
<<<<<<< HEAD
<<<<<<< HEAD
#endif
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	if (!is_host_active(musb))
		return;

	/* NOTE:  caller guarantees it will turn off the reset when
	 * the appropriate amount of time has passed
	 */
	power = musb_readb(mbase, MUSB_POWER);
	if (do_reset) {

		/*
		 * If RESUME is set, we must make sure it stays minimum 20 ms.
		 * Then we must clear RESUME and wait a bit to let musb start
		 * generating SOFs. If we don't do this, OPT HS A 6.8 tests
		 * fail with "Error! Did not receive an SOF before suspend
		 * detected".
		 */
		if (power &  MUSB_POWER_RESUME) {
			while (time_before(jiffies, musb->rh_timer))
				msleep(1);
			musb_writeb(mbase, MUSB_POWER,
				power & ~MUSB_POWER_RESUME);
			msleep(1);
		}

<<<<<<< HEAD
		musb->ignore_disconnect = true;
=======
>>>>>>> refs/remotes/origin/master
		power &= 0xf0;
		musb_writeb(mbase, MUSB_POWER,
				power | MUSB_POWER_RESET);

		musb->port1_status |= USB_PORT_STAT_RESET;
		musb->port1_status &= ~USB_PORT_STAT_ENABLE;
<<<<<<< HEAD
		musb->rh_timer = jiffies + msecs_to_jiffies(50);
=======
		schedule_delayed_work(&musb->deassert_reset_work, 50);
>>>>>>> refs/remotes/origin/master
	} else {
		dev_dbg(musb->controller, "root port reset stopped\n");
		musb_writeb(mbase, MUSB_POWER,
				power & ~MUSB_POWER_RESET);

<<<<<<< HEAD
		musb->ignore_disconnect = false;

=======
>>>>>>> refs/remotes/origin/master
		power = musb_readb(mbase, MUSB_POWER);
		if (power & MUSB_POWER_HSMODE) {
			dev_dbg(musb->controller, "high-speed device connected\n");
			musb->port1_status |= USB_PORT_STAT_HIGH_SPEED;
		}

		musb->port1_status &= ~USB_PORT_STAT_RESET;
		musb->port1_status |= USB_PORT_STAT_ENABLE
					| (USB_PORT_STAT_C_RESET << 16)
					| (USB_PORT_STAT_C_ENABLE << 16);
<<<<<<< HEAD
		usb_hcd_poll_rh_status(musb_to_hcd(musb));
=======
		usb_hcd_poll_rh_status(musb->hcd);
>>>>>>> refs/remotes/origin/master

		musb->vbuserr_retry = VBUSERR_RETRY_COUNT;
	}
}

void musb_root_disconnect(struct musb *musb)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct usb_otg	*otg = musb->xceiv->otg;

>>>>>>> refs/remotes/origin/cm-10.0
	musb->port1_status = USB_PORT_STAT_POWER
			| (USB_PORT_STAT_C_CONNECTION << 16);

	usb_hcd_poll_rh_status(musb_to_hcd(musb));
=======
	struct usb_otg	*otg = musb->xceiv->otg;

	musb->port1_status = USB_PORT_STAT_POWER
			| (USB_PORT_STAT_C_CONNECTION << 16);

	usb_hcd_poll_rh_status(musb->hcd);
>>>>>>> refs/remotes/origin/master
	musb->is_active = 0;

	switch (musb->xceiv->state) {
	case OTG_STATE_A_SUSPEND:
<<<<<<< HEAD
<<<<<<< HEAD
#ifdef	CONFIG_USB_MUSB_OTG
		if (is_otg_enabled(musb)
				&& musb->xceiv->host->b_hnp_enable) {
=======
		if (is_otg_enabled(musb)
				&& otg->host->b_hnp_enable) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (otg->host->b_hnp_enable) {
>>>>>>> refs/remotes/origin/master
			musb->xceiv->state = OTG_STATE_A_PERIPHERAL;
			musb->g.is_a_peripheral = 1;
			break;
		}
<<<<<<< HEAD
<<<<<<< HEAD
#endif
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		/* FALLTHROUGH */
	case OTG_STATE_A_HOST:
		musb->xceiv->state = OTG_STATE_A_WAIT_BCON;
		musb->is_active = 0;
		break;
	case OTG_STATE_A_WAIT_VFALL:
		musb->xceiv->state = OTG_STATE_B_IDLE;
		break;
	default:
		dev_dbg(musb->controller, "host disconnect (%s)\n",
<<<<<<< HEAD
			otg_state_string(musb->xceiv->state));
=======
			usb_otg_state_string(musb->xceiv->state));
>>>>>>> refs/remotes/origin/master
	}
}


/*---------------------------------------------------------------------*/

/* Caller may or may not hold musb->lock */
int musb_hub_status_data(struct usb_hcd *hcd, char *buf)
{
	struct musb	*musb = hcd_to_musb(hcd);
	int		retval = 0;

	/* called in_irq() via usb_hcd_poll_rh_status() */
	if (musb->port1_status & 0xffff0000) {
		*buf = 0x02;
		retval = 1;
	}
	return retval;
}

<<<<<<< HEAD
=======
static int musb_has_gadget(struct musb *musb)
{
	/*
	 * In host-only mode we start a connection right away. In OTG mode
	 * we have to wait until we loaded a gadget. We don't really need a
	 * gadget if we operate as a host but we should not start a session
	 * as a device without a gadget or else we explode.
	 */
#ifdef CONFIG_USB_MUSB_HOST
	return 1;
#else
	if (musb->port_mode == MUSB_PORT_MODE_HOST)
		return 1;
	return musb->g.dev.driver != NULL;
#endif
}

>>>>>>> refs/remotes/origin/master
int musb_hub_control(
	struct usb_hcd	*hcd,
	u16		typeReq,
	u16		wValue,
	u16		wIndex,
	char		*buf,
	u16		wLength)
{
	struct musb	*musb = hcd_to_musb(hcd);
	u32		temp;
	int		retval = 0;
	unsigned long	flags;

	spin_lock_irqsave(&musb->lock, flags);

	if (unlikely(!HCD_HW_ACCESSIBLE(hcd))) {
		spin_unlock_irqrestore(&musb->lock, flags);
		return -ESHUTDOWN;
	}

	/* hub features:  always zero, setting is a NOP
	 * port features: reported, sometimes updated when host is active
	 * no indicators
	 */
	switch (typeReq) {
	case ClearHubFeature:
	case SetHubFeature:
		switch (wValue) {
		case C_HUB_OVER_CURRENT:
		case C_HUB_LOCAL_POWER:
			break;
		default:
			goto error;
		}
		break;
	case ClearPortFeature:
		if ((wIndex & 0xff) != 1)
			goto error;

		switch (wValue) {
		case USB_PORT_FEAT_ENABLE:
			break;
		case USB_PORT_FEAT_SUSPEND:
			musb_port_suspend(musb, false);
			break;
		case USB_PORT_FEAT_POWER:
<<<<<<< HEAD
			if (!(is_otg_enabled(musb) && hcd->self.is_b_host))
=======
			if (!hcd->self.is_b_host)
>>>>>>> refs/remotes/origin/master
				musb_platform_set_vbus(musb, 0);
			break;
		case USB_PORT_FEAT_C_CONNECTION:
		case USB_PORT_FEAT_C_ENABLE:
		case USB_PORT_FEAT_C_OVER_CURRENT:
		case USB_PORT_FEAT_C_RESET:
		case USB_PORT_FEAT_C_SUSPEND:
			break;
		default:
			goto error;
		}
		dev_dbg(musb->controller, "clear feature %d\n", wValue);
		musb->port1_status &= ~(1 << wValue);
		break;
	case GetHubDescriptor:
		{
		struct usb_hub_descriptor *desc = (void *)buf;

		desc->bDescLength = 9;
		desc->bDescriptorType = 0x29;
		desc->bNbrPorts = 1;
		desc->wHubCharacteristics = cpu_to_le16(
				  0x0001	/* per-port power switching */
				| 0x0010	/* no overcurrent reporting */
				);
		desc->bPwrOn2PwrGood = 5;	/* msec/2 */
		desc->bHubContrCurrent = 0;

		/* workaround bogus struct definition */
		desc->u.hs.DeviceRemovable[0] = 0x02;	/* port 1 */
		desc->u.hs.DeviceRemovable[1] = 0xff;
		}
		break;
	case GetHubStatus:
		temp = 0;
		*(__le32 *) buf = cpu_to_le32(temp);
		break;
	case GetPortStatus:
		if (wIndex != 1)
			goto error;

<<<<<<< HEAD
		/* finish RESET signaling? */
		if ((musb->port1_status & USB_PORT_STAT_RESET)
				&& time_after_eq(jiffies, musb->rh_timer))
			musb_port_reset(musb, false);

		/* finish RESUME signaling? */
		if ((musb->port1_status & MUSB_PORT_STAT_RESUME)
				&& time_after_eq(jiffies, musb->rh_timer)) {
			u8		power;

			power = musb_readb(musb->mregs, MUSB_POWER);
			power &= ~MUSB_POWER_RESUME;
			dev_dbg(musb->controller, "root port resume stopped, power %02x\n",
					power);
			musb_writeb(musb->mregs, MUSB_POWER, power);

			/* ISSUE:  DaVinci (RTL 1.300) disconnects after
			 * resume of high speed peripherals (but not full
			 * speed ones).
			 */

			musb->is_active = 1;
			musb->port1_status &= ~(USB_PORT_STAT_SUSPEND
					| MUSB_PORT_STAT_RESUME);
			musb->port1_status |= USB_PORT_STAT_C_SUSPEND << 16;
			usb_hcd_poll_rh_status(musb_to_hcd(musb));
			/* NOTE: it might really be A_WAIT_BCON ... */
			musb->xceiv->state = OTG_STATE_A_HOST;
		}

=======
>>>>>>> refs/remotes/origin/master
		put_unaligned(cpu_to_le32(musb->port1_status
					& ~MUSB_PORT_STAT_RESUME),
				(__le32 *) buf);

		/* port change status is more interesting */
		dev_dbg(musb->controller, "port status %08x\n",
				musb->port1_status);
		break;
	case SetPortFeature:
		if ((wIndex & 0xff) != 1)
			goto error;

		switch (wValue) {
		case USB_PORT_FEAT_POWER:
			/* NOTE: this controller has a strange state machine
			 * that involves "requesting sessions" according to
			 * magic side effects from incompletely-described
			 * rules about startup...
			 *
			 * This call is what really starts the host mode; be
			 * very careful about side effects if you reorder any
			 * initialization logic, e.g. for OTG, or change any
			 * logic relating to VBUS power-up.
			 */
<<<<<<< HEAD
			if (!(is_otg_enabled(musb) && hcd->self.is_b_host))
=======
			if (!hcd->self.is_b_host && musb_has_gadget(musb))
>>>>>>> refs/remotes/origin/master
				musb_start(musb);
			break;
		case USB_PORT_FEAT_RESET:
			musb_port_reset(musb, true);
			break;
		case USB_PORT_FEAT_SUSPEND:
			musb_port_suspend(musb, true);
			break;
		case USB_PORT_FEAT_TEST:
			if (unlikely(is_host_active(musb)))
				goto error;

			wIndex >>= 8;
			switch (wIndex) {
			case 1:
				pr_debug("TEST_J\n");
				temp = MUSB_TEST_J;
				break;
			case 2:
				pr_debug("TEST_K\n");
				temp = MUSB_TEST_K;
				break;
			case 3:
				pr_debug("TEST_SE0_NAK\n");
				temp = MUSB_TEST_SE0_NAK;
				break;
			case 4:
				pr_debug("TEST_PACKET\n");
				temp = MUSB_TEST_PACKET;
				musb_load_testpacket(musb);
				break;
			case 5:
				pr_debug("TEST_FORCE_ENABLE\n");
				temp = MUSB_TEST_FORCE_HOST
					| MUSB_TEST_FORCE_HS;

				musb_writeb(musb->mregs, MUSB_DEVCTL,
						MUSB_DEVCTL_SESSION);
				break;
			case 6:
				pr_debug("TEST_FIFO_ACCESS\n");
				temp = MUSB_TEST_FIFO_ACCESS;
				break;
			default:
				goto error;
			}
			musb_writeb(musb->mregs, MUSB_TESTMODE, temp);
			break;
		default:
			goto error;
		}
		dev_dbg(musb->controller, "set feature %d\n", wValue);
		musb->port1_status |= 1 << wValue;
		break;

	default:
error:
		/* "protocol stall" on error */
		retval = -EPIPE;
	}
	spin_unlock_irqrestore(&musb->lock, flags);
	return retval;
}
