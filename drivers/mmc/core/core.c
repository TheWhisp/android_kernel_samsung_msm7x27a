/*
 *  linux/drivers/mmc/core/core.c
 *
 *  Copyright (C) 2003-2004 Russell King, All Rights Reserved.
 *  SD support Copyright (C) 2004 Ian Molton, All Rights Reserved.
 *  Copyright (C) 2005-2008 Pierre Ossman, All Rights Reserved.
 *  MMCv4 support Copyright (C) 2006 Philip Langdale, All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/completion.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/pagemap.h>
#include <linux/err.h>
#include <linux/leds.h>
#include <linux/scatterlist.h>
#include <linux/log2.h>
#include <linux/regulator/consumer.h>
#include <linux/pm_runtime.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/suspend.h>
#include <linux/fault-inject.h>
#include <linux/random.h>
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/wakelock.h>
#include <linux/pm.h>
=======
#include <linux/pm_wakeup.h>
#include <linux/suspend.h>
#include <linux/fault-inject.h>
#include <linux/random.h>
#include <linux/slab.h>
#include <linux/of.h>
>>>>>>> refs/remotes/origin/master
=======
#include <linux/wakelock.h>
#include <linux/pm.h>
>>>>>>> refs/remotes/origin/cm-11.0

#include <linux/mmc/card.h>
#include <linux/mmc/host.h>
#include <linux/mmc/mmc.h>
#include <linux/mmc/sd.h>

#include "core.h"
#include "bus.h"
#include "host.h"
#include "sdio_bus.h"

#include "mmc_ops.h"
#include "sd_ops.h"
#include "sdio_ops.h"

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
/*
 * The Background operations can take a long time, depends on the house keeping
 * operations the card has to perform
 */
#define MMC_BKOPS_MAX_TIMEOUT    (4 * 60 * 1000) /* max time to wait in ms */

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
static struct workqueue_struct *workqueue;
=======
/* If the device is not responding */
#define MMC_CORE_TIMEOUT_MS	(10 * 60 * 1000) /* 10 minute timeout */

/*
 * Background operations can take a long time, depending on the housekeeping
 * operations the card has to perform.
 */
#define MMC_BKOPS_MAX_TIMEOUT	(4 * 60 * 1000) /* max time to wait in ms */

static struct workqueue_struct *workqueue;
static const unsigned freqs[] = { 400000, 300000, 200000, 100000 };
>>>>>>> refs/remotes/origin/master

/*
 * Enabling software CRCs on the data blocks can be a significant (30%)
 * performance cost, and for other reasons may not always be desired.
 * So we allow it it to be disabled.
 */
<<<<<<< HEAD
<<<<<<< HEAD
int use_spi_crc = 1;
=======
bool use_spi_crc = 1;
>>>>>>> refs/remotes/origin/cm-10.0
=======
bool use_spi_crc = 1;
>>>>>>> refs/remotes/origin/master
module_param(use_spi_crc, bool, 0);

/*
 * We normally treat cards as removed during suspend if they are not
 * known to be on a non-removable bus, to avoid the risk of writing
 * back data to a different card after resume.  Allow this to be
 * overridden if necessary.
 */
#ifdef CONFIG_MMC_UNSAFE_RESUME
<<<<<<< HEAD
<<<<<<< HEAD
int mmc_assume_removable;
#else
int mmc_assume_removable = 1;
=======
bool mmc_assume_removable;
#else
bool mmc_assume_removable = 1;
>>>>>>> refs/remotes/origin/cm-10.0
=======
bool mmc_assume_removable;
#else
bool mmc_assume_removable = 1;
>>>>>>> refs/remotes/origin/master
#endif
EXPORT_SYMBOL(mmc_assume_removable);
module_param_named(removable, mmc_assume_removable, bool, 0644);
MODULE_PARM_DESC(
	removable,
	"MMC/SD cards are removable and may be removed during suspend");

/*
 * Internal function. Schedule delayed work in the MMC work queue.
 */
static int mmc_schedule_delayed_work(struct delayed_work *work,
				     unsigned long delay)
{
	return queue_delayed_work(workqueue, work, delay);
}

/*
 * Internal function. Flush all scheduled work from the MMC work queue.
 */
static void mmc_flush_scheduled_work(void)
{
	flush_workqueue(workqueue);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_FAIL_MMC_REQUEST

/*
 * Internal function. Inject random data errors.
 * If mmc_data is NULL no errors are injected.
 */
static void mmc_should_fail_request(struct mmc_host *host,
				    struct mmc_request *mrq)
{
	struct mmc_command *cmd = mrq->cmd;
	struct mmc_data *data = mrq->data;
	static const int data_errors[] = {
		-ETIMEDOUT,
		-EILSEQ,
		-EIO,
	};

	if (!data)
		return;

	if (cmd->error || data->error ||
	    !should_fail(&host->fail_mmc_request, data->blksz * data->blocks))
		return;

<<<<<<< HEAD
	data->error = data_errors[random32() % ARRAY_SIZE(data_errors)];
	data->bytes_xfered = (random32() % (data->bytes_xfered >> 9)) << 9;
=======
	data->error = data_errors[prandom_u32() % ARRAY_SIZE(data_errors)];
	data->bytes_xfered = (prandom_u32() % (data->bytes_xfered >> 9)) << 9;
>>>>>>> refs/remotes/origin/master
}

#else /* CONFIG_FAIL_MMC_REQUEST */

static inline void mmc_should_fail_request(struct mmc_host *host,
					   struct mmc_request *mrq)
{
}

#endif /* CONFIG_FAIL_MMC_REQUEST */

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/**
 *	mmc_request_done - finish processing an MMC request
 *	@host: MMC host which completed request
 *	@mrq: MMC request which request
 *
 *	MMC drivers should call this function when they have completed
 *	their processing of a request.
 */
void mmc_request_done(struct mmc_host *host, struct mmc_request *mrq)
{
	struct mmc_command *cmd = mrq->cmd;
	int err = cmd->error;
<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_MMC_PERF_PROFILING
	ktime_t diff;
#endif
=======
>>>>>>> refs/remotes/origin/master
=======
#ifdef CONFIG_MMC_PERF_PROFILING
	ktime_t diff;
#endif
>>>>>>> refs/remotes/origin/cm-11.0

	if (err && cmd->retries && mmc_host_is_spi(host)) {
		if (cmd->resp[0] & R1_SPI_ILLEGAL_COMMAND)
			cmd->retries = 0;
	}

	if (err && cmd->retries && !mmc_card_removed(host->card)) {
<<<<<<< HEAD
<<<<<<< HEAD
		pr_debug("%s: req failed (CMD%u): %d, retrying...\n",
			mmc_hostname(host), cmd->opcode, err);

		cmd->retries--;
		cmd->error = 0;
		host->ops->request(host, mrq);
	} else {
=======
=======
>>>>>>> refs/remotes/origin/master
		/*
		 * Request starter must handle retries - see
		 * mmc_wait_for_req_done().
		 */
		if (mrq->done)
			mrq->done(mrq);
	} else {
		mmc_should_fail_request(host, mrq);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		led_trigger_event(host->led, LED_OFF);

		pr_debug("%s: req done (CMD%u): %d: %08x %08x %08x %08x\n",
			mmc_hostname(host), cmd->opcode, err,
			cmd->resp[0], cmd->resp[1],
			cmd->resp[2], cmd->resp[3]);

		if (mrq->data) {
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
#ifdef CONFIG_MMC_PERF_PROFILING
			if (host->perf_enable) {
				diff = ktime_sub(ktime_get(), host->perf.start);
				if (mrq->data->flags == MMC_DATA_READ) {
					host->perf.rbytes_drv +=
							mrq->data->bytes_xfered;
					host->perf.rtime_drv =
						ktime_add(host->perf.rtime_drv,
							diff);
				} else {
					host->perf.wbytes_drv +=
						mrq->data->bytes_xfered;
					host->perf.wtime_drv =
						ktime_add(host->perf.wtime_drv,
							diff);
				}
			}
#endif
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
			pr_debug("%s:     %d bytes transferred: %d\n",
				mmc_hostname(host),
				mrq->data->bytes_xfered, mrq->data->error);
		}

		if (mrq->stop) {
			pr_debug("%s:     (CMD%u): %d: %08x %08x %08x %08x\n",
				mmc_hostname(host), mrq->stop->opcode,
				mrq->stop->error,
				mrq->stop->resp[0], mrq->stop->resp[1],
				mrq->stop->resp[2], mrq->stop->resp[3]);
		}

		if (mrq->done)
			mrq->done(mrq);

		mmc_host_clk_release(host);
	}
}

EXPORT_SYMBOL(mmc_request_done);

static void
mmc_start_request(struct mmc_host *host, struct mmc_request *mrq)
{
#ifdef CONFIG_MMC_DEBUG
	unsigned int i, sz;
	struct scatterlist *sg;
#endif

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	if (mrq->sbc) {
		pr_debug("<%s: starting CMD%u arg %08x flags %08x>\n",
			 mmc_hostname(host), mrq->sbc->opcode,
			 mrq->sbc->arg, mrq->sbc->flags);
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	pr_debug("%s: starting CMD%u arg %08x flags %08x\n",
		 mmc_hostname(host), mrq->cmd->opcode,
		 mrq->cmd->arg, mrq->cmd->flags);

	if (mrq->data) {
		pr_debug("%s:     blksz %d blocks %d flags %08x "
			"tsac %d ms nsac %d\n",
			mmc_hostname(host), mrq->data->blksz,
			mrq->data->blocks, mrq->data->flags,
			mrq->data->timeout_ns / 1000000,
			mrq->data->timeout_clks);
	}

	if (mrq->stop) {
		pr_debug("%s:     CMD%u arg %08x flags %08x\n",
			 mmc_hostname(host), mrq->stop->opcode,
			 mrq->stop->arg, mrq->stop->flags);
	}

	WARN_ON(!host->claimed);

	mrq->cmd->error = 0;
	mrq->cmd->mrq = mrq;
	if (mrq->data) {
		BUG_ON(mrq->data->blksz > host->max_blk_size);
		BUG_ON(mrq->data->blocks > host->max_blk_count);
		BUG_ON(mrq->data->blocks * mrq->data->blksz >
			host->max_req_size);

#ifdef CONFIG_MMC_DEBUG
		sz = 0;
		for_each_sg(mrq->data->sg, sg, mrq->data->sg_len, i)
			sz += sg->length;
		BUG_ON(sz != mrq->data->blocks * mrq->data->blksz);
#endif

		mrq->cmd->data = mrq->data;
		mrq->data->error = 0;
		mrq->data->mrq = mrq;
		if (mrq->stop) {
			mrq->data->stop = mrq->stop;
			mrq->stop->error = 0;
			mrq->stop->mrq = mrq;
		}
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
#ifdef CONFIG_MMC_PERF_PROFILING
		if (host->perf_enable)
			host->perf.start = ktime_get();
#endif
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	}
	mmc_host_clk_hold(host);
	led_trigger_event(host->led, LED_FULL);
	host->ops->request(host, mrq);
}

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
static void mmc_wait_done(struct mmc_request *mrq)
{
	complete(mrq->done_data);
}

/**
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
/**
 *	mmc_start_bkops - start BKOPS for supported cards
 *	@card: MMC card to start BKOPS
 *
 *	Start background operations whenever requested.
 *	when the urgent BKOPS bit is set in a R1 command response
 *	then background operations should be started immediately.
*/
void mmc_start_bkops(struct mmc_card *card)
{
	int err;
	unsigned long flags;
	int timeout;

	BUG_ON(!card);
	if (!card->ext_csd.bkops_en || !(card->host->caps2 & MMC_CAP2_BKOPS))
		return;

	if (mmc_card_check_bkops(card)) {
		spin_lock_irqsave(&card->host->lock, flags);
		mmc_card_clr_check_bkops(card);
		spin_unlock_irqrestore(&card->host->lock, flags);
		if (mmc_is_exception_event(card, EXT_CSD_URGENT_BKOPS))
			if (card->ext_csd.raw_bkops_status)
				mmc_card_set_need_bkops(card);
	}

	/*
	 * If card is already doing bkops or need for
	 * bkops flag is not set, then do nothing just
	 * return
	 */
	if (mmc_card_doing_bkops(card) || !mmc_card_need_bkops(card))
		return;

	mmc_claim_host(card->host);

	timeout = (card->ext_csd.raw_bkops_status >= EXT_CSD_BKOPS_LEVEL_2) ?
		MMC_BKOPS_MAX_TIMEOUT : 0;

	err = mmc_switch(card, EXT_CSD_CMD_SET_NORMAL,
			EXT_CSD_BKOPS_START, 1, timeout);
	if (err) {
		pr_warning("%s: error %d starting bkops\n",
			   mmc_hostname(card->host), err);
		mmc_card_clr_need_bkops(card);
		goto out;
	}

	spin_lock_irqsave(&card->host->lock, flags);
	mmc_card_clr_need_bkops(card);

<<<<<<< HEAD
=======
/**
 *	mmc_start_bkops - start BKOPS for supported cards
 *	@card: MMC card to start BKOPS
 *	@form_exception: A flag to indicate if this function was
 *			 called due to an exception raised by the card
 *
 *	Start background operations whenever requested.
 *	When the urgent BKOPS bit is set in a R1 command response
 *	then background operations should be started immediately.
*/
void mmc_start_bkops(struct mmc_card *card, bool from_exception)
{
	int err;
	int timeout;
	bool use_busy_signal;

	BUG_ON(!card);

	if (!card->ext_csd.bkops_en || mmc_card_doing_bkops(card))
		return;

	err = mmc_read_bkops_status(card);
	if (err) {
		pr_err("%s: Failed to read bkops status: %d\n",
		       mmc_hostname(card->host), err);
		return;
	}

	if (!card->ext_csd.raw_bkops_status)
		return;

	if (card->ext_csd.raw_bkops_status < EXT_CSD_BKOPS_LEVEL_2 &&
	    from_exception)
		return;

	mmc_claim_host(card->host);
	if (card->ext_csd.raw_bkops_status >= EXT_CSD_BKOPS_LEVEL_2) {
		timeout = MMC_BKOPS_MAX_TIMEOUT;
		use_busy_signal = true;
	} else {
		timeout = 0;
		use_busy_signal = false;
	}

	err = __mmc_switch(card, EXT_CSD_CMD_SET_NORMAL,
			EXT_CSD_BKOPS_START, 1, timeout, use_busy_signal, true);
	if (err) {
		pr_warn("%s: Error %d starting bkops\n",
			mmc_hostname(card->host), err);
		goto out;
	}

>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	/*
	 * For urgent bkops status (LEVEL_2 and more)
	 * bkops executed synchronously, otherwise
	 * the operation is in progress
	 */
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (card->ext_csd.raw_bkops_status >= EXT_CSD_BKOPS_LEVEL_2)
		mmc_card_set_check_bkops(card);
	else
		mmc_card_set_doing_bkops(card);

	spin_unlock_irqrestore(&card->host->lock, flags);
<<<<<<< HEAD
=======
	if (!use_busy_signal)
		mmc_card_set_doing_bkops(card);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
out:
	mmc_release_host(card->host);
}
EXPORT_SYMBOL(mmc_start_bkops);

<<<<<<< HEAD
<<<<<<< HEAD
=======
/*
 * mmc_wait_data_done() - done callback for data request
 * @mrq: done data request
 *
 * Wakes up mmc context, passed as a callback to host controller driver
 */
static void mmc_wait_data_done(struct mmc_request *mrq)
{
	mrq->host->context_info.is_done_rcv = true;
	wake_up_interruptible(&mrq->host->context_info.wait);
}

>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
static void mmc_wait_done(struct mmc_request *mrq)
{
	complete(&mrq->completion);
}

<<<<<<< HEAD
=======
/*
 *__mmc_start_data_req() - starts data request
 * @host: MMC host to start the request
 * @mrq: data request to start
 *
 * Sets the done callback to be called when request is completed by the card.
 * Starts data mmc request execution
 */
static int __mmc_start_data_req(struct mmc_host *host, struct mmc_request *mrq)
{
	mrq->done = mmc_wait_data_done;
	mrq->host = host;
	if (mmc_card_removed(host->card)) {
		mrq->cmd->error = -ENOMEDIUM;
		mmc_wait_data_done(mrq);
		return -ENOMEDIUM;
	}
	mmc_start_request(host, mrq);

	return 0;
}

>>>>>>> refs/remotes/origin/master
static int __mmc_start_req(struct mmc_host *host, struct mmc_request *mrq)
{
	init_completion(&mrq->completion);
	mrq->done = mmc_wait_done;
	if (mmc_card_removed(host->card)) {
		mrq->cmd->error = -ENOMEDIUM;
		complete(&mrq->completion);
		return -ENOMEDIUM;
	}
	mmc_start_request(host, mrq);
	return 0;
}

<<<<<<< HEAD
=======
/*
 * mmc_wait_for_data_req_done() - wait for request completed
 * @host: MMC host to prepare the command.
 * @mrq: MMC request to wait for
 *
 * Blocks MMC context till host controller will ack end of data request
 * execution or new request notification arrives from the block layer.
 * Handles command retries.
 *
 * Returns enum mmc_blk_status after checking errors.
 */
static int mmc_wait_for_data_req_done(struct mmc_host *host,
				      struct mmc_request *mrq,
				      struct mmc_async_req *next_req)
{
	struct mmc_command *cmd;
	struct mmc_context_info *context_info = &host->context_info;
	int err;
	unsigned long flags;

	while (1) {
		wait_event_interruptible(context_info->wait,
				(context_info->is_done_rcv ||
				 context_info->is_new_req));
		spin_lock_irqsave(&context_info->lock, flags);
		context_info->is_waiting_last_req = false;
		spin_unlock_irqrestore(&context_info->lock, flags);
		if (context_info->is_done_rcv) {
			context_info->is_done_rcv = false;
			context_info->is_new_req = false;
			cmd = mrq->cmd;

			if (!cmd->error || !cmd->retries ||
			    mmc_card_removed(host->card)) {
				err = host->areq->err_check(host->card,
							    host->areq);
				break; /* return err */
			} else {
				pr_info("%s: req failed (CMD%u): %d, retrying...\n",
					mmc_hostname(host),
					cmd->opcode, cmd->error);
				cmd->retries--;
				cmd->error = 0;
				host->ops->request(host, mrq);
				continue; /* wait for done/new event again */
			}
		} else if (context_info->is_new_req) {
			context_info->is_new_req = false;
			if (!next_req) {
				err = MMC_BLK_NEW_REQUEST;
				break; /* return err */
			}
		}
	}
	return err;
}

>>>>>>> refs/remotes/origin/master
static void mmc_wait_for_req_done(struct mmc_host *host,
				  struct mmc_request *mrq)
{
	struct mmc_command *cmd;

	while (1) {
<<<<<<< HEAD
<<<<<<< HEAD
		wait_for_completion_io(&mrq->completion);

		cmd = mrq->cmd;
=======
		wait_for_completion(&mrq->completion);
=======
		wait_for_completion_io(&mrq->completion);
>>>>>>> refs/remotes/origin/cm-11.0

		cmd = mrq->cmd;

		/*
		 * If host has timed out waiting for the sanitize
		 * to complete, card might be still in programming state
		 * so let's try to bring the card out of programming
		 * state.
		 */
		if (cmd->sanitize_busy && cmd->error == -ETIMEDOUT) {
			if (!mmc_interrupt_hpi(host->card)) {
				pr_warning("%s: %s: Interrupted sanitize\n",
					   mmc_hostname(host), __func__);
				cmd->error = 0;
				break;
			} else {
				pr_err("%s: %s: Failed to interrupt sanitize\n",
				       mmc_hostname(host), __func__);
			}
		}
>>>>>>> refs/remotes/origin/master
		if (!cmd->error || !cmd->retries ||
		    mmc_card_removed(host->card))
			break;

		pr_debug("%s: req failed (CMD%u): %d, retrying...\n",
			 mmc_hostname(host), cmd->opcode, cmd->error);
		cmd->retries--;
		cmd->error = 0;
		host->ops->request(host, mrq);
	}
}

/**
 *	mmc_pre_req - Prepare for a new request
 *	@host: MMC host to prepare command
 *	@mrq: MMC request to prepare for
 *	@is_first_req: true if there is no previous started request
 *                     that may run in parellel to this call, otherwise false
 *
 *	mmc_pre_req() is called in prior to mmc_start_req() to let
 *	host prepare for the new request. Preparation of a request may be
 *	performed while another request is running on the host.
 */
static void mmc_pre_req(struct mmc_host *host, struct mmc_request *mrq,
		 bool is_first_req)
{
	if (host->ops->pre_req) {
		mmc_host_clk_hold(host);
		host->ops->pre_req(host, mrq, is_first_req);
		mmc_host_clk_release(host);
	}
}

/**
 *	mmc_post_req - Post process a completed request
 *	@host: MMC host to post process command
 *	@mrq: MMC request to post process for
 *	@err: Error, if non zero, clean up any resources made in pre_req
 *
 *	Let the host post process a completed request. Post processing of
 *	a request may be performed while another reuqest is running.
 */
static void mmc_post_req(struct mmc_host *host, struct mmc_request *mrq,
			 int err)
{
	if (host->ops->post_req) {
		mmc_host_clk_hold(host);
		host->ops->post_req(host, mrq, err);
		mmc_host_clk_release(host);
	}
}

/**
 *	mmc_start_req - start a non-blocking request
 *	@host: MMC host to start command
 *	@areq: async request to start
 *	@error: out parameter returns 0 for success, otherwise non zero
 *
 *	Start a new MMC custom command request for a host.
 *	If there is on ongoing async request wait for completion
 *	of that request and start the new one and return.
 *	Does not wait for the new request to complete.
 *
 *      Returns the completed request, NULL in case of none completed.
 *	Wait for the an ongoing request (previoulsy started) to complete and
 *	return the completed request. If there is no ongoing request, NULL
 *	is returned without waiting. NULL is not an error condition.
 */
struct mmc_async_req *mmc_start_req(struct mmc_host *host,
				    struct mmc_async_req *areq, int *error)
{
	int err = 0;
	int start_err = 0;
	struct mmc_async_req *data = host->areq;

	/* Prepare a new request */
	if (areq)
		mmc_pre_req(host, areq->mrq, !host->areq);

	if (host->areq) {
<<<<<<< HEAD
		mmc_wait_for_req_done(host, host->areq->mrq);
		err = host->areq->err_check(host->card, host->areq);
	}

	if (!err && areq)
		start_err = __mmc_start_req(host, areq->mrq);
=======
		err = mmc_wait_for_data_req_done(host, host->areq->mrq,	areq);
		if (err == MMC_BLK_NEW_REQUEST) {
			if (error)
				*error = err;
			/*
			 * The previous request was not completed,
			 * nothing to return
			 */
			return NULL;
		}
		/*
		 * Check BKOPS urgency for each R1 response
		 */
		if (host->card && mmc_card_mmc(host->card) &&
		    ((mmc_resp_type(host->areq->mrq->cmd) == MMC_RSP_R1) ||
		     (mmc_resp_type(host->areq->mrq->cmd) == MMC_RSP_R1B)) &&
		    (host->areq->mrq->cmd->resp[0] & R1_EXCEPTION_EVENT))
			mmc_start_bkops(host->card, true);
	}

	if (!err && areq)
		start_err = __mmc_start_data_req(host, areq->mrq);
>>>>>>> refs/remotes/origin/master

	if (host->areq)
		mmc_post_req(host, host->areq->mrq, 0);

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	/* Cancel a prepared request if it was not started. */
	if ((err || start_err) && areq)
			mmc_post_req(host, areq->mrq, -EINVAL);
=======
	 /* Cancel a prepared request if it was not started. */
	if ((err || start_err) && areq)
		mmc_post_req(host, areq->mrq, -EINVAL);
>>>>>>> refs/remotes/origin/master

	if (err)
		host->areq = NULL;
	else
		host->areq = areq;

	if (error)
		*error = err;
	return data;
}
EXPORT_SYMBOL(mmc_start_req);

/**
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 *	mmc_wait_for_req - start a request and wait for completion
 *	@host: MMC host to start command
 *	@mrq: MMC request to start
 *
 *	Start a new MMC custom command request for a host, and wait
 *	for the command to complete. Does not attempt to parse the
 *	response.
 */
void mmc_wait_for_req(struct mmc_host *host, struct mmc_request *mrq)
{
<<<<<<< HEAD
<<<<<<< HEAD
	DECLARE_COMPLETION_ONSTACK(complete);

	mrq->done_data = &complete;
	mrq->done = mmc_wait_done;
	if (mmc_card_removed(host->card)) {
		mrq->cmd->error = -ENOMEDIUM;
		return;
	}

	mmc_start_request(host, mrq);

	wait_for_completion_io(&complete);
}

EXPORT_SYMBOL(mmc_wait_for_req);
=======
=======
>>>>>>> refs/remotes/origin/master
	__mmc_start_req(host, mrq);
	mmc_wait_for_req_done(host, mrq);
}
EXPORT_SYMBOL(mmc_wait_for_req);

/**
 *	mmc_interrupt_hpi - Issue for High priority Interrupt
 *	@card: the MMC card associated with the HPI transfer
 *
 *	Issued High Priority Interrupt, and check for card status
<<<<<<< HEAD
 *	util out-of prg-state.
=======
 *	until out-of prg-state.
>>>>>>> refs/remotes/origin/master
 */
int mmc_interrupt_hpi(struct mmc_card *card)
{
	int err;
	u32 status;
<<<<<<< HEAD
=======
	unsigned long prg_wait;
>>>>>>> refs/remotes/origin/master

	BUG_ON(!card);

	if (!card->ext_csd.hpi_en) {
		pr_info("%s: HPI enable bit unset\n", mmc_hostname(card->host));
		return 1;
	}

	mmc_claim_host(card->host);
	err = mmc_send_status(card, &status);
	if (err) {
		pr_err("%s: Get card status fail\n", mmc_hostname(card->host));
		goto out;
	}

<<<<<<< HEAD
	/*
	 * If the card status is in PRG-state, we can send the HPI command.
	 */
	if (R1_CURRENT_STATE(status) == R1_STATE_PRG) {
		do {
			/*
			 * We don't know when the HPI command will finish
			 * processing, so we need to resend HPI until out
			 * of prg-state, and keep checking the card status
			 * with SEND_STATUS.  If a timeout error occurs when
			 * sending the HPI command, we are already out of
			 * prg-state.
			 */
			err = mmc_send_hpi_cmd(card, &status);
			if (err)
				pr_debug("%s: abort HPI (%d error)\n",
					 mmc_hostname(card->host), err);

			err = mmc_send_status(card, &status);
			if (err)
				break;
		} while (R1_CURRENT_STATE(status) == R1_STATE_PRG);
	} else
		pr_debug("%s: Left prg-state\n", mmc_hostname(card->host));
=======
	switch (R1_CURRENT_STATE(status)) {
	case R1_STATE_IDLE:
	case R1_STATE_READY:
	case R1_STATE_STBY:
	case R1_STATE_TRAN:
		/*
		 * In idle and transfer states, HPI is not needed and the caller
		 * can issue the next intended command immediately
		 */
		goto out;
	case R1_STATE_PRG:
		break;
	default:
		/* In all other states, it's illegal to issue HPI */
		pr_debug("%s: HPI cannot be sent. Card state=%d\n",
			mmc_hostname(card->host), R1_CURRENT_STATE(status));
		err = -EINVAL;
		goto out;
	}

	err = mmc_send_hpi_cmd(card, &status);
	if (err)
		goto out;

	prg_wait = jiffies + msecs_to_jiffies(card->ext_csd.out_of_int_time);
	do {
		err = mmc_send_status(card, &status);

		if (!err && R1_CURRENT_STATE(status) == R1_STATE_TRAN)
			break;
		if (time_after(jiffies, prg_wait))
			err = -ETIMEDOUT;
	} while (!err);
>>>>>>> refs/remotes/origin/master

out:
	mmc_release_host(card->host);
	return err;
}
EXPORT_SYMBOL(mmc_interrupt_hpi);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

/**
 *	mmc_wait_for_cmd - start a command and wait for completion
 *	@host: MMC host to start command
 *	@cmd: MMC command to start
 *	@retries: maximum number of retries
 *
 *	Start a new MMC command for a host, and wait for the command
 *	to complete.  Return any error that occurred while the command
 *	was executing.  Do not attempt to parse the response.
 */
int mmc_wait_for_cmd(struct mmc_host *host, struct mmc_command *cmd, int retries)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct mmc_request mrq = {0};
=======
	struct mmc_request mrq = {NULL};
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct mmc_request mrq = {NULL};
>>>>>>> refs/remotes/origin/master

	WARN_ON(!host->claimed);

	memset(cmd->resp, 0, sizeof(cmd->resp));
	cmd->retries = retries;

	mrq.cmd = cmd;
	cmd->data = NULL;

	mmc_wait_for_req(host, &mrq);

	return cmd->error;
}

EXPORT_SYMBOL(mmc_wait_for_cmd);

/**
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
 *	mmc_interrupt_bkops - interrupt ongoing BKOPS
 *	@card: MMC card to check BKOPS
 *
 *	Send HPI command to interrupt ongoing background operations,
 *	to allow rapid servicing of foreground operations,e.g. read/
 *	writes. Wait until the card comes out of the programming state
 *	to avoid errors in servicing read/write requests.
 */
int mmc_interrupt_bkops(struct mmc_card *card)
{
	int err = 0;
	unsigned long flags;

	BUG_ON(!card);

	err = mmc_interrupt_hpi(card);

	spin_lock_irqsave(&card->host->lock, flags);
	mmc_card_clr_doing_bkops(card);
	spin_unlock_irqrestore(&card->host->lock, flags);

	return err;
}
EXPORT_SYMBOL(mmc_interrupt_bkops);
<<<<<<< HEAD
=======
 *	mmc_stop_bkops - stop ongoing BKOPS
 *	@card: MMC card to check BKOPS
 *
 *	Send HPI command to stop ongoing background operations to
 *	allow rapid servicing of foreground operations, e.g. read/
 *	writes. Wait until the card comes out of the programming state
 *	to avoid errors in servicing read/write requests.
 */
int mmc_stop_bkops(struct mmc_card *card)
{
	int err = 0;

	BUG_ON(!card);
	err = mmc_interrupt_hpi(card);

	/*
	 * If err is EINVAL, we can't issue an HPI.
	 * It should complete the BKOPS.
	 */
	if (!err || (err == -EINVAL)) {
		mmc_card_clr_doing_bkops(card);
		err = 0;
	}

	return err;
}
EXPORT_SYMBOL(mmc_stop_bkops);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

int mmc_read_bkops_status(struct mmc_card *card)
{
	int err;
<<<<<<< HEAD
<<<<<<< HEAD
	u8 ext_csd[512];
=======
	u8 *ext_csd;

	/*
	 * In future work, we should consider storing the entire ext_csd.
	 */
	ext_csd = kmalloc(512, GFP_KERNEL);
	if (!ext_csd) {
		pr_err("%s: could not allocate buffer to receive the ext_csd.\n",
		       mmc_hostname(card->host));
		return -ENOMEM;
	}
>>>>>>> refs/remotes/origin/master
=======
	u8 ext_csd[512];
>>>>>>> refs/remotes/origin/cm-11.0

	mmc_claim_host(card->host);
	err = mmc_send_ext_csd(card, ext_csd);
	mmc_release_host(card->host);
	if (err)
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
		return err;

	card->ext_csd.raw_bkops_status = ext_csd[EXT_CSD_BKOPS_STATUS];
	card->ext_csd.raw_exception_status = ext_csd[EXT_CSD_EXP_EVENTS_STATUS];

	return 0;
}
EXPORT_SYMBOL(mmc_read_bkops_status);

int mmc_is_exception_event(struct mmc_card *card, unsigned int value)
{
	int err;

	err = mmc_read_bkops_status(card);
	if (err) {
		pr_err("%s: Didn't read bkops status : %d\n",
		       mmc_hostname(card->host), err);
		return 0;
	}

	/* In eMMC 4.41, R1_EXCEPTION_EVENT is URGENT_BKOPS */
	if (card->ext_csd.rev == 5)
		return 1;

	return (card->ext_csd.raw_exception_status & value) ? 1 : 0;
}
EXPORT_SYMBOL(mmc_is_exception_event);

/**
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
		goto out;

	card->ext_csd.raw_bkops_status = ext_csd[EXT_CSD_BKOPS_STATUS];
	card->ext_csd.raw_exception_status = ext_csd[EXT_CSD_EXP_EVENTS_STATUS];
out:
	kfree(ext_csd);
	return err;
}
EXPORT_SYMBOL(mmc_read_bkops_status);

/**
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
 *	mmc_set_data_timeout - set the timeout for a data command
 *	@data: data phase for command
 *	@card: the MMC card associated with the data transfer
 *
 *	Computes the data timeout parameters according to the
 *	correct algorithm given the card type.
 */
void mmc_set_data_timeout(struct mmc_data *data, const struct mmc_card *card)
{
	unsigned int mult;

	/*
	 * SDIO cards only define an upper 1 s limit on access.
	 */
	if (mmc_card_sdio(card)) {
		data->timeout_ns = 1000000000;
		data->timeout_clks = 0;
		return;
	}

	/*
	 * SD cards use a 100 multiplier rather than 10
	 */
	mult = mmc_card_sd(card) ? 100 : 10;

	/*
	 * Scale up the multiplier (and therefore the timeout) by
	 * the r2w factor for writes.
	 */
	if (data->flags & MMC_DATA_WRITE)
		mult <<= card->csd.r2w_factor;

	data->timeout_ns = card->csd.tacc_ns * mult;
	data->timeout_clks = card->csd.tacc_clks * mult;

	/*
	 * SD cards also have an upper limit on the timeout.
	 */
	if (mmc_card_sd(card)) {
		unsigned int timeout_us, limit_us;

		timeout_us = data->timeout_ns / 1000;
		if (mmc_host_clk_rate(card->host))
			timeout_us += data->timeout_clks * 1000 /
				(mmc_host_clk_rate(card->host) / 1000);

		if (data->flags & MMC_DATA_WRITE)
			/*
<<<<<<< HEAD
<<<<<<< HEAD
			 * The limit is really 250 ms, but that is
			 * insufficient for some crappy cards.
			 */
			limit_us = 800000;
=======
=======
>>>>>>> refs/remotes/origin/master
			 * The MMC spec "It is strongly recommended
			 * for hosts to implement more than 500ms
			 * timeout value even if the card indicates
			 * the 250ms maximum busy length."  Even the
			 * previous value of 300ms is known to be
			 * insufficient for some cards.
			 */
			limit_us = 3000000;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		else
			limit_us = 100000;

		/*
		 * SDHC cards always use these fixed values.
		 */
		if (timeout_us > limit_us || mmc_card_blockaddr(card)) {
			data->timeout_ns = limit_us * 1000;
			data->timeout_clks = 0;
		}
	}
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master

	/*
	 * Some cards require longer data read timeout than indicated in CSD.
	 * Address this by setting the read timeout to a "reasonably high"
	 * value. For the cards tested, 300ms has proven enough. If necessary,
	 * this value can be increased if other problematic cards require this.
	 */
	if (mmc_card_long_read_time(card) && data->flags & MMC_DATA_READ) {
		data->timeout_ns = 300000000;
		data->timeout_clks = 0;
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * Some cards need very high timeouts if driven in SPI mode.
	 * The worst observed timeout was 900ms after writing a
	 * continuous stream of data until the internal logic
	 * overflowed.
	 */
	if (mmc_host_is_spi(card->host)) {
		if (data->flags & MMC_DATA_WRITE) {
			if (data->timeout_ns < 1000000000)
				data->timeout_ns = 1000000000;	/* 1s */
		} else {
			if (data->timeout_ns < 100000000)
				data->timeout_ns =  100000000;	/* 100ms */
		}
	}
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	/* Increase the timeout values for some bad INAND MCP devices */
	if (card->quirks & MMC_QUIRK_INAND_DATA_TIMEOUT) {
		data->timeout_ns = 4000000000u; /* 4s */
		data->timeout_clks = 0;
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
}
EXPORT_SYMBOL(mmc_set_data_timeout);

/**
 *	mmc_align_data_size - pads a transfer size to a more optimal value
 *	@card: the MMC card associated with the data transfer
 *	@sz: original transfer size
 *
 *	Pads the original data size with a number of extra bytes in
 *	order to avoid controller bugs and/or performance hits
 *	(e.g. some controllers revert to PIO for certain sizes).
 *
 *	Returns the improved size, which might be unmodified.
 *
 *	Note that this function is only relevant when issuing a
 *	single scatter gather entry.
 */
unsigned int mmc_align_data_size(struct mmc_card *card, unsigned int sz)
{
	/*
	 * FIXME: We don't have a system for the controller to tell
	 * the core about its problems yet, so for now we just 32-bit
	 * align the size.
	 */
	sz = ((sz + 3) / 4) * 4;

	return sz;
}
EXPORT_SYMBOL(mmc_align_data_size);

/**
<<<<<<< HEAD
<<<<<<< HEAD
 *	mmc_host_enable - enable a host.
 *	@host: mmc host to enable
 *
 *	Hosts that support power saving can use the 'enable' and 'disable'
 *	methods to exit and enter power saving states. For more information
 *	see comments for struct mmc_host_ops.
 */
int mmc_host_enable(struct mmc_host *host)
{
	if (!(host->caps & MMC_CAP_DISABLE))
		return 0;

	if (host->en_dis_recurs)
		return 0;

	if (host->nesting_cnt++)
		return 0;

	cancel_delayed_work_sync(&host->disable);

	if (host->enabled)
		return 0;

	if (host->ops->enable) {
		int err;

		host->en_dis_recurs = 1;
		mmc_host_clk_hold(host);
		err = host->ops->enable(host);
		mmc_host_clk_release(host);
		host->en_dis_recurs = 0;

		if (err) {
			pr_debug("%s: enable error %d\n",
				 mmc_hostname(host), err);
			return err;
		}
	}
	host->enabled = 1;
	return 0;
}
EXPORT_SYMBOL(mmc_host_enable);

static int mmc_host_do_disable(struct mmc_host *host, int lazy)
{
	if (host->ops->disable) {
		int err;

		host->en_dis_recurs = 1;
		mmc_host_clk_hold(host);
		err = host->ops->disable(host, lazy);
		mmc_host_clk_release(host);
		host->en_dis_recurs = 0;

		if (err < 0) {
			pr_debug("%s: disable error %d\n",
				 mmc_hostname(host), err);
			return err;
		}
		if (err > 0) {
			unsigned long delay = msecs_to_jiffies(err);

			mmc_schedule_delayed_work(&host->disable, delay);
		}
	}
	host->enabled = 0;
	return 0;
}

/**
 *	mmc_host_disable - disable a host.
 *	@host: mmc host to disable
 *
 *	Hosts that support power saving can use the 'enable' and 'disable'
 *	methods to exit and enter power saving states. For more information
 *	see comments for struct mmc_host_ops.
 */
int mmc_host_disable(struct mmc_host *host)
{
	int err;

	if (!(host->caps & MMC_CAP_DISABLE))
		return 0;

	if (host->en_dis_recurs)
		return 0;

	if (--host->nesting_cnt)
		return 0;

	if (!host->enabled)
		return 0;

	err = mmc_host_do_disable(host, 0);
	return err;
}
EXPORT_SYMBOL(mmc_host_disable);

/**
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 *	__mmc_claim_host - exclusively claim a host
 *	@host: mmc host to claim
 *	@abort: whether or not the operation should be aborted
 *
 *	Claim a host for a set of operations.  If @abort is non null and
 *	dereference a non-zero value then this will return prematurely with
 *	that non-zero value without acquiring the lock.  Returns zero
 *	with the lock held otherwise.
 */
int __mmc_claim_host(struct mmc_host *host, atomic_t *abort)
{
	DECLARE_WAITQUEUE(wait, current);
	unsigned long flags;
	int stop;

	might_sleep();

	add_wait_queue(&host->wq, &wait);
<<<<<<< HEAD
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
=======

>>>>>>> refs/remotes/origin/cm-11.0
	spin_lock_irqsave(&host->lock, flags);
	while (1) {
		set_current_state(TASK_UNINTERRUPTIBLE);
		stop = abort ? atomic_read(abort) : 0;
		if (stop || !host->claimed || host->claimer == current)
			break;
		spin_unlock_irqrestore(&host->lock, flags);
		schedule();
		spin_lock_irqsave(&host->lock, flags);
	}
	set_current_state(TASK_RUNNING);
	if (!stop) {
		host->claimed = 1;
		host->claimer = current;
		host->claim_cnt += 1;
	} else
		wake_up(&host->wq);
	spin_unlock_irqrestore(&host->lock, flags);
	remove_wait_queue(&host->wq, &wait);
<<<<<<< HEAD
<<<<<<< HEAD
	if (!stop)
		mmc_host_enable(host);
=======
	if (host->ops->enable && !stop && host->claim_cnt == 1)
		host->ops->enable(host);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (host->ops->enable && !stop && host->claim_cnt == 1)
		host->ops->enable(host);
>>>>>>> refs/remotes/origin/master
	return stop;
}

EXPORT_SYMBOL(__mmc_claim_host);

/**
<<<<<<< HEAD
 *	mmc_try_claim_host - try exclusively to claim a host
 *	@host: mmc host to claim
 *
 *	Returns %1 if the host is claimed, %0 otherwise.
 */
int mmc_try_claim_host(struct mmc_host *host)
{
	int claimed_host = 0;
	unsigned long flags;

	spin_lock_irqsave(&host->lock, flags);
	if (!host->claimed || host->claimer == current) {
		host->claimed = 1;
		host->claimer = current;
		host->claim_cnt += 1;
		claimed_host = 1;
	}
	spin_unlock_irqrestore(&host->lock, flags);
<<<<<<< HEAD
=======
	if (host->ops->enable && claimed_host && host->claim_cnt == 1)
		host->ops->enable(host);
>>>>>>> refs/remotes/origin/cm-10.0
	return claimed_host;
}
EXPORT_SYMBOL(mmc_try_claim_host);

/**
<<<<<<< HEAD
 *	mmc_do_release_host - release a claimed host
 *	@host: mmc host to release
 *
 *	If you successfully claimed a host, this function will
 *	release it again.
 */
void mmc_do_release_host(struct mmc_host *host)
{
	unsigned long flags;

=======
=======
>>>>>>> refs/remotes/origin/master
 *	mmc_release_host - release a host
 *	@host: mmc host to release
 *
 *	Release a MMC host, allowing others to claim the host
 *	for their operations.
 */
void mmc_release_host(struct mmc_host *host)
{
	unsigned long flags;

	WARN_ON(!host->claimed);

	if (host->ops->disable && host->claim_cnt == 1)
		host->ops->disable(host);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	spin_lock_irqsave(&host->lock, flags);
	if (--host->claim_cnt) {
		/* Release for nested claim */
		spin_unlock_irqrestore(&host->lock, flags);
	} else {
		host->claimed = 0;
		host->claimer = NULL;
		spin_unlock_irqrestore(&host->lock, flags);
		wake_up(&host->wq);
	}
}
<<<<<<< HEAD
<<<<<<< HEAD
EXPORT_SYMBOL(mmc_do_release_host);

void mmc_host_deeper_disable(struct work_struct *work)
{
	struct mmc_host *host =
		container_of(work, struct mmc_host, disable.work);

	/* If the host is claimed then we do not want to disable it anymore */
	if (!mmc_try_claim_host(host))
		return;
	mmc_host_do_disable(host, 1);
	mmc_do_release_host(host);
}

/**
 *	mmc_host_lazy_disable - lazily disable a host.
 *	@host: mmc host to disable
 *
 *	Hosts that support power saving can use the 'enable' and 'disable'
 *	methods to exit and enter power saving states. For more information
 *	see comments for struct mmc_host_ops.
 */
int mmc_host_lazy_disable(struct mmc_host *host)
{
	if (!(host->caps & MMC_CAP_DISABLE))
		return 0;

	if (host->en_dis_recurs)
		return 0;

	if (--host->nesting_cnt)
		return 0;

	if (!host->enabled)
		return 0;

	if (host->disable_delay) {
		mmc_schedule_delayed_work(&host->disable,
				msecs_to_jiffies(host->disable_delay));
		return 0;
	} else
		return mmc_host_do_disable(host, 1);
}
EXPORT_SYMBOL(mmc_host_lazy_disable);

/**
 *	mmc_release_host - release a host
 *	@host: mmc host to release
 *
 *	Release a MMC host, allowing others to claim the host
 *	for their operations.
 */
void mmc_release_host(struct mmc_host *host)
{
	WARN_ON(!host->claimed);

	mmc_host_lazy_disable(host);

	mmc_do_release_host(host);
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
EXPORT_SYMBOL(mmc_release_host);
=======
EXPORT_SYMBOL(mmc_release_host);

/*
 * This is a helper function, which fetches a runtime pm reference for the
 * card device and also claims the host.
 */
void mmc_get_card(struct mmc_card *card)
{
	pm_runtime_get_sync(&card->dev);
	mmc_claim_host(card->host);
}
EXPORT_SYMBOL(mmc_get_card);

/*
 * This is a helper function, which releases the host and drops the runtime
 * pm reference for the card device.
 */
void mmc_put_card(struct mmc_card *card)
{
	mmc_release_host(card->host);
	pm_runtime_mark_last_busy(&card->dev);
	pm_runtime_put_autosuspend(&card->dev);
}
EXPORT_SYMBOL(mmc_put_card);
>>>>>>> refs/remotes/origin/master

/*
 * Internal function that does the actual ios call to the host driver,
 * optionally printing some debug output.
 */
<<<<<<< HEAD
<<<<<<< HEAD
void mmc_set_ios(struct mmc_host *host)
=======
static inline void mmc_set_ios(struct mmc_host *host)
>>>>>>> refs/remotes/origin/master
=======
void mmc_set_ios(struct mmc_host *host)
>>>>>>> refs/remotes/origin/cm-11.0
{
	struct mmc_ios *ios = &host->ios;

	pr_debug("%s: clock %uHz busmode %u powermode %u cs %u Vdd %u "
		"width %u timing %u\n",
		 mmc_hostname(host), ios->clock, ios->bus_mode,
		 ios->power_mode, ios->chip_select, ios->vdd,
		 ios->bus_width, ios->timing);

	if (ios->clock > 0)
		mmc_set_ungated(host);
	host->ops->set_ios(host, ios);
}
<<<<<<< HEAD
<<<<<<< HEAD
EXPORT_SYMBOL(mmc_set_ios);
=======
>>>>>>> refs/remotes/origin/master
=======
EXPORT_SYMBOL(mmc_set_ios);
>>>>>>> refs/remotes/origin/cm-11.0

/*
 * Control chip select pin on a host.
 */
void mmc_set_chip_select(struct mmc_host *host, int mode)
{
	mmc_host_clk_hold(host);
	host->ios.chip_select = mode;
	mmc_set_ios(host);
	mmc_host_clk_release(host);
}

/*
 * Sets the host clock to the highest possible frequency that
 * is below "hz".
 */
static void __mmc_set_clock(struct mmc_host *host, unsigned int hz)
{
	WARN_ON(hz < host->f_min);

	if (hz > host->f_max)
		hz = host->f_max;

	host->ios.clock = hz;
	mmc_set_ios(host);
}

void mmc_set_clock(struct mmc_host *host, unsigned int hz)
{
	mmc_host_clk_hold(host);
	__mmc_set_clock(host, hz);
	mmc_host_clk_release(host);
}

#ifdef CONFIG_MMC_CLKGATE
/*
 * This gates the clock by setting it to 0 Hz.
 */
void mmc_gate_clock(struct mmc_host *host)
{
	unsigned long flags;

<<<<<<< HEAD
<<<<<<< HEAD
	WARN_ON(!host->ios.clock);

=======
>>>>>>> refs/remotes/origin/master
=======
	WARN_ON(!host->ios.clock);

>>>>>>> refs/remotes/origin/cm-11.0
	spin_lock_irqsave(&host->clk_lock, flags);
	host->clk_old = host->ios.clock;
	host->ios.clock = 0;
	host->clk_gated = true;
	spin_unlock_irqrestore(&host->clk_lock, flags);
	mmc_set_ios(host);
}

/*
 * This restores the clock from gating by using the cached
 * clock value.
 */
void mmc_ungate_clock(struct mmc_host *host)
{
	/*
	 * We should previously have gated the clock, so the clock shall
	 * be 0 here! The clock may however be 0 during initialization,
	 * when some request operations are performed before setting
	 * the frequency. When ungate is requested in that situation
	 * we just ignore the call.
	 */
	if (host->clk_old) {
<<<<<<< HEAD
<<<<<<< HEAD
		WARN_ON(host->ios.clock);
=======
		BUG_ON(host->ios.clock);
>>>>>>> refs/remotes/origin/master
=======
		WARN_ON(host->ios.clock);
>>>>>>> refs/remotes/origin/cm-11.0
		/* This call will also set host->clk_gated to false */
		__mmc_set_clock(host, host->clk_old);
	}
}

void mmc_set_ungated(struct mmc_host *host)
{
	unsigned long flags;

	/*
	 * We've been given a new frequency while the clock is gated,
	 * so make sure we regard this as ungating it.
	 */
	spin_lock_irqsave(&host->clk_lock, flags);
	host->clk_gated = false;
	spin_unlock_irqrestore(&host->clk_lock, flags);
}

#else
void mmc_set_ungated(struct mmc_host *host)
{
}
#endif

/*
 * Change the bus mode (open drain/push-pull) of a host.
 */
void mmc_set_bus_mode(struct mmc_host *host, unsigned int mode)
{
	mmc_host_clk_hold(host);
	host->ios.bus_mode = mode;
	mmc_set_ios(host);
	mmc_host_clk_release(host);
}

/*
 * Change data bus width of a host.
 */
void mmc_set_bus_width(struct mmc_host *host, unsigned int width)
{
	mmc_host_clk_hold(host);
	host->ios.bus_width = width;
	mmc_set_ios(host);
	mmc_host_clk_release(host);
}

/**
 * mmc_vdd_to_ocrbitnum - Convert a voltage to the OCR bit number
 * @vdd:	voltage (mV)
 * @low_bits:	prefer low bits in boundary cases
 *
 * This function returns the OCR bit number according to the provided @vdd
 * value. If conversion is not possible a negative errno value returned.
 *
 * Depending on the @low_bits flag the function prefers low or high OCR bits
 * on boundary voltages. For example,
 * with @low_bits = true, 3300 mV translates to ilog2(MMC_VDD_32_33);
 * with @low_bits = false, 3300 mV translates to ilog2(MMC_VDD_33_34);
 *
 * Any value in the [1951:1999] range translates to the ilog2(MMC_VDD_20_21).
 */
static int mmc_vdd_to_ocrbitnum(int vdd, bool low_bits)
{
	const int max_bit = ilog2(MMC_VDD_35_36);
	int bit;

	if (vdd < 1650 || vdd > 3600)
		return -EINVAL;

	if (vdd >= 1650 && vdd <= 1950)
		return ilog2(MMC_VDD_165_195);

	if (low_bits)
		vdd -= 1;

	/* Base 2000 mV, step 100 mV, bit's base 8. */
	bit = (vdd - 2000) / 100 + 8;
	if (bit > max_bit)
		return max_bit;
	return bit;
}

/**
 * mmc_vddrange_to_ocrmask - Convert a voltage range to the OCR mask
 * @vdd_min:	minimum voltage value (mV)
 * @vdd_max:	maximum voltage value (mV)
 *
 * This function returns the OCR mask bits according to the provided @vdd_min
 * and @vdd_max values. If conversion is not possible the function returns 0.
 *
 * Notes wrt boundary cases:
 * This function sets the OCR bits for all boundary voltages, for example
 * [3300:3400] range is translated to MMC_VDD_32_33 | MMC_VDD_33_34 |
 * MMC_VDD_34_35 mask.
 */
u32 mmc_vddrange_to_ocrmask(int vdd_min, int vdd_max)
{
	u32 mask = 0;

	if (vdd_max < vdd_min)
		return 0;

	/* Prefer high bits for the boundary vdd_max values. */
	vdd_max = mmc_vdd_to_ocrbitnum(vdd_max, false);
	if (vdd_max < 0)
		return 0;

	/* Prefer low bits for the boundary vdd_min values. */
	vdd_min = mmc_vdd_to_ocrbitnum(vdd_min, true);
	if (vdd_min < 0)
		return 0;

	/* Fill the mask, from max bit to min bit. */
	while (vdd_max >= vdd_min)
		mask |= 1 << vdd_max--;

	return mask;
}
EXPORT_SYMBOL(mmc_vddrange_to_ocrmask);

<<<<<<< HEAD
=======
#ifdef CONFIG_OF

/**
 * mmc_of_parse_voltage - return mask of supported voltages
 * @np: The device node need to be parsed.
 * @mask: mask of voltages available for MMC/SD/SDIO
 *
 * 1. Return zero on success.
 * 2. Return negative errno: voltage-range is invalid.
 */
int mmc_of_parse_voltage(struct device_node *np, u32 *mask)
{
	const u32 *voltage_ranges;
	int num_ranges, i;

	voltage_ranges = of_get_property(np, "voltage-ranges", &num_ranges);
	num_ranges = num_ranges / sizeof(*voltage_ranges) / 2;
	if (!voltage_ranges || !num_ranges) {
		pr_info("%s: voltage-ranges unspecified\n", np->full_name);
		return -EINVAL;
	}

	for (i = 0; i < num_ranges; i++) {
		const int j = i * 2;
		u32 ocr_mask;

		ocr_mask = mmc_vddrange_to_ocrmask(
				be32_to_cpu(voltage_ranges[j]),
				be32_to_cpu(voltage_ranges[j + 1]));
		if (!ocr_mask) {
			pr_err("%s: voltage-range #%d is invalid\n",
				np->full_name, i);
			return -EINVAL;
		}
		*mask |= ocr_mask;
	}

	return 0;
}
EXPORT_SYMBOL(mmc_of_parse_voltage);

#endif /* CONFIG_OF */

>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_REGULATOR

/**
 * mmc_regulator_get_ocrmask - return mask of supported voltages
 * @supply: regulator to use
 *
 * This returns either a negative errno, or a mask of voltages that
 * can be provided to MMC/SD/SDIO devices using the specified voltage
 * regulator.  This would normally be called before registering the
 * MMC host adapter.
 */
int mmc_regulator_get_ocrmask(struct regulator *supply)
{
	int			result = 0;
	int			count;
	int			i;

	count = regulator_count_voltages(supply);
	if (count < 0)
		return count;

	for (i = 0; i < count; i++) {
		int		vdd_uV;
		int		vdd_mV;

		vdd_uV = regulator_list_voltage(supply, i);
		if (vdd_uV <= 0)
			continue;

		vdd_mV = vdd_uV / 1000;
		result |= mmc_vddrange_to_ocrmask(vdd_mV, vdd_mV);
	}

	return result;
}
<<<<<<< HEAD
EXPORT_SYMBOL(mmc_regulator_get_ocrmask);
=======
EXPORT_SYMBOL_GPL(mmc_regulator_get_ocrmask);
>>>>>>> refs/remotes/origin/master

/**
 * mmc_regulator_set_ocr - set regulator to match host->ios voltage
 * @mmc: the host to regulate
 * @supply: regulator to use
 * @vdd_bit: zero for power off, else a bit number (host->ios.vdd)
 *
 * Returns zero on success, else negative errno.
 *
 * MMC host drivers may use this to enable or disable a regulator using
 * a particular supply voltage.  This would normally be called from the
 * set_ios() method.
 */
int mmc_regulator_set_ocr(struct mmc_host *mmc,
			struct regulator *supply,
			unsigned short vdd_bit)
{
	int			result = 0;
	int			min_uV, max_uV;

	if (vdd_bit) {
		int		tmp;
		int		voltage;

<<<<<<< HEAD
		/* REVISIT mmc_vddrange_to_ocrmask() may have set some
=======
		/*
		 * REVISIT mmc_vddrange_to_ocrmask() may have set some
>>>>>>> refs/remotes/origin/master
		 * bits this regulator doesn't quite support ... don't
		 * be too picky, most cards and regulators are OK with
		 * a 0.1V range goof (it's a small error percentage).
		 */
		tmp = vdd_bit - ilog2(MMC_VDD_165_195);
		if (tmp == 0) {
			min_uV = 1650 * 1000;
			max_uV = 1950 * 1000;
		} else {
			min_uV = 1900 * 1000 + tmp * 100 * 1000;
			max_uV = min_uV + 100 * 1000;
		}

<<<<<<< HEAD
		/* avoid needless changes to this voltage; the regulator
		 * might not allow this operation
		 */
		voltage = regulator_get_voltage(supply);
<<<<<<< HEAD
=======

		if (mmc->caps2 & MMC_CAP2_BROKEN_VOLTAGE)
			min_uV = max_uV = voltage;

>>>>>>> refs/remotes/origin/cm-10.0
=======
		/*
		 * If we're using a fixed/static regulator, don't call
		 * regulator_set_voltage; it would fail.
		 */
		voltage = regulator_get_voltage(supply);

		if (!regulator_can_change_voltage(supply))
			min_uV = max_uV = voltage;

>>>>>>> refs/remotes/origin/master
		if (voltage < 0)
			result = voltage;
		else if (voltage < min_uV || voltage > max_uV)
			result = regulator_set_voltage(supply, min_uV, max_uV);
		else
			result = 0;

		if (result == 0 && !mmc->regulator_enabled) {
			result = regulator_enable(supply);
			if (!result)
				mmc->regulator_enabled = true;
		}
	} else if (mmc->regulator_enabled) {
		result = regulator_disable(supply);
		if (result == 0)
			mmc->regulator_enabled = false;
	}

	if (result)
		dev_err(mmc_dev(mmc),
			"could not set regulator OCR (%d)\n", result);
	return result;
}
<<<<<<< HEAD
EXPORT_SYMBOL(mmc_regulator_set_ocr);
=======
EXPORT_SYMBOL_GPL(mmc_regulator_set_ocr);

int mmc_regulator_get_supply(struct mmc_host *mmc)
{
	struct device *dev = mmc_dev(mmc);
	struct regulator *supply;
	int ret;

	supply = devm_regulator_get(dev, "vmmc");
	mmc->supply.vmmc = supply;
	mmc->supply.vqmmc = devm_regulator_get_optional(dev, "vqmmc");

	if (IS_ERR(supply))
		return PTR_ERR(supply);

	ret = mmc_regulator_get_ocrmask(supply);
	if (ret > 0)
		mmc->ocr_avail = ret;
	else
		dev_warn(mmc_dev(mmc), "Failed getting OCR mask: %d\n", ret);

	return 0;
}
EXPORT_SYMBOL_GPL(mmc_regulator_get_supply);
>>>>>>> refs/remotes/origin/master

#endif /* CONFIG_REGULATOR */

/*
 * Mask off any voltages we don't support and select
 * the lowest voltage
 */
u32 mmc_select_voltage(struct mmc_host *host, u32 ocr)
{
	int bit;

<<<<<<< HEAD
	ocr &= host->ocr_avail;

	bit = ffs(ocr);
	if (bit) {
		bit -= 1;

		ocr &= 3 << bit;

		mmc_host_clk_hold(host);
		host->ios.vdd = bit;
		mmc_set_ios(host);
		mmc_host_clk_release(host);
	} else {
		pr_warning("%s: host doesn't support card's voltages\n",
				mmc_hostname(host));
		ocr = 0;
	}

	return ocr;
}

int mmc_set_signal_voltage(struct mmc_host *host, int signal_voltage, bool cmd11)
{
	struct mmc_command cmd = {0};
	int err = 0;
=======
	/*
	 * Sanity check the voltages that the card claims to
	 * support.
	 */
	if (ocr & 0x7F) {
		dev_warn(mmc_dev(host),
		"card claims to support voltages below defined range\n");
		ocr &= ~0x7F;
	}

	ocr &= host->ocr_avail;
	if (!ocr) {
		dev_warn(mmc_dev(host), "no support for card's volts\n");
		return 0;
	}

	if (host->caps2 & MMC_CAP2_FULL_PWR_CYCLE) {
		bit = ffs(ocr) - 1;
		ocr &= 3 << bit;
		mmc_power_cycle(host, ocr);
	} else {
		bit = fls(ocr) - 1;
		ocr &= 3 << bit;
		if (bit != host->ios.vdd)
			dev_warn(mmc_dev(host), "exceeding card's volts\n");
	}

	return ocr;
}

int __mmc_set_signal_voltage(struct mmc_host *host, int signal_voltage)
{
	int err = 0;
	int old_signal_voltage = host->ios.signal_voltage;

	host->ios.signal_voltage = signal_voltage;
	if (host->ops->start_signal_voltage_switch) {
		mmc_host_clk_hold(host);
		err = host->ops->start_signal_voltage_switch(host, &host->ios);
		mmc_host_clk_release(host);
	}

	if (err)
		host->ios.signal_voltage = old_signal_voltage;

	return err;

}

int mmc_set_signal_voltage(struct mmc_host *host, int signal_voltage, u32 ocr)
{
	struct mmc_command cmd = {0};
	int err = 0;
	u32 clock;
>>>>>>> refs/remotes/origin/master

	BUG_ON(!host);

	/*
	 * Send CMD11 only if the request is to switch the card to
	 * 1.8V signalling.
	 */
<<<<<<< HEAD
	if ((signal_voltage != MMC_SIGNAL_VOLTAGE_330) && cmd11) {
		cmd.opcode = SD_SWITCH_VOLTAGE;
		cmd.arg = 0;
		cmd.flags = MMC_RSP_R1 | MMC_CMD_AC;

		err = mmc_wait_for_cmd(host, &cmd, 0);
		if (err)
			return err;

		if (!mmc_host_is_spi(host) && (cmd.resp[0] & R1_ERROR))
			return -EIO;
	}

	host->ios.signal_voltage = signal_voltage;

	if (host->ops->start_signal_voltage_switch) {
		mmc_host_clk_hold(host);
		err = host->ops->start_signal_voltage_switch(host, &host->ios);
		mmc_host_clk_release(host);
	}

=======
	if (signal_voltage == MMC_SIGNAL_VOLTAGE_330)
		return __mmc_set_signal_voltage(host, signal_voltage);

	/*
	 * If we cannot switch voltages, return failure so the caller
	 * can continue without UHS mode
	 */
	if (!host->ops->start_signal_voltage_switch)
		return -EPERM;
	if (!host->ops->card_busy)
		pr_warning("%s: cannot verify signal voltage switch\n",
				mmc_hostname(host));

	cmd.opcode = SD_SWITCH_VOLTAGE;
	cmd.arg = 0;
	cmd.flags = MMC_RSP_R1 | MMC_CMD_AC;

	err = mmc_wait_for_cmd(host, &cmd, 0);
	if (err)
		return err;

	if (!mmc_host_is_spi(host) && (cmd.resp[0] & R1_ERROR))
		return -EIO;

	mmc_host_clk_hold(host);
	/*
	 * The card should drive cmd and dat[0:3] low immediately
	 * after the response of cmd11, but wait 1 ms to be sure
	 */
	mmc_delay(1);
	if (host->ops->card_busy && !host->ops->card_busy(host)) {
		err = -EAGAIN;
		goto power_cycle;
	}
	/*
	 * During a signal voltage level switch, the clock must be gated
	 * for 5 ms according to the SD spec
	 */
	clock = host->ios.clock;
	host->ios.clock = 0;
	mmc_set_ios(host);

	if (__mmc_set_signal_voltage(host, signal_voltage)) {
		/*
		 * Voltages may not have been switched, but we've already
		 * sent CMD11, so a power cycle is required anyway
		 */
		err = -EAGAIN;
		goto power_cycle;
	}

	/* Keep clock gated for at least 5 ms */
	mmc_delay(5);
	host->ios.clock = clock;
	mmc_set_ios(host);

	/* Wait for at least 1 ms according to spec */
	mmc_delay(1);

	/*
	 * Failure to switch is indicated by the card holding
	 * dat[0:3] low
	 */
	if (host->ops->card_busy && host->ops->card_busy(host))
		err = -EAGAIN;

power_cycle:
	if (err) {
		pr_debug("%s: Signal voltage switch failed, "
			"power cycling card\n", mmc_hostname(host));
		mmc_power_cycle(host, ocr);
	}

	mmc_host_clk_release(host);

>>>>>>> refs/remotes/origin/master
	return err;
}

/*
 * Select timing parameters for host.
 */
void mmc_set_timing(struct mmc_host *host, unsigned int timing)
{
	mmc_host_clk_hold(host);
	host->ios.timing = timing;
	mmc_set_ios(host);
	mmc_host_clk_release(host);
}

/*
 * Select appropriate driver type for host.
 */
void mmc_set_driver_type(struct mmc_host *host, unsigned int drv_type)
{
	mmc_host_clk_hold(host);
	host->ios.drv_type = drv_type;
	mmc_set_ios(host);
	mmc_host_clk_release(host);
}
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======

>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
/*
 * Apply power to the MMC stack.  This is a two-stage process.
 * First, we enable power to the card without the clock running.
 * We then wait a bit for the power to stabilise.  Finally,
 * enable the bus drivers and clock to the card.
 *
 * We must _NOT_ enable the clock prior to power stablising.
 *
 * If a host does all the power sequencing itself, ignore the
 * initial MMC_POWER_UP stage.
 */
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
void mmc_power_up(struct mmc_host *host)
{
	int bit;

	mmc_host_clk_hold(host);

	/* If ocr is set, we use it */
	if (host->ocr)
		bit = ffs(host->ocr) - 1;
	else
		bit = fls(host->ocr_avail) - 1;

	host->ios.vdd = bit;
<<<<<<< HEAD
<<<<<<< HEAD
	if (mmc_host_is_spi(host)) {
		host->ios.chip_select = MMC_CS_HIGH;
		host->ios.bus_mode = MMC_BUSMODE_PUSHPULL;
	} else {
=======
	if (mmc_host_is_spi(host)) 
		host->ios.chip_select = MMC_CS_HIGH;
	else {
>>>>>>> refs/remotes/origin/cm-10.0
		host->ios.chip_select = MMC_CS_DONTCARE;
		host->ios.bus_mode = MMC_BUSMODE_OPENDRAIN;
	}
=======
void mmc_power_up(struct mmc_host *host, u32 ocr)
{
	if (host->ios.power_mode == MMC_POWER_ON)
		return;

	mmc_host_clk_hold(host);

	host->ios.vdd = fls(ocr) - 1;
	if (mmc_host_is_spi(host))
=======
	if (mmc_host_is_spi(host)) 
>>>>>>> refs/remotes/origin/cm-11.0
		host->ios.chip_select = MMC_CS_HIGH;
	else {
		host->ios.chip_select = MMC_CS_DONTCARE;
<<<<<<< HEAD
	host->ios.bus_mode = MMC_BUSMODE_PUSHPULL;
>>>>>>> refs/remotes/origin/master
=======
		host->ios.bus_mode = MMC_BUSMODE_OPENDRAIN;
	}
>>>>>>> refs/remotes/origin/cm-11.0
	host->ios.power_mode = MMC_POWER_UP;
	host->ios.bus_width = MMC_BUS_WIDTH_1;
	host->ios.timing = MMC_TIMING_LEGACY;
	mmc_set_ios(host);

<<<<<<< HEAD
=======
	/* Set signal voltage to 3.3V */
	__mmc_set_signal_voltage(host, MMC_SIGNAL_VOLTAGE_330);

>>>>>>> refs/remotes/origin/master
	/*
	 * This delay should be sufficient to allow the power supply
	 * to reach the minimum voltage.
	 */
	mmc_delay(10);

	host->ios.clock = host->f_init;

	host->ios.power_mode = MMC_POWER_ON;
	mmc_set_ios(host);

	/*
	 * This delay must be at least 74 clock sizes, or 1 ms, or the
	 * time required to reach a stable voltage.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
#if defined(CONFIG_MACH_JENA)
	mmc_delay(30);
#else
	mmc_delay(10);
#endif
=======
	mmc_delay(10);

>>>>>>> refs/remotes/origin/cm-10.0
=======
	mmc_delay(10);

>>>>>>> refs/remotes/origin/master
	mmc_host_clk_release(host);
}

void mmc_power_off(struct mmc_host *host)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (host->ios.power_mode == MMC_POWER_OFF)
		return;

>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	mmc_host_clk_hold(host);

	host->ios.clock = 0;
	host->ios.vdd = 0;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
	
>>>>>>> refs/remotes/origin/cm-10.0
=======
	
>>>>>>> refs/remotes/origin/cm-11.0

	/*
	 * Reset ocr mask to be the highest possible voltage supported for
	 * this mmc host. This value will be used at next power up.
	 */
	host->ocr = 1 << (fls(host->ocr_avail) - 1);
=======
>>>>>>> refs/remotes/origin/master

	if (!mmc_host_is_spi(host)) {
		host->ios.bus_mode = MMC_BUSMODE_OPENDRAIN;
		host->ios.chip_select = MMC_CS_DONTCARE;
	}
	host->ios.power_mode = MMC_POWER_OFF;
	host->ios.bus_width = MMC_BUS_WIDTH_1;
	host->ios.timing = MMC_TIMING_LEGACY;
	mmc_set_ios(host);

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * Some configurations, such as the 802.11 SDIO card in the OLPC
	 * XO-1.5, require a short delay after poweroff before the card
	 * can be successfully turned on again.
	 */
	mmc_delay(1);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	mmc_host_clk_release(host);
}

=======
	mmc_host_clk_release(host);
}

void mmc_power_cycle(struct mmc_host *host, u32 ocr)
{
	mmc_power_off(host);
	/* Wait at least 1 ms according to SD spec */
	mmc_delay(1);
	mmc_power_up(host, ocr);
}

>>>>>>> refs/remotes/origin/master
/*
 * Cleanup when the last reference to the bus operator is dropped.
 */
static void __mmc_release_bus(struct mmc_host *host)
{
	BUG_ON(!host);
	BUG_ON(host->bus_refs);
	BUG_ON(!host->bus_dead);

	host->bus_ops = NULL;
}

/*
 * Increase reference count of bus operator
 */
static inline void mmc_bus_get(struct mmc_host *host)
{
	unsigned long flags;

	spin_lock_irqsave(&host->lock, flags);
	host->bus_refs++;
	spin_unlock_irqrestore(&host->lock, flags);
}

/*
 * Decrease reference count of bus operator and free it if
 * it is the last reference.
 */
static inline void mmc_bus_put(struct mmc_host *host)
{
	unsigned long flags;

	spin_lock_irqsave(&host->lock, flags);
	host->bus_refs--;
	if ((host->bus_refs == 0) && host->bus_ops)
		__mmc_release_bus(host);
	spin_unlock_irqrestore(&host->lock, flags);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
int mmc_resume_bus(struct mmc_host *host)
{
	unsigned long flags;

	if (!mmc_bus_needs_resume(host))
		return -EINVAL;

	printk("%s: Starting deferred resume\n", mmc_hostname(host));
	spin_lock_irqsave(&host->lock, flags);
	host->bus_resume_flags &= ~MMC_BUSRESUME_NEEDS_RESUME;
	host->rescan_disable = 0;
	spin_unlock_irqrestore(&host->lock, flags);

	mmc_bus_get(host);
	if (host->bus_ops && !host->bus_dead) {
		mmc_power_up(host);
		BUG_ON(!host->bus_ops->resume);
		host->bus_ops->resume(host);
	}

	if (host->bus_ops->detect && !host->bus_dead)
		host->bus_ops->detect(host);

	mmc_bus_put(host);
	printk("%s: Deferred resume completed\n", mmc_hostname(host));
	return 0;
}

EXPORT_SYMBOL(mmc_resume_bus);

<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
/*
 * Assign a mmc bus handler to a host. Only one bus handler may control a
 * host at any given time.
 */
void mmc_attach_bus(struct mmc_host *host, const struct mmc_bus_ops *ops)
{
	unsigned long flags;

	BUG_ON(!host);
	BUG_ON(!ops);

	WARN_ON(!host->claimed);

	spin_lock_irqsave(&host->lock, flags);

	BUG_ON(host->bus_ops);
	BUG_ON(host->bus_refs);

	host->bus_ops = ops;
	host->bus_refs = 1;
	host->bus_dead = 0;

	spin_unlock_irqrestore(&host->lock, flags);
}

/*
 * Remove the current bus handler from a host.
 */
void mmc_detach_bus(struct mmc_host *host)
{
	unsigned long flags;

	BUG_ON(!host);

	WARN_ON(!host->claimed);
	WARN_ON(!host->bus_ops);

	spin_lock_irqsave(&host->lock, flags);

	host->bus_dead = 1;

	spin_unlock_irqrestore(&host->lock, flags);

	mmc_bus_put(host);
}

<<<<<<< HEAD
=======
static void _mmc_detect_change(struct mmc_host *host, unsigned long delay,
				bool cd_irq)
{
#ifdef CONFIG_MMC_DEBUG
	unsigned long flags;
	spin_lock_irqsave(&host->lock, flags);
	WARN_ON(host->removed);
	spin_unlock_irqrestore(&host->lock, flags);
#endif

	/*
	 * If the device is configured as wakeup, we prevent a new sleep for
	 * 5 s to give provision for user space to consume the event.
	 */
	if (cd_irq && !(host->caps & MMC_CAP_NEEDS_POLL) &&
		device_can_wakeup(mmc_dev(host)))
		pm_wakeup_event(mmc_dev(host), 5000);

	host->detect_change = 1;
	mmc_schedule_delayed_work(&host->detect, delay);
}

>>>>>>> refs/remotes/origin/master
/**
 *	mmc_detect_change - process change of state on a MMC socket
 *	@host: host which changed state.
 *	@delay: optional delay to wait before detection (jiffies)
 *
 *	MMC drivers should call this when they detect a card has been
 *	inserted or removed. The MMC layer will confirm that any
 *	present card is still functional, and initialize any newly
 *	inserted.
 */
void mmc_detect_change(struct mmc_host *host, unsigned long delay)
{
<<<<<<< HEAD
#ifdef CONFIG_MMC_DEBUG
	unsigned long flags;
	spin_lock_irqsave(&host->lock, flags);
	WARN_ON(host->removed);
	spin_unlock_irqrestore(&host->lock, flags);
#endif
<<<<<<< HEAD

	wake_lock(&host->detect_wake_lock);
	host->detect_change = 1;
=======
	host->detect_change = 1;

	wake_lock(&host->detect_wake_lock);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
	mmc_schedule_delayed_work(&host->detect, delay);
}

=======
	_mmc_detect_change(host, delay, true);
}
>>>>>>> refs/remotes/origin/master
EXPORT_SYMBOL(mmc_detect_change);

void mmc_init_erase(struct mmc_card *card)
{
	unsigned int sz;

	if (is_power_of_2(card->erase_size))
		card->erase_shift = ffs(card->erase_size) - 1;
	else
		card->erase_shift = 0;

	/*
	 * It is possible to erase an arbitrarily large area of an SD or MMC
	 * card.  That is not desirable because it can take a long time
	 * (minutes) potentially delaying more important I/O, and also the
	 * timeout calculations become increasingly hugely over-estimated.
	 * Consequently, 'pref_erase' is defined as a guide to limit erases
	 * to that size and alignment.
	 *
	 * For SD cards that define Allocation Unit size, limit erases to one
	 * Allocation Unit at a time.  For MMC cards that define High Capacity
	 * Erase Size, whether it is switched on or not, limit to that size.
	 * Otherwise just have a stab at a good value.  For modern cards it
	 * will end up being 4MiB.  Note that if the value is too small, it
	 * can end up taking longer to erase.
	 */
	if (mmc_card_sd(card) && card->ssr.au) {
		card->pref_erase = card->ssr.au;
		card->erase_shift = ffs(card->ssr.au) - 1;
	} else if (card->ext_csd.hc_erase_size) {
		card->pref_erase = card->ext_csd.hc_erase_size;
	} else {
		sz = (card->csd.capacity << (card->csd.read_blkbits - 9)) >> 11;
		if (sz < 128)
			card->pref_erase = 512 * 1024 / 512;
		else if (sz < 512)
			card->pref_erase = 1024 * 1024 / 512;
		else if (sz < 1024)
			card->pref_erase = 2 * 1024 * 1024 / 512;
		else
			card->pref_erase = 4 * 1024 * 1024 / 512;
		if (card->pref_erase < card->erase_size)
			card->pref_erase = card->erase_size;
		else {
			sz = card->pref_erase % card->erase_size;
			if (sz)
				card->pref_erase += card->erase_size - sz;
		}
	}
}

static unsigned int mmc_mmc_erase_timeout(struct mmc_card *card,
				          unsigned int arg, unsigned int qty)
{
	unsigned int erase_timeout;

<<<<<<< HEAD
<<<<<<< HEAD
	if (card->ext_csd.erase_group_def & 1) {
=======
=======
>>>>>>> refs/remotes/origin/master
	if (arg == MMC_DISCARD_ARG ||
	    (arg == MMC_TRIM_ARG && card->ext_csd.rev >= 6)) {
		erase_timeout = card->ext_csd.trim_timeout;
	} else if (card->ext_csd.erase_group_def & 1) {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		/* High Capacity Erase Group Size uses HC timeouts */
		if (arg == MMC_TRIM_ARG)
			erase_timeout = card->ext_csd.trim_timeout;
		else
			erase_timeout = card->ext_csd.hc_erase_timeout;
	} else {
		/* CSD Erase Group Size uses write timeout */
		unsigned int mult = (10 << card->csd.r2w_factor);
		unsigned int timeout_clks = card->csd.tacc_clks * mult;
		unsigned int timeout_us;

		/* Avoid overflow: e.g. tacc_ns=80000000 mult=1280 */
		if (card->csd.tacc_ns < 1000000)
			timeout_us = (card->csd.tacc_ns * mult) / 1000;
		else
			timeout_us = (card->csd.tacc_ns / 1000) * mult;

		/*
		 * ios.clock is only a target.  The real clock rate might be
		 * less but not that much less, so fudge it by multiplying by 2.
		 */
		timeout_clks <<= 1;
		timeout_us += (timeout_clks * 1000) /
			      (mmc_host_clk_rate(card->host) / 1000);

		erase_timeout = timeout_us / 1000;

		/*
		 * Theoretically, the calculation could underflow so round up
		 * to 1ms in that case.
		 */
		if (!erase_timeout)
			erase_timeout = 1;
	}

	/* Multiplier for secure operations */
	if (arg & MMC_SECURE_ARGS) {
		if (arg == MMC_SECURE_ERASE_ARG)
			erase_timeout *= card->ext_csd.sec_erase_mult;
		else
			erase_timeout *= card->ext_csd.sec_trim_mult;
	}

	erase_timeout *= qty;

	/*
	 * Ensure at least a 1 second timeout for SPI as per
	 * 'mmc_set_data_timeout()'
	 */
	if (mmc_host_is_spi(card->host) && erase_timeout < 1000)
		erase_timeout = 1000;

	return erase_timeout;
}

static unsigned int mmc_sd_erase_timeout(struct mmc_card *card,
					 unsigned int arg,
					 unsigned int qty)
{
	unsigned int erase_timeout;

	if (card->ssr.erase_timeout) {
		/* Erase timeout specified in SD Status Register (SSR) */
		erase_timeout = card->ssr.erase_timeout * qty +
				card->ssr.erase_offset;
	} else {
		/*
		 * Erase timeout not specified in SD Status Register (SSR) so
		 * use 250ms per write block.
		 */
		erase_timeout = 250 * qty;
	}

	/* Must not be less than 1 second */
	if (erase_timeout < 1000)
		erase_timeout = 1000;

	return erase_timeout;
}

static unsigned int mmc_erase_timeout(struct mmc_card *card,
				      unsigned int arg,
				      unsigned int qty)
{
	if (mmc_card_sd(card))
		return mmc_sd_erase_timeout(card, arg, qty);
	else
		return mmc_mmc_erase_timeout(card, arg, qty);
}

static int mmc_do_erase(struct mmc_card *card, unsigned int from,
			unsigned int to, unsigned int arg)
{
	struct mmc_command cmd = {0};
	unsigned int qty = 0;
<<<<<<< HEAD
=======
	unsigned long timeout;
>>>>>>> refs/remotes/origin/master
	int err;

	/*
	 * qty is used to calculate the erase timeout which depends on how many
	 * erase groups (or allocation units in SD terminology) are affected.
	 * We count erasing part of an erase group as one erase group.
	 * For SD, the allocation units are always a power of 2.  For MMC, the
	 * erase group size is almost certainly also power of 2, but it does not
	 * seem to insist on that in the JEDEC standard, so we fall back to
	 * division in that case.  SD may not specify an allocation unit size,
	 * in which case the timeout is based on the number of write blocks.
	 *
	 * Note that the timeout for secure trim 2 will only be correct if the
	 * number of erase groups specified is the same as the total of all
	 * preceding secure trim 1 commands.  Since the power may have been
	 * lost since the secure trim 1 commands occurred, it is generally
	 * impossible to calculate the secure trim 2 timeout correctly.
	 */
	if (card->erase_shift)
		qty += ((to >> card->erase_shift) -
			(from >> card->erase_shift)) + 1;
	else if (mmc_card_sd(card))
		qty += to - from + 1;
	else
		qty += ((to / card->erase_size) -
			(from / card->erase_size)) + 1;

	if (!mmc_card_blockaddr(card)) {
		from <<= 9;
		to <<= 9;
	}

	if (mmc_card_sd(card))
		cmd.opcode = SD_ERASE_WR_BLK_START;
	else
		cmd.opcode = MMC_ERASE_GROUP_START;
	cmd.arg = from;
	cmd.flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_AC;
	err = mmc_wait_for_cmd(card->host, &cmd, 0);
	if (err) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "mmc_erase: group start error %d, "
		       "status %#x\n", err, cmd.resp[0]);
		err = -EINVAL;
=======
		pr_err("mmc_erase: group start error %d, "
		       "status %#x\n", err, cmd.resp[0]);
		err = -EIO;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_err("mmc_erase: group start error %d, "
		       "status %#x\n", err, cmd.resp[0]);
		err = -EIO;
>>>>>>> refs/remotes/origin/master
		goto out;
	}

	memset(&cmd, 0, sizeof(struct mmc_command));
	if (mmc_card_sd(card))
		cmd.opcode = SD_ERASE_WR_BLK_END;
	else
		cmd.opcode = MMC_ERASE_GROUP_END;
	cmd.arg = to;
	cmd.flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_AC;
	err = mmc_wait_for_cmd(card->host, &cmd, 0);
	if (err) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "mmc_erase: group end error %d, status %#x\n",
		       err, cmd.resp[0]);
		err = -EINVAL;
=======
		pr_err("mmc_erase: group end error %d, status %#x\n",
		       err, cmd.resp[0]);
		err = -EIO;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_err("mmc_erase: group end error %d, status %#x\n",
		       err, cmd.resp[0]);
		err = -EIO;
>>>>>>> refs/remotes/origin/master
		goto out;
	}

	memset(&cmd, 0, sizeof(struct mmc_command));
	cmd.opcode = MMC_ERASE;
	cmd.arg = arg;
	cmd.flags = MMC_RSP_SPI_R1B | MMC_RSP_R1B | MMC_CMD_AC;
	cmd.cmd_timeout_ms = mmc_erase_timeout(card, arg, qty);
	err = mmc_wait_for_cmd(card->host, &cmd, 0);
	if (err) {
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "mmc_erase: erase error %d, status %#x\n",
=======
		pr_err("mmc_erase: erase error %d, status %#x\n",
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_err("mmc_erase: erase error %d, status %#x\n",
>>>>>>> refs/remotes/origin/master
		       err, cmd.resp[0]);
		err = -EIO;
		goto out;
	}

	if (mmc_host_is_spi(card->host))
		goto out;

<<<<<<< HEAD
=======
	timeout = jiffies + msecs_to_jiffies(MMC_CORE_TIMEOUT_MS);
>>>>>>> refs/remotes/origin/master
	do {
		memset(&cmd, 0, sizeof(struct mmc_command));
		cmd.opcode = MMC_SEND_STATUS;
		cmd.arg = card->rca << 16;
		cmd.flags = MMC_RSP_R1 | MMC_CMD_AC;
		/* Do not retry else we can't see errors */
		err = mmc_wait_for_cmd(card->host, &cmd, 0);
		if (err || (cmd.resp[0] & 0xFDF92000)) {
<<<<<<< HEAD
<<<<<<< HEAD
			printk(KERN_ERR "error %d requesting status %#x\n",
=======
			pr_err("error %d requesting status %#x\n",
>>>>>>> refs/remotes/origin/cm-10.0
=======
			pr_err("error %d requesting status %#x\n",
>>>>>>> refs/remotes/origin/master
				err, cmd.resp[0]);
			err = -EIO;
			goto out;
		}
<<<<<<< HEAD
	} while (!(cmd.resp[0] & R1_READY_FOR_DATA) ||
<<<<<<< HEAD
		 R1_CURRENT_STATE(cmd.resp[0]) == 7);
=======
		 R1_CURRENT_STATE(cmd.resp[0]) == R1_STATE_PRG);
>>>>>>> refs/remotes/origin/cm-10.0
=======

		/* Timeout if the device never becomes ready for data and
		 * never leaves the program state.
		 */
		if (time_after(jiffies, timeout)) {
			pr_err("%s: Card stuck in programming state! %s\n",
				mmc_hostname(card->host), __func__);
			err =  -EIO;
			goto out;
		}

	} while (!(cmd.resp[0] & R1_READY_FOR_DATA) ||
		 (R1_CURRENT_STATE(cmd.resp[0]) == R1_STATE_PRG));
>>>>>>> refs/remotes/origin/master
out:
	return err;
}

/**
 * mmc_erase - erase sectors.
 * @card: card to erase
 * @from: first sector to erase
 * @nr: number of sectors to erase
 * @arg: erase command argument (SD supports only %MMC_ERASE_ARG)
 *
 * Caller must claim host before calling this function.
 */
int mmc_erase(struct mmc_card *card, unsigned int from, unsigned int nr,
	      unsigned int arg)
{
	unsigned int rem, to = from + nr;

	if (!(card->host->caps & MMC_CAP_ERASE) ||
	    !(card->csd.cmdclass & CCC_ERASE))
		return -EOPNOTSUPP;

	if (!card->erase_size)
		return -EOPNOTSUPP;

	if (mmc_card_sd(card) && arg != MMC_ERASE_ARG)
		return -EOPNOTSUPP;

	if ((arg & MMC_SECURE_ARGS) &&
	    !(card->ext_csd.sec_feature_support & EXT_CSD_SEC_ER_EN))
		return -EOPNOTSUPP;

	if ((arg & MMC_TRIM_ARGS) &&
	    !(card->ext_csd.sec_feature_support & EXT_CSD_SEC_GB_CL_EN))
		return -EOPNOTSUPP;

	if (arg == MMC_SECURE_ERASE_ARG) {
		if (from % card->erase_size || nr % card->erase_size)
			return -EINVAL;
	}

	if (arg == MMC_ERASE_ARG) {
		rem = from % card->erase_size;
		if (rem) {
			rem = card->erase_size - rem;
			from += rem;
			if (nr > rem)
				nr -= rem;
			else
				return 0;
		}
		rem = nr % card->erase_size;
		if (rem)
			nr -= rem;
	}

	if (nr == 0)
		return 0;

	to = from + nr;

	if (to <= from)
		return -EINVAL;

	/* 'from' and 'to' are inclusive */
	to -= 1;

	return mmc_do_erase(card, from, to, arg);
}
EXPORT_SYMBOL(mmc_erase);

int mmc_can_erase(struct mmc_card *card)
{
	if ((card->host->caps & MMC_CAP_ERASE) &&
	    (card->csd.cmdclass & CCC_ERASE) && card->erase_size)
		return 1;
	return 0;
}
EXPORT_SYMBOL(mmc_can_erase);

int mmc_can_trim(struct mmc_card *card)
{
	if (card->ext_csd.sec_feature_support & EXT_CSD_SEC_GB_CL_EN)
		return 1;
	return 0;
}
EXPORT_SYMBOL(mmc_can_trim);

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
int mmc_can_discard(struct mmc_card *card)
{
	/*
	 * As there's no way to detect the discard support bit at v4.5
	 * use the s/w feature support filed.
	 */
	if (card->ext_csd.feature_support & MMC_DISCARD_FEATURE)
		return 1;
	return 0;
}
EXPORT_SYMBOL(mmc_can_discard);

int mmc_can_sanitize(struct mmc_card *card)
{
	if (!mmc_can_trim(card) && !mmc_can_erase(card))
		return 0;
	if (card->ext_csd.sec_feature_support & EXT_CSD_SEC_SANITIZE)
		return 1;
	return 0;
}
EXPORT_SYMBOL(mmc_can_sanitize);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
int mmc_can_secure_erase_trim(struct mmc_card *card)
{
	if (card->ext_csd.sec_feature_support & EXT_CSD_SEC_ER_EN)
		return 1;
	return 0;
}
EXPORT_SYMBOL(mmc_can_secure_erase_trim);

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
int mmc_can_poweroff_notify(const struct mmc_card *card)
{
	return card &&
		mmc_card_mmc(card) &&
		card->host->bus_ops->poweroff_notify &&
		(card->poweroff_notify_state == MMC_POWERED_ON);
}
EXPORT_SYMBOL(mmc_can_poweroff_notify);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
int mmc_erase_group_aligned(struct mmc_card *card, unsigned int from,
			    unsigned int nr)
{
	if (!card->erase_size)
		return 0;
	if (from % card->erase_size || nr % card->erase_size)
		return 0;
	return 1;
}
EXPORT_SYMBOL(mmc_erase_group_aligned);

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static unsigned int mmc_do_calc_max_discard(struct mmc_card *card,
					    unsigned int arg)
{
	struct mmc_host *host = card->host;
	unsigned int max_discard, x, y, qty = 0, max_qty, timeout;
	unsigned int last_timeout = 0;

	if (card->erase_shift)
		max_qty = UINT_MAX >> card->erase_shift;
	else if (mmc_card_sd(card))
		max_qty = UINT_MAX;
	else
		max_qty = UINT_MAX / card->erase_size;

	/* Find the largest qty with an OK timeout */
	do {
		y = 0;
		for (x = 1; x && x <= max_qty && max_qty - x >= qty; x <<= 1) {
			timeout = mmc_erase_timeout(card, arg, qty + x);
			if (timeout > host->max_discard_to)
				break;
			if (timeout < last_timeout)
				break;
			last_timeout = timeout;
			y = x;
		}
		qty += y;
	} while (y);

	if (!qty)
		return 0;

	if (qty == 1)
		return 1;

	/* Convert qty to sectors */
	if (card->erase_shift)
		max_discard = --qty << card->erase_shift;
	else if (mmc_card_sd(card))
		max_discard = qty;
	else
		max_discard = --qty * card->erase_size;

	return max_discard;
}

unsigned int mmc_calc_max_discard(struct mmc_card *card)
{
	struct mmc_host *host = card->host;
	unsigned int max_discard, max_trim;

	if (!host->max_discard_to)
		return UINT_MAX;

	/*
	 * Without erase_group_def set, MMC erase timeout depends on clock
	 * frequence which can change.  In that case, the best choice is
	 * just the preferred erase size.
	 */
	if (mmc_card_mmc(card) && !(card->ext_csd.erase_group_def & 1))
		return card->pref_erase;

	max_discard = mmc_do_calc_max_discard(card, MMC_ERASE_ARG);
	if (mmc_can_trim(card)) {
		max_trim = mmc_do_calc_max_discard(card, MMC_TRIM_ARG);
		if (max_trim < max_discard)
			max_discard = max_trim;
	} else if (max_discard < card->erase_size) {
		max_discard = 0;
	}
	pr_debug("%s: calculated max. discard sectors %u for timeout %u ms\n",
		 mmc_hostname(host), max_discard, host->max_discard_to);
	return max_discard;
}
EXPORT_SYMBOL(mmc_calc_max_discard);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
int mmc_set_blocklen(struct mmc_card *card, unsigned int blocklen)
{
	struct mmc_command cmd = {0};

	if (mmc_card_blockaddr(card) || mmc_card_ddr_mode(card))
		return 0;

	cmd.opcode = MMC_SET_BLOCKLEN;
	cmd.arg = blocklen;
	cmd.flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_AC;
	return mmc_wait_for_cmd(card->host, &cmd, 5);
}
EXPORT_SYMBOL(mmc_set_blocklen);

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
int mmc_set_blockcount(struct mmc_card *card, unsigned int blockcount,
			bool is_rel_write)
{
	struct mmc_command cmd = {0};

	cmd.opcode = MMC_SET_BLOCK_COUNT;
	cmd.arg = blockcount & 0x0000FFFF;
	if (is_rel_write)
		cmd.arg |= 1 << 31;
	cmd.flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_AC;
	return mmc_wait_for_cmd(card->host, &cmd, 5);
}
EXPORT_SYMBOL(mmc_set_blockcount);

>>>>>>> refs/remotes/origin/master
static void mmc_hw_reset_for_init(struct mmc_host *host)
{
	if (!(host->caps & MMC_CAP_HW_RESET) || !host->ops->hw_reset)
		return;
	mmc_host_clk_hold(host);
	host->ops->hw_reset(host);
	mmc_host_clk_release(host);
}

int mmc_can_reset(struct mmc_card *card)
{
	u8 rst_n_function;

	if (!mmc_card_mmc(card))
		return 0;
	rst_n_function = card->ext_csd.rst_n_function;
	if ((rst_n_function & EXT_CSD_RST_N_EN_MASK) != EXT_CSD_RST_N_ENABLED)
		return 0;
	return 1;
}
EXPORT_SYMBOL(mmc_can_reset);

static int mmc_do_hw_reset(struct mmc_host *host, int check)
{
	struct mmc_card *card = host->card;

	if (!host->bus_ops->power_restore)
		return -EOPNOTSUPP;

	if (!(host->caps & MMC_CAP_HW_RESET) || !host->ops->hw_reset)
		return -EOPNOTSUPP;

	if (!card)
		return -EINVAL;

	if (!mmc_can_reset(card))
		return -EOPNOTSUPP;

	mmc_host_clk_hold(host);
	mmc_set_clock(host, host->f_init);

	host->ops->hw_reset(host);

	/* If the reset has happened, then a status command will fail */
	if (check) {
		struct mmc_command cmd = {0};
		int err;

		cmd.opcode = MMC_SEND_STATUS;
		if (!mmc_host_is_spi(card->host))
			cmd.arg = card->rca << 16;
		cmd.flags = MMC_RSP_SPI_R2 | MMC_RSP_R1 | MMC_CMD_AC;
		err = mmc_wait_for_cmd(card->host, &cmd, 0);
		if (!err) {
			mmc_host_clk_release(host);
			return -ENOSYS;
		}
	}

	host->card->state &= ~(MMC_STATE_HIGHSPEED | MMC_STATE_HIGHSPEED_DDR);
	if (mmc_host_is_spi(host)) {
		host->ios.chip_select = MMC_CS_HIGH;
		host->ios.bus_mode = MMC_BUSMODE_PUSHPULL;
	} else {
		host->ios.chip_select = MMC_CS_DONTCARE;
		host->ios.bus_mode = MMC_BUSMODE_OPENDRAIN;
	}
	host->ios.bus_width = MMC_BUS_WIDTH_1;
	host->ios.timing = MMC_TIMING_LEGACY;
	mmc_set_ios(host);

	mmc_host_clk_release(host);

	return host->bus_ops->power_restore(host);
}

int mmc_hw_reset(struct mmc_host *host)
{
	return mmc_do_hw_reset(host, 0);
}
EXPORT_SYMBOL(mmc_hw_reset);

int mmc_hw_reset_check(struct mmc_host *host)
{
	return mmc_do_hw_reset(host, 1);
}
EXPORT_SYMBOL(mmc_hw_reset_check);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static int mmc_rescan_try_freq(struct mmc_host *host, unsigned freq)
{
	host->f_init = freq;

#ifdef CONFIG_MMC_DEBUG
	pr_info("%s: %s: trying to init card at %u Hz\n",
		mmc_hostname(host), __func__, host->f_init);
#endif
<<<<<<< HEAD
	mmc_power_up(host);

	/*
<<<<<<< HEAD
=======
=======
	mmc_power_up(host, host->ocr_avail);

	/*
>>>>>>> refs/remotes/origin/master
	 * Some eMMCs (with VCCQ always on) may not be reset after power up, so
	 * do a hardware reset if possible.
	 */
	mmc_hw_reset_for_init(host);

<<<<<<< HEAD
	/* Initialization should be done at 3.3 V I/O voltage. */
	mmc_set_signal_voltage(host, MMC_SIGNAL_VOLTAGE_330, 0);

	/*
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/*
>>>>>>> refs/remotes/origin/master
	 * sdio_reset sends CMD52 to reset card.  Since we do not know
	 * if the card is being re-initialized, just send it.  CMD52
	 * should be ignored by SD/eMMC cards.
	 */
	sdio_reset(host);
	mmc_go_idle(host);

	mmc_send_if_cond(host, host->ocr_avail);

	/* Order's important: probe SDIO, then SD, then MMC */
	if (!mmc_attach_sdio(host))
		return 0;
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0

	if (!host->ios.vdd)
		mmc_power_up(host);

<<<<<<< HEAD
	if (!mmc_attach_sd(host))
		return 0;

	if (!host->ios.vdd)
		mmc_power_up(host);

=======
	if (!mmc_attach_sd(host))
		return 0;
>>>>>>> refs/remotes/origin/master
=======
	if (!mmc_attach_sd(host))
		return 0;

	if (!host->ios.vdd)
		mmc_power_up(host);

>>>>>>> refs/remotes/origin/cm-11.0
	if (!mmc_attach_mmc(host))
		return 0;

	mmc_power_off(host);
	return -EIO;
}

int _mmc_detect_card_removed(struct mmc_host *host)
{
	int ret;

	if ((host->caps & MMC_CAP_NONREMOVABLE) || !host->bus_ops->alive)
		return 0;

	if (!host->card || mmc_card_removed(host->card))
		return 1;

	ret = host->bus_ops->alive(host);
<<<<<<< HEAD
=======

	/*
	 * Card detect status and alive check may be out of sync if card is
	 * removed slowly, when card detect switch changes while card/slot
	 * pads are still contacted in hardware (refer to "SD Card Mechanical
	 * Addendum, Appendix C: Card Detection Switch"). So reschedule a
	 * detect work 200ms later for this case.
	 */
	if (!ret && host->ops->get_cd && !host->ops->get_cd(host)) {
		mmc_detect_change(host, msecs_to_jiffies(200));
		pr_debug("%s: card removed too slowly\n", mmc_hostname(host));
	}

>>>>>>> refs/remotes/origin/master
	if (ret) {
		mmc_card_set_removed(host->card);
		pr_debug("%s: card remove detected\n", mmc_hostname(host));
	}

	return ret;
}

int mmc_detect_card_removed(struct mmc_host *host)
{
	struct mmc_card *card = host->card;
<<<<<<< HEAD
<<<<<<< HEAD

	WARN_ON(!host->claimed);
=======
=======
>>>>>>> refs/remotes/origin/master
	int ret;

	WARN_ON(!host->claimed);

	if (!card)
		return 1;

	ret = mmc_card_removed(card);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * The card will be considered unchanged unless we have been asked to
	 * detect a change or host requires polling to provide card detection.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (card && !host->detect_change && !(host->caps & MMC_CAP_NEEDS_POLL))
		return mmc_card_removed(card);

	host->detect_change = 0;

	return _mmc_detect_card_removed(host);
=======
	if (!host->detect_change && !(host->caps & MMC_CAP_NEEDS_POLL) &&
	    !(host->caps2 & MMC_CAP2_DETECT_ON_ERR))
=======
	if (!host->detect_change && !(host->caps & MMC_CAP_NEEDS_POLL))
>>>>>>> refs/remotes/origin/master
		return ret;

	host->detect_change = 0;
	if (!ret) {
		ret = _mmc_detect_card_removed(host);
<<<<<<< HEAD
		if (ret && (host->caps2 & MMC_CAP2_DETECT_ON_ERR)) {
=======
		if (ret && (host->caps & MMC_CAP_NEEDS_POLL)) {
>>>>>>> refs/remotes/origin/master
			/*
			 * Schedule a detect work as soon as possible to let a
			 * rescan handle the card removal.
			 */
			cancel_delayed_work(&host->detect);
<<<<<<< HEAD
			mmc_detect_change(host, 0);
=======
			_mmc_detect_change(host, 0, false);
>>>>>>> refs/remotes/origin/master
		}
	}

	return ret;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL(mmc_detect_card_removed);

void mmc_rescan(struct work_struct *work)
{
	struct mmc_host *host =
		container_of(work, struct mmc_host, detect.work);
<<<<<<< HEAD
<<<<<<< HEAD
	bool extend_wakelock = false;
=======
	int i;
>>>>>>> refs/remotes/origin/master
=======
	bool extend_wakelock = false;
>>>>>>> refs/remotes/origin/cm-11.0

	if (host->rescan_disable)
		return;

<<<<<<< HEAD
=======
	/* If there is a non-removable card registered, only scan once */
	if ((host->caps & MMC_CAP_NONREMOVABLE) && host->rescan_entered)
		return;
	host->rescan_entered = 1;

>>>>>>> refs/remotes/origin/master
	mmc_bus_get(host);

	/*
	 * if there is a _removable_ card registered, check whether it is
	 * still present
	 */
	if (host->bus_ops && host->bus_ops->detect && !host->bus_dead
	    && !(host->caps & MMC_CAP_NONREMOVABLE))
		host->bus_ops->detect(host);

<<<<<<< HEAD
<<<<<<< HEAD
=======
	host->detect_change = 0;
>>>>>>> refs/remotes/origin/cm-10.0
	/* If the card was removed the bus will be marked
	 * as dead - extend the wakelock so userspace
	 * can respond */
	if (host->bus_dead)
		extend_wakelock = 1;

<<<<<<< HEAD
	host->detect_change = 0;
<<<<<<< HEAD
=======
=======
	/* If the card was removed the bus will be marked
	 * as dead - extend the wakelock so userspace
	 * can respond */
	if (host->bus_dead)
		extend_wakelock = 1;

>>>>>>> refs/remotes/origin/cm-11.0

	/* If the card was removed the bus will be marked
	 * as dead - extend the wakelock so userspace
	 * can respond */
	if (host->bus_dead)
		extend_wakelock = 1;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
	host->detect_change = 0;
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

	/*
	 * Let mmc_bus_put() free the bus/bus_ops if we've found that
	 * the card is no longer present.
	 */
	mmc_bus_put(host);
	mmc_bus_get(host);

	/* if there still is a card present, stop here */
	if (host->bus_ops != NULL) {
		mmc_bus_put(host);
		goto out;
	}

	/*
	 * Only we can add a new handler, so it's safe to
	 * release the lock here.
	 */
	mmc_bus_put(host);

<<<<<<< HEAD
	if (host->ops->get_cd && host->ops->get_cd(host) == 0)
		goto out;

	mmc_claim_host(host);
	if (!mmc_rescan_try_freq(host, host->f_min))
		extend_wakelock = true;
<<<<<<< HEAD
	mmc_release_host(host);

 out:
	if (extend_wakelock)
		wake_lock_timeout(&host->detect_wake_lock, HZ / 2);
	else
		wake_unlock(&host->detect_wake_lock);
	if (host->caps & MMC_CAP_NEEDS_POLL) {
		wake_lock(&host->detect_wake_lock);
		mmc_schedule_delayed_work(&host->detect, HZ);
	}
=======
	if (host->ops->get_cd && host->ops->get_cd(host) == 0) {
		mmc_claim_host(host);
		mmc_power_off(host);
		mmc_release_host(host);
		goto out;
	}

	mmc_claim_host(host);
	for (i = 0; i < ARRAY_SIZE(freqs); i++) {
		if (!mmc_rescan_try_freq(host, max(freqs[i], host->f_min)))
			break;
		if (freqs[i] <= host->f_min)
			break;
	}
=======
>>>>>>> refs/remotes/origin/cm-11.0
	mmc_release_host(host);

 out:
	if (extend_wakelock)
		wake_lock_timeout(&host->detect_wake_lock, HZ / 2);
	else
		wake_unlock(&host->detect_wake_lock);
	if (host->caps & MMC_CAP_NEEDS_POLL) {
		wake_lock(&host->detect_wake_lock);
		mmc_schedule_delayed_work(&host->detect, HZ);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======
	}
>>>>>>> refs/remotes/origin/cm-11.0
}

void mmc_start_host(struct mmc_host *host)
{
<<<<<<< HEAD
	mmc_power_off(host);
	mmc_detect_change(host, 0);
=======
	host->f_init = max(freqs[0], host->f_min);
	host->rescan_disable = 0;
	if (host->caps2 & MMC_CAP2_NO_PRESCAN_POWERUP)
		mmc_power_off(host);
	else
		mmc_power_up(host, host->ocr_avail);
	_mmc_detect_change(host, 0, false);
>>>>>>> refs/remotes/origin/master
}

void mmc_stop_host(struct mmc_host *host)
{
#ifdef CONFIG_MMC_DEBUG
	unsigned long flags;
	spin_lock_irqsave(&host->lock, flags);
	host->removed = 1;
	spin_unlock_irqrestore(&host->lock, flags);
#endif

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	if (host->caps & MMC_CAP_DISABLE)
		cancel_delayed_work(&host->disable);
=======
>>>>>>> refs/remotes/origin/cm-10.0
	if (cancel_delayed_work_sync(&host->detect))
		wake_unlock(&host->detect_wake_lock);
=======
	host->rescan_disable = 1;
	cancel_delayed_work_sync(&host->detect);
>>>>>>> refs/remotes/origin/master
=======
	if (cancel_delayed_work_sync(&host->detect))
		wake_unlock(&host->detect_wake_lock);
>>>>>>> refs/remotes/origin/cm-11.0
	mmc_flush_scheduled_work();

	/* clear pm flags now and let card drivers set them as needed */
	host->pm_flags = 0;

	mmc_bus_get(host);
	if (host->bus_ops && !host->bus_dead) {
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
		mmc_claim_host(host);
		if (mmc_can_poweroff_notify(host->card)) {
			int err = host->bus_ops->poweroff_notify(host,
						MMC_PW_OFF_NOTIFY_LONG);
			if (err)
				pr_info("%s: error [%d] in poweroff notify\n",
					mmc_hostname(host), err);
		}
		mmc_release_host(host);
		/* Calling bus_ops->remove() with a claimed host can deadlock */
>>>>>>> refs/remotes/origin/cm-10.0
		if (host->bus_ops->remove)
			host->bus_ops->remove(host);

=======
		/* Calling bus_ops->remove() with a claimed host can deadlock */
		host->bus_ops->remove(host);
>>>>>>> refs/remotes/origin/master
		mmc_claim_host(host);
		mmc_detach_bus(host);
		mmc_power_off(host);
		mmc_release_host(host);
		mmc_bus_put(host);
		return;
	}
	mmc_bus_put(host);

	BUG_ON(host->card);

	mmc_power_off(host);
}

int mmc_power_save_host(struct mmc_host *host)
{
	int ret = 0;

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_MMC_DEBUG
	pr_info("%s: %s: powering down\n", mmc_hostname(host), __func__);
#endif

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	mmc_bus_get(host);

	if (!host->bus_ops || host->bus_dead || !host->bus_ops->power_restore) {
		mmc_bus_put(host);
		return -EINVAL;
	}

	if (host->bus_ops->power_save)
		ret = host->bus_ops->power_save(host);
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	mmc_claim_host(host);
	if (mmc_can_poweroff_notify(host->card)) {
		int err = host->bus_ops->poweroff_notify(host,
					MMC_PW_OFF_NOTIFY_SHORT);
		if (err)
			pr_info("%s: error [%d] in poweroff notify\n",
				mmc_hostname(host), err);
	}
	mmc_release_host(host);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

	mmc_bus_put(host);

	mmc_power_off(host);

	return ret;
}
EXPORT_SYMBOL(mmc_power_save_host);

int mmc_power_restore_host(struct mmc_host *host)
{
	int ret;

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_MMC_DEBUG
	pr_info("%s: %s: powering up\n", mmc_hostname(host), __func__);
#endif

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	mmc_bus_get(host);

	if (!host->bus_ops || host->bus_dead || !host->bus_ops->power_restore) {
		mmc_bus_put(host);
		return -EINVAL;
	}

<<<<<<< HEAD
	mmc_power_up(host);
=======
	mmc_power_up(host, host->card->ocr);
>>>>>>> refs/remotes/origin/master
	ret = host->bus_ops->power_restore(host);

	mmc_bus_put(host);

	return ret;
}
EXPORT_SYMBOL(mmc_power_restore_host);

<<<<<<< HEAD
int mmc_card_awake(struct mmc_host *host)
{
	int err = -ENOSYS;

<<<<<<< HEAD
	mmc_bus_get(host);

	if (host->bus_ops && !host->bus_dead && host->bus_ops->awake)
		err = host->bus_ops->awake(host);
=======
	if (host->caps2 & MMC_CAP2_NO_SLEEP_CMD)
		return 0;

	mmc_bus_get(host);

	if (host->bus_ops && !host->bus_dead && host->bus_ops->awake) {
		err = host->bus_ops->awake(host);
		if (!err)
			mmc_card_clr_sleep(host->card);
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0

	mmc_bus_put(host);

	return err;
}
EXPORT_SYMBOL(mmc_card_awake);

int mmc_card_sleep(struct mmc_host *host)
{
	int err = -ENOSYS;

<<<<<<< HEAD
	mmc_bus_get(host);

	if (host->bus_ops && !host->bus_dead && host->bus_ops->awake)
		err = host->bus_ops->sleep(host);
=======
	if (host->caps2 & MMC_CAP2_NO_SLEEP_CMD)
		return 0;

	mmc_bus_get(host);

	if (host->bus_ops && !host->bus_dead && host->bus_ops->sleep) {
		err = host->bus_ops->sleep(host);
		if (!err)
			mmc_card_set_sleep(host->card);
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0

	mmc_bus_put(host);

	return err;
}
EXPORT_SYMBOL(mmc_card_sleep);

int mmc_card_can_sleep(struct mmc_host *host)
{
	struct mmc_card *card = host->card;

	if (card && mmc_card_mmc(card) && card->ext_csd.rev >= 3)
		return 1;
	return 0;
}
EXPORT_SYMBOL(mmc_card_can_sleep);

<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
/*
 * Flush the cache to the non-volatile storage.
 */
int mmc_flush_cache(struct mmc_card *card)
{
	struct mmc_host *host = card->host;
	int err = 0;

	if (!(host->caps2 & MMC_CAP2_CACHE_CTRL))
		return err;

	if (mmc_card_mmc(card) &&
			(card->ext_csd.cache_size > 0) &&
			(card->ext_csd.cache_ctrl & 1)) {
		err = mmc_switch(card, EXT_CSD_CMD_SET_NORMAL,
				EXT_CSD_FLUSH_CACHE, 1, 0);
		if (err)
			pr_err("%s: cache flush error %d\n",
					mmc_hostname(card->host), err);
	}

	return err;
}
EXPORT_SYMBOL(mmc_flush_cache);

/*
 * Turn the cache ON/OFF.
 * Turning the cache OFF shall trigger flushing of the data
 * to the non-volatile storage.
<<<<<<< HEAD
=======
 * This function should be called with host claimed
>>>>>>> refs/remotes/origin/master
 */
int mmc_cache_ctrl(struct mmc_host *host, u8 enable)
{
	struct mmc_card *card = host->card;
	unsigned int timeout;
	int err = 0;

	if (!(host->caps2 & MMC_CAP2_CACHE_CTRL) ||
			mmc_card_is_removable(host))
		return err;

<<<<<<< HEAD
	mmc_claim_host(host);
=======
>>>>>>> refs/remotes/origin/master
	if (card && mmc_card_mmc(card) &&
			(card->ext_csd.cache_size > 0)) {
		enable = !!enable;

		if (card->ext_csd.cache_ctrl ^ enable) {
			timeout = enable ? card->ext_csd.generic_cmd6_time : 0;
			err = mmc_switch(card, EXT_CSD_CMD_SET_NORMAL,
					EXT_CSD_CACHE_CTRL, enable, timeout);
			if (err)
				pr_err("%s: cache %s error %d\n",
						mmc_hostname(card->host),
						enable ? "on" : "off",
						err);
			else
				card->ext_csd.cache_ctrl = enable;
		}
	}
<<<<<<< HEAD
	mmc_release_host(host);
=======
>>>>>>> refs/remotes/origin/master

	return err;
}
EXPORT_SYMBOL(mmc_cache_ctrl);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
#ifdef CONFIG_PM

/**
 *	mmc_suspend_host - suspend a host
 *	@host: mmc host
 */
int mmc_suspend_host(struct mmc_host *host)
{
	int err = 0;

	if (mmc_bus_needs_resume(host))
		return 0;
<<<<<<< HEAD

<<<<<<< HEAD
	if (host->caps & MMC_CAP_DISABLE)
		cancel_delayed_work(&host->disable);
	if (cancel_delayed_work(&host->detect))
		wake_unlock(&host->detect_wake_lock);
	mmc_flush_scheduled_work();
=======
>>>>>>> refs/remotes/origin/cm-11.0

	mmc_bus_get(host);
	if (host->bus_ops && !host->bus_dead) {

=======
	err = mmc_cache_ctrl(host, 0);
	if (err)
		goto out;

	mmc_bus_get(host);
	if (host->bus_ops && !host->bus_dead) {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
		/*
		 * A long response time is not acceptable for device drivers
		 * when doing suspend. Prevent mmc_claim_host in the suspend
		 * sequence, to potentially wait "forever" by trying to
		 * pre-claim the host.
		 *
		 * Skip try claim host for SDIO cards, doing so fixes deadlock
		 * conditions. The function driver suspend may again call into
		 * SDIO driver within a different context for enabling power
		 * save mode in the card and hence wait in mmc_claim_host
		 * causing deadlock.
		 */
		if (!(host->card && mmc_card_sdio(host->card)))
			if (!mmc_try_claim_host(host))
				err = -EBUSY;

		if (!err) {
<<<<<<< HEAD
<<<<<<< HEAD
			if (host->bus_ops->suspend)
				err = host->bus_ops->suspend(host);
			if (!(host->card && mmc_card_sdio(host->card)))
				mmc_do_release_host(host);
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
			if (host->bus_ops->suspend) {
				if (mmc_card_doing_bkops(host->card))
					mmc_interrupt_bkops(host->card);

				err = host->bus_ops->suspend(host);
			}
			if (!(host->card && mmc_card_sdio(host->card)))
				mmc_release_host(host);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0

			if (err == -ENOSYS || !host->bus_ops->resume) {
				/*
				 * We simply "remove" the card in this case.
<<<<<<< HEAD
<<<<<<< HEAD
=======
				 * It will be redetected on resume.  (Calling
				 * bus_ops->remove() with a claimed host can
				 * deadlock.)
>>>>>>> refs/remotes/origin/cm-10.0
=======
				 * It will be redetected on resume.  (Calling
				 * bus_ops->remove() with a claimed host can
				 * deadlock.)
>>>>>>> refs/remotes/origin/cm-11.0
				 * It will be redetected on resume.
				 */
				if (host->bus_ops->remove)
					host->bus_ops->remove(host);
				mmc_claim_host(host);
				mmc_detach_bus(host);
				mmc_power_off(host);
				mmc_release_host(host);
				host->pm_flags = 0;
				err = 0;
			}
		}
	}
	mmc_bus_put(host);

	if (!err && !mmc_card_keep_power(host))
		mmc_power_off(host);

<<<<<<< HEAD
=======
out:
>>>>>>> refs/remotes/origin/cm-10.0
	return err;
}

EXPORT_SYMBOL(mmc_suspend_host);

/**
 *	mmc_resume_host - resume a previously suspended host
 *	@host: mmc host
 */
int mmc_resume_host(struct mmc_host *host)
{
	int err = 0;

	mmc_bus_get(host);
	if (mmc_bus_manual_resume(host)) {
		host->bus_resume_flags |= MMC_BUSRESUME_NEEDS_RESUME;
		mmc_bus_put(host);
		return 0;
	}

	if (host->bus_ops && !host->bus_dead) {
		if (!mmc_card_keep_power(host)) {
			mmc_power_up(host);
			mmc_select_voltage(host, host->ocr);
			/*
			 * Tell runtime PM core we just powered up the card,
			 * since it still believes the card is powered off.
			 * Note that currently runtime PM is only enabled
			 * for SDIO cards that are MMC_CAP_POWER_OFF_CARD
			 */
			if (mmc_card_sdio(host->card) &&
			    (host->caps & MMC_CAP_POWER_OFF_CARD)) {
				pm_runtime_disable(&host->card->dev);
				pm_runtime_set_active(&host->card->dev);
				pm_runtime_enable(&host->card->dev);
			}
		}
		BUG_ON(!host->bus_ops->resume);
		err = host->bus_ops->resume(host);
		if (err) {
<<<<<<< HEAD
			printk(KERN_WARNING "%s: error %d during resume "
=======
			pr_warning("%s: error %d during resume "
>>>>>>> refs/remotes/origin/cm-10.0
					    "(card was removed?)\n",
					    mmc_hostname(host), err);
			err = 0;
		}
	}
	host->pm_flags &= ~MMC_PM_KEEP_POWER;
	mmc_bus_put(host);

	return err;
}
EXPORT_SYMBOL(mmc_resume_host);

=======
#ifdef CONFIG_PM

>>>>>>> refs/remotes/origin/master
/* Do the card removal on suspend if card is assumed removeable
 * Do that in pm notifier while userspace isn't yet frozen, so we will be able
   to sync the card.
*/
int mmc_pm_notify(struct notifier_block *notify_block,
					unsigned long mode, void *unused)
{
	struct mmc_host *host = container_of(
		notify_block, struct mmc_host, pm_notify);
	unsigned long flags;
<<<<<<< HEAD

=======
	int err = 0;
>>>>>>> refs/remotes/origin/master

	switch (mode) {
	case PM_HIBERNATION_PREPARE:
	case PM_SUSPEND_PREPARE:
<<<<<<< HEAD

		spin_lock_irqsave(&host->lock, flags);
		if (mmc_bus_needs_resume(host)) {
			spin_unlock_irqrestore(&host->lock, flags);
			break;
		}
		host->rescan_disable = 1;
		spin_unlock_irqrestore(&host->lock, flags);
		if (cancel_delayed_work_sync(&host->detect))
			wake_unlock(&host->detect_wake_lock);

		if (!host->bus_ops || host->bus_ops->suspend)
			break;
<<<<<<< HEAD
<<<<<<< HEAD

		mmc_claim_host(host);

		if (host->bus_ops->remove)
			host->bus_ops->remove(host);

=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
		mmc_claim_host(host);
		if (mmc_can_poweroff_notify(host->card)) {
			int err = host->bus_ops->poweroff_notify(host,
						MMC_PW_OFF_NOTIFY_SHORT);
			if (err)
				pr_info("%s: error [%d] in poweroff notify\n",
					mmc_hostname(host), err);
		}
		mmc_release_host(host);

		/* Calling bus_ops->remove() with a claimed host can deadlock */
		if (host->bus_ops->remove)
			host->bus_ops->remove(host);

		mmc_claim_host(host);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		spin_lock_irqsave(&host->lock, flags);
		host->rescan_disable = 1;
		spin_unlock_irqrestore(&host->lock, flags);
		cancel_delayed_work_sync(&host->detect);

		if (!host->bus_ops)
			break;

		/* Validate prerequisites for suspend */
		if (host->bus_ops->pre_suspend)
			err = host->bus_ops->pre_suspend(host);
		if (!err && host->bus_ops->suspend)
			break;

		/* Calling bus_ops->remove() with a claimed host can deadlock */
		host->bus_ops->remove(host);
		mmc_claim_host(host);
>>>>>>> refs/remotes/origin/master
		mmc_detach_bus(host);
		mmc_power_off(host);
		mmc_release_host(host);
		host->pm_flags = 0;
		break;

	case PM_POST_SUSPEND:
	case PM_POST_HIBERNATION:
	case PM_POST_RESTORE:

		spin_lock_irqsave(&host->lock, flags);
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
		if (mmc_bus_manual_resume(host)) {
			spin_unlock_irqrestore(&host->lock, flags);
			break;
		}
		host->rescan_disable = 0;
		spin_unlock_irqrestore(&host->lock, flags);
		mmc_detect_change(host, 0);
=======
		host->rescan_disable = 0;
		spin_unlock_irqrestore(&host->lock, flags);
		_mmc_detect_change(host, 0, false);
>>>>>>> refs/remotes/origin/master

	}

	return 0;
}
#endif

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
#ifdef CONFIG_MMC_EMBEDDED_SDIO
void mmc_set_embedded_sdio_data(struct mmc_host *host,
				struct sdio_cis *cis,
				struct sdio_cccr *cccr,
				struct sdio_embedded_func *funcs,
				int num_funcs)
{
	host->embedded_sdio_data.cis = cis;
	host->embedded_sdio_data.cccr = cccr;
	host->embedded_sdio_data.funcs = funcs;
	host->embedded_sdio_data.num_funcs = num_funcs;
}

EXPORT_SYMBOL(mmc_set_embedded_sdio_data);
#endif

<<<<<<< HEAD
=======
/**
 * mmc_init_context_info() - init synchronization context
 * @host: mmc host
 *
 * Init struct context_info needed to implement asynchronous
 * request mechanism, used by mmc core, host driver and mmc requests
 * supplier.
 */
void mmc_init_context_info(struct mmc_host *host)
{
	spin_lock_init(&host->context_info.lock);
	host->context_info.is_new_req = false;
	host->context_info.is_done_rcv = false;
	host->context_info.is_waiting_last_req = false;
	init_waitqueue_head(&host->context_info.wait);
}

>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
static int __init mmc_init(void)
{
	int ret;

	workqueue = alloc_ordered_workqueue("kmmcd", 0);
	if (!workqueue)
		return -ENOMEM;

	ret = mmc_register_bus();
	if (ret)
		goto destroy_workqueue;

	ret = mmc_register_host_class();
	if (ret)
		goto unregister_bus;

	ret = sdio_register_bus();
	if (ret)
		goto unregister_host_class;

	return 0;

unregister_host_class:
	mmc_unregister_host_class();
unregister_bus:
	mmc_unregister_bus();
destroy_workqueue:
	destroy_workqueue(workqueue);

	return ret;
}

static void __exit mmc_exit(void)
{
	sdio_unregister_bus();
	mmc_unregister_host_class();
	mmc_unregister_bus();
	destroy_workqueue(workqueue);
}

subsys_initcall(mmc_init);
module_exit(mmc_exit);

MODULE_LICENSE("GPL");
