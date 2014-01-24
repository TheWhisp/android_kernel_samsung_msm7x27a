snd-hda-intel-objs := hda_intel.o
<<<<<<< HEAD

<<<<<<< HEAD
snd-hda-codec-y := hda_codec.o
=======
snd-hda-codec-y := hda_codec.o hda_jack.o
>>>>>>> refs/remotes/origin/cm-10.0
snd-hda-codec-$(CONFIG_SND_HDA_GENERIC) += hda_generic.o
=======
# for haswell power well
snd-hda-intel-$(CONFIG_SND_HDA_I915) +=	hda_i915.o

snd-hda-codec-y := hda_codec.o hda_jack.o hda_auto_parser.o
>>>>>>> refs/remotes/origin/master
snd-hda-codec-$(CONFIG_PROC_FS) += hda_proc.o
snd-hda-codec-$(CONFIG_SND_HDA_HWDEP) += hda_hwdep.o
snd-hda-codec-$(CONFIG_SND_HDA_INPUT_BEEP) += hda_beep.o

<<<<<<< HEAD
<<<<<<< HEAD
=======
# for trace-points
CFLAGS_hda_codec.o := -I$(src)

>>>>>>> refs/remotes/origin/cm-10.0
=======
# for trace-points
CFLAGS_hda_codec.o := -I$(src)
CFLAGS_hda_intel.o := -I$(src)

snd-hda-codec-generic-objs :=	hda_generic.o
>>>>>>> refs/remotes/origin/master
snd-hda-codec-realtek-objs :=	patch_realtek.o
snd-hda-codec-cmedia-objs :=	patch_cmedia.o
snd-hda-codec-analog-objs :=	patch_analog.o
snd-hda-codec-idt-objs :=	patch_sigmatel.o
snd-hda-codec-si3054-objs :=	patch_si3054.o
snd-hda-codec-cirrus-objs :=	patch_cirrus.o
snd-hda-codec-ca0110-objs :=	patch_ca0110.o
<<<<<<< HEAD
<<<<<<< HEAD
=======
snd-hda-codec-ca0132-objs :=	patch_ca0132.o
>>>>>>> refs/remotes/origin/cm-10.0
=======
snd-hda-codec-ca0132-objs :=	patch_ca0132.o
>>>>>>> refs/remotes/origin/master
snd-hda-codec-conexant-objs :=	patch_conexant.o
snd-hda-codec-via-objs :=	patch_via.o
snd-hda-codec-hdmi-objs :=	patch_hdmi.o hda_eld.o

# common driver
obj-$(CONFIG_SND_HDA_INTEL) := snd-hda-codec.o

<<<<<<< HEAD
# codec drivers (note: CONFIG_SND_HDA_CODEC_XXX are booleans)
ifdef CONFIG_SND_HDA_CODEC_REALTEK
obj-$(CONFIG_SND_HDA_INTEL) += snd-hda-codec-realtek.o
endif
ifdef CONFIG_SND_HDA_CODEC_CMEDIA
obj-$(CONFIG_SND_HDA_INTEL) += snd-hda-codec-cmedia.o
endif
ifdef CONFIG_SND_HDA_CODEC_ANALOG
obj-$(CONFIG_SND_HDA_INTEL) += snd-hda-codec-analog.o
endif
ifdef CONFIG_SND_HDA_CODEC_SIGMATEL
obj-$(CONFIG_SND_HDA_INTEL) += snd-hda-codec-idt.o
endif
ifdef CONFIG_SND_HDA_CODEC_SI3054
obj-$(CONFIG_SND_HDA_INTEL) += snd-hda-codec-si3054.o
endif
ifdef CONFIG_SND_HDA_CODEC_CIRRUS
obj-$(CONFIG_SND_HDA_INTEL) += snd-hda-codec-cirrus.o
endif
ifdef CONFIG_SND_HDA_CODEC_CA0110
obj-$(CONFIG_SND_HDA_INTEL) += snd-hda-codec-ca0110.o
endif
<<<<<<< HEAD
=======
ifdef CONFIG_SND_HDA_CODEC_CA0132
obj-$(CONFIG_SND_HDA_INTEL) += snd-hda-codec-ca0132.o
endif
>>>>>>> refs/remotes/origin/cm-10.0
ifdef CONFIG_SND_HDA_CODEC_CONEXANT
obj-$(CONFIG_SND_HDA_INTEL) += snd-hda-codec-conexant.o
endif
ifdef CONFIG_SND_HDA_CODEC_VIA
obj-$(CONFIG_SND_HDA_INTEL) += snd-hda-codec-via.o
endif
ifdef CONFIG_SND_HDA_CODEC_HDMI
obj-$(CONFIG_SND_HDA_INTEL) += snd-hda-codec-hdmi.o
endif
=======
# codec drivers
obj-$(CONFIG_SND_HDA_GENERIC) += snd-hda-codec-generic.o
obj-$(CONFIG_SND_HDA_CODEC_REALTEK) += snd-hda-codec-realtek.o
obj-$(CONFIG_SND_HDA_CODEC_CMEDIA) += snd-hda-codec-cmedia.o
obj-$(CONFIG_SND_HDA_CODEC_ANALOG) += snd-hda-codec-analog.o
obj-$(CONFIG_SND_HDA_CODEC_SIGMATEL) += snd-hda-codec-idt.o
obj-$(CONFIG_SND_HDA_CODEC_SI3054) += snd-hda-codec-si3054.o
obj-$(CONFIG_SND_HDA_CODEC_CIRRUS) += snd-hda-codec-cirrus.o
obj-$(CONFIG_SND_HDA_CODEC_CA0110) += snd-hda-codec-ca0110.o
obj-$(CONFIG_SND_HDA_CODEC_CA0132) += snd-hda-codec-ca0132.o
obj-$(CONFIG_SND_HDA_CODEC_CONEXANT) += snd-hda-codec-conexant.o
obj-$(CONFIG_SND_HDA_CODEC_VIA) += snd-hda-codec-via.o
obj-$(CONFIG_SND_HDA_CODEC_HDMI) += snd-hda-codec-hdmi.o
>>>>>>> refs/remotes/origin/master

# this must be the last entry after codec drivers;
# otherwise the codec patches won't be hooked before the PCI probe
# when built in kernel
obj-$(CONFIG_SND_HDA_INTEL) += snd-hda-intel.o
