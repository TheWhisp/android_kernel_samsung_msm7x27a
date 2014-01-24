/*
   drbd.c

   This file is part of DRBD by Philipp Reisner and Lars Ellenberg.

   Copyright (C) 2001-2008, LINBIT Information Technologies GmbH.
   Copyright (C) 1999-2008, Philipp Reisner <philipp.reisner@linbit.com>.
   Copyright (C) 2002-2008, Lars Ellenberg <lars.ellenberg@linbit.com>.

   Thanks to Carter Burden, Bart Grantham and Gennadiy Nerubayev
   from Logicworks, Inc. for making SDP replication support possible.

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
#include <asm/uaccess.h>
#include <asm/types.h>
#include <net/sock.h>
#include <linux/ctype.h>
#include <linux/mutex.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/memcontrol.h>
#include <linux/mm_inline.h>
#include <linux/slab.h>
#include <linux/random.h>
#include <linux/reboot.h>
#include <linux/notifier.h>
#include <linux/kthread.h>
<<<<<<< HEAD

=======
#include <linux/workqueue.h>
>>>>>>> refs/remotes/origin/master
#define __KERNEL_SYSCALLS__
#include <linux/unistd.h>
#include <linux/vmalloc.h>

#include <linux/drbd_limits.h>
#include "drbd_int.h"
#include "drbd_req.h" /* only for _req_mod in tl_release and tl_clear */

#include "drbd_vli.h"

<<<<<<< HEAD
struct after_state_chg_work {
	struct drbd_work w;
	union drbd_state os;
	union drbd_state ns;
	enum chg_state_flags flags;
	struct completion *done;
};

=======
>>>>>>> refs/remotes/origin/master
static DEFINE_MUTEX(drbd_main_mutex);
int drbdd_init(struct drbd_thread *);
int drbd_worker(struct drbd_thread *);
int drbd_asender(struct drbd_thread *);

int drbd_init(void);
static int drbd_open(struct block_device *bdev, fmode_t mode);
<<<<<<< HEAD
static int drbd_release(struct gendisk *gd, fmode_t mode);
static int w_after_state_ch(struct drbd_conf *mdev, struct drbd_work *w, int unused);
static void after_state_ch(struct drbd_conf *mdev, union drbd_state os,
			   union drbd_state ns, enum chg_state_flags flags);
static int w_md_sync(struct drbd_conf *mdev, struct drbd_work *w, int unused);
static void md_sync_timer_fn(unsigned long data);
static int w_bitmap_io(struct drbd_conf *mdev, struct drbd_work *w, int unused);
static int w_go_diskless(struct drbd_conf *mdev, struct drbd_work *w, int unused);
=======
static void drbd_release(struct gendisk *gd, fmode_t mode);
static int w_md_sync(struct drbd_work *w, int unused);
static void md_sync_timer_fn(unsigned long data);
static int w_bitmap_io(struct drbd_work *w, int unused);
static int w_go_diskless(struct drbd_work *w, int unused);
>>>>>>> refs/remotes/origin/master

MODULE_AUTHOR("Philipp Reisner <phil@linbit.com>, "
	      "Lars Ellenberg <lars@linbit.com>");
MODULE_DESCRIPTION("drbd - Distributed Replicated Block Device v" REL_VERSION);
MODULE_VERSION(REL_VERSION);
MODULE_LICENSE("GPL");
<<<<<<< HEAD
MODULE_PARM_DESC(minor_count, "Maximum number of drbd devices ("
=======
MODULE_PARM_DESC(minor_count, "Approximate number of drbd devices ("
>>>>>>> refs/remotes/origin/master
		 __stringify(DRBD_MINOR_COUNT_MIN) "-" __stringify(DRBD_MINOR_COUNT_MAX) ")");
MODULE_ALIAS_BLOCKDEV_MAJOR(DRBD_MAJOR);

#include <linux/moduleparam.h>
/* allow_open_on_secondary */
MODULE_PARM_DESC(allow_oos, "DONT USE!");
/* thanks to these macros, if compiled into the kernel (not-module),
 * this becomes the boot parameter drbd.minor_count */
module_param(minor_count, uint, 0444);
module_param(disable_sendpage, bool, 0644);
module_param(allow_oos, bool, 0);
<<<<<<< HEAD
module_param(cn_idx, uint, 0444);
=======
>>>>>>> refs/remotes/origin/master
module_param(proc_details, int, 0644);

#ifdef CONFIG_DRBD_FAULT_INJECTION
int enable_faults;
int fault_rate;
static int fault_count;
int fault_devs;
/* bitmap of enabled faults */
module_param(enable_faults, int, 0664);
/* fault rate % value - applies to all enabled faults */
module_param(fault_rate, int, 0664);
/* count of faults inserted */
module_param(fault_count, int, 0664);
/* bitmap of devices to insert faults on */
module_param(fault_devs, int, 0644);
#endif

/* module parameter, defined */
unsigned int minor_count = DRBD_MINOR_COUNT_DEF;
<<<<<<< HEAD
<<<<<<< HEAD
int disable_sendpage;
int allow_oos;
=======
bool disable_sendpage;
bool allow_oos;
>>>>>>> refs/remotes/origin/cm-10.0
unsigned int cn_idx = CN_IDX_DRBD;
=======
bool disable_sendpage;
bool allow_oos;
>>>>>>> refs/remotes/origin/master
int proc_details;       /* Detail level in proc drbd*/

/* Module parameter for setting the user mode helper program
 * to run. Default is /sbin/drbdadm */
char usermode_helper[80] = "/sbin/drbdadm";

module_param_string(usermode_helper, usermode_helper, sizeof(usermode_helper), 0644);

/* in 2.6.x, our device mapping and config info contains our virtual gendisks
 * as member "struct gendisk *vdisk;"
 */
<<<<<<< HEAD
struct drbd_conf **minor_table;

struct kmem_cache *drbd_request_cache;
struct kmem_cache *drbd_ee_cache;	/* epoch entries */
=======
struct idr minors;
struct list_head drbd_tconns;  /* list of struct drbd_tconn */

struct kmem_cache *drbd_request_cache;
struct kmem_cache *drbd_ee_cache;	/* peer requests */
>>>>>>> refs/remotes/origin/master
struct kmem_cache *drbd_bm_ext_cache;	/* bitmap extents */
struct kmem_cache *drbd_al_ext_cache;	/* activity log extents */
mempool_t *drbd_request_mempool;
mempool_t *drbd_ee_mempool;
<<<<<<< HEAD
=======
mempool_t *drbd_md_io_page_pool;
struct bio_set *drbd_md_io_bio_set;
>>>>>>> refs/remotes/origin/master

/* I do not use a standard mempool, because:
   1) I want to hand out the pre-allocated objects first.
   2) I want to be able to interrupt sleeping allocation with a signal.
   Note: This is a single linked list, the next pointer is the private
	 member of struct page.
 */
struct page *drbd_pp_pool;
spinlock_t   drbd_pp_lock;
int          drbd_pp_vacant;
wait_queue_head_t drbd_pp_wait;

DEFINE_RATELIMIT_STATE(drbd_ratelimit_state, 5 * HZ, 5);

static const struct block_device_operations drbd_ops = {
	.owner =   THIS_MODULE,
	.open =    drbd_open,
	.release = drbd_release,
};

<<<<<<< HEAD
#define ARRY_SIZE(A) (sizeof(A)/sizeof(A[0]))
=======
struct bio *bio_alloc_drbd(gfp_t gfp_mask)
{
	struct bio *bio;

	if (!drbd_md_io_bio_set)
		return bio_alloc(gfp_mask, 1);

	bio = bio_alloc_bioset(gfp_mask, 1, drbd_md_io_bio_set);
	if (!bio)
		return NULL;
	return bio;
}
>>>>>>> refs/remotes/origin/master

#ifdef __CHECKER__
/* When checking with sparse, and this is an inline function, sparse will
   give tons of false positives. When this is a real functions sparse works.
 */
int _get_ldev_if_state(struct drbd_conf *mdev, enum drbd_disk_state mins)
{
	int io_allowed;

	atomic_inc(&mdev->local_cnt);
	io_allowed = (mdev->state.disk >= mins);
	if (!io_allowed) {
		if (atomic_dec_and_test(&mdev->local_cnt))
			wake_up(&mdev->misc_wait);
	}
	return io_allowed;
}

#endif

/**
<<<<<<< HEAD
 * DOC: The transfer log
 *
 * The transfer log is a single linked list of &struct drbd_tl_epoch objects.
 * mdev->newest_tle points to the head, mdev->oldest_tle points to the tail
 * of the list. There is always at least one &struct drbd_tl_epoch object.
 *
 * Each &struct drbd_tl_epoch has a circular double linked list of requests
 * attached.
 */
static int tl_init(struct drbd_conf *mdev)
{
	struct drbd_tl_epoch *b;

	/* during device minor initialization, we may well use GFP_KERNEL */
	b = kmalloc(sizeof(struct drbd_tl_epoch), GFP_KERNEL);
	if (!b)
		return 0;
	INIT_LIST_HEAD(&b->requests);
	INIT_LIST_HEAD(&b->w.list);
	b->next = NULL;
	b->br_number = 4711;
	b->n_writes = 0;
	b->w.cb = NULL; /* if this is != NULL, we need to dec_ap_pending in tl_clear */

	mdev->oldest_tle = b;
	mdev->newest_tle = b;
	INIT_LIST_HEAD(&mdev->out_of_sequence_requests);

	mdev->tl_hash = NULL;
	mdev->tl_hash_s = 0;

	return 1;
}

static void tl_cleanup(struct drbd_conf *mdev)
{
	D_ASSERT(mdev->oldest_tle == mdev->newest_tle);
	D_ASSERT(list_empty(&mdev->out_of_sequence_requests));
	kfree(mdev->oldest_tle);
	mdev->oldest_tle = NULL;
	kfree(mdev->unused_spare_tle);
	mdev->unused_spare_tle = NULL;
	kfree(mdev->tl_hash);
	mdev->tl_hash = NULL;
	mdev->tl_hash_s = 0;
}

/**
 * _tl_add_barrier() - Adds a barrier to the transfer log
 * @mdev:	DRBD device.
 * @new:	Barrier to be added before the current head of the TL.
 *
 * The caller must hold the req_lock.
 */
void _tl_add_barrier(struct drbd_conf *mdev, struct drbd_tl_epoch *new)
{
	struct drbd_tl_epoch *newest_before;

	INIT_LIST_HEAD(&new->requests);
	INIT_LIST_HEAD(&new->w.list);
	new->w.cb = NULL; /* if this is != NULL, we need to dec_ap_pending in tl_clear */
	new->next = NULL;
	new->n_writes = 0;

	newest_before = mdev->newest_tle;
	/* never send a barrier number == 0, because that is special-cased
	 * when using TCQ for our write ordering code */
	new->br_number = (newest_before->br_number+1) ?: 1;
	if (mdev->newest_tle != new) {
		mdev->newest_tle->next = new;
		mdev->newest_tle = new;
	}
}

/**
 * tl_release() - Free or recycle the oldest &struct drbd_tl_epoch object of the TL
 * @mdev:	DRBD device.
=======
 * tl_release() - mark as BARRIER_ACKED all requests in the corresponding transfer log epoch
 * @tconn:	DRBD connection.
>>>>>>> refs/remotes/origin/master
 * @barrier_nr:	Expected identifier of the DRBD write barrier packet.
 * @set_size:	Expected number of requests before that barrier.
 *
 * In case the passed barrier_nr or set_size does not match the oldest
<<<<<<< HEAD
 * &struct drbd_tl_epoch objects this function will cause a termination
 * of the connection.
 */
void tl_release(struct drbd_conf *mdev, unsigned int barrier_nr,
		       unsigned int set_size)
{
	struct drbd_tl_epoch *b, *nob; /* next old barrier */
	struct list_head *le, *tle;
	struct drbd_request *r;

	spin_lock_irq(&mdev->req_lock);

	b = mdev->oldest_tle;

	/* first some paranoia code */
	if (b == NULL) {
		dev_err(DEV, "BAD! BarrierAck #%u received, but no epoch in tl!?\n",
			barrier_nr);
		goto bail;
	}
	if (b->br_number != barrier_nr) {
		dev_err(DEV, "BAD! BarrierAck #%u received, expected #%u!\n",
			barrier_nr, b->br_number);
		goto bail;
	}
	if (b->n_writes != set_size) {
		dev_err(DEV, "BAD! BarrierAck #%u received with n_writes=%u, expected n_writes=%u!\n",
			barrier_nr, set_size, b->n_writes);
		goto bail;
	}

	/* Clean up list of requests processed during current epoch */
	list_for_each_safe(le, tle, &b->requests) {
		r = list_entry(le, struct drbd_request, tl_requests);
		_req_mod(r, barrier_acked);
	}
	/* There could be requests on the list waiting for completion
	   of the write to the local disk. To avoid corruptions of
	   slab's data structures we have to remove the lists head.

	   Also there could have been a barrier ack out of sequence, overtaking
	   the write acks - which would be a bug and violating write ordering.
	   To not deadlock in case we lose connection while such requests are
	   still pending, we need some way to find them for the
	   _req_mode(connection_lost_while_pending).

	   These have been list_move'd to the out_of_sequence_requests list in
	   _req_mod(, barrier_acked) above.
	   */
	list_del_init(&b->requests);

	nob = b->next;
	if (test_and_clear_bit(CREATE_BARRIER, &mdev->flags)) {
		_tl_add_barrier(mdev, b);
		if (nob)
			mdev->oldest_tle = nob;
		/* if nob == NULL b was the only barrier, and becomes the new
		   barrier. Therefore mdev->oldest_tle points already to b */
	} else {
		D_ASSERT(nob != NULL);
		mdev->oldest_tle = nob;
		kfree(b);
	}

	spin_unlock_irq(&mdev->req_lock);
	dec_ap_pending(mdev);
=======
 * epoch of not yet barrier-acked requests, this function will cause a
 * termination of the connection.
 */
void tl_release(struct drbd_tconn *tconn, unsigned int barrier_nr,
		unsigned int set_size)
{
	struct drbd_request *r;
	struct drbd_request *req = NULL;
	int expect_epoch = 0;
	int expect_size = 0;

	spin_lock_irq(&tconn->req_lock);

	/* find oldest not yet barrier-acked write request,
	 * count writes in its epoch. */
	list_for_each_entry(r, &tconn->transfer_log, tl_requests) {
		const unsigned s = r->rq_state;
		if (!req) {
			if (!(s & RQ_WRITE))
				continue;
			if (!(s & RQ_NET_MASK))
				continue;
			if (s & RQ_NET_DONE)
				continue;
			req = r;
			expect_epoch = req->epoch;
			expect_size ++;
		} else {
			if (r->epoch != expect_epoch)
				break;
			if (!(s & RQ_WRITE))
				continue;
			/* if (s & RQ_DONE): not expected */
			/* if (!(s & RQ_NET_MASK)): not expected */
			expect_size++;
		}
	}

	/* first some paranoia code */
	if (req == NULL) {
		conn_err(tconn, "BAD! BarrierAck #%u received, but no epoch in tl!?\n",
			 barrier_nr);
		goto bail;
	}
	if (expect_epoch != barrier_nr) {
		conn_err(tconn, "BAD! BarrierAck #%u received, expected #%u!\n",
			 barrier_nr, expect_epoch);
		goto bail;
	}

	if (expect_size != set_size) {
		conn_err(tconn, "BAD! BarrierAck #%u received with n_writes=%u, expected n_writes=%u!\n",
			 barrier_nr, set_size, expect_size);
		goto bail;
	}

	/* Clean up list of requests processed during current epoch. */
	/* this extra list walk restart is paranoia,
	 * to catch requests being barrier-acked "unexpectedly".
	 * It usually should find the same req again, or some READ preceding it. */
	list_for_each_entry(req, &tconn->transfer_log, tl_requests)
		if (req->epoch == expect_epoch)
			break;
	list_for_each_entry_safe_from(req, r, &tconn->transfer_log, tl_requests) {
		if (req->epoch != expect_epoch)
			break;
		_req_mod(req, BARRIER_ACKED);
	}
	spin_unlock_irq(&tconn->req_lock);
>>>>>>> refs/remotes/origin/master

	return;

bail:
<<<<<<< HEAD
	spin_unlock_irq(&mdev->req_lock);
	drbd_force_state(mdev, NS(conn, C_PROTOCOL_ERROR));
=======
	spin_unlock_irq(&tconn->req_lock);
	conn_request_state(tconn, NS(conn, C_PROTOCOL_ERROR), CS_HARD);
>>>>>>> refs/remotes/origin/master
}


/**
 * _tl_restart() - Walks the transfer log, and applies an action to all requests
 * @mdev:	DRBD device.
 * @what:       The action/event to perform with all request objects
 *
<<<<<<< HEAD
 * @what might be one of connection_lost_while_pending, resend, fail_frozen_disk_io,
 * restart_frozen_disk_io.
 */
static void _tl_restart(struct drbd_conf *mdev, enum drbd_req_event what)
{
	struct drbd_tl_epoch *b, *tmp, **pn;
	struct list_head *le, *tle, carry_reads;
	struct drbd_request *req;
	int rv, n_writes, n_reads;

	b = mdev->oldest_tle;
	pn = &mdev->oldest_tle;
	while (b) {
		n_writes = 0;
		n_reads = 0;
		INIT_LIST_HEAD(&carry_reads);
		list_for_each_safe(le, tle, &b->requests) {
			req = list_entry(le, struct drbd_request, tl_requests);
			rv = _req_mod(req, what);

			n_writes += (rv & MR_WRITE) >> MR_WRITE_SHIFT;
			n_reads  += (rv & MR_READ) >> MR_READ_SHIFT;
		}
		tmp = b->next;

		if (n_writes) {
			if (what == resend) {
				b->n_writes = n_writes;
				if (b->w.cb == NULL) {
					b->w.cb = w_send_barrier;
					inc_ap_pending(mdev);
					set_bit(CREATE_BARRIER, &mdev->flags);
				}

				drbd_queue_work(&mdev->data.work, &b->w);
			}
			pn = &b->next;
		} else {
			if (n_reads)
				list_add(&carry_reads, &b->requests);
			/* there could still be requests on that ring list,
			 * in case local io is still pending */
			list_del(&b->requests);

			/* dec_ap_pending corresponding to queue_barrier.
			 * the newest barrier may not have been queued yet,
			 * in which case w.cb is still NULL. */
			if (b->w.cb != NULL)
				dec_ap_pending(mdev);

			if (b == mdev->newest_tle) {
				/* recycle, but reinit! */
				D_ASSERT(tmp == NULL);
				INIT_LIST_HEAD(&b->requests);
				list_splice(&carry_reads, &b->requests);
				INIT_LIST_HEAD(&b->w.list);
				b->w.cb = NULL;
				b->br_number = net_random();
				b->n_writes = 0;

				*pn = b;
				break;
			}
			*pn = tmp;
			kfree(b);
		}
		b = tmp;
		list_splice(&carry_reads, &b->requests);
	}
}


/**
 * tl_clear() - Clears all requests and &struct drbd_tl_epoch objects out of the TL
 * @mdev:	DRBD device.
 *
 * This is called after the connection to the peer was lost. The storage covered
 * by the requests on the transfer gets marked as our of sync. Called from the
 * receiver thread and the worker thread.
 */
void tl_clear(struct drbd_conf *mdev)
{
	struct list_head *le, *tle;
	struct drbd_request *r;

	spin_lock_irq(&mdev->req_lock);

	_tl_restart(mdev, connection_lost_while_pending);

	/* we expect this list to be empty. */
	D_ASSERT(list_empty(&mdev->out_of_sequence_requests));

	/* but just in case, clean it up anyways! */
	list_for_each_safe(le, tle, &mdev->out_of_sequence_requests) {
		r = list_entry(le, struct drbd_request, tl_requests);
		/* It would be nice to complete outside of spinlock.
		 * But this is easier for now. */
		_req_mod(r, connection_lost_while_pending);
	}

	/* ensure bit indicating barrier is required is clear */
	clear_bit(CREATE_BARRIER, &mdev->flags);

	memset(mdev->app_reads_hash, 0, APP_R_HSIZE*sizeof(void *));

	spin_unlock_irq(&mdev->req_lock);
}

void tl_restart(struct drbd_conf *mdev, enum drbd_req_event what)
{
	spin_lock_irq(&mdev->req_lock);
	_tl_restart(mdev, what);
	spin_unlock_irq(&mdev->req_lock);
}

/**
 * cl_wide_st_chg() - true if the state change is a cluster wide one
 * @mdev:	DRBD device.
 * @os:		old (current) state.
 * @ns:		new (wanted) state.
 */
static int cl_wide_st_chg(struct drbd_conf *mdev,
			  union drbd_state os, union drbd_state ns)
{
	return (os.conn >= C_CONNECTED && ns.conn >= C_CONNECTED &&
		 ((os.role != R_PRIMARY && ns.role == R_PRIMARY) ||
		  (os.conn != C_STARTING_SYNC_T && ns.conn == C_STARTING_SYNC_T) ||
		  (os.conn != C_STARTING_SYNC_S && ns.conn == C_STARTING_SYNC_S) ||
		  (os.disk != D_DISKLESS && ns.disk == D_DISKLESS))) ||
		(os.conn >= C_CONNECTED && ns.conn == C_DISCONNECTING) ||
		(os.conn == C_CONNECTED && ns.conn == C_VERIFY_S);
}

enum drbd_state_rv
drbd_change_state(struct drbd_conf *mdev, enum chg_state_flags f,
		  union drbd_state mask, union drbd_state val)
{
	unsigned long flags;
	union drbd_state os, ns;
	enum drbd_state_rv rv;

	spin_lock_irqsave(&mdev->req_lock, flags);
	os = mdev->state;
	ns.i = (os.i & ~mask.i) | val.i;
	rv = _drbd_set_state(mdev, ns, f, NULL);
	ns = mdev->state;
	spin_unlock_irqrestore(&mdev->req_lock, flags);

	return rv;
}

/**
 * drbd_force_state() - Impose a change which happens outside our control on our state
 * @mdev:	DRBD device.
 * @mask:	mask of state bits to change.
 * @val:	value of new state bits.
 */
void drbd_force_state(struct drbd_conf *mdev,
	union drbd_state mask, union drbd_state val)
{
	drbd_change_state(mdev, CS_HARD, mask, val);
}

static enum drbd_state_rv is_valid_state(struct drbd_conf *, union drbd_state);
static enum drbd_state_rv is_valid_state_transition(struct drbd_conf *,
						    union drbd_state,
						    union drbd_state);
static union drbd_state sanitize_state(struct drbd_conf *mdev, union drbd_state os,
				       union drbd_state ns, const char **warn_sync_abort);
int drbd_send_state_req(struct drbd_conf *,
			union drbd_state, union drbd_state);

static enum drbd_state_rv
_req_st_cond(struct drbd_conf *mdev, union drbd_state mask,
	     union drbd_state val)
{
	union drbd_state os, ns;
	unsigned long flags;
	enum drbd_state_rv rv;

	if (test_and_clear_bit(CL_ST_CHG_SUCCESS, &mdev->flags))
		return SS_CW_SUCCESS;

	if (test_and_clear_bit(CL_ST_CHG_FAIL, &mdev->flags))
		return SS_CW_FAILED_BY_PEER;

	rv = 0;
	spin_lock_irqsave(&mdev->req_lock, flags);
	os = mdev->state;
	ns.i = (os.i & ~mask.i) | val.i;
	ns = sanitize_state(mdev, os, ns, NULL);

	if (!cl_wide_st_chg(mdev, os, ns))
		rv = SS_CW_NO_NEED;
	if (!rv) {
		rv = is_valid_state(mdev, ns);
		if (rv == SS_SUCCESS) {
			rv = is_valid_state_transition(mdev, ns, os);
			if (rv == SS_SUCCESS)
				rv = SS_UNKNOWN_ERROR; /* cont waiting, otherwise fail. */
		}
	}
	spin_unlock_irqrestore(&mdev->req_lock, flags);

	return rv;
}

/**
 * drbd_req_state() - Perform an eventually cluster wide state change
 * @mdev:	DRBD device.
 * @mask:	mask of state bits to change.
 * @val:	value of new state bits.
 * @f:		flags
 *
 * Should not be called directly, use drbd_request_state() or
 * _drbd_request_state().
 */
static enum drbd_state_rv
drbd_req_state(struct drbd_conf *mdev, union drbd_state mask,
	       union drbd_state val, enum chg_state_flags f)
{
	struct completion done;
	unsigned long flags;
	union drbd_state os, ns;
	enum drbd_state_rv rv;

	init_completion(&done);

	if (f & CS_SERIALIZE)
		mutex_lock(&mdev->state_mutex);

	spin_lock_irqsave(&mdev->req_lock, flags);
	os = mdev->state;
	ns.i = (os.i & ~mask.i) | val.i;
	ns = sanitize_state(mdev, os, ns, NULL);

	if (cl_wide_st_chg(mdev, os, ns)) {
		rv = is_valid_state(mdev, ns);
		if (rv == SS_SUCCESS)
			rv = is_valid_state_transition(mdev, ns, os);
		spin_unlock_irqrestore(&mdev->req_lock, flags);

		if (rv < SS_SUCCESS) {
			if (f & CS_VERBOSE)
				print_st_err(mdev, os, ns, rv);
			goto abort;
		}

		drbd_state_lock(mdev);
		if (!drbd_send_state_req(mdev, mask, val)) {
			drbd_state_unlock(mdev);
			rv = SS_CW_FAILED_BY_PEER;
			if (f & CS_VERBOSE)
				print_st_err(mdev, os, ns, rv);
			goto abort;
		}

		wait_event(mdev->state_wait,
			(rv = _req_st_cond(mdev, mask, val)));

		if (rv < SS_SUCCESS) {
			drbd_state_unlock(mdev);
			if (f & CS_VERBOSE)
				print_st_err(mdev, os, ns, rv);
			goto abort;
		}
		spin_lock_irqsave(&mdev->req_lock, flags);
		os = mdev->state;
		ns.i = (os.i & ~mask.i) | val.i;
		rv = _drbd_set_state(mdev, ns, f, &done);
		drbd_state_unlock(mdev);
	} else {
		rv = _drbd_set_state(mdev, ns, f, &done);
	}

	spin_unlock_irqrestore(&mdev->req_lock, flags);

	if (f & CS_WAIT_COMPLETE && rv == SS_SUCCESS) {
		D_ASSERT(current != mdev->worker.task);
		wait_for_completion(&done);
	}

abort:
	if (f & CS_SERIALIZE)
		mutex_unlock(&mdev->state_mutex);

	return rv;
}

/**
 * _drbd_request_state() - Request a state change (with flags)
 * @mdev:	DRBD device.
 * @mask:	mask of state bits to change.
 * @val:	value of new state bits.
 * @f:		flags
 *
 * Cousin of drbd_request_state(), useful with the CS_WAIT_COMPLETE
 * flag, or when logging of failed state change requests is not desired.
 */
enum drbd_state_rv
_drbd_request_state(struct drbd_conf *mdev, union drbd_state mask,
		    union drbd_state val, enum chg_state_flags f)
{
	enum drbd_state_rv rv;

	wait_event(mdev->state_wait,
		   (rv = drbd_req_state(mdev, mask, val, f)) != SS_IN_TRANSIENT_STATE);

	return rv;
}

static void print_st(struct drbd_conf *mdev, char *name, union drbd_state ns)
{
	dev_err(DEV, " %s = { cs:%s ro:%s/%s ds:%s/%s %c%c%c%c }\n",
	    name,
	    drbd_conn_str(ns.conn),
	    drbd_role_str(ns.role),
	    drbd_role_str(ns.peer),
	    drbd_disk_str(ns.disk),
	    drbd_disk_str(ns.pdsk),
	    is_susp(ns) ? 's' : 'r',
	    ns.aftr_isp ? 'a' : '-',
	    ns.peer_isp ? 'p' : '-',
	    ns.user_isp ? 'u' : '-'
	    );
}

void print_st_err(struct drbd_conf *mdev, union drbd_state os,
	          union drbd_state ns, enum drbd_state_rv err)
{
	if (err == SS_IN_TRANSIENT_STATE)
		return;
	dev_err(DEV, "State change failed: %s\n", drbd_set_st_err_str(err));
	print_st(mdev, " state", os);
	print_st(mdev, "wanted", ns);
}


/**
 * is_valid_state() - Returns an SS_ error code if ns is not valid
 * @mdev:	DRBD device.
 * @ns:		State to consider.
 */
static enum drbd_state_rv
is_valid_state(struct drbd_conf *mdev, union drbd_state ns)
{
	/* See drbd_state_sw_errors in drbd_strings.c */

	enum drbd_fencing_p fp;
	enum drbd_state_rv rv = SS_SUCCESS;

	fp = FP_DONT_CARE;
	if (get_ldev(mdev)) {
		fp = mdev->ldev->dc.fencing;
		put_ldev(mdev);
	}

	if (get_net_conf(mdev)) {
		if (!mdev->net_conf->two_primaries &&
		    ns.role == R_PRIMARY && ns.peer == R_PRIMARY)
			rv = SS_TWO_PRIMARIES;
		put_net_conf(mdev);
	}

	if (rv <= 0)
		/* already found a reason to abort */;
	else if (ns.role == R_SECONDARY && mdev->open_cnt)
		rv = SS_DEVICE_IN_USE;

	else if (ns.role == R_PRIMARY && ns.conn < C_CONNECTED && ns.disk < D_UP_TO_DATE)
		rv = SS_NO_UP_TO_DATE_DISK;

	else if (fp >= FP_RESOURCE &&
		 ns.role == R_PRIMARY && ns.conn < C_CONNECTED && ns.pdsk >= D_UNKNOWN)
		rv = SS_PRIMARY_NOP;

	else if (ns.role == R_PRIMARY && ns.disk <= D_INCONSISTENT && ns.pdsk <= D_INCONSISTENT)
		rv = SS_NO_UP_TO_DATE_DISK;

	else if (ns.conn > C_CONNECTED && ns.disk < D_INCONSISTENT)
		rv = SS_NO_LOCAL_DISK;

	else if (ns.conn > C_CONNECTED && ns.pdsk < D_INCONSISTENT)
		rv = SS_NO_REMOTE_DISK;

	else if (ns.conn > C_CONNECTED && ns.disk < D_UP_TO_DATE && ns.pdsk < D_UP_TO_DATE)
		rv = SS_NO_UP_TO_DATE_DISK;

	else if ((ns.conn == C_CONNECTED ||
		  ns.conn == C_WF_BITMAP_S ||
		  ns.conn == C_SYNC_SOURCE ||
		  ns.conn == C_PAUSED_SYNC_S) &&
		  ns.disk == D_OUTDATED)
		rv = SS_CONNECTED_OUTDATES;

	else if ((ns.conn == C_VERIFY_S || ns.conn == C_VERIFY_T) &&
		 (mdev->sync_conf.verify_alg[0] == 0))
		rv = SS_NO_VERIFY_ALG;

	else if ((ns.conn == C_VERIFY_S || ns.conn == C_VERIFY_T) &&
		  mdev->agreed_pro_version < 88)
		rv = SS_NOT_SUPPORTED;

	else if (ns.conn >= C_CONNECTED && ns.pdsk == D_UNKNOWN)
		rv = SS_CONNECTED_OUTDATES;

	return rv;
}

/**
 * is_valid_state_transition() - Returns an SS_ error code if the state transition is not possible
 * @mdev:	DRBD device.
 * @ns:		new state.
 * @os:		old state.
 */
static enum drbd_state_rv
is_valid_state_transition(struct drbd_conf *mdev, union drbd_state ns,
			  union drbd_state os)
{
	enum drbd_state_rv rv = SS_SUCCESS;

	if ((ns.conn == C_STARTING_SYNC_T || ns.conn == C_STARTING_SYNC_S) &&
	    os.conn > C_CONNECTED)
		rv = SS_RESYNC_RUNNING;

	if (ns.conn == C_DISCONNECTING && os.conn == C_STANDALONE)
		rv = SS_ALREADY_STANDALONE;

	if (ns.disk > D_ATTACHING && os.disk == D_DISKLESS)
		rv = SS_IS_DISKLESS;

	if (ns.conn == C_WF_CONNECTION && os.conn < C_UNCONNECTED)
		rv = SS_NO_NET_CONFIG;

	if (ns.disk == D_OUTDATED && os.disk < D_OUTDATED && os.disk != D_ATTACHING)
		rv = SS_LOWER_THAN_OUTDATED;

	if (ns.conn == C_DISCONNECTING && os.conn == C_UNCONNECTED)
		rv = SS_IN_TRANSIENT_STATE;

	if (ns.conn == os.conn && ns.conn == C_WF_REPORT_PARAMS)
		rv = SS_IN_TRANSIENT_STATE;

	if ((ns.conn == C_VERIFY_S || ns.conn == C_VERIFY_T) && os.conn < C_CONNECTED)
		rv = SS_NEED_CONNECTION;

	if ((ns.conn == C_VERIFY_S || ns.conn == C_VERIFY_T) &&
	    ns.conn != os.conn && os.conn > C_CONNECTED)
		rv = SS_RESYNC_RUNNING;

	if ((ns.conn == C_STARTING_SYNC_S || ns.conn == C_STARTING_SYNC_T) &&
	    os.conn < C_CONNECTED)
		rv = SS_NEED_CONNECTION;

	if ((ns.conn == C_SYNC_TARGET || ns.conn == C_SYNC_SOURCE)
	    && os.conn < C_WF_REPORT_PARAMS)
		rv = SS_NEED_CONNECTION; /* No NetworkFailure -> SyncTarget etc... */

	return rv;
}

/**
 * sanitize_state() - Resolves implicitly necessary additional changes to a state transition
 * @mdev:	DRBD device.
 * @os:		old state.
 * @ns:		new state.
 * @warn_sync_abort:
 *
 * When we loose connection, we have to set the state of the peers disk (pdsk)
 * to D_UNKNOWN. This rule and many more along those lines are in this function.
 */
static union drbd_state sanitize_state(struct drbd_conf *mdev, union drbd_state os,
				       union drbd_state ns, const char **warn_sync_abort)
{
	enum drbd_fencing_p fp;
	enum drbd_disk_state disk_min, disk_max, pdsk_min, pdsk_max;

	fp = FP_DONT_CARE;
	if (get_ldev(mdev)) {
		fp = mdev->ldev->dc.fencing;
		put_ldev(mdev);
	}

	/* Disallow Network errors to configure a device's network part */
	if ((ns.conn >= C_TIMEOUT && ns.conn <= C_TEAR_DOWN) &&
	    os.conn <= C_DISCONNECTING)
		ns.conn = os.conn;

	/* After a network error (+C_TEAR_DOWN) only C_UNCONNECTED or C_DISCONNECTING can follow.
	 * If you try to go into some Sync* state, that shall fail (elsewhere). */
	if (os.conn >= C_TIMEOUT && os.conn <= C_TEAR_DOWN &&
	    ns.conn != C_UNCONNECTED && ns.conn != C_DISCONNECTING && ns.conn <= C_TEAR_DOWN)
		ns.conn = os.conn;

	/* we cannot fail (again) if we already detached */
	if (ns.disk == D_FAILED && os.disk == D_DISKLESS)
		ns.disk = D_DISKLESS;

	/* if we are only D_ATTACHING yet,
	 * we can (and should) go directly to D_DISKLESS. */
	if (ns.disk == D_FAILED && os.disk == D_ATTACHING)
		ns.disk = D_DISKLESS;

	/* After C_DISCONNECTING only C_STANDALONE may follow */
	if (os.conn == C_DISCONNECTING && ns.conn != C_STANDALONE)
		ns.conn = os.conn;

	if (ns.conn < C_CONNECTED) {
		ns.peer_isp = 0;
		ns.peer = R_UNKNOWN;
		if (ns.pdsk > D_UNKNOWN || ns.pdsk < D_INCONSISTENT)
			ns.pdsk = D_UNKNOWN;
	}

	/* Clear the aftr_isp when becoming unconfigured */
	if (ns.conn == C_STANDALONE && ns.disk == D_DISKLESS && ns.role == R_SECONDARY)
		ns.aftr_isp = 0;

	/* Abort resync if a disk fails/detaches */
	if (os.conn > C_CONNECTED && ns.conn > C_CONNECTED &&
	    (ns.disk <= D_FAILED || ns.pdsk <= D_FAILED)) {
		if (warn_sync_abort)
			*warn_sync_abort =
				os.conn == C_VERIFY_S || os.conn == C_VERIFY_T ?
				"Online-verify" : "Resync";
		ns.conn = C_CONNECTED;
	}

	/* Connection breaks down before we finished "Negotiating" */
	if (ns.conn < C_CONNECTED && ns.disk == D_NEGOTIATING &&
	    get_ldev_if_state(mdev, D_NEGOTIATING)) {
		if (mdev->ed_uuid == mdev->ldev->md.uuid[UI_CURRENT]) {
			ns.disk = mdev->new_state_tmp.disk;
			ns.pdsk = mdev->new_state_tmp.pdsk;
		} else {
			dev_alert(DEV, "Connection lost while negotiating, no data!\n");
			ns.disk = D_DISKLESS;
			ns.pdsk = D_UNKNOWN;
		}
		put_ldev(mdev);
	}

	/* D_CONSISTENT and D_OUTDATED vanish when we get connected */
	if (ns.conn >= C_CONNECTED && ns.conn < C_AHEAD) {
		if (ns.disk == D_CONSISTENT || ns.disk == D_OUTDATED)
			ns.disk = D_UP_TO_DATE;
		if (ns.pdsk == D_CONSISTENT || ns.pdsk == D_OUTDATED)
			ns.pdsk = D_UP_TO_DATE;
	}

	/* Implications of the connection stat on the disk states */
	disk_min = D_DISKLESS;
	disk_max = D_UP_TO_DATE;
	pdsk_min = D_INCONSISTENT;
	pdsk_max = D_UNKNOWN;
	switch ((enum drbd_conns)ns.conn) {
	case C_WF_BITMAP_T:
	case C_PAUSED_SYNC_T:
	case C_STARTING_SYNC_T:
	case C_WF_SYNC_UUID:
	case C_BEHIND:
		disk_min = D_INCONSISTENT;
		disk_max = D_OUTDATED;
		pdsk_min = D_UP_TO_DATE;
		pdsk_max = D_UP_TO_DATE;
		break;
	case C_VERIFY_S:
	case C_VERIFY_T:
		disk_min = D_UP_TO_DATE;
		disk_max = D_UP_TO_DATE;
		pdsk_min = D_UP_TO_DATE;
		pdsk_max = D_UP_TO_DATE;
		break;
	case C_CONNECTED:
		disk_min = D_DISKLESS;
		disk_max = D_UP_TO_DATE;
		pdsk_min = D_DISKLESS;
		pdsk_max = D_UP_TO_DATE;
		break;
	case C_WF_BITMAP_S:
	case C_PAUSED_SYNC_S:
	case C_STARTING_SYNC_S:
	case C_AHEAD:
		disk_min = D_UP_TO_DATE;
		disk_max = D_UP_TO_DATE;
		pdsk_min = D_INCONSISTENT;
		pdsk_max = D_CONSISTENT; /* D_OUTDATED would be nice. But explicit outdate necessary*/
		break;
	case C_SYNC_TARGET:
		disk_min = D_INCONSISTENT;
		disk_max = D_INCONSISTENT;
		pdsk_min = D_UP_TO_DATE;
		pdsk_max = D_UP_TO_DATE;
		break;
	case C_SYNC_SOURCE:
		disk_min = D_UP_TO_DATE;
		disk_max = D_UP_TO_DATE;
		pdsk_min = D_INCONSISTENT;
		pdsk_max = D_INCONSISTENT;
		break;
	case C_STANDALONE:
	case C_DISCONNECTING:
	case C_UNCONNECTED:
	case C_TIMEOUT:
	case C_BROKEN_PIPE:
	case C_NETWORK_FAILURE:
	case C_PROTOCOL_ERROR:
	case C_TEAR_DOWN:
	case C_WF_CONNECTION:
	case C_WF_REPORT_PARAMS:
	case C_MASK:
		break;
	}
	if (ns.disk > disk_max)
		ns.disk = disk_max;

	if (ns.disk < disk_min) {
		dev_warn(DEV, "Implicitly set disk from %s to %s\n",
			 drbd_disk_str(ns.disk), drbd_disk_str(disk_min));
		ns.disk = disk_min;
	}
	if (ns.pdsk > pdsk_max)
		ns.pdsk = pdsk_max;

	if (ns.pdsk < pdsk_min) {
		dev_warn(DEV, "Implicitly set pdsk from %s to %s\n",
			 drbd_disk_str(ns.pdsk), drbd_disk_str(pdsk_min));
		ns.pdsk = pdsk_min;
	}

	if (fp == FP_STONITH &&
	    (ns.role == R_PRIMARY && ns.conn < C_CONNECTED && ns.pdsk > D_OUTDATED) &&
	    !(os.role == R_PRIMARY && os.conn < C_CONNECTED && os.pdsk > D_OUTDATED))
		ns.susp_fen = 1; /* Suspend IO while fence-peer handler runs (peer lost) */

	if (mdev->sync_conf.on_no_data == OND_SUSPEND_IO &&
	    (ns.role == R_PRIMARY && ns.disk < D_UP_TO_DATE && ns.pdsk < D_UP_TO_DATE) &&
	    !(os.role == R_PRIMARY && os.disk < D_UP_TO_DATE && os.pdsk < D_UP_TO_DATE))
		ns.susp_nod = 1; /* Suspend IO while no data available (no accessible data available) */

	if (ns.aftr_isp || ns.peer_isp || ns.user_isp) {
		if (ns.conn == C_SYNC_SOURCE)
			ns.conn = C_PAUSED_SYNC_S;
		if (ns.conn == C_SYNC_TARGET)
			ns.conn = C_PAUSED_SYNC_T;
	} else {
		if (ns.conn == C_PAUSED_SYNC_S)
			ns.conn = C_SYNC_SOURCE;
		if (ns.conn == C_PAUSED_SYNC_T)
			ns.conn = C_SYNC_TARGET;
	}

	return ns;
}

/* helper for __drbd_set_state */
static void set_ov_position(struct drbd_conf *mdev, enum drbd_conns cs)
{
	if (mdev->agreed_pro_version < 90)
		mdev->ov_start_sector = 0;
	mdev->rs_total = drbd_bm_bits(mdev);
	mdev->ov_position = 0;
	if (cs == C_VERIFY_T) {
		/* starting online verify from an arbitrary position
		 * does not fit well into the existing protocol.
		 * on C_VERIFY_T, we initialize ov_left and friends
		 * implicitly in receive_DataRequest once the
		 * first P_OV_REQUEST is received */
		mdev->ov_start_sector = ~(sector_t)0;
	} else {
		unsigned long bit = BM_SECT_TO_BIT(mdev->ov_start_sector);
		if (bit >= mdev->rs_total) {
			mdev->ov_start_sector =
				BM_BIT_TO_SECT(mdev->rs_total - 1);
			mdev->rs_total = 1;
		} else
			mdev->rs_total -= bit;
		mdev->ov_position = mdev->ov_start_sector;
	}
	mdev->ov_left = mdev->rs_total;
}

static void drbd_resume_al(struct drbd_conf *mdev)
{
	if (test_and_clear_bit(AL_SUSPENDED, &mdev->flags))
		dev_info(DEV, "Resumed AL updates\n");
}

/**
 * __drbd_set_state() - Set a new DRBD state
 * @mdev:	DRBD device.
 * @ns:		new state.
 * @flags:	Flags
 * @done:	Optional completion, that will get completed after the after_state_ch() finished
 *
 * Caller needs to hold req_lock, and global_state_lock. Do not call directly.
 */
enum drbd_state_rv
__drbd_set_state(struct drbd_conf *mdev, union drbd_state ns,
	         enum chg_state_flags flags, struct completion *done)
{
	union drbd_state os;
	enum drbd_state_rv rv = SS_SUCCESS;
	const char *warn_sync_abort = NULL;
	struct after_state_chg_work *ascw;

	os = mdev->state;

	ns = sanitize_state(mdev, os, ns, &warn_sync_abort);

	if (ns.i == os.i)
		return SS_NOTHING_TO_DO;

	if (!(flags & CS_HARD)) {
		/*  pre-state-change checks ; only look at ns  */
		/* See drbd_state_sw_errors in drbd_strings.c */

		rv = is_valid_state(mdev, ns);
		if (rv < SS_SUCCESS) {
			/* If the old state was illegal as well, then let
			   this happen...*/

			if (is_valid_state(mdev, os) == rv)
				rv = is_valid_state_transition(mdev, ns, os);
		} else
			rv = is_valid_state_transition(mdev, ns, os);
	}

	if (rv < SS_SUCCESS) {
		if (flags & CS_VERBOSE)
			print_st_err(mdev, os, ns, rv);
		return rv;
	}

	if (warn_sync_abort)
		dev_warn(DEV, "%s aborted.\n", warn_sync_abort);

	{
	char *pbp, pb[300];
	pbp = pb;
	*pbp = 0;
	if (ns.role != os.role)
		pbp += sprintf(pbp, "role( %s -> %s ) ",
			       drbd_role_str(os.role),
			       drbd_role_str(ns.role));
	if (ns.peer != os.peer)
		pbp += sprintf(pbp, "peer( %s -> %s ) ",
			       drbd_role_str(os.peer),
			       drbd_role_str(ns.peer));
	if (ns.conn != os.conn)
		pbp += sprintf(pbp, "conn( %s -> %s ) ",
			       drbd_conn_str(os.conn),
			       drbd_conn_str(ns.conn));
	if (ns.disk != os.disk)
		pbp += sprintf(pbp, "disk( %s -> %s ) ",
			       drbd_disk_str(os.disk),
			       drbd_disk_str(ns.disk));
	if (ns.pdsk != os.pdsk)
		pbp += sprintf(pbp, "pdsk( %s -> %s ) ",
			       drbd_disk_str(os.pdsk),
			       drbd_disk_str(ns.pdsk));
	if (is_susp(ns) != is_susp(os))
		pbp += sprintf(pbp, "susp( %d -> %d ) ",
			       is_susp(os),
			       is_susp(ns));
	if (ns.aftr_isp != os.aftr_isp)
		pbp += sprintf(pbp, "aftr_isp( %d -> %d ) ",
			       os.aftr_isp,
			       ns.aftr_isp);
	if (ns.peer_isp != os.peer_isp)
		pbp += sprintf(pbp, "peer_isp( %d -> %d ) ",
			       os.peer_isp,
			       ns.peer_isp);
	if (ns.user_isp != os.user_isp)
		pbp += sprintf(pbp, "user_isp( %d -> %d ) ",
			       os.user_isp,
			       ns.user_isp);
	dev_info(DEV, "%s\n", pb);
	}

	/* solve the race between becoming unconfigured,
	 * worker doing the cleanup, and
	 * admin reconfiguring us:
	 * on (re)configure, first set CONFIG_PENDING,
	 * then wait for a potentially exiting worker,
	 * start the worker, and schedule one no_op.
	 * then proceed with configuration.
	 */
	if (ns.disk == D_DISKLESS &&
	    ns.conn == C_STANDALONE &&
	    ns.role == R_SECONDARY &&
	    !test_and_set_bit(CONFIG_PENDING, &mdev->flags))
		set_bit(DEVICE_DYING, &mdev->flags);

	/* if we are going -> D_FAILED or D_DISKLESS, grab one extra reference
	 * on the ldev here, to be sure the transition -> D_DISKLESS resp.
	 * drbd_ldev_destroy() won't happen before our corresponding
	 * after_state_ch works run, where we put_ldev again. */
	if ((os.disk != D_FAILED && ns.disk == D_FAILED) ||
	    (os.disk != D_DISKLESS && ns.disk == D_DISKLESS))
		atomic_inc(&mdev->local_cnt);

	mdev->state = ns;

	if (os.disk == D_ATTACHING && ns.disk >= D_NEGOTIATING)
		drbd_print_uuids(mdev, "attached to UUIDs");

	wake_up(&mdev->misc_wait);
	wake_up(&mdev->state_wait);

	/* aborted verify run. log the last position */
	if ((os.conn == C_VERIFY_S || os.conn == C_VERIFY_T) &&
	    ns.conn < C_CONNECTED) {
		mdev->ov_start_sector =
			BM_BIT_TO_SECT(drbd_bm_bits(mdev) - mdev->ov_left);
		dev_info(DEV, "Online Verify reached sector %llu\n",
			(unsigned long long)mdev->ov_start_sector);
	}

	if ((os.conn == C_PAUSED_SYNC_T || os.conn == C_PAUSED_SYNC_S) &&
	    (ns.conn == C_SYNC_TARGET  || ns.conn == C_SYNC_SOURCE)) {
		dev_info(DEV, "Syncer continues.\n");
		mdev->rs_paused += (long)jiffies
				  -(long)mdev->rs_mark_time[mdev->rs_last_mark];
		if (ns.conn == C_SYNC_TARGET)
			mod_timer(&mdev->resync_timer, jiffies);
	}

	if ((os.conn == C_SYNC_TARGET  || os.conn == C_SYNC_SOURCE) &&
	    (ns.conn == C_PAUSED_SYNC_T || ns.conn == C_PAUSED_SYNC_S)) {
		dev_info(DEV, "Resync suspended\n");
		mdev->rs_mark_time[mdev->rs_last_mark] = jiffies;
	}

	if (os.conn == C_CONNECTED &&
	    (ns.conn == C_VERIFY_S || ns.conn == C_VERIFY_T)) {
		unsigned long now = jiffies;
		int i;

		set_ov_position(mdev, ns.conn);
		mdev->rs_start = now;
		mdev->rs_last_events = 0;
		mdev->rs_last_sect_ev = 0;
		mdev->ov_last_oos_size = 0;
		mdev->ov_last_oos_start = 0;

		for (i = 0; i < DRBD_SYNC_MARKS; i++) {
			mdev->rs_mark_left[i] = mdev->ov_left;
			mdev->rs_mark_time[i] = now;
		}

		drbd_rs_controller_reset(mdev);

		if (ns.conn == C_VERIFY_S) {
			dev_info(DEV, "Starting Online Verify from sector %llu\n",
					(unsigned long long)mdev->ov_position);
			mod_timer(&mdev->resync_timer, jiffies);
		}
	}

	if (get_ldev(mdev)) {
		u32 mdf = mdev->ldev->md.flags & ~(MDF_CONSISTENT|MDF_PRIMARY_IND|
						 MDF_CONNECTED_IND|MDF_WAS_UP_TO_DATE|
						 MDF_PEER_OUT_DATED|MDF_CRASHED_PRIMARY);

		if (test_bit(CRASHED_PRIMARY, &mdev->flags))
			mdf |= MDF_CRASHED_PRIMARY;
		if (mdev->state.role == R_PRIMARY ||
		    (mdev->state.pdsk < D_INCONSISTENT && mdev->state.peer == R_PRIMARY))
			mdf |= MDF_PRIMARY_IND;
		if (mdev->state.conn > C_WF_REPORT_PARAMS)
			mdf |= MDF_CONNECTED_IND;
		if (mdev->state.disk > D_INCONSISTENT)
			mdf |= MDF_CONSISTENT;
		if (mdev->state.disk > D_OUTDATED)
			mdf |= MDF_WAS_UP_TO_DATE;
		if (mdev->state.pdsk <= D_OUTDATED && mdev->state.pdsk >= D_INCONSISTENT)
			mdf |= MDF_PEER_OUT_DATED;
		if (mdf != mdev->ldev->md.flags) {
			mdev->ldev->md.flags = mdf;
			drbd_md_mark_dirty(mdev);
		}
		if (os.disk < D_CONSISTENT && ns.disk >= D_CONSISTENT)
			drbd_set_ed_uuid(mdev, mdev->ldev->md.uuid[UI_CURRENT]);
		put_ldev(mdev);
	}

	/* Peer was forced D_UP_TO_DATE & R_PRIMARY, consider to resync */
	if (os.disk == D_INCONSISTENT && os.pdsk == D_INCONSISTENT &&
	    os.peer == R_SECONDARY && ns.peer == R_PRIMARY)
		set_bit(CONSIDER_RESYNC, &mdev->flags);

	/* Receiver should clean up itself */
	if (os.conn != C_DISCONNECTING && ns.conn == C_DISCONNECTING)
		drbd_thread_stop_nowait(&mdev->receiver);

	/* Now the receiver finished cleaning up itself, it should die */
	if (os.conn != C_STANDALONE && ns.conn == C_STANDALONE)
		drbd_thread_stop_nowait(&mdev->receiver);

	/* Upon network failure, we need to restart the receiver. */
	if (os.conn > C_TEAR_DOWN &&
	    ns.conn <= C_TEAR_DOWN && ns.conn >= C_TIMEOUT)
		drbd_thread_restart_nowait(&mdev->receiver);

	/* Resume AL writing if we get a connection */
	if (os.conn < C_CONNECTED && ns.conn >= C_CONNECTED)
		drbd_resume_al(mdev);

	ascw = kmalloc(sizeof(*ascw), GFP_ATOMIC);
	if (ascw) {
		ascw->os = os;
		ascw->ns = ns;
		ascw->flags = flags;
		ascw->w.cb = w_after_state_ch;
		ascw->done = done;
		drbd_queue_work(&mdev->data.work, &ascw->w);
	} else {
		dev_warn(DEV, "Could not kmalloc an ascw\n");
	}

	return rv;
}

static int w_after_state_ch(struct drbd_conf *mdev, struct drbd_work *w, int unused)
{
	struct after_state_chg_work *ascw =
		container_of(w, struct after_state_chg_work, w);
	after_state_ch(mdev, ascw->os, ascw->ns, ascw->flags);
	if (ascw->flags & CS_WAIT_COMPLETE) {
		D_ASSERT(ascw->done != NULL);
		complete(ascw->done);
	}
	kfree(ascw);

	return 1;
}

static void abw_start_sync(struct drbd_conf *mdev, int rv)
{
	if (rv) {
		dev_err(DEV, "Writing the bitmap failed not starting resync.\n");
		_drbd_request_state(mdev, NS(conn, C_CONNECTED), CS_VERBOSE);
		return;
	}

	switch (mdev->state.conn) {
	case C_STARTING_SYNC_T:
		_drbd_request_state(mdev, NS(conn, C_WF_SYNC_UUID), CS_VERBOSE);
		break;
	case C_STARTING_SYNC_S:
		drbd_start_resync(mdev, C_SYNC_SOURCE);
		break;
	}
}

int drbd_bitmap_io_from_worker(struct drbd_conf *mdev,
		int (*io_fn)(struct drbd_conf *),
		char *why, enum bm_flag flags)
{
	int rv;

	D_ASSERT(current == mdev->worker.task);

	/* open coded non-blocking drbd_suspend_io(mdev); */
	set_bit(SUSPEND_IO, &mdev->flags);

	drbd_bm_lock(mdev, why, flags);
	rv = io_fn(mdev);
	drbd_bm_unlock(mdev);

	drbd_resume_io(mdev);

	return rv;
}

/**
 * after_state_ch() - Perform after state change actions that may sleep
 * @mdev:	DRBD device.
 * @os:		old state.
 * @ns:		new state.
 * @flags:	Flags
 */
static void after_state_ch(struct drbd_conf *mdev, union drbd_state os,
			   union drbd_state ns, enum chg_state_flags flags)
{
	enum drbd_fencing_p fp;
	enum drbd_req_event what = nothing;
	union drbd_state nsm = (union drbd_state){ .i = -1 };

	if (os.conn != C_CONNECTED && ns.conn == C_CONNECTED) {
		clear_bit(CRASHED_PRIMARY, &mdev->flags);
		if (mdev->p_uuid)
			mdev->p_uuid[UI_FLAGS] &= ~((u64)2);
	}

	fp = FP_DONT_CARE;
	if (get_ldev(mdev)) {
		fp = mdev->ldev->dc.fencing;
		put_ldev(mdev);
	}

	/* Inform userspace about the change... */
	drbd_bcast_state(mdev, ns);

	if (!(os.role == R_PRIMARY && os.disk < D_UP_TO_DATE && os.pdsk < D_UP_TO_DATE) &&
	    (ns.role == R_PRIMARY && ns.disk < D_UP_TO_DATE && ns.pdsk < D_UP_TO_DATE))
		drbd_khelper(mdev, "pri-on-incon-degr");

	/* Here we have the actions that are performed after a
	   state change. This function might sleep */

	nsm.i = -1;
	if (ns.susp_nod) {
		if (os.conn < C_CONNECTED && ns.conn >= C_CONNECTED)
			what = resend;

		if (os.disk == D_ATTACHING && ns.disk > D_ATTACHING)
			what = restart_frozen_disk_io;

		if (what != nothing)
			nsm.susp_nod = 0;
	}

	if (ns.susp_fen) {
		/* case1: The outdate peer handler is successful: */
		if (os.pdsk > D_OUTDATED  && ns.pdsk <= D_OUTDATED) {
			tl_clear(mdev);
			if (test_bit(NEW_CUR_UUID, &mdev->flags)) {
				drbd_uuid_new_current(mdev);
				clear_bit(NEW_CUR_UUID, &mdev->flags);
			}
			spin_lock_irq(&mdev->req_lock);
			_drbd_set_state(_NS(mdev, susp_fen, 0), CS_VERBOSE, NULL);
			spin_unlock_irq(&mdev->req_lock);
		}
		/* case2: The connection was established again: */
		if (os.conn < C_CONNECTED && ns.conn >= C_CONNECTED) {
			clear_bit(NEW_CUR_UUID, &mdev->flags);
			what = resend;
			nsm.susp_fen = 0;
		}
	}

	if (what != nothing) {
		spin_lock_irq(&mdev->req_lock);
		_tl_restart(mdev, what);
		nsm.i &= mdev->state.i;
		_drbd_set_state(mdev, nsm, CS_VERBOSE, NULL);
		spin_unlock_irq(&mdev->req_lock);
	}

	/* Became sync source.  With protocol >= 96, we still need to send out
	 * the sync uuid now. Need to do that before any drbd_send_state, or
	 * the other side may go "paused sync" before receiving the sync uuids,
	 * which is unexpected. */
	if ((os.conn != C_SYNC_SOURCE && os.conn != C_PAUSED_SYNC_S) &&
	    (ns.conn == C_SYNC_SOURCE || ns.conn == C_PAUSED_SYNC_S) &&
	    mdev->agreed_pro_version >= 96 && get_ldev(mdev)) {
		drbd_gen_and_send_sync_uuid(mdev);
		put_ldev(mdev);
	}

	/* Do not change the order of the if above and the two below... */
	if (os.pdsk == D_DISKLESS && ns.pdsk > D_DISKLESS) {      /* attach on the peer */
		drbd_send_uuids(mdev);
		drbd_send_state(mdev);
	}
	/* No point in queuing send_bitmap if we don't have a connection
	 * anymore, so check also the _current_ state, not only the new state
	 * at the time this work was queued. */
	if (os.conn != C_WF_BITMAP_S && ns.conn == C_WF_BITMAP_S &&
	    mdev->state.conn == C_WF_BITMAP_S)
		drbd_queue_bitmap_io(mdev, &drbd_send_bitmap, NULL,
				"send_bitmap (WFBitMapS)",
				BM_LOCKED_TEST_ALLOWED);

	/* Lost contact to peer's copy of the data */
	if ((os.pdsk >= D_INCONSISTENT &&
	     os.pdsk != D_UNKNOWN &&
	     os.pdsk != D_OUTDATED)
	&&  (ns.pdsk < D_INCONSISTENT ||
	     ns.pdsk == D_UNKNOWN ||
	     ns.pdsk == D_OUTDATED)) {
		if (get_ldev(mdev)) {
			if ((ns.role == R_PRIMARY || ns.peer == R_PRIMARY) &&
			    mdev->ldev->md.uuid[UI_BITMAP] == 0 && ns.disk >= D_UP_TO_DATE) {
				if (is_susp(mdev->state)) {
					set_bit(NEW_CUR_UUID, &mdev->flags);
				} else {
					drbd_uuid_new_current(mdev);
					drbd_send_uuids(mdev);
				}
			}
			put_ldev(mdev);
		}
	}

	if (ns.pdsk < D_INCONSISTENT && get_ldev(mdev)) {
		if (ns.peer == R_PRIMARY && mdev->ldev->md.uuid[UI_BITMAP] == 0) {
			drbd_uuid_new_current(mdev);
			drbd_send_uuids(mdev);
		}

		/* D_DISKLESS Peer becomes secondary */
		if (os.peer == R_PRIMARY && ns.peer == R_SECONDARY)
			/* We may still be Primary ourselves.
			 * No harm done if the bitmap still changes,
			 * redirtied pages will follow later. */
			drbd_bitmap_io_from_worker(mdev, &drbd_bm_write,
				"demote diskless peer", BM_LOCKED_SET_ALLOWED);
		put_ldev(mdev);
	}

	/* Write out all changed bits on demote.
	 * Though, no need to da that just yet
	 * if there is a resync going on still */
	if (os.role == R_PRIMARY && ns.role == R_SECONDARY &&
		mdev->state.conn <= C_CONNECTED && get_ldev(mdev)) {
		/* No changes to the bitmap expected this time, so assert that,
		 * even though no harm was done if it did change. */
		drbd_bitmap_io_from_worker(mdev, &drbd_bm_write,
				"demote", BM_LOCKED_TEST_ALLOWED);
		put_ldev(mdev);
	}

	/* Last part of the attaching process ... */
	if (ns.conn >= C_CONNECTED &&
	    os.disk == D_ATTACHING && ns.disk == D_NEGOTIATING) {
		drbd_send_sizes(mdev, 0, 0);  /* to start sync... */
		drbd_send_uuids(mdev);
		drbd_send_state(mdev);
	}

	/* We want to pause/continue resync, tell peer. */
	if (ns.conn >= C_CONNECTED &&
	     ((os.aftr_isp != ns.aftr_isp) ||
	      (os.user_isp != ns.user_isp)))
		drbd_send_state(mdev);

	/* In case one of the isp bits got set, suspend other devices. */
	if ((!os.aftr_isp && !os.peer_isp && !os.user_isp) &&
	    (ns.aftr_isp || ns.peer_isp || ns.user_isp))
		suspend_other_sg(mdev);

	/* Make sure the peer gets informed about eventual state
	   changes (ISP bits) while we were in WFReportParams. */
	if (os.conn == C_WF_REPORT_PARAMS && ns.conn >= C_CONNECTED)
		drbd_send_state(mdev);

	if (os.conn != C_AHEAD && ns.conn == C_AHEAD)
		drbd_send_state(mdev);

	/* We are in the progress to start a full sync... */
	if ((os.conn != C_STARTING_SYNC_T && ns.conn == C_STARTING_SYNC_T) ||
	    (os.conn != C_STARTING_SYNC_S && ns.conn == C_STARTING_SYNC_S))
		/* no other bitmap changes expected during this phase */
		drbd_queue_bitmap_io(mdev,
			&drbd_bmio_set_n_write, &abw_start_sync,
			"set_n_write from StartingSync", BM_LOCKED_TEST_ALLOWED);

	/* We are invalidating our self... */
	if (os.conn < C_CONNECTED && ns.conn < C_CONNECTED &&
	    os.disk > D_INCONSISTENT && ns.disk == D_INCONSISTENT)
		/* other bitmap operation expected during this phase */
		drbd_queue_bitmap_io(mdev, &drbd_bmio_set_n_write, NULL,
			"set_n_write from invalidate", BM_LOCKED_MASK);

	/* first half of local IO error, failure to attach,
	 * or administrative detach */
	if (os.disk != D_FAILED && ns.disk == D_FAILED) {
		enum drbd_io_error_p eh;
		int was_io_error;
		/* corresponding get_ldev was in __drbd_set_state, to serialize
		 * our cleanup here with the transition to D_DISKLESS,
		 * so it is safe to dreference ldev here. */
		eh = mdev->ldev->dc.on_io_error;
		was_io_error = test_and_clear_bit(WAS_IO_ERROR, &mdev->flags);

		/* current state still has to be D_FAILED,
		 * there is only one way out: to D_DISKLESS,
		 * and that may only happen after our put_ldev below. */
		if (mdev->state.disk != D_FAILED)
			dev_err(DEV,
				"ASSERT FAILED: disk is %s during detach\n",
				drbd_disk_str(mdev->state.disk));

		if (drbd_send_state(mdev))
			dev_warn(DEV, "Notified peer that I am detaching my disk\n");
		else
			dev_err(DEV, "Sending state for detaching disk failed\n");

		drbd_rs_cancel_all(mdev);

		/* In case we want to get something to stable storage still,
		 * this may be the last chance.
		 * Following put_ldev may transition to D_DISKLESS. */
		drbd_md_sync(mdev);
		put_ldev(mdev);

		if (was_io_error && eh == EP_CALL_HELPER)
			drbd_khelper(mdev, "local-io-error");
	}

        /* second half of local IO error, failure to attach,
         * or administrative detach,
         * after local_cnt references have reached zero again */
        if (os.disk != D_DISKLESS && ns.disk == D_DISKLESS) {
                /* We must still be diskless,
                 * re-attach has to be serialized with this! */
                if (mdev->state.disk != D_DISKLESS)
                        dev_err(DEV,
                                "ASSERT FAILED: disk is %s while going diskless\n",
                                drbd_disk_str(mdev->state.disk));

                mdev->rs_total = 0;
                mdev->rs_failed = 0;
                atomic_set(&mdev->rs_pending_cnt, 0);

		if (drbd_send_state(mdev))
			dev_warn(DEV, "Notified peer that I'm now diskless.\n");
		/* corresponding get_ldev in __drbd_set_state
		 * this may finally trigger drbd_ldev_destroy. */
		put_ldev(mdev);
	}

	/* Notify peer that I had a local IO error, and did not detached.. */
	if (os.disk == D_UP_TO_DATE && ns.disk == D_INCONSISTENT)
		drbd_send_state(mdev);

	/* Disks got bigger while they were detached */
	if (ns.disk > D_NEGOTIATING && ns.pdsk > D_NEGOTIATING &&
	    test_and_clear_bit(RESYNC_AFTER_NEG, &mdev->flags)) {
		if (ns.conn == C_CONNECTED)
			resync_after_online_grow(mdev);
	}

	/* A resync finished or aborted, wake paused devices... */
	if ((os.conn > C_CONNECTED && ns.conn <= C_CONNECTED) ||
	    (os.peer_isp && !ns.peer_isp) ||
	    (os.user_isp && !ns.user_isp))
		resume_next_sg(mdev);

	/* sync target done with resync.  Explicitly notify peer, even though
	 * it should (at least for non-empty resyncs) already know itself. */
	if (os.disk < D_UP_TO_DATE && os.conn >= C_SYNC_SOURCE && ns.conn == C_CONNECTED)
		drbd_send_state(mdev);

	/* This triggers bitmap writeout of potentially still unwritten pages
	 * if the resync finished cleanly, or aborted because of peer disk
	 * failure, or because of connection loss.
	 * For resync aborted because of local disk failure, we cannot do
	 * any bitmap writeout anymore.
	 * No harm done if some bits change during this phase.
	 */
	if (os.conn > C_CONNECTED && ns.conn <= C_CONNECTED && get_ldev(mdev)) {
		drbd_queue_bitmap_io(mdev, &drbd_bm_write, NULL,
			"write from resync_finished", BM_LOCKED_SET_ALLOWED);
		put_ldev(mdev);
	}

	/* free tl_hash if we Got thawed and are C_STANDALONE */
	if (ns.conn == C_STANDALONE && !is_susp(ns) && mdev->tl_hash)
		drbd_free_tl_hash(mdev);

	/* Upon network connection, we need to start the receiver */
	if (os.conn == C_STANDALONE && ns.conn == C_UNCONNECTED)
		drbd_thread_start(&mdev->receiver);

	/* Terminate worker thread if we are unconfigured - it will be
	   restarted as needed... */
	if (ns.disk == D_DISKLESS &&
	    ns.conn == C_STANDALONE &&
	    ns.role == R_SECONDARY) {
		if (os.aftr_isp != ns.aftr_isp)
			resume_next_sg(mdev);
		/* set in __drbd_set_state, unless CONFIG_PENDING was set */
		if (test_bit(DEVICE_DYING, &mdev->flags))
			drbd_thread_stop_nowait(&mdev->worker);
	}

	drbd_md_sync(mdev);
}


static int drbd_thread_setup(void *arg)
{
	struct drbd_thread *thi = (struct drbd_thread *) arg;
	struct drbd_conf *mdev = thi->mdev;
	unsigned long flags;
	int retval;

=======
 * @what might be one of CONNECTION_LOST_WHILE_PENDING, RESEND, FAIL_FROZEN_DISK_IO,
 * RESTART_FROZEN_DISK_IO.
 */
/* must hold resource->req_lock */
void _tl_restart(struct drbd_tconn *tconn, enum drbd_req_event what)
{
	struct drbd_request *req, *r;

	list_for_each_entry_safe(req, r, &tconn->transfer_log, tl_requests)
		_req_mod(req, what);
}

void tl_restart(struct drbd_tconn *tconn, enum drbd_req_event what)
{
	spin_lock_irq(&tconn->req_lock);
	_tl_restart(tconn, what);
	spin_unlock_irq(&tconn->req_lock);
}

/**
 * tl_clear() - Clears all requests and &struct drbd_tl_epoch objects out of the TL
 * @mdev:	DRBD device.
 *
 * This is called after the connection to the peer was lost. The storage covered
 * by the requests on the transfer gets marked as our of sync. Called from the
 * receiver thread and the worker thread.
 */
void tl_clear(struct drbd_tconn *tconn)
{
	tl_restart(tconn, CONNECTION_LOST_WHILE_PENDING);
}

/**
 * tl_abort_disk_io() - Abort disk I/O for all requests for a certain mdev in the TL
 * @mdev:	DRBD device.
 */
void tl_abort_disk_io(struct drbd_conf *mdev)
{
	struct drbd_tconn *tconn = mdev->tconn;
	struct drbd_request *req, *r;

	spin_lock_irq(&tconn->req_lock);
	list_for_each_entry_safe(req, r, &tconn->transfer_log, tl_requests) {
		if (!(req->rq_state & RQ_LOCAL_PENDING))
			continue;
		if (req->w.mdev != mdev)
			continue;
		_req_mod(req, ABORT_DISK_IO);
	}
	spin_unlock_irq(&tconn->req_lock);
}

static int drbd_thread_setup(void *arg)
{
	struct drbd_thread *thi = (struct drbd_thread *) arg;
	struct drbd_tconn *tconn = thi->tconn;
	unsigned long flags;
	int retval;

	snprintf(current->comm, sizeof(current->comm), "drbd_%c_%s",
		 thi->name[0], thi->tconn->name);

>>>>>>> refs/remotes/origin/master
restart:
	retval = thi->function(thi);

	spin_lock_irqsave(&thi->t_lock, flags);

<<<<<<< HEAD
	/* if the receiver has been "Exiting", the last thing it did
	 * was set the conn state to "StandAlone",
	 * if now a re-connect request comes in, conn state goes C_UNCONNECTED,
	 * and receiver thread will be "started".
	 * drbd_thread_start needs to set "Restarting" in that case.
	 * t_state check and assignment needs to be within the same spinlock,
	 * so either thread_start sees Exiting, and can remap to Restarting,
	 * or thread_start see None, and can proceed as normal.
	 */

	if (thi->t_state == Restarting) {
		dev_info(DEV, "Restarting %s\n", current->comm);
		thi->t_state = Running;
=======
	/* if the receiver has been "EXITING", the last thing it did
	 * was set the conn state to "StandAlone",
	 * if now a re-connect request comes in, conn state goes C_UNCONNECTED,
	 * and receiver thread will be "started".
	 * drbd_thread_start needs to set "RESTARTING" in that case.
	 * t_state check and assignment needs to be within the same spinlock,
	 * so either thread_start sees EXITING, and can remap to RESTARTING,
	 * or thread_start see NONE, and can proceed as normal.
	 */

	if (thi->t_state == RESTARTING) {
		conn_info(tconn, "Restarting %s thread\n", thi->name);
		thi->t_state = RUNNING;
>>>>>>> refs/remotes/origin/master
		spin_unlock_irqrestore(&thi->t_lock, flags);
		goto restart;
	}

	thi->task = NULL;
<<<<<<< HEAD
	thi->t_state = None;
	smp_mb();
	complete(&thi->stop);
	spin_unlock_irqrestore(&thi->t_lock, flags);

	dev_info(DEV, "Terminating %s\n", current->comm);

	/* Release mod reference taken when thread was started */
=======
	thi->t_state = NONE;
	smp_mb();
	complete_all(&thi->stop);
	spin_unlock_irqrestore(&thi->t_lock, flags);

	conn_info(tconn, "Terminating %s\n", current->comm);

	/* Release mod reference taken when thread was started */

	kref_put(&tconn->kref, &conn_destroy);
>>>>>>> refs/remotes/origin/master
	module_put(THIS_MODULE);
	return retval;
}

<<<<<<< HEAD
static void drbd_thread_init(struct drbd_conf *mdev, struct drbd_thread *thi,
		      int (*func) (struct drbd_thread *))
{
	spin_lock_init(&thi->t_lock);
	thi->task    = NULL;
	thi->t_state = None;
	thi->function = func;
	thi->mdev = mdev;
=======
static void drbd_thread_init(struct drbd_tconn *tconn, struct drbd_thread *thi,
			     int (*func) (struct drbd_thread *), char *name)
{
	spin_lock_init(&thi->t_lock);
	thi->task    = NULL;
	thi->t_state = NONE;
	thi->function = func;
	thi->tconn = tconn;
	strncpy(thi->name, name, ARRAY_SIZE(thi->name));
>>>>>>> refs/remotes/origin/master
}

int drbd_thread_start(struct drbd_thread *thi)
{
<<<<<<< HEAD
	struct drbd_conf *mdev = thi->mdev;
	struct task_struct *nt;
	unsigned long flags;

	const char *me =
		thi == &mdev->receiver ? "receiver" :
		thi == &mdev->asender  ? "asender"  :
		thi == &mdev->worker   ? "worker"   : "NONSENSE";

=======
	struct drbd_tconn *tconn = thi->tconn;
	struct task_struct *nt;
	unsigned long flags;

>>>>>>> refs/remotes/origin/master
	/* is used from state engine doing drbd_thread_stop_nowait,
	 * while holding the req lock irqsave */
	spin_lock_irqsave(&thi->t_lock, flags);

	switch (thi->t_state) {
<<<<<<< HEAD
	case None:
		dev_info(DEV, "Starting %s thread (from %s [%d])\n",
				me, current->comm, current->pid);

		/* Get ref on module for thread - this is released when thread exits */
		if (!try_module_get(THIS_MODULE)) {
			dev_err(DEV, "Failed to get module reference in drbd_thread_start\n");
=======
	case NONE:
		conn_info(tconn, "Starting %s thread (from %s [%d])\n",
			 thi->name, current->comm, current->pid);

		/* Get ref on module for thread - this is released when thread exits */
		if (!try_module_get(THIS_MODULE)) {
			conn_err(tconn, "Failed to get module reference in drbd_thread_start\n");
>>>>>>> refs/remotes/origin/master
			spin_unlock_irqrestore(&thi->t_lock, flags);
			return false;
		}

<<<<<<< HEAD
		init_completion(&thi->stop);
		D_ASSERT(thi->task == NULL);
		thi->reset_cpu_mask = 1;
		thi->t_state = Running;
=======
		kref_get(&thi->tconn->kref);

		init_completion(&thi->stop);
		thi->reset_cpu_mask = 1;
		thi->t_state = RUNNING;
>>>>>>> refs/remotes/origin/master
		spin_unlock_irqrestore(&thi->t_lock, flags);
		flush_signals(current); /* otherw. may get -ERESTARTNOINTR */

		nt = kthread_create(drbd_thread_setup, (void *) thi,
<<<<<<< HEAD
				    "drbd%d_%s", mdev_to_minor(mdev), me);

		if (IS_ERR(nt)) {
			dev_err(DEV, "Couldn't start thread\n");

=======
				    "drbd_%c_%s", thi->name[0], thi->tconn->name);

		if (IS_ERR(nt)) {
			conn_err(tconn, "Couldn't start thread\n");

			kref_put(&tconn->kref, &conn_destroy);
>>>>>>> refs/remotes/origin/master
			module_put(THIS_MODULE);
			return false;
		}
		spin_lock_irqsave(&thi->t_lock, flags);
		thi->task = nt;
<<<<<<< HEAD
		thi->t_state = Running;
		spin_unlock_irqrestore(&thi->t_lock, flags);
		wake_up_process(nt);
		break;
	case Exiting:
		thi->t_state = Restarting;
		dev_info(DEV, "Restarting %s thread (from %s [%d])\n",
				me, current->comm, current->pid);
		/* fall through */
	case Running:
	case Restarting:
=======
		thi->t_state = RUNNING;
		spin_unlock_irqrestore(&thi->t_lock, flags);
		wake_up_process(nt);
		break;
	case EXITING:
		thi->t_state = RESTARTING;
		conn_info(tconn, "Restarting %s thread (from %s [%d])\n",
				thi->name, current->comm, current->pid);
		/* fall through */
	case RUNNING:
	case RESTARTING:
>>>>>>> refs/remotes/origin/master
	default:
		spin_unlock_irqrestore(&thi->t_lock, flags);
		break;
	}

	return true;
}


void _drbd_thread_stop(struct drbd_thread *thi, int restart, int wait)
{
	unsigned long flags;

<<<<<<< HEAD
	enum drbd_thread_state ns = restart ? Restarting : Exiting;
=======
	enum drbd_thread_state ns = restart ? RESTARTING : EXITING;
>>>>>>> refs/remotes/origin/master

	/* may be called from state engine, holding the req lock irqsave */
	spin_lock_irqsave(&thi->t_lock, flags);

<<<<<<< HEAD
	if (thi->t_state == None) {
=======
	if (thi->t_state == NONE) {
>>>>>>> refs/remotes/origin/master
		spin_unlock_irqrestore(&thi->t_lock, flags);
		if (restart)
			drbd_thread_start(thi);
		return;
	}

	if (thi->t_state != ns) {
		if (thi->task == NULL) {
			spin_unlock_irqrestore(&thi->t_lock, flags);
			return;
		}

		thi->t_state = ns;
		smp_mb();
		init_completion(&thi->stop);
		if (thi->task != current)
			force_sig(DRBD_SIGKILL, thi->task);
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
	}

	spin_unlock_irqrestore(&thi->t_lock, flags);

	if (wait)
		wait_for_completion(&thi->stop);
}

<<<<<<< HEAD
=======
static struct drbd_thread *drbd_task_to_thread(struct drbd_tconn *tconn, struct task_struct *task)
{
	struct drbd_thread *thi =
		task == tconn->receiver.task ? &tconn->receiver :
		task == tconn->asender.task  ? &tconn->asender :
		task == tconn->worker.task   ? &tconn->worker : NULL;

	return thi;
}

char *drbd_task_to_thread_name(struct drbd_tconn *tconn, struct task_struct *task)
{
	struct drbd_thread *thi = drbd_task_to_thread(tconn, task);
	return thi ? thi->name : task->comm;
}

int conn_lowest_minor(struct drbd_tconn *tconn)
{
	struct drbd_conf *mdev;
	int vnr = 0, m;

	rcu_read_lock();
	mdev = idr_get_next(&tconn->volumes, &vnr);
	m = mdev ? mdev_to_minor(mdev) : -1;
	rcu_read_unlock();

	return m;
}

>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_SMP
/**
 * drbd_calc_cpu_mask() - Generate CPU masks, spread over all CPUs
 * @mdev:	DRBD device.
 *
 * Forces all threads of a device onto the same CPU. This is beneficial for
 * DRBD's performance. May be overwritten by user's configuration.
 */
<<<<<<< HEAD
void drbd_calc_cpu_mask(struct drbd_conf *mdev)
=======
void drbd_calc_cpu_mask(struct drbd_tconn *tconn)
>>>>>>> refs/remotes/origin/master
{
	int ord, cpu;

	/* user override. */
<<<<<<< HEAD
	if (cpumask_weight(mdev->cpu_mask))
		return;

	ord = mdev_to_minor(mdev) % cpumask_weight(cpu_online_mask);
	for_each_online_cpu(cpu) {
		if (ord-- == 0) {
			cpumask_set_cpu(cpu, mdev->cpu_mask);
=======
	if (cpumask_weight(tconn->cpu_mask))
		return;

	ord = conn_lowest_minor(tconn) % cpumask_weight(cpu_online_mask);
	for_each_online_cpu(cpu) {
		if (ord-- == 0) {
			cpumask_set_cpu(cpu, tconn->cpu_mask);
>>>>>>> refs/remotes/origin/master
			return;
		}
	}
	/* should not be reached */
<<<<<<< HEAD
	cpumask_setall(mdev->cpu_mask);
=======
	cpumask_setall(tconn->cpu_mask);
>>>>>>> refs/remotes/origin/master
}

/**
 * drbd_thread_current_set_cpu() - modifies the cpu mask of the _current_ thread
 * @mdev:	DRBD device.
<<<<<<< HEAD
=======
 * @thi:	drbd_thread object
>>>>>>> refs/remotes/origin/master
 *
 * call in the "main loop" of _all_ threads, no need for any mutex, current won't die
 * prematurely.
 */
<<<<<<< HEAD
void drbd_thread_current_set_cpu(struct drbd_conf *mdev)
{
	struct task_struct *p = current;
	struct drbd_thread *thi =
		p == mdev->asender.task  ? &mdev->asender  :
		p == mdev->receiver.task ? &mdev->receiver :
		p == mdev->worker.task   ? &mdev->worker   :
		NULL;
	ERR_IF(thi == NULL)
		return;
	if (!thi->reset_cpu_mask)
		return;
	thi->reset_cpu_mask = 0;
	set_cpus_allowed_ptr(p, mdev->cpu_mask);
}
#endif

/* the appropriate socket mutex must be held already */
int _drbd_send_cmd(struct drbd_conf *mdev, struct socket *sock,
			  enum drbd_packets cmd, struct p_header80 *h,
			  size_t size, unsigned msg_flags)
{
	int sent, ok;

	ERR_IF(!h) return false;
	ERR_IF(!size) return false;

	h->magic   = BE_DRBD_MAGIC;
	h->command = cpu_to_be16(cmd);
	h->length  = cpu_to_be16(size-sizeof(struct p_header80));

	sent = drbd_send(mdev, sock, h, size, msg_flags);

	ok = (sent == size);
	if (!ok && !signal_pending(current))
		dev_warn(DEV, "short sent %s size=%d sent=%d\n",
		    cmdname(cmd), (int)size, sent);
	return ok;
}

/* don't pass the socket. we may only look at it
 * when we hold the appropriate socket mutex.
 */
int drbd_send_cmd(struct drbd_conf *mdev, int use_data_socket,
		  enum drbd_packets cmd, struct p_header80 *h, size_t size)
{
	int ok = 0;
	struct socket *sock;

	if (use_data_socket) {
		mutex_lock(&mdev->data.mutex);
		sock = mdev->data.socket;
	} else {
		mutex_lock(&mdev->meta.mutex);
		sock = mdev->meta.socket;
	}

	/* drbd_disconnect() could have called drbd_free_sock()
	 * while we were waiting in down()... */
	if (likely(sock != NULL))
		ok = _drbd_send_cmd(mdev, sock, cmd, h, size, 0);

	if (use_data_socket)
		mutex_unlock(&mdev->data.mutex);
	else
		mutex_unlock(&mdev->meta.mutex);
	return ok;
}

int drbd_send_cmd2(struct drbd_conf *mdev, enum drbd_packets cmd, char *data,
		   size_t size)
{
	struct p_header80 h;
	int ok;

	h.magic   = BE_DRBD_MAGIC;
	h.command = cpu_to_be16(cmd);
	h.length  = cpu_to_be16(size);

	if (!drbd_get_data_sock(mdev))
		return 0;

	ok = (sizeof(h) ==
		drbd_send(mdev, mdev->data.socket, &h, sizeof(h), 0));
	ok = ok && (size ==
		drbd_send(mdev, mdev->data.socket, data, size, 0));

	drbd_put_data_sock(mdev);

	return ok;
}

int drbd_send_sync_param(struct drbd_conf *mdev, struct syncer_conf *sc)
{
	struct p_rs_param_95 *p;
	struct socket *sock;
	int size, rv;
	const int apv = mdev->agreed_pro_version;

	size = apv <= 87 ? sizeof(struct p_rs_param)
		: apv == 88 ? sizeof(struct p_rs_param)
			+ strlen(mdev->sync_conf.verify_alg) + 1
		: apv <= 94 ? sizeof(struct p_rs_param_89)
		: /* apv >= 95 */ sizeof(struct p_rs_param_95);

	/* used from admin command context and receiver/worker context.
	 * to avoid kmalloc, grab the socket right here,
	 * then use the pre-allocated sbuf there */
	mutex_lock(&mdev->data.mutex);
	sock = mdev->data.socket;

	if (likely(sock != NULL)) {
		enum drbd_packets cmd = apv >= 89 ? P_SYNC_PARAM89 : P_SYNC_PARAM;

		p = &mdev->data.sbuf.rs_param_95;

		/* initialize verify_alg and csums_alg */
		memset(p->verify_alg, 0, 2 * SHARED_SECRET_MAX);

		p->rate = cpu_to_be32(sc->rate);
		p->c_plan_ahead = cpu_to_be32(sc->c_plan_ahead);
		p->c_delay_target = cpu_to_be32(sc->c_delay_target);
		p->c_fill_target = cpu_to_be32(sc->c_fill_target);
		p->c_max_rate = cpu_to_be32(sc->c_max_rate);

		if (apv >= 88)
			strcpy(p->verify_alg, mdev->sync_conf.verify_alg);
		if (apv >= 89)
			strcpy(p->csums_alg, mdev->sync_conf.csums_alg);

		rv = _drbd_send_cmd(mdev, sock, cmd, &p->head, size, 0);
	} else
		rv = 0; /* not ok */

	mutex_unlock(&mdev->data.mutex);

	return rv;
}

int drbd_send_protocol(struct drbd_conf *mdev)
{
	struct p_protocol *p;
	int size, cf, rv;

	size = sizeof(struct p_protocol);

	if (mdev->agreed_pro_version >= 87)
		size += strlen(mdev->net_conf->integrity_alg) + 1;

	/* we must not recurse into our own queue,
	 * as that is blocked during handshake */
	p = kmalloc(size, GFP_NOIO);
	if (p == NULL)
		return 0;

	p->protocol      = cpu_to_be32(mdev->net_conf->wire_protocol);
	p->after_sb_0p   = cpu_to_be32(mdev->net_conf->after_sb_0p);
	p->after_sb_1p   = cpu_to_be32(mdev->net_conf->after_sb_1p);
	p->after_sb_2p   = cpu_to_be32(mdev->net_conf->after_sb_2p);
	p->two_primaries = cpu_to_be32(mdev->net_conf->two_primaries);

	cf = 0;
	if (mdev->net_conf->want_lose)
		cf |= CF_WANT_LOSE;
	if (mdev->net_conf->dry_run) {
		if (mdev->agreed_pro_version >= 92)
			cf |= CF_DRY_RUN;
		else {
			dev_err(DEV, "--dry-run is not supported by peer");
			kfree(p);
			return -1;
		}
	}
	p->conn_flags    = cpu_to_be32(cf);

	if (mdev->agreed_pro_version >= 87)
		strcpy(p->integrity_alg, mdev->net_conf->integrity_alg);

	rv = drbd_send_cmd(mdev, USE_DATA_SOCKET, P_PROTOCOL,
			   (struct p_header80 *)p, size);
	kfree(p);
	return rv;
=======
void drbd_thread_current_set_cpu(struct drbd_thread *thi)
{
	struct task_struct *p = current;

	if (!thi->reset_cpu_mask)
		return;
	thi->reset_cpu_mask = 0;
	set_cpus_allowed_ptr(p, thi->tconn->cpu_mask);
}
#endif

/**
 * drbd_header_size  -  size of a packet header
 *
 * The header size is a multiple of 8, so any payload following the header is
 * word aligned on 64-bit architectures.  (The bitmap send and receive code
 * relies on this.)
 */
unsigned int drbd_header_size(struct drbd_tconn *tconn)
{
	if (tconn->agreed_pro_version >= 100) {
		BUILD_BUG_ON(!IS_ALIGNED(sizeof(struct p_header100), 8));
		return sizeof(struct p_header100);
	} else {
		BUILD_BUG_ON(sizeof(struct p_header80) !=
			     sizeof(struct p_header95));
		BUILD_BUG_ON(!IS_ALIGNED(sizeof(struct p_header80), 8));
		return sizeof(struct p_header80);
	}
}

static unsigned int prepare_header80(struct p_header80 *h, enum drbd_packet cmd, int size)
{
	h->magic   = cpu_to_be32(DRBD_MAGIC);
	h->command = cpu_to_be16(cmd);
	h->length  = cpu_to_be16(size);
	return sizeof(struct p_header80);
}

static unsigned int prepare_header95(struct p_header95 *h, enum drbd_packet cmd, int size)
{
	h->magic   = cpu_to_be16(DRBD_MAGIC_BIG);
	h->command = cpu_to_be16(cmd);
	h->length = cpu_to_be32(size);
	return sizeof(struct p_header95);
}

static unsigned int prepare_header100(struct p_header100 *h, enum drbd_packet cmd,
				      int size, int vnr)
{
	h->magic = cpu_to_be32(DRBD_MAGIC_100);
	h->volume = cpu_to_be16(vnr);
	h->command = cpu_to_be16(cmd);
	h->length = cpu_to_be32(size);
	h->pad = 0;
	return sizeof(struct p_header100);
}

static unsigned int prepare_header(struct drbd_tconn *tconn, int vnr,
				   void *buffer, enum drbd_packet cmd, int size)
{
	if (tconn->agreed_pro_version >= 100)
		return prepare_header100(buffer, cmd, size, vnr);
	else if (tconn->agreed_pro_version >= 95 &&
		 size > DRBD_MAX_SIZE_H80_PACKET)
		return prepare_header95(buffer, cmd, size);
	else
		return prepare_header80(buffer, cmd, size);
}

static void *__conn_prepare_command(struct drbd_tconn *tconn,
				    struct drbd_socket *sock)
{
	if (!sock->socket)
		return NULL;
	return sock->sbuf + drbd_header_size(tconn);
}

void *conn_prepare_command(struct drbd_tconn *tconn, struct drbd_socket *sock)
{
	void *p;

	mutex_lock(&sock->mutex);
	p = __conn_prepare_command(tconn, sock);
	if (!p)
		mutex_unlock(&sock->mutex);

	return p;
}

void *drbd_prepare_command(struct drbd_conf *mdev, struct drbd_socket *sock)
{
	return conn_prepare_command(mdev->tconn, sock);
}

static int __send_command(struct drbd_tconn *tconn, int vnr,
			  struct drbd_socket *sock, enum drbd_packet cmd,
			  unsigned int header_size, void *data,
			  unsigned int size)
{
	int msg_flags;
	int err;

	/*
	 * Called with @data == NULL and the size of the data blocks in @size
	 * for commands that send data blocks.  For those commands, omit the
	 * MSG_MORE flag: this will increase the likelihood that data blocks
	 * which are page aligned on the sender will end up page aligned on the
	 * receiver.
	 */
	msg_flags = data ? MSG_MORE : 0;

	header_size += prepare_header(tconn, vnr, sock->sbuf, cmd,
				      header_size + size);
	err = drbd_send_all(tconn, sock->socket, sock->sbuf, header_size,
			    msg_flags);
	if (data && !err)
		err = drbd_send_all(tconn, sock->socket, data, size, 0);
	return err;
}

static int __conn_send_command(struct drbd_tconn *tconn, struct drbd_socket *sock,
			       enum drbd_packet cmd, unsigned int header_size,
			       void *data, unsigned int size)
{
	return __send_command(tconn, 0, sock, cmd, header_size, data, size);
}

int conn_send_command(struct drbd_tconn *tconn, struct drbd_socket *sock,
		      enum drbd_packet cmd, unsigned int header_size,
		      void *data, unsigned int size)
{
	int err;

	err = __conn_send_command(tconn, sock, cmd, header_size, data, size);
	mutex_unlock(&sock->mutex);
	return err;
}

int drbd_send_command(struct drbd_conf *mdev, struct drbd_socket *sock,
		      enum drbd_packet cmd, unsigned int header_size,
		      void *data, unsigned int size)
{
	int err;

	err = __send_command(mdev->tconn, mdev->vnr, sock, cmd, header_size,
			     data, size);
	mutex_unlock(&sock->mutex);
	return err;
}

int drbd_send_ping(struct drbd_tconn *tconn)
{
	struct drbd_socket *sock;

	sock = &tconn->meta;
	if (!conn_prepare_command(tconn, sock))
		return -EIO;
	return conn_send_command(tconn, sock, P_PING, 0, NULL, 0);
}

int drbd_send_ping_ack(struct drbd_tconn *tconn)
{
	struct drbd_socket *sock;

	sock = &tconn->meta;
	if (!conn_prepare_command(tconn, sock))
		return -EIO;
	return conn_send_command(tconn, sock, P_PING_ACK, 0, NULL, 0);
}

int drbd_send_sync_param(struct drbd_conf *mdev)
{
	struct drbd_socket *sock;
	struct p_rs_param_95 *p;
	int size;
	const int apv = mdev->tconn->agreed_pro_version;
	enum drbd_packet cmd;
	struct net_conf *nc;
	struct disk_conf *dc;

	sock = &mdev->tconn->data;
	p = drbd_prepare_command(mdev, sock);
	if (!p)
		return -EIO;

	rcu_read_lock();
	nc = rcu_dereference(mdev->tconn->net_conf);

	size = apv <= 87 ? sizeof(struct p_rs_param)
		: apv == 88 ? sizeof(struct p_rs_param)
			+ strlen(nc->verify_alg) + 1
		: apv <= 94 ? sizeof(struct p_rs_param_89)
		: /* apv >= 95 */ sizeof(struct p_rs_param_95);

	cmd = apv >= 89 ? P_SYNC_PARAM89 : P_SYNC_PARAM;

	/* initialize verify_alg and csums_alg */
	memset(p->verify_alg, 0, 2 * SHARED_SECRET_MAX);

	if (get_ldev(mdev)) {
		dc = rcu_dereference(mdev->ldev->disk_conf);
		p->resync_rate = cpu_to_be32(dc->resync_rate);
		p->c_plan_ahead = cpu_to_be32(dc->c_plan_ahead);
		p->c_delay_target = cpu_to_be32(dc->c_delay_target);
		p->c_fill_target = cpu_to_be32(dc->c_fill_target);
		p->c_max_rate = cpu_to_be32(dc->c_max_rate);
		put_ldev(mdev);
	} else {
		p->resync_rate = cpu_to_be32(DRBD_RESYNC_RATE_DEF);
		p->c_plan_ahead = cpu_to_be32(DRBD_C_PLAN_AHEAD_DEF);
		p->c_delay_target = cpu_to_be32(DRBD_C_DELAY_TARGET_DEF);
		p->c_fill_target = cpu_to_be32(DRBD_C_FILL_TARGET_DEF);
		p->c_max_rate = cpu_to_be32(DRBD_C_MAX_RATE_DEF);
	}

	if (apv >= 88)
		strcpy(p->verify_alg, nc->verify_alg);
	if (apv >= 89)
		strcpy(p->csums_alg, nc->csums_alg);
	rcu_read_unlock();

	return drbd_send_command(mdev, sock, cmd, size, NULL, 0);
}

int __drbd_send_protocol(struct drbd_tconn *tconn, enum drbd_packet cmd)
{
	struct drbd_socket *sock;
	struct p_protocol *p;
	struct net_conf *nc;
	int size, cf;

	sock = &tconn->data;
	p = __conn_prepare_command(tconn, sock);
	if (!p)
		return -EIO;

	rcu_read_lock();
	nc = rcu_dereference(tconn->net_conf);

	if (nc->tentative && tconn->agreed_pro_version < 92) {
		rcu_read_unlock();
		mutex_unlock(&sock->mutex);
		conn_err(tconn, "--dry-run is not supported by peer");
		return -EOPNOTSUPP;
	}

	size = sizeof(*p);
	if (tconn->agreed_pro_version >= 87)
		size += strlen(nc->integrity_alg) + 1;

	p->protocol      = cpu_to_be32(nc->wire_protocol);
	p->after_sb_0p   = cpu_to_be32(nc->after_sb_0p);
	p->after_sb_1p   = cpu_to_be32(nc->after_sb_1p);
	p->after_sb_2p   = cpu_to_be32(nc->after_sb_2p);
	p->two_primaries = cpu_to_be32(nc->two_primaries);
	cf = 0;
	if (nc->discard_my_data)
		cf |= CF_DISCARD_MY_DATA;
	if (nc->tentative)
		cf |= CF_DRY_RUN;
	p->conn_flags    = cpu_to_be32(cf);

	if (tconn->agreed_pro_version >= 87)
		strcpy(p->integrity_alg, nc->integrity_alg);
	rcu_read_unlock();

	return __conn_send_command(tconn, sock, cmd, size, NULL, 0);
}

int drbd_send_protocol(struct drbd_tconn *tconn)
{
	int err;

	mutex_lock(&tconn->data.mutex);
	err = __drbd_send_protocol(tconn, P_PROTOCOL);
	mutex_unlock(&tconn->data.mutex);

	return err;
>>>>>>> refs/remotes/origin/master
}

int _drbd_send_uuids(struct drbd_conf *mdev, u64 uuid_flags)
{
<<<<<<< HEAD
	struct p_uuids p;
	int i;

	if (!get_ldev_if_state(mdev, D_NEGOTIATING))
		return 1;

	for (i = UI_CURRENT; i < UI_SIZE; i++)
		p.uuid[i] = mdev->ldev ? cpu_to_be64(mdev->ldev->md.uuid[i]) : 0;

	mdev->comm_bm_set = drbd_bm_total_weight(mdev);
	p.uuid[UI_SIZE] = cpu_to_be64(mdev->comm_bm_set);
	uuid_flags |= mdev->net_conf->want_lose ? 1 : 0;
	uuid_flags |= test_bit(CRASHED_PRIMARY, &mdev->flags) ? 2 : 0;
	uuid_flags |= mdev->new_state_tmp.disk == D_INCONSISTENT ? 4 : 0;
	p.uuid[UI_FLAGS] = cpu_to_be64(uuid_flags);

	put_ldev(mdev);

	return drbd_send_cmd(mdev, USE_DATA_SOCKET, P_UUIDS,
			     (struct p_header80 *)&p, sizeof(p));
=======
	struct drbd_socket *sock;
	struct p_uuids *p;
	int i;

	if (!get_ldev_if_state(mdev, D_NEGOTIATING))
		return 0;

	sock = &mdev->tconn->data;
	p = drbd_prepare_command(mdev, sock);
	if (!p) {
		put_ldev(mdev);
		return -EIO;
	}
	spin_lock_irq(&mdev->ldev->md.uuid_lock);
	for (i = UI_CURRENT; i < UI_SIZE; i++)
		p->uuid[i] = cpu_to_be64(mdev->ldev->md.uuid[i]);
	spin_unlock_irq(&mdev->ldev->md.uuid_lock);

	mdev->comm_bm_set = drbd_bm_total_weight(mdev);
	p->uuid[UI_SIZE] = cpu_to_be64(mdev->comm_bm_set);
	rcu_read_lock();
	uuid_flags |= rcu_dereference(mdev->tconn->net_conf)->discard_my_data ? 1 : 0;
	rcu_read_unlock();
	uuid_flags |= test_bit(CRASHED_PRIMARY, &mdev->flags) ? 2 : 0;
	uuid_flags |= mdev->new_state_tmp.disk == D_INCONSISTENT ? 4 : 0;
	p->uuid[UI_FLAGS] = cpu_to_be64(uuid_flags);

	put_ldev(mdev);
	return drbd_send_command(mdev, sock, P_UUIDS, sizeof(*p), NULL, 0);
>>>>>>> refs/remotes/origin/master
}

int drbd_send_uuids(struct drbd_conf *mdev)
{
	return _drbd_send_uuids(mdev, 0);
}

int drbd_send_uuids_skip_initial_sync(struct drbd_conf *mdev)
{
	return _drbd_send_uuids(mdev, 8);
}

void drbd_print_uuids(struct drbd_conf *mdev, const char *text)
{
	if (get_ldev_if_state(mdev, D_NEGOTIATING)) {
		u64 *uuid = mdev->ldev->md.uuid;
		dev_info(DEV, "%s %016llX:%016llX:%016llX:%016llX\n",
		     text,
		     (unsigned long long)uuid[UI_CURRENT],
		     (unsigned long long)uuid[UI_BITMAP],
		     (unsigned long long)uuid[UI_HISTORY_START],
		     (unsigned long long)uuid[UI_HISTORY_END]);
		put_ldev(mdev);
	} else {
		dev_info(DEV, "%s effective data uuid: %016llX\n",
				text,
				(unsigned long long)mdev->ed_uuid);
	}
}

<<<<<<< HEAD
int drbd_gen_and_send_sync_uuid(struct drbd_conf *mdev)
{
	struct p_rs_uuid p;
=======
void drbd_gen_and_send_sync_uuid(struct drbd_conf *mdev)
{
	struct drbd_socket *sock;
	struct p_rs_uuid *p;
>>>>>>> refs/remotes/origin/master
	u64 uuid;

	D_ASSERT(mdev->state.disk == D_UP_TO_DATE);

<<<<<<< HEAD
	uuid = mdev->ldev->md.uuid[UI_BITMAP] + UUID_NEW_BM_OFFSET;
	drbd_uuid_set(mdev, UI_BITMAP, uuid);
	drbd_print_uuids(mdev, "updated sync UUID");
	drbd_md_sync(mdev);
	p.uuid = cpu_to_be64(uuid);

	return drbd_send_cmd(mdev, USE_DATA_SOCKET, P_SYNC_UUID,
			     (struct p_header80 *)&p, sizeof(p));
=======
	uuid = mdev->ldev->md.uuid[UI_BITMAP];
	if (uuid && uuid != UUID_JUST_CREATED)
		uuid = uuid + UUID_NEW_BM_OFFSET;
	else
		get_random_bytes(&uuid, sizeof(u64));
	drbd_uuid_set(mdev, UI_BITMAP, uuid);
	drbd_print_uuids(mdev, "updated sync UUID");
	drbd_md_sync(mdev);

	sock = &mdev->tconn->data;
	p = drbd_prepare_command(mdev, sock);
	if (p) {
		p->uuid = cpu_to_be64(uuid);
		drbd_send_command(mdev, sock, P_SYNC_UUID, sizeof(*p), NULL, 0);
	}
>>>>>>> refs/remotes/origin/master
}

int drbd_send_sizes(struct drbd_conf *mdev, int trigger_reply, enum dds_flags flags)
{
<<<<<<< HEAD
	struct p_sizes p;
	sector_t d_size, u_size;
	int q_order_type, max_bio_size;
	int ok;
=======
	struct drbd_socket *sock;
	struct p_sizes *p;
	sector_t d_size, u_size;
	int q_order_type;
	unsigned int max_bio_size;
>>>>>>> refs/remotes/origin/master

	if (get_ldev_if_state(mdev, D_NEGOTIATING)) {
		D_ASSERT(mdev->ldev->backing_bdev);
		d_size = drbd_get_max_capacity(mdev->ldev);
<<<<<<< HEAD
		u_size = mdev->ldev->dc.disk_size;
		q_order_type = drbd_queue_order_type(mdev);
		max_bio_size = queue_max_hw_sectors(mdev->ldev->backing_bdev->bd_disk->queue) << 9;
		max_bio_size = min_t(int, max_bio_size, DRBD_MAX_BIO_SIZE);
=======
		rcu_read_lock();
		u_size = rcu_dereference(mdev->ldev->disk_conf)->disk_size;
		rcu_read_unlock();
		q_order_type = drbd_queue_order_type(mdev);
		max_bio_size = queue_max_hw_sectors(mdev->ldev->backing_bdev->bd_disk->queue) << 9;
		max_bio_size = min(max_bio_size, DRBD_MAX_BIO_SIZE);
>>>>>>> refs/remotes/origin/master
		put_ldev(mdev);
	} else {
		d_size = 0;
		u_size = 0;
		q_order_type = QUEUE_ORDERED_NONE;
		max_bio_size = DRBD_MAX_BIO_SIZE; /* ... multiple BIOs per peer_request */
	}

<<<<<<< HEAD
	p.d_size = cpu_to_be64(d_size);
	p.u_size = cpu_to_be64(u_size);
	p.c_size = cpu_to_be64(trigger_reply ? 0 : drbd_get_capacity(mdev->this_bdev));
	p.max_bio_size = cpu_to_be32(max_bio_size);
	p.queue_order_type = cpu_to_be16(q_order_type);
	p.dds_flags = cpu_to_be16(flags);

	ok = drbd_send_cmd(mdev, USE_DATA_SOCKET, P_SIZES,
			   (struct p_header80 *)&p, sizeof(p));
	return ok;
}

/**
 * drbd_send_state() - Sends the drbd state to the peer
 * @mdev:	DRBD device.
 */
int drbd_send_state(struct drbd_conf *mdev)
{
	struct socket *sock;
	struct p_state p;
	int ok = 0;

	/* Grab state lock so we wont send state if we're in the middle
	 * of a cluster wide state change on another thread */
	drbd_state_lock(mdev);

	mutex_lock(&mdev->data.mutex);

	p.state = cpu_to_be32(mdev->state.i); /* Within the send mutex */
	sock = mdev->data.socket;

	if (likely(sock != NULL)) {
		ok = _drbd_send_cmd(mdev, sock, P_STATE,
				    (struct p_header80 *)&p, sizeof(p), 0);
	}

	mutex_unlock(&mdev->data.mutex);

	drbd_state_unlock(mdev);
	return ok;
}

int drbd_send_state_req(struct drbd_conf *mdev,
	union drbd_state mask, union drbd_state val)
{
	struct p_req_state p;

	p.mask    = cpu_to_be32(mask.i);
	p.val     = cpu_to_be32(val.i);

	return drbd_send_cmd(mdev, USE_DATA_SOCKET, P_STATE_CHG_REQ,
			     (struct p_header80 *)&p, sizeof(p));
}

int drbd_send_sr_reply(struct drbd_conf *mdev, enum drbd_state_rv retcode)
{
	struct p_req_state_reply p;

	p.retcode    = cpu_to_be32(retcode);

	return drbd_send_cmd(mdev, USE_META_SOCKET, P_STATE_CHG_REPLY,
			     (struct p_header80 *)&p, sizeof(p));
}

int fill_bitmap_rle_bits(struct drbd_conf *mdev,
	struct p_compressed_bm *p,
	struct bm_xfer_ctx *c)
=======
	sock = &mdev->tconn->data;
	p = drbd_prepare_command(mdev, sock);
	if (!p)
		return -EIO;

	if (mdev->tconn->agreed_pro_version <= 94)
		max_bio_size = min(max_bio_size, DRBD_MAX_SIZE_H80_PACKET);
	else if (mdev->tconn->agreed_pro_version < 100)
		max_bio_size = min(max_bio_size, DRBD_MAX_BIO_SIZE_P95);

	p->d_size = cpu_to_be64(d_size);
	p->u_size = cpu_to_be64(u_size);
	p->c_size = cpu_to_be64(trigger_reply ? 0 : drbd_get_capacity(mdev->this_bdev));
	p->max_bio_size = cpu_to_be32(max_bio_size);
	p->queue_order_type = cpu_to_be16(q_order_type);
	p->dds_flags = cpu_to_be16(flags);
	return drbd_send_command(mdev, sock, P_SIZES, sizeof(*p), NULL, 0);
}

/**
 * drbd_send_current_state() - Sends the drbd state to the peer
 * @mdev:	DRBD device.
 */
int drbd_send_current_state(struct drbd_conf *mdev)
{
	struct drbd_socket *sock;
	struct p_state *p;

	sock = &mdev->tconn->data;
	p = drbd_prepare_command(mdev, sock);
	if (!p)
		return -EIO;
	p->state = cpu_to_be32(mdev->state.i); /* Within the send mutex */
	return drbd_send_command(mdev, sock, P_STATE, sizeof(*p), NULL, 0);
}

/**
 * drbd_send_state() - After a state change, sends the new state to the peer
 * @mdev:      DRBD device.
 * @state:     the state to send, not necessarily the current state.
 *
 * Each state change queues an "after_state_ch" work, which will eventually
 * send the resulting new state to the peer. If more state changes happen
 * between queuing and processing of the after_state_ch work, we still
 * want to send each intermediary state in the order it occurred.
 */
int drbd_send_state(struct drbd_conf *mdev, union drbd_state state)
{
	struct drbd_socket *sock;
	struct p_state *p;

	sock = &mdev->tconn->data;
	p = drbd_prepare_command(mdev, sock);
	if (!p)
		return -EIO;
	p->state = cpu_to_be32(state.i); /* Within the send mutex */
	return drbd_send_command(mdev, sock, P_STATE, sizeof(*p), NULL, 0);
}

int drbd_send_state_req(struct drbd_conf *mdev, union drbd_state mask, union drbd_state val)
{
	struct drbd_socket *sock;
	struct p_req_state *p;

	sock = &mdev->tconn->data;
	p = drbd_prepare_command(mdev, sock);
	if (!p)
		return -EIO;
	p->mask = cpu_to_be32(mask.i);
	p->val = cpu_to_be32(val.i);
	return drbd_send_command(mdev, sock, P_STATE_CHG_REQ, sizeof(*p), NULL, 0);
}

int conn_send_state_req(struct drbd_tconn *tconn, union drbd_state mask, union drbd_state val)
{
	enum drbd_packet cmd;
	struct drbd_socket *sock;
	struct p_req_state *p;

	cmd = tconn->agreed_pro_version < 100 ? P_STATE_CHG_REQ : P_CONN_ST_CHG_REQ;
	sock = &tconn->data;
	p = conn_prepare_command(tconn, sock);
	if (!p)
		return -EIO;
	p->mask = cpu_to_be32(mask.i);
	p->val = cpu_to_be32(val.i);
	return conn_send_command(tconn, sock, cmd, sizeof(*p), NULL, 0);
}

void drbd_send_sr_reply(struct drbd_conf *mdev, enum drbd_state_rv retcode)
{
	struct drbd_socket *sock;
	struct p_req_state_reply *p;

	sock = &mdev->tconn->meta;
	p = drbd_prepare_command(mdev, sock);
	if (p) {
		p->retcode = cpu_to_be32(retcode);
		drbd_send_command(mdev, sock, P_STATE_CHG_REPLY, sizeof(*p), NULL, 0);
	}
}

void conn_send_sr_reply(struct drbd_tconn *tconn, enum drbd_state_rv retcode)
{
	struct drbd_socket *sock;
	struct p_req_state_reply *p;
	enum drbd_packet cmd = tconn->agreed_pro_version < 100 ? P_STATE_CHG_REPLY : P_CONN_ST_CHG_REPLY;

	sock = &tconn->meta;
	p = conn_prepare_command(tconn, sock);
	if (p) {
		p->retcode = cpu_to_be32(retcode);
		conn_send_command(tconn, sock, cmd, sizeof(*p), NULL, 0);
	}
}

static void dcbp_set_code(struct p_compressed_bm *p, enum drbd_bitmap_code code)
{
	BUG_ON(code & ~0xf);
	p->encoding = (p->encoding & ~0xf) | code;
}

static void dcbp_set_start(struct p_compressed_bm *p, int set)
{
	p->encoding = (p->encoding & ~0x80) | (set ? 0x80 : 0);
}

static void dcbp_set_pad_bits(struct p_compressed_bm *p, int n)
{
	BUG_ON(n & ~0x7);
	p->encoding = (p->encoding & (~0x7 << 4)) | (n << 4);
}

int fill_bitmap_rle_bits(struct drbd_conf *mdev,
			 struct p_compressed_bm *p,
			 unsigned int size,
			 struct bm_xfer_ctx *c)
>>>>>>> refs/remotes/origin/master
{
	struct bitstream bs;
	unsigned long plain_bits;
	unsigned long tmp;
	unsigned long rl;
	unsigned len;
	unsigned toggle;
<<<<<<< HEAD
	int bits;

	/* may we use this feature? */
	if ((mdev->sync_conf.use_rle == 0) ||
		(mdev->agreed_pro_version < 90))
			return 0;
=======
	int bits, use_rle;

	/* may we use this feature? */
	rcu_read_lock();
	use_rle = rcu_dereference(mdev->tconn->net_conf)->use_rle;
	rcu_read_unlock();
	if (!use_rle || mdev->tconn->agreed_pro_version < 90)
		return 0;
>>>>>>> refs/remotes/origin/master

	if (c->bit_offset >= c->bm_bits)
		return 0; /* nothing to do. */

	/* use at most thus many bytes */
<<<<<<< HEAD
	bitstream_init(&bs, p->code, BM_PACKET_VLI_BYTES_MAX, 0);
	memset(p->code, 0, BM_PACKET_VLI_BYTES_MAX);
=======
	bitstream_init(&bs, p->code, size, 0);
	memset(p->code, 0, size);
>>>>>>> refs/remotes/origin/master
	/* plain bits covered in this code string */
	plain_bits = 0;

	/* p->encoding & 0x80 stores whether the first run length is set.
	 * bit offset is implicit.
	 * start with toggle == 2 to be able to tell the first iteration */
	toggle = 2;

	/* see how much plain bits we can stuff into one packet
	 * using RLE and VLI. */
	do {
		tmp = (toggle == 0) ? _drbd_bm_find_next_zero(mdev, c->bit_offset)
				    : _drbd_bm_find_next(mdev, c->bit_offset);
		if (tmp == -1UL)
			tmp = c->bm_bits;
		rl = tmp - c->bit_offset;

		if (toggle == 2) { /* first iteration */
			if (rl == 0) {
				/* the first checked bit was set,
				 * store start value, */
<<<<<<< HEAD
				DCBP_set_start(p, 1);
=======
				dcbp_set_start(p, 1);
>>>>>>> refs/remotes/origin/master
				/* but skip encoding of zero run length */
				toggle = !toggle;
				continue;
			}
<<<<<<< HEAD
			DCBP_set_start(p, 0);
=======
			dcbp_set_start(p, 0);
>>>>>>> refs/remotes/origin/master
		}

		/* paranoia: catch zero runlength.
		 * can only happen if bitmap is modified while we scan it. */
		if (rl == 0) {
			dev_err(DEV, "unexpected zero runlength while encoding bitmap "
			    "t:%u bo:%lu\n", toggle, c->bit_offset);
			return -1;
		}

		bits = vli_encode_bits(&bs, rl);
		if (bits == -ENOBUFS) /* buffer full */
			break;
		if (bits <= 0) {
			dev_err(DEV, "error while encoding bitmap: %d\n", bits);
			return 0;
		}

		toggle = !toggle;
		plain_bits += rl;
		c->bit_offset = tmp;
	} while (c->bit_offset < c->bm_bits);

	len = bs.cur.b - p->code + !!bs.cur.bit;

	if (plain_bits < (len << 3)) {
		/* incompressible with this method.
		 * we need to rewind both word and bit position. */
		c->bit_offset -= plain_bits;
		bm_xfer_ctx_bit_to_word_offset(c);
		c->bit_offset = c->word_offset * BITS_PER_LONG;
		return 0;
	}

	/* RLE + VLI was able to compress it just fine.
	 * update c->word_offset. */
	bm_xfer_ctx_bit_to_word_offset(c);

	/* store pad_bits */
<<<<<<< HEAD
	DCBP_set_pad_bits(p, (8 - bs.cur.bit) & 0x7);
=======
	dcbp_set_pad_bits(p, (8 - bs.cur.bit) & 0x7);
>>>>>>> refs/remotes/origin/master

	return len;
}

/**
 * send_bitmap_rle_or_plain
 *
 * Return 0 when done, 1 when another iteration is needed, and a negative error
 * code upon failure.
 */
static int
<<<<<<< HEAD
send_bitmap_rle_or_plain(struct drbd_conf *mdev,
			 struct p_header80 *h, struct bm_xfer_ctx *c)
{
	struct p_compressed_bm *p = (void*)h;
	unsigned long num_words;
	int len;
	int ok;

	len = fill_bitmap_rle_bits(mdev, p, c);

=======
send_bitmap_rle_or_plain(struct drbd_conf *mdev, struct bm_xfer_ctx *c)
{
	struct drbd_socket *sock = &mdev->tconn->data;
	unsigned int header_size = drbd_header_size(mdev->tconn);
	struct p_compressed_bm *p = sock->sbuf + header_size;
	int len, err;

	len = fill_bitmap_rle_bits(mdev, p,
			DRBD_SOCKET_BUFFER_SIZE - header_size - sizeof(*p), c);
>>>>>>> refs/remotes/origin/master
	if (len < 0)
		return -EIO;

	if (len) {
<<<<<<< HEAD
		DCBP_set_code(p, RLE_VLI_Bits);
		ok = _drbd_send_cmd(mdev, mdev->data.socket, P_COMPRESSED_BITMAP, h,
			sizeof(*p) + len, 0);

		c->packets[0]++;
		c->bytes[0] += sizeof(*p) + len;
=======
		dcbp_set_code(p, RLE_VLI_Bits);
		err = __send_command(mdev->tconn, mdev->vnr, sock,
				     P_COMPRESSED_BITMAP, sizeof(*p) + len,
				     NULL, 0);
		c->packets[0]++;
		c->bytes[0] += header_size + sizeof(*p) + len;
>>>>>>> refs/remotes/origin/master

		if (c->bit_offset >= c->bm_bits)
			len = 0; /* DONE */
	} else {
		/* was not compressible.
		 * send a buffer full of plain text bits instead. */
<<<<<<< HEAD
		num_words = min_t(size_t, BM_PACKET_WORDS, c->bm_words - c->word_offset);
		len = num_words * sizeof(long);
		if (len)
			drbd_bm_get_lel(mdev, c->word_offset, num_words, (unsigned long*)h->payload);
		ok = _drbd_send_cmd(mdev, mdev->data.socket, P_BITMAP,
				   h, sizeof(struct p_header80) + len, 0);
=======
		unsigned int data_size;
		unsigned long num_words;
		unsigned long *p = sock->sbuf + header_size;

		data_size = DRBD_SOCKET_BUFFER_SIZE - header_size;
		num_words = min_t(size_t, data_size / sizeof(*p),
				  c->bm_words - c->word_offset);
		len = num_words * sizeof(*p);
		if (len)
			drbd_bm_get_lel(mdev, c->word_offset, num_words, p);
		err = __send_command(mdev->tconn, mdev->vnr, sock, P_BITMAP, len, NULL, 0);
>>>>>>> refs/remotes/origin/master
		c->word_offset += num_words;
		c->bit_offset = c->word_offset * BITS_PER_LONG;

		c->packets[1]++;
<<<<<<< HEAD
		c->bytes[1] += sizeof(struct p_header80) + len;
=======
		c->bytes[1] += header_size + len;
>>>>>>> refs/remotes/origin/master

		if (c->bit_offset > c->bm_bits)
			c->bit_offset = c->bm_bits;
	}
<<<<<<< HEAD
	if (ok) {
=======
	if (!err) {
>>>>>>> refs/remotes/origin/master
		if (len == 0) {
			INFO_bm_xfer_stats(mdev, "send", c);
			return 0;
		} else
			return 1;
	}
	return -EIO;
}

/* See the comment at receive_bitmap() */
<<<<<<< HEAD
int _drbd_send_bitmap(struct drbd_conf *mdev)
{
	struct bm_xfer_ctx c;
	struct p_header80 *p;
	int err;

	ERR_IF(!mdev->bitmap) return false;

	/* maybe we should use some per thread scratch page,
	 * and allocate that during initial device creation? */
	p = (struct p_header80 *) __get_free_page(GFP_NOIO);
	if (!p) {
		dev_err(DEV, "failed to allocate one page buffer in %s\n", __func__);
		return false;
	}
=======
static int _drbd_send_bitmap(struct drbd_conf *mdev)
{
	struct bm_xfer_ctx c;
	int err;

	if (!expect(mdev->bitmap))
		return false;
>>>>>>> refs/remotes/origin/master

	if (get_ldev(mdev)) {
		if (drbd_md_test_flag(mdev->ldev, MDF_FULL_SYNC)) {
			dev_info(DEV, "Writing the whole bitmap, MDF_FullSync was set.\n");
			drbd_bm_set_all(mdev);
			if (drbd_bm_write(mdev)) {
				/* write_bm did fail! Leave full sync flag set in Meta P_DATA
				 * but otherwise process as per normal - need to tell other
				 * side that a full resync is required! */
				dev_err(DEV, "Failed to write bitmap to disk!\n");
			} else {
				drbd_md_clear_flag(mdev, MDF_FULL_SYNC);
				drbd_md_sync(mdev);
			}
		}
		put_ldev(mdev);
	}

	c = (struct bm_xfer_ctx) {
		.bm_bits = drbd_bm_bits(mdev),
		.bm_words = drbd_bm_words(mdev),
	};

	do {
<<<<<<< HEAD
		err = send_bitmap_rle_or_plain(mdev, p, &c);
	} while (err > 0);

	free_page((unsigned long) p);
=======
		err = send_bitmap_rle_or_plain(mdev, &c);
	} while (err > 0);

>>>>>>> refs/remotes/origin/master
	return err == 0;
}

int drbd_send_bitmap(struct drbd_conf *mdev)
{
<<<<<<< HEAD
	int err;

	if (!drbd_get_data_sock(mdev))
		return -1;
	err = !_drbd_send_bitmap(mdev);
	drbd_put_data_sock(mdev);
	return err;
}

int drbd_send_b_ack(struct drbd_conf *mdev, u32 barrier_nr, u32 set_size)
{
	int ok;
	struct p_barrier_ack p;

	p.barrier  = barrier_nr;
	p.set_size = cpu_to_be32(set_size);

	if (mdev->state.conn < C_CONNECTED)
		return false;
	ok = drbd_send_cmd(mdev, USE_META_SOCKET, P_BARRIER_ACK,
			(struct p_header80 *)&p, sizeof(p));
	return ok;
=======
	struct drbd_socket *sock = &mdev->tconn->data;
	int err = -1;

	mutex_lock(&sock->mutex);
	if (sock->socket)
		err = !_drbd_send_bitmap(mdev);
	mutex_unlock(&sock->mutex);
	return err;
}

void drbd_send_b_ack(struct drbd_tconn *tconn, u32 barrier_nr, u32 set_size)
{
	struct drbd_socket *sock;
	struct p_barrier_ack *p;

	if (tconn->cstate < C_WF_REPORT_PARAMS)
		return;

	sock = &tconn->meta;
	p = conn_prepare_command(tconn, sock);
	if (!p)
		return;
	p->barrier = barrier_nr;
	p->set_size = cpu_to_be32(set_size);
	conn_send_command(tconn, sock, P_BARRIER_ACK, sizeof(*p), NULL, 0);
>>>>>>> refs/remotes/origin/master
}

/**
 * _drbd_send_ack() - Sends an ack packet
 * @mdev:	DRBD device.
 * @cmd:	Packet command code.
 * @sector:	sector, needs to be in big endian byte order
 * @blksize:	size in byte, needs to be in big endian byte order
 * @block_id:	Id, big endian byte order
 */
<<<<<<< HEAD
static int _drbd_send_ack(struct drbd_conf *mdev, enum drbd_packets cmd,
			  u64 sector,
			  u32 blksize,
			  u64 block_id)
{
	int ok;
	struct p_block_ack p;

	p.sector   = sector;
	p.block_id = block_id;
	p.blksize  = blksize;
	p.seq_num  = cpu_to_be32(atomic_add_return(1, &mdev->packet_seq));

	if (!mdev->meta.socket || mdev->state.conn < C_CONNECTED)
		return false;
	ok = drbd_send_cmd(mdev, USE_META_SOCKET, cmd,
				(struct p_header80 *)&p, sizeof(p));
	return ok;
=======
static int _drbd_send_ack(struct drbd_conf *mdev, enum drbd_packet cmd,
			  u64 sector, u32 blksize, u64 block_id)
{
	struct drbd_socket *sock;
	struct p_block_ack *p;

	if (mdev->state.conn < C_CONNECTED)
		return -EIO;

	sock = &mdev->tconn->meta;
	p = drbd_prepare_command(mdev, sock);
	if (!p)
		return -EIO;
	p->sector = sector;
	p->block_id = block_id;
	p->blksize = blksize;
	p->seq_num = cpu_to_be32(atomic_inc_return(&mdev->packet_seq));
	return drbd_send_command(mdev, sock, cmd, sizeof(*p), NULL, 0);
>>>>>>> refs/remotes/origin/master
}

/* dp->sector and dp->block_id already/still in network byte order,
 * data_size is payload size according to dp->head,
 * and may need to be corrected for digest size. */
<<<<<<< HEAD
int drbd_send_ack_dp(struct drbd_conf *mdev, enum drbd_packets cmd,
		     struct p_data *dp, int data_size)
{
	data_size -= (mdev->agreed_pro_version >= 87 && mdev->integrity_r_tfm) ?
		crypto_hash_digestsize(mdev->integrity_r_tfm) : 0;
	return _drbd_send_ack(mdev, cmd, dp->sector, cpu_to_be32(data_size),
			      dp->block_id);
}

int drbd_send_ack_rp(struct drbd_conf *mdev, enum drbd_packets cmd,
		     struct p_block_req *rp)
{
	return _drbd_send_ack(mdev, cmd, rp->sector, rp->blksize, rp->block_id);
=======
void drbd_send_ack_dp(struct drbd_conf *mdev, enum drbd_packet cmd,
		      struct p_data *dp, int data_size)
{
	if (mdev->tconn->peer_integrity_tfm)
		data_size -= crypto_hash_digestsize(mdev->tconn->peer_integrity_tfm);
	_drbd_send_ack(mdev, cmd, dp->sector, cpu_to_be32(data_size),
		       dp->block_id);
}

void drbd_send_ack_rp(struct drbd_conf *mdev, enum drbd_packet cmd,
		      struct p_block_req *rp)
{
	_drbd_send_ack(mdev, cmd, rp->sector, rp->blksize, rp->block_id);
>>>>>>> refs/remotes/origin/master
}

/**
 * drbd_send_ack() - Sends an ack packet
<<<<<<< HEAD
 * @mdev:	DRBD device.
 * @cmd:	Packet command code.
 * @e:		Epoch entry.
 */
int drbd_send_ack(struct drbd_conf *mdev,
	enum drbd_packets cmd, struct drbd_epoch_entry *e)
{
	return _drbd_send_ack(mdev, cmd,
			      cpu_to_be64(e->sector),
			      cpu_to_be32(e->size),
			      e->block_id);
=======
 * @mdev:	DRBD device
 * @cmd:	packet command code
 * @peer_req:	peer request
 */
int drbd_send_ack(struct drbd_conf *mdev, enum drbd_packet cmd,
		  struct drbd_peer_request *peer_req)
{
	return _drbd_send_ack(mdev, cmd,
			      cpu_to_be64(peer_req->i.sector),
			      cpu_to_be32(peer_req->i.size),
			      peer_req->block_id);
>>>>>>> refs/remotes/origin/master
}

/* This function misuses the block_id field to signal if the blocks
 * are is sync or not. */
<<<<<<< HEAD
int drbd_send_ack_ex(struct drbd_conf *mdev, enum drbd_packets cmd,
=======
int drbd_send_ack_ex(struct drbd_conf *mdev, enum drbd_packet cmd,
>>>>>>> refs/remotes/origin/master
		     sector_t sector, int blksize, u64 block_id)
{
	return _drbd_send_ack(mdev, cmd,
			      cpu_to_be64(sector),
			      cpu_to_be32(blksize),
			      cpu_to_be64(block_id));
}

int drbd_send_drequest(struct drbd_conf *mdev, int cmd,
		       sector_t sector, int size, u64 block_id)
{
<<<<<<< HEAD
	int ok;
	struct p_block_req p;

	p.sector   = cpu_to_be64(sector);
	p.block_id = block_id;
	p.blksize  = cpu_to_be32(size);

	ok = drbd_send_cmd(mdev, USE_DATA_SOCKET, cmd,
				(struct p_header80 *)&p, sizeof(p));
	return ok;
}

int drbd_send_drequest_csum(struct drbd_conf *mdev,
			    sector_t sector, int size,
			    void *digest, int digest_size,
			    enum drbd_packets cmd)
{
	int ok;
	struct p_block_req p;

	p.sector   = cpu_to_be64(sector);
	p.block_id = BE_DRBD_MAGIC + 0xbeef;
	p.blksize  = cpu_to_be32(size);

	p.head.magic   = BE_DRBD_MAGIC;
	p.head.command = cpu_to_be16(cmd);
	p.head.length  = cpu_to_be16(sizeof(p) - sizeof(struct p_header80) + digest_size);

	mutex_lock(&mdev->data.mutex);

	ok = (sizeof(p) == drbd_send(mdev, mdev->data.socket, &p, sizeof(p), 0));
	ok = ok && (digest_size == drbd_send(mdev, mdev->data.socket, digest, digest_size, 0));

	mutex_unlock(&mdev->data.mutex);

	return ok;
=======
	struct drbd_socket *sock;
	struct p_block_req *p;

	sock = &mdev->tconn->data;
	p = drbd_prepare_command(mdev, sock);
	if (!p)
		return -EIO;
	p->sector = cpu_to_be64(sector);
	p->block_id = block_id;
	p->blksize = cpu_to_be32(size);
	return drbd_send_command(mdev, sock, cmd, sizeof(*p), NULL, 0);
}

int drbd_send_drequest_csum(struct drbd_conf *mdev, sector_t sector, int size,
			    void *digest, int digest_size, enum drbd_packet cmd)
{
	struct drbd_socket *sock;
	struct p_block_req *p;

	/* FIXME: Put the digest into the preallocated socket buffer.  */

	sock = &mdev->tconn->data;
	p = drbd_prepare_command(mdev, sock);
	if (!p)
		return -EIO;
	p->sector = cpu_to_be64(sector);
	p->block_id = ID_SYNCER /* unused */;
	p->blksize = cpu_to_be32(size);
	return drbd_send_command(mdev, sock, cmd, sizeof(*p),
				 digest, digest_size);
>>>>>>> refs/remotes/origin/master
}

int drbd_send_ov_request(struct drbd_conf *mdev, sector_t sector, int size)
{
<<<<<<< HEAD
	int ok;
	struct p_block_req p;

	p.sector   = cpu_to_be64(sector);
	p.block_id = BE_DRBD_MAGIC + 0xbabe;
	p.blksize  = cpu_to_be32(size);

	ok = drbd_send_cmd(mdev, USE_DATA_SOCKET, P_OV_REQUEST,
			   (struct p_header80 *)&p, sizeof(p));
	return ok;
=======
	struct drbd_socket *sock;
	struct p_block_req *p;

	sock = &mdev->tconn->data;
	p = drbd_prepare_command(mdev, sock);
	if (!p)
		return -EIO;
	p->sector = cpu_to_be64(sector);
	p->block_id = ID_SYNCER /* unused */;
	p->blksize = cpu_to_be32(size);
	return drbd_send_command(mdev, sock, P_OV_REQUEST, sizeof(*p), NULL, 0);
>>>>>>> refs/remotes/origin/master
}

/* called on sndtimeo
 * returns false if we should retry,
 * true if we think connection is dead
 */
<<<<<<< HEAD
static int we_should_drop_the_connection(struct drbd_conf *mdev, struct socket *sock)
=======
static int we_should_drop_the_connection(struct drbd_tconn *tconn, struct socket *sock)
>>>>>>> refs/remotes/origin/master
{
	int drop_it;
	/* long elapsed = (long)(jiffies - mdev->last_received); */

<<<<<<< HEAD
	drop_it =   mdev->meta.socket == sock
		|| !mdev->asender.task
		|| get_t_state(&mdev->asender) != Running
		|| mdev->state.conn < C_CONNECTED;
=======
	drop_it =   tconn->meta.socket == sock
		|| !tconn->asender.task
		|| get_t_state(&tconn->asender) != RUNNING
		|| tconn->cstate < C_WF_REPORT_PARAMS;
>>>>>>> refs/remotes/origin/master

	if (drop_it)
		return true;

<<<<<<< HEAD
	drop_it = !--mdev->ko_count;
	if (!drop_it) {
		dev_err(DEV, "[%s/%d] sock_sendmsg time expired, ko = %u\n",
		       current->comm, current->pid, mdev->ko_count);
		request_ping(mdev);
=======
	drop_it = !--tconn->ko_count;
	if (!drop_it) {
		conn_err(tconn, "[%s/%d] sock_sendmsg time expired, ko = %u\n",
			 current->comm, current->pid, tconn->ko_count);
		request_ping(tconn);
>>>>>>> refs/remotes/origin/master
	}

	return drop_it; /* && (mdev->state == R_PRIMARY) */;
}

<<<<<<< HEAD
=======
static void drbd_update_congested(struct drbd_tconn *tconn)
{
	struct sock *sk = tconn->data.socket->sk;
	if (sk->sk_wmem_queued > sk->sk_sndbuf * 4 / 5)
		set_bit(NET_CONGESTED, &tconn->flags);
}

>>>>>>> refs/remotes/origin/master
/* The idea of sendpage seems to be to put some kind of reference
 * to the page into the skb, and to hand it over to the NIC. In
 * this process get_page() gets called.
 *
 * As soon as the page was really sent over the network put_page()
 * gets called by some part of the network layer. [ NIC driver? ]
 *
 * [ get_page() / put_page() increment/decrement the count. If count
 *   reaches 0 the page will be freed. ]
 *
 * This works nicely with pages from FSs.
 * But this means that in protocol A we might signal IO completion too early!
 *
 * In order not to corrupt data during a resync we must make sure
 * that we do not reuse our own buffer pages (EEs) to early, therefore
 * we have the net_ee list.
 *
 * XFS seems to have problems, still, it submits pages with page_count == 0!
 * As a workaround, we disable sendpage on pages
 * with page_count == 0 or PageSlab.
 */
static int _drbd_no_send_page(struct drbd_conf *mdev, struct page *page,
<<<<<<< HEAD
		   int offset, size_t size, unsigned msg_flags)
{
	int sent = drbd_send(mdev, mdev->data.socket, kmap(page) + offset, size, msg_flags);
	kunmap(page);
	if (sent == size)
		mdev->send_cnt += size>>9;
	return sent == size;
=======
			      int offset, size_t size, unsigned msg_flags)
{
	struct socket *socket;
	void *addr;
	int err;

	socket = mdev->tconn->data.socket;
	addr = kmap(page) + offset;
	err = drbd_send_all(mdev->tconn, socket, addr, size, msg_flags);
	kunmap(page);
	if (!err)
		mdev->send_cnt += size >> 9;
	return err;
>>>>>>> refs/remotes/origin/master
}

static int _drbd_send_page(struct drbd_conf *mdev, struct page *page,
		    int offset, size_t size, unsigned msg_flags)
{
<<<<<<< HEAD
	mm_segment_t oldfs = get_fs();
	int sent, ok;
	int len = size;
=======
	struct socket *socket = mdev->tconn->data.socket;
	mm_segment_t oldfs = get_fs();
	int len = size;
	int err = -EIO;
>>>>>>> refs/remotes/origin/master

	/* e.g. XFS meta- & log-data is in slab pages, which have a
	 * page_count of 0 and/or have PageSlab() set.
	 * we cannot use send_page for those, as that does get_page();
	 * put_page(); and would cause either a VM_BUG directly, or
	 * __page_cache_release a page that would actually still be referenced
	 * by someone, leading to some obscure delayed Oops somewhere else. */
	if (disable_sendpage || (page_count(page) < 1) || PageSlab(page))
		return _drbd_no_send_page(mdev, page, offset, size, msg_flags);

	msg_flags |= MSG_NOSIGNAL;
<<<<<<< HEAD
	drbd_update_congested(mdev);
	set_fs(KERNEL_DS);
	do {
		sent = mdev->data.socket->ops->sendpage(mdev->data.socket, page,
							offset, len,
							msg_flags);
		if (sent == -EAGAIN) {
			if (we_should_drop_the_connection(mdev,
							  mdev->data.socket))
				break;
			else
				continue;
		}
		if (sent <= 0) {
			dev_warn(DEV, "%s: size=%d len=%d sent=%d\n",
			     __func__, (int)size, len, sent);
=======
	drbd_update_congested(mdev->tconn);
	set_fs(KERNEL_DS);
	do {
		int sent;

		sent = socket->ops->sendpage(socket, page, offset, len, msg_flags);
		if (sent <= 0) {
			if (sent == -EAGAIN) {
				if (we_should_drop_the_connection(mdev->tconn, socket))
					break;
				continue;
			}
			dev_warn(DEV, "%s: size=%d len=%d sent=%d\n",
			     __func__, (int)size, len, sent);
			if (sent < 0)
				err = sent;
>>>>>>> refs/remotes/origin/master
			break;
		}
		len    -= sent;
		offset += sent;
	} while (len > 0 /* THINK && mdev->cstate >= C_CONNECTED*/);
	set_fs(oldfs);
<<<<<<< HEAD
	clear_bit(NET_CONGESTED, &mdev->flags);

	ok = (len == 0);
	if (likely(ok))
		mdev->send_cnt += size>>9;
	return ok;
=======
	clear_bit(NET_CONGESTED, &mdev->tconn->flags);

	if (len == 0) {
		err = 0;
		mdev->send_cnt += size >> 9;
	}
	return err;
>>>>>>> refs/remotes/origin/master
}

static int _drbd_send_bio(struct drbd_conf *mdev, struct bio *bio)
{
	struct bio_vec *bvec;
	int i;
	/* hint all but last page with MSG_MORE */
<<<<<<< HEAD
	__bio_for_each_segment(bvec, bio, i, 0) {
		if (!_drbd_no_send_page(mdev, bvec->bv_page,
				     bvec->bv_offset, bvec->bv_len,
				     i == bio->bi_vcnt -1 ? 0 : MSG_MORE))
			return 0;
	}
	return 1;
=======
	bio_for_each_segment(bvec, bio, i) {
		int err;

		err = _drbd_no_send_page(mdev, bvec->bv_page,
					 bvec->bv_offset, bvec->bv_len,
					 i == bio->bi_vcnt - 1 ? 0 : MSG_MORE);
		if (err)
			return err;
	}
	return 0;
>>>>>>> refs/remotes/origin/master
}

static int _drbd_send_zc_bio(struct drbd_conf *mdev, struct bio *bio)
{
	struct bio_vec *bvec;
	int i;
	/* hint all but last page with MSG_MORE */
<<<<<<< HEAD
	__bio_for_each_segment(bvec, bio, i, 0) {
		if (!_drbd_send_page(mdev, bvec->bv_page,
				     bvec->bv_offset, bvec->bv_len,
				     i == bio->bi_vcnt -1 ? 0 : MSG_MORE))
			return 0;
	}
	return 1;
}

static int _drbd_send_zc_ee(struct drbd_conf *mdev, struct drbd_epoch_entry *e)
{
	struct page *page = e->pages;
	unsigned len = e->size;
	/* hint all but last page with MSG_MORE */
	page_chain_for_each(page) {
		unsigned l = min_t(unsigned, len, PAGE_SIZE);
		if (!_drbd_send_page(mdev, page, 0, l,
				page_chain_next(page) ? MSG_MORE : 0))
			return 0;
		len -= l;
	}
	return 1;
=======
	bio_for_each_segment(bvec, bio, i) {
		int err;

		err = _drbd_send_page(mdev, bvec->bv_page,
				      bvec->bv_offset, bvec->bv_len,
				      i == bio->bi_vcnt - 1 ? 0 : MSG_MORE);
		if (err)
			return err;
	}
	return 0;
}

static int _drbd_send_zc_ee(struct drbd_conf *mdev,
			    struct drbd_peer_request *peer_req)
{
	struct page *page = peer_req->pages;
	unsigned len = peer_req->i.size;
	int err;

	/* hint all but last page with MSG_MORE */
	page_chain_for_each(page) {
		unsigned l = min_t(unsigned, len, PAGE_SIZE);

		err = _drbd_send_page(mdev, page, 0, l,
				      page_chain_next(page) ? MSG_MORE : 0);
		if (err)
			return err;
		len -= l;
	}
	return 0;
>>>>>>> refs/remotes/origin/master
}

static u32 bio_flags_to_wire(struct drbd_conf *mdev, unsigned long bi_rw)
{
<<<<<<< HEAD
	if (mdev->agreed_pro_version >= 95)
=======
	if (mdev->tconn->agreed_pro_version >= 95)
>>>>>>> refs/remotes/origin/master
		return  (bi_rw & REQ_SYNC ? DP_RW_SYNC : 0) |
			(bi_rw & REQ_FUA ? DP_FUA : 0) |
			(bi_rw & REQ_FLUSH ? DP_FLUSH : 0) |
			(bi_rw & REQ_DISCARD ? DP_DISCARD : 0);
	else
		return bi_rw & REQ_SYNC ? DP_RW_SYNC : 0;
}

/* Used to send write requests
 * R_PRIMARY -> Peer	(P_DATA)
 */
int drbd_send_dblock(struct drbd_conf *mdev, struct drbd_request *req)
{
<<<<<<< HEAD
	int ok = 1;
	struct p_data p;
	unsigned int dp_flags = 0;
	void *dgb;
	int dgs;

	if (!drbd_get_data_sock(mdev))
		return 0;

	dgs = (mdev->agreed_pro_version >= 87 && mdev->integrity_w_tfm) ?
		crypto_hash_digestsize(mdev->integrity_w_tfm) : 0;

	if (req->size <= DRBD_MAX_SIZE_H80_PACKET) {
		p.head.h80.magic   = BE_DRBD_MAGIC;
		p.head.h80.command = cpu_to_be16(P_DATA);
		p.head.h80.length  =
			cpu_to_be16(sizeof(p) - sizeof(union p_header) + dgs + req->size);
	} else {
		p.head.h95.magic   = BE_DRBD_MAGIC_BIG;
		p.head.h95.command = cpu_to_be16(P_DATA);
		p.head.h95.length  =
			cpu_to_be32(sizeof(p) - sizeof(union p_header) + dgs + req->size);
	}

	p.sector   = cpu_to_be64(req->sector);
	p.block_id = (unsigned long)req;
	p.seq_num  = cpu_to_be32(req->seq_num =
				 atomic_add_return(1, &mdev->packet_seq));

	dp_flags = bio_flags_to_wire(mdev, req->master_bio->bi_rw);

	if (mdev->state.conn >= C_SYNC_SOURCE &&
	    mdev->state.conn <= C_PAUSED_SYNC_T)
		dp_flags |= DP_MAY_SET_IN_SYNC;

	p.dp_flags = cpu_to_be32(dp_flags);
	set_bit(UNPLUG_REMOTE, &mdev->flags);
	ok = (sizeof(p) ==
		drbd_send(mdev, mdev->data.socket, &p, sizeof(p), dgs ? MSG_MORE : 0));
	if (ok && dgs) {
		dgb = mdev->int_dig_out;
		drbd_csum_bio(mdev, mdev->integrity_w_tfm, req->master_bio, dgb);
		ok = dgs == drbd_send(mdev, mdev->data.socket, dgb, dgs, 0);
	}
	if (ok) {
=======
	struct drbd_socket *sock;
	struct p_data *p;
	unsigned int dp_flags = 0;
	int dgs;
	int err;

	sock = &mdev->tconn->data;
	p = drbd_prepare_command(mdev, sock);
	dgs = mdev->tconn->integrity_tfm ? crypto_hash_digestsize(mdev->tconn->integrity_tfm) : 0;

	if (!p)
		return -EIO;
	p->sector = cpu_to_be64(req->i.sector);
	p->block_id = (unsigned long)req;
	p->seq_num = cpu_to_be32(atomic_inc_return(&mdev->packet_seq));
	dp_flags = bio_flags_to_wire(mdev, req->master_bio->bi_rw);
	if (mdev->state.conn >= C_SYNC_SOURCE &&
	    mdev->state.conn <= C_PAUSED_SYNC_T)
		dp_flags |= DP_MAY_SET_IN_SYNC;
	if (mdev->tconn->agreed_pro_version >= 100) {
		if (req->rq_state & RQ_EXP_RECEIVE_ACK)
			dp_flags |= DP_SEND_RECEIVE_ACK;
		if (req->rq_state & RQ_EXP_WRITE_ACK)
			dp_flags |= DP_SEND_WRITE_ACK;
	}
	p->dp_flags = cpu_to_be32(dp_flags);
	if (dgs)
		drbd_csum_bio(mdev, mdev->tconn->integrity_tfm, req->master_bio, p + 1);
	err = __send_command(mdev->tconn, mdev->vnr, sock, P_DATA, sizeof(*p) + dgs, NULL, req->i.size);
	if (!err) {
>>>>>>> refs/remotes/origin/master
		/* For protocol A, we have to memcpy the payload into
		 * socket buffers, as we may complete right away
		 * as soon as we handed it over to tcp, at which point the data
		 * pages may become invalid.
		 *
		 * For data-integrity enabled, we copy it as well, so we can be
		 * sure that even if the bio pages may still be modified, it
		 * won't change the data on the wire, thus if the digest checks
		 * out ok after sending on this side, but does not fit on the
		 * receiving side, we sure have detected corruption elsewhere.
		 */
<<<<<<< HEAD
		if (mdev->net_conf->wire_protocol == DRBD_PROT_A || dgs)
			ok = _drbd_send_bio(mdev, req->master_bio);
		else
			ok = _drbd_send_zc_bio(mdev, req->master_bio);
=======
		if (!(req->rq_state & (RQ_EXP_RECEIVE_ACK | RQ_EXP_WRITE_ACK)) || dgs)
			err = _drbd_send_bio(mdev, req->master_bio);
		else
			err = _drbd_send_zc_bio(mdev, req->master_bio);
>>>>>>> refs/remotes/origin/master

		/* double check digest, sometimes buffers have been modified in flight. */
		if (dgs > 0 && dgs <= 64) {
			/* 64 byte, 512 bit, is the largest digest size
			 * currently supported in kernel crypto. */
			unsigned char digest[64];
<<<<<<< HEAD
			drbd_csum_bio(mdev, mdev->integrity_w_tfm, req->master_bio, digest);
			if (memcmp(mdev->int_dig_out, digest, dgs)) {
				dev_warn(DEV,
					"Digest mismatch, buffer modified by upper layers during write: %llus +%u\n",
					(unsigned long long)req->sector, req->size);
=======
			drbd_csum_bio(mdev, mdev->tconn->integrity_tfm, req->master_bio, digest);
			if (memcmp(p + 1, digest, dgs)) {
				dev_warn(DEV,
					"Digest mismatch, buffer modified by upper layers during write: %llus +%u\n",
					(unsigned long long)req->i.sector, req->i.size);
>>>>>>> refs/remotes/origin/master
			}
		} /* else if (dgs > 64) {
		     ... Be noisy about digest too large ...
		} */
	}
<<<<<<< HEAD

	drbd_put_data_sock(mdev);

	return ok;
=======
	mutex_unlock(&sock->mutex);  /* locked by drbd_prepare_command() */

	return err;
>>>>>>> refs/remotes/origin/master
}

/* answer packet, used to send data back for read requests:
 *  Peer       -> (diskless) R_PRIMARY   (P_DATA_REPLY)
 *  C_SYNC_SOURCE -> C_SYNC_TARGET         (P_RS_DATA_REPLY)
 */
<<<<<<< HEAD
int drbd_send_block(struct drbd_conf *mdev, enum drbd_packets cmd,
		    struct drbd_epoch_entry *e)
{
	int ok;
	struct p_data p;
	void *dgb;
	int dgs;

	dgs = (mdev->agreed_pro_version >= 87 && mdev->integrity_w_tfm) ?
		crypto_hash_digestsize(mdev->integrity_w_tfm) : 0;

	if (e->size <= DRBD_MAX_SIZE_H80_PACKET) {
		p.head.h80.magic   = BE_DRBD_MAGIC;
		p.head.h80.command = cpu_to_be16(cmd);
		p.head.h80.length  =
			cpu_to_be16(sizeof(p) - sizeof(struct p_header80) + dgs + e->size);
	} else {
		p.head.h95.magic   = BE_DRBD_MAGIC_BIG;
		p.head.h95.command = cpu_to_be16(cmd);
		p.head.h95.length  =
			cpu_to_be32(sizeof(p) - sizeof(struct p_header80) + dgs + e->size);
	}

	p.sector   = cpu_to_be64(e->sector);
	p.block_id = e->block_id;
	/* p.seq_num  = 0;    No sequence numbers here.. */

	/* Only called by our kernel thread.
	 * This one may be interrupted by DRBD_SIG and/or DRBD_SIGKILL
	 * in response to admin command or module unload.
	 */
	if (!drbd_get_data_sock(mdev))
		return 0;

	ok = sizeof(p) == drbd_send(mdev, mdev->data.socket, &p, sizeof(p), dgs ? MSG_MORE : 0);
	if (ok && dgs) {
		dgb = mdev->int_dig_out;
		drbd_csum_ee(mdev, mdev->integrity_w_tfm, e, dgb);
		ok = dgs == drbd_send(mdev, mdev->data.socket, dgb, dgs, 0);
	}
	if (ok)
		ok = _drbd_send_zc_ee(mdev, e);

	drbd_put_data_sock(mdev);

	return ok;
}

int drbd_send_oos(struct drbd_conf *mdev, struct drbd_request *req)
{
	struct p_block_desc p;

	p.sector  = cpu_to_be64(req->sector);
	p.blksize = cpu_to_be32(req->size);

	return drbd_send_cmd(mdev, USE_DATA_SOCKET, P_OUT_OF_SYNC, &p.head, sizeof(p));
=======
int drbd_send_block(struct drbd_conf *mdev, enum drbd_packet cmd,
		    struct drbd_peer_request *peer_req)
{
	struct drbd_socket *sock;
	struct p_data *p;
	int err;
	int dgs;

	sock = &mdev->tconn->data;
	p = drbd_prepare_command(mdev, sock);

	dgs = mdev->tconn->integrity_tfm ? crypto_hash_digestsize(mdev->tconn->integrity_tfm) : 0;

	if (!p)
		return -EIO;
	p->sector = cpu_to_be64(peer_req->i.sector);
	p->block_id = peer_req->block_id;
	p->seq_num = 0;  /* unused */
	p->dp_flags = 0;
	if (dgs)
		drbd_csum_ee(mdev, mdev->tconn->integrity_tfm, peer_req, p + 1);
	err = __send_command(mdev->tconn, mdev->vnr, sock, cmd, sizeof(*p) + dgs, NULL, peer_req->i.size);
	if (!err)
		err = _drbd_send_zc_ee(mdev, peer_req);
	mutex_unlock(&sock->mutex);  /* locked by drbd_prepare_command() */

	return err;
}

int drbd_send_out_of_sync(struct drbd_conf *mdev, struct drbd_request *req)
{
	struct drbd_socket *sock;
	struct p_block_desc *p;

	sock = &mdev->tconn->data;
	p = drbd_prepare_command(mdev, sock);
	if (!p)
		return -EIO;
	p->sector = cpu_to_be64(req->i.sector);
	p->blksize = cpu_to_be32(req->i.size);
	return drbd_send_command(mdev, sock, P_OUT_OF_SYNC, sizeof(*p), NULL, 0);
>>>>>>> refs/remotes/origin/master
}

/*
  drbd_send distinguishes two cases:

  Packets sent via the data socket "sock"
  and packets sent via the meta data socket "msock"

		    sock                      msock
  -----------------+-------------------------+------------------------------
  timeout           conf.timeout / 2          conf.timeout / 2
  timeout action    send a ping via msock     Abort communication
					      and close all sockets
*/

/*
 * you must have down()ed the appropriate [m]sock_mutex elsewhere!
 */
<<<<<<< HEAD
int drbd_send(struct drbd_conf *mdev, struct socket *sock,
=======
int drbd_send(struct drbd_tconn *tconn, struct socket *sock,
>>>>>>> refs/remotes/origin/master
	      void *buf, size_t size, unsigned msg_flags)
{
	struct kvec iov;
	struct msghdr msg;
	int rv, sent = 0;

	if (!sock)
<<<<<<< HEAD
		return -1000;
=======
		return -EBADR;
>>>>>>> refs/remotes/origin/master

	/* THINK  if (signal_pending) return ... ? */

	iov.iov_base = buf;
	iov.iov_len  = size;

	msg.msg_name       = NULL;
	msg.msg_namelen    = 0;
	msg.msg_control    = NULL;
	msg.msg_controllen = 0;
	msg.msg_flags      = msg_flags | MSG_NOSIGNAL;

<<<<<<< HEAD
	if (sock == mdev->data.socket) {
		mdev->ko_count = mdev->net_conf->ko_count;
		drbd_update_congested(mdev);
=======
	if (sock == tconn->data.socket) {
		rcu_read_lock();
		tconn->ko_count = rcu_dereference(tconn->net_conf)->ko_count;
		rcu_read_unlock();
		drbd_update_congested(tconn);
>>>>>>> refs/remotes/origin/master
	}
	do {
		/* STRANGE
		 * tcp_sendmsg does _not_ use its size parameter at all ?
		 *
		 * -EAGAIN on timeout, -EINTR on signal.
		 */
/* THINK
 * do we need to block DRBD_SIG if sock == &meta.socket ??
 * otherwise wake_asender() might interrupt some send_*Ack !
 */
		rv = kernel_sendmsg(sock, &msg, &iov, 1, size);
		if (rv == -EAGAIN) {
<<<<<<< HEAD
			if (we_should_drop_the_connection(mdev, sock))
=======
			if (we_should_drop_the_connection(tconn, sock))
>>>>>>> refs/remotes/origin/master
				break;
			else
				continue;
		}
<<<<<<< HEAD
		D_ASSERT(rv != 0);
=======
>>>>>>> refs/remotes/origin/master
		if (rv == -EINTR) {
			flush_signals(current);
			rv = 0;
		}
		if (rv < 0)
			break;
		sent += rv;
		iov.iov_base += rv;
		iov.iov_len  -= rv;
	} while (sent < size);

<<<<<<< HEAD
	if (sock == mdev->data.socket)
		clear_bit(NET_CONGESTED, &mdev->flags);

	if (rv <= 0) {
		if (rv != -EAGAIN) {
			dev_err(DEV, "%s_sendmsg returned %d\n",
			    sock == mdev->meta.socket ? "msock" : "sock",
			    rv);
			drbd_force_state(mdev, NS(conn, C_BROKEN_PIPE));
		} else
			drbd_force_state(mdev, NS(conn, C_TIMEOUT));
=======
	if (sock == tconn->data.socket)
		clear_bit(NET_CONGESTED, &tconn->flags);

	if (rv <= 0) {
		if (rv != -EAGAIN) {
			conn_err(tconn, "%s_sendmsg returned %d\n",
				 sock == tconn->meta.socket ? "msock" : "sock",
				 rv);
			conn_request_state(tconn, NS(conn, C_BROKEN_PIPE), CS_HARD);
		} else
			conn_request_state(tconn, NS(conn, C_TIMEOUT), CS_HARD);
>>>>>>> refs/remotes/origin/master
	}

	return sent;
}

<<<<<<< HEAD
=======
/**
 * drbd_send_all  -  Send an entire buffer
 *
 * Returns 0 upon success and a negative error value otherwise.
 */
int drbd_send_all(struct drbd_tconn *tconn, struct socket *sock, void *buffer,
		  size_t size, unsigned msg_flags)
{
	int err;

	err = drbd_send(tconn, sock, buffer, size, msg_flags);
	if (err < 0)
		return err;
	if (err != size)
		return -EIO;
	return 0;
}

>>>>>>> refs/remotes/origin/master
static int drbd_open(struct block_device *bdev, fmode_t mode)
{
	struct drbd_conf *mdev = bdev->bd_disk->private_data;
	unsigned long flags;
	int rv = 0;

	mutex_lock(&drbd_main_mutex);
<<<<<<< HEAD
	spin_lock_irqsave(&mdev->req_lock, flags);
=======
	spin_lock_irqsave(&mdev->tconn->req_lock, flags);
>>>>>>> refs/remotes/origin/master
	/* to have a stable mdev->state.role
	 * and no race with updating open_cnt */

	if (mdev->state.role != R_PRIMARY) {
		if (mode & FMODE_WRITE)
			rv = -EROFS;
		else if (!allow_oos)
			rv = -EMEDIUMTYPE;
	}

	if (!rv)
		mdev->open_cnt++;
<<<<<<< HEAD
	spin_unlock_irqrestore(&mdev->req_lock, flags);
=======
	spin_unlock_irqrestore(&mdev->tconn->req_lock, flags);
>>>>>>> refs/remotes/origin/master
	mutex_unlock(&drbd_main_mutex);

	return rv;
}

<<<<<<< HEAD
static int drbd_release(struct gendisk *gd, fmode_t mode)
=======
static void drbd_release(struct gendisk *gd, fmode_t mode)
>>>>>>> refs/remotes/origin/master
{
	struct drbd_conf *mdev = gd->private_data;
	mutex_lock(&drbd_main_mutex);
	mdev->open_cnt--;
	mutex_unlock(&drbd_main_mutex);
<<<<<<< HEAD
	return 0;
=======
>>>>>>> refs/remotes/origin/master
}

static void drbd_set_defaults(struct drbd_conf *mdev)
{
<<<<<<< HEAD
	/* This way we get a compile error when sync_conf grows,
	   and we forgot to initialize it here */
	mdev->sync_conf = (struct syncer_conf) {
		/* .rate = */		DRBD_RATE_DEF,
		/* .after = */		DRBD_AFTER_DEF,
		/* .al_extents = */	DRBD_AL_EXTENTS_DEF,
		/* .verify_alg = */	{}, 0,
		/* .cpu_mask = */	{}, 0,
		/* .csums_alg = */	{}, 0,
		/* .use_rle = */	0,
		/* .on_no_data = */	DRBD_ON_NO_DATA_DEF,
		/* .c_plan_ahead = */	DRBD_C_PLAN_AHEAD_DEF,
		/* .c_delay_target = */	DRBD_C_DELAY_TARGET_DEF,
		/* .c_fill_target = */	DRBD_C_FILL_TARGET_DEF,
		/* .c_max_rate = */	DRBD_C_MAX_RATE_DEF,
		/* .c_min_rate = */	DRBD_C_MIN_RATE_DEF
	};

	/* Have to use that way, because the layout differs between
	   big endian and little endian */
	mdev->state = (union drbd_state) {
=======
	/* Beware! The actual layout differs
	 * between big endian and little endian */
	mdev->state = (union drbd_dev_state) {
>>>>>>> refs/remotes/origin/master
		{ .role = R_SECONDARY,
		  .peer = R_UNKNOWN,
		  .conn = C_STANDALONE,
		  .disk = D_DISKLESS,
		  .pdsk = D_UNKNOWN,
<<<<<<< HEAD
		  .susp = 0,
		  .susp_nod = 0,
		  .susp_fen = 0
=======
>>>>>>> refs/remotes/origin/master
		} };
}

void drbd_init_set_defaults(struct drbd_conf *mdev)
{
	/* the memset(,0,) did most of this.
	 * note: only assignments, no allocation in here */

	drbd_set_defaults(mdev);

	atomic_set(&mdev->ap_bio_cnt, 0);
	atomic_set(&mdev->ap_pending_cnt, 0);
	atomic_set(&mdev->rs_pending_cnt, 0);
	atomic_set(&mdev->unacked_cnt, 0);
	atomic_set(&mdev->local_cnt, 0);
<<<<<<< HEAD
	atomic_set(&mdev->net_cnt, 0);
	atomic_set(&mdev->packet_seq, 0);
	atomic_set(&mdev->pp_in_use, 0);
=======
>>>>>>> refs/remotes/origin/master
	atomic_set(&mdev->pp_in_use_by_net, 0);
	atomic_set(&mdev->rs_sect_in, 0);
	atomic_set(&mdev->rs_sect_ev, 0);
	atomic_set(&mdev->ap_in_flight, 0);
<<<<<<< HEAD

	mutex_init(&mdev->md_io_mutex);
	mutex_init(&mdev->data.mutex);
	mutex_init(&mdev->meta.mutex);
	sema_init(&mdev->data.work.s, 0);
	sema_init(&mdev->meta.work.s, 0);
	mutex_init(&mdev->state_mutex);

	spin_lock_init(&mdev->data.work.q_lock);
	spin_lock_init(&mdev->meta.work.q_lock);

	spin_lock_init(&mdev->al_lock);
	spin_lock_init(&mdev->req_lock);
	spin_lock_init(&mdev->peer_seq_lock);
	spin_lock_init(&mdev->epoch_lock);
=======
	atomic_set(&mdev->md_io_in_use, 0);

	mutex_init(&mdev->own_state_mutex);
	mdev->state_mutex = &mdev->own_state_mutex;

	spin_lock_init(&mdev->al_lock);
	spin_lock_init(&mdev->peer_seq_lock);
>>>>>>> refs/remotes/origin/master

	INIT_LIST_HEAD(&mdev->active_ee);
	INIT_LIST_HEAD(&mdev->sync_ee);
	INIT_LIST_HEAD(&mdev->done_ee);
	INIT_LIST_HEAD(&mdev->read_ee);
	INIT_LIST_HEAD(&mdev->net_ee);
	INIT_LIST_HEAD(&mdev->resync_reads);
<<<<<<< HEAD
	INIT_LIST_HEAD(&mdev->data.work.q);
	INIT_LIST_HEAD(&mdev->meta.work.q);
=======
>>>>>>> refs/remotes/origin/master
	INIT_LIST_HEAD(&mdev->resync_work.list);
	INIT_LIST_HEAD(&mdev->unplug_work.list);
	INIT_LIST_HEAD(&mdev->go_diskless.list);
	INIT_LIST_HEAD(&mdev->md_sync_work.list);
	INIT_LIST_HEAD(&mdev->start_resync_work.list);
	INIT_LIST_HEAD(&mdev->bm_io_work.w.list);

	mdev->resync_work.cb  = w_resync_timer;
	mdev->unplug_work.cb  = w_send_write_hint;
	mdev->go_diskless.cb  = w_go_diskless;
	mdev->md_sync_work.cb = w_md_sync;
	mdev->bm_io_work.w.cb = w_bitmap_io;
	mdev->start_resync_work.cb = w_start_resync;
<<<<<<< HEAD
=======

	mdev->resync_work.mdev  = mdev;
	mdev->unplug_work.mdev  = mdev;
	mdev->go_diskless.mdev  = mdev;
	mdev->md_sync_work.mdev = mdev;
	mdev->bm_io_work.w.mdev = mdev;
	mdev->start_resync_work.mdev = mdev;

>>>>>>> refs/remotes/origin/master
	init_timer(&mdev->resync_timer);
	init_timer(&mdev->md_sync_timer);
	init_timer(&mdev->start_resync_timer);
	init_timer(&mdev->request_timer);
	mdev->resync_timer.function = resync_timer_fn;
	mdev->resync_timer.data = (unsigned long) mdev;
	mdev->md_sync_timer.function = md_sync_timer_fn;
	mdev->md_sync_timer.data = (unsigned long) mdev;
	mdev->start_resync_timer.function = start_resync_timer_fn;
	mdev->start_resync_timer.data = (unsigned long) mdev;
	mdev->request_timer.function = request_timer_fn;
	mdev->request_timer.data = (unsigned long) mdev;

	init_waitqueue_head(&mdev->misc_wait);
	init_waitqueue_head(&mdev->state_wait);
<<<<<<< HEAD
	init_waitqueue_head(&mdev->net_cnt_wait);
=======
>>>>>>> refs/remotes/origin/master
	init_waitqueue_head(&mdev->ee_wait);
	init_waitqueue_head(&mdev->al_wait);
	init_waitqueue_head(&mdev->seq_wait);

<<<<<<< HEAD
	drbd_thread_init(mdev, &mdev->receiver, drbdd_init);
	drbd_thread_init(mdev, &mdev->worker, drbd_worker);
	drbd_thread_init(mdev, &mdev->asender, drbd_asender);

	mdev->agreed_pro_version = PRO_VERSION_MAX;
	mdev->write_ordering = WO_bdev_flush;
=======
>>>>>>> refs/remotes/origin/master
	mdev->resync_wenr = LC_FREE;
	mdev->peer_max_bio_size = DRBD_MAX_BIO_SIZE_SAFE;
	mdev->local_max_bio_size = DRBD_MAX_BIO_SIZE_SAFE;
}

void drbd_mdev_cleanup(struct drbd_conf *mdev)
{
	int i;
<<<<<<< HEAD
	if (mdev->receiver.t_state != None)
		dev_err(DEV, "ASSERT FAILED: receiver t_state == %d expected 0.\n",
				mdev->receiver.t_state);

	/* no need to lock it, I'm the only thread alive */
	if (atomic_read(&mdev->current_epoch->epoch_size) !=  0)
		dev_err(DEV, "epoch_size:%d\n", atomic_read(&mdev->current_epoch->epoch_size));
=======
	if (mdev->tconn->receiver.t_state != NONE)
		dev_err(DEV, "ASSERT FAILED: receiver t_state == %d expected 0.\n",
				mdev->tconn->receiver.t_state);

>>>>>>> refs/remotes/origin/master
	mdev->al_writ_cnt  =
	mdev->bm_writ_cnt  =
	mdev->read_cnt     =
	mdev->recv_cnt     =
	mdev->send_cnt     =
	mdev->writ_cnt     =
	mdev->p_size       =
	mdev->rs_start     =
	mdev->rs_total     =
	mdev->rs_failed    = 0;
	mdev->rs_last_events = 0;
	mdev->rs_last_sect_ev = 0;
	for (i = 0; i < DRBD_SYNC_MARKS; i++) {
		mdev->rs_mark_left[i] = 0;
		mdev->rs_mark_time[i] = 0;
	}
<<<<<<< HEAD
	D_ASSERT(mdev->net_conf == NULL);
=======
	D_ASSERT(mdev->tconn->net_conf == NULL);
>>>>>>> refs/remotes/origin/master

	drbd_set_my_capacity(mdev, 0);
	if (mdev->bitmap) {
		/* maybe never allocated. */
		drbd_bm_resize(mdev, 0, 1);
		drbd_bm_cleanup(mdev);
	}

<<<<<<< HEAD
	drbd_free_resources(mdev);
	clear_bit(AL_SUSPENDED, &mdev->flags);

	/*
	 * currently we drbd_init_ee only on module load, so
	 * we may do drbd_release_ee only on module unload!
	 */
=======
	drbd_free_bc(mdev->ldev);
	mdev->ldev = NULL;

	clear_bit(AL_SUSPENDED, &mdev->flags);

>>>>>>> refs/remotes/origin/master
	D_ASSERT(list_empty(&mdev->active_ee));
	D_ASSERT(list_empty(&mdev->sync_ee));
	D_ASSERT(list_empty(&mdev->done_ee));
	D_ASSERT(list_empty(&mdev->read_ee));
	D_ASSERT(list_empty(&mdev->net_ee));
	D_ASSERT(list_empty(&mdev->resync_reads));
<<<<<<< HEAD
	D_ASSERT(list_empty(&mdev->data.work.q));
	D_ASSERT(list_empty(&mdev->meta.work.q));
=======
	D_ASSERT(list_empty(&mdev->tconn->sender_work.q));
>>>>>>> refs/remotes/origin/master
	D_ASSERT(list_empty(&mdev->resync_work.list));
	D_ASSERT(list_empty(&mdev->unplug_work.list));
	D_ASSERT(list_empty(&mdev->go_diskless.list));

	drbd_set_defaults(mdev);
}


static void drbd_destroy_mempools(void)
{
	struct page *page;

	while (drbd_pp_pool) {
		page = drbd_pp_pool;
		drbd_pp_pool = (struct page *)page_private(page);
		__free_page(page);
		drbd_pp_vacant--;
	}

	/* D_ASSERT(atomic_read(&drbd_pp_vacant)==0); */

<<<<<<< HEAD
=======
	if (drbd_md_io_bio_set)
		bioset_free(drbd_md_io_bio_set);
	if (drbd_md_io_page_pool)
		mempool_destroy(drbd_md_io_page_pool);
>>>>>>> refs/remotes/origin/master
	if (drbd_ee_mempool)
		mempool_destroy(drbd_ee_mempool);
	if (drbd_request_mempool)
		mempool_destroy(drbd_request_mempool);
	if (drbd_ee_cache)
		kmem_cache_destroy(drbd_ee_cache);
	if (drbd_request_cache)
		kmem_cache_destroy(drbd_request_cache);
	if (drbd_bm_ext_cache)
		kmem_cache_destroy(drbd_bm_ext_cache);
	if (drbd_al_ext_cache)
		kmem_cache_destroy(drbd_al_ext_cache);

<<<<<<< HEAD
=======
	drbd_md_io_bio_set   = NULL;
	drbd_md_io_page_pool = NULL;
>>>>>>> refs/remotes/origin/master
	drbd_ee_mempool      = NULL;
	drbd_request_mempool = NULL;
	drbd_ee_cache        = NULL;
	drbd_request_cache   = NULL;
	drbd_bm_ext_cache    = NULL;
	drbd_al_ext_cache    = NULL;

	return;
}

static int drbd_create_mempools(void)
{
	struct page *page;
	const int number = (DRBD_MAX_BIO_SIZE/PAGE_SIZE) * minor_count;
	int i;

	/* prepare our caches and mempools */
	drbd_request_mempool = NULL;
	drbd_ee_cache        = NULL;
	drbd_request_cache   = NULL;
	drbd_bm_ext_cache    = NULL;
	drbd_al_ext_cache    = NULL;
	drbd_pp_pool         = NULL;
<<<<<<< HEAD
=======
	drbd_md_io_page_pool = NULL;
	drbd_md_io_bio_set   = NULL;
>>>>>>> refs/remotes/origin/master

	/* caches */
	drbd_request_cache = kmem_cache_create(
		"drbd_req", sizeof(struct drbd_request), 0, 0, NULL);
	if (drbd_request_cache == NULL)
		goto Enomem;

	drbd_ee_cache = kmem_cache_create(
<<<<<<< HEAD
		"drbd_ee", sizeof(struct drbd_epoch_entry), 0, 0, NULL);
=======
		"drbd_ee", sizeof(struct drbd_peer_request), 0, 0, NULL);
>>>>>>> refs/remotes/origin/master
	if (drbd_ee_cache == NULL)
		goto Enomem;

	drbd_bm_ext_cache = kmem_cache_create(
		"drbd_bm", sizeof(struct bm_extent), 0, 0, NULL);
	if (drbd_bm_ext_cache == NULL)
		goto Enomem;

	drbd_al_ext_cache = kmem_cache_create(
		"drbd_al", sizeof(struct lc_element), 0, 0, NULL);
	if (drbd_al_ext_cache == NULL)
		goto Enomem;

	/* mempools */
<<<<<<< HEAD
=======
	drbd_md_io_bio_set = bioset_create(DRBD_MIN_POOL_PAGES, 0);
	if (drbd_md_io_bio_set == NULL)
		goto Enomem;

	drbd_md_io_page_pool = mempool_create_page_pool(DRBD_MIN_POOL_PAGES, 0);
	if (drbd_md_io_page_pool == NULL)
		goto Enomem;

>>>>>>> refs/remotes/origin/master
	drbd_request_mempool = mempool_create(number,
		mempool_alloc_slab, mempool_free_slab, drbd_request_cache);
	if (drbd_request_mempool == NULL)
		goto Enomem;

	drbd_ee_mempool = mempool_create(number,
		mempool_alloc_slab, mempool_free_slab, drbd_ee_cache);
	if (drbd_ee_mempool == NULL)
		goto Enomem;

	/* drbd's page pool */
	spin_lock_init(&drbd_pp_lock);

	for (i = 0; i < number; i++) {
		page = alloc_page(GFP_HIGHUSER);
		if (!page)
			goto Enomem;
		set_page_private(page, (unsigned long)drbd_pp_pool);
		drbd_pp_pool = page;
	}
	drbd_pp_vacant = number;

	return 0;

Enomem:
	drbd_destroy_mempools(); /* in case we allocated some */
	return -ENOMEM;
}

static int drbd_notify_sys(struct notifier_block *this, unsigned long code,
	void *unused)
{
	/* just so we have it.  you never know what interesting things we
	 * might want to do here some day...
	 */

	return NOTIFY_DONE;
}

static struct notifier_block drbd_notifier = {
	.notifier_call = drbd_notify_sys,
};

<<<<<<< HEAD
static void drbd_release_ee_lists(struct drbd_conf *mdev)
{
	int rr;

	rr = drbd_release_ee(mdev, &mdev->active_ee);
	if (rr)
		dev_err(DEV, "%d EEs in active list found!\n", rr);

	rr = drbd_release_ee(mdev, &mdev->sync_ee);
	if (rr)
		dev_err(DEV, "%d EEs in sync list found!\n", rr);

	rr = drbd_release_ee(mdev, &mdev->read_ee);
	if (rr)
		dev_err(DEV, "%d EEs in read list found!\n", rr);

	rr = drbd_release_ee(mdev, &mdev->done_ee);
	if (rr)
		dev_err(DEV, "%d EEs in done list found!\n", rr);

	rr = drbd_release_ee(mdev, &mdev->net_ee);
=======
static void drbd_release_all_peer_reqs(struct drbd_conf *mdev)
{
	int rr;

	rr = drbd_free_peer_reqs(mdev, &mdev->active_ee);
	if (rr)
		dev_err(DEV, "%d EEs in active list found!\n", rr);

	rr = drbd_free_peer_reqs(mdev, &mdev->sync_ee);
	if (rr)
		dev_err(DEV, "%d EEs in sync list found!\n", rr);

	rr = drbd_free_peer_reqs(mdev, &mdev->read_ee);
	if (rr)
		dev_err(DEV, "%d EEs in read list found!\n", rr);

	rr = drbd_free_peer_reqs(mdev, &mdev->done_ee);
	if (rr)
		dev_err(DEV, "%d EEs in done list found!\n", rr);

	rr = drbd_free_peer_reqs(mdev, &mdev->net_ee);
>>>>>>> refs/remotes/origin/master
	if (rr)
		dev_err(DEV, "%d EEs in net list found!\n", rr);
}

<<<<<<< HEAD
/* caution. no locking.
 * currently only used from module cleanup code. */
static void drbd_delete_device(unsigned int minor)
{
	struct drbd_conf *mdev = minor_to_mdev(minor);

	if (!mdev)
		return;

	/* paranoia asserts */
	if (mdev->open_cnt != 0)
		dev_err(DEV, "open_cnt = %d in %s:%u", mdev->open_cnt,
				__FILE__ , __LINE__);

	ERR_IF (!list_empty(&mdev->data.work.q)) {
		struct list_head *lp;
		list_for_each(lp, &mdev->data.work.q) {
			dev_err(DEV, "lp = %p\n", lp);
		}
	};
	/* end paranoia asserts */

	del_gendisk(mdev->vdisk);

=======
/* caution. no locking. */
void drbd_minor_destroy(struct kref *kref)
{
	struct drbd_conf *mdev = container_of(kref, struct drbd_conf, kref);
	struct drbd_tconn *tconn = mdev->tconn;

	del_timer_sync(&mdev->request_timer);

	/* paranoia asserts */
	D_ASSERT(mdev->open_cnt == 0);
	/* end paranoia asserts */

>>>>>>> refs/remotes/origin/master
	/* cleanup stuff that may have been allocated during
	 * device (re-)configuration or state changes */

	if (mdev->this_bdev)
		bdput(mdev->this_bdev);

<<<<<<< HEAD
	drbd_free_resources(mdev);

	drbd_release_ee_lists(mdev);

	/* should be freed on disconnect? */
	kfree(mdev->ee_hash);
	/*
	mdev->ee_hash_s = 0;
	mdev->ee_hash = NULL;
	*/
=======
	drbd_free_bc(mdev->ldev);
	mdev->ldev = NULL;

	drbd_release_all_peer_reqs(mdev);
>>>>>>> refs/remotes/origin/master

	lc_destroy(mdev->act_log);
	lc_destroy(mdev->resync);

	kfree(mdev->p_uuid);
	/* mdev->p_uuid = NULL; */

<<<<<<< HEAD
	kfree(mdev->int_dig_out);
	kfree(mdev->int_dig_in);
	kfree(mdev->int_dig_vv);

	/* cleanup the rest that has been
	 * allocated from drbd_new_device
	 * and actually free the mdev itself */
	drbd_free_mdev(mdev);
}

static void drbd_cleanup(void)
{
	unsigned int i;
=======
	if (mdev->bitmap) /* should no longer be there. */
		drbd_bm_cleanup(mdev);
	__free_page(mdev->md_io_page);
	put_disk(mdev->vdisk);
	blk_cleanup_queue(mdev->rq_queue);
	kfree(mdev->rs_plan_s);
	kfree(mdev);

	kref_put(&tconn->kref, &conn_destroy);
}

/* One global retry thread, if we need to push back some bio and have it
 * reinserted through our make request function.
 */
static struct retry_worker {
	struct workqueue_struct *wq;
	struct work_struct worker;

	spinlock_t lock;
	struct list_head writes;
} retry;

static void do_retry(struct work_struct *ws)
{
	struct retry_worker *retry = container_of(ws, struct retry_worker, worker);
	LIST_HEAD(writes);
	struct drbd_request *req, *tmp;

	spin_lock_irq(&retry->lock);
	list_splice_init(&retry->writes, &writes);
	spin_unlock_irq(&retry->lock);

	list_for_each_entry_safe(req, tmp, &writes, tl_requests) {
		struct drbd_conf *mdev = req->w.mdev;
		struct bio *bio = req->master_bio;
		unsigned long start_time = req->start_time;
		bool expected;

		expected = 
			expect(atomic_read(&req->completion_ref) == 0) &&
			expect(req->rq_state & RQ_POSTPONED) &&
			expect((req->rq_state & RQ_LOCAL_PENDING) == 0 ||
				(req->rq_state & RQ_LOCAL_ABORTED) != 0);

		if (!expected)
			dev_err(DEV, "req=%p completion_ref=%d rq_state=%x\n",
				req, atomic_read(&req->completion_ref),
				req->rq_state);

		/* We still need to put one kref associated with the
		 * "completion_ref" going zero in the code path that queued it
		 * here.  The request object may still be referenced by a
		 * frozen local req->private_bio, in case we force-detached.
		 */
		kref_put(&req->kref, drbd_req_destroy);

		/* A single suspended or otherwise blocking device may stall
		 * all others as well.  Fortunately, this code path is to
		 * recover from a situation that "should not happen":
		 * concurrent writes in multi-primary setup.
		 * In a "normal" lifecycle, this workqueue is supposed to be
		 * destroyed without ever doing anything.
		 * If it turns out to be an issue anyways, we can do per
		 * resource (replication group) or per device (minor) retry
		 * workqueues instead.
		 */

		/* We are not just doing generic_make_request(),
		 * as we want to keep the start_time information. */
		inc_ap_bio(mdev);
		__drbd_make_request(mdev, bio, start_time);
	}
}

void drbd_restart_request(struct drbd_request *req)
{
	unsigned long flags;
	spin_lock_irqsave(&retry.lock, flags);
	list_move_tail(&req->tl_requests, &retry.writes);
	spin_unlock_irqrestore(&retry.lock, flags);

	/* Drop the extra reference that would otherwise
	 * have been dropped by complete_master_bio.
	 * do_retry() needs to grab a new one. */
	dec_ap_bio(req->w.mdev);

	queue_work(retry.wq, &retry.worker);
}


static void drbd_cleanup(void)
{
	unsigned int i;
	struct drbd_conf *mdev;
	struct drbd_tconn *tconn, *tmp;
>>>>>>> refs/remotes/origin/master

	unregister_reboot_notifier(&drbd_notifier);

	/* first remove proc,
	 * drbdsetup uses it's presence to detect
	 * whether DRBD is loaded.
	 * If we would get stuck in proc removal,
	 * but have netlink already deregistered,
	 * some drbdsetup commands may wait forever
	 * for an answer.
	 */
	if (drbd_proc)
		remove_proc_entry("drbd", NULL);

<<<<<<< HEAD
	drbd_nl_cleanup();

	if (minor_table) {
		i = minor_count;
		while (i--)
			drbd_delete_device(i);
		drbd_destroy_mempools();
	}

	kfree(minor_table);

	unregister_blkdev(DRBD_MAJOR, "drbd");

	printk(KERN_INFO "drbd: module cleanup done.\n");
}

/**
 * drbd_congested() - Callback for pdflush
 * @congested_data:	User data
 * @bdi_bits:		Bits pdflush is currently interested in
 *
 * Returns 1<<BDI_async_congested and/or 1<<BDI_sync_congested if we are congested.
 */
static int drbd_congested(void *congested_data, int bdi_bits)
{
	struct drbd_conf *mdev = congested_data;
	struct request_queue *q;
	char reason = '-';
	int r = 0;

	if (!may_inc_ap_bio(mdev)) {
		/* DRBD has frozen IO */
		r = bdi_bits;
		reason = 'd';
		goto out;
	}

	if (get_ldev(mdev)) {
		q = bdev_get_queue(mdev->ldev->backing_bdev);
		r = bdi_congested(&q->backing_dev_info, bdi_bits);
		put_ldev(mdev);
		if (r)
			reason = 'b';
	}

	if (bdi_bits & (1 << BDI_async_congested) && test_bit(NET_CONGESTED, &mdev->flags)) {
		r |= (1 << BDI_async_congested);
		reason = reason == 'b' ? 'a' : 'n';
	}

out:
	mdev->congestion_reason = reason;
	return r;
}

struct drbd_conf *drbd_new_device(unsigned int minor)
=======
	if (retry.wq)
		destroy_workqueue(retry.wq);

	drbd_genl_unregister();

	idr_for_each_entry(&minors, mdev, i) {
		idr_remove(&minors, mdev_to_minor(mdev));
		idr_remove(&mdev->tconn->volumes, mdev->vnr);
		destroy_workqueue(mdev->submit.wq);
		del_gendisk(mdev->vdisk);
		/* synchronize_rcu(); No other threads running at this point */
		kref_put(&mdev->kref, &drbd_minor_destroy);
	}

	/* not _rcu since, no other updater anymore. Genl already unregistered */
	list_for_each_entry_safe(tconn, tmp, &drbd_tconns, all_tconn) {
		list_del(&tconn->all_tconn); /* not _rcu no proc, not other threads */
		/* synchronize_rcu(); */
		kref_put(&tconn->kref, &conn_destroy);
	}

	drbd_destroy_mempools();
	unregister_blkdev(DRBD_MAJOR, "drbd");

	idr_destroy(&minors);

	printk(KERN_INFO "drbd: module cleanup done.\n");
}

/**
 * drbd_congested() - Callback for the flusher thread
 * @congested_data:	User data
 * @bdi_bits:		Bits the BDI flusher thread is currently interested in
 *
 * Returns 1<<BDI_async_congested and/or 1<<BDI_sync_congested if we are congested.
 */
static int drbd_congested(void *congested_data, int bdi_bits)
{
	struct drbd_conf *mdev = congested_data;
	struct request_queue *q;
	char reason = '-';
	int r = 0;

	if (!may_inc_ap_bio(mdev)) {
		/* DRBD has frozen IO */
		r = bdi_bits;
		reason = 'd';
		goto out;
	}

	if (test_bit(CALLBACK_PENDING, &mdev->tconn->flags)) {
		r |= (1 << BDI_async_congested);
		/* Without good local data, we would need to read from remote,
		 * and that would need the worker thread as well, which is
		 * currently blocked waiting for that usermode helper to
		 * finish.
		 */
		if (!get_ldev_if_state(mdev, D_UP_TO_DATE))
			r |= (1 << BDI_sync_congested);
		else
			put_ldev(mdev);
		r &= bdi_bits;
		reason = 'c';
		goto out;
	}

	if (get_ldev(mdev)) {
		q = bdev_get_queue(mdev->ldev->backing_bdev);
		r = bdi_congested(&q->backing_dev_info, bdi_bits);
		put_ldev(mdev);
		if (r)
			reason = 'b';
	}

	if (bdi_bits & (1 << BDI_async_congested) && test_bit(NET_CONGESTED, &mdev->tconn->flags)) {
		r |= (1 << BDI_async_congested);
		reason = reason == 'b' ? 'a' : 'n';
	}

out:
	mdev->congestion_reason = reason;
	return r;
}

static void drbd_init_workqueue(struct drbd_work_queue* wq)
{
	spin_lock_init(&wq->q_lock);
	INIT_LIST_HEAD(&wq->q);
	init_waitqueue_head(&wq->q_wait);
}

struct drbd_tconn *conn_get_by_name(const char *name)
{
	struct drbd_tconn *tconn;

	if (!name || !name[0])
		return NULL;

	rcu_read_lock();
	list_for_each_entry_rcu(tconn, &drbd_tconns, all_tconn) {
		if (!strcmp(tconn->name, name)) {
			kref_get(&tconn->kref);
			goto found;
		}
	}
	tconn = NULL;
found:
	rcu_read_unlock();
	return tconn;
}

struct drbd_tconn *conn_get_by_addrs(void *my_addr, int my_addr_len,
				     void *peer_addr, int peer_addr_len)
{
	struct drbd_tconn *tconn;

	rcu_read_lock();
	list_for_each_entry_rcu(tconn, &drbd_tconns, all_tconn) {
		if (tconn->my_addr_len == my_addr_len &&
		    tconn->peer_addr_len == peer_addr_len &&
		    !memcmp(&tconn->my_addr, my_addr, my_addr_len) &&
		    !memcmp(&tconn->peer_addr, peer_addr, peer_addr_len)) {
			kref_get(&tconn->kref);
			goto found;
		}
	}
	tconn = NULL;
found:
	rcu_read_unlock();
	return tconn;
}

static int drbd_alloc_socket(struct drbd_socket *socket)
{
	socket->rbuf = (void *) __get_free_page(GFP_KERNEL);
	if (!socket->rbuf)
		return -ENOMEM;
	socket->sbuf = (void *) __get_free_page(GFP_KERNEL);
	if (!socket->sbuf)
		return -ENOMEM;
	return 0;
}

static void drbd_free_socket(struct drbd_socket *socket)
{
	free_page((unsigned long) socket->sbuf);
	free_page((unsigned long) socket->rbuf);
}

void conn_free_crypto(struct drbd_tconn *tconn)
{
	drbd_free_sock(tconn);

	crypto_free_hash(tconn->csums_tfm);
	crypto_free_hash(tconn->verify_tfm);
	crypto_free_hash(tconn->cram_hmac_tfm);
	crypto_free_hash(tconn->integrity_tfm);
	crypto_free_hash(tconn->peer_integrity_tfm);
	kfree(tconn->int_dig_in);
	kfree(tconn->int_dig_vv);

	tconn->csums_tfm = NULL;
	tconn->verify_tfm = NULL;
	tconn->cram_hmac_tfm = NULL;
	tconn->integrity_tfm = NULL;
	tconn->peer_integrity_tfm = NULL;
	tconn->int_dig_in = NULL;
	tconn->int_dig_vv = NULL;
}

int set_resource_options(struct drbd_tconn *tconn, struct res_opts *res_opts)
{
	cpumask_var_t new_cpu_mask;
	int err;

	if (!zalloc_cpumask_var(&new_cpu_mask, GFP_KERNEL))
		return -ENOMEM;
		/*
		retcode = ERR_NOMEM;
		drbd_msg_put_info("unable to allocate cpumask");
		*/

	/* silently ignore cpu mask on UP kernel */
	if (nr_cpu_ids > 1 && res_opts->cpu_mask[0] != 0) {
		/* FIXME: Get rid of constant 32 here */
		err = bitmap_parse(res_opts->cpu_mask, 32,
				   cpumask_bits(new_cpu_mask), nr_cpu_ids);
		if (err) {
			conn_warn(tconn, "bitmap_parse() failed with %d\n", err);
			/* retcode = ERR_CPU_MASK_PARSE; */
			goto fail;
		}
	}
	tconn->res_opts = *res_opts;
	if (!cpumask_equal(tconn->cpu_mask, new_cpu_mask)) {
		cpumask_copy(tconn->cpu_mask, new_cpu_mask);
		drbd_calc_cpu_mask(tconn);
		tconn->receiver.reset_cpu_mask = 1;
		tconn->asender.reset_cpu_mask = 1;
		tconn->worker.reset_cpu_mask = 1;
	}
	err = 0;

fail:
	free_cpumask_var(new_cpu_mask);
	return err;

}

/* caller must be under genl_lock() */
struct drbd_tconn *conn_create(const char *name, struct res_opts *res_opts)
{
	struct drbd_tconn *tconn;

	tconn = kzalloc(sizeof(struct drbd_tconn), GFP_KERNEL);
	if (!tconn)
		return NULL;

	tconn->name = kstrdup(name, GFP_KERNEL);
	if (!tconn->name)
		goto fail;

	if (drbd_alloc_socket(&tconn->data))
		goto fail;
	if (drbd_alloc_socket(&tconn->meta))
		goto fail;

	if (!zalloc_cpumask_var(&tconn->cpu_mask, GFP_KERNEL))
		goto fail;

	if (set_resource_options(tconn, res_opts))
		goto fail;

	tconn->current_epoch = kzalloc(sizeof(struct drbd_epoch), GFP_KERNEL);
	if (!tconn->current_epoch)
		goto fail;

	INIT_LIST_HEAD(&tconn->transfer_log);

	INIT_LIST_HEAD(&tconn->current_epoch->list);
	tconn->epochs = 1;
	spin_lock_init(&tconn->epoch_lock);
	tconn->write_ordering = WO_bdev_flush;

	tconn->send.seen_any_write_yet = false;
	tconn->send.current_epoch_nr = 0;
	tconn->send.current_epoch_writes = 0;

	tconn->cstate = C_STANDALONE;
	mutex_init(&tconn->cstate_mutex);
	spin_lock_init(&tconn->req_lock);
	mutex_init(&tconn->conf_update);
	init_waitqueue_head(&tconn->ping_wait);
	idr_init(&tconn->volumes);

	drbd_init_workqueue(&tconn->sender_work);
	mutex_init(&tconn->data.mutex);
	mutex_init(&tconn->meta.mutex);

	drbd_thread_init(tconn, &tconn->receiver, drbdd_init, "receiver");
	drbd_thread_init(tconn, &tconn->worker, drbd_worker, "worker");
	drbd_thread_init(tconn, &tconn->asender, drbd_asender, "asender");

	kref_init(&tconn->kref);
	list_add_tail_rcu(&tconn->all_tconn, &drbd_tconns);

	return tconn;

fail:
	kfree(tconn->current_epoch);
	free_cpumask_var(tconn->cpu_mask);
	drbd_free_socket(&tconn->meta);
	drbd_free_socket(&tconn->data);
	kfree(tconn->name);
	kfree(tconn);

	return NULL;
}

void conn_destroy(struct kref *kref)
{
	struct drbd_tconn *tconn = container_of(kref, struct drbd_tconn, kref);

	if (atomic_read(&tconn->current_epoch->epoch_size) !=  0)
		conn_err(tconn, "epoch_size:%d\n", atomic_read(&tconn->current_epoch->epoch_size));
	kfree(tconn->current_epoch);

	idr_destroy(&tconn->volumes);

	free_cpumask_var(tconn->cpu_mask);
	drbd_free_socket(&tconn->meta);
	drbd_free_socket(&tconn->data);
	kfree(tconn->name);
	kfree(tconn->int_dig_in);
	kfree(tconn->int_dig_vv);
	kfree(tconn);
}

int init_submitter(struct drbd_conf *mdev)
{
	/* opencoded create_singlethread_workqueue(),
	 * to be able to say "drbd%d", ..., minor */
	mdev->submit.wq = alloc_workqueue("drbd%u_submit",
			WQ_UNBOUND | WQ_MEM_RECLAIM, 1, mdev->minor);
	if (!mdev->submit.wq)
		return -ENOMEM;

	INIT_WORK(&mdev->submit.worker, do_submit);
	spin_lock_init(&mdev->submit.lock);
	INIT_LIST_HEAD(&mdev->submit.writes);
	return 0;
}

enum drbd_ret_code conn_new_minor(struct drbd_tconn *tconn, unsigned int minor, int vnr)
>>>>>>> refs/remotes/origin/master
{
	struct drbd_conf *mdev;
	struct gendisk *disk;
	struct request_queue *q;
<<<<<<< HEAD
=======
	int vnr_got = vnr;
	int minor_got = minor;
	enum drbd_ret_code err = ERR_NOMEM;

	mdev = minor_to_mdev(minor);
	if (mdev)
		return ERR_MINOR_EXISTS;
>>>>>>> refs/remotes/origin/master

	/* GFP_KERNEL, we are outside of all write-out paths */
	mdev = kzalloc(sizeof(struct drbd_conf), GFP_KERNEL);
	if (!mdev)
<<<<<<< HEAD
		return NULL;
	if (!zalloc_cpumask_var(&mdev->cpu_mask, GFP_KERNEL))
		goto out_no_cpumask;

	mdev->minor = minor;
=======
		return ERR_NOMEM;

	kref_get(&tconn->kref);
	mdev->tconn = tconn;

	mdev->minor = minor;
	mdev->vnr = vnr;
>>>>>>> refs/remotes/origin/master

	drbd_init_set_defaults(mdev);

	q = blk_alloc_queue(GFP_KERNEL);
	if (!q)
		goto out_no_q;
	mdev->rq_queue = q;
	q->queuedata   = mdev;

	disk = alloc_disk(1);
	if (!disk)
		goto out_no_disk;
	mdev->vdisk = disk;

	set_disk_ro(disk, true);

	disk->queue = q;
	disk->major = DRBD_MAJOR;
	disk->first_minor = minor;
	disk->fops = &drbd_ops;
	sprintf(disk->disk_name, "drbd%d", minor);
	disk->private_data = mdev;

	mdev->this_bdev = bdget(MKDEV(DRBD_MAJOR, minor));
	/* we have no partitions. we contain only ourselves. */
	mdev->this_bdev->bd_contains = mdev->this_bdev;

	q->backing_dev_info.congested_fn = drbd_congested;
	q->backing_dev_info.congested_data = mdev;

	blk_queue_make_request(q, drbd_make_request);
<<<<<<< HEAD
=======
	blk_queue_flush(q, REQ_FLUSH | REQ_FUA);
>>>>>>> refs/remotes/origin/master
	/* Setting the max_hw_sectors to an odd value of 8kibyte here
	   This triggers a max_bio_size message upon first attach or connect */
	blk_queue_max_hw_sectors(q, DRBD_MAX_BIO_SIZE_SAFE >> 8);
	blk_queue_bounce_limit(q, BLK_BOUNCE_ANY);
	blk_queue_merge_bvec(q, drbd_merge_bvec);
<<<<<<< HEAD
	q->queue_lock = &mdev->req_lock;
=======
	q->queue_lock = &mdev->tconn->req_lock; /* needed since we use */
>>>>>>> refs/remotes/origin/master

	mdev->md_io_page = alloc_page(GFP_KERNEL);
	if (!mdev->md_io_page)
		goto out_no_io_page;

	if (drbd_bm_init(mdev))
		goto out_no_bitmap;
<<<<<<< HEAD
	/* no need to lock access, we are still initializing this minor device. */
	if (!tl_init(mdev))
		goto out_no_tl;

	mdev->app_reads_hash = kzalloc(APP_R_HSIZE*sizeof(void *), GFP_KERNEL);
	if (!mdev->app_reads_hash)
		goto out_no_app_reads;

	mdev->current_epoch = kzalloc(sizeof(struct drbd_epoch), GFP_KERNEL);
	if (!mdev->current_epoch)
		goto out_no_epoch;

	INIT_LIST_HEAD(&mdev->current_epoch->list);
	mdev->epochs = 1;

	return mdev;

/* out_whatever_else:
	kfree(mdev->current_epoch); */
out_no_epoch:
	kfree(mdev->app_reads_hash);
out_no_app_reads:
	tl_cleanup(mdev);
out_no_tl:
=======
	mdev->read_requests = RB_ROOT;
	mdev->write_requests = RB_ROOT;

	minor_got = idr_alloc(&minors, mdev, minor, minor + 1, GFP_KERNEL);
	if (minor_got < 0) {
		if (minor_got == -ENOSPC) {
			err = ERR_MINOR_EXISTS;
			drbd_msg_put_info("requested minor exists already");
		}
		goto out_no_minor_idr;
	}

	vnr_got = idr_alloc(&tconn->volumes, mdev, vnr, vnr + 1, GFP_KERNEL);
	if (vnr_got < 0) {
		if (vnr_got == -ENOSPC) {
			err = ERR_INVALID_REQUEST;
			drbd_msg_put_info("requested volume exists already");
		}
		goto out_idr_remove_minor;
	}

	if (init_submitter(mdev)) {
		err = ERR_NOMEM;
		drbd_msg_put_info("unable to create submit workqueue");
		goto out_idr_remove_vol;
	}

	add_disk(disk);
	kref_init(&mdev->kref); /* one ref for both idrs and the the add_disk */

	/* inherit the connection state */
	mdev->state.conn = tconn->cstate;
	if (mdev->state.conn == C_WF_REPORT_PARAMS)
		drbd_connected(mdev);

	return NO_ERROR;

out_idr_remove_vol:
	idr_remove(&tconn->volumes, vnr_got);
out_idr_remove_minor:
	idr_remove(&minors, minor_got);
	synchronize_rcu();
out_no_minor_idr:
>>>>>>> refs/remotes/origin/master
	drbd_bm_cleanup(mdev);
out_no_bitmap:
	__free_page(mdev->md_io_page);
out_no_io_page:
	put_disk(disk);
out_no_disk:
	blk_cleanup_queue(q);
out_no_q:
<<<<<<< HEAD
	free_cpumask_var(mdev->cpu_mask);
out_no_cpumask:
	kfree(mdev);
	return NULL;
}

/* counterpart of drbd_new_device.
 * last part of drbd_delete_device. */
void drbd_free_mdev(struct drbd_conf *mdev)
{
	kfree(mdev->current_epoch);
	kfree(mdev->app_reads_hash);
	tl_cleanup(mdev);
	if (mdev->bitmap) /* should no longer be there. */
		drbd_bm_cleanup(mdev);
	__free_page(mdev->md_io_page);
	put_disk(mdev->vdisk);
	blk_cleanup_queue(mdev->rq_queue);
	free_cpumask_var(mdev->cpu_mask);
	drbd_free_tl_hash(mdev);
	kfree(mdev);
}


=======
	kfree(mdev);
	kref_put(&tconn->kref, &conn_destroy);
	return err;
}

>>>>>>> refs/remotes/origin/master
int __init drbd_init(void)
{
	int err;

<<<<<<< HEAD
	if (sizeof(struct p_handshake) != 80) {
		printk(KERN_ERR
		       "drbd: never change the size or layout "
		       "of the HandShake packet.\n");
		return -EINVAL;
	}

	if (minor_count < DRBD_MINOR_COUNT_MIN || minor_count > DRBD_MINOR_COUNT_MAX) {
		printk(KERN_ERR
			"drbd: invalid minor_count (%d)\n", minor_count);
#ifdef MODULE
		return -EINVAL;
#else
		minor_count = 8;
#endif
	}

	err = drbd_nl_init();
	if (err)
		return err;

=======
	if (minor_count < DRBD_MINOR_COUNT_MIN || minor_count > DRBD_MINOR_COUNT_MAX) {
		printk(KERN_ERR
		       "drbd: invalid minor_count (%d)\n", minor_count);
#ifdef MODULE
		return -EINVAL;
#else
		minor_count = DRBD_MINOR_COUNT_DEF;
#endif
	}

>>>>>>> refs/remotes/origin/master
	err = register_blkdev(DRBD_MAJOR, "drbd");
	if (err) {
		printk(KERN_ERR
		       "drbd: unable to register block device major %d\n",
		       DRBD_MAJOR);
		return err;
	}

	register_reboot_notifier(&drbd_notifier);

	/*
	 * allocate all necessary structs
	 */
<<<<<<< HEAD
	err = -ENOMEM;

	init_waitqueue_head(&drbd_pp_wait);

	drbd_proc = NULL; /* play safe for drbd_cleanup */
	minor_table = kzalloc(sizeof(struct drbd_conf *)*minor_count,
				GFP_KERNEL);
	if (!minor_table)
		goto Enomem;

	err = drbd_create_mempools();
	if (err)
		goto Enomem;

	drbd_proc = proc_create_data("drbd", S_IFREG | S_IRUGO , NULL, &drbd_proc_fops, NULL);
	if (!drbd_proc)	{
		printk(KERN_ERR "drbd: unable to register proc file\n");
		goto Enomem;
	}

	rwlock_init(&global_state_lock);
=======
	init_waitqueue_head(&drbd_pp_wait);

	drbd_proc = NULL; /* play safe for drbd_cleanup */
	idr_init(&minors);

	rwlock_init(&global_state_lock);
	INIT_LIST_HEAD(&drbd_tconns);

	err = drbd_genl_register();
	if (err) {
		printk(KERN_ERR "drbd: unable to register generic netlink family\n");
		goto fail;
	}

	err = drbd_create_mempools();
	if (err)
		goto fail;

	err = -ENOMEM;
	drbd_proc = proc_create_data("drbd", S_IFREG | S_IRUGO , NULL, &drbd_proc_fops, NULL);
	if (!drbd_proc)	{
		printk(KERN_ERR "drbd: unable to register proc file\n");
		goto fail;
	}

	retry.wq = create_singlethread_workqueue("drbd-reissue");
	if (!retry.wq) {
		printk(KERN_ERR "drbd: unable to create retry workqueue\n");
		goto fail;
	}
	INIT_WORK(&retry.worker, do_retry);
	spin_lock_init(&retry.lock);
	INIT_LIST_HEAD(&retry.writes);
>>>>>>> refs/remotes/origin/master

	printk(KERN_INFO "drbd: initialized. "
	       "Version: " REL_VERSION " (api:%d/proto:%d-%d)\n",
	       API_VERSION, PRO_VERSION_MIN, PRO_VERSION_MAX);
	printk(KERN_INFO "drbd: %s\n", drbd_buildtag());
	printk(KERN_INFO "drbd: registered as block device major %d\n",
		DRBD_MAJOR);
<<<<<<< HEAD
	printk(KERN_INFO "drbd: minor_table @ 0x%p\n", minor_table);

	return 0; /* Success! */

Enomem:
	drbd_cleanup();
	if (err == -ENOMEM)
		/* currently always the case */
=======

	return 0; /* Success! */

fail:
	drbd_cleanup();
	if (err == -ENOMEM)
>>>>>>> refs/remotes/origin/master
		printk(KERN_ERR "drbd: ran out of memory\n");
	else
		printk(KERN_ERR "drbd: initialization failure\n");
	return err;
}

void drbd_free_bc(struct drbd_backing_dev *ldev)
{
	if (ldev == NULL)
		return;

	blkdev_put(ldev->backing_bdev, FMODE_READ | FMODE_WRITE | FMODE_EXCL);
	blkdev_put(ldev->md_bdev, FMODE_READ | FMODE_WRITE | FMODE_EXCL);

<<<<<<< HEAD
	kfree(ldev);
}

void drbd_free_sock(struct drbd_conf *mdev)
{
	if (mdev->data.socket) {
		mutex_lock(&mdev->data.mutex);
		kernel_sock_shutdown(mdev->data.socket, SHUT_RDWR);
		sock_release(mdev->data.socket);
		mdev->data.socket = NULL;
		mutex_unlock(&mdev->data.mutex);
	}
	if (mdev->meta.socket) {
		mutex_lock(&mdev->meta.mutex);
		kernel_sock_shutdown(mdev->meta.socket, SHUT_RDWR);
		sock_release(mdev->meta.socket);
		mdev->meta.socket = NULL;
		mutex_unlock(&mdev->meta.mutex);
	}
}


void drbd_free_resources(struct drbd_conf *mdev)
{
	crypto_free_hash(mdev->csums_tfm);
	mdev->csums_tfm = NULL;
	crypto_free_hash(mdev->verify_tfm);
	mdev->verify_tfm = NULL;
	crypto_free_hash(mdev->cram_hmac_tfm);
	mdev->cram_hmac_tfm = NULL;
	crypto_free_hash(mdev->integrity_w_tfm);
	mdev->integrity_w_tfm = NULL;
	crypto_free_hash(mdev->integrity_r_tfm);
	mdev->integrity_r_tfm = NULL;

	drbd_free_sock(mdev);

	__no_warn(local,
		  drbd_free_bc(mdev->ldev);
		  mdev->ldev = NULL;);
}

/* meta data management */

struct meta_data_on_disk {
	u64 la_size;           /* last agreed size. */
=======
	kfree(ldev->disk_conf);
	kfree(ldev);
}

void drbd_free_sock(struct drbd_tconn *tconn)
{
	if (tconn->data.socket) {
		mutex_lock(&tconn->data.mutex);
		kernel_sock_shutdown(tconn->data.socket, SHUT_RDWR);
		sock_release(tconn->data.socket);
		tconn->data.socket = NULL;
		mutex_unlock(&tconn->data.mutex);
	}
	if (tconn->meta.socket) {
		mutex_lock(&tconn->meta.mutex);
		kernel_sock_shutdown(tconn->meta.socket, SHUT_RDWR);
		sock_release(tconn->meta.socket);
		tconn->meta.socket = NULL;
		mutex_unlock(&tconn->meta.mutex);
	}
}

/* meta data management */

void conn_md_sync(struct drbd_tconn *tconn)
{
	struct drbd_conf *mdev;
	int vnr;

	rcu_read_lock();
	idr_for_each_entry(&tconn->volumes, mdev, vnr) {
		kref_get(&mdev->kref);
		rcu_read_unlock();
		drbd_md_sync(mdev);
		kref_put(&mdev->kref, &drbd_minor_destroy);
		rcu_read_lock();
	}
	rcu_read_unlock();
}

/* aligned 4kByte */
struct meta_data_on_disk {
	u64 la_size_sect;      /* last agreed size. */
>>>>>>> refs/remotes/origin/master
	u64 uuid[UI_SIZE];   /* UUIDs. */
	u64 device_uuid;
	u64 reserved_u64_1;
	u32 flags;             /* MDF */
	u32 magic;
	u32 md_size_sect;
	u32 al_offset;         /* offset to this block */
<<<<<<< HEAD
	u32 al_nr_extents;     /* important for restoring the AL */
	      /* `-- act_log->nr_elements <-- sync_conf.al_extents */
	u32 bm_offset;         /* offset to the bitmap, from here */
	u32 bm_bytes_per_bit;  /* BM_BLOCK_SIZE */
	u32 la_peer_max_bio_size;   /* last peer max_bio_size */
	u32 reserved_u32[3];

} __packed;

/**
 * drbd_md_sync() - Writes the meta data super block if the MD_DIRTY flag bit is set
 * @mdev:	DRBD device.
 */
void drbd_md_sync(struct drbd_conf *mdev)
{
	struct meta_data_on_disk *buffer;
	sector_t sector;
	int i;

	del_timer(&mdev->md_sync_timer);
	/* timer may be rearmed by drbd_md_mark_dirty() now. */
	if (!test_and_clear_bit(MD_DIRTY, &mdev->flags))
		return;

	/* We use here D_FAILED and not D_ATTACHING because we try to write
	 * metadata even if we detach due to a disk failure! */
	if (!get_ldev_if_state(mdev, D_FAILED))
		return;

	mutex_lock(&mdev->md_io_mutex);
	buffer = (struct meta_data_on_disk *)page_address(mdev->md_io_page);
	memset(buffer, 0, 512);

	buffer->la_size = cpu_to_be64(drbd_get_capacity(mdev->this_bdev));
	for (i = UI_CURRENT; i < UI_SIZE; i++)
		buffer->uuid[i] = cpu_to_be64(mdev->ldev->md.uuid[i]);
	buffer->flags = cpu_to_be32(mdev->ldev->md.flags);
	buffer->magic = cpu_to_be32(DRBD_MD_MAGIC);
=======
	u32 al_nr_extents;     /* important for restoring the AL (userspace) */
	      /* `-- act_log->nr_elements <-- ldev->dc.al_extents */
	u32 bm_offset;         /* offset to the bitmap, from here */
	u32 bm_bytes_per_bit;  /* BM_BLOCK_SIZE */
	u32 la_peer_max_bio_size;   /* last peer max_bio_size */

	/* see al_tr_number_to_on_disk_sector() */
	u32 al_stripes;
	u32 al_stripe_size_4k;

	u8 reserved_u8[4096 - (7*8 + 10*4)];
} __packed;



void drbd_md_write(struct drbd_conf *mdev, void *b)
{
	struct meta_data_on_disk *buffer = b;
	sector_t sector;
	int i;

	memset(buffer, 0, sizeof(*buffer));

	buffer->la_size_sect = cpu_to_be64(drbd_get_capacity(mdev->this_bdev));
	for (i = UI_CURRENT; i < UI_SIZE; i++)
		buffer->uuid[i] = cpu_to_be64(mdev->ldev->md.uuid[i]);
	buffer->flags = cpu_to_be32(mdev->ldev->md.flags);
	buffer->magic = cpu_to_be32(DRBD_MD_MAGIC_84_UNCLEAN);
>>>>>>> refs/remotes/origin/master

	buffer->md_size_sect  = cpu_to_be32(mdev->ldev->md.md_size_sect);
	buffer->al_offset     = cpu_to_be32(mdev->ldev->md.al_offset);
	buffer->al_nr_extents = cpu_to_be32(mdev->act_log->nr_elements);
	buffer->bm_bytes_per_bit = cpu_to_be32(BM_BLOCK_SIZE);
	buffer->device_uuid = cpu_to_be64(mdev->ldev->md.device_uuid);

	buffer->bm_offset = cpu_to_be32(mdev->ldev->md.bm_offset);
	buffer->la_peer_max_bio_size = cpu_to_be32(mdev->peer_max_bio_size);

<<<<<<< HEAD
	D_ASSERT(drbd_md_ss__(mdev, mdev->ldev) == mdev->ldev->md.md_offset);
	sector = mdev->ldev->md.md_offset;

	if (!drbd_md_sync_page_io(mdev, mdev->ldev, sector, WRITE)) {
		/* this was a try anyways ... */
		dev_err(DEV, "meta data update failed!\n");
		drbd_chk_io_error(mdev, 1, true);
	}
=======
	buffer->al_stripes = cpu_to_be32(mdev->ldev->md.al_stripes);
	buffer->al_stripe_size_4k = cpu_to_be32(mdev->ldev->md.al_stripe_size_4k);

	D_ASSERT(drbd_md_ss(mdev->ldev) == mdev->ldev->md.md_offset);
	sector = mdev->ldev->md.md_offset;

	if (drbd_md_sync_page_io(mdev, mdev->ldev, sector, WRITE)) {
		/* this was a try anyways ... */
		dev_err(DEV, "meta data update failed!\n");
		drbd_chk_io_error(mdev, 1, DRBD_META_IO_ERROR);
	}
}

/**
 * drbd_md_sync() - Writes the meta data super block if the MD_DIRTY flag bit is set
 * @mdev:	DRBD device.
 */
void drbd_md_sync(struct drbd_conf *mdev)
{
	struct meta_data_on_disk *buffer;

	/* Don't accidentally change the DRBD meta data layout. */
	BUILD_BUG_ON(UI_SIZE != 4);
	BUILD_BUG_ON(sizeof(struct meta_data_on_disk) != 4096);

	del_timer(&mdev->md_sync_timer);
	/* timer may be rearmed by drbd_md_mark_dirty() now. */
	if (!test_and_clear_bit(MD_DIRTY, &mdev->flags))
		return;

	/* We use here D_FAILED and not D_ATTACHING because we try to write
	 * metadata even if we detach due to a disk failure! */
	if (!get_ldev_if_state(mdev, D_FAILED))
		return;

	buffer = drbd_md_get_buffer(mdev);
	if (!buffer)
		goto out;

	drbd_md_write(mdev, buffer);
>>>>>>> refs/remotes/origin/master

	/* Update mdev->ldev->md.la_size_sect,
	 * since we updated it on metadata. */
	mdev->ldev->md.la_size_sect = drbd_get_capacity(mdev->this_bdev);

<<<<<<< HEAD
	mutex_unlock(&mdev->md_io_mutex);
	put_ldev(mdev);
}

=======
	drbd_md_put_buffer(mdev);
out:
	put_ldev(mdev);
}

static int check_activity_log_stripe_size(struct drbd_conf *mdev,
		struct meta_data_on_disk *on_disk,
		struct drbd_md *in_core)
{
	u32 al_stripes = be32_to_cpu(on_disk->al_stripes);
	u32 al_stripe_size_4k = be32_to_cpu(on_disk->al_stripe_size_4k);
	u64 al_size_4k;

	/* both not set: default to old fixed size activity log */
	if (al_stripes == 0 && al_stripe_size_4k == 0) {
		al_stripes = 1;
		al_stripe_size_4k = MD_32kB_SECT/8;
	}

	/* some paranoia plausibility checks */

	/* we need both values to be set */
	if (al_stripes == 0 || al_stripe_size_4k == 0)
		goto err;

	al_size_4k = (u64)al_stripes * al_stripe_size_4k;

	/* Upper limit of activity log area, to avoid potential overflow
	 * problems in al_tr_number_to_on_disk_sector(). As right now, more
	 * than 72 * 4k blocks total only increases the amount of history,
	 * limiting this arbitrarily to 16 GB is not a real limitation ;-)  */
	if (al_size_4k > (16 * 1024 * 1024/4))
		goto err;

	/* Lower limit: we need at least 8 transaction slots (32kB)
	 * to not break existing setups */
	if (al_size_4k < MD_32kB_SECT/8)
		goto err;

	in_core->al_stripe_size_4k = al_stripe_size_4k;
	in_core->al_stripes = al_stripes;
	in_core->al_size_4k = al_size_4k;

	return 0;
err:
	dev_err(DEV, "invalid activity log striping: al_stripes=%u, al_stripe_size_4k=%u\n",
			al_stripes, al_stripe_size_4k);
	return -EINVAL;
}

static int check_offsets_and_sizes(struct drbd_conf *mdev, struct drbd_backing_dev *bdev)
{
	sector_t capacity = drbd_get_capacity(bdev->md_bdev);
	struct drbd_md *in_core = &bdev->md;
	s32 on_disk_al_sect;
	s32 on_disk_bm_sect;

	/* The on-disk size of the activity log, calculated from offsets, and
	 * the size of the activity log calculated from the stripe settings,
	 * should match.
	 * Though we could relax this a bit: it is ok, if the striped activity log
	 * fits in the available on-disk activity log size.
	 * Right now, that would break how resize is implemented.
	 * TODO: make drbd_determine_dev_size() (and the drbdmeta tool) aware
	 * of possible unused padding space in the on disk layout. */
	if (in_core->al_offset < 0) {
		if (in_core->bm_offset > in_core->al_offset)
			goto err;
		on_disk_al_sect = -in_core->al_offset;
		on_disk_bm_sect = in_core->al_offset - in_core->bm_offset;
	} else {
		if (in_core->al_offset != MD_4kB_SECT)
			goto err;
		if (in_core->bm_offset < in_core->al_offset + in_core->al_size_4k * MD_4kB_SECT)
			goto err;

		on_disk_al_sect = in_core->bm_offset - MD_4kB_SECT;
		on_disk_bm_sect = in_core->md_size_sect - in_core->bm_offset;
	}

	/* old fixed size meta data is exactly that: fixed. */
	if (in_core->meta_dev_idx >= 0) {
		if (in_core->md_size_sect != MD_128MB_SECT
		||  in_core->al_offset != MD_4kB_SECT
		||  in_core->bm_offset != MD_4kB_SECT + MD_32kB_SECT
		||  in_core->al_stripes != 1
		||  in_core->al_stripe_size_4k != MD_32kB_SECT/8)
			goto err;
	}

	if (capacity < in_core->md_size_sect)
		goto err;
	if (capacity - in_core->md_size_sect < drbd_md_first_sector(bdev))
		goto err;

	/* should be aligned, and at least 32k */
	if ((on_disk_al_sect & 7) || (on_disk_al_sect < MD_32kB_SECT))
		goto err;

	/* should fit (for now: exactly) into the available on-disk space;
	 * overflow prevention is in check_activity_log_stripe_size() above. */
	if (on_disk_al_sect != in_core->al_size_4k * MD_4kB_SECT)
		goto err;

	/* again, should be aligned */
	if (in_core->bm_offset & 7)
		goto err;

	/* FIXME check for device grow with flex external meta data? */

	/* can the available bitmap space cover the last agreed device size? */
	if (on_disk_bm_sect < (in_core->la_size_sect+7)/MD_4kB_SECT/8/512)
		goto err;

	return 0;

err:
	dev_err(DEV, "meta data offsets don't make sense: idx=%d "
			"al_s=%u, al_sz4k=%u, al_offset=%d, bm_offset=%d, "
			"md_size_sect=%u, la_size=%llu, md_capacity=%llu\n",
			in_core->meta_dev_idx,
			in_core->al_stripes, in_core->al_stripe_size_4k,
			in_core->al_offset, in_core->bm_offset, in_core->md_size_sect,
			(unsigned long long)in_core->la_size_sect,
			(unsigned long long)capacity);

	return -EINVAL;
}


>>>>>>> refs/remotes/origin/master
/**
 * drbd_md_read() - Reads in the meta data super block
 * @mdev:	DRBD device.
 * @bdev:	Device from which the meta data should be read in.
 *
<<<<<<< HEAD
 * Return 0 (NO_ERROR) on success, and an enum drbd_ret_code in case
 * something goes wrong.  Currently only: ERR_IO_MD_DISK, ERR_MD_INVALID.
=======
 * Return NO_ERROR on success, and an enum drbd_ret_code in case
 * something goes wrong.
 *
 * Called exactly once during drbd_adm_attach(), while still being D_DISKLESS,
 * even before @bdev is assigned to @mdev->ldev.
>>>>>>> refs/remotes/origin/master
 */
int drbd_md_read(struct drbd_conf *mdev, struct drbd_backing_dev *bdev)
{
	struct meta_data_on_disk *buffer;
<<<<<<< HEAD
	int i, rv = NO_ERROR;

	if (!get_ldev_if_state(mdev, D_ATTACHING))
		return ERR_IO_MD_DISK;

	mutex_lock(&mdev->md_io_mutex);
	buffer = (struct meta_data_on_disk *)page_address(mdev->md_io_page);

	if (!drbd_md_sync_page_io(mdev, bdev, bdev->md.md_offset, READ)) {
=======
	u32 magic, flags;
	int i, rv = NO_ERROR;

	if (mdev->state.disk != D_DISKLESS)
		return ERR_DISK_CONFIGURED;

	buffer = drbd_md_get_buffer(mdev);
	if (!buffer)
		return ERR_NOMEM;

	/* First, figure out where our meta data superblock is located,
	 * and read it. */
	bdev->md.meta_dev_idx = bdev->disk_conf->meta_dev_idx;
	bdev->md.md_offset = drbd_md_ss(bdev);

	if (drbd_md_sync_page_io(mdev, bdev, bdev->md.md_offset, READ)) {
>>>>>>> refs/remotes/origin/master
		/* NOTE: can't do normal error processing here as this is
		   called BEFORE disk is attached */
		dev_err(DEV, "Error while reading metadata.\n");
		rv = ERR_IO_MD_DISK;
		goto err;
	}

<<<<<<< HEAD
	if (be32_to_cpu(buffer->magic) != DRBD_MD_MAGIC) {
		dev_err(DEV, "Error while reading metadata, magic not found.\n");
		rv = ERR_MD_INVALID;
		goto err;
	}
	if (be32_to_cpu(buffer->al_offset) != bdev->md.al_offset) {
		dev_err(DEV, "unexpected al_offset: %d (expected %d)\n",
		    be32_to_cpu(buffer->al_offset), bdev->md.al_offset);
		rv = ERR_MD_INVALID;
		goto err;
	}
	if (be32_to_cpu(buffer->bm_offset) != bdev->md.bm_offset) {
		dev_err(DEV, "unexpected bm_offset: %d (expected %d)\n",
		    be32_to_cpu(buffer->bm_offset), bdev->md.bm_offset);
		rv = ERR_MD_INVALID;
		goto err;
	}
	if (be32_to_cpu(buffer->md_size_sect) != bdev->md.md_size_sect) {
		dev_err(DEV, "unexpected md_size: %u (expected %u)\n",
		    be32_to_cpu(buffer->md_size_sect), bdev->md.md_size_sect);
		rv = ERR_MD_INVALID;
=======
	magic = be32_to_cpu(buffer->magic);
	flags = be32_to_cpu(buffer->flags);
	if (magic == DRBD_MD_MAGIC_84_UNCLEAN ||
	    (magic == DRBD_MD_MAGIC_08 && !(flags & MDF_AL_CLEAN))) {
			/* btw: that's Activity Log clean, not "all" clean. */
		dev_err(DEV, "Found unclean meta data. Did you \"drbdadm apply-al\"?\n");
		rv = ERR_MD_UNCLEAN;
		goto err;
	}

	rv = ERR_MD_INVALID;
	if (magic != DRBD_MD_MAGIC_08) {
		if (magic == DRBD_MD_MAGIC_07)
			dev_err(DEV, "Found old (0.7) meta data magic. Did you \"drbdadm create-md\"?\n");
		else
			dev_err(DEV, "Meta data magic not found. Did you \"drbdadm create-md\"?\n");
>>>>>>> refs/remotes/origin/master
		goto err;
	}

	if (be32_to_cpu(buffer->bm_bytes_per_bit) != BM_BLOCK_SIZE) {
		dev_err(DEV, "unexpected bm_bytes_per_bit: %u (expected %u)\n",
		    be32_to_cpu(buffer->bm_bytes_per_bit), BM_BLOCK_SIZE);
<<<<<<< HEAD
		rv = ERR_MD_INVALID;
		goto err;
	}

	bdev->md.la_size_sect = be64_to_cpu(buffer->la_size);
	for (i = UI_CURRENT; i < UI_SIZE; i++)
		bdev->md.uuid[i] = be64_to_cpu(buffer->uuid[i]);
	bdev->md.flags = be32_to_cpu(buffer->flags);
	mdev->sync_conf.al_extents = be32_to_cpu(buffer->al_nr_extents);
	bdev->md.device_uuid = be64_to_cpu(buffer->device_uuid);

	spin_lock_irq(&mdev->req_lock);
	if (mdev->state.conn < C_CONNECTED) {
		int peer;
		peer = be32_to_cpu(buffer->la_peer_max_bio_size);
		peer = max_t(int, peer, DRBD_MAX_BIO_SIZE_SAFE);
		mdev->peer_max_bio_size = peer;
	}
	spin_unlock_irq(&mdev->req_lock);

	if (mdev->sync_conf.al_extents < 7)
		mdev->sync_conf.al_extents = 127;

 err:
	mutex_unlock(&mdev->md_io_mutex);
	put_ldev(mdev);
=======
		goto err;
	}


	/* convert to in_core endian */
	bdev->md.la_size_sect = be64_to_cpu(buffer->la_size_sect);
	for (i = UI_CURRENT; i < UI_SIZE; i++)
		bdev->md.uuid[i] = be64_to_cpu(buffer->uuid[i]);
	bdev->md.flags = be32_to_cpu(buffer->flags);
	bdev->md.device_uuid = be64_to_cpu(buffer->device_uuid);

	bdev->md.md_size_sect = be32_to_cpu(buffer->md_size_sect);
	bdev->md.al_offset = be32_to_cpu(buffer->al_offset);
	bdev->md.bm_offset = be32_to_cpu(buffer->bm_offset);

	if (check_activity_log_stripe_size(mdev, buffer, &bdev->md))
		goto err;
	if (check_offsets_and_sizes(mdev, bdev))
		goto err;

	if (be32_to_cpu(buffer->bm_offset) != bdev->md.bm_offset) {
		dev_err(DEV, "unexpected bm_offset: %d (expected %d)\n",
		    be32_to_cpu(buffer->bm_offset), bdev->md.bm_offset);
		goto err;
	}
	if (be32_to_cpu(buffer->md_size_sect) != bdev->md.md_size_sect) {
		dev_err(DEV, "unexpected md_size: %u (expected %u)\n",
		    be32_to_cpu(buffer->md_size_sect), bdev->md.md_size_sect);
		goto err;
	}

	rv = NO_ERROR;

	spin_lock_irq(&mdev->tconn->req_lock);
	if (mdev->state.conn < C_CONNECTED) {
		unsigned int peer;
		peer = be32_to_cpu(buffer->la_peer_max_bio_size);
		peer = max(peer, DRBD_MAX_BIO_SIZE_SAFE);
		mdev->peer_max_bio_size = peer;
	}
	spin_unlock_irq(&mdev->tconn->req_lock);

 err:
	drbd_md_put_buffer(mdev);
>>>>>>> refs/remotes/origin/master

	return rv;
}

/**
 * drbd_md_mark_dirty() - Mark meta data super block as dirty
 * @mdev:	DRBD device.
 *
 * Call this function if you change anything that should be written to
 * the meta-data super block. This function sets MD_DIRTY, and starts a
 * timer that ensures that within five seconds you have to call drbd_md_sync().
 */
#ifdef DEBUG
void drbd_md_mark_dirty_(struct drbd_conf *mdev, unsigned int line, const char *func)
{
	if (!test_and_set_bit(MD_DIRTY, &mdev->flags)) {
		mod_timer(&mdev->md_sync_timer, jiffies + HZ);
		mdev->last_md_mark_dirty.line = line;
		mdev->last_md_mark_dirty.func = func;
	}
}
#else
void drbd_md_mark_dirty(struct drbd_conf *mdev)
{
	if (!test_and_set_bit(MD_DIRTY, &mdev->flags))
		mod_timer(&mdev->md_sync_timer, jiffies + 5*HZ);
}
#endif

<<<<<<< HEAD
static void drbd_uuid_move_history(struct drbd_conf *mdev) __must_hold(local)
=======
void drbd_uuid_move_history(struct drbd_conf *mdev) __must_hold(local)
>>>>>>> refs/remotes/origin/master
{
	int i;

	for (i = UI_HISTORY_START; i < UI_HISTORY_END; i++)
		mdev->ldev->md.uuid[i+1] = mdev->ldev->md.uuid[i];
}

<<<<<<< HEAD
void _drbd_uuid_set(struct drbd_conf *mdev, int idx, u64 val) __must_hold(local)
=======
void __drbd_uuid_set(struct drbd_conf *mdev, int idx, u64 val) __must_hold(local)
>>>>>>> refs/remotes/origin/master
{
	if (idx == UI_CURRENT) {
		if (mdev->state.role == R_PRIMARY)
			val |= 1;
		else
			val &= ~((u64)1);

		drbd_set_ed_uuid(mdev, val);
	}

	mdev->ldev->md.uuid[idx] = val;
	drbd_md_mark_dirty(mdev);
}

<<<<<<< HEAD

void drbd_uuid_set(struct drbd_conf *mdev, int idx, u64 val) __must_hold(local)
{
=======
void _drbd_uuid_set(struct drbd_conf *mdev, int idx, u64 val) __must_hold(local)
{
	unsigned long flags;
	spin_lock_irqsave(&mdev->ldev->md.uuid_lock, flags);
	__drbd_uuid_set(mdev, idx, val);
	spin_unlock_irqrestore(&mdev->ldev->md.uuid_lock, flags);
}

void drbd_uuid_set(struct drbd_conf *mdev, int idx, u64 val) __must_hold(local)
{
	unsigned long flags;
	spin_lock_irqsave(&mdev->ldev->md.uuid_lock, flags);
>>>>>>> refs/remotes/origin/master
	if (mdev->ldev->md.uuid[idx]) {
		drbd_uuid_move_history(mdev);
		mdev->ldev->md.uuid[UI_HISTORY_START] = mdev->ldev->md.uuid[idx];
	}
<<<<<<< HEAD
	_drbd_uuid_set(mdev, idx, val);
=======
	__drbd_uuid_set(mdev, idx, val);
	spin_unlock_irqrestore(&mdev->ldev->md.uuid_lock, flags);
>>>>>>> refs/remotes/origin/master
}

/**
 * drbd_uuid_new_current() - Creates a new current UUID
 * @mdev:	DRBD device.
 *
 * Creates a new current UUID, and rotates the old current UUID into
 * the bitmap slot. Causes an incremental resync upon next connect.
 */
void drbd_uuid_new_current(struct drbd_conf *mdev) __must_hold(local)
{
	u64 val;
<<<<<<< HEAD
	unsigned long long bm_uuid = mdev->ldev->md.uuid[UI_BITMAP];
=======
	unsigned long long bm_uuid;

	get_random_bytes(&val, sizeof(u64));

	spin_lock_irq(&mdev->ldev->md.uuid_lock);
	bm_uuid = mdev->ldev->md.uuid[UI_BITMAP];
>>>>>>> refs/remotes/origin/master

	if (bm_uuid)
		dev_warn(DEV, "bm UUID was already set: %llX\n", bm_uuid);

	mdev->ldev->md.uuid[UI_BITMAP] = mdev->ldev->md.uuid[UI_CURRENT];
<<<<<<< HEAD

	get_random_bytes(&val, sizeof(u64));
	_drbd_uuid_set(mdev, UI_CURRENT, val);
=======
	__drbd_uuid_set(mdev, UI_CURRENT, val);
	spin_unlock_irq(&mdev->ldev->md.uuid_lock);

>>>>>>> refs/remotes/origin/master
	drbd_print_uuids(mdev, "new current UUID");
	/* get it to stable storage _now_ */
	drbd_md_sync(mdev);
}

void drbd_uuid_set_bm(struct drbd_conf *mdev, u64 val) __must_hold(local)
{
<<<<<<< HEAD
	if (mdev->ldev->md.uuid[UI_BITMAP] == 0 && val == 0)
		return;

=======
	unsigned long flags;
	if (mdev->ldev->md.uuid[UI_BITMAP] == 0 && val == 0)
		return;

	spin_lock_irqsave(&mdev->ldev->md.uuid_lock, flags);
>>>>>>> refs/remotes/origin/master
	if (val == 0) {
		drbd_uuid_move_history(mdev);
		mdev->ldev->md.uuid[UI_HISTORY_START] = mdev->ldev->md.uuid[UI_BITMAP];
		mdev->ldev->md.uuid[UI_BITMAP] = 0;
	} else {
		unsigned long long bm_uuid = mdev->ldev->md.uuid[UI_BITMAP];
		if (bm_uuid)
			dev_warn(DEV, "bm UUID was already set: %llX\n", bm_uuid);

		mdev->ldev->md.uuid[UI_BITMAP] = val & ~((u64)1);
	}
<<<<<<< HEAD
=======
	spin_unlock_irqrestore(&mdev->ldev->md.uuid_lock, flags);

>>>>>>> refs/remotes/origin/master
	drbd_md_mark_dirty(mdev);
}

/**
 * drbd_bmio_set_n_write() - io_fn for drbd_queue_bitmap_io() or drbd_bitmap_io()
 * @mdev:	DRBD device.
 *
 * Sets all bits in the bitmap and writes the whole bitmap to stable storage.
 */
int drbd_bmio_set_n_write(struct drbd_conf *mdev)
{
	int rv = -EIO;

	if (get_ldev_if_state(mdev, D_ATTACHING)) {
		drbd_md_set_flag(mdev, MDF_FULL_SYNC);
		drbd_md_sync(mdev);
		drbd_bm_set_all(mdev);

		rv = drbd_bm_write(mdev);

		if (!rv) {
			drbd_md_clear_flag(mdev, MDF_FULL_SYNC);
			drbd_md_sync(mdev);
		}

		put_ldev(mdev);
	}

	return rv;
}

/**
 * drbd_bmio_clear_n_write() - io_fn for drbd_queue_bitmap_io() or drbd_bitmap_io()
 * @mdev:	DRBD device.
 *
 * Clears all bits in the bitmap and writes the whole bitmap to stable storage.
 */
int drbd_bmio_clear_n_write(struct drbd_conf *mdev)
{
	int rv = -EIO;

	drbd_resume_al(mdev);
	if (get_ldev_if_state(mdev, D_ATTACHING)) {
		drbd_bm_clear_all(mdev);
		rv = drbd_bm_write(mdev);
		put_ldev(mdev);
	}

	return rv;
}

<<<<<<< HEAD
static int w_bitmap_io(struct drbd_conf *mdev, struct drbd_work *w, int unused)
{
	struct bm_io_work *work = container_of(w, struct bm_io_work, w);
=======
static int w_bitmap_io(struct drbd_work *w, int unused)
{
	struct bm_io_work *work = container_of(w, struct bm_io_work, w);
	struct drbd_conf *mdev = w->mdev;
>>>>>>> refs/remotes/origin/master
	int rv = -EIO;

	D_ASSERT(atomic_read(&mdev->ap_bio_cnt) == 0);

	if (get_ldev(mdev)) {
		drbd_bm_lock(mdev, work->why, work->flags);
		rv = work->io_fn(mdev);
		drbd_bm_unlock(mdev);
		put_ldev(mdev);
	}

<<<<<<< HEAD
	clear_bit(BITMAP_IO, &mdev->flags);
	smp_mb__after_clear_bit();
=======
	clear_bit_unlock(BITMAP_IO, &mdev->flags);
>>>>>>> refs/remotes/origin/master
	wake_up(&mdev->misc_wait);

	if (work->done)
		work->done(mdev, rv);

	clear_bit(BITMAP_IO_QUEUED, &mdev->flags);
	work->why = NULL;
	work->flags = 0;

<<<<<<< HEAD
	return 1;
=======
	return 0;
>>>>>>> refs/remotes/origin/master
}

void drbd_ldev_destroy(struct drbd_conf *mdev)
{
	lc_destroy(mdev->resync);
	mdev->resync = NULL;
	lc_destroy(mdev->act_log);
	mdev->act_log = NULL;
	__no_warn(local,
		drbd_free_bc(mdev->ldev);
		mdev->ldev = NULL;);

<<<<<<< HEAD
	if (mdev->md_io_tmpp) {
		__free_page(mdev->md_io_tmpp);
		mdev->md_io_tmpp = NULL;
	}
	clear_bit(GO_DISKLESS, &mdev->flags);
}

static int w_go_diskless(struct drbd_conf *mdev, struct drbd_work *w, int unused)
{
=======
	clear_bit(GO_DISKLESS, &mdev->flags);
}

static int w_go_diskless(struct drbd_work *w, int unused)
{
	struct drbd_conf *mdev = w->mdev;

>>>>>>> refs/remotes/origin/master
	D_ASSERT(mdev->state.disk == D_FAILED);
	/* we cannot assert local_cnt == 0 here, as get_ldev_if_state will
	 * inc/dec it frequently. Once we are D_DISKLESS, no one will touch
	 * the protected members anymore, though, so once put_ldev reaches zero
	 * again, it will be safe to free them. */
<<<<<<< HEAD
	drbd_force_state(mdev, NS(disk, D_DISKLESS));
	return 1;
}

void drbd_go_diskless(struct drbd_conf *mdev)
{
	D_ASSERT(mdev->state.disk == D_FAILED);
	if (!test_and_set_bit(GO_DISKLESS, &mdev->flags))
		drbd_queue_work(&mdev->data.work, &mdev->go_diskless);
=======

	/* Try to write changed bitmap pages, read errors may have just
	 * set some bits outside the area covered by the activity log.
	 *
	 * If we have an IO error during the bitmap writeout,
	 * we will want a full sync next time, just in case.
	 * (Do we want a specific meta data flag for this?)
	 *
	 * If that does not make it to stable storage either,
	 * we cannot do anything about that anymore.
	 *
	 * We still need to check if both bitmap and ldev are present, we may
	 * end up here after a failed attach, before ldev was even assigned.
	 */
	if (mdev->bitmap && mdev->ldev) {
		/* An interrupted resync or similar is allowed to recounts bits
		 * while we detach.
		 * Any modifications would not be expected anymore, though.
		 */
		if (drbd_bitmap_io_from_worker(mdev, drbd_bm_write,
					"detach", BM_LOCKED_TEST_ALLOWED)) {
			if (test_bit(WAS_READ_ERROR, &mdev->flags)) {
				drbd_md_set_flag(mdev, MDF_FULL_SYNC);
				drbd_md_sync(mdev);
			}
		}
	}

	drbd_force_state(mdev, NS(disk, D_DISKLESS));
	return 0;
>>>>>>> refs/remotes/origin/master
}

/**
 * drbd_queue_bitmap_io() - Queues an IO operation on the whole bitmap
 * @mdev:	DRBD device.
 * @io_fn:	IO callback to be called when bitmap IO is possible
 * @done:	callback to be called after the bitmap IO was performed
 * @why:	Descriptive text of the reason for doing the IO
 *
 * While IO on the bitmap happens we freeze application IO thus we ensure
 * that drbd_set_out_of_sync() can not be called. This function MAY ONLY be
 * called from worker context. It MUST NOT be used while a previous such
 * work is still pending!
 */
void drbd_queue_bitmap_io(struct drbd_conf *mdev,
			  int (*io_fn)(struct drbd_conf *),
			  void (*done)(struct drbd_conf *, int),
			  char *why, enum bm_flag flags)
{
<<<<<<< HEAD
	D_ASSERT(current == mdev->worker.task);
=======
	D_ASSERT(current == mdev->tconn->worker.task);
>>>>>>> refs/remotes/origin/master

	D_ASSERT(!test_bit(BITMAP_IO_QUEUED, &mdev->flags));
	D_ASSERT(!test_bit(BITMAP_IO, &mdev->flags));
	D_ASSERT(list_empty(&mdev->bm_io_work.w.list));
	if (mdev->bm_io_work.why)
		dev_err(DEV, "FIXME going to queue '%s' but '%s' still pending?\n",
			why, mdev->bm_io_work.why);

	mdev->bm_io_work.io_fn = io_fn;
	mdev->bm_io_work.done = done;
	mdev->bm_io_work.why = why;
	mdev->bm_io_work.flags = flags;

<<<<<<< HEAD
	spin_lock_irq(&mdev->req_lock);
	set_bit(BITMAP_IO, &mdev->flags);
	if (atomic_read(&mdev->ap_bio_cnt) == 0) {
		if (!test_and_set_bit(BITMAP_IO_QUEUED, &mdev->flags))
			drbd_queue_work(&mdev->data.work, &mdev->bm_io_work.w);
	}
	spin_unlock_irq(&mdev->req_lock);
=======
	spin_lock_irq(&mdev->tconn->req_lock);
	set_bit(BITMAP_IO, &mdev->flags);
	if (atomic_read(&mdev->ap_bio_cnt) == 0) {
		if (!test_and_set_bit(BITMAP_IO_QUEUED, &mdev->flags))
			drbd_queue_work(&mdev->tconn->sender_work, &mdev->bm_io_work.w);
	}
	spin_unlock_irq(&mdev->tconn->req_lock);
>>>>>>> refs/remotes/origin/master
}

/**
 * drbd_bitmap_io() -  Does an IO operation on the whole bitmap
 * @mdev:	DRBD device.
 * @io_fn:	IO callback to be called when bitmap IO is possible
 * @why:	Descriptive text of the reason for doing the IO
 *
 * freezes application IO while that the actual IO operations runs. This
 * functions MAY NOT be called from worker context.
 */
int drbd_bitmap_io(struct drbd_conf *mdev, int (*io_fn)(struct drbd_conf *),
		char *why, enum bm_flag flags)
{
	int rv;

<<<<<<< HEAD
	D_ASSERT(current != mdev->worker.task);
=======
	D_ASSERT(current != mdev->tconn->worker.task);
>>>>>>> refs/remotes/origin/master

	if ((flags & BM_LOCKED_SET_ALLOWED) == 0)
		drbd_suspend_io(mdev);

	drbd_bm_lock(mdev, why, flags);
	rv = io_fn(mdev);
	drbd_bm_unlock(mdev);

	if ((flags & BM_LOCKED_SET_ALLOWED) == 0)
		drbd_resume_io(mdev);

	return rv;
}

void drbd_md_set_flag(struct drbd_conf *mdev, int flag) __must_hold(local)
{
	if ((mdev->ldev->md.flags & flag) != flag) {
		drbd_md_mark_dirty(mdev);
		mdev->ldev->md.flags |= flag;
	}
}

void drbd_md_clear_flag(struct drbd_conf *mdev, int flag) __must_hold(local)
{
	if ((mdev->ldev->md.flags & flag) != 0) {
		drbd_md_mark_dirty(mdev);
		mdev->ldev->md.flags &= ~flag;
	}
}
int drbd_md_test_flag(struct drbd_backing_dev *bdev, int flag)
{
	return (bdev->md.flags & flag) != 0;
}

static void md_sync_timer_fn(unsigned long data)
{
	struct drbd_conf *mdev = (struct drbd_conf *) data;

<<<<<<< HEAD
	drbd_queue_work_front(&mdev->data.work, &mdev->md_sync_work);
}

static int w_md_sync(struct drbd_conf *mdev, struct drbd_work *w, int unused)
{
=======
	/* must not double-queue! */
	if (list_empty(&mdev->md_sync_work.list))
		drbd_queue_work_front(&mdev->tconn->sender_work, &mdev->md_sync_work);
}

static int w_md_sync(struct drbd_work *w, int unused)
{
	struct drbd_conf *mdev = w->mdev;

>>>>>>> refs/remotes/origin/master
	dev_warn(DEV, "md_sync_timer expired! Worker calls drbd_md_sync().\n");
#ifdef DEBUG
	dev_warn(DEV, "last md_mark_dirty: %s:%u\n",
		mdev->last_md_mark_dirty.func, mdev->last_md_mark_dirty.line);
#endif
	drbd_md_sync(mdev);
<<<<<<< HEAD
	return 1;
=======
	return 0;
}

const char *cmdname(enum drbd_packet cmd)
{
	/* THINK may need to become several global tables
	 * when we want to support more than
	 * one PRO_VERSION */
	static const char *cmdnames[] = {
		[P_DATA]	        = "Data",
		[P_DATA_REPLY]	        = "DataReply",
		[P_RS_DATA_REPLY]	= "RSDataReply",
		[P_BARRIER]	        = "Barrier",
		[P_BITMAP]	        = "ReportBitMap",
		[P_BECOME_SYNC_TARGET]  = "BecomeSyncTarget",
		[P_BECOME_SYNC_SOURCE]  = "BecomeSyncSource",
		[P_UNPLUG_REMOTE]	= "UnplugRemote",
		[P_DATA_REQUEST]	= "DataRequest",
		[P_RS_DATA_REQUEST]     = "RSDataRequest",
		[P_SYNC_PARAM]	        = "SyncParam",
		[P_SYNC_PARAM89]	= "SyncParam89",
		[P_PROTOCOL]            = "ReportProtocol",
		[P_UUIDS]	        = "ReportUUIDs",
		[P_SIZES]	        = "ReportSizes",
		[P_STATE]	        = "ReportState",
		[P_SYNC_UUID]           = "ReportSyncUUID",
		[P_AUTH_CHALLENGE]      = "AuthChallenge",
		[P_AUTH_RESPONSE]	= "AuthResponse",
		[P_PING]		= "Ping",
		[P_PING_ACK]	        = "PingAck",
		[P_RECV_ACK]	        = "RecvAck",
		[P_WRITE_ACK]	        = "WriteAck",
		[P_RS_WRITE_ACK]	= "RSWriteAck",
		[P_SUPERSEDED]          = "Superseded",
		[P_NEG_ACK]	        = "NegAck",
		[P_NEG_DREPLY]	        = "NegDReply",
		[P_NEG_RS_DREPLY]	= "NegRSDReply",
		[P_BARRIER_ACK]	        = "BarrierAck",
		[P_STATE_CHG_REQ]       = "StateChgRequest",
		[P_STATE_CHG_REPLY]     = "StateChgReply",
		[P_OV_REQUEST]          = "OVRequest",
		[P_OV_REPLY]            = "OVReply",
		[P_OV_RESULT]           = "OVResult",
		[P_CSUM_RS_REQUEST]     = "CsumRSRequest",
		[P_RS_IS_IN_SYNC]	= "CsumRSIsInSync",
		[P_COMPRESSED_BITMAP]   = "CBitmap",
		[P_DELAY_PROBE]         = "DelayProbe",
		[P_OUT_OF_SYNC]		= "OutOfSync",
		[P_RETRY_WRITE]		= "RetryWrite",
		[P_RS_CANCEL]		= "RSCancel",
		[P_CONN_ST_CHG_REQ]	= "conn_st_chg_req",
		[P_CONN_ST_CHG_REPLY]	= "conn_st_chg_reply",
		[P_RETRY_WRITE]		= "retry_write",
		[P_PROTOCOL_UPDATE]	= "protocol_update",

		/* enum drbd_packet, but not commands - obsoleted flags:
		 *	P_MAY_IGNORE
		 *	P_MAX_OPT_CMD
		 */
	};

	/* too big for the array: 0xfffX */
	if (cmd == P_INITIAL_META)
		return "InitialMeta";
	if (cmd == P_INITIAL_DATA)
		return "InitialData";
	if (cmd == P_CONNECTION_FEATURES)
		return "ConnectionFeatures";
	if (cmd >= ARRAY_SIZE(cmdnames))
		return "Unknown";
	return cmdnames[cmd];
}

/**
 * drbd_wait_misc  -  wait for a request to make progress
 * @mdev:	device associated with the request
 * @i:		the struct drbd_interval embedded in struct drbd_request or
 *		struct drbd_peer_request
 */
int drbd_wait_misc(struct drbd_conf *mdev, struct drbd_interval *i)
{
	struct net_conf *nc;
	DEFINE_WAIT(wait);
	long timeout;

	rcu_read_lock();
	nc = rcu_dereference(mdev->tconn->net_conf);
	if (!nc) {
		rcu_read_unlock();
		return -ETIMEDOUT;
	}
	timeout = nc->ko_count ? nc->timeout * HZ / 10 * nc->ko_count : MAX_SCHEDULE_TIMEOUT;
	rcu_read_unlock();

	/* Indicate to wake up mdev->misc_wait on progress.  */
	i->waiting = true;
	prepare_to_wait(&mdev->misc_wait, &wait, TASK_INTERRUPTIBLE);
	spin_unlock_irq(&mdev->tconn->req_lock);
	timeout = schedule_timeout(timeout);
	finish_wait(&mdev->misc_wait, &wait);
	spin_lock_irq(&mdev->tconn->req_lock);
	if (!timeout || mdev->state.conn < C_CONNECTED)
		return -ETIMEDOUT;
	if (signal_pending(current))
		return -ERESTARTSYS;
	return 0;
>>>>>>> refs/remotes/origin/master
}

#ifdef CONFIG_DRBD_FAULT_INJECTION
/* Fault insertion support including random number generator shamelessly
 * stolen from kernel/rcutorture.c */
struct fault_random_state {
	unsigned long state;
	unsigned long count;
};

#define FAULT_RANDOM_MULT 39916801  /* prime */
#define FAULT_RANDOM_ADD	479001701 /* prime */
#define FAULT_RANDOM_REFRESH 10000

/*
 * Crude but fast random-number generator.  Uses a linear congruential
 * generator, with occasional help from get_random_bytes().
 */
static unsigned long
_drbd_fault_random(struct fault_random_state *rsp)
{
	long refresh;

	if (!rsp->count--) {
		get_random_bytes(&refresh, sizeof(refresh));
		rsp->state += refresh;
		rsp->count = FAULT_RANDOM_REFRESH;
	}
	rsp->state = rsp->state * FAULT_RANDOM_MULT + FAULT_RANDOM_ADD;
	return swahw32(rsp->state);
}

static char *
_drbd_fault_str(unsigned int type) {
	static char *_faults[] = {
		[DRBD_FAULT_MD_WR] = "Meta-data write",
		[DRBD_FAULT_MD_RD] = "Meta-data read",
		[DRBD_FAULT_RS_WR] = "Resync write",
		[DRBD_FAULT_RS_RD] = "Resync read",
		[DRBD_FAULT_DT_WR] = "Data write",
		[DRBD_FAULT_DT_RD] = "Data read",
		[DRBD_FAULT_DT_RA] = "Data read ahead",
		[DRBD_FAULT_BM_ALLOC] = "BM allocation",
		[DRBD_FAULT_AL_EE] = "EE allocation",
		[DRBD_FAULT_RECEIVE] = "receive data corruption",
	};

	return (type < DRBD_FAULT_MAX) ? _faults[type] : "**Unknown**";
}

unsigned int
_drbd_insert_fault(struct drbd_conf *mdev, unsigned int type)
{
	static struct fault_random_state rrs = {0, 0};

	unsigned int ret = (
		(fault_devs == 0 ||
			((1 << mdev_to_minor(mdev)) & fault_devs) != 0) &&
		(((_drbd_fault_random(&rrs) % 100) + 1) <= fault_rate));

	if (ret) {
		fault_count++;

		if (__ratelimit(&drbd_ratelimit_state))
			dev_warn(DEV, "***Simulating %s failure\n",
				_drbd_fault_str(type));
	}

	return ret;
}
#endif

const char *drbd_buildtag(void)
{
	/* DRBD built from external sources has here a reference to the
	   git hash of the source code. */

	static char buildtag[38] = "\0uilt-in";

	if (buildtag[0] == 0) {
<<<<<<< HEAD
#ifdef CONFIG_MODULES
		if (THIS_MODULE != NULL)
			sprintf(buildtag, "srcversion: %-24s", THIS_MODULE->srcversion);
		else
#endif
			buildtag[0] = 'b';
=======
#ifdef MODULE
		sprintf(buildtag, "srcversion: %-24s", THIS_MODULE->srcversion);
#else
		buildtag[0] = 'b';
#endif
>>>>>>> refs/remotes/origin/master
	}

	return buildtag;
}

module_init(drbd_init)
module_exit(drbd_cleanup)

EXPORT_SYMBOL(drbd_conn_str);
EXPORT_SYMBOL(drbd_role_str);
EXPORT_SYMBOL(drbd_disk_str);
EXPORT_SYMBOL(drbd_set_st_err_str);
