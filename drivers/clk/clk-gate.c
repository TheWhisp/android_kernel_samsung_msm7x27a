/*
 * Copyright (C) 2010-2011 Canonical Ltd <jeremy.kerr@canonical.com>
 * Copyright (C) 2011-2012 Mike Turquette, Linaro Ltd <mturquette@linaro.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Gated clock implementation
 */

#include <linux/clk-provider.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/err.h>
#include <linux/string.h>

/**
 * DOC: basic gatable clock which can gate and ungate it's ouput
 *
 * Traits of this clock:
 * prepare - clk_(un)prepare only ensures parent is (un)prepared
 * enable - clk_enable and clk_disable are functional & control gating
 * rate - inherits rate from parent.  No clk_set_rate support
 * parent - fixed parent.  No clk_set_parent support
 */

#define to_clk_gate(_hw) container_of(_hw, struct clk_gate, hw)

<<<<<<< HEAD
static void clk_gate_set_bit(struct clk_gate *gate)
{
	u32 reg;
	unsigned long flags = 0;

	if (gate->lock)
		spin_lock_irqsave(gate->lock, flags);

	reg = readl(gate->reg);
	reg |= BIT(gate->bit_idx);
	writel(reg, gate->reg);

	if (gate->lock)
		spin_unlock_irqrestore(gate->lock, flags);
}

static void clk_gate_clear_bit(struct clk_gate *gate)
{
	u32 reg;
	unsigned long flags = 0;
=======
/*
 * It works on following logic:
 *
 * For enabling clock, enable = 1
 *	set2dis = 1	-> clear bit	-> set = 0
 *	set2dis = 0	-> set bit	-> set = 1
 *
 * For disabling clock, enable = 0
 *	set2dis = 1	-> set bit	-> set = 1
 *	set2dis = 0	-> clear bit	-> set = 0
 *
 * So, result is always: enable xor set2dis.
 */
static void clk_gate_endisable(struct clk_hw *hw, int enable)
{
	struct clk_gate *gate = to_clk_gate(hw);
	int set = gate->flags & CLK_GATE_SET_TO_DISABLE ? 1 : 0;
	unsigned long flags = 0;
	u32 reg;

	set ^= enable;
>>>>>>> refs/remotes/origin/master

	if (gate->lock)
		spin_lock_irqsave(gate->lock, flags);

<<<<<<< HEAD
	reg = readl(gate->reg);
	reg &= ~BIT(gate->bit_idx);
	writel(reg, gate->reg);
=======
	if (gate->flags & CLK_GATE_HIWORD_MASK) {
		reg = BIT(gate->bit_idx + 16);
		if (set)
			reg |= BIT(gate->bit_idx);
	} else {
		reg = clk_readl(gate->reg);

		if (set)
			reg |= BIT(gate->bit_idx);
		else
			reg &= ~BIT(gate->bit_idx);
	}

	clk_writel(reg, gate->reg);
>>>>>>> refs/remotes/origin/master

	if (gate->lock)
		spin_unlock_irqrestore(gate->lock, flags);
}

static int clk_gate_enable(struct clk_hw *hw)
{
<<<<<<< HEAD
	struct clk_gate *gate = to_clk_gate(hw);

	if (gate->flags & CLK_GATE_SET_TO_DISABLE)
		clk_gate_clear_bit(gate);
	else
		clk_gate_set_bit(gate);

	return 0;
}
EXPORT_SYMBOL_GPL(clk_gate_enable);

static void clk_gate_disable(struct clk_hw *hw)
{
	struct clk_gate *gate = to_clk_gate(hw);

	if (gate->flags & CLK_GATE_SET_TO_DISABLE)
		clk_gate_set_bit(gate);
	else
		clk_gate_clear_bit(gate);
}
EXPORT_SYMBOL_GPL(clk_gate_disable);
=======
	clk_gate_endisable(hw, 1);

	return 0;
}

static void clk_gate_disable(struct clk_hw *hw)
{
	clk_gate_endisable(hw, 0);
}
>>>>>>> refs/remotes/origin/master

static int clk_gate_is_enabled(struct clk_hw *hw)
{
	u32 reg;
	struct clk_gate *gate = to_clk_gate(hw);

<<<<<<< HEAD
	reg = readl(gate->reg);
=======
	reg = clk_readl(gate->reg);
>>>>>>> refs/remotes/origin/master

	/* if a set bit disables this clk, flip it before masking */
	if (gate->flags & CLK_GATE_SET_TO_DISABLE)
		reg ^= BIT(gate->bit_idx);

	reg &= BIT(gate->bit_idx);

	return reg ? 1 : 0;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(clk_gate_is_enabled);

struct clk_ops clk_gate_ops = {
=======

const struct clk_ops clk_gate_ops = {
>>>>>>> refs/remotes/origin/master
	.enable = clk_gate_enable,
	.disable = clk_gate_disable,
	.is_enabled = clk_gate_is_enabled,
};
EXPORT_SYMBOL_GPL(clk_gate_ops);

<<<<<<< HEAD
=======
/**
 * clk_register_gate - register a gate clock with the clock framework
 * @dev: device that is registering this clock
 * @name: name of this clock
 * @parent_name: name of this clock's parent
 * @flags: framework-specific flags for this clock
 * @reg: register address to control gating of this clock
 * @bit_idx: which bit in the register controls gating of this clock
 * @clk_gate_flags: gate-specific flags for this clock
 * @lock: shared register lock for this clock
 */
>>>>>>> refs/remotes/origin/master
struct clk *clk_register_gate(struct device *dev, const char *name,
		const char *parent_name, unsigned long flags,
		void __iomem *reg, u8 bit_idx,
		u8 clk_gate_flags, spinlock_t *lock)
{
	struct clk_gate *gate;
	struct clk *clk;
<<<<<<< HEAD

	gate = kzalloc(sizeof(struct clk_gate), GFP_KERNEL);

	if (!gate) {
		pr_err("%s: could not allocate gated clk\n", __func__);
		return NULL;
	}

=======
	struct clk_init_data init;

	if (clk_gate_flags & CLK_GATE_HIWORD_MASK) {
		if (bit_idx > 16) {
			pr_err("gate bit exceeds LOWORD field\n");
			return ERR_PTR(-EINVAL);
		}
	}

	/* allocate the gate */
	gate = kzalloc(sizeof(struct clk_gate), GFP_KERNEL);
	if (!gate) {
		pr_err("%s: could not allocate gated clk\n", __func__);
		return ERR_PTR(-ENOMEM);
	}

	init.name = name;
	init.ops = &clk_gate_ops;
	init.flags = flags | CLK_IS_BASIC;
	init.parent_names = (parent_name ? &parent_name: NULL);
	init.num_parents = (parent_name ? 1 : 0);

>>>>>>> refs/remotes/origin/master
	/* struct clk_gate assignments */
	gate->reg = reg;
	gate->bit_idx = bit_idx;
	gate->flags = clk_gate_flags;
	gate->lock = lock;
<<<<<<< HEAD

	if (parent_name) {
		gate->parent[0] = kstrdup(parent_name, GFP_KERNEL);
		if (!gate->parent[0])
			goto out;
	}

	clk = clk_register(dev, name,
			&clk_gate_ops, &gate->hw,
			gate->parent,
			(parent_name ? 1 : 0),
			flags);
	if (clk)
		return clk;
out:
	kfree(gate->parent[0]);
	kfree(gate);

	return NULL;
}
=======
	gate->hw.init = &init;

	clk = clk_register(dev, &gate->hw);

	if (IS_ERR(clk))
		kfree(gate);

	return clk;
}
EXPORT_SYMBOL_GPL(clk_register_gate);
>>>>>>> refs/remotes/origin/master
