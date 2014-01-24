/*
 * MMCIF eMMC driver.
 *
 * Copyright (C) 2010 Renesas Solutions Corp.
 * Yusuke Goda <yusuke.goda.sx@renesas.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 *
 * TODO
 *  1. DMA
 *  2. Power management
 *  3. Handle MMC errors better
 *
 */

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
/*
 * The MMCIF driver is now processing MMC requests asynchronously, according
 * to the Linux MMC API requirement.
 *
 * The MMCIF driver processes MMC requests in up to 3 stages: command, optional
 * data, and optional stop. To achieve asynchronous processing each of these
 * stages is split into two halves: a top and a bottom half. The top half
 * initialises the hardware, installs a timeout handler to handle completion
 * timeouts, and returns. In case of the command stage this immediately returns
 * control to the caller, leaving all further processing to run asynchronously.
 * All further request processing is performed by the bottom halves.
 *
 * The bottom half further consists of a "hard" IRQ handler, an IRQ handler
 * thread, a DMA completion callback, if DMA is used, a timeout work, and
 * request- and stage-specific handler methods.
 *
 * Each bottom half run begins with either a hardware interrupt, a DMA callback
 * invocation, or a timeout work run. In case of an error or a successful
 * processing completion, the MMC core is informed and the request processing is
 * finished. In case processing has to continue, i.e., if data has to be read
 * from or written to the card, or if a stop command has to be sent, the next
 * top half is called, which performs the necessary hardware handling and
 * reschedules the timeout work. This returns the driver state machine into the
 * bottom half waiting state.
 */

#include <linux/bitops.h>
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#include <linux/clk.h>
#include <linux/completion.h>
#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <linux/dmaengine.h>
#include <linux/mmc/card.h>
#include <linux/mmc/core.h>
#include <linux/mmc/host.h>
#include <linux/mmc/mmc.h>
#include <linux/mmc/sdio.h>
#include <linux/mmc/sh_mmcif.h>
<<<<<<< HEAD
#include <linux/pagemap.h>
#include <linux/platform_device.h>
<<<<<<< HEAD
#include <linux/pm_runtime.h>
#include <linux/spinlock.h>
=======
#include <linux/pm_qos.h>
#include <linux/pm_runtime.h>
#include <linux/spinlock.h>
#include <linux/module.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/mmc/slot-gpio.h>
#include <linux/mod_devicetable.h>
#include <linux/mutex.h>
#include <linux/pagemap.h>
#include <linux/platform_device.h>
#include <linux/pm_qos.h>
#include <linux/pm_runtime.h>
#include <linux/sh_dma.h>
#include <linux/spinlock.h>
#include <linux/module.h>
>>>>>>> refs/remotes/origin/master

#define DRIVER_NAME	"sh_mmcif"
#define DRIVER_VERSION	"2010-04-28"

/* CE_CMD_SET */
#define CMD_MASK		0x3f000000
#define CMD_SET_RTYP_NO		((0 << 23) | (0 << 22))
#define CMD_SET_RTYP_6B		((0 << 23) | (1 << 22)) /* R1/R1b/R3/R4/R5 */
#define CMD_SET_RTYP_17B	((1 << 23) | (0 << 22)) /* R2 */
#define CMD_SET_RBSY		(1 << 21) /* R1b */
#define CMD_SET_CCSEN		(1 << 20)
#define CMD_SET_WDAT		(1 << 19) /* 1: on data, 0: no data */
#define CMD_SET_DWEN		(1 << 18) /* 1: write, 0: read */
#define CMD_SET_CMLTE		(1 << 17) /* 1: multi block trans, 0: single */
#define CMD_SET_CMD12EN		(1 << 16) /* 1: CMD12 auto issue */
#define CMD_SET_RIDXC_INDEX	((0 << 15) | (0 << 14)) /* index check */
#define CMD_SET_RIDXC_BITS	((0 << 15) | (1 << 14)) /* check bits check */
#define CMD_SET_RIDXC_NO	((1 << 15) | (0 << 14)) /* no check */
#define CMD_SET_CRC7C		((0 << 13) | (0 << 12)) /* CRC7 check*/
#define CMD_SET_CRC7C_BITS	((0 << 13) | (1 << 12)) /* check bits check*/
#define CMD_SET_CRC7C_INTERNAL	((1 << 13) | (0 << 12)) /* internal CRC7 check*/
#define CMD_SET_CRC16C		(1 << 10) /* 0: CRC16 check*/
#define CMD_SET_CRCSTE		(1 << 8) /* 1: not receive CRC status */
#define CMD_SET_TBIT		(1 << 7) /* 1: tran mission bit "Low" */
#define CMD_SET_OPDM		(1 << 6) /* 1: open/drain */
#define CMD_SET_CCSH		(1 << 5)
<<<<<<< HEAD
=======
#define CMD_SET_DARS		(1 << 2) /* Dual Data Rate */
>>>>>>> refs/remotes/origin/master
#define CMD_SET_DATW_1		((0 << 1) | (0 << 0)) /* 1bit */
#define CMD_SET_DATW_4		((0 << 1) | (1 << 0)) /* 4bit */
#define CMD_SET_DATW_8		((1 << 1) | (0 << 0)) /* 8bit */

/* CE_CMD_CTRL */
#define CMD_CTRL_BREAK		(1 << 0)

/* CE_BLOCK_SET */
#define BLOCK_SIZE_MASK		0x0000ffff

/* CE_INT */
#define INT_CCSDE		(1 << 29)
#define INT_CMD12DRE		(1 << 26)
#define INT_CMD12RBE		(1 << 25)
#define INT_CMD12CRE		(1 << 24)
#define INT_DTRANE		(1 << 23)
#define INT_BUFRE		(1 << 22)
#define INT_BUFWEN		(1 << 21)
#define INT_BUFREN		(1 << 20)
#define INT_CCSRCV		(1 << 19)
#define INT_RBSYE		(1 << 17)
#define INT_CRSPE		(1 << 16)
#define INT_CMDVIO		(1 << 15)
#define INT_BUFVIO		(1 << 14)
#define INT_WDATERR		(1 << 11)
#define INT_RDATERR		(1 << 10)
#define INT_RIDXERR		(1 << 9)
#define INT_RSPERR		(1 << 8)
#define INT_CCSTO		(1 << 5)
#define INT_CRCSTO		(1 << 4)
#define INT_WDATTO		(1 << 3)
#define INT_RDATTO		(1 << 2)
#define INT_RBSYTO		(1 << 1)
#define INT_RSPTO		(1 << 0)
#define INT_ERR_STS		(INT_CMDVIO | INT_BUFVIO | INT_WDATERR |  \
				 INT_RDATERR | INT_RIDXERR | INT_RSPERR | \
				 INT_CCSTO | INT_CRCSTO | INT_WDATTO |	  \
				 INT_RDATTO | INT_RBSYTO | INT_RSPTO)

<<<<<<< HEAD
=======
#define INT_ALL			(INT_RBSYE | INT_CRSPE | INT_BUFREN |	 \
				 INT_BUFWEN | INT_CMD12DRE | INT_BUFRE | \
				 INT_DTRANE | INT_CMD12RBE | INT_CMD12CRE)

#define INT_CCS			(INT_CCSTO | INT_CCSRCV | INT_CCSDE)

>>>>>>> refs/remotes/origin/master
/* CE_INT_MASK */
#define MASK_ALL		0x00000000
#define MASK_MCCSDE		(1 << 29)
#define MASK_MCMD12DRE		(1 << 26)
#define MASK_MCMD12RBE		(1 << 25)
#define MASK_MCMD12CRE		(1 << 24)
#define MASK_MDTRANE		(1 << 23)
#define MASK_MBUFRE		(1 << 22)
#define MASK_MBUFWEN		(1 << 21)
#define MASK_MBUFREN		(1 << 20)
#define MASK_MCCSRCV		(1 << 19)
#define MASK_MRBSYE		(1 << 17)
#define MASK_MCRSPE		(1 << 16)
#define MASK_MCMDVIO		(1 << 15)
#define MASK_MBUFVIO		(1 << 14)
#define MASK_MWDATERR		(1 << 11)
#define MASK_MRDATERR		(1 << 10)
#define MASK_MRIDXERR		(1 << 9)
#define MASK_MRSPERR		(1 << 8)
#define MASK_MCCSTO		(1 << 5)
#define MASK_MCRCSTO		(1 << 4)
#define MASK_MWDATTO		(1 << 3)
#define MASK_MRDATTO		(1 << 2)
#define MASK_MRBSYTO		(1 << 1)
#define MASK_MRSPTO		(1 << 0)

<<<<<<< HEAD
<<<<<<< HEAD
=======
#define MASK_START_CMD		(MASK_MCMDVIO | MASK_MBUFVIO | MASK_MWDATERR | \
				 MASK_MRDATERR | MASK_MRIDXERR | MASK_MRSPERR | \
				 MASK_MCCSTO | MASK_MCRCSTO | MASK_MWDATTO | \
				 MASK_MRDATTO | MASK_MRBSYTO | MASK_MRSPTO)

>>>>>>> refs/remotes/origin/cm-10.0
=======
#define MASK_START_CMD		(MASK_MCMDVIO | MASK_MBUFVIO | MASK_MWDATERR | \
				 MASK_MRDATERR | MASK_MRIDXERR | MASK_MRSPERR | \
				 MASK_MCRCSTO | MASK_MWDATTO | \
				 MASK_MRDATTO | MASK_MRBSYTO | MASK_MRSPTO)

#define MASK_CLEAN		(INT_ERR_STS | MASK_MRBSYE | MASK_MCRSPE |	\
				 MASK_MBUFREN | MASK_MBUFWEN |			\
				 MASK_MCMD12DRE | MASK_MBUFRE | MASK_MDTRANE |	\
				 MASK_MCMD12RBE | MASK_MCMD12CRE)

>>>>>>> refs/remotes/origin/master
/* CE_HOST_STS1 */
#define STS1_CMDSEQ		(1 << 31)

/* CE_HOST_STS2 */
#define STS2_CRCSTE		(1 << 31)
#define STS2_CRC16E		(1 << 30)
#define STS2_AC12CRCE		(1 << 29)
#define STS2_RSPCRC7E		(1 << 28)
#define STS2_CRCSTEBE		(1 << 27)
#define STS2_RDATEBE		(1 << 26)
#define STS2_AC12REBE		(1 << 25)
#define STS2_RSPEBE		(1 << 24)
#define STS2_AC12IDXE		(1 << 23)
#define STS2_RSPIDXE		(1 << 22)
#define STS2_CCSTO		(1 << 15)
#define STS2_RDATTO		(1 << 14)
#define STS2_DATBSYTO		(1 << 13)
#define STS2_CRCSTTO		(1 << 12)
#define STS2_AC12BSYTO		(1 << 11)
#define STS2_RSPBSYTO		(1 << 10)
#define STS2_AC12RSPTO		(1 << 9)
#define STS2_RSPTO		(1 << 8)
#define STS2_CRC_ERR		(STS2_CRCSTE | STS2_CRC16E |		\
				 STS2_AC12CRCE | STS2_RSPCRC7E | STS2_CRCSTEBE)
#define STS2_TIMEOUT_ERR	(STS2_CCSTO | STS2_RDATTO |		\
				 STS2_DATBSYTO | STS2_CRCSTTO |		\
				 STS2_AC12BSYTO | STS2_RSPBSYTO |	\
				 STS2_AC12RSPTO | STS2_RSPTO)

#define CLKDEV_EMMC_DATA	52000000 /* 52MHz */
#define CLKDEV_MMC_DATA		20000000 /* 20MHz */
#define CLKDEV_INIT		400000   /* 400 KHz */

enum mmcif_state {
	STATE_IDLE,
	STATE_REQUEST,
	STATE_IOS,
<<<<<<< HEAD
};

<<<<<<< HEAD
struct sh_mmcif_host {
	struct mmc_host *mmc;
	struct mmc_data *data;
	struct platform_device *pd;
=======
=======
	STATE_TIMEOUT,
};

>>>>>>> refs/remotes/origin/master
enum mmcif_wait_for {
	MMCIF_WAIT_FOR_REQUEST,
	MMCIF_WAIT_FOR_CMD,
	MMCIF_WAIT_FOR_MREAD,
	MMCIF_WAIT_FOR_MWRITE,
	MMCIF_WAIT_FOR_READ,
	MMCIF_WAIT_FOR_WRITE,
	MMCIF_WAIT_FOR_READ_END,
	MMCIF_WAIT_FOR_WRITE_END,
	MMCIF_WAIT_FOR_STOP,
};

struct sh_mmcif_host {
	struct mmc_host *mmc;
	struct mmc_request *mrq;
	struct platform_device *pd;
<<<<<<< HEAD
	struct sh_dmae_slave dma_slave_tx;
	struct sh_dmae_slave dma_slave_rx;
>>>>>>> refs/remotes/origin/cm-10.0
	struct clk *hclk;
	unsigned int clk;
	int bus_width;
	bool sd_error;
<<<<<<< HEAD
	long timeout;
	void __iomem *addr;
	struct completion intr_wait;
	enum mmcif_state state;
	spinlock_t lock;
	bool power;
=======
=======
	struct clk *hclk;
	unsigned int clk;
	int bus_width;
	unsigned char timing;
	bool sd_error;
>>>>>>> refs/remotes/origin/master
	bool dying;
	long timeout;
	void __iomem *addr;
	u32 *pio_ptr;
	spinlock_t lock;		/* protect sh_mmcif_host::state */
	enum mmcif_state state;
	enum mmcif_wait_for wait_for;
	struct delayed_work timeout_work;
	size_t blocksize;
	int sg_idx;
	int sg_blkidx;
	bool power;
	bool card_present;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
	bool ccs_enable;		/* Command Completion Signal support */
	bool clk_ctrl2_enable;
	struct mutex thread_lock;
>>>>>>> refs/remotes/origin/master

	/* DMA support */
	struct dma_chan		*chan_rx;
	struct dma_chan		*chan_tx;
	struct completion	dma_complete;
	bool			dma_active;
};

static inline void sh_mmcif_bitset(struct sh_mmcif_host *host,
					unsigned int reg, u32 val)
{
	writel(val | readl(host->addr + reg), host->addr + reg);
}

static inline void sh_mmcif_bitclr(struct sh_mmcif_host *host,
					unsigned int reg, u32 val)
{
	writel(~val & readl(host->addr + reg), host->addr + reg);
}

static void mmcif_dma_complete(void *arg)
{
	struct sh_mmcif_host *host = arg;
<<<<<<< HEAD
<<<<<<< HEAD
	dev_dbg(&host->pd->dev, "Command completed\n");

	if (WARN(!host->data, "%s: NULL data in DMA completion!\n",
		 dev_name(&host->pd->dev)))
		return;

	if (host->data->flags & MMC_DATA_READ)
		dma_unmap_sg(host->chan_rx->device->dev,
			     host->data->sg, host->data->sg_len,
			     DMA_FROM_DEVICE);
	else
		dma_unmap_sg(host->chan_tx->device->dev,
			     host->data->sg, host->data->sg_len,
=======
	struct mmc_data *data = host->mrq->data;

	dev_dbg(&host->pd->dev, "Command completed\n");

	if (WARN(!data, "%s: NULL data in DMA completion!\n",
		 dev_name(&host->pd->dev)))
		return;

	if (data->flags & MMC_DATA_READ)
		dma_unmap_sg(host->chan_rx->device->dev,
			     data->sg, data->sg_len,
			     DMA_FROM_DEVICE);
	else
		dma_unmap_sg(host->chan_tx->device->dev,
			     data->sg, data->sg_len,
>>>>>>> refs/remotes/origin/cm-10.0
			     DMA_TO_DEVICE);

=======
	struct mmc_request *mrq = host->mrq;

	dev_dbg(&host->pd->dev, "Command completed\n");

	if (WARN(!mrq || !mrq->data, "%s: NULL data in DMA completion!\n",
		 dev_name(&host->pd->dev)))
		return;

>>>>>>> refs/remotes/origin/master
	complete(&host->dma_complete);
}

static void sh_mmcif_start_dma_rx(struct sh_mmcif_host *host)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct scatterlist *sg = host->data->sg;
=======
	struct mmc_data *data = host->mrq->data;
	struct scatterlist *sg = data->sg;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct mmc_data *data = host->mrq->data;
	struct scatterlist *sg = data->sg;
>>>>>>> refs/remotes/origin/master
	struct dma_async_tx_descriptor *desc = NULL;
	struct dma_chan *chan = host->chan_rx;
	dma_cookie_t cookie = -EINVAL;
	int ret;

<<<<<<< HEAD
<<<<<<< HEAD
	ret = dma_map_sg(chan->device->dev, sg, host->data->sg_len,
			 DMA_FROM_DEVICE);
	if (ret > 0) {
		host->dma_active = true;
		desc = chan->device->device_prep_slave_sg(chan, sg, ret,
			DMA_FROM_DEVICE, DMA_PREP_INTERRUPT | DMA_CTRL_ACK);
=======
=======
>>>>>>> refs/remotes/origin/master
	ret = dma_map_sg(chan->device->dev, sg, data->sg_len,
			 DMA_FROM_DEVICE);
	if (ret > 0) {
		host->dma_active = true;
		desc = dmaengine_prep_slave_sg(chan, sg, ret,
			DMA_DEV_TO_MEM, DMA_PREP_INTERRUPT | DMA_CTRL_ACK);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	if (desc) {
		desc->callback = mmcif_dma_complete;
		desc->callback_param = host;
		cookie = dmaengine_submit(desc);
		sh_mmcif_bitset(host, MMCIF_CE_BUF_ACC, BUF_ACC_DMAREN);
		dma_async_issue_pending(chan);
	}
	dev_dbg(&host->pd->dev, "%s(): mapped %d -> %d, cookie %d\n",
<<<<<<< HEAD
<<<<<<< HEAD
		__func__, host->data->sg_len, ret, cookie);
=======
		__func__, data->sg_len, ret, cookie);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		__func__, data->sg_len, ret, cookie);
>>>>>>> refs/remotes/origin/master

	if (!desc) {
		/* DMA failed, fall back to PIO */
		if (ret >= 0)
			ret = -EIO;
		host->chan_rx = NULL;
		host->dma_active = false;
		dma_release_channel(chan);
		/* Free the Tx channel too */
		chan = host->chan_tx;
		if (chan) {
			host->chan_tx = NULL;
			dma_release_channel(chan);
		}
		dev_warn(&host->pd->dev,
			 "DMA failed: %d, falling back to PIO\n", ret);
		sh_mmcif_bitclr(host, MMCIF_CE_BUF_ACC, BUF_ACC_DMAREN | BUF_ACC_DMAWEN);
	}

	dev_dbg(&host->pd->dev, "%s(): desc %p, cookie %d, sg[%d]\n", __func__,
<<<<<<< HEAD
<<<<<<< HEAD
		desc, cookie, host->data->sg_len);
=======
		desc, cookie, data->sg_len);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		desc, cookie, data->sg_len);
>>>>>>> refs/remotes/origin/master
}

static void sh_mmcif_start_dma_tx(struct sh_mmcif_host *host)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct scatterlist *sg = host->data->sg;
=======
	struct mmc_data *data = host->mrq->data;
	struct scatterlist *sg = data->sg;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct mmc_data *data = host->mrq->data;
	struct scatterlist *sg = data->sg;
>>>>>>> refs/remotes/origin/master
	struct dma_async_tx_descriptor *desc = NULL;
	struct dma_chan *chan = host->chan_tx;
	dma_cookie_t cookie = -EINVAL;
	int ret;

<<<<<<< HEAD
<<<<<<< HEAD
	ret = dma_map_sg(chan->device->dev, sg, host->data->sg_len,
			 DMA_TO_DEVICE);
	if (ret > 0) {
		host->dma_active = true;
		desc = chan->device->device_prep_slave_sg(chan, sg, ret,
			DMA_TO_DEVICE, DMA_PREP_INTERRUPT | DMA_CTRL_ACK);
=======
=======
>>>>>>> refs/remotes/origin/master
	ret = dma_map_sg(chan->device->dev, sg, data->sg_len,
			 DMA_TO_DEVICE);
	if (ret > 0) {
		host->dma_active = true;
		desc = dmaengine_prep_slave_sg(chan, sg, ret,
			DMA_MEM_TO_DEV, DMA_PREP_INTERRUPT | DMA_CTRL_ACK);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	if (desc) {
		desc->callback = mmcif_dma_complete;
		desc->callback_param = host;
		cookie = dmaengine_submit(desc);
		sh_mmcif_bitset(host, MMCIF_CE_BUF_ACC, BUF_ACC_DMAWEN);
		dma_async_issue_pending(chan);
	}
	dev_dbg(&host->pd->dev, "%s(): mapped %d -> %d, cookie %d\n",
<<<<<<< HEAD
<<<<<<< HEAD
		__func__, host->data->sg_len, ret, cookie);
=======
		__func__, data->sg_len, ret, cookie);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		__func__, data->sg_len, ret, cookie);
>>>>>>> refs/remotes/origin/master

	if (!desc) {
		/* DMA failed, fall back to PIO */
		if (ret >= 0)
			ret = -EIO;
		host->chan_tx = NULL;
		host->dma_active = false;
		dma_release_channel(chan);
		/* Free the Rx channel too */
		chan = host->chan_rx;
		if (chan) {
			host->chan_rx = NULL;
			dma_release_channel(chan);
		}
		dev_warn(&host->pd->dev,
			 "DMA failed: %d, falling back to PIO\n", ret);
		sh_mmcif_bitclr(host, MMCIF_CE_BUF_ACC, BUF_ACC_DMAREN | BUF_ACC_DMAWEN);
	}

	dev_dbg(&host->pd->dev, "%s(): desc %p, cookie %d\n", __func__,
		desc, cookie);
}

<<<<<<< HEAD
static bool sh_mmcif_filter(struct dma_chan *chan, void *arg)
{
	dev_dbg(chan->device->dev, "%s: slave data %p\n", __func__, arg);
	chan->private = arg;
	return true;
}

static void sh_mmcif_request_dma(struct sh_mmcif_host *host,
				 struct sh_mmcif_plat_data *pdata)
{
<<<<<<< HEAD
=======
	struct sh_dmae_slave *tx, *rx;
>>>>>>> refs/remotes/origin/cm-10.0
	host->dma_active = false;

	/* We can only either use DMA for both Tx and Rx or not use it at all */
	if (pdata->dma) {
<<<<<<< HEAD
=======
		dev_warn(&host->pd->dev,
			 "Update your platform to use embedded DMA slave IDs\n");
		tx = &pdata->dma->chan_priv_tx;
		rx = &pdata->dma->chan_priv_rx;
	} else {
		tx = &host->dma_slave_tx;
		tx->slave_id = pdata->slave_id_tx;
		rx = &host->dma_slave_rx;
		rx->slave_id = pdata->slave_id_rx;
	}
	if (tx->slave_id > 0 && rx->slave_id > 0) {
>>>>>>> refs/remotes/origin/cm-10.0
		dma_cap_mask_t mask;

		dma_cap_zero(mask);
		dma_cap_set(DMA_SLAVE, mask);

<<<<<<< HEAD
		host->chan_tx = dma_request_channel(mask, sh_mmcif_filter,
						    &pdata->dma->chan_priv_tx);
=======
		host->chan_tx = dma_request_channel(mask, sh_mmcif_filter, tx);
>>>>>>> refs/remotes/origin/cm-10.0
		dev_dbg(&host->pd->dev, "%s: TX: got channel %p\n", __func__,
			host->chan_tx);

		if (!host->chan_tx)
			return;

<<<<<<< HEAD
		host->chan_rx = dma_request_channel(mask, sh_mmcif_filter,
						    &pdata->dma->chan_priv_rx);
=======
		host->chan_rx = dma_request_channel(mask, sh_mmcif_filter, rx);
>>>>>>> refs/remotes/origin/cm-10.0
		dev_dbg(&host->pd->dev, "%s: RX: got channel %p\n", __func__,
			host->chan_rx);

		if (!host->chan_rx) {
			dma_release_channel(host->chan_tx);
			host->chan_tx = NULL;
			return;
		}

		init_completion(&host->dma_complete);
	}
=======
static void sh_mmcif_request_dma(struct sh_mmcif_host *host,
				 struct sh_mmcif_plat_data *pdata)
{
	struct resource *res = platform_get_resource(host->pd, IORESOURCE_MEM, 0);
	struct dma_slave_config cfg;
	dma_cap_mask_t mask;
	int ret;

	host->dma_active = false;

	if (pdata) {
		if (pdata->slave_id_tx <= 0 || pdata->slave_id_rx <= 0)
			return;
	} else if (!host->pd->dev.of_node) {
		return;
	}

	/* We can only either use DMA for both Tx and Rx or not use it at all */
	dma_cap_zero(mask);
	dma_cap_set(DMA_SLAVE, mask);

	host->chan_tx = dma_request_slave_channel_compat(mask, shdma_chan_filter,
				pdata ? (void *)pdata->slave_id_tx : NULL,
				&host->pd->dev, "tx");
	dev_dbg(&host->pd->dev, "%s: TX: got channel %p\n", __func__,
		host->chan_tx);

	if (!host->chan_tx)
		return;

	/* In the OF case the driver will get the slave ID from the DT */
	if (pdata)
		cfg.slave_id = pdata->slave_id_tx;
	cfg.direction = DMA_MEM_TO_DEV;
	cfg.dst_addr = res->start + MMCIF_CE_DATA;
	cfg.src_addr = 0;
	ret = dmaengine_slave_config(host->chan_tx, &cfg);
	if (ret < 0)
		goto ecfgtx;

	host->chan_rx = dma_request_slave_channel_compat(mask, shdma_chan_filter,
				pdata ? (void *)pdata->slave_id_rx : NULL,
				&host->pd->dev, "rx");
	dev_dbg(&host->pd->dev, "%s: RX: got channel %p\n", __func__,
		host->chan_rx);

	if (!host->chan_rx)
		goto erqrx;

	if (pdata)
		cfg.slave_id = pdata->slave_id_rx;
	cfg.direction = DMA_DEV_TO_MEM;
	cfg.dst_addr = 0;
	cfg.src_addr = res->start + MMCIF_CE_DATA;
	ret = dmaengine_slave_config(host->chan_rx, &cfg);
	if (ret < 0)
		goto ecfgrx;

	return;

ecfgrx:
	dma_release_channel(host->chan_rx);
	host->chan_rx = NULL;
erqrx:
ecfgtx:
	dma_release_channel(host->chan_tx);
	host->chan_tx = NULL;
>>>>>>> refs/remotes/origin/master
}

static void sh_mmcif_release_dma(struct sh_mmcif_host *host)
{
	sh_mmcif_bitclr(host, MMCIF_CE_BUF_ACC, BUF_ACC_DMAREN | BUF_ACC_DMAWEN);
	/* Descriptors are freed automatically */
	if (host->chan_tx) {
		struct dma_chan *chan = host->chan_tx;
		host->chan_tx = NULL;
		dma_release_channel(chan);
	}
	if (host->chan_rx) {
		struct dma_chan *chan = host->chan_rx;
		host->chan_rx = NULL;
		dma_release_channel(chan);
	}

	host->dma_active = false;
}

static void sh_mmcif_clock_control(struct sh_mmcif_host *host, unsigned int clk)
{
	struct sh_mmcif_plat_data *p = host->pd->dev.platform_data;
<<<<<<< HEAD
=======
	bool sup_pclk = p ? p->sup_pclk : false;
>>>>>>> refs/remotes/origin/master

	sh_mmcif_bitclr(host, MMCIF_CE_CLK_CTRL, CLK_ENABLE);
	sh_mmcif_bitclr(host, MMCIF_CE_CLK_CTRL, CLK_CLEAR);

	if (!clk)
		return;
<<<<<<< HEAD
	if (p->sup_pclk && clk == host->clk)
		sh_mmcif_bitset(host, MMCIF_CE_CLK_CTRL, CLK_SUP_PCLK);
	else
		sh_mmcif_bitset(host, MMCIF_CE_CLK_CTRL, CLK_CLEAR &
<<<<<<< HEAD
			(ilog2(__rounddown_pow_of_two(host->clk / clk)) << 16));
=======
				((fls(DIV_ROUND_UP(host->clk,
						   clk) - 1) - 1) << 16));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (sup_pclk && clk == host->clk)
		sh_mmcif_bitset(host, MMCIF_CE_CLK_CTRL, CLK_SUP_PCLK);
	else
		sh_mmcif_bitset(host, MMCIF_CE_CLK_CTRL, CLK_CLEAR &
				((fls(DIV_ROUND_UP(host->clk,
						   clk) - 1) - 1) << 16));
>>>>>>> refs/remotes/origin/master

	sh_mmcif_bitset(host, MMCIF_CE_CLK_CTRL, CLK_ENABLE);
}

static void sh_mmcif_sync_reset(struct sh_mmcif_host *host)
{
	u32 tmp;

	tmp = 0x010f0000 & sh_mmcif_readl(host->addr, MMCIF_CE_CLK_CTRL);

	sh_mmcif_writel(host->addr, MMCIF_CE_VERSION, SOFT_RST_ON);
	sh_mmcif_writel(host->addr, MMCIF_CE_VERSION, SOFT_RST_OFF);
<<<<<<< HEAD
	sh_mmcif_bitset(host, MMCIF_CE_CLK_CTRL, tmp |
		SRSPTO_256 | SRBSYTO_29 | SRWDTO_29 | SCCSTO_29);
=======
	if (host->ccs_enable)
		tmp |= SCCSTO_29;
	if (host->clk_ctrl2_enable)
		sh_mmcif_writel(host->addr, MMCIF_CE_CLK_CTRL2, 0x0F0F0000);
	sh_mmcif_bitset(host, MMCIF_CE_CLK_CTRL, tmp |
		SRSPTO_256 | SRBSYTO_29 | SRWDTO_29);
>>>>>>> refs/remotes/origin/master
	/* byte swap on */
	sh_mmcif_bitset(host, MMCIF_CE_BUF_ACC, BUF_ACC_ATYP);
}

static int sh_mmcif_error_manage(struct sh_mmcif_host *host)
{
	u32 state1, state2;
<<<<<<< HEAD
<<<<<<< HEAD
	int ret, timeout = 10000000;
=======
	int ret, timeout;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	int ret, timeout;
>>>>>>> refs/remotes/origin/master

	host->sd_error = false;

	state1 = sh_mmcif_readl(host->addr, MMCIF_CE_HOST_STS1);
	state2 = sh_mmcif_readl(host->addr, MMCIF_CE_HOST_STS2);
	dev_dbg(&host->pd->dev, "ERR HOST_STS1 = %08x\n", state1);
	dev_dbg(&host->pd->dev, "ERR HOST_STS2 = %08x\n", state2);

	if (state1 & STS1_CMDSEQ) {
		sh_mmcif_bitset(host, MMCIF_CE_CMD_CTRL, CMD_CTRL_BREAK);
		sh_mmcif_bitset(host, MMCIF_CE_CMD_CTRL, ~CMD_CTRL_BREAK);
<<<<<<< HEAD
<<<<<<< HEAD
		while (1) {
			timeout--;
			if (timeout < 0) {
				dev_err(&host->pd->dev,
					"Forceed end of command sequence timeout err\n");
				return -EIO;
			}
			if (!(sh_mmcif_readl(host->addr, MMCIF_CE_HOST_STS1)
								& STS1_CMDSEQ))
				break;
			mdelay(1);
		}
=======
=======
>>>>>>> refs/remotes/origin/master
		for (timeout = 10000000; timeout; timeout--) {
			if (!(sh_mmcif_readl(host->addr, MMCIF_CE_HOST_STS1)
			      & STS1_CMDSEQ))
				break;
			mdelay(1);
		}
		if (!timeout) {
			dev_err(&host->pd->dev,
				"Forced end of command sequence timeout err\n");
			return -EIO;
		}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		sh_mmcif_sync_reset(host);
		dev_dbg(&host->pd->dev, "Forced end of command sequence\n");
		return -EIO;
	}

	if (state2 & STS2_CRC_ERR) {
<<<<<<< HEAD
<<<<<<< HEAD
		dev_dbg(&host->pd->dev, ": Happened CRC error\n");
		ret = -EIO;
	} else if (state2 & STS2_TIMEOUT_ERR) {
		dev_dbg(&host->pd->dev, ": Happened Timeout error\n");
		ret = -ETIMEDOUT;
	} else {
		dev_dbg(&host->pd->dev, ": Happened End/Index error\n");
=======
		dev_dbg(&host->pd->dev, ": CRC error\n");
		ret = -EIO;
	} else if (state2 & STS2_TIMEOUT_ERR) {
		dev_dbg(&host->pd->dev, ": Timeout\n");
		ret = -ETIMEDOUT;
	} else {
		dev_dbg(&host->pd->dev, ": End/Index error\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
		dev_err(&host->pd->dev, " CRC error: state %u, wait %u\n",
			host->state, host->wait_for);
		ret = -EIO;
	} else if (state2 & STS2_TIMEOUT_ERR) {
		dev_err(&host->pd->dev, " Timeout: state %u, wait %u\n",
			host->state, host->wait_for);
		ret = -ETIMEDOUT;
	} else {
		dev_dbg(&host->pd->dev, " End/Index error: state %u, wait %u\n",
			host->state, host->wait_for);
>>>>>>> refs/remotes/origin/master
		ret = -EIO;
	}
	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int sh_mmcif_single_read(struct sh_mmcif_host *host,
					struct mmc_request *mrq)
{
	struct mmc_data *data = mrq->data;
	long time;
	u32 blocksize, i, *p = sg_virt(data->sg);

	/* buf read enable */
	sh_mmcif_bitset(host, MMCIF_CE_INT_MASK, MASK_MBUFREN);
	time = wait_for_completion_interruptible_timeout(&host->intr_wait,
			host->timeout);
	if (time <= 0 || host->sd_error)
		return sh_mmcif_error_manage(host);

	blocksize = (BLOCK_SIZE_MASK &
			sh_mmcif_readl(host->addr, MMCIF_CE_BLOCK_SET)) + 3;
	for (i = 0; i < blocksize / 4; i++)
=======
=======
>>>>>>> refs/remotes/origin/master
static bool sh_mmcif_next_block(struct sh_mmcif_host *host, u32 *p)
{
	struct mmc_data *data = host->mrq->data;

	host->sg_blkidx += host->blocksize;

	/* data->sg->length must be a multiple of host->blocksize? */
	BUG_ON(host->sg_blkidx > data->sg->length);

	if (host->sg_blkidx == data->sg->length) {
		host->sg_blkidx = 0;
		if (++host->sg_idx < data->sg_len)
			host->pio_ptr = sg_virt(++data->sg);
	} else {
		host->pio_ptr = p;
	}

<<<<<<< HEAD
	if (host->sg_idx == data->sg_len)
		return false;

	return true;
=======
	return host->sg_idx != data->sg_len;
>>>>>>> refs/remotes/origin/master
}

static void sh_mmcif_single_read(struct sh_mmcif_host *host,
				 struct mmc_request *mrq)
{
	host->blocksize = (sh_mmcif_readl(host->addr, MMCIF_CE_BLOCK_SET) &
			   BLOCK_SIZE_MASK) + 3;

	host->wait_for = MMCIF_WAIT_FOR_READ;
<<<<<<< HEAD
	schedule_delayed_work(&host->timeout_work, host->timeout);
=======
>>>>>>> refs/remotes/origin/master

	/* buf read enable */
	sh_mmcif_bitset(host, MMCIF_CE_INT_MASK, MASK_MBUFREN);
}

static bool sh_mmcif_read_block(struct sh_mmcif_host *host)
{
	struct mmc_data *data = host->mrq->data;
	u32 *p = sg_virt(data->sg);
	int i;

	if (host->sd_error) {
		data->error = sh_mmcif_error_manage(host);
<<<<<<< HEAD
=======
		dev_dbg(&host->pd->dev, "%s(): %d\n", __func__, data->error);
>>>>>>> refs/remotes/origin/master
		return false;
	}

	for (i = 0; i < host->blocksize / 4; i++)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		*p++ = sh_mmcif_readl(host->addr, MMCIF_CE_DATA);

	/* buffer read end */
	sh_mmcif_bitset(host, MMCIF_CE_INT_MASK, MASK_MBUFRE);
<<<<<<< HEAD
<<<<<<< HEAD
	time = wait_for_completion_interruptible_timeout(&host->intr_wait,
			host->timeout);
	if (time <= 0 || host->sd_error)
		return sh_mmcif_error_manage(host);

	return 0;
}

static int sh_mmcif_multi_read(struct sh_mmcif_host *host,
					struct mmc_request *mrq)
{
	struct mmc_data *data = mrq->data;
	long time;
	u32 blocksize, i, j, sec, *p;

	blocksize = BLOCK_SIZE_MASK & sh_mmcif_readl(host->addr,
						     MMCIF_CE_BLOCK_SET);
	for (j = 0; j < data->sg_len; j++) {
		p = sg_virt(data->sg);
		for (sec = 0; sec < data->sg->length / blocksize; sec++) {
			sh_mmcif_bitset(host, MMCIF_CE_INT_MASK, MASK_MBUFREN);
			/* buf read enable */
			time = wait_for_completion_interruptible_timeout(&host->intr_wait,
				host->timeout);

			if (time <= 0 || host->sd_error)
				return sh_mmcif_error_manage(host);

			for (i = 0; i < blocksize / 4; i++)
				*p++ = sh_mmcif_readl(host->addr,
						      MMCIF_CE_DATA);
		}
		if (j < data->sg_len - 1)
			data->sg++;
	}
	return 0;
}

static int sh_mmcif_single_write(struct sh_mmcif_host *host,
					struct mmc_request *mrq)
{
	struct mmc_data *data = mrq->data;
	long time;
	u32 blocksize, i, *p = sg_virt(data->sg);

	sh_mmcif_bitset(host, MMCIF_CE_INT_MASK, MASK_MBUFWEN);

	/* buf write enable */
	time = wait_for_completion_interruptible_timeout(&host->intr_wait,
			host->timeout);
	if (time <= 0 || host->sd_error)
		return sh_mmcif_error_manage(host);

	blocksize = (BLOCK_SIZE_MASK &
			sh_mmcif_readl(host->addr, MMCIF_CE_BLOCK_SET)) + 3;
	for (i = 0; i < blocksize / 4; i++)
=======
=======
>>>>>>> refs/remotes/origin/master
	host->wait_for = MMCIF_WAIT_FOR_READ_END;

	return true;
}

static void sh_mmcif_multi_read(struct sh_mmcif_host *host,
				struct mmc_request *mrq)
{
	struct mmc_data *data = mrq->data;

	if (!data->sg_len || !data->sg->length)
		return;

	host->blocksize = sh_mmcif_readl(host->addr, MMCIF_CE_BLOCK_SET) &
		BLOCK_SIZE_MASK;

	host->wait_for = MMCIF_WAIT_FOR_MREAD;
	host->sg_idx = 0;
	host->sg_blkidx = 0;
	host->pio_ptr = sg_virt(data->sg);
<<<<<<< HEAD
	schedule_delayed_work(&host->timeout_work, host->timeout);
=======

>>>>>>> refs/remotes/origin/master
	sh_mmcif_bitset(host, MMCIF_CE_INT_MASK, MASK_MBUFREN);
}

static bool sh_mmcif_mread_block(struct sh_mmcif_host *host)
{
	struct mmc_data *data = host->mrq->data;
	u32 *p = host->pio_ptr;
	int i;

	if (host->sd_error) {
		data->error = sh_mmcif_error_manage(host);
<<<<<<< HEAD
=======
		dev_dbg(&host->pd->dev, "%s(): %d\n", __func__, data->error);
>>>>>>> refs/remotes/origin/master
		return false;
	}

	BUG_ON(!data->sg->length);

	for (i = 0; i < host->blocksize / 4; i++)
		*p++ = sh_mmcif_readl(host->addr, MMCIF_CE_DATA);

	if (!sh_mmcif_next_block(host, p))
		return false;

<<<<<<< HEAD
	schedule_delayed_work(&host->timeout_work, host->timeout);
=======
>>>>>>> refs/remotes/origin/master
	sh_mmcif_bitset(host, MMCIF_CE_INT_MASK, MASK_MBUFREN);

	return true;
}

static void sh_mmcif_single_write(struct sh_mmcif_host *host,
					struct mmc_request *mrq)
{
	host->blocksize = (sh_mmcif_readl(host->addr, MMCIF_CE_BLOCK_SET) &
			   BLOCK_SIZE_MASK) + 3;

	host->wait_for = MMCIF_WAIT_FOR_WRITE;
<<<<<<< HEAD
	schedule_delayed_work(&host->timeout_work, host->timeout);
=======
>>>>>>> refs/remotes/origin/master

	/* buf write enable */
	sh_mmcif_bitset(host, MMCIF_CE_INT_MASK, MASK_MBUFWEN);
}

static bool sh_mmcif_write_block(struct sh_mmcif_host *host)
{
	struct mmc_data *data = host->mrq->data;
	u32 *p = sg_virt(data->sg);
	int i;

	if (host->sd_error) {
		data->error = sh_mmcif_error_manage(host);
<<<<<<< HEAD
=======
		dev_dbg(&host->pd->dev, "%s(): %d\n", __func__, data->error);
>>>>>>> refs/remotes/origin/master
		return false;
	}

	for (i = 0; i < host->blocksize / 4; i++)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		sh_mmcif_writel(host->addr, MMCIF_CE_DATA, *p++);

	/* buffer write end */
	sh_mmcif_bitset(host, MMCIF_CE_INT_MASK, MASK_MDTRANE);
<<<<<<< HEAD
<<<<<<< HEAD

	time = wait_for_completion_interruptible_timeout(&host->intr_wait,
			host->timeout);
	if (time <= 0 || host->sd_error)
		return sh_mmcif_error_manage(host);

	return 0;
}

static int sh_mmcif_multi_write(struct sh_mmcif_host *host,
						struct mmc_request *mrq)
{
	struct mmc_data *data = mrq->data;
	long time;
	u32 i, sec, j, blocksize, *p;

	blocksize = BLOCK_SIZE_MASK & sh_mmcif_readl(host->addr,
						     MMCIF_CE_BLOCK_SET);

	for (j = 0; j < data->sg_len; j++) {
		p = sg_virt(data->sg);
		for (sec = 0; sec < data->sg->length / blocksize; sec++) {
			sh_mmcif_bitset(host, MMCIF_CE_INT_MASK, MASK_MBUFWEN);
			/* buf write enable*/
			time = wait_for_completion_interruptible_timeout(&host->intr_wait,
				host->timeout);

			if (time <= 0 || host->sd_error)
				return sh_mmcif_error_manage(host);

			for (i = 0; i < blocksize / 4; i++)
				sh_mmcif_writel(host->addr,
						MMCIF_CE_DATA, *p++);
		}
		if (j < data->sg_len - 1)
			data->sg++;
	}
	return 0;
=======
=======
>>>>>>> refs/remotes/origin/master
	host->wait_for = MMCIF_WAIT_FOR_WRITE_END;

	return true;
}

static void sh_mmcif_multi_write(struct sh_mmcif_host *host,
				struct mmc_request *mrq)
{
	struct mmc_data *data = mrq->data;

	if (!data->sg_len || !data->sg->length)
		return;

	host->blocksize = sh_mmcif_readl(host->addr, MMCIF_CE_BLOCK_SET) &
		BLOCK_SIZE_MASK;

	host->wait_for = MMCIF_WAIT_FOR_MWRITE;
	host->sg_idx = 0;
	host->sg_blkidx = 0;
	host->pio_ptr = sg_virt(data->sg);
<<<<<<< HEAD
	schedule_delayed_work(&host->timeout_work, host->timeout);
=======

>>>>>>> refs/remotes/origin/master
	sh_mmcif_bitset(host, MMCIF_CE_INT_MASK, MASK_MBUFWEN);
}

static bool sh_mmcif_mwrite_block(struct sh_mmcif_host *host)
{
	struct mmc_data *data = host->mrq->data;
	u32 *p = host->pio_ptr;
	int i;

	if (host->sd_error) {
		data->error = sh_mmcif_error_manage(host);
<<<<<<< HEAD
=======
		dev_dbg(&host->pd->dev, "%s(): %d\n", __func__, data->error);
>>>>>>> refs/remotes/origin/master
		return false;
	}

	BUG_ON(!data->sg->length);

	for (i = 0; i < host->blocksize / 4; i++)
		sh_mmcif_writel(host->addr, MMCIF_CE_DATA, *p++);

	if (!sh_mmcif_next_block(host, p))
		return false;

<<<<<<< HEAD
	schedule_delayed_work(&host->timeout_work, host->timeout);
	sh_mmcif_bitset(host, MMCIF_CE_INT_MASK, MASK_MBUFWEN);

	return true;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	sh_mmcif_bitset(host, MMCIF_CE_INT_MASK, MASK_MBUFWEN);

	return true;
>>>>>>> refs/remotes/origin/master
}

static void sh_mmcif_get_response(struct sh_mmcif_host *host,
						struct mmc_command *cmd)
{
	if (cmd->flags & MMC_RSP_136) {
		cmd->resp[0] = sh_mmcif_readl(host->addr, MMCIF_CE_RESP3);
		cmd->resp[1] = sh_mmcif_readl(host->addr, MMCIF_CE_RESP2);
		cmd->resp[2] = sh_mmcif_readl(host->addr, MMCIF_CE_RESP1);
		cmd->resp[3] = sh_mmcif_readl(host->addr, MMCIF_CE_RESP0);
	} else
		cmd->resp[0] = sh_mmcif_readl(host->addr, MMCIF_CE_RESP0);
}

static void sh_mmcif_get_cmd12response(struct sh_mmcif_host *host,
						struct mmc_command *cmd)
{
	cmd->resp[0] = sh_mmcif_readl(host->addr, MMCIF_CE_RESP_CMD12);
}

static u32 sh_mmcif_set_cmd(struct sh_mmcif_host *host,
<<<<<<< HEAD
<<<<<<< HEAD
		struct mmc_request *mrq, struct mmc_command *cmd, u32 opc)
{
=======
=======
>>>>>>> refs/remotes/origin/master
			    struct mmc_request *mrq)
{
	struct mmc_data *data = mrq->data;
	struct mmc_command *cmd = mrq->cmd;
	u32 opc = cmd->opcode;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	u32 tmp = 0;

	/* Response Type check */
	switch (mmc_resp_type(cmd)) {
	case MMC_RSP_NONE:
		tmp |= CMD_SET_RTYP_NO;
		break;
	case MMC_RSP_R1:
	case MMC_RSP_R1B:
	case MMC_RSP_R3:
		tmp |= CMD_SET_RTYP_6B;
		break;
	case MMC_RSP_R2:
		tmp |= CMD_SET_RTYP_17B;
		break;
	default:
		dev_err(&host->pd->dev, "Unsupported response type.\n");
		break;
	}
	switch (opc) {
	/* RBSY */
<<<<<<< HEAD
=======
	case MMC_SLEEP_AWAKE:
>>>>>>> refs/remotes/origin/master
	case MMC_SWITCH:
	case MMC_STOP_TRANSMISSION:
	case MMC_SET_WRITE_PROT:
	case MMC_CLR_WRITE_PROT:
	case MMC_ERASE:
<<<<<<< HEAD
<<<<<<< HEAD
	case MMC_GEN_CMD:
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		tmp |= CMD_SET_RBSY;
		break;
	}
	/* WDAT / DATW */
<<<<<<< HEAD
<<<<<<< HEAD
	if (host->data) {
=======
	if (data) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (data) {
>>>>>>> refs/remotes/origin/master
		tmp |= CMD_SET_WDAT;
		switch (host->bus_width) {
		case MMC_BUS_WIDTH_1:
			tmp |= CMD_SET_DATW_1;
			break;
		case MMC_BUS_WIDTH_4:
			tmp |= CMD_SET_DATW_4;
			break;
		case MMC_BUS_WIDTH_8:
			tmp |= CMD_SET_DATW_8;
			break;
		default:
			dev_err(&host->pd->dev, "Unsupported bus width.\n");
			break;
		}
<<<<<<< HEAD
=======
		switch (host->timing) {
		case MMC_TIMING_UHS_DDR50:
			/*
			 * MMC core will only set this timing, if the host
			 * advertises the MMC_CAP_UHS_DDR50 capability. MMCIF
			 * implementations with this capability, e.g. sh73a0,
			 * will have to set it in their platform data.
			 */
			tmp |= CMD_SET_DARS;
			break;
		}
>>>>>>> refs/remotes/origin/master
	}
	/* DWEN */
	if (opc == MMC_WRITE_BLOCK || opc == MMC_WRITE_MULTIPLE_BLOCK)
		tmp |= CMD_SET_DWEN;
	/* CMLTE/CMD12EN */
	if (opc == MMC_READ_MULTIPLE_BLOCK || opc == MMC_WRITE_MULTIPLE_BLOCK) {
		tmp |= CMD_SET_CMLTE | CMD_SET_CMD12EN;
		sh_mmcif_bitset(host, MMCIF_CE_BLOCK_SET,
<<<<<<< HEAD
<<<<<<< HEAD
					mrq->data->blocks << 16);
=======
				data->blocks << 16);
>>>>>>> refs/remotes/origin/cm-10.0
=======
				data->blocks << 16);
>>>>>>> refs/remotes/origin/master
	}
	/* RIDXC[1:0] check bits */
	if (opc == MMC_SEND_OP_COND || opc == MMC_ALL_SEND_CID ||
	    opc == MMC_SEND_CSD || opc == MMC_SEND_CID)
		tmp |= CMD_SET_RIDXC_BITS;
	/* RCRC7C[1:0] check bits */
	if (opc == MMC_SEND_OP_COND)
		tmp |= CMD_SET_CRC7C_BITS;
	/* RCRC7C[1:0] internal CRC7 */
	if (opc == MMC_ALL_SEND_CID ||
		opc == MMC_SEND_CSD || opc == MMC_SEND_CID)
		tmp |= CMD_SET_CRC7C_INTERNAL;

<<<<<<< HEAD
<<<<<<< HEAD
	return opc = ((opc << 24) | tmp);
}

static int sh_mmcif_data_trans(struct sh_mmcif_host *host,
				struct mmc_request *mrq, u32 opc)
{
	int ret;

	switch (opc) {
	case MMC_READ_MULTIPLE_BLOCK:
		ret = sh_mmcif_multi_read(host, mrq);
		break;
	case MMC_WRITE_MULTIPLE_BLOCK:
		ret = sh_mmcif_multi_write(host, mrq);
		break;
	case MMC_WRITE_BLOCK:
		ret = sh_mmcif_single_write(host, mrq);
		break;
	case MMC_READ_SINGLE_BLOCK:
	case MMC_SEND_EXT_CSD:
		ret = sh_mmcif_single_read(host, mrq);
		break;
	default:
		dev_err(&host->pd->dev, "UNSUPPORTED CMD = d'%08d\n", opc);
		ret = -EINVAL;
		break;
	}
	return ret;
}

static void sh_mmcif_start_cmd(struct sh_mmcif_host *host,
			struct mmc_request *mrq, struct mmc_command *cmd)
{
	long time;
	int ret = 0, mask = 0;
	u32 opc = cmd->opcode;

	switch (opc) {
	/* respons busy check */
=======
=======
>>>>>>> refs/remotes/origin/master
	return (opc << 24) | tmp;
}

static int sh_mmcif_data_trans(struct sh_mmcif_host *host,
			       struct mmc_request *mrq, u32 opc)
{
	switch (opc) {
	case MMC_READ_MULTIPLE_BLOCK:
		sh_mmcif_multi_read(host, mrq);
		return 0;
	case MMC_WRITE_MULTIPLE_BLOCK:
		sh_mmcif_multi_write(host, mrq);
		return 0;
	case MMC_WRITE_BLOCK:
		sh_mmcif_single_write(host, mrq);
		return 0;
	case MMC_READ_SINGLE_BLOCK:
	case MMC_SEND_EXT_CSD:
		sh_mmcif_single_read(host, mrq);
		return 0;
	default:
<<<<<<< HEAD
		dev_err(&host->pd->dev, "UNSUPPORTED CMD = d'%08d\n", opc);
=======
		dev_err(&host->pd->dev, "Unsupported CMD%d\n", opc);
>>>>>>> refs/remotes/origin/master
		return -EINVAL;
	}
}

static void sh_mmcif_start_cmd(struct sh_mmcif_host *host,
			       struct mmc_request *mrq)
{
	struct mmc_command *cmd = mrq->cmd;
	u32 opc = cmd->opcode;
	u32 mask;

	switch (opc) {
	/* response busy check */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
	case MMC_SLEEP_AWAKE:
>>>>>>> refs/remotes/origin/master
	case MMC_SWITCH:
	case MMC_STOP_TRANSMISSION:
	case MMC_SET_WRITE_PROT:
	case MMC_CLR_WRITE_PROT:
	case MMC_ERASE:
<<<<<<< HEAD
<<<<<<< HEAD
	case MMC_GEN_CMD:
		mask = MASK_MRBSYE;
		break;
	default:
		mask = MASK_MCRSPE;
		break;
	}
	mask |=	MASK_MCMDVIO | MASK_MBUFVIO | MASK_MWDATERR |
		MASK_MRDATERR | MASK_MRIDXERR | MASK_MRSPERR |
		MASK_MCCSTO | MASK_MCRCSTO | MASK_MWDATTO |
		MASK_MRDATTO | MASK_MRBSYTO | MASK_MRSPTO;

	if (host->data) {
=======
=======
>>>>>>> refs/remotes/origin/master
		mask = MASK_START_CMD | MASK_MRBSYE;
		break;
	default:
		mask = MASK_START_CMD | MASK_MCRSPE;
		break;
	}

<<<<<<< HEAD
	if (mrq->data) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (host->ccs_enable)
		mask |= MASK_MCCSTO;

	if (mrq->data) {
>>>>>>> refs/remotes/origin/master
		sh_mmcif_writel(host->addr, MMCIF_CE_BLOCK_SET, 0);
		sh_mmcif_writel(host->addr, MMCIF_CE_BLOCK_SET,
				mrq->data->blksz);
	}
<<<<<<< HEAD
<<<<<<< HEAD
	opc = sh_mmcif_set_cmd(host, mrq, cmd, opc);
=======
	opc = sh_mmcif_set_cmd(host, mrq);
>>>>>>> refs/remotes/origin/cm-10.0

	sh_mmcif_writel(host->addr, MMCIF_CE_INT, 0xD80430C0);
=======
	opc = sh_mmcif_set_cmd(host, mrq);

	if (host->ccs_enable)
		sh_mmcif_writel(host->addr, MMCIF_CE_INT, 0xD80430C0);
	else
		sh_mmcif_writel(host->addr, MMCIF_CE_INT, 0xD80430C0 | INT_CCS);
>>>>>>> refs/remotes/origin/master
	sh_mmcif_writel(host->addr, MMCIF_CE_INT_MASK, mask);
	/* set arg */
	sh_mmcif_writel(host->addr, MMCIF_CE_ARG, cmd->arg);
	/* set cmd */
	sh_mmcif_writel(host->addr, MMCIF_CE_CMD_SET, opc);

<<<<<<< HEAD
<<<<<<< HEAD
	time = wait_for_completion_interruptible_timeout(&host->intr_wait,
		host->timeout);
	if (time <= 0) {
		cmd->error = sh_mmcif_error_manage(host);
		return;
	}
	if (host->sd_error) {
		switch (cmd->opcode) {
		case MMC_ALL_SEND_CID:
		case MMC_SELECT_CARD:
		case MMC_APP_CMD:
			cmd->error = -ETIMEDOUT;
			break;
		default:
			dev_dbg(&host->pd->dev, "Cmd(d'%d) err\n",
					cmd->opcode);
			cmd->error = sh_mmcif_error_manage(host);
			break;
		}
		host->sd_error = false;
		return;
	}
	if (!(cmd->flags & MMC_RSP_PRESENT)) {
		cmd->error = 0;
		return;
	}
	sh_mmcif_get_response(host, cmd);
	if (host->data) {
		if (!host->dma_active) {
			ret = sh_mmcif_data_trans(host, mrq, cmd->opcode);
		} else {
			long time =
				wait_for_completion_interruptible_timeout(&host->dma_complete,
									  host->timeout);
			if (!time)
				ret = -ETIMEDOUT;
			else if (time < 0)
				ret = time;
			sh_mmcif_bitclr(host, MMCIF_CE_BUF_ACC,
					BUF_ACC_DMAREN | BUF_ACC_DMAWEN);
			host->dma_active = false;
		}
		if (ret < 0)
			mrq->data->bytes_xfered = 0;
		else
			mrq->data->bytes_xfered =
				mrq->data->blocks * mrq->data->blksz;
	}
	cmd->error = ret;
}

static void sh_mmcif_stop_cmd(struct sh_mmcif_host *host,
		struct mmc_request *mrq, struct mmc_command *cmd)
{
	long time;

	if (mrq->cmd->opcode == MMC_READ_MULTIPLE_BLOCK)
		sh_mmcif_bitset(host, MMCIF_CE_INT_MASK, MASK_MCMD12DRE);
	else if (mrq->cmd->opcode == MMC_WRITE_MULTIPLE_BLOCK)
		sh_mmcif_bitset(host, MMCIF_CE_INT_MASK, MASK_MCMD12RBE);
	else {
		dev_err(&host->pd->dev, "unsupported stop cmd\n");
		cmd->error = sh_mmcif_error_manage(host);
		return;
	}

	time = wait_for_completion_interruptible_timeout(&host->intr_wait,
			host->timeout);
	if (time <= 0 || host->sd_error) {
		cmd->error = sh_mmcif_error_manage(host);
		return;
	}
	sh_mmcif_get_cmd12response(host, cmd);
	cmd->error = 0;
=======
=======
>>>>>>> refs/remotes/origin/master
	host->wait_for = MMCIF_WAIT_FOR_CMD;
	schedule_delayed_work(&host->timeout_work, host->timeout);
}

static void sh_mmcif_stop_cmd(struct sh_mmcif_host *host,
			      struct mmc_request *mrq)
{
	switch (mrq->cmd->opcode) {
	case MMC_READ_MULTIPLE_BLOCK:
		sh_mmcif_bitset(host, MMCIF_CE_INT_MASK, MASK_MCMD12DRE);
		break;
	case MMC_WRITE_MULTIPLE_BLOCK:
		sh_mmcif_bitset(host, MMCIF_CE_INT_MASK, MASK_MCMD12RBE);
		break;
	default:
		dev_err(&host->pd->dev, "unsupported stop cmd\n");
		mrq->stop->error = sh_mmcif_error_manage(host);
		return;
	}

	host->wait_for = MMCIF_WAIT_FOR_STOP;
<<<<<<< HEAD
	schedule_delayed_work(&host->timeout_work, host->timeout);
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static void sh_mmcif_request(struct mmc_host *mmc, struct mmc_request *mrq)
{
	struct sh_mmcif_host *host = mmc_priv(mmc);
	unsigned long flags;

	spin_lock_irqsave(&host->lock, flags);
	if (host->state != STATE_IDLE) {
<<<<<<< HEAD
=======
		dev_dbg(&host->pd->dev, "%s() rejected, state %u\n", __func__, host->state);
>>>>>>> refs/remotes/origin/master
		spin_unlock_irqrestore(&host->lock, flags);
		mrq->cmd->error = -EAGAIN;
		mmc_request_done(mmc, mrq);
		return;
	}

	host->state = STATE_REQUEST;
	spin_unlock_irqrestore(&host->lock, flags);

	switch (mrq->cmd->opcode) {
	/* MMCIF does not support SD/SDIO command */
<<<<<<< HEAD
	case SD_IO_SEND_OP_COND:
	case MMC_APP_CMD:
=======
	case MMC_SLEEP_AWAKE: /* = SD_IO_SEND_OP_COND (5) */
	case MMC_SEND_EXT_CSD: /* = SD_SEND_IF_COND (8) */
		if ((mrq->cmd->flags & MMC_CMD_MASK) != MMC_CMD_BCR)
			break;
	case MMC_APP_CMD:
	case SD_IO_RW_DIRECT:
>>>>>>> refs/remotes/origin/master
		host->state = STATE_IDLE;
		mrq->cmd->error = -ETIMEDOUT;
		mmc_request_done(mmc, mrq);
		return;
<<<<<<< HEAD
	case MMC_SEND_EXT_CSD: /* = SD_SEND_IF_COND (8) */
		if (!mrq->data) {
			/* send_if_cond cmd (not support) */
			host->state = STATE_IDLE;
			mrq->cmd->error = -ETIMEDOUT;
			mmc_request_done(mmc, mrq);
			return;
		}
		break;
	default:
		break;
	}
<<<<<<< HEAD
	host->data = mrq->data;
	if (mrq->data) {
		if (mrq->data->flags & MMC_DATA_READ) {
			if (host->chan_rx)
				sh_mmcif_start_dma_rx(host);
		} else {
			if (host->chan_tx)
				sh_mmcif_start_dma_tx(host);
		}
	}
	sh_mmcif_start_cmd(host, mrq, mrq->cmd);
	host->data = NULL;

	if (!mrq->cmd->error && mrq->stop)
		sh_mmcif_stop_cmd(host, mrq, mrq->stop);
	host->state = STATE_IDLE;
	mmc_request_done(mmc, mrq);
=======
=======
	default:
		break;
	}
>>>>>>> refs/remotes/origin/master

	host->mrq = mrq;

	sh_mmcif_start_cmd(host, mrq);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
}

static int sh_mmcif_clk_update(struct sh_mmcif_host *host)
{
	int ret = clk_prepare_enable(host->hclk);

	if (!ret) {
		host->clk = clk_get_rate(host->hclk);
		host->mmc->f_max = host->clk / 2;
		host->mmc->f_min = host->clk / 512;
	}

	return ret;
}

static void sh_mmcif_set_power(struct sh_mmcif_host *host, struct mmc_ios *ios)
{
	struct mmc_host *mmc = host->mmc;

	if (!IS_ERR(mmc->supply.vmmc))
		/* Errors ignored... */
		mmc_regulator_set_ocr(mmc, mmc->supply.vmmc,
				      ios->power_mode ? ios->vdd : 0);
>>>>>>> refs/remotes/origin/master
}

static void sh_mmcif_set_ios(struct mmc_host *mmc, struct mmc_ios *ios)
{
	struct sh_mmcif_host *host = mmc_priv(mmc);
<<<<<<< HEAD
	struct sh_mmcif_plat_data *p = host->pd->dev.platform_data;
=======
>>>>>>> refs/remotes/origin/master
	unsigned long flags;

	spin_lock_irqsave(&host->lock, flags);
	if (host->state != STATE_IDLE) {
<<<<<<< HEAD
=======
		dev_dbg(&host->pd->dev, "%s() rejected, state %u\n", __func__, host->state);
>>>>>>> refs/remotes/origin/master
		spin_unlock_irqrestore(&host->lock, flags);
		return;
	}

	host->state = STATE_IOS;
	spin_unlock_irqrestore(&host->lock, flags);

	if (ios->power_mode == MMC_POWER_UP) {
<<<<<<< HEAD
<<<<<<< HEAD
		if (p->set_pwr)
			p->set_pwr(host->pd, ios->power_mode);
		if (!host->power) {
			/* See if we also get DMA */
			sh_mmcif_request_dma(host, host->pd->dev.platform_data);
			pm_runtime_get_sync(&host->pd->dev);
			host->power = true;
=======
=======
>>>>>>> refs/remotes/origin/master
		if (!host->card_present) {
			/* See if we also get DMA */
			sh_mmcif_request_dma(host, host->pd->dev.platform_data);
			host->card_present = true;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		}
=======
		}
		sh_mmcif_set_power(host, ios);
>>>>>>> refs/remotes/origin/master
	} else if (ios->power_mode == MMC_POWER_OFF || !ios->clock) {
		/* clock stop */
		sh_mmcif_clock_control(host, 0);
		if (ios->power_mode == MMC_POWER_OFF) {
<<<<<<< HEAD
<<<<<<< HEAD
			if (host->power) {
				pm_runtime_put(&host->pd->dev);
				sh_mmcif_release_dma(host);
				host->power = false;
			}
			if (p->down_pwr)
=======
=======
>>>>>>> refs/remotes/origin/master
			if (host->card_present) {
				sh_mmcif_release_dma(host);
				host->card_present = false;
			}
		}
		if (host->power) {
<<<<<<< HEAD
			pm_runtime_put(&host->pd->dev);
			host->power = false;
			if (p->down_pwr && ios->power_mode == MMC_POWER_OFF)
>>>>>>> refs/remotes/origin/cm-10.0
				p->down_pwr(host->pd);
=======
			pm_runtime_put_sync(&host->pd->dev);
			clk_disable_unprepare(host->hclk);
			host->power = false;
			if (ios->power_mode == MMC_POWER_OFF)
				sh_mmcif_set_power(host, ios);
>>>>>>> refs/remotes/origin/master
		}
		host->state = STATE_IDLE;
		return;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	if (ios->clock)
		sh_mmcif_clock_control(host, ios->clock);
=======
	if (ios->clock) {
		if (!host->power) {
			if (p->set_pwr)
				p->set_pwr(host->pd, ios->power_mode);
=======
	if (ios->clock) {
		if (!host->power) {
			sh_mmcif_clk_update(host);
>>>>>>> refs/remotes/origin/master
			pm_runtime_get_sync(&host->pd->dev);
			host->power = true;
			sh_mmcif_sync_reset(host);
		}
		sh_mmcif_clock_control(host, ios->clock);
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

=======

	host->timing = ios->timing;
>>>>>>> refs/remotes/origin/master
	host->bus_width = ios->bus_width;
	host->state = STATE_IDLE;
}

static int sh_mmcif_get_cd(struct mmc_host *mmc)
{
	struct sh_mmcif_host *host = mmc_priv(mmc);
	struct sh_mmcif_plat_data *p = host->pd->dev.platform_data;
<<<<<<< HEAD

	if (!p->get_cd)
=======
	int ret = mmc_gpio_get_cd(mmc);

	if (ret >= 0)
		return ret;

	if (!p || !p->get_cd)
>>>>>>> refs/remotes/origin/master
		return -ENOSYS;
	else
		return p->get_cd(host->pd);
}

static struct mmc_host_ops sh_mmcif_ops = {
	.request	= sh_mmcif_request,
	.set_ios	= sh_mmcif_set_ios,
	.get_cd		= sh_mmcif_get_cd,
};

<<<<<<< HEAD
<<<<<<< HEAD
static void sh_mmcif_detect(struct mmc_host *mmc)
{
	mmc_detect_change(mmc, 0);
=======
=======
>>>>>>> refs/remotes/origin/master
static bool sh_mmcif_end_cmd(struct sh_mmcif_host *host)
{
	struct mmc_command *cmd = host->mrq->cmd;
	struct mmc_data *data = host->mrq->data;
	long time;

	if (host->sd_error) {
		switch (cmd->opcode) {
		case MMC_ALL_SEND_CID:
		case MMC_SELECT_CARD:
		case MMC_APP_CMD:
			cmd->error = -ETIMEDOUT;
<<<<<<< HEAD
			host->sd_error = false;
			break;
		default:
			cmd->error = sh_mmcif_error_manage(host);
			dev_dbg(&host->pd->dev, "Cmd(d'%d) error %d\n",
				cmd->opcode, cmd->error);
			break;
		}
=======
			break;
		default:
			cmd->error = sh_mmcif_error_manage(host);
			break;
		}
		dev_dbg(&host->pd->dev, "CMD%d error %d\n",
			cmd->opcode, cmd->error);
		host->sd_error = false;
>>>>>>> refs/remotes/origin/master
		return false;
	}
	if (!(cmd->flags & MMC_RSP_PRESENT)) {
		cmd->error = 0;
		return false;
	}

	sh_mmcif_get_response(host, cmd);

	if (!data)
		return false;

<<<<<<< HEAD
=======
	/*
	 * Completion can be signalled from DMA callback and error, so, have to
	 * reset here, before setting .dma_active
	 */
	init_completion(&host->dma_complete);

>>>>>>> refs/remotes/origin/master
	if (data->flags & MMC_DATA_READ) {
		if (host->chan_rx)
			sh_mmcif_start_dma_rx(host);
	} else {
		if (host->chan_tx)
			sh_mmcif_start_dma_tx(host);
	}

	if (!host->dma_active) {
		data->error = sh_mmcif_data_trans(host, host->mrq, cmd->opcode);
<<<<<<< HEAD
		if (!data->error)
			return true;
		return false;
=======
		return !data->error;
>>>>>>> refs/remotes/origin/master
	}

	/* Running in the IRQ thread, can sleep */
	time = wait_for_completion_interruptible_timeout(&host->dma_complete,
							 host->timeout);
<<<<<<< HEAD
=======

	if (data->flags & MMC_DATA_READ)
		dma_unmap_sg(host->chan_rx->device->dev,
			     data->sg, data->sg_len,
			     DMA_FROM_DEVICE);
	else
		dma_unmap_sg(host->chan_tx->device->dev,
			     data->sg, data->sg_len,
			     DMA_TO_DEVICE);

>>>>>>> refs/remotes/origin/master
	if (host->sd_error) {
		dev_err(host->mmc->parent,
			"Error IRQ while waiting for DMA completion!\n");
		/* Woken up by an error IRQ: abort DMA */
<<<<<<< HEAD
		if (data->flags & MMC_DATA_READ)
			dmaengine_terminate_all(host->chan_rx);
		else
			dmaengine_terminate_all(host->chan_tx);
		data->error = sh_mmcif_error_manage(host);
	} else if (!time) {
		data->error = -ETIMEDOUT;
	} else if (time < 0) {
=======
		data->error = sh_mmcif_error_manage(host);
	} else if (!time) {
		dev_err(host->mmc->parent, "DMA timeout!\n");
		data->error = -ETIMEDOUT;
	} else if (time < 0) {
		dev_err(host->mmc->parent,
			"wait_for_completion_...() error %ld!\n", time);
>>>>>>> refs/remotes/origin/master
		data->error = time;
	}
	sh_mmcif_bitclr(host, MMCIF_CE_BUF_ACC,
			BUF_ACC_DMAREN | BUF_ACC_DMAWEN);
	host->dma_active = false;

<<<<<<< HEAD
	if (data->error)
		data->bytes_xfered = 0;
=======
	if (data->error) {
		data->bytes_xfered = 0;
		/* Abort DMA */
		if (data->flags & MMC_DATA_READ)
			dmaengine_terminate_all(host->chan_rx);
		else
			dmaengine_terminate_all(host->chan_tx);
	}
>>>>>>> refs/remotes/origin/master

	return false;
}

static irqreturn_t sh_mmcif_irqt(int irq, void *dev_id)
{
	struct sh_mmcif_host *host = dev_id;
<<<<<<< HEAD
	struct mmc_request *mrq = host->mrq;

	cancel_delayed_work_sync(&host->timeout_work);

=======
	struct mmc_request *mrq;
	bool wait = false;

	cancel_delayed_work_sync(&host->timeout_work);

	mutex_lock(&host->thread_lock);

	mrq = host->mrq;
	if (!mrq) {
		dev_dbg(&host->pd->dev, "IRQ thread state %u, wait %u: NULL mrq!\n",
			host->state, host->wait_for);
		mutex_unlock(&host->thread_lock);
		return IRQ_HANDLED;
	}

>>>>>>> refs/remotes/origin/master
	/*
	 * All handlers return true, if processing continues, and false, if the
	 * request has to be completed - successfully or not
	 */
	switch (host->wait_for) {
	case MMCIF_WAIT_FOR_REQUEST:
		/* We're too late, the timeout has already kicked in */
<<<<<<< HEAD
		return IRQ_HANDLED;
	case MMCIF_WAIT_FOR_CMD:
		if (sh_mmcif_end_cmd(host))
			/* Wait for data */
			return IRQ_HANDLED;
		break;
	case MMCIF_WAIT_FOR_MREAD:
		if (sh_mmcif_mread_block(host))
			/* Wait for more data */
			return IRQ_HANDLED;
		break;
	case MMCIF_WAIT_FOR_READ:
		if (sh_mmcif_read_block(host))
			/* Wait for data end */
			return IRQ_HANDLED;
		break;
	case MMCIF_WAIT_FOR_MWRITE:
		if (sh_mmcif_mwrite_block(host))
			/* Wait data to write */
			return IRQ_HANDLED;
		break;
	case MMCIF_WAIT_FOR_WRITE:
		if (sh_mmcif_write_block(host))
			/* Wait for data end */
			return IRQ_HANDLED;
=======
		mutex_unlock(&host->thread_lock);
		return IRQ_HANDLED;
	case MMCIF_WAIT_FOR_CMD:
		/* Wait for data? */
		wait = sh_mmcif_end_cmd(host);
		break;
	case MMCIF_WAIT_FOR_MREAD:
		/* Wait for more data? */
		wait = sh_mmcif_mread_block(host);
		break;
	case MMCIF_WAIT_FOR_READ:
		/* Wait for data end? */
		wait = sh_mmcif_read_block(host);
		break;
	case MMCIF_WAIT_FOR_MWRITE:
		/* Wait data to write? */
		wait = sh_mmcif_mwrite_block(host);
		break;
	case MMCIF_WAIT_FOR_WRITE:
		/* Wait for data end? */
		wait = sh_mmcif_write_block(host);
>>>>>>> refs/remotes/origin/master
		break;
	case MMCIF_WAIT_FOR_STOP:
		if (host->sd_error) {
			mrq->stop->error = sh_mmcif_error_manage(host);
<<<<<<< HEAD
=======
			dev_dbg(&host->pd->dev, "%s(): %d\n", __func__, mrq->stop->error);
>>>>>>> refs/remotes/origin/master
			break;
		}
		sh_mmcif_get_cmd12response(host, mrq->stop);
		mrq->stop->error = 0;
		break;
	case MMCIF_WAIT_FOR_READ_END:
	case MMCIF_WAIT_FOR_WRITE_END:
<<<<<<< HEAD
		if (host->sd_error)
			mrq->data->error = sh_mmcif_error_manage(host);
<<<<<<< HEAD
=======
		if (host->sd_error) {
			mrq->data->error = sh_mmcif_error_manage(host);
			dev_dbg(&host->pd->dev, "%s(): %d\n", __func__, mrq->data->error);
		}
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
		break;
	default:
		BUG();
	}

<<<<<<< HEAD
=======
	if (wait) {
		schedule_delayed_work(&host->timeout_work, host->timeout);
		/* Wait for more data */
		mutex_unlock(&host->thread_lock);
		return IRQ_HANDLED;
	}

>>>>>>> refs/remotes/origin/master
	if (host->wait_for != MMCIF_WAIT_FOR_STOP) {
		struct mmc_data *data = mrq->data;
		if (!mrq->cmd->error && data && !data->error)
			data->bytes_xfered =
				data->blocks * data->blksz;

		if (mrq->stop && !mrq->cmd->error && (!data || !data->error)) {
			sh_mmcif_stop_cmd(host, mrq);
<<<<<<< HEAD
			if (!mrq->stop->error)
				return IRQ_HANDLED;
=======
			if (!mrq->stop->error) {
				schedule_delayed_work(&host->timeout_work, host->timeout);
				mutex_unlock(&host->thread_lock);
				return IRQ_HANDLED;
			}
>>>>>>> refs/remotes/origin/master
		}
	}

	host->wait_for = MMCIF_WAIT_FOR_REQUEST;
	host->state = STATE_IDLE;
	host->mrq = NULL;
	mmc_request_done(host->mmc, mrq);

<<<<<<< HEAD
	return IRQ_HANDLED;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	mutex_unlock(&host->thread_lock);

	return IRQ_HANDLED;
>>>>>>> refs/remotes/origin/master
}

static irqreturn_t sh_mmcif_intr(int irq, void *dev_id)
{
	struct sh_mmcif_host *host = dev_id;
<<<<<<< HEAD
	u32 state;
	int err = 0;

	state = sh_mmcif_readl(host->addr, MMCIF_CE_INT);

<<<<<<< HEAD
	if (state & INT_RBSYE) {
=======
	if (state & INT_ERR_STS) {
		/* error interrupts - process first */
		sh_mmcif_writel(host->addr, MMCIF_CE_INT, ~state);
		sh_mmcif_bitclr(host, MMCIF_CE_INT_MASK, state);
		err = 1;
	} else if (state & INT_RBSYE) {
>>>>>>> refs/remotes/origin/cm-10.0
		sh_mmcif_writel(host->addr, MMCIF_CE_INT,
				~(INT_RBSYE | INT_CRSPE));
		sh_mmcif_bitclr(host, MMCIF_CE_INT_MASK, MASK_MRBSYE);
	} else if (state & INT_CRSPE) {
		sh_mmcif_writel(host->addr, MMCIF_CE_INT, ~INT_CRSPE);
		sh_mmcif_bitclr(host, MMCIF_CE_INT_MASK, MASK_MCRSPE);
	} else if (state & INT_BUFREN) {
		sh_mmcif_writel(host->addr, MMCIF_CE_INT, ~INT_BUFREN);
		sh_mmcif_bitclr(host, MMCIF_CE_INT_MASK, MASK_MBUFREN);
	} else if (state & INT_BUFWEN) {
		sh_mmcif_writel(host->addr, MMCIF_CE_INT, ~INT_BUFWEN);
		sh_mmcif_bitclr(host, MMCIF_CE_INT_MASK, MASK_MBUFWEN);
	} else if (state & INT_CMD12DRE) {
		sh_mmcif_writel(host->addr, MMCIF_CE_INT,
			~(INT_CMD12DRE | INT_CMD12RBE |
			  INT_CMD12CRE | INT_BUFRE));
		sh_mmcif_bitclr(host, MMCIF_CE_INT_MASK, MASK_MCMD12DRE);
	} else if (state & INT_BUFRE) {
		sh_mmcif_writel(host->addr, MMCIF_CE_INT, ~INT_BUFRE);
		sh_mmcif_bitclr(host, MMCIF_CE_INT_MASK, MASK_MBUFRE);
	} else if (state & INT_DTRANE) {
		sh_mmcif_writel(host->addr, MMCIF_CE_INT, ~INT_DTRANE);
		sh_mmcif_bitclr(host, MMCIF_CE_INT_MASK, MASK_MDTRANE);
	} else if (state & INT_CMD12RBE) {
		sh_mmcif_writel(host->addr, MMCIF_CE_INT,
				~(INT_CMD12RBE | INT_CMD12CRE));
		sh_mmcif_bitclr(host, MMCIF_CE_INT_MASK, MASK_MCMD12RBE);
<<<<<<< HEAD
	} else if (state & INT_ERR_STS) {
		/* err interrupts */
		sh_mmcif_writel(host->addr, MMCIF_CE_INT, ~state);
		sh_mmcif_bitclr(host, MMCIF_CE_INT_MASK, state);
		err = 1;
=======
>>>>>>> refs/remotes/origin/cm-10.0
	} else {
		dev_dbg(&host->pd->dev, "Unsupported interrupt: 0x%x\n", state);
		sh_mmcif_writel(host->addr, MMCIF_CE_INT, ~state);
		sh_mmcif_bitclr(host, MMCIF_CE_INT_MASK, state);
		err = 1;
	}
	if (err) {
		host->sd_error = true;
		dev_dbg(&host->pd->dev, "int err state = %08x\n", state);
	}
<<<<<<< HEAD
	if (state & ~(INT_CMD12RBE | INT_CMD12CRE))
		complete(&host->intr_wait);
	else
		dev_dbg(&host->pd->dev, "Unexpected IRQ 0x%x\n", state);
=======
	if (state & ~(INT_CMD12RBE | INT_CMD12CRE)) {
=======
	u32 state, mask;

	state = sh_mmcif_readl(host->addr, MMCIF_CE_INT);
	mask = sh_mmcif_readl(host->addr, MMCIF_CE_INT_MASK);
	if (host->ccs_enable)
		sh_mmcif_writel(host->addr, MMCIF_CE_INT, ~(state & mask));
	else
		sh_mmcif_writel(host->addr, MMCIF_CE_INT, INT_CCS | ~(state & mask));
	sh_mmcif_bitclr(host, MMCIF_CE_INT_MASK, state & MASK_CLEAN);

	if (state & ~MASK_CLEAN)
		dev_dbg(&host->pd->dev, "IRQ state = 0x%08x incompletely cleared\n",
			state);

	if (state & INT_ERR_STS || state & ~INT_ALL) {
		host->sd_error = true;
		dev_dbg(&host->pd->dev, "int err state = 0x%08x\n", state);
	}
	if (state & ~(INT_CMD12RBE | INT_CMD12CRE)) {
		if (!host->mrq)
			dev_dbg(&host->pd->dev, "NULL IRQ state = 0x%08x\n", state);
>>>>>>> refs/remotes/origin/master
		if (!host->dma_active)
			return IRQ_WAKE_THREAD;
		else if (host->sd_error)
			mmcif_dma_complete(host);
	} else {
		dev_dbg(&host->pd->dev, "Unexpected IRQ 0x%x\n", state);
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return IRQ_HANDLED;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static void mmcif_timeout_work(struct work_struct *work)
{
	struct delayed_work *d = container_of(work, struct delayed_work, work);
	struct sh_mmcif_host *host = container_of(d, struct sh_mmcif_host, timeout_work);
	struct mmc_request *mrq = host->mrq;
<<<<<<< HEAD
=======
	unsigned long flags;
>>>>>>> refs/remotes/origin/master

	if (host->dying)
		/* Don't run after mmc_remove_host() */
		return;

<<<<<<< HEAD
=======
	dev_err(&host->pd->dev, "Timeout waiting for %u on CMD%u\n",
		host->wait_for, mrq->cmd->opcode);

	spin_lock_irqsave(&host->lock, flags);
	if (host->state == STATE_IDLE) {
		spin_unlock_irqrestore(&host->lock, flags);
		return;
	}

	host->state = STATE_TIMEOUT;
	spin_unlock_irqrestore(&host->lock, flags);

>>>>>>> refs/remotes/origin/master
	/*
	 * Handle races with cancel_delayed_work(), unless
	 * cancel_delayed_work_sync() is used
	 */
	switch (host->wait_for) {
	case MMCIF_WAIT_FOR_CMD:
		mrq->cmd->error = sh_mmcif_error_manage(host);
		break;
	case MMCIF_WAIT_FOR_STOP:
		mrq->stop->error = sh_mmcif_error_manage(host);
		break;
	case MMCIF_WAIT_FOR_MREAD:
	case MMCIF_WAIT_FOR_MWRITE:
	case MMCIF_WAIT_FOR_READ:
	case MMCIF_WAIT_FOR_WRITE:
	case MMCIF_WAIT_FOR_READ_END:
	case MMCIF_WAIT_FOR_WRITE_END:
		mrq->data->error = sh_mmcif_error_manage(host);
		break;
	default:
		BUG();
	}

	host->state = STATE_IDLE;
	host->wait_for = MMCIF_WAIT_FOR_REQUEST;
	host->mrq = NULL;
	mmc_request_done(host->mmc, mrq);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
static int __devinit sh_mmcif_probe(struct platform_device *pdev)
=======
static void sh_mmcif_init_ocr(struct sh_mmcif_host *host)
{
	struct sh_mmcif_plat_data *pd = host->pd->dev.platform_data;
	struct mmc_host *mmc = host->mmc;

	mmc_regulator_get_supply(mmc);

	if (!pd)
		return;

	if (!mmc->ocr_avail)
		mmc->ocr_avail = pd->ocr;
	else if (pd->ocr)
		dev_warn(mmc_dev(mmc), "Platform OCR mask is ignored\n");
}

static int sh_mmcif_probe(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	int ret = 0, irq[2];
	struct mmc_host *mmc;
	struct sh_mmcif_host *host;
<<<<<<< HEAD
	struct sh_mmcif_plat_data *pd;
	struct resource *res;
	void __iomem *reg;
	char clk_name[8];

	irq[0] = platform_get_irq(pdev, 0);
	irq[1] = platform_get_irq(pdev, 1);
	if (irq[0] < 0 || irq[1] < 0) {
=======
	struct sh_mmcif_plat_data *pd = pdev->dev.platform_data;
	struct resource *res;
	void __iomem *reg;
	const char *name;

	irq[0] = platform_get_irq(pdev, 0);
	irq[1] = platform_get_irq(pdev, 1);
	if (irq[0] < 0) {
>>>>>>> refs/remotes/origin/master
		dev_err(&pdev->dev, "Get irq error\n");
		return -ENXIO;
	}
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev, "platform_get_resource error.\n");
		return -ENXIO;
	}
	reg = ioremap(res->start, resource_size(res));
	if (!reg) {
		dev_err(&pdev->dev, "ioremap error.\n");
		return -ENOMEM;
	}
<<<<<<< HEAD
	pd = pdev->dev.platform_data;
	if (!pd) {
		dev_err(&pdev->dev, "sh_mmcif plat data error.\n");
		ret = -ENXIO;
		goto clean_up;
	}
	mmc = mmc_alloc_host(sizeof(struct sh_mmcif_host), &pdev->dev);
	if (!mmc) {
		ret = -ENOMEM;
		goto clean_up;
	}
	host		= mmc_priv(mmc);
	host->mmc	= mmc;
	host->addr	= reg;
	host->timeout	= 1000;

	snprintf(clk_name, sizeof(clk_name), "mmc%d", pdev->id);
	host->hclk = clk_get(&pdev->dev, clk_name);
	if (IS_ERR(host->hclk)) {
		dev_err(&pdev->dev, "cannot get clock \"%s\"\n", clk_name);
		ret = PTR_ERR(host->hclk);
		goto clean_up1;
	}
	clk_enable(host->hclk);
	host->clk = clk_get_rate(host->hclk);
	host->pd = pdev;

<<<<<<< HEAD
	init_completion(&host->intr_wait);
	spin_lock_init(&host->lock);

	mmc->ops = &sh_mmcif_ops;
	mmc->f_max = host->clk;
	/* close to 400KHz */
	if (mmc->f_max < 51200000)
		mmc->f_min = mmc->f_max / 128;
	else if (mmc->f_max < 102400000)
		mmc->f_min = mmc->f_max / 256;
	else
		mmc->f_min = mmc->f_max / 512;
=======
	spin_lock_init(&host->lock);

	mmc->ops = &sh_mmcif_ops;
	mmc->f_max = host->clk / 2;
	mmc->f_min = host->clk / 512;
>>>>>>> refs/remotes/origin/cm-10.0
	if (pd->ocr)
		mmc->ocr_avail = pd->ocr;
	mmc->caps = MMC_CAP_MMC_HIGHSPEED;
	if (pd->caps)
=======

	mmc = mmc_alloc_host(sizeof(struct sh_mmcif_host), &pdev->dev);
	if (!mmc) {
		ret = -ENOMEM;
		goto ealloch;
	}

	ret = mmc_of_parse(mmc);
	if (ret < 0)
		goto eofparse;

	host		= mmc_priv(mmc);
	host->mmc	= mmc;
	host->addr	= reg;
	host->timeout	= msecs_to_jiffies(1000);
	host->ccs_enable = !pd || !pd->ccs_unsupported;
	host->clk_ctrl2_enable = pd && pd->clk_ctrl2_present;

	host->pd = pdev;

	spin_lock_init(&host->lock);

	mmc->ops = &sh_mmcif_ops;
	sh_mmcif_init_ocr(host);

	mmc->caps |= MMC_CAP_MMC_HIGHSPEED | MMC_CAP_WAIT_WHILE_BUSY;
	if (pd && pd->caps)
>>>>>>> refs/remotes/origin/master
		mmc->caps |= pd->caps;
	mmc->max_segs = 32;
	mmc->max_blk_size = 512;
	mmc->max_req_size = PAGE_CACHE_SIZE * mmc->max_segs;
	mmc->max_blk_count = mmc->max_req_size / mmc->max_blk_size;
	mmc->max_seg_size = mmc->max_req_size;

<<<<<<< HEAD
	sh_mmcif_sync_reset(host);
=======
>>>>>>> refs/remotes/origin/master
	platform_set_drvdata(pdev, host);

	pm_runtime_enable(&pdev->dev);
	host->power = false;

<<<<<<< HEAD
	ret = pm_runtime_resume(&pdev->dev);
	if (ret < 0)
		goto clean_up2;

<<<<<<< HEAD
	mmc_add_host(mmc);

	sh_mmcif_writel(host->addr, MMCIF_CE_INT_MASK, MASK_ALL);

	ret = request_irq(irq[0], sh_mmcif_intr, 0, "sh_mmc:error", host);
=======
	INIT_DELAYED_WORK(&host->timeout_work, mmcif_timeout_work);

	sh_mmcif_writel(host->addr, MMCIF_CE_INT_MASK, MASK_ALL);

	ret = request_threaded_irq(irq[0], sh_mmcif_intr, sh_mmcif_irqt, 0, "sh_mmc:error", host);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret) {
		dev_err(&pdev->dev, "request_irq error (sh_mmc:error)\n");
		goto clean_up3;
	}
<<<<<<< HEAD
	ret = request_irq(irq[1], sh_mmcif_intr, 0, "sh_mmc:int", host);
	if (ret) {
		free_irq(irq[0], host);
		dev_err(&pdev->dev, "request_irq error (sh_mmc:int)\n");
		goto clean_up3;
	}

	sh_mmcif_detect(host->mmc);
=======
	ret = request_threaded_irq(irq[1], sh_mmcif_intr, sh_mmcif_irqt, 0, "sh_mmc:int", host);
	if (ret) {
		dev_err(&pdev->dev, "request_irq error (sh_mmc:int)\n");
		goto clean_up4;
	}

	ret = mmc_add_host(mmc);
	if (ret < 0)
		goto clean_up5;

	dev_pm_qos_expose_latency_limit(&pdev->dev, 100);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	host->hclk = clk_get(&pdev->dev, NULL);
	if (IS_ERR(host->hclk)) {
		ret = PTR_ERR(host->hclk);
		dev_err(&pdev->dev, "cannot get clock: %d\n", ret);
		goto eclkget;
	}
	ret = sh_mmcif_clk_update(host);
	if (ret < 0)
		goto eclkupdate;

	ret = pm_runtime_resume(&pdev->dev);
	if (ret < 0)
		goto eresume;

	INIT_DELAYED_WORK(&host->timeout_work, mmcif_timeout_work);

	sh_mmcif_sync_reset(host);
	sh_mmcif_writel(host->addr, MMCIF_CE_INT_MASK, MASK_ALL);

	name = irq[1] < 0 ? dev_name(&pdev->dev) : "sh_mmc:error";
	ret = request_threaded_irq(irq[0], sh_mmcif_intr, sh_mmcif_irqt, 0, name, host);
	if (ret) {
		dev_err(&pdev->dev, "request_irq error (%s)\n", name);
		goto ereqirq0;
	}
	if (irq[1] >= 0) {
		ret = request_threaded_irq(irq[1], sh_mmcif_intr, sh_mmcif_irqt,
					   0, "sh_mmc:int", host);
		if (ret) {
			dev_err(&pdev->dev, "request_irq error (sh_mmc:int)\n");
			goto ereqirq1;
		}
	}

	if (pd && pd->use_cd_gpio) {
		ret = mmc_gpio_request_cd(mmc, pd->cd_gpio, 0);
		if (ret < 0)
			goto erqcd;
	}

	mutex_init(&host->thread_lock);

	clk_disable_unprepare(host->hclk);
	ret = mmc_add_host(mmc);
	if (ret < 0)
		goto emmcaddh;

	dev_pm_qos_expose_latency_limit(&pdev->dev, 100);
>>>>>>> refs/remotes/origin/master

	dev_info(&pdev->dev, "driver version %s\n", DRIVER_VERSION);
	dev_dbg(&pdev->dev, "chip ver H'%04x\n",
		sh_mmcif_readl(host->addr, MMCIF_CE_VERSION) & 0x0000ffff);
	return ret;

<<<<<<< HEAD
<<<<<<< HEAD
clean_up3:
	mmc_remove_host(mmc);
=======
clean_up5:
	free_irq(irq[1], host);
clean_up4:
	free_irq(irq[0], host);
clean_up3:
>>>>>>> refs/remotes/origin/cm-10.0
	pm_runtime_suspend(&pdev->dev);
clean_up2:
	pm_runtime_disable(&pdev->dev);
	clk_disable(host->hclk);
clean_up1:
	mmc_free_host(mmc);
clean_up:
	if (reg)
		iounmap(reg);
	return ret;
}

static int __devexit sh_mmcif_remove(struct platform_device *pdev)
=======
emmcaddh:
erqcd:
	if (irq[1] >= 0)
		free_irq(irq[1], host);
ereqirq1:
	free_irq(irq[0], host);
ereqirq0:
	pm_runtime_suspend(&pdev->dev);
eresume:
	clk_disable_unprepare(host->hclk);
eclkupdate:
	clk_put(host->hclk);
eclkget:
	pm_runtime_disable(&pdev->dev);
eofparse:
	mmc_free_host(mmc);
ealloch:
	iounmap(reg);
	return ret;
}

static int sh_mmcif_remove(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	struct sh_mmcif_host *host = platform_get_drvdata(pdev);
	int irq[2];

<<<<<<< HEAD
<<<<<<< HEAD
	pm_runtime_get_sync(&pdev->dev);

	mmc_remove_host(host->mmc);
	sh_mmcif_writel(host->addr, MMCIF_CE_INT_MASK, MASK_ALL);

=======
	host->dying = true;
=======
	host->dying = true;
	clk_prepare_enable(host->hclk);
>>>>>>> refs/remotes/origin/master
	pm_runtime_get_sync(&pdev->dev);

	dev_pm_qos_hide_latency_limit(&pdev->dev);

	mmc_remove_host(host->mmc);
	sh_mmcif_writel(host->addr, MMCIF_CE_INT_MASK, MASK_ALL);

	/*
	 * FIXME: cancel_delayed_work(_sync)() and free_irq() race with the
	 * mmc_remove_host() call above. But swapping order doesn't help either
	 * (a query on the linux-mmc mailing list didn't bring any replies).
	 */
	cancel_delayed_work_sync(&host->timeout_work);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (host->addr)
		iounmap(host->addr);

	irq[0] = platform_get_irq(pdev, 0);
	irq[1] = platform_get_irq(pdev, 1);

	free_irq(irq[0], host);
<<<<<<< HEAD
	free_irq(irq[1], host);

	platform_set_drvdata(pdev, NULL);

	clk_disable(host->hclk);
=======
	if (irq[1] >= 0)
		free_irq(irq[1], host);

	clk_disable_unprepare(host->hclk);
>>>>>>> refs/remotes/origin/master
	mmc_free_host(host->mmc);
	pm_runtime_put_sync(&pdev->dev);
	pm_runtime_disable(&pdev->dev);

	return 0;
}

<<<<<<< HEAD
#ifdef CONFIG_PM
static int sh_mmcif_suspend(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct sh_mmcif_host *host = platform_get_drvdata(pdev);
	int ret = mmc_suspend_host(host->mmc);

	if (!ret) {
		sh_mmcif_writel(host->addr, MMCIF_CE_INT_MASK, MASK_ALL);
		clk_disable(host->hclk);
	}

	return ret;
=======
#ifdef CONFIG_PM_SLEEP
static int sh_mmcif_suspend(struct device *dev)
{
	struct sh_mmcif_host *host = dev_get_drvdata(dev);

	sh_mmcif_writel(host->addr, MMCIF_CE_INT_MASK, MASK_ALL);

	return 0;
>>>>>>> refs/remotes/origin/master
}

static int sh_mmcif_resume(struct device *dev)
{
<<<<<<< HEAD
	struct platform_device *pdev = to_platform_device(dev);
	struct sh_mmcif_host *host = platform_get_drvdata(pdev);

	clk_enable(host->hclk);

	return mmc_resume_host(host->mmc);
}
#else
#define sh_mmcif_suspend	NULL
#define sh_mmcif_resume		NULL
#endif	/* CONFIG_PM */

static const struct dev_pm_ops sh_mmcif_dev_pm_ops = {
	.suspend = sh_mmcif_suspend,
	.resume = sh_mmcif_resume,
=======
	return 0;
}
#endif

static const struct of_device_id mmcif_of_match[] = {
	{ .compatible = "renesas,sh-mmcif" },
	{ }
};
MODULE_DEVICE_TABLE(of, mmcif_of_match);

static const struct dev_pm_ops sh_mmcif_dev_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(sh_mmcif_suspend, sh_mmcif_resume)
>>>>>>> refs/remotes/origin/master
};

static struct platform_driver sh_mmcif_driver = {
	.probe		= sh_mmcif_probe,
	.remove		= sh_mmcif_remove,
	.driver		= {
		.name	= DRIVER_NAME,
		.pm	= &sh_mmcif_dev_pm_ops,
<<<<<<< HEAD
	},
};

<<<<<<< HEAD
static int __init sh_mmcif_init(void)
{
	return platform_driver_register(&sh_mmcif_driver);
}

static void __exit sh_mmcif_exit(void)
{
	platform_driver_unregister(&sh_mmcif_driver);
}

module_init(sh_mmcif_init);
module_exit(sh_mmcif_exit);

=======
module_platform_driver(sh_mmcif_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		.owner	= THIS_MODULE,
		.of_match_table = mmcif_of_match,
	},
};

module_platform_driver(sh_mmcif_driver);
>>>>>>> refs/remotes/origin/master

MODULE_DESCRIPTION("SuperH on-chip MMC/eMMC interface driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:" DRIVER_NAME);
MODULE_AUTHOR("Yusuke Goda <yusuke.goda.sx@renesas.com>");
