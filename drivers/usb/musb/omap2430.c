/*
 * Copyright (C) 2005-2007 by Texas Instruments
 * Some code has been taken from tusb6010.c
 * Copyrights for that are attributable to:
 * Copyright (C) 2006 Nokia Corporation
 * Tony Lindgren <tony@atomide.com>
 *
 * This file is part of the Inventra Controller Driver for Linux.
 *
 * The Inventra Controller Driver for Linux is free software; you
 * can redistribute it and/or modify it under the terms of the GNU
 * General Public License version 2 as published by the Free Software
 * Foundation.
 *
 * The Inventra Controller Driver for Linux is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
 * License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with The Inventra Controller Driver for Linux ; if not,
 * write to the Free Software Foundation, Inc., 59 Temple Place,
 * Suite 330, Boston, MA  02111-1307  USA
 *
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/list.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/clk.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/io.h>
=======
#include <linux/io.h>
#include <linux/of.h>
>>>>>>> refs/remotes/origin/master
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/pm_runtime.h>
#include <linux/err.h>
<<<<<<< HEAD
=======
#include <linux/delay.h>
#include <linux/usb/musb-omap.h>
#include <linux/usb/omap_control_usb.h>
#include <linux/of_platform.h>
>>>>>>> refs/remotes/origin/master

#include "musb_core.h"
#include "omap2430.h"

struct omap2430_glue {
	struct device		*dev;
	struct platform_device	*musb;
<<<<<<< HEAD
};
#define glue_to_musb(g)		platform_get_drvdata(g->musb)

=======
	enum omap_musb_vbus_id_status status;
	struct work_struct	omap_musb_mailbox_work;
	struct device		*control_otghs;
};
#define glue_to_musb(g)		platform_get_drvdata(g->musb)

static struct omap2430_glue	*_glue;

>>>>>>> refs/remotes/origin/master
static struct timer_list musb_idle_timer;

static void musb_do_idle(unsigned long _musb)
{
	struct musb	*musb = (void *)_musb;
	unsigned long	flags;
<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_USB_MUSB_HDRC_HCD
	u8	power;
#endif
=======
	u8	power;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	u8	power;
>>>>>>> refs/remotes/origin/master
	u8	devctl;

	spin_lock_irqsave(&musb->lock, flags);

	switch (musb->xceiv->state) {
	case OTG_STATE_A_WAIT_BCON:

		devctl = musb_readb(musb->mregs, MUSB_DEVCTL);
		if (devctl & MUSB_DEVCTL_BDEVICE) {
			musb->xceiv->state = OTG_STATE_B_IDLE;
			MUSB_DEV_MODE(musb);
		} else {
			musb->xceiv->state = OTG_STATE_A_IDLE;
			MUSB_HST_MODE(musb);
		}
		break;
<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_USB_MUSB_HDRC_HCD
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	case OTG_STATE_A_SUSPEND:
		/* finish RESUME signaling? */
		if (musb->port1_status & MUSB_PORT_STAT_RESUME) {
			power = musb_readb(musb->mregs, MUSB_POWER);
			power &= ~MUSB_POWER_RESUME;
			dev_dbg(musb->controller, "root port resume stopped, power %02x\n", power);
			musb_writeb(musb->mregs, MUSB_POWER, power);
			musb->is_active = 1;
			musb->port1_status &= ~(USB_PORT_STAT_SUSPEND
						| MUSB_PORT_STAT_RESUME);
			musb->port1_status |= USB_PORT_STAT_C_SUSPEND << 16;
<<<<<<< HEAD
			usb_hcd_poll_rh_status(musb_to_hcd(musb));
=======
			usb_hcd_poll_rh_status(musb->hcd);
>>>>>>> refs/remotes/origin/master
			/* NOTE: it might really be A_WAIT_BCON ... */
			musb->xceiv->state = OTG_STATE_A_HOST;
		}
		break;
<<<<<<< HEAD
<<<<<<< HEAD
#endif
#ifdef CONFIG_USB_MUSB_HDRC_HCD
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	case OTG_STATE_A_HOST:
		devctl = musb_readb(musb->mregs, MUSB_DEVCTL);
		if (devctl &  MUSB_DEVCTL_BDEVICE)
			musb->xceiv->state = OTG_STATE_B_IDLE;
		else
			musb->xceiv->state = OTG_STATE_A_WAIT_BCON;
<<<<<<< HEAD
<<<<<<< HEAD
#endif
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	default:
		break;
	}
	spin_unlock_irqrestore(&musb->lock, flags);
}


static void omap2430_musb_try_idle(struct musb *musb, unsigned long timeout)
{
	unsigned long		default_timeout = jiffies + msecs_to_jiffies(3);
	static unsigned long	last_timer;

	if (timeout == 0)
		timeout = default_timeout;

	/* Never idle if active, or when VBUS timeout is not set as host */
	if (musb->is_active || ((musb->a_wait_bcon == 0)
			&& (musb->xceiv->state == OTG_STATE_A_WAIT_BCON))) {
		dev_dbg(musb->controller, "%s active, deleting timer\n",
<<<<<<< HEAD
			otg_state_string(musb->xceiv->state));
=======
			usb_otg_state_string(musb->xceiv->state));
>>>>>>> refs/remotes/origin/master
		del_timer(&musb_idle_timer);
		last_timer = jiffies;
		return;
	}

	if (time_after(last_timer, timeout)) {
		if (!timer_pending(&musb_idle_timer))
			last_timer = timeout;
		else {
			dev_dbg(musb->controller, "Longer idle timer already pending, ignoring\n");
			return;
		}
	}
	last_timer = timeout;

	dev_dbg(musb->controller, "%s inactive, for idle timer for %lu ms\n",
<<<<<<< HEAD
		otg_state_string(musb->xceiv->state),
=======
		usb_otg_state_string(musb->xceiv->state),
>>>>>>> refs/remotes/origin/master
		(unsigned long)jiffies_to_msecs(timeout - jiffies));
	mod_timer(&musb_idle_timer, timeout);
}

static void omap2430_musb_set_vbus(struct musb *musb, int is_on)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct usb_otg	*otg = musb->xceiv->otg;
>>>>>>> refs/remotes/origin/cm-10.0
	u8		devctl;
	unsigned long timeout = jiffies + msecs_to_jiffies(1000);
	int ret = 1;
=======
	struct usb_otg	*otg = musb->xceiv->otg;
	u8		devctl;
	unsigned long timeout = jiffies + msecs_to_jiffies(1000);
>>>>>>> refs/remotes/origin/master
	/* HDRC controls CPEN, but beware current surges during device
	 * connect.  They can trigger transient overcurrent conditions
	 * that must be ignored.
	 */

	devctl = musb_readb(musb->mregs, MUSB_DEVCTL);

	if (is_on) {
		if (musb->xceiv->state == OTG_STATE_A_IDLE) {
<<<<<<< HEAD
=======
			int loops = 100;
>>>>>>> refs/remotes/origin/master
			/* start the session */
			devctl |= MUSB_DEVCTL_SESSION;
			musb_writeb(musb->mregs, MUSB_DEVCTL, devctl);
			/*
			 * Wait for the musb to set as A device to enable the
			 * VBUS
			 */
			while (musb_readb(musb->mregs, MUSB_DEVCTL) & 0x80) {

<<<<<<< HEAD
				cpu_relax();

				if (time_after(jiffies, timeout)) {
					dev_err(musb->controller,
					"configured as A device timeout");
					ret = -EINVAL;
=======
				mdelay(5);
				cpu_relax();

				if (time_after(jiffies, timeout)
				    || loops-- <= 0) {
					dev_err(musb->controller,
					"configured as A device timeout");
>>>>>>> refs/remotes/origin/master
					break;
				}
			}

<<<<<<< HEAD
<<<<<<< HEAD
			if (ret && musb->xceiv->set_vbus)
				otg_set_vbus(musb->xceiv, 1);
		} else {
			musb->is_active = 1;
			musb->xceiv->default_a = 1;
=======
			if (ret && otg->set_vbus)
				otg_set_vbus(otg, 1);
		} else {
			musb->is_active = 1;
			otg->default_a = 1;
>>>>>>> refs/remotes/origin/cm-10.0
=======
			otg_set_vbus(otg, 1);
		} else {
			musb->is_active = 1;
			otg->default_a = 1;
>>>>>>> refs/remotes/origin/master
			musb->xceiv->state = OTG_STATE_A_WAIT_VRISE;
			devctl |= MUSB_DEVCTL_SESSION;
			MUSB_HST_MODE(musb);
		}
	} else {
		musb->is_active = 0;

		/* NOTE:  we're skipping A_WAIT_VFALL -> A_IDLE and
		 * jumping right to B_IDLE...
		 */

<<<<<<< HEAD
<<<<<<< HEAD
		musb->xceiv->default_a = 0;
=======
		otg->default_a = 0;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		otg->default_a = 0;
>>>>>>> refs/remotes/origin/master
		musb->xceiv->state = OTG_STATE_B_IDLE;
		devctl &= ~MUSB_DEVCTL_SESSION;

		MUSB_DEV_MODE(musb);
	}
	musb_writeb(musb->mregs, MUSB_DEVCTL, devctl);

	dev_dbg(musb->controller, "VBUS %s, devctl %02x "
		/* otg %3x conf %08x prcm %08x */ "\n",
<<<<<<< HEAD
		otg_state_string(musb->xceiv->state),
=======
		usb_otg_state_string(musb->xceiv->state),
>>>>>>> refs/remotes/origin/master
		musb_readb(musb->mregs, MUSB_DEVCTL));
}

static int omap2430_musb_set_mode(struct musb *musb, u8 musb_mode)
{
	u8	devctl = musb_readb(musb->mregs, MUSB_DEVCTL);

	devctl |= MUSB_DEVCTL_SESSION;
	musb_writeb(musb->mregs, MUSB_DEVCTL, devctl);

	return 0;
}

static inline void omap2430_low_level_exit(struct musb *musb)
{
	u32 l;

	/* in any role */
	l = musb_readl(musb->mregs, OTG_FORCESTDBY);
	l |= ENABLEFORCE;	/* enable MSTANDBY */
	musb_writel(musb->mregs, OTG_FORCESTDBY, l);
}

static inline void omap2430_low_level_init(struct musb *musb)
{
	u32 l;

	l = musb_readl(musb->mregs, OTG_FORCESTDBY);
	l &= ~ENABLEFORCE;	/* disable MSTANDBY */
	musb_writel(musb->mregs, OTG_FORCESTDBY, l);
}

<<<<<<< HEAD
<<<<<<< HEAD
/* blocking notifier support */
=======
>>>>>>> refs/remotes/origin/cm-10.0
static int musb_otg_notifications(struct notifier_block *nb,
		unsigned long event, void *unused)
{
	struct musb	*musb = container_of(nb, struct musb, nb);
<<<<<<< HEAD
=======

	musb->xceiv_event = event;
	schedule_work(&musb->otg_notifier_work);

	return NOTIFY_OK;
}

static void musb_otg_notifier_work(struct work_struct *data_notifier_work)
{
	struct musb *musb = container_of(data_notifier_work, struct musb, otg_notifier_work);
>>>>>>> refs/remotes/origin/cm-10.0
	struct device *dev = musb->controller;
	struct musb_hdrc_platform_data *pdata = dev->platform_data;
	struct omap_musb_board_data *data = pdata->board_data;

<<<<<<< HEAD
	switch (event) {
	case USB_EVENT_ID:
		dev_dbg(musb->controller, "ID GND\n");

		if (is_otg_enabled(musb)) {
#ifdef CONFIG_USB_GADGET_MUSB_HDRC
			if (musb->gadget_driver) {
				pm_runtime_get_sync(musb->controller);
				otg_init(musb->xceiv);
				omap2430_musb_set_vbus(musb, 1);
			}
#endif
		} else {
			pm_runtime_get_sync(musb->controller);
			otg_init(musb->xceiv);
=======
	switch (musb->xceiv_event) {
	case USB_EVENT_ID:
		dev_dbg(musb->controller, "ID GND\n");

		if (!is_otg_enabled(musb) || musb->gadget_driver) {
			pm_runtime_get_sync(musb->controller);
			usb_phy_init(musb->xceiv);
>>>>>>> refs/remotes/origin/cm-10.0
=======
void omap_musb_mailbox(enum omap_musb_vbus_id_status status)
{
	struct omap2430_glue	*glue = _glue;

	if (!glue) {
		pr_err("%s: musb core is not yet initialized\n", __func__);
		return;
	}
	glue->status = status;

	if (!glue_to_musb(glue)) {
		pr_err("%s: musb core is not yet ready\n", __func__);
		return;
	}

	schedule_work(&glue->omap_musb_mailbox_work);
}
EXPORT_SYMBOL_GPL(omap_musb_mailbox);

static void omap_musb_set_mailbox(struct omap2430_glue *glue)
{
	struct musb *musb = glue_to_musb(glue);
	struct device *dev = musb->controller;
	struct musb_hdrc_platform_data *pdata = dev_get_platdata(dev);
	struct omap_musb_board_data *data = pdata->board_data;
	struct usb_otg *otg = musb->xceiv->otg;

	switch (glue->status) {
	case OMAP_MUSB_ID_GROUND:
		dev_dbg(dev, "ID GND\n");

		otg->default_a = true;
		musb->xceiv->state = OTG_STATE_A_IDLE;
		musb->xceiv->last_event = USB_EVENT_ID;
		if (musb->gadget_driver) {
			pm_runtime_get_sync(dev);
			omap_control_usb_set_mode(glue->control_otghs,
				USB_MODE_HOST);
>>>>>>> refs/remotes/origin/master
			omap2430_musb_set_vbus(musb, 1);
		}
		break;

<<<<<<< HEAD
	case USB_EVENT_VBUS:
		dev_dbg(musb->controller, "VBUS Connect\n");

<<<<<<< HEAD
#ifdef CONFIG_USB_GADGET_MUSB_HDRC
		if (musb->gadget_driver)
			pm_runtime_get_sync(musb->controller);
#endif
		otg_init(musb->xceiv);
=======
		if (musb->gadget_driver)
			pm_runtime_get_sync(musb->controller);
		usb_phy_init(musb->xceiv);
>>>>>>> refs/remotes/origin/cm-10.0
		break;

	case USB_EVENT_NONE:
		dev_dbg(musb->controller, "VBUS Disconnect\n");

<<<<<<< HEAD
#ifdef CONFIG_USB_GADGET_MUSB_HDRC
		if (is_otg_enabled(musb) || is_peripheral_enabled(musb))
			if (musb->gadget_driver)
#endif
			{
=======
		if (is_otg_enabled(musb) || is_peripheral_enabled(musb))
			if (musb->gadget_driver) {
>>>>>>> refs/remotes/origin/cm-10.0
				pm_runtime_mark_last_busy(musb->controller);
				pm_runtime_put_autosuspend(musb->controller);
			}

		if (data->interface_type == MUSB_INTERFACE_UTMI) {
<<<<<<< HEAD
			if (musb->xceiv->set_vbus)
				otg_set_vbus(musb->xceiv, 0);
		}
		otg_shutdown(musb->xceiv);
		break;
	default:
		dev_dbg(musb->controller, "ID float\n");
		return NOTIFY_DONE;
	}

	return NOTIFY_OK;
=======
			if (musb->xceiv->otg->set_vbus)
				otg_set_vbus(musb->xceiv->otg, 0);
		}
		usb_phy_shutdown(musb->xceiv);
		break;
	default:
		dev_dbg(musb->controller, "ID float\n");
	}
>>>>>>> refs/remotes/origin/cm-10.0
=======
	case OMAP_MUSB_VBUS_VALID:
		dev_dbg(dev, "VBUS Connect\n");

		otg->default_a = false;
		musb->xceiv->state = OTG_STATE_B_IDLE;
		musb->xceiv->last_event = USB_EVENT_VBUS;
		if (musb->gadget_driver)
			pm_runtime_get_sync(dev);
		omap_control_usb_set_mode(glue->control_otghs, USB_MODE_DEVICE);
		break;

	case OMAP_MUSB_ID_FLOAT:
	case OMAP_MUSB_VBUS_OFF:
		dev_dbg(dev, "VBUS Disconnect\n");

		musb->xceiv->last_event = USB_EVENT_NONE;
		if (musb->gadget_driver) {
			omap2430_musb_set_vbus(musb, 0);
			pm_runtime_mark_last_busy(dev);
			pm_runtime_put_autosuspend(dev);
		}

		if (data->interface_type == MUSB_INTERFACE_UTMI)
			otg_set_vbus(musb->xceiv->otg, 0);

		omap_control_usb_set_mode(glue->control_otghs,
			USB_MODE_DISCONNECT);
		break;
	default:
		dev_dbg(dev, "ID float\n");
	}

	atomic_notifier_call_chain(&musb->xceiv->notifier,
			musb->xceiv->last_event, NULL);
}


static void omap_musb_mailbox_work(struct work_struct *mailbox_work)
{
	struct omap2430_glue *glue = container_of(mailbox_work,
				struct omap2430_glue, omap_musb_mailbox_work);
	omap_musb_set_mailbox(glue);
}

static irqreturn_t omap2430_musb_interrupt(int irq, void *__hci)
{
	unsigned long   flags;
	irqreturn_t     retval = IRQ_NONE;
	struct musb     *musb = __hci;

	spin_lock_irqsave(&musb->lock, flags);

	musb->int_usb = musb_readb(musb->mregs, MUSB_INTRUSB);
	musb->int_tx = musb_readw(musb->mregs, MUSB_INTRTX);
	musb->int_rx = musb_readw(musb->mregs, MUSB_INTRRX);

	if (musb->int_usb || musb->int_tx || musb->int_rx)
		retval = musb_interrupt(musb);

	spin_unlock_irqrestore(&musb->lock, flags);

	return retval;
>>>>>>> refs/remotes/origin/master
}

static int omap2430_musb_init(struct musb *musb)
{
	u32 l;
	int status = 0;
	struct device *dev = musb->controller;
<<<<<<< HEAD
	struct musb_hdrc_platform_data *plat = dev->platform_data;
=======
	struct omap2430_glue *glue = dev_get_drvdata(dev->parent);
	struct musb_hdrc_platform_data *plat = dev_get_platdata(dev);
>>>>>>> refs/remotes/origin/master
	struct omap_musb_board_data *data = plat->board_data;

	/* We require some kind of external transceiver, hooked
	 * up through ULPI.  TWL4030-family PMICs include one,
	 * which needs a driver, drivers aren't always needed.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	musb->xceiv = otg_get_transceiver();
=======
	musb->xceiv = usb_get_transceiver();
>>>>>>> refs/remotes/origin/cm-10.0
	if (!musb->xceiv) {
		pr_err("HS USB OTG: no transceiver configured\n");
		return -ENODEV;
	}

<<<<<<< HEAD
=======
	INIT_WORK(&musb->otg_notifier_work, musb_otg_notifier_work);

>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (dev->parent->of_node) {
		musb->phy = devm_phy_get(dev->parent, "usb2-phy");

		/* We can't totally remove musb->xceiv as of now because
		 * musb core uses xceiv.state and xceiv.otg. Once we have
		 * a separate state machine to handle otg, these can be moved
		 * out of xceiv and then we can start using the generic PHY
		 * framework
		 */
		musb->xceiv = devm_usb_get_phy_by_phandle(dev->parent,
		    "usb-phy", 0);
	} else {
		musb->xceiv = devm_usb_get_phy_dev(dev, 0);
		musb->phy = devm_phy_get(dev, "usb");
	}

	if (IS_ERR(musb->xceiv)) {
		status = PTR_ERR(musb->xceiv);

		if (status == -ENXIO)
			return status;

		pr_err("HS USB OTG: no transceiver configured\n");
		return -EPROBE_DEFER;
	}

	if (IS_ERR(musb->phy)) {
		pr_err("HS USB OTG: no PHY configured\n");
		return PTR_ERR(musb->phy);
	}
	musb->isr = omap2430_musb_interrupt;

>>>>>>> refs/remotes/origin/master
	status = pm_runtime_get_sync(dev);
	if (status < 0) {
		dev_err(dev, "pm_runtime_get_sync FAILED %d\n", status);
		goto err1;
	}

	l = musb_readl(musb->mregs, OTG_INTERFSEL);

	if (data->interface_type == MUSB_INTERFACE_UTMI) {
		/* OMAP4 uses Internal PHY GS70 which uses UTMI interface */
		l &= ~ULPI_12PIN;       /* Disable ULPI */
		l |= UTMI_8BIT;         /* Enable UTMI  */
	} else {
		l |= ULPI_12PIN;
	}

	musb_writel(musb->mregs, OTG_INTERFSEL, l);

	pr_debug("HS USB OTG: revision 0x%x, sysconfig 0x%02x, "
			"sysstatus 0x%x, intrfsel 0x%x, simenable  0x%x\n",
			musb_readl(musb->mregs, OTG_REVISION),
			musb_readl(musb->mregs, OTG_SYSCONFIG),
			musb_readl(musb->mregs, OTG_SYSSTATUS),
			musb_readl(musb->mregs, OTG_INTERFSEL),
			musb_readl(musb->mregs, OTG_SIMENABLE));

<<<<<<< HEAD
	musb->nb.notifier_call = musb_otg_notifications;
<<<<<<< HEAD
	status = otg_register_notifier(musb->xceiv, &musb->nb);
=======
	status = usb_register_notifier(musb->xceiv, &musb->nb);
>>>>>>> refs/remotes/origin/cm-10.0

	if (status)
		dev_dbg(musb->controller, "notification register failed\n");

	setup_timer(&musb_idle_timer, musb_do_idle, (unsigned long) musb);

<<<<<<< HEAD
	return 0;

err1:
	pm_runtime_disable(dev);
=======
=======
	setup_timer(&musb_idle_timer, musb_do_idle, (unsigned long) musb);

	if (glue->status != OMAP_MUSB_UNKNOWN)
		omap_musb_set_mailbox(glue);

	phy_init(musb->phy);

>>>>>>> refs/remotes/origin/master
	pm_runtime_put_noidle(musb->controller);
	return 0;

err1:
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return status;
}

static void omap2430_musb_enable(struct musb *musb)
{
	u8		devctl;
	unsigned long timeout = jiffies + msecs_to_jiffies(1000);
	struct device *dev = musb->controller;
<<<<<<< HEAD
	struct musb_hdrc_platform_data *pdata = dev->platform_data;
	struct omap_musb_board_data *data = pdata->board_data;

	switch (musb->xceiv->last_event) {

	case USB_EVENT_ID:
<<<<<<< HEAD
		otg_init(musb->xceiv);
		if (data->interface_type == MUSB_INTERFACE_UTMI) {
			devctl = musb_readb(musb->mregs, MUSB_DEVCTL);
			/* start the session */
			devctl |= MUSB_DEVCTL_SESSION;
			musb_writeb(musb->mregs, MUSB_DEVCTL, devctl);
			while (musb_readb(musb->mregs, MUSB_DEVCTL) &
						MUSB_DEVCTL_BDEVICE) {
				cpu_relax();

				if (time_after(jiffies, timeout)) {
					dev_err(musb->controller,
					"configured as A device timeout");
					break;
				}
=======
		usb_phy_init(musb->xceiv);
=======
	struct omap2430_glue *glue = dev_get_drvdata(dev->parent);
	struct musb_hdrc_platform_data *pdata = dev_get_platdata(dev);
	struct omap_musb_board_data *data = pdata->board_data;

	switch (glue->status) {

	case OMAP_MUSB_ID_GROUND:
		omap_control_usb_set_mode(glue->control_otghs, USB_MODE_HOST);
>>>>>>> refs/remotes/origin/master
		if (data->interface_type != MUSB_INTERFACE_UTMI)
			break;
		devctl = musb_readb(musb->mregs, MUSB_DEVCTL);
		/* start the session */
		devctl |= MUSB_DEVCTL_SESSION;
		musb_writeb(musb->mregs, MUSB_DEVCTL, devctl);
		while (musb_readb(musb->mregs, MUSB_DEVCTL) &
				MUSB_DEVCTL_BDEVICE) {
			cpu_relax();

			if (time_after(jiffies, timeout)) {
				dev_err(dev, "configured as A device timeout");
				break;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			}
		}
		break;

<<<<<<< HEAD
	case USB_EVENT_VBUS:
<<<<<<< HEAD
		otg_init(musb->xceiv);
=======
		usb_phy_init(musb->xceiv);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	case OMAP_MUSB_VBUS_VALID:
		omap_control_usb_set_mode(glue->control_otghs, USB_MODE_DEVICE);
>>>>>>> refs/remotes/origin/master
		break;

	default:
		break;
	}
}

static void omap2430_musb_disable(struct musb *musb)
{
<<<<<<< HEAD
	if (musb->xceiv->last_event)
<<<<<<< HEAD
		otg_shutdown(musb->xceiv);
=======
		usb_phy_shutdown(musb->xceiv);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct device *dev = musb->controller;
	struct omap2430_glue *glue = dev_get_drvdata(dev->parent);

	if (glue->status != OMAP_MUSB_UNKNOWN)
		omap_control_usb_set_mode(glue->control_otghs,
			USB_MODE_DISCONNECT);
>>>>>>> refs/remotes/origin/master
}

static int omap2430_musb_exit(struct musb *musb)
{
	del_timer_sync(&musb_idle_timer);
<<<<<<< HEAD
<<<<<<< HEAD

	omap2430_low_level_exit(musb);
	otg_put_transceiver(musb->xceiv);
=======
	cancel_work_sync(&musb->otg_notifier_work);

	omap2430_low_level_exit(musb);
	usb_put_transceiver(musb->xceiv);
>>>>>>> refs/remotes/origin/cm-10.0
=======

	omap2430_low_level_exit(musb);
	phy_exit(musb->phy);
>>>>>>> refs/remotes/origin/master

	return 0;
}

static const struct musb_platform_ops omap2430_ops = {
	.init		= omap2430_musb_init,
	.exit		= omap2430_musb_exit,

	.set_mode	= omap2430_musb_set_mode,
	.try_idle	= omap2430_musb_try_idle,

	.set_vbus	= omap2430_musb_set_vbus,

	.enable		= omap2430_musb_enable,
	.disable	= omap2430_musb_disable,
};

static u64 omap2430_dmamask = DMA_BIT_MASK(32);

<<<<<<< HEAD
<<<<<<< HEAD
static int __init omap2430_probe(struct platform_device *pdev)
=======
static int __devinit omap2430_probe(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct musb_hdrc_platform_data	*pdata = pdev->dev.platform_data;
	struct platform_device		*musb;
	struct omap2430_glue		*glue;
	int				ret = -ENOMEM;

	glue = kzalloc(sizeof(*glue), GFP_KERNEL);
=======
static int omap2430_probe(struct platform_device *pdev)
{
	struct resource			musb_resources[3];
	struct musb_hdrc_platform_data	*pdata = dev_get_platdata(&pdev->dev);
	struct omap_musb_board_data	*data;
	struct platform_device		*musb;
	struct omap2430_glue		*glue;
	struct device_node		*np = pdev->dev.of_node;
	struct musb_hdrc_config		*config;
	int				ret = -ENOMEM;

	glue = devm_kzalloc(&pdev->dev, sizeof(*glue), GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (!glue) {
		dev_err(&pdev->dev, "failed to allocate glue context\n");
		goto err0;
	}

<<<<<<< HEAD
	musb = platform_device_alloc("musb-hdrc", -1);
	if (!musb) {
		dev_err(&pdev->dev, "failed to allocate musb device\n");
		goto err1;
=======
	musb = platform_device_alloc("musb-hdrc", PLATFORM_DEVID_AUTO);
	if (!musb) {
		dev_err(&pdev->dev, "failed to allocate musb device\n");
		goto err0;
>>>>>>> refs/remotes/origin/master
	}

	musb->dev.parent		= &pdev->dev;
	musb->dev.dma_mask		= &omap2430_dmamask;
	musb->dev.coherent_dma_mask	= omap2430_dmamask;

	glue->dev			= &pdev->dev;
	glue->musb			= musb;
<<<<<<< HEAD

=======
	glue->status			= OMAP_MUSB_UNKNOWN;
	glue->control_otghs = ERR_PTR(-ENODEV);

	if (np) {
		struct device_node *control_node;
		struct platform_device *control_pdev;

		pdata = devm_kzalloc(&pdev->dev, sizeof(*pdata), GFP_KERNEL);
		if (!pdata) {
			dev_err(&pdev->dev,
				"failed to allocate musb platform data\n");
			goto err2;
		}

		data = devm_kzalloc(&pdev->dev, sizeof(*data), GFP_KERNEL);
		if (!data) {
			dev_err(&pdev->dev,
				"failed to allocate musb board data\n");
			goto err2;
		}

		config = devm_kzalloc(&pdev->dev, sizeof(*config), GFP_KERNEL);
		if (!config) {
			dev_err(&pdev->dev,
				"failed to allocate musb hdrc config\n");
			goto err2;
		}

		of_property_read_u32(np, "mode", (u32 *)&pdata->mode);
		of_property_read_u32(np, "interface-type",
						(u32 *)&data->interface_type);
		of_property_read_u32(np, "num-eps", (u32 *)&config->num_eps);
		of_property_read_u32(np, "ram-bits", (u32 *)&config->ram_bits);
		of_property_read_u32(np, "power", (u32 *)&pdata->power);
		config->multipoint = of_property_read_bool(np, "multipoint");

		pdata->board_data	= data;
		pdata->config		= config;

		control_node = of_parse_phandle(np, "ctrl-module", 0);
		if (control_node) {
			control_pdev = of_find_device_by_node(control_node);
			if (!control_pdev) {
				dev_err(&pdev->dev, "Failed to get control device\n");
				ret = -EINVAL;
				goto err2;
			}
			glue->control_otghs = &control_pdev->dev;
		}
	}
>>>>>>> refs/remotes/origin/master
	pdata->platform_ops		= &omap2430_ops;

	platform_set_drvdata(pdev, glue);

<<<<<<< HEAD
	ret = platform_device_add_resources(musb, pdev->resource,
			pdev->num_resources);
=======
	/*
	 * REVISIT if we ever have two instances of the wrapper, we will be
	 * in big trouble
	 */
	_glue	= glue;

	INIT_WORK(&glue->omap_musb_mailbox_work, omap_musb_mailbox_work);

	memset(musb_resources, 0x00, sizeof(*musb_resources) *
			ARRAY_SIZE(musb_resources));

	musb_resources[0].name = pdev->resource[0].name;
	musb_resources[0].start = pdev->resource[0].start;
	musb_resources[0].end = pdev->resource[0].end;
	musb_resources[0].flags = pdev->resource[0].flags;

	musb_resources[1].name = pdev->resource[1].name;
	musb_resources[1].start = pdev->resource[1].start;
	musb_resources[1].end = pdev->resource[1].end;
	musb_resources[1].flags = pdev->resource[1].flags;

	musb_resources[2].name = pdev->resource[2].name;
	musb_resources[2].start = pdev->resource[2].start;
	musb_resources[2].end = pdev->resource[2].end;
	musb_resources[2].flags = pdev->resource[2].flags;

	ret = platform_device_add_resources(musb, musb_resources,
			ARRAY_SIZE(musb_resources));
>>>>>>> refs/remotes/origin/master
	if (ret) {
		dev_err(&pdev->dev, "failed to add resources\n");
		goto err2;
	}

	ret = platform_device_add_data(musb, pdata, sizeof(*pdata));
	if (ret) {
		dev_err(&pdev->dev, "failed to add platform_data\n");
		goto err2;
	}

	pm_runtime_enable(&pdev->dev);

	ret = platform_device_add(musb);
	if (ret) {
		dev_err(&pdev->dev, "failed to register musb device\n");
		goto err2;
	}

	return 0;

err2:
	platform_device_put(musb);

<<<<<<< HEAD
err1:
	kfree(glue);

=======
>>>>>>> refs/remotes/origin/master
err0:
	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int __exit omap2430_remove(struct platform_device *pdev)
=======
static int __devexit omap2430_remove(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct omap2430_glue		*glue = platform_get_drvdata(pdev);

	platform_device_del(glue->musb);
	platform_device_put(glue->musb);
<<<<<<< HEAD
	pm_runtime_put(&pdev->dev);
	pm_runtime_disable(&pdev->dev);
=======
>>>>>>> refs/remotes/origin/cm-10.0
	kfree(glue);
=======
static int omap2430_remove(struct platform_device *pdev)
{
	struct omap2430_glue		*glue = platform_get_drvdata(pdev);

	cancel_work_sync(&glue->omap_musb_mailbox_work);
	platform_device_unregister(glue->musb);
>>>>>>> refs/remotes/origin/master

	return 0;
}

#ifdef CONFIG_PM

static int omap2430_runtime_suspend(struct device *dev)
{
	struct omap2430_glue		*glue = dev_get_drvdata(dev);
	struct musb			*musb = glue_to_musb(glue);

<<<<<<< HEAD
<<<<<<< HEAD
	omap2430_low_level_exit(musb);
	otg_set_suspend(musb->xceiv, 1);
=======
=======
>>>>>>> refs/remotes/origin/master
	if (musb) {
		musb->context.otg_interfsel = musb_readl(musb->mregs,
				OTG_INTERFSEL);

		omap2430_low_level_exit(musb);
<<<<<<< HEAD
		usb_phy_set_suspend(musb->xceiv, 1);
	}
>>>>>>> refs/remotes/origin/cm-10.0
=======
		phy_power_off(musb->phy);
	}
>>>>>>> refs/remotes/origin/master

	return 0;
}

static int omap2430_runtime_resume(struct device *dev)
{
	struct omap2430_glue		*glue = dev_get_drvdata(dev);
	struct musb			*musb = glue_to_musb(glue);

<<<<<<< HEAD
<<<<<<< HEAD
	omap2430_low_level_init(musb);
	otg_set_suspend(musb->xceiv, 0);
=======
=======
>>>>>>> refs/remotes/origin/master
	if (musb) {
		omap2430_low_level_init(musb);
		musb_writel(musb->mregs, OTG_INTERFSEL,
				musb->context.otg_interfsel);
<<<<<<< HEAD

		usb_phy_set_suspend(musb->xceiv, 0);
	}
>>>>>>> refs/remotes/origin/cm-10.0
=======
		phy_power_on(musb->phy);
	}
>>>>>>> refs/remotes/origin/master

	return 0;
}

static struct dev_pm_ops omap2430_pm_ops = {
	.runtime_suspend = omap2430_runtime_suspend,
	.runtime_resume = omap2430_runtime_resume,
};

#define DEV_PM_OPS	(&omap2430_pm_ops)
#else
#define DEV_PM_OPS	NULL
#endif

<<<<<<< HEAD
static struct platform_driver omap2430_driver = {
<<<<<<< HEAD
	.remove		= __exit_p(omap2430_remove),
=======
	.probe		= omap2430_probe,
	.remove		= __devexit_p(omap2430_remove),
>>>>>>> refs/remotes/origin/cm-10.0
	.driver		= {
		.name	= "musb-omap2430",
		.pm	= DEV_PM_OPS,
=======
#ifdef CONFIG_OF
static const struct of_device_id omap2430_id_table[] = {
	{
		.compatible = "ti,omap4-musb"
	},
	{
		.compatible = "ti,omap3-musb"
	},
	{},
};
MODULE_DEVICE_TABLE(of, omap2430_id_table);
#endif

static struct platform_driver omap2430_driver = {
	.probe		= omap2430_probe,
	.remove		= omap2430_remove,
	.driver		= {
		.name	= "musb-omap2430",
		.pm	= DEV_PM_OPS,
		.of_match_table = of_match_ptr(omap2430_id_table),
>>>>>>> refs/remotes/origin/master
	},
};

MODULE_DESCRIPTION("OMAP2PLUS MUSB Glue Layer");
MODULE_AUTHOR("Felipe Balbi <balbi@ti.com>");
MODULE_LICENSE("GPL v2");

static int __init omap2430_init(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	return platform_driver_probe(&omap2430_driver, omap2430_probe);
}
subsys_initcall(omap2430_init);
=======
	return platform_driver_register(&omap2430_driver);
}
module_init(omap2430_init);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return platform_driver_register(&omap2430_driver);
}
subsys_initcall(omap2430_init);
>>>>>>> refs/remotes/origin/master

static void __exit omap2430_exit(void)
{
	platform_driver_unregister(&omap2430_driver);
}
module_exit(omap2430_exit);
