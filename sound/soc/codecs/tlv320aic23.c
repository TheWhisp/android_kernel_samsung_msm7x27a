/*
 * ALSA SoC TLV320AIC23 codec driver
 *
 * Author:      Arun KS, <arunks@mistralsolutions.com>
 * Copyright:   (C) 2008 Mistral Solutions Pvt Ltd.,
 *
 * Based on sound/soc/codecs/wm8731.c by Richard Purdie
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Notes:
 *  The AIC23 is a driver for a low power stereo audio
 *  codec tlv320aic23
 *
 *  The machine layer should disable unsupported inputs/outputs by
 *  snd_soc_dapm_disable_pin(codec, "LHPOUT"), etc.
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/pm.h>
#include <linux/i2c.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/platform_device.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/regmap.h>
>>>>>>> refs/remotes/origin/master
#include <linux/slab.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/tlv.h>
#include <sound/initval.h>

#include "tlv320aic23.h"

<<<<<<< HEAD
#define AIC23_VERSION "0.1"

/*
 * AIC23 register cache
 */
static const u16 tlv320aic23_reg[] = {
	0x0097, 0x0097, 0x00F9, 0x00F9,	/* 0 */
	0x001A, 0x0004, 0x0007, 0x0001,	/* 4 */
	0x0020, 0x0000, 0x0000, 0x0000,	/* 8 */
	0x0000, 0x0000, 0x0000, 0x0000,	/* 12 */
};

<<<<<<< HEAD
/*
 * read tlv320aic23 register cache
 */
static inline unsigned int tlv320aic23_read_reg_cache(struct snd_soc_codec
						      *codec, unsigned int reg)
{
	u16 *cache = codec->reg_cache;
	if (reg >= ARRAY_SIZE(tlv320aic23_reg))
		return -1;
	return cache[reg];
}

/*
 * write tlv320aic23 register cache
 */
static inline void tlv320aic23_write_reg_cache(struct snd_soc_codec *codec,
					       u8 reg, u16 value)
{
	u16 *cache = codec->reg_cache;
	if (reg >= ARRAY_SIZE(tlv320aic23_reg))
		return;
	cache[reg] = value;
}

/*
 * write to the tlv320aic23 register space
 */
static int tlv320aic23_write(struct snd_soc_codec *codec, unsigned int reg,
			     unsigned int value)
{

	u8 data[2];

	/* TLV320AIC23 has 7 bit address and 9 bits of data
	 * so we need to switch one data bit into reg and rest
	 * of data into val
	 */

	if (reg > 9 && reg != 15) {
		printk(KERN_WARNING "%s Invalid register R%u\n", __func__, reg);
		return -1;
	}

	data[0] = (reg << 1) | (value >> 8 & 0x01);
	data[1] = value & 0xff;

	tlv320aic23_write_reg_cache(codec, reg, value);

	if (codec->hw_write(codec->control_data, data, 2) == 2)
		return 0;

	printk(KERN_ERR "%s cannot write %03x to register R%u\n", __func__,
	       value, reg);

	return -EIO;
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
/*
 * AIC23 register cache
 */
static const struct reg_default tlv320aic23_reg[] = {
	{  0, 0x0097 },
	{  1, 0x0097 },
	{  2, 0x00F9 },
	{  3, 0x00F9 },
	{  4, 0x001A },
	{  5, 0x0004 },
	{  6, 0x0007 },
	{  7, 0x0001 },
	{  8, 0x0020 },
	{  9, 0x0000 },
};

static const struct regmap_config tlv320aic23_regmap = {
	.reg_bits = 7,
	.val_bits = 9,

	.max_register = TLV320AIC23_RESET,
	.reg_defaults = tlv320aic23_reg,
	.num_reg_defaults = ARRAY_SIZE(tlv320aic23_reg),
	.cache_type = REGCACHE_RBTREE,
};

>>>>>>> refs/remotes/origin/master
static const char *rec_src_text[] = { "Line", "Mic" };
static const char *deemph_text[] = {"None", "32Khz", "44.1Khz", "48Khz"};

static const struct soc_enum rec_src_enum =
	SOC_ENUM_SINGLE(TLV320AIC23_ANLG, 2, 2, rec_src_text);

static const struct snd_kcontrol_new tlv320aic23_rec_src_mux_controls =
SOC_DAPM_ENUM("Input Select", rec_src_enum);

static const struct soc_enum tlv320aic23_rec_src =
	SOC_ENUM_SINGLE(TLV320AIC23_ANLG, 2, 2, rec_src_text);
static const struct soc_enum tlv320aic23_deemph =
	SOC_ENUM_SINGLE(TLV320AIC23_DIGT, 1, 4, deemph_text);

static const DECLARE_TLV_DB_SCALE(out_gain_tlv, -12100, 100, 0);
static const DECLARE_TLV_DB_SCALE(input_gain_tlv, -1725, 75, 0);
static const DECLARE_TLV_DB_SCALE(sidetone_vol_tlv, -1800, 300, 0);

static int snd_soc_tlv320aic23_put_volsw(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	u16 val, reg;

	val = (ucontrol->value.integer.value[0] & 0x07);

	/* linear conversion to userspace
	* 000	=	-6db
	* 001	=	-9db
	* 010	=	-12db
	* 011	=	-18db (Min)
	* 100	=	0db (Max)
	*/
	val = (val >= 4) ? 4  : (3 - val);

<<<<<<< HEAD
<<<<<<< HEAD
	reg = tlv320aic23_read_reg_cache(codec, TLV320AIC23_ANLG) & (~0x1C0);
	tlv320aic23_write(codec, TLV320AIC23_ANLG, reg | (val << 6));
=======
	reg = snd_soc_read(codec, TLV320AIC23_ANLG) & (~0x1C0);
	snd_soc_write(codec, TLV320AIC23_ANLG, reg | (val << 6));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	reg = snd_soc_read(codec, TLV320AIC23_ANLG) & (~0x1C0);
	snd_soc_write(codec, TLV320AIC23_ANLG, reg | (val << 6));
>>>>>>> refs/remotes/origin/master

	return 0;
}

static int snd_soc_tlv320aic23_get_volsw(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	u16 val;

<<<<<<< HEAD
<<<<<<< HEAD
	val = tlv320aic23_read_reg_cache(codec, TLV320AIC23_ANLG) & (0x1C0);
=======
	val = snd_soc_read(codec, TLV320AIC23_ANLG) & (0x1C0);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	val = snd_soc_read(codec, TLV320AIC23_ANLG) & (0x1C0);
>>>>>>> refs/remotes/origin/master
	val = val >> 6;
	val = (val >= 4) ? 4  : (3 -  val);
	ucontrol->value.integer.value[0] = val;
	return 0;

}

<<<<<<< HEAD
<<<<<<< HEAD
#define SOC_TLV320AIC23_SINGLE_TLV(xname, reg, shift, max, invert, tlv_array) \
{	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
	.access = SNDRV_CTL_ELEM_ACCESS_TLV_READ |\
		 SNDRV_CTL_ELEM_ACCESS_READWRITE,\
	.tlv.p = (tlv_array), \
	.info = snd_soc_info_volsw, .get = snd_soc_tlv320aic23_get_volsw,\
	.put = snd_soc_tlv320aic23_put_volsw, \
	.private_value =  SOC_SINGLE_VALUE(reg, shift, max, invert) }

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static const struct snd_kcontrol_new tlv320aic23_snd_controls[] = {
	SOC_DOUBLE_R_TLV("Digital Playback Volume", TLV320AIC23_LCHNVOL,
			 TLV320AIC23_RCHNVOL, 0, 127, 0, out_gain_tlv),
	SOC_SINGLE("Digital Playback Switch", TLV320AIC23_DIGT, 3, 1, 1),
	SOC_DOUBLE_R("Line Input Switch", TLV320AIC23_LINVOL,
		     TLV320AIC23_RINVOL, 7, 1, 0),
	SOC_DOUBLE_R_TLV("Line Input Volume", TLV320AIC23_LINVOL,
			 TLV320AIC23_RINVOL, 0, 31, 0, input_gain_tlv),
	SOC_SINGLE("Mic Input Switch", TLV320AIC23_ANLG, 1, 1, 1),
	SOC_SINGLE("Mic Booster Switch", TLV320AIC23_ANLG, 0, 1, 0),
<<<<<<< HEAD
<<<<<<< HEAD
	SOC_TLV320AIC23_SINGLE_TLV("Sidetone Volume", TLV320AIC23_ANLG,
				  6, 4, 0, sidetone_vol_tlv),
=======
	SOC_SINGLE_EXT_TLV("Sidetone Volume", TLV320AIC23_ANLG, 6, 4, 0,
			   snd_soc_tlv320aic23_get_volsw,
			   snd_soc_tlv320aic23_put_volsw, sidetone_vol_tlv),
>>>>>>> refs/remotes/origin/cm-10.0
=======
	SOC_SINGLE_EXT_TLV("Sidetone Volume", TLV320AIC23_ANLG, 6, 4, 0,
			   snd_soc_tlv320aic23_get_volsw,
			   snd_soc_tlv320aic23_put_volsw, sidetone_vol_tlv),
>>>>>>> refs/remotes/origin/master
	SOC_ENUM("Playback De-emphasis", tlv320aic23_deemph),
};

/* PGA Mixer controls for Line and Mic switch */
static const struct snd_kcontrol_new tlv320aic23_output_mixer_controls[] = {
	SOC_DAPM_SINGLE("Line Bypass Switch", TLV320AIC23_ANLG, 3, 1, 0),
	SOC_DAPM_SINGLE("Mic Sidetone Switch", TLV320AIC23_ANLG, 5, 1, 0),
	SOC_DAPM_SINGLE("Playback Switch", TLV320AIC23_ANLG, 4, 1, 0),
};

static const struct snd_soc_dapm_widget tlv320aic23_dapm_widgets[] = {
	SND_SOC_DAPM_DAC("DAC", "Playback", TLV320AIC23_PWR, 3, 1),
	SND_SOC_DAPM_ADC("ADC", "Capture", TLV320AIC23_PWR, 2, 1),
	SND_SOC_DAPM_MUX("Capture Source", SND_SOC_NOPM, 0, 0,
			 &tlv320aic23_rec_src_mux_controls),
	SND_SOC_DAPM_MIXER("Output Mixer", TLV320AIC23_PWR, 4, 1,
			   &tlv320aic23_output_mixer_controls[0],
			   ARRAY_SIZE(tlv320aic23_output_mixer_controls)),
	SND_SOC_DAPM_PGA("Line Input", TLV320AIC23_PWR, 0, 1, NULL, 0),
	SND_SOC_DAPM_PGA("Mic Input", TLV320AIC23_PWR, 1, 1, NULL, 0),

	SND_SOC_DAPM_OUTPUT("LHPOUT"),
	SND_SOC_DAPM_OUTPUT("RHPOUT"),
	SND_SOC_DAPM_OUTPUT("LOUT"),
	SND_SOC_DAPM_OUTPUT("ROUT"),

	SND_SOC_DAPM_INPUT("LLINEIN"),
	SND_SOC_DAPM_INPUT("RLINEIN"),

	SND_SOC_DAPM_INPUT("MICIN"),
};

static const struct snd_soc_dapm_route tlv320aic23_intercon[] = {
	/* Output Mixer */
	{"Output Mixer", "Line Bypass Switch", "Line Input"},
	{"Output Mixer", "Playback Switch", "DAC"},
	{"Output Mixer", "Mic Sidetone Switch", "Mic Input"},

	/* Outputs */
	{"RHPOUT", NULL, "Output Mixer"},
	{"LHPOUT", NULL, "Output Mixer"},
	{"LOUT", NULL, "Output Mixer"},
	{"ROUT", NULL, "Output Mixer"},

	/* Inputs */
	{"Line Input", "NULL", "LLINEIN"},
	{"Line Input", "NULL", "RLINEIN"},

	{"Mic Input", "NULL", "MICIN"},

	/* input mux */
	{"Capture Source", "Line", "Line Input"},
	{"Capture Source", "Mic", "Mic Input"},
	{"ADC", NULL, "Capture Source"},

};

/* AIC23 driver data */
struct aic23 {
<<<<<<< HEAD
	enum snd_soc_control_type control_type;
<<<<<<< HEAD
	void *control_data;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct regmap *regmap;
>>>>>>> refs/remotes/origin/master
	int mclk;
	int requested_adc;
	int requested_dac;
};

/*
 * Common Crystals used
 * 11.2896 Mhz /128 = *88.2k  /192 = 58.8k
 * 12.0000 Mhz /125 = *96k    /136 = 88.235K
 * 12.2880 Mhz /128 = *96k    /192 = 64k
 * 16.9344 Mhz /128 = 132.3k /192 = *88.2k
 * 18.4320 Mhz /128 = 144k   /192 = *96k
 */

/*
 * Normal BOSR 0-256/2 = 128, 1-384/2 = 192
 * USB BOSR 0-250/2 = 125, 1-272/2 = 136
 */
static const int bosr_usb_divisor_table[] = {
	128, 125, 192, 136
};
#define LOWER_GROUP ((1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<6) | (1<<7))
#define UPPER_GROUP ((1<<8) | (1<<9) | (1<<10) | (1<<11)        | (1<<15))
static const unsigned short sr_valid_mask[] = {
	LOWER_GROUP|UPPER_GROUP,	/* Normal, bosr - 0*/
	LOWER_GROUP,			/* Usb, bosr - 0*/
	LOWER_GROUP|UPPER_GROUP,	/* Normal, bosr - 1*/
	UPPER_GROUP,			/* Usb, bosr - 1*/
};
/*
 * Every divisor is a factor of 11*12
 */
#define SR_MULT (11*12)
#define A(x) (SR_MULT/x)
static const unsigned char sr_adc_mult_table[] = {
	A(2), A(2), A(12), A(12),  0, 0, A(3), A(1),
	A(2), A(2), A(11), A(11),  0, 0, 0, A(1)
};
static const unsigned char sr_dac_mult_table[] = {
	A(2), A(12), A(2), A(12),  0, 0, A(3), A(1),
	A(2), A(11), A(2), A(11),  0, 0, 0, A(1)
};

static unsigned get_score(int adc, int adc_l, int adc_h, int need_adc,
		int dac, int dac_l, int dac_h, int need_dac)
{
	if ((adc >= adc_l) && (adc <= adc_h) &&
			(dac >= dac_l) && (dac <= dac_h)) {
		int diff_adc = need_adc - adc;
		int diff_dac = need_dac - dac;
		return abs(diff_adc) + abs(diff_dac);
	}
	return UINT_MAX;
}

static int find_rate(int mclk, u32 need_adc, u32 need_dac)
{
	int i, j;
	int best_i = -1;
	int best_j = -1;
	int best_div = 0;
	unsigned best_score = UINT_MAX;
	int adc_l, adc_h, dac_l, dac_h;

	need_adc *= SR_MULT;
	need_dac *= SR_MULT;
	/*
	 * rates given are +/- 1/32
	 */
	adc_l = need_adc - (need_adc >> 5);
	adc_h = need_adc + (need_adc >> 5);
	dac_l = need_dac - (need_dac >> 5);
	dac_h = need_dac + (need_dac >> 5);
	for (i = 0; i < ARRAY_SIZE(bosr_usb_divisor_table); i++) {
		int base = mclk / bosr_usb_divisor_table[i];
		int mask = sr_valid_mask[i];
		for (j = 0; j < ARRAY_SIZE(sr_adc_mult_table);
				j++, mask >>= 1) {
			int adc;
			int dac;
			int score;
			if ((mask & 1) == 0)
				continue;
			adc = base * sr_adc_mult_table[j];
			dac = base * sr_dac_mult_table[j];
			score = get_score(adc, adc_l, adc_h, need_adc,
					dac, dac_l, dac_h, need_dac);
			if (best_score > score) {
				best_score = score;
				best_i = i;
				best_j = j;
				best_div = 0;
			}
			score = get_score((adc >> 1), adc_l, adc_h, need_adc,
					(dac >> 1), dac_l, dac_h, need_dac);
			/* prefer to have a /2 */
			if ((score != UINT_MAX) && (best_score >= score)) {
				best_score = score;
				best_i = i;
				best_j = j;
				best_div = 1;
			}
		}
	}
	return (best_j << 2) | best_i | (best_div << TLV320AIC23_CLKIN_SHIFT);
}

#ifdef DEBUG
static void get_current_sample_rates(struct snd_soc_codec *codec, int mclk,
		u32 *sample_rate_adc, u32 *sample_rate_dac)
{
<<<<<<< HEAD
<<<<<<< HEAD
	int src = tlv320aic23_read_reg_cache(codec, TLV320AIC23_SRATE);
=======
	int src = snd_soc_read(codec, TLV320AIC23_SRATE);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	int src = snd_soc_read(codec, TLV320AIC23_SRATE);
>>>>>>> refs/remotes/origin/master
	int sr = (src >> 2) & 0x0f;
	int val = (mclk / bosr_usb_divisor_table[src & 3]);
	int adc = (val * sr_adc_mult_table[sr]) / SR_MULT;
	int dac = (val * sr_dac_mult_table[sr]) / SR_MULT;
	if (src & TLV320AIC23_CLKIN_HALF) {
		adc >>= 1;
		dac >>= 1;
	}
	*sample_rate_adc = adc;
	*sample_rate_dac = dac;
}
#endif

static int set_sample_rate_control(struct snd_soc_codec *codec, int mclk,
		u32 sample_rate_adc, u32 sample_rate_dac)
{
	/* Search for the right sample rate */
	int data = find_rate(mclk, sample_rate_adc, sample_rate_dac);
	if (data < 0) {
		printk(KERN_ERR "%s:Invalid rate %u,%u requested\n",
				__func__, sample_rate_adc, sample_rate_dac);
		return -EINVAL;
	}
<<<<<<< HEAD
<<<<<<< HEAD
	tlv320aic23_write(codec, TLV320AIC23_SRATE, data);
=======
	snd_soc_write(codec, TLV320AIC23_SRATE, data);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	snd_soc_write(codec, TLV320AIC23_SRATE, data);
>>>>>>> refs/remotes/origin/master
#ifdef DEBUG
	{
		u32 adc, dac;
		get_current_sample_rates(codec, mclk, &adc, &dac);
		printk(KERN_DEBUG "actual samplerate = %u,%u reg=%x\n",
			adc, dac, data);
	}
#endif
	return 0;
}

static int tlv320aic23_hw_params(struct snd_pcm_substream *substream,
				 struct snd_pcm_hw_params *params,
				 struct snd_soc_dai *dai)
{
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_codec *codec = rtd->codec;
=======
	struct snd_soc_codec *codec = dai->codec;
>>>>>>> refs/remotes/origin/master
	u16 iface_reg;
	int ret;
	struct aic23 *aic23 = snd_soc_codec_get_drvdata(codec);
	u32 sample_rate_adc = aic23->requested_adc;
	u32 sample_rate_dac = aic23->requested_dac;
	u32 sample_rate = params_rate(params);

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
		aic23->requested_dac = sample_rate_dac = sample_rate;
		if (!sample_rate_adc)
			sample_rate_adc = sample_rate;
	} else {
		aic23->requested_adc = sample_rate_adc = sample_rate;
		if (!sample_rate_dac)
			sample_rate_dac = sample_rate;
	}
	ret = set_sample_rate_control(codec, aic23->mclk, sample_rate_adc,
			sample_rate_dac);
	if (ret < 0)
		return ret;

<<<<<<< HEAD
<<<<<<< HEAD
	iface_reg =
	    tlv320aic23_read_reg_cache(codec,
				       TLV320AIC23_DIGT_FMT) & ~(0x03 << 2);
=======
	iface_reg = snd_soc_read(codec, TLV320AIC23_DIGT_FMT) & ~(0x03 << 2);

>>>>>>> refs/remotes/origin/cm-10.0
=======
	iface_reg = snd_soc_read(codec, TLV320AIC23_DIGT_FMT) & ~(0x03 << 2);

>>>>>>> refs/remotes/origin/master
	switch (params_format(params)) {
	case SNDRV_PCM_FORMAT_S16_LE:
		break;
	case SNDRV_PCM_FORMAT_S20_3LE:
		iface_reg |= (0x01 << 2);
		break;
	case SNDRV_PCM_FORMAT_S24_LE:
		iface_reg |= (0x02 << 2);
		break;
	case SNDRV_PCM_FORMAT_S32_LE:
		iface_reg |= (0x03 << 2);
		break;
	}
<<<<<<< HEAD
<<<<<<< HEAD
	tlv320aic23_write(codec, TLV320AIC23_DIGT_FMT, iface_reg);
=======
	snd_soc_write(codec, TLV320AIC23_DIGT_FMT, iface_reg);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	snd_soc_write(codec, TLV320AIC23_DIGT_FMT, iface_reg);
>>>>>>> refs/remotes/origin/master

	return 0;
}

static int tlv320aic23_pcm_prepare(struct snd_pcm_substream *substream,
				   struct snd_soc_dai *dai)
{
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_codec *codec = rtd->codec;

	/* set active */
<<<<<<< HEAD
	tlv320aic23_write(codec, TLV320AIC23_ACTIVE, 0x0001);
=======
	snd_soc_write(codec, TLV320AIC23_ACTIVE, 0x0001);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct snd_soc_codec *codec = dai->codec;

	/* set active */
	snd_soc_write(codec, TLV320AIC23_ACTIVE, 0x0001);
>>>>>>> refs/remotes/origin/master

	return 0;
}

static void tlv320aic23_shutdown(struct snd_pcm_substream *substream,
				 struct snd_soc_dai *dai)
{
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_codec *codec = rtd->codec;
=======
	struct snd_soc_codec *codec = dai->codec;
>>>>>>> refs/remotes/origin/master
	struct aic23 *aic23 = snd_soc_codec_get_drvdata(codec);

	/* deactivate */
	if (!codec->active) {
		udelay(50);
<<<<<<< HEAD
<<<<<<< HEAD
		tlv320aic23_write(codec, TLV320AIC23_ACTIVE, 0x0);
=======
		snd_soc_write(codec, TLV320AIC23_ACTIVE, 0x0);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		snd_soc_write(codec, TLV320AIC23_ACTIVE, 0x0);
>>>>>>> refs/remotes/origin/master
	}
	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		aic23->requested_dac = 0;
	else
		aic23->requested_adc = 0;
}

static int tlv320aic23_mute(struct snd_soc_dai *dai, int mute)
{
	struct snd_soc_codec *codec = dai->codec;
	u16 reg;

<<<<<<< HEAD
<<<<<<< HEAD
	reg = tlv320aic23_read_reg_cache(codec, TLV320AIC23_DIGT);
=======
	reg = snd_soc_read(codec, TLV320AIC23_DIGT);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	reg = snd_soc_read(codec, TLV320AIC23_DIGT);
>>>>>>> refs/remotes/origin/master
	if (mute)
		reg |= TLV320AIC23_DACM_MUTE;

	else
		reg &= ~TLV320AIC23_DACM_MUTE;

<<<<<<< HEAD
<<<<<<< HEAD
	tlv320aic23_write(codec, TLV320AIC23_DIGT, reg);
=======
	snd_soc_write(codec, TLV320AIC23_DIGT, reg);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	snd_soc_write(codec, TLV320AIC23_DIGT, reg);
>>>>>>> refs/remotes/origin/master

	return 0;
}

static int tlv320aic23_set_dai_fmt(struct snd_soc_dai *codec_dai,
				   unsigned int fmt)
{
	struct snd_soc_codec *codec = codec_dai->codec;
	u16 iface_reg;

<<<<<<< HEAD
<<<<<<< HEAD
	iface_reg =
	    tlv320aic23_read_reg_cache(codec, TLV320AIC23_DIGT_FMT) & (~0x03);
=======
	iface_reg = snd_soc_read(codec, TLV320AIC23_DIGT_FMT) & (~0x03);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	iface_reg = snd_soc_read(codec, TLV320AIC23_DIGT_FMT) & (~0x03);
>>>>>>> refs/remotes/origin/master

	/* set master/slave audio interface */
	switch (fmt & SND_SOC_DAIFMT_MASTER_MASK) {
	case SND_SOC_DAIFMT_CBM_CFM:
		iface_reg |= TLV320AIC23_MS_MASTER;
		break;
	case SND_SOC_DAIFMT_CBS_CFS:
<<<<<<< HEAD
<<<<<<< HEAD
=======
		iface_reg &= ~TLV320AIC23_MS_MASTER;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		iface_reg &= ~TLV320AIC23_MS_MASTER;
>>>>>>> refs/remotes/origin/master
		break;
	default:
		return -EINVAL;

	}

	/* interface format */
	switch (fmt & SND_SOC_DAIFMT_FORMAT_MASK) {
	case SND_SOC_DAIFMT_I2S:
		iface_reg |= TLV320AIC23_FOR_I2S;
		break;
	case SND_SOC_DAIFMT_DSP_A:
		iface_reg |= TLV320AIC23_LRP_ON;
	case SND_SOC_DAIFMT_DSP_B:
		iface_reg |= TLV320AIC23_FOR_DSP;
		break;
	case SND_SOC_DAIFMT_RIGHT_J:
		break;
	case SND_SOC_DAIFMT_LEFT_J:
		iface_reg |= TLV320AIC23_FOR_LJUST;
		break;
	default:
		return -EINVAL;

	}

<<<<<<< HEAD
<<<<<<< HEAD
	tlv320aic23_write(codec, TLV320AIC23_DIGT_FMT, iface_reg);
=======
	snd_soc_write(codec, TLV320AIC23_DIGT_FMT, iface_reg);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	snd_soc_write(codec, TLV320AIC23_DIGT_FMT, iface_reg);
>>>>>>> refs/remotes/origin/master

	return 0;
}

static int tlv320aic23_set_dai_sysclk(struct snd_soc_dai *codec_dai,
				      int clk_id, unsigned int freq, int dir)
{
	struct aic23 *aic23 = snd_soc_dai_get_drvdata(codec_dai);
	aic23->mclk = freq;
	return 0;
}

static int tlv320aic23_set_bias_level(struct snd_soc_codec *codec,
				      enum snd_soc_bias_level level)
{
<<<<<<< HEAD
<<<<<<< HEAD
	u16 reg = tlv320aic23_read_reg_cache(codec, TLV320AIC23_PWR) & 0xff7f;
=======
	u16 reg = snd_soc_read(codec, TLV320AIC23_PWR) & 0x17f;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	u16 reg = snd_soc_read(codec, TLV320AIC23_PWR) & 0x17f;
>>>>>>> refs/remotes/origin/master

	switch (level) {
	case SND_SOC_BIAS_ON:
		/* vref/mid, osc on, dac unmute */
		reg &= ~(TLV320AIC23_DEVICE_PWR_OFF | TLV320AIC23_OSC_OFF | \
			TLV320AIC23_DAC_OFF);
<<<<<<< HEAD
<<<<<<< HEAD
		tlv320aic23_write(codec, TLV320AIC23_PWR, reg);
=======
		snd_soc_write(codec, TLV320AIC23_PWR, reg);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		snd_soc_write(codec, TLV320AIC23_PWR, reg);
>>>>>>> refs/remotes/origin/master
		break;
	case SND_SOC_BIAS_PREPARE:
		break;
	case SND_SOC_BIAS_STANDBY:
		/* everything off except vref/vmid, */
<<<<<<< HEAD
<<<<<<< HEAD
		tlv320aic23_write(codec, TLV320AIC23_PWR, reg | \
			TLV320AIC23_CLK_OFF);
		break;
	case SND_SOC_BIAS_OFF:
		/* everything off, dac mute, inactive */
		tlv320aic23_write(codec, TLV320AIC23_ACTIVE, 0x0);
		tlv320aic23_write(codec, TLV320AIC23_PWR, 0xffff);
=======
=======
>>>>>>> refs/remotes/origin/master
		snd_soc_write(codec, TLV320AIC23_PWR,
			      reg | TLV320AIC23_CLK_OFF);
		break;
	case SND_SOC_BIAS_OFF:
		/* everything off, dac mute, inactive */
		snd_soc_write(codec, TLV320AIC23_ACTIVE, 0x0);
		snd_soc_write(codec, TLV320AIC23_PWR, 0x1ff);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		break;
	}
	codec->dapm.bias_level = level;
	return 0;
}

#define AIC23_RATES	SNDRV_PCM_RATE_8000_96000
#define AIC23_FORMATS	(SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S20_3LE | \
			 SNDRV_PCM_FMTBIT_S24_3LE | SNDRV_PCM_FMTBIT_S32_LE)

<<<<<<< HEAD
<<<<<<< HEAD
static struct snd_soc_dai_ops tlv320aic23_dai_ops = {
=======
static const struct snd_soc_dai_ops tlv320aic23_dai_ops = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static const struct snd_soc_dai_ops tlv320aic23_dai_ops = {
>>>>>>> refs/remotes/origin/master
	.prepare	= tlv320aic23_pcm_prepare,
	.hw_params	= tlv320aic23_hw_params,
	.shutdown	= tlv320aic23_shutdown,
	.digital_mute	= tlv320aic23_mute,
	.set_fmt	= tlv320aic23_set_dai_fmt,
	.set_sysclk	= tlv320aic23_set_dai_sysclk,
};

static struct snd_soc_dai_driver tlv320aic23_dai = {
	.name = "tlv320aic23-hifi",
	.playback = {
		     .stream_name = "Playback",
		     .channels_min = 2,
		     .channels_max = 2,
		     .rates = AIC23_RATES,
		     .formats = AIC23_FORMATS,},
	.capture = {
		    .stream_name = "Capture",
		    .channels_min = 2,
		    .channels_max = 2,
		    .rates = AIC23_RATES,
		    .formats = AIC23_FORMATS,},
	.ops = &tlv320aic23_dai_ops,
};

<<<<<<< HEAD
<<<<<<< HEAD
static int tlv320aic23_suspend(struct snd_soc_codec *codec,
			       pm_message_t state)
=======
static int tlv320aic23_suspend(struct snd_soc_codec *codec)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int tlv320aic23_suspend(struct snd_soc_codec *codec)
>>>>>>> refs/remotes/origin/master
{
	tlv320aic23_set_bias_level(codec, SND_SOC_BIAS_OFF);

	return 0;
}

static int tlv320aic23_resume(struct snd_soc_codec *codec)
{
<<<<<<< HEAD
<<<<<<< HEAD
	u16 reg;

	/* Sync reg_cache with the hardware */
	for (reg = 0; reg <= TLV320AIC23_ACTIVE; reg++) {
		u16 val = tlv320aic23_read_reg_cache(codec, reg);
		tlv320aic23_write(codec, reg, val);
	}
=======
	snd_soc_cache_sync(codec);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct aic23 *aic23 = snd_soc_codec_get_drvdata(codec);
	regcache_mark_dirty(aic23->regmap);
	regcache_sync(aic23->regmap);
>>>>>>> refs/remotes/origin/master
	tlv320aic23_set_bias_level(codec, SND_SOC_BIAS_STANDBY);

	return 0;
}

static int tlv320aic23_probe(struct snd_soc_codec *codec)
{
<<<<<<< HEAD
	struct aic23 *aic23 = snd_soc_codec_get_drvdata(codec);
<<<<<<< HEAD
	int reg;

	printk(KERN_INFO "AIC23 Audio Codec %s\n", AIC23_VERSION);
	codec->control_data = aic23->control_data;
	codec->hw_write = (hw_write_t)i2c_master_send;
	codec->hw_read = NULL;

	/* Reset codec */
	tlv320aic23_write(codec, TLV320AIC23_RESET, 0);
=======
	int ret;

	printk(KERN_INFO "AIC23 Audio Codec %s\n", AIC23_VERSION);

	ret = snd_soc_codec_set_cache_io(codec, 7, 9, aic23->control_type);
=======
	int ret;

	ret = snd_soc_codec_set_cache_io(codec, 7, 9, SND_SOC_REGMAP);
>>>>>>> refs/remotes/origin/master
	if (ret < 0) {
		dev_err(codec->dev, "Failed to set cache I/O: %d\n", ret);
		return ret;
	}

	/* Reset codec */
	snd_soc_write(codec, TLV320AIC23_RESET, 0);

<<<<<<< HEAD
	/* Write the register default value to cache for reserved registers,
	 * so the write to the these registers are suppressed by the cache
	 * restore code when it skips writes of default registers.
	 */
	snd_soc_cache_write(codec, 0x0A, 0);
	snd_soc_cache_write(codec, 0x0B, 0);
	snd_soc_cache_write(codec, 0x0C, 0);
	snd_soc_cache_write(codec, 0x0D, 0);
	snd_soc_cache_write(codec, 0x0E, 0);
>>>>>>> refs/remotes/origin/cm-10.0

	/* power on device */
	tlv320aic23_set_bias_level(codec, SND_SOC_BIAS_STANDBY);

<<<<<<< HEAD
	tlv320aic23_write(codec, TLV320AIC23_DIGT, TLV320AIC23_DEEMP_44K);

	/* Unmute input */
	reg = tlv320aic23_read_reg_cache(codec, TLV320AIC23_LINVOL);
	tlv320aic23_write(codec, TLV320AIC23_LINVOL,
			  (reg & (~TLV320AIC23_LIM_MUTED)) |
			  (TLV320AIC23_LRS_ENABLED));

	reg = tlv320aic23_read_reg_cache(codec, TLV320AIC23_RINVOL);
	tlv320aic23_write(codec, TLV320AIC23_RINVOL,
			  (reg & (~TLV320AIC23_LIM_MUTED)) |
			  TLV320AIC23_LRS_ENABLED);

	reg = tlv320aic23_read_reg_cache(codec, TLV320AIC23_ANLG);
	tlv320aic23_write(codec, TLV320AIC23_ANLG,
			 (reg) & (~TLV320AIC23_BYPASS_ON) &
			 (~TLV320AIC23_MICM_MUTED));

	/* Default output volume */
	tlv320aic23_write(codec, TLV320AIC23_LCHNVOL,
			  TLV320AIC23_DEFAULT_OUT_VOL &
			  TLV320AIC23_OUT_VOL_MASK);
	tlv320aic23_write(codec, TLV320AIC23_RCHNVOL,
			  TLV320AIC23_DEFAULT_OUT_VOL &
			  TLV320AIC23_OUT_VOL_MASK);

	tlv320aic23_write(codec, TLV320AIC23_ACTIVE, 0x1);

	snd_soc_add_controls(codec, tlv320aic23_snd_controls,
=======
=======
	/* power on device */
	tlv320aic23_set_bias_level(codec, SND_SOC_BIAS_STANDBY);

>>>>>>> refs/remotes/origin/master
	snd_soc_write(codec, TLV320AIC23_DIGT, TLV320AIC23_DEEMP_44K);

	/* Unmute input */
	snd_soc_update_bits(codec, TLV320AIC23_LINVOL,
			    TLV320AIC23_LIM_MUTED, TLV320AIC23_LRS_ENABLED);

	snd_soc_update_bits(codec, TLV320AIC23_RINVOL,
			    TLV320AIC23_LIM_MUTED, TLV320AIC23_LRS_ENABLED);

	snd_soc_update_bits(codec, TLV320AIC23_ANLG,
			    TLV320AIC23_BYPASS_ON | TLV320AIC23_MICM_MUTED,
			    0);

	/* Default output volume */
	snd_soc_write(codec, TLV320AIC23_LCHNVOL,
		      TLV320AIC23_DEFAULT_OUT_VOL & TLV320AIC23_OUT_VOL_MASK);
	snd_soc_write(codec, TLV320AIC23_RCHNVOL,
		      TLV320AIC23_DEFAULT_OUT_VOL & TLV320AIC23_OUT_VOL_MASK);

	snd_soc_write(codec, TLV320AIC23_ACTIVE, 0x1);

<<<<<<< HEAD
	snd_soc_add_codec_controls(codec, tlv320aic23_snd_controls,
>>>>>>> refs/remotes/origin/cm-10.0
				ARRAY_SIZE(tlv320aic23_snd_controls));

=======
>>>>>>> refs/remotes/origin/master
	return 0;
}

static int tlv320aic23_remove(struct snd_soc_codec *codec)
{
	tlv320aic23_set_bias_level(codec, SND_SOC_BIAS_OFF);
	return 0;
}

static struct snd_soc_codec_driver soc_codec_dev_tlv320aic23 = {
<<<<<<< HEAD
	.reg_cache_size = ARRAY_SIZE(tlv320aic23_reg),
	.reg_word_size = sizeof(u16),
	.reg_cache_default = tlv320aic23_reg,
=======
>>>>>>> refs/remotes/origin/master
	.probe = tlv320aic23_probe,
	.remove = tlv320aic23_remove,
	.suspend = tlv320aic23_suspend,
	.resume = tlv320aic23_resume,
<<<<<<< HEAD
<<<<<<< HEAD
	.read = tlv320aic23_read_reg_cache,
	.write = tlv320aic23_write,
=======
>>>>>>> refs/remotes/origin/cm-10.0
	.set_bias_level = tlv320aic23_set_bias_level,
=======
	.set_bias_level = tlv320aic23_set_bias_level,
	.controls = tlv320aic23_snd_controls,
	.num_controls = ARRAY_SIZE(tlv320aic23_snd_controls),
>>>>>>> refs/remotes/origin/master
	.dapm_widgets = tlv320aic23_dapm_widgets,
	.num_dapm_widgets = ARRAY_SIZE(tlv320aic23_dapm_widgets),
	.dapm_routes = tlv320aic23_intercon,
	.num_dapm_routes = ARRAY_SIZE(tlv320aic23_intercon),
};

<<<<<<< HEAD
#if defined(CONFIG_I2C) || defined(CONFIG_I2C_MODULE)
=======
>>>>>>> refs/remotes/origin/master
/*
 * If the i2c layer weren't so broken, we could pass this kind of data
 * around
 */
static int tlv320aic23_codec_probe(struct i2c_client *i2c,
				   const struct i2c_device_id *i2c_id)
{
	struct aic23 *aic23;
	int ret;

	if (!i2c_check_functionality(i2c->adapter, I2C_FUNC_SMBUS_BYTE_DATA))
		return -EINVAL;

<<<<<<< HEAD
<<<<<<< HEAD
	aic23 = kzalloc(sizeof(struct aic23), GFP_KERNEL);
=======
	aic23 = devm_kzalloc(&i2c->dev, sizeof(struct aic23), GFP_KERNEL);
>>>>>>> refs/remotes/origin/cm-10.0
	if (aic23 == NULL)
		return -ENOMEM;

	i2c_set_clientdata(i2c, aic23);
<<<<<<< HEAD
	aic23->control_data = i2c;
=======
>>>>>>> refs/remotes/origin/cm-10.0
	aic23->control_type = SND_SOC_I2C;

	ret =  snd_soc_register_codec(&i2c->dev,
			&soc_codec_dev_tlv320aic23, &tlv320aic23_dai, 1);
<<<<<<< HEAD
	if (ret < 0)
		kfree(aic23);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	aic23 = devm_kzalloc(&i2c->dev, sizeof(struct aic23), GFP_KERNEL);
	if (aic23 == NULL)
		return -ENOMEM;

	aic23->regmap = devm_regmap_init_i2c(i2c, &tlv320aic23_regmap);
	if (IS_ERR(aic23->regmap))
		return PTR_ERR(aic23->regmap);

	i2c_set_clientdata(i2c, aic23);

	ret =  snd_soc_register_codec(&i2c->dev,
			&soc_codec_dev_tlv320aic23, &tlv320aic23_dai, 1);
>>>>>>> refs/remotes/origin/master
	return ret;
}
static int __exit tlv320aic23_i2c_remove(struct i2c_client *i2c)
{
	snd_soc_unregister_codec(&i2c->dev);
<<<<<<< HEAD
<<<<<<< HEAD
	kfree(i2c_get_clientdata(i2c));
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return 0;
}

static const struct i2c_device_id tlv320aic23_id[] = {
	{"tlv320aic23", 0},
	{}
};

MODULE_DEVICE_TABLE(i2c, tlv320aic23_id);

static struct i2c_driver tlv320aic23_i2c_driver = {
	.driver = {
		   .name = "tlv320aic23-codec",
		   },
	.probe = tlv320aic23_codec_probe,
	.remove = __exit_p(tlv320aic23_i2c_remove),
	.id_table = tlv320aic23_id,
};

<<<<<<< HEAD
#endif

static int __init tlv320aic23_modinit(void)
{
	int ret;
#if defined(CONFIG_I2C) || defined(CONFIG_I2C_MODULE)
	ret = i2c_add_driver(&tlv320aic23_i2c_driver);
	if (ret != 0) {
		printk(KERN_ERR "Failed to register TLV320AIC23 I2C driver: %d\n",
		       ret);
	}
#endif
	return ret;
}
module_init(tlv320aic23_modinit);

static void __exit tlv320aic23_exit(void)
{
#if defined(CONFIG_I2C) || defined(CONFIG_I2C_MODULE)
	i2c_del_driver(&tlv320aic23_i2c_driver);
#endif
}
module_exit(tlv320aic23_exit);
=======
module_i2c_driver(tlv320aic23_i2c_driver);
>>>>>>> refs/remotes/origin/master

MODULE_DESCRIPTION("ASoC TLV320AIC23 codec driver");
MODULE_AUTHOR("Arun KS <arunks@mistralsolutions.com>");
MODULE_LICENSE("GPL");
