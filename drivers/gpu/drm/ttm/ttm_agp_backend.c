/**************************************************************************
 *
 * Copyright (c) 2006-2009 VMware, Inc., Palo Alto, CA., USA
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS, AUTHORS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 **************************************************************************/
/*
 * Authors: Thomas Hellstrom <thellstrom-at-vmware-dot-com>
 *          Keith Packard.
 */

<<<<<<< HEAD
<<<<<<< HEAD
#include "ttm/ttm_module.h"
#include "ttm/ttm_bo_driver.h"
=======
#define pr_fmt(fmt) "[TTM] " fmt

#include "ttm/ttm_module.h"
#include "ttm/ttm_bo_driver.h"
#include "ttm/ttm_page_alloc.h"
>>>>>>> refs/remotes/origin/cm-10.0
#ifdef TTM_HAS_AGP
#include "ttm/ttm_placement.h"
=======
#define pr_fmt(fmt) "[TTM] " fmt

#include <drm/ttm/ttm_module.h>
#include <drm/ttm/ttm_bo_driver.h>
#include <drm/ttm/ttm_page_alloc.h>
#ifdef TTM_HAS_AGP
#include <drm/ttm/ttm_placement.h>
>>>>>>> refs/remotes/origin/master
#include <linux/agp_backend.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <asm/agp.h>

struct ttm_agp_backend {
<<<<<<< HEAD
<<<<<<< HEAD
	struct ttm_backend backend;
=======
	struct ttm_tt ttm;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct ttm_tt ttm;
>>>>>>> refs/remotes/origin/master
	struct agp_memory *mem;
	struct agp_bridge_data *bridge;
};

<<<<<<< HEAD
<<<<<<< HEAD
static int ttm_agp_populate(struct ttm_backend *backend,
			    unsigned long num_pages, struct page **pages,
			    struct page *dummy_read_page,
			    dma_addr_t *dma_addrs)
{
	struct ttm_agp_backend *agp_be =
	    container_of(backend, struct ttm_agp_backend, backend);
	struct page **cur_page, **last_page = pages + num_pages;
	struct agp_memory *mem;

	mem = agp_allocate_memory(agp_be->bridge, num_pages, AGP_USER_MEMORY);
=======
=======
>>>>>>> refs/remotes/origin/master
static int ttm_agp_bind(struct ttm_tt *ttm, struct ttm_mem_reg *bo_mem)
{
	struct ttm_agp_backend *agp_be = container_of(ttm, struct ttm_agp_backend, ttm);
	struct drm_mm_node *node = bo_mem->mm_node;
	struct agp_memory *mem;
	int ret, cached = (bo_mem->placement & TTM_PL_FLAG_CACHED);
	unsigned i;

	mem = agp_allocate_memory(agp_be->bridge, ttm->num_pages, AGP_USER_MEMORY);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (unlikely(mem == NULL))
		return -ENOMEM;

	mem->page_count = 0;
<<<<<<< HEAD
<<<<<<< HEAD
	for (cur_page = pages; cur_page < last_page; ++cur_page) {
		struct page *page = *cur_page;
		if (!page)
			page = dummy_read_page;
=======
=======
>>>>>>> refs/remotes/origin/master
	for (i = 0; i < ttm->num_pages; i++) {
		struct page *page = ttm->pages[i];

		if (!page)
			page = ttm->dummy_read_page;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

		mem->pages[mem->page_count++] = page;
	}
	agp_be->mem = mem;
<<<<<<< HEAD
<<<<<<< HEAD
	return 0;
}

static int ttm_agp_bind(struct ttm_backend *backend, struct ttm_mem_reg *bo_mem)
{
	struct ttm_agp_backend *agp_be =
	    container_of(backend, struct ttm_agp_backend, backend);
	struct drm_mm_node *node = bo_mem->mm_node;
	struct agp_memory *mem = agp_be->mem;
	int cached = (bo_mem->placement & TTM_PL_FLAG_CACHED);
	int ret;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	mem->is_flushed = 1;
	mem->type = (cached) ? AGP_USER_CACHED_MEMORY : AGP_USER_MEMORY;

	ret = agp_bind_memory(mem, node->start);
	if (ret)
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR TTM_PFX "AGP Bind memory failed.\n");
=======
		pr_err("AGP Bind memory failed\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_err("AGP Bind memory failed\n");
>>>>>>> refs/remotes/origin/master

	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int ttm_agp_unbind(struct ttm_backend *backend)
{
	struct ttm_agp_backend *agp_be =
	    container_of(backend, struct ttm_agp_backend, backend);

	if (agp_be->mem->is_bound)
		return agp_unbind_memory(agp_be->mem);
	else
		return 0;
}

static void ttm_agp_clear(struct ttm_backend *backend)
{
	struct ttm_agp_backend *agp_be =
	    container_of(backend, struct ttm_agp_backend, backend);
	struct agp_memory *mem = agp_be->mem;

	if (mem) {
		ttm_agp_unbind(backend);
		agp_free_memory(mem);
	}
	agp_be->mem = NULL;
}

static void ttm_agp_destroy(struct ttm_backend *backend)
{
	struct ttm_agp_backend *agp_be =
	    container_of(backend, struct ttm_agp_backend, backend);

	if (agp_be->mem)
		ttm_agp_clear(backend);
=======
=======
>>>>>>> refs/remotes/origin/master
static int ttm_agp_unbind(struct ttm_tt *ttm)
{
	struct ttm_agp_backend *agp_be = container_of(ttm, struct ttm_agp_backend, ttm);

	if (agp_be->mem) {
		if (agp_be->mem->is_bound)
			return agp_unbind_memory(agp_be->mem);
		agp_free_memory(agp_be->mem);
		agp_be->mem = NULL;
	}
	return 0;
}

static void ttm_agp_destroy(struct ttm_tt *ttm)
{
	struct ttm_agp_backend *agp_be = container_of(ttm, struct ttm_agp_backend, ttm);

	if (agp_be->mem)
		ttm_agp_unbind(ttm);
	ttm_tt_fini(ttm);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	kfree(agp_be);
}

static struct ttm_backend_func ttm_agp_func = {
<<<<<<< HEAD
<<<<<<< HEAD
	.populate = ttm_agp_populate,
	.clear = ttm_agp_clear,
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	.bind = ttm_agp_bind,
	.unbind = ttm_agp_unbind,
	.destroy = ttm_agp_destroy,
};

<<<<<<< HEAD
<<<<<<< HEAD
struct ttm_backend *ttm_agp_backend_init(struct ttm_bo_device *bdev,
					 struct agp_bridge_data *bridge)
=======
=======
>>>>>>> refs/remotes/origin/master
struct ttm_tt *ttm_agp_tt_create(struct ttm_bo_device *bdev,
				 struct agp_bridge_data *bridge,
				 unsigned long size, uint32_t page_flags,
				 struct page *dummy_read_page)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	struct ttm_agp_backend *agp_be;

	agp_be = kmalloc(sizeof(*agp_be), GFP_KERNEL);
	if (!agp_be)
		return NULL;

	agp_be->mem = NULL;
	agp_be->bridge = bridge;
<<<<<<< HEAD
<<<<<<< HEAD
	agp_be->backend.func = &ttm_agp_func;
	agp_be->backend.bdev = bdev;
	return &agp_be->backend;
}
EXPORT_SYMBOL(ttm_agp_backend_init);
=======
=======
>>>>>>> refs/remotes/origin/master
	agp_be->ttm.func = &ttm_agp_func;

	if (ttm_tt_init(&agp_be->ttm, bdev, size, page_flags, dummy_read_page)) {
		return NULL;
	}

	return &agp_be->ttm;
}
EXPORT_SYMBOL(ttm_agp_tt_create);

int ttm_agp_tt_populate(struct ttm_tt *ttm)
{
	if (ttm->state != tt_unpopulated)
		return 0;

	return ttm_pool_populate(ttm);
}
EXPORT_SYMBOL(ttm_agp_tt_populate);

void ttm_agp_tt_unpopulate(struct ttm_tt *ttm)
{
	ttm_pool_unpopulate(ttm);
}
EXPORT_SYMBOL(ttm_agp_tt_unpopulate);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#endif
