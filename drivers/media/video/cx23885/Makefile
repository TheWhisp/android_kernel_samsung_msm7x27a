cx23885-objs	:= cx23885-cards.o cx23885-video.o cx23885-vbi.o \
		    cx23885-core.o cx23885-i2c.o cx23885-dvb.o cx23885-417.o \
		    cx23885-ioctl.o cx23885-ir.o cx23885-av.o cx23885-input.o \
		    cx23888-ir.o netup-init.o cimax2.o netup-eeprom.o \
<<<<<<< HEAD
		    cx23885-f300.o
=======
		    cx23885-f300.o cx23885-alsa.o
>>>>>>> refs/remotes/origin/cm-10.0

obj-$(CONFIG_VIDEO_CX23885) += cx23885.o
obj-$(CONFIG_MEDIA_ALTERA_CI) += altera-ci.o

<<<<<<< HEAD
EXTRA_CFLAGS += -Idrivers/media/video
EXTRA_CFLAGS += -Idrivers/media/common/tuners
EXTRA_CFLAGS += -Idrivers/media/dvb/dvb-core
EXTRA_CFLAGS += -Idrivers/media/dvb/frontends

EXTRA_CFLAGS += $(extra-cflags-y) $(extra-cflags-m)
=======
ccflags-y += -Idrivers/media/video
ccflags-y += -Idrivers/media/common/tuners
ccflags-y += -Idrivers/media/dvb/dvb-core
ccflags-y += -Idrivers/media/dvb/frontends

ccflags-y += $(extra-cflags-y) $(extra-cflags-m)
>>>>>>> refs/remotes/origin/cm-10.0
