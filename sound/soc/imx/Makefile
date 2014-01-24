# i.MX Platform Support
<<<<<<< HEAD
snd-soc-imx-objs := imx-ssi.o
snd-soc-imx-fiq-objs := imx-pcm-fiq.o
snd-soc-imx-mx2-objs := imx-pcm-dma-mx2.o

obj-$(CONFIG_SND_IMX_SOC) += snd-soc-imx.o
obj-$(CONFIG_SND_MXC_SOC_FIQ) += snd-soc-imx-fiq.o
obj-$(CONFIG_SND_MXC_SOC_MX2) += snd-soc-imx-mx2.o
=======
snd-soc-imx-ssi-objs := imx-ssi.o
snd-soc-imx-audmux-objs := imx-audmux.o

obj-$(CONFIG_SND_SOC_IMX_SSI) += snd-soc-imx-ssi.o
obj-$(CONFIG_SND_SOC_IMX_AUDMUX) += snd-soc-imx-audmux.o

obj-$(CONFIG_SND_SOC_IMX_PCM) += snd-soc-imx-pcm.o
snd-soc-imx-pcm-y := imx-pcm.o
snd-soc-imx-pcm-$(CONFIG_SND_MXC_SOC_FIQ) += imx-pcm-fiq.o
snd-soc-imx-pcm-$(CONFIG_SND_MXC_SOC_MX2) += imx-pcm-dma-mx2.o
>>>>>>> refs/remotes/origin/cm-10.0

# i.MX Machine Support
snd-soc-eukrea-tlv320-objs := eukrea-tlv320.o
snd-soc-phycore-ac97-objs := phycore-ac97.o
snd-soc-mx27vis-aic32x4-objs := mx27vis-aic32x4.o
snd-soc-wm1133-ev1-objs := wm1133-ev1.o

obj-$(CONFIG_SND_SOC_EUKREA_TLV320) += snd-soc-eukrea-tlv320.o
obj-$(CONFIG_SND_SOC_PHYCORE_AC97) += snd-soc-phycore-ac97.o
obj-$(CONFIG_SND_SOC_MX27VIS_AIC32X4) += snd-soc-mx27vis-aic32x4.o
obj-$(CONFIG_SND_MXC_SOC_WM1133_EV1) += snd-soc-wm1133-ev1.o
