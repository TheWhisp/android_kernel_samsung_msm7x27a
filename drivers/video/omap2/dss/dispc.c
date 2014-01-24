/*
 * linux/drivers/video/omap2/dss/dispc.c
 *
 * Copyright (C) 2009 Nokia Corporation
 * Author: Tomi Valkeinen <tomi.valkeinen@nokia.com>
 *
 * Some code and ideas taken from drivers/video/omap/ driver
 * by Imre Deak.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define DSS_SUBSYS_NAME "DISPC"

#include <linux/kernel.h>
#include <linux/dma-mapping.h>
#include <linux/vmalloc.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/jiffies.h>
#include <linux/seq_file.h>
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/hardirq.h>
<<<<<<< HEAD
#include <linux/interrupt.h>
<<<<<<< HEAD

#include <plat/sram.h>
=======
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>

>>>>>>> refs/remotes/origin/cm-10.0
#include <plat/clock.h>
=======
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/sizes.h>
>>>>>>> refs/remotes/origin/master

#include <video/omapdss.h>

#include "dss.h"
#include "dss_features.h"
#include "dispc.h"

/* DISPC */
#define DISPC_SZ_REGS			SZ_4K

<<<<<<< HEAD
#define DISPC_IRQ_MASK_ERROR            (DISPC_IRQ_GFX_FIFO_UNDERFLOW | \
					 DISPC_IRQ_OCP_ERR | \
					 DISPC_IRQ_VID1_FIFO_UNDERFLOW | \
					 DISPC_IRQ_VID2_FIFO_UNDERFLOW | \
					 DISPC_IRQ_SYNC_LOST | \
					 DISPC_IRQ_SYNC_LOST_DIGIT)

#define DISPC_MAX_NR_ISRS		8

struct omap_dispc_isr_data {
	omap_dispc_isr_t	isr;
	void			*arg;
	u32			mask;
};

<<<<<<< HEAD
struct dispc_h_coef {
	s8 hc4;
	s8 hc3;
	u8 hc2;
	s8 hc1;
	s8 hc0;
};

struct dispc_v_coef {
	s8 vc22;
	s8 vc2;
	u8 vc1;
	s8 vc0;
	s8 vc00;
=======
=======
>>>>>>> refs/remotes/origin/master
enum omap_burst_size {
	BURST_SIZE_X2 = 0,
	BURST_SIZE_X4 = 1,
	BURST_SIZE_X8 = 2,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};

#define REG_GET(idx, start, end) \
	FLD_GET(dispc_read_reg(idx), start, end)

#define REG_FLD_MOD(idx, val, start, end)				\
	dispc_write_reg(idx, FLD_MOD(dispc_read_reg(idx), val, start, end))

<<<<<<< HEAD
struct dispc_irq_stats {
	unsigned long last_reset;
	unsigned irq_count;
	unsigned irqs[32];
};

static struct {
	struct platform_device *pdev;
	void __iomem    *base;
<<<<<<< HEAD
	int irq;

	u32	fifo_size[3];
=======
=======
struct dispc_features {
	u8 sw_start;
	u8 fp_start;
	u8 bp_start;
	u16 sw_max;
	u16 vp_max;
	u16 hp_max;
	u8 mgr_width_start;
	u8 mgr_height_start;
	u16 mgr_width_max;
	u16 mgr_height_max;
	unsigned long max_lcd_pclk;
	unsigned long max_tv_pclk;
	int (*calc_scaling) (unsigned long pclk, unsigned long lclk,
		const struct omap_video_timings *mgr_timings,
		u16 width, u16 height, u16 out_width, u16 out_height,
		enum omap_color_mode color_mode, bool *five_taps,
		int *x_predecim, int *y_predecim, int *decim_x, int *decim_y,
		u16 pos_x, unsigned long *core_clk, bool mem_to_mem);
	unsigned long (*calc_core_clk) (unsigned long pclk,
		u16 width, u16 height, u16 out_width, u16 out_height,
		bool mem_to_mem);
	u8 num_fifos;

	/* swap GFX & WB fifos */
	bool gfx_fifo_workaround:1;

	/* no DISPC_IRQ_FRAMEDONETV on this SoC */
	bool no_framedone_tv:1;

	/* revert to the OMAP4 mechanism of DISPC Smart Standby operation */
	bool mstandby_workaround:1;
};

#define DISPC_MAX_NR_FIFOS 5

static struct {
	struct platform_device *pdev;
	void __iomem    *base;
>>>>>>> refs/remotes/origin/master

	int		ctx_loss_cnt;

	int irq;
<<<<<<< HEAD
	struct clk *dss_clk;

	u32	fifo_size[MAX_DSS_OVERLAYS];
>>>>>>> refs/remotes/origin/cm-10.0

	spinlock_t irq_lock;
	u32 irq_error_mask;
	struct omap_dispc_isr_data registered_isr[DISPC_MAX_NR_ISRS];
	u32 error_irqs;
	struct work_struct error_work;

<<<<<<< HEAD
=======
	bool		ctx_valid;
>>>>>>> refs/remotes/origin/cm-10.0
	u32		ctx[DISPC_SZ_REGS / sizeof(u32)];

#ifdef CONFIG_OMAP2_DSS_COLLECT_IRQ_STATS
	spinlock_t irq_stats_lock;
	struct dispc_irq_stats irq_stats;
#endif
=======

	unsigned long core_clk_rate;
	unsigned long tv_pclk_rate;

	u32 fifo_size[DISPC_MAX_NR_FIFOS];
	/* maps which plane is using a fifo. fifo-id -> plane-id */
	int fifo_assignment[DISPC_MAX_NR_FIFOS];

	bool		ctx_valid;
	u32		ctx[DISPC_SZ_REGS / sizeof(u32)];

	const struct dispc_features *feat;
>>>>>>> refs/remotes/origin/master
} dispc;

enum omap_color_component {
	/* used for all color formats for OMAP3 and earlier
	 * and for RGB and Y color component on OMAP4
	 */
	DISPC_COLOR_COMPONENT_RGB_Y		= 1 << 0,
	/* used for UV component for
	 * OMAP_DSS_COLOR_YUV2, OMAP_DSS_COLOR_UYVY, OMAP_DSS_COLOR_NV12
	 * color formats on OMAP4
	 */
	DISPC_COLOR_COMPONENT_UV		= 1 << 1,
};

<<<<<<< HEAD
static void _omap_dispc_set_irqs(void);
=======
enum mgr_reg_fields {
	DISPC_MGR_FLD_ENABLE,
	DISPC_MGR_FLD_STNTFT,
	DISPC_MGR_FLD_GO,
	DISPC_MGR_FLD_TFTDATALINES,
	DISPC_MGR_FLD_STALLMODE,
	DISPC_MGR_FLD_TCKENABLE,
	DISPC_MGR_FLD_TCKSELECTION,
	DISPC_MGR_FLD_CPR,
	DISPC_MGR_FLD_FIFOHANDCHECK,
	/* used to maintain a count of the above fields */
	DISPC_MGR_FLD_NUM,
};

static const struct {
	const char *name;
	u32 vsync_irq;
	u32 framedone_irq;
	u32 sync_lost_irq;
	struct reg_field reg_desc[DISPC_MGR_FLD_NUM];
} mgr_desc[] = {
	[OMAP_DSS_CHANNEL_LCD] = {
		.name		= "LCD",
		.vsync_irq	= DISPC_IRQ_VSYNC,
		.framedone_irq	= DISPC_IRQ_FRAMEDONE,
		.sync_lost_irq	= DISPC_IRQ_SYNC_LOST,
		.reg_desc	= {
			[DISPC_MGR_FLD_ENABLE]		= { DISPC_CONTROL,  0,  0 },
			[DISPC_MGR_FLD_STNTFT]		= { DISPC_CONTROL,  3,  3 },
			[DISPC_MGR_FLD_GO]		= { DISPC_CONTROL,  5,  5 },
			[DISPC_MGR_FLD_TFTDATALINES]	= { DISPC_CONTROL,  9,  8 },
			[DISPC_MGR_FLD_STALLMODE]	= { DISPC_CONTROL, 11, 11 },
			[DISPC_MGR_FLD_TCKENABLE]	= { DISPC_CONFIG,  10, 10 },
			[DISPC_MGR_FLD_TCKSELECTION]	= { DISPC_CONFIG,  11, 11 },
			[DISPC_MGR_FLD_CPR]		= { DISPC_CONFIG,  15, 15 },
			[DISPC_MGR_FLD_FIFOHANDCHECK]	= { DISPC_CONFIG,  16, 16 },
		},
	},
	[OMAP_DSS_CHANNEL_DIGIT] = {
		.name		= "DIGIT",
		.vsync_irq	= DISPC_IRQ_EVSYNC_ODD | DISPC_IRQ_EVSYNC_EVEN,
		.framedone_irq	= DISPC_IRQ_FRAMEDONETV,
		.sync_lost_irq	= DISPC_IRQ_SYNC_LOST_DIGIT,
		.reg_desc	= {
			[DISPC_MGR_FLD_ENABLE]		= { DISPC_CONTROL,  1,  1 },
			[DISPC_MGR_FLD_STNTFT]		= { },
			[DISPC_MGR_FLD_GO]		= { DISPC_CONTROL,  6,  6 },
			[DISPC_MGR_FLD_TFTDATALINES]	= { },
			[DISPC_MGR_FLD_STALLMODE]	= { },
			[DISPC_MGR_FLD_TCKENABLE]	= { DISPC_CONFIG,  12, 12 },
			[DISPC_MGR_FLD_TCKSELECTION]	= { DISPC_CONFIG,  13, 13 },
			[DISPC_MGR_FLD_CPR]		= { },
			[DISPC_MGR_FLD_FIFOHANDCHECK]	= { DISPC_CONFIG,  16, 16 },
		},
	},
	[OMAP_DSS_CHANNEL_LCD2] = {
		.name		= "LCD2",
		.vsync_irq	= DISPC_IRQ_VSYNC2,
		.framedone_irq	= DISPC_IRQ_FRAMEDONE2,
		.sync_lost_irq	= DISPC_IRQ_SYNC_LOST2,
		.reg_desc	= {
			[DISPC_MGR_FLD_ENABLE]		= { DISPC_CONTROL2,  0,  0 },
			[DISPC_MGR_FLD_STNTFT]		= { DISPC_CONTROL2,  3,  3 },
			[DISPC_MGR_FLD_GO]		= { DISPC_CONTROL2,  5,  5 },
			[DISPC_MGR_FLD_TFTDATALINES]	= { DISPC_CONTROL2,  9,  8 },
			[DISPC_MGR_FLD_STALLMODE]	= { DISPC_CONTROL2, 11, 11 },
			[DISPC_MGR_FLD_TCKENABLE]	= { DISPC_CONFIG2,  10, 10 },
			[DISPC_MGR_FLD_TCKSELECTION]	= { DISPC_CONFIG2,  11, 11 },
			[DISPC_MGR_FLD_CPR]		= { DISPC_CONFIG2,  15, 15 },
			[DISPC_MGR_FLD_FIFOHANDCHECK]	= { DISPC_CONFIG2,  16, 16 },
		},
	},
	[OMAP_DSS_CHANNEL_LCD3] = {
		.name		= "LCD3",
		.vsync_irq	= DISPC_IRQ_VSYNC3,
		.framedone_irq	= DISPC_IRQ_FRAMEDONE3,
		.sync_lost_irq	= DISPC_IRQ_SYNC_LOST3,
		.reg_desc	= {
			[DISPC_MGR_FLD_ENABLE]		= { DISPC_CONTROL3,  0,  0 },
			[DISPC_MGR_FLD_STNTFT]		= { DISPC_CONTROL3,  3,  3 },
			[DISPC_MGR_FLD_GO]		= { DISPC_CONTROL3,  5,  5 },
			[DISPC_MGR_FLD_TFTDATALINES]	= { DISPC_CONTROL3,  9,  8 },
			[DISPC_MGR_FLD_STALLMODE]	= { DISPC_CONTROL3, 11, 11 },
			[DISPC_MGR_FLD_TCKENABLE]	= { DISPC_CONFIG3,  10, 10 },
			[DISPC_MGR_FLD_TCKSELECTION]	= { DISPC_CONFIG3,  11, 11 },
			[DISPC_MGR_FLD_CPR]		= { DISPC_CONFIG3,  15, 15 },
			[DISPC_MGR_FLD_FIFOHANDCHECK]	= { DISPC_CONFIG3,  16, 16 },
		},
	},
};

struct color_conv_coef {
	int ry, rcr, rcb, gy, gcr, gcb, by, bcr, bcb;
	int full_range;
};

static unsigned long dispc_plane_pclk_rate(enum omap_plane plane);
static unsigned long dispc_plane_lclk_rate(enum omap_plane plane);
>>>>>>> refs/remotes/origin/master

static inline void dispc_write_reg(const u16 idx, u32 val)
{
	__raw_writel(val, dispc.base + idx);
}

static inline u32 dispc_read_reg(const u16 idx)
{
	return __raw_readl(dispc.base + idx);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
static int dispc_get_ctx_loss_count(void)
{
	struct device *dev = &dispc.pdev->dev;
	struct omap_display_platform_data *pdata = dev->platform_data;
	struct omap_dss_board_info *board_data = pdata->board_data;
	int cnt;

	if (!board_data->get_context_loss_count)
		return -ENOENT;

	cnt = board_data->get_context_loss_count(dev);

	WARN_ONCE(cnt < 0, "get_context_loss_count failed: %d\n", cnt);

	return cnt;
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
static u32 mgr_fld_read(enum omap_channel channel, enum mgr_reg_fields regfld)
{
	const struct reg_field rfld = mgr_desc[channel].reg_desc[regfld];
	return REG_GET(rfld.reg, rfld.high, rfld.low);
}

static void mgr_fld_write(enum omap_channel channel,
					enum mgr_reg_fields regfld, int val) {
	const struct reg_field rfld = mgr_desc[channel].reg_desc[regfld];
	REG_FLD_MOD(rfld.reg, val, rfld.high, rfld.low);
}

>>>>>>> refs/remotes/origin/master
#define SR(reg) \
	dispc.ctx[DISPC_##reg / sizeof(u32)] = dispc_read_reg(DISPC_##reg)
#define RR(reg) \
	dispc_write_reg(DISPC_##reg, dispc.ctx[DISPC_##reg / sizeof(u32)])

<<<<<<< HEAD
<<<<<<< HEAD
void dispc_save_context(void)
{
	int i;
	if (cpu_is_omap24xx())
		return;

	SR(SYSCONFIG);
	SR(IRQENABLE);
	SR(CONTROL);
	SR(CONFIG);
	SR(DEFAULT_COLOR(OMAP_DSS_CHANNEL_LCD));
	SR(DEFAULT_COLOR(OMAP_DSS_CHANNEL_DIGIT));
	SR(TRANS_COLOR(OMAP_DSS_CHANNEL_LCD));
	SR(TRANS_COLOR(OMAP_DSS_CHANNEL_DIGIT));
	SR(LINE_NUMBER);
	SR(TIMING_H(OMAP_DSS_CHANNEL_LCD));
	SR(TIMING_V(OMAP_DSS_CHANNEL_LCD));
	SR(POL_FREQ(OMAP_DSS_CHANNEL_LCD));
	SR(DIVISORo(OMAP_DSS_CHANNEL_LCD));
	SR(GLOBAL_ALPHA);
	SR(SIZE_MGR(OMAP_DSS_CHANNEL_DIGIT));
	SR(SIZE_MGR(OMAP_DSS_CHANNEL_LCD));
	if (dss_has_feature(FEAT_MGR_LCD2)) {
		SR(CONTROL2);
		SR(DEFAULT_COLOR(OMAP_DSS_CHANNEL_LCD2));
		SR(TRANS_COLOR(OMAP_DSS_CHANNEL_LCD2));
		SR(SIZE_MGR(OMAP_DSS_CHANNEL_LCD2));
		SR(TIMING_H(OMAP_DSS_CHANNEL_LCD2));
		SR(TIMING_V(OMAP_DSS_CHANNEL_LCD2));
		SR(POL_FREQ(OMAP_DSS_CHANNEL_LCD2));
		SR(DIVISORo(OMAP_DSS_CHANNEL_LCD2));
		SR(CONFIG2);
	}

	SR(OVL_BA0(OMAP_DSS_GFX));
	SR(OVL_BA1(OMAP_DSS_GFX));
	SR(OVL_POSITION(OMAP_DSS_GFX));
	SR(OVL_SIZE(OMAP_DSS_GFX));
	SR(OVL_ATTRIBUTES(OMAP_DSS_GFX));
	SR(OVL_FIFO_THRESHOLD(OMAP_DSS_GFX));
	SR(OVL_ROW_INC(OMAP_DSS_GFX));
	SR(OVL_PIXEL_INC(OMAP_DSS_GFX));
	SR(OVL_WINDOW_SKIP(OMAP_DSS_GFX));
	SR(OVL_TABLE_BA(OMAP_DSS_GFX));

	SR(DATA_CYCLE1(OMAP_DSS_CHANNEL_LCD));
	SR(DATA_CYCLE2(OMAP_DSS_CHANNEL_LCD));
	SR(DATA_CYCLE3(OMAP_DSS_CHANNEL_LCD));

	SR(CPR_COEF_R(OMAP_DSS_CHANNEL_LCD));
	SR(CPR_COEF_G(OMAP_DSS_CHANNEL_LCD));
	SR(CPR_COEF_B(OMAP_DSS_CHANNEL_LCD));
	if (dss_has_feature(FEAT_MGR_LCD2)) {
		SR(CPR_COEF_B(OMAP_DSS_CHANNEL_LCD2));
		SR(CPR_COEF_G(OMAP_DSS_CHANNEL_LCD2));
		SR(CPR_COEF_R(OMAP_DSS_CHANNEL_LCD2));

		SR(DATA_CYCLE1(OMAP_DSS_CHANNEL_LCD2));
		SR(DATA_CYCLE2(OMAP_DSS_CHANNEL_LCD2));
		SR(DATA_CYCLE3(OMAP_DSS_CHANNEL_LCD2));
	}

	SR(OVL_PRELOAD(OMAP_DSS_GFX));

	/* VID1 */
	SR(OVL_BA0(OMAP_DSS_VIDEO1));
	SR(OVL_BA1(OMAP_DSS_VIDEO1));
	SR(OVL_POSITION(OMAP_DSS_VIDEO1));
	SR(OVL_SIZE(OMAP_DSS_VIDEO1));
	SR(OVL_ATTRIBUTES(OMAP_DSS_VIDEO1));
	SR(OVL_FIFO_THRESHOLD(OMAP_DSS_VIDEO1));
	SR(OVL_ROW_INC(OMAP_DSS_VIDEO1));
	SR(OVL_PIXEL_INC(OMAP_DSS_VIDEO1));
	SR(OVL_FIR(OMAP_DSS_VIDEO1));
	SR(OVL_PICTURE_SIZE(OMAP_DSS_VIDEO1));
	SR(OVL_ACCU0(OMAP_DSS_VIDEO1));
	SR(OVL_ACCU1(OMAP_DSS_VIDEO1));

	for (i = 0; i < 8; i++)
		SR(OVL_FIR_COEF_H(OMAP_DSS_VIDEO1, i));

	for (i = 0; i < 8; i++)
		SR(OVL_FIR_COEF_HV(OMAP_DSS_VIDEO1, i));

	for (i = 0; i < 5; i++)
		SR(OVL_CONV_COEF(OMAP_DSS_VIDEO1, i));

	for (i = 0; i < 8; i++)
		SR(OVL_FIR_COEF_V(OMAP_DSS_VIDEO1, i));

	if (dss_has_feature(FEAT_HANDLE_UV_SEPARATE)) {
		SR(OVL_BA0_UV(OMAP_DSS_VIDEO1));
		SR(OVL_BA1_UV(OMAP_DSS_VIDEO1));
		SR(OVL_FIR2(OMAP_DSS_VIDEO1));
		SR(OVL_ACCU2_0(OMAP_DSS_VIDEO1));
		SR(OVL_ACCU2_1(OMAP_DSS_VIDEO1));

		for (i = 0; i < 8; i++)
			SR(OVL_FIR_COEF_H2(OMAP_DSS_VIDEO1, i));

		for (i = 0; i < 8; i++)
			SR(OVL_FIR_COEF_HV2(OMAP_DSS_VIDEO1, i));

		for (i = 0; i < 8; i++)
			SR(OVL_FIR_COEF_V2(OMAP_DSS_VIDEO1, i));
	}
	if (dss_has_feature(FEAT_ATTR2))
		SR(OVL_ATTRIBUTES2(OMAP_DSS_VIDEO1));

	SR(OVL_PRELOAD(OMAP_DSS_VIDEO1));

	/* VID2 */
	SR(OVL_BA0(OMAP_DSS_VIDEO2));
	SR(OVL_BA1(OMAP_DSS_VIDEO2));
	SR(OVL_POSITION(OMAP_DSS_VIDEO2));
	SR(OVL_SIZE(OMAP_DSS_VIDEO2));
	SR(OVL_ATTRIBUTES(OMAP_DSS_VIDEO2));
	SR(OVL_FIFO_THRESHOLD(OMAP_DSS_VIDEO2));
	SR(OVL_ROW_INC(OMAP_DSS_VIDEO2));
	SR(OVL_PIXEL_INC(OMAP_DSS_VIDEO2));
	SR(OVL_FIR(OMAP_DSS_VIDEO2));
	SR(OVL_PICTURE_SIZE(OMAP_DSS_VIDEO2));
	SR(OVL_ACCU0(OMAP_DSS_VIDEO2));
	SR(OVL_ACCU1(OMAP_DSS_VIDEO2));

	for (i = 0; i < 8; i++)
		SR(OVL_FIR_COEF_H(OMAP_DSS_VIDEO2, i));

	for (i = 0; i < 8; i++)
		SR(OVL_FIR_COEF_HV(OMAP_DSS_VIDEO2, i));

	for (i = 0; i < 5; i++)
		SR(OVL_CONV_COEF(OMAP_DSS_VIDEO2, i));

	for (i = 0; i < 8; i++)
		SR(OVL_FIR_COEF_V(OMAP_DSS_VIDEO2, i));

	if (dss_has_feature(FEAT_HANDLE_UV_SEPARATE)) {
		SR(OVL_BA0_UV(OMAP_DSS_VIDEO2));
		SR(OVL_BA1_UV(OMAP_DSS_VIDEO2));
		SR(OVL_FIR2(OMAP_DSS_VIDEO2));
		SR(OVL_ACCU2_0(OMAP_DSS_VIDEO2));
		SR(OVL_ACCU2_1(OMAP_DSS_VIDEO2));

		for (i = 0; i < 8; i++)
			SR(OVL_FIR_COEF_H2(OMAP_DSS_VIDEO2, i));

		for (i = 0; i < 8; i++)
			SR(OVL_FIR_COEF_HV2(OMAP_DSS_VIDEO2, i));

		for (i = 0; i < 8; i++)
			SR(OVL_FIR_COEF_V2(OMAP_DSS_VIDEO2, i));
	}
	if (dss_has_feature(FEAT_ATTR2))
		SR(OVL_ATTRIBUTES2(OMAP_DSS_VIDEO2));

	SR(OVL_PRELOAD(OMAP_DSS_VIDEO2));

	if (dss_has_feature(FEAT_CORE_CLK_DIV))
		SR(DIVISOR);
}

void dispc_restore_context(void)
{
	int i;
	RR(SYSCONFIG);
	/*RR(IRQENABLE);*/
	/*RR(CONTROL);*/
	RR(CONFIG);
	RR(DEFAULT_COLOR(OMAP_DSS_CHANNEL_LCD));
	RR(DEFAULT_COLOR(OMAP_DSS_CHANNEL_DIGIT));
	RR(TRANS_COLOR(OMAP_DSS_CHANNEL_LCD));
	RR(TRANS_COLOR(OMAP_DSS_CHANNEL_DIGIT));
	RR(LINE_NUMBER);
	RR(TIMING_H(OMAP_DSS_CHANNEL_LCD));
	RR(TIMING_V(OMAP_DSS_CHANNEL_LCD));
	RR(POL_FREQ(OMAP_DSS_CHANNEL_LCD));
	RR(DIVISORo(OMAP_DSS_CHANNEL_LCD));
	RR(GLOBAL_ALPHA);
	RR(SIZE_MGR(OMAP_DSS_CHANNEL_DIGIT));
	RR(SIZE_MGR(OMAP_DSS_CHANNEL_LCD));
	if (dss_has_feature(FEAT_MGR_LCD2)) {
		RR(DEFAULT_COLOR(OMAP_DSS_CHANNEL_LCD2));
		RR(TRANS_COLOR(OMAP_DSS_CHANNEL_LCD2));
		RR(SIZE_MGR(OMAP_DSS_CHANNEL_LCD2));
		RR(TIMING_H(OMAP_DSS_CHANNEL_LCD2));
		RR(TIMING_V(OMAP_DSS_CHANNEL_LCD2));
		RR(POL_FREQ(OMAP_DSS_CHANNEL_LCD2));
		RR(DIVISORo(OMAP_DSS_CHANNEL_LCD2));
		RR(CONFIG2);
	}

	RR(OVL_BA0(OMAP_DSS_GFX));
	RR(OVL_BA1(OMAP_DSS_GFX));
	RR(OVL_POSITION(OMAP_DSS_GFX));
	RR(OVL_SIZE(OMAP_DSS_GFX));
	RR(OVL_ATTRIBUTES(OMAP_DSS_GFX));
	RR(OVL_FIFO_THRESHOLD(OMAP_DSS_GFX));
	RR(OVL_ROW_INC(OMAP_DSS_GFX));
	RR(OVL_PIXEL_INC(OMAP_DSS_GFX));
	RR(OVL_WINDOW_SKIP(OMAP_DSS_GFX));
	RR(OVL_TABLE_BA(OMAP_DSS_GFX));


	RR(DATA_CYCLE1(OMAP_DSS_CHANNEL_LCD));
	RR(DATA_CYCLE2(OMAP_DSS_CHANNEL_LCD));
	RR(DATA_CYCLE3(OMAP_DSS_CHANNEL_LCD));

	RR(CPR_COEF_R(OMAP_DSS_CHANNEL_LCD));
	RR(CPR_COEF_G(OMAP_DSS_CHANNEL_LCD));
	RR(CPR_COEF_B(OMAP_DSS_CHANNEL_LCD));
	if (dss_has_feature(FEAT_MGR_LCD2)) {
		RR(DATA_CYCLE1(OMAP_DSS_CHANNEL_LCD2));
		RR(DATA_CYCLE2(OMAP_DSS_CHANNEL_LCD2));
		RR(DATA_CYCLE3(OMAP_DSS_CHANNEL_LCD2));

		RR(CPR_COEF_B(OMAP_DSS_CHANNEL_LCD2));
		RR(CPR_COEF_G(OMAP_DSS_CHANNEL_LCD2));
		RR(CPR_COEF_R(OMAP_DSS_CHANNEL_LCD2));
	}

	RR(OVL_PRELOAD(OMAP_DSS_GFX));

	/* VID1 */
	RR(OVL_BA0(OMAP_DSS_VIDEO1));
	RR(OVL_BA1(OMAP_DSS_VIDEO1));
	RR(OVL_POSITION(OMAP_DSS_VIDEO1));
	RR(OVL_SIZE(OMAP_DSS_VIDEO1));
	RR(OVL_ATTRIBUTES(OMAP_DSS_VIDEO1));
	RR(OVL_FIFO_THRESHOLD(OMAP_DSS_VIDEO1));
	RR(OVL_ROW_INC(OMAP_DSS_VIDEO1));
	RR(OVL_PIXEL_INC(OMAP_DSS_VIDEO1));
	RR(OVL_FIR(OMAP_DSS_VIDEO1));
	RR(OVL_PICTURE_SIZE(OMAP_DSS_VIDEO1));
	RR(OVL_ACCU0(OMAP_DSS_VIDEO1));
	RR(OVL_ACCU1(OMAP_DSS_VIDEO1));

	for (i = 0; i < 8; i++)
		RR(OVL_FIR_COEF_H(OMAP_DSS_VIDEO1, i));

	for (i = 0; i < 8; i++)
		RR(OVL_FIR_COEF_HV(OMAP_DSS_VIDEO1, i));

	for (i = 0; i < 5; i++)
		RR(OVL_CONV_COEF(OMAP_DSS_VIDEO1, i));

	for (i = 0; i < 8; i++)
		RR(OVL_FIR_COEF_V(OMAP_DSS_VIDEO1, i));

	if (dss_has_feature(FEAT_HANDLE_UV_SEPARATE)) {
		RR(OVL_BA0_UV(OMAP_DSS_VIDEO1));
		RR(OVL_BA1_UV(OMAP_DSS_VIDEO1));
		RR(OVL_FIR2(OMAP_DSS_VIDEO1));
		RR(OVL_ACCU2_0(OMAP_DSS_VIDEO1));
		RR(OVL_ACCU2_1(OMAP_DSS_VIDEO1));

		for (i = 0; i < 8; i++)
			RR(OVL_FIR_COEF_H2(OMAP_DSS_VIDEO1, i));

		for (i = 0; i < 8; i++)
			RR(OVL_FIR_COEF_HV2(OMAP_DSS_VIDEO1, i));

		for (i = 0; i < 8; i++)
			RR(OVL_FIR_COEF_V2(OMAP_DSS_VIDEO1, i));
	}
	if (dss_has_feature(FEAT_ATTR2))
		RR(OVL_ATTRIBUTES2(OMAP_DSS_VIDEO1));

	RR(OVL_PRELOAD(OMAP_DSS_VIDEO1));

	/* VID2 */
	RR(OVL_BA0(OMAP_DSS_VIDEO2));
	RR(OVL_BA1(OMAP_DSS_VIDEO2));
	RR(OVL_POSITION(OMAP_DSS_VIDEO2));
	RR(OVL_SIZE(OMAP_DSS_VIDEO2));
	RR(OVL_ATTRIBUTES(OMAP_DSS_VIDEO2));
	RR(OVL_FIFO_THRESHOLD(OMAP_DSS_VIDEO2));
	RR(OVL_ROW_INC(OMAP_DSS_VIDEO2));
	RR(OVL_PIXEL_INC(OMAP_DSS_VIDEO2));
	RR(OVL_FIR(OMAP_DSS_VIDEO2));
	RR(OVL_PICTURE_SIZE(OMAP_DSS_VIDEO2));
	RR(OVL_ACCU0(OMAP_DSS_VIDEO2));
	RR(OVL_ACCU1(OMAP_DSS_VIDEO2));

	for (i = 0; i < 8; i++)
		RR(OVL_FIR_COEF_H(OMAP_DSS_VIDEO2, i));

	for (i = 0; i < 8; i++)
		RR(OVL_FIR_COEF_HV(OMAP_DSS_VIDEO2, i));

	for (i = 0; i < 5; i++)
		RR(OVL_CONV_COEF(OMAP_DSS_VIDEO2, i));

	for (i = 0; i < 8; i++)
		RR(OVL_FIR_COEF_V(OMAP_DSS_VIDEO2, i));

	if (dss_has_feature(FEAT_HANDLE_UV_SEPARATE)) {
		RR(OVL_BA0_UV(OMAP_DSS_VIDEO2));
		RR(OVL_BA1_UV(OMAP_DSS_VIDEO2));
		RR(OVL_FIR2(OMAP_DSS_VIDEO2));
		RR(OVL_ACCU2_0(OMAP_DSS_VIDEO2));
		RR(OVL_ACCU2_1(OMAP_DSS_VIDEO2));

		for (i = 0; i < 8; i++)
			RR(OVL_FIR_COEF_H2(OMAP_DSS_VIDEO2, i));

		for (i = 0; i < 8; i++)
			RR(OVL_FIR_COEF_HV2(OMAP_DSS_VIDEO2, i));

		for (i = 0; i < 8; i++)
			RR(OVL_FIR_COEF_V2(OMAP_DSS_VIDEO2, i));
	}
	if (dss_has_feature(FEAT_ATTR2))
		RR(OVL_ATTRIBUTES2(OMAP_DSS_VIDEO2));

	RR(OVL_PRELOAD(OMAP_DSS_VIDEO2));
=======
=======
>>>>>>> refs/remotes/origin/master
static void dispc_save_context(void)
{
	int i, j;

	DSSDBG("dispc_save_context\n");

	SR(IRQENABLE);
	SR(CONTROL);
	SR(CONFIG);
	SR(LINE_NUMBER);
	if (dss_has_feature(FEAT_ALPHA_FIXED_ZORDER) ||
			dss_has_feature(FEAT_ALPHA_FREE_ZORDER))
		SR(GLOBAL_ALPHA);
	if (dss_has_feature(FEAT_MGR_LCD2)) {
		SR(CONTROL2);
		SR(CONFIG2);
	}
<<<<<<< HEAD
=======
	if (dss_has_feature(FEAT_MGR_LCD3)) {
		SR(CONTROL3);
		SR(CONFIG3);
	}
>>>>>>> refs/remotes/origin/master

	for (i = 0; i < dss_feat_get_num_mgrs(); i++) {
		SR(DEFAULT_COLOR(i));
		SR(TRANS_COLOR(i));
		SR(SIZE_MGR(i));
		if (i == OMAP_DSS_CHANNEL_DIGIT)
			continue;
		SR(TIMING_H(i));
		SR(TIMING_V(i));
		SR(POL_FREQ(i));
		SR(DIVISORo(i));

		SR(DATA_CYCLE1(i));
		SR(DATA_CYCLE2(i));
		SR(DATA_CYCLE3(i));

		if (dss_has_feature(FEAT_CPR)) {
			SR(CPR_COEF_R(i));
			SR(CPR_COEF_G(i));
			SR(CPR_COEF_B(i));
		}
	}

	for (i = 0; i < dss_feat_get_num_ovls(); i++) {
		SR(OVL_BA0(i));
		SR(OVL_BA1(i));
		SR(OVL_POSITION(i));
		SR(OVL_SIZE(i));
		SR(OVL_ATTRIBUTES(i));
		SR(OVL_FIFO_THRESHOLD(i));
		SR(OVL_ROW_INC(i));
		SR(OVL_PIXEL_INC(i));
		if (dss_has_feature(FEAT_PRELOAD))
			SR(OVL_PRELOAD(i));
		if (i == OMAP_DSS_GFX) {
			SR(OVL_WINDOW_SKIP(i));
			SR(OVL_TABLE_BA(i));
			continue;
		}
		SR(OVL_FIR(i));
		SR(OVL_PICTURE_SIZE(i));
		SR(OVL_ACCU0(i));
		SR(OVL_ACCU1(i));

		for (j = 0; j < 8; j++)
			SR(OVL_FIR_COEF_H(i, j));

		for (j = 0; j < 8; j++)
			SR(OVL_FIR_COEF_HV(i, j));

		for (j = 0; j < 5; j++)
			SR(OVL_CONV_COEF(i, j));

		if (dss_has_feature(FEAT_FIR_COEF_V)) {
			for (j = 0; j < 8; j++)
				SR(OVL_FIR_COEF_V(i, j));
		}

		if (dss_has_feature(FEAT_HANDLE_UV_SEPARATE)) {
			SR(OVL_BA0_UV(i));
			SR(OVL_BA1_UV(i));
			SR(OVL_FIR2(i));
			SR(OVL_ACCU2_0(i));
			SR(OVL_ACCU2_1(i));

			for (j = 0; j < 8; j++)
				SR(OVL_FIR_COEF_H2(i, j));

			for (j = 0; j < 8; j++)
				SR(OVL_FIR_COEF_HV2(i, j));

			for (j = 0; j < 8; j++)
				SR(OVL_FIR_COEF_V2(i, j));
		}
		if (dss_has_feature(FEAT_ATTR2))
			SR(OVL_ATTRIBUTES2(i));
	}

	if (dss_has_feature(FEAT_CORE_CLK_DIV))
		SR(DIVISOR);

<<<<<<< HEAD
	dispc.ctx_loss_cnt = dispc_get_ctx_loss_count();
=======
	dispc.ctx_loss_cnt = dss_get_ctx_loss_count();
>>>>>>> refs/remotes/origin/master
	dispc.ctx_valid = true;

	DSSDBG("context saved, ctx_loss_count %d\n", dispc.ctx_loss_cnt);
}

static void dispc_restore_context(void)
{
	int i, j, ctx;

	DSSDBG("dispc_restore_context\n");

	if (!dispc.ctx_valid)
		return;

<<<<<<< HEAD
	ctx = dispc_get_ctx_loss_count();
=======
	ctx = dss_get_ctx_loss_count();
>>>>>>> refs/remotes/origin/master

	if (ctx >= 0 && ctx == dispc.ctx_loss_cnt)
		return;

	DSSDBG("ctx_loss_count: saved %d, current %d\n",
			dispc.ctx_loss_cnt, ctx);

	/*RR(IRQENABLE);*/
	/*RR(CONTROL);*/
	RR(CONFIG);
	RR(LINE_NUMBER);
	if (dss_has_feature(FEAT_ALPHA_FIXED_ZORDER) ||
			dss_has_feature(FEAT_ALPHA_FREE_ZORDER))
		RR(GLOBAL_ALPHA);
	if (dss_has_feature(FEAT_MGR_LCD2))
		RR(CONFIG2);
<<<<<<< HEAD
=======
	if (dss_has_feature(FEAT_MGR_LCD3))
		RR(CONFIG3);
>>>>>>> refs/remotes/origin/master

	for (i = 0; i < dss_feat_get_num_mgrs(); i++) {
		RR(DEFAULT_COLOR(i));
		RR(TRANS_COLOR(i));
		RR(SIZE_MGR(i));
		if (i == OMAP_DSS_CHANNEL_DIGIT)
			continue;
		RR(TIMING_H(i));
		RR(TIMING_V(i));
		RR(POL_FREQ(i));
		RR(DIVISORo(i));

		RR(DATA_CYCLE1(i));
		RR(DATA_CYCLE2(i));
		RR(DATA_CYCLE3(i));

		if (dss_has_feature(FEAT_CPR)) {
			RR(CPR_COEF_R(i));
			RR(CPR_COEF_G(i));
			RR(CPR_COEF_B(i));
		}
	}

	for (i = 0; i < dss_feat_get_num_ovls(); i++) {
		RR(OVL_BA0(i));
		RR(OVL_BA1(i));
		RR(OVL_POSITION(i));
		RR(OVL_SIZE(i));
		RR(OVL_ATTRIBUTES(i));
		RR(OVL_FIFO_THRESHOLD(i));
		RR(OVL_ROW_INC(i));
		RR(OVL_PIXEL_INC(i));
		if (dss_has_feature(FEAT_PRELOAD))
			RR(OVL_PRELOAD(i));
		if (i == OMAP_DSS_GFX) {
			RR(OVL_WINDOW_SKIP(i));
			RR(OVL_TABLE_BA(i));
			continue;
		}
		RR(OVL_FIR(i));
		RR(OVL_PICTURE_SIZE(i));
		RR(OVL_ACCU0(i));
		RR(OVL_ACCU1(i));

		for (j = 0; j < 8; j++)
			RR(OVL_FIR_COEF_H(i, j));

		for (j = 0; j < 8; j++)
			RR(OVL_FIR_COEF_HV(i, j));

		for (j = 0; j < 5; j++)
			RR(OVL_CONV_COEF(i, j));

		if (dss_has_feature(FEAT_FIR_COEF_V)) {
			for (j = 0; j < 8; j++)
				RR(OVL_FIR_COEF_V(i, j));
		}

		if (dss_has_feature(FEAT_HANDLE_UV_SEPARATE)) {
			RR(OVL_BA0_UV(i));
			RR(OVL_BA1_UV(i));
			RR(OVL_FIR2(i));
			RR(OVL_ACCU2_0(i));
			RR(OVL_ACCU2_1(i));

			for (j = 0; j < 8; j++)
				RR(OVL_FIR_COEF_H2(i, j));

			for (j = 0; j < 8; j++)
				RR(OVL_FIR_COEF_HV2(i, j));

			for (j = 0; j < 8; j++)
				RR(OVL_FIR_COEF_V2(i, j));
		}
		if (dss_has_feature(FEAT_ATTR2))
			RR(OVL_ATTRIBUTES2(i));
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	if (dss_has_feature(FEAT_CORE_CLK_DIV))
		RR(DIVISOR);

	/* enable last, because LCD & DIGIT enable are here */
	RR(CONTROL);
	if (dss_has_feature(FEAT_MGR_LCD2))
		RR(CONTROL2);
<<<<<<< HEAD
	/* clear spurious SYNC_LOST_DIGIT interrupts */
	dispc_write_reg(DISPC_IRQSTATUS, DISPC_IRQ_SYNC_LOST_DIGIT);
=======
	if (dss_has_feature(FEAT_MGR_LCD3))
		RR(CONTROL3);
	/* clear spurious SYNC_LOST_DIGIT interrupts */
	dispc_clear_irqstatus(DISPC_IRQ_SYNC_LOST_DIGIT);
>>>>>>> refs/remotes/origin/master

	/*
	 * enable last so IRQs won't trigger before
	 * the context is fully restored
	 */
	RR(IRQENABLE);
<<<<<<< HEAD
<<<<<<< HEAD
=======

	DSSDBG("context restored\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======

	DSSDBG("context restored\n");
>>>>>>> refs/remotes/origin/master
}

#undef SR
#undef RR

<<<<<<< HEAD
<<<<<<< HEAD
static inline void enable_clocks(bool enable)
{
	if (enable)
		dss_clk_enable(DSS_CLK_ICK | DSS_CLK_FCK);
	else
		dss_clk_disable(DSS_CLK_ICK | DSS_CLK_FCK);
}

bool dispc_go_busy(enum omap_channel channel)
{
	int bit;

	if (channel == OMAP_DSS_CHANNEL_LCD ||
			channel == OMAP_DSS_CHANNEL_LCD2)
=======
=======
>>>>>>> refs/remotes/origin/master
int dispc_runtime_get(void)
{
	int r;

	DSSDBG("dispc_runtime_get\n");

	r = pm_runtime_get_sync(&dispc.pdev->dev);
	WARN_ON(r < 0);
	return r < 0 ? r : 0;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL(dispc_runtime_get);
>>>>>>> refs/remotes/origin/master

void dispc_runtime_put(void)
{
	int r;

	DSSDBG("dispc_runtime_put\n");

	r = pm_runtime_put_sync(&dispc.pdev->dev);
<<<<<<< HEAD
	WARN_ON(r < 0);
}

static inline bool dispc_mgr_is_lcd(enum omap_channel channel)
{
	if (channel == OMAP_DSS_CHANNEL_LCD ||
			channel == OMAP_DSS_CHANNEL_LCD2)
		return true;
	else
		return false;
}

static struct omap_dss_device *dispc_mgr_get_device(enum omap_channel channel)
{
	struct omap_overlay_manager *mgr =
		omap_dss_get_overlay_manager(channel);

	return mgr ? mgr->device : NULL;
}

u32 dispc_mgr_get_vsync_irq(enum omap_channel channel)
{
	switch (channel) {
	case OMAP_DSS_CHANNEL_LCD:
		return DISPC_IRQ_VSYNC;
	case OMAP_DSS_CHANNEL_LCD2:
		return DISPC_IRQ_VSYNC2;
	case OMAP_DSS_CHANNEL_DIGIT:
		return DISPC_IRQ_EVSYNC_ODD | DISPC_IRQ_EVSYNC_EVEN;
	default:
		BUG();
	}
}

u32 dispc_mgr_get_framedone_irq(enum omap_channel channel)
{
	switch (channel) {
	case OMAP_DSS_CHANNEL_LCD:
		return DISPC_IRQ_FRAMEDONE;
	case OMAP_DSS_CHANNEL_LCD2:
		return DISPC_IRQ_FRAMEDONE2;
	case OMAP_DSS_CHANNEL_DIGIT:
		return 0;
	default:
		BUG();
	}
=======
	WARN_ON(r < 0 && r != -ENOSYS);
}
EXPORT_SYMBOL(dispc_runtime_put);

u32 dispc_mgr_get_vsync_irq(enum omap_channel channel)
{
	return mgr_desc[channel].vsync_irq;
}
EXPORT_SYMBOL(dispc_mgr_get_vsync_irq);

u32 dispc_mgr_get_framedone_irq(enum omap_channel channel)
{
	if (channel == OMAP_DSS_CHANNEL_DIGIT && dispc.feat->no_framedone_tv)
		return 0;

	return mgr_desc[channel].framedone_irq;
}
EXPORT_SYMBOL(dispc_mgr_get_framedone_irq);

u32 dispc_mgr_get_sync_lost_irq(enum omap_channel channel)
{
	return mgr_desc[channel].sync_lost_irq;
}
EXPORT_SYMBOL(dispc_mgr_get_sync_lost_irq);

u32 dispc_wb_get_framedone_irq(void)
{
	return DISPC_IRQ_FRAMEDONEWB;
>>>>>>> refs/remotes/origin/master
}

bool dispc_mgr_go_busy(enum omap_channel channel)
{
<<<<<<< HEAD
	int bit;

	if (dispc_mgr_is_lcd(channel))
>>>>>>> refs/remotes/origin/cm-10.0
		bit = 5; /* GOLCD */
	else
		bit = 6; /* GODIGIT */

	if (channel == OMAP_DSS_CHANNEL_LCD2)
		return REG_GET(DISPC_CONTROL2, bit, bit) == 1;
	else
		return REG_GET(DISPC_CONTROL, bit, bit) == 1;
}

<<<<<<< HEAD
void dispc_go(enum omap_channel channel)
=======
void dispc_mgr_go(enum omap_channel channel)
>>>>>>> refs/remotes/origin/cm-10.0
{
	int bit;
	bool enable_bit, go_bit;

<<<<<<< HEAD
	enable_clocks(1);

	if (channel == OMAP_DSS_CHANNEL_LCD ||
			channel == OMAP_DSS_CHANNEL_LCD2)
=======
	if (dispc_mgr_is_lcd(channel))
>>>>>>> refs/remotes/origin/cm-10.0
		bit = 0; /* LCDENABLE */
	else
		bit = 1; /* DIGITALENABLE */

	/* if the channel is not enabled, we don't need GO */
	if (channel == OMAP_DSS_CHANNEL_LCD2)
		enable_bit = REG_GET(DISPC_CONTROL2, bit, bit) == 1;
	else
		enable_bit = REG_GET(DISPC_CONTROL, bit, bit) == 1;

	if (!enable_bit)
<<<<<<< HEAD
		goto end;

	if (channel == OMAP_DSS_CHANNEL_LCD ||
			channel == OMAP_DSS_CHANNEL_LCD2)
=======
		return;

	if (dispc_mgr_is_lcd(channel))
>>>>>>> refs/remotes/origin/cm-10.0
		bit = 5; /* GOLCD */
	else
		bit = 6; /* GODIGIT */

	if (channel == OMAP_DSS_CHANNEL_LCD2)
		go_bit = REG_GET(DISPC_CONTROL2, bit, bit) == 1;
	else
		go_bit = REG_GET(DISPC_CONTROL, bit, bit) == 1;

	if (go_bit) {
		DSSERR("GO bit not down for channel %d\n", channel);
<<<<<<< HEAD
		goto end;
=======
		return;
>>>>>>> refs/remotes/origin/cm-10.0
	}

	DSSDBG("GO %s\n", channel == OMAP_DSS_CHANNEL_LCD ? "LCD" :
		(channel == OMAP_DSS_CHANNEL_LCD2 ? "LCD2" : "DIGIT"));

	if (channel == OMAP_DSS_CHANNEL_LCD2)
		REG_FLD_MOD(DISPC_CONTROL2, 1, bit, bit);
	else
		REG_FLD_MOD(DISPC_CONTROL, 1, bit, bit);
<<<<<<< HEAD
end:
	enable_clocks(0);
}

static void _dispc_write_firh_reg(enum omap_plane plane, int reg, u32 value)
=======
}

static void dispc_ovl_write_firh_reg(enum omap_plane plane, int reg, u32 value)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return mgr_fld_read(channel, DISPC_MGR_FLD_GO) == 1;
}
EXPORT_SYMBOL(dispc_mgr_go_busy);

void dispc_mgr_go(enum omap_channel channel)
{
	WARN_ON(dispc_mgr_is_enabled(channel) == false);
	WARN_ON(dispc_mgr_go_busy(channel));

	DSSDBG("GO %s\n", mgr_desc[channel].name);

	mgr_fld_write(channel, DISPC_MGR_FLD_GO, 1);
}
EXPORT_SYMBOL(dispc_mgr_go);

bool dispc_wb_go_busy(void)
{
	return REG_GET(DISPC_CONTROL2, 6, 6) == 1;
}

void dispc_wb_go(void)
{
	enum omap_plane plane = OMAP_DSS_WB;
	bool enable, go;

	enable = REG_GET(DISPC_OVL_ATTRIBUTES(plane), 0, 0) == 1;

	if (!enable)
		return;

	go = REG_GET(DISPC_CONTROL2, 6, 6) == 1;
	if (go) {
		DSSERR("GO bit not down for WB\n");
		return;
	}

	REG_FLD_MOD(DISPC_CONTROL2, 1, 6, 6);
}

static void dispc_ovl_write_firh_reg(enum omap_plane plane, int reg, u32 value)
>>>>>>> refs/remotes/origin/master
{
	dispc_write_reg(DISPC_OVL_FIR_COEF_H(plane, reg), value);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void _dispc_write_firhv_reg(enum omap_plane plane, int reg, u32 value)
=======
static void dispc_ovl_write_firhv_reg(enum omap_plane plane, int reg, u32 value)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void dispc_ovl_write_firhv_reg(enum omap_plane plane, int reg, u32 value)
>>>>>>> refs/remotes/origin/master
{
	dispc_write_reg(DISPC_OVL_FIR_COEF_HV(plane, reg), value);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void _dispc_write_firv_reg(enum omap_plane plane, int reg, u32 value)
=======
static void dispc_ovl_write_firv_reg(enum omap_plane plane, int reg, u32 value)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void dispc_ovl_write_firv_reg(enum omap_plane plane, int reg, u32 value)
>>>>>>> refs/remotes/origin/master
{
	dispc_write_reg(DISPC_OVL_FIR_COEF_V(plane, reg), value);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void _dispc_write_firh2_reg(enum omap_plane plane, int reg, u32 value)
=======
static void dispc_ovl_write_firh2_reg(enum omap_plane plane, int reg, u32 value)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void dispc_ovl_write_firh2_reg(enum omap_plane plane, int reg, u32 value)
>>>>>>> refs/remotes/origin/master
{
	BUG_ON(plane == OMAP_DSS_GFX);

	dispc_write_reg(DISPC_OVL_FIR_COEF_H2(plane, reg), value);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void _dispc_write_firhv2_reg(enum omap_plane plane, int reg, u32 value)
=======
static void dispc_ovl_write_firhv2_reg(enum omap_plane plane, int reg,
		u32 value)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void dispc_ovl_write_firhv2_reg(enum omap_plane plane, int reg,
		u32 value)
>>>>>>> refs/remotes/origin/master
{
	BUG_ON(plane == OMAP_DSS_GFX);

	dispc_write_reg(DISPC_OVL_FIR_COEF_HV2(plane, reg), value);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void _dispc_write_firv2_reg(enum omap_plane plane, int reg, u32 value)
=======
static void dispc_ovl_write_firv2_reg(enum omap_plane plane, int reg, u32 value)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void dispc_ovl_write_firv2_reg(enum omap_plane plane, int reg, u32 value)
>>>>>>> refs/remotes/origin/master
{
	BUG_ON(plane == OMAP_DSS_GFX);

	dispc_write_reg(DISPC_OVL_FIR_COEF_V2(plane, reg), value);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void _dispc_set_scale_coef(enum omap_plane plane, int hscaleup,
				  int vscaleup, int five_taps,
				  enum omap_color_component color_comp)
{
	/* Coefficients for horizontal up-sampling */
	static const struct dispc_h_coef coef_hup[8] = {
		{  0,   0, 128,   0,  0 },
		{ -1,  13, 124,  -8,  0 },
		{ -2,  30, 112, -11, -1 },
		{ -5,  51,  95, -11, -2 },
		{  0,  -9,  73,  73, -9 },
		{ -2, -11,  95,  51, -5 },
		{ -1, -11, 112,  30, -2 },
		{  0,  -8, 124,  13, -1 },
	};

	/* Coefficients for vertical up-sampling */
	static const struct dispc_v_coef coef_vup_3tap[8] = {
		{ 0,  0, 128,  0, 0 },
		{ 0,  3, 123,  2, 0 },
		{ 0, 12, 111,  5, 0 },
		{ 0, 32,  89,  7, 0 },
		{ 0,  0,  64, 64, 0 },
		{ 0,  7,  89, 32, 0 },
		{ 0,  5, 111, 12, 0 },
		{ 0,  2, 123,  3, 0 },
	};

	static const struct dispc_v_coef coef_vup_5tap[8] = {
		{  0,   0, 128,   0,  0 },
		{ -1,  13, 124,  -8,  0 },
		{ -2,  30, 112, -11, -1 },
		{ -5,  51,  95, -11, -2 },
		{  0,  -9,  73,  73, -9 },
		{ -2, -11,  95,  51, -5 },
		{ -1, -11, 112,  30, -2 },
		{  0,  -8, 124,  13, -1 },
	};

	/* Coefficients for horizontal down-sampling */
	static const struct dispc_h_coef coef_hdown[8] = {
		{   0, 36, 56, 36,  0 },
		{   4, 40, 55, 31, -2 },
		{   8, 44, 54, 27, -5 },
		{  12, 48, 53, 22, -7 },
		{  -9, 17, 52, 51, 17 },
		{  -7, 22, 53, 48, 12 },
		{  -5, 27, 54, 44,  8 },
		{  -2, 31, 55, 40,  4 },
	};

	/* Coefficients for vertical down-sampling */
	static const struct dispc_v_coef coef_vdown_3tap[8] = {
		{ 0, 36, 56, 36, 0 },
		{ 0, 40, 57, 31, 0 },
		{ 0, 45, 56, 27, 0 },
		{ 0, 50, 55, 23, 0 },
		{ 0, 18, 55, 55, 0 },
		{ 0, 23, 55, 50, 0 },
		{ 0, 27, 56, 45, 0 },
		{ 0, 31, 57, 40, 0 },
	};

	static const struct dispc_v_coef coef_vdown_5tap[8] = {
		{   0, 36, 56, 36,  0 },
		{   4, 40, 55, 31, -2 },
		{   8, 44, 54, 27, -5 },
		{  12, 48, 53, 22, -7 },
		{  -9, 17, 52, 51, 17 },
		{  -7, 22, 53, 48, 12 },
		{  -5, 27, 54, 44,  8 },
		{  -2, 31, 55, 40,  4 },
	};

	const struct dispc_h_coef *h_coef;
	const struct dispc_v_coef *v_coef;
	int i;

	if (hscaleup)
		h_coef = coef_hup;
	else
		h_coef = coef_hdown;

	if (vscaleup)
		v_coef = five_taps ? coef_vup_5tap : coef_vup_3tap;
	else
		v_coef = five_taps ? coef_vdown_5tap : coef_vdown_3tap;
=======
=======
>>>>>>> refs/remotes/origin/master
static void dispc_ovl_set_scale_coef(enum omap_plane plane, int fir_hinc,
				int fir_vinc, int five_taps,
				enum omap_color_component color_comp)
{
	const struct dispc_coef *h_coef, *v_coef;
	int i;

	h_coef = dispc_ovl_get_scale_coef(fir_hinc, true);
	v_coef = dispc_ovl_get_scale_coef(fir_vinc, five_taps);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	for (i = 0; i < 8; i++) {
		u32 h, hv;

<<<<<<< HEAD
<<<<<<< HEAD
		h = FLD_VAL(h_coef[i].hc0, 7, 0)
			| FLD_VAL(h_coef[i].hc1, 15, 8)
			| FLD_VAL(h_coef[i].hc2, 23, 16)
			| FLD_VAL(h_coef[i].hc3, 31, 24);
		hv = FLD_VAL(h_coef[i].hc4, 7, 0)
			| FLD_VAL(v_coef[i].vc0, 15, 8)
			| FLD_VAL(v_coef[i].vc1, 23, 16)
			| FLD_VAL(v_coef[i].vc2, 31, 24);

		if (color_comp == DISPC_COLOR_COMPONENT_RGB_Y) {
			_dispc_write_firh_reg(plane, i, h);
			_dispc_write_firhv_reg(plane, i, hv);
		} else {
			_dispc_write_firh2_reg(plane, i, h);
			_dispc_write_firhv2_reg(plane, i, hv);
=======
=======
>>>>>>> refs/remotes/origin/master
		h = FLD_VAL(h_coef[i].hc0_vc00, 7, 0)
			| FLD_VAL(h_coef[i].hc1_vc0, 15, 8)
			| FLD_VAL(h_coef[i].hc2_vc1, 23, 16)
			| FLD_VAL(h_coef[i].hc3_vc2, 31, 24);
		hv = FLD_VAL(h_coef[i].hc4_vc22, 7, 0)
			| FLD_VAL(v_coef[i].hc1_vc0, 15, 8)
			| FLD_VAL(v_coef[i].hc2_vc1, 23, 16)
			| FLD_VAL(v_coef[i].hc3_vc2, 31, 24);

		if (color_comp == DISPC_COLOR_COMPONENT_RGB_Y) {
			dispc_ovl_write_firh_reg(plane, i, h);
			dispc_ovl_write_firhv_reg(plane, i, hv);
		} else {
			dispc_ovl_write_firh2_reg(plane, i, h);
			dispc_ovl_write_firhv2_reg(plane, i, hv);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		}

	}

	if (five_taps) {
		for (i = 0; i < 8; i++) {
			u32 v;
<<<<<<< HEAD
<<<<<<< HEAD
			v = FLD_VAL(v_coef[i].vc00, 7, 0)
				| FLD_VAL(v_coef[i].vc22, 15, 8);
			if (color_comp == DISPC_COLOR_COMPONENT_RGB_Y)
				_dispc_write_firv_reg(plane, i, v);
			else
				_dispc_write_firv2_reg(plane, i, v);
=======
=======
>>>>>>> refs/remotes/origin/master
			v = FLD_VAL(v_coef[i].hc0_vc00, 7, 0)
				| FLD_VAL(v_coef[i].hc4_vc22, 15, 8);
			if (color_comp == DISPC_COLOR_COMPONENT_RGB_Y)
				dispc_ovl_write_firv_reg(plane, i, v);
			else
				dispc_ovl_write_firv2_reg(plane, i, v);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		}
	}
}

<<<<<<< HEAD
static void _dispc_setup_color_conv_coef(void)
{
<<<<<<< HEAD
=======
	int i;
>>>>>>> refs/remotes/origin/cm-10.0
	const struct color_conv_coef {
		int  ry,  rcr,  rcb,   gy,  gcr,  gcb,   by,  bcr,  bcb;
		int  full_range;
	}  ctbl_bt601_5 = {
		298,  409,    0,  298, -208, -100,  298,    0,  517, 0,
	};

	const struct color_conv_coef *ct;

#define CVAL(x, y) (FLD_VAL(x, 26, 16) | FLD_VAL(y, 10, 0))

	ct = &ctbl_bt601_5;

<<<<<<< HEAD
	dispc_write_reg(DISPC_OVL_CONV_COEF(OMAP_DSS_VIDEO1, 0),
		CVAL(ct->rcr, ct->ry));
	dispc_write_reg(DISPC_OVL_CONV_COEF(OMAP_DSS_VIDEO1, 1),
		CVAL(ct->gy,  ct->rcb));
	dispc_write_reg(DISPC_OVL_CONV_COEF(OMAP_DSS_VIDEO1, 2),
		CVAL(ct->gcb, ct->gcr));
	dispc_write_reg(DISPC_OVL_CONV_COEF(OMAP_DSS_VIDEO1, 3),
		CVAL(ct->bcr, ct->by));
	dispc_write_reg(DISPC_OVL_CONV_COEF(OMAP_DSS_VIDEO1, 4),
		CVAL(0, ct->bcb));

	dispc_write_reg(DISPC_OVL_CONV_COEF(OMAP_DSS_VIDEO2, 0),
		CVAL(ct->rcr, ct->ry));
	dispc_write_reg(DISPC_OVL_CONV_COEF(OMAP_DSS_VIDEO2, 1),
		CVAL(ct->gy, ct->rcb));
	dispc_write_reg(DISPC_OVL_CONV_COEF(OMAP_DSS_VIDEO2, 2),
		CVAL(ct->gcb, ct->gcr));
	dispc_write_reg(DISPC_OVL_CONV_COEF(OMAP_DSS_VIDEO2, 3),
		CVAL(ct->bcr, ct->by));
	dispc_write_reg(DISPC_OVL_CONV_COEF(OMAP_DSS_VIDEO2, 4),
		CVAL(0, ct->bcb));

#undef CVAL

	REG_FLD_MOD(DISPC_OVL_ATTRIBUTES(OMAP_DSS_VIDEO1),
		ct->full_range, 11, 11);
	REG_FLD_MOD(DISPC_OVL_ATTRIBUTES(OMAP_DSS_VIDEO2),
		ct->full_range, 11, 11);
}


static void _dispc_set_plane_ba0(enum omap_plane plane, u32 paddr)
=======
	for (i = 1; i < dss_feat_get_num_ovls(); i++) {
		dispc_write_reg(DISPC_OVL_CONV_COEF(i, 0),
			CVAL(ct->rcr, ct->ry));
		dispc_write_reg(DISPC_OVL_CONV_COEF(i, 1),
			CVAL(ct->gy,  ct->rcb));
		dispc_write_reg(DISPC_OVL_CONV_COEF(i, 2),
			CVAL(ct->gcb, ct->gcr));
		dispc_write_reg(DISPC_OVL_CONV_COEF(i, 3),
			CVAL(ct->bcr, ct->by));
		dispc_write_reg(DISPC_OVL_CONV_COEF(i, 4),
			CVAL(0, ct->bcb));

		REG_FLD_MOD(DISPC_OVL_ATTRIBUTES(i), ct->full_range,
			11, 11);
	}

#undef CVAL
}


static void dispc_ovl_set_ba0(enum omap_plane plane, u32 paddr)
>>>>>>> refs/remotes/origin/cm-10.0
=======

static void dispc_ovl_write_color_conv_coef(enum omap_plane plane,
		const struct color_conv_coef *ct)
{
#define CVAL(x, y) (FLD_VAL(x, 26, 16) | FLD_VAL(y, 10, 0))

	dispc_write_reg(DISPC_OVL_CONV_COEF(plane, 0), CVAL(ct->rcr, ct->ry));
	dispc_write_reg(DISPC_OVL_CONV_COEF(plane, 1), CVAL(ct->gy,  ct->rcb));
	dispc_write_reg(DISPC_OVL_CONV_COEF(plane, 2), CVAL(ct->gcb, ct->gcr));
	dispc_write_reg(DISPC_OVL_CONV_COEF(plane, 3), CVAL(ct->bcr, ct->by));
	dispc_write_reg(DISPC_OVL_CONV_COEF(plane, 4), CVAL(0, ct->bcb));

	REG_FLD_MOD(DISPC_OVL_ATTRIBUTES(plane), ct->full_range, 11, 11);

#undef CVAL
}

static void dispc_setup_color_conv_coef(void)
{
	int i;
	int num_ovl = dss_feat_get_num_ovls();
	int num_wb = dss_feat_get_num_wbs();
	const struct color_conv_coef ctbl_bt601_5_ovl = {
		298, 409, 0, 298, -208, -100, 298, 0, 517, 0,
	};
	const struct color_conv_coef ctbl_bt601_5_wb = {
		66, 112, -38, 129, -94, -74, 25, -18, 112, 0,
	};

	for (i = 1; i < num_ovl; i++)
		dispc_ovl_write_color_conv_coef(i, &ctbl_bt601_5_ovl);

	for (; i < num_wb; i++)
		dispc_ovl_write_color_conv_coef(i, &ctbl_bt601_5_wb);
}

static void dispc_ovl_set_ba0(enum omap_plane plane, u32 paddr)
>>>>>>> refs/remotes/origin/master
{
	dispc_write_reg(DISPC_OVL_BA0(plane), paddr);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void _dispc_set_plane_ba1(enum omap_plane plane, u32 paddr)
=======
static void dispc_ovl_set_ba1(enum omap_plane plane, u32 paddr)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void dispc_ovl_set_ba1(enum omap_plane plane, u32 paddr)
>>>>>>> refs/remotes/origin/master
{
	dispc_write_reg(DISPC_OVL_BA1(plane), paddr);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void _dispc_set_plane_ba0_uv(enum omap_plane plane, u32 paddr)
=======
static void dispc_ovl_set_ba0_uv(enum omap_plane plane, u32 paddr)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void dispc_ovl_set_ba0_uv(enum omap_plane plane, u32 paddr)
>>>>>>> refs/remotes/origin/master
{
	dispc_write_reg(DISPC_OVL_BA0_UV(plane), paddr);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void _dispc_set_plane_ba1_uv(enum omap_plane plane, u32 paddr)
=======
static void dispc_ovl_set_ba1_uv(enum omap_plane plane, u32 paddr)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void dispc_ovl_set_ba1_uv(enum omap_plane plane, u32 paddr)
>>>>>>> refs/remotes/origin/master
{
	dispc_write_reg(DISPC_OVL_BA1_UV(plane), paddr);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void _dispc_set_plane_pos(enum omap_plane plane, int x, int y)
=======
static void dispc_ovl_set_pos(enum omap_plane plane, int x, int y)
>>>>>>> refs/remotes/origin/cm-10.0
{
	u32 val = FLD_VAL(y, 26, 16) | FLD_VAL(x, 10, 0);
=======
static void dispc_ovl_set_pos(enum omap_plane plane,
		enum omap_overlay_caps caps, int x, int y)
{
	u32 val;

	if ((caps & OMAP_DSS_OVL_CAP_POS) == 0)
		return;

	val = FLD_VAL(y, 26, 16) | FLD_VAL(x, 10, 0);
>>>>>>> refs/remotes/origin/master

	dispc_write_reg(DISPC_OVL_POSITION(plane), val);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void _dispc_set_pic_size(enum omap_plane plane, int width, int height)
=======
static void dispc_ovl_set_pic_size(enum omap_plane plane, int width, int height)
>>>>>>> refs/remotes/origin/cm-10.0
{
	u32 val = FLD_VAL(height - 1, 26, 16) | FLD_VAL(width - 1, 10, 0);

	if (plane == OMAP_DSS_GFX)
=======
static void dispc_ovl_set_input_size(enum omap_plane plane, int width,
		int height)
{
	u32 val = FLD_VAL(height - 1, 26, 16) | FLD_VAL(width - 1, 10, 0);

	if (plane == OMAP_DSS_GFX || plane == OMAP_DSS_WB)
>>>>>>> refs/remotes/origin/master
		dispc_write_reg(DISPC_OVL_SIZE(plane), val);
	else
		dispc_write_reg(DISPC_OVL_PICTURE_SIZE(plane), val);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void _dispc_set_vid_size(enum omap_plane plane, int width, int height)
=======
static void dispc_ovl_set_vid_size(enum omap_plane plane, int width, int height)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void dispc_ovl_set_output_size(enum omap_plane plane, int width,
		int height)
>>>>>>> refs/remotes/origin/master
{
	u32 val;

	BUG_ON(plane == OMAP_DSS_GFX);

	val = FLD_VAL(height - 1, 26, 16) | FLD_VAL(width - 1, 10, 0);

<<<<<<< HEAD
	dispc_write_reg(DISPC_OVL_SIZE(plane), val);
}

<<<<<<< HEAD
static void _dispc_set_pre_mult_alpha(enum omap_plane plane, bool enable)
{
	if (!dss_has_feature(FEAT_PRE_MULT_ALPHA))
		return;

	if (!dss_has_feature(FEAT_GLOBAL_ALPHA_VID1) &&
		plane == OMAP_DSS_VIDEO1)
=======
static void dispc_ovl_set_zorder(enum omap_plane plane, u8 zorder)
{
	struct omap_overlay *ovl = omap_dss_get_overlay(plane);

	if ((ovl->caps & OMAP_DSS_OVL_CAP_ZORDER) == 0)
=======
	if (plane == OMAP_DSS_WB)
		dispc_write_reg(DISPC_OVL_PICTURE_SIZE(plane), val);
	else
		dispc_write_reg(DISPC_OVL_SIZE(plane), val);
}

static void dispc_ovl_set_zorder(enum omap_plane plane,
		enum omap_overlay_caps caps, u8 zorder)
{
	if ((caps & OMAP_DSS_OVL_CAP_ZORDER) == 0)
>>>>>>> refs/remotes/origin/master
		return;

	REG_FLD_MOD(DISPC_OVL_ATTRIBUTES(plane), zorder, 27, 26);
}

static void dispc_ovl_enable_zorder_planes(void)
{
	int i;

	if (!dss_has_feature(FEAT_ALPHA_FREE_ZORDER))
		return;

	for (i = 0; i < dss_feat_get_num_ovls(); i++)
		REG_FLD_MOD(DISPC_OVL_ATTRIBUTES(i), 1, 25, 25);
}

<<<<<<< HEAD
static void dispc_ovl_set_pre_mult_alpha(enum omap_plane plane, bool enable)
{
	struct omap_overlay *ovl = omap_dss_get_overlay(plane);

	if ((ovl->caps & OMAP_DSS_OVL_CAP_PRE_MULT_ALPHA) == 0)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void dispc_ovl_set_pre_mult_alpha(enum omap_plane plane,
		enum omap_overlay_caps caps, bool enable)
{
	if ((caps & OMAP_DSS_OVL_CAP_PRE_MULT_ALPHA) == 0)
>>>>>>> refs/remotes/origin/master
		return;

	REG_FLD_MOD(DISPC_OVL_ATTRIBUTES(plane), enable ? 1 : 0, 28, 28);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void _dispc_setup_global_alpha(enum omap_plane plane, u8 global_alpha)
{
	if (!dss_has_feature(FEAT_GLOBAL_ALPHA))
		return;

	if (!dss_has_feature(FEAT_GLOBAL_ALPHA_VID1) &&
		plane == OMAP_DSS_VIDEO1)
		return;

	if (plane == OMAP_DSS_GFX)
		REG_FLD_MOD(DISPC_GLOBAL_ALPHA, global_alpha, 7, 0);
	else if (plane == OMAP_DSS_VIDEO2)
		REG_FLD_MOD(DISPC_GLOBAL_ALPHA, global_alpha, 23, 16);
}

static void _dispc_set_pix_inc(enum omap_plane plane, s32 inc)
=======
static void dispc_ovl_setup_global_alpha(enum omap_plane plane, u8 global_alpha)
{
	static const unsigned shifts[] = { 0, 8, 16, 24, };
	int shift;
	struct omap_overlay *ovl = omap_dss_get_overlay(plane);

	if ((ovl->caps & OMAP_DSS_OVL_CAP_GLOBAL_ALPHA) == 0)
=======
static void dispc_ovl_setup_global_alpha(enum omap_plane plane,
		enum omap_overlay_caps caps, u8 global_alpha)
{
	static const unsigned shifts[] = { 0, 8, 16, 24, };
	int shift;

	if ((caps & OMAP_DSS_OVL_CAP_GLOBAL_ALPHA) == 0)
>>>>>>> refs/remotes/origin/master
		return;

	shift = shifts[plane];
	REG_FLD_MOD(DISPC_GLOBAL_ALPHA, global_alpha, shift + 7, shift);
}

static void dispc_ovl_set_pix_inc(enum omap_plane plane, s32 inc)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	dispc_write_reg(DISPC_OVL_PIXEL_INC(plane), inc);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void _dispc_set_row_inc(enum omap_plane plane, s32 inc)
=======
static void dispc_ovl_set_row_inc(enum omap_plane plane, s32 inc)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void dispc_ovl_set_row_inc(enum omap_plane plane, s32 inc)
>>>>>>> refs/remotes/origin/master
{
	dispc_write_reg(DISPC_OVL_ROW_INC(plane), inc);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void _dispc_set_color_mode(enum omap_plane plane,
=======
static void dispc_ovl_set_color_mode(enum omap_plane plane,
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void dispc_ovl_set_color_mode(enum omap_plane plane,
>>>>>>> refs/remotes/origin/master
		enum omap_color_mode color_mode)
{
	u32 m = 0;
	if (plane != OMAP_DSS_GFX) {
		switch (color_mode) {
		case OMAP_DSS_COLOR_NV12:
			m = 0x0; break;
<<<<<<< HEAD
<<<<<<< HEAD
		case OMAP_DSS_COLOR_RGB12U:
			m = 0x1; break;
		case OMAP_DSS_COLOR_RGBA16:
			m = 0x2; break;
		case OMAP_DSS_COLOR_RGBX16:
=======
=======
>>>>>>> refs/remotes/origin/master
		case OMAP_DSS_COLOR_RGBX16:
			m = 0x1; break;
		case OMAP_DSS_COLOR_RGBA16:
			m = 0x2; break;
		case OMAP_DSS_COLOR_RGB12U:
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			m = 0x4; break;
		case OMAP_DSS_COLOR_ARGB16:
			m = 0x5; break;
		case OMAP_DSS_COLOR_RGB16:
			m = 0x6; break;
		case OMAP_DSS_COLOR_ARGB16_1555:
			m = 0x7; break;
		case OMAP_DSS_COLOR_RGB24U:
			m = 0x8; break;
		case OMAP_DSS_COLOR_RGB24P:
			m = 0x9; break;
		case OMAP_DSS_COLOR_YUV2:
			m = 0xa; break;
		case OMAP_DSS_COLOR_UYVY:
			m = 0xb; break;
		case OMAP_DSS_COLOR_ARGB32:
			m = 0xc; break;
		case OMAP_DSS_COLOR_RGBA32:
			m = 0xd; break;
		case OMAP_DSS_COLOR_RGBX32:
			m = 0xe; break;
		case OMAP_DSS_COLOR_XRGB16_1555:
			m = 0xf; break;
		default:
<<<<<<< HEAD
			BUG(); break;
=======
			BUG(); return;
>>>>>>> refs/remotes/origin/master
		}
	} else {
		switch (color_mode) {
		case OMAP_DSS_COLOR_CLUT1:
			m = 0x0; break;
		case OMAP_DSS_COLOR_CLUT2:
			m = 0x1; break;
		case OMAP_DSS_COLOR_CLUT4:
			m = 0x2; break;
		case OMAP_DSS_COLOR_CLUT8:
			m = 0x3; break;
		case OMAP_DSS_COLOR_RGB12U:
			m = 0x4; break;
		case OMAP_DSS_COLOR_ARGB16:
			m = 0x5; break;
		case OMAP_DSS_COLOR_RGB16:
			m = 0x6; break;
		case OMAP_DSS_COLOR_ARGB16_1555:
			m = 0x7; break;
		case OMAP_DSS_COLOR_RGB24U:
			m = 0x8; break;
		case OMAP_DSS_COLOR_RGB24P:
			m = 0x9; break;
<<<<<<< HEAD
<<<<<<< HEAD
		case OMAP_DSS_COLOR_YUV2:
			m = 0xa; break;
		case OMAP_DSS_COLOR_UYVY:
=======
		case OMAP_DSS_COLOR_RGBX16:
			m = 0xa; break;
		case OMAP_DSS_COLOR_RGBA16:
>>>>>>> refs/remotes/origin/cm-10.0
=======
		case OMAP_DSS_COLOR_RGBX16:
			m = 0xa; break;
		case OMAP_DSS_COLOR_RGBA16:
>>>>>>> refs/remotes/origin/master
			m = 0xb; break;
		case OMAP_DSS_COLOR_ARGB32:
			m = 0xc; break;
		case OMAP_DSS_COLOR_RGBA32:
			m = 0xd; break;
		case OMAP_DSS_COLOR_RGBX32:
			m = 0xe; break;
		case OMAP_DSS_COLOR_XRGB16_1555:
			m = 0xf; break;
		default:
<<<<<<< HEAD
			BUG(); break;
=======
			BUG(); return;
>>>>>>> refs/remotes/origin/master
		}
	}

	REG_FLD_MOD(DISPC_OVL_ATTRIBUTES(plane), m, 4, 1);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void _dispc_set_channel_out(enum omap_plane plane,
		enum omap_channel channel)
=======
void dispc_ovl_set_channel_out(enum omap_plane plane, enum omap_channel channel)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void dispc_ovl_configure_burst_type(enum omap_plane plane,
		enum omap_dss_rotation_type rotation_type)
{
	if (dss_has_feature(FEAT_BURST_2D) == 0)
		return;

	if (rotation_type == OMAP_DSS_ROT_TILER)
		REG_FLD_MOD(DISPC_OVL_ATTRIBUTES(plane), 1, 29, 29);
	else
		REG_FLD_MOD(DISPC_OVL_ATTRIBUTES(plane), 0, 29, 29);
}

void dispc_ovl_set_channel_out(enum omap_plane plane, enum omap_channel channel)
>>>>>>> refs/remotes/origin/master
{
	int shift;
	u32 val;
	int chan = 0, chan2 = 0;

	switch (plane) {
	case OMAP_DSS_GFX:
		shift = 8;
		break;
	case OMAP_DSS_VIDEO1:
	case OMAP_DSS_VIDEO2:
<<<<<<< HEAD
<<<<<<< HEAD
=======
	case OMAP_DSS_VIDEO3:
>>>>>>> refs/remotes/origin/cm-10.0
=======
	case OMAP_DSS_VIDEO3:
>>>>>>> refs/remotes/origin/master
		shift = 16;
		break;
	default:
		BUG();
		return;
	}

	val = dispc_read_reg(DISPC_OVL_ATTRIBUTES(plane));
	if (dss_has_feature(FEAT_MGR_LCD2)) {
		switch (channel) {
		case OMAP_DSS_CHANNEL_LCD:
			chan = 0;
			chan2 = 0;
			break;
		case OMAP_DSS_CHANNEL_DIGIT:
			chan = 1;
			chan2 = 0;
			break;
		case OMAP_DSS_CHANNEL_LCD2:
			chan = 0;
			chan2 = 1;
			break;
<<<<<<< HEAD
		default:
			BUG();
=======
		case OMAP_DSS_CHANNEL_LCD3:
			if (dss_has_feature(FEAT_MGR_LCD3)) {
				chan = 0;
				chan2 = 2;
			} else {
				BUG();
				return;
			}
			break;
		default:
			BUG();
			return;
>>>>>>> refs/remotes/origin/master
		}

		val = FLD_MOD(val, chan, shift, shift);
		val = FLD_MOD(val, chan2, 31, 30);
	} else {
		val = FLD_MOD(val, channel, shift, shift);
	}
	dispc_write_reg(DISPC_OVL_ATTRIBUTES(plane), val);
}
<<<<<<< HEAD

<<<<<<< HEAD
void dispc_set_burst_size(enum omap_plane plane,
		enum omap_burst_size burst_size)
{
	int shift;
	u32 val;

	enable_clocks(1);

	switch (plane) {
	case OMAP_DSS_GFX:
		shift = 6;
		break;
	case OMAP_DSS_VIDEO1:
	case OMAP_DSS_VIDEO2:
		shift = 14;
		break;
	default:
		BUG();
		return;
	}

	val = dispc_read_reg(DISPC_OVL_ATTRIBUTES(plane));
	val = FLD_MOD(val, burst_size, shift+1, shift);
	dispc_write_reg(DISPC_OVL_ATTRIBUTES(plane), val);

	enable_clocks(0);
=======
=======
EXPORT_SYMBOL(dispc_ovl_set_channel_out);

>>>>>>> refs/remotes/origin/master
static enum omap_channel dispc_ovl_get_channel_out(enum omap_plane plane)
{
	int shift;
	u32 val;
	enum omap_channel channel;

	switch (plane) {
	case OMAP_DSS_GFX:
		shift = 8;
		break;
	case OMAP_DSS_VIDEO1:
	case OMAP_DSS_VIDEO2:
	case OMAP_DSS_VIDEO3:
		shift = 16;
		break;
	default:
		BUG();
<<<<<<< HEAD
=======
		return 0;
>>>>>>> refs/remotes/origin/master
	}

	val = dispc_read_reg(DISPC_OVL_ATTRIBUTES(plane));

<<<<<<< HEAD
	if (dss_has_feature(FEAT_MGR_LCD2)) {
=======
	if (dss_has_feature(FEAT_MGR_LCD3)) {
		if (FLD_GET(val, 31, 30) == 0)
			channel = FLD_GET(val, shift, shift);
		else if (FLD_GET(val, 31, 30) == 1)
			channel = OMAP_DSS_CHANNEL_LCD2;
		else
			channel = OMAP_DSS_CHANNEL_LCD3;
	} else if (dss_has_feature(FEAT_MGR_LCD2)) {
>>>>>>> refs/remotes/origin/master
		if (FLD_GET(val, 31, 30) == 0)
			channel = FLD_GET(val, shift, shift);
		else
			channel = OMAP_DSS_CHANNEL_LCD2;
	} else {
		channel = FLD_GET(val, shift, shift);
	}

	return channel;
}

<<<<<<< HEAD
static void dispc_ovl_set_burst_size(enum omap_plane plane,
		enum omap_burst_size burst_size)
{
	static const unsigned shifts[] = { 6, 14, 14, 14, };
=======
void dispc_wb_set_channel_in(enum dss_writeback_channel channel)
{
	enum omap_plane plane = OMAP_DSS_WB;

	REG_FLD_MOD(DISPC_OVL_ATTRIBUTES(plane), channel, 18, 16);
}

static void dispc_ovl_set_burst_size(enum omap_plane plane,
		enum omap_burst_size burst_size)
{
	static const unsigned shifts[] = { 6, 14, 14, 14, 14, };
>>>>>>> refs/remotes/origin/master
	int shift;

	shift = shifts[plane];
	REG_FLD_MOD(DISPC_OVL_ATTRIBUTES(plane), burst_size, shift + 1, shift);
}

static void dispc_configure_burst_sizes(void)
{
	int i;
	const int burst_size = BURST_SIZE_X8;

	/* Configure burst size always to maximum size */
<<<<<<< HEAD
	for (i = 0; i < omap_dss_get_num_overlays(); ++i)
=======
	for (i = 0; i < dss_feat_get_num_ovls(); ++i)
>>>>>>> refs/remotes/origin/master
		dispc_ovl_set_burst_size(i, burst_size);
}

static u32 dispc_ovl_get_burst_size(enum omap_plane plane)
{
	unsigned unit = dss_feat_get_burst_size_unit();
	/* burst multiplier is always x8 (see dispc_configure_burst_sizes()) */
	return unit * 8;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

void dispc_enable_gamma_table(bool enable)
{
	/*
	 * This is partially implemented to support only disabling of
	 * the gamma table.
	 */
	if (enable) {
		DSSWARN("Gamma table enabling for TV not yet supported");
		return;
	}

	REG_FLD_MOD(DISPC_CONFIG, enable, 9, 9);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void _dispc_set_vid_color_conv(enum omap_plane plane, bool enable)
=======
static void dispc_mgr_enable_cpr(enum omap_channel channel, bool enable)
{
	u16 reg;

	if (channel == OMAP_DSS_CHANNEL_LCD)
		reg = DISPC_CONFIG;
	else if (channel == OMAP_DSS_CHANNEL_LCD2)
		reg = DISPC_CONFIG2;
	else
		return;

	REG_FLD_MOD(reg, enable, 15, 15);
}

static void dispc_mgr_set_cpr_coef(enum omap_channel channel,
		struct omap_dss_cpr_coefs *coefs)
{
	u32 coef_r, coef_g, coef_b;

	if (!dispc_mgr_is_lcd(channel))
=======
static void dispc_mgr_enable_cpr(enum omap_channel channel, bool enable)
{
	if (channel == OMAP_DSS_CHANNEL_DIGIT)
		return;

	mgr_fld_write(channel, DISPC_MGR_FLD_CPR, enable);
}

static void dispc_mgr_set_cpr_coef(enum omap_channel channel,
		const struct omap_dss_cpr_coefs *coefs)
{
	u32 coef_r, coef_g, coef_b;

	if (!dss_mgr_is_lcd(channel))
>>>>>>> refs/remotes/origin/master
		return;

	coef_r = FLD_VAL(coefs->rr, 31, 22) | FLD_VAL(coefs->rg, 20, 11) |
		FLD_VAL(coefs->rb, 9, 0);
	coef_g = FLD_VAL(coefs->gr, 31, 22) | FLD_VAL(coefs->gg, 20, 11) |
		FLD_VAL(coefs->gb, 9, 0);
	coef_b = FLD_VAL(coefs->br, 31, 22) | FLD_VAL(coefs->bg, 20, 11) |
		FLD_VAL(coefs->bb, 9, 0);

	dispc_write_reg(DISPC_CPR_COEF_R(channel), coef_r);
	dispc_write_reg(DISPC_CPR_COEF_G(channel), coef_g);
	dispc_write_reg(DISPC_CPR_COEF_B(channel), coef_b);
}

static void dispc_ovl_set_vid_color_conv(enum omap_plane plane, bool enable)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	u32 val;

	BUG_ON(plane == OMAP_DSS_GFX);

	val = dispc_read_reg(DISPC_OVL_ATTRIBUTES(plane));
	val = FLD_MOD(val, enable, 9, 9);
	dispc_write_reg(DISPC_OVL_ATTRIBUTES(plane), val);
}

<<<<<<< HEAD
<<<<<<< HEAD
void dispc_enable_replication(enum omap_plane plane, bool enable)
{
	int bit;

	if (plane == OMAP_DSS_GFX)
		bit = 5;
	else
		bit = 10;

	enable_clocks(1);
	REG_FLD_MOD(DISPC_OVL_ATTRIBUTES(plane), enable, bit, bit);
	enable_clocks(0);
}

void dispc_set_lcd_size(enum omap_channel channel, u16 width, u16 height)
=======
static void dispc_ovl_enable_replication(enum omap_plane plane, bool enable)
=======
static void dispc_ovl_enable_replication(enum omap_plane plane,
		enum omap_overlay_caps caps, bool enable)
>>>>>>> refs/remotes/origin/master
{
	static const unsigned shifts[] = { 5, 10, 10, 10 };
	int shift;

<<<<<<< HEAD
=======
	if ((caps & OMAP_DSS_OVL_CAP_REPLICATION) == 0)
		return;

>>>>>>> refs/remotes/origin/master
	shift = shifts[plane];
	REG_FLD_MOD(DISPC_OVL_ATTRIBUTES(plane), enable, shift, shift);
}

<<<<<<< HEAD
void dispc_mgr_set_lcd_size(enum omap_channel channel, u16 width, u16 height)
>>>>>>> refs/remotes/origin/cm-10.0
{
	u32 val;
	BUG_ON((width > (1 << 11)) || (height > (1 << 11)));
	val = FLD_VAL(height - 1, 26, 16) | FLD_VAL(width - 1, 10, 0);
<<<<<<< HEAD
	enable_clocks(1);
	dispc_write_reg(DISPC_SIZE_MGR(channel), val);
	enable_clocks(0);
=======
	dispc_write_reg(DISPC_SIZE_MGR(channel), val);
>>>>>>> refs/remotes/origin/cm-10.0
}

void dispc_set_digit_size(u16 width, u16 height)
{
	u32 val;
	BUG_ON((width > (1 << 11)) || (height > (1 << 11)));
	val = FLD_VAL(height - 1, 26, 16) | FLD_VAL(width - 1, 10, 0);
<<<<<<< HEAD
	enable_clocks(1);
	dispc_write_reg(DISPC_SIZE_MGR(OMAP_DSS_CHANNEL_DIGIT), val);
	enable_clocks(0);
=======
	dispc_write_reg(DISPC_SIZE_MGR(OMAP_DSS_CHANNEL_DIGIT), val);
>>>>>>> refs/remotes/origin/cm-10.0
}

static void dispc_read_plane_fifo_sizes(void)
{
	u32 size;
	int plane;
	u8 start, end;
<<<<<<< HEAD

	enable_clocks(1);

	dss_feat_get_reg_field(FEAT_REG_FIFOSIZE, &start, &end);

	for (plane = 0; plane < ARRAY_SIZE(dispc.fifo_size); ++plane) {
		size = FLD_GET(dispc_read_reg(DISPC_OVL_FIFO_SIZE_STATUS(plane)),
			start, end);
		dispc.fifo_size[plane] = size;
	}

	enable_clocks(0);
}

u32 dispc_get_plane_fifo_size(enum omap_plane plane)
=======
	u32 unit;

	unit = dss_feat_get_buffer_size_unit();

	dss_feat_get_reg_field(FEAT_REG_FIFOSIZE, &start, &end);

	for (plane = 0; plane < dss_feat_get_num_ovls(); ++plane) {
		size = REG_GET(DISPC_OVL_FIFO_SIZE_STATUS(plane), start, end);
		size *= unit;
		dispc.fifo_size[plane] = size;
=======
static void dispc_mgr_set_size(enum omap_channel channel, u16 width,
		u16 height)
{
	u32 val;

	val = FLD_VAL(height - 1, dispc.feat->mgr_height_start, 16) |
		FLD_VAL(width - 1, dispc.feat->mgr_width_start, 0);

	dispc_write_reg(DISPC_SIZE_MGR(channel), val);
}

static void dispc_init_fifos(void)
{
	u32 size;
	int fifo;
	u8 start, end;
	u32 unit;

	unit = dss_feat_get_buffer_size_unit();

	dss_feat_get_reg_field(FEAT_REG_FIFOSIZE, &start, &end);

	for (fifo = 0; fifo < dispc.feat->num_fifos; ++fifo) {
		size = REG_GET(DISPC_OVL_FIFO_SIZE_STATUS(fifo), start, end);
		size *= unit;
		dispc.fifo_size[fifo] = size;

		/*
		 * By default fifos are mapped directly to overlays, fifo 0 to
		 * ovl 0, fifo 1 to ovl 1, etc.
		 */
		dispc.fifo_assignment[fifo] = fifo;
	}

	/*
	 * The GFX fifo on OMAP4 is smaller than the other fifos. The small fifo
	 * causes problems with certain use cases, like using the tiler in 2D
	 * mode. The below hack swaps the fifos of GFX and WB planes, thus
	 * giving GFX plane a larger fifo. WB but should work fine with a
	 * smaller fifo.
	 */
	if (dispc.feat->gfx_fifo_workaround) {
		u32 v;

		v = dispc_read_reg(DISPC_GLOBAL_BUFFER);

		v = FLD_MOD(v, 4, 2, 0); /* GFX BUF top to WB */
		v = FLD_MOD(v, 4, 5, 3); /* GFX BUF bottom to WB */
		v = FLD_MOD(v, 0, 26, 24); /* WB BUF top to GFX */
		v = FLD_MOD(v, 0, 29, 27); /* WB BUF bottom to GFX */

		dispc_write_reg(DISPC_GLOBAL_BUFFER, v);

		dispc.fifo_assignment[OMAP_DSS_GFX] = OMAP_DSS_WB;
		dispc.fifo_assignment[OMAP_DSS_WB] = OMAP_DSS_GFX;
>>>>>>> refs/remotes/origin/master
	}
}

static u32 dispc_ovl_get_fifo_size(enum omap_plane plane)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
{
	return dispc.fifo_size[plane];
}

<<<<<<< HEAD
void dispc_setup_plane_fifo(enum omap_plane plane, u32 low, u32 high)
{
	u8 hi_start, hi_end, lo_start, lo_end;
=======
=======
{
	int fifo;
	u32 size = 0;

	for (fifo = 0; fifo < dispc.feat->num_fifos; ++fifo) {
		if (dispc.fifo_assignment[fifo] == plane)
			size += dispc.fifo_size[fifo];
	}

	return size;
}

>>>>>>> refs/remotes/origin/master
void dispc_ovl_set_fifo_threshold(enum omap_plane plane, u32 low, u32 high)
{
	u8 hi_start, hi_end, lo_start, lo_end;
	u32 unit;

	unit = dss_feat_get_buffer_size_unit();

	WARN_ON(low % unit != 0);
	WARN_ON(high % unit != 0);

	low /= unit;
	high /= unit;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	dss_feat_get_reg_field(FEAT_REG_FIFOHIGHTHRESHOLD, &hi_start, &hi_end);
	dss_feat_get_reg_field(FEAT_REG_FIFOLOWTHRESHOLD, &lo_start, &lo_end);

<<<<<<< HEAD
<<<<<<< HEAD
	enable_clocks(1);

	DSSDBG("fifo(%d) low/high old %u/%u, new %u/%u\n",
			plane,
			REG_GET(DISPC_OVL_FIFO_THRESHOLD(plane),
				lo_start, lo_end),
			REG_GET(DISPC_OVL_FIFO_THRESHOLD(plane),
				hi_start, hi_end),
			low, high);
=======
=======
>>>>>>> refs/remotes/origin/master
	DSSDBG("fifo(%d) threshold (bytes), old %u/%u, new %u/%u\n",
			plane,
			REG_GET(DISPC_OVL_FIFO_THRESHOLD(plane),
				lo_start, lo_end) * unit,
			REG_GET(DISPC_OVL_FIFO_THRESHOLD(plane),
				hi_start, hi_end) * unit,
			low * unit, high * unit);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	dispc_write_reg(DISPC_OVL_FIFO_THRESHOLD(plane),
			FLD_VAL(high, hi_start, hi_end) |
			FLD_VAL(low, lo_start, lo_end));
<<<<<<< HEAD
<<<<<<< HEAD

	enable_clocks(0);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

void dispc_enable_fifomerge(bool enable)
{
<<<<<<< HEAD
<<<<<<< HEAD
	enable_clocks(1);

	DSSDBG("FIFO merge %s\n", enable ? "enabled" : "disabled");
	REG_FLD_MOD(DISPC_CONFIG, enable ? 1 : 0, 14, 14);

	enable_clocks(0);
}

static void _dispc_set_fir(enum omap_plane plane,
=======
=======
>>>>>>> refs/remotes/origin/master
	if (!dss_has_feature(FEAT_FIFO_MERGE)) {
		WARN_ON(enable);
		return;
	}

	DSSDBG("FIFO merge %s\n", enable ? "enabled" : "disabled");
	REG_FLD_MOD(DISPC_CONFIG, enable ? 1 : 0, 14, 14);
}

void dispc_ovl_compute_fifo_thresholds(enum omap_plane plane,
		u32 *fifo_low, u32 *fifo_high, bool use_fifomerge,
		bool manual_update)
{
	/*
	 * All sizes are in bytes. Both the buffer and burst are made of
	 * buffer_units, and the fifo thresholds must be buffer_unit aligned.
	 */

	unsigned buf_unit = dss_feat_get_buffer_size_unit();
	unsigned ovl_fifo_size, total_fifo_size, burst_size;
	int i;

	burst_size = dispc_ovl_get_burst_size(plane);
	ovl_fifo_size = dispc_ovl_get_fifo_size(plane);

	if (use_fifomerge) {
		total_fifo_size = 0;
<<<<<<< HEAD
		for (i = 0; i < omap_dss_get_num_overlays(); ++i)
=======
		for (i = 0; i < dss_feat_get_num_ovls(); ++i)
>>>>>>> refs/remotes/origin/master
			total_fifo_size += dispc_ovl_get_fifo_size(i);
	} else {
		total_fifo_size = ovl_fifo_size;
	}

	/*
	 * We use the same low threshold for both fifomerge and non-fifomerge
	 * cases, but for fifomerge we calculate the high threshold using the
	 * combined fifo size
	 */

	if (manual_update && dss_has_feature(FEAT_OMAP3_DSI_FIFO_BUG)) {
		*fifo_low = ovl_fifo_size - burst_size * 2;
		*fifo_high = total_fifo_size - burst_size;
<<<<<<< HEAD
=======
	} else if (plane == OMAP_DSS_WB) {
		/*
		 * Most optimal configuration for writeback is to push out data
		 * to the interconnect the moment writeback pushes enough pixels
		 * in the FIFO to form a burst
		 */
		*fifo_low = 0;
		*fifo_high = burst_size;
>>>>>>> refs/remotes/origin/master
	} else {
		*fifo_low = ovl_fifo_size - burst_size;
		*fifo_high = total_fifo_size - buf_unit;
	}
}

static void dispc_ovl_set_fir(enum omap_plane plane,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				int hinc, int vinc,
				enum omap_color_component color_comp)
{
	u32 val;

	if (color_comp == DISPC_COLOR_COMPONENT_RGB_Y) {
		u8 hinc_start, hinc_end, vinc_start, vinc_end;

		dss_feat_get_reg_field(FEAT_REG_FIRHINC,
					&hinc_start, &hinc_end);
		dss_feat_get_reg_field(FEAT_REG_FIRVINC,
					&vinc_start, &vinc_end);
		val = FLD_VAL(vinc, vinc_start, vinc_end) |
				FLD_VAL(hinc, hinc_start, hinc_end);

		dispc_write_reg(DISPC_OVL_FIR(plane), val);
	} else {
		val = FLD_VAL(vinc, 28, 16) | FLD_VAL(hinc, 12, 0);
		dispc_write_reg(DISPC_OVL_FIR2(plane), val);
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
static void _dispc_set_vid_accu0(enum omap_plane plane, int haccu, int vaccu)
=======
static void dispc_ovl_set_vid_accu0(enum omap_plane plane, int haccu, int vaccu)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void dispc_ovl_set_vid_accu0(enum omap_plane plane, int haccu, int vaccu)
>>>>>>> refs/remotes/origin/master
{
	u32 val;
	u8 hor_start, hor_end, vert_start, vert_end;

	dss_feat_get_reg_field(FEAT_REG_HORIZONTALACCU, &hor_start, &hor_end);
	dss_feat_get_reg_field(FEAT_REG_VERTICALACCU, &vert_start, &vert_end);

	val = FLD_VAL(vaccu, vert_start, vert_end) |
			FLD_VAL(haccu, hor_start, hor_end);

	dispc_write_reg(DISPC_OVL_ACCU0(plane), val);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void _dispc_set_vid_accu1(enum omap_plane plane, int haccu, int vaccu)
=======
static void dispc_ovl_set_vid_accu1(enum omap_plane plane, int haccu, int vaccu)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void dispc_ovl_set_vid_accu1(enum omap_plane plane, int haccu, int vaccu)
>>>>>>> refs/remotes/origin/master
{
	u32 val;
	u8 hor_start, hor_end, vert_start, vert_end;

	dss_feat_get_reg_field(FEAT_REG_HORIZONTALACCU, &hor_start, &hor_end);
	dss_feat_get_reg_field(FEAT_REG_VERTICALACCU, &vert_start, &vert_end);

	val = FLD_VAL(vaccu, vert_start, vert_end) |
			FLD_VAL(haccu, hor_start, hor_end);

	dispc_write_reg(DISPC_OVL_ACCU1(plane), val);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void _dispc_set_vid_accu2_0(enum omap_plane plane, int haccu, int vaccu)
=======
static void dispc_ovl_set_vid_accu2_0(enum omap_plane plane, int haccu,
		int vaccu)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void dispc_ovl_set_vid_accu2_0(enum omap_plane plane, int haccu,
		int vaccu)
>>>>>>> refs/remotes/origin/master
{
	u32 val;

	val = FLD_VAL(vaccu, 26, 16) | FLD_VAL(haccu, 10, 0);
	dispc_write_reg(DISPC_OVL_ACCU2_0(plane), val);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void _dispc_set_vid_accu2_1(enum omap_plane plane, int haccu, int vaccu)
=======
static void dispc_ovl_set_vid_accu2_1(enum omap_plane plane, int haccu,
		int vaccu)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void dispc_ovl_set_vid_accu2_1(enum omap_plane plane, int haccu,
		int vaccu)
>>>>>>> refs/remotes/origin/master
{
	u32 val;

	val = FLD_VAL(vaccu, 26, 16) | FLD_VAL(haccu, 10, 0);
	dispc_write_reg(DISPC_OVL_ACCU2_1(plane), val);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void _dispc_set_scale_param(enum omap_plane plane,
=======
static void dispc_ovl_set_scale_param(enum omap_plane plane,
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void dispc_ovl_set_scale_param(enum omap_plane plane,
>>>>>>> refs/remotes/origin/master
		u16 orig_width, u16 orig_height,
		u16 out_width, u16 out_height,
		bool five_taps, u8 rotation,
		enum omap_color_component color_comp)
{
	int fir_hinc, fir_vinc;
<<<<<<< HEAD
<<<<<<< HEAD
	int hscaleup, vscaleup;

	hscaleup = orig_width <= out_width;
	vscaleup = orig_height <= out_height;

	_dispc_set_scale_coef(plane, hscaleup, vscaleup, five_taps, color_comp);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	fir_hinc = 1024 * orig_width / out_width;
	fir_vinc = 1024 * orig_height / out_height;

<<<<<<< HEAD
<<<<<<< HEAD
	_dispc_set_fir(plane, fir_hinc, fir_vinc, color_comp);
}

static void _dispc_set_scaling_common(enum omap_plane plane,
=======
=======
>>>>>>> refs/remotes/origin/master
	dispc_ovl_set_scale_coef(plane, fir_hinc, fir_vinc, five_taps,
				color_comp);
	dispc_ovl_set_fir(plane, fir_hinc, fir_vinc, color_comp);
}

<<<<<<< HEAD
static void dispc_ovl_set_scaling_common(enum omap_plane plane,
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void dispc_ovl_set_accu_uv(enum omap_plane plane,
		u16 orig_width,	u16 orig_height, u16 out_width, u16 out_height,
		bool ilace, enum omap_color_mode color_mode, u8 rotation)
{
	int h_accu2_0, h_accu2_1;
	int v_accu2_0, v_accu2_1;
	int chroma_hinc, chroma_vinc;
	int idx;

	struct accu {
		s8 h0_m, h0_n;
		s8 h1_m, h1_n;
		s8 v0_m, v0_n;
		s8 v1_m, v1_n;
	};

	const struct accu *accu_table;
	const struct accu *accu_val;

	static const struct accu accu_nv12[4] = {
		{  0, 1,  0, 1 , -1, 2, 0, 1 },
		{  1, 2, -3, 4 ,  0, 1, 0, 1 },
		{ -1, 1,  0, 1 , -1, 2, 0, 1 },
		{ -1, 2, -1, 2 , -1, 1, 0, 1 },
	};

	static const struct accu accu_nv12_ilace[4] = {
		{  0, 1,  0, 1 , -3, 4, -1, 4 },
		{ -1, 4, -3, 4 ,  0, 1,  0, 1 },
		{ -1, 1,  0, 1 , -1, 4, -3, 4 },
		{ -3, 4, -3, 4 , -1, 1,  0, 1 },
	};

	static const struct accu accu_yuv[4] = {
		{  0, 1, 0, 1,  0, 1, 0, 1 },
		{  0, 1, 0, 1,  0, 1, 0, 1 },
		{ -1, 1, 0, 1,  0, 1, 0, 1 },
		{  0, 1, 0, 1, -1, 1, 0, 1 },
	};

	switch (rotation) {
	case OMAP_DSS_ROT_0:
		idx = 0;
		break;
	case OMAP_DSS_ROT_90:
		idx = 1;
		break;
	case OMAP_DSS_ROT_180:
		idx = 2;
		break;
	case OMAP_DSS_ROT_270:
		idx = 3;
		break;
	default:
		BUG();
		return;
	}

	switch (color_mode) {
	case OMAP_DSS_COLOR_NV12:
		if (ilace)
			accu_table = accu_nv12_ilace;
		else
			accu_table = accu_nv12;
		break;
	case OMAP_DSS_COLOR_YUV2:
	case OMAP_DSS_COLOR_UYVY:
		accu_table = accu_yuv;
		break;
	default:
		BUG();
		return;
	}

	accu_val = &accu_table[idx];

	chroma_hinc = 1024 * orig_width / out_width;
	chroma_vinc = 1024 * orig_height / out_height;

	h_accu2_0 = (accu_val->h0_m * chroma_hinc / accu_val->h0_n) % 1024;
	h_accu2_1 = (accu_val->h1_m * chroma_hinc / accu_val->h1_n) % 1024;
	v_accu2_0 = (accu_val->v0_m * chroma_vinc / accu_val->v0_n) % 1024;
	v_accu2_1 = (accu_val->v1_m * chroma_vinc / accu_val->v1_n) % 1024;

	dispc_ovl_set_vid_accu2_0(plane, h_accu2_0, v_accu2_0);
	dispc_ovl_set_vid_accu2_1(plane, h_accu2_1, v_accu2_1);
}

static void dispc_ovl_set_scaling_common(enum omap_plane plane,
>>>>>>> refs/remotes/origin/master
		u16 orig_width, u16 orig_height,
		u16 out_width, u16 out_height,
		bool ilace, bool five_taps,
		bool fieldmode, enum omap_color_mode color_mode,
		u8 rotation)
{
	int accu0 = 0;
	int accu1 = 0;
	u32 l;

<<<<<<< HEAD
<<<<<<< HEAD
	_dispc_set_scale_param(plane, orig_width, orig_height,
=======
	dispc_ovl_set_scale_param(plane, orig_width, orig_height,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	dispc_ovl_set_scale_param(plane, orig_width, orig_height,
>>>>>>> refs/remotes/origin/master
				out_width, out_height, five_taps,
				rotation, DISPC_COLOR_COMPONENT_RGB_Y);
	l = dispc_read_reg(DISPC_OVL_ATTRIBUTES(plane));

	/* RESIZEENABLE and VERTICALTAPS */
	l &= ~((0x3 << 5) | (0x1 << 21));
	l |= (orig_width != out_width) ? (1 << 5) : 0;
	l |= (orig_height != out_height) ? (1 << 6) : 0;
	l |= five_taps ? (1 << 21) : 0;

	/* VRESIZECONF and HRESIZECONF */
	if (dss_has_feature(FEAT_RESIZECONF)) {
		l &= ~(0x3 << 7);
		l |= (orig_width <= out_width) ? 0 : (1 << 7);
		l |= (orig_height <= out_height) ? 0 : (1 << 8);
	}

	/* LINEBUFFERSPLIT */
	if (dss_has_feature(FEAT_LINEBUFFERSPLIT)) {
		l &= ~(0x1 << 22);
		l |= five_taps ? (1 << 22) : 0;
	}

	dispc_write_reg(DISPC_OVL_ATTRIBUTES(plane), l);

	/*
	 * field 0 = even field = bottom field
	 * field 1 = odd field = top field
	 */
	if (ilace && !fieldmode) {
		accu1 = 0;
		accu0 = ((1024 * orig_height / out_height) / 2) & 0x3ff;
		if (accu0 >= 1024/2) {
			accu1 = 1024/2;
			accu0 -= accu1;
		}
	}

<<<<<<< HEAD
<<<<<<< HEAD
	_dispc_set_vid_accu0(plane, 0, accu0);
	_dispc_set_vid_accu1(plane, 0, accu1);
}

static void _dispc_set_scaling_uv(enum omap_plane plane,
=======
=======
>>>>>>> refs/remotes/origin/master
	dispc_ovl_set_vid_accu0(plane, 0, accu0);
	dispc_ovl_set_vid_accu1(plane, 0, accu1);
}

static void dispc_ovl_set_scaling_uv(enum omap_plane plane,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		u16 orig_width, u16 orig_height,
		u16 out_width, u16 out_height,
		bool ilace, bool five_taps,
		bool fieldmode, enum omap_color_mode color_mode,
		u8 rotation)
{
	int scale_x = out_width != orig_width;
	int scale_y = out_height != orig_height;
<<<<<<< HEAD
=======
	bool chroma_upscale = plane != OMAP_DSS_WB ? true : false;
>>>>>>> refs/remotes/origin/master

	if (!dss_has_feature(FEAT_HANDLE_UV_SEPARATE))
		return;
	if ((color_mode != OMAP_DSS_COLOR_YUV2 &&
			color_mode != OMAP_DSS_COLOR_UYVY &&
			color_mode != OMAP_DSS_COLOR_NV12)) {
		/* reset chroma resampling for RGB formats  */
<<<<<<< HEAD
		REG_FLD_MOD(DISPC_OVL_ATTRIBUTES2(plane), 0, 8, 8);
		return;
	}
	switch (color_mode) {
	case OMAP_DSS_COLOR_NV12:
		/* UV is subsampled by 2 vertically*/
		orig_height >>= 1;
		/* UV is subsampled by 2 horz.*/
		orig_width >>= 1;
		break;
	case OMAP_DSS_COLOR_YUV2:
	case OMAP_DSS_COLOR_UYVY:
		/*For YUV422 with 90/270 rotation,
		 *we don't upsample chroma
		 */
		if (rotation == OMAP_DSS_ROT_0 ||
			rotation == OMAP_DSS_ROT_180)
			/* UV is subsampled by 2 hrz*/
			orig_width >>= 1;
		/* must use FIR for YUV422 if rotated */
		if (rotation != OMAP_DSS_ROT_0)
			scale_x = scale_y = true;
		break;
	default:
		BUG();
=======
		if (plane != OMAP_DSS_WB)
			REG_FLD_MOD(DISPC_OVL_ATTRIBUTES2(plane), 0, 8, 8);
		return;
	}

	dispc_ovl_set_accu_uv(plane, orig_width, orig_height, out_width,
			out_height, ilace, color_mode, rotation);

	switch (color_mode) {
	case OMAP_DSS_COLOR_NV12:
		if (chroma_upscale) {
			/* UV is subsampled by 2 horizontally and vertically */
			orig_height >>= 1;
			orig_width >>= 1;
		} else {
			/* UV is downsampled by 2 horizontally and vertically */
			orig_height <<= 1;
			orig_width <<= 1;
		}

		break;
	case OMAP_DSS_COLOR_YUV2:
	case OMAP_DSS_COLOR_UYVY:
		/* For YUV422 with 90/270 rotation, we don't upsample chroma */
		if (rotation == OMAP_DSS_ROT_0 ||
				rotation == OMAP_DSS_ROT_180) {
			if (chroma_upscale)
				/* UV is subsampled by 2 horizontally */
				orig_width >>= 1;
			else
				/* UV is downsampled by 2 horizontally */
				orig_width <<= 1;
		}

		/* must use FIR for YUV422 if rotated */
		if (rotation != OMAP_DSS_ROT_0)
			scale_x = scale_y = true;

		break;
	default:
		BUG();
		return;
>>>>>>> refs/remotes/origin/master
	}

	if (out_width != orig_width)
		scale_x = true;
	if (out_height != orig_height)
		scale_y = true;

<<<<<<< HEAD
<<<<<<< HEAD
	_dispc_set_scale_param(plane, orig_width, orig_height,
=======
	dispc_ovl_set_scale_param(plane, orig_width, orig_height,
>>>>>>> refs/remotes/origin/cm-10.0
			out_width, out_height, five_taps,
				rotation, DISPC_COLOR_COMPONENT_UV);

	REG_FLD_MOD(DISPC_OVL_ATTRIBUTES2(plane),
		(scale_x || scale_y) ? 1 : 0, 8, 8);
=======
	dispc_ovl_set_scale_param(plane, orig_width, orig_height,
			out_width, out_height, five_taps,
				rotation, DISPC_COLOR_COMPONENT_UV);

	if (plane != OMAP_DSS_WB)
		REG_FLD_MOD(DISPC_OVL_ATTRIBUTES2(plane),
			(scale_x || scale_y) ? 1 : 0, 8, 8);

>>>>>>> refs/remotes/origin/master
	/* set H scaling */
	REG_FLD_MOD(DISPC_OVL_ATTRIBUTES(plane), scale_x ? 1 : 0, 5, 5);
	/* set V scaling */
	REG_FLD_MOD(DISPC_OVL_ATTRIBUTES(plane), scale_y ? 1 : 0, 6, 6);
<<<<<<< HEAD

<<<<<<< HEAD
	_dispc_set_vid_accu2_0(plane, 0x80, 0);
	_dispc_set_vid_accu2_1(plane, 0x80, 0);
}

static void _dispc_set_scaling(enum omap_plane plane,
=======
	dispc_ovl_set_vid_accu2_0(plane, 0x80, 0);
	dispc_ovl_set_vid_accu2_1(plane, 0x80, 0);
}

static void dispc_ovl_set_scaling(enum omap_plane plane,
>>>>>>> refs/remotes/origin/cm-10.0
=======
}

static void dispc_ovl_set_scaling(enum omap_plane plane,
>>>>>>> refs/remotes/origin/master
		u16 orig_width, u16 orig_height,
		u16 out_width, u16 out_height,
		bool ilace, bool five_taps,
		bool fieldmode, enum omap_color_mode color_mode,
		u8 rotation)
{
	BUG_ON(plane == OMAP_DSS_GFX);

<<<<<<< HEAD
<<<<<<< HEAD
	_dispc_set_scaling_common(plane,
=======
	dispc_ovl_set_scaling_common(plane,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	dispc_ovl_set_scaling_common(plane,
>>>>>>> refs/remotes/origin/master
			orig_width, orig_height,
			out_width, out_height,
			ilace, five_taps,
			fieldmode, color_mode,
			rotation);

<<<<<<< HEAD
<<<<<<< HEAD
	_dispc_set_scaling_uv(plane,
=======
	dispc_ovl_set_scaling_uv(plane,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	dispc_ovl_set_scaling_uv(plane,
>>>>>>> refs/remotes/origin/master
		orig_width, orig_height,
		out_width, out_height,
		ilace, five_taps,
		fieldmode, color_mode,
		rotation);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void _dispc_set_rotation_attrs(enum omap_plane plane, u8 rotation,
=======
static void dispc_ovl_set_rotation_attrs(enum omap_plane plane, u8 rotation,
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void dispc_ovl_set_rotation_attrs(enum omap_plane plane, u8 rotation,
		enum omap_dss_rotation_type rotation_type,
>>>>>>> refs/remotes/origin/master
		bool mirroring, enum omap_color_mode color_mode)
{
	bool row_repeat = false;
	int vidrot = 0;

	if (color_mode == OMAP_DSS_COLOR_YUV2 ||
			color_mode == OMAP_DSS_COLOR_UYVY) {

		if (mirroring) {
			switch (rotation) {
			case OMAP_DSS_ROT_0:
				vidrot = 2;
				break;
			case OMAP_DSS_ROT_90:
				vidrot = 1;
				break;
			case OMAP_DSS_ROT_180:
				vidrot = 0;
				break;
			case OMAP_DSS_ROT_270:
				vidrot = 3;
				break;
			}
		} else {
			switch (rotation) {
			case OMAP_DSS_ROT_0:
				vidrot = 0;
				break;
			case OMAP_DSS_ROT_90:
				vidrot = 1;
				break;
			case OMAP_DSS_ROT_180:
				vidrot = 2;
				break;
			case OMAP_DSS_ROT_270:
				vidrot = 3;
				break;
			}
		}

		if (rotation == OMAP_DSS_ROT_90 || rotation == OMAP_DSS_ROT_270)
			row_repeat = true;
		else
			row_repeat = false;
	}

	REG_FLD_MOD(DISPC_OVL_ATTRIBUTES(plane), vidrot, 13, 12);
	if (dss_has_feature(FEAT_ROWREPEATENABLE))
		REG_FLD_MOD(DISPC_OVL_ATTRIBUTES(plane),
			row_repeat ? 1 : 0, 18, 18);
<<<<<<< HEAD
=======

	if (color_mode == OMAP_DSS_COLOR_NV12) {
		bool doublestride = (rotation_type == OMAP_DSS_ROT_TILER) &&
					(rotation == OMAP_DSS_ROT_0 ||
					rotation == OMAP_DSS_ROT_180);
		/* DOUBLESTRIDE */
		REG_FLD_MOD(DISPC_OVL_ATTRIBUTES(plane), doublestride, 22, 22);
	}

>>>>>>> refs/remotes/origin/master
}

static int color_mode_to_bpp(enum omap_color_mode color_mode)
{
	switch (color_mode) {
	case OMAP_DSS_COLOR_CLUT1:
		return 1;
	case OMAP_DSS_COLOR_CLUT2:
		return 2;
	case OMAP_DSS_COLOR_CLUT4:
		return 4;
	case OMAP_DSS_COLOR_CLUT8:
	case OMAP_DSS_COLOR_NV12:
		return 8;
	case OMAP_DSS_COLOR_RGB12U:
	case OMAP_DSS_COLOR_RGB16:
	case OMAP_DSS_COLOR_ARGB16:
	case OMAP_DSS_COLOR_YUV2:
	case OMAP_DSS_COLOR_UYVY:
	case OMAP_DSS_COLOR_RGBA16:
	case OMAP_DSS_COLOR_RGBX16:
	case OMAP_DSS_COLOR_ARGB16_1555:
	case OMAP_DSS_COLOR_XRGB16_1555:
		return 16;
	case OMAP_DSS_COLOR_RGB24P:
		return 24;
	case OMAP_DSS_COLOR_RGB24U:
	case OMAP_DSS_COLOR_ARGB32:
	case OMAP_DSS_COLOR_RGBA32:
	case OMAP_DSS_COLOR_RGBX32:
		return 32;
	default:
		BUG();
<<<<<<< HEAD
=======
		return 0;
>>>>>>> refs/remotes/origin/master
	}
}

static s32 pixinc(int pixels, u8 ps)
{
	if (pixels == 1)
		return 1;
	else if (pixels > 1)
		return 1 + (pixels - 1) * ps;
	else if (pixels < 0)
		return 1 - (-pixels + 1) * ps;
	else
		BUG();
<<<<<<< HEAD
=======
		return 0;
>>>>>>> refs/remotes/origin/master
}

static void calc_vrfb_rotation_offset(u8 rotation, bool mirror,
		u16 screen_width,
		u16 width, u16 height,
		enum omap_color_mode color_mode, bool fieldmode,
		unsigned int field_offset,
		unsigned *offset0, unsigned *offset1,
<<<<<<< HEAD
		s32 *row_inc, s32 *pix_inc)
=======
		s32 *row_inc, s32 *pix_inc, int x_predecim, int y_predecim)
>>>>>>> refs/remotes/origin/master
{
	u8 ps;

	/* FIXME CLUT formats */
	switch (color_mode) {
	case OMAP_DSS_COLOR_CLUT1:
	case OMAP_DSS_COLOR_CLUT2:
	case OMAP_DSS_COLOR_CLUT4:
	case OMAP_DSS_COLOR_CLUT8:
		BUG();
		return;
	case OMAP_DSS_COLOR_YUV2:
	case OMAP_DSS_COLOR_UYVY:
		ps = 4;
		break;
	default:
		ps = color_mode_to_bpp(color_mode) / 8;
		break;
	}

	DSSDBG("calc_rot(%d): scrw %d, %dx%d\n", rotation, screen_width,
			width, height);

	/*
	 * field 0 = even field = bottom field
	 * field 1 = odd field = top field
	 */
	switch (rotation + mirror * 4) {
	case OMAP_DSS_ROT_0:
	case OMAP_DSS_ROT_180:
		/*
		 * If the pixel format is YUV or UYVY divide the width
		 * of the image by 2 for 0 and 180 degree rotation.
		 */
		if (color_mode == OMAP_DSS_COLOR_YUV2 ||
			color_mode == OMAP_DSS_COLOR_UYVY)
			width = width >> 1;
	case OMAP_DSS_ROT_90:
	case OMAP_DSS_ROT_270:
		*offset1 = 0;
		if (field_offset)
			*offset0 = field_offset * screen_width * ps;
		else
			*offset0 = 0;

<<<<<<< HEAD
		*row_inc = pixinc(1 + (screen_width - width) +
				(fieldmode ? screen_width : 0),
				ps);
		*pix_inc = pixinc(1, ps);
=======
		*row_inc = pixinc(1 +
			(y_predecim * screen_width - x_predecim * width) +
			(fieldmode ? screen_width : 0), ps);
		*pix_inc = pixinc(x_predecim, ps);
>>>>>>> refs/remotes/origin/master
		break;

	case OMAP_DSS_ROT_0 + 4:
	case OMAP_DSS_ROT_180 + 4:
		/* If the pixel format is YUV or UYVY divide the width
		 * of the image by 2  for 0 degree and 180 degree
		 */
		if (color_mode == OMAP_DSS_COLOR_YUV2 ||
			color_mode == OMAP_DSS_COLOR_UYVY)
			width = width >> 1;
	case OMAP_DSS_ROT_90 + 4:
	case OMAP_DSS_ROT_270 + 4:
		*offset1 = 0;
		if (field_offset)
			*offset0 = field_offset * screen_width * ps;
		else
			*offset0 = 0;
<<<<<<< HEAD
		*row_inc = pixinc(1 - (screen_width + width) -
				(fieldmode ? screen_width : 0),
				ps);
		*pix_inc = pixinc(1, ps);
=======
		*row_inc = pixinc(1 -
			(y_predecim * screen_width + x_predecim * width) -
			(fieldmode ? screen_width : 0), ps);
		*pix_inc = pixinc(x_predecim, ps);
>>>>>>> refs/remotes/origin/master
		break;

	default:
		BUG();
<<<<<<< HEAD
=======
		return;
>>>>>>> refs/remotes/origin/master
	}
}

static void calc_dma_rotation_offset(u8 rotation, bool mirror,
		u16 screen_width,
		u16 width, u16 height,
		enum omap_color_mode color_mode, bool fieldmode,
		unsigned int field_offset,
		unsigned *offset0, unsigned *offset1,
<<<<<<< HEAD
		s32 *row_inc, s32 *pix_inc)
=======
		s32 *row_inc, s32 *pix_inc, int x_predecim, int y_predecim)
>>>>>>> refs/remotes/origin/master
{
	u8 ps;
	u16 fbw, fbh;

	/* FIXME CLUT formats */
	switch (color_mode) {
	case OMAP_DSS_COLOR_CLUT1:
	case OMAP_DSS_COLOR_CLUT2:
	case OMAP_DSS_COLOR_CLUT4:
	case OMAP_DSS_COLOR_CLUT8:
		BUG();
		return;
	default:
		ps = color_mode_to_bpp(color_mode) / 8;
		break;
	}

	DSSDBG("calc_rot(%d): scrw %d, %dx%d\n", rotation, screen_width,
			width, height);

	/* width & height are overlay sizes, convert to fb sizes */

	if (rotation == OMAP_DSS_ROT_0 || rotation == OMAP_DSS_ROT_180) {
		fbw = width;
		fbh = height;
	} else {
		fbw = height;
		fbh = width;
	}

	/*
	 * field 0 = even field = bottom field
	 * field 1 = odd field = top field
	 */
	switch (rotation + mirror * 4) {
	case OMAP_DSS_ROT_0:
		*offset1 = 0;
		if (field_offset)
			*offset0 = *offset1 + field_offset * screen_width * ps;
		else
			*offset0 = *offset1;
<<<<<<< HEAD
		*row_inc = pixinc(1 + (screen_width - fbw) +
				(fieldmode ? screen_width : 0),
				ps);
		*pix_inc = pixinc(1, ps);
=======
		*row_inc = pixinc(1 +
			(y_predecim * screen_width - fbw * x_predecim) +
			(fieldmode ? screen_width : 0),	ps);
		if (color_mode == OMAP_DSS_COLOR_YUV2 ||
			color_mode == OMAP_DSS_COLOR_UYVY)
			*pix_inc = pixinc(x_predecim, 2 * ps);
		else
			*pix_inc = pixinc(x_predecim, ps);
>>>>>>> refs/remotes/origin/master
		break;
	case OMAP_DSS_ROT_90:
		*offset1 = screen_width * (fbh - 1) * ps;
		if (field_offset)
			*offset0 = *offset1 + field_offset * ps;
		else
			*offset0 = *offset1;
<<<<<<< HEAD
		*row_inc = pixinc(screen_width * (fbh - 1) + 1 +
				(fieldmode ? 1 : 0), ps);
		*pix_inc = pixinc(-screen_width, ps);
=======
		*row_inc = pixinc(screen_width * (fbh * x_predecim - 1) +
				y_predecim + (fieldmode ? 1 : 0), ps);
		*pix_inc = pixinc(-x_predecim * screen_width, ps);
>>>>>>> refs/remotes/origin/master
		break;
	case OMAP_DSS_ROT_180:
		*offset1 = (screen_width * (fbh - 1) + fbw - 1) * ps;
		if (field_offset)
			*offset0 = *offset1 - field_offset * screen_width * ps;
		else
			*offset0 = *offset1;
		*row_inc = pixinc(-1 -
<<<<<<< HEAD
				(screen_width - fbw) -
				(fieldmode ? screen_width : 0),
				ps);
		*pix_inc = pixinc(-1, ps);
=======
			(y_predecim * screen_width - fbw * x_predecim) -
			(fieldmode ? screen_width : 0),	ps);
		if (color_mode == OMAP_DSS_COLOR_YUV2 ||
			color_mode == OMAP_DSS_COLOR_UYVY)
			*pix_inc = pixinc(-x_predecim, 2 * ps);
		else
			*pix_inc = pixinc(-x_predecim, ps);
>>>>>>> refs/remotes/origin/master
		break;
	case OMAP_DSS_ROT_270:
		*offset1 = (fbw - 1) * ps;
		if (field_offset)
			*offset0 = *offset1 - field_offset * ps;
		else
			*offset0 = *offset1;
<<<<<<< HEAD
		*row_inc = pixinc(-screen_width * (fbh - 1) - 1 -
				(fieldmode ? 1 : 0), ps);
		*pix_inc = pixinc(screen_width, ps);
=======
		*row_inc = pixinc(-screen_width * (fbh * x_predecim - 1) -
				y_predecim - (fieldmode ? 1 : 0), ps);
		*pix_inc = pixinc(x_predecim * screen_width, ps);
>>>>>>> refs/remotes/origin/master
		break;

	/* mirroring */
	case OMAP_DSS_ROT_0 + 4:
		*offset1 = (fbw - 1) * ps;
		if (field_offset)
			*offset0 = *offset1 + field_offset * screen_width * ps;
		else
			*offset0 = *offset1;
<<<<<<< HEAD
		*row_inc = pixinc(screen_width * 2 - 1 +
				(fieldmode ? screen_width : 0),
				ps);
		*pix_inc = pixinc(-1, ps);
=======
		*row_inc = pixinc(y_predecim * screen_width * 2 - 1 +
				(fieldmode ? screen_width : 0),
				ps);
		if (color_mode == OMAP_DSS_COLOR_YUV2 ||
			color_mode == OMAP_DSS_COLOR_UYVY)
			*pix_inc = pixinc(-x_predecim, 2 * ps);
		else
			*pix_inc = pixinc(-x_predecim, ps);
>>>>>>> refs/remotes/origin/master
		break;

	case OMAP_DSS_ROT_90 + 4:
		*offset1 = 0;
		if (field_offset)
			*offset0 = *offset1 + field_offset * ps;
		else
			*offset0 = *offset1;
<<<<<<< HEAD
		*row_inc = pixinc(-screen_width * (fbh - 1) + 1 +
				(fieldmode ? 1 : 0),
				ps);
		*pix_inc = pixinc(screen_width, ps);
=======
		*row_inc = pixinc(-screen_width * (fbh * x_predecim - 1) +
				y_predecim + (fieldmode ? 1 : 0),
				ps);
		*pix_inc = pixinc(x_predecim * screen_width, ps);
>>>>>>> refs/remotes/origin/master
		break;

	case OMAP_DSS_ROT_180 + 4:
		*offset1 = screen_width * (fbh - 1) * ps;
		if (field_offset)
			*offset0 = *offset1 - field_offset * screen_width * ps;
		else
			*offset0 = *offset1;
<<<<<<< HEAD
		*row_inc = pixinc(1 - screen_width * 2 -
				(fieldmode ? screen_width : 0),
				ps);
		*pix_inc = pixinc(1, ps);
=======
		*row_inc = pixinc(1 - y_predecim * screen_width * 2 -
				(fieldmode ? screen_width : 0),
				ps);
		if (color_mode == OMAP_DSS_COLOR_YUV2 ||
			color_mode == OMAP_DSS_COLOR_UYVY)
			*pix_inc = pixinc(x_predecim, 2 * ps);
		else
			*pix_inc = pixinc(x_predecim, ps);
>>>>>>> refs/remotes/origin/master
		break;

	case OMAP_DSS_ROT_270 + 4:
		*offset1 = (screen_width * (fbh - 1) + fbw - 1) * ps;
		if (field_offset)
			*offset0 = *offset1 - field_offset * ps;
		else
			*offset0 = *offset1;
<<<<<<< HEAD
		*row_inc = pixinc(screen_width * (fbh - 1) - 1 -
				(fieldmode ? 1 : 0),
				ps);
		*pix_inc = pixinc(-screen_width, ps);
=======
		*row_inc = pixinc(screen_width * (fbh * x_predecim - 1) -
				y_predecim - (fieldmode ? 1 : 0),
				ps);
		*pix_inc = pixinc(-x_predecim * screen_width, ps);
>>>>>>> refs/remotes/origin/master
		break;

	default:
		BUG();
<<<<<<< HEAD
	}
}

static unsigned long calc_fclk_five_taps(enum omap_channel channel, u16 width,
		u16 height, u16 out_width, u16 out_height,
		enum omap_color_mode color_mode)
{
	u32 fclk = 0;
<<<<<<< HEAD
	/* FIXME venc pclk? */
	u64 tmp, pclk = dispc_pclk_rate(channel);

	if (height > out_height) {
		/* FIXME get real display PPL */
		unsigned int ppl = 800;
=======
	u64 tmp, pclk = dispc_mgr_pclk_rate(channel);
=======
		return;
	}
}

static void calc_tiler_rotation_offset(u16 screen_width, u16 width,
		enum omap_color_mode color_mode, bool fieldmode,
		unsigned int field_offset, unsigned *offset0, unsigned *offset1,
		s32 *row_inc, s32 *pix_inc, int x_predecim, int y_predecim)
{
	u8 ps;

	switch (color_mode) {
	case OMAP_DSS_COLOR_CLUT1:
	case OMAP_DSS_COLOR_CLUT2:
	case OMAP_DSS_COLOR_CLUT4:
	case OMAP_DSS_COLOR_CLUT8:
		BUG();
		return;
	default:
		ps = color_mode_to_bpp(color_mode) / 8;
		break;
	}

	DSSDBG("scrw %d, width %d\n", screen_width, width);

	/*
	 * field 0 = even field = bottom field
	 * field 1 = odd field = top field
	 */
	*offset1 = 0;
	if (field_offset)
		*offset0 = *offset1 + field_offset * screen_width * ps;
	else
		*offset0 = *offset1;
	*row_inc = pixinc(1 + (y_predecim * screen_width - width * x_predecim) +
			(fieldmode ? screen_width : 0), ps);
	if (color_mode == OMAP_DSS_COLOR_YUV2 ||
		color_mode == OMAP_DSS_COLOR_UYVY)
		*pix_inc = pixinc(x_predecim, 2 * ps);
	else
		*pix_inc = pixinc(x_predecim, ps);
}

/*
 * This function is used to avoid synclosts in OMAP3, because of some
 * undocumented horizontal position and timing related limitations.
 */
static int check_horiz_timing_omap3(unsigned long pclk, unsigned long lclk,
		const struct omap_video_timings *t, u16 pos_x,
		u16 width, u16 height, u16 out_width, u16 out_height)
{
	const int ds = DIV_ROUND_UP(height, out_height);
	unsigned long nonactive;
	static const u8 limits[3] = { 8, 10, 20 };
	u64 val, blank;
	int i;

	nonactive = t->x_res + t->hfp + t->hsw + t->hbp - out_width;

	i = 0;
	if (out_height < height)
		i++;
	if (out_width < width)
		i++;
	blank = div_u64((u64)(t->hbp + t->hsw + t->hfp) * lclk, pclk);
	DSSDBG("blanking period + ppl = %llu (limit = %u)\n", blank, limits[i]);
	if (blank <= limits[i])
		return -EINVAL;

	/*
	 * Pixel data should be prepared before visible display point starts.
	 * So, atleast DS-2 lines must have already been fetched by DISPC
	 * during nonactive - pos_x period.
	 */
	val = div_u64((u64)(nonactive - pos_x) * lclk, pclk);
	DSSDBG("(nonactive - pos_x) * pcd = %llu max(0, DS - 2) * width = %d\n",
		val, max(0, ds - 2) * width);
	if (val < max(0, ds - 2) * width)
		return -EINVAL;

	/*
	 * All lines need to be refilled during the nonactive period of which
	 * only one line can be loaded during the active period. So, atleast
	 * DS - 1 lines should be loaded during nonactive period.
	 */
	val =  div_u64((u64)nonactive * lclk, pclk);
	DSSDBG("nonactive * pcd  = %llu, max(0, DS - 1) * width = %d\n",
		val, max(0, ds - 1) * width);
	if (val < max(0, ds - 1) * width)
		return -EINVAL;

	return 0;
}

static unsigned long calc_core_clk_five_taps(unsigned long pclk,
		const struct omap_video_timings *mgr_timings, u16 width,
		u16 height, u16 out_width, u16 out_height,
		enum omap_color_mode color_mode)
{
	u32 core_clk = 0;
	u64 tmp;
>>>>>>> refs/remotes/origin/master

	if (height <= out_height && width <= out_width)
		return (unsigned long) pclk;

	if (height > out_height) {
<<<<<<< HEAD
		struct omap_dss_device *dssdev = dispc_mgr_get_device(channel);
		unsigned int ppl = dssdev->panel.timings.x_res;
>>>>>>> refs/remotes/origin/cm-10.0

		tmp = pclk * height * out_width;
		do_div(tmp, 2 * out_height * ppl);
		fclk = tmp;
=======
		unsigned int ppl = mgr_timings->x_res;

		tmp = pclk * height * out_width;
		do_div(tmp, 2 * out_height * ppl);
		core_clk = tmp;
>>>>>>> refs/remotes/origin/master

		if (height > 2 * out_height) {
			if (ppl == out_width)
				return 0;

			tmp = pclk * (height - 2 * out_height) * out_width;
			do_div(tmp, 2 * out_height * (ppl - out_width));
<<<<<<< HEAD
			fclk = max(fclk, (u32) tmp);
=======
			core_clk = max_t(u32, core_clk, tmp);
>>>>>>> refs/remotes/origin/master
		}
	}

	if (width > out_width) {
		tmp = pclk * width;
		do_div(tmp, out_width);
<<<<<<< HEAD
		fclk = max(fclk, (u32) tmp);

		if (color_mode == OMAP_DSS_COLOR_RGB24U)
			fclk <<= 1;
	}

	return fclk;
}

static unsigned long calc_fclk(enum omap_channel channel, u16 width,
		u16 height, u16 out_width, u16 out_height)
{
	unsigned int hf, vf;
<<<<<<< HEAD
=======
	unsigned long pclk = dispc_mgr_pclk_rate(channel);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		core_clk = max_t(u32, core_clk, tmp);

		if (color_mode == OMAP_DSS_COLOR_RGB24U)
			core_clk <<= 1;
	}

	return core_clk;
}

static unsigned long calc_core_clk_24xx(unsigned long pclk, u16 width,
		u16 height, u16 out_width, u16 out_height, bool mem_to_mem)
{
	if (height > out_height && width > out_width)
		return pclk * 4;
	else
		return pclk * 2;
}

static unsigned long calc_core_clk_34xx(unsigned long pclk, u16 width,
		u16 height, u16 out_width, u16 out_height, bool mem_to_mem)
{
	unsigned int hf, vf;
>>>>>>> refs/remotes/origin/master

	/*
	 * FIXME how to determine the 'A' factor
	 * for the no downscaling case ?
	 */

	if (width > 3 * out_width)
		hf = 4;
	else if (width > 2 * out_width)
		hf = 3;
	else if (width > out_width)
		hf = 2;
	else
		hf = 1;
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
	if (height > out_height)
		vf = 2;
	else
		vf = 1;

<<<<<<< HEAD
<<<<<<< HEAD
	/* FIXME venc pclk? */
	return dispc_pclk_rate(channel) * vf * hf;
}

void dispc_set_channel_out(enum omap_plane plane, enum omap_channel channel_out)
{
	enable_clocks(1);
	_dispc_set_channel_out(plane, channel_out);
	enable_clocks(0);
}

static int _dispc_setup_plane(enum omap_plane plane,
		u32 paddr, u16 screen_width,
		u16 pos_x, u16 pos_y,
		u16 width, u16 height,
		u16 out_width, u16 out_height,
		enum omap_color_mode color_mode,
		bool ilace,
		enum omap_dss_rotation_type rotation_type,
		u8 rotation, int mirror,
		u8 global_alpha, u8 pre_mult_alpha,
		enum omap_channel channel, u32 puv_addr)
{
	const int maxdownscale = cpu_is_omap34xx() ? 4 : 2;
	bool five_taps = 0;
	bool fieldmode = 0;
	int cconv = 0;
	unsigned offset0, offset1;
	s32 row_inc;
	s32 pix_inc;
	u16 frame_height = height;
	unsigned int field_offset = 0;

	if (paddr == 0)
		return -EINVAL;

	if (ilace && height == out_height)
=======
	if (cpu_is_omap24xx()) {
		if (vf > 1 && hf > 1)
			return pclk * 4;
		else
			return pclk * 2;
	} else if (cpu_is_omap34xx()) {
		return pclk * vf * hf;
	} else {
		if (hf > 1)
			return DIV_ROUND_UP(pclk, out_width) * width;
		else
			return pclk;
	}
}

static int dispc_ovl_calc_scaling(enum omap_plane plane,
		enum omap_channel channel, u16 width, u16 height,
		u16 out_width, u16 out_height,
		enum omap_color_mode color_mode, bool *five_taps)
{
	struct omap_overlay *ovl = omap_dss_get_overlay(plane);
	const int maxdownscale = dss_feat_get_param_max(FEAT_PARAM_DOWNSCALE);
	const int maxsinglelinewidth =
				dss_feat_get_param_max(FEAT_PARAM_LINEWIDTH);
	unsigned long fclk = 0;

	if (width == out_width && height == out_height)
		return 0;

	if ((ovl->caps & OMAP_DSS_OVL_CAP_SCALE) == 0)
		return -EINVAL;

	if (out_width < width / maxdownscale ||
			out_width > width * 8)
		return -EINVAL;

	if (out_height < height / maxdownscale ||
			out_height > height * 8)
		return -EINVAL;

	if (cpu_is_omap24xx()) {
		if (width > maxsinglelinewidth)
			DSSERR("Cannot scale max input width exceeded");
		*five_taps = false;
		fclk = calc_fclk(channel, width, height, out_width,
								out_height);
	} else if (cpu_is_omap34xx()) {
		if (width > (maxsinglelinewidth * 2)) {
			DSSERR("Cannot setup scaling");
			DSSERR("width exceeds maximum width possible");
			return -EINVAL;
		}
		fclk = calc_fclk_five_taps(channel, width, height, out_width,
						out_height, color_mode);
		if (width > maxsinglelinewidth) {
			if (height > out_height && height < out_height * 2)
				*five_taps = false;
			else {
				DSSERR("cannot setup scaling with five taps");
				return -EINVAL;
			}
		}
		if (!*five_taps)
			fclk = calc_fclk(channel, width, height, out_width,
					out_height);
	} else {
		if (width > maxsinglelinewidth) {
			DSSERR("Cannot scale width exceeds max line width");
			return -EINVAL;
		}
		fclk = calc_fclk(channel, width, height, out_width,
				out_height);
	}

	DSSDBG("required fclk rate = %lu Hz\n", fclk);
	DSSDBG("current fclk rate = %lu Hz\n", dispc_fclk_rate());

	if (!fclk || fclk > dispc_fclk_rate()) {
		DSSERR("failed to set up scaling, "
			"required fclk rate = %lu Hz, "
			"current fclk rate = %lu Hz\n",
			fclk, dispc_fclk_rate());
		return -EINVAL;
	}

	return 0;
}

int dispc_ovl_setup(enum omap_plane plane, struct omap_overlay_info *oi,
		bool ilace, bool replication)
{
	struct omap_overlay *ovl = omap_dss_get_overlay(plane);
	bool five_taps = true;
	bool fieldmode = 0;
=======
	return pclk * vf * hf;
}

static unsigned long calc_core_clk_44xx(unsigned long pclk, u16 width,
		u16 height, u16 out_width, u16 out_height, bool mem_to_mem)
{
	/*
	 * If the overlay/writeback is in mem to mem mode, there are no
	 * downscaling limitations with respect to pixel clock, return 1 as
	 * required core clock to represent that we have sufficient enough
	 * core clock to do maximum downscaling
	 */
	if (mem_to_mem)
		return 1;

	if (width > out_width)
		return DIV_ROUND_UP(pclk, out_width) * width;
	else
		return pclk;
}

static int dispc_ovl_calc_scaling_24xx(unsigned long pclk, unsigned long lclk,
		const struct omap_video_timings *mgr_timings,
		u16 width, u16 height, u16 out_width, u16 out_height,
		enum omap_color_mode color_mode, bool *five_taps,
		int *x_predecim, int *y_predecim, int *decim_x, int *decim_y,
		u16 pos_x, unsigned long *core_clk, bool mem_to_mem)
{
	int error;
	u16 in_width, in_height;
	int min_factor = min(*decim_x, *decim_y);
	const int maxsinglelinewidth =
			dss_feat_get_param_max(FEAT_PARAM_LINEWIDTH);

	*five_taps = false;

	do {
		in_height = DIV_ROUND_UP(height, *decim_y);
		in_width = DIV_ROUND_UP(width, *decim_x);
		*core_clk = dispc.feat->calc_core_clk(pclk, in_width,
				in_height, out_width, out_height, mem_to_mem);
		error = (in_width > maxsinglelinewidth || !*core_clk ||
			*core_clk > dispc_core_clk_rate());
		if (error) {
			if (*decim_x == *decim_y) {
				*decim_x = min_factor;
				++*decim_y;
			} else {
				swap(*decim_x, *decim_y);
				if (*decim_x < *decim_y)
					++*decim_x;
			}
		}
	} while (*decim_x <= *x_predecim && *decim_y <= *y_predecim && error);

	if (in_width > maxsinglelinewidth) {
		DSSERR("Cannot scale max input width exceeded");
		return -EINVAL;
	}
	return 0;
}

static int dispc_ovl_calc_scaling_34xx(unsigned long pclk, unsigned long lclk,
		const struct omap_video_timings *mgr_timings,
		u16 width, u16 height, u16 out_width, u16 out_height,
		enum omap_color_mode color_mode, bool *five_taps,
		int *x_predecim, int *y_predecim, int *decim_x, int *decim_y,
		u16 pos_x, unsigned long *core_clk, bool mem_to_mem)
{
	int error;
	u16 in_width, in_height;
	int min_factor = min(*decim_x, *decim_y);
	const int maxsinglelinewidth =
			dss_feat_get_param_max(FEAT_PARAM_LINEWIDTH);

	do {
		in_height = DIV_ROUND_UP(height, *decim_y);
		in_width = DIV_ROUND_UP(width, *decim_x);
		*core_clk = calc_core_clk_five_taps(pclk, mgr_timings,
			in_width, in_height, out_width, out_height, color_mode);

		error = check_horiz_timing_omap3(pclk, lclk, mgr_timings,
				pos_x, in_width, in_height, out_width,
				out_height);

		if (in_width > maxsinglelinewidth)
			if (in_height > out_height &&
						in_height < out_height * 2)
				*five_taps = false;
		if (!*five_taps)
			*core_clk = dispc.feat->calc_core_clk(pclk, in_width,
					in_height, out_width, out_height,
					mem_to_mem);

		error = (error || in_width > maxsinglelinewidth * 2 ||
			(in_width > maxsinglelinewidth && *five_taps) ||
			!*core_clk || *core_clk > dispc_core_clk_rate());
		if (error) {
			if (*decim_x == *decim_y) {
				*decim_x = min_factor;
				++*decim_y;
			} else {
				swap(*decim_x, *decim_y);
				if (*decim_x < *decim_y)
					++*decim_x;
			}
		}
	} while (*decim_x <= *x_predecim && *decim_y <= *y_predecim && error);

	if (check_horiz_timing_omap3(pclk, lclk, mgr_timings, pos_x, width,
				height, out_width, out_height)){
			DSSERR("horizontal timing too tight\n");
			return -EINVAL;
	}

	if (in_width > (maxsinglelinewidth * 2)) {
		DSSERR("Cannot setup scaling");
		DSSERR("width exceeds maximum width possible");
		return -EINVAL;
	}

	if (in_width > maxsinglelinewidth && *five_taps) {
		DSSERR("cannot setup scaling with five taps");
		return -EINVAL;
	}
	return 0;
}

static int dispc_ovl_calc_scaling_44xx(unsigned long pclk, unsigned long lclk,
		const struct omap_video_timings *mgr_timings,
		u16 width, u16 height, u16 out_width, u16 out_height,
		enum omap_color_mode color_mode, bool *five_taps,
		int *x_predecim, int *y_predecim, int *decim_x, int *decim_y,
		u16 pos_x, unsigned long *core_clk, bool mem_to_mem)
{
	u16 in_width, in_width_max;
	int decim_x_min = *decim_x;
	u16 in_height = DIV_ROUND_UP(height, *decim_y);
	const int maxsinglelinewidth =
				dss_feat_get_param_max(FEAT_PARAM_LINEWIDTH);
	const int maxdownscale = dss_feat_get_param_max(FEAT_PARAM_DOWNSCALE);

	if (mem_to_mem) {
		in_width_max = out_width * maxdownscale;
	} else {
		in_width_max = dispc_core_clk_rate() /
					DIV_ROUND_UP(pclk, out_width);
	}

	*decim_x = DIV_ROUND_UP(width, in_width_max);

	*decim_x = *decim_x > decim_x_min ? *decim_x : decim_x_min;
	if (*decim_x > *x_predecim)
		return -EINVAL;

	do {
		in_width = DIV_ROUND_UP(width, *decim_x);
	} while (*decim_x <= *x_predecim &&
			in_width > maxsinglelinewidth && ++*decim_x);

	if (in_width > maxsinglelinewidth) {
		DSSERR("Cannot scale width exceeds max line width");
		return -EINVAL;
	}

	*core_clk = dispc.feat->calc_core_clk(pclk, in_width, in_height,
				out_width, out_height, mem_to_mem);
	return 0;
}

static int dispc_ovl_calc_scaling(unsigned long pclk, unsigned long lclk,
		enum omap_overlay_caps caps,
		const struct omap_video_timings *mgr_timings,
		u16 width, u16 height, u16 out_width, u16 out_height,
		enum omap_color_mode color_mode, bool *five_taps,
		int *x_predecim, int *y_predecim, u16 pos_x,
		enum omap_dss_rotation_type rotation_type, bool mem_to_mem)
{
	const int maxdownscale = dss_feat_get_param_max(FEAT_PARAM_DOWNSCALE);
	const int max_decim_limit = 16;
	unsigned long core_clk = 0;
	int decim_x, decim_y, ret;

	if (width == out_width && height == out_height)
		return 0;

	if ((caps & OMAP_DSS_OVL_CAP_SCALE) == 0)
		return -EINVAL;

	if (mem_to_mem) {
		*x_predecim = *y_predecim = 1;
	} else {
		*x_predecim = max_decim_limit;
		*y_predecim = (rotation_type == OMAP_DSS_ROT_TILER &&
				dss_has_feature(FEAT_BURST_2D)) ?
				2 : max_decim_limit;
	}

	if (color_mode == OMAP_DSS_COLOR_CLUT1 ||
	    color_mode == OMAP_DSS_COLOR_CLUT2 ||
	    color_mode == OMAP_DSS_COLOR_CLUT4 ||
	    color_mode == OMAP_DSS_COLOR_CLUT8) {
		*x_predecim = 1;
		*y_predecim = 1;
		*five_taps = false;
		return 0;
	}

	decim_x = DIV_ROUND_UP(DIV_ROUND_UP(width, out_width), maxdownscale);
	decim_y = DIV_ROUND_UP(DIV_ROUND_UP(height, out_height), maxdownscale);

	if (decim_x > *x_predecim || out_width > width * 8)
		return -EINVAL;

	if (decim_y > *y_predecim || out_height > height * 8)
		return -EINVAL;

	ret = dispc.feat->calc_scaling(pclk, lclk, mgr_timings, width, height,
		out_width, out_height, color_mode, five_taps,
		x_predecim, y_predecim, &decim_x, &decim_y, pos_x, &core_clk,
		mem_to_mem);
	if (ret)
		return ret;

	DSSDBG("required core clk rate = %lu Hz\n", core_clk);
	DSSDBG("current core clk rate = %lu Hz\n", dispc_core_clk_rate());

	if (!core_clk || core_clk > dispc_core_clk_rate()) {
		DSSERR("failed to set up scaling, "
			"required core clk rate = %lu Hz, "
			"current core clk rate = %lu Hz\n",
			core_clk, dispc_core_clk_rate());
		return -EINVAL;
	}

	*x_predecim = decim_x;
	*y_predecim = decim_y;
	return 0;
}

int dispc_ovl_check(enum omap_plane plane, enum omap_channel channel,
		const struct omap_overlay_info *oi,
		const struct omap_video_timings *timings,
		int *x_predecim, int *y_predecim)
{
	enum omap_overlay_caps caps = dss_feat_get_overlay_caps(plane);
	bool five_taps = true;
	bool fieldmode = false;
	u16 in_height = oi->height;
	u16 in_width = oi->width;
	bool ilace = timings->interlace;
	u16 out_width, out_height;
	int pos_x = oi->pos_x;
	unsigned long pclk = dispc_mgr_pclk_rate(channel);
	unsigned long lclk = dispc_mgr_lclk_rate(channel);

	out_width = oi->out_width == 0 ? oi->width : oi->out_width;
	out_height = oi->out_height == 0 ? oi->height : oi->out_height;

	if (ilace && oi->height == out_height)
		fieldmode = true;

	if (ilace) {
		if (fieldmode)
			in_height /= 2;
		out_height /= 2;

		DSSDBG("adjusting for ilace: height %d, out_height %d\n",
				in_height, out_height);
	}

	if (!dss_feat_color_mode_supported(plane, oi->color_mode))
		return -EINVAL;

	return dispc_ovl_calc_scaling(pclk, lclk, caps, timings, in_width,
			in_height, out_width, out_height, oi->color_mode,
			&five_taps, x_predecim, y_predecim, pos_x,
			oi->rotation_type, false);
}
EXPORT_SYMBOL(dispc_ovl_check);

static int dispc_ovl_setup_common(enum omap_plane plane,
		enum omap_overlay_caps caps, u32 paddr, u32 p_uv_addr,
		u16 screen_width, int pos_x, int pos_y, u16 width, u16 height,
		u16 out_width, u16 out_height, enum omap_color_mode color_mode,
		u8 rotation, bool mirror, u8 zorder, u8 pre_mult_alpha,
		u8 global_alpha, enum omap_dss_rotation_type rotation_type,
		bool replication, const struct omap_video_timings *mgr_timings,
		bool mem_to_mem)
{
	bool five_taps = true;
	bool fieldmode = false;
>>>>>>> refs/remotes/origin/master
	int r, cconv = 0;
	unsigned offset0, offset1;
	s32 row_inc;
	s32 pix_inc;
<<<<<<< HEAD
	u16 frame_height = oi->height;
	unsigned int field_offset = 0;
	u16 outw, outh;
	enum omap_channel channel;

	channel = dispc_ovl_get_channel_out(plane);

	DSSDBG("dispc_ovl_setup %d, pa %x, pa_uv %x, sw %d, %d,%d, %dx%d -> "
		"%dx%d, cmode %x, rot %d, mir %d, ilace %d chan %d repl %d\n",
		plane, oi->paddr, oi->p_uv_addr,
		oi->screen_width, oi->pos_x, oi->pos_y, oi->width, oi->height,
		oi->out_width, oi->out_height, oi->color_mode, oi->rotation,
		oi->mirror, ilace, channel, replication);

	if (oi->paddr == 0)
		return -EINVAL;

	outw = oi->out_width == 0 ? oi->width : oi->out_width;
	outh = oi->out_height == 0 ? oi->height : oi->out_height;

	if (ilace && oi->height == outh)
>>>>>>> refs/remotes/origin/cm-10.0
		fieldmode = 1;

	if (ilace) {
		if (fieldmode)
<<<<<<< HEAD
			height /= 2;
=======
	u16 frame_width, frame_height;
	unsigned int field_offset = 0;
	u16 in_height = height;
	u16 in_width = width;
	int x_predecim = 1, y_predecim = 1;
	bool ilace = mgr_timings->interlace;
	unsigned long pclk = dispc_plane_pclk_rate(plane);
	unsigned long lclk = dispc_plane_lclk_rate(plane);

	if (paddr == 0)
		return -EINVAL;

	out_width = out_width == 0 ? width : out_width;
	out_height = out_height == 0 ? height : out_height;

	if (ilace && height == out_height)
		fieldmode = true;

	if (ilace) {
		if (fieldmode)
			in_height /= 2;
>>>>>>> refs/remotes/origin/master
		pos_y /= 2;
		out_height /= 2;

		DSSDBG("adjusting for ilace: height %d, pos_y %d, "
<<<<<<< HEAD
				"out_height %d\n",
				height, pos_y, out_height);
=======
			"out_height %d\n", in_height, pos_y,
			out_height);
>>>>>>> refs/remotes/origin/master
	}

	if (!dss_feat_color_mode_supported(plane, color_mode))
		return -EINVAL;

<<<<<<< HEAD
	if (plane == OMAP_DSS_GFX) {
		if (width != out_width || height != out_height)
			return -EINVAL;
	} else {
		/* video plane */

		unsigned long fclk = 0;

		if (out_width < width / maxdownscale ||
		   out_width > width * 8)
			return -EINVAL;

		if (out_height < height / maxdownscale ||
		   out_height > height * 8)
			return -EINVAL;

		if (color_mode == OMAP_DSS_COLOR_YUV2 ||
			color_mode == OMAP_DSS_COLOR_UYVY ||
			color_mode == OMAP_DSS_COLOR_NV12)
			cconv = 1;

		/* Must use 5-tap filter? */
		five_taps = height > out_height * 2;

		if (!five_taps) {
			fclk = calc_fclk(channel, width, height, out_width,
					out_height);

			/* Try 5-tap filter if 3-tap fclk is too high */
			if (cpu_is_omap34xx() && height > out_height &&
					fclk > dispc_fclk_rate())
				five_taps = true;
		}

		if (width > (2048 >> five_taps)) {
			DSSERR("failed to set up scaling, fclk too low\n");
			return -EINVAL;
		}

		if (five_taps)
			fclk = calc_fclk_five_taps(channel, width, height,
					out_width, out_height, color_mode);

		DSSDBG("required fclk rate = %lu Hz\n", fclk);
		DSSDBG("current fclk rate = %lu Hz\n", dispc_fclk_rate());

		if (!fclk || fclk > dispc_fclk_rate()) {
			DSSERR("failed to set up scaling, "
					"required fclk rate = %lu Hz, "
					"current fclk rate = %lu Hz\n",
					fclk, dispc_fclk_rate());
			return -EINVAL;
		}
	}
=======
			oi->height /= 2;
		oi->pos_y /= 2;
		outh /= 2;

		DSSDBG("adjusting for ilace: height %d, pos_y %d, "
				"out_height %d\n",
				oi->height, oi->pos_y, outh);
	}

	if (!dss_feat_color_mode_supported(plane, oi->color_mode))
		return -EINVAL;

	r = dispc_ovl_calc_scaling(plane, channel, oi->width, oi->height,
			outw, outh, oi->color_mode,
			&five_taps);
	if (r)
		return r;

	if (oi->color_mode == OMAP_DSS_COLOR_YUV2 ||
			oi->color_mode == OMAP_DSS_COLOR_UYVY ||
			oi->color_mode == OMAP_DSS_COLOR_NV12)
		cconv = 1;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	r = dispc_ovl_calc_scaling(pclk, lclk, caps, mgr_timings, in_width,
			in_height, out_width, out_height, color_mode,
			&five_taps, &x_predecim, &y_predecim, pos_x,
			rotation_type, mem_to_mem);
	if (r)
		return r;

	in_width = DIV_ROUND_UP(in_width, x_predecim);
	in_height = DIV_ROUND_UP(in_height, y_predecim);

	if (color_mode == OMAP_DSS_COLOR_YUV2 ||
			color_mode == OMAP_DSS_COLOR_UYVY ||
			color_mode == OMAP_DSS_COLOR_NV12)
		cconv = 1;
>>>>>>> refs/remotes/origin/master

	if (ilace && !fieldmode) {
		/*
		 * when downscaling the bottom field may have to start several
		 * source lines below the top field. Unfortunately ACCUI
		 * registers will only hold the fractional part of the offset
		 * so the integer part must be added to the base address of the
		 * bottom field.
		 */
<<<<<<< HEAD
<<<<<<< HEAD
		if (!height || height == out_height)
			field_offset = 0;
		else
			field_offset = height / out_height / 2;
=======
		if (!oi->height || oi->height == outh)
			field_offset = 0;
		else
			field_offset = oi->height / outh / 2;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (!in_height || in_height == out_height)
			field_offset = 0;
		else
			field_offset = in_height / out_height / 2;
>>>>>>> refs/remotes/origin/master
	}

	/* Fields are independent but interleaved in memory. */
	if (fieldmode)
		field_offset = 1;

<<<<<<< HEAD
<<<<<<< HEAD
	if (rotation_type == OMAP_DSS_ROT_DMA)
		calc_dma_rotation_offset(rotation, mirror,
				screen_width, width, frame_height, color_mode,
				fieldmode, field_offset,
				&offset0, &offset1, &row_inc, &pix_inc);
	else
		calc_vrfb_rotation_offset(rotation, mirror,
				screen_width, width, frame_height, color_mode,
				fieldmode, field_offset,
=======
	if (oi->rotation_type == OMAP_DSS_ROT_DMA)
		calc_dma_rotation_offset(oi->rotation, oi->mirror,
				oi->screen_width, oi->width, frame_height,
				oi->color_mode, fieldmode, field_offset,
				&offset0, &offset1, &row_inc, &pix_inc);
	else
		calc_vrfb_rotation_offset(oi->rotation, oi->mirror,
				oi->screen_width, oi->width, frame_height,
				oi->color_mode, fieldmode, field_offset,
>>>>>>> refs/remotes/origin/cm-10.0
				&offset0, &offset1, &row_inc, &pix_inc);
=======
	offset0 = 0;
	offset1 = 0;
	row_inc = 0;
	pix_inc = 0;

	if (plane == OMAP_DSS_WB) {
		frame_width = out_width;
		frame_height = out_height;
	} else {
		frame_width = in_width;
		frame_height = height;
	}

	if (rotation_type == OMAP_DSS_ROT_TILER)
		calc_tiler_rotation_offset(screen_width, frame_width,
				color_mode, fieldmode, field_offset,
				&offset0, &offset1, &row_inc, &pix_inc,
				x_predecim, y_predecim);
	else if (rotation_type == OMAP_DSS_ROT_DMA)
		calc_dma_rotation_offset(rotation, mirror, screen_width,
				frame_width, frame_height,
				color_mode, fieldmode, field_offset,
				&offset0, &offset1, &row_inc, &pix_inc,
				x_predecim, y_predecim);
	else
		calc_vrfb_rotation_offset(rotation, mirror,
				screen_width, frame_width, frame_height,
				color_mode, fieldmode, field_offset,
				&offset0, &offset1, &row_inc, &pix_inc,
				x_predecim, y_predecim);
>>>>>>> refs/remotes/origin/master

	DSSDBG("offset0 %u, offset1 %u, row_inc %d, pix_inc %d\n",
			offset0, offset1, row_inc, pix_inc);

<<<<<<< HEAD
<<<<<<< HEAD
	_dispc_set_color_mode(plane, color_mode);

	_dispc_set_plane_ba0(plane, paddr + offset0);
	_dispc_set_plane_ba1(plane, paddr + offset1);

	if (OMAP_DSS_COLOR_NV12 == color_mode) {
		_dispc_set_plane_ba0_uv(plane, puv_addr + offset0);
		_dispc_set_plane_ba1_uv(plane, puv_addr + offset1);
	}


	_dispc_set_row_inc(plane, row_inc);
	_dispc_set_pix_inc(plane, pix_inc);

	DSSDBG("%d,%d %dx%d -> %dx%d\n", pos_x, pos_y, width, height,
			out_width, out_height);

	_dispc_set_plane_pos(plane, pos_x, pos_y);

	_dispc_set_pic_size(plane, width, height);

	if (plane != OMAP_DSS_GFX) {
		_dispc_set_scaling(plane, width, height,
				   out_width, out_height,
				   ilace, five_taps, fieldmode,
				   color_mode, rotation);
		_dispc_set_vid_size(plane, out_width, out_height);
		_dispc_set_vid_color_conv(plane, cconv);
	}

	_dispc_set_rotation_attrs(plane, rotation, mirror, color_mode);

	_dispc_set_pre_mult_alpha(plane, pre_mult_alpha);
	_dispc_setup_global_alpha(plane, global_alpha);
=======
	dispc_ovl_set_color_mode(plane, oi->color_mode);

	dispc_ovl_set_ba0(plane, oi->paddr + offset0);
	dispc_ovl_set_ba1(plane, oi->paddr + offset1);

	if (OMAP_DSS_COLOR_NV12 == oi->color_mode) {
		dispc_ovl_set_ba0_uv(plane, oi->p_uv_addr + offset0);
		dispc_ovl_set_ba1_uv(plane, oi->p_uv_addr + offset1);
	}


	dispc_ovl_set_row_inc(plane, row_inc);
	dispc_ovl_set_pix_inc(plane, pix_inc);

	DSSDBG("%d,%d %dx%d -> %dx%d\n", oi->pos_x, oi->pos_y, oi->width,
			oi->height, outw, outh);

	dispc_ovl_set_pos(plane, oi->pos_x, oi->pos_y);

	dispc_ovl_set_pic_size(plane, oi->width, oi->height);

	if (ovl->caps & OMAP_DSS_OVL_CAP_SCALE) {
		dispc_ovl_set_scaling(plane, oi->width, oi->height,
				   outw, outh,
				   ilace, five_taps, fieldmode,
				   oi->color_mode, oi->rotation);
		dispc_ovl_set_vid_size(plane, outw, outh);
		dispc_ovl_set_vid_color_conv(plane, cconv);
	}

	dispc_ovl_set_rotation_attrs(plane, oi->rotation, oi->mirror,
			oi->color_mode);

	dispc_ovl_set_zorder(plane, oi->zorder);
	dispc_ovl_set_pre_mult_alpha(plane, oi->pre_mult_alpha);
	dispc_ovl_setup_global_alpha(plane, oi->global_alpha);

	dispc_ovl_enable_replication(plane, replication);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	dispc_ovl_set_color_mode(plane, color_mode);

	dispc_ovl_configure_burst_type(plane, rotation_type);

	dispc_ovl_set_ba0(plane, paddr + offset0);
	dispc_ovl_set_ba1(plane, paddr + offset1);

	if (OMAP_DSS_COLOR_NV12 == color_mode) {
		dispc_ovl_set_ba0_uv(plane, p_uv_addr + offset0);
		dispc_ovl_set_ba1_uv(plane, p_uv_addr + offset1);
	}

	dispc_ovl_set_row_inc(plane, row_inc);
	dispc_ovl_set_pix_inc(plane, pix_inc);

	DSSDBG("%d,%d %dx%d -> %dx%d\n", pos_x, pos_y, in_width,
			in_height, out_width, out_height);

	dispc_ovl_set_pos(plane, caps, pos_x, pos_y);

	dispc_ovl_set_input_size(plane, in_width, in_height);

	if (caps & OMAP_DSS_OVL_CAP_SCALE) {
		dispc_ovl_set_scaling(plane, in_width, in_height, out_width,
				   out_height, ilace, five_taps, fieldmode,
				   color_mode, rotation);
		dispc_ovl_set_output_size(plane, out_width, out_height);
		dispc_ovl_set_vid_color_conv(plane, cconv);
	}

	dispc_ovl_set_rotation_attrs(plane, rotation, rotation_type, mirror,
			color_mode);

	dispc_ovl_set_zorder(plane, caps, zorder);
	dispc_ovl_set_pre_mult_alpha(plane, caps, pre_mult_alpha);
	dispc_ovl_setup_global_alpha(plane, caps, global_alpha);

	dispc_ovl_enable_replication(plane, caps, replication);
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void _dispc_enable_plane(enum omap_plane plane, bool enable)
{
	REG_FLD_MOD(DISPC_OVL_ATTRIBUTES(plane), enable ? 1 : 0, 0, 0);
=======
int dispc_ovl_enable(enum omap_plane plane, bool enable)
{
	DSSDBG("dispc_enable_plane %d, %d\n", plane, enable);

	REG_FLD_MOD(DISPC_OVL_ATTRIBUTES(plane), enable ? 1 : 0, 0, 0);

	return 0;
>>>>>>> refs/remotes/origin/cm-10.0
}

static void dispc_disable_isr(void *data, u32 mask)
{
	struct completion *compl = data;
	complete(compl);
}

static void _enable_lcd_out(enum omap_channel channel, bool enable)
{
<<<<<<< HEAD
	if (channel == OMAP_DSS_CHANNEL_LCD2)
		REG_FLD_MOD(DISPC_CONTROL2, enable ? 1 : 0, 0, 0);
	else
		REG_FLD_MOD(DISPC_CONTROL, enable ? 1 : 0, 0, 0);
}

static void dispc_enable_lcd_out(enum omap_channel channel, bool enable)
=======
	if (channel == OMAP_DSS_CHANNEL_LCD2) {
		REG_FLD_MOD(DISPC_CONTROL2, enable ? 1 : 0, 0, 0);
		/* flush posted write */
		dispc_read_reg(DISPC_CONTROL2);
	} else {
		REG_FLD_MOD(DISPC_CONTROL, enable ? 1 : 0, 0, 0);
		dispc_read_reg(DISPC_CONTROL);
	}
}

static void dispc_mgr_enable_lcd_out(enum omap_channel channel, bool enable)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct completion frame_done_completion;
	bool is_on;
	int r;
	u32 irq;

<<<<<<< HEAD
	enable_clocks(1);

=======
>>>>>>> refs/remotes/origin/cm-10.0
	/* When we disable LCD output, we need to wait until frame is done.
	 * Otherwise the DSS is still working, and turning off the clocks
	 * prevents DSS from going to OFF mode */
	is_on = channel == OMAP_DSS_CHANNEL_LCD2 ?
			REG_GET(DISPC_CONTROL2, 0, 0) :
			REG_GET(DISPC_CONTROL, 0, 0);

	irq = channel == OMAP_DSS_CHANNEL_LCD2 ? DISPC_IRQ_FRAMEDONE2 :
			DISPC_IRQ_FRAMEDONE;

	if (!enable && is_on) {
		init_completion(&frame_done_completion);

		r = omap_dispc_register_isr(dispc_disable_isr,
				&frame_done_completion, irq);

		if (r)
			DSSERR("failed to register FRAMEDONE isr\n");
	}

	_enable_lcd_out(channel, enable);

	if (!enable && is_on) {
		if (!wait_for_completion_timeout(&frame_done_completion,
					msecs_to_jiffies(100)))
			DSSERR("timeout waiting for FRAME DONE\n");

		r = omap_dispc_unregister_isr(dispc_disable_isr,
				&frame_done_completion, irq);

		if (r)
			DSSERR("failed to unregister FRAMEDONE isr\n");
	}
<<<<<<< HEAD

	enable_clocks(0);
=======
>>>>>>> refs/remotes/origin/cm-10.0
}

static void _enable_digit_out(bool enable)
{
	REG_FLD_MOD(DISPC_CONTROL, enable ? 1 : 0, 1, 1);
<<<<<<< HEAD
}

static void dispc_enable_digit_out(bool enable)
{
	struct completion frame_done_completion;
	int r;

	enable_clocks(1);

	if (REG_GET(DISPC_CONTROL, 1, 1) == enable) {
		enable_clocks(0);
		return;
	}
=======
	/* flush posted write */
	dispc_read_reg(DISPC_CONTROL);
}

static void dispc_mgr_enable_digit_out(bool enable)
{
	struct completion frame_done_completion;
	enum dss_hdmi_venc_clk_source_select src;
	int r, i;
	u32 irq_mask;
	int num_irqs;

	if (REG_GET(DISPC_CONTROL, 1, 1) == enable)
		return;

	src = dss_get_hdmi_venc_clk_source();
>>>>>>> refs/remotes/origin/cm-10.0

	if (enable) {
		unsigned long flags;
		/* When we enable digit output, we'll get an extra digit
		 * sync lost interrupt, that we need to ignore */
		spin_lock_irqsave(&dispc.irq_lock, flags);
		dispc.irq_error_mask &= ~DISPC_IRQ_SYNC_LOST_DIGIT;
		_omap_dispc_set_irqs();
		spin_unlock_irqrestore(&dispc.irq_lock, flags);
	}

	/* When we disable digit output, we need to wait until fields are done.
	 * Otherwise the DSS is still working, and turning off the clocks
	 * prevents DSS from going to OFF mode. And when enabling, we need to
	 * wait for the extra sync losts */
	init_completion(&frame_done_completion);

<<<<<<< HEAD
	r = omap_dispc_register_isr(dispc_disable_isr, &frame_done_completion,
			DISPC_IRQ_EVSYNC_EVEN | DISPC_IRQ_EVSYNC_ODD);
	if (r)
		DSSERR("failed to register EVSYNC isr\n");

	_enable_digit_out(enable);

	/* XXX I understand from TRM that we should only wait for the
	 * current field to complete. But it seems we have to wait
	 * for both fields */
	if (!wait_for_completion_timeout(&frame_done_completion,
				msecs_to_jiffies(100)))
		DSSERR("timeout waiting for EVSYNC\n");

	if (!wait_for_completion_timeout(&frame_done_completion,
				msecs_to_jiffies(100)))
		DSSERR("timeout waiting for EVSYNC\n");

	r = omap_dispc_unregister_isr(dispc_disable_isr,
			&frame_done_completion,
			DISPC_IRQ_EVSYNC_EVEN | DISPC_IRQ_EVSYNC_ODD);
	if (r)
		DSSERR("failed to unregister EVSYNC isr\n");
=======
	if (src == DSS_HDMI_M_PCLK && enable == false) {
		irq_mask = DISPC_IRQ_FRAMEDONETV;
		num_irqs = 1;
	} else {
		irq_mask = DISPC_IRQ_EVSYNC_EVEN | DISPC_IRQ_EVSYNC_ODD;
		/* XXX I understand from TRM that we should only wait for the
		 * current field to complete. But it seems we have to wait for
		 * both fields */
		num_irqs = 2;
	}

	r = omap_dispc_register_isr(dispc_disable_isr, &frame_done_completion,
			irq_mask);
	if (r)
		DSSERR("failed to register %x isr\n", irq_mask);

	_enable_digit_out(enable);

	for (i = 0; i < num_irqs; ++i) {
		if (!wait_for_completion_timeout(&frame_done_completion,
					msecs_to_jiffies(100)))
			DSSERR("timeout waiting for digit out to %s\n",
					enable ? "start" : "stop");
	}

	r = omap_dispc_unregister_isr(dispc_disable_isr, &frame_done_completion,
			irq_mask);
	if (r)
		DSSERR("failed to unregister %x isr\n", irq_mask);
>>>>>>> refs/remotes/origin/cm-10.0

	if (enable) {
		unsigned long flags;
		spin_lock_irqsave(&dispc.irq_lock, flags);
<<<<<<< HEAD
		dispc.irq_error_mask = DISPC_IRQ_MASK_ERROR;
		if (dss_has_feature(FEAT_MGR_LCD2))
			dispc.irq_error_mask |= DISPC_IRQ_SYNC_LOST2;
=======
		dispc.irq_error_mask |= DISPC_IRQ_SYNC_LOST_DIGIT;
>>>>>>> refs/remotes/origin/cm-10.0
		dispc_write_reg(DISPC_IRQSTATUS, DISPC_IRQ_SYNC_LOST_DIGIT);
		_omap_dispc_set_irqs();
		spin_unlock_irqrestore(&dispc.irq_lock, flags);
	}
<<<<<<< HEAD

	enable_clocks(0);
}

bool dispc_is_channel_enabled(enum omap_channel channel)
=======
}

bool dispc_mgr_is_enabled(enum omap_channel channel)
>>>>>>> refs/remotes/origin/cm-10.0
{
	if (channel == OMAP_DSS_CHANNEL_LCD)
		return !!REG_GET(DISPC_CONTROL, 0, 0);
	else if (channel == OMAP_DSS_CHANNEL_DIGIT)
		return !!REG_GET(DISPC_CONTROL, 1, 1);
	else if (channel == OMAP_DSS_CHANNEL_LCD2)
		return !!REG_GET(DISPC_CONTROL2, 0, 0);
	else
		BUG();
}

<<<<<<< HEAD
void dispc_enable_channel(enum omap_channel channel, bool enable)
{
	if (channel == OMAP_DSS_CHANNEL_LCD ||
			channel == OMAP_DSS_CHANNEL_LCD2)
		dispc_enable_lcd_out(channel, enable);
	else if (channel == OMAP_DSS_CHANNEL_DIGIT)
		dispc_enable_digit_out(enable);
=======
void dispc_mgr_enable(enum omap_channel channel, bool enable)
{
	if (dispc_mgr_is_lcd(channel))
		dispc_mgr_enable_lcd_out(channel, enable);
	else if (channel == OMAP_DSS_CHANNEL_DIGIT)
		dispc_mgr_enable_digit_out(enable);
>>>>>>> refs/remotes/origin/cm-10.0
	else
		BUG();
}

void dispc_lcd_enable_signal_polarity(bool act_high)
=======
int dispc_ovl_setup(enum omap_plane plane, const struct omap_overlay_info *oi,
		bool replication, const struct omap_video_timings *mgr_timings,
		bool mem_to_mem)
{
	int r;
	enum omap_overlay_caps caps = dss_feat_get_overlay_caps(plane);
	enum omap_channel channel;

	channel = dispc_ovl_get_channel_out(plane);

	DSSDBG("dispc_ovl_setup %d, pa %x, pa_uv %x, sw %d, %d,%d, %dx%d -> "
		"%dx%d, cmode %x, rot %d, mir %d, chan %d repl %d\n",
		plane, oi->paddr, oi->p_uv_addr, oi->screen_width, oi->pos_x,
		oi->pos_y, oi->width, oi->height, oi->out_width, oi->out_height,
		oi->color_mode, oi->rotation, oi->mirror, channel, replication);

	r = dispc_ovl_setup_common(plane, caps, oi->paddr, oi->p_uv_addr,
		oi->screen_width, oi->pos_x, oi->pos_y, oi->width, oi->height,
		oi->out_width, oi->out_height, oi->color_mode, oi->rotation,
		oi->mirror, oi->zorder, oi->pre_mult_alpha, oi->global_alpha,
		oi->rotation_type, replication, mgr_timings, mem_to_mem);

	return r;
}
EXPORT_SYMBOL(dispc_ovl_setup);

int dispc_wb_setup(const struct omap_dss_writeback_info *wi,
		bool mem_to_mem, const struct omap_video_timings *mgr_timings)
{
	int r;
	u32 l;
	enum omap_plane plane = OMAP_DSS_WB;
	const int pos_x = 0, pos_y = 0;
	const u8 zorder = 0, global_alpha = 0;
	const bool replication = false;
	bool truncation;
	int in_width = mgr_timings->x_res;
	int in_height = mgr_timings->y_res;
	enum omap_overlay_caps caps =
		OMAP_DSS_OVL_CAP_SCALE | OMAP_DSS_OVL_CAP_PRE_MULT_ALPHA;

	DSSDBG("dispc_wb_setup, pa %x, pa_uv %x, %d,%d -> %dx%d, cmode %x, "
		"rot %d, mir %d\n", wi->paddr, wi->p_uv_addr, in_width,
		in_height, wi->width, wi->height, wi->color_mode, wi->rotation,
		wi->mirror);

	r = dispc_ovl_setup_common(plane, caps, wi->paddr, wi->p_uv_addr,
		wi->buf_width, pos_x, pos_y, in_width, in_height, wi->width,
		wi->height, wi->color_mode, wi->rotation, wi->mirror, zorder,
		wi->pre_mult_alpha, global_alpha, wi->rotation_type,
		replication, mgr_timings, mem_to_mem);

	switch (wi->color_mode) {
	case OMAP_DSS_COLOR_RGB16:
	case OMAP_DSS_COLOR_RGB24P:
	case OMAP_DSS_COLOR_ARGB16:
	case OMAP_DSS_COLOR_RGBA16:
	case OMAP_DSS_COLOR_RGB12U:
	case OMAP_DSS_COLOR_ARGB16_1555:
	case OMAP_DSS_COLOR_XRGB16_1555:
	case OMAP_DSS_COLOR_RGBX16:
		truncation = true;
		break;
	default:
		truncation = false;
		break;
	}

	/* setup extra DISPC_WB_ATTRIBUTES */
	l = dispc_read_reg(DISPC_OVL_ATTRIBUTES(plane));
	l = FLD_MOD(l, truncation, 10, 10);	/* TRUNCATIONENABLE */
	l = FLD_MOD(l, mem_to_mem, 19, 19);	/* WRITEBACKMODE */
	dispc_write_reg(DISPC_OVL_ATTRIBUTES(plane), l);

	return r;
}

int dispc_ovl_enable(enum omap_plane plane, bool enable)
{
	DSSDBG("dispc_enable_plane %d, %d\n", plane, enable);

	REG_FLD_MOD(DISPC_OVL_ATTRIBUTES(plane), enable ? 1 : 0, 0, 0);

	return 0;
}
EXPORT_SYMBOL(dispc_ovl_enable);

bool dispc_ovl_enabled(enum omap_plane plane)
{
	return REG_GET(DISPC_OVL_ATTRIBUTES(plane), 0, 0);
}
EXPORT_SYMBOL(dispc_ovl_enabled);

void dispc_mgr_enable(enum omap_channel channel, bool enable)
{
	mgr_fld_write(channel, DISPC_MGR_FLD_ENABLE, enable);
	/* flush posted write */
	mgr_fld_read(channel, DISPC_MGR_FLD_ENABLE);
}
EXPORT_SYMBOL(dispc_mgr_enable);

bool dispc_mgr_is_enabled(enum omap_channel channel)
{
	return !!mgr_fld_read(channel, DISPC_MGR_FLD_ENABLE);
}
EXPORT_SYMBOL(dispc_mgr_is_enabled);

void dispc_wb_enable(bool enable)
{
	dispc_ovl_enable(OMAP_DSS_WB, enable);
}

bool dispc_wb_is_enabled(void)
{
	return dispc_ovl_enabled(OMAP_DSS_WB);
}

static void dispc_lcd_enable_signal_polarity(bool act_high)
>>>>>>> refs/remotes/origin/master
{
	if (!dss_has_feature(FEAT_LCDENABLEPOL))
		return;

<<<<<<< HEAD
<<<<<<< HEAD
	enable_clocks(1);
	REG_FLD_MOD(DISPC_CONTROL, act_high ? 1 : 0, 29, 29);
	enable_clocks(0);
=======
	REG_FLD_MOD(DISPC_CONTROL, act_high ? 1 : 0, 29, 29);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	REG_FLD_MOD(DISPC_CONTROL, act_high ? 1 : 0, 29, 29);
>>>>>>> refs/remotes/origin/master
}

void dispc_lcd_enable_signal(bool enable)
{
	if (!dss_has_feature(FEAT_LCDENABLESIGNAL))
		return;

<<<<<<< HEAD
<<<<<<< HEAD
	enable_clocks(1);
	REG_FLD_MOD(DISPC_CONTROL, enable ? 1 : 0, 28, 28);
	enable_clocks(0);
=======
	REG_FLD_MOD(DISPC_CONTROL, enable ? 1 : 0, 28, 28);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	REG_FLD_MOD(DISPC_CONTROL, enable ? 1 : 0, 28, 28);
>>>>>>> refs/remotes/origin/master
}

void dispc_pck_free_enable(bool enable)
{
	if (!dss_has_feature(FEAT_PCKFREEENABLE))
		return;

<<<<<<< HEAD
<<<<<<< HEAD
	enable_clocks(1);
	REG_FLD_MOD(DISPC_CONTROL, enable ? 1 : 0, 27, 27);
	enable_clocks(0);
}

void dispc_enable_fifohandcheck(enum omap_channel channel, bool enable)
{
	enable_clocks(1);
=======
	REG_FLD_MOD(DISPC_CONTROL, enable ? 1 : 0, 27, 27);
}

void dispc_mgr_enable_fifohandcheck(enum omap_channel channel, bool enable)
{
>>>>>>> refs/remotes/origin/cm-10.0
	if (channel == OMAP_DSS_CHANNEL_LCD2)
		REG_FLD_MOD(DISPC_CONFIG2, enable ? 1 : 0, 16, 16);
	else
		REG_FLD_MOD(DISPC_CONFIG, enable ? 1 : 0, 16, 16);
<<<<<<< HEAD
	enable_clocks(0);
}


void dispc_set_lcd_display_type(enum omap_channel channel,
=======
}


void dispc_mgr_set_lcd_display_type(enum omap_channel channel,
>>>>>>> refs/remotes/origin/cm-10.0
		enum omap_lcd_display_type type)
{
	int mode;

	switch (type) {
	case OMAP_DSS_LCD_DISPLAY_STN:
		mode = 0;
		break;

	case OMAP_DSS_LCD_DISPLAY_TFT:
		mode = 1;
		break;

	default:
		BUG();
		return;
	}

<<<<<<< HEAD
	enable_clocks(1);
=======
>>>>>>> refs/remotes/origin/cm-10.0
	if (channel == OMAP_DSS_CHANNEL_LCD2)
		REG_FLD_MOD(DISPC_CONTROL2, mode, 3, 3);
	else
		REG_FLD_MOD(DISPC_CONTROL, mode, 3, 3);
<<<<<<< HEAD
	enable_clocks(0);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	REG_FLD_MOD(DISPC_CONTROL, enable ? 1 : 0, 27, 27);
}

static void dispc_mgr_enable_fifohandcheck(enum omap_channel channel, bool enable)
{
	mgr_fld_write(channel, DISPC_MGR_FLD_FIFOHANDCHECK, enable);
}


static void dispc_mgr_set_lcd_type_tft(enum omap_channel channel)
{
	mgr_fld_write(channel, DISPC_MGR_FLD_STNTFT, 1);
>>>>>>> refs/remotes/origin/master
}

void dispc_set_loadmode(enum omap_dss_load_mode mode)
{
<<<<<<< HEAD
<<<<<<< HEAD
	enable_clocks(1);
	REG_FLD_MOD(DISPC_CONFIG, mode, 2, 1);
	enable_clocks(0);
}


void dispc_set_default_color(enum omap_channel channel, u32 color)
{
	enable_clocks(1);
	dispc_write_reg(DISPC_DEFAULT_COLOR(channel), color);
	enable_clocks(0);
}

u32 dispc_get_default_color(enum omap_channel channel)
{
	u32 l;

	BUG_ON(channel != OMAP_DSS_CHANNEL_DIGIT &&
		channel != OMAP_DSS_CHANNEL_LCD &&
		channel != OMAP_DSS_CHANNEL_LCD2);

	enable_clocks(1);
	l = dispc_read_reg(DISPC_DEFAULT_COLOR(channel));
	enable_clocks(0);

	return l;
}

void dispc_set_trans_key(enum omap_channel ch,
		enum omap_dss_trans_key_type type,
		u32 trans_key)
{
	enable_clocks(1);
=======
=======
>>>>>>> refs/remotes/origin/master
	REG_FLD_MOD(DISPC_CONFIG, mode, 2, 1);
}


static void dispc_mgr_set_default_color(enum omap_channel channel, u32 color)
{
	dispc_write_reg(DISPC_DEFAULT_COLOR(channel), color);
}

static void dispc_mgr_set_trans_key(enum omap_channel ch,
		enum omap_dss_trans_key_type type,
		u32 trans_key)
{
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	if (ch == OMAP_DSS_CHANNEL_LCD)
		REG_FLD_MOD(DISPC_CONFIG, type, 11, 11);
	else if (ch == OMAP_DSS_CHANNEL_DIGIT)
		REG_FLD_MOD(DISPC_CONFIG, type, 13, 13);
	else /* OMAP_DSS_CHANNEL_LCD2 */
		REG_FLD_MOD(DISPC_CONFIG2, type, 11, 11);

	dispc_write_reg(DISPC_TRANS_COLOR(ch), trans_key);
<<<<<<< HEAD
	enable_clocks(0);
}

void dispc_get_trans_key(enum omap_channel ch,
		enum omap_dss_trans_key_type *type,
		u32 *trans_key)
{
	enable_clocks(1);
	if (type) {
		if (ch == OMAP_DSS_CHANNEL_LCD)
			*type = REG_GET(DISPC_CONFIG, 11, 11);
		else if (ch == OMAP_DSS_CHANNEL_DIGIT)
			*type = REG_GET(DISPC_CONFIG, 13, 13);
		else if (ch == OMAP_DSS_CHANNEL_LCD2)
			*type = REG_GET(DISPC_CONFIG2, 11, 11);
		else
			BUG();
	}

	if (trans_key)
		*trans_key = dispc_read_reg(DISPC_TRANS_COLOR(ch));
	enable_clocks(0);
}

void dispc_enable_trans_key(enum omap_channel ch, bool enable)
{
	enable_clocks(1);
=======
=======
	mgr_fld_write(ch, DISPC_MGR_FLD_TCKSELECTION, type);

	dispc_write_reg(DISPC_TRANS_COLOR(ch), trans_key);
>>>>>>> refs/remotes/origin/master
}

static void dispc_mgr_enable_trans_key(enum omap_channel ch, bool enable)
{
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	if (ch == OMAP_DSS_CHANNEL_LCD)
		REG_FLD_MOD(DISPC_CONFIG, enable, 10, 10);
	else if (ch == OMAP_DSS_CHANNEL_DIGIT)
		REG_FLD_MOD(DISPC_CONFIG, enable, 12, 12);
	else /* OMAP_DSS_CHANNEL_LCD2 */
		REG_FLD_MOD(DISPC_CONFIG2, enable, 10, 10);
<<<<<<< HEAD
	enable_clocks(0);
}
void dispc_enable_alpha_blending(enum omap_channel ch, bool enable)
{
	if (!dss_has_feature(FEAT_GLOBAL_ALPHA))
		return;

	enable_clocks(1);
=======
=======
	mgr_fld_write(ch, DISPC_MGR_FLD_TCKENABLE, enable);
>>>>>>> refs/remotes/origin/master
}

static void dispc_mgr_enable_alpha_fixed_zorder(enum omap_channel ch,
		bool enable)
{
	if (!dss_has_feature(FEAT_ALPHA_FIXED_ZORDER))
		return;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (ch == OMAP_DSS_CHANNEL_LCD)
		REG_FLD_MOD(DISPC_CONFIG, enable, 18, 18);
	else if (ch == OMAP_DSS_CHANNEL_DIGIT)
		REG_FLD_MOD(DISPC_CONFIG, enable, 19, 19);
<<<<<<< HEAD
<<<<<<< HEAD
	else /* OMAP_DSS_CHANNEL_LCD2 */
		REG_FLD_MOD(DISPC_CONFIG2, enable, 18, 18);
	enable_clocks(0);
}
bool dispc_alpha_blending_enabled(enum omap_channel ch)
{
	bool enabled;

	if (!dss_has_feature(FEAT_GLOBAL_ALPHA))
		return false;

	enable_clocks(1);
	if (ch == OMAP_DSS_CHANNEL_LCD)
		enabled = REG_GET(DISPC_CONFIG, 18, 18);
	else if (ch == OMAP_DSS_CHANNEL_DIGIT)
		enabled = REG_GET(DISPC_CONFIG, 19, 19);
	else if (ch == OMAP_DSS_CHANNEL_LCD2)
		enabled = REG_GET(DISPC_CONFIG2, 18, 18);
	else
		BUG();
	enable_clocks(0);

	return enabled;
}


bool dispc_trans_key_enabled(enum omap_channel ch)
{
	bool enabled;

	enable_clocks(1);
	if (ch == OMAP_DSS_CHANNEL_LCD)
		enabled = REG_GET(DISPC_CONFIG, 10, 10);
	else if (ch == OMAP_DSS_CHANNEL_DIGIT)
		enabled = REG_GET(DISPC_CONFIG, 12, 12);
	else if (ch == OMAP_DSS_CHANNEL_LCD2)
		enabled = REG_GET(DISPC_CONFIG2, 10, 10);
	else
		BUG();
	enable_clocks(0);

	return enabled;
}


void dispc_set_tft_data_lines(enum omap_channel channel, u8 data_lines)
=======
}

void dispc_mgr_setup(enum omap_channel channel,
		struct omap_overlay_manager_info *info)
=======
}

void dispc_mgr_setup(enum omap_channel channel,
		const struct omap_overlay_manager_info *info)
>>>>>>> refs/remotes/origin/master
{
	dispc_mgr_set_default_color(channel, info->default_color);
	dispc_mgr_set_trans_key(channel, info->trans_key_type, info->trans_key);
	dispc_mgr_enable_trans_key(channel, info->trans_enabled);
	dispc_mgr_enable_alpha_fixed_zorder(channel,
			info->partial_alpha_enabled);
	if (dss_has_feature(FEAT_CPR)) {
		dispc_mgr_enable_cpr(channel, info->cpr_enable);
		dispc_mgr_set_cpr_coef(channel, &info->cpr_coefs);
	}
}
<<<<<<< HEAD

void dispc_mgr_set_tft_data_lines(enum omap_channel channel, u8 data_lines)
>>>>>>> refs/remotes/origin/cm-10.0
=======
EXPORT_SYMBOL(dispc_mgr_setup);

static void dispc_mgr_set_tft_data_lines(enum omap_channel channel, u8 data_lines)
>>>>>>> refs/remotes/origin/master
{
	int code;

	switch (data_lines) {
	case 12:
		code = 0;
		break;
	case 16:
		code = 1;
		break;
	case 18:
		code = 2;
		break;
	case 24:
		code = 3;
		break;
	default:
		BUG();
		return;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	enable_clocks(1);
=======
>>>>>>> refs/remotes/origin/cm-10.0
	if (channel == OMAP_DSS_CHANNEL_LCD2)
		REG_FLD_MOD(DISPC_CONTROL2, code, 9, 8);
	else
		REG_FLD_MOD(DISPC_CONTROL, code, 9, 8);
<<<<<<< HEAD
	enable_clocks(0);
}

void dispc_set_parallel_interface_mode(enum omap_channel channel,
		enum omap_parallel_interface_mode mode)
{
	u32 l;
	int stallmode;
	int gpout0 = 1;
	int gpout1;

	switch (mode) {
	case OMAP_DSS_PARALLELMODE_BYPASS:
		stallmode = 0;
		gpout1 = 1;
		break;

	case OMAP_DSS_PARALLELMODE_RFBI:
		stallmode = 1;
		gpout1 = 0;
		break;

	case OMAP_DSS_PARALLELMODE_DSI:
		stallmode = 1;
		gpout1 = 1;
		break;

=======
}

void dispc_mgr_set_io_pad_mode(enum dss_io_pad_mode mode)
=======
	mgr_fld_write(channel, DISPC_MGR_FLD_TFTDATALINES, code);
}

static void dispc_mgr_set_io_pad_mode(enum dss_io_pad_mode mode)
>>>>>>> refs/remotes/origin/master
{
	u32 l;
	int gpout0, gpout1;

	switch (mode) {
	case DSS_IO_PAD_MODE_RESET:
		gpout0 = 0;
		gpout1 = 0;
		break;
	case DSS_IO_PAD_MODE_RFBI:
		gpout0 = 1;
		gpout1 = 0;
		break;
	case DSS_IO_PAD_MODE_BYPASS:
		gpout0 = 1;
		gpout1 = 1;
		break;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	default:
		BUG();
		return;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	enable_clocks(1);

	if (channel == OMAP_DSS_CHANNEL_LCD2) {
		l = dispc_read_reg(DISPC_CONTROL2);
		l = FLD_MOD(l, stallmode, 11, 11);
		dispc_write_reg(DISPC_CONTROL2, l);
	} else {
		l = dispc_read_reg(DISPC_CONTROL);
		l = FLD_MOD(l, stallmode, 11, 11);
		l = FLD_MOD(l, gpout0, 15, 15);
		l = FLD_MOD(l, gpout1, 16, 16);
		dispc_write_reg(DISPC_CONTROL, l);
	}

	enable_clocks(0);
=======
=======
>>>>>>> refs/remotes/origin/master
	l = dispc_read_reg(DISPC_CONTROL);
	l = FLD_MOD(l, gpout0, 15, 15);
	l = FLD_MOD(l, gpout1, 16, 16);
	dispc_write_reg(DISPC_CONTROL, l);
}

<<<<<<< HEAD
void dispc_mgr_enable_stallmode(enum omap_channel channel, bool enable)
{
	if (channel == OMAP_DSS_CHANNEL_LCD2)
		REG_FLD_MOD(DISPC_CONTROL2, enable, 11, 11);
	else
		REG_FLD_MOD(DISPC_CONTROL, enable, 11, 11);
>>>>>>> refs/remotes/origin/cm-10.0
}

static bool _dispc_lcd_timings_ok(int hsw, int hfp, int hbp,
		int vsw, int vfp, int vbp)
{
	if (cpu_is_omap24xx() || omap_rev() < OMAP3430_REV_ES3_0) {
		if (hsw < 1 || hsw > 64 ||
				hfp < 1 || hfp > 256 ||
				hbp < 1 || hbp > 256 ||
				vsw < 1 || vsw > 64 ||
				vfp < 0 || vfp > 255 ||
				vbp < 0 || vbp > 255)
			return false;
	} else {
		if (hsw < 1 || hsw > 256 ||
				hfp < 1 || hfp > 4096 ||
				hbp < 1 || hbp > 4096 ||
				vsw < 1 || vsw > 256 ||
				vfp < 0 || vfp > 4095 ||
				vbp < 0 || vbp > 4095)
			return false;
	}

	return true;
}

bool dispc_lcd_timings_ok(struct omap_video_timings *timings)
{
	return _dispc_lcd_timings_ok(timings->hsw, timings->hfp,
			timings->hbp, timings->vsw,
			timings->vfp, timings->vbp);
}

<<<<<<< HEAD
static void _dispc_set_lcd_timings(enum omap_channel channel, int hsw,
=======
static void _dispc_mgr_set_lcd_timings(enum omap_channel channel, int hsw,
>>>>>>> refs/remotes/origin/cm-10.0
		int hfp, int hbp, int vsw, int vfp, int vbp)
{
	u32 timing_h, timing_v;

	if (cpu_is_omap24xx() || omap_rev() < OMAP3430_REV_ES3_0) {
		timing_h = FLD_VAL(hsw-1, 5, 0) | FLD_VAL(hfp-1, 15, 8) |
			FLD_VAL(hbp-1, 27, 20);

		timing_v = FLD_VAL(vsw-1, 5, 0) | FLD_VAL(vfp, 15, 8) |
			FLD_VAL(vbp, 27, 20);
	} else {
		timing_h = FLD_VAL(hsw-1, 7, 0) | FLD_VAL(hfp-1, 19, 8) |
			FLD_VAL(hbp-1, 31, 20);

		timing_v = FLD_VAL(vsw-1, 7, 0) | FLD_VAL(vfp, 19, 8) |
			FLD_VAL(vbp, 31, 20);
	}

<<<<<<< HEAD
	enable_clocks(1);
	dispc_write_reg(DISPC_TIMING_H(channel), timing_h);
	dispc_write_reg(DISPC_TIMING_V(channel), timing_v);
	enable_clocks(0);
}

/* change name to mode? */
void dispc_set_lcd_timings(enum omap_channel channel,
=======
	dispc_write_reg(DISPC_TIMING_H(channel), timing_h);
	dispc_write_reg(DISPC_TIMING_V(channel), timing_v);
}

/* change name to mode? */
void dispc_mgr_set_lcd_timings(enum omap_channel channel,
>>>>>>> refs/remotes/origin/cm-10.0
		struct omap_video_timings *timings)
{
	unsigned xtot, ytot;
	unsigned long ht, vt;

	if (!_dispc_lcd_timings_ok(timings->hsw, timings->hfp,
				timings->hbp, timings->vsw,
				timings->vfp, timings->vbp))
		BUG();

<<<<<<< HEAD
	_dispc_set_lcd_timings(channel, timings->hsw, timings->hfp,
			timings->hbp, timings->vsw, timings->vfp,
			timings->vbp);

	dispc_set_lcd_size(channel, timings->x_res, timings->y_res);
=======
	_dispc_mgr_set_lcd_timings(channel, timings->hsw, timings->hfp,
			timings->hbp, timings->vsw, timings->vfp,
			timings->vbp);

	dispc_mgr_set_lcd_size(channel, timings->x_res, timings->y_res);
>>>>>>> refs/remotes/origin/cm-10.0

	xtot = timings->x_res + timings->hfp + timings->hsw + timings->hbp;
	ytot = timings->y_res + timings->vfp + timings->vsw + timings->vbp;

	ht = (timings->pixel_clock * 1000) / xtot;
	vt = (timings->pixel_clock * 1000) / xtot / ytot;

	DSSDBG("channel %d xres %u yres %u\n", channel, timings->x_res,
			timings->y_res);
	DSSDBG("pck %u\n", timings->pixel_clock);
	DSSDBG("hsw %d hfp %d hbp %d vsw %d vfp %d vbp %d\n",
			timings->hsw, timings->hfp, timings->hbp,
			timings->vsw, timings->vfp, timings->vbp);

	DSSDBG("hsync %luHz, vsync %luHz\n", ht, vt);
}

<<<<<<< HEAD
static void dispc_set_lcd_divisor(enum omap_channel channel, u16 lck_div,
		u16 pck_div)
{
	BUG_ON(lck_div < 1);
	BUG_ON(pck_div < 2);

	enable_clocks(1);
	dispc_write_reg(DISPC_DIVISORo(channel),
			FLD_VAL(lck_div, 23, 16) | FLD_VAL(pck_div, 7, 0));
	enable_clocks(0);
}

static void dispc_get_lcd_divisor(enum omap_channel channel, int *lck_div,
=======
=======
static void dispc_mgr_enable_stallmode(enum omap_channel channel, bool enable)
{
	mgr_fld_write(channel, DISPC_MGR_FLD_STALLMODE, enable);
}

void dispc_mgr_set_lcd_config(enum omap_channel channel,
		const struct dss_lcd_mgr_config *config)
{
	dispc_mgr_set_io_pad_mode(config->io_pad_mode);

	dispc_mgr_enable_stallmode(channel, config->stallmode);
	dispc_mgr_enable_fifohandcheck(channel, config->fifohandcheck);

	dispc_mgr_set_clock_div(channel, &config->clock_info);

	dispc_mgr_set_tft_data_lines(channel, config->video_port_width);

	dispc_lcd_enable_signal_polarity(config->lcden_sig_polarity);

	dispc_mgr_set_lcd_type_tft(channel);
}
EXPORT_SYMBOL(dispc_mgr_set_lcd_config);

static bool _dispc_mgr_size_ok(u16 width, u16 height)
{
	return width <= dispc.feat->mgr_width_max &&
		height <= dispc.feat->mgr_height_max;
}

static bool _dispc_lcd_timings_ok(int hsw, int hfp, int hbp,
		int vsw, int vfp, int vbp)
{
	if (hsw < 1 || hsw > dispc.feat->sw_max ||
			hfp < 1 || hfp > dispc.feat->hp_max ||
			hbp < 1 || hbp > dispc.feat->hp_max ||
			vsw < 1 || vsw > dispc.feat->sw_max ||
			vfp < 0 || vfp > dispc.feat->vp_max ||
			vbp < 0 || vbp > dispc.feat->vp_max)
		return false;
	return true;
}

static bool _dispc_mgr_pclk_ok(enum omap_channel channel,
		unsigned long pclk)
{
	if (dss_mgr_is_lcd(channel))
		return pclk <= dispc.feat->max_lcd_pclk ? true : false;
	else
		return pclk <= dispc.feat->max_tv_pclk ? true : false;
}

bool dispc_mgr_timings_ok(enum omap_channel channel,
		const struct omap_video_timings *timings)
{
	bool timings_ok;

	timings_ok = _dispc_mgr_size_ok(timings->x_res, timings->y_res);

	timings_ok &= _dispc_mgr_pclk_ok(channel, timings->pixel_clock * 1000);

	if (dss_mgr_is_lcd(channel)) {
		timings_ok &= _dispc_lcd_timings_ok(timings->hsw, timings->hfp,
				timings->hbp, timings->vsw, timings->vfp,
				timings->vbp);
	}

	return timings_ok;
}

static void _dispc_mgr_set_lcd_timings(enum omap_channel channel, int hsw,
		int hfp, int hbp, int vsw, int vfp, int vbp,
		enum omap_dss_signal_level vsync_level,
		enum omap_dss_signal_level hsync_level,
		enum omap_dss_signal_edge data_pclk_edge,
		enum omap_dss_signal_level de_level,
		enum omap_dss_signal_edge sync_pclk_edge)

{
	u32 timing_h, timing_v, l;
	bool onoff, rf, ipc;

	timing_h = FLD_VAL(hsw-1, dispc.feat->sw_start, 0) |
			FLD_VAL(hfp-1, dispc.feat->fp_start, 8) |
			FLD_VAL(hbp-1, dispc.feat->bp_start, 20);
	timing_v = FLD_VAL(vsw-1, dispc.feat->sw_start, 0) |
			FLD_VAL(vfp, dispc.feat->fp_start, 8) |
			FLD_VAL(vbp, dispc.feat->bp_start, 20);

	dispc_write_reg(DISPC_TIMING_H(channel), timing_h);
	dispc_write_reg(DISPC_TIMING_V(channel), timing_v);

	switch (data_pclk_edge) {
	case OMAPDSS_DRIVE_SIG_RISING_EDGE:
		ipc = false;
		break;
	case OMAPDSS_DRIVE_SIG_FALLING_EDGE:
		ipc = true;
		break;
	case OMAPDSS_DRIVE_SIG_OPPOSITE_EDGES:
	default:
		BUG();
	}

	switch (sync_pclk_edge) {
	case OMAPDSS_DRIVE_SIG_OPPOSITE_EDGES:
		onoff = false;
		rf = false;
		break;
	case OMAPDSS_DRIVE_SIG_FALLING_EDGE:
		onoff = true;
		rf = false;
		break;
	case OMAPDSS_DRIVE_SIG_RISING_EDGE:
		onoff = true;
		rf = true;
		break;
	default:
		BUG();
	}

	l = dispc_read_reg(DISPC_POL_FREQ(channel));
	l |= FLD_VAL(onoff, 17, 17);
	l |= FLD_VAL(rf, 16, 16);
	l |= FLD_VAL(de_level, 15, 15);
	l |= FLD_VAL(ipc, 14, 14);
	l |= FLD_VAL(hsync_level, 13, 13);
	l |= FLD_VAL(vsync_level, 12, 12);
	dispc_write_reg(DISPC_POL_FREQ(channel), l);
}

/* change name to mode? */
void dispc_mgr_set_timings(enum omap_channel channel,
		const struct omap_video_timings *timings)
{
	unsigned xtot, ytot;
	unsigned long ht, vt;
	struct omap_video_timings t = *timings;

	DSSDBG("channel %d xres %u yres %u\n", channel, t.x_res, t.y_res);

	if (!dispc_mgr_timings_ok(channel, &t)) {
		BUG();
		return;
	}

	if (dss_mgr_is_lcd(channel)) {
		_dispc_mgr_set_lcd_timings(channel, t.hsw, t.hfp, t.hbp, t.vsw,
				t.vfp, t.vbp, t.vsync_level, t.hsync_level,
				t.data_pclk_edge, t.de_level, t.sync_pclk_edge);

		xtot = t.x_res + t.hfp + t.hsw + t.hbp;
		ytot = t.y_res + t.vfp + t.vsw + t.vbp;

		ht = (timings->pixel_clock * 1000) / xtot;
		vt = (timings->pixel_clock * 1000) / xtot / ytot;

		DSSDBG("pck %u\n", timings->pixel_clock);
		DSSDBG("hsw %d hfp %d hbp %d vsw %d vfp %d vbp %d\n",
			t.hsw, t.hfp, t.hbp, t.vsw, t.vfp, t.vbp);
		DSSDBG("vsync_level %d hsync_level %d data_pclk_edge %d de_level %d sync_pclk_edge %d\n",
			t.vsync_level, t.hsync_level, t.data_pclk_edge,
			t.de_level, t.sync_pclk_edge);

		DSSDBG("hsync %luHz, vsync %luHz\n", ht, vt);
	} else {
		if (t.interlace == true)
			t.y_res /= 2;
	}

	dispc_mgr_set_size(channel, t.x_res, t.y_res);
}
EXPORT_SYMBOL(dispc_mgr_set_timings);

>>>>>>> refs/remotes/origin/master
static void dispc_mgr_set_lcd_divisor(enum omap_channel channel, u16 lck_div,
		u16 pck_div)
{
	BUG_ON(lck_div < 1);
	BUG_ON(pck_div < 1);

	dispc_write_reg(DISPC_DIVISORo(channel),
			FLD_VAL(lck_div, 23, 16) | FLD_VAL(pck_div, 7, 0));
<<<<<<< HEAD
}

static void dispc_mgr_get_lcd_divisor(enum omap_channel channel, int *lck_div,
>>>>>>> refs/remotes/origin/cm-10.0
=======

	if (dss_has_feature(FEAT_CORE_CLK_DIV) == false &&
			channel == OMAP_DSS_CHANNEL_LCD)
		dispc.core_clk_rate = dispc_fclk_rate() / lck_div;
}

static void dispc_mgr_get_lcd_divisor(enum omap_channel channel, int *lck_div,
>>>>>>> refs/remotes/origin/master
		int *pck_div)
{
	u32 l;
	l = dispc_read_reg(DISPC_DIVISORo(channel));
	*lck_div = FLD_GET(l, 23, 16);
	*pck_div = FLD_GET(l, 7, 0);
}

unsigned long dispc_fclk_rate(void)
{
	struct platform_device *dsidev;
	unsigned long r = 0;

	switch (dss_get_dispc_clk_source()) {
	case OMAP_DSS_CLK_SRC_FCK:
<<<<<<< HEAD
<<<<<<< HEAD
		r = dss_clk_get_rate(DSS_CLK_FCK);
=======
		r = clk_get_rate(dispc.dss_clk);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		r = dss_get_dispc_clk_rate();
>>>>>>> refs/remotes/origin/master
		break;
	case OMAP_DSS_CLK_SRC_DSI_PLL_HSDIV_DISPC:
		dsidev = dsi_get_dsidev_from_id(0);
		r = dsi_get_pll_hsdiv_dispc_rate(dsidev);
		break;
	case OMAP_DSS_CLK_SRC_DSI2_PLL_HSDIV_DISPC:
		dsidev = dsi_get_dsidev_from_id(1);
		r = dsi_get_pll_hsdiv_dispc_rate(dsidev);
		break;
	default:
		BUG();
<<<<<<< HEAD
=======
		return 0;
>>>>>>> refs/remotes/origin/master
	}

	return r;
}

<<<<<<< HEAD
<<<<<<< HEAD
unsigned long dispc_lclk_rate(enum omap_channel channel)
=======
unsigned long dispc_mgr_lclk_rate(enum omap_channel channel)
>>>>>>> refs/remotes/origin/cm-10.0
=======
unsigned long dispc_mgr_lclk_rate(enum omap_channel channel)
>>>>>>> refs/remotes/origin/master
{
	struct platform_device *dsidev;
	int lcd;
	unsigned long r;
	u32 l;

<<<<<<< HEAD
	l = dispc_read_reg(DISPC_DIVISORo(channel));

	lcd = FLD_GET(l, 23, 16);

	switch (dss_get_lcd_clk_source(channel)) {
	case OMAP_DSS_CLK_SRC_FCK:
<<<<<<< HEAD
		r = dss_clk_get_rate(DSS_CLK_FCK);
=======
		r = clk_get_rate(dispc.dss_clk);
>>>>>>> refs/remotes/origin/cm-10.0
		break;
	case OMAP_DSS_CLK_SRC_DSI_PLL_HSDIV_DISPC:
		dsidev = dsi_get_dsidev_from_id(0);
		r = dsi_get_pll_hsdiv_dispc_rate(dsidev);
		break;
	case OMAP_DSS_CLK_SRC_DSI2_PLL_HSDIV_DISPC:
		dsidev = dsi_get_dsidev_from_id(1);
		r = dsi_get_pll_hsdiv_dispc_rate(dsidev);
		break;
	default:
		BUG();
	}

	return r / lcd;
}

<<<<<<< HEAD
unsigned long dispc_pclk_rate(enum omap_channel channel)
{
	int pcd;
	unsigned long r;
	u32 l;

	l = dispc_read_reg(DISPC_DIVISORo(channel));

	pcd = FLD_GET(l, 7, 0);

	r = dispc_lclk_rate(channel);

	return r / pcd;
=======
=======
	if (dss_mgr_is_lcd(channel)) {
		l = dispc_read_reg(DISPC_DIVISORo(channel));

		lcd = FLD_GET(l, 23, 16);

		switch (dss_get_lcd_clk_source(channel)) {
		case OMAP_DSS_CLK_SRC_FCK:
			r = dss_get_dispc_clk_rate();
			break;
		case OMAP_DSS_CLK_SRC_DSI_PLL_HSDIV_DISPC:
			dsidev = dsi_get_dsidev_from_id(0);
			r = dsi_get_pll_hsdiv_dispc_rate(dsidev);
			break;
		case OMAP_DSS_CLK_SRC_DSI2_PLL_HSDIV_DISPC:
			dsidev = dsi_get_dsidev_from_id(1);
			r = dsi_get_pll_hsdiv_dispc_rate(dsidev);
			break;
		default:
			BUG();
			return 0;
		}

		return r / lcd;
	} else {
		return dispc_fclk_rate();
	}
}

>>>>>>> refs/remotes/origin/master
unsigned long dispc_mgr_pclk_rate(enum omap_channel channel)
{
	unsigned long r;

<<<<<<< HEAD
	if (dispc_mgr_is_lcd(channel)) {
=======
	if (dss_mgr_is_lcd(channel)) {
>>>>>>> refs/remotes/origin/master
		int pcd;
		u32 l;

		l = dispc_read_reg(DISPC_DIVISORo(channel));

		pcd = FLD_GET(l, 7, 0);

		r = dispc_mgr_lclk_rate(channel);

		return r / pcd;
	} else {
<<<<<<< HEAD
		struct omap_dss_device *dssdev =
			dispc_mgr_get_device(channel);

		switch (dssdev->type) {
		case OMAP_DISPLAY_TYPE_VENC:
			return venc_get_pixel_clock();
		case OMAP_DISPLAY_TYPE_HDMI:
			return hdmi_get_pixel_clock();
		default:
			BUG();
		}
	}
>>>>>>> refs/remotes/origin/cm-10.0
}

void dispc_dump_clocks(struct seq_file *s)
{
	int lcd, pcd;
	u32 l;
	enum omap_dss_clk_source dispc_clk_src = dss_get_dispc_clk_source();
	enum omap_dss_clk_source lcd_clk_src;

<<<<<<< HEAD
	enable_clocks(1);
=======
	if (dispc_runtime_get())
		return;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		return dispc.tv_pclk_rate;
	}
}

void dispc_set_tv_pclk(unsigned long pclk)
{
	dispc.tv_pclk_rate = pclk;
}

unsigned long dispc_core_clk_rate(void)
{
	return dispc.core_clk_rate;
}

static unsigned long dispc_plane_pclk_rate(enum omap_plane plane)
{
	enum omap_channel channel;

	if (plane == OMAP_DSS_WB)
		return 0;

	channel = dispc_ovl_get_channel_out(plane);

	return dispc_mgr_pclk_rate(channel);
}

static unsigned long dispc_plane_lclk_rate(enum omap_plane plane)
{
	enum omap_channel channel;

	if (plane == OMAP_DSS_WB)
		return 0;

	channel	= dispc_ovl_get_channel_out(plane);

	return dispc_mgr_lclk_rate(channel);
}

static void dispc_dump_clocks_channel(struct seq_file *s, enum omap_channel channel)
{
	int lcd, pcd;
	enum omap_dss_clk_source lcd_clk_src;

	seq_printf(s, "- %s -\n", mgr_desc[channel].name);

	lcd_clk_src = dss_get_lcd_clk_source(channel);

	seq_printf(s, "%s clk source = %s (%s)\n", mgr_desc[channel].name,
		dss_get_generic_clk_source_name(lcd_clk_src),
		dss_feat_get_clk_source_name(lcd_clk_src));

	dispc_mgr_get_lcd_divisor(channel, &lcd, &pcd);

	seq_printf(s, "lck\t\t%-16lulck div\t%u\n",
		dispc_mgr_lclk_rate(channel), lcd);
	seq_printf(s, "pck\t\t%-16lupck div\t%u\n",
		dispc_mgr_pclk_rate(channel), pcd);
}

void dispc_dump_clocks(struct seq_file *s)
{
	int lcd;
	u32 l;
	enum omap_dss_clk_source dispc_clk_src = dss_get_dispc_clk_source();

	if (dispc_runtime_get())
		return;
>>>>>>> refs/remotes/origin/master

	seq_printf(s, "- DISPC -\n");

	seq_printf(s, "dispc fclk source = %s (%s)\n",
			dss_get_generic_clk_source_name(dispc_clk_src),
			dss_feat_get_clk_source_name(dispc_clk_src));

	seq_printf(s, "fck\t\t%-16lu\n", dispc_fclk_rate());

	if (dss_has_feature(FEAT_CORE_CLK_DIV)) {
		seq_printf(s, "- DISPC-CORE-CLK -\n");
		l = dispc_read_reg(DISPC_DIVISOR);
		lcd = FLD_GET(l, 23, 16);

		seq_printf(s, "lck\t\t%-16lulck div\t%u\n",
				(dispc_fclk_rate()/lcd), lcd);
	}
<<<<<<< HEAD
	seq_printf(s, "- LCD1 -\n");

	lcd_clk_src = dss_get_lcd_clk_source(OMAP_DSS_CHANNEL_LCD);

	seq_printf(s, "lcd1_clk source = %s (%s)\n",
		dss_get_generic_clk_source_name(lcd_clk_src),
		dss_feat_get_clk_source_name(lcd_clk_src));

<<<<<<< HEAD
	dispc_get_lcd_divisor(OMAP_DSS_CHANNEL_LCD, &lcd, &pcd);

	seq_printf(s, "lck\t\t%-16lulck div\t%u\n",
			dispc_lclk_rate(OMAP_DSS_CHANNEL_LCD), lcd);
	seq_printf(s, "pck\t\t%-16lupck div\t%u\n",
			dispc_pclk_rate(OMAP_DSS_CHANNEL_LCD), pcd);
=======
	dispc_mgr_get_lcd_divisor(OMAP_DSS_CHANNEL_LCD, &lcd, &pcd);

	seq_printf(s, "lck\t\t%-16lulck div\t%u\n",
			dispc_mgr_lclk_rate(OMAP_DSS_CHANNEL_LCD), lcd);
	seq_printf(s, "pck\t\t%-16lupck div\t%u\n",
			dispc_mgr_pclk_rate(OMAP_DSS_CHANNEL_LCD), pcd);
>>>>>>> refs/remotes/origin/cm-10.0
	if (dss_has_feature(FEAT_MGR_LCD2)) {
		seq_printf(s, "- LCD2 -\n");

		lcd_clk_src = dss_get_lcd_clk_source(OMAP_DSS_CHANNEL_LCD2);

		seq_printf(s, "lcd2_clk source = %s (%s)\n",
			dss_get_generic_clk_source_name(lcd_clk_src),
			dss_feat_get_clk_source_name(lcd_clk_src));

<<<<<<< HEAD
		dispc_get_lcd_divisor(OMAP_DSS_CHANNEL_LCD2, &lcd, &pcd);

		seq_printf(s, "lck\t\t%-16lulck div\t%u\n",
				dispc_lclk_rate(OMAP_DSS_CHANNEL_LCD2), lcd);
		seq_printf(s, "pck\t\t%-16lupck div\t%u\n",
				dispc_pclk_rate(OMAP_DSS_CHANNEL_LCD2), pcd);
	}
	enable_clocks(0);
=======
		dispc_mgr_get_lcd_divisor(OMAP_DSS_CHANNEL_LCD2, &lcd, &pcd);

		seq_printf(s, "lck\t\t%-16lulck div\t%u\n",
				dispc_mgr_lclk_rate(OMAP_DSS_CHANNEL_LCD2), lcd);
		seq_printf(s, "pck\t\t%-16lupck div\t%u\n",
				dispc_mgr_pclk_rate(OMAP_DSS_CHANNEL_LCD2), pcd);
	}

	dispc_runtime_put();
>>>>>>> refs/remotes/origin/cm-10.0
}

#ifdef CONFIG_OMAP2_DSS_COLLECT_IRQ_STATS
void dispc_dump_irqs(struct seq_file *s)
{
	unsigned long flags;
	struct dispc_irq_stats stats;

	spin_lock_irqsave(&dispc.irq_stats_lock, flags);

	stats = dispc.irq_stats;
	memset(&dispc.irq_stats, 0, sizeof(dispc.irq_stats));
	dispc.irq_stats.last_reset = jiffies;

	spin_unlock_irqrestore(&dispc.irq_stats_lock, flags);

	seq_printf(s, "period %u ms\n",
			jiffies_to_msecs(jiffies - stats.last_reset));

	seq_printf(s, "irqs %d\n", stats.irq_count);
#define PIS(x) \
	seq_printf(s, "%-20s %10d\n", #x, stats.irqs[ffs(DISPC_IRQ_##x)-1]);

	PIS(FRAMEDONE);
	PIS(VSYNC);
	PIS(EVSYNC_EVEN);
	PIS(EVSYNC_ODD);
	PIS(ACBIAS_COUNT_STAT);
	PIS(PROG_LINE_NUM);
	PIS(GFX_FIFO_UNDERFLOW);
	PIS(GFX_END_WIN);
	PIS(PAL_GAMMA_MASK);
	PIS(OCP_ERR);
	PIS(VID1_FIFO_UNDERFLOW);
	PIS(VID1_END_WIN);
	PIS(VID2_FIFO_UNDERFLOW);
	PIS(VID2_END_WIN);
<<<<<<< HEAD
=======
	if (dss_feat_get_num_ovls() > 3) {
		PIS(VID3_FIFO_UNDERFLOW);
		PIS(VID3_END_WIN);
	}
>>>>>>> refs/remotes/origin/cm-10.0
	PIS(SYNC_LOST);
	PIS(SYNC_LOST_DIGIT);
	PIS(WAKEUP);
	if (dss_has_feature(FEAT_MGR_LCD2)) {
		PIS(FRAMEDONE2);
		PIS(VSYNC2);
		PIS(ACBIAS_COUNT_STAT2);
		PIS(SYNC_LOST2);
	}
#undef PIS
}
#endif

void dispc_dump_regs(struct seq_file *s)
{
<<<<<<< HEAD
#define DUMPREG(r) seq_printf(s, "%-50s %08x\n", #r, dispc_read_reg(r))

	dss_clk_enable(DSS_CLK_ICK | DSS_CLK_FCK);

=======
=======

	dispc_dump_clocks_channel(s, OMAP_DSS_CHANNEL_LCD);

	if (dss_has_feature(FEAT_MGR_LCD2))
		dispc_dump_clocks_channel(s, OMAP_DSS_CHANNEL_LCD2);
	if (dss_has_feature(FEAT_MGR_LCD3))
		dispc_dump_clocks_channel(s, OMAP_DSS_CHANNEL_LCD3);

	dispc_runtime_put();
}

static void dispc_dump_regs(struct seq_file *s)
{
>>>>>>> refs/remotes/origin/master
	int i, j;
	const char *mgr_names[] = {
		[OMAP_DSS_CHANNEL_LCD]		= "LCD",
		[OMAP_DSS_CHANNEL_DIGIT]	= "TV",
		[OMAP_DSS_CHANNEL_LCD2]		= "LCD2",
<<<<<<< HEAD
=======
		[OMAP_DSS_CHANNEL_LCD3]		= "LCD3",
>>>>>>> refs/remotes/origin/master
	};
	const char *ovl_names[] = {
		[OMAP_DSS_GFX]		= "GFX",
		[OMAP_DSS_VIDEO1]	= "VID1",
		[OMAP_DSS_VIDEO2]	= "VID2",
		[OMAP_DSS_VIDEO3]	= "VID3",
	};
	const char **p_names;

#define DUMPREG(r) seq_printf(s, "%-50s %08x\n", #r, dispc_read_reg(r))

	if (dispc_runtime_get())
		return;

	/* DISPC common registers */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	DUMPREG(DISPC_REVISION);
	DUMPREG(DISPC_SYSCONFIG);
	DUMPREG(DISPC_SYSSTATUS);
	DUMPREG(DISPC_IRQSTATUS);
	DUMPREG(DISPC_IRQENABLE);
	DUMPREG(DISPC_CONTROL);
	DUMPREG(DISPC_CONFIG);
	DUMPREG(DISPC_CAPABLE);
<<<<<<< HEAD
<<<<<<< HEAD
	DUMPREG(DISPC_DEFAULT_COLOR(OMAP_DSS_CHANNEL_LCD));
	DUMPREG(DISPC_DEFAULT_COLOR(OMAP_DSS_CHANNEL_DIGIT));
	DUMPREG(DISPC_TRANS_COLOR(OMAP_DSS_CHANNEL_LCD));
	DUMPREG(DISPC_TRANS_COLOR(OMAP_DSS_CHANNEL_DIGIT));
	DUMPREG(DISPC_LINE_STATUS);
	DUMPREG(DISPC_LINE_NUMBER);
	DUMPREG(DISPC_TIMING_H(OMAP_DSS_CHANNEL_LCD));
	DUMPREG(DISPC_TIMING_V(OMAP_DSS_CHANNEL_LCD));
	DUMPREG(DISPC_POL_FREQ(OMAP_DSS_CHANNEL_LCD));
	DUMPREG(DISPC_DIVISORo(OMAP_DSS_CHANNEL_LCD));
	DUMPREG(DISPC_GLOBAL_ALPHA);
	DUMPREG(DISPC_SIZE_MGR(OMAP_DSS_CHANNEL_DIGIT));
	DUMPREG(DISPC_SIZE_MGR(OMAP_DSS_CHANNEL_LCD));
	if (dss_has_feature(FEAT_MGR_LCD2)) {
		DUMPREG(DISPC_CONTROL2);
		DUMPREG(DISPC_CONFIG2);
		DUMPREG(DISPC_DEFAULT_COLOR(OMAP_DSS_CHANNEL_LCD2));
		DUMPREG(DISPC_TRANS_COLOR(OMAP_DSS_CHANNEL_LCD2));
		DUMPREG(DISPC_TIMING_H(OMAP_DSS_CHANNEL_LCD2));
		DUMPREG(DISPC_TIMING_V(OMAP_DSS_CHANNEL_LCD2));
		DUMPREG(DISPC_POL_FREQ(OMAP_DSS_CHANNEL_LCD2));
		DUMPREG(DISPC_DIVISORo(OMAP_DSS_CHANNEL_LCD2));
		DUMPREG(DISPC_SIZE_MGR(OMAP_DSS_CHANNEL_LCD2));
	}

	DUMPREG(DISPC_OVL_BA0(OMAP_DSS_GFX));
	DUMPREG(DISPC_OVL_BA1(OMAP_DSS_GFX));
	DUMPREG(DISPC_OVL_POSITION(OMAP_DSS_GFX));
	DUMPREG(DISPC_OVL_SIZE(OMAP_DSS_GFX));
	DUMPREG(DISPC_OVL_ATTRIBUTES(OMAP_DSS_GFX));
	DUMPREG(DISPC_OVL_FIFO_THRESHOLD(OMAP_DSS_GFX));
	DUMPREG(DISPC_OVL_FIFO_SIZE_STATUS(OMAP_DSS_GFX));
	DUMPREG(DISPC_OVL_ROW_INC(OMAP_DSS_GFX));
	DUMPREG(DISPC_OVL_PIXEL_INC(OMAP_DSS_GFX));
	DUMPREG(DISPC_OVL_WINDOW_SKIP(OMAP_DSS_GFX));
	DUMPREG(DISPC_OVL_TABLE_BA(OMAP_DSS_GFX));

	DUMPREG(DISPC_DATA_CYCLE1(OMAP_DSS_CHANNEL_LCD));
	DUMPREG(DISPC_DATA_CYCLE2(OMAP_DSS_CHANNEL_LCD));
	DUMPREG(DISPC_DATA_CYCLE3(OMAP_DSS_CHANNEL_LCD));

	DUMPREG(DISPC_CPR_COEF_R(OMAP_DSS_CHANNEL_LCD));
	DUMPREG(DISPC_CPR_COEF_G(OMAP_DSS_CHANNEL_LCD));
	DUMPREG(DISPC_CPR_COEF_B(OMAP_DSS_CHANNEL_LCD));
	if (dss_has_feature(FEAT_MGR_LCD2)) {
		DUMPREG(DISPC_DATA_CYCLE1(OMAP_DSS_CHANNEL_LCD2));
		DUMPREG(DISPC_DATA_CYCLE2(OMAP_DSS_CHANNEL_LCD2));
		DUMPREG(DISPC_DATA_CYCLE3(OMAP_DSS_CHANNEL_LCD2));

		DUMPREG(DISPC_CPR_COEF_R(OMAP_DSS_CHANNEL_LCD2));
		DUMPREG(DISPC_CPR_COEF_G(OMAP_DSS_CHANNEL_LCD2));
		DUMPREG(DISPC_CPR_COEF_B(OMAP_DSS_CHANNEL_LCD2));
	}

	DUMPREG(DISPC_OVL_PRELOAD(OMAP_DSS_GFX));

	DUMPREG(DISPC_OVL_BA0(OMAP_DSS_VIDEO1));
	DUMPREG(DISPC_OVL_BA1(OMAP_DSS_VIDEO1));
	DUMPREG(DISPC_OVL_POSITION(OMAP_DSS_VIDEO1));
	DUMPREG(DISPC_OVL_SIZE(OMAP_DSS_VIDEO1));
	DUMPREG(DISPC_OVL_ATTRIBUTES(OMAP_DSS_VIDEO1));
	DUMPREG(DISPC_OVL_FIFO_THRESHOLD(OMAP_DSS_VIDEO1));
	DUMPREG(DISPC_OVL_FIFO_SIZE_STATUS(OMAP_DSS_VIDEO1));
	DUMPREG(DISPC_OVL_ROW_INC(OMAP_DSS_VIDEO1));
	DUMPREG(DISPC_OVL_PIXEL_INC(OMAP_DSS_VIDEO1));
	DUMPREG(DISPC_OVL_FIR(OMAP_DSS_VIDEO1));
	DUMPREG(DISPC_OVL_PICTURE_SIZE(OMAP_DSS_VIDEO1));
	DUMPREG(DISPC_OVL_ACCU0(OMAP_DSS_VIDEO1));
	DUMPREG(DISPC_OVL_ACCU1(OMAP_DSS_VIDEO1));

	DUMPREG(DISPC_OVL_BA0(OMAP_DSS_VIDEO2));
	DUMPREG(DISPC_OVL_BA1(OMAP_DSS_VIDEO2));
	DUMPREG(DISPC_OVL_POSITION(OMAP_DSS_VIDEO2));
	DUMPREG(DISPC_OVL_SIZE(OMAP_DSS_VIDEO2));
	DUMPREG(DISPC_OVL_ATTRIBUTES(OMAP_DSS_VIDEO2));
	DUMPREG(DISPC_OVL_FIFO_THRESHOLD(OMAP_DSS_VIDEO2));
	DUMPREG(DISPC_OVL_FIFO_SIZE_STATUS(OMAP_DSS_VIDEO2));
	DUMPREG(DISPC_OVL_ROW_INC(OMAP_DSS_VIDEO2));
	DUMPREG(DISPC_OVL_PIXEL_INC(OMAP_DSS_VIDEO2));
	DUMPREG(DISPC_OVL_FIR(OMAP_DSS_VIDEO2));
	DUMPREG(DISPC_OVL_PICTURE_SIZE(OMAP_DSS_VIDEO2));
	DUMPREG(DISPC_OVL_ACCU0(OMAP_DSS_VIDEO2));
	DUMPREG(DISPC_OVL_ACCU1(OMAP_DSS_VIDEO2));

	DUMPREG(DISPC_OVL_FIR_COEF_H(OMAP_DSS_VIDEO1, 0));
	DUMPREG(DISPC_OVL_FIR_COEF_H(OMAP_DSS_VIDEO1, 1));
	DUMPREG(DISPC_OVL_FIR_COEF_H(OMAP_DSS_VIDEO1, 2));
	DUMPREG(DISPC_OVL_FIR_COEF_H(OMAP_DSS_VIDEO1, 3));
	DUMPREG(DISPC_OVL_FIR_COEF_H(OMAP_DSS_VIDEO1, 4));
	DUMPREG(DISPC_OVL_FIR_COEF_H(OMAP_DSS_VIDEO1, 5));
	DUMPREG(DISPC_OVL_FIR_COEF_H(OMAP_DSS_VIDEO1, 6));
	DUMPREG(DISPC_OVL_FIR_COEF_H(OMAP_DSS_VIDEO1, 7));
	DUMPREG(DISPC_OVL_FIR_COEF_HV(OMAP_DSS_VIDEO1, 0));
	DUMPREG(DISPC_OVL_FIR_COEF_HV(OMAP_DSS_VIDEO1, 1));
	DUMPREG(DISPC_OVL_FIR_COEF_HV(OMAP_DSS_VIDEO1, 2));
	DUMPREG(DISPC_OVL_FIR_COEF_HV(OMAP_DSS_VIDEO1, 3));
	DUMPREG(DISPC_OVL_FIR_COEF_HV(OMAP_DSS_VIDEO1, 4));
	DUMPREG(DISPC_OVL_FIR_COEF_HV(OMAP_DSS_VIDEO1, 5));
	DUMPREG(DISPC_OVL_FIR_COEF_HV(OMAP_DSS_VIDEO1, 6));
	DUMPREG(DISPC_OVL_FIR_COEF_HV(OMAP_DSS_VIDEO1, 7));
	DUMPREG(DISPC_OVL_CONV_COEF(OMAP_DSS_VIDEO1, 0));
	DUMPREG(DISPC_OVL_CONV_COEF(OMAP_DSS_VIDEO1, 1));
	DUMPREG(DISPC_OVL_CONV_COEF(OMAP_DSS_VIDEO1, 2));
	DUMPREG(DISPC_OVL_CONV_COEF(OMAP_DSS_VIDEO1, 3));
	DUMPREG(DISPC_OVL_CONV_COEF(OMAP_DSS_VIDEO1, 4));
	DUMPREG(DISPC_OVL_FIR_COEF_V(OMAP_DSS_VIDEO1, 0));
	DUMPREG(DISPC_OVL_FIR_COEF_V(OMAP_DSS_VIDEO1, 1));
	DUMPREG(DISPC_OVL_FIR_COEF_V(OMAP_DSS_VIDEO1, 2));
	DUMPREG(DISPC_OVL_FIR_COEF_V(OMAP_DSS_VIDEO1, 3));
	DUMPREG(DISPC_OVL_FIR_COEF_V(OMAP_DSS_VIDEO1, 4));
	DUMPREG(DISPC_OVL_FIR_COEF_V(OMAP_DSS_VIDEO1, 5));
	DUMPREG(DISPC_OVL_FIR_COEF_V(OMAP_DSS_VIDEO1, 6));
	DUMPREG(DISPC_OVL_FIR_COEF_V(OMAP_DSS_VIDEO1, 7));

	if (dss_has_feature(FEAT_HANDLE_UV_SEPARATE)) {
		DUMPREG(DISPC_OVL_BA0_UV(OMAP_DSS_VIDEO1));
		DUMPREG(DISPC_OVL_BA1_UV(OMAP_DSS_VIDEO1));
		DUMPREG(DISPC_OVL_FIR2(OMAP_DSS_VIDEO1));
		DUMPREG(DISPC_OVL_ACCU2_0(OMAP_DSS_VIDEO1));
		DUMPREG(DISPC_OVL_ACCU2_1(OMAP_DSS_VIDEO1));

		DUMPREG(DISPC_OVL_FIR_COEF_H2(OMAP_DSS_VIDEO1, 0));
		DUMPREG(DISPC_OVL_FIR_COEF_H2(OMAP_DSS_VIDEO1, 1));
		DUMPREG(DISPC_OVL_FIR_COEF_H2(OMAP_DSS_VIDEO1, 2));
		DUMPREG(DISPC_OVL_FIR_COEF_H2(OMAP_DSS_VIDEO1, 3));
		DUMPREG(DISPC_OVL_FIR_COEF_H2(OMAP_DSS_VIDEO1, 4));
		DUMPREG(DISPC_OVL_FIR_COEF_H2(OMAP_DSS_VIDEO1, 5));
		DUMPREG(DISPC_OVL_FIR_COEF_H2(OMAP_DSS_VIDEO1, 6));
		DUMPREG(DISPC_OVL_FIR_COEF_H2(OMAP_DSS_VIDEO1, 7));

		DUMPREG(DISPC_OVL_FIR_COEF_HV2(OMAP_DSS_VIDEO1, 0));
		DUMPREG(DISPC_OVL_FIR_COEF_HV2(OMAP_DSS_VIDEO1, 1));
		DUMPREG(DISPC_OVL_FIR_COEF_HV2(OMAP_DSS_VIDEO1, 2));
		DUMPREG(DISPC_OVL_FIR_COEF_HV2(OMAP_DSS_VIDEO1, 3));
		DUMPREG(DISPC_OVL_FIR_COEF_HV2(OMAP_DSS_VIDEO1, 4));
		DUMPREG(DISPC_OVL_FIR_COEF_HV2(OMAP_DSS_VIDEO1, 5));
		DUMPREG(DISPC_OVL_FIR_COEF_HV2(OMAP_DSS_VIDEO1, 6));
		DUMPREG(DISPC_OVL_FIR_COEF_HV2(OMAP_DSS_VIDEO1, 7));

		DUMPREG(DISPC_OVL_FIR_COEF_V2(OMAP_DSS_VIDEO1, 0));
		DUMPREG(DISPC_OVL_FIR_COEF_V2(OMAP_DSS_VIDEO1, 1));
		DUMPREG(DISPC_OVL_FIR_COEF_V2(OMAP_DSS_VIDEO1, 2));
		DUMPREG(DISPC_OVL_FIR_COEF_V2(OMAP_DSS_VIDEO1, 3));
		DUMPREG(DISPC_OVL_FIR_COEF_V2(OMAP_DSS_VIDEO1, 4));
		DUMPREG(DISPC_OVL_FIR_COEF_V2(OMAP_DSS_VIDEO1, 5));
		DUMPREG(DISPC_OVL_FIR_COEF_V2(OMAP_DSS_VIDEO1, 6));
		DUMPREG(DISPC_OVL_FIR_COEF_V2(OMAP_DSS_VIDEO1, 7));
	}
	if (dss_has_feature(FEAT_ATTR2))
		DUMPREG(DISPC_OVL_ATTRIBUTES2(OMAP_DSS_VIDEO1));


	DUMPREG(DISPC_OVL_FIR_COEF_H(OMAP_DSS_VIDEO2, 0));
	DUMPREG(DISPC_OVL_FIR_COEF_H(OMAP_DSS_VIDEO2, 1));
	DUMPREG(DISPC_OVL_FIR_COEF_H(OMAP_DSS_VIDEO2, 2));
	DUMPREG(DISPC_OVL_FIR_COEF_H(OMAP_DSS_VIDEO2, 3));
	DUMPREG(DISPC_OVL_FIR_COEF_H(OMAP_DSS_VIDEO2, 4));
	DUMPREG(DISPC_OVL_FIR_COEF_H(OMAP_DSS_VIDEO2, 5));
	DUMPREG(DISPC_OVL_FIR_COEF_H(OMAP_DSS_VIDEO2, 6));
	DUMPREG(DISPC_OVL_FIR_COEF_H(OMAP_DSS_VIDEO2, 7));
	DUMPREG(DISPC_OVL_FIR_COEF_HV(OMAP_DSS_VIDEO2, 0));
	DUMPREG(DISPC_OVL_FIR_COEF_HV(OMAP_DSS_VIDEO2, 1));
	DUMPREG(DISPC_OVL_FIR_COEF_HV(OMAP_DSS_VIDEO2, 2));
	DUMPREG(DISPC_OVL_FIR_COEF_HV(OMAP_DSS_VIDEO2, 3));
	DUMPREG(DISPC_OVL_FIR_COEF_HV(OMAP_DSS_VIDEO2, 4));
	DUMPREG(DISPC_OVL_FIR_COEF_HV(OMAP_DSS_VIDEO2, 5));
	DUMPREG(DISPC_OVL_FIR_COEF_HV(OMAP_DSS_VIDEO2, 6));
	DUMPREG(DISPC_OVL_FIR_COEF_HV(OMAP_DSS_VIDEO2, 7));
	DUMPREG(DISPC_OVL_CONV_COEF(OMAP_DSS_VIDEO2, 0));
	DUMPREG(DISPC_OVL_CONV_COEF(OMAP_DSS_VIDEO2, 1));
	DUMPREG(DISPC_OVL_CONV_COEF(OMAP_DSS_VIDEO2, 2));
	DUMPREG(DISPC_OVL_CONV_COEF(OMAP_DSS_VIDEO2, 3));
	DUMPREG(DISPC_OVL_CONV_COEF(OMAP_DSS_VIDEO2, 4));
	DUMPREG(DISPC_OVL_FIR_COEF_V(OMAP_DSS_VIDEO2, 0));
	DUMPREG(DISPC_OVL_FIR_COEF_V(OMAP_DSS_VIDEO2, 1));
	DUMPREG(DISPC_OVL_FIR_COEF_V(OMAP_DSS_VIDEO2, 2));
	DUMPREG(DISPC_OVL_FIR_COEF_V(OMAP_DSS_VIDEO2, 3));
	DUMPREG(DISPC_OVL_FIR_COEF_V(OMAP_DSS_VIDEO2, 4));
	DUMPREG(DISPC_OVL_FIR_COEF_V(OMAP_DSS_VIDEO2, 5));
	DUMPREG(DISPC_OVL_FIR_COEF_V(OMAP_DSS_VIDEO2, 6));
	DUMPREG(DISPC_OVL_FIR_COEF_V(OMAP_DSS_VIDEO2, 7));

	if (dss_has_feature(FEAT_HANDLE_UV_SEPARATE)) {
		DUMPREG(DISPC_OVL_BA0_UV(OMAP_DSS_VIDEO2));
		DUMPREG(DISPC_OVL_BA1_UV(OMAP_DSS_VIDEO2));
		DUMPREG(DISPC_OVL_FIR2(OMAP_DSS_VIDEO2));
		DUMPREG(DISPC_OVL_ACCU2_0(OMAP_DSS_VIDEO2));
		DUMPREG(DISPC_OVL_ACCU2_1(OMAP_DSS_VIDEO2));

		DUMPREG(DISPC_OVL_FIR_COEF_H2(OMAP_DSS_VIDEO2, 0));
		DUMPREG(DISPC_OVL_FIR_COEF_H2(OMAP_DSS_VIDEO2, 1));
		DUMPREG(DISPC_OVL_FIR_COEF_H2(OMAP_DSS_VIDEO2, 2));
		DUMPREG(DISPC_OVL_FIR_COEF_H2(OMAP_DSS_VIDEO2, 3));
		DUMPREG(DISPC_OVL_FIR_COEF_H2(OMAP_DSS_VIDEO2, 4));
		DUMPREG(DISPC_OVL_FIR_COEF_H2(OMAP_DSS_VIDEO2, 5));
		DUMPREG(DISPC_OVL_FIR_COEF_H2(OMAP_DSS_VIDEO2, 6));
		DUMPREG(DISPC_OVL_FIR_COEF_H2(OMAP_DSS_VIDEO2, 7));

		DUMPREG(DISPC_OVL_FIR_COEF_HV2(OMAP_DSS_VIDEO2, 0));
		DUMPREG(DISPC_OVL_FIR_COEF_HV2(OMAP_DSS_VIDEO2, 1));
		DUMPREG(DISPC_OVL_FIR_COEF_HV2(OMAP_DSS_VIDEO2, 2));
		DUMPREG(DISPC_OVL_FIR_COEF_HV2(OMAP_DSS_VIDEO2, 3));
		DUMPREG(DISPC_OVL_FIR_COEF_HV2(OMAP_DSS_VIDEO2, 4));
		DUMPREG(DISPC_OVL_FIR_COEF_HV2(OMAP_DSS_VIDEO2, 5));
		DUMPREG(DISPC_OVL_FIR_COEF_HV2(OMAP_DSS_VIDEO2, 6));
		DUMPREG(DISPC_OVL_FIR_COEF_HV2(OMAP_DSS_VIDEO2, 7));

		DUMPREG(DISPC_OVL_FIR_COEF_V2(OMAP_DSS_VIDEO2, 0));
		DUMPREG(DISPC_OVL_FIR_COEF_V2(OMAP_DSS_VIDEO2, 1));
		DUMPREG(DISPC_OVL_FIR_COEF_V2(OMAP_DSS_VIDEO2, 2));
		DUMPREG(DISPC_OVL_FIR_COEF_V2(OMAP_DSS_VIDEO2, 3));
		DUMPREG(DISPC_OVL_FIR_COEF_V2(OMAP_DSS_VIDEO2, 4));
		DUMPREG(DISPC_OVL_FIR_COEF_V2(OMAP_DSS_VIDEO2, 5));
		DUMPREG(DISPC_OVL_FIR_COEF_V2(OMAP_DSS_VIDEO2, 6));
		DUMPREG(DISPC_OVL_FIR_COEF_V2(OMAP_DSS_VIDEO2, 7));
	}
	if (dss_has_feature(FEAT_ATTR2))
		DUMPREG(DISPC_OVL_ATTRIBUTES2(OMAP_DSS_VIDEO2));

	DUMPREG(DISPC_OVL_PRELOAD(OMAP_DSS_VIDEO1));
	DUMPREG(DISPC_OVL_PRELOAD(OMAP_DSS_VIDEO2));

	dss_clk_disable(DSS_CLK_ICK | DSS_CLK_FCK);
#undef DUMPREG
}

static void _dispc_set_pol_freq(enum omap_channel channel, bool onoff, bool rf,
		bool ieo, bool ipc, bool ihs, bool ivs, u8 acbi, u8 acb)
=======
=======
>>>>>>> refs/remotes/origin/master
	DUMPREG(DISPC_LINE_STATUS);
	DUMPREG(DISPC_LINE_NUMBER);
	if (dss_has_feature(FEAT_ALPHA_FIXED_ZORDER) ||
			dss_has_feature(FEAT_ALPHA_FREE_ZORDER))
		DUMPREG(DISPC_GLOBAL_ALPHA);
	if (dss_has_feature(FEAT_MGR_LCD2)) {
		DUMPREG(DISPC_CONTROL2);
		DUMPREG(DISPC_CONFIG2);
	}
<<<<<<< HEAD
=======
	if (dss_has_feature(FEAT_MGR_LCD3)) {
		DUMPREG(DISPC_CONTROL3);
		DUMPREG(DISPC_CONFIG3);
	}
>>>>>>> refs/remotes/origin/master

#undef DUMPREG

#define DISPC_REG(i, name) name(i)
#define DUMPREG(i, r) seq_printf(s, "%s(%s)%*s %08x\n", #r, p_names[i], \
<<<<<<< HEAD
	48 - strlen(#r) - strlen(p_names[i]), " ", \
=======
	(int)(48 - strlen(#r) - strlen(p_names[i])), " ", \
>>>>>>> refs/remotes/origin/master
	dispc_read_reg(DISPC_REG(i, r)))

	p_names = mgr_names;

	/* DISPC channel specific registers */
	for (i = 0; i < dss_feat_get_num_mgrs(); i++) {
		DUMPREG(i, DISPC_DEFAULT_COLOR);
		DUMPREG(i, DISPC_TRANS_COLOR);
		DUMPREG(i, DISPC_SIZE_MGR);

		if (i == OMAP_DSS_CHANNEL_DIGIT)
			continue;

		DUMPREG(i, DISPC_DEFAULT_COLOR);
		DUMPREG(i, DISPC_TRANS_COLOR);
		DUMPREG(i, DISPC_TIMING_H);
		DUMPREG(i, DISPC_TIMING_V);
		DUMPREG(i, DISPC_POL_FREQ);
		DUMPREG(i, DISPC_DIVISORo);
		DUMPREG(i, DISPC_SIZE_MGR);

		DUMPREG(i, DISPC_DATA_CYCLE1);
		DUMPREG(i, DISPC_DATA_CYCLE2);
		DUMPREG(i, DISPC_DATA_CYCLE3);

		if (dss_has_feature(FEAT_CPR)) {
			DUMPREG(i, DISPC_CPR_COEF_R);
			DUMPREG(i, DISPC_CPR_COEF_G);
			DUMPREG(i, DISPC_CPR_COEF_B);
		}
	}

	p_names = ovl_names;

	for (i = 0; i < dss_feat_get_num_ovls(); i++) {
		DUMPREG(i, DISPC_OVL_BA0);
		DUMPREG(i, DISPC_OVL_BA1);
		DUMPREG(i, DISPC_OVL_POSITION);
		DUMPREG(i, DISPC_OVL_SIZE);
		DUMPREG(i, DISPC_OVL_ATTRIBUTES);
		DUMPREG(i, DISPC_OVL_FIFO_THRESHOLD);
		DUMPREG(i, DISPC_OVL_FIFO_SIZE_STATUS);
		DUMPREG(i, DISPC_OVL_ROW_INC);
		DUMPREG(i, DISPC_OVL_PIXEL_INC);
		if (dss_has_feature(FEAT_PRELOAD))
			DUMPREG(i, DISPC_OVL_PRELOAD);

		if (i == OMAP_DSS_GFX) {
			DUMPREG(i, DISPC_OVL_WINDOW_SKIP);
			DUMPREG(i, DISPC_OVL_TABLE_BA);
			continue;
		}

		DUMPREG(i, DISPC_OVL_FIR);
		DUMPREG(i, DISPC_OVL_PICTURE_SIZE);
		DUMPREG(i, DISPC_OVL_ACCU0);
		DUMPREG(i, DISPC_OVL_ACCU1);
		if (dss_has_feature(FEAT_HANDLE_UV_SEPARATE)) {
			DUMPREG(i, DISPC_OVL_BA0_UV);
			DUMPREG(i, DISPC_OVL_BA1_UV);
			DUMPREG(i, DISPC_OVL_FIR2);
			DUMPREG(i, DISPC_OVL_ACCU2_0);
			DUMPREG(i, DISPC_OVL_ACCU2_1);
		}
		if (dss_has_feature(FEAT_ATTR2))
			DUMPREG(i, DISPC_OVL_ATTRIBUTES2);
		if (dss_has_feature(FEAT_PRELOAD))
			DUMPREG(i, DISPC_OVL_PRELOAD);
	}

#undef DISPC_REG
#undef DUMPREG

#define DISPC_REG(plane, name, i) name(plane, i)
#define DUMPREG(plane, name, i) \
	seq_printf(s, "%s_%d(%s)%*s %08x\n", #name, i, p_names[plane], \
<<<<<<< HEAD
	46 - strlen(#name) - strlen(p_names[plane]), " ", \
=======
	(int)(46 - strlen(#name) - strlen(p_names[plane])), " ", \
>>>>>>> refs/remotes/origin/master
	dispc_read_reg(DISPC_REG(plane, name, i)))

	/* Video pipeline coefficient registers */

	/* start from OMAP_DSS_VIDEO1 */
	for (i = 1; i < dss_feat_get_num_ovls(); i++) {
		for (j = 0; j < 8; j++)
			DUMPREG(i, DISPC_OVL_FIR_COEF_H, j);

		for (j = 0; j < 8; j++)
			DUMPREG(i, DISPC_OVL_FIR_COEF_HV, j);

		for (j = 0; j < 5; j++)
			DUMPREG(i, DISPC_OVL_CONV_COEF, j);

		if (dss_has_feature(FEAT_FIR_COEF_V)) {
			for (j = 0; j < 8; j++)
				DUMPREG(i, DISPC_OVL_FIR_COEF_V, j);
		}

		if (dss_has_feature(FEAT_HANDLE_UV_SEPARATE)) {
			for (j = 0; j < 8; j++)
				DUMPREG(i, DISPC_OVL_FIR_COEF_H2, j);

			for (j = 0; j < 8; j++)
				DUMPREG(i, DISPC_OVL_FIR_COEF_HV2, j);

			for (j = 0; j < 8; j++)
				DUMPREG(i, DISPC_OVL_FIR_COEF_V2, j);
		}
	}

	dispc_runtime_put();

#undef DISPC_REG
#undef DUMPREG
}

<<<<<<< HEAD
static void _dispc_mgr_set_pol_freq(enum omap_channel channel, bool onoff,
		bool rf, bool ieo, bool ipc, bool ihs, bool ivs, u8 acbi,
		u8 acb)
>>>>>>> refs/remotes/origin/cm-10.0
{
	u32 l = 0;

	DSSDBG("onoff %d rf %d ieo %d ipc %d ihs %d ivs %d acbi %d acb %d\n",
			onoff, rf, ieo, ipc, ihs, ivs, acbi, acb);

	l |= FLD_VAL(onoff, 17, 17);
	l |= FLD_VAL(rf, 16, 16);
	l |= FLD_VAL(ieo, 15, 15);
	l |= FLD_VAL(ipc, 14, 14);
	l |= FLD_VAL(ihs, 13, 13);
	l |= FLD_VAL(ivs, 12, 12);
	l |= FLD_VAL(acbi, 11, 8);
	l |= FLD_VAL(acb, 7, 0);

<<<<<<< HEAD
	enable_clocks(1);
	dispc_write_reg(DISPC_POL_FREQ(channel), l);
	enable_clocks(0);
}

void dispc_set_pol_freq(enum omap_channel channel,
		enum omap_panel_config config, u8 acbi, u8 acb)
{
	_dispc_set_pol_freq(channel, (config & OMAP_DSS_LCD_ONOFF) != 0,
=======
	dispc_write_reg(DISPC_POL_FREQ(channel), l);
}

void dispc_mgr_set_pol_freq(enum omap_channel channel,
		enum omap_panel_config config, u8 acbi, u8 acb)
{
	_dispc_mgr_set_pol_freq(channel, (config & OMAP_DSS_LCD_ONOFF) != 0,
>>>>>>> refs/remotes/origin/cm-10.0
			(config & OMAP_DSS_LCD_RF) != 0,
			(config & OMAP_DSS_LCD_IEO) != 0,
			(config & OMAP_DSS_LCD_IPC) != 0,
			(config & OMAP_DSS_LCD_IHS) != 0,
			(config & OMAP_DSS_LCD_IVS) != 0,
			acbi, acb);
}

/* with fck as input clock rate, find dispc dividers that produce req_pck */
void dispc_find_clk_divs(bool is_tft, unsigned long req_pck, unsigned long fck,
		struct dispc_clock_info *cinfo)
{
<<<<<<< HEAD
	u16 pcd_min = is_tft ? 2 : 3;
=======
	u16 pcd_min, pcd_max;
>>>>>>> refs/remotes/origin/cm-10.0
	unsigned long best_pck;
	u16 best_ld, cur_ld;
	u16 best_pd, cur_pd;

<<<<<<< HEAD
=======
	pcd_min = dss_feat_get_param_min(FEAT_PARAM_DSS_PCD);
	pcd_max = dss_feat_get_param_max(FEAT_PARAM_DSS_PCD);

	if (!is_tft)
		pcd_min = 3;

>>>>>>> refs/remotes/origin/cm-10.0
	best_pck = 0;
	best_ld = 0;
	best_pd = 0;

	for (cur_ld = 1; cur_ld <= 255; ++cur_ld) {
		unsigned long lck = fck / cur_ld;

<<<<<<< HEAD
		for (cur_pd = pcd_min; cur_pd <= 255; ++cur_pd) {
=======
		for (cur_pd = pcd_min; cur_pd <= pcd_max; ++cur_pd) {
>>>>>>> refs/remotes/origin/cm-10.0
			unsigned long pck = lck / cur_pd;
			long old_delta = abs(best_pck - req_pck);
			long new_delta = abs(pck - req_pck);

			if (best_pck == 0 || new_delta < old_delta) {
				best_pck = pck;
				best_ld = cur_ld;
				best_pd = cur_pd;

				if (pck == req_pck)
					goto found;
			}

			if (pck < req_pck)
				break;
		}

		if (lck / pcd_min < req_pck)
			break;
	}

found:
	cinfo->lck_div = best_ld;
	cinfo->pck_div = best_pd;
	cinfo->lck = fck / cinfo->lck_div;
	cinfo->pck = cinfo->lck / cinfo->pck_div;
}

/* calculate clock rates using dividers in cinfo */
int dispc_calc_clock_rates(unsigned long dispc_fclk_rate,
		struct dispc_clock_info *cinfo)
{
	if (cinfo->lck_div > 255 || cinfo->lck_div == 0)
		return -EINVAL;
<<<<<<< HEAD
	if (cinfo->pck_div < 2 || cinfo->pck_div > 255)
=======
	if (cinfo->pck_div < 1 || cinfo->pck_div > 255)
>>>>>>> refs/remotes/origin/cm-10.0
		return -EINVAL;

	cinfo->lck = dispc_fclk_rate / cinfo->lck_div;
	cinfo->pck = cinfo->lck / cinfo->pck_div;

	return 0;
}

<<<<<<< HEAD
int dispc_set_clock_div(enum omap_channel channel,
=======
int dispc_mgr_set_clock_div(enum omap_channel channel,
>>>>>>> refs/remotes/origin/cm-10.0
		struct dispc_clock_info *cinfo)
=======
/* calculate clock rates using dividers in cinfo */
int dispc_calc_clock_rates(unsigned long dispc_fclk_rate,
		struct dispc_clock_info *cinfo)
{
	if (cinfo->lck_div > 255 || cinfo->lck_div == 0)
		return -EINVAL;
	if (cinfo->pck_div < 1 || cinfo->pck_div > 255)
		return -EINVAL;

	cinfo->lck = dispc_fclk_rate / cinfo->lck_div;
	cinfo->pck = cinfo->lck / cinfo->pck_div;

	return 0;
}

bool dispc_div_calc(unsigned long dispc,
		unsigned long pck_min, unsigned long pck_max,
		dispc_div_calc_func func, void *data)
{
	int lckd, lckd_start, lckd_stop;
	int pckd, pckd_start, pckd_stop;
	unsigned long pck, lck;
	unsigned long lck_max;
	unsigned long pckd_hw_min, pckd_hw_max;
	unsigned min_fck_per_pck;
	unsigned long fck;

#ifdef CONFIG_OMAP2_DSS_MIN_FCK_PER_PCK
	min_fck_per_pck = CONFIG_OMAP2_DSS_MIN_FCK_PER_PCK;
#else
	min_fck_per_pck = 0;
#endif

	pckd_hw_min = dss_feat_get_param_min(FEAT_PARAM_DSS_PCD);
	pckd_hw_max = dss_feat_get_param_max(FEAT_PARAM_DSS_PCD);

	lck_max = dss_feat_get_param_max(FEAT_PARAM_DSS_FCK);

	pck_min = pck_min ? pck_min : 1;
	pck_max = pck_max ? pck_max : ULONG_MAX;

	lckd_start = max(DIV_ROUND_UP(dispc, lck_max), 1ul);
	lckd_stop = min(dispc / pck_min, 255ul);

	for (lckd = lckd_start; lckd <= lckd_stop; ++lckd) {
		lck = dispc / lckd;

		pckd_start = max(DIV_ROUND_UP(lck, pck_max), pckd_hw_min);
		pckd_stop = min(lck / pck_min, pckd_hw_max);

		for (pckd = pckd_start; pckd <= pckd_stop; ++pckd) {
			pck = lck / pckd;

			/*
			 * For OMAP2/3 the DISPC fclk is the same as LCD's logic
			 * clock, which means we're configuring DISPC fclk here
			 * also. Thus we need to use the calculated lck. For
			 * OMAP4+ the DISPC fclk is a separate clock.
			 */
			if (dss_has_feature(FEAT_CORE_CLK_DIV))
				fck = dispc_core_clk_rate();
			else
				fck = lck;

			if (fck < pck * min_fck_per_pck)
				continue;

			if (func(lckd, pckd, lck, pck, data))
				return true;
		}
	}

	return false;
}

void dispc_mgr_set_clock_div(enum omap_channel channel,
		const struct dispc_clock_info *cinfo)
>>>>>>> refs/remotes/origin/master
{
	DSSDBG("lck = %lu (%u)\n", cinfo->lck, cinfo->lck_div);
	DSSDBG("pck = %lu (%u)\n", cinfo->pck, cinfo->pck_div);

<<<<<<< HEAD
<<<<<<< HEAD
	dispc_set_lcd_divisor(channel, cinfo->lck_div, cinfo->pck_div);
=======
	dispc_mgr_set_lcd_divisor(channel, cinfo->lck_div, cinfo->pck_div);
>>>>>>> refs/remotes/origin/cm-10.0

	return 0;
}

<<<<<<< HEAD
int dispc_get_clock_div(enum omap_channel channel,
=======
int dispc_mgr_get_clock_div(enum omap_channel channel,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	dispc_mgr_set_lcd_divisor(channel, cinfo->lck_div, cinfo->pck_div);
}

int dispc_mgr_get_clock_div(enum omap_channel channel,
>>>>>>> refs/remotes/origin/master
		struct dispc_clock_info *cinfo)
{
	unsigned long fck;

	fck = dispc_fclk_rate();

	cinfo->lck_div = REG_GET(DISPC_DIVISORo(channel), 23, 16);
	cinfo->pck_div = REG_GET(DISPC_DIVISORo(channel), 7, 0);

	cinfo->lck = fck / cinfo->lck_div;
	cinfo->pck = cinfo->lck / cinfo->pck_div;

	return 0;
}

<<<<<<< HEAD
/* dispc.irq_lock has to be locked by the caller */
static void _omap_dispc_set_irqs(void)
{
	u32 mask;
	u32 old_mask;
	int i;
	struct omap_dispc_isr_data *isr_data;

	mask = dispc.irq_error_mask;

	for (i = 0; i < DISPC_MAX_NR_ISRS; i++) {
		isr_data = &dispc.registered_isr[i];

		if (isr_data->isr == NULL)
			continue;

		mask |= isr_data->mask;
	}

<<<<<<< HEAD
	enable_clocks(1);

=======
>>>>>>> refs/remotes/origin/cm-10.0
	old_mask = dispc_read_reg(DISPC_IRQENABLE);
	/* clear the irqstatus for newly enabled irqs */
	dispc_write_reg(DISPC_IRQSTATUS, (mask ^ old_mask) & mask);

	dispc_write_reg(DISPC_IRQENABLE, mask);
<<<<<<< HEAD

	enable_clocks(0);
=======
>>>>>>> refs/remotes/origin/cm-10.0
}

int omap_dispc_register_isr(omap_dispc_isr_t isr, void *arg, u32 mask)
{
	int i;
	int ret;
	unsigned long flags;
	struct omap_dispc_isr_data *isr_data;

	if (isr == NULL)
		return -EINVAL;

	spin_lock_irqsave(&dispc.irq_lock, flags);

	/* check for duplicate entry */
	for (i = 0; i < DISPC_MAX_NR_ISRS; i++) {
		isr_data = &dispc.registered_isr[i];
		if (isr_data->isr == isr && isr_data->arg == arg &&
				isr_data->mask == mask) {
			ret = -EINVAL;
			goto err;
		}
	}

	isr_data = NULL;
	ret = -EBUSY;

	for (i = 0; i < DISPC_MAX_NR_ISRS; i++) {
		isr_data = &dispc.registered_isr[i];

		if (isr_data->isr != NULL)
			continue;

		isr_data->isr = isr;
		isr_data->arg = arg;
		isr_data->mask = mask;
		ret = 0;

		break;
	}

	if (ret)
		goto err;

	_omap_dispc_set_irqs();

	spin_unlock_irqrestore(&dispc.irq_lock, flags);

	return 0;
err:
	spin_unlock_irqrestore(&dispc.irq_lock, flags);

	return ret;
}
EXPORT_SYMBOL(omap_dispc_register_isr);

int omap_dispc_unregister_isr(omap_dispc_isr_t isr, void *arg, u32 mask)
{
	int i;
	unsigned long flags;
	int ret = -EINVAL;
	struct omap_dispc_isr_data *isr_data;

	spin_lock_irqsave(&dispc.irq_lock, flags);

	for (i = 0; i < DISPC_MAX_NR_ISRS; i++) {
		isr_data = &dispc.registered_isr[i];
		if (isr_data->isr != isr || isr_data->arg != arg ||
				isr_data->mask != mask)
			continue;

		/* found the correct isr */

		isr_data->isr = NULL;
		isr_data->arg = NULL;
		isr_data->mask = 0;

		ret = 0;
		break;
	}

	if (ret == 0)
		_omap_dispc_set_irqs();

	spin_unlock_irqrestore(&dispc.irq_lock, flags);

	return ret;
}
EXPORT_SYMBOL(omap_dispc_unregister_isr);

#ifdef DEBUG
static void print_irq_status(u32 status)
{
	if ((status & dispc.irq_error_mask) == 0)
		return;

	printk(KERN_DEBUG "DISPC IRQ: 0x%x: ", status);

#define PIS(x) \
	if (status & DISPC_IRQ_##x) \
		printk(#x " ");
	PIS(GFX_FIFO_UNDERFLOW);
	PIS(OCP_ERR);
	PIS(VID1_FIFO_UNDERFLOW);
	PIS(VID2_FIFO_UNDERFLOW);
<<<<<<< HEAD
=======
	if (dss_feat_get_num_ovls() > 3)
		PIS(VID3_FIFO_UNDERFLOW);
>>>>>>> refs/remotes/origin/cm-10.0
	PIS(SYNC_LOST);
	PIS(SYNC_LOST_DIGIT);
	if (dss_has_feature(FEAT_MGR_LCD2))
		PIS(SYNC_LOST2);
#undef PIS

	printk("\n");
}
#endif

/* Called from dss.c. Note that we don't touch clocks here,
 * but we presume they are on because we got an IRQ. However,
 * an irq handler may turn the clocks off, so we may not have
 * clock later in the function. */
static irqreturn_t omap_dispc_irq_handler(int irq, void *arg)
{
	int i;
	u32 irqstatus, irqenable;
	u32 handledirqs = 0;
	u32 unhandled_errors;
	struct omap_dispc_isr_data *isr_data;
	struct omap_dispc_isr_data registered_isr[DISPC_MAX_NR_ISRS];

	spin_lock(&dispc.irq_lock);

	irqstatus = dispc_read_reg(DISPC_IRQSTATUS);
	irqenable = dispc_read_reg(DISPC_IRQENABLE);

	/* IRQ is not for us */
	if (!(irqstatus & irqenable)) {
		spin_unlock(&dispc.irq_lock);
		return IRQ_NONE;
	}

#ifdef CONFIG_OMAP2_DSS_COLLECT_IRQ_STATS
	spin_lock(&dispc.irq_stats_lock);
	dispc.irq_stats.irq_count++;
	dss_collect_irq_stats(irqstatus, dispc.irq_stats.irqs);
	spin_unlock(&dispc.irq_stats_lock);
#endif

#ifdef DEBUG
	if (dss_debug)
		print_irq_status(irqstatus);
#endif
	/* Ack the interrupt. Do it here before clocks are possibly turned
	 * off */
	dispc_write_reg(DISPC_IRQSTATUS, irqstatus);
	/* flush posted write */
	dispc_read_reg(DISPC_IRQSTATUS);

	/* make a copy and unlock, so that isrs can unregister
	 * themselves */
	memcpy(registered_isr, dispc.registered_isr,
			sizeof(registered_isr));

	spin_unlock(&dispc.irq_lock);

	for (i = 0; i < DISPC_MAX_NR_ISRS; i++) {
		isr_data = &registered_isr[i];

		if (!isr_data->isr)
			continue;

		if (isr_data->mask & irqstatus) {
			isr_data->isr(isr_data->arg, irqstatus);
			handledirqs |= isr_data->mask;
		}
	}

	spin_lock(&dispc.irq_lock);

	unhandled_errors = irqstatus & ~handledirqs & dispc.irq_error_mask;

	if (unhandled_errors) {
		dispc.error_irqs |= unhandled_errors;

		dispc.irq_error_mask &= ~unhandled_errors;
		_omap_dispc_set_irqs();

		schedule_work(&dispc.error_work);
	}

	spin_unlock(&dispc.irq_lock);

	return IRQ_HANDLED;
}

static void dispc_error_worker(struct work_struct *work)
{
	int i;
	u32 errors;
	unsigned long flags;
<<<<<<< HEAD
=======
	static const unsigned fifo_underflow_bits[] = {
		DISPC_IRQ_GFX_FIFO_UNDERFLOW,
		DISPC_IRQ_VID1_FIFO_UNDERFLOW,
		DISPC_IRQ_VID2_FIFO_UNDERFLOW,
		DISPC_IRQ_VID3_FIFO_UNDERFLOW,
	};

	static const unsigned sync_lost_bits[] = {
		DISPC_IRQ_SYNC_LOST,
		DISPC_IRQ_SYNC_LOST_DIGIT,
		DISPC_IRQ_SYNC_LOST2,
	};
>>>>>>> refs/remotes/origin/cm-10.0

	spin_lock_irqsave(&dispc.irq_lock, flags);
	errors = dispc.error_irqs;
	dispc.error_irqs = 0;
	spin_unlock_irqrestore(&dispc.irq_lock, flags);

<<<<<<< HEAD
	if (errors & DISPC_IRQ_GFX_FIFO_UNDERFLOW) {
		DSSERR("GFX_FIFO_UNDERFLOW, disabling GFX\n");
		for (i = 0; i < omap_dss_get_num_overlays(); ++i) {
			struct omap_overlay *ovl;
			ovl = omap_dss_get_overlay(i);

			if (!(ovl->caps & OMAP_DSS_OVL_CAP_DISPC))
				continue;

			if (ovl->id == 0) {
				dispc_enable_plane(ovl->id, 0);
				dispc_go(ovl->manager->id);
				mdelay(50);
				break;
			}
		}
	}

	if (errors & DISPC_IRQ_VID1_FIFO_UNDERFLOW) {
		DSSERR("VID1_FIFO_UNDERFLOW, disabling VID1\n");
		for (i = 0; i < omap_dss_get_num_overlays(); ++i) {
			struct omap_overlay *ovl;
			ovl = omap_dss_get_overlay(i);

			if (!(ovl->caps & OMAP_DSS_OVL_CAP_DISPC))
				continue;

			if (ovl->id == 1) {
				dispc_enable_plane(ovl->id, 0);
				dispc_go(ovl->manager->id);
				mdelay(50);
				break;
			}
		}
	}

	if (errors & DISPC_IRQ_VID2_FIFO_UNDERFLOW) {
		DSSERR("VID2_FIFO_UNDERFLOW, disabling VID2\n");
		for (i = 0; i < omap_dss_get_num_overlays(); ++i) {
			struct omap_overlay *ovl;
			ovl = omap_dss_get_overlay(i);

			if (!(ovl->caps & OMAP_DSS_OVL_CAP_DISPC))
				continue;

			if (ovl->id == 2) {
				dispc_enable_plane(ovl->id, 0);
				dispc_go(ovl->manager->id);
				mdelay(50);
				break;
			}
		}
	}

	if (errors & DISPC_IRQ_SYNC_LOST) {
		struct omap_overlay_manager *manager = NULL;
		bool enable = false;

		DSSERR("SYNC_LOST, disabling LCD\n");

		for (i = 0; i < omap_dss_get_num_overlay_managers(); ++i) {
			struct omap_overlay_manager *mgr;
			mgr = omap_dss_get_overlay_manager(i);

			if (mgr->id == OMAP_DSS_CHANNEL_LCD) {
				manager = mgr;
				enable = mgr->device->state ==
						OMAP_DSS_DISPLAY_ACTIVE;
				mgr->device->driver->disable(mgr->device);
				break;
			}
		}

		if (manager) {
			struct omap_dss_device *dssdev = manager->device;
			for (i = 0; i < omap_dss_get_num_overlays(); ++i) {
				struct omap_overlay *ovl;
				ovl = omap_dss_get_overlay(i);

				if (!(ovl->caps & OMAP_DSS_OVL_CAP_DISPC))
					continue;

				if (ovl->id != 0 && ovl->manager == manager)
					dispc_enable_plane(ovl->id, 0);
			}

			dispc_go(manager->id);
			mdelay(50);
			if (enable)
				dssdev->driver->enable(dssdev);
		}
	}

	if (errors & DISPC_IRQ_SYNC_LOST_DIGIT) {
		struct omap_overlay_manager *manager = NULL;
		bool enable = false;

		DSSERR("SYNC_LOST_DIGIT, disabling TV\n");

		for (i = 0; i < omap_dss_get_num_overlay_managers(); ++i) {
			struct omap_overlay_manager *mgr;
			mgr = omap_dss_get_overlay_manager(i);

			if (mgr->id == OMAP_DSS_CHANNEL_DIGIT) {
				manager = mgr;
				enable = mgr->device->state ==
						OMAP_DSS_DISPLAY_ACTIVE;
				mgr->device->driver->disable(mgr->device);
				break;
			}
		}

		if (manager) {
			struct omap_dss_device *dssdev = manager->device;
			for (i = 0; i < omap_dss_get_num_overlays(); ++i) {
				struct omap_overlay *ovl;
				ovl = omap_dss_get_overlay(i);

				if (!(ovl->caps & OMAP_DSS_OVL_CAP_DISPC))
					continue;

				if (ovl->id != 0 && ovl->manager == manager)
					dispc_enable_plane(ovl->id, 0);
			}

			dispc_go(manager->id);
			mdelay(50);
			if (enable)
				dssdev->driver->enable(dssdev);
		}
	}

	if (errors & DISPC_IRQ_SYNC_LOST2) {
		struct omap_overlay_manager *manager = NULL;
		bool enable = false;

		DSSERR("SYNC_LOST for LCD2, disabling LCD2\n");

		for (i = 0; i < omap_dss_get_num_overlay_managers(); ++i) {
			struct omap_overlay_manager *mgr;
			mgr = omap_dss_get_overlay_manager(i);

			if (mgr->id == OMAP_DSS_CHANNEL_LCD2) {
				manager = mgr;
				enable = mgr->device->state ==
						OMAP_DSS_DISPLAY_ACTIVE;
				mgr->device->driver->disable(mgr->device);
				break;
			}
		}

		if (manager) {
			struct omap_dss_device *dssdev = manager->device;
=======
	dispc_runtime_get();

	for (i = 0; i < omap_dss_get_num_overlays(); ++i) {
		struct omap_overlay *ovl;
		unsigned bit;

		ovl = omap_dss_get_overlay(i);
		bit = fifo_underflow_bits[i];

		if (bit & errors) {
			DSSERR("FIFO UNDERFLOW on %s, disabling the overlay\n",
					ovl->name);
			dispc_ovl_enable(ovl->id, false);
			dispc_mgr_go(ovl->manager->id);
			mdelay(50);
		}
	}

	for (i = 0; i < omap_dss_get_num_overlay_managers(); ++i) {
		struct omap_overlay_manager *mgr;
		unsigned bit;

		mgr = omap_dss_get_overlay_manager(i);
		bit = sync_lost_bits[i];

		if (bit & errors) {
			struct omap_dss_device *dssdev = mgr->device;
			bool enable;

			DSSERR("SYNC_LOST on channel %s, restarting the output "
					"with video overlays disabled\n",
					mgr->name);

			enable = dssdev->state == OMAP_DSS_DISPLAY_ACTIVE;
			dssdev->driver->disable(dssdev);

>>>>>>> refs/remotes/origin/cm-10.0
			for (i = 0; i < omap_dss_get_num_overlays(); ++i) {
				struct omap_overlay *ovl;
				ovl = omap_dss_get_overlay(i);

<<<<<<< HEAD
				if (!(ovl->caps & OMAP_DSS_OVL_CAP_DISPC))
					continue;

				if (ovl->id != 0 && ovl->manager == manager)
					dispc_enable_plane(ovl->id, 0);
			}

			dispc_go(manager->id);
			mdelay(50);
=======
				if (ovl->id != OMAP_DSS_GFX &&
						ovl->manager == mgr)
					dispc_ovl_enable(ovl->id, false);
			}

			dispc_mgr_go(mgr->id);
			mdelay(50);

>>>>>>> refs/remotes/origin/cm-10.0
			if (enable)
				dssdev->driver->enable(dssdev);
		}
	}

	if (errors & DISPC_IRQ_OCP_ERR) {
		DSSERR("OCP_ERR\n");
		for (i = 0; i < omap_dss_get_num_overlay_managers(); ++i) {
			struct omap_overlay_manager *mgr;
			mgr = omap_dss_get_overlay_manager(i);
<<<<<<< HEAD

			if (mgr->caps & OMAP_DSS_OVL_CAP_DISPC)
=======
			if (mgr->device && mgr->device->driver)
>>>>>>> refs/remotes/origin/cm-10.0
				mgr->device->driver->disable(mgr->device);
		}
	}

	spin_lock_irqsave(&dispc.irq_lock, flags);
	dispc.irq_error_mask |= errors;
	_omap_dispc_set_irqs();
	spin_unlock_irqrestore(&dispc.irq_lock, flags);
<<<<<<< HEAD
=======

	dispc_runtime_put();
>>>>>>> refs/remotes/origin/cm-10.0
}

int omap_dispc_wait_for_irq_timeout(u32 irqmask, unsigned long timeout)
{
	void dispc_irq_wait_handler(void *data, u32 mask)
	{
		complete((struct completion *)data);
	}

	int r;
	DECLARE_COMPLETION_ONSTACK(completion);

	r = omap_dispc_register_isr(dispc_irq_wait_handler, &completion,
			irqmask);

	if (r)
		return r;

	timeout = wait_for_completion_timeout(&completion, timeout);

	omap_dispc_unregister_isr(dispc_irq_wait_handler, &completion, irqmask);

	if (timeout == 0)
		return -ETIMEDOUT;

	if (timeout == -ERESTARTSYS)
		return -ERESTARTSYS;

	return 0;
}

int omap_dispc_wait_for_irq_interruptible_timeout(u32 irqmask,
		unsigned long timeout)
{
	void dispc_irq_wait_handler(void *data, u32 mask)
	{
		complete((struct completion *)data);
	}

	int r;
	DECLARE_COMPLETION_ONSTACK(completion);

	r = omap_dispc_register_isr(dispc_irq_wait_handler, &completion,
			irqmask);

	if (r)
		return r;

	timeout = wait_for_completion_interruptible_timeout(&completion,
			timeout);

	omap_dispc_unregister_isr(dispc_irq_wait_handler, &completion, irqmask);

	if (timeout == 0)
		return -ETIMEDOUT;

	if (timeout == -ERESTARTSYS)
		return -ERESTARTSYS;
=======
u32 dispc_read_irqstatus(void)
{
	return dispc_read_reg(DISPC_IRQSTATUS);
}
EXPORT_SYMBOL(dispc_read_irqstatus);

void dispc_clear_irqstatus(u32 mask)
{
	dispc_write_reg(DISPC_IRQSTATUS, mask);
}
EXPORT_SYMBOL(dispc_clear_irqstatus);

u32 dispc_read_irqenable(void)
{
	return dispc_read_reg(DISPC_IRQENABLE);
}
EXPORT_SYMBOL(dispc_read_irqenable);

void dispc_write_irqenable(u32 mask)
{
	u32 old_mask = dispc_read_reg(DISPC_IRQENABLE);

	/* clear the irqstatus for newly enabled irqs */
	dispc_clear_irqstatus((mask ^ old_mask) & mask);

	dispc_write_reg(DISPC_IRQENABLE, mask);
}
EXPORT_SYMBOL(dispc_write_irqenable);

void dispc_enable_sidle(void)
{
	REG_FLD_MOD(DISPC_SYSCONFIG, 2, 4, 3);	/* SIDLEMODE: smart idle */
}

void dispc_disable_sidle(void)
{
	REG_FLD_MOD(DISPC_SYSCONFIG, 1, 4, 3);	/* SIDLEMODE: no idle */
}

static void _omap_dispc_initial_config(void)
{
	u32 l;

	/* Exclusively enable DISPC_CORE_CLK and set divider to 1 */
	if (dss_has_feature(FEAT_CORE_CLK_DIV)) {
		l = dispc_read_reg(DISPC_DIVISOR);
		/* Use DISPC_DIVISOR.LCD, instead of DISPC_DIVISOR1.LCD */
		l = FLD_MOD(l, 1, 0, 0);
		l = FLD_MOD(l, 1, 23, 16);
		dispc_write_reg(DISPC_DIVISOR, l);

		dispc.core_clk_rate = dispc_fclk_rate();
	}

	/* FUNCGATED */
	if (dss_has_feature(FEAT_FUNCGATED))
		REG_FLD_MOD(DISPC_CONFIG, 1, 9, 9);

	dispc_setup_color_conv_coef();

	dispc_set_loadmode(OMAP_DSS_LOAD_FRAME_ONLY);

	dispc_init_fifos();

	dispc_configure_burst_sizes();

	dispc_ovl_enable_zorder_planes();

	if (dispc.feat->mstandby_workaround)
		REG_FLD_MOD(DISPC_MSTANDBY_CTRL, 1, 0, 0);
}

static const struct dispc_features omap24xx_dispc_feats __initconst = {
	.sw_start		=	5,
	.fp_start		=	15,
	.bp_start		=	27,
	.sw_max			=	64,
	.vp_max			=	255,
	.hp_max			=	256,
	.mgr_width_start	=	10,
	.mgr_height_start	=	26,
	.mgr_width_max		=	2048,
	.mgr_height_max		=	2048,
	.max_lcd_pclk		=	66500000,
	.calc_scaling		=	dispc_ovl_calc_scaling_24xx,
	.calc_core_clk		=	calc_core_clk_24xx,
	.num_fifos		=	3,
	.no_framedone_tv	=	true,
};

static const struct dispc_features omap34xx_rev1_0_dispc_feats __initconst = {
	.sw_start		=	5,
	.fp_start		=	15,
	.bp_start		=	27,
	.sw_max			=	64,
	.vp_max			=	255,
	.hp_max			=	256,
	.mgr_width_start	=	10,
	.mgr_height_start	=	26,
	.mgr_width_max		=	2048,
	.mgr_height_max		=	2048,
	.max_lcd_pclk		=	173000000,
	.max_tv_pclk		=	59000000,
	.calc_scaling		=	dispc_ovl_calc_scaling_34xx,
	.calc_core_clk		=	calc_core_clk_34xx,
	.num_fifos		=	3,
	.no_framedone_tv	=	true,
};

static const struct dispc_features omap34xx_rev3_0_dispc_feats __initconst = {
	.sw_start		=	7,
	.fp_start		=	19,
	.bp_start		=	31,
	.sw_max			=	256,
	.vp_max			=	4095,
	.hp_max			=	4096,
	.mgr_width_start	=	10,
	.mgr_height_start	=	26,
	.mgr_width_max		=	2048,
	.mgr_height_max		=	2048,
	.max_lcd_pclk		=	173000000,
	.max_tv_pclk		=	59000000,
	.calc_scaling		=	dispc_ovl_calc_scaling_34xx,
	.calc_core_clk		=	calc_core_clk_34xx,
	.num_fifos		=	3,
	.no_framedone_tv	=	true,
};

static const struct dispc_features omap44xx_dispc_feats __initconst = {
	.sw_start		=	7,
	.fp_start		=	19,
	.bp_start		=	31,
	.sw_max			=	256,
	.vp_max			=	4095,
	.hp_max			=	4096,
	.mgr_width_start	=	10,
	.mgr_height_start	=	26,
	.mgr_width_max		=	2048,
	.mgr_height_max		=	2048,
	.max_lcd_pclk		=	170000000,
	.max_tv_pclk		=	185625000,
	.calc_scaling		=	dispc_ovl_calc_scaling_44xx,
	.calc_core_clk		=	calc_core_clk_44xx,
	.num_fifos		=	5,
	.gfx_fifo_workaround	=	true,
};

static const struct dispc_features omap54xx_dispc_feats __initconst = {
	.sw_start		=	7,
	.fp_start		=	19,
	.bp_start		=	31,
	.sw_max			=	256,
	.vp_max			=	4095,
	.hp_max			=	4096,
	.mgr_width_start	=	11,
	.mgr_height_start	=	27,
	.mgr_width_max		=	4096,
	.mgr_height_max		=	4096,
	.max_lcd_pclk		=	170000000,
	.max_tv_pclk		=	186000000,
	.calc_scaling		=	dispc_ovl_calc_scaling_44xx,
	.calc_core_clk		=	calc_core_clk_44xx,
	.num_fifos		=	5,
	.gfx_fifo_workaround	=	true,
	.mstandby_workaround	=	true,
};

static int __init dispc_init_features(struct platform_device *pdev)
{
	const struct dispc_features *src;
	struct dispc_features *dst;

	dst = devm_kzalloc(&pdev->dev, sizeof(*dst), GFP_KERNEL);
	if (!dst) {
		dev_err(&pdev->dev, "Failed to allocate DISPC Features\n");
		return -ENOMEM;
	}

	switch (omapdss_get_version()) {
	case OMAPDSS_VER_OMAP24xx:
		src = &omap24xx_dispc_feats;
		break;

	case OMAPDSS_VER_OMAP34xx_ES1:
		src = &omap34xx_rev1_0_dispc_feats;
		break;

	case OMAPDSS_VER_OMAP34xx_ES3:
	case OMAPDSS_VER_OMAP3630:
	case OMAPDSS_VER_AM35xx:
		src = &omap34xx_rev3_0_dispc_feats;
		break;

	case OMAPDSS_VER_OMAP4430_ES1:
	case OMAPDSS_VER_OMAP4430_ES2:
	case OMAPDSS_VER_OMAP4:
		src = &omap44xx_dispc_feats;
		break;

	case OMAPDSS_VER_OMAP5:
		src = &omap54xx_dispc_feats;
		break;

	default:
		return -ENODEV;
	}

	memcpy(dst, src, sizeof(*dst));
	dispc.feat = dst;
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
#ifdef CONFIG_OMAP2_DSS_FAKE_VSYNC
void dispc_fake_vsync_irq(void)
{
	u32 irqstatus = DISPC_IRQ_VSYNC;
	int i;

	WARN_ON(!in_interrupt());

	for (i = 0; i < DISPC_MAX_NR_ISRS; i++) {
		struct omap_dispc_isr_data *isr_data;
		isr_data = &dispc.registered_isr[i];

		if (!isr_data->isr)
			continue;

		if (isr_data->mask & irqstatus)
			isr_data->isr(isr_data->arg, irqstatus);
	}
}
#endif

static void _omap_dispc_initialize_irq(void)
{
	unsigned long flags;

	spin_lock_irqsave(&dispc.irq_lock, flags);

	memset(dispc.registered_isr, 0, sizeof(dispc.registered_isr));

	dispc.irq_error_mask = DISPC_IRQ_MASK_ERROR;
	if (dss_has_feature(FEAT_MGR_LCD2))
		dispc.irq_error_mask |= DISPC_IRQ_SYNC_LOST2;
<<<<<<< HEAD
=======
	if (dss_feat_get_num_ovls() > 3)
		dispc.irq_error_mask |= DISPC_IRQ_VID3_FIFO_UNDERFLOW;
>>>>>>> refs/remotes/origin/cm-10.0

	/* there's SYNC_LOST_DIGIT waiting after enabling the DSS,
	 * so clear it */
	dispc_write_reg(DISPC_IRQSTATUS, dispc_read_reg(DISPC_IRQSTATUS));

	_omap_dispc_set_irqs();

	spin_unlock_irqrestore(&dispc.irq_lock, flags);
}

void dispc_enable_sidle(void)
{
	REG_FLD_MOD(DISPC_SYSCONFIG, 2, 4, 3);	/* SIDLEMODE: smart idle */
}

void dispc_disable_sidle(void)
{
	REG_FLD_MOD(DISPC_SYSCONFIG, 1, 4, 3);	/* SIDLEMODE: no idle */
}

static void _omap_dispc_initial_config(void)
{
	u32 l;

<<<<<<< HEAD
	l = dispc_read_reg(DISPC_SYSCONFIG);
	l = FLD_MOD(l, 2, 13, 12);	/* MIDLEMODE: smart standby */
	l = FLD_MOD(l, 2, 4, 3);	/* SIDLEMODE: smart idle */
	l = FLD_MOD(l, 1, 2, 2);	/* ENWAKEUP */
	l = FLD_MOD(l, 1, 0, 0);	/* AUTOIDLE */
	dispc_write_reg(DISPC_SYSCONFIG, l);

=======
>>>>>>> refs/remotes/origin/cm-10.0
	/* Exclusively enable DISPC_CORE_CLK and set divider to 1 */
	if (dss_has_feature(FEAT_CORE_CLK_DIV)) {
		l = dispc_read_reg(DISPC_DIVISOR);
		/* Use DISPC_DIVISOR.LCD, instead of DISPC_DIVISOR1.LCD */
		l = FLD_MOD(l, 1, 0, 0);
		l = FLD_MOD(l, 1, 23, 16);
		dispc_write_reg(DISPC_DIVISOR, l);
	}

	/* FUNCGATED */
	if (dss_has_feature(FEAT_FUNCGATED))
		REG_FLD_MOD(DISPC_CONFIG, 1, 9, 9);

<<<<<<< HEAD
	/* L3 firewall setting: enable access to OCM RAM */
	/* XXX this should be somewhere in plat-omap */
	if (cpu_is_omap24xx())
		__raw_writel(0x402000b0, OMAP2_L3_IO_ADDRESS(0x680050a0));

=======
>>>>>>> refs/remotes/origin/cm-10.0
	_dispc_setup_color_conv_coef();

	dispc_set_loadmode(OMAP_DSS_LOAD_FRAME_ONLY);

	dispc_read_plane_fifo_sizes();
<<<<<<< HEAD
}

int dispc_enable_plane(enum omap_plane plane, bool enable)
{
	DSSDBG("dispc_enable_plane %d, %d\n", plane, enable);

	enable_clocks(1);
	_dispc_enable_plane(plane, enable);
	enable_clocks(0);

	return 0;
}

int dispc_setup_plane(enum omap_plane plane,
		       u32 paddr, u16 screen_width,
		       u16 pos_x, u16 pos_y,
		       u16 width, u16 height,
		       u16 out_width, u16 out_height,
		       enum omap_color_mode color_mode,
		       bool ilace,
		       enum omap_dss_rotation_type rotation_type,
		       u8 rotation, bool mirror, u8 global_alpha,
		       u8 pre_mult_alpha, enum omap_channel channel,
		       u32 puv_addr)
{
	int r = 0;

	DSSDBG("dispc_setup_plane %d, pa %x, sw %d, %d, %d, %dx%d -> "
	       "%dx%d, ilace %d, cmode %x, rot %d, mir %d chan %d\n",
	       plane, paddr, screen_width, pos_x, pos_y,
	       width, height,
	       out_width, out_height,
	       ilace, color_mode,
	       rotation, mirror, channel);

	enable_clocks(1);

	r = _dispc_setup_plane(plane,
			   paddr, screen_width,
			   pos_x, pos_y,
			   width, height,
			   out_width, out_height,
			   color_mode, ilace,
			   rotation_type,
			   rotation, mirror,
			   global_alpha,
			   pre_mult_alpha,
			   channel, puv_addr);

	enable_clocks(0);

	return r;
=======

	dispc_configure_burst_sizes();

	dispc_ovl_enable_zorder_planes();
>>>>>>> refs/remotes/origin/cm-10.0
}

/* DISPC HW IP initialisation */
static int omap_dispchw_probe(struct platform_device *pdev)
=======
int dispc_request_irq(irq_handler_t handler, void *dev_id)
{
	return devm_request_irq(&dispc.pdev->dev, dispc.irq, handler,
			     IRQF_SHARED, "OMAP DISPC", dev_id);
}
EXPORT_SYMBOL(dispc_request_irq);

void dispc_free_irq(void *dev_id)
{
	devm_free_irq(&dispc.pdev->dev, dispc.irq, dev_id);
}
EXPORT_SYMBOL(dispc_free_irq);

/* DISPC HW IP initialisation */
static int __init omap_dispchw_probe(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	u32 rev;
	int r = 0;
	struct resource *dispc_mem;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct clk *clk;
>>>>>>> refs/remotes/origin/cm-10.0

	dispc.pdev = pdev;

	spin_lock_init(&dispc.irq_lock);

#ifdef CONFIG_OMAP2_DSS_COLLECT_IRQ_STATS
	spin_lock_init(&dispc.irq_stats_lock);
	dispc.irq_stats.last_reset = jiffies;
#endif

	INIT_WORK(&dispc.error_work, dispc_error_worker);
=======

	dispc.pdev = pdev;

	r = dispc_init_features(dispc.pdev);
	if (r)
		return r;
>>>>>>> refs/remotes/origin/master

	dispc_mem = platform_get_resource(dispc.pdev, IORESOURCE_MEM, 0);
	if (!dispc_mem) {
		DSSERR("can't get IORESOURCE_MEM DISPC\n");
<<<<<<< HEAD
<<<<<<< HEAD
		r = -EINVAL;
		goto fail0;
	}
	dispc.base = ioremap(dispc_mem->start, resource_size(dispc_mem));
	if (!dispc.base) {
		DSSERR("can't ioremap DISPC\n");
		r = -ENOMEM;
		goto fail0;
	}
	dispc.irq = platform_get_irq(dispc.pdev, 0);
	if (dispc.irq < 0) {
		DSSERR("platform_get_irq failed\n");
		r = -ENODEV;
		goto fail1;
	}

	r = request_irq(dispc.irq, omap_dispc_irq_handler, IRQF_SHARED,
		"OMAP DISPC", dispc.pdev);
	if (r < 0) {
		DSSERR("request_irq failed\n");
		goto fail1;
	}

	enable_clocks(1);
=======
=======
>>>>>>> refs/remotes/origin/master
		return -EINVAL;
	}

	dispc.base = devm_ioremap(&pdev->dev, dispc_mem->start,
				  resource_size(dispc_mem));
	if (!dispc.base) {
		DSSERR("can't ioremap DISPC\n");
		return -ENOMEM;
	}

	dispc.irq = platform_get_irq(dispc.pdev, 0);
	if (dispc.irq < 0) {
		DSSERR("platform_get_irq failed\n");
		return -ENODEV;
	}

<<<<<<< HEAD
	r = devm_request_irq(&pdev->dev, dispc.irq, omap_dispc_irq_handler,
			     IRQF_SHARED, "OMAP DISPC", dispc.pdev);
	if (r < 0) {
		DSSERR("request_irq failed\n");
		return r;
	}

	clk = clk_get(&pdev->dev, "fck");
	if (IS_ERR(clk)) {
		DSSERR("can't get fck\n");
		r = PTR_ERR(clk);
		return r;
	}

	dispc.dss_clk = clk;

	pm_runtime_enable(&pdev->dev);
=======
	pm_runtime_enable(&pdev->dev);
	pm_runtime_irq_safe(&pdev->dev);
>>>>>>> refs/remotes/origin/master

	r = dispc_runtime_get();
	if (r)
		goto err_runtime_get;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

	_omap_dispc_initial_config();

	_omap_dispc_initialize_irq();

<<<<<<< HEAD
	dispc_save_context();

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======

	_omap_dispc_initial_config();

>>>>>>> refs/remotes/origin/master
	rev = dispc_read_reg(DISPC_REVISION);
	dev_dbg(&pdev->dev, "OMAP DISPC rev %d.%d\n",
	       FLD_GET(rev, 7, 4), FLD_GET(rev, 3, 0));

<<<<<<< HEAD
<<<<<<< HEAD
	enable_clocks(0);

	return 0;
fail1:
	iounmap(dispc.base);
fail0:
=======
	dispc_runtime_put();

=======
	dispc_runtime_put();

	dss_init_overlay_managers();

	dss_debugfs_create_file("dispc", dispc_dump_regs);

>>>>>>> refs/remotes/origin/master
	return 0;

err_runtime_get:
	pm_runtime_disable(&pdev->dev);
<<<<<<< HEAD
	clk_put(dispc.dss_clk);
>>>>>>> refs/remotes/origin/cm-10.0
	return r;
}

static int omap_dispchw_remove(struct platform_device *pdev)
{
<<<<<<< HEAD
	free_irq(dispc.irq, dispc.pdev);
	iounmap(dispc.base);
	return 0;
}

=======
	pm_runtime_disable(&pdev->dev);

	clk_put(dispc.dss_clk);
=======
	return r;
}

static int __exit omap_dispchw_remove(struct platform_device *pdev)
{
	pm_runtime_disable(&pdev->dev);

	dss_uninit_overlay_managers();
>>>>>>> refs/remotes/origin/master

	return 0;
}

static int dispc_runtime_suspend(struct device *dev)
{
	dispc_save_context();
<<<<<<< HEAD
	dss_runtime_put();
=======
>>>>>>> refs/remotes/origin/master

	return 0;
}

static int dispc_runtime_resume(struct device *dev)
{
<<<<<<< HEAD
	int r;

	r = dss_runtime_get();
	if (r < 0)
		return r;

=======
>>>>>>> refs/remotes/origin/master
	dispc_restore_context();

	return 0;
}

static const struct dev_pm_ops dispc_pm_ops = {
	.runtime_suspend = dispc_runtime_suspend,
	.runtime_resume = dispc_runtime_resume,
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
static struct platform_driver omap_dispchw_driver = {
	.probe          = omap_dispchw_probe,
	.remove         = omap_dispchw_remove,
	.driver         = {
		.name   = "omapdss_dispc",
		.owner  = THIS_MODULE,
<<<<<<< HEAD
=======
		.pm	= &dispc_pm_ops,
>>>>>>> refs/remotes/origin/cm-10.0
	},
};

int dispc_init_platform_driver(void)
{
	return platform_driver_register(&omap_dispchw_driver);
}

void dispc_uninit_platform_driver(void)
{
	return platform_driver_unregister(&omap_dispchw_driver);
=======
static struct platform_driver omap_dispchw_driver = {
	.remove         = __exit_p(omap_dispchw_remove),
	.driver         = {
		.name   = "omapdss_dispc",
		.owner  = THIS_MODULE,
		.pm	= &dispc_pm_ops,
	},
};

int __init dispc_init_platform_driver(void)
{
	return platform_driver_probe(&omap_dispchw_driver, omap_dispchw_probe);
}

void __exit dispc_uninit_platform_driver(void)
{
	platform_driver_unregister(&omap_dispchw_driver);
>>>>>>> refs/remotes/origin/master
}
