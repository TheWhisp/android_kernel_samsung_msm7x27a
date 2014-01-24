/*
 * arch/xtensa/kernel/time.c
 *
 * Timer and clock support.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2005 Tensilica Inc.
 *
 * Chris Zankel <chris@zankel.net>
 */

#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/clocksource.h>
<<<<<<< HEAD
=======
#include <linux/clockchips.h>
>>>>>>> refs/remotes/origin/master
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/profile.h>
#include <linux/delay.h>
<<<<<<< HEAD
=======
#include <linux/irqdomain.h>
#include <linux/sched_clock.h>
>>>>>>> refs/remotes/origin/master

#include <asm/timex.h>
#include <asm/platform.h>

<<<<<<< HEAD
#ifdef CONFIG_XTENSA_CALIBRATE_CCOUNT
unsigned long ccount_per_jiffy;		/* per 1/HZ */
unsigned long nsec_per_ccount;		/* nsec per ccount increment */
#endif

static cycle_t ccount_read(void)
=======
unsigned long ccount_freq;		/* ccount Hz */

static cycle_t ccount_read(struct clocksource *cs)
>>>>>>> refs/remotes/origin/master
{
	return (cycle_t)get_ccount();
}

<<<<<<< HEAD
=======
static u32 notrace ccount_sched_clock_read(void)
{
	return get_ccount();
}

>>>>>>> refs/remotes/origin/master
static struct clocksource ccount_clocksource = {
	.name = "ccount",
	.rating = 200,
	.read = ccount_read,
	.mask = CLOCKSOURCE_MASK(32),
<<<<<<< HEAD
<<<<<<< HEAD
	/*
	 * With a shift of 22 the lower limit of the cpu clock is
	 * 1MHz, where NSEC_PER_CCOUNT is 1000 or a bit less than
	 * 2^10: Since we have 32 bits and the multiplicator can
	 * already take up as much as 10 bits, this leaves us with
	 * remaining upper 22 bits.
	 */
	.shift = 22,
=======
>>>>>>> refs/remotes/origin/cm-10.0
};
=======
};

static int ccount_timer_set_next_event(unsigned long delta,
		struct clock_event_device *dev);
static void ccount_timer_set_mode(enum clock_event_mode mode,
		struct clock_event_device *evt);
static struct ccount_timer_t {
	struct clock_event_device evt;
	int irq_enabled;
} ccount_timer = {
	.evt = {
		.name		= "ccount_clockevent",
		.features	= CLOCK_EVT_FEAT_ONESHOT,
		.rating		= 300,
		.set_next_event	= ccount_timer_set_next_event,
		.set_mode	= ccount_timer_set_mode,
	},
};

static int ccount_timer_set_next_event(unsigned long delta,
		struct clock_event_device *dev)
{
	unsigned long flags, next;
	int ret = 0;

	local_irq_save(flags);
	next = get_ccount() + delta;
	set_linux_timer(next);
	if (next - get_ccount() > delta)
		ret = -ETIME;
	local_irq_restore(flags);

	return ret;
}

static void ccount_timer_set_mode(enum clock_event_mode mode,
		struct clock_event_device *evt)
{
	struct ccount_timer_t *timer =
		container_of(evt, struct ccount_timer_t, evt);

	/*
	 * There is no way to disable the timer interrupt at the device level,
	 * only at the intenable register itself. Since enable_irq/disable_irq
	 * calls are nested, we need to make sure that these calls are
	 * balanced.
	 */
	switch (mode) {
	case CLOCK_EVT_MODE_SHUTDOWN:
	case CLOCK_EVT_MODE_UNUSED:
		if (timer->irq_enabled) {
			disable_irq(evt->irq);
			timer->irq_enabled = 0;
		}
		break;
	case CLOCK_EVT_MODE_RESUME:
	case CLOCK_EVT_MODE_ONESHOT:
		if (!timer->irq_enabled) {
			enable_irq(evt->irq);
			timer->irq_enabled = 1;
		}
	default:
		break;
	}
}
>>>>>>> refs/remotes/origin/master

static irqreturn_t timer_interrupt(int irq, void *dev_id);
static struct irqaction timer_irqaction = {
	.handler =	timer_interrupt,
<<<<<<< HEAD
	.flags =	IRQF_DISABLED,
	.name =		"timer",
=======
	.flags =	IRQF_TIMER,
	.name =		"timer",
	.dev_id =	&ccount_timer,
>>>>>>> refs/remotes/origin/master
};

void __init time_init(void)
{
#ifdef CONFIG_XTENSA_CALIBRATE_CCOUNT
	printk("Calibrating CPU frequency ");
	platform_calibrate_ccount();
<<<<<<< HEAD
	printk("%d.%02d MHz\n", (int)ccount_per_jiffy/(1000000/HZ),
			(int)(ccount_per_jiffy/(10000/HZ))%100);
#endif
<<<<<<< HEAD
	ccount_clocksource.mult =
		clocksource_hz2mult(CCOUNT_PER_JIFFY * HZ,
				ccount_clocksource.shift);
	clocksource_register(&ccount_clocksource);
=======
	clocksource_register_hz(&ccount_clocksource, CCOUNT_PER_JIFFY * HZ);
>>>>>>> refs/remotes/origin/cm-10.0

	/* Initialize the linux timer interrupt. */

	setup_irq(LINUX_TIMER_INT, &timer_irqaction);
	set_linux_timer(get_ccount() + CCOUNT_PER_JIFFY);
=======
	printk("%d.%02d MHz\n", (int)ccount_freq/1000000,
			(int)(ccount_freq/10000)%100);
#else
	ccount_freq = CONFIG_XTENSA_CPU_CLOCK*1000000UL;
#endif
	clocksource_register_hz(&ccount_clocksource, ccount_freq);

	ccount_timer.evt.cpumask = cpumask_of(0);
	ccount_timer.evt.irq = irq_create_mapping(NULL, LINUX_TIMER_INT);
	if (WARN(!ccount_timer.evt.irq, "error: can't map timer irq"))
		return;
	clockevents_config_and_register(&ccount_timer.evt, ccount_freq, 0xf,
			0xffffffff);
	setup_irq(ccount_timer.evt.irq, &timer_irqaction);
	ccount_timer.irq_enabled = 1;

	setup_sched_clock(ccount_sched_clock_read, 32, ccount_freq);
>>>>>>> refs/remotes/origin/master
}

/*
 * The timer interrupt is called HZ times per second.
 */

irqreturn_t timer_interrupt (int irq, void *dev_id)
{
<<<<<<< HEAD

	unsigned long next;

	next = get_linux_timer();

again:
	while ((signed long)(get_ccount() - next) > 0) {

		profile_tick(CPU_PROFILING);
#ifndef CONFIG_SMP
		update_process_times(user_mode(get_irq_regs()));
#endif

		xtime_update(1); /* Linux handler in kernel/time/timekeeping */

		/* Note that writing CCOMPARE clears the interrupt. */

		next += CCOUNT_PER_JIFFY;
		set_linux_timer(next);
	}

	/* Allow platform to do something useful (Wdog). */

	platform_heartbeat();

	/* Make sure we didn't miss any tick... */

	if ((signed long)(get_ccount() - next) > 0)
		goto again;

=======
	struct ccount_timer_t *timer = dev_id;
	struct clock_event_device *evt = &timer->evt;

	evt->event_handler(evt);

	/* Allow platform to do something useful (Wdog). */
	platform_heartbeat();

>>>>>>> refs/remotes/origin/master
	return IRQ_HANDLED;
}

#ifndef CONFIG_GENERIC_CALIBRATE_DELAY
<<<<<<< HEAD
void __cpuinit calibrate_delay(void)
{
	loops_per_jiffy = CCOUNT_PER_JIFFY;
=======
void calibrate_delay(void)
{
	loops_per_jiffy = ccount_freq / HZ;
>>>>>>> refs/remotes/origin/master
	printk("Calibrating delay loop (skipped)... "
	       "%lu.%02lu BogoMIPS preset\n",
	       loops_per_jiffy/(1000000/HZ),
	       (loops_per_jiffy/(10000/HZ)) % 100);
}
#endif
