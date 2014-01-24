/*
 * PowerPC 64-bit swsusp implementation
 *
 * Copyright 2006 Johannes Berg <johannes@sipsolutions.net>
 *
 * GPLv2
 */

<<<<<<< HEAD
#include <asm/system.h>
#include <asm/iommu.h>
#include <linux/irq.h>
=======
#include <asm/iommu.h>
#include <linux/irq.h>
#include <linux/sched.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/interrupt.h>

void do_after_copyback(void)
{
	iommu_restore();
	touch_softlockup_watchdog();
	mb();
}

void _iommu_save(void)
{
	iommu_save();
}
