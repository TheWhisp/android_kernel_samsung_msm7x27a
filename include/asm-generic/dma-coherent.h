#ifndef DMA_COHERENT_H
#define DMA_COHERENT_H

#ifdef CONFIG_HAVE_GENERIC_DMA_COHERENT
/*
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
 * These two functions are only for dma allocator.
=======
 * These three functions are only for dma allocator.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * These three functions are only for dma allocator.
>>>>>>> refs/remotes/origin/master
=======
 * These three functions are only for dma allocator.
>>>>>>> refs/remotes/origin/cm-11.0
 * Don't use them in device drivers.
 */
int dma_alloc_from_coherent(struct device *dev, ssize_t size,
				       dma_addr_t *dma_handle, void **ret);
int dma_release_from_coherent(struct device *dev, int order, void *vaddr);

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
int dma_mmap_from_coherent(struct device *dev, struct vm_area_struct *vma,
			    void *cpu_addr, size_t size, int *ret);
>>>>>>> refs/remotes/origin/cm-10.0
=======
int dma_mmap_from_coherent(struct device *dev, struct vm_area_struct *vma,
			    void *cpu_addr, size_t size, int *ret);
>>>>>>> refs/remotes/origin/master
=======
int dma_mmap_from_coherent(struct device *dev, struct vm_area_struct *vma,
			    void *cpu_addr, size_t size, int *ret);
>>>>>>> refs/remotes/origin/cm-11.0
/*
 * Standard interface
 */
#define ARCH_HAS_DMA_DECLARE_COHERENT_MEMORY
extern int
dma_declare_coherent_memory(struct device *dev, dma_addr_t bus_addr,
			    dma_addr_t device_addr, size_t size, int flags);

extern void
dma_release_declared_memory(struct device *dev);

extern void *
dma_mark_declared_memory_occupied(struct device *dev,
				  dma_addr_t device_addr, size_t size);
#else
#define dma_alloc_from_coherent(dev, size, handle, ret) (0)
#define dma_release_from_coherent(dev, order, vaddr) (0)
<<<<<<< HEAD
=======
#define dma_mmap_from_coherent(dev, vma, vaddr, order, ret) (0)
>>>>>>> refs/remotes/origin/master
#endif

#endif
