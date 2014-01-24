/**************************************************************************
 * Copyright (c) 2011, Intel Corporation.
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *
 **************************************************************************/

#include "psb_drv.h"
#include "mid_bios.h"
#include "mdfld_output.h"
#include "mdfld_dsi_output.h"
#include "tc35876x-dsi-lvds.h"

#include <asm/intel_scu_ipc.h>

#ifdef CONFIG_BACKLIGHT_CLASS_DEVICE

#define MRST_BLC_MAX_PWM_REG_FREQ	    0xFFFF
#define BLC_PWM_PRECISION_FACTOR 100	/* 10000000 */
#define BLC_PWM_FREQ_CALC_CONSTANT 32
#define MHz 1000000
#define BRIGHTNESS_MIN_LEVEL 1
#define BRIGHTNESS_MAX_LEVEL 100
#define BRIGHTNESS_MASK	0xFF
#define BLC_POLARITY_NORMAL 0
#define BLC_POLARITY_INVERSE 1
#define BLC_ADJUSTMENT_MAX 100

#define MDFLD_BLC_PWM_PRECISION_FACTOR    10
#define MDFLD_BLC_MAX_PWM_REG_FREQ        0xFFFE
#define MDFLD_BLC_MIN_PWM_REG_FREQ        0x2

#define MDFLD_BACKLIGHT_PWM_POLARITY_BIT_CLEAR (0xFFFE)
#define MDFLD_BACKLIGHT_PWM_CTL_SHIFT	(16)

static struct backlight_device *mdfld_backlight_device;

int mdfld_set_brightness(struct backlight_device *bd)
{
	struct drm_device *dev =
		(struct drm_device *)bl_get_data(mdfld_backlight_device);
	struct drm_psb_private *dev_priv = dev->dev_private;
	int level = bd->props.brightness;

	DRM_DEBUG_DRIVER("backlight level set to %d\n", level);

	/* Perform value bounds checking */
	if (level < BRIGHTNESS_MIN_LEVEL)
		level = BRIGHTNESS_MIN_LEVEL;

	if (gma_power_begin(dev, false)) {
		u32 adjusted_level = 0;

		/*
		 * Adjust the backlight level with the percent in
		 * dev_priv->blc_adj2
		 */
		adjusted_level = level * dev_priv->blc_adj2;
		adjusted_level = adjusted_level / BLC_ADJUSTMENT_MAX;
		dev_priv->brightness_adjusted = adjusted_level;

		if (mdfld_get_panel_type(dev, 0) == TC35876X) {
			if (dev_priv->dpi_panel_on[0] ||
					dev_priv->dpi_panel_on[2])
				tc35876x_brightness_control(dev,
						dev_priv->brightness_adjusted);
		} else {
			if (dev_priv->dpi_panel_on[0])
				mdfld_dsi_brightness_control(dev, 0,
						dev_priv->brightness_adjusted);
		}

		if (dev_priv->dpi_panel_on[2])
			mdfld_dsi_brightness_control(dev, 2,
					dev_priv->brightness_adjusted);
		gma_power_end(dev);
	}

	/* cache the brightness for later use */
	dev_priv->brightness = level;
	return 0;
}

static int mdfld_get_brightness(struct backlight_device *bd)
{
	struct drm_device *dev =
		(struct drm_device *)bl_get_data(mdfld_backlight_device);
	struct drm_psb_private *dev_priv = dev->dev_private;

	DRM_DEBUG_DRIVER("brightness = 0x%x \n", dev_priv->brightness);

	/* return locally cached var instead of HW read (due to DPST etc.) */
	return dev_priv->brightness;
}

static const struct backlight_ops mdfld_ops = {
	.get_brightness = mdfld_get_brightness,
	.update_status  = mdfld_set_brightness,
};

static int device_backlight_init(struct drm_device *dev)
{
	struct drm_psb_private *dev_priv = (struct drm_psb_private *)
		dev->dev_private;

	dev_priv->blc_adj1 = BLC_ADJUSTMENT_MAX;
	dev_priv->blc_adj2 = BLC_ADJUSTMENT_MAX;

	return 0;
}

static int mdfld_backlight_init(struct drm_device *dev)
{
	struct backlight_properties props;
	int ret = 0;

	memset(&props, 0, sizeof(struct backlight_properties));
	props.max_brightness = BRIGHTNESS_MAX_LEVEL;
	props.type = BACKLIGHT_PLATFORM;
	mdfld_backlight_device = backlight_device_register("mdfld-bl",
				NULL, (void *)dev, &mdfld_ops, &props);

	if (IS_ERR(mdfld_backlight_device))
		return PTR_ERR(mdfld_backlight_device);

	ret = device_backlight_init(dev);
	if (ret)
		return ret;

	mdfld_backlight_device->props.brightness = BRIGHTNESS_MAX_LEVEL;
	mdfld_backlight_device->props.max_brightness = BRIGHTNESS_MAX_LEVEL;
	backlight_update_status(mdfld_backlight_device);
	return 0;
}
#endif

struct backlight_device *mdfld_get_backlight_device(void)
{
#ifdef CONFIG_BACKLIGHT_CLASS_DEVICE
	return mdfld_backlight_device;
#else
	return NULL;
#endif
}

/*
 * mdfld_save_display_registers
 *
 * Description: We are going to suspend so save current display
 * register state.
 *
 * Notes: FIXME_JLIU7 need to add the support for DPI MIPI & HDMI audio
 */
<<<<<<< HEAD
static int mdfld_save_display_registers(struct drm_device *dev, int pipe)
{
	struct drm_psb_private *dev_priv = dev->dev_private;
	struct medfield_state *regs = &dev_priv->regs.mdfld;
	int i;

	/* register */
	u32 dpll_reg = MRST_DPLL_A;
	u32 fp_reg = MRST_FPA0;
	u32 pipeconf_reg = PIPEACONF;
	u32 htot_reg = HTOTAL_A;
	u32 hblank_reg = HBLANK_A;
	u32 hsync_reg = HSYNC_A;
	u32 vtot_reg = VTOTAL_A;
	u32 vblank_reg = VBLANK_A;
	u32 vsync_reg = VSYNC_A;
	u32 pipesrc_reg = PIPEASRC;
	u32 dspstride_reg = DSPASTRIDE;
	u32 dsplinoff_reg = DSPALINOFF;
	u32 dsptileoff_reg = DSPATILEOFF;
	u32 dspsize_reg = DSPASIZE;
	u32 dsppos_reg = DSPAPOS;
	u32 dspsurf_reg = DSPASURF;
	u32 mipi_reg = MIPI;
	u32 dspcntr_reg = DSPACNTR;
	u32 dspstatus_reg = PIPEASTAT;
	u32 palette_reg = PALETTE_A;

	/* pointer to values */
	u32 *dpll_val = &regs->saveDPLL_A;
	u32 *fp_val = &regs->saveFPA0;
	u32 *pipeconf_val = &regs->savePIPEACONF;
	u32 *htot_val = &regs->saveHTOTAL_A;
	u32 *hblank_val = &regs->saveHBLANK_A;
	u32 *hsync_val = &regs->saveHSYNC_A;
	u32 *vtot_val = &regs->saveVTOTAL_A;
	u32 *vblank_val = &regs->saveVBLANK_A;
	u32 *vsync_val = &regs->saveVSYNC_A;
	u32 *pipesrc_val = &regs->savePIPEASRC;
	u32 *dspstride_val = &regs->saveDSPASTRIDE;
	u32 *dsplinoff_val = &regs->saveDSPALINOFF;
	u32 *dsptileoff_val = &regs->saveDSPATILEOFF;
	u32 *dspsize_val = &regs->saveDSPASIZE;
	u32 *dsppos_val = &regs->saveDSPAPOS;
	u32 *dspsurf_val = &regs->saveDSPASURF;
	u32 *mipi_val = &regs->saveMIPI;
	u32 *dspcntr_val = &regs->saveDSPACNTR;
	u32 *dspstatus_val = &regs->saveDSPASTATUS;
	u32 *palette_val = regs->save_palette_a;

	switch (pipe) {
	case 0:
		break;
	case 1:
		/* regester */
		dpll_reg = MDFLD_DPLL_B;
		fp_reg = MDFLD_DPLL_DIV0;
		pipeconf_reg = PIPEBCONF;
		htot_reg = HTOTAL_B;
		hblank_reg = HBLANK_B;
		hsync_reg = HSYNC_B;
		vtot_reg = VTOTAL_B;
		vblank_reg = VBLANK_B;
		vsync_reg = VSYNC_B;
		pipesrc_reg = PIPEBSRC;
		dspstride_reg = DSPBSTRIDE;
		dsplinoff_reg = DSPBLINOFF;
		dsptileoff_reg = DSPBTILEOFF;
		dspsize_reg = DSPBSIZE;
		dsppos_reg = DSPBPOS;
		dspsurf_reg = DSPBSURF;
		dspcntr_reg = DSPBCNTR;
		dspstatus_reg = PIPEBSTAT;
		palette_reg = PALETTE_B;

		/* values */
		dpll_val = &regs->saveDPLL_B;
		fp_val = &regs->saveFPB0;
		pipeconf_val = &regs->savePIPEBCONF;
		htot_val = &regs->saveHTOTAL_B;
		hblank_val = &regs->saveHBLANK_B;
		hsync_val = &regs->saveHSYNC_B;
		vtot_val = &regs->saveVTOTAL_B;
		vblank_val = &regs->saveVBLANK_B;
		vsync_val = &regs->saveVSYNC_B;
		pipesrc_val = &regs->savePIPEBSRC;
		dspstride_val = &regs->saveDSPBSTRIDE;
		dsplinoff_val = &regs->saveDSPBLINOFF;
		dsptileoff_val = &regs->saveDSPBTILEOFF;
		dspsize_val = &regs->saveDSPBSIZE;
		dsppos_val = &regs->saveDSPBPOS;
		dspsurf_val = &regs->saveDSPBSURF;
		dspcntr_val = &regs->saveDSPBCNTR;
		dspstatus_val = &regs->saveDSPBSTATUS;
		palette_val = regs->save_palette_b;
		break;
	case 2:
		/* register */
		pipeconf_reg = PIPECCONF;
		htot_reg = HTOTAL_C;
		hblank_reg = HBLANK_C;
		hsync_reg = HSYNC_C;
		vtot_reg = VTOTAL_C;
		vblank_reg = VBLANK_C;
		vsync_reg = VSYNC_C;
		pipesrc_reg = PIPECSRC;
		dspstride_reg = DSPCSTRIDE;
		dsplinoff_reg = DSPCLINOFF;
		dsptileoff_reg = DSPCTILEOFF;
		dspsize_reg = DSPCSIZE;
		dsppos_reg = DSPCPOS;
		dspsurf_reg = DSPCSURF;
		mipi_reg = MIPI_C;
		dspcntr_reg = DSPCCNTR;
		dspstatus_reg = PIPECSTAT;
		palette_reg = PALETTE_C;

		/* pointer to values */
		pipeconf_val = &regs->savePIPECCONF;
		htot_val = &regs->saveHTOTAL_C;
		hblank_val = &regs->saveHBLANK_C;
		hsync_val = &regs->saveHSYNC_C;
		vtot_val = &regs->saveVTOTAL_C;
		vblank_val = &regs->saveVBLANK_C;
		vsync_val = &regs->saveVSYNC_C;
		pipesrc_val = &regs->savePIPECSRC;
		dspstride_val = &regs->saveDSPCSTRIDE;
		dsplinoff_val = &regs->saveDSPCLINOFF;
		dsptileoff_val = &regs->saveDSPCTILEOFF;
		dspsize_val = &regs->saveDSPCSIZE;
		dsppos_val = &regs->saveDSPCPOS;
		dspsurf_val = &regs->saveDSPCSURF;
		mipi_val = &regs->saveMIPI_C;
		dspcntr_val = &regs->saveDSPCCNTR;
		dspstatus_val = &regs->saveDSPCSTATUS;
		palette_val = regs->save_palette_c;
=======
static int mdfld_save_display_registers(struct drm_device *dev, int pipenum)
{
	struct drm_psb_private *dev_priv = dev->dev_private;
	struct medfield_state *regs = &dev_priv->regs.mdfld;
	struct psb_pipe *pipe = &dev_priv->regs.pipe[pipenum];
	const struct psb_offset *map = &dev_priv->regmap[pipenum];
	int i;
	u32 *mipi_val;

	/* register */
	u32 mipi_reg = MIPI;

	switch (pipenum) {
	case 0:
		mipi_val = &regs->saveMIPI;
		break;
	case 1:
		mipi_val = &regs->saveMIPI;
		break;
	case 2:
		/* register */
		mipi_reg = MIPI_C;
		/* pointer to values */
		mipi_val = &regs->saveMIPI_C;
>>>>>>> refs/remotes/origin/master
		break;
	default:
		DRM_ERROR("%s, invalid pipe number.\n", __func__);
		return -EINVAL;
	}

	/* Pipe & plane A info */
<<<<<<< HEAD
	*dpll_val = PSB_RVDC32(dpll_reg);
	*fp_val = PSB_RVDC32(fp_reg);
	*pipeconf_val = PSB_RVDC32(pipeconf_reg);
	*htot_val = PSB_RVDC32(htot_reg);
	*hblank_val = PSB_RVDC32(hblank_reg);
	*hsync_val = PSB_RVDC32(hsync_reg);
	*vtot_val = PSB_RVDC32(vtot_reg);
	*vblank_val = PSB_RVDC32(vblank_reg);
	*vsync_val = PSB_RVDC32(vsync_reg);
	*pipesrc_val = PSB_RVDC32(pipesrc_reg);
	*dspstride_val = PSB_RVDC32(dspstride_reg);
	*dsplinoff_val = PSB_RVDC32(dsplinoff_reg);
	*dsptileoff_val = PSB_RVDC32(dsptileoff_reg);
	*dspsize_val = PSB_RVDC32(dspsize_reg);
	*dsppos_val = PSB_RVDC32(dsppos_reg);
	*dspsurf_val = PSB_RVDC32(dspsurf_reg);
	*dspcntr_val = PSB_RVDC32(dspcntr_reg);
	*dspstatus_val = PSB_RVDC32(dspstatus_reg);

	/*save palette (gamma) */
	for (i = 0; i < 256; i++)
		palette_val[i] = PSB_RVDC32(palette_reg + (i << 2));

	if (pipe == 1) {
=======
	pipe->dpll = PSB_RVDC32(map->dpll);
	pipe->fp0 = PSB_RVDC32(map->fp0);
	pipe->conf = PSB_RVDC32(map->conf);
	pipe->htotal = PSB_RVDC32(map->htotal);
	pipe->hblank = PSB_RVDC32(map->hblank);
	pipe->hsync = PSB_RVDC32(map->hsync);
	pipe->vtotal = PSB_RVDC32(map->vtotal);
	pipe->vblank = PSB_RVDC32(map->vblank);
	pipe->vsync = PSB_RVDC32(map->vsync);
	pipe->src = PSB_RVDC32(map->src);
	pipe->stride = PSB_RVDC32(map->stride);
	pipe->linoff = PSB_RVDC32(map->linoff);
	pipe->tileoff = PSB_RVDC32(map->tileoff);
	pipe->size = PSB_RVDC32(map->size);
	pipe->pos = PSB_RVDC32(map->pos);
	pipe->surf = PSB_RVDC32(map->surf);
	pipe->cntr = PSB_RVDC32(map->cntr);
	pipe->status = PSB_RVDC32(map->status);

	/*save palette (gamma) */
	for (i = 0; i < 256; i++)
		pipe->palette[i] = PSB_RVDC32(map->palette + (i << 2));

	if (pipenum == 1) {
>>>>>>> refs/remotes/origin/master
		regs->savePFIT_CONTROL = PSB_RVDC32(PFIT_CONTROL);
		regs->savePFIT_PGM_RATIOS = PSB_RVDC32(PFIT_PGM_RATIOS);

		regs->saveHDMIPHYMISCCTL = PSB_RVDC32(HDMIPHYMISCCTL);
		regs->saveHDMIB_CONTROL = PSB_RVDC32(HDMIB_CONTROL);
		return 0;
	}

	*mipi_val = PSB_RVDC32(mipi_reg);
	return 0;
}

/*
 * mdfld_restore_display_registers
 *
 * Description: We are going to resume so restore display register state.
 *
 * Notes: FIXME_JLIU7 need to add the support for DPI MIPI & HDMI audio
 */
<<<<<<< HEAD
static int mdfld_restore_display_registers(struct drm_device *dev, int pipe)
=======
static int mdfld_restore_display_registers(struct drm_device *dev, int pipenum)
>>>>>>> refs/remotes/origin/master
{
	/* To get  panel out of ULPS mode. */
	u32 temp = 0;
	u32 device_ready_reg = DEVICE_READY_REG;
	struct drm_psb_private *dev_priv = dev->dev_private;
	struct mdfld_dsi_config *dsi_config = NULL;
	struct medfield_state *regs = &dev_priv->regs.mdfld;
<<<<<<< HEAD
	u32 i = 0;
	u32 dpll = 0;
	u32 timeout = 0;

	/* regester */
	u32 dpll_reg = MRST_DPLL_A;
	u32 fp_reg = MRST_FPA0;
	u32 pipeconf_reg = PIPEACONF;
	u32 htot_reg = HTOTAL_A;
	u32 hblank_reg = HBLANK_A;
	u32 hsync_reg = HSYNC_A;
	u32 vtot_reg = VTOTAL_A;
	u32 vblank_reg = VBLANK_A;
	u32 vsync_reg = VSYNC_A;
	u32 pipesrc_reg = PIPEASRC;
	u32 dspstride_reg = DSPASTRIDE;
	u32 dsplinoff_reg = DSPALINOFF;
	u32 dsptileoff_reg = DSPATILEOFF;
	u32 dspsize_reg = DSPASIZE;
	u32 dsppos_reg = DSPAPOS;
	u32 dspsurf_reg = DSPASURF;
	u32 dspstatus_reg = PIPEASTAT;
	u32 mipi_reg = MIPI;
	u32 dspcntr_reg = DSPACNTR;
	u32 palette_reg = PALETTE_A;

	/* values */
	u32 dpll_val = regs->saveDPLL_A & ~DPLL_VCO_ENABLE;
	u32 fp_val = regs->saveFPA0;
	u32 pipeconf_val = regs->savePIPEACONF;
	u32 htot_val = regs->saveHTOTAL_A;
	u32 hblank_val = regs->saveHBLANK_A;
	u32 hsync_val = regs->saveHSYNC_A;
	u32 vtot_val = regs->saveVTOTAL_A;
	u32 vblank_val = regs->saveVBLANK_A;
	u32 vsync_val = regs->saveVSYNC_A;
	u32 pipesrc_val = regs->savePIPEASRC;
	u32 dspstride_val = regs->saveDSPASTRIDE;
	u32 dsplinoff_val = regs->saveDSPALINOFF;
	u32 dsptileoff_val = regs->saveDSPATILEOFF;
	u32 dspsize_val = regs->saveDSPASIZE;
	u32 dsppos_val = regs->saveDSPAPOS;
	u32 dspsurf_val = regs->saveDSPASURF;
	u32 dspstatus_val = regs->saveDSPASTATUS;
	u32 mipi_val = regs->saveMIPI;
	u32 dspcntr_val = regs->saveDSPACNTR;
	u32 *palette_val = regs->save_palette_a;

	switch (pipe) {
	case 0:
		dsi_config = dev_priv->dsi_configs[0];
		break;
	case 1:
		/* regester */
		dpll_reg = MDFLD_DPLL_B;
		fp_reg = MDFLD_DPLL_DIV0;
		pipeconf_reg = PIPEBCONF;
		htot_reg = HTOTAL_B;
		hblank_reg = HBLANK_B;
		hsync_reg = HSYNC_B;
		vtot_reg = VTOTAL_B;
		vblank_reg = VBLANK_B;
		vsync_reg = VSYNC_B;
		pipesrc_reg = PIPEBSRC;
		dspstride_reg = DSPBSTRIDE;
		dsplinoff_reg = DSPBLINOFF;
		dsptileoff_reg = DSPBTILEOFF;
		dspsize_reg = DSPBSIZE;
		dsppos_reg = DSPBPOS;
		dspsurf_reg = DSPBSURF;
		dspcntr_reg = DSPBCNTR;
		dspstatus_reg = PIPEBSTAT;
		palette_reg = PALETTE_B;

		/* values */
		dpll_val = regs->saveDPLL_B & ~DPLL_VCO_ENABLE;
		fp_val = regs->saveFPB0;
		pipeconf_val = regs->savePIPEBCONF;
		htot_val = regs->saveHTOTAL_B;
		hblank_val = regs->saveHBLANK_B;
		hsync_val = regs->saveHSYNC_B;
		vtot_val = regs->saveVTOTAL_B;
		vblank_val = regs->saveVBLANK_B;
		vsync_val = regs->saveVSYNC_B;
		pipesrc_val = regs->savePIPEBSRC;
		dspstride_val = regs->saveDSPBSTRIDE;
		dsplinoff_val = regs->saveDSPBLINOFF;
		dsptileoff_val = regs->saveDSPBTILEOFF;
		dspsize_val = regs->saveDSPBSIZE;
		dsppos_val = regs->saveDSPBPOS;
		dspsurf_val = regs->saveDSPBSURF;
		dspcntr_val = regs->saveDSPBCNTR;
		dspstatus_val = regs->saveDSPBSTATUS;
		palette_val = regs->save_palette_b;
		break;
	case 2:
		/* regester */
		pipeconf_reg = PIPECCONF;
		htot_reg = HTOTAL_C;
		hblank_reg = HBLANK_C;
		hsync_reg = HSYNC_C;
		vtot_reg = VTOTAL_C;
		vblank_reg = VBLANK_C;
		vsync_reg = VSYNC_C;
		pipesrc_reg = PIPECSRC;
		dspstride_reg = DSPCSTRIDE;
		dsplinoff_reg = DSPCLINOFF;
		dsptileoff_reg = DSPCTILEOFF;
		dspsize_reg = DSPCSIZE;
		dsppos_reg = DSPCPOS;
		dspsurf_reg = DSPCSURF;
		mipi_reg = MIPI_C;
		dspcntr_reg = DSPCCNTR;
		dspstatus_reg = PIPECSTAT;
		palette_reg = PALETTE_C;

		/* values */
		pipeconf_val = regs->savePIPECCONF;
		htot_val = regs->saveHTOTAL_C;
		hblank_val = regs->saveHBLANK_C;
		hsync_val = regs->saveHSYNC_C;
		vtot_val = regs->saveVTOTAL_C;
		vblank_val = regs->saveVBLANK_C;
		vsync_val = regs->saveVSYNC_C;
		pipesrc_val = regs->savePIPECSRC;
		dspstride_val = regs->saveDSPCSTRIDE;
		dsplinoff_val = regs->saveDSPCLINOFF;
		dsptileoff_val = regs->saveDSPCTILEOFF;
		dspsize_val = regs->saveDSPCSIZE;
		dsppos_val = regs->saveDSPCPOS;
		dspsurf_val = regs->saveDSPCSURF;
		mipi_val = regs->saveMIPI_C;
		dspcntr_val = regs->saveDSPCCNTR;
		dspstatus_val = regs->saveDSPCSTATUS;
		palette_val = regs->save_palette_c;

=======
	struct psb_pipe *pipe = &dev_priv->regs.pipe[pipenum];
	const struct psb_offset *map = &dev_priv->regmap[pipenum];
	u32 i;
	u32 dpll;
	u32 timeout = 0;

	/* register */
	u32 mipi_reg = MIPI;

	/* values */
	u32 dpll_val = pipe->dpll;
	u32 mipi_val = regs->saveMIPI;

	switch (pipenum) {
	case 0:
		dpll_val &= ~DPLL_VCO_ENABLE;
		dsi_config = dev_priv->dsi_configs[0];
		break;
	case 1:
		dpll_val &= ~DPLL_VCO_ENABLE;
		break;
	case 2:
		mipi_reg = MIPI_C;
		mipi_val = regs->saveMIPI_C;
>>>>>>> refs/remotes/origin/master
		dsi_config = dev_priv->dsi_configs[1];
		break;
	default:
		DRM_ERROR("%s, invalid pipe number.\n", __func__);
		return -EINVAL;
	}

	/*make sure VGA plane is off. it initializes to on after reset!*/
	PSB_WVDC32(0x80000000, VGACNTRL);

<<<<<<< HEAD
	if (pipe == 1) {
		PSB_WVDC32(dpll_val & ~DPLL_VCO_ENABLE, dpll_reg);
		PSB_RVDC32(dpll_reg);

		PSB_WVDC32(fp_val, fp_reg);
	} else {

		dpll = PSB_RVDC32(dpll_reg);
=======
	if (pipenum == 1) {
		PSB_WVDC32(dpll_val & ~DPLL_VCO_ENABLE, map->dpll);
		PSB_RVDC32(map->dpll);

		PSB_WVDC32(pipe->fp0, map->fp0);
	} else {

		dpll = PSB_RVDC32(map->dpll);
>>>>>>> refs/remotes/origin/master

		if (!(dpll & DPLL_VCO_ENABLE)) {

			/* When ungating power of DPLL, needs to wait 0.5us
			   before enable the VCO */
			if (dpll & MDFLD_PWR_GATE_EN) {
				dpll &= ~MDFLD_PWR_GATE_EN;
<<<<<<< HEAD
				PSB_WVDC32(dpll, dpll_reg);
=======
				PSB_WVDC32(dpll, map->dpll);
>>>>>>> refs/remotes/origin/master
				/* FIXME_MDFLD PO - change 500 to 1 after PO */
				udelay(500);
			}

<<<<<<< HEAD
			PSB_WVDC32(fp_val, fp_reg);
			PSB_WVDC32(dpll_val, dpll_reg);
=======
			PSB_WVDC32(pipe->fp0, map->fp0);
			PSB_WVDC32(dpll_val, map->dpll);
>>>>>>> refs/remotes/origin/master
			/* FIXME_MDFLD PO - change 500 to 1 after PO */
			udelay(500);

			dpll_val |= DPLL_VCO_ENABLE;
<<<<<<< HEAD
			PSB_WVDC32(dpll_val, dpll_reg);
			PSB_RVDC32(dpll_reg);

			/* wait for DSI PLL to lock */
			while (timeout < 20000 &&
			  !(PSB_RVDC32(pipeconf_reg) & PIPECONF_DSIPLL_LOCK)) {
=======
			PSB_WVDC32(dpll_val, map->dpll);
			PSB_RVDC32(map->dpll);

			/* wait for DSI PLL to lock */
			while (timeout < 20000 &&
			  !(PSB_RVDC32(map->conf) & PIPECONF_DSIPLL_LOCK)) {
>>>>>>> refs/remotes/origin/master
				udelay(150);
				timeout++;
			}

			if (timeout == 20000) {
				DRM_ERROR("%s, can't lock DSIPLL.\n",
								__func__);
				return -EINVAL;
			}
		}
	}
	/* Restore mode */
<<<<<<< HEAD
	PSB_WVDC32(htot_val, htot_reg);
	PSB_WVDC32(hblank_val, hblank_reg);
	PSB_WVDC32(hsync_val, hsync_reg);
	PSB_WVDC32(vtot_val, vtot_reg);
	PSB_WVDC32(vblank_val, vblank_reg);
	PSB_WVDC32(vsync_val, vsync_reg);
	PSB_WVDC32(pipesrc_val, pipesrc_reg);
	PSB_WVDC32(dspstatus_val, dspstatus_reg);

	/*set up the plane*/
	PSB_WVDC32(dspstride_val, dspstride_reg);
	PSB_WVDC32(dsplinoff_val, dsplinoff_reg);
	PSB_WVDC32(dsptileoff_val, dsptileoff_reg);
	PSB_WVDC32(dspsize_val, dspsize_reg);
	PSB_WVDC32(dsppos_val, dsppos_reg);
	PSB_WVDC32(dspsurf_val, dspsurf_reg);

	if (pipe == 1) {
		/* restore palette (gamma) */
		/*DRM_UDELAY(50000); */
		for (i = 0; i < 256; i++)
			PSB_WVDC32(palette_val[i], palette_reg + (i << 2));
=======
	PSB_WVDC32(pipe->htotal, map->htotal);
	PSB_WVDC32(pipe->hblank, map->hblank);
	PSB_WVDC32(pipe->hsync, map->hsync);
	PSB_WVDC32(pipe->vtotal, map->vtotal);
	PSB_WVDC32(pipe->vblank, map->vblank);
	PSB_WVDC32(pipe->vsync, map->vsync);
	PSB_WVDC32(pipe->src, map->src);
	PSB_WVDC32(pipe->status, map->status);

	/*set up the plane*/
	PSB_WVDC32(pipe->stride, map->stride);
	PSB_WVDC32(pipe->linoff, map->linoff);
	PSB_WVDC32(pipe->tileoff, map->tileoff);
	PSB_WVDC32(pipe->size, map->size);
	PSB_WVDC32(pipe->pos, map->pos);
	PSB_WVDC32(pipe->surf, map->surf);

	if (pipenum == 1) {
		/* restore palette (gamma) */
		/*DRM_UDELAY(50000); */
		for (i = 0; i < 256; i++)
			PSB_WVDC32(pipe->palette[i], map->palette + (i << 2));
>>>>>>> refs/remotes/origin/master

		PSB_WVDC32(regs->savePFIT_CONTROL, PFIT_CONTROL);
		PSB_WVDC32(regs->savePFIT_PGM_RATIOS, PFIT_PGM_RATIOS);

		/*TODO: resume HDMI port */

		/*TODO: resume pipe*/

		/*enable the plane*/
<<<<<<< HEAD
		PSB_WVDC32(dspcntr_val & ~DISPLAY_PLANE_ENABLE, dspcntr_reg);
=======
		PSB_WVDC32(pipe->cntr & ~DISPLAY_PLANE_ENABLE, map->cntr);
>>>>>>> refs/remotes/origin/master

		return 0;
	}

	/*set up pipe related registers*/
	PSB_WVDC32(mipi_val, mipi_reg);

	/*setup MIPI adapter + MIPI IP registers*/
	if (dsi_config)
<<<<<<< HEAD
		mdfld_dsi_controller_init(dsi_config, pipe);
=======
		mdfld_dsi_controller_init(dsi_config, pipenum);
>>>>>>> refs/remotes/origin/master

	if (in_atomic() || in_interrupt())
		mdelay(20);
	else
		msleep(20);

	/*enable the plane*/
<<<<<<< HEAD
	PSB_WVDC32(dspcntr_val, dspcntr_reg);
=======
	PSB_WVDC32(pipe->cntr, map->cntr);
>>>>>>> refs/remotes/origin/master

	if (in_atomic() || in_interrupt())
		mdelay(20);
	else
		msleep(20);

	/* LP Hold Release */
	temp = REG_READ(mipi_reg);
	temp |= LP_OUTPUT_HOLD_RELEASE;
	REG_WRITE(mipi_reg, temp);
	mdelay(1);


	/* Set DSI host to exit from Utra Low Power State */
	temp = REG_READ(device_ready_reg);
	temp &= ~ULPS_MASK;
	temp |= 0x3;
	temp |= EXIT_ULPS_DEV_READY;
	REG_WRITE(device_ready_reg, temp);
	mdelay(1);

	temp = REG_READ(device_ready_reg);
	temp &= ~ULPS_MASK;
	temp |= EXITING_ULPS;
	REG_WRITE(device_ready_reg, temp);
	mdelay(1);

	/*enable the pipe*/
<<<<<<< HEAD
	PSB_WVDC32(pipeconf_val, pipeconf_reg);
=======
	PSB_WVDC32(pipe->conf, map->conf);
>>>>>>> refs/remotes/origin/master

	/* restore palette (gamma) */
	/*DRM_UDELAY(50000); */
	for (i = 0; i < 256; i++)
<<<<<<< HEAD
		PSB_WVDC32(palette_val[i], palette_reg + (i << 2));
=======
		PSB_WVDC32(pipe->palette[i], map->palette + (i << 2));
>>>>>>> refs/remotes/origin/master

	return 0;
}

static int mdfld_save_registers(struct drm_device *dev)
{
	/* mdfld_save_cursor_overlay_registers(dev); */
	mdfld_save_display_registers(dev, 0);
	mdfld_save_display_registers(dev, 2);
	mdfld_disable_crtc(dev, 0);
	mdfld_disable_crtc(dev, 2);

	return 0;
}

static int mdfld_restore_registers(struct drm_device *dev)
{
	mdfld_restore_display_registers(dev, 2);
	mdfld_restore_display_registers(dev, 0);
	/* mdfld_restore_cursor_overlay_registers(dev); */

	return 0;
}

static int mdfld_power_down(struct drm_device *dev)
{
	/* FIXME */
	return 0;
}

static int mdfld_power_up(struct drm_device *dev)
{
	/* FIXME */
	return 0;
}

<<<<<<< HEAD
=======
/* Medfield  */
static const struct psb_offset mdfld_regmap[3] = {
	{
		.fp0 = MRST_FPA0,
		.fp1 = MRST_FPA1,
		.cntr = DSPACNTR,
		.conf = PIPEACONF,
		.src = PIPEASRC,
		.dpll = MRST_DPLL_A,
		.htotal = HTOTAL_A,
		.hblank = HBLANK_A,
		.hsync = HSYNC_A,
		.vtotal = VTOTAL_A,
		.vblank = VBLANK_A,
		.vsync = VSYNC_A,
		.stride = DSPASTRIDE,
		.size = DSPASIZE,
		.pos = DSPAPOS,
		.surf = DSPASURF,
		.addr = MRST_DSPABASE,
		.status = PIPEASTAT,
		.linoff = DSPALINOFF,
		.tileoff = DSPATILEOFF,
		.palette = PALETTE_A,
	},
	{
		.fp0 = MDFLD_DPLL_DIV0,
		.cntr = DSPBCNTR,
		.conf = PIPEBCONF,
		.src = PIPEBSRC,
		.dpll = MDFLD_DPLL_B,
		.htotal = HTOTAL_B,
		.hblank = HBLANK_B,
		.hsync = HSYNC_B,
		.vtotal = VTOTAL_B,
		.vblank = VBLANK_B,
		.vsync = VSYNC_B,
		.stride = DSPBSTRIDE,
		.size = DSPBSIZE,
		.pos = DSPBPOS,
		.surf = DSPBSURF,
		.addr = MRST_DSPBBASE,
		.status = PIPEBSTAT,
		.linoff = DSPBLINOFF,
		.tileoff = DSPBTILEOFF,
		.palette = PALETTE_B,
	},
	{
		.fp0 = MRST_FPA0,	/* This is what the old code did ?? */
		.cntr = DSPCCNTR,
		.conf = PIPECCONF,
		.src = PIPECSRC,
		/* No DPLL_C */
		.dpll = MRST_DPLL_A,
		.htotal = HTOTAL_C,
		.hblank = HBLANK_C,
		.hsync = HSYNC_C,
		.vtotal = VTOTAL_C,
		.vblank = VBLANK_C,
		.vsync = VSYNC_C,
		.stride = DSPCSTRIDE,
		.size = DSPBSIZE,
		.pos = DSPCPOS,
		.surf = DSPCSURF,
		.addr = MDFLD_DSPCBASE,
		.status = PIPECSTAT,
		.linoff = DSPCLINOFF,
		.tileoff = DSPCTILEOFF,
		.palette = PALETTE_C,
	},
};

static int mdfld_chip_setup(struct drm_device *dev)
{
	struct drm_psb_private *dev_priv = dev->dev_private;
	if (pci_enable_msi(dev->pdev))
		dev_warn(dev->dev, "Enabling MSI failed!\n");
	dev_priv->regmap = mdfld_regmap;
	return mid_chip_setup(dev);
}

>>>>>>> refs/remotes/origin/master
const struct psb_ops mdfld_chip_ops = {
	.name = "mdfld",
	.accel_2d = 0,
	.pipes = 3,
	.crtcs = 3,
<<<<<<< HEAD
	.sgx_offset = MRST_SGX_OFFSET,

	.chip_setup = mid_chip_setup,
=======
	.lvds_mask = (1 << 1),
	.hdmi_mask = (1 << 1),
	.cursor_needs_phys = 0,
	.sgx_offset = MRST_SGX_OFFSET,

	.chip_setup = mdfld_chip_setup,
>>>>>>> refs/remotes/origin/master
	.crtc_helper = &mdfld_helper_funcs,
	.crtc_funcs = &psb_intel_crtc_funcs,

	.output_init = mdfld_output_init,

#ifdef CONFIG_BACKLIGHT_CLASS_DEVICE
	.backlight_init = mdfld_backlight_init,
#endif

	.save_regs = mdfld_save_registers,
	.restore_regs = mdfld_restore_registers,
	.power_down = mdfld_power_down,
	.power_up = mdfld_power_up,
};
