/*
 * Copyright 2006 Dave Airlie <airlied@linux.ie>
 * Copyright © 2006-2007 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Authors:
 *	Eric Anholt <eric@anholt.net>
 */
#include <linux/i2c.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include "drmP.h"
#include "drm.h"
#include "drm_crtc.h"
#include "intel_drv.h"
#include "i915_drm.h"
=======
#include <drm/drmP.h>
#include <drm/drm_crtc.h>
#include "intel_drv.h"
#include <drm/i915_drm.h>
>>>>>>> refs/remotes/origin/master
#include "i915_drv.h"
#include "dvo.h"

#define SIL164_ADDR	0x38
#define CH7xxx_ADDR	0x76
#define TFP410_ADDR	0x38
<<<<<<< HEAD
=======
#define NS2501_ADDR     0x38
>>>>>>> refs/remotes/origin/master

static const struct intel_dvo_device intel_dvo_devices[] = {
	{
		.type = INTEL_DVO_CHIP_TMDS,
		.name = "sil164",
		.dvo_reg = DVOC,
		.slave_addr = SIL164_ADDR,
		.dev_ops = &sil164_ops,
	},
	{
		.type = INTEL_DVO_CHIP_TMDS,
		.name = "ch7xxx",
		.dvo_reg = DVOC,
		.slave_addr = CH7xxx_ADDR,
		.dev_ops = &ch7xxx_ops,
	},
	{
<<<<<<< HEAD
=======
		.type = INTEL_DVO_CHIP_TMDS,
		.name = "ch7xxx",
		.dvo_reg = DVOC,
		.slave_addr = 0x75, /* For some ch7010 */
		.dev_ops = &ch7xxx_ops,
	},
	{
>>>>>>> refs/remotes/origin/master
		.type = INTEL_DVO_CHIP_LVDS,
		.name = "ivch",
		.dvo_reg = DVOA,
		.slave_addr = 0x02, /* Might also be 0x44, 0x84, 0xc4 */
		.dev_ops = &ivch_ops,
	},
	{
		.type = INTEL_DVO_CHIP_TMDS,
		.name = "tfp410",
		.dvo_reg = DVOC,
		.slave_addr = TFP410_ADDR,
		.dev_ops = &tfp410_ops,
	},
	{
		.type = INTEL_DVO_CHIP_LVDS,
		.name = "ch7017",
		.dvo_reg = DVOC,
		.slave_addr = 0x75,
		.gpio = GMBUS_PORT_DPB,
		.dev_ops = &ch7017_ops,
<<<<<<< HEAD
	}
=======
	},
	{
	        .type = INTEL_DVO_CHIP_TMDS,
		.name = "ns2501",
		.dvo_reg = DVOC,
		.slave_addr = NS2501_ADDR,
		.dev_ops = &ns2501_ops,
       }
>>>>>>> refs/remotes/origin/master
};

struct intel_dvo {
	struct intel_encoder base;

	struct intel_dvo_device dev;

	struct drm_display_mode *panel_fixed_mode;
	bool panel_wants_dither;
};

<<<<<<< HEAD
static struct intel_dvo *enc_to_intel_dvo(struct drm_encoder *encoder)
{
	return container_of(encoder, struct intel_dvo, base.base);
=======
static struct intel_dvo *enc_to_dvo(struct intel_encoder *encoder)
{
	return container_of(encoder, struct intel_dvo, base);
>>>>>>> refs/remotes/origin/master
}

static struct intel_dvo *intel_attached_dvo(struct drm_connector *connector)
{
<<<<<<< HEAD
	return container_of(intel_attached_encoder(connector),
			    struct intel_dvo, base);
}

static void intel_dvo_dpms(struct drm_encoder *encoder, int mode)
{
	struct drm_i915_private *dev_priv = encoder->dev->dev_private;
	struct intel_dvo *intel_dvo = enc_to_intel_dvo(encoder);
	u32 dvo_reg = intel_dvo->dev.dvo_reg;
	u32 temp = I915_READ(dvo_reg);

	if (mode == DRM_MODE_DPMS_ON) {
		I915_WRITE(dvo_reg, temp | DVO_ENABLE);
		I915_READ(dvo_reg);
		intel_dvo->dev.dev_ops->dpms(&intel_dvo->dev, mode);
	} else {
		intel_dvo->dev.dev_ops->dpms(&intel_dvo->dev, mode);
		I915_WRITE(dvo_reg, temp & ~DVO_ENABLE);
		I915_READ(dvo_reg);
	}
=======
	return enc_to_dvo(intel_attached_encoder(connector));
}

static bool intel_dvo_connector_get_hw_state(struct intel_connector *connector)
{
	struct intel_dvo *intel_dvo = intel_attached_dvo(&connector->base);

	return intel_dvo->dev.dev_ops->get_hw_state(&intel_dvo->dev);
}

static bool intel_dvo_get_hw_state(struct intel_encoder *encoder,
				   enum pipe *pipe)
{
	struct drm_device *dev = encoder->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_dvo *intel_dvo = enc_to_dvo(encoder);
	u32 tmp;

	tmp = I915_READ(intel_dvo->dev.dvo_reg);

	if (!(tmp & DVO_ENABLE))
		return false;

	*pipe = PORT_TO_PIPE(tmp);

	return true;
}

static void intel_dvo_get_config(struct intel_encoder *encoder,
				 struct intel_crtc_config *pipe_config)
{
	struct drm_i915_private *dev_priv = encoder->base.dev->dev_private;
	struct intel_dvo *intel_dvo = enc_to_dvo(encoder);
	u32 tmp, flags = 0;

	tmp = I915_READ(intel_dvo->dev.dvo_reg);
	if (tmp & DVO_HSYNC_ACTIVE_HIGH)
		flags |= DRM_MODE_FLAG_PHSYNC;
	else
		flags |= DRM_MODE_FLAG_NHSYNC;
	if (tmp & DVO_VSYNC_ACTIVE_HIGH)
		flags |= DRM_MODE_FLAG_PVSYNC;
	else
		flags |= DRM_MODE_FLAG_NVSYNC;

	pipe_config->adjusted_mode.flags |= flags;

	pipe_config->adjusted_mode.crtc_clock = pipe_config->port_clock;
}

static void intel_disable_dvo(struct intel_encoder *encoder)
{
	struct drm_i915_private *dev_priv = encoder->base.dev->dev_private;
	struct intel_dvo *intel_dvo = enc_to_dvo(encoder);
	u32 dvo_reg = intel_dvo->dev.dvo_reg;
	u32 temp = I915_READ(dvo_reg);

	intel_dvo->dev.dev_ops->dpms(&intel_dvo->dev, false);
	I915_WRITE(dvo_reg, temp & ~DVO_ENABLE);
	I915_READ(dvo_reg);
}

static void intel_enable_dvo(struct intel_encoder *encoder)
{
	struct drm_i915_private *dev_priv = encoder->base.dev->dev_private;
	struct intel_dvo *intel_dvo = enc_to_dvo(encoder);
	struct intel_crtc *crtc = to_intel_crtc(encoder->base.crtc);
	u32 dvo_reg = intel_dvo->dev.dvo_reg;
	u32 temp = I915_READ(dvo_reg);

	I915_WRITE(dvo_reg, temp | DVO_ENABLE);
	I915_READ(dvo_reg);
	intel_dvo->dev.dev_ops->mode_set(&intel_dvo->dev,
					 &crtc->config.requested_mode,
					 &crtc->config.adjusted_mode);

	intel_dvo->dev.dev_ops->dpms(&intel_dvo->dev, true);
}

/* Special dpms function to support cloning between dvo/sdvo/crt. */
static void intel_dvo_dpms(struct drm_connector *connector, int mode)
{
	struct intel_dvo *intel_dvo = intel_attached_dvo(connector);
	struct drm_crtc *crtc;
	struct intel_crtc_config *config;

	/* dvo supports only 2 dpms states. */
	if (mode != DRM_MODE_DPMS_ON)
		mode = DRM_MODE_DPMS_OFF;

	if (mode == connector->dpms)
		return;

	connector->dpms = mode;

	/* Only need to change hw state when actually enabled */
	crtc = intel_dvo->base.base.crtc;
	if (!crtc) {
		intel_dvo->base.connectors_active = false;
		return;
	}

	/* We call connector dpms manually below in case pipe dpms doesn't
	 * change due to cloning. */
	if (mode == DRM_MODE_DPMS_ON) {
		config = &to_intel_crtc(crtc)->config;

		intel_dvo->base.connectors_active = true;

		intel_crtc_update_dpms(crtc);

		intel_dvo->dev.dev_ops->mode_set(&intel_dvo->dev,
						 &config->requested_mode,
						 &config->adjusted_mode);

		intel_dvo->dev.dev_ops->dpms(&intel_dvo->dev, true);
	} else {
		intel_dvo->dev.dev_ops->dpms(&intel_dvo->dev, false);

		intel_dvo->base.connectors_active = false;

		intel_crtc_update_dpms(crtc);
	}

	intel_modeset_check_state(connector->dev);
>>>>>>> refs/remotes/origin/master
}

static int intel_dvo_mode_valid(struct drm_connector *connector,
				struct drm_display_mode *mode)
{
	struct intel_dvo *intel_dvo = intel_attached_dvo(connector);

	if (mode->flags & DRM_MODE_FLAG_DBLSCAN)
		return MODE_NO_DBLESCAN;

	/* XXX: Validate clock range */

	if (intel_dvo->panel_fixed_mode) {
		if (mode->hdisplay > intel_dvo->panel_fixed_mode->hdisplay)
			return MODE_PANEL;
		if (mode->vdisplay > intel_dvo->panel_fixed_mode->vdisplay)
			return MODE_PANEL;
	}

	return intel_dvo->dev.dev_ops->mode_valid(&intel_dvo->dev, mode);
}

<<<<<<< HEAD
static bool intel_dvo_mode_fixup(struct drm_encoder *encoder,
				 struct drm_display_mode *mode,
				 struct drm_display_mode *adjusted_mode)
{
	struct intel_dvo *intel_dvo = enc_to_intel_dvo(encoder);
=======
static bool intel_dvo_compute_config(struct intel_encoder *encoder,
				     struct intel_crtc_config *pipe_config)
{
	struct intel_dvo *intel_dvo = enc_to_dvo(encoder);
	struct drm_display_mode *adjusted_mode = &pipe_config->adjusted_mode;
>>>>>>> refs/remotes/origin/master

	/* If we have timings from the BIOS for the panel, put them in
	 * to the adjusted mode.  The CRTC will be set up for this mode,
	 * with the panel scaling set up to source from the H/VDisplay
	 * of the original mode.
	 */
	if (intel_dvo->panel_fixed_mode != NULL) {
#define C(x) adjusted_mode->x = intel_dvo->panel_fixed_mode->x
		C(hdisplay);
		C(hsync_start);
		C(hsync_end);
		C(htotal);
		C(vdisplay);
		C(vsync_start);
		C(vsync_end);
		C(vtotal);
		C(clock);
<<<<<<< HEAD
<<<<<<< HEAD
		drm_mode_set_crtcinfo(adjusted_mode, CRTC_INTERLACE_HALVE_V);
=======
>>>>>>> refs/remotes/origin/cm-10.0
#undef C
	}

	if (intel_dvo->dev.dev_ops->mode_fixup)
		return intel_dvo->dev.dev_ops->mode_fixup(&intel_dvo->dev, mode, adjusted_mode);
=======
#undef C

		drm_mode_set_crtcinfo(adjusted_mode, 0);
	}
>>>>>>> refs/remotes/origin/master

	return true;
}

<<<<<<< HEAD
static void intel_dvo_mode_set(struct drm_encoder *encoder,
			       struct drm_display_mode *mode,
			       struct drm_display_mode *adjusted_mode)
{
	struct drm_device *dev = encoder->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *intel_crtc = to_intel_crtc(encoder->crtc);
	struct intel_dvo *intel_dvo = enc_to_intel_dvo(encoder);
	int pipe = intel_crtc->pipe;
	u32 dvo_val;
	u32 dvo_reg = intel_dvo->dev.dvo_reg, dvo_srcdim_reg;
	int dpll_reg = DPLL(pipe);
=======
static void intel_dvo_mode_set(struct intel_encoder *encoder)
{
	struct drm_device *dev = encoder->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_crtc *crtc = to_intel_crtc(encoder->base.crtc);
	struct drm_display_mode *adjusted_mode = &crtc->config.adjusted_mode;
	struct intel_dvo *intel_dvo = enc_to_dvo(encoder);
	int pipe = crtc->pipe;
	u32 dvo_val;
	u32 dvo_reg = intel_dvo->dev.dvo_reg, dvo_srcdim_reg;
>>>>>>> refs/remotes/origin/master

	switch (dvo_reg) {
	case DVOA:
	default:
		dvo_srcdim_reg = DVOA_SRCDIM;
		break;
	case DVOB:
		dvo_srcdim_reg = DVOB_SRCDIM;
		break;
	case DVOC:
		dvo_srcdim_reg = DVOC_SRCDIM;
		break;
	}

<<<<<<< HEAD
	intel_dvo->dev.dev_ops->mode_set(&intel_dvo->dev, mode, adjusted_mode);

=======
>>>>>>> refs/remotes/origin/master
	/* Save the data order, since I don't know what it should be set to. */
	dvo_val = I915_READ(dvo_reg) &
		  (DVO_PRESERVE_MASK | DVO_DATA_ORDER_GBRG);
	dvo_val |= DVO_DATA_ORDER_FP | DVO_BORDER_ENABLE |
		   DVO_BLANK_ACTIVE_HIGH;

	if (pipe == 1)
		dvo_val |= DVO_PIPE_B_SELECT;
	dvo_val |= DVO_PIPE_STALL;
	if (adjusted_mode->flags & DRM_MODE_FLAG_PHSYNC)
		dvo_val |= DVO_HSYNC_ACTIVE_HIGH;
	if (adjusted_mode->flags & DRM_MODE_FLAG_PVSYNC)
		dvo_val |= DVO_VSYNC_ACTIVE_HIGH;

<<<<<<< HEAD
	I915_WRITE(dpll_reg, I915_READ(dpll_reg) | DPLL_DVO_HIGH_SPEED);

=======
>>>>>>> refs/remotes/origin/master
	/*I915_WRITE(DVOB_SRCDIM,
	  (adjusted_mode->hdisplay << DVO_SRCDIM_HORIZONTAL_SHIFT) |
	  (adjusted_mode->VDisplay << DVO_SRCDIM_VERTICAL_SHIFT));*/
	I915_WRITE(dvo_srcdim_reg,
		   (adjusted_mode->hdisplay << DVO_SRCDIM_HORIZONTAL_SHIFT) |
		   (adjusted_mode->vdisplay << DVO_SRCDIM_VERTICAL_SHIFT));
	/*I915_WRITE(DVOB, dvo_val);*/
	I915_WRITE(dvo_reg, dvo_val);
}

/**
 * Detect the output connection on our DVO device.
 *
 * Unimplemented.
 */
static enum drm_connector_status
intel_dvo_detect(struct drm_connector *connector, bool force)
{
	struct intel_dvo *intel_dvo = intel_attached_dvo(connector);
<<<<<<< HEAD
=======
	DRM_DEBUG_KMS("[CONNECTOR:%d:%s]\n",
		      connector->base.id, drm_get_connector_name(connector));
>>>>>>> refs/remotes/origin/master
	return intel_dvo->dev.dev_ops->detect(&intel_dvo->dev);
}

static int intel_dvo_get_modes(struct drm_connector *connector)
{
	struct intel_dvo *intel_dvo = intel_attached_dvo(connector);
	struct drm_i915_private *dev_priv = connector->dev->dev_private;

	/* We should probably have an i2c driver get_modes function for those
	 * devices which will have a fixed set of modes determined by the chip
	 * (TV-out, for example), but for now with just TMDS and LVDS,
	 * that's not the case.
	 */
	intel_ddc_get_modes(connector,
<<<<<<< HEAD
			    &dev_priv->gmbus[GMBUS_PORT_DPC].adapter);
=======
			    intel_gmbus_get_adapter(dev_priv, GMBUS_PORT_DPC));
>>>>>>> refs/remotes/origin/master
	if (!list_empty(&connector->probed_modes))
		return 1;

	if (intel_dvo->panel_fixed_mode != NULL) {
		struct drm_display_mode *mode;
		mode = drm_mode_duplicate(connector->dev, intel_dvo->panel_fixed_mode);
		if (mode) {
			drm_mode_probed_add(connector, mode);
			return 1;
		}
	}

	return 0;
}

static void intel_dvo_destroy(struct drm_connector *connector)
{
<<<<<<< HEAD
	drm_sysfs_connector_remove(connector);
=======
>>>>>>> refs/remotes/origin/master
	drm_connector_cleanup(connector);
	kfree(connector);
}

<<<<<<< HEAD
static const struct drm_encoder_helper_funcs intel_dvo_helper_funcs = {
	.dpms = intel_dvo_dpms,
	.mode_fixup = intel_dvo_mode_fixup,
	.prepare = intel_encoder_prepare,
	.mode_set = intel_dvo_mode_set,
	.commit = intel_encoder_commit,
};

static const struct drm_connector_funcs intel_dvo_connector_funcs = {
	.dpms = drm_helper_connector_dpms,
=======
static const struct drm_connector_funcs intel_dvo_connector_funcs = {
	.dpms = intel_dvo_dpms,
>>>>>>> refs/remotes/origin/master
	.detect = intel_dvo_detect,
	.destroy = intel_dvo_destroy,
	.fill_modes = drm_helper_probe_single_connector_modes,
};

static const struct drm_connector_helper_funcs intel_dvo_connector_helper_funcs = {
	.mode_valid = intel_dvo_mode_valid,
	.get_modes = intel_dvo_get_modes,
	.best_encoder = intel_best_encoder,
};

static void intel_dvo_enc_destroy(struct drm_encoder *encoder)
{
<<<<<<< HEAD
	struct intel_dvo *intel_dvo = enc_to_intel_dvo(encoder);
=======
	struct intel_dvo *intel_dvo = enc_to_dvo(to_intel_encoder(encoder));
>>>>>>> refs/remotes/origin/master

	if (intel_dvo->dev.dev_ops->destroy)
		intel_dvo->dev.dev_ops->destroy(&intel_dvo->dev);

	kfree(intel_dvo->panel_fixed_mode);

	intel_encoder_destroy(encoder);
}

static const struct drm_encoder_funcs intel_dvo_enc_funcs = {
	.destroy = intel_dvo_enc_destroy,
};

/**
 * Attempts to get a fixed panel timing for LVDS (currently only the i830).
 *
 * Other chips with DVO LVDS will need to extend this to deal with the LVDS
 * chip being on DVOB/C and having multiple pipes.
 */
static struct drm_display_mode *
intel_dvo_get_current_mode(struct drm_connector *connector)
{
	struct drm_device *dev = connector->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_dvo *intel_dvo = intel_attached_dvo(connector);
	uint32_t dvo_val = I915_READ(intel_dvo->dev.dvo_reg);
	struct drm_display_mode *mode = NULL;

	/* If the DVO port is active, that'll be the LVDS, so we can pull out
	 * its timings to get how the BIOS set up the panel.
	 */
	if (dvo_val & DVO_ENABLE) {
		struct drm_crtc *crtc;
		int pipe = (dvo_val & DVO_PIPE_B_SELECT) ? 1 : 0;

		crtc = intel_get_crtc_for_pipe(dev, pipe);
		if (crtc) {
			mode = intel_crtc_mode_get(dev, crtc);
			if (mode) {
				mode->type |= DRM_MODE_TYPE_PREFERRED;
				if (dvo_val & DVO_HSYNC_ACTIVE_HIGH)
					mode->flags |= DRM_MODE_FLAG_PHSYNC;
				if (dvo_val & DVO_VSYNC_ACTIVE_HIGH)
					mode->flags |= DRM_MODE_FLAG_PVSYNC;
			}
		}
	}

	return mode;
}

void intel_dvo_init(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct intel_encoder *intel_encoder;
	struct intel_dvo *intel_dvo;
	struct intel_connector *intel_connector;
	int i;
	int encoder_type = DRM_MODE_ENCODER_NONE;

<<<<<<< HEAD
	intel_dvo = kzalloc(sizeof(struct intel_dvo), GFP_KERNEL);
	if (!intel_dvo)
		return;

	intel_connector = kzalloc(sizeof(struct intel_connector), GFP_KERNEL);
=======
	intel_dvo = kzalloc(sizeof(*intel_dvo), GFP_KERNEL);
	if (!intel_dvo)
		return;

	intel_connector = kzalloc(sizeof(*intel_connector), GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (!intel_connector) {
		kfree(intel_dvo);
		return;
	}

	intel_encoder = &intel_dvo->base;
	drm_encoder_init(dev, &intel_encoder->base,
			 &intel_dvo_enc_funcs, encoder_type);

<<<<<<< HEAD
=======
	intel_encoder->disable = intel_disable_dvo;
	intel_encoder->enable = intel_enable_dvo;
	intel_encoder->get_hw_state = intel_dvo_get_hw_state;
	intel_encoder->get_config = intel_dvo_get_config;
	intel_encoder->compute_config = intel_dvo_compute_config;
	intel_encoder->mode_set = intel_dvo_mode_set;
	intel_connector->get_hw_state = intel_dvo_connector_get_hw_state;

>>>>>>> refs/remotes/origin/master
	/* Now, try to find a controller */
	for (i = 0; i < ARRAY_SIZE(intel_dvo_devices); i++) {
		struct drm_connector *connector = &intel_connector->base;
		const struct intel_dvo_device *dvo = &intel_dvo_devices[i];
		struct i2c_adapter *i2c;
		int gpio;
		bool dvoinit;

		/* Allow the I2C driver info to specify the GPIO to be used in
		 * special cases, but otherwise default to what's defined
		 * in the spec.
		 */
<<<<<<< HEAD
		if (dvo->gpio != 0)
=======
		if (intel_gmbus_is_port_valid(dvo->gpio))
>>>>>>> refs/remotes/origin/master
			gpio = dvo->gpio;
		else if (dvo->type == INTEL_DVO_CHIP_LVDS)
			gpio = GMBUS_PORT_SSC;
		else
			gpio = GMBUS_PORT_DPB;

		/* Set up the I2C bus necessary for the chip we're probing.
		 * It appears that everything is on GPIOE except for panels
		 * on i830 laptops, which are on GPIOB (DVOA).
		 */
<<<<<<< HEAD
		i2c = &dev_priv->gmbus[gpio].adapter;
=======
		i2c = intel_gmbus_get_adapter(dev_priv, gpio);
>>>>>>> refs/remotes/origin/master

		intel_dvo->dev = *dvo;

		/* GMBUS NAK handling seems to be unstable, hence let the
		 * transmitter detection run in bit banging mode for now.
		 */
		intel_gmbus_force_bit(i2c, true);

		dvoinit = dvo->dev_ops->init(&intel_dvo->dev, i2c);

		intel_gmbus_force_bit(i2c, false);

		if (!dvoinit)
			continue;

		intel_encoder->type = INTEL_OUTPUT_DVO;
		intel_encoder->crtc_mask = (1 << 0) | (1 << 1);
		switch (dvo->type) {
		case INTEL_DVO_CHIP_TMDS:
<<<<<<< HEAD
			intel_encoder->clone_mask =
				(1 << INTEL_DVO_TMDS_CLONE_BIT) |
				(1 << INTEL_ANALOG_CLONE_BIT);
=======
			intel_encoder->cloneable = true;
>>>>>>> refs/remotes/origin/master
			drm_connector_init(dev, connector,
					   &intel_dvo_connector_funcs,
					   DRM_MODE_CONNECTOR_DVII);
			encoder_type = DRM_MODE_ENCODER_TMDS;
			break;
		case INTEL_DVO_CHIP_LVDS:
<<<<<<< HEAD
			intel_encoder->clone_mask =
				(1 << INTEL_DVO_LVDS_CLONE_BIT);
=======
			intel_encoder->cloneable = false;
>>>>>>> refs/remotes/origin/master
			drm_connector_init(dev, connector,
					   &intel_dvo_connector_funcs,
					   DRM_MODE_CONNECTOR_LVDS);
			encoder_type = DRM_MODE_ENCODER_LVDS;
			break;
		}

		drm_connector_helper_add(connector,
					 &intel_dvo_connector_helper_funcs);
		connector->display_info.subpixel_order = SubPixelHorizontalRGB;
		connector->interlace_allowed = false;
		connector->doublescan_allowed = false;

<<<<<<< HEAD
		drm_encoder_helper_add(&intel_encoder->base,
				       &intel_dvo_helper_funcs);

=======
>>>>>>> refs/remotes/origin/master
		intel_connector_attach_encoder(intel_connector, intel_encoder);
		if (dvo->type == INTEL_DVO_CHIP_LVDS) {
			/* For our LVDS chipsets, we should hopefully be able
			 * to dig the fixed panel mode out of the BIOS data.
			 * However, it's in a different format from the BIOS
			 * data on chipsets with integrated LVDS (stored in AIM
			 * headers, likely), so for now, just get the current
			 * mode being output through DVO.
			 */
			intel_dvo->panel_fixed_mode =
				intel_dvo_get_current_mode(connector);
			intel_dvo->panel_wants_dither = true;
		}

		drm_sysfs_connector_add(connector);
		return;
	}

	drm_encoder_cleanup(&intel_encoder->base);
	kfree(intel_dvo);
	kfree(intel_connector);
}
