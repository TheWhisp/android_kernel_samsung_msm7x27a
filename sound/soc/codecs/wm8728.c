/*
 * wm8728.c  --  WM8728 ALSA SoC Audio driver
 *
 * Copyright 2008 Wolfson Microelectronics plc
 *
 * Author: Mark Brown <broonie@opensource.wolfsonmicro.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/pm.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
<<<<<<< HEAD
#include <linux/spi/spi.h>
#include <linux/slab.h>
<<<<<<< HEAD
=======
#include <linux/of_device.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/regmap.h>
#include <linux/spi/spi.h>
#include <linux/slab.h>
#include <linux/of_device.h>
>>>>>>> refs/remotes/origin/master
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/initval.h>
#include <sound/tlv.h>

#include "wm8728.h"

/*
 * We can't read the WM8728 register space so we cache them instead.
 * Note that the defaults here aren't the physical defaults, we latch
 * the volume update bits, mute the output and enable infinite zero
 * detect.
 */
<<<<<<< HEAD
static const u16 wm8728_reg_defaults[] = {
	0x1ff,
	0x1ff,
	0x001,
	0x100,
=======
static const struct reg_default wm8728_reg_defaults[] = {
	{ 0, 0x1ff },
	{ 1, 0x1ff },
	{ 2, 0x001 },
	{ 3, 0x100 },
>>>>>>> refs/remotes/origin/master
};

/* codec private data */
struct wm8728_priv {
<<<<<<< HEAD
	enum snd_soc_control_type control_type;
=======
	struct regmap *regmap;
>>>>>>> refs/remotes/origin/master
};

static const DECLARE_TLV_DB_SCALE(wm8728_tlv, -12750, 50, 1);

static const struct snd_kcontrol_new wm8728_snd_controls[] = {

SOC_DOUBLE_R_TLV("Digital Playback Volume", WM8728_DACLVOL, WM8728_DACRVOL,
		 0, 255, 0, wm8728_tlv),

SOC_SINGLE("Deemphasis", WM8728_DACCTL, 1, 1, 0),
};

/*
 * DAPM controls.
 */
static const struct snd_soc_dapm_widget wm8728_dapm_widgets[] = {
SND_SOC_DAPM_DAC("DAC", "HiFi Playback", SND_SOC_NOPM, 0, 0),
SND_SOC_DAPM_OUTPUT("VOUTL"),
SND_SOC_DAPM_OUTPUT("VOUTR"),
};

static const struct snd_soc_dapm_route wm8728_intercon[] = {
	{"VOUTL", NULL, "DAC"},
	{"VOUTR", NULL, "DAC"},
};

static int wm8728_mute(struct snd_soc_dai *dai, int mute)
{
	struct snd_soc_codec *codec = dai->codec;
	u16 mute_reg = snd_soc_read(codec, WM8728_DACCTL);

	if (mute)
		snd_soc_write(codec, WM8728_DACCTL, mute_reg | 1);
	else
		snd_soc_write(codec, WM8728_DACCTL, mute_reg & ~1);

	return 0;
}

static int wm8728_hw_params(struct snd_pcm_substream *substream,
	struct snd_pcm_hw_params *params,
	struct snd_soc_dai *dai)
{
<<<<<<< HEAD
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_codec *codec = rtd->codec;
=======
	struct snd_soc_codec *codec = dai->codec;
>>>>>>> refs/remotes/origin/master
	u16 dac = snd_soc_read(codec, WM8728_DACCTL);

	dac &= ~0x18;

	switch (params_format(params)) {
	case SNDRV_PCM_FORMAT_S16_LE:
		break;
	case SNDRV_PCM_FORMAT_S20_3LE:
		dac |= 0x10;
		break;
	case SNDRV_PCM_FORMAT_S24_LE:
		dac |= 0x08;
		break;
	default:
		return -EINVAL;
	}

	snd_soc_write(codec, WM8728_DACCTL, dac);

	return 0;
}

static int wm8728_set_dai_fmt(struct snd_soc_dai *codec_dai,
		unsigned int fmt)
{
	struct snd_soc_codec *codec = codec_dai->codec;
	u16 iface = snd_soc_read(codec, WM8728_IFCTL);

	/* Currently only I2S is supported by the driver, though the
	 * hardware is more flexible.
	 */
	switch (fmt & SND_SOC_DAIFMT_FORMAT_MASK) {
	case SND_SOC_DAIFMT_I2S:
		iface |= 1;
		break;
	default:
		return -EINVAL;
	}

	/* The hardware only support full slave mode */
	switch (fmt & SND_SOC_DAIFMT_MASTER_MASK) {
	case SND_SOC_DAIFMT_CBS_CFS:
		break;
	default:
		return -EINVAL;
	}

	switch (fmt & SND_SOC_DAIFMT_INV_MASK) {
	case SND_SOC_DAIFMT_NB_NF:
		iface &= ~0x22;
		break;
	case SND_SOC_DAIFMT_IB_NF:
		iface |=  0x20;
		iface &= ~0x02;
		break;
	case SND_SOC_DAIFMT_NB_IF:
		iface |= 0x02;
		iface &= ~0x20;
		break;
	case SND_SOC_DAIFMT_IB_IF:
		iface |= 0x22;
		break;
	default:
		return -EINVAL;
	}

	snd_soc_write(codec, WM8728_IFCTL, iface);
	return 0;
}

static int wm8728_set_bias_level(struct snd_soc_codec *codec,
				 enum snd_soc_bias_level level)
{
<<<<<<< HEAD
	u16 reg;
	int i;
=======
	struct wm8728_priv *wm8728 = snd_soc_codec_get_drvdata(codec);
	u16 reg;
>>>>>>> refs/remotes/origin/master

	switch (level) {
	case SND_SOC_BIAS_ON:
	case SND_SOC_BIAS_PREPARE:
	case SND_SOC_BIAS_STANDBY:
		if (codec->dapm.bias_level == SND_SOC_BIAS_OFF) {
			/* Power everything up... */
			reg = snd_soc_read(codec, WM8728_DACCTL);
			snd_soc_write(codec, WM8728_DACCTL, reg & ~0x4);

			/* ..then sync in the register cache. */
<<<<<<< HEAD
			for (i = 0; i < ARRAY_SIZE(wm8728_reg_defaults); i++)
				snd_soc_write(codec, i,
					     snd_soc_read(codec, i));
=======
			regcache_sync(wm8728->regmap);
>>>>>>> refs/remotes/origin/master
		}
		break;

	case SND_SOC_BIAS_OFF:
		reg = snd_soc_read(codec, WM8728_DACCTL);
		snd_soc_write(codec, WM8728_DACCTL, reg | 0x4);
		break;
	}
	codec->dapm.bias_level = level;
	return 0;
}

#define WM8728_RATES (SNDRV_PCM_RATE_8000_192000)

#define WM8728_FORMATS (SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S20_3LE |\
	SNDRV_PCM_FMTBIT_S24_LE)

<<<<<<< HEAD
<<<<<<< HEAD
static struct snd_soc_dai_ops wm8728_dai_ops = {
=======
static const struct snd_soc_dai_ops wm8728_dai_ops = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static const struct snd_soc_dai_ops wm8728_dai_ops = {
>>>>>>> refs/remotes/origin/master
	.hw_params	= wm8728_hw_params,
	.digital_mute	= wm8728_mute,
	.set_fmt	= wm8728_set_dai_fmt,
};

static struct snd_soc_dai_driver wm8728_dai = {
	.name = "wm8728-hifi",
	.playback = {
		.stream_name = "Playback",
		.channels_min = 2,
		.channels_max = 2,
		.rates = WM8728_RATES,
		.formats = WM8728_FORMATS,
	},
	.ops = &wm8728_dai_ops,
};

<<<<<<< HEAD
<<<<<<< HEAD
static int wm8728_suspend(struct snd_soc_codec *codec, pm_message_t state)
=======
static int wm8728_suspend(struct snd_soc_codec *codec)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int wm8728_suspend(struct snd_soc_codec *codec)
>>>>>>> refs/remotes/origin/master
{
	wm8728_set_bias_level(codec, SND_SOC_BIAS_OFF);

	return 0;
}

static int wm8728_resume(struct snd_soc_codec *codec)
{
	wm8728_set_bias_level(codec, SND_SOC_BIAS_STANDBY);

	return 0;
}

static int wm8728_probe(struct snd_soc_codec *codec)
{
<<<<<<< HEAD
	struct wm8728_priv *wm8728 = snd_soc_codec_get_drvdata(codec);
	int ret;

	ret = snd_soc_codec_set_cache_io(codec, 7, 9, wm8728->control_type);
=======
	int ret;

	ret = snd_soc_codec_set_cache_io(codec, 7, 9, SND_SOC_REGMAP);
>>>>>>> refs/remotes/origin/master
	if (ret < 0) {
		printk(KERN_ERR "wm8728: failed to configure cache I/O: %d\n",
		       ret);
		return ret;
	}

	/* power on device */
	wm8728_set_bias_level(codec, SND_SOC_BIAS_STANDBY);

<<<<<<< HEAD
<<<<<<< HEAD
	snd_soc_add_controls(codec, wm8728_snd_controls,
				ARRAY_SIZE(wm8728_snd_controls));

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return ret;
}

static int wm8728_remove(struct snd_soc_codec *codec)
{
	wm8728_set_bias_level(codec, SND_SOC_BIAS_OFF);
	return 0;
}

static struct snd_soc_codec_driver soc_codec_dev_wm8728 = {
	.probe =	wm8728_probe,
	.remove =	wm8728_remove,
	.suspend =	wm8728_suspend,
	.resume =	wm8728_resume,
	.set_bias_level = wm8728_set_bias_level,
<<<<<<< HEAD
	.reg_cache_size = ARRAY_SIZE(wm8728_reg_defaults),
	.reg_word_size = sizeof(u16),
	.reg_cache_default = wm8728_reg_defaults,
<<<<<<< HEAD
=======
	.controls = wm8728_snd_controls,
	.num_controls = ARRAY_SIZE(wm8728_snd_controls),
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.controls = wm8728_snd_controls,
	.num_controls = ARRAY_SIZE(wm8728_snd_controls),
>>>>>>> refs/remotes/origin/master
	.dapm_widgets = wm8728_dapm_widgets,
	.num_dapm_widgets = ARRAY_SIZE(wm8728_dapm_widgets),
	.dapm_routes = wm8728_intercon,
	.num_dapm_routes = ARRAY_SIZE(wm8728_intercon),
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static const struct of_device_id wm8728_of_match[] = {
	{ .compatible = "wlf,wm8728", },
	{ }
};
MODULE_DEVICE_TABLE(of, wm8728_of_match);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
#if defined(CONFIG_SPI_MASTER)
static int __devinit wm8728_spi_probe(struct spi_device *spi)
=======
static const struct regmap_config wm8728_regmap = {
	.reg_bits = 7,
	.val_bits = 9,
	.max_register = WM8728_IFCTL,

	.reg_defaults = wm8728_reg_defaults,
	.num_reg_defaults = ARRAY_SIZE(wm8728_reg_defaults),
	.cache_type = REGCACHE_RBTREE,
};

#if defined(CONFIG_SPI_MASTER)
static int wm8728_spi_probe(struct spi_device *spi)
>>>>>>> refs/remotes/origin/master
{
	struct wm8728_priv *wm8728;
	int ret;

<<<<<<< HEAD
	wm8728 = kzalloc(sizeof(struct wm8728_priv), GFP_KERNEL);
	if (wm8728 == NULL)
		return -ENOMEM;

	wm8728->control_type = SND_SOC_SPI;
=======
	wm8728 = devm_kzalloc(&spi->dev, sizeof(struct wm8728_priv),
			      GFP_KERNEL);
	if (wm8728 == NULL)
		return -ENOMEM;

	wm8728->regmap = devm_regmap_init_spi(spi, &wm8728_regmap);
	if (IS_ERR(wm8728->regmap))
		return PTR_ERR(wm8728->regmap);

>>>>>>> refs/remotes/origin/master
	spi_set_drvdata(spi, wm8728);

	ret = snd_soc_register_codec(&spi->dev,
			&soc_codec_dev_wm8728, &wm8728_dai, 1);
<<<<<<< HEAD
	if (ret < 0)
		kfree(wm8728);
	return ret;
}

static int __devexit wm8728_spi_remove(struct spi_device *spi)
{
	snd_soc_unregister_codec(&spi->dev);
	kfree(spi_get_drvdata(spi));
=======

	return ret;
}

static int wm8728_spi_remove(struct spi_device *spi)
{
	snd_soc_unregister_codec(&spi->dev);

>>>>>>> refs/remotes/origin/master
	return 0;
}

static struct spi_driver wm8728_spi_driver = {
	.driver = {
<<<<<<< HEAD
<<<<<<< HEAD
		.name	= "wm8728-codec",
		.owner	= THIS_MODULE,
=======
		.name	= "wm8728",
		.owner	= THIS_MODULE,
		.of_match_table = wm8728_of_match,
>>>>>>> refs/remotes/origin/cm-10.0
	},
	.probe		= wm8728_spi_probe,
	.remove		= __devexit_p(wm8728_spi_remove),
};
#endif /* CONFIG_SPI_MASTER */

#if defined(CONFIG_I2C) || defined(CONFIG_I2C_MODULE)
static __devinit int wm8728_i2c_probe(struct i2c_client *i2c,
				      const struct i2c_device_id *id)
=======
		.name	= "wm8728",
		.owner	= THIS_MODULE,
		.of_match_table = wm8728_of_match,
	},
	.probe		= wm8728_spi_probe,
	.remove		= wm8728_spi_remove,
};
#endif /* CONFIG_SPI_MASTER */

#if IS_ENABLED(CONFIG_I2C)
static int wm8728_i2c_probe(struct i2c_client *i2c,
			    const struct i2c_device_id *id)
>>>>>>> refs/remotes/origin/master
{
	struct wm8728_priv *wm8728;
	int ret;

<<<<<<< HEAD
	wm8728 = kzalloc(sizeof(struct wm8728_priv), GFP_KERNEL);
	if (wm8728 == NULL)
		return -ENOMEM;

	i2c_set_clientdata(i2c, wm8728);
	wm8728->control_type = SND_SOC_I2C;

	ret =  snd_soc_register_codec(&i2c->dev,
			&soc_codec_dev_wm8728, &wm8728_dai, 1);
	if (ret < 0)
		kfree(wm8728);
	return ret;
}

static __devexit int wm8728_i2c_remove(struct i2c_client *client)
{
	snd_soc_unregister_codec(&client->dev);
	kfree(i2c_get_clientdata(client));
=======
	wm8728 = devm_kzalloc(&i2c->dev, sizeof(struct wm8728_priv),
			      GFP_KERNEL);
	if (wm8728 == NULL)
		return -ENOMEM;

	wm8728->regmap = devm_regmap_init_i2c(i2c, &wm8728_regmap);
	if (IS_ERR(wm8728->regmap))
		return PTR_ERR(wm8728->regmap);

	i2c_set_clientdata(i2c, wm8728);

	ret =  snd_soc_register_codec(&i2c->dev,
			&soc_codec_dev_wm8728, &wm8728_dai, 1);

	return ret;
}

static int wm8728_i2c_remove(struct i2c_client *client)
{
	snd_soc_unregister_codec(&client->dev);
>>>>>>> refs/remotes/origin/master
	return 0;
}

static const struct i2c_device_id wm8728_i2c_id[] = {
	{ "wm8728", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, wm8728_i2c_id);

static struct i2c_driver wm8728_i2c_driver = {
	.driver = {
<<<<<<< HEAD
<<<<<<< HEAD
		.name = "wm8728-codec",
		.owner = THIS_MODULE,
=======
		.name = "wm8728",
		.owner = THIS_MODULE,
		.of_match_table = wm8728_of_match,
>>>>>>> refs/remotes/origin/cm-10.0
	},
	.probe =    wm8728_i2c_probe,
	.remove =   __devexit_p(wm8728_i2c_remove),
=======
		.name = "wm8728",
		.owner = THIS_MODULE,
		.of_match_table = wm8728_of_match,
	},
	.probe =    wm8728_i2c_probe,
	.remove =   wm8728_i2c_remove,
>>>>>>> refs/remotes/origin/master
	.id_table = wm8728_i2c_id,
};
#endif

static int __init wm8728_modinit(void)
{
	int ret = 0;
<<<<<<< HEAD
#if defined(CONFIG_I2C) || defined(CONFIG_I2C_MODULE)
=======
#if IS_ENABLED(CONFIG_I2C)
>>>>>>> refs/remotes/origin/master
	ret = i2c_add_driver(&wm8728_i2c_driver);
	if (ret != 0) {
		printk(KERN_ERR "Failed to register wm8728 I2C driver: %d\n",
		       ret);
	}
#endif
#if defined(CONFIG_SPI_MASTER)
	ret = spi_register_driver(&wm8728_spi_driver);
	if (ret != 0) {
		printk(KERN_ERR "Failed to register wm8728 SPI driver: %d\n",
		       ret);
	}
#endif
	return ret;
}
module_init(wm8728_modinit);

static void __exit wm8728_exit(void)
{
<<<<<<< HEAD
#if defined(CONFIG_I2C) || defined(CONFIG_I2C_MODULE)
=======
#if IS_ENABLED(CONFIG_I2C)
>>>>>>> refs/remotes/origin/master
	i2c_del_driver(&wm8728_i2c_driver);
#endif
#if defined(CONFIG_SPI_MASTER)
	spi_unregister_driver(&wm8728_spi_driver);
#endif
}
module_exit(wm8728_exit);

MODULE_DESCRIPTION("ASoC WM8728 driver");
MODULE_AUTHOR("Mark Brown <broonie@opensource.wolfsonmicro.com>");
MODULE_LICENSE("GPL");
