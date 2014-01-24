
#include <linux/module.h>
#include <sound/soc.h>

#include <asm/mach-types.h>

#include "../codecs/wm8940.h"
#include "pxa2xx-i2s.h"

static int imote2_asoc_hw_params(struct snd_pcm_substream *substream,
				 struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	unsigned int clk = 0;
	int ret;

	switch (params_rate(params)) {
	case 8000:
	case 16000:
	case 48000:
	case 96000:
		clk = 12288000;
		break;
	case 11025:
	case 22050:
	case 44100:
		clk = 11289600;
		break;
	}

<<<<<<< HEAD
	/* set codec DAI configuration */
	ret = snd_soc_dai_set_fmt(codec_dai, SND_SOC_DAIFMT_I2S
				  | SND_SOC_DAIFMT_NB_NF
				  | SND_SOC_DAIFMT_CBS_CFS);
	if (ret < 0)
		return ret;

	/* CPU should be clock master */
	ret = snd_soc_dai_set_fmt(cpu_dai,  SND_SOC_DAIFMT_I2S
				  | SND_SOC_DAIFMT_NB_NF
				  | SND_SOC_DAIFMT_CBS_CFS);
	if (ret < 0)
		return ret;

=======
>>>>>>> refs/remotes/origin/cm-10.0
	ret = snd_soc_dai_set_sysclk(codec_dai, 0, clk,
				     SND_SOC_CLOCK_IN);
	if (ret < 0)
		return ret;

	/* set the I2S system clock as input (unused) */
	ret = snd_soc_dai_set_sysclk(cpu_dai, PXA2XX_I2S_SYSCLK, clk,
		SND_SOC_CLOCK_OUT);

	return ret;
}

static struct snd_soc_ops imote2_asoc_ops = {
	.hw_params = imote2_asoc_hw_params,
};

static struct snd_soc_dai_link imote2_dai = {
	.name = "WM8940",
	.stream_name = "WM8940",
	.cpu_dai_name = "pxa2xx-i2s",
	.codec_dai_name = "wm8940-hifi",
	.platform_name = "pxa-pcm-audio",
	.codec_name = "wm8940-codec.0-0034",
<<<<<<< HEAD
	.ops = &imote2_asoc_ops,
};

static struct snd_soc_card snd_soc_imote2 = {
	.name = "Imote2",
=======
	.dai_fmt = SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF |
		   SND_SOC_DAIFMT_CBS_CFS,
	.ops = &imote2_asoc_ops,
};

static struct snd_soc_card imote2 = {
	.name = "Imote2",
	.owner = THIS_MODULE,
>>>>>>> refs/remotes/origin/cm-10.0
	.dai_link = &imote2_dai,
	.num_links = 1,
};

<<<<<<< HEAD
static struct platform_device *imote2_snd_device;

static int __init imote2_asoc_init(void)
{
	int ret;

	if (!machine_is_intelmote2())
		return -ENODEV;
	imote2_snd_device = platform_device_alloc("soc-audio", -1);
	if (!imote2_snd_device)
		return -ENOMEM;

	platform_set_drvdata(imote2_snd_device, &snd_soc_imote2);
	ret = platform_device_add(imote2_snd_device);
	if (ret)
		platform_device_put(imote2_snd_device);

	return ret;
}
module_init(imote2_asoc_init);

static void __exit imote2_asoc_exit(void)
{
	platform_device_unregister(imote2_snd_device);
}
module_exit(imote2_asoc_exit);
=======
static int __devinit imote2_probe(struct platform_device *pdev)
{
	struct snd_soc_card *card = &imote2;
	int ret;

	card->dev = &pdev->dev;

	ret = snd_soc_register_card(card);
	if (ret)
		dev_err(&pdev->dev, "snd_soc_register_card() failed: %d\n",
			ret);
	return ret;
}

static int __devexit imote2_remove(struct platform_device *pdev)
{
	struct snd_soc_card *card = platform_get_drvdata(pdev);

	snd_soc_unregister_card(card);
	return 0;
}

static struct platform_driver imote2_driver = {
	.driver		= {
		.name	= "imote2-audio",
		.owner	= THIS_MODULE,
	},
	.probe		= imote2_probe,
	.remove		= __devexit_p(imote2_remove),
};

module_platform_driver(imote2_driver);
>>>>>>> refs/remotes/origin/cm-10.0

MODULE_AUTHOR("Jonathan Cameron");
MODULE_DESCRIPTION("ALSA SoC Imote 2");
MODULE_LICENSE("GPL");
<<<<<<< HEAD
=======
MODULE_ALIAS("platform:imote2-audio");
>>>>>>> refs/remotes/origin/cm-10.0
