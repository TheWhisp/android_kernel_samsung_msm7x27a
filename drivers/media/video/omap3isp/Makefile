# Makefile for OMAP3 ISP driver

<<<<<<< HEAD
ifdef CONFIG_VIDEO_OMAP3_DEBUG
EXTRA_CFLAGS += -DDEBUG
endif
=======
ccflags-$(CONFIG_VIDEO_OMAP3_DEBUG) += -DDEBUG
>>>>>>> refs/remotes/origin/cm-10.0

omap3-isp-objs += \
	isp.o ispqueue.o ispvideo.o \
	ispcsiphy.o ispccp2.o ispcsi2.o \
	ispccdc.o isppreview.o ispresizer.o \
	ispstat.o isph3a_aewb.o isph3a_af.o isphist.o

obj-$(CONFIG_VIDEO_OMAP3) += omap3-isp.o
