#
# Makefile for the linux kernel.
#
<<<<<<< HEAD

<<<<<<< HEAD
obj-y	:= irq.o pcie.o time.o common.o mpp.o
=======
obj-y	:= irq.o pcie.o time.o common.o mpp.o addr-map.o
>>>>>>> refs/remotes/origin/cm-10.0
obj-m	:=
obj-n	:=
obj-	:=

obj-$(CONFIG_GENERIC_GPIO) += gpio.o
=======
ccflags-$(CONFIG_ARCH_MULTIPLATFORM) := -I$(srctree)/$(src)/include

orion-gpio-$(CONFIG_GPIOLIB)      += gpio.o
obj-$(CONFIG_PLAT_ORION_LEGACY)   += irq.o pcie.o time.o common.o mpp.o
obj-$(CONFIG_PLAT_ORION_LEGACY)   += $(orion-gpio-y)
>>>>>>> refs/remotes/origin/master
