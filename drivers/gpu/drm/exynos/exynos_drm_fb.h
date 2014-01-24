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

#ifndef _EXYNOS_DRM_FB_H_
#define _EXYNOS_DRM_FB_H

<<<<<<< HEAD
static inline int exynos_drm_format_num_buffers(uint32_t format)
{
	switch (format) {
	case DRM_FORMAT_NV12M:
	case DRM_FORMAT_NV12MT:
		return 2;
	case DRM_FORMAT_YUV420M:
		return 3;
	default:
		return 1;
	}
}

=======
>>>>>>> refs/remotes/origin/master
struct drm_framebuffer *
exynos_drm_framebuffer_init(struct drm_device *dev,
			    struct drm_mode_fb_cmd2 *mode_cmd,
			    struct drm_gem_object *obj);

/* get memory information of a drm framebuffer */
struct exynos_drm_gem_buf *exynos_drm_fb_buffer(struct drm_framebuffer *fb,
						 int index);

void exynos_drm_mode_config_init(struct drm_device *dev);

<<<<<<< HEAD
=======
/* set a buffer count to drm framebuffer. */
void exynos_drm_fb_set_buf_cnt(struct drm_framebuffer *fb,
						unsigned int cnt);

/* get a buffer count to drm framebuffer. */
unsigned int exynos_drm_fb_get_buf_cnt(struct drm_framebuffer *fb);

>>>>>>> refs/remotes/origin/master
#endif
