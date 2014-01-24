/* Copyright (c) 2011-2012, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */


#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/platform_device.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/of_device.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/of_device.h>
>>>>>>> refs/remotes/origin/cm-11.0
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/soc.h>

static struct snd_soc_dai_ops msm_fe_dai_ops = {};

/* Conventional and unconventional sample rate supported */
static unsigned int supported_sample_rates[] = {
	8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000
};

static struct snd_pcm_hw_constraint_list constraints_sample_rates = {
	.count = ARRAY_SIZE(supported_sample_rates),
	.list = supported_sample_rates,
	.mask = 0,
};

static int multimedia_startup(struct snd_pcm_substream *substream,
	struct snd_soc_dai *dai)
{
	snd_pcm_hw_constraint_list(substream->runtime, 0,
		SNDRV_PCM_HW_PARAM_RATE,
		&constraints_sample_rates);

	return 0;
}

static struct snd_soc_dai_ops msm_fe_Multimedia_dai_ops = {
	.startup	= multimedia_startup,
};

static struct snd_soc_dai_driver msm_fe_dais[] = {
	{
		.playback = {
			.stream_name = "Multimedia1 Playback",
<<<<<<< HEAD
<<<<<<< HEAD
			.rates = SNDRV_PCM_RATE_8000_48000,
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
			.channels_min = 1,
			.channels_max = 2,
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
			.aif_name = "MM_DL1",
			.rates = (SNDRV_PCM_RATE_8000_48000|
					SNDRV_PCM_RATE_KNOT),
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
			.channels_min = 1,
			.channels_max = 6,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
			.rate_min =     8000,
			.rate_max =	48000,
		},
		.capture = {
			.stream_name = "Multimedia1 Capture",
<<<<<<< HEAD
<<<<<<< HEAD
			.rates = SNDRV_PCM_RATE_8000_48000,
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
			.channels_min = 1,
			.channels_max = 2,
			.rate_min =     8000,
			.rate_max =	48000,
		},
		.ops = &msm_fe_dai_ops,
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
			.aif_name = "MM_UL1",
			.rates = (SNDRV_PCM_RATE_8000_48000|
					SNDRV_PCM_RATE_KNOT),
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
			.channels_min = 1,
			.channels_max = 4,
			.rate_min =     8000,
			.rate_max =	48000,
		},
		.ops = &msm_fe_Multimedia_dai_ops,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
		.name = "MultiMedia1",
	},
	{
		.playback = {
			.stream_name = "Multimedia2 Playback",
<<<<<<< HEAD
<<<<<<< HEAD
			.rates = SNDRV_PCM_RATE_8000_48000,
=======
			.aif_name = "MM_DL2",
			.rates = (SNDRV_PCM_RATE_8000_48000|
					SNDRV_PCM_RATE_KNOT),
>>>>>>> refs/remotes/origin/cm-10.0
=======
			.aif_name = "MM_DL2",
			.rates = (SNDRV_PCM_RATE_8000_48000|
					SNDRV_PCM_RATE_KNOT),
>>>>>>> refs/remotes/origin/cm-11.0
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
			.channels_min = 1,
			.channels_max = 6,
			.rate_min =     8000,
			.rate_max =	48000,
		},
		.capture = {
			.stream_name = "Multimedia2 Capture",
<<<<<<< HEAD
<<<<<<< HEAD
			.rates = SNDRV_PCM_RATE_8000_48000,
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
			.channels_min = 1,
			.channels_max = 2,
			.rate_min =     8000,
			.rate_max =	48000,
		},
		.ops = &msm_fe_dai_ops,
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
			.aif_name = "MM_UL2",
			.rates = (SNDRV_PCM_RATE_8000_48000|
					SNDRV_PCM_RATE_KNOT),
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
			.channels_min = 1,
			.channels_max = 8,
			.rate_min =     8000,
			.rate_max =	48000,
		},
		.ops = &msm_fe_Multimedia_dai_ops,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
		.name = "MultiMedia2",
	},
	{
		.playback = {
			.stream_name = "Voice Playback",
<<<<<<< HEAD
<<<<<<< HEAD
=======
			.aif_name = "CS-VOICE_DL1",
>>>>>>> refs/remotes/origin/cm-10.0
=======
			.aif_name = "CS-VOICE_DL1",
>>>>>>> refs/remotes/origin/cm-11.0
			.rates = SNDRV_PCM_RATE_8000_48000,
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
			.channels_min = 1,
			.channels_max = 2,
			.rate_min =     8000,
			.rate_max =     48000,
		},
		.capture = {
			.stream_name = "Voice Capture",
<<<<<<< HEAD
<<<<<<< HEAD
=======
			.aif_name = "CS-VOICE_UL1",
>>>>>>> refs/remotes/origin/cm-10.0
=======
			.aif_name = "CS-VOICE_UL1",
>>>>>>> refs/remotes/origin/cm-11.0
			.rates = SNDRV_PCM_RATE_8000_48000,
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
			.channels_min = 1,
			.channels_max = 2,
			.rate_min =     8000,
			.rate_max =     48000,
		},
		.ops = &msm_fe_dai_ops,
		.name = "CS-VOICE",
	},
	{
		.playback = {
			.stream_name = "VoIP Playback",
<<<<<<< HEAD
<<<<<<< HEAD
=======
			.aif_name = "VOIP_DL",
>>>>>>> refs/remotes/origin/cm-10.0
=======
			.aif_name = "VOIP_DL",
>>>>>>> refs/remotes/origin/cm-11.0
			.rates = SNDRV_PCM_RATE_8000_48000,
			.formats = SNDRV_PCM_FMTBIT_S16_LE |
					SNDRV_PCM_FMTBIT_SPECIAL,
			.channels_min = 1,
			.channels_max = 2,
			.rate_min =	8000,
			.rate_max = 48000,
		},
		.capture = {
			.stream_name = "VoIP Capture",
<<<<<<< HEAD
<<<<<<< HEAD
=======
			.aif_name = "VOIP_UL",
>>>>>>> refs/remotes/origin/cm-10.0
=======
			.aif_name = "VOIP_UL",
>>>>>>> refs/remotes/origin/cm-11.0
			.rates = SNDRV_PCM_RATE_8000_48000,
			.formats = SNDRV_PCM_FMTBIT_S16_LE |
					SNDRV_PCM_FMTBIT_SPECIAL,
			.channels_min = 1,
			.channels_max = 2,
			.rate_min =	8000,
			.rate_max = 48000,
		},
		.ops = &msm_fe_dai_ops,
		.name = "VoIP",
	},
	{
		.playback = {
			.stream_name = "MultiMedia3 Playback",
<<<<<<< HEAD
<<<<<<< HEAD
=======
			.aif_name = "MM_DL3",
>>>>>>> refs/remotes/origin/cm-10.0
=======
			.aif_name = "MM_DL3",
>>>>>>> refs/remotes/origin/cm-11.0
			.rates = (SNDRV_PCM_RATE_8000_48000 |
					SNDRV_PCM_RATE_KNOT),
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
			.channels_min = 1,
<<<<<<< HEAD
<<<<<<< HEAD
			.channels_max = 2,
=======
			.channels_max = 6,
>>>>>>> refs/remotes/origin/cm-10.0
=======
			.channels_max = 6,
>>>>>>> refs/remotes/origin/cm-11.0
			.rate_min =	8000,
			.rate_max = 48000,
		},
		.ops = &msm_fe_Multimedia_dai_ops,
		.name = "MultiMedia3",
	},
	{
		.playback = {
			.stream_name = "MultiMedia4 Playback",
<<<<<<< HEAD
<<<<<<< HEAD
=======
			.aif_name = "MM_DL4",
>>>>>>> refs/remotes/origin/cm-10.0
=======
			.aif_name = "MM_DL4",
>>>>>>> refs/remotes/origin/cm-11.0
			.rates = (SNDRV_PCM_RATE_8000_48000 |
					SNDRV_PCM_RATE_KNOT),
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
			.channels_min = 1,
<<<<<<< HEAD
<<<<<<< HEAD
			.channels_max = 2,
			.rate_min =	8000,
			.rate_max = 48000,
		},
		.ops = &msm_fe_Multimedia_dai_ops,
		.name = "MultiMedia4",
	},
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
			.channels_max = 6,
			.rate_min =	8000,
			.rate_max = 48000,
		},
		.capture = {
			.stream_name = "MultiMedia4 Capture",
			.aif_name = "MM_UL4",
			.rates = (SNDRV_PCM_RATE_8000_48000|
					SNDRV_PCM_RATE_KNOT),
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
			.channels_min = 1,
			.channels_max = 8,
			.rate_min =     8000,
			.rate_max =	48000,
		},
		.ops = &msm_fe_Multimedia_dai_ops,
		.name = "MultiMedia4",
	},
	{
		.playback = {
			.stream_name = "MultiMedia5 Playback",
			.aif_name = "MM_DL5",
			.rates = (SNDRV_PCM_RATE_8000_48000 |
					SNDRV_PCM_RATE_KNOT),
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
			.channels_min = 1,
			.channels_max = 6,
			.rate_min =	8000,
			.rate_max = 48000,
		},
		.ops = &msm_fe_Multimedia_dai_ops,
		.name = "MultiMedia5",
	},
	{
		.playback = {
			.stream_name = "MultiMedia6 Playback",
			.aif_name = "MM_DL6",
			.rates = (SNDRV_PCM_RATE_8000_48000 |
					SNDRV_PCM_RATE_KNOT),
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
			.channels_min = 1,
			.channels_max = 6,
			.rate_min =	8000,
			.rate_max = 48000,
		},
		.ops = &msm_fe_Multimedia_dai_ops,
		.name = "MultiMedia6",
	},
	{
		.playback = {
			.stream_name = "MultiMedia7 Playback",
			.aif_name = "MM_DL7",
			.rates = (SNDRV_PCM_RATE_8000_48000 |
					SNDRV_PCM_RATE_KNOT),
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
			.channels_min = 1,
			.channels_max = 6,
			.rate_min =	8000,
			.rate_max = 48000,
		},
		.ops = &msm_fe_Multimedia_dai_ops,
		.name = "MultiMedia7",
	},
	{
		.playback = {
			.stream_name = "MultiMedia8 Playback",
			.aif_name = "MM_DL8",
			.rates = (SNDRV_PCM_RATE_8000_48000 |
					SNDRV_PCM_RATE_KNOT),
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
			.channels_min = 1,
			.channels_max = 6,
			.rate_min =	8000,
			.rate_max = 48000,
		},
		.ops = &msm_fe_Multimedia_dai_ops,
		.name = "MultiMedia8",
	},
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
	/* FE DAIs created for hostless operation purpose */
	{
		.playback = {
			.stream_name = "SLIMBUS0 Hostless Playback",
<<<<<<< HEAD
<<<<<<< HEAD
=======
			.aif_name = "SLIM0_DL_HL",
>>>>>>> refs/remotes/origin/cm-10.0
=======
			.aif_name = "SLIM0_DL_HL",
>>>>>>> refs/remotes/origin/cm-11.0
			.rates = SNDRV_PCM_RATE_8000_48000,
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
			.channels_min = 1,
			.channels_max = 2,
			.rate_min =     8000,
			.rate_max =     48000,
		},
		.capture = {
			.stream_name = "SLIMBUS0 Hostless Capture",
<<<<<<< HEAD
<<<<<<< HEAD
=======
			.aif_name = "SLIM0_UL_HL",
>>>>>>> refs/remotes/origin/cm-10.0
=======
			.aif_name = "SLIM0_UL_HL",
>>>>>>> refs/remotes/origin/cm-11.0
			.rates = SNDRV_PCM_RATE_8000_48000,
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
			.channels_min = 1,
			.channels_max = 2,
			.rate_min =     8000,
			.rate_max =     48000,
		},
		.ops = &msm_fe_dai_ops,
		.name = "SLIMBUS0_HOSTLESS",
	},
	{
		.playback = {
			.stream_name = "INT_FM Hostless Playback",
<<<<<<< HEAD
<<<<<<< HEAD
=======
			.aif_name = "INTFM_DL_HL",
>>>>>>> refs/remotes/origin/cm-10.0
=======
			.aif_name = "INTFM_DL_HL",
>>>>>>> refs/remotes/origin/cm-11.0
			.rates = SNDRV_PCM_RATE_8000_48000,
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
			.channels_min = 1,
			.channels_max = 2,
			.rate_min =     8000,
			.rate_max =     48000,
		},
		.capture = {
			.stream_name = "INT_FM Hostless Capture",
<<<<<<< HEAD
<<<<<<< HEAD
=======
			.aif_name = "INTFM_UL_HL",
>>>>>>> refs/remotes/origin/cm-10.0
=======
			.aif_name = "INTFM_UL_HL",
>>>>>>> refs/remotes/origin/cm-11.0
			.rates = SNDRV_PCM_RATE_8000_48000,
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
			.channels_min = 1,
			.channels_max = 2,
			.rate_min =     8000,
			.rate_max =     48000,
		},
		.ops = &msm_fe_dai_ops,
		.name = "INT_FM_HOSTLESS",
	},
	{
		.playback = {
			.stream_name = "AFE-PROXY Playback",
<<<<<<< HEAD
<<<<<<< HEAD
=======
			.aif_name = "PCM_RX",
>>>>>>> refs/remotes/origin/cm-10.0
=======
			.aif_name = "PCM_RX",
>>>>>>> refs/remotes/origin/cm-11.0
			.rates = (SNDRV_PCM_RATE_8000 |
				SNDRV_PCM_RATE_16000 |
				SNDRV_PCM_RATE_48000),
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
			.channels_min = 1,
			.channels_max = 2,
			.rate_min =     8000,
			.rate_max =     48000,
		},
		.capture = {
			.stream_name = "AFE-PROXY Capture",
<<<<<<< HEAD
<<<<<<< HEAD
=======
			.aif_name = "PCM_TX",
>>>>>>> refs/remotes/origin/cm-10.0
=======
			.aif_name = "PCM_TX",
>>>>>>> refs/remotes/origin/cm-11.0
			.rates = (SNDRV_PCM_RATE_8000 |
				SNDRV_PCM_RATE_16000 |
				SNDRV_PCM_RATE_48000),
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
			.channels_min = 1,
			.channels_max = 2,
			.rate_min =     8000,
			.rate_max =     48000,
		},
		.ops = &msm_fe_dai_ops,
		.name = "AFE-PROXY",
	},
	{
		.playback = {
			.stream_name = "HDMI_Rx Hostless Playback",
<<<<<<< HEAD
<<<<<<< HEAD
=======
			.aif_name = "HDMI_DL_HL",
>>>>>>> refs/remotes/origin/cm-10.0
=======
			.aif_name = "HDMI_DL_HL",
>>>>>>> refs/remotes/origin/cm-11.0
			.rates = SNDRV_PCM_RATE_8000_48000,
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
			.channels_min = 1,
			.channels_max = 2,
			.rate_min = 8000,
			.rate_max = 48000,
		},
		.ops = &msm_fe_dai_ops,
		.name = "HDMI_HOSTLESS"
	},
	{
		.playback = {
			.stream_name = "AUXPCM Hostless Playback",
<<<<<<< HEAD
<<<<<<< HEAD
			.rates = SNDRV_PCM_RATE_8000,
=======
			.aif_name = "AUXPCM_DL_HL",
			.rates = SNDRV_PCM_RATE_8000 | SNDRV_PCM_RATE_16000,
>>>>>>> refs/remotes/origin/cm-10.0
=======
			.aif_name = "AUXPCM_DL_HL",
			.rates = SNDRV_PCM_RATE_8000 | SNDRV_PCM_RATE_16000,
>>>>>>> refs/remotes/origin/cm-11.0
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
			.channels_min = 1,
			.channels_max = 1,
			.rate_min =     8000,
<<<<<<< HEAD
<<<<<<< HEAD
			.rate_max =     8000,
		},
		.capture = {
			.stream_name = "AUXPCM Hostless Capture",
			.rates = SNDRV_PCM_RATE_8000,
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
			.rate_max =     16000,
		},
		.capture = {
			.stream_name = "AUXPCM Hostless Capture",
			.aif_name = "AUXPCM_UL_HL",
			.rates = SNDRV_PCM_RATE_8000 | SNDRV_PCM_RATE_16000,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
			.channels_min = 1,
			.channels_max = 1,
			.rate_min =     8000,
<<<<<<< HEAD
<<<<<<< HEAD
			.rate_max =    48000,
=======
			.rate_max =    16000,
>>>>>>> refs/remotes/origin/cm-10.0
=======
			.rate_max =    16000,
>>>>>>> refs/remotes/origin/cm-11.0
		},
		.ops = &msm_fe_dai_ops,
		.name = "AUXPCM_HOSTLESS",
	},
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	{
		.playback = {
			.stream_name = "Voice Stub Playback",
			.aif_name = "VOICE_STUB_DL",
			.rates = SNDRV_PCM_RATE_8000_48000,
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
			.channels_min = 1,
			.channels_max = 2,
			.rate_min = 8000,
			.rate_max = 48000,
		},
		.capture = {
			.stream_name = "Voice Stub Capture",
			.aif_name = "VOICE_STUB_UL",
			.rates = SNDRV_PCM_RATE_8000_48000,
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
			.channels_min = 1,
			.channels_max = 2,
			.rate_min = 8000,
			.rate_max = 48000,
		},
		.ops = &msm_fe_dai_ops,
		.name = "VOICE_STUB",
	},
	{
		.playback = {
			.stream_name = "VoLTE Playback",
			.aif_name = "VoLTE_DL",
			.rates = SNDRV_PCM_RATE_8000_48000,
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
			.channels_min = 1,
			.channels_max = 2,
			.rate_min =     8000,
			.rate_max =     48000,
		},
		.capture = {
			.stream_name = "VoLTE Capture",
			.aif_name = "VoLTE_UL",
			.rates = SNDRV_PCM_RATE_8000_48000,
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
			.channels_min = 1,
			.channels_max = 2,
			.rate_min =     8000,
			.rate_max =     48000,
		},
		.ops = &msm_fe_dai_ops,
		.name = "VoLTE",
	},
	{
		.capture = {
			.stream_name = "MI2S_TX Hostless Capture",
			.aif_name = "MI2S_UL_HL",
			.rates = SNDRV_PCM_RATE_8000_48000,
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
			.channels_min = 1,
			.channels_max = 8,
			.rate_min =     8000,
			.rate_max =    48000,
		},
		.ops = &msm_fe_dai_ops,
		.name = "MI2S_TX_HOSTLESS",
	},
	{
		.playback = {
			.stream_name = "SEC_I2S_RX Hostless Playback",
			.aif_name = "SEC_I2S_DL_HL",
			.rates = SNDRV_PCM_RATE_8000_48000,
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
			.channels_min = 1,
			.channels_max = 2,
			.rate_min =     8000,
			.rate_max =    48000,
		},
		.ops = &msm_fe_dai_ops,
		.name = "SEC_I2S_RX_HOSTLESS",
	},
	{
		.playback = {
			.stream_name = "SGLTE Playback",
			.aif_name = "SGLTE_DL",
			.rates = SNDRV_PCM_RATE_8000_48000,
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
			.channels_min = 1,
			.channels_max = 2,
			.rate_min =     8000,
			.rate_max =     48000,
		},
		.capture = {
			.stream_name = "SGLTE Capture",
			.aif_name = "SGLTE_UL",
			.rates = SNDRV_PCM_RATE_8000_48000,
			.formats = SNDRV_PCM_FMTBIT_S16_LE,
			.channels_min = 1,
			.channels_max = 2,
			.rate_min =     8000,
			.rate_max =     48000,
		},
		.ops = &msm_fe_dai_ops,
		.name = "SGLTE",
	},
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
};

static __devinit int msm_fe_dai_dev_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
<<<<<<< HEAD
	dev_dbg(&pdev->dev, "%s: dev name %s\n", __func__,
	dev_name(&pdev->dev));
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (pdev->dev.of_node)
		dev_set_name(&pdev->dev, "%s", "msm-dai-fe");

	dev_dbg(&pdev->dev, "%s: dev name %s\n", __func__,
		dev_name(&pdev->dev));
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
	return snd_soc_register_dais(&pdev->dev, msm_fe_dais,
		ARRAY_SIZE(msm_fe_dais));
}

static __devexit int msm_fe_dai_dev_remove(struct platform_device *pdev)
{
	snd_soc_unregister_dai(&pdev->dev);
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
static const struct of_device_id msm_dai_fe_dt_match[] = {
	{.compatible = "qcom,msm-dai-fe"},
	{}
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
static struct platform_driver msm_fe_dai_driver = {
	.probe  = msm_fe_dai_dev_probe,
	.remove = msm_fe_dai_dev_remove,
	.driver = {
		.name = "msm-dai-fe",
		.owner = THIS_MODULE,
<<<<<<< HEAD
<<<<<<< HEAD
=======
		.of_match_table = msm_dai_fe_dt_match,
>>>>>>> refs/remotes/origin/cm-10.0
=======
		.of_match_table = msm_dai_fe_dt_match,
>>>>>>> refs/remotes/origin/cm-11.0
	},
};

static int __init msm_fe_dai_init(void)
{
	return platform_driver_register(&msm_fe_dai_driver);
}
module_init(msm_fe_dai_init);

static void __exit msm_fe_dai_exit(void)
{
	platform_driver_unregister(&msm_fe_dai_driver);
}
module_exit(msm_fe_dai_exit);

/* Module information */
MODULE_DESCRIPTION("MSM Frontend DAI driver");
MODULE_LICENSE("GPL v2");
