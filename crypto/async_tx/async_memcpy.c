/*
 * copy offload engine support
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
#include <linux/highmem.h>
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
#include <linux/async_tx.h>

/**
 * async_memcpy - attempt to copy memory with a dma engine.
 * @dest: destination page
 * @src: src page
 * @dest_offset: offset into 'dest' to start transaction
 * @src_offset: offset into 'src' to start transaction
 * @len: length in bytes
 * @submit: submission / completion modifiers
 *
 * honored flags: ASYNC_TX_ACK
 */
struct dma_async_tx_descriptor *
async_memcpy(struct page *dest, struct page *src, unsigned int dest_offset,
	     unsigned int src_offset, size_t len,
	     struct async_submit_ctl *submit)
{
	struct dma_chan *chan = async_tx_find_channel(submit, DMA_MEMCPY,
						      &dest, 1, &src, 1, len);
	struct dma_device *device = chan ? chan->device : NULL;
	struct dma_async_tx_descriptor *tx = NULL;
<<<<<<< HEAD

	if (device && is_dma_copy_aligned(device, src_offset, dest_offset, len)) {
		dma_addr_t dma_dest, dma_src;
=======
	struct dmaengine_unmap_data *unmap = NULL;

	if (device)
		unmap = dmaengine_get_unmap_data(device->dev, 2, GFP_NOIO);

	if (unmap && is_dma_copy_aligned(device, src_offset, dest_offset, len)) {
>>>>>>> refs/remotes/origin/master
		unsigned long dma_prep_flags = 0;

		if (submit->cb_fn)
			dma_prep_flags |= DMA_PREP_INTERRUPT;
		if (submit->flags & ASYNC_TX_FENCE)
			dma_prep_flags |= DMA_PREP_FENCE;
<<<<<<< HEAD
		dma_dest = dma_map_page(device->dev, dest, dest_offset, len,
					DMA_FROM_DEVICE);

		dma_src = dma_map_page(device->dev, src, src_offset, len,
				       DMA_TO_DEVICE);

		tx = device->device_prep_dma_memcpy(chan, dma_dest, dma_src,
						    len, dma_prep_flags);
=======

		unmap->to_cnt = 1;
		unmap->addr[0] = dma_map_page(device->dev, src, src_offset, len,
					      DMA_TO_DEVICE);
		unmap->from_cnt = 1;
		unmap->addr[1] = dma_map_page(device->dev, dest, dest_offset, len,
					      DMA_FROM_DEVICE);
		unmap->len = len;

		tx = device->device_prep_dma_memcpy(chan, unmap->addr[1],
						    unmap->addr[0], len,
						    dma_prep_flags);
>>>>>>> refs/remotes/origin/master
	}

	if (tx) {
		pr_debug("%s: (async) len: %zu\n", __func__, len);
<<<<<<< HEAD
=======

		dma_set_unmap(tx, unmap);
>>>>>>> refs/remotes/origin/master
		async_tx_submit(chan, tx, submit);
	} else {
		void *dest_buf, *src_buf;
		pr_debug("%s: (sync) len: %zu\n", __func__, len);

		/* wait for any prerequisite operations */
		async_tx_quiesce(&submit->depend_tx);

<<<<<<< HEAD
<<<<<<< HEAD
		dest_buf = kmap_atomic(dest, KM_USER0) + dest_offset;
		src_buf = kmap_atomic(src, KM_USER1) + src_offset;

		memcpy(dest_buf, src_buf, len);

		kunmap_atomic(src_buf, KM_USER1);
		kunmap_atomic(dest_buf, KM_USER0);
=======
=======
>>>>>>> refs/remotes/origin/master
		dest_buf = kmap_atomic(dest) + dest_offset;
		src_buf = kmap_atomic(src) + src_offset;

		memcpy(dest_buf, src_buf, len);

		kunmap_atomic(src_buf);
		kunmap_atomic(dest_buf);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

		async_tx_sync_epilog(submit);
	}

<<<<<<< HEAD
=======
	dmaengine_unmap_put(unmap);

>>>>>>> refs/remotes/origin/master
	return tx;
}
EXPORT_SYMBOL_GPL(async_memcpy);

MODULE_AUTHOR("Intel Corporation");
MODULE_DESCRIPTION("asynchronous memcpy api");
MODULE_LICENSE("GPL");
