#
# Makefile for RapidIO interconnect services
#
<<<<<<< HEAD
obj-y += rio.o rio-access.o rio-driver.o rio-scan.o rio-sysfs.o

obj-$(CONFIG_RAPIDIO)		+= switches/
<<<<<<< HEAD
=======
obj-$(CONFIG_RAPIDIO)		+= devices/
>>>>>>> refs/remotes/origin/cm-10.0
=======
obj-$(CONFIG_RAPIDIO) += rapidio.o
rapidio-y := rio.o rio-access.o rio-driver.o rio-sysfs.o

obj-$(CONFIG_RAPIDIO_ENUM_BASIC) += rio-scan.o

obj-$(CONFIG_RAPIDIO)		+= switches/
obj-$(CONFIG_RAPIDIO)		+= devices/
>>>>>>> refs/remotes/origin/master

subdir-ccflags-$(CONFIG_RAPIDIO_DEBUG) := -DDEBUG
