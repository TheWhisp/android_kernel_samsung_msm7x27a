/*
 * ads117x.c  --  Driver for ads1174/8 ADC chips
 *
 * Copyright 2009 ShotSpotter Inc.
 * Author: Graeme Gregory <gg@slimlogic.co.uk>
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 */

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/device.h>
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

#define ADS117X_RATES (SNDRV_PCM_RATE_8000_48000)
#define ADS117X_FORMATS (SNDRV_PCM_FMTBIT_S16_LE)

<<<<<<< HEAD
=======
static const struct snd_soc_dapm_widget ads117x_dapm_widgets[] = {
SND_SOC_DAPM_INPUT("Input1"),
SND_SOC_DAPM_INPUT("Input2"),
SND_SOC_DAPM_INPUT("Input3"),
SND_SOC_DAPM_INPUT("Input4"),
SND_SOC_DAPM_INPUT("Input5"),
SND_SOC_DAPM_INPUT("Input6"),
SND_SOC_DAPM_INPUT("Input7"),
SND_SOC_DAPM_INPUT("Input8"),
};

static const struct snd_soc_dapm_route ads117x_dapm_routes[] = {
	{ "Capture", NULL, "Input1" },
	{ "Capture", NULL, "Input2" },
	{ "Capture", NULL, "Input3" },
	{ "Capture", NULL, "Input4" },
	{ "Capture", NULL, "Input5" },
	{ "Capture", NULL, "Input6" },
	{ "Capture", NULL, "Input7" },
	{ "Capture", NULL, "Input8" },
};

>>>>>>> refs/remotes/origin/master
static struct snd_soc_dai_driver ads117x_dai = {
/* ADC */
	.name = "ads117x-hifi",
	.capture = {
		.stream_name = "Capture",
		.channels_min = 1,
		.channels_max = 32,
		.rates = ADS117X_RATES,
		.formats = ADS117X_FORMATS,},
};

<<<<<<< HEAD
static struct snd_soc_codec_driver soc_codec_dev_ads117x;

static __devinit int ads117x_probe(struct platform_device *pdev)
=======
static struct snd_soc_codec_driver soc_codec_dev_ads117x = {
	.dapm_widgets = ads117x_dapm_widgets,
	.num_dapm_widgets = ARRAY_SIZE(ads117x_dapm_widgets),
	.dapm_routes = ads117x_dapm_routes,
	.num_dapm_routes = ARRAY_SIZE(ads117x_dapm_routes),
};

static int ads117x_probe(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	return snd_soc_register_codec(&pdev->dev,
			&soc_codec_dev_ads117x, &ads117x_dai, 1);
}

<<<<<<< HEAD
static int __devexit ads117x_remove(struct platform_device *pdev)
=======
static int ads117x_remove(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	snd_soc_unregister_codec(&pdev->dev);
	return 0;
}

static struct platform_driver ads117x_codec_driver = {
	.driver = {
			.name = "ads117x-codec",
			.owner = THIS_MODULE,
	},

	.probe = ads117x_probe,
<<<<<<< HEAD
	.remove = __devexit_p(ads117x_remove),
};

<<<<<<< HEAD
static int __init ads117x_init(void)
{
	return platform_driver_register(&ads117x_codec_driver);
}
module_init(ads117x_init);

static void __exit ads117x_exit(void)
{
	platform_driver_unregister(&ads117x_codec_driver);
}
module_exit(ads117x_exit);
=======
module_platform_driver(ads117x_codec_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.remove = ads117x_remove,
};

module_platform_driver(ads117x_codec_driver);
>>>>>>> refs/remotes/origin/master

MODULE_DESCRIPTION("ASoC ads117x driver");
MODULE_AUTHOR("Graeme Gregory");
MODULE_LICENSE("GPL");
