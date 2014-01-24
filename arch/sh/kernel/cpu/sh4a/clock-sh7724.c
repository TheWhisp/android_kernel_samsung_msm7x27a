/*
 * arch/sh/kernel/cpu/sh4a/clock-sh7724.c
 *
 * SH7724 clock framework support
 *
 * Copyright (C) 2009 Magnus Damm
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
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/clk.h>
#include <linux/clkdev.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/clock.h>
#include <asm/hwblk.h>
=======
#include <linux/sh_clk.h>
#include <asm/clock.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/sh_clk.h>
#include <asm/clock.h>
>>>>>>> refs/remotes/origin/master
#include <cpu/sh7724.h>

/* SH7724 registers */
#define FRQCRA		0xa4150000
#define FRQCRB		0xa4150004
#define VCLKCR		0xa4150048
#define FCLKACR		0xa4150008
#define FCLKBCR		0xa415000c
#define IRDACLKCR	0xa4150018
#define PLLCR		0xa4150024
<<<<<<< HEAD
<<<<<<< HEAD
=======
#define MSTPCR0		0xa4150030
#define MSTPCR1		0xa4150034
#define MSTPCR2		0xa4150038
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define MSTPCR0		0xa4150030
#define MSTPCR1		0xa4150034
#define MSTPCR2		0xa4150038
>>>>>>> refs/remotes/origin/master
#define SPUCLKCR	0xa415003c
#define FLLFRQ		0xa4150050
#define LSTATS		0xa4150060

/* Fixed 32 KHz root clock for RTC and Power Management purposes */
static struct clk r_clk = {
	.rate           = 32768,
};

/*
 * Default rate for the root input clock, reset this with clk_set_rate()
 * from the platform code.
 */
static struct clk extal_clk = {
	.rate		= 33333333,
};

/* The fll multiplies the 32khz r_clk, may be used instead of extal */
static unsigned long fll_recalc(struct clk *clk)
{
	unsigned long mult = 0;
	unsigned long div = 1;

	if (__raw_readl(PLLCR) & 0x1000)
		mult = __raw_readl(FLLFRQ) & 0x3ff;

	if (__raw_readl(FLLFRQ) & 0x4000)
		div = 2;

	return (clk->parent->rate * mult) / div;
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct clk_ops fll_clk_ops = {
=======
static struct sh_clk_ops fll_clk_ops = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct sh_clk_ops fll_clk_ops = {
>>>>>>> refs/remotes/origin/master
	.recalc		= fll_recalc,
};

static struct clk fll_clk = {
	.ops		= &fll_clk_ops,
	.parent		= &r_clk,
	.flags		= CLK_ENABLE_ON_INIT,
};

static unsigned long pll_recalc(struct clk *clk)
{
	unsigned long mult = 1;

	if (__raw_readl(PLLCR) & 0x4000)
		mult = (((__raw_readl(FRQCRA) >> 24) & 0x3f) + 1) * 2;

	return clk->parent->rate * mult;
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct clk_ops pll_clk_ops = {
=======
static struct sh_clk_ops pll_clk_ops = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct sh_clk_ops pll_clk_ops = {
>>>>>>> refs/remotes/origin/master
	.recalc		= pll_recalc,
};

static struct clk pll_clk = {
	.ops		= &pll_clk_ops,
	.flags		= CLK_ENABLE_ON_INIT,
};

/* A fixed divide-by-3 block use by the div6 clocks */
static unsigned long div3_recalc(struct clk *clk)
{
	return clk->parent->rate / 3;
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct clk_ops div3_clk_ops = {
=======
static struct sh_clk_ops div3_clk_ops = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct sh_clk_ops div3_clk_ops = {
>>>>>>> refs/remotes/origin/master
	.recalc		= div3_recalc,
};

static struct clk div3_clk = {
	.ops		= &div3_clk_ops,
	.parent		= &pll_clk,
};

<<<<<<< HEAD
<<<<<<< HEAD
/* External input clock (pin name: FSIMCKA/FSIMCKB ) */
=======
/* External input clock (pin name: FSIMCKA/FSIMCKB/DV_CLKI ) */
>>>>>>> refs/remotes/origin/cm-10.0
=======
/* External input clock (pin name: FSIMCKA/FSIMCKB/DV_CLKI ) */
>>>>>>> refs/remotes/origin/master
struct clk sh7724_fsimcka_clk = {
};

struct clk sh7724_fsimckb_clk = {
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
struct clk sh7724_dv_clki = {
};

>>>>>>> refs/remotes/origin/cm-10.0
=======
struct clk sh7724_dv_clki = {
};

>>>>>>> refs/remotes/origin/master
static struct clk *main_clks[] = {
	&r_clk,
	&extal_clk,
	&fll_clk,
	&pll_clk,
	&div3_clk,
	&sh7724_fsimcka_clk,
	&sh7724_fsimckb_clk,
<<<<<<< HEAD
<<<<<<< HEAD
=======
	&sh7724_dv_clki,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	&sh7724_dv_clki,
>>>>>>> refs/remotes/origin/master
};

static void div4_kick(struct clk *clk)
{
	unsigned long value;

	/* set KICK bit in FRQCRA to update hardware setting */
	value = __raw_readl(FRQCRA);
	value |= (1 << 31);
	__raw_writel(value, FRQCRA);
}

static int divisors[] = { 2, 3, 4, 6, 8, 12, 16, 0, 24, 32, 36, 48, 0, 72 };

static struct clk_div_mult_table div4_div_mult_table = {
	.divisors = divisors,
	.nr_divisors = ARRAY_SIZE(divisors),
};

static struct clk_div4_table div4_table = {
	.div_mult_table = &div4_div_mult_table,
	.kick = div4_kick,
};

enum { DIV4_I, DIV4_SH, DIV4_B, DIV4_P, DIV4_M1, DIV4_NR };

#define DIV4(_reg, _bit, _mask, _flags) \
  SH_CLK_DIV4(&pll_clk, _reg, _bit, _mask, _flags)

struct clk div4_clks[DIV4_NR] = {
	[DIV4_I] = DIV4(FRQCRA, 20, 0x2f7d, CLK_ENABLE_ON_INIT),
	[DIV4_SH] = DIV4(FRQCRA, 12, 0x2f7c, CLK_ENABLE_ON_INIT),
	[DIV4_B] = DIV4(FRQCRA, 8, 0x2f7c, CLK_ENABLE_ON_INIT),
	[DIV4_P] = DIV4(FRQCRA, 0, 0x2f7c, 0),
	[DIV4_M1] = DIV4(FRQCRB, 4, 0x2f7c, CLK_ENABLE_ON_INIT),
};

<<<<<<< HEAD
<<<<<<< HEAD
enum { DIV6_V, DIV6_I, DIV6_S, DIV6_NR };

static struct clk div6_clks[DIV6_NR] = {
	[DIV6_V] = SH_CLK_DIV6(&div3_clk, VCLKCR, 0),
	[DIV6_I] = SH_CLK_DIV6(&div3_clk, IRDACLKCR, 0),
	[DIV6_S] = SH_CLK_DIV6(&div3_clk, SPUCLKCR, CLK_ENABLE_ON_INIT),
};

enum { DIV6_FA, DIV6_FB, DIV6_REPARENT_NR };

/* Indices are important - they are the actual src selecting values */
=======
=======
>>>>>>> refs/remotes/origin/master
enum { DIV6_V, DIV6_I, DIV6_S, DIV6_FA, DIV6_FB, DIV6_NR };

/* Indices are important - they are the actual src selecting values */
static struct clk *common_parent[] = {
	[0] = &div3_clk,
	[1] = NULL,
};

static struct clk *vclkcr_parent[8] = {
	[0] = &div3_clk,
	[2] = &sh7724_dv_clki,
	[4] = &extal_clk,
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static struct clk *fclkacr_parent[] = {
	[0] = &div3_clk,
	[1] = NULL,
	[2] = &sh7724_fsimcka_clk,
	[3] = NULL,
};

static struct clk *fclkbcr_parent[] = {
	[0] = &div3_clk,
	[1] = NULL,
	[2] = &sh7724_fsimckb_clk,
	[3] = NULL,
};

<<<<<<< HEAD
<<<<<<< HEAD
static struct clk div6_reparent_clks[DIV6_REPARENT_NR] = {
	[DIV6_FA] = SH_CLK_DIV6_EXT(&div3_clk, FCLKACR, 0,
				      fclkacr_parent, ARRAY_SIZE(fclkacr_parent), 6, 2),
	[DIV6_FB] = SH_CLK_DIV6_EXT(&div3_clk, FCLKBCR, 0,
=======
=======
>>>>>>> refs/remotes/origin/master
static struct clk div6_clks[DIV6_NR] = {
	[DIV6_V] = SH_CLK_DIV6_EXT(VCLKCR, 0,
			vclkcr_parent, ARRAY_SIZE(vclkcr_parent), 12, 3),
	[DIV6_I] = SH_CLK_DIV6_EXT(IRDACLKCR, 0,
			common_parent, ARRAY_SIZE(common_parent), 6, 1),
	[DIV6_S] = SH_CLK_DIV6_EXT(SPUCLKCR, CLK_ENABLE_ON_INIT,
			common_parent, ARRAY_SIZE(common_parent), 6, 1),
	[DIV6_FA] = SH_CLK_DIV6_EXT(FCLKACR, 0,
				      fclkacr_parent, ARRAY_SIZE(fclkacr_parent), 6, 2),
	[DIV6_FB] = SH_CLK_DIV6_EXT(FCLKBCR, 0,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				      fclkbcr_parent, ARRAY_SIZE(fclkbcr_parent), 6, 2),
};

static struct clk mstp_clks[HWBLK_NR] = {
<<<<<<< HEAD
<<<<<<< HEAD
	SH_HWBLK_CLK(HWBLK_TLB, &div4_clks[DIV4_I], CLK_ENABLE_ON_INIT),
	SH_HWBLK_CLK(HWBLK_IC, &div4_clks[DIV4_I], CLK_ENABLE_ON_INIT),
	SH_HWBLK_CLK(HWBLK_OC, &div4_clks[DIV4_I], CLK_ENABLE_ON_INIT),
	SH_HWBLK_CLK(HWBLK_RSMEM, &div4_clks[DIV4_B], CLK_ENABLE_ON_INIT),
	SH_HWBLK_CLK(HWBLK_ILMEM, &div4_clks[DIV4_I], CLK_ENABLE_ON_INIT),
	SH_HWBLK_CLK(HWBLK_L2C, &div4_clks[DIV4_SH], CLK_ENABLE_ON_INIT),
	SH_HWBLK_CLK(HWBLK_FPU, &div4_clks[DIV4_I], CLK_ENABLE_ON_INIT),
	SH_HWBLK_CLK(HWBLK_INTC, &div4_clks[DIV4_P], CLK_ENABLE_ON_INIT),
	SH_HWBLK_CLK(HWBLK_DMAC0, &div4_clks[DIV4_B], 0),
	SH_HWBLK_CLK(HWBLK_SHYWAY, &div4_clks[DIV4_SH], CLK_ENABLE_ON_INIT),
	SH_HWBLK_CLK(HWBLK_HUDI, &div4_clks[DIV4_P], 0),
	SH_HWBLK_CLK(HWBLK_UBC, &div4_clks[DIV4_I], 0),
	SH_HWBLK_CLK(HWBLK_TMU0, &div4_clks[DIV4_P], 0),
	SH_HWBLK_CLK(HWBLK_CMT, &r_clk, 0),
	SH_HWBLK_CLK(HWBLK_RWDT, &r_clk, 0),
	SH_HWBLK_CLK(HWBLK_DMAC1, &div4_clks[DIV4_B], 0),
	SH_HWBLK_CLK(HWBLK_TMU1, &div4_clks[DIV4_P], 0),
	SH_HWBLK_CLK(HWBLK_SCIF0, &div4_clks[DIV4_P], 0),
	SH_HWBLK_CLK(HWBLK_SCIF1, &div4_clks[DIV4_P], 0),
	SH_HWBLK_CLK(HWBLK_SCIF2, &div4_clks[DIV4_P], 0),
	SH_HWBLK_CLK(HWBLK_SCIF3, &div4_clks[DIV4_B], 0),
	SH_HWBLK_CLK(HWBLK_SCIF4, &div4_clks[DIV4_B], 0),
	SH_HWBLK_CLK(HWBLK_SCIF5, &div4_clks[DIV4_B], 0),
	SH_HWBLK_CLK(HWBLK_MSIOF0, &div4_clks[DIV4_B], 0),
	SH_HWBLK_CLK(HWBLK_MSIOF1, &div4_clks[DIV4_B], 0),

	SH_HWBLK_CLK(HWBLK_KEYSC, &r_clk, 0),
	SH_HWBLK_CLK(HWBLK_RTC, &r_clk, 0),
	SH_HWBLK_CLK(HWBLK_IIC0, &div4_clks[DIV4_P], 0),
	SH_HWBLK_CLK(HWBLK_IIC1, &div4_clks[DIV4_P], 0),

	SH_HWBLK_CLK(HWBLK_MMC, &div4_clks[DIV4_B], 0),
	SH_HWBLK_CLK(HWBLK_ETHER, &div4_clks[DIV4_B], 0),
	SH_HWBLK_CLK(HWBLK_ATAPI, &div4_clks[DIV4_B], 0),
	SH_HWBLK_CLK(HWBLK_TPU, &div4_clks[DIV4_B], 0),
	SH_HWBLK_CLK(HWBLK_IRDA, &div4_clks[DIV4_P], 0),
	SH_HWBLK_CLK(HWBLK_TSIF, &div4_clks[DIV4_B], 0),
	SH_HWBLK_CLK(HWBLK_USB1, &div4_clks[DIV4_B], 0),
	SH_HWBLK_CLK(HWBLK_USB0, &div4_clks[DIV4_B], 0),
	SH_HWBLK_CLK(HWBLK_2DG, &div4_clks[DIV4_B], 0),
	SH_HWBLK_CLK(HWBLK_SDHI0, &div4_clks[DIV4_B], 0),
	SH_HWBLK_CLK(HWBLK_SDHI1, &div4_clks[DIV4_B], 0),
	SH_HWBLK_CLK(HWBLK_VEU1, &div4_clks[DIV4_B], 0),
	SH_HWBLK_CLK(HWBLK_CEU1, &div4_clks[DIV4_B], 0),
	SH_HWBLK_CLK(HWBLK_BEU1, &div4_clks[DIV4_B], 0),
	SH_HWBLK_CLK(HWBLK_2DDMAC, &div4_clks[DIV4_SH], 0),
	SH_HWBLK_CLK(HWBLK_SPU, &div4_clks[DIV4_B], 0),
	SH_HWBLK_CLK(HWBLK_JPU, &div4_clks[DIV4_B], 0),
	SH_HWBLK_CLK(HWBLK_VOU, &div4_clks[DIV4_B], 0),
	SH_HWBLK_CLK(HWBLK_BEU0, &div4_clks[DIV4_B], 0),
	SH_HWBLK_CLK(HWBLK_CEU0, &div4_clks[DIV4_B], 0),
	SH_HWBLK_CLK(HWBLK_VEU0, &div4_clks[DIV4_B], 0),
	SH_HWBLK_CLK(HWBLK_VPU, &div4_clks[DIV4_B], 0),
	SH_HWBLK_CLK(HWBLK_LCDC, &div4_clks[DIV4_B], 0),
};

#define CLKDEV_CON_ID(_id, _clk) { .con_id = _id, .clk = _clk }

=======
=======
>>>>>>> refs/remotes/origin/master
	[HWBLK_TLB] = SH_CLK_MSTP32(&div4_clks[DIV4_I],	    MSTPCR0, 31, CLK_ENABLE_ON_INIT),
	[HWBLK_IC] = SH_CLK_MSTP32(&div4_clks[DIV4_I],	    MSTPCR0, 30, CLK_ENABLE_ON_INIT),
	[HWBLK_OC] = SH_CLK_MSTP32(&div4_clks[DIV4_I],	    MSTPCR0, 29, CLK_ENABLE_ON_INIT),
	[HWBLK_RSMEM] = SH_CLK_MSTP32(&div4_clks[DIV4_B],   MSTPCR0, 28, CLK_ENABLE_ON_INIT),
	[HWBLK_ILMEM] = SH_CLK_MSTP32(&div4_clks[DIV4_I],   MSTPCR0, 27, CLK_ENABLE_ON_INIT),
	[HWBLK_L2C] = SH_CLK_MSTP32(&div4_clks[DIV4_SH],    MSTPCR0, 26, CLK_ENABLE_ON_INIT),
	[HWBLK_FPU] = SH_CLK_MSTP32(&div4_clks[DIV4_I],	    MSTPCR0, 24, CLK_ENABLE_ON_INIT),
	[HWBLK_INTC] = SH_CLK_MSTP32(&div4_clks[DIV4_P],    MSTPCR0, 22, CLK_ENABLE_ON_INIT),
	[HWBLK_DMAC0] = SH_CLK_MSTP32(&div4_clks[DIV4_B],   MSTPCR0, 21, 0),
	[HWBLK_SHYWAY] = SH_CLK_MSTP32(&div4_clks[DIV4_SH], MSTPCR0, 20, CLK_ENABLE_ON_INIT),
	[HWBLK_HUDI] = SH_CLK_MSTP32(&div4_clks[DIV4_P],    MSTPCR0, 19, 0),
	[HWBLK_UBC] = SH_CLK_MSTP32(&div4_clks[DIV4_I],     MSTPCR0, 17, 0),
	[HWBLK_TMU0] = SH_CLK_MSTP32(&div4_clks[DIV4_P],    MSTPCR0, 15, 0),
	[HWBLK_CMT] = SH_CLK_MSTP32(&r_clk,		    MSTPCR0, 14, 0),
	[HWBLK_RWDT] = SH_CLK_MSTP32(&r_clk,		    MSTPCR0, 13, 0),
	[HWBLK_DMAC1] = SH_CLK_MSTP32(&div4_clks[DIV4_B],   MSTPCR0, 12, 0),
	[HWBLK_TMU1] = SH_CLK_MSTP32(&div4_clks[DIV4_P],    MSTPCR0, 10, 0),
	[HWBLK_SCIF0] = SH_CLK_MSTP32(&div4_clks[DIV4_P],   MSTPCR0, 9, 0),
	[HWBLK_SCIF1] = SH_CLK_MSTP32(&div4_clks[DIV4_P],   MSTPCR0, 8, 0),
	[HWBLK_SCIF2] = SH_CLK_MSTP32(&div4_clks[DIV4_P],   MSTPCR0, 7, 0),
	[HWBLK_SCIF3] = SH_CLK_MSTP32(&div4_clks[DIV4_B],   MSTPCR0, 6, 0),
	[HWBLK_SCIF4] = SH_CLK_MSTP32(&div4_clks[DIV4_B],   MSTPCR0, 5, 0),
	[HWBLK_SCIF5] = SH_CLK_MSTP32(&div4_clks[DIV4_B],   MSTPCR0, 4, 0),
	[HWBLK_MSIOF0] = SH_CLK_MSTP32(&div4_clks[DIV4_B],  MSTPCR0, 2, 0),
	[HWBLK_MSIOF1] = SH_CLK_MSTP32(&div4_clks[DIV4_B],  MSTPCR0, 1, 0),

	[HWBLK_KEYSC] = SH_CLK_MSTP32(&r_clk,		    MSTPCR1, 12, 0),
	[HWBLK_RTC] = SH_CLK_MSTP32(&r_clk,		    MSTPCR1, 11, 0),
	[HWBLK_IIC0] = SH_CLK_MSTP32(&div4_clks[DIV4_P],    MSTPCR1, 9, 0),
	[HWBLK_IIC1] = SH_CLK_MSTP32(&div4_clks[DIV4_P],    MSTPCR1, 8, 0),

	[HWBLK_MMC] = SH_CLK_MSTP32(&div4_clks[DIV4_B],	    MSTPCR2, 29, 0),
	[HWBLK_ETHER] = SH_CLK_MSTP32(&div4_clks[DIV4_B],   MSTPCR2, 28, 0),
	[HWBLK_ATAPI] = SH_CLK_MSTP32(&div4_clks[DIV4_B],   MSTPCR2, 26, 0),
	[HWBLK_TPU] = SH_CLK_MSTP32(&div4_clks[DIV4_B],	    MSTPCR2, 25, 0),
	[HWBLK_IRDA] = SH_CLK_MSTP32(&div4_clks[DIV4_P],    MSTPCR2, 24, 0),
	[HWBLK_TSIF] = SH_CLK_MSTP32(&div4_clks[DIV4_B],    MSTPCR2, 22, 0),
	[HWBLK_USB1] = SH_CLK_MSTP32(&div4_clks[DIV4_B],    MSTPCR2, 21, 0),
	[HWBLK_USB0] = SH_CLK_MSTP32(&div4_clks[DIV4_B],    MSTPCR2, 20, 0),
	[HWBLK_2DG] = SH_CLK_MSTP32(&div4_clks[DIV4_B],	    MSTPCR2, 19, 0),
	[HWBLK_SDHI0] = SH_CLK_MSTP32(&div4_clks[DIV4_B],   MSTPCR2, 18, 0),
	[HWBLK_SDHI1] = SH_CLK_MSTP32(&div4_clks[DIV4_B],   MSTPCR2, 17, 0),
	[HWBLK_VEU1] = SH_CLK_MSTP32(&div4_clks[DIV4_B],    MSTPCR2, 15, 0),
	[HWBLK_CEU1] = SH_CLK_MSTP32(&div4_clks[DIV4_B],    MSTPCR2, 13, 0),
	[HWBLK_BEU1] = SH_CLK_MSTP32(&div4_clks[DIV4_B],    MSTPCR2, 12, 0),
	[HWBLK_2DDMAC] = SH_CLK_MSTP32(&div4_clks[DIV4_SH], MSTPCR2, 10, 0),
	[HWBLK_SPU] = SH_CLK_MSTP32(&div4_clks[DIV4_B],	    MSTPCR2, 9, 0),
	[HWBLK_JPU] = SH_CLK_MSTP32(&div4_clks[DIV4_B],	    MSTPCR2, 6, 0),
	[HWBLK_VOU] = SH_CLK_MSTP32(&div4_clks[DIV4_B],	    MSTPCR2, 5, 0),
	[HWBLK_BEU0] = SH_CLK_MSTP32(&div4_clks[DIV4_B],    MSTPCR2, 4, 0),
	[HWBLK_CEU0] = SH_CLK_MSTP32(&div4_clks[DIV4_B],    MSTPCR2, 3, 0),
	[HWBLK_VEU0] = SH_CLK_MSTP32(&div4_clks[DIV4_B],    MSTPCR2, 2, 0),
	[HWBLK_VPU] = SH_CLK_MSTP32(&div4_clks[DIV4_B],	    MSTPCR2, 1, 0),
	[HWBLK_LCDC] = SH_CLK_MSTP32(&div4_clks[DIV4_B],    MSTPCR2, 0, 0),
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static struct clk_lookup lookups[] = {
	/* main clocks */
	CLKDEV_CON_ID("rclk", &r_clk),
	CLKDEV_CON_ID("extal", &extal_clk),
	CLKDEV_CON_ID("fll_clk", &fll_clk),
	CLKDEV_CON_ID("pll_clk", &pll_clk),
	CLKDEV_CON_ID("div3_clk", &div3_clk),

	/* DIV4 clocks */
	CLKDEV_CON_ID("cpu_clk", &div4_clks[DIV4_I]),
	CLKDEV_CON_ID("shyway_clk", &div4_clks[DIV4_SH]),
	CLKDEV_CON_ID("bus_clk", &div4_clks[DIV4_B]),
	CLKDEV_CON_ID("peripheral_clk", &div4_clks[DIV4_P]),
	CLKDEV_CON_ID("vpu_clk", &div4_clks[DIV4_M1]),

	/* DIV6 clocks */
	CLKDEV_CON_ID("video_clk", &div6_clks[DIV6_V]),
<<<<<<< HEAD
<<<<<<< HEAD
	CLKDEV_CON_ID("fsia_clk", &div6_reparent_clks[DIV6_FA]),
	CLKDEV_CON_ID("fsib_clk", &div6_reparent_clks[DIV6_FB]),
=======
	CLKDEV_CON_ID("fsia_clk", &div6_clks[DIV6_FA]),
	CLKDEV_CON_ID("fsib_clk", &div6_clks[DIV6_FB]),
>>>>>>> refs/remotes/origin/cm-10.0
=======
	CLKDEV_CON_ID("fsia_clk", &div6_clks[DIV6_FA]),
	CLKDEV_CON_ID("fsib_clk", &div6_clks[DIV6_FB]),
>>>>>>> refs/remotes/origin/master
	CLKDEV_CON_ID("irda_clk", &div6_clks[DIV6_I]),
	CLKDEV_CON_ID("spu_clk", &div6_clks[DIV6_S]),

	/* MSTP clocks */
	CLKDEV_CON_ID("tlb0", &mstp_clks[HWBLK_TLB]),
	CLKDEV_CON_ID("ic0", &mstp_clks[HWBLK_IC]),
	CLKDEV_CON_ID("oc0", &mstp_clks[HWBLK_OC]),
	CLKDEV_CON_ID("rs0", &mstp_clks[HWBLK_RSMEM]),
	CLKDEV_CON_ID("ilmem0", &mstp_clks[HWBLK_ILMEM]),
	CLKDEV_CON_ID("l2c0", &mstp_clks[HWBLK_L2C]),
	CLKDEV_CON_ID("fpu0", &mstp_clks[HWBLK_FPU]),
	CLKDEV_CON_ID("intc0", &mstp_clks[HWBLK_INTC]),
<<<<<<< HEAD
<<<<<<< HEAD
	CLKDEV_CON_ID("dmac0", &mstp_clks[HWBLK_DMAC0]),
	CLKDEV_CON_ID("sh0", &mstp_clks[HWBLK_SHYWAY]),
	CLKDEV_CON_ID("hudi0", &mstp_clks[HWBLK_HUDI]),
	CLKDEV_CON_ID("ubc0", &mstp_clks[HWBLK_UBC]),
	{
		/* TMU0 */
		.dev_id		= "sh_tmu.0",
		.con_id		= "tmu_fck",
		.clk		= &mstp_clks[HWBLK_TMU0],
	}, {
		/* TMU1 */
		.dev_id		= "sh_tmu.1",
		.con_id		= "tmu_fck",
		.clk		= &mstp_clks[HWBLK_TMU0],
	}, {
		/* TMU2 */
		.dev_id		= "sh_tmu.2",
		.con_id		= "tmu_fck",
		.clk		= &mstp_clks[HWBLK_TMU0],
	}, {
		/* TMU3 */
		.dev_id		= "sh_tmu.3",
		.con_id		= "tmu_fck",
		.clk		= &mstp_clks[HWBLK_TMU1],
	},
	CLKDEV_CON_ID("cmt_fck", &mstp_clks[HWBLK_CMT]),
	CLKDEV_CON_ID("rwdt0", &mstp_clks[HWBLK_RWDT]),
	CLKDEV_CON_ID("dmac1", &mstp_clks[HWBLK_DMAC1]),
	{
		/* TMU4 */
		.dev_id		= "sh_tmu.4",
		.con_id		= "tmu_fck",
		.clk		= &mstp_clks[HWBLK_TMU1],
	}, {
		/* TMU5 */
		.dev_id		= "sh_tmu.5",
		.con_id		= "tmu_fck",
		.clk		= &mstp_clks[HWBLK_TMU1],
	}, {
		/* SCIF0 */
		.dev_id		= "sh-sci.0",
		.con_id		= "sci_fck",
		.clk		= &mstp_clks[HWBLK_SCIF0],
	}, {
		/* SCIF1 */
		.dev_id		= "sh-sci.1",
		.con_id		= "sci_fck",
		.clk		= &mstp_clks[HWBLK_SCIF1],
	}, {
		/* SCIF2 */
		.dev_id		= "sh-sci.2",
		.con_id		= "sci_fck",
		.clk		= &mstp_clks[HWBLK_SCIF2],
	}, {
		/* SCIF3 */
		.dev_id		= "sh-sci.3",
		.con_id		= "sci_fck",
		.clk		= &mstp_clks[HWBLK_SCIF3],
	}, {
		/* SCIF4 */
		.dev_id		= "sh-sci.4",
		.con_id		= "sci_fck",
		.clk		= &mstp_clks[HWBLK_SCIF4],
	}, {
		/* SCIF5 */
		.dev_id		= "sh-sci.5",
		.con_id		= "sci_fck",
		.clk		= &mstp_clks[HWBLK_SCIF5],
	},
	CLKDEV_CON_ID("msiof0", &mstp_clks[HWBLK_MSIOF0]),
	CLKDEV_CON_ID("msiof1", &mstp_clks[HWBLK_MSIOF1]),
	CLKDEV_CON_ID("keysc0", &mstp_clks[HWBLK_KEYSC]),
	CLKDEV_CON_ID("rtc0", &mstp_clks[HWBLK_RTC]),
	CLKDEV_CON_ID("i2c0", &mstp_clks[HWBLK_IIC0]),
	CLKDEV_CON_ID("i2c1", &mstp_clks[HWBLK_IIC1]),
	CLKDEV_CON_ID("mmc0", &mstp_clks[HWBLK_MMC]),
	CLKDEV_CON_ID("eth0", &mstp_clks[HWBLK_ETHER]),
=======
=======
>>>>>>> refs/remotes/origin/master
	CLKDEV_DEV_ID("sh-dma-engine.0", &mstp_clks[HWBLK_DMAC0]),
	CLKDEV_CON_ID("sh0", &mstp_clks[HWBLK_SHYWAY]),
	CLKDEV_CON_ID("hudi0", &mstp_clks[HWBLK_HUDI]),
	CLKDEV_CON_ID("ubc0", &mstp_clks[HWBLK_UBC]),

	CLKDEV_ICK_ID("tmu_fck", "sh_tmu.0", &mstp_clks[HWBLK_TMU0]),
	CLKDEV_ICK_ID("tmu_fck", "sh_tmu.1", &mstp_clks[HWBLK_TMU0]),
	CLKDEV_ICK_ID("tmu_fck", "sh_tmu.2", &mstp_clks[HWBLK_TMU0]),
	CLKDEV_ICK_ID("tmu_fck", "sh_tmu.3", &mstp_clks[HWBLK_TMU1]),

	CLKDEV_CON_ID("cmt_fck", &mstp_clks[HWBLK_CMT]),
	CLKDEV_DEV_ID("sh-wdt.0", &mstp_clks[HWBLK_RWDT]),
	CLKDEV_DEV_ID("sh-dma-engine.1", &mstp_clks[HWBLK_DMAC1]),

	CLKDEV_ICK_ID("tmu_fck", "sh_tmu.4", &mstp_clks[HWBLK_TMU1]),
	CLKDEV_ICK_ID("tmu_fck", "sh_tmu.5", &mstp_clks[HWBLK_TMU1]),
	CLKDEV_DEV_ID("sh-sci.0", &mstp_clks[HWBLK_SCIF0]),
	CLKDEV_DEV_ID("sh-sci.1", &mstp_clks[HWBLK_SCIF1]),
	CLKDEV_DEV_ID("sh-sci.2", &mstp_clks[HWBLK_SCIF2]),
	CLKDEV_DEV_ID("sh-sci.3", &mstp_clks[HWBLK_SCIF3]),
	CLKDEV_DEV_ID("sh-sci.4", &mstp_clks[HWBLK_SCIF4]),
	CLKDEV_DEV_ID("sh-sci.5", &mstp_clks[HWBLK_SCIF5]),

	CLKDEV_DEV_ID("spi_sh_msiof.0", &mstp_clks[HWBLK_MSIOF0]),
	CLKDEV_DEV_ID("spi_sh_msiof.1", &mstp_clks[HWBLK_MSIOF1]),
	CLKDEV_DEV_ID("sh_keysc.0", &mstp_clks[HWBLK_KEYSC]),
	CLKDEV_CON_ID("rtc0", &mstp_clks[HWBLK_RTC]),
	CLKDEV_DEV_ID("i2c-sh_mobile.0", &mstp_clks[HWBLK_IIC0]),
	CLKDEV_DEV_ID("i2c-sh_mobile.1", &mstp_clks[HWBLK_IIC1]),
	CLKDEV_DEV_ID("sh_mmcif.0", &mstp_clks[HWBLK_MMC]),
<<<<<<< HEAD
	CLKDEV_DEV_ID("sh-eth.0", &mstp_clks[HWBLK_ETHER]),
>>>>>>> refs/remotes/origin/cm-10.0
=======
	CLKDEV_DEV_ID("sh7724-ether.0", &mstp_clks[HWBLK_ETHER]),
>>>>>>> refs/remotes/origin/master
	CLKDEV_CON_ID("atapi0", &mstp_clks[HWBLK_ATAPI]),
	CLKDEV_CON_ID("tpu0", &mstp_clks[HWBLK_TPU]),
	CLKDEV_CON_ID("irda0", &mstp_clks[HWBLK_IRDA]),
	CLKDEV_CON_ID("tsif0", &mstp_clks[HWBLK_TSIF]),
<<<<<<< HEAD
	CLKDEV_CON_ID("usb1", &mstp_clks[HWBLK_USB1]),
	CLKDEV_CON_ID("usb0", &mstp_clks[HWBLK_USB0]),
	CLKDEV_CON_ID("2dg0", &mstp_clks[HWBLK_2DG]),
<<<<<<< HEAD
	CLKDEV_CON_ID("sdhi0", &mstp_clks[HWBLK_SDHI0]),
	CLKDEV_CON_ID("sdhi1", &mstp_clks[HWBLK_SDHI1]),
	CLKDEV_CON_ID("veu1", &mstp_clks[HWBLK_VEU1]),
	CLKDEV_CON_ID("ceu1", &mstp_clks[HWBLK_CEU1]),
	CLKDEV_CON_ID("beu1", &mstp_clks[HWBLK_BEU1]),
	CLKDEV_CON_ID("2ddmac0", &mstp_clks[HWBLK_2DDMAC]),
	CLKDEV_CON_ID("spu0", &mstp_clks[HWBLK_SPU]),
	CLKDEV_CON_ID("jpu0", &mstp_clks[HWBLK_JPU]),
	CLKDEV_CON_ID("vou0", &mstp_clks[HWBLK_VOU]),
	CLKDEV_CON_ID("beu0", &mstp_clks[HWBLK_BEU0]),
	CLKDEV_CON_ID("ceu0", &mstp_clks[HWBLK_CEU0]),
	CLKDEV_CON_ID("veu0", &mstp_clks[HWBLK_VEU0]),
	CLKDEV_CON_ID("vpu0", &mstp_clks[HWBLK_VPU]),
	CLKDEV_CON_ID("lcdc0", &mstp_clks[HWBLK_LCDC]),
=======
=======
	CLKDEV_DEV_ID("renesas_usbhs.1", &mstp_clks[HWBLK_USB1]),
	CLKDEV_DEV_ID("renesas_usbhs.0", &mstp_clks[HWBLK_USB0]),
	CLKDEV_CON_ID("2dg0", &mstp_clks[HWBLK_2DG]),
>>>>>>> refs/remotes/origin/master
	CLKDEV_DEV_ID("sh_mobile_sdhi.0", &mstp_clks[HWBLK_SDHI0]),
	CLKDEV_DEV_ID("sh_mobile_sdhi.1", &mstp_clks[HWBLK_SDHI1]),
	CLKDEV_CON_ID("veu1", &mstp_clks[HWBLK_VEU1]),
	CLKDEV_DEV_ID("sh_mobile_ceu.1", &mstp_clks[HWBLK_CEU1]),
	CLKDEV_CON_ID("beu1", &mstp_clks[HWBLK_BEU1]),
	CLKDEV_CON_ID("2ddmac0", &mstp_clks[HWBLK_2DDMAC]),
	CLKDEV_DEV_ID("sh_fsi.0", &mstp_clks[HWBLK_SPU]),
	CLKDEV_CON_ID("jpu0", &mstp_clks[HWBLK_JPU]),
	CLKDEV_DEV_ID("sh-vou.0", &mstp_clks[HWBLK_VOU]),
	CLKDEV_CON_ID("beu0", &mstp_clks[HWBLK_BEU0]),
	CLKDEV_DEV_ID("sh_mobile_ceu.0", &mstp_clks[HWBLK_CEU0]),
	CLKDEV_CON_ID("veu0", &mstp_clks[HWBLK_VEU0]),
	CLKDEV_CON_ID("vpu0", &mstp_clks[HWBLK_VPU]),
	CLKDEV_DEV_ID("sh_mobile_lcdc_fb.0", &mstp_clks[HWBLK_LCDC]),
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};

int __init arch_clk_init(void)
{
	int k, ret = 0;

	/* autodetect extal or fll configuration */
	if (__raw_readl(PLLCR) & 0x1000)
		pll_clk.parent = &fll_clk;
	else
		pll_clk.parent = &extal_clk;

	for (k = 0; !ret && (k < ARRAY_SIZE(main_clks)); k++)
		ret = clk_register(main_clks[k]);

	clkdev_add_table(lookups, ARRAY_SIZE(lookups));

	if (!ret)
		ret = sh_clk_div4_register(div4_clks, DIV4_NR, &div4_table);

	if (!ret)
<<<<<<< HEAD
<<<<<<< HEAD
		ret = sh_clk_div6_register(div6_clks, DIV6_NR);

	if (!ret)
		ret = sh_clk_div6_reparent_register(div6_reparent_clks, DIV6_REPARENT_NR);

	if (!ret)
		ret = sh_hwblk_clk_register(mstp_clks, HWBLK_NR);
=======
		ret = sh_clk_div6_reparent_register(div6_clks, DIV6_NR);

	if (!ret)
		ret = sh_clk_mstp32_register(mstp_clks, HWBLK_NR);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		ret = sh_clk_div6_reparent_register(div6_clks, DIV6_NR);

	if (!ret)
		ret = sh_clk_mstp_register(mstp_clks, HWBLK_NR);
>>>>>>> refs/remotes/origin/master

	return ret;
}
