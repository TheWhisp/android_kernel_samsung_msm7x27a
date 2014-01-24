/*
 * Copyright (C) 2009-2010, Lars-Peter Clausen <lars@metafoo.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  You should have received a copy of the  GNU General Public License along
 *  with this program; if not, write  to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/io.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/io.h>
#include <linux/regmap.h>
>>>>>>> refs/remotes/origin/master

#include <linux/delay.h>

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/initval.h>
#include <sound/soc.h>
<<<<<<< HEAD

#define JZ4740_REG_CODEC_1 0x0
#define JZ4740_REG_CODEC_2 0x1
=======
#include <sound/tlv.h>

#define JZ4740_REG_CODEC_1 0x0
#define JZ4740_REG_CODEC_2 0x4
>>>>>>> refs/remotes/origin/master

#define JZ4740_CODEC_1_LINE_ENABLE BIT(29)
#define JZ4740_CODEC_1_MIC_ENABLE BIT(28)
#define JZ4740_CODEC_1_SW1_ENABLE BIT(27)
#define JZ4740_CODEC_1_ADC_ENABLE BIT(26)
#define JZ4740_CODEC_1_SW2_ENABLE BIT(25)
#define JZ4740_CODEC_1_DAC_ENABLE BIT(24)
#define JZ4740_CODEC_1_VREF_DISABLE BIT(20)
#define JZ4740_CODEC_1_VREF_AMP_DISABLE BIT(19)
#define JZ4740_CODEC_1_VREF_PULLDOWN BIT(18)
#define JZ4740_CODEC_1_VREF_LOW_CURRENT BIT(17)
#define JZ4740_CODEC_1_VREF_HIGH_CURRENT BIT(16)
#define JZ4740_CODEC_1_HEADPHONE_DISABLE BIT(14)
#define JZ4740_CODEC_1_HEADPHONE_AMP_CHANGE_ANY BIT(13)
#define JZ4740_CODEC_1_HEADPHONE_CHARGE BIT(12)
#define JZ4740_CODEC_1_HEADPHONE_PULLDOWN (BIT(11) | BIT(10))
#define JZ4740_CODEC_1_HEADPHONE_POWERDOWN_M BIT(9)
#define JZ4740_CODEC_1_HEADPHONE_POWERDOWN BIT(8)
#define JZ4740_CODEC_1_SUSPEND BIT(1)
#define JZ4740_CODEC_1_RESET BIT(0)

#define JZ4740_CODEC_1_LINE_ENABLE_OFFSET 29
#define JZ4740_CODEC_1_MIC_ENABLE_OFFSET 28
#define JZ4740_CODEC_1_SW1_ENABLE_OFFSET 27
#define JZ4740_CODEC_1_ADC_ENABLE_OFFSET 26
#define JZ4740_CODEC_1_SW2_ENABLE_OFFSET 25
#define JZ4740_CODEC_1_DAC_ENABLE_OFFSET 24
#define JZ4740_CODEC_1_HEADPHONE_DISABLE_OFFSET 14
#define JZ4740_CODEC_1_HEADPHONE_POWERDOWN_OFFSET 8

#define JZ4740_CODEC_2_INPUT_VOLUME_MASK		0x1f0000
#define JZ4740_CODEC_2_SAMPLE_RATE_MASK			0x000f00
#define JZ4740_CODEC_2_MIC_BOOST_GAIN_MASK		0x000030
#define JZ4740_CODEC_2_HEADPHONE_VOLUME_MASK	0x000003

#define JZ4740_CODEC_2_INPUT_VOLUME_OFFSET		16
#define JZ4740_CODEC_2_SAMPLE_RATE_OFFSET		 8
#define JZ4740_CODEC_2_MIC_BOOST_GAIN_OFFSET	 4
#define JZ4740_CODEC_2_HEADPHONE_VOLUME_OFFSET	 0

<<<<<<< HEAD
static const uint32_t jz4740_codec_regs[] = {
	0x021b2302, 0x00170803,
};

struct jz4740_codec {
	void __iomem *base;
	struct resource *mem;
};

static unsigned int jz4740_codec_read(struct snd_soc_codec *codec,
	unsigned int reg)
{
	struct jz4740_codec *jz4740_codec = snd_soc_codec_get_drvdata(codec);
	return readl(jz4740_codec->base + (reg << 2));
}

static int jz4740_codec_write(struct snd_soc_codec *codec, unsigned int reg,
	unsigned int val)
{
	struct jz4740_codec *jz4740_codec = snd_soc_codec_get_drvdata(codec);
	u32 *cache = codec->reg_cache;

	cache[reg] = val;
	writel(val, jz4740_codec->base + (reg << 2));

	return 0;
}

static const struct snd_kcontrol_new jz4740_codec_controls[] = {
	SOC_SINGLE("Master Playback Volume", JZ4740_REG_CODEC_2,
			JZ4740_CODEC_2_HEADPHONE_VOLUME_OFFSET, 3, 0),
	SOC_SINGLE("Master Capture Volume", JZ4740_REG_CODEC_2,
			JZ4740_CODEC_2_INPUT_VOLUME_OFFSET, 31, 0),
	SOC_SINGLE("Master Playback Switch", JZ4740_REG_CODEC_1,
			JZ4740_CODEC_1_HEADPHONE_DISABLE_OFFSET, 1, 1),
	SOC_SINGLE("Mic Capture Volume", JZ4740_REG_CODEC_2,
			JZ4740_CODEC_2_MIC_BOOST_GAIN_OFFSET, 3, 0),
=======
static const struct reg_default jz4740_codec_reg_defaults[] = {
	{ JZ4740_REG_CODEC_1, 0x021b2302 },
	{ JZ4740_REG_CODEC_2, 0x00170803 },
};

struct jz4740_codec {
	struct regmap *regmap;
};

static const unsigned int jz4740_mic_tlv[] = {
	TLV_DB_RANGE_HEAD(2),
	0, 2, TLV_DB_SCALE_ITEM(0, 600, 0),
	3, 3, TLV_DB_SCALE_ITEM(2000, 0, 0),
};

static const DECLARE_TLV_DB_SCALE(jz4740_out_tlv, 0, 200, 0);
static const DECLARE_TLV_DB_SCALE(jz4740_in_tlv, -3450, 150, 0);

static const struct snd_kcontrol_new jz4740_codec_controls[] = {
	SOC_SINGLE_TLV("Master Playback Volume", JZ4740_REG_CODEC_2,
			JZ4740_CODEC_2_HEADPHONE_VOLUME_OFFSET, 3, 0,
			jz4740_out_tlv),
	SOC_SINGLE_TLV("Master Capture Volume", JZ4740_REG_CODEC_2,
			JZ4740_CODEC_2_INPUT_VOLUME_OFFSET, 31, 0,
			jz4740_in_tlv),
	SOC_SINGLE("Master Playback Switch", JZ4740_REG_CODEC_1,
			JZ4740_CODEC_1_HEADPHONE_DISABLE_OFFSET, 1, 1),
	SOC_SINGLE_TLV("Mic Capture Volume", JZ4740_REG_CODEC_2,
			JZ4740_CODEC_2_MIC_BOOST_GAIN_OFFSET, 3, 0,
			jz4740_mic_tlv),
>>>>>>> refs/remotes/origin/master
};

static const struct snd_kcontrol_new jz4740_codec_output_controls[] = {
	SOC_DAPM_SINGLE("Bypass Switch", JZ4740_REG_CODEC_1,
			JZ4740_CODEC_1_SW1_ENABLE_OFFSET, 1, 0),
	SOC_DAPM_SINGLE("DAC Switch", JZ4740_REG_CODEC_1,
			JZ4740_CODEC_1_SW2_ENABLE_OFFSET, 1, 0),
};

static const struct snd_kcontrol_new jz4740_codec_input_controls[] = {
	SOC_DAPM_SINGLE("Line Capture Switch", JZ4740_REG_CODEC_1,
			JZ4740_CODEC_1_LINE_ENABLE_OFFSET, 1, 0),
	SOC_DAPM_SINGLE("Mic Capture Switch", JZ4740_REG_CODEC_1,
			JZ4740_CODEC_1_MIC_ENABLE_OFFSET, 1, 0),
};

static const struct snd_soc_dapm_widget jz4740_codec_dapm_widgets[] = {
	SND_SOC_DAPM_ADC("ADC", "Capture", JZ4740_REG_CODEC_1,
			JZ4740_CODEC_1_ADC_ENABLE_OFFSET, 0),
	SND_SOC_DAPM_DAC("DAC", "Playback", JZ4740_REG_CODEC_1,
			JZ4740_CODEC_1_DAC_ENABLE_OFFSET, 0),

	SND_SOC_DAPM_MIXER("Output Mixer", JZ4740_REG_CODEC_1,
			JZ4740_CODEC_1_HEADPHONE_POWERDOWN_OFFSET, 1,
			jz4740_codec_output_controls,
			ARRAY_SIZE(jz4740_codec_output_controls)),

	SND_SOC_DAPM_MIXER_NAMED_CTL("Input Mixer", SND_SOC_NOPM, 0, 0,
			jz4740_codec_input_controls,
			ARRAY_SIZE(jz4740_codec_input_controls)),
	SND_SOC_DAPM_MIXER("Line Input", SND_SOC_NOPM, 0, 0, NULL, 0),

	SND_SOC_DAPM_OUTPUT("LOUT"),
	SND_SOC_DAPM_OUTPUT("ROUT"),

	SND_SOC_DAPM_INPUT("MIC"),
	SND_SOC_DAPM_INPUT("LIN"),
	SND_SOC_DAPM_INPUT("RIN"),
};

static const struct snd_soc_dapm_route jz4740_codec_dapm_routes[] = {
	{"Line Input", NULL, "LIN"},
	{"Line Input", NULL, "RIN"},

	{"Input Mixer", "Line Capture Switch", "Line Input"},
	{"Input Mixer", "Mic Capture Switch", "MIC"},

	{"ADC", NULL, "Input Mixer"},

	{"Output Mixer", "Bypass Switch", "Input Mixer"},
	{"Output Mixer", "DAC Switch", "DAC"},

	{"LOUT", NULL, "Output Mixer"},
	{"ROUT", NULL, "Output Mixer"},
};

static int jz4740_codec_hw_params(struct snd_pcm_substream *substream,
	struct snd_pcm_hw_params *params, struct snd_soc_dai *dai)
{
<<<<<<< HEAD
	uint32_t val;
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_codec *codec =rtd->codec;
=======
	struct jz4740_codec *jz4740_codec = snd_soc_codec_get_drvdata(dai->codec);
	uint32_t val;
>>>>>>> refs/remotes/origin/master

	switch (params_rate(params)) {
	case 8000:
		val = 0;
		break;
	case 11025:
		val = 1;
		break;
	case 12000:
		val = 2;
		break;
	case 16000:
		val = 3;
		break;
	case 22050:
		val = 4;
		break;
	case 24000:
		val = 5;
		break;
	case 32000:
		val = 6;
		break;
	case 44100:
		val = 7;
		break;
	case 48000:
		val = 8;
		break;
	default:
		return -EINVAL;
	}

	val <<= JZ4740_CODEC_2_SAMPLE_RATE_OFFSET;

<<<<<<< HEAD
	snd_soc_update_bits(codec, JZ4740_REG_CODEC_2,
=======
	regmap_update_bits(jz4740_codec->regmap, JZ4740_REG_CODEC_2,
>>>>>>> refs/remotes/origin/master
				JZ4740_CODEC_2_SAMPLE_RATE_MASK, val);

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct snd_soc_dai_ops jz4740_codec_dai_ops = {
=======
static const struct snd_soc_dai_ops jz4740_codec_dai_ops = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static const struct snd_soc_dai_ops jz4740_codec_dai_ops = {
>>>>>>> refs/remotes/origin/master
	.hw_params = jz4740_codec_hw_params,
};

static struct snd_soc_dai_driver jz4740_codec_dai = {
	.name = "jz4740-hifi",
	.playback = {
		.stream_name = "Playback",
		.channels_min = 2,
		.channels_max = 2,
		.rates = SNDRV_PCM_RATE_8000_48000,
		.formats = SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S8,
	},
	.capture = {
		.stream_name = "Capture",
		.channels_min = 2,
		.channels_max = 2,
		.rates = SNDRV_PCM_RATE_8000_48000,
		.formats = SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S8,
	},
	.ops = &jz4740_codec_dai_ops,
	.symmetric_rates = 1,
};

<<<<<<< HEAD
static void jz4740_codec_wakeup(struct snd_soc_codec *codec)
{
	int i;
	uint32_t *cache = codec->reg_cache;

	snd_soc_update_bits(codec, JZ4740_REG_CODEC_1,
		JZ4740_CODEC_1_RESET, JZ4740_CODEC_1_RESET);
	udelay(2);

	snd_soc_update_bits(codec, JZ4740_REG_CODEC_1,
		JZ4740_CODEC_1_SUSPEND | JZ4740_CODEC_1_RESET, 0);

	for (i = 0; i < ARRAY_SIZE(jz4740_codec_regs); ++i)
		jz4740_codec_write(codec, i, cache[i]);
=======
static void jz4740_codec_wakeup(struct regmap *regmap)
{
	regmap_update_bits(regmap, JZ4740_REG_CODEC_1,
		JZ4740_CODEC_1_RESET, JZ4740_CODEC_1_RESET);
	udelay(2);

	regmap_update_bits(regmap, JZ4740_REG_CODEC_1,
		JZ4740_CODEC_1_SUSPEND | JZ4740_CODEC_1_RESET, 0);

	regcache_sync(regmap);
>>>>>>> refs/remotes/origin/master
}

static int jz4740_codec_set_bias_level(struct snd_soc_codec *codec,
	enum snd_soc_bias_level level)
{
<<<<<<< HEAD
=======
	struct jz4740_codec *jz4740_codec = snd_soc_codec_get_drvdata(codec);
	struct regmap *regmap = jz4740_codec->regmap;
>>>>>>> refs/remotes/origin/master
	unsigned int mask;
	unsigned int value;

	switch (level) {
	case SND_SOC_BIAS_ON:
		break;
	case SND_SOC_BIAS_PREPARE:
		mask = JZ4740_CODEC_1_VREF_DISABLE |
				JZ4740_CODEC_1_VREF_AMP_DISABLE |
				JZ4740_CODEC_1_HEADPHONE_POWERDOWN_M;
		value = 0;

<<<<<<< HEAD
		snd_soc_update_bits(codec, JZ4740_REG_CODEC_1, mask, value);
=======
		regmap_update_bits(regmap, JZ4740_REG_CODEC_1, mask, value);
>>>>>>> refs/remotes/origin/master
		break;
	case SND_SOC_BIAS_STANDBY:
		/* The only way to clear the suspend flag is to reset the codec */
		if (codec->dapm.bias_level == SND_SOC_BIAS_OFF)
<<<<<<< HEAD
			jz4740_codec_wakeup(codec);
=======
			jz4740_codec_wakeup(regmap);
>>>>>>> refs/remotes/origin/master

		mask = JZ4740_CODEC_1_VREF_DISABLE |
			JZ4740_CODEC_1_VREF_AMP_DISABLE |
			JZ4740_CODEC_1_HEADPHONE_POWERDOWN_M;
		value = JZ4740_CODEC_1_VREF_DISABLE |
			JZ4740_CODEC_1_VREF_AMP_DISABLE |
			JZ4740_CODEC_1_HEADPHONE_POWERDOWN_M;

<<<<<<< HEAD
		snd_soc_update_bits(codec, JZ4740_REG_CODEC_1, mask, value);
=======
		regmap_update_bits(regmap, JZ4740_REG_CODEC_1, mask, value);
>>>>>>> refs/remotes/origin/master
		break;
	case SND_SOC_BIAS_OFF:
		mask = JZ4740_CODEC_1_SUSPEND;
		value = JZ4740_CODEC_1_SUSPEND;

<<<<<<< HEAD
		snd_soc_update_bits(codec, JZ4740_REG_CODEC_1, mask, value);
=======
		regmap_update_bits(regmap, JZ4740_REG_CODEC_1, mask, value);
		regcache_mark_dirty(regmap);
>>>>>>> refs/remotes/origin/master
		break;
	default:
		break;
	}

	codec->dapm.bias_level = level;

	return 0;
}

static int jz4740_codec_dev_probe(struct snd_soc_codec *codec)
{
<<<<<<< HEAD
	snd_soc_update_bits(codec, JZ4740_REG_CODEC_1,
=======
	struct jz4740_codec *jz4740_codec = snd_soc_codec_get_drvdata(codec);

	regmap_update_bits(jz4740_codec->regmap, JZ4740_REG_CODEC_1,
>>>>>>> refs/remotes/origin/master
			JZ4740_CODEC_1_SW2_ENABLE, JZ4740_CODEC_1_SW2_ENABLE);

	jz4740_codec_set_bias_level(codec, SND_SOC_BIAS_STANDBY);

	return 0;
}

static int jz4740_codec_dev_remove(struct snd_soc_codec *codec)
{
	jz4740_codec_set_bias_level(codec, SND_SOC_BIAS_OFF);

	return 0;
}

#ifdef CONFIG_PM_SLEEP

<<<<<<< HEAD
<<<<<<< HEAD
static int jz4740_codec_suspend(struct snd_soc_codec *codec, pm_message_t state)
=======
static int jz4740_codec_suspend(struct snd_soc_codec *codec)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int jz4740_codec_suspend(struct snd_soc_codec *codec)
>>>>>>> refs/remotes/origin/master
{
	return jz4740_codec_set_bias_level(codec, SND_SOC_BIAS_OFF);
}

static int jz4740_codec_resume(struct snd_soc_codec *codec)
{
	return jz4740_codec_set_bias_level(codec, SND_SOC_BIAS_STANDBY);
}

#else
#define jz4740_codec_suspend NULL
#define jz4740_codec_resume NULL
#endif

static struct snd_soc_codec_driver soc_codec_dev_jz4740_codec = {
	.probe = jz4740_codec_dev_probe,
	.remove = jz4740_codec_dev_remove,
	.suspend = jz4740_codec_suspend,
	.resume = jz4740_codec_resume,
<<<<<<< HEAD
	.read = jz4740_codec_read,
	.write = jz4740_codec_write,
	.set_bias_level = jz4740_codec_set_bias_level,
	.reg_cache_default	= jz4740_codec_regs,
	.reg_word_size = sizeof(u32),
	.reg_cache_size	= 2,
=======
	.set_bias_level = jz4740_codec_set_bias_level,
>>>>>>> refs/remotes/origin/master

	.controls = jz4740_codec_controls,
	.num_controls = ARRAY_SIZE(jz4740_codec_controls),
	.dapm_widgets = jz4740_codec_dapm_widgets,
	.num_dapm_widgets = ARRAY_SIZE(jz4740_codec_dapm_widgets),
	.dapm_routes = jz4740_codec_dapm_routes,
	.num_dapm_routes = ARRAY_SIZE(jz4740_codec_dapm_routes),
};

<<<<<<< HEAD
static int __devinit jz4740_codec_probe(struct platform_device *pdev)
=======
static const struct regmap_config jz4740_codec_regmap_config = {
	.reg_bits = 32,
	.reg_stride = 4,
	.val_bits = 32,
	.max_register = JZ4740_REG_CODEC_2,

	.reg_defaults = jz4740_codec_reg_defaults,
	.num_reg_defaults = ARRAY_SIZE(jz4740_codec_reg_defaults),
	.cache_type = REGCACHE_RBTREE,
};

static int jz4740_codec_probe(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	int ret;
	struct jz4740_codec *jz4740_codec;
	struct resource *mem;
<<<<<<< HEAD

<<<<<<< HEAD
	jz4740_codec = kzalloc(sizeof(*jz4740_codec), GFP_KERNEL);
=======
	jz4740_codec = devm_kzalloc(&pdev->dev, sizeof(*jz4740_codec),
				    GFP_KERNEL);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	void __iomem *base;

	jz4740_codec = devm_kzalloc(&pdev->dev, sizeof(*jz4740_codec),
				    GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (!jz4740_codec)
		return -ENOMEM;

	mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
<<<<<<< HEAD
	if (!mem) {
		dev_err(&pdev->dev, "Failed to get mmio memory resource\n");
		ret = -ENOENT;
<<<<<<< HEAD
		goto err_free_codec;
=======
		goto err_out;
>>>>>>> refs/remotes/origin/cm-10.0
	}

	mem = request_mem_region(mem->start, resource_size(mem), pdev->name);
	if (!mem) {
		dev_err(&pdev->dev, "Failed to request mmio memory region\n");
		ret = -EBUSY;
<<<<<<< HEAD
		goto err_free_codec;
=======
		goto err_out;
>>>>>>> refs/remotes/origin/cm-10.0
	}

	jz4740_codec->base = ioremap(mem->start, resource_size(mem));
	if (!jz4740_codec->base) {
		dev_err(&pdev->dev, "Failed to ioremap mmio memory\n");
		ret = -EBUSY;
		goto err_release_mem_region;
	}
	jz4740_codec->mem = mem;
=======
	base = devm_ioremap_resource(&pdev->dev, mem);
	if (IS_ERR(base))
		return PTR_ERR(base);

	jz4740_codec->regmap = devm_regmap_init_mmio(&pdev->dev, base,
					    &jz4740_codec_regmap_config);
	if (IS_ERR(jz4740_codec->regmap))
		return PTR_ERR(jz4740_codec->regmap);
>>>>>>> refs/remotes/origin/master

	platform_set_drvdata(pdev, jz4740_codec);

	ret = snd_soc_register_codec(&pdev->dev,
			&soc_codec_dev_jz4740_codec, &jz4740_codec_dai, 1);
<<<<<<< HEAD
	if (ret) {
		dev_err(&pdev->dev, "Failed to register codec\n");
		goto err_iounmap;
	}

	return 0;

err_iounmap:
	iounmap(jz4740_codec->base);
err_release_mem_region:
	release_mem_region(mem->start, resource_size(mem));
<<<<<<< HEAD
err_free_codec:
	kfree(jz4740_codec);

=======
err_out:
>>>>>>> refs/remotes/origin/cm-10.0
	return ret;
}

static int __devexit jz4740_codec_remove(struct platform_device *pdev)
{
	struct jz4740_codec *jz4740_codec = platform_get_drvdata(pdev);
	struct resource *mem = jz4740_codec->mem;

	snd_soc_unregister_codec(&pdev->dev);

	iounmap(jz4740_codec->base);
	release_mem_region(mem->start, resource_size(mem));

	platform_set_drvdata(pdev, NULL);
<<<<<<< HEAD
	kfree(jz4740_codec);
=======
>>>>>>> refs/remotes/origin/cm-10.0

=======
	if (ret)
		dev_err(&pdev->dev, "Failed to register codec\n");

	return ret;
}

static int jz4740_codec_remove(struct platform_device *pdev)
{
	snd_soc_unregister_codec(&pdev->dev);

>>>>>>> refs/remotes/origin/master
	return 0;
}

static struct platform_driver jz4740_codec_driver = {
	.probe = jz4740_codec_probe,
<<<<<<< HEAD
	.remove = __devexit_p(jz4740_codec_remove),
=======
	.remove = jz4740_codec_remove,
>>>>>>> refs/remotes/origin/master
	.driver = {
		.name = "jz4740-codec",
		.owner = THIS_MODULE,
	},
};

<<<<<<< HEAD
<<<<<<< HEAD
static int __init jz4740_codec_init(void)
{
	return platform_driver_register(&jz4740_codec_driver);
}
module_init(jz4740_codec_init);

static void __exit jz4740_codec_exit(void)
{
	platform_driver_unregister(&jz4740_codec_driver);
}
module_exit(jz4740_codec_exit);
=======
module_platform_driver(jz4740_codec_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
module_platform_driver(jz4740_codec_driver);
>>>>>>> refs/remotes/origin/master

MODULE_DESCRIPTION("JZ4740 SoC internal codec driver");
MODULE_AUTHOR("Lars-Peter Clausen <lars@metafoo.de>");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:jz4740-codec");
