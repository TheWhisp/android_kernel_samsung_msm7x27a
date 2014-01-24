/***************************************************************************/

/*
 *	sltimers.c -- generic ColdFire slice timer support.
 *
 *	Copyright (C) 2009-2010, Philippe De Muyter <phdm@macqel.be>
 *	based on
 *	timers.c -- generic ColdFire hardware timer support.
 *	Copyright (C) 1999-2008, Greg Ungerer <gerg@snapgear.com>
 */

/***************************************************************************/

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/profile.h>
#include <linux/clocksource.h>
#include <asm/io.h>
#include <asm/traps.h>
#include <asm/machdep.h>
#include <asm/coldfire.h>
#include <asm/mcfslt.h>
#include <asm/mcfsim.h>

/***************************************************************************/

#ifdef CONFIG_HIGHPROFILE

/*
 *	By default use Slice Timer 1 as the profiler clock timer.
 */
<<<<<<< HEAD
#define	PA(a)	(MCF_MBAR + MCFSLT_TIMER1 + (a))
=======
#define	PA(a)	(MCFSLT_TIMER1 + (a))
>>>>>>> refs/remotes/origin/master

/*
 *	Choose a reasonably fast profile timer. Make it an odd value to
 *	try and get good coverage of kernel operations.
 */
#define	PROFILEHZ	1013

irqreturn_t mcfslt_profile_tick(int irq, void *dummy)
{
	/* Reset Slice Timer 1 */
	__raw_writel(MCFSLT_SSR_BE | MCFSLT_SSR_TE, PA(MCFSLT_SSR));
	if (current->pid)
		profile_tick(CPU_PROFILING);
	return IRQ_HANDLED;
}

static struct irqaction mcfslt_profile_irq = {
	.name	 = "profile timer",
<<<<<<< HEAD
	.flags	 = IRQF_DISABLED | IRQF_TIMER,
=======
	.flags	 = IRQF_TIMER,
>>>>>>> refs/remotes/origin/master
	.handler = mcfslt_profile_tick,
};

void mcfslt_profile_init(void)
{
	printk(KERN_INFO "PROFILE: lodging TIMER 1 @ %dHz as profile timer\n",
	       PROFILEHZ);

	setup_irq(MCF_IRQ_PROFILER, &mcfslt_profile_irq);

	/* Set up TIMER 2 as high speed profile clock */
	__raw_writel(MCF_BUSCLK / PROFILEHZ - 1, PA(MCFSLT_STCNT));
	__raw_writel(MCFSLT_SCR_RUN | MCFSLT_SCR_IEN | MCFSLT_SCR_TEN,
								PA(MCFSLT_SCR));

}

#endif	/* CONFIG_HIGHPROFILE */

/***************************************************************************/

/*
 *	By default use Slice Timer 0 as the system clock timer.
 */
<<<<<<< HEAD
#define	TA(a)	(MCF_MBAR + MCFSLT_TIMER0 + (a))
=======
#define	TA(a)	(MCFSLT_TIMER0 + (a))
>>>>>>> refs/remotes/origin/master

static u32 mcfslt_cycles_per_jiffy;
static u32 mcfslt_cnt;

<<<<<<< HEAD
<<<<<<< HEAD
=======
static irq_handler_t timer_interrupt;

>>>>>>> refs/remotes/origin/cm-10.0
=======
static irq_handler_t timer_interrupt;

>>>>>>> refs/remotes/origin/master
static irqreturn_t mcfslt_tick(int irq, void *dummy)
{
	/* Reset Slice Timer 0 */
	__raw_writel(MCFSLT_SSR_BE | MCFSLT_SSR_TE, TA(MCFSLT_SSR));
	mcfslt_cnt += mcfslt_cycles_per_jiffy;
<<<<<<< HEAD
<<<<<<< HEAD
	return arch_timer_interrupt(irq, dummy);
=======
	return timer_interrupt(irq, dummy);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return timer_interrupt(irq, dummy);
>>>>>>> refs/remotes/origin/master
}

static struct irqaction mcfslt_timer_irq = {
	.name	 = "timer",
<<<<<<< HEAD
	.flags	 = IRQF_DISABLED | IRQF_TIMER,
=======
	.flags	 = IRQF_TIMER,
>>>>>>> refs/remotes/origin/master
	.handler = mcfslt_tick,
};

static cycle_t mcfslt_read_clk(struct clocksource *cs)
{
	unsigned long flags;
<<<<<<< HEAD
<<<<<<< HEAD
	u32 cycles;
	u16 scnt;
=======
	u32 cycles, scnt;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	u32 cycles, scnt;
>>>>>>> refs/remotes/origin/master

	local_irq_save(flags);
	scnt = __raw_readl(TA(MCFSLT_SCNT));
	cycles = mcfslt_cnt;
<<<<<<< HEAD
<<<<<<< HEAD
	local_irq_restore(flags);

	/* subtract because slice timers count down */
	return cycles - scnt;
=======
=======
>>>>>>> refs/remotes/origin/master
	if (__raw_readl(TA(MCFSLT_SSR)) & MCFSLT_SSR_TE) {
		cycles += mcfslt_cycles_per_jiffy;
		scnt = __raw_readl(TA(MCFSLT_SCNT));
	}
	local_irq_restore(flags);

	/* subtract because slice timers count down */
	return cycles + ((mcfslt_cycles_per_jiffy - 1) - scnt);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static struct clocksource mcfslt_clk = {
	.name	= "slt",
	.rating	= 250,
	.read	= mcfslt_read_clk,
<<<<<<< HEAD
<<<<<<< HEAD
	.shift	= 20,
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	.mask	= CLOCKSOURCE_MASK(32),
	.flags	= CLOCK_SOURCE_IS_CONTINUOUS,
};

<<<<<<< HEAD
<<<<<<< HEAD
void hw_timer_init(void)
=======
void hw_timer_init(irq_handler_t handler)
>>>>>>> refs/remotes/origin/cm-10.0
=======
void hw_timer_init(irq_handler_t handler)
>>>>>>> refs/remotes/origin/master
{
	mcfslt_cycles_per_jiffy = MCF_BUSCLK / HZ;
	/*
	 *	The coldfire slice timer (SLT) runs from STCNT to 0 included,
	 *	then STCNT again and so on.  It counts thus actually
	 *	STCNT + 1 steps for 1 tick, not STCNT.  So if you want
	 *	n cycles, initialize STCNT with n - 1.
	 */
	__raw_writel(mcfslt_cycles_per_jiffy - 1, TA(MCFSLT_STCNT));
	__raw_writel(MCFSLT_SCR_RUN | MCFSLT_SCR_IEN | MCFSLT_SCR_TEN,
								TA(MCFSLT_SCR));
	/* initialize mcfslt_cnt knowing that slice timers count down */
	mcfslt_cnt = mcfslt_cycles_per_jiffy;

<<<<<<< HEAD
<<<<<<< HEAD
	setup_irq(MCF_IRQ_TIMER, &mcfslt_timer_irq);

	mcfslt_clk.mult = clocksource_hz2mult(MCF_BUSCLK, mcfslt_clk.shift);
	clocksource_register(&mcfslt_clk);
=======
=======
>>>>>>> refs/remotes/origin/master
	timer_interrupt = handler;
	setup_irq(MCF_IRQ_TIMER, &mcfslt_timer_irq);

	clocksource_register_hz(&mcfslt_clk, MCF_BUSCLK);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_HIGHPROFILE
	mcfslt_profile_init();
#endif
}
