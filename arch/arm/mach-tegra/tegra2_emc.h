/*
 * Copyright (C) 2011 Google, Inc.
 *
 * Author:
 *	Colin Cross <ccross@android.com>
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

<<<<<<< HEAD
#define TEGRA_EMC_NUM_REGS 46

struct tegra_emc_table {
	unsigned long rate;
	u32 regs[TEGRA_EMC_NUM_REGS];
};

int tegra_emc_set_rate(unsigned long rate);
long tegra_emc_round_rate(unsigned long rate);
void tegra_init_emc(const struct tegra_emc_table *table, int table_size);
=======
#ifndef __MACH_TEGRA_TEGRA2_EMC_H_
#define __MACH_TEGRA_TEGRA2_EMC_H

int tegra_emc_set_rate(unsigned long rate);
long tegra_emc_round_rate(unsigned long rate);

#endif
>>>>>>> refs/remotes/origin/cm-10.0
