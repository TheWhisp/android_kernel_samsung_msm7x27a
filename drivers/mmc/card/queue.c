/*
 *  linux/drivers/mmc/card/queue.c
 *
 *  Copyright (C) 2003 Russell King, All Rights Reserved.
 *  Copyright 2006-2007 Pierre Ossman
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/blkdev.h>
#include <linux/freezer.h>
#include <linux/kthread.h>
#include <linux/scatterlist.h>

#include <linux/mmc/card.h>
#include <linux/mmc/host.h>
#include "queue.h"

#define MMC_QUEUE_BOUNCESZ	65536

#define MMC_QUEUE_SUSPENDED	(1 << 0)

/*
<<<<<<< HEAD
=======
 * Based on benchmark tests the default num of requests to trigger the write
 * packing was determined, to keep the read latency as low as possible and
 * manage to keep the high write throughput.
 */
#define DEFAULT_NUM_REQS_TO_START_PACK 17

/*
>>>>>>> refs/remotes/origin/cm-10.0
 * Prepare a MMC request. This just filters out odd stuff.
 */
static int mmc_prep_request(struct request_queue *q, struct request *req)
{
	struct mmc_queue *mq = q->queuedata;

	/*
	 * We only like normal block requests and discards.
	 */
	if (req->cmd_type != REQ_TYPE_FS && !(req->cmd_flags & REQ_DISCARD)) {
		blk_dump_rq_flags(req, "MMC bad request");
		return BLKPREP_KILL;
	}

	if (mq && mmc_card_removed(mq->card))
		return BLKPREP_KILL;

	req->cmd_flags |= REQ_DONTPREP;

	return BLKPREP_OK;
}

static int mmc_queue_thread(void *d)
{
	struct mmc_queue *mq = d;
	struct request_queue *q = mq->queue;
	struct request *req;

<<<<<<< HEAD
#ifdef CONFIG_MMC_PERF_PROFILING
	ktime_t start, diff;
	struct mmc_host *host = mq->card->host;
	unsigned long bytes_xfer;
#endif


=======
>>>>>>> refs/remotes/origin/cm-10.0
	current->flags |= PF_MEMALLOC;

	down(&mq->thread_sem);
	do {
<<<<<<< HEAD
=======
		struct mmc_queue_req *tmp;
>>>>>>> refs/remotes/origin/cm-10.0
		req = NULL;	/* Must be set to NULL at each iteration */

		spin_lock_irq(q->queue_lock);
		set_current_state(TASK_INTERRUPTIBLE);
		req = blk_fetch_request(q);
<<<<<<< HEAD
		mq->req = req;
		spin_unlock_irq(q->queue_lock);

		if (!req) {
=======
		mq->mqrq_cur->req = req;
		spin_unlock_irq(q->queue_lock);

		if (req || mq->mqrq_prev->req) {
			if (mmc_card_doing_bkops(mq->card))
				mmc_interrupt_bkops(mq->card);

			set_current_state(TASK_RUNNING);
			mq->issue_fn(mq, req);
		} else {
>>>>>>> refs/remotes/origin/cm-10.0
			if (kthread_should_stop()) {
				set_current_state(TASK_RUNNING);
				break;
			}
<<<<<<< HEAD
			up(&mq->thread_sem);
			schedule();
			down(&mq->thread_sem);
			continue;
		}
		set_current_state(TASK_RUNNING);

#ifdef CONFIG_MMC_PERF_PROFILING
		if (host->perf_enable) {
			bytes_xfer = blk_rq_bytes(req);
			if (rq_data_dir(req) == READ) {
				start = ktime_get();
				mq->issue_fn(mq, req);
				diff = ktime_sub(ktime_get(), start);
				host->perf.rbytes_mmcq += bytes_xfer;
				host->perf.rtime_mmcq =
					ktime_add(host->perf.rtime_mmcq, diff);
			} else {
				start = ktime_get();
				mq->issue_fn(mq, req);
				diff = ktime_sub(ktime_get(), start);
				host->perf.wbytes_mmcq += bytes_xfer;
				host->perf.wtime_mmcq =
					ktime_add(host->perf.wtime_mmcq, diff);
			}
		} else {
			mq->issue_fn(mq, req);
		}
#else
			mq->issue_fn(mq, req);
#endif
=======

			mmc_start_bkops(mq->card);
			up(&mq->thread_sem);
			schedule();
			down(&mq->thread_sem);
		}

		/* Current request becomes previous request and vice versa. */
		mq->mqrq_prev->brq.mrq.data = NULL;
		mq->mqrq_prev->req = NULL;
		tmp = mq->mqrq_prev;
		mq->mqrq_prev = mq->mqrq_cur;
		mq->mqrq_cur = tmp;
>>>>>>> refs/remotes/origin/cm-10.0
	} while (1);
	up(&mq->thread_sem);

	return 0;
}

/*
 * Generic MMC request handler.  This is called for any queue on a
 * particular host.  When the host is not busy, we look for a request
 * on any queue on this host, and attempt to issue it.  This may
 * not be the queue we were asked to process.
 */
static void mmc_request(struct request_queue *q)
{
	struct mmc_queue *mq = q->queuedata;
	struct request *req;

	if (!mq) {
		while ((req = blk_fetch_request(q)) != NULL) {
			req->cmd_flags |= REQ_QUIET;
			__blk_end_request_all(req, -EIO);
		}
		return;
	}

<<<<<<< HEAD
	if (!mq->req)
		wake_up_process(mq->thread);
}

=======
	if (!mq->mqrq_cur->req && !mq->mqrq_prev->req)
		wake_up_process(mq->thread);
}

static struct scatterlist *mmc_alloc_sg(int sg_len, int *err)
{
	struct scatterlist *sg;

	sg = kmalloc(sizeof(struct scatterlist)*sg_len, GFP_KERNEL);
	if (!sg)
		*err = -ENOMEM;
	else {
		*err = 0;
		sg_init_table(sg, sg_len);
	}

	return sg;
}

static void mmc_queue_setup_discard(struct request_queue *q,
				    struct mmc_card *card)
{
	unsigned max_discard;

	max_discard = mmc_calc_max_discard(card);
	if (!max_discard)
		return;

	queue_flag_set_unlocked(QUEUE_FLAG_DISCARD, q);
	q->limits.max_discard_sectors = max_discard;
	if (card->erased_byte == 0 && !mmc_can_discard(card))
		q->limits.discard_zeroes_data = 1;
	q->limits.discard_granularity = card->pref_erase << 9;
	/* granularity must not be greater than max. discard */
	if (card->pref_erase > max_discard)
		q->limits.discard_granularity = 0;
	if (mmc_can_secure_erase_trim(card))
		queue_flag_set_unlocked(QUEUE_FLAG_SECDISCARD, q);
}

static void mmc_queue_setup_sanitize(struct request_queue *q)
{
	queue_flag_set_unlocked(QUEUE_FLAG_SANITIZE, q);
}

>>>>>>> refs/remotes/origin/cm-10.0
/**
 * mmc_init_queue - initialise a queue structure.
 * @mq: mmc queue
 * @card: mmc card to attach this queue
 * @lock: queue lock
 * @subname: partition subname
 *
 * Initialise a MMC card request queue.
 */
int mmc_init_queue(struct mmc_queue *mq, struct mmc_card *card,
		   spinlock_t *lock, const char *subname)
{
	struct mmc_host *host = card->host;
	u64 limit = BLK_BOUNCE_HIGH;
	int ret;
<<<<<<< HEAD
=======
	struct mmc_queue_req *mqrq_cur = &mq->mqrq[0];
	struct mmc_queue_req *mqrq_prev = &mq->mqrq[1];
>>>>>>> refs/remotes/origin/cm-10.0

	if (mmc_dev(host)->dma_mask && *mmc_dev(host)->dma_mask)
		limit = *mmc_dev(host)->dma_mask;

	mq->card = card;
	mq->queue = blk_init_queue(mmc_request, lock);
	if (!mq->queue)
		return -ENOMEM;

<<<<<<< HEAD
	mq->queue->queuedata = mq;
	mq->req = NULL;

	blk_queue_prep_rq(mq->queue, mmc_prep_request);
	queue_flag_set_unlocked(QUEUE_FLAG_NONROT, mq->queue);
	if (mmc_can_erase(card)) {
		queue_flag_set_unlocked(QUEUE_FLAG_DISCARD, mq->queue);
		mq->queue->limits.max_discard_sectors = UINT_MAX;
		if (card->erased_byte == 0)
			mq->queue->limits.discard_zeroes_data = 1;
		mq->queue->limits.discard_granularity = card->pref_erase << 9;
		if (mmc_can_secure_erase_trim(card))
			queue_flag_set_unlocked(QUEUE_FLAG_SECDISCARD,
						mq->queue);
	}
=======
	memset(&mq->mqrq_cur, 0, sizeof(mq->mqrq_cur));
	memset(&mq->mqrq_prev, 0, sizeof(mq->mqrq_prev));
	INIT_LIST_HEAD(&mqrq_cur->packed_list);
	INIT_LIST_HEAD(&mqrq_prev->packed_list);
	mq->mqrq_cur = mqrq_cur;
	mq->mqrq_prev = mqrq_prev;
	mq->queue->queuedata = mq;
	mq->num_wr_reqs_to_start_packing = DEFAULT_NUM_REQS_TO_START_PACK;

	blk_queue_prep_rq(mq->queue, mmc_prep_request);
	queue_flag_set_unlocked(QUEUE_FLAG_NONROT, mq->queue);
	if (mmc_can_erase(card))
		mmc_queue_setup_discard(mq->queue, card);

	if ((mmc_can_sanitize(card) && (host->caps2 & MMC_CAP2_SANITIZE)))
		mmc_queue_setup_sanitize(mq->queue);
>>>>>>> refs/remotes/origin/cm-10.0

#ifdef CONFIG_MMC_BLOCK_BOUNCE
	if (host->max_segs == 1) {
		unsigned int bouncesz;

		bouncesz = MMC_QUEUE_BOUNCESZ;

		if (bouncesz > host->max_req_size)
			bouncesz = host->max_req_size;
		if (bouncesz > host->max_seg_size)
			bouncesz = host->max_seg_size;
		if (bouncesz > (host->max_blk_count * 512))
			bouncesz = host->max_blk_count * 512;

		if (bouncesz > 512) {
<<<<<<< HEAD
			mq->bounce_buf = kmalloc(bouncesz, GFP_KERNEL);
			if (!mq->bounce_buf) {
				printk(KERN_WARNING "%s: unable to "
					"allocate bounce buffer\n",
					mmc_card_name(card));
			}
		}

		if (mq->bounce_buf) {
=======
			mqrq_cur->bounce_buf = kmalloc(bouncesz, GFP_KERNEL);
			if (!mqrq_cur->bounce_buf) {
				pr_warning("%s: unable to "
					"allocate bounce cur buffer\n",
					mmc_card_name(card));
			}
			mqrq_prev->bounce_buf = kmalloc(bouncesz, GFP_KERNEL);
			if (!mqrq_prev->bounce_buf) {
				pr_warning("%s: unable to "
					"allocate bounce prev buffer\n",
					mmc_card_name(card));
				kfree(mqrq_cur->bounce_buf);
				mqrq_cur->bounce_buf = NULL;
			}
		}

		if (mqrq_cur->bounce_buf && mqrq_prev->bounce_buf) {
>>>>>>> refs/remotes/origin/cm-10.0
			blk_queue_bounce_limit(mq->queue, BLK_BOUNCE_ANY);
			blk_queue_max_hw_sectors(mq->queue, bouncesz / 512);
			blk_queue_max_segments(mq->queue, bouncesz / 512);
			blk_queue_max_segment_size(mq->queue, bouncesz);

<<<<<<< HEAD
			mq->sg = kmalloc(sizeof(struct scatterlist),
				GFP_KERNEL);
			if (!mq->sg) {
				ret = -ENOMEM;
				goto cleanup_queue;
			}
			sg_init_table(mq->sg, 1);

			mq->bounce_sg = kmalloc(sizeof(struct scatterlist) *
				bouncesz / 512, GFP_KERNEL);
			if (!mq->bounce_sg) {
				ret = -ENOMEM;
				goto cleanup_queue;
			}
			sg_init_table(mq->bounce_sg, bouncesz / 512);
=======
			mqrq_cur->sg = mmc_alloc_sg(1, &ret);
			if (ret)
				goto cleanup_queue;

			mqrq_cur->bounce_sg =
				mmc_alloc_sg(bouncesz / 512, &ret);
			if (ret)
				goto cleanup_queue;

			mqrq_prev->sg = mmc_alloc_sg(1, &ret);
			if (ret)
				goto cleanup_queue;

			mqrq_prev->bounce_sg =
				mmc_alloc_sg(bouncesz / 512, &ret);
			if (ret)
				goto cleanup_queue;
>>>>>>> refs/remotes/origin/cm-10.0
		}
	}
#endif

<<<<<<< HEAD
	if (!mq->bounce_buf) {
=======
	if (!mqrq_cur->bounce_buf && !mqrq_prev->bounce_buf) {
>>>>>>> refs/remotes/origin/cm-10.0
		blk_queue_bounce_limit(mq->queue, limit);
		blk_queue_max_hw_sectors(mq->queue,
			min(host->max_blk_count, host->max_req_size / 512));
		blk_queue_max_segments(mq->queue, host->max_segs);
		blk_queue_max_segment_size(mq->queue, host->max_seg_size);

<<<<<<< HEAD
		mq->sg = kmalloc(sizeof(struct scatterlist) *
			host->max_segs, GFP_KERNEL);
		if (!mq->sg) {
			ret = -ENOMEM;
			goto cleanup_queue;
		}
		sg_init_table(mq->sg, host->max_segs);
=======
		mqrq_cur->sg = mmc_alloc_sg(host->max_segs, &ret);
		if (ret)
			goto cleanup_queue;


		mqrq_prev->sg = mmc_alloc_sg(host->max_segs, &ret);
		if (ret)
			goto cleanup_queue;
>>>>>>> refs/remotes/origin/cm-10.0
	}

	sema_init(&mq->thread_sem, 1);

	mq->thread = kthread_run(mmc_queue_thread, mq, "mmcqd/%d%s",
		host->index, subname ? subname : "");

	if (IS_ERR(mq->thread)) {
		ret = PTR_ERR(mq->thread);
		goto free_bounce_sg;
	}

	return 0;
 free_bounce_sg:
<<<<<<< HEAD
 	if (mq->bounce_sg)
 		kfree(mq->bounce_sg);
 	mq->bounce_sg = NULL;
 cleanup_queue:
 	if (mq->sg)
		kfree(mq->sg);
	mq->sg = NULL;
	if (mq->bounce_buf)
		kfree(mq->bounce_buf);
	mq->bounce_buf = NULL;
=======
	kfree(mqrq_cur->bounce_sg);
	mqrq_cur->bounce_sg = NULL;
	kfree(mqrq_prev->bounce_sg);
	mqrq_prev->bounce_sg = NULL;

 cleanup_queue:
	kfree(mqrq_cur->sg);
	mqrq_cur->sg = NULL;
	kfree(mqrq_cur->bounce_buf);
	mqrq_cur->bounce_buf = NULL;

	kfree(mqrq_prev->sg);
	mqrq_prev->sg = NULL;
	kfree(mqrq_prev->bounce_buf);
	mqrq_prev->bounce_buf = NULL;

>>>>>>> refs/remotes/origin/cm-10.0
	blk_cleanup_queue(mq->queue);
	return ret;
}

void mmc_cleanup_queue(struct mmc_queue *mq)
{
	struct request_queue *q = mq->queue;
	unsigned long flags;
<<<<<<< HEAD
=======
	struct mmc_queue_req *mqrq_cur = mq->mqrq_cur;
	struct mmc_queue_req *mqrq_prev = mq->mqrq_prev;
>>>>>>> refs/remotes/origin/cm-10.0

	/* Make sure the queue isn't suspended, as that will deadlock */
	mmc_queue_resume(mq);

	/* Then terminate our worker thread */
	kthread_stop(mq->thread);

	/* Empty the queue */
	spin_lock_irqsave(q->queue_lock, flags);
	q->queuedata = NULL;
	blk_start_queue(q);
	spin_unlock_irqrestore(q->queue_lock, flags);

<<<<<<< HEAD
 	if (mq->bounce_sg)
 		kfree(mq->bounce_sg);
 	mq->bounce_sg = NULL;

	kfree(mq->sg);
	mq->sg = NULL;

	if (mq->bounce_buf)
		kfree(mq->bounce_buf);
	mq->bounce_buf = NULL;
=======
	kfree(mqrq_cur->bounce_sg);
	mqrq_cur->bounce_sg = NULL;

	kfree(mqrq_cur->sg);
	mqrq_cur->sg = NULL;

	kfree(mqrq_cur->bounce_buf);
	mqrq_cur->bounce_buf = NULL;

	kfree(mqrq_prev->bounce_sg);
	mqrq_prev->bounce_sg = NULL;

	kfree(mqrq_prev->sg);
	mqrq_prev->sg = NULL;

	kfree(mqrq_prev->bounce_buf);
	mqrq_prev->bounce_buf = NULL;
>>>>>>> refs/remotes/origin/cm-10.0

	mq->card = NULL;
}
EXPORT_SYMBOL(mmc_cleanup_queue);

/**
 * mmc_queue_suspend - suspend a MMC request queue
 * @mq: MMC queue to suspend
 *
 * Stop the block request queue, and wait for our thread to
 * complete any outstanding requests.  This ensures that we
 * won't suspend while a request is being processed.
 */
<<<<<<< HEAD
void mmc_queue_suspend(struct mmc_queue *mq)
{
	struct request_queue *q = mq->queue;
	unsigned long flags;
=======
int mmc_queue_suspend(struct mmc_queue *mq)
{
	struct request_queue *q = mq->queue;
	unsigned long flags;
	int rc = 0;
>>>>>>> refs/remotes/origin/cm-10.0

	if (!(mq->flags & MMC_QUEUE_SUSPENDED)) {
		mq->flags |= MMC_QUEUE_SUSPENDED;

		spin_lock_irqsave(q->queue_lock, flags);
		blk_stop_queue(q);
		spin_unlock_irqrestore(q->queue_lock, flags);

<<<<<<< HEAD
		down(&mq->thread_sem);
	}
=======
		rc = down_trylock(&mq->thread_sem);
		if (rc) {
			/*
			 * Failed to take the lock so better to abort the
			 * suspend because mmcqd thread is processing requests.
			 */
			mq->flags &= ~MMC_QUEUE_SUSPENDED;
			spin_lock_irqsave(q->queue_lock, flags);
			blk_start_queue(q);
			spin_unlock_irqrestore(q->queue_lock, flags);
			rc = -EBUSY;
		}
	}
	return rc;
>>>>>>> refs/remotes/origin/cm-10.0
}

/**
 * mmc_queue_resume - resume a previously suspended MMC request queue
 * @mq: MMC queue to resume
 */
void mmc_queue_resume(struct mmc_queue *mq)
{
	struct request_queue *q = mq->queue;
	unsigned long flags;

	if (mq->flags & MMC_QUEUE_SUSPENDED) {
		mq->flags &= ~MMC_QUEUE_SUSPENDED;

		up(&mq->thread_sem);

		spin_lock_irqsave(q->queue_lock, flags);
		blk_start_queue(q);
		spin_unlock_irqrestore(q->queue_lock, flags);
	}
}

<<<<<<< HEAD
/*
 * Prepare the sg list(s) to be handed of to the host driver
 */
unsigned int mmc_queue_map_sg(struct mmc_queue *mq)
=======
static unsigned int mmc_queue_packed_map_sg(struct mmc_queue *mq,
					    struct mmc_queue_req *mqrq,
					    struct scatterlist *sg)
{
	struct scatterlist *__sg;
	unsigned int sg_len = 0;
	struct request *req;
	enum mmc_packed_cmd cmd;

	cmd = mqrq->packed_cmd;

	if (cmd == MMC_PACKED_WRITE) {
		__sg = sg;
		sg_set_buf(__sg, mqrq->packed_cmd_hdr,
				sizeof(mqrq->packed_cmd_hdr));
		sg_len++;
		__sg->page_link &= ~0x02;
	}

	__sg = sg + sg_len;
	list_for_each_entry(req, &mqrq->packed_list, queuelist) {
		sg_len += blk_rq_map_sg(mq->queue, req, __sg);
		__sg = sg + (sg_len - 1);
		(__sg++)->page_link &= ~0x02;
	}
	sg_mark_end(sg + (sg_len - 1));
	return sg_len;
}

/*
 * Prepare the sg list(s) to be handed of to the host driver
 */
unsigned int mmc_queue_map_sg(struct mmc_queue *mq, struct mmc_queue_req *mqrq)
>>>>>>> refs/remotes/origin/cm-10.0
{
	unsigned int sg_len;
	size_t buflen;
	struct scatterlist *sg;
	int i;

<<<<<<< HEAD
	if (!mq->bounce_buf)
		return blk_rq_map_sg(mq->queue, mq->req, mq->sg);

	BUG_ON(!mq->bounce_sg);

	sg_len = blk_rq_map_sg(mq->queue, mq->req, mq->bounce_sg);

	mq->bounce_sg_len = sg_len;

	buflen = 0;
	for_each_sg(mq->bounce_sg, sg, sg_len, i)
		buflen += sg->length;

	sg_init_one(mq->sg, mq->bounce_buf, buflen);
=======
	if (!mqrq->bounce_buf) {
		if (!list_empty(&mqrq->packed_list))
			return mmc_queue_packed_map_sg(mq, mqrq, mqrq->sg);
		else
			return blk_rq_map_sg(mq->queue, mqrq->req, mqrq->sg);
	}

	BUG_ON(!mqrq->bounce_sg);

	if (!list_empty(&mqrq->packed_list))
		sg_len = mmc_queue_packed_map_sg(mq, mqrq, mqrq->bounce_sg);
	else
		sg_len = blk_rq_map_sg(mq->queue, mqrq->req, mqrq->bounce_sg);

	mqrq->bounce_sg_len = sg_len;

	buflen = 0;
	for_each_sg(mqrq->bounce_sg, sg, sg_len, i)
		buflen += sg->length;

	sg_init_one(mqrq->sg, mqrq->bounce_buf, buflen);
>>>>>>> refs/remotes/origin/cm-10.0

	return 1;
}

/*
 * If writing, bounce the data to the buffer before the request
 * is sent to the host driver
 */
<<<<<<< HEAD
void mmc_queue_bounce_pre(struct mmc_queue *mq)
{
	if (!mq->bounce_buf)
		return;

	if (rq_data_dir(mq->req) != WRITE)
		return;

	sg_copy_to_buffer(mq->bounce_sg, mq->bounce_sg_len,
		mq->bounce_buf, mq->sg[0].length);
=======
void mmc_queue_bounce_pre(struct mmc_queue_req *mqrq)
{
	if (!mqrq->bounce_buf)
		return;

	if (rq_data_dir(mqrq->req) != WRITE)
		return;

	sg_copy_to_buffer(mqrq->bounce_sg, mqrq->bounce_sg_len,
		mqrq->bounce_buf, mqrq->sg[0].length);
>>>>>>> refs/remotes/origin/cm-10.0
}

/*
 * If reading, bounce the data from the buffer after the request
 * has been handled by the host driver
 */
<<<<<<< HEAD
void mmc_queue_bounce_post(struct mmc_queue *mq)
{
	if (!mq->bounce_buf)
		return;

	if (rq_data_dir(mq->req) != READ)
		return;

	sg_copy_from_buffer(mq->bounce_sg, mq->bounce_sg_len,
		mq->bounce_buf, mq->sg[0].length);
}

=======
void mmc_queue_bounce_post(struct mmc_queue_req *mqrq)
{
	if (!mqrq->bounce_buf)
		return;

	if (rq_data_dir(mqrq->req) != READ)
		return;

	sg_copy_from_buffer(mqrq->bounce_sg, mqrq->bounce_sg_len,
		mqrq->bounce_buf, mqrq->sg[0].length);
}
>>>>>>> refs/remotes/origin/cm-10.0
