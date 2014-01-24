#ifndef _ASM_DMA_MAPPING_H
#define _ASM_DMA_MAPPING_H

#include <asm/scatterlist.h>
<<<<<<< HEAD
#include <asm/cache.h>
#include <asm-generic/dma-coherent.h>

#ifndef CONFIG_SGI_IP27	/* Kludge to fix 2.6.39 build for IP27 */
=======
#include <asm/dma-coherence.h>
#include <asm/cache.h>
#include <asm-generic/dma-coherent.h>

#ifndef CONFIG_SGI_IP27 /* Kludge to fix 2.6.39 build for IP27 */
>>>>>>> refs/remotes/origin/master
#include <dma-coherence.h>
#endif

extern struct dma_map_ops *mips_dma_map_ops;

static inline struct dma_map_ops *get_dma_ops(struct device *dev)
{
	if (dev && dev->archdata.dma_ops)
		return dev->archdata.dma_ops;
	else
		return mips_dma_map_ops;
}

static inline bool dma_capable(struct device *dev, dma_addr_t addr, size_t size)
{
	if (!dev->dma_mask)
		return 0;

	return addr + size <= *dev->dma_mask;
}

static inline void dma_mark_clean(void *addr, size_t size) {}

#include <asm-generic/dma-mapping-common.h>

static inline int dma_supported(struct device *dev, u64 mask)
{
	struct dma_map_ops *ops = get_dma_ops(dev);
	return ops->dma_supported(dev, mask);
}

static inline int dma_mapping_error(struct device *dev, u64 mask)
{
	struct dma_map_ops *ops = get_dma_ops(dev);
<<<<<<< HEAD
=======

	debug_dma_mapping_error(dev, mask);
>>>>>>> refs/remotes/origin/master
	return ops->mapping_error(dev, mask);
}

static inline int
dma_set_mask(struct device *dev, u64 mask)
{
	if(!dev->dma_mask || !dma_supported(dev, mask))
		return -EIO;

	*dev->dma_mask = mask;

	return 0;
}

extern void dma_cache_sync(struct device *dev, void *vaddr, size_t size,
	       enum dma_data_direction direction);

<<<<<<< HEAD
<<<<<<< HEAD
static inline void *dma_alloc_coherent(struct device *dev, size_t size,
				       dma_addr_t *dma_handle, gfp_t gfp)
=======
=======
>>>>>>> refs/remotes/origin/master
#define dma_alloc_coherent(d,s,h,f)	dma_alloc_attrs(d,s,h,f,NULL)

static inline void *dma_alloc_attrs(struct device *dev, size_t size,
				    dma_addr_t *dma_handle, gfp_t gfp,
				    struct dma_attrs *attrs)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	void *ret;
	struct dma_map_ops *ops = get_dma_ops(dev);

<<<<<<< HEAD
<<<<<<< HEAD
	ret = ops->alloc_coherent(dev, size, dma_handle, gfp);
=======
	ret = ops->alloc(dev, size, dma_handle, gfp, attrs);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ret = ops->alloc(dev, size, dma_handle, gfp, attrs);
>>>>>>> refs/remotes/origin/master

	debug_dma_alloc_coherent(dev, size, *dma_handle, ret);

	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline void dma_free_coherent(struct device *dev, size_t size,
				     void *vaddr, dma_addr_t dma_handle)
{
	struct dma_map_ops *ops = get_dma_ops(dev);

	ops->free_coherent(dev, size, vaddr, dma_handle);
=======
=======
>>>>>>> refs/remotes/origin/master
#define dma_free_coherent(d,s,c,h) dma_free_attrs(d,s,c,h,NULL)

static inline void dma_free_attrs(struct device *dev, size_t size,
				  void *vaddr, dma_addr_t dma_handle,
				  struct dma_attrs *attrs)
{
	struct dma_map_ops *ops = get_dma_ops(dev);

	ops->free(dev, size, vaddr, dma_handle, attrs);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	debug_dma_free_coherent(dev, size, vaddr, dma_handle);
}


void *dma_alloc_noncoherent(struct device *dev, size_t size,
			   dma_addr_t *dma_handle, gfp_t flag);

void dma_free_noncoherent(struct device *dev, size_t size,
			 void *vaddr, dma_addr_t dma_handle);

#endif /* _ASM_DMA_MAPPING_H */
