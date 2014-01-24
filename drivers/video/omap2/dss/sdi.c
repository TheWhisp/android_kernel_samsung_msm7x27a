/*
 * linux/drivers/video/omap2/dss/sdi.c
 *
 * Copyright (C) 2009 Nokia Corporation
 * Author: Tomi Valkeinen <tomi.valkeinen@nokia.com>
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

#define DSS_SUBSYS_NAME "SDI"

#include <linux/kernel.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/regulator/consumer.h>

#include <video/omapdss.h>
#include <plat/cpu.h>
=======
=======
>>>>>>> refs/remotes/origin/master
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/regulator/consumer.h>
#include <linux/export.h>
<<<<<<< HEAD

#include <video/omapdss.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include "dss.h"

static struct {
	bool update_enabled;
	struct regulator *vdds_sdi_reg;
} sdi;

static void sdi_basic_init(struct omap_dss_device *dssdev)

{
<<<<<<< HEAD
	dispc_set_parallel_interface_mode(dssdev->manager->id,
			OMAP_DSS_PARALLELMODE_BYPASS);

	dispc_set_lcd_display_type(dssdev->manager->id,
			OMAP_DSS_LCD_DISPLAY_TFT);

	dispc_set_tft_data_lines(dssdev->manager->id, 24);
=======
	dispc_mgr_set_io_pad_mode(DSS_IO_PAD_MODE_BYPASS);
	dispc_mgr_enable_stallmode(dssdev->manager->id, false);

	dispc_mgr_set_lcd_display_type(dssdev->manager->id,
			OMAP_DSS_LCD_DISPLAY_TFT);

	dispc_mgr_set_tft_data_lines(dssdev->manager->id, 24);
>>>>>>> refs/remotes/origin/cm-10.0
	dispc_lcd_enable_signal_polarity(1);
}

int omapdss_sdi_display_enable(struct omap_dss_device *dssdev)
{
	struct omap_video_timings *t = &dssdev->panel.timings;
	struct dss_clock_info dss_cinfo;
	struct dispc_clock_info dispc_cinfo;
	u16 lck_div, pck_div;
	unsigned long fck;
	unsigned long pck;
	int r;

<<<<<<< HEAD
	r = omap_dss_start_device(dssdev);
	if (r) {
		DSSERR("failed to start device\n");
		goto err0;
=======
	if (dssdev->manager == NULL) {
		DSSERR("failed to enable display: no manager\n");
		return -ENODEV;
	}

	r = omap_dss_start_device(dssdev);
	if (r) {
		DSSERR("failed to start device\n");
		goto err_start_dev;
>>>>>>> refs/remotes/origin/cm-10.0
	}

	r = regulator_enable(sdi.vdds_sdi_reg);
	if (r)
<<<<<<< HEAD
		goto err1;

	dss_clk_enable(DSS_CLK_ICK | DSS_CLK_FCK);
=======
		goto err_reg_enable;

	r = dss_runtime_get();
	if (r)
		goto err_get_dss;

	r = dispc_runtime_get();
	if (r)
		goto err_get_dispc;
>>>>>>> refs/remotes/origin/cm-10.0

	sdi_basic_init(dssdev);

	/* 15.5.9.1.2 */
	dssdev->panel.config |= OMAP_DSS_LCD_RF | OMAP_DSS_LCD_ONOFF;

<<<<<<< HEAD
	dispc_set_pol_freq(dssdev->manager->id, dssdev->panel.config,
=======
	dispc_mgr_set_pol_freq(dssdev->manager->id, dssdev->panel.config,
>>>>>>> refs/remotes/origin/cm-10.0
			dssdev->panel.acbi, dssdev->panel.acb);

	r = dss_calc_clock_div(1, t->pixel_clock * 1000,
			&dss_cinfo, &dispc_cinfo);
	if (r)
<<<<<<< HEAD
		goto err2;
=======
		goto err_calc_clock_div;
>>>>>>> refs/remotes/origin/cm-10.0

	fck = dss_cinfo.fck;
	lck_div = dispc_cinfo.lck_div;
	pck_div = dispc_cinfo.pck_div;

	pck = fck / lck_div / pck_div / 1000;
=======
#include <linux/platform_device.h>
#include <linux/string.h>

#include <video/omapdss.h>
#include "dss.h"

static struct {
	struct platform_device *pdev;

	bool update_enabled;
	struct regulator *vdds_sdi_reg;

	struct dss_lcd_mgr_config mgr_config;
	struct omap_video_timings timings;
	int datapairs;

	struct omap_dss_device output;
} sdi;

struct sdi_clk_calc_ctx {
	unsigned long pck_min, pck_max;

	struct dss_clock_info dss_cinfo;
	struct dispc_clock_info dispc_cinfo;
};

static bool dpi_calc_dispc_cb(int lckd, int pckd, unsigned long lck,
		unsigned long pck, void *data)
{
	struct sdi_clk_calc_ctx *ctx = data;

	ctx->dispc_cinfo.lck_div = lckd;
	ctx->dispc_cinfo.pck_div = pckd;
	ctx->dispc_cinfo.lck = lck;
	ctx->dispc_cinfo.pck = pck;

	return true;
}

static bool dpi_calc_dss_cb(int fckd, unsigned long fck, void *data)
{
	struct sdi_clk_calc_ctx *ctx = data;

	ctx->dss_cinfo.fck = fck;
	ctx->dss_cinfo.fck_div = fckd;

	return dispc_div_calc(fck, ctx->pck_min, ctx->pck_max,
			dpi_calc_dispc_cb, ctx);
}

static int sdi_calc_clock_div(unsigned long pclk,
		struct dss_clock_info *dss_cinfo,
		struct dispc_clock_info *dispc_cinfo)
{
	int i;
	struct sdi_clk_calc_ctx ctx;

	/*
	 * DSS fclk gives us very few possibilities, so finding a good pixel
	 * clock may not be possible. We try multiple times to find the clock,
	 * each time widening the pixel clock range we look for, up to
	 * +/- 1MHz.
	 */

	for (i = 0; i < 10; ++i) {
		bool ok;

		memset(&ctx, 0, sizeof(ctx));
		if (pclk > 1000 * i * i * i)
			ctx.pck_min = max(pclk - 1000 * i * i * i, 0lu);
		else
			ctx.pck_min = 0;
		ctx.pck_max = pclk + 1000 * i * i * i;

		ok = dss_div_calc(ctx.pck_min, dpi_calc_dss_cb, &ctx);
		if (ok) {
			*dss_cinfo = ctx.dss_cinfo;
			*dispc_cinfo = ctx.dispc_cinfo;
			return 0;
		}
	}

	return -EINVAL;
}

static void sdi_config_lcd_manager(struct omap_dss_device *dssdev)
{
	struct omap_overlay_manager *mgr = sdi.output.manager;

	sdi.mgr_config.io_pad_mode = DSS_IO_PAD_MODE_BYPASS;

	sdi.mgr_config.stallmode = false;
	sdi.mgr_config.fifohandcheck = false;

	sdi.mgr_config.video_port_width = 24;
	sdi.mgr_config.lcden_sig_polarity = 1;

	dss_mgr_set_lcd_config(mgr, &sdi.mgr_config);
}

static int sdi_display_enable(struct omap_dss_device *dssdev)
{
	struct omap_dss_device *out = &sdi.output;
	struct omap_video_timings *t = &sdi.timings;
	struct dss_clock_info dss_cinfo;
	struct dispc_clock_info dispc_cinfo;
	unsigned long pck;
	int r;

	if (out == NULL || out->manager == NULL) {
		DSSERR("failed to enable display: no output/manager\n");
		return -ENODEV;
	}

	r = regulator_enable(sdi.vdds_sdi_reg);
	if (r)
		goto err_reg_enable;

	r = dispc_runtime_get();
	if (r)
		goto err_get_dispc;

	/* 15.5.9.1.2 */
	t->data_pclk_edge = OMAPDSS_DRIVE_SIG_RISING_EDGE;
	t->sync_pclk_edge = OMAPDSS_DRIVE_SIG_RISING_EDGE;

	r = sdi_calc_clock_div(t->pixel_clock * 1000, &dss_cinfo, &dispc_cinfo);
	if (r)
		goto err_calc_clock_div;

	sdi.mgr_config.clock_info = dispc_cinfo;

	pck = dss_cinfo.fck / dispc_cinfo.lck_div / dispc_cinfo.pck_div / 1000;
>>>>>>> refs/remotes/origin/master

	if (pck != t->pixel_clock) {
		DSSWARN("Could not find exact pixel clock. Requested %d kHz, "
				"got %lu kHz\n",
				t->pixel_clock, pck);

		t->pixel_clock = pck;
	}


<<<<<<< HEAD
<<<<<<< HEAD
	dispc_set_lcd_timings(dssdev->manager->id, t);

	r = dss_set_clock_div(&dss_cinfo);
	if (r)
		goto err2;

	r = dispc_set_clock_div(dssdev->manager->id, &dispc_cinfo);
	if (r)
		goto err2;
=======
	dispc_mgr_set_lcd_timings(dssdev->manager->id, t);
=======
	dss_mgr_set_timings(out->manager, t);
>>>>>>> refs/remotes/origin/master

	r = dss_set_clock_div(&dss_cinfo);
	if (r)
		goto err_set_dss_clock_div;

<<<<<<< HEAD
	r = dispc_mgr_set_clock_div(dssdev->manager->id, &dispc_cinfo);
	if (r)
		goto err_set_dispc_clock_div;
>>>>>>> refs/remotes/origin/cm-10.0

	dss_sdi_init(dssdev->phy.sdi.datapairs);
	r = dss_sdi_enable();
	if (r)
<<<<<<< HEAD
		goto err1;
	mdelay(2);

	dssdev->manager->enable(dssdev->manager);

	return 0;
err2:
	dss_clk_disable(DSS_CLK_ICK | DSS_CLK_FCK);
	regulator_disable(sdi.vdds_sdi_reg);
err1:
	omap_dss_stop_device(dssdev);
err0:
=======
		goto err_sdi_enable;
	mdelay(2);

	r = dss_mgr_enable(dssdev->manager);
=======
	sdi_config_lcd_manager(dssdev);

	/*
	 * LCLK and PCLK divisors are located in shadow registers, and we
	 * normally write them to DISPC registers when enabling the output.
	 * However, SDI uses pck-free as source clock for its PLL, and pck-free
	 * is affected by the divisors. And as we need the PLL before enabling
	 * the output, we need to write the divisors early.
	 *
	 * It seems just writing to the DISPC register is enough, and we don't
	 * need to care about the shadow register mechanism for pck-free. The
	 * exact reason for this is unknown.
	 */
	dispc_mgr_set_clock_div(out->manager->id, &sdi.mgr_config.clock_info);

	dss_sdi_init(sdi.datapairs);
	r = dss_sdi_enable();
	if (r)
		goto err_sdi_enable;
	mdelay(2);

	r = dss_mgr_enable(out->manager);
>>>>>>> refs/remotes/origin/master
	if (r)
		goto err_mgr_enable;

	return 0;

err_mgr_enable:
	dss_sdi_disable();
err_sdi_enable:
<<<<<<< HEAD
err_set_dispc_clock_div:
=======
>>>>>>> refs/remotes/origin/master
err_set_dss_clock_div:
err_calc_clock_div:
	dispc_runtime_put();
err_get_dispc:
<<<<<<< HEAD
	dss_runtime_put();
err_get_dss:
	regulator_disable(sdi.vdds_sdi_reg);
err_reg_enable:
	omap_dss_stop_device(dssdev);
err_start_dev:
>>>>>>> refs/remotes/origin/cm-10.0
	return r;
}
EXPORT_SYMBOL(omapdss_sdi_display_enable);

void omapdss_sdi_display_disable(struct omap_dss_device *dssdev)
{
<<<<<<< HEAD
	dssdev->manager->disable(dssdev->manager);

	dss_sdi_disable();

	dss_clk_disable(DSS_CLK_ICK | DSS_CLK_FCK);
=======
	dss_mgr_disable(dssdev->manager);
=======
	regulator_disable(sdi.vdds_sdi_reg);
err_reg_enable:
	return r;
}

static void sdi_display_disable(struct omap_dss_device *dssdev)
{
	struct omap_overlay_manager *mgr = sdi.output.manager;

	dss_mgr_disable(mgr);
>>>>>>> refs/remotes/origin/master

	dss_sdi_disable();

	dispc_runtime_put();
<<<<<<< HEAD
	dss_runtime_put();
>>>>>>> refs/remotes/origin/cm-10.0

	regulator_disable(sdi.vdds_sdi_reg);

	omap_dss_stop_device(dssdev);
}
EXPORT_SYMBOL(omapdss_sdi_display_disable);

int sdi_init_display(struct omap_dss_device *dssdev)
{
	DSSDBG("SDI init\n");

	if (sdi.vdds_sdi_reg == NULL) {
		struct regulator *vdds_sdi;

		vdds_sdi = dss_get_vdds_sdi();

		if (IS_ERR(vdds_sdi)) {
			DSSERR("can't get VDDS_SDI regulator\n");
			return PTR_ERR(vdds_sdi);
		}

		sdi.vdds_sdi_reg = vdds_sdi;
	}

	return 0;
}

int sdi_init(void)
{
	return 0;
}

void sdi_exit(void)
{
=======

	regulator_disable(sdi.vdds_sdi_reg);
}

static void sdi_set_timings(struct omap_dss_device *dssdev,
		struct omap_video_timings *timings)
{
	sdi.timings = *timings;
}

static void sdi_get_timings(struct omap_dss_device *dssdev,
		struct omap_video_timings *timings)
{
	*timings = sdi.timings;
}

static int sdi_check_timings(struct omap_dss_device *dssdev,
			struct omap_video_timings *timings)
{
	struct omap_overlay_manager *mgr = sdi.output.manager;

	if (mgr && !dispc_mgr_timings_ok(mgr->id, timings))
		return -EINVAL;

	if (timings->pixel_clock == 0)
		return -EINVAL;

	return 0;
}

static void sdi_set_datapairs(struct omap_dss_device *dssdev, int datapairs)
{
	sdi.datapairs = datapairs;
}

static int sdi_init_regulator(void)
{
	struct regulator *vdds_sdi;

	if (sdi.vdds_sdi_reg)
		return 0;

	vdds_sdi = devm_regulator_get(&sdi.pdev->dev, "vdds_sdi");
	if (IS_ERR(vdds_sdi)) {
		DSSERR("can't get VDDS_SDI regulator\n");
		return PTR_ERR(vdds_sdi);
	}

	sdi.vdds_sdi_reg = vdds_sdi;

	return 0;
}

static int sdi_connect(struct omap_dss_device *dssdev,
		struct omap_dss_device *dst)
{
	struct omap_overlay_manager *mgr;
	int r;

	r = sdi_init_regulator();
	if (r)
		return r;

	mgr = omap_dss_get_overlay_manager(dssdev->dispc_channel);
	if (!mgr)
		return -ENODEV;

	r = dss_mgr_connect(mgr, dssdev);
	if (r)
		return r;

	r = omapdss_output_set_device(dssdev, dst);
	if (r) {
		DSSERR("failed to connect output to new device: %s\n",
				dst->name);
		dss_mgr_disconnect(mgr, dssdev);
		return r;
	}

	return 0;
}

static void sdi_disconnect(struct omap_dss_device *dssdev,
		struct omap_dss_device *dst)
{
	WARN_ON(dst != dssdev->dst);

	if (dst != dssdev->dst)
		return;

	omapdss_output_unset_device(dssdev);

	if (dssdev->manager)
		dss_mgr_disconnect(dssdev->manager, dssdev);
}

static const struct omapdss_sdi_ops sdi_ops = {
	.connect = sdi_connect,
	.disconnect = sdi_disconnect,

	.enable = sdi_display_enable,
	.disable = sdi_display_disable,

	.check_timings = sdi_check_timings,
	.set_timings = sdi_set_timings,
	.get_timings = sdi_get_timings,

	.set_datapairs = sdi_set_datapairs,
};

static void sdi_init_output(struct platform_device *pdev)
{
	struct omap_dss_device *out = &sdi.output;

	out->dev = &pdev->dev;
	out->id = OMAP_DSS_OUTPUT_SDI;
	out->output_type = OMAP_DISPLAY_TYPE_SDI;
	out->name = "sdi.0";
	out->dispc_channel = OMAP_DSS_CHANNEL_LCD;
	out->ops.sdi = &sdi_ops;
	out->owner = THIS_MODULE;

	omapdss_register_output(out);
}

static void __exit sdi_uninit_output(struct platform_device *pdev)
{
	struct omap_dss_device *out = &sdi.output;

	omapdss_unregister_output(out);
}

static int omap_sdi_probe(struct platform_device *pdev)
{
	sdi.pdev = pdev;

	sdi_init_output(pdev);

	return 0;
}

static int __exit omap_sdi_remove(struct platform_device *pdev)
{
	sdi_uninit_output(pdev);

	return 0;
}

static struct platform_driver omap_sdi_driver = {
	.probe		= omap_sdi_probe,
	.remove         = __exit_p(omap_sdi_remove),
	.driver         = {
		.name   = "omapdss_sdi",
		.owner  = THIS_MODULE,
	},
};

int __init sdi_init_platform_driver(void)
{
	return platform_driver_register(&omap_sdi_driver);
}

void __exit sdi_uninit_platform_driver(void)
{
	platform_driver_unregister(&omap_sdi_driver);
>>>>>>> refs/remotes/origin/master
}
