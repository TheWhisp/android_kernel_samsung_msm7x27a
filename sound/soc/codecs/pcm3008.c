/*
 * ALSA Soc PCM3008 codec support
 *
 * Author:	Hugo Villeneuve
 * Copyright (C) 2008 Lyrtech inc
 *
 * Based on AC97 Soc codec, original copyright follow:
 * Copyright 2005 Wolfson Microelectronics PLC.
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 * Generic PCM3008 support.
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/slab.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/master
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/initval.h>
#include <sound/soc.h>

#include "pcm3008.h"

<<<<<<< HEAD
#define PCM3008_VERSION "0.2"
=======
static int pcm3008_dac_ev(struct snd_soc_dapm_widget *w,
			  struct snd_kcontrol *kcontrol,
			  int event)
{
	struct snd_soc_codec *codec = w->codec;
	struct pcm3008_setup_data *setup = codec->dev->platform_data;

	gpio_set_value_cansleep(setup->pdda_pin,
				SND_SOC_DAPM_EVENT_ON(event));

	return 0;
}

static int pcm3008_adc_ev(struct snd_soc_dapm_widget *w,
			  struct snd_kcontrol *kcontrol,
			  int event)
{
	struct snd_soc_codec *codec = w->codec;
	struct pcm3008_setup_data *setup = codec->dev->platform_data;

	gpio_set_value_cansleep(setup->pdad_pin,
				SND_SOC_DAPM_EVENT_ON(event));

	return 0;
}

static const struct snd_soc_dapm_widget pcm3008_dapm_widgets[] = {
SND_SOC_DAPM_INPUT("VINL"),
SND_SOC_DAPM_INPUT("VINR"),

SND_SOC_DAPM_DAC_E("DAC", NULL, SND_SOC_NOPM, 0, 0, pcm3008_dac_ev,
		   SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD),
SND_SOC_DAPM_ADC_E("ADC", NULL, SND_SOC_NOPM, 0, 0, pcm3008_adc_ev,
		   SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD),

SND_SOC_DAPM_OUTPUT("VOUTL"),
SND_SOC_DAPM_OUTPUT("VOUTR"),
};

static const struct snd_soc_dapm_route pcm3008_dapm_routes[] = {
	{ "PCM3008 Capture", NULL, "ADC" },
	{ "ADC", NULL, "VINL" },
	{ "ADC", NULL, "VINR" },

	{ "DAC", NULL, "PCM3008 Playback" },
	{ "VOUTL", NULL, "DAC" },
	{ "VOUTR", NULL, "DAC" },
};
>>>>>>> refs/remotes/origin/master

#define PCM3008_RATES (SNDRV_PCM_RATE_32000 | SNDRV_PCM_RATE_44100 |	\
		       SNDRV_PCM_RATE_48000)

static struct snd_soc_dai_driver pcm3008_dai = {
	.name = "pcm3008-hifi",
	.playback = {
		.stream_name = "PCM3008 Playback",
		.channels_min = 1,
		.channels_max = 2,
		.rates = PCM3008_RATES,
		.formats = SNDRV_PCM_FMTBIT_S16_LE,
	},
	.capture = {
		.stream_name = "PCM3008 Capture",
		.channels_min = 1,
		.channels_max = 2,
		.rates = PCM3008_RATES,
		.formats = SNDRV_PCM_FMTBIT_S16_LE,
	},
};

<<<<<<< HEAD
static void pcm3008_gpio_free(struct pcm3008_setup_data *setup)
{
	gpio_free(setup->dem0_pin);
	gpio_free(setup->dem1_pin);
	gpio_free(setup->pdad_pin);
	gpio_free(setup->pdda_pin);
}

static int pcm3008_soc_probe(struct snd_soc_codec *codec)
{
	struct pcm3008_setup_data *setup = codec->dev->platform_data;
	int ret = 0;

	printk(KERN_INFO "PCM3008 SoC Audio Codec %s\n", PCM3008_VERSION);
=======
static struct snd_soc_codec_driver soc_codec_dev_pcm3008 = {
	.dapm_widgets = pcm3008_dapm_widgets,
	.num_dapm_widgets = ARRAY_SIZE(pcm3008_dapm_widgets),
	.dapm_routes = pcm3008_dapm_routes,
	.num_dapm_routes = ARRAY_SIZE(pcm3008_dapm_routes),
};

static int pcm3008_codec_probe(struct platform_device *pdev)
{
	struct pcm3008_setup_data *setup = pdev->dev.platform_data;
	int ret;

	if (!setup)
		return -EINVAL;
>>>>>>> refs/remotes/origin/master

	/* DEM1  DEM0  DE-EMPHASIS_MODE
	 * Low   Low   De-emphasis 44.1 kHz ON
	 * Low   High  De-emphasis OFF
	 * High  Low   De-emphasis 48 kHz ON
	 * High  High  De-emphasis 32 kHz ON
	 */

	/* Configure DEM0 GPIO (turning OFF DAC De-emphasis). */
<<<<<<< HEAD
	ret = gpio_request(setup->dem0_pin, "codec_dem0");
	if (ret == 0)
		ret = gpio_direction_output(setup->dem0_pin, 1);
	if (ret != 0)
		goto gpio_err;

	/* Configure DEM1 GPIO (turning OFF DAC De-emphasis). */
	ret = gpio_request(setup->dem1_pin, "codec_dem1");
	if (ret == 0)
		ret = gpio_direction_output(setup->dem1_pin, 0);
	if (ret != 0)
		goto gpio_err;

	/* Configure PDAD GPIO. */
	ret = gpio_request(setup->pdad_pin, "codec_pdad");
	if (ret == 0)
		ret = gpio_direction_output(setup->pdad_pin, 1);
	if (ret != 0)
		goto gpio_err;

	/* Configure PDDA GPIO. */
	ret = gpio_request(setup->pdda_pin, "codec_pdda");
	if (ret == 0)
		ret = gpio_direction_output(setup->pdda_pin, 1);
	if (ret != 0)
		goto gpio_err;

	return ret;

gpio_err:
	pcm3008_gpio_free(setup);

	return ret;
}

static int pcm3008_soc_remove(struct snd_soc_codec *codec)
{
	struct pcm3008_setup_data *setup = codec->dev->platform_data;

	pcm3008_gpio_free(setup);
	return 0;
}

#ifdef CONFIG_PM
<<<<<<< HEAD
static int pcm3008_soc_suspend(struct snd_soc_codec *codec, pm_message_t msg)
=======
static int pcm3008_soc_suspend(struct snd_soc_codec *codec)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct pcm3008_setup_data *setup = codec->dev->platform_data;

	gpio_set_value(setup->pdad_pin, 0);
	gpio_set_value(setup->pdda_pin, 0);

	return 0;
}

static int pcm3008_soc_resume(struct snd_soc_codec *codec)
{
	struct pcm3008_setup_data *setup = codec->dev->platform_data;

	gpio_set_value(setup->pdad_pin, 1);
	gpio_set_value(setup->pdda_pin, 1);

	return 0;
}
#else
#define pcm3008_soc_suspend NULL
#define pcm3008_soc_resume NULL
#endif

static struct snd_soc_codec_driver soc_codec_dev_pcm3008 = {
	.probe = 	pcm3008_soc_probe,
	.remove = 	pcm3008_soc_remove,
	.suspend =	pcm3008_soc_suspend,
	.resume =	pcm3008_soc_resume,
};

static int __devinit pcm3008_codec_probe(struct platform_device *pdev)
{
=======
	ret = devm_gpio_request_one(&pdev->dev, setup->dem0_pin,
				    GPIOF_OUT_INIT_HIGH, "codec_dem0");
	if (ret != 0)
		return ret;

	/* Configure DEM1 GPIO (turning OFF DAC De-emphasis). */
	ret = devm_gpio_request_one(&pdev->dev, setup->dem1_pin,
				    GPIOF_OUT_INIT_LOW, "codec_dem1");
	if (ret != 0)
		return ret;

	/* Configure PDAD GPIO. */
	ret = devm_gpio_request_one(&pdev->dev, setup->pdad_pin,
				    GPIOF_OUT_INIT_LOW, "codec_pdad");
	if (ret != 0)
		return ret;

	/* Configure PDDA GPIO. */
	ret = devm_gpio_request_one(&pdev->dev, setup->pdda_pin,
				    GPIOF_OUT_INIT_LOW, "codec_pdda");
	if (ret != 0)
		return ret;

>>>>>>> refs/remotes/origin/master
	return snd_soc_register_codec(&pdev->dev,
			&soc_codec_dev_pcm3008, &pcm3008_dai, 1);
}

<<<<<<< HEAD
static int __devexit pcm3008_codec_remove(struct platform_device *pdev)
{
	snd_soc_unregister_codec(&pdev->dev);
=======
static int pcm3008_codec_remove(struct platform_device *pdev)
{
	snd_soc_unregister_codec(&pdev->dev);

>>>>>>> refs/remotes/origin/master
	return 0;
}

MODULE_ALIAS("platform:pcm3008-codec");

static struct platform_driver pcm3008_codec_driver = {
	.probe		= pcm3008_codec_probe,
<<<<<<< HEAD
	.remove		= __devexit_p(pcm3008_codec_remove),
=======
	.remove		= pcm3008_codec_remove,
>>>>>>> refs/remotes/origin/master
	.driver		= {
		.name	= "pcm3008-codec",
		.owner	= THIS_MODULE,
	},
};

<<<<<<< HEAD
<<<<<<< HEAD
static int __init pcm3008_modinit(void)
{
	return platform_driver_register(&pcm3008_codec_driver);
}
module_init(pcm3008_modinit);

static void __exit pcm3008_exit(void)
{
	platform_driver_unregister(&pcm3008_codec_driver);
}
module_exit(pcm3008_exit);
=======
module_platform_driver(pcm3008_codec_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
module_platform_driver(pcm3008_codec_driver);
>>>>>>> refs/remotes/origin/master

MODULE_DESCRIPTION("Soc PCM3008 driver");
MODULE_AUTHOR("Hugo Villeneuve");
MODULE_LICENSE("GPL");
