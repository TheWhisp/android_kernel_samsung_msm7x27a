#
# Makefile for the MB93090-MB00 motherboard stuff
#

ifeq "$(CONFIG_PCI)" "y"
<<<<<<< HEAD
<<<<<<< HEAD
obj-y := pci-frv.o pci-irq.o pci-vdk.o pci-iomap.o
=======
obj-y := pci-frv.o pci-irq.o pci-vdk.o
>>>>>>> refs/remotes/origin/cm-10.0
=======
obj-y := pci-frv.o pci-irq.o pci-vdk.o
>>>>>>> refs/remotes/origin/master

ifeq "$(CONFIG_MMU)" "y"
obj-y += pci-dma.o
else
obj-y += pci-dma-nommu.o
endif
endif

obj-$(CONFIG_MTD) += flash.o
