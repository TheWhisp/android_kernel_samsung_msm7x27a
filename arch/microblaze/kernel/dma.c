/*
 * Copyright (C) 2009-2010 PetaLogix
 * Copyright (C) 2006 Benjamin Herrenschmidt, IBM Corporation
 *
 * Provide default implementations of the DMA mapping callbacks for
 * directly mapped busses.
 */

#include <linux/device.h>
#include <linux/dma-mapping.h>
#include <linux/gfp.h>
#include <linux/dma-debug.h>
<<<<<<< HEAD
#include <asm/bug.h>
#include <asm/cacheflush.h>
=======
#include <linux/export.h>
#include <asm/bug.h>
>>>>>>> refs/remotes/origin/cm-10.0

/*
 * Generic direct DMA implementation
 *
 * This implementation supports a per-device offset that can be applied if
 * the address at which memory is visible to devices is not 0. Platform code
 * can set archdata.dma_data to an unsigned long holding the offset. By
 * default the offset is PCI_DRAM_OFFSET.
 */
<<<<<<< HEAD
static inline void __dma_sync_page(unsigned long paddr, unsigned long offset,
				size_t size, enum dma_data_direction direction)
{
	switch (direction) {
	case DMA_TO_DEVICE:
	case DMA_BIDIRECTIONAL:
		flush_dcache_range(paddr + offset, paddr + offset + size);
		break;
	case DMA_FROM_DEVICE:
		invalidate_dcache_range(paddr + offset, paddr + offset + size);
		break;
	default:
		BUG();
	}
}
=======
>>>>>>> refs/remotes/origin/cm-10.0

static unsigned long get_dma_direct_offset(struct device *dev)
{
	if (likely(dev))
		return (unsigned long)dev->archdata.dma_data;

	return PCI_DRAM_OFFSET; /* FIXME Not sure if is correct */
}

#define NOT_COHERENT_CACHE

static void *dma_direct_alloc_coherent(struct device *dev, size_t size,
<<<<<<< HEAD
				dma_addr_t *dma_handle, gfp_t flag)
=======
				       dma_addr_t *dma_handle, gfp_t flag,
				       struct dma_attrs *attrs)
>>>>>>> refs/remotes/origin/cm-10.0
{
#ifdef NOT_COHERENT_CACHE
	return consistent_alloc(flag, size, dma_handle);
#else
	void *ret;
	struct page *page;
	int node = dev_to_node(dev);

	/* ignore region specifiers */
	flag  &= ~(__GFP_HIGHMEM);

	page = alloc_pages_node(node, flag, get_order(size));
	if (page == NULL)
		return NULL;
	ret = page_address(page);
	memset(ret, 0, size);
	*dma_handle = virt_to_phys(ret) + get_dma_direct_offset(dev);

	return ret;
#endif
}

static void dma_direct_free_coherent(struct device *dev, size_t size,
<<<<<<< HEAD
			      void *vaddr, dma_addr_t dma_handle)
=======
				     void *vaddr, dma_addr_t dma_handle,
				     struct dma_attrs *attrs)
>>>>>>> refs/remotes/origin/cm-10.0
{
#ifdef NOT_COHERENT_CACHE
	consistent_free(size, vaddr);
#else
	free_pages((unsigned long)vaddr, get_order(size));
#endif
}

static int dma_direct_map_sg(struct device *dev, struct scatterlist *sgl,
			     int nents, enum dma_data_direction direction,
			     struct dma_attrs *attrs)
{
	struct scatterlist *sg;
	int i;

	/* FIXME this part of code is untested */
	for_each_sg(sgl, sg, nents, i) {
		sg->dma_address = sg_phys(sg) + get_dma_direct_offset(dev);
<<<<<<< HEAD
		__dma_sync_page(page_to_phys(sg_page(sg)), sg->offset,
=======
		__dma_sync(page_to_phys(sg_page(sg)) + sg->offset,
>>>>>>> refs/remotes/origin/cm-10.0
							sg->length, direction);
	}

	return nents;
}

static void dma_direct_unmap_sg(struct device *dev, struct scatterlist *sg,
				int nents, enum dma_data_direction direction,
				struct dma_attrs *attrs)
{
}

static int dma_direct_dma_supported(struct device *dev, u64 mask)
{
	return 1;
}

static inline dma_addr_t dma_direct_map_page(struct device *dev,
					     struct page *page,
					     unsigned long offset,
					     size_t size,
					     enum dma_data_direction direction,
					     struct dma_attrs *attrs)
{
<<<<<<< HEAD
	__dma_sync_page(page_to_phys(page), offset, size, direction);
=======
	__dma_sync(page_to_phys(page) + offset, size, direction);
>>>>>>> refs/remotes/origin/cm-10.0
	return page_to_phys(page) + offset + get_dma_direct_offset(dev);
}

static inline void dma_direct_unmap_page(struct device *dev,
					 dma_addr_t dma_address,
					 size_t size,
					 enum dma_data_direction direction,
					 struct dma_attrs *attrs)
{
/* There is not necessary to do cache cleanup
 *
 * phys_to_virt is here because in __dma_sync_page is __virt_to_phys and
 * dma_address is physical address
 */
<<<<<<< HEAD
	__dma_sync_page(dma_address, 0 , size, direction);
}

struct dma_map_ops dma_direct_ops = {
	.alloc_coherent	= dma_direct_alloc_coherent,
	.free_coherent	= dma_direct_free_coherent,
=======
	__dma_sync(dma_address, size, direction);
}

static inline void
dma_direct_sync_single_for_cpu(struct device *dev,
			       dma_addr_t dma_handle, size_t size,
			       enum dma_data_direction direction)
{
	/*
	 * It's pointless to flush the cache as the memory segment
	 * is given to the CPU
	 */

	if (direction == DMA_FROM_DEVICE)
		__dma_sync(dma_handle, size, direction);
}

static inline void
dma_direct_sync_single_for_device(struct device *dev,
				  dma_addr_t dma_handle, size_t size,
				  enum dma_data_direction direction)
{
	/*
	 * It's pointless to invalidate the cache if the device isn't
	 * supposed to write to the relevant region
	 */

	if (direction == DMA_TO_DEVICE)
		__dma_sync(dma_handle, size, direction);
}

static inline void
dma_direct_sync_sg_for_cpu(struct device *dev,
			   struct scatterlist *sgl, int nents,
			   enum dma_data_direction direction)
{
	struct scatterlist *sg;
	int i;

	/* FIXME this part of code is untested */
	if (direction == DMA_FROM_DEVICE)
		for_each_sg(sgl, sg, nents, i)
			__dma_sync(sg->dma_address, sg->length, direction);
}

static inline void
dma_direct_sync_sg_for_device(struct device *dev,
			      struct scatterlist *sgl, int nents,
			      enum dma_data_direction direction)
{
	struct scatterlist *sg;
	int i;

	/* FIXME this part of code is untested */
	if (direction == DMA_TO_DEVICE)
		for_each_sg(sgl, sg, nents, i)
			__dma_sync(sg->dma_address, sg->length, direction);
}

struct dma_map_ops dma_direct_ops = {
	.alloc		= dma_direct_alloc_coherent,
	.free		= dma_direct_free_coherent,
>>>>>>> refs/remotes/origin/cm-10.0
	.map_sg		= dma_direct_map_sg,
	.unmap_sg	= dma_direct_unmap_sg,
	.dma_supported	= dma_direct_dma_supported,
	.map_page	= dma_direct_map_page,
	.unmap_page	= dma_direct_unmap_page,
<<<<<<< HEAD
=======
	.sync_single_for_cpu		= dma_direct_sync_single_for_cpu,
	.sync_single_for_device		= dma_direct_sync_single_for_device,
	.sync_sg_for_cpu		= dma_direct_sync_sg_for_cpu,
	.sync_sg_for_device		= dma_direct_sync_sg_for_device,
>>>>>>> refs/remotes/origin/cm-10.0
};
EXPORT_SYMBOL(dma_direct_ops);

/* Number of entries preallocated for DMA-API debugging */
#define PREALLOC_DMA_DEBUG_ENTRIES (1 << 16)

static int __init dma_init(void)
{
       dma_debug_init(PREALLOC_DMA_DEBUG_ENTRIES);

       return 0;
}
fs_initcall(dma_init);
