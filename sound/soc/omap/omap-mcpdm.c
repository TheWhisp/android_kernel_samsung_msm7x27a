/*
 * omap-mcpdm.c  --  OMAP ALSA SoC DAI driver using McPDM port
 *
<<<<<<< HEAD
 * Copyright (C) 2009 Texas Instruments
 *
 * Author: Misael Lopez Cruz <x0052729@ti.com>
 * Contact: Jorge Eduardo Candelaria <x0107209@ti.com>
 *          Margarita Olaya <magi.olaya@ti.com>
=======
 * Copyright (C) 2009 - 2011 Texas Instruments
 *
 * Author: Misael Lopez Cruz <misael.lopez@ti.com>
 * Contact: Jorge Eduardo Candelaria <x0107209@ti.com>
 *          Margarita Olaya <magi.olaya@ti.com>
 *          Peter Ujfalusi <peter.ujfalusi@ti.com>
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
#define DEBUG

#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/wait.h>
#include <linux/interrupt.h>
#include <linux/err.h>
#include <linux/delay.h>
=======
#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/err.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/slab.h>
#include <linux/pm_runtime.h>

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
<<<<<<< HEAD
#include <sound/initval.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
#include <sound/soc.h>

#include <plat/dma.h>
#include <plat/omap_hwmod.h>
#include "omap-mcpdm.h"
#include "omap-pcm.h"
<<<<<<< HEAD
#if defined(CONFIG_SND_OMAP_SOC_ABE_DSP) ||\
	defined(CONFIG_SND_OMAP_SOC_ABE_DSP_MODULE)
#include "omap-abe-dsp.h"
#include "abe/abe_main.h"
#include "abe/port_mgr.h"
#endif

#define MCPDM_LEGACY_DAI_DL1	0
#define MCPDM_LEGACY_DAI_UL1	1
#define MCPDM_ABE_DAI_DL1		2
#define MCPDM_ABE_DAI_DL2		3
#define MCPDM_ABE_DAI_VIB		4
#define MCPDM_ABE_DAI_UL1		5
=======
>>>>>>> refs/remotes/origin/cm-10.0

struct omap_mcpdm {
	struct device *dev;
	unsigned long phys_base;
	void __iomem *io_base;
	int irq;
<<<<<<< HEAD
	struct delayed_work delayed_work;

	struct mutex mutex;
	struct omap_mcpdm_platform_data *pdata;
	struct completion irq_completion;
	struct abe *abe;
	struct omap_abe_port *dl_port;
	struct omap_abe_port *ul_port;
=======

	struct mutex mutex;
>>>>>>> refs/remotes/origin/cm-10.0

	/* channel data */
	u32 dn_channels;
	u32 up_channels;
<<<<<<< HEAD
	int active;
	int ul_active;
	int abe_mode;

=======

	/* McPDM FIFO thresholds */
	u32 dn_threshold;
	u32 up_threshold;

	/* McPDM dn offsets for rx1, and 2 channels */
	u32 dn_rx_offset;
>>>>>>> refs/remotes/origin/cm-10.0
};

/*
 * Stream DMA parameters
 */
static struct omap_pcm_dma_data omap_mcpdm_dai_dma_params[] = {
	{
		.name = "Audio playback",
		.dma_req = OMAP44XX_DMA_MCPDM_DL,
		.data_type = OMAP_DMA_DATA_TYPE_S32,
		.sync_mode = OMAP_DMA_SYNC_PACKET,
<<<<<<< HEAD
		.packet_size = 16,
		.port_addr = OMAP44XX_MCPDM_L3_BASE + MCPDM_DN_DATA,
=======
		.port_addr = OMAP44XX_MCPDM_L3_BASE + MCPDM_REG_DN_DATA,
>>>>>>> refs/remotes/origin/cm-10.0
	},
	{
		.name = "Audio capture",
		.dma_req = OMAP44XX_DMA_MCPDM_UP,
		.data_type = OMAP_DMA_DATA_TYPE_S32,
		.sync_mode = OMAP_DMA_SYNC_PACKET,
<<<<<<< HEAD
		.packet_size = 16,
		.port_addr = OMAP44XX_MCPDM_L3_BASE + MCPDM_UP_DATA,
	},
};

static inline void omap_mcpdm_write(struct omap_mcpdm *mcpdm,
		u16 reg, u32 val)
=======
		.port_addr = OMAP44XX_MCPDM_L3_BASE + MCPDM_REG_UP_DATA,
	},
};

static inline void omap_mcpdm_write(struct omap_mcpdm *mcpdm, u16 reg, u32 val)
>>>>>>> refs/remotes/origin/cm-10.0
{
	__raw_writel(val, mcpdm->io_base + reg);
}

static inline int omap_mcpdm_read(struct omap_mcpdm *mcpdm, u16 reg)
{
	return __raw_readl(mcpdm->io_base + reg);
}

#ifdef DEBUG
static void omap_mcpdm_reg_dump(struct omap_mcpdm *mcpdm)
{
	dev_dbg(mcpdm->dev, "***********************\n");
	dev_dbg(mcpdm->dev, "IRQSTATUS_RAW:  0x%04x\n",
<<<<<<< HEAD
			omap_mcpdm_read(mcpdm, MCPDM_IRQSTATUS_RAW));
	dev_dbg(mcpdm->dev, "IRQSTATUS:  0x%04x\n",
			omap_mcpdm_read(mcpdm, MCPDM_IRQSTATUS));
	dev_dbg(mcpdm->dev, "IRQENABLE_SET:  0x%04x\n",
			omap_mcpdm_read(mcpdm, MCPDM_IRQENABLE_SET));
	dev_dbg(mcpdm->dev, "IRQENABLE_CLR:  0x%04x\n",
			omap_mcpdm_read(mcpdm, MCPDM_IRQENABLE_CLR));
	dev_dbg(mcpdm->dev, "IRQWAKE_EN: 0x%04x\n",
			omap_mcpdm_read(mcpdm, MCPDM_IRQWAKE_EN));
	dev_dbg(mcpdm->dev, "DMAENABLE_SET: 0x%04x\n",
			omap_mcpdm_read(mcpdm, MCPDM_DMAENABLE_SET));
	dev_dbg(mcpdm->dev, "DMAENABLE_CLR:  0x%04x\n",
			omap_mcpdm_read(mcpdm, MCPDM_DMAENABLE_CLR));
	dev_dbg(mcpdm->dev, "DMAWAKEEN:  0x%04x\n",
			omap_mcpdm_read(mcpdm, MCPDM_DMAWAKEEN));
	dev_dbg(mcpdm->dev, "CTRL:  0x%04x\n",
			omap_mcpdm_read(mcpdm, MCPDM_CTRL));
	dev_dbg(mcpdm->dev, "DN_DATA:  0x%04x\n",
			omap_mcpdm_read(mcpdm, MCPDM_DN_DATA));
	dev_dbg(mcpdm->dev, "UP_DATA: 0x%04x\n",
			omap_mcpdm_read(mcpdm, MCPDM_UP_DATA));
	dev_dbg(mcpdm->dev, "FIFO_CTRL_DN: 0x%04x\n",
			omap_mcpdm_read(mcpdm, MCPDM_FIFO_CTRL_DN));
	dev_dbg(mcpdm->dev, "FIFO_CTRL_UP:  0x%04x\n",
			omap_mcpdm_read(mcpdm, MCPDM_FIFO_CTRL_UP));
=======
			omap_mcpdm_read(mcpdm, MCPDM_REG_IRQSTATUS_RAW));
	dev_dbg(mcpdm->dev, "IRQSTATUS:  0x%04x\n",
			omap_mcpdm_read(mcpdm, MCPDM_REG_IRQSTATUS));
	dev_dbg(mcpdm->dev, "IRQENABLE_SET:  0x%04x\n",
			omap_mcpdm_read(mcpdm, MCPDM_REG_IRQENABLE_SET));
	dev_dbg(mcpdm->dev, "IRQENABLE_CLR:  0x%04x\n",
			omap_mcpdm_read(mcpdm, MCPDM_REG_IRQENABLE_CLR));
	dev_dbg(mcpdm->dev, "IRQWAKE_EN: 0x%04x\n",
			omap_mcpdm_read(mcpdm, MCPDM_REG_IRQWAKE_EN));
	dev_dbg(mcpdm->dev, "DMAENABLE_SET: 0x%04x\n",
			omap_mcpdm_read(mcpdm, MCPDM_REG_DMAENABLE_SET));
	dev_dbg(mcpdm->dev, "DMAENABLE_CLR:  0x%04x\n",
			omap_mcpdm_read(mcpdm, MCPDM_REG_DMAENABLE_CLR));
	dev_dbg(mcpdm->dev, "DMAWAKEEN:  0x%04x\n",
			omap_mcpdm_read(mcpdm, MCPDM_REG_DMAWAKEEN));
	dev_dbg(mcpdm->dev, "CTRL:  0x%04x\n",
			omap_mcpdm_read(mcpdm, MCPDM_REG_CTRL));
	dev_dbg(mcpdm->dev, "DN_DATA:  0x%04x\n",
			omap_mcpdm_read(mcpdm, MCPDM_REG_DN_DATA));
	dev_dbg(mcpdm->dev, "UP_DATA: 0x%04x\n",
			omap_mcpdm_read(mcpdm, MCPDM_REG_UP_DATA));
	dev_dbg(mcpdm->dev, "FIFO_CTRL_DN: 0x%04x\n",
			omap_mcpdm_read(mcpdm, MCPDM_REG_FIFO_CTRL_DN));
	dev_dbg(mcpdm->dev, "FIFO_CTRL_UP:  0x%04x\n",
			omap_mcpdm_read(mcpdm, MCPDM_REG_FIFO_CTRL_UP));
>>>>>>> refs/remotes/origin/cm-10.0
	dev_dbg(mcpdm->dev, "***********************\n");
}
#else
static void omap_mcpdm_reg_dump(struct omap_mcpdm *mcpdm) {}
#endif

/*
 * Enables the transfer through the PDM interface to/from the Phoenix
 * codec by enabling the corresponding UP or DN channels.
 */
static void omap_mcpdm_start(struct omap_mcpdm *mcpdm)
{
<<<<<<< HEAD
	u32 ctrl = omap_mcpdm_read(mcpdm, MCPDM_CTRL);

	ctrl |= SW_UP_RST;
	ctrl |= SW_DN_RST;
	omap_mcpdm_write(mcpdm, MCPDM_CTRL, ctrl);
	ctrl |= mcpdm->up_channels;
	ctrl |= mcpdm->dn_channels;
	omap_mcpdm_write(mcpdm, MCPDM_CTRL, ctrl);
	ctrl &= ~SW_DN_RST;
	omap_mcpdm_write(mcpdm, MCPDM_CTRL, ctrl);
}

static void omap_mcpdm_start_ul(struct omap_mcpdm *mcpdm)
{
	u32 ctrl;

	if (!mcpdm->ul_active++) {
		ctrl = omap_mcpdm_read(mcpdm, MCPDM_CTRL);
		ctrl &= ~SW_UP_RST;
		omap_mcpdm_write(mcpdm, MCPDM_CTRL, ctrl);
	}
}

static void omap_mcpdm_stop_ul(struct omap_mcpdm *mcpdm)
{
	u32 ctrl;

	if (!--mcpdm->ul_active) {
		ctrl = omap_mcpdm_read(mcpdm, MCPDM_CTRL);
		ctrl |= SW_UP_RST;
		omap_mcpdm_write(mcpdm, MCPDM_CTRL, ctrl);
	}
=======
	u32 ctrl = omap_mcpdm_read(mcpdm, MCPDM_REG_CTRL);

	ctrl |= (MCPDM_SW_DN_RST | MCPDM_SW_UP_RST);
	omap_mcpdm_write(mcpdm, MCPDM_REG_CTRL, ctrl);

	ctrl |= mcpdm->dn_channels | mcpdm->up_channels;
	omap_mcpdm_write(mcpdm, MCPDM_REG_CTRL, ctrl);

	ctrl &= ~(MCPDM_SW_DN_RST | MCPDM_SW_UP_RST);
	omap_mcpdm_write(mcpdm, MCPDM_REG_CTRL, ctrl);
>>>>>>> refs/remotes/origin/cm-10.0
}

/*
 * Disables the transfer through the PDM interface to/from the Phoenix
 * codec by disabling the corresponding UP or DN channels.
 */
static void omap_mcpdm_stop(struct omap_mcpdm *mcpdm)
{
<<<<<<< HEAD
	u32 ctrl = omap_mcpdm_read(mcpdm, MCPDM_CTRL);

	ctrl |= SW_UP_RST;
	ctrl |= SW_DN_RST;
	omap_mcpdm_write(mcpdm, MCPDM_CTRL, ctrl);
	ctrl &= ~mcpdm->up_channels;
	ctrl &= ~mcpdm->dn_channels;
	omap_mcpdm_write(mcpdm, MCPDM_CTRL, ctrl);
	ctrl &= ~SW_UP_RST;
	ctrl &= ~SW_DN_RST;
	omap_mcpdm_write(mcpdm, MCPDM_CTRL, ctrl);
=======
	u32 ctrl = omap_mcpdm_read(mcpdm, MCPDM_REG_CTRL);

	ctrl |= (MCPDM_SW_DN_RST | MCPDM_SW_UP_RST);
	omap_mcpdm_write(mcpdm, MCPDM_REG_CTRL, ctrl);

	ctrl &= ~(mcpdm->dn_channels | mcpdm->up_channels);
	omap_mcpdm_write(mcpdm, MCPDM_REG_CTRL, ctrl);

	ctrl &= ~(MCPDM_SW_DN_RST | MCPDM_SW_UP_RST);
	omap_mcpdm_write(mcpdm, MCPDM_REG_CTRL, ctrl);

>>>>>>> refs/remotes/origin/cm-10.0
}

/*
 * Is the physical McPDM interface active.
 */
static inline int omap_mcpdm_active(struct omap_mcpdm *mcpdm)
{
<<<<<<< HEAD
	return omap_mcpdm_read(mcpdm, MCPDM_CTRL) & (PDM_DN_MASK | PDM_UP_MASK);
}

/*
 * Configures McPDM uplink for audio recording.
 * This function should be called before omap_mcpdm_start.
 */
static void omap_mcpdm_capture_open(struct omap_mcpdm *mcpdm)
{
	/* Enable irq request generation */
	omap_mcpdm_write(mcpdm, MCPDM_IRQENABLE_SET,
			MCPDM_UP_IRQ_EMPTY | MCPDM_UP_IRQ_FULL);

	/* Configure uplink threshold */
	omap_mcpdm_write(mcpdm, MCPDM_FIFO_CTRL_UP, 2);

	/* Configure DMA controller */
	omap_mcpdm_write(mcpdm, MCPDM_DMAENABLE_SET, DMA_UP_ENABLE);

}

/*
 * Configures McPDM downlink for audio playback.
 * This function should be called before omap_mcpdm_start.
 */
static void omap_mcpdm_playback_open(struct omap_mcpdm *mcpdm)
{
	/* Enable irq request generation */
	omap_mcpdm_write(mcpdm, MCPDM_IRQENABLE_SET,
			MCPDM_DN_IRQ_EMPTY | MCPDM_DN_IRQ_FULL);

	/* Configure uplink threshold */
	omap_mcpdm_write(mcpdm, MCPDM_FIFO_CTRL_DN, 2);

	/* Enable DMA request generation */
	omap_mcpdm_write(mcpdm, MCPDM_DMAENABLE_SET, DMA_DN_ENABLE);
}

/*
 * Cleans McPDM uplink configuration.
 * This function should be called when the stream is closed.
 */
static void omap_mcpdm_capture_close(struct omap_mcpdm *mcpdm)
{
	/* Disable irq request generation */
	omap_mcpdm_write(mcpdm, MCPDM_IRQENABLE_CLR,
			MCPDM_UP_IRQ_EMPTY | MCPDM_UP_IRQ_FULL);

	/* Disable DMA request generation */
	omap_mcpdm_write(mcpdm, MCPDM_DMAENABLE_CLR, DMA_UP_ENABLE);
}

/*
 * Cleans McPDM downlink configuration.
 * This function should be called when the stream is closed.
 */
static void omap_mcpdm_playback_close(struct omap_mcpdm *mcpdm)
{
	/* Disable irq request generation */
	omap_mcpdm_write(mcpdm, MCPDM_IRQENABLE_CLR,
			MCPDM_DN_IRQ_EMPTY | MCPDM_DN_IRQ_FULL);

	/* Disable DMA request generation */
	omap_mcpdm_write(mcpdm, MCPDM_DMAENABLE_CLR, DMA_DN_ENABLE);
=======
	return omap_mcpdm_read(mcpdm, MCPDM_REG_CTRL) &
					(MCPDM_PDM_DN_MASK | MCPDM_PDM_UP_MASK);
}

/*
 * Configures McPDM uplink, and downlink for audio.
 * This function should be called before omap_mcpdm_start.
 */
static void omap_mcpdm_open_streams(struct omap_mcpdm *mcpdm)
{
	omap_mcpdm_write(mcpdm, MCPDM_REG_IRQENABLE_SET,
			MCPDM_DN_IRQ_EMPTY | MCPDM_DN_IRQ_FULL |
			MCPDM_UP_IRQ_EMPTY | MCPDM_UP_IRQ_FULL);

	/* Enable DN RX1/2 offset cancellation feature, if configured */
	if (mcpdm->dn_rx_offset) {
		u32 dn_offset = mcpdm->dn_rx_offset;

		omap_mcpdm_write(mcpdm, MCPDM_REG_DN_OFFSET, dn_offset);
		dn_offset |= (MCPDM_DN_OFST_RX1_EN | MCPDM_DN_OFST_RX2_EN);
		omap_mcpdm_write(mcpdm, MCPDM_REG_DN_OFFSET, dn_offset);
	}

	omap_mcpdm_write(mcpdm, MCPDM_REG_FIFO_CTRL_DN, mcpdm->dn_threshold);
	omap_mcpdm_write(mcpdm, MCPDM_REG_FIFO_CTRL_UP, mcpdm->up_threshold);

	omap_mcpdm_write(mcpdm, MCPDM_REG_DMAENABLE_SET,
			MCPDM_DMA_DN_ENABLE | MCPDM_DMA_UP_ENABLE);
}

/*
 * Cleans McPDM uplink, and downlink configuration.
 * This function should be called when the stream is closed.
 */
static void omap_mcpdm_close_streams(struct omap_mcpdm *mcpdm)
{
	/* Disable irq request generation for downlink */
	omap_mcpdm_write(mcpdm, MCPDM_REG_IRQENABLE_CLR,
			MCPDM_DN_IRQ_EMPTY | MCPDM_DN_IRQ_FULL);

	/* Disable DMA request generation for downlink */
	omap_mcpdm_write(mcpdm, MCPDM_REG_DMAENABLE_CLR, MCPDM_DMA_DN_ENABLE);

	/* Disable irq request generation for uplink */
	omap_mcpdm_write(mcpdm, MCPDM_REG_IRQENABLE_CLR,
			MCPDM_UP_IRQ_EMPTY | MCPDM_UP_IRQ_FULL);

	/* Disable DMA request generation for uplink */
	omap_mcpdm_write(mcpdm, MCPDM_REG_DMAENABLE_CLR, MCPDM_DMA_UP_ENABLE);

	/* Disable RX1/2 offset cancellation */
	if (mcpdm->dn_rx_offset)
		omap_mcpdm_write(mcpdm, MCPDM_REG_DN_OFFSET, 0);
>>>>>>> refs/remotes/origin/cm-10.0
}

static irqreturn_t omap_mcpdm_irq_handler(int irq, void *dev_id)
{
	struct omap_mcpdm *mcpdm = dev_id;
	int irq_status;

<<<<<<< HEAD
	irq_status = omap_mcpdm_read(mcpdm, MCPDM_IRQSTATUS);

	/* Acknowledge irq event */
	omap_mcpdm_write(mcpdm, MCPDM_IRQSTATUS, irq_status);

	if (irq_status & MCPDM_DN_IRQ_FULL)
		dev_err(mcpdm->dev, "DN FIFO error %x\n", irq_status);

	if (irq_status & MCPDM_DN_IRQ_EMPTY)
		dev_err(mcpdm->dev, "DN FIFO error %x\n", irq_status);

	if (irq_status & MCPDM_DN_IRQ)
		dev_dbg(mcpdm->dev, "DN write request\n");

	if (irq_status & MCPDM_UP_IRQ_FULL)
		dev_err(mcpdm->dev, "UP FIFO error %x\n", irq_status);

	if (irq_status & MCPDM_UP_IRQ_EMPTY)
		dev_err(mcpdm->dev, "UP FIFO error %x\n", irq_status);

	if (irq_status & MCPDM_UP_IRQ)
		dev_dbg(mcpdm->dev, "UP write request\n");
=======
	irq_status = omap_mcpdm_read(mcpdm, MCPDM_REG_IRQSTATUS);

	/* Acknowledge irq event */
	omap_mcpdm_write(mcpdm, MCPDM_REG_IRQSTATUS, irq_status);

	if (irq_status & MCPDM_DN_IRQ_FULL)
		dev_dbg(mcpdm->dev, "DN (playback) FIFO Full\n");

	if (irq_status & MCPDM_DN_IRQ_EMPTY)
		dev_dbg(mcpdm->dev, "DN (playback) FIFO Empty\n");

	if (irq_status & MCPDM_DN_IRQ)
		dev_dbg(mcpdm->dev, "DN (playback) write request\n");

	if (irq_status & MCPDM_UP_IRQ_FULL)
		dev_dbg(mcpdm->dev, "UP (capture) FIFO Full\n");

	if (irq_status & MCPDM_UP_IRQ_EMPTY)
		dev_dbg(mcpdm->dev, "UP (capture) FIFO Empty\n");

	if (irq_status & MCPDM_UP_IRQ)
		dev_dbg(mcpdm->dev, "UP (capture) write request\n");
>>>>>>> refs/remotes/origin/cm-10.0

	return IRQ_HANDLED;
}

static int omap_mcpdm_dai_startup(struct snd_pcm_substream *substream,
				  struct snd_soc_dai *dai)
{
	struct omap_mcpdm *mcpdm = snd_soc_dai_get_drvdata(dai);
<<<<<<< HEAD
	u32 ctrl;
	int err = 0;

	dev_dbg(dai->dev, "%s: active %d\n", __func__, dai->active);

	/* make sure we stop any pre-existing shutdown */
	cancel_delayed_work_sync(&mcpdm->delayed_work);

	mutex_lock(&mcpdm->mutex);

	if (!mcpdm->active++) {
		pm_runtime_get_sync(mcpdm->dev);

		/* Disable lines while request is ongoing */
		omap_mcpdm_write(mcpdm, MCPDM_CTRL, 0x0000);

		/* Set pdm out format */
		ctrl = PDMOUTFORMAT_LJUST & PDMOUTFORMAT;
		ctrl |= SW_UP_RST;
		ctrl |= SW_DN_RST;
		omap_mcpdm_write(mcpdm, MCPDM_CTRL, ctrl);

		/* Enable McPDM watch dog for ES above ES 1.0 to avoid saturation */
		if (omap_rev() != OMAP4430_REV_ES1_0) {
			ctrl = omap_mcpdm_read(mcpdm, MCPDM_CTRL);
			omap_mcpdm_write(mcpdm, MCPDM_CTRL, ctrl | WD_EN);
		}
	}

	if (dai->id > 1)
		mcpdm->abe_mode = 1;
	else
		mcpdm->abe_mode = 0;

	mutex_unlock(&mcpdm->mutex);

	return err;
}

/* work to delay McPDM shutdown */
static void playback_work(struct work_struct *work)
{
	struct omap_mcpdm *mcpdm =
			container_of(work, struct omap_mcpdm, delayed_work.work);

	if (!mcpdm->active && omap_mcpdm_active(mcpdm)) {
		/* ABE playback stop handled by delayed work */
		if (mcpdm->abe_mode) {
			omap_abe_port_disable(mcpdm->abe, mcpdm->dl_port);
			omap_abe_port_disable(mcpdm->abe, mcpdm->ul_port);
			udelay(250);
			omap_mcpdm_stop(mcpdm);
			omap_mcpdm_playback_close(mcpdm);
			omap_mcpdm_capture_close(mcpdm);
			abe_dsp_shutdown();
			abe_dsp_pm_put();
		} else {
			omap_mcpdm_stop(mcpdm);
			omap_mcpdm_playback_close(mcpdm);
			omap_mcpdm_capture_close(mcpdm);
		}
	}

	if (!omap_mcpdm_active(mcpdm))
		pm_runtime_put_sync(mcpdm->dev);
}

static void omap_mcpdm_dai_shutdown(struct snd_pcm_substream *substream,
				    struct snd_soc_dai *dai)
{
	struct omap_mcpdm *mcpdm = snd_soc_dai_get_drvdata(dai);

	dev_dbg(dai->dev, "%s: active %d\n", __func__, dai->active);

	mutex_lock(&mcpdm->mutex);

	if (!--mcpdm->active) {
		cancel_delayed_work_sync(&mcpdm->delayed_work);
		schedule_delayed_work(&mcpdm->delayed_work,
			msecs_to_jiffies(1000)); /* TODO: pdata ? */
=======

	mutex_lock(&mcpdm->mutex);

	if (!dai->active) {
		/* Enable watch dog for ES above ES 1.0 to avoid saturation */
		if (omap_rev() != OMAP4430_REV_ES1_0) {
			u32 ctrl = omap_mcpdm_read(mcpdm, MCPDM_REG_CTRL);

			omap_mcpdm_write(mcpdm, MCPDM_REG_CTRL,
					 ctrl | MCPDM_WD_EN);
		}
		omap_mcpdm_open_streams(mcpdm);
	}

	mutex_unlock(&mcpdm->mutex);

	return 0;
}

static void omap_mcpdm_dai_shutdown(struct snd_pcm_substream *substream,
				  struct snd_soc_dai *dai)
{
	struct omap_mcpdm *mcpdm = snd_soc_dai_get_drvdata(dai);

	mutex_lock(&mcpdm->mutex);

	if (!dai->active) {
		if (omap_mcpdm_active(mcpdm)) {
			omap_mcpdm_stop(mcpdm);
			omap_mcpdm_close_streams(mcpdm);
		}
>>>>>>> refs/remotes/origin/cm-10.0
	}

	mutex_unlock(&mcpdm->mutex);
}

static int omap_mcpdm_dai_hw_params(struct snd_pcm_substream *substream,
				    struct snd_pcm_hw_params *params,
				    struct snd_soc_dai *dai)
{
	struct omap_mcpdm *mcpdm = snd_soc_dai_get_drvdata(dai);
	int stream = substream->stream;
<<<<<<< HEAD
	int channels, link_mask = 0;

	snd_soc_dai_set_dma_data(dai, substream,
				 &omap_mcpdm_dai_dma_params[stream]);

	/* ABE DAIs have fixed channels and IDs > MCPDM_LEGACY_DAI_DL1 */
	if (dai->id > MCPDM_LEGACY_DAI_DL1) {
		mcpdm->dn_channels = PDM_DN_MASK | PDM_CMD_MASK;
		mcpdm->up_channels = PDM_UP1_EN | PDM_UP2_EN;
		return 0;
	}

	channels = params_channels(params);
	switch (channels) {
	case 4:
		if (stream == SNDRV_PCM_STREAM_CAPTURE)
			/* up to 2 channels for capture */
			return -EINVAL;
		link_mask |= 1 << 3;
	case 3:
		if (stream == SNDRV_PCM_STREAM_CAPTURE)
			/* up to 2 channels for capture */
			return -EINVAL;
=======
	struct omap_pcm_dma_data *dma_data;
	int channels;
	int link_mask = 0;

	channels = params_channels(params);
	switch (channels) {
	case 5:
		if (stream == SNDRV_PCM_STREAM_CAPTURE)
			/* up to 3 channels for capture */
			return -EINVAL;
		link_mask |= 1 << 4;
	case 4:
		if (stream == SNDRV_PCM_STREAM_CAPTURE)
			/* up to 3 channels for capture */
			return -EINVAL;
		link_mask |= 1 << 3;
	case 3:
>>>>>>> refs/remotes/origin/cm-10.0
		link_mask |= 1 << 2;
	case 2:
		link_mask |= 1 << 1;
	case 1:
		link_mask |= 1 << 0;
		break;
	default:
		/* unsupported number of channels */
		return -EINVAL;
	}

<<<<<<< HEAD
	if (stream == SNDRV_PCM_STREAM_PLAYBACK)
		/* Downlink channels */
		mcpdm->dn_channels = (link_mask << 3) & (PDM_DN_MASK | PDM_CMD_MASK);
	else
		/* Uplink channels */
		mcpdm->up_channels = link_mask & (PDM_UP_MASK | PDM_STATUS_MASK);
=======
	dma_data = &omap_mcpdm_dai_dma_params[stream];

	/* Configure McPDM channels, and DMA packet size */
	if (stream == SNDRV_PCM_STREAM_PLAYBACK) {
		mcpdm->dn_channels = link_mask << 3;
		dma_data->packet_size =
			(MCPDM_DN_THRES_MAX - mcpdm->dn_threshold) * channels;
	} else {
		mcpdm->up_channels = link_mask << 0;
		dma_data->packet_size = mcpdm->up_threshold * channels;
	}

	snd_soc_dai_set_dma_data(dai, substream, dma_data);
>>>>>>> refs/remotes/origin/cm-10.0

	return 0;
}

static int omap_mcpdm_prepare(struct snd_pcm_substream *substream,
				  struct snd_soc_dai *dai)
{
	struct omap_mcpdm *mcpdm = snd_soc_dai_get_drvdata(dai);

	if (!omap_mcpdm_active(mcpdm)) {
<<<<<<< HEAD
		if (mcpdm->abe_mode) {
			/* Check if ABE McPDM DL is already started */
			if ((omap_abe_port_is_enabled(mcpdm->abe, mcpdm->dl_port)) ||
			    (omap_abe_port_is_enabled(mcpdm->abe, mcpdm->ul_port)))
				return 0;

			abe_dsp_pm_get();

			/* start ATC before McPDM IP */
			omap_abe_port_enable(mcpdm->abe, mcpdm->dl_port);
			omap_abe_port_enable(mcpdm->abe, mcpdm->ul_port);

			/* wait 250us for ABE tick */
			udelay(250);
			omap_mcpdm_playback_open(mcpdm);
			omap_mcpdm_capture_open(mcpdm);
			omap_mcpdm_start(mcpdm);
		} else {
			omap_mcpdm_playback_open(mcpdm);
			omap_mcpdm_capture_open(mcpdm);
			omap_mcpdm_start(mcpdm);
		}
	}
	return 0;
}

static void omap_mcpdm_dai_trigger(struct snd_pcm_substream *substream,
		int cmd, struct snd_soc_dai *dai)
{
	struct omap_mcpdm *mcpdm = snd_soc_dai_get_drvdata(dai);

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
		if (substream->stream == SNDRV_PCM_STREAM_CAPTURE)
			omap_mcpdm_start_ul(mcpdm);
		break;
	case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
		break;
	case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
		break;
	case SNDRV_PCM_TRIGGER_STOP:
		if (substream->stream == SNDRV_PCM_STREAM_CAPTURE)
			omap_mcpdm_stop_ul(mcpdm);
		break;
	default:
		break;
	}
}

static struct snd_soc_dai_ops omap_mcpdm_dai_ops = {
=======
		omap_mcpdm_start(mcpdm);
		omap_mcpdm_reg_dump(mcpdm);
	}

	return 0;
}

static const struct snd_soc_dai_ops omap_mcpdm_dai_ops = {
>>>>>>> refs/remotes/origin/cm-10.0
	.startup	= omap_mcpdm_dai_startup,
	.shutdown	= omap_mcpdm_dai_shutdown,
	.hw_params	= omap_mcpdm_dai_hw_params,
	.prepare	= omap_mcpdm_prepare,
<<<<<<< HEAD
	.trigger	= omap_mcpdm_dai_trigger,
=======
>>>>>>> refs/remotes/origin/cm-10.0
};

static int omap_mcpdm_probe(struct snd_soc_dai *dai)
{
	struct omap_mcpdm *mcpdm = snd_soc_dai_get_drvdata(dai);
	int ret;

	pm_runtime_enable(mcpdm->dev);

	/* Disable lines while request is ongoing */
	pm_runtime_get_sync(mcpdm->dev);
<<<<<<< HEAD
	omap_mcpdm_write(mcpdm, MCPDM_CTRL, 0x00);

	ret = request_irq(mcpdm->irq, omap_mcpdm_irq_handler,
				0, "McPDM", (void *)mcpdm);
	if (ret)
		dev_err(mcpdm->dev, "Request for McPDM IRQ failed\n");

	pm_runtime_put_sync(mcpdm->dev);
=======
	omap_mcpdm_write(mcpdm, MCPDM_REG_CTRL, 0x00);

	ret = request_irq(mcpdm->irq, omap_mcpdm_irq_handler,
				0, "McPDM", (void *)mcpdm);

	pm_runtime_put_sync(mcpdm->dev);

	if (ret) {
		dev_err(mcpdm->dev, "Request for IRQ failed\n");
		pm_runtime_disable(mcpdm->dev);
	}

	/* Configure McPDM threshold values */
	mcpdm->dn_threshold = 2;
	mcpdm->up_threshold = MCPDM_UP_THRES_MAX - 3;
>>>>>>> refs/remotes/origin/cm-10.0
	return ret;
}

static int omap_mcpdm_remove(struct snd_soc_dai *dai)
{
	struct omap_mcpdm *mcpdm = snd_soc_dai_get_drvdata(dai);

	free_irq(mcpdm->irq, (void *)mcpdm);
	pm_runtime_disable(mcpdm->dev);

	return 0;
}

#define OMAP_MCPDM_RATES	(SNDRV_PCM_RATE_88200 | SNDRV_PCM_RATE_96000)
#define OMAP_MCPDM_FORMATS	SNDRV_PCM_FMTBIT_S32_LE

<<<<<<< HEAD
static struct snd_soc_dai_driver omap_mcpdm_dai[] = {
{
	.name = "mcpdm-dl",
	.id	= MCPDM_LEGACY_DAI_DL1,
=======
static struct snd_soc_dai_driver omap_mcpdm_dai = {
>>>>>>> refs/remotes/origin/cm-10.0
	.probe = omap_mcpdm_probe,
	.remove = omap_mcpdm_remove,
	.probe_order = SND_SOC_COMP_ORDER_LATE,
	.remove_order = SND_SOC_COMP_ORDER_EARLY,
	.playback = {
		.channels_min = 1,
<<<<<<< HEAD
		.channels_max = 4,
		.rates = OMAP_MCPDM_RATES,
		.formats = OMAP_MCPDM_FORMATS,
	},
	.ops = &omap_mcpdm_dai_ops,
},
{
	.name = "mcpdm-ul",
	.id	= MCPDM_LEGACY_DAI_UL1,
	.probe_order = SND_SOC_COMP_ORDER_LATE,
	.remove_order = SND_SOC_COMP_ORDER_EARLY,
	.capture = {
		.channels_min = 1,
		.channels_max = 2,
		.rates = OMAP_MCPDM_RATES,
		.formats = OMAP_MCPDM_FORMATS,
	},
	.ops = &omap_mcpdm_dai_ops,
},
#if defined(CONFIG_SND_OMAP_SOC_ABE_DSP) ||\
	defined(CONFIG_SND_OMAP_SOC_ABE_DSP_MODULE)
{
	.name = "mcpdm-dl1",
	.id	= MCPDM_ABE_DAI_DL1,
	.probe_order = SND_SOC_COMP_ORDER_LATE,
	.remove_order = SND_SOC_COMP_ORDER_EARLY,
	.playback = {
		.channels_min = 1,
		.channels_max = 2,
		.rates = OMAP_MCPDM_RATES,
		.formats = OMAP_MCPDM_FORMATS,
	},
	.ops = &omap_mcpdm_dai_ops,
},
{
	.name = "mcpdm-dl2",
	.id	= MCPDM_ABE_DAI_DL2,
	.probe_order = SND_SOC_COMP_ORDER_LATE,
	.remove_order = SND_SOC_COMP_ORDER_EARLY,
	.playback = {
		.channels_min = 1,
		.channels_max = 2,
		.rates = OMAP_MCPDM_RATES,
		.formats = OMAP_MCPDM_FORMATS,
	},
	.ops = &omap_mcpdm_dai_ops,
},
{
	.name = "mcpdm-vib",
	.id	= MCPDM_ABE_DAI_VIB,
	.probe_order = SND_SOC_COMP_ORDER_LATE,
	.remove_order = SND_SOC_COMP_ORDER_EARLY,
	.playback = {
		.channels_min = 1,
		.channels_max = 2,
		.rates = OMAP_MCPDM_RATES,
		.formats = OMAP_MCPDM_FORMATS,
	},
	.ops = &omap_mcpdm_dai_ops,
},
{
	.name = "mcpdm-ul1",
	.id	= MCPDM_ABE_DAI_UL1,
	.probe_order = SND_SOC_COMP_ORDER_LATE,
	.remove_order = SND_SOC_COMP_ORDER_EARLY,
	.capture = {
		.channels_min = 1,
		.channels_max = 2,
		.rates = OMAP_MCPDM_RATES,
		.formats = OMAP_MCPDM_FORMATS,
	},
	.ops = &omap_mcpdm_dai_ops,
},
#endif
 };
=======
		.channels_max = 5,
		.rates = OMAP_MCPDM_RATES,
		.formats = OMAP_MCPDM_FORMATS,
		.sig_bits = 24,
	},
	.capture = {
		.channels_min = 1,
		.channels_max = 3,
		.rates = OMAP_MCPDM_RATES,
		.formats = OMAP_MCPDM_FORMATS,
		.sig_bits = 24,
	},
	.ops = &omap_mcpdm_dai_ops,
};

void omap_mcpdm_configure_dn_offsets(struct snd_soc_pcm_runtime *rtd,
				    u8 rx1, u8 rx2)
{
	struct omap_mcpdm *mcpdm = snd_soc_dai_get_drvdata(rtd->cpu_dai);

	mcpdm->dn_rx_offset = MCPDM_DNOFST_RX1(rx1) | MCPDM_DNOFST_RX2(rx2);
}
EXPORT_SYMBOL_GPL(omap_mcpdm_configure_dn_offsets);
>>>>>>> refs/remotes/origin/cm-10.0

static __devinit int asoc_mcpdm_probe(struct platform_device *pdev)
{
	struct omap_mcpdm *mcpdm;
	struct resource *res;
<<<<<<< HEAD
	int ret = 0, err;
=======
	int ret = 0;
>>>>>>> refs/remotes/origin/cm-10.0

	mcpdm = kzalloc(sizeof(struct omap_mcpdm), GFP_KERNEL);
	if (!mcpdm)
		return -ENOMEM;

	platform_set_drvdata(pdev, mcpdm);

	mutex_init(&mcpdm->mutex);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (res == NULL) {
		dev_err(&pdev->dev, "no resource\n");
		goto err_res;
	}

<<<<<<< HEAD
=======
	if (!request_mem_region(res->start, resource_size(res), "McPDM")) {
		ret = -EBUSY;
		goto err_res;
	}

>>>>>>> refs/remotes/origin/cm-10.0
	mcpdm->io_base = ioremap(res->start, resource_size(res));
	if (!mcpdm->io_base) {
		ret = -ENOMEM;
		goto err_iomap;
	}

	mcpdm->irq = platform_get_irq(pdev, 0);
	if (mcpdm->irq < 0) {
		ret = mcpdm->irq;
		goto err_irq;
	}

	mcpdm->dev = &pdev->dev;

<<<<<<< HEAD
	INIT_DELAYED_WORK(&mcpdm->delayed_work, playback_work);

#if defined(CONFIG_SND_OMAP_SOC_ABE_DSP) ||\
	defined(CONFIG_SND_OMAP_SOC_ABE_DSP_MODULE)

	mcpdm->abe = omap_abe_port_mgr_get();

	mcpdm->dl_port = omap_abe_port_open(mcpdm->abe, OMAP_ABE_BE_PORT_PDM_DL1);
	if (mcpdm->dl_port == NULL)
		goto err_irq;
	mcpdm->ul_port = omap_abe_port_open(mcpdm->abe, OMAP_ABE_BE_PORT_PDM_UL1);
	if (mcpdm->ul_port == NULL)
		goto err_irq;
#endif

	ret = snd_soc_register_dais(&pdev->dev, omap_mcpdm_dai,
			ARRAY_SIZE(omap_mcpdm_dai));
	if (ret == 0)
		return 0;

#if defined(CONFIG_SND_OMAP_SOC_ABE_DSP) ||\
	defined(CONFIG_SND_OMAP_SOC_ABE_DSP_MODULE)
	omap_abe_port_close(mcpdm->abe, mcpdm->dl_port);
	omap_abe_port_close(mcpdm->abe, mcpdm->ul_port);
#endif
=======
	ret = snd_soc_register_dai(&pdev->dev, &omap_mcpdm_dai);
	if (!ret)
		return 0;

>>>>>>> refs/remotes/origin/cm-10.0
err_irq:
	iounmap(mcpdm->io_base);
err_iomap:
	release_mem_region(res->start, resource_size(res));
err_res:
	kfree(mcpdm);
	return ret;
}

static int __devexit asoc_mcpdm_remove(struct platform_device *pdev)
{
	struct omap_mcpdm *mcpdm = platform_get_drvdata(pdev);
	struct resource *res;

<<<<<<< HEAD
	flush_delayed_work_sync(&mcpdm->delayed_work);

	snd_soc_unregister_dais(&pdev->dev, ARRAY_SIZE(omap_mcpdm_dai));

#if defined(CONFIG_SND_OMAP_SOC_ABE_DSP) ||\
	defined(CONFIG_SND_OMAP_SOC_ABE_DSP_MODULE)
	omap_abe_port_close(mcpdm->abe, mcpdm->dl_port);
	omap_abe_port_close(mcpdm->abe, mcpdm->ul_port);
	omap_abe_port_mgr_put(mcpdm->abe);
#endif

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	iounmap(mcpdm->io_base);
=======
	snd_soc_unregister_dai(&pdev->dev);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	iounmap(mcpdm->io_base);
	release_mem_region(res->start, resource_size(res));
>>>>>>> refs/remotes/origin/cm-10.0

	kfree(mcpdm);
	return 0;
}

static struct platform_driver asoc_mcpdm_driver = {
	.driver = {
<<<<<<< HEAD
			.name = "omap-mcpdm",
			.owner = THIS_MODULE,
	},

	.probe = asoc_mcpdm_probe,
	.remove = __devexit_p(asoc_mcpdm_remove),
};

static int __init snd_omap_mcpdm_init(void)
{
	return platform_driver_register(&asoc_mcpdm_driver);
}
module_init(snd_omap_mcpdm_init);

static void __exit snd_omap_mcpdm_exit(void)
{
	platform_driver_unregister(&asoc_mcpdm_driver);
}
module_exit(snd_omap_mcpdm_exit);

MODULE_AUTHOR("Misael Lopez Cruz <x0052729@ti.com>");
=======
		.name	= "omap-mcpdm",
		.owner	= THIS_MODULE,
	},

	.probe	= asoc_mcpdm_probe,
	.remove	= __devexit_p(asoc_mcpdm_remove),
};

module_platform_driver(asoc_mcpdm_driver);

MODULE_AUTHOR("Misael Lopez Cruz <misael.lopez@ti.com>");
>>>>>>> refs/remotes/origin/cm-10.0
MODULE_DESCRIPTION("OMAP PDM SoC Interface");
MODULE_LICENSE("GPL");
