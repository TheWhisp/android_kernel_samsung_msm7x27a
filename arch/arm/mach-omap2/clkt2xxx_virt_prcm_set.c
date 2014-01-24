/*
 * OMAP2xxx DVFS virtual clock functions
 *
<<<<<<< HEAD
 * Copyright (C) 2005-2008 Texas Instruments, Inc.
=======
 * Copyright (C) 2005-2008, 2012 Texas Instruments, Inc.
>>>>>>> refs/remotes/origin/master
 * Copyright (C) 2004-2010 Nokia Corporation
 *
 * Contacts:
 * Richard Woodruff <r-woodruff2@ti.com>
 * Paul Walmsley
 *
 * Based on earlier work by Tuukka Tikkanen, Tony Lindgren,
 * Gordon McNutt and RidgeRun, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * XXX Some of this code should be replaceable by the upcoming OPP layer
 * code.  However, some notion of "rate set" is probably still necessary
 * for OMAP2xxx at least.  Rate sets should be generalized so they can be
 * used for any OMAP chip, not just OMAP2xxx.  In particular, Richard Woodruff
 * has in the past expressed a preference to use rate sets for OPP changes,
 * rather than dynamically recalculating the clock tree, so if someone wants
 * this badly enough to write the code to handle it, we should support it
 * as an option.
 */
#undef DEBUG

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/cpufreq.h>
#include <linux/slab.h>

<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <plat/cpu.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <plat/clock.h>
#include <plat/sram.h>
#include <plat/sdrc.h>

#include "clock.h"
#include "clock2xxx.h"
#include "opp2xxx.h"
#include "cm2xxx_3xxx.h"
#include "cm-regbits-24xx.h"
=======
#include "soc.h"
#include "clock.h"
#include "clock2xxx.h"
#include "opp2xxx.h"
#include "cm2xxx.h"
#include "cm-regbits-24xx.h"
#include "sdrc.h"
#include "sram.h"
>>>>>>> refs/remotes/origin/master

const struct prcm_config *curr_prcm_set;
const struct prcm_config *rate_table;

<<<<<<< HEAD
=======
/*
 * sys_ck_rate: the rate of the external high-frequency clock
 * oscillator on the board.  Set by the SoC-specific clock init code.
 * Once set during a boot, will not change.
 */
static unsigned long sys_ck_rate;

>>>>>>> refs/remotes/origin/master
/**
 * omap2_table_mpu_recalc - just return the MPU speed
 * @clk: virt_prcm_set struct clk
 *
 * Set virt_prcm_set's rate to the mpu_speed field of the current PRCM set.
 */
<<<<<<< HEAD
unsigned long omap2_table_mpu_recalc(struct clk *clk)
=======
unsigned long omap2_table_mpu_recalc(struct clk_hw *clk,
				     unsigned long parent_rate)
>>>>>>> refs/remotes/origin/master
{
	return curr_prcm_set->mpu_speed;
}

/*
 * Look for a rate equal or less than the target rate given a configuration set.
 *
 * What's not entirely clear is "which" field represents the key field.
 * Some might argue L3-DDR, others ARM, others IVA. This code is simple and
 * just uses the ARM rates.
 */
<<<<<<< HEAD
long omap2_round_to_table_rate(struct clk *clk, unsigned long rate)
=======
long omap2_round_to_table_rate(struct clk_hw *hw, unsigned long rate,
			       unsigned long *parent_rate)
>>>>>>> refs/remotes/origin/master
{
	const struct prcm_config *ptr;
	long highest_rate;

	highest_rate = -EINVAL;

	for (ptr = rate_table; ptr->mpu_speed; ptr++) {
		if (!(ptr->flags & cpu_mask))
			continue;
<<<<<<< HEAD
		if (ptr->xtal_speed != sclk->rate)
=======
		if (ptr->xtal_speed != sys_ck_rate)
>>>>>>> refs/remotes/origin/master
			continue;

		highest_rate = ptr->mpu_speed;

		/* Can check only after xtal frequency check */
		if (ptr->mpu_speed <= rate)
			break;
	}
	return highest_rate;
}

/* Sets basic clocks based on the specified rate */
<<<<<<< HEAD
int omap2_select_table_rate(struct clk *clk, unsigned long rate)
{
	u32 cur_rate, done_rate, bypass = 0, tmp;
=======
int omap2_select_table_rate(struct clk_hw *hw, unsigned long rate,
			    unsigned long parent_rate)
{
	u32 cur_rate, done_rate, bypass = 0;
>>>>>>> refs/remotes/origin/master
	const struct prcm_config *prcm;
	unsigned long found_speed = 0;
	unsigned long flags;

	for (prcm = rate_table; prcm->mpu_speed; prcm++) {
		if (!(prcm->flags & cpu_mask))
			continue;

<<<<<<< HEAD
		if (prcm->xtal_speed != sclk->rate)
=======
		if (prcm->xtal_speed != sys_ck_rate)
>>>>>>> refs/remotes/origin/master
			continue;

		if (prcm->mpu_speed <= rate) {
			found_speed = prcm->mpu_speed;
			break;
		}
	}

	if (!found_speed) {
		printk(KERN_INFO "Could not set MPU rate to %luMHz\n",
		       rate / 1000000);
		return -EINVAL;
	}

	curr_prcm_set = prcm;
<<<<<<< HEAD
	cur_rate = omap2xxx_clk_get_core_rate(dclk);
=======
	cur_rate = omap2xxx_clk_get_core_rate();
>>>>>>> refs/remotes/origin/master

	if (prcm->dpll_speed == cur_rate / 2) {
		omap2xxx_sdrc_reprogram(CORE_CLK_SRC_DPLL, 1);
	} else if (prcm->dpll_speed == cur_rate * 2) {
		omap2xxx_sdrc_reprogram(CORE_CLK_SRC_DPLL_X2, 1);
	} else if (prcm->dpll_speed != cur_rate) {
		local_irq_save(flags);

		if (prcm->dpll_speed == prcm->xtal_speed)
			bypass = 1;

		if ((prcm->cm_clksel2_pll & OMAP24XX_CORE_CLK_SRC_MASK) ==
		    CORE_CLK_SRC_DPLL_X2)
			done_rate = CORE_CLK_SRC_DPLL_X2;
		else
			done_rate = CORE_CLK_SRC_DPLL;

<<<<<<< HEAD
		/* MPU divider */
		omap2_cm_write_mod_reg(prcm->cm_clksel_mpu, MPU_MOD, CM_CLKSEL);

		/* dsp + iva1 div(2420), iva2.1(2430) */
		omap2_cm_write_mod_reg(prcm->cm_clksel_dsp,
				 OMAP24XX_DSP_MOD, CM_CLKSEL);

		omap2_cm_write_mod_reg(prcm->cm_clksel_gfx, GFX_MOD, CM_CLKSEL);

		/* Major subsystem dividers */
		tmp = omap2_cm_read_mod_reg(CORE_MOD, CM_CLKSEL1) & OMAP24XX_CLKSEL_DSS2_MASK;
		omap2_cm_write_mod_reg(prcm->cm_clksel1_core | tmp, CORE_MOD,
				 CM_CLKSEL1);

		if (cpu_is_omap2430())
			omap2_cm_write_mod_reg(prcm->cm_clksel_mdm,
					 OMAP2430_MDM_MOD, CM_CLKSEL);
=======
		omap2xxx_cm_set_mod_dividers(prcm->cm_clksel_mpu,
					     prcm->cm_clksel_dsp,
					     prcm->cm_clksel_gfx,
					     prcm->cm_clksel1_core,
					     prcm->cm_clksel_mdm);
>>>>>>> refs/remotes/origin/master

		/* x2 to enter omap2xxx_sdrc_init_params() */
		omap2xxx_sdrc_reprogram(CORE_CLK_SRC_DPLL_X2, 1);

		omap2_set_prcm(prcm->cm_clksel1_pll, prcm->base_sdrc_rfr,
			       bypass);

		omap2xxx_sdrc_init_params(omap2xxx_sdrc_dll_is_unlocked());
		omap2xxx_sdrc_reprogram(done_rate, 0);

		local_irq_restore(flags);
	}

	return 0;
}
<<<<<<< HEAD
<<<<<<< HEAD

#ifdef CONFIG_CPU_FREQ
/*
 * Walk PRCM rate table and fillout cpufreq freq_table
 * XXX This should be replaced by an OPP layer in the near future
 */
static struct cpufreq_frequency_table *freq_table;

void omap2_clk_init_cpufreq_table(struct cpufreq_frequency_table **table)
{
	const struct prcm_config *prcm;
	int i = 0;
	int tbl_sz = 0;

	if (!cpu_is_omap24xx())
		return;

	for (prcm = rate_table; prcm->mpu_speed; prcm++) {
		if (!(prcm->flags & cpu_mask))
			continue;
		if (prcm->xtal_speed != sclk->rate)
			continue;

		/* don't put bypass rates in table */
		if (prcm->dpll_speed == prcm->xtal_speed)
			continue;

		tbl_sz++;
	}

	/*
	 * XXX Ensure that we're doing what CPUFreq expects for this error
	 * case and the following one
	 */
	if (tbl_sz == 0) {
		pr_warning("%s: no matching entries in rate_table\n",
			   __func__);
		return;
	}

	/* Include the CPUFREQ_TABLE_END terminator entry */
	tbl_sz++;

	freq_table = kzalloc(sizeof(struct cpufreq_frequency_table) * tbl_sz,
			     GFP_ATOMIC);
	if (!freq_table) {
		pr_err("%s: could not kzalloc frequency table\n", __func__);
		return;
	}

	for (prcm = rate_table; prcm->mpu_speed; prcm++) {
		if (!(prcm->flags & cpu_mask))
			continue;
		if (prcm->xtal_speed != sclk->rate)
			continue;

		/* don't put bypass rates in table */
		if (prcm->dpll_speed == prcm->xtal_speed)
			continue;

		freq_table[i].index = i;
		freq_table[i].frequency = prcm->mpu_speed / 1000;
		i++;
	}

	freq_table[i].index = i;
	freq_table[i].frequency = CPUFREQ_TABLE_END;

	*table = &freq_table[0];
}

void omap2_clk_exit_cpufreq_table(struct cpufreq_frequency_table **table)
{
	if (!cpu_is_omap24xx())
		return;

	kfree(freq_table);
}

#endif
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======

/**
 * omap2xxx_clkt_vps_check_bootloader_rate - determine which of the rate
 * table sets matches the current CORE DPLL hardware rate
 *
 * Check the MPU rate set by bootloader.  Sets the 'curr_prcm_set'
 * global to point to the active rate set when found; otherwise, sets
 * it to NULL.  No return value;
 */
void omap2xxx_clkt_vps_check_bootloader_rates(void)
{
	const struct prcm_config *prcm = NULL;
	unsigned long rate;

	rate = omap2xxx_clk_get_core_rate();
	for (prcm = rate_table; prcm->mpu_speed; prcm++) {
		if (!(prcm->flags & cpu_mask))
			continue;
		if (prcm->xtal_speed != sys_ck_rate)
			continue;
		if (prcm->dpll_speed <= rate)
			break;
	}
	curr_prcm_set = prcm;
}

/**
 * omap2xxx_clkt_vps_late_init - store a copy of the sys_ck rate
 *
 * Store a copy of the sys_ck rate for later use by the OMAP2xxx DVFS
 * code.  (The sys_ck rate does not -- or rather, must not -- change
 * during kernel runtime.)  Must be called after we have a valid
 * sys_ck rate, but before the virt_prcm_set clock rate is
 * recalculated.  No return value.
 */
void omap2xxx_clkt_vps_late_init(void)
{
	struct clk *c;

	c = clk_get(NULL, "sys_ck");
	if (IS_ERR(c)) {
		WARN(1, "could not locate sys_ck\n");
	} else {
		sys_ck_rate = clk_get_rate(c);
		clk_put(c);
	}
}
>>>>>>> refs/remotes/origin/master
