/*
 * Driver for the Atmel AHB DMA Controller (aka HDMA or DMAC on AT91 systems)
 *
 * Copyright (C) 2008 Atmel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 *
<<<<<<< HEAD
 * This supports the Atmel AHB DMA Controller,
 *
 * The driver has currently been tested with the Atmel AT91SAM9RL
 * and AT91SAM9G45 series.
 */

=======
 * This supports the Atmel AHB DMA Controller found in several Atmel SoCs.
 * The only Atmel DMA Controller that is not covered by this driver is the one
 * found on AT91SAM9263.
 */

#include <dt-bindings/dma/at91.h>
>>>>>>> refs/remotes/origin/master
#include <linux/clk.h>
#include <linux/dmaengine.h>
#include <linux/dma-mapping.h>
#include <linux/dmapool.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
<<<<<<< HEAD
<<<<<<< HEAD

#include "at_hdmac_regs.h"
=======
#include <linux/of.h>
#include <linux/of_device.h>

#include "at_hdmac_regs.h"
#include "dmaengine.h"
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_dma.h>

#include "at_hdmac_regs.h"
#include "dmaengine.h"
>>>>>>> refs/remotes/origin/master

/*
 * Glossary
 * --------
 *
 * at_hdmac		: Name of the ATmel AHB DMA Controller
 * at_dma_ / atdma	: ATmel DMA controller entity related
 * atc_	/ atchan	: ATmel DMA Channel entity related
 */

#define	ATC_DEFAULT_CFG		(ATC_FIFOCFG_HALFFIFO)
<<<<<<< HEAD
#define	ATC_DEFAULT_CTRLA	(0)
=======
>>>>>>> refs/remotes/origin/master
#define	ATC_DEFAULT_CTRLB	(ATC_SIF(AT_DMA_MEM_IF) \
				|ATC_DIF(AT_DMA_MEM_IF))

/*
 * Initial number of descriptors to allocate for each channel. This could
 * be increased during dma usage.
 */
static unsigned int init_nr_desc_per_channel = 64;
module_param(init_nr_desc_per_channel, uint, 0644);
MODULE_PARM_DESC(init_nr_desc_per_channel,
		 "initial descriptors per channel (default: 64)");


/* prototypes */
static dma_cookie_t atc_tx_submit(struct dma_async_tx_descriptor *tx);
<<<<<<< HEAD
=======
static void atc_issue_pending(struct dma_chan *chan);
>>>>>>> refs/remotes/origin/master


/*----------------------------------------------------------------------*/

static struct at_desc *atc_first_active(struct at_dma_chan *atchan)
{
	return list_first_entry(&atchan->active_list,
				struct at_desc, desc_node);
}

static struct at_desc *atc_first_queued(struct at_dma_chan *atchan)
{
	return list_first_entry(&atchan->queue,
				struct at_desc, desc_node);
}

/**
 * atc_alloc_descriptor - allocate and return an initialized descriptor
 * @chan: the channel to allocate descriptors for
 * @gfp_flags: GFP allocation flags
 *
 * Note: The ack-bit is positioned in the descriptor flag at creation time
 *       to make initial allocation more convenient. This bit will be cleared
 *       and control will be given to client at usage time (during
 *       preparation functions).
 */
static struct at_desc *atc_alloc_descriptor(struct dma_chan *chan,
					    gfp_t gfp_flags)
{
	struct at_desc	*desc = NULL;
	struct at_dma	*atdma = to_at_dma(chan->device);
	dma_addr_t phys;

	desc = dma_pool_alloc(atdma->dma_desc_pool, gfp_flags, &phys);
	if (desc) {
		memset(desc, 0, sizeof(struct at_desc));
		INIT_LIST_HEAD(&desc->tx_list);
		dma_async_tx_descriptor_init(&desc->txd, chan);
		/* txd.flags will be overwritten in prep functions */
		desc->txd.flags = DMA_CTRL_ACK;
		desc->txd.tx_submit = atc_tx_submit;
		desc->txd.phys = phys;
	}

	return desc;
}

/**
 * atc_desc_get - get an unused descriptor from free_list
 * @atchan: channel we want a new descriptor for
 */
static struct at_desc *atc_desc_get(struct at_dma_chan *atchan)
{
	struct at_desc *desc, *_desc;
	struct at_desc *ret = NULL;
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned int i = 0;
	LIST_HEAD(tmp_list);

	spin_lock_bh(&atchan->lock);
=======
=======
>>>>>>> refs/remotes/origin/master
	unsigned long flags;
	unsigned int i = 0;
	LIST_HEAD(tmp_list);

	spin_lock_irqsave(&atchan->lock, flags);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	list_for_each_entry_safe(desc, _desc, &atchan->free_list, desc_node) {
		i++;
		if (async_tx_test_ack(&desc->txd)) {
			list_del(&desc->desc_node);
			ret = desc;
			break;
		}
		dev_dbg(chan2dev(&atchan->chan_common),
				"desc %p not ACKed\n", desc);
	}
<<<<<<< HEAD
<<<<<<< HEAD
	spin_unlock_bh(&atchan->lock);
=======
	spin_unlock_irqrestore(&atchan->lock, flags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	spin_unlock_irqrestore(&atchan->lock, flags);
>>>>>>> refs/remotes/origin/master
	dev_vdbg(chan2dev(&atchan->chan_common),
		"scanned %u descriptors on freelist\n", i);

	/* no more descriptor available in initial pool: create one more */
	if (!ret) {
		ret = atc_alloc_descriptor(&atchan->chan_common, GFP_ATOMIC);
		if (ret) {
<<<<<<< HEAD
<<<<<<< HEAD
			spin_lock_bh(&atchan->lock);
			atchan->descs_allocated++;
			spin_unlock_bh(&atchan->lock);
=======
			spin_lock_irqsave(&atchan->lock, flags);
			atchan->descs_allocated++;
			spin_unlock_irqrestore(&atchan->lock, flags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			spin_lock_irqsave(&atchan->lock, flags);
			atchan->descs_allocated++;
			spin_unlock_irqrestore(&atchan->lock, flags);
>>>>>>> refs/remotes/origin/master
		} else {
			dev_err(chan2dev(&atchan->chan_common),
					"not enough descriptors available\n");
		}
	}

	return ret;
}

/**
 * atc_desc_put - move a descriptor, including any children, to the free list
 * @atchan: channel we work on
 * @desc: descriptor, at the head of a chain, to move to free list
 */
static void atc_desc_put(struct at_dma_chan *atchan, struct at_desc *desc)
{
	if (desc) {
		struct at_desc *child;
<<<<<<< HEAD
<<<<<<< HEAD

		spin_lock_bh(&atchan->lock);
=======
		unsigned long flags;

		spin_lock_irqsave(&atchan->lock, flags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		unsigned long flags;

		spin_lock_irqsave(&atchan->lock, flags);
>>>>>>> refs/remotes/origin/master
		list_for_each_entry(child, &desc->tx_list, desc_node)
			dev_vdbg(chan2dev(&atchan->chan_common),
					"moving child desc %p to freelist\n",
					child);
		list_splice_init(&desc->tx_list, &atchan->free_list);
		dev_vdbg(chan2dev(&atchan->chan_common),
			 "moving desc %p to freelist\n", desc);
		list_add(&desc->desc_node, &atchan->free_list);
<<<<<<< HEAD
<<<<<<< HEAD
		spin_unlock_bh(&atchan->lock);
=======
		spin_unlock_irqrestore(&atchan->lock, flags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		spin_unlock_irqrestore(&atchan->lock, flags);
>>>>>>> refs/remotes/origin/master
	}
}

/**
<<<<<<< HEAD
 * atc_desc_chain - build chain adding a descripor
 * @first: address of first descripor of the chain
 * @prev: address of previous descripor of the chain
=======
 * atc_desc_chain - build chain adding a descriptor
 * @first: address of first descriptor of the chain
 * @prev: address of previous descriptor of the chain
>>>>>>> refs/remotes/origin/master
 * @desc: descriptor to queue
 *
 * Called from prep_* functions
 */
static void atc_desc_chain(struct at_desc **first, struct at_desc **prev,
			   struct at_desc *desc)
{
	if (!(*first)) {
		*first = desc;
	} else {
		/* inform the HW lli about chaining */
		(*prev)->lli.dscr = desc->txd.phys;
		/* insert the link descriptor to the LD ring */
		list_add_tail(&desc->desc_node,
				&(*first)->tx_list);
	}
	*prev = desc;
}

/**
<<<<<<< HEAD
<<<<<<< HEAD
 * atc_assign_cookie - compute and assign new cookie
 * @atchan: channel we work on
 * @desc: descriptor to assign cookie for
 *
 * Called with atchan->lock held and bh disabled
 */
static dma_cookie_t
atc_assign_cookie(struct at_dma_chan *atchan, struct at_desc *desc)
{
	dma_cookie_t cookie = atchan->chan_common.cookie;

	if (++cookie < 0)
		cookie = 1;

	atchan->chan_common.cookie = cookie;
	desc->txd.cookie = cookie;

	return cookie;
}

/**
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 * atc_dostart - starts the DMA engine for real
 * @atchan: the channel we want to start
 * @first: first descriptor in the list we want to begin with
 *
 * Called with atchan->lock held and bh disabled
 */
static void atc_dostart(struct at_dma_chan *atchan, struct at_desc *first)
{
	struct at_dma	*atdma = to_at_dma(atchan->chan_common.device);

	/* ASSERT:  channel is idle */
	if (atc_chan_is_enabled(atchan)) {
		dev_err(chan2dev(&atchan->chan_common),
			"BUG: Attempted to start non-idle channel\n");
		dev_err(chan2dev(&atchan->chan_common),
			"  channel: s0x%x d0x%x ctrl0x%x:0x%x l0x%x\n",
			channel_readl(atchan, SADDR),
			channel_readl(atchan, DADDR),
			channel_readl(atchan, CTRLA),
			channel_readl(atchan, CTRLB),
			channel_readl(atchan, DSCR));

		/* The tasklet will hopefully advance the queue... */
		return;
	}

	vdbg_dump_regs(atchan);

	channel_writel(atchan, SADDR, 0);
	channel_writel(atchan, DADDR, 0);
	channel_writel(atchan, CTRLA, 0);
	channel_writel(atchan, CTRLB, 0);
	channel_writel(atchan, DSCR, first->txd.phys);
	dma_writel(atdma, CHER, atchan->mask);

	vdbg_dump_regs(atchan);
}

<<<<<<< HEAD
=======
/*
 * atc_get_current_descriptors -
 * locate the descriptor which equal to physical address in DSCR
 * @atchan: the channel we want to start
 * @dscr_addr: physical descriptor address in DSCR
 */
static struct at_desc *atc_get_current_descriptors(struct at_dma_chan *atchan,
							u32 dscr_addr)
{
	struct at_desc  *desc, *_desc, *child, *desc_cur = NULL;

	list_for_each_entry_safe(desc, _desc, &atchan->active_list, desc_node) {
		if (desc->lli.dscr == dscr_addr) {
			desc_cur = desc;
			break;
		}

		list_for_each_entry(child, &desc->tx_list, desc_node) {
			if (child->lli.dscr == dscr_addr) {
				desc_cur = child;
				break;
			}
		}
	}

	return desc_cur;
}

/*
 * atc_get_bytes_left -
 * Get the number of bytes residue in dma buffer,
 * @chan: the channel we want to start
 */
static int atc_get_bytes_left(struct dma_chan *chan)
{
	struct at_dma_chan      *atchan = to_at_dma_chan(chan);
	struct at_dma           *atdma = to_at_dma(chan->device);
	int	chan_id = atchan->chan_common.chan_id;
	struct at_desc *desc_first = atc_first_active(atchan);
	struct at_desc *desc_cur;
	int ret = 0, count = 0;

	/*
	 * Initialize necessary values in the first time.
	 * remain_desc record remain desc length.
	 */
	if (atchan->remain_desc == 0)
		/* First descriptor embedds the transaction length */
		atchan->remain_desc = desc_first->len;

	/*
	 * This happens when current descriptor transfer complete.
	 * The residual buffer size should reduce current descriptor length.
	 */
	if (unlikely(test_bit(ATC_IS_BTC, &atchan->status))) {
		clear_bit(ATC_IS_BTC, &atchan->status);
		desc_cur = atc_get_current_descriptors(atchan,
						channel_readl(atchan, DSCR));
		if (!desc_cur) {
			ret = -EINVAL;
			goto out;
		}
		atchan->remain_desc -= (desc_cur->lli.ctrla & ATC_BTSIZE_MAX)
						<< (desc_first->tx_width);
		if (atchan->remain_desc < 0) {
			ret = -EINVAL;
			goto out;
		} else {
			ret = atchan->remain_desc;
		}
	} else {
		/*
		 * Get residual bytes when current
		 * descriptor transfer in progress.
		 */
		count = (channel_readl(atchan, CTRLA) & ATC_BTSIZE_MAX)
				<< (desc_first->tx_width);
		ret = atchan->remain_desc - count;
	}
	/*
	 * Check fifo empty.
	 */
	if (!(dma_readl(atdma, CHSR) & AT_DMA_EMPT(chan_id)))
		atc_issue_pending(chan);

out:
	return ret;
}

>>>>>>> refs/remotes/origin/master
/**
 * atc_chain_complete - finish work for one transaction chain
 * @atchan: channel we work on
 * @desc: descriptor at the head of the chain we want do complete
 *
 * Called with atchan->lock held and bh disabled */
static void
atc_chain_complete(struct at_dma_chan *atchan, struct at_desc *desc)
{
	struct dma_async_tx_descriptor	*txd = &desc->txd;

	dev_vdbg(chan2dev(&atchan->chan_common),
		"descriptor %u complete\n", txd->cookie);

<<<<<<< HEAD
<<<<<<< HEAD
	atchan->completed_cookie = txd->cookie;
=======
	/* mark the descriptor as complete for non cyclic cases only */
	if (!atc_chan_is_cyclic(atchan))
		dma_cookie_complete(txd);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* mark the descriptor as complete for non cyclic cases only */
	if (!atc_chan_is_cyclic(atchan))
		dma_cookie_complete(txd);
>>>>>>> refs/remotes/origin/master

	/* move children to free_list */
	list_splice_init(&desc->tx_list, &atchan->free_list);
	/* move myself to free_list */
	list_move(&desc->desc_node, &atchan->free_list);

<<<<<<< HEAD
	/* unmap dma addresses (not on slave channels) */
	if (!atchan->chan_common.private) {
		struct device *parent = chan2parent(&atchan->chan_common);
		if (!(txd->flags & DMA_COMPL_SKIP_DEST_UNMAP)) {
			if (txd->flags & DMA_COMPL_DEST_UNMAP_SINGLE)
				dma_unmap_single(parent,
						desc->lli.daddr,
						desc->len, DMA_FROM_DEVICE);
			else
				dma_unmap_page(parent,
						desc->lli.daddr,
						desc->len, DMA_FROM_DEVICE);
		}
		if (!(txd->flags & DMA_COMPL_SKIP_SRC_UNMAP)) {
			if (txd->flags & DMA_COMPL_SRC_UNMAP_SINGLE)
				dma_unmap_single(parent,
						desc->lli.saddr,
						desc->len, DMA_TO_DEVICE);
			else
				dma_unmap_page(parent,
						desc->lli.saddr,
						desc->len, DMA_TO_DEVICE);
		}
	}

	/* for cyclic transfers,
	 * no need to replay callback function while stopping */
<<<<<<< HEAD
	if (!test_bit(ATC_IS_CYCLIC, &atchan->status)) {
=======
	if (!atc_chan_is_cyclic(atchan)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	dma_descriptor_unmap(txd);
	/* for cyclic transfers,
	 * no need to replay callback function while stopping */
	if (!atc_chan_is_cyclic(atchan)) {
>>>>>>> refs/remotes/origin/master
		dma_async_tx_callback	callback = txd->callback;
		void			*param = txd->callback_param;

		/*
		 * The API requires that no submissions are done from a
		 * callback, so we don't need to drop the lock here
		 */
		if (callback)
			callback(param);
	}

	dma_run_dependencies(txd);
}

/**
 * atc_complete_all - finish work for all transactions
 * @atchan: channel to complete transactions for
 *
 * Eventually submit queued descriptors if any
 *
 * Assume channel is idle while calling this function
 * Called with atchan->lock held and bh disabled
 */
static void atc_complete_all(struct at_dma_chan *atchan)
{
	struct at_desc *desc, *_desc;
	LIST_HEAD(list);

	dev_vdbg(chan2dev(&atchan->chan_common), "complete all\n");

<<<<<<< HEAD
	BUG_ON(atc_chan_is_enabled(atchan));

=======
>>>>>>> refs/remotes/origin/master
	/*
	 * Submit queued descriptors ASAP, i.e. before we go through
	 * the completed ones.
	 */
	if (!list_empty(&atchan->queue))
		atc_dostart(atchan, atc_first_queued(atchan));
	/* empty active_list now it is completed */
	list_splice_init(&atchan->active_list, &list);
	/* empty queue list by moving descriptors (if any) to active_list */
	list_splice_init(&atchan->queue, &atchan->active_list);

	list_for_each_entry_safe(desc, _desc, &list, desc_node)
		atc_chain_complete(atchan, desc);
}

/**
<<<<<<< HEAD
 * atc_cleanup_descriptors - cleanup up finished descriptors in active_list
 * @atchan: channel to be cleaned up
 *
 * Called with atchan->lock held and bh disabled
 */
static void atc_cleanup_descriptors(struct at_dma_chan *atchan)
{
	struct at_desc	*desc, *_desc;
	struct at_desc	*child;

	dev_vdbg(chan2dev(&atchan->chan_common), "cleanup descriptors\n");

	list_for_each_entry_safe(desc, _desc, &atchan->active_list, desc_node) {
		if (!(desc->lli.ctrla & ATC_DONE))
			/* This one is currently in progress */
			return;

		list_for_each_entry(child, &desc->tx_list, desc_node)
			if (!(child->lli.ctrla & ATC_DONE))
				/* Currently in progress */
				return;

		/*
		 * No descriptors so far seem to be in progress, i.e.
		 * this chain must be done.
		 */
		atc_chain_complete(atchan, desc);
	}
}

/**
=======
>>>>>>> refs/remotes/origin/master
 * atc_advance_work - at the end of a transaction, move forward
 * @atchan: channel where the transaction ended
 *
 * Called with atchan->lock held and bh disabled
 */
static void atc_advance_work(struct at_dma_chan *atchan)
{
	dev_vdbg(chan2dev(&atchan->chan_common), "advance_work\n");

<<<<<<< HEAD
=======
	if (atc_chan_is_enabled(atchan))
		return;

>>>>>>> refs/remotes/origin/master
	if (list_empty(&atchan->active_list) ||
	    list_is_singular(&atchan->active_list)) {
		atc_complete_all(atchan);
	} else {
		atc_chain_complete(atchan, atc_first_active(atchan));
		/* advance work */
		atc_dostart(atchan, atc_first_active(atchan));
	}
}


/**
 * atc_handle_error - handle errors reported by DMA controller
 * @atchan: channel where error occurs
 *
 * Called with atchan->lock held and bh disabled
 */
static void atc_handle_error(struct at_dma_chan *atchan)
{
	struct at_desc *bad_desc;
	struct at_desc *child;

	/*
	 * The descriptor currently at the head of the active list is
	 * broked. Since we don't have any way to report errors, we'll
	 * just have to scream loudly and try to carry on.
	 */
	bad_desc = atc_first_active(atchan);
	list_del_init(&bad_desc->desc_node);

	/* As we are stopped, take advantage to push queued descriptors
	 * in active_list */
	list_splice_init(&atchan->queue, atchan->active_list.prev);

	/* Try to restart the controller */
	if (!list_empty(&atchan->active_list))
		atc_dostart(atchan, atc_first_active(atchan));

	/*
	 * KERN_CRITICAL may seem harsh, but since this only happens
	 * when someone submits a bad physical address in a
	 * descriptor, we should consider ourselves lucky that the
	 * controller flagged an error instead of scribbling over
	 * random memory locations.
	 */
	dev_crit(chan2dev(&atchan->chan_common),
			"Bad descriptor submitted for DMA!\n");
	dev_crit(chan2dev(&atchan->chan_common),
			"  cookie: %d\n", bad_desc->txd.cookie);
	atc_dump_lli(atchan, &bad_desc->lli);
	list_for_each_entry(child, &bad_desc->tx_list, desc_node)
		atc_dump_lli(atchan, &child->lli);

	/* Pretend the descriptor completed successfully */
	atc_chain_complete(atchan, bad_desc);
}

/**
 * atc_handle_cyclic - at the end of a period, run callback function
 * @atchan: channel used for cyclic operations
 *
 * Called with atchan->lock held and bh disabled
 */
static void atc_handle_cyclic(struct at_dma_chan *atchan)
{
	struct at_desc			*first = atc_first_active(atchan);
	struct dma_async_tx_descriptor	*txd = &first->txd;
	dma_async_tx_callback		callback = txd->callback;
	void				*param = txd->callback_param;

	dev_vdbg(chan2dev(&atchan->chan_common),
			"new cyclic period llp 0x%08x\n",
			channel_readl(atchan, DSCR));

	if (callback)
		callback(param);
}

/*--  IRQ & Tasklet  ---------------------------------------------------*/

static void atc_tasklet(unsigned long data)
{
	struct at_dma_chan *atchan = (struct at_dma_chan *)data;
<<<<<<< HEAD
<<<<<<< HEAD

	spin_lock(&atchan->lock);
	if (test_and_clear_bit(ATC_IS_ERROR, &atchan->status))
		atc_handle_error(atchan);
	else if (test_bit(ATC_IS_CYCLIC, &atchan->status))
=======
=======
>>>>>>> refs/remotes/origin/master
	unsigned long flags;

	spin_lock_irqsave(&atchan->lock, flags);
	if (test_and_clear_bit(ATC_IS_ERROR, &atchan->status))
		atc_handle_error(atchan);
	else if (atc_chan_is_cyclic(atchan))
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		atc_handle_cyclic(atchan);
	else
		atc_advance_work(atchan);

<<<<<<< HEAD
<<<<<<< HEAD
	spin_unlock(&atchan->lock);
=======
	spin_unlock_irqrestore(&atchan->lock, flags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	spin_unlock_irqrestore(&atchan->lock, flags);
>>>>>>> refs/remotes/origin/master
}

static irqreturn_t at_dma_interrupt(int irq, void *dev_id)
{
	struct at_dma		*atdma = (struct at_dma *)dev_id;
	struct at_dma_chan	*atchan;
	int			i;
	u32			status, pending, imr;
	int			ret = IRQ_NONE;

	do {
		imr = dma_readl(atdma, EBCIMR);
		status = dma_readl(atdma, EBCISR);
		pending = status & imr;

		if (!pending)
			break;

		dev_vdbg(atdma->dma_common.dev,
			"interrupt: status = 0x%08x, 0x%08x, 0x%08x\n",
			 status, imr, pending);

		for (i = 0; i < atdma->dma_common.chancnt; i++) {
			atchan = &atdma->chan[i];
			if (pending & (AT_DMA_BTC(i) | AT_DMA_ERR(i))) {
				if (pending & AT_DMA_ERR(i)) {
					/* Disable channel on AHB error */
					dma_writel(atdma, CHDR,
						AT_DMA_RES(i) | atchan->mask);
					/* Give information to tasklet */
					set_bit(ATC_IS_ERROR, &atchan->status);
				}
<<<<<<< HEAD
=======
				if (pending & AT_DMA_BTC(i))
					set_bit(ATC_IS_BTC, &atchan->status);
>>>>>>> refs/remotes/origin/master
				tasklet_schedule(&atchan->tasklet);
				ret = IRQ_HANDLED;
			}
		}

	} while (pending);

	return ret;
}


/*--  DMA Engine API  --------------------------------------------------*/

/**
 * atc_tx_submit - set the prepared descriptor(s) to be executed by the engine
 * @desc: descriptor at the head of the transaction chain
 *
 * Queue chain if DMA engine is working already
 *
 * Cookie increment and adding to active_list or queue must be atomic
 */
static dma_cookie_t atc_tx_submit(struct dma_async_tx_descriptor *tx)
{
	struct at_desc		*desc = txd_to_at_desc(tx);
	struct at_dma_chan	*atchan = to_at_dma_chan(tx->chan);
	dma_cookie_t		cookie;
<<<<<<< HEAD
<<<<<<< HEAD

	spin_lock_bh(&atchan->lock);
	cookie = atc_assign_cookie(atchan, desc);
=======
=======
>>>>>>> refs/remotes/origin/master
	unsigned long		flags;

	spin_lock_irqsave(&atchan->lock, flags);
	cookie = dma_cookie_assign(tx);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	if (list_empty(&atchan->active_list)) {
		dev_vdbg(chan2dev(tx->chan), "tx_submit: started %u\n",
				desc->txd.cookie);
		atc_dostart(atchan, desc);
		list_add_tail(&desc->desc_node, &atchan->active_list);
	} else {
		dev_vdbg(chan2dev(tx->chan), "tx_submit: queued %u\n",
				desc->txd.cookie);
		list_add_tail(&desc->desc_node, &atchan->queue);
	}

<<<<<<< HEAD
<<<<<<< HEAD
	spin_unlock_bh(&atchan->lock);
=======
	spin_unlock_irqrestore(&atchan->lock, flags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	spin_unlock_irqrestore(&atchan->lock, flags);
>>>>>>> refs/remotes/origin/master

	return cookie;
}

/**
 * atc_prep_dma_memcpy - prepare a memcpy operation
 * @chan: the channel to prepare operation on
 * @dest: operation virtual destination address
 * @src: operation virtual source address
 * @len: operation length
 * @flags: tx descriptor status flags
 */
static struct dma_async_tx_descriptor *
atc_prep_dma_memcpy(struct dma_chan *chan, dma_addr_t dest, dma_addr_t src,
		size_t len, unsigned long flags)
{
	struct at_dma_chan	*atchan = to_at_dma_chan(chan);
	struct at_desc		*desc = NULL;
	struct at_desc		*first = NULL;
	struct at_desc		*prev = NULL;
	size_t			xfer_count;
	size_t			offset;
	unsigned int		src_width;
	unsigned int		dst_width;
	u32			ctrla;
	u32			ctrlb;

	dev_vdbg(chan2dev(chan), "prep_dma_memcpy: d0x%x s0x%x l0x%zx f0x%lx\n",
			dest, src, len, flags);

	if (unlikely(!len)) {
		dev_dbg(chan2dev(chan), "prep_dma_memcpy: length is zero!\n");
		return NULL;
	}

<<<<<<< HEAD
	ctrla =   ATC_DEFAULT_CTRLA;
=======
>>>>>>> refs/remotes/origin/master
	ctrlb =   ATC_DEFAULT_CTRLB | ATC_IEN
		| ATC_SRC_ADDR_MODE_INCR
		| ATC_DST_ADDR_MODE_INCR
		| ATC_FC_MEM2MEM;

	/*
	 * We can be a lot more clever here, but this should take care
	 * of the most common optimization.
	 */
	if (!((src | dest  | len) & 3)) {
<<<<<<< HEAD
		ctrla |= ATC_SRC_WIDTH_WORD | ATC_DST_WIDTH_WORD;
		src_width = dst_width = 2;
	} else if (!((src | dest | len) & 1)) {
		ctrla |= ATC_SRC_WIDTH_HALFWORD | ATC_DST_WIDTH_HALFWORD;
		src_width = dst_width = 1;
	} else {
		ctrla |= ATC_SRC_WIDTH_BYTE | ATC_DST_WIDTH_BYTE;
=======
		ctrla = ATC_SRC_WIDTH_WORD | ATC_DST_WIDTH_WORD;
		src_width = dst_width = 2;
	} else if (!((src | dest | len) & 1)) {
		ctrla = ATC_SRC_WIDTH_HALFWORD | ATC_DST_WIDTH_HALFWORD;
		src_width = dst_width = 1;
	} else {
		ctrla = ATC_SRC_WIDTH_BYTE | ATC_DST_WIDTH_BYTE;
>>>>>>> refs/remotes/origin/master
		src_width = dst_width = 0;
	}

	for (offset = 0; offset < len; offset += xfer_count << src_width) {
		xfer_count = min_t(size_t, (len - offset) >> src_width,
				ATC_BTSIZE_MAX);

		desc = atc_desc_get(atchan);
		if (!desc)
			goto err_desc_get;

		desc->lli.saddr = src + offset;
		desc->lli.daddr = dest + offset;
		desc->lli.ctrla = ctrla | xfer_count;
		desc->lli.ctrlb = ctrlb;

		desc->txd.cookie = 0;

		atc_desc_chain(&first, &prev, desc);
	}

	/* First descriptor of the chain embedds additional information */
	first->txd.cookie = -EBUSY;
	first->len = len;
<<<<<<< HEAD
=======
	first->tx_width = src_width;
>>>>>>> refs/remotes/origin/master

	/* set end-of-link to the last link descriptor of list*/
	set_desc_eol(desc);

	first->txd.flags = flags; /* client is in control of this ack */

	return &first->txd;

err_desc_get:
	atc_desc_put(atchan, first);
	return NULL;
}


/**
 * atc_prep_slave_sg - prepare descriptors for a DMA_SLAVE transaction
 * @chan: DMA channel
 * @sgl: scatterlist to transfer to/from
 * @sg_len: number of entries in @scatterlist
 * @direction: DMA direction
 * @flags: tx descriptor status flags
<<<<<<< HEAD
<<<<<<< HEAD
 */
static struct dma_async_tx_descriptor *
atc_prep_slave_sg(struct dma_chan *chan, struct scatterlist *sgl,
		unsigned int sg_len, enum dma_data_direction direction,
		unsigned long flags)
{
	struct at_dma_chan	*atchan = to_at_dma_chan(chan);
	struct at_dma_slave	*atslave = chan->private;
=======
=======
>>>>>>> refs/remotes/origin/master
 * @context: transaction context (ignored)
 */
static struct dma_async_tx_descriptor *
atc_prep_slave_sg(struct dma_chan *chan, struct scatterlist *sgl,
		unsigned int sg_len, enum dma_transfer_direction direction,
		unsigned long flags, void *context)
{
	struct at_dma_chan	*atchan = to_at_dma_chan(chan);
	struct at_dma_slave	*atslave = chan->private;
	struct dma_slave_config	*sconfig = &atchan->dma_sconfig;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	struct at_desc		*first = NULL;
	struct at_desc		*prev = NULL;
	u32			ctrla;
	u32			ctrlb;
	dma_addr_t		reg;
	unsigned int		reg_width;
	unsigned int		mem_width;
	unsigned int		i;
	struct scatterlist	*sg;
	size_t			total_len = 0;

	dev_vdbg(chan2dev(chan), "prep_slave_sg (%d): %s f0x%lx\n",
			sg_len,
<<<<<<< HEAD
<<<<<<< HEAD
			direction == DMA_TO_DEVICE ? "TO DEVICE" : "FROM DEVICE",
=======
			direction == DMA_MEM_TO_DEV ? "TO DEVICE" : "FROM DEVICE",
>>>>>>> refs/remotes/origin/cm-10.0
=======
			direction == DMA_MEM_TO_DEV ? "TO DEVICE" : "FROM DEVICE",
>>>>>>> refs/remotes/origin/master
			flags);

	if (unlikely(!atslave || !sg_len)) {
		dev_dbg(chan2dev(chan), "prep_slave_sg: sg length is zero!\n");
		return NULL;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	reg_width = atslave->reg_width;

=======
>>>>>>> refs/remotes/origin/cm-10.0
	ctrla = ATC_DEFAULT_CTRLA | atslave->ctrla;
	ctrlb = ATC_IEN;

	switch (direction) {
<<<<<<< HEAD
	case DMA_TO_DEVICE:
=======
	case DMA_MEM_TO_DEV:
		reg_width = convert_buswidth(sconfig->dst_addr_width);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ctrla =   ATC_SCSIZE(sconfig->src_maxburst)
		| ATC_DCSIZE(sconfig->dst_maxburst);
	ctrlb = ATC_IEN;

	switch (direction) {
	case DMA_MEM_TO_DEV:
		reg_width = convert_buswidth(sconfig->dst_addr_width);
>>>>>>> refs/remotes/origin/master
		ctrla |=  ATC_DST_WIDTH(reg_width);
		ctrlb |=  ATC_DST_ADDR_MODE_FIXED
			| ATC_SRC_ADDR_MODE_INCR
			| ATC_FC_MEM2PER
<<<<<<< HEAD
			| ATC_SIF(AT_DMA_MEM_IF) | ATC_DIF(AT_DMA_PER_IF);
<<<<<<< HEAD
		reg = atslave->tx_reg;
=======
		reg = sconfig->dst_addr;
>>>>>>> refs/remotes/origin/cm-10.0
=======
			| ATC_SIF(atchan->mem_if) | ATC_DIF(atchan->per_if);
		reg = sconfig->dst_addr;
>>>>>>> refs/remotes/origin/master
		for_each_sg(sgl, sg, sg_len, i) {
			struct at_desc	*desc;
			u32		len;
			u32		mem;

			desc = atc_desc_get(atchan);
			if (!desc)
				goto err_desc_get;

			mem = sg_dma_address(sg);
			len = sg_dma_len(sg);
			if (unlikely(!len)) {
				dev_dbg(chan2dev(chan),
					"prep_slave_sg: sg(%d) data length is zero\n", i);
				goto err;
			}
			mem_width = 2;
			if (unlikely(mem & 3 || len & 3))
				mem_width = 0;

			desc->lli.saddr = mem;
			desc->lli.daddr = reg;
			desc->lli.ctrla = ctrla
					| ATC_SRC_WIDTH(mem_width)
					| len >> mem_width;
			desc->lli.ctrlb = ctrlb;

			atc_desc_chain(&first, &prev, desc);
			total_len += len;
		}
		break;
<<<<<<< HEAD
<<<<<<< HEAD
	case DMA_FROM_DEVICE:
=======
	case DMA_DEV_TO_MEM:
		reg_width = convert_buswidth(sconfig->src_addr_width);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	case DMA_DEV_TO_MEM:
		reg_width = convert_buswidth(sconfig->src_addr_width);
>>>>>>> refs/remotes/origin/master
		ctrla |=  ATC_SRC_WIDTH(reg_width);
		ctrlb |=  ATC_DST_ADDR_MODE_INCR
			| ATC_SRC_ADDR_MODE_FIXED
			| ATC_FC_PER2MEM
<<<<<<< HEAD
			| ATC_SIF(AT_DMA_PER_IF) | ATC_DIF(AT_DMA_MEM_IF);

<<<<<<< HEAD
		reg = atslave->rx_reg;
=======
		reg = sconfig->src_addr;
>>>>>>> refs/remotes/origin/cm-10.0
=======
			| ATC_SIF(atchan->per_if) | ATC_DIF(atchan->mem_if);

		reg = sconfig->src_addr;
>>>>>>> refs/remotes/origin/master
		for_each_sg(sgl, sg, sg_len, i) {
			struct at_desc	*desc;
			u32		len;
			u32		mem;

			desc = atc_desc_get(atchan);
			if (!desc)
				goto err_desc_get;

			mem = sg_dma_address(sg);
			len = sg_dma_len(sg);
			if (unlikely(!len)) {
				dev_dbg(chan2dev(chan),
					"prep_slave_sg: sg(%d) data length is zero\n", i);
				goto err;
			}
			mem_width = 2;
			if (unlikely(mem & 3 || len & 3))
				mem_width = 0;

			desc->lli.saddr = reg;
			desc->lli.daddr = mem;
			desc->lli.ctrla = ctrla
					| ATC_DST_WIDTH(mem_width)
					| len >> reg_width;
			desc->lli.ctrlb = ctrlb;

			atc_desc_chain(&first, &prev, desc);
			total_len += len;
		}
		break;
	default:
		return NULL;
	}

	/* set end-of-link to the last link descriptor of list*/
	set_desc_eol(prev);

	/* First descriptor of the chain embedds additional information */
	first->txd.cookie = -EBUSY;
	first->len = total_len;
<<<<<<< HEAD
=======
	first->tx_width = reg_width;
>>>>>>> refs/remotes/origin/master

	/* first link descriptor of list is responsible of flags */
	first->txd.flags = flags; /* client is in control of this ack */

	return &first->txd;

err_desc_get:
	dev_err(chan2dev(chan), "not enough descriptors available\n");
err:
	atc_desc_put(atchan, first);
	return NULL;
}

/**
 * atc_dma_cyclic_check_values
 * Check for too big/unaligned periods and unaligned DMA buffer
 */
static int
atc_dma_cyclic_check_values(unsigned int reg_width, dma_addr_t buf_addr,
<<<<<<< HEAD
<<<<<<< HEAD
		size_t period_len, enum dma_data_direction direction)
=======
		size_t period_len, enum dma_transfer_direction direction)
>>>>>>> refs/remotes/origin/cm-10.0
=======
		size_t period_len)
>>>>>>> refs/remotes/origin/master
{
	if (period_len > (ATC_BTSIZE_MAX << reg_width))
		goto err_out;
	if (unlikely(period_len & ((1 << reg_width) - 1)))
		goto err_out;
	if (unlikely(buf_addr & ((1 << reg_width) - 1)))
		goto err_out;
<<<<<<< HEAD
<<<<<<< HEAD
	if (unlikely(!(direction & (DMA_TO_DEVICE | DMA_FROM_DEVICE))))
=======
	if (unlikely(!(direction & (DMA_DEV_TO_MEM | DMA_MEM_TO_DEV))))
>>>>>>> refs/remotes/origin/cm-10.0
		goto err_out;
=======
>>>>>>> refs/remotes/origin/master

	return 0;

err_out:
	return -EINVAL;
}

/**
<<<<<<< HEAD
 * atc_dma_cyclic_fill_desc - Fill one period decriptor
 */
static int
<<<<<<< HEAD
atc_dma_cyclic_fill_desc(struct at_dma_slave *atslave, struct at_desc *desc,
		unsigned int period_index, dma_addr_t buf_addr,
		size_t period_len, enum dma_data_direction direction)
{
	u32		ctrla;
	unsigned int	reg_width = atslave->reg_width;
=======
=======
 * atc_dma_cyclic_fill_desc - Fill one period descriptor
 */
static int
>>>>>>> refs/remotes/origin/master
atc_dma_cyclic_fill_desc(struct dma_chan *chan, struct at_desc *desc,
		unsigned int period_index, dma_addr_t buf_addr,
		unsigned int reg_width, size_t period_len,
		enum dma_transfer_direction direction)
{
	struct at_dma_chan	*atchan = to_at_dma_chan(chan);
<<<<<<< HEAD
	struct at_dma_slave	*atslave = chan->private;
	struct dma_slave_config	*sconfig = &atchan->dma_sconfig;
	u32			ctrla;
>>>>>>> refs/remotes/origin/cm-10.0

	/* prepare common CRTLA value */
	ctrla =   ATC_DEFAULT_CTRLA | atslave->ctrla
=======
	struct dma_slave_config	*sconfig = &atchan->dma_sconfig;
	u32			ctrla;

	/* prepare common CRTLA value */
	ctrla =   ATC_SCSIZE(sconfig->src_maxburst)
		| ATC_DCSIZE(sconfig->dst_maxburst)
>>>>>>> refs/remotes/origin/master
		| ATC_DST_WIDTH(reg_width)
		| ATC_SRC_WIDTH(reg_width)
		| period_len >> reg_width;

	switch (direction) {
<<<<<<< HEAD
<<<<<<< HEAD
	case DMA_TO_DEVICE:
		desc->lli.saddr = buf_addr + (period_len * period_index);
		desc->lli.daddr = atslave->tx_reg;
=======
	case DMA_MEM_TO_DEV:
		desc->lli.saddr = buf_addr + (period_len * period_index);
		desc->lli.daddr = sconfig->dst_addr;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	case DMA_MEM_TO_DEV:
		desc->lli.saddr = buf_addr + (period_len * period_index);
		desc->lli.daddr = sconfig->dst_addr;
>>>>>>> refs/remotes/origin/master
		desc->lli.ctrla = ctrla;
		desc->lli.ctrlb = ATC_DST_ADDR_MODE_FIXED
				| ATC_SRC_ADDR_MODE_INCR
				| ATC_FC_MEM2PER
<<<<<<< HEAD
				| ATC_SIF(AT_DMA_MEM_IF)
				| ATC_DIF(AT_DMA_PER_IF);
		break;

<<<<<<< HEAD
	case DMA_FROM_DEVICE:
		desc->lli.saddr = atslave->rx_reg;
=======
	case DMA_DEV_TO_MEM:
		desc->lli.saddr = sconfig->src_addr;
>>>>>>> refs/remotes/origin/cm-10.0
=======
				| ATC_SIF(atchan->mem_if)
				| ATC_DIF(atchan->per_if);
		break;

	case DMA_DEV_TO_MEM:
		desc->lli.saddr = sconfig->src_addr;
>>>>>>> refs/remotes/origin/master
		desc->lli.daddr = buf_addr + (period_len * period_index);
		desc->lli.ctrla = ctrla;
		desc->lli.ctrlb = ATC_DST_ADDR_MODE_INCR
				| ATC_SRC_ADDR_MODE_FIXED
				| ATC_FC_PER2MEM
<<<<<<< HEAD
				| ATC_SIF(AT_DMA_PER_IF)
				| ATC_DIF(AT_DMA_MEM_IF);
=======
				| ATC_SIF(atchan->per_if)
				| ATC_DIF(atchan->mem_if);
>>>>>>> refs/remotes/origin/master
		break;

	default:
		return -EINVAL;
	}

	return 0;
}

/**
 * atc_prep_dma_cyclic - prepare the cyclic DMA transfer
 * @chan: the DMA channel to prepare
 * @buf_addr: physical DMA address where the buffer starts
 * @buf_len: total number of bytes for the entire buffer
 * @period_len: number of bytes for each period
 * @direction: transfer direction, to or from device
<<<<<<< HEAD
<<<<<<< HEAD
 */
static struct dma_async_tx_descriptor *
atc_prep_dma_cyclic(struct dma_chan *chan, dma_addr_t buf_addr, size_t buf_len,
		size_t period_len, enum dma_data_direction direction)
{
	struct at_dma_chan	*atchan = to_at_dma_chan(chan);
	struct at_dma_slave	*atslave = chan->private;
	struct at_desc		*first = NULL;
	struct at_desc		*prev = NULL;
	unsigned long		was_cyclic;
=======
=======
 * @flags: tx descriptor status flags
>>>>>>> refs/remotes/origin/master
 * @context: transfer context (ignored)
 */
static struct dma_async_tx_descriptor *
atc_prep_dma_cyclic(struct dma_chan *chan, dma_addr_t buf_addr, size_t buf_len,
		size_t period_len, enum dma_transfer_direction direction,
<<<<<<< HEAD
		void *context)
=======
		unsigned long flags, void *context)
>>>>>>> refs/remotes/origin/master
{
	struct at_dma_chan	*atchan = to_at_dma_chan(chan);
	struct at_dma_slave	*atslave = chan->private;
	struct dma_slave_config	*sconfig = &atchan->dma_sconfig;
	struct at_desc		*first = NULL;
	struct at_desc		*prev = NULL;
	unsigned long		was_cyclic;
	unsigned int		reg_width;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	unsigned int		periods = buf_len / period_len;
	unsigned int		i;

	dev_vdbg(chan2dev(chan), "prep_dma_cyclic: %s buf@0x%08x - %d (%d/%d)\n",
<<<<<<< HEAD
<<<<<<< HEAD
			direction == DMA_TO_DEVICE ? "TO DEVICE" : "FROM DEVICE",
=======
			direction == DMA_MEM_TO_DEV ? "TO DEVICE" : "FROM DEVICE",
>>>>>>> refs/remotes/origin/cm-10.0
=======
			direction == DMA_MEM_TO_DEV ? "TO DEVICE" : "FROM DEVICE",
>>>>>>> refs/remotes/origin/master
			buf_addr,
			periods, buf_len, period_len);

	if (unlikely(!atslave || !buf_len || !period_len)) {
		dev_dbg(chan2dev(chan), "prep_dma_cyclic: length is zero!\n");
		return NULL;
	}

	was_cyclic = test_and_set_bit(ATC_IS_CYCLIC, &atchan->status);
	if (was_cyclic) {
		dev_dbg(chan2dev(chan), "prep_dma_cyclic: channel in use!\n");
		return NULL;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	/* Check for too big/unaligned periods and unaligned DMA buffer */
	if (atc_dma_cyclic_check_values(atslave->reg_width, buf_addr,
=======
=======
	if (unlikely(!is_slave_direction(direction)))
		goto err_out;

>>>>>>> refs/remotes/origin/master
	if (sconfig->direction == DMA_MEM_TO_DEV)
		reg_width = convert_buswidth(sconfig->dst_addr_width);
	else
		reg_width = convert_buswidth(sconfig->src_addr_width);

	/* Check for too big/unaligned periods and unaligned DMA buffer */
<<<<<<< HEAD
	if (atc_dma_cyclic_check_values(reg_width, buf_addr,
>>>>>>> refs/remotes/origin/cm-10.0
					period_len, direction))
=======
	if (atc_dma_cyclic_check_values(reg_width, buf_addr, period_len))
>>>>>>> refs/remotes/origin/master
		goto err_out;

	/* build cyclic linked list */
	for (i = 0; i < periods; i++) {
		struct at_desc	*desc;

		desc = atc_desc_get(atchan);
		if (!desc)
			goto err_desc_get;

<<<<<<< HEAD
<<<<<<< HEAD
		if (atc_dma_cyclic_fill_desc(atslave, desc, i, buf_addr,
						period_len, direction))
=======
		if (atc_dma_cyclic_fill_desc(chan, desc, i, buf_addr,
					     reg_width, period_len, direction))
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (atc_dma_cyclic_fill_desc(chan, desc, i, buf_addr,
					     reg_width, period_len, direction))
>>>>>>> refs/remotes/origin/master
			goto err_desc_get;

		atc_desc_chain(&first, &prev, desc);
	}

	/* lets make a cyclic list */
	prev->lli.dscr = first->txd.phys;

	/* First descriptor of the chain embedds additional information */
	first->txd.cookie = -EBUSY;
	first->len = buf_len;
<<<<<<< HEAD
=======
	first->tx_width = reg_width;
>>>>>>> refs/remotes/origin/master

	return &first->txd;

err_desc_get:
	dev_err(chan2dev(chan), "not enough descriptors available\n");
	atc_desc_put(atchan, first);
err_out:
	clear_bit(ATC_IS_CYCLIC, &atchan->status);
	return NULL;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static int set_runtime_config(struct dma_chan *chan,
			      struct dma_slave_config *sconfig)
{
	struct at_dma_chan	*atchan = to_at_dma_chan(chan);

	/* Check if it is chan is configured for slave transfers */
	if (!chan->private)
		return -EINVAL;

	memcpy(&atchan->dma_sconfig, sconfig, sizeof(*sconfig));

	convert_burst(&atchan->dma_sconfig.src_maxburst);
	convert_burst(&atchan->dma_sconfig.dst_maxburst);

	return 0;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

static int atc_control(struct dma_chan *chan, enum dma_ctrl_cmd cmd,
		       unsigned long arg)
{
	struct at_dma_chan	*atchan = to_at_dma_chan(chan);
	struct at_dma		*atdma = to_at_dma(chan->device);
	int			chan_id = atchan->chan_common.chan_id;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	unsigned long		flags;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	unsigned long		flags;
>>>>>>> refs/remotes/origin/master

	LIST_HEAD(list);

	dev_vdbg(chan2dev(chan), "atc_control (%d)\n", cmd);

	if (cmd == DMA_PAUSE) {
<<<<<<< HEAD
<<<<<<< HEAD
		spin_lock_bh(&atchan->lock);
=======
		spin_lock_irqsave(&atchan->lock, flags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		spin_lock_irqsave(&atchan->lock, flags);
>>>>>>> refs/remotes/origin/master

		dma_writel(atdma, CHER, AT_DMA_SUSP(chan_id));
		set_bit(ATC_IS_PAUSED, &atchan->status);

<<<<<<< HEAD
<<<<<<< HEAD
		spin_unlock_bh(&atchan->lock);
	} else if (cmd == DMA_RESUME) {
		if (!test_bit(ATC_IS_PAUSED, &atchan->status))
			return 0;

		spin_lock_bh(&atchan->lock);
=======
=======
>>>>>>> refs/remotes/origin/master
		spin_unlock_irqrestore(&atchan->lock, flags);
	} else if (cmd == DMA_RESUME) {
		if (!atc_chan_is_paused(atchan))
			return 0;

		spin_lock_irqsave(&atchan->lock, flags);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

		dma_writel(atdma, CHDR, AT_DMA_RES(chan_id));
		clear_bit(ATC_IS_PAUSED, &atchan->status);

<<<<<<< HEAD
<<<<<<< HEAD
		spin_unlock_bh(&atchan->lock);
=======
		spin_unlock_irqrestore(&atchan->lock, flags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		spin_unlock_irqrestore(&atchan->lock, flags);
>>>>>>> refs/remotes/origin/master
	} else if (cmd == DMA_TERMINATE_ALL) {
		struct at_desc	*desc, *_desc;
		/*
		 * This is only called when something went wrong elsewhere, so
		 * we don't really care about the data. Just disable the
		 * channel. We still have to poll the channel enable bit due
		 * to AHB/HSB limitations.
		 */
<<<<<<< HEAD
<<<<<<< HEAD
		spin_lock_bh(&atchan->lock);
=======
		spin_lock_irqsave(&atchan->lock, flags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		spin_lock_irqsave(&atchan->lock, flags);
>>>>>>> refs/remotes/origin/master

		/* disabling channel: must also remove suspend state */
		dma_writel(atdma, CHDR, AT_DMA_RES(chan_id) | atchan->mask);

		/* confirm that this channel is disabled */
		while (dma_readl(atdma, CHSR) & atchan->mask)
			cpu_relax();

		/* active_list entries will end up before queued entries */
		list_splice_init(&atchan->queue, &list);
		list_splice_init(&atchan->active_list, &list);

		/* Flush all pending and queued descriptors */
		list_for_each_entry_safe(desc, _desc, &list, desc_node)
			atc_chain_complete(atchan, desc);

		clear_bit(ATC_IS_PAUSED, &atchan->status);
		/* if channel dedicated to cyclic operations, free it */
		clear_bit(ATC_IS_CYCLIC, &atchan->status);

<<<<<<< HEAD
<<<<<<< HEAD
		spin_unlock_bh(&atchan->lock);
=======
		spin_unlock_irqrestore(&atchan->lock, flags);
	} else if (cmd == DMA_SLAVE_CONFIG) {
		return set_runtime_config(chan, (struct dma_slave_config *)arg);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		spin_unlock_irqrestore(&atchan->lock, flags);
	} else if (cmd == DMA_SLAVE_CONFIG) {
		return set_runtime_config(chan, (struct dma_slave_config *)arg);
>>>>>>> refs/remotes/origin/master
	} else {
		return -ENXIO;
	}

	return 0;
}

/**
 * atc_tx_status - poll for transaction completion
 * @chan: DMA channel
 * @cookie: transaction identifier to check status of
 * @txstate: if not %NULL updated with transaction state
 *
 * If @txstate is passed in, upon return it reflect the driver
 * internal state and can be used with dma_async_is_complete() to check
 * the status of multiple cookies without re-checking hardware state.
 */
static enum dma_status
atc_tx_status(struct dma_chan *chan,
		dma_cookie_t cookie,
		struct dma_tx_state *txstate)
{
	struct at_dma_chan	*atchan = to_at_dma_chan(chan);
<<<<<<< HEAD
	dma_cookie_t		last_used;
	dma_cookie_t		last_complete;
<<<<<<< HEAD
	enum dma_status		ret;

	spin_lock_bh(&atchan->lock);

	last_complete = atchan->completed_cookie;
	last_used = chan->cookie;

	ret = dma_async_is_complete(cookie, last_complete, last_used);
	if (ret != DMA_SUCCESS) {
		atc_cleanup_descriptors(atchan);

		last_complete = atchan->completed_cookie;
		last_used = chan->cookie;

		ret = dma_async_is_complete(cookie, last_complete, last_used);
	}

	spin_unlock_bh(&atchan->lock);

	if (ret != DMA_SUCCESS)
		dma_set_tx_state(txstate, last_complete, last_used,
			atc_first_active(atchan)->len);
	else
		dma_set_tx_state(txstate, last_complete, last_used, 0);

	if (test_bit(ATC_IS_PAUSED, &atchan->status))
=======
	unsigned long		flags;
	enum dma_status		ret;

	spin_lock_irqsave(&atchan->lock, flags);

	ret = dma_cookie_status(chan, cookie, txstate);
	if (ret != DMA_SUCCESS) {
		atc_cleanup_descriptors(atchan);

		ret = dma_cookie_status(chan, cookie, txstate);
	}

	last_complete = chan->completed_cookie;
	last_used = chan->cookie;

	spin_unlock_irqrestore(&atchan->lock, flags);

	if (ret != DMA_SUCCESS)
		dma_set_residue(txstate, atc_first_active(atchan)->len);

	if (atc_chan_is_paused(atchan))
>>>>>>> refs/remotes/origin/cm-10.0
		ret = DMA_PAUSED;

	dev_vdbg(chan2dev(chan), "tx_status %d: cookie = %d (d%d, u%d)\n",
		 ret, cookie, last_complete ? last_complete : 0,
		 last_used ? last_used : 0);
=======
	unsigned long		flags;
	enum dma_status		ret;
	int bytes = 0;

	ret = dma_cookie_status(chan, cookie, txstate);
	if (ret == DMA_COMPLETE)
		return ret;
	/*
	 * There's no point calculating the residue if there's
	 * no txstate to store the value.
	 */
	if (!txstate)
		return DMA_ERROR;

	spin_lock_irqsave(&atchan->lock, flags);

	/*  Get number of bytes left in the active transactions */
	bytes = atc_get_bytes_left(chan);

	spin_unlock_irqrestore(&atchan->lock, flags);

	if (unlikely(bytes < 0)) {
		dev_vdbg(chan2dev(chan), "get residual bytes error\n");
		return DMA_ERROR;
	} else {
		dma_set_residue(txstate, bytes);
	}

	dev_vdbg(chan2dev(chan), "tx_status %d: cookie = %d residue = %d\n",
		 ret, cookie, bytes);
>>>>>>> refs/remotes/origin/master

	return ret;
}

/**
 * atc_issue_pending - try to finish work
 * @chan: target DMA channel
 */
static void atc_issue_pending(struct dma_chan *chan)
{
	struct at_dma_chan	*atchan = to_at_dma_chan(chan);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	unsigned long		flags;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	unsigned long		flags;
>>>>>>> refs/remotes/origin/master

	dev_vdbg(chan2dev(chan), "issue_pending\n");

	/* Not needed for cyclic transfers */
<<<<<<< HEAD
<<<<<<< HEAD
	if (test_bit(ATC_IS_CYCLIC, &atchan->status))
		return;

	spin_lock_bh(&atchan->lock);
	if (!atc_chan_is_enabled(atchan)) {
		atc_advance_work(atchan);
	}
	spin_unlock_bh(&atchan->lock);
=======
=======
>>>>>>> refs/remotes/origin/master
	if (atc_chan_is_cyclic(atchan))
		return;

	spin_lock_irqsave(&atchan->lock, flags);
<<<<<<< HEAD
	if (!atc_chan_is_enabled(atchan)) {
		atc_advance_work(atchan);
	}
	spin_unlock_irqrestore(&atchan->lock, flags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	atc_advance_work(atchan);
	spin_unlock_irqrestore(&atchan->lock, flags);
>>>>>>> refs/remotes/origin/master
}

/**
 * atc_alloc_chan_resources - allocate resources for DMA channel
 * @chan: allocate descriptor resources for this channel
 * @client: current client requesting the channel be ready for requests
 *
 * return - the number of allocated descriptors
 */
static int atc_alloc_chan_resources(struct dma_chan *chan)
{
	struct at_dma_chan	*atchan = to_at_dma_chan(chan);
	struct at_dma		*atdma = to_at_dma(chan->device);
	struct at_desc		*desc;
	struct at_dma_slave	*atslave;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	unsigned long		flags;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	unsigned long		flags;
>>>>>>> refs/remotes/origin/master
	int			i;
	u32			cfg;
	LIST_HEAD(tmp_list);

	dev_vdbg(chan2dev(chan), "alloc_chan_resources\n");

	/* ASSERT:  channel is idle */
	if (atc_chan_is_enabled(atchan)) {
		dev_dbg(chan2dev(chan), "DMA channel not idle ?\n");
		return -EIO;
	}

	cfg = ATC_DEFAULT_CFG;

	atslave = chan->private;
	if (atslave) {
		/*
		 * We need controller-specific data to set up slave
		 * transfers.
		 */
		BUG_ON(!atslave->dma_dev || atslave->dma_dev != atdma->dma_common.dev);

<<<<<<< HEAD
		/* if cfg configuration specified take it instad of default */
=======
		/* if cfg configuration specified take it instead of default */
>>>>>>> refs/remotes/origin/master
		if (atslave->cfg)
			cfg = atslave->cfg;
	}

	/* have we already been set up?
	 * reconfigure channel but no need to reallocate descriptors */
	if (!list_empty(&atchan->free_list))
		return atchan->descs_allocated;

	/* Allocate initial pool of descriptors */
	for (i = 0; i < init_nr_desc_per_channel; i++) {
		desc = atc_alloc_descriptor(chan, GFP_KERNEL);
		if (!desc) {
			dev_err(atdma->dma_common.dev,
				"Only %d initial descriptors\n", i);
			break;
		}
		list_add_tail(&desc->desc_node, &tmp_list);
	}

<<<<<<< HEAD
<<<<<<< HEAD
	spin_lock_bh(&atchan->lock);
	atchan->descs_allocated = i;
	list_splice(&tmp_list, &atchan->free_list);
	atchan->completed_cookie = chan->cookie = 1;
	spin_unlock_bh(&atchan->lock);
=======
	spin_lock_irqsave(&atchan->lock, flags);
	atchan->descs_allocated = i;
	list_splice(&tmp_list, &atchan->free_list);
	dma_cookie_init(chan);
	spin_unlock_irqrestore(&atchan->lock, flags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	spin_lock_irqsave(&atchan->lock, flags);
	atchan->descs_allocated = i;
	atchan->remain_desc = 0;
	list_splice(&tmp_list, &atchan->free_list);
	dma_cookie_init(chan);
	spin_unlock_irqrestore(&atchan->lock, flags);
>>>>>>> refs/remotes/origin/master

	/* channel parameters */
	channel_writel(atchan, CFG, cfg);

	dev_dbg(chan2dev(chan),
		"alloc_chan_resources: allocated %d descriptors\n",
		atchan->descs_allocated);

	return atchan->descs_allocated;
}

/**
 * atc_free_chan_resources - free all channel resources
 * @chan: DMA channel
 */
static void atc_free_chan_resources(struct dma_chan *chan)
{
	struct at_dma_chan	*atchan = to_at_dma_chan(chan);
	struct at_dma		*atdma = to_at_dma(chan->device);
	struct at_desc		*desc, *_desc;
	LIST_HEAD(list);

	dev_dbg(chan2dev(chan), "free_chan_resources: (descs allocated=%u)\n",
		atchan->descs_allocated);

	/* ASSERT:  channel is idle */
	BUG_ON(!list_empty(&atchan->active_list));
	BUG_ON(!list_empty(&atchan->queue));
	BUG_ON(atc_chan_is_enabled(atchan));

	list_for_each_entry_safe(desc, _desc, &atchan->free_list, desc_node) {
		dev_vdbg(chan2dev(chan), "  freeing descriptor %p\n", desc);
		list_del(&desc->desc_node);
		/* free link descriptor */
		dma_pool_free(atdma->dma_desc_pool, desc, desc->txd.phys);
	}
	list_splice_init(&atchan->free_list, &list);
	atchan->descs_allocated = 0;
	atchan->status = 0;
<<<<<<< HEAD
=======
	atchan->remain_desc = 0;
>>>>>>> refs/remotes/origin/master

	dev_vdbg(chan2dev(chan), "free_chan_resources: done\n");
}

<<<<<<< HEAD

/*--  Module Management  -----------------------------------------------*/

<<<<<<< HEAD
=======
=======
#ifdef CONFIG_OF
static bool at_dma_filter(struct dma_chan *chan, void *slave)
{
	struct at_dma_slave *atslave = slave;

	if (atslave->dma_dev == chan->device->dev) {
		chan->private = atslave;
		return true;
	} else {
		return false;
	}
}

static struct dma_chan *at_dma_xlate(struct of_phandle_args *dma_spec,
				     struct of_dma *of_dma)
{
	struct dma_chan *chan;
	struct at_dma_chan *atchan;
	struct at_dma_slave *atslave;
	dma_cap_mask_t mask;
	unsigned int per_id;
	struct platform_device *dmac_pdev;

	if (dma_spec->args_count != 2)
		return NULL;

	dmac_pdev = of_find_device_by_node(dma_spec->np);

	dma_cap_zero(mask);
	dma_cap_set(DMA_SLAVE, mask);

	atslave = devm_kzalloc(&dmac_pdev->dev, sizeof(*atslave), GFP_KERNEL);
	if (!atslave)
		return NULL;

	atslave->cfg = ATC_DST_H2SEL_HW | ATC_SRC_H2SEL_HW;
	/*
	 * We can fill both SRC_PER and DST_PER, one of these fields will be
	 * ignored depending on DMA transfer direction.
	 */
	per_id = dma_spec->args[1] & AT91_DMA_CFG_PER_ID_MASK;
	atslave->cfg |= ATC_DST_PER_MSB(per_id) | ATC_DST_PER(per_id)
		     | ATC_SRC_PER_MSB(per_id) | ATC_SRC_PER(per_id);
	/*
	 * We have to translate the value we get from the device tree since
	 * the half FIFO configuration value had to be 0 to keep backward
	 * compatibility.
	 */
	switch (dma_spec->args[1] & AT91_DMA_CFG_FIFOCFG_MASK) {
	case AT91_DMA_CFG_FIFOCFG_ALAP:
		atslave->cfg |= ATC_FIFOCFG_LARGESTBURST;
		break;
	case AT91_DMA_CFG_FIFOCFG_ASAP:
		atslave->cfg |= ATC_FIFOCFG_ENOUGHSPACE;
		break;
	case AT91_DMA_CFG_FIFOCFG_HALF:
	default:
		atslave->cfg |= ATC_FIFOCFG_HALFFIFO;
	}
	atslave->dma_dev = &dmac_pdev->dev;

	chan = dma_request_channel(mask, at_dma_filter, atslave);
	if (!chan)
		return NULL;

	atchan = to_at_dma_chan(chan);
	atchan->per_if = dma_spec->args[0] & 0xff;
	atchan->mem_if = (dma_spec->args[0] >> 16) & 0xff;

	return chan;
}
#else
static struct dma_chan *at_dma_xlate(struct of_phandle_args *dma_spec,
				     struct of_dma *of_dma)
{
	return NULL;
}
#endif

/*--  Module Management  -----------------------------------------------*/

>>>>>>> refs/remotes/origin/master
/* cap_mask is a multi-u32 bitfield, fill it with proper C code. */
static struct at_dma_platform_data at91sam9rl_config = {
	.nr_channels = 2,
};
static struct at_dma_platform_data at91sam9g45_config = {
	.nr_channels = 8,
};

#if defined(CONFIG_OF)
static const struct of_device_id atmel_dma_dt_ids[] = {
	{
		.compatible = "atmel,at91sam9rl-dma",
		.data = &at91sam9rl_config,
	}, {
		.compatible = "atmel,at91sam9g45-dma",
		.data = &at91sam9g45_config,
	}, {
		/* sentinel */
	}
};

MODULE_DEVICE_TABLE(of, atmel_dma_dt_ids);
#endif

static const struct platform_device_id atdma_devtypes[] = {
	{
		.name = "at91sam9rl_dma",
		.driver_data = (unsigned long) &at91sam9rl_config,
	}, {
		.name = "at91sam9g45_dma",
		.driver_data = (unsigned long) &at91sam9g45_config,
	}, {
		/* sentinel */
	}
};

<<<<<<< HEAD
static inline struct at_dma_platform_data * __init at_dma_get_driver_data(
=======
static inline const struct at_dma_platform_data * __init at_dma_get_driver_data(
>>>>>>> refs/remotes/origin/master
						struct platform_device *pdev)
{
	if (pdev->dev.of_node) {
		const struct of_device_id *match;
		match = of_match_node(atmel_dma_dt_ids, pdev->dev.of_node);
		if (match == NULL)
			return NULL;
		return match->data;
	}
	return (struct at_dma_platform_data *)
			platform_get_device_id(pdev)->driver_data;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/**
 * at_dma_off - disable DMA controller
 * @atdma: the Atmel HDAMC device
 */
static void at_dma_off(struct at_dma *atdma)
{
	dma_writel(atdma, EN, 0);

	/* disable all interrupts */
	dma_writel(atdma, EBCIDR, -1L);

	/* confirm that all channels are disabled */
	while (dma_readl(atdma, CHSR) & atdma->all_chan_mask)
		cpu_relax();
}

static int __init at_dma_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct at_dma_platform_data *pdata;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	struct resource		*io;
	struct at_dma		*atdma;
	size_t			size;
	int			irq;
	int			err;
	int			i;
<<<<<<< HEAD
<<<<<<< HEAD

	/* get DMA Controller parameters from platform */
	pdata = pdev->dev.platform_data;
	if (!pdata || pdata->nr_channels > AT_DMA_MAX_NR_CHANNELS)
		return -EINVAL;
=======
	struct at_dma_platform_data *plat_dat;
=======
	const struct at_dma_platform_data *plat_dat;
>>>>>>> refs/remotes/origin/master

	/* setup platform data for each SoC */
	dma_cap_set(DMA_MEMCPY, at91sam9rl_config.cap_mask);
	dma_cap_set(DMA_MEMCPY, at91sam9g45_config.cap_mask);
	dma_cap_set(DMA_SLAVE, at91sam9g45_config.cap_mask);

	/* get DMA parameters from controller type */
	plat_dat = at_dma_get_driver_data(pdev);
	if (!plat_dat)
		return -ENODEV;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	io = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!io)
		return -EINVAL;

	irq = platform_get_irq(pdev, 0);
	if (irq < 0)
		return irq;

	size = sizeof(struct at_dma);
<<<<<<< HEAD
<<<<<<< HEAD
	size += pdata->nr_channels * sizeof(struct at_dma_chan);
=======
	size += plat_dat->nr_channels * sizeof(struct at_dma_chan);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	size += plat_dat->nr_channels * sizeof(struct at_dma_chan);
>>>>>>> refs/remotes/origin/master
	atdma = kzalloc(size, GFP_KERNEL);
	if (!atdma)
		return -ENOMEM;

<<<<<<< HEAD
<<<<<<< HEAD
	/* discover transaction capabilites from the platform data */
	atdma->dma_common.cap_mask = pdata->cap_mask;
	atdma->all_chan_mask = (1 << pdata->nr_channels) - 1;

	size = io->end - io->start + 1;
=======
=======
>>>>>>> refs/remotes/origin/master
	/* discover transaction capabilities */
	atdma->dma_common.cap_mask = plat_dat->cap_mask;
	atdma->all_chan_mask = (1 << plat_dat->nr_channels) - 1;

	size = resource_size(io);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (!request_mem_region(io->start, size, pdev->dev.driver->name)) {
		err = -EBUSY;
		goto err_kfree;
	}

	atdma->regs = ioremap(io->start, size);
	if (!atdma->regs) {
		err = -ENOMEM;
		goto err_release_r;
	}

	atdma->clk = clk_get(&pdev->dev, "dma_clk");
	if (IS_ERR(atdma->clk)) {
		err = PTR_ERR(atdma->clk);
		goto err_clk;
	}
<<<<<<< HEAD
	clk_enable(atdma->clk);
=======
	err = clk_prepare_enable(atdma->clk);
	if (err)
		goto err_clk_prepare;
>>>>>>> refs/remotes/origin/master

	/* force dma off, just in case */
	at_dma_off(atdma);

	err = request_irq(irq, at_dma_interrupt, 0, "at_hdmac", atdma);
	if (err)
		goto err_irq;

	platform_set_drvdata(pdev, atdma);

	/* create a pool of consistent memory blocks for hardware descriptors */
	atdma->dma_desc_pool = dma_pool_create("at_hdmac_desc_pool",
			&pdev->dev, sizeof(struct at_desc),
			4 /* word alignment */, 0);
	if (!atdma->dma_desc_pool) {
		dev_err(&pdev->dev, "No memory for descriptors dma pool\n");
		err = -ENOMEM;
		goto err_pool_create;
	}

	/* clear any pending interrupt */
	while (dma_readl(atdma, EBCISR))
		cpu_relax();

	/* initialize channels related values */
	INIT_LIST_HEAD(&atdma->dma_common.channels);
<<<<<<< HEAD
<<<<<<< HEAD
	for (i = 0; i < pdata->nr_channels; i++, atdma->dma_common.chancnt++) {
		struct at_dma_chan	*atchan = &atdma->chan[i];

		atchan->chan_common.device = &atdma->dma_common;
		atchan->chan_common.cookie = atchan->completed_cookie = 1;
		atchan->chan_common.chan_id = i;
=======
	for (i = 0; i < plat_dat->nr_channels; i++) {
		struct at_dma_chan	*atchan = &atdma->chan[i];

		atchan->chan_common.device = &atdma->dma_common;
		dma_cookie_init(&atchan->chan_common);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	for (i = 0; i < plat_dat->nr_channels; i++) {
		struct at_dma_chan	*atchan = &atdma->chan[i];

		atchan->mem_if = AT_DMA_MEM_IF;
		atchan->per_if = AT_DMA_PER_IF;
		atchan->chan_common.device = &atdma->dma_common;
		dma_cookie_init(&atchan->chan_common);
>>>>>>> refs/remotes/origin/master
		list_add_tail(&atchan->chan_common.device_node,
				&atdma->dma_common.channels);

		atchan->ch_regs = atdma->regs + ch_regs(i);
		spin_lock_init(&atchan->lock);
		atchan->mask = 1 << i;

		INIT_LIST_HEAD(&atchan->active_list);
		INIT_LIST_HEAD(&atchan->queue);
		INIT_LIST_HEAD(&atchan->free_list);

		tasklet_init(&atchan->tasklet, atc_tasklet,
				(unsigned long)atchan);
		atc_enable_chan_irq(atdma, i);
	}

	/* set base routines */
	atdma->dma_common.device_alloc_chan_resources = atc_alloc_chan_resources;
	atdma->dma_common.device_free_chan_resources = atc_free_chan_resources;
	atdma->dma_common.device_tx_status = atc_tx_status;
	atdma->dma_common.device_issue_pending = atc_issue_pending;
	atdma->dma_common.dev = &pdev->dev;

	/* set prep routines based on capability */
	if (dma_has_cap(DMA_MEMCPY, atdma->dma_common.cap_mask))
		atdma->dma_common.device_prep_dma_memcpy = atc_prep_dma_memcpy;

<<<<<<< HEAD
<<<<<<< HEAD
	if (dma_has_cap(DMA_SLAVE, atdma->dma_common.cap_mask))
		atdma->dma_common.device_prep_slave_sg = atc_prep_slave_sg;

	if (dma_has_cap(DMA_CYCLIC, atdma->dma_common.cap_mask))
		atdma->dma_common.device_prep_dma_cyclic = atc_prep_dma_cyclic;

	if (dma_has_cap(DMA_SLAVE, atdma->dma_common.cap_mask) ||
	    dma_has_cap(DMA_CYCLIC, atdma->dma_common.cap_mask))
		atdma->dma_common.device_control = atc_control;
=======
=======
>>>>>>> refs/remotes/origin/master
	if (dma_has_cap(DMA_SLAVE, atdma->dma_common.cap_mask)) {
		atdma->dma_common.device_prep_slave_sg = atc_prep_slave_sg;
		/* controller can do slave DMA: can trigger cyclic transfers */
		dma_cap_set(DMA_CYCLIC, atdma->dma_common.cap_mask);
		atdma->dma_common.device_prep_dma_cyclic = atc_prep_dma_cyclic;
		atdma->dma_common.device_control = atc_control;
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	dma_writel(atdma, EN, AT_DMA_ENABLE);

	dev_info(&pdev->dev, "Atmel AHB DMA Controller ( %s%s), %d channels\n",
	  dma_has_cap(DMA_MEMCPY, atdma->dma_common.cap_mask) ? "cpy " : "",
	  dma_has_cap(DMA_SLAVE, atdma->dma_common.cap_mask)  ? "slave " : "",
<<<<<<< HEAD
<<<<<<< HEAD
	  atdma->dma_common.chancnt);
=======
	  plat_dat->nr_channels);
>>>>>>> refs/remotes/origin/cm-10.0

	dma_async_device_register(&atdma->dma_common);

	return 0;

err_pool_create:
	platform_set_drvdata(pdev, NULL);
	free_irq(platform_get_irq(pdev, 0), atdma);
err_irq:
	clk_disable(atdma->clk);
=======
	  plat_dat->nr_channels);

	dma_async_device_register(&atdma->dma_common);

	/*
	 * Do not return an error if the dmac node is not present in order to
	 * not break the existing way of requesting channel with
	 * dma_request_channel().
	 */
	if (pdev->dev.of_node) {
		err = of_dma_controller_register(pdev->dev.of_node,
						 at_dma_xlate, atdma);
		if (err) {
			dev_err(&pdev->dev, "could not register of_dma_controller\n");
			goto err_of_dma_controller_register;
		}
	}

	return 0;

err_of_dma_controller_register:
	dma_async_device_unregister(&atdma->dma_common);
	dma_pool_destroy(atdma->dma_desc_pool);
err_pool_create:
	free_irq(platform_get_irq(pdev, 0), atdma);
err_irq:
	clk_disable_unprepare(atdma->clk);
err_clk_prepare:
>>>>>>> refs/remotes/origin/master
	clk_put(atdma->clk);
err_clk:
	iounmap(atdma->regs);
	atdma->regs = NULL;
err_release_r:
	release_mem_region(io->start, size);
err_kfree:
	kfree(atdma);
	return err;
}

<<<<<<< HEAD
static int __exit at_dma_remove(struct platform_device *pdev)
=======
static int at_dma_remove(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	struct at_dma		*atdma = platform_get_drvdata(pdev);
	struct dma_chan		*chan, *_chan;
	struct resource		*io;

	at_dma_off(atdma);
	dma_async_device_unregister(&atdma->dma_common);

	dma_pool_destroy(atdma->dma_desc_pool);
<<<<<<< HEAD
	platform_set_drvdata(pdev, NULL);
=======
>>>>>>> refs/remotes/origin/master
	free_irq(platform_get_irq(pdev, 0), atdma);

	list_for_each_entry_safe(chan, _chan, &atdma->dma_common.channels,
			device_node) {
		struct at_dma_chan	*atchan = to_at_dma_chan(chan);

		/* Disable interrupts */
		atc_disable_chan_irq(atdma, chan->chan_id);
		tasklet_disable(&atchan->tasklet);

		tasklet_kill(&atchan->tasklet);
		list_del(&chan->device_node);
	}

<<<<<<< HEAD
	clk_disable(atdma->clk);
=======
	clk_disable_unprepare(atdma->clk);
>>>>>>> refs/remotes/origin/master
	clk_put(atdma->clk);

	iounmap(atdma->regs);
	atdma->regs = NULL;

	io = platform_get_resource(pdev, IORESOURCE_MEM, 0);
<<<<<<< HEAD
<<<<<<< HEAD
	release_mem_region(io->start, io->end - io->start + 1);
=======
	release_mem_region(io->start, resource_size(io));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	release_mem_region(io->start, resource_size(io));
>>>>>>> refs/remotes/origin/master

	kfree(atdma);

	return 0;
}

static void at_dma_shutdown(struct platform_device *pdev)
{
	struct at_dma	*atdma = platform_get_drvdata(pdev);

	at_dma_off(platform_get_drvdata(pdev));
<<<<<<< HEAD
	clk_disable(atdma->clk);
}

<<<<<<< HEAD
=======
=======
	clk_disable_unprepare(atdma->clk);
}

>>>>>>> refs/remotes/origin/master
static int at_dma_prepare(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct at_dma *atdma = platform_get_drvdata(pdev);
	struct dma_chan *chan, *_chan;

	list_for_each_entry_safe(chan, _chan, &atdma->dma_common.channels,
			device_node) {
		struct at_dma_chan *atchan = to_at_dma_chan(chan);
		/* wait for transaction completion (except in cyclic case) */
		if (atc_chan_is_enabled(atchan) && !atc_chan_is_cyclic(atchan))
			return -EAGAIN;
	}
	return 0;
}

static void atc_suspend_cyclic(struct at_dma_chan *atchan)
{
	struct dma_chan	*chan = &atchan->chan_common;

	/* Channel should be paused by user
	 * do it anyway even if it is not done already */
	if (!atc_chan_is_paused(atchan)) {
		dev_warn(chan2dev(chan),
		"cyclic channel not paused, should be done by channel user\n");
		atc_control(chan, DMA_PAUSE, 0);
	}

	/* now preserve additional data for cyclic operations */
	/* next descriptor address in the cyclic list */
	atchan->save_dscr = channel_readl(atchan, DSCR);

	vdbg_dump_regs(atchan);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static int at_dma_suspend_noirq(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct at_dma *atdma = platform_get_drvdata(pdev);
<<<<<<< HEAD
<<<<<<< HEAD

	at_dma_off(platform_get_drvdata(pdev));
=======
=======
>>>>>>> refs/remotes/origin/master
	struct dma_chan *chan, *_chan;

	/* preserve data */
	list_for_each_entry_safe(chan, _chan, &atdma->dma_common.channels,
			device_node) {
		struct at_dma_chan *atchan = to_at_dma_chan(chan);

		if (atc_chan_is_cyclic(atchan))
			atc_suspend_cyclic(atchan);
		atchan->save_cfg = channel_readl(atchan, CFG);
	}
	atdma->save_imr = dma_readl(atdma, EBCIMR);

	/* disable DMA controller */
	at_dma_off(atdma);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	clk_disable(atdma->clk);
	return 0;
}

<<<<<<< HEAD
=======
=======
	clk_disable_unprepare(atdma->clk);
	return 0;
}

>>>>>>> refs/remotes/origin/master
static void atc_resume_cyclic(struct at_dma_chan *atchan)
{
	struct at_dma	*atdma = to_at_dma(atchan->chan_common.device);

	/* restore channel status for cyclic descriptors list:
	 * next descriptor in the cyclic list at the time of suspend */
	channel_writel(atchan, SADDR, 0);
	channel_writel(atchan, DADDR, 0);
	channel_writel(atchan, CTRLA, 0);
	channel_writel(atchan, CTRLB, 0);
	channel_writel(atchan, DSCR, atchan->save_dscr);
	dma_writel(atdma, CHER, atchan->mask);

	/* channel pause status should be removed by channel user
	 * We cannot take the initiative to do it here */

	vdbg_dump_regs(atchan);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static int at_dma_resume_noirq(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct at_dma *atdma = platform_get_drvdata(pdev);
<<<<<<< HEAD
<<<<<<< HEAD

	clk_enable(atdma->clk);
	dma_writel(atdma, EN, AT_DMA_ENABLE);
=======
	struct dma_chan *chan, *_chan;

	/* bring back DMA controller */
	clk_enable(atdma->clk);
=======
	struct dma_chan *chan, *_chan;

	/* bring back DMA controller */
	clk_prepare_enable(atdma->clk);
>>>>>>> refs/remotes/origin/master
	dma_writel(atdma, EN, AT_DMA_ENABLE);

	/* clear any pending interrupt */
	while (dma_readl(atdma, EBCISR))
		cpu_relax();

	/* restore saved data */
	dma_writel(atdma, EBCIER, atdma->save_imr);
	list_for_each_entry_safe(chan, _chan, &atdma->dma_common.channels,
			device_node) {
		struct at_dma_chan *atchan = to_at_dma_chan(chan);

		channel_writel(atchan, CFG, atchan->save_cfg);
		if (atc_chan_is_cyclic(atchan))
			atc_resume_cyclic(atchan);
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return 0;
}

static const struct dev_pm_ops at_dma_dev_pm_ops = {
<<<<<<< HEAD
<<<<<<< HEAD
=======
	.prepare = at_dma_prepare,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.prepare = at_dma_prepare,
>>>>>>> refs/remotes/origin/master
	.suspend_noirq = at_dma_suspend_noirq,
	.resume_noirq = at_dma_resume_noirq,
};

static struct platform_driver at_dma_driver = {
<<<<<<< HEAD
	.remove		= __exit_p(at_dma_remove),
	.shutdown	= at_dma_shutdown,
<<<<<<< HEAD
	.driver = {
		.name	= "at_hdmac",
		.pm	= &at_dma_dev_pm_ops,
=======
=======
	.remove		= at_dma_remove,
	.shutdown	= at_dma_shutdown,
>>>>>>> refs/remotes/origin/master
	.id_table	= atdma_devtypes,
	.driver = {
		.name	= "at_hdmac",
		.pm	= &at_dma_dev_pm_ops,
		.of_match_table	= of_match_ptr(atmel_dma_dt_ids),
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	},
};

static int __init at_dma_init(void)
{
	return platform_driver_probe(&at_dma_driver, at_dma_probe);
}
subsys_initcall(at_dma_init);

static void __exit at_dma_exit(void)
{
	platform_driver_unregister(&at_dma_driver);
}
module_exit(at_dma_exit);

MODULE_DESCRIPTION("Atmel AHB DMA Controller driver");
MODULE_AUTHOR("Nicolas Ferre <nicolas.ferre@atmel.com>");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:at_hdmac");
