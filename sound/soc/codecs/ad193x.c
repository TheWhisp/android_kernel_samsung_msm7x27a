/*
 * AD193X Audio Codec driver supporting AD1936/7/8/9
 *
 * Copyright 2010 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/i2c.h>
#include <linux/spi/spi.h>
#include <linux/slab.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/initval.h>
#include <sound/soc.h>
#include <sound/tlv.h>
#include "ad193x.h"

/* codec private data */
struct ad193x_priv {
<<<<<<< HEAD
<<<<<<< HEAD
	enum snd_soc_control_type control_type;
	int sysclk;
};

/* ad193x register cache & default register settings */
static const u8 ad193x_reg[AD193X_NUM_REGS] = {
	0, 0, 0, 0, 0, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0, 0, 0,
};

/*
 * AD193X volume/mute/de-emphasis etc. controls
 */
static const char *ad193x_deemp[] = {"None", "48kHz", "44.1kHz", "32kHz"};
=======
=======
>>>>>>> refs/remotes/origin/master
	struct regmap *regmap;
	int sysclk;
};

/*
 * AD193X volume/mute/de-emphasis etc. controls
 */
static const char * const ad193x_deemp[] = {"None", "48kHz", "44.1kHz", "32kHz"};
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

static const struct soc_enum ad193x_deemp_enum =
	SOC_ENUM_SINGLE(AD193X_DAC_CTRL2, 1, 4, ad193x_deemp);

<<<<<<< HEAD
<<<<<<< HEAD
static const struct snd_kcontrol_new ad193x_snd_controls[] = {
	/* DAC volume control */
	SOC_DOUBLE_R("DAC1 Volume", AD193X_DAC_L1_VOL,
			AD193X_DAC_R1_VOL, 0, 0xFF, 1),
	SOC_DOUBLE_R("DAC2 Volume", AD193X_DAC_L2_VOL,
			AD193X_DAC_R2_VOL, 0, 0xFF, 1),
	SOC_DOUBLE_R("DAC3 Volume", AD193X_DAC_L3_VOL,
			AD193X_DAC_R3_VOL, 0, 0xFF, 1),
	SOC_DOUBLE_R("DAC4 Volume", AD193X_DAC_L4_VOL,
			AD193X_DAC_R4_VOL, 0, 0xFF, 1),
=======
=======
>>>>>>> refs/remotes/origin/master
static const DECLARE_TLV_DB_MINMAX(adau193x_tlv, -9563, 0);

static const struct snd_kcontrol_new ad193x_snd_controls[] = {
	/* DAC volume control */
	SOC_DOUBLE_R_TLV("DAC1 Volume", AD193X_DAC_L1_VOL,
			AD193X_DAC_R1_VOL, 0, 0xFF, 1, adau193x_tlv),
	SOC_DOUBLE_R_TLV("DAC2 Volume", AD193X_DAC_L2_VOL,
			AD193X_DAC_R2_VOL, 0, 0xFF, 1, adau193x_tlv),
	SOC_DOUBLE_R_TLV("DAC3 Volume", AD193X_DAC_L3_VOL,
			AD193X_DAC_R3_VOL, 0, 0xFF, 1, adau193x_tlv),
	SOC_DOUBLE_R_TLV("DAC4 Volume", AD193X_DAC_L4_VOL,
			AD193X_DAC_R4_VOL, 0, 0xFF, 1, adau193x_tlv),
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	/* ADC switch control */
	SOC_DOUBLE("ADC1 Switch", AD193X_ADC_CTRL0, AD193X_ADCL1_MUTE,
		AD193X_ADCR1_MUTE, 1, 1),
	SOC_DOUBLE("ADC2 Switch", AD193X_ADC_CTRL0, AD193X_ADCL2_MUTE,
		AD193X_ADCR2_MUTE, 1, 1),

	/* DAC switch control */
	SOC_DOUBLE("DAC1 Switch", AD193X_DAC_CHNL_MUTE, AD193X_DACL1_MUTE,
		AD193X_DACR1_MUTE, 1, 1),
	SOC_DOUBLE("DAC2 Switch", AD193X_DAC_CHNL_MUTE, AD193X_DACL2_MUTE,
		AD193X_DACR2_MUTE, 1, 1),
	SOC_DOUBLE("DAC3 Switch", AD193X_DAC_CHNL_MUTE, AD193X_DACL3_MUTE,
		AD193X_DACR3_MUTE, 1, 1),
	SOC_DOUBLE("DAC4 Switch", AD193X_DAC_CHNL_MUTE, AD193X_DACL4_MUTE,
		AD193X_DACR4_MUTE, 1, 1),

	/* ADC high-pass filter */
	SOC_SINGLE("ADC High Pass Filter Switch", AD193X_ADC_CTRL0,
			AD193X_ADC_HIGHPASS_FILTER, 1, 0),

	/* DAC de-emphasis */
	SOC_ENUM("Playback Deemphasis", ad193x_deemp_enum),
};

static const struct snd_soc_dapm_widget ad193x_dapm_widgets[] = {
	SND_SOC_DAPM_DAC("DAC", "Playback", AD193X_DAC_CTRL0, 0, 1),
	SND_SOC_DAPM_ADC("ADC", "Capture", SND_SOC_NOPM, 0, 0),
	SND_SOC_DAPM_SUPPLY("PLL_PWR", AD193X_PLL_CLK_CTRL0, 0, 1, NULL, 0),
	SND_SOC_DAPM_SUPPLY("ADC_PWR", AD193X_ADC_CTRL0, 0, 1, NULL, 0),
<<<<<<< HEAD
<<<<<<< HEAD
=======
	SND_SOC_DAPM_SUPPLY("SYSCLK", AD193X_PLL_CLK_CTRL0, 7, 0, NULL, 0),
>>>>>>> refs/remotes/origin/cm-10.0
=======
	SND_SOC_DAPM_SUPPLY("SYSCLK", AD193X_PLL_CLK_CTRL0, 7, 0, NULL, 0),
>>>>>>> refs/remotes/origin/master
	SND_SOC_DAPM_OUTPUT("DAC1OUT"),
	SND_SOC_DAPM_OUTPUT("DAC2OUT"),
	SND_SOC_DAPM_OUTPUT("DAC3OUT"),
	SND_SOC_DAPM_OUTPUT("DAC4OUT"),
	SND_SOC_DAPM_INPUT("ADC1IN"),
	SND_SOC_DAPM_INPUT("ADC2IN"),
};

static const struct snd_soc_dapm_route audio_paths[] = {
<<<<<<< HEAD
<<<<<<< HEAD
	{ "DAC", NULL, "PLL_PWR" },
	{ "ADC", NULL, "PLL_PWR" },
	{ "DAC", NULL, "ADC_PWR" },
	{ "ADC", NULL, "ADC_PWR" },
	{ "DAC1OUT", "DAC1 Switch", "DAC" },
	{ "DAC2OUT", "DAC2 Switch", "DAC" },
	{ "DAC3OUT", "DAC3 Switch", "DAC" },
	{ "DAC4OUT", "DAC4 Switch", "DAC" },
	{ "ADC", "ADC1 Switch", "ADC1IN" },
	{ "ADC", "ADC2 Switch", "ADC2IN" },
=======
=======
>>>>>>> refs/remotes/origin/master
	{ "DAC", NULL, "SYSCLK" },
	{ "ADC", NULL, "SYSCLK" },
	{ "DAC", NULL, "ADC_PWR" },
	{ "ADC", NULL, "ADC_PWR" },
	{ "DAC1OUT", NULL, "DAC" },
	{ "DAC2OUT", NULL, "DAC" },
	{ "DAC3OUT", NULL, "DAC" },
	{ "DAC4OUT", NULL, "DAC" },
	{ "ADC", NULL, "ADC1IN" },
	{ "ADC", NULL, "ADC2IN" },
	{ "SYSCLK", NULL, "PLL_PWR" },
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};

/*
 * DAI ops entries
 */

static int ad193x_mute(struct snd_soc_dai *dai, int mute)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct snd_soc_codec *codec = dai->codec;
	int reg;

	reg = snd_soc_read(codec, AD193X_DAC_CTRL2);
	reg = (mute > 0) ? reg | AD193X_DAC_MASTER_MUTE : reg &
		(~AD193X_DAC_MASTER_MUTE);
	snd_soc_write(codec, AD193X_DAC_CTRL2, reg);
=======
=======
>>>>>>> refs/remotes/origin/master
	struct ad193x_priv *ad193x = snd_soc_codec_get_drvdata(dai->codec);

	if (mute)
		regmap_update_bits(ad193x->regmap, AD193X_DAC_CTRL2,
				    AD193X_DAC_MASTER_MUTE,
				    AD193X_DAC_MASTER_MUTE);
	else
		regmap_update_bits(ad193x->regmap, AD193X_DAC_CTRL2,
				    AD193X_DAC_MASTER_MUTE, 0);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return 0;
}

static int ad193x_set_tdm_slot(struct snd_soc_dai *dai, unsigned int tx_mask,
			       unsigned int rx_mask, int slots, int width)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct snd_soc_codec *codec = dai->codec;
	int dac_reg = snd_soc_read(codec, AD193X_DAC_CTRL1);
	int adc_reg = snd_soc_read(codec, AD193X_ADC_CTRL2);

	dac_reg &= ~AD193X_DAC_CHAN_MASK;
	adc_reg &= ~AD193X_ADC_CHAN_MASK;

	switch (slots) {
	case 2:
		dac_reg |= AD193X_DAC_2_CHANNELS << AD193X_DAC_CHAN_SHFT;
		adc_reg |= AD193X_ADC_2_CHANNELS << AD193X_ADC_CHAN_SHFT;
		break;
	case 4:
		dac_reg |= AD193X_DAC_4_CHANNELS << AD193X_DAC_CHAN_SHFT;
		adc_reg |= AD193X_ADC_4_CHANNELS << AD193X_ADC_CHAN_SHFT;
		break;
	case 8:
		dac_reg |= AD193X_DAC_8_CHANNELS << AD193X_DAC_CHAN_SHFT;
		adc_reg |= AD193X_ADC_8_CHANNELS << AD193X_ADC_CHAN_SHFT;
		break;
	case 16:
		dac_reg |= AD193X_DAC_16_CHANNELS << AD193X_DAC_CHAN_SHFT;
		adc_reg |= AD193X_ADC_16_CHANNELS << AD193X_ADC_CHAN_SHFT;
=======
=======
>>>>>>> refs/remotes/origin/master
	struct ad193x_priv *ad193x = snd_soc_codec_get_drvdata(dai->codec);
	unsigned int channels;

	switch (slots) {
	case 2:
		channels = AD193X_2_CHANNELS;
		break;
	case 4:
		channels = AD193X_4_CHANNELS;
		break;
	case 8:
		channels = AD193X_8_CHANNELS;
		break;
	case 16:
		channels = AD193X_16_CHANNELS;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		break;
	default:
		return -EINVAL;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	snd_soc_write(codec, AD193X_DAC_CTRL1, dac_reg);
	snd_soc_write(codec, AD193X_ADC_CTRL2, adc_reg);
=======
=======
>>>>>>> refs/remotes/origin/master
	regmap_update_bits(ad193x->regmap, AD193X_DAC_CTRL1,
		AD193X_DAC_CHAN_MASK, channels << AD193X_DAC_CHAN_SHFT);
	regmap_update_bits(ad193x->regmap, AD193X_ADC_CTRL2,
		AD193X_ADC_CHAN_MASK, channels << AD193X_ADC_CHAN_SHFT);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return 0;
}

static int ad193x_set_dai_fmt(struct snd_soc_dai *codec_dai,
		unsigned int fmt)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct snd_soc_codec *codec = codec_dai->codec;
	int adc_reg1, adc_reg2, dac_reg;

	adc_reg1 = snd_soc_read(codec, AD193X_ADC_CTRL1);
	adc_reg2 = snd_soc_read(codec, AD193X_ADC_CTRL2);
	dac_reg = snd_soc_read(codec, AD193X_DAC_CTRL1);
=======
=======
>>>>>>> refs/remotes/origin/master
	struct ad193x_priv *ad193x = snd_soc_codec_get_drvdata(codec_dai->codec);
	unsigned int adc_serfmt = 0;
	unsigned int adc_fmt = 0;
	unsigned int dac_fmt = 0;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	/* At present, the driver only support AUX ADC mode(SND_SOC_DAIFMT_I2S
	 * with TDM) and ADC&DAC TDM mode(SND_SOC_DAIFMT_DSP_A)
	 */
	switch (fmt & SND_SOC_DAIFMT_FORMAT_MASK) {
	case SND_SOC_DAIFMT_I2S:
<<<<<<< HEAD
<<<<<<< HEAD
		adc_reg1 &= ~AD193X_ADC_SERFMT_MASK;
		adc_reg1 |= AD193X_ADC_SERFMT_TDM;
		break;
	case SND_SOC_DAIFMT_DSP_A:
		adc_reg1 &= ~AD193X_ADC_SERFMT_MASK;
		adc_reg1 |= AD193X_ADC_SERFMT_AUX;
=======
=======
>>>>>>> refs/remotes/origin/master
		adc_serfmt |= AD193X_ADC_SERFMT_TDM;
		break;
	case SND_SOC_DAIFMT_DSP_A:
		adc_serfmt |= AD193X_ADC_SERFMT_AUX;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		break;
	default:
		return -EINVAL;
	}

	switch (fmt & SND_SOC_DAIFMT_INV_MASK) {
	case SND_SOC_DAIFMT_NB_NF: /* normal bit clock + frame */
<<<<<<< HEAD
<<<<<<< HEAD
		adc_reg2 &= ~AD193X_ADC_LEFT_HIGH;
		adc_reg2 &= ~AD193X_ADC_BCLK_INV;
		dac_reg &= ~AD193X_DAC_LEFT_HIGH;
		dac_reg &= ~AD193X_DAC_BCLK_INV;
		break;
	case SND_SOC_DAIFMT_NB_IF: /* normal bclk + invert frm */
		adc_reg2 |= AD193X_ADC_LEFT_HIGH;
		adc_reg2 &= ~AD193X_ADC_BCLK_INV;
		dac_reg |= AD193X_DAC_LEFT_HIGH;
		dac_reg &= ~AD193X_DAC_BCLK_INV;
		break;
	case SND_SOC_DAIFMT_IB_NF: /* invert bclk + normal frm */
		adc_reg2 &= ~AD193X_ADC_LEFT_HIGH;
		adc_reg2 |= AD193X_ADC_BCLK_INV;
		dac_reg &= ~AD193X_DAC_LEFT_HIGH;
		dac_reg |= AD193X_DAC_BCLK_INV;
		break;

	case SND_SOC_DAIFMT_IB_IF: /* invert bclk + frm */
		adc_reg2 |= AD193X_ADC_LEFT_HIGH;
		adc_reg2 |= AD193X_ADC_BCLK_INV;
		dac_reg |= AD193X_DAC_LEFT_HIGH;
		dac_reg |= AD193X_DAC_BCLK_INV;
=======
=======
>>>>>>> refs/remotes/origin/master
		break;
	case SND_SOC_DAIFMT_NB_IF: /* normal bclk + invert frm */
		adc_fmt |= AD193X_ADC_LEFT_HIGH;
		dac_fmt |= AD193X_DAC_LEFT_HIGH;
		break;
	case SND_SOC_DAIFMT_IB_NF: /* invert bclk + normal frm */
		adc_fmt |= AD193X_ADC_BCLK_INV;
		dac_fmt |= AD193X_DAC_BCLK_INV;
		break;
	case SND_SOC_DAIFMT_IB_IF: /* invert bclk + frm */
		adc_fmt |= AD193X_ADC_LEFT_HIGH;
		adc_fmt |= AD193X_ADC_BCLK_INV;
		dac_fmt |= AD193X_DAC_LEFT_HIGH;
		dac_fmt |= AD193X_DAC_BCLK_INV;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		break;
	default:
		return -EINVAL;
	}

	switch (fmt & SND_SOC_DAIFMT_MASTER_MASK) {
	case SND_SOC_DAIFMT_CBM_CFM: /* codec clk & frm master */
<<<<<<< HEAD
<<<<<<< HEAD
		adc_reg2 |= AD193X_ADC_LCR_MASTER;
		adc_reg2 |= AD193X_ADC_BCLK_MASTER;
		dac_reg |= AD193X_DAC_LCR_MASTER;
		dac_reg |= AD193X_DAC_BCLK_MASTER;
		break;
	case SND_SOC_DAIFMT_CBS_CFM: /* codec clk slave & frm master */
		adc_reg2 |= AD193X_ADC_LCR_MASTER;
		adc_reg2 &= ~AD193X_ADC_BCLK_MASTER;
		dac_reg |= AD193X_DAC_LCR_MASTER;
		dac_reg &= ~AD193X_DAC_BCLK_MASTER;
		break;
	case SND_SOC_DAIFMT_CBM_CFS: /* codec clk master & frame slave */
		adc_reg2 &= ~AD193X_ADC_LCR_MASTER;
		adc_reg2 |= AD193X_ADC_BCLK_MASTER;
		dac_reg &= ~AD193X_DAC_LCR_MASTER;
		dac_reg |= AD193X_DAC_BCLK_MASTER;
		break;
	case SND_SOC_DAIFMT_CBS_CFS: /* codec clk & frm slave */
		adc_reg2 &= ~AD193X_ADC_LCR_MASTER;
		adc_reg2 &= ~AD193X_ADC_BCLK_MASTER;
		dac_reg &= ~AD193X_DAC_LCR_MASTER;
		dac_reg &= ~AD193X_DAC_BCLK_MASTER;
=======
=======
>>>>>>> refs/remotes/origin/master
		adc_fmt |= AD193X_ADC_LCR_MASTER;
		adc_fmt |= AD193X_ADC_BCLK_MASTER;
		dac_fmt |= AD193X_DAC_LCR_MASTER;
		dac_fmt |= AD193X_DAC_BCLK_MASTER;
		break;
	case SND_SOC_DAIFMT_CBS_CFM: /* codec clk slave & frm master */
		adc_fmt |= AD193X_ADC_LCR_MASTER;
		dac_fmt |= AD193X_DAC_LCR_MASTER;
		break;
	case SND_SOC_DAIFMT_CBM_CFS: /* codec clk master & frame slave */
		adc_fmt |= AD193X_ADC_BCLK_MASTER;
		dac_fmt |= AD193X_DAC_BCLK_MASTER;
		break;
	case SND_SOC_DAIFMT_CBS_CFS: /* codec clk & frm slave */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		break;
	default:
		return -EINVAL;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	snd_soc_write(codec, AD193X_ADC_CTRL1, adc_reg1);
	snd_soc_write(codec, AD193X_ADC_CTRL2, adc_reg2);
	snd_soc_write(codec, AD193X_DAC_CTRL1, dac_reg);
=======
=======
>>>>>>> refs/remotes/origin/master
	regmap_update_bits(ad193x->regmap, AD193X_ADC_CTRL1,
		AD193X_ADC_SERFMT_MASK, adc_serfmt);
	regmap_update_bits(ad193x->regmap, AD193X_ADC_CTRL2,
		AD193X_ADC_FMT_MASK, adc_fmt);
	regmap_update_bits(ad193x->regmap, AD193X_DAC_CTRL1,
		AD193X_DAC_FMT_MASK, dac_fmt);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return 0;
}

static int ad193x_set_dai_sysclk(struct snd_soc_dai *codec_dai,
		int clk_id, unsigned int freq, int dir)
{
	struct snd_soc_codec *codec = codec_dai->codec;
	struct ad193x_priv *ad193x = snd_soc_codec_get_drvdata(codec);
	switch (freq) {
	case 12288000:
	case 18432000:
	case 24576000:
	case 36864000:
		ad193x->sysclk = freq;
		return 0;
	}
	return -EINVAL;
}

static int ad193x_hw_params(struct snd_pcm_substream *substream,
		struct snd_pcm_hw_params *params,
		struct snd_soc_dai *dai)
{
<<<<<<< HEAD
<<<<<<< HEAD
	int word_len = 0, reg = 0, master_rate = 0;
=======
	int word_len = 0, master_rate = 0;
>>>>>>> refs/remotes/origin/cm-10.0

	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_codec *codec = rtd->codec;
	struct ad193x_priv *ad193x = snd_soc_codec_get_drvdata(codec);

	/* bit size */
	switch (params_format(params)) {
	case SNDRV_PCM_FORMAT_S16_LE:
		word_len = 3;
		break;
	case SNDRV_PCM_FORMAT_S20_3LE:
		word_len = 1;
		break;
	case SNDRV_PCM_FORMAT_S24_LE:
	case SNDRV_PCM_FORMAT_S32_LE:
=======
	int word_len = 0, master_rate = 0;
	struct snd_soc_codec *codec = dai->codec;
	struct ad193x_priv *ad193x = snd_soc_codec_get_drvdata(codec);

	/* bit size */
	switch (params_width(params)) {
	case 16:
		word_len = 3;
		break;
	case 20:
		word_len = 1;
		break;
	case 24:
	case 32:
>>>>>>> refs/remotes/origin/master
		word_len = 0;
		break;
	}

	switch (ad193x->sysclk) {
	case 12288000:
		master_rate = AD193X_PLL_INPUT_256;
		break;
	case 18432000:
		master_rate = AD193X_PLL_INPUT_384;
		break;
	case 24576000:
		master_rate = AD193X_PLL_INPUT_512;
		break;
	case 36864000:
		master_rate = AD193X_PLL_INPUT_768;
		break;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	reg = snd_soc_read(codec, AD193X_PLL_CLK_CTRL0);
	reg = (reg & AD193X_PLL_INPUT_MASK) | master_rate;
	snd_soc_write(codec, AD193X_PLL_CLK_CTRL0, reg);

	reg = snd_soc_read(codec, AD193X_DAC_CTRL2);
	reg = (reg & (~AD193X_DAC_WORD_LEN_MASK))
		| (word_len << AD193X_DAC_WORD_LEN_SHFT);
	snd_soc_write(codec, AD193X_DAC_CTRL2, reg);

	reg = snd_soc_read(codec, AD193X_ADC_CTRL1);
	reg = (reg & (~AD193X_ADC_WORD_LEN_MASK)) | word_len;
	snd_soc_write(codec, AD193X_ADC_CTRL1, reg);
=======
=======
>>>>>>> refs/remotes/origin/master
	regmap_update_bits(ad193x->regmap, AD193X_PLL_CLK_CTRL0,
			    AD193X_PLL_INPUT_MASK, master_rate);

	regmap_update_bits(ad193x->regmap, AD193X_DAC_CTRL2,
			    AD193X_DAC_WORD_LEN_MASK,
			    word_len << AD193X_DAC_WORD_LEN_SHFT);

	regmap_update_bits(ad193x->regmap, AD193X_ADC_CTRL1,
			    AD193X_ADC_WORD_LEN_MASK, word_len);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct snd_soc_dai_ops ad193x_dai_ops = {
=======
static const struct snd_soc_dai_ops ad193x_dai_ops = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static const struct snd_soc_dai_ops ad193x_dai_ops = {
>>>>>>> refs/remotes/origin/master
	.hw_params = ad193x_hw_params,
	.digital_mute = ad193x_mute,
	.set_tdm_slot = ad193x_set_tdm_slot,
	.set_sysclk	= ad193x_set_dai_sysclk,
	.set_fmt = ad193x_set_dai_fmt,
};

/* codec DAI instance */
static struct snd_soc_dai_driver ad193x_dai = {
	.name = "ad193x-hifi",
	.playback = {
		.stream_name = "Playback",
		.channels_min = 2,
		.channels_max = 8,
		.rates = SNDRV_PCM_RATE_48000,
		.formats = SNDRV_PCM_FMTBIT_S32_LE | SNDRV_PCM_FMTBIT_S16_LE |
			SNDRV_PCM_FMTBIT_S20_3LE | SNDRV_PCM_FMTBIT_S24_LE,
	},
	.capture = {
		.stream_name = "Capture",
		.channels_min = 2,
		.channels_max = 4,
		.rates = SNDRV_PCM_RATE_48000,
		.formats = SNDRV_PCM_FMTBIT_S32_LE | SNDRV_PCM_FMTBIT_S16_LE |
			SNDRV_PCM_FMTBIT_S20_3LE | SNDRV_PCM_FMTBIT_S24_LE,
	},
	.ops = &ad193x_dai_ops,
};

static int ad193x_probe(struct snd_soc_codec *codec)
{
	struct ad193x_priv *ad193x = snd_soc_codec_get_drvdata(codec);
<<<<<<< HEAD
<<<<<<< HEAD
	struct snd_soc_dapm_context *dapm = &codec->dapm;
	int ret;

	if (ad193x->control_type == SND_SOC_I2C)
		ret = snd_soc_codec_set_cache_io(codec, 8, 8, ad193x->control_type);
	else
		ret = snd_soc_codec_set_cache_io(codec, 16, 8, ad193x->control_type);
=======
=======
>>>>>>> refs/remotes/origin/master
	int ret;

	codec->control_data = ad193x->regmap;
	ret = snd_soc_codec_set_cache_io(codec, 0, 0, SND_SOC_REGMAP);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (ret < 0) {
		dev_err(codec->dev, "failed to set cache I/O: %d\n", ret);
		return ret;
	}

	/* default setting for ad193x */

	/* unmute dac channels */
<<<<<<< HEAD
<<<<<<< HEAD
	snd_soc_write(codec, AD193X_DAC_CHNL_MUTE, 0x0);
	/* de-emphasis: 48kHz, powedown dac */
	snd_soc_write(codec, AD193X_DAC_CTRL2, 0x1A);
	/* powerdown dac, dac in tdm mode */
	snd_soc_write(codec, AD193X_DAC_CTRL0, 0x41);
	/* high-pass filter enable */
	snd_soc_write(codec, AD193X_ADC_CTRL0, 0x3);
	/* sata delay=1, adc aux mode */
	snd_soc_write(codec, AD193X_ADC_CTRL1, 0x43);
	/* pll input: mclki/xi */
	snd_soc_write(codec, AD193X_PLL_CLK_CTRL0, 0x99); /* mclk=24.576Mhz: 0x9D; mclk=12.288Mhz: 0x99 */
	snd_soc_write(codec, AD193X_PLL_CLK_CTRL1, 0x04);

	snd_soc_add_controls(codec, ad193x_snd_controls,
			     ARRAY_SIZE(ad193x_snd_controls));
	snd_soc_dapm_new_controls(dapm, ad193x_dapm_widgets,
				  ARRAY_SIZE(ad193x_dapm_widgets));
	snd_soc_dapm_add_routes(dapm, audio_paths, ARRAY_SIZE(audio_paths));
=======
=======
>>>>>>> refs/remotes/origin/master
	regmap_write(ad193x->regmap, AD193X_DAC_CHNL_MUTE, 0x0);
	/* de-emphasis: 48kHz, powedown dac */
	regmap_write(ad193x->regmap, AD193X_DAC_CTRL2, 0x1A);
	/* powerdown dac, dac in tdm mode */
	regmap_write(ad193x->regmap, AD193X_DAC_CTRL0, 0x41);
	/* high-pass filter enable */
	regmap_write(ad193x->regmap, AD193X_ADC_CTRL0, 0x3);
	/* sata delay=1, adc aux mode */
	regmap_write(ad193x->regmap, AD193X_ADC_CTRL1, 0x43);
	/* pll input: mclki/xi */
	regmap_write(ad193x->regmap, AD193X_PLL_CLK_CTRL0, 0x99); /* mclk=24.576Mhz: 0x9D; mclk=12.288Mhz: 0x99 */
	regmap_write(ad193x->regmap, AD193X_PLL_CLK_CTRL1, 0x04);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return ret;
}

static struct snd_soc_codec_driver soc_codec_dev_ad193x = {
	.probe = 	ad193x_probe,
<<<<<<< HEAD
<<<<<<< HEAD
	.reg_cache_default = ad193x_reg,
	.reg_cache_size = AD193X_NUM_REGS,
	.reg_word_size = sizeof(u16),
};

#if defined(CONFIG_SPI_MASTER)
=======
=======
>>>>>>> refs/remotes/origin/master
	.controls = ad193x_snd_controls,
	.num_controls = ARRAY_SIZE(ad193x_snd_controls),
	.dapm_widgets = ad193x_dapm_widgets,
	.num_dapm_widgets = ARRAY_SIZE(ad193x_dapm_widgets),
	.dapm_routes = audio_paths,
	.num_dapm_routes = ARRAY_SIZE(audio_paths),
};

static bool adau193x_reg_volatile(struct device *dev, unsigned int reg)
{
	return false;
}

#if defined(CONFIG_SPI_MASTER)

static const struct regmap_config ad193x_spi_regmap_config = {
	.val_bits = 8,
	.reg_bits = 16,
	.read_flag_mask = 0x09,
	.write_flag_mask = 0x08,

	.max_register = AD193X_NUM_REGS - 1,
	.volatile_reg = adau193x_reg_volatile,
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
static int __devinit ad193x_spi_probe(struct spi_device *spi)
{
	struct ad193x_priv *ad193x;
	int ret;

<<<<<<< HEAD
	ad193x = kzalloc(sizeof(struct ad193x_priv), GFP_KERNEL);
	if (ad193x == NULL)
		return -ENOMEM;

	spi_set_drvdata(spi, ad193x);
	ad193x->control_type = SND_SOC_SPI;
=======
=======
static int ad193x_spi_probe(struct spi_device *spi)
{
	struct ad193x_priv *ad193x;

>>>>>>> refs/remotes/origin/master
	ad193x = devm_kzalloc(&spi->dev, sizeof(struct ad193x_priv),
			      GFP_KERNEL);
	if (ad193x == NULL)
		return -ENOMEM;

<<<<<<< HEAD
	ad193x->regmap = regmap_init_spi(spi, &ad193x_spi_regmap_config);
	if (IS_ERR(ad193x->regmap)) {
		ret = PTR_ERR(ad193x->regmap);
		goto err_out;
	}

	spi_set_drvdata(spi, ad193x);
>>>>>>> refs/remotes/origin/cm-10.0

	ret = snd_soc_register_codec(&spi->dev,
			&soc_codec_dev_ad193x, &ad193x_dai, 1);
	if (ret < 0)
<<<<<<< HEAD
		kfree(ad193x);
=======
		goto err_regmap_exit;

	return 0;

err_regmap_exit:
	regmap_exit(ad193x->regmap);
err_out:
>>>>>>> refs/remotes/origin/cm-10.0
	return ret;
}

static int __devexit ad193x_spi_remove(struct spi_device *spi)
{
<<<<<<< HEAD
	snd_soc_unregister_codec(&spi->dev);
	kfree(spi_get_drvdata(spi));
=======
	struct ad193x_priv *ad193x = spi_get_drvdata(spi);

	snd_soc_unregister_codec(&spi->dev);
	regmap_exit(ad193x->regmap);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ad193x->regmap = devm_regmap_init_spi(spi, &ad193x_spi_regmap_config);
	if (IS_ERR(ad193x->regmap))
		return PTR_ERR(ad193x->regmap);

	spi_set_drvdata(spi, ad193x);

	return snd_soc_register_codec(&spi->dev, &soc_codec_dev_ad193x,
			&ad193x_dai, 1);
}

static int ad193x_spi_remove(struct spi_device *spi)
{
	snd_soc_unregister_codec(&spi->dev);
>>>>>>> refs/remotes/origin/master
	return 0;
}

static struct spi_driver ad193x_spi_driver = {
	.driver = {
		.name	= "ad193x",
		.owner	= THIS_MODULE,
	},
	.probe		= ad193x_spi_probe,
<<<<<<< HEAD
	.remove		= __devexit_p(ad193x_spi_remove),
};
#endif

#if defined(CONFIG_I2C) || defined(CONFIG_I2C_MODULE)
<<<<<<< HEAD
=======
=======
	.remove		= ad193x_spi_remove,
};
#endif

#if IS_ENABLED(CONFIG_I2C)
>>>>>>> refs/remotes/origin/master

static const struct regmap_config ad193x_i2c_regmap_config = {
	.val_bits = 8,
	.reg_bits = 8,

	.max_register = AD193X_NUM_REGS - 1,
	.volatile_reg = adau193x_reg_volatile,
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static const struct i2c_device_id ad193x_id[] = {
	{ "ad1936", 0 },
	{ "ad1937", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, ad193x_id);

<<<<<<< HEAD
static int __devinit ad193x_i2c_probe(struct i2c_client *client,
		const struct i2c_device_id *id)
{
	struct ad193x_priv *ad193x;
	int ret;

<<<<<<< HEAD
	ad193x = kzalloc(sizeof(struct ad193x_priv), GFP_KERNEL);
	if (ad193x == NULL)
		return -ENOMEM;

	i2c_set_clientdata(client, ad193x);
	ad193x->control_type = SND_SOC_I2C;
=======
=======
static int ad193x_i2c_probe(struct i2c_client *client,
			    const struct i2c_device_id *id)
{
	struct ad193x_priv *ad193x;

>>>>>>> refs/remotes/origin/master
	ad193x = devm_kzalloc(&client->dev, sizeof(struct ad193x_priv),
			      GFP_KERNEL);
	if (ad193x == NULL)
		return -ENOMEM;

<<<<<<< HEAD
	ad193x->regmap = regmap_init_i2c(client, &ad193x_i2c_regmap_config);
	if (IS_ERR(ad193x->regmap)) {
		ret = PTR_ERR(ad193x->regmap);
		goto err_out;
	}

	i2c_set_clientdata(client, ad193x);
>>>>>>> refs/remotes/origin/cm-10.0

	ret =  snd_soc_register_codec(&client->dev,
			&soc_codec_dev_ad193x, &ad193x_dai, 1);
	if (ret < 0)
<<<<<<< HEAD
		kfree(ad193x);
=======
		goto err_regmap_exit;

	return 0;

err_regmap_exit:
	regmap_exit(ad193x->regmap);
err_out:
>>>>>>> refs/remotes/origin/cm-10.0
	return ret;
}

static int __devexit ad193x_i2c_remove(struct i2c_client *client)
{
<<<<<<< HEAD
	snd_soc_unregister_codec(&client->dev);
	kfree(i2c_get_clientdata(client));
=======
	struct ad193x_priv *ad193x = i2c_get_clientdata(client);

	snd_soc_unregister_codec(&client->dev);
	regmap_exit(ad193x->regmap);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ad193x->regmap = devm_regmap_init_i2c(client, &ad193x_i2c_regmap_config);
	if (IS_ERR(ad193x->regmap))
		return PTR_ERR(ad193x->regmap);

	i2c_set_clientdata(client, ad193x);

	return snd_soc_register_codec(&client->dev, &soc_codec_dev_ad193x,
			&ad193x_dai, 1);
}

static int ad193x_i2c_remove(struct i2c_client *client)
{
	snd_soc_unregister_codec(&client->dev);
>>>>>>> refs/remotes/origin/master
	return 0;
}

static struct i2c_driver ad193x_i2c_driver = {
	.driver = {
		.name = "ad193x",
	},
	.probe    = ad193x_i2c_probe,
<<<<<<< HEAD
	.remove   = __devexit_p(ad193x_i2c_remove),
=======
	.remove   = ad193x_i2c_remove,
>>>>>>> refs/remotes/origin/master
	.id_table = ad193x_id,
};
#endif

static int __init ad193x_modinit(void)
{
	int ret;

<<<<<<< HEAD
#if defined(CONFIG_I2C) || defined(CONFIG_I2C_MODULE)
=======
#if IS_ENABLED(CONFIG_I2C)
>>>>>>> refs/remotes/origin/master
	ret =  i2c_add_driver(&ad193x_i2c_driver);
	if (ret != 0) {
		printk(KERN_ERR "Failed to register AD193X I2C driver: %d\n",
				ret);
	}
#endif

#if defined(CONFIG_SPI_MASTER)
	ret = spi_register_driver(&ad193x_spi_driver);
	if (ret != 0) {
		printk(KERN_ERR "Failed to register AD193X SPI driver: %d\n",
				ret);
	}
#endif
	return ret;
}
module_init(ad193x_modinit);

static void __exit ad193x_modexit(void)
{
#if defined(CONFIG_SPI_MASTER)
	spi_unregister_driver(&ad193x_spi_driver);
#endif

<<<<<<< HEAD
#if defined(CONFIG_I2C) || defined(CONFIG_I2C_MODULE)
=======
#if IS_ENABLED(CONFIG_I2C)
>>>>>>> refs/remotes/origin/master
	i2c_del_driver(&ad193x_i2c_driver);
#endif
}
module_exit(ad193x_modexit);

MODULE_DESCRIPTION("ASoC ad193x driver");
MODULE_AUTHOR("Barry Song <21cnbao@gmail.com>");
MODULE_LICENSE("GPL");
