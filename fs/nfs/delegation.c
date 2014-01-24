/*
 * linux/fs/nfs/delegation.c
 *
 * Copyright (C) 2004 Trond Myklebust
 *
 * NFS file delegation management
 *
 */
#include <linux/completion.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/spinlock.h>

#include <linux/nfs4.h>
#include <linux/nfs_fs.h>
#include <linux/nfs_xdr.h>

#include "nfs4_fs.h"
#include "delegation.h"
#include "internal.h"
<<<<<<< HEAD
=======
#include "nfs4trace.h"
>>>>>>> refs/remotes/origin/master

static void nfs_free_delegation(struct nfs_delegation *delegation)
{
	if (delegation->cred) {
		put_rpccred(delegation->cred);
		delegation->cred = NULL;
	}
	kfree_rcu(delegation, rcu);
}

/**
 * nfs_mark_delegation_referenced - set delegation's REFERENCED flag
 * @delegation: delegation to process
 *
 */
void nfs_mark_delegation_referenced(struct nfs_delegation *delegation)
{
	set_bit(NFS_DELEGATION_REFERENCED, &delegation->flags);
}

/**
 * nfs_have_delegation - check if inode has a delegation
 * @inode: inode to check
 * @flags: delegation types to check for
 *
 * Returns one if inode has the indicated delegation, otherwise zero.
 */
<<<<<<< HEAD
int nfs_have_delegation(struct inode *inode, fmode_t flags)
=======
int nfs4_have_delegation(struct inode *inode, fmode_t flags)
>>>>>>> refs/remotes/origin/master
{
	struct nfs_delegation *delegation;
	int ret = 0;

	flags &= FMODE_READ|FMODE_WRITE;
	rcu_read_lock();
	delegation = rcu_dereference(NFS_I(inode)->delegation);
<<<<<<< HEAD
	if (delegation != NULL && (delegation->type & flags) == flags) {
=======
	if (delegation != NULL && (delegation->type & flags) == flags &&
	    !test_bit(NFS_DELEGATION_RETURNING, &delegation->flags)) {
>>>>>>> refs/remotes/origin/master
		nfs_mark_delegation_referenced(delegation);
		ret = 1;
	}
	rcu_read_unlock();
	return ret;
}

<<<<<<< HEAD
static int nfs_delegation_claim_locks(struct nfs_open_context *ctx, struct nfs4_state *state)
=======
static int nfs_delegation_claim_locks(struct nfs_open_context *ctx, struct nfs4_state *state, const nfs4_stateid *stateid)
>>>>>>> refs/remotes/origin/master
{
	struct inode *inode = state->inode;
	struct file_lock *fl;
	int status = 0;

	if (inode->i_flock == NULL)
		goto out;

<<<<<<< HEAD
	/* Protect inode->i_flock using the file locks lock */
	lock_flocks();
=======
	/* Protect inode->i_flock using the i_lock */
	spin_lock(&inode->i_lock);
>>>>>>> refs/remotes/origin/master
	for (fl = inode->i_flock; fl != NULL; fl = fl->fl_next) {
		if (!(fl->fl_flags & (FL_POSIX|FL_FLOCK)))
			continue;
		if (nfs_file_open_context(fl->fl_file) != ctx)
			continue;
<<<<<<< HEAD
		unlock_flocks();
		status = nfs4_lock_delegation_recall(state, fl);
		if (status < 0)
			goto out;
		lock_flocks();
	}
	unlock_flocks();
=======
		spin_unlock(&inode->i_lock);
		status = nfs4_lock_delegation_recall(fl, state, stateid);
		if (status < 0)
			goto out;
		spin_lock(&inode->i_lock);
	}
	spin_unlock(&inode->i_lock);
>>>>>>> refs/remotes/origin/master
out:
	return status;
}

static int nfs_delegation_claim_opens(struct inode *inode, const nfs4_stateid *stateid)
{
	struct nfs_inode *nfsi = NFS_I(inode);
	struct nfs_open_context *ctx;
<<<<<<< HEAD
	struct nfs4_state *state;
=======
	struct nfs4_state_owner *sp;
	struct nfs4_state *state;
	unsigned int seq;
>>>>>>> refs/remotes/origin/master
	int err;

again:
	spin_lock(&inode->i_lock);
	list_for_each_entry(ctx, &nfsi->open_files, list) {
		state = ctx->state;
		if (state == NULL)
			continue;
		if (!test_bit(NFS_DELEGATED_STATE, &state->flags))
			continue;
<<<<<<< HEAD
<<<<<<< HEAD
		if (memcmp(state->stateid.data, stateid->data, sizeof(state->stateid.data)) != 0)
=======
		if (!nfs4_stateid_match(&state->stateid, stateid))
>>>>>>> refs/remotes/origin/cm-10.0
			continue;
		get_nfs_open_context(ctx);
		spin_unlock(&inode->i_lock);
		err = nfs4_open_delegation_recall(ctx, state, stateid);
		if (err >= 0)
			err = nfs_delegation_claim_locks(ctx, state);
=======
		if (!nfs4_stateid_match(&state->stateid, stateid))
			continue;
		get_nfs_open_context(ctx);
		spin_unlock(&inode->i_lock);
		sp = state->owner;
		/* Block nfs4_proc_unlck */
		mutex_lock(&sp->so_delegreturn_mutex);
		seq = raw_seqcount_begin(&sp->so_reclaim_seqcount);
		err = nfs4_open_delegation_recall(ctx, state, stateid);
		if (!err)
			err = nfs_delegation_claim_locks(ctx, state, stateid);
		if (!err && read_seqcount_retry(&sp->so_reclaim_seqcount, seq))
			err = -EAGAIN;
		mutex_unlock(&sp->so_delegreturn_mutex);
>>>>>>> refs/remotes/origin/master
		put_nfs_open_context(ctx);
		if (err != 0)
			return err;
		goto again;
	}
	spin_unlock(&inode->i_lock);
	return 0;
}

/**
 * nfs_inode_reclaim_delegation - process a delegation reclaim request
 * @inode: inode to process
 * @cred: credential to use for request
 * @res: new delegation state from server
 *
 */
void nfs_inode_reclaim_delegation(struct inode *inode, struct rpc_cred *cred,
				  struct nfs_openres *res)
{
	struct nfs_delegation *delegation;
	struct rpc_cred *oldcred = NULL;

	rcu_read_lock();
	delegation = rcu_dereference(NFS_I(inode)->delegation);
	if (delegation != NULL) {
		spin_lock(&delegation->lock);
		if (delegation->inode != NULL) {
<<<<<<< HEAD
<<<<<<< HEAD
			memcpy(delegation->stateid.data, res->delegation.data,
			       sizeof(delegation->stateid.data));
=======
			nfs4_stateid_copy(&delegation->stateid, &res->delegation);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			nfs4_stateid_copy(&delegation->stateid, &res->delegation);
>>>>>>> refs/remotes/origin/master
			delegation->type = res->delegation_type;
			delegation->maxsize = res->maxsize;
			oldcred = delegation->cred;
			delegation->cred = get_rpccred(cred);
			clear_bit(NFS_DELEGATION_NEED_RECLAIM,
				  &delegation->flags);
			NFS_I(inode)->delegation_state = delegation->type;
			spin_unlock(&delegation->lock);
			put_rpccred(oldcred);
			rcu_read_unlock();
<<<<<<< HEAD
=======
			trace_nfs4_reclaim_delegation(inode, res->delegation_type);
>>>>>>> refs/remotes/origin/master
		} else {
			/* We appear to have raced with a delegation return. */
			spin_unlock(&delegation->lock);
			rcu_read_unlock();
			nfs_inode_set_delegation(inode, cred, res);
		}
	} else {
		rcu_read_unlock();
	}
}

static int nfs_do_return_delegation(struct inode *inode, struct nfs_delegation *delegation, int issync)
{
	int res = 0;

	res = nfs4_proc_delegreturn(inode, delegation->cred, &delegation->stateid, issync);
	nfs_free_delegation(delegation);
	return res;
}

static struct inode *nfs_delegation_grab_inode(struct nfs_delegation *delegation)
{
	struct inode *inode = NULL;

	spin_lock(&delegation->lock);
	if (delegation->inode != NULL)
		inode = igrab(delegation->inode);
	spin_unlock(&delegation->lock);
	return inode;
}

static struct nfs_delegation *
<<<<<<< HEAD
nfs_detach_delegation_locked(struct nfs_inode *nfsi,
			     struct nfs_server *server)
{
	struct nfs_delegation *delegation =
		rcu_dereference_protected(nfsi->delegation,
				lockdep_is_held(&server->nfs_client->cl_lock));

	if (delegation == NULL)
		goto nomatch;

	spin_lock(&delegation->lock);
=======
nfs_start_delegation_return_locked(struct nfs_inode *nfsi)
{
	struct nfs_delegation *ret = NULL;
	struct nfs_delegation *delegation = rcu_dereference(nfsi->delegation);

	if (delegation == NULL)
		goto out;
	spin_lock(&delegation->lock);
	if (!test_and_set_bit(NFS_DELEGATION_RETURNING, &delegation->flags))
		ret = delegation;
	spin_unlock(&delegation->lock);
out:
	return ret;
}

static struct nfs_delegation *
nfs_start_delegation_return(struct nfs_inode *nfsi)
{
	struct nfs_delegation *delegation;

	rcu_read_lock();
	delegation = nfs_start_delegation_return_locked(nfsi);
	rcu_read_unlock();
	return delegation;
}

static void
nfs_abort_delegation_return(struct nfs_delegation *delegation,
		struct nfs_client *clp)
{

	spin_lock(&delegation->lock);
	clear_bit(NFS_DELEGATION_RETURNING, &delegation->flags);
	set_bit(NFS_DELEGATION_RETURN, &delegation->flags);
	spin_unlock(&delegation->lock);
	set_bit(NFS4CLNT_DELEGRETURN, &clp->cl_state);
}

static struct nfs_delegation *
nfs_detach_delegation_locked(struct nfs_inode *nfsi,
		struct nfs_delegation *delegation,
		struct nfs_client *clp)
{
	struct nfs_delegation *deleg_cur =
		rcu_dereference_protected(nfsi->delegation,
				lockdep_is_held(&clp->cl_lock));

	if (deleg_cur == NULL || delegation != deleg_cur)
		return NULL;

	spin_lock(&delegation->lock);
	set_bit(NFS_DELEGATION_RETURNING, &delegation->flags);
>>>>>>> refs/remotes/origin/master
	list_del_rcu(&delegation->super_list);
	delegation->inode = NULL;
	nfsi->delegation_state = 0;
	rcu_assign_pointer(nfsi->delegation, NULL);
	spin_unlock(&delegation->lock);
	return delegation;
<<<<<<< HEAD
nomatch:
	return NULL;
}

static struct nfs_delegation *nfs_detach_delegation(struct nfs_inode *nfsi,
						    struct nfs_server *server)
{
	struct nfs_client *clp = server->nfs_client;
	struct nfs_delegation *delegation;

	spin_lock(&clp->cl_lock);
	delegation = nfs_detach_delegation_locked(nfsi, server);
=======
}

static struct nfs_delegation *nfs_detach_delegation(struct nfs_inode *nfsi,
		struct nfs_delegation *delegation,
		struct nfs_server *server)
{
	struct nfs_client *clp = server->nfs_client;

	spin_lock(&clp->cl_lock);
	delegation = nfs_detach_delegation_locked(nfsi, delegation, clp);
>>>>>>> refs/remotes/origin/master
	spin_unlock(&clp->cl_lock);
	return delegation;
}

<<<<<<< HEAD
=======
static struct nfs_delegation *
nfs_inode_detach_delegation(struct inode *inode)
{
	struct nfs_inode *nfsi = NFS_I(inode);
	struct nfs_server *server = NFS_SERVER(inode);
	struct nfs_delegation *delegation;

	delegation = nfs_start_delegation_return(nfsi);
	if (delegation == NULL)
		return NULL;
	return nfs_detach_delegation(nfsi, delegation, server);
}

>>>>>>> refs/remotes/origin/master
/**
 * nfs_inode_set_delegation - set up a delegation on an inode
 * @inode: inode to which delegation applies
 * @cred: cred to use for subsequent delegation processing
 * @res: new delegation state from server
 *
 * Returns zero on success, or a negative errno value.
 */
int nfs_inode_set_delegation(struct inode *inode, struct rpc_cred *cred, struct nfs_openres *res)
{
	struct nfs_server *server = NFS_SERVER(inode);
	struct nfs_client *clp = server->nfs_client;
	struct nfs_inode *nfsi = NFS_I(inode);
	struct nfs_delegation *delegation, *old_delegation;
	struct nfs_delegation *freeme = NULL;
	int status = 0;

	delegation = kmalloc(sizeof(*delegation), GFP_NOFS);
	if (delegation == NULL)
		return -ENOMEM;
<<<<<<< HEAD
<<<<<<< HEAD
	memcpy(delegation->stateid.data, res->delegation.data,
			sizeof(delegation->stateid.data));
	delegation->type = res->delegation_type;
	delegation->maxsize = res->maxsize;
	delegation->change_attr = nfsi->change_attr;
=======
=======
>>>>>>> refs/remotes/origin/master
	nfs4_stateid_copy(&delegation->stateid, &res->delegation);
	delegation->type = res->delegation_type;
	delegation->maxsize = res->maxsize;
	delegation->change_attr = inode->i_version;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	delegation->cred = get_rpccred(cred);
	delegation->inode = inode;
	delegation->flags = 1<<NFS_DELEGATION_REFERENCED;
	spin_lock_init(&delegation->lock);

	spin_lock(&clp->cl_lock);
	old_delegation = rcu_dereference_protected(nfsi->delegation,
					lockdep_is_held(&clp->cl_lock));
	if (old_delegation != NULL) {
<<<<<<< HEAD
<<<<<<< HEAD
		if (memcmp(&delegation->stateid, &old_delegation->stateid,
					sizeof(old_delegation->stateid)) == 0 &&
=======
		if (nfs4_stateid_match(&delegation->stateid,
					&old_delegation->stateid) &&
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (nfs4_stateid_match(&delegation->stateid,
					&old_delegation->stateid) &&
>>>>>>> refs/remotes/origin/master
				delegation->type == old_delegation->type) {
			goto out;
		}
		/*
		 * Deal with broken servers that hand out two
		 * delegations for the same file.
<<<<<<< HEAD
<<<<<<< HEAD
=======
		 * Allow for upgrades to a WRITE delegation, but
		 * nothing else.
>>>>>>> refs/remotes/origin/cm-10.0
=======
		 * Allow for upgrades to a WRITE delegation, but
		 * nothing else.
>>>>>>> refs/remotes/origin/master
		 */
		dfprintk(FILE, "%s: server %s handed out "
				"a duplicate delegation!\n",
				__func__, clp->cl_hostname);
<<<<<<< HEAD
<<<<<<< HEAD
		if (delegation->type <= old_delegation->type) {
=======
		if (delegation->type == old_delegation->type ||
		    !(delegation->type & FMODE_WRITE)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (delegation->type == old_delegation->type ||
		    !(delegation->type & FMODE_WRITE)) {
>>>>>>> refs/remotes/origin/master
			freeme = delegation;
			delegation = NULL;
			goto out;
		}
<<<<<<< HEAD
		freeme = nfs_detach_delegation_locked(nfsi, server);
=======
		freeme = nfs_detach_delegation_locked(nfsi, 
				old_delegation, clp);
		if (freeme == NULL)
			goto out;
>>>>>>> refs/remotes/origin/master
	}
	list_add_rcu(&delegation->super_list, &server->delegations);
	nfsi->delegation_state = delegation->type;
	rcu_assign_pointer(nfsi->delegation, delegation);
	delegation = NULL;

	/* Ensure we revalidate the attributes and page cache! */
	spin_lock(&inode->i_lock);
	nfsi->cache_validity |= NFS_INO_REVAL_FORCED;
	spin_unlock(&inode->i_lock);
<<<<<<< HEAD
=======
	trace_nfs4_set_delegation(inode, res->delegation_type);
>>>>>>> refs/remotes/origin/master

out:
	spin_unlock(&clp->cl_lock);
	if (delegation != NULL)
		nfs_free_delegation(delegation);
	if (freeme != NULL)
		nfs_do_return_delegation(inode, freeme, 0);
	return status;
}

/*
 * Basic procedure for returning a delegation to the server
 */
<<<<<<< HEAD
static int __nfs_inode_return_delegation(struct inode *inode, struct nfs_delegation *delegation, int issync)
{
	struct nfs_inode *nfsi = NFS_I(inode);
	int err;

	/*
	 * Guard against new delegated open/lock/unlock calls and against
	 * state recovery
	 */
	down_write(&nfsi->rwsem);
	err = nfs_delegation_claim_opens(inode, &delegation->stateid);
	up_write(&nfsi->rwsem);
	if (err)
=======
static int nfs_end_delegation_return(struct inode *inode, struct nfs_delegation *delegation, int issync)
{
	struct nfs_client *clp = NFS_SERVER(inode)->nfs_client;
	struct nfs_inode *nfsi = NFS_I(inode);
	int err;

	if (delegation == NULL)
		return 0;
	do {
		err = nfs_delegation_claim_opens(inode, &delegation->stateid);
		if (!issync || err != -EAGAIN)
			break;
		/*
		 * Guard against state recovery
		 */
		err = nfs4_wait_clnt_recover(clp);
	} while (err == 0);

	if (err) {
		nfs_abort_delegation_return(delegation, clp);
		goto out;
	}
	if (!nfs_detach_delegation(nfsi, delegation, NFS_SERVER(inode)))
>>>>>>> refs/remotes/origin/master
		goto out;

	err = nfs_do_return_delegation(inode, delegation, issync);
out:
	return err;
}

<<<<<<< HEAD
=======
static bool nfs_delegation_need_return(struct nfs_delegation *delegation)
{
	bool ret = false;

	if (test_and_clear_bit(NFS_DELEGATION_RETURN, &delegation->flags))
		ret = true;
	if (test_and_clear_bit(NFS_DELEGATION_RETURN_IF_CLOSED, &delegation->flags) && !ret) {
		struct inode *inode;

		spin_lock(&delegation->lock);
		inode = delegation->inode;
		if (inode && list_empty(&NFS_I(inode)->open_files))
			ret = true;
		spin_unlock(&delegation->lock);
	}
	return ret;
}

>>>>>>> refs/remotes/origin/master
/**
 * nfs_client_return_marked_delegations - return previously marked delegations
 * @clp: nfs_client to process
 *
<<<<<<< HEAD
=======
 * Note that this function is designed to be called by the state
 * manager thread. For this reason, it cannot flush the dirty data,
 * since that could deadlock in case of a state recovery error.
 *
>>>>>>> refs/remotes/origin/master
 * Returns zero on success, or a negative errno value.
 */
int nfs_client_return_marked_delegations(struct nfs_client *clp)
{
	struct nfs_delegation *delegation;
	struct nfs_server *server;
	struct inode *inode;
	int err = 0;

restart:
	rcu_read_lock();
	list_for_each_entry_rcu(server, &clp->cl_superblocks, client_link) {
		list_for_each_entry_rcu(delegation, &server->delegations,
								super_list) {
<<<<<<< HEAD
			if (!test_and_clear_bit(NFS_DELEGATION_RETURN,
							&delegation->flags))
=======
			if (!nfs_delegation_need_return(delegation))
>>>>>>> refs/remotes/origin/master
				continue;
			inode = nfs_delegation_grab_inode(delegation);
			if (inode == NULL)
				continue;
<<<<<<< HEAD
			delegation = nfs_detach_delegation(NFS_I(inode),
								server);
			rcu_read_unlock();

			if (delegation != NULL) {
				filemap_flush(inode->i_mapping);
				err = __nfs_inode_return_delegation(inode,
								delegation, 0);
			}
=======
			delegation = nfs_start_delegation_return_locked(NFS_I(inode));
			rcu_read_unlock();

			err = nfs_end_delegation_return(inode, delegation, 0);
>>>>>>> refs/remotes/origin/master
			iput(inode);
			if (!err)
				goto restart;
			set_bit(NFS4CLNT_DELEGRETURN, &clp->cl_state);
			return err;
		}
	}
	rcu_read_unlock();
	return 0;
}

/**
 * nfs_inode_return_delegation_noreclaim - return delegation, don't reclaim opens
 * @inode: inode to process
 *
 * Does not protect against delegation reclaims, therefore really only safe
 * to be called from nfs4_clear_inode().
 */
void nfs_inode_return_delegation_noreclaim(struct inode *inode)
{
<<<<<<< HEAD
	struct nfs_server *server = NFS_SERVER(inode);
	struct nfs_inode *nfsi = NFS_I(inode);
	struct nfs_delegation *delegation;

	if (rcu_access_pointer(nfsi->delegation) != NULL) {
		delegation = nfs_detach_delegation(nfsi, server);
		if (delegation != NULL)
			nfs_do_return_delegation(inode, delegation, 0);
	}
=======
	struct nfs_delegation *delegation;

	delegation = nfs_inode_detach_delegation(inode);
	if (delegation != NULL)
		nfs_do_return_delegation(inode, delegation, 0);
>>>>>>> refs/remotes/origin/master
}

/**
 * nfs_inode_return_delegation - synchronously return a delegation
 * @inode: inode to process
 *
<<<<<<< HEAD
 * Returns zero on success, or a negative errno value.
 */
int nfs_inode_return_delegation(struct inode *inode)
{
	struct nfs_server *server = NFS_SERVER(inode);
=======
 * This routine will always flush any dirty data to disk on the
 * assumption that if we need to return the delegation, then
 * we should stop caching.
 *
 * Returns zero on success, or a negative errno value.
 */
int nfs4_inode_return_delegation(struct inode *inode)
{
>>>>>>> refs/remotes/origin/master
	struct nfs_inode *nfsi = NFS_I(inode);
	struct nfs_delegation *delegation;
	int err = 0;

<<<<<<< HEAD
	if (rcu_access_pointer(nfsi->delegation) != NULL) {
		delegation = nfs_detach_delegation(nfsi, server);
		if (delegation != NULL) {
			nfs_wb_all(inode);
			err = __nfs_inode_return_delegation(inode, delegation, 1);
		}
	}
	return err;
}

=======
	nfs_wb_all(inode);
	delegation = nfs_start_delegation_return(nfsi);
	if (delegation != NULL)
		err = nfs_end_delegation_return(inode, delegation, 1);
	return err;
}

static void nfs_mark_return_if_closed_delegation(struct nfs_server *server,
		struct nfs_delegation *delegation)
{
	set_bit(NFS_DELEGATION_RETURN_IF_CLOSED, &delegation->flags);
	set_bit(NFS4CLNT_DELEGRETURN, &server->nfs_client->cl_state);
}

>>>>>>> refs/remotes/origin/master
static void nfs_mark_return_delegation(struct nfs_server *server,
		struct nfs_delegation *delegation)
{
	set_bit(NFS_DELEGATION_RETURN, &delegation->flags);
	set_bit(NFS4CLNT_DELEGRETURN, &server->nfs_client->cl_state);
}

<<<<<<< HEAD
=======
static bool nfs_server_mark_return_all_delegations(struct nfs_server *server)
{
	struct nfs_delegation *delegation;
	bool ret = false;

	list_for_each_entry_rcu(delegation, &server->delegations, super_list) {
		nfs_mark_return_delegation(server, delegation);
		ret = true;
	}
	return ret;
}

static void nfs_client_mark_return_all_delegations(struct nfs_client *clp)
{
	struct nfs_server *server;

	rcu_read_lock();
	list_for_each_entry_rcu(server, &clp->cl_superblocks, client_link)
		nfs_server_mark_return_all_delegations(server);
	rcu_read_unlock();
}

static void nfs_delegation_run_state_manager(struct nfs_client *clp)
{
	if (test_bit(NFS4CLNT_DELEGRETURN, &clp->cl_state))
		nfs4_schedule_state_manager(clp);
}

/**
 * nfs_expire_all_delegations
 * @clp: client to process
 *
 */
void nfs_expire_all_delegations(struct nfs_client *clp)
{
	nfs_client_mark_return_all_delegations(clp);
	nfs_delegation_run_state_manager(clp);
}

>>>>>>> refs/remotes/origin/master
/**
 * nfs_super_return_all_delegations - return delegations for one superblock
 * @sb: sb to process
 *
 */
<<<<<<< HEAD
void nfs_super_return_all_delegations(struct super_block *sb)
{
	struct nfs_server *server = NFS_SB(sb);
	struct nfs_client *clp = server->nfs_client;
	struct nfs_delegation *delegation;
=======
void nfs_server_return_all_delegations(struct nfs_server *server)
{
	struct nfs_client *clp = server->nfs_client;
	bool need_wait;
>>>>>>> refs/remotes/origin/master

	if (clp == NULL)
		return;

	rcu_read_lock();
<<<<<<< HEAD
	list_for_each_entry_rcu(delegation, &server->delegations, super_list) {
		spin_lock(&delegation->lock);
		set_bit(NFS_DELEGATION_RETURN, &delegation->flags);
		spin_unlock(&delegation->lock);
	}
	rcu_read_unlock();

	if (nfs_client_return_marked_delegations(clp) != 0)
		nfs4_schedule_state_manager(clp);
}

static void nfs_mark_return_all_delegation_types(struct nfs_server *server,
=======
	need_wait = nfs_server_mark_return_all_delegations(server);
	rcu_read_unlock();

	if (need_wait) {
		nfs4_schedule_state_manager(clp);
		nfs4_wait_clnt_recover(clp);
	}
}

static void nfs_mark_return_unused_delegation_types(struct nfs_server *server,
>>>>>>> refs/remotes/origin/master
						 fmode_t flags)
{
	struct nfs_delegation *delegation;

	list_for_each_entry_rcu(delegation, &server->delegations, super_list) {
		if ((delegation->type == (FMODE_READ|FMODE_WRITE)) && !(flags & FMODE_WRITE))
			continue;
		if (delegation->type & flags)
<<<<<<< HEAD
			nfs_mark_return_delegation(server, delegation);
	}
}

static void nfs_client_mark_return_all_delegation_types(struct nfs_client *clp,
=======
			nfs_mark_return_if_closed_delegation(server, delegation);
	}
}

static void nfs_client_mark_return_unused_delegation_types(struct nfs_client *clp,
>>>>>>> refs/remotes/origin/master
							fmode_t flags)
{
	struct nfs_server *server;

	rcu_read_lock();
	list_for_each_entry_rcu(server, &clp->cl_superblocks, client_link)
<<<<<<< HEAD
		nfs_mark_return_all_delegation_types(server, flags);
	rcu_read_unlock();
}

<<<<<<< HEAD
static void nfs_client_mark_return_all_delegations(struct nfs_client *clp)
{
	nfs_client_mark_return_all_delegation_types(clp, FMODE_READ|FMODE_WRITE);
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
static void nfs_delegation_run_state_manager(struct nfs_client *clp)
{
	if (test_bit(NFS4CLNT_DELEGRETURN, &clp->cl_state))
		nfs4_schedule_state_manager(clp);
}

=======
		nfs_mark_return_unused_delegation_types(server, flags);
	rcu_read_unlock();
}

>>>>>>> refs/remotes/origin/master
void nfs_remove_bad_delegation(struct inode *inode)
{
	struct nfs_delegation *delegation;

<<<<<<< HEAD
	delegation = nfs_detach_delegation(NFS_I(inode), NFS_SERVER(inode));
=======
	delegation = nfs_inode_detach_delegation(inode);
>>>>>>> refs/remotes/origin/master
	if (delegation) {
		nfs_inode_find_state_and_recover(inode, &delegation->stateid);
		nfs_free_delegation(delegation);
	}
}
<<<<<<< HEAD
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(nfs_remove_bad_delegation);
>>>>>>> refs/remotes/origin/cm-10.0

/**
 * nfs_expire_all_delegation_types
=======
EXPORT_SYMBOL_GPL(nfs_remove_bad_delegation);

/**
 * nfs_expire_unused_delegation_types
>>>>>>> refs/remotes/origin/master
 * @clp: client to process
 * @flags: delegation types to expire
 *
 */
<<<<<<< HEAD
void nfs_expire_all_delegation_types(struct nfs_client *clp, fmode_t flags)
{
	nfs_client_mark_return_all_delegation_types(clp, flags);
	nfs_delegation_run_state_manager(clp);
}

/**
 * nfs_expire_all_delegations
 * @clp: client to process
 *
 */
void nfs_expire_all_delegations(struct nfs_client *clp)
{
	nfs_expire_all_delegation_types(clp, FMODE_READ|FMODE_WRITE);
}

<<<<<<< HEAD
/**
 * nfs_handle_cb_pathdown - return all delegations after NFS4ERR_CB_PATH_DOWN
 * @clp: client to process
 *
 */
void nfs_handle_cb_pathdown(struct nfs_client *clp)
{
	if (clp == NULL)
		return;
	nfs_client_mark_return_all_delegations(clp);
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
void nfs_expire_unused_delegation_types(struct nfs_client *clp, fmode_t flags)
{
	nfs_client_mark_return_unused_delegation_types(clp, flags);
	nfs_delegation_run_state_manager(clp);
}

>>>>>>> refs/remotes/origin/master
static void nfs_mark_return_unreferenced_delegations(struct nfs_server *server)
{
	struct nfs_delegation *delegation;

	list_for_each_entry_rcu(delegation, &server->delegations, super_list) {
		if (test_and_clear_bit(NFS_DELEGATION_REFERENCED, &delegation->flags))
			continue;
<<<<<<< HEAD
		nfs_mark_return_delegation(server, delegation);
=======
		nfs_mark_return_if_closed_delegation(server, delegation);
>>>>>>> refs/remotes/origin/master
	}
}

/**
 * nfs_expire_unreferenced_delegations - Eliminate unused delegations
 * @clp: nfs_client to process
 *
 */
void nfs_expire_unreferenced_delegations(struct nfs_client *clp)
{
	struct nfs_server *server;

	rcu_read_lock();
	list_for_each_entry_rcu(server, &clp->cl_superblocks, client_link)
		nfs_mark_return_unreferenced_delegations(server);
	rcu_read_unlock();

	nfs_delegation_run_state_manager(clp);
}

/**
 * nfs_async_inode_return_delegation - asynchronously return a delegation
 * @inode: inode to process
<<<<<<< HEAD
<<<<<<< HEAD
 * @stateid: state ID information from CB_RECALL arguments
=======
 * @stateid: state ID information
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * @stateid: state ID information
>>>>>>> refs/remotes/origin/master
 *
 * Returns zero on success, or a negative errno value.
 */
int nfs_async_inode_return_delegation(struct inode *inode,
				      const nfs4_stateid *stateid)
{
	struct nfs_server *server = NFS_SERVER(inode);
	struct nfs_client *clp = server->nfs_client;
	struct nfs_delegation *delegation;

<<<<<<< HEAD
	rcu_read_lock();
	delegation = rcu_dereference(NFS_I(inode)->delegation);

<<<<<<< HEAD
	if (!clp->cl_mvops->validate_stateid(delegation, stateid)) {
=======
	if (!clp->cl_mvops->match_stateid(&delegation->stateid, stateid)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	filemap_flush(inode->i_mapping);

	rcu_read_lock();
	delegation = rcu_dereference(NFS_I(inode)->delegation);

	if (!clp->cl_mvops->match_stateid(&delegation->stateid, stateid)) {
>>>>>>> refs/remotes/origin/master
		rcu_read_unlock();
		return -ENOENT;
	}
	nfs_mark_return_delegation(server, delegation);
	rcu_read_unlock();

	nfs_delegation_run_state_manager(clp);
	return 0;
}

static struct inode *
nfs_delegation_find_inode_server(struct nfs_server *server,
				 const struct nfs_fh *fhandle)
{
	struct nfs_delegation *delegation;
	struct inode *res = NULL;

	list_for_each_entry_rcu(delegation, &server->delegations, super_list) {
		spin_lock(&delegation->lock);
		if (delegation->inode != NULL &&
		    nfs_compare_fh(fhandle, &NFS_I(delegation->inode)->fh) == 0) {
			res = igrab(delegation->inode);
		}
		spin_unlock(&delegation->lock);
		if (res != NULL)
			break;
	}
	return res;
}

/**
 * nfs_delegation_find_inode - retrieve the inode associated with a delegation
 * @clp: client state handle
 * @fhandle: filehandle from a delegation recall
 *
 * Returns pointer to inode matching "fhandle," or NULL if a matching inode
 * cannot be found.
 */
struct inode *nfs_delegation_find_inode(struct nfs_client *clp,
					const struct nfs_fh *fhandle)
{
	struct nfs_server *server;
	struct inode *res = NULL;

	rcu_read_lock();
	list_for_each_entry_rcu(server, &clp->cl_superblocks, client_link) {
		res = nfs_delegation_find_inode_server(server, fhandle);
		if (res != NULL)
			break;
	}
	rcu_read_unlock();
	return res;
}

static void nfs_delegation_mark_reclaim_server(struct nfs_server *server)
{
	struct nfs_delegation *delegation;

	list_for_each_entry_rcu(delegation, &server->delegations, super_list)
		set_bit(NFS_DELEGATION_NEED_RECLAIM, &delegation->flags);
}

/**
 * nfs_delegation_mark_reclaim - mark all delegations as needing to be reclaimed
 * @clp: nfs_client to process
 *
 */
void nfs_delegation_mark_reclaim(struct nfs_client *clp)
{
	struct nfs_server *server;

	rcu_read_lock();
	list_for_each_entry_rcu(server, &clp->cl_superblocks, client_link)
		nfs_delegation_mark_reclaim_server(server);
	rcu_read_unlock();
}

/**
 * nfs_delegation_reap_unclaimed - reap unclaimed delegations after reboot recovery is done
 * @clp: nfs_client to process
 *
 */
void nfs_delegation_reap_unclaimed(struct nfs_client *clp)
{
	struct nfs_delegation *delegation;
	struct nfs_server *server;
	struct inode *inode;

restart:
	rcu_read_lock();
	list_for_each_entry_rcu(server, &clp->cl_superblocks, client_link) {
		list_for_each_entry_rcu(delegation, &server->delegations,
								super_list) {
			if (test_bit(NFS_DELEGATION_NEED_RECLAIM,
						&delegation->flags) == 0)
				continue;
			inode = nfs_delegation_grab_inode(delegation);
			if (inode == NULL)
				continue;
			delegation = nfs_detach_delegation(NFS_I(inode),
<<<<<<< HEAD
								server);
=======
					delegation, server);
>>>>>>> refs/remotes/origin/master
			rcu_read_unlock();

			if (delegation != NULL)
				nfs_free_delegation(delegation);
			iput(inode);
			goto restart;
		}
	}
	rcu_read_unlock();
}

/**
 * nfs_delegations_present - check for existence of delegations
 * @clp: client state handle
 *
 * Returns one if there are any nfs_delegation structures attached
 * to this nfs_client.
 */
int nfs_delegations_present(struct nfs_client *clp)
{
	struct nfs_server *server;
	int ret = 0;

	rcu_read_lock();
	list_for_each_entry_rcu(server, &clp->cl_superblocks, client_link)
		if (!list_empty(&server->delegations)) {
			ret = 1;
			break;
		}
	rcu_read_unlock();
	return ret;
}

/**
 * nfs4_copy_delegation_stateid - Copy inode's state ID information
 * @dst: stateid data structure to fill in
 * @inode: inode to check
<<<<<<< HEAD
<<<<<<< HEAD
 *
 * Returns one and fills in "dst->data" * if inode had a delegation,
 * otherwise zero is returned.
 */
int nfs4_copy_delegation_stateid(nfs4_stateid *dst, struct inode *inode)
{
	struct nfs_inode *nfsi = NFS_I(inode);
	struct nfs_delegation *delegation;
	int ret = 0;

	rcu_read_lock();
	delegation = rcu_dereference(nfsi->delegation);
	if (delegation != NULL) {
		memcpy(dst->data, delegation->stateid.data, sizeof(dst->data));
		ret = 1;
=======
=======
>>>>>>> refs/remotes/origin/master
 * @flags: delegation type requirement
 *
 * Returns "true" and fills in "dst->data" * if inode had a delegation,
 * otherwise "false" is returned.
 */
bool nfs4_copy_delegation_stateid(nfs4_stateid *dst, struct inode *inode,
		fmode_t flags)
{
	struct nfs_inode *nfsi = NFS_I(inode);
	struct nfs_delegation *delegation;
	bool ret;

	flags &= FMODE_READ|FMODE_WRITE;
	rcu_read_lock();
	delegation = rcu_dereference(nfsi->delegation);
	ret = (delegation != NULL && (delegation->type & flags) == flags);
	if (ret) {
		nfs4_stateid_copy(dst, &delegation->stateid);
		nfs_mark_delegation_referenced(delegation);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}
	rcu_read_unlock();
	return ret;
}
