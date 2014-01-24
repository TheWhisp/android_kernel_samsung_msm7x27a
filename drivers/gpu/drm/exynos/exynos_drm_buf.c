/* exynos_drm_buf.c
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
 */

#include "drmP.h"
#include "drm.h"
#include "exynos_drm.h"
=======
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <drm/drmP.h>
#include <drm/exynos_drm.h>
>>>>>>> refs/remotes/origin/master

#include "exynos_drm_drv.h"
#include "exynos_drm_gem.h"
#include "exynos_drm_buf.h"
<<<<<<< HEAD
=======
#include "exynos_drm_iommu.h"
>>>>>>> refs/remotes/origin/master

static int lowlevel_buffer_allocate(struct drm_device *dev,
		unsigned int flags, struct exynos_drm_gem_buf *buf)
{
<<<<<<< HEAD
	dma_addr_t start_addr;
	unsigned int npages, page_size, i = 0;
	struct scatterlist *sgl;
	int ret = 0;

	DRM_DEBUG_KMS("%s\n", __FILE__);

	if (IS_NONCONTIG_BUFFER(flags)) {
		DRM_DEBUG_KMS("not support allocation type.\n");
		return -EINVAL;
	}
=======
	int ret = 0;
	enum dma_attr attr;
	unsigned int nr_pages;
>>>>>>> refs/remotes/origin/master

	if (buf->dma_addr) {
		DRM_DEBUG_KMS("already allocated.\n");
		return 0;
	}

<<<<<<< HEAD
	if (buf->size >= SZ_1M) {
		npages = buf->size >> SECTION_SHIFT;
		page_size = SECTION_SIZE;
	} else if (buf->size >= SZ_64K) {
		npages = buf->size >> 16;
		page_size = SZ_64K;
	} else {
		npages = buf->size >> PAGE_SHIFT;
		page_size = PAGE_SIZE;
	}

	buf->sgt = kzalloc(sizeof(struct sg_table), GFP_KERNEL);
	if (!buf->sgt) {
		DRM_ERROR("failed to allocate sg table.\n");
		return -ENOMEM;
	}

	ret = sg_alloc_table(buf->sgt, npages, GFP_KERNEL);
	if (ret < 0) {
		DRM_ERROR("failed to initialize sg table.\n");
		kfree(buf->sgt);
		buf->sgt = NULL;
		return -ENOMEM;
	}

	buf->kvaddr = dma_alloc_writecombine(dev->dev, buf->size,
			&buf->dma_addr, GFP_KERNEL);
	if (!buf->kvaddr) {
		DRM_ERROR("failed to allocate buffer.\n");
		ret = -ENOMEM;
		goto err1;
	}

	buf->pages = kzalloc(sizeof(struct page) * npages, GFP_KERNEL);
	if (!buf->pages) {
		DRM_ERROR("failed to allocate pages.\n");
		ret = -ENOMEM;
		goto err2;
	}

	sgl = buf->sgt->sgl;
	start_addr = buf->dma_addr;

	while (i < npages) {
		buf->pages[i] = phys_to_page(start_addr);
		sg_set_page(sgl, buf->pages[i], page_size, 0);
		sg_dma_address(sgl) = start_addr;
		start_addr += page_size;
		sgl = sg_next(sgl);
		i++;
	}

	DRM_DEBUG_KMS("vaddr(0x%lx), dma_addr(0x%lx), size(0x%lx)\n",
			(unsigned long)buf->kvaddr,
=======
	init_dma_attrs(&buf->dma_attrs);

	/*
	 * if EXYNOS_BO_CONTIG, fully physically contiguous memory
	 * region will be allocated else physically contiguous
	 * as possible.
	 */
	if (!(flags & EXYNOS_BO_NONCONTIG))
		dma_set_attr(DMA_ATTR_FORCE_CONTIGUOUS, &buf->dma_attrs);

	/*
	 * if EXYNOS_BO_WC or EXYNOS_BO_NONCACHABLE, writecombine mapping
	 * else cachable mapping.
	 */
	if (flags & EXYNOS_BO_WC || !(flags & EXYNOS_BO_CACHABLE))
		attr = DMA_ATTR_WRITE_COMBINE;
	else
		attr = DMA_ATTR_NON_CONSISTENT;

	dma_set_attr(attr, &buf->dma_attrs);
	dma_set_attr(DMA_ATTR_NO_KERNEL_MAPPING, &buf->dma_attrs);

	nr_pages = buf->size >> PAGE_SHIFT;

	if (!is_drm_iommu_supported(dev)) {
		dma_addr_t start_addr;
		unsigned int i = 0;

		buf->pages = drm_calloc_large(nr_pages, sizeof(struct page *));
		if (!buf->pages) {
			DRM_ERROR("failed to allocate pages.\n");
			return -ENOMEM;
		}

		buf->kvaddr = (void __iomem *)dma_alloc_attrs(dev->dev,
					buf->size,
					&buf->dma_addr, GFP_KERNEL,
					&buf->dma_attrs);
		if (!buf->kvaddr) {
			DRM_ERROR("failed to allocate buffer.\n");
			ret = -ENOMEM;
			goto err_free;
		}

		start_addr = buf->dma_addr;
		while (i < nr_pages) {
			buf->pages[i] = phys_to_page(start_addr);
			start_addr += PAGE_SIZE;
			i++;
		}
	} else {

		buf->pages = dma_alloc_attrs(dev->dev, buf->size,
					&buf->dma_addr, GFP_KERNEL,
					&buf->dma_attrs);
		if (!buf->pages) {
			DRM_ERROR("failed to allocate buffer.\n");
			return -ENOMEM;
		}
	}

	buf->sgt = drm_prime_pages_to_sg(buf->pages, nr_pages);
	if (IS_ERR(buf->sgt)) {
		DRM_ERROR("failed to get sg table.\n");
		ret = PTR_ERR(buf->sgt);
		goto err_free_attrs;
	}

	DRM_DEBUG_KMS("dma_addr(0x%lx), size(0x%lx)\n",
>>>>>>> refs/remotes/origin/master
			(unsigned long)buf->dma_addr,
			buf->size);

	return ret;
<<<<<<< HEAD
err2:
	dma_free_writecombine(dev->dev, buf->size, buf->kvaddr,
			(dma_addr_t)buf->dma_addr);
	buf->dma_addr = (dma_addr_t)NULL;
err1:
	sg_free_table(buf->sgt);
	kfree(buf->sgt);
	buf->sgt = NULL;
=======

err_free_attrs:
	dma_free_attrs(dev->dev, buf->size, buf->pages,
			(dma_addr_t)buf->dma_addr, &buf->dma_attrs);
	buf->dma_addr = (dma_addr_t)NULL;
err_free:
	if (!is_drm_iommu_supported(dev))
		drm_free_large(buf->pages);
>>>>>>> refs/remotes/origin/master

	return ret;
}

static void lowlevel_buffer_deallocate(struct drm_device *dev,
		unsigned int flags, struct exynos_drm_gem_buf *buf)
{
<<<<<<< HEAD
	DRM_DEBUG_KMS("%s.\n", __FILE__);

	/*
	 * release only physically continuous memory and
	 * non-continuous memory would be released by exynos
	 * gem framework.
	 */
	if (IS_NONCONTIG_BUFFER(flags)) {
		DRM_DEBUG_KMS("not support allocation type.\n");
		return;
	}

=======
>>>>>>> refs/remotes/origin/master
	if (!buf->dma_addr) {
		DRM_DEBUG_KMS("dma_addr is invalid.\n");
		return;
	}

<<<<<<< HEAD
	DRM_DEBUG_KMS("vaddr(0x%lx), dma_addr(0x%lx), size(0x%lx)\n",
			(unsigned long)buf->kvaddr,
=======
	DRM_DEBUG_KMS("dma_addr(0x%lx), size(0x%lx)\n",
>>>>>>> refs/remotes/origin/master
			(unsigned long)buf->dma_addr,
			buf->size);

	sg_free_table(buf->sgt);

	kfree(buf->sgt);
	buf->sgt = NULL;

<<<<<<< HEAD
	kfree(buf->pages);
	buf->pages = NULL;

	dma_free_writecombine(dev->dev, buf->size, buf->kvaddr,
				(dma_addr_t)buf->dma_addr);
=======
	if (!is_drm_iommu_supported(dev)) {
		dma_free_attrs(dev->dev, buf->size, buf->kvaddr,
				(dma_addr_t)buf->dma_addr, &buf->dma_attrs);
		drm_free_large(buf->pages);
	} else
		dma_free_attrs(dev->dev, buf->size, buf->pages,
				(dma_addr_t)buf->dma_addr, &buf->dma_attrs);

>>>>>>> refs/remotes/origin/master
	buf->dma_addr = (dma_addr_t)NULL;
}

struct exynos_drm_gem_buf *exynos_drm_init_buf(struct drm_device *dev,
						unsigned int size)
{
	struct exynos_drm_gem_buf *buffer;

<<<<<<< HEAD
	DRM_DEBUG_KMS("%s.\n", __FILE__);
	DRM_DEBUG_KMS("desired size = 0x%x\n", size);

	buffer = kzalloc(sizeof(*buffer), GFP_KERNEL);
	if (!buffer) {
		DRM_ERROR("failed to allocate exynos_drm_gem_buf.\n");
		return NULL;
	}
=======
	DRM_DEBUG_KMS("desired size = 0x%x\n", size);

	buffer = kzalloc(sizeof(*buffer), GFP_KERNEL);
	if (!buffer)
		return NULL;
>>>>>>> refs/remotes/origin/master

	buffer->size = size;
	return buffer;
}

void exynos_drm_fini_buf(struct drm_device *dev,
				struct exynos_drm_gem_buf *buffer)
{
<<<<<<< HEAD
	DRM_DEBUG_KMS("%s.\n", __FILE__);

	if (!buffer) {
		DRM_DEBUG_KMS("buffer is null.\n");
		return;
	}

=======
>>>>>>> refs/remotes/origin/master
	kfree(buffer);
	buffer = NULL;
}

int exynos_drm_alloc_buf(struct drm_device *dev,
		struct exynos_drm_gem_buf *buf, unsigned int flags)
{

	/*
	 * allocate memory region and set the memory information
	 * to vaddr and dma_addr of a buffer object.
	 */
	if (lowlevel_buffer_allocate(dev, flags, buf) < 0)
		return -ENOMEM;

	return 0;
}

void exynos_drm_free_buf(struct drm_device *dev,
		unsigned int flags, struct exynos_drm_gem_buf *buffer)
{

	lowlevel_buffer_deallocate(dev, flags, buffer);
}
