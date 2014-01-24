/*
 * omap-dmic.c  --  OMAP ASoC DMIC DAI driver
 *
<<<<<<< HEAD
 * Copyright (C) 2010 Texas Instruments
 *
 * Author: Liam Girdwood <lrg@ti.com>
 *	   David Lambert <dlambert@ti.com>
 *	   Misael Lopez Cruz <misael.lopez@ti.com>
=======
 * Copyright (C) 2010 - 2011 Texas Instruments
 *
 * Author: David Lambert <dlambert@ti.com>
 *	   Misael Lopez Cruz <misael.lopez@ti.com>
 *	   Liam Girdwood <lrg@ti.com>
 *	   Peter Ujfalusi <peter.ujfalusi@ti.com>
>>>>>>> refs/remotes/origin/cm-10.0
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

<<<<<<< HEAD
#undef DEBUG

=======
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/pm_runtime.h>
<<<<<<< HEAD

#include <plat/dma.h>
#include <plat/dmic.h>
=======
#include <plat/dma.h>
>>>>>>> refs/remotes/origin/cm-10.0

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/initval.h>
#include <sound/soc.h>

#include "omap-pcm.h"
#include "omap-dmic.h"

<<<<<<< HEAD
#define OMAP_DMIC_RATES		(SNDRV_PCM_RATE_96000 | SNDRV_PCM_RATE_192000)
#define OMAP_DMIC_FORMATS	SNDRV_PCM_FMTBIT_S32_LE

#define OMAP4_LEGACY_DMIC0		0
#define OMAP4_ABE_DMIC0		1
#define OMAP4_ABE_DMIC1		2
#define OMAP4_ABE_DMIC2		3

struct omap_dmic {
	struct device *dev;
	void __iomem *io_base;
	int clk_freq;
	int sysclk;
	int active;
	int running;
	int channels;
	int abe_mode;
	u32 up_enable;
=======
struct omap_dmic {
	struct device *dev;
	void __iomem *io_base;
	struct clk *fclk;
	int fclk_freq;
	int out_freq;
	int clk_div;
	int sysclk;
	int threshold;
	u32 ch_enabled;
	bool active;
>>>>>>> refs/remotes/origin/cm-10.0
	struct mutex mutex;
};

/*
 * Stream DMA parameters
 */
static struct omap_pcm_dma_data omap_dmic_dai_dma_params = {
	.name		= "DMIC capture",
	.data_type	= OMAP_DMA_DATA_TYPE_S32,
	.sync_mode	= OMAP_DMA_SYNC_PACKET,
<<<<<<< HEAD
	.packet_size	= 2,
	.port_addr	= OMAP44XX_DMIC_L3_BASE + OMAP_DMIC_DATA,
=======
>>>>>>> refs/remotes/origin/cm-10.0
};

static inline void omap_dmic_write(struct omap_dmic *dmic, u16 reg, u32 val)
{
	__raw_writel(val, dmic->io_base + reg);
}

static inline int omap_dmic_read(struct omap_dmic *dmic, u16 reg)
{
	return __raw_readl(dmic->io_base + reg);
}

<<<<<<< HEAD
/*
 * Enables and disables DMIC channels through the DMIC interface
 */
static inline void dmic_set_up_channels(struct omap_dmic *dmic)
{
	u32 ctrl = omap_dmic_read(dmic, OMAP_DMIC_CTRL) & ~OMAP_DMIC_UP_ENABLE_MASK;
	omap_dmic_write(dmic, OMAP_DMIC_CTRL, ctrl | dmic->up_enable);
}

static inline int dmic_is_enabled(struct omap_dmic *dmic)
{
	return omap_dmic_read(dmic, OMAP_DMIC_CTRL) & OMAP_DMIC_UP_ENABLE_MASK;
}

static int omap_dmic_set_clkdiv(struct snd_soc_dai *dai,
				int div_id, int div)
{
	struct omap_dmic *dmic = snd_soc_dai_get_drvdata(dai);
	int div_sel = -EINVAL;
	u32 ctrl;

	if (div_id != OMAP_DMIC_CLKDIV)
		return -ENODEV;

	switch (dmic->clk_freq) {
	case 19200000:
		switch (div) {
		case 5:
			div_sel = 0x1;
			break;
		case 8:
			div_sel = 0x0;
			break;
		default:
			dev_err(dai->dev, "invalid div_sel (%d) for 19200000Hz", div);
			return -EINVAL;
		}
		break;
	case 24000000:
		switch (div) {
		case 10:
			div_sel = 0x2;
			break;
		default:
			dev_err(dai->dev, "invalid div_sel (%d) for 24000000Hz", div);
			return -EINVAL;
		}
		break;
	case 24576000:
		switch (div) {
		case 8:
			div_sel = 0x3;
			break;
		case 16:
			div_sel = 0x4;
			break;
		default:
			dev_err(dai->dev, "invalid div_sel (%d) for 24576000Hz", div);
			return -EINVAL;
		}
		break;
	case 12000000:
		switch (div) {
		case 5:
			div_sel = 0x5;
			break;
		default:
			dev_err(dai->dev, "invalid div_sel (%d) for 12000000Hz", div);
			return -EINVAL;
		}
		break;
	default:
		dev_err(dai->dev, "invalid freq %d\n", dmic->clk_freq);
		return -EINVAL;
	}

	if (div_sel < 0) {
		dev_err(dai->dev, "divider not supported %d\n", div);
		return -EINVAL;
	}

	ctrl = omap_dmic_read(dmic, OMAP_DMIC_CTRL) & ~OMAP_DMIC_CLK_DIV_MASK;

	omap_dmic_write(dmic, OMAP_DMIC_CTRL,
			ctrl | (div_sel << OMAP_DMIC_CLK_DIV_SHIFT));

	return 0;
}

/*
 * Configures DMIC for audio recording.
 * This function should be called before omap_dmic_start.
 */
static void omap_dmic_open(struct omap_dmic *dmic)
{
	u32 ctrl;

	/* Configure uplink threshold */
	omap_dmic_write(dmic, OMAP_DMIC_FIFO_CTRL, 2);

	/* Set dmic out format */
	ctrl = omap_dmic_read(dmic, OMAP_DMIC_CTRL)
		& ~(OMAP_DMIC_FORMAT | OMAP_DMIC_POLAR_MASK);
	omap_dmic_write(dmic, OMAP_DMIC_CTRL,
			ctrl | OMAP_DMICOUTFORMAT_LJUST |
			OMAP_DMIC_POLAR1 | OMAP_DMIC_POLAR2 | OMAP_DMIC_POLAR3);
}

/*
 * Cleans DMIC uplink configuration.
 * This function should be called when the stream is closed.
 */
static void omap_dmic_close(struct omap_dmic *dmic)
{
	/* Disable DMA request generation */
	omap_dmic_write(dmic, OMAP_DMIC_DMAENABLE_CLR, OMAP_DMIC_DMA_ENABLE);

=======
static inline void omap_dmic_start(struct omap_dmic *dmic)
{
	u32 ctrl = omap_dmic_read(dmic, OMAP_DMIC_CTRL_REG);

	/* Configure DMA controller */
	omap_dmic_write(dmic, OMAP_DMIC_DMAENABLE_SET_REG,
			OMAP_DMIC_DMA_ENABLE);

	omap_dmic_write(dmic, OMAP_DMIC_CTRL_REG, ctrl | dmic->ch_enabled);
}

static inline void omap_dmic_stop(struct omap_dmic *dmic)
{
	u32 ctrl = omap_dmic_read(dmic, OMAP_DMIC_CTRL_REG);
	omap_dmic_write(dmic, OMAP_DMIC_CTRL_REG,
			ctrl & ~OMAP_DMIC_UP_ENABLE_MASK);

	/* Disable DMA request generation */
	omap_dmic_write(dmic, OMAP_DMIC_DMAENABLE_CLR_REG,
			OMAP_DMIC_DMA_ENABLE);

}

static inline int dmic_is_enabled(struct omap_dmic *dmic)
{
	return omap_dmic_read(dmic, OMAP_DMIC_CTRL_REG) &
						OMAP_DMIC_UP_ENABLE_MASK;
>>>>>>> refs/remotes/origin/cm-10.0
}

static int omap_dmic_dai_startup(struct snd_pcm_substream *substream,
				  struct snd_soc_dai *dai)
{
	struct omap_dmic *dmic = snd_soc_dai_get_drvdata(dai);
	int ret = 0;

	mutex_lock(&dmic->mutex);

<<<<<<< HEAD
	if (!dmic->active) {
		pm_runtime_get_sync(dmic->dev);

		if (dai->id > OMAP4_LEGACY_DMIC0)
			dmic->abe_mode = 1;

		omap_dmic_open(dmic);
	} else {
		/* legacy and ABE mode are mutually exclusive */
		if (dai->id > OMAP4_LEGACY_DMIC0 && !dmic->abe_mode) {
			ret = -EBUSY;
			goto out;
		}
	}

	dmic->active++;

out:
	mutex_unlock(&dmic->mutex);
=======
	if (!dai->active)
		dmic->active = 1;
	else
		ret = -EBUSY;

	mutex_unlock(&dmic->mutex);

>>>>>>> refs/remotes/origin/cm-10.0
	return ret;
}

static void omap_dmic_dai_shutdown(struct snd_pcm_substream *substream,
				    struct snd_soc_dai *dai)
{
	struct omap_dmic *dmic = snd_soc_dai_get_drvdata(dai);

	mutex_lock(&dmic->mutex);

<<<<<<< HEAD
	if (--dmic->active == 0) {
		omap_dmic_close(dmic);
		pm_runtime_put_sync(dmic->dev);
		dmic->abe_mode = 0;
	}
=======
	if (!dai->active)
		dmic->active = 0;
>>>>>>> refs/remotes/origin/cm-10.0

	mutex_unlock(&dmic->mutex);
}

<<<<<<< HEAD
=======
static int omap_dmic_select_divider(struct omap_dmic *dmic, int sample_rate)
{
	int divider = -EINVAL;

	/*
	 * 192KHz rate is only supported with 19.2MHz/3.84MHz clock
	 * configuration.
	 */
	if (sample_rate == 192000) {
		if (dmic->fclk_freq == 19200000 && dmic->out_freq == 3840000)
			divider = 0x6; /* Divider: 5 (192KHz sampling rate) */
		else
			dev_err(dmic->dev,
				"invalid clock configuration for 192KHz\n");

		return divider;
	}

	switch (dmic->out_freq) {
	case 1536000:
		if (dmic->fclk_freq != 24576000)
			goto div_err;
		divider = 0x4; /* Divider: 16 */
		break;
	case 2400000:
		switch (dmic->fclk_freq) {
		case 12000000:
			divider = 0x5; /* Divider: 5 */
			break;
		case 19200000:
			divider = 0x0; /* Divider: 8 */
			break;
		case 24000000:
			divider = 0x2; /* Divider: 10 */
			break;
		default:
			goto div_err;
		}
		break;
	case 3072000:
		if (dmic->fclk_freq != 24576000)
			goto div_err;
		divider = 0x3; /* Divider: 8 */
		break;
	case 3840000:
		if (dmic->fclk_freq != 19200000)
			goto div_err;
		divider = 0x1; /* Divider: 5 (96KHz sampling rate) */
		break;
	default:
		dev_err(dmic->dev, "invalid out frequency: %dHz\n",
			dmic->out_freq);
		break;
	}

	return divider;

div_err:
	dev_err(dmic->dev, "invalid out frequency %dHz for %dHz input\n",
		dmic->out_freq, dmic->fclk_freq);
	return -EINVAL;
}

>>>>>>> refs/remotes/origin/cm-10.0
static int omap_dmic_dai_hw_params(struct snd_pcm_substream *substream,
				    struct snd_pcm_hw_params *params,
				    struct snd_soc_dai *dai)
{
	struct omap_dmic *dmic = snd_soc_dai_get_drvdata(dai);
<<<<<<< HEAD
	int channels, rate, div;
	int ret = 0;

	channels = params_channels(params);
	if (dai->id == OMAP4_LEGACY_DMIC0) {
		switch (channels) {
		case 2:
		case 4:
		case 6:
			dmic->channels = channels;
			break;
		default:
			dev_err(dmic->dev, "invalid number of legacy channels\n");
			return -EINVAL;
		}
	} else {
		if (channels != 2) {
			dev_err(dmic->dev, "invalid number of ABE channels\n");
			return -EINVAL;
		}
	}

	rate = params_rate(params);
	switch (rate) {
	case 96000:
		div = 8;
		break;
	case 192000:
		div = 5;
		break;
	default:
		dev_err(dmic->dev, "rate %d not supported\n", rate);
=======
	int channels;

	dmic->clk_div = omap_dmic_select_divider(dmic, params_rate(params));
	if (dmic->clk_div < 0) {
		dev_err(dmic->dev, "no valid divider for %dHz from %dHz\n",
			dmic->out_freq, dmic->fclk_freq);
		return -EINVAL;
	}

	dmic->ch_enabled = 0;
	channels = params_channels(params);
	switch (channels) {
	case 6:
		dmic->ch_enabled |= OMAP_DMIC_UP3_ENABLE;
	case 4:
		dmic->ch_enabled |= OMAP_DMIC_UP2_ENABLE;
	case 2:
		dmic->ch_enabled |= OMAP_DMIC_UP1_ENABLE;
		break;
	default:
		dev_err(dmic->dev, "invalid number of legacy channels\n");
>>>>>>> refs/remotes/origin/cm-10.0
		return -EINVAL;
	}

	/* packet size is threshold * channels */
<<<<<<< HEAD
	omap_dmic_dai_dma_params.packet_size = 2 * channels;
	snd_soc_dai_set_dma_data(dai, substream, &omap_dmic_dai_dma_params);

	return ret;
}

static void dmic_config_up_channels(struct omap_dmic *dmic, int dai_id,
		int enable)
{
	if (enable) {
		switch (dai_id) {
		case OMAP4_LEGACY_DMIC0:
			switch (dmic->channels) {
			case 6:
				dmic->up_enable = OMAP_DMIC_UP1_ENABLE | OMAP_DMIC_UP2_ENABLE
					 | OMAP_DMIC_UP3_ENABLE;
				break;
			case 4:
				dmic->up_enable = OMAP_DMIC_UP1_ENABLE | OMAP_DMIC_UP2_ENABLE;
				break;
			case 2:
				dmic->up_enable = OMAP_DMIC_UP1_ENABLE;
				break;
			default:
				break;
			}
			break;
		case OMAP4_ABE_DMIC0:
		case OMAP4_ABE_DMIC1:
		case OMAP4_ABE_DMIC2:
			/*
			 * ABE expects all the DMIC interfaces to be
			 * enabled, so enabling them when at least one
			 * DMIC DAI is running
			 */
			if (dmic->running)
				dmic->up_enable |= OMAP_DMIC_UP1_ENABLE |
					OMAP_DMIC_UP2_ENABLE |
					OMAP_DMIC_UP3_ENABLE;
			break;
		default:
			break;
		}
	} else {
		switch (dai_id) {
		case OMAP4_LEGACY_DMIC0:
			dmic->up_enable = 0;
			break;
		case OMAP4_ABE_DMIC0:
		case OMAP4_ABE_DMIC1:
		case OMAP4_ABE_DMIC2:
			/*
			 * Disable all DMIC interfaces only when
			 * all DAIs are stopped
			 */
			if (!dmic->running)
				dmic->up_enable = 0;
			break;
		default:
			break;
		}
	}
=======
	omap_dmic_dai_dma_params.packet_size = dmic->threshold * channels;
	snd_soc_dai_set_dma_data(dai, substream, &omap_dmic_dai_dma_params);

	return 0;
>>>>>>> refs/remotes/origin/cm-10.0
}

static int omap_dmic_dai_prepare(struct snd_pcm_substream *substream,
				  struct snd_soc_dai *dai)
{
	struct omap_dmic *dmic = snd_soc_dai_get_drvdata(dai);
<<<<<<< HEAD

	/* Configure DMA controller */
	omap_dmic_write(dmic, OMAP_DMIC_DMAENABLE_SET, OMAP_DMIC_DMA_ENABLE);
=======
	u32 ctrl;

	/* Configure uplink threshold */
	omap_dmic_write(dmic, OMAP_DMIC_FIFO_CTRL_REG, dmic->threshold);

	ctrl = omap_dmic_read(dmic, OMAP_DMIC_CTRL_REG);

	/* Set dmic out format */
	ctrl &= ~(OMAP_DMIC_FORMAT | OMAP_DMIC_POLAR_MASK);
	ctrl |= (OMAP_DMICOUTFORMAT_LJUST | OMAP_DMIC_POLAR1 |
		 OMAP_DMIC_POLAR2 | OMAP_DMIC_POLAR3);

	/* Configure dmic clock divider */
	ctrl &= ~OMAP_DMIC_CLK_DIV_MASK;
	ctrl |= OMAP_DMIC_CLK_DIV(dmic->clk_div);

	omap_dmic_write(dmic, OMAP_DMIC_CTRL_REG, ctrl);

	omap_dmic_write(dmic, OMAP_DMIC_CTRL_REG,
			ctrl | OMAP_DMICOUTFORMAT_LJUST | OMAP_DMIC_POLAR1 |
			OMAP_DMIC_POLAR2 | OMAP_DMIC_POLAR3);
>>>>>>> refs/remotes/origin/cm-10.0

	return 0;
}

static int omap_dmic_dai_trigger(struct snd_pcm_substream *substream,
				  int cmd, struct snd_soc_dai *dai)
{
	struct omap_dmic *dmic = snd_soc_dai_get_drvdata(dai);

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
<<<<<<< HEAD
		dmic->running++;
		dmic_config_up_channels(dmic, dai->id, 1);
		dmic_set_up_channels(dmic);
		break;
	case SNDRV_PCM_TRIGGER_STOP:
		dmic->running--;
		dmic_config_up_channels(dmic, dai->id, 0);
		dmic_set_up_channels(dmic);
=======
		omap_dmic_start(dmic);
		break;
	case SNDRV_PCM_TRIGGER_STOP:
		omap_dmic_stop(dmic);
>>>>>>> refs/remotes/origin/cm-10.0
		break;
	default:
		break;
	}

	return 0;
}

<<<<<<< HEAD
static int omap_dmic_set_dai_sysclk(struct snd_soc_dai *dai,
				    int clk_id, unsigned int freq,
				    int dir)
{
	struct omap_dmic *dmic = snd_soc_dai_get_drvdata(dai);
	struct clk *dmic_clk, *parent_clk;
	int ret = 0;

	dmic_clk = clk_get(NULL, "dmic_fck");
	if (IS_ERR(dmic_clk)) {
		dev_err(dmic->dev, "cant get dmic_fck\n");
		return -ENODEV;
=======
static int omap_dmic_select_fclk(struct omap_dmic *dmic, int clk_id,
				 unsigned int freq)
{
	struct clk *parent_clk;
	char *parent_clk_name;
	int ret = 0;

	switch (freq) {
	case 12000000:
	case 19200000:
	case 24000000:
	case 24576000:
		break;
	default:
		dev_err(dmic->dev, "invalid input frequency: %dHz\n", freq);
		dmic->fclk_freq = 0;
		return -EINVAL;
	}

	if (dmic->sysclk == clk_id) {
		dmic->fclk_freq = freq;
		return 0;
	}

	/* re-parent not allowed if a stream is ongoing */
	if (dmic->active && dmic_is_enabled(dmic)) {
		dev_err(dmic->dev, "can't re-parent when DMIC active\n");
		return -EBUSY;
>>>>>>> refs/remotes/origin/cm-10.0
	}

	switch (clk_id) {
	case OMAP_DMIC_SYSCLK_PAD_CLKS:
<<<<<<< HEAD
		parent_clk = clk_get(NULL, "pad_clks_ck");
		if (IS_ERR(parent_clk)) {
			dev_err(dmic->dev, "cant get pad_clks_ck\n");
			ret = -ENODEV;
			goto err_par;
		}
		break;
	case OMAP_DMIC_SYSCLK_SLIMBLUS_CLKS:
		parent_clk = clk_get(NULL, "slimbus_clk");
		if (IS_ERR(parent_clk)) {
			dev_err(dmic->dev, "cant get slimbus_clk\n");
			ret = -ENODEV;
			goto err_par;
		}
		break;
	case OMAP_DMIC_SYSCLK_SYNC_MUX_CLKS:
		parent_clk = clk_get(NULL, "dmic_sync_mux_ck");
		if (IS_ERR(parent_clk)) {
			dev_err(dmic->dev, "cant get dmic_sync_mux_ck\n");
			ret = -ENODEV;
			goto err_par;
		}
		break;
	default:
		dev_err(dai->dev, "clk_id not supported %d\n", clk_id);
		ret = -EINVAL;
		goto err_par;
	}

	if (dmic->sysclk != clk_id) {
		/* re-parent not allowed if a stream is ongoing */
		if (dmic_is_enabled(dmic)) {
			dev_err(dmic->dev, "cant re-parent when DMIC active\n");
			ret = -EBUSY;
			goto err_busy;
		}

		/* disable clock while reparenting */
		pm_runtime_put_sync(dmic->dev);
		ret = clk_set_parent(dmic_clk, parent_clk);
		pm_runtime_get_sync(dmic->dev);
		if (ret < 0) {
			dev_err(dmic->dev, "re-parent failed\n");
			goto err_busy;
		}

		dmic->sysclk = clk_id;

		//ret = clk_set_rate(dmic_clk, freq);
		if (ret < 0)
			dev_err(dmic->dev, "clock set to %d Hz failed\n", freq);
		else
			dmic->clk_freq = freq;
	}

err_busy:
	clk_put(parent_clk);
err_par:
	clk_put(dmic_clk);
=======
		parent_clk_name = "pad_clks_ck";
		break;
	case OMAP_DMIC_SYSCLK_SLIMBLUS_CLKS:
		parent_clk_name = "slimbus_clk";
		break;
	case OMAP_DMIC_SYSCLK_SYNC_MUX_CLKS:
		parent_clk_name = "dmic_sync_mux_ck";
		break;
	default:
		dev_err(dmic->dev, "fclk clk_id (%d) not supported\n", clk_id);
		return -EINVAL;
	}

	parent_clk = clk_get(dmic->dev, parent_clk_name);
	if (IS_ERR(parent_clk)) {
		dev_err(dmic->dev, "can't get %s\n", parent_clk_name);
		return -ENODEV;
	}

	mutex_lock(&dmic->mutex);
	if (dmic->active) {
		/* disable clock while reparenting */
		pm_runtime_put_sync(dmic->dev);
		ret = clk_set_parent(dmic->fclk, parent_clk);
		pm_runtime_get_sync(dmic->dev);
	} else {
		ret = clk_set_parent(dmic->fclk, parent_clk);
	}
	mutex_unlock(&dmic->mutex);

	if (ret < 0) {
		dev_err(dmic->dev, "re-parent failed\n");
		goto err_busy;
	}

	dmic->sysclk = clk_id;
	dmic->fclk_freq = freq;

err_busy:
	clk_put(parent_clk);
>>>>>>> refs/remotes/origin/cm-10.0

	return ret;
}

<<<<<<< HEAD
static struct snd_soc_dai_ops omap_dmic_dai_ops = {
=======
static int omap_dmic_select_outclk(struct omap_dmic *dmic, int clk_id,
				    unsigned int freq)
{
	int ret = 0;

	if (clk_id != OMAP_DMIC_ABE_DMIC_CLK) {
		dev_err(dmic->dev, "output clk_id (%d) not supported\n",
			clk_id);
		return -EINVAL;
	}

	switch (freq) {
	case 1536000:
	case 2400000:
	case 3072000:
	case 3840000:
		dmic->out_freq = freq;
		break;
	default:
		dev_err(dmic->dev, "invalid out frequency: %dHz\n", freq);
		dmic->out_freq = 0;
		ret = -EINVAL;
	}

	return ret;
}

static int omap_dmic_set_dai_sysclk(struct snd_soc_dai *dai, int clk_id,
				    unsigned int freq, int dir)
{
	struct omap_dmic *dmic = snd_soc_dai_get_drvdata(dai);

	if (dir == SND_SOC_CLOCK_IN)
		return omap_dmic_select_fclk(dmic, clk_id, freq);
	else if (dir == SND_SOC_CLOCK_OUT)
		return omap_dmic_select_outclk(dmic, clk_id, freq);

	dev_err(dmic->dev, "invalid clock direction (%d)\n", dir);
	return -EINVAL;
}

static const struct snd_soc_dai_ops omap_dmic_dai_ops = {
>>>>>>> refs/remotes/origin/cm-10.0
	.startup	= omap_dmic_dai_startup,
	.shutdown	= omap_dmic_dai_shutdown,
	.hw_params	= omap_dmic_dai_hw_params,
	.prepare	= omap_dmic_dai_prepare,
	.trigger	= omap_dmic_dai_trigger,
	.set_sysclk	= omap_dmic_set_dai_sysclk,
<<<<<<< HEAD
	.set_clkdiv	= omap_dmic_set_clkdiv,
};

static struct snd_soc_dai_driver omap_dmic_dai[] = {
{
	.name = "omap-dmic-dai-0",
	.id	= OMAP4_LEGACY_DMIC0,
	.capture = {
		.channels_min = 2,
		.channels_max = 6,
		.rates = OMAP_DMIC_RATES,
		.formats = OMAP_DMIC_FORMATS,
	},
	.ops = &omap_dmic_dai_ops,
},
{
	.name = "omap-dmic-abe-dai-0",
	.id	= OMAP4_ABE_DMIC0,
	.capture = {
		.channels_min = 2,
		.channels_max = 2,
		.rates = OMAP_DMIC_RATES,
		.formats = OMAP_DMIC_FORMATS,
	},
	.ops = &omap_dmic_dai_ops,
},
{
	.name = "omap-dmic-abe-dai-1",
	.id	= OMAP4_ABE_DMIC1,
	.capture = {
		.channels_min = 2,
		.channels_max = 2,
		.rates = OMAP_DMIC_RATES,
		.formats = OMAP_DMIC_FORMATS,
	},
	.ops = &omap_dmic_dai_ops,
},
{
	.name = "omap-dmic-abe-dai-2",
	.id	= OMAP4_ABE_DMIC2,
	.capture = {
		.channels_min = 2,
		.channels_max = 2,
		.rates = OMAP_DMIC_RATES,
		.formats = OMAP_DMIC_FORMATS,
	},
	.ops = &omap_dmic_dai_ops,
},
=======
};

static int omap_dmic_probe(struct snd_soc_dai *dai)
{
	struct omap_dmic *dmic = snd_soc_dai_get_drvdata(dai);

	pm_runtime_enable(dmic->dev);

	/* Disable lines while request is ongoing */
	pm_runtime_get_sync(dmic->dev);
	omap_dmic_write(dmic, OMAP_DMIC_CTRL_REG, 0x00);
	pm_runtime_put_sync(dmic->dev);

	/* Configure DMIC threshold value */
	dmic->threshold = OMAP_DMIC_THRES_MAX - 3;
	return 0;
}

static int omap_dmic_remove(struct snd_soc_dai *dai)
{
	struct omap_dmic *dmic = snd_soc_dai_get_drvdata(dai);

	pm_runtime_disable(dmic->dev);

	return 0;
}

static struct snd_soc_dai_driver omap_dmic_dai = {
	.name = "omap-dmic",
	.probe = omap_dmic_probe,
	.remove = omap_dmic_remove,
	.capture = {
		.channels_min = 2,
		.channels_max = 6,
		.rates = SNDRV_PCM_RATE_96000 | SNDRV_PCM_RATE_192000,
		.formats = SNDRV_PCM_FMTBIT_S32_LE,
		.sig_bits = 24,
	},
	.ops = &omap_dmic_dai_ops,
>>>>>>> refs/remotes/origin/cm-10.0
};

static __devinit int asoc_dmic_probe(struct platform_device *pdev)
{
	struct omap_dmic *dmic;
	struct resource *res;
	int ret;

<<<<<<< HEAD
	dmic = kzalloc(sizeof(struct omap_dmic), GFP_KERNEL);
=======
	dmic = devm_kzalloc(&pdev->dev, sizeof(struct omap_dmic), GFP_KERNEL);
>>>>>>> refs/remotes/origin/cm-10.0
	if (!dmic)
		return -ENOMEM;

	platform_set_drvdata(pdev, dmic);
	dmic->dev = &pdev->dev;
	dmic->sysclk = OMAP_DMIC_SYSCLK_SYNC_MUX_CLKS;

	mutex_init(&dmic->mutex);

<<<<<<< HEAD
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(dmic->dev, "invalid memory resource\n");
		ret = -ENODEV;
		goto err_res;
	}

	dmic->io_base = ioremap(res->start, resource_size(res));
	if (!dmic->io_base) {
		ret = -ENOMEM;
		goto err_res;
	}
=======
	dmic->fclk = clk_get(dmic->dev, "dmic_fck");
	if (IS_ERR(dmic->fclk)) {
		dev_err(dmic->dev, "cant get dmic_fck\n");
		return -ENODEV;
	}

	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "dma");
	if (!res) {
		dev_err(dmic->dev, "invalid dma memory resource\n");
		ret = -ENODEV;
		goto err_put_clk;
	}
	omap_dmic_dai_dma_params.port_addr = res->start + OMAP_DMIC_DATA_REG;
>>>>>>> refs/remotes/origin/cm-10.0

	res = platform_get_resource(pdev, IORESOURCE_DMA, 0);
	if (!res) {
		dev_err(dmic->dev, "invalid dma resource\n");
		ret = -ENODEV;
<<<<<<< HEAD
		goto err_dai;
	}
	omap_dmic_dai_dma_params.dma_req = res->start;

	pm_runtime_enable(dmic->dev);

	/* Disable lines while request is ongoing */
	omap_dmic_write(dmic, OMAP_DMIC_CTRL, 0x00);

	ret = snd_soc_register_dais(&pdev->dev, omap_dmic_dai,
			ARRAY_SIZE(omap_dmic_dai));
	if (ret)
		goto err_dai;

	return 0;

err_dai:
	iounmap(dmic->io_base);
err_res:
	kfree(dmic);
=======
		goto err_put_clk;
	}
	omap_dmic_dai_dma_params.dma_req = res->start;

	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "mpu");
	if (!res) {
		dev_err(dmic->dev, "invalid memory resource\n");
		ret = -ENODEV;
		goto err_put_clk;
	}

	if (!devm_request_mem_region(&pdev->dev, res->start,
				     resource_size(res), pdev->name)) {
		dev_err(dmic->dev, "memory region already claimed\n");
		ret = -ENODEV;
		goto err_put_clk;
	}

	dmic->io_base = devm_ioremap(&pdev->dev, res->start,
				     resource_size(res));
	if (!dmic->io_base) {
		ret = -ENOMEM;
		goto err_put_clk;
	}

	ret = snd_soc_register_dai(&pdev->dev, &omap_dmic_dai);
	if (ret)
		goto err_put_clk;

	return 0;

err_put_clk:
	clk_put(dmic->fclk);
>>>>>>> refs/remotes/origin/cm-10.0
	return ret;
}

static int __devexit asoc_dmic_remove(struct platform_device *pdev)
{
	struct omap_dmic *dmic = platform_get_drvdata(pdev);

<<<<<<< HEAD
	snd_soc_unregister_dais(&pdev->dev, ARRAY_SIZE(omap_dmic_dai));
	iounmap(dmic->io_base);
	pm_runtime_disable(dmic->dev);
	kfree(dmic);
=======
	snd_soc_unregister_dai(&pdev->dev);
	clk_put(dmic->fclk);
>>>>>>> refs/remotes/origin/cm-10.0

	return 0;
}

static struct platform_driver asoc_dmic_driver = {
	.driver = {
<<<<<<< HEAD
		.name = "omap-dmic-dai",
=======
		.name = "omap-dmic",
>>>>>>> refs/remotes/origin/cm-10.0
		.owner = THIS_MODULE,
	},
	.probe = asoc_dmic_probe,
	.remove = __devexit_p(asoc_dmic_remove),
};

<<<<<<< HEAD
static int __init snd_omap_dmic_init(void)
{
	return platform_driver_register(&asoc_dmic_driver);
}
module_init(snd_omap_dmic_init);

static void __exit snd_omap_dmic_exit(void)
{
	platform_driver_unregister(&asoc_dmic_driver);
}
module_exit(snd_omap_dmic_exit);

MODULE_ALIAS("platform:omap-dmic-dai");
MODULE_AUTHOR("David Lambert <dlambert@ti.com>");
=======
module_platform_driver(asoc_dmic_driver);

MODULE_ALIAS("platform:omap-dmic");
MODULE_AUTHOR("Peter Ujfalusi <peter.ujfalusi@ti.com>");
>>>>>>> refs/remotes/origin/cm-10.0
MODULE_DESCRIPTION("OMAP DMIC ASoC Interface");
MODULE_LICENSE("GPL");
