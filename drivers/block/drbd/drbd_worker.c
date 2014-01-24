/*
   drbd_worker.c

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
#include <linux/drbd.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/mm.h>
#include <linux/memcontrol.h>
#include <linux/mm_inline.h>
#include <linux/slab.h>
#include <linux/random.h>
#include <linux/string.h>
#include <linux/scatterlist.h>

#include "drbd_int.h"
#include "drbd_req.h"

<<<<<<< HEAD
static int w_make_ov_request(struct drbd_conf *mdev, struct drbd_work *w, int cancel);
static int w_make_resync_request(struct drbd_conf *mdev,
				 struct drbd_work *w, int cancel);

=======
static int w_make_ov_request(struct drbd_work *w, int cancel);
>>>>>>> refs/remotes/origin/master


/* endio handlers:
 *   drbd_md_io_complete (defined here)
<<<<<<< HEAD
 *   drbd_endio_pri (defined here)
 *   drbd_endio_sec (defined here)
=======
 *   drbd_request_endio (defined here)
 *   drbd_peer_request_endio (defined here)
>>>>>>> refs/remotes/origin/master
 *   bm_async_io_complete (defined in drbd_bitmap.c)
 *
 * For all these callbacks, note the following:
 * The callbacks will be called in irq context by the IDE drivers,
 * and in Softirqs/Tasklets/BH context by the SCSI drivers.
 * Try to get the locking right :)
 *
 */


/* About the global_state_lock
   Each state transition on an device holds a read lock. In case we have
<<<<<<< HEAD
   to evaluate the sync after dependencies, we grab a write lock, because
=======
   to evaluate the resync after dependencies, we grab a write lock, because
>>>>>>> refs/remotes/origin/master
   we need stable states on all devices for that.  */
rwlock_t global_state_lock;

/* used for synchronous meta data and bitmap IO
 * submitted by drbd_md_sync_page_io()
 */
void drbd_md_io_complete(struct bio *bio, int error)
{
	struct drbd_md_io *md_io;
<<<<<<< HEAD

	md_io = (struct drbd_md_io *)bio->bi_private;
	md_io->error = error;

	complete(&md_io->event);
=======
	struct drbd_conf *mdev;

	md_io = (struct drbd_md_io *)bio->bi_private;
	mdev = container_of(md_io, struct drbd_conf, md_io);

	md_io->error = error;

	/* We grabbed an extra reference in _drbd_md_sync_page_io() to be able
	 * to timeout on the lower level device, and eventually detach from it.
	 * If this io completion runs after that timeout expired, this
	 * drbd_md_put_buffer() may allow us to finally try and re-attach.
	 * During normal operation, this only puts that extra reference
	 * down to 1 again.
	 * Make sure we first drop the reference, and only then signal
	 * completion, or we may (in drbd_al_read_log()) cycle so fast into the
	 * next drbd_md_sync_page_io(), that we trigger the
	 * ASSERT(atomic_read(&mdev->md_io_in_use) == 1) there.
	 */
	drbd_md_put_buffer(mdev);
	md_io->done = 1;
	wake_up(&mdev->misc_wait);
	bio_put(bio);
	if (mdev->ldev) /* special case: drbd_md_read() during drbd_adm_attach() */
		put_ldev(mdev);
>>>>>>> refs/remotes/origin/master
}

/* reads on behalf of the partner,
 * "submitted" by the receiver
 */
<<<<<<< HEAD
void drbd_endio_read_sec_final(struct drbd_epoch_entry *e) __releases(local)
{
	unsigned long flags = 0;
	struct drbd_conf *mdev = e->mdev;

	D_ASSERT(e->block_id != ID_VACANT);

	spin_lock_irqsave(&mdev->req_lock, flags);
	mdev->read_cnt += e->size >> 9;
	list_del(&e->w.list);
	if (list_empty(&mdev->read_ee))
		wake_up(&mdev->ee_wait);
	if (test_bit(__EE_WAS_ERROR, &e->flags))
		__drbd_chk_io_error(mdev, false);
	spin_unlock_irqrestore(&mdev->req_lock, flags);

	drbd_queue_work(&mdev->data.work, &e->w);
=======
void drbd_endio_read_sec_final(struct drbd_peer_request *peer_req) __releases(local)
{
	unsigned long flags = 0;
	struct drbd_conf *mdev = peer_req->w.mdev;

	spin_lock_irqsave(&mdev->tconn->req_lock, flags);
	mdev->read_cnt += peer_req->i.size >> 9;
	list_del(&peer_req->w.list);
	if (list_empty(&mdev->read_ee))
		wake_up(&mdev->ee_wait);
	if (test_bit(__EE_WAS_ERROR, &peer_req->flags))
		__drbd_chk_io_error(mdev, DRBD_READ_ERROR);
	spin_unlock_irqrestore(&mdev->tconn->req_lock, flags);

	drbd_queue_work(&mdev->tconn->sender_work, &peer_req->w);
>>>>>>> refs/remotes/origin/master
	put_ldev(mdev);
}

/* writes on behalf of the partner, or resync writes,
 * "submitted" by the receiver, final stage.  */
<<<<<<< HEAD
static void drbd_endio_write_sec_final(struct drbd_epoch_entry *e) __releases(local)
{
	unsigned long flags = 0;
	struct drbd_conf *mdev = e->mdev;
	sector_t e_sector;
	int do_wake;
	int is_syncer_req;
	int do_al_complete_io;

	D_ASSERT(e->block_id != ID_VACANT);

	/* after we moved e to done_ee,
	 * we may no longer access it,
	 * it may be freed/reused already!
	 * (as soon as we release the req_lock) */
	e_sector = e->sector;
	do_al_complete_io = e->flags & EE_CALL_AL_COMPLETE_IO;
	is_syncer_req = is_syncer_block_id(e->block_id);

	spin_lock_irqsave(&mdev->req_lock, flags);
	mdev->writ_cnt += e->size >> 9;
	list_del(&e->w.list); /* has been on active_ee or sync_ee */
	list_add_tail(&e->w.list, &mdev->done_ee);

	/* No hlist_del_init(&e->collision) here, we did not send the Ack yet,
	 * neither did we wake possibly waiting conflicting requests.
	 * done from "drbd_process_done_ee" within the appropriate w.cb
	 * (e_end_block/e_end_resync_block) or from _drbd_clear_done_ee */

	do_wake = is_syncer_req
		? list_empty(&mdev->sync_ee)
		: list_empty(&mdev->active_ee);

	if (test_bit(__EE_WAS_ERROR, &e->flags))
		__drbd_chk_io_error(mdev, false);
	spin_unlock_irqrestore(&mdev->req_lock, flags);

	if (is_syncer_req)
		drbd_rs_complete_io(mdev, e_sector);
=======
static void drbd_endio_write_sec_final(struct drbd_peer_request *peer_req) __releases(local)
{
	unsigned long flags = 0;
	struct drbd_conf *mdev = peer_req->w.mdev;
	struct drbd_interval i;
	int do_wake;
	u64 block_id;
	int do_al_complete_io;

	/* after we moved peer_req to done_ee,
	 * we may no longer access it,
	 * it may be freed/reused already!
	 * (as soon as we release the req_lock) */
	i = peer_req->i;
	do_al_complete_io = peer_req->flags & EE_CALL_AL_COMPLETE_IO;
	block_id = peer_req->block_id;

	spin_lock_irqsave(&mdev->tconn->req_lock, flags);
	mdev->writ_cnt += peer_req->i.size >> 9;
	list_move_tail(&peer_req->w.list, &mdev->done_ee);

	/*
	 * Do not remove from the write_requests tree here: we did not send the
	 * Ack yet and did not wake possibly waiting conflicting requests.
	 * Removed from the tree from "drbd_process_done_ee" within the
	 * appropriate w.cb (e_end_block/e_end_resync_block) or from
	 * _drbd_clear_done_ee.
	 */

	do_wake = list_empty(block_id == ID_SYNCER ? &mdev->sync_ee : &mdev->active_ee);

	if (test_bit(__EE_WAS_ERROR, &peer_req->flags))
		__drbd_chk_io_error(mdev, DRBD_WRITE_ERROR);
	spin_unlock_irqrestore(&mdev->tconn->req_lock, flags);

	if (block_id == ID_SYNCER)
		drbd_rs_complete_io(mdev, i.sector);
>>>>>>> refs/remotes/origin/master

	if (do_wake)
		wake_up(&mdev->ee_wait);

	if (do_al_complete_io)
<<<<<<< HEAD
		drbd_al_complete_io(mdev, e_sector);

	wake_asender(mdev);
=======
		drbd_al_complete_io(mdev, &i);

	wake_asender(mdev->tconn);
>>>>>>> refs/remotes/origin/master
	put_ldev(mdev);
}

/* writes on behalf of the partner, or resync writes,
 * "submitted" by the receiver.
 */
<<<<<<< HEAD
void drbd_endio_sec(struct bio *bio, int error)
{
	struct drbd_epoch_entry *e = bio->bi_private;
	struct drbd_conf *mdev = e->mdev;
=======
void drbd_peer_request_endio(struct bio *bio, int error)
{
	struct drbd_peer_request *peer_req = bio->bi_private;
	struct drbd_conf *mdev = peer_req->w.mdev;
>>>>>>> refs/remotes/origin/master
	int uptodate = bio_flagged(bio, BIO_UPTODATE);
	int is_write = bio_data_dir(bio) == WRITE;

	if (error && __ratelimit(&drbd_ratelimit_state))
		dev_warn(DEV, "%s: error=%d s=%llus\n",
				is_write ? "write" : "read", error,
<<<<<<< HEAD
				(unsigned long long)e->sector);
=======
				(unsigned long long)peer_req->i.sector);
>>>>>>> refs/remotes/origin/master
	if (!error && !uptodate) {
		if (__ratelimit(&drbd_ratelimit_state))
			dev_warn(DEV, "%s: setting error to -EIO s=%llus\n",
					is_write ? "write" : "read",
<<<<<<< HEAD
					(unsigned long long)e->sector);
=======
					(unsigned long long)peer_req->i.sector);
>>>>>>> refs/remotes/origin/master
		/* strange behavior of some lower level drivers...
		 * fail the request by clearing the uptodate flag,
		 * but do not return any error?! */
		error = -EIO;
	}

	if (error)
<<<<<<< HEAD
		set_bit(__EE_WAS_ERROR, &e->flags);

	bio_put(bio); /* no need for the bio anymore */
	if (atomic_dec_and_test(&e->pending_bios)) {
		if (is_write)
			drbd_endio_write_sec_final(e);
		else
			drbd_endio_read_sec_final(e);
=======
		set_bit(__EE_WAS_ERROR, &peer_req->flags);

	bio_put(bio); /* no need for the bio anymore */
	if (atomic_dec_and_test(&peer_req->pending_bios)) {
		if (is_write)
			drbd_endio_write_sec_final(peer_req);
		else
			drbd_endio_read_sec_final(peer_req);
>>>>>>> refs/remotes/origin/master
	}
}

/* read, readA or write requests on R_PRIMARY coming from drbd_make_request
 */
<<<<<<< HEAD
void drbd_endio_pri(struct bio *bio, int error)
{
	unsigned long flags;
	struct drbd_request *req = bio->bi_private;
	struct drbd_conf *mdev = req->mdev;
=======
void drbd_request_endio(struct bio *bio, int error)
{
	unsigned long flags;
	struct drbd_request *req = bio->bi_private;
	struct drbd_conf *mdev = req->w.mdev;
>>>>>>> refs/remotes/origin/master
	struct bio_and_error m;
	enum drbd_req_event what;
	int uptodate = bio_flagged(bio, BIO_UPTODATE);

	if (!error && !uptodate) {
		dev_warn(DEV, "p %s: setting error to -EIO\n",
			 bio_data_dir(bio) == WRITE ? "write" : "read");
		/* strange behavior of some lower level drivers...
		 * fail the request by clearing the uptodate flag,
		 * but do not return any error?! */
		error = -EIO;
	}

<<<<<<< HEAD
	/* to avoid recursion in __req_mod */
	if (unlikely(error)) {
		what = (bio_data_dir(bio) == WRITE)
			? write_completed_with_error
			: (bio_rw(bio) == READ)
			  ? read_completed_with_error
			  : read_ahead_completed_with_error;
	} else
		what = completed_ok;
=======

	/* If this request was aborted locally before,
	 * but now was completed "successfully",
	 * chances are that this caused arbitrary data corruption.
	 *
	 * "aborting" requests, or force-detaching the disk, is intended for
	 * completely blocked/hung local backing devices which do no longer
	 * complete requests at all, not even do error completions.  In this
	 * situation, usually a hard-reset and failover is the only way out.
	 *
	 * By "aborting", basically faking a local error-completion,
	 * we allow for a more graceful swichover by cleanly migrating services.
	 * Still the affected node has to be rebooted "soon".
	 *
	 * By completing these requests, we allow the upper layers to re-use
	 * the associated data pages.
	 *
	 * If later the local backing device "recovers", and now DMAs some data
	 * from disk into the original request pages, in the best case it will
	 * just put random data into unused pages; but typically it will corrupt
	 * meanwhile completely unrelated data, causing all sorts of damage.
	 *
	 * Which means delayed successful completion,
	 * especially for READ requests,
	 * is a reason to panic().
	 *
	 * We assume that a delayed *error* completion is OK,
	 * though we still will complain noisily about it.
	 */
	if (unlikely(req->rq_state & RQ_LOCAL_ABORTED)) {
		if (__ratelimit(&drbd_ratelimit_state))
			dev_emerg(DEV, "delayed completion of aborted local request; disk-timeout may be too aggressive\n");

		if (!error)
			panic("possible random memory corruption caused by delayed completion of aborted local request\n");
	}

	/* to avoid recursion in __req_mod */
	if (unlikely(error)) {
		what = (bio_data_dir(bio) == WRITE)
			? WRITE_COMPLETED_WITH_ERROR
			: (bio_rw(bio) == READ)
			  ? READ_COMPLETED_WITH_ERROR
			  : READ_AHEAD_COMPLETED_WITH_ERROR;
	} else
		what = COMPLETED_OK;
>>>>>>> refs/remotes/origin/master

	bio_put(req->private_bio);
	req->private_bio = ERR_PTR(error);

	/* not req_mod(), we need irqsave here! */
<<<<<<< HEAD
	spin_lock_irqsave(&mdev->req_lock, flags);
	__req_mod(req, what, &m);
	spin_unlock_irqrestore(&mdev->req_lock, flags);
=======
	spin_lock_irqsave(&mdev->tconn->req_lock, flags);
	__req_mod(req, what, &m);
	spin_unlock_irqrestore(&mdev->tconn->req_lock, flags);
	put_ldev(mdev);
>>>>>>> refs/remotes/origin/master

	if (m.bio)
		complete_master_bio(mdev, &m);
}

<<<<<<< HEAD
int w_read_retry_remote(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	struct drbd_request *req = container_of(w, struct drbd_request, w);

	/* We should not detach for read io-error,
	 * but try to WRITE the P_DATA_REPLY to the failed location,
	 * to give the disk the chance to relocate that block */

	spin_lock_irq(&mdev->req_lock);
	if (cancel || mdev->state.pdsk != D_UP_TO_DATE) {
		_req_mod(req, read_retry_remote_canceled);
		spin_unlock_irq(&mdev->req_lock);
		return 1;
	}
	spin_unlock_irq(&mdev->req_lock);

	return w_send_read_req(mdev, w, 0);
}

void drbd_csum_ee(struct drbd_conf *mdev, struct crypto_hash *tfm, struct drbd_epoch_entry *e, void *digest)
{
	struct hash_desc desc;
	struct scatterlist sg;
	struct page *page = e->pages;
=======
void drbd_csum_ee(struct drbd_conf *mdev, struct crypto_hash *tfm,
		  struct drbd_peer_request *peer_req, void *digest)
{
	struct hash_desc desc;
	struct scatterlist sg;
	struct page *page = peer_req->pages;
>>>>>>> refs/remotes/origin/master
	struct page *tmp;
	unsigned len;

	desc.tfm = tfm;
	desc.flags = 0;

	sg_init_table(&sg, 1);
	crypto_hash_init(&desc);

	while ((tmp = page_chain_next(page))) {
		/* all but the last page will be fully used */
		sg_set_page(&sg, page, PAGE_SIZE, 0);
		crypto_hash_update(&desc, &sg, sg.length);
		page = tmp;
	}
	/* and now the last, possibly only partially used page */
<<<<<<< HEAD
	len = e->size & (PAGE_SIZE - 1);
=======
	len = peer_req->i.size & (PAGE_SIZE - 1);
>>>>>>> refs/remotes/origin/master
	sg_set_page(&sg, page, len ?: PAGE_SIZE, 0);
	crypto_hash_update(&desc, &sg, sg.length);
	crypto_hash_final(&desc, digest);
}

void drbd_csum_bio(struct drbd_conf *mdev, struct crypto_hash *tfm, struct bio *bio, void *digest)
{
	struct hash_desc desc;
	struct scatterlist sg;
	struct bio_vec *bvec;
	int i;

	desc.tfm = tfm;
	desc.flags = 0;

	sg_init_table(&sg, 1);
	crypto_hash_init(&desc);

<<<<<<< HEAD
	__bio_for_each_segment(bvec, bio, i, 0) {
=======
	bio_for_each_segment(bvec, bio, i) {
>>>>>>> refs/remotes/origin/master
		sg_set_page(&sg, bvec->bv_page, bvec->bv_len, bvec->bv_offset);
		crypto_hash_update(&desc, &sg, sg.length);
	}
	crypto_hash_final(&desc, digest);
}

<<<<<<< HEAD
/* TODO merge common code with w_e_end_ov_req */
int w_e_send_csum(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	struct drbd_epoch_entry *e = container_of(w, struct drbd_epoch_entry, w);
	int digest_size;
	void *digest;
	int ok = 1;

	D_ASSERT(e->block_id == DRBD_MAGIC + 0xbeef);
=======
/* MAYBE merge common code with w_e_end_ov_req */
static int w_e_send_csum(struct drbd_work *w, int cancel)
{
	struct drbd_peer_request *peer_req = container_of(w, struct drbd_peer_request, w);
	struct drbd_conf *mdev = w->mdev;
	int digest_size;
	void *digest;
	int err = 0;
>>>>>>> refs/remotes/origin/master

	if (unlikely(cancel))
		goto out;

<<<<<<< HEAD
	if (likely((e->flags & EE_WAS_ERROR) != 0))
		goto out;

	digest_size = crypto_hash_digestsize(mdev->csums_tfm);
	digest = kmalloc(digest_size, GFP_NOIO);
	if (digest) {
		sector_t sector = e->sector;
		unsigned int size = e->size;
		drbd_csum_ee(mdev, mdev->csums_tfm, e, digest);
		/* Free e and pages before send.
		 * In case we block on congestion, we could otherwise run into
		 * some distributed deadlock, if the other side blocks on
		 * congestion as well, because our receiver blocks in
		 * drbd_pp_alloc due to pp_in_use > max_buffers. */
		drbd_free_ee(mdev, e);
		e = NULL;
		inc_rs_pending(mdev);
		ok = drbd_send_drequest_csum(mdev, sector, size,
					     digest, digest_size,
					     P_CSUM_RS_REQUEST);
		kfree(digest);
	} else {
		dev_err(DEV, "kmalloc() of digest failed.\n");
		ok = 0;
	}

out:
	if (e)
		drbd_free_ee(mdev, e);

	if (unlikely(!ok))
		dev_err(DEV, "drbd_send_drequest(..., csum) failed\n");
	return ok;
=======
	if (unlikely((peer_req->flags & EE_WAS_ERROR) != 0))
		goto out;

	digest_size = crypto_hash_digestsize(mdev->tconn->csums_tfm);
	digest = kmalloc(digest_size, GFP_NOIO);
	if (digest) {
		sector_t sector = peer_req->i.sector;
		unsigned int size = peer_req->i.size;
		drbd_csum_ee(mdev, mdev->tconn->csums_tfm, peer_req, digest);
		/* Free peer_req and pages before send.
		 * In case we block on congestion, we could otherwise run into
		 * some distributed deadlock, if the other side blocks on
		 * congestion as well, because our receiver blocks in
		 * drbd_alloc_pages due to pp_in_use > max_buffers. */
		drbd_free_peer_req(mdev, peer_req);
		peer_req = NULL;
		inc_rs_pending(mdev);
		err = drbd_send_drequest_csum(mdev, sector, size,
					      digest, digest_size,
					      P_CSUM_RS_REQUEST);
		kfree(digest);
	} else {
		dev_err(DEV, "kmalloc() of digest failed.\n");
		err = -ENOMEM;
	}

out:
	if (peer_req)
		drbd_free_peer_req(mdev, peer_req);

	if (unlikely(err))
		dev_err(DEV, "drbd_send_drequest(..., csum) failed\n");
	return err;
>>>>>>> refs/remotes/origin/master
}

#define GFP_TRY	(__GFP_HIGHMEM | __GFP_NOWARN)

static int read_for_csum(struct drbd_conf *mdev, sector_t sector, int size)
{
<<<<<<< HEAD
	struct drbd_epoch_entry *e;
=======
	struct drbd_peer_request *peer_req;
>>>>>>> refs/remotes/origin/master

	if (!get_ldev(mdev))
		return -EIO;

	if (drbd_rs_should_slow_down(mdev, sector))
		goto defer;

	/* GFP_TRY, because if there is no memory available right now, this may
	 * be rescheduled for later. It is "only" background resync, after all. */
<<<<<<< HEAD
	e = drbd_alloc_ee(mdev, DRBD_MAGIC+0xbeef, sector, size, GFP_TRY);
	if (!e)
		goto defer;

	e->w.cb = w_e_send_csum;
	spin_lock_irq(&mdev->req_lock);
	list_add(&e->w.list, &mdev->read_ee);
	spin_unlock_irq(&mdev->req_lock);

	atomic_add(size >> 9, &mdev->rs_sect_ev);
	if (drbd_submit_ee(mdev, e, READ, DRBD_FAULT_RS_RD) == 0)
=======
	peer_req = drbd_alloc_peer_req(mdev, ID_SYNCER /* unused */, sector,
				       size, GFP_TRY);
	if (!peer_req)
		goto defer;

	peer_req->w.cb = w_e_send_csum;
	spin_lock_irq(&mdev->tconn->req_lock);
	list_add(&peer_req->w.list, &mdev->read_ee);
	spin_unlock_irq(&mdev->tconn->req_lock);

	atomic_add(size >> 9, &mdev->rs_sect_ev);
	if (drbd_submit_peer_request(mdev, peer_req, READ, DRBD_FAULT_RS_RD) == 0)
>>>>>>> refs/remotes/origin/master
		return 0;

	/* If it failed because of ENOMEM, retry should help.  If it failed
	 * because bio_add_page failed (probably broken lower level driver),
	 * retry may or may not help.
	 * If it does not, you may need to force disconnect. */
<<<<<<< HEAD
	spin_lock_irq(&mdev->req_lock);
	list_del(&e->w.list);
	spin_unlock_irq(&mdev->req_lock);

	drbd_free_ee(mdev, e);
=======
	spin_lock_irq(&mdev->tconn->req_lock);
	list_del(&peer_req->w.list);
	spin_unlock_irq(&mdev->tconn->req_lock);

	drbd_free_peer_req(mdev, peer_req);
>>>>>>> refs/remotes/origin/master
defer:
	put_ldev(mdev);
	return -EAGAIN;
}

<<<<<<< HEAD
int w_resync_timer(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	switch (mdev->state.conn) {
	case C_VERIFY_S:
		w_make_ov_request(mdev, w, cancel);
		break;
	case C_SYNC_TARGET:
		w_make_resync_request(mdev, w, cancel);
		break;
	}

	return 1;
=======
int w_resync_timer(struct drbd_work *w, int cancel)
{
	struct drbd_conf *mdev = w->mdev;
	switch (mdev->state.conn) {
	case C_VERIFY_S:
		w_make_ov_request(w, cancel);
		break;
	case C_SYNC_TARGET:
		w_make_resync_request(w, cancel);
		break;
	}

	return 0;
>>>>>>> refs/remotes/origin/master
}

void resync_timer_fn(unsigned long data)
{
	struct drbd_conf *mdev = (struct drbd_conf *) data;

	if (list_empty(&mdev->resync_work.list))
<<<<<<< HEAD
		drbd_queue_work(&mdev->data.work, &mdev->resync_work);
=======
		drbd_queue_work(&mdev->tconn->sender_work, &mdev->resync_work);
>>>>>>> refs/remotes/origin/master
}

static void fifo_set(struct fifo_buffer *fb, int value)
{
	int i;

	for (i = 0; i < fb->size; i++)
		fb->values[i] = value;
}

static int fifo_push(struct fifo_buffer *fb, int value)
{
	int ov;

	ov = fb->values[fb->head_index];
	fb->values[fb->head_index++] = value;

	if (fb->head_index >= fb->size)
		fb->head_index = 0;

	return ov;
}

static void fifo_add_val(struct fifo_buffer *fb, int value)
{
	int i;

	for (i = 0; i < fb->size; i++)
		fb->values[i] += value;
}

<<<<<<< HEAD
static int drbd_rs_controller(struct drbd_conf *mdev)
{
=======
struct fifo_buffer *fifo_alloc(int fifo_size)
{
	struct fifo_buffer *fb;

	fb = kzalloc(sizeof(struct fifo_buffer) + sizeof(int) * fifo_size, GFP_NOIO);
	if (!fb)
		return NULL;

	fb->head_index = 0;
	fb->size = fifo_size;
	fb->total = 0;

	return fb;
}

static int drbd_rs_controller(struct drbd_conf *mdev)
{
	struct disk_conf *dc;
>>>>>>> refs/remotes/origin/master
	unsigned int sect_in;  /* Number of sectors that came in since the last turn */
	unsigned int want;     /* The number of sectors we want in the proxy */
	int req_sect; /* Number of sectors to request in this turn */
	int correction; /* Number of sectors more we need in the proxy*/
	int cps; /* correction per invocation of drbd_rs_controller() */
	int steps; /* Number of time steps to plan ahead */
	int curr_corr;
	int max_sect;
<<<<<<< HEAD
=======
	struct fifo_buffer *plan;
>>>>>>> refs/remotes/origin/master

	sect_in = atomic_xchg(&mdev->rs_sect_in, 0); /* Number of sectors that came in */
	mdev->rs_in_flight -= sect_in;

<<<<<<< HEAD
	spin_lock(&mdev->peer_seq_lock); /* get an atomic view on mdev->rs_plan_s */

	steps = mdev->rs_plan_s.size; /* (mdev->sync_conf.c_plan_ahead * 10 * SLEEP_TIME) / HZ; */

	if (mdev->rs_in_flight + sect_in == 0) { /* At start of resync */
		want = ((mdev->sync_conf.rate * 2 * SLEEP_TIME) / HZ) * steps;
	} else { /* normal path */
		want = mdev->sync_conf.c_fill_target ? mdev->sync_conf.c_fill_target :
			sect_in * mdev->sync_conf.c_delay_target * HZ / (SLEEP_TIME * 10);
	}

	correction = want - mdev->rs_in_flight - mdev->rs_planed;

	/* Plan ahead */
	cps = correction / steps;
	fifo_add_val(&mdev->rs_plan_s, cps);
	mdev->rs_planed += cps * steps;

	/* What we do in this step */
	curr_corr = fifo_push(&mdev->rs_plan_s, 0);
	spin_unlock(&mdev->peer_seq_lock);
	mdev->rs_planed -= curr_corr;
=======
	dc = rcu_dereference(mdev->ldev->disk_conf);
	plan = rcu_dereference(mdev->rs_plan_s);

	steps = plan->size; /* (dc->c_plan_ahead * 10 * SLEEP_TIME) / HZ; */

	if (mdev->rs_in_flight + sect_in == 0) { /* At start of resync */
		want = ((dc->resync_rate * 2 * SLEEP_TIME) / HZ) * steps;
	} else { /* normal path */
		want = dc->c_fill_target ? dc->c_fill_target :
			sect_in * dc->c_delay_target * HZ / (SLEEP_TIME * 10);
	}

	correction = want - mdev->rs_in_flight - plan->total;

	/* Plan ahead */
	cps = correction / steps;
	fifo_add_val(plan, cps);
	plan->total += cps * steps;

	/* What we do in this step */
	curr_corr = fifo_push(plan, 0);
	plan->total -= curr_corr;
>>>>>>> refs/remotes/origin/master

	req_sect = sect_in + curr_corr;
	if (req_sect < 0)
		req_sect = 0;

<<<<<<< HEAD
	max_sect = (mdev->sync_conf.c_max_rate * 2 * SLEEP_TIME) / HZ;
=======
	max_sect = (dc->c_max_rate * 2 * SLEEP_TIME) / HZ;
>>>>>>> refs/remotes/origin/master
	if (req_sect > max_sect)
		req_sect = max_sect;

	/*
	dev_warn(DEV, "si=%u if=%d wa=%u co=%d st=%d cps=%d pl=%d cc=%d rs=%d\n",
		 sect_in, mdev->rs_in_flight, want, correction,
		 steps, cps, mdev->rs_planed, curr_corr, req_sect);
	*/

	return req_sect;
}

static int drbd_rs_number_requests(struct drbd_conf *mdev)
{
	int number;
<<<<<<< HEAD
	if (mdev->rs_plan_s.size) { /* mdev->sync_conf.c_plan_ahead */
		number = drbd_rs_controller(mdev) >> (BM_BLOCK_SHIFT - 9);
		mdev->c_sync_rate = number * HZ * (BM_BLOCK_SIZE / 1024) / SLEEP_TIME;
	} else {
		mdev->c_sync_rate = mdev->sync_conf.rate;
		number = SLEEP_TIME * mdev->c_sync_rate  / ((BM_BLOCK_SIZE / 1024) * HZ);
	}
=======

	rcu_read_lock();
	if (rcu_dereference(mdev->rs_plan_s)->size) {
		number = drbd_rs_controller(mdev) >> (BM_BLOCK_SHIFT - 9);
		mdev->c_sync_rate = number * HZ * (BM_BLOCK_SIZE / 1024) / SLEEP_TIME;
	} else {
		mdev->c_sync_rate = rcu_dereference(mdev->ldev->disk_conf)->resync_rate;
		number = SLEEP_TIME * mdev->c_sync_rate  / ((BM_BLOCK_SIZE / 1024) * HZ);
	}
	rcu_read_unlock();
>>>>>>> refs/remotes/origin/master

	/* ignore the amount of pending requests, the resync controller should
	 * throttle down to incoming reply rate soon enough anyways. */
	return number;
}

<<<<<<< HEAD
static int w_make_resync_request(struct drbd_conf *mdev,
				 struct drbd_work *w, int cancel)
{
=======
int w_make_resync_request(struct drbd_work *w, int cancel)
{
	struct drbd_conf *mdev = w->mdev;
>>>>>>> refs/remotes/origin/master
	unsigned long bit;
	sector_t sector;
	const sector_t capacity = drbd_get_capacity(mdev->this_bdev);
	int max_bio_size;
	int number, rollback_i, size;
	int align, queued, sndbuf;
	int i = 0;

	if (unlikely(cancel))
<<<<<<< HEAD
		return 1;
=======
		return 0;
>>>>>>> refs/remotes/origin/master

	if (mdev->rs_total == 0) {
		/* empty resync? */
		drbd_resync_finished(mdev);
<<<<<<< HEAD
		return 1;
=======
		return 0;
>>>>>>> refs/remotes/origin/master
	}

	if (!get_ldev(mdev)) {
		/* Since we only need to access mdev->rsync a
		   get_ldev_if_state(mdev,D_FAILED) would be sufficient, but
		   to continue resync with a broken disk makes no sense at
		   all */
		dev_err(DEV, "Disk broke down during resync!\n");
<<<<<<< HEAD
		return 1;
=======
		return 0;
>>>>>>> refs/remotes/origin/master
	}

	max_bio_size = queue_max_hw_sectors(mdev->rq_queue) << 9;
	number = drbd_rs_number_requests(mdev);
	if (number == 0)
		goto requeue;

	for (i = 0; i < number; i++) {
		/* Stop generating RS requests, when half of the send buffer is filled */
<<<<<<< HEAD
		mutex_lock(&mdev->data.mutex);
		if (mdev->data.socket) {
			queued = mdev->data.socket->sk->sk_wmem_queued;
			sndbuf = mdev->data.socket->sk->sk_sndbuf;
=======
		mutex_lock(&mdev->tconn->data.mutex);
		if (mdev->tconn->data.socket) {
			queued = mdev->tconn->data.socket->sk->sk_wmem_queued;
			sndbuf = mdev->tconn->data.socket->sk->sk_sndbuf;
>>>>>>> refs/remotes/origin/master
		} else {
			queued = 1;
			sndbuf = 0;
		}
<<<<<<< HEAD
		mutex_unlock(&mdev->data.mutex);
=======
		mutex_unlock(&mdev->tconn->data.mutex);
>>>>>>> refs/remotes/origin/master
		if (queued > sndbuf / 2)
			goto requeue;

next_sector:
		size = BM_BLOCK_SIZE;
		bit  = drbd_bm_find_next(mdev, mdev->bm_resync_fo);

		if (bit == DRBD_END_OF_BITMAP) {
			mdev->bm_resync_fo = drbd_bm_bits(mdev);
			put_ldev(mdev);
<<<<<<< HEAD
			return 1;
=======
			return 0;
>>>>>>> refs/remotes/origin/master
		}

		sector = BM_BIT_TO_SECT(bit);

		if (drbd_rs_should_slow_down(mdev, sector) ||
		    drbd_try_rs_begin_io(mdev, sector)) {
			mdev->bm_resync_fo = bit;
			goto requeue;
		}
		mdev->bm_resync_fo = bit + 1;

		if (unlikely(drbd_bm_test_bit(mdev, bit) == 0)) {
			drbd_rs_complete_io(mdev, sector);
			goto next_sector;
		}

#if DRBD_MAX_BIO_SIZE > BM_BLOCK_SIZE
		/* try to find some adjacent bits.
		 * we stop if we have already the maximum req size.
		 *
		 * Additionally always align bigger requests, in order to
		 * be prepared for all stripe sizes of software RAIDs.
		 */
		align = 1;
		rollback_i = i;
		for (;;) {
			if (size + BM_BLOCK_SIZE > max_bio_size)
				break;

			/* Be always aligned */
			if (sector & ((1<<(align+3))-1))
				break;

			/* do not cross extent boundaries */
			if (((bit+1) & BM_BLOCKS_PER_BM_EXT_MASK) == 0)
				break;
			/* now, is it actually dirty, after all?
			 * caution, drbd_bm_test_bit is tri-state for some
			 * obscure reason; ( b == 0 ) would get the out-of-band
			 * only accidentally right because of the "oddly sized"
			 * adjustment below */
			if (drbd_bm_test_bit(mdev, bit+1) != 1)
				break;
			bit++;
			size += BM_BLOCK_SIZE;
			if ((BM_BLOCK_SIZE << align) <= size)
				align++;
			i++;
		}
		/* if we merged some,
		 * reset the offset to start the next drbd_bm_find_next from */
		if (size > BM_BLOCK_SIZE)
			mdev->bm_resync_fo = bit + 1;
#endif

		/* adjust very last sectors, in case we are oddly sized */
		if (sector + (size>>9) > capacity)
			size = (capacity-sector)<<9;
<<<<<<< HEAD
		if (mdev->agreed_pro_version >= 89 && mdev->csums_tfm) {
			switch (read_for_csum(mdev, sector, size)) {
			case -EIO: /* Disk failure */
				put_ldev(mdev);
				return 0;
=======
		if (mdev->tconn->agreed_pro_version >= 89 && mdev->tconn->csums_tfm) {
			switch (read_for_csum(mdev, sector, size)) {
			case -EIO: /* Disk failure */
				put_ldev(mdev);
				return -EIO;
>>>>>>> refs/remotes/origin/master
			case -EAGAIN: /* allocation failed, or ldev busy */
				drbd_rs_complete_io(mdev, sector);
				mdev->bm_resync_fo = BM_SECT_TO_BIT(sector);
				i = rollback_i;
				goto requeue;
			case 0:
				/* everything ok */
				break;
			default:
				BUG();
			}
		} else {
<<<<<<< HEAD
			inc_rs_pending(mdev);
			if (!drbd_send_drequest(mdev, P_RS_DATA_REQUEST,
					       sector, size, ID_SYNCER)) {
				dev_err(DEV, "drbd_send_drequest() failed, aborting...\n");
				dec_rs_pending(mdev);
				put_ldev(mdev);
				return 0;
=======
			int err;

			inc_rs_pending(mdev);
			err = drbd_send_drequest(mdev, P_RS_DATA_REQUEST,
						 sector, size, ID_SYNCER);
			if (err) {
				dev_err(DEV, "drbd_send_drequest() failed, aborting...\n");
				dec_rs_pending(mdev);
				put_ldev(mdev);
				return err;
>>>>>>> refs/remotes/origin/master
			}
		}
	}

	if (mdev->bm_resync_fo >= drbd_bm_bits(mdev)) {
		/* last syncer _request_ was sent,
		 * but the P_RS_DATA_REPLY not yet received.  sync will end (and
		 * next sync group will resume), as soon as we receive the last
		 * resync data block, and the last bit is cleared.
		 * until then resync "work" is "inactive" ...
		 */
		put_ldev(mdev);
<<<<<<< HEAD
		return 1;
=======
		return 0;
>>>>>>> refs/remotes/origin/master
	}

 requeue:
	mdev->rs_in_flight += (i << (BM_BLOCK_SHIFT - 9));
	mod_timer(&mdev->resync_timer, jiffies + SLEEP_TIME);
	put_ldev(mdev);
<<<<<<< HEAD
	return 1;
}

static int w_make_ov_request(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	int number, i, size;
	sector_t sector;
	const sector_t capacity = drbd_get_capacity(mdev->this_bdev);
=======
	return 0;
}

static int w_make_ov_request(struct drbd_work *w, int cancel)
{
	struct drbd_conf *mdev = w->mdev;
	int number, i, size;
	sector_t sector;
	const sector_t capacity = drbd_get_capacity(mdev->this_bdev);
	bool stop_sector_reached = false;
>>>>>>> refs/remotes/origin/master

	if (unlikely(cancel))
		return 1;

	number = drbd_rs_number_requests(mdev);

	sector = mdev->ov_position;
	for (i = 0; i < number; i++) {
<<<<<<< HEAD
		if (sector >= capacity) {
			return 1;
		}
=======
		if (sector >= capacity)
			return 1;

		/* We check for "finished" only in the reply path:
		 * w_e_end_ov_reply().
		 * We need to send at least one request out. */
		stop_sector_reached = i > 0
			&& verify_can_do_stop_sector(mdev)
			&& sector >= mdev->ov_stop_sector;
		if (stop_sector_reached)
			break;
>>>>>>> refs/remotes/origin/master

		size = BM_BLOCK_SIZE;

		if (drbd_rs_should_slow_down(mdev, sector) ||
		    drbd_try_rs_begin_io(mdev, sector)) {
			mdev->ov_position = sector;
			goto requeue;
		}

		if (sector + (size>>9) > capacity)
			size = (capacity-sector)<<9;

		inc_rs_pending(mdev);
<<<<<<< HEAD
		if (!drbd_send_ov_request(mdev, sector, size)) {
=======
		if (drbd_send_ov_request(mdev, sector, size)) {
>>>>>>> refs/remotes/origin/master
			dec_rs_pending(mdev);
			return 0;
		}
		sector += BM_SECT_PER_BIT;
	}
	mdev->ov_position = sector;

 requeue:
	mdev->rs_in_flight += (i << (BM_BLOCK_SHIFT - 9));
<<<<<<< HEAD
	mod_timer(&mdev->resync_timer, jiffies + SLEEP_TIME);
	return 1;
}


void start_resync_timer_fn(unsigned long data)
{
	struct drbd_conf *mdev = (struct drbd_conf *) data;

	drbd_queue_work(&mdev->data.work, &mdev->start_resync_work);
}

int w_start_resync(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	if (atomic_read(&mdev->unacked_cnt) || atomic_read(&mdev->rs_pending_cnt)) {
		dev_warn(DEV, "w_start_resync later...\n");
		mdev->start_resync_timer.expires = jiffies + HZ/10;
		add_timer(&mdev->start_resync_timer);
		return 1;
	}

	drbd_start_resync(mdev, C_SYNC_SOURCE);
	clear_bit(AHEAD_TO_SYNC_SOURCE, &mdev->current_epoch->flags);
	return 1;
}

int w_ov_finished(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	kfree(w);
	ov_oos_print(mdev);
	drbd_resync_finished(mdev);

	return 1;
}

static int w_resync_finished(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
=======
	if (i == 0 || !stop_sector_reached)
		mod_timer(&mdev->resync_timer, jiffies + SLEEP_TIME);
	return 1;
}

int w_ov_finished(struct drbd_work *w, int cancel)
{
	struct drbd_conf *mdev = w->mdev;
	kfree(w);
	ov_out_of_sync_print(mdev);
	drbd_resync_finished(mdev);

	return 0;
}

static int w_resync_finished(struct drbd_work *w, int cancel)
{
	struct drbd_conf *mdev = w->mdev;
>>>>>>> refs/remotes/origin/master
	kfree(w);

	drbd_resync_finished(mdev);

<<<<<<< HEAD
	return 1;
=======
	return 0;
>>>>>>> refs/remotes/origin/master
}

static void ping_peer(struct drbd_conf *mdev)
{
<<<<<<< HEAD
	clear_bit(GOT_PING_ACK, &mdev->flags);
	request_ping(mdev);
	wait_event(mdev->misc_wait,
		   test_bit(GOT_PING_ACK, &mdev->flags) || mdev->state.conn < C_CONNECTED);
=======
	struct drbd_tconn *tconn = mdev->tconn;

	clear_bit(GOT_PING_ACK, &tconn->flags);
	request_ping(tconn);
	wait_event(tconn->ping_wait,
		   test_bit(GOT_PING_ACK, &tconn->flags) || mdev->state.conn < C_CONNECTED);
>>>>>>> refs/remotes/origin/master
}

int drbd_resync_finished(struct drbd_conf *mdev)
{
	unsigned long db, dt, dbdt;
	unsigned long n_oos;
	union drbd_state os, ns;
	struct drbd_work *w;
	char *khelper_cmd = NULL;
	int verify_done = 0;

	/* Remove all elements from the resync LRU. Since future actions
	 * might set bits in the (main) bitmap, then the entries in the
	 * resync LRU would be wrong. */
	if (drbd_rs_del_all(mdev)) {
		/* In case this is not possible now, most probably because
		 * there are P_RS_DATA_REPLY Packets lingering on the worker's
		 * queue (or even the read operations for those packets
		 * is not finished by now).   Retry in 100ms. */

		schedule_timeout_interruptible(HZ / 10);
		w = kmalloc(sizeof(struct drbd_work), GFP_ATOMIC);
		if (w) {
			w->cb = w_resync_finished;
<<<<<<< HEAD
			drbd_queue_work(&mdev->data.work, w);
=======
			w->mdev = mdev;
			drbd_queue_work(&mdev->tconn->sender_work, w);
>>>>>>> refs/remotes/origin/master
			return 1;
		}
		dev_err(DEV, "Warn failed to drbd_rs_del_all() and to kmalloc(w).\n");
	}

	dt = (jiffies - mdev->rs_start - mdev->rs_paused) / HZ;
	if (dt <= 0)
		dt = 1;
<<<<<<< HEAD
	db = mdev->rs_total;
=======
	
	db = mdev->rs_total;
	/* adjust for verify start and stop sectors, respective reached position */
	if (mdev->state.conn == C_VERIFY_S || mdev->state.conn == C_VERIFY_T)
		db -= mdev->ov_left;

>>>>>>> refs/remotes/origin/master
	dbdt = Bit2KB(db/dt);
	mdev->rs_paused /= HZ;

	if (!get_ldev(mdev))
		goto out;

	ping_peer(mdev);

<<<<<<< HEAD
	spin_lock_irq(&mdev->req_lock);
	os = mdev->state;
=======
	spin_lock_irq(&mdev->tconn->req_lock);
	os = drbd_read_state(mdev);
>>>>>>> refs/remotes/origin/master

	verify_done = (os.conn == C_VERIFY_S || os.conn == C_VERIFY_T);

	/* This protects us against multiple calls (that can happen in the presence
	   of application IO), and against connectivity loss just before we arrive here. */
	if (os.conn <= C_CONNECTED)
		goto out_unlock;

	ns = os;
	ns.conn = C_CONNECTED;

	dev_info(DEV, "%s done (total %lu sec; paused %lu sec; %lu K/sec)\n",
<<<<<<< HEAD
	     verify_done ? "Online verify " : "Resync",
=======
	     verify_done ? "Online verify" : "Resync",
>>>>>>> refs/remotes/origin/master
	     dt + mdev->rs_paused, mdev->rs_paused, dbdt);

	n_oos = drbd_bm_total_weight(mdev);

	if (os.conn == C_VERIFY_S || os.conn == C_VERIFY_T) {
		if (n_oos) {
			dev_alert(DEV, "Online verify found %lu %dk block out of sync!\n",
			      n_oos, Bit2KB(1));
			khelper_cmd = "out-of-sync";
		}
	} else {
		D_ASSERT((n_oos - mdev->rs_failed) == 0);

		if (os.conn == C_SYNC_TARGET || os.conn == C_PAUSED_SYNC_T)
			khelper_cmd = "after-resync-target";

<<<<<<< HEAD
		if (mdev->csums_tfm && mdev->rs_total) {
=======
		if (mdev->tconn->csums_tfm && mdev->rs_total) {
>>>>>>> refs/remotes/origin/master
			const unsigned long s = mdev->rs_same_csum;
			const unsigned long t = mdev->rs_total;
			const int ratio =
				(t == 0)     ? 0 :
			(t < 100000) ? ((s*100)/t) : (s/(t/100));
			dev_info(DEV, "%u %% had equal checksums, eliminated: %luK; "
			     "transferred %luK total %luK\n",
			     ratio,
			     Bit2KB(mdev->rs_same_csum),
			     Bit2KB(mdev->rs_total - mdev->rs_same_csum),
			     Bit2KB(mdev->rs_total));
		}
	}

	if (mdev->rs_failed) {
		dev_info(DEV, "            %lu failed blocks\n", mdev->rs_failed);

		if (os.conn == C_SYNC_TARGET || os.conn == C_PAUSED_SYNC_T) {
			ns.disk = D_INCONSISTENT;
			ns.pdsk = D_UP_TO_DATE;
		} else {
			ns.disk = D_UP_TO_DATE;
			ns.pdsk = D_INCONSISTENT;
		}
	} else {
		ns.disk = D_UP_TO_DATE;
		ns.pdsk = D_UP_TO_DATE;

		if (os.conn == C_SYNC_TARGET || os.conn == C_PAUSED_SYNC_T) {
			if (mdev->p_uuid) {
				int i;
				for (i = UI_BITMAP ; i <= UI_HISTORY_END ; i++)
					_drbd_uuid_set(mdev, i, mdev->p_uuid[i]);
				drbd_uuid_set(mdev, UI_BITMAP, mdev->ldev->md.uuid[UI_CURRENT]);
				_drbd_uuid_set(mdev, UI_CURRENT, mdev->p_uuid[UI_CURRENT]);
			} else {
				dev_err(DEV, "mdev->p_uuid is NULL! BUG\n");
			}
		}

		if (!(os.conn == C_VERIFY_S || os.conn == C_VERIFY_T)) {
			/* for verify runs, we don't update uuids here,
			 * so there would be nothing to report. */
			drbd_uuid_set_bm(mdev, 0UL);
			drbd_print_uuids(mdev, "updated UUIDs");
			if (mdev->p_uuid) {
				/* Now the two UUID sets are equal, update what we
				 * know of the peer. */
				int i;
				for (i = UI_CURRENT ; i <= UI_HISTORY_END ; i++)
					mdev->p_uuid[i] = mdev->ldev->md.uuid[i];
			}
		}
	}

	_drbd_set_state(mdev, ns, CS_VERBOSE, NULL);
out_unlock:
<<<<<<< HEAD
	spin_unlock_irq(&mdev->req_lock);
=======
	spin_unlock_irq(&mdev->tconn->req_lock);
>>>>>>> refs/remotes/origin/master
	put_ldev(mdev);
out:
	mdev->rs_total  = 0;
	mdev->rs_failed = 0;
	mdev->rs_paused = 0;
<<<<<<< HEAD
	if (verify_done)
=======

	/* reset start sector, if we reached end of device */
	if (verify_done && mdev->ov_left == 0)
>>>>>>> refs/remotes/origin/master
		mdev->ov_start_sector = 0;

	drbd_md_sync(mdev);

	if (khelper_cmd)
		drbd_khelper(mdev, khelper_cmd);

	return 1;
}

/* helper */
<<<<<<< HEAD
static void move_to_net_ee_or_free(struct drbd_conf *mdev, struct drbd_epoch_entry *e)
{
	if (drbd_ee_has_active_page(e)) {
		/* This might happen if sendpage() has not finished */
		int i = (e->size + PAGE_SIZE -1) >> PAGE_SHIFT;
		atomic_add(i, &mdev->pp_in_use_by_net);
		atomic_sub(i, &mdev->pp_in_use);
		spin_lock_irq(&mdev->req_lock);
		list_add_tail(&e->w.list, &mdev->net_ee);
		spin_unlock_irq(&mdev->req_lock);
		wake_up(&drbd_pp_wait);
	} else
		drbd_free_ee(mdev, e);
=======
static void move_to_net_ee_or_free(struct drbd_conf *mdev, struct drbd_peer_request *peer_req)
{
	if (drbd_peer_req_has_active_page(peer_req)) {
		/* This might happen if sendpage() has not finished */
		int i = (peer_req->i.size + PAGE_SIZE -1) >> PAGE_SHIFT;
		atomic_add(i, &mdev->pp_in_use_by_net);
		atomic_sub(i, &mdev->pp_in_use);
		spin_lock_irq(&mdev->tconn->req_lock);
		list_add_tail(&peer_req->w.list, &mdev->net_ee);
		spin_unlock_irq(&mdev->tconn->req_lock);
		wake_up(&drbd_pp_wait);
	} else
		drbd_free_peer_req(mdev, peer_req);
>>>>>>> refs/remotes/origin/master
}

/**
 * w_e_end_data_req() - Worker callback, to send a P_DATA_REPLY packet in response to a P_DATA_REQUEST
 * @mdev:	DRBD device.
 * @w:		work object.
 * @cancel:	The connection will be closed anyways
 */
<<<<<<< HEAD
int w_e_end_data_req(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	struct drbd_epoch_entry *e = container_of(w, struct drbd_epoch_entry, w);
	int ok;

	if (unlikely(cancel)) {
		drbd_free_ee(mdev, e);
		dec_unacked(mdev);
		return 1;
	}

	if (likely((e->flags & EE_WAS_ERROR) == 0)) {
		ok = drbd_send_block(mdev, P_DATA_REPLY, e);
	} else {
		if (__ratelimit(&drbd_ratelimit_state))
			dev_err(DEV, "Sending NegDReply. sector=%llus.\n",
			    (unsigned long long)e->sector);

		ok = drbd_send_ack(mdev, P_NEG_DREPLY, e);
=======
int w_e_end_data_req(struct drbd_work *w, int cancel)
{
	struct drbd_peer_request *peer_req = container_of(w, struct drbd_peer_request, w);
	struct drbd_conf *mdev = w->mdev;
	int err;

	if (unlikely(cancel)) {
		drbd_free_peer_req(mdev, peer_req);
		dec_unacked(mdev);
		return 0;
	}

	if (likely((peer_req->flags & EE_WAS_ERROR) == 0)) {
		err = drbd_send_block(mdev, P_DATA_REPLY, peer_req);
	} else {
		if (__ratelimit(&drbd_ratelimit_state))
			dev_err(DEV, "Sending NegDReply. sector=%llus.\n",
			    (unsigned long long)peer_req->i.sector);

		err = drbd_send_ack(mdev, P_NEG_DREPLY, peer_req);
>>>>>>> refs/remotes/origin/master
	}

	dec_unacked(mdev);

<<<<<<< HEAD
	move_to_net_ee_or_free(mdev, e);

	if (unlikely(!ok))
		dev_err(DEV, "drbd_send_block() failed\n");
	return ok;
}

/**
 * w_e_end_rsdata_req() - Worker callback to send a P_RS_DATA_REPLY packet in response to a P_RS_DATA_REQUESTRS
=======
	move_to_net_ee_or_free(mdev, peer_req);

	if (unlikely(err))
		dev_err(DEV, "drbd_send_block() failed\n");
	return err;
}

/**
 * w_e_end_rsdata_req() - Worker callback to send a P_RS_DATA_REPLY packet in response to a P_RS_DATA_REQUEST
>>>>>>> refs/remotes/origin/master
 * @mdev:	DRBD device.
 * @w:		work object.
 * @cancel:	The connection will be closed anyways
 */
<<<<<<< HEAD
int w_e_end_rsdata_req(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	struct drbd_epoch_entry *e = container_of(w, struct drbd_epoch_entry, w);
	int ok;

	if (unlikely(cancel)) {
		drbd_free_ee(mdev, e);
		dec_unacked(mdev);
		return 1;
	}

	if (get_ldev_if_state(mdev, D_FAILED)) {
		drbd_rs_complete_io(mdev, e->sector);
=======
int w_e_end_rsdata_req(struct drbd_work *w, int cancel)
{
	struct drbd_peer_request *peer_req = container_of(w, struct drbd_peer_request, w);
	struct drbd_conf *mdev = w->mdev;
	int err;

	if (unlikely(cancel)) {
		drbd_free_peer_req(mdev, peer_req);
		dec_unacked(mdev);
		return 0;
	}

	if (get_ldev_if_state(mdev, D_FAILED)) {
		drbd_rs_complete_io(mdev, peer_req->i.sector);
>>>>>>> refs/remotes/origin/master
		put_ldev(mdev);
	}

	if (mdev->state.conn == C_AHEAD) {
<<<<<<< HEAD
		ok = drbd_send_ack(mdev, P_RS_CANCEL, e);
	} else if (likely((e->flags & EE_WAS_ERROR) == 0)) {
		if (likely(mdev->state.pdsk >= D_INCONSISTENT)) {
			inc_rs_pending(mdev);
			ok = drbd_send_block(mdev, P_RS_DATA_REPLY, e);
=======
		err = drbd_send_ack(mdev, P_RS_CANCEL, peer_req);
	} else if (likely((peer_req->flags & EE_WAS_ERROR) == 0)) {
		if (likely(mdev->state.pdsk >= D_INCONSISTENT)) {
			inc_rs_pending(mdev);
			err = drbd_send_block(mdev, P_RS_DATA_REPLY, peer_req);
>>>>>>> refs/remotes/origin/master
		} else {
			if (__ratelimit(&drbd_ratelimit_state))
				dev_err(DEV, "Not sending RSDataReply, "
				    "partner DISKLESS!\n");
<<<<<<< HEAD
			ok = 1;
=======
			err = 0;
>>>>>>> refs/remotes/origin/master
		}
	} else {
		if (__ratelimit(&drbd_ratelimit_state))
			dev_err(DEV, "Sending NegRSDReply. sector %llus.\n",
<<<<<<< HEAD
			    (unsigned long long)e->sector);

		ok = drbd_send_ack(mdev, P_NEG_RS_DREPLY, e);

		/* update resync data with failure */
		drbd_rs_failed_io(mdev, e->sector, e->size);
=======
			    (unsigned long long)peer_req->i.sector);

		err = drbd_send_ack(mdev, P_NEG_RS_DREPLY, peer_req);

		/* update resync data with failure */
		drbd_rs_failed_io(mdev, peer_req->i.sector, peer_req->i.size);
>>>>>>> refs/remotes/origin/master
	}

	dec_unacked(mdev);

<<<<<<< HEAD
	move_to_net_ee_or_free(mdev, e);

	if (unlikely(!ok))
		dev_err(DEV, "drbd_send_block() failed\n");
	return ok;
}

int w_e_end_csum_rs_req(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	struct drbd_epoch_entry *e = container_of(w, struct drbd_epoch_entry, w);
	struct digest_info *di;
	int digest_size;
	void *digest = NULL;
	int ok, eq = 0;

	if (unlikely(cancel)) {
		drbd_free_ee(mdev, e);
		dec_unacked(mdev);
		return 1;
	}

	if (get_ldev(mdev)) {
		drbd_rs_complete_io(mdev, e->sector);
		put_ldev(mdev);
	}

	di = e->digest;

	if (likely((e->flags & EE_WAS_ERROR) == 0)) {
		/* quick hack to try to avoid a race against reconfiguration.
		 * a real fix would be much more involved,
		 * introducing more locking mechanisms */
		if (mdev->csums_tfm) {
			digest_size = crypto_hash_digestsize(mdev->csums_tfm);
=======
	move_to_net_ee_or_free(mdev, peer_req);

	if (unlikely(err))
		dev_err(DEV, "drbd_send_block() failed\n");
	return err;
}

int w_e_end_csum_rs_req(struct drbd_work *w, int cancel)
{
	struct drbd_peer_request *peer_req = container_of(w, struct drbd_peer_request, w);
	struct drbd_conf *mdev = w->mdev;
	struct digest_info *di;
	int digest_size;
	void *digest = NULL;
	int err, eq = 0;

	if (unlikely(cancel)) {
		drbd_free_peer_req(mdev, peer_req);
		dec_unacked(mdev);
		return 0;
	}

	if (get_ldev(mdev)) {
		drbd_rs_complete_io(mdev, peer_req->i.sector);
		put_ldev(mdev);
	}

	di = peer_req->digest;

	if (likely((peer_req->flags & EE_WAS_ERROR) == 0)) {
		/* quick hack to try to avoid a race against reconfiguration.
		 * a real fix would be much more involved,
		 * introducing more locking mechanisms */
		if (mdev->tconn->csums_tfm) {
			digest_size = crypto_hash_digestsize(mdev->tconn->csums_tfm);
>>>>>>> refs/remotes/origin/master
			D_ASSERT(digest_size == di->digest_size);
			digest = kmalloc(digest_size, GFP_NOIO);
		}
		if (digest) {
<<<<<<< HEAD
			drbd_csum_ee(mdev, mdev->csums_tfm, e, digest);
=======
			drbd_csum_ee(mdev, mdev->tconn->csums_tfm, peer_req, digest);
>>>>>>> refs/remotes/origin/master
			eq = !memcmp(digest, di->digest, digest_size);
			kfree(digest);
		}

		if (eq) {
<<<<<<< HEAD
			drbd_set_in_sync(mdev, e->sector, e->size);
			/* rs_same_csums unit is BM_BLOCK_SIZE */
			mdev->rs_same_csum += e->size >> BM_BLOCK_SHIFT;
			ok = drbd_send_ack(mdev, P_RS_IS_IN_SYNC, e);
		} else {
			inc_rs_pending(mdev);
			e->block_id = ID_SYNCER; /* By setting block_id, digest pointer becomes invalid! */
			e->flags &= ~EE_HAS_DIGEST; /* This e no longer has a digest pointer */
			kfree(di);
			ok = drbd_send_block(mdev, P_RS_DATA_REPLY, e);
		}
	} else {
		ok = drbd_send_ack(mdev, P_NEG_RS_DREPLY, e);
=======
			drbd_set_in_sync(mdev, peer_req->i.sector, peer_req->i.size);
			/* rs_same_csums unit is BM_BLOCK_SIZE */
			mdev->rs_same_csum += peer_req->i.size >> BM_BLOCK_SHIFT;
			err = drbd_send_ack(mdev, P_RS_IS_IN_SYNC, peer_req);
		} else {
			inc_rs_pending(mdev);
			peer_req->block_id = ID_SYNCER; /* By setting block_id, digest pointer becomes invalid! */
			peer_req->flags &= ~EE_HAS_DIGEST; /* This peer request no longer has a digest pointer */
			kfree(di);
			err = drbd_send_block(mdev, P_RS_DATA_REPLY, peer_req);
		}
	} else {
		err = drbd_send_ack(mdev, P_NEG_RS_DREPLY, peer_req);
>>>>>>> refs/remotes/origin/master
		if (__ratelimit(&drbd_ratelimit_state))
			dev_err(DEV, "Sending NegDReply. I guess it gets messy.\n");
	}

	dec_unacked(mdev);
<<<<<<< HEAD
	move_to_net_ee_or_free(mdev, e);

	if (unlikely(!ok))
		dev_err(DEV, "drbd_send_block/ack() failed\n");
	return ok;
}

/* TODO merge common code with w_e_send_csum */
int w_e_end_ov_req(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	struct drbd_epoch_entry *e = container_of(w, struct drbd_epoch_entry, w);
	sector_t sector = e->sector;
	unsigned int size = e->size;
	int digest_size;
	void *digest;
	int ok = 1;
=======
	move_to_net_ee_or_free(mdev, peer_req);

	if (unlikely(err))
		dev_err(DEV, "drbd_send_block/ack() failed\n");
	return err;
}

int w_e_end_ov_req(struct drbd_work *w, int cancel)
{
	struct drbd_peer_request *peer_req = container_of(w, struct drbd_peer_request, w);
	struct drbd_conf *mdev = w->mdev;
	sector_t sector = peer_req->i.sector;
	unsigned int size = peer_req->i.size;
	int digest_size;
	void *digest;
	int err = 0;
>>>>>>> refs/remotes/origin/master

	if (unlikely(cancel))
		goto out;

<<<<<<< HEAD
	digest_size = crypto_hash_digestsize(mdev->verify_tfm);
	digest = kmalloc(digest_size, GFP_NOIO);
	if (!digest) {
		ok = 0;	/* terminate the connection in case the allocation failed */
		goto out;
	}

	if (likely(!(e->flags & EE_WAS_ERROR)))
		drbd_csum_ee(mdev, mdev->verify_tfm, e, digest);
=======
	digest_size = crypto_hash_digestsize(mdev->tconn->verify_tfm);
	digest = kmalloc(digest_size, GFP_NOIO);
	if (!digest) {
		err = 1;	/* terminate the connection in case the allocation failed */
		goto out;
	}

	if (likely(!(peer_req->flags & EE_WAS_ERROR)))
		drbd_csum_ee(mdev, mdev->tconn->verify_tfm, peer_req, digest);
>>>>>>> refs/remotes/origin/master
	else
		memset(digest, 0, digest_size);

	/* Free e and pages before send.
	 * In case we block on congestion, we could otherwise run into
	 * some distributed deadlock, if the other side blocks on
	 * congestion as well, because our receiver blocks in
<<<<<<< HEAD
	 * drbd_pp_alloc due to pp_in_use > max_buffers. */
	drbd_free_ee(mdev, e);
	e = NULL;
	inc_rs_pending(mdev);
	ok = drbd_send_drequest_csum(mdev, sector, size,
				     digest, digest_size,
				     P_OV_REPLY);
	if (!ok)
=======
	 * drbd_alloc_pages due to pp_in_use > max_buffers. */
	drbd_free_peer_req(mdev, peer_req);
	peer_req = NULL;
	inc_rs_pending(mdev);
	err = drbd_send_drequest_csum(mdev, sector, size, digest, digest_size, P_OV_REPLY);
	if (err)
>>>>>>> refs/remotes/origin/master
		dec_rs_pending(mdev);
	kfree(digest);

out:
<<<<<<< HEAD
	if (e)
		drbd_free_ee(mdev, e);
	dec_unacked(mdev);
	return ok;
}

void drbd_ov_oos_found(struct drbd_conf *mdev, sector_t sector, int size)
=======
	if (peer_req)
		drbd_free_peer_req(mdev, peer_req);
	dec_unacked(mdev);
	return err;
}

void drbd_ov_out_of_sync_found(struct drbd_conf *mdev, sector_t sector, int size)
>>>>>>> refs/remotes/origin/master
{
	if (mdev->ov_last_oos_start + mdev->ov_last_oos_size == sector) {
		mdev->ov_last_oos_size += size>>9;
	} else {
		mdev->ov_last_oos_start = sector;
		mdev->ov_last_oos_size = size>>9;
	}
	drbd_set_out_of_sync(mdev, sector, size);
}

<<<<<<< HEAD
int w_e_end_ov_reply(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	struct drbd_epoch_entry *e = container_of(w, struct drbd_epoch_entry, w);
	struct digest_info *di;
	void *digest;
	sector_t sector = e->sector;
	unsigned int size = e->size;
	int digest_size;
	int ok, eq = 0;

	if (unlikely(cancel)) {
		drbd_free_ee(mdev, e);
		dec_unacked(mdev);
		return 1;
=======
int w_e_end_ov_reply(struct drbd_work *w, int cancel)
{
	struct drbd_peer_request *peer_req = container_of(w, struct drbd_peer_request, w);
	struct drbd_conf *mdev = w->mdev;
	struct digest_info *di;
	void *digest;
	sector_t sector = peer_req->i.sector;
	unsigned int size = peer_req->i.size;
	int digest_size;
	int err, eq = 0;
	bool stop_sector_reached = false;

	if (unlikely(cancel)) {
		drbd_free_peer_req(mdev, peer_req);
		dec_unacked(mdev);
		return 0;
>>>>>>> refs/remotes/origin/master
	}

	/* after "cancel", because after drbd_disconnect/drbd_rs_cancel_all
	 * the resync lru has been cleaned up already */
	if (get_ldev(mdev)) {
<<<<<<< HEAD
		drbd_rs_complete_io(mdev, e->sector);
		put_ldev(mdev);
	}

	di = e->digest;

	if (likely((e->flags & EE_WAS_ERROR) == 0)) {
		digest_size = crypto_hash_digestsize(mdev->verify_tfm);
		digest = kmalloc(digest_size, GFP_NOIO);
		if (digest) {
			drbd_csum_ee(mdev, mdev->verify_tfm, e, digest);
=======
		drbd_rs_complete_io(mdev, peer_req->i.sector);
		put_ldev(mdev);
	}

	di = peer_req->digest;

	if (likely((peer_req->flags & EE_WAS_ERROR) == 0)) {
		digest_size = crypto_hash_digestsize(mdev->tconn->verify_tfm);
		digest = kmalloc(digest_size, GFP_NOIO);
		if (digest) {
			drbd_csum_ee(mdev, mdev->tconn->verify_tfm, peer_req, digest);
>>>>>>> refs/remotes/origin/master

			D_ASSERT(digest_size == di->digest_size);
			eq = !memcmp(digest, di->digest, digest_size);
			kfree(digest);
		}
	}

<<<<<<< HEAD
		/* Free e and pages before send.
		 * In case we block on congestion, we could otherwise run into
		 * some distributed deadlock, if the other side blocks on
		 * congestion as well, because our receiver blocks in
		 * drbd_pp_alloc due to pp_in_use > max_buffers. */
	drbd_free_ee(mdev, e);
	if (!eq)
		drbd_ov_oos_found(mdev, sector, size);
	else
		ov_oos_print(mdev);

	ok = drbd_send_ack_ex(mdev, P_OV_RESULT, sector, size,
			      eq ? ID_IN_SYNC : ID_OUT_OF_SYNC);
=======
	/* Free peer_req and pages before send.
	 * In case we block on congestion, we could otherwise run into
	 * some distributed deadlock, if the other side blocks on
	 * congestion as well, because our receiver blocks in
	 * drbd_alloc_pages due to pp_in_use > max_buffers. */
	drbd_free_peer_req(mdev, peer_req);
	if (!eq)
		drbd_ov_out_of_sync_found(mdev, sector, size);
	else
		ov_out_of_sync_print(mdev);

	err = drbd_send_ack_ex(mdev, P_OV_RESULT, sector, size,
			       eq ? ID_IN_SYNC : ID_OUT_OF_SYNC);
>>>>>>> refs/remotes/origin/master

	dec_unacked(mdev);

	--mdev->ov_left;

	/* let's advance progress step marks only for every other megabyte */
	if ((mdev->ov_left & 0x200) == 0x200)
		drbd_advance_rs_marks(mdev, mdev->ov_left);

<<<<<<< HEAD
	if (mdev->ov_left == 0) {
		ov_oos_print(mdev);
		drbd_resync_finished(mdev);
	}

	return ok;
}

int w_prev_work_done(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	struct drbd_wq_barrier *b = container_of(w, struct drbd_wq_barrier, w);
	complete(&b->done);
	return 1;
}

int w_send_barrier(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	struct drbd_tl_epoch *b = container_of(w, struct drbd_tl_epoch, w);
	struct p_barrier *p = &mdev->data.sbuf.barrier;
	int ok = 1;

	/* really avoid racing with tl_clear.  w.cb may have been referenced
	 * just before it was reassigned and re-queued, so double check that.
	 * actually, this race was harmless, since we only try to send the
	 * barrier packet here, and otherwise do nothing with the object.
	 * but compare with the head of w_clear_epoch */
	spin_lock_irq(&mdev->req_lock);
	if (w->cb != w_send_barrier || mdev->state.conn < C_CONNECTED)
		cancel = 1;
	spin_unlock_irq(&mdev->req_lock);
	if (cancel)
		return 1;

	if (!drbd_get_data_sock(mdev))
		return 0;
	p->barrier = b->br_number;
	/* inc_ap_pending was done where this was queued.
	 * dec_ap_pending will be done in got_BarrierAck
	 * or (on connection loss) in w_clear_epoch.  */
	ok = _drbd_send_cmd(mdev, mdev->data.socket, P_BARRIER,
				(struct p_header80 *)p, sizeof(*p), 0);
	drbd_put_data_sock(mdev);

	return ok;
}

int w_send_write_hint(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	if (cancel)
		return 1;
	return drbd_send_short_cmd(mdev, P_UNPLUG_REMOTE);
}

int w_send_oos(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	struct drbd_request *req = container_of(w, struct drbd_request, w);
	int ok;

	if (unlikely(cancel)) {
		req_mod(req, send_canceled);
		return 1;
	}

	ok = drbd_send_oos(mdev, req);
	req_mod(req, oos_handed_to_network);

	return ok;
=======
	stop_sector_reached = verify_can_do_stop_sector(mdev) &&
		(sector + (size>>9)) >= mdev->ov_stop_sector;

	if (mdev->ov_left == 0 || stop_sector_reached) {
		ov_out_of_sync_print(mdev);
		drbd_resync_finished(mdev);
	}

	return err;
}

int w_prev_work_done(struct drbd_work *w, int cancel)
{
	struct drbd_wq_barrier *b = container_of(w, struct drbd_wq_barrier, w);

	complete(&b->done);
	return 0;
}

/* FIXME
 * We need to track the number of pending barrier acks,
 * and to be able to wait for them.
 * See also comment in drbd_adm_attach before drbd_suspend_io.
 */
int drbd_send_barrier(struct drbd_tconn *tconn)
{
	struct p_barrier *p;
	struct drbd_socket *sock;

	sock = &tconn->data;
	p = conn_prepare_command(tconn, sock);
	if (!p)
		return -EIO;
	p->barrier = tconn->send.current_epoch_nr;
	p->pad = 0;
	tconn->send.current_epoch_writes = 0;

	return conn_send_command(tconn, sock, P_BARRIER, sizeof(*p), NULL, 0);
}

int w_send_write_hint(struct drbd_work *w, int cancel)
{
	struct drbd_conf *mdev = w->mdev;
	struct drbd_socket *sock;

	if (cancel)
		return 0;
	sock = &mdev->tconn->data;
	if (!drbd_prepare_command(mdev, sock))
		return -EIO;
	return drbd_send_command(mdev, sock, P_UNPLUG_REMOTE, 0, NULL, 0);
}

static void re_init_if_first_write(struct drbd_tconn *tconn, unsigned int epoch)
{
	if (!tconn->send.seen_any_write_yet) {
		tconn->send.seen_any_write_yet = true;
		tconn->send.current_epoch_nr = epoch;
		tconn->send.current_epoch_writes = 0;
	}
}

static void maybe_send_barrier(struct drbd_tconn *tconn, unsigned int epoch)
{
	/* re-init if first write on this connection */
	if (!tconn->send.seen_any_write_yet)
		return;
	if (tconn->send.current_epoch_nr != epoch) {
		if (tconn->send.current_epoch_writes)
			drbd_send_barrier(tconn);
		tconn->send.current_epoch_nr = epoch;
	}
}

int w_send_out_of_sync(struct drbd_work *w, int cancel)
{
	struct drbd_request *req = container_of(w, struct drbd_request, w);
	struct drbd_conf *mdev = w->mdev;
	struct drbd_tconn *tconn = mdev->tconn;
	int err;

	if (unlikely(cancel)) {
		req_mod(req, SEND_CANCELED);
		return 0;
	}

	/* this time, no tconn->send.current_epoch_writes++;
	 * If it was sent, it was the closing barrier for the last
	 * replicated epoch, before we went into AHEAD mode.
	 * No more barriers will be sent, until we leave AHEAD mode again. */
	maybe_send_barrier(tconn, req->epoch);

	err = drbd_send_out_of_sync(mdev, req);
	req_mod(req, OOS_HANDED_TO_NETWORK);

	return err;
>>>>>>> refs/remotes/origin/master
}

/**
 * w_send_dblock() - Worker callback to send a P_DATA packet in order to mirror a write request
 * @mdev:	DRBD device.
 * @w:		work object.
 * @cancel:	The connection will be closed anyways
 */
<<<<<<< HEAD
int w_send_dblock(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	struct drbd_request *req = container_of(w, struct drbd_request, w);
	int ok;

	if (unlikely(cancel)) {
		req_mod(req, send_canceled);
		return 1;
	}

	ok = drbd_send_dblock(mdev, req);
	req_mod(req, ok ? handed_over_to_network : send_failed);

	return ok;
=======
int w_send_dblock(struct drbd_work *w, int cancel)
{
	struct drbd_request *req = container_of(w, struct drbd_request, w);
	struct drbd_conf *mdev = w->mdev;
	struct drbd_tconn *tconn = mdev->tconn;
	int err;

	if (unlikely(cancel)) {
		req_mod(req, SEND_CANCELED);
		return 0;
	}

	re_init_if_first_write(tconn, req->epoch);
	maybe_send_barrier(tconn, req->epoch);
	tconn->send.current_epoch_writes++;

	err = drbd_send_dblock(mdev, req);
	req_mod(req, err ? SEND_FAILED : HANDED_OVER_TO_NETWORK);

	return err;
>>>>>>> refs/remotes/origin/master
}

/**
 * w_send_read_req() - Worker callback to send a read request (P_DATA_REQUEST) packet
 * @mdev:	DRBD device.
 * @w:		work object.
 * @cancel:	The connection will be closed anyways
 */
<<<<<<< HEAD
int w_send_read_req(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	struct drbd_request *req = container_of(w, struct drbd_request, w);
	int ok;

	if (unlikely(cancel)) {
		req_mod(req, send_canceled);
		return 1;
	}

	ok = drbd_send_drequest(mdev, P_DATA_REQUEST, req->sector, req->size,
				(unsigned long)req);

	if (!ok) {
		/* ?? we set C_TIMEOUT or C_BROKEN_PIPE in drbd_send();
		 * so this is probably redundant */
		if (mdev->state.conn >= C_CONNECTED)
			drbd_force_state(mdev, NS(conn, C_NETWORK_FAILURE));
	}
	req_mod(req, ok ? handed_over_to_network : send_failed);

	return ok;
}

int w_restart_disk_io(struct drbd_conf *mdev, struct drbd_work *w, int cancel)
{
	struct drbd_request *req = container_of(w, struct drbd_request, w);

	if (bio_data_dir(req->master_bio) == WRITE && req->rq_state & RQ_IN_ACT_LOG)
		drbd_al_begin_io(mdev, req->sector);
	/* Calling drbd_al_begin_io() out of the worker might deadlocks
	   theoretically. Practically it can not deadlock, since this is
	   only used when unfreezing IOs. All the extents of the requests
	   that made it into the TL are already active */
=======
int w_send_read_req(struct drbd_work *w, int cancel)
{
	struct drbd_request *req = container_of(w, struct drbd_request, w);
	struct drbd_conf *mdev = w->mdev;
	struct drbd_tconn *tconn = mdev->tconn;
	int err;

	if (unlikely(cancel)) {
		req_mod(req, SEND_CANCELED);
		return 0;
	}

	/* Even read requests may close a write epoch,
	 * if there was any yet. */
	maybe_send_barrier(tconn, req->epoch);

	err = drbd_send_drequest(mdev, P_DATA_REQUEST, req->i.sector, req->i.size,
				 (unsigned long)req);

	req_mod(req, err ? SEND_FAILED : HANDED_OVER_TO_NETWORK);

	return err;
}

int w_restart_disk_io(struct drbd_work *w, int cancel)
{
	struct drbd_request *req = container_of(w, struct drbd_request, w);
	struct drbd_conf *mdev = w->mdev;

	if (bio_data_dir(req->master_bio) == WRITE && req->rq_state & RQ_IN_ACT_LOG)
		drbd_al_begin_io(mdev, &req->i, false);
>>>>>>> refs/remotes/origin/master

	drbd_req_make_private_bio(req, req->master_bio);
	req->private_bio->bi_bdev = mdev->ldev->backing_bdev;
	generic_make_request(req->private_bio);

<<<<<<< HEAD
	return 1;
=======
	return 0;
>>>>>>> refs/remotes/origin/master
}

static int _drbd_may_sync_now(struct drbd_conf *mdev)
{
	struct drbd_conf *odev = mdev;
<<<<<<< HEAD

	while (1) {
		if (odev->sync_conf.after == -1)
			return 1;
		odev = minor_to_mdev(odev->sync_conf.after);
		ERR_IF(!odev) return 1;
=======
	int resync_after;

	while (1) {
		if (!odev->ldev || odev->state.disk == D_DISKLESS)
			return 1;
		rcu_read_lock();
		resync_after = rcu_dereference(odev->ldev->disk_conf)->resync_after;
		rcu_read_unlock();
		if (resync_after == -1)
			return 1;
		odev = minor_to_mdev(resync_after);
		if (!odev)
			return 1;
>>>>>>> refs/remotes/origin/master
		if ((odev->state.conn >= C_SYNC_SOURCE &&
		     odev->state.conn <= C_PAUSED_SYNC_T) ||
		    odev->state.aftr_isp || odev->state.peer_isp ||
		    odev->state.user_isp)
			return 0;
	}
}

/**
 * _drbd_pause_after() - Pause resync on all devices that may not resync now
 * @mdev:	DRBD device.
 *
 * Called from process context only (admin command and after_state_ch).
 */
static int _drbd_pause_after(struct drbd_conf *mdev)
{
	struct drbd_conf *odev;
	int i, rv = 0;

<<<<<<< HEAD
	for (i = 0; i < minor_count; i++) {
		odev = minor_to_mdev(i);
		if (!odev)
			continue;
=======
	rcu_read_lock();
	idr_for_each_entry(&minors, odev, i) {
>>>>>>> refs/remotes/origin/master
		if (odev->state.conn == C_STANDALONE && odev->state.disk == D_DISKLESS)
			continue;
		if (!_drbd_may_sync_now(odev))
			rv |= (__drbd_set_state(_NS(odev, aftr_isp, 1), CS_HARD, NULL)
			       != SS_NOTHING_TO_DO);
	}
<<<<<<< HEAD
=======
	rcu_read_unlock();
>>>>>>> refs/remotes/origin/master

	return rv;
}

/**
 * _drbd_resume_next() - Resume resync on all devices that may resync now
 * @mdev:	DRBD device.
 *
 * Called from process context only (admin command and worker).
 */
static int _drbd_resume_next(struct drbd_conf *mdev)
{
	struct drbd_conf *odev;
	int i, rv = 0;

<<<<<<< HEAD
	for (i = 0; i < minor_count; i++) {
		odev = minor_to_mdev(i);
		if (!odev)
			continue;
=======
	rcu_read_lock();
	idr_for_each_entry(&minors, odev, i) {
>>>>>>> refs/remotes/origin/master
		if (odev->state.conn == C_STANDALONE && odev->state.disk == D_DISKLESS)
			continue;
		if (odev->state.aftr_isp) {
			if (_drbd_may_sync_now(odev))
				rv |= (__drbd_set_state(_NS(odev, aftr_isp, 0),
							CS_HARD, NULL)
				       != SS_NOTHING_TO_DO) ;
		}
	}
<<<<<<< HEAD
=======
	rcu_read_unlock();
>>>>>>> refs/remotes/origin/master
	return rv;
}

void resume_next_sg(struct drbd_conf *mdev)
{
	write_lock_irq(&global_state_lock);
	_drbd_resume_next(mdev);
	write_unlock_irq(&global_state_lock);
}

void suspend_other_sg(struct drbd_conf *mdev)
{
	write_lock_irq(&global_state_lock);
	_drbd_pause_after(mdev);
	write_unlock_irq(&global_state_lock);
}

<<<<<<< HEAD
static int sync_after_error(struct drbd_conf *mdev, int o_minor)
{
	struct drbd_conf *odev;

	if (o_minor == -1)
		return NO_ERROR;
	if (o_minor < -1 || minor_to_mdev(o_minor) == NULL)
		return ERR_SYNC_AFTER;
=======
/* caller must hold global_state_lock */
enum drbd_ret_code drbd_resync_after_valid(struct drbd_conf *mdev, int o_minor)
{
	struct drbd_conf *odev;
	int resync_after;

	if (o_minor == -1)
		return NO_ERROR;
	if (o_minor < -1 || o_minor > MINORMASK)
		return ERR_RESYNC_AFTER;
>>>>>>> refs/remotes/origin/master

	/* check for loops */
	odev = minor_to_mdev(o_minor);
	while (1) {
		if (odev == mdev)
<<<<<<< HEAD
			return ERR_SYNC_AFTER_CYCLE;

		/* dependency chain ends here, no cycles. */
		if (odev->sync_conf.after == -1)
			return NO_ERROR;

		/* follow the dependency chain */
		odev = minor_to_mdev(odev->sync_conf.after);
	}
}

int drbd_alter_sa(struct drbd_conf *mdev, int na)
{
	int changes;
	int retcode;

	write_lock_irq(&global_state_lock);
	retcode = sync_after_error(mdev, na);
	if (retcode == NO_ERROR) {
		mdev->sync_conf.after = na;
		do {
			changes  = _drbd_pause_after(mdev);
			changes |= _drbd_resume_next(mdev);
		} while (changes);
	}
	write_unlock_irq(&global_state_lock);
	return retcode;
=======
			return ERR_RESYNC_AFTER_CYCLE;

		/* You are free to depend on diskless, non-existing,
		 * or not yet/no longer existing minors.
		 * We only reject dependency loops.
		 * We cannot follow the dependency chain beyond a detached or
		 * missing minor.
		 */
		if (!odev || !odev->ldev || odev->state.disk == D_DISKLESS)
			return NO_ERROR;

		rcu_read_lock();
		resync_after = rcu_dereference(odev->ldev->disk_conf)->resync_after;
		rcu_read_unlock();
		/* dependency chain ends here, no cycles. */
		if (resync_after == -1)
			return NO_ERROR;

		/* follow the dependency chain */
		odev = minor_to_mdev(resync_after);
	}
}

/* caller must hold global_state_lock */
void drbd_resync_after_changed(struct drbd_conf *mdev)
{
	int changes;

	do {
		changes  = _drbd_pause_after(mdev);
		changes |= _drbd_resume_next(mdev);
	} while (changes);
>>>>>>> refs/remotes/origin/master
}

void drbd_rs_controller_reset(struct drbd_conf *mdev)
{
<<<<<<< HEAD
	atomic_set(&mdev->rs_sect_in, 0);
	atomic_set(&mdev->rs_sect_ev, 0);
	mdev->rs_in_flight = 0;
	mdev->rs_planed = 0;
	spin_lock(&mdev->peer_seq_lock);
	fifo_set(&mdev->rs_plan_s, 0);
	spin_unlock(&mdev->peer_seq_lock);
=======
	struct fifo_buffer *plan;

	atomic_set(&mdev->rs_sect_in, 0);
	atomic_set(&mdev->rs_sect_ev, 0);
	mdev->rs_in_flight = 0;

	/* Updating the RCU protected object in place is necessary since
	   this function gets called from atomic context.
	   It is valid since all other updates also lead to an completely
	   empty fifo */
	rcu_read_lock();
	plan = rcu_dereference(mdev->rs_plan_s);
	plan->total = 0;
	fifo_set(plan, 0);
	rcu_read_unlock();
}

void start_resync_timer_fn(unsigned long data)
{
	struct drbd_conf *mdev = (struct drbd_conf *) data;

	drbd_queue_work(&mdev->tconn->sender_work, &mdev->start_resync_work);
}

int w_start_resync(struct drbd_work *w, int cancel)
{
	struct drbd_conf *mdev = w->mdev;

	if (atomic_read(&mdev->unacked_cnt) || atomic_read(&mdev->rs_pending_cnt)) {
		dev_warn(DEV, "w_start_resync later...\n");
		mdev->start_resync_timer.expires = jiffies + HZ/10;
		add_timer(&mdev->start_resync_timer);
		return 0;
	}

	drbd_start_resync(mdev, C_SYNC_SOURCE);
	clear_bit(AHEAD_TO_SYNC_SOURCE, &mdev->flags);
	return 0;
>>>>>>> refs/remotes/origin/master
}

/**
 * drbd_start_resync() - Start the resync process
 * @mdev:	DRBD device.
 * @side:	Either C_SYNC_SOURCE or C_SYNC_TARGET
 *
 * This function might bring you directly into one of the
 * C_PAUSED_SYNC_* states.
 */
void drbd_start_resync(struct drbd_conf *mdev, enum drbd_conns side)
{
	union drbd_state ns;
	int r;

	if (mdev->state.conn >= C_SYNC_SOURCE && mdev->state.conn < C_AHEAD) {
		dev_err(DEV, "Resync already running!\n");
		return;
	}

<<<<<<< HEAD
	if (mdev->state.conn < C_AHEAD) {
		/* In case a previous resync run was aborted by an IO error/detach on the peer. */
		drbd_rs_cancel_all(mdev);
		/* This should be done when we abort the resync. We definitely do not
		   want to have this for connections going back and forth between
		   Ahead/Behind and SyncSource/SyncTarget */
	}

	if (side == C_SYNC_TARGET) {
		/* Since application IO was locked out during C_WF_BITMAP_T and
		   C_WF_SYNC_UUID we are still unmodified. Before going to C_SYNC_TARGET
		   we check that we might make the data inconsistent. */
		r = drbd_khelper(mdev, "before-resync-target");
		r = (r >> 8) & 0xff;
		if (r > 0) {
			dev_info(DEV, "before-resync-target handler returned %d, "
			     "dropping connection.\n", r);
			drbd_force_state(mdev, NS(conn, C_DISCONNECTING));
			return;
		}
	} else /* C_SYNC_SOURCE */ {
		r = drbd_khelper(mdev, "before-resync-source");
		r = (r >> 8) & 0xff;
		if (r > 0) {
			if (r == 3) {
				dev_info(DEV, "before-resync-source handler returned %d, "
					 "ignoring. Old userland tools?", r);
			} else {
				dev_info(DEV, "before-resync-source handler returned %d, "
					 "dropping connection.\n", r);
				drbd_force_state(mdev, NS(conn, C_DISCONNECTING));
				return;
			}
		}
	}

	drbd_state_lock(mdev);

	if (!get_ldev_if_state(mdev, D_NEGOTIATING)) {
		drbd_state_unlock(mdev);
		return;
	}

	write_lock_irq(&global_state_lock);
	ns = mdev->state;
=======
	if (!test_bit(B_RS_H_DONE, &mdev->flags)) {
		if (side == C_SYNC_TARGET) {
			/* Since application IO was locked out during C_WF_BITMAP_T and
			   C_WF_SYNC_UUID we are still unmodified. Before going to C_SYNC_TARGET
			   we check that we might make the data inconsistent. */
			r = drbd_khelper(mdev, "before-resync-target");
			r = (r >> 8) & 0xff;
			if (r > 0) {
				dev_info(DEV, "before-resync-target handler returned %d, "
					 "dropping connection.\n", r);
				conn_request_state(mdev->tconn, NS(conn, C_DISCONNECTING), CS_HARD);
				return;
			}
		} else /* C_SYNC_SOURCE */ {
			r = drbd_khelper(mdev, "before-resync-source");
			r = (r >> 8) & 0xff;
			if (r > 0) {
				if (r == 3) {
					dev_info(DEV, "before-resync-source handler returned %d, "
						 "ignoring. Old userland tools?", r);
				} else {
					dev_info(DEV, "before-resync-source handler returned %d, "
						 "dropping connection.\n", r);
					conn_request_state(mdev->tconn, NS(conn, C_DISCONNECTING), CS_HARD);
					return;
				}
			}
		}
	}

	if (current == mdev->tconn->worker.task) {
		/* The worker should not sleep waiting for state_mutex,
		   that can take long */
		if (!mutex_trylock(mdev->state_mutex)) {
			set_bit(B_RS_H_DONE, &mdev->flags);
			mdev->start_resync_timer.expires = jiffies + HZ/5;
			add_timer(&mdev->start_resync_timer);
			return;
		}
	} else {
		mutex_lock(mdev->state_mutex);
	}
	clear_bit(B_RS_H_DONE, &mdev->flags);

	write_lock_irq(&global_state_lock);
	/* Did some connection breakage or IO error race with us? */
	if (mdev->state.conn < C_CONNECTED
	|| !get_ldev_if_state(mdev, D_NEGOTIATING)) {
		write_unlock_irq(&global_state_lock);
		mutex_unlock(mdev->state_mutex);
		return;
	}

	ns = drbd_read_state(mdev);
>>>>>>> refs/remotes/origin/master

	ns.aftr_isp = !_drbd_may_sync_now(mdev);

	ns.conn = side;

	if (side == C_SYNC_TARGET)
		ns.disk = D_INCONSISTENT;
	else /* side == C_SYNC_SOURCE */
		ns.pdsk = D_INCONSISTENT;

	r = __drbd_set_state(mdev, ns, CS_VERBOSE, NULL);
<<<<<<< HEAD
	ns = mdev->state;
=======
	ns = drbd_read_state(mdev);
>>>>>>> refs/remotes/origin/master

	if (ns.conn < C_CONNECTED)
		r = SS_UNKNOWN_ERROR;

	if (r == SS_SUCCESS) {
		unsigned long tw = drbd_bm_total_weight(mdev);
		unsigned long now = jiffies;
		int i;

		mdev->rs_failed    = 0;
		mdev->rs_paused    = 0;
		mdev->rs_same_csum = 0;
		mdev->rs_last_events = 0;
		mdev->rs_last_sect_ev = 0;
		mdev->rs_total     = tw;
		mdev->rs_start     = now;
		for (i = 0; i < DRBD_SYNC_MARKS; i++) {
			mdev->rs_mark_left[i] = tw;
			mdev->rs_mark_time[i] = now;
		}
		_drbd_pause_after(mdev);
	}
	write_unlock_irq(&global_state_lock);

	if (r == SS_SUCCESS) {
<<<<<<< HEAD
=======
		/* reset rs_last_bcast when a resync or verify is started,
		 * to deal with potential jiffies wrap. */
		mdev->rs_last_bcast = jiffies - HZ;

>>>>>>> refs/remotes/origin/master
		dev_info(DEV, "Began resync as %s (will sync %lu KB [%lu bits set]).\n",
		     drbd_conn_str(ns.conn),
		     (unsigned long) mdev->rs_total << (BM_BLOCK_SHIFT-10),
		     (unsigned long) mdev->rs_total);
		if (side == C_SYNC_TARGET)
			mdev->bm_resync_fo = 0;

		/* Since protocol 96, we must serialize drbd_gen_and_send_sync_uuid
		 * with w_send_oos, or the sync target will get confused as to
		 * how much bits to resync.  We cannot do that always, because for an
		 * empty resync and protocol < 95, we need to do it here, as we call
		 * drbd_resync_finished from here in that case.
		 * We drbd_gen_and_send_sync_uuid here for protocol < 96,
		 * and from after_state_ch otherwise. */
<<<<<<< HEAD
		if (side == C_SYNC_SOURCE && mdev->agreed_pro_version < 96)
			drbd_gen_and_send_sync_uuid(mdev);

		if (mdev->agreed_pro_version < 95 && mdev->rs_total == 0) {
=======
		if (side == C_SYNC_SOURCE && mdev->tconn->agreed_pro_version < 96)
			drbd_gen_and_send_sync_uuid(mdev);

		if (mdev->tconn->agreed_pro_version < 95 && mdev->rs_total == 0) {
>>>>>>> refs/remotes/origin/master
			/* This still has a race (about when exactly the peers
			 * detect connection loss) that can lead to a full sync
			 * on next handshake. In 8.3.9 we fixed this with explicit
			 * resync-finished notifications, but the fix
			 * introduces a protocol change.  Sleeping for some
			 * time longer than the ping interval + timeout on the
			 * SyncSource, to give the SyncTarget the chance to
			 * detect connection loss, then waiting for a ping
			 * response (implicit in drbd_resync_finished) reduces
			 * the race considerably, but does not solve it. */
<<<<<<< HEAD
			if (side == C_SYNC_SOURCE)
				schedule_timeout_interruptible(
					mdev->net_conf->ping_int * HZ +
					mdev->net_conf->ping_timeo*HZ/9);
=======
			if (side == C_SYNC_SOURCE) {
				struct net_conf *nc;
				int timeo;

				rcu_read_lock();
				nc = rcu_dereference(mdev->tconn->net_conf);
				timeo = nc->ping_int * HZ + nc->ping_timeo * HZ / 9;
				rcu_read_unlock();
				schedule_timeout_interruptible(timeo);
			}
>>>>>>> refs/remotes/origin/master
			drbd_resync_finished(mdev);
		}

		drbd_rs_controller_reset(mdev);
		/* ns.conn may already be != mdev->state.conn,
		 * we may have been paused in between, or become paused until
		 * the timer triggers.
		 * No matter, that is handled in resync_timer_fn() */
		if (ns.conn == C_SYNC_TARGET)
			mod_timer(&mdev->resync_timer, jiffies);

		drbd_md_sync(mdev);
	}
	put_ldev(mdev);
<<<<<<< HEAD
	drbd_state_unlock(mdev);
}

int drbd_worker(struct drbd_thread *thi)
{
	struct drbd_conf *mdev = thi->mdev;
	struct drbd_work *w = NULL;
	LIST_HEAD(work_list);
	int intr = 0, i;

	sprintf(current->comm, "drbd%d_worker", mdev_to_minor(mdev));

	while (get_t_state(thi) == Running) {
		drbd_thread_current_set_cpu(mdev);

		if (down_trylock(&mdev->data.work.s)) {
			mutex_lock(&mdev->data.mutex);
			if (mdev->data.socket && !mdev->net_conf->no_cork)
				drbd_tcp_uncork(mdev->data.socket);
			mutex_unlock(&mdev->data.mutex);

			intr = down_interruptible(&mdev->data.work.s);

			mutex_lock(&mdev->data.mutex);
			if (mdev->data.socket  && !mdev->net_conf->no_cork)
				drbd_tcp_cork(mdev->data.socket);
			mutex_unlock(&mdev->data.mutex);
		}

		if (intr) {
			D_ASSERT(intr == -EINTR);
			flush_signals(current);
			ERR_IF (get_t_state(thi) == Running)
				continue;
			break;
		}

		if (get_t_state(thi) != Running)
			break;
		/* With this break, we have done a down() but not consumed
		   the entry from the list. The cleanup code takes care of
		   this...   */

		w = NULL;
		spin_lock_irq(&mdev->data.work.q_lock);
		ERR_IF(list_empty(&mdev->data.work.q)) {
			/* something terribly wrong in our logic.
			 * we were able to down() the semaphore,
			 * but the list is empty... doh.
			 *
			 * what is the best thing to do now?
			 * try again from scratch, restarting the receiver,
			 * asender, whatnot? could break even more ugly,
			 * e.g. when we are primary, but no good local data.
			 *
			 * I'll try to get away just starting over this loop.
			 */
			spin_unlock_irq(&mdev->data.work.q_lock);
			continue;
		}
		w = list_entry(mdev->data.work.q.next, struct drbd_work, list);
		list_del_init(&w->list);
		spin_unlock_irq(&mdev->data.work.q_lock);

		if (!w->cb(mdev, w, mdev->state.conn < C_CONNECTED)) {
			/* dev_warn(DEV, "worker: a callback failed! \n"); */
			if (mdev->state.conn >= C_CONNECTED)
				drbd_force_state(mdev,
						NS(conn, C_NETWORK_FAILURE));
		}
	}
	D_ASSERT(test_bit(DEVICE_DYING, &mdev->flags));
	D_ASSERT(test_bit(CONFIG_PENDING, &mdev->flags));

	spin_lock_irq(&mdev->data.work.q_lock);
	i = 0;
	while (!list_empty(&mdev->data.work.q)) {
		list_splice_init(&mdev->data.work.q, &work_list);
		spin_unlock_irq(&mdev->data.work.q_lock);

		while (!list_empty(&work_list)) {
			w = list_entry(work_list.next, struct drbd_work, list);
			list_del_init(&w->list);
			w->cb(mdev, w, 1);
			i++; /* dead debugging code */
		}

		spin_lock_irq(&mdev->data.work.q_lock);
	}
	sema_init(&mdev->data.work.s, 0);
	/* DANGEROUS race: if someone did queue his work within the spinlock,
	 * but up() ed outside the spinlock, we could get an up() on the
	 * semaphore without corresponding list entry.
	 * So don't do that.
	 */
	spin_unlock_irq(&mdev->data.work.q_lock);

	D_ASSERT(mdev->state.disk == D_DISKLESS && mdev->state.conn == C_STANDALONE);
	/* _drbd_set_state only uses stop_nowait.
	 * wait here for the Exiting receiver. */
	drbd_thread_stop(&mdev->receiver);
	drbd_mdev_cleanup(mdev);

	dev_info(DEV, "worker terminated\n");

	clear_bit(DEVICE_DYING, &mdev->flags);
	clear_bit(CONFIG_PENDING, &mdev->flags);
	wake_up(&mdev->state_wait);
=======
	mutex_unlock(mdev->state_mutex);
}

/* If the resource already closed the current epoch, but we did not
 * (because we have not yet seen new requests), we should send the
 * corresponding barrier now.  Must be checked within the same spinlock
 * that is used to check for new requests. */
bool need_to_send_barrier(struct drbd_tconn *connection)
{
	if (!connection->send.seen_any_write_yet)
		return false;

	/* Skip barriers that do not contain any writes.
	 * This may happen during AHEAD mode. */
	if (!connection->send.current_epoch_writes)
		return false;

	/* ->req_lock is held when requests are queued on
	 * connection->sender_work, and put into ->transfer_log.
	 * It is also held when ->current_tle_nr is increased.
	 * So either there are already new requests queued,
	 * and corresponding barriers will be send there.
	 * Or nothing new is queued yet, so the difference will be 1.
	 */
	if (atomic_read(&connection->current_tle_nr) !=
	    connection->send.current_epoch_nr + 1)
		return false;

	return true;
}

bool dequeue_work_batch(struct drbd_work_queue *queue, struct list_head *work_list)
{
	spin_lock_irq(&queue->q_lock);
	list_splice_init(&queue->q, work_list);
	spin_unlock_irq(&queue->q_lock);
	return !list_empty(work_list);
}

bool dequeue_work_item(struct drbd_work_queue *queue, struct list_head *work_list)
{
	spin_lock_irq(&queue->q_lock);
	if (!list_empty(&queue->q))
		list_move(queue->q.next, work_list);
	spin_unlock_irq(&queue->q_lock);
	return !list_empty(work_list);
}

void wait_for_work(struct drbd_tconn *connection, struct list_head *work_list)
{
	DEFINE_WAIT(wait);
	struct net_conf *nc;
	int uncork, cork;

	dequeue_work_item(&connection->sender_work, work_list);
	if (!list_empty(work_list))
		return;

	/* Still nothing to do?
	 * Maybe we still need to close the current epoch,
	 * even if no new requests are queued yet.
	 *
	 * Also, poke TCP, just in case.
	 * Then wait for new work (or signal). */
	rcu_read_lock();
	nc = rcu_dereference(connection->net_conf);
	uncork = nc ? nc->tcp_cork : 0;
	rcu_read_unlock();
	if (uncork) {
		mutex_lock(&connection->data.mutex);
		if (connection->data.socket)
			drbd_tcp_uncork(connection->data.socket);
		mutex_unlock(&connection->data.mutex);
	}

	for (;;) {
		int send_barrier;
		prepare_to_wait(&connection->sender_work.q_wait, &wait, TASK_INTERRUPTIBLE);
		spin_lock_irq(&connection->req_lock);
		spin_lock(&connection->sender_work.q_lock);	/* FIXME get rid of this one? */
		/* dequeue single item only,
		 * we still use drbd_queue_work_front() in some places */
		if (!list_empty(&connection->sender_work.q))
			list_move(connection->sender_work.q.next, work_list);
		spin_unlock(&connection->sender_work.q_lock);	/* FIXME get rid of this one? */
		if (!list_empty(work_list) || signal_pending(current)) {
			spin_unlock_irq(&connection->req_lock);
			break;
		}
		send_barrier = need_to_send_barrier(connection);
		spin_unlock_irq(&connection->req_lock);
		if (send_barrier) {
			drbd_send_barrier(connection);
			connection->send.current_epoch_nr++;
		}
		schedule();
		/* may be woken up for other things but new work, too,
		 * e.g. if the current epoch got closed.
		 * In which case we send the barrier above. */
	}
	finish_wait(&connection->sender_work.q_wait, &wait);

	/* someone may have changed the config while we have been waiting above. */
	rcu_read_lock();
	nc = rcu_dereference(connection->net_conf);
	cork = nc ? nc->tcp_cork : 0;
	rcu_read_unlock();
	mutex_lock(&connection->data.mutex);
	if (connection->data.socket) {
		if (cork)
			drbd_tcp_cork(connection->data.socket);
		else if (!uncork)
			drbd_tcp_uncork(connection->data.socket);
	}
	mutex_unlock(&connection->data.mutex);
}

int drbd_worker(struct drbd_thread *thi)
{
	struct drbd_tconn *tconn = thi->tconn;
	struct drbd_work *w = NULL;
	struct drbd_conf *mdev;
	LIST_HEAD(work_list);
	int vnr;

	while (get_t_state(thi) == RUNNING) {
		drbd_thread_current_set_cpu(thi);

		/* as long as we use drbd_queue_work_front(),
		 * we may only dequeue single work items here, not batches. */
		if (list_empty(&work_list))
			wait_for_work(tconn, &work_list);

		if (signal_pending(current)) {
			flush_signals(current);
			if (get_t_state(thi) == RUNNING) {
				conn_warn(tconn, "Worker got an unexpected signal\n");
				continue;
			}
			break;
		}

		if (get_t_state(thi) != RUNNING)
			break;

		while (!list_empty(&work_list)) {
			w = list_first_entry(&work_list, struct drbd_work, list);
			list_del_init(&w->list);
			if (w->cb(w, tconn->cstate < C_WF_REPORT_PARAMS) == 0)
				continue;
			if (tconn->cstate >= C_WF_REPORT_PARAMS)
				conn_request_state(tconn, NS(conn, C_NETWORK_FAILURE), CS_HARD);
		}
	}

	do {
		while (!list_empty(&work_list)) {
			w = list_first_entry(&work_list, struct drbd_work, list);
			list_del_init(&w->list);
			w->cb(w, 1);
		}
		dequeue_work_batch(&tconn->sender_work, &work_list);
	} while (!list_empty(&work_list));

	rcu_read_lock();
	idr_for_each_entry(&tconn->volumes, mdev, vnr) {
		D_ASSERT(mdev->state.disk == D_DISKLESS && mdev->state.conn == C_STANDALONE);
		kref_get(&mdev->kref);
		rcu_read_unlock();
		drbd_mdev_cleanup(mdev);
		kref_put(&mdev->kref, &drbd_minor_destroy);
		rcu_read_lock();
	}
	rcu_read_unlock();
>>>>>>> refs/remotes/origin/master

	return 0;
}
