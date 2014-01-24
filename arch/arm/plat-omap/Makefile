#
# Makefile for the linux kernel.
#

<<<<<<< HEAD
# Common support
obj-y := common.o sram.o clock.o devices.o dma.o mux.o \
<<<<<<< HEAD
	 usb.o fb.o io.o counter_32k.o
=======
	 usb.o fb.o counter_32k.o
>>>>>>> refs/remotes/origin/cm-10.0
=======
ccflags-$(CONFIG_ARCH_MULTIPLATFORM) := -I$(srctree)/arch/arm/plat-omap/include

# Common support
obj-y := sram.o dma.o counter_32k.o
>>>>>>> refs/remotes/origin/master
obj-m :=
obj-n :=
obj-  :=

<<<<<<< HEAD
# OCPI interconnect support for 1710, 1610 and 5912
obj-$(CONFIG_ARCH_OMAP16XX) += ocpi.o

# omap_device support (OMAP2+ only at the moment)
obj-$(CONFIG_ARCH_OMAP2) += omap_device.o
obj-$(CONFIG_ARCH_OMAP3) += omap_device.o
obj-$(CONFIG_ARCH_OMAP4) += omap_device.o

<<<<<<< HEAD
obj-$(CONFIG_OMAP_MCBSP) += mcbsp.o
obj-$(CONFIG_OMAP_IOMMU) += iommu.o iovmm.o
obj-$(CONFIG_OMAP_IOMMU_DEBUG) += iommu-debug.o

obj-$(CONFIG_CPU_FREQ) += cpu-omap.o
=======
>>>>>>> refs/remotes/origin/cm-10.0
obj-$(CONFIG_OMAP_DM_TIMER) += dmtimer.o
obj-$(CONFIG_OMAP_DEBUG_DEVICES) += debug-devices.o
=======
# omap_device support (OMAP2+ only at the moment)

obj-$(CONFIG_OMAP_DM_TIMER) += dmtimer.o
>>>>>>> refs/remotes/origin/master
obj-$(CONFIG_OMAP_DEBUG_LEDS) += debug-leds.o
i2c-omap-$(CONFIG_I2C_OMAP) := i2c.o
obj-y += $(i2c-omap-m) $(i2c-omap-y)

<<<<<<< HEAD
# OMAP mailbox framework
obj-$(CONFIG_OMAP_MBOX_FWK) += mailbox.o

obj-$(CONFIG_OMAP_PM_NOOP) += omap-pm-noop.o
=======
>>>>>>> refs/remotes/origin/master
