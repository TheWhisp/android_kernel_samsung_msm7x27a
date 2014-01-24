/*
 *  Module for the pnfs nfs4 file layout driver.
 *  Defines all I/O and Policy interface operations, plus code
 *  to register itself with the pNFS client.
 *
 *  Copyright (c) 2002
 *  The Regents of the University of Michigan
 *  All Rights Reserved
 *
 *  Dean Hildebrand <dhildebz@umich.edu>
 *
 *  Permission is granted to use, copy, create derivative works, and
 *  redistribute this software and such derivative works for any purpose,
 *  so long as the name of the University of Michigan is not used in
 *  any advertising or publicity pertaining to the use or distribution
 *  of this software without specific, written prior authorization. If
 *  the above copyright notice or any other identification of the
 *  University of Michigan is included in any copy of any portion of
 *  this software, then the disclaimer below must also be included.
 *
 *  This software is provided as is, without representation or warranty
 *  of any kind either express or implied, including without limitation
 *  the implied warranties of merchantability, fitness for a particular
 *  purpose, or noninfringement.  The Regents of the University of
 *  Michigan shall not be liable for any damages, including special,
 *  indirect, incidental, or consequential damages, with respect to any
 *  claim arising out of or in connection with the use of the software,
 *  even if it has been or is hereafter advised of the possibility of
 *  such damages.
 */

#include <linux/nfs_fs.h>
#include <linux/nfs_page.h>
<<<<<<< HEAD
<<<<<<< HEAD

#include "internal.h"
=======
=======
>>>>>>> refs/remotes/origin/master
#include <linux/module.h>

#include <linux/sunrpc/metrics.h>

<<<<<<< HEAD
#include "internal.h"
#include "delegation.h"
>>>>>>> refs/remotes/origin/cm-10.0
#include "nfs4filelayout.h"
=======
#include "nfs4session.h"
#include "internal.h"
#include "delegation.h"
#include "nfs4filelayout.h"
#include "nfs4trace.h"
>>>>>>> refs/remotes/origin/master

#define NFSDBG_FACILITY         NFSDBG_PNFS_LD

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dean Hildebrand <dhildebz@umich.edu>");
MODULE_DESCRIPTION("The NFSv4 file layout driver");

#define FILELAYOUT_POLL_RETRY_MAX     (15*HZ)

static loff_t
filelayout_get_dense_offset(struct nfs4_filelayout_segment *flseg,
			    loff_t offset)
{
	u32 stripe_width = flseg->stripe_unit * flseg->dsaddr->stripe_count;
<<<<<<< HEAD
<<<<<<< HEAD
	u64 tmp;

	offset -= flseg->pattern_offset;
	tmp = offset;
	do_div(tmp, stripe_width);

	return tmp * flseg->stripe_unit + do_div(offset, flseg->stripe_unit);
=======
=======
>>>>>>> refs/remotes/origin/master
	u64 stripe_no;
	u32 rem;

	offset -= flseg->pattern_offset;
	stripe_no = div_u64(offset, stripe_width);
	div_u64_rem(offset, flseg->stripe_unit, &rem);

	return stripe_no * flseg->stripe_unit + rem;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

/* This function is used by the layout driver to calculate the
 * offset of the file on the dserver based on whether the
 * layout type is STRIPE_DENSE or STRIPE_SPARSE
 */
static loff_t
filelayout_get_dserver_offset(struct pnfs_layout_segment *lseg, loff_t offset)
{
	struct nfs4_filelayout_segment *flseg = FILELAYOUT_LSEG(lseg);

	switch (flseg->stripe_type) {
	case STRIPE_SPARSE:
		return offset;

	case STRIPE_DENSE:
		return filelayout_get_dense_offset(flseg, offset);
	}

	BUG();
}

<<<<<<< HEAD
<<<<<<< HEAD
/* For data server errors we don't recover from */
static void
filelayout_set_lo_fail(struct pnfs_layout_segment *lseg)
{
	if (lseg->pls_range.iomode == IOMODE_RW) {
		dprintk("%s Setting layout IOMODE_RW fail bit\n", __func__);
		set_bit(lo_fail_bit(IOMODE_RW), &lseg->pls_layout->plh_flags);
	} else {
		dprintk("%s Setting layout IOMODE_READ fail bit\n", __func__);
		set_bit(lo_fail_bit(IOMODE_READ), &lseg->pls_layout->plh_flags);
	}
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
static int filelayout_async_handle_error(struct rpc_task *task,
					 struct nfs4_state *state,
					 struct nfs_client *clp,
					 int *reset)
{
<<<<<<< HEAD
	if (task->tk_status >= 0)
		return 0;

	*reset = 0;

	switch (task->tk_status) {
=======
	struct nfs_server *mds_server = NFS_SERVER(state->inode);
	struct nfs_client *mds_client = mds_server->nfs_client;

	if (task->tk_status >= 0)
		return 0;
	*reset = 0;
=======
static void filelayout_reset_write(struct nfs_write_data *data)
{
	struct nfs_pgio_header *hdr = data->header;
	struct rpc_task *task = &data->task;

	if (!test_and_set_bit(NFS_IOHDR_REDO, &hdr->flags)) {
		dprintk("%s Reset task %5u for i/o through MDS "
			"(req %s/%lld, %u bytes @ offset %llu)\n", __func__,
			data->task.tk_pid,
			hdr->inode->i_sb->s_id,
			(long long)NFS_FILEID(hdr->inode),
			data->args.count,
			(unsigned long long)data->args.offset);

		task->tk_status = pnfs_write_done_resend_to_mds(hdr->inode,
							&hdr->pages,
							hdr->completion_ops,
							hdr->dreq);
	}
}

static void filelayout_reset_read(struct nfs_read_data *data)
{
	struct nfs_pgio_header *hdr = data->header;
	struct rpc_task *task = &data->task;

	if (!test_and_set_bit(NFS_IOHDR_REDO, &hdr->flags)) {
		dprintk("%s Reset task %5u for i/o through MDS "
			"(req %s/%lld, %u bytes @ offset %llu)\n", __func__,
			data->task.tk_pid,
			hdr->inode->i_sb->s_id,
			(long long)NFS_FILEID(hdr->inode),
			data->args.count,
			(unsigned long long)data->args.offset);

		task->tk_status = pnfs_read_done_resend_to_mds(hdr->inode,
							&hdr->pages,
							hdr->completion_ops,
							hdr->dreq);
	}
}

static void filelayout_fenceme(struct inode *inode, struct pnfs_layout_hdr *lo)
{
	if (!test_and_clear_bit(NFS_LAYOUT_RETURN, &lo->plh_flags))
		return;
	pnfs_return_layout(inode);
}

static int filelayout_async_handle_error(struct rpc_task *task,
					 struct nfs4_state *state,
					 struct nfs_client *clp,
					 struct pnfs_layout_segment *lseg)
{
	struct pnfs_layout_hdr *lo = lseg->pls_layout;
	struct inode *inode = lo->plh_inode;
	struct nfs_server *mds_server = NFS_SERVER(inode);
	struct nfs4_deviceid_node *devid = FILELAYOUT_DEVID_NODE(lseg);
	struct nfs_client *mds_client = mds_server->nfs_client;
	struct nfs4_slot_table *tbl = &clp->cl_session->fc_slot_table;

	if (task->tk_status >= 0)
		return 0;
>>>>>>> refs/remotes/origin/master

	switch (task->tk_status) {
	/* MDS state errors */
	case -NFS4ERR_DELEG_REVOKED:
	case -NFS4ERR_ADMIN_REVOKED:
	case -NFS4ERR_BAD_STATEID:
<<<<<<< HEAD
		nfs_remove_bad_delegation(state->inode);
	case -NFS4ERR_OPENMODE:
		nfs4_schedule_stateid_recovery(mds_server, state);
		goto wait_on_recovery;
	case -NFS4ERR_EXPIRED:
		nfs4_schedule_stateid_recovery(mds_server, state);
		nfs4_schedule_lease_recovery(mds_client);
		goto wait_on_recovery;
	/* DS session errors */
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (state == NULL)
			break;
		nfs_remove_bad_delegation(state->inode);
	case -NFS4ERR_OPENMODE:
		if (state == NULL)
			break;
		if (nfs4_schedule_stateid_recovery(mds_server, state) < 0)
			goto out_bad_stateid;
		goto wait_on_recovery;
	case -NFS4ERR_EXPIRED:
		if (state != NULL) {
			if (nfs4_schedule_stateid_recovery(mds_server, state) < 0)
				goto out_bad_stateid;
		}
		nfs4_schedule_lease_recovery(mds_client);
		goto wait_on_recovery;
	/* DS session errors */
>>>>>>> refs/remotes/origin/master
	case -NFS4ERR_BADSESSION:
	case -NFS4ERR_BADSLOT:
	case -NFS4ERR_BAD_HIGH_SLOT:
	case -NFS4ERR_DEADSESSION:
	case -NFS4ERR_CONN_NOT_BOUND_TO_SESSION:
	case -NFS4ERR_SEQ_FALSE_RETRY:
	case -NFS4ERR_SEQ_MISORDERED:
		dprintk("%s ERROR %d, Reset session. Exchangeid "
			"flags 0x%x\n", __func__, task->tk_status,
			clp->cl_exchange_flags);
<<<<<<< HEAD
		nfs4_schedule_session_recovery(clp->cl_session);
		break;
	case -NFS4ERR_DELAY:
	case -NFS4ERR_GRACE:
	case -EKEYEXPIRED:
=======
		nfs4_schedule_session_recovery(clp->cl_session, task->tk_status);
		break;
	case -NFS4ERR_DELAY:
	case -NFS4ERR_GRACE:
>>>>>>> refs/remotes/origin/master
		rpc_delay(task, FILELAYOUT_POLL_RETRY_MAX);
		break;
	case -NFS4ERR_RETRY_UNCACHED_REP:
		break;
<<<<<<< HEAD
	default:
		dprintk("%s DS error. Retry through MDS %d\n", __func__,
			task->tk_status);
		*reset = 1;
		break;
	}
<<<<<<< HEAD
	task->tk_status = 0;
	return -EAGAIN;
=======
out:
	task->tk_status = 0;
	return -EAGAIN;
=======
	/* Invalidate Layout errors */
	case -NFS4ERR_PNFS_NO_LAYOUT:
	case -ESTALE:           /* mapped NFS4ERR_STALE */
	case -EBADHANDLE:       /* mapped NFS4ERR_BADHANDLE */
	case -EISDIR:           /* mapped NFS4ERR_ISDIR */
	case -NFS4ERR_FHEXPIRED:
	case -NFS4ERR_WRONG_TYPE:
		dprintk("%s Invalid layout error %d\n", __func__,
			task->tk_status);
		/*
		 * Destroy layout so new i/o will get a new layout.
		 * Layout will not be destroyed until all current lseg
		 * references are put. Mark layout as invalid to resend failed
		 * i/o and all i/o waiting on the slot table to the MDS until
		 * layout is destroyed and a new valid layout is obtained.
		 */
		pnfs_destroy_layout(NFS_I(inode));
		rpc_wake_up(&tbl->slot_tbl_waitq);
		goto reset;
	/* RPC connection errors */
	case -ECONNREFUSED:
	case -EHOSTDOWN:
	case -EHOSTUNREACH:
	case -ENETUNREACH:
	case -EIO:
	case -ETIMEDOUT:
	case -EPIPE:
		dprintk("%s DS connection error %d\n", __func__,
			task->tk_status);
		nfs4_mark_deviceid_unavailable(devid);
		set_bit(NFS_LAYOUT_RETURN, &lo->plh_flags);
		rpc_wake_up(&tbl->slot_tbl_waitq);
		/* fall through */
	default:
reset:
		dprintk("%s Retry through MDS. Error %d\n", __func__,
			task->tk_status);
		return -NFS4ERR_RESET_TO_MDS;
	}
out:
	task->tk_status = 0;
	return -EAGAIN;
out_bad_stateid:
	task->tk_status = -EIO;
	return 0;
>>>>>>> refs/remotes/origin/master
wait_on_recovery:
	rpc_sleep_on(&mds_client->cl_rpcwaitq, task, NULL);
	if (test_bit(NFS4CLNT_MANAGER_RUNNING, &mds_client->cl_state) == 0)
		rpc_wake_up_queued_task(&mds_client->cl_rpcwaitq, task);
	goto out;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

/* NFS_PROTO call done callback routines */

static int filelayout_read_done_cb(struct rpc_task *task,
				struct nfs_read_data *data)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct nfs_client *clp = data->ds_clp;
=======
>>>>>>> refs/remotes/origin/cm-10.0
	int reset = 0;

	dprintk("%s DS read\n", __func__);

	if (filelayout_async_handle_error(task, data->args.context->state,
					  data->ds_clp, &reset) == -EAGAIN) {
		dprintk("%s calling restart ds_clp %p ds_clp->cl_session %p\n",
			__func__, data->ds_clp, data->ds_clp->cl_session);
		if (reset) {
<<<<<<< HEAD
			filelayout_set_lo_fail(data->lseg);
			nfs4_reset_read(task, data);
			clp = NFS_SERVER(data->inode)->nfs_client;
		}
		nfs_restart_rpc(task, clp);
=======
			pnfs_set_lo_fail(data->lseg);
			nfs4_reset_read(task, data);
		}
		rpc_restart_call_prepare(task);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct nfs_pgio_header *hdr = data->header;
	int err;

	trace_nfs4_pnfs_read(data, task->tk_status);
	err = filelayout_async_handle_error(task, data->args.context->state,
					    data->ds_clp, hdr->lseg);

	switch (err) {
	case -NFS4ERR_RESET_TO_MDS:
		filelayout_reset_read(data);
		return task->tk_status;
	case -EAGAIN:
		rpc_restart_call_prepare(task);
>>>>>>> refs/remotes/origin/master
		return -EAGAIN;
	}

	return 0;
}

/*
 * We reference the rpc_cred of the first WRITE that triggers the need for
 * a LAYOUTCOMMIT, and use it to send the layoutcommit compound.
 * rfc5661 is not clear about which credential should be used.
 */
static void
filelayout_set_layoutcommit(struct nfs_write_data *wdata)
{
<<<<<<< HEAD
	if (FILELAYOUT_LSEG(wdata->lseg)->commit_through_mds ||
=======
	struct nfs_pgio_header *hdr = wdata->header;

	if (FILELAYOUT_LSEG(hdr->lseg)->commit_through_mds ||
>>>>>>> refs/remotes/origin/master
	    wdata->res.verf->committed == NFS_FILE_SYNC)
		return;

	pnfs_set_layoutcommit(wdata);
<<<<<<< HEAD
	dprintk("%s ionde %lu pls_end_pos %lu\n", __func__, wdata->inode->i_ino,
		(unsigned long) NFS_I(wdata->inode)->layout->plh_lwb);
=======
	dprintk("%s ionde %lu pls_end_pos %lu\n", __func__, hdr->inode->i_ino,
		(unsigned long) NFS_I(hdr->inode)->layout->plh_lwb);
}

bool
filelayout_test_devid_unavailable(struct nfs4_deviceid_node *node)
{
	return filelayout_test_devid_invalid(node) ||
		nfs4_test_deviceid_unavailable(node);
}

static bool
filelayout_reset_to_mds(struct pnfs_layout_segment *lseg)
{
	struct nfs4_deviceid_node *node = FILELAYOUT_DEVID_NODE(lseg);

	return filelayout_test_devid_unavailable(node);
>>>>>>> refs/remotes/origin/master
}

/*
 * Call ops for the async read/write cases
 * In the case of dense layouts, the offset needs to be reset to its
 * original value.
 */
static void filelayout_read_prepare(struct rpc_task *task, void *data)
{
<<<<<<< HEAD
	struct nfs_read_data *rdata = (struct nfs_read_data *)data;

	rdata->read_done_cb = filelayout_read_done_cb;

	if (nfs41_setup_sequence(rdata->ds_clp->cl_session,
				&rdata->args.seq_args, &rdata->res.seq_res,
<<<<<<< HEAD
				0, task))
=======
				task))
>>>>>>> refs/remotes/origin/cm-10.0
		return;

	rpc_call_start(task);
=======
	struct nfs_read_data *rdata = data;

	if (unlikely(test_bit(NFS_CONTEXT_BAD, &rdata->args.context->flags))) {
		rpc_exit(task, -EIO);
		return;
	}
	if (filelayout_reset_to_mds(rdata->header->lseg)) {
		dprintk("%s task %u reset io to MDS\n", __func__, task->tk_pid);
		filelayout_reset_read(rdata);
		rpc_exit(task, 0);
		return;
	}
	rdata->read_done_cb = filelayout_read_done_cb;

	if (nfs41_setup_sequence(rdata->ds_clp->cl_session,
			&rdata->args.seq_args,
			&rdata->res.seq_res,
			task))
		return;
	nfs4_set_rw_stateid(&rdata->args.stateid, rdata->args.context,
			rdata->args.lock_context, FMODE_READ);
>>>>>>> refs/remotes/origin/master
}

static void filelayout_read_call_done(struct rpc_task *task, void *data)
{
<<<<<<< HEAD
	struct nfs_read_data *rdata = (struct nfs_read_data *)data;

	dprintk("--> %s task->tk_status %d\n", __func__, task->tk_status);

	/* Note this may cause RPC to be resent */
	rdata->mds_ops->rpc_call_done(task, data);
}

<<<<<<< HEAD
=======
static void filelayout_read_count_stats(struct rpc_task *task, void *data)
{
	struct nfs_read_data *rdata = (struct nfs_read_data *)data;

	rpc_count_iostats(task, NFS_SERVER(rdata->inode)->client->cl_metrics);
}

>>>>>>> refs/remotes/origin/cm-10.0
static void filelayout_read_release(void *data)
{
	struct nfs_read_data *rdata = (struct nfs_read_data *)data;

<<<<<<< HEAD
=======
	put_lseg(rdata->lseg);
>>>>>>> refs/remotes/origin/cm-10.0
	rdata->mds_ops->rpc_release(data);
=======
	struct nfs_read_data *rdata = data;

	dprintk("--> %s task->tk_status %d\n", __func__, task->tk_status);

	if (test_bit(NFS_IOHDR_REDO, &rdata->header->flags) &&
	    task->tk_status == 0)
		return;

	/* Note this may cause RPC to be resent */
	rdata->header->mds_ops->rpc_call_done(task, data);
}

static void filelayout_read_count_stats(struct rpc_task *task, void *data)
{
	struct nfs_read_data *rdata = data;

	rpc_count_iostats(task, NFS_SERVER(rdata->header->inode)->client->cl_metrics);
}

static void filelayout_read_release(void *data)
{
	struct nfs_read_data *rdata = data;
	struct pnfs_layout_hdr *lo = rdata->header->lseg->pls_layout;

	filelayout_fenceme(lo->plh_inode, lo);
	nfs_put_client(rdata->ds_clp);
	rdata->header->mds_ops->rpc_release(data);
>>>>>>> refs/remotes/origin/master
}

static int filelayout_write_done_cb(struct rpc_task *task,
				struct nfs_write_data *data)
{
<<<<<<< HEAD
	int reset = 0;

	if (filelayout_async_handle_error(task, data->args.context->state,
					  data->ds_clp, &reset) == -EAGAIN) {
<<<<<<< HEAD
		struct nfs_client *clp;

		dprintk("%s calling restart ds_clp %p ds_clp->cl_session %p\n",
			__func__, data->ds_clp, data->ds_clp->cl_session);
		if (reset) {
			filelayout_set_lo_fail(data->lseg);
			nfs4_reset_write(task, data);
			clp = NFS_SERVER(data->inode)->nfs_client;
		} else
			clp = data->ds_clp;
		nfs_restart_rpc(task, clp);
=======
		dprintk("%s calling restart ds_clp %p ds_clp->cl_session %p\n",
			__func__, data->ds_clp, data->ds_clp->cl_session);
		if (reset) {
			pnfs_set_lo_fail(data->lseg);
			nfs4_reset_write(task, data);
		}
		rpc_restart_call_prepare(task);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct nfs_pgio_header *hdr = data->header;
	int err;

	trace_nfs4_pnfs_write(data, task->tk_status);
	err = filelayout_async_handle_error(task, data->args.context->state,
					    data->ds_clp, hdr->lseg);

	switch (err) {
	case -NFS4ERR_RESET_TO_MDS:
		filelayout_reset_write(data);
		return task->tk_status;
	case -EAGAIN:
		rpc_restart_call_prepare(task);
>>>>>>> refs/remotes/origin/master
		return -EAGAIN;
	}

	filelayout_set_layoutcommit(data);
	return 0;
}

/* Fake up some data that will cause nfs_commit_release to retry the writes. */
<<<<<<< HEAD
static void prepare_to_resend_writes(struct nfs_write_data *data)
=======
static void prepare_to_resend_writes(struct nfs_commit_data *data)
>>>>>>> refs/remotes/origin/master
{
	struct nfs_page *first = nfs_list_entry(data->pages.next);

	data->task.tk_status = 0;
<<<<<<< HEAD
	memcpy(data->verf.verifier, first->wb_verf.verifier,
	       sizeof(first->wb_verf.verifier));
	data->verf.verifier[0]++; /* ensure verifier mismatch */
}

static int filelayout_commit_done_cb(struct rpc_task *task,
				     struct nfs_write_data *data)
{
	int reset = 0;

	if (filelayout_async_handle_error(task, data->args.context->state,
					  data->ds_clp, &reset) == -EAGAIN) {
		dprintk("%s calling restart ds_clp %p ds_clp->cl_session %p\n",
			__func__, data->ds_clp, data->ds_clp->cl_session);
		if (reset) {
			prepare_to_resend_writes(data);
<<<<<<< HEAD
			filelayout_set_lo_fail(data->lseg);
		} else
			nfs_restart_rpc(task, data->ds_clp);
=======
			pnfs_set_lo_fail(data->lseg);
		} else
			rpc_restart_call_prepare(task);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	memcpy(&data->verf.verifier, &first->wb_verf,
	       sizeof(data->verf.verifier));
	data->verf.verifier.data[0]++; /* ensure verifier mismatch */
}

static int filelayout_commit_done_cb(struct rpc_task *task,
				     struct nfs_commit_data *data)
{
	int err;

	trace_nfs4_pnfs_commit_ds(data, task->tk_status);
	err = filelayout_async_handle_error(task, NULL, data->ds_clp,
					    data->lseg);

	switch (err) {
	case -NFS4ERR_RESET_TO_MDS:
		prepare_to_resend_writes(data);
		return -EAGAIN;
	case -EAGAIN:
		rpc_restart_call_prepare(task);
>>>>>>> refs/remotes/origin/master
		return -EAGAIN;
	}

	return 0;
}

static void filelayout_write_prepare(struct rpc_task *task, void *data)
{
<<<<<<< HEAD
	struct nfs_write_data *wdata = (struct nfs_write_data *)data;

	if (nfs41_setup_sequence(wdata->ds_clp->cl_session,
				&wdata->args.seq_args, &wdata->res.seq_res,
<<<<<<< HEAD
				0, task))
=======
				task))
>>>>>>> refs/remotes/origin/cm-10.0
		return;

	rpc_call_start(task);
=======
	struct nfs_write_data *wdata = data;

	if (unlikely(test_bit(NFS_CONTEXT_BAD, &wdata->args.context->flags))) {
		rpc_exit(task, -EIO);
		return;
	}
	if (filelayout_reset_to_mds(wdata->header->lseg)) {
		dprintk("%s task %u reset io to MDS\n", __func__, task->tk_pid);
		filelayout_reset_write(wdata);
		rpc_exit(task, 0);
		return;
	}
	if (nfs41_setup_sequence(wdata->ds_clp->cl_session,
			&wdata->args.seq_args,
			&wdata->res.seq_res,
			task))
		return;
	nfs4_set_rw_stateid(&wdata->args.stateid, wdata->args.context,
			wdata->args.lock_context, FMODE_WRITE);
>>>>>>> refs/remotes/origin/master
}

static void filelayout_write_call_done(struct rpc_task *task, void *data)
{
<<<<<<< HEAD
	struct nfs_write_data *wdata = (struct nfs_write_data *)data;

	/* Note this may cause RPC to be resent */
	wdata->mds_ops->rpc_call_done(task, data);
}

<<<<<<< HEAD
=======
static void filelayout_write_count_stats(struct rpc_task *task, void *data)
{
	struct nfs_write_data *wdata = (struct nfs_write_data *)data;

	rpc_count_iostats(task, NFS_SERVER(wdata->inode)->client->cl_metrics);
}

>>>>>>> refs/remotes/origin/cm-10.0
static void filelayout_write_release(void *data)
{
	struct nfs_write_data *wdata = (struct nfs_write_data *)data;

<<<<<<< HEAD
=======
	put_lseg(wdata->lseg);
>>>>>>> refs/remotes/origin/cm-10.0
	wdata->mds_ops->rpc_release(data);
}

static void filelayout_commit_release(void *data)
{
	struct nfs_write_data *wdata = (struct nfs_write_data *)data;

	nfs_commit_release_pages(wdata);
	if (atomic_dec_and_test(&NFS_I(wdata->inode)->commits_outstanding))
		nfs_commit_clear_lock(NFS_I(wdata->inode));
<<<<<<< HEAD
	nfs_commitdata_release(wdata);
}

struct rpc_call_ops filelayout_read_call_ops = {
	.rpc_call_prepare = filelayout_read_prepare,
	.rpc_call_done = filelayout_read_call_done,
	.rpc_release = filelayout_read_release,
};

struct rpc_call_ops filelayout_write_call_ops = {
	.rpc_call_prepare = filelayout_write_prepare,
	.rpc_call_done = filelayout_write_call_done,
	.rpc_release = filelayout_write_release,
};

struct rpc_call_ops filelayout_commit_call_ops = {
	.rpc_call_prepare = filelayout_write_prepare,
	.rpc_call_done = filelayout_write_call_done,
=======
	put_lseg(wdata->lseg);
	nfs_commitdata_release(wdata);
=======
	struct nfs_write_data *wdata = data;

	if (test_bit(NFS_IOHDR_REDO, &wdata->header->flags) &&
	    task->tk_status == 0)
		return;

	/* Note this may cause RPC to be resent */
	wdata->header->mds_ops->rpc_call_done(task, data);
}

static void filelayout_write_count_stats(struct rpc_task *task, void *data)
{
	struct nfs_write_data *wdata = data;

	rpc_count_iostats(task, NFS_SERVER(wdata->header->inode)->client->cl_metrics);
}

static void filelayout_write_release(void *data)
{
	struct nfs_write_data *wdata = data;
	struct pnfs_layout_hdr *lo = wdata->header->lseg->pls_layout;

	filelayout_fenceme(lo->plh_inode, lo);
	nfs_put_client(wdata->ds_clp);
	wdata->header->mds_ops->rpc_release(data);
}

static void filelayout_commit_prepare(struct rpc_task *task, void *data)
{
	struct nfs_commit_data *wdata = data;

	nfs41_setup_sequence(wdata->ds_clp->cl_session,
			&wdata->args.seq_args,
			&wdata->res.seq_res,
			task);
}

static void filelayout_write_commit_done(struct rpc_task *task, void *data)
{
	struct nfs_commit_data *wdata = data;

	/* Note this may cause RPC to be resent */
	wdata->mds_ops->rpc_call_done(task, data);
}

static void filelayout_commit_count_stats(struct rpc_task *task, void *data)
{
	struct nfs_commit_data *cdata = data;

	rpc_count_iostats(task, NFS_SERVER(cdata->inode)->client->cl_metrics);
}

static void filelayout_commit_release(void *calldata)
{
	struct nfs_commit_data *data = calldata;

	data->completion_ops->completion(data);
	pnfs_put_lseg(data->lseg);
	nfs_put_client(data->ds_clp);
	nfs_commitdata_release(data);
>>>>>>> refs/remotes/origin/master
}

static const struct rpc_call_ops filelayout_read_call_ops = {
	.rpc_call_prepare = filelayout_read_prepare,
	.rpc_call_done = filelayout_read_call_done,
	.rpc_count_stats = filelayout_read_count_stats,
	.rpc_release = filelayout_read_release,
};

static const struct rpc_call_ops filelayout_write_call_ops = {
	.rpc_call_prepare = filelayout_write_prepare,
	.rpc_call_done = filelayout_write_call_done,
	.rpc_count_stats = filelayout_write_count_stats,
	.rpc_release = filelayout_write_release,
};

static const struct rpc_call_ops filelayout_commit_call_ops = {
<<<<<<< HEAD
	.rpc_call_prepare = filelayout_write_prepare,
	.rpc_call_done = filelayout_write_call_done,
	.rpc_count_stats = filelayout_write_count_stats,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.rpc_call_prepare = filelayout_commit_prepare,
	.rpc_call_done = filelayout_write_commit_done,
	.rpc_count_stats = filelayout_commit_count_stats,
>>>>>>> refs/remotes/origin/master
	.rpc_release = filelayout_commit_release,
};

static enum pnfs_try_status
filelayout_read_pagelist(struct nfs_read_data *data)
{
<<<<<<< HEAD
	struct pnfs_layout_segment *lseg = data->lseg;
	struct nfs4_pnfs_ds *ds;
	loff_t offset = data->args.offset;
	u32 j, idx;
	struct nfs_fh *fh;
	int status;

	dprintk("--> %s ino %lu pgbase %u req %Zu@%llu\n",
		__func__, data->inode->i_ino,
		data->args.pgbase, (size_t)data->args.count, offset);

<<<<<<< HEAD
=======
	if (test_bit(NFS_DEVICEID_INVALID, &FILELAYOUT_DEVID_NODE(lseg)->flags))
		return PNFS_NOT_ATTEMPTED;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct nfs_pgio_header *hdr = data->header;
	struct pnfs_layout_segment *lseg = hdr->lseg;
	struct nfs4_pnfs_ds *ds;
	struct rpc_clnt *ds_clnt;
	loff_t offset = data->args.offset;
	u32 j, idx;
	struct nfs_fh *fh;

	dprintk("--> %s ino %lu pgbase %u req %Zu@%llu\n",
		__func__, hdr->inode->i_ino,
		data->args.pgbase, (size_t)data->args.count, offset);

>>>>>>> refs/remotes/origin/master
	/* Retrieve the correct rpc_client for the byte range */
	j = nfs4_fl_calc_j_index(lseg, offset);
	idx = nfs4_fl_calc_ds_index(lseg, j);
	ds = nfs4_fl_prepare_ds(lseg, idx);
<<<<<<< HEAD
	if (!ds) {
		/* Either layout fh index faulty, or ds connect failed */
		set_bit(lo_fail_bit(IOMODE_RW), &lseg->pls_layout->plh_flags);
		set_bit(lo_fail_bit(IOMODE_READ), &lseg->pls_layout->plh_flags);
		return PNFS_NOT_ATTEMPTED;
	}
<<<<<<< HEAD
	dprintk("%s USE DS:ip %x %hu\n", __func__,
		ntohl(ds->ds_ip_addr), ntohs(ds->ds_port));
=======
	dprintk("%s USE DS: %s\n", __func__, ds->ds_remotestr);
>>>>>>> refs/remotes/origin/cm-10.0

	/* No multipath support. Use first DS */
=======
	if (!ds)
		return PNFS_NOT_ATTEMPTED;

	ds_clnt = nfs4_find_or_create_ds_client(ds->ds_clp, hdr->inode);
	if (IS_ERR(ds_clnt))
		return PNFS_NOT_ATTEMPTED;

	dprintk("%s USE DS: %s cl_count %d\n", __func__,
		ds->ds_remotestr, atomic_read(&ds->ds_clp->cl_count));

	/* No multipath support. Use first DS */
	atomic_inc(&ds->ds_clp->cl_count);
>>>>>>> refs/remotes/origin/master
	data->ds_clp = ds->ds_clp;
	fh = nfs4_fl_select_ds_fh(lseg, j);
	if (fh)
		data->args.fh = fh;

	data->args.offset = filelayout_get_dserver_offset(lseg, offset);
	data->mds_offset = offset;

	/* Perform an asynchronous read to ds */
<<<<<<< HEAD
	status = nfs_initiate_read(data, ds->ds_clp->cl_rpcclient,
				   &filelayout_read_call_ops);
	BUG_ON(status != 0);
=======
	nfs_initiate_read(ds_clnt, data,
				  &filelayout_read_call_ops, RPC_TASK_SOFTCONN);
>>>>>>> refs/remotes/origin/master
	return PNFS_ATTEMPTED;
}

/* Perform async writes. */
static enum pnfs_try_status
filelayout_write_pagelist(struct nfs_write_data *data, int sync)
{
<<<<<<< HEAD
	struct pnfs_layout_segment *lseg = data->lseg;
	struct nfs4_pnfs_ds *ds;
	loff_t offset = data->args.offset;
	u32 j, idx;
	struct nfs_fh *fh;
	int status;

<<<<<<< HEAD
=======
	if (test_bit(NFS_DEVICEID_INVALID, &FILELAYOUT_DEVID_NODE(lseg)->flags))
		return PNFS_NOT_ATTEMPTED;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct nfs_pgio_header *hdr = data->header;
	struct pnfs_layout_segment *lseg = hdr->lseg;
	struct nfs4_pnfs_ds *ds;
	struct rpc_clnt *ds_clnt;
	loff_t offset = data->args.offset;
	u32 j, idx;
	struct nfs_fh *fh;

>>>>>>> refs/remotes/origin/master
	/* Retrieve the correct rpc_client for the byte range */
	j = nfs4_fl_calc_j_index(lseg, offset);
	idx = nfs4_fl_calc_ds_index(lseg, j);
	ds = nfs4_fl_prepare_ds(lseg, idx);
<<<<<<< HEAD
	if (!ds) {
<<<<<<< HEAD
		printk(KERN_ERR "%s: prepare_ds failed, use MDS\n", __func__);
=======
		printk(KERN_ERR "NFS: %s: prepare_ds failed, use MDS\n",
			__func__);
>>>>>>> refs/remotes/origin/cm-10.0
		set_bit(lo_fail_bit(IOMODE_RW), &lseg->pls_layout->plh_flags);
		set_bit(lo_fail_bit(IOMODE_READ), &lseg->pls_layout->plh_flags);
		return PNFS_NOT_ATTEMPTED;
	}
<<<<<<< HEAD
	dprintk("%s ino %lu sync %d req %Zu@%llu DS:%x:%hu\n", __func__,
		data->inode->i_ino, sync, (size_t) data->args.count, offset,
		ntohl(ds->ds_ip_addr), ntohs(ds->ds_port));
=======
	dprintk("%s ino %lu sync %d req %Zu@%llu DS: %s\n", __func__,
		data->inode->i_ino, sync, (size_t) data->args.count, offset,
		ds->ds_remotestr);
>>>>>>> refs/remotes/origin/cm-10.0

	data->write_done_cb = filelayout_write_done_cb;
=======
	if (!ds)
		return PNFS_NOT_ATTEMPTED;

	ds_clnt = nfs4_find_or_create_ds_client(ds->ds_clp, hdr->inode);
	if (IS_ERR(ds_clnt))
		return PNFS_NOT_ATTEMPTED;

	dprintk("%s ino %lu sync %d req %Zu@%llu DS: %s cl_count %d\n",
		__func__, hdr->inode->i_ino, sync, (size_t) data->args.count,
		offset, ds->ds_remotestr, atomic_read(&ds->ds_clp->cl_count));

	data->write_done_cb = filelayout_write_done_cb;
	atomic_inc(&ds->ds_clp->cl_count);
>>>>>>> refs/remotes/origin/master
	data->ds_clp = ds->ds_clp;
	fh = nfs4_fl_select_ds_fh(lseg, j);
	if (fh)
		data->args.fh = fh;
	/*
	 * Get the file offset on the dserver. Set the write offset to
	 * this offset and save the original offset.
	 */
	data->args.offset = filelayout_get_dserver_offset(lseg, offset);

	/* Perform an asynchronous write */
<<<<<<< HEAD
	status = nfs_initiate_write(data, ds->ds_clp->cl_rpcclient,
				    &filelayout_write_call_ops, sync);
	BUG_ON(status != 0);
=======
	nfs_initiate_write(ds_clnt, data,
				    &filelayout_write_call_ops, sync,
				    RPC_TASK_SOFTCONN);
>>>>>>> refs/remotes/origin/master
	return PNFS_ATTEMPTED;
}

/*
 * filelayout_check_layout()
 *
 * Make sure layout segment parameters are sane WRT the device.
 * At this point no generic layer initialization of the lseg has occurred,
 * and nothing has been added to the layout_hdr cache.
 *
 */
static int
filelayout_check_layout(struct pnfs_layout_hdr *lo,
			struct nfs4_filelayout_segment *fl,
			struct nfs4_layoutget_res *lgr,
			struct nfs4_deviceid *id,
			gfp_t gfp_flags)
{
	struct nfs4_deviceid_node *d;
	struct nfs4_file_layout_dsaddr *dsaddr;
	int status = -EINVAL;
	struct nfs_server *nfss = NFS_SERVER(lo->plh_inode);

	dprintk("--> %s\n", __func__);

	/* FIXME: remove this check when layout segment support is added */
	if (lgr->range.offset != 0 ||
	    lgr->range.length != NFS4_MAX_UINT64) {
		dprintk("%s Only whole file layouts supported. Use MDS i/o\n",
			__func__);
		goto out;
	}

	if (fl->pattern_offset > lgr->range.offset) {
		dprintk("%s pattern_offset %lld too large\n",
				__func__, fl->pattern_offset);
		goto out;
	}

	if (!fl->stripe_unit || fl->stripe_unit % PAGE_SIZE) {
		dprintk("%s Invalid stripe unit (%u)\n",
			__func__, fl->stripe_unit);
		goto out;
	}

	/* find and reference the deviceid */
	d = nfs4_find_get_deviceid(NFS_SERVER(lo->plh_inode)->pnfs_curr_ld,
				   NFS_SERVER(lo->plh_inode)->nfs_client, id);
	if (d == NULL) {
<<<<<<< HEAD
		dsaddr = get_device_info(lo->plh_inode, id, gfp_flags);
=======
		dsaddr = filelayout_get_device_info(lo->plh_inode, id,
				lo->plh_lc_cred, gfp_flags);
>>>>>>> refs/remotes/origin/master
		if (dsaddr == NULL)
			goto out;
	} else
		dsaddr = container_of(d, struct nfs4_file_layout_dsaddr, id_node);
<<<<<<< HEAD
<<<<<<< HEAD
	fl->dsaddr = dsaddr;

	if (fl->first_stripe_index < 0 ||
	    fl->first_stripe_index >= dsaddr->stripe_count) {
		dprintk("%s Bad first_stripe_index %d\n",
=======
	/* Found deviceid is being reaped */
	if (test_bit(NFS_DEVICEID_INVALID, &dsaddr->id_node.flags))
=======
	/* Found deviceid is unavailable */
	if (filelayout_test_devid_unavailable(&dsaddr->id_node))
>>>>>>> refs/remotes/origin/master
			goto out_put;

	fl->dsaddr = dsaddr;

	if (fl->first_stripe_index >= dsaddr->stripe_count) {
		dprintk("%s Bad first_stripe_index %u\n",
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				__func__, fl->first_stripe_index);
		goto out_put;
	}

	if ((fl->stripe_type == STRIPE_SPARSE &&
	    fl->num_fh > 1 && fl->num_fh != dsaddr->ds_num) ||
	    (fl->stripe_type == STRIPE_DENSE &&
	    fl->num_fh != dsaddr->stripe_count)) {
		dprintk("%s num_fh %u not valid for given packing\n",
			__func__, fl->num_fh);
		goto out_put;
	}

	if (fl->stripe_unit % nfss->rsize || fl->stripe_unit % nfss->wsize) {
		dprintk("%s Stripe unit (%u) not aligned with rsize %u "
			"wsize %u\n", __func__, fl->stripe_unit, nfss->rsize,
			nfss->wsize);
	}

	status = 0;
out:
	dprintk("--> %s returns %d\n", __func__, status);
	return status;
out_put:
	nfs4_fl_put_deviceid(dsaddr);
	goto out;
}

static void filelayout_free_fh_array(struct nfs4_filelayout_segment *fl)
{
	int i;

	for (i = 0; i < fl->num_fh; i++) {
		if (!fl->fh_array[i])
			break;
		kfree(fl->fh_array[i]);
	}
	kfree(fl->fh_array);
	fl->fh_array = NULL;
}

static void
_filelayout_free_lseg(struct nfs4_filelayout_segment *fl)
{
	filelayout_free_fh_array(fl);
	kfree(fl);
}

static int
filelayout_decode_layout(struct pnfs_layout_hdr *flo,
			 struct nfs4_filelayout_segment *fl,
			 struct nfs4_layoutget_res *lgr,
			 struct nfs4_deviceid *id,
			 gfp_t gfp_flags)
{
	struct xdr_stream stream;
	struct xdr_buf buf;
	struct page *scratch;
	__be32 *p;
	uint32_t nfl_util;
	int i;

	dprintk("%s: set_layout_map Begin\n", __func__);

	scratch = alloc_page(gfp_flags);
	if (!scratch)
		return -ENOMEM;

	xdr_init_decode_pages(&stream, &buf, lgr->layoutp->pages, lgr->layoutp->len);
	xdr_set_scratch_buffer(&stream, page_address(scratch), PAGE_SIZE);

	/* 20 = ufl_util (4), first_stripe_index (4), pattern_offset (8),
	 * num_fh (4) */
	p = xdr_inline_decode(&stream, NFS4_DEVICEID4_SIZE + 20);
	if (unlikely(!p))
		goto out_err;

	memcpy(id, p, sizeof(*id));
	p += XDR_QUADLEN(NFS4_DEVICEID4_SIZE);
	nfs4_print_deviceid(id);

	nfl_util = be32_to_cpup(p++);
	if (nfl_util & NFL4_UFLG_COMMIT_THRU_MDS)
		fl->commit_through_mds = 1;
	if (nfl_util & NFL4_UFLG_DENSE)
		fl->stripe_type = STRIPE_DENSE;
	else
		fl->stripe_type = STRIPE_SPARSE;
	fl->stripe_unit = nfl_util & ~NFL4_UFLG_MASK;

	fl->first_stripe_index = be32_to_cpup(p++);
	p = xdr_decode_hyper(p, &fl->pattern_offset);
	fl->num_fh = be32_to_cpup(p++);

	dprintk("%s: nfl_util 0x%X num_fh %u fsi %u po %llu\n",
		__func__, nfl_util, fl->num_fh, fl->first_stripe_index,
		fl->pattern_offset);

	/* Note that a zero value for num_fh is legal for STRIPE_SPARSE.
	 * Futher checking is done in filelayout_check_layout */
<<<<<<< HEAD
<<<<<<< HEAD
	if (fl->num_fh < 0 || fl->num_fh >
=======
	if (fl->num_fh >
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (fl->num_fh >
>>>>>>> refs/remotes/origin/master
	    max(NFS4_PNFS_MAX_STRIPE_CNT, NFS4_PNFS_MAX_MULTI_CNT))
		goto out_err;

	if (fl->num_fh > 0) {
<<<<<<< HEAD
		fl->fh_array = kzalloc(fl->num_fh * sizeof(struct nfs_fh *),
=======
		fl->fh_array = kcalloc(fl->num_fh, sizeof(fl->fh_array[0]),
>>>>>>> refs/remotes/origin/master
				       gfp_flags);
		if (!fl->fh_array)
			goto out_err;
	}

	for (i = 0; i < fl->num_fh; i++) {
		/* Do we want to use a mempool here? */
		fl->fh_array[i] = kmalloc(sizeof(struct nfs_fh), gfp_flags);
		if (!fl->fh_array[i])
			goto out_err_free;

		p = xdr_inline_decode(&stream, 4);
		if (unlikely(!p))
			goto out_err_free;
		fl->fh_array[i]->size = be32_to_cpup(p++);
		if (sizeof(struct nfs_fh) < fl->fh_array[i]->size) {
<<<<<<< HEAD
<<<<<<< HEAD
			printk(KERN_ERR "Too big fh %d received %d\n",
=======
			printk(KERN_ERR "NFS: Too big fh %d received %d\n",
>>>>>>> refs/remotes/origin/cm-10.0
=======
			printk(KERN_ERR "NFS: Too big fh %d received %d\n",
>>>>>>> refs/remotes/origin/master
			       i, fl->fh_array[i]->size);
			goto out_err_free;
		}

		p = xdr_inline_decode(&stream, fl->fh_array[i]->size);
		if (unlikely(!p))
			goto out_err_free;
		memcpy(fl->fh_array[i]->data, p, fl->fh_array[i]->size);
		dprintk("DEBUG: %s: fh len %d\n", __func__,
			fl->fh_array[i]->size);
	}

	__free_page(scratch);
	return 0;

out_err_free:
	filelayout_free_fh_array(fl);
out_err:
	__free_page(scratch);
	return -EIO;
}

static void
filelayout_free_lseg(struct pnfs_layout_segment *lseg)
{
	struct nfs4_filelayout_segment *fl = FILELAYOUT_LSEG(lseg);

	dprintk("--> %s\n", __func__);
	nfs4_fl_put_deviceid(fl->dsaddr);
<<<<<<< HEAD
	kfree(fl->commit_buckets);
	_filelayout_free_lseg(fl);
}

=======
	/* This assumes a single RW lseg */
	if (lseg->pls_range.iomode == IOMODE_RW) {
		struct nfs4_filelayout *flo;

		flo = FILELAYOUT_FROM_HDR(lseg->pls_layout);
		flo->commit_info.nbuckets = 0;
		kfree(flo->commit_info.buckets);
		flo->commit_info.buckets = NULL;
	}
	_filelayout_free_lseg(fl);
}

static int
filelayout_alloc_commit_info(struct pnfs_layout_segment *lseg,
			     struct nfs_commit_info *cinfo,
			     gfp_t gfp_flags)
{
	struct nfs4_filelayout_segment *fl = FILELAYOUT_LSEG(lseg);
	struct pnfs_commit_bucket *buckets;
	int size;

	if (fl->commit_through_mds)
		return 0;
	if (cinfo->ds->nbuckets != 0) {
		/* This assumes there is only one IOMODE_RW lseg.  What
		 * we really want to do is have a layout_hdr level
		 * dictionary of <multipath_list4, fh> keys, each
		 * associated with a struct list_head, populated by calls
		 * to filelayout_write_pagelist().
		 * */
		return 0;
	}

	size = (fl->stripe_type == STRIPE_SPARSE) ?
		fl->dsaddr->ds_num : fl->dsaddr->stripe_count;

	buckets = kcalloc(size, sizeof(struct pnfs_commit_bucket),
			  gfp_flags);
	if (!buckets)
		return -ENOMEM;
	else {
		int i;

		spin_lock(cinfo->lock);
		if (cinfo->ds->nbuckets != 0)
			kfree(buckets);
		else {
			cinfo->ds->buckets = buckets;
			cinfo->ds->nbuckets = size;
			for (i = 0; i < size; i++) {
				INIT_LIST_HEAD(&buckets[i].written);
				INIT_LIST_HEAD(&buckets[i].committing);
			}
		}
		spin_unlock(cinfo->lock);
		return 0;
	}
}

>>>>>>> refs/remotes/origin/master
static struct pnfs_layout_segment *
filelayout_alloc_lseg(struct pnfs_layout_hdr *layoutid,
		      struct nfs4_layoutget_res *lgr,
		      gfp_t gfp_flags)
{
	struct nfs4_filelayout_segment *fl;
	int rc;
	struct nfs4_deviceid id;

	dprintk("--> %s\n", __func__);
	fl = kzalloc(sizeof(*fl), gfp_flags);
	if (!fl)
		return NULL;

	rc = filelayout_decode_layout(layoutid, fl, lgr, &id, gfp_flags);
	if (rc != 0 || filelayout_check_layout(layoutid, fl, lgr, &id, gfp_flags)) {
		_filelayout_free_lseg(fl);
		return NULL;
	}
<<<<<<< HEAD

	/* This assumes there is only one IOMODE_RW lseg.  What
	 * we really want to do is have a layout_hdr level
	 * dictionary of <multipath_list4, fh> keys, each
	 * associated with a struct list_head, populated by calls
	 * to filelayout_write_pagelist().
	 * */
	if ((!fl->commit_through_mds) && (lgr->range.iomode == IOMODE_RW)) {
		int i;
		int size = (fl->stripe_type == STRIPE_SPARSE) ?
			fl->dsaddr->ds_num : fl->dsaddr->stripe_count;

<<<<<<< HEAD
		fl->commit_buckets = kcalloc(size, sizeof(struct list_head), gfp_flags);
=======
		fl->commit_buckets = kcalloc(size, sizeof(struct nfs4_fl_commit_bucket), gfp_flags);
>>>>>>> refs/remotes/origin/cm-10.0
		if (!fl->commit_buckets) {
			filelayout_free_lseg(&fl->generic_hdr);
			return NULL;
		}
		fl->number_of_buckets = size;
<<<<<<< HEAD
		for (i = 0; i < size; i++)
			INIT_LIST_HEAD(&fl->commit_buckets[i]);
=======
		for (i = 0; i < size; i++) {
			INIT_LIST_HEAD(&fl->commit_buckets[i].written);
			INIT_LIST_HEAD(&fl->commit_buckets[i].committing);
		}
>>>>>>> refs/remotes/origin/cm-10.0
	}
=======
>>>>>>> refs/remotes/origin/master
	return &fl->generic_hdr;
}

/*
 * filelayout_pg_test(). Called by nfs_can_coalesce_requests()
 *
 * return true  : coalesce page
 * return false : don't coalesce page
 */
<<<<<<< HEAD
<<<<<<< HEAD
bool
=======
static bool
>>>>>>> refs/remotes/origin/cm-10.0
=======
static bool
>>>>>>> refs/remotes/origin/master
filelayout_pg_test(struct nfs_pageio_descriptor *pgio, struct nfs_page *prev,
		   struct nfs_page *req)
{
	u64 p_stripe, r_stripe;
	u32 stripe_unit;

	if (!pnfs_generic_pg_test(pgio, prev, req) ||
	    !nfs_generic_pg_test(pgio, prev, req))
		return false;

<<<<<<< HEAD
<<<<<<< HEAD
	if (!pgio->pg_lseg)
		return 1;
=======
>>>>>>> refs/remotes/origin/cm-10.0
	p_stripe = (u64)prev->wb_index << PAGE_CACHE_SHIFT;
	r_stripe = (u64)req->wb_index << PAGE_CACHE_SHIFT;
=======
	p_stripe = (u64)req_offset(prev);
	r_stripe = (u64)req_offset(req);
>>>>>>> refs/remotes/origin/master
	stripe_unit = FILELAYOUT_LSEG(pgio->pg_lseg)->stripe_unit;

	do_div(p_stripe, stripe_unit);
	do_div(r_stripe, stripe_unit);

	return (p_stripe == r_stripe);
}

<<<<<<< HEAD
<<<<<<< HEAD
static bool filelayout_mark_pnfs_commit(struct pnfs_layout_segment *lseg)
{
	return !FILELAYOUT_LSEG(lseg)->commit_through_mds;
}

=======
=======
>>>>>>> refs/remotes/origin/master
static void
filelayout_pg_init_read(struct nfs_pageio_descriptor *pgio,
			struct nfs_page *req)
{
<<<<<<< HEAD
	BUG_ON(pgio->pg_lseg != NULL);

=======
	WARN_ON_ONCE(pgio->pg_lseg != NULL);

	if (req->wb_offset != req->wb_pgbase) {
		/*
		 * Handling unaligned pages is difficult, because have to
		 * somehow split a req in two in certain cases in the
		 * pg.test code.  Avoid this by just not using pnfs
		 * in this case.
		 */
		nfs_pageio_reset_read_mds(pgio);
		return;
	}
>>>>>>> refs/remotes/origin/master
	pgio->pg_lseg = pnfs_update_layout(pgio->pg_inode,
					   req->wb_context,
					   0,
					   NFS4_MAX_UINT64,
					   IOMODE_READ,
					   GFP_KERNEL);
	/* If no lseg, fall back to read through mds */
	if (pgio->pg_lseg == NULL)
		nfs_pageio_reset_read_mds(pgio);
}

static void
filelayout_pg_init_write(struct nfs_pageio_descriptor *pgio,
			 struct nfs_page *req)
{
<<<<<<< HEAD
	BUG_ON(pgio->pg_lseg != NULL);

=======
	struct nfs_commit_info cinfo;
	int status;

	WARN_ON_ONCE(pgio->pg_lseg != NULL);

	if (req->wb_offset != req->wb_pgbase)
		goto out_mds;
>>>>>>> refs/remotes/origin/master
	pgio->pg_lseg = pnfs_update_layout(pgio->pg_inode,
					   req->wb_context,
					   0,
					   NFS4_MAX_UINT64,
					   IOMODE_RW,
					   GFP_NOFS);
	/* If no lseg, fall back to write through mds */
	if (pgio->pg_lseg == NULL)
<<<<<<< HEAD
		nfs_pageio_reset_write_mds(pgio);
=======
		goto out_mds;
	nfs_init_cinfo(&cinfo, pgio->pg_inode, pgio->pg_dreq);
	status = filelayout_alloc_commit_info(pgio->pg_lseg, &cinfo, GFP_NOFS);
	if (status < 0) {
		pnfs_put_lseg(pgio->pg_lseg);
		pgio->pg_lseg = NULL;
		goto out_mds;
	}
	return;
out_mds:
	nfs_pageio_reset_write_mds(pgio);
>>>>>>> refs/remotes/origin/master
}

static const struct nfs_pageio_ops filelayout_pg_read_ops = {
	.pg_init = filelayout_pg_init_read,
	.pg_test = filelayout_pg_test,
	.pg_doio = pnfs_generic_pg_readpages,
};

static const struct nfs_pageio_ops filelayout_pg_write_ops = {
	.pg_init = filelayout_pg_init_write,
	.pg_test = filelayout_pg_test,
	.pg_doio = pnfs_generic_pg_writepages,
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static u32 select_bucket_index(struct nfs4_filelayout_segment *fl, u32 j)
{
	if (fl->stripe_type == STRIPE_SPARSE)
		return nfs4_fl_calc_ds_index(&fl->generic_hdr, j);
	else
		return j;
}

<<<<<<< HEAD
<<<<<<< HEAD
struct list_head *filelayout_choose_commit_list(struct nfs_page *req)
{
	struct pnfs_layout_segment *lseg = req->wb_commit_lseg;
=======
=======
>>>>>>> refs/remotes/origin/master
/* The generic layer is about to remove the req from the commit list.
 * If this will make the bucket empty, it will need to put the lseg reference.
 */
static void
<<<<<<< HEAD
filelayout_clear_request_commit(struct nfs_page *req)
{
	struct pnfs_layout_segment *freeme = NULL;
	struct inode *inode = req->wb_context->dentry->d_inode;

	spin_lock(&inode->i_lock);
	if (!test_and_clear_bit(PG_COMMIT_TO_DS, &req->wb_flags))
		goto out;
	if (list_is_singular(&req->wb_list)) {
		struct pnfs_layout_segment *lseg;

		/* From here we can find the bucket, but for the moment,
		 * since there is only one relevant lseg...
		 */
		list_for_each_entry(lseg, &NFS_I(inode)->layout->plh_segs, pls_list) {
			if (lseg->pls_range.iomode == IOMODE_RW) {
				freeme = lseg;
				break;
			}
		}
	}
out:
	nfs_request_remove_commit_list(req);
	spin_unlock(&inode->i_lock);
	put_lseg(freeme);
=======
filelayout_clear_request_commit(struct nfs_page *req,
				struct nfs_commit_info *cinfo)
{
	struct pnfs_layout_segment *freeme = NULL;

	spin_lock(cinfo->lock);
	if (!test_and_clear_bit(PG_COMMIT_TO_DS, &req->wb_flags))
		goto out;
	cinfo->ds->nwritten--;
	if (list_is_singular(&req->wb_list)) {
		struct pnfs_commit_bucket *bucket;

		bucket = list_first_entry(&req->wb_list,
					  struct pnfs_commit_bucket,
					  written);
		freeme = bucket->wlseg;
		bucket->wlseg = NULL;
	}
out:
	nfs_request_remove_commit_list(req, cinfo);
	spin_unlock(cinfo->lock);
	pnfs_put_lseg(freeme);
>>>>>>> refs/remotes/origin/master
}

static struct list_head *
filelayout_choose_commit_list(struct nfs_page *req,
<<<<<<< HEAD
			      struct pnfs_layout_segment *lseg)
{
>>>>>>> refs/remotes/origin/cm-10.0
	struct nfs4_filelayout_segment *fl = FILELAYOUT_LSEG(lseg);
	u32 i, j;
	struct list_head *list;

<<<<<<< HEAD
=======
	if (fl->commit_through_mds)
		return &NFS_I(req->wb_context->dentry->d_inode)->commit_list;

>>>>>>> refs/remotes/origin/cm-10.0
=======
			      struct pnfs_layout_segment *lseg,
			      struct nfs_commit_info *cinfo)
{
	struct nfs4_filelayout_segment *fl = FILELAYOUT_LSEG(lseg);
	u32 i, j;
	struct list_head *list;
	struct pnfs_commit_bucket *buckets;

	if (fl->commit_through_mds)
		return &cinfo->mds->list;

>>>>>>> refs/remotes/origin/master
	/* Note that we are calling nfs4_fl_calc_j_index on each page
	 * that ends up being committed to a data server.  An attractive
	 * alternative is to add a field to nfs_write_data and nfs_page
	 * to store the value calculated in filelayout_write_pagelist
	 * and just use that here.
	 */
<<<<<<< HEAD
	j = nfs4_fl_calc_j_index(lseg,
				 (loff_t)req->wb_index << PAGE_CACHE_SHIFT);
	i = select_bucket_index(fl, j);
<<<<<<< HEAD
	list = &fl->commit_buckets[i];
	if (list_empty(list)) {
		/* Non-empty buckets hold a reference on the lseg */
		get_lseg(lseg);
	}
	return list;
}

=======
	list = &fl->commit_buckets[i].written;
=======
	j = nfs4_fl_calc_j_index(lseg, req_offset(req));
	i = select_bucket_index(fl, j);
	buckets = cinfo->ds->buckets;
	list = &buckets[i].written;
>>>>>>> refs/remotes/origin/master
	if (list_empty(list)) {
		/* Non-empty buckets hold a reference on the lseg.  That ref
		 * is normally transferred to the COMMIT call and released
		 * there.  It could also be released if the last req is pulled
		 * off due to a rewrite, in which case it will be done in
<<<<<<< HEAD
		 * filelayout_remove_commit_req
		 */
		get_lseg(lseg);
	}
	set_bit(PG_COMMIT_TO_DS, &req->wb_flags);
=======
		 * filelayout_clear_request_commit
		 */
		buckets[i].wlseg = pnfs_get_lseg(lseg);
	}
	set_bit(PG_COMMIT_TO_DS, &req->wb_flags);
	cinfo->ds->nwritten++;
>>>>>>> refs/remotes/origin/master
	return list;
}

static void
filelayout_mark_request_commit(struct nfs_page *req,
<<<<<<< HEAD
		struct pnfs_layout_segment *lseg)
{
	struct list_head *list;

	list = filelayout_choose_commit_list(req, lseg);
	nfs_request_add_commit_list(req, list);
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
			       struct pnfs_layout_segment *lseg,
			       struct nfs_commit_info *cinfo)
{
	struct list_head *list;

	list = filelayout_choose_commit_list(req, lseg, cinfo);
	nfs_request_add_commit_list(req, list, cinfo);
}

>>>>>>> refs/remotes/origin/master
static u32 calc_ds_index_from_commit(struct pnfs_layout_segment *lseg, u32 i)
{
	struct nfs4_filelayout_segment *flseg = FILELAYOUT_LSEG(lseg);

	if (flseg->stripe_type == STRIPE_SPARSE)
		return i;
	else
		return nfs4_fl_calc_ds_index(lseg, i);
}

static struct nfs_fh *
select_ds_fh_from_commit(struct pnfs_layout_segment *lseg, u32 i)
{
	struct nfs4_filelayout_segment *flseg = FILELAYOUT_LSEG(lseg);

	if (flseg->stripe_type == STRIPE_SPARSE) {
		if (flseg->num_fh == 1)
			i = 0;
		else if (flseg->num_fh == 0)
			/* Use the MDS OPEN fh set in nfs_read_rpcsetup */
			return NULL;
	}
	return flseg->fh_array[i];
}

<<<<<<< HEAD
static int filelayout_initiate_commit(struct nfs_write_data *data, int how)
{
	struct pnfs_layout_segment *lseg = data->lseg;
	struct nfs4_pnfs_ds *ds;
=======
static int filelayout_initiate_commit(struct nfs_commit_data *data, int how)
{
	struct pnfs_layout_segment *lseg = data->lseg;
	struct nfs4_pnfs_ds *ds;
	struct rpc_clnt *ds_clnt;
>>>>>>> refs/remotes/origin/master
	u32 idx;
	struct nfs_fh *fh;

	idx = calc_ds_index_from_commit(lseg, data->ds_commit_index);
	ds = nfs4_fl_prepare_ds(lseg, idx);
<<<<<<< HEAD
	if (!ds) {
<<<<<<< HEAD
		printk(KERN_ERR "%s: prepare_ds failed, use MDS\n", __func__);
		set_bit(lo_fail_bit(IOMODE_RW), &lseg->pls_layout->plh_flags);
		set_bit(lo_fail_bit(IOMODE_READ), &lseg->pls_layout->plh_flags);
		prepare_to_resend_writes(data);
		data->mds_ops->rpc_release(data);
=======
		printk(KERN_ERR "NFS: %s: prepare_ds failed, use MDS\n",
			__func__);
		set_bit(lo_fail_bit(IOMODE_RW), &lseg->pls_layout->plh_flags);
		set_bit(lo_fail_bit(IOMODE_READ), &lseg->pls_layout->plh_flags);
		prepare_to_resend_writes(data);
		filelayout_commit_release(data);
>>>>>>> refs/remotes/origin/cm-10.0
		return -EAGAIN;
	}
	dprintk("%s ino %lu, how %d\n", __func__, data->inode->i_ino, how);
	data->write_done_cb = filelayout_commit_done_cb;
=======
	if (!ds)
		goto out_err;

	ds_clnt = nfs4_find_or_create_ds_client(ds->ds_clp, data->inode);
	if (IS_ERR(ds_clnt))
		goto out_err;

	dprintk("%s ino %lu, how %d cl_count %d\n", __func__,
		data->inode->i_ino, how, atomic_read(&ds->ds_clp->cl_count));
	data->commit_done_cb = filelayout_commit_done_cb;
	atomic_inc(&ds->ds_clp->cl_count);
>>>>>>> refs/remotes/origin/master
	data->ds_clp = ds->ds_clp;
	fh = select_ds_fh_from_commit(lseg, data->ds_commit_index);
	if (fh)
		data->args.fh = fh;
<<<<<<< HEAD
	return nfs_initiate_commit(data, ds->ds_clp->cl_rpcclient,
				   &filelayout_commit_call_ops, how);
}

/*
 * This is only useful while we are using whole file layouts.
 */
<<<<<<< HEAD
static struct pnfs_layout_segment *find_only_write_lseg(struct inode *inode)
{
	struct pnfs_layout_segment *lseg, *rv = NULL;

	spin_lock(&inode->i_lock);
	list_for_each_entry(lseg, &NFS_I(inode)->layout->plh_segs, pls_list)
		if (lseg->pls_range.iomode == IOMODE_RW)
			rv = get_lseg(lseg);
=======
static struct pnfs_layout_segment *
find_only_write_lseg_locked(struct inode *inode)
{
	struct pnfs_layout_segment *lseg;

	list_for_each_entry(lseg, &NFS_I(inode)->layout->plh_segs, pls_list)
		if (lseg->pls_range.iomode == IOMODE_RW)
			return lseg;
	return NULL;
}

static struct pnfs_layout_segment *find_only_write_lseg(struct inode *inode)
{
	struct pnfs_layout_segment *rv;

	spin_lock(&inode->i_lock);
	rv = find_only_write_lseg_locked(inode);
	if (rv)
		get_lseg(rv);
>>>>>>> refs/remotes/origin/cm-10.0
	spin_unlock(&inode->i_lock);
	return rv;
}

<<<<<<< HEAD
static int alloc_ds_commits(struct inode *inode, struct list_head *list)
=======
static int
filelayout_scan_ds_commit_list(struct nfs4_fl_commit_bucket *bucket, int max,
		spinlock_t *lock)
{
	struct list_head *src = &bucket->written;
	struct list_head *dst = &bucket->committing;
=======
	return nfs_initiate_commit(ds_clnt, data,
				   &filelayout_commit_call_ops, how,
				   RPC_TASK_SOFTCONN);
out_err:
	prepare_to_resend_writes(data);
	filelayout_commit_release(data);
	return -EAGAIN;
}

static int
transfer_commit_list(struct list_head *src, struct list_head *dst,
		     struct nfs_commit_info *cinfo, int max)
{
>>>>>>> refs/remotes/origin/master
	struct nfs_page *req, *tmp;
	int ret = 0;

	list_for_each_entry_safe(req, tmp, src, wb_list) {
		if (!nfs_lock_request(req))
			continue;
<<<<<<< HEAD
		if (cond_resched_lock(lock))
			list_safe_reset_next(req, tmp, wb_list);
		nfs_request_remove_commit_list(req);
		clear_bit(PG_COMMIT_TO_DS, &req->wb_flags);
		nfs_list_add_request(req, dst);
		ret++;
		if (ret == max)
=======
		kref_get(&req->wb_kref);
		if (cond_resched_lock(cinfo->lock))
			list_safe_reset_next(req, tmp, wb_list);
		nfs_request_remove_commit_list(req, cinfo);
		clear_bit(PG_COMMIT_TO_DS, &req->wb_flags);
		nfs_list_add_request(req, dst);
		ret++;
		if ((ret == max) && !cinfo->dreq)
>>>>>>> refs/remotes/origin/master
			break;
	}
	return ret;
}

<<<<<<< HEAD
/* Move reqs from written to committing lists, returning count of number moved.
 * Note called with i_lock held.
 */
static int filelayout_scan_commit_lists(struct inode *inode, int max,
		spinlock_t *lock)
{
	struct pnfs_layout_segment *lseg;
	struct nfs4_filelayout_segment *fl;
	int i, rv = 0, cnt;

	lseg = find_only_write_lseg_locked(inode);
	if (!lseg)
		goto out_done;
	fl = FILELAYOUT_LSEG(lseg);
	if (fl->commit_through_mds)
		goto out_done;
	for (i = 0; i < fl->number_of_buckets && max != 0; i++) {
		cnt = filelayout_scan_ds_commit_list(&fl->commit_buckets[i],
				max, lock);
		max -= cnt;
		rv += cnt;
	}
out_done:
	return rv;
}

static unsigned int
alloc_ds_commits(struct inode *inode, struct list_head *list)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct pnfs_layout_segment *lseg;
	struct nfs4_filelayout_segment *fl;
	struct nfs_write_data *data;
	int i, j;
<<<<<<< HEAD
=======
	unsigned int nreq = 0;
>>>>>>> refs/remotes/origin/cm-10.0

	/* Won't need this when non-whole file layout segments are supported
	 * instead we will use a pnfs_layout_hdr structure */
	lseg = find_only_write_lseg(inode);
	if (!lseg)
		return 0;
	fl = FILELAYOUT_LSEG(lseg);
	for (i = 0; i < fl->number_of_buckets; i++) {
<<<<<<< HEAD
		if (list_empty(&fl->commit_buckets[i]))
			continue;
		data = nfs_commitdata_alloc();
		if (!data)
			goto out_bad;
		data->ds_commit_index = i;
		data->lseg = lseg;
		list_add(&data->pages, list);
	}
	put_lseg(lseg);
	return 0;

out_bad:
	for (j = i; j < fl->number_of_buckets; j++) {
		if (list_empty(&fl->commit_buckets[i]))
			continue;
		nfs_retry_commit(&fl->commit_buckets[i], lseg);
=======
		if (list_empty(&fl->commit_buckets[i].committing))
=======
static int
filelayout_scan_ds_commit_list(struct pnfs_commit_bucket *bucket,
			       struct nfs_commit_info *cinfo,
			       int max)
{
	struct list_head *src = &bucket->written;
	struct list_head *dst = &bucket->committing;
	int ret;

	ret = transfer_commit_list(src, dst, cinfo, max);
	if (ret) {
		cinfo->ds->nwritten -= ret;
		cinfo->ds->ncommitting += ret;
		bucket->clseg = bucket->wlseg;
		if (list_empty(src))
			bucket->wlseg = NULL;
		else
			pnfs_get_lseg(bucket->clseg);
	}
	return ret;
}

/* Move reqs from written to committing lists, returning count of number moved.
 * Note called with cinfo->lock held.
 */
static int filelayout_scan_commit_lists(struct nfs_commit_info *cinfo,
					int max)
{
	int i, rv = 0, cnt;

	for (i = 0; i < cinfo->ds->nbuckets && max != 0; i++) {
		cnt = filelayout_scan_ds_commit_list(&cinfo->ds->buckets[i],
						     cinfo, max);
		max -= cnt;
		rv += cnt;
	}
	return rv;
}

/* Pull everything off the committing lists and dump into @dst */
static void filelayout_recover_commit_reqs(struct list_head *dst,
					   struct nfs_commit_info *cinfo)
{
	struct pnfs_commit_bucket *b;
	int i;

	/* NOTE cinfo->lock is NOT held, relying on fact that this is
	 * only called on single thread per dreq.
	 * Can't take the lock because need to do pnfs_put_lseg
	 */
	for (i = 0, b = cinfo->ds->buckets; i < cinfo->ds->nbuckets; i++, b++) {
		if (transfer_commit_list(&b->written, dst, cinfo, 0)) {
			pnfs_put_lseg(b->wlseg);
			b->wlseg = NULL;
		}
	}
	cinfo->ds->nwritten = 0;
}

static unsigned int
alloc_ds_commits(struct nfs_commit_info *cinfo, struct list_head *list)
{
	struct pnfs_ds_commit_info *fl_cinfo;
	struct pnfs_commit_bucket *bucket;
	struct nfs_commit_data *data;
	int i, j;
	unsigned int nreq = 0;

	fl_cinfo = cinfo->ds;
	bucket = fl_cinfo->buckets;
	for (i = 0; i < fl_cinfo->nbuckets; i++, bucket++) {
		if (list_empty(&bucket->committing))
>>>>>>> refs/remotes/origin/master
			continue;
		data = nfs_commitdata_alloc();
		if (!data)
			break;
		data->ds_commit_index = i;
<<<<<<< HEAD
		data->lseg = lseg;
=======
		data->lseg = bucket->clseg;
		bucket->clseg = NULL;
>>>>>>> refs/remotes/origin/master
		list_add(&data->pages, list);
		nreq++;
	}

	/* Clean up on error */
<<<<<<< HEAD
	for (j = i; j < fl->number_of_buckets; j++) {
		if (list_empty(&fl->commit_buckets[i].committing))
			continue;
		nfs_retry_commit(&fl->commit_buckets[i].committing, lseg);
>>>>>>> refs/remotes/origin/cm-10.0
		put_lseg(lseg);  /* associated with emptying bucket */
	}
	put_lseg(lseg);
	/* Caller will clean up entries put on list */
<<<<<<< HEAD
	return -ENOMEM;
=======
	return nreq;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	for (j = i; j < fl_cinfo->nbuckets; j++, bucket++) {
		if (list_empty(&bucket->committing))
			continue;
		nfs_retry_commit(&bucket->committing, bucket->clseg, cinfo);
		pnfs_put_lseg(bucket->clseg);
		bucket->clseg = NULL;
	}
	/* Caller will clean up entries put on list */
	return nreq;
>>>>>>> refs/remotes/origin/master
}

/* This follows nfs_commit_list pretty closely */
static int
filelayout_commit_pagelist(struct inode *inode, struct list_head *mds_pages,
<<<<<<< HEAD
			   int how)
{
	struct nfs_write_data	*data, *tmp;
	LIST_HEAD(list);
<<<<<<< HEAD

	if (!list_empty(mds_pages)) {
		data = nfs_commitdata_alloc();
		if (!data)
			goto out_bad;
		data->lseg = NULL;
		list_add(&data->pages, &list);
	}

	if (alloc_ds_commits(inode, &list))
		goto out_bad;

	list_for_each_entry_safe(data, tmp, &list, pages) {
		list_del_init(&data->pages);
		atomic_inc(&NFS_I(inode)->commits_outstanding);
=======
=======
			   int how, struct nfs_commit_info *cinfo)
{
	struct nfs_commit_data *data, *tmp;
	LIST_HEAD(list);
>>>>>>> refs/remotes/origin/master
	unsigned int nreq = 0;

	if (!list_empty(mds_pages)) {
		data = nfs_commitdata_alloc();
		if (data != NULL) {
			data->lseg = NULL;
			list_add(&data->pages, &list);
			nreq++;
		} else
<<<<<<< HEAD
			nfs_retry_commit(mds_pages, NULL);
	}

	nreq += alloc_ds_commits(inode, &list);

	if (nreq == 0) {
		nfs_commit_clear_lock(NFS_I(inode));
		goto out;
	}

	atomic_add(nreq, &NFS_I(inode)->commits_outstanding);

	list_for_each_entry_safe(data, tmp, &list, pages) {
		list_del_init(&data->pages);
>>>>>>> refs/remotes/origin/cm-10.0
		if (!data->lseg) {
			nfs_init_commit(data, mds_pages, NULL);
			nfs_initiate_commit(data, NFS_CLIENT(inode),
					    data->mds_ops, how);
		} else {
<<<<<<< HEAD
			nfs_init_commit(data, &FILELAYOUT_LSEG(data->lseg)->commit_buckets[data->ds_commit_index], data->lseg);
			filelayout_initiate_commit(data, how);
		}
	}
	return 0;
 out_bad:
	list_for_each_entry_safe(data, tmp, &list, pages) {
		nfs_retry_commit(&data->pages, data->lseg);
		list_del_init(&data->pages);
		nfs_commit_free(data);
	}
	nfs_retry_commit(mds_pages, NULL);
	nfs_commit_clear_lock(NFS_I(inode));
	return -ENOMEM;
=======
			nfs_init_commit(data, &FILELAYOUT_LSEG(data->lseg)->commit_buckets[data->ds_commit_index].committing, data->lseg);
=======
			nfs_retry_commit(mds_pages, NULL, cinfo);
	}

	nreq += alloc_ds_commits(cinfo, &list);

	if (nreq == 0) {
		cinfo->completion_ops->error_cleanup(NFS_I(inode));
		goto out;
	}

	atomic_add(nreq, &cinfo->mds->rpcs_out);

	list_for_each_entry_safe(data, tmp, &list, pages) {
		list_del_init(&data->pages);
		if (!data->lseg) {
			nfs_init_commit(data, mds_pages, NULL, cinfo);
			nfs_initiate_commit(NFS_CLIENT(inode), data,
					    data->mds_ops, how, 0);
		} else {
			struct pnfs_commit_bucket *buckets;

			buckets = cinfo->ds->buckets;
			nfs_init_commit(data, &buckets[data->ds_commit_index].committing, data->lseg, cinfo);
>>>>>>> refs/remotes/origin/master
			filelayout_initiate_commit(data, how);
		}
	}
out:
<<<<<<< HEAD
	return PNFS_ATTEMPTED;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	cinfo->ds->ncommitting = 0;
	return PNFS_ATTEMPTED;
>>>>>>> refs/remotes/origin/master
}

static void
filelayout_free_deveiceid_node(struct nfs4_deviceid_node *d)
{
	nfs4_fl_free_deviceid(container_of(d, struct nfs4_file_layout_dsaddr, id_node));
}

<<<<<<< HEAD
=======
static struct pnfs_layout_hdr *
filelayout_alloc_layout_hdr(struct inode *inode, gfp_t gfp_flags)
{
	struct nfs4_filelayout *flo;

	flo = kzalloc(sizeof(*flo), gfp_flags);
	return &flo->generic_hdr;
}

static void
filelayout_free_layout_hdr(struct pnfs_layout_hdr *lo)
{
	kfree(FILELAYOUT_FROM_HDR(lo));
}

static struct pnfs_ds_commit_info *
filelayout_get_ds_info(struct inode *inode)
{
	struct pnfs_layout_hdr *layout = NFS_I(inode)->layout;

	if (layout == NULL)
		return NULL;
	else
		return &FILELAYOUT_FROM_HDR(layout)->commit_info;
}

>>>>>>> refs/remotes/origin/master
static struct pnfs_layoutdriver_type filelayout_type = {
	.id			= LAYOUT_NFSV4_1_FILES,
	.name			= "LAYOUT_NFSV4_1_FILES",
	.owner			= THIS_MODULE,
<<<<<<< HEAD
	.alloc_lseg		= filelayout_alloc_lseg,
	.free_lseg		= filelayout_free_lseg,
<<<<<<< HEAD
	.pg_test		= filelayout_pg_test,
	.mark_pnfs_commit	= filelayout_mark_pnfs_commit,
	.choose_commit_list	= filelayout_choose_commit_list,
=======
	.pg_read_ops		= &filelayout_pg_read_ops,
	.pg_write_ops		= &filelayout_pg_write_ops,
	.mark_request_commit	= filelayout_mark_request_commit,
	.clear_request_commit	= filelayout_clear_request_commit,
	.scan_commit_lists	= filelayout_scan_commit_lists,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.alloc_layout_hdr	= filelayout_alloc_layout_hdr,
	.free_layout_hdr	= filelayout_free_layout_hdr,
	.alloc_lseg		= filelayout_alloc_lseg,
	.free_lseg		= filelayout_free_lseg,
	.pg_read_ops		= &filelayout_pg_read_ops,
	.pg_write_ops		= &filelayout_pg_write_ops,
	.get_ds_info		= &filelayout_get_ds_info,
	.mark_request_commit	= filelayout_mark_request_commit,
	.clear_request_commit	= filelayout_clear_request_commit,
	.scan_commit_lists	= filelayout_scan_commit_lists,
	.recover_commit_reqs	= filelayout_recover_commit_reqs,
>>>>>>> refs/remotes/origin/master
	.commit_pagelist	= filelayout_commit_pagelist,
	.read_pagelist		= filelayout_read_pagelist,
	.write_pagelist		= filelayout_write_pagelist,
	.free_deviceid_node	= filelayout_free_deveiceid_node,
};

static int __init nfs4filelayout_init(void)
{
	printk(KERN_INFO "%s: NFSv4 File Layout Driver Registering...\n",
	       __func__);
	return pnfs_register_layoutdriver(&filelayout_type);
}

static void __exit nfs4filelayout_exit(void)
{
	printk(KERN_INFO "%s: NFSv4 File Layout Driver Unregistering...\n",
	       __func__);
	pnfs_unregister_layoutdriver(&filelayout_type);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
MODULE_ALIAS("nfs-layouttype4-1");

>>>>>>> refs/remotes/origin/cm-10.0
=======
MODULE_ALIAS("nfs-layouttype4-1");

>>>>>>> refs/remotes/origin/master
module_init(nfs4filelayout_init);
module_exit(nfs4filelayout_exit);
