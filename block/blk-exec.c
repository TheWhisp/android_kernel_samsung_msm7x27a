/*
 * Functions related to setting various queue properties from drivers
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/bio.h>
#include <linux/blkdev.h>
<<<<<<< HEAD
=======
#include <linux/blk-mq.h>
#include <linux/sched/sysctl.h>
>>>>>>> refs/remotes/origin/master

#include "blk.h"

/*
 * for max sense size
 */
#include <scsi/scsi_cmnd.h>

/**
 * blk_end_sync_rq - executes a completion event on a request
 * @rq: request to complete
 * @error: end I/O status of the request
 */
static void blk_end_sync_rq(struct request *rq, int error)
{
	struct completion *waiting = rq->end_io_data;

	rq->end_io_data = NULL;
<<<<<<< HEAD
	__blk_put_request(rq->q, rq);
=======
>>>>>>> refs/remotes/origin/master

	/*
	 * complete last, if this is a stack request the process (and thus
	 * the rq pointer) could be invalid right after this complete()
	 */
	complete(waiting);
}

/**
 * blk_execute_rq_nowait - insert a request into queue for execution
 * @q:		queue to insert the request in
 * @bd_disk:	matching gendisk
 * @rq:		request to insert
 * @at_head:    insert request at head or tail of queue
 * @done:	I/O completion handler
 *
 * Description:
 *    Insert a fully prepared request at the back of the I/O scheduler queue
 *    for execution.  Don't wait for completion.
<<<<<<< HEAD
=======
 *
 * Note:
 *    This function will invoke @done directly if the queue is dead.
>>>>>>> refs/remotes/origin/master
 */
void blk_execute_rq_nowait(struct request_queue *q, struct gendisk *bd_disk,
			   struct request *rq, int at_head,
			   rq_end_io_fn *done)
{
	int where = at_head ? ELEVATOR_INSERT_FRONT : ELEVATOR_INSERT_BACK;
<<<<<<< HEAD

<<<<<<< HEAD
	if (unlikely(test_bit(QUEUE_FLAG_DEAD, &q->queue_flags))) {
=======
	WARN_ON(irqs_disabled());
	spin_lock_irq(q->queue_lock);

	if (unlikely(blk_queue_dead(q))) {
		spin_unlock_irq(q->queue_lock);
>>>>>>> refs/remotes/origin/cm-10.0
		rq->errors = -ENXIO;
		if (rq->end_io)
			rq->end_io(rq, rq->errors);
		return;
	}

	rq->rq_disk = bd_disk;
	rq->end_io = done;
<<<<<<< HEAD
	WARN_ON(irqs_disabled());
	spin_lock_irq(q->queue_lock);
=======
>>>>>>> refs/remotes/origin/cm-10.0
	__elv_add_request(q, rq, where);
	__blk_run_queue(q);
	/* the queue is stopped so it won't be run */
	if (rq->cmd_type == REQ_TYPE_PM_RESUME)
		q->request_fn(q);
=======
	bool is_pm_resume;

	WARN_ON(irqs_disabled());

	rq->rq_disk = bd_disk;
	rq->end_io = done;

	if (q->mq_ops) {
		blk_mq_insert_request(q, rq, true);
		return;
	}

	/*
	 * need to check this before __blk_run_queue(), because rq can
	 * be freed before that returns.
	 */
	is_pm_resume = rq->cmd_type == REQ_TYPE_PM_RESUME;

	spin_lock_irq(q->queue_lock);

	if (unlikely(blk_queue_dying(q))) {
		rq->cmd_flags |= REQ_QUIET; 
		rq->errors = -ENXIO;
		__blk_end_request_all(rq, rq->errors);
		spin_unlock_irq(q->queue_lock);
		return;
	}

	__elv_add_request(q, rq, where);
	__blk_run_queue(q);
	/* the queue is stopped so it won't be run */
	if (is_pm_resume)
		__blk_run_queue_uncond(q);
>>>>>>> refs/remotes/origin/master
	spin_unlock_irq(q->queue_lock);
}
EXPORT_SYMBOL_GPL(blk_execute_rq_nowait);

/**
 * blk_execute_rq - insert a request into queue for execution
 * @q:		queue to insert the request in
 * @bd_disk:	matching gendisk
 * @rq:		request to insert
 * @at_head:    insert request at head or tail of queue
 *
 * Description:
 *    Insert a fully prepared request at the back of the I/O scheduler queue
 *    for execution and wait for completion.
 */
int blk_execute_rq(struct request_queue *q, struct gendisk *bd_disk,
		   struct request *rq, int at_head)
{
	DECLARE_COMPLETION_ONSTACK(wait);
	char sense[SCSI_SENSE_BUFFERSIZE];
	int err = 0;
	unsigned long hang_check;

<<<<<<< HEAD
	/*
	 * we need an extra reference to the request, so we can look at
	 * it after io completion
	 */
	rq->ref_count++;

=======
>>>>>>> refs/remotes/origin/master
	if (!rq->sense) {
		memset(sense, 0, sizeof(sense));
		rq->sense = sense;
		rq->sense_len = 0;
	}

	rq->end_io_data = &wait;
	blk_execute_rq_nowait(q, bd_disk, rq, at_head, blk_end_sync_rq);

	/* Prevent hang_check timer from firing at us during very long I/O */
	hang_check = sysctl_hung_task_timeout_secs;
	if (hang_check)
<<<<<<< HEAD
		while (!wait_for_completion_timeout(&wait, hang_check * (HZ/2)));
	else
		wait_for_completion(&wait);
=======
		while (!wait_for_completion_io_timeout(&wait, hang_check * (HZ/2)));
	else
		wait_for_completion_io(&wait);
>>>>>>> refs/remotes/origin/master

	if (rq->errors)
		err = -EIO;

	return err;
}
EXPORT_SYMBOL(blk_execute_rq);
