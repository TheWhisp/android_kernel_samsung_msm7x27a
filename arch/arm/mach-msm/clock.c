/* arch/arm/mach-msm/clock.c
 *
 * Copyright (C) 2007 Google, Inc.
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright (c) 2007-2011, The Linux Foundation. All rights reserved.
=======
 * Copyright (c) 2007-2012, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Copyright (c) 2007-2012, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/master
=======
 * Copyright (c) 2007-2012, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-11.0
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 */

<<<<<<< HEAD
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/spinlock.h>
#include <linux/string.h>
#include <linux/module.h>
#include <linux/clk.h>
#include <linux/clkdev.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/dma-mapping.h>

#include "clock.h"

/* Find the voltage level required for a given rate. */
static int find_vdd_level(struct clk *clk, unsigned long rate)
=======
#include <linux/list.h>
#include <trace/events/power.h>

#include "clock.h"

=======
#include <linux/list.h>
#include <trace/events/power.h>

#include "clock.h"

>>>>>>> refs/remotes/origin/cm-11.0
struct handoff_clk {
	struct list_head list;
	struct clk *clk;
};
static LIST_HEAD(handoff_list);

/* Find the voltage level required for a given rate. */
int find_vdd_level(struct clk *clk, unsigned long rate)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
{
	int level;

	for (level = 0; level < ARRAY_SIZE(clk->fmax); level++)
		if (rate <= clk->fmax[level])
			break;

	if (level == ARRAY_SIZE(clk->fmax)) {
		pr_err("Rate %lu for %s is greater than highest Fmax\n", rate,
			clk->dbg_name);
		return -EINVAL;
	}

	return level;
}

/* Update voltage level given the current votes. */
static int update_vdd(struct clk_vdd_class *vdd_class)
{
	int level, rc;

	for (level = ARRAY_SIZE(vdd_class->level_votes)-1; level > 0; level--)
		if (vdd_class->level_votes[level])
			break;

	if (level == vdd_class->cur_level)
		return 0;

	rc = vdd_class->set_vdd(vdd_class, level);
	if (!rc)
		vdd_class->cur_level = level;

	return rc;
}

/* Vote for a voltage level. */
int vote_vdd_level(struct clk_vdd_class *vdd_class, int level)
{
	unsigned long flags;
	int rc;

	spin_lock_irqsave(&vdd_class->lock, flags);
	vdd_class->level_votes[level]++;
	rc = update_vdd(vdd_class);
	if (rc)
		vdd_class->level_votes[level]--;
	spin_unlock_irqrestore(&vdd_class->lock, flags);

	return rc;
}

/* Remove vote for a voltage level. */
int unvote_vdd_level(struct clk_vdd_class *vdd_class, int level)
{
	unsigned long flags;
	int rc = 0;

	spin_lock_irqsave(&vdd_class->lock, flags);
	if (WARN(!vdd_class->level_votes[level],
			"Reference counts are incorrect for %s level %d\n",
			vdd_class->class_name, level))
		goto out;
	vdd_class->level_votes[level]--;
	rc = update_vdd(vdd_class);
	if (rc)
		vdd_class->level_votes[level]++;
out:
	spin_unlock_irqrestore(&vdd_class->lock, flags);
	return rc;
}

/* Vote for a voltage level corresponding to a clock's rate. */
static int vote_rate_vdd(struct clk *clk, unsigned long rate)
{
	int level;

	if (!clk->vdd_class)
		return 0;

	level = find_vdd_level(clk, rate);
	if (level < 0)
		return level;

	return vote_vdd_level(clk->vdd_class, level);
}

/* Remove vote for a voltage level corresponding to a clock's rate. */
static void unvote_rate_vdd(struct clk *clk, unsigned long rate)
{
	int level;

	if (!clk->vdd_class)
		return;

	level = find_vdd_level(clk, rate);
	if (level < 0)
		return;

	unvote_vdd_level(clk->vdd_class, level);
}

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_CLOCK_MAP
static unsigned clock_count;
unsigned long *clock_enable_map;

static void clk_log_map_init(size_t count)
{
	dma_addr_t addr;
	clock_enable_map = dma_alloc_coherent(NULL, BITS_TO_LONGS(count), &addr,
					      GFP_KERNEL);
}

static void clk_log_map_register(struct clk *clk)
{
	if (!clk->id)
		clk->id = clock_count++;
}

static void clk_log_map_enable(struct clk *clk)
{
	__set_bit(clk->id, clock_enable_map);
}

static void clk_log_map_disable(struct clk *clk)
{
	__clear_bit(clk->id, clock_enable_map);
}
#else
static void clk_log_map_init(size_t count) { }
static void clk_log_map_register(struct clk *clk) { }
static void clk_log_map_enable(struct clk *clk) { }
static void clk_log_map_disable(struct clk *clk) { }
#endif
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
int clk_prepare(struct clk *clk)
{
	int ret = 0;
	struct clk *parent;

	if (!clk)
		return 0;
	if (IS_ERR(clk))
		return -EINVAL;

	mutex_lock(&clk->prepare_lock);
	if (clk->prepare_count == 0) {
		parent = clk_get_parent(clk);

		ret = clk_prepare(parent);
		if (ret)
			goto out;
		ret = clk_prepare(clk->depends);
		if (ret)
			goto err_prepare_depends;

		if (clk->ops->prepare)
			ret = clk->ops->prepare(clk);
		if (ret)
			goto err_prepare_clock;
	}
	clk->prepare_count++;
out:
	mutex_unlock(&clk->prepare_lock);
	return ret;
err_prepare_clock:
	clk_unprepare(clk->depends);
err_prepare_depends:
	clk_unprepare(parent);
	goto out;
}
EXPORT_SYMBOL(clk_prepare);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0

/*
 * Standard clock functions defined in include/linux/clk.h
 */
int clk_enable(struct clk *clk)
{
	int ret = 0;
	unsigned long flags;
	struct clk *parent;

	if (!clk)
		return 0;
<<<<<<< HEAD
<<<<<<< HEAD

	spin_lock_irqsave(&clk->lock, flags);
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (IS_ERR(clk))
		return -EINVAL;

	spin_lock_irqsave(&clk->lock, flags);
	if (WARN(!clk->warned && !clk->prepare_count,
				"%s: Don't call enable on unprepared clocks\n",
				clk->dbg_name))
		clk->warned = true;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (clk->count == 0) {
		parent = clk_get_parent(clk);

		ret = clk_enable(parent);
		if (ret)
			goto err_enable_parent;
		ret = clk_enable(clk->depends);
		if (ret)
			goto err_enable_depends;

		ret = vote_rate_vdd(clk, clk->rate);
		if (ret)
			goto err_vote_vdd;
<<<<<<< HEAD
<<<<<<< HEAD
		if (clk->ops->enable) {
			ret = clk->ops->enable(clk);
			clk_log_map_enable(clk);
		}
		if (ret)
			goto err_enable_clock;
	} else if (clk->flags & CLKFLAG_HANDOFF_RATE) {
		/*
		 * The clock was already enabled by handoff code so there is no
		 * need to enable it again here. Clearing the handoff flag will
		 * prevent the lateinit handoff code from disabling the clock if
		 * a client driver still has it enabled.
		 */
		clk->flags &= ~CLKFLAG_HANDOFF_RATE;
		goto out;
	}
	clk->count++;
out:
=======
		trace_clock_enable(clk->dbg_name, 1, smp_processor_id());
		if (clk->ops->enable)
			ret = clk->ops->enable(clk);
		if (ret)
			goto err_enable_clock;
	}
	clk->count++;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		trace_clock_enable(clk->dbg_name, 1, smp_processor_id());
		if (clk->ops->enable)
			ret = clk->ops->enable(clk);
		if (ret)
			goto err_enable_clock;
	}
	clk->count++;
>>>>>>> refs/remotes/origin/cm-11.0
	spin_unlock_irqrestore(&clk->lock, flags);

	return 0;

err_enable_clock:
	unvote_rate_vdd(clk, clk->rate);
err_vote_vdd:
	clk_disable(clk->depends);
err_enable_depends:
	clk_disable(parent);
err_enable_parent:
	spin_unlock_irqrestore(&clk->lock, flags);
	return ret;
}
EXPORT_SYMBOL(clk_enable);

void clk_disable(struct clk *clk)
{
	unsigned long flags;

<<<<<<< HEAD
<<<<<<< HEAD
	if (!clk)
		return;

	spin_lock_irqsave(&clk->lock, flags);
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (IS_ERR_OR_NULL(clk))
		return;

	spin_lock_irqsave(&clk->lock, flags);
	if (WARN(!clk->warned && !clk->prepare_count,
				"%s: Never called prepare or calling disable "
				"after unprepare\n",
				clk->dbg_name))
		clk->warned = true;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (WARN(clk->count == 0, "%s is unbalanced", clk->dbg_name))
		goto out;
	if (clk->count == 1) {
		struct clk *parent = clk_get_parent(clk);

<<<<<<< HEAD
<<<<<<< HEAD
		if (clk->ops->disable) {
			clk->ops->disable(clk);
			clk_log_map_disable(clk);
		}
=======
		trace_clock_disable(clk->dbg_name, 0, smp_processor_id());
		if (clk->ops->disable)
			clk->ops->disable(clk);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		trace_clock_disable(clk->dbg_name, 0, smp_processor_id());
		if (clk->ops->disable)
			clk->ops->disable(clk);
>>>>>>> refs/remotes/origin/cm-11.0
		unvote_rate_vdd(clk, clk->rate);
		clk_disable(clk->depends);
		clk_disable(parent);
	}
	clk->count--;
out:
	spin_unlock_irqrestore(&clk->lock, flags);
}
EXPORT_SYMBOL(clk_disable);

<<<<<<< HEAD
<<<<<<< HEAD
int clk_reset(struct clk *clk, enum clk_reset_action action)
{
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
void clk_unprepare(struct clk *clk)
{
	if (IS_ERR_OR_NULL(clk))
		return;

	mutex_lock(&clk->prepare_lock);
	if (!clk->prepare_count) {
		if (WARN(!clk->warned, "%s is unbalanced (prepare)",
				clk->dbg_name))
			clk->warned = true;
		goto out;
	}
	if (clk->prepare_count == 1) {
		struct clk *parent = clk_get_parent(clk);

		if (WARN(!clk->warned && clk->count,
			"%s: Don't call unprepare when the clock is enabled\n",
				clk->dbg_name))
			clk->warned = true;

		if (clk->ops->unprepare)
			clk->ops->unprepare(clk);
		clk_unprepare(clk->depends);
		clk_unprepare(parent);
	}
	clk->prepare_count--;
out:
	mutex_unlock(&clk->prepare_lock);
}
EXPORT_SYMBOL(clk_unprepare);

int clk_reset(struct clk *clk, enum clk_reset_action action)
{
	if (IS_ERR_OR_NULL(clk))
		return -EINVAL;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (!clk->ops->reset)
		return -ENOSYS;

	return clk->ops->reset(clk, action);
}
EXPORT_SYMBOL(clk_reset);

unsigned long clk_get_rate(struct clk *clk)
{
<<<<<<< HEAD
<<<<<<< HEAD
	if (!clk->ops->get_rate)
		return 0;

=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (IS_ERR_OR_NULL(clk))
		return 0;

	if (!clk->ops->get_rate)
		return clk->rate;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
	return clk->ops->get_rate(clk);
}
EXPORT_SYMBOL(clk_get_rate);

int clk_set_rate(struct clk *clk, unsigned long rate)
{
	unsigned long start_rate, flags;
<<<<<<< HEAD
<<<<<<< HEAD
	int rc;
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	int rc = 0;

	if (IS_ERR_OR_NULL(clk))
		return -EINVAL;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0

	if (!clk->ops->set_rate)
		return -ENOSYS;

	spin_lock_irqsave(&clk->lock, flags);
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0

	/* Return early if the rate isn't going to change */
	if (clk->rate == rate)
		goto out;

	trace_clock_set_rate(clk->dbg_name, rate, smp_processor_id());
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (clk->count) {
		start_rate = clk->rate;
		/* Enforce vdd requirements for target frequency. */
		rc = vote_rate_vdd(clk, rate);
		if (rc)
			goto err_vote_vdd;
		rc = clk->ops->set_rate(clk, rate);
		if (rc)
			goto err_set_rate;
		/* Release vdd requirements for starting frequency. */
		unvote_rate_vdd(clk, start_rate);
	} else {
		rc = clk->ops->set_rate(clk, rate);
	}

	if (!rc)
		clk->rate = rate;
<<<<<<< HEAD
<<<<<<< HEAD

=======
out:
>>>>>>> refs/remotes/origin/cm-10.0
	spin_unlock_irqrestore(&clk->lock, flags);
	return rc;

=======
out:
	spin_unlock_irqrestore(&clk->lock, flags);
	return rc;

>>>>>>> refs/remotes/origin/cm-11.0
err_set_rate:
	unvote_rate_vdd(clk, rate);
err_vote_vdd:
	spin_unlock_irqrestore(&clk->lock, flags);
	return rc;
}
EXPORT_SYMBOL(clk_set_rate);

long clk_round_rate(struct clk *clk, unsigned long rate)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (IS_ERR_OR_NULL(clk))
		return -EINVAL;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (IS_ERR_OR_NULL(clk))
		return -EINVAL;

>>>>>>> refs/remotes/origin/cm-11.0
	if (!clk->ops->round_rate)
		return -ENOSYS;

	return clk->ops->round_rate(clk, rate);
}
EXPORT_SYMBOL(clk_round_rate);

int clk_set_max_rate(struct clk *clk, unsigned long rate)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (IS_ERR_OR_NULL(clk))
		return -EINVAL;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (IS_ERR_OR_NULL(clk))
		return -EINVAL;

>>>>>>> refs/remotes/origin/cm-11.0
	if (!clk->ops->set_max_rate)
		return -ENOSYS;

	return clk->ops->set_max_rate(clk, rate);
}
EXPORT_SYMBOL(clk_set_max_rate);

int clk_set_parent(struct clk *clk, struct clk *parent)
{
	if (!clk->ops->set_parent)
		return 0;

	return clk->ops->set_parent(clk, parent);
}
EXPORT_SYMBOL(clk_set_parent);

struct clk *clk_get_parent(struct clk *clk)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (IS_ERR_OR_NULL(clk))
		return NULL;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (IS_ERR_OR_NULL(clk))
		return NULL;

>>>>>>> refs/remotes/origin/cm-11.0
	if (!clk->ops->get_parent)
		return NULL;

	return clk->ops->get_parent(clk);
}
EXPORT_SYMBOL(clk_get_parent);

int clk_set_flags(struct clk *clk, unsigned long flags)
{
<<<<<<< HEAD
<<<<<<< HEAD
	if (clk == NULL || IS_ERR(clk))
=======
	if (IS_ERR_OR_NULL(clk))
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (IS_ERR_OR_NULL(clk))
>>>>>>> refs/remotes/origin/cm-11.0
		return -EINVAL;
	if (!clk->ops->set_flags)
		return -ENOSYS;

	return clk->ops->set_flags(clk, flags);
}
EXPORT_SYMBOL(clk_set_flags);

static struct clock_init_data __initdata *clk_init_data;
<<<<<<< HEAD

<<<<<<< HEAD
=======
static enum handoff __init __handoff_clk(struct clk *clk)
{
	enum handoff ret;
	struct handoff_clk *h;
	unsigned long rate;
	int err = 0;

	/*
	 * Tree roots don't have parents, but need to be handed off. So,
	 * terminate recursion by returning "enabled". Also return "enabled"
	 * for clocks with non-zero enable counts since they must have already
	 * been handed off.
	 */
	if (clk == NULL || clk->count)
		return HANDOFF_ENABLED_CLK;

	/* Clocks without handoff functions are assumed to be disabled. */
	if (!clk->ops->handoff || (clk->flags & CLKFLAG_SKIP_HANDOFF))
		return HANDOFF_DISABLED_CLK;

	/*
	 * Handoff functions for children must be called before their parents'
	 * so that the correct parent is returned by the clk_get_parent() below.
	 */
	ret = clk->ops->handoff(clk);
	if (ret == HANDOFF_ENABLED_CLK) {
		ret = __handoff_clk(clk_get_parent(clk));
		if (ret == HANDOFF_ENABLED_CLK) {
			h = kmalloc(sizeof(*h), GFP_KERNEL);
			if (!h) {
				err = -ENOMEM;
				goto out;
			}
			err = clk_prepare_enable(clk);
			if (err)
				goto out;
			rate = clk_get_rate(clk);
			if (rate)
				pr_debug("%s rate=%lu\n", clk->dbg_name, rate);
			h->clk = clk;
			list_add_tail(&h->list, &handoff_list);
		}
	}
out:
	if (err) {
		pr_err("%s handoff failed (%d)\n", clk->dbg_name, err);
		kfree(h);
		ret = HANDOFF_DISABLED_CLK;
	}
	return ret;
}

>>>>>>> refs/remotes/origin/cm-10.0
=======

static enum handoff __init __handoff_clk(struct clk *clk)
{
	enum handoff ret;
	struct handoff_clk *h;
	unsigned long rate;
	int err = 0;

	/*
	 * Tree roots don't have parents, but need to be handed off. So,
	 * terminate recursion by returning "enabled". Also return "enabled"
	 * for clocks with non-zero enable counts since they must have already
	 * been handed off.
	 */
	if (clk == NULL || clk->count)
		return HANDOFF_ENABLED_CLK;

	/* Clocks without handoff functions are assumed to be disabled. */
	if (!clk->ops->handoff || (clk->flags & CLKFLAG_SKIP_HANDOFF))
		return HANDOFF_DISABLED_CLK;

	/*
	 * Handoff functions for children must be called before their parents'
	 * so that the correct parent is returned by the clk_get_parent() below.
	 */
	ret = clk->ops->handoff(clk);
	if (ret == HANDOFF_ENABLED_CLK) {
		ret = __handoff_clk(clk_get_parent(clk));
		if (ret == HANDOFF_ENABLED_CLK) {
			h = kmalloc(sizeof(*h), GFP_KERNEL);
			if (!h) {
				err = -ENOMEM;
				goto out;
			}
			err = clk_prepare_enable(clk);
			if (err)
				goto out;
			rate = clk_get_rate(clk);
			if (rate)
				pr_debug("%s rate=%lu\n", clk->dbg_name, rate);
			h->clk = clk;
			list_add_tail(&h->list, &handoff_list);
		}
	}
out:
	if (err) {
		pr_err("%s handoff failed (%d)\n", clk->dbg_name, err);
		kfree(h);
		ret = HANDOFF_DISABLED_CLK;
	}
	return ret;
}

>>>>>>> refs/remotes/origin/cm-11.0
void __init msm_clock_init(struct clock_init_data *data)
{
	unsigned n;
	struct clk_lookup *clock_tbl;
	size_t num_clocks;
<<<<<<< HEAD
<<<<<<< HEAD

	clk_log_map_init(500);
	clk_init_data = data;
	if (clk_init_data->init)
		clk_init_data->init();
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	struct clk *clk;

	clk_init_data = data;
	if (clk_init_data->pre_init)
		clk_init_data->pre_init();
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0

	clock_tbl = data->table;
	num_clocks = data->size;

	for (n = 0; n < num_clocks; n++) {
<<<<<<< HEAD
<<<<<<< HEAD
		struct clk *clk = clock_tbl[n].clk;
		struct clk *parent = clk_get_parent(clk);
		clk_log_map_register(clk);
		clk_set_parent(clk, parent);
		if (clk->ops->handoff && !(clk->flags & CLKFLAG_HANDOFF_RATE)) {
			if (clk->ops->handoff(clk)) {
				clk->flags |= CLKFLAG_HANDOFF_RATE;
				clk_enable(clk);
			}
		}
	}

	clkdev_add_table(clock_tbl, num_clocks);
}

/*
 * The bootloader and/or AMSS may have left various clocks enabled.
 * Disable any clocks that have not been explicitly enabled by a
 * clk_enable() call and don't have the CLKFLAG_SKIP_AUTO_OFF flag.
 */
static int __init clock_late_init(void)
{
	unsigned n, count = 0;
	unsigned long flags;
	int ret = 0;

	clock_debug_init(clk_init_data);
	for (n = 0; n < clk_init_data->size; n++) {
		struct clk *clk = clk_init_data->table[n].clk;
		bool handoff = false;

		clock_debug_add(clk);
		if (!(clk->flags & CLKFLAG_SKIP_AUTO_OFF)) {
			spin_lock_irqsave(&clk->lock, flags);
			if (!clk->count && clk->ops->auto_off) {
				count++;
				clk->ops->auto_off(clk);
			}
			if (clk->flags & CLKFLAG_HANDOFF_RATE) {
				clk->flags &= ~CLKFLAG_HANDOFF_RATE;
				handoff = true;
			}
			spin_unlock_irqrestore(&clk->lock, flags);
			/*
			 * Calling clk_disable() outside the lock is safe since
			 * it doesn't need to be atomic with the flag change.
			 */
			if (handoff)
				clk_disable(clk);
		}
	}
	pr_info("clock_late_init() disabled %d unused clocks\n", count);
=======
		struct clk *parent;
		clk = clock_tbl[n].clk;
		parent = clk_get_parent(clk);
		if (parent && list_empty(&clk->siblings))
			list_add(&clk->siblings, &parent->children);
	}

	/*
	 * Detect and preserve initial clock state until clock_late_init() or
	 * a driver explicitly changes it, whichever is first.
	 */
	for (n = 0; n < num_clocks; n++)
		__handoff_clk(clock_tbl[n].clk);

	clkdev_add_table(clock_tbl, num_clocks);

	if (clk_init_data->post_init)
		clk_init_data->post_init();
}

static int __init clock_late_init(void)
{
	struct handoff_clk *h, *h_temp;
	int n, ret = 0;

	clock_debug_init(clk_init_data);
	for (n = 0; n < clk_init_data->size; n++)
		clock_debug_add(clk_init_data->table[n].clk);

	pr_info("%s: Removing enables held for handed-off clocks\n", __func__);
	list_for_each_entry_safe(h, h_temp, &handoff_list, list) {
		clk_disable_unprepare(h->clk);
		list_del(&h->list);
		kfree(h);
	}

>>>>>>> refs/remotes/origin/cm-10.0
=======
		struct clk *parent;
		clk = clock_tbl[n].clk;
		parent = clk_get_parent(clk);
		if (parent && list_empty(&clk->siblings))
			list_add(&clk->siblings, &parent->children);
	}

	/*
	 * Detect and preserve initial clock state until clock_late_init() or
	 * a driver explicitly changes it, whichever is first.
	 */
	for (n = 0; n < num_clocks; n++)
		__handoff_clk(clock_tbl[n].clk);

	clkdev_add_table(clock_tbl, num_clocks);

	if (clk_init_data->post_init)
		clk_init_data->post_init();
}

static int __init clock_late_init(void)
{
	struct handoff_clk *h, *h_temp;
	int n, ret = 0;

	clock_debug_init(clk_init_data);
	for (n = 0; n < clk_init_data->size; n++)
		clock_debug_add(clk_init_data->table[n].clk);

	pr_info("%s: Removing enables held for handed-off clocks\n", __func__);
	list_for_each_entry_safe(h, h_temp, &handoff_list, list) {
		clk_disable_unprepare(h->clk);
		list_del(&h->list);
		kfree(h);
	}

>>>>>>> refs/remotes/origin/cm-11.0
	if (clk_init_data->late_init)
		ret = clk_init_data->late_init();
	return ret;
}
late_initcall(clock_late_init);
<<<<<<< HEAD
=======
#include <linux/clk-provider.h>
#include <linux/module.h>

#include "clock.h"

int clk_reset(struct clk *clk, enum clk_reset_action action)
{
	struct clk_hw *hw = __clk_get_hw(clk);
	struct msm_clk *m = to_msm_clk(hw);
	return m->reset(hw, action);
}
EXPORT_SYMBOL(clk_reset);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
