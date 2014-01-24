#ifndef _ASM_X86_DMA_MAPPING_H
#define _ASM_X86_DMA_MAPPING_H

/*
<<<<<<< HEAD
<<<<<<< HEAD
 * IOMMU interface. See Documentation/PCI/PCI-DMA-mapping.txt and
=======
 * IOMMU interface. See Documentation/DMA-API-HOWTO.txt and
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * IOMMU interface. See Documentation/DMA-API-HOWTO.txt and
>>>>>>> refs/remotes/origin/master
 * Documentation/DMA-API.txt for documentation.
 */

#include <linux/kmemcheck.h>
#include <linux/scatterlist.h>
#include <linux/dma-debug.h>
#include <linux/dma-attrs.h>
#include <asm/io.h>
#include <asm/swiotlb.h>
#include <asm-generic/dma-coherent.h>
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/dma-contiguous.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/dma-contiguous.h>
>>>>>>> refs/remotes/origin/master
=======
#include <linux/dma-contiguous.h>
>>>>>>> refs/remotes/origin/cm-11.0

#ifdef CONFIG_ISA
# define ISA_DMA_BIT_MASK DMA_BIT_MASK(24)
#else
# define ISA_DMA_BIT_MASK DMA_BIT_MASK(32)
#endif

#define DMA_ERROR_CODE	0

extern int iommu_merge;
extern struct device x86_dma_fallback_dev;
extern int panic_on_overflow;

extern struct dma_map_ops *dma_ops;

static inline struct dma_map_ops *get_dma_ops(struct device *dev)
{
<<<<<<< HEAD
#ifdef CONFIG_X86_32
=======
#ifndef CONFIG_X86_DEV_DMA_OPS
>>>>>>> refs/remotes/origin/master
	return dma_ops;
#else
	if (unlikely(!dev) || !dev->archdata.dma_ops)
		return dma_ops;
	else
		return dev->archdata.dma_ops;
#endif
}

#include <asm-generic/dma-mapping-common.h>

/* Make sure we keep the same behaviour */
static inline int dma_mapping_error(struct device *dev, dma_addr_t dma_addr)
{
	struct dma_map_ops *ops = get_dma_ops(dev);
<<<<<<< HEAD
=======
	debug_dma_mapping_error(dev, dma_addr);
>>>>>>> refs/remotes/origin/master
	if (ops->mapping_error)
		return ops->mapping_error(dev, dma_addr);

	return (dma_addr == DMA_ERROR_CODE);
}

#define dma_alloc_noncoherent(d, s, h, f) dma_alloc_coherent(d, s, h, f)
#define dma_free_noncoherent(d, s, v, h) dma_free_coherent(d, s, v, h)

extern int dma_supported(struct device *hwdev, u64 mask);
extern int dma_set_mask(struct device *dev, u64 mask);

extern void *dma_generic_alloc_coherent(struct device *dev, size_t size,
<<<<<<< HEAD
<<<<<<< HEAD
					dma_addr_t *dma_addr, gfp_t flag);
=======
=======
>>>>>>> refs/remotes/origin/master
					dma_addr_t *dma_addr, gfp_t flag,
					struct dma_attrs *attrs);

extern void dma_generic_free_coherent(struct device *dev, size_t size,
				      void *vaddr, dma_addr_t dma_addr,
				      struct dma_attrs *attrs);
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======

#ifdef CONFIG_X86_DMA_REMAP /* Platform code defines bridge-specific code */
extern bool dma_capable(struct device *dev, dma_addr_t addr, size_t size);
extern dma_addr_t phys_to_dma(struct device *dev, phys_addr_t paddr);
extern phys_addr_t dma_to_phys(struct device *dev, dma_addr_t daddr);
#else
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

static inline bool dma_capable(struct device *dev, dma_addr_t addr, size_t size)
{
	if (!dev->dma_mask)
		return 0;

	return addr + size - 1 <= *dev->dma_mask;
}

static inline dma_addr_t phys_to_dma(struct device *dev, phys_addr_t paddr)
{
	return paddr;
}

static inline phys_addr_t dma_to_phys(struct device *dev, dma_addr_t daddr)
{
	return daddr;
}
<<<<<<< HEAD
=======
#endif /* CONFIG_X86_DMA_REMAP */
>>>>>>> refs/remotes/origin/master

static inline void
dma_cache_sync(struct device *dev, void *vaddr, size_t size,
	enum dma_data_direction dir)
{
	flush_write_buffers();
}

static inline unsigned long dma_alloc_coherent_mask(struct device *dev,
						    gfp_t gfp)
{
	unsigned long dma_mask = 0;

	dma_mask = dev->coherent_dma_mask;
	if (!dma_mask)
		dma_mask = (gfp & GFP_DMA) ? DMA_BIT_MASK(24) : DMA_BIT_MASK(32);

	return dma_mask;
}

static inline gfp_t dma_alloc_coherent_gfp_flags(struct device *dev, gfp_t gfp)
{
	unsigned long dma_mask = dma_alloc_coherent_mask(dev, gfp);

	if (dma_mask <= DMA_BIT_MASK(24))
		gfp |= GFP_DMA;
#ifdef CONFIG_X86_64
	if (dma_mask <= DMA_BIT_MASK(32) && !(gfp & GFP_DMA))
		gfp |= GFP_DMA32;
#endif
       return gfp;
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline void *
dma_alloc_coherent(struct device *dev, size_t size, dma_addr_t *dma_handle,
		gfp_t gfp)
=======
=======
>>>>>>> refs/remotes/origin/master
#define dma_alloc_coherent(d,s,h,f)	dma_alloc_attrs(d,s,h,f,NULL)

static inline void *
dma_alloc_attrs(struct device *dev, size_t size, dma_addr_t *dma_handle,
		gfp_t gfp, struct dma_attrs *attrs)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	struct dma_map_ops *ops = get_dma_ops(dev);
	void *memory;

	gfp &= ~(__GFP_DMA | __GFP_HIGHMEM | __GFP_DMA32);

	if (dma_alloc_from_coherent(dev, size, dma_handle, &memory))
		return memory;

	if (!dev)
		dev = &x86_dma_fallback_dev;

	if (!is_device_dma_capable(dev))
		return NULL;

<<<<<<< HEAD
<<<<<<< HEAD
	if (!ops->alloc_coherent)
		return NULL;

	memory = ops->alloc_coherent(dev, size, dma_handle,
				     dma_alloc_coherent_gfp_flags(dev, gfp));
=======
=======
>>>>>>> refs/remotes/origin/master
	if (!ops->alloc)
		return NULL;

	memory = ops->alloc(dev, size, dma_handle,
			    dma_alloc_coherent_gfp_flags(dev, gfp), attrs);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	debug_dma_alloc_coherent(dev, size, *dma_handle, memory);

	return memory;
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline void dma_free_coherent(struct device *dev, size_t size,
				     void *vaddr, dma_addr_t bus)
=======
=======
>>>>>>> refs/remotes/origin/master
#define dma_free_coherent(d,s,c,h) dma_free_attrs(d,s,c,h,NULL)

static inline void dma_free_attrs(struct device *dev, size_t size,
				  void *vaddr, dma_addr_t bus,
				  struct dma_attrs *attrs)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	struct dma_map_ops *ops = get_dma_ops(dev);

	WARN_ON(irqs_disabled());       /* for portability */

	if (dma_release_from_coherent(dev, get_order(size), vaddr))
		return;

	debug_dma_free_coherent(dev, size, vaddr, bus);
<<<<<<< HEAD
<<<<<<< HEAD
	if (ops->free_coherent)
		ops->free_coherent(dev, size, vaddr, bus);
=======
	if (ops->free)
		ops->free(dev, size, vaddr, bus, attrs);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (ops->free)
		ops->free(dev, size, vaddr, bus, attrs);
>>>>>>> refs/remotes/origin/master
}

#endif
