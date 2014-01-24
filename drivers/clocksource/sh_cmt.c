/*
 * SuperH Timer Support - CMT
 *
 *  Copyright (C) 2008 Magnus Damm
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/io.h>
#include <linux/clk.h>
#include <linux/irq.h>
#include <linux/err.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/delay.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/delay.h>
>>>>>>> refs/remotes/origin/master
#include <linux/clocksource.h>
#include <linux/clockchips.h>
#include <linux/sh_timer.h>
#include <linux/slab.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/module.h>
#include <linux/pm_domain.h>
>>>>>>> refs/remotes/origin/cm-10.0

struct sh_cmt_priv {
	void __iomem *mapbase;
=======
#include <linux/module.h>
#include <linux/pm_domain.h>
#include <linux/pm_runtime.h>

struct sh_cmt_priv {
	void __iomem *mapbase;
	void __iomem *mapbase_str;
>>>>>>> refs/remotes/origin/master
	struct clk *clk;
	unsigned long width; /* 16 or 32 bit version of hardware block */
	unsigned long overflow_bit;
	unsigned long clear_bits;
	struct irqaction irqaction;
	struct platform_device *pdev;

	unsigned long flags;
	unsigned long match_value;
	unsigned long next_match_value;
	unsigned long max_match_value;
	unsigned long rate;
<<<<<<< HEAD
	spinlock_t lock;
	struct clock_event_device ced;
	struct clocksource cs;
	unsigned long total_cycles;
};

static DEFINE_SPINLOCK(sh_cmt_lock);

#define CMSTR -1 /* shared register */
=======
	raw_spinlock_t lock;
	struct clock_event_device ced;
	struct clocksource cs;
	unsigned long total_cycles;
	bool cs_enabled;

	/* callbacks for CMSTR and CMCSR access */
	unsigned long (*read_control)(void __iomem *base, unsigned long offs);
	void (*write_control)(void __iomem *base, unsigned long offs,
			      unsigned long value);

	/* callbacks for CMCNT and CMCOR access */
	unsigned long (*read_count)(void __iomem *base, unsigned long offs);
	void (*write_count)(void __iomem *base, unsigned long offs,
			    unsigned long value);
};

/* Examples of supported CMT timer register layouts and I/O access widths:
 *
 * "16-bit counter and 16-bit control" as found on sh7263:
 * CMSTR 0xfffec000 16-bit
 * CMCSR 0xfffec002 16-bit
 * CMCNT 0xfffec004 16-bit
 * CMCOR 0xfffec006 16-bit
 *
 * "32-bit counter and 16-bit control" as found on sh7372, sh73a0, r8a7740:
 * CMSTR 0xffca0000 16-bit
 * CMCSR 0xffca0060 16-bit
 * CMCNT 0xffca0064 32-bit
 * CMCOR 0xffca0068 32-bit
 *
 * "32-bit counter and 32-bit control" as found on r8a73a4 and r8a7790:
 * CMSTR 0xffca0500 32-bit
 * CMCSR 0xffca0510 32-bit
 * CMCNT 0xffca0514 32-bit
 * CMCOR 0xffca0518 32-bit
 */

static unsigned long sh_cmt_read16(void __iomem *base, unsigned long offs)
{
	return ioread16(base + (offs << 1));
}

static unsigned long sh_cmt_read32(void __iomem *base, unsigned long offs)
{
	return ioread32(base + (offs << 2));
}

static void sh_cmt_write16(void __iomem *base, unsigned long offs,
			   unsigned long value)
{
	iowrite16(value, base + (offs << 1));
}

static void sh_cmt_write32(void __iomem *base, unsigned long offs,
			   unsigned long value)
{
	iowrite32(value, base + (offs << 2));
}

>>>>>>> refs/remotes/origin/master
#define CMCSR 0 /* channel register */
#define CMCNT 1 /* channel register */
#define CMCOR 2 /* channel register */

<<<<<<< HEAD
static inline unsigned long sh_cmt_read(struct sh_cmt_priv *p, int reg_nr)
{
	struct sh_timer_config *cfg = p->pdev->dev.platform_data;
	void __iomem *base = p->mapbase;
	unsigned long offs;

	if (reg_nr == CMSTR) {
		offs = 0;
		base -= cfg->channel_offset;
	} else
		offs = reg_nr;

	if (p->width == 16)
		offs <<= 1;
	else {
		offs <<= 2;
		if ((reg_nr == CMCNT) || (reg_nr == CMCOR))
			return ioread32(base + offs);
	}

	return ioread16(base + offs);
}

static inline void sh_cmt_write(struct sh_cmt_priv *p, int reg_nr,
				unsigned long value)
{
	struct sh_timer_config *cfg = p->pdev->dev.platform_data;
	void __iomem *base = p->mapbase;
	unsigned long offs;

	if (reg_nr == CMSTR) {
		offs = 0;
		base -= cfg->channel_offset;
	} else
		offs = reg_nr;

	if (p->width == 16)
		offs <<= 1;
	else {
		offs <<= 2;
		if ((reg_nr == CMCNT) || (reg_nr == CMCOR)) {
			iowrite32(value, base + offs);
			return;
		}
	}

	iowrite16(value, base + offs);
=======
static inline unsigned long sh_cmt_read_cmstr(struct sh_cmt_priv *p)
{
	return p->read_control(p->mapbase_str, 0);
}

static inline unsigned long sh_cmt_read_cmcsr(struct sh_cmt_priv *p)
{
	return p->read_control(p->mapbase, CMCSR);
}

static inline unsigned long sh_cmt_read_cmcnt(struct sh_cmt_priv *p)
{
	return p->read_count(p->mapbase, CMCNT);
}

static inline void sh_cmt_write_cmstr(struct sh_cmt_priv *p,
				      unsigned long value)
{
	p->write_control(p->mapbase_str, 0, value);
}

static inline void sh_cmt_write_cmcsr(struct sh_cmt_priv *p,
				      unsigned long value)
{
	p->write_control(p->mapbase, CMCSR, value);
}

static inline void sh_cmt_write_cmcnt(struct sh_cmt_priv *p,
				      unsigned long value)
{
	p->write_count(p->mapbase, CMCNT, value);
}

static inline void sh_cmt_write_cmcor(struct sh_cmt_priv *p,
				      unsigned long value)
{
	p->write_count(p->mapbase, CMCOR, value);
>>>>>>> refs/remotes/origin/master
}

static unsigned long sh_cmt_get_counter(struct sh_cmt_priv *p,
					int *has_wrapped)
{
	unsigned long v1, v2, v3;
	int o1, o2;

<<<<<<< HEAD
	o1 = sh_cmt_read(p, CMCSR) & p->overflow_bit;
=======
	o1 = sh_cmt_read_cmcsr(p) & p->overflow_bit;
>>>>>>> refs/remotes/origin/master

	/* Make sure the timer value is stable. Stolen from acpi_pm.c */
	do {
		o2 = o1;
<<<<<<< HEAD
		v1 = sh_cmt_read(p, CMCNT);
		v2 = sh_cmt_read(p, CMCNT);
		v3 = sh_cmt_read(p, CMCNT);
		o1 = sh_cmt_read(p, CMCSR) & p->overflow_bit;
=======
		v1 = sh_cmt_read_cmcnt(p);
		v2 = sh_cmt_read_cmcnt(p);
		v3 = sh_cmt_read_cmcnt(p);
		o1 = sh_cmt_read_cmcsr(p) & p->overflow_bit;
>>>>>>> refs/remotes/origin/master
	} while (unlikely((o1 != o2) || (v1 > v2 && v1 < v3)
			  || (v2 > v3 && v2 < v1) || (v3 > v1 && v3 < v2)));

	*has_wrapped = o1;
	return v2;
}

<<<<<<< HEAD
=======
static DEFINE_RAW_SPINLOCK(sh_cmt_lock);
>>>>>>> refs/remotes/origin/master

static void sh_cmt_start_stop_ch(struct sh_cmt_priv *p, int start)
{
	struct sh_timer_config *cfg = p->pdev->dev.platform_data;
	unsigned long flags, value;

	/* start stop register shared by multiple timer channels */
<<<<<<< HEAD
	spin_lock_irqsave(&sh_cmt_lock, flags);
	value = sh_cmt_read(p, CMSTR);
=======
	raw_spin_lock_irqsave(&sh_cmt_lock, flags);
	value = sh_cmt_read_cmstr(p);
>>>>>>> refs/remotes/origin/master

	if (start)
		value |= 1 << cfg->timer_bit;
	else
		value &= ~(1 << cfg->timer_bit);

<<<<<<< HEAD
	sh_cmt_write(p, CMSTR, value);
	spin_unlock_irqrestore(&sh_cmt_lock, flags);
=======
	sh_cmt_write_cmstr(p, value);
	raw_spin_unlock_irqrestore(&sh_cmt_lock, flags);
>>>>>>> refs/remotes/origin/master
}

static int sh_cmt_enable(struct sh_cmt_priv *p, unsigned long *rate)
{
<<<<<<< HEAD
<<<<<<< HEAD
	int ret;
=======
	int k, ret;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	int k, ret;

	pm_runtime_get_sync(&p->pdev->dev);
	dev_pm_syscore_device(&p->pdev->dev, true);
>>>>>>> refs/remotes/origin/master

	/* enable clock */
	ret = clk_enable(p->clk);
	if (ret) {
		dev_err(&p->pdev->dev, "cannot enable clock\n");
<<<<<<< HEAD
<<<<<<< HEAD
		return ret;
=======
		goto err0;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		goto err0;
>>>>>>> refs/remotes/origin/master
	}

	/* make sure channel is disabled */
	sh_cmt_start_stop_ch(p, 0);

	/* configure channel, periodic mode and maximum timeout */
	if (p->width == 16) {
		*rate = clk_get_rate(p->clk) / 512;
<<<<<<< HEAD
		sh_cmt_write(p, CMCSR, 0x43);
	} else {
		*rate = clk_get_rate(p->clk) / 8;
		sh_cmt_write(p, CMCSR, 0x01a4);
	}

	sh_cmt_write(p, CMCOR, 0xffffffff);
	sh_cmt_write(p, CMCNT, 0);

<<<<<<< HEAD
	/* enable channel */
	sh_cmt_start_stop_ch(p, 1);
	return 0;
=======
=======
		sh_cmt_write_cmcsr(p, 0x43);
	} else {
		*rate = clk_get_rate(p->clk) / 8;
		sh_cmt_write_cmcsr(p, 0x01a4);
	}

	sh_cmt_write_cmcor(p, 0xffffffff);
	sh_cmt_write_cmcnt(p, 0);

>>>>>>> refs/remotes/origin/master
	/*
	 * According to the sh73a0 user's manual, as CMCNT can be operated
	 * only by the RCLK (Pseudo 32 KHz), there's one restriction on
	 * modifying CMCNT register; two RCLK cycles are necessary before
	 * this register is either read or any modification of the value
	 * it holds is reflected in the LSI's actual operation.
	 *
	 * While at it, we're supposed to clear out the CMCNT as of this
	 * moment, so make sure it's processed properly here.  This will
	 * take RCLKx2 at maximum.
	 */
	for (k = 0; k < 100; k++) {
<<<<<<< HEAD
		if (!sh_cmt_read(p, CMCNT))
=======
		if (!sh_cmt_read_cmcnt(p))
>>>>>>> refs/remotes/origin/master
			break;
		udelay(1);
	}

<<<<<<< HEAD
	if (sh_cmt_read(p, CMCNT)) {
=======
	if (sh_cmt_read_cmcnt(p)) {
>>>>>>> refs/remotes/origin/master
		dev_err(&p->pdev->dev, "cannot clear CMCNT\n");
		ret = -ETIMEDOUT;
		goto err1;
	}

	/* enable channel */
	sh_cmt_start_stop_ch(p, 1);
	return 0;
 err1:
	/* stop clock */
	clk_disable(p->clk);

 err0:
	return ret;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static void sh_cmt_disable(struct sh_cmt_priv *p)
{
	/* disable channel */
	sh_cmt_start_stop_ch(p, 0);

	/* disable interrupts in CMT block */
<<<<<<< HEAD
	sh_cmt_write(p, CMCSR, 0);

	/* stop clock */
	clk_disable(p->clk);
=======
	sh_cmt_write_cmcsr(p, 0);

	/* stop clock */
	clk_disable(p->clk);

	dev_pm_syscore_device(&p->pdev->dev, false);
	pm_runtime_put(&p->pdev->dev);
>>>>>>> refs/remotes/origin/master
}

/* private flags */
#define FLAG_CLOCKEVENT (1 << 0)
#define FLAG_CLOCKSOURCE (1 << 1)
#define FLAG_REPROGRAM (1 << 2)
#define FLAG_SKIPEVENT (1 << 3)
#define FLAG_IRQCONTEXT (1 << 4)

static void sh_cmt_clock_event_program_verify(struct sh_cmt_priv *p,
					      int absolute)
{
	unsigned long new_match;
	unsigned long value = p->next_match_value;
	unsigned long delay = 0;
	unsigned long now = 0;
	int has_wrapped;

	now = sh_cmt_get_counter(p, &has_wrapped);
	p->flags |= FLAG_REPROGRAM; /* force reprogram */

	if (has_wrapped) {
		/* we're competing with the interrupt handler.
		 *  -> let the interrupt handler reprogram the timer.
		 *  -> interrupt number two handles the event.
		 */
		p->flags |= FLAG_SKIPEVENT;
		return;
	}

	if (absolute)
		now = 0;

	do {
		/* reprogram the timer hardware,
		 * but don't save the new match value yet.
		 */
		new_match = now + value + delay;
		if (new_match > p->max_match_value)
			new_match = p->max_match_value;

<<<<<<< HEAD
		sh_cmt_write(p, CMCOR, new_match);
=======
		sh_cmt_write_cmcor(p, new_match);
>>>>>>> refs/remotes/origin/master

		now = sh_cmt_get_counter(p, &has_wrapped);
		if (has_wrapped && (new_match > p->match_value)) {
			/* we are changing to a greater match value,
			 * so this wrap must be caused by the counter
			 * matching the old value.
			 * -> first interrupt reprograms the timer.
			 * -> interrupt number two handles the event.
			 */
			p->flags |= FLAG_SKIPEVENT;
			break;
		}

		if (has_wrapped) {
			/* we are changing to a smaller match value,
			 * so the wrap must be caused by the counter
			 * matching the new value.
			 * -> save programmed match value.
			 * -> let isr handle the event.
			 */
			p->match_value = new_match;
			break;
		}

		/* be safe: verify hardware settings */
		if (now < new_match) {
			/* timer value is below match value, all good.
			 * this makes sure we won't miss any match events.
			 * -> save programmed match value.
			 * -> let isr handle the event.
			 */
			p->match_value = new_match;
			break;
		}

		/* the counter has reached a value greater
		 * than our new match value. and since the
		 * has_wrapped flag isn't set we must have
		 * programmed a too close event.
		 * -> increase delay and retry.
		 */
		if (delay)
			delay <<= 1;
		else
			delay = 1;

		if (!delay)
			dev_warn(&p->pdev->dev, "too long delay\n");

	} while (delay);
}

static void __sh_cmt_set_next(struct sh_cmt_priv *p, unsigned long delta)
{
	if (delta > p->max_match_value)
		dev_warn(&p->pdev->dev, "delta out of range\n");

	p->next_match_value = delta;
	sh_cmt_clock_event_program_verify(p, 0);
}

static void sh_cmt_set_next(struct sh_cmt_priv *p, unsigned long delta)
{
	unsigned long flags;

<<<<<<< HEAD
	spin_lock_irqsave(&p->lock, flags);
	__sh_cmt_set_next(p, delta);
	spin_unlock_irqrestore(&p->lock, flags);
=======
	raw_spin_lock_irqsave(&p->lock, flags);
	__sh_cmt_set_next(p, delta);
	raw_spin_unlock_irqrestore(&p->lock, flags);
>>>>>>> refs/remotes/origin/master
}

static irqreturn_t sh_cmt_interrupt(int irq, void *dev_id)
{
	struct sh_cmt_priv *p = dev_id;

	/* clear flags */
<<<<<<< HEAD
	sh_cmt_write(p, CMCSR, sh_cmt_read(p, CMCSR) & p->clear_bits);
=======
	sh_cmt_write_cmcsr(p, sh_cmt_read_cmcsr(p) & p->clear_bits);
>>>>>>> refs/remotes/origin/master

	/* update clock source counter to begin with if enabled
	 * the wrap flag should be cleared by the timer specific
	 * isr before we end up here.
	 */
	if (p->flags & FLAG_CLOCKSOURCE)
		p->total_cycles += p->match_value + 1;

	if (!(p->flags & FLAG_REPROGRAM))
		p->next_match_value = p->max_match_value;

	p->flags |= FLAG_IRQCONTEXT;

	if (p->flags & FLAG_CLOCKEVENT) {
		if (!(p->flags & FLAG_SKIPEVENT)) {
			if (p->ced.mode == CLOCK_EVT_MODE_ONESHOT) {
				p->next_match_value = p->max_match_value;
				p->flags |= FLAG_REPROGRAM;
			}

			p->ced.event_handler(&p->ced);
		}
	}

	p->flags &= ~FLAG_SKIPEVENT;

	if (p->flags & FLAG_REPROGRAM) {
		p->flags &= ~FLAG_REPROGRAM;
		sh_cmt_clock_event_program_verify(p, 1);

		if (p->flags & FLAG_CLOCKEVENT)
			if ((p->ced.mode == CLOCK_EVT_MODE_SHUTDOWN)
			    || (p->match_value == p->next_match_value))
				p->flags &= ~FLAG_REPROGRAM;
	}

	p->flags &= ~FLAG_IRQCONTEXT;

	return IRQ_HANDLED;
}

static int sh_cmt_start(struct sh_cmt_priv *p, unsigned long flag)
{
	int ret = 0;
	unsigned long flags;

<<<<<<< HEAD
	spin_lock_irqsave(&p->lock, flags);
=======
	raw_spin_lock_irqsave(&p->lock, flags);
>>>>>>> refs/remotes/origin/master

	if (!(p->flags & (FLAG_CLOCKEVENT | FLAG_CLOCKSOURCE)))
		ret = sh_cmt_enable(p, &p->rate);

	if (ret)
		goto out;
	p->flags |= flag;

	/* setup timeout if no clockevent */
	if ((flag == FLAG_CLOCKSOURCE) && (!(p->flags & FLAG_CLOCKEVENT)))
		__sh_cmt_set_next(p, p->max_match_value);
 out:
<<<<<<< HEAD
	spin_unlock_irqrestore(&p->lock, flags);
=======
	raw_spin_unlock_irqrestore(&p->lock, flags);
>>>>>>> refs/remotes/origin/master

	return ret;
}

static void sh_cmt_stop(struct sh_cmt_priv *p, unsigned long flag)
{
	unsigned long flags;
	unsigned long f;

<<<<<<< HEAD
	spin_lock_irqsave(&p->lock, flags);
=======
	raw_spin_lock_irqsave(&p->lock, flags);
>>>>>>> refs/remotes/origin/master

	f = p->flags & (FLAG_CLOCKEVENT | FLAG_CLOCKSOURCE);
	p->flags &= ~flag;

	if (f && !(p->flags & (FLAG_CLOCKEVENT | FLAG_CLOCKSOURCE)))
		sh_cmt_disable(p);

	/* adjust the timeout to maximum if only clocksource left */
	if ((flag == FLAG_CLOCKEVENT) && (p->flags & FLAG_CLOCKSOURCE))
		__sh_cmt_set_next(p, p->max_match_value);

<<<<<<< HEAD
	spin_unlock_irqrestore(&p->lock, flags);
=======
	raw_spin_unlock_irqrestore(&p->lock, flags);
>>>>>>> refs/remotes/origin/master
}

static struct sh_cmt_priv *cs_to_sh_cmt(struct clocksource *cs)
{
	return container_of(cs, struct sh_cmt_priv, cs);
}

static cycle_t sh_cmt_clocksource_read(struct clocksource *cs)
{
	struct sh_cmt_priv *p = cs_to_sh_cmt(cs);
	unsigned long flags, raw;
	unsigned long value;
	int has_wrapped;

<<<<<<< HEAD
	spin_lock_irqsave(&p->lock, flags);
=======
	raw_spin_lock_irqsave(&p->lock, flags);
>>>>>>> refs/remotes/origin/master
	value = p->total_cycles;
	raw = sh_cmt_get_counter(p, &has_wrapped);

	if (unlikely(has_wrapped))
		raw += p->match_value + 1;
<<<<<<< HEAD
	spin_unlock_irqrestore(&p->lock, flags);
=======
	raw_spin_unlock_irqrestore(&p->lock, flags);
>>>>>>> refs/remotes/origin/master

	return value + raw;
}

static int sh_cmt_clocksource_enable(struct clocksource *cs)
{
	int ret;
	struct sh_cmt_priv *p = cs_to_sh_cmt(cs);

<<<<<<< HEAD
	p->total_cycles = 0;

	ret = sh_cmt_start(p, FLAG_CLOCKSOURCE);
	if (!ret)
		__clocksource_updatefreq_hz(cs, p->rate);
=======
	WARN_ON(p->cs_enabled);

	p->total_cycles = 0;

	ret = sh_cmt_start(p, FLAG_CLOCKSOURCE);
	if (!ret) {
		__clocksource_updatefreq_hz(cs, p->rate);
		p->cs_enabled = true;
	}
>>>>>>> refs/remotes/origin/master
	return ret;
}

static void sh_cmt_clocksource_disable(struct clocksource *cs)
{
<<<<<<< HEAD
	sh_cmt_stop(cs_to_sh_cmt(cs), FLAG_CLOCKSOURCE);
=======
	struct sh_cmt_priv *p = cs_to_sh_cmt(cs);

	WARN_ON(!p->cs_enabled);

	sh_cmt_stop(p, FLAG_CLOCKSOURCE);
	p->cs_enabled = false;
}

static void sh_cmt_clocksource_suspend(struct clocksource *cs)
{
	struct sh_cmt_priv *p = cs_to_sh_cmt(cs);

	sh_cmt_stop(p, FLAG_CLOCKSOURCE);
	pm_genpd_syscore_poweroff(&p->pdev->dev);
>>>>>>> refs/remotes/origin/master
}

static void sh_cmt_clocksource_resume(struct clocksource *cs)
{
<<<<<<< HEAD
	sh_cmt_start(cs_to_sh_cmt(cs), FLAG_CLOCKSOURCE);
=======
	struct sh_cmt_priv *p = cs_to_sh_cmt(cs);

	pm_genpd_syscore_poweron(&p->pdev->dev);
	sh_cmt_start(p, FLAG_CLOCKSOURCE);
>>>>>>> refs/remotes/origin/master
}

static int sh_cmt_register_clocksource(struct sh_cmt_priv *p,
				       char *name, unsigned long rating)
{
	struct clocksource *cs = &p->cs;

	cs->name = name;
	cs->rating = rating;
	cs->read = sh_cmt_clocksource_read;
	cs->enable = sh_cmt_clocksource_enable;
	cs->disable = sh_cmt_clocksource_disable;
<<<<<<< HEAD
	cs->suspend = sh_cmt_clocksource_disable;
=======
	cs->suspend = sh_cmt_clocksource_suspend;
>>>>>>> refs/remotes/origin/master
	cs->resume = sh_cmt_clocksource_resume;
	cs->mask = CLOCKSOURCE_MASK(sizeof(unsigned long) * 8);
	cs->flags = CLOCK_SOURCE_IS_CONTINUOUS;

	dev_info(&p->pdev->dev, "used as clock source\n");

	/* Register with dummy 1 Hz value, gets updated in ->enable() */
	clocksource_register_hz(cs, 1);
	return 0;
}

static struct sh_cmt_priv *ced_to_sh_cmt(struct clock_event_device *ced)
{
	return container_of(ced, struct sh_cmt_priv, ced);
}

static void sh_cmt_clock_event_start(struct sh_cmt_priv *p, int periodic)
{
	struct clock_event_device *ced = &p->ced;

	sh_cmt_start(p, FLAG_CLOCKEVENT);

	/* TODO: calculate good shift from rate and counter bit width */

	ced->shift = 32;
	ced->mult = div_sc(p->rate, NSEC_PER_SEC, ced->shift);
	ced->max_delta_ns = clockevent_delta2ns(p->max_match_value, ced);
	ced->min_delta_ns = clockevent_delta2ns(0x1f, ced);

	if (periodic)
		sh_cmt_set_next(p, ((p->rate + HZ/2) / HZ) - 1);
	else
		sh_cmt_set_next(p, p->max_match_value);
}

static void sh_cmt_clock_event_mode(enum clock_event_mode mode,
				    struct clock_event_device *ced)
{
	struct sh_cmt_priv *p = ced_to_sh_cmt(ced);

	/* deal with old setting first */
	switch (ced->mode) {
	case CLOCK_EVT_MODE_PERIODIC:
	case CLOCK_EVT_MODE_ONESHOT:
		sh_cmt_stop(p, FLAG_CLOCKEVENT);
		break;
	default:
		break;
	}

	switch (mode) {
	case CLOCK_EVT_MODE_PERIODIC:
		dev_info(&p->pdev->dev, "used for periodic clock events\n");
		sh_cmt_clock_event_start(p, 1);
		break;
	case CLOCK_EVT_MODE_ONESHOT:
		dev_info(&p->pdev->dev, "used for oneshot clock events\n");
		sh_cmt_clock_event_start(p, 0);
		break;
	case CLOCK_EVT_MODE_SHUTDOWN:
	case CLOCK_EVT_MODE_UNUSED:
		sh_cmt_stop(p, FLAG_CLOCKEVENT);
		break;
	default:
		break;
	}
}

static int sh_cmt_clock_event_next(unsigned long delta,
				   struct clock_event_device *ced)
{
	struct sh_cmt_priv *p = ced_to_sh_cmt(ced);

	BUG_ON(ced->mode != CLOCK_EVT_MODE_ONESHOT);
	if (likely(p->flags & FLAG_IRQCONTEXT))
		p->next_match_value = delta - 1;
	else
		sh_cmt_set_next(p, delta - 1);

	return 0;
}

<<<<<<< HEAD
=======
static void sh_cmt_clock_event_suspend(struct clock_event_device *ced)
{
	struct sh_cmt_priv *p = ced_to_sh_cmt(ced);

	pm_genpd_syscore_poweroff(&p->pdev->dev);
	clk_unprepare(p->clk);
}

static void sh_cmt_clock_event_resume(struct clock_event_device *ced)
{
	struct sh_cmt_priv *p = ced_to_sh_cmt(ced);

	clk_prepare(p->clk);
	pm_genpd_syscore_poweron(&p->pdev->dev);
}

>>>>>>> refs/remotes/origin/master
static void sh_cmt_register_clockevent(struct sh_cmt_priv *p,
				       char *name, unsigned long rating)
{
	struct clock_event_device *ced = &p->ced;

	memset(ced, 0, sizeof(*ced));

	ced->name = name;
	ced->features = CLOCK_EVT_FEAT_PERIODIC;
	ced->features |= CLOCK_EVT_FEAT_ONESHOT;
	ced->rating = rating;
	ced->cpumask = cpumask_of(0);
	ced->set_next_event = sh_cmt_clock_event_next;
	ced->set_mode = sh_cmt_clock_event_mode;
<<<<<<< HEAD
=======
	ced->suspend = sh_cmt_clock_event_suspend;
	ced->resume = sh_cmt_clock_event_resume;
>>>>>>> refs/remotes/origin/master

	dev_info(&p->pdev->dev, "used for clock events\n");
	clockevents_register_device(ced);
}

static int sh_cmt_register(struct sh_cmt_priv *p, char *name,
			   unsigned long clockevent_rating,
			   unsigned long clocksource_rating)
{
<<<<<<< HEAD
	if (p->width == (sizeof(p->max_match_value) * 8))
		p->max_match_value = ~0;
	else
		p->max_match_value = (1 << p->width) - 1;

	p->match_value = p->max_match_value;
	spin_lock_init(&p->lock);

=======
>>>>>>> refs/remotes/origin/master
	if (clockevent_rating)
		sh_cmt_register_clockevent(p, name, clockevent_rating);

	if (clocksource_rating)
		sh_cmt_register_clocksource(p, name, clocksource_rating);

	return 0;
}

static int sh_cmt_setup(struct sh_cmt_priv *p, struct platform_device *pdev)
{
	struct sh_timer_config *cfg = pdev->dev.platform_data;
<<<<<<< HEAD
	struct resource *res;
=======
	struct resource *res, *res2;
>>>>>>> refs/remotes/origin/master
	int irq, ret;
	ret = -ENXIO;

	memset(p, 0, sizeof(*p));
	p->pdev = pdev;

	if (!cfg) {
		dev_err(&p->pdev->dev, "missing platform data\n");
		goto err0;
	}

<<<<<<< HEAD
	platform_set_drvdata(pdev, p);

=======
>>>>>>> refs/remotes/origin/master
	res = platform_get_resource(p->pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&p->pdev->dev, "failed to get I/O memory\n");
		goto err0;
	}

<<<<<<< HEAD
=======
	/* optional resource for the shared timer start/stop register */
	res2 = platform_get_resource(p->pdev, IORESOURCE_MEM, 1);

>>>>>>> refs/remotes/origin/master
	irq = platform_get_irq(p->pdev, 0);
	if (irq < 0) {
		dev_err(&p->pdev->dev, "failed to get irq\n");
		goto err0;
	}

	/* map memory, let mapbase point to our channel */
	p->mapbase = ioremap_nocache(res->start, resource_size(res));
	if (p->mapbase == NULL) {
		dev_err(&p->pdev->dev, "failed to remap I/O memory\n");
		goto err0;
	}

<<<<<<< HEAD
=======
	/* map second resource for CMSTR */
	p->mapbase_str = ioremap_nocache(res2 ? res2->start :
					 res->start - cfg->channel_offset,
					 res2 ? resource_size(res2) : 2);
	if (p->mapbase_str == NULL) {
		dev_err(&p->pdev->dev, "failed to remap I/O second memory\n");
		goto err1;
	}

>>>>>>> refs/remotes/origin/master
	/* request irq using setup_irq() (too early for request_irq()) */
	p->irqaction.name = dev_name(&p->pdev->dev);
	p->irqaction.handler = sh_cmt_interrupt;
	p->irqaction.dev_id = p;
<<<<<<< HEAD
	p->irqaction.flags = IRQF_DISABLED | IRQF_TIMER | \
			     IRQF_IRQPOLL  | IRQF_NOBALANCING;
=======
	p->irqaction.flags = IRQF_TIMER | IRQF_IRQPOLL | IRQF_NOBALANCING;
>>>>>>> refs/remotes/origin/master

	/* get hold of clock */
	p->clk = clk_get(&p->pdev->dev, "cmt_fck");
	if (IS_ERR(p->clk)) {
		dev_err(&p->pdev->dev, "cannot get clock\n");
		ret = PTR_ERR(p->clk);
<<<<<<< HEAD
		goto err1;
=======
		goto err2;
	}

	ret = clk_prepare(p->clk);
	if (ret < 0)
		goto err3;

	if (res2 && (resource_size(res2) == 4)) {
		/* assume both CMSTR and CMCSR to be 32-bit */
		p->read_control = sh_cmt_read32;
		p->write_control = sh_cmt_write32;
	} else {
		p->read_control = sh_cmt_read16;
		p->write_control = sh_cmt_write16;
>>>>>>> refs/remotes/origin/master
	}

	if (resource_size(res) == 6) {
		p->width = 16;
<<<<<<< HEAD
=======
		p->read_count = sh_cmt_read16;
		p->write_count = sh_cmt_write16;
>>>>>>> refs/remotes/origin/master
		p->overflow_bit = 0x80;
		p->clear_bits = ~0x80;
	} else {
		p->width = 32;
<<<<<<< HEAD
=======
		p->read_count = sh_cmt_read32;
		p->write_count = sh_cmt_write32;
>>>>>>> refs/remotes/origin/master
		p->overflow_bit = 0x8000;
		p->clear_bits = ~0xc000;
	}

<<<<<<< HEAD
=======
	if (p->width == (sizeof(p->max_match_value) * 8))
		p->max_match_value = ~0;
	else
		p->max_match_value = (1 << p->width) - 1;

	p->match_value = p->max_match_value;
	raw_spin_lock_init(&p->lock);

>>>>>>> refs/remotes/origin/master
	ret = sh_cmt_register(p, (char *)dev_name(&p->pdev->dev),
			      cfg->clockevent_rating,
			      cfg->clocksource_rating);
	if (ret) {
		dev_err(&p->pdev->dev, "registration failed\n");
<<<<<<< HEAD
		goto err1;
	}
=======
		goto err4;
	}
	p->cs_enabled = false;
>>>>>>> refs/remotes/origin/master

	ret = setup_irq(irq, &p->irqaction);
	if (ret) {
		dev_err(&p->pdev->dev, "failed to request irq %d\n", irq);
<<<<<<< HEAD
		goto err1;
	}

	return 0;

=======
		goto err4;
	}

	platform_set_drvdata(pdev, p);

	return 0;
err4:
	clk_unprepare(p->clk);
err3:
	clk_put(p->clk);
err2:
	iounmap(p->mapbase_str);
>>>>>>> refs/remotes/origin/master
err1:
	iounmap(p->mapbase);
err0:
	return ret;
}

<<<<<<< HEAD
static int __devinit sh_cmt_probe(struct platform_device *pdev)
{
	struct sh_cmt_priv *p = platform_get_drvdata(pdev);
	int ret;

<<<<<<< HEAD
=======
	if (!is_early_platform_device(pdev))
		pm_genpd_dev_always_on(&pdev->dev, true);

>>>>>>> refs/remotes/origin/cm-10.0
	if (p) {
		dev_info(&pdev->dev, "kept as earlytimer\n");
		return 0;
=======
static int sh_cmt_probe(struct platform_device *pdev)
{
	struct sh_cmt_priv *p = platform_get_drvdata(pdev);
	struct sh_timer_config *cfg = pdev->dev.platform_data;
	int ret;

	if (!is_early_platform_device(pdev)) {
		pm_runtime_set_active(&pdev->dev);
		pm_runtime_enable(&pdev->dev);
	}

	if (p) {
		dev_info(&pdev->dev, "kept as earlytimer\n");
		goto out;
>>>>>>> refs/remotes/origin/master
	}

	p = kmalloc(sizeof(*p), GFP_KERNEL);
	if (p == NULL) {
		dev_err(&pdev->dev, "failed to allocate driver data\n");
		return -ENOMEM;
	}

	ret = sh_cmt_setup(p, pdev);
	if (ret) {
		kfree(p);
<<<<<<< HEAD
		platform_set_drvdata(pdev, NULL);
	}
	return ret;
}

static int __devexit sh_cmt_remove(struct platform_device *pdev)
=======
		pm_runtime_idle(&pdev->dev);
		return ret;
	}
	if (is_early_platform_device(pdev))
		return 0;

 out:
	if (cfg->clockevent_rating || cfg->clocksource_rating)
		pm_runtime_irq_safe(&pdev->dev);
	else
		pm_runtime_idle(&pdev->dev);

	return 0;
}

static int sh_cmt_remove(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	return -EBUSY; /* cannot unregister clockevent and clocksource */
}

static struct platform_driver sh_cmt_device_driver = {
	.probe		= sh_cmt_probe,
<<<<<<< HEAD
	.remove		= __devexit_p(sh_cmt_remove),
=======
	.remove		= sh_cmt_remove,
>>>>>>> refs/remotes/origin/master
	.driver		= {
		.name	= "sh_cmt",
	}
};

static int __init sh_cmt_init(void)
{
	return platform_driver_register(&sh_cmt_device_driver);
}

static void __exit sh_cmt_exit(void)
{
	platform_driver_unregister(&sh_cmt_device_driver);
}

early_platform_init("earlytimer", &sh_cmt_device_driver);
<<<<<<< HEAD
module_init(sh_cmt_init);
=======
subsys_initcall(sh_cmt_init);
>>>>>>> refs/remotes/origin/master
module_exit(sh_cmt_exit);

MODULE_AUTHOR("Magnus Damm");
MODULE_DESCRIPTION("SuperH CMT Timer Driver");
MODULE_LICENSE("GPL v2");
