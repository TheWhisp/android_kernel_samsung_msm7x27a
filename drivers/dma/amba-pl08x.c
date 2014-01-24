/*
 * Copyright (c) 2006 ARM Ltd.
 * Copyright (c) 2010 ST-Ericsson SA
 *
 * Author: Peter Pearse <peter.pearse@arm.com>
 * Author: Linus Walleij <linus.walleij@stericsson.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59
 * Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * The full GNU General Public License is in this distribution in the file
 * called COPYING.
 *
 * Documentation: ARM DDI 0196G == PL080
 * Documentation: ARM DDI 0218E == PL081
<<<<<<< HEAD
=======
 * Documentation: S3C6410 User's Manual == PL080S
>>>>>>> refs/remotes/origin/master
 *
 * PL080 & PL081 both have 16 sets of DMA signals that can be routed to any
 * channel.
 *
 * The PL080 has 8 channels available for simultaneous use, and the PL081
 * has only two channels. So on these DMA controllers the number of channels
 * and the number of incoming DMA signals are two totally different things.
 * It is usually not possible to theoretically handle all physical signals,
 * so a multiplexing scheme with possible denial of use is necessary.
 *
 * The PL080 has a dual bus master, PL081 has a single master.
 *
<<<<<<< HEAD
=======
 * PL080S is a version modified by Samsung and used in S3C64xx SoCs.
 * It differs in following aspects:
 * - CH_CONFIG register at different offset,
 * - separate CH_CONTROL2 register for transfer size,
 * - bigger maximum transfer size,
 * - 8-word aligned LLI, instead of 4-word, due to extra CCTL2 word,
 * - no support for peripheral flow control.
 *
>>>>>>> refs/remotes/origin/master
 * Memory to peripheral transfer may be visualized as
 *	Get data from memory to DMAC
 *	Until no data left
 *		On burst request from peripheral
 *			Destination burst from DMAC to peripheral
 *			Clear burst request
 *	Raise terminal count interrupt
 *
 * For peripherals with a FIFO:
 * Source      burst size == half the depth of the peripheral FIFO
 * Destination burst size == the depth of the peripheral FIFO
 *
 * (Bursts are irrelevant for mem to mem transfers - there are no burst
 * signals, the DMA controller will simply facilitate its AHB master.)
 *
 * ASSUMES default (little) endianness for DMA transfers
 *
 * The PL08x has two flow control settings:
 *  - DMAC flow control: the transfer size defines the number of transfers
 *    which occur for the current LLI entry, and the DMAC raises TC at the
 *    end of every LLI entry.  Observed behaviour shows the DMAC listening
 *    to both the BREQ and SREQ signals (contrary to documented),
 *    transferring data if either is active.  The LBREQ and LSREQ signals
 *    are ignored.
 *
 *  - Peripheral flow control: the transfer size is ignored (and should be
 *    zero).  The data is transferred from the current LLI entry, until
 *    after the final transfer signalled by LBREQ or LSREQ.  The DMAC
<<<<<<< HEAD
 *    will then move to the next LLI entry.
 *
<<<<<<< HEAD
 * Only the former works sanely with scatter lists, so we only implement
 * the DMAC flow control method.  However, peripherals which use the LBREQ
 * and LSREQ signals (eg, MMCI) are unable to use this mode, which through
 * these hardware restrictions prevents them from using scatter DMA.
 *
 * Global TODO:
 * - Break out common code from arch/arm/mach-s3c64xx and share
 */
#include <linux/device.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/dmapool.h>
#include <linux/dmaengine.h>
#include <linux/amba/bus.h>
#include <linux/amba/pl08x.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>

#include <asm/hardware/pl080.h>

#define DRIVER_NAME	"pl08xdmac"

=======
 * Global TODO:
 * - Break out common code from arch/arm/mach-s3c64xx and share
=======
 *    will then move to the next LLI entry. Unsupported by PL080S.
>>>>>>> refs/remotes/origin/master
 */
#include <linux/amba/bus.h>
#include <linux/amba/pl08x.h>
#include <linux/debugfs.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/dmaengine.h>
#include <linux/dmapool.h>
#include <linux/dma-mapping.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/pm_runtime.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <asm/hardware/pl080.h>

#include "dmaengine.h"
=======
#include <linux/amba/pl080.h>

#include "dmaengine.h"
#include "virt-dma.h"
>>>>>>> refs/remotes/origin/master

#define DRIVER_NAME	"pl08xdmac"

static struct amba_driver pl08x_amba_driver;
<<<<<<< HEAD

>>>>>>> refs/remotes/origin/cm-10.0
=======
struct pl08x_driver_data;

>>>>>>> refs/remotes/origin/master
/**
 * struct vendor_data - vendor-specific config parameters for PL08x derivatives
 * @channels: the number of channels available in this variant
 * @dualmaster: whether this version supports dual AHB masters or not.
<<<<<<< HEAD
 */
struct vendor_data {
	u8 channels;
	bool dualmaster;
};

/*
 * PL08X private data structures
 * An LLI struct - see PL08x TRM.  Note that next uses bit[0] as a bus bit,
 * start & end do not - their bus bit info is in cctl.  Also note that these
 * are fixed 32-bit quantities.
 */
struct pl08x_lli {
	u32 src;
	u32 dst;
	u32 lli;
	u32 cctl;
=======
 * @nomadik: whether the channels have Nomadik security extension bits
 *	that need to be checked for permission before use and some registers are
 *	missing
 * @pl080s: whether this version is a PL080S, which has separate register and
 *	LLI word for transfer size.
 */
struct vendor_data {
	u8 config_offset;
	u8 channels;
	bool dualmaster;
	bool nomadik;
	bool pl080s;
	u32 max_transfer_size;
};

/**
 * struct pl08x_bus_data - information of source or destination
 * busses for a transfer
 * @addr: current address
 * @maxwidth: the maximum width of a transfer on this bus
 * @buswidth: the width of this bus in bytes: 1, 2 or 4
 */
struct pl08x_bus_data {
	dma_addr_t addr;
	u8 maxwidth;
	u8 buswidth;
};

#define IS_BUS_ALIGNED(bus) IS_ALIGNED((bus)->addr, (bus)->buswidth)

/**
 * struct pl08x_phy_chan - holder for the physical channels
 * @id: physical index to this channel
 * @lock: a lock to use when altering an instance of this struct
 * @serving: the virtual channel currently being served by this physical
 * channel
 * @locked: channel unavailable for the system, e.g. dedicated to secure
 * world
 */
struct pl08x_phy_chan {
	unsigned int id;
	void __iomem *base;
	void __iomem *reg_config;
	spinlock_t lock;
	struct pl08x_dma_chan *serving;
	bool locked;
};

/**
 * struct pl08x_sg - structure containing data per sg
 * @src_addr: src address of sg
 * @dst_addr: dst address of sg
 * @len: transfer len in bytes
 * @node: node for txd's dsg_list
 */
struct pl08x_sg {
	dma_addr_t src_addr;
	dma_addr_t dst_addr;
	size_t len;
	struct list_head node;
};

/**
 * struct pl08x_txd - wrapper for struct dma_async_tx_descriptor
 * @vd: virtual DMA descriptor
 * @dsg_list: list of children sg's
 * @llis_bus: DMA memory address (physical) start for the LLIs
 * @llis_va: virtual memory address start for the LLIs
 * @cctl: control reg values for current txd
 * @ccfg: config reg values for current txd
 * @done: this marks completed descriptors, which should not have their
 *   mux released.
 * @cyclic: indicate cyclic transfers
 */
struct pl08x_txd {
	struct virt_dma_desc vd;
	struct list_head dsg_list;
	dma_addr_t llis_bus;
	u32 *llis_va;
	/* Default cctl value for LLIs */
	u32 cctl;
	/*
	 * Settings to be put into the physical channel when we
	 * trigger this txd.  Other registers are in llis_va[0].
	 */
	u32 ccfg;
	bool done;
	bool cyclic;
};

/**
 * struct pl08x_dma_chan_state - holds the PL08x specific virtual channel
 * states
 * @PL08X_CHAN_IDLE: the channel is idle
 * @PL08X_CHAN_RUNNING: the channel has allocated a physical transport
 * channel and is running a transfer on it
 * @PL08X_CHAN_PAUSED: the channel has allocated a physical transport
 * channel, but the transfer is currently paused
 * @PL08X_CHAN_WAITING: the channel is waiting for a physical transport
 * channel to become available (only pertains to memcpy channels)
 */
enum pl08x_dma_chan_state {
	PL08X_CHAN_IDLE,
	PL08X_CHAN_RUNNING,
	PL08X_CHAN_PAUSED,
	PL08X_CHAN_WAITING,
};

/**
 * struct pl08x_dma_chan - this structure wraps a DMA ENGINE channel
 * @vc: wrappped virtual channel
 * @phychan: the physical channel utilized by this channel, if there is one
 * @name: name of channel
 * @cd: channel platform data
 * @runtime_addr: address for RX/TX according to the runtime config
 * @at: active transaction on this channel
 * @lock: a lock for this channel data
 * @host: a pointer to the host (internal use)
 * @state: whether the channel is idle, paused, running etc
 * @slave: whether this channel is a device (slave) or for memcpy
 * @signal: the physical DMA request signal which this channel is using
 * @mux_use: count of descriptors using this DMA request signal setting
 */
struct pl08x_dma_chan {
	struct virt_dma_chan vc;
	struct pl08x_phy_chan *phychan;
	const char *name;
	const struct pl08x_channel_data *cd;
	struct dma_slave_config cfg;
	struct pl08x_txd *at;
	struct pl08x_driver_data *host;
	enum pl08x_dma_chan_state state;
	bool slave;
	int signal;
	unsigned mux_use;
>>>>>>> refs/remotes/origin/master
};

/**
 * struct pl08x_driver_data - the local state holder for the PL08x
 * @slave: slave engine for this instance
 * @memcpy: memcpy engine for this instance
 * @base: virtual memory base (remapped) for the PL08x
 * @adev: the corresponding AMBA (PrimeCell) bus entry
 * @vd: vendor data for this PL08x variant
 * @pd: platform data passed in from the platform/machine
 * @phy_chans: array of data for the physical channels
 * @pool: a pool for the LLI descriptors
<<<<<<< HEAD
 * @pool_ctr: counter of LLIs in the pool
<<<<<<< HEAD
 * @lli_buses: bitmask to or in to LLI pointer selecting AHB port for LLI fetches
=======
 * @lli_buses: bitmask to or in to LLI pointer selecting AHB port for LLI
 * fetches
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * @lli_buses: bitmask to or in to LLI pointer selecting AHB port for LLI
 * fetches
>>>>>>> refs/remotes/origin/master
 * @mem_buses: set to indicate memory transfers on AHB2.
 * @lock: a spinlock for this struct
 */
struct pl08x_driver_data {
	struct dma_device slave;
	struct dma_device memcpy;
	void __iomem *base;
	struct amba_device *adev;
	const struct vendor_data *vd;
	struct pl08x_platform_data *pd;
	struct pl08x_phy_chan *phy_chans;
	struct dma_pool *pool;
<<<<<<< HEAD
	int pool_ctr;
	u8 lli_buses;
	u8 mem_buses;
	spinlock_t lock;
=======
	u8 lli_buses;
	u8 mem_buses;
	u8 lli_words;
>>>>>>> refs/remotes/origin/master
};

/*
 * PL08X specific defines
 */

<<<<<<< HEAD
<<<<<<< HEAD
/*
 * Memory boundaries: the manual for PL08x says that the controller
 * cannot read past a 1KiB boundary, so these defines are used to
 * create transfer LLIs that do not cross such boundaries.
 */
#define PL08X_BOUNDARY_SHIFT		(10)	/* 1KB 0x400 */
#define PL08X_BOUNDARY_SIZE		(1 << PL08X_BOUNDARY_SHIFT)

/* Minimum period between work queue runs */
#define PL08X_WQ_PERIODMIN	20

=======
>>>>>>> refs/remotes/origin/cm-10.0
/* Size (bytes) of each LLI buffer allocated for one transfer */
# define PL08X_LLI_TSFR_SIZE	0x2000

/* Maximum times we call dma_pool_alloc on this pool without freeing */
<<<<<<< HEAD
#define PL08X_MAX_ALLOCS	0x40
=======
>>>>>>> refs/remotes/origin/cm-10.0
#define MAX_NUM_TSFR_LLIS	(PL08X_LLI_TSFR_SIZE/sizeof(struct pl08x_lli))
=======
/* The order of words in an LLI. */
#define PL080_LLI_SRC		0
#define PL080_LLI_DST		1
#define PL080_LLI_LLI		2
#define PL080_LLI_CCTL		3
#define PL080S_LLI_CCTL2	4

/* Total words in an LLI. */
#define PL080_LLI_WORDS		4
#define PL080S_LLI_WORDS	8

/*
 * Number of LLIs in each LLI buffer allocated for one transfer
 * (maximum times we call dma_pool_alloc on this pool without freeing)
 */
#define MAX_NUM_TSFR_LLIS	512
>>>>>>> refs/remotes/origin/master
#define PL08X_ALIGN		8

static inline struct pl08x_dma_chan *to_pl08x_chan(struct dma_chan *chan)
{
<<<<<<< HEAD
	return container_of(chan, struct pl08x_dma_chan, chan);
=======
	return container_of(chan, struct pl08x_dma_chan, vc.chan);
>>>>>>> refs/remotes/origin/master
}

static inline struct pl08x_txd *to_pl08x_txd(struct dma_async_tx_descriptor *tx)
{
<<<<<<< HEAD
	return container_of(tx, struct pl08x_txd, tx);
=======
	return container_of(tx, struct pl08x_txd, vd.tx);
}

/*
 * Mux handling.
 *
 * This gives us the DMA request input to the PL08x primecell which the
 * peripheral described by the channel data will be routed to, possibly
 * via a board/SoC specific external MUX.  One important point to note
 * here is that this does not depend on the physical channel.
 */
static int pl08x_request_mux(struct pl08x_dma_chan *plchan)
{
	const struct pl08x_platform_data *pd = plchan->host->pd;
	int ret;

	if (plchan->mux_use++ == 0 && pd->get_xfer_signal) {
		ret = pd->get_xfer_signal(plchan->cd);
		if (ret < 0) {
			plchan->mux_use = 0;
			return ret;
		}

		plchan->signal = ret;
	}
	return 0;
}

static void pl08x_release_mux(struct pl08x_dma_chan *plchan)
{
	const struct pl08x_platform_data *pd = plchan->host->pd;

	if (plchan->signal >= 0) {
		WARN_ON(plchan->mux_use == 0);

		if (--plchan->mux_use == 0 && pd->put_xfer_signal) {
			pd->put_xfer_signal(plchan->cd, plchan->signal);
			plchan->signal = -1;
		}
	}
>>>>>>> refs/remotes/origin/master
}

/*
 * Physical channel handling
 */

/* Whether a certain channel is busy or not */
static int pl08x_phy_channel_busy(struct pl08x_phy_chan *ch)
{
	unsigned int val;

<<<<<<< HEAD
	val = readl(ch->base + PL080_CH_CONFIG);
	return val & PL080_CONFIG_ACTIVE;
}

=======
	val = readl(ch->reg_config);
	return val & PL080_CONFIG_ACTIVE;
}

static void pl08x_write_lli(struct pl08x_driver_data *pl08x,
		struct pl08x_phy_chan *phychan, const u32 *lli, u32 ccfg)
{
	if (pl08x->vd->pl080s)
		dev_vdbg(&pl08x->adev->dev,
			"WRITE channel %d: csrc=0x%08x, cdst=0x%08x, "
			"clli=0x%08x, cctl=0x%08x, cctl2=0x%08x, ccfg=0x%08x\n",
			phychan->id, lli[PL080_LLI_SRC], lli[PL080_LLI_DST],
			lli[PL080_LLI_LLI], lli[PL080_LLI_CCTL],
			lli[PL080S_LLI_CCTL2], ccfg);
	else
		dev_vdbg(&pl08x->adev->dev,
			"WRITE channel %d: csrc=0x%08x, cdst=0x%08x, "
			"clli=0x%08x, cctl=0x%08x, ccfg=0x%08x\n",
			phychan->id, lli[PL080_LLI_SRC], lli[PL080_LLI_DST],
			lli[PL080_LLI_LLI], lli[PL080_LLI_CCTL], ccfg);

	writel_relaxed(lli[PL080_LLI_SRC], phychan->base + PL080_CH_SRC_ADDR);
	writel_relaxed(lli[PL080_LLI_DST], phychan->base + PL080_CH_DST_ADDR);
	writel_relaxed(lli[PL080_LLI_LLI], phychan->base + PL080_CH_LLI);
	writel_relaxed(lli[PL080_LLI_CCTL], phychan->base + PL080_CH_CONTROL);

	if (pl08x->vd->pl080s)
		writel_relaxed(lli[PL080S_LLI_CCTL2],
				phychan->base + PL080S_CH_CONTROL2);

	writel(ccfg, phychan->reg_config);
}

>>>>>>> refs/remotes/origin/master
/*
 * Set the initial DMA register values i.e. those for the first LLI
 * The next LLI pointer and the configuration interrupt bit have
 * been set when the LLIs were constructed.  Poke them into the hardware
 * and start the transfer.
 */
<<<<<<< HEAD
static void pl08x_start_txd(struct pl08x_dma_chan *plchan,
	struct pl08x_txd *txd)
{
	struct pl08x_driver_data *pl08x = plchan->host;
	struct pl08x_phy_chan *phychan = plchan->phychan;
	struct pl08x_lli *lli = &txd->llis_va[0];
	u32 val;

=======
static void pl08x_start_next_txd(struct pl08x_dma_chan *plchan)
{
	struct pl08x_driver_data *pl08x = plchan->host;
	struct pl08x_phy_chan *phychan = plchan->phychan;
	struct virt_dma_desc *vd = vchan_next_desc(&plchan->vc);
	struct pl08x_txd *txd = to_pl08x_txd(&vd->tx);
	u32 val;

	list_del(&txd->vd.node);

>>>>>>> refs/remotes/origin/master
	plchan->at = txd;

	/* Wait for channel inactive */
	while (pl08x_phy_channel_busy(phychan))
		cpu_relax();

<<<<<<< HEAD
	dev_vdbg(&pl08x->adev->dev,
		"WRITE channel %d: csrc=0x%08x, cdst=0x%08x, "
		"clli=0x%08x, cctl=0x%08x, ccfg=0x%08x\n",
		phychan->id, lli->src, lli->dst, lli->lli, lli->cctl,
		txd->ccfg);

	writel(lli->src, phychan->base + PL080_CH_SRC_ADDR);
	writel(lli->dst, phychan->base + PL080_CH_DST_ADDR);
	writel(lli->lli, phychan->base + PL080_CH_LLI);
	writel(lli->cctl, phychan->base + PL080_CH_CONTROL);
	writel(txd->ccfg, phychan->base + PL080_CH_CONFIG);
=======
	pl08x_write_lli(pl08x, phychan, &txd->llis_va[0], txd->ccfg);
>>>>>>> refs/remotes/origin/master

	/* Enable the DMA channel */
	/* Do not access config register until channel shows as disabled */
	while (readl(pl08x->base + PL080_EN_CHAN) & (1 << phychan->id))
		cpu_relax();

	/* Do not access config register until channel shows as inactive */
<<<<<<< HEAD
	val = readl(phychan->base + PL080_CH_CONFIG);
	while ((val & PL080_CONFIG_ACTIVE) || (val & PL080_CONFIG_ENABLE))
		val = readl(phychan->base + PL080_CH_CONFIG);

	writel(val | PL080_CONFIG_ENABLE, phychan->base + PL080_CH_CONFIG);
=======
	val = readl(phychan->reg_config);
	while ((val & PL080_CONFIG_ACTIVE) || (val & PL080_CONFIG_ENABLE))
		val = readl(phychan->reg_config);

	writel(val | PL080_CONFIG_ENABLE, phychan->reg_config);
>>>>>>> refs/remotes/origin/master
}

/*
 * Pause the channel by setting the HALT bit.
 *
 * For M->P transfers, pause the DMAC first and then stop the peripheral -
 * the FIFO can only drain if the peripheral is still requesting data.
 * (note: this can still timeout if the DMAC FIFO never drains of data.)
 *
 * For P->M transfers, disable the peripheral first to stop it filling
 * the DMAC FIFO, and then pause the DMAC.
 */
static void pl08x_pause_phy_chan(struct pl08x_phy_chan *ch)
{
	u32 val;
	int timeout;

	/* Set the HALT bit and wait for the FIFO to drain */
<<<<<<< HEAD
	val = readl(ch->base + PL080_CH_CONFIG);
	val |= PL080_CONFIG_HALT;
	writel(val, ch->base + PL080_CH_CONFIG);
=======
	val = readl(ch->reg_config);
	val |= PL080_CONFIG_HALT;
	writel(val, ch->reg_config);
>>>>>>> refs/remotes/origin/master

	/* Wait for channel inactive */
	for (timeout = 1000; timeout; timeout--) {
		if (!pl08x_phy_channel_busy(ch))
			break;
		udelay(1);
	}
	if (pl08x_phy_channel_busy(ch))
		pr_err("pl08x: channel%u timeout waiting for pause\n", ch->id);
}

static void pl08x_resume_phy_chan(struct pl08x_phy_chan *ch)
{
	u32 val;

	/* Clear the HALT bit */
<<<<<<< HEAD
	val = readl(ch->base + PL080_CH_CONFIG);
	val &= ~PL080_CONFIG_HALT;
	writel(val, ch->base + PL080_CH_CONFIG);
}

<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	val = readl(ch->reg_config);
	val &= ~PL080_CONFIG_HALT;
	writel(val, ch->reg_config);
}

>>>>>>> refs/remotes/origin/master
/*
 * pl08x_terminate_phy_chan() stops the channel, clears the FIFO and
 * clears any pending interrupt status.  This should not be used for
 * an on-going transfer, but as a method of shutting down a channel
 * (eg, when it's no longer used) or terminating a transfer.
 */
static void pl08x_terminate_phy_chan(struct pl08x_driver_data *pl08x,
	struct pl08x_phy_chan *ch)
{
<<<<<<< HEAD
	u32 val = readl(ch->base + PL080_CH_CONFIG);
=======
	u32 val = readl(ch->reg_config);
>>>>>>> refs/remotes/origin/master

	val &= ~(PL080_CONFIG_ENABLE | PL080_CONFIG_ERR_IRQ_MASK |
	         PL080_CONFIG_TC_IRQ_MASK);

<<<<<<< HEAD
	writel(val, ch->base + PL080_CH_CONFIG);
=======
	writel(val, ch->reg_config);
>>>>>>> refs/remotes/origin/master

	writel(1 << ch->id, pl08x->base + PL080_ERR_CLEAR);
	writel(1 << ch->id, pl08x->base + PL080_TC_CLEAR);
}

static inline u32 get_bytes_in_cctl(u32 cctl)
{
	/* The source width defines the number of bytes */
	u32 bytes = cctl & PL080_CONTROL_TRANSFER_SIZE_MASK;

<<<<<<< HEAD
=======
	cctl &= PL080_CONTROL_SWIDTH_MASK;

	switch (cctl >> PL080_CONTROL_SWIDTH_SHIFT) {
	case PL080_WIDTH_8BIT:
		break;
	case PL080_WIDTH_16BIT:
		bytes *= 2;
		break;
	case PL080_WIDTH_32BIT:
		bytes *= 4;
		break;
	}
	return bytes;
}

static inline u32 get_bytes_in_cctl_pl080s(u32 cctl, u32 cctl1)
{
	/* The source width defines the number of bytes */
	u32 bytes = cctl1 & PL080S_CONTROL_TRANSFER_SIZE_MASK;

	cctl &= PL080_CONTROL_SWIDTH_MASK;

>>>>>>> refs/remotes/origin/master
	switch (cctl >> PL080_CONTROL_SWIDTH_SHIFT) {
	case PL080_WIDTH_8BIT:
		break;
	case PL080_WIDTH_16BIT:
		bytes *= 2;
		break;
	case PL080_WIDTH_32BIT:
		bytes *= 4;
		break;
	}
	return bytes;
}

/* The channel should be paused when calling this */
static u32 pl08x_getbytes_chan(struct pl08x_dma_chan *plchan)
{
<<<<<<< HEAD
	struct pl08x_phy_chan *ch;
	struct pl08x_txd *txd;
	unsigned long flags;
	size_t bytes = 0;

	spin_lock_irqsave(&plchan->lock, flags);
	ch = plchan->phychan;
	txd = plchan->at;

=======
	struct pl08x_driver_data *pl08x = plchan->host;
	const u32 *llis_va, *llis_va_limit;
	struct pl08x_phy_chan *ch;
	dma_addr_t llis_bus;
	struct pl08x_txd *txd;
	u32 llis_max_words;
	size_t bytes;
	u32 clli;

	ch = plchan->phychan;
	txd = plchan->at;

	if (!ch || !txd)
		return 0;

>>>>>>> refs/remotes/origin/master
	/*
	 * Follow the LLIs to get the number of remaining
	 * bytes in the currently active transaction.
	 */
<<<<<<< HEAD
	if (ch && txd) {
		u32 clli = readl(ch->base + PL080_CH_LLI) & ~PL080_LLI_LM_AHB2;

		/* First get the remaining bytes in the active transfer */
		bytes = get_bytes_in_cctl(readl(ch->base + PL080_CH_CONTROL));

		if (clli) {
			struct pl08x_lli *llis_va = txd->llis_va;
			dma_addr_t llis_bus = txd->llis_bus;
			int index;

			BUG_ON(clli < llis_bus || clli >= llis_bus +
				sizeof(struct pl08x_lli) * MAX_NUM_TSFR_LLIS);

			/*
			 * Locate the next LLI - as this is an array,
			 * it's simple maths to find.
			 */
			index = (clli - llis_bus) / sizeof(struct pl08x_lli);

			for (; index < MAX_NUM_TSFR_LLIS; index++) {
				bytes += get_bytes_in_cctl(llis_va[index].cctl);

				/*
				 * A LLI pointer of 0 terminates the LLI list
				 */
				if (!llis_va[index].lli)
					break;
			}
		}
	}

	/* Sum up all queued transactions */
	if (!list_empty(&plchan->pend_list)) {
		struct pl08x_txd *txdi;
		list_for_each_entry(txdi, &plchan->pend_list, node) {
<<<<<<< HEAD
			bytes += txdi->len;
=======
			struct pl08x_sg *dsg;
			list_for_each_entry(dsg, &txd->dsg_list, node)
				bytes += dsg->len;
>>>>>>> refs/remotes/origin/cm-10.0
		}
	}

	spin_unlock_irqrestore(&plchan->lock, flags);
=======
	clli = readl(ch->base + PL080_CH_LLI) & ~PL080_LLI_LM_AHB2;

	/* First get the remaining bytes in the active transfer */
	if (pl08x->vd->pl080s)
		bytes = get_bytes_in_cctl_pl080s(
				readl(ch->base + PL080_CH_CONTROL),
				readl(ch->base + PL080S_CH_CONTROL2));
	else
		bytes = get_bytes_in_cctl(readl(ch->base + PL080_CH_CONTROL));

	if (!clli)
		return bytes;

	llis_va = txd->llis_va;
	llis_bus = txd->llis_bus;

	llis_max_words = pl08x->lli_words * MAX_NUM_TSFR_LLIS;
	BUG_ON(clli < llis_bus || clli >= llis_bus +
						sizeof(u32) * llis_max_words);

	/*
	 * Locate the next LLI - as this is an array,
	 * it's simple maths to find.
	 */
	llis_va += (clli - llis_bus) / sizeof(u32);

	llis_va_limit = llis_va + llis_max_words;

	for (; llis_va < llis_va_limit; llis_va += pl08x->lli_words) {
		if (pl08x->vd->pl080s)
			bytes += get_bytes_in_cctl_pl080s(
						llis_va[PL080_LLI_CCTL],
						llis_va[PL080S_LLI_CCTL2]);
		else
			bytes += get_bytes_in_cctl(llis_va[PL080_LLI_CCTL]);

		/*
		 * A LLI pointer going backward terminates the LLI list
		 */
		if (llis_va[PL080_LLI_LLI] <= clli)
			break;
	}
>>>>>>> refs/remotes/origin/master

	return bytes;
}

/*
 * Allocate a physical channel for a virtual channel
 *
 * Try to locate a physical channel to be used for this transfer. If all
 * are taken return NULL and the requester will have to cope by using
 * some fallback PIO mode or retrying later.
 */
static struct pl08x_phy_chan *
pl08x_get_phy_channel(struct pl08x_driver_data *pl08x,
		      struct pl08x_dma_chan *virt_chan)
{
	struct pl08x_phy_chan *ch = NULL;
	unsigned long flags;
	int i;

	for (i = 0; i < pl08x->vd->channels; i++) {
		ch = &pl08x->phy_chans[i];

		spin_lock_irqsave(&ch->lock, flags);

<<<<<<< HEAD
		if (!ch->serving) {
			ch->serving = virt_chan;
			ch->signal = -1;
=======
		if (!ch->locked && !ch->serving) {
			ch->serving = virt_chan;
>>>>>>> refs/remotes/origin/master
			spin_unlock_irqrestore(&ch->lock, flags);
			break;
		}

		spin_unlock_irqrestore(&ch->lock, flags);
	}

	if (i == pl08x->vd->channels) {
		/* No physical channel available, cope with it */
		return NULL;
	}

<<<<<<< HEAD
<<<<<<< HEAD
=======
	pm_runtime_get_sync(&pl08x->adev->dev);
>>>>>>> refs/remotes/origin/cm-10.0
	return ch;
}

static inline void pl08x_put_phy_channel(struct pl08x_driver_data *pl08x,
					 struct pl08x_phy_chan *ch)
{
	unsigned long flags;

	spin_lock_irqsave(&ch->lock, flags);

	/* Stop the channel and clear its interrupts */
	pl08x_terminate_phy_chan(pl08x, ch);

<<<<<<< HEAD
=======
	pm_runtime_put(&pl08x->adev->dev);

>>>>>>> refs/remotes/origin/cm-10.0
	/* Mark it as free */
	ch->serving = NULL;
	spin_unlock_irqrestore(&ch->lock, flags);
=======
	return ch;
}

/* Mark the physical channel as free.  Note, this write is atomic. */
static inline void pl08x_put_phy_channel(struct pl08x_driver_data *pl08x,
					 struct pl08x_phy_chan *ch)
{
	ch->serving = NULL;
}

/*
 * Try to allocate a physical channel.  When successful, assign it to
 * this virtual channel, and initiate the next descriptor.  The
 * virtual channel lock must be held at this point.
 */
static void pl08x_phy_alloc_and_start(struct pl08x_dma_chan *plchan)
{
	struct pl08x_driver_data *pl08x = plchan->host;
	struct pl08x_phy_chan *ch;

	ch = pl08x_get_phy_channel(pl08x, plchan);
	if (!ch) {
		dev_dbg(&pl08x->adev->dev, "no physical channel available for xfer on %s\n", plchan->name);
		plchan->state = PL08X_CHAN_WAITING;
		return;
	}

	dev_dbg(&pl08x->adev->dev, "allocated physical channel %d for xfer on %s\n",
		ch->id, plchan->name);

	plchan->phychan = ch;
	plchan->state = PL08X_CHAN_RUNNING;
	pl08x_start_next_txd(plchan);
}

static void pl08x_phy_reassign_start(struct pl08x_phy_chan *ch,
	struct pl08x_dma_chan *plchan)
{
	struct pl08x_driver_data *pl08x = plchan->host;

	dev_dbg(&pl08x->adev->dev, "reassigned physical channel %d for xfer on %s\n",
		ch->id, plchan->name);

	/*
	 * We do this without taking the lock; we're really only concerned
	 * about whether this pointer is NULL or not, and we're guaranteed
	 * that this will only be called when it _already_ is non-NULL.
	 */
	ch->serving = plchan;
	plchan->phychan = ch;
	plchan->state = PL08X_CHAN_RUNNING;
	pl08x_start_next_txd(plchan);
}

/*
 * Free a physical DMA channel, potentially reallocating it to another
 * virtual channel if we have any pending.
 */
static void pl08x_phy_free(struct pl08x_dma_chan *plchan)
{
	struct pl08x_driver_data *pl08x = plchan->host;
	struct pl08x_dma_chan *p, *next;

 retry:
	next = NULL;

	/* Find a waiting virtual channel for the next transfer. */
	list_for_each_entry(p, &pl08x->memcpy.channels, vc.chan.device_node)
		if (p->state == PL08X_CHAN_WAITING) {
			next = p;
			break;
		}

	if (!next) {
		list_for_each_entry(p, &pl08x->slave.channels, vc.chan.device_node)
			if (p->state == PL08X_CHAN_WAITING) {
				next = p;
				break;
			}
	}

	/* Ensure that the physical channel is stopped */
	pl08x_terminate_phy_chan(pl08x, plchan->phychan);

	if (next) {
		bool success;

		/*
		 * Eww.  We know this isn't going to deadlock
		 * but lockdep probably doesn't.
		 */
		spin_lock(&next->vc.lock);
		/* Re-check the state now that we have the lock */
		success = next->state == PL08X_CHAN_WAITING;
		if (success)
			pl08x_phy_reassign_start(plchan->phychan, next);
		spin_unlock(&next->vc.lock);

		/* If the state changed, try to find another channel */
		if (!success)
			goto retry;
	} else {
		/* No more jobs, so free up the physical channel */
		pl08x_put_phy_channel(pl08x, plchan->phychan);
	}

	plchan->phychan = NULL;
	plchan->state = PL08X_CHAN_IDLE;
>>>>>>> refs/remotes/origin/master
}

/*
 * LLI handling
 */

static inline unsigned int pl08x_get_bytes_for_cctl(unsigned int coded)
{
	switch (coded) {
	case PL080_WIDTH_8BIT:
		return 1;
	case PL080_WIDTH_16BIT:
		return 2;
	case PL080_WIDTH_32BIT:
		return 4;
	default:
		break;
	}
	BUG();
	return 0;
}

static inline u32 pl08x_cctl_bits(u32 cctl, u8 srcwidth, u8 dstwidth,
				  size_t tsize)
{
	u32 retbits = cctl;

	/* Remove all src, dst and transfer size bits */
	retbits &= ~PL080_CONTROL_DWIDTH_MASK;
	retbits &= ~PL080_CONTROL_SWIDTH_MASK;
	retbits &= ~PL080_CONTROL_TRANSFER_SIZE_MASK;

	/* Then set the bits according to the parameters */
	switch (srcwidth) {
	case 1:
		retbits |= PL080_WIDTH_8BIT << PL080_CONTROL_SWIDTH_SHIFT;
		break;
	case 2:
		retbits |= PL080_WIDTH_16BIT << PL080_CONTROL_SWIDTH_SHIFT;
		break;
	case 4:
		retbits |= PL080_WIDTH_32BIT << PL080_CONTROL_SWIDTH_SHIFT;
		break;
	default:
		BUG();
		break;
	}

	switch (dstwidth) {
	case 1:
		retbits |= PL080_WIDTH_8BIT << PL080_CONTROL_DWIDTH_SHIFT;
		break;
	case 2:
		retbits |= PL080_WIDTH_16BIT << PL080_CONTROL_DWIDTH_SHIFT;
		break;
	case 4:
		retbits |= PL080_WIDTH_32BIT << PL080_CONTROL_DWIDTH_SHIFT;
		break;
	default:
		BUG();
		break;
	}

<<<<<<< HEAD
=======
	tsize &= PL080_CONTROL_TRANSFER_SIZE_MASK;
>>>>>>> refs/remotes/origin/master
	retbits |= tsize << PL080_CONTROL_TRANSFER_SIZE_SHIFT;
	return retbits;
}

struct pl08x_lli_build_data {
	struct pl08x_txd *txd;
<<<<<<< HEAD
<<<<<<< HEAD
	struct pl08x_driver_data *pl08x;
	struct pl08x_bus_data srcbus;
	struct pl08x_bus_data dstbus;
	size_t remainder;
};

/*
 * Autoselect a master bus to use for the transfer this prefers the
 * destination bus if both available if fixed address on one bus the
 * other will be chosen
=======
=======
>>>>>>> refs/remotes/origin/master
	struct pl08x_bus_data srcbus;
	struct pl08x_bus_data dstbus;
	size_t remainder;
	u32 lli_bus;
};

/*
 * Autoselect a master bus to use for the transfer. Slave will be the chosen as
 * victim in case src & dest are not similarly aligned. i.e. If after aligning
 * masters address with width requirements of transfer (by sending few byte by
 * byte data), slave is still not aligned, then its width will be reduced to
 * BYTE.
 * - prefers the destination bus if both available
 * - prefers bus with fixed address (i.e. peripheral)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 */
static void pl08x_choose_master_bus(struct pl08x_lli_build_data *bd,
	struct pl08x_bus_data **mbus, struct pl08x_bus_data **sbus, u32 cctl)
{
	if (!(cctl & PL080_CONTROL_DST_INCR)) {
<<<<<<< HEAD
<<<<<<< HEAD
		*mbus = &bd->srcbus;
		*sbus = &bd->dstbus;
	} else if (!(cctl & PL080_CONTROL_SRC_INCR)) {
		*mbus = &bd->dstbus;
		*sbus = &bd->srcbus;
	} else {
		if (bd->dstbus.buswidth == 4) {
			*mbus = &bd->dstbus;
			*sbus = &bd->srcbus;
		} else if (bd->srcbus.buswidth == 4) {
			*mbus = &bd->srcbus;
			*sbus = &bd->dstbus;
		} else if (bd->dstbus.buswidth == 2) {
			*mbus = &bd->dstbus;
			*sbus = &bd->srcbus;
		} else if (bd->srcbus.buswidth == 2) {
			*mbus = &bd->srcbus;
			*sbus = &bd->dstbus;
		} else {
			/* bd->srcbus.buswidth == 1 */
			*mbus = &bd->dstbus;
			*sbus = &bd->srcbus;
=======
=======
>>>>>>> refs/remotes/origin/master
		*mbus = &bd->dstbus;
		*sbus = &bd->srcbus;
	} else if (!(cctl & PL080_CONTROL_SRC_INCR)) {
		*mbus = &bd->srcbus;
		*sbus = &bd->dstbus;
	} else {
		if (bd->dstbus.buswidth >= bd->srcbus.buswidth) {
			*mbus = &bd->dstbus;
			*sbus = &bd->srcbus;
		} else {
			*mbus = &bd->srcbus;
			*sbus = &bd->dstbus;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		}
	}
}

/*
 * Fills in one LLI for a certain transfer descriptor and advance the counter
 */
<<<<<<< HEAD
static void pl08x_fill_lli_for_desc(struct pl08x_lli_build_data *bd,
	int num_llis, int len, u32 cctl)
{
	struct pl08x_lli *llis_va = bd->txd->llis_va;
=======
static void pl08x_fill_lli_for_desc(struct pl08x_driver_data *pl08x,
				    struct pl08x_lli_build_data *bd,
				    int num_llis, int len, u32 cctl, u32 cctl2)
{
	u32 offset = num_llis * pl08x->lli_words;
	u32 *llis_va = bd->txd->llis_va + offset;
>>>>>>> refs/remotes/origin/master
	dma_addr_t llis_bus = bd->txd->llis_bus;

	BUG_ON(num_llis >= MAX_NUM_TSFR_LLIS);

<<<<<<< HEAD
	llis_va[num_llis].cctl = cctl;
	llis_va[num_llis].src = bd->srcbus.addr;
	llis_va[num_llis].dst = bd->dstbus.addr;
<<<<<<< HEAD
	llis_va[num_llis].lli = llis_bus + (num_llis + 1) * sizeof(struct pl08x_lli);
	if (bd->pl08x->lli_buses & PL08X_AHB2)
		llis_va[num_llis].lli |= PL080_LLI_LM_AHB2;
=======
	llis_va[num_llis].lli = llis_bus + (num_llis + 1) *
		sizeof(struct pl08x_lli);
	llis_va[num_llis].lli |= bd->lli_bus;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* Advance the offset to next LLI. */
	offset += pl08x->lli_words;

	llis_va[PL080_LLI_SRC] = bd->srcbus.addr;
	llis_va[PL080_LLI_DST] = bd->dstbus.addr;
	llis_va[PL080_LLI_LLI] = (llis_bus + sizeof(u32) * offset);
	llis_va[PL080_LLI_LLI] |= bd->lli_bus;
	llis_va[PL080_LLI_CCTL] = cctl;
	if (pl08x->vd->pl080s)
		llis_va[PL080S_LLI_CCTL2] = cctl2;
>>>>>>> refs/remotes/origin/master

	if (cctl & PL080_CONTROL_SRC_INCR)
		bd->srcbus.addr += len;
	if (cctl & PL080_CONTROL_DST_INCR)
		bd->dstbus.addr += len;

	BUG_ON(bd->remainder < len);

	bd->remainder -= len;
}

<<<<<<< HEAD
<<<<<<< HEAD
/*
 * Return number of bytes to fill to boundary, or len.
 * This calculation works for any value of addr.
 */
static inline size_t pl08x_pre_boundary(u32 addr, size_t len)
{
	size_t boundary_len = PL08X_BOUNDARY_SIZE -
			(addr & (PL08X_BOUNDARY_SIZE - 1));

	return min(boundary_len, len);
=======
static inline void prep_byte_width_lli(struct pl08x_lli_build_data *bd,
		u32 *cctl, u32 len, int num_llis, size_t *total_bytes)
{
	*cctl = pl08x_cctl_bits(*cctl, 1, 1, len);
	pl08x_fill_lli_for_desc(bd, num_llis, len, *cctl);
	(*total_bytes) += len;
>>>>>>> refs/remotes/origin/cm-10.0
}

=======
static inline void prep_byte_width_lli(struct pl08x_driver_data *pl08x,
			struct pl08x_lli_build_data *bd, u32 *cctl, u32 len,
			int num_llis, size_t *total_bytes)
{
	*cctl = pl08x_cctl_bits(*cctl, 1, 1, len);
	pl08x_fill_lli_for_desc(pl08x, bd, num_llis, len, *cctl, len);
	(*total_bytes) += len;
}

#ifdef VERBOSE_DEBUG
static void pl08x_dump_lli(struct pl08x_driver_data *pl08x,
			   const u32 *llis_va, int num_llis)
{
	int i;

	if (pl08x->vd->pl080s) {
		dev_vdbg(&pl08x->adev->dev,
			"%-3s %-9s  %-10s %-10s %-10s %-10s %s\n",
			"lli", "", "csrc", "cdst", "clli", "cctl", "cctl2");
		for (i = 0; i < num_llis; i++) {
			dev_vdbg(&pl08x->adev->dev,
				"%3d @%p: 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\n",
				i, llis_va, llis_va[PL080_LLI_SRC],
				llis_va[PL080_LLI_DST], llis_va[PL080_LLI_LLI],
				llis_va[PL080_LLI_CCTL],
				llis_va[PL080S_LLI_CCTL2]);
			llis_va += pl08x->lli_words;
		}
	} else {
		dev_vdbg(&pl08x->adev->dev,
			"%-3s %-9s  %-10s %-10s %-10s %s\n",
			"lli", "", "csrc", "cdst", "clli", "cctl");
		for (i = 0; i < num_llis; i++) {
			dev_vdbg(&pl08x->adev->dev,
				"%3d @%p: 0x%08x 0x%08x 0x%08x 0x%08x\n",
				i, llis_va, llis_va[PL080_LLI_SRC],
				llis_va[PL080_LLI_DST], llis_va[PL080_LLI_LLI],
				llis_va[PL080_LLI_CCTL]);
			llis_va += pl08x->lli_words;
		}
	}
}
#else
static inline void pl08x_dump_lli(struct pl08x_driver_data *pl08x,
				  const u32 *llis_va, int num_llis) {}
#endif

>>>>>>> refs/remotes/origin/master
/*
 * This fills in the table of LLIs for the transfer descriptor
 * Note that we assume we never have to change the burst sizes
 * Return 0 for error
 */
static int pl08x_fill_llis_for_desc(struct pl08x_driver_data *pl08x,
			      struct pl08x_txd *txd)
{
	struct pl08x_bus_data *mbus, *sbus;
	struct pl08x_lli_build_data bd;
	int num_llis = 0;
<<<<<<< HEAD
<<<<<<< HEAD
	u32 cctl;
	size_t max_bytes_per_lli;
	size_t total_bytes = 0;
	struct pl08x_lli *llis_va;

	txd->llis_va = dma_pool_alloc(pl08x->pool, GFP_NOWAIT,
				      &txd->llis_bus);
=======
	u32 cctl, early_bytes = 0;
	size_t max_bytes_per_lli, total_bytes;
	struct pl08x_lli *llis_va;
	struct pl08x_sg *dsg;

	txd->llis_va = dma_pool_alloc(pl08x->pool, GFP_NOWAIT, &txd->llis_bus);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	u32 cctl, early_bytes = 0;
	size_t max_bytes_per_lli, total_bytes;
	u32 *llis_va, *last_lli;
	struct pl08x_sg *dsg;

	txd->llis_va = dma_pool_alloc(pl08x->pool, GFP_NOWAIT, &txd->llis_bus);
>>>>>>> refs/remotes/origin/master
	if (!txd->llis_va) {
		dev_err(&pl08x->adev->dev, "%s no memory for llis\n", __func__);
		return 0;
	}

<<<<<<< HEAD
	pl08x->pool_ctr++;

<<<<<<< HEAD
	/* Get the default CCTL */
	cctl = txd->cctl;

	bd.txd = txd;
	bd.pl08x = pl08x;
	bd.srcbus.addr = txd->src_addr;
	bd.dstbus.addr = txd->dst_addr;
=======
	bd.txd = txd;
	bd.lli_bus = (pl08x->lli_buses & PL08X_AHB2) ? PL080_LLI_LM_AHB2 : 0;
	cctl = txd->cctl;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	bd.txd = txd;
	bd.lli_bus = (pl08x->lli_buses & PL08X_AHB2) ? PL080_LLI_LM_AHB2 : 0;
	cctl = txd->cctl;
>>>>>>> refs/remotes/origin/master

	/* Find maximum width of the source bus */
	bd.srcbus.maxwidth =
		pl08x_get_bytes_for_cctl((cctl & PL080_CONTROL_SWIDTH_MASK) >>
				       PL080_CONTROL_SWIDTH_SHIFT);

	/* Find maximum width of the destination bus */
	bd.dstbus.maxwidth =
		pl08x_get_bytes_for_cctl((cctl & PL080_CONTROL_DWIDTH_MASK) >>
				       PL080_CONTROL_DWIDTH_SHIFT);

<<<<<<< HEAD
<<<<<<< HEAD
	/* Set up the bus widths to the maximum */
	bd.srcbus.buswidth = bd.srcbus.maxwidth;
	bd.dstbus.buswidth = bd.dstbus.maxwidth;
	dev_vdbg(&pl08x->adev->dev,
		 "%s source bus is %d bytes wide, dest bus is %d bytes wide\n",
		 __func__, bd.srcbus.buswidth, bd.dstbus.buswidth);


	/*
	 * Bytes transferred == tsize * MIN(buswidths), not max(buswidths)
	 */
	max_bytes_per_lli = min(bd.srcbus.buswidth, bd.dstbus.buswidth) *
		PL080_CONTROL_TRANSFER_SIZE_MASK;
	dev_vdbg(&pl08x->adev->dev,
		 "%s max bytes per lli = %zu\n",
		 __func__, max_bytes_per_lli);

	/* We need to count this down to zero */
	bd.remainder = txd->len;
	dev_vdbg(&pl08x->adev->dev,
		 "%s remainder = %zu\n",
		 __func__, bd.remainder);

	/*
	 * Choose bus to align to
	 * - prefers destination bus if both available
	 * - if fixed address on one bus chooses other
	 */
	pl08x_choose_master_bus(&bd, &mbus, &sbus, cctl);

	if (txd->len < mbus->buswidth) {
		/* Less than a bus width available - send as single bytes */
		while (bd.remainder) {
			dev_vdbg(&pl08x->adev->dev,
				 "%s single byte LLIs for a transfer of "
				 "less than a bus width (remain 0x%08x)\n",
				 __func__, bd.remainder);
			cctl = pl08x_cctl_bits(cctl, 1, 1, 1);
			pl08x_fill_lli_for_desc(&bd, num_llis++, 1, cctl);
			total_bytes++;
		}
	} else {
		/* Make one byte LLIs until master bus is aligned */
		while ((mbus->addr) % (mbus->buswidth)) {
			dev_vdbg(&pl08x->adev->dev,
				"%s adjustment lli for less than bus width "
				 "(remain 0x%08x)\n",
				 __func__, bd.remainder);
			cctl = pl08x_cctl_bits(cctl, 1, 1, 1);
			pl08x_fill_lli_for_desc(&bd, num_llis++, 1, cctl);
			total_bytes++;
		}

		/*
		 * Master now aligned
		 * - if slave is not then we must set its width down
		 */
		if (sbus->addr % sbus->buswidth) {
			dev_dbg(&pl08x->adev->dev,
				"%s set down bus width to one byte\n",
				 __func__);

			sbus->buswidth = 1;
		}

		/*
		 * Make largest possible LLIs until less than one bus
		 * width left
		 */
		while (bd.remainder > (mbus->buswidth - 1)) {
			size_t lli_len, target_len, tsize, odd_bytes;

			/*
			 * If enough left try to send max possible,
			 * otherwise try to send the remainder
			 */
			target_len = min(bd.remainder, max_bytes_per_lli);

			/*
			 * Set bus lengths for incrementing buses to the
			 * number of bytes which fill to next memory boundary,
			 * limiting on the target length calculated above.
			 */
			if (cctl & PL080_CONTROL_SRC_INCR)
				bd.srcbus.fill_bytes =
					pl08x_pre_boundary(bd.srcbus.addr,
						target_len);
			else
				bd.srcbus.fill_bytes = target_len;

			if (cctl & PL080_CONTROL_DST_INCR)
				bd.dstbus.fill_bytes =
					pl08x_pre_boundary(bd.dstbus.addr,
						target_len);
			else
				bd.dstbus.fill_bytes = target_len;

			/* Find the nearest */
			lli_len	= min(bd.srcbus.fill_bytes,
				      bd.dstbus.fill_bytes);

			BUG_ON(lli_len > bd.remainder);

			if (lli_len <= 0) {
				dev_err(&pl08x->adev->dev,
					"%s lli_len is %zu, <= 0\n",
						__func__, lli_len);
				return 0;
			}

			if (lli_len == target_len) {
				/*
				 * Can send what we wanted.
				 * Maintain alignment
				 */
				lli_len	= (lli_len/mbus->buswidth) *
							mbus->buswidth;
				odd_bytes = 0;
			} else {
				/*
				 * So now we know how many bytes to transfer
				 * to get to the nearest boundary.  The next
				 * LLI will past the boundary.  However, we
				 * may be working to a boundary on the slave
				 * bus.  We need to ensure the master stays
				 * aligned, and that we are working in
				 * multiples of the bus widths.
				 */
				odd_bytes = lli_len % mbus->buswidth;
				lli_len -= odd_bytes;

			}

			if (lli_len) {
				/*
				 * Check against minimum bus alignment:
				 * Calculate actual transfer size in relation
				 * to bus width an get a maximum remainder of
				 * the smallest bus width - 1
				 */
				/* FIXME: use round_down()? */
				tsize = lli_len / min(mbus->buswidth,
						      sbus->buswidth);
				lli_len	= tsize * min(mbus->buswidth,
						      sbus->buswidth);

				if (target_len != lli_len) {
					dev_vdbg(&pl08x->adev->dev,
					"%s can't send what we want. Desired 0x%08zx, lli of 0x%08zx bytes in txd of 0x%08zx\n",
					__func__, target_len, lli_len, txd->len);
				}

				cctl = pl08x_cctl_bits(cctl,
						       bd.srcbus.buswidth,
						       bd.dstbus.buswidth,
						       tsize);

				dev_vdbg(&pl08x->adev->dev,
					"%s fill lli with single lli chunk of size 0x%08zx (remainder 0x%08zx)\n",
					__func__, lli_len, bd.remainder);
				pl08x_fill_lli_for_desc(&bd, num_llis++,
					lli_len, cctl);
				total_bytes += lli_len;
			}


			if (odd_bytes) {
				/*
				 * Creep past the boundary, maintaining
				 * master alignment
				 */
				int j;
				for (j = 0; (j < mbus->buswidth)
						&& (bd.remainder); j++) {
					cctl = pl08x_cctl_bits(cctl, 1, 1, 1);
					dev_vdbg(&pl08x->adev->dev,
						"%s align with boundary, single byte (remain 0x%08zx)\n",
						__func__, bd.remainder);
					pl08x_fill_lli_for_desc(&bd,
						num_llis++, 1, cctl);
					total_bytes++;
				}
			}
		}

		/*
		 * Send any odd bytes
		 */
		while (bd.remainder) {
			cctl = pl08x_cctl_bits(cctl, 1, 1, 1);
			dev_vdbg(&pl08x->adev->dev,
				"%s align with boundary, single odd byte (remain %zu)\n",
				__func__, bd.remainder);
			pl08x_fill_lli_for_desc(&bd, num_llis++, 1, cctl);
			total_bytes++;
		}
	}
	if (total_bytes != txd->len) {
		dev_err(&pl08x->adev->dev,
			"%s size of encoded lli:s don't match total txd, transferred 0x%08zx from size 0x%08zx\n",
			__func__, total_bytes, txd->len);
		return 0;
	}

	if (num_llis >= MAX_NUM_TSFR_LLIS) {
		dev_err(&pl08x->adev->dev,
			"%s need to increase MAX_NUM_TSFR_LLIS from 0x%08x\n",
			__func__, (u32) MAX_NUM_TSFR_LLIS);
		return 0;
=======
=======
>>>>>>> refs/remotes/origin/master
	list_for_each_entry(dsg, &txd->dsg_list, node) {
		total_bytes = 0;
		cctl = txd->cctl;

		bd.srcbus.addr = dsg->src_addr;
		bd.dstbus.addr = dsg->dst_addr;
		bd.remainder = dsg->len;
		bd.srcbus.buswidth = bd.srcbus.maxwidth;
		bd.dstbus.buswidth = bd.dstbus.maxwidth;

		pl08x_choose_master_bus(&bd, &mbus, &sbus, cctl);

<<<<<<< HEAD
		dev_vdbg(&pl08x->adev->dev, "src=0x%08x%s/%u dst=0x%08x%s/%u len=%zu\n",
			bd.srcbus.addr, cctl & PL080_CONTROL_SRC_INCR ? "+" : "",
			bd.srcbus.buswidth,
			bd.dstbus.addr, cctl & PL080_CONTROL_DST_INCR ? "+" : "",
=======
		dev_vdbg(&pl08x->adev->dev,
			"src=0x%08llx%s/%u dst=0x%08llx%s/%u len=%zu\n",
			(u64)bd.srcbus.addr,
			cctl & PL080_CONTROL_SRC_INCR ? "+" : "",
			bd.srcbus.buswidth,
			(u64)bd.dstbus.addr,
			cctl & PL080_CONTROL_DST_INCR ? "+" : "",
>>>>>>> refs/remotes/origin/master
			bd.dstbus.buswidth,
			bd.remainder);
		dev_vdbg(&pl08x->adev->dev, "mbus=%s sbus=%s\n",
			mbus == &bd.srcbus ? "src" : "dst",
			sbus == &bd.srcbus ? "src" : "dst");

		/*
		 * Zero length is only allowed if all these requirements are
		 * met:
		 * - flow controller is peripheral.
		 * - src.addr is aligned to src.width
		 * - dst.addr is aligned to dst.width
		 *
		 * sg_len == 1 should be true, as there can be two cases here:
		 *
		 * - Memory addresses are contiguous and are not scattered.
		 *   Here, Only one sg will be passed by user driver, with
		 *   memory address and zero length. We pass this to controller
		 *   and after the transfer it will receive the last burst
		 *   request from peripheral and so transfer finishes.
		 *
		 * - Memory addresses are scattered and are not contiguous.
		 *   Here, Obviously as DMA controller doesn't know when a lli's
		 *   transfer gets over, it can't load next lli. So in this
		 *   case, there has to be an assumption that only one lli is
		 *   supported. Thus, we can't have scattered addresses.
		 */
		if (!bd.remainder) {
			u32 fc = (txd->ccfg & PL080_CONFIG_FLOW_CONTROL_MASK) >>
				PL080_CONFIG_FLOW_CONTROL_SHIFT;
			if (!((fc >= PL080_FLOW_SRC2DST_DST) &&
					(fc <= PL080_FLOW_SRC2DST_SRC))) {
				dev_err(&pl08x->adev->dev, "%s sg len can't be zero",
					__func__);
				return 0;
			}

<<<<<<< HEAD
			if ((bd.srcbus.addr % bd.srcbus.buswidth) ||
					(bd.dstbus.addr % bd.dstbus.buswidth)) {
=======
			if (!IS_BUS_ALIGNED(&bd.srcbus) ||
				!IS_BUS_ALIGNED(&bd.dstbus)) {
>>>>>>> refs/remotes/origin/master
				dev_err(&pl08x->adev->dev,
					"%s src & dst address must be aligned to src"
					" & dst width if peripheral is flow controller",
					__func__);
				return 0;
			}

			cctl = pl08x_cctl_bits(cctl, bd.srcbus.buswidth,
					bd.dstbus.buswidth, 0);
<<<<<<< HEAD
			pl08x_fill_lli_for_desc(&bd, num_llis++, 0, cctl);
=======
			pl08x_fill_lli_for_desc(pl08x, &bd, num_llis++,
					0, cctl, 0);
>>>>>>> refs/remotes/origin/master
			break;
		}

		/*
		 * Send byte by byte for following cases
		 * - Less than a bus width available
		 * - until master bus is aligned
		 */
		if (bd.remainder < mbus->buswidth)
			early_bytes = bd.remainder;
<<<<<<< HEAD
		else if ((mbus->addr) % (mbus->buswidth)) {
			early_bytes = mbus->buswidth - (mbus->addr) %
				(mbus->buswidth);
=======
		else if (!IS_BUS_ALIGNED(mbus)) {
			early_bytes = mbus->buswidth -
				(mbus->addr & (mbus->buswidth - 1));
>>>>>>> refs/remotes/origin/master
			if ((bd.remainder - early_bytes) < mbus->buswidth)
				early_bytes = bd.remainder;
		}

		if (early_bytes) {
			dev_vdbg(&pl08x->adev->dev,
				"%s byte width LLIs (remain 0x%08x)\n",
				__func__, bd.remainder);
<<<<<<< HEAD
			prep_byte_width_lli(&bd, &cctl, early_bytes, num_llis++,
				&total_bytes);
=======
			prep_byte_width_lli(pl08x, &bd, &cctl, early_bytes,
				num_llis++, &total_bytes);
>>>>>>> refs/remotes/origin/master
		}

		if (bd.remainder) {
			/*
			 * Master now aligned
			 * - if slave is not then we must set its width down
			 */
<<<<<<< HEAD
			if (sbus->addr % sbus->buswidth) {
=======
			if (!IS_BUS_ALIGNED(sbus)) {
>>>>>>> refs/remotes/origin/master
				dev_dbg(&pl08x->adev->dev,
					"%s set down bus width to one byte\n",
					__func__);

				sbus->buswidth = 1;
			}

			/*
			 * Bytes transferred = tsize * src width, not
			 * MIN(buswidths)
			 */
			max_bytes_per_lli = bd.srcbus.buswidth *
<<<<<<< HEAD
				PL080_CONTROL_TRANSFER_SIZE_MASK;
=======
						pl08x->vd->max_transfer_size;
>>>>>>> refs/remotes/origin/master
			dev_vdbg(&pl08x->adev->dev,
				"%s max bytes per lli = %zu\n",
				__func__, max_bytes_per_lli);

			/*
			 * Make largest possible LLIs until less than one bus
			 * width left
			 */
			while (bd.remainder > (mbus->buswidth - 1)) {
				size_t lli_len, tsize, width;

				/*
				 * If enough left try to send max possible,
				 * otherwise try to send the remainder
				 */
				lli_len = min(bd.remainder, max_bytes_per_lli);

				/*
				 * Check against maximum bus alignment:
				 * Calculate actual transfer size in relation to
				 * bus width an get a maximum remainder of the
				 * highest bus width - 1
				 */
				width = max(mbus->buswidth, sbus->buswidth);
				lli_len = (lli_len / width) * width;
				tsize = lli_len / bd.srcbus.buswidth;

				dev_vdbg(&pl08x->adev->dev,
					"%s fill lli with single lli chunk of "
					"size 0x%08zx (remainder 0x%08zx)\n",
					__func__, lli_len, bd.remainder);

				cctl = pl08x_cctl_bits(cctl, bd.srcbus.buswidth,
					bd.dstbus.buswidth, tsize);
<<<<<<< HEAD
				pl08x_fill_lli_for_desc(&bd, num_llis++,
						lli_len, cctl);
=======
				pl08x_fill_lli_for_desc(pl08x, &bd, num_llis++,
						lli_len, cctl, tsize);
>>>>>>> refs/remotes/origin/master
				total_bytes += lli_len;
			}

			/*
			 * Send any odd bytes
			 */
			if (bd.remainder) {
				dev_vdbg(&pl08x->adev->dev,
					"%s align with boundary, send odd bytes (remain %zu)\n",
					__func__, bd.remainder);
<<<<<<< HEAD
				prep_byte_width_lli(&bd, &cctl, bd.remainder,
						num_llis++, &total_bytes);
=======
				prep_byte_width_lli(pl08x, &bd, &cctl,
					bd.remainder, num_llis++, &total_bytes);
>>>>>>> refs/remotes/origin/master
			}
		}

		if (total_bytes != dsg->len) {
			dev_err(&pl08x->adev->dev,
				"%s size of encoded lli:s don't match total txd, transferred 0x%08zx from size 0x%08zx\n",
				__func__, total_bytes, dsg->len);
			return 0;
		}

		if (num_llis >= MAX_NUM_TSFR_LLIS) {
			dev_err(&pl08x->adev->dev,
				"%s need to increase MAX_NUM_TSFR_LLIS from 0x%08x\n",
<<<<<<< HEAD
				__func__, (u32) MAX_NUM_TSFR_LLIS);
			return 0;
		}
>>>>>>> refs/remotes/origin/cm-10.0
	}

	llis_va = txd->llis_va;
	/* The final LLI terminates the LLI. */
	llis_va[num_llis - 1].lli = 0;
	/* The final LLI element shall also fire an interrupt. */
	llis_va[num_llis - 1].cctl |= PL080_CONTROL_TC_IRQ_EN;

#ifdef VERBOSE_DEBUG
	{
		int i;

<<<<<<< HEAD
		for (i = 0; i < num_llis; i++) {
			dev_vdbg(&pl08x->adev->dev,
				 "lli %d @%p: csrc=0x%08x, cdst=0x%08x, cctl=0x%08x, clli=0x%08x\n",
				 i,
				 &llis_va[i],
				 llis_va[i].src,
				 llis_va[i].dst,
				 llis_va[i].cctl,
				 llis_va[i].lli
=======
		dev_vdbg(&pl08x->adev->dev,
			 "%-3s %-9s  %-10s %-10s %-10s %s\n",
			 "lli", "", "csrc", "cdst", "clli", "cctl");
		for (i = 0; i < num_llis; i++) {
			dev_vdbg(&pl08x->adev->dev,
				 "%3d @%p: 0x%08x 0x%08x 0x%08x 0x%08x\n",
				 i, &llis_va[i], llis_va[i].src,
				 llis_va[i].dst, llis_va[i].lli, llis_va[i].cctl
>>>>>>> refs/remotes/origin/cm-10.0
				);
		}
	}
#endif
=======
				__func__, MAX_NUM_TSFR_LLIS);
			return 0;
		}
	}

	llis_va = txd->llis_va;
	last_lli = llis_va + (num_llis - 1) * pl08x->lli_words;

	if (txd->cyclic) {
		/* Link back to the first LLI. */
		last_lli[PL080_LLI_LLI] = txd->llis_bus | bd.lli_bus;
	} else {
		/* The final LLI terminates the LLI. */
		last_lli[PL080_LLI_LLI] = 0;
		/* The final LLI element shall also fire an interrupt. */
		last_lli[PL080_LLI_CCTL] |= PL080_CONTROL_TC_IRQ_EN;
	}

	pl08x_dump_lli(pl08x, llis_va, num_llis);
>>>>>>> refs/remotes/origin/master

	return num_llis;
}

<<<<<<< HEAD
/* You should call this with the struct pl08x lock held */
static void pl08x_free_txd(struct pl08x_driver_data *pl08x,
			   struct pl08x_txd *txd)
{
<<<<<<< HEAD
	/* Free the LLI */
	dma_pool_free(pl08x->pool, txd->llis_va, txd->llis_bus);

	pl08x->pool_ctr--;

=======
	struct pl08x_sg *dsg, *_dsg;

	/* Free the LLI */
	if (txd->llis_va)
		dma_pool_free(pl08x->pool, txd->llis_va, txd->llis_bus);

	pl08x->pool_ctr--;

=======
static void pl08x_free_txd(struct pl08x_driver_data *pl08x,
			   struct pl08x_txd *txd)
{
	struct pl08x_sg *dsg, *_dsg;

	if (txd->llis_va)
		dma_pool_free(pl08x->pool, txd->llis_va, txd->llis_bus);

>>>>>>> refs/remotes/origin/master
	list_for_each_entry_safe(dsg, _dsg, &txd->dsg_list, node) {
		list_del(&dsg->node);
		kfree(dsg);
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	kfree(txd);
}

static void pl08x_free_txd_list(struct pl08x_driver_data *pl08x,
				struct pl08x_dma_chan *plchan)
{
	struct pl08x_txd *txdi = NULL;
	struct pl08x_txd *next;

	if (!list_empty(&plchan->pend_list)) {
		list_for_each_entry_safe(txdi,
					 next, &plchan->pend_list, node) {
			list_del(&txdi->node);
			pl08x_free_txd(pl08x, txdi);
		}
	}
=======
	kfree(txd);
}

static void pl08x_desc_free(struct virt_dma_desc *vd)
{
	struct pl08x_txd *txd = to_pl08x_txd(&vd->tx);
	struct pl08x_dma_chan *plchan = to_pl08x_chan(vd->tx.chan);

	dma_descriptor_unmap(&vd->tx);
	if (!txd->done)
		pl08x_release_mux(plchan);

	pl08x_free_txd(plchan->host, txd);
}

static void pl08x_free_txd_list(struct pl08x_driver_data *pl08x,
				struct pl08x_dma_chan *plchan)
{
	LIST_HEAD(head);

	vchan_get_all_descriptors(&plchan->vc, &head);
	vchan_dma_desc_free_list(&plchan->vc, &head);
>>>>>>> refs/remotes/origin/master
}

/*
 * The DMA ENGINE API
 */
static int pl08x_alloc_chan_resources(struct dma_chan *chan)
{
	return 0;
}

static void pl08x_free_chan_resources(struct dma_chan *chan)
{
<<<<<<< HEAD
}

/*
 * This should be called with the channel plchan->lock held
 */
static int prep_phy_channel(struct pl08x_dma_chan *plchan,
			    struct pl08x_txd *txd)
{
	struct pl08x_driver_data *pl08x = plchan->host;
	struct pl08x_phy_chan *ch;
	int ret;

	/* Check if we already have a channel */
<<<<<<< HEAD
	if (plchan->phychan)
		return 0;
=======
	if (plchan->phychan) {
		ch = plchan->phychan;
		goto got_channel;
	}
>>>>>>> refs/remotes/origin/cm-10.0

	ch = pl08x_get_phy_channel(pl08x, plchan);
	if (!ch) {
		/* No physical channel available, cope with it */
		dev_dbg(&pl08x->adev->dev, "no physical channel available for xfer on %s\n", plchan->name);
		return -EBUSY;
	}

	/*
	 * OK we have a physical channel: for memcpy() this is all we
	 * need, but for slaves the physical signals may be muxed!
	 * Can the platform allow us to use this channel?
	 */
<<<<<<< HEAD
	if (plchan->slave &&
	    ch->signal < 0 &&
	    pl08x->pd->get_signal) {
=======
	if (plchan->slave && pl08x->pd->get_signal) {
>>>>>>> refs/remotes/origin/cm-10.0
		ret = pl08x->pd->get_signal(plchan);
		if (ret < 0) {
			dev_dbg(&pl08x->adev->dev,
				"unable to use physical channel %d for transfer on %s due to platform restrictions\n",
				ch->id, plchan->name);
			/* Release physical channel & return */
			pl08x_put_phy_channel(pl08x, ch);
			return -EBUSY;
		}
		ch->signal = ret;
<<<<<<< HEAD

		/* Assign the flow control signal to this channel */
		if (txd->direction == DMA_TO_DEVICE)
			txd->ccfg |= ch->signal << PL080_CONFIG_DST_SEL_SHIFT;
		else if (txd->direction == DMA_FROM_DEVICE)
			txd->ccfg |= ch->signal << PL080_CONFIG_SRC_SEL_SHIFT;
	}

=======
	}

	plchan->phychan = ch;
>>>>>>> refs/remotes/origin/cm-10.0
	dev_dbg(&pl08x->adev->dev, "allocated physical channel %d and signal %d for xfer on %s\n",
		 ch->id,
		 ch->signal,
		 plchan->name);

<<<<<<< HEAD
	plchan->phychan_hold++;
	plchan->phychan = ch;
=======
got_channel:
	/* Assign the flow control signal to this channel */
	if (txd->direction == DMA_MEM_TO_DEV)
		txd->ccfg |= ch->signal << PL080_CONFIG_DST_SEL_SHIFT;
	else if (txd->direction == DMA_DEV_TO_MEM)
		txd->ccfg |= ch->signal << PL080_CONFIG_SRC_SEL_SHIFT;

	plchan->phychan_hold++;
>>>>>>> refs/remotes/origin/cm-10.0

	return 0;
}

static void release_phy_channel(struct pl08x_dma_chan *plchan)
{
	struct pl08x_driver_data *pl08x = plchan->host;

	if ((plchan->phychan->signal >= 0) && pl08x->pd->put_signal) {
		pl08x->pd->put_signal(plchan);
		plchan->phychan->signal = -1;
	}
	pl08x_put_phy_channel(pl08x, plchan->phychan);
	plchan->phychan = NULL;
}

static dma_cookie_t pl08x_tx_submit(struct dma_async_tx_descriptor *tx)
{
	struct pl08x_dma_chan *plchan = to_pl08x_chan(tx->chan);
	struct pl08x_txd *txd = to_pl08x_txd(tx);
	unsigned long flags;
<<<<<<< HEAD

	spin_lock_irqsave(&plchan->lock, flags);

	plchan->chan.cookie += 1;
	if (plchan->chan.cookie < 0)
		plchan->chan.cookie = 1;
	tx->cookie = plchan->chan.cookie;
=======
	dma_cookie_t cookie;

	spin_lock_irqsave(&plchan->lock, flags);
	cookie = dma_cookie_assign(tx);
>>>>>>> refs/remotes/origin/cm-10.0

	/* Put this onto the pending list */
	list_add_tail(&txd->node, &plchan->pend_list);

	/*
	 * If there was no physical channel available for this memcpy,
	 * stack the request up and indicate that the channel is waiting
	 * for a free physical channel.
	 */
	if (!plchan->slave && !plchan->phychan) {
		/* Do this memcpy whenever there is a channel ready */
		plchan->state = PL08X_CHAN_WAITING;
		plchan->waiting = txd;
	} else {
		plchan->phychan_hold--;
	}

	spin_unlock_irqrestore(&plchan->lock, flags);

<<<<<<< HEAD
	return tx->cookie;
=======
	return cookie;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* Ensure all queued descriptors are freed */
	vchan_free_chan_resources(to_virt_chan(chan));
>>>>>>> refs/remotes/origin/master
}

static struct dma_async_tx_descriptor *pl08x_prep_dma_interrupt(
		struct dma_chan *chan, unsigned long flags)
{
	struct dma_async_tx_descriptor *retval = NULL;

	return retval;
}

/*
 * Code accessing dma_async_is_complete() in a tight loop may give problems.
 * If slaves are relying on interrupts to signal completion this function
 * must not be called with interrupts disabled.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static enum dma_status
pl08x_dma_tx_status(struct dma_chan *chan,
		    dma_cookie_t cookie,
		    struct dma_tx_state *txstate)
{
	struct pl08x_dma_chan *plchan = to_pl08x_chan(chan);
	dma_cookie_t last_used;
	dma_cookie_t last_complete;
	enum dma_status ret;
	u32 bytesleft = 0;

	last_used = plchan->chan.cookie;
	last_complete = plchan->lc;

	ret = dma_async_is_complete(cookie, last_complete, last_used);
	if (ret == DMA_SUCCESS) {
		dma_set_tx_state(txstate, last_complete, last_used, 0);
		return ret;
	}

	/*
	 * This cookie not complete yet
	 */
	last_used = plchan->chan.cookie;
	last_complete = plchan->lc;

	/* Get number of bytes left in the active transactions and queue */
	bytesleft = pl08x_getbytes_chan(plchan);

	dma_set_tx_state(txstate, last_complete, last_used,
			 bytesleft);
=======
static enum dma_status pl08x_dma_tx_status(struct dma_chan *chan,
		dma_cookie_t cookie, struct dma_tx_state *txstate)
{
	struct pl08x_dma_chan *plchan = to_pl08x_chan(chan);
	enum dma_status ret;

	ret = dma_cookie_status(chan, cookie, txstate);
	if (ret == DMA_SUCCESS)
		return ret;
=======
static enum dma_status pl08x_dma_tx_status(struct dma_chan *chan,
		dma_cookie_t cookie, struct dma_tx_state *txstate)
{
	struct pl08x_dma_chan *plchan = to_pl08x_chan(chan);
	struct virt_dma_desc *vd;
	unsigned long flags;
	enum dma_status ret;
	size_t bytes = 0;

	ret = dma_cookie_status(chan, cookie, txstate);
	if (ret == DMA_COMPLETE)
		return ret;

	/*
	 * There's no point calculating the residue if there's
	 * no txstate to store the value.
	 */
	if (!txstate) {
		if (plchan->state == PL08X_CHAN_PAUSED)
			ret = DMA_PAUSED;
		return ret;
	}

	spin_lock_irqsave(&plchan->vc.lock, flags);
	ret = dma_cookie_status(chan, cookie, txstate);
	if (ret != DMA_COMPLETE) {
		vd = vchan_find_desc(&plchan->vc, cookie);
		if (vd) {
			/* On the issued list, so hasn't been processed yet */
			struct pl08x_txd *txd = to_pl08x_txd(&vd->tx);
			struct pl08x_sg *dsg;

			list_for_each_entry(dsg, &txd->dsg_list, node)
				bytes += dsg->len;
		} else {
			bytes = pl08x_getbytes_chan(plchan);
		}
	}
	spin_unlock_irqrestore(&plchan->vc.lock, flags);
>>>>>>> refs/remotes/origin/master

	/*
	 * This cookie not complete yet
	 * Get number of bytes left in the active transactions and queue
	 */
<<<<<<< HEAD
	dma_set_residue(txstate, pl08x_getbytes_chan(plchan));
>>>>>>> refs/remotes/origin/cm-10.0

	if (plchan->state == PL08X_CHAN_PAUSED)
		return DMA_PAUSED;

	/* Whether waiting or running, we're in progress */
	return DMA_IN_PROGRESS;
=======
	dma_set_residue(txstate, bytes);

	if (plchan->state == PL08X_CHAN_PAUSED && ret == DMA_IN_PROGRESS)
		ret = DMA_PAUSED;

	/* Whether waiting or running, we're in progress */
	return ret;
>>>>>>> refs/remotes/origin/master
}

/* PrimeCell DMA extension */
struct burst_table {
<<<<<<< HEAD
<<<<<<< HEAD
	int burstwords;
=======
	u32 burstwords;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	u32 burstwords;
>>>>>>> refs/remotes/origin/master
	u32 reg;
};

static const struct burst_table burst_sizes[] = {
	{
		.burstwords = 256,
<<<<<<< HEAD
<<<<<<< HEAD
		.reg = (PL080_BSIZE_256 << PL080_CONTROL_SB_SIZE_SHIFT) |
			(PL080_BSIZE_256 << PL080_CONTROL_DB_SIZE_SHIFT),
	},
	{
		.burstwords = 128,
		.reg = (PL080_BSIZE_128 << PL080_CONTROL_SB_SIZE_SHIFT) |
			(PL080_BSIZE_128 << PL080_CONTROL_DB_SIZE_SHIFT),
	},
	{
		.burstwords = 64,
		.reg = (PL080_BSIZE_64 << PL080_CONTROL_SB_SIZE_SHIFT) |
			(PL080_BSIZE_64 << PL080_CONTROL_DB_SIZE_SHIFT),
	},
	{
		.burstwords = 32,
		.reg = (PL080_BSIZE_32 << PL080_CONTROL_SB_SIZE_SHIFT) |
			(PL080_BSIZE_32 << PL080_CONTROL_DB_SIZE_SHIFT),
	},
	{
		.burstwords = 16,
		.reg = (PL080_BSIZE_16 << PL080_CONTROL_SB_SIZE_SHIFT) |
			(PL080_BSIZE_16 << PL080_CONTROL_DB_SIZE_SHIFT),
	},
	{
		.burstwords = 8,
		.reg = (PL080_BSIZE_8 << PL080_CONTROL_SB_SIZE_SHIFT) |
			(PL080_BSIZE_8 << PL080_CONTROL_DB_SIZE_SHIFT),
	},
	{
		.burstwords = 4,
		.reg = (PL080_BSIZE_4 << PL080_CONTROL_SB_SIZE_SHIFT) |
			(PL080_BSIZE_4 << PL080_CONTROL_DB_SIZE_SHIFT),
	},
	{
		.burstwords = 1,
		.reg = (PL080_BSIZE_1 << PL080_CONTROL_SB_SIZE_SHIFT) |
			(PL080_BSIZE_1 << PL080_CONTROL_DB_SIZE_SHIFT),
	},
};

=======
=======
>>>>>>> refs/remotes/origin/master
		.reg = PL080_BSIZE_256,
	},
	{
		.burstwords = 128,
		.reg = PL080_BSIZE_128,
	},
	{
		.burstwords = 64,
		.reg = PL080_BSIZE_64,
	},
	{
		.burstwords = 32,
		.reg = PL080_BSIZE_32,
	},
	{
		.burstwords = 16,
		.reg = PL080_BSIZE_16,
	},
	{
		.burstwords = 8,
		.reg = PL080_BSIZE_8,
	},
	{
		.burstwords = 4,
		.reg = PL080_BSIZE_4,
	},
	{
		.burstwords = 0,
		.reg = PL080_BSIZE_1,
	},
};

/*
 * Given the source and destination available bus masks, select which
 * will be routed to each port.  We try to have source and destination
 * on separate ports, but always respect the allowable settings.
 */
static u32 pl08x_select_bus(u8 src, u8 dst)
{
	u32 cctl = 0;

	if (!(dst & PL08X_AHB1) || ((dst & PL08X_AHB2) && (src & PL08X_AHB1)))
		cctl |= PL080_CONTROL_DST_AHB2;
	if (!(src & PL08X_AHB1) || ((src & PL08X_AHB2) && !(dst & PL08X_AHB2)))
		cctl |= PL080_CONTROL_SRC_AHB2;

	return cctl;
}

static u32 pl08x_cctl(u32 cctl)
{
	cctl &= ~(PL080_CONTROL_SRC_AHB2 | PL080_CONTROL_DST_AHB2 |
		  PL080_CONTROL_SRC_INCR | PL080_CONTROL_DST_INCR |
		  PL080_CONTROL_PROT_MASK);

	/* Access the cell in privileged mode, non-bufferable, non-cacheable */
	return cctl | PL080_CONTROL_PROT_SYS;
}

static u32 pl08x_width(enum dma_slave_buswidth width)
{
	switch (width) {
	case DMA_SLAVE_BUSWIDTH_1_BYTE:
		return PL080_WIDTH_8BIT;
	case DMA_SLAVE_BUSWIDTH_2_BYTES:
		return PL080_WIDTH_16BIT;
	case DMA_SLAVE_BUSWIDTH_4_BYTES:
		return PL080_WIDTH_32BIT;
	default:
		return ~0;
	}
}

static u32 pl08x_burst(u32 maxburst)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(burst_sizes); i++)
		if (burst_sizes[i].burstwords <= maxburst)
			break;

	return burst_sizes[i].reg;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
static int dma_set_runtime_config(struct dma_chan *chan,
				  struct dma_slave_config *config)
{
	struct pl08x_dma_chan *plchan = to_pl08x_chan(chan);
	struct pl08x_driver_data *pl08x = plchan->host;
<<<<<<< HEAD
	struct pl08x_channel_data *cd = plchan->cd;
	enum dma_slave_buswidth addr_width;
	dma_addr_t addr;
	u32 maxburst;
	u32 cctl = 0;
	int i;
=======
	enum dma_slave_buswidth addr_width;
	u32 width, burst, maxburst;
	u32 cctl = 0;
>>>>>>> refs/remotes/origin/cm-10.0

	if (!plchan->slave)
		return -EINVAL;

	/* Transfer direction */
	plchan->runtime_direction = config->direction;
<<<<<<< HEAD
	if (config->direction == DMA_TO_DEVICE) {
		addr = config->dst_addr;
		addr_width = config->dst_addr_width;
		maxburst = config->dst_maxburst;
	} else if (config->direction == DMA_FROM_DEVICE) {
		addr = config->src_addr;
=======
	if (config->direction == DMA_MEM_TO_DEV) {
		addr_width = config->dst_addr_width;
		maxburst = config->dst_maxburst;
	} else if (config->direction == DMA_DEV_TO_MEM) {
>>>>>>> refs/remotes/origin/cm-10.0
		addr_width = config->src_addr_width;
		maxburst = config->src_maxburst;
	} else {
		dev_err(&pl08x->adev->dev,
			"bad runtime_config: alien transfer direction\n");
		return -EINVAL;
	}

<<<<<<< HEAD
	switch (addr_width) {
	case DMA_SLAVE_BUSWIDTH_1_BYTE:
		cctl |= (PL080_WIDTH_8BIT << PL080_CONTROL_SWIDTH_SHIFT) |
			(PL080_WIDTH_8BIT << PL080_CONTROL_DWIDTH_SHIFT);
		break;
	case DMA_SLAVE_BUSWIDTH_2_BYTES:
		cctl |= (PL080_WIDTH_16BIT << PL080_CONTROL_SWIDTH_SHIFT) |
			(PL080_WIDTH_16BIT << PL080_CONTROL_DWIDTH_SHIFT);
		break;
	case DMA_SLAVE_BUSWIDTH_4_BYTES:
		cctl |= (PL080_WIDTH_32BIT << PL080_CONTROL_SWIDTH_SHIFT) |
			(PL080_WIDTH_32BIT << PL080_CONTROL_DWIDTH_SHIFT);
		break;
	default:
=======
	width = pl08x_width(addr_width);
	if (width == ~0) {
>>>>>>> refs/remotes/origin/cm-10.0
		dev_err(&pl08x->adev->dev,
			"bad runtime_config: alien address width\n");
		return -EINVAL;
	}

<<<<<<< HEAD
	/*
	 * Now decide on a maxburst:
=======
=======
static u32 pl08x_get_cctl(struct pl08x_dma_chan *plchan,
	enum dma_slave_buswidth addr_width, u32 maxburst)
{
	u32 width, burst, cctl = 0;

	width = pl08x_width(addr_width);
	if (width == ~0)
		return ~0;

>>>>>>> refs/remotes/origin/master
	cctl |= width << PL080_CONTROL_SWIDTH_SHIFT;
	cctl |= width << PL080_CONTROL_DWIDTH_SHIFT;

	/*
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	 * If this channel will only request single transfers, set this
	 * down to ONE element.  Also select one element if no maxburst
	 * is specified.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (plchan->cd->single || maxburst == 0) {
		cctl |= (PL080_BSIZE_1 << PL080_CONTROL_SB_SIZE_SHIFT) |
			(PL080_BSIZE_1 << PL080_CONTROL_DB_SIZE_SHIFT);
	} else {
		for (i = 0; i < ARRAY_SIZE(burst_sizes); i++)
			if (burst_sizes[i].burstwords <= maxburst)
				break;
		cctl |= burst_sizes[i].reg;
	}

	plchan->runtime_addr = addr;

	/* Modify the default channel data to fit PrimeCell request */
	cd->cctl = cctl;
=======
=======
>>>>>>> refs/remotes/origin/master
	if (plchan->cd->single)
		maxburst = 1;

	burst = pl08x_burst(maxburst);
	cctl |= burst << PL080_CONTROL_SB_SIZE_SHIFT;
	cctl |= burst << PL080_CONTROL_DB_SIZE_SHIFT;

<<<<<<< HEAD
	plchan->device_fc = config->device_fc;

	if (plchan->runtime_direction == DMA_DEV_TO_MEM) {
		plchan->src_addr = config->src_addr;
		plchan->src_cctl = pl08x_cctl(cctl) | PL080_CONTROL_DST_INCR |
			pl08x_select_bus(plchan->cd->periph_buses,
					 pl08x->mem_buses);
	} else {
		plchan->dst_addr = config->dst_addr;
		plchan->dst_cctl = pl08x_cctl(cctl) | PL080_CONTROL_SRC_INCR |
			pl08x_select_bus(pl08x->mem_buses,
					 plchan->cd->periph_buses);
	}
>>>>>>> refs/remotes/origin/cm-10.0

	dev_dbg(&pl08x->adev->dev,
		"configured channel %s (%s) for %s, data width %d, "
		"maxburst %d words, LE, CCTL=0x%08x\n",
		dma_chan_name(chan), plchan->name,
<<<<<<< HEAD
		(config->direction == DMA_FROM_DEVICE) ? "RX" : "TX",
=======
		(config->direction == DMA_DEV_TO_MEM) ? "RX" : "TX",
>>>>>>> refs/remotes/origin/cm-10.0
		addr_width,
		maxburst,
		cctl);

	return 0;
}

/*
 * Slave transactions callback to the slave device to allow
 * synchronization of slave DMA signals with the DMAC enable
 */
static void pl08x_issue_pending(struct dma_chan *chan)
{
	struct pl08x_dma_chan *plchan = to_pl08x_chan(chan);
	unsigned long flags;

	spin_lock_irqsave(&plchan->lock, flags);
	/* Something is already active, or we're waiting for a channel... */
	if (plchan->at || plchan->state == PL08X_CHAN_WAITING) {
		spin_unlock_irqrestore(&plchan->lock, flags);
		return;
	}

	/* Take the first element in the queue and execute it */
	if (!list_empty(&plchan->pend_list)) {
		struct pl08x_txd *next;

		next = list_first_entry(&plchan->pend_list,
					struct pl08x_txd,
					node);
		list_del(&next->node);
		plchan->state = PL08X_CHAN_RUNNING;

		pl08x_start_txd(plchan, next);
	}

	spin_unlock_irqrestore(&plchan->lock, flags);
}

static int pl08x_prep_channel_resources(struct pl08x_dma_chan *plchan,
					struct pl08x_txd *txd)
{
	struct pl08x_driver_data *pl08x = plchan->host;
	unsigned long flags;
	int num_llis, ret;

	num_llis = pl08x_fill_llis_for_desc(pl08x, txd);
	if (!num_llis) {
<<<<<<< HEAD
		kfree(txd);
=======
		spin_lock_irqsave(&plchan->lock, flags);
		pl08x_free_txd(pl08x, txd);
		spin_unlock_irqrestore(&plchan->lock, flags);
>>>>>>> refs/remotes/origin/cm-10.0
		return -EINVAL;
	}

	spin_lock_irqsave(&plchan->lock, flags);

	/*
	 * See if we already have a physical channel allocated,
	 * else this is the time to try to get one.
	 */
	ret = prep_phy_channel(plchan, txd);
	if (ret) {
		/*
		 * No physical channel was available.
		 *
		 * memcpy transfers can be sorted out at submission time.
		 *
		 * Slave transfers may have been denied due to platform
		 * channel muxing restrictions.  Since there is no guarantee
		 * that this will ever be resolved, and the signal must be
		 * acquired AFTER acquiring the physical channel, we will let
		 * them be NACK:ed with -EBUSY here. The drivers can retry
		 * the prep() call if they are eager on doing this using DMA.
		 */
		if (plchan->slave) {
			pl08x_free_txd_list(pl08x, plchan);
			pl08x_free_txd(pl08x, txd);
			spin_unlock_irqrestore(&plchan->lock, flags);
			return -EBUSY;
		}
	} else
		/*
		 * Else we're all set, paused and ready to roll, status
		 * will switch to PL08X_CHAN_RUNNING when we call
		 * issue_pending(). If there is something running on the
		 * channel already we don't change its state.
		 */
		if (plchan->state == PL08X_CHAN_IDLE)
			plchan->state = PL08X_CHAN_PAUSED;

	spin_unlock_irqrestore(&plchan->lock, flags);
=======
	return pl08x_cctl(cctl);
}

static int dma_set_runtime_config(struct dma_chan *chan,
				  struct dma_slave_config *config)
{
	struct pl08x_dma_chan *plchan = to_pl08x_chan(chan);
	struct pl08x_driver_data *pl08x = plchan->host;

	if (!plchan->slave)
		return -EINVAL;

	/* Reject definitely invalid configurations */
	if (config->src_addr_width == DMA_SLAVE_BUSWIDTH_8_BYTES ||
	    config->dst_addr_width == DMA_SLAVE_BUSWIDTH_8_BYTES)
		return -EINVAL;

	if (config->device_fc && pl08x->vd->pl080s) {
		dev_err(&pl08x->adev->dev,
			"%s: PL080S does not support peripheral flow control\n",
			__func__);
		return -EINVAL;
	}

	plchan->cfg = *config;
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
/*
 * Given the source and destination available bus masks, select which
 * will be routed to each port.  We try to have source and destination
 * on separate ports, but always respect the allowable settings.
 */
static u32 pl08x_select_bus(struct pl08x_driver_data *pl08x, u8 src, u8 dst)
{
	u32 cctl = 0;

	if (!(dst & PL08X_AHB1) || ((dst & PL08X_AHB2) && (src & PL08X_AHB1)))
		cctl |= PL080_CONTROL_DST_AHB2;
	if (!(src & PL08X_AHB1) || ((src & PL08X_AHB2) && !(dst & PL08X_AHB2)))
		cctl |= PL080_CONTROL_SRC_AHB2;

	return cctl;
}

static struct pl08x_txd *pl08x_get_txd(struct pl08x_dma_chan *plchan,
	unsigned long flags)
{
	struct pl08x_txd *txd = kzalloc(sizeof(struct pl08x_txd), GFP_NOWAIT);
=======
static struct pl08x_txd *pl08x_get_txd(struct pl08x_dma_chan *plchan,
	unsigned long flags)
{
	struct pl08x_txd *txd = kzalloc(sizeof(*txd), GFP_NOWAIT);
>>>>>>> refs/remotes/origin/cm-10.0

	if (txd) {
		dma_async_tx_descriptor_init(&txd->tx, &plchan->chan);
		txd->tx.flags = flags;
		txd->tx.tx_submit = pl08x_tx_submit;
		INIT_LIST_HEAD(&txd->node);
<<<<<<< HEAD
=======
		INIT_LIST_HEAD(&txd->dsg_list);
>>>>>>> refs/remotes/origin/cm-10.0
=======
/*
 * Slave transactions callback to the slave device to allow
 * synchronization of slave DMA signals with the DMAC enable
 */
static void pl08x_issue_pending(struct dma_chan *chan)
{
	struct pl08x_dma_chan *plchan = to_pl08x_chan(chan);
	unsigned long flags;

	spin_lock_irqsave(&plchan->vc.lock, flags);
	if (vchan_issue_pending(&plchan->vc)) {
		if (!plchan->phychan && plchan->state != PL08X_CHAN_WAITING)
			pl08x_phy_alloc_and_start(plchan);
	}
	spin_unlock_irqrestore(&plchan->vc.lock, flags);
}

static struct pl08x_txd *pl08x_get_txd(struct pl08x_dma_chan *plchan)
{
	struct pl08x_txd *txd = kzalloc(sizeof(*txd), GFP_NOWAIT);

	if (txd) {
		INIT_LIST_HEAD(&txd->dsg_list);
>>>>>>> refs/remotes/origin/master

		/* Always enable error and terminal interrupts */
		txd->ccfg = PL080_CONFIG_ERR_IRQ_MASK |
			    PL080_CONFIG_TC_IRQ_MASK;
	}
	return txd;
}

/*
 * Initialize a descriptor to be used by memcpy submit
 */
static struct dma_async_tx_descriptor *pl08x_prep_dma_memcpy(
		struct dma_chan *chan, dma_addr_t dest, dma_addr_t src,
		size_t len, unsigned long flags)
{
	struct pl08x_dma_chan *plchan = to_pl08x_chan(chan);
	struct pl08x_driver_data *pl08x = plchan->host;
	struct pl08x_txd *txd;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct pl08x_sg *dsg;
>>>>>>> refs/remotes/origin/cm-10.0
	int ret;

	txd = pl08x_get_txd(plchan, flags);
=======
	struct pl08x_sg *dsg;
	int ret;

	txd = pl08x_get_txd(plchan);
>>>>>>> refs/remotes/origin/master
	if (!txd) {
		dev_err(&pl08x->adev->dev,
			"%s no memory for descriptor\n", __func__);
		return NULL;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	txd->direction = DMA_NONE;
	txd->src_addr = src;
	txd->dst_addr = dest;
	txd->len = len;
=======
=======
>>>>>>> refs/remotes/origin/master
	dsg = kzalloc(sizeof(struct pl08x_sg), GFP_NOWAIT);
	if (!dsg) {
		pl08x_free_txd(pl08x, txd);
		dev_err(&pl08x->adev->dev, "%s no memory for pl080 sg\n",
				__func__);
		return NULL;
	}
	list_add_tail(&dsg->node, &txd->dsg_list);

<<<<<<< HEAD
	txd->direction = DMA_NONE;
	dsg->src_addr = src;
	dsg->dst_addr = dest;
	dsg->len = len;
>>>>>>> refs/remotes/origin/cm-10.0

	/* Set platform data for m2m */
	txd->ccfg |= PL080_FLOW_MEM2MEM << PL080_CONFIG_FLOW_CONTROL_SHIFT;
	txd->cctl = pl08x->pd->memcpy_channel.cctl &
=======
	dsg->src_addr = src;
	dsg->dst_addr = dest;
	dsg->len = len;

	/* Set platform data for m2m */
	txd->ccfg |= PL080_FLOW_MEM2MEM << PL080_CONFIG_FLOW_CONTROL_SHIFT;
	txd->cctl = pl08x->pd->memcpy_channel.cctl_memcpy &
>>>>>>> refs/remotes/origin/master
			~(PL080_CONTROL_DST_AHB2 | PL080_CONTROL_SRC_AHB2);

	/* Both to be incremented or the code will break */
	txd->cctl |= PL080_CONTROL_SRC_INCR | PL080_CONTROL_DST_INCR;

	if (pl08x->vd->dualmaster)
<<<<<<< HEAD
<<<<<<< HEAD
		txd->cctl |= pl08x_select_bus(pl08x,
					pl08x->mem_buses, pl08x->mem_buses);
=======
		txd->cctl |= pl08x_select_bus(pl08x->mem_buses,
					      pl08x->mem_buses);
>>>>>>> refs/remotes/origin/cm-10.0

	ret = pl08x_prep_channel_resources(plchan, txd);
	if (ret)
		return NULL;

	return &txd->tx;
}

static struct dma_async_tx_descriptor *pl08x_prep_slave_sg(
		struct dma_chan *chan, struct scatterlist *sgl,
<<<<<<< HEAD
		unsigned int sg_len, enum dma_data_direction direction,
		unsigned long flags)
=======
		unsigned int sg_len, enum dma_transfer_direction direction,
		unsigned long flags, void *context)
>>>>>>> refs/remotes/origin/cm-10.0
=======
		txd->cctl |= pl08x_select_bus(pl08x->mem_buses,
					      pl08x->mem_buses);

	ret = pl08x_fill_llis_for_desc(plchan->host, txd);
	if (!ret) {
		pl08x_free_txd(pl08x, txd);
		return NULL;
	}

	return vchan_tx_prep(&plchan->vc, &txd->vd, flags);
}

static struct pl08x_txd *pl08x_init_txd(
		struct dma_chan *chan,
		enum dma_transfer_direction direction,
		dma_addr_t *slave_addr)
>>>>>>> refs/remotes/origin/master
{
	struct pl08x_dma_chan *plchan = to_pl08x_chan(chan);
	struct pl08x_driver_data *pl08x = plchan->host;
	struct pl08x_txd *txd;
<<<<<<< HEAD
<<<<<<< HEAD
	u8 src_buses, dst_buses;
	int ret;

	/*
	 * Current implementation ASSUMES only one sg
	 */
	if (sg_len != 1) {
		dev_err(&pl08x->adev->dev, "%s prepared too long sglist\n",
			__func__);
		BUG();
	}

	dev_dbg(&pl08x->adev->dev, "%s prepare transaction of %d bytes from %s\n",
		__func__, sgl->length, plchan->name);
=======
	struct pl08x_sg *dsg;
	struct scatterlist *sg;
	dma_addr_t slave_addr;
	int ret, tmp;

	dev_dbg(&pl08x->adev->dev, "%s prepare transaction of %d bytes from %s\n",
			__func__, sgl->length, plchan->name);
>>>>>>> refs/remotes/origin/cm-10.0

	txd = pl08x_get_txd(plchan, flags);
=======
	enum dma_slave_buswidth addr_width;
	int ret, tmp;
	u8 src_buses, dst_buses;
	u32 maxburst, cctl;

	txd = pl08x_get_txd(plchan);
>>>>>>> refs/remotes/origin/master
	if (!txd) {
		dev_err(&pl08x->adev->dev, "%s no txd\n", __func__);
		return NULL;
	}

<<<<<<< HEAD
	if (direction != plchan->runtime_direction)
		dev_err(&pl08x->adev->dev, "%s DMA setup does not match "
			"the direction configured for the PrimeCell\n",
			__func__);

=======
>>>>>>> refs/remotes/origin/master
	/*
	 * Set up addresses, the PrimeCell configured address
	 * will take precedence since this may configure the
	 * channel target address dynamically at runtime.
	 */
<<<<<<< HEAD
	txd->direction = direction;
<<<<<<< HEAD
	txd->len = sgl->length;

	txd->cctl = plchan->cd->cctl &
			~(PL080_CONTROL_SRC_AHB2 | PL080_CONTROL_DST_AHB2 |
			  PL080_CONTROL_SRC_INCR | PL080_CONTROL_DST_INCR |
			  PL080_CONTROL_PROT_MASK);

	/* Access the cell in privileged mode, non-bufferable, non-cacheable */
	txd->cctl |= PL080_CONTROL_PROT_SYS;

	if (direction == DMA_TO_DEVICE) {
		txd->ccfg |= PL080_FLOW_MEM2PER << PL080_CONFIG_FLOW_CONTROL_SHIFT;
		txd->cctl |= PL080_CONTROL_SRC_INCR;
		txd->src_addr = sgl->dma_address;
		if (plchan->runtime_addr)
			txd->dst_addr = plchan->runtime_addr;
		else
			txd->dst_addr = plchan->cd->addr;
		src_buses = pl08x->mem_buses;
		dst_buses = plchan->cd->periph_buses;
	} else if (direction == DMA_FROM_DEVICE) {
		txd->ccfg |= PL080_FLOW_PER2MEM << PL080_CONFIG_FLOW_CONTROL_SHIFT;
		txd->cctl |= PL080_CONTROL_DST_INCR;
		if (plchan->runtime_addr)
			txd->src_addr = plchan->runtime_addr;
		else
			txd->src_addr = plchan->cd->addr;
		txd->dst_addr = sgl->dma_address;
		src_buses = plchan->cd->periph_buses;
		dst_buses = pl08x->mem_buses;
	} else {
=======

	if (direction == DMA_MEM_TO_DEV) {
		txd->cctl = plchan->dst_cctl;
		slave_addr = plchan->dst_addr;
	} else if (direction == DMA_DEV_TO_MEM) {
		txd->cctl = plchan->src_cctl;
		slave_addr = plchan->src_addr;
	} else {
		pl08x_free_txd(pl08x, txd);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (direction == DMA_MEM_TO_DEV) {
		cctl = PL080_CONTROL_SRC_INCR;
		*slave_addr = plchan->cfg.dst_addr;
		addr_width = plchan->cfg.dst_addr_width;
		maxburst = plchan->cfg.dst_maxburst;
		src_buses = pl08x->mem_buses;
		dst_buses = plchan->cd->periph_buses;
	} else if (direction == DMA_DEV_TO_MEM) {
		cctl = PL080_CONTROL_DST_INCR;
		*slave_addr = plchan->cfg.src_addr;
		addr_width = plchan->cfg.src_addr_width;
		maxburst = plchan->cfg.src_maxburst;
		src_buses = plchan->cd->periph_buses;
		dst_buses = pl08x->mem_buses;
	} else {
		pl08x_free_txd(pl08x, txd);
>>>>>>> refs/remotes/origin/master
		dev_err(&pl08x->adev->dev,
			"%s direction unsupported\n", __func__);
		return NULL;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	txd->cctl |= pl08x_select_bus(pl08x, src_buses, dst_buses);
=======
	if (plchan->device_fc)
=======
	cctl |= pl08x_get_cctl(plchan, addr_width, maxburst);
	if (cctl == ~0) {
		pl08x_free_txd(pl08x, txd);
		dev_err(&pl08x->adev->dev,
			"DMA slave configuration botched?\n");
		return NULL;
	}

	txd->cctl = cctl | pl08x_select_bus(src_buses, dst_buses);

	if (plchan->cfg.device_fc)
>>>>>>> refs/remotes/origin/master
		tmp = (direction == DMA_MEM_TO_DEV) ? PL080_FLOW_MEM2PER_PER :
			PL080_FLOW_PER2MEM_PER;
	else
		tmp = (direction == DMA_MEM_TO_DEV) ? PL080_FLOW_MEM2PER :
			PL080_FLOW_PER2MEM;

	txd->ccfg |= tmp << PL080_CONFIG_FLOW_CONTROL_SHIFT;

<<<<<<< HEAD
	for_each_sg(sgl, sg, sg_len, tmp) {
		dsg = kzalloc(sizeof(struct pl08x_sg), GFP_NOWAIT);
		if (!dsg) {
=======
	ret = pl08x_request_mux(plchan);
	if (ret < 0) {
		pl08x_free_txd(pl08x, txd);
		dev_dbg(&pl08x->adev->dev,
			"unable to mux for transfer on %s due to platform restrictions\n",
			plchan->name);
		return NULL;
	}

	dev_dbg(&pl08x->adev->dev, "allocated DMA request signal %d for xfer on %s\n",
		 plchan->signal, plchan->name);

	/* Assign the flow control signal to this channel */
	if (direction == DMA_MEM_TO_DEV)
		txd->ccfg |= plchan->signal << PL080_CONFIG_DST_SEL_SHIFT;
	else
		txd->ccfg |= plchan->signal << PL080_CONFIG_SRC_SEL_SHIFT;

	return txd;
}

static int pl08x_tx_add_sg(struct pl08x_txd *txd,
			   enum dma_transfer_direction direction,
			   dma_addr_t slave_addr,
			   dma_addr_t buf_addr,
			   unsigned int len)
{
	struct pl08x_sg *dsg;

	dsg = kzalloc(sizeof(struct pl08x_sg), GFP_NOWAIT);
	if (!dsg)
		return -ENOMEM;

	list_add_tail(&dsg->node, &txd->dsg_list);

	dsg->len = len;
	if (direction == DMA_MEM_TO_DEV) {
		dsg->src_addr = buf_addr;
		dsg->dst_addr = slave_addr;
	} else {
		dsg->src_addr = slave_addr;
		dsg->dst_addr = buf_addr;
	}

	return 0;
}

static struct dma_async_tx_descriptor *pl08x_prep_slave_sg(
		struct dma_chan *chan, struct scatterlist *sgl,
		unsigned int sg_len, enum dma_transfer_direction direction,
		unsigned long flags, void *context)
{
	struct pl08x_dma_chan *plchan = to_pl08x_chan(chan);
	struct pl08x_driver_data *pl08x = plchan->host;
	struct pl08x_txd *txd;
	struct scatterlist *sg;
	int ret, tmp;
	dma_addr_t slave_addr;

	dev_dbg(&pl08x->adev->dev, "%s prepare transaction of %d bytes from %s\n",
			__func__, sg_dma_len(sgl), plchan->name);

	txd = pl08x_init_txd(chan, direction, &slave_addr);
	if (!txd)
		return NULL;

	for_each_sg(sgl, sg, sg_len, tmp) {
		ret = pl08x_tx_add_sg(txd, direction, slave_addr,
				      sg_dma_address(sg),
				      sg_dma_len(sg));
		if (ret) {
			pl08x_release_mux(plchan);
>>>>>>> refs/remotes/origin/master
			pl08x_free_txd(pl08x, txd);
			dev_err(&pl08x->adev->dev, "%s no mem for pl080 sg\n",
					__func__);
			return NULL;
		}
<<<<<<< HEAD
		list_add_tail(&dsg->node, &txd->dsg_list);

		dsg->len = sg_dma_len(sg);
		if (direction == DMA_MEM_TO_DEV) {
			dsg->src_addr = sg_phys(sg);
			dsg->dst_addr = slave_addr;
		} else {
			dsg->src_addr = slave_addr;
			dsg->dst_addr = sg_phys(sg);
		}
	}
>>>>>>> refs/remotes/origin/cm-10.0

	ret = pl08x_prep_channel_resources(plchan, txd);
	if (ret)
		return NULL;

	return &txd->tx;
=======
	}

	ret = pl08x_fill_llis_for_desc(plchan->host, txd);
	if (!ret) {
		pl08x_release_mux(plchan);
		pl08x_free_txd(pl08x, txd);
		return NULL;
	}

	return vchan_tx_prep(&plchan->vc, &txd->vd, flags);
}

static struct dma_async_tx_descriptor *pl08x_prep_dma_cyclic(
		struct dma_chan *chan, dma_addr_t buf_addr, size_t buf_len,
		size_t period_len, enum dma_transfer_direction direction,
		unsigned long flags, void *context)
{
	struct pl08x_dma_chan *plchan = to_pl08x_chan(chan);
	struct pl08x_driver_data *pl08x = plchan->host;
	struct pl08x_txd *txd;
	int ret, tmp;
	dma_addr_t slave_addr;

	dev_dbg(&pl08x->adev->dev,
		"%s prepare cyclic transaction of %d/%d bytes %s %s\n",
		__func__, period_len, buf_len,
		direction == DMA_MEM_TO_DEV ? "to" : "from",
		plchan->name);

	txd = pl08x_init_txd(chan, direction, &slave_addr);
	if (!txd)
		return NULL;

	txd->cyclic = true;
	txd->cctl |= PL080_CONTROL_TC_IRQ_EN;
	for (tmp = 0; tmp < buf_len; tmp += period_len) {
		ret = pl08x_tx_add_sg(txd, direction, slave_addr,
				      buf_addr + tmp, period_len);
		if (ret) {
			pl08x_release_mux(plchan);
			pl08x_free_txd(pl08x, txd);
			return NULL;
		}
	}

	ret = pl08x_fill_llis_for_desc(plchan->host, txd);
	if (!ret) {
		pl08x_release_mux(plchan);
		pl08x_free_txd(pl08x, txd);
		return NULL;
	}

	return vchan_tx_prep(&plchan->vc, &txd->vd, flags);
>>>>>>> refs/remotes/origin/master
}

static int pl08x_control(struct dma_chan *chan, enum dma_ctrl_cmd cmd,
			 unsigned long arg)
{
	struct pl08x_dma_chan *plchan = to_pl08x_chan(chan);
	struct pl08x_driver_data *pl08x = plchan->host;
	unsigned long flags;
	int ret = 0;

	/* Controls applicable to inactive channels */
	if (cmd == DMA_SLAVE_CONFIG) {
		return dma_set_runtime_config(chan,
					      (struct dma_slave_config *)arg);
	}

	/*
	 * Anything succeeds on channels with no physical allocation and
	 * no queued transfers.
	 */
<<<<<<< HEAD
	spin_lock_irqsave(&plchan->lock, flags);
	if (!plchan->phychan && !plchan->at) {
		spin_unlock_irqrestore(&plchan->lock, flags);
=======
	spin_lock_irqsave(&plchan->vc.lock, flags);
	if (!plchan->phychan && !plchan->at) {
		spin_unlock_irqrestore(&plchan->vc.lock, flags);
>>>>>>> refs/remotes/origin/master
		return 0;
	}

	switch (cmd) {
	case DMA_TERMINATE_ALL:
		plchan->state = PL08X_CHAN_IDLE;

		if (plchan->phychan) {
<<<<<<< HEAD
			pl08x_terminate_phy_chan(pl08x, plchan->phychan);

=======
>>>>>>> refs/remotes/origin/master
			/*
			 * Mark physical channel as free and free any slave
			 * signal
			 */
<<<<<<< HEAD
			release_phy_channel(plchan);
<<<<<<< HEAD
=======
			plchan->phychan_hold = 0;
>>>>>>> refs/remotes/origin/cm-10.0
		}
		/* Dequeue jobs and free LLIs */
		if (plchan->at) {
			pl08x_free_txd(pl08x, plchan->at);
=======
			pl08x_phy_free(plchan);
		}
		/* Dequeue jobs and free LLIs */
		if (plchan->at) {
			pl08x_desc_free(&plchan->at->vd);
>>>>>>> refs/remotes/origin/master
			plchan->at = NULL;
		}
		/* Dequeue jobs not yet fired as well */
		pl08x_free_txd_list(pl08x, plchan);
		break;
	case DMA_PAUSE:
		pl08x_pause_phy_chan(plchan->phychan);
		plchan->state = PL08X_CHAN_PAUSED;
		break;
	case DMA_RESUME:
		pl08x_resume_phy_chan(plchan->phychan);
		plchan->state = PL08X_CHAN_RUNNING;
		break;
	default:
		/* Unknown command */
		ret = -ENXIO;
		break;
	}

<<<<<<< HEAD
	spin_unlock_irqrestore(&plchan->lock, flags);
=======
	spin_unlock_irqrestore(&plchan->vc.lock, flags);
>>>>>>> refs/remotes/origin/master

	return ret;
}

bool pl08x_filter_id(struct dma_chan *chan, void *chan_id)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct pl08x_dma_chan *plchan = to_pl08x_chan(chan);
	char *name = chan_id;

=======
=======
>>>>>>> refs/remotes/origin/master
	struct pl08x_dma_chan *plchan;
	char *name = chan_id;

	/* Reject channels for devices not bound to this driver */
	if (chan->device->dev->driver != &pl08x_amba_driver.drv)
		return false;

	plchan = to_pl08x_chan(chan);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	/* Check that the channel is not taken! */
	if (!strcmp(plchan->name, name))
		return true;

	return false;
}

/*
 * Just check that the device is there and active
 * TODO: turn this bit on/off depending on the number of physical channels
 * actually used, if it is zero... well shut it off. That will save some
 * power. Cut the clock at the same time.
 */
static void pl08x_ensure_on(struct pl08x_driver_data *pl08x)
{
<<<<<<< HEAD
<<<<<<< HEAD
	u32 val;

	val = readl(pl08x->base + PL080_CONFIG);
	val &= ~(PL080_CONFIG_M2_BE | PL080_CONFIG_M1_BE | PL080_CONFIG_ENABLE);
	/* We implicitly clear bit 1 and that means little-endian mode */
	val |= PL080_CONFIG_ENABLE;
	writel(val, pl08x->base + PL080_CONFIG);
=======
	writel(PL080_CONFIG_ENABLE, pl08x->base + PL080_CONFIG);
>>>>>>> refs/remotes/origin/cm-10.0
}

static void pl08x_unmap_buffers(struct pl08x_txd *txd)
{
	struct device *dev = txd->tx.chan->device->dev;
<<<<<<< HEAD

	if (!(txd->tx.flags & DMA_COMPL_SKIP_SRC_UNMAP)) {
		if (txd->tx.flags & DMA_COMPL_SRC_UNMAP_SINGLE)
			dma_unmap_single(dev, txd->src_addr, txd->len,
				DMA_TO_DEVICE);
		else
			dma_unmap_page(dev, txd->src_addr, txd->len,
				DMA_TO_DEVICE);
	}
	if (!(txd->tx.flags & DMA_COMPL_SKIP_DEST_UNMAP)) {
		if (txd->tx.flags & DMA_COMPL_DEST_UNMAP_SINGLE)
			dma_unmap_single(dev, txd->dst_addr, txd->len,
				DMA_FROM_DEVICE);
		else
			dma_unmap_page(dev, txd->dst_addr, txd->len,
				DMA_FROM_DEVICE);
=======
	struct pl08x_sg *dsg;

	if (!(txd->tx.flags & DMA_COMPL_SKIP_SRC_UNMAP)) {
		if (txd->tx.flags & DMA_COMPL_SRC_UNMAP_SINGLE)
			list_for_each_entry(dsg, &txd->dsg_list, node)
				dma_unmap_single(dev, dsg->src_addr, dsg->len,
						DMA_TO_DEVICE);
		else {
			list_for_each_entry(dsg, &txd->dsg_list, node)
				dma_unmap_page(dev, dsg->src_addr, dsg->len,
						DMA_TO_DEVICE);
		}
	}
	if (!(txd->tx.flags & DMA_COMPL_SKIP_DEST_UNMAP)) {
		if (txd->tx.flags & DMA_COMPL_DEST_UNMAP_SINGLE)
			list_for_each_entry(dsg, &txd->dsg_list, node)
				dma_unmap_single(dev, dsg->dst_addr, dsg->len,
						DMA_FROM_DEVICE);
		else
			list_for_each_entry(dsg, &txd->dsg_list, node)
				dma_unmap_page(dev, dsg->dst_addr, dsg->len,
						DMA_FROM_DEVICE);
>>>>>>> refs/remotes/origin/cm-10.0
	}
}

static void pl08x_tasklet(unsigned long data)
{
	struct pl08x_dma_chan *plchan = (struct pl08x_dma_chan *) data;
	struct pl08x_driver_data *pl08x = plchan->host;
	struct pl08x_txd *txd;
	unsigned long flags;

	spin_lock_irqsave(&plchan->lock, flags);

	txd = plchan->at;
	plchan->at = NULL;

	if (txd) {
		/* Update last completed */
<<<<<<< HEAD
		plchan->lc = txd->tx.cookie;
=======
		dma_cookie_complete(&txd->tx);
>>>>>>> refs/remotes/origin/cm-10.0
	}

	/* If a new descriptor is queued, set it up plchan->at is NULL here */
	if (!list_empty(&plchan->pend_list)) {
		struct pl08x_txd *next;

		next = list_first_entry(&plchan->pend_list,
					struct pl08x_txd,
					node);
		list_del(&next->node);

		pl08x_start_txd(plchan, next);
	} else if (plchan->phychan_hold) {
		/*
		 * This channel is still in use - we have a new txd being
		 * prepared and will soon be queued.  Don't give up the
		 * physical channel.
		 */
	} else {
		struct pl08x_dma_chan *waiting = NULL;

		/*
		 * No more jobs, so free up the physical channel
		 * Free any allocated signal on slave transfers too
		 */
		release_phy_channel(plchan);
		plchan->state = PL08X_CHAN_IDLE;

		/*
		 * And NOW before anyone else can grab that free:d up
		 * physical channel, see if there is some memcpy pending
		 * that seriously needs to start because of being stacked
		 * up while we were choking the physical channels with data.
		 */
		list_for_each_entry(waiting, &pl08x->memcpy.channels,
				    chan.device_node) {
<<<<<<< HEAD
		  if (waiting->state == PL08X_CHAN_WAITING &&
			    waiting->waiting != NULL) {
=======
			if (waiting->state == PL08X_CHAN_WAITING &&
				waiting->waiting != NULL) {
>>>>>>> refs/remotes/origin/cm-10.0
				int ret;

				/* This should REALLY not fail now */
				ret = prep_phy_channel(waiting,
						       waiting->waiting);
				BUG_ON(ret);
				waiting->phychan_hold--;
				waiting->state = PL08X_CHAN_RUNNING;
				waiting->waiting = NULL;
				pl08x_issue_pending(&waiting->chan);
				break;
			}
		}
	}

	spin_unlock_irqrestore(&plchan->lock, flags);

	if (txd) {
		dma_async_tx_callback callback = txd->tx.callback;
		void *callback_param = txd->tx.callback_param;

		/* Don't try to unmap buffers on slave channels */
		if (!plchan->slave)
			pl08x_unmap_buffers(txd);

		/* Free the descriptor */
		spin_lock_irqsave(&plchan->lock, flags);
		pl08x_free_txd(pl08x, txd);
		spin_unlock_irqrestore(&plchan->lock, flags);

		/* Callback to signal completion */
		if (callback)
			callback(callback_param);
	}
=======
	/* The Nomadik variant does not have the config register */
	if (pl08x->vd->nomadik)
		return;
	writel(PL080_CONFIG_ENABLE, pl08x->base + PL080_CONFIG);
>>>>>>> refs/remotes/origin/master
}

static irqreturn_t pl08x_irq(int irq, void *dev)
{
	struct pl08x_driver_data *pl08x = dev;
<<<<<<< HEAD
<<<<<<< HEAD
	u32 mask = 0;
	u32 val;
	int i;

	val = readl(pl08x->base + PL080_ERR_STATUS);
	if (val) {
		/* An error interrupt (on one or more channels) */
		dev_err(&pl08x->adev->dev,
			"%s error interrupt, register value 0x%08x\n",
				__func__, val);
		/*
		 * Simply clear ALL PL08X error interrupts,
		 * regardless of channel and cause
		 * FIXME: should be 0x00000003 on PL081 really.
		 */
		writel(0x000000FF, pl08x->base + PL080_ERR_CLEAR);
	}
	val = readl(pl08x->base + PL080_INT_STATUS);
	for (i = 0; i < pl08x->vd->channels; i++) {
		if ((1 << i) & val) {
=======
=======
>>>>>>> refs/remotes/origin/master
	u32 mask = 0, err, tc, i;

	/* check & clear - ERR & TC interrupts */
	err = readl(pl08x->base + PL080_ERR_STATUS);
	if (err) {
		dev_err(&pl08x->adev->dev, "%s error interrupt, register value 0x%08x\n",
			__func__, err);
		writel(err, pl08x->base + PL080_ERR_CLEAR);
	}
<<<<<<< HEAD
	tc = readl(pl08x->base + PL080_INT_STATUS);
=======
	tc = readl(pl08x->base + PL080_TC_STATUS);
>>>>>>> refs/remotes/origin/master
	if (tc)
		writel(tc, pl08x->base + PL080_TC_CLEAR);

	if (!err && !tc)
		return IRQ_NONE;

	for (i = 0; i < pl08x->vd->channels; i++) {
		if (((1 << i) & err) || ((1 << i) & tc)) {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
			/* Locate physical channel */
			struct pl08x_phy_chan *phychan = &pl08x->phy_chans[i];
			struct pl08x_dma_chan *plchan = phychan->serving;

<<<<<<< HEAD
			/* Schedule tasklet on this channel */
			tasklet_schedule(&plchan->tasklet);

			mask |= (1 << i);
		}
	}
	/* Clear only the terminal interrupts on channels we processed */
	writel(mask, pl08x->base + PL080_TC_CLEAR);
=======
=======
			/* Locate physical channel */
			struct pl08x_phy_chan *phychan = &pl08x->phy_chans[i];
			struct pl08x_dma_chan *plchan = phychan->serving;
			struct pl08x_txd *tx;

>>>>>>> refs/remotes/origin/master
			if (!plchan) {
				dev_err(&pl08x->adev->dev,
					"%s Error TC interrupt on unused channel: 0x%08x\n",
					__func__, i);
				continue;
			}

<<<<<<< HEAD
			/* Schedule tasklet on this channel */
			tasklet_schedule(&plchan->tasklet);
			mask |= (1 << i);
		}
	}
>>>>>>> refs/remotes/origin/cm-10.0
=======
			spin_lock(&plchan->vc.lock);
			tx = plchan->at;
			if (tx && tx->cyclic) {
				vchan_cyclic_callback(&tx->vd);
			} else if (tx) {
				plchan->at = NULL;
				/*
				 * This descriptor is done, release its mux
				 * reservation.
				 */
				pl08x_release_mux(plchan);
				tx->done = true;
				vchan_cookie_complete(&tx->vd);

				/*
				 * And start the next descriptor (if any),
				 * otherwise free this channel.
				 */
				if (vchan_next_desc(&plchan->vc))
					pl08x_start_next_txd(plchan);
				else
					pl08x_phy_free(plchan);
			}
			spin_unlock(&plchan->vc.lock);

			mask |= (1 << i);
		}
	}
>>>>>>> refs/remotes/origin/master

	return mask ? IRQ_HANDLED : IRQ_NONE;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
static void pl08x_dma_slave_init(struct pl08x_dma_chan *chan)
{
	u32 cctl = pl08x_cctl(chan->cd->cctl);

	chan->slave = true;
	chan->name = chan->cd->bus_id;
	chan->src_addr = chan->cd->addr;
	chan->dst_addr = chan->cd->addr;
	chan->src_cctl = cctl | PL080_CONTROL_DST_INCR |
		pl08x_select_bus(chan->cd->periph_buses, chan->host->mem_buses);
	chan->dst_cctl = cctl | PL080_CONTROL_SRC_INCR |
		pl08x_select_bus(chan->host->mem_buses, chan->cd->periph_buses);
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
static void pl08x_dma_slave_init(struct pl08x_dma_chan *chan)
{
	chan->slave = true;
	chan->name = chan->cd->bus_id;
	chan->cfg.src_addr = chan->cd->addr;
	chan->cfg.dst_addr = chan->cd->addr;
}

>>>>>>> refs/remotes/origin/master
/*
 * Initialise the DMAC memcpy/slave channels.
 * Make a local wrapper to hold required data
 */
static int pl08x_dma_init_virtual_channels(struct pl08x_driver_data *pl08x,
<<<<<<< HEAD
<<<<<<< HEAD
					   struct dma_device *dmadev,
					   unsigned int channels,
					   bool slave)
=======
		struct dma_device *dmadev, unsigned int channels, bool slave)
>>>>>>> refs/remotes/origin/cm-10.0
=======
		struct dma_device *dmadev, unsigned int channels, bool slave)
>>>>>>> refs/remotes/origin/master
{
	struct pl08x_dma_chan *chan;
	int i;

	INIT_LIST_HEAD(&dmadev->channels);

	/*
	 * Register as many many memcpy as we have physical channels,
	 * we won't always be able to use all but the code will have
	 * to cope with that situation.
	 */
	for (i = 0; i < channels; i++) {
<<<<<<< HEAD
<<<<<<< HEAD
		chan = kzalloc(sizeof(struct pl08x_dma_chan), GFP_KERNEL);
=======
		chan = kzalloc(sizeof(*chan), GFP_KERNEL);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		chan = kzalloc(sizeof(*chan), GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
		if (!chan) {
			dev_err(&pl08x->adev->dev,
				"%s no memory for channel\n", __func__);
			return -ENOMEM;
		}

		chan->host = pl08x;
		chan->state = PL08X_CHAN_IDLE;
<<<<<<< HEAD

		if (slave) {
<<<<<<< HEAD
			chan->slave = true;
			chan->name = pl08x->pd->slave_channels[i].bus_id;
			chan->cd = &pl08x->pd->slave_channels[i];
=======
			chan->cd = &pl08x->pd->slave_channels[i];
			pl08x_dma_slave_init(chan);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		chan->signal = -1;

		if (slave) {
			chan->cd = &pl08x->pd->slave_channels[i];
			pl08x_dma_slave_init(chan);
>>>>>>> refs/remotes/origin/master
		} else {
			chan->cd = &pl08x->pd->memcpy_channel;
			chan->name = kasprintf(GFP_KERNEL, "memcpy%d", i);
			if (!chan->name) {
				kfree(chan);
				return -ENOMEM;
			}
		}
<<<<<<< HEAD
		if (chan->cd->circular_buffer) {
			dev_err(&pl08x->adev->dev,
				"channel %s: circular buffers not supported\n",
				chan->name);
			kfree(chan);
			continue;
		}
<<<<<<< HEAD
		dev_info(&pl08x->adev->dev,
=======
		dev_dbg(&pl08x->adev->dev,
>>>>>>> refs/remotes/origin/cm-10.0
			 "initialize virtual channel \"%s\"\n",
			 chan->name);

		chan->chan.device = dmadev;
<<<<<<< HEAD
		chan->chan.cookie = 0;
		chan->lc = 0;
=======
		dma_cookie_init(&chan->chan);
>>>>>>> refs/remotes/origin/cm-10.0

		spin_lock_init(&chan->lock);
		INIT_LIST_HEAD(&chan->pend_list);
		tasklet_init(&chan->tasklet, pl08x_tasklet,
			     (unsigned long) chan);

		list_add_tail(&chan->chan.device_node, &dmadev->channels);
=======
		dev_dbg(&pl08x->adev->dev,
			 "initialize virtual channel \"%s\"\n",
			 chan->name);

		chan->vc.desc_free = pl08x_desc_free;
		vchan_init(&chan->vc, dmadev);
>>>>>>> refs/remotes/origin/master
	}
	dev_info(&pl08x->adev->dev, "initialized %d virtual %s channels\n",
		 i, slave ? "slave" : "memcpy");
	return i;
}

static void pl08x_free_virtual_channels(struct dma_device *dmadev)
{
	struct pl08x_dma_chan *chan = NULL;
	struct pl08x_dma_chan *next;

	list_for_each_entry_safe(chan,
<<<<<<< HEAD
				 next, &dmadev->channels, chan.device_node) {
		list_del(&chan->chan.device_node);
=======
				 next, &dmadev->channels, vc.chan.device_node) {
		list_del(&chan->vc.chan.device_node);
>>>>>>> refs/remotes/origin/master
		kfree(chan);
	}
}

#ifdef CONFIG_DEBUG_FS
static const char *pl08x_state_str(enum pl08x_dma_chan_state state)
{
	switch (state) {
	case PL08X_CHAN_IDLE:
		return "idle";
	case PL08X_CHAN_RUNNING:
		return "running";
	case PL08X_CHAN_PAUSED:
		return "paused";
	case PL08X_CHAN_WAITING:
		return "waiting";
	default:
		break;
	}
	return "UNKNOWN STATE";
}

static int pl08x_debugfs_show(struct seq_file *s, void *data)
{
	struct pl08x_driver_data *pl08x = s->private;
	struct pl08x_dma_chan *chan;
	struct pl08x_phy_chan *ch;
	unsigned long flags;
	int i;

	seq_printf(s, "PL08x physical channels:\n");
	seq_printf(s, "CHANNEL:\tUSER:\n");
	seq_printf(s, "--------\t-----\n");
	for (i = 0; i < pl08x->vd->channels; i++) {
		struct pl08x_dma_chan *virt_chan;

		ch = &pl08x->phy_chans[i];

		spin_lock_irqsave(&ch->lock, flags);
		virt_chan = ch->serving;

<<<<<<< HEAD
		seq_printf(s, "%d\t\t%s\n",
			   ch->id, virt_chan ? virt_chan->name : "(none)");
=======
		seq_printf(s, "%d\t\t%s%s\n",
			   ch->id,
			   virt_chan ? virt_chan->name : "(none)",
			   ch->locked ? " LOCKED" : "");
>>>>>>> refs/remotes/origin/master

		spin_unlock_irqrestore(&ch->lock, flags);
	}

	seq_printf(s, "\nPL08x virtual memcpy channels:\n");
	seq_printf(s, "CHANNEL:\tSTATE:\n");
	seq_printf(s, "--------\t------\n");
<<<<<<< HEAD
	list_for_each_entry(chan, &pl08x->memcpy.channels, chan.device_node) {
=======
	list_for_each_entry(chan, &pl08x->memcpy.channels, vc.chan.device_node) {
>>>>>>> refs/remotes/origin/master
		seq_printf(s, "%s\t\t%s\n", chan->name,
			   pl08x_state_str(chan->state));
	}

	seq_printf(s, "\nPL08x virtual slave channels:\n");
	seq_printf(s, "CHANNEL:\tSTATE:\n");
	seq_printf(s, "--------\t------\n");
<<<<<<< HEAD
	list_for_each_entry(chan, &pl08x->slave.channels, chan.device_node) {
=======
	list_for_each_entry(chan, &pl08x->slave.channels, vc.chan.device_node) {
>>>>>>> refs/remotes/origin/master
		seq_printf(s, "%s\t\t%s\n", chan->name,
			   pl08x_state_str(chan->state));
	}

	return 0;
}

static int pl08x_debugfs_open(struct inode *inode, struct file *file)
{
	return single_open(file, pl08x_debugfs_show, inode->i_private);
}

static const struct file_operations pl08x_debugfs_operations = {
	.open		= pl08x_debugfs_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static void init_pl08x_debugfs(struct pl08x_driver_data *pl08x)
{
	/* Expose a simple debugfs interface to view all clocks */
<<<<<<< HEAD
<<<<<<< HEAD
	(void) debugfs_create_file(dev_name(&pl08x->adev->dev), S_IFREG | S_IRUGO,
				   NULL, pl08x,
				   &pl08x_debugfs_operations);
=======
	(void) debugfs_create_file(dev_name(&pl08x->adev->dev),
			S_IFREG | S_IRUGO, NULL, pl08x,
			&pl08x_debugfs_operations);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	(void) debugfs_create_file(dev_name(&pl08x->adev->dev),
			S_IFREG | S_IRUGO, NULL, pl08x,
			&pl08x_debugfs_operations);
>>>>>>> refs/remotes/origin/master
}

#else
static inline void init_pl08x_debugfs(struct pl08x_driver_data *pl08x)
{
}
#endif

static int pl08x_probe(struct amba_device *adev, const struct amba_id *id)
{
	struct pl08x_driver_data *pl08x;
	const struct vendor_data *vd = id->data;
<<<<<<< HEAD
=======
	u32 tsfr_size;
>>>>>>> refs/remotes/origin/master
	int ret = 0;
	int i;

	ret = amba_request_regions(adev, NULL);
	if (ret)
		return ret;

<<<<<<< HEAD
	/* Create the driver state holder */
<<<<<<< HEAD
	pl08x = kzalloc(sizeof(struct pl08x_driver_data), GFP_KERNEL);
=======
	pl08x = kzalloc(sizeof(*pl08x), GFP_KERNEL);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* Ensure that we can do DMA */
	ret = dma_set_mask_and_coherent(&adev->dev, DMA_BIT_MASK(32));
	if (ret)
		goto out_no_pl08x;

	/* Create the driver state holder */
	pl08x = kzalloc(sizeof(*pl08x), GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (!pl08x) {
		ret = -ENOMEM;
		goto out_no_pl08x;
	}

<<<<<<< HEAD
<<<<<<< HEAD
=======
	pm_runtime_set_active(&adev->dev);
	pm_runtime_enable(&adev->dev);

>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	/* Initialize memcpy engine */
	dma_cap_set(DMA_MEMCPY, pl08x->memcpy.cap_mask);
	pl08x->memcpy.dev = &adev->dev;
	pl08x->memcpy.device_alloc_chan_resources = pl08x_alloc_chan_resources;
	pl08x->memcpy.device_free_chan_resources = pl08x_free_chan_resources;
	pl08x->memcpy.device_prep_dma_memcpy = pl08x_prep_dma_memcpy;
	pl08x->memcpy.device_prep_dma_interrupt = pl08x_prep_dma_interrupt;
	pl08x->memcpy.device_tx_status = pl08x_dma_tx_status;
	pl08x->memcpy.device_issue_pending = pl08x_issue_pending;
	pl08x->memcpy.device_control = pl08x_control;

	/* Initialize slave engine */
	dma_cap_set(DMA_SLAVE, pl08x->slave.cap_mask);
<<<<<<< HEAD
=======
	dma_cap_set(DMA_CYCLIC, pl08x->slave.cap_mask);
>>>>>>> refs/remotes/origin/master
	pl08x->slave.dev = &adev->dev;
	pl08x->slave.device_alloc_chan_resources = pl08x_alloc_chan_resources;
	pl08x->slave.device_free_chan_resources = pl08x_free_chan_resources;
	pl08x->slave.device_prep_dma_interrupt = pl08x_prep_dma_interrupt;
	pl08x->slave.device_tx_status = pl08x_dma_tx_status;
	pl08x->slave.device_issue_pending = pl08x_issue_pending;
	pl08x->slave.device_prep_slave_sg = pl08x_prep_slave_sg;
<<<<<<< HEAD
=======
	pl08x->slave.device_prep_dma_cyclic = pl08x_prep_dma_cyclic;
>>>>>>> refs/remotes/origin/master
	pl08x->slave.device_control = pl08x_control;

	/* Get the platform data */
	pl08x->pd = dev_get_platdata(&adev->dev);
	if (!pl08x->pd) {
		dev_err(&adev->dev, "no platform data supplied\n");
<<<<<<< HEAD
=======
		ret = -EINVAL;
>>>>>>> refs/remotes/origin/master
		goto out_no_platdata;
	}

	/* Assign useful pointers to the driver state */
	pl08x->adev = adev;
	pl08x->vd = vd;

	/* By default, AHB1 only.  If dualmaster, from platform */
	pl08x->lli_buses = PL08X_AHB1;
	pl08x->mem_buses = PL08X_AHB1;
	if (pl08x->vd->dualmaster) {
		pl08x->lli_buses = pl08x->pd->lli_buses;
		pl08x->mem_buses = pl08x->pd->mem_buses;
	}

<<<<<<< HEAD
	/* A DMA memory pool for LLIs, align on 1-byte boundary */
	pl08x->pool = dma_pool_create(DRIVER_NAME, &pl08x->adev->dev,
			PL08X_LLI_TSFR_SIZE, PL08X_ALIGN, 0);
=======
	if (vd->pl080s)
		pl08x->lli_words = PL080S_LLI_WORDS;
	else
		pl08x->lli_words = PL080_LLI_WORDS;
	tsfr_size = MAX_NUM_TSFR_LLIS * pl08x->lli_words * sizeof(u32);

	/* A DMA memory pool for LLIs, align on 1-byte boundary */
	pl08x->pool = dma_pool_create(DRIVER_NAME, &pl08x->adev->dev,
						tsfr_size, PL08X_ALIGN, 0);
>>>>>>> refs/remotes/origin/master
	if (!pl08x->pool) {
		ret = -ENOMEM;
		goto out_no_lli_pool;
	}

<<<<<<< HEAD
	spin_lock_init(&pl08x->lock);

=======
>>>>>>> refs/remotes/origin/master
	pl08x->base = ioremap(adev->res.start, resource_size(&adev->res));
	if (!pl08x->base) {
		ret = -ENOMEM;
		goto out_no_ioremap;
	}

	/* Turn on the PL08x */
	pl08x_ensure_on(pl08x);

	/* Attach the interrupt handler */
	writel(0x000000FF, pl08x->base + PL080_ERR_CLEAR);
	writel(0x000000FF, pl08x->base + PL080_TC_CLEAR);

<<<<<<< HEAD
	ret = request_irq(adev->irq[0], pl08x_irq, IRQF_DISABLED,
			  DRIVER_NAME, pl08x);
=======
	ret = request_irq(adev->irq[0], pl08x_irq, 0, DRIVER_NAME, pl08x);
>>>>>>> refs/remotes/origin/master
	if (ret) {
		dev_err(&adev->dev, "%s failed to request interrupt %d\n",
			__func__, adev->irq[0]);
		goto out_no_irq;
	}

	/* Initialize physical channels */
<<<<<<< HEAD
<<<<<<< HEAD
	pl08x->phy_chans = kmalloc((vd->channels * sizeof(struct pl08x_phy_chan)),
=======
	pl08x->phy_chans = kmalloc((vd->channels * sizeof(*pl08x->phy_chans)),
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pl08x->phy_chans = kzalloc((vd->channels * sizeof(*pl08x->phy_chans)),
>>>>>>> refs/remotes/origin/master
			GFP_KERNEL);
	if (!pl08x->phy_chans) {
		dev_err(&adev->dev, "%s failed to allocate "
			"physical channel holders\n",
			__func__);
<<<<<<< HEAD
=======
		ret = -ENOMEM;
>>>>>>> refs/remotes/origin/master
		goto out_no_phychans;
	}

	for (i = 0; i < vd->channels; i++) {
		struct pl08x_phy_chan *ch = &pl08x->phy_chans[i];

		ch->id = i;
		ch->base = pl08x->base + PL080_Cx_BASE(i);
<<<<<<< HEAD
		spin_lock_init(&ch->lock);
		ch->serving = NULL;
		ch->signal = -1;
<<<<<<< HEAD
		dev_info(&adev->dev,
			 "physical channel %d is %s\n", i,
			 pl08x_phy_channel_busy(ch) ? "BUSY" : "FREE");
=======
		dev_dbg(&adev->dev, "physical channel %d is %s\n",
			i, pl08x_phy_channel_busy(ch) ? "BUSY" : "FREE");
>>>>>>> refs/remotes/origin/cm-10.0
=======
		ch->reg_config = ch->base + vd->config_offset;
		spin_lock_init(&ch->lock);

		/*
		 * Nomadik variants can have channels that are locked
		 * down for the secure world only. Lock up these channels
		 * by perpetually serving a dummy virtual channel.
		 */
		if (vd->nomadik) {
			u32 val;

			val = readl(ch->reg_config);
			if (val & (PL080N_CONFIG_ITPROT | PL080N_CONFIG_SECPROT)) {
				dev_info(&adev->dev, "physical channel %d reserved for secure access only\n", i);
				ch->locked = true;
			}
		}

		dev_dbg(&adev->dev, "physical channel %d is %s\n",
			i, pl08x_phy_channel_busy(ch) ? "BUSY" : "FREE");
>>>>>>> refs/remotes/origin/master
	}

	/* Register as many memcpy channels as there are physical channels */
	ret = pl08x_dma_init_virtual_channels(pl08x, &pl08x->memcpy,
					      pl08x->vd->channels, false);
	if (ret <= 0) {
		dev_warn(&pl08x->adev->dev,
			 "%s failed to enumerate memcpy channels - %d\n",
			 __func__, ret);
		goto out_no_memcpy;
	}
	pl08x->memcpy.chancnt = ret;

	/* Register slave channels */
	ret = pl08x_dma_init_virtual_channels(pl08x, &pl08x->slave,
<<<<<<< HEAD
<<<<<<< HEAD
					      pl08x->pd->num_slave_channels,
					      true);
=======
			pl08x->pd->num_slave_channels, true);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			pl08x->pd->num_slave_channels, true);
>>>>>>> refs/remotes/origin/master
	if (ret <= 0) {
		dev_warn(&pl08x->adev->dev,
			"%s failed to enumerate slave channels - %d\n",
				__func__, ret);
		goto out_no_slave;
	}
	pl08x->slave.chancnt = ret;

	ret = dma_async_device_register(&pl08x->memcpy);
	if (ret) {
		dev_warn(&pl08x->adev->dev,
			"%s failed to register memcpy as an async device - %d\n",
			__func__, ret);
		goto out_no_memcpy_reg;
	}

	ret = dma_async_device_register(&pl08x->slave);
	if (ret) {
		dev_warn(&pl08x->adev->dev,
			"%s failed to register slave as an async device - %d\n",
			__func__, ret);
		goto out_no_slave_reg;
	}

	amba_set_drvdata(adev, pl08x);
	init_pl08x_debugfs(pl08x);
<<<<<<< HEAD
	dev_info(&pl08x->adev->dev, "DMA: PL%03x rev%u at 0x%08llx irq %d\n",
		 amba_part(adev), amba_rev(adev),
		 (unsigned long long)adev->res.start, adev->irq[0]);
<<<<<<< HEAD
=======

	pm_runtime_put(&adev->dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	dev_info(&pl08x->adev->dev, "DMA: PL%03x%s rev%u at 0x%08llx irq %d\n",
		 amba_part(adev), pl08x->vd->pl080s ? "s" : "", amba_rev(adev),
		 (unsigned long long)adev->res.start, adev->irq[0]);

>>>>>>> refs/remotes/origin/master
	return 0;

out_no_slave_reg:
	dma_async_device_unregister(&pl08x->memcpy);
out_no_memcpy_reg:
	pl08x_free_virtual_channels(&pl08x->slave);
out_no_slave:
	pl08x_free_virtual_channels(&pl08x->memcpy);
out_no_memcpy:
	kfree(pl08x->phy_chans);
out_no_phychans:
	free_irq(adev->irq[0], pl08x);
out_no_irq:
	iounmap(pl08x->base);
out_no_ioremap:
	dma_pool_destroy(pl08x->pool);
out_no_lli_pool:
out_no_platdata:
<<<<<<< HEAD
<<<<<<< HEAD
=======
	pm_runtime_put(&adev->dev);
	pm_runtime_disable(&adev->dev);

>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	kfree(pl08x);
out_no_pl08x:
	amba_release_regions(adev);
	return ret;
}

/* PL080 has 8 channels and the PL080 have just 2 */
static struct vendor_data vendor_pl080 = {
<<<<<<< HEAD
	.channels = 8,
	.dualmaster = true,
};

static struct vendor_data vendor_pl081 = {
	.channels = 2,
	.dualmaster = false,
};

static struct amba_id pl08x_ids[] = {
=======
	.config_offset = PL080_CH_CONFIG,
	.channels = 8,
	.dualmaster = true,
	.max_transfer_size = PL080_CONTROL_TRANSFER_SIZE_MASK,
};

static struct vendor_data vendor_nomadik = {
	.config_offset = PL080_CH_CONFIG,
	.channels = 8,
	.dualmaster = true,
	.nomadik = true,
	.max_transfer_size = PL080_CONTROL_TRANSFER_SIZE_MASK,
};

static struct vendor_data vendor_pl080s = {
	.config_offset = PL080S_CH_CONFIG,
	.channels = 8,
	.pl080s = true,
	.max_transfer_size = PL080S_CONTROL_TRANSFER_SIZE_MASK,
};

static struct vendor_data vendor_pl081 = {
	.config_offset = PL080_CH_CONFIG,
	.channels = 2,
	.dualmaster = false,
	.max_transfer_size = PL080_CONTROL_TRANSFER_SIZE_MASK,
};

static struct amba_id pl08x_ids[] = {
	/* Samsung PL080S variant */
	{
		.id	= 0x0a141080,
		.mask	= 0xffffffff,
		.data	= &vendor_pl080s,
	},
>>>>>>> refs/remotes/origin/master
	/* PL080 */
	{
		.id	= 0x00041080,
		.mask	= 0x000fffff,
		.data	= &vendor_pl080,
	},
	/* PL081 */
	{
		.id	= 0x00041081,
		.mask	= 0x000fffff,
		.data	= &vendor_pl081,
	},
	/* Nomadik 8815 PL080 variant */
	{
<<<<<<< HEAD
		.id	= 0x00280880,
		.mask	= 0x00ffffff,
		.data	= &vendor_pl080,
=======
		.id	= 0x00280080,
		.mask	= 0x00ffffff,
		.data	= &vendor_nomadik,
>>>>>>> refs/remotes/origin/master
	},
	{ 0, 0 },
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
MODULE_DEVICE_TABLE(amba, pl08x_ids);

>>>>>>> refs/remotes/origin/cm-10.0
=======
MODULE_DEVICE_TABLE(amba, pl08x_ids);

>>>>>>> refs/remotes/origin/master
static struct amba_driver pl08x_amba_driver = {
	.drv.name	= DRIVER_NAME,
	.id_table	= pl08x_ids,
	.probe		= pl08x_probe,
};

static int __init pl08x_init(void)
{
	int retval;
	retval = amba_driver_register(&pl08x_amba_driver);
	if (retval)
		printk(KERN_WARNING DRIVER_NAME
		       "failed to register as an AMBA device (%d)\n",
		       retval);
	return retval;
}
subsys_initcall(pl08x_init);
