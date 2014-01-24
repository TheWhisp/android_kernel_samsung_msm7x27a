#ifndef _ASM_X86_DEVICE_H
#define _ASM_X86_DEVICE_H

struct dev_archdata {
<<<<<<< HEAD
#ifdef CONFIG_ACPI
	void	*acpi_handle;
#endif
#ifdef CONFIG_X86_64
struct dma_map_ops *dma_ops;
#endif
<<<<<<< HEAD
#if defined(CONFIG_DMAR) || defined(CONFIG_AMD_IOMMU)
=======
#if defined(CONFIG_INTEL_IOMMU) || defined(CONFIG_AMD_IOMMU)
>>>>>>> refs/remotes/origin/cm-10.0
=======
#ifdef CONFIG_X86_DEV_DMA_OPS
	struct dma_map_ops *dma_ops;
#endif
#if defined(CONFIG_INTEL_IOMMU) || defined(CONFIG_AMD_IOMMU)
>>>>>>> refs/remotes/origin/master
	void *iommu; /* hook for IOMMU specific extension */
#endif
};

struct pdev_archdata {
};

#endif /* _ASM_X86_DEVICE_H */
