/*
 * arch/arm/mach-tegra/include/mach/clk.h
 *
 * Copyright (C) 2010 Google, Inc.
 *
 * Author:
 *	Erik Gilling <konkers@google.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __MACH_CLK_H
#define __MACH_CLK_H

struct clk;

<<<<<<< HEAD
=======
enum tegra_clk_ex_param {
	TEGRA_CLK_VI_INP_SEL,
	TEGRA_CLK_DTV_INVERT,
	TEGRA_CLK_NAND_PAD_DIV2_ENB,
	TEGRA_CLK_PLLD_CSI_OUT_ENB,
	TEGRA_CLK_PLLD_DSI_OUT_ENB,
	TEGRA_CLK_PLLD_MIPI_MUX_SEL,
};

>>>>>>> refs/remotes/origin/cm-10.0
void tegra_periph_reset_deassert(struct clk *c);
void tegra_periph_reset_assert(struct clk *c);

unsigned long clk_get_rate_all_locked(struct clk *c);
<<<<<<< HEAD
void tegra_sdmmc_tap_delay(struct clk *c, int delay);
=======
void tegra2_sdmmc_tap_delay(struct clk *c, int delay);
int tegra_clk_cfg_ex(struct clk *c, enum tegra_clk_ex_param p, u32 setting);
>>>>>>> refs/remotes/origin/cm-10.0

#endif
