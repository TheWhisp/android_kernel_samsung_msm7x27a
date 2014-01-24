#
# Makefile for Marvell's PXA168 processors line
#

<<<<<<< HEAD
obj-y				+= common.o clock.o devices.o time.o

# SoC support
obj-$(CONFIG_CPU_PXA168)	+= pxa168.o irq-pxa168.o
obj-$(CONFIG_CPU_PXA910)	+= pxa910.o irq-pxa168.o
<<<<<<< HEAD
obj-$(CONFIG_CPU_MMP2)		+= mmp2.o irq-mmp2.o
=======
obj-$(CONFIG_CPU_MMP2)		+= mmp2.o irq-mmp2.o sram.o
>>>>>>> refs/remotes/origin/cm-10.0
=======
obj-y				+= common.o devices.o time.o

# SoC support
obj-$(CONFIG_CPU_PXA168)	+= pxa168.o
obj-$(CONFIG_CPU_PXA910)	+= pxa910.o
obj-$(CONFIG_CPU_MMP2)		+= mmp2.o sram.o

ifeq ($(CONFIG_COMMON_CLK), )
obj-y				+= clock.o
obj-$(CONFIG_CPU_PXA168)	+= clock-pxa168.o
obj-$(CONFIG_CPU_PXA910)	+= clock-pxa910.o
obj-$(CONFIG_CPU_MMP2)		+= clock-mmp2.o
endif
ifeq ($(CONFIG_PM),y)
obj-$(CONFIG_CPU_PXA910)	+= pm-pxa910.o
obj-$(CONFIG_CPU_MMP2)		+= pm-mmp2.o
endif
>>>>>>> refs/remotes/origin/master

# board support
obj-$(CONFIG_MACH_ASPENITE)	+= aspenite.o
obj-$(CONFIG_MACH_ZYLONITE2)	+= aspenite.o
obj-$(CONFIG_MACH_AVENGERS_LITE)+= avengers_lite.o
obj-$(CONFIG_MACH_TAVOREVB)	+= tavorevb.o
obj-$(CONFIG_MACH_TTC_DKB)	+= ttc_dkb.o
obj-$(CONFIG_MACH_BROWNSTONE)	+= brownstone.o
obj-$(CONFIG_MACH_FLINT)	+= flint.o
obj-$(CONFIG_MACH_MARVELL_JASPER) += jasper.o
<<<<<<< HEAD
<<<<<<< HEAD
obj-$(CONFIG_MACH_TETON_BGA)	+= teton_bga.o
=======
obj-$(CONFIG_MACH_MMP_DT)	+= mmp-dt.o
obj-$(CONFIG_MACH_TETON_BGA)	+= teton_bga.o
obj-$(CONFIG_MACH_GPLUGD)	+= gplugd.o
>>>>>>> refs/remotes/origin/cm-10.0
=======
obj-$(CONFIG_MACH_MMP_DT)	+= mmp-dt.o
obj-$(CONFIG_MACH_MMP2_DT)	+= mmp2-dt.o
obj-$(CONFIG_MACH_TETON_BGA)	+= teton_bga.o
obj-$(CONFIG_MACH_GPLUGD)	+= gplugd.o
>>>>>>> refs/remotes/origin/master
