/*
 * soc-pcm.c  --  ALSA SoC PCM
 *
 * Copyright 2005 Wolfson Microelectronics PLC.
 * Copyright 2005 Openedhand Ltd.
 * Copyright (C) 2010 Slimlogic Ltd.
 * Copyright (C) 2010 Texas Instruments Inc.
 *
 * Authors: Liam Girdwood <lrg@ti.com>
 *          Mark Brown <broonie@opensource.wolfsonmicro.com>       
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
<<<<<<< HEAD
#include <linux/pm_runtime.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
#include <linux/debugfs.h>
#include <linux/dma-mapping.h>
#include <linux/export.h>
=======
#include <linux/pinctrl/consumer.h>
#include <linux/pm_runtime.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
#include <linux/export.h>
#include <linux/debugfs.h>
>>>>>>> refs/remotes/origin/master
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/soc-dpcm.h>
#include <sound/initval.h>

<<<<<<< HEAD
#define MAX_BE_USERS	8	/* adjust if too low for everday use */

static int soc_dpcm_be_dai_hw_free(struct snd_soc_pcm_runtime *fe, int stream);

/* ASoC no host IO hardware.
 * TODO: fine tune these values for all host less transfers.
 */
static const struct snd_pcm_hardware no_host_hardware = {
	.info			= SNDRV_PCM_INFO_MMAP |
				  SNDRV_PCM_INFO_MMAP_VALID |
				  SNDRV_PCM_INFO_INTERLEAVED |
				  SNDRV_PCM_INFO_PAUSE |
				  SNDRV_PCM_INFO_RESUME,
	.formats		= SNDRV_PCM_FMTBIT_S16_LE |
				  SNDRV_PCM_FMTBIT_S32_LE,
	.period_bytes_min	= PAGE_SIZE >> 2,
	.period_bytes_max	= PAGE_SIZE >> 1,
	.periods_min		= 2,
	.periods_max		= 4,
	.buffer_bytes_max	= PAGE_SIZE,
};

/*
 * We can only hw_free, stop, pause or suspend a BE DAI if any of it's FE
 * are not running, paused or suspended for the specified stream direction.
 */
int snd_soc_dpcm_can_be_free_stop(struct snd_soc_pcm_runtime *fe,
		struct snd_soc_pcm_runtime *be, int stream)
{
	struct snd_soc_dpcm_params *dpcm_params;

	list_for_each_entry(dpcm_params, &be->dpcm[stream].fe_clients, list_fe) {

		if (dpcm_params->fe == fe)
			continue;

		if (dpcm_params->fe->dpcm[stream].state == SND_SOC_DPCM_STATE_START ||
			dpcm_params->fe->dpcm[stream].state == SND_SOC_DPCM_STATE_PAUSED ||
			dpcm_params->fe->dpcm[stream].state == SND_SOC_DPCM_STATE_SUSPEND)
			return 0;
	}
	return 1;
}
EXPORT_SYMBOL_GPL(snd_soc_dpcm_can_be_free_stop);

/*
 * We can only change hw params a BE DAI if any of it's FE are not prepared,
 * running, paused or suspended for the specified stream direction.
 */
static int snd_soc_dpcm_can_be_params(struct snd_soc_pcm_runtime *fe,
		struct snd_soc_pcm_runtime *be, int stream)
{
	struct snd_soc_dpcm_params *dpcm_params;

	list_for_each_entry(dpcm_params, &be->dpcm[stream].fe_clients, list_fe) {

		if (dpcm_params->fe == fe)
			continue;

		if (dpcm_params->fe->dpcm[stream].state == SND_SOC_DPCM_STATE_START ||
			dpcm_params->fe->dpcm[stream].state == SND_SOC_DPCM_STATE_PAUSED ||
			dpcm_params->fe->dpcm[stream].state == SND_SOC_DPCM_STATE_SUSPEND ||
			dpcm_params->fe->dpcm[stream].state == SND_SOC_DPCM_STATE_PREPARE)
			return 0;
	}
	return 1;
=======
#define DPCM_MAX_BE_USERS	8

/**
 * snd_soc_set_runtime_hwparams - set the runtime hardware parameters
 * @substream: the pcm substream
 * @hw: the hardware parameters
 *
 * Sets the substream runtime hardware parameters.
 */
int snd_soc_set_runtime_hwparams(struct snd_pcm_substream *substream,
	const struct snd_pcm_hardware *hw)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	runtime->hw.info = hw->info;
	runtime->hw.formats = hw->formats;
	runtime->hw.period_bytes_min = hw->period_bytes_min;
	runtime->hw.period_bytes_max = hw->period_bytes_max;
	runtime->hw.periods_min = hw->periods_min;
	runtime->hw.periods_max = hw->periods_max;
	runtime->hw.buffer_bytes_max = hw->buffer_bytes_max;
	runtime->hw.fifo_size = hw->fifo_size;
	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_set_runtime_hwparams);

/* DPCM stream event, send event to FE and all active BEs. */
int dpcm_dapm_stream_event(struct snd_soc_pcm_runtime *fe, int dir,
	int event)
{
	struct snd_soc_dpcm *dpcm;

	list_for_each_entry(dpcm, &fe->dpcm[dir].be_clients, list_be) {

		struct snd_soc_pcm_runtime *be = dpcm->be;

		dev_dbg(be->dev, "ASoC: BE %s event %d dir %d\n",
				be->dai_link->name, event, dir);

		snd_soc_dapm_stream_event(be, dir, event);
	}

	snd_soc_dapm_stream_event(fe, dir, event);

	return 0;
>>>>>>> refs/remotes/origin/master
}

static int soc_pcm_apply_symmetry(struct snd_pcm_substream *substream,
					struct snd_soc_dai *soc_dai)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	int ret;

<<<<<<< HEAD
	if (!soc_dai->driver->symmetric_rates &&
	    !rtd->dai_link->symmetric_rates)
		return 0;

	/* This can happen if multiple streams are starting simultaneously -
	 * the second can need to get its constraints before the first has
	 * picked a rate.  Complain and allow the application to carry on.
	 */
	if (!soc_dai->rate) {
		dev_warn(soc_dai->dev,
			 "Not enforcing symmetric_rates due to race\n");
		return 0;
	}

	dev_dbg(soc_dai->dev, "Symmetry forces %dHz rate\n", soc_dai->rate);

	ret = snd_pcm_hw_constraint_minmax(substream->runtime,
					   SNDRV_PCM_HW_PARAM_RATE,
					   soc_dai->rate, soc_dai->rate);
	if (ret < 0) {
		dev_err(soc_dai->dev,
			"Unable to apply rate symmetry constraint: %d\n", ret);
		return ret;
=======
	if (soc_dai->rate && (soc_dai->driver->symmetric_rates ||
				rtd->dai_link->symmetric_rates)) {
		dev_dbg(soc_dai->dev, "ASoC: Symmetry forces %dHz rate\n",
				soc_dai->rate);

		ret = snd_pcm_hw_constraint_minmax(substream->runtime,
						SNDRV_PCM_HW_PARAM_RATE,
						soc_dai->rate, soc_dai->rate);
		if (ret < 0) {
			dev_err(soc_dai->dev,
				"ASoC: Unable to apply rate constraint: %d\n",
				ret);
			return ret;
		}
	}

	if (soc_dai->channels && (soc_dai->driver->symmetric_channels ||
				rtd->dai_link->symmetric_channels)) {
		dev_dbg(soc_dai->dev, "ASoC: Symmetry forces %d channel(s)\n",
				soc_dai->channels);

		ret = snd_pcm_hw_constraint_minmax(substream->runtime,
						SNDRV_PCM_HW_PARAM_CHANNELS,
						soc_dai->channels,
						soc_dai->channels);
		if (ret < 0) {
			dev_err(soc_dai->dev,
				"ASoC: Unable to apply channel symmetry constraint: %d\n",
				ret);
			return ret;
		}
	}

	if (soc_dai->sample_bits && (soc_dai->driver->symmetric_samplebits ||
				rtd->dai_link->symmetric_samplebits)) {
		dev_dbg(soc_dai->dev, "ASoC: Symmetry forces %d sample bits\n",
				soc_dai->sample_bits);

		ret = snd_pcm_hw_constraint_minmax(substream->runtime,
						SNDRV_PCM_HW_PARAM_SAMPLE_BITS,
						soc_dai->sample_bits,
						soc_dai->sample_bits);
		if (ret < 0) {
			dev_err(soc_dai->dev,
				"ASoC: Unable to apply sample bits symmetry constraint: %d\n",
				ret);
			return ret;
		}
	}

	return 0;
}

static int soc_pcm_params_symmetry(struct snd_pcm_substream *substream,
				struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	unsigned int rate, channels, sample_bits, symmetry;

	rate = params_rate(params);
	channels = params_channels(params);
	sample_bits = snd_pcm_format_physical_width(params_format(params));

	/* reject unmatched parameters when applying symmetry */
	symmetry = cpu_dai->driver->symmetric_rates ||
		codec_dai->driver->symmetric_rates ||
		rtd->dai_link->symmetric_rates;
	if (symmetry && cpu_dai->rate && cpu_dai->rate != rate) {
		dev_err(rtd->dev, "ASoC: unmatched rate symmetry: %d - %d\n",
				cpu_dai->rate, rate);
		return -EINVAL;
	}

	symmetry = cpu_dai->driver->symmetric_channels ||
		codec_dai->driver->symmetric_channels ||
		rtd->dai_link->symmetric_channels;
	if (symmetry && cpu_dai->channels && cpu_dai->channels != channels) {
		dev_err(rtd->dev, "ASoC: unmatched channel symmetry: %d - %d\n",
				cpu_dai->channels, channels);
		return -EINVAL;
	}

	symmetry = cpu_dai->driver->symmetric_samplebits ||
		codec_dai->driver->symmetric_samplebits ||
		rtd->dai_link->symmetric_samplebits;
	if (symmetry && cpu_dai->sample_bits && cpu_dai->sample_bits != sample_bits) {
		dev_err(rtd->dev, "ASoC: unmatched sample bits symmetry: %d - %d\n",
				cpu_dai->sample_bits, sample_bits);
		return -EINVAL;
>>>>>>> refs/remotes/origin/master
	}

	return 0;
}

<<<<<<< HEAD
=======
static bool soc_pcm_has_symmetry(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai_driver *cpu_driver = rtd->cpu_dai->driver;
	struct snd_soc_dai_driver *codec_driver = rtd->codec_dai->driver;
	struct snd_soc_dai_link *link = rtd->dai_link;

	return cpu_driver->symmetric_rates || codec_driver->symmetric_rates ||
		link->symmetric_rates || cpu_driver->symmetric_channels ||
		codec_driver->symmetric_channels || link->symmetric_channels ||
		cpu_driver->symmetric_samplebits ||
		codec_driver->symmetric_samplebits ||
		link->symmetric_samplebits;
}

>>>>>>> refs/remotes/origin/master
/*
 * List of sample sizes that might go over the bus for parameter
 * application.  There ought to be a wildcard sample size for things
 * like the DAC/ADC resolution to use but there isn't right now.
 */
static int sample_sizes[] = {
<<<<<<< HEAD
	8, 16, 24, 32,
=======
	24, 32,
>>>>>>> refs/remotes/origin/master
};

static void soc_pcm_apply_msb(struct snd_pcm_substream *substream,
			      struct snd_soc_dai *dai)
{
	int ret, i, bits;

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		bits = dai->driver->playback.sig_bits;
	else
		bits = dai->driver->capture.sig_bits;

	if (!bits)
		return;

	for (i = 0; i < ARRAY_SIZE(sample_sizes); i++) {
		if (bits >= sample_sizes[i])
			continue;

		ret = snd_pcm_hw_constraint_msbits(substream->runtime, 0,
						   sample_sizes[i], bits);
		if (ret != 0)
			dev_warn(dai->dev,
<<<<<<< HEAD
				 "Failed to set MSB %d/%d: %d\n",
=======
				 "ASoC: Failed to set MSB %d/%d: %d\n",
>>>>>>> refs/remotes/origin/master
				 bits, sample_sizes[i], ret);
	}
}

<<<<<<< HEAD
/*
 * stream event, send event to FE and all active BEs.
 */
int soc_dpcm_dapm_stream_event(struct snd_soc_pcm_runtime *fe,
	int dir, const char *stream, int event)
{
	struct snd_soc_dpcm_params *dpcm_params;

	snd_soc_dapm_rtd_stream_event(fe, dir, event);

	list_for_each_entry(dpcm_params, &fe->dpcm[dir].be_clients, list_be) {

		struct snd_soc_pcm_runtime *be = dpcm_params->be;

		dev_dbg(be->dev, "pm: BE %s stream %s event %d dir %d\n",
				be->dai_link->name, stream, event, dir);

		snd_soc_dapm_rtd_stream_event(be, dir, event);
	}

	return 0;
=======
static void soc_pcm_init_runtime_hw(struct snd_pcm_runtime *runtime,
	struct snd_soc_pcm_stream *codec_stream,
	struct snd_soc_pcm_stream *cpu_stream)
{
	struct snd_pcm_hardware *hw = &runtime->hw;

	hw->channels_min = max(codec_stream->channels_min,
		cpu_stream->channels_min);
	hw->channels_max = min(codec_stream->channels_max,
		cpu_stream->channels_max);
	if (hw->formats)
		hw->formats &= codec_stream->formats & cpu_stream->formats;
	else
		hw->formats = codec_stream->formats & cpu_stream->formats;
	hw->rates = snd_pcm_rate_mask_intersect(codec_stream->rates,
		cpu_stream->rates);

	hw->rate_min = 0;
	hw->rate_max = UINT_MAX;

	snd_pcm_limit_hw_rates(runtime);

	hw->rate_min = max(hw->rate_min, cpu_stream->rate_min);
	hw->rate_min = max(hw->rate_min, codec_stream->rate_min);
	hw->rate_max = min_not_zero(hw->rate_max, cpu_stream->rate_max);
	hw->rate_max = min_not_zero(hw->rate_max, codec_stream->rate_max);
>>>>>>> refs/remotes/origin/master
}

/*
 * Called by ALSA when a PCM substream is opened, the runtime->hw record is
 * then initialized and any private data can be allocated. This also calls
 * startup for the cpu DAI, platform, machine and codec DAI.
 */
static int soc_pcm_open(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct snd_soc_platform *platform = rtd->platform;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	struct snd_soc_dai_driver *cpu_dai_drv = cpu_dai->driver;
	struct snd_soc_dai_driver *codec_dai_drv = codec_dai->driver;
	int ret = 0;

<<<<<<< HEAD
=======
	pinctrl_pm_select_default_state(cpu_dai->dev);
	pinctrl_pm_select_default_state(codec_dai->dev);
>>>>>>> refs/remotes/origin/master
	pm_runtime_get_sync(cpu_dai->dev);
	pm_runtime_get_sync(codec_dai->dev);
	pm_runtime_get_sync(platform->dev);

	mutex_lock_nested(&rtd->pcm_mutex, rtd->pcm_subclass);

<<<<<<< HEAD
	if (rtd->dai_link->no_host_mode == SND_SOC_DAI_LINK_NO_HOST)
		snd_soc_set_runtime_hwparams(substream, &no_host_hardware);

	/* startup the audio subsystem */
	if (cpu_dai->driver->ops->startup) {
		ret = cpu_dai->driver->ops->startup(substream, cpu_dai);
		if (ret < 0) {
			printk(KERN_ERR "asoc: can't open interface %s\n",
				cpu_dai->name);
=======
	/* startup the audio subsystem */
	if (cpu_dai->driver->ops && cpu_dai->driver->ops->startup) {
		ret = cpu_dai->driver->ops->startup(substream, cpu_dai);
		if (ret < 0) {
			dev_err(cpu_dai->dev, "ASoC: can't open interface"
				" %s: %d\n", cpu_dai->name, ret);
>>>>>>> refs/remotes/origin/master
			goto out;
		}
	}

	if (platform->driver->ops && platform->driver->ops->open) {
		ret = platform->driver->ops->open(substream);
		if (ret < 0) {
<<<<<<< HEAD
			printk(KERN_ERR "asoc: can't open platform %s\n", platform->name);
=======
			dev_err(platform->dev, "ASoC: can't open platform"
				" %s: %d\n", platform->name, ret);
>>>>>>> refs/remotes/origin/master
			goto platform_err;
		}
	}

<<<<<<< HEAD
	if (codec_dai->driver->ops->startup) {
		if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
			ret = codec_dai->driver->ops->startup(substream,
								codec_dai);
			if (ret < 0) {
				printk(KERN_ERR "asoc: can't open codec %s\n",
					codec_dai->name);
				goto codec_dai_err;
			}
		} else {
			if (!codec_dai->capture_active) {
				ret = codec_dai->driver->ops->startup(substream,
								codec_dai);
				if (ret < 0) {
					printk(KERN_ERR "can't open codec %s\n",
						codec_dai->name);
					goto codec_dai_err;
				}
			}
=======
	if (codec_dai->driver->ops && codec_dai->driver->ops->startup) {
		ret = codec_dai->driver->ops->startup(substream, codec_dai);
		if (ret < 0) {
			dev_err(codec_dai->dev, "ASoC: can't open codec"
				" %s: %d\n", codec_dai->name, ret);
			goto codec_dai_err;
>>>>>>> refs/remotes/origin/master
		}
	}

	if (rtd->dai_link->ops && rtd->dai_link->ops->startup) {
		ret = rtd->dai_link->ops->startup(substream);
		if (ret < 0) {
<<<<<<< HEAD
			printk(KERN_ERR "asoc: %s startup failed\n", rtd->dai_link->name);
=======
			pr_err("ASoC: %s startup failed: %d\n",
			       rtd->dai_link->name, ret);
>>>>>>> refs/remotes/origin/master
			goto machine_err;
		}
	}

<<<<<<< HEAD
	/* DSP DAI links compat checks are different */
=======
	/* Dynamic PCM DAI links compat checks use dynamic capabilities */
>>>>>>> refs/remotes/origin/master
	if (rtd->dai_link->dynamic || rtd->dai_link->no_pcm)
		goto dynamic;

	/* Check that the codec and cpu DAIs are compatible */
	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
<<<<<<< HEAD
		runtime->hw.rate_min =
			max(codec_dai_drv->playback.rate_min,
			    cpu_dai_drv->playback.rate_min);
		runtime->hw.rate_max =
			min(codec_dai_drv->playback.rate_max,
			    cpu_dai_drv->playback.rate_max);
		runtime->hw.channels_min =
			max(codec_dai_drv->playback.channels_min,
				cpu_dai_drv->playback.channels_min);
		runtime->hw.channels_max =
			min(codec_dai_drv->playback.channels_max,
				cpu_dai_drv->playback.channels_max);
		runtime->hw.formats =
			codec_dai_drv->playback.formats & cpu_dai_drv->playback.formats;
		runtime->hw.rates =
			codec_dai_drv->playback.rates & cpu_dai_drv->playback.rates;
		if (codec_dai_drv->playback.rates
			   & (SNDRV_PCM_RATE_KNOT | SNDRV_PCM_RATE_CONTINUOUS))
			runtime->hw.rates |= cpu_dai_drv->playback.rates;
		if (cpu_dai_drv->playback.rates
			   & (SNDRV_PCM_RATE_KNOT | SNDRV_PCM_RATE_CONTINUOUS))
			runtime->hw.rates |= codec_dai_drv->playback.rates;
	} else {
		runtime->hw.rate_min =
			max(codec_dai_drv->capture.rate_min,
			    cpu_dai_drv->capture.rate_min);
		runtime->hw.rate_max =
			min(codec_dai_drv->capture.rate_max,
			    cpu_dai_drv->capture.rate_max);
		runtime->hw.channels_min =
			max(codec_dai_drv->capture.channels_min,
				cpu_dai_drv->capture.channels_min);
		runtime->hw.channels_max =
			min(codec_dai_drv->capture.channels_max,
				cpu_dai_drv->capture.channels_max);
		runtime->hw.formats =
			codec_dai_drv->capture.formats & cpu_dai_drv->capture.formats;
		runtime->hw.rates =
			codec_dai_drv->capture.rates & cpu_dai_drv->capture.rates;
		if (codec_dai_drv->capture.rates
			   & (SNDRV_PCM_RATE_KNOT | SNDRV_PCM_RATE_CONTINUOUS))
			runtime->hw.rates |= cpu_dai_drv->capture.rates;
		if (cpu_dai_drv->capture.rates
			   & (SNDRV_PCM_RATE_KNOT | SNDRV_PCM_RATE_CONTINUOUS))
			runtime->hw.rates |= codec_dai_drv->capture.rates;
	}

	ret = -EINVAL;
	snd_pcm_limit_hw_rates(runtime);
	if (!runtime->hw.rates) {
		printk(KERN_ERR "asoc: %s <-> %s No matching rates\n",
=======
		soc_pcm_init_runtime_hw(runtime, &codec_dai_drv->playback,
			&cpu_dai_drv->playback);
	} else {
		soc_pcm_init_runtime_hw(runtime, &codec_dai_drv->capture,
			&cpu_dai_drv->capture);
	}

	if (soc_pcm_has_symmetry(substream))
		runtime->hw.info |= SNDRV_PCM_INFO_JOINT_DUPLEX;

	ret = -EINVAL;
	if (!runtime->hw.rates) {
		printk(KERN_ERR "ASoC: %s <-> %s No matching rates\n",
>>>>>>> refs/remotes/origin/master
			codec_dai->name, cpu_dai->name);
		goto config_err;
	}
	if (!runtime->hw.formats) {
<<<<<<< HEAD
		printk(KERN_ERR "asoc: %s <-> %s No matching formats\n",
=======
		printk(KERN_ERR "ASoC: %s <-> %s No matching formats\n",
>>>>>>> refs/remotes/origin/master
			codec_dai->name, cpu_dai->name);
		goto config_err;
	}
	if (!runtime->hw.channels_min || !runtime->hw.channels_max ||
	    runtime->hw.channels_min > runtime->hw.channels_max) {
<<<<<<< HEAD
		printk(KERN_ERR "asoc: %s <-> %s No matching channels\n",
=======
		printk(KERN_ERR "ASoC: %s <-> %s No matching channels\n",
>>>>>>> refs/remotes/origin/master
				codec_dai->name, cpu_dai->name);
		goto config_err;
	}

	soc_pcm_apply_msb(substream, codec_dai);
	soc_pcm_apply_msb(substream, cpu_dai);

	/* Symmetry only applies if we've already got an active stream. */
	if (cpu_dai->active) {
		ret = soc_pcm_apply_symmetry(substream, cpu_dai);
		if (ret != 0)
			goto config_err;
	}

	if (codec_dai->active) {
		ret = soc_pcm_apply_symmetry(substream, codec_dai);
		if (ret != 0)
			goto config_err;
	}

<<<<<<< HEAD
	pr_debug("asoc: %s <-> %s info:\n",
			codec_dai->name, cpu_dai->name);
	pr_debug("asoc: rate mask 0x%x\n", runtime->hw.rates);
	pr_debug("asoc: min ch %d max ch %d\n", runtime->hw.channels_min,
		 runtime->hw.channels_max);
	pr_debug("asoc: min rate %d max rate %d\n", runtime->hw.rate_min,
=======
	pr_debug("ASoC: %s <-> %s info:\n",
			codec_dai->name, cpu_dai->name);
	pr_debug("ASoC: rate mask 0x%x\n", runtime->hw.rates);
	pr_debug("ASoC: min ch %d max ch %d\n", runtime->hw.channels_min,
		 runtime->hw.channels_max);
	pr_debug("ASoC: min rate %d max rate %d\n", runtime->hw.rate_min,
>>>>>>> refs/remotes/origin/master
		 runtime->hw.rate_max);

dynamic:
	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
		cpu_dai->playback_active++;
		codec_dai->playback_active++;
	} else {
		cpu_dai->capture_active++;
		codec_dai->capture_active++;
	}
	cpu_dai->active++;
	codec_dai->active++;
	rtd->codec->active++;
	mutex_unlock(&rtd->pcm_mutex);
	return 0;

config_err:
	if (rtd->dai_link->ops && rtd->dai_link->ops->shutdown)
		rtd->dai_link->ops->shutdown(substream);

machine_err:
	if (codec_dai->driver->ops->shutdown)
		codec_dai->driver->ops->shutdown(substream, codec_dai);

codec_dai_err:
	if (platform->driver->ops && platform->driver->ops->close)
		platform->driver->ops->close(substream);

platform_err:
	if (cpu_dai->driver->ops->shutdown)
		cpu_dai->driver->ops->shutdown(substream, cpu_dai);
out:
	mutex_unlock(&rtd->pcm_mutex);

	pm_runtime_put(platform->dev);
	pm_runtime_put(codec_dai->dev);
	pm_runtime_put(cpu_dai->dev);
<<<<<<< HEAD
=======
	if (!codec_dai->active)
		pinctrl_pm_select_sleep_state(codec_dai->dev);
	if (!cpu_dai->active)
		pinctrl_pm_select_sleep_state(cpu_dai->dev);
>>>>>>> refs/remotes/origin/master

	return ret;
}

/*
 * Power down the audio subsystem pmdown_time msecs after close is called.
 * This is to ensure there are no pops or clicks in between any music tracks
 * due to DAPM power cycling.
 */
static void close_delayed_work(struct work_struct *work)
{
	struct snd_soc_pcm_runtime *rtd =
			container_of(work, struct snd_soc_pcm_runtime, delayed_work.work);
	struct snd_soc_dai *codec_dai = rtd->codec_dai;

	mutex_lock_nested(&rtd->pcm_mutex, rtd->pcm_subclass);

<<<<<<< HEAD
	pr_debug("pop wq checking: %s status: %s waiting: %s\n",
		 codec_dai->driver->playback.stream_name,
		 codec_dai->playback_active ? "active" : "inactive",
		 codec_dai->pop_wait ? "yes" : "no");

	/* are we waiting on this codec DAI stream */
	if (codec_dai->pop_wait == 1) {
		codec_dai->pop_wait = 0;
		snd_soc_dapm_stream_event(rtd,
			codec_dai->driver->playback.stream_name,
			SND_SOC_DAPM_STREAM_STOP);
=======
	dev_dbg(rtd->dev, "ASoC: pop wq checking: %s status: %s waiting: %s\n",
		 codec_dai->driver->playback.stream_name,
		 codec_dai->playback_active ? "active" : "inactive",
		 rtd->pop_wait ? "yes" : "no");

	/* are we waiting on this codec DAI stream */
	if (rtd->pop_wait == 1) {
		rtd->pop_wait = 0;
		snd_soc_dapm_stream_event(rtd, SNDRV_PCM_STREAM_PLAYBACK,
					  SND_SOC_DAPM_STREAM_STOP);
>>>>>>> refs/remotes/origin/master
	}

	mutex_unlock(&rtd->pcm_mutex);
}

/*
 * Called by ALSA when a PCM substream is closed. Private data can be
 * freed here. The cpu DAI, codec DAI, machine and platform are also
 * shutdown.
 */
static int soc_pcm_close(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_platform *platform = rtd->platform;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	struct snd_soc_codec *codec = rtd->codec;

	mutex_lock_nested(&rtd->pcm_mutex, rtd->pcm_subclass);

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
		cpu_dai->playback_active--;
		codec_dai->playback_active--;
	} else {
		cpu_dai->capture_active--;
		codec_dai->capture_active--;
	}

	cpu_dai->active--;
	codec_dai->active--;
	codec->active--;

	/* clear the corresponding DAIs rate when inactive */
	if (!cpu_dai->active)
		cpu_dai->rate = 0;

	if (!codec_dai->active)
		codec_dai->rate = 0;

<<<<<<< HEAD
	/* Muting the DAC suppresses artifacts caused during digital
	 * shutdown, for example from stopping clocks.
	 */
	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		snd_soc_dai_digital_mute(codec_dai, 1);

	if (cpu_dai->driver->ops->shutdown)
		cpu_dai->driver->ops->shutdown(substream, cpu_dai);

	if (codec_dai->driver->ops->shutdown) {
		if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
			codec_dai->driver->ops->shutdown(substream, codec_dai);
		} else {
			if (!codec_dai->capture_active)
				codec_dai->driver->ops->shutdown(substream,
								codec_dai);
		}
	}
=======
	if (cpu_dai->driver->ops->shutdown)
		cpu_dai->driver->ops->shutdown(substream, cpu_dai);

	if (codec_dai->driver->ops->shutdown)
		codec_dai->driver->ops->shutdown(substream, codec_dai);
>>>>>>> refs/remotes/origin/master

	if (rtd->dai_link->ops && rtd->dai_link->ops->shutdown)
		rtd->dai_link->ops->shutdown(substream);

	if (platform->driver->ops && platform->driver->ops->close)
		platform->driver->ops->close(substream);
	cpu_dai->runtime = NULL;

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
<<<<<<< HEAD
		if (codec->ignore_pmdown_time ||
		    rtd->dai_link->ignore_pmdown_time ||
		    !rtd->pmdown_time) {
			/* powered down playback stream now */
			snd_soc_dapm_stream_event(rtd,
				codec_dai->driver->playback.stream_name,
				SND_SOC_DAPM_STREAM_STOP);
		} else {
			/* start delayed pop wq here for playback streams */
			codec_dai->pop_wait = 1;
			schedule_delayed_work(&rtd->delayed_work,
				msecs_to_jiffies(rtd->pmdown_time));
		}
	} else {
		/* capture streams can be powered down now */
		if (!codec_dai->capture_active)
			snd_soc_dapm_stream_event(rtd,
			codec_dai->driver->capture.stream_name,
			SND_SOC_DAPM_STREAM_STOP);
=======
		if (!rtd->pmdown_time || codec->ignore_pmdown_time ||
		    rtd->dai_link->ignore_pmdown_time) {
			/* powered down playback stream now */
			snd_soc_dapm_stream_event(rtd,
						  SNDRV_PCM_STREAM_PLAYBACK,
						  SND_SOC_DAPM_STREAM_STOP);
		} else {
			/* start delayed pop wq here for playback streams */
			rtd->pop_wait = 1;
			queue_delayed_work(system_power_efficient_wq,
					   &rtd->delayed_work,
					   msecs_to_jiffies(rtd->pmdown_time));
		}
	} else {
		/* capture streams can be powered down now */
		snd_soc_dapm_stream_event(rtd, SNDRV_PCM_STREAM_CAPTURE,
					  SND_SOC_DAPM_STREAM_STOP);
>>>>>>> refs/remotes/origin/master
	}

	mutex_unlock(&rtd->pcm_mutex);

	pm_runtime_put(platform->dev);
	pm_runtime_put(codec_dai->dev);
	pm_runtime_put(cpu_dai->dev);
<<<<<<< HEAD
=======
	if (!codec_dai->active)
		pinctrl_pm_select_sleep_state(codec_dai->dev);
	if (!cpu_dai->active)
		pinctrl_pm_select_sleep_state(cpu_dai->dev);
>>>>>>> refs/remotes/origin/master

	return 0;
}

/*
 * Called by ALSA when the PCM substream is prepared, can set format, sample
 * rate, etc.  This function is non atomic and can be called multiple times,
 * it can refer to the runtime info.
 */
static int soc_pcm_prepare(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_platform *platform = rtd->platform;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	int ret = 0;

	mutex_lock_nested(&rtd->pcm_mutex, rtd->pcm_subclass);

	if (rtd->dai_link->ops && rtd->dai_link->ops->prepare) {
		ret = rtd->dai_link->ops->prepare(substream);
		if (ret < 0) {
<<<<<<< HEAD
			printk(KERN_ERR "asoc: machine prepare error\n");
=======
			dev_err(rtd->card->dev, "ASoC: machine prepare error:"
				" %d\n", ret);
>>>>>>> refs/remotes/origin/master
			goto out;
		}
	}

	if (platform->driver->ops && platform->driver->ops->prepare) {
		ret = platform->driver->ops->prepare(substream);
		if (ret < 0) {
<<<<<<< HEAD
			printk(KERN_ERR "asoc: platform prepare error\n");
=======
			dev_err(platform->dev, "ASoC: platform prepare error:"
				" %d\n", ret);
>>>>>>> refs/remotes/origin/master
			goto out;
		}
	}

<<<<<<< HEAD
	if (codec_dai->driver->ops->prepare) {
		ret = codec_dai->driver->ops->prepare(substream, codec_dai);
		if (ret < 0) {
			printk(KERN_ERR "asoc: codec DAI prepare error\n");
=======
	if (codec_dai->driver->ops && codec_dai->driver->ops->prepare) {
		ret = codec_dai->driver->ops->prepare(substream, codec_dai);
		if (ret < 0) {
			dev_err(codec_dai->dev, "ASoC: DAI prepare error: %d\n",
				ret);
>>>>>>> refs/remotes/origin/master
			goto out;
		}
	}

<<<<<<< HEAD
	if (cpu_dai->driver->ops->prepare) {
		ret = cpu_dai->driver->ops->prepare(substream, cpu_dai);
		if (ret < 0) {
			printk(KERN_ERR "asoc: cpu DAI prepare error\n");
=======
	if (cpu_dai->driver->ops && cpu_dai->driver->ops->prepare) {
		ret = cpu_dai->driver->ops->prepare(substream, cpu_dai);
		if (ret < 0) {
			dev_err(cpu_dai->dev, "ASoC: DAI prepare error: %d\n",
				ret);
>>>>>>> refs/remotes/origin/master
			goto out;
		}
	}

	/* cancel any delayed stream shutdown that is pending */
	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK &&
<<<<<<< HEAD
	    codec_dai->pop_wait) {
		codec_dai->pop_wait = 0;
		cancel_delayed_work(&rtd->delayed_work);
	}

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		snd_soc_dapm_stream_event(rtd,
					  codec_dai->driver->playback.stream_name,
					  SND_SOC_DAPM_STREAM_START);
	else {
		if (codec_dai->capture_active == 1)
			snd_soc_dapm_stream_event(rtd,
					  codec_dai->driver->capture.stream_name,
					  SND_SOC_DAPM_STREAM_START);
	}
	snd_soc_dai_digital_mute(codec_dai, 0);
=======
	    rtd->pop_wait) {
		rtd->pop_wait = 0;
		cancel_delayed_work(&rtd->delayed_work);
	}

	snd_soc_dapm_stream_event(rtd, substream->stream,
			SND_SOC_DAPM_STREAM_START);

	snd_soc_dai_digital_mute(codec_dai, 0, substream->stream);
>>>>>>> refs/remotes/origin/master

out:
	mutex_unlock(&rtd->pcm_mutex);
	return ret;
}

/*
 * Called by ALSA when the hardware params are set by application. This
 * function can also be called multiple times and can allocate buffers
 * (using snd_pcm_lib_* ). It's non-atomic.
 */
static int soc_pcm_hw_params(struct snd_pcm_substream *substream,
				struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_platform *platform = rtd->platform;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	int ret = 0;

	mutex_lock_nested(&rtd->pcm_mutex, rtd->pcm_subclass);

<<<<<<< HEAD
	if (rtd->dai_link->ops && rtd->dai_link->ops->hw_params) {
		ret = rtd->dai_link->ops->hw_params(substream, params);
		if (ret < 0) {
			printk(KERN_ERR "asoc: machine hw_params failed\n");
=======
	ret = soc_pcm_params_symmetry(substream, params);
	if (ret)
		goto out;

	if (rtd->dai_link->ops && rtd->dai_link->ops->hw_params) {
		ret = rtd->dai_link->ops->hw_params(substream, params);
		if (ret < 0) {
			dev_err(rtd->card->dev, "ASoC: machine hw_params"
				" failed: %d\n", ret);
>>>>>>> refs/remotes/origin/master
			goto out;
		}
	}

<<<<<<< HEAD
	if (codec_dai->driver->ops->hw_params) {
		if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
			ret = codec_dai->driver->ops->hw_params(substream,
							params, codec_dai);
			if (ret < 0) {
				printk(KERN_ERR "not set codec %s hw params\n",
					codec_dai->name);
				goto codec_err;
			}
		} else {
			if (codec_dai->capture_active == 1) {
				ret = codec_dai->driver->ops->hw_params(
						substream, params, codec_dai);
				if (ret < 0) {
					printk(KERN_ERR "fail: %s hw params\n",
						codec_dai->name);
					goto codec_err;
				}
			}
		}
	}

	if (cpu_dai->driver->ops->hw_params) {
		ret = cpu_dai->driver->ops->hw_params(substream, params, cpu_dai);
		if (ret < 0) {
			printk(KERN_ERR "asoc: interface %s hw params failed\n",
				cpu_dai->name);
=======
	if (codec_dai->driver->ops && codec_dai->driver->ops->hw_params) {
		ret = codec_dai->driver->ops->hw_params(substream, params, codec_dai);
		if (ret < 0) {
			dev_err(codec_dai->dev, "ASoC: can't set %s hw params:"
				" %d\n", codec_dai->name, ret);
			goto codec_err;
		}
	}

	if (cpu_dai->driver->ops && cpu_dai->driver->ops->hw_params) {
		ret = cpu_dai->driver->ops->hw_params(substream, params, cpu_dai);
		if (ret < 0) {
			dev_err(cpu_dai->dev, "ASoC: %s hw params failed: %d\n",
				cpu_dai->name, ret);
>>>>>>> refs/remotes/origin/master
			goto interface_err;
		}
	}

	if (platform->driver->ops && platform->driver->ops->hw_params) {
		ret = platform->driver->ops->hw_params(substream, params);
		if (ret < 0) {
<<<<<<< HEAD
			printk(KERN_ERR "asoc: platform %s hw params failed\n",
				platform->name);
=======
			dev_err(platform->dev, "ASoC: %s hw params failed: %d\n",
			       platform->name, ret);
>>>>>>> refs/remotes/origin/master
			goto platform_err;
		}
	}

<<<<<<< HEAD
	/* store the rate for each DAIs */
	cpu_dai->rate = params_rate(params);
	codec_dai->rate = params_rate(params);

	/* malloc a page for hostless IO.
	 * FIXME: rework with alsa-lib changes so that this malloc is not required.
	 */
	if (rtd->dai_link->no_host_mode == SND_SOC_DAI_LINK_NO_HOST) {
		substream->dma_buffer.dev.type = SNDRV_DMA_TYPE_DEV;
		substream->dma_buffer.dev.dev = rtd->dev;
		substream->dma_buffer.dev.dev->coherent_dma_mask = DMA_BIT_MASK(32);
		substream->dma_buffer.private_data = NULL;

		ret = snd_pcm_lib_malloc_pages(substream, PAGE_SIZE);
		if (ret < 0)
			goto platform_err;
	}
=======
	/* store the parameters for each DAIs */
	cpu_dai->rate = params_rate(params);
	cpu_dai->channels = params_channels(params);
	cpu_dai->sample_bits =
		snd_pcm_format_physical_width(params_format(params));

	codec_dai->rate = params_rate(params);
	codec_dai->channels = params_channels(params);
	codec_dai->sample_bits =
		snd_pcm_format_physical_width(params_format(params));
>>>>>>> refs/remotes/origin/master

out:
	mutex_unlock(&rtd->pcm_mutex);
	return ret;

platform_err:
<<<<<<< HEAD
	if (cpu_dai->driver->ops->hw_free)
		cpu_dai->driver->ops->hw_free(substream, cpu_dai);

interface_err:
	if (codec_dai->driver->ops->hw_free)
=======
	if (cpu_dai->driver->ops && cpu_dai->driver->ops->hw_free)
		cpu_dai->driver->ops->hw_free(substream, cpu_dai);

interface_err:
	if (codec_dai->driver->ops && codec_dai->driver->ops->hw_free)
>>>>>>> refs/remotes/origin/master
		codec_dai->driver->ops->hw_free(substream, codec_dai);

codec_err:
	if (rtd->dai_link->ops && rtd->dai_link->ops->hw_free)
		rtd->dai_link->ops->hw_free(substream);

	mutex_unlock(&rtd->pcm_mutex);
	return ret;
}

/*
 * Frees resources allocated by hw_params, can be called multiple times
 */
static int soc_pcm_hw_free(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_platform *platform = rtd->platform;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
<<<<<<< HEAD
	struct snd_soc_codec *codec = rtd->codec;

	mutex_lock_nested(&rtd->pcm_mutex, rtd->pcm_subclass);

	/* apply codec digital mute */
	if (!codec->active)
		snd_soc_dai_digital_mute(codec_dai, 1);
=======
	bool playback = substream->stream == SNDRV_PCM_STREAM_PLAYBACK;

	mutex_lock_nested(&rtd->pcm_mutex, rtd->pcm_subclass);

	/* clear the corresponding DAIs parameters when going to be inactive */
	if (cpu_dai->active == 1) {
		cpu_dai->rate = 0;
		cpu_dai->channels = 0;
		cpu_dai->sample_bits = 0;
	}

	if (codec_dai->active == 1) {
		codec_dai->rate = 0;
		codec_dai->channels = 0;
		codec_dai->sample_bits = 0;
	}

	/* apply codec digital mute */
	if ((playback && codec_dai->playback_active == 1) ||
	    (!playback && codec_dai->capture_active == 1))
		snd_soc_dai_digital_mute(codec_dai, 1, substream->stream);
>>>>>>> refs/remotes/origin/master

	/* free any machine hw params */
	if (rtd->dai_link->ops && rtd->dai_link->ops->hw_free)
		rtd->dai_link->ops->hw_free(substream);

	/* free any DMA resources */
	if (platform->driver->ops && platform->driver->ops->hw_free)
		platform->driver->ops->hw_free(substream);

	/* now free hw params for the DAIs  */
<<<<<<< HEAD
	if (codec_dai->driver->ops->hw_free)
		codec_dai->driver->ops->hw_free(substream, codec_dai);

	if (cpu_dai->driver->ops->hw_free)
		cpu_dai->driver->ops->hw_free(substream, cpu_dai);

	if (rtd->dai_link->no_host_mode == SND_SOC_DAI_LINK_NO_HOST)
		snd_pcm_lib_free_pages(substream);

=======
	if (codec_dai->driver->ops && codec_dai->driver->ops->hw_free)
		codec_dai->driver->ops->hw_free(substream, codec_dai);

	if (cpu_dai->driver->ops && cpu_dai->driver->ops->hw_free)
		cpu_dai->driver->ops->hw_free(substream, cpu_dai);

>>>>>>> refs/remotes/origin/master
	mutex_unlock(&rtd->pcm_mutex);
	return 0;
}

static int soc_pcm_trigger(struct snd_pcm_substream *substream, int cmd)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_platform *platform = rtd->platform;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	int ret;

<<<<<<< HEAD
	if (codec_dai->driver->ops->trigger) {
		if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
			ret = codec_dai->driver->ops->trigger(substream,
						cmd, codec_dai);
			if (ret < 0)
				return ret;
		} else {
			if (codec_dai->capture_active == 1) {
				ret = codec_dai->driver->ops->trigger(
						substream, cmd, codec_dai);
				if (ret < 0)
					return ret;
			}
		}
=======
	if (codec_dai->driver->ops && codec_dai->driver->ops->trigger) {
		ret = codec_dai->driver->ops->trigger(substream, cmd, codec_dai);
		if (ret < 0)
			return ret;
>>>>>>> refs/remotes/origin/master
	}

	if (platform->driver->ops && platform->driver->ops->trigger) {
		ret = platform->driver->ops->trigger(substream, cmd);
		if (ret < 0)
			return ret;
	}

<<<<<<< HEAD
	if (cpu_dai->driver->ops->trigger) {
=======
	if (cpu_dai->driver->ops && cpu_dai->driver->ops->trigger) {
>>>>>>> refs/remotes/origin/master
		ret = cpu_dai->driver->ops->trigger(substream, cmd, cpu_dai);
		if (ret < 0)
			return ret;
	}
	return 0;
}

<<<<<<< HEAD
int soc_pcm_bespoke_trigger(struct snd_pcm_substream *substream, int cmd)
=======
static int soc_pcm_bespoke_trigger(struct snd_pcm_substream *substream,
				   int cmd)
>>>>>>> refs/remotes/origin/master
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_platform *platform = rtd->platform;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	int ret;

<<<<<<< HEAD
	if (codec_dai->driver->ops->bespoke_trigger) {
=======
	if (codec_dai->driver->ops &&
	    codec_dai->driver->ops->bespoke_trigger) {
>>>>>>> refs/remotes/origin/master
		ret = codec_dai->driver->ops->bespoke_trigger(substream, cmd, codec_dai);
		if (ret < 0)
			return ret;
	}

	if (platform->driver->bespoke_trigger) {
		ret = platform->driver->bespoke_trigger(substream, cmd);
		if (ret < 0)
			return ret;
	}

<<<<<<< HEAD
	if (cpu_dai->driver->ops->bespoke_trigger) {
=======
	if (cpu_dai->driver->ops && cpu_dai->driver->ops->bespoke_trigger) {
>>>>>>> refs/remotes/origin/master
		ret = cpu_dai->driver->ops->bespoke_trigger(substream, cmd, cpu_dai);
		if (ret < 0)
			return ret;
	}
	return 0;
}
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
/*
 * soc level wrapper for pointer callback
 * If cpu_dai, codec_dai, platform driver has the delay callback, than
 * the runtime->delay will be updated accordingly.
 */
static snd_pcm_uframes_t soc_pcm_pointer(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_platform *platform = rtd->platform;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	struct snd_pcm_runtime *runtime = substream->runtime;
	snd_pcm_uframes_t offset = 0;
	snd_pcm_sframes_t delay = 0;

	if (platform->driver->ops && platform->driver->ops->pointer)
		offset = platform->driver->ops->pointer(substream);

<<<<<<< HEAD
	if (cpu_dai->driver->ops->delay)
		delay += cpu_dai->driver->ops->delay(substream, cpu_dai);

	if (codec_dai->driver->ops->delay)
=======
	if (cpu_dai->driver->ops && cpu_dai->driver->ops->delay)
		delay += cpu_dai->driver->ops->delay(substream, cpu_dai);

	if (codec_dai->driver->ops && codec_dai->driver->ops->delay)
>>>>>>> refs/remotes/origin/master
		delay += codec_dai->driver->ops->delay(substream, codec_dai);

	if (platform->driver->delay)
		delay += platform->driver->delay(substream, codec_dai);

	runtime->delay = delay;

	return offset;
}

<<<<<<< HEAD
static inline int be_connect(struct snd_soc_pcm_runtime *fe,
		struct snd_soc_pcm_runtime *be, int stream)
{
	struct snd_soc_dpcm_params *dpcm_params;

	if (!fe->dpcm[stream].runtime) {
		dev_err(fe->dev, "%s no runtime\n", fe->dai_link->name);
		return -ENODEV;
	}

	/* only add new dpcm_paramss */
	list_for_each_entry(dpcm_params, &fe->dpcm[stream].be_clients, list_be) {
		if (dpcm_params->be == be && dpcm_params->fe == fe)
			return 0;
	}

	dpcm_params = kzalloc(sizeof(struct snd_soc_dpcm_params), GFP_KERNEL);
	if (!dpcm_params)
		return -ENOMEM;

	dpcm_params->be = be;
	dpcm_params->fe = fe;
	be->dpcm[stream].runtime = fe->dpcm[stream].runtime;
	dpcm_params->state = SND_SOC_DPCM_LINK_STATE_NEW;
	list_add(&dpcm_params->list_be, &fe->dpcm[stream].be_clients);
	list_add(&dpcm_params->list_fe, &be->dpcm[stream].fe_clients);

	dev_dbg(fe->dev, "  connected new DSP %s path %s %s %s\n",
=======
/* connect a FE and BE */
static int dpcm_be_connect(struct snd_soc_pcm_runtime *fe,
		struct snd_soc_pcm_runtime *be, int stream)
{
	struct snd_soc_dpcm *dpcm;

	/* only add new dpcms */
	list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be) {
		if (dpcm->be == be && dpcm->fe == fe)
			return 0;
	}

	dpcm = kzalloc(sizeof(struct snd_soc_dpcm), GFP_KERNEL);
	if (!dpcm)
		return -ENOMEM;

	dpcm->be = be;
	dpcm->fe = fe;
	be->dpcm[stream].runtime = fe->dpcm[stream].runtime;
	dpcm->state = SND_SOC_DPCM_LINK_STATE_NEW;
	list_add(&dpcm->list_be, &fe->dpcm[stream].be_clients);
	list_add(&dpcm->list_fe, &be->dpcm[stream].fe_clients);

	dev_dbg(fe->dev, "connected new DPCM %s path %s %s %s\n",
>>>>>>> refs/remotes/origin/master
			stream ? "capture" : "playback",  fe->dai_link->name,
			stream ? "<-" : "->", be->dai_link->name);

#ifdef CONFIG_DEBUG_FS
<<<<<<< HEAD
	dpcm_params->debugfs_state = debugfs_create_u32(be->dai_link->name, 0644,
			fe->debugfs_dpcm_root, &dpcm_params->state);
#endif

	return 1;
}

static inline void be_reparent(struct snd_soc_pcm_runtime *fe,
			struct snd_soc_pcm_runtime *be, int stream)
{
	struct snd_soc_dpcm_params *dpcm_params;
=======
	dpcm->debugfs_state = debugfs_create_u32(be->dai_link->name, 0644,
			fe->debugfs_dpcm_root, &dpcm->state);
#endif
	return 1;
}

/* reparent a BE onto another FE */
static void dpcm_be_reparent(struct snd_soc_pcm_runtime *fe,
			struct snd_soc_pcm_runtime *be, int stream)
{
	struct snd_soc_dpcm *dpcm;
>>>>>>> refs/remotes/origin/master
	struct snd_pcm_substream *fe_substream, *be_substream;

	/* reparent if BE is connected to other FEs */
	if (!be->dpcm[stream].users)
		return;

	be_substream = snd_soc_dpcm_get_substream(be, stream);

<<<<<<< HEAD
	list_for_each_entry(dpcm_params, &be->dpcm[stream].fe_clients, list_fe) {
		if (dpcm_params->fe != fe) {

			dev_dbg(fe->dev, "  reparent %s path %s %s %s\n",
					stream ? "capture" : "playback",
					dpcm_params->fe->dai_link->name,
					stream ? "<-" : "->", dpcm_params->be->dai_link->name);

			fe_substream = snd_soc_dpcm_get_substream(dpcm_params->fe,
								stream);
			be_substream->runtime = fe_substream->runtime;
			break;
		}
	}
}

static inline void be_disconnect(struct snd_soc_pcm_runtime *fe, int stream)
{
	struct snd_soc_dpcm_params *dpcm_params, *d;

	list_for_each_entry_safe(dpcm_params, d, &fe->dpcm[stream].be_clients, list_be) {
		dev_dbg(fe->dev, "BE %s disconnect check for %s\n",
				stream ? "capture" : "playback",
				dpcm_params->be->dai_link->name);

		if (dpcm_params->state == SND_SOC_DPCM_LINK_STATE_FREE) {
			dev_dbg(fe->dev, "  freed DSP %s path %s %s %s\n",
					stream ? "capture" : "playback", fe->dai_link->name,
					stream ? "<-" : "->", dpcm_params->be->dai_link->name);

			/* BEs still alive need new FE */
			be_reparent(fe, dpcm_params->be, stream);

#ifdef CONFIG_DEBUG_FS
			debugfs_remove(dpcm_params->debugfs_state);
#endif

			list_del(&dpcm_params->list_be);
			list_del(&dpcm_params->list_fe);
			kfree(dpcm_params);
		}
	}
}

static struct snd_soc_pcm_runtime *be_get_rtd(struct snd_soc_card *card,
		struct snd_soc_dapm_widget *widget)
=======
	list_for_each_entry(dpcm, &be->dpcm[stream].fe_clients, list_fe) {
		if (dpcm->fe == fe)
			continue;

		dev_dbg(fe->dev, "reparent %s path %s %s %s\n",
			stream ? "capture" : "playback",
			dpcm->fe->dai_link->name,
			stream ? "<-" : "->", dpcm->be->dai_link->name);

		fe_substream = snd_soc_dpcm_get_substream(dpcm->fe, stream);
		be_substream->runtime = fe_substream->runtime;
		break;
	}
}

/* disconnect a BE and FE */
void dpcm_be_disconnect(struct snd_soc_pcm_runtime *fe, int stream)
{
	struct snd_soc_dpcm *dpcm, *d;

	list_for_each_entry_safe(dpcm, d, &fe->dpcm[stream].be_clients, list_be) {
		dev_dbg(fe->dev, "ASoC: BE %s disconnect check for %s\n",
				stream ? "capture" : "playback",
				dpcm->be->dai_link->name);

		if (dpcm->state != SND_SOC_DPCM_LINK_STATE_FREE)
			continue;

		dev_dbg(fe->dev, "freed DSP %s path %s %s %s\n",
			stream ? "capture" : "playback", fe->dai_link->name,
			stream ? "<-" : "->", dpcm->be->dai_link->name);

		/* BEs still alive need new FE */
		dpcm_be_reparent(fe, dpcm->be, stream);

#ifdef CONFIG_DEBUG_FS
		debugfs_remove(dpcm->debugfs_state);
#endif
		list_del(&dpcm->list_be);
		list_del(&dpcm->list_fe);
		kfree(dpcm);
	}
}

/* get BE for DAI widget and stream */
static struct snd_soc_pcm_runtime *dpcm_get_be(struct snd_soc_card *card,
		struct snd_soc_dapm_widget *widget, int stream)
>>>>>>> refs/remotes/origin/master
{
	struct snd_soc_pcm_runtime *be;
	int i;

<<<<<<< HEAD
	if (!widget->sname) {
		dev_err(card->dev, "widget %s has no stream\n", widget->name);
		return NULL;
	}

	for (i = 0; i < card->num_links; i++) {
		be = &card->rtd[i];

		if (!strcmp(widget->sname, be->dai_link->stream_name))
			return be;
	}

	dev_err(card->dev, "can't get BE for %s\n", widget->name);
	return NULL;
}

static struct snd_soc_dapm_widget *be_get_widget(struct snd_soc_card *card,
		struct snd_soc_pcm_runtime *rtd)
{
	struct snd_soc_dapm_widget *widget;

	list_for_each_entry(widget, &card->widgets, list) {

		if (!widget->sname)
			continue;

		if (!strcmp(widget->sname, rtd->dai_link->stream_name))
			return widget;
	}

	dev_err(card->dev, "can't get widget for %s\n",
			rtd->dai_link->stream_name);
	return NULL;
=======
	if (stream == SNDRV_PCM_STREAM_PLAYBACK) {
		for (i = 0; i < card->num_links; i++) {
			be = &card->rtd[i];

			if (!be->dai_link->no_pcm)
				continue;

			if (be->cpu_dai->playback_widget == widget ||
				be->codec_dai->playback_widget == widget)
				return be;
		}
	} else {

		for (i = 0; i < card->num_links; i++) {
			be = &card->rtd[i];

			if (!be->dai_link->no_pcm)
				continue;

			if (be->cpu_dai->capture_widget == widget ||
				be->codec_dai->capture_widget == widget)
				return be;
		}
	}

	dev_err(card->dev, "ASoC: can't get %s BE for %s\n",
		stream ? "capture" : "playback", widget->name);
	return NULL;
}

static inline struct snd_soc_dapm_widget *
	rtd_get_cpu_widget(struct snd_soc_pcm_runtime *rtd, int stream)
{
	if (stream == SNDRV_PCM_STREAM_PLAYBACK)
		return rtd->cpu_dai->playback_widget;
	else
		return rtd->cpu_dai->capture_widget;
}

static inline struct snd_soc_dapm_widget *
	rtd_get_codec_widget(struct snd_soc_pcm_runtime *rtd, int stream)
{
	if (stream == SNDRV_PCM_STREAM_PLAYBACK)
		return rtd->codec_dai->playback_widget;
	else
		return rtd->codec_dai->capture_widget;
>>>>>>> refs/remotes/origin/master
}

static int widget_in_list(struct snd_soc_dapm_widget_list *list,
		struct snd_soc_dapm_widget *widget)
{
	int i;

	for (i = 0; i < list->num_widgets; i++) {
<<<<<<< HEAD
			if (widget == list->widgets[i])
				return 1;
=======
		if (widget == list->widgets[i])
			return 1;
>>>>>>> refs/remotes/origin/master
	}

	return 0;
}

<<<<<<< HEAD
static int fe_path_get(struct snd_soc_pcm_runtime *fe,
=======
int dpcm_path_get(struct snd_soc_pcm_runtime *fe,
>>>>>>> refs/remotes/origin/master
	int stream, struct snd_soc_dapm_widget_list **list_)
{
	struct snd_soc_dai *cpu_dai = fe->cpu_dai;
	struct snd_soc_dapm_widget_list *list;
	int paths;

	list = kzalloc(sizeof(struct snd_soc_dapm_widget_list) +
			sizeof(struct snd_soc_dapm_widget *), GFP_KERNEL);
	if (list == NULL)
		return -ENOMEM;

	/* get number of valid DAI paths and their widgets */
	paths = snd_soc_dapm_dai_get_connected_widgets(cpu_dai, stream, &list);

<<<<<<< HEAD
	dev_dbg(fe->dev, "found %d audio %s paths\n", paths,
=======
	dev_dbg(fe->dev, "ASoC: found %d audio %s paths\n", paths,
>>>>>>> refs/remotes/origin/master
			stream ? "capture" : "playback");

	*list_ = list;
	return paths;
}

<<<<<<< HEAD
static inline void fe_path_put(struct snd_soc_dapm_widget_list **list)
{
	kfree(*list);
}

static int be_prune_old(struct snd_soc_pcm_runtime *fe, int stream,
	struct snd_soc_dapm_widget_list **list_)
{
	struct snd_soc_card *card = fe->card;
	struct snd_soc_dpcm_params *dpcm_params;
	struct snd_soc_dapm_widget_list *list = *list_;
	struct snd_soc_dapm_widget *widget;
	int old = 0;

	/* Destroy any old FE <--> BE connections */
	list_for_each_entry(dpcm_params, &fe->dpcm[stream].be_clients, list_be) {

		/* is there a valid widget for this BE */
		widget = be_get_widget(card, dpcm_params->be);
		if (!widget) {
			dev_err(fe->dev, "no widget found for %s\n",
					dpcm_params->be->dai_link->name);
			continue;
		}

		/* prune the BE if it's no longer in our active list */
		if (widget_in_list(list, widget))
			continue;

		dev_dbg(fe->dev, "pruning %s BE %s for %s\n",
			stream ? "capture" : "playback", dpcm_params->be->dai_link->name,
			fe->dai_link->name);
		dpcm_params->state = SND_SOC_DPCM_LINK_STATE_FREE;
		dpcm_params->be->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_BE;
		old++;
	}

	dev_dbg(fe->dev, "found %d old BEs\n", old);
	return old;
}

static int be_add_new(struct snd_soc_pcm_runtime *fe, int stream,
=======
static int dpcm_prune_paths(struct snd_soc_pcm_runtime *fe, int stream,
	struct snd_soc_dapm_widget_list **list_)
{
	struct snd_soc_dpcm *dpcm;
	struct snd_soc_dapm_widget_list *list = *list_;
	struct snd_soc_dapm_widget *widget;
	int prune = 0;

	/* Destroy any old FE <--> BE connections */
	list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be) {

		/* is there a valid CPU DAI widget for this BE */
		widget = rtd_get_cpu_widget(dpcm->be, stream);

		/* prune the BE if it's no longer in our active list */
		if (widget && widget_in_list(list, widget))
			continue;

		/* is there a valid CODEC DAI widget for this BE */
		widget = rtd_get_codec_widget(dpcm->be, stream);

		/* prune the BE if it's no longer in our active list */
		if (widget && widget_in_list(list, widget))
			continue;

		dev_dbg(fe->dev, "ASoC: pruning %s BE %s for %s\n",
			stream ? "capture" : "playback",
			dpcm->be->dai_link->name, fe->dai_link->name);
		dpcm->state = SND_SOC_DPCM_LINK_STATE_FREE;
		dpcm->be->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_BE;
		prune++;
	}

	dev_dbg(fe->dev, "ASoC: found %d old BE paths for pruning\n", prune);
	return prune;
}

static int dpcm_add_paths(struct snd_soc_pcm_runtime *fe, int stream,
>>>>>>> refs/remotes/origin/master
	struct snd_soc_dapm_widget_list **list_)
{
	struct snd_soc_card *card = fe->card;
	struct snd_soc_dapm_widget_list *list = *list_;
<<<<<<< HEAD
	enum snd_soc_dapm_type be_type;
	int i, new = 0, err;

	if (stream == SNDRV_PCM_STREAM_PLAYBACK)
		be_type = snd_soc_dapm_aif_out;
	else
		be_type = snd_soc_dapm_aif_in;

	/* Create any new FE <--> BE connections */
	for (i = 0; i < list->num_widgets; i++) {

		if (list->widgets[i]->id == be_type) {
			struct snd_soc_pcm_runtime *be;

			/* is there a valid BE rtd for this widget */
			be = be_get_rtd(card, list->widgets[i]);
			if (!be) {
				dev_err(fe->dev, "no BE found for %s\n",
						list->widgets[i]->name);
				continue;
			}

			/* don't connect if FE is not running */
			if (!fe->dpcm[stream].runtime)
				continue;

			/* newly connected FE and BE */
			err = be_connect(fe, be, stream);
			if (err < 0) {
				dev_err(fe->dev, "can't connect %s\n", list->widgets[i]->name);
				break;
			} else if (err == 0) /* already connected */
				continue;

			/* new */
			be->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_BE;
			new++;
		}
	}

	dev_dbg(fe->dev, "found %d new BEs\n", new);
=======
	struct snd_soc_pcm_runtime *be;
	int i, new = 0, err;

	/* Create any new FE <--> BE connections */
	for (i = 0; i < list->num_widgets; i++) {

		switch (list->widgets[i]->id) {
		case snd_soc_dapm_dai_in:
		case snd_soc_dapm_dai_out:
			break;
		default:
			continue;
		}

		/* is there a valid BE rtd for this widget */
		be = dpcm_get_be(card, list->widgets[i], stream);
		if (!be) {
			dev_err(fe->dev, "ASoC: no BE found for %s\n",
					list->widgets[i]->name);
			continue;
		}

		/* make sure BE is a real BE */
		if (!be->dai_link->no_pcm)
			continue;

		/* don't connect if FE is not running */
		if (!fe->dpcm[stream].runtime && !fe->fe_compr)
			continue;

		/* newly connected FE and BE */
		err = dpcm_be_connect(fe, be, stream);
		if (err < 0) {
			dev_err(fe->dev, "ASoC: can't connect %s\n",
				list->widgets[i]->name);
			break;
		} else if (err == 0) /* already connected */
			continue;

		/* new */
		be->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_BE;
		new++;
	}

	dev_dbg(fe->dev, "ASoC: found %d new BE paths\n", new);
>>>>>>> refs/remotes/origin/master
	return new;
}

/*
 * Find the corresponding BE DAIs that source or sink audio to this
 * FE substream.
 */
<<<<<<< HEAD
static int dpcm_process_paths(struct snd_soc_pcm_runtime *fe,
	int stream, struct snd_soc_dapm_widget_list **list, int new)
{
	if (new)
		return be_add_new(fe, stream, list);
	else
		return be_prune_old(fe, stream, list);
	return 0;
}

/*
 * Clear the runtime pending state of all BE's.
 */
static void fe_clear_pending(struct snd_soc_pcm_runtime *fe, int stream)
{
	struct snd_soc_dpcm_params *dpcm_params;

	list_for_each_entry(dpcm_params, &fe->dpcm[stream].be_clients, list_be)
		dpcm_params->be->dpcm[stream].runtime_update =
						SND_SOC_DPCM_UPDATE_NO;
}

/* Unwind the BE startup */
static void soc_dpcm_be_dai_startup_unwind(struct snd_soc_pcm_runtime *fe, int stream)
{
	struct snd_soc_dpcm_params *dpcm_params;

	/* disable any enabled and non active backends */
	list_for_each_entry(dpcm_params, &fe->dpcm[stream].be_clients, list_be) {

		struct snd_soc_pcm_runtime *be = dpcm_params->be;
=======
int dpcm_process_paths(struct snd_soc_pcm_runtime *fe,
	int stream, struct snd_soc_dapm_widget_list **list, int new)
{
	if (new)
		return dpcm_add_paths(fe, stream, list);
	else
		return dpcm_prune_paths(fe, stream, list);
}

void dpcm_clear_pending_state(struct snd_soc_pcm_runtime *fe, int stream)
{
	struct snd_soc_dpcm *dpcm;

	list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be)
		dpcm->be->dpcm[stream].runtime_update =
						SND_SOC_DPCM_UPDATE_NO;
}

static void dpcm_be_dai_startup_unwind(struct snd_soc_pcm_runtime *fe,
	int stream)
{
	struct snd_soc_dpcm *dpcm;

	/* disable any enabled and non active backends */
	list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be) {

		struct snd_soc_pcm_runtime *be = dpcm->be;
>>>>>>> refs/remotes/origin/master
		struct snd_pcm_substream *be_substream =
			snd_soc_dpcm_get_substream(be, stream);

		if (be->dpcm[stream].users == 0)
<<<<<<< HEAD
			dev_err(be->dev, "no users %s at close - state %d\n",
				stream ? "capture" : "playback", be->dpcm[stream].state);
=======
			dev_err(be->dev, "ASoC: no users %s at close - state %d\n",
				stream ? "capture" : "playback",
				be->dpcm[stream].state);
>>>>>>> refs/remotes/origin/master

		if (--be->dpcm[stream].users != 0)
			continue;

		if (be->dpcm[stream].state != SND_SOC_DPCM_STATE_OPEN)
			continue;

		soc_pcm_close(be_substream);
		be_substream->runtime = NULL;
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
		be->dpcm[stream].state = SND_SOC_DPCM_STATE_CLOSE;
	}
}

<<<<<<< HEAD
/* Startup all new BE */
static int soc_dpcm_be_dai_startup(struct snd_soc_pcm_runtime *fe, int stream)
{
	struct snd_soc_dpcm_params *dpcm_params;
	int err, count = 0;

	/* only startup BE DAIs that are either sinks or sources to this FE DAI */
	list_for_each_entry(dpcm_params, &fe->dpcm[stream].be_clients, list_be) {

		struct snd_soc_pcm_runtime *be = dpcm_params->be;
		struct snd_pcm_substream *be_substream =
			snd_soc_dpcm_get_substream(be, stream);

=======
int dpcm_be_dai_startup(struct snd_soc_pcm_runtime *fe, int stream)
{
	struct snd_soc_dpcm *dpcm;
	int err, count = 0;

	/* only startup BE DAIs that are either sinks or sources to this FE DAI */
	list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be) {

		struct snd_soc_pcm_runtime *be = dpcm->be;
		struct snd_pcm_substream *be_substream =
			snd_soc_dpcm_get_substream(be, stream);

		if (!be_substream) {
			dev_err(be->dev, "ASoC: no backend %s stream\n",
				stream ? "capture" : "playback");
			continue;
		}

>>>>>>> refs/remotes/origin/master
		/* is this op for this BE ? */
		if (!snd_soc_dpcm_be_can_update(fe, be, stream))
			continue;

<<<<<<< HEAD
		/* first time the dpcm_params is open ? */
		if (be->dpcm[stream].users == MAX_BE_USERS)
			dev_err(be->dev, "too many users %s at open - state %d\n",
				stream ? "capture" : "playback", be->dpcm[stream].state);
=======
		/* first time the dpcm is open ? */
		if (be->dpcm[stream].users == DPCM_MAX_BE_USERS)
			dev_err(be->dev, "ASoC: too many users %s at open %d\n",
				stream ? "capture" : "playback",
				be->dpcm[stream].state);
>>>>>>> refs/remotes/origin/master

		if (be->dpcm[stream].users++ != 0)
			continue;

		if ((be->dpcm[stream].state != SND_SOC_DPCM_STATE_NEW) &&
		    (be->dpcm[stream].state != SND_SOC_DPCM_STATE_CLOSE))
			continue;

<<<<<<< HEAD
		dev_dbg(be->dev, "dpcm: open BE %s\n", be->dai_link->name);
=======
		dev_dbg(be->dev, "ASoC: open %s BE %s\n",
			stream ? "capture" : "playback", be->dai_link->name);
>>>>>>> refs/remotes/origin/master

		be_substream->runtime = be->dpcm[stream].runtime;
		err = soc_pcm_open(be_substream);
		if (err < 0) {
<<<<<<< HEAD
			dev_err(be->dev, "BE open failed %d\n", err);
			be->dpcm[stream].users--;
			if (be->dpcm[stream].users < 0)
				dev_err(be->dev, "no users %s at unwind - state %d\n",
						stream ? "capture" : "playback",
						be->dpcm[stream].state);
=======
			dev_err(be->dev, "ASoC: BE open failed %d\n", err);
			be->dpcm[stream].users--;
			if (be->dpcm[stream].users < 0)
				dev_err(be->dev, "ASoC: no users %s at unwind %d\n",
					stream ? "capture" : "playback",
					be->dpcm[stream].state);
>>>>>>> refs/remotes/origin/master

			be->dpcm[stream].state = SND_SOC_DPCM_STATE_CLOSE;
			goto unwind;
		}

		be->dpcm[stream].state = SND_SOC_DPCM_STATE_OPEN;
		count++;
	}

	return count;

unwind:
	/* disable any enabled and non active backends */
<<<<<<< HEAD
	list_for_each_entry_continue_reverse(dpcm_params, &fe->dpcm[stream].be_clients, list_be) {
		struct snd_soc_pcm_runtime *be = dpcm_params->be;
=======
	list_for_each_entry_continue_reverse(dpcm, &fe->dpcm[stream].be_clients, list_be) {
		struct snd_soc_pcm_runtime *be = dpcm->be;
>>>>>>> refs/remotes/origin/master
		struct snd_pcm_substream *be_substream =
			snd_soc_dpcm_get_substream(be, stream);

		if (!snd_soc_dpcm_be_can_update(fe, be, stream))
			continue;

		if (be->dpcm[stream].users == 0)
<<<<<<< HEAD
			dev_err(be->dev, "no users %s at close - state %d\n",
				stream ? "capture" : "playback", be->dpcm[stream].state);
=======
			dev_err(be->dev, "ASoC: no users %s at close %d\n",
				stream ? "capture" : "playback",
				be->dpcm[stream].state);
>>>>>>> refs/remotes/origin/master

		if (--be->dpcm[stream].users != 0)
			continue;

		if (be->dpcm[stream].state != SND_SOC_DPCM_STATE_OPEN)
			continue;

		soc_pcm_close(be_substream);
		be_substream->runtime = NULL;
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
		be->dpcm[stream].state = SND_SOC_DPCM_STATE_CLOSE;
	}

	return err;
}

<<<<<<< HEAD
void soc_dpcm_set_dynamic_runtime(struct snd_pcm_substream *substream)
=======
static void dpcm_init_runtime_hw(struct snd_pcm_runtime *runtime,
	struct snd_soc_pcm_stream *stream)
{
	runtime->hw.rate_min = stream->rate_min;
	runtime->hw.rate_max = stream->rate_max;
	runtime->hw.channels_min = stream->channels_min;
	runtime->hw.channels_max = stream->channels_max;
	if (runtime->hw.formats)
		runtime->hw.formats &= stream->formats;
	else
		runtime->hw.formats = stream->formats;
	runtime->hw.rates = stream->rates;
}

static void dpcm_set_fe_runtime(struct snd_pcm_substream *substream)
>>>>>>> refs/remotes/origin/master
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	struct snd_soc_dai_driver *cpu_dai_drv = cpu_dai->driver;

<<<<<<< HEAD
	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
		runtime->hw.rate_min = cpu_dai_drv->playback.rate_min;
		runtime->hw.rate_max = cpu_dai_drv->playback.rate_max;
		runtime->hw.channels_min = cpu_dai_drv->playback.channels_min;
		runtime->hw.channels_max = cpu_dai_drv->playback.channels_max;
		runtime->hw.formats &= cpu_dai_drv->playback.formats;
		runtime->hw.rates = cpu_dai_drv->playback.rates;
	} else {
		runtime->hw.rate_min = cpu_dai_drv->capture.rate_min;
		runtime->hw.rate_max = cpu_dai_drv->capture.rate_max;
		runtime->hw.channels_min = cpu_dai_drv->capture.channels_min;
		runtime->hw.channels_max = cpu_dai_drv->capture.channels_max;
		runtime->hw.formats &= cpu_dai_drv->capture.formats;
		runtime->hw.rates = cpu_dai_drv->capture.rates;
	}
}

static int soc_dpcm_fe_dai_startup(struct snd_pcm_substream *fe_substream)
=======
	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		dpcm_init_runtime_hw(runtime, &cpu_dai_drv->playback);
	else
		dpcm_init_runtime_hw(runtime, &cpu_dai_drv->capture);
}

static int dpcm_fe_dai_startup(struct snd_pcm_substream *fe_substream)
>>>>>>> refs/remotes/origin/master
{
	struct snd_soc_pcm_runtime *fe = fe_substream->private_data;
	struct snd_pcm_runtime *runtime = fe_substream->runtime;
	int stream = fe_substream->stream, ret = 0;

<<<<<<< HEAD
	mutex_lock(&fe->card->dpcm_mutex);
	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_FE;

	ret = soc_dpcm_be_dai_startup(fe, fe_substream->stream);
	if (ret < 0) {
		dev_err(fe->dev,"dpcm: failed to start some BEs %d\n", ret);
		goto be_err;
	}

	dev_dbg(fe->dev, "dpcm: open FE %s\n", fe->dai_link->name);
=======
	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_FE;

	ret = dpcm_be_dai_startup(fe, fe_substream->stream);
	if (ret < 0) {
		dev_err(fe->dev,"ASoC: failed to start some BEs %d\n", ret);
		goto be_err;
	}

	dev_dbg(fe->dev, "ASoC: open FE %s\n", fe->dai_link->name);
>>>>>>> refs/remotes/origin/master

	/* start the DAI frontend */
	ret = soc_pcm_open(fe_substream);
	if (ret < 0) {
<<<<<<< HEAD
		dev_err(fe->dev,"dpcm: failed to start FE %d\n", ret);
=======
		dev_err(fe->dev,"ASoC: failed to start FE %d\n", ret);
>>>>>>> refs/remotes/origin/master
		goto unwind;
	}

	fe->dpcm[stream].state = SND_SOC_DPCM_STATE_OPEN;

<<<<<<< HEAD
	soc_dpcm_set_dynamic_runtime(fe_substream);
	snd_pcm_limit_hw_rates(runtime);

	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_NO;
	mutex_unlock(&fe->card->dpcm_mutex);
	return 0;

unwind:
	soc_dpcm_be_dai_startup_unwind(fe, fe_substream->stream);
be_err:
	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_NO;
	mutex_unlock(&fe->card->dpcm_mutex);
	return ret;
}

/* BE shutdown - called on DAPM sync updates (i.e. FE is already running)*/
static int soc_dpcm_be_dai_shutdown(struct snd_soc_pcm_runtime *fe, int stream)
{
	struct snd_soc_dpcm_params *dpcm_params;

	/* only shutdown backends that are either sinks or sources to this frontend DAI */
	list_for_each_entry(dpcm_params, &fe->dpcm[stream].be_clients, list_be) {

		struct snd_soc_pcm_runtime *be = dpcm_params->be;
=======
	dpcm_set_fe_runtime(fe_substream);
	snd_pcm_limit_hw_rates(runtime);

	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_NO;
	return 0;

unwind:
	dpcm_be_dai_startup_unwind(fe, fe_substream->stream);
be_err:
	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_NO;
	return ret;
}

int dpcm_be_dai_shutdown(struct snd_soc_pcm_runtime *fe, int stream)
{
	struct snd_soc_dpcm *dpcm;

	/* only shutdown BEs that are either sinks or sources to this FE DAI */
	list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be) {

		struct snd_soc_pcm_runtime *be = dpcm->be;
>>>>>>> refs/remotes/origin/master
		struct snd_pcm_substream *be_substream =
			snd_soc_dpcm_get_substream(be, stream);

		/* is this op for this BE ? */
		if (!snd_soc_dpcm_be_can_update(fe, be, stream))
			continue;

		if (be->dpcm[stream].users == 0)
<<<<<<< HEAD
			dev_err(be->dev, "no users %s at close - state %d\n",
				stream ? "capture" : "playback", be->dpcm[stream].state);
=======
			dev_err(be->dev, "ASoC: no users %s at close - state %d\n",
				stream ? "capture" : "playback",
				be->dpcm[stream].state);
>>>>>>> refs/remotes/origin/master

		if (--be->dpcm[stream].users != 0)
			continue;

		if ((be->dpcm[stream].state != SND_SOC_DPCM_STATE_HW_FREE) &&
		    (be->dpcm[stream].state != SND_SOC_DPCM_STATE_OPEN))
			continue;

<<<<<<< HEAD
		dev_dbg(be->dev, "dpcm: close BE %s\n",
			dpcm_params->fe->dai_link->name);
=======
		dev_dbg(be->dev, "ASoC: close BE %s\n",
			dpcm->fe->dai_link->name);
>>>>>>> refs/remotes/origin/master

		soc_pcm_close(be_substream);
		be_substream->runtime = NULL;

		be->dpcm[stream].state = SND_SOC_DPCM_STATE_CLOSE;
	}
	return 0;
}

<<<<<<< HEAD
/* FE +BE shutdown - called on FE PCM ops */
static int soc_dpcm_fe_dai_shutdown(struct snd_pcm_substream *substream)
=======
static int dpcm_fe_dai_shutdown(struct snd_pcm_substream *substream)
>>>>>>> refs/remotes/origin/master
{
	struct snd_soc_pcm_runtime *fe = substream->private_data;
	int stream = substream->stream;

<<<<<<< HEAD
	mutex_lock(&fe->card->dpcm_mutex);
	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_FE;
	
	dev_dbg(fe->dev, "dpcm: close FE %s\n", fe->dai_link->name);
=======
	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_FE;

	/* shutdown the BEs */
	dpcm_be_dai_shutdown(fe, substream->stream);

	dev_dbg(fe->dev, "ASoC: close FE %s\n", fe->dai_link->name);
>>>>>>> refs/remotes/origin/master

	/* now shutdown the frontend */
	soc_pcm_close(substream);

<<<<<<< HEAD
	/* shutdown the BEs */
	soc_dpcm_be_dai_shutdown(fe, substream->stream);
	/* run the stream event for each BE */
	if (stream == SNDRV_PCM_STREAM_PLAYBACK)
		soc_dpcm_dapm_stream_event(fe, stream,
				fe->cpu_dai->driver->playback.stream_name,
				SND_SOC_DAPM_STREAM_STOP);
	else
		soc_dpcm_dapm_stream_event(fe, stream,
				fe->cpu_dai->driver->capture.stream_name,
				SND_SOC_DAPM_STREAM_STOP);

	fe->dpcm[stream].state = SND_SOC_DPCM_STATE_CLOSE;
	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_NO;

	mutex_unlock(&fe->card->dpcm_mutex);
	return 0;
}

static int soc_dpcm_be_dai_hw_params(struct snd_soc_pcm_runtime *fe, int stream)
{
	struct snd_soc_dpcm_params *dpcm_params;
	int ret;

	list_for_each_entry(dpcm_params, &fe->dpcm[stream].be_clients, list_be) {

		struct snd_soc_pcm_runtime *be = dpcm_params->be;
=======
	/* run the stream event for each BE */
	dpcm_dapm_stream_event(fe, stream, SND_SOC_DAPM_STREAM_STOP);

	fe->dpcm[stream].state = SND_SOC_DPCM_STATE_CLOSE;
	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_NO;
	return 0;
}

int dpcm_be_dai_hw_free(struct snd_soc_pcm_runtime *fe, int stream)
{
	struct snd_soc_dpcm *dpcm;

	/* only hw_params backends that are either sinks or sources
	 * to this frontend DAI */
	list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be) {

		struct snd_soc_pcm_runtime *be = dpcm->be;
>>>>>>> refs/remotes/origin/master
		struct snd_pcm_substream *be_substream =
			snd_soc_dpcm_get_substream(be, stream);

		/* is this op for this BE ? */
		if (!snd_soc_dpcm_be_can_update(fe, be, stream))
			continue;

<<<<<<< HEAD
		/* only allow hw_params() if no connected FEs are running */
		if (!snd_soc_dpcm_can_be_params(fe, be, stream))
			continue;

		if ((be->dpcm[stream].state != SND_SOC_DPCM_STATE_OPEN) &&
			(be->dpcm[stream].state != SND_SOC_DPCM_STATE_HW_PARAMS) &&
		    (be->dpcm[stream].state != SND_SOC_DPCM_STATE_HW_FREE))
			continue;

		dev_dbg(be->dev, "dpcm: hw_params BE %s\n",
			dpcm_params->fe->dai_link->name);

		/* copy params for each dpcm_params */
		memcpy(&dpcm_params->hw_params, &fe->dpcm[stream].hw_params,
				sizeof(struct snd_pcm_hw_params));

		/* perform any hw_params fixups */
		if (be->dai_link->be_hw_params_fixup) {
			ret = be->dai_link->be_hw_params_fixup(be,
					&dpcm_params->hw_params);
			if (ret < 0) {
				dev_err(be->dev,
					"dpcm: hw_params BE fixup failed %d\n",
					ret);
				goto unwind;
			}
		}

		ret = soc_pcm_hw_params(be_substream, &dpcm_params->hw_params);
		if (ret < 0) {
			dev_err(dpcm_params->be->dev, "dpcm: hw_params BE failed %d\n", ret);
=======
		/* only free hw when no longer used - check all FEs */
		if (!snd_soc_dpcm_can_be_free_stop(fe, be, stream))
				continue;

		if ((be->dpcm[stream].state != SND_SOC_DPCM_STATE_HW_PARAMS) &&
		    (be->dpcm[stream].state != SND_SOC_DPCM_STATE_PREPARE) &&
		    (be->dpcm[stream].state != SND_SOC_DPCM_STATE_HW_FREE) &&
		    (be->dpcm[stream].state != SND_SOC_DPCM_STATE_PAUSED) &&
		    (be->dpcm[stream].state != SND_SOC_DPCM_STATE_STOP))
			continue;

		dev_dbg(be->dev, "ASoC: hw_free BE %s\n",
			dpcm->fe->dai_link->name);

		soc_pcm_hw_free(be_substream);

		be->dpcm[stream].state = SND_SOC_DPCM_STATE_HW_FREE;
	}

	return 0;
}

static int dpcm_fe_dai_hw_free(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *fe = substream->private_data;
	int err, stream = substream->stream;

	mutex_lock_nested(&fe->card->mutex, SND_SOC_CARD_CLASS_RUNTIME);
	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_FE;

	dev_dbg(fe->dev, "ASoC: hw_free FE %s\n", fe->dai_link->name);

	/* call hw_free on the frontend */
	err = soc_pcm_hw_free(substream);
	if (err < 0)
		dev_err(fe->dev,"ASoC: hw_free FE %s failed\n",
			fe->dai_link->name);

	/* only hw_params backends that are either sinks or sources
	 * to this frontend DAI */
	err = dpcm_be_dai_hw_free(fe, stream);

	fe->dpcm[stream].state = SND_SOC_DPCM_STATE_HW_FREE;
	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_NO;

	mutex_unlock(&fe->card->mutex);
	return 0;
}

int dpcm_be_dai_hw_params(struct snd_soc_pcm_runtime *fe, int stream)
{
	struct snd_soc_dpcm *dpcm;
	int ret;

	list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be) {

		struct snd_soc_pcm_runtime *be = dpcm->be;
		struct snd_pcm_substream *be_substream =
			snd_soc_dpcm_get_substream(be, stream);

		/* is this op for this BE ? */
		if (!snd_soc_dpcm_be_can_update(fe, be, stream))
			continue;

		/* only allow hw_params() if no connected FEs are running */
		if (!snd_soc_dpcm_can_be_params(fe, be, stream))
			continue;

		if ((be->dpcm[stream].state != SND_SOC_DPCM_STATE_OPEN) &&
		    (be->dpcm[stream].state != SND_SOC_DPCM_STATE_HW_PARAMS) &&
		    (be->dpcm[stream].state != SND_SOC_DPCM_STATE_HW_FREE))
			continue;

		dev_dbg(be->dev, "ASoC: hw_params BE %s\n",
			dpcm->fe->dai_link->name);

		/* copy params for each dpcm */
		memcpy(&dpcm->hw_params, &fe->dpcm[stream].hw_params,
				sizeof(struct snd_pcm_hw_params));

		/* perform any hw_params fixups */
		if (be->dai_link->be_hw_params_fixup) {
			ret = be->dai_link->be_hw_params_fixup(be,
					&dpcm->hw_params);
			if (ret < 0) {
				dev_err(be->dev,
					"ASoC: hw_params BE fixup failed %d\n",
					ret);
				goto unwind;
			}
		}

		ret = soc_pcm_hw_params(be_substream, &dpcm->hw_params);
		if (ret < 0) {
			dev_err(dpcm->be->dev,
				"ASoC: hw_params BE failed %d\n", ret);
>>>>>>> refs/remotes/origin/master
			goto unwind;
		}

		be->dpcm[stream].state = SND_SOC_DPCM_STATE_HW_PARAMS;
	}
	return 0;

unwind:
	/* disable any enabled and non active backends */
<<<<<<< HEAD
	list_for_each_entry_continue_reverse(dpcm_params, &fe->dpcm[stream].be_clients, list_be) {
		struct snd_soc_pcm_runtime *be = dpcm_params->be;
=======
	list_for_each_entry_continue_reverse(dpcm, &fe->dpcm[stream].be_clients, list_be) {
		struct snd_soc_pcm_runtime *be = dpcm->be;
>>>>>>> refs/remotes/origin/master
		struct snd_pcm_substream *be_substream =
			snd_soc_dpcm_get_substream(be, stream);

		if (!snd_soc_dpcm_be_can_update(fe, be, stream))
			continue;

		/* only allow hw_free() if no connected FEs are running */
		if (!snd_soc_dpcm_can_be_free_stop(fe, be, stream))
			continue;

		if ((be->dpcm[stream].state != SND_SOC_DPCM_STATE_OPEN) &&
<<<<<<< HEAD
			(be->dpcm[stream].state != SND_SOC_DPCM_STATE_HW_PARAMS) &&
		    (be->dpcm[stream].state != SND_SOC_DPCM_STATE_HW_FREE) &&
		     (be->dpcm[stream].state != SND_SOC_DPCM_STATE_STOP))
=======
		   (be->dpcm[stream].state != SND_SOC_DPCM_STATE_HW_PARAMS) &&
		   (be->dpcm[stream].state != SND_SOC_DPCM_STATE_HW_FREE) &&
		   (be->dpcm[stream].state != SND_SOC_DPCM_STATE_STOP))
>>>>>>> refs/remotes/origin/master
			continue;

		soc_pcm_hw_free(be_substream);
	}

	return ret;
}

<<<<<<< HEAD
int soc_dpcm_fe_dai_hw_params(struct snd_pcm_substream *substream,
				    struct snd_pcm_hw_params *params)
=======
static int dpcm_fe_dai_hw_params(struct snd_pcm_substream *substream,
				 struct snd_pcm_hw_params *params)
>>>>>>> refs/remotes/origin/master
{
	struct snd_soc_pcm_runtime *fe = substream->private_data;
	int ret, stream = substream->stream;

<<<<<<< HEAD
	mutex_lock(&fe->card->dpcm_mutex);
=======
	mutex_lock_nested(&fe->card->mutex, SND_SOC_CARD_CLASS_RUNTIME);
>>>>>>> refs/remotes/origin/master
	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_FE;

	memcpy(&fe->dpcm[substream->stream].hw_params, params,
			sizeof(struct snd_pcm_hw_params));
<<<<<<< HEAD
	ret = soc_dpcm_be_dai_hw_params(fe, substream->stream);
	if (ret < 0) {
		dev_err(fe->dev,"dpcm: hw_params failed for some BEs %d\n", ret);
		goto out;
	}

	dev_dbg(fe->dev, "dpcm: hw_params FE %s rate %d chan %x fmt %d\n",
			fe->dai_link->name, params_rate(params), params_channels(params),
			params_format(params));
=======
	ret = dpcm_be_dai_hw_params(fe, substream->stream);
	if (ret < 0) {
		dev_err(fe->dev,"ASoC: hw_params BE failed %d\n", ret);
		goto out;
	}

	dev_dbg(fe->dev, "ASoC: hw_params FE %s rate %d chan %x fmt %d\n",
			fe->dai_link->name, params_rate(params),
			params_channels(params), params_format(params));
>>>>>>> refs/remotes/origin/master

	/* call hw_params on the frontend */
	ret = soc_pcm_hw_params(substream, params);
	if (ret < 0) {
<<<<<<< HEAD
		dev_err(fe->dev,"dpcm: hw_params FE failed %d\n", ret);
		soc_dpcm_be_dai_hw_free(fe, stream);
=======
		dev_err(fe->dev,"ASoC: hw_params FE failed %d\n", ret);
		dpcm_be_dai_hw_free(fe, stream);
>>>>>>> refs/remotes/origin/master
	 } else
		fe->dpcm[stream].state = SND_SOC_DPCM_STATE_HW_PARAMS;

out:
	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_NO;
<<<<<<< HEAD
	mutex_unlock(&fe->card->dpcm_mutex);
	return ret;
}

static int dpcm_do_trigger(struct snd_soc_dpcm_params *dpcm_params,
=======
	mutex_unlock(&fe->card->mutex);
	return ret;
}

static int dpcm_do_trigger(struct snd_soc_dpcm *dpcm,
>>>>>>> refs/remotes/origin/master
		struct snd_pcm_substream *substream, int cmd)
{
	int ret;

<<<<<<< HEAD
	dev_dbg(dpcm_params->be->dev, "dpcm: trigger BE %s cmd %d\n",
			dpcm_params->fe->dai_link->name, cmd);

	ret = soc_pcm_trigger(substream, cmd);
	if (ret < 0)
		dev_err(dpcm_params->be->dev,"dpcm: trigger BE failed %d\n", ret);
=======
	dev_dbg(dpcm->be->dev, "ASoC: trigger BE %s cmd %d\n",
			dpcm->fe->dai_link->name, cmd);

	ret = soc_pcm_trigger(substream, cmd);
	if (ret < 0)
		dev_err(dpcm->be->dev,"ASoC: trigger BE failed %d\n", ret);
>>>>>>> refs/remotes/origin/master

	return ret;
}

<<<<<<< HEAD
int soc_dpcm_be_dai_trigger(struct snd_soc_pcm_runtime *fe, int stream, int cmd)
{
	struct snd_soc_dpcm_params *dpcm_params;
	int ret = 0;

	if ((cmd == SNDRV_PCM_TRIGGER_PAUSE_RELEASE) ||
				(cmd == SNDRV_PCM_TRIGGER_PAUSE_PUSH))
		return ret;

	list_for_each_entry(dpcm_params, &fe->dpcm[stream].be_clients, list_be) {

		struct snd_soc_pcm_runtime *be = dpcm_params->be;
=======
int dpcm_be_dai_trigger(struct snd_soc_pcm_runtime *fe, int stream,
			       int cmd)
{
	struct snd_soc_dpcm *dpcm;
	int ret = 0;

	list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be) {

		struct snd_soc_pcm_runtime *be = dpcm->be;
>>>>>>> refs/remotes/origin/master
		struct snd_pcm_substream *be_substream =
			snd_soc_dpcm_get_substream(be, stream);

		/* is this op for this BE ? */
		if (!snd_soc_dpcm_be_can_update(fe, be, stream))
			continue;

		switch (cmd) {
		case SNDRV_PCM_TRIGGER_START:
			if ((be->dpcm[stream].state != SND_SOC_DPCM_STATE_PREPARE) &&
			    (be->dpcm[stream].state != SND_SOC_DPCM_STATE_STOP))
				continue;

<<<<<<< HEAD
			ret = dpcm_do_trigger(dpcm_params, be_substream, cmd);
=======
			ret = dpcm_do_trigger(dpcm, be_substream, cmd);
>>>>>>> refs/remotes/origin/master
			if (ret)
				return ret;

			be->dpcm[stream].state = SND_SOC_DPCM_STATE_START;
			break;
		case SNDRV_PCM_TRIGGER_RESUME:
			if ((be->dpcm[stream].state != SND_SOC_DPCM_STATE_SUSPEND))
				continue;

<<<<<<< HEAD
			ret = dpcm_do_trigger(dpcm_params, be_substream, cmd);
=======
			ret = dpcm_do_trigger(dpcm, be_substream, cmd);
>>>>>>> refs/remotes/origin/master
			if (ret)
				return ret;

			be->dpcm[stream].state = SND_SOC_DPCM_STATE_START;
			break;
		case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
			if ((be->dpcm[stream].state != SND_SOC_DPCM_STATE_PAUSED))
				continue;

<<<<<<< HEAD
			ret = dpcm_do_trigger(dpcm_params, be_substream, cmd);
=======
			ret = dpcm_do_trigger(dpcm, be_substream, cmd);
>>>>>>> refs/remotes/origin/master
			if (ret)
				return ret;

			be->dpcm[stream].state = SND_SOC_DPCM_STATE_START;
			break;
		case SNDRV_PCM_TRIGGER_STOP:
			if (be->dpcm[stream].state != SND_SOC_DPCM_STATE_START)
				continue;

			if (!snd_soc_dpcm_can_be_free_stop(fe, be, stream))
				continue;

<<<<<<< HEAD
			ret = dpcm_do_trigger(dpcm_params, be_substream, cmd);
=======
			ret = dpcm_do_trigger(dpcm, be_substream, cmd);
>>>>>>> refs/remotes/origin/master
			if (ret)
				return ret;

			be->dpcm[stream].state = SND_SOC_DPCM_STATE_STOP;
			break;
		case SNDRV_PCM_TRIGGER_SUSPEND:
			if (be->dpcm[stream].state != SND_SOC_DPCM_STATE_STOP)
				continue;

			if (!snd_soc_dpcm_can_be_free_stop(fe, be, stream))
				continue;

<<<<<<< HEAD
			ret = dpcm_do_trigger(dpcm_params, be_substream, cmd);
=======
			ret = dpcm_do_trigger(dpcm, be_substream, cmd);
>>>>>>> refs/remotes/origin/master
			if (ret)
				return ret;

			be->dpcm[stream].state = SND_SOC_DPCM_STATE_SUSPEND;
			break;
		case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
			if (be->dpcm[stream].state != SND_SOC_DPCM_STATE_START)
				continue;

			if (!snd_soc_dpcm_can_be_free_stop(fe, be, stream))
				continue;

<<<<<<< HEAD
			ret = dpcm_do_trigger(dpcm_params, be_substream, cmd);
=======
			ret = dpcm_do_trigger(dpcm, be_substream, cmd);
>>>>>>> refs/remotes/origin/master
			if (ret)
				return ret;

			be->dpcm[stream].state = SND_SOC_DPCM_STATE_PAUSED;
			break;
		}
	}

	return ret;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(soc_dpcm_be_dai_trigger);

int soc_dpcm_fe_dai_trigger(struct snd_pcm_substream *substream, int cmd)
=======
EXPORT_SYMBOL_GPL(dpcm_be_dai_trigger);

static int dpcm_fe_dai_trigger(struct snd_pcm_substream *substream, int cmd)
>>>>>>> refs/remotes/origin/master
{
	struct snd_soc_pcm_runtime *fe = substream->private_data;
	int stream = substream->stream, ret;
	enum snd_soc_dpcm_trigger trigger = fe->dai_link->trigger[stream];

	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_FE;

	switch (trigger) {
	case SND_SOC_DPCM_TRIGGER_PRE:
		/* call trigger on the frontend before the backend. */

<<<<<<< HEAD
		dev_dbg(fe->dev, "dpcm: pre trigger FE %s cmd %d\n",
=======
		dev_dbg(fe->dev, "ASoC: pre trigger FE %s cmd %d\n",
>>>>>>> refs/remotes/origin/master
				fe->dai_link->name, cmd);

		ret = soc_pcm_trigger(substream, cmd);
		if (ret < 0) {
<<<<<<< HEAD
			dev_err(fe->dev,"dpcm: trigger FE failed %d\n", ret);
			goto out;
		}

		ret = soc_dpcm_be_dai_trigger(fe, substream->stream, cmd);
=======
			dev_err(fe->dev,"ASoC: trigger FE failed %d\n", ret);
			goto out;
		}

		ret = dpcm_be_dai_trigger(fe, substream->stream, cmd);
>>>>>>> refs/remotes/origin/master
		break;
	case SND_SOC_DPCM_TRIGGER_POST:
		/* call trigger on the frontend after the backend. */

<<<<<<< HEAD
		ret = soc_dpcm_be_dai_trigger(fe, substream->stream, cmd);
		if (ret < 0) {
			dev_err(fe->dev,"dpcm: trigger FE failed %d\n", ret);
			goto out;
		}

		dev_dbg(fe->dev, "dpcm: post trigger FE %s cmd %d\n",
=======
		ret = dpcm_be_dai_trigger(fe, substream->stream, cmd);
		if (ret < 0) {
			dev_err(fe->dev,"ASoC: trigger FE failed %d\n", ret);
			goto out;
		}

		dev_dbg(fe->dev, "ASoC: post trigger FE %s cmd %d\n",
>>>>>>> refs/remotes/origin/master
				fe->dai_link->name, cmd);

		ret = soc_pcm_trigger(substream, cmd);
		break;
	case SND_SOC_DPCM_TRIGGER_BESPOKE:
		/* bespoke trigger() - handles both FE and BEs */

<<<<<<< HEAD
		dev_dbg(fe->dev, "dpcm: bespoke trigger FE %s cmd %d\n",
=======
		dev_dbg(fe->dev, "ASoC: bespoke trigger FE %s cmd %d\n",
>>>>>>> refs/remotes/origin/master
				fe->dai_link->name, cmd);

		ret = soc_pcm_bespoke_trigger(substream, cmd);
		if (ret < 0) {
<<<<<<< HEAD
			dev_err(fe->dev,"dpcm: trigger FE failed %d\n", ret);
=======
			dev_err(fe->dev,"ASoC: trigger FE failed %d\n", ret);
>>>>>>> refs/remotes/origin/master
			goto out;
		}
		break;
	default:
<<<<<<< HEAD
		dev_err(fe->dev, "dpcm: invalid trigger cmd %d for %s\n", cmd,
=======
		dev_err(fe->dev, "ASoC: invalid trigger cmd %d for %s\n", cmd,
>>>>>>> refs/remotes/origin/master
				fe->dai_link->name);
		ret = -EINVAL;
		goto out;
	}

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
	case SNDRV_PCM_TRIGGER_RESUME:
	case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
		fe->dpcm[stream].state = SND_SOC_DPCM_STATE_START;
		break;
	case SNDRV_PCM_TRIGGER_STOP:
	case SNDRV_PCM_TRIGGER_SUSPEND:
	case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
		fe->dpcm[stream].state = SND_SOC_DPCM_STATE_STOP;
		break;
	}

out:
	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_NO;
	return ret;
}

<<<<<<< HEAD
static int soc_dpcm_be_dai_prepare(struct snd_soc_pcm_runtime *fe, int stream)
{
	struct snd_soc_dpcm_params *dpcm_params;
	int ret = 0;

	list_for_each_entry(dpcm_params, &fe->dpcm[stream].be_clients, list_be) {

		struct snd_soc_pcm_runtime *be = dpcm_params->be;
=======
int dpcm_be_dai_prepare(struct snd_soc_pcm_runtime *fe, int stream)
{
	struct snd_soc_dpcm *dpcm;
	int ret = 0;

	list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be) {

		struct snd_soc_pcm_runtime *be = dpcm->be;
>>>>>>> refs/remotes/origin/master
		struct snd_pcm_substream *be_substream =
			snd_soc_dpcm_get_substream(be, stream);

		/* is this op for this BE ? */
		if (!snd_soc_dpcm_be_can_update(fe, be, stream))
			continue;

		if ((be->dpcm[stream].state != SND_SOC_DPCM_STATE_HW_PARAMS) &&
		    (be->dpcm[stream].state != SND_SOC_DPCM_STATE_STOP))
			continue;

<<<<<<< HEAD
		dev_dbg(be->dev, "dpcm: prepare BE %s\n",
			dpcm_params->fe->dai_link->name);

		ret = soc_pcm_prepare(be_substream);
		if (ret < 0) {
			dev_err(be->dev, "dpcm: backend prepare failed %d\n",
=======
		dev_dbg(be->dev, "ASoC: prepare BE %s\n",
			dpcm->fe->dai_link->name);

		ret = soc_pcm_prepare(be_substream);
		if (ret < 0) {
			dev_err(be->dev, "ASoC: backend prepare failed %d\n",
>>>>>>> refs/remotes/origin/master
				ret);
			break;
		}

		be->dpcm[stream].state = SND_SOC_DPCM_STATE_PREPARE;
	}
	return ret;
}

<<<<<<< HEAD
int soc_dpcm_fe_dai_prepare(struct snd_pcm_substream *substream)
=======
static int dpcm_fe_dai_prepare(struct snd_pcm_substream *substream)
>>>>>>> refs/remotes/origin/master
{
	struct snd_soc_pcm_runtime *fe = substream->private_data;
	int stream = substream->stream, ret = 0;

<<<<<<< HEAD
	mutex_lock(&fe->card->dpcm_mutex);

	dev_dbg(fe->dev, "dpcm: prepare FE %s\n", fe->dai_link->name);
=======
	mutex_lock_nested(&fe->card->mutex, SND_SOC_CARD_CLASS_RUNTIME);

	dev_dbg(fe->dev, "ASoC: prepare FE %s\n", fe->dai_link->name);
>>>>>>> refs/remotes/origin/master

	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_FE;

	/* there is no point preparing this FE if there are no BEs */
	if (list_empty(&fe->dpcm[stream].be_clients)) {
<<<<<<< HEAD
		dev_err(fe->dev, "dpcm: no backend DAIs enabled for %s\n",
=======
		dev_err(fe->dev, "ASoC: no backend DAIs enabled for %s\n",
>>>>>>> refs/remotes/origin/master
				fe->dai_link->name);
		ret = -EINVAL;
		goto out;
	}

<<<<<<< HEAD
	ret = soc_dpcm_be_dai_prepare(fe, substream->stream);
=======
	ret = dpcm_be_dai_prepare(fe, substream->stream);
>>>>>>> refs/remotes/origin/master
	if (ret < 0)
		goto out;

	/* call prepare on the frontend */
	ret = soc_pcm_prepare(substream);
	if (ret < 0) {
<<<<<<< HEAD
		dev_err(fe->dev,"dpcm: prepare FE %s failed\n", fe->dai_link->name);
=======
		dev_err(fe->dev,"ASoC: prepare FE %s failed\n",
			fe->dai_link->name);
>>>>>>> refs/remotes/origin/master
		goto out;
	}

	/* run the stream event for each BE */
<<<<<<< HEAD
	if (stream == SNDRV_PCM_STREAM_PLAYBACK)
		soc_dpcm_dapm_stream_event(fe, stream,
				fe->cpu_dai->driver->playback.stream_name,
				SND_SOC_DAPM_STREAM_START);
	else
		soc_dpcm_dapm_stream_event(fe, stream,
				fe->cpu_dai->driver->capture.stream_name,
				SND_SOC_DAPM_STREAM_START);

=======
	dpcm_dapm_stream_event(fe, stream, SND_SOC_DAPM_STREAM_START);
>>>>>>> refs/remotes/origin/master
	fe->dpcm[stream].state = SND_SOC_DPCM_STATE_PREPARE;

out:
	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_NO;
<<<<<<< HEAD
	mutex_unlock(&fe->card->dpcm_mutex);
	return ret;
}

static int soc_dpcm_be_dai_hw_free(struct snd_soc_pcm_runtime *fe, int stream)
{
	struct snd_soc_dpcm_params *dpcm_params;

	/* only hw_params backends that are either sinks or sources
	 * to this frontend DAI */
	list_for_each_entry(dpcm_params, &fe->dpcm[stream].be_clients, list_be) {

		struct snd_soc_pcm_runtime *be = dpcm_params->be;
		struct snd_pcm_substream *be_substream =
			snd_soc_dpcm_get_substream(be, stream);

		/* is this op for this BE ? */
		if (!snd_soc_dpcm_be_can_update(fe, be, stream))
			continue;

		/* only free hw when no longer used - check all FEs */
		if (!snd_soc_dpcm_can_be_free_stop(fe, be, stream))
				continue;

		if ((be->dpcm[stream].state != SND_SOC_DPCM_STATE_HW_PARAMS) &&
		    (be->dpcm[stream].state != SND_SOC_DPCM_STATE_PREPARE) &&
			(be->dpcm[stream].state != SND_SOC_DPCM_STATE_HW_FREE) &&
		    (be->dpcm[stream].state != SND_SOC_DPCM_STATE_STOP))
			continue;

		dev_dbg(be->dev, "dpcm: hw_free BE %s\n",
			dpcm_params->fe->dai_link->name);

		soc_pcm_hw_free(be_substream);

		be->dpcm[stream].state = SND_SOC_DPCM_STATE_HW_FREE;
	}

	return 0;
}

int soc_dpcm_fe_dai_hw_free(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_runtime *fe = substream->private_data;
	int err, stream = substream->stream;

	mutex_lock(&fe->card->dpcm_mutex);
	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_FE;

	dev_dbg(fe->dev, "dpcm: hw_free FE %s\n", fe->dai_link->name);

	/* call hw_free on the frontend */
	err = soc_pcm_hw_free(substream);
	if (err < 0)
		dev_err(fe->dev,"dpcm: hw_free FE %s failed\n", fe->dai_link->name);

	/* only hw_params backends that are either sinks or sources
	 * to this frontend DAI */
	err = soc_dpcm_be_dai_hw_free(fe, stream);

	fe->dpcm[stream].state = SND_SOC_DPCM_STATE_HW_FREE;
	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_NO;

	mutex_unlock(&fe->card->dpcm_mutex);
	return 0;
=======
	mutex_unlock(&fe->card->mutex);

	return ret;
>>>>>>> refs/remotes/origin/master
}

static int soc_pcm_ioctl(struct snd_pcm_substream *substream,
		     unsigned int cmd, void *arg)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_platform *platform = rtd->platform;

<<<<<<< HEAD
	if (platform->driver->ops->ioctl)
=======
	if (platform->driver->ops && platform->driver->ops->ioctl)
>>>>>>> refs/remotes/origin/master
		return platform->driver->ops->ioctl(substream, cmd, arg);
	return snd_pcm_lib_ioctl(substream, cmd, arg);
}

static int dpcm_run_update_shutdown(struct snd_soc_pcm_runtime *fe, int stream)
{
<<<<<<< HEAD
	struct snd_pcm_substream *substream = snd_soc_dpcm_get_substream(fe, stream);
	enum snd_soc_dpcm_trigger trigger = fe->dai_link->trigger[stream];
	int err;

	dev_dbg(fe->dev, "runtime %s close on FE %s\n",
=======
	struct snd_pcm_substream *substream =
		snd_soc_dpcm_get_substream(fe, stream);
	enum snd_soc_dpcm_trigger trigger = fe->dai_link->trigger[stream];
	int err;

	dev_dbg(fe->dev, "ASoC: runtime %s close on FE %s\n",
>>>>>>> refs/remotes/origin/master
			stream ? "capture" : "playback", fe->dai_link->name);

	if (trigger == SND_SOC_DPCM_TRIGGER_BESPOKE) {
		/* call bespoke trigger - FE takes care of all BE triggers */
<<<<<<< HEAD
		dev_dbg(fe->dev, "dpcm: bespoke trigger FE %s cmd stop\n",
=======
		dev_dbg(fe->dev, "ASoC: bespoke trigger FE %s cmd stop\n",
>>>>>>> refs/remotes/origin/master
				fe->dai_link->name);

		err = soc_pcm_bespoke_trigger(substream, SNDRV_PCM_TRIGGER_STOP);
		if (err < 0)
<<<<<<< HEAD
			dev_err(fe->dev,"dpcm: trigger FE failed %d\n", err);
	} else {
		dev_dbg(fe->dev, "dpcm: trigger FE %s cmd stop\n",
			fe->dai_link->name);

		err = soc_dpcm_be_dai_trigger(fe, stream, SNDRV_PCM_TRIGGER_STOP);
		if (err < 0)
			dev_err(fe->dev,"dpcm: trigger FE failed %d\n", err);
	}

	err = soc_dpcm_be_dai_hw_free(fe, stream);
	if (err < 0)
		dev_err(fe->dev,"dpcm: hw_free FE failed %d\n", err);

	err = soc_dpcm_be_dai_shutdown(fe, stream);
	if (err < 0)
		dev_err(fe->dev,"dpcm: shutdown FE failed %d\n", err);

	/* run the stream event for each BE */
	if (stream == SNDRV_PCM_STREAM_PLAYBACK)
		soc_dpcm_dapm_stream_event(fe, stream,
				fe->cpu_dai->driver->playback.stream_name,
				SND_SOC_DAPM_STREAM_NOP);
	else
		soc_dpcm_dapm_stream_event(fe, stream,
				fe->cpu_dai->driver->capture.stream_name,
				SND_SOC_DAPM_STREAM_NOP);
=======
			dev_err(fe->dev,"ASoC: trigger FE failed %d\n", err);
	} else {
		dev_dbg(fe->dev, "ASoC: trigger FE %s cmd stop\n",
			fe->dai_link->name);

		err = dpcm_be_dai_trigger(fe, stream, SNDRV_PCM_TRIGGER_STOP);
		if (err < 0)
			dev_err(fe->dev,"ASoC: trigger FE failed %d\n", err);
	}

	err = dpcm_be_dai_hw_free(fe, stream);
	if (err < 0)
		dev_err(fe->dev,"ASoC: hw_free FE failed %d\n", err);

	err = dpcm_be_dai_shutdown(fe, stream);
	if (err < 0)
		dev_err(fe->dev,"ASoC: shutdown FE failed %d\n", err);

	/* run the stream event for each BE */
	dpcm_dapm_stream_event(fe, stream, SND_SOC_DAPM_STREAM_NOP);
>>>>>>> refs/remotes/origin/master

	return 0;
}

static int dpcm_run_update_startup(struct snd_soc_pcm_runtime *fe, int stream)
{
<<<<<<< HEAD
	struct snd_pcm_substream *substream = snd_soc_dpcm_get_substream(fe, stream);
	struct snd_soc_dpcm_params *dpcm_params;
	enum snd_soc_dpcm_trigger trigger = fe->dai_link->trigger[stream];
	int ret;

	dev_dbg(fe->dev, "runtime %s open on FE %s\n",
=======
	struct snd_pcm_substream *substream =
		snd_soc_dpcm_get_substream(fe, stream);
	struct snd_soc_dpcm *dpcm;
	enum snd_soc_dpcm_trigger trigger = fe->dai_link->trigger[stream];
	int ret;

	dev_dbg(fe->dev, "ASoC: runtime %s open on FE %s\n",
>>>>>>> refs/remotes/origin/master
			stream ? "capture" : "playback", fe->dai_link->name);

	/* Only start the BE if the FE is ready */
	if (fe->dpcm[stream].state == SND_SOC_DPCM_STATE_HW_FREE ||
		fe->dpcm[stream].state == SND_SOC_DPCM_STATE_CLOSE)
		return -EINVAL;

	/* startup must always be called for new BEs */
<<<<<<< HEAD
	ret = soc_dpcm_be_dai_startup(fe, stream);
	if (ret < 0) {
		goto disconnect;
		return ret;
	}
=======
	ret = dpcm_be_dai_startup(fe, stream);
	if (ret < 0)
		goto disconnect;
>>>>>>> refs/remotes/origin/master

	/* keep going if FE state is > open */
	if (fe->dpcm[stream].state == SND_SOC_DPCM_STATE_OPEN)
		return 0;

<<<<<<< HEAD
	ret = soc_dpcm_be_dai_hw_params(fe, stream);
	if (ret < 0) {
		goto close;
		return ret;
	}
=======
	ret = dpcm_be_dai_hw_params(fe, stream);
	if (ret < 0)
		goto close;
>>>>>>> refs/remotes/origin/master

	/* keep going if FE state is > hw_params */
	if (fe->dpcm[stream].state == SND_SOC_DPCM_STATE_HW_PARAMS)
		return 0;


<<<<<<< HEAD
	ret = soc_dpcm_be_dai_prepare(fe, stream);
	if (ret < 0) {
		goto hw_free;
		return ret;
	}

	/* run the stream event for each BE */
	if (stream == SNDRV_PCM_STREAM_PLAYBACK)
		soc_dpcm_dapm_stream_event(fe, stream,
				fe->cpu_dai->driver->playback.stream_name,
				SND_SOC_DAPM_STREAM_NOP);
	else
		soc_dpcm_dapm_stream_event(fe, stream,
				fe->cpu_dai->driver->capture.stream_name,
				SND_SOC_DAPM_STREAM_NOP);
=======
	ret = dpcm_be_dai_prepare(fe, stream);
	if (ret < 0)
		goto hw_free;

	/* run the stream event for each BE */
	dpcm_dapm_stream_event(fe, stream, SND_SOC_DAPM_STREAM_NOP);
>>>>>>> refs/remotes/origin/master

	/* keep going if FE state is > prepare */
	if (fe->dpcm[stream].state == SND_SOC_DPCM_STATE_PREPARE ||
		fe->dpcm[stream].state == SND_SOC_DPCM_STATE_STOP)
		return 0;

	if (trigger == SND_SOC_DPCM_TRIGGER_BESPOKE) {
		/* call trigger on the frontend - FE takes care of all BE triggers */
<<<<<<< HEAD
		dev_dbg(fe->dev, "dpcm: bespoke trigger FE %s cmd start\n",
=======
		dev_dbg(fe->dev, "ASoC: bespoke trigger FE %s cmd start\n",
>>>>>>> refs/remotes/origin/master
				fe->dai_link->name);

		ret = soc_pcm_bespoke_trigger(substream, SNDRV_PCM_TRIGGER_START);
		if (ret < 0) {
<<<<<<< HEAD
			dev_err(fe->dev,"dpcm: bespoke trigger FE failed %d\n", ret);
			goto hw_free;
		}
	} else {
		dev_dbg(fe->dev, "dpcm: trigger FE %s cmd start\n",
			fe->dai_link->name);

		ret = soc_dpcm_be_dai_trigger(fe, stream,
					SNDRV_PCM_TRIGGER_START);
		if (ret < 0) {
			dev_err(fe->dev,"dpcm: trigger FE failed %d\n", ret);
=======
			dev_err(fe->dev,"ASoC: bespoke trigger FE failed %d\n", ret);
			goto hw_free;
		}
	} else {
		dev_dbg(fe->dev, "ASoC: trigger FE %s cmd start\n",
			fe->dai_link->name);

		ret = dpcm_be_dai_trigger(fe, stream,
					SNDRV_PCM_TRIGGER_START);
		if (ret < 0) {
			dev_err(fe->dev,"ASoC: trigger FE failed %d\n", ret);
>>>>>>> refs/remotes/origin/master
			goto hw_free;
		}
	}

	return 0;

hw_free:
<<<<<<< HEAD
	soc_dpcm_be_dai_hw_free(fe, stream);
close:
	soc_dpcm_be_dai_shutdown(fe, stream);
disconnect:
	/* disconnect any non started BEs */
	list_for_each_entry(dpcm_params, &fe->dpcm[stream].be_clients, list_be) {
		struct snd_soc_pcm_runtime *be = dpcm_params->be;
		if (be->dpcm[stream].state != SND_SOC_DPCM_STATE_START)
				dpcm_params->state = SND_SOC_DPCM_LINK_STATE_FREE;
=======
	dpcm_be_dai_hw_free(fe, stream);
close:
	dpcm_be_dai_shutdown(fe, stream);
disconnect:
	/* disconnect any non started BEs */
	list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be) {
		struct snd_soc_pcm_runtime *be = dpcm->be;
		if (be->dpcm[stream].state != SND_SOC_DPCM_STATE_START)
				dpcm->state = SND_SOC_DPCM_LINK_STATE_FREE;
>>>>>>> refs/remotes/origin/master
	}

	return ret;
}

static int dpcm_run_new_update(struct snd_soc_pcm_runtime *fe, int stream)
{
	int ret;

	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_BE;
	ret = dpcm_run_update_startup(fe, stream);
	if (ret < 0)
<<<<<<< HEAD
		dev_err(fe->dev, "failed to startup some BEs\n");
=======
		dev_err(fe->dev, "ASoC: failed to startup some BEs\n");
>>>>>>> refs/remotes/origin/master
	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_NO;

	return ret;
}

static int dpcm_run_old_update(struct snd_soc_pcm_runtime *fe, int stream)
{
	int ret;

	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_BE;
	ret = dpcm_run_update_shutdown(fe, stream);
	if (ret < 0)
<<<<<<< HEAD
		dev_err(fe->dev, "failed to shutdown some BEs\n");
=======
		dev_err(fe->dev, "ASoC: failed to shutdown some BEs\n");
>>>>>>> refs/remotes/origin/master
	fe->dpcm[stream].runtime_update = SND_SOC_DPCM_UPDATE_NO;

	return ret;
}

<<<<<<< HEAD
/* called when any mixer updates change FE -> BE the stream */
int soc_dpcm_runtime_update(struct snd_soc_dapm_widget *widget)
{
	struct snd_soc_card *card;
	int i, ret = 0, old, new, paths;

	if (widget->codec)
		card = widget->codec->card;
	else if (widget->platform)
		card = widget->platform->card;
	else
		return -EINVAL;

	mutex_lock(&card->dpcm_mutex);

=======
/* Called by DAPM mixer/mux changes to update audio routing between PCMs and
 * any DAI links.
 */
int soc_dpcm_runtime_update(struct snd_soc_card *card)
{
	int i, old, new, paths;

	mutex_lock_nested(&card->mutex, SND_SOC_CARD_CLASS_RUNTIME);
>>>>>>> refs/remotes/origin/master
	for (i = 0; i < card->num_rtd; i++) {
		struct snd_soc_dapm_widget_list *list;
		struct snd_soc_pcm_runtime *fe = &card->rtd[i];

		/* make sure link is FE */
		if (!fe->dai_link->dynamic)
			continue;

		/* only check active links */
		if (!fe->cpu_dai->active)
			continue;

		/* DAPM sync will call this to update DSP paths */
<<<<<<< HEAD
		dev_dbg(fe->dev, "DPCM runtime update for FE %s\n", fe->dai_link->name);
=======
		dev_dbg(fe->dev, "ASoC: DPCM runtime update for FE %s\n",
			fe->dai_link->name);
>>>>>>> refs/remotes/origin/master

		/* skip if FE doesn't have playback capability */
		if (!fe->cpu_dai->driver->playback.channels_min)
			goto capture;

<<<<<<< HEAD
		paths = fe_path_get(fe, SNDRV_PCM_STREAM_PLAYBACK, &list);
		if (paths < 0) {
			dev_warn(fe->dev, "%s no valid %s route from source to sink\n",
					fe->dai_link->name,  "playback");
			ret = paths;
			goto out;
=======
		paths = dpcm_path_get(fe, SNDRV_PCM_STREAM_PLAYBACK, &list);
		if (paths < 0) {
			dev_warn(fe->dev, "ASoC: %s no valid %s path\n",
					fe->dai_link->name,  "playback");
			mutex_unlock(&card->mutex);
			return paths;
>>>>>>> refs/remotes/origin/master
		}

		/* update any new playback paths */
		new = dpcm_process_paths(fe, SNDRV_PCM_STREAM_PLAYBACK, &list, 1);
		if (new) {
			dpcm_run_new_update(fe, SNDRV_PCM_STREAM_PLAYBACK);
<<<<<<< HEAD
			fe_clear_pending(fe, SNDRV_PCM_STREAM_PLAYBACK);
			be_disconnect(fe, SNDRV_PCM_STREAM_PLAYBACK);
=======
			dpcm_clear_pending_state(fe, SNDRV_PCM_STREAM_PLAYBACK);
			dpcm_be_disconnect(fe, SNDRV_PCM_STREAM_PLAYBACK);
>>>>>>> refs/remotes/origin/master
		}

		/* update any old playback paths */
		old = dpcm_process_paths(fe, SNDRV_PCM_STREAM_PLAYBACK, &list, 0);
		if (old) {
			dpcm_run_old_update(fe, SNDRV_PCM_STREAM_PLAYBACK);
<<<<<<< HEAD
			fe_clear_pending(fe, SNDRV_PCM_STREAM_PLAYBACK);
			be_disconnect(fe, SNDRV_PCM_STREAM_PLAYBACK);
		}

		fe_path_put(&list);

=======
			dpcm_clear_pending_state(fe, SNDRV_PCM_STREAM_PLAYBACK);
			dpcm_be_disconnect(fe, SNDRV_PCM_STREAM_PLAYBACK);
		}

>>>>>>> refs/remotes/origin/master
capture:
		/* skip if FE doesn't have capture capability */
		if (!fe->cpu_dai->driver->capture.channels_min)
			continue;

<<<<<<< HEAD
		paths = fe_path_get(fe, SNDRV_PCM_STREAM_CAPTURE, &list);
		if (paths < 0) {
			dev_warn(fe->dev, "%s no valid %s route from source to sink\n",
					fe->dai_link->name,  "capture");
			ret = paths;
			goto out;
=======
		paths = dpcm_path_get(fe, SNDRV_PCM_STREAM_CAPTURE, &list);
		if (paths < 0) {
			dev_warn(fe->dev, "ASoC: %s no valid %s path\n",
					fe->dai_link->name,  "capture");
			mutex_unlock(&card->mutex);
			return paths;
>>>>>>> refs/remotes/origin/master
		}

		/* update any new capture paths */
		new = dpcm_process_paths(fe, SNDRV_PCM_STREAM_CAPTURE, &list, 1);
		if (new) {
			dpcm_run_new_update(fe, SNDRV_PCM_STREAM_CAPTURE);
<<<<<<< HEAD
			fe_clear_pending(fe, SNDRV_PCM_STREAM_CAPTURE);
			be_disconnect(fe, SNDRV_PCM_STREAM_CAPTURE);
=======
			dpcm_clear_pending_state(fe, SNDRV_PCM_STREAM_CAPTURE);
			dpcm_be_disconnect(fe, SNDRV_PCM_STREAM_CAPTURE);
>>>>>>> refs/remotes/origin/master
		}

		/* update any old capture paths */
		old = dpcm_process_paths(fe, SNDRV_PCM_STREAM_CAPTURE, &list, 0);
		if (old) {
			dpcm_run_old_update(fe, SNDRV_PCM_STREAM_CAPTURE);
<<<<<<< HEAD
			fe_clear_pending(fe, SNDRV_PCM_STREAM_CAPTURE);
			be_disconnect(fe, SNDRV_PCM_STREAM_CAPTURE);
		}

		fe_path_put(&list);
	}

out:
	mutex_unlock(&card->dpcm_mutex);
	return ret;
}

int soc_dpcm_be_digital_mute(struct snd_soc_pcm_runtime *fe, int mute)
{
	struct snd_soc_dpcm_params *dpcm_params;

	list_for_each_entry(dpcm_params,
			&fe->dpcm[SNDRV_PCM_STREAM_PLAYBACK].be_clients, list_be) {

		struct snd_soc_pcm_runtime *be = dpcm_params->be;
=======
			dpcm_clear_pending_state(fe, SNDRV_PCM_STREAM_CAPTURE);
			dpcm_be_disconnect(fe, SNDRV_PCM_STREAM_CAPTURE);
		}

		dpcm_path_put(&list);
	}

	mutex_unlock(&card->mutex);
	return 0;
}
int soc_dpcm_be_digital_mute(struct snd_soc_pcm_runtime *fe, int mute)
{
	struct snd_soc_dpcm *dpcm;
	struct list_head *clients =
		&fe->dpcm[SNDRV_PCM_STREAM_PLAYBACK].be_clients;

	list_for_each_entry(dpcm, clients, list_be) {

		struct snd_soc_pcm_runtime *be = dpcm->be;
>>>>>>> refs/remotes/origin/master
		struct snd_soc_dai *dai = be->codec_dai;
		struct snd_soc_dai_driver *drv = dai->driver;

		if (be->dai_link->ignore_suspend)
			continue;

<<<<<<< HEAD
		dev_dbg(be->dev, "BE digital mute %s\n", be->dai_link->name);

		if (drv->ops->digital_mute && dai->playback_active)
				drv->ops->digital_mute(dai, mute);
	}

	return 0;
}

int soc_dpcm_be_cpu_dai_suspend(struct snd_soc_pcm_runtime *fe)
{
	struct snd_soc_dpcm_params *dpcm_params;

	/* suspend for playback */
	list_for_each_entry(dpcm_params,
			&fe->dpcm[SNDRV_PCM_STREAM_PLAYBACK].be_clients, list_be) {

		struct snd_soc_pcm_runtime *be = dpcm_params->be;
		struct snd_soc_dai *dai = be->cpu_dai;
		struct snd_soc_dai_driver *drv = dai->driver;

		if (be->dai_link->ignore_suspend)
			continue;

		dev_dbg(be->dev, "pm: BE CPU DAI playback suspend %s\n",
				be->dai_link->name);

		if (drv->suspend && !drv->ac97_control)
				drv->suspend(dai);
	}

	/* suspend for capture */
	list_for_each_entry(dpcm_params,
			&fe->dpcm[SNDRV_PCM_STREAM_CAPTURE].be_clients, list_be) {

		struct snd_soc_pcm_runtime *be = dpcm_params->be;
		struct snd_soc_dai *dai = be->cpu_dai;
		struct snd_soc_dai_driver *drv = dai->driver;

		if (be->dai_link->ignore_suspend)
			continue;

		dev_dbg(be->dev, "pm: BE CPU DAI capture suspend %s\n",
				be->dai_link->name);

		if (drv->suspend && !drv->ac97_control)
				drv->suspend(dai);
	}

	return 0;
}

int soc_dpcm_be_ac97_cpu_dai_suspend(struct snd_soc_pcm_runtime *fe)
{
	struct snd_soc_dpcm_params *dpcm_params;

	/* suspend for playback */
	list_for_each_entry(dpcm_params,
			&fe->dpcm[SNDRV_PCM_STREAM_PLAYBACK].be_clients, list_be) {

		struct snd_soc_pcm_runtime *be = dpcm_params->be;
		struct snd_soc_dai *dai = be->cpu_dai;
		struct snd_soc_dai_driver *drv = dai->driver;

		if (be->dai_link->ignore_suspend)
			continue;

		dev_dbg(be->dev, "pm: BE CPU DAI playback suspend %s\n",
				be->dai_link->name);

		if (drv->suspend && drv->ac97_control)
				drv->suspend(dai);
	}

	/* suspend for capture */
	list_for_each_entry(dpcm_params,
			&fe->dpcm[SNDRV_PCM_STREAM_CAPTURE].be_clients, list_be) {

		struct snd_soc_pcm_runtime *be = dpcm_params->be;
		struct snd_soc_dai *dai = be->cpu_dai;
		struct snd_soc_dai_driver *drv = dai->driver;

		if (be->dai_link->ignore_suspend)
			continue;

		dev_dbg(be->dev, "pm: BE CPU DAI capture suspend %s\n",
				be->dai_link->name);

		if (drv->suspend && drv->ac97_control)
				drv->suspend(dai);
=======
		dev_dbg(be->dev, "ASoC: BE digital mute %s\n", be->dai_link->name);

		if (drv->ops && drv->ops->digital_mute && dai->playback_active)
			drv->ops->digital_mute(dai, mute);
>>>>>>> refs/remotes/origin/master
	}

	return 0;
}

<<<<<<< HEAD
int soc_dpcm_be_platform_suspend(struct snd_soc_pcm_runtime *fe)
{
	struct snd_soc_dpcm_params *dpcm_params;

	/* suspend for playback */
	list_for_each_entry(dpcm_params,
			&fe->dpcm[SNDRV_PCM_STREAM_PLAYBACK].be_clients, list_be) {

		struct snd_soc_pcm_runtime *be = dpcm_params->be;
		struct snd_soc_platform *platform = be->platform;
		struct snd_soc_platform_driver *drv = platform->driver;
		struct snd_soc_dai *dai = be->cpu_dai;

		if (be->dai_link->ignore_suspend)
			continue;

		dev_dbg(be->dev, "pm: BE platform playback suspend %s\n",
				be->dai_link->name);

		if (drv->suspend && !platform->suspended) {
			drv->suspend(dai);
			platform->suspended = 1;
		}
	}

	/* suspend for capture */
	list_for_each_entry(dpcm_params,
			&fe->dpcm[SNDRV_PCM_STREAM_CAPTURE].be_clients, list_be) {

		struct snd_soc_pcm_runtime *be = dpcm_params->be;
		struct snd_soc_platform *platform = be->platform;
		struct snd_soc_platform_driver *drv = platform->driver;
		struct snd_soc_dai *dai = be->cpu_dai;

		if (be->dai_link->ignore_suspend)
			continue;

		dev_dbg(be->dev, "pm: BE platform capture suspend %s\n",
				be->dai_link->name);

		if (drv->suspend && !platform->suspended) {
			drv->suspend(dai);
			platform->suspended = 1;
		}
	}
	return 0;
}

int soc_dpcm_fe_suspend(struct snd_soc_pcm_runtime *fe)
{
	struct snd_soc_dai *dai = fe->cpu_dai;
	struct snd_soc_dai_driver *dai_drv = dai->driver;
	struct snd_soc_platform *platform = fe->platform;
	struct snd_soc_platform_driver *plat_drv = platform->driver;

	if (dai_drv->suspend && !dai_drv->ac97_control)
		dai_drv->suspend(dai);

	if (plat_drv->suspend && !platform->suspended) {
		plat_drv->suspend(dai);
		platform->suspended = 1;
	}

	soc_dpcm_be_cpu_dai_suspend(fe);
	soc_dpcm_be_platform_suspend(fe);

	return 0;
}

int soc_dpcm_be_cpu_dai_resume(struct snd_soc_pcm_runtime *fe)
{
	struct snd_soc_dpcm_params *dpcm_params;

	/* resume for playback */
	list_for_each_entry(dpcm_params,
			&fe->dpcm[SNDRV_PCM_STREAM_PLAYBACK].be_clients, list_be) {

		struct snd_soc_pcm_runtime *be = dpcm_params->be;
		struct snd_soc_dai *dai = be->cpu_dai;
		struct snd_soc_dai_driver *drv = dai->driver;

		if (be->dai_link->ignore_suspend)
			continue;

		dev_dbg(be->dev, "pm: BE CPU DAI playback resume %s\n",
				be->dai_link->name);

		if (drv->resume && !drv->ac97_control)
				drv->resume(dai);
	}

	/* suspend for capture */
	list_for_each_entry(dpcm_params,
			&fe->dpcm[SNDRV_PCM_STREAM_CAPTURE].be_clients, list_be) {

		struct snd_soc_pcm_runtime *be = dpcm_params->be;
		struct snd_soc_dai *dai = be->cpu_dai;
		struct snd_soc_dai_driver *drv = dai->driver;

		if (be->dai_link->ignore_suspend)
			continue;

		dev_dbg(be->dev, "pm: BE CPU DAI capture resume %s\n",
				be->dai_link->name);

		if (drv->resume && !drv->ac97_control)
				drv->resume(dai);
	}

	return 0;
}

int soc_dpcm_be_ac97_cpu_dai_resume(struct snd_soc_pcm_runtime *fe)
{
	struct snd_soc_dpcm_params *dpcm_params;

	/* resume for playback */
	list_for_each_entry(dpcm_params,
			&fe->dpcm[SNDRV_PCM_STREAM_PLAYBACK].be_clients, list_be) {

		struct snd_soc_pcm_runtime *be = dpcm_params->be;
		struct snd_soc_dai *dai = be->cpu_dai;
		struct snd_soc_dai_driver *drv = dai->driver;

		if (be->dai_link->ignore_suspend)
			continue;

		dev_dbg(be->dev, "pm: BE CPU DAI playback resume %s\n",
				be->dai_link->name);

		if (drv->resume && drv->ac97_control)
				drv->resume(dai);
	}

	/* suspend for capture */
	list_for_each_entry(dpcm_params,
			&fe->dpcm[SNDRV_PCM_STREAM_CAPTURE].be_clients, list_be) {

		struct snd_soc_pcm_runtime *be = dpcm_params->be;
		struct snd_soc_dai *dai = be->cpu_dai;
		struct snd_soc_dai_driver *drv = dai->driver;

		if (be->dai_link->ignore_suspend)
			continue;

		dev_dbg(be->dev, "pm: BE CPU DAI capture resume %s\n",
				be->dai_link->name);

		if (drv->resume && drv->ac97_control)
				drv->resume(dai);
	}

	return 0;
}

int soc_dpcm_be_platform_resume(struct snd_soc_pcm_runtime *fe)
{
	struct snd_soc_dpcm_params *dpcm_params;

	/* resume for playback */
	list_for_each_entry(dpcm_params,
			&fe->dpcm[SNDRV_PCM_STREAM_PLAYBACK].be_clients, list_be) {

		struct snd_soc_pcm_runtime *be = dpcm_params->be;
		struct snd_soc_platform *platform = be->platform;
		struct snd_soc_platform_driver *drv = platform->driver;
		struct snd_soc_dai *dai = be->cpu_dai;

		if (be->dai_link->ignore_suspend)
			continue;

		dev_dbg(be->dev, "pm: BE platform playback resume %s\n",
				be->dai_link->name);

		if (drv->resume && platform->suspended) {
			drv->resume(dai);
			platform->suspended = 0;
		}
	}

	/* resume for capture */
	list_for_each_entry(dpcm_params,
			&fe->dpcm[SNDRV_PCM_STREAM_CAPTURE].be_clients, list_be) {

		struct snd_soc_pcm_runtime *be = dpcm_params->be;
		struct snd_soc_platform *platform = be->platform;
		struct snd_soc_platform_driver *drv = platform->driver;
		struct snd_soc_dai *dai = be->cpu_dai;

		if (be->dai_link->ignore_suspend)
			continue;

		dev_dbg(be->dev, "pm: BE platform capture resume %s\n",
				be->dai_link->name);

		if (drv->resume && platform->suspended) {
			drv->resume(dai);
			platform->suspended = 0;
		}
	}

	return 0;
}

int soc_dpcm_fe_resume(struct snd_soc_pcm_runtime *fe)
{
	struct snd_soc_dai *dai = fe->cpu_dai;
	struct snd_soc_dai_driver *dai_drv = dai->driver;
	struct snd_soc_platform *platform = fe->platform;
	struct snd_soc_platform_driver *plat_drv = platform->driver;

	soc_dpcm_be_cpu_dai_resume(fe);
	soc_dpcm_be_platform_resume(fe);

	if (dai_drv->resume && !dai_drv->ac97_control)
		dai_drv->resume(dai);

	if (plat_drv->resume && platform->suspended) {
		plat_drv->resume(dai);
		platform->suspended = 0;
	}

	return 0;
}

/* called when opening FE stream  */
int soc_dpcm_fe_dai_open(struct snd_pcm_substream *fe_substream)
{
	struct snd_soc_pcm_runtime *fe = fe_substream->private_data;
	struct snd_soc_dpcm_params *dpcm_params;
=======
static int dpcm_fe_dai_open(struct snd_pcm_substream *fe_substream)
{
	struct snd_soc_pcm_runtime *fe = fe_substream->private_data;
	struct snd_soc_dpcm *dpcm;
>>>>>>> refs/remotes/origin/master
	struct snd_soc_dapm_widget_list *list;
	int ret;
	int stream = fe_substream->stream;

<<<<<<< HEAD
	fe->dpcm[stream].runtime = fe_substream->runtime;

	if (fe_path_get(fe, stream, &list) <= 0) {
		dev_warn(fe->dev, "asoc: %s no valid %s route from source to sink\n",
			fe->dai_link->name, stream ? "capture" : "playback");
			return -EINVAL;
	}

	/* calculate valid and active FE <-> BE dpcm_paramss */
	dpcm_process_paths(fe, stream, &list, 1);

	ret = soc_dpcm_fe_dai_startup(fe_substream);
	if (ret < 0) {
		/* clean up all links */
		list_for_each_entry(dpcm_params, &fe->dpcm[stream].be_clients, list_be)
				dpcm_params->state = SND_SOC_DPCM_LINK_STATE_FREE;

		be_disconnect(fe, stream);
		fe->dpcm[stream].runtime = NULL;
	}

	fe_clear_pending(fe, stream);
	fe_path_put(&list);
	return ret;
}

/* called when closing FE stream  */
int soc_dpcm_fe_dai_close(struct snd_pcm_substream *fe_substream)
{
	struct snd_soc_pcm_runtime *fe = fe_substream->private_data;
	struct snd_soc_dpcm_params *dpcm_params;
	int stream = fe_substream->stream, ret;

	ret = soc_dpcm_fe_dai_shutdown(fe_substream);

	/* mark FE's links ready to prune */
	list_for_each_entry(dpcm_params, &fe->dpcm[stream].be_clients, list_be)
		dpcm_params->state = SND_SOC_DPCM_LINK_STATE_FREE;

	be_disconnect(fe, stream);

	fe->dpcm[stream].runtime = NULL;

=======
	mutex_lock_nested(&fe->card->mutex, SND_SOC_CARD_CLASS_RUNTIME);
	fe->dpcm[stream].runtime = fe_substream->runtime;

	if (dpcm_path_get(fe, stream, &list) <= 0) {
		dev_dbg(fe->dev, "ASoC: %s no valid %s route\n",
			fe->dai_link->name, stream ? "capture" : "playback");
	}

	/* calculate valid and active FE <-> BE dpcms */
	dpcm_process_paths(fe, stream, &list, 1);

	ret = dpcm_fe_dai_startup(fe_substream);
	if (ret < 0) {
		/* clean up all links */
		list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be)
			dpcm->state = SND_SOC_DPCM_LINK_STATE_FREE;

		dpcm_be_disconnect(fe, stream);
		fe->dpcm[stream].runtime = NULL;
	}

	dpcm_clear_pending_state(fe, stream);
	dpcm_path_put(&list);
	mutex_unlock(&fe->card->mutex);
	return ret;
}

static int dpcm_fe_dai_close(struct snd_pcm_substream *fe_substream)
{
	struct snd_soc_pcm_runtime *fe = fe_substream->private_data;
	struct snd_soc_dpcm *dpcm;
	int stream = fe_substream->stream, ret;

	mutex_lock_nested(&fe->card->mutex, SND_SOC_CARD_CLASS_RUNTIME);
	ret = dpcm_fe_dai_shutdown(fe_substream);

	/* mark FE's links ready to prune */
	list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be)
		dpcm->state = SND_SOC_DPCM_LINK_STATE_FREE;

	dpcm_be_disconnect(fe, stream);

	fe->dpcm[stream].runtime = NULL;
	mutex_unlock(&fe->card->mutex);
>>>>>>> refs/remotes/origin/master
	return ret;
}

/* create a new pcm */
int soc_new_pcm(struct snd_soc_pcm_runtime *rtd, int num)
{
<<<<<<< HEAD
	struct snd_soc_codec *codec = rtd->codec;
	struct snd_soc_platform *platform = rtd->platform;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	struct snd_pcm_substream *substream[2];
=======
	struct snd_soc_platform *platform = rtd->platform;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
>>>>>>> refs/remotes/origin/master
	struct snd_pcm *pcm;
	char new_name[64];
	int ret = 0, playback = 0, capture = 0;

	if (rtd->dai_link->dynamic || rtd->dai_link->no_pcm) {
<<<<<<< HEAD
		if (cpu_dai->driver->playback.channels_min)
			playback = 1;
		if (cpu_dai->driver->capture.channels_min)
			capture = 1;
	} else {
		if (codec_dai->driver->playback.channels_min)
			playback = 1;
		if (codec_dai->driver->capture.channels_min)
			capture = 1;
	}

=======
		playback = rtd->dai_link->dpcm_playback;
		capture = rtd->dai_link->dpcm_capture;
	} else {
		if (codec_dai->driver->playback.channels_min &&
		    cpu_dai->driver->playback.channels_min)
			playback = 1;
		if (codec_dai->driver->capture.channels_min &&
		    cpu_dai->driver->capture.channels_min)
			capture = 1;
	}

	if (rtd->dai_link->playback_only) {
		playback = 1;
		capture = 0;
	}

	if (rtd->dai_link->capture_only) {
		playback = 0;
		capture = 1;
	}

>>>>>>> refs/remotes/origin/master
	/* create the PCM */
	if (rtd->dai_link->no_pcm) {
		snprintf(new_name, sizeof(new_name), "(%s)",
			rtd->dai_link->stream_name);

<<<<<<< HEAD
		ret = snd_pcm_new_soc_be(rtd->card->snd_card, new_name, num,
=======
		ret = snd_pcm_new_internal(rtd->card->snd_card, new_name, num,
>>>>>>> refs/remotes/origin/master
				playback, capture, &pcm);
	} else {
		if (rtd->dai_link->dynamic)
			snprintf(new_name, sizeof(new_name), "%s (*)",
				rtd->dai_link->stream_name);
		else
			snprintf(new_name, sizeof(new_name), "%s %s-%d",
				rtd->dai_link->stream_name, codec_dai->name, num);

		ret = snd_pcm_new(rtd->card->snd_card, new_name, num, playback,
			capture, &pcm);
	}
	if (ret < 0) {
<<<<<<< HEAD
		printk(KERN_ERR "asoc: can't create pcm for codec %s\n", codec->name);
		return ret;
	}
	dev_dbg(rtd->card->dev, "registered pcm #%d %s\n",num, new_name);

	rtd->pcm = pcm;
	pcm->private_data = rtd;
	INIT_DELAYED_WORK(&rtd->delayed_work, close_delayed_work);

	substream[SNDRV_PCM_STREAM_PLAYBACK] =
			pcm->streams[SNDRV_PCM_STREAM_PLAYBACK].substream;
	substream[SNDRV_PCM_STREAM_CAPTURE] =
			pcm->streams[SNDRV_PCM_STREAM_CAPTURE].substream;

	if (rtd->dai_link->no_pcm) {
		if (playback)
			substream[SNDRV_PCM_STREAM_PLAYBACK]->private_data = rtd;
		if (capture)
			substream[SNDRV_PCM_STREAM_CAPTURE]->private_data = rtd;
		goto out;
	}

        /* setup any hostless PCMs - i.e. no host IO is performed */
	if (rtd->dai_link->no_host_mode) {
		if (substream[SNDRV_PCM_STREAM_PLAYBACK]) {
			substream[SNDRV_PCM_STREAM_PLAYBACK]->hw_no_buffer = 1;
			snd_soc_set_runtime_hwparams(
				substream[SNDRV_PCM_STREAM_PLAYBACK],
				&no_host_hardware);
		}
		if (substream[SNDRV_PCM_STREAM_CAPTURE]) {
			substream[SNDRV_PCM_STREAM_CAPTURE]->hw_no_buffer = 1;
			snd_soc_set_runtime_hwparams(
				substream[SNDRV_PCM_STREAM_CAPTURE],
				&no_host_hardware);
		}
	}

	/* ASoC PCM operations */
	if (rtd->dai_link->dynamic) {
		rtd->ops.open		= soc_dpcm_fe_dai_open;
		rtd->ops.hw_params	= soc_dpcm_fe_dai_hw_params;
		rtd->ops.prepare	= soc_dpcm_fe_dai_prepare;
		rtd->ops.trigger	= soc_dpcm_fe_dai_trigger;
		rtd->ops.hw_free	= soc_dpcm_fe_dai_hw_free;
		rtd->ops.close		= soc_dpcm_fe_dai_close;
=======
		dev_err(rtd->card->dev, "ASoC: can't create pcm for %s\n",
			rtd->dai_link->name);
		return ret;
	}
	dev_dbg(rtd->card->dev, "ASoC: registered pcm #%d %s\n",num, new_name);

	/* DAPM dai link stream work */
	INIT_DELAYED_WORK(&rtd->delayed_work, close_delayed_work);

	rtd->pcm = pcm;
	pcm->private_data = rtd;

	if (rtd->dai_link->no_pcm) {
		if (playback)
			pcm->streams[SNDRV_PCM_STREAM_PLAYBACK].substream->private_data = rtd;
		if (capture)
			pcm->streams[SNDRV_PCM_STREAM_CAPTURE].substream->private_data = rtd;
		goto out;
	}

	/* ASoC PCM operations */
	if (rtd->dai_link->dynamic) {
		rtd->ops.open		= dpcm_fe_dai_open;
		rtd->ops.hw_params	= dpcm_fe_dai_hw_params;
		rtd->ops.prepare	= dpcm_fe_dai_prepare;
		rtd->ops.trigger	= dpcm_fe_dai_trigger;
		rtd->ops.hw_free	= dpcm_fe_dai_hw_free;
		rtd->ops.close		= dpcm_fe_dai_close;
>>>>>>> refs/remotes/origin/master
		rtd->ops.pointer	= soc_pcm_pointer;
		rtd->ops.ioctl		= soc_pcm_ioctl;
	} else {
		rtd->ops.open		= soc_pcm_open;
		rtd->ops.hw_params	= soc_pcm_hw_params;
		rtd->ops.prepare	= soc_pcm_prepare;
		rtd->ops.trigger	= soc_pcm_trigger;
		rtd->ops.hw_free	= soc_pcm_hw_free;
		rtd->ops.close		= soc_pcm_close;
		rtd->ops.pointer	= soc_pcm_pointer;
		rtd->ops.ioctl		= soc_pcm_ioctl;
	}

	if (platform->driver->ops) {
		rtd->ops.ack		= platform->driver->ops->ack;
		rtd->ops.copy		= platform->driver->ops->copy;
		rtd->ops.silence	= platform->driver->ops->silence;
		rtd->ops.page		= platform->driver->ops->page;
		rtd->ops.mmap		= platform->driver->ops->mmap;
	}

	if (playback)
		snd_pcm_set_ops(pcm, SNDRV_PCM_STREAM_PLAYBACK, &rtd->ops);

	if (capture)
		snd_pcm_set_ops(pcm, SNDRV_PCM_STREAM_CAPTURE, &rtd->ops);

	if (platform->driver->pcm_new) {
		ret = platform->driver->pcm_new(rtd);
		if (ret < 0) {
<<<<<<< HEAD
			pr_err("asoc: platform pcm constructor failed\n");
=======
			dev_err(platform->dev,
				"ASoC: pcm constructor failed: %d\n",
				ret);
>>>>>>> refs/remotes/origin/master
			return ret;
		}
	}

	pcm->private_free = platform->driver->pcm_free;
out:
<<<<<<< HEAD
	printk(KERN_INFO "asoc: %s <-> %s mapping ok\n", codec_dai->name,
=======
	dev_info(rtd->card->dev, "%s <-> %s mapping ok\n", codec_dai->name,
>>>>>>> refs/remotes/origin/master
		cpu_dai->name);
	return ret;
}

<<<<<<< HEAD
=======
/* is the current PCM operation for this FE ? */
int snd_soc_dpcm_fe_can_update(struct snd_soc_pcm_runtime *fe, int stream)
{
	if (fe->dpcm[stream].runtime_update == SND_SOC_DPCM_UPDATE_FE)
		return 1;
	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_dpcm_fe_can_update);

/* is the current PCM operation for this BE ? */
int snd_soc_dpcm_be_can_update(struct snd_soc_pcm_runtime *fe,
		struct snd_soc_pcm_runtime *be, int stream)
{
	if ((fe->dpcm[stream].runtime_update == SND_SOC_DPCM_UPDATE_FE) ||
	   ((fe->dpcm[stream].runtime_update == SND_SOC_DPCM_UPDATE_BE) &&
		  be->dpcm[stream].runtime_update))
		return 1;
	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_dpcm_be_can_update);

/* get the substream for this BE */
struct snd_pcm_substream *
	snd_soc_dpcm_get_substream(struct snd_soc_pcm_runtime *be, int stream)
{
	return be->pcm->streams[stream].substream;
}
EXPORT_SYMBOL_GPL(snd_soc_dpcm_get_substream);

/* get the BE runtime state */
enum snd_soc_dpcm_state
	snd_soc_dpcm_be_get_state(struct snd_soc_pcm_runtime *be, int stream)
{
	return be->dpcm[stream].state;
}
EXPORT_SYMBOL_GPL(snd_soc_dpcm_be_get_state);

/* set the BE runtime state */
void snd_soc_dpcm_be_set_state(struct snd_soc_pcm_runtime *be,
		int stream, enum snd_soc_dpcm_state state)
{
	be->dpcm[stream].state = state;
}
EXPORT_SYMBOL_GPL(snd_soc_dpcm_be_set_state);

/*
 * We can only hw_free, stop, pause or suspend a BE DAI if any of it's FE
 * are not running, paused or suspended for the specified stream direction.
 */
int snd_soc_dpcm_can_be_free_stop(struct snd_soc_pcm_runtime *fe,
		struct snd_soc_pcm_runtime *be, int stream)
{
	struct snd_soc_dpcm *dpcm;
	int state;

	list_for_each_entry(dpcm, &be->dpcm[stream].fe_clients, list_fe) {

		if (dpcm->fe == fe)
			continue;

		state = dpcm->fe->dpcm[stream].state;
		if (state == SND_SOC_DPCM_STATE_START ||
			state == SND_SOC_DPCM_STATE_PAUSED ||
			state == SND_SOC_DPCM_STATE_SUSPEND)
			return 0;
	}

	/* it's safe to free/stop this BE DAI */
	return 1;
}
EXPORT_SYMBOL_GPL(snd_soc_dpcm_can_be_free_stop);

/*
 * We can only change hw params a BE DAI if any of it's FE are not prepared,
 * running, paused or suspended for the specified stream direction.
 */
int snd_soc_dpcm_can_be_params(struct snd_soc_pcm_runtime *fe,
		struct snd_soc_pcm_runtime *be, int stream)
{
	struct snd_soc_dpcm *dpcm;
	int state;

	list_for_each_entry(dpcm, &be->dpcm[stream].fe_clients, list_fe) {

		if (dpcm->fe == fe)
			continue;

		state = dpcm->fe->dpcm[stream].state;
		if (state == SND_SOC_DPCM_STATE_START ||
			state == SND_SOC_DPCM_STATE_PAUSED ||
			state == SND_SOC_DPCM_STATE_SUSPEND ||
			state == SND_SOC_DPCM_STATE_PREPARE)
			return 0;
	}

	/* it's safe to change hw_params */
	return 1;
}
EXPORT_SYMBOL_GPL(snd_soc_dpcm_can_be_params);

int snd_soc_platform_trigger(struct snd_pcm_substream *substream,
		int cmd, struct snd_soc_platform *platform)
{
	if (platform->driver->ops && platform->driver->ops->trigger)
		return platform->driver->ops->trigger(substream, cmd);
	return 0;
}
EXPORT_SYMBOL_GPL(snd_soc_platform_trigger);

>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_DEBUG_FS
static char *dpcm_state_string(enum snd_soc_dpcm_state state)
{
	switch (state) {
	case SND_SOC_DPCM_STATE_NEW:
		return "new";
	case SND_SOC_DPCM_STATE_OPEN:
		return "open";
	case SND_SOC_DPCM_STATE_HW_PARAMS:
		return "hw_params";
	case SND_SOC_DPCM_STATE_PREPARE:
		return "prepare";
	case SND_SOC_DPCM_STATE_START:
		return "start";
	case SND_SOC_DPCM_STATE_STOP:
		return "stop";
	case SND_SOC_DPCM_STATE_SUSPEND:
		return "suspend";
	case SND_SOC_DPCM_STATE_PAUSED:
		return "paused";
	case SND_SOC_DPCM_STATE_HW_FREE:
		return "hw_free";
	case SND_SOC_DPCM_STATE_CLOSE:
		return "close";
	}

	return "unknown";
}

<<<<<<< HEAD
static int soc_dpcm_state_open_file(struct inode *inode, struct file *file)
{
	file->private_data = inode->i_private;
	return 0;
}

static ssize_t soc_dpcm_show_state(struct snd_soc_pcm_runtime *fe,
				int stream, char *buf, size_t size)
{
	struct snd_pcm_hw_params *params = &fe->dpcm[stream].hw_params;
	struct snd_soc_dpcm_params *dpcm_params;
=======
static ssize_t dpcm_show_state(struct snd_soc_pcm_runtime *fe,
				int stream, char *buf, size_t size)
{
	struct snd_pcm_hw_params *params = &fe->dpcm[stream].hw_params;
	struct snd_soc_dpcm *dpcm;
>>>>>>> refs/remotes/origin/master
	ssize_t offset = 0;

	/* FE state */
	offset += snprintf(buf + offset, size - offset,
			"[%s - %s]\n", fe->dai_link->name,
			stream ? "Capture" : "Playback");

	offset += snprintf(buf + offset, size - offset, "State: %s\n",
	                dpcm_state_string(fe->dpcm[stream].state));

	if ((fe->dpcm[stream].state >= SND_SOC_DPCM_STATE_HW_PARAMS) &&
	    (fe->dpcm[stream].state <= SND_SOC_DPCM_STATE_STOP))
		offset += snprintf(buf + offset, size - offset,
				"Hardware Params: "
				"Format = %s, Channels = %d, Rate = %d\n",
				snd_pcm_format_name(params_format(params)),
				params_channels(params),
				params_rate(params));

	/* BEs state */
	offset += snprintf(buf + offset, size - offset, "Backends:\n");

	if (list_empty(&fe->dpcm[stream].be_clients)) {
		offset += snprintf(buf + offset, size - offset,
				" No active DSP links\n");
		goto out;
	}

<<<<<<< HEAD
	list_for_each_entry(dpcm_params, &fe->dpcm[stream].be_clients, list_be) {
		struct snd_soc_pcm_runtime *be = dpcm_params->be;
=======
	list_for_each_entry(dpcm, &fe->dpcm[stream].be_clients, list_be) {
		struct snd_soc_pcm_runtime *be = dpcm->be;
		params = &dpcm->hw_params;
>>>>>>> refs/remotes/origin/master

		offset += snprintf(buf + offset, size - offset,
				"- %s\n", be->dai_link->name);

		offset += snprintf(buf + offset, size - offset,
				"   State: %s\n",
<<<<<<< HEAD
				dpcm_state_string(fe->dpcm[stream].state));
=======
				dpcm_state_string(be->dpcm[stream].state));
>>>>>>> refs/remotes/origin/master

		if ((be->dpcm[stream].state >= SND_SOC_DPCM_STATE_HW_PARAMS) &&
		    (be->dpcm[stream].state <= SND_SOC_DPCM_STATE_STOP))
			offset += snprintf(buf + offset, size - offset,
				"   Hardware Params: "
				"Format = %s, Channels = %d, Rate = %d\n",
				snd_pcm_format_name(params_format(params)),
				params_channels(params),
				params_rate(params));
	}

out:
	return offset;
}

<<<<<<< HEAD
static ssize_t soc_dpcm_state_read_file(struct file *file, char __user *user_buf,
=======
static ssize_t dpcm_state_read_file(struct file *file, char __user *user_buf,
>>>>>>> refs/remotes/origin/master
				size_t count, loff_t *ppos)
{
	struct snd_soc_pcm_runtime *fe = file->private_data;
	ssize_t out_count = PAGE_SIZE, offset = 0, ret = 0;
	char *buf;

	buf = kmalloc(out_count, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	if (fe->cpu_dai->driver->playback.channels_min)
<<<<<<< HEAD
		offset += soc_dpcm_show_state(fe, SNDRV_PCM_STREAM_PLAYBACK,
					buf + offset, out_count - offset);

	if (fe->cpu_dai->driver->capture.channels_min)
		offset += soc_dpcm_show_state(fe, SNDRV_PCM_STREAM_CAPTURE,
					buf + offset, out_count - offset);

        ret = simple_read_from_buffer(user_buf, count, ppos, buf, offset);

        kfree(buf);

        return ret;
}

static const struct file_operations soc_dpcm_state_fops = {
	.open = soc_dpcm_state_open_file,
	.read = soc_dpcm_state_read_file,
=======
		offset += dpcm_show_state(fe, SNDRV_PCM_STREAM_PLAYBACK,
					buf + offset, out_count - offset);

	if (fe->cpu_dai->driver->capture.channels_min)
		offset += dpcm_show_state(fe, SNDRV_PCM_STREAM_CAPTURE,
					buf + offset, out_count - offset);

	ret = simple_read_from_buffer(user_buf, count, ppos, buf, offset);

	kfree(buf);
	return ret;
}

static const struct file_operations dpcm_state_fops = {
	.open = simple_open,
	.read = dpcm_state_read_file,
>>>>>>> refs/remotes/origin/master
	.llseek = default_llseek,
};

int soc_dpcm_debugfs_add(struct snd_soc_pcm_runtime *rtd)
{
<<<<<<< HEAD
=======
	if (!rtd->dai_link)
		return 0;

>>>>>>> refs/remotes/origin/master
	rtd->debugfs_dpcm_root = debugfs_create_dir(rtd->dai_link->name,
			rtd->card->debugfs_card_root);
	if (!rtd->debugfs_dpcm_root) {
		dev_dbg(rtd->dev,
			 "ASoC: Failed to create dpcm debugfs directory %s\n",
			 rtd->dai_link->name);
		return -EINVAL;
	}

<<<<<<< HEAD
	rtd->debugfs_dpcm_state = debugfs_create_file("state", 0644,
						rtd->debugfs_dpcm_root,
						rtd, &soc_dpcm_state_fops);
=======
	rtd->debugfs_dpcm_state = debugfs_create_file("state", 0444,
						rtd->debugfs_dpcm_root,
						rtd, &dpcm_state_fops);
>>>>>>> refs/remotes/origin/master

	return 0;
}
#endif
