/*
 * xor offload engine api
 *
 * Copyright © 2006, Intel Corporation.
 *
 *      Dan Williams <dan.j.williams@intel.com>
 *
 *      with architecture considerations by:
 *      Neil Brown <neilb@suse.de>
 *      Jeff Garzik <jeff@garzik.org>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */
#include <linux/kernel.h>
#include <linux/interrupt.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/master
#include <linux/mm.h>
#include <linux/dma-mapping.h>
#include <linux/raid/xor.h>
#include <linux/async_tx.h>

/* do_async_xor - dma map the pages and perform the xor with an engine */
static __async_inline struct dma_async_tx_descriptor *
<<<<<<< HEAD
do_async_xor(struct dma_chan *chan, struct page *dest, struct page **src_list,
	     unsigned int offset, int src_cnt, size_t len, dma_addr_t *dma_src,
=======
do_async_xor(struct dma_chan *chan, struct dmaengine_unmap_data *unmap,
>>>>>>> refs/remotes/origin/master
	     struct async_submit_ctl *submit)
{
	struct dma_device *dma = chan->device;
	struct dma_async_tx_descriptor *tx = NULL;
<<<<<<< HEAD
	int src_off = 0;
	int i;
	dma_async_tx_callback cb_fn_orig = submit->cb_fn;
	void *cb_param_orig = submit->cb_param;
	enum async_tx_flags flags_orig = submit->flags;
	enum dma_ctrl_flags dma_flags;
	int xor_src_cnt = 0;
	dma_addr_t dma_dest;

	/* map the dest bidrectional in case it is re-used as a source */
	dma_dest = dma_map_page(dma->dev, dest, offset, len, DMA_BIDIRECTIONAL);
	for (i = 0; i < src_cnt; i++) {
		/* only map the dest once */
		if (!src_list[i])
			continue;
		if (unlikely(src_list[i] == dest)) {
			dma_src[xor_src_cnt++] = dma_dest;
			continue;
		}
		dma_src[xor_src_cnt++] = dma_map_page(dma->dev, src_list[i], offset,
						      len, DMA_TO_DEVICE);
	}
	src_cnt = xor_src_cnt;

	while (src_cnt) {
		submit->flags = flags_orig;
		dma_flags = 0;
		xor_src_cnt = min(src_cnt, (int)dma->max_xor);
		/* if we are submitting additional xors, leave the chain open,
		 * clear the callback parameters, and leave the destination
		 * buffer mapped
=======
	dma_async_tx_callback cb_fn_orig = submit->cb_fn;
	void *cb_param_orig = submit->cb_param;
	enum async_tx_flags flags_orig = submit->flags;
	enum dma_ctrl_flags dma_flags = 0;
	int src_cnt = unmap->to_cnt;
	int xor_src_cnt;
	dma_addr_t dma_dest = unmap->addr[unmap->to_cnt];
	dma_addr_t *src_list = unmap->addr;

	while (src_cnt) {
		dma_addr_t tmp;

		submit->flags = flags_orig;
		xor_src_cnt = min(src_cnt, (int)dma->max_xor);
		/* if we are submitting additional xors, leave the chain open
		 * and clear the callback parameters
>>>>>>> refs/remotes/origin/master
		 */
		if (src_cnt > xor_src_cnt) {
			submit->flags &= ~ASYNC_TX_ACK;
			submit->flags |= ASYNC_TX_FENCE;
<<<<<<< HEAD
			dma_flags = DMA_COMPL_SKIP_DEST_UNMAP;
=======
>>>>>>> refs/remotes/origin/master
			submit->cb_fn = NULL;
			submit->cb_param = NULL;
		} else {
			submit->cb_fn = cb_fn_orig;
			submit->cb_param = cb_param_orig;
		}
		if (submit->cb_fn)
			dma_flags |= DMA_PREP_INTERRUPT;
		if (submit->flags & ASYNC_TX_FENCE)
			dma_flags |= DMA_PREP_FENCE;
<<<<<<< HEAD
		/* Since we have clobbered the src_list we are committed
		 * to doing this asynchronously.  Drivers force forward progress
		 * in case they can not provide a descriptor
		 */
		tx = dma->device_prep_dma_xor(chan, dma_dest, &dma_src[src_off],
					      xor_src_cnt, len, dma_flags);
=======

		/* Drivers force forward progress in case they can not provide a
		 * descriptor
		 */
		tmp = src_list[0];
		if (src_list > unmap->addr)
			src_list[0] = dma_dest;
		tx = dma->device_prep_dma_xor(chan, dma_dest, src_list,
					      xor_src_cnt, unmap->len,
					      dma_flags);
		src_list[0] = tmp;

>>>>>>> refs/remotes/origin/master

		if (unlikely(!tx))
			async_tx_quiesce(&submit->depend_tx);

		/* spin wait for the preceding transactions to complete */
		while (unlikely(!tx)) {
			dma_async_issue_pending(chan);
			tx = dma->device_prep_dma_xor(chan, dma_dest,
<<<<<<< HEAD
						      &dma_src[src_off],
						      xor_src_cnt, len,
						      dma_flags);
		}

=======
						      src_list,
						      xor_src_cnt, unmap->len,
						      dma_flags);
		}

		dma_set_unmap(tx, unmap);
>>>>>>> refs/remotes/origin/master
		async_tx_submit(chan, tx, submit);
		submit->depend_tx = tx;

		if (src_cnt > xor_src_cnt) {
			/* drop completed sources */
			src_cnt -= xor_src_cnt;
<<<<<<< HEAD
			src_off += xor_src_cnt;

			/* use the intermediate result a source */
			dma_src[--src_off] = dma_dest;
			src_cnt++;
=======
			/* use the intermediate result a source */
			src_cnt++;
			src_list += xor_src_cnt - 1;
>>>>>>> refs/remotes/origin/master
		} else
			break;
	}

	return tx;
}

static void
do_sync_xor(struct page *dest, struct page **src_list, unsigned int offset,
	    int src_cnt, size_t len, struct async_submit_ctl *submit)
{
	int i;
	int xor_src_cnt = 0;
	int src_off = 0;
	void *dest_buf;
	void **srcs;

	if (submit->scribble)
		srcs = submit->scribble;
	else
		srcs = (void **) src_list;

	/* convert to buffer pointers */
	for (i = 0; i < src_cnt; i++)
		if (src_list[i])
			srcs[xor_src_cnt++] = page_address(src_list[i]) + offset;
	src_cnt = xor_src_cnt;
	/* set destination address */
	dest_buf = page_address(dest) + offset;

	if (submit->flags & ASYNC_TX_XOR_ZERO_DST)
		memset(dest_buf, 0, len);

	while (src_cnt > 0) {
		/* process up to 'MAX_XOR_BLOCKS' sources */
		xor_src_cnt = min(src_cnt, MAX_XOR_BLOCKS);
		xor_blocks(xor_src_cnt, len, dest_buf, &srcs[src_off]);

		/* drop completed sources */
		src_cnt -= xor_src_cnt;
		src_off += xor_src_cnt;
	}

	async_tx_sync_epilog(submit);
}

/**
 * async_xor - attempt to xor a set of blocks with a dma engine.
 * @dest: destination page
 * @src_list: array of source pages
 * @offset: common src/dst offset to start transaction
 * @src_cnt: number of source pages
 * @len: length in bytes
 * @submit: submission / completion modifiers
 *
 * honored flags: ASYNC_TX_ACK, ASYNC_TX_XOR_ZERO_DST, ASYNC_TX_XOR_DROP_DST
 *
 * xor_blocks always uses the dest as a source so the
 * ASYNC_TX_XOR_ZERO_DST flag must be set to not include dest data in
 * the calculation.  The assumption with dma eninges is that they only
 * use the destination buffer as a source when it is explicity specified
 * in the source list.
 *
 * src_list note: if the dest is also a source it must be at index zero.
 * The contents of this array will be overwritten if a scribble region
 * is not specified.
 */
struct dma_async_tx_descriptor *
async_xor(struct page *dest, struct page **src_list, unsigned int offset,
	  int src_cnt, size_t len, struct async_submit_ctl *submit)
{
	struct dma_chan *chan = async_tx_find_channel(submit, DMA_XOR,
						      &dest, 1, src_list,
						      src_cnt, len);
<<<<<<< HEAD
	dma_addr_t *dma_src = NULL;

	BUG_ON(src_cnt <= 1);

	if (submit->scribble)
		dma_src = submit->scribble;
	else if (sizeof(dma_addr_t) <= sizeof(struct page *))
		dma_src = (dma_addr_t *) src_list;

	if (dma_src && chan && is_dma_xor_aligned(chan->device, offset, 0, len)) {
		/* run the xor asynchronously */
		pr_debug("%s (async): len: %zu\n", __func__, len);

		return do_async_xor(chan, dest, src_list, offset, src_cnt, len,
				    dma_src, submit);
	} else {
=======
	struct dma_device *device = chan ? chan->device : NULL;
	struct dmaengine_unmap_data *unmap = NULL;

	BUG_ON(src_cnt <= 1);

	if (device)
		unmap = dmaengine_get_unmap_data(device->dev, src_cnt+1, GFP_NOIO);

	if (unmap && is_dma_xor_aligned(device, offset, 0, len)) {
		struct dma_async_tx_descriptor *tx;
		int i, j;

		/* run the xor asynchronously */
		pr_debug("%s (async): len: %zu\n", __func__, len);

		unmap->len = len;
		for (i = 0, j = 0; i < src_cnt; i++) {
			if (!src_list[i])
				continue;
			unmap->to_cnt++;
			unmap->addr[j++] = dma_map_page(device->dev, src_list[i],
							offset, len, DMA_TO_DEVICE);
		}

		/* map it bidirectional as it may be re-used as a source */
		unmap->addr[j] = dma_map_page(device->dev, dest, offset, len,
					      DMA_BIDIRECTIONAL);
		unmap->bidi_cnt = 1;

		tx = do_async_xor(chan, unmap, submit);
		dmaengine_unmap_put(unmap);
		return tx;
	} else {
		dmaengine_unmap_put(unmap);
>>>>>>> refs/remotes/origin/master
		/* run the xor synchronously */
		pr_debug("%s (sync): len: %zu\n", __func__, len);
		WARN_ONCE(chan, "%s: no space for dma address conversion\n",
			  __func__);

		/* in the sync case the dest is an implied source
		 * (assumes the dest is the first source)
		 */
		if (submit->flags & ASYNC_TX_XOR_DROP_DST) {
			src_cnt--;
			src_list++;
		}

		/* wait for any prerequisite operations */
		async_tx_quiesce(&submit->depend_tx);

		do_sync_xor(dest, src_list, offset, src_cnt, len, submit);

		return NULL;
	}
}
EXPORT_SYMBOL_GPL(async_xor);

static int page_is_zero(struct page *p, unsigned int offset, size_t len)
{
<<<<<<< HEAD
	char *a = page_address(p) + offset;
	return ((*(u32 *) a) == 0 &&
		memcmp(a, a + 4, len - 4) == 0);
=======
	return !memchr_inv(page_address(p) + offset, 0, len);
>>>>>>> refs/remotes/origin/master
}

static inline struct dma_chan *
xor_val_chan(struct async_submit_ctl *submit, struct page *dest,
		 struct page **src_list, int src_cnt, size_t len)
{
	#ifdef CONFIG_ASYNC_TX_DISABLE_XOR_VAL_DMA
	return NULL;
	#endif
	return async_tx_find_channel(submit, DMA_XOR_VAL, &dest, 1, src_list,
				     src_cnt, len);
}

/**
 * async_xor_val - attempt a xor parity check with a dma engine.
 * @dest: destination page used if the xor is performed synchronously
 * @src_list: array of source pages
 * @offset: offset in pages to start transaction
 * @src_cnt: number of source pages
 * @len: length in bytes
 * @result: 0 if sum == 0 else non-zero
 * @submit: submission / completion modifiers
 *
 * honored flags: ASYNC_TX_ACK
 *
 * src_list note: if the dest is also a source it must be at index zero.
 * The contents of this array will be overwritten if a scribble region
 * is not specified.
 */
struct dma_async_tx_descriptor *
async_xor_val(struct page *dest, struct page **src_list, unsigned int offset,
	      int src_cnt, size_t len, enum sum_check_flags *result,
	      struct async_submit_ctl *submit)
{
	struct dma_chan *chan = xor_val_chan(submit, dest, src_list, src_cnt, len);
	struct dma_device *device = chan ? chan->device : NULL;
	struct dma_async_tx_descriptor *tx = NULL;
<<<<<<< HEAD
	dma_addr_t *dma_src = NULL;

	BUG_ON(src_cnt <= 1);

	if (submit->scribble)
		dma_src = submit->scribble;
	else if (sizeof(dma_addr_t) <= sizeof(struct page *))
		dma_src = (dma_addr_t *) src_list;

	if (dma_src && device && src_cnt <= device->max_xor &&
=======
	struct dmaengine_unmap_data *unmap = NULL;

	BUG_ON(src_cnt <= 1);

	if (device)
		unmap = dmaengine_get_unmap_data(device->dev, src_cnt, GFP_NOIO);

	if (unmap && src_cnt <= device->max_xor &&
>>>>>>> refs/remotes/origin/master
	    is_dma_xor_aligned(device, offset, 0, len)) {
		unsigned long dma_prep_flags = 0;
		int i;

		pr_debug("%s: (async) len: %zu\n", __func__, len);

		if (submit->cb_fn)
			dma_prep_flags |= DMA_PREP_INTERRUPT;
		if (submit->flags & ASYNC_TX_FENCE)
			dma_prep_flags |= DMA_PREP_FENCE;
<<<<<<< HEAD
		for (i = 0; i < src_cnt; i++)
			dma_src[i] = dma_map_page(device->dev, src_list[i],
						  offset, len, DMA_TO_DEVICE);

		tx = device->device_prep_dma_xor_val(chan, dma_src, src_cnt,
=======

		for (i = 0; i < src_cnt; i++) {
			unmap->addr[i] = dma_map_page(device->dev, src_list[i],
						      offset, len, DMA_TO_DEVICE);
			unmap->to_cnt++;
		}
		unmap->len = len;

		tx = device->device_prep_dma_xor_val(chan, unmap->addr, src_cnt,
>>>>>>> refs/remotes/origin/master
						     len, result,
						     dma_prep_flags);
		if (unlikely(!tx)) {
			async_tx_quiesce(&submit->depend_tx);

			while (!tx) {
				dma_async_issue_pending(chan);
				tx = device->device_prep_dma_xor_val(chan,
<<<<<<< HEAD
					dma_src, src_cnt, len, result,
					dma_prep_flags);
			}
		}

=======
					unmap->addr, src_cnt, len, result,
					dma_prep_flags);
			}
		}
		dma_set_unmap(tx, unmap);
>>>>>>> refs/remotes/origin/master
		async_tx_submit(chan, tx, submit);
	} else {
		enum async_tx_flags flags_orig = submit->flags;

		pr_debug("%s: (sync) len: %zu\n", __func__, len);
		WARN_ONCE(device && src_cnt <= device->max_xor,
			  "%s: no space for dma address conversion\n",
			  __func__);

		submit->flags |= ASYNC_TX_XOR_DROP_DST;
		submit->flags &= ~ASYNC_TX_ACK;

		tx = async_xor(dest, src_list, offset, src_cnt, len, submit);

		async_tx_quiesce(&tx);

		*result = !page_is_zero(dest, offset, len) << SUM_CHECK_P;

		async_tx_sync_epilog(submit);
		submit->flags = flags_orig;
	}
<<<<<<< HEAD
=======
	dmaengine_unmap_put(unmap);
>>>>>>> refs/remotes/origin/master

	return tx;
}
EXPORT_SYMBOL_GPL(async_xor_val);

MODULE_AUTHOR("Intel Corporation");
MODULE_DESCRIPTION("asynchronous xor/xor-zero-sum api");
MODULE_LICENSE("GPL");
