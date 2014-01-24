/*
 * kirkwood-i2s.c
 *
 * (c) 2010 Arnaud Patard <apatard@mandriva.com>
 * (c) 2010 Arnaud Patard <arnaud.patard@rtp-net.org>
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/mbus.h>
#include <linux/delay.h>
<<<<<<< HEAD
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <plat/audio.h>
#include "kirkwood.h"

#define DRV_NAME	"kirkwood-i2s"

#define KIRKWOOD_I2S_RATES \
	(SNDRV_PCM_RATE_44100 | \
	 SNDRV_PCM_RATE_48000 | SNDRV_PCM_RATE_96000)
=======
#include <linux/clk.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <linux/platform_data/asoc-kirkwood.h>
#include <linux/of.h>

#include "kirkwood.h"

#define DRV_NAME	"mvebu-audio"

>>>>>>> refs/remotes/origin/master
#define KIRKWOOD_I2S_FORMATS \
	(SNDRV_PCM_FMTBIT_S16_LE | \
	 SNDRV_PCM_FMTBIT_S24_LE | \
	 SNDRV_PCM_FMTBIT_S32_LE)

<<<<<<< HEAD
=======
#define KIRKWOOD_SPDIF_FORMATS \
	(SNDRV_PCM_FMTBIT_S16_LE | \
	 SNDRV_PCM_FMTBIT_S24_LE)

>>>>>>> refs/remotes/origin/master
static int kirkwood_i2s_set_fmt(struct snd_soc_dai *cpu_dai,
		unsigned int fmt)
{
	struct kirkwood_dma_data *priv = snd_soc_dai_get_drvdata(cpu_dai);
	unsigned long mask;
	unsigned long value;

	switch (fmt & SND_SOC_DAIFMT_FORMAT_MASK) {
	case SND_SOC_DAIFMT_RIGHT_J:
		mask = KIRKWOOD_I2S_CTL_RJ;
		break;
	case SND_SOC_DAIFMT_LEFT_J:
		mask = KIRKWOOD_I2S_CTL_LJ;
		break;
	case SND_SOC_DAIFMT_I2S:
		mask = KIRKWOOD_I2S_CTL_I2S;
		break;
	default:
		return -EINVAL;
	}

	/*
	 * Set same format for playback and record
	 * This avoids some troubles.
	 */
	value = readl(priv->io+KIRKWOOD_I2S_PLAYCTL);
	value &= ~KIRKWOOD_I2S_CTL_JUST_MASK;
	value |= mask;
	writel(value, priv->io+KIRKWOOD_I2S_PLAYCTL);

	value = readl(priv->io+KIRKWOOD_I2S_RECCTL);
	value &= ~KIRKWOOD_I2S_CTL_JUST_MASK;
	value |= mask;
	writel(value, priv->io+KIRKWOOD_I2S_RECCTL);

	return 0;
}

static inline void kirkwood_set_dco(void __iomem *io, unsigned long rate)
{
	unsigned long value;

	value = KIRKWOOD_DCO_CTL_OFFSET_0;
	switch (rate) {
	default:
	case 44100:
		value |= KIRKWOOD_DCO_CTL_FREQ_11;
		break;
	case 48000:
		value |= KIRKWOOD_DCO_CTL_FREQ_12;
		break;
	case 96000:
		value |= KIRKWOOD_DCO_CTL_FREQ_24;
		break;
	}
	writel(value, io + KIRKWOOD_DCO_CTL);

	/* wait for dco locked */
	do {
		cpu_relax();
		value = readl(io + KIRKWOOD_DCO_SPCR_STATUS);
<<<<<<< HEAD
		value &= KIRKWOOD_DCO_SPCR_STATUS;
	} while (value == 0);
}

=======
		value &= KIRKWOOD_DCO_SPCR_STATUS_DCO_LOCK;
	} while (value == 0);
}

static void kirkwood_set_rate(struct snd_soc_dai *dai,
	struct kirkwood_dma_data *priv, unsigned long rate)
{
	uint32_t clks_ctrl;

	if (IS_ERR(priv->extclk)) {
		/* use internal dco for the supported rates
		 * defined in kirkwood_i2s_dai */
		dev_dbg(dai->dev, "%s: dco set rate = %lu\n",
			__func__, rate);
		kirkwood_set_dco(priv->io, rate);

		clks_ctrl = KIRKWOOD_MCLK_SOURCE_DCO;
	} else {
		/* use the external clock for the other rates
		 * defined in kirkwood_i2s_dai_extclk */
		dev_dbg(dai->dev, "%s: extclk set rate = %lu -> %lu\n",
			__func__, rate, 256 * rate);
		clk_set_rate(priv->extclk, 256 * rate);

		clks_ctrl = KIRKWOOD_MCLK_SOURCE_EXTCLK;
	}
	writel(clks_ctrl, priv->io + KIRKWOOD_CLOCKS_CTRL);
}

>>>>>>> refs/remotes/origin/master
static int kirkwood_i2s_startup(struct snd_pcm_substream *substream,
		struct snd_soc_dai *dai)
{
	struct kirkwood_dma_data *priv = snd_soc_dai_get_drvdata(dai);

	snd_soc_dai_set_dma_data(dai, substream, priv);
	return 0;
}

static int kirkwood_i2s_hw_params(struct snd_pcm_substream *substream,
				 struct snd_pcm_hw_params *params,
				 struct snd_soc_dai *dai)
{
	struct kirkwood_dma_data *priv = snd_soc_dai_get_drvdata(dai);
<<<<<<< HEAD
	unsigned int i2s_reg, reg;
	unsigned long i2s_value, value;

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
		i2s_reg = KIRKWOOD_I2S_PLAYCTL;
		reg = KIRKWOOD_PLAYCTL;
	} else {
		i2s_reg = KIRKWOOD_I2S_RECCTL;
		reg = KIRKWOOD_RECCTL;
	}

	/* set dco conf */
	kirkwood_set_dco(priv->io, params_rate(params));
=======
	uint32_t ctl_play, ctl_rec;
	unsigned int i2s_reg;
	unsigned long i2s_value;

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
		i2s_reg = KIRKWOOD_I2S_PLAYCTL;
	} else {
		i2s_reg = KIRKWOOD_I2S_RECCTL;
	}

	kirkwood_set_rate(dai, priv, params_rate(params));
>>>>>>> refs/remotes/origin/master

	i2s_value = readl(priv->io+i2s_reg);
	i2s_value &= ~KIRKWOOD_I2S_CTL_SIZE_MASK;

<<<<<<< HEAD
	value = readl(priv->io+reg);
	value &= ~KIRKWOOD_PLAYCTL_SIZE_MASK;

=======
>>>>>>> refs/remotes/origin/master
	/*
	 * Size settings in play/rec i2s control regs and play/rec control
	 * regs must be the same.
	 */
	switch (params_format(params)) {
	case SNDRV_PCM_FORMAT_S16_LE:
		i2s_value |= KIRKWOOD_I2S_CTL_SIZE_16;
<<<<<<< HEAD
		value |= KIRKWOOD_PLAYCTL_SIZE_16_C;
=======
		ctl_play = KIRKWOOD_PLAYCTL_SIZE_16_C |
			   KIRKWOOD_PLAYCTL_I2S_EN |
			   KIRKWOOD_PLAYCTL_SPDIF_EN;
		ctl_rec = KIRKWOOD_RECCTL_SIZE_16_C |
			  KIRKWOOD_RECCTL_I2S_EN |
			  KIRKWOOD_RECCTL_SPDIF_EN;
>>>>>>> refs/remotes/origin/master
		break;
	/*
	 * doesn't work... S20_3LE != kirkwood 20bit format ?
	 *
	case SNDRV_PCM_FORMAT_S20_3LE:
		i2s_value |= KIRKWOOD_I2S_CTL_SIZE_20;
<<<<<<< HEAD
		value |= KIRKWOOD_PLAYCTL_SIZE_20;
=======
		ctl_play = KIRKWOOD_PLAYCTL_SIZE_20 |
			   KIRKWOOD_PLAYCTL_I2S_EN;
		ctl_rec = KIRKWOOD_RECCTL_SIZE_20 |
			  KIRKWOOD_RECCTL_I2S_EN;
>>>>>>> refs/remotes/origin/master
		break;
	*/
	case SNDRV_PCM_FORMAT_S24_LE:
		i2s_value |= KIRKWOOD_I2S_CTL_SIZE_24;
<<<<<<< HEAD
		value |= KIRKWOOD_PLAYCTL_SIZE_24;
		break;
	case SNDRV_PCM_FORMAT_S32_LE:
		i2s_value |= KIRKWOOD_I2S_CTL_SIZE_32;
		value |= KIRKWOOD_PLAYCTL_SIZE_32;
=======
		ctl_play = KIRKWOOD_PLAYCTL_SIZE_24 |
			   KIRKWOOD_PLAYCTL_I2S_EN |
			   KIRKWOOD_PLAYCTL_SPDIF_EN;
		ctl_rec = KIRKWOOD_RECCTL_SIZE_24 |
			  KIRKWOOD_RECCTL_I2S_EN |
			  KIRKWOOD_RECCTL_SPDIF_EN;
		break;
	case SNDRV_PCM_FORMAT_S32_LE:
		i2s_value |= KIRKWOOD_I2S_CTL_SIZE_32;
		ctl_play = KIRKWOOD_PLAYCTL_SIZE_32 |
			   KIRKWOOD_PLAYCTL_I2S_EN;
		ctl_rec = KIRKWOOD_RECCTL_SIZE_32 |
			  KIRKWOOD_RECCTL_I2S_EN;
>>>>>>> refs/remotes/origin/master
		break;
	default:
		return -EINVAL;
	}

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
<<<<<<< HEAD
		value &= ~KIRKWOOD_PLAYCTL_MONO_MASK;
		if (params_channels(params) == 1)
			value |= KIRKWOOD_PLAYCTL_MONO_BOTH;
		else
			value |= KIRKWOOD_PLAYCTL_MONO_OFF;
	}

	writel(i2s_value, priv->io+i2s_reg);
	writel(value, priv->io+reg);
=======
		if (params_channels(params) == 1)
			ctl_play |= KIRKWOOD_PLAYCTL_MONO_BOTH;
		else
			ctl_play |= KIRKWOOD_PLAYCTL_MONO_OFF;

		priv->ctl_play &= ~(KIRKWOOD_PLAYCTL_MONO_MASK |
				    KIRKWOOD_PLAYCTL_ENABLE_MASK |
				    KIRKWOOD_PLAYCTL_SIZE_MASK);
		priv->ctl_play |= ctl_play;
	} else {
		priv->ctl_rec &= ~KIRKWOOD_RECCTL_SIZE_MASK;
		priv->ctl_rec |= ctl_rec;
	}

	writel(i2s_value, priv->io+i2s_reg);
>>>>>>> refs/remotes/origin/master

	return 0;
}

static int kirkwood_i2s_play_trigger(struct snd_pcm_substream *substream,
				int cmd, struct snd_soc_dai *dai)
{
	struct kirkwood_dma_data *priv = snd_soc_dai_get_drvdata(dai);
<<<<<<< HEAD
	unsigned long value;

	/*
	 * specs says KIRKWOOD_PLAYCTL must be read 2 times before
	 * changing it. So read 1 time here and 1 later.
	 */
	value = readl(priv->io + KIRKWOOD_PLAYCTL);

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
		/* stop audio, enable interrupts */
		value = readl(priv->io + KIRKWOOD_PLAYCTL);
		value |= KIRKWOOD_PLAYCTL_PAUSE;
		writel(value, priv->io + KIRKWOOD_PLAYCTL);

=======
	uint32_t ctl, value;

	ctl = readl(priv->io + KIRKWOOD_PLAYCTL);
	if (ctl & KIRKWOOD_PLAYCTL_PAUSE) {
		unsigned timeout = 5000;
		/*
		 * The Armada510 spec says that if we enter pause mode, the
		 * busy bit must be read back as clear _twice_.  Make sure
		 * we respect that otherwise we get DMA underruns.
		 */
		do {
			value = ctl;
			ctl = readl(priv->io + KIRKWOOD_PLAYCTL);
			if (!((ctl | value) & KIRKWOOD_PLAYCTL_PLAY_BUSY))
				break;
			udelay(1);
		} while (timeout--);

		if ((ctl | value) & KIRKWOOD_PLAYCTL_PLAY_BUSY)
			dev_notice(dai->dev, "timed out waiting for busy to deassert: %08x\n",
				   ctl);
	}

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
		/* configure */
		ctl = priv->ctl_play;
		if (dai->id == 0)
			ctl &= ~KIRKWOOD_PLAYCTL_SPDIF_EN;	/* i2s */
		else
			ctl &= ~KIRKWOOD_PLAYCTL_I2S_EN;	/* spdif */

		value = ctl & ~KIRKWOOD_PLAYCTL_ENABLE_MASK;
		writel(value, priv->io + KIRKWOOD_PLAYCTL);

		/* enable interrupts */
>>>>>>> refs/remotes/origin/master
		value = readl(priv->io + KIRKWOOD_INT_MASK);
		value |= KIRKWOOD_INT_CAUSE_PLAY_BYTES;
		writel(value, priv->io + KIRKWOOD_INT_MASK);

<<<<<<< HEAD
		/* configure audio & enable i2s playback */
		value = readl(priv->io + KIRKWOOD_PLAYCTL);
		value &= ~KIRKWOOD_PLAYCTL_BURST_MASK;
		value &= ~(KIRKWOOD_PLAYCTL_PAUSE | KIRKWOOD_PLAYCTL_I2S_MUTE
				| KIRKWOOD_PLAYCTL_SPDIF_EN);

		if (priv->burst == 32)
			value |= KIRKWOOD_PLAYCTL_BURST_32;
		else
			value |= KIRKWOOD_PLAYCTL_BURST_128;
		value |= KIRKWOOD_PLAYCTL_I2S_EN;
		writel(value, priv->io + KIRKWOOD_PLAYCTL);
=======
		/* enable playback */
		writel(ctl, priv->io + KIRKWOOD_PLAYCTL);
>>>>>>> refs/remotes/origin/master
		break;

	case SNDRV_PCM_TRIGGER_STOP:
		/* stop audio, disable interrupts */
<<<<<<< HEAD
		value = readl(priv->io + KIRKWOOD_PLAYCTL);
		value |= KIRKWOOD_PLAYCTL_PAUSE | KIRKWOOD_PLAYCTL_I2S_MUTE;
		writel(value, priv->io + KIRKWOOD_PLAYCTL);
=======
		ctl |= KIRKWOOD_PLAYCTL_PAUSE | KIRKWOOD_PLAYCTL_I2S_MUTE |
				KIRKWOOD_PLAYCTL_SPDIF_MUTE;
		writel(ctl, priv->io + KIRKWOOD_PLAYCTL);
>>>>>>> refs/remotes/origin/master

		value = readl(priv->io + KIRKWOOD_INT_MASK);
		value &= ~KIRKWOOD_INT_CAUSE_PLAY_BYTES;
		writel(value, priv->io + KIRKWOOD_INT_MASK);

		/* disable all playbacks */
<<<<<<< HEAD
		value = readl(priv->io + KIRKWOOD_PLAYCTL);
		value &= ~(KIRKWOOD_PLAYCTL_I2S_EN | KIRKWOOD_PLAYCTL_SPDIF_EN);
		writel(value, priv->io + KIRKWOOD_PLAYCTL);
=======
		ctl &= ~KIRKWOOD_PLAYCTL_ENABLE_MASK;
		writel(ctl, priv->io + KIRKWOOD_PLAYCTL);
>>>>>>> refs/remotes/origin/master
		break;

	case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
	case SNDRV_PCM_TRIGGER_SUSPEND:
<<<<<<< HEAD
		value = readl(priv->io + KIRKWOOD_PLAYCTL);
		value |= KIRKWOOD_PLAYCTL_PAUSE | KIRKWOOD_PLAYCTL_I2S_MUTE;
		writel(value, priv->io + KIRKWOOD_PLAYCTL);
=======
		ctl |= KIRKWOOD_PLAYCTL_PAUSE | KIRKWOOD_PLAYCTL_I2S_MUTE |
				KIRKWOOD_PLAYCTL_SPDIF_MUTE;
		writel(ctl, priv->io + KIRKWOOD_PLAYCTL);
>>>>>>> refs/remotes/origin/master
		break;

	case SNDRV_PCM_TRIGGER_RESUME:
	case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
<<<<<<< HEAD
		value = readl(priv->io + KIRKWOOD_PLAYCTL);
		value &= ~(KIRKWOOD_PLAYCTL_PAUSE | KIRKWOOD_PLAYCTL_I2S_MUTE);
		writel(value, priv->io + KIRKWOOD_PLAYCTL);
=======
		ctl &= ~(KIRKWOOD_PLAYCTL_PAUSE | KIRKWOOD_PLAYCTL_I2S_MUTE |
				KIRKWOOD_PLAYCTL_SPDIF_MUTE);
		writel(ctl, priv->io + KIRKWOOD_PLAYCTL);
>>>>>>> refs/remotes/origin/master
		break;

	default:
		return -EINVAL;
	}

	return 0;
}

static int kirkwood_i2s_rec_trigger(struct snd_pcm_substream *substream,
				int cmd, struct snd_soc_dai *dai)
{
	struct kirkwood_dma_data *priv = snd_soc_dai_get_drvdata(dai);
<<<<<<< HEAD
	unsigned long value;
=======
	uint32_t ctl, value;
>>>>>>> refs/remotes/origin/master

	value = readl(priv->io + KIRKWOOD_RECCTL);

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
<<<<<<< HEAD
		/* stop audio, enable interrupts */
		value = readl(priv->io + KIRKWOOD_RECCTL);
		value |= KIRKWOOD_RECCTL_PAUSE;
		writel(value, priv->io + KIRKWOOD_RECCTL);

=======
		/* configure */
		ctl = priv->ctl_rec;
		if (dai->id == 0)
			ctl &= ~KIRKWOOD_RECCTL_SPDIF_EN;	/* i2s */
		else
			ctl &= ~KIRKWOOD_RECCTL_I2S_EN;		/* spdif */

		value = ctl & ~(KIRKWOOD_RECCTL_I2S_EN |
				KIRKWOOD_RECCTL_SPDIF_EN);
		writel(value, priv->io + KIRKWOOD_RECCTL);

		/* enable interrupts */
>>>>>>> refs/remotes/origin/master
		value = readl(priv->io + KIRKWOOD_INT_MASK);
		value |= KIRKWOOD_INT_CAUSE_REC_BYTES;
		writel(value, priv->io + KIRKWOOD_INT_MASK);

<<<<<<< HEAD
		/* configure audio & enable i2s record */
		value = readl(priv->io + KIRKWOOD_RECCTL);
		value &= ~KIRKWOOD_RECCTL_BURST_MASK;
		value &= ~KIRKWOOD_RECCTL_MONO;
		value &= ~(KIRKWOOD_RECCTL_PAUSE | KIRKWOOD_RECCTL_MUTE
			| KIRKWOOD_RECCTL_SPDIF_EN);

		if (priv->burst == 32)
			value |= KIRKWOOD_RECCTL_BURST_32;
		else
			value |= KIRKWOOD_RECCTL_BURST_128;
		value |= KIRKWOOD_RECCTL_I2S_EN;

		writel(value, priv->io + KIRKWOOD_RECCTL);
=======
		/* enable record */
		writel(ctl, priv->io + KIRKWOOD_RECCTL);
>>>>>>> refs/remotes/origin/master
		break;

	case SNDRV_PCM_TRIGGER_STOP:
		/* stop audio, disable interrupts */
		value = readl(priv->io + KIRKWOOD_RECCTL);
		value |= KIRKWOOD_RECCTL_PAUSE | KIRKWOOD_RECCTL_MUTE;
		writel(value, priv->io + KIRKWOOD_RECCTL);

		value = readl(priv->io + KIRKWOOD_INT_MASK);
		value &= ~KIRKWOOD_INT_CAUSE_REC_BYTES;
		writel(value, priv->io + KIRKWOOD_INT_MASK);

		/* disable all records */
		value = readl(priv->io + KIRKWOOD_RECCTL);
		value &= ~(KIRKWOOD_RECCTL_I2S_EN | KIRKWOOD_RECCTL_SPDIF_EN);
		writel(value, priv->io + KIRKWOOD_RECCTL);
		break;

	case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
	case SNDRV_PCM_TRIGGER_SUSPEND:
		value = readl(priv->io + KIRKWOOD_RECCTL);
		value |= KIRKWOOD_RECCTL_PAUSE | KIRKWOOD_RECCTL_MUTE;
		writel(value, priv->io + KIRKWOOD_RECCTL);
		break;

	case SNDRV_PCM_TRIGGER_RESUME:
	case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
		value = readl(priv->io + KIRKWOOD_RECCTL);
		value &= ~(KIRKWOOD_RECCTL_PAUSE | KIRKWOOD_RECCTL_MUTE);
		writel(value, priv->io + KIRKWOOD_RECCTL);
		break;

	default:
		return -EINVAL;
	}

	return 0;
}

static int kirkwood_i2s_trigger(struct snd_pcm_substream *substream, int cmd,
			       struct snd_soc_dai *dai)
{
	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		return kirkwood_i2s_play_trigger(substream, cmd, dai);
	else
		return kirkwood_i2s_rec_trigger(substream, cmd, dai);

	return 0;
}

<<<<<<< HEAD
static int kirkwood_i2s_probe(struct snd_soc_dai *dai)
{
	struct kirkwood_dma_data *priv = snd_soc_dai_get_drvdata(dai);
=======
static int kirkwood_i2s_init(struct kirkwood_dma_data *priv)
{
>>>>>>> refs/remotes/origin/master
	unsigned long value;
	unsigned int reg_data;

	/* put system in a "safe" state : */
	/* disable audio interrupts */
	writel(0xffffffff, priv->io + KIRKWOOD_INT_CAUSE);
	writel(0, priv->io + KIRKWOOD_INT_MASK);

	reg_data = readl(priv->io + 0x1200);
	reg_data &= (~(0x333FF8));
	reg_data |= 0x111D18;
	writel(reg_data, priv->io + 0x1200);

	msleep(500);

	reg_data = readl(priv->io + 0x1200);
	reg_data &= (~(0x333FF8));
	reg_data |= 0x111D18;
	writel(reg_data, priv->io + 0x1200);

	/* disable playback/record */
	value = readl(priv->io + KIRKWOOD_PLAYCTL);
<<<<<<< HEAD
	value &= ~(KIRKWOOD_PLAYCTL_I2S_EN|KIRKWOOD_PLAYCTL_SPDIF_EN);
=======
	value &= ~KIRKWOOD_PLAYCTL_ENABLE_MASK;
>>>>>>> refs/remotes/origin/master
	writel(value, priv->io + KIRKWOOD_PLAYCTL);

	value = readl(priv->io + KIRKWOOD_RECCTL);
	value &= ~(KIRKWOOD_RECCTL_I2S_EN | KIRKWOOD_RECCTL_SPDIF_EN);
	writel(value, priv->io + KIRKWOOD_RECCTL);

	return 0;

}

<<<<<<< HEAD
static int kirkwood_i2s_remove(struct snd_soc_dai *dai)
{
	return 0;
}

<<<<<<< HEAD
static struct snd_soc_dai_ops kirkwood_i2s_dai_ops = {
=======
static const struct snd_soc_dai_ops kirkwood_i2s_dai_ops = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static const struct snd_soc_dai_ops kirkwood_i2s_dai_ops = {
>>>>>>> refs/remotes/origin/master
	.startup	= kirkwood_i2s_startup,
	.trigger	= kirkwood_i2s_trigger,
	.hw_params      = kirkwood_i2s_hw_params,
	.set_fmt        = kirkwood_i2s_set_fmt,
};

<<<<<<< HEAD

static struct snd_soc_dai_driver kirkwood_i2s_dai = {
	.probe = kirkwood_i2s_probe,
	.remove = kirkwood_i2s_remove,
	.playback = {
		.channels_min = 1,
		.channels_max = 2,
		.rates = KIRKWOOD_I2S_RATES,
		.formats = KIRKWOOD_I2S_FORMATS,},
	.capture = {
		.channels_min = 1,
		.channels_max = 2,
		.rates = KIRKWOOD_I2S_RATES,
		.formats = KIRKWOOD_I2S_FORMATS,},
	.ops = &kirkwood_i2s_dai_ops,
};

static __devinit int kirkwood_i2s_dev_probe(struct platform_device *pdev)
{
	struct resource *mem;
	struct kirkwood_asoc_platform_data *data =
		pdev->dev.platform_data;
	struct kirkwood_dma_data *priv;
	int err;

	priv = kzalloc(sizeof(struct kirkwood_dma_data), GFP_KERNEL);
	if (!priv) {
		dev_err(&pdev->dev, "allocation failed\n");
		err = -ENOMEM;
		goto error;
=======
static struct snd_soc_dai_driver kirkwood_i2s_dai[2] = {
    {
	.name = "i2s",
	.id = 0,
	.playback = {
		.channels_min = 1,
		.channels_max = 2,
		.rates = SNDRV_PCM_RATE_44100 | SNDRV_PCM_RATE_48000 |
				SNDRV_PCM_RATE_96000,
		.formats = KIRKWOOD_I2S_FORMATS,
	},
	.capture = {
		.channels_min = 1,
		.channels_max = 2,
		.rates = SNDRV_PCM_RATE_44100 | SNDRV_PCM_RATE_48000 |
				SNDRV_PCM_RATE_96000,
		.formats = KIRKWOOD_I2S_FORMATS,
	},
	.ops = &kirkwood_i2s_dai_ops,
    },
    {
	.name = "spdif",
	.id = 1,
	.playback = {
		.channels_min = 1,
		.channels_max = 2,
		.rates = SNDRV_PCM_RATE_44100 | SNDRV_PCM_RATE_48000 |
				SNDRV_PCM_RATE_96000,
		.formats = KIRKWOOD_SPDIF_FORMATS,
	},
	.capture = {
		.channels_min = 1,
		.channels_max = 2,
		.rates = SNDRV_PCM_RATE_44100 | SNDRV_PCM_RATE_48000 |
				SNDRV_PCM_RATE_96000,
		.formats = KIRKWOOD_SPDIF_FORMATS,
	},
	.ops = &kirkwood_i2s_dai_ops,
    },
};

static struct snd_soc_dai_driver kirkwood_i2s_dai_extclk[2] = {
    {
	.name = "i2s",
	.id = 0,
	.playback = {
		.channels_min = 1,
		.channels_max = 2,
		.rates = SNDRV_PCM_RATE_CONTINUOUS,
		.rate_min = 5512,
		.rate_max = 192000,
		.formats = KIRKWOOD_I2S_FORMATS,
	},
	.capture = {
		.channels_min = 1,
		.channels_max = 2,
		.rates = SNDRV_PCM_RATE_CONTINUOUS,
		.rate_min = 5512,
		.rate_max = 192000,
		.formats = KIRKWOOD_I2S_FORMATS,
	},
	.ops = &kirkwood_i2s_dai_ops,
    },
    {
	.name = "spdif",
	.id = 1,
	.playback = {
		.channels_min = 1,
		.channels_max = 2,
		.rates = SNDRV_PCM_RATE_CONTINUOUS,
		.rate_min = 5512,
		.rate_max = 192000,
		.formats = KIRKWOOD_SPDIF_FORMATS,
	},
	.capture = {
		.channels_min = 1,
		.channels_max = 2,
		.rates = SNDRV_PCM_RATE_CONTINUOUS,
		.rate_min = 5512,
		.rate_max = 192000,
		.formats = KIRKWOOD_SPDIF_FORMATS,
	},
	.ops = &kirkwood_i2s_dai_ops,
    },
};

static const struct snd_soc_component_driver kirkwood_i2s_component = {
	.name		= DRV_NAME,
};

static int kirkwood_i2s_dev_probe(struct platform_device *pdev)
{
	struct kirkwood_asoc_platform_data *data = pdev->dev.platform_data;
	struct snd_soc_dai_driver *soc_dai = kirkwood_i2s_dai;
	struct kirkwood_dma_data *priv;
	struct resource *mem;
	struct device_node *np = pdev->dev.of_node;
	int err;

	priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
	if (!priv) {
		dev_err(&pdev->dev, "allocation failed\n");
		return -ENOMEM;
>>>>>>> refs/remotes/origin/master
	}
	dev_set_drvdata(&pdev->dev, priv);

	mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
<<<<<<< HEAD
	if (!mem) {
		dev_err(&pdev->dev, "platform_get_resource failed\n");
		err = -ENXIO;
		goto err_alloc;
	}

	priv->mem = request_mem_region(mem->start, SZ_16K, DRV_NAME);
	if (!priv->mem) {
		dev_err(&pdev->dev, "request_mem_region failed\n");
		err = -EBUSY;
<<<<<<< HEAD
		goto error;
=======
		goto err_alloc;
>>>>>>> refs/remotes/origin/cm-10.0
	}

	priv->io = ioremap(priv->mem->start, SZ_16K);
	if (!priv->io) {
		dev_err(&pdev->dev, "ioremap failed\n");
		err = -ENOMEM;
		goto err_iomem;
	}

	priv->irq = platform_get_irq(pdev, 0);
	if (priv->irq <= 0) {
		dev_err(&pdev->dev, "platform_get_irq failed\n");
		err = -ENXIO;
		goto err_ioremap;
	}

<<<<<<< HEAD
	if (!data || !data->dram) {
=======
	if (!data) {
>>>>>>> refs/remotes/origin/cm-10.0
		dev_err(&pdev->dev, "no platform data ?!\n");
		err = -EINVAL;
		goto err_ioremap;
	}

<<<<<<< HEAD
	priv->dram = data->dram;
=======
>>>>>>> refs/remotes/origin/cm-10.0
	priv->burst = data->burst;

	return snd_soc_register_dai(&pdev->dev, &kirkwood_i2s_dai);

err_ioremap:
	iounmap(priv->io);
err_iomem:
	release_mem_region(priv->mem->start, SZ_16K);
err_alloc:
	kfree(priv);
error:
	return err;
}

static __devexit int kirkwood_i2s_dev_remove(struct platform_device *pdev)
{
	struct kirkwood_dma_data *priv = dev_get_drvdata(&pdev->dev);

	snd_soc_unregister_dai(&pdev->dev);
	iounmap(priv->io);
	release_mem_region(priv->mem->start, SZ_16K);
	kfree(priv);
=======
	priv->io = devm_ioremap_resource(&pdev->dev, mem);
	if (IS_ERR(priv->io))
		return PTR_ERR(priv->io);

	priv->irq = platform_get_irq(pdev, 0);
	if (priv->irq <= 0) {
		dev_err(&pdev->dev, "platform_get_irq failed\n");
		return -ENXIO;
	}

	if (np) {
		priv->burst = 128;		/* might be 32 or 128 */
	} else if (data) {
		priv->burst = data->burst;
	} else {
		dev_err(&pdev->dev, "no DT nor platform data ?!\n");
		return -EINVAL;
	}

	priv->clk = devm_clk_get(&pdev->dev, np ? "internal" : NULL);
	if (IS_ERR(priv->clk)) {
		dev_err(&pdev->dev, "no clock\n");
		return PTR_ERR(priv->clk);
	}

	err = clk_prepare_enable(priv->clk);
	if (err < 0)
		return err;

	priv->extclk = devm_clk_get(&pdev->dev, "extclk");
	if (IS_ERR(priv->extclk)) {
		if (PTR_ERR(priv->extclk) == -EPROBE_DEFER)
			return -EPROBE_DEFER;
	} else {
		if (priv->extclk == priv->clk) {
			devm_clk_put(&pdev->dev, priv->extclk);
			priv->extclk = ERR_PTR(-EINVAL);
		} else {
			dev_info(&pdev->dev, "found external clock\n");
			clk_prepare_enable(priv->extclk);
			soc_dai = kirkwood_i2s_dai_extclk;
		}
	}

	/* Some sensible defaults - this reflects the powerup values */
	priv->ctl_play = KIRKWOOD_PLAYCTL_SIZE_24;
	priv->ctl_rec = KIRKWOOD_RECCTL_SIZE_24;

	/* Select the burst size */
	if (priv->burst == 32) {
		priv->ctl_play |= KIRKWOOD_PLAYCTL_BURST_32;
		priv->ctl_rec |= KIRKWOOD_RECCTL_BURST_32;
	} else {
		priv->ctl_play |= KIRKWOOD_PLAYCTL_BURST_128;
		priv->ctl_rec |= KIRKWOOD_RECCTL_BURST_128;
	}

	err = snd_soc_register_component(&pdev->dev, &kirkwood_i2s_component,
					 soc_dai, 2);
	if (err) {
		dev_err(&pdev->dev, "snd_soc_register_component failed\n");
		goto err_component;
	}

	err = snd_soc_register_platform(&pdev->dev, &kirkwood_soc_platform);
	if (err) {
		dev_err(&pdev->dev, "snd_soc_register_platform failed\n");
		goto err_platform;
	}

	kirkwood_i2s_init(priv);

	return 0;
 err_platform:
	snd_soc_unregister_component(&pdev->dev);
 err_component:
	if (!IS_ERR(priv->extclk))
		clk_disable_unprepare(priv->extclk);
	clk_disable_unprepare(priv->clk);

	return err;
}

static int kirkwood_i2s_dev_remove(struct platform_device *pdev)
{
	struct kirkwood_dma_data *priv = dev_get_drvdata(&pdev->dev);

	snd_soc_unregister_platform(&pdev->dev);
	snd_soc_unregister_component(&pdev->dev);

	if (!IS_ERR(priv->extclk))
		clk_disable_unprepare(priv->extclk);
	clk_disable_unprepare(priv->clk);
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
static struct platform_driver kirkwood_i2s_driver = {
	.probe  = kirkwood_i2s_dev_probe,
<<<<<<< HEAD
	.remove = kirkwood_i2s_dev_remove,
=======
	.remove = __devexit_p(kirkwood_i2s_dev_remove),
>>>>>>> refs/remotes/origin/cm-10.0
	.driver = {
		.name = DRV_NAME,
		.owner = THIS_MODULE,
	},
};

<<<<<<< HEAD
static int __init kirkwood_i2s_init(void)
{
	return platform_driver_register(&kirkwood_i2s_driver);
}
module_init(kirkwood_i2s_init);

static void __exit kirkwood_i2s_exit(void)
{
	platform_driver_unregister(&kirkwood_i2s_driver);
}
module_exit(kirkwood_i2s_exit);
=======
module_platform_driver(kirkwood_i2s_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
#ifdef CONFIG_OF
static struct of_device_id mvebu_audio_of_match[] = {
	{ .compatible = "marvell,kirkwood-audio" },
	{ .compatible = "marvell,dove-audio" },
	{ }
};
MODULE_DEVICE_TABLE(of, mvebu_audio_of_match);
#endif

static struct platform_driver kirkwood_i2s_driver = {
	.probe  = kirkwood_i2s_dev_probe,
	.remove = kirkwood_i2s_dev_remove,
	.driver = {
		.name = DRV_NAME,
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(mvebu_audio_of_match),
	},
};

module_platform_driver(kirkwood_i2s_driver);
>>>>>>> refs/remotes/origin/master

/* Module information */
MODULE_AUTHOR("Arnaud Patard, <arnaud.patard@rtp-net.org>");
MODULE_DESCRIPTION("Kirkwood I2S SoC Interface");
MODULE_LICENSE("GPL");
<<<<<<< HEAD
MODULE_ALIAS("platform:kirkwood-i2s");
=======
MODULE_ALIAS("platform:mvebu-audio");
>>>>>>> refs/remotes/origin/master
