/*
 * Enhanced Host Controller Interface (EHCI) driver for USB.
 *
 * Maintainer: Alan Stern <stern@rowland.harvard.edu>
 *
 * Copyright (c) 2000-2004 by David Brownell
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

#include <linux/module.h>
#include <linux/pci.h>
#include <linux/dmapool.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ioport.h>
#include <linux/sched.h>
#include <linux/vmalloc.h>
#include <linux/errno.h>
#include <linux/init.h>
<<<<<<< HEAD
#include <linux/timer.h>
#include <linux/ktime.h>
=======
#include <linux/hrtimer.h>
>>>>>>> refs/remotes/origin/master
#include <linux/list.h>
#include <linux/interrupt.h>
#include <linux/usb.h>
#include <linux/usb/hcd.h>
#include <linux/moduleparam.h>
#include <linux/dma-mapping.h>
#include <linux/debugfs.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <linux/uaccess.h>
=======
>>>>>>> refs/remotes/origin/master

#include <asm/byteorder.h>
#include <asm/io.h>
#include <asm/irq.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/system.h>
#include <asm/unaligned.h>

=======
=======
>>>>>>> refs/remotes/origin/master
#include <asm/unaligned.h>

#if defined(CONFIG_PPC_PS3)
#include <asm/firmware.h>
#endif

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*-------------------------------------------------------------------------*/

/*
 * EHCI hc_driver implementation ... experimental, incomplete.
 * Based on the final 1.0 register interface specification.
 *
 * USB 2.0 shows up in upcoming www.pcmcia.org technology.
 * First was PCMCIA, like ISA; then CardBus, which is PCI.
 * Next comes "CardBay", using USB 2.0 signals.
 *
 * Contains additional contributions by Brad Hards, Rory Bolt, and others.
 * Special thanks to Intel and VIA for providing host controllers to
 * test this driver on, and Cypress (including In-System Design) for
 * providing early devices for those host controllers to talk to!
 */

#define DRIVER_AUTHOR "David Brownell"
#define DRIVER_DESC "USB 2.0 'Enhanced' Host Controller (EHCI) Driver"

static const char	hcd_name [] = "ehci_hcd";


<<<<<<< HEAD
#undef VERBOSE_DEBUG
#undef EHCI_URB_TRACE

#ifdef DEBUG
#define EHCI_STATS
#endif

=======
#undef EHCI_URB_TRACE

>>>>>>> refs/remotes/origin/master
/* magic numbers that can affect system performance */
#define	EHCI_TUNE_CERR		3	/* 0-3 qtd retries; 0 == don't stop */
#define	EHCI_TUNE_RL_HS		4	/* nak throttle; see 4.9 */
#define	EHCI_TUNE_RL_TT		0
#define	EHCI_TUNE_MULT_HS	1	/* 1-3 transactions/uframe; 4.10.3 */
#define	EHCI_TUNE_MULT_TT	1
/*
 * Some drivers think it's safe to schedule isochronous transfers more than
 * 256 ms into the future (partly as a result of an old bug in the scheduling
 * code).  In an attempt to avoid trouble, we will use a minimum scheduling
 * length of 512 frames instead of 256.
 */
#define	EHCI_TUNE_FLS		1	/* (medium) 512-frame schedule */

<<<<<<< HEAD
#define EHCI_IAA_MSECS		10		/* arbitrary */
#define EHCI_IO_JIFFIES		(HZ/10)		/* io watchdog > irq_thresh */
#define EHCI_ASYNC_JIFFIES	(HZ/20)		/* async idle timeout */
#define EHCI_SHRINK_JIFFIES	(DIV_ROUND_UP(HZ, 200) + 1)
<<<<<<< HEAD
						/* 200-ms async qh unlink delay */
=======
						/* 5-ms async qh unlink delay */
>>>>>>> refs/remotes/origin/cm-10.0

=======
>>>>>>> refs/remotes/origin/master
/* Initial IRQ latency:  faster than hw default */
static int log2_irq_thresh = 0;		// 0 to 6
module_param (log2_irq_thresh, int, S_IRUGO);
MODULE_PARM_DESC (log2_irq_thresh, "log2 IRQ latency, 1-64 microframes");

/* initial park setting:  slower than hw default */
static unsigned park = 0;
module_param (park, uint, S_IRUGO);
MODULE_PARM_DESC (park, "park setting; 1-3 back-to-back async packets");

/* for flakey hardware, ignore overcurrent indicators */
<<<<<<< HEAD
<<<<<<< HEAD
static int ignore_oc = 0;
=======
static bool ignore_oc = 0;
>>>>>>> refs/remotes/origin/cm-10.0
module_param (ignore_oc, bool, S_IRUGO);
MODULE_PARM_DESC (ignore_oc, "ignore bogus hardware overcurrent indications");

/* for link power management(LPM) feature */
static unsigned int hird;
module_param(hird, int, S_IRUGO);
<<<<<<< HEAD
MODULE_PARM_DESC(hird, "host initiated resume duration, +1 for each 75us\n");
=======
MODULE_PARM_DESC(hird, "host initiated resume duration, +1 for each 75us");
>>>>>>> refs/remotes/origin/cm-10.0

=======
static bool ignore_oc = 0;
module_param (ignore_oc, bool, S_IRUGO);
MODULE_PARM_DESC (ignore_oc, "ignore bogus hardware overcurrent indications");

>>>>>>> refs/remotes/origin/master
#define	INTR_MASK (STS_IAA | STS_FATAL | STS_PCD | STS_ERR | STS_INT)

/*-------------------------------------------------------------------------*/

#include "ehci.h"
<<<<<<< HEAD
#include "ehci-dbg.c"
#include "pci-quirks.h"

/*-------------------------------------------------------------------------*/

static void
timer_action(struct ehci_hcd *ehci, enum ehci_timer_action action)
{
	/* Don't override timeouts which shrink or (later) disable
	 * the async ring; just the I/O watchdog.  Note that if a
	 * SHRINK were pending, OFF would never be requested.
	 */
	if (timer_pending(&ehci->watchdog)
			&& ((BIT(TIMER_ASYNC_SHRINK) | BIT(TIMER_ASYNC_OFF))
				& ehci->actions))
		return;

	if (!test_and_set_bit(action, &ehci->actions)) {
		unsigned long t;

		switch (action) {
		case TIMER_IO_WATCHDOG:
			if (!ehci->need_io_watchdog)
				return;
			t = EHCI_IO_JIFFIES;
			break;
		case TIMER_ASYNC_OFF:
			t = EHCI_ASYNC_JIFFIES;
			break;
		/* case TIMER_ASYNC_SHRINK: */
		default:
			t = EHCI_SHRINK_JIFFIES;
			break;
		}
		mod_timer(&ehci->watchdog, t + jiffies);
	}
}

/*-------------------------------------------------------------------------*/

/*
 * handshake - spin reading hc until handshake completes or fails
=======
#include "pci-quirks.h"

static void compute_tt_budget(u8 budget_table[EHCI_BANDWIDTH_SIZE],
		struct ehci_tt *tt);

/*
 * The MosChip MCS9990 controller updates its microframe counter
 * a little before the frame counter, and occasionally we will read
 * the invalid intermediate value.  Avoid problems by checking the
 * microframe number (the low-order 3 bits); if they are 0 then
 * re-read the register to get the correct value.
 */
static unsigned ehci_moschip_read_frame_index(struct ehci_hcd *ehci)
{
	unsigned uf;

	uf = ehci_readl(ehci, &ehci->regs->frame_index);
	if (unlikely((uf & 7) == 0))
		uf = ehci_readl(ehci, &ehci->regs->frame_index);
	return uf;
}

static inline unsigned ehci_read_frame_index(struct ehci_hcd *ehci)
{
	if (ehci->frame_index_bug)
		return ehci_moschip_read_frame_index(ehci);
	return ehci_readl(ehci, &ehci->regs->frame_index);
}

#include "ehci-dbg.c"

/*-------------------------------------------------------------------------*/

/*
 * ehci_handshake - spin reading hc until handshake completes or fails
>>>>>>> refs/remotes/origin/master
 * @ptr: address of hc register to be read
 * @mask: bits to look at in result of read
 * @done: value of those bits when handshake succeeds
 * @usec: timeout in microseconds
 *
 * Returns negative errno, or zero on success
 *
 * Success happens when the "mask" bits have the specified value (hardware
 * handshake done).  There are two failure modes:  "usec" have passed (major
 * hardware flakeout), or the register reads as all-ones (hardware removed).
 *
 * That last failure should_only happen in cases like physical cardbus eject
 * before driver shutdown. But it also seems to be caused by bugs in cardbus
 * bridge shutdown:  shutting down the bridge before the devices using it.
 */
<<<<<<< HEAD
static int handshake (struct ehci_hcd *ehci, void __iomem *ptr,
		      u32 mask, u32 done, int usec)
=======
int ehci_handshake(struct ehci_hcd *ehci, void __iomem *ptr,
		   u32 mask, u32 done, int usec)
>>>>>>> refs/remotes/origin/master
{
	u32	result;

	do {
		result = ehci_readl(ehci, ptr);
		if (result == ~(u32)0)		/* card removed */
			return -ENODEV;
		result &= mask;
		if (result == done)
			return 0;
		udelay (1);
		usec--;
	} while (usec > 0);
	return -ETIMEDOUT;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(ehci_handshake);
>>>>>>> refs/remotes/origin/master

/* check TDI/ARC silicon is in host mode */
static int tdi_in_host_mode (struct ehci_hcd *ehci)
{
<<<<<<< HEAD
	u32 __iomem	*reg_ptr;
	u32		tmp;

	reg_ptr = (u32 __iomem *)(((u8 __iomem *)ehci->regs) + USBMODE);
	tmp = ehci_readl(ehci, reg_ptr);
	return (tmp & 3) == USBMODE_CM_HC;
}

/* force HC to halt state from unknown (EHCI spec section 2.3) */
static int ehci_halt (struct ehci_hcd *ehci)
{
	u32	temp = ehci_readl(ehci, &ehci->regs->status);
=======
	u32		tmp;

	tmp = ehci_readl(ehci, &ehci->regs->usbmode);
	return (tmp & 3) == USBMODE_CM_HC;
}

/*
 * Force HC to halt state from unknown (EHCI spec section 2.3).
 * Must be called with interrupts enabled and the lock not held.
 */
static int ehci_halt (struct ehci_hcd *ehci)
{
	u32	temp;

	spin_lock_irq(&ehci->lock);
>>>>>>> refs/remotes/origin/master

	/* disable any irqs left enabled by previous code */
	ehci_writel(ehci, 0, &ehci->regs->intr_enable);

<<<<<<< HEAD
	if (ehci_is_TDI(ehci) && tdi_in_host_mode(ehci) == 0) {
		return 0;
	}

	if ((temp & STS_HALT) != 0)
		return 0;

	temp = ehci_readl(ehci, &ehci->regs->command);
	temp &= ~CMD_RUN;
	ehci_writel(ehci, temp, &ehci->regs->command);
	return handshake (ehci, &ehci->regs->status,
			  STS_HALT, STS_HALT, 16 * 125);
}

<<<<<<< HEAD
=======
#if defined(CONFIG_USB_SUSPEND) && defined(CONFIG_PPC_PS3)

/*
 * The EHCI controller of the Cell Super Companion Chip used in the
 * PS3 will stop the root hub after all root hub ports are suspended.
 * When in this condition handshake will return -ETIMEDOUT.  The
 * STS_HLT bit will not be set, so inspection of the frame index is
 * used here to test for the condition.  If the condition is found
 * return success to allow the USB suspend to complete.
 */

static int handshake_for_broken_root_hub(struct ehci_hcd *ehci,
					 void __iomem *ptr, u32 mask, u32 done,
					 int usec)
{
	unsigned int old_index;
	int error;

	if (!firmware_has_feature(FW_FEATURE_PS3_LV1))
		return -ETIMEDOUT;

	old_index = ehci_read_frame_index(ehci);

	error = handshake(ehci, ptr, mask, done, usec);

	if (error == -ETIMEDOUT && ehci_read_frame_index(ehci) == old_index)
		return 0;

	return error;
}

#else

static int handshake_for_broken_root_hub(struct ehci_hcd *ehci,
					 void __iomem *ptr, u32 mask, u32 done,
					 int usec)
{
	return -ETIMEDOUT;
}

#endif

>>>>>>> refs/remotes/origin/cm-10.0
static int handshake_on_error_set_halt(struct ehci_hcd *ehci, void __iomem *ptr,
				       u32 mask, u32 done, int usec)
{
	int error;

	error = handshake(ehci, ptr, mask, done, usec);
<<<<<<< HEAD
	if (error) {
		ehci_halt(ehci);
		ehci_to_hcd(ehci)->state = HC_STATE_HALT;
=======
	if (error == -ETIMEDOUT)
		error = handshake_for_broken_root_hub(ehci, ptr, mask, done,
						      usec);

	if (error) {
		ehci_halt(ehci);
		ehci->rh_state = EHCI_RH_HALTED;
>>>>>>> refs/remotes/origin/cm-10.0
		ehci_err(ehci, "force halt; handshake %p %08x %08x -> %d\n",
			ptr, mask, done, error);
	}

	return error;
=======
	if (ehci_is_TDI(ehci) && !tdi_in_host_mode(ehci)) {
		spin_unlock_irq(&ehci->lock);
		return 0;
	}

	/*
	 * This routine gets called during probe before ehci->command
	 * has been initialized, so we can't rely on its value.
	 */
	ehci->command &= ~CMD_RUN;
	temp = ehci_readl(ehci, &ehci->regs->command);
	temp &= ~(CMD_RUN | CMD_IAAD);
	ehci_writel(ehci, temp, &ehci->regs->command);

	spin_unlock_irq(&ehci->lock);
	synchronize_irq(ehci_to_hcd(ehci)->irq);

	return ehci_handshake(ehci, &ehci->regs->status,
			  STS_HALT, STS_HALT, 16 * 125);
>>>>>>> refs/remotes/origin/master
}

/* put TDI/ARC silicon into EHCI mode */
static void tdi_reset (struct ehci_hcd *ehci)
{
<<<<<<< HEAD
	u32 __iomem	*reg_ptr;
	u32		tmp;

	reg_ptr = (u32 __iomem *)(((u8 __iomem *)ehci->regs) + USBMODE);
	tmp = ehci_readl(ehci, reg_ptr);
=======
	u32		tmp;

	tmp = ehci_readl(ehci, &ehci->regs->usbmode);
>>>>>>> refs/remotes/origin/master
	tmp |= USBMODE_CM_HC;
	/* The default byte access to MMR space is LE after
	 * controller reset. Set the required endian mode
	 * for transfer buffers to match the host microprocessor
	 */
	if (ehci_big_endian_mmio(ehci))
		tmp |= USBMODE_BE;
<<<<<<< HEAD
	ehci_writel(ehci, tmp, reg_ptr);
}

/* reset a non-running (STS_HALT == 1) controller */
=======
	ehci_writel(ehci, tmp, &ehci->regs->usbmode);
}

/*
 * Reset a non-running (STS_HALT == 1) controller.
 * Must be called with interrupts enabled and the lock not held.
 */
>>>>>>> refs/remotes/origin/master
static int ehci_reset (struct ehci_hcd *ehci)
{
	int	retval;
	u32	command = ehci_readl(ehci, &ehci->regs->command);

	/* If the EHCI debug controller is active, special care must be
	 * taken before and after a host controller reset */
<<<<<<< HEAD
	if (ehci->debug && !dbgp_reset_prep())
=======
	if (ehci->debug && !dbgp_reset_prep(ehci_to_hcd(ehci)))
>>>>>>> refs/remotes/origin/master
		ehci->debug = NULL;

	command |= CMD_RESET;
	dbg_cmd (ehci, "reset", command);
	ehci_writel(ehci, command, &ehci->regs->command);
<<<<<<< HEAD
<<<<<<< HEAD
	ehci_to_hcd(ehci)->state = HC_STATE_HALT;
=======
	ehci->rh_state = EHCI_RH_HALTED;
>>>>>>> refs/remotes/origin/cm-10.0
	ehci->next_statechange = jiffies;
	retval = handshake (ehci, &ehci->regs->command,
=======
	ehci->rh_state = EHCI_RH_HALTED;
	ehci->next_statechange = jiffies;
	retval = ehci_handshake(ehci, &ehci->regs->command,
>>>>>>> refs/remotes/origin/master
			    CMD_RESET, 0, 250 * 1000);

	if (ehci->has_hostpc) {
		ehci_writel(ehci, USBMODE_EX_HC | USBMODE_EX_VBPS,
<<<<<<< HEAD
			(u32 __iomem *)(((u8 *)ehci->regs) + USBMODE_EX));
		ehci_writel(ehci, TXFIFO_DEFAULT,
			(u32 __iomem *)(((u8 *)ehci->regs) + TXFILLTUNING));
=======
				&ehci->regs->usbmode_ex);
		ehci_writel(ehci, TXFIFO_DEFAULT, &ehci->regs->txfill_tuning);
>>>>>>> refs/remotes/origin/master
	}
	if (retval)
		return retval;

	if (ehci_is_TDI(ehci))
		tdi_reset (ehci);

	if (ehci->debug)
<<<<<<< HEAD
		dbgp_external_startup();

<<<<<<< HEAD
=======
	ehci->port_c_suspend = ehci->suspended_ports =
			ehci->resuming_ports = 0;
>>>>>>> refs/remotes/origin/cm-10.0
	return retval;
}

/* idle the controller (from running) */
=======
		dbgp_external_startup(ehci_to_hcd(ehci));

	ehci->port_c_suspend = ehci->suspended_ports =
			ehci->resuming_ports = 0;
	return retval;
}

/*
 * Idle the controller (turn off the schedules).
 * Must be called with interrupts enabled and the lock not held.
 */
>>>>>>> refs/remotes/origin/master
static void ehci_quiesce (struct ehci_hcd *ehci)
{
	u32	temp;

<<<<<<< HEAD
#ifdef DEBUG
<<<<<<< HEAD
	if (!HC_IS_RUNNING (ehci_to_hcd(ehci)->state))
=======
	if (ehci->rh_state != EHCI_RH_RUNNING)
>>>>>>> refs/remotes/origin/cm-10.0
		BUG ();
#endif

	/* wait for any schedule enables/disables to take effect */
	temp = ehci_readl(ehci, &ehci->regs->command) << 10;
	temp &= STS_ASS | STS_PSS;
	if (handshake_on_error_set_halt(ehci, &ehci->regs->status,
					STS_ASS | STS_PSS, temp, 16 * 125))
		return;

	/* then disable anything that's still active */
	temp = ehci_readl(ehci, &ehci->regs->command);
	temp &= ~(CMD_ASE | CMD_IAAD | CMD_PSE);
	ehci_writel(ehci, temp, &ehci->regs->command);

	/* hardware can take 16 microframes to turn off ... */
	handshake_on_error_set_halt(ehci, &ehci->regs->status,
				    STS_ASS | STS_PSS, 0, 16 * 125);
=======
	if (ehci->rh_state != EHCI_RH_RUNNING)
		return;

	/* wait for any schedule enables/disables to take effect */
	temp = (ehci->command << 10) & (STS_ASS | STS_PSS);
	ehci_handshake(ehci, &ehci->regs->status, STS_ASS | STS_PSS, temp,
			16 * 125);

	/* then disable anything that's still active */
	spin_lock_irq(&ehci->lock);
	ehci->command &= ~(CMD_ASE | CMD_PSE);
	ehci_writel(ehci, ehci->command, &ehci->regs->command);
	spin_unlock_irq(&ehci->lock);

	/* hardware can take 16 microframes to turn off ... */
	ehci_handshake(ehci, &ehci->regs->status, STS_ASS | STS_PSS, 0,
			16 * 125);
>>>>>>> refs/remotes/origin/master
}

/*-------------------------------------------------------------------------*/

static void end_unlink_async(struct ehci_hcd *ehci);
<<<<<<< HEAD
static void ehci_work(struct ehci_hcd *ehci);

#include "ehci-hub.c"
#include "ehci-lpm.c"
#include "ehci-mem.c"
#include "ehci-q.c"
#include "ehci-sched.c"
<<<<<<< HEAD
=======
#include "ehci-sysfs.c"
>>>>>>> refs/remotes/origin/cm-10.0

/*-------------------------------------------------------------------------*/

static void ehci_iaa_watchdog(unsigned long param)
{
	struct ehci_hcd		*ehci = (struct ehci_hcd *) param;
	unsigned long		flags;

	spin_lock_irqsave (&ehci->lock, flags);

	/* Lost IAA irqs wedge things badly; seen first with a vt8235.
	 * So we need this watchdog, but must protect it against both
	 * (a) SMP races against real IAA firing and retriggering, and
	 * (b) clean HC shutdown, when IAA watchdog was pending.
	 */
	if (ehci->reclaim
			&& !timer_pending(&ehci->iaa_watchdog)
<<<<<<< HEAD
			&& HC_IS_RUNNING(ehci_to_hcd(ehci)->state)) {
=======
			&& ehci->rh_state == EHCI_RH_RUNNING) {
>>>>>>> refs/remotes/origin/cm-10.0
		u32 cmd, status;

		/* If we get here, IAA is *REALLY* late.  It's barely
		 * conceivable that the system is so busy that CMD_IAAD
		 * is still legitimately set, so let's be sure it's
		 * clear before we read STS_IAA.  (The HC should clear
		 * CMD_IAAD when it sets STS_IAA.)
		 */
		cmd = ehci_readl(ehci, &ehci->regs->command);
		if (cmd & CMD_IAAD)
			ehci_writel(ehci, cmd & ~CMD_IAAD,
					&ehci->regs->command);

		/* If IAA is set here it either legitimately triggered
		 * before we cleared IAAD above (but _way_ late, so we'll
		 * still count it as lost) ... or a silicon erratum:
		 * - VIA seems to set IAA without triggering the IRQ;
		 * - IAAD potentially cleared without setting IAA.
		 */
		status = ehci_readl(ehci, &ehci->regs->status);
		if ((status & STS_IAA) || !(cmd & CMD_IAAD)) {
			COUNT (ehci->stats.lost_iaa);
			ehci_writel(ehci, STS_IAA, &ehci->regs->status);
		}

		ehci_vdbg(ehci, "IAA watchdog: status %x cmd %x\n",
				status, cmd);
		end_unlink_async(ehci);
	}

	spin_unlock_irqrestore(&ehci->lock, flags);
}

static void ehci_watchdog(unsigned long param)
{
	struct ehci_hcd		*ehci = (struct ehci_hcd *) param;
	unsigned long		flags;

	spin_lock_irqsave(&ehci->lock, flags);

	/* stop async processing after it's idled a bit */
	if (test_bit (TIMER_ASYNC_OFF, &ehci->actions))
		start_unlink_async (ehci, ehci->async);

	/* ehci could run by timer, without IRQs ... */
	ehci_work (ehci);

	spin_unlock_irqrestore (&ehci->lock, flags);
}

=======
static void unlink_empty_async(struct ehci_hcd *ehci);
static void unlink_empty_async_suspended(struct ehci_hcd *ehci);
static void ehci_work(struct ehci_hcd *ehci);
static void start_unlink_intr(struct ehci_hcd *ehci, struct ehci_qh *qh);
static void end_unlink_intr(struct ehci_hcd *ehci, struct ehci_qh *qh);

#include "ehci-timer.c"
#include "ehci-hub.c"
#include "ehci-mem.c"
#include "ehci-q.c"
#include "ehci-sched.c"
#include "ehci-sysfs.c"

/*-------------------------------------------------------------------------*/

>>>>>>> refs/remotes/origin/master
/* On some systems, leaving remote wakeup enabled prevents system shutdown.
 * The firmware seems to think that powering off is a wakeup event!
 * This routine turns off remote wakeup and everything else, on all ports.
 */
static void ehci_turn_off_all_ports(struct ehci_hcd *ehci)
{
	int	port = HCS_N_PORTS(ehci->hcs_params);

	while (port--)
		ehci_writel(ehci, PORT_RWC_BITS,
				&ehci->regs->port_status[port]);
}

/*
 * Halt HC, turn off all ports, and let the BIOS use the companion controllers.
<<<<<<< HEAD
 * Should be called with ehci->lock held.
=======
 * Must be called with interrupts enabled and the lock not held.
>>>>>>> refs/remotes/origin/master
 */
static void ehci_silence_controller(struct ehci_hcd *ehci)
{
	ehci_halt(ehci);
<<<<<<< HEAD
=======

	spin_lock_irq(&ehci->lock);
	ehci->rh_state = EHCI_RH_HALTED;
>>>>>>> refs/remotes/origin/master
	ehci_turn_off_all_ports(ehci);

	/* make BIOS/etc use companion controller during reboot */
	ehci_writel(ehci, 0, &ehci->regs->configured_flag);

	/* unblock posted writes */
	ehci_readl(ehci, &ehci->regs->configured_flag);
<<<<<<< HEAD
=======
	spin_unlock_irq(&ehci->lock);
>>>>>>> refs/remotes/origin/master
}

/* ehci_shutdown kick in for silicon on any bus (not just pci, etc).
 * This forcibly disables dma and IRQs, helping kexec and other cases
 * where the next system software may expect clean state.
 */
static void ehci_shutdown(struct usb_hcd *hcd)
{
	struct ehci_hcd	*ehci = hcd_to_ehci(hcd);

<<<<<<< HEAD
	del_timer_sync(&ehci->watchdog);
	del_timer_sync(&ehci->iaa_watchdog);

	spin_lock_irq(&ehci->lock);
	ehci_silence_controller(ehci);
	spin_unlock_irq(&ehci->lock);
}

static void __maybe_unused ehci_port_power (struct ehci_hcd *ehci, int is_on)
{
	unsigned port;

	if (!HCS_PPC (ehci->hcs_params))
		return;

	ehci_dbg (ehci, "...power%s ports...\n", is_on ? "up" : "down");
	for (port = HCS_N_PORTS (ehci->hcs_params); port > 0; )
		(void) ehci_hub_control(ehci_to_hcd(ehci),
				is_on ? SetPortFeature : ClearPortFeature,
				USB_PORT_FEAT_POWER,
				port--, NULL, 0);
	/* Flush those writes */
	ehci_readl(ehci, &ehci->regs->command);
	msleep(20);
=======
	spin_lock_irq(&ehci->lock);
	ehci->shutdown = true;
	ehci->rh_state = EHCI_RH_STOPPING;
	ehci->enabled_hrtimer_events = 0;
	spin_unlock_irq(&ehci->lock);

	ehci_silence_controller(ehci);

	hrtimer_cancel(&ehci->hrtimer);
>>>>>>> refs/remotes/origin/master
}

/*-------------------------------------------------------------------------*/

/*
 * ehci_work is called from some interrupts, timers, and so on.
 * it calls driver completion functions, after dropping ehci->lock.
 */
static void ehci_work (struct ehci_hcd *ehci)
{
<<<<<<< HEAD
	timer_action_done (ehci, TIMER_IO_WATCHDOG);

=======
>>>>>>> refs/remotes/origin/master
	/* another CPU may drop ehci->lock during a schedule scan while
	 * it reports urb completions.  this flag guards against bogus
	 * attempts at re-entrant schedule scanning.
	 */
<<<<<<< HEAD
	if (ehci->scanning)
		return;
	ehci->scanning = 1;
	scan_async (ehci);
	if (ehci->next_uframe != -1)
		scan_periodic (ehci);
	ehci->scanning = 0;
=======
	if (ehci->scanning) {
		ehci->need_rescan = true;
		return;
	}
	ehci->scanning = true;

 rescan:
	ehci->need_rescan = false;
	if (ehci->async_count)
		scan_async(ehci);
	if (ehci->intr_count > 0)
		scan_intr(ehci);
	if (ehci->isoc_count > 0)
		scan_isoc(ehci);
	if (ehci->need_rescan)
		goto rescan;
	ehci->scanning = false;
>>>>>>> refs/remotes/origin/master

	/* the IO watchdog guards against hardware or driver bugs that
	 * misplace IRQs, and should let us run completely without IRQs.
	 * such lossage has been observed on both VT6202 and VT8235.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (HC_IS_RUNNING (ehci_to_hcd(ehci)->state) &&
=======
	if (ehci->rh_state == EHCI_RH_RUNNING &&
>>>>>>> refs/remotes/origin/cm-10.0
			(ehci->async->qh_next.ptr != NULL ||
			 ehci->periodic_sched != 0))
		timer_action (ehci, TIMER_IO_WATCHDOG);
=======
	turn_on_io_watchdog(ehci);
>>>>>>> refs/remotes/origin/master
}

/*
 * Called when the ehci_hcd module is removed.
 */
static void ehci_stop (struct usb_hcd *hcd)
{
	struct ehci_hcd		*ehci = hcd_to_ehci (hcd);

	ehci_dbg (ehci, "stop\n");

	/* no more interrupts ... */
<<<<<<< HEAD
	del_timer_sync (&ehci->watchdog);
	del_timer_sync(&ehci->iaa_watchdog);

	spin_lock_irq(&ehci->lock);
<<<<<<< HEAD
	if (HC_IS_RUNNING (hcd->state))
=======
	if (ehci->rh_state == EHCI_RH_RUNNING)
>>>>>>> refs/remotes/origin/cm-10.0
		ehci_quiesce (ehci);

	ehci_silence_controller(ehci);
	ehci_reset (ehci);
	spin_unlock_irq(&ehci->lock);

<<<<<<< HEAD
	remove_companion_file(ehci);
=======
	remove_sysfs_files(ehci);
>>>>>>> refs/remotes/origin/cm-10.0
=======

	spin_lock_irq(&ehci->lock);
	ehci->enabled_hrtimer_events = 0;
	spin_unlock_irq(&ehci->lock);

	ehci_quiesce(ehci);
	ehci_silence_controller(ehci);
	ehci_reset (ehci);

	hrtimer_cancel(&ehci->hrtimer);
	remove_sysfs_files(ehci);
>>>>>>> refs/remotes/origin/master
	remove_debug_files (ehci);

	/* root hub is shut down separately (first, when possible) */
	spin_lock_irq (&ehci->lock);
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (ehci->async) {
		/*
		 * TODO: Observed that ehci->async next ptr is not
		 * NULL sometimes which leads to crash in mem_cleanup.
		 * Root cause is not yet known why this messup is
		 * happenning.
		 * The follwing workaround fixes the crash caused
		 * by this temporarily.
		 * check if async next ptr is not NULL and unlink
		 * explictly.
		 */
		if (ehci->async->qh_next.ptr != NULL)
			start_unlink_async(ehci, ehci->async->qh_next.qh);
		ehci_work (ehci);
	}
<<<<<<< HEAD
=======
	end_free_itds(ehci);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	spin_unlock_irq (&ehci->lock);
	ehci_mem_cleanup (ehci);

	if (ehci->amd_pll_fix == 1)
		usb_amd_dev_put();

<<<<<<< HEAD
#ifdef	EHCI_STATS
	ehci_dbg (ehci, "irq normal %ld err %ld reclaim %ld (lost %ld)\n",
		ehci->stats.normal, ehci->stats.error, ehci->stats.reclaim,
		ehci->stats.lost_iaa);
	ehci_dbg (ehci, "complete %ld unlink %ld\n",
		ehci->stats.complete, ehci->stats.unlink);
#endif

=======
>>>>>>> refs/remotes/origin/master
	dbg_status (ehci, "ehci_stop completed",
		    ehci_readl(ehci, &ehci->regs->status));
}

/* one-time init, only for memory state */
static int ehci_init(struct usb_hcd *hcd)
{
	struct ehci_hcd		*ehci = hcd_to_ehci(hcd);
	u32			temp;
	int			retval;
	u32			hcc_params;
	struct ehci_qh_hw	*hw;

	spin_lock_init(&ehci->lock);

	/*
	 * keep io watchdog by default, those good HCDs could turn off it later
	 */
	ehci->need_io_watchdog = 1;
<<<<<<< HEAD
	init_timer(&ehci->watchdog);
	ehci->watchdog.function = ehci_watchdog;
	ehci->watchdog.data = (unsigned long) ehci;

	init_timer(&ehci->iaa_watchdog);
	ehci->iaa_watchdog.function = ehci_iaa_watchdog;
	ehci->iaa_watchdog.data = (unsigned long) ehci;
=======

	hrtimer_init(&ehci->hrtimer, CLOCK_MONOTONIC, HRTIMER_MODE_ABS);
	ehci->hrtimer.function = ehci_hrtimer_func;
	ehci->next_hrtimer_event = EHCI_HRTIMER_NO_EVENT;
>>>>>>> refs/remotes/origin/master

	hcc_params = ehci_readl(ehci, &ehci->caps->hcc_params);

	/*
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	 * by default set standard 80% (== 100 usec/uframe) max periodic
	 * bandwidth as required by USB 2.0
	 */
	ehci->uframe_periodic_max = 100;

	/*
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	 * hw default: 1K periodic list heads, one per frame.
	 * periodic_size can shrink by USBCMD update if hcc_params allows.
	 */
	ehci->periodic_size = DEFAULT_I_TDPS;
<<<<<<< HEAD
	INIT_LIST_HEAD(&ehci->cached_itd_list);
	INIT_LIST_HEAD(&ehci->cached_sitd_list);
=======
	INIT_LIST_HEAD(&ehci->async_unlink);
	INIT_LIST_HEAD(&ehci->async_idle);
	INIT_LIST_HEAD(&ehci->intr_unlink_wait);
	INIT_LIST_HEAD(&ehci->intr_unlink);
	INIT_LIST_HEAD(&ehci->intr_qh_list);
	INIT_LIST_HEAD(&ehci->cached_itd_list);
	INIT_LIST_HEAD(&ehci->cached_sitd_list);
	INIT_LIST_HEAD(&ehci->tt_list);
>>>>>>> refs/remotes/origin/master

	if (HCC_PGM_FRAMELISTLEN(hcc_params)) {
		/* periodic schedule size can be smaller than default */
		switch (EHCI_TUNE_FLS) {
		case 0: ehci->periodic_size = 1024; break;
		case 1: ehci->periodic_size = 512; break;
		case 2: ehci->periodic_size = 256; break;
		default:	BUG();
		}
	}
	if ((retval = ehci_mem_init(ehci, GFP_KERNEL)) < 0)
		return retval;

	/* controllers may cache some of the periodic schedule ... */
	if (HCC_ISOC_CACHE(hcc_params))		// full frame cache
<<<<<<< HEAD
		ehci->i_thresh = 2 + 8;
	else					// N microframes cached
		ehci->i_thresh = 2 + HCC_ISOC_THRES(hcc_params);

	ehci->reclaim = NULL;
	ehci->next_uframe = -1;
	ehci->clock_frame = -1;

=======
		ehci->i_thresh = 0;
	else					// N microframes cached
		ehci->i_thresh = 2 + HCC_ISOC_THRES(hcc_params);

>>>>>>> refs/remotes/origin/master
	/*
	 * dedicate a qh for the async ring head, since we couldn't unlink
	 * a 'real' qh without stopping the async schedule [4.8].  use it
	 * as the 'reclamation list head' too.
	 * its dummy is used in hw_alt_next of many tds, to prevent the qh
	 * from automatically advancing to the next td after short reads.
	 */
	ehci->async->qh_next.qh = NULL;
	hw = ehci->async->hw;
	hw->hw_next = QH_NEXT(ehci, ehci->async->qh_dma);
	hw->hw_info1 = cpu_to_hc32(ehci, QH_HEAD);
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
#if defined(CONFIG_PPC_PS3)
	hw->hw_info1 |= cpu_to_hc32(ehci, (1 << 7));	/* I = 1 */
#endif
>>>>>>> refs/remotes/origin/cm-10.0
=======
#if defined(CONFIG_PPC_PS3)
	hw->hw_info1 |= cpu_to_hc32(ehci, QH_INACTIVATE);
#endif
>>>>>>> refs/remotes/origin/master
=======
#if defined(CONFIG_PPC_PS3)
	hw->hw_info1 |= cpu_to_hc32(ehci, (1 << 7));	/* I = 1 */
#endif
>>>>>>> refs/remotes/origin/cm-11.0
	hw->hw_token = cpu_to_hc32(ehci, QTD_STS_HALT);
	hw->hw_qtd_next = EHCI_LIST_END(ehci);
	ehci->async->qh_state = QH_STATE_LINKED;
	hw->hw_alt_next = QTD_NEXT(ehci, ehci->async->dummy->qtd_dma);

	/* clear interrupt enables, set irq latency */
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
	log2_irq_thresh = ehci->log2_irq_thresh;

>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
	log2_irq_thresh = ehci->log2_irq_thresh;

>>>>>>> refs/remotes/origin/cm-11.0
	if (log2_irq_thresh < 0 || log2_irq_thresh > 6)
		log2_irq_thresh = 0;
	temp = 1 << (16 + log2_irq_thresh);
	if (HCC_PER_PORT_CHANGE_EVENT(hcc_params)) {
		ehci->has_ppcd = 1;
		ehci_dbg(ehci, "enable per-port change event\n");
		temp |= CMD_PPCEE;
	}
	if (HCC_CANPARK(hcc_params)) {
		/* HW default park == 3, on hardware that supports it (like
		 * NVidia and ALI silicon), maximizes throughput on the async
		 * schedule by avoiding QH fetches between transfers.
		 *
		 * With fast usb storage devices and NForce2, "park" seems to
		 * make problems:  throughput reduction (!), data errors...
		 */
		if (park) {
			park = min(park, (unsigned) 3);
			temp |= CMD_PARK;
			temp |= park << 8;
		}
		ehci_dbg(ehci, "park %d\n", park);
	}
	if (HCC_PGM_FRAMELISTLEN(hcc_params)) {
		/* periodic schedule size can be smaller than default */
		temp &= ~(3 << 2);
		temp |= (EHCI_TUNE_FLS << 2);
	}
<<<<<<< HEAD
	if (HCC_LPM(hcc_params)) {
		/* support link power management EHCI 1.1 addendum */
		ehci_dbg(ehci, "support lpm\n");
		ehci->has_lpm = 1;
		if (hird > 0xf) {
			ehci_dbg(ehci, "hird %d invalid, use default 0",
			hird);
			hird = 0;
		}
		temp |= hird << 24;
	}
=======
>>>>>>> refs/remotes/origin/master
	ehci->command = temp;

	/* Accept arbitrarily long scatter-gather lists */
	if (!(hcd->driver->flags & HCD_LOCAL_MEM))
		hcd->self.sg_tablesize = ~0;
	return 0;
}

/* start HC running; it's halted, ehci_init() has been run (once) */
<<<<<<< HEAD
<<<<<<< HEAD
static int __maybe_unused ehci_run (struct usb_hcd *hcd)
{
	struct ehci_hcd		*ehci = hcd_to_ehci (hcd);
<<<<<<< HEAD
	int			retval;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int ehci_run (struct usb_hcd *hcd)
=======
static int __maybe_unused ehci_run (struct usb_hcd *hcd)
>>>>>>> refs/remotes/origin/cm-11.0
{
	struct ehci_hcd		*ehci = hcd_to_ehci (hcd);
>>>>>>> refs/remotes/origin/master
	u32			temp;
	u32			hcc_params;

	hcd->uses_new_polling = 1;

	/* EHCI spec section 4.1 */
<<<<<<< HEAD
<<<<<<< HEAD
	/*
	 * TDI driver does the ehci_reset in their reset callback.
	 * Don't reset here, because configuration settings will
	 * vanish.
	 */
	if (!ehci_is_TDI(ehci) && (retval = ehci_reset(ehci)) != 0) {
		ehci_mem_cleanup(ehci);
		return retval;
	}
=======

>>>>>>> refs/remotes/origin/cm-10.0
=======

>>>>>>> refs/remotes/origin/master
	ehci_writel(ehci, ehci->periodic_dma, &ehci->regs->frame_list);
	ehci_writel(ehci, (u32)ehci->async->qh_dma, &ehci->regs->async_next);

	/*
	 * hcc_params controls whether ehci->regs->segment must (!!!)
	 * be used; it constrains QH/ITD/SITD and QTD locations.
	 * pci_pool consistent memory always uses segment zero.
	 * streaming mappings for I/O buffers, like pci_map_single(),
	 * can return segments above 4GB, if the device allows.
	 *
	 * NOTE:  the dma mask is visible through dma_supported(), so
	 * drivers can pass this info along ... like NETIF_F_HIGHDMA,
	 * Scsi_Host.highmem_io, and so forth.  It's readonly to all
	 * host side drivers though.
	 */
	hcc_params = ehci_readl(ehci, &ehci->caps->hcc_params);
	if (HCC_64BIT_ADDR(hcc_params)) {
		ehci_writel(ehci, 0, &ehci->regs->segment);
#if 0
// this is deeply broken on almost all architectures
		if (!dma_set_mask(hcd->self.controller, DMA_BIT_MASK(64)))
			ehci_info(ehci, "enabled 64bit DMA\n");
#endif
	}


	// Philips, Intel, and maybe others need CMD_RUN before the
	// root hub will detect new devices (why?); NEC doesn't
	ehci->command &= ~(CMD_LRESET|CMD_IAAD|CMD_PSE|CMD_ASE|CMD_RESET);
	ehci->command |= CMD_RUN;
	ehci_writel(ehci, ehci->command, &ehci->regs->command);
	dbg_cmd (ehci, "init", ehci->command);

	/*
	 * Start, enabling full USB 2.0 functionality ... usb 1.1 devices
	 * are explicitly handed to companion controller(s), so no TT is
	 * involved with the root hub.  (Except where one is integrated,
	 * and there's no companion controller unless maybe for USB OTG.)
	 *
	 * Turning on the CF flag will transfer ownership of all ports
	 * from the companions to the EHCI controller.  If any of the
	 * companions are in the middle of a port reset at the time, it
	 * could cause trouble.  Write-locking ehci_cf_port_reset_rwsem
	 * guarantees that no resets are in progress.  After we set CF,
	 * a short delay lets the hardware catch up; new resets shouldn't
	 * be started before the port switching actions could complete.
	 */
	down_write(&ehci_cf_port_reset_rwsem);
<<<<<<< HEAD
<<<<<<< HEAD
	hcd->state = HC_STATE_RUNNING;
=======
	ehci->rh_state = EHCI_RH_RUNNING;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ehci->rh_state = EHCI_RH_RUNNING;
>>>>>>> refs/remotes/origin/master
	ehci_writel(ehci, FLAG_CF, &ehci->regs->configured_flag);
	ehci_readl(ehci, &ehci->regs->command);	/* unblock posted writes */
	msleep(5);
	up_write(&ehci_cf_port_reset_rwsem);
	ehci->last_periodic_enable = ktime_get_real();

	temp = HC_VERSION(ehci, ehci_readl(ehci, &ehci->caps->hc_capbase));
	ehci_info (ehci,
		"USB %x.%x started, EHCI %x.%02x%s\n",
		((ehci->sbrn & 0xf0)>>4), (ehci->sbrn & 0x0f),
		temp >> 8, temp & 0xff,
		ignore_oc ? ", overcurrent ignored" : "");

	ehci_writel(ehci, INTR_MASK,
		    &ehci->regs->intr_enable); /* Turn On Interrupts */

	/* GRR this is run-once init(), being done every time the HC starts.
	 * So long as they're part of class devices, we can't do it init()
	 * since the class device isn't created that early.
	 */
	create_debug_files(ehci);
<<<<<<< HEAD
<<<<<<< HEAD
	create_companion_file(ehci);
=======
	create_sysfs_files(ehci);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	create_sysfs_files(ehci);
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
static int __maybe_unused ehci_setup (struct usb_hcd *hcd)
=======
int ehci_setup(struct usb_hcd *hcd)
>>>>>>> refs/remotes/origin/master
{
	struct ehci_hcd *ehci = hcd_to_ehci(hcd);
	int retval;

	ehci->regs = (void __iomem *)ehci->caps +
	    HC_LENGTH(ehci, ehci_readl(ehci, &ehci->caps->hc_capbase));
	dbg_hcs_params(ehci, "reset");
	dbg_hcc_params(ehci, "reset");

	/* cache this readonly data; minimize chip reads */
	ehci->hcs_params = ehci_readl(ehci, &ehci->caps->hcs_params);

	ehci->sbrn = HCD_USB2;

<<<<<<< HEAD
	retval = ehci_halt(ehci);
	if (retval)
		return retval;

	/* data structure init */
	retval = ehci_init(hcd);
=======
	/* data structure init */
	retval = ehci_init(hcd);
	if (retval)
		return retval;

	retval = ehci_halt(ehci);
>>>>>>> refs/remotes/origin/master
	if (retval)
		return retval;

	ehci_reset(ehci);

	return 0;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(ehci_setup);
>>>>>>> refs/remotes/origin/master

/*-------------------------------------------------------------------------*/

static irqreturn_t ehci_irq (struct usb_hcd *hcd)
{
	struct ehci_hcd		*ehci = hcd_to_ehci (hcd);
	u32			status, masked_status, pcd_status = 0, cmd;
	int			bh;

	spin_lock (&ehci->lock);

	status = ehci_readl(ehci, &ehci->regs->status);

	/* e.g. cardbus physical eject */
	if (status == ~(u32) 0) {
		ehci_dbg (ehci, "device removed\n");
		goto dead;
	}

	/*
	 * We don't use STS_FLR, but some controllers don't like it to
	 * remain on, so mask it out along with the other status bits.
	 */
	masked_status = status & (INTR_MASK | STS_FLR);

	/* Shared IRQ? */
<<<<<<< HEAD
<<<<<<< HEAD
	if (!masked_status || unlikely(hcd->state == HC_STATE_HALT)) {
=======
	if (!masked_status || unlikely(ehci->rh_state == EHCI_RH_HALTED)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!masked_status || unlikely(ehci->rh_state == EHCI_RH_HALTED)) {
>>>>>>> refs/remotes/origin/master
		spin_unlock(&ehci->lock);
		return IRQ_NONE;
	}

	/* clear (just) interrupts */
	ehci_writel(ehci, masked_status, &ehci->regs->status);
	cmd = ehci_readl(ehci, &ehci->regs->command);
	bh = 0;

<<<<<<< HEAD
#ifdef	VERBOSE_DEBUG
	/* unrequested/ignored: Frame List Rollover */
	dbg_status (ehci, "irq", status);
#endif

	/* INT, ERR, and IAA interrupt rates can be throttled */

=======
>>>>>>> refs/remotes/origin/master
	/* normal [4.15.1.2] or error [4.15.1.1] completion */
	if (likely ((status & (STS_INT|STS_ERR)) != 0)) {
		if (likely ((status & STS_ERR) == 0))
			COUNT (ehci->stats.normal);
		else
			COUNT (ehci->stats.error);
		bh = 1;
	}

	/* complete the unlinking of some qh [4.15.2.3] */
	if (status & STS_IAA) {
<<<<<<< HEAD
		/* guard against (alleged) silicon errata */
		if (cmd & CMD_IAAD) {
			ehci_writel(ehci, cmd & ~CMD_IAAD,
					&ehci->regs->command);
			ehci_dbg(ehci, "IAA with IAAD still set?\n");
		}
		if (ehci->reclaim) {
			COUNT(ehci->stats.reclaim);
			end_unlink_async(ehci);
		} else
			ehci_dbg(ehci, "IAA with nothing to reclaim?\n");
=======

		/* Turn off the IAA watchdog */
		ehci->enabled_hrtimer_events &= ~BIT(EHCI_HRTIMER_IAA_WATCHDOG);

		/*
		 * Mild optimization: Allow another IAAD to reset the
		 * hrtimer, if one occurs before the next expiration.
		 * In theory we could always cancel the hrtimer, but
		 * tests show that about half the time it will be reset
		 * for some other event anyway.
		 */
		if (ehci->next_hrtimer_event == EHCI_HRTIMER_IAA_WATCHDOG)
			++ehci->next_hrtimer_event;

		/* guard against (alleged) silicon errata */
		if (cmd & CMD_IAAD)
			ehci_dbg(ehci, "IAA with IAAD still set?\n");
		if (ehci->iaa_in_progress)
			COUNT(ehci->stats.iaa);
		end_unlink_async(ehci);
>>>>>>> refs/remotes/origin/master
	}

	/* remote wakeup [4.3.1] */
	if (status & STS_PCD) {
		unsigned	i = HCS_N_PORTS (ehci->hcs_params);
<<<<<<< HEAD
		u32		ppcd = 0;
=======
		u32		ppcd = ~0;
>>>>>>> refs/remotes/origin/master

		/* kick root hub later */
		pcd_status = status;

		/* resume root hub? */
<<<<<<< HEAD
<<<<<<< HEAD
		if (hcd->state == HC_STATE_SUSPENDED)
=======
		if (ehci->rh_state == EHCI_RH_SUSPENDED)
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (ehci->rh_state == EHCI_RH_SUSPENDED)
>>>>>>> refs/remotes/origin/master
			usb_hcd_resume_root_hub(hcd);

		/* get per-port change detect bits */
		if (ehci->has_ppcd)
			ppcd = status >> 16;

		while (i--) {
			int pstatus;

			/* leverage per-port change bits feature */
<<<<<<< HEAD
			if (ehci->has_ppcd && !(ppcd & (1 << i)))
=======
			if (!(ppcd & (1 << i)))
>>>>>>> refs/remotes/origin/master
				continue;
			pstatus = ehci_readl(ehci,
					 &ehci->regs->port_status[i]);

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
			/*set RS bit in case of remote wakeup*/
			if (ehci_is_TDI(ehci) && !(cmd & CMD_RUN) &&
					(pstatus & PORT_SUSPEND))
				ehci_writel(ehci, cmd | CMD_RUN,
						&ehci->regs->command);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
			if (pstatus & PORT_OWNER)
				continue;
			if (!(test_bit(i, &ehci->suspended_ports) &&
					((pstatus & PORT_RESUME) ||
						!(pstatus & PORT_SUSPEND)) &&
					(pstatus & PORT_PE) &&
					ehci->reset_done[i] == 0))
				continue;

			/* start 20 msec resume signaling from this port,
			 * and make khubd collect PORT_STAT_C_SUSPEND to
			 * stop that signaling.  Use 5 ms extra for safety,
			 * like usb_port_resume() does.
			 */
			ehci->reset_done[i] = jiffies + msecs_to_jiffies(25);
<<<<<<< HEAD
<<<<<<< HEAD
=======
			set_bit(i, &ehci->resuming_ports);
>>>>>>> refs/remotes/origin/cm-10.0
			ehci_dbg (ehci, "port %d remote wakeup\n", i + 1);
=======
			set_bit(i, &ehci->resuming_ports);
			ehci_dbg (ehci, "port %d remote wakeup\n", i + 1);
			usb_hcd_start_port_resume(&hcd->self, i);
>>>>>>> refs/remotes/origin/master
			mod_timer(&hcd->rh_timer, ehci->reset_done[i]);
		}
	}

	/* PCI errors [4.15.2.4] */
	if (unlikely ((status & STS_FATAL) != 0)) {
		ehci_err(ehci, "fatal error\n");
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
		if (hcd->driver->dump_regs)
			hcd->driver->dump_regs(hcd);
		panic("System error\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (hcd->driver->dump_regs)
			hcd->driver->dump_regs(hcd);
		panic("System error\n");
>>>>>>> refs/remotes/origin/cm-11.0
		dbg_cmd(ehci, "fatal", cmd);
		dbg_status(ehci, "fatal", status);
		ehci_halt(ehci);
dead:
		ehci_reset(ehci);
		ehci_writel(ehci, 0, &ehci->regs->configured_flag);
		usb_hc_died(hcd);
		/* generic layer kills/unlinks all urbs, then
		 * uses ehci_stop to clean up the rest
		 */
		bh = 1;
=======
		dbg_cmd(ehci, "fatal", cmd);
		dbg_status(ehci, "fatal", status);
dead:
		usb_hc_died(hcd);

		/* Don't let the controller do anything more */
		ehci->shutdown = true;
		ehci->rh_state = EHCI_RH_STOPPING;
		ehci->command &= ~(CMD_RUN | CMD_ASE | CMD_PSE);
		ehci_writel(ehci, ehci->command, &ehci->regs->command);
		ehci_writel(ehci, 0, &ehci->regs->intr_enable);
		ehci_handle_controller_death(ehci);

		/* Handle completions when the controller stops */
		bh = 0;
>>>>>>> refs/remotes/origin/master
	}

	if (bh)
		ehci_work (ehci);
	spin_unlock (&ehci->lock);
	if (pcd_status)
		usb_hcd_poll_rh_status(hcd);
	return IRQ_HANDLED;
}

/*-------------------------------------------------------------------------*/

/*
 * non-error returns are a promise to giveback() the urb later
 * we drop ownership so next owner (or urb unlink) can get it
 *
 * urb + dev is in hcd.self.controller.urb_list
 * we're queueing TDs onto software and hardware lists
 *
 * hcd-specific init for hcpriv hasn't been done yet
 *
 * NOTE:  control, bulk, and interrupt share the same code to append TDs
 * to a (possibly active) QH, and the same QH scanning code.
 */
static int ehci_urb_enqueue (
	struct usb_hcd	*hcd,
	struct urb	*urb,
	gfp_t		mem_flags
) {
	struct ehci_hcd		*ehci = hcd_to_ehci (hcd);
	struct list_head	qtd_list;

	INIT_LIST_HEAD (&qtd_list);

	switch (usb_pipetype (urb->pipe)) {
	case PIPE_CONTROL:
		/* qh_completions() code doesn't handle all the fault cases
		 * in multi-TD control transfers.  Even 1KB is rare anyway.
		 */
		if (urb->transfer_buffer_length > (16 * 1024))
			return -EMSGSIZE;
		/* FALLTHROUGH */
	/* case PIPE_BULK: */
	default:
		if (!qh_urb_transaction (ehci, urb, &qtd_list, mem_flags))
			return -ENOMEM;
		return submit_async(ehci, urb, &qtd_list, mem_flags);

	case PIPE_INTERRUPT:
		if (!qh_urb_transaction (ehci, urb, &qtd_list, mem_flags))
			return -ENOMEM;
		return intr_submit(ehci, urb, &qtd_list, mem_flags);

	case PIPE_ISOCHRONOUS:
		if (urb->dev->speed == USB_SPEED_HIGH)
			return itd_submit (ehci, urb, mem_flags);
		else
			return sitd_submit (ehci, urb, mem_flags);
	}
}

<<<<<<< HEAD
static void unlink_async (struct ehci_hcd *ehci, struct ehci_qh *qh)
{
	/* failfast */
<<<<<<< HEAD
	if (!HC_IS_RUNNING(ehci_to_hcd(ehci)->state) && ehci->reclaim)
=======
	if (ehci->rh_state != EHCI_RH_RUNNING && ehci->reclaim)
>>>>>>> refs/remotes/origin/cm-10.0
		end_unlink_async(ehci);

	/* If the QH isn't linked then there's nothing we can do
	 * unless we were called during a giveback, in which case
	 * qh_completions() has to deal with it.
	 */
	if (qh->qh_state != QH_STATE_LINKED) {
		if (qh->qh_state == QH_STATE_COMPLETING)
			qh->needs_rescan = 1;
		return;
	}

	/* defer till later if busy */
	if (ehci->reclaim) {
		struct ehci_qh		*last;

		for (last = ehci->reclaim;
				last->reclaim;
				last = last->reclaim)
			continue;
		qh->qh_state = QH_STATE_UNLINK_WAIT;
		last->reclaim = qh;

	/* start IAA cycle */
	} else
		start_unlink_async (ehci, qh);
}

=======
>>>>>>> refs/remotes/origin/master
/* remove from hardware lists
 * completions normally happen asynchronously
 */

static int ehci_urb_dequeue(struct usb_hcd *hcd, struct urb *urb, int status)
{
	struct ehci_hcd		*ehci = hcd_to_ehci (hcd);
	struct ehci_qh		*qh;
	unsigned long		flags;
	int			rc;

	spin_lock_irqsave (&ehci->lock, flags);
	rc = usb_hcd_check_unlink_urb(hcd, urb, status);
	if (rc)
		goto done;

<<<<<<< HEAD
	switch (usb_pipetype (urb->pipe)) {
	// case PIPE_CONTROL:
	// case PIPE_BULK:
	default:
		qh = (struct ehci_qh *) urb->hcpriv;
		if (!qh)
			break;
		switch (qh->qh_state) {
		case QH_STATE_LINKED:
		case QH_STATE_COMPLETING:
			unlink_async(ehci, qh);
=======
	if (usb_pipetype(urb->pipe) == PIPE_ISOCHRONOUS) {
		/*
		 * We don't expedite dequeue for isochronous URBs.
		 * Just wait until they complete normally or their
		 * time slot expires.
		 */
	} else {
		qh = (struct ehci_qh *) urb->hcpriv;
		qh->exception = 1;
		switch (qh->qh_state) {
		case QH_STATE_LINKED:
			if (usb_pipetype(urb->pipe) == PIPE_INTERRUPT)
				start_unlink_intr(ehci, qh);
			else
				start_unlink_async(ehci, qh);
			break;
		case QH_STATE_COMPLETING:
			qh->dequeue_during_giveback = 1;
>>>>>>> refs/remotes/origin/master
			break;
		case QH_STATE_UNLINK:
		case QH_STATE_UNLINK_WAIT:
			/* already started */
			break;
		case QH_STATE_IDLE:
			/* QH might be waiting for a Clear-TT-Buffer */
			qh_completions(ehci, qh);
			break;
		}
<<<<<<< HEAD
		break;

	case PIPE_INTERRUPT:
		qh = (struct ehci_qh *) urb->hcpriv;
		if (!qh)
			break;
		switch (qh->qh_state) {
		case QH_STATE_LINKED:
		case QH_STATE_COMPLETING:
			intr_deschedule (ehci, qh);
			break;
		case QH_STATE_IDLE:
			qh_completions (ehci, qh);
			break;
		default:
			ehci_dbg (ehci, "bogus qh %p state %d\n",
					qh, qh->qh_state);
			goto done;
		}
		break;

	case PIPE_ISOCHRONOUS:
		// itd or sitd ...

		// wait till next completion, do it then.
		// completion irqs can wait up to 1024 msec,
		break;
=======
>>>>>>> refs/remotes/origin/master
	}
done:
	spin_unlock_irqrestore (&ehci->lock, flags);
	return rc;
}

/*-------------------------------------------------------------------------*/

// bulk qh holds the data toggle

static void
ehci_endpoint_disable (struct usb_hcd *hcd, struct usb_host_endpoint *ep)
{
	struct ehci_hcd		*ehci = hcd_to_ehci (hcd);
	unsigned long		flags;
<<<<<<< HEAD
	struct ehci_qh		*qh, *tmp;
=======
	struct ehci_qh		*qh;
>>>>>>> refs/remotes/origin/master

	/* ASSERT:  any requests/urbs are being unlinked */
	/* ASSERT:  nobody can be submitting urbs for this any more */

rescan:
	spin_lock_irqsave (&ehci->lock, flags);
	qh = ep->hcpriv;
	if (!qh)
		goto done;

	/* endpoints can be iso streams.  for now, we don't
	 * accelerate iso completions ... so spin a while.
	 */
	if (qh->hw == NULL) {
<<<<<<< HEAD
		ehci_vdbg (ehci, "iso delay\n");
		goto idle_timeout;
	}

<<<<<<< HEAD
	if (!HC_IS_RUNNING (hcd->state))
=======
	if (ehci->rh_state != EHCI_RH_RUNNING)
>>>>>>> refs/remotes/origin/cm-10.0
		qh->qh_state = QH_STATE_IDLE;
	switch (qh->qh_state) {
	case QH_STATE_LINKED:
	case QH_STATE_COMPLETING:
		for (tmp = ehci->async->qh_next.qh;
				tmp && tmp != qh;
				tmp = tmp->qh_next.qh)
			continue;
		/* periodic qh self-unlinks on empty, and a COMPLETING qh
		 * may already be unlinked.
		 */
		if (tmp)
			unlink_async(ehci, qh);
		/* FALL THROUGH */
=======
		struct ehci_iso_stream	*stream = ep->hcpriv;

		if (!list_empty(&stream->td_list))
			goto idle_timeout;

		/* BUG_ON(!list_empty(&stream->free_list)); */
		reserve_release_iso_bandwidth(ehci, stream, -1);
		kfree(stream);
		goto done;
	}

	qh->exception = 1;
	if (ehci->rh_state < EHCI_RH_RUNNING)
		qh->qh_state = QH_STATE_IDLE;
	switch (qh->qh_state) {
	case QH_STATE_LINKED:
		WARN_ON(!list_empty(&qh->qtd_list));
		if (usb_endpoint_type(&ep->desc) != USB_ENDPOINT_XFER_INT)
			start_unlink_async(ehci, qh);
		else
			start_unlink_intr(ehci, qh);
		/* FALL THROUGH */
	case QH_STATE_COMPLETING:	/* already in unlinking */
>>>>>>> refs/remotes/origin/master
	case QH_STATE_UNLINK:		/* wait for hw to finish? */
	case QH_STATE_UNLINK_WAIT:
idle_timeout:
		spin_unlock_irqrestore (&ehci->lock, flags);
		schedule_timeout_uninterruptible(1);
		goto rescan;
	case QH_STATE_IDLE:		/* fully unlinked */
		if (qh->clearing_tt)
			goto idle_timeout;
		if (list_empty (&qh->qtd_list)) {
<<<<<<< HEAD
			qh_put (qh);
=======
			if (qh->ps.bw_uperiod)
				reserve_release_intr_bandwidth(ehci, qh, -1);
			qh_destroy(ehci, qh);
>>>>>>> refs/remotes/origin/master
			break;
		}
		/* else FALL THROUGH */
	default:
		/* caller was supposed to have unlinked any requests;
		 * that's not our job.  just leak this memory.
		 */
		ehci_err (ehci, "qh %p (#%02x) state %d%s\n",
			qh, ep->desc.bEndpointAddress, qh->qh_state,
			list_empty (&qh->qtd_list) ? "" : "(has tds)");
		break;
	}
<<<<<<< HEAD
	ep->hcpriv = NULL;
done:
	spin_unlock_irqrestore (&ehci->lock, flags);
}

static void __maybe_unused
<<<<<<< HEAD
=======
 done:
	ep->hcpriv = NULL;
	spin_unlock_irqrestore (&ehci->lock, flags);
}

static void
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
ehci_endpoint_reset(struct usb_hcd *hcd, struct usb_host_endpoint *ep)
{
	struct ehci_hcd		*ehci = hcd_to_ehci(hcd);
	struct ehci_qh		*qh;
	int			eptype = usb_endpoint_type(&ep->desc);
	int			epnum = usb_endpoint_num(&ep->desc);
	int			is_out = usb_endpoint_dir_out(&ep->desc);
	unsigned long		flags;

	if (eptype != USB_ENDPOINT_XFER_BULK && eptype != USB_ENDPOINT_XFER_INT)
		return;

	spin_lock_irqsave(&ehci->lock, flags);
	qh = ep->hcpriv;

	/* For Bulk and Interrupt endpoints we maintain the toggle state
	 * in the hardware; the toggle bits in udev aren't used at all.
	 * When an endpoint is reset by usb_clear_halt() we must reset
	 * the toggle bit in the QH.
	 */
	if (qh) {
<<<<<<< HEAD
		usb_settoggle(qh->dev, epnum, is_out, 0);
		if (!list_empty(&qh->qtd_list)) {
			WARN_ONCE(1, "clear_halt for a busy endpoint\n");
		} else if (qh->qh_state == QH_STATE_LINKED ||
				qh->qh_state == QH_STATE_COMPLETING) {

=======
		if (!list_empty(&qh->qtd_list)) {
			WARN_ONCE(1, "clear_halt for a busy endpoint\n");
		} else {
>>>>>>> refs/remotes/origin/master
			/* The toggle value in the QH can't be updated
			 * while the QH is active.  Unlink it now;
			 * re-linking will call qh_refresh().
			 */
<<<<<<< HEAD
			if (eptype == USB_ENDPOINT_XFER_BULK)
				unlink_async(ehci, qh);
			else
				intr_deschedule(ehci, qh);
=======
			usb_settoggle(qh->ps.udev, epnum, is_out, 0);
			qh->exception = 1;
			if (eptype == USB_ENDPOINT_XFER_BULK)
				start_unlink_async(ehci, qh);
			else
				start_unlink_intr(ehci, qh);
>>>>>>> refs/remotes/origin/master
		}
	}
	spin_unlock_irqrestore(&ehci->lock, flags);
}

static int ehci_get_frame (struct usb_hcd *hcd)
{
	struct ehci_hcd		*ehci = hcd_to_ehci (hcd);
	return (ehci_read_frame_index(ehci) >> 3) % ehci->periodic_size;
}

/*-------------------------------------------------------------------------*/

<<<<<<< HEAD
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_AUTHOR (DRIVER_AUTHOR);
MODULE_LICENSE ("GPL");

#ifdef CONFIG_PCI
#include "ehci-pci.c"
#define	PCI_DRIVER		ehci_pci_driver
#endif

#ifdef CONFIG_PPC_PS3
#include "ehci-ps3.c"
#define	PS3_SYSTEM_BUS_DRIVER	ps3_ehci_driver
#endif

#ifdef CONFIG_USB_EHCI_HCD_PPC_OF
#include "ehci-ppc-of.c"
#define OF_PLATFORM_DRIVER	ehci_hcd_ppc_of_driver
#endif

#ifdef CONFIG_XPS_USB_HCD_XILINX
#include "ehci-xilinx-of.c"
#define XILINX_OF_PLATFORM_DRIVER	ehci_hcd_xilinx_of_driver
#endif

#ifdef CONFIG_USB_EHCI_FSL
#include "ehci-fsl.c"
#define PLATFORM_DRIVER_PRESENT
#endif

#ifdef CONFIG_USB_EHCI_MXC
#include "ehci-mxc.c"
#define PLATFORM_DRIVER_PRESENT
#endif

#ifdef CONFIG_USB_EHCI_SH
#include "ehci-sh.c"
#define PLATFORM_DRIVER_PRESENT
#endif

<<<<<<< HEAD
#ifdef CONFIG_SOC_AU1200
=======
#ifdef CONFIG_MIPS_ALCHEMY
>>>>>>> refs/remotes/origin/cm-10.0
#include "ehci-au1xxx.c"
#define PLATFORM_DRIVER_PRESENT
#endif

#ifdef CONFIG_USB_EHCI_HCD_OMAP
#include "ehci-omap.c"
#define PLATFORM_DRIVER_PRESENT
#endif

#ifdef CONFIG_PLAT_ORION
#include "ehci-orion.c"
#define PLATFORM_DRIVER_PRESENT
#endif

#ifdef CONFIG_ARCH_IXP4XX
#include "ehci-ixp4xx.c"
#define PLATFORM_DRIVER_PRESENT
#endif

#ifdef CONFIG_USB_W90X900_EHCI
#include "ehci-w90x900.c"
#define PLATFORM_DRIVER_PRESENT
#endif

#ifdef CONFIG_ARCH_AT91
#include "ehci-atmel.c"
#define PLATFORM_DRIVER_PRESENT
#endif

#ifdef CONFIG_USB_OCTEON_EHCI
#include "ehci-octeon.c"
#define PLATFORM_DRIVER_PRESENT
#endif

#ifdef CONFIG_USB_CNS3XXX_EHCI
#include "ehci-cns3xxx.c"
#define PLATFORM_DRIVER_PRESENT
#endif

#ifdef CONFIG_ARCH_VT8500
#include "ehci-vt8500.c"
#define PLATFORM_DRIVER_PRESENT
#endif

#ifdef CONFIG_PLAT_SPEAR
#include "ehci-spear.c"
#define PLATFORM_DRIVER_PRESENT
#endif

#ifdef CONFIG_USB_EHCI_MSM_72K
#include "ehci-msm72k.c"
#define PLATFORM_DRIVER_PRESENT
#endif

#ifdef CONFIG_USB_EHCI_MSM
#include "ehci-msm.c"
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include "ehci-msm2.c"
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include "ehci-msm2.c"
>>>>>>> refs/remotes/origin/cm-11.0
#define PLATFORM_DRIVER_PRESENT
#endif

#ifdef CONFIG_USB_EHCI_HCD_PMC_MSP
#include "ehci-pmcmsp.c"
#define PLATFORM_DRIVER_PRESENT
#endif

#ifdef CONFIG_USB_EHCI_TEGRA
#include "ehci-tegra.c"
#define PLATFORM_DRIVER_PRESENT
#endif

#ifdef CONFIG_USB_EHCI_S5P
#include "ehci-s5p.c"
#define PLATFORM_DRIVER_PRESENT
#endif

#ifdef CONFIG_USB_EHCI_ATH79
#include "ehci-ath79.c"
#define PLATFORM_DRIVER_PRESENT
#endif

#ifdef CONFIG_SPARC_LEON
#include "ehci-grlib.c"
#define PLATFORM_DRIVER_PRESENT
#endif

#ifdef CONFIG_USB_EHCI_MSM_HSIC
#include "ehci-msm-hsic.c"
#define PLATFORM_DRIVER_PRESENT
#endif

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
#ifdef CONFIG_USB_PXA168_EHCI
#include "ehci-pxa168.c"
#define PLATFORM_DRIVER_PRESENT
#endif

#ifdef CONFIG_CPU_XLR
#include "ehci-xls.c"
#define PLATFORM_DRIVER_PRESENT
#endif

#ifdef CONFIG_USB_EHCI_MV
#include "ehci-mv.c"
#define PLATFORM_DRIVER_PRESENT
#endif

#ifdef CONFIG_MACH_LOONGSON1
#include "ehci-ls1x.c"
#define PLATFORM_DRIVER_PRESENT
#endif

#ifdef CONFIG_USB_EHCI_HCD_PLATFORM
#include "ehci-platform.c"
#define PLATFORM_DRIVER_PRESENT
#endif

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
#if !defined(PCI_DRIVER) && !defined(PLATFORM_DRIVER_PRESENT) && \
    !defined(PS3_SYSTEM_BUS_DRIVER) && !defined(OF_PLATFORM_DRIVER) && \
    !defined(XILINX_OF_PLATFORM_DRIVER)
#error "missing bus glue for ehci-hcd"
#endif

static struct platform_driver *plat_drivers[]  = {
#ifdef CONFIG_USB_EHCI_FSL
	&ehci_fsl_driver,
#endif

#ifdef CONFIG_USB_EHCI_MXC
	&ehci_mxc_driver,
#endif

#ifdef CONFIG_CPU_SUBTYPE_SH7786
	&ehci_hcd_sh_driver,
#endif

#ifdef CONFIG_SOC_AU1200
	&ehci_hcd_au1xxx_driver,
#endif

#ifdef CONFIG_USB_EHCI_HCD_OMAP
	&ehci_hcd_omap_driver,
#endif

#ifdef CONFIG_PLAT_ORION
	&ehci_orion_driver,
#endif

#ifdef CONFIG_ARCH_IXP4XX
	&ixp4xx_ehci_driver,
#endif

#ifdef CONFIG_USB_W90X900_EHCI
	&ehci_hcd_w90x900_driver,
#endif

#ifdef CONFIG_ARCH_AT91
	&ehci_atmel_driver,
#endif

#ifdef CONFIG_USB_OCTEON_EHCI
	&ehci_octeon_driver,
#endif

#ifdef CONFIG_USB_CNS3XXX_EHCI
	&cns3xxx_ehci_driver,
#endif

#ifdef CONFIG_ARCH_VT8500
	&vt8500_ehci_driver,
#endif

#ifdef CONFIG_PLAT_SPEAR
	&spear_ehci_hcd_driver,
#endif

#ifdef CONFIG_USB_EHCI_HCD_PMC_MSP
	&ehci_hcd_msp_driver
#endif

#ifdef CONFIG_USB_EHCI_TEGRA
	&tegra_ehci_driver
#endif

#ifdef CONFIG_USB_EHCI_S5P
	&s5p_ehci_driver
#endif

#ifdef CONFIG_USB_EHCI_ATH79
	&ehci_ath79_driver
#endif

#ifdef CONFIG_SPARC_LEON
	&ehci_grlib_driver
#endif

#if defined(CONFIG_USB_EHCI_MSM_72K) || defined(CONFIG_USB_EHCI_MSM)
	&ehci_msm_driver,
#endif

#ifdef CONFIG_USB_EHCI_MSM_HSIC
<<<<<<< HEAD
<<<<<<< HEAD
	&ehci_msm_hsic_driver
#endif

=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	&ehci_msm_hsic_driver,
#endif

#ifdef CONFIG_USB_EHCI_MSM
	&ehci_msm2_driver,
#endif

#ifdef CONFIG_USB_PXA168_EHCI
	&ehci_pxa168_driver,
#endif

#ifdef CONFIG_CPU_XLR
	&ehci_xls_driver,
#endif

#ifdef CONFIG_USB_EHCI_MV
	&ehci_mv_driver,
#endif

#ifdef CONFIG_MACH_LOONGSON1
	&ehci_ls1x_driver,
#endif

#ifdef CONFIG_USB_EHCI_HCD_PLATFORM
	&ehci_platform_driver,
#endif
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
};


static int __init ehci_hcd_init(void)
{
	int i, retval = 0;
=======
/* Device addition and removal */

static void ehci_remove_device(struct usb_hcd *hcd, struct usb_device *udev)
{
	struct ehci_hcd		*ehci = hcd_to_ehci(hcd);

	spin_lock_irq(&ehci->lock);
	drop_tt(udev);
	spin_unlock_irq(&ehci->lock);
}

/*-------------------------------------------------------------------------*/

#ifdef	CONFIG_PM

/* suspend/resume, section 4.3 */

/* These routines handle the generic parts of controller suspend/resume */

int ehci_suspend(struct usb_hcd *hcd, bool do_wakeup)
{
	struct ehci_hcd		*ehci = hcd_to_ehci(hcd);

	if (time_before(jiffies, ehci->next_statechange))
		msleep(10);

	/*
	 * Root hub was already suspended.  Disable IRQ emission and
	 * mark HW unaccessible.  The PM and USB cores make sure that
	 * the root hub is either suspended or stopped.
	 */
	ehci_prepare_ports_for_controller_suspend(ehci, do_wakeup);

	spin_lock_irq(&ehci->lock);
	ehci_writel(ehci, 0, &ehci->regs->intr_enable);
	(void) ehci_readl(ehci, &ehci->regs->intr_enable);

	clear_bit(HCD_FLAG_HW_ACCESSIBLE, &hcd->flags);
	spin_unlock_irq(&ehci->lock);

	synchronize_irq(hcd->irq);

	/* Check for race with a wakeup request */
	if (do_wakeup && HCD_WAKEUP_PENDING(hcd)) {
		ehci_resume(hcd, false);
		return -EBUSY;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(ehci_suspend);

/* Returns 0 if power was preserved, 1 if power was lost */
int ehci_resume(struct usb_hcd *hcd, bool hibernated)
{
	struct ehci_hcd		*ehci = hcd_to_ehci(hcd);

	if (time_before(jiffies, ehci->next_statechange))
		msleep(100);

	/* Mark hardware accessible again as we are back to full power by now */
	set_bit(HCD_FLAG_HW_ACCESSIBLE, &hcd->flags);

	if (ehci->shutdown)
		return 0;		/* Controller is dead */

	/*
	 * If CF is still set and we aren't resuming from hibernation
	 * then we maintained suspend power.
	 * Just undo the effect of ehci_suspend().
	 */
	if (ehci_readl(ehci, &ehci->regs->configured_flag) == FLAG_CF &&
			!hibernated) {
		int	mask = INTR_MASK;

		ehci_prepare_ports_for_controller_resume(ehci);

		spin_lock_irq(&ehci->lock);
		if (ehci->shutdown)
			goto skip;

		if (!hcd->self.root_hub->do_remote_wakeup)
			mask &= ~STS_PCD;
		ehci_writel(ehci, mask, &ehci->regs->intr_enable);
		ehci_readl(ehci, &ehci->regs->intr_enable);
 skip:
		spin_unlock_irq(&ehci->lock);
		return 0;
	}

	/*
	 * Else reset, to cope with power loss or resume from hibernation
	 * having let the firmware kick in during reboot.
	 */
	usb_root_hub_lost_power(hcd->self.root_hub);
	(void) ehci_halt(ehci);
	(void) ehci_reset(ehci);

	spin_lock_irq(&ehci->lock);
	if (ehci->shutdown)
		goto skip;

	ehci_writel(ehci, ehci->command, &ehci->regs->command);
	ehci_writel(ehci, FLAG_CF, &ehci->regs->configured_flag);
	ehci_readl(ehci, &ehci->regs->command);	/* unblock posted writes */

	ehci->rh_state = EHCI_RH_SUSPENDED;
	spin_unlock_irq(&ehci->lock);

	return 1;
}
EXPORT_SYMBOL_GPL(ehci_resume);

#endif

/*-------------------------------------------------------------------------*/

/*
 * Generic structure: This gets copied for platform drivers so that
 * individual entries can be overridden as needed.
 */

static const struct hc_driver ehci_hc_driver = {
	.description =		hcd_name,
	.product_desc =		"EHCI Host Controller",
	.hcd_priv_size =	sizeof(struct ehci_hcd),

	/*
	 * generic hardware linkage
	 */
	.irq =			ehci_irq,
	.flags =		HCD_MEMORY | HCD_USB2 | HCD_BH,

	/*
	 * basic lifecycle operations
	 */
	.reset =		ehci_setup,
	.start =		ehci_run,
	.stop =			ehci_stop,
	.shutdown =		ehci_shutdown,

	/*
	 * managing i/o requests and associated device resources
	 */
	.urb_enqueue =		ehci_urb_enqueue,
	.urb_dequeue =		ehci_urb_dequeue,
	.endpoint_disable =	ehci_endpoint_disable,
	.endpoint_reset =	ehci_endpoint_reset,
	.clear_tt_buffer_complete =	ehci_clear_tt_buffer_complete,

	/*
	 * scheduling support
	 */
	.get_frame_number =	ehci_get_frame,

	/*
	 * root hub support
	 */
	.hub_status_data =	ehci_hub_status_data,
	.hub_control =		ehci_hub_control,
	.bus_suspend =		ehci_bus_suspend,
	.bus_resume =		ehci_bus_resume,
	.relinquish_port =	ehci_relinquish_port,
	.port_handed_over =	ehci_port_handed_over,

	/*
	 * device support
	 */
	.free_dev =		ehci_remove_device,
};

void ehci_init_driver(struct hc_driver *drv,
		const struct ehci_driver_overrides *over)
{
	/* Copy the generic table to drv and then apply the overrides */
	*drv = ehci_hc_driver;

	if (over) {
		drv->hcd_priv_size += over->extra_priv_size;
		if (over->reset)
			drv->reset = over->reset;
	}
}
EXPORT_SYMBOL_GPL(ehci_init_driver);

/*-------------------------------------------------------------------------*/

MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_AUTHOR (DRIVER_AUTHOR);
MODULE_LICENSE ("GPL");

#ifdef CONFIG_USB_EHCI_FSL
#include "ehci-fsl.c"
#define	PLATFORM_DRIVER		ehci_fsl_driver
#endif

#ifdef CONFIG_USB_EHCI_SH
#include "ehci-sh.c"
#define PLATFORM_DRIVER		ehci_hcd_sh_driver
#endif

#ifdef CONFIG_PPC_PS3
#include "ehci-ps3.c"
#define	PS3_SYSTEM_BUS_DRIVER	ps3_ehci_driver
#endif

#ifdef CONFIG_USB_EHCI_HCD_PPC_OF
#include "ehci-ppc-of.c"
#define OF_PLATFORM_DRIVER	ehci_hcd_ppc_of_driver
#endif

#ifdef CONFIG_XPS_USB_HCD_XILINX
#include "ehci-xilinx-of.c"
#define XILINX_OF_PLATFORM_DRIVER	ehci_hcd_xilinx_of_driver
#endif

#ifdef CONFIG_USB_OCTEON_EHCI
#include "ehci-octeon.c"
#define PLATFORM_DRIVER		ehci_octeon_driver
#endif

#ifdef CONFIG_TILE_USB
#include "ehci-tilegx.c"
#define	PLATFORM_DRIVER		ehci_hcd_tilegx_driver
#endif

#ifdef CONFIG_USB_EHCI_HCD_PMC_MSP
#include "ehci-pmcmsp.c"
#define	PLATFORM_DRIVER		ehci_hcd_msp_driver
#endif

#ifdef CONFIG_SPARC_LEON
#include "ehci-grlib.c"
#define PLATFORM_DRIVER		ehci_grlib_driver
#endif

#ifdef CONFIG_USB_EHCI_MV
#include "ehci-mv.c"
#define        PLATFORM_DRIVER         ehci_mv_driver
#endif

#ifdef CONFIG_MIPS_SEAD3
#include "ehci-sead3.c"
#define	PLATFORM_DRIVER		ehci_hcd_sead3_driver
#endif

static int __init ehci_hcd_init(void)
{
	int retval = 0;
>>>>>>> refs/remotes/origin/master
=======
};


static int __init ehci_hcd_init(void)
{
	int i, retval = 0;
>>>>>>> refs/remotes/origin/cm-11.0

	if (usb_disabled())
		return -ENODEV;

	printk(KERN_INFO "%s: " DRIVER_DESC "\n", hcd_name);
	set_bit(USB_EHCI_LOADED, &usb_hcds_loaded);
	if (test_bit(USB_UHCI_LOADED, &usb_hcds_loaded) ||
			test_bit(USB_OHCI_LOADED, &usb_hcds_loaded))
		printk(KERN_WARNING "Warning! ehci_hcd should always be loaded"
				" before uhci_hcd and ohci_hcd, not after\n");

	pr_debug("%s: block sizes: qh %Zd qtd %Zd itd %Zd sitd %Zd\n",
		 hcd_name,
		 sizeof(struct ehci_qh), sizeof(struct ehci_qtd),
		 sizeof(struct ehci_itd), sizeof(struct ehci_sitd));

<<<<<<< HEAD
#ifdef DEBUG
=======
#ifdef CONFIG_DYNAMIC_DEBUG
>>>>>>> refs/remotes/origin/master
	ehci_debug_root = debugfs_create_dir("ehci", usb_debug_root);
	if (!ehci_debug_root) {
		retval = -ENOENT;
		goto err_debug;
	}
#endif

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	for (i = 0; i < ARRAY_SIZE(plat_drivers); i++) {
		retval = platform_driver_register(plat_drivers[i]);
		if (retval) {
			while (--i >= 0)
				platform_driver_unregister(plat_drivers[i]);
			goto clean0;
		}
	}

#ifdef PCI_DRIVER
	retval = pci_register_driver(&PCI_DRIVER);
	if (retval < 0)
		goto clean1;
=======
#ifdef PLATFORM_DRIVER
	retval = platform_driver_register(&PLATFORM_DRIVER);
	if (retval < 0)
		goto clean0;
>>>>>>> refs/remotes/origin/master
#endif

#ifdef PS3_SYSTEM_BUS_DRIVER
	retval = ps3_ehci_driver_register(&PS3_SYSTEM_BUS_DRIVER);
	if (retval < 0)
		goto clean2;
#endif

#ifdef OF_PLATFORM_DRIVER
	retval = platform_driver_register(&OF_PLATFORM_DRIVER);
	if (retval < 0)
		goto clean3;
#endif

#ifdef XILINX_OF_PLATFORM_DRIVER
	retval = platform_driver_register(&XILINX_OF_PLATFORM_DRIVER);
	if (retval < 0)
		goto clean4;
#endif
	return retval;

#ifdef XILINX_OF_PLATFORM_DRIVER
	/* platform_driver_unregister(&XILINX_OF_PLATFORM_DRIVER); */
clean4:
#endif
#ifdef OF_PLATFORM_DRIVER
	platform_driver_unregister(&OF_PLATFORM_DRIVER);
clean3:
#endif
#ifdef PS3_SYSTEM_BUS_DRIVER
	ps3_ehci_driver_unregister(&PS3_SYSTEM_BUS_DRIVER);
clean2:
#endif
<<<<<<< HEAD
#ifdef PCI_DRIVER
	pci_unregister_driver(&PCI_DRIVER);
clean1:
#endif
	for (i = 0; i < ARRAY_SIZE(plat_drivers); i++)
		platform_driver_unregister(plat_drivers[i]);
<<<<<<< HEAD
clean0:
#ifdef DEBUG
=======
#ifdef PLATFORM_DRIVER
	platform_driver_unregister(&PLATFORM_DRIVER);
clean0:
#endif
#ifdef CONFIG_DYNAMIC_DEBUG
>>>>>>> refs/remotes/origin/master
=======
clean0:
#ifdef DEBUG
>>>>>>> refs/remotes/origin/cm-11.0
	debugfs_remove(ehci_debug_root);
	ehci_debug_root = NULL;
err_debug:
#endif
	clear_bit(USB_EHCI_LOADED, &usb_hcds_loaded);
	return retval;
}
module_init(ehci_hcd_init);

static void __exit ehci_hcd_cleanup(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	int i;
=======
>>>>>>> refs/remotes/origin/master
=======
	int i;
>>>>>>> refs/remotes/origin/cm-11.0
#ifdef XILINX_OF_PLATFORM_DRIVER
	platform_driver_unregister(&XILINX_OF_PLATFORM_DRIVER);
#endif
#ifdef OF_PLATFORM_DRIVER
	platform_driver_unregister(&OF_PLATFORM_DRIVER);
#endif
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0

	for (i = 0; i < ARRAY_SIZE(plat_drivers); i++)
		platform_driver_unregister(plat_drivers[i]);

#ifdef PCI_DRIVER
	pci_unregister_driver(&PCI_DRIVER);
=======
#ifdef PLATFORM_DRIVER
	platform_driver_unregister(&PLATFORM_DRIVER);
>>>>>>> refs/remotes/origin/master
#endif
#ifdef PS3_SYSTEM_BUS_DRIVER
	ps3_ehci_driver_unregister(&PS3_SYSTEM_BUS_DRIVER);
#endif
<<<<<<< HEAD
#ifdef DEBUG
=======
#ifdef CONFIG_DYNAMIC_DEBUG
>>>>>>> refs/remotes/origin/master
	debugfs_remove(ehci_debug_root);
#endif
	clear_bit(USB_EHCI_LOADED, &usb_hcds_loaded);
}
module_exit(ehci_hcd_cleanup);
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
