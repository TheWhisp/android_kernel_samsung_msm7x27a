#ifndef ___ASM_SPARC_DMA_MAPPING_H
#define ___ASM_SPARC_DMA_MAPPING_H

#include <linux/scatterlist.h>
#include <linux/mm.h>
#include <linux/dma-debug.h>

#define DMA_ERROR_CODE	(~(dma_addr_t)0x0)

extern int dma_supported(struct device *dev, u64 mask);

#define dma_alloc_noncoherent(d, s, h, f) dma_alloc_coherent(d, s, h, f)
#define dma_free_noncoherent(d, s, v, h) dma_free_coherent(d, s, v, h)

<<<<<<< HEAD
extern struct dma_map_ops *dma_ops, pci32_dma_ops;
=======
extern struct dma_map_ops *dma_ops;
extern struct dma_map_ops *leon_dma_ops;
extern struct dma_map_ops pci32_dma_ops;

>>>>>>> refs/remotes/origin/master
extern struct bus_type pci_bus_type;

static inline struct dma_map_ops *get_dma_ops(struct device *dev)
{
#if defined(CONFIG_SPARC32) && defined(CONFIG_PCI)
<<<<<<< HEAD
	if (dev->bus == &pci_bus_type)
=======
	if (sparc_cpu_model == sparc_leon)
		return leon_dma_ops;
	else if (dev->bus == &pci_bus_type)
>>>>>>> refs/remotes/origin/master
		return &pci32_dma_ops;
#endif
	return dma_ops;
}

#include <asm-generic/dma-mapping-common.h>

<<<<<<< HEAD
<<<<<<< HEAD
static inline void *dma_alloc_coherent(struct device *dev, size_t size,
				       dma_addr_t *dma_handle, gfp_t flag)
=======
=======
>>>>>>> refs/remotes/origin/master
#define dma_alloc_coherent(d,s,h,f)	dma_alloc_attrs(d,s,h,f,NULL)

static inline void *dma_alloc_attrs(struct device *dev, size_t size,
				    dma_addr_t *dma_handle, gfp_t flag,
				    struct dma_attrs *attrs)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	struct dma_map_ops *ops = get_dma_ops(dev);
	void *cpu_addr;

<<<<<<< HEAD
<<<<<<< HEAD
	cpu_addr = ops->alloc_coherent(dev, size, dma_handle, flag);
=======
	cpu_addr = ops->alloc(dev, size, dma_handle, flag, attrs);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	cpu_addr = ops->alloc(dev, size, dma_handle, flag, attrs);
>>>>>>> refs/remotes/origin/master
	debug_dma_alloc_coherent(dev, size, *dma_handle, cpu_addr);
	return cpu_addr;
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline void dma_free_coherent(struct device *dev, size_t size,
				     void *cpu_addr, dma_addr_t dma_handle)
=======
=======
>>>>>>> refs/remotes/origin/master
#define dma_free_coherent(d,s,c,h) dma_free_attrs(d,s,c,h,NULL)

static inline void dma_free_attrs(struct device *dev, size_t size,
				  void *cpu_addr, dma_addr_t dma_handle,
				  struct dma_attrs *attrs)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	struct dma_map_ops *ops = get_dma_ops(dev);

	debug_dma_free_coherent(dev, size, cpu_addr, dma_handle);
<<<<<<< HEAD
<<<<<<< HEAD
	ops->free_coherent(dev, size, cpu_addr, dma_handle);
=======
	ops->free(dev, size, cpu_addr, dma_handle, attrs);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ops->free(dev, size, cpu_addr, dma_handle, attrs);
>>>>>>> refs/remotes/origin/master
}

static inline int dma_mapping_error(struct device *dev, dma_addr_t dma_addr)
{
<<<<<<< HEAD
=======
	debug_dma_mapping_error(dev, dma_addr);
>>>>>>> refs/remotes/origin/master
	return (dma_addr == DMA_ERROR_CODE);
}

static inline int dma_set_mask(struct device *dev, u64 mask)
{
#ifdef CONFIG_PCI
	if (dev->bus == &pci_bus_type) {
		if (!dev->dma_mask || !dma_supported(dev, mask))
			return -EINVAL;
		*dev->dma_mask = mask;
		return 0;
	}
#endif
	return -EINVAL;
}

#endif
