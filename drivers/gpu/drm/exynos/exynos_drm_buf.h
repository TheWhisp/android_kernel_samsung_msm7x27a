/* exynos_drm_buf.h
 *
 * Copyright (c) 2011 Samsung Electronics Co., Ltd.
 * Author: Inki Dae <inki.dae@samsung.com>
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

#ifndef _EXYNOS_DRM_BUF_H_
#define _EXYNOS_DRM_BUF_H_

/* create and initialize buffer object. */
struct exynos_drm_gem_buf *exynos_drm_init_buf(struct drm_device *dev,
						unsigned int size);

/* destroy buffer object. */
void exynos_drm_fini_buf(struct drm_device *dev,
				struct exynos_drm_gem_buf *buffer);

<<<<<<< HEAD
/* allocate physical memory region and setup sgt and pages. */
=======
/* allocate physical memory region and setup sgt. */
>>>>>>> refs/remotes/origin/master
int exynos_drm_alloc_buf(struct drm_device *dev,
				struct exynos_drm_gem_buf *buf,
				unsigned int flags);

<<<<<<< HEAD
/* release physical memory region, sgt and pages. */
=======
/* release physical memory region, and sgt. */
>>>>>>> refs/remotes/origin/master
void exynos_drm_free_buf(struct drm_device *dev,
				unsigned int flags,
				struct exynos_drm_gem_buf *buffer);

#endif
