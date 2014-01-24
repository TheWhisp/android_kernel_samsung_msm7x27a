/* -----------------------------------------------------------------------------
 * Copyright (c) 2011 Ozmo Inc
 * Released under the GNU General Public License Version 2 (GPLv2).
 *
 * This file provides protocol independent part of the implementation of the USB
 * service for a PD.
 * The implementation of this service is split into two parts the first of which
 * is protocol independent and the second contains protocol specific details.
 * This split is to allow alternative protocols to be defined.
<<<<<<< HEAD
 * The implemenation of this service uses ozhcd.c to implement a USB HCD.
 * -----------------------------------------------------------------------------
 */
#include <linux/init.h>
=======
 * The implementation of this service uses ozhcd.c to implement a USB HCD.
 * -----------------------------------------------------------------------------
 */

>>>>>>> refs/remotes/origin/master
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/sched.h>
#include <linux/netdevice.h>
#include <linux/errno.h>
#include <linux/input.h>
#include <asm/unaligned.h>
<<<<<<< HEAD
#include "ozconfig.h"
=======
#include "ozdbg.h"
>>>>>>> refs/remotes/origin/master
#include "ozprotocol.h"
#include "ozeltbuf.h"
#include "ozpd.h"
#include "ozproto.h"
#include "ozusbif.h"
#include "ozhcd.h"
<<<<<<< HEAD
#include "oztrace.h"
#include "ozusbsvc.h"
#include "ozevent.h"
/*------------------------------------------------------------------------------
=======
#include "ozusbsvc.h"

/*
>>>>>>> refs/remotes/origin/master
 * This is called once when the driver is loaded to initialise the USB service.
 * Context: process
 */
int oz_usb_init(void)
{
<<<<<<< HEAD
	oz_event_log(OZ_EVT_SERVICE, 1, OZ_APPID_USB, 0, 0);
	return oz_hcd_init();
}
/*------------------------------------------------------------------------------
=======
	return oz_hcd_init();
}

/*
>>>>>>> refs/remotes/origin/master
 * This is called once when the driver is unloaded to terminate the USB service.
 * Context: process
 */
void oz_usb_term(void)
{
<<<<<<< HEAD
	oz_event_log(OZ_EVT_SERVICE, 2, OZ_APPID_USB, 0, 0);
	oz_hcd_term();
}
/*------------------------------------------------------------------------------
=======
	oz_hcd_term();
}

/*
>>>>>>> refs/remotes/origin/master
 * This is called when the USB service is started or resumed for a PD.
 * Context: softirq
 */
int oz_usb_start(struct oz_pd *pd, int resume)
{
	int rc = 0;
	struct oz_usb_ctx *usb_ctx;
<<<<<<< HEAD
	struct oz_usb_ctx *old_ctx = 0;
	oz_event_log(OZ_EVT_SERVICE, 3, OZ_APPID_USB, 0, resume);
	if (resume) {
		oz_trace("USB service resumed.\n");
		return 0;
	}
	oz_trace("USB service started.\n");
=======
	struct oz_usb_ctx *old_ctx;

	if (resume) {
		oz_dbg(ON, "USB service resumed\n");
		return 0;
	}
	oz_dbg(ON, "USB service started\n");
>>>>>>> refs/remotes/origin/master
	/* Create a USB context in case we need one. If we find the PD already
	 * has a USB context then we will destroy it.
	 */
	usb_ctx = kzalloc(sizeof(struct oz_usb_ctx), GFP_ATOMIC);
<<<<<<< HEAD
	if (usb_ctx == 0)
=======
	if (usb_ctx == NULL)
>>>>>>> refs/remotes/origin/master
		return -ENOMEM;
	atomic_set(&usb_ctx->ref_count, 1);
	usb_ctx->pd = pd;
	usb_ctx->stopped = 0;
	/* Install the USB context if the PD doesn't already have one.
	 * If it does already have one then destroy the one we have just
	 * created.
	 */
	spin_lock_bh(&pd->app_lock[OZ_APPID_USB-1]);
	old_ctx = pd->app_ctx[OZ_APPID_USB-1];
<<<<<<< HEAD
	if (old_ctx == 0)
=======
	if (old_ctx == NULL)
>>>>>>> refs/remotes/origin/master
		pd->app_ctx[OZ_APPID_USB-1] = usb_ctx;
	oz_usb_get(pd->app_ctx[OZ_APPID_USB-1]);
	spin_unlock_bh(&pd->app_lock[OZ_APPID_USB-1]);
	if (old_ctx) {
<<<<<<< HEAD
		oz_trace("Already have USB context.\n");
=======
		oz_dbg(ON, "Already have USB context\n");
>>>>>>> refs/remotes/origin/master
		kfree(usb_ctx);
		usb_ctx = old_ctx;
	} else if (usb_ctx) {
		/* Take a reference to the PD. This will be released when
		 * the USB context is destroyed.
		 */
		oz_pd_get(pd);
	}
	/* If we already had a USB context and had obtained a port from
	 * the USB HCD then just reset the port. If we didn't have a port
	 * then report the arrival to the USB HCD so we get one.
	 */
	if (usb_ctx->hport) {
		oz_hcd_pd_reset(usb_ctx, usb_ctx->hport);
	} else {
		usb_ctx->hport = oz_hcd_pd_arrived(usb_ctx);
<<<<<<< HEAD
		if (usb_ctx->hport == 0) {
			oz_trace("USB hub returned null port.\n");
			spin_lock_bh(&pd->app_lock[OZ_APPID_USB-1]);
			pd->app_ctx[OZ_APPID_USB-1] = 0;
=======
		if (usb_ctx->hport == NULL) {
			oz_dbg(ON, "USB hub returned null port\n");
			spin_lock_bh(&pd->app_lock[OZ_APPID_USB-1]);
			pd->app_ctx[OZ_APPID_USB-1] = NULL;
>>>>>>> refs/remotes/origin/master
			spin_unlock_bh(&pd->app_lock[OZ_APPID_USB-1]);
			oz_usb_put(usb_ctx);
			rc = -1;
		}
	}
	oz_usb_put(usb_ctx);
	return rc;
}
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> refs/remotes/origin/master
 * This is called when the USB service is stopped or paused for a PD.
 * Context: softirq or process
 */
void oz_usb_stop(struct oz_pd *pd, int pause)
{
	struct oz_usb_ctx *usb_ctx;
<<<<<<< HEAD
	oz_event_log(OZ_EVT_SERVICE, 4, OZ_APPID_USB, 0, pause);
	if (pause) {
		oz_trace("USB service paused.\n");
=======

	if (pause) {
		oz_dbg(ON, "USB service paused\n");
>>>>>>> refs/remotes/origin/master
		return;
	}
	spin_lock_bh(&pd->app_lock[OZ_APPID_USB-1]);
	usb_ctx = (struct oz_usb_ctx *)pd->app_ctx[OZ_APPID_USB-1];
<<<<<<< HEAD
	pd->app_ctx[OZ_APPID_USB-1] = 0;
	spin_unlock_bh(&pd->app_lock[OZ_APPID_USB-1]);
	if (usb_ctx) {
		unsigned long tout = jiffies + HZ;
		oz_trace("USB service stopping...\n");
=======
	pd->app_ctx[OZ_APPID_USB-1] = NULL;
	spin_unlock_bh(&pd->app_lock[OZ_APPID_USB-1]);
	if (usb_ctx) {
		struct timespec ts, now;
		getnstimeofday(&ts);
		oz_dbg(ON, "USB service stopping...\n");
>>>>>>> refs/remotes/origin/master
		usb_ctx->stopped = 1;
		/* At this point the reference count on the usb context should
		 * be 2 - one from when we created it and one from the hcd
		 * which claims a reference. Since stopped = 1 no one else
		 * should get in but someone may already be in. So wait
		 * until they leave but timeout after 1 second.
		 */
<<<<<<< HEAD
		while ((atomic_read(&usb_ctx->ref_count) > 2) &&
			time_before(jiffies, tout))
			;
		oz_trace("USB service stopped.\n");
=======
		while ((atomic_read(&usb_ctx->ref_count) > 2)) {
			getnstimeofday(&now);
			/*Approx 1 Sec. this is not perfect calculation*/
			if (now.tv_sec != ts.tv_sec)
				break;
		}
		oz_dbg(ON, "USB service stopped\n");
>>>>>>> refs/remotes/origin/master
		oz_hcd_pd_departed(usb_ctx->hport);
		/* Release the reference taken in oz_usb_start.
		 */
		oz_usb_put(usb_ctx);
	}
}
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> refs/remotes/origin/master
 * This increments the reference count of the context area for a specific PD.
 * This ensures this context area does not disappear while still in use.
 * Context: softirq
 */
void oz_usb_get(void *hpd)
{
	struct oz_usb_ctx *usb_ctx = (struct oz_usb_ctx *)hpd;
<<<<<<< HEAD
	atomic_inc(&usb_ctx->ref_count);
}
/*------------------------------------------------------------------------------
 * This decrements the reference count of the context area for a specific PD
 * and destroys the context area if the reference count becomes zero.
 * Context: softirq or process
=======

	atomic_inc(&usb_ctx->ref_count);
}

/*
 * This decrements the reference count of the context area for a specific PD
 * and destroys the context area if the reference count becomes zero.
 * Context: irq or process
>>>>>>> refs/remotes/origin/master
 */
void oz_usb_put(void *hpd)
{
	struct oz_usb_ctx *usb_ctx = (struct oz_usb_ctx *)hpd;
<<<<<<< HEAD
	if (atomic_dec_and_test(&usb_ctx->ref_count)) {
		oz_trace("Dealloc USB context.\n");
=======

	if (atomic_dec_and_test(&usb_ctx->ref_count)) {
		oz_dbg(ON, "Dealloc USB context\n");
>>>>>>> refs/remotes/origin/master
		oz_pd_put(usb_ctx->pd);
		kfree(usb_ctx);
	}
}
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> refs/remotes/origin/master
 * Context: softirq
 */
int oz_usb_heartbeat(struct oz_pd *pd)
{
	struct oz_usb_ctx *usb_ctx;
	int rc = 0;
<<<<<<< HEAD
=======

>>>>>>> refs/remotes/origin/master
	spin_lock_bh(&pd->app_lock[OZ_APPID_USB-1]);
	usb_ctx = (struct oz_usb_ctx *)pd->app_ctx[OZ_APPID_USB-1];
	if (usb_ctx)
		oz_usb_get(usb_ctx);
	spin_unlock_bh(&pd->app_lock[OZ_APPID_USB-1]);
<<<<<<< HEAD
	if (usb_ctx == 0)
=======
	if (usb_ctx == NULL)
>>>>>>> refs/remotes/origin/master
		return rc;
	if (usb_ctx->stopped)
		goto done;
	if (usb_ctx->hport)
		if (oz_hcd_heartbeat(usb_ctx->hport))
			rc = 1;
done:
	oz_usb_put(usb_ctx);
	return rc;
}
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> refs/remotes/origin/master
 * Context: softirq
 */
int oz_usb_stream_create(void *hpd, u8 ep_num)
{
	struct oz_usb_ctx *usb_ctx = (struct oz_usb_ctx *)hpd;
	struct oz_pd *pd = usb_ctx->pd;
<<<<<<< HEAD
	oz_trace("oz_usb_stream_create(0x%x)\n", ep_num);
=======

	oz_dbg(ON, "%s: (0x%x)\n", __func__, ep_num);
>>>>>>> refs/remotes/origin/master
	if (pd->mode & OZ_F_ISOC_NO_ELTS) {
		oz_isoc_stream_create(pd, ep_num);
	} else {
		oz_pd_get(pd);
		if (oz_elt_stream_create(&pd->elt_buff, ep_num,
			4*pd->max_tx_size)) {
			oz_pd_put(pd);
			return -1;
		}
	}
	return 0;
}
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> refs/remotes/origin/master
 * Context: softirq
 */
int oz_usb_stream_delete(void *hpd, u8 ep_num)
{
	struct oz_usb_ctx *usb_ctx = (struct oz_usb_ctx *)hpd;
<<<<<<< HEAD
	if (usb_ctx) {
		struct oz_pd *pd = usb_ctx->pd;
		if (pd) {
			oz_trace("oz_usb_stream_delete(0x%x)\n", ep_num);
=======

	if (usb_ctx) {
		struct oz_pd *pd = usb_ctx->pd;
		if (pd) {
			oz_dbg(ON, "%s: (0x%x)\n", __func__, ep_num);
>>>>>>> refs/remotes/origin/master
			if (pd->mode & OZ_F_ISOC_NO_ELTS) {
				oz_isoc_stream_delete(pd, ep_num);
			} else {
				if (oz_elt_stream_delete(&pd->elt_buff, ep_num))
					return -1;
				oz_pd_put(pd);
			}
		}
	}
	return 0;
}
<<<<<<< HEAD
/*------------------------------------------------------------------------------
=======

/*
>>>>>>> refs/remotes/origin/master
 * Context: softirq or process
 */
void oz_usb_request_heartbeat(void *hpd)
{
	struct oz_usb_ctx *usb_ctx = (struct oz_usb_ctx *)hpd;
<<<<<<< HEAD
=======

>>>>>>> refs/remotes/origin/master
	if (usb_ctx && usb_ctx->pd)
		oz_pd_request_heartbeat(usb_ctx->pd);
}
