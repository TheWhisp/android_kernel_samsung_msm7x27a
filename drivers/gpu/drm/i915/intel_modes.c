/*
 * Copyright (c) 2007 Dave Airlie <airlied@linux.ie>
 * Copyright (c) 2007, 2010 Intel Corporation
 *   Jesse Barnes <jesse.barnes@intel.com>
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
 */

#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/fb.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include "drmP.h"
=======
#include <drm/drm_edid.h>
#include "drmP.h"
#include "drm_edid.h"
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <drm/drm_edid.h>
#include <drm/drmP.h>
>>>>>>> refs/remotes/origin/master
#include "intel_drv.h"
#include "i915_drv.h"

/**
<<<<<<< HEAD
 * intel_ddc_probe
 *
 */
bool intel_ddc_probe(struct intel_encoder *intel_encoder, int ddc_bus)
{
	struct drm_i915_private *dev_priv = intel_encoder->base.dev->dev_private;
	u8 out_buf[] = { 0x0, 0x0};
	u8 buf[2];
	struct i2c_msg msgs[] = {
		{
<<<<<<< HEAD
			.addr = 0x50,
=======
			.addr = DDC_ADDR,
>>>>>>> refs/remotes/origin/cm-10.0
			.flags = 0,
			.len = 1,
			.buf = out_buf,
		},
		{
<<<<<<< HEAD
			.addr = 0x50,
=======
			.addr = DDC_ADDR,
>>>>>>> refs/remotes/origin/cm-10.0
			.flags = I2C_M_RD,
			.len = 1,
			.buf = buf,
		}
	};

	return i2c_transfer(&dev_priv->gmbus[ddc_bus].adapter, msgs, 2) == 2;
}

/**
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
 * intel_connector_update_modes - update connector from edid
 * @connector: DRM connector device to use
 * @edid: previously read EDID information
 */
int intel_connector_update_modes(struct drm_connector *connector,
				struct edid *edid)
{
	int ret;

	drm_mode_connector_update_edid_property(connector, edid);
	ret = drm_add_edid_modes(connector, edid);
	drm_edid_to_eld(connector, edid);
<<<<<<< HEAD
	connector->display_info.raw_edid = NULL;
	kfree(edid);
=======
>>>>>>> refs/remotes/origin/master

	return ret;
}

/**
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 * intel_ddc_get_modes - get modelist from monitor
 * @connector: DRM connector device to use
 * @adapter: i2c adapter
 *
 * Fetch the EDID information from @connector using the DDC bus.
 */
int intel_ddc_get_modes(struct drm_connector *connector,
			struct i2c_adapter *adapter)
{
	struct edid *edid;
<<<<<<< HEAD
<<<<<<< HEAD
	int ret = 0;

	edid = drm_get_edid(connector, adapter);
	if (edid) {
		drm_mode_connector_update_edid_property(connector, edid);
		ret = drm_add_edid_modes(connector, edid);
		connector->display_info.raw_edid = NULL;
		kfree(edid);
	}

	return ret;
}

static const char *force_audio_names[] = {
	"off",
	"auto",
	"on",
=======
=======
	int ret;
>>>>>>> refs/remotes/origin/master

	edid = drm_get_edid(connector, adapter);
	if (!edid)
		return 0;

<<<<<<< HEAD
	return intel_connector_update_modes(connector, edid);
=======
	ret = intel_connector_update_modes(connector, edid);
	kfree(edid);

	return ret;
>>>>>>> refs/remotes/origin/master
}

static const struct drm_prop_enum_list force_audio_names[] = {
	{ HDMI_AUDIO_OFF_DVI, "force-dvi" },
	{ HDMI_AUDIO_OFF, "off" },
	{ HDMI_AUDIO_AUTO, "auto" },
	{ HDMI_AUDIO_ON, "on" },
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};

void
intel_attach_force_audio_property(struct drm_connector *connector)
{
	struct drm_device *dev = connector->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct drm_property *prop;
<<<<<<< HEAD
<<<<<<< HEAD
	int i;

	prop = dev_priv->force_audio_property;
	if (prop == NULL) {
		prop = drm_property_create(dev, DRM_MODE_PROP_ENUM,
					   "audio",
=======
=======
>>>>>>> refs/remotes/origin/master

	prop = dev_priv->force_audio_property;
	if (prop == NULL) {
		prop = drm_property_create_enum(dev, 0,
					   "audio",
					   force_audio_names,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
					   ARRAY_SIZE(force_audio_names));
		if (prop == NULL)
			return;

<<<<<<< HEAD
<<<<<<< HEAD
		for (i = 0; i < ARRAY_SIZE(force_audio_names); i++)
			drm_property_add_enum(prop, i, i-1, force_audio_names[i]);

=======
>>>>>>> refs/remotes/origin/cm-10.0
		dev_priv->force_audio_property = prop;
	}
	drm_connector_attach_property(connector, prop, 0);
}

<<<<<<< HEAD
static const char *broadcast_rgb_names[] = {
	"Full",
	"Limited 16:235",
=======
static const struct drm_prop_enum_list broadcast_rgb_names[] = {
	{ 0, "Full" },
	{ 1, "Limited 16:235" },
>>>>>>> refs/remotes/origin/cm-10.0
=======
		dev_priv->force_audio_property = prop;
	}
	drm_object_attach_property(&connector->base, prop, 0);
}

static const struct drm_prop_enum_list broadcast_rgb_names[] = {
	{ INTEL_BROADCAST_RGB_AUTO, "Automatic" },
	{ INTEL_BROADCAST_RGB_FULL, "Full" },
	{ INTEL_BROADCAST_RGB_LIMITED, "Limited 16:235" },
>>>>>>> refs/remotes/origin/master
};

void
intel_attach_broadcast_rgb_property(struct drm_connector *connector)
{
	struct drm_device *dev = connector->dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct drm_property *prop;
<<<<<<< HEAD
<<<<<<< HEAD
	int i;

	prop = dev_priv->broadcast_rgb_property;
	if (prop == NULL) {
		prop = drm_property_create(dev, DRM_MODE_PROP_ENUM,
					   "Broadcast RGB",
=======
=======
>>>>>>> refs/remotes/origin/master

	prop = dev_priv->broadcast_rgb_property;
	if (prop == NULL) {
		prop = drm_property_create_enum(dev, DRM_MODE_PROP_ENUM,
					   "Broadcast RGB",
					   broadcast_rgb_names,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
					   ARRAY_SIZE(broadcast_rgb_names));
		if (prop == NULL)
			return;

<<<<<<< HEAD
<<<<<<< HEAD
		for (i = 0; i < ARRAY_SIZE(broadcast_rgb_names); i++)
			drm_property_add_enum(prop, i, i, broadcast_rgb_names[i]);

=======
>>>>>>> refs/remotes/origin/cm-10.0
		dev_priv->broadcast_rgb_property = prop;
	}

	drm_connector_attach_property(connector, prop, 0);
=======
		dev_priv->broadcast_rgb_property = prop;
	}

	drm_object_attach_property(&connector->base, prop, 0);
>>>>>>> refs/remotes/origin/master
}
