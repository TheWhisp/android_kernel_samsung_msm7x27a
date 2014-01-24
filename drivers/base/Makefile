# Makefile for the Linux device tree

<<<<<<< HEAD
<<<<<<< HEAD
obj-y			:= core.o sys.o bus.o dd.o syscore.o \
			   driver.o class.o platform.o \
			   cpu.o firmware.o init.o map.o devres.o \
			   attribute_container.o transport_class.o
obj-$(CONFIG_DEVTMPFS)	+= devtmpfs.o
obj-y			+= power/
obj-$(CONFIG_HAS_DMA)	+= dma-mapping.o
obj-$(CONFIG_HAVE_GENERIC_DMA_COHERENT) += dma-coherent.o
=======
obj-y			:= core.o bus.o dd.o syscore.o \
			   driver.o class.o platform.o \
			   cpu.o firmware.o init.o map.o devres.o \
			   attribute_container.o transport_class.o \
			   topology.o sys.o
obj-$(CONFIG_DEVTMPFS)	+= devtmpfs.o
obj-$(CONFIG_CMA) += dma-contiguous.o
obj-y			+= power/
obj-$(CONFIG_HAS_DMA)	+= dma-mapping.o
obj-$(CONFIG_HAVE_GENERIC_DMA_COHERENT) += dma-coherent.o
obj-$(CONFIG_DMA_SHARED_BUFFER) += dma-buf.o
>>>>>>> refs/remotes/origin/cm-10.0
obj-$(CONFIG_GENLOCK) += genlock.o
=======
obj-y			:= component.o core.o bus.o dd.o syscore.o \
			   driver.o class.o platform.o \
			   cpu.o firmware.o init.o map.o devres.o \
			   attribute_container.o transport_class.o \
			   topology.o
obj-$(CONFIG_DEVTMPFS)	+= devtmpfs.o
obj-$(CONFIG_DMA_CMA) += dma-contiguous.o
obj-y			+= power/
obj-$(CONFIG_HAS_DMA)	+= dma-mapping.o
obj-$(CONFIG_HAVE_GENERIC_DMA_COHERENT) += dma-coherent.o
obj-$(CONFIG_DMA_SHARED_BUFFER) += dma-buf.o reservation.o
>>>>>>> refs/remotes/origin/master
obj-$(CONFIG_ISA)	+= isa.o
obj-$(CONFIG_FW_LOADER)	+= firmware_class.o
obj-$(CONFIG_NUMA)	+= node.o
obj-$(CONFIG_MEMORY_HOTPLUG_SPARSE) += memory.o
<<<<<<< HEAD
<<<<<<< HEAD
obj-$(CONFIG_SMP)	+= topology.o
obj-$(CONFIG_IOMMU_API) += iommu.o
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
ifeq ($(CONFIG_SYSFS),y)
obj-$(CONFIG_MODULES)	+= module.o
endif
obj-$(CONFIG_SYS_HYPERVISOR) += hypervisor.o
<<<<<<< HEAD
<<<<<<< HEAD
=======
obj-$(CONFIG_REGMAP)	+= regmap/
obj-$(CONFIG_SOC_BUS) += soc.o
>>>>>>> refs/remotes/origin/cm-10.0

obj-$(CONFIG_SYNC)	+= sync.o
obj-$(CONFIG_SW_SYNC)	+= sw_sync.o
=======
obj-$(CONFIG_REGMAP)	+= regmap/
obj-$(CONFIG_SOC_BUS) += soc.o
obj-$(CONFIG_PINCTRL) += pinctrl.o
>>>>>>> refs/remotes/origin/master

ccflags-$(CONFIG_DEBUG_DRIVER) := -DDEBUG

