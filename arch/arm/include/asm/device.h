/*
 * Arch specific extensions to struct device
 *
 * This file is released under the GPLv2
 */
#ifndef ASMARM_DEVICE_H
#define ASMARM_DEVICE_H

struct dev_archdata {
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_DMABOUNCE
	struct dmabounce_device_info *dmabounce;
#endif
};

struct pdev_archdata {
=======
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	struct dma_map_ops	*dma_ops;
#ifdef CONFIG_DMABOUNCE
	struct dmabounce_device_info *dmabounce;
#endif
#ifdef CONFIG_IOMMU_API
	void *iommu; /* private IOMMU data */
#endif
#ifdef CONFIG_ARM_DMA_USE_IOMMU
	struct dma_iommu_mapping	*mapping;
#endif
};

struct omap_device;

struct pdev_archdata {
#ifdef CONFIG_ARCH_OMAP
	struct omap_device *od;
#endif
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
};

=======
};

#ifdef CONFIG_ARM_DMA_USE_IOMMU
#define to_dma_iommu_mapping(dev) ((dev)->archdata.mapping)
#else
#define to_dma_iommu_mapping(dev) NULL
#endif

>>>>>>> refs/remotes/origin/master
#endif
