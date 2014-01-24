/*
 * Copyright (C) 2008 Maarten Maathuis.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE COPYRIGHT OWNER(S) AND/OR ITS SUPPLIERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

<<<<<<< HEAD
#include "drmP.h"
#include "drm_crtc_helper.h"
#include "nouveau_drv.h"
#include "nouveau_fb.h"
#include "nouveau_fbcon.h"
#include "nouveau_hw.h"
#include "nouveau_crtc.h"
#include "nouveau_dma.h"
<<<<<<< HEAD
=======
#include "nouveau_connector.h"
#include "nouveau_gpio.h"
>>>>>>> refs/remotes/origin/cm-10.0
#include "nv50_display.h"

=======
#include <drm/drmP.h>
#include <drm/drm_crtc_helper.h>

#include "nouveau_fbcon.h"
#include "dispnv04/hw.h"
#include "nouveau_crtc.h"
#include "nouveau_dma.h"
#include "nouveau_gem.h"
#include "nouveau_connector.h"
#include "nv50_display.h"

#include "nouveau_fence.h"

#include <engine/disp.h>

#include <core/class.h>

static int
nouveau_display_vblank_handler(void *data, int head)
{
	struct nouveau_drm *drm = data;
	drm_handle_vblank(drm->dev, head);
	return NVKM_EVENT_KEEP;
}

int
nouveau_display_vblank_enable(struct drm_device *dev, int head)
{
	struct nouveau_display *disp = nouveau_display(dev);
	if (disp) {
		nouveau_event_get(disp->vblank[head]);
		return 0;
	}
	return -EIO;
}

void
nouveau_display_vblank_disable(struct drm_device *dev, int head)
{
	struct nouveau_display *disp = nouveau_display(dev);
	if (disp)
		nouveau_event_put(disp->vblank[head]);
}

static void
nouveau_display_vblank_fini(struct drm_device *dev)
{
	struct nouveau_display *disp = nouveau_display(dev);
	int i;

	if (disp->vblank) {
		for (i = 0; i < dev->mode_config.num_crtc; i++)
			nouveau_event_ref(NULL, &disp->vblank[i]);
		kfree(disp->vblank);
		disp->vblank = NULL;
	}

	drm_vblank_cleanup(dev);
}

static int
nouveau_display_vblank_init(struct drm_device *dev)
{
	struct nouveau_display *disp = nouveau_display(dev);
	struct nouveau_drm *drm = nouveau_drm(dev);
	struct nouveau_disp *pdisp = nouveau_disp(drm->device);
	int ret, i;

	disp->vblank = kzalloc(dev->mode_config.num_crtc *
			       sizeof(*disp->vblank), GFP_KERNEL);
	if (!disp->vblank)
		return -ENOMEM;

	for (i = 0; i < dev->mode_config.num_crtc; i++) {
		ret = nouveau_event_new(pdisp->vblank, i,
					nouveau_display_vblank_handler,
					drm, &disp->vblank[i]);
		if (ret) {
			nouveau_display_vblank_fini(dev);
			return ret;
		}
	}

	ret = drm_vblank_init(dev, dev->mode_config.num_crtc);
	if (ret) {
		nouveau_display_vblank_fini(dev);
		return ret;
	}

	return 0;
}

>>>>>>> refs/remotes/origin/master
static void
nouveau_user_framebuffer_destroy(struct drm_framebuffer *drm_fb)
{
	struct nouveau_framebuffer *fb = nouveau_framebuffer(drm_fb);

	if (fb->nvbo)
<<<<<<< HEAD
		drm_gem_object_unreference_unlocked(fb->nvbo->gem);
=======
		drm_gem_object_unreference_unlocked(&fb->nvbo->gem);
>>>>>>> refs/remotes/origin/master

	drm_framebuffer_cleanup(drm_fb);
	kfree(fb);
}

static int
nouveau_user_framebuffer_create_handle(struct drm_framebuffer *drm_fb,
				       struct drm_file *file_priv,
				       unsigned int *handle)
{
	struct nouveau_framebuffer *fb = nouveau_framebuffer(drm_fb);

<<<<<<< HEAD
	return drm_gem_handle_create(file_priv, fb->nvbo->gem, handle);
=======
	return drm_gem_handle_create(file_priv, &fb->nvbo->gem, handle);
>>>>>>> refs/remotes/origin/master
}

static const struct drm_framebuffer_funcs nouveau_framebuffer_funcs = {
	.destroy = nouveau_user_framebuffer_destroy,
	.create_handle = nouveau_user_framebuffer_create_handle,
};

int
nouveau_framebuffer_init(struct drm_device *dev,
			 struct nouveau_framebuffer *nv_fb,
<<<<<<< HEAD
<<<<<<< HEAD
			 struct drm_mode_fb_cmd *mode_cmd,
=======
			 struct drm_mode_fb_cmd2 *mode_cmd,
>>>>>>> refs/remotes/origin/cm-10.0
			 struct nouveau_bo *nvbo)
{
	struct drm_nouveau_private *dev_priv = dev->dev_private;
	struct drm_framebuffer *fb = &nv_fb->base;
	int ret;

	ret = drm_framebuffer_init(dev, fb, &nouveau_framebuffer_funcs);
	if (ret) {
		return ret;
	}

	drm_helper_mode_fill_fb_struct(fb, mode_cmd);
	nv_fb->nvbo = nvbo;

	if (dev_priv->card_type >= NV_50) {
=======
			 struct drm_mode_fb_cmd2 *mode_cmd,
			 struct nouveau_bo *nvbo)
{
	struct nouveau_drm *drm = nouveau_drm(dev);
	struct drm_framebuffer *fb = &nv_fb->base;
	int ret;

	drm_helper_mode_fill_fb_struct(fb, mode_cmd);
	nv_fb->nvbo = nvbo;

	if (nv_device(drm->device)->card_type >= NV_50) {
>>>>>>> refs/remotes/origin/master
		u32 tile_flags = nouveau_bo_tile_layout(nvbo);
		if (tile_flags == 0x7a00 ||
		    tile_flags == 0xfe00)
			nv_fb->r_dma = NvEvoFB32;
		else
		if (tile_flags == 0x7000)
			nv_fb->r_dma = NvEvoFB16;
		else
			nv_fb->r_dma = NvEvoVRAM_LP;

		switch (fb->depth) {
<<<<<<< HEAD
		case  8: nv_fb->r_format = NV50_EVO_CRTC_FB_DEPTH_8; break;
		case 15: nv_fb->r_format = NV50_EVO_CRTC_FB_DEPTH_15; break;
		case 16: nv_fb->r_format = NV50_EVO_CRTC_FB_DEPTH_16; break;
		case 24:
		case 32: nv_fb->r_format = NV50_EVO_CRTC_FB_DEPTH_24; break;
		case 30: nv_fb->r_format = NV50_EVO_CRTC_FB_DEPTH_30; break;
		default:
			 NV_ERROR(dev, "unknown depth %d\n", fb->depth);
			 return -EINVAL;
		}

		if (dev_priv->chipset == 0x50)
			nv_fb->r_format |= (tile_flags << 8);

<<<<<<< HEAD
		if (!tile_flags)
			nv_fb->r_pitch = 0x00100000 | fb->pitch;
		else {
			u32 mode = nvbo->tile_mode;
			if (dev_priv->card_type >= NV_C0)
				mode >>= 4;
			nv_fb->r_pitch = ((fb->pitch / 4) << 4) | mode;
=======
		if (!tile_flags) {
			if (dev_priv->card_type < NV_D0)
=======
		case  8: nv_fb->r_format = 0x1e00; break;
		case 15: nv_fb->r_format = 0xe900; break;
		case 16: nv_fb->r_format = 0xe800; break;
		case 24:
		case 32: nv_fb->r_format = 0xcf00; break;
		case 30: nv_fb->r_format = 0xd100; break;
		default:
			 NV_ERROR(drm, "unknown depth %d\n", fb->depth);
			 return -EINVAL;
		}

		if (nvbo->tile_flags & NOUVEAU_GEM_TILE_NONCONTIG) {
			NV_ERROR(drm, "framebuffer requires contiguous bo\n");
			return -EINVAL;
		}

		if (nv_device(drm->device)->chipset == 0x50)
			nv_fb->r_format |= (tile_flags << 8);

		if (!tile_flags) {
			if (nv_device(drm->device)->card_type < NV_D0)
>>>>>>> refs/remotes/origin/master
				nv_fb->r_pitch = 0x00100000 | fb->pitches[0];
			else
				nv_fb->r_pitch = 0x01000000 | fb->pitches[0];
		} else {
			u32 mode = nvbo->tile_mode;
<<<<<<< HEAD
			if (dev_priv->card_type >= NV_C0)
				mode >>= 4;
			nv_fb->r_pitch = ((fb->pitches[0] / 4) << 4) | mode;
>>>>>>> refs/remotes/origin/cm-10.0
		}
	}

=======
			if (nv_device(drm->device)->card_type >= NV_C0)
				mode >>= 4;
			nv_fb->r_pitch = ((fb->pitches[0] / 4) << 4) | mode;
		}
	}

	ret = drm_framebuffer_init(dev, fb, &nouveau_framebuffer_funcs);
	if (ret) {
		return ret;
	}

>>>>>>> refs/remotes/origin/master
	return 0;
}

static struct drm_framebuffer *
nouveau_user_framebuffer_create(struct drm_device *dev,
				struct drm_file *file_priv,
<<<<<<< HEAD
<<<<<<< HEAD
				struct drm_mode_fb_cmd *mode_cmd)
=======
				struct drm_mode_fb_cmd2 *mode_cmd)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct nouveau_framebuffer *nouveau_fb;
	struct drm_gem_object *gem;
	int ret;

<<<<<<< HEAD
	gem = drm_gem_object_lookup(dev, file_priv, mode_cmd->handle);
=======
	gem = drm_gem_object_lookup(dev, file_priv, mode_cmd->handles[0]);
>>>>>>> refs/remotes/origin/cm-10.0
=======
				struct drm_mode_fb_cmd2 *mode_cmd)
{
	struct nouveau_framebuffer *nouveau_fb;
	struct drm_gem_object *gem;
	int ret = -ENOMEM;

	gem = drm_gem_object_lookup(dev, file_priv, mode_cmd->handles[0]);
>>>>>>> refs/remotes/origin/master
	if (!gem)
		return ERR_PTR(-ENOENT);

	nouveau_fb = kzalloc(sizeof(struct nouveau_framebuffer), GFP_KERNEL);
	if (!nouveau_fb)
<<<<<<< HEAD
		return ERR_PTR(-ENOMEM);

	ret = nouveau_framebuffer_init(dev, nouveau_fb, mode_cmd, nouveau_gem_object(gem));
	if (ret) {
		drm_gem_object_unreference(gem);
		return ERR_PTR(ret);
	}

	return &nouveau_fb->base;
}

<<<<<<< HEAD
const struct drm_mode_config_funcs nouveau_mode_config_funcs = {
=======
static const struct drm_mode_config_funcs nouveau_mode_config_funcs = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
		goto err_unref;

	ret = nouveau_framebuffer_init(dev, nouveau_fb, mode_cmd, nouveau_gem_object(gem));
	if (ret)
		goto err;

	return &nouveau_fb->base;

err:
	kfree(nouveau_fb);
err_unref:
	drm_gem_object_unreference(gem);
	return ERR_PTR(ret);
}

static const struct drm_mode_config_funcs nouveau_mode_config_funcs = {
>>>>>>> refs/remotes/origin/master
	.fb_create = nouveau_user_framebuffer_create,
	.output_poll_changed = nouveau_fbcon_output_poll_changed,
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master

struct nouveau_drm_prop_enum_list {
	u8 gen_mask;
	int type;
	char *name;
};

static struct nouveau_drm_prop_enum_list underscan[] = {
	{ 6, UNDERSCAN_AUTO, "auto" },
	{ 6, UNDERSCAN_OFF, "off" },
	{ 6, UNDERSCAN_ON, "on" },
	{}
};

static struct nouveau_drm_prop_enum_list dither_mode[] = {
	{ 7, DITHERING_MODE_AUTO, "auto" },
	{ 7, DITHERING_MODE_OFF, "off" },
	{ 1, DITHERING_MODE_ON, "on" },
	{ 6, DITHERING_MODE_STATIC2X2, "static 2x2" },
	{ 6, DITHERING_MODE_DYNAMIC2X2, "dynamic 2x2" },
	{ 4, DITHERING_MODE_TEMPORAL, "temporal" },
	{}
};

static struct nouveau_drm_prop_enum_list dither_depth[] = {
	{ 6, DITHERING_DEPTH_AUTO, "auto" },
	{ 6, DITHERING_DEPTH_6BPC, "6 bpc" },
	{ 6, DITHERING_DEPTH_8BPC, "8 bpc" },
	{}
};

#define PROP_ENUM(p,gen,n,list) do {                                           \
	struct nouveau_drm_prop_enum_list *l = (list);                         \
	int c = 0;                                                             \
	while (l->gen_mask) {                                                  \
		if (l->gen_mask & (1 << (gen)))                                \
			c++;                                                   \
		l++;                                                           \
	}                                                                      \
	if (c) {                                                               \
		p = drm_property_create(dev, DRM_MODE_PROP_ENUM, n, c);        \
		l = (list);                                                    \
		c = 0;                                                         \
		while (p && l->gen_mask) {                                     \
			if (l->gen_mask & (1 << (gen))) {                      \
				drm_property_add_enum(p, c, l->type, l->name); \
				c++;                                           \
			}                                                      \
			l++;                                                   \
		}                                                              \
	}                                                                      \
} while(0)

int
nouveau_display_init(struct drm_device *dev)
{
<<<<<<< HEAD
	struct drm_nouveau_private *dev_priv = dev->dev_private;
	struct nouveau_display_engine *disp = &dev_priv->engine.display;
=======
	struct nouveau_display *disp = nouveau_display(dev);
>>>>>>> refs/remotes/origin/master
	struct drm_connector *connector;
	int ret;

	ret = disp->init(dev);
	if (ret)
		return ret;

<<<<<<< HEAD
	/* power on internal panel if it's not already.  the init tables of
	 * some vbios default this to off for some reason, causing the
	 * panel to not work after resume
	 */
	if (nouveau_gpio_func_get(dev, DCB_GPIO_PANEL_POWER) == 0) {
		nouveau_gpio_func_set(dev, DCB_GPIO_PANEL_POWER, true);
		msleep(300);
	}

=======
>>>>>>> refs/remotes/origin/master
	/* enable polling for external displays */
	drm_kms_helper_poll_enable(dev);

	/* enable hotplug interrupts */
	list_for_each_entry(connector, &dev->mode_config.connector_list, head) {
		struct nouveau_connector *conn = nouveau_connector(connector);
<<<<<<< HEAD
		nouveau_gpio_irq(dev, 0, conn->hpd, 0xff, true);
=======
		if (conn->hpd_func) nouveau_event_get(conn->hpd_func);
>>>>>>> refs/remotes/origin/master
	}

	return ret;
}

void
nouveau_display_fini(struct drm_device *dev)
{
<<<<<<< HEAD
	struct drm_nouveau_private *dev_priv = dev->dev_private;
	struct nouveau_display_engine *disp = &dev_priv->engine.display;
=======
	struct nouveau_display *disp = nouveau_display(dev);
>>>>>>> refs/remotes/origin/master
	struct drm_connector *connector;

	/* disable hotplug interrupts */
	list_for_each_entry(connector, &dev->mode_config.connector_list, head) {
		struct nouveau_connector *conn = nouveau_connector(connector);
<<<<<<< HEAD
		nouveau_gpio_irq(dev, 0, conn->hpd, 0xff, false);
=======
		if (conn->hpd_func) nouveau_event_put(conn->hpd_func);
>>>>>>> refs/remotes/origin/master
	}

	drm_kms_helper_poll_disable(dev);
	disp->fini(dev);
}

int
nouveau_display_create(struct drm_device *dev)
{
<<<<<<< HEAD
	struct drm_nouveau_private *dev_priv = dev->dev_private;
	struct nouveau_display_engine *disp = &dev_priv->engine.display;
	int ret, gen;

=======
	struct nouveau_drm *drm = nouveau_drm(dev);
	struct nouveau_display *disp;
	int ret, gen;

	disp = drm->display = kzalloc(sizeof(*disp), GFP_KERNEL);
	if (!disp)
		return -ENOMEM;

>>>>>>> refs/remotes/origin/master
	drm_mode_config_init(dev);
	drm_mode_create_scaling_mode_property(dev);
	drm_mode_create_dvi_i_properties(dev);

<<<<<<< HEAD
	if (dev_priv->card_type < NV_50)
		gen = 0;
	else
	if (dev_priv->card_type < NV_D0)
=======
	if (nv_device(drm->device)->card_type < NV_50)
		gen = 0;
	else
	if (nv_device(drm->device)->card_type < NV_D0)
>>>>>>> refs/remotes/origin/master
		gen = 1;
	else
		gen = 2;

	PROP_ENUM(disp->dithering_mode, gen, "dithering mode", dither_mode);
	PROP_ENUM(disp->dithering_depth, gen, "dithering depth", dither_depth);
	PROP_ENUM(disp->underscan_property, gen, "underscan", underscan);

	disp->underscan_hborder_property =
		drm_property_create_range(dev, 0, "underscan hborder", 0, 128);

	disp->underscan_vborder_property =
		drm_property_create_range(dev, 0, "underscan vborder", 0, 128);

<<<<<<< HEAD
	if (gen == 1) {
		disp->vibrant_hue_property =
			drm_property_create(dev, DRM_MODE_PROP_RANGE,
					    "vibrant hue", 2);
		disp->vibrant_hue_property->values[0] = 0;
		disp->vibrant_hue_property->values[1] = 180; /* -90..+90 */

		disp->color_vibrance_property =
			drm_property_create(dev, DRM_MODE_PROP_RANGE,
					    "color vibrance", 2);
		disp->color_vibrance_property->values[0] = 0;
		disp->color_vibrance_property->values[1] = 200; /* -100..+100 */
	}

	dev->mode_config.funcs = (void *)&nouveau_mode_config_funcs;
=======
	if (gen >= 1) {
		/* -90..+90 */
		disp->vibrant_hue_property =
			drm_property_create_range(dev, 0, "vibrant hue", 0, 180);

		/* -100..+100 */
		disp->color_vibrance_property =
			drm_property_create_range(dev, 0, "color vibrance", 0, 200);
	}

	dev->mode_config.funcs = &nouveau_mode_config_funcs;
>>>>>>> refs/remotes/origin/master
	dev->mode_config.fb_base = pci_resource_start(dev->pdev, 1);

	dev->mode_config.min_width = 0;
	dev->mode_config.min_height = 0;
<<<<<<< HEAD
	if (dev_priv->card_type < NV_10) {
		dev->mode_config.max_width = 2048;
		dev->mode_config.max_height = 2048;
	} else
	if (dev_priv->card_type < NV_50) {
=======
	if (nv_device(drm->device)->card_type < NV_10) {
		dev->mode_config.max_width = 2048;
		dev->mode_config.max_height = 2048;
	} else
	if (nv_device(drm->device)->card_type < NV_50) {
>>>>>>> refs/remotes/origin/master
		dev->mode_config.max_width = 4096;
		dev->mode_config.max_height = 4096;
	} else {
		dev->mode_config.max_width = 8192;
		dev->mode_config.max_height = 8192;
	}

	dev->mode_config.preferred_depth = 24;
	dev->mode_config.prefer_shadow = 1;

<<<<<<< HEAD
	drm_kms_helper_poll_init(dev);
	drm_kms_helper_poll_disable(dev);

	ret = disp->create(dev);
	if (ret)
		return ret;

	if (dev->mode_config.num_crtc) {
		ret = drm_vblank_init(dev, dev->mode_config.num_crtc);
		if (ret)
			return ret;
	}

=======
	if (nv_device(drm->device)->chipset < 0x11)
		dev->mode_config.async_page_flip = false;
	else
		dev->mode_config.async_page_flip = true;

	drm_kms_helper_poll_init(dev);
	drm_kms_helper_poll_disable(dev);

	if (drm->vbios.dcb.entries) {
		if (nv_device(drm->device)->card_type < NV_50)
			ret = nv04_display_create(dev);
		else
			ret = nv50_display_create(dev);
	} else {
		ret = 0;
	}

	if (ret)
		goto disp_create_err;

	if (dev->mode_config.num_crtc) {
		ret = nouveau_display_vblank_init(dev);
		if (ret)
			goto vblank_err;
	}

	nouveau_backlight_init(dev);
	return 0;

vblank_err:
	disp->dtor(dev);
disp_create_err:
	drm_kms_helper_poll_fini(dev);
	drm_mode_config_cleanup(dev);
>>>>>>> refs/remotes/origin/master
	return ret;
}

void
nouveau_display_destroy(struct drm_device *dev)
{
<<<<<<< HEAD
	struct drm_nouveau_private *dev_priv = dev->dev_private;
	struct nouveau_display_engine *disp = &dev_priv->engine.display;

	drm_vblank_cleanup(dev);

	disp->destroy(dev);

	drm_kms_helper_poll_fini(dev);
	drm_mode_config_cleanup(dev);
}

>>>>>>> refs/remotes/origin/cm-10.0
int
nouveau_vblank_enable(struct drm_device *dev, int crtc)
{
	struct drm_nouveau_private *dev_priv = dev->dev_private;

	if (dev_priv->card_type >= NV_50)
		nv_mask(dev, NV50_PDISPLAY_INTR_EN_1, 0,
			NV50_PDISPLAY_INTR_EN_1_VBLANK_CRTC_(crtc));
	else
		NVWriteCRTC(dev, crtc, NV_PCRTC_INTR_EN_0,
			    NV_PCRTC_INTR_0_VBLANK);

	return 0;
}

void
nouveau_vblank_disable(struct drm_device *dev, int crtc)
{
	struct drm_nouveau_private *dev_priv = dev->dev_private;

	if (dev_priv->card_type >= NV_50)
		nv_mask(dev, NV50_PDISPLAY_INTR_EN_1,
			NV50_PDISPLAY_INTR_EN_1_VBLANK_CRTC_(crtc), 0);
	else
		NVWriteCRTC(dev, crtc, NV_PCRTC_INTR_EN_0, 0);
}

static int
nouveau_page_flip_reserve(struct nouveau_bo *old_bo,
			  struct nouveau_bo *new_bo)
{
	int ret;

	ret = nouveau_bo_pin(new_bo, TTM_PL_FLAG_VRAM);
	if (ret)
		return ret;

	ret = ttm_bo_reserve(&new_bo->bo, false, false, false, 0);
	if (ret)
		goto fail;

	ret = ttm_bo_reserve(&old_bo->bo, false, false, false, 0);
	if (ret)
		goto fail_unreserve;

	return 0;

fail_unreserve:
	ttm_bo_unreserve(&new_bo->bo);
fail:
	nouveau_bo_unpin(new_bo);
	return ret;
}

static void
nouveau_page_flip_unreserve(struct nouveau_bo *old_bo,
			    struct nouveau_bo *new_bo,
			    struct nouveau_fence *fence)
{
	nouveau_bo_fence(new_bo, fence);
	ttm_bo_unreserve(&new_bo->bo);

	nouveau_bo_fence(old_bo, fence);
	ttm_bo_unreserve(&old_bo->bo);

	nouveau_bo_unpin(old_bo);
=======
	struct nouveau_display *disp = nouveau_display(dev);

	nouveau_backlight_exit(dev);
	nouveau_display_vblank_fini(dev);

	drm_kms_helper_poll_fini(dev);
	drm_mode_config_cleanup(dev);

	if (disp->dtor)
		disp->dtor(dev);

	nouveau_drm(dev)->display = NULL;
	kfree(disp);
}

int
nouveau_display_suspend(struct drm_device *dev)
{
	struct nouveau_drm *drm = nouveau_drm(dev);
	struct drm_crtc *crtc;

	nouveau_display_fini(dev);

	NV_INFO(drm, "unpinning framebuffer(s)...\n");
	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head) {
		struct nouveau_framebuffer *nouveau_fb;

		nouveau_fb = nouveau_framebuffer(crtc->fb);
		if (!nouveau_fb || !nouveau_fb->nvbo)
			continue;

		nouveau_bo_unpin(nouveau_fb->nvbo);
	}

	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head) {
		struct nouveau_crtc *nv_crtc = nouveau_crtc(crtc);

		nouveau_bo_unmap(nv_crtc->cursor.nvbo);
		nouveau_bo_unpin(nv_crtc->cursor.nvbo);
	}

	return 0;
}

void
nouveau_display_repin(struct drm_device *dev)
{
	struct nouveau_drm *drm = nouveau_drm(dev);
	struct drm_crtc *crtc;
	int ret;

	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head) {
		struct nouveau_framebuffer *nouveau_fb;

		nouveau_fb = nouveau_framebuffer(crtc->fb);
		if (!nouveau_fb || !nouveau_fb->nvbo)
			continue;

		nouveau_bo_pin(nouveau_fb->nvbo, TTM_PL_FLAG_VRAM);
	}

	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head) {
		struct nouveau_crtc *nv_crtc = nouveau_crtc(crtc);

		ret = nouveau_bo_pin(nv_crtc->cursor.nvbo, TTM_PL_FLAG_VRAM);
		if (!ret)
			ret = nouveau_bo_map(nv_crtc->cursor.nvbo);
		if (ret)
			NV_ERROR(drm, "Could not pin/map cursor.\n");
	}
}

void
nouveau_display_resume(struct drm_device *dev)
{
	struct drm_crtc *crtc;
	nouveau_display_init(dev);

	/* Force CLUT to get re-loaded during modeset */
	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head) {
		struct nouveau_crtc *nv_crtc = nouveau_crtc(crtc);

		nv_crtc->lut.depth = 0;
	}

	drm_helper_resume_force_mode(dev);

	list_for_each_entry(crtc, &dev->mode_config.crtc_list, head) {
		struct nouveau_crtc *nv_crtc = nouveau_crtc(crtc);
		u32 offset = nv_crtc->cursor.nvbo->bo.offset;

		nv_crtc->cursor.set_offset(nv_crtc, offset);
		nv_crtc->cursor.set_pos(nv_crtc, nv_crtc->cursor_saved_x,
						 nv_crtc->cursor_saved_y);
	}
>>>>>>> refs/remotes/origin/master
}

static int
nouveau_page_flip_emit(struct nouveau_channel *chan,
		       struct nouveau_bo *old_bo,
		       struct nouveau_bo *new_bo,
		       struct nouveau_page_flip_state *s,
		       struct nouveau_fence **pfence)
{
<<<<<<< HEAD
	struct drm_nouveau_private *dev_priv = chan->dev->dev_private;
	struct drm_device *dev = chan->dev;
=======
	struct nouveau_fence_chan *fctx = chan->fence;
	struct nouveau_drm *drm = chan->drm;
	struct drm_device *dev = drm->dev;
>>>>>>> refs/remotes/origin/master
	unsigned long flags;
	int ret;

	/* Queue it to the pending list */
	spin_lock_irqsave(&dev->event_lock, flags);
<<<<<<< HEAD
	list_add_tail(&s->head, &chan->nvsw.flip);
=======
	list_add_tail(&s->head, &fctx->flip);
>>>>>>> refs/remotes/origin/master
	spin_unlock_irqrestore(&dev->event_lock, flags);

	/* Synchronize with the old framebuffer */
	ret = nouveau_fence_sync(old_bo->bo.sync_obj, chan);
	if (ret)
		goto fail;

	/* Emit the pageflip */
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/master
	ret = RING_SPACE(chan, 2);
	if (ret)
		goto fail;

<<<<<<< HEAD
	if (dev_priv->card_type < NV_C0)
		BEGIN_RING(chan, NvSubSw, NV_SW_PAGE_FLIP, 1);
	else
		BEGIN_NVC0(chan, 2, NvSubM2MF, 0x0500, 1);
	OUT_RING  (chan, 0);
=======
	ret = RING_SPACE(chan, 3);
	if (ret)
		goto fail;

	if (dev_priv->card_type < NV_C0) {
		BEGIN_RING(chan, NvSubSw, NV_SW_PAGE_FLIP, 1);
		OUT_RING  (chan, 0x00000000);
		OUT_RING  (chan, 0x00000000);
	} else {
		BEGIN_NVC0(chan, 2, 0, NV10_SUBCHAN_REF_CNT, 1);
		OUT_RING  (chan, ++chan->fence.sequence);
		BEGIN_NVC0(chan, 8, 0, NVSW_SUBCHAN_PAGE_FLIP, 0x0000);
	}
>>>>>>> refs/remotes/origin/cm-10.0
	FIRE_RING (chan);

	ret = nouveau_fence_new(chan, pfence, true);
=======
	if (nv_device(drm->device)->card_type < NV_C0)
		BEGIN_NV04(chan, NvSubSw, NV_SW_PAGE_FLIP, 1);
	else
		BEGIN_NVC0(chan, FermiSw, NV_SW_PAGE_FLIP, 1);
	OUT_RING  (chan, 0x00000000);
	FIRE_RING (chan);

	ret = nouveau_fence_new(chan, false, pfence);
>>>>>>> refs/remotes/origin/master
	if (ret)
		goto fail;

	return 0;
fail:
	spin_lock_irqsave(&dev->event_lock, flags);
	list_del(&s->head);
	spin_unlock_irqrestore(&dev->event_lock, flags);
	return ret;
}

int
nouveau_crtc_page_flip(struct drm_crtc *crtc, struct drm_framebuffer *fb,
<<<<<<< HEAD
		       struct drm_pending_vblank_event *event)
{
	struct drm_device *dev = crtc->dev;
	struct drm_nouveau_private *dev_priv = dev->dev_private;
	struct nouveau_bo *old_bo = nouveau_framebuffer(crtc->fb)->nvbo;
	struct nouveau_bo *new_bo = nouveau_framebuffer(fb)->nvbo;
	struct nouveau_page_flip_state *s;
	struct nouveau_channel *chan;
	struct nouveau_fence *fence;
	int ret;

	if (!dev_priv->channel)
=======
		       struct drm_pending_vblank_event *event, u32 flags)
{
	const int swap_interval = (flags & DRM_MODE_PAGE_FLIP_ASYNC) ? 0 : 1;
	struct drm_device *dev = crtc->dev;
	struct nouveau_drm *drm = nouveau_drm(dev);
	struct nouveau_bo *old_bo = nouveau_framebuffer(crtc->fb)->nvbo;
	struct nouveau_bo *new_bo = nouveau_framebuffer(fb)->nvbo;
	struct nouveau_page_flip_state *s;
	struct nouveau_channel *chan = drm->channel;
	struct nouveau_fence *fence;
	int ret;

	if (!drm->channel)
>>>>>>> refs/remotes/origin/master
		return -ENODEV;

	s = kzalloc(sizeof(*s), GFP_KERNEL);
	if (!s)
		return -ENOMEM;

<<<<<<< HEAD
	/* Don't let the buffers go away while we flip */
	ret = nouveau_page_flip_reserve(old_bo, new_bo);
	if (ret)
		goto fail_free;

	/* Initialize a page flip struct */
	*s = (struct nouveau_page_flip_state)
		{ { }, event, nouveau_crtc(crtc)->index,
<<<<<<< HEAD
		  fb->bits_per_pixel, fb->pitch, crtc->x, crtc->y,
=======
		  fb->bits_per_pixel, fb->pitches[0], crtc->x, crtc->y,
>>>>>>> refs/remotes/origin/cm-10.0
		  new_bo->bo.offset };

	/* Choose the channel the flip will be handled in */
	chan = nouveau_fence_channel(new_bo->bo.sync_obj);
	if (!chan)
		chan = nouveau_channel_get_unlocked(dev_priv->channel);
	mutex_lock(&chan->mutex);

	/* Emit a page flip */
	if (dev_priv->card_type >= NV_50) {
<<<<<<< HEAD
		ret = nv50_display_flip_next(crtc, fb, chan);
=======
		if (dev_priv->card_type >= NV_D0)
			ret = nvd0_display_flip_next(crtc, fb, chan, 0);
		else
			ret = nv50_display_flip_next(crtc, fb, chan);
>>>>>>> refs/remotes/origin/cm-10.0
		if (ret) {
			nouveau_channel_put(&chan);
			goto fail_unreserve;
		}
	}

	ret = nouveau_page_flip_emit(chan, old_bo, new_bo, s, &fence);
	nouveau_channel_put(&chan);
=======
	/* synchronise rendering channel with the kernel's channel */
	spin_lock(&new_bo->bo.bdev->fence_lock);
	fence = nouveau_fence_ref(new_bo->bo.sync_obj);
	spin_unlock(&new_bo->bo.bdev->fence_lock);
	ret = nouveau_fence_sync(fence, chan);
	nouveau_fence_unref(&fence);
	if (ret)
		goto fail_free;

	if (new_bo != old_bo) {
		ret = nouveau_bo_pin(new_bo, TTM_PL_FLAG_VRAM);
		if (ret)
			goto fail_free;
	}

	mutex_lock(&chan->cli->mutex);
	ret = ttm_bo_reserve(&old_bo->bo, true, false, false, NULL);
	if (ret)
		goto fail_unpin;

	/* Initialize a page flip struct */
	*s = (struct nouveau_page_flip_state)
		{ { }, event, nouveau_crtc(crtc)->index,
		  fb->bits_per_pixel, fb->pitches[0], crtc->x, crtc->y,
		  new_bo->bo.offset };

	/* Emit a page flip */
	if (nv_device(drm->device)->card_type >= NV_50) {
		ret = nv50_display_flip_next(crtc, fb, chan, swap_interval);
		if (ret)
			goto fail_unreserve;
	} else {
		struct nv04_display *dispnv04 = nv04_display(dev);
		int head = nouveau_crtc(crtc)->index;

		if (swap_interval) {
			ret = RING_SPACE(chan, 8);
			if (ret)
				goto fail_unreserve;

			BEGIN_NV04(chan, NvSubImageBlit, 0x012c, 1);
			OUT_RING  (chan, 0);
			BEGIN_NV04(chan, NvSubImageBlit, 0x0134, 1);
			OUT_RING  (chan, head);
			BEGIN_NV04(chan, NvSubImageBlit, 0x0100, 1);
			OUT_RING  (chan, 0);
			BEGIN_NV04(chan, NvSubImageBlit, 0x0130, 1);
			OUT_RING  (chan, 0);
		}

		nouveau_bo_ref(new_bo, &dispnv04->image[head]);
	}

	ret = nouveau_page_flip_emit(chan, old_bo, new_bo, s, &fence);
	mutex_unlock(&chan->cli->mutex);
>>>>>>> refs/remotes/origin/master
	if (ret)
		goto fail_unreserve;

	/* Update the crtc struct and cleanup */
	crtc->fb = fb;

<<<<<<< HEAD
	nouveau_page_flip_unreserve(old_bo, new_bo, fence);
=======
	nouveau_bo_fence(old_bo, fence);
	ttm_bo_unreserve(&old_bo->bo);
	if (old_bo != new_bo)
		nouveau_bo_unpin(old_bo);
>>>>>>> refs/remotes/origin/master
	nouveau_fence_unref(&fence);
	return 0;

fail_unreserve:
<<<<<<< HEAD
	nouveau_page_flip_unreserve(old_bo, new_bo, NULL);
=======
	ttm_bo_unreserve(&old_bo->bo);
fail_unpin:
	mutex_unlock(&chan->cli->mutex);
	if (old_bo != new_bo)
		nouveau_bo_unpin(new_bo);
>>>>>>> refs/remotes/origin/master
fail_free:
	kfree(s);
	return ret;
}

int
nouveau_finish_page_flip(struct nouveau_channel *chan,
			 struct nouveau_page_flip_state *ps)
{
<<<<<<< HEAD
	struct drm_device *dev = chan->dev;
=======
	struct nouveau_fence_chan *fctx = chan->fence;
	struct nouveau_drm *drm = chan->drm;
	struct drm_device *dev = drm->dev;
>>>>>>> refs/remotes/origin/master
	struct nouveau_page_flip_state *s;
	unsigned long flags;

	spin_lock_irqsave(&dev->event_lock, flags);

<<<<<<< HEAD
	if (list_empty(&chan->nvsw.flip)) {
		NV_ERROR(dev, "Unexpected pageflip in channel %d.\n", chan->id);
=======
	if (list_empty(&fctx->flip)) {
		NV_ERROR(drm, "unexpected pageflip\n");
>>>>>>> refs/remotes/origin/master
		spin_unlock_irqrestore(&dev->event_lock, flags);
		return -EINVAL;
	}

<<<<<<< HEAD
	s = list_first_entry(&chan->nvsw.flip,
			     struct nouveau_page_flip_state, head);
	if (s->event) {
		struct drm_pending_vblank_event *e = s->event;
		struct timeval now;

		do_gettimeofday(&now);
		e->event.sequence = 0;
		e->event.tv_sec = now.tv_sec;
		e->event.tv_usec = now.tv_usec;
		list_add_tail(&e->base.link, &e->base.file_priv->event_list);
		wake_up_interruptible(&e->base.file_priv->event_wait);
	}
=======
	s = list_first_entry(&fctx->flip, struct nouveau_page_flip_state, head);
	if (s->event)
		drm_send_vblank_event(dev, s->crtc, s->event);
>>>>>>> refs/remotes/origin/master

	list_del(&s->head);
	if (ps)
		*ps = *s;
	kfree(s);

	spin_unlock_irqrestore(&dev->event_lock, flags);
	return 0;
}
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======

int
nouveau_flip_complete(void *data)
{
	struct nouveau_channel *chan = data;
	struct nouveau_drm *drm = chan->drm;
	struct nouveau_page_flip_state state;

	if (!nouveau_finish_page_flip(chan, &state)) {
		if (nv_device(drm->device)->card_type < NV_50) {
			nv_set_crtc_base(drm->dev, state.crtc, state.offset +
					 state.y * state.pitch +
					 state.x * state.bpp / 8);
		}
	}

	return 0;
}
>>>>>>> refs/remotes/origin/master

int
nouveau_display_dumb_create(struct drm_file *file_priv, struct drm_device *dev,
			    struct drm_mode_create_dumb *args)
{
	struct nouveau_bo *bo;
	int ret;

	args->pitch = roundup(args->width * (args->bpp / 8), 256);
	args->size = args->pitch * args->height;
	args->size = roundup(args->size, PAGE_SIZE);

	ret = nouveau_gem_new(dev, args->size, 0, NOUVEAU_GEM_DOMAIN_VRAM, 0, 0, &bo);
	if (ret)
		return ret;

<<<<<<< HEAD
	ret = drm_gem_handle_create(file_priv, bo->gem, &args->handle);
	drm_gem_object_unreference_unlocked(bo->gem);
=======
	ret = drm_gem_handle_create(file_priv, &bo->gem, &args->handle);
	drm_gem_object_unreference_unlocked(&bo->gem);
>>>>>>> refs/remotes/origin/master
	return ret;
}

int
<<<<<<< HEAD
nouveau_display_dumb_destroy(struct drm_file *file_priv, struct drm_device *dev,
			     uint32_t handle)
{
	return drm_gem_handle_delete(file_priv, handle);
}

int
=======
>>>>>>> refs/remotes/origin/master
nouveau_display_dumb_map_offset(struct drm_file *file_priv,
				struct drm_device *dev,
				uint32_t handle, uint64_t *poffset)
{
	struct drm_gem_object *gem;

	gem = drm_gem_object_lookup(dev, file_priv, handle);
	if (gem) {
<<<<<<< HEAD
		struct nouveau_bo *bo = gem->driver_private;
		*poffset = bo->bo.addr_space_offset;
=======
		struct nouveau_bo *bo = nouveau_gem_object(gem);
		*poffset = drm_vma_node_offset_addr(&bo->bo.vma_node);
>>>>>>> refs/remotes/origin/master
		drm_gem_object_unreference_unlocked(gem);
		return 0;
	}

	return -ENOENT;
}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
