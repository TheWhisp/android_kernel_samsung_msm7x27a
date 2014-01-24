#
# Makefile for the kernel mmc core.
#

obj-$(CONFIG_MMC)		+= mmc_core.o
mmc_core-y			:= core.o bus.o host.o \
				   mmc.o mmc_ops.o sd.o sd_ops.o \
				   sdio.o sdio_ops.o sdio_bus.o \
				   sdio_cis.o sdio_io.o sdio_irq.o \
<<<<<<< HEAD
				   quirks.o
=======
				   quirks.o cd-gpio.o
>>>>>>> refs/remotes/origin/cm-10.0

mmc_core-$(CONFIG_DEBUG_FS)	+= debugfs.o
