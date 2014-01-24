/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2000  Ani Joshi <ajoshi@unixbox.com>
 * Copyright (C) 2000, 2001, 06  Ralf Baechle <ralf@linux-mips.org>
 * swiped from i386, and cloned for MIPS by Geert, polished by Ralf.
 */

#include <linux/types.h>
#include <linux/dma-mapping.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/scatterlist.h>
#include <linux/string.h>
#include <linux/gfp.h>
<<<<<<< HEAD
=======
#include <linux/highmem.h>
>>>>>>> refs/remotes/origin/cm-10.0

#include <asm/cache.h>
#include <asm/io.h>

#include <dma-coherence.h>

<<<<<<< HEAD
static inline unsigned long dma_addr_to_virt(struct device *dev,
	dma_addr_t dma_addr)
{
	unsigned long addr = plat_dma_addr_to_phys(dev, dma_addr);

	return (unsigned long)phys_to_virt(addr);
}

/*
 * The affected CPUs below in 'cpu_needs_post_dma_flush()' can
 * speculatively fill random cachelines with stale data at any time,
 * requiring an extra flush post-DMA.
 *
=======
static inline struct page *dma_addr_to_page(struct device *dev,
	dma_addr_t dma_addr)
{
	return pfn_to_page(
		plat_dma_addr_to_phys(dev, dma_addr) >> PAGE_SHIFT);
}

/*
>>>>>>> refs/remotes/origin/cm-10.0
 * Warning on the terminology - Linux calls an uncached area coherent;
 * MIPS terminology calls memory areas with hardware maintained coherency
 * coherent.
 */
<<<<<<< HEAD
static inline int cpu_needs_post_dma_flush(struct device *dev)
{
	return !plat_device_is_coherent(dev) &&
	       (current_cpu_type() == CPU_R10000 ||
		current_cpu_type() == CPU_R12000 ||
		current_cpu_type() == CPU_BMIPS5000);
=======

static inline int cpu_is_noncoherent_r10000(struct device *dev)
{
	return !plat_device_is_coherent(dev) &&
	       (current_cpu_type() == CPU_R10000 ||
	       current_cpu_type() == CPU_R12000);
>>>>>>> refs/remotes/origin/cm-10.0
}

static gfp_t massage_gfp_flags(const struct device *dev, gfp_t gfp)
{
	gfp_t dma_flag;

	/* ignore region specifiers */
	gfp &= ~(__GFP_DMA | __GFP_DMA32 | __GFP_HIGHMEM);

#ifdef CONFIG_ISA
	if (dev == NULL)
		dma_flag = __GFP_DMA;
	else
#endif
#if defined(CONFIG_ZONE_DMA32) && defined(CONFIG_ZONE_DMA)
	     if (dev->coherent_dma_mask < DMA_BIT_MASK(32))
			dma_flag = __GFP_DMA;
	else if (dev->coherent_dma_mask < DMA_BIT_MASK(64))
			dma_flag = __GFP_DMA32;
	else
#endif
#if defined(CONFIG_ZONE_DMA32) && !defined(CONFIG_ZONE_DMA)
	     if (dev->coherent_dma_mask < DMA_BIT_MASK(64))
		dma_flag = __GFP_DMA32;
	else
#endif
#if defined(CONFIG_ZONE_DMA) && !defined(CONFIG_ZONE_DMA32)
	     if (dev->coherent_dma_mask < DMA_BIT_MASK(64))
		dma_flag = __GFP_DMA;
	else
#endif
		dma_flag = 0;

	/* Don't invoke OOM killer */
	gfp |= __GFP_NORETRY;

	return gfp | dma_flag;
}

void *dma_alloc_noncoherent(struct device *dev, size_t size,
	dma_addr_t * dma_handle, gfp_t gfp)
{
	void *ret;

	gfp = massage_gfp_flags(dev, gfp);

	ret = (void *) __get_free_pages(gfp, get_order(size));

	if (ret != NULL) {
		memset(ret, 0, size);
		*dma_handle = plat_map_dma_mem(dev, ret, size);
	}

	return ret;
}
EXPORT_SYMBOL(dma_alloc_noncoherent);

static void *mips_dma_alloc_coherent(struct device *dev, size_t size,
<<<<<<< HEAD
	dma_addr_t * dma_handle, gfp_t gfp)
=======
	dma_addr_t * dma_handle, gfp_t gfp, struct dma_attrs *attrs)
>>>>>>> refs/remotes/origin/cm-10.0
{
	void *ret;

	if (dma_alloc_from_coherent(dev, size, dma_handle, &ret))
		return ret;

	gfp = massage_gfp_flags(dev, gfp);

	ret = (void *) __get_free_pages(gfp, get_order(size));

	if (ret) {
		memset(ret, 0, size);
		*dma_handle = plat_map_dma_mem(dev, ret, size);

		if (!plat_device_is_coherent(dev)) {
			dma_cache_wback_inv((unsigned long) ret, size);
			ret = UNCAC_ADDR(ret);
		}
	}

	return ret;
}


void dma_free_noncoherent(struct device *dev, size_t size, void *vaddr,
	dma_addr_t dma_handle)
{
	plat_unmap_dma_mem(dev, dma_handle, size, DMA_BIDIRECTIONAL);
	free_pages((unsigned long) vaddr, get_order(size));
}
EXPORT_SYMBOL(dma_free_noncoherent);

static void mips_dma_free_coherent(struct device *dev, size_t size, void *vaddr,
<<<<<<< HEAD
	dma_addr_t dma_handle)
=======
	dma_addr_t dma_handle, struct dma_attrs *attrs)
>>>>>>> refs/remotes/origin/cm-10.0
{
	unsigned long addr = (unsigned long) vaddr;
	int order = get_order(size);

	if (dma_release_from_coherent(dev, order, vaddr))
		return;

	plat_unmap_dma_mem(dev, dma_handle, size, DMA_BIDIRECTIONAL);

	if (!plat_device_is_coherent(dev))
		addr = CAC_ADDR(addr);

	free_pages(addr, get_order(size));
}

<<<<<<< HEAD
static inline void __dma_sync(unsigned long addr, size_t size,
=======
static inline void __dma_sync_virtual(void *addr, size_t size,
>>>>>>> refs/remotes/origin/cm-10.0
	enum dma_data_direction direction)
{
	switch (direction) {
	case DMA_TO_DEVICE:
<<<<<<< HEAD
		dma_cache_wback(addr, size);
		break;

	case DMA_FROM_DEVICE:
		dma_cache_inv(addr, size);
		break;

	case DMA_BIDIRECTIONAL:
		dma_cache_wback_inv(addr, size);
=======
		dma_cache_wback((unsigned long)addr, size);
		break;

	case DMA_FROM_DEVICE:
		dma_cache_inv((unsigned long)addr, size);
		break;

	case DMA_BIDIRECTIONAL:
		dma_cache_wback_inv((unsigned long)addr, size);
>>>>>>> refs/remotes/origin/cm-10.0
		break;

	default:
		BUG();
	}
}

<<<<<<< HEAD
static void mips_dma_unmap_page(struct device *dev, dma_addr_t dma_addr,
	size_t size, enum dma_data_direction direction, struct dma_attrs *attrs)
{
<<<<<<< HEAD
	if (cpu_is_noncoherent_r10000(dev))
		__dma_sync(dma_addr_to_virt(dev, dma_addr), size,
		           direction);
=======
	if (cpu_needs_post_dma_flush(dev))
		__dma_sync(dma_addr_to_page(dev, dma_addr),
			   dma_addr & ~PAGE_MASK, size, direction);
>>>>>>> 15c6df1... Squashed update of kernel from 3.4.74 to 3.4.75
=======
/*
 * A single sg entry may refer to multiple physically contiguous
 * pages. But we still need to process highmem pages individually.
 * If highmem is not configured then the bulk of this loop gets
 * optimized out.
 */
static inline void __dma_sync(struct page *page,
	unsigned long offset, size_t size, enum dma_data_direction direction)
{
	size_t left = size;

	do {
		size_t len = left;

		if (PageHighMem(page)) {
			void *addr;

			if (offset + len > PAGE_SIZE) {
				if (offset >= PAGE_SIZE) {
					page += offset >> PAGE_SHIFT;
					offset &= ~PAGE_MASK;
				}
				len = PAGE_SIZE - offset;
			}

			addr = kmap_atomic(page);
			__dma_sync_virtual(addr + offset, len, direction);
			kunmap_atomic(addr);
		} else
			__dma_sync_virtual(page_address(page) + offset,
					   size, direction);
		offset = 0;
		page++;
		left -= len;
	} while (left);
}

static void mips_dma_unmap_page(struct device *dev, dma_addr_t dma_addr,
	size_t size, enum dma_data_direction direction, struct dma_attrs *attrs)
{
	if (cpu_is_noncoherent_r10000(dev))
		__dma_sync(dma_addr_to_page(dev, dma_addr),
			   dma_addr & ~PAGE_MASK, size, direction);
>>>>>>> refs/remotes/origin/cm-10.0

	plat_unmap_dma_mem(dev, dma_addr, size, direction);
}

static int mips_dma_map_sg(struct device *dev, struct scatterlist *sg,
	int nents, enum dma_data_direction direction, struct dma_attrs *attrs)
{
	int i;

	for (i = 0; i < nents; i++, sg++) {
<<<<<<< HEAD
		unsigned long addr;

		addr = (unsigned long) sg_virt(sg);
		if (!plat_device_is_coherent(dev) && addr)
			__dma_sync(addr, sg->length, direction);
		sg->dma_address = plat_map_dma_mem(dev,
				                   (void *)addr, sg->length);
=======
		if (!plat_device_is_coherent(dev))
			__dma_sync(sg_page(sg), sg->offset, sg->length,
				   direction);
		sg->dma_address = plat_map_dma_mem_page(dev, sg_page(sg)) +
				  sg->offset;
>>>>>>> refs/remotes/origin/cm-10.0
	}

	return nents;
}

static dma_addr_t mips_dma_map_page(struct device *dev, struct page *page,
	unsigned long offset, size_t size, enum dma_data_direction direction,
	struct dma_attrs *attrs)
{
<<<<<<< HEAD
	unsigned long addr;

	addr = (unsigned long) page_address(page) + offset;

	if (!plat_device_is_coherent(dev))
		__dma_sync(addr, size, direction);

	return plat_map_dma_mem(dev, (void *)addr, size);
=======
	if (!plat_device_is_coherent(dev))
		__dma_sync(page, offset, size, direction);

	return plat_map_dma_mem_page(dev, page) + offset;
>>>>>>> refs/remotes/origin/cm-10.0
}

static void mips_dma_unmap_sg(struct device *dev, struct scatterlist *sg,
	int nhwentries, enum dma_data_direction direction,
	struct dma_attrs *attrs)
{
<<<<<<< HEAD
	unsigned long addr;
=======
>>>>>>> refs/remotes/origin/cm-10.0
	int i;

	for (i = 0; i < nhwentries; i++, sg++) {
		if (!plat_device_is_coherent(dev) &&
<<<<<<< HEAD
		    direction != DMA_TO_DEVICE) {
			addr = (unsigned long) sg_virt(sg);
			if (addr)
				__dma_sync(addr, sg->length, direction);
		}
=======
		    direction != DMA_TO_DEVICE)
			__dma_sync(sg_page(sg), sg->offset, sg->length,
				   direction);
>>>>>>> refs/remotes/origin/cm-10.0
		plat_unmap_dma_mem(dev, sg->dma_address, sg->length, direction);
	}
}

static void mips_dma_sync_single_for_cpu(struct device *dev,
	dma_addr_t dma_handle, size_t size, enum dma_data_direction direction)
{
<<<<<<< HEAD
<<<<<<< HEAD
	if (cpu_is_noncoherent_r10000(dev)) {
		unsigned long addr;

		addr = dma_addr_to_virt(dev, dma_handle);
		__dma_sync(addr, size, direction);
	}
=======
	if (cpu_needs_post_dma_flush(dev))
		__dma_sync(dma_addr_to_page(dev, dma_handle),
			   dma_handle & ~PAGE_MASK, size, direction);
>>>>>>> 15c6df1... Squashed update of kernel from 3.4.74 to 3.4.75
=======
	if (cpu_is_noncoherent_r10000(dev))
		__dma_sync(dma_addr_to_page(dev, dma_handle),
			   dma_handle & ~PAGE_MASK, size, direction);
>>>>>>> refs/remotes/origin/cm-10.0
}

static void mips_dma_sync_single_for_device(struct device *dev,
	dma_addr_t dma_handle, size_t size, enum dma_data_direction direction)
{
	plat_extra_sync_for_device(dev);
<<<<<<< HEAD
	if (!plat_device_is_coherent(dev)) {
		unsigned long addr;

		addr = dma_addr_to_virt(dev, dma_handle);
		__dma_sync(addr, size, direction);
	}
=======
	if (!plat_device_is_coherent(dev))
		__dma_sync(dma_addr_to_page(dev, dma_handle),
			   dma_handle & ~PAGE_MASK, size, direction);
>>>>>>> refs/remotes/origin/cm-10.0
}

static void mips_dma_sync_sg_for_cpu(struct device *dev,
	struct scatterlist *sg, int nelems, enum dma_data_direction direction)
{
	int i;

	/* Make sure that gcc doesn't leave the empty loop body.  */
	for (i = 0; i < nelems; i++, sg++) {
<<<<<<< HEAD
<<<<<<< HEAD
		if (cpu_is_noncoherent_r10000(dev))
			__dma_sync((unsigned long)page_address(sg_page(sg)),
			           sg->length, direction);
=======
		if (cpu_needs_post_dma_flush(dev))
			__dma_sync(sg_page(sg), sg->offset, sg->length,
				   direction);
>>>>>>> 15c6df1... Squashed update of kernel from 3.4.74 to 3.4.75
=======
		if (cpu_is_noncoherent_r10000(dev))
			__dma_sync(sg_page(sg), sg->offset, sg->length,
				   direction);
>>>>>>> refs/remotes/origin/cm-10.0
	}
}

static void mips_dma_sync_sg_for_device(struct device *dev,
	struct scatterlist *sg, int nelems, enum dma_data_direction direction)
{
	int i;

	/* Make sure that gcc doesn't leave the empty loop body.  */
	for (i = 0; i < nelems; i++, sg++) {
		if (!plat_device_is_coherent(dev))
<<<<<<< HEAD
			__dma_sync((unsigned long)page_address(sg_page(sg)),
			           sg->length, direction);
=======
			__dma_sync(sg_page(sg), sg->offset, sg->length,
				   direction);
>>>>>>> refs/remotes/origin/cm-10.0
	}
}

int mips_dma_mapping_error(struct device *dev, dma_addr_t dma_addr)
{
	return plat_dma_mapping_error(dev, dma_addr);
}

int mips_dma_supported(struct device *dev, u64 mask)
{
	return plat_dma_supported(dev, mask);
}

void dma_cache_sync(struct device *dev, void *vaddr, size_t size,
			 enum dma_data_direction direction)
{
	BUG_ON(direction == DMA_NONE);

	plat_extra_sync_for_device(dev);
	if (!plat_device_is_coherent(dev))
<<<<<<< HEAD
		__dma_sync((unsigned long)vaddr, size, direction);
=======
		__dma_sync_virtual(vaddr, size, direction);
>>>>>>> refs/remotes/origin/cm-10.0
}

EXPORT_SYMBOL(dma_cache_sync);

static struct dma_map_ops mips_default_dma_map_ops = {
<<<<<<< HEAD
	.alloc_coherent = mips_dma_alloc_coherent,
	.free_coherent = mips_dma_free_coherent,
=======
	.alloc = mips_dma_alloc_coherent,
	.free = mips_dma_free_coherent,
>>>>>>> refs/remotes/origin/cm-10.0
	.map_page = mips_dma_map_page,
	.unmap_page = mips_dma_unmap_page,
	.map_sg = mips_dma_map_sg,
	.unmap_sg = mips_dma_unmap_sg,
	.sync_single_for_cpu = mips_dma_sync_single_for_cpu,
	.sync_single_for_device = mips_dma_sync_single_for_device,
	.sync_sg_for_cpu = mips_dma_sync_sg_for_cpu,
	.sync_sg_for_device = mips_dma_sync_sg_for_device,
	.mapping_error = mips_dma_mapping_error,
	.dma_supported = mips_dma_supported
};

struct dma_map_ops *mips_dma_map_ops = &mips_default_dma_map_ops;
EXPORT_SYMBOL(mips_dma_map_ops);

#define PREALLOC_DMA_DEBUG_ENTRIES (1 << 16)

static int __init mips_dma_init(void)
{
	dma_debug_init(PREALLOC_DMA_DEBUG_ENTRIES);

	return 0;
}
fs_initcall(mips_dma_init);
