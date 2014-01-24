/*
   drbd_req.c

   This file is part of DRBD by Philipp Reisner and Lars Ellenberg.

   Copyright (C) 2001-2008, LINBIT Information Technologies GmbH.
   Copyright (C) 1999-2008, Philipp Reisner <philipp.reisner@linbit.com>.
   Copyright (C) 2002-2008, Lars Ellenberg <lars.ellenberg@linbit.com>.

   drbd is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   drbd is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with drbd; see the file COPYING.  If not, write to
   the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

 */

#include <linux/module.h>

#include <linux/slab.h>
#include <linux/drbd.h>
#include "drbd_int.h"
#include "drbd_req.h"


<<<<<<< HEAD
/* Update disk stats at start of I/O request */
static void _drbd_start_io_acct(struct drbd_conf *mdev, struct drbd_request *req, struct bio *bio)
{
	const int rw = bio_data_dir(bio);
=======
static bool drbd_may_do_local_read(struct drbd_conf *mdev, sector_t sector, int size);

/* Update disk stats at start of I/O request */
static void _drbd_start_io_acct(struct drbd_conf *mdev, struct drbd_request *req)
{
	const int rw = bio_data_dir(req->master_bio);
>>>>>>> refs/remotes/origin/master
	int cpu;
	cpu = part_stat_lock();
	part_round_stats(cpu, &mdev->vdisk->part0);
	part_stat_inc(cpu, &mdev->vdisk->part0, ios[rw]);
<<<<<<< HEAD
	part_stat_add(cpu, &mdev->vdisk->part0, sectors[rw], bio_sectors(bio));
=======
	part_stat_add(cpu, &mdev->vdisk->part0, sectors[rw], req->i.size >> 9);
	(void) cpu; /* The macro invocations above want the cpu argument, I do not like
		       the compiler warning about cpu only assigned but never used... */
>>>>>>> refs/remotes/origin/master
	part_inc_in_flight(&mdev->vdisk->part0, rw);
	part_stat_unlock();
}

/* Update disk stats when completing request upwards */
static void _drbd_end_io_acct(struct drbd_conf *mdev, struct drbd_request *req)
{
	int rw = bio_data_dir(req->master_bio);
	unsigned long duration = jiffies - req->start_time;
	int cpu;
	cpu = part_stat_lock();
	part_stat_add(cpu, &mdev->vdisk->part0, ticks[rw], duration);
	part_round_stats(cpu, &mdev->vdisk->part0);
	part_dec_in_flight(&mdev->vdisk->part0, rw);
	part_stat_unlock();
}

<<<<<<< HEAD
static void _req_is_done(struct drbd_conf *mdev, struct drbd_request *req, const int rw)
{
	const unsigned long s = req->rq_state;
=======
static struct drbd_request *drbd_req_new(struct drbd_conf *mdev,
					       struct bio *bio_src)
{
	struct drbd_request *req;

	req = mempool_alloc(drbd_request_mempool, GFP_NOIO);
	if (!req)
		return NULL;

	drbd_req_make_private_bio(req, bio_src);
	req->rq_state    = bio_data_dir(bio_src) == WRITE ? RQ_WRITE : 0;
	req->w.mdev      = mdev;
	req->master_bio  = bio_src;
	req->epoch       = 0;

	drbd_clear_interval(&req->i);
	req->i.sector     = bio_src->bi_sector;
	req->i.size      = bio_src->bi_size;
	req->i.local = true;
	req->i.waiting = false;

	INIT_LIST_HEAD(&req->tl_requests);
	INIT_LIST_HEAD(&req->w.list);

	/* one reference to be put by __drbd_make_request */
	atomic_set(&req->completion_ref, 1);
	/* one kref as long as completion_ref > 0 */
	kref_init(&req->kref);
	return req;
}

void drbd_req_destroy(struct kref *kref)
{
	struct drbd_request *req = container_of(kref, struct drbd_request, kref);
	struct drbd_conf *mdev = req->w.mdev;
	const unsigned s = req->rq_state;

	if ((req->master_bio && !(s & RQ_POSTPONED)) ||
		atomic_read(&req->completion_ref) ||
		(s & RQ_LOCAL_PENDING) ||
		((s & RQ_NET_MASK) && !(s & RQ_NET_DONE))) {
		dev_err(DEV, "drbd_req_destroy: Logic BUG rq_state = 0x%x, completion_ref = %d\n",
				s, atomic_read(&req->completion_ref));
		return;
	}
>>>>>>> refs/remotes/origin/master

	/* remove it from the transfer log.
	 * well, only if it had been there in the first
	 * place... if it had not (local only or conflicting
	 * and never sent), it should still be "empty" as
	 * initialized in drbd_req_new(), so we can list_del() it
	 * here unconditionally */
<<<<<<< HEAD
	list_del(&req->tl_requests);
=======
	list_del_init(&req->tl_requests);
>>>>>>> refs/remotes/origin/master

	/* if it was a write, we may have to set the corresponding
	 * bit(s) out-of-sync first. If it had a local part, we need to
	 * release the reference to the activity log. */
<<<<<<< HEAD
	if (rw == WRITE) {
=======
	if (s & RQ_WRITE) {
>>>>>>> refs/remotes/origin/master
		/* Set out-of-sync unless both OK flags are set
		 * (local only or remote failed).
		 * Other places where we set out-of-sync:
		 * READ with local io-error */
<<<<<<< HEAD
		if (!(s & RQ_NET_OK) || !(s & RQ_LOCAL_OK))
			drbd_set_out_of_sync(mdev, req->sector, req->size);

		if ((s & RQ_NET_OK) && (s & RQ_LOCAL_OK) && (s & RQ_NET_SIS))
			drbd_set_in_sync(mdev, req->sector, req->size);

		/* one might be tempted to move the drbd_al_complete_io
		 * to the local io completion callback drbd_endio_pri.
=======

		/* There is a special case:
		 * we may notice late that IO was suspended,
		 * and postpone, or schedule for retry, a write,
		 * before it even was submitted or sent.
		 * In that case we do not want to touch the bitmap at all.
		 */
		if ((s & (RQ_POSTPONED|RQ_LOCAL_MASK|RQ_NET_MASK)) != RQ_POSTPONED) {
			if (!(s & RQ_NET_OK) || !(s & RQ_LOCAL_OK))
				drbd_set_out_of_sync(mdev, req->i.sector, req->i.size);

			if ((s & RQ_NET_OK) && (s & RQ_LOCAL_OK) && (s & RQ_NET_SIS))
				drbd_set_in_sync(mdev, req->i.sector, req->i.size);
		}

		/* one might be tempted to move the drbd_al_complete_io
		 * to the local io completion callback drbd_request_endio.
>>>>>>> refs/remotes/origin/master
		 * but, if this was a mirror write, we may only
		 * drbd_al_complete_io after this is RQ_NET_DONE,
		 * otherwise the extent could be dropped from the al
		 * before it has actually been written on the peer.
		 * if we crash before our peer knows about the request,
		 * but after the extent has been dropped from the al,
		 * we would forget to resync the corresponding extent.
		 */
<<<<<<< HEAD
		if (s & RQ_LOCAL_MASK) {
			if (get_ldev_if_state(mdev, D_FAILED)) {
				if (s & RQ_IN_ACT_LOG)
					drbd_al_complete_io(mdev, req->sector);
				put_ldev(mdev);
			} else if (__ratelimit(&drbd_ratelimit_state)) {
				dev_warn(DEV, "Should have called drbd_al_complete_io(, %llu), "
				     "but my Disk seems to have failed :(\n",
				     (unsigned long long) req->sector);
=======
		if (s & RQ_IN_ACT_LOG) {
			if (get_ldev_if_state(mdev, D_FAILED)) {
				drbd_al_complete_io(mdev, &req->i);
				put_ldev(mdev);
			} else if (__ratelimit(&drbd_ratelimit_state)) {
				dev_warn(DEV, "Should have called drbd_al_complete_io(, %llu, %u), "
					 "but my Disk seems to have failed :(\n",
					 (unsigned long long) req->i.sector, req->i.size);
>>>>>>> refs/remotes/origin/master
			}
		}
	}

<<<<<<< HEAD
	drbd_req_free(req);
}

static void queue_barrier(struct drbd_conf *mdev)
{
	struct drbd_tl_epoch *b;

	/* We are within the req_lock. Once we queued the barrier for sending,
	 * we set the CREATE_BARRIER bit. It is cleared as soon as a new
	 * barrier/epoch object is added. This is the only place this bit is
	 * set. It indicates that the barrier for this epoch is already queued,
	 * and no new epoch has been created yet. */
	if (test_bit(CREATE_BARRIER, &mdev->flags))
		return;

	b = mdev->newest_tle;
	b->w.cb = w_send_barrier;
	/* inc_ap_pending done here, so we won't
	 * get imbalanced on connection loss.
	 * dec_ap_pending will be done in got_BarrierAck
	 * or (on connection loss) in tl_clear.  */
	inc_ap_pending(mdev);
	drbd_queue_work(&mdev->data.work, &b->w);
	set_bit(CREATE_BARRIER, &mdev->flags);
}

static void _about_to_complete_local_write(struct drbd_conf *mdev,
	struct drbd_request *req)
{
	const unsigned long s = req->rq_state;
	struct drbd_request *i;
	struct drbd_epoch_entry *e;
	struct hlist_node *n;
	struct hlist_head *slot;

	/* Before we can signal completion to the upper layers,
	 * we may need to close the current epoch.
	 * We can skip this, if this request has not even been sent, because we
	 * did not have a fully established connection yet/anymore, during
	 * bitmap exchange, or while we are C_AHEAD due to congestion policy.
	 */
	if (mdev->state.conn >= C_CONNECTED &&
	    (s & RQ_NET_SENT) != 0 &&
	    req->epoch == mdev->newest_tle->br_number)
		queue_barrier(mdev);

	/* we need to do the conflict detection stuff,
	 * if we have the ee_hash (two_primaries) and
	 * this has been on the network */
	if ((s & RQ_NET_DONE) && mdev->ee_hash != NULL) {
		const sector_t sector = req->sector;
		const int size = req->size;

		/* ASSERT:
		 * there must be no conflicting requests, since
		 * they must have been failed on the spot */
#define OVERLAPS overlaps(sector, size, i->sector, i->size)
		slot = tl_hash_slot(mdev, sector);
		hlist_for_each_entry(i, n, slot, collision) {
			if (OVERLAPS) {
				dev_alert(DEV, "LOGIC BUG: completed: %p %llus +%u; "
				      "other: %p %llus +%u\n",
				      req, (unsigned long long)sector, size,
				      i, (unsigned long long)i->sector, i->size);
			}
		}

		/* maybe "wake" those conflicting epoch entries
		 * that wait for this request to finish.
		 *
		 * currently, there can be only _one_ such ee
		 * (well, or some more, which would be pending
		 * P_DISCARD_ACK not yet sent by the asender...),
		 * since we block the receiver thread upon the
		 * first conflict detection, which will wait on
		 * misc_wait.  maybe we want to assert that?
		 *
		 * anyways, if we found one,
		 * we just have to do a wake_up.  */
#undef OVERLAPS
#define OVERLAPS overlaps(sector, size, e->sector, e->size)
		slot = ee_hash_slot(mdev, req->sector);
		hlist_for_each_entry(e, n, slot, collision) {
			if (OVERLAPS) {
				wake_up(&mdev->misc_wait);
				break;
			}
		}
	}
#undef OVERLAPS
=======
	mempool_free(req, drbd_request_mempool);
}

static void wake_all_senders(struct drbd_tconn *tconn) {
	wake_up(&tconn->sender_work.q_wait);
}

/* must hold resource->req_lock */
void start_new_tl_epoch(struct drbd_tconn *tconn)
{
	/* no point closing an epoch, if it is empty, anyways. */
	if (tconn->current_tle_writes == 0)
		return;

	tconn->current_tle_writes = 0;
	atomic_inc(&tconn->current_tle_nr);
	wake_all_senders(tconn);
>>>>>>> refs/remotes/origin/master
}

void complete_master_bio(struct drbd_conf *mdev,
		struct bio_and_error *m)
{
	bio_endio(m->bio, m->error);
	dec_ap_bio(mdev);
}

<<<<<<< HEAD
=======

static void drbd_remove_request_interval(struct rb_root *root,
					 struct drbd_request *req)
{
	struct drbd_conf *mdev = req->w.mdev;
	struct drbd_interval *i = &req->i;

	drbd_remove_interval(root, i);

	/* Wake up any processes waiting for this request to complete.  */
	if (i->waiting)
		wake_up(&mdev->misc_wait);
}

>>>>>>> refs/remotes/origin/master
/* Helper for __req_mod().
 * Set m->bio to the master bio, if it is fit to be completed,
 * or leave it alone (it is initialized to NULL in __req_mod),
 * if it has already been completed, or cannot be completed yet.
 * If m->bio is set, the error status to be returned is placed in m->error.
 */
<<<<<<< HEAD
void _req_may_be_done(struct drbd_request *req, struct bio_and_error *m)
{
	const unsigned long s = req->rq_state;
	struct drbd_conf *mdev = req->mdev;
	/* only WRITES may end up here without a master bio (on barrier ack) */
	int rw = req->master_bio ? bio_data_dir(req->master_bio) : WRITE;
=======
static
void drbd_req_complete(struct drbd_request *req, struct bio_and_error *m)
{
	const unsigned s = req->rq_state;
	struct drbd_conf *mdev = req->w.mdev;
	int rw;
	int error, ok;
>>>>>>> refs/remotes/origin/master

	/* we must not complete the master bio, while it is
	 *	still being processed by _drbd_send_zc_bio (drbd_send_dblock)
	 *	not yet acknowledged by the peer
	 *	not yet completed by the local io subsystem
	 * these flags may get cleared in any order by
	 *	the worker,
	 *	the receiver,
	 *	the bio_endio completion callbacks.
	 */
<<<<<<< HEAD
	if (s & RQ_NET_QUEUED)
		return;
	if (s & RQ_NET_PENDING)
		return;
	if (s & RQ_LOCAL_PENDING)
		return;

	if (req->master_bio) {
		/* this is data_received (remote read)
		 * or protocol C P_WRITE_ACK
		 * or protocol B P_RECV_ACK
		 * or protocol A "handed_over_to_network" (SendAck)
		 * or canceled or failed,
		 * or killed from the transfer log due to connection loss.
		 */

		/*
		 * figure out whether to report success or failure.
		 *
		 * report success when at least one of the operations succeeded.
		 * or, to put the other way,
		 * only report failure, when both operations failed.
		 *
		 * what to do about the failures is handled elsewhere.
		 * what we need to do here is just: complete the master_bio.
		 *
		 * local completion error, if any, has been stored as ERR_PTR
		 * in private_bio within drbd_endio_pri.
		 */
		int ok = (s & RQ_LOCAL_OK) || (s & RQ_NET_OK);
		int error = PTR_ERR(req->private_bio);

		/* remove the request from the conflict detection
		 * respective block_id verification hash */
		if (!hlist_unhashed(&req->collision))
			hlist_del(&req->collision);
		else
			D_ASSERT((s & (RQ_NET_MASK & ~RQ_NET_DONE)) == 0);

		/* for writes we need to do some extra housekeeping */
		if (rw == WRITE)
			_about_to_complete_local_write(mdev, req);

		/* Update disk stats */
		_drbd_end_io_acct(mdev, req);

=======
	if ((s & RQ_LOCAL_PENDING && !(s & RQ_LOCAL_ABORTED)) ||
	    (s & RQ_NET_QUEUED) || (s & RQ_NET_PENDING) ||
	    (s & RQ_COMPLETION_SUSP)) {
		dev_err(DEV, "drbd_req_complete: Logic BUG rq_state = 0x%x\n", s);
		return;
	}

	if (!req->master_bio) {
		dev_err(DEV, "drbd_req_complete: Logic BUG, master_bio == NULL!\n");
		return;
	}

	rw = bio_rw(req->master_bio);

	/*
	 * figure out whether to report success or failure.
	 *
	 * report success when at least one of the operations succeeded.
	 * or, to put the other way,
	 * only report failure, when both operations failed.
	 *
	 * what to do about the failures is handled elsewhere.
	 * what we need to do here is just: complete the master_bio.
	 *
	 * local completion error, if any, has been stored as ERR_PTR
	 * in private_bio within drbd_request_endio.
	 */
	ok = (s & RQ_LOCAL_OK) || (s & RQ_NET_OK);
	error = PTR_ERR(req->private_bio);

	/* remove the request from the conflict detection
	 * respective block_id verification hash */
	if (!drbd_interval_empty(&req->i)) {
		struct rb_root *root;

		if (rw == WRITE)
			root = &mdev->write_requests;
		else
			root = &mdev->read_requests;
		drbd_remove_request_interval(root, req);
	}

	/* Before we can signal completion to the upper layers,
	 * we may need to close the current transfer log epoch.
	 * We are within the request lock, so we can simply compare
	 * the request epoch number with the current transfer log
	 * epoch number.  If they match, increase the current_tle_nr,
	 * and reset the transfer log epoch write_cnt.
	 */
	if (rw == WRITE &&
	    req->epoch == atomic_read(&mdev->tconn->current_tle_nr))
		start_new_tl_epoch(mdev->tconn);

	/* Update disk stats */
	_drbd_end_io_acct(mdev, req);

	/* If READ failed,
	 * have it be pushed back to the retry work queue,
	 * so it will re-enter __drbd_make_request(),
	 * and be re-assigned to a suitable local or remote path,
	 * or failed if we do not have access to good data anymore.
	 *
	 * Unless it was failed early by __drbd_make_request(),
	 * because no path was available, in which case
	 * it was not even added to the transfer_log.
	 *
	 * READA may fail, and will not be retried.
	 *
	 * WRITE should have used all available paths already.
	 */
	if (!ok && rw == READ && !list_empty(&req->tl_requests))
		req->rq_state |= RQ_POSTPONED;

	if (!(req->rq_state & RQ_POSTPONED)) {
>>>>>>> refs/remotes/origin/master
		m->error = ok ? 0 : (error ?: -EIO);
		m->bio = req->master_bio;
		req->master_bio = NULL;
	}
<<<<<<< HEAD

	if ((s & RQ_NET_MASK) == 0 || (s & RQ_NET_DONE)) {
		/* this is disconnected (local only) operation,
		 * or protocol C P_WRITE_ACK,
		 * or protocol A or B P_BARRIER_ACK,
		 * or killed from the transfer log due to connection loss. */
		_req_is_done(mdev, req, rw);
	}
	/* else: network part and not DONE yet. that is
	 * protocol A or B, barrier ack still pending... */
}

static void _req_may_be_done_not_susp(struct drbd_request *req, struct bio_and_error *m)
{
	struct drbd_conf *mdev = req->mdev;

	if (!is_susp(mdev->state))
		_req_may_be_done(req, m);
}

/*
 * checks whether there was an overlapping request
 * or ee already registered.
 *
 * if so, return 1, in which case this request is completed on the spot,
 * without ever being submitted or send.
 *
 * return 0 if it is ok to submit this request.
 *
 * NOTE:
 * paranoia: assume something above us is broken, and issues different write
 * requests for the same block simultaneously...
 *
 * To ensure these won't be reordered differently on both nodes, resulting in
 * diverging data sets, we discard the later one(s). Not that this is supposed
 * to happen, but this is the rationale why we also have to check for
 * conflicting requests with local origin, and why we have to do so regardless
 * of whether we allowed multiple primaries.
 *
 * BTW, in case we only have one primary, the ee_hash is empty anyways, and the
 * second hlist_for_each_entry becomes a noop. This is even simpler than to
 * grab a reference on the net_conf, and check for the two_primaries flag...
 */
static int _req_conflicts(struct drbd_request *req)
{
	struct drbd_conf *mdev = req->mdev;
	const sector_t sector = req->sector;
	const int size = req->size;
	struct drbd_request *i;
	struct drbd_epoch_entry *e;
	struct hlist_node *n;
	struct hlist_head *slot;

	D_ASSERT(hlist_unhashed(&req->collision));

	if (!get_net_conf(mdev))
		return 0;

	/* BUG_ON */
	ERR_IF (mdev->tl_hash_s == 0)
		goto out_no_conflict;
	BUG_ON(mdev->tl_hash == NULL);

#define OVERLAPS overlaps(i->sector, i->size, sector, size)
	slot = tl_hash_slot(mdev, sector);
	hlist_for_each_entry(i, n, slot, collision) {
		if (OVERLAPS) {
			dev_alert(DEV, "%s[%u] Concurrent local write detected! "
			      "[DISCARD L] new: %llus +%u; "
			      "pending: %llus +%u\n",
			      current->comm, current->pid,
			      (unsigned long long)sector, size,
			      (unsigned long long)i->sector, i->size);
			goto out_conflict;
		}
	}

	if (mdev->ee_hash_s) {
		/* now, check for overlapping requests with remote origin */
		BUG_ON(mdev->ee_hash == NULL);
#undef OVERLAPS
#define OVERLAPS overlaps(e->sector, e->size, sector, size)
		slot = ee_hash_slot(mdev, sector);
		hlist_for_each_entry(e, n, slot, collision) {
			if (OVERLAPS) {
				dev_alert(DEV, "%s[%u] Concurrent remote write detected!"
				      " [DISCARD L] new: %llus +%u; "
				      "pending: %llus +%u\n",
				      current->comm, current->pid,
				      (unsigned long long)sector, size,
				      (unsigned long long)e->sector, e->size);
				goto out_conflict;
			}
		}
	}
#undef OVERLAPS

out_no_conflict:
	/* this is like it should be, and what we expected.
	 * our users do behave after all... */
	put_net_conf(mdev);
	return 0;

out_conflict:
	put_net_conf(mdev);
	return 1;
=======
}

static int drbd_req_put_completion_ref(struct drbd_request *req, struct bio_and_error *m, int put)
{
	struct drbd_conf *mdev = req->w.mdev;
	D_ASSERT(m || (req->rq_state & RQ_POSTPONED));

	if (!atomic_sub_and_test(put, &req->completion_ref))
		return 0;

	drbd_req_complete(req, m);

	if (req->rq_state & RQ_POSTPONED) {
		/* don't destroy the req object just yet,
		 * but queue it for retry */
		drbd_restart_request(req);
		return 0;
	}

	return 1;
}

/* I'd like this to be the only place that manipulates
 * req->completion_ref and req->kref. */
static void mod_rq_state(struct drbd_request *req, struct bio_and_error *m,
		int clear, int set)
{
	struct drbd_conf *mdev = req->w.mdev;
	unsigned s = req->rq_state;
	int c_put = 0;
	int k_put = 0;

	if (drbd_suspended(mdev) && !((s | clear) & RQ_COMPLETION_SUSP))
		set |= RQ_COMPLETION_SUSP;

	/* apply */

	req->rq_state &= ~clear;
	req->rq_state |= set;

	/* no change? */
	if (req->rq_state == s)
		return;

	/* intent: get references */

	if (!(s & RQ_LOCAL_PENDING) && (set & RQ_LOCAL_PENDING))
		atomic_inc(&req->completion_ref);

	if (!(s & RQ_NET_PENDING) && (set & RQ_NET_PENDING)) {
		inc_ap_pending(mdev);
		atomic_inc(&req->completion_ref);
	}

	if (!(s & RQ_NET_QUEUED) && (set & RQ_NET_QUEUED))
		atomic_inc(&req->completion_ref);

	if (!(s & RQ_EXP_BARR_ACK) && (set & RQ_EXP_BARR_ACK))
		kref_get(&req->kref); /* wait for the DONE */

	if (!(s & RQ_NET_SENT) && (set & RQ_NET_SENT))
		atomic_add(req->i.size >> 9, &mdev->ap_in_flight);

	if (!(s & RQ_COMPLETION_SUSP) && (set & RQ_COMPLETION_SUSP))
		atomic_inc(&req->completion_ref);

	/* progress: put references */

	if ((s & RQ_COMPLETION_SUSP) && (clear & RQ_COMPLETION_SUSP))
		++c_put;

	if (!(s & RQ_LOCAL_ABORTED) && (set & RQ_LOCAL_ABORTED)) {
		D_ASSERT(req->rq_state & RQ_LOCAL_PENDING);
		/* local completion may still come in later,
		 * we need to keep the req object around. */
		kref_get(&req->kref);
		++c_put;
	}

	if ((s & RQ_LOCAL_PENDING) && (clear & RQ_LOCAL_PENDING)) {
		if (req->rq_state & RQ_LOCAL_ABORTED)
			++k_put;
		else
			++c_put;
	}

	if ((s & RQ_NET_PENDING) && (clear & RQ_NET_PENDING)) {
		dec_ap_pending(mdev);
		++c_put;
	}

	if ((s & RQ_NET_QUEUED) && (clear & RQ_NET_QUEUED))
		++c_put;

	if ((s & RQ_EXP_BARR_ACK) && !(s & RQ_NET_DONE) && (set & RQ_NET_DONE)) {
		if (req->rq_state & RQ_NET_SENT)
			atomic_sub(req->i.size >> 9, &mdev->ap_in_flight);
		++k_put;
	}

	/* potentially complete and destroy */

	if (k_put || c_put) {
		/* Completion does it's own kref_put.  If we are going to
		 * kref_sub below, we need req to be still around then. */
		int at_least = k_put + !!c_put;
		int refcount = atomic_read(&req->kref.refcount);
		if (refcount < at_least)
			dev_err(DEV,
				"mod_rq_state: Logic BUG: %x -> %x: refcount = %d, should be >= %d\n",
				s, req->rq_state, refcount, at_least);
	}

	/* If we made progress, retry conflicting peer requests, if any. */
	if (req->i.waiting)
		wake_up(&mdev->misc_wait);

	if (c_put)
		k_put += drbd_req_put_completion_ref(req, m, c_put);
	if (k_put)
		kref_sub(&req->kref, k_put, drbd_req_destroy);
}

static void drbd_report_io_error(struct drbd_conf *mdev, struct drbd_request *req)
{
        char b[BDEVNAME_SIZE];

	if (!__ratelimit(&drbd_ratelimit_state))
		return;

	dev_warn(DEV, "local %s IO error sector %llu+%u on %s\n",
			(req->rq_state & RQ_WRITE) ? "WRITE" : "READ",
			(unsigned long long)req->i.sector,
			req->i.size >> 9,
			bdevname(mdev->ldev->backing_bdev, b));
>>>>>>> refs/remotes/origin/master
}

/* obviously this could be coded as many single functions
 * instead of one huge switch,
 * or by putting the code directly in the respective locations
 * (as it has been before).
 *
 * but having it this way
 *  enforces that it is all in this one place, where it is easier to audit,
 *  it makes it obvious that whatever "event" "happens" to a request should
 *  happen "atomically" within the req_lock,
 *  and it enforces that we have to think in a very structured manner
 *  about the "events" that may happen to a request during its life time ...
 */
int __req_mod(struct drbd_request *req, enum drbd_req_event what,
		struct bio_and_error *m)
{
<<<<<<< HEAD
	struct drbd_conf *mdev = req->mdev;
	int rv = 0;
	m->bio = NULL;
=======
	struct drbd_conf *mdev = req->w.mdev;
	struct net_conf *nc;
	int p, rv = 0;

	if (m)
		m->bio = NULL;
>>>>>>> refs/remotes/origin/master

	switch (what) {
	default:
		dev_err(DEV, "LOGIC BUG in %s:%u\n", __FILE__ , __LINE__);
		break;

	/* does not happen...
	 * initialization done in drbd_req_new
<<<<<<< HEAD
	case created:
		break;
		*/

	case to_be_send: /* via network */
		/* reached via drbd_make_request_common
		 * and from w_read_retry_remote */
		D_ASSERT(!(req->rq_state & RQ_NET_MASK));
		req->rq_state |= RQ_NET_PENDING;
		inc_ap_pending(mdev);
		break;

	case to_be_submitted: /* locally */
		/* reached via drbd_make_request_common */
		D_ASSERT(!(req->rq_state & RQ_LOCAL_MASK));
		req->rq_state |= RQ_LOCAL_PENDING;
		break;

	case completed_ok:
		if (bio_data_dir(req->master_bio) == WRITE)
			mdev->writ_cnt += req->size>>9;
		else
			mdev->read_cnt += req->size>>9;

		req->rq_state |= (RQ_LOCAL_COMPLETED|RQ_LOCAL_OK);
		req->rq_state &= ~RQ_LOCAL_PENDING;

		_req_may_be_done_not_susp(req, m);
		put_ldev(mdev);
		break;

	case write_completed_with_error:
		req->rq_state |= RQ_LOCAL_COMPLETED;
		req->rq_state &= ~RQ_LOCAL_PENDING;

		__drbd_chk_io_error(mdev, false);
		_req_may_be_done_not_susp(req, m);
		put_ldev(mdev);
		break;

	case read_ahead_completed_with_error:
		/* it is legal to fail READA */
		req->rq_state |= RQ_LOCAL_COMPLETED;
		req->rq_state &= ~RQ_LOCAL_PENDING;
		_req_may_be_done_not_susp(req, m);
		put_ldev(mdev);
		break;

	case read_completed_with_error:
		drbd_set_out_of_sync(mdev, req->sector, req->size);

		req->rq_state |= RQ_LOCAL_COMPLETED;
		req->rq_state &= ~RQ_LOCAL_PENDING;

		D_ASSERT(!(req->rq_state & RQ_NET_MASK));

		__drbd_chk_io_error(mdev, false);
		put_ldev(mdev);

		/* no point in retrying if there is no good remote data,
		 * or we have no connection. */
		if (mdev->state.pdsk != D_UP_TO_DATE) {
			_req_may_be_done_not_susp(req, m);
			break;
		}

		/* _req_mod(req,to_be_send); oops, recursion... */
		req->rq_state |= RQ_NET_PENDING;
		inc_ap_pending(mdev);
		/* fall through: _req_mod(req,queue_for_net_read); */

	case queue_for_net_read:
=======
	case CREATED:
		break;
		*/

	case TO_BE_SENT: /* via network */
		/* reached via __drbd_make_request
		 * and from w_read_retry_remote */
		D_ASSERT(!(req->rq_state & RQ_NET_MASK));
		rcu_read_lock();
		nc = rcu_dereference(mdev->tconn->net_conf);
		p = nc->wire_protocol;
		rcu_read_unlock();
		req->rq_state |=
			p == DRBD_PROT_C ? RQ_EXP_WRITE_ACK :
			p == DRBD_PROT_B ? RQ_EXP_RECEIVE_ACK : 0;
		mod_rq_state(req, m, 0, RQ_NET_PENDING);
		break;

	case TO_BE_SUBMITTED: /* locally */
		/* reached via __drbd_make_request */
		D_ASSERT(!(req->rq_state & RQ_LOCAL_MASK));
		mod_rq_state(req, m, 0, RQ_LOCAL_PENDING);
		break;

	case COMPLETED_OK:
		if (req->rq_state & RQ_WRITE)
			mdev->writ_cnt += req->i.size >> 9;
		else
			mdev->read_cnt += req->i.size >> 9;

		mod_rq_state(req, m, RQ_LOCAL_PENDING,
				RQ_LOCAL_COMPLETED|RQ_LOCAL_OK);
		break;

	case ABORT_DISK_IO:
		mod_rq_state(req, m, 0, RQ_LOCAL_ABORTED);
		break;

	case WRITE_COMPLETED_WITH_ERROR:
		drbd_report_io_error(mdev, req);
		__drbd_chk_io_error(mdev, DRBD_WRITE_ERROR);
		mod_rq_state(req, m, RQ_LOCAL_PENDING, RQ_LOCAL_COMPLETED);
		break;

	case READ_COMPLETED_WITH_ERROR:
		drbd_set_out_of_sync(mdev, req->i.sector, req->i.size);
		drbd_report_io_error(mdev, req);
		__drbd_chk_io_error(mdev, DRBD_READ_ERROR);
		/* fall through. */
	case READ_AHEAD_COMPLETED_WITH_ERROR:
		/* it is legal to fail READA, no __drbd_chk_io_error in that case. */
		mod_rq_state(req, m, RQ_LOCAL_PENDING, RQ_LOCAL_COMPLETED);
		break;

	case QUEUE_FOR_NET_READ:
>>>>>>> refs/remotes/origin/master
		/* READ or READA, and
		 * no local disk,
		 * or target area marked as invalid,
		 * or just got an io-error. */
<<<<<<< HEAD
		/* from drbd_make_request_common
		 * or from bio_endio during read io-error recovery */

		/* so we can verify the handle in the answer packet
		 * corresponding hlist_del is in _req_may_be_done() */
		hlist_add_head(&req->collision, ar_hash_slot(mdev, req->sector));
=======
		/* from __drbd_make_request
		 * or from bio_endio during read io-error recovery */

		/* So we can verify the handle in the answer packet.
		 * Corresponding drbd_remove_request_interval is in
		 * drbd_req_complete() */
		D_ASSERT(drbd_interval_empty(&req->i));
		drbd_insert_interval(&mdev->read_requests, &req->i);
>>>>>>> refs/remotes/origin/master

		set_bit(UNPLUG_REMOTE, &mdev->flags);

		D_ASSERT(req->rq_state & RQ_NET_PENDING);
<<<<<<< HEAD
		req->rq_state |= RQ_NET_QUEUED;
		req->w.cb = (req->rq_state & RQ_LOCAL_MASK)
			? w_read_retry_remote
			: w_send_read_req;
		drbd_queue_work(&mdev->data.work, &req->w);
		break;

	case queue_for_net_write:
		/* assert something? */
		/* from drbd_make_request_common only */

		hlist_add_head(&req->collision, tl_hash_slot(mdev, req->sector));
		/* corresponding hlist_del is in _req_may_be_done() */
=======
		D_ASSERT((req->rq_state & RQ_LOCAL_MASK) == 0);
		mod_rq_state(req, m, 0, RQ_NET_QUEUED);
		req->w.cb = w_send_read_req;
		drbd_queue_work(&mdev->tconn->sender_work, &req->w);
		break;

	case QUEUE_FOR_NET_WRITE:
		/* assert something? */
		/* from __drbd_make_request only */

		/* Corresponding drbd_remove_request_interval is in
		 * drbd_req_complete() */
		D_ASSERT(drbd_interval_empty(&req->i));
		drbd_insert_interval(&mdev->write_requests, &req->i);
>>>>>>> refs/remotes/origin/master

		/* NOTE
		 * In case the req ended up on the transfer log before being
		 * queued on the worker, it could lead to this request being
		 * missed during cleanup after connection loss.
		 * So we have to do both operations here,
		 * within the same lock that protects the transfer log.
		 *
		 * _req_add_to_epoch(req); this has to be after the
		 * _maybe_start_new_epoch(req); which happened in
<<<<<<< HEAD
		 * drbd_make_request_common, because we now may set the bit
=======
		 * __drbd_make_request, because we now may set the bit
>>>>>>> refs/remotes/origin/master
		 * again ourselves to close the current epoch.
		 *
		 * Add req to the (now) current epoch (barrier). */

		/* otherwise we may lose an unplug, which may cause some remote
		 * io-scheduler timeout to expire, increasing maximum latency,
		 * hurting performance. */
		set_bit(UNPLUG_REMOTE, &mdev->flags);

<<<<<<< HEAD
		/* see drbd_make_request_common,
		 * just after it grabs the req_lock */
		D_ASSERT(test_bit(CREATE_BARRIER, &mdev->flags) == 0);

		req->epoch = mdev->newest_tle->br_number;

		/* increment size of current epoch */
		mdev->newest_tle->n_writes++;

		/* queue work item to send data */
		D_ASSERT(req->rq_state & RQ_NET_PENDING);
		req->rq_state |= RQ_NET_QUEUED;
		req->w.cb =  w_send_dblock;
		drbd_queue_work(&mdev->data.work, &req->w);

		/* close the epoch, in case it outgrew the limit */
		if (mdev->newest_tle->n_writes >= mdev->net_conf->max_epoch_size)
			queue_barrier(mdev);

		break;

	case queue_for_send_oos:
		req->rq_state |= RQ_NET_QUEUED;
		req->w.cb =  w_send_oos;
		drbd_queue_work(&mdev->data.work, &req->w);
		break;

	case oos_handed_to_network:
		/* actually the same */
	case send_canceled:
		/* treat it the same */
	case send_failed:
		/* real cleanup will be done from tl_clear.  just update flags
		 * so it is no longer marked as on the worker queue */
		req->rq_state &= ~RQ_NET_QUEUED;
		/* if we did it right, tl_clear should be scheduled only after
		 * this, so this should not be necessary! */
		_req_may_be_done_not_susp(req, m);
		break;

	case handed_over_to_network:
		/* assert something? */
		if (bio_data_dir(req->master_bio) == WRITE)
			atomic_add(req->size>>9, &mdev->ap_in_flight);

		if (bio_data_dir(req->master_bio) == WRITE &&
		    mdev->net_conf->wire_protocol == DRBD_PROT_A) {
			/* this is what is dangerous about protocol A:
			 * pretend it was successfully written on the peer. */
			if (req->rq_state & RQ_NET_PENDING) {
				dec_ap_pending(mdev);
				req->rq_state &= ~RQ_NET_PENDING;
				req->rq_state |= RQ_NET_OK;
			} /* else: neg-ack was faster... */
=======
		/* queue work item to send data */
		D_ASSERT(req->rq_state & RQ_NET_PENDING);
		mod_rq_state(req, m, 0, RQ_NET_QUEUED|RQ_EXP_BARR_ACK);
		req->w.cb =  w_send_dblock;
		drbd_queue_work(&mdev->tconn->sender_work, &req->w);

		/* close the epoch, in case it outgrew the limit */
		rcu_read_lock();
		nc = rcu_dereference(mdev->tconn->net_conf);
		p = nc->max_epoch_size;
		rcu_read_unlock();
		if (mdev->tconn->current_tle_writes >= p)
			start_new_tl_epoch(mdev->tconn);

		break;

	case QUEUE_FOR_SEND_OOS:
		mod_rq_state(req, m, 0, RQ_NET_QUEUED);
		req->w.cb =  w_send_out_of_sync;
		drbd_queue_work(&mdev->tconn->sender_work, &req->w);
		break;

	case READ_RETRY_REMOTE_CANCELED:
	case SEND_CANCELED:
	case SEND_FAILED:
		/* real cleanup will be done from tl_clear.  just update flags
		 * so it is no longer marked as on the worker queue */
		mod_rq_state(req, m, RQ_NET_QUEUED, 0);
		break;

	case HANDED_OVER_TO_NETWORK:
		/* assert something? */
		if (bio_data_dir(req->master_bio) == WRITE &&
		    !(req->rq_state & (RQ_EXP_RECEIVE_ACK | RQ_EXP_WRITE_ACK))) {
			/* this is what is dangerous about protocol A:
			 * pretend it was successfully written on the peer. */
			if (req->rq_state & RQ_NET_PENDING)
				mod_rq_state(req, m, RQ_NET_PENDING, RQ_NET_OK);
			/* else: neg-ack was faster... */
>>>>>>> refs/remotes/origin/master
			/* it is still not yet RQ_NET_DONE until the
			 * corresponding epoch barrier got acked as well,
			 * so we know what to dirty on connection loss */
		}
<<<<<<< HEAD
		req->rq_state &= ~RQ_NET_QUEUED;
		req->rq_state |= RQ_NET_SENT;
		/* because _drbd_send_zc_bio could sleep, and may want to
		 * dereference the bio even after the "write_acked_by_peer" and
		 * "completed_ok" events came in, once we return from
		 * _drbd_send_zc_bio (drbd_send_dblock), we have to check
		 * whether it is done already, and end it.  */
		_req_may_be_done_not_susp(req, m);
		break;

	case read_retry_remote_canceled:
		req->rq_state &= ~RQ_NET_QUEUED;
		/* fall through, in case we raced with drbd_disconnect */
	case connection_lost_while_pending:
		/* transfer log cleanup after connection loss */
		/* assert something? */
		if (req->rq_state & RQ_NET_PENDING)
			dec_ap_pending(mdev);
		req->rq_state &= ~(RQ_NET_OK|RQ_NET_PENDING);
		req->rq_state |= RQ_NET_DONE;
		if (req->rq_state & RQ_NET_SENT && req->rq_state & RQ_WRITE)
			atomic_sub(req->size>>9, &mdev->ap_in_flight);

		/* if it is still queued, we may not complete it here.
		 * it will be canceled soon. */
		if (!(req->rq_state & RQ_NET_QUEUED))
			_req_may_be_done(req, m); /* Allowed while state.susp */
		break;

	case write_acked_by_peer_and_sis:
		req->rq_state |= RQ_NET_SIS;
	case conflict_discarded_by_peer:
		/* for discarded conflicting writes of multiple primaries,
		 * there is no need to keep anything in the tl, potential
		 * node crashes are covered by the activity log. */
		if (what == conflict_discarded_by_peer)
			dev_alert(DEV, "Got DiscardAck packet %llus +%u!"
			      " DRBD is not a random data generator!\n",
			      (unsigned long long)req->sector, req->size);
		req->rq_state |= RQ_NET_DONE;
		/* fall through */
	case write_acked_by_peer:
		/* protocol C; successfully written on peer.
		 * Nothing to do here.
		 * We want to keep the tl in place for all protocols, to cater
		 * for volatile write-back caches on lower level devices.
		 *
		 * A barrier request is expected to have forced all prior
		 * requests onto stable storage, so completion of a barrier
		 * request could set NET_DONE right here, and not wait for the
		 * P_BARRIER_ACK, but that is an unnecessary optimization. */

		/* this makes it effectively the same as for: */
	case recv_acked_by_peer:
		/* protocol B; pretends to be successfully written on peer.
		 * see also notes above in handed_over_to_network about
		 * protocol != C */
		req->rq_state |= RQ_NET_OK;
		D_ASSERT(req->rq_state & RQ_NET_PENDING);
		dec_ap_pending(mdev);
		atomic_sub(req->size>>9, &mdev->ap_in_flight);
		req->rq_state &= ~RQ_NET_PENDING;
		_req_may_be_done_not_susp(req, m);
		break;

	case neg_acked:
		/* assert something? */
		if (req->rq_state & RQ_NET_PENDING) {
			dec_ap_pending(mdev);
			atomic_sub(req->size>>9, &mdev->ap_in_flight);
		}
		req->rq_state &= ~(RQ_NET_OK|RQ_NET_PENDING);

		req->rq_state |= RQ_NET_DONE;
		_req_may_be_done_not_susp(req, m);
		/* else: done by handed_over_to_network */
		break;

	case fail_frozen_disk_io:
		if (!(req->rq_state & RQ_LOCAL_COMPLETED))
			break;

		_req_may_be_done(req, m); /* Allowed while state.susp */
		break;

	case restart_frozen_disk_io:
		if (!(req->rq_state & RQ_LOCAL_COMPLETED))
			break;

		req->rq_state &= ~RQ_LOCAL_COMPLETED;
=======
		mod_rq_state(req, m, RQ_NET_QUEUED, RQ_NET_SENT);
		break;

	case OOS_HANDED_TO_NETWORK:
		/* Was not set PENDING, no longer QUEUED, so is now DONE
		 * as far as this connection is concerned. */
		mod_rq_state(req, m, RQ_NET_QUEUED, RQ_NET_DONE);
		break;

	case CONNECTION_LOST_WHILE_PENDING:
		/* transfer log cleanup after connection loss */
		mod_rq_state(req, m,
				RQ_NET_OK|RQ_NET_PENDING|RQ_COMPLETION_SUSP,
				RQ_NET_DONE);
		break;

	case CONFLICT_RESOLVED:
		/* for superseded conflicting writes of multiple primaries,
		 * there is no need to keep anything in the tl, potential
		 * node crashes are covered by the activity log.
		 *
		 * If this request had been marked as RQ_POSTPONED before,
		 * it will actually not be completed, but "restarted",
		 * resubmitted from the retry worker context. */
		D_ASSERT(req->rq_state & RQ_NET_PENDING);
		D_ASSERT(req->rq_state & RQ_EXP_WRITE_ACK);
		mod_rq_state(req, m, RQ_NET_PENDING, RQ_NET_DONE|RQ_NET_OK);
		break;

	case WRITE_ACKED_BY_PEER_AND_SIS:
		req->rq_state |= RQ_NET_SIS;
	case WRITE_ACKED_BY_PEER:
		D_ASSERT(req->rq_state & RQ_EXP_WRITE_ACK);
		/* protocol C; successfully written on peer.
		 * Nothing more to do here.
		 * We want to keep the tl in place for all protocols, to cater
		 * for volatile write-back caches on lower level devices. */

		goto ack_common;
	case RECV_ACKED_BY_PEER:
		D_ASSERT(req->rq_state & RQ_EXP_RECEIVE_ACK);
		/* protocol B; pretends to be successfully written on peer.
		 * see also notes above in HANDED_OVER_TO_NETWORK about
		 * protocol != C */
	ack_common:
		D_ASSERT(req->rq_state & RQ_NET_PENDING);
		mod_rq_state(req, m, RQ_NET_PENDING, RQ_NET_OK);
		break;

	case POSTPONE_WRITE:
		D_ASSERT(req->rq_state & RQ_EXP_WRITE_ACK);
		/* If this node has already detected the write conflict, the
		 * worker will be waiting on misc_wait.  Wake it up once this
		 * request has completed locally.
		 */
		D_ASSERT(req->rq_state & RQ_NET_PENDING);
		req->rq_state |= RQ_POSTPONED;
		if (req->i.waiting)
			wake_up(&mdev->misc_wait);
		/* Do not clear RQ_NET_PENDING. This request will make further
		 * progress via restart_conflicting_writes() or
		 * fail_postponed_requests(). Hopefully. */
		break;

	case NEG_ACKED:
		mod_rq_state(req, m, RQ_NET_OK|RQ_NET_PENDING, 0);
		break;

	case FAIL_FROZEN_DISK_IO:
		if (!(req->rq_state & RQ_LOCAL_COMPLETED))
			break;
		mod_rq_state(req, m, RQ_COMPLETION_SUSP, 0);
		break;

	case RESTART_FROZEN_DISK_IO:
		if (!(req->rq_state & RQ_LOCAL_COMPLETED))
			break;

		mod_rq_state(req, m,
				RQ_COMPLETION_SUSP|RQ_LOCAL_COMPLETED,
				RQ_LOCAL_PENDING);
>>>>>>> refs/remotes/origin/master

		rv = MR_READ;
		if (bio_data_dir(req->master_bio) == WRITE)
			rv = MR_WRITE;

<<<<<<< HEAD
		get_ldev(mdev);
		req->w.cb = w_restart_disk_io;
		drbd_queue_work(&mdev->data.work, &req->w);
		break;

	case resend:
		/* If RQ_NET_OK is already set, we got a P_WRITE_ACK or P_RECV_ACK
		   before the connection loss (B&C only); only P_BARRIER_ACK was missing.
		   Trowing them out of the TL here by pretending we got a BARRIER_ACK
		   We ensure that the peer was not rebooted */
		if (!(req->rq_state & RQ_NET_OK)) {
			if (req->w.cb) {
				drbd_queue_work(&mdev->data.work, &req->w);
				rv = req->rq_state & RQ_WRITE ? MR_WRITE : MR_READ;
			}
			break;
		}
		/* else, fall through to barrier_acked */

	case barrier_acked:
=======
		get_ldev(mdev); /* always succeeds in this call path */
		req->w.cb = w_restart_disk_io;
		drbd_queue_work(&mdev->tconn->sender_work, &req->w);
		break;

	case RESEND:
		/* Simply complete (local only) READs. */
		if (!(req->rq_state & RQ_WRITE) && !req->w.cb) {
			mod_rq_state(req, m, RQ_COMPLETION_SUSP, 0);
			break;
		}

		/* If RQ_NET_OK is already set, we got a P_WRITE_ACK or P_RECV_ACK
		   before the connection loss (B&C only); only P_BARRIER_ACK
		   (or the local completion?) was missing when we suspended.
		   Throwing them out of the TL here by pretending we got a BARRIER_ACK.
		   During connection handshake, we ensure that the peer was not rebooted. */
		if (!(req->rq_state & RQ_NET_OK)) {
			/* FIXME could this possibly be a req->w.cb == w_send_out_of_sync?
			 * in that case we must not set RQ_NET_PENDING. */

			mod_rq_state(req, m, RQ_COMPLETION_SUSP, RQ_NET_QUEUED|RQ_NET_PENDING);
			if (req->w.cb) {
				drbd_queue_work(&mdev->tconn->sender_work, &req->w);
				rv = req->rq_state & RQ_WRITE ? MR_WRITE : MR_READ;
			} /* else: FIXME can this happen? */
			break;
		}
		/* else, fall through to BARRIER_ACKED */

	case BARRIER_ACKED:
		/* barrier ack for READ requests does not make sense */
>>>>>>> refs/remotes/origin/master
		if (!(req->rq_state & RQ_WRITE))
			break;

		if (req->rq_state & RQ_NET_PENDING) {
<<<<<<< HEAD
			/* barrier came in before all requests have been acked.
			 * this is bad, because if the connection is lost now,
			 * we won't be able to clean them up... */
			dev_err(DEV, "FIXME (barrier_acked but pending)\n");
			list_move(&req->tl_requests, &mdev->out_of_sequence_requests);
		}
		if ((req->rq_state & RQ_NET_MASK) != 0) {
			req->rq_state |= RQ_NET_DONE;
			if (mdev->net_conf->wire_protocol == DRBD_PROT_A)
				atomic_sub(req->size>>9, &mdev->ap_in_flight);
		}
		_req_may_be_done(req, m); /* Allowed while state.susp */
		break;

	case data_received:
		D_ASSERT(req->rq_state & RQ_NET_PENDING);
		dec_ap_pending(mdev);
		req->rq_state &= ~RQ_NET_PENDING;
		req->rq_state |= (RQ_NET_OK|RQ_NET_DONE);
		_req_may_be_done_not_susp(req, m);
=======
			/* barrier came in before all requests were acked.
			 * this is bad, because if the connection is lost now,
			 * we won't be able to clean them up... */
			dev_err(DEV, "FIXME (BARRIER_ACKED but pending)\n");
		}
		/* Allowed to complete requests, even while suspended.
		 * As this is called for all requests within a matching epoch,
		 * we need to filter, and only set RQ_NET_DONE for those that
		 * have actually been on the wire. */
		mod_rq_state(req, m, RQ_COMPLETION_SUSP,
				(req->rq_state & RQ_NET_MASK) ? RQ_NET_DONE : 0);
		break;

	case DATA_RECEIVED:
		D_ASSERT(req->rq_state & RQ_NET_PENDING);
		mod_rq_state(req, m, RQ_NET_PENDING, RQ_NET_OK|RQ_NET_DONE);
		break;

	case QUEUE_AS_DRBD_BARRIER:
		start_new_tl_epoch(mdev->tconn);
		mod_rq_state(req, m, 0, RQ_NET_OK|RQ_NET_DONE);
>>>>>>> refs/remotes/origin/master
		break;
	};

	return rv;
}

/* we may do a local read if:
 * - we are consistent (of course),
 * - or we are generally inconsistent,
 *   BUT we are still/already IN SYNC for this area.
 *   since size may be bigger than BM_BLOCK_SIZE,
 *   we may need to check several bits.
 */
<<<<<<< HEAD
static int drbd_may_do_local_read(struct drbd_conf *mdev, sector_t sector, int size)
=======
static bool drbd_may_do_local_read(struct drbd_conf *mdev, sector_t sector, int size)
>>>>>>> refs/remotes/origin/master
{
	unsigned long sbnr, ebnr;
	sector_t esector, nr_sectors;

	if (mdev->state.disk == D_UP_TO_DATE)
<<<<<<< HEAD
		return 1;
	if (mdev->state.disk >= D_OUTDATED)
		return 0;
	if (mdev->state.disk <  D_INCONSISTENT)
		return 0;
	/* state.disk == D_INCONSISTENT   We will have a look at the BitMap */
	nr_sectors = drbd_get_capacity(mdev->this_bdev);
	esector = sector + (size >> 9) - 1;

=======
		return true;
	if (mdev->state.disk != D_INCONSISTENT)
		return false;
	esector = sector + (size >> 9) - 1;
	nr_sectors = drbd_get_capacity(mdev->this_bdev);
>>>>>>> refs/remotes/origin/master
	D_ASSERT(sector  < nr_sectors);
	D_ASSERT(esector < nr_sectors);

	sbnr = BM_SECT_TO_BIT(sector);
	ebnr = BM_SECT_TO_BIT(esector);

<<<<<<< HEAD
	return 0 == drbd_bm_count_bits(mdev, sbnr, ebnr);
}

static int drbd_make_request_common(struct drbd_conf *mdev, struct bio *bio, unsigned long start_time)
{
	const int rw = bio_rw(bio);
	const int size = bio->bi_size;
	const sector_t sector = bio->bi_sector;
	struct drbd_tl_epoch *b = NULL;
	struct drbd_request *req;
	int local, remote, send_oos = 0;
	int err = -EIO;
	int ret = 0;
=======
	return drbd_bm_count_bits(mdev, sbnr, ebnr) == 0;
}

static bool remote_due_to_read_balancing(struct drbd_conf *mdev, sector_t sector,
		enum drbd_read_balancing rbm)
{
	struct backing_dev_info *bdi;
	int stripe_shift;

	switch (rbm) {
	case RB_CONGESTED_REMOTE:
		bdi = &mdev->ldev->backing_bdev->bd_disk->queue->backing_dev_info;
		return bdi_read_congested(bdi);
	case RB_LEAST_PENDING:
		return atomic_read(&mdev->local_cnt) >
			atomic_read(&mdev->ap_pending_cnt) + atomic_read(&mdev->rs_pending_cnt);
	case RB_32K_STRIPING:  /* stripe_shift = 15 */
	case RB_64K_STRIPING:
	case RB_128K_STRIPING:
	case RB_256K_STRIPING:
	case RB_512K_STRIPING:
	case RB_1M_STRIPING:   /* stripe_shift = 20 */
		stripe_shift = (rbm - RB_32K_STRIPING + 15);
		return (sector >> (stripe_shift - 9)) & 1;
	case RB_ROUND_ROBIN:
		return test_and_change_bit(READ_BALANCE_RR, &mdev->flags);
	case RB_PREFER_REMOTE:
		return true;
	case RB_PREFER_LOCAL:
	default:
		return false;
	}
}

/*
 * complete_conflicting_writes  -  wait for any conflicting write requests
 *
 * The write_requests tree contains all active write requests which we
 * currently know about.  Wait for any requests to complete which conflict with
 * the new one.
 *
 * Only way out: remove the conflicting intervals from the tree.
 */
static void complete_conflicting_writes(struct drbd_request *req)
{
	DEFINE_WAIT(wait);
	struct drbd_conf *mdev = req->w.mdev;
	struct drbd_interval *i;
	sector_t sector = req->i.sector;
	int size = req->i.size;

	i = drbd_find_overlap(&mdev->write_requests, sector, size);
	if (!i)
		return;

	for (;;) {
		prepare_to_wait(&mdev->misc_wait, &wait, TASK_UNINTERRUPTIBLE);
		i = drbd_find_overlap(&mdev->write_requests, sector, size);
		if (!i)
			break;
		/* Indicate to wake up device->misc_wait on progress.  */
		i->waiting = true;
		spin_unlock_irq(&mdev->tconn->req_lock);
		schedule();
		spin_lock_irq(&mdev->tconn->req_lock);
	}
	finish_wait(&mdev->misc_wait, &wait);
}

/* called within req_lock and rcu_read_lock() */
static void maybe_pull_ahead(struct drbd_conf *mdev)
{
	struct drbd_tconn *tconn = mdev->tconn;
	struct net_conf *nc;
	bool congested = false;
	enum drbd_on_congestion on_congestion;

	rcu_read_lock();
	nc = rcu_dereference(tconn->net_conf);
	on_congestion = nc ? nc->on_congestion : OC_BLOCK;
	rcu_read_unlock();
	if (on_congestion == OC_BLOCK ||
	    tconn->agreed_pro_version < 96)
		return;

	/* If I don't even have good local storage, we can not reasonably try
	 * to pull ahead of the peer. We also need the local reference to make
	 * sure mdev->act_log is there.
	 */
	if (!get_ldev_if_state(mdev, D_UP_TO_DATE))
		return;

	if (nc->cong_fill &&
	    atomic_read(&mdev->ap_in_flight) >= nc->cong_fill) {
		dev_info(DEV, "Congestion-fill threshold reached\n");
		congested = true;
	}

	if (mdev->act_log->used >= nc->cong_extents) {
		dev_info(DEV, "Congestion-extents threshold reached\n");
		congested = true;
	}

	if (congested) {
		/* start a new epoch for non-mirrored writes */
		start_new_tl_epoch(mdev->tconn);

		if (on_congestion == OC_PULL_AHEAD)
			_drbd_set_state(_NS(mdev, conn, C_AHEAD), 0, NULL);
		else  /*nc->on_congestion == OC_DISCONNECT */
			_drbd_set_state(_NS(mdev, conn, C_DISCONNECTING), 0, NULL);
	}
	put_ldev(mdev);
}

/* If this returns false, and req->private_bio is still set,
 * this should be submitted locally.
 *
 * If it returns false, but req->private_bio is not set,
 * we do not have access to good data :(
 *
 * Otherwise, this destroys req->private_bio, if any,
 * and returns true.
 */
static bool do_remote_read(struct drbd_request *req)
{
	struct drbd_conf *mdev = req->w.mdev;
	enum drbd_read_balancing rbm;

	if (req->private_bio) {
		if (!drbd_may_do_local_read(mdev,
					req->i.sector, req->i.size)) {
			bio_put(req->private_bio);
			req->private_bio = NULL;
			put_ldev(mdev);
		}
	}

	if (mdev->state.pdsk != D_UP_TO_DATE)
		return false;

	if (req->private_bio == NULL)
		return true;

	/* TODO: improve read balancing decisions, take into account drbd
	 * protocol, pending requests etc. */

	rcu_read_lock();
	rbm = rcu_dereference(mdev->ldev->disk_conf)->read_balancing;
	rcu_read_unlock();

	if (rbm == RB_PREFER_LOCAL && req->private_bio)
		return false; /* submit locally */

	if (remote_due_to_read_balancing(mdev, req->i.sector, rbm)) {
		if (req->private_bio) {
			bio_put(req->private_bio);
			req->private_bio = NULL;
			put_ldev(mdev);
		}
		return true;
	}

	return false;
}

/* returns number of connections (== 1, for drbd 8.4)
 * expected to actually write this data,
 * which does NOT include those that we are L_AHEAD for. */
static int drbd_process_write_request(struct drbd_request *req)
{
	struct drbd_conf *mdev = req->w.mdev;
	int remote, send_oos;

	remote = drbd_should_do_remote(mdev->state);
	send_oos = drbd_should_send_out_of_sync(mdev->state);

	/* Need to replicate writes.  Unless it is an empty flush,
	 * which is better mapped to a DRBD P_BARRIER packet,
	 * also for drbd wire protocol compatibility reasons.
	 * If this was a flush, just start a new epoch.
	 * Unless the current epoch was empty anyways, or we are not currently
	 * replicating, in which case there is no point. */
	if (unlikely(req->i.size == 0)) {
		/* The only size==0 bios we expect are empty flushes. */
		D_ASSERT(req->master_bio->bi_rw & REQ_FLUSH);
		if (remote)
			_req_mod(req, QUEUE_AS_DRBD_BARRIER);
		return remote;
	}

	if (!remote && !send_oos)
		return 0;

	D_ASSERT(!(remote && send_oos));

	if (remote) {
		_req_mod(req, TO_BE_SENT);
		_req_mod(req, QUEUE_FOR_NET_WRITE);
	} else if (drbd_set_out_of_sync(mdev, req->i.sector, req->i.size))
		_req_mod(req, QUEUE_FOR_SEND_OOS);

	return remote;
}

static void
drbd_submit_req_private_bio(struct drbd_request *req)
{
	struct drbd_conf *mdev = req->w.mdev;
	struct bio *bio = req->private_bio;
	const int rw = bio_rw(bio);

	bio->bi_bdev = mdev->ldev->backing_bdev;

	/* State may have changed since we grabbed our reference on the
	 * ->ldev member. Double check, and short-circuit to endio.
	 * In case the last activity log transaction failed to get on
	 * stable storage, and this is a WRITE, we may not even submit
	 * this bio. */
	if (get_ldev(mdev)) {
		if (drbd_insert_fault(mdev,
				      rw == WRITE ? DRBD_FAULT_DT_WR
				    : rw == READ  ? DRBD_FAULT_DT_RD
				    :               DRBD_FAULT_DT_RA))
			bio_endio(bio, -EIO);
		else
			generic_make_request(bio);
		put_ldev(mdev);
	} else
		bio_endio(bio, -EIO);
}

static void drbd_queue_write(struct drbd_conf *mdev, struct drbd_request *req)
{
	spin_lock(&mdev->submit.lock);
	list_add_tail(&req->tl_requests, &mdev->submit.writes);
	spin_unlock(&mdev->submit.lock);
	queue_work(mdev->submit.wq, &mdev->submit.worker);
}

/* returns the new drbd_request pointer, if the caller is expected to
 * drbd_send_and_submit() it (to save latency), or NULL if we queued the
 * request on the submitter thread.
 * Returns ERR_PTR(-ENOMEM) if we cannot allocate a drbd_request.
 */
struct drbd_request *
drbd_request_prepare(struct drbd_conf *mdev, struct bio *bio, unsigned long start_time)
{
	const int rw = bio_data_dir(bio);
	struct drbd_request *req;
>>>>>>> refs/remotes/origin/master

	/* allocate outside of all locks; */
	req = drbd_req_new(mdev, bio);
	if (!req) {
		dec_ap_bio(mdev);
		/* only pass the error to the upper layers.
		 * if user cannot handle io errors, that's not our business. */
		dev_err(DEV, "could not kmalloc() req\n");
		bio_endio(bio, -ENOMEM);
<<<<<<< HEAD
		return 0;
	}
	req->start_time = start_time;

	local = get_ldev(mdev);
	if (!local) {
		bio_put(req->private_bio); /* or we get a bio leak */
		req->private_bio = NULL;
	}
	if (rw == WRITE) {
		remote = 1;
	} else {
		/* READ || READA */
		if (local) {
			if (!drbd_may_do_local_read(mdev, sector, size)) {
				/* we could kick the syncer to
				 * sync this extent asap, wait for
				 * it, then continue locally.
				 * Or just issue the request remotely.
				 */
				local = 0;
				bio_put(req->private_bio);
				req->private_bio = NULL;
				put_ldev(mdev);
			}
		}
		remote = !local && mdev->state.pdsk >= D_UP_TO_DATE;
	}

	/* If we have a disk, but a READA request is mapped to remote,
	 * we are R_PRIMARY, D_INCONSISTENT, SyncTarget.
	 * Just fail that READA request right here.
	 *
	 * THINK: maybe fail all READA when not local?
	 *        or make this configurable...
	 *        if network is slow, READA won't do any good.
	 */
	if (rw == READA && mdev->state.disk >= D_INCONSISTENT && !local) {
		err = -EWOULDBLOCK;
		goto fail_and_free_req;
	}

	/* For WRITES going to the local disk, grab a reference on the target
	 * extent.  This waits for any resync activity in the corresponding
	 * resync extent to finish, and, if necessary, pulls in the target
	 * extent into the activity log, which involves further disk io because
	 * of transactional on-disk meta data updates. */
	if (rw == WRITE && local && !test_bit(AL_SUSPENDED, &mdev->flags)) {
		req->rq_state |= RQ_IN_ACT_LOG;
		drbd_al_begin_io(mdev, sector);
	}

	remote = remote && drbd_should_do_remote(mdev->state);
	send_oos = rw == WRITE && drbd_should_send_oos(mdev->state);
	D_ASSERT(!(remote && send_oos));

	if (!(local || remote) && !is_susp(mdev->state)) {
		if (__ratelimit(&drbd_ratelimit_state))
			dev_err(DEV, "IO ERROR: neither local nor remote disk\n");
		goto fail_free_complete;
	}

	/* For WRITE request, we have to make sure that we have an
	 * unused_spare_tle, in case we need to start a new epoch.
	 * I try to be smart and avoid to pre-allocate always "just in case",
	 * but there is a race between testing the bit and pointer outside the
	 * spinlock, and grabbing the spinlock.
	 * if we lost that race, we retry.  */
	if (rw == WRITE && (remote || send_oos) &&
	    mdev->unused_spare_tle == NULL &&
	    test_bit(CREATE_BARRIER, &mdev->flags)) {
allocate_barrier:
		b = kmalloc(sizeof(struct drbd_tl_epoch), GFP_NOIO);
		if (!b) {
			dev_err(DEV, "Failed to alloc barrier.\n");
			err = -ENOMEM;
			goto fail_free_complete;
		}
	}

	/* GOOD, everything prepared, grab the spin_lock */
	spin_lock_irq(&mdev->req_lock);

	if (is_susp(mdev->state)) {
		/* If we got suspended, use the retry mechanism of
		   generic_make_request() to restart processing of this
		   bio. In the next call to drbd_make_request
		   we sleep in inc_ap_bio() */
		ret = 1;
		spin_unlock_irq(&mdev->req_lock);
		goto fail_free_complete;
	}

	if (remote || send_oos) {
		remote = drbd_should_do_remote(mdev->state);
		send_oos = rw == WRITE && drbd_should_send_oos(mdev->state);
		D_ASSERT(!(remote && send_oos));

		if (!(remote || send_oos))
			dev_warn(DEV, "lost connection while grabbing the req_lock!\n");
		if (!(local || remote)) {
			dev_err(DEV, "IO ERROR: neither local nor remote disk\n");
			spin_unlock_irq(&mdev->req_lock);
			goto fail_free_complete;
		}
	}

	if (b && mdev->unused_spare_tle == NULL) {
		mdev->unused_spare_tle = b;
		b = NULL;
	}
	if (rw == WRITE && (remote || send_oos) &&
	    mdev->unused_spare_tle == NULL &&
	    test_bit(CREATE_BARRIER, &mdev->flags)) {
		/* someone closed the current epoch
		 * while we were grabbing the spinlock */
		spin_unlock_irq(&mdev->req_lock);
		goto allocate_barrier;
	}


	/* Update disk stats */
	_drbd_start_io_acct(mdev, req, bio);

	/* _maybe_start_new_epoch(mdev);
	 * If we need to generate a write barrier packet, we have to add the
	 * new epoch (barrier) object, and queue the barrier packet for sending,
	 * and queue the req's data after it _within the same lock_, otherwise
	 * we have race conditions were the reorder domains could be mixed up.
	 *
	 * Even read requests may start a new epoch and queue the corresponding
	 * barrier packet.  To get the write ordering right, we only have to
	 * make sure that, if this is a write request and it triggered a
	 * barrier packet, this request is queued within the same spinlock. */
	if ((remote || send_oos) && mdev->unused_spare_tle &&
	    test_and_clear_bit(CREATE_BARRIER, &mdev->flags)) {
		_tl_add_barrier(mdev, mdev->unused_spare_tle);
		mdev->unused_spare_tle = NULL;
	} else {
		D_ASSERT(!(remote && rw == WRITE &&
			   test_bit(CREATE_BARRIER, &mdev->flags)));
	}

	/* NOTE
	 * Actually, 'local' may be wrong here already, since we may have failed
	 * to write to the meta data, and may become wrong anytime because of
	 * local io-error for some other request, which would lead to us
	 * "detaching" the local disk.
	 *
	 * 'remote' may become wrong any time because the network could fail.
	 *
	 * This is a harmless race condition, though, since it is handled
	 * correctly at the appropriate places; so it just defers the failure
	 * of the respective operation.
	 */

	/* mark them early for readability.
	 * this just sets some state flags. */
	if (remote)
		_req_mod(req, to_be_send);
	if (local)
		_req_mod(req, to_be_submitted);

	/* check this request on the collision detection hash tables.
	 * if we have a conflict, just complete it here.
	 * THINK do we want to check reads, too? (I don't think so...) */
	if (rw == WRITE && _req_conflicts(req))
		goto fail_conflicting;

	list_add_tail(&req->tl_requests, &mdev->newest_tle->requests);

	/* NOTE remote first: to get the concurrent write detection right,
	 * we must register the request before start of local IO.  */
	if (remote) {
		/* either WRITE and C_CONNECTED,
		 * or READ, and no local disk,
		 * or READ, but not in sync.
		 */
		_req_mod(req, (rw == WRITE)
				? queue_for_net_write
				: queue_for_net_read);
	}
	if (send_oos && drbd_set_out_of_sync(mdev, sector, size))
		_req_mod(req, queue_for_send_oos);

	if (remote &&
	    mdev->net_conf->on_congestion != OC_BLOCK && mdev->agreed_pro_version >= 96) {
		int congested = 0;

		if (mdev->net_conf->cong_fill &&
		    atomic_read(&mdev->ap_in_flight) >= mdev->net_conf->cong_fill) {
			dev_info(DEV, "Congestion-fill threshold reached\n");
			congested = 1;
		}

		if (mdev->act_log->used >= mdev->net_conf->cong_extents) {
			dev_info(DEV, "Congestion-extents threshold reached\n");
			congested = 1;
		}

		if (congested) {
			queue_barrier(mdev); /* last barrier, after mirrored writes */

			if (mdev->net_conf->on_congestion == OC_PULL_AHEAD)
				_drbd_set_state(_NS(mdev, conn, C_AHEAD), 0, NULL);
			else  /*mdev->net_conf->on_congestion == OC_DISCONNECT */
				_drbd_set_state(_NS(mdev, conn, C_DISCONNECTING), 0, NULL);
		}
	}

	spin_unlock_irq(&mdev->req_lock);
	kfree(b); /* if someone else has beaten us to it... */

	if (local) {
		req->private_bio->bi_bdev = mdev->ldev->backing_bdev;

		/* State may have changed since we grabbed our reference on the
		 * mdev->ldev member. Double check, and short-circuit to endio.
		 * In case the last activity log transaction failed to get on
		 * stable storage, and this is a WRITE, we may not even submit
		 * this bio. */
		if (get_ldev(mdev)) {
			if (drbd_insert_fault(mdev,   rw == WRITE ? DRBD_FAULT_DT_WR
						    : rw == READ  ? DRBD_FAULT_DT_RD
						    :               DRBD_FAULT_DT_RA))
				bio_endio(req->private_bio, -EIO);
			else
				generic_make_request(req->private_bio);
			put_ldev(mdev);
		} else
			bio_endio(req->private_bio, -EIO);
	}

	return 0;

fail_conflicting:
	/* this is a conflicting request.
	 * even though it may have been only _partially_
	 * overlapping with one of the currently pending requests,
	 * without even submitting or sending it, we will
	 * pretend that it was successfully served right now.
	 */
	_drbd_end_io_acct(mdev, req);
	spin_unlock_irq(&mdev->req_lock);
	if (remote)
		dec_ap_pending(mdev);
	/* THINK: do we want to fail it (-EIO), or pretend success?
	 * this pretends success. */
	err = 0;

fail_free_complete:
	if (req->rq_state & RQ_IN_ACT_LOG)
		drbd_al_complete_io(mdev, sector);
fail_and_free_req:
	if (local) {
		bio_put(req->private_bio);
		req->private_bio = NULL;
		put_ldev(mdev);
	}
	if (!ret)
		bio_endio(bio, err);

	drbd_req_free(req);
	dec_ap_bio(mdev);
	kfree(b);

	return ret;
}

/* helper function for drbd_make_request
 * if we can determine just by the mdev (state) that this request will fail,
 * return 1
 * otherwise return 0
 */
static int drbd_fail_request_early(struct drbd_conf *mdev, int is_write)
{
	if (mdev->state.role != R_PRIMARY &&
		(!allow_oos || is_write)) {
		if (__ratelimit(&drbd_ratelimit_state)) {
			dev_err(DEV, "Process %s[%u] tried to %s; "
			    "since we are not in Primary state, "
			    "we cannot allow this\n",
			    current->comm, current->pid,
			    is_write ? "WRITE" : "READ");
		}
		return 1;
	}

	return 0;
}

<<<<<<< HEAD
int drbd_make_request(struct request_queue *q, struct bio *bio)
=======
void drbd_make_request(struct request_queue *q, struct bio *bio)
>>>>>>> refs/remotes/origin/cm-10.0
{
	unsigned int s_enr, e_enr;
	struct drbd_conf *mdev = (struct drbd_conf *) q->queuedata;
	unsigned long start_time;

	if (drbd_fail_request_early(mdev, bio_data_dir(bio) & WRITE)) {
		bio_endio(bio, -EPERM);
<<<<<<< HEAD
		return 0;
=======
		return;
>>>>>>> refs/remotes/origin/cm-10.0
	}

=======
		return ERR_PTR(-ENOMEM);
	}
	req->start_time = start_time;

	if (!get_ldev(mdev)) {
		bio_put(req->private_bio);
		req->private_bio = NULL;
	}

	/* Update disk stats */
	_drbd_start_io_acct(mdev, req);

	if (rw == WRITE && req->private_bio && req->i.size
	&& !test_bit(AL_SUSPENDED, &mdev->flags)) {
		if (!drbd_al_begin_io_fastpath(mdev, &req->i)) {
			drbd_queue_write(mdev, req);
			return NULL;
		}
		req->rq_state |= RQ_IN_ACT_LOG;
	}

	return req;
}

static void drbd_send_and_submit(struct drbd_conf *mdev, struct drbd_request *req)
{
	const int rw = bio_rw(req->master_bio);
	struct bio_and_error m = { NULL, };
	bool no_remote = false;

	spin_lock_irq(&mdev->tconn->req_lock);
	if (rw == WRITE) {
		/* This may temporarily give up the req_lock,
		 * but will re-aquire it before it returns here.
		 * Needs to be before the check on drbd_suspended() */
		complete_conflicting_writes(req);
		/* no more giving up req_lock from now on! */

		/* check for congestion, and potentially stop sending
		 * full data updates, but start sending "dirty bits" only. */
		maybe_pull_ahead(mdev);
	}


	if (drbd_suspended(mdev)) {
		/* push back and retry: */
		req->rq_state |= RQ_POSTPONED;
		if (req->private_bio) {
			bio_put(req->private_bio);
			req->private_bio = NULL;
			put_ldev(mdev);
		}
		goto out;
	}

	/* We fail READ/READA early, if we can not serve it.
	 * We must do this before req is registered on any lists.
	 * Otherwise, drbd_req_complete() will queue failed READ for retry. */
	if (rw != WRITE) {
		if (!do_remote_read(req) && !req->private_bio)
			goto nodata;
	}

	/* which transfer log epoch does this belong to? */
	req->epoch = atomic_read(&mdev->tconn->current_tle_nr);

	/* no point in adding empty flushes to the transfer log,
	 * they are mapped to drbd barriers already. */
	if (likely(req->i.size!=0)) {
		if (rw == WRITE)
			mdev->tconn->current_tle_writes++;

		list_add_tail(&req->tl_requests, &mdev->tconn->transfer_log);
	}

	if (rw == WRITE) {
		if (!drbd_process_write_request(req))
			no_remote = true;
	} else {
		/* We either have a private_bio, or we can read from remote.
		 * Otherwise we had done the goto nodata above. */
		if (req->private_bio == NULL) {
			_req_mod(req, TO_BE_SENT);
			_req_mod(req, QUEUE_FOR_NET_READ);
		} else
			no_remote = true;
	}

	if (req->private_bio) {
		/* needs to be marked within the same spinlock */
		_req_mod(req, TO_BE_SUBMITTED);
		/* but we need to give up the spinlock to submit */
		spin_unlock_irq(&mdev->tconn->req_lock);
		drbd_submit_req_private_bio(req);
		spin_lock_irq(&mdev->tconn->req_lock);
	} else if (no_remote) {
nodata:
		if (__ratelimit(&drbd_ratelimit_state))
			dev_err(DEV, "IO ERROR: neither local nor remote data, sector %llu+%u\n",
					(unsigned long long)req->i.sector, req->i.size >> 9);
		/* A write may have been queued for send_oos, however.
		 * So we can not simply free it, we must go through drbd_req_put_completion_ref() */
	}

out:
	if (drbd_req_put_completion_ref(req, &m, 1))
		kref_put(&req->kref, drbd_req_destroy);
	spin_unlock_irq(&mdev->tconn->req_lock);

	if (m.bio)
		complete_master_bio(mdev, &m);
}

void __drbd_make_request(struct drbd_conf *mdev, struct bio *bio, unsigned long start_time)
{
	struct drbd_request *req = drbd_request_prepare(mdev, bio, start_time);
	if (IS_ERR_OR_NULL(req))
		return;
	drbd_send_and_submit(mdev, req);
}

static void submit_fast_path(struct drbd_conf *mdev, struct list_head *incoming)
{
	struct drbd_request *req, *tmp;
	list_for_each_entry_safe(req, tmp, incoming, tl_requests) {
		const int rw = bio_data_dir(req->master_bio);

		if (rw == WRITE /* rw != WRITE should not even end up here! */
		&& req->private_bio && req->i.size
		&& !test_bit(AL_SUSPENDED, &mdev->flags)) {
			if (!drbd_al_begin_io_fastpath(mdev, &req->i))
				continue;

			req->rq_state |= RQ_IN_ACT_LOG;
		}

		list_del_init(&req->tl_requests);
		drbd_send_and_submit(mdev, req);
	}
}

static bool prepare_al_transaction_nonblock(struct drbd_conf *mdev,
					    struct list_head *incoming,
					    struct list_head *pending)
{
	struct drbd_request *req, *tmp;
	int wake = 0;
	int err;

	spin_lock_irq(&mdev->al_lock);
	list_for_each_entry_safe(req, tmp, incoming, tl_requests) {
		err = drbd_al_begin_io_nonblock(mdev, &req->i);
		if (err == -EBUSY)
			wake = 1;
		if (err)
			continue;
		req->rq_state |= RQ_IN_ACT_LOG;
		list_move_tail(&req->tl_requests, pending);
	}
	spin_unlock_irq(&mdev->al_lock);
	if (wake)
		wake_up(&mdev->al_wait);

	return !list_empty(pending);
}

void do_submit(struct work_struct *ws)
{
	struct drbd_conf *mdev = container_of(ws, struct drbd_conf, submit.worker);
	LIST_HEAD(incoming);
	LIST_HEAD(pending);
	struct drbd_request *req, *tmp;

	for (;;) {
		spin_lock(&mdev->submit.lock);
		list_splice_tail_init(&mdev->submit.writes, &incoming);
		spin_unlock(&mdev->submit.lock);

		submit_fast_path(mdev, &incoming);
		if (list_empty(&incoming))
			break;

		wait_event(mdev->al_wait, prepare_al_transaction_nonblock(mdev, &incoming, &pending));
		/* Maybe more was queued, while we prepared the transaction?
		 * Try to stuff them into this transaction as well.
		 * Be strictly non-blocking here, no wait_event, we already
		 * have something to commit.
		 * Stop if we don't make any more progres.
		 */
		for (;;) {
			LIST_HEAD(more_pending);
			LIST_HEAD(more_incoming);
			bool made_progress;

			/* It is ok to look outside the lock,
			 * it's only an optimization anyways */
			if (list_empty(&mdev->submit.writes))
				break;

			spin_lock(&mdev->submit.lock);
			list_splice_tail_init(&mdev->submit.writes, &more_incoming);
			spin_unlock(&mdev->submit.lock);

			if (list_empty(&more_incoming))
				break;

			made_progress = prepare_al_transaction_nonblock(mdev, &more_incoming, &more_pending);

			list_splice_tail_init(&more_pending, &pending);
			list_splice_tail_init(&more_incoming, &incoming);

			if (!made_progress)
				break;
		}
		drbd_al_begin_io_commit(mdev, false);

		list_for_each_entry_safe(req, tmp, &pending, tl_requests) {
			list_del_init(&req->tl_requests);
			drbd_send_and_submit(mdev, req);
		}
	}
}

void drbd_make_request(struct request_queue *q, struct bio *bio)
{
	struct drbd_conf *mdev = (struct drbd_conf *) q->queuedata;
	unsigned long start_time;

>>>>>>> refs/remotes/origin/master
	start_time = jiffies;

	/*
	 * what we "blindly" assume:
	 */
<<<<<<< HEAD
	D_ASSERT(bio->bi_size > 0);
	D_ASSERT((bio->bi_size & 0x1ff) == 0);
	D_ASSERT(bio->bi_idx == 0);

	/* to make some things easier, force alignment of requests within the
	 * granularity of our hash tables */
	s_enr = bio->bi_sector >> HT_SHIFT;
	e_enr = (bio->bi_sector+(bio->bi_size>>9)-1) >> HT_SHIFT;

	if (likely(s_enr == e_enr)) {
		inc_ap_bio(mdev, 1);
<<<<<<< HEAD
		return drbd_make_request_common(mdev, bio, start_time);
=======
		drbd_make_request_common(mdev, bio, start_time);
		return;
>>>>>>> refs/remotes/origin/cm-10.0
	}

	/* can this bio be split generically?
	 * Maybe add our own split-arbitrary-bios function. */
	if (bio->bi_vcnt != 1 || bio->bi_idx != 0 || bio->bi_size > DRBD_MAX_BIO_SIZE) {
		/* rather error out here than BUG in bio_split */
		dev_err(DEV, "bio would need to, but cannot, be split: "
		    "(vcnt=%u,idx=%u,size=%u,sector=%llu)\n",
		    bio->bi_vcnt, bio->bi_idx, bio->bi_size,
		    (unsigned long long)bio->bi_sector);
		bio_endio(bio, -EINVAL);
	} else {
		/* This bio crosses some boundary, so we have to split it. */
		struct bio_pair *bp;
		/* works for the "do not cross hash slot boundaries" case
		 * e.g. sector 262269, size 4096
		 * s_enr = 262269 >> 6 = 4097
		 * e_enr = (262269+8-1) >> 6 = 4098
		 * HT_SHIFT = 6
		 * sps = 64, mask = 63
		 * first_sectors = 64 - (262269 & 63) = 3
		 */
		const sector_t sect = bio->bi_sector;
		const int sps = 1 << HT_SHIFT; /* sectors per slot */
		const int mask = sps - 1;
		const sector_t first_sectors = sps - (sect & mask);
		bp = bio_split(bio, first_sectors);

		/* we need to get a "reference count" (ap_bio_cnt)
		 * to avoid races with the disconnect/reconnect/suspend code.
		 * In case we need to split the bio here, we need to get three references
		 * atomically, otherwise we might deadlock when trying to submit the
		 * second one! */
		inc_ap_bio(mdev, 3);

		D_ASSERT(e_enr == s_enr + 1);

		while (drbd_make_request_common(mdev, &bp->bio1, start_time))
			inc_ap_bio(mdev, 1);

		while (drbd_make_request_common(mdev, &bp->bio2, start_time))
			inc_ap_bio(mdev, 1);

		dec_ap_bio(mdev);

		bio_pair_release(bp);
	}
<<<<<<< HEAD
	return 0;
=======
>>>>>>> refs/remotes/origin/cm-10.0
}

/* This is called by bio_add_page().  With this function we reduce
 * the number of BIOs that span over multiple DRBD_MAX_BIO_SIZEs
 * units (was AL_EXTENTs).
 *
 * we do the calculation within the lower 32bit of the byte offsets,
 * since we don't care for actual offset, but only check whether it
 * would cross "activity log extent" boundaries.
 *
 * As long as the BIO is empty we have to allow at least one bvec,
 * regardless of size and offset.  so the resulting bio may still
 * cross extent boundaries.  those are dealt with (bio_split) in
 * drbd_make_request.
=======
	D_ASSERT(IS_ALIGNED(bio->bi_size, 512));

	inc_ap_bio(mdev);
	__drbd_make_request(mdev, bio, start_time);
}

/* This is called by bio_add_page().
 *
 * q->max_hw_sectors and other global limits are already enforced there.
 *
 * We need to call down to our lower level device,
 * in case it has special restrictions.
 *
 * We also may need to enforce configured max-bio-bvecs limits.
 *
 * As long as the BIO is empty we have to allow at least one bvec,
 * regardless of size and offset, so no need to ask lower levels.
>>>>>>> refs/remotes/origin/master
 */
int drbd_merge_bvec(struct request_queue *q, struct bvec_merge_data *bvm, struct bio_vec *bvec)
{
	struct drbd_conf *mdev = (struct drbd_conf *) q->queuedata;
<<<<<<< HEAD
	unsigned int bio_offset =
		(unsigned int)bvm->bi_sector << 9; /* 32 bit */
	unsigned int bio_size = bvm->bi_size;
	int limit, backing_limit;

	limit = DRBD_MAX_BIO_SIZE
	      - ((bio_offset & (DRBD_MAX_BIO_SIZE-1)) + bio_size);
	if (limit < 0)
		limit = 0;
	if (bio_size == 0) {
		if (limit <= bvec->bv_len)
			limit = bvec->bv_len;
	} else if (limit && get_ldev(mdev)) {
=======
	unsigned int bio_size = bvm->bi_size;
	int limit = DRBD_MAX_BIO_SIZE;
	int backing_limit;

	if (bio_size && get_ldev(mdev)) {
		unsigned int max_hw_sectors = queue_max_hw_sectors(q);
>>>>>>> refs/remotes/origin/master
		struct request_queue * const b =
			mdev->ldev->backing_bdev->bd_disk->queue;
		if (b->merge_bvec_fn) {
			backing_limit = b->merge_bvec_fn(b, bvm, bvec);
			limit = min(limit, backing_limit);
		}
		put_ldev(mdev);
<<<<<<< HEAD
=======
		if ((limit >> 9) > max_hw_sectors)
			limit = max_hw_sectors << 9;
>>>>>>> refs/remotes/origin/master
	}
	return limit;
}

<<<<<<< HEAD
void request_timer_fn(unsigned long data)
{
	struct drbd_conf *mdev = (struct drbd_conf *) data;
	struct drbd_request *req; /* oldest request */
	struct list_head *le;
	unsigned long et = 0; /* effective timeout = ko_count * timeout */

	if (get_net_conf(mdev)) {
		et = mdev->net_conf->timeout*HZ/10 * mdev->net_conf->ko_count;
		put_net_conf(mdev);
	}
	if (!et || mdev->state.conn < C_WF_REPORT_PARAMS)
		return; /* Recurring timer stopped */

	spin_lock_irq(&mdev->req_lock);
	le = &mdev->oldest_tle->requests;
	if (list_empty(le)) {
		spin_unlock_irq(&mdev->req_lock);
		mod_timer(&mdev->request_timer, jiffies + et);
		return;
	}

	le = le->prev;
	req = list_entry(le, struct drbd_request, tl_requests);
	if (time_is_before_eq_jiffies(req->start_time + et)) {
		if (req->rq_state & RQ_NET_PENDING) {
			dev_warn(DEV, "Remote failed to finish a request within ko-count * timeout\n");
			_drbd_set_state(_NS(mdev, conn, C_TIMEOUT), CS_VERBOSE, NULL);
		} else {
			dev_warn(DEV, "Local backing block device frozen?\n");
			mod_timer(&mdev->request_timer, jiffies + et);
		}
	} else {
		mod_timer(&mdev->request_timer, req->start_time + et);
	}

	spin_unlock_irq(&mdev->req_lock);
=======
struct drbd_request *find_oldest_request(struct drbd_tconn *tconn)
{
	/* Walk the transfer log,
	 * and find the oldest not yet completed request */
	struct drbd_request *r;
	list_for_each_entry(r, &tconn->transfer_log, tl_requests) {
		if (atomic_read(&r->completion_ref))
			return r;
	}
	return NULL;
}

void request_timer_fn(unsigned long data)
{
	struct drbd_conf *mdev = (struct drbd_conf *) data;
	struct drbd_tconn *tconn = mdev->tconn;
	struct drbd_request *req; /* oldest request */
	struct net_conf *nc;
	unsigned long ent = 0, dt = 0, et, nt; /* effective timeout = ko_count * timeout */
	unsigned long now;

	rcu_read_lock();
	nc = rcu_dereference(tconn->net_conf);
	if (nc && mdev->state.conn >= C_WF_REPORT_PARAMS)
		ent = nc->timeout * HZ/10 * nc->ko_count;

	if (get_ldev(mdev)) { /* implicit state.disk >= D_INCONSISTENT */
		dt = rcu_dereference(mdev->ldev->disk_conf)->disk_timeout * HZ / 10;
		put_ldev(mdev);
	}
	rcu_read_unlock();

	et = min_not_zero(dt, ent);

	if (!et)
		return; /* Recurring timer stopped */

	now = jiffies;

	spin_lock_irq(&tconn->req_lock);
	req = find_oldest_request(tconn);
	if (!req) {
		spin_unlock_irq(&tconn->req_lock);
		mod_timer(&mdev->request_timer, now + et);
		return;
	}

	/* The request is considered timed out, if
	 * - we have some effective timeout from the configuration,
	 *   with above state restrictions applied,
	 * - the oldest request is waiting for a response from the network
	 *   resp. the local disk,
	 * - the oldest request is in fact older than the effective timeout,
	 * - the connection was established (resp. disk was attached)
	 *   for longer than the timeout already.
	 * Note that for 32bit jiffies and very stable connections/disks,
	 * we may have a wrap around, which is catched by
	 *   !time_in_range(now, last_..._jif, last_..._jif + timeout).
	 *
	 * Side effect: once per 32bit wrap-around interval, which means every
	 * ~198 days with 250 HZ, we have a window where the timeout would need
	 * to expire twice (worst case) to become effective. Good enough.
	 */
	if (ent && req->rq_state & RQ_NET_PENDING &&
		 time_after(now, req->start_time + ent) &&
		!time_in_range(now, tconn->last_reconnect_jif, tconn->last_reconnect_jif + ent)) {
		dev_warn(DEV, "Remote failed to finish a request within ko-count * timeout\n");
		_drbd_set_state(_NS(mdev, conn, C_TIMEOUT), CS_VERBOSE | CS_HARD, NULL);
	}
	if (dt && req->rq_state & RQ_LOCAL_PENDING && req->w.mdev == mdev &&
		 time_after(now, req->start_time + dt) &&
		!time_in_range(now, mdev->last_reattach_jif, mdev->last_reattach_jif + dt)) {
		dev_warn(DEV, "Local backing device failed to meet the disk-timeout\n");
		__drbd_chk_io_error(mdev, DRBD_FORCE_DETACH);
	}
	nt = (time_after(now, req->start_time + et) ? now : req->start_time) + et;
	spin_unlock_irq(&tconn->req_lock);
	mod_timer(&mdev->request_timer, nt);
>>>>>>> refs/remotes/origin/master
}
