/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd.
 * Authors:
 *	Inki Dae <inki.dae@samsung.com>
 *	Joonyoung Shim <jy0922.shim@samsung.com>
 *	Seung-Woo Kim <sw0312.kim@samsung.com>
 *
<<<<<<< HEAD
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
 * VA LINUX SYSTEMS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
=======
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
>>>>>>> refs/remotes/origin/master
 */

#ifndef _EXYNOS_DRM_ENCODER_H_
#define _EXYNOS_DRM_ENCODER_H_

struct exynos_drm_manager;

void exynos_drm_encoder_setup(struct drm_device *dev);
struct drm_encoder *exynos_drm_encoder_create(struct drm_device *dev,
					       struct exynos_drm_manager *mgr,
					       unsigned int possible_crtcs);
struct exynos_drm_manager *
exynos_drm_get_manager(struct drm_encoder *encoder);
void exynos_drm_fn_encoder(struct drm_crtc *crtc, void *data,
			    void (*fn)(struct drm_encoder *, void *));
void exynos_drm_enable_vblank(struct drm_encoder *encoder, void *data);
void exynos_drm_disable_vblank(struct drm_encoder *encoder, void *data);
<<<<<<< HEAD
void exynos_drm_encoder_crtc_plane_commit(struct drm_encoder *encoder,
					  void *data);
void exynos_drm_encoder_crtc_commit(struct drm_encoder *encoder, void *data);
void exynos_drm_encoder_dpms_from_crtc(struct drm_encoder *encoder,
					void *data);
void exynos_drm_encoder_crtc_dpms(struct drm_encoder *encoder, void *data);
void exynos_drm_encoder_crtc_mode_set(struct drm_encoder *encoder, void *data);
void exynos_drm_encoder_crtc_disable(struct drm_encoder *encoder, void *data);
=======
void exynos_drm_encoder_crtc_dpms(struct drm_encoder *encoder, void *data);
void exynos_drm_encoder_crtc_pipe(struct drm_encoder *encoder, void *data);
void exynos_drm_encoder_plane_mode_set(struct drm_encoder *encoder, void *data);
void exynos_drm_encoder_plane_commit(struct drm_encoder *encoder, void *data);
void exynos_drm_encoder_plane_enable(struct drm_encoder *encoder, void *data);
void exynos_drm_encoder_plane_disable(struct drm_encoder *encoder, void *data);
void exynos_drm_encoder_complete_scanout(struct drm_framebuffer *fb);
>>>>>>> refs/remotes/origin/master

#endif
