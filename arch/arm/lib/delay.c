/*
<<<<<<< HEAD
 *  Originally from linux/arch/arm/lib/delay.S
 *
 *  Copyright (C) 1995, 1996 Russell King
 *  Copyright (c) 2010, The Linux Foundation. All rights reserved.
 *  Copyright (C) 1993 Linus Torvalds
 *  Copyright (C) 1997 Martin Mares <mj@atrey.karlin.mff.cuni.cz>
 *  Copyright (C) 2005-2006 Atmel Corporation
=======
 * Delay loops based on the OpenRISC implementation.
 *
 * Copyright (C) 2012 ARM Limited
>>>>>>> refs/remotes/origin/master
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
<<<<<<< HEAD
 */
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/timex.h>

/*
 * Oh, if only we had a cycle counter...
 */
void delay_loop(unsigned long loops)
{
	asm volatile(
	"1:	subs %0, %0, #1 \n"
	"	bhi 1b		\n"
	: /* No output */
	: "r" (loops)
	);
}

#ifdef ARCH_HAS_READ_CURRENT_TIMER
/*
 * Assuming read_current_timer() is monotonically increasing
 * across calls.
 */
void read_current_timer_delay_loop(unsigned long loops)
{
	unsigned long bclock, now;

	read_current_timer(&bclock);
	do {
		read_current_timer(&now);
	} while ((now - bclock) < loops);
}
#endif

static void (*delay_fn)(unsigned long) = delay_loop;

void set_delay_fn(void (*fn)(unsigned long))
{
	delay_fn = fn;
}

/*
 * loops = usecs * HZ * loops_per_jiffy / 1000000
 */
void __delay(unsigned long loops)
{
	delay_fn(loops);
}
EXPORT_SYMBOL(__delay);

/*
 * 0 <= xloops <= 0x7fffff06
 * loops_per_jiffy <= 0x01ffffff (max. 3355 bogomips)
 */
void __const_udelay(unsigned long xloops)
{
	unsigned long lpj;
	unsigned long loops;

	xloops >>= 14;			/* max = 0x01ffffff */
	lpj = loops_per_jiffy >> 10;	/* max = 0x0001ffff */
	loops = lpj * xloops;		/* max = 0x00007fff */
	loops >>= 6;			/* max = 2^32-1 */

	if (loops)
		__delay(loops);
}
EXPORT_SYMBOL(__const_udelay);

/*
 * usecs  <= 2000
 * HZ  <= 1000
 */
void __udelay(unsigned long usecs)
{
	__const_udelay(usecs * ((2199023UL*HZ)>>11));
}
EXPORT_SYMBOL(__udelay);
=======
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * Author: Will Deacon <will.deacon@arm.com>
 */

#include <linux/delay.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/timex.h>

/*
 * Default to the loop-based delay implementation.
 */
struct arm_delay_ops arm_delay_ops = {
	.delay		= __loop_delay,
	.const_udelay	= __loop_const_udelay,
	.udelay		= __loop_udelay,
};

static const struct delay_timer *delay_timer;
static bool delay_calibrated;

int read_current_timer(unsigned long *timer_val)
{
	if (!delay_timer)
		return -ENXIO;

	*timer_val = delay_timer->read_current_timer();
	return 0;
}
EXPORT_SYMBOL_GPL(read_current_timer);

static void __timer_delay(unsigned long cycles)
{
	cycles_t start = get_cycles();

	while ((get_cycles() - start) < cycles)
		cpu_relax();
}

static void __timer_const_udelay(unsigned long xloops)
{
	unsigned long long loops = xloops;
	loops *= arm_delay_ops.ticks_per_jiffy;
	__timer_delay(loops >> UDELAY_SHIFT);
}

static void __timer_udelay(unsigned long usecs)
{
	__timer_const_udelay(usecs * UDELAY_MULT);
}

void __init register_current_timer_delay(const struct delay_timer *timer)
{
	if (!delay_calibrated) {
		pr_info("Switching to timer-based delay loop\n");
		delay_timer			= timer;
		lpj_fine			= timer->freq / HZ;

		/* cpufreq may scale loops_per_jiffy, so keep a private copy */
		arm_delay_ops.ticks_per_jiffy	= lpj_fine;
		arm_delay_ops.delay		= __timer_delay;
		arm_delay_ops.const_udelay	= __timer_const_udelay;
		arm_delay_ops.udelay		= __timer_udelay;

		delay_calibrated		= true;
	} else {
		pr_info("Ignoring duplicate/late registration of read_current_timer delay\n");
	}
}

unsigned long calibrate_delay_is_known(void)
{
	delay_calibrated = true;
	return lpj_fine;
}
>>>>>>> refs/remotes/origin/master
