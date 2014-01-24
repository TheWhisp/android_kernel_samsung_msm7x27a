/*
 * Copyright 2010 Tilera Corporation. All Rights Reserved.
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation, version 2.
 *
 *   This program is distributed in the hope that it will be useful, but
 *   WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
 *   NON INFRINGEMENT.  See the GNU General Public License for
 *   more details.
 */

#ifndef _ASM_TILE_DMA_MAPPING_H
#define _ASM_TILE_DMA_MAPPING_H

#include <linux/mm.h>
#include <linux/scatterlist.h>
#include <linux/cache.h>
#include <linux/io.h>

<<<<<<< HEAD
/*
 * Note that on x86 and powerpc, there is a "struct dma_mapping_ops"
 * that is used for all the DMA operations.  For now, we don't have an
 * equivalent on tile, because we only have a single way of doing DMA.
 * (Tilera bug 7994 to use dma_mapping_ops.)
 */

#define dma_alloc_noncoherent(d, s, h, f) dma_alloc_coherent(d, s, h, f)
#define dma_free_noncoherent(d, s, v, h) dma_free_coherent(d, s, v, h)

extern dma_addr_t dma_map_single(struct device *dev, void *ptr, size_t size,
			  enum dma_data_direction);
extern void dma_unmap_single(struct device *dev, dma_addr_t dma_addr,
			     size_t size, enum dma_data_direction);
extern int dma_map_sg(struct device *dev, struct scatterlist *sg, int nents,
	       enum dma_data_direction);
extern void dma_unmap_sg(struct device *dev, struct scatterlist *sg,
			 int nhwentries, enum dma_data_direction);
extern dma_addr_t dma_map_page(struct device *dev, struct page *page,
			       unsigned long offset, size_t size,
			       enum dma_data_direction);
extern void dma_unmap_page(struct device *dev, dma_addr_t dma_address,
			   size_t size, enum dma_data_direction);
extern void dma_sync_sg_for_cpu(struct device *dev, struct scatterlist *sg,
				int nelems, enum dma_data_direction);
extern void dma_sync_sg_for_device(struct device *dev, struct scatterlist *sg,
				   int nelems, enum dma_data_direction);


void *dma_alloc_coherent(struct device *dev, size_t size,
			   dma_addr_t *dma_handle, gfp_t flag);

void dma_free_coherent(struct device *dev, size_t size,
			 void *vaddr, dma_addr_t dma_handle);

extern void dma_sync_single_for_cpu(struct device *, dma_addr_t, size_t,
				    enum dma_data_direction);
extern void dma_sync_single_for_device(struct device *, dma_addr_t,
				       size_t, enum dma_data_direction);
extern void dma_sync_single_range_for_cpu(struct device *, dma_addr_t,
					  unsigned long offset, size_t,
					  enum dma_data_direction);
extern void dma_sync_single_range_for_device(struct device *, dma_addr_t,
					     unsigned long offset, size_t,
					     enum dma_data_direction);
extern void dma_cache_sync(struct device *dev, void *vaddr, size_t,
			   enum dma_data_direction);
=======
#ifdef __tilegx__
#define ARCH_HAS_DMA_GET_REQUIRED_MASK
#endif

extern struct dma_map_ops *tile_dma_map_ops;
extern struct dma_map_ops *gx_pci_dma_map_ops;
extern struct dma_map_ops *gx_legacy_pci_dma_map_ops;
extern struct dma_map_ops *gx_hybrid_pci_dma_map_ops;

static inline struct dma_map_ops *get_dma_ops(struct device *dev)
{
	if (dev && dev->archdata.dma_ops)
		return dev->archdata.dma_ops;
	else
		return tile_dma_map_ops;
}

static inline dma_addr_t get_dma_offset(struct device *dev)
{
	return dev->archdata.dma_offset;
}

static inline void set_dma_offset(struct device *dev, dma_addr_t off)
{
	dev->archdata.dma_offset = off;
}

static inline dma_addr_t phys_to_dma(struct device *dev, phys_addr_t paddr)
{
	return paddr;
}

static inline phys_addr_t dma_to_phys(struct device *dev, dma_addr_t daddr)
{
	return daddr;
}

static inline void dma_mark_clean(void *addr, size_t size) {}

#include <asm-generic/dma-mapping-common.h>

static inline void set_dma_ops(struct device *dev, struct dma_map_ops *ops)
{
	dev->archdata.dma_ops = ops;
}

static inline bool dma_capable(struct device *dev, dma_addr_t addr, size_t size)
{
	if (!dev->dma_mask)
		return 0;

	return addr + size - 1 <= *dev->dma_mask;
}
>>>>>>> refs/remotes/origin/master

static inline int
dma_mapping_error(struct device *dev, dma_addr_t dma_addr)
{
<<<<<<< HEAD
	return 0;
=======
	debug_dma_mapping_error(dev, dma_addr);
	return get_dma_ops(dev)->mapping_error(dev, dma_addr);
>>>>>>> refs/remotes/origin/master
}

static inline int
dma_supported(struct device *dev, u64 mask)
{
<<<<<<< HEAD
	return 1;
=======
	return get_dma_ops(dev)->dma_supported(dev, mask);
>>>>>>> refs/remotes/origin/master
}

static inline int
dma_set_mask(struct device *dev, u64 mask)
{
<<<<<<< HEAD
=======
	struct dma_map_ops *dma_ops = get_dma_ops(dev);

	/*
	 * For PCI devices with 64-bit DMA addressing capability, promote
	 * the dma_ops to hybrid, with the consistent memory DMA space limited
	 * to 32-bit. For 32-bit capable devices, limit the streaming DMA
	 * address range to max_direct_dma_addr.
	 */
	if (dma_ops == gx_pci_dma_map_ops ||
	    dma_ops == gx_hybrid_pci_dma_map_ops ||
	    dma_ops == gx_legacy_pci_dma_map_ops) {
		if (mask == DMA_BIT_MASK(64) &&
		    dma_ops == gx_legacy_pci_dma_map_ops)
			set_dma_ops(dev, gx_hybrid_pci_dma_map_ops);
		else if (mask > dev->archdata.max_direct_dma_addr)
			mask = dev->archdata.max_direct_dma_addr;
	}

>>>>>>> refs/remotes/origin/master
	if (!dev->dma_mask || !dma_supported(dev, mask))
		return -EIO;

	*dev->dma_mask = mask;

	return 0;
}

<<<<<<< HEAD
=======
static inline void *dma_alloc_attrs(struct device *dev, size_t size,
				    dma_addr_t *dma_handle, gfp_t flag,
				    struct dma_attrs *attrs)
{
	struct dma_map_ops *dma_ops = get_dma_ops(dev);
	void *cpu_addr;

	cpu_addr = dma_ops->alloc(dev, size, dma_handle, flag, attrs);

	debug_dma_alloc_coherent(dev, size, *dma_handle, cpu_addr);

	return cpu_addr;
}

static inline void dma_free_attrs(struct device *dev, size_t size,
				  void *cpu_addr, dma_addr_t dma_handle,
				  struct dma_attrs *attrs)
{
	struct dma_map_ops *dma_ops = get_dma_ops(dev);

	debug_dma_free_coherent(dev, size, cpu_addr, dma_handle);

	dma_ops->free(dev, size, cpu_addr, dma_handle, attrs);
}

#define dma_alloc_coherent(d, s, h, f) dma_alloc_attrs(d, s, h, f, NULL)
#define dma_alloc_noncoherent(d, s, h, f) dma_alloc_attrs(d, s, h, f, NULL)
#define dma_free_coherent(d, s, v, h) dma_free_attrs(d, s, v, h, NULL)
#define dma_free_noncoherent(d, s, v, h) dma_free_attrs(d, s, v, h, NULL)

/*
 * dma_alloc_noncoherent() is #defined to return coherent memory,
 * so there's no need to do any flushing here.
 */
static inline void dma_cache_sync(struct device *dev, void *vaddr, size_t size,
				  enum dma_data_direction direction)
{
}

>>>>>>> refs/remotes/origin/master
#endif /* _ASM_TILE_DMA_MAPPING_H */
