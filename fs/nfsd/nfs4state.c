/*
*  Copyright (c) 2001 The Regents of the University of Michigan.
*  All rights reserved.
*
*  Kendrick Smith <kmsmith@umich.edu>
*  Andy Adamson <kandros@umich.edu>
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*  1. Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*  2. Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the
*     documentation and/or other materials provided with the distribution.
*  3. Neither the name of the University nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
*  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
*  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
*  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
*  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
*  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
*  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
*  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
*  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

#include <linux/file.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/namei.h>
#include <linux/swap.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/pagemap.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/sunrpc/svcauth_gss.h>
#include <linux/sunrpc/clnt.h>
#include "xdr4.h"
#include "vfs.h"

#define NFSDDBG_FACILITY                NFSDDBG_PROC

/* Globals */
time_t nfsd4_lease = 90;     /* default lease time */
time_t nfsd4_grace = 90;
static time_t boot_time;
<<<<<<< HEAD
static u32 current_ownerid = 1;
static u32 current_fileid = 1;
static u32 current_delegid = 1;
static stateid_t zerostateid;             /* bits all 0 */
static stateid_t onestateid;              /* bits all 1 */
static u64 current_sessionid = 1;

#define ZERO_STATEID(stateid) (!memcmp((stateid), &zerostateid, sizeof(stateid_t)))
#define ONE_STATEID(stateid)  (!memcmp((stateid), &onestateid, sizeof(stateid_t)))

/* forward declarations */
static struct nfs4_stateid * find_stateid(stateid_t *stid, int flags);
static struct nfs4_delegation * find_delegation_stateid(struct inode *ino, stateid_t *stid);
static char user_recovery_dirname[PATH_MAX] = "/var/lib/nfs/v4recovery";
static void nfs4_set_recdir(char *recdir);
=======
=======
#include <linux/pagemap.h>
#include <linux/ratelimit.h>
#include <linux/sunrpc/svcauth_gss.h>
#include <linux/sunrpc/addr.h>
#include "xdr4.h"
#include "xdr4cb.h"
#include "vfs.h"
#include "current_stateid.h"

#include "netns.h"

#define NFSDDBG_FACILITY                NFSDDBG_PROC
>>>>>>> refs/remotes/origin/master

#define all_ones {{~0,~0},~0}
static const stateid_t one_stateid = {
	.si_generation = ~0,
	.si_opaque = all_ones,
};
static const stateid_t zero_stateid = {
	/* all fields zero */
};
static const stateid_t currentstateid = {
	.si_generation = 1,
};

static u64 current_sessionid = 1;

#define ZERO_STATEID(stateid) (!memcmp((stateid), &zero_stateid, sizeof(stateid_t)))
#define ONE_STATEID(stateid)  (!memcmp((stateid), &one_stateid, sizeof(stateid_t)))
#define CURRENT_STATEID(stateid) (!memcmp((stateid), &currentstateid, sizeof(stateid_t)))

/* forward declarations */
static int check_for_locks(struct nfs4_file *filp, struct nfs4_lockowner *lowner);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

/* Locking: */

/* Currently used for almost all code touching nfsv4 state: */
static DEFINE_MUTEX(client_mutex);

/*
 * Currently used for the del_recall_lru and file hash table.  In an
 * effort to decrease the scope of the client_mutex, this spinlock may
 * eventually cover more:
 */
static DEFINE_SPINLOCK(recall_lock);

<<<<<<< HEAD
<<<<<<< HEAD
static struct kmem_cache *stateowner_slab = NULL;
=======
static struct kmem_cache *openowner_slab = NULL;
static struct kmem_cache *lockowner_slab = NULL;
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct kmem_cache *openowner_slab = NULL;
static struct kmem_cache *lockowner_slab = NULL;
>>>>>>> refs/remotes/origin/master
static struct kmem_cache *file_slab = NULL;
static struct kmem_cache *stateid_slab = NULL;
static struct kmem_cache *deleg_slab = NULL;

void
nfs4_lock_state(void)
{
	mutex_lock(&client_mutex);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
static void free_session(struct kref *);

/* Must be called under the client_lock */
static void nfsd4_put_session_locked(struct nfsd4_session *ses)
{
	kref_put(&ses->se_ref, free_session);
}

static void nfsd4_get_session(struct nfsd4_session *ses)
{
	kref_get(&ses->se_ref);
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
static void free_session(struct nfsd4_session *);

static bool is_session_dead(struct nfsd4_session *ses)
{
	return ses->se_flags & NFS4_SESSION_DEAD;
}

void nfsd4_put_session(struct nfsd4_session *ses)
{
	if (atomic_dec_and_test(&ses->se_ref) && is_session_dead(ses))
		free_session(ses);
}

static __be32 mark_session_dead_locked(struct nfsd4_session *ses, int ref_held_by_me)
{
	if (atomic_read(&ses->se_ref) > ref_held_by_me)
		return nfserr_jukebox;
	ses->se_flags |= NFS4_SESSION_DEAD;
	return nfs_ok;
}

static __be32 nfsd4_get_session_locked(struct nfsd4_session *ses)
{
	if (is_session_dead(ses))
		return nfserr_badsession;
	atomic_inc(&ses->se_ref);
	return nfs_ok;
}

>>>>>>> refs/remotes/origin/master
void
nfs4_unlock_state(void)
{
	mutex_unlock(&client_mutex);
}

<<<<<<< HEAD
=======
static bool is_client_expired(struct nfs4_client *clp)
{
	return clp->cl_time == 0;
}

static __be32 mark_client_expired_locked(struct nfs4_client *clp)
{
	if (atomic_read(&clp->cl_refcount))
		return nfserr_jukebox;
	clp->cl_time = 0;
	return nfs_ok;
}

static __be32 mark_client_expired(struct nfs4_client *clp)
{
	struct nfsd_net *nn = net_generic(clp->net, nfsd_net_id);
	__be32 ret;

	spin_lock(&nn->client_lock);
	ret = mark_client_expired_locked(clp);
	spin_unlock(&nn->client_lock);
	return ret;
}

static __be32 get_client_locked(struct nfs4_client *clp)
{
	if (is_client_expired(clp))
		return nfserr_expired;
	atomic_inc(&clp->cl_refcount);
	return nfs_ok;
}

/* must be called under the client_lock */
static inline void
renew_client_locked(struct nfs4_client *clp)
{
	struct nfsd_net *nn = net_generic(clp->net, nfsd_net_id);

	if (is_client_expired(clp)) {
		WARN_ON(1);
		printk("%s: client (clientid %08x/%08x) already expired\n",
			__func__,
			clp->cl_clientid.cl_boot,
			clp->cl_clientid.cl_id);
		return;
	}

	dprintk("renewing client (clientid %08x/%08x)\n",
			clp->cl_clientid.cl_boot,
			clp->cl_clientid.cl_id);
	list_move_tail(&clp->cl_lru, &nn->client_lru);
	clp->cl_time = get_seconds();
}

static inline void
renew_client(struct nfs4_client *clp)
{
	struct nfsd_net *nn = net_generic(clp->net, nfsd_net_id);

	spin_lock(&nn->client_lock);
	renew_client_locked(clp);
	spin_unlock(&nn->client_lock);
}

static void put_client_renew_locked(struct nfs4_client *clp)
{
	if (!atomic_dec_and_test(&clp->cl_refcount))
		return;
	if (!is_client_expired(clp))
		renew_client_locked(clp);
}

void put_client_renew(struct nfs4_client *clp)
{
	struct nfsd_net *nn = net_generic(clp->net, nfsd_net_id);

	if (!atomic_dec_and_lock(&clp->cl_refcount, &nn->client_lock))
		return;
	if (!is_client_expired(clp))
		renew_client_locked(clp);
	spin_unlock(&nn->client_lock);
}


>>>>>>> refs/remotes/origin/master
static inline u32
opaque_hashval(const void *ptr, int nbytes)
{
	unsigned char *cptr = (unsigned char *) ptr;

	u32 x = 0;
	while (nbytes--) {
		x *= 37;
		x += *cptr++;
	}
	return x;
}

<<<<<<< HEAD
static struct list_head del_recall_lru;

<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static void nfsd4_free_file(struct nfs4_file *f)
{
	kmem_cache_free(file_slab, f);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static inline void
put_nfs4_file(struct nfs4_file *fi)
{
	if (atomic_dec_and_lock(&fi->fi_ref, &recall_lock)) {
<<<<<<< HEAD
		list_del(&fi->fi_hash);
		spin_unlock(&recall_lock);
		iput(fi->fi_inode);
<<<<<<< HEAD
		kmem_cache_free(file_slab, fi);
=======
		nfsd4_free_file(fi);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		hlist_del(&fi->fi_hash);
		spin_unlock(&recall_lock);
		iput(fi->fi_inode);
		nfsd4_free_file(fi);
>>>>>>> refs/remotes/origin/master
	}
}

static inline void
get_nfs4_file(struct nfs4_file *fi)
{
	atomic_inc(&fi->fi_ref);
}

static int num_delegations;
<<<<<<< HEAD
unsigned int max_delegations;
=======
unsigned long max_delegations;
>>>>>>> refs/remotes/origin/master

/*
 * Open owner state (share locks)
 */

<<<<<<< HEAD
<<<<<<< HEAD
/* hash tables for nfs4_stateowner */
=======
/* hash tables for lock and open owners */
>>>>>>> refs/remotes/origin/cm-10.0
=======
/* hash tables for lock and open owners */
>>>>>>> refs/remotes/origin/master
#define OWNER_HASH_BITS              8
#define OWNER_HASH_SIZE             (1 << OWNER_HASH_BITS)
#define OWNER_HASH_MASK             (OWNER_HASH_SIZE - 1)

<<<<<<< HEAD
<<<<<<< HEAD
#define ownerid_hashval(id) \
        ((id) & OWNER_HASH_MASK)
#define ownerstr_hashval(clientid, ownername) \
        (((clientid) + opaque_hashval((ownername.data), (ownername.len))) & OWNER_HASH_MASK)

static struct list_head	ownerid_hashtbl[OWNER_HASH_SIZE];
=======
=======
>>>>>>> refs/remotes/origin/master
static unsigned int ownerstr_hashval(u32 clientid, struct xdr_netobj *ownername)
{
	unsigned int ret;

	ret = opaque_hashval(ownername->data, ownername->len);
	ret += clientid;
	return ret & OWNER_HASH_MASK;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
static struct list_head	ownerstr_hashtbl[OWNER_HASH_SIZE];

=======
>>>>>>> refs/remotes/origin/master
/* hash table for nfs4_file */
#define FILE_HASH_BITS                   8
#define FILE_HASH_SIZE                  (1 << FILE_HASH_BITS)

<<<<<<< HEAD
<<<<<<< HEAD
/* hash table for (open)nfs4_stateid */
#define STATEID_HASH_BITS              10
#define STATEID_HASH_SIZE              (1 << STATEID_HASH_BITS)
#define STATEID_HASH_MASK              (STATEID_HASH_SIZE - 1)

#define file_hashval(x) \
        hash_ptr(x, FILE_HASH_BITS)
#define stateid_hashval(owner_id, file_id)  \
        (((owner_id) + (file_id)) & STATEID_HASH_MASK)

static struct list_head file_hashtbl[FILE_HASH_SIZE];
static struct list_head stateid_hashtbl[STATEID_HASH_SIZE];
=======
=======
>>>>>>> refs/remotes/origin/master
static unsigned int file_hashval(struct inode *ino)
{
	/* XXX: why are we hashing on inode pointer, anyway? */
	return hash_ptr(ino, FILE_HASH_BITS);
}

<<<<<<< HEAD
static struct list_head file_hashtbl[FILE_HASH_SIZE];
>>>>>>> refs/remotes/origin/cm-10.0

static void __nfs4_file_get_access(struct nfs4_file *fp, int oflag)
{
	BUG_ON(!(fp->fi_fds[oflag] || fp->fi_fds[O_RDWR]));
=======
static struct hlist_head file_hashtbl[FILE_HASH_SIZE];

static void __nfs4_file_get_access(struct nfs4_file *fp, int oflag)
{
	WARN_ON_ONCE(!(fp->fi_fds[oflag] || fp->fi_fds[O_RDWR]));
>>>>>>> refs/remotes/origin/master
	atomic_inc(&fp->fi_access[oflag]);
}

static void nfs4_file_get_access(struct nfs4_file *fp, int oflag)
{
	if (oflag == O_RDWR) {
		__nfs4_file_get_access(fp, O_RDONLY);
		__nfs4_file_get_access(fp, O_WRONLY);
	} else
		__nfs4_file_get_access(fp, oflag);
}

static void nfs4_file_put_fd(struct nfs4_file *fp, int oflag)
{
	if (fp->fi_fds[oflag]) {
		fput(fp->fi_fds[oflag]);
		fp->fi_fds[oflag] = NULL;
	}
}

static void __nfs4_file_put_access(struct nfs4_file *fp, int oflag)
{
	if (atomic_dec_and_test(&fp->fi_access[oflag])) {
		nfs4_file_put_fd(fp, oflag);
		if (atomic_read(&fp->fi_access[1 - oflag]) == 0)
			nfs4_file_put_fd(fp, O_RDWR);
	}
}

static void nfs4_file_put_access(struct nfs4_file *fp, int oflag)
{
	if (oflag == O_RDWR) {
		__nfs4_file_put_access(fp, O_RDONLY);
		__nfs4_file_put_access(fp, O_WRONLY);
	} else
		__nfs4_file_put_access(fp, oflag);
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct nfs4_delegation *
alloc_init_deleg(struct nfs4_client *clp, struct nfs4_stateid *stp, struct svc_fh *current_fh, u32 type)
=======
static inline int get_new_stid(struct nfs4_stid *stid)
{
	static int min_stateid = 0;
	struct idr *stateids = &stid->sc_client->cl_stateids;
	int new_stid;
	int error;

	error = idr_get_new_above(stateids, stid, min_stateid, &new_stid);
	/*
	 * Note: the necessary preallocation was done in
	 * nfs4_alloc_stateid().  The idr code caps the number of
	 * preallocations that can exist at a time, but the state lock
	 * prevents anyone from using ours before we get here:
	 */
	BUG_ON(error);
=======
static struct nfs4_stid *nfs4_alloc_stid(struct nfs4_client *cl, struct
kmem_cache *slab)
{
	struct idr *stateids = &cl->cl_stateids;
	struct nfs4_stid *stid;
	int new_id;

	stid = kmem_cache_alloc(slab, GFP_KERNEL);
	if (!stid)
		return NULL;

	new_id = idr_alloc_cyclic(stateids, stid, 0, 0, GFP_KERNEL);
	if (new_id < 0)
		goto out_free;
	stid->sc_client = cl;
	stid->sc_type = 0;
	stid->sc_stateid.si_opaque.so_id = new_id;
	stid->sc_stateid.si_opaque.so_clid = cl->cl_clientid;
	/* Will be incremented before return to client: */
	stid->sc_stateid.si_generation = 0;

>>>>>>> refs/remotes/origin/master
	/*
	 * It shouldn't be a problem to reuse an opaque stateid value.
	 * I don't think it is for 4.1.  But with 4.0 I worry that, for
	 * example, a stray write retransmission could be accepted by
	 * the server when it should have been rejected.  Therefore,
	 * adopt a trick from the sctp code to attempt to maximize the
	 * amount of time until an id is reused, by ensuring they always
	 * "increase" (mod INT_MAX):
	 */
<<<<<<< HEAD

	min_stateid = new_stid+1;
	if (min_stateid == INT_MAX)
		min_stateid = 0;
	return new_stid;
}

static void init_stid(struct nfs4_stid *stid, struct nfs4_client *cl, unsigned char type)
{
	stateid_t *s = &stid->sc_stateid;
	int new_id;

	stid->sc_type = type;
	stid->sc_client = cl;
	s->si_opaque.so_clid = cl->cl_clientid;
	new_id = get_new_stid(stid);
	s->si_opaque.so_id = (u32)new_id;
	/* Will be incremented before return to client: */
	s->si_generation = 0;
}

static struct nfs4_stid *nfs4_alloc_stid(struct nfs4_client *cl, struct kmem_cache *slab)
{
	struct idr *stateids = &cl->cl_stateids;

	if (!idr_pre_get(stateids, GFP_KERNEL))
		return NULL;
	/*
	 * Note: if we fail here (or any time between now and the time
	 * we actually get the new idr), we won't need to undo the idr
	 * preallocation, since the idr code caps the number of
	 * preallocated entries.
	 */
	return kmem_cache_alloc(slab, GFP_KERNEL);
=======
	return stid;
out_free:
	kmem_cache_free(slab, stid);
	return NULL;
>>>>>>> refs/remotes/origin/master
}

static struct nfs4_ol_stateid * nfs4_alloc_stateid(struct nfs4_client *clp)
{
	return openlockstateid(nfs4_alloc_stid(clp, stateid_slab));
}

static struct nfs4_delegation *
<<<<<<< HEAD
alloc_init_deleg(struct nfs4_client *clp, struct nfs4_ol_stateid *stp, struct svc_fh *current_fh, u32 type)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct nfs4_delegation *dp;
	struct nfs4_file *fp = stp->st_file;

	dprintk("NFSD alloc_init_deleg\n");
	/*
	 * Major work on the lease subsystem (for example, to support
	 * calbacks on stat) will be required before we can support
	 * write delegations properly.
	 */
	if (type != NFS4_OPEN_DELEGATE_READ)
		return NULL;
	if (fp->fi_had_conflict)
		return NULL;
	if (num_delegations > max_delegations)
		return NULL;
<<<<<<< HEAD
	dp = kmem_cache_alloc(deleg_slab, GFP_KERNEL);
	if (dp == NULL)
		return dp;
=======
	dp = delegstateid(nfs4_alloc_stid(clp, deleg_slab));
	if (dp == NULL)
		return dp;
	init_stid(&dp->dl_stid, clp, NFS4_DELEG_STID);
=======
alloc_init_deleg(struct nfs4_client *clp, struct nfs4_ol_stateid *stp, struct svc_fh *current_fh)
{
	struct nfs4_delegation *dp;

	dprintk("NFSD alloc_init_deleg\n");
	if (num_delegations > max_delegations)
		return NULL;
	dp = delegstateid(nfs4_alloc_stid(clp, deleg_slab));
	if (dp == NULL)
		return dp;
	dp->dl_stid.sc_type = NFS4_DELEG_STID;
>>>>>>> refs/remotes/origin/master
	/*
	 * delegation seqid's are never incremented.  The 4.1 special
	 * meaning of seqid 0 isn't meaningful, really, but let's avoid
	 * 0 anyway just for consistency and use 1:
	 */
	dp->dl_stid.sc_stateid.si_generation = 1;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	num_delegations++;
	INIT_LIST_HEAD(&dp->dl_perfile);
	INIT_LIST_HEAD(&dp->dl_perclnt);
	INIT_LIST_HEAD(&dp->dl_recall_lru);
<<<<<<< HEAD
<<<<<<< HEAD
	dp->dl_client = clp;
	get_nfs4_file(fp);
	dp->dl_file = fp;
	dp->dl_type = type;
	dp->dl_stateid.si_boot = boot_time;
	dp->dl_stateid.si_stateownerid = current_delegid++;
	dp->dl_stateid.si_fileid = 0;
	dp->dl_stateid.si_generation = 0;
=======
	get_nfs4_file(fp);
	dp->dl_file = fp;
	dp->dl_type = type;
>>>>>>> refs/remotes/origin/cm-10.0
	fh_copy_shallow(&dp->dl_fh, &current_fh->fh_handle);
	dp->dl_time = 0;
	atomic_set(&dp->dl_count, 1);
	INIT_WORK(&dp->dl_recall.cb_work, nfsd4_do_callback_rpc);
	return dp;
}

=======
	dp->dl_file = NULL;
	dp->dl_type = NFS4_OPEN_DELEGATE_READ;
	fh_copy_shallow(&dp->dl_fh, &current_fh->fh_handle);
	dp->dl_time = 0;
	atomic_set(&dp->dl_count, 1);
	nfsd4_init_callback(&dp->dl_recall);
	return dp;
}

static void remove_stid(struct nfs4_stid *s)
{
	struct idr *stateids = &s->sc_client->cl_stateids;

	idr_remove(stateids, s->sc_stateid.si_opaque.so_id);
}

static void nfs4_free_stid(struct kmem_cache *slab, struct nfs4_stid *s)
{
	kmem_cache_free(slab, s);
}

>>>>>>> refs/remotes/origin/master
void
nfs4_put_delegation(struct nfs4_delegation *dp)
{
	if (atomic_dec_and_test(&dp->dl_count)) {
<<<<<<< HEAD
		dprintk("NFSD: freeing dp %p\n",dp);
		put_nfs4_file(dp->dl_file);
		kmem_cache_free(deleg_slab, dp);
=======
		nfs4_free_stid(deleg_slab, &dp->dl_stid);
>>>>>>> refs/remotes/origin/master
		num_delegations--;
	}
}

static void nfs4_put_deleg_lease(struct nfs4_file *fp)
{
	if (atomic_dec_and_test(&fp->fi_delegees)) {
		vfs_setlease(fp->fi_deleg_file, F_UNLCK, &fp->fi_lease);
		fp->fi_lease = NULL;
		fput(fp->fi_deleg_file);
		fp->fi_deleg_file = NULL;
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
static void unhash_stid(struct nfs4_stid *s)
{
	struct idr *stateids = &s->sc_client->cl_stateids;

	idr_remove(stateids, s->sc_stateid.si_opaque.so_id);
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
static void unhash_stid(struct nfs4_stid *s)
{
	s->sc_type = 0;
}

>>>>>>> refs/remotes/origin/master
/* Called under the state lock. */
static void
unhash_delegation(struct nfs4_delegation *dp)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
	unhash_stid(&dp->dl_stid);
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	list_del_init(&dp->dl_perclnt);
	spin_lock(&recall_lock);
	list_del_init(&dp->dl_perfile);
	list_del_init(&dp->dl_recall_lru);
	spin_unlock(&recall_lock);
	nfs4_put_deleg_lease(dp->dl_file);
<<<<<<< HEAD
	nfs4_put_delegation(dp);
}

=======
	put_nfs4_file(dp->dl_file);
	dp->dl_file = NULL;
}



static void destroy_revoked_delegation(struct nfs4_delegation *dp)
{
	list_del_init(&dp->dl_recall_lru);
	remove_stid(&dp->dl_stid);
	nfs4_put_delegation(dp);
}

static void destroy_delegation(struct nfs4_delegation *dp)
{
	unhash_delegation(dp);
	remove_stid(&dp->dl_stid);
	nfs4_put_delegation(dp);
}

static void revoke_delegation(struct nfs4_delegation *dp)
{
	struct nfs4_client *clp = dp->dl_stid.sc_client;

	if (clp->cl_minorversion == 0)
		destroy_delegation(dp);
	else {
		unhash_delegation(dp);
		dp->dl_stid.sc_type = NFS4_REVOKED_DELEG_STID;
		list_add(&dp->dl_recall_lru, &clp->cl_revoked);
	}
}

>>>>>>> refs/remotes/origin/master
/* 
 * SETCLIENTID state 
 */

<<<<<<< HEAD
/* client_lock protects the client lru list and session hash table */
static DEFINE_SPINLOCK(client_lock);

/* Hash tables for nfs4_clientid state */
#define CLIENT_HASH_BITS                 4
#define CLIENT_HASH_SIZE                (1 << CLIENT_HASH_BITS)
#define CLIENT_HASH_MASK                (CLIENT_HASH_SIZE - 1)

<<<<<<< HEAD
#define clientid_hashval(id) \
	((id) & CLIENT_HASH_MASK)
#define clientstr_hashval(name) \
	(opaque_hashval((name), 8) & CLIENT_HASH_MASK)
=======
=======
>>>>>>> refs/remotes/origin/master
static unsigned int clientid_hashval(u32 id)
{
	return id & CLIENT_HASH_MASK;
}

static unsigned int clientstr_hashval(const char *name)
{
	return opaque_hashval(name, 8) & CLIENT_HASH_MASK;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
/*
 * reclaim_str_hashtbl[] holds known client info from previous reset/reboot
 * used in reboot/reset lease grace period processing
 *
 * conf_id_hashtbl[], and conf_str_hashtbl[] hold confirmed
 * setclientid_confirmed info. 
 *
 * unconf_str_hastbl[] and unconf_id_hashtbl[] hold unconfirmed 
 * setclientid info.
 *
 * client_lru holds client queue ordered by nfs4_client.cl_time
 * for lease renewal.
 *
 * close_lru holds (open) stateowner queue ordered by nfs4_stateowner.so_time
 * for last close replay.
 */
static struct list_head	reclaim_str_hashtbl[CLIENT_HASH_SIZE];
static int reclaim_str_hashtbl_size = 0;
static struct list_head	conf_id_hashtbl[CLIENT_HASH_SIZE];
static struct list_head	conf_str_hashtbl[CLIENT_HASH_SIZE];
static struct list_head	unconf_str_hashtbl[CLIENT_HASH_SIZE];
static struct list_head	unconf_id_hashtbl[CLIENT_HASH_SIZE];
static struct list_head client_lru;
static struct list_head close_lru;

=======
>>>>>>> refs/remotes/origin/master
/*
 * We store the NONE, READ, WRITE, and BOTH bits separately in the
 * st_{access,deny}_bmap field of the stateid, in order to track not
 * only what share bits are currently in force, but also what
 * combinations of share bits previous opens have used.  This allows us
 * to enforce the recommendation of rfc 3530 14.2.19 that the server
 * return an error if the client attempt to downgrade to a combination
 * of share bits not explicable by closing some of its previous opens.
 *
 * XXX: This enforcement is actually incomplete, since we don't keep
 * track of access/deny bit combinations; so, e.g., we allow:
 *
 *	OPEN allow read, deny write
 *	OPEN allow both, deny none
 *	DOWNGRADE allow read, deny none
 *
 * which we should reject.
 */
<<<<<<< HEAD
static void
set_access(unsigned int *access, unsigned long bmap) {
	int i;

	*access = 0;
	for (i = 1; i < 4; i++) {
		if (test_bit(i, &bmap))
			*access |= i;
	}
}

static void
set_deny(unsigned int *deny, unsigned long bmap) {
	int i;

	*deny = 0;
	for (i = 0; i < 4; i++) {
		if (test_bit(i, &bmap))
			*deny |= i ;
	}
}

static int
<<<<<<< HEAD
test_share(struct nfs4_stateid *stp, struct nfsd4_open *open) {
=======
test_share(struct nfs4_ol_stateid *stp, struct nfsd4_open *open) {
>>>>>>> refs/remotes/origin/cm-10.0
	unsigned int access, deny;

	set_access(&access, stp->st_access_bmap);
	set_deny(&deny, stp->st_deny_bmap);
	if ((access & open->op_share_deny) || (deny & open->op_share_access))
		return 0;
	return 1;
=======
static unsigned int
bmap_to_share_mode(unsigned long bmap) {
	int i;
	unsigned int access = 0;

	for (i = 1; i < 4; i++) {
		if (test_bit(i, &bmap))
			access |= i;
	}
	return access;
}

static bool
test_share(struct nfs4_ol_stateid *stp, struct nfsd4_open *open) {
	unsigned int access, deny;

	access = bmap_to_share_mode(stp->st_access_bmap);
	deny = bmap_to_share_mode(stp->st_deny_bmap);
	if ((access & open->op_share_deny) || (deny & open->op_share_access))
		return false;
	return true;
}

/* set share access for a given stateid */
static inline void
set_access(u32 access, struct nfs4_ol_stateid *stp)
{
	__set_bit(access, &stp->st_access_bmap);
}

/* clear share access for a given stateid */
static inline void
clear_access(u32 access, struct nfs4_ol_stateid *stp)
{
	__clear_bit(access, &stp->st_access_bmap);
}

/* test whether a given stateid has access */
static inline bool
test_access(u32 access, struct nfs4_ol_stateid *stp)
{
	return test_bit(access, &stp->st_access_bmap);
}

/* set share deny for a given stateid */
static inline void
set_deny(u32 access, struct nfs4_ol_stateid *stp)
{
	__set_bit(access, &stp->st_deny_bmap);
}

/* clear share deny for a given stateid */
static inline void
clear_deny(u32 access, struct nfs4_ol_stateid *stp)
{
	__clear_bit(access, &stp->st_deny_bmap);
}

/* test whether a given stateid is denying specific access */
static inline bool
test_deny(u32 access, struct nfs4_ol_stateid *stp)
{
	return test_bit(access, &stp->st_deny_bmap);
>>>>>>> refs/remotes/origin/master
}

static int nfs4_access_to_omode(u32 access)
{
	switch (access & NFS4_SHARE_ACCESS_BOTH) {
	case NFS4_SHARE_ACCESS_READ:
		return O_RDONLY;
	case NFS4_SHARE_ACCESS_WRITE:
		return O_WRONLY;
	case NFS4_SHARE_ACCESS_BOTH:
		return O_RDWR;
	}
<<<<<<< HEAD
	BUG();
}

<<<<<<< HEAD
static void unhash_generic_stateid(struct nfs4_stateid *stp)
{
	list_del(&stp->st_hash);
=======
static void unhash_generic_stateid(struct nfs4_ol_stateid *stp)
{
>>>>>>> refs/remotes/origin/cm-10.0
=======
	WARN_ON_ONCE(1);
	return O_RDONLY;
}

/* release all access and file references for a given stateid */
static void
release_all_access(struct nfs4_ol_stateid *stp)
{
	int i;

	for (i = 1; i < 4; i++) {
		if (test_access(i, stp))
			nfs4_file_put_access(stp->st_file,
					     nfs4_access_to_omode(i));
		clear_access(i, stp);
	}
}

static void unhash_generic_stateid(struct nfs4_ol_stateid *stp)
{
>>>>>>> refs/remotes/origin/master
	list_del(&stp->st_perfile);
	list_del(&stp->st_perstateowner);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void free_generic_stateid(struct nfs4_stateid *stp)
=======
static void close_generic_stateid(struct nfs4_ol_stateid *stp)
>>>>>>> refs/remotes/origin/cm-10.0
{
	int i;

	if (stp->st_access_bmap) {
		for (i = 1; i < 4; i++) {
			if (test_bit(i, &stp->st_access_bmap))
				nfs4_file_put_access(stp->st_file,
						nfs4_access_to_omode(i));
<<<<<<< HEAD
		}
	}
	put_nfs4_file(stp->st_file);
	kmem_cache_free(stateid_slab, stp);
}

static void release_lock_stateid(struct nfs4_stateid *stp)
=======
			__clear_bit(i, &stp->st_access_bmap);
		}
	}
=======
static void close_generic_stateid(struct nfs4_ol_stateid *stp)
{
	release_all_access(stp);
>>>>>>> refs/remotes/origin/master
	put_nfs4_file(stp->st_file);
	stp->st_file = NULL;
}

static void free_generic_stateid(struct nfs4_ol_stateid *stp)
{
<<<<<<< HEAD
	kmem_cache_free(stateid_slab, stp);
}

static void release_lock_stateid(struct nfs4_ol_stateid *stp)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	remove_stid(&stp->st_stid);
	nfs4_free_stid(stateid_slab, &stp->st_stid);
}

static void release_lock_stateid(struct nfs4_ol_stateid *stp)
>>>>>>> refs/remotes/origin/master
{
	struct file *file;

	unhash_generic_stateid(stp);
<<<<<<< HEAD
<<<<<<< HEAD
	file = find_any_file(stp->st_file);
	if (file)
		locks_remove_posix(file, (fl_owner_t)stp->st_stateowner);
	free_generic_stateid(stp);
}

static void unhash_lockowner(struct nfs4_stateowner *sop)
{
	struct nfs4_stateid *stp;

	list_del(&sop->so_idhash);
	list_del(&sop->so_strhash);
	list_del(&sop->so_perstateid);
	while (!list_empty(&sop->so_stateids)) {
		stp = list_first_entry(&sop->so_stateids,
				struct nfs4_stateid, st_perstateowner);
=======
=======
>>>>>>> refs/remotes/origin/master
	unhash_stid(&stp->st_stid);
	file = find_any_file(stp->st_file);
	if (file)
		locks_remove_posix(file, (fl_owner_t)lockowner(stp->st_stateowner));
	close_generic_stateid(stp);
	free_generic_stateid(stp);
}

static void unhash_lockowner(struct nfs4_lockowner *lo)
{
	struct nfs4_ol_stateid *stp;

	list_del(&lo->lo_owner.so_strhash);
	list_del(&lo->lo_perstateid);
	list_del(&lo->lo_owner_ino_hash);
	while (!list_empty(&lo->lo_owner.so_stateids)) {
		stp = list_first_entry(&lo->lo_owner.so_stateids,
				struct nfs4_ol_stateid, st_perstateowner);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		release_lock_stateid(stp);
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
static void release_lockowner(struct nfs4_stateowner *sop)
{
	unhash_lockowner(sop);
	nfs4_put_stateowner(sop);
}

static void
release_stateid_lockowners(struct nfs4_stateid *open_stp)
{
	struct nfs4_stateowner *lock_sop;

	while (!list_empty(&open_stp->st_lockowners)) {
		lock_sop = list_entry(open_stp->st_lockowners.next,
				struct nfs4_stateowner, so_perstateid);
		/* list_del(&open_stp->st_lockowners);  */
		BUG_ON(lock_sop->so_is_open_owner);
		release_lockowner(lock_sop);
	}
}

static void release_open_stateid(struct nfs4_stateid *stp)
{
	unhash_generic_stateid(stp);
	release_stateid_lockowners(stp);
	free_generic_stateid(stp);
}

static void unhash_openowner(struct nfs4_stateowner *sop)
{
	struct nfs4_stateid *stp;

	list_del(&sop->so_idhash);
	list_del(&sop->so_strhash);
	list_del(&sop->so_perclient);
	list_del(&sop->so_perstateid); /* XXX: necessary? */
	while (!list_empty(&sop->so_stateids)) {
		stp = list_first_entry(&sop->so_stateids,
				struct nfs4_stateid, st_perstateowner);
=======
=======
>>>>>>> refs/remotes/origin/master
static void release_lockowner(struct nfs4_lockowner *lo)
{
	unhash_lockowner(lo);
	nfs4_free_lockowner(lo);
}

static void
release_stateid_lockowners(struct nfs4_ol_stateid *open_stp)
{
	struct nfs4_lockowner *lo;

	while (!list_empty(&open_stp->st_lockowners)) {
		lo = list_entry(open_stp->st_lockowners.next,
				struct nfs4_lockowner, lo_perstateid);
		release_lockowner(lo);
	}
}

static void unhash_open_stateid(struct nfs4_ol_stateid *stp)
{
	unhash_generic_stateid(stp);
	release_stateid_lockowners(stp);
	close_generic_stateid(stp);
}

static void release_open_stateid(struct nfs4_ol_stateid *stp)
{
	unhash_open_stateid(stp);
<<<<<<< HEAD
	unhash_stid(&stp->st_stid);
=======
>>>>>>> refs/remotes/origin/master
	free_generic_stateid(stp);
}

static void unhash_openowner(struct nfs4_openowner *oo)
{
	struct nfs4_ol_stateid *stp;

	list_del(&oo->oo_owner.so_strhash);
	list_del(&oo->oo_perclient);
	while (!list_empty(&oo->oo_owner.so_stateids)) {
		stp = list_first_entry(&oo->oo_owner.so_stateids,
				struct nfs4_ol_stateid, st_perstateowner);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		release_open_stateid(stp);
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
static void release_openowner(struct nfs4_stateowner *sop)
{
	unhash_openowner(sop);
	list_del(&sop->so_close_lru);
	nfs4_put_stateowner(sop);
=======
=======
>>>>>>> refs/remotes/origin/master
static void release_last_closed_stateid(struct nfs4_openowner *oo)
{
	struct nfs4_ol_stateid *s = oo->oo_last_closed_stid;

	if (s) {
<<<<<<< HEAD
		unhash_stid(&s->st_stid);
=======
>>>>>>> refs/remotes/origin/master
		free_generic_stateid(s);
		oo->oo_last_closed_stid = NULL;
	}
}

static void release_openowner(struct nfs4_openowner *oo)
{
	unhash_openowner(oo);
	list_del(&oo->oo_close_lru);
	release_last_closed_stateid(oo);
	nfs4_free_openowner(oo);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
}

#define SESSION_HASH_SIZE	512
static struct list_head sessionid_hashtbl[SESSION_HASH_SIZE];

=======
}

>>>>>>> refs/remotes/origin/master
static inline int
hash_sessionid(struct nfs4_sessionid *sessionid)
{
	struct nfsd4_sessionid *sid = (struct nfsd4_sessionid *)sessionid;

	return sid->sequence % SESSION_HASH_SIZE;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
#ifdef NFSD_DEBUG
>>>>>>> refs/remotes/origin/cm-10.0
=======
#ifdef NFSD_DEBUG
>>>>>>> refs/remotes/origin/master
static inline void
dump_sessionid(const char *fn, struct nfs4_sessionid *sessionid)
{
	u32 *ptr = (u32 *)(&sessionid->data[0]);
	dprintk("%s: %u:%u:%u:%u\n", fn, ptr[0], ptr[1], ptr[2], ptr[3]);
}
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
#else
static inline void
dump_sessionid(const char *fn, struct nfs4_sessionid *sessionid)
{
}
#endif

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
/*
 * Bump the seqid on cstate->replay_owner, and clear replay_owner if it
 * won't be used for replay.
 */
void nfsd4_bump_seqid(struct nfsd4_compound_state *cstate, __be32 nfserr)
{
	struct nfs4_stateowner *so = cstate->replay_owner;

	if (nfserr == nfserr_replay_me)
		return;

	if (!seqid_mutating_err(ntohl(nfserr))) {
		cstate->replay_owner = NULL;
		return;
	}
	if (!so)
		return;
	if (so->so_is_open_owner)
		release_last_closed_stateid(openowner(so));
	so->so_seqid++;
	return;
}
>>>>>>> refs/remotes/origin/master

static void
gen_sessionid(struct nfsd4_session *ses)
{
	struct nfs4_client *clp = ses->se_client;
	struct nfsd4_sessionid *sid;

	sid = (struct nfsd4_sessionid *)ses->se_sessionid.data;
	sid->clientid = clp->cl_clientid;
	sid->sequence = current_sessionid++;
	sid->reserved = 0;
}

/*
 * The protocol defines ca_maxresponssize_cached to include the size of
 * the rpc header, but all we need to cache is the data starting after
 * the end of the initial SEQUENCE operation--the rest we regenerate
 * each time.  Therefore we can advertise a ca_maxresponssize_cached
 * value that is the number of bytes in our cache plus a few additional
 * bytes.  In order to stay on the safe side, and not promise more than
 * we can cache, those additional bytes must be the minimum possible: 24
 * bytes of rpc header (xid through accept state, with AUTH_NULL
 * verifier), 12 for the compound header (with zero-length tag), and 44
 * for the SEQUENCE op response:
 */
#define NFSD_MIN_HDR_SEQ_SZ  (24 + 12 + 44)

static void
free_session_slots(struct nfsd4_session *ses)
{
	int i;

	for (i = 0; i < ses->se_fchannel.maxreqs; i++)
		kfree(ses->se_slots[i]);
}

/*
 * We don't actually need to cache the rpc and session headers, so we
 * can allocate a little less for each slot:
 */
<<<<<<< HEAD
static inline int slot_bytes(struct nfsd4_channel_attrs *ca)
{
	return ca->maxresp_cached - NFSD_MIN_HDR_SEQ_SZ;
}

static int nfsd4_sanitize_slot_size(u32 size)
{
	size -= NFSD_MIN_HDR_SEQ_SZ; /* We don't cache the rpc header */
	size = min_t(u32, size, NFSD_SLOT_CACHE_SIZE);

	return size;
=======
static inline u32 slot_bytes(struct nfsd4_channel_attrs *ca)
{
	u32 size;

	if (ca->maxresp_cached < NFSD_MIN_HDR_SEQ_SZ)
		size = 0;
	else
		size = ca->maxresp_cached - NFSD_MIN_HDR_SEQ_SZ;
	return size + sizeof(struct nfsd4_slot);
>>>>>>> refs/remotes/origin/master
}

/*
 * XXX: If we run out of reserved DRC memory we could (up to a point)
 * re-negotiate active sessions and reduce their slot usage to make
<<<<<<< HEAD
<<<<<<< HEAD
 * rooom for new connections. For now we just fail the create session.
=======
 * room for new connections. For now we just fail the create session.
>>>>>>> refs/remotes/origin/cm-10.0
 */
static int nfsd4_get_drc_mem(int slotsize, u32 num)
{
	int avail;

	num = min_t(u32, num, NFSD_MAX_SLOTS_PER_SESSION);

	spin_lock(&nfsd_drc_lock);
	avail = min_t(int, NFSD_MAX_MEM_PER_SESSION,
			nfsd_drc_max_mem - nfsd_drc_mem_used);
=======
 * room for new connections. For now we just fail the create session.
 */
static u32 nfsd4_get_drc_mem(struct nfsd4_channel_attrs *ca)
{
	u32 slotsize = slot_bytes(ca);
	u32 num = ca->maxreqs;
	int avail;

	spin_lock(&nfsd_drc_lock);
	avail = min((unsigned long)NFSD_MAX_MEM_PER_SESSION,
		    nfsd_drc_max_mem - nfsd_drc_mem_used);
>>>>>>> refs/remotes/origin/master
	num = min_t(int, num, avail / slotsize);
	nfsd_drc_mem_used += num * slotsize;
	spin_unlock(&nfsd_drc_lock);

	return num;
}

<<<<<<< HEAD
static void nfsd4_put_drc_mem(int slotsize, int num)
{
	spin_lock(&nfsd_drc_lock);
	nfsd_drc_mem_used -= slotsize * num;
	spin_unlock(&nfsd_drc_lock);
}

static struct nfsd4_session *alloc_session(int slotsize, int numslots)
{
=======
static void nfsd4_put_drc_mem(struct nfsd4_channel_attrs *ca)
{
	int slotsize = slot_bytes(ca);

	spin_lock(&nfsd_drc_lock);
	nfsd_drc_mem_used -= slotsize * ca->maxreqs;
	spin_unlock(&nfsd_drc_lock);
}

static struct nfsd4_session *alloc_session(struct nfsd4_channel_attrs *attrs)
{
	int numslots = attrs->maxreqs;
	int slotsize = slot_bytes(attrs);
>>>>>>> refs/remotes/origin/master
	struct nfsd4_session *new;
	int mem, i;

	BUILD_BUG_ON(NFSD_MAX_SLOTS_PER_SESSION * sizeof(struct nfsd4_slot *)
			+ sizeof(struct nfsd4_session) > PAGE_SIZE);
	mem = numslots * sizeof(struct nfsd4_slot *);

	new = kzalloc(sizeof(*new) + mem, GFP_KERNEL);
	if (!new)
		return NULL;
	/* allocate each struct nfsd4_slot and data cache in one piece */
	for (i = 0; i < numslots; i++) {
<<<<<<< HEAD
		mem = sizeof(struct nfsd4_slot) + slotsize;
		new->se_slots[i] = kzalloc(mem, GFP_KERNEL);
=======
		new->se_slots[i] = kzalloc(slotsize, GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
		if (!new->se_slots[i])
			goto out_free;
	}
	return new;
out_free:
	while (i--)
		kfree(new->se_slots[i]);
	kfree(new);
	return NULL;
}

<<<<<<< HEAD
static void init_forechannel_attrs(struct nfsd4_channel_attrs *new, struct nfsd4_channel_attrs *req, int numslots, int slotsize)
{
	u32 maxrpc = nfsd_serv->sv_max_mesg;

	new->maxreqs = numslots;
	new->maxresp_cached = min_t(u32, req->maxresp_cached,
					slotsize + NFSD_MIN_HDR_SEQ_SZ);
	new->maxreq_sz = min_t(u32, req->maxreq_sz, maxrpc);
	new->maxresp_sz = min_t(u32, req->maxresp_sz, maxrpc);
	new->maxops = min_t(u32, req->maxops, NFSD_MAX_OPS_PER_COMPOUND);
}

static void free_conn(struct nfsd4_conn *c)
=======
static void free_conn(struct nfsd4_conn *c)
>>>>>>> refs/remotes/origin/master
{
	svc_xprt_put(c->cn_xprt);
	kfree(c);
}

static void nfsd4_conn_lost(struct svc_xpt_user *u)
{
	struct nfsd4_conn *c = container_of(u, struct nfsd4_conn, cn_xpt_user);
	struct nfs4_client *clp = c->cn_session->se_client;

	spin_lock(&clp->cl_lock);
	if (!list_empty(&c->cn_persession)) {
		list_del(&c->cn_persession);
		free_conn(c);
	}
<<<<<<< HEAD
	spin_unlock(&clp->cl_lock);
	nfsd4_probe_callback(clp);
=======
	nfsd4_probe_callback(clp);
	spin_unlock(&clp->cl_lock);
>>>>>>> refs/remotes/origin/master
}

static struct nfsd4_conn *alloc_conn(struct svc_rqst *rqstp, u32 flags)
{
	struct nfsd4_conn *conn;

	conn = kmalloc(sizeof(struct nfsd4_conn), GFP_KERNEL);
	if (!conn)
		return NULL;
	svc_xprt_get(rqstp->rq_xprt);
	conn->cn_xprt = rqstp->rq_xprt;
	conn->cn_flags = flags;
	INIT_LIST_HEAD(&conn->cn_xpt_user.list);
	return conn;
}

static void __nfsd4_hash_conn(struct nfsd4_conn *conn, struct nfsd4_session *ses)
{
	conn->cn_session = ses;
	list_add(&conn->cn_persession, &ses->se_conns);
}

static void nfsd4_hash_conn(struct nfsd4_conn *conn, struct nfsd4_session *ses)
{
	struct nfs4_client *clp = ses->se_client;

	spin_lock(&clp->cl_lock);
	__nfsd4_hash_conn(conn, ses);
	spin_unlock(&clp->cl_lock);
}

static int nfsd4_register_conn(struct nfsd4_conn *conn)
{
	conn->cn_xpt_user.callback = nfsd4_conn_lost;
	return register_xpt_user(conn->cn_xprt, &conn->cn_xpt_user);
}

<<<<<<< HEAD
static __be32 nfsd4_new_conn(struct svc_rqst *rqstp, struct nfsd4_session *ses, u32 dir)
{
	struct nfsd4_conn *conn;
	int ret;

	conn = alloc_conn(rqstp, dir);
	if (!conn)
		return nfserr_jukebox;
=======
static void nfsd4_init_conn(struct svc_rqst *rqstp, struct nfsd4_conn *conn, struct nfsd4_session *ses)
{
	int ret;

>>>>>>> refs/remotes/origin/master
	nfsd4_hash_conn(conn, ses);
	ret = nfsd4_register_conn(conn);
	if (ret)
		/* oops; xprt is already down: */
		nfsd4_conn_lost(&conn->cn_xpt_user);
<<<<<<< HEAD
	return nfs_ok;
}

static __be32 nfsd4_new_conn_from_crses(struct svc_rqst *rqstp, struct nfsd4_session *ses)
{
	u32 dir = NFS4_CDFC4_FORE;

	if (ses->se_flags & SESSION4_BACK_CHAN)
		dir |= NFS4_CDFC4_BACK;

	return nfsd4_new_conn(rqstp, ses, dir);
=======
	if (conn->cn_flags & NFS4_CDFC4_BACK) {
		/* callback channel may be back up */
		nfsd4_probe_callback(ses->se_client);
	}
}

static struct nfsd4_conn *alloc_conn_from_crses(struct svc_rqst *rqstp, struct nfsd4_create_session *cses)
{
	u32 dir = NFS4_CDFC4_FORE;

	if (cses->flags & SESSION4_BACK_CHAN)
		dir |= NFS4_CDFC4_BACK;
	return alloc_conn(rqstp, dir);
>>>>>>> refs/remotes/origin/master
}

/* must be called under client_lock */
static void nfsd4_del_conns(struct nfsd4_session *s)
{
	struct nfs4_client *clp = s->se_client;
	struct nfsd4_conn *c;

	spin_lock(&clp->cl_lock);
	while (!list_empty(&s->se_conns)) {
		c = list_first_entry(&s->se_conns, struct nfsd4_conn, cn_persession);
		list_del_init(&c->cn_persession);
		spin_unlock(&clp->cl_lock);

		unregister_xpt_user(c->cn_xprt, &c->cn_xpt_user);
		free_conn(c);

		spin_lock(&clp->cl_lock);
	}
	spin_unlock(&clp->cl_lock);
}

<<<<<<< HEAD
<<<<<<< HEAD
void free_session(struct kref *kref)
=======
static void free_session(struct kref *kref)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct nfsd4_session *ses;
	int mem;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	lockdep_assert_held(&client_lock);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	lockdep_assert_held(&client_lock);
>>>>>>> refs/remotes/origin/cm-11.0
	ses = container_of(kref, struct nfsd4_session, se_ref);
	nfsd4_del_conns(ses);
	spin_lock(&nfsd_drc_lock);
	mem = ses->se_fchannel.maxreqs * slot_bytes(&ses->se_fchannel);
	nfsd_drc_mem_used -= mem;
	spin_unlock(&nfsd_drc_lock);
=======
static void __free_session(struct nfsd4_session *ses)
{
>>>>>>> refs/remotes/origin/master
	free_session_slots(ses);
	kfree(ses);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
void nfsd4_put_session(struct nfsd4_session *ses)
{
	spin_lock(&client_lock);
	nfsd4_put_session_locked(ses);
	spin_unlock(&client_lock);
}

>>>>>>> refs/remotes/origin/cm-10.0
static struct nfsd4_session *alloc_init_session(struct svc_rqst *rqstp, struct nfs4_client *clp, struct nfsd4_create_session *cses)
{
	struct nfsd4_session *new;
	struct nfsd4_channel_attrs *fchan = &cses->fore_channel;
	int numslots, slotsize;
	int status;
	int idx;

	/*
	 * Note decreasing slot size below client's request may
	 * make it difficult for client to function correctly, whereas
	 * decreasing the number of slots will (just?) affect
	 * performance.  When short on memory we therefore prefer to
	 * decrease number of slots instead of their size.
	 */
	slotsize = nfsd4_sanitize_slot_size(fchan->maxresp_cached);
	numslots = nfsd4_get_drc_mem(slotsize, fchan->maxreqs);
	if (numslots < 1)
		return NULL;

	new = alloc_session(slotsize, numslots);
	if (!new) {
		nfsd4_put_drc_mem(slotsize, fchan->maxreqs);
		return NULL;
	}
	init_forechannel_attrs(&new->se_fchannel, fchan, numslots, slotsize);
=======
static void free_session(struct nfsd4_session *ses)
{
	struct nfsd_net *nn = net_generic(ses->se_client->net, nfsd_net_id);

	lockdep_assert_held(&nn->client_lock);
	nfsd4_del_conns(ses);
	nfsd4_put_drc_mem(&ses->se_fchannel);
	__free_session(ses);
}

static void init_session(struct svc_rqst *rqstp, struct nfsd4_session *new, struct nfs4_client *clp, struct nfsd4_create_session *cses)
{
	int idx;
	struct nfsd_net *nn = net_generic(SVC_NET(rqstp), nfsd_net_id);
>>>>>>> refs/remotes/origin/master

	new->se_client = clp;
	gen_sessionid(new);

	INIT_LIST_HEAD(&new->se_conns);

	new->se_cb_seq_nr = 1;
	new->se_flags = cses->flags;
	new->se_cb_prog = cses->callback_prog;
<<<<<<< HEAD
	kref_init(&new->se_ref);
	idx = hash_sessionid(&new->se_sessionid);
	spin_lock(&client_lock);
	list_add(&new->se_hash, &sessionid_hashtbl[idx]);
	spin_lock(&clp->cl_lock);
	list_add(&new->se_perclnt, &clp->cl_sessions);
	spin_unlock(&clp->cl_lock);
	spin_unlock(&client_lock);

	status = nfsd4_new_conn_from_crses(rqstp, new);
	/* whoops: benny points out, status is ignored! (err, or bogus) */
	if (status) {
<<<<<<< HEAD
		free_session(&new->se_ref);
=======
		spin_lock(&client_lock);
		free_session(&new->se_ref);
		spin_unlock(&client_lock);
>>>>>>> refs/remotes/origin/cm-10.0
		return NULL;
	}
=======
	new->se_cb_sec = cses->cb_sec;
	atomic_set(&new->se_ref, 0);
	idx = hash_sessionid(&new->se_sessionid);
	spin_lock(&nn->client_lock);
	list_add(&new->se_hash, &nn->sessionid_hashtbl[idx]);
	spin_lock(&clp->cl_lock);
	list_add(&new->se_perclnt, &clp->cl_sessions);
	spin_unlock(&clp->cl_lock);
	spin_unlock(&nn->client_lock);
	memcpy(&new->se_fchannel, &cses->fore_channel,
			sizeof(struct nfsd4_channel_attrs));
>>>>>>> refs/remotes/origin/master
	if (cses->flags & SESSION4_BACK_CHAN) {
		struct sockaddr *sa = svc_addr(rqstp);
		/*
		 * This is a little silly; with sessions there's no real
		 * use for the callback address.  Use the peer address
		 * as a reasonable default for now, but consider fixing
		 * the rpc client not to require an address in the
		 * future:
		 */
		rpc_copy_addr((struct sockaddr *)&clp->cl_cb_conn.cb_addr, sa);
		clp->cl_cb_conn.cb_addrlen = svc_addr_len(sa);
	}
<<<<<<< HEAD
	nfsd4_probe_callback(clp);
	return new;
=======
>>>>>>> refs/remotes/origin/master
}

/* caller must hold client_lock */
static struct nfsd4_session *
<<<<<<< HEAD
find_in_sessionid_hashtbl(struct nfs4_sessionid *sessionid)
{
	struct nfsd4_session *elem;
	int idx;
=======
find_in_sessionid_hashtbl(struct nfs4_sessionid *sessionid, struct net *net)
{
	struct nfsd4_session *elem;
	int idx;
	struct nfsd_net *nn = net_generic(net, nfsd_net_id);
>>>>>>> refs/remotes/origin/master

	dump_sessionid(__func__, sessionid);
	idx = hash_sessionid(sessionid);
	/* Search in the appropriate list */
<<<<<<< HEAD
	list_for_each_entry(elem, &sessionid_hashtbl[idx], se_hash) {
=======
	list_for_each_entry(elem, &nn->sessionid_hashtbl[idx], se_hash) {
>>>>>>> refs/remotes/origin/master
		if (!memcmp(elem->se_sessionid.data, sessionid->data,
			    NFS4_MAX_SESSIONID_LEN)) {
			return elem;
		}
	}

	dprintk("%s: session not found\n", __func__);
	return NULL;
}

/* caller must hold client_lock */
static void
unhash_session(struct nfsd4_session *ses)
{
	list_del(&ses->se_hash);
	spin_lock(&ses->se_client->cl_lock);
	list_del(&ses->se_perclnt);
	spin_unlock(&ses->se_client->cl_lock);
}

<<<<<<< HEAD
/* must be called under the client_lock */
static inline void
renew_client_locked(struct nfs4_client *clp)
{
	if (is_client_expired(clp)) {
		dprintk("%s: client (clientid %08x/%08x) already expired\n",
			__func__,
			clp->cl_clientid.cl_boot,
			clp->cl_clientid.cl_id);
		return;
	}

<<<<<<< HEAD
	/*
	* Move client to the end to the LRU list.
	*/
=======
>>>>>>> refs/remotes/origin/cm-10.0
	dprintk("renewing client (clientid %08x/%08x)\n", 
			clp->cl_clientid.cl_boot, 
			clp->cl_clientid.cl_id);
	list_move_tail(&clp->cl_lru, &client_lru);
	clp->cl_time = get_seconds();
}

static inline void
renew_client(struct nfs4_client *clp)
{
	spin_lock(&client_lock);
	renew_client_locked(clp);
	spin_unlock(&client_lock);
}

/* SETCLIENTID and SETCLIENTID_CONFIRM Helper functions */
static int
STALE_CLIENTID(clientid_t *clid)
{
	if (clid->cl_boot == boot_time)
		return 0;
	dprintk("NFSD stale clientid (%08x/%08x) boot_time %08lx\n",
		clid->cl_boot, clid->cl_id, boot_time);
=======
/* SETCLIENTID and SETCLIENTID_CONFIRM Helper functions */
static int
STALE_CLIENTID(clientid_t *clid, struct nfsd_net *nn)
{
	if (clid->cl_boot == nn->boot_time)
		return 0;
	dprintk("NFSD stale clientid (%08x/%08x) boot_time %08lx\n",
		clid->cl_boot, clid->cl_id, nn->boot_time);
>>>>>>> refs/remotes/origin/master
	return 1;
}

/* 
 * XXX Should we use a slab cache ?
 * This type of memory management is somewhat inefficient, but we use it
 * anyway since SETCLIENTID is not a common operation.
 */
static struct nfs4_client *alloc_client(struct xdr_netobj name)
{
	struct nfs4_client *clp;

	clp = kzalloc(sizeof(struct nfs4_client), GFP_KERNEL);
	if (clp == NULL)
		return NULL;
<<<<<<< HEAD
<<<<<<< HEAD
	clp->cl_name.data = kmalloc(name.len, GFP_KERNEL);
=======
	clp->cl_name.data = kmemdup(name.data, name.len, GFP_KERNEL);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	clp->cl_name.data = kmemdup(name.data, name.len, GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (clp->cl_name.data == NULL) {
		kfree(clp);
		return NULL;
	}
<<<<<<< HEAD
<<<<<<< HEAD
	memcpy(clp->cl_name.data, name.data, name.len);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	clp->cl_name.len = name.len;
	return clp;
}

static inline void
free_client(struct nfs4_client *clp)
{
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
	lockdep_assert_held(&client_lock);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct nfsd_net __maybe_unused *nn = net_generic(clp->net, nfsd_net_id);

	lockdep_assert_held(&nn->client_lock);
>>>>>>> refs/remotes/origin/master
=======
	lockdep_assert_held(&client_lock);
>>>>>>> refs/remotes/origin/cm-11.0
	while (!list_empty(&clp->cl_sessions)) {
		struct nfsd4_session *ses;
		ses = list_entry(clp->cl_sessions.next, struct nfsd4_session,
				se_perclnt);
		list_del(&ses->se_perclnt);
<<<<<<< HEAD
<<<<<<< HEAD
		nfsd4_put_session(ses);
=======
		nfsd4_put_session_locked(ses);
>>>>>>> refs/remotes/origin/cm-10.0
	}
	if (clp->cl_cred.cr_group_info)
		put_group_info(clp->cl_cred.cr_group_info);
	kfree(clp->cl_principal);
	kfree(clp->cl_name.data);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	idr_remove_all(&clp->cl_stateids);
	idr_destroy(&clp->cl_stateids);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	idr_remove_all(&clp->cl_stateids);
	idr_destroy(&clp->cl_stateids);
>>>>>>> refs/remotes/origin/cm-11.0
	kfree(clp);
}

void
release_session_client(struct nfsd4_session *session)
{
	struct nfs4_client *clp = session->se_client;

	if (!atomic_dec_and_lock(&clp->cl_refcount, &client_lock))
		return;
	if (is_client_expired(clp)) {
		free_client(clp);
		session->se_client = NULL;
	} else
		renew_client_locked(clp);
	spin_unlock(&client_lock);
}

=======
		WARN_ON_ONCE(atomic_read(&ses->se_ref));
		free_session(ses);
	}
	free_svc_cred(&clp->cl_cred);
	kfree(clp->cl_name.data);
	idr_destroy(&clp->cl_stateids);
	kfree(clp);
}

>>>>>>> refs/remotes/origin/master
/* must be called under the client_lock */
static inline void
unhash_client_locked(struct nfs4_client *clp)
{
	struct nfsd4_session *ses;

<<<<<<< HEAD
	mark_client_expired(clp);
=======
>>>>>>> refs/remotes/origin/master
	list_del(&clp->cl_lru);
	spin_lock(&clp->cl_lock);
	list_for_each_entry(ses, &clp->cl_sessions, se_perclnt)
		list_del_init(&ses->se_hash);
	spin_unlock(&clp->cl_lock);
}

static void
<<<<<<< HEAD
expire_client(struct nfs4_client *clp)
{
<<<<<<< HEAD
	struct nfs4_stateowner *sop;
=======
	struct nfs4_openowner *oo;
>>>>>>> refs/remotes/origin/cm-10.0
	struct nfs4_delegation *dp;
	struct list_head reaplist;
=======
destroy_client(struct nfs4_client *clp)
{
	struct nfs4_openowner *oo;
	struct nfs4_delegation *dp;
	struct list_head reaplist;
	struct nfsd_net *nn = net_generic(clp->net, nfsd_net_id);
>>>>>>> refs/remotes/origin/master

	INIT_LIST_HEAD(&reaplist);
	spin_lock(&recall_lock);
	while (!list_empty(&clp->cl_delegations)) {
		dp = list_entry(clp->cl_delegations.next, struct nfs4_delegation, dl_perclnt);
		list_del_init(&dp->dl_perclnt);
		list_move(&dp->dl_recall_lru, &reaplist);
	}
	spin_unlock(&recall_lock);
	while (!list_empty(&reaplist)) {
		dp = list_entry(reaplist.next, struct nfs4_delegation, dl_recall_lru);
<<<<<<< HEAD
<<<<<<< HEAD
		list_del_init(&dp->dl_recall_lru);
		unhash_delegation(dp);
	}
	while (!list_empty(&clp->cl_openowners)) {
		sop = list_entry(clp->cl_openowners.next, struct nfs4_stateowner, so_perclient);
		release_openowner(sop);
=======
		unhash_delegation(dp);
=======
		destroy_delegation(dp);
	}
	list_splice_init(&clp->cl_revoked, &reaplist);
	while (!list_empty(&reaplist)) {
		dp = list_entry(reaplist.next, struct nfs4_delegation, dl_recall_lru);
		destroy_revoked_delegation(dp);
>>>>>>> refs/remotes/origin/master
	}
	while (!list_empty(&clp->cl_openowners)) {
		oo = list_entry(clp->cl_openowners.next, struct nfs4_openowner, oo_perclient);
		release_openowner(oo);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}
	nfsd4_shutdown_callback(clp);
	if (clp->cl_cb_conn.cb_xprt)
		svc_xprt_put(clp->cl_cb_conn.cb_xprt);
	list_del(&clp->cl_idhash);
<<<<<<< HEAD
	list_del(&clp->cl_strhash);
	spin_lock(&client_lock);
	unhash_client_locked(clp);
	if (atomic_read(&clp->cl_refcount) == 0)
		free_client(clp);
	spin_unlock(&client_lock);
=======
	if (test_bit(NFSD4_CLIENT_CONFIRMED, &clp->cl_flags))
		rb_erase(&clp->cl_namenode, &nn->conf_name_tree);
	else
		rb_erase(&clp->cl_namenode, &nn->unconf_name_tree);
	spin_lock(&nn->client_lock);
	unhash_client_locked(clp);
	WARN_ON_ONCE(atomic_read(&clp->cl_refcount));
	free_client(clp);
	spin_unlock(&nn->client_lock);
}

static void expire_client(struct nfs4_client *clp)
{
	nfsd4_client_record_remove(clp);
	destroy_client(clp);
>>>>>>> refs/remotes/origin/master
}

static void copy_verf(struct nfs4_client *target, nfs4_verifier *source)
{
	memcpy(target->cl_verifier.data, source->data,
			sizeof(target->cl_verifier.data));
}

static void copy_clid(struct nfs4_client *target, struct nfs4_client *source)
{
	target->cl_clientid.cl_boot = source->cl_clientid.cl_boot; 
	target->cl_clientid.cl_id = source->cl_clientid.cl_id; 
}

<<<<<<< HEAD
static void copy_cred(struct svc_cred *target, struct svc_cred *source)
{
=======
static int copy_cred(struct svc_cred *target, struct svc_cred *source)
{
	if (source->cr_principal) {
		target->cr_principal =
				kstrdup(source->cr_principal, GFP_KERNEL);
		if (target->cr_principal == NULL)
			return -ENOMEM;
	} else
		target->cr_principal = NULL;
	target->cr_flavor = source->cr_flavor;
>>>>>>> refs/remotes/origin/master
	target->cr_uid = source->cr_uid;
	target->cr_gid = source->cr_gid;
	target->cr_group_info = source->cr_group_info;
	get_group_info(target->cr_group_info);
<<<<<<< HEAD
=======
	target->cr_gss_mech = source->cr_gss_mech;
	if (source->cr_gss_mech)
		gss_mech_get(source->cr_gss_mech);
	return 0;
}

static long long
compare_blob(const struct xdr_netobj *o1, const struct xdr_netobj *o2)
{
	long long res;

	res = o1->len - o2->len;
	if (res)
		return res;
	return (long long)memcmp(o1->data, o2->data, o1->len);
>>>>>>> refs/remotes/origin/master
}

static int same_name(const char *n1, const char *n2)
{
	return 0 == memcmp(n1, n2, HEXDIR_LEN);
}

static int
same_verf(nfs4_verifier *v1, nfs4_verifier *v2)
{
	return 0 == memcmp(v1->data, v2->data, sizeof(v1->data));
}

static int
same_clid(clientid_t *cl1, clientid_t *cl2)
{
	return (cl1->cl_boot == cl2->cl_boot) && (cl1->cl_id == cl2->cl_id);
}

<<<<<<< HEAD
/* XXX what about NGROUP */
static int
same_creds(struct svc_cred *cr1, struct svc_cred *cr2)
{
	return cr1->cr_uid == cr2->cr_uid;
}

static void gen_clid(struct nfs4_client *clp)
{
	static u32 current_clientid = 1;

	clp->cl_clientid.cl_boot = boot_time;
=======
static bool groups_equal(struct group_info *g1, struct group_info *g2)
{
	int i;

	if (g1->ngroups != g2->ngroups)
		return false;
	for (i=0; i<g1->ngroups; i++)
		if (!gid_eq(GROUP_AT(g1, i), GROUP_AT(g2, i)))
			return false;
	return true;
}

/*
 * RFC 3530 language requires clid_inuse be returned when the
 * "principal" associated with a requests differs from that previously
 * used.  We use uid, gid's, and gss principal string as our best
 * approximation.  We also don't want to allow non-gss use of a client
 * established using gss: in theory cr_principal should catch that
 * change, but in practice cr_principal can be null even in the gss case
 * since gssd doesn't always pass down a principal string.
 */
static bool is_gss_cred(struct svc_cred *cr)
{
	/* Is cr_flavor one of the gss "pseudoflavors"?: */
	return (cr->cr_flavor > RPC_AUTH_MAXFLAVOR);
}


static bool
same_creds(struct svc_cred *cr1, struct svc_cred *cr2)
{
	if ((is_gss_cred(cr1) != is_gss_cred(cr2))
		|| (!uid_eq(cr1->cr_uid, cr2->cr_uid))
		|| (!gid_eq(cr1->cr_gid, cr2->cr_gid))
		|| !groups_equal(cr1->cr_group_info, cr2->cr_group_info))
		return false;
	if (cr1->cr_principal == cr2->cr_principal)
		return true;
	if (!cr1->cr_principal || !cr2->cr_principal)
		return false;
	return 0 == strcmp(cr1->cr_principal, cr2->cr_principal);
}

static bool svc_rqst_integrity_protected(struct svc_rqst *rqstp)
{
	struct svc_cred *cr = &rqstp->rq_cred;
	u32 service;

	if (!cr->cr_gss_mech)
		return false;
	service = gss_pseudoflavor_to_service(cr->cr_gss_mech, cr->cr_flavor);
	return service == RPC_GSS_SVC_INTEGRITY ||
	       service == RPC_GSS_SVC_PRIVACY;
}

static bool mach_creds_match(struct nfs4_client *cl, struct svc_rqst *rqstp)
{
	struct svc_cred *cr = &rqstp->rq_cred;

	if (!cl->cl_mach_cred)
		return true;
	if (cl->cl_cred.cr_gss_mech != cr->cr_gss_mech)
		return false;
	if (!svc_rqst_integrity_protected(rqstp))
		return false;
	if (!cr->cr_principal)
		return false;
	return 0 == strcmp(cl->cl_cred.cr_principal, cr->cr_principal);
}

static void gen_clid(struct nfs4_client *clp, struct nfsd_net *nn)
{
	static u32 current_clientid = 1;

	clp->cl_clientid.cl_boot = nn->boot_time;
>>>>>>> refs/remotes/origin/master
	clp->cl_clientid.cl_id = current_clientid++; 
}

static void gen_confirm(struct nfs4_client *clp)
{
<<<<<<< HEAD
<<<<<<< HEAD
	static u32 i;
	u32 *p;

	p = (u32 *)clp->cl_confirm.data;
	*p++ = get_seconds();
	*p++ = i++;
=======
=======
>>>>>>> refs/remotes/origin/master
	__be32 verf[2];
	static u32 i;

	verf[0] = (__be32)get_seconds();
	verf[1] = (__be32)i++;
	memcpy(clp->cl_confirm.data, verf, sizeof(clp->cl_confirm.data));
}

static struct nfs4_stid *find_stateid(struct nfs4_client *cl, stateid_t *t)
{
<<<<<<< HEAD
	return idr_find(&cl->cl_stateids, t->si_opaque.so_id);
=======
	struct nfs4_stid *ret;

	ret = idr_find(&cl->cl_stateids, t->si_opaque.so_id);
	if (!ret || !ret->sc_type)
		return NULL;
	return ret;
>>>>>>> refs/remotes/origin/master
}

static struct nfs4_stid *find_stateid_by_type(struct nfs4_client *cl, stateid_t *t, char typemask)
{
	struct nfs4_stid *s;

	s = find_stateid(cl, t);
	if (!s)
		return NULL;
	if (typemask & s->sc_type)
		return s;
	return NULL;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
}

static struct nfs4_client *create_client(struct xdr_netobj name, char *recdir,
=======
}

static struct nfs4_client *create_client(struct xdr_netobj name,
>>>>>>> refs/remotes/origin/master
		struct svc_rqst *rqstp, nfs4_verifier *verf)
{
	struct nfs4_client *clp;
	struct sockaddr *sa = svc_addr(rqstp);
<<<<<<< HEAD
	char *princ;
=======
	int ret;
	struct net *net = SVC_NET(rqstp);
	struct nfsd_net *nn = net_generic(net, nfsd_net_id);
>>>>>>> refs/remotes/origin/master

	clp = alloc_client(name);
	if (clp == NULL)
		return NULL;

	INIT_LIST_HEAD(&clp->cl_sessions);
<<<<<<< HEAD

	princ = svc_gss_principal(rqstp);
	if (princ) {
		clp->cl_principal = kstrdup(princ, GFP_KERNEL);
		if (clp->cl_principal == NULL) {
<<<<<<< HEAD
			free_client(clp);
=======
			spin_lock(&client_lock);
			free_client(clp);
			spin_unlock(&client_lock);
>>>>>>> refs/remotes/origin/cm-10.0
			return NULL;
		}
	}

<<<<<<< HEAD
=======
	idr_init(&clp->cl_stateids);
>>>>>>> refs/remotes/origin/cm-10.0
	memcpy(clp->cl_recdir, recdir, HEXDIR_LEN);
	atomic_set(&clp->cl_refcount, 0);
	clp->cl_cb_state = NFSD4_CB_UNKNOWN;
	INIT_LIST_HEAD(&clp->cl_idhash);
	INIT_LIST_HEAD(&clp->cl_strhash);
=======
	ret = copy_cred(&clp->cl_cred, &rqstp->rq_cred);
	if (ret) {
		spin_lock(&nn->client_lock);
		free_client(clp);
		spin_unlock(&nn->client_lock);
		return NULL;
	}
	idr_init(&clp->cl_stateids);
	atomic_set(&clp->cl_refcount, 0);
	clp->cl_cb_state = NFSD4_CB_UNKNOWN;
	INIT_LIST_HEAD(&clp->cl_idhash);
>>>>>>> refs/remotes/origin/master
	INIT_LIST_HEAD(&clp->cl_openowners);
	INIT_LIST_HEAD(&clp->cl_delegations);
	INIT_LIST_HEAD(&clp->cl_lru);
	INIT_LIST_HEAD(&clp->cl_callbacks);
<<<<<<< HEAD
	spin_lock_init(&clp->cl_lock);
	INIT_WORK(&clp->cl_cb_null.cb_work, nfsd4_do_callback_rpc);
=======
	INIT_LIST_HEAD(&clp->cl_revoked);
	spin_lock_init(&clp->cl_lock);
	nfsd4_init_callback(&clp->cl_cb_null);
>>>>>>> refs/remotes/origin/master
	clp->cl_time = get_seconds();
	clear_bit(0, &clp->cl_cb_slot_busy);
	rpc_init_wait_queue(&clp->cl_cb_waitq, "Backchannel slot table");
	copy_verf(clp, verf);
	rpc_copy_addr((struct sockaddr *) &clp->cl_addr, sa);
<<<<<<< HEAD
	clp->cl_flavor = rqstp->rq_flavor;
	copy_cred(&clp->cl_cred, &rqstp->rq_cred);
	gen_confirm(clp);
	clp->cl_cb_session = NULL;
	return clp;
}

<<<<<<< HEAD
static int check_name(struct xdr_netobj name)
{
	if (name.len == 0) 
		return 0;
	if (name.len > NFS4_OPAQUE_LIMIT) {
		dprintk("NFSD: check_name: name too long(%d)!\n", name.len);
		return 0;
	}
	return 1;
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
static void
add_to_unconfirmed(struct nfs4_client *clp, unsigned int strhashval)
{
	unsigned int idhashval;

	list_add(&clp->cl_strhash, &unconf_str_hashtbl[strhashval]);
	idhashval = clientid_hashval(clp->cl_clientid.cl_id);
	list_add(&clp->cl_idhash, &unconf_id_hashtbl[idhashval]);
=======
	gen_confirm(clp);
	clp->cl_cb_session = NULL;
	clp->net = net;
	return clp;
}

static void
add_clp_to_name_tree(struct nfs4_client *new_clp, struct rb_root *root)
{
	struct rb_node **new = &(root->rb_node), *parent = NULL;
	struct nfs4_client *clp;

	while (*new) {
		clp = rb_entry(*new, struct nfs4_client, cl_namenode);
		parent = *new;

		if (compare_blob(&clp->cl_name, &new_clp->cl_name) > 0)
			new = &((*new)->rb_left);
		else
			new = &((*new)->rb_right);
	}

	rb_link_node(&new_clp->cl_namenode, parent, new);
	rb_insert_color(&new_clp->cl_namenode, root);
}

static struct nfs4_client *
find_clp_in_name_tree(struct xdr_netobj *name, struct rb_root *root)
{
	long long cmp;
	struct rb_node *node = root->rb_node;
	struct nfs4_client *clp;

	while (node) {
		clp = rb_entry(node, struct nfs4_client, cl_namenode);
		cmp = compare_blob(&clp->cl_name, name);
		if (cmp > 0)
			node = node->rb_left;
		else if (cmp < 0)
			node = node->rb_right;
		else
			return clp;
	}
	return NULL;
}

static void
add_to_unconfirmed(struct nfs4_client *clp)
{
	unsigned int idhashval;
	struct nfsd_net *nn = net_generic(clp->net, nfsd_net_id);

	clear_bit(NFSD4_CLIENT_CONFIRMED, &clp->cl_flags);
	add_clp_to_name_tree(clp, &nn->unconf_name_tree);
	idhashval = clientid_hashval(clp->cl_clientid.cl_id);
	list_add(&clp->cl_idhash, &nn->unconf_id_hashtbl[idhashval]);
>>>>>>> refs/remotes/origin/master
	renew_client(clp);
}

static void
move_to_confirmed(struct nfs4_client *clp)
{
	unsigned int idhashval = clientid_hashval(clp->cl_clientid.cl_id);
<<<<<<< HEAD
	unsigned int strhashval;

	dprintk("NFSD: move_to_confirm nfs4_client %p\n", clp);
	list_move(&clp->cl_idhash, &conf_id_hashtbl[idhashval]);
	strhashval = clientstr_hashval(clp->cl_recdir);
	list_move(&clp->cl_strhash, &conf_str_hashtbl[strhashval]);
=======
	struct nfsd_net *nn = net_generic(clp->net, nfsd_net_id);

	dprintk("NFSD: move_to_confirm nfs4_client %p\n", clp);
	list_move(&clp->cl_idhash, &nn->conf_id_hashtbl[idhashval]);
	rb_erase(&clp->cl_namenode, &nn->unconf_name_tree);
	add_clp_to_name_tree(clp, &nn->conf_name_tree);
	set_bit(NFSD4_CLIENT_CONFIRMED, &clp->cl_flags);
>>>>>>> refs/remotes/origin/master
	renew_client(clp);
}

static struct nfs4_client *
<<<<<<< HEAD
find_confirmed_client(clientid_t *clid)
=======
find_client_in_id_table(struct list_head *tbl, clientid_t *clid, bool sessions)
>>>>>>> refs/remotes/origin/master
{
	struct nfs4_client *clp;
	unsigned int idhashval = clientid_hashval(clid->cl_id);

<<<<<<< HEAD
	list_for_each_entry(clp, &conf_id_hashtbl[idhashval], cl_idhash) {
<<<<<<< HEAD
		if (same_clid(&clp->cl_clientid, clid))
			return clp;
=======
		if (same_clid(&clp->cl_clientid, clid)) {
			renew_client(clp);
			return clp;
		}
>>>>>>> refs/remotes/origin/cm-10.0
=======
	list_for_each_entry(clp, &tbl[idhashval], cl_idhash) {
		if (same_clid(&clp->cl_clientid, clid)) {
			if ((bool)clp->cl_minorversion != sessions)
				return NULL;
			renew_client(clp);
			return clp;
		}
>>>>>>> refs/remotes/origin/master
	}
	return NULL;
}

static struct nfs4_client *
<<<<<<< HEAD
find_unconfirmed_client(clientid_t *clid)
{
	struct nfs4_client *clp;
	unsigned int idhashval = clientid_hashval(clid->cl_id);

	list_for_each_entry(clp, &unconf_id_hashtbl[idhashval], cl_idhash) {
		if (same_clid(&clp->cl_clientid, clid))
			return clp;
	}
	return NULL;
=======
find_confirmed_client(clientid_t *clid, bool sessions, struct nfsd_net *nn)
{
	struct list_head *tbl = nn->conf_id_hashtbl;

	return find_client_in_id_table(tbl, clid, sessions);
}

static struct nfs4_client *
find_unconfirmed_client(clientid_t *clid, bool sessions, struct nfsd_net *nn)
{
	struct list_head *tbl = nn->unconf_id_hashtbl;

	return find_client_in_id_table(tbl, clid, sessions);
>>>>>>> refs/remotes/origin/master
}

static bool clp_used_exchangeid(struct nfs4_client *clp)
{
	return clp->cl_exchange_flags != 0;
} 

static struct nfs4_client *
<<<<<<< HEAD
find_confirmed_client_by_str(const char *dname, unsigned int hashval)
{
	struct nfs4_client *clp;

	list_for_each_entry(clp, &conf_str_hashtbl[hashval], cl_strhash) {
		if (same_name(clp->cl_recdir, dname))
			return clp;
	}
	return NULL;
}

static struct nfs4_client *
find_unconfirmed_client_by_str(const char *dname, unsigned int hashval)
{
	struct nfs4_client *clp;

	list_for_each_entry(clp, &unconf_str_hashtbl[hashval], cl_strhash) {
		if (same_name(clp->cl_recdir, dname))
			return clp;
	}
	return NULL;
}

<<<<<<< HEAD
static void rpc_svcaddr2sockaddr(struct sockaddr *sa, unsigned short family, union svc_addr_u *svcaddr)
{
	switch (family) {
	case AF_INET:
		((struct sockaddr_in *)sa)->sin_family = AF_INET;
		((struct sockaddr_in *)sa)->sin_addr = svcaddr->addr;
		return;
	case AF_INET6:
		((struct sockaddr_in6 *)sa)->sin6_family = AF_INET6;
		((struct sockaddr_in6 *)sa)->sin6_addr = svcaddr->addr6;
		return;
	}
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
find_confirmed_client_by_name(struct xdr_netobj *name, struct nfsd_net *nn)
{
	return find_clp_in_name_tree(name, &nn->conf_name_tree);
}

static struct nfs4_client *
find_unconfirmed_client_by_name(struct xdr_netobj *name, struct nfsd_net *nn)
{
	return find_clp_in_name_tree(name, &nn->unconf_name_tree);
}

>>>>>>> refs/remotes/origin/master
static void
gen_callback(struct nfs4_client *clp, struct nfsd4_setclientid *se, struct svc_rqst *rqstp)
{
	struct nfs4_cb_conn *conn = &clp->cl_cb_conn;
	struct sockaddr	*sa = svc_addr(rqstp);
	u32 scopeid = rpc_get_scope_id(sa);
	unsigned short expected_family;

	/* Currently, we only support tcp and tcp6 for the callback channel */
	if (se->se_callback_netid_len == 3 &&
	    !memcmp(se->se_callback_netid_val, "tcp", 3))
		expected_family = AF_INET;
	else if (se->se_callback_netid_len == 4 &&
		 !memcmp(se->se_callback_netid_val, "tcp6", 4))
		expected_family = AF_INET6;
	else
		goto out_err;

<<<<<<< HEAD
<<<<<<< HEAD
	conn->cb_addrlen = rpc_uaddr2sockaddr(se->se_callback_addr_val,
=======
	conn->cb_addrlen = rpc_uaddr2sockaddr(&init_net, se->se_callback_addr_val,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	conn->cb_addrlen = rpc_uaddr2sockaddr(clp->net, se->se_callback_addr_val,
>>>>>>> refs/remotes/origin/master
					    se->se_callback_addr_len,
					    (struct sockaddr *)&conn->cb_addr,
					    sizeof(conn->cb_addr));

	if (!conn->cb_addrlen || conn->cb_addr.ss_family != expected_family)
		goto out_err;

	if (conn->cb_addr.ss_family == AF_INET6)
		((struct sockaddr_in6 *)&conn->cb_addr)->sin6_scope_id = scopeid;

	conn->cb_prog = se->se_callback_prog;
	conn->cb_ident = se->se_callback_ident;
<<<<<<< HEAD
<<<<<<< HEAD
	rpc_svcaddr2sockaddr((struct sockaddr *)&conn->cb_saddr, expected_family, &rqstp->rq_daddr);
=======
	memcpy(&conn->cb_saddr, &rqstp->rq_daddr, rqstp->rq_daddrlen);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	memcpy(&conn->cb_saddr, &rqstp->rq_daddr, rqstp->rq_daddrlen);
>>>>>>> refs/remotes/origin/master
	return;
out_err:
	conn->cb_addr.ss_family = AF_UNSPEC;
	conn->cb_addrlen = 0;
	dprintk(KERN_INFO "NFSD: this client (clientid %08x/%08x) "
		"will not receive delegations\n",
		clp->cl_clientid.cl_boot, clp->cl_clientid.cl_id);

	return;
}

/*
 * Cache a reply. nfsd4_check_drc_limit() has bounded the cache size.
 */
void
nfsd4_store_cache_entry(struct nfsd4_compoundres *resp)
{
	struct nfsd4_slot *slot = resp->cstate.slot;
	unsigned int base;

	dprintk("--> %s slot %p\n", __func__, slot);

	slot->sl_opcnt = resp->opcnt;
	slot->sl_status = resp->cstate.status;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	slot->sl_flags |= NFSD4_SLOT_INITIALIZED;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	slot->sl_flags |= NFSD4_SLOT_INITIALIZED;
>>>>>>> refs/remotes/origin/master
	if (nfsd4_not_cached(resp)) {
		slot->sl_datalen = 0;
		return;
	}
	slot->sl_datalen = (char *)resp->p - (char *)resp->cstate.datap;
	base = (char *)resp->cstate.datap -
					(char *)resp->xbuf->head[0].iov_base;
	if (read_bytes_from_xdr_buf(resp->xbuf, base, slot->sl_data,
				    slot->sl_datalen))
		WARN("%s: sessions DRC could not cache compound\n", __func__);
	return;
}

/*
 * Encode the replay sequence operation from the slot values.
 * If cachethis is FALSE encode the uncached rep error on the next
 * operation which sets resp->p and increments resp->opcnt for
 * nfs4svc_encode_compoundres.
 *
 */
static __be32
nfsd4_enc_sequence_replay(struct nfsd4_compoundargs *args,
			  struct nfsd4_compoundres *resp)
{
	struct nfsd4_op *op;
	struct nfsd4_slot *slot = resp->cstate.slot;

<<<<<<< HEAD
<<<<<<< HEAD
	dprintk("--> %s resp->opcnt %d cachethis %u \n", __func__,
		resp->opcnt, resp->cstate.slot->sl_cachethis);

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	/* Encode the replayed sequence operation */
	op = &args->ops[resp->opcnt - 1];
	nfsd4_encode_operation(resp, op);

	/* Return nfserr_retry_uncached_rep in next operation. */
<<<<<<< HEAD
<<<<<<< HEAD
	if (args->opcnt > 1 && slot->sl_cachethis == 0) {
=======
	if (args->opcnt > 1 && !(slot->sl_flags & NFSD4_SLOT_CACHETHIS)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (args->opcnt > 1 && !(slot->sl_flags & NFSD4_SLOT_CACHETHIS)) {
>>>>>>> refs/remotes/origin/master
		op = &args->ops[resp->opcnt++];
		op->status = nfserr_retry_uncached_rep;
		nfsd4_encode_operation(resp, op);
	}
	return op->status;
}

/*
 * The sequence operation is not cached because we can use the slot and
 * session values.
 */
__be32
nfsd4_replay_cache_entry(struct nfsd4_compoundres *resp,
			 struct nfsd4_sequence *seq)
{
	struct nfsd4_slot *slot = resp->cstate.slot;
	__be32 status;

	dprintk("--> %s slot %p\n", __func__, slot);

	/* Either returns 0 or nfserr_retry_uncached */
	status = nfsd4_enc_sequence_replay(resp->rqstp->rq_argp, resp);
	if (status == nfserr_retry_uncached_rep)
		return status;

	/* The sequence operation has been encoded, cstate->datap set. */
	memcpy(resp->cstate.datap, slot->sl_data, slot->sl_datalen);

	resp->opcnt = slot->sl_opcnt;
	resp->p = resp->cstate.datap + XDR_QUADLEN(slot->sl_datalen);
	status = slot->sl_status;

	return status;
}

/*
 * Set the exchange_id flags returned by the server.
 */
static void
nfsd4_set_ex_flags(struct nfs4_client *new, struct nfsd4_exchange_id *clid)
{
	/* pNFS is not supported */
	new->cl_exchange_flags |= EXCHGID4_FLAG_USE_NON_PNFS;

	/* Referrals are supported, Migration is not. */
	new->cl_exchange_flags |= EXCHGID4_FLAG_SUPP_MOVED_REFER;

	/* set the wire flags to return to client. */
	clid->flags = new->cl_exchange_flags;
}

<<<<<<< HEAD
=======
static bool client_has_state(struct nfs4_client *clp)
{
	/*
	 * Note clp->cl_openowners check isn't quite right: there's no
	 * need to count owners without stateid's.
	 *
	 * Also note we should probably be using this in 4.0 case too.
	 */
	return !list_empty(&clp->cl_openowners)
		|| !list_empty(&clp->cl_delegations)
		|| !list_empty(&clp->cl_sessions);
}

>>>>>>> refs/remotes/origin/master
__be32
nfsd4_exchange_id(struct svc_rqst *rqstp,
		  struct nfsd4_compound_state *cstate,
		  struct nfsd4_exchange_id *exid)
{
	struct nfs4_client *unconf, *conf, *new;
<<<<<<< HEAD
	int status;
	unsigned int		strhashval;
	char			dname[HEXDIR_LEN];
	char			addr_str[INET6_ADDRSTRLEN];
	nfs4_verifier		verf = exid->verifier;
	struct sockaddr		*sa = svc_addr(rqstp);
=======
	__be32 status;
	char			addr_str[INET6_ADDRSTRLEN];
	nfs4_verifier		verf = exid->verifier;
	struct sockaddr		*sa = svc_addr(rqstp);
	bool	update = exid->flags & EXCHGID4_FLAG_UPD_CONFIRMED_REC_A;
	struct nfsd_net		*nn = net_generic(SVC_NET(rqstp), nfsd_net_id);
>>>>>>> refs/remotes/origin/master

	rpc_ntop(sa, addr_str, sizeof(addr_str));
	dprintk("%s rqstp=%p exid=%p clname.len=%u clname.data=%p "
		"ip_addr=%s flags %x, spa_how %d\n",
		__func__, rqstp, exid, exid->clname.len, exid->clname.data,
		addr_str, exid->flags, exid->spa_how);

<<<<<<< HEAD
<<<<<<< HEAD
	if (!check_name(exid->clname) || (exid->flags & ~EXCHGID4_FLAG_MASK_A))
=======
	if (exid->flags & ~EXCHGID4_FLAG_MASK_A)
>>>>>>> refs/remotes/origin/cm-10.0
		return nfserr_inval;

	/* Currently only support SP4_NONE */
	switch (exid->spa_how) {
	case SP4_NONE:
		break;
	case SP4_SSV:
		return nfserr_serverfault;
	default:
		BUG();				/* checked by xdr code */
	case SP4_MACH_CRED:
		return nfserr_serverfault;	/* no excuse :-/ */
	}

	status = nfs4_make_rec_clidname(dname, &exid->clname);

	if (status)
		goto error;

	strhashval = clientstr_hashval(dname);

	nfs4_lock_state();
	status = nfs_ok;

	conf = find_confirmed_client_by_str(dname, strhashval);
	if (conf) {
		if (!clp_used_exchangeid(conf)) {
			status = nfserr_clid_inuse; /* XXX: ? */
			goto out;
		}
		if (!same_verf(&verf, &conf->cl_verifier)) {
			/* 18.35.4 case 8 */
			if (exid->flags & EXCHGID4_FLAG_UPD_CONFIRMED_REC_A) {
				status = nfserr_not_same;
				goto out;
			}
			/* Client reboot: destroy old state */
			expire_client(conf);
			goto out_new;
		}
		if (!same_creds(&conf->cl_cred, &rqstp->rq_cred)) {
			/* 18.35.4 case 9 */
			if (exid->flags & EXCHGID4_FLAG_UPD_CONFIRMED_REC_A) {
				status = nfserr_perm;
=======
	if (exid->flags & ~EXCHGID4_FLAG_MASK_A)
		return nfserr_inval;

	switch (exid->spa_how) {
	case SP4_MACH_CRED:
		if (!svc_rqst_integrity_protected(rqstp))
			return nfserr_inval;
	case SP4_NONE:
		break;
	default:				/* checked by xdr code */
		WARN_ON_ONCE(1);
	case SP4_SSV:
		return nfserr_encr_alg_unsupp;
	}

	/* Cases below refer to rfc 5661 section 18.35.4: */
	nfs4_lock_state();
	conf = find_confirmed_client_by_name(&exid->clname, nn);
	if (conf) {
		bool creds_match = same_creds(&conf->cl_cred, &rqstp->rq_cred);
		bool verfs_match = same_verf(&verf, &conf->cl_verifier);

		if (update) {
			if (!clp_used_exchangeid(conf)) { /* buggy client */
				status = nfserr_inval;
				goto out;
			}
			if (!mach_creds_match(conf, rqstp)) {
				status = nfserr_wrong_cred;
				goto out;
			}
			if (!creds_match) { /* case 9 */
				status = nfserr_perm;
				goto out;
			}
			if (!verfs_match) { /* case 8 */
				status = nfserr_not_same;
				goto out;
			}
			/* case 6 */
			exid->flags |= EXCHGID4_FLAG_CONFIRMED_R;
			new = conf;
			goto out_copy;
		}
		if (!creds_match) { /* case 3 */
			if (client_has_state(conf)) {
				status = nfserr_clid_inuse;
>>>>>>> refs/remotes/origin/master
				goto out;
			}
			expire_client(conf);
			goto out_new;
		}
<<<<<<< HEAD
		/*
		 * Set bit when the owner id and verifier map to an already
		 * confirmed client id (18.35.3).
		 */
		exid->flags |= EXCHGID4_FLAG_CONFIRMED_R;

		/*
		 * Falling into 18.35.4 case 2, possible router replay.
		 * Leave confirmed record intact and return same result.
		 */
		copy_verf(conf, &verf);
		new = conf;
		goto out_copy;
	}

	/* 18.35.4 case 7 */
	if (exid->flags & EXCHGID4_FLAG_UPD_CONFIRMED_REC_A) {
=======
		if (verfs_match) { /* case 2 */
			conf->cl_exchange_flags |= EXCHGID4_FLAG_CONFIRMED_R;
			new = conf;
			goto out_copy;
		}
		/* case 5, client reboot */
		goto out_new;
	}

	if (update) { /* case 7 */
>>>>>>> refs/remotes/origin/master
		status = nfserr_noent;
		goto out;
	}

<<<<<<< HEAD
	unconf  = find_unconfirmed_client_by_str(dname, strhashval);
	if (unconf) {
		/*
		 * Possible retry or client restart.  Per 18.35.4 case 4,
		 * a new unconfirmed record should be generated regardless
		 * of whether any properties have changed.
		 */
		expire_client(unconf);
	}

out_new:
	/* Normal case */
	new = create_client(exid->clname, dname, rqstp, &verf);
=======
	unconf  = find_unconfirmed_client_by_name(&exid->clname, nn);
	if (unconf) /* case 4, possible retry or client restart */
		expire_client(unconf);

	/* case 1 (normal case) */
out_new:
	new = create_client(exid->clname, rqstp, &verf);
>>>>>>> refs/remotes/origin/master
	if (new == NULL) {
		status = nfserr_jukebox;
		goto out;
	}
<<<<<<< HEAD

	gen_clid(new);
	add_to_unconfirmed(new, strhashval);
=======
	new->cl_minorversion = cstate->minorversion;
	new->cl_mach_cred = (exid->spa_how == SP4_MACH_CRED);

	gen_clid(new, nn);
	add_to_unconfirmed(new);
>>>>>>> refs/remotes/origin/master
out_copy:
	exid->clientid.cl_boot = new->cl_clientid.cl_boot;
	exid->clientid.cl_id = new->cl_clientid.cl_id;

<<<<<<< HEAD
	exid->seqid = 1;
=======
	exid->seqid = new->cl_cs_slot.sl_seqid + 1;
>>>>>>> refs/remotes/origin/master
	nfsd4_set_ex_flags(new, exid);

	dprintk("nfsd4_exchange_id seqid %d flags %x\n",
		new->cl_cs_slot.sl_seqid, new->cl_exchange_flags);
	status = nfs_ok;

out:
	nfs4_unlock_state();
<<<<<<< HEAD
error:
	dprintk("nfsd4_exchange_id returns %d\n", ntohl(status));
	return status;
}

static int
=======
	return status;
}

static __be32
>>>>>>> refs/remotes/origin/master
check_slot_seqid(u32 seqid, u32 slot_seqid, int slot_inuse)
{
	dprintk("%s enter. seqid %d slot_seqid %d\n", __func__, seqid,
		slot_seqid);

	/* The slot is in use, and no response has been sent. */
	if (slot_inuse) {
		if (seqid == slot_seqid)
			return nfserr_jukebox;
		else
			return nfserr_seq_misordered;
	}
<<<<<<< HEAD
<<<<<<< HEAD
	/* Normal */
	if (likely(seqid == slot_seqid + 1))
		return nfs_ok;
	/* Replay */
	if (seqid == slot_seqid)
		return nfserr_replay_cache;
	/* Wraparound */
	if (seqid == 1 && (slot_seqid + 1) == 0)
		return nfs_ok;
	/* Misordered replay or misordered new request */
=======
=======
>>>>>>> refs/remotes/origin/master
	/* Note unsigned 32-bit arithmetic handles wraparound: */
	if (likely(seqid == slot_seqid + 1))
		return nfs_ok;
	if (seqid == slot_seqid)
		return nfserr_replay_cache;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return nfserr_seq_misordered;
}

/*
 * Cache the create session result into the create session single DRC
 * slot cache by saving the xdr structure. sl_seqid has been set.
 * Do this for solo or embedded create session operations.
 */
static void
nfsd4_cache_create_session(struct nfsd4_create_session *cr_ses,
<<<<<<< HEAD
			   struct nfsd4_clid_slot *slot, int nfserr)
=======
			   struct nfsd4_clid_slot *slot, __be32 nfserr)
>>>>>>> refs/remotes/origin/master
{
	slot->sl_status = nfserr;
	memcpy(&slot->sl_cr_ses, cr_ses, sizeof(*cr_ses));
}

static __be32
nfsd4_replay_create_session(struct nfsd4_create_session *cr_ses,
			    struct nfsd4_clid_slot *slot)
{
	memcpy(cr_ses, &slot->sl_cr_ses, sizeof(*cr_ses));
	return slot->sl_status;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
#define NFSD_MIN_REQ_HDR_SEQ_SZ	((\
			2 * 2 + /* credential,verifier: AUTH_NULL, length 0 */ \
			1 +	/* MIN tag is length with zero, only length */ \
			3 +	/* version, opcount, opcode */ \
			XDR_QUADLEN(NFS4_MAX_SESSIONID_LEN) + \
				/* seqid, slotID, slotID, cache */ \
			4 ) * sizeof(__be32))

#define NFSD_MIN_RESP_HDR_SEQ_SZ ((\
			2 +	/* verifier: AUTH_NULL, length 0 */\
			1 +	/* status */ \
			1 +	/* MIN tag is length with zero, only length */ \
			3 +	/* opcount, opcode, opstatus*/ \
			XDR_QUADLEN(NFS4_MAX_SESSIONID_LEN) + \
				/* seqid, slotID, slotID, slotID, status */ \
			5 ) * sizeof(__be32))

<<<<<<< HEAD
static __be32 check_forechannel_attrs(struct nfsd4_channel_attrs fchannel)
{
	return fchannel.maxreq_sz < NFSD_MIN_REQ_HDR_SEQ_SZ
		|| fchannel.maxresp_sz < NFSD_MIN_RESP_HDR_SEQ_SZ;
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
static __be32 check_forechannel_attrs(struct nfsd4_channel_attrs *ca, struct nfsd_net *nn)
{
	u32 maxrpc = nn->nfsd_serv->sv_max_mesg;

	if (ca->maxreq_sz < NFSD_MIN_REQ_HDR_SEQ_SZ)
		return nfserr_toosmall;
	if (ca->maxresp_sz < NFSD_MIN_RESP_HDR_SEQ_SZ)
		return nfserr_toosmall;
	ca->headerpadsz = 0;
	ca->maxreq_sz = min_t(u32, ca->maxreq_sz, maxrpc);
	ca->maxresp_sz = min_t(u32, ca->maxresp_sz, maxrpc);
	ca->maxops = min_t(u32, ca->maxops, NFSD_MAX_OPS_PER_COMPOUND);
	ca->maxresp_cached = min_t(u32, ca->maxresp_cached,
			NFSD_SLOT_CACHE_SIZE + NFSD_MIN_HDR_SEQ_SZ);
	ca->maxreqs = min_t(u32, ca->maxreqs, NFSD_MAX_SLOTS_PER_SESSION);
	/*
	 * Note decreasing slot size below client's request may make it
	 * difficult for client to function correctly, whereas
	 * decreasing the number of slots will (just?) affect
	 * performance.  When short on memory we therefore prefer to
	 * decrease number of slots instead of their size.  Clients that
	 * request larger slots than they need will get poor results:
	 */
	ca->maxreqs = nfsd4_get_drc_mem(ca);
	if (!ca->maxreqs)
		return nfserr_jukebox;

	return nfs_ok;
}

static __be32 check_backchannel_attrs(struct nfsd4_channel_attrs *ca)
{
	ca->headerpadsz = 0;

	/*
	 * These RPC_MAX_HEADER macros are overkill, especially since we
	 * don't even do gss on the backchannel yet.  But this is still
	 * less than 1k.  Tighten up this estimate in the unlikely event
	 * it turns out to be a problem for some client:
	 */
	if (ca->maxreq_sz < NFS4_enc_cb_recall_sz + RPC_MAX_HEADER_WITH_AUTH)
		return nfserr_toosmall;
	if (ca->maxresp_sz < NFS4_dec_cb_recall_sz + RPC_MAX_REPHEADER_WITH_AUTH)
		return nfserr_toosmall;
	ca->maxresp_cached = 0;
	if (ca->maxops < 2)
		return nfserr_toosmall;

	return nfs_ok;
}

static __be32 nfsd4_check_cb_sec(struct nfsd4_cb_sec *cbs)
{
	switch (cbs->flavor) {
	case RPC_AUTH_NULL:
	case RPC_AUTH_UNIX:
		return nfs_ok;
	default:
		/*
		 * GSS case: the spec doesn't allow us to return this
		 * error.  But it also doesn't allow us not to support
		 * GSS.
		 * I'd rather this fail hard than return some error the
		 * client might think it can already handle:
		 */
		return nfserr_encr_alg_unsupp;
	}
}

>>>>>>> refs/remotes/origin/master
__be32
nfsd4_create_session(struct svc_rqst *rqstp,
		     struct nfsd4_compound_state *cstate,
		     struct nfsd4_create_session *cr_ses)
{
	struct sockaddr *sa = svc_addr(rqstp);
	struct nfs4_client *conf, *unconf;
	struct nfsd4_session *new;
<<<<<<< HEAD
	struct nfsd4_clid_slot *cs_slot = NULL;
	bool confirm_me = false;
	int status = 0;

	if (cr_ses->flags & ~SESSION4_FLAG_MASK_A)
		return nfserr_inval;

	nfs4_lock_state();
	unconf = find_unconfirmed_client(&cr_ses->clientid);
	conf = find_confirmed_client(&cr_ses->clientid);

	if (conf) {
		cs_slot = &conf->cl_cs_slot;
		status = check_slot_seqid(cr_ses->seqid, cs_slot->sl_seqid, 0);
		if (status == nfserr_replay_cache) {
			dprintk("Got a create_session replay! seqid= %d\n",
				cs_slot->sl_seqid);
			/* Return the cached reply status */
			status = nfsd4_replay_create_session(cr_ses, cs_slot);
			goto out;
		} else if (cr_ses->seqid != cs_slot->sl_seqid + 1) {
			status = nfserr_seq_misordered;
			dprintk("Sequence misordered!\n");
			dprintk("Expected seqid= %d but got seqid= %d\n",
				cs_slot->sl_seqid, cr_ses->seqid);
			goto out;
		}
	} else if (unconf) {
		if (!same_creds(&unconf->cl_cred, &rqstp->rq_cred) ||
		    !rpc_cmp_addr(sa, (struct sockaddr *) &unconf->cl_addr)) {
			status = nfserr_clid_inuse;
			goto out;
		}

=======
	struct nfsd4_conn *conn;
	struct nfsd4_clid_slot *cs_slot = NULL;
	__be32 status = 0;
	struct nfsd_net *nn = net_generic(SVC_NET(rqstp), nfsd_net_id);

	if (cr_ses->flags & ~SESSION4_FLAG_MASK_A)
		return nfserr_inval;
	status = nfsd4_check_cb_sec(&cr_ses->cb_sec);
	if (status)
		return status;
	status = check_forechannel_attrs(&cr_ses->fore_channel, nn);
	if (status)
		return status;
	status = check_backchannel_attrs(&cr_ses->back_channel);
	if (status)
		return status;
	status = nfserr_jukebox;
	new = alloc_session(&cr_ses->fore_channel);
	if (!new)
		goto out_release_drc_mem;
	conn = alloc_conn_from_crses(rqstp, cr_ses);
	if (!conn)
		goto out_free_session;

	nfs4_lock_state();
	unconf = find_unconfirmed_client(&cr_ses->clientid, true, nn);
	conf = find_confirmed_client(&cr_ses->clientid, true, nn);
	WARN_ON_ONCE(conf && unconf);

	if (conf) {
		status = nfserr_wrong_cred;
		if (!mach_creds_match(conf, rqstp))
			goto out_free_conn;
		cs_slot = &conf->cl_cs_slot;
		status = check_slot_seqid(cr_ses->seqid, cs_slot->sl_seqid, 0);
		if (status == nfserr_replay_cache) {
			status = nfsd4_replay_create_session(cr_ses, cs_slot);
			goto out_free_conn;
		} else if (cr_ses->seqid != cs_slot->sl_seqid + 1) {
			status = nfserr_seq_misordered;
			goto out_free_conn;
		}
	} else if (unconf) {
		struct nfs4_client *old;
		if (!same_creds(&unconf->cl_cred, &rqstp->rq_cred) ||
		    !rpc_cmp_addr(sa, (struct sockaddr *) &unconf->cl_addr)) {
			status = nfserr_clid_inuse;
			goto out_free_conn;
		}
		status = nfserr_wrong_cred;
		if (!mach_creds_match(unconf, rqstp))
			goto out_free_conn;
>>>>>>> refs/remotes/origin/master
		cs_slot = &unconf->cl_cs_slot;
		status = check_slot_seqid(cr_ses->seqid, cs_slot->sl_seqid, 0);
		if (status) {
			/* an unconfirmed replay returns misordered */
			status = nfserr_seq_misordered;
<<<<<<< HEAD
			goto out;
		}

		confirm_me = true;
		conf = unconf;
	} else {
		status = nfserr_stale_clientid;
		goto out;
	}

	/*
	 * XXX: we should probably set this at creation time, and check
	 * for consistent minorversion use throughout:
	 */
	conf->cl_minorversion = 1;
=======
			goto out_free_conn;
		}
		old = find_confirmed_client_by_name(&unconf->cl_name, nn);
		if (old) {
			status = mark_client_expired(old);
			if (status)
				goto out_free_conn;
			expire_client(old);
		}
		move_to_confirmed(unconf);
		conf = unconf;
	} else {
		status = nfserr_stale_clientid;
		goto out_free_conn;
	}
	status = nfs_ok;
>>>>>>> refs/remotes/origin/master
	/*
	 * We do not support RDMA or persistent sessions
	 */
	cr_ses->flags &= ~SESSION4_PERSIST;
	cr_ses->flags &= ~SESSION4_RDMA;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	status = nfserr_toosmall;
	if (check_forechannel_attrs(cr_ses->fore_channel))
		goto out;

>>>>>>> refs/remotes/origin/cm-10.0
	status = nfserr_jukebox;
	new = alloc_init_session(rqstp, conf, cr_ses);
	if (!new)
		goto out;
	status = nfs_ok;
	memcpy(cr_ses->sessionid.data, new->se_sessionid.data,
	       NFS4_MAX_SESSIONID_LEN);
	memcpy(&cr_ses->fore_channel, &new->se_fchannel,
		sizeof(struct nfsd4_channel_attrs));
=======
	init_session(rqstp, new, conf, cr_ses);
	nfsd4_init_conn(rqstp, conn, new);

	memcpy(cr_ses->sessionid.data, new->se_sessionid.data,
	       NFS4_MAX_SESSIONID_LEN);
>>>>>>> refs/remotes/origin/master
	cs_slot->sl_seqid++;
	cr_ses->seqid = cs_slot->sl_seqid;

	/* cache solo and embedded create sessions under the state lock */
	nfsd4_cache_create_session(cr_ses, cs_slot, status);
<<<<<<< HEAD
	if (confirm_me)
		move_to_confirmed(conf);
out:
	nfs4_unlock_state();
	dprintk("%s returns %d\n", __func__, ntohl(status));
	return status;
}

static bool nfsd4_last_compound_op(struct svc_rqst *rqstp)
{
	struct nfsd4_compoundres *resp = rqstp->rq_resp;
	struct nfsd4_compoundargs *argp = rqstp->rq_argp;

	return argp->opcnt == resp->opcnt;
=======
	nfs4_unlock_state();
	return status;
out_free_conn:
	nfs4_unlock_state();
	free_conn(conn);
out_free_session:
	__free_session(new);
out_release_drc_mem:
	nfsd4_put_drc_mem(&cr_ses->fore_channel);
	return status;
>>>>>>> refs/remotes/origin/master
}

static __be32 nfsd4_map_bcts_dir(u32 *dir)
{
	switch (*dir) {
	case NFS4_CDFC4_FORE:
	case NFS4_CDFC4_BACK:
		return nfs_ok;
	case NFS4_CDFC4_FORE_OR_BOTH:
	case NFS4_CDFC4_BACK_OR_BOTH:
		*dir = NFS4_CDFC4_BOTH;
		return nfs_ok;
	};
	return nfserr_inval;
}

<<<<<<< HEAD
=======
__be32 nfsd4_backchannel_ctl(struct svc_rqst *rqstp, struct nfsd4_compound_state *cstate, struct nfsd4_backchannel_ctl *bc)
{
	struct nfsd4_session *session = cstate->session;
	struct nfsd_net *nn = net_generic(SVC_NET(rqstp), nfsd_net_id);
	__be32 status;

	status = nfsd4_check_cb_sec(&bc->bc_cb_sec);
	if (status)
		return status;
	spin_lock(&nn->client_lock);
	session->se_cb_prog = bc->bc_cb_program;
	session->se_cb_sec = bc->bc_cb_sec;
	spin_unlock(&nn->client_lock);

	nfsd4_probe_callback(session->se_client);

	return nfs_ok;
}

>>>>>>> refs/remotes/origin/master
__be32 nfsd4_bind_conn_to_session(struct svc_rqst *rqstp,
		     struct nfsd4_compound_state *cstate,
		     struct nfsd4_bind_conn_to_session *bcts)
{
	__be32 status;
<<<<<<< HEAD

	if (!nfsd4_last_compound_op(rqstp))
		return nfserr_not_only_op;
	spin_lock(&client_lock);
	cstate->session = find_in_sessionid_hashtbl(&bcts->sessionid);
	/* Sorta weird: we only need the refcnt'ing because new_conn acquires
	 * client_lock iself: */
	if (cstate->session) {
		nfsd4_get_session(cstate->session);
		atomic_inc(&cstate->session->se_client->cl_refcount);
	}
	spin_unlock(&client_lock);
	if (!cstate->session)
		return nfserr_badsession;

	status = nfsd4_map_bcts_dir(&bcts->dir);
	if (!status)
		nfsd4_new_conn(rqstp, cstate->session, bcts->dir);
=======
	struct nfsd4_conn *conn;
	struct nfsd4_session *session;
	struct nfsd_net *nn = net_generic(SVC_NET(rqstp), nfsd_net_id);

	if (!nfsd4_last_compound_op(rqstp))
		return nfserr_not_only_op;
	nfs4_lock_state();
	spin_lock(&nn->client_lock);
	session = find_in_sessionid_hashtbl(&bcts->sessionid, SVC_NET(rqstp));
	spin_unlock(&nn->client_lock);
	status = nfserr_badsession;
	if (!session)
		goto out;
	status = nfserr_wrong_cred;
	if (!mach_creds_match(session->se_client, rqstp))
		goto out;
	status = nfsd4_map_bcts_dir(&bcts->dir);
	if (status)
		goto out;
	conn = alloc_conn(rqstp, bcts->dir);
	status = nfserr_jukebox;
	if (!conn)
		goto out;
	nfsd4_init_conn(rqstp, conn, session);
	status = nfs_ok;
out:
	nfs4_unlock_state();
>>>>>>> refs/remotes/origin/master
	return status;
}

static bool nfsd4_compound_in_session(struct nfsd4_session *session, struct nfs4_sessionid *sid)
{
	if (!session)
		return 0;
	return !memcmp(sid, &session->se_sessionid, sizeof(*sid));
}

__be32
nfsd4_destroy_session(struct svc_rqst *r,
		      struct nfsd4_compound_state *cstate,
		      struct nfsd4_destroy_session *sessionid)
{
	struct nfsd4_session *ses;
<<<<<<< HEAD
	u32 status = nfserr_badsession;

	/* Notes:
	 * - The confirmed nfs4_client->cl_sessionid holds destroyed sessinid
	 * - Should we return nfserr_back_chan_busy if waiting for
	 *   callbacks on to-be-destroyed session?
	 * - Do we need to clear any callback info from previous session?
	 */

	if (nfsd4_compound_in_session(cstate->session, &sessionid->sessionid)) {
		if (!nfsd4_last_compound_op(r))
			return nfserr_not_only_op;
	}
	dump_sessionid(__func__, &sessionid->sessionid);
	spin_lock(&client_lock);
	ses = find_in_sessionid_hashtbl(&sessionid->sessionid);
	if (!ses) {
		spin_unlock(&client_lock);
		goto out;
	}

	unhash_session(ses);
	spin_unlock(&client_lock);

	nfs4_lock_state();
	nfsd4_probe_callback_sync(ses->se_client);
	nfs4_unlock_state();

<<<<<<< HEAD
	nfsd4_del_conns(ses);

	nfsd4_put_session(ses);
=======
	spin_lock(&client_lock);
	nfsd4_del_conns(ses);
	nfsd4_put_session_locked(ses);
	spin_unlock(&client_lock);
>>>>>>> refs/remotes/origin/cm-10.0
	status = nfs_ok;
out:
	dprintk("%s returns %d\n", __func__, ntohl(status));
=======
	__be32 status;
	int ref_held_by_me = 0;
	struct nfsd_net *nn = net_generic(SVC_NET(r), nfsd_net_id);

	nfs4_lock_state();
	status = nfserr_not_only_op;
	if (nfsd4_compound_in_session(cstate->session, &sessionid->sessionid)) {
		if (!nfsd4_last_compound_op(r))
			goto out;
		ref_held_by_me++;
	}
	dump_sessionid(__func__, &sessionid->sessionid);
	spin_lock(&nn->client_lock);
	ses = find_in_sessionid_hashtbl(&sessionid->sessionid, SVC_NET(r));
	status = nfserr_badsession;
	if (!ses)
		goto out_client_lock;
	status = nfserr_wrong_cred;
	if (!mach_creds_match(ses->se_client, r))
		goto out_client_lock;
	nfsd4_get_session_locked(ses);
	status = mark_session_dead_locked(ses, 1 + ref_held_by_me);
	if (status)
		goto out_put_session;
	unhash_session(ses);
	spin_unlock(&nn->client_lock);

	nfsd4_probe_callback_sync(ses->se_client);

	spin_lock(&nn->client_lock);
	status = nfs_ok;
out_put_session:
	nfsd4_put_session(ses);
out_client_lock:
	spin_unlock(&nn->client_lock);
out:
	nfs4_unlock_state();
>>>>>>> refs/remotes/origin/master
	return status;
}

static struct nfsd4_conn *__nfsd4_find_conn(struct svc_xprt *xpt, struct nfsd4_session *s)
{
	struct nfsd4_conn *c;

	list_for_each_entry(c, &s->se_conns, cn_persession) {
		if (c->cn_xprt == xpt) {
			return c;
		}
	}
	return NULL;
}

<<<<<<< HEAD
static void nfsd4_sequence_check_conn(struct nfsd4_conn *new, struct nfsd4_session *ses)
{
	struct nfs4_client *clp = ses->se_client;
	struct nfsd4_conn *c;
=======
static __be32 nfsd4_sequence_check_conn(struct nfsd4_conn *new, struct nfsd4_session *ses)
{
	struct nfs4_client *clp = ses->se_client;
	struct nfsd4_conn *c;
	__be32 status = nfs_ok;
>>>>>>> refs/remotes/origin/master
	int ret;

	spin_lock(&clp->cl_lock);
	c = __nfsd4_find_conn(new->cn_xprt, ses);
<<<<<<< HEAD
	if (c) {
		spin_unlock(&clp->cl_lock);
		free_conn(new);
		return;
	}
=======
	if (c)
		goto out_free;
	status = nfserr_conn_not_bound_to_session;
	if (clp->cl_mach_cred)
		goto out_free;
>>>>>>> refs/remotes/origin/master
	__nfsd4_hash_conn(new, ses);
	spin_unlock(&clp->cl_lock);
	ret = nfsd4_register_conn(new);
	if (ret)
		/* oops; xprt is already down: */
		nfsd4_conn_lost(&new->cn_xpt_user);
<<<<<<< HEAD
	return;
=======
	return nfs_ok;
out_free:
	spin_unlock(&clp->cl_lock);
	free_conn(new);
	return status;
>>>>>>> refs/remotes/origin/master
}

static bool nfsd4_session_too_many_ops(struct svc_rqst *rqstp, struct nfsd4_session *session)
{
	struct nfsd4_compoundargs *args = rqstp->rq_argp;

	return args->opcnt > session->se_fchannel.maxops;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static bool nfsd4_request_too_big(struct svc_rqst *rqstp,
				  struct nfsd4_session *session)
{
	struct xdr_buf *xb = &rqstp->rq_arg;

	return xb->len > session->se_fchannel.maxreq_sz;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
__be32
nfsd4_sequence(struct svc_rqst *rqstp,
	       struct nfsd4_compound_state *cstate,
	       struct nfsd4_sequence *seq)
{
	struct nfsd4_compoundres *resp = rqstp->rq_resp;
	struct nfsd4_session *session;
<<<<<<< HEAD
	struct nfsd4_slot *slot;
	struct nfsd4_conn *conn;
	int status;
=======
	struct nfs4_client *clp;
	struct nfsd4_slot *slot;
	struct nfsd4_conn *conn;
	__be32 status;
	struct nfsd_net *nn = net_generic(SVC_NET(rqstp), nfsd_net_id);
>>>>>>> refs/remotes/origin/master

	if (resp->opcnt != 1)
		return nfserr_sequence_pos;

	/*
	 * Will be either used or freed by nfsd4_sequence_check_conn
	 * below.
	 */
	conn = alloc_conn(rqstp, NFS4_CDFC4_FORE);
	if (!conn)
		return nfserr_jukebox;

<<<<<<< HEAD
	spin_lock(&client_lock);
	status = nfserr_badsession;
	session = find_in_sessionid_hashtbl(&seq->sessionid);
	if (!session)
		goto out;

	status = nfserr_too_many_ops;
	if (nfsd4_session_too_many_ops(rqstp, session))
		goto out;

<<<<<<< HEAD
=======
	status = nfserr_req_too_big;
	if (nfsd4_request_too_big(rqstp, session))
		goto out;

>>>>>>> refs/remotes/origin/cm-10.0
	status = nfserr_badslot;
	if (seq->slotid >= session->se_fchannel.maxreqs)
		goto out;
=======
	spin_lock(&nn->client_lock);
	status = nfserr_badsession;
	session = find_in_sessionid_hashtbl(&seq->sessionid, SVC_NET(rqstp));
	if (!session)
		goto out_no_session;
	clp = session->se_client;
	status = get_client_locked(clp);
	if (status)
		goto out_no_session;
	status = nfsd4_get_session_locked(session);
	if (status)
		goto out_put_client;

	status = nfserr_too_many_ops;
	if (nfsd4_session_too_many_ops(rqstp, session))
		goto out_put_session;

	status = nfserr_req_too_big;
	if (nfsd4_request_too_big(rqstp, session))
		goto out_put_session;

	status = nfserr_badslot;
	if (seq->slotid >= session->se_fchannel.maxreqs)
		goto out_put_session;
>>>>>>> refs/remotes/origin/master

	slot = session->se_slots[seq->slotid];
	dprintk("%s: slotid %d\n", __func__, seq->slotid);

	/* We do not negotiate the number of slots yet, so set the
	 * maxslots to the session maxreqs which is used to encode
	 * sr_highest_slotid and the sr_target_slot id to maxslots */
	seq->maxslots = session->se_fchannel.maxreqs;

<<<<<<< HEAD
<<<<<<< HEAD
	status = check_slot_seqid(seq->seqid, slot->sl_seqid, slot->sl_inuse);
	if (status == nfserr_replay_cache) {
=======
=======
>>>>>>> refs/remotes/origin/master
	status = check_slot_seqid(seq->seqid, slot->sl_seqid,
					slot->sl_flags & NFSD4_SLOT_INUSE);
	if (status == nfserr_replay_cache) {
		status = nfserr_seq_misordered;
		if (!(slot->sl_flags & NFSD4_SLOT_INITIALIZED))
<<<<<<< HEAD
			goto out;
>>>>>>> refs/remotes/origin/cm-10.0
=======
			goto out_put_session;
>>>>>>> refs/remotes/origin/master
		cstate->slot = slot;
		cstate->session = session;
		/* Return the cached reply status and set cstate->status
		 * for nfsd4_proc_compound processing */
		status = nfsd4_replay_cache_entry(resp, seq);
		cstate->status = nfserr_replay_cache;
		goto out;
	}
	if (status)
<<<<<<< HEAD
		goto out;

	nfsd4_sequence_check_conn(conn, session);
	conn = NULL;

	/* Success! bump slot seqid */
<<<<<<< HEAD
	slot->sl_inuse = true;
	slot->sl_seqid = seq->seqid;
	slot->sl_cachethis = seq->cachethis;
=======
=======
		goto out_put_session;

	status = nfsd4_sequence_check_conn(conn, session);
	conn = NULL;
	if (status)
		goto out_put_session;

	/* Success! bump slot seqid */
>>>>>>> refs/remotes/origin/master
	slot->sl_seqid = seq->seqid;
	slot->sl_flags |= NFSD4_SLOT_INUSE;
	if (seq->cachethis)
		slot->sl_flags |= NFSD4_SLOT_CACHETHIS;
	else
		slot->sl_flags &= ~NFSD4_SLOT_CACHETHIS;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	cstate->slot = slot;
	cstate->session = session;

out:
<<<<<<< HEAD
	/* Hold a session reference until done processing the compound. */
	if (cstate->session) {
		struct nfs4_client *clp = session->se_client;

		nfsd4_get_session(cstate->session);
		atomic_inc(&clp->cl_refcount);
<<<<<<< HEAD
		if (clp->cl_cb_state == NFSD4_CB_DOWN)
			seq->status_flags |= SEQ4_STATUS_CB_PATH_DOWN;
=======
		switch (clp->cl_cb_state) {
		case NFSD4_CB_DOWN:
			seq->status_flags = SEQ4_STATUS_CB_PATH_DOWN;
			break;
		case NFSD4_CB_FAULT:
			seq->status_flags = SEQ4_STATUS_BACKCHANNEL_FAULT;
			break;
		default:
			seq->status_flags = 0;
		}
>>>>>>> refs/remotes/origin/cm-10.0
	}
	kfree(conn);
	spin_unlock(&client_lock);
	dprintk("%s: return %d\n", __func__, ntohl(status));
	return status;
}

<<<<<<< HEAD
=======
static inline bool has_resources(struct nfs4_client *clp)
{
	return !list_empty(&clp->cl_openowners)
		|| !list_empty(&clp->cl_delegations)
		|| !list_empty(&clp->cl_sessions);
=======
	switch (clp->cl_cb_state) {
	case NFSD4_CB_DOWN:
		seq->status_flags = SEQ4_STATUS_CB_PATH_DOWN;
		break;
	case NFSD4_CB_FAULT:
		seq->status_flags = SEQ4_STATUS_BACKCHANNEL_FAULT;
		break;
	default:
		seq->status_flags = 0;
	}
	if (!list_empty(&clp->cl_revoked))
		seq->status_flags |= SEQ4_STATUS_RECALLABLE_STATE_REVOKED;
out_no_session:
	kfree(conn);
	spin_unlock(&nn->client_lock);
	return status;
out_put_session:
	nfsd4_put_session(session);
out_put_client:
	put_client_renew_locked(clp);
	goto out_no_session;
>>>>>>> refs/remotes/origin/master
}

__be32
nfsd4_destroy_clientid(struct svc_rqst *rqstp, struct nfsd4_compound_state *cstate, struct nfsd4_destroy_clientid *dc)
{
	struct nfs4_client *conf, *unconf, *clp;
<<<<<<< HEAD
	int status = 0;

	nfs4_lock_state();
	unconf = find_unconfirmed_client(&dc->clientid);
	conf = find_confirmed_client(&dc->clientid);
=======
	__be32 status = 0;
	struct nfsd_net *nn = net_generic(SVC_NET(rqstp), nfsd_net_id);

	nfs4_lock_state();
	unconf = find_unconfirmed_client(&dc->clientid, true, nn);
	conf = find_confirmed_client(&dc->clientid, true, nn);
	WARN_ON_ONCE(conf && unconf);
>>>>>>> refs/remotes/origin/master

	if (conf) {
		clp = conf;

<<<<<<< HEAD
		if (!is_client_expired(conf) && has_resources(conf)) {
			status = nfserr_clientid_busy;
			goto out;
		}

		/* rfc5661 18.50.3 */
		if (cstate->session && conf == cstate->session->se_client) {
=======
		if (client_has_state(conf)) {
>>>>>>> refs/remotes/origin/master
			status = nfserr_clientid_busy;
			goto out;
		}
	} else if (unconf)
		clp = unconf;
	else {
		status = nfserr_stale_clientid;
		goto out;
	}
<<<<<<< HEAD

	expire_client(clp);
out:
	nfs4_unlock_state();
	dprintk("%s return %d\n", __func__, ntohl(status));
	return status;
}

>>>>>>> refs/remotes/origin/cm-10.0
__be32
nfsd4_reclaim_complete(struct svc_rqst *rqstp, struct nfsd4_compound_state *cstate, struct nfsd4_reclaim_complete *rc)
{
	int status = 0;
=======
	if (!mach_creds_match(clp, rqstp)) {
		status = nfserr_wrong_cred;
		goto out;
	}
	expire_client(clp);
out:
	nfs4_unlock_state();
	return status;
}

__be32
nfsd4_reclaim_complete(struct svc_rqst *rqstp, struct nfsd4_compound_state *cstate, struct nfsd4_reclaim_complete *rc)
{
	__be32 status = 0;
>>>>>>> refs/remotes/origin/master

	if (rc->rca_one_fs) {
		if (!cstate->current_fh.fh_dentry)
			return nfserr_nofilehandle;
		/*
		 * We don't take advantage of the rca_one_fs case.
		 * That's OK, it's optional, we can safely ignore it.
		 */
		 return nfs_ok;
	}

	nfs4_lock_state();
	status = nfserr_complete_already;
<<<<<<< HEAD
<<<<<<< HEAD
	if (cstate->session->se_client->cl_firststate)
=======
	if (test_and_set_bit(NFSD4_CLIENT_RECLAIM_COMPLETE,
			     &cstate->session->se_client->cl_flags))
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (test_and_set_bit(NFSD4_CLIENT_RECLAIM_COMPLETE,
			     &cstate->session->se_client->cl_flags))
>>>>>>> refs/remotes/origin/master
		goto out;

	status = nfserr_stale_clientid;
	if (is_client_expired(cstate->session->se_client))
		/*
		 * The following error isn't really legal.
		 * But we only get here if the client just explicitly
		 * destroyed the client.  Surely it no longer cares what
		 * error it gets back on an operation for the dead
		 * client.
		 */
		goto out;

	status = nfs_ok;
<<<<<<< HEAD
<<<<<<< HEAD
	nfsd4_create_clid_dir(cstate->session->se_client);
=======
	nfsd4_client_record_create(cstate->session->se_client);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	nfsd4_client_record_create(cstate->session->se_client);
>>>>>>> refs/remotes/origin/master
out:
	nfs4_unlock_state();
	return status;
}

__be32
nfsd4_setclientid(struct svc_rqst *rqstp, struct nfsd4_compound_state *cstate,
		  struct nfsd4_setclientid *setclid)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct xdr_netobj 	clname = { 
		.len = setclid->se_namelen,
		.data = setclid->se_name,
	};
=======
	struct xdr_netobj 	clname = setclid->se_name;
>>>>>>> refs/remotes/origin/cm-10.0
	nfs4_verifier		clverifier = setclid->se_verf;
	unsigned int 		strhashval;
	struct nfs4_client	*conf, *unconf, *new;
	__be32 			status;
	char                    dname[HEXDIR_LEN];
	
<<<<<<< HEAD
	if (!check_name(clname))
		return nfserr_inval;

=======
>>>>>>> refs/remotes/origin/cm-10.0
	status = nfs4_make_rec_clidname(dname, &clname);
	if (status)
		return status;

	/* 
	 * XXX The Duplicate Request Cache (DRC) has been checked (??)
	 * We get here on a DRC miss.
	 */

	strhashval = clientstr_hashval(dname);

	nfs4_lock_state();
	conf = find_confirmed_client_by_str(dname, strhashval);
	if (conf) {
		/* RFC 3530 14.2.33 CASE 0: */
=======
	struct xdr_netobj 	clname = setclid->se_name;
	nfs4_verifier		clverifier = setclid->se_verf;
	struct nfs4_client	*conf, *unconf, *new;
	__be32 			status;
	struct nfsd_net		*nn = net_generic(SVC_NET(rqstp), nfsd_net_id);

	/* Cases below refer to rfc 3530 section 14.2.33: */
	nfs4_lock_state();
	conf = find_confirmed_client_by_name(&clname, nn);
	if (conf) {
		/* case 0: */
>>>>>>> refs/remotes/origin/master
		status = nfserr_clid_inuse;
		if (clp_used_exchangeid(conf))
			goto out;
		if (!same_creds(&conf->cl_cred, &rqstp->rq_cred)) {
			char addr_str[INET6_ADDRSTRLEN];
			rpc_ntop((struct sockaddr *) &conf->cl_addr, addr_str,
				 sizeof(addr_str));
			dprintk("NFSD: setclientid: string in use by client "
				"at %s\n", addr_str);
			goto out;
		}
	}
<<<<<<< HEAD
	/*
	 * section 14.2.33 of RFC 3530 (under the heading "IMPLEMENTATION")
	 * has a description of SETCLIENTID request processing consisting
	 * of 5 bullet points, labeled as CASE0 - CASE4 below.
	 */
	unconf = find_unconfirmed_client_by_str(dname, strhashval);
	status = nfserr_jukebox;
	if (!conf) {
		/*
		 * RFC 3530 14.2.33 CASE 4:
		 * placed first, because it is the normal case
		 */
		if (unconf)
			expire_client(unconf);
		new = create_client(clname, dname, rqstp, &clverifier);
		if (new == NULL)
			goto out;
		gen_clid(new);
	} else if (same_verf(&conf->cl_verifier, &clverifier)) {
		/*
		 * RFC 3530 14.2.33 CASE 1:
		 * probable callback update
		 */
		if (unconf) {
			/* Note this is removing unconfirmed {*x***},
			 * which is stronger than RFC recommended {vxc**}.
			 * This has the advantage that there is at most
			 * one {*x***} in either list at any time.
			 */
			expire_client(unconf);
		}
		new = create_client(clname, dname, rqstp, &clverifier);
		if (new == NULL)
			goto out;
		copy_clid(new, conf);
	} else if (!unconf) {
		/*
		 * RFC 3530 14.2.33 CASE 2:
		 * probable client reboot; state will be removed if
		 * confirmed.
		 */
		new = create_client(clname, dname, rqstp, &clverifier);
		if (new == NULL)
			goto out;
		gen_clid(new);
	} else {
		/*
		 * RFC 3530 14.2.33 CASE 3:
		 * probable client reboot; state will be removed if
		 * confirmed.
		 */
		expire_client(unconf);
		new = create_client(clname, dname, rqstp, &clverifier);
		if (new == NULL)
			goto out;
		gen_clid(new);
	}
	/*
	 * XXX: we should probably set this at creation time, and check
	 * for consistent minorversion use throughout:
	 */
	new->cl_minorversion = 0;
	gen_callback(new, setclid, rqstp);
	add_to_unconfirmed(new, strhashval);
=======
	unconf = find_unconfirmed_client_by_name(&clname, nn);
	if (unconf)
		expire_client(unconf);
	status = nfserr_jukebox;
	new = create_client(clname, rqstp, &clverifier);
	if (new == NULL)
		goto out;
	if (conf && same_verf(&conf->cl_verifier, &clverifier))
		/* case 1: probable callback update */
		copy_clid(new, conf);
	else /* case 4 (new client) or cases 2, 3 (client reboot): */
		gen_clid(new, nn);
	new->cl_minorversion = 0;
	gen_callback(new, setclid, rqstp);
	add_to_unconfirmed(new);
>>>>>>> refs/remotes/origin/master
	setclid->se_clientid.cl_boot = new->cl_clientid.cl_boot;
	setclid->se_clientid.cl_id = new->cl_clientid.cl_id;
	memcpy(setclid->se_confirm.data, new->cl_confirm.data, sizeof(setclid->se_confirm.data));
	status = nfs_ok;
out:
	nfs4_unlock_state();
	return status;
}


<<<<<<< HEAD
/*
 * Section 14.2.34 of RFC 3530 (under the heading "IMPLEMENTATION") has
 * a description of SETCLIENTID_CONFIRM request processing consisting of 4
 * bullets, labeled as CASE1 - CASE4 below.
 */
=======
>>>>>>> refs/remotes/origin/master
__be32
nfsd4_setclientid_confirm(struct svc_rqst *rqstp,
			 struct nfsd4_compound_state *cstate,
			 struct nfsd4_setclientid_confirm *setclientid_confirm)
{
<<<<<<< HEAD
	struct sockaddr *sa = svc_addr(rqstp);
=======
>>>>>>> refs/remotes/origin/master
	struct nfs4_client *conf, *unconf;
	nfs4_verifier confirm = setclientid_confirm->sc_confirm; 
	clientid_t * clid = &setclientid_confirm->sc_clientid;
	__be32 status;
<<<<<<< HEAD

	if (STALE_CLIENTID(clid))
		return nfserr_stale_clientid;
	/* 
	 * XXX The Duplicate Request Cache (DRC) has been checked (??)
	 * We get here on a DRC miss.
	 */

	nfs4_lock_state();

	conf = find_confirmed_client(clid);
	unconf = find_unconfirmed_client(clid);

	status = nfserr_clid_inuse;
	if (conf && !rpc_cmp_addr((struct sockaddr *) &conf->cl_addr, sa))
		goto out;
	if (unconf && !rpc_cmp_addr((struct sockaddr *) &unconf->cl_addr, sa))
		goto out;

	/*
	 * section 14.2.34 of RFC 3530 has a description of
	 * SETCLIENTID_CONFIRM request processing consisting
	 * of 4 bullet points, labeled as CASE1 - CASE4 below.
	 */
	if (conf && unconf && same_verf(&confirm, &unconf->cl_confirm)) {
		/*
		 * RFC 3530 14.2.34 CASE 1:
		 * callback update
		 */
		if (!same_creds(&conf->cl_cred, &unconf->cl_cred))
			status = nfserr_clid_inuse;
		else {
			nfsd4_change_callback(conf, &unconf->cl_cb_conn);
			nfsd4_probe_callback(conf);
			expire_client(unconf);
			status = nfs_ok;

		}
	} else if (conf && !unconf) {
		/*
		 * RFC 3530 14.2.34 CASE 2:
		 * probable retransmitted request; play it safe and
		 * do nothing.
		 */
		if (!same_creds(&conf->cl_cred, &rqstp->rq_cred))
			status = nfserr_clid_inuse;
		else
			status = nfs_ok;
	} else if (!conf && unconf
			&& same_verf(&unconf->cl_confirm, &confirm)) {
		/*
		 * RFC 3530 14.2.34 CASE 3:
		 * Normal case; new or rebooted client:
		 */
		if (!same_creds(&unconf->cl_cred, &rqstp->rq_cred)) {
			status = nfserr_clid_inuse;
		} else {
			unsigned int hash =
				clientstr_hashval(unconf->cl_recdir);
			conf = find_confirmed_client_by_str(unconf->cl_recdir,
							    hash);
			if (conf) {
<<<<<<< HEAD
				nfsd4_remove_clid_dir(conf);
=======
				nfsd4_client_record_remove(conf);
>>>>>>> refs/remotes/origin/cm-10.0
				expire_client(conf);
			}
			move_to_confirmed(unconf);
			conf = unconf;
			nfsd4_probe_callback(conf);
			status = nfs_ok;
		}
	} else if ((!conf || (conf && !same_verf(&conf->cl_confirm, &confirm)))
	    && (!unconf || (unconf && !same_verf(&unconf->cl_confirm,
				    				&confirm)))) {
		/*
		 * RFC 3530 14.2.34 CASE 4:
		 * Client probably hasn't noticed that we rebooted yet.
		 */
		status = nfserr_stale_clientid;
	} else {
		/* check that we have hit one of the cases...*/
		status = nfserr_clid_inuse;
=======
	struct nfsd_net	*nn = net_generic(SVC_NET(rqstp), nfsd_net_id);

	if (STALE_CLIENTID(clid, nn))
		return nfserr_stale_clientid;
	nfs4_lock_state();

	conf = find_confirmed_client(clid, false, nn);
	unconf = find_unconfirmed_client(clid, false, nn);
	/*
	 * We try hard to give out unique clientid's, so if we get an
	 * attempt to confirm the same clientid with a different cred,
	 * there's a bug somewhere.  Let's charitably assume it's our
	 * bug.
	 */
	status = nfserr_serverfault;
	if (unconf && !same_creds(&unconf->cl_cred, &rqstp->rq_cred))
		goto out;
	if (conf && !same_creds(&conf->cl_cred, &rqstp->rq_cred))
		goto out;
	/* cases below refer to rfc 3530 section 14.2.34: */
	if (!unconf || !same_verf(&confirm, &unconf->cl_confirm)) {
		if (conf && !unconf) /* case 2: probable retransmit */
			status = nfs_ok;
		else /* case 4: client hasn't noticed we rebooted yet? */
			status = nfserr_stale_clientid;
		goto out;
	}
	status = nfs_ok;
	if (conf) { /* case 1: callback update */
		nfsd4_change_callback(conf, &unconf->cl_cb_conn);
		nfsd4_probe_callback(conf);
		expire_client(unconf);
	} else { /* case 3: normal case; new or rebooted client */
		conf = find_confirmed_client_by_name(&unconf->cl_name, nn);
		if (conf) {
			status = mark_client_expired(conf);
			if (status)
				goto out;
			expire_client(conf);
		}
		move_to_confirmed(unconf);
		nfsd4_probe_callback(unconf);
>>>>>>> refs/remotes/origin/master
	}
out:
	nfs4_unlock_state();
	return status;
}

<<<<<<< HEAD
<<<<<<< HEAD
/* OPEN Share state helper functions */
static inline struct nfs4_file *
alloc_init_file(struct inode *ino)
{
	struct nfs4_file *fp;
	unsigned int hashval = file_hashval(ino);

	fp = kmem_cache_alloc(file_slab, GFP_KERNEL);
	if (fp) {
		atomic_set(&fp->fi_ref, 1);
		INIT_LIST_HEAD(&fp->fi_hash);
		INIT_LIST_HEAD(&fp->fi_stateids);
		INIT_LIST_HEAD(&fp->fi_delegations);
		fp->fi_inode = igrab(ino);
		fp->fi_id = current_fileid++;
		fp->fi_had_conflict = false;
		fp->fi_lease = NULL;
		memset(fp->fi_fds, 0, sizeof(fp->fi_fds));
		memset(fp->fi_access, 0, sizeof(fp->fi_access));
		spin_lock(&recall_lock);
		list_add(&fp->fi_hash, &file_hashtbl[hashval]);
		spin_unlock(&recall_lock);
		return fp;
	}
	return NULL;
=======
=======
>>>>>>> refs/remotes/origin/master
static struct nfs4_file *nfsd4_alloc_file(void)
{
	return kmem_cache_alloc(file_slab, GFP_KERNEL);
}

/* OPEN Share state helper functions */
static void nfsd4_init_file(struct nfs4_file *fp, struct inode *ino)
{
	unsigned int hashval = file_hashval(ino);

	atomic_set(&fp->fi_ref, 1);
<<<<<<< HEAD
	INIT_LIST_HEAD(&fp->fi_hash);
=======
>>>>>>> refs/remotes/origin/master
	INIT_LIST_HEAD(&fp->fi_stateids);
	INIT_LIST_HEAD(&fp->fi_delegations);
	fp->fi_inode = igrab(ino);
	fp->fi_had_conflict = false;
	fp->fi_lease = NULL;
	memset(fp->fi_fds, 0, sizeof(fp->fi_fds));
	memset(fp->fi_access, 0, sizeof(fp->fi_access));
	spin_lock(&recall_lock);
<<<<<<< HEAD
	list_add(&fp->fi_hash, &file_hashtbl[hashval]);
	spin_unlock(&recall_lock);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	hlist_add_head(&fp->fi_hash, &file_hashtbl[hashval]);
	spin_unlock(&recall_lock);
>>>>>>> refs/remotes/origin/master
}

static void
nfsd4_free_slab(struct kmem_cache **slab)
{
	if (*slab == NULL)
		return;
	kmem_cache_destroy(*slab);
	*slab = NULL;
}

void
nfsd4_free_slabs(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	nfsd4_free_slab(&stateowner_slab);
=======
	nfsd4_free_slab(&openowner_slab);
	nfsd4_free_slab(&lockowner_slab);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	nfsd4_free_slab(&openowner_slab);
	nfsd4_free_slab(&lockowner_slab);
>>>>>>> refs/remotes/origin/master
	nfsd4_free_slab(&file_slab);
	nfsd4_free_slab(&stateid_slab);
	nfsd4_free_slab(&deleg_slab);
}

<<<<<<< HEAD
<<<<<<< HEAD
static int
nfsd4_init_slabs(void)
{
	stateowner_slab = kmem_cache_create("nfsd4_stateowners",
			sizeof(struct nfs4_stateowner), 0, 0, NULL);
	if (stateowner_slab == NULL)
=======
=======
>>>>>>> refs/remotes/origin/master
int
nfsd4_init_slabs(void)
{
	openowner_slab = kmem_cache_create("nfsd4_openowners",
			sizeof(struct nfs4_openowner), 0, 0, NULL);
	if (openowner_slab == NULL)
		goto out_nomem;
	lockowner_slab = kmem_cache_create("nfsd4_lockowners",
			sizeof(struct nfs4_lockowner), 0, 0, NULL);
	if (lockowner_slab == NULL)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		goto out_nomem;
	file_slab = kmem_cache_create("nfsd4_files",
			sizeof(struct nfs4_file), 0, 0, NULL);
	if (file_slab == NULL)
		goto out_nomem;
	stateid_slab = kmem_cache_create("nfsd4_stateids",
<<<<<<< HEAD
<<<<<<< HEAD
			sizeof(struct nfs4_stateid), 0, 0, NULL);
=======
			sizeof(struct nfs4_ol_stateid), 0, 0, NULL);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			sizeof(struct nfs4_ol_stateid), 0, 0, NULL);
>>>>>>> refs/remotes/origin/master
	if (stateid_slab == NULL)
		goto out_nomem;
	deleg_slab = kmem_cache_create("nfsd4_delegations",
			sizeof(struct nfs4_delegation), 0, 0, NULL);
	if (deleg_slab == NULL)
		goto out_nomem;
	return 0;
out_nomem:
	nfsd4_free_slabs();
	dprintk("nfsd4: out of memory while initializing nfsv4\n");
	return -ENOMEM;
}

<<<<<<< HEAD
<<<<<<< HEAD
void
nfs4_free_stateowner(struct kref *kref)
{
	struct nfs4_stateowner *sop =
		container_of(kref, struct nfs4_stateowner, so_ref);
	kfree(sop->so_owner.data);
	kmem_cache_free(stateowner_slab, sop);
}

static inline struct nfs4_stateowner *
alloc_stateowner(struct xdr_netobj *owner)
{
	struct nfs4_stateowner *sop;

	if ((sop = kmem_cache_alloc(stateowner_slab, GFP_KERNEL))) {
		if ((sop->so_owner.data = kmalloc(owner->len, GFP_KERNEL))) {
			memcpy(sop->so_owner.data, owner->data, owner->len);
			sop->so_owner.len = owner->len;
			kref_init(&sop->so_ref);
			return sop;
		} 
		kmem_cache_free(stateowner_slab, sop);
	}
	return NULL;
}

static struct nfs4_stateowner *
alloc_init_open_stateowner(unsigned int strhashval, struct nfs4_client *clp, struct nfsd4_open *open) {
	struct nfs4_stateowner *sop;
	struct nfs4_replay *rp;
	unsigned int idhashval;

	if (!(sop = alloc_stateowner(&open->op_owner)))
		return NULL;
	idhashval = ownerid_hashval(current_ownerid);
	INIT_LIST_HEAD(&sop->so_idhash);
	INIT_LIST_HEAD(&sop->so_strhash);
	INIT_LIST_HEAD(&sop->so_perclient);
	INIT_LIST_HEAD(&sop->so_stateids);
	INIT_LIST_HEAD(&sop->so_perstateid);  /* not used */
	INIT_LIST_HEAD(&sop->so_close_lru);
	sop->so_time = 0;
	list_add(&sop->so_idhash, &ownerid_hashtbl[idhashval]);
	list_add(&sop->so_strhash, &ownerstr_hashtbl[strhashval]);
	list_add(&sop->so_perclient, &clp->cl_openowners);
	sop->so_is_open_owner = 1;
	sop->so_id = current_ownerid++;
	sop->so_client = clp;
	sop->so_seqid = open->op_seqid;
	sop->so_confirmed = 0;
	rp = &sop->so_replay;
	rp->rp_status = nfserr_serverfault;
	rp->rp_buflen = 0;
	rp->rp_buf = rp->rp_ibuf;
	return sop;
}

static inline void
init_stateid(struct nfs4_stateid *stp, struct nfs4_file *fp, struct nfsd4_open *open) {
	struct nfs4_stateowner *sop = open->op_stateowner;
	unsigned int hashval = stateid_hashval(sop->so_id, fp->fi_id);

	INIT_LIST_HEAD(&stp->st_hash);
	INIT_LIST_HEAD(&stp->st_perstateowner);
	INIT_LIST_HEAD(&stp->st_lockowners);
	INIT_LIST_HEAD(&stp->st_perfile);
	list_add(&stp->st_hash, &stateid_hashtbl[hashval]);
	list_add(&stp->st_perstateowner, &sop->so_stateids);
	list_add(&stp->st_perfile, &fp->fi_stateids);
	stp->st_stateowner = sop;
	get_nfs4_file(fp);
	stp->st_file = fp;
	stp->st_stateid.si_boot = boot_time;
	stp->st_stateid.si_stateownerid = sop->so_id;
	stp->st_stateid.si_fileid = fp->fi_id;
	stp->st_stateid.si_generation = 0;
	stp->st_access_bmap = 0;
	stp->st_deny_bmap = 0;
	__set_bit(open->op_share_access & ~NFS4_SHARE_WANT_MASK,
		  &stp->st_access_bmap);
=======
=======
>>>>>>> refs/remotes/origin/master
void nfs4_free_openowner(struct nfs4_openowner *oo)
{
	kfree(oo->oo_owner.so_owner.data);
	kmem_cache_free(openowner_slab, oo);
}

void nfs4_free_lockowner(struct nfs4_lockowner *lo)
{
	kfree(lo->lo_owner.so_owner.data);
	kmem_cache_free(lockowner_slab, lo);
}

static void init_nfs4_replay(struct nfs4_replay *rp)
{
	rp->rp_status = nfserr_serverfault;
	rp->rp_buflen = 0;
	rp->rp_buf = rp->rp_ibuf;
}

static inline void *alloc_stateowner(struct kmem_cache *slab, struct xdr_netobj *owner, struct nfs4_client *clp)
{
	struct nfs4_stateowner *sop;

	sop = kmem_cache_alloc(slab, GFP_KERNEL);
	if (!sop)
		return NULL;

	sop->so_owner.data = kmemdup(owner->data, owner->len, GFP_KERNEL);
	if (!sop->so_owner.data) {
		kmem_cache_free(slab, sop);
		return NULL;
	}
	sop->so_owner.len = owner->len;

	INIT_LIST_HEAD(&sop->so_stateids);
	sop->so_client = clp;
	init_nfs4_replay(&sop->so_replay);
	return sop;
}

static void hash_openowner(struct nfs4_openowner *oo, struct nfs4_client *clp, unsigned int strhashval)
{
<<<<<<< HEAD
	list_add(&oo->oo_owner.so_strhash, &ownerstr_hashtbl[strhashval]);
=======
	struct nfsd_net *nn = net_generic(clp->net, nfsd_net_id);

	list_add(&oo->oo_owner.so_strhash, &nn->ownerstr_hashtbl[strhashval]);
>>>>>>> refs/remotes/origin/master
	list_add(&oo->oo_perclient, &clp->cl_openowners);
}

static struct nfs4_openowner *
alloc_init_open_stateowner(unsigned int strhashval, struct nfs4_client *clp, struct nfsd4_open *open) {
	struct nfs4_openowner *oo;

	oo = alloc_stateowner(openowner_slab, &open->op_owner, clp);
	if (!oo)
		return NULL;
	oo->oo_owner.so_is_open_owner = 1;
	oo->oo_owner.so_seqid = open->op_seqid;
	oo->oo_flags = NFS4_OO_NEW;
	oo->oo_time = 0;
	oo->oo_last_closed_stid = NULL;
	INIT_LIST_HEAD(&oo->oo_close_lru);
	hash_openowner(oo, clp, strhashval);
	return oo;
}

static void init_open_stateid(struct nfs4_ol_stateid *stp, struct nfs4_file *fp, struct nfsd4_open *open) {
	struct nfs4_openowner *oo = open->op_openowner;
<<<<<<< HEAD
	struct nfs4_client *clp = oo->oo_owner.so_client;

	init_stid(&stp->st_stid, clp, NFS4_OPEN_STID);
=======

	stp->st_stid.sc_type = NFS4_OPEN_STID;
>>>>>>> refs/remotes/origin/master
	INIT_LIST_HEAD(&stp->st_lockowners);
	list_add(&stp->st_perstateowner, &oo->oo_owner.so_stateids);
	list_add(&stp->st_perfile, &fp->fi_stateids);
	stp->st_stateowner = &oo->oo_owner;
	get_nfs4_file(fp);
	stp->st_file = fp;
	stp->st_access_bmap = 0;
	stp->st_deny_bmap = 0;
<<<<<<< HEAD
	__set_bit(open->op_share_access, &stp->st_access_bmap);
>>>>>>> refs/remotes/origin/cm-10.0
	__set_bit(open->op_share_deny, &stp->st_deny_bmap);
=======
	set_access(open->op_share_access, stp);
	set_deny(open->op_share_deny, stp);
>>>>>>> refs/remotes/origin/master
	stp->st_openstp = NULL;
}

static void
<<<<<<< HEAD
<<<<<<< HEAD
move_to_close_lru(struct nfs4_stateowner *sop)
{
	dprintk("NFSD: move_to_close_lru nfs4_stateowner %p\n", sop);

	list_move_tail(&sop->so_close_lru, &close_lru);
	sop->so_time = get_seconds();
=======
move_to_close_lru(struct nfs4_openowner *oo)
{
	dprintk("NFSD: move_to_close_lru nfs4_openowner %p\n", oo);

	list_move_tail(&oo->oo_close_lru, &close_lru);
	oo->oo_time = get_seconds();
>>>>>>> refs/remotes/origin/cm-10.0
=======
move_to_close_lru(struct nfs4_openowner *oo, struct net *net)
{
	struct nfsd_net *nn = net_generic(net, nfsd_net_id);

	dprintk("NFSD: move_to_close_lru nfs4_openowner %p\n", oo);

	list_move_tail(&oo->oo_close_lru, &nn->close_lru);
	oo->oo_time = get_seconds();
>>>>>>> refs/remotes/origin/master
}

static int
same_owner_str(struct nfs4_stateowner *sop, struct xdr_netobj *owner,
							clientid_t *clid)
{
	return (sop->so_owner.len == owner->len) &&
		0 == memcmp(sop->so_owner.data, owner->data, owner->len) &&
		(sop->so_client->cl_clientid.cl_id == clid->cl_id);
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct nfs4_stateowner *
find_openstateowner_str(unsigned int hashval, struct nfsd4_open *open)
{
	struct nfs4_stateowner *so = NULL;

	list_for_each_entry(so, &ownerstr_hashtbl[hashval], so_strhash) {
		if (same_owner_str(so, &open->op_owner, &open->op_clientid))
			return so;
=======
static struct nfs4_openowner *
find_openstateowner_str(unsigned int hashval, struct nfsd4_open *open)
{
	struct nfs4_stateowner *so;
	struct nfs4_openowner *oo;

	list_for_each_entry(so, &ownerstr_hashtbl[hashval], so_strhash) {
=======
static struct nfs4_openowner *
find_openstateowner_str(unsigned int hashval, struct nfsd4_open *open,
			bool sessions, struct nfsd_net *nn)
{
	struct nfs4_stateowner *so;
	struct nfs4_openowner *oo;
	struct nfs4_client *clp;

	list_for_each_entry(so, &nn->ownerstr_hashtbl[hashval], so_strhash) {
>>>>>>> refs/remotes/origin/master
		if (!so->so_is_open_owner)
			continue;
		if (same_owner_str(so, &open->op_owner, &open->op_clientid)) {
			oo = openowner(so);
<<<<<<< HEAD
			renew_client(oo->oo_owner.so_client);
			return oo;
		}
>>>>>>> refs/remotes/origin/cm-10.0
=======
			clp = oo->oo_owner.so_client;
			if ((bool)clp->cl_minorversion != sessions)
				return NULL;
			renew_client(oo->oo_owner.so_client);
			return oo;
		}
>>>>>>> refs/remotes/origin/master
	}
	return NULL;
}

/* search file_hashtbl[] for file */
static struct nfs4_file *
find_file(struct inode *ino)
{
	unsigned int hashval = file_hashval(ino);
	struct nfs4_file *fp;

	spin_lock(&recall_lock);
<<<<<<< HEAD
	list_for_each_entry(fp, &file_hashtbl[hashval], fi_hash) {
=======
	hlist_for_each_entry(fp, &file_hashtbl[hashval], fi_hash) {
>>>>>>> refs/remotes/origin/master
		if (fp->fi_inode == ino) {
			get_nfs4_file(fp);
			spin_unlock(&recall_lock);
			return fp;
		}
	}
	spin_unlock(&recall_lock);
	return NULL;
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline int access_valid(u32 x, u32 minorversion)
{
	if ((x & NFS4_SHARE_ACCESS_MASK) < NFS4_SHARE_ACCESS_READ)
		return 0;
	if ((x & NFS4_SHARE_ACCESS_MASK) > NFS4_SHARE_ACCESS_BOTH)
		return 0;
	x &= ~NFS4_SHARE_ACCESS_MASK;
	if (minorversion && x) {
		if ((x & NFS4_SHARE_WANT_MASK) > NFS4_SHARE_WANT_CANCEL)
			return 0;
		if ((x & NFS4_SHARE_WHEN_MASK) > NFS4_SHARE_PUSH_DELEG_WHEN_UNCONTENDED)
			return 0;
		x &= ~(NFS4_SHARE_WANT_MASK | NFS4_SHARE_WHEN_MASK);
	}
	if (x)
		return 0;
	return 1;
}

static inline int deny_valid(u32 x)
{
	/* Note: unlike access bits, deny bits may be zero. */
	return x <= NFS4_SHARE_DENY_BOTH;
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * Called to check deny when READ with all zero stateid or
 * WRITE with all zero or all one stateid
 */
static __be32
nfs4_share_conflict(struct svc_fh *current_fh, unsigned int deny_type)
{
	struct inode *ino = current_fh->fh_dentry->d_inode;
	struct nfs4_file *fp;
<<<<<<< HEAD
<<<<<<< HEAD
	struct nfs4_stateid *stp;
=======
	struct nfs4_ol_stateid *stp;
>>>>>>> refs/remotes/origin/cm-10.0
	__be32 ret;

	dprintk("NFSD: nfs4_share_conflict\n");

=======
	struct nfs4_ol_stateid *stp;
	__be32 ret;

>>>>>>> refs/remotes/origin/master
	fp = find_file(ino);
	if (!fp)
		return nfs_ok;
	ret = nfserr_locked;
	/* Search for conflicting share reservations */
	list_for_each_entry(stp, &fp->fi_stateids, st_perfile) {
<<<<<<< HEAD
		if (test_bit(deny_type, &stp->st_deny_bmap) ||
		    test_bit(NFS4_SHARE_DENY_BOTH, &stp->st_deny_bmap))
=======
		if (test_deny(deny_type, stp) ||
		    test_deny(NFS4_SHARE_DENY_BOTH, stp))
>>>>>>> refs/remotes/origin/master
			goto out;
	}
	ret = nfs_ok;
out:
	put_nfs4_file(fp);
	return ret;
}

static void nfsd_break_one_deleg(struct nfs4_delegation *dp)
{
<<<<<<< HEAD
=======
	struct nfs4_client *clp = dp->dl_stid.sc_client;
	struct nfsd_net *nn = net_generic(clp->net, nfsd_net_id);

>>>>>>> refs/remotes/origin/master
	/* We're assuming the state code never drops its reference
	 * without first removing the lease.  Since we're in this lease
	 * callback (and since the lease code is serialized by the kernel
	 * lock) we know the server hasn't removed the lease yet, we know
	 * it's safe to take a reference: */
	atomic_inc(&dp->dl_count);

<<<<<<< HEAD
	list_add_tail(&dp->dl_recall_lru, &del_recall_lru);

	/* only place dl_time is set. protected by lock_flocks*/
=======
	list_add_tail(&dp->dl_recall_lru, &nn->del_recall_lru);

	/* Only place dl_time is set; protected by i_lock: */
>>>>>>> refs/remotes/origin/master
	dp->dl_time = get_seconds();

	nfsd4_cb_recall(dp);
}

<<<<<<< HEAD
/* Called from break_lease() with lock_flocks() held. */
=======
/* Called from break_lease() with i_lock held. */
>>>>>>> refs/remotes/origin/master
static void nfsd_break_deleg_cb(struct file_lock *fl)
{
	struct nfs4_file *fp = (struct nfs4_file *)fl->fl_owner;
	struct nfs4_delegation *dp;

<<<<<<< HEAD
	BUG_ON(!fp);
	/* We assume break_lease is only called once per lease: */
	BUG_ON(fp->fi_had_conflict);
=======
	if (!fp) {
		WARN(1, "(%p)->fl_owner NULL\n", fl);
		return;
	}
	if (fp->fi_had_conflict) {
		WARN(1, "duplicate break on %p\n", fp);
		return;
	}
>>>>>>> refs/remotes/origin/master
	/*
	 * We don't want the locks code to timeout the lease for us;
	 * we'll remove it ourself if a delegation isn't returned
	 * in time:
	 */
	fl->fl_break_time = 0;

	spin_lock(&recall_lock);
	fp->fi_had_conflict = true;
	list_for_each_entry(dp, &fp->fi_delegations, dl_perfile)
		nfsd_break_one_deleg(dp);
	spin_unlock(&recall_lock);
}

static
int nfsd_change_deleg_cb(struct file_lock **onlist, int arg)
{
	if (arg & F_UNLCK)
		return lease_modify(onlist, arg);
	else
		return -EAGAIN;
}

static const struct lock_manager_operations nfsd_lease_mng_ops = {
<<<<<<< HEAD
<<<<<<< HEAD
	.fl_break = nfsd_break_deleg_cb,
	.fl_change = nfsd_change_deleg_cb,
};

=======
=======
>>>>>>> refs/remotes/origin/master
	.lm_break = nfsd_break_deleg_cb,
	.lm_change = nfsd_change_deleg_cb,
};

static __be32 nfsd4_check_seqid(struct nfsd4_compound_state *cstate, struct nfs4_stateowner *so, u32 seqid)
{
	if (nfsd4_has_session(cstate))
		return nfs_ok;
	if (seqid == so->so_seqid - 1)
		return nfserr_replay_me;
	if (seqid == so->so_seqid)
		return nfs_ok;
	return nfserr_bad_seqid;
}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

__be32
nfsd4_process_open1(struct nfsd4_compound_state *cstate,
		    struct nfsd4_open *open)
=======

__be32
nfsd4_process_open1(struct nfsd4_compound_state *cstate,
		    struct nfsd4_open *open, struct nfsd_net *nn)
>>>>>>> refs/remotes/origin/master
{
	clientid_t *clientid = &open->op_clientid;
	struct nfs4_client *clp = NULL;
	unsigned int strhashval;
<<<<<<< HEAD
<<<<<<< HEAD
	struct nfs4_stateowner *sop = NULL;

	if (!check_name(open->op_owner))
		return nfserr_inval;

	if (STALE_CLIENTID(&open->op_clientid))
		return nfserr_stale_clientid;

	strhashval = ownerstr_hashval(clientid->cl_id, open->op_owner);
	sop = find_openstateowner_str(strhashval, open);
	open->op_stateowner = sop;
	if (!sop) {
		/* Make sure the client's lease hasn't expired. */
		clp = find_confirmed_client(clientid);
		if (clp == NULL)
			return nfserr_expired;
		goto renew;
	}
	/* When sessions are used, skip open sequenceid processing */
	if (nfsd4_has_session(cstate))
		goto renew;
	if (!sop->so_confirmed) {
		/* Replace unconfirmed owners without checking for replay. */
		clp = sop->so_client;
		release_openowner(sop);
		open->op_stateowner = NULL;
		goto renew;
	}
	if (open->op_seqid == sop->so_seqid - 1) {
		if (sop->so_replay.rp_buflen)
			return nfserr_replay_me;
		/* The original OPEN failed so spectacularly
		 * that we don't even have replay data saved!
		 * Therefore, we have no choice but to continue
		 * processing this OPEN; presumably, we'll
		 * fail again for the same reason.
		 */
		dprintk("nfsd4_process_open1: replay with no replay cache\n");
		goto renew;
	}
	if (open->op_seqid != sop->so_seqid)
		return nfserr_bad_seqid;
renew:
	if (open->op_stateowner == NULL) {
		sop = alloc_init_open_stateowner(strhashval, clp, open);
		if (sop == NULL)
			return nfserr_jukebox;
		open->op_stateowner = sop;
	}
	list_del_init(&sop->so_close_lru);
	renew_client(sop->so_client);
=======
	struct nfs4_openowner *oo = NULL;
	__be32 status;

	if (STALE_CLIENTID(&open->op_clientid))
=======
	struct nfs4_openowner *oo = NULL;
	__be32 status;

	if (STALE_CLIENTID(&open->op_clientid, nn))
>>>>>>> refs/remotes/origin/master
		return nfserr_stale_clientid;
	/*
	 * In case we need it later, after we've already created the
	 * file and don't want to risk a further failure:
	 */
	open->op_file = nfsd4_alloc_file();
	if (open->op_file == NULL)
		return nfserr_jukebox;

	strhashval = ownerstr_hashval(clientid->cl_id, &open->op_owner);
<<<<<<< HEAD
	oo = find_openstateowner_str(strhashval, open);
	open->op_openowner = oo;
	if (!oo) {
		clp = find_confirmed_client(clientid);
=======
	oo = find_openstateowner_str(strhashval, open, cstate->minorversion, nn);
	open->op_openowner = oo;
	if (!oo) {
		clp = find_confirmed_client(clientid, cstate->minorversion,
					    nn);
>>>>>>> refs/remotes/origin/master
		if (clp == NULL)
			return nfserr_expired;
		goto new_owner;
	}
	if (!(oo->oo_flags & NFS4_OO_CONFIRMED)) {
		/* Replace unconfirmed owners without checking for replay. */
		clp = oo->oo_owner.so_client;
		release_openowner(oo);
		open->op_openowner = NULL;
		goto new_owner;
	}
	status = nfsd4_check_seqid(cstate, &oo->oo_owner, open->op_seqid);
	if (status)
		return status;
	clp = oo->oo_owner.so_client;
	goto alloc_stateid;
new_owner:
	oo = alloc_init_open_stateowner(strhashval, clp, open);
	if (oo == NULL)
		return nfserr_jukebox;
	open->op_openowner = oo;
alloc_stateid:
	open->op_stp = nfs4_alloc_stateid(clp);
	if (!open->op_stp)
		return nfserr_jukebox;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return nfs_ok;
}

static inline __be32
nfs4_check_delegmode(struct nfs4_delegation *dp, int flags)
{
	if ((flags & WR_STATE) && (dp->dl_type == NFS4_OPEN_DELEGATE_READ))
		return nfserr_openmode;
	else
		return nfs_ok;
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct nfs4_delegation *
find_delegation_file(struct nfs4_file *fp, stateid_t *stid)
{
	struct nfs4_delegation *dp;

	spin_lock(&recall_lock);
	list_for_each_entry(dp, &fp->fi_delegations, dl_perfile)
		if (dp->dl_stateid.si_stateownerid == stid->si_stateownerid) {
			spin_unlock(&recall_lock);
			return dp;
		}
	spin_unlock(&recall_lock);
	return NULL;
}

static int share_access_to_flags(u32 share_access)
{
	share_access &= ~NFS4_SHARE_WANT_MASK;

	return share_access == NFS4_SHARE_ACCESS_READ ? RD_STATE : WR_STATE;
}

static __be32
nfs4_check_deleg(struct nfs4_file *fp, struct nfsd4_open *open,
=======
=======
>>>>>>> refs/remotes/origin/master
static int share_access_to_flags(u32 share_access)
{
	return share_access == NFS4_SHARE_ACCESS_READ ? RD_STATE : WR_STATE;
}

static struct nfs4_delegation *find_deleg_stateid(struct nfs4_client *cl, stateid_t *s)
{
	struct nfs4_stid *ret;

	ret = find_stateid_by_type(cl, s, NFS4_DELEG_STID);
	if (!ret)
		return NULL;
	return delegstateid(ret);
}

static bool nfsd4_is_deleg_cur(struct nfsd4_open *open)
{
	return open->op_claim_type == NFS4_OPEN_CLAIM_DELEGATE_CUR ||
	       open->op_claim_type == NFS4_OPEN_CLAIM_DELEG_CUR_FH;
}

static __be32
<<<<<<< HEAD
nfs4_check_deleg(struct nfs4_client *cl, struct nfs4_file *fp, struct nfsd4_open *open,
>>>>>>> refs/remotes/origin/cm-10.0
=======
nfs4_check_deleg(struct nfs4_client *cl, struct nfsd4_open *open,
>>>>>>> refs/remotes/origin/master
		struct nfs4_delegation **dp)
{
	int flags;
	__be32 status = nfserr_bad_stateid;

<<<<<<< HEAD
<<<<<<< HEAD
	*dp = find_delegation_file(fp, &open->op_delegate_stateid);
=======
	*dp = find_deleg_stateid(cl, &open->op_delegate_stateid);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	*dp = find_deleg_stateid(cl, &open->op_delegate_stateid);
>>>>>>> refs/remotes/origin/master
	if (*dp == NULL)
		goto out;
	flags = share_access_to_flags(open->op_share_access);
	status = nfs4_check_delegmode(*dp, flags);
	if (status)
		*dp = NULL;
out:
<<<<<<< HEAD
<<<<<<< HEAD
	if (open->op_claim_type != NFS4_OPEN_CLAIM_DELEGATE_CUR)
		return nfs_ok;
	if (status)
		return status;
	open->op_stateowner->so_confirmed = 1;
=======
=======
>>>>>>> refs/remotes/origin/master
	if (!nfsd4_is_deleg_cur(open))
		return nfs_ok;
	if (status)
		return status;
	open->op_openowner->oo_flags |= NFS4_OO_CONFIRMED;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return nfs_ok;
}

static __be32
<<<<<<< HEAD
<<<<<<< HEAD
nfs4_check_open(struct nfs4_file *fp, struct nfsd4_open *open, struct nfs4_stateid **stpp)
{
	struct nfs4_stateid *local;
	__be32 status = nfserr_share_denied;
	struct nfs4_stateowner *sop = open->op_stateowner;
=======
=======
>>>>>>> refs/remotes/origin/master
nfs4_check_open(struct nfs4_file *fp, struct nfsd4_open *open, struct nfs4_ol_stateid **stpp)
{
	struct nfs4_ol_stateid *local;
	struct nfs4_openowner *oo = open->op_openowner;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	list_for_each_entry(local, &fp->fi_stateids, st_perfile) {
		/* ignore lock owners */
		if (local->st_stateowner->so_is_open_owner == 0)
			continue;
		/* remember if we have seen this open owner */
<<<<<<< HEAD
<<<<<<< HEAD
		if (local->st_stateowner == sop)
			*stpp = local;
		/* check for conflicting share reservations */
		if (!test_share(local, open))
			goto out;
	}
	status = 0;
out:
	return status;
}

static inline struct nfs4_stateid *
nfs4_alloc_stateid(void)
{
	return kmem_cache_alloc(stateid_slab, GFP_KERNEL);
=======
=======
>>>>>>> refs/remotes/origin/master
		if (local->st_stateowner == &oo->oo_owner)
			*stpp = local;
		/* check for conflicting share reservations */
		if (!test_share(local, open))
			return nfserr_share_denied;
	}
	return nfs_ok;
}

<<<<<<< HEAD
static void nfs4_free_stateid(struct nfs4_ol_stateid *s)
{
	kmem_cache_free(stateid_slab, s);
>>>>>>> refs/remotes/origin/cm-10.0
}

=======
>>>>>>> refs/remotes/origin/master
static inline int nfs4_access_to_access(u32 nfs4_access)
{
	int flags = 0;

	if (nfs4_access & NFS4_SHARE_ACCESS_READ)
		flags |= NFSD_MAY_READ;
	if (nfs4_access & NFS4_SHARE_ACCESS_WRITE)
		flags |= NFSD_MAY_WRITE;
	return flags;
}

static __be32 nfs4_get_vfs_file(struct svc_rqst *rqstp, struct nfs4_file *fp,
		struct svc_fh *cur_fh, struct nfsd4_open *open)
{
	__be32 status;
	int oflag = nfs4_access_to_omode(open->op_share_access);
	int access = nfs4_access_to_access(open->op_share_access);

<<<<<<< HEAD
<<<<<<< HEAD
	/* CLAIM_DELEGATE_CUR is used in response to a broken lease;
	 * allowing it to break the lease and return EAGAIN leaves the
	 * client unable to make progress in returning the delegation */
	if (open->op_claim_type == NFS4_OPEN_CLAIM_DELEGATE_CUR)
		access |= NFSD_MAY_NOT_BREAK_LEASE;

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (!fp->fi_fds[oflag]) {
		status = nfsd_open(rqstp, cur_fh, S_IFREG, access,
			&fp->fi_fds[oflag]);
		if (status)
			return status;
	}
	nfs4_file_get_access(fp, oflag);

	return nfs_ok;
}

<<<<<<< HEAD
<<<<<<< HEAD
static __be32
nfs4_new_open(struct svc_rqst *rqstp, struct nfs4_stateid **stpp,
		struct nfs4_file *fp, struct svc_fh *cur_fh,
		struct nfsd4_open *open)
{
	struct nfs4_stateid *stp;
	__be32 status;

	stp = nfs4_alloc_stateid();
	if (stp == NULL)
		return nfserr_jukebox;

	status = nfs4_get_vfs_file(rqstp, fp, cur_fh, open);
	if (status) {
		kmem_cache_free(stateid_slab, stp);
		return status;
	}
	*stpp = stp;
	return 0;
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static inline __be32
nfsd4_truncate(struct svc_rqst *rqstp, struct svc_fh *fh,
		struct nfsd4_open *open)
{
	struct iattr iattr = {
		.ia_valid = ATTR_SIZE,
		.ia_size = 0,
	};
	if (!open->op_truncate)
		return 0;
	if (!(open->op_share_access & NFS4_SHARE_ACCESS_WRITE))
		return nfserr_inval;
	return nfsd_setattr(rqstp, fh, &iattr, 0, (time_t)0);
}

static __be32
<<<<<<< HEAD
<<<<<<< HEAD
nfs4_upgrade_open(struct svc_rqst *rqstp, struct nfs4_file *fp, struct svc_fh *cur_fh, struct nfs4_stateid *stp, struct nfsd4_open *open)
{
	u32 op_share_access = open->op_share_access & ~NFS4_SHARE_WANT_MASK;
=======
nfs4_upgrade_open(struct svc_rqst *rqstp, struct nfs4_file *fp, struct svc_fh *cur_fh, struct nfs4_ol_stateid *stp, struct nfsd4_open *open)
{
	u32 op_share_access = open->op_share_access;
>>>>>>> refs/remotes/origin/cm-10.0
	bool new_access;
	__be32 status;

	new_access = !test_bit(op_share_access, &stp->st_access_bmap);
=======
nfs4_upgrade_open(struct svc_rqst *rqstp, struct nfs4_file *fp, struct svc_fh *cur_fh, struct nfs4_ol_stateid *stp, struct nfsd4_open *open)
{
	u32 op_share_access = open->op_share_access;
	bool new_access;
	__be32 status;

	new_access = !test_access(op_share_access, stp);
>>>>>>> refs/remotes/origin/master
	if (new_access) {
		status = nfs4_get_vfs_file(rqstp, fp, cur_fh, open);
		if (status)
			return status;
	}
	status = nfsd4_truncate(rqstp, cur_fh, open);
	if (status) {
		if (new_access) {
			int oflag = nfs4_access_to_omode(op_share_access);
			nfs4_file_put_access(fp, oflag);
		}
		return status;
	}
	/* remember the open */
<<<<<<< HEAD
	__set_bit(op_share_access, &stp->st_access_bmap);
	__set_bit(open->op_share_deny, &stp->st_deny_bmap);
=======
	set_access(op_share_access, stp);
	set_deny(open->op_share_deny, stp);
>>>>>>> refs/remotes/origin/master

	return nfs_ok;
}


static void
<<<<<<< HEAD
<<<<<<< HEAD
nfs4_set_claim_prev(struct nfsd4_open *open)
{
	open->op_stateowner->so_confirmed = 1;
	open->op_stateowner->so_client->cl_firststate = 1;
=======
nfs4_set_claim_prev(struct nfsd4_open *open, bool has_session)
{
	open->op_openowner->oo_flags |= NFS4_OO_CONFIRMED;
>>>>>>> refs/remotes/origin/cm-10.0
=======
nfs4_set_claim_prev(struct nfsd4_open *open, bool has_session)
{
	open->op_openowner->oo_flags |= NFS4_OO_CONFIRMED;
>>>>>>> refs/remotes/origin/master
}

/* Should we give out recallable state?: */
static bool nfsd4_cb_channel_good(struct nfs4_client *clp)
{
	if (clp->cl_cb_state == NFSD4_CB_UP)
		return true;
	/*
	 * In the sessions case, since we don't have to establish a
	 * separate connection for callbacks, we assume it's OK
	 * until we hear otherwise:
	 */
	return clp->cl_minorversion && clp->cl_cb_state == NFSD4_CB_UNKNOWN;
}

static struct file_lock *nfs4_alloc_init_lease(struct nfs4_delegation *dp, int flag)
{
	struct file_lock *fl;

	fl = locks_alloc_lock();
	if (!fl)
		return NULL;
	locks_init_lock(fl);
	fl->fl_lmops = &nfsd_lease_mng_ops;
<<<<<<< HEAD
	fl->fl_flags = FL_LEASE;
=======
	fl->fl_flags = FL_DELEG;
>>>>>>> refs/remotes/origin/master
	fl->fl_type = flag == NFS4_OPEN_DELEGATE_READ? F_RDLCK: F_WRLCK;
	fl->fl_end = OFFSET_MAX;
	fl->fl_owner = (fl_owner_t)(dp->dl_file);
	fl->fl_pid = current->tgid;
	return fl;
}

<<<<<<< HEAD
static int nfs4_setlease(struct nfs4_delegation *dp, int flag)
=======
static int nfs4_setlease(struct nfs4_delegation *dp)
>>>>>>> refs/remotes/origin/master
{
	struct nfs4_file *fp = dp->dl_file;
	struct file_lock *fl;
	int status;

<<<<<<< HEAD
	fl = nfs4_alloc_init_lease(dp, flag);
	if (!fl)
		return -ENOMEM;
	fl->fl_file = find_readable_file(fp);
<<<<<<< HEAD
	list_add(&dp->dl_perclnt, &dp->dl_client->cl_delegations);
=======
	list_add(&dp->dl_perclnt, &dp->dl_stid.sc_client->cl_delegations);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	fl = nfs4_alloc_init_lease(dp, NFS4_OPEN_DELEGATE_READ);
	if (!fl)
		return -ENOMEM;
	fl->fl_file = find_readable_file(fp);
	list_add(&dp->dl_perclnt, &dp->dl_stid.sc_client->cl_delegations);
>>>>>>> refs/remotes/origin/master
	status = vfs_setlease(fl->fl_file, fl->fl_type, &fl);
	if (status) {
		list_del_init(&dp->dl_perclnt);
		locks_free_lock(fl);
<<<<<<< HEAD
		return -ENOMEM;
	}
	fp->fi_lease = fl;
	fp->fi_deleg_file = fl->fl_file;
	get_file(fp->fi_deleg_file);
=======
		return status;
	}
	fp->fi_lease = fl;
	fp->fi_deleg_file = get_file(fl->fl_file);
>>>>>>> refs/remotes/origin/master
	atomic_set(&fp->fi_delegees, 1);
	list_add(&dp->dl_perfile, &fp->fi_delegations);
	return 0;
}

<<<<<<< HEAD
static int nfs4_set_delegation(struct nfs4_delegation *dp, int flag)
{
	struct nfs4_file *fp = dp->dl_file;

	if (!fp->fi_lease)
		return nfs4_setlease(dp, flag);
	spin_lock(&recall_lock);
	if (fp->fi_had_conflict) {
		spin_unlock(&recall_lock);
		return -EAGAIN;
=======
static int nfs4_set_delegation(struct nfs4_delegation *dp, struct nfs4_file *fp)
{
	int status;

	if (fp->fi_had_conflict)
		return -EAGAIN;
	get_nfs4_file(fp);
	dp->dl_file = fp;
	if (!fp->fi_lease) {
		status = nfs4_setlease(dp);
		if (status)
			goto out_free;
		return 0;
	}
	spin_lock(&recall_lock);
	if (fp->fi_had_conflict) {
		spin_unlock(&recall_lock);
		status = -EAGAIN;
		goto out_free;
>>>>>>> refs/remotes/origin/master
	}
	atomic_inc(&fp->fi_delegees);
	list_add(&dp->dl_perfile, &fp->fi_delegations);
	spin_unlock(&recall_lock);
<<<<<<< HEAD
<<<<<<< HEAD
	list_add(&dp->dl_perclnt, &dp->dl_client->cl_delegations);
	return 0;
}

=======
	list_add(&dp->dl_perclnt, &dp->dl_stid.sc_client->cl_delegations);
	return 0;
=======
	list_add(&dp->dl_perclnt, &dp->dl_stid.sc_client->cl_delegations);
	return 0;
out_free:
	put_nfs4_file(fp);
	dp->dl_file = fp;
	return status;
>>>>>>> refs/remotes/origin/master
}

static void nfsd4_open_deleg_none_ext(struct nfsd4_open *open, int status)
{
	open->op_delegate_type = NFS4_OPEN_DELEGATE_NONE_EXT;
	if (status == -EAGAIN)
		open->op_why_no_deleg = WND4_CONTENTION;
	else {
		open->op_why_no_deleg = WND4_RESOURCE;
		switch (open->op_deleg_want) {
		case NFS4_SHARE_WANT_READ_DELEG:
		case NFS4_SHARE_WANT_WRITE_DELEG:
		case NFS4_SHARE_WANT_ANY_DELEG:
			break;
		case NFS4_SHARE_WANT_CANCEL:
			open->op_why_no_deleg = WND4_CANCELLED;
			break;
		case NFS4_SHARE_WANT_NO_DELEG:
<<<<<<< HEAD
			BUG();	/* not supposed to get here */
=======
			WARN_ON_ONCE(1);
>>>>>>> refs/remotes/origin/master
		}
	}
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
/*
 * Attempt to hand out a delegation.
 */
static void
<<<<<<< HEAD
nfs4_open_delegation(struct svc_fh *fh, struct nfsd4_open *open, struct nfs4_stateid *stp)
{
	struct nfs4_delegation *dp;
	struct nfs4_stateowner *sop = stp->st_stateowner;
	int cb_up;
	int status, flag = 0;

	cb_up = nfsd4_cb_channel_good(sop->so_client);
=======
nfs4_open_delegation(struct svc_fh *fh, struct nfsd4_open *open, struct nfs4_ol_stateid *stp)
=======
/*
 * Attempt to hand out a delegation.
 *
 * Note we don't support write delegations, and won't until the vfs has
 * proper support for them.
 */
static void
nfs4_open_delegation(struct net *net, struct svc_fh *fh,
		     struct nfsd4_open *open, struct nfs4_ol_stateid *stp)
>>>>>>> refs/remotes/origin/master
{
	struct nfs4_delegation *dp;
	struct nfs4_openowner *oo = container_of(stp->st_stateowner, struct nfs4_openowner, oo_owner);
	int cb_up;
<<<<<<< HEAD
	int status = 0, flag = 0;

	cb_up = nfsd4_cb_channel_good(oo->oo_owner.so_client);
>>>>>>> refs/remotes/origin/cm-10.0
	flag = NFS4_OPEN_DELEGATE_NONE;
=======
	int status = 0;

	cb_up = nfsd4_cb_channel_good(oo->oo_owner.so_client);
>>>>>>> refs/remotes/origin/master
	open->op_recall = 0;
	switch (open->op_claim_type) {
		case NFS4_OPEN_CLAIM_PREVIOUS:
			if (!cb_up)
				open->op_recall = 1;
<<<<<<< HEAD
			flag = open->op_delegate_type;
			if (flag == NFS4_OPEN_DELEGATE_NONE)
				goto out;
			break;
		case NFS4_OPEN_CLAIM_NULL:
			/* Let's not give out any delegations till everyone's
			 * had the chance to reclaim theirs.... */
			if (locks_in_grace())
				goto out;
<<<<<<< HEAD
			if (!cb_up || !sop->so_confirmed)
=======
			if (!cb_up || !(oo->oo_flags & NFS4_OO_CONFIRMED))
>>>>>>> refs/remotes/origin/cm-10.0
				goto out;
			if (open->op_share_access & NFS4_SHARE_ACCESS_WRITE)
				flag = NFS4_OPEN_DELEGATE_WRITE;
			else
				flag = NFS4_OPEN_DELEGATE_READ;
			break;
		default:
			goto out;
	}

<<<<<<< HEAD
	dp = alloc_init_deleg(sop->so_client, stp, fh, flag);
=======
	dp = alloc_init_deleg(oo->oo_owner.so_client, stp, fh, flag);
>>>>>>> refs/remotes/origin/cm-10.0
	if (dp == NULL)
		goto out_no_deleg;
	status = nfs4_set_delegation(dp, flag);
	if (status)
		goto out_free;

<<<<<<< HEAD
	memcpy(&open->op_delegate_stateid, &dp->dl_stateid, sizeof(dp->dl_stateid));

	dprintk("NFSD: delegation stateid=" STATEID_FMT "\n",
		STATEID_VAL(&dp->dl_stateid));
out:
	if (open->op_claim_type == NFS4_OPEN_CLAIM_PREVIOUS
			&& flag == NFS4_OPEN_DELEGATE_NONE
			&& open->op_delegate_type != NFS4_OPEN_DELEGATE_NONE)
		dprintk("NFSD: WARNING: refusing delegation reclaim\n");
	open->op_delegate_type = flag;
=======
=======
			if (open->op_delegate_type != NFS4_OPEN_DELEGATE_READ)
				goto out_no_deleg;
			break;
		case NFS4_OPEN_CLAIM_NULL:
			/*
			 * Let's not give out any delegations till everyone's
			 * had the chance to reclaim theirs....
			 */
			if (locks_in_grace(net))
				goto out_no_deleg;
			if (!cb_up || !(oo->oo_flags & NFS4_OO_CONFIRMED))
				goto out_no_deleg;
			/*
			 * Also, if the file was opened for write or
			 * create, there's a good chance the client's
			 * about to write to it, resulting in an
			 * immediate recall (since we don't support
			 * write delegations):
			 */
			if (open->op_share_access & NFS4_SHARE_ACCESS_WRITE)
				goto out_no_deleg;
			if (open->op_create == NFS4_OPEN_CREATE)
				goto out_no_deleg;
			break;
		default:
			goto out_no_deleg;
	}
	dp = alloc_init_deleg(oo->oo_owner.so_client, stp, fh);
	if (dp == NULL)
		goto out_no_deleg;
	status = nfs4_set_delegation(dp, stp->st_file);
	if (status)
		goto out_free;

>>>>>>> refs/remotes/origin/master
	memcpy(&open->op_delegate_stateid, &dp->dl_stid.sc_stateid, sizeof(dp->dl_stid.sc_stateid));

	dprintk("NFSD: delegation stateid=" STATEID_FMT "\n",
		STATEID_VAL(&dp->dl_stid.sc_stateid));
<<<<<<< HEAD
out:
	open->op_delegate_type = flag;
	if (flag == NFS4_OPEN_DELEGATE_NONE) {
		if (open->op_claim_type == NFS4_OPEN_CLAIM_PREVIOUS &&
		    open->op_delegate_type != NFS4_OPEN_DELEGATE_NONE)
			dprintk("NFSD: WARNING: refusing delegation reclaim\n");

		/* 4.1 client asking for a delegation? */
		if (open->op_deleg_want)
			nfsd4_open_deleg_none_ext(open, status);
	}
>>>>>>> refs/remotes/origin/cm-10.0
	return;
out_free:
	nfs4_put_delegation(dp);
out_no_deleg:
	flag = NFS4_OPEN_DELEGATE_NONE;
	goto out;
}

<<<<<<< HEAD
=======
=======
	open->op_delegate_type = NFS4_OPEN_DELEGATE_READ;
	return;
out_free:
	remove_stid(&dp->dl_stid);
	nfs4_put_delegation(dp);
out_no_deleg:
	open->op_delegate_type = NFS4_OPEN_DELEGATE_NONE;
	if (open->op_claim_type == NFS4_OPEN_CLAIM_PREVIOUS &&
	    open->op_delegate_type != NFS4_OPEN_DELEGATE_NONE) {
		dprintk("NFSD: WARNING: refusing delegation reclaim\n");
		open->op_recall = 1;
	}

	/* 4.1 client asking for a delegation? */
	if (open->op_deleg_want)
		nfsd4_open_deleg_none_ext(open, status);
	return;
}

>>>>>>> refs/remotes/origin/master
static void nfsd4_deleg_xgrade_none_ext(struct nfsd4_open *open,
					struct nfs4_delegation *dp)
{
	if (open->op_deleg_want == NFS4_SHARE_WANT_READ_DELEG &&
	    dp->dl_type == NFS4_OPEN_DELEGATE_WRITE) {
		open->op_delegate_type = NFS4_OPEN_DELEGATE_NONE_EXT;
		open->op_why_no_deleg = WND4_NOT_SUPP_DOWNGRADE;
	} else if (open->op_deleg_want == NFS4_SHARE_WANT_WRITE_DELEG &&
		   dp->dl_type == NFS4_OPEN_DELEGATE_WRITE) {
		open->op_delegate_type = NFS4_OPEN_DELEGATE_NONE_EXT;
		open->op_why_no_deleg = WND4_NOT_SUPP_UPGRADE;
	}
	/* Otherwise the client must be confused wanting a delegation
	 * it already has, therefore we don't return
	 * NFS4_OPEN_DELEGATE_NONE_EXT and reason.
	 */
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * called with nfs4_lock_state() held.
 */
__be32
nfsd4_process_open2(struct svc_rqst *rqstp, struct svc_fh *current_fh, struct nfsd4_open *open)
{
	struct nfsd4_compoundres *resp = rqstp->rq_resp;
<<<<<<< HEAD
<<<<<<< HEAD
	struct nfs4_file *fp = NULL;
	struct inode *ino = current_fh->fh_dentry->d_inode;
	struct nfs4_stateid *stp = NULL;
	struct nfs4_delegation *dp = NULL;
	__be32 status;

	status = nfserr_inval;
	if (!access_valid(open->op_share_access, resp->cstate.minorversion)
			|| !deny_valid(open->op_share_deny))
		goto out;
=======
=======
>>>>>>> refs/remotes/origin/master
	struct nfs4_client *cl = open->op_openowner->oo_owner.so_client;
	struct nfs4_file *fp = NULL;
	struct inode *ino = current_fh->fh_dentry->d_inode;
	struct nfs4_ol_stateid *stp = NULL;
	struct nfs4_delegation *dp = NULL;
	__be32 status;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * Lookup file; if found, lookup stateid and check open request,
	 * and check for delegations in the process of being recalled.
	 * If not found, create the nfs4_file struct
	 */
	fp = find_file(ino);
	if (fp) {
		if ((status = nfs4_check_open(fp, open, &stp)))
			goto out;
<<<<<<< HEAD
<<<<<<< HEAD
		status = nfs4_check_deleg(fp, open, &dp);
=======
		status = nfs4_check_deleg(cl, fp, open, &dp);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		status = nfs4_check_deleg(cl, open, &dp);
>>>>>>> refs/remotes/origin/master
		if (status)
			goto out;
	} else {
		status = nfserr_bad_stateid;
<<<<<<< HEAD
<<<<<<< HEAD
		if (open->op_claim_type == NFS4_OPEN_CLAIM_DELEGATE_CUR)
			goto out;
		status = nfserr_jukebox;
		fp = alloc_init_file(ino);
		if (fp == NULL)
			goto out;
=======
=======
>>>>>>> refs/remotes/origin/master
		if (nfsd4_is_deleg_cur(open))
			goto out;
		status = nfserr_jukebox;
		fp = open->op_file;
		open->op_file = NULL;
		nfsd4_init_file(fp, ino);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	/*
	 * OPEN the file, or upgrade an existing OPEN.
	 * If truncate fails, the OPEN fails.
	 */
	if (stp) {
		/* Stateid was found, this is an OPEN upgrade */
		status = nfs4_upgrade_open(rqstp, fp, current_fh, stp, open);
		if (status)
			goto out;
<<<<<<< HEAD
<<<<<<< HEAD
		update_stateid(&stp->st_stateid);
	} else {
		status = nfs4_new_open(rqstp, &stp, fp, current_fh, open);
		if (status)
			goto out;
		init_stateid(stp, fp, open);
=======
	} else {
		status = nfs4_get_vfs_file(rqstp, fp, current_fh, open);
=======
	} else {
		status = nfs4_get_vfs_file(rqstp, fp, current_fh, open);
		if (status)
			goto out;
		status = nfsd4_truncate(rqstp, current_fh, open);
>>>>>>> refs/remotes/origin/master
		if (status)
			goto out;
		stp = open->op_stp;
		open->op_stp = NULL;
		init_open_stateid(stp, fp, open);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		status = nfsd4_truncate(rqstp, current_fh, open);
		if (status) {
			release_open_stateid(stp);
			goto out;
		}
<<<<<<< HEAD
		if (nfsd4_has_session(&resp->cstate))
			update_stateid(&stp->st_stateid);
	}
	memcpy(&open->op_stateid, &stp->st_stateid, sizeof(stateid_t));

	if (nfsd4_has_session(&resp->cstate))
		open->op_stateowner->so_confirmed = 1;
=======
=======
>>>>>>> refs/remotes/origin/master
	}
	update_stateid(&stp->st_stid.sc_stateid);
	memcpy(&open->op_stateid, &stp->st_stid.sc_stateid, sizeof(stateid_t));

	if (nfsd4_has_session(&resp->cstate)) {
		open->op_openowner->oo_flags |= NFS4_OO_CONFIRMED;

		if (open->op_deleg_want & NFS4_SHARE_WANT_NO_DELEG) {
			open->op_delegate_type = NFS4_OPEN_DELEGATE_NONE_EXT;
			open->op_why_no_deleg = WND4_NOT_WANTED;
			goto nodeleg;
		}
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	/*
	* Attempt to hand out a delegation. No error return, because the
	* OPEN succeeds even if we fail.
	*/
<<<<<<< HEAD
	nfs4_open_delegation(current_fh, open, stp);
<<<<<<< HEAD

	status = nfs_ok;

	dprintk("%s: stateid=" STATEID_FMT "\n", __func__,
		STATEID_VAL(&stp->st_stateid));
out:
	if (fp)
		put_nfs4_file(fp);
	if (status == 0 && open->op_claim_type == NFS4_OPEN_CLAIM_PREVIOUS)
		nfs4_set_claim_prev(open);
=======
=======
	nfs4_open_delegation(SVC_NET(rqstp), current_fh, open, stp);
>>>>>>> refs/remotes/origin/master
nodeleg:
	status = nfs_ok;

	dprintk("%s: stateid=" STATEID_FMT "\n", __func__,
		STATEID_VAL(&stp->st_stid.sc_stateid));
out:
	/* 4.1 client trying to upgrade/downgrade delegation? */
	if (open->op_delegate_type == NFS4_OPEN_DELEGATE_NONE && dp &&
	    open->op_deleg_want)
		nfsd4_deleg_xgrade_none_ext(open, dp);

	if (fp)
		put_nfs4_file(fp);
	if (status == 0 && open->op_claim_type == NFS4_OPEN_CLAIM_PREVIOUS)
		nfs4_set_claim_prev(open, nfsd4_has_session(&resp->cstate));
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	/*
	* To finish the open response, we just need to set the rflags.
	*/
	open->op_rflags = NFS4_OPEN_RESULT_LOCKTYPE_POSIX;
<<<<<<< HEAD
<<<<<<< HEAD
	if (!open->op_stateowner->so_confirmed &&
=======
	if (!(open->op_openowner->oo_flags & NFS4_OO_CONFIRMED) &&
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!(open->op_openowner->oo_flags & NFS4_OO_CONFIRMED) &&
>>>>>>> refs/remotes/origin/master
	    !nfsd4_has_session(&resp->cstate))
		open->op_rflags |= NFS4_OPEN_RESULT_CONFIRM;

	return status;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
void nfsd4_cleanup_open_state(struct nfsd4_open *open, __be32 status)
{
	if (open->op_openowner) {
		struct nfs4_openowner *oo = open->op_openowner;

		if (!list_empty(&oo->oo_owner.so_stateids))
			list_del_init(&oo->oo_close_lru);
		if (oo->oo_flags & NFS4_OO_NEW) {
			if (status) {
				release_openowner(oo);
				open->op_openowner = NULL;
			} else
				oo->oo_flags &= ~NFS4_OO_NEW;
		}
	}
	if (open->op_file)
		nfsd4_free_file(open->op_file);
	if (open->op_stp)
<<<<<<< HEAD
		nfs4_free_stateid(open->op_stp);
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
		free_generic_stateid(open->op_stp);
}

static __be32 lookup_clientid(clientid_t *clid, bool session, struct nfsd_net *nn, struct nfs4_client **clp)
{
	struct nfs4_client *found;

	if (STALE_CLIENTID(clid, nn))
		return nfserr_stale_clientid;
	found = find_confirmed_client(clid, session, nn);
	if (clp)
		*clp = found;
	return found ? nfs_ok : nfserr_expired;
}

>>>>>>> refs/remotes/origin/master
__be32
nfsd4_renew(struct svc_rqst *rqstp, struct nfsd4_compound_state *cstate,
	    clientid_t *clid)
{
	struct nfs4_client *clp;
	__be32 status;
<<<<<<< HEAD
=======
	struct nfsd_net *nn = net_generic(SVC_NET(rqstp), nfsd_net_id);
>>>>>>> refs/remotes/origin/master

	nfs4_lock_state();
	dprintk("process_renew(%08x/%08x): starting\n", 
			clid->cl_boot, clid->cl_id);
<<<<<<< HEAD
	status = nfserr_stale_clientid;
	if (STALE_CLIENTID(clid))
		goto out;
	clp = find_confirmed_client(clid);
	status = nfserr_expired;
	if (clp == NULL) {
		/* We assume the client took too long to RENEW. */
		dprintk("nfsd4_renew: clientid not found!\n");
		goto out;
	}
<<<<<<< HEAD
	renew_client(clp);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	status = lookup_clientid(clid, cstate->minorversion, nn, &clp);
	if (status)
		goto out;
>>>>>>> refs/remotes/origin/master
	status = nfserr_cb_path_down;
	if (!list_empty(&clp->cl_delegations)
			&& clp->cl_cb_state != NFSD4_CB_UP)
		goto out;
	status = nfs_ok;
out:
	nfs4_unlock_state();
	return status;
}

<<<<<<< HEAD
static struct lock_manager nfsd4_manager = {
};

static void
nfsd4_end_grace(void)
{
	dprintk("NFSD: end of grace period\n");
<<<<<<< HEAD
	nfsd4_recdir_purge_old();
=======
	nfsd4_record_grace_done(&init_net, boot_time);
>>>>>>> refs/remotes/origin/cm-10.0
	locks_end_grace(&nfsd4_manager);
=======
static void
nfsd4_end_grace(struct nfsd_net *nn)
{
	/* do nothing if grace period already ended */
	if (nn->grace_ended)
		return;

	dprintk("NFSD: end of grace period\n");
	nn->grace_ended = true;
	nfsd4_record_grace_done(nn, nn->boot_time);
	locks_end_grace(&nn->nfsd4_manager);
>>>>>>> refs/remotes/origin/master
	/*
	 * Now that every NFSv4 client has had the chance to recover and
	 * to see the (possibly new, possibly shorter) lease time, we
	 * can safely set the next grace time to the current lease time:
	 */
<<<<<<< HEAD
	nfsd4_grace = nfsd4_lease;
}

static time_t
nfs4_laundromat(void)
{
	struct nfs4_client *clp;
<<<<<<< HEAD
	struct nfs4_stateowner *sop;
=======
	struct nfs4_openowner *oo;
>>>>>>> refs/remotes/origin/cm-10.0
	struct nfs4_delegation *dp;
	struct list_head *pos, *next, reaplist;
	time_t cutoff = get_seconds() - nfsd4_lease;
	time_t t, clientid_val = nfsd4_lease;
	time_t u, test_val = nfsd4_lease;
=======
	nn->nfsd4_grace = nn->nfsd4_lease;
}

static time_t
nfs4_laundromat(struct nfsd_net *nn)
{
	struct nfs4_client *clp;
	struct nfs4_openowner *oo;
	struct nfs4_delegation *dp;
	struct list_head *pos, *next, reaplist;
	time_t cutoff = get_seconds() - nn->nfsd4_lease;
	time_t t, clientid_val = nn->nfsd4_lease;
	time_t u, test_val = nn->nfsd4_lease;
>>>>>>> refs/remotes/origin/master

	nfs4_lock_state();

	dprintk("NFSD: laundromat service - starting\n");
<<<<<<< HEAD
	if (locks_in_grace())
		nfsd4_end_grace();
	INIT_LIST_HEAD(&reaplist);
	spin_lock(&client_lock);
	list_for_each_safe(pos, next, &client_lru) {
=======
	nfsd4_end_grace(nn);
	INIT_LIST_HEAD(&reaplist);
	spin_lock(&nn->client_lock);
	list_for_each_safe(pos, next, &nn->client_lru) {
>>>>>>> refs/remotes/origin/master
		clp = list_entry(pos, struct nfs4_client, cl_lru);
		if (time_after((unsigned long)clp->cl_time, (unsigned long)cutoff)) {
			t = clp->cl_time - cutoff;
			if (clientid_val > t)
				clientid_val = t;
			break;
		}
<<<<<<< HEAD
		if (atomic_read(&clp->cl_refcount)) {
=======
		if (mark_client_expired_locked(clp)) {
>>>>>>> refs/remotes/origin/master
			dprintk("NFSD: client in use (clientid %08x)\n",
				clp->cl_clientid.cl_id);
			continue;
		}
<<<<<<< HEAD
		unhash_client_locked(clp);
		list_add(&clp->cl_lru, &reaplist);
	}
	spin_unlock(&client_lock);
=======
		list_move(&clp->cl_lru, &reaplist);
	}
	spin_unlock(&nn->client_lock);
>>>>>>> refs/remotes/origin/master
	list_for_each_safe(pos, next, &reaplist) {
		clp = list_entry(pos, struct nfs4_client, cl_lru);
		dprintk("NFSD: purging unused client (clientid %08x)\n",
			clp->cl_clientid.cl_id);
<<<<<<< HEAD
<<<<<<< HEAD
		nfsd4_remove_clid_dir(clp);
=======
		nfsd4_client_record_remove(clp);
>>>>>>> refs/remotes/origin/cm-10.0
		expire_client(clp);
	}
	spin_lock(&recall_lock);
	list_for_each_safe(pos, next, &del_recall_lru) {
		dp = list_entry (pos, struct nfs4_delegation, dl_recall_lru);
=======
		expire_client(clp);
	}
	spin_lock(&recall_lock);
	list_for_each_safe(pos, next, &nn->del_recall_lru) {
		dp = list_entry (pos, struct nfs4_delegation, dl_recall_lru);
		if (net_generic(dp->dl_stid.sc_client->net, nfsd_net_id) != nn)
			continue;
>>>>>>> refs/remotes/origin/master
		if (time_after((unsigned long)dp->dl_time, (unsigned long)cutoff)) {
			u = dp->dl_time - cutoff;
			if (test_val > u)
				test_val = u;
			break;
		}
		list_move(&dp->dl_recall_lru, &reaplist);
	}
	spin_unlock(&recall_lock);
	list_for_each_safe(pos, next, &reaplist) {
		dp = list_entry (pos, struct nfs4_delegation, dl_recall_lru);
<<<<<<< HEAD
<<<<<<< HEAD
		list_del_init(&dp->dl_recall_lru);
=======
>>>>>>> refs/remotes/origin/cm-10.0
		unhash_delegation(dp);
	}
	test_val = nfsd4_lease;
	list_for_each_safe(pos, next, &close_lru) {
<<<<<<< HEAD
		sop = list_entry(pos, struct nfs4_stateowner, so_close_lru);
		if (time_after((unsigned long)sop->so_time, (unsigned long)cutoff)) {
			u = sop->so_time - cutoff;
=======
		oo = container_of(pos, struct nfs4_openowner, oo_close_lru);
		if (time_after((unsigned long)oo->oo_time, (unsigned long)cutoff)) {
			u = oo->oo_time - cutoff;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		revoke_delegation(dp);
	}
	test_val = nn->nfsd4_lease;
	list_for_each_safe(pos, next, &nn->close_lru) {
		oo = container_of(pos, struct nfs4_openowner, oo_close_lru);
		if (time_after((unsigned long)oo->oo_time, (unsigned long)cutoff)) {
			u = oo->oo_time - cutoff;
>>>>>>> refs/remotes/origin/master
			if (test_val > u)
				test_val = u;
			break;
		}
<<<<<<< HEAD
<<<<<<< HEAD
		dprintk("NFSD: purging unused open stateowner (so_id %d)\n",
			sop->so_id);
		release_openowner(sop);
=======
		release_openowner(oo);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		release_openowner(oo);
>>>>>>> refs/remotes/origin/master
	}
	if (clientid_val < NFSD_LAUNDROMAT_MINTIMEOUT)
		clientid_val = NFSD_LAUNDROMAT_MINTIMEOUT;
	nfs4_unlock_state();
	return clientid_val;
}

static struct workqueue_struct *laundry_wq;
static void laundromat_main(struct work_struct *);
<<<<<<< HEAD
static DECLARE_DELAYED_WORK(laundromat_work, laundromat_main);

static void
laundromat_main(struct work_struct *not_used)
{
	time_t t;

	t = nfs4_laundromat();
	dprintk("NFSD: laundromat_main - sleeping for %ld seconds\n", t);
	queue_delayed_work(laundry_wq, &laundromat_work, t*HZ);
}

<<<<<<< HEAD
static struct nfs4_stateowner *
search_close_lru(u32 st_id, int flags)
{
	struct nfs4_stateowner *local = NULL;

	if (flags & CLOSE_STATE) {
		list_for_each_entry(local, &close_lru, so_close_lru) {
			if (local->so_id == st_id)
				return local;
		}
	}
	return NULL;
}

static inline int
nfs4_check_fh(struct svc_fh *fhp, struct nfs4_stateid *stp)
{
	return fhp->fh_dentry->d_inode != stp->st_file->fi_inode;
=======
=======

static void
laundromat_main(struct work_struct *laundry)
{
	time_t t;
	struct delayed_work *dwork = container_of(laundry, struct delayed_work,
						  work);
	struct nfsd_net *nn = container_of(dwork, struct nfsd_net,
					   laundromat_work);

	t = nfs4_laundromat(nn);
	dprintk("NFSD: laundromat_main - sleeping for %ld seconds\n", t);
	queue_delayed_work(laundry_wq, &nn->laundromat_work, t*HZ);
}

>>>>>>> refs/remotes/origin/master
static inline __be32 nfs4_check_fh(struct svc_fh *fhp, struct nfs4_ol_stateid *stp)
{
	if (fhp->fh_dentry->d_inode != stp->st_file->fi_inode)
		return nfserr_bad_stateid;
	return nfs_ok;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
}

static int
STALE_STATEID(stateid_t *stateid)
{
<<<<<<< HEAD
	if (stateid->si_boot == boot_time)
=======
	if (stateid->si_opaque.so_clid.cl_boot == boot_time)
>>>>>>> refs/remotes/origin/cm-10.0
		return 0;
	dprintk("NFSD: stale stateid " STATEID_FMT "!\n",
		STATEID_VAL(stateid));
	return 1;
}

static inline int
access_permit_read(unsigned long access_bmap)
{
	return test_bit(NFS4_SHARE_ACCESS_READ, &access_bmap) ||
		test_bit(NFS4_SHARE_ACCESS_BOTH, &access_bmap) ||
		test_bit(NFS4_SHARE_ACCESS_WRITE, &access_bmap);
}

static inline int
access_permit_write(unsigned long access_bmap)
{
	return test_bit(NFS4_SHARE_ACCESS_WRITE, &access_bmap) ||
		test_bit(NFS4_SHARE_ACCESS_BOTH, &access_bmap);
}

static
<<<<<<< HEAD
__be32 nfs4_check_openmode(struct nfs4_stateid *stp, int flags)
=======
__be32 nfs4_check_openmode(struct nfs4_ol_stateid *stp, int flags)
>>>>>>> refs/remotes/origin/cm-10.0
=======
}

static inline int
access_permit_read(struct nfs4_ol_stateid *stp)
{
	return test_access(NFS4_SHARE_ACCESS_READ, stp) ||
		test_access(NFS4_SHARE_ACCESS_BOTH, stp) ||
		test_access(NFS4_SHARE_ACCESS_WRITE, stp);
}

static inline int
access_permit_write(struct nfs4_ol_stateid *stp)
{
	return test_access(NFS4_SHARE_ACCESS_WRITE, stp) ||
		test_access(NFS4_SHARE_ACCESS_BOTH, stp);
}

static
__be32 nfs4_check_openmode(struct nfs4_ol_stateid *stp, int flags)
>>>>>>> refs/remotes/origin/master
{
        __be32 status = nfserr_openmode;

	/* For lock stateid's, we test the parent open, not the lock: */
	if (stp->st_openstp)
		stp = stp->st_openstp;
<<<<<<< HEAD
	if ((flags & WR_STATE) && (!access_permit_write(stp->st_access_bmap)))
                goto out;
	if ((flags & RD_STATE) && (!access_permit_read(stp->st_access_bmap)))
=======
	if ((flags & WR_STATE) && !access_permit_write(stp))
                goto out;
	if ((flags & RD_STATE) && !access_permit_read(stp))
>>>>>>> refs/remotes/origin/master
                goto out;
	status = nfs_ok;
out:
	return status;
}

static inline __be32
<<<<<<< HEAD
check_special_stateids(svc_fh *current_fh, stateid_t *stateid, int flags)
{
	if (ONE_STATEID(stateid) && (flags & RD_STATE))
		return nfs_ok;
	else if (locks_in_grace()) {
=======
check_special_stateids(struct net *net, svc_fh *current_fh, stateid_t *stateid, int flags)
{
	if (ONE_STATEID(stateid) && (flags & RD_STATE))
		return nfs_ok;
	else if (locks_in_grace(net)) {
>>>>>>> refs/remotes/origin/master
		/* Answer in remaining cases depends on existence of
		 * conflicting state; so we must wait out the grace period. */
		return nfserr_grace;
	} else if (flags & WR_STATE)
		return nfs4_share_conflict(current_fh,
				NFS4_SHARE_DENY_WRITE);
	else /* (flags & RD_STATE) && ZERO_STATEID(stateid) */
		return nfs4_share_conflict(current_fh,
				NFS4_SHARE_DENY_READ);
}

/*
 * Allow READ/WRITE during grace period on recovered state only for files
 * that are not able to provide mandatory locking.
 */
static inline int
<<<<<<< HEAD
grace_disallows_io(struct inode *inode)
{
	return locks_in_grace() && mandatory_lock(inode);
}

<<<<<<< HEAD
static int check_stateid_generation(stateid_t *in, stateid_t *ref, int flags)
=======
/* Returns true iff a is later than b: */
static bool stateid_generation_after(stateid_t *a, stateid_t *b)
{
	return (s32)a->si_generation - (s32)b->si_generation > 0;
}

static int check_stateid_generation(stateid_t *in, stateid_t *ref, bool has_session)
>>>>>>> refs/remotes/origin/cm-10.0
=======
grace_disallows_io(struct net *net, struct inode *inode)
{
	return locks_in_grace(net) && mandatory_lock(inode);
}

/* Returns true iff a is later than b: */
static bool stateid_generation_after(stateid_t *a, stateid_t *b)
{
	return (s32)(a->si_generation - b->si_generation) > 0;
}

static __be32 check_stateid_generation(stateid_t *in, stateid_t *ref, bool has_session)
>>>>>>> refs/remotes/origin/master
{
	/*
	 * When sessions are used the stateid generation number is ignored
	 * when it is zero.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if ((flags & HAS_SESSION) && in->si_generation == 0)
		goto out;

	/* If the client sends us a stateid from the future, it's buggy: */
	if (in->si_generation > ref->si_generation)
		return nfserr_bad_stateid;
	/*
	 * The following, however, can happen.  For example, if the
	 * client sends an open and some IO at the same time, the open
	 * may bump si_generation while the IO is still in flight.
	 * Thanks to hard links and renames, the client never knows what
	 * file an open will affect.  So it could avoid that situation
	 * only by serializing all opens and IO from the same open
	 * owner.  To recover from the old_stateid error, the client
	 * will just have to retry the IO:
	 */
	if (in->si_generation < ref->si_generation)
		return nfserr_old_stateid;
out:
	return nfs_ok;
}

static int is_delegation_stateid(stateid_t *stateid)
{
	return stateid->si_fileid == 0;
=======
=======
>>>>>>> refs/remotes/origin/master
	if (has_session && in->si_generation == 0)
		return nfs_ok;

	if (in->si_generation == ref->si_generation)
		return nfs_ok;

	/* If the client sends us a stateid from the future, it's buggy: */
	if (stateid_generation_after(in, ref))
		return nfserr_bad_stateid;
	/*
	 * However, we could see a stateid from the past, even from a
	 * non-buggy client.  For example, if the client sends a lock
	 * while some IO is outstanding, the lock may bump si_generation
	 * while the IO is still in flight.  The client could avoid that
	 * situation by waiting for responses on all the IO requests,
	 * but better performance may result in retrying IO that
	 * receives an old_stateid error if requests are rarely
	 * reordered in flight:
	 */
	return nfserr_old_stateid;
}

<<<<<<< HEAD
__be32 nfs4_validate_stateid(struct nfs4_client *cl, stateid_t *stateid)
=======
static __be32 nfsd4_validate_stateid(struct nfs4_client *cl, stateid_t *stateid)
>>>>>>> refs/remotes/origin/master
{
	struct nfs4_stid *s;
	struct nfs4_ol_stateid *ols;
	__be32 status;

<<<<<<< HEAD
	if (STALE_STATEID(stateid))
		return nfserr_stale_stateid;

	s = find_stateid(cl, stateid);
	if (!s)
		 return nfserr_stale_stateid;
	status = check_stateid_generation(stateid, &s->sc_stateid, 1);
	if (status)
		return status;
	if (!(s->sc_type & (NFS4_OPEN_STID | NFS4_LOCK_STID)))
		return nfs_ok;
	ols = openlockstateid(s);
	if (ols->st_stateowner->so_is_open_owner
	    && !(openowner(ols->st_stateowner)->oo_flags & NFS4_OO_CONFIRMED))
		return nfserr_bad_stateid;
	return nfs_ok;
}

static __be32 nfsd4_lookup_stateid(stateid_t *stateid, unsigned char typemask, struct nfs4_stid **s)
{
	struct nfs4_client *cl;

	if (ZERO_STATEID(stateid) || ONE_STATEID(stateid))
		return nfserr_bad_stateid;
	if (STALE_STATEID(stateid))
		return nfserr_stale_stateid;
	cl = find_confirmed_client(&stateid->si_opaque.so_clid);
	if (!cl)
		return nfserr_expired;
=======
	if (ZERO_STATEID(stateid) || ONE_STATEID(stateid))
		return nfserr_bad_stateid;
	/* Client debugging aid. */
	if (!same_clid(&stateid->si_opaque.so_clid, &cl->cl_clientid)) {
		char addr_str[INET6_ADDRSTRLEN];
		rpc_ntop((struct sockaddr *)&cl->cl_addr, addr_str,
				 sizeof(addr_str));
		pr_warn_ratelimited("NFSD: client %s testing state ID "
					"with incorrect client ID\n", addr_str);
		return nfserr_bad_stateid;
	}
	s = find_stateid(cl, stateid);
	if (!s)
		return nfserr_bad_stateid;
	status = check_stateid_generation(stateid, &s->sc_stateid, 1);
	if (status)
		return status;
	switch (s->sc_type) {
	case NFS4_DELEG_STID:
		return nfs_ok;
	case NFS4_REVOKED_DELEG_STID:
		return nfserr_deleg_revoked;
	case NFS4_OPEN_STID:
	case NFS4_LOCK_STID:
		ols = openlockstateid(s);
		if (ols->st_stateowner->so_is_open_owner
	    			&& !(openowner(ols->st_stateowner)->oo_flags
						& NFS4_OO_CONFIRMED))
			return nfserr_bad_stateid;
		return nfs_ok;
	default:
		printk("unknown stateid type %x\n", s->sc_type);
	case NFS4_CLOSED_STID:
		return nfserr_bad_stateid;
	}
}

static __be32 nfsd4_lookup_stateid(stateid_t *stateid, unsigned char typemask,
				   struct nfs4_stid **s, bool sessions,
				   struct nfsd_net *nn)
{
	struct nfs4_client *cl;
	__be32 status;

	if (ZERO_STATEID(stateid) || ONE_STATEID(stateid))
		return nfserr_bad_stateid;
	status = lookup_clientid(&stateid->si_opaque.so_clid, sessions,
							nn, &cl);
	if (status == nfserr_stale_clientid)
		return nfserr_stale_stateid;
	if (status)
		return status;
>>>>>>> refs/remotes/origin/master
	*s = find_stateid_by_type(cl, stateid, typemask);
	if (!*s)
		return nfserr_bad_stateid;
	return nfs_ok;
<<<<<<< HEAD

>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

/*
* Checks for stateid operations
*/
__be32
<<<<<<< HEAD
nfs4_preprocess_stateid_op(struct nfsd4_compound_state *cstate,
			   stateid_t *stateid, int flags, struct file **filpp)
{
<<<<<<< HEAD
	struct nfs4_stateid *stp = NULL;
=======
	struct nfs4_stid *s;
	struct nfs4_ol_stateid *stp = NULL;
>>>>>>> refs/remotes/origin/cm-10.0
	struct nfs4_delegation *dp = NULL;
	struct svc_fh *current_fh = &cstate->current_fh;
	struct inode *ino = current_fh->fh_dentry->d_inode;
=======
nfs4_preprocess_stateid_op(struct net *net, struct nfsd4_compound_state *cstate,
			   stateid_t *stateid, int flags, struct file **filpp)
{
	struct nfs4_stid *s;
	struct nfs4_ol_stateid *stp = NULL;
	struct nfs4_delegation *dp = NULL;
	struct svc_fh *current_fh = &cstate->current_fh;
	struct inode *ino = current_fh->fh_dentry->d_inode;
	struct nfsd_net *nn = net_generic(net, nfsd_net_id);
>>>>>>> refs/remotes/origin/master
	__be32 status;

	if (filpp)
		*filpp = NULL;

<<<<<<< HEAD
	if (grace_disallows_io(ino))
		return nfserr_grace;

<<<<<<< HEAD
	if (nfsd4_has_session(cstate))
		flags |= HAS_SESSION;

	if (ZERO_STATEID(stateid) || ONE_STATEID(stateid))
		return check_special_stateids(current_fh, stateid, flags);

	status = nfserr_stale_stateid;
	if (STALE_STATEID(stateid)) 
		goto out;

	/*
	 * We assume that any stateid that has the current boot time,
	 * but that we can't find, is expired:
	 */
	status = nfserr_expired;
	if (is_delegation_stateid(stateid)) {
		dp = find_delegation_stateid(ino, stateid);
		if (!dp)
			goto out;
		status = check_stateid_generation(stateid, &dp->dl_stateid,
						  flags);
		if (status)
			goto out;
		status = nfs4_check_delegmode(dp, flags);
		if (status)
			goto out;
		renew_client(dp->dl_client);
=======
	if (ZERO_STATEID(stateid) || ONE_STATEID(stateid))
		return check_special_stateids(current_fh, stateid, flags);

	status = nfsd4_lookup_stateid(stateid, NFS4_DELEG_STID|NFS4_OPEN_STID|NFS4_LOCK_STID, &s);
=======
	if (grace_disallows_io(net, ino))
		return nfserr_grace;

	if (ZERO_STATEID(stateid) || ONE_STATEID(stateid))
		return check_special_stateids(net, current_fh, stateid, flags);

	status = nfsd4_lookup_stateid(stateid, NFS4_DELEG_STID|NFS4_OPEN_STID|NFS4_LOCK_STID,
				      &s, cstate->minorversion, nn);
>>>>>>> refs/remotes/origin/master
	if (status)
		return status;
	status = check_stateid_generation(stateid, &s->sc_stateid, nfsd4_has_session(cstate));
	if (status)
		goto out;
	switch (s->sc_type) {
	case NFS4_DELEG_STID:
		dp = delegstateid(s);
		status = nfs4_check_delegmode(dp, flags);
		if (status)
			goto out;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		if (filpp) {
			*filpp = dp->dl_file->fi_deleg_file;
			BUG_ON(!*filpp);
		}
<<<<<<< HEAD
	} else { /* open or lock stateid */
		stp = find_stateid(stateid, flags);
		if (!stp)
			goto out;
		status = nfserr_bad_stateid;
		if (nfs4_check_fh(current_fh, stp))
			goto out;
		if (!stp->st_stateowner->so_confirmed)
			goto out;
		status = check_stateid_generation(stateid, &stp->st_stateid,
						  flags);
		if (status)
			goto out;
		status = nfs4_check_openmode(stp, flags);
		if (status)
			goto out;
		renew_client(stp->st_stateowner->so_client);
=======
=======
		if (filpp) {
			*filpp = dp->dl_file->fi_deleg_file;
			if (!*filpp) {
				WARN_ON_ONCE(1);
				status = nfserr_serverfault;
				goto out;
			}
		}
>>>>>>> refs/remotes/origin/master
		break;
	case NFS4_OPEN_STID:
	case NFS4_LOCK_STID:
		stp = openlockstateid(s);
		status = nfs4_check_fh(current_fh, stp);
		if (status)
			goto out;
		if (stp->st_stateowner->so_is_open_owner
		    && !(openowner(stp->st_stateowner)->oo_flags & NFS4_OO_CONFIRMED))
			goto out;
		status = nfs4_check_openmode(stp, flags);
		if (status)
			goto out;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		if (filpp) {
			if (flags & RD_STATE)
				*filpp = find_readable_file(stp->st_file);
			else
				*filpp = find_writeable_file(stp->st_file);
		}
<<<<<<< HEAD
<<<<<<< HEAD
=======
		break;
	default:
		return nfserr_bad_stateid;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		break;
	default:
		return nfserr_bad_stateid;
>>>>>>> refs/remotes/origin/master
	}
	status = nfs_ok;
out:
	return status;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static __be32
nfsd4_free_lock_stateid(struct nfs4_ol_stateid *stp)
{
	if (check_for_locks(stp->st_file, lockowner(stp->st_stateowner)))
		return nfserr_locks_held;
	release_lock_stateid(stp);
	return nfs_ok;
}

/*
 * Test if the stateid is valid
 */
__be32
nfsd4_test_stateid(struct svc_rqst *rqstp, struct nfsd4_compound_state *cstate,
		   struct nfsd4_test_stateid *test_stateid)
{
	struct nfsd4_test_stateid_id *stateid;
	struct nfs4_client *cl = cstate->session->se_client;

	nfs4_lock_state();
	list_for_each_entry(stateid, &test_stateid->ts_stateid_list, ts_id_list)
<<<<<<< HEAD
		stateid->ts_id_status = nfs4_validate_stateid(cl, &stateid->ts_id_stateid);
=======
		stateid->ts_id_status =
			nfsd4_validate_stateid(cl, &stateid->ts_id_stateid);
>>>>>>> refs/remotes/origin/master
	nfs4_unlock_state();

	return nfs_ok;
}

__be32
nfsd4_free_stateid(struct svc_rqst *rqstp, struct nfsd4_compound_state *cstate,
		   struct nfsd4_free_stateid *free_stateid)
{
	stateid_t *stateid = &free_stateid->fr_stateid;
	struct nfs4_stid *s;
<<<<<<< HEAD
=======
	struct nfs4_delegation *dp;
>>>>>>> refs/remotes/origin/master
	struct nfs4_client *cl = cstate->session->se_client;
	__be32 ret = nfserr_bad_stateid;

	nfs4_lock_state();
	s = find_stateid(cl, stateid);
	if (!s)
		goto out;
	switch (s->sc_type) {
	case NFS4_DELEG_STID:
		ret = nfserr_locks_held;
		goto out;
	case NFS4_OPEN_STID:
	case NFS4_LOCK_STID:
		ret = check_stateid_generation(stateid, &s->sc_stateid, 1);
		if (ret)
			goto out;
		if (s->sc_type == NFS4_LOCK_STID)
			ret = nfsd4_free_lock_stateid(openlockstateid(s));
		else
			ret = nfserr_locks_held;
		break;
<<<<<<< HEAD
=======
	case NFS4_REVOKED_DELEG_STID:
		dp = delegstateid(s);
		destroy_revoked_delegation(dp);
		ret = nfs_ok;
		break;
>>>>>>> refs/remotes/origin/master
	default:
		ret = nfserr_bad_stateid;
	}
out:
	nfs4_unlock_state();
	return ret;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static inline int
setlkflg (int type)
{
	return (type == NFS4_READW_LT || type == NFS4_READ_LT) ?
		RD_STATE : WR_STATE;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static __be32 nfs4_seqid_op_checks(struct nfsd4_compound_state *cstate, stateid_t *stateid, u32 seqid, struct nfs4_ol_stateid *stp)
{
	struct svc_fh *current_fh = &cstate->current_fh;
	struct nfs4_stateowner *sop = stp->st_stateowner;
	__be32 status;

	status = nfsd4_check_seqid(cstate, sop, seqid);
	if (status)
		return status;
<<<<<<< HEAD
	if (stp->st_stid.sc_type == NFS4_CLOSED_STID)
		/*
		 * "Closed" stateid's exist *only* to return
		 * nfserr_replay_me from the previous step.
=======
	if (stp->st_stid.sc_type == NFS4_CLOSED_STID
		|| stp->st_stid.sc_type == NFS4_REVOKED_DELEG_STID)
		/*
		 * "Closed" stateid's exist *only* to return
		 * nfserr_replay_me from the previous step, and
		 * revoked delegations are kept only for free_stateid.
>>>>>>> refs/remotes/origin/master
		 */
		return nfserr_bad_stateid;
	status = check_stateid_generation(stateid, &stp->st_stid.sc_stateid, nfsd4_has_session(cstate));
	if (status)
		return status;
	return nfs4_check_fh(current_fh, stp);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/* 
 * Checks for sequence id mutating operations. 
 */
static __be32
nfs4_preprocess_seqid_op(struct nfsd4_compound_state *cstate, u32 seqid,
<<<<<<< HEAD
<<<<<<< HEAD
			 stateid_t *stateid, int flags,
			 struct nfs4_stateowner **sopp,
			 struct nfs4_stateid **stpp, struct nfsd4_lock *lock)
{
	struct nfs4_stateid *stp;
	struct nfs4_stateowner *sop;
	struct svc_fh *current_fh = &cstate->current_fh;
	__be32 status;
=======
			 stateid_t *stateid, char typemask,
			 struct nfs4_ol_stateid **stpp)
{
	__be32 status;
	struct nfs4_stid *s;
>>>>>>> refs/remotes/origin/cm-10.0
=======
			 stateid_t *stateid, char typemask,
			 struct nfs4_ol_stateid **stpp,
			 struct nfsd_net *nn)
{
	__be32 status;
	struct nfs4_stid *s;
>>>>>>> refs/remotes/origin/master

	dprintk("NFSD: %s: seqid=%d stateid = " STATEID_FMT "\n", __func__,
		seqid, STATEID_VAL(stateid));

	*stpp = NULL;
<<<<<<< HEAD
<<<<<<< HEAD
	*sopp = NULL;

	if (ZERO_STATEID(stateid) || ONE_STATEID(stateid)) {
		dprintk("NFSD: preprocess_seqid_op: magic stateid!\n");
		return nfserr_bad_stateid;
	}

	if (STALE_STATEID(stateid))
		return nfserr_stale_stateid;

	if (nfsd4_has_session(cstate))
		flags |= HAS_SESSION;

	/*
	* We return BAD_STATEID if filehandle doesn't match stateid, 
	* the confirmed flag is incorrecly set, or the generation 
	* number is incorrect.  
	*/
	stp = find_stateid(stateid, flags);
	if (stp == NULL) {
		/*
		 * Also, we should make sure this isn't just the result of
		 * a replayed close:
		 */
		sop = search_close_lru(stateid->si_stateownerid, flags);
		/* It's not stale; let's assume it's expired: */
		if (sop == NULL)
			return nfserr_expired;
		*sopp = sop;
		goto check_replay;
	}

	*stpp = stp;
	*sopp = sop = stp->st_stateowner;

	if (lock) {
		clientid_t *lockclid = &lock->v.new.clientid;
		struct nfs4_client *clp = sop->so_client;
		int lkflg = 0;
		__be32 status;

		lkflg = setlkflg(lock->lk_type);

		if (lock->lk_is_new) {
			if (!sop->so_is_open_owner)
				return nfserr_bad_stateid;
			if (!(flags & HAS_SESSION) &&
			    !same_clid(&clp->cl_clientid, lockclid))
				return nfserr_bad_stateid;
			/* stp is the open stateid */
			status = nfs4_check_openmode(stp, lkflg);
			if (status)
				return status;
		} else {
			/* stp is the lock stateid */
			status = nfs4_check_openmode(stp->st_openstp, lkflg);
			if (status)
				return status;
               }
	}

	if (nfs4_check_fh(current_fh, stp)) {
		dprintk("NFSD: preprocess_seqid_op: fh-stateid mismatch!\n");
		return nfserr_bad_stateid;
	}

	/*
	*  We now validate the seqid and stateid generation numbers.
	*  For the moment, we ignore the possibility of 
	*  generation number wraparound.
	*/
	if (!(flags & HAS_SESSION) && seqid != sop->so_seqid)
		goto check_replay;

	if (sop->so_confirmed && flags & CONFIRM) {
		dprintk("NFSD: preprocess_seqid_op: expected"
				" unconfirmed stateowner!\n");
		return nfserr_bad_stateid;
	}
	if (!sop->so_confirmed && !(flags & CONFIRM)) {
		dprintk("NFSD: preprocess_seqid_op: stateowner not"
				" confirmed yet!\n");
		return nfserr_bad_stateid;
	}
	status = check_stateid_generation(stateid, &stp->st_stateid, flags);
	if (status)
		return status;
	renew_client(sop->so_client);
	return nfs_ok;

check_replay:
	if (seqid == sop->so_seqid - 1) {
		dprintk("NFSD: preprocess_seqid_op: retransmission?\n");
		/* indicate replay to calling function */
		return nfserr_replay_me;
	}
	dprintk("NFSD: preprocess_seqid_op: bad seqid (expected %d, got %d)\n",
			sop->so_seqid, seqid);
	*sopp = NULL;
	return nfserr_bad_seqid;
=======
	status = nfsd4_lookup_stateid(stateid, typemask, &s);
	if (status)
		return status;
	*stpp = openlockstateid(s);
	cstate->replay_owner = (*stpp)->st_stateowner;
=======
	status = nfsd4_lookup_stateid(stateid, typemask, &s,
				      cstate->minorversion, nn);
	if (status)
		return status;
	*stpp = openlockstateid(s);
	if (!nfsd4_has_session(cstate))
		cstate->replay_owner = (*stpp)->st_stateowner;
>>>>>>> refs/remotes/origin/master

	return nfs4_seqid_op_checks(cstate, stateid, seqid, *stpp);
}

<<<<<<< HEAD
static __be32 nfs4_preprocess_confirmed_seqid_op(struct nfsd4_compound_state *cstate, u32 seqid, stateid_t *stateid, struct nfs4_ol_stateid **stpp)
=======
static __be32 nfs4_preprocess_confirmed_seqid_op(struct nfsd4_compound_state *cstate, u32 seqid,
						 stateid_t *stateid, struct nfs4_ol_stateid **stpp, struct nfsd_net *nn)
>>>>>>> refs/remotes/origin/master
{
	__be32 status;
	struct nfs4_openowner *oo;

	status = nfs4_preprocess_seqid_op(cstate, seqid, stateid,
<<<<<<< HEAD
						NFS4_OPEN_STID, stpp);
=======
						NFS4_OPEN_STID, stpp, nn);
>>>>>>> refs/remotes/origin/master
	if (status)
		return status;
	oo = openowner((*stpp)->st_stateowner);
	if (!(oo->oo_flags & NFS4_OO_CONFIRMED))
		return nfserr_bad_stateid;
	return nfs_ok;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

__be32
nfsd4_open_confirm(struct svc_rqst *rqstp, struct nfsd4_compound_state *cstate,
		   struct nfsd4_open_confirm *oc)
{
	__be32 status;
<<<<<<< HEAD
<<<<<<< HEAD
	struct nfs4_stateowner *sop;
	struct nfs4_stateid *stp;
=======
	struct nfs4_openowner *oo;
	struct nfs4_ol_stateid *stp;
>>>>>>> refs/remotes/origin/cm-10.0

	dprintk("NFSD: nfsd4_open_confirm on file %.*s\n",
			(int)cstate->current_fh.fh_dentry->d_name.len,
			cstate->current_fh.fh_dentry->d_name.name);
=======
	struct nfs4_openowner *oo;
	struct nfs4_ol_stateid *stp;
	struct nfsd_net *nn = net_generic(SVC_NET(rqstp), nfsd_net_id);

	dprintk("NFSD: nfsd4_open_confirm on file %pd\n",
			cstate->current_fh.fh_dentry);
>>>>>>> refs/remotes/origin/master

	status = fh_verify(rqstp, &cstate->current_fh, S_IFREG, 0);
	if (status)
		return status;

	nfs4_lock_state();

<<<<<<< HEAD
<<<<<<< HEAD
	if ((status = nfs4_preprocess_seqid_op(cstate,
					oc->oc_seqid, &oc->oc_req_stateid,
					CONFIRM | OPEN_STATE,
					&oc->oc_stateowner, &stp, NULL)))
		goto out; 

	sop = oc->oc_stateowner;
	sop->so_confirmed = 1;
	update_stateid(&stp->st_stateid);
	memcpy(&oc->oc_resp_stateid, &stp->st_stateid, sizeof(stateid_t));
	dprintk("NFSD: %s: success, seqid=%d stateid=" STATEID_FMT "\n",
		__func__, oc->oc_seqid, STATEID_VAL(&stp->st_stateid));

	nfsd4_create_clid_dir(sop->so_client);
out:
	if (oc->oc_stateowner) {
		nfs4_get_stateowner(oc->oc_stateowner);
		cstate->replay_owner = oc->oc_stateowner;
	}
	nfs4_unlock_state();
	return status;
}

static inline void nfs4_file_downgrade(struct nfs4_stateid *stp, unsigned int to_access)
{
	int i;

	for (i = 1; i < 4; i++) {
		if (test_bit(i, &stp->st_access_bmap)
					&& ((i & to_access) != i)) {
			nfs4_file_put_access(stp->st_file, nfs4_access_to_omode(i));
			__clear_bit(i, &stp->st_access_bmap);
		}
=======
	status = nfs4_preprocess_seqid_op(cstate,
					oc->oc_seqid, &oc->oc_req_stateid,
					NFS4_OPEN_STID, &stp);
=======
	status = nfs4_preprocess_seqid_op(cstate,
					oc->oc_seqid, &oc->oc_req_stateid,
					NFS4_OPEN_STID, &stp, nn);
>>>>>>> refs/remotes/origin/master
	if (status)
		goto out;
	oo = openowner(stp->st_stateowner);
	status = nfserr_bad_stateid;
	if (oo->oo_flags & NFS4_OO_CONFIRMED)
		goto out;
	oo->oo_flags |= NFS4_OO_CONFIRMED;
	update_stateid(&stp->st_stid.sc_stateid);
	memcpy(&oc->oc_resp_stateid, &stp->st_stid.sc_stateid, sizeof(stateid_t));
	dprintk("NFSD: %s: success, seqid=%d stateid=" STATEID_FMT "\n",
		__func__, oc->oc_seqid, STATEID_VAL(&stp->st_stid.sc_stateid));

	nfsd4_client_record_create(oo->oo_owner.so_client);
	status = nfs_ok;
out:
<<<<<<< HEAD
=======
	nfsd4_bump_seqid(cstate, status);
>>>>>>> refs/remotes/origin/master
	if (!cstate->replay_owner)
		nfs4_unlock_state();
	return status;
}

static inline void nfs4_stateid_downgrade_bit(struct nfs4_ol_stateid *stp, u32 access)
{
<<<<<<< HEAD
	if (!test_bit(access, &stp->st_access_bmap))
		return;
	nfs4_file_put_access(stp->st_file, nfs4_access_to_omode(access));
	__clear_bit(access, &stp->st_access_bmap);
=======
	if (!test_access(access, stp))
		return;
	nfs4_file_put_access(stp->st_file, nfs4_access_to_omode(access));
	clear_access(access, stp);
>>>>>>> refs/remotes/origin/master
}

static inline void nfs4_stateid_downgrade(struct nfs4_ol_stateid *stp, u32 to_access)
{
	switch (to_access) {
	case NFS4_SHARE_ACCESS_READ:
		nfs4_stateid_downgrade_bit(stp, NFS4_SHARE_ACCESS_WRITE);
		nfs4_stateid_downgrade_bit(stp, NFS4_SHARE_ACCESS_BOTH);
		break;
	case NFS4_SHARE_ACCESS_WRITE:
		nfs4_stateid_downgrade_bit(stp, NFS4_SHARE_ACCESS_READ);
		nfs4_stateid_downgrade_bit(stp, NFS4_SHARE_ACCESS_BOTH);
		break;
	case NFS4_SHARE_ACCESS_BOTH:
		break;
	default:
<<<<<<< HEAD
		BUG();
>>>>>>> refs/remotes/origin/cm-10.0
=======
		WARN_ON_ONCE(1);
>>>>>>> refs/remotes/origin/master
	}
}

static void
<<<<<<< HEAD
reset_union_bmap_deny(unsigned long deny, unsigned long *bmap)
=======
reset_union_bmap_deny(unsigned long deny, struct nfs4_ol_stateid *stp)
>>>>>>> refs/remotes/origin/master
{
	int i;
	for (i = 0; i < 4; i++) {
		if ((i & deny) != i)
<<<<<<< HEAD
			__clear_bit(i, bmap);
=======
			clear_deny(i, stp);
>>>>>>> refs/remotes/origin/master
	}
}

__be32
nfsd4_open_downgrade(struct svc_rqst *rqstp,
		     struct nfsd4_compound_state *cstate,
		     struct nfsd4_open_downgrade *od)
{
	__be32 status;
<<<<<<< HEAD
<<<<<<< HEAD
	struct nfs4_stateid *stp;
=======
	struct nfs4_ol_stateid *stp;
>>>>>>> refs/remotes/origin/cm-10.0

	dprintk("NFSD: nfsd4_open_downgrade on file %.*s\n", 
			(int)cstate->current_fh.fh_dentry->d_name.len,
			cstate->current_fh.fh_dentry->d_name.name);

<<<<<<< HEAD
	if (!access_valid(od->od_share_access, cstate->minorversion)
			|| !deny_valid(od->od_share_deny))
		return nfserr_inval;
	/* We don't yet support WANT bits: */
	od->od_share_access &= NFS4_SHARE_ACCESS_MASK;

	nfs4_lock_state();
	if ((status = nfs4_preprocess_seqid_op(cstate,
					od->od_seqid,
					&od->od_stateid, 
					OPEN_STATE,
					&od->od_stateowner, &stp, NULL)))
		goto out; 

=======
=======
	struct nfs4_ol_stateid *stp;
	struct nfsd_net *nn = net_generic(SVC_NET(rqstp), nfsd_net_id);

	dprintk("NFSD: nfsd4_open_downgrade on file %pd\n", 
			cstate->current_fh.fh_dentry);

>>>>>>> refs/remotes/origin/master
	/* We don't yet support WANT bits: */
	if (od->od_deleg_want)
		dprintk("NFSD: %s: od_deleg_want=0x%x ignored\n", __func__,
			od->od_deleg_want);

	nfs4_lock_state();
	status = nfs4_preprocess_confirmed_seqid_op(cstate, od->od_seqid,
<<<<<<< HEAD
					&od->od_stateid, &stp);
	if (status)
		goto out; 
>>>>>>> refs/remotes/origin/cm-10.0
	status = nfserr_inval;
	if (!test_bit(od->od_share_access, &stp->st_access_bmap)) {
		dprintk("NFSD:access not a subset current bitmap: 0x%lx, input access=%08x\n",
			stp->st_access_bmap, od->od_share_access);
		goto out;
	}
	if (!test_bit(od->od_share_deny, &stp->st_deny_bmap)) {
=======
					&od->od_stateid, &stp, nn);
	if (status)
		goto out; 
	status = nfserr_inval;
	if (!test_access(od->od_share_access, stp)) {
		dprintk("NFSD: access not a subset current bitmap: 0x%lx, input access=%08x\n",
			stp->st_access_bmap, od->od_share_access);
		goto out;
	}
	if (!test_deny(od->od_share_deny, stp)) {
>>>>>>> refs/remotes/origin/master
		dprintk("NFSD:deny not a subset current bitmap: 0x%lx, input deny=%08x\n",
			stp->st_deny_bmap, od->od_share_deny);
		goto out;
	}
<<<<<<< HEAD
<<<<<<< HEAD
	nfs4_file_downgrade(stp, od->od_share_access);

	reset_union_bmap_deny(od->od_share_deny, &stp->st_deny_bmap);

	update_stateid(&stp->st_stateid);
	memcpy(&od->od_stateid, &stp->st_stateid, sizeof(stateid_t));
	status = nfs_ok;
out:
	if (od->od_stateowner) {
		nfs4_get_stateowner(od->od_stateowner);
		cstate->replay_owner = od->od_stateowner;
	}
	nfs4_unlock_state();
	return status;
}

=======
	nfs4_stateid_downgrade(stp, od->od_share_access);

	reset_union_bmap_deny(od->od_share_deny, &stp->st_deny_bmap);
=======
	nfs4_stateid_downgrade(stp, od->od_share_access);

	reset_union_bmap_deny(od->od_share_deny, stp);
>>>>>>> refs/remotes/origin/master

	update_stateid(&stp->st_stid.sc_stateid);
	memcpy(&od->od_stateid, &stp->st_stid.sc_stateid, sizeof(stateid_t));
	status = nfs_ok;
out:
<<<<<<< HEAD
=======
	nfsd4_bump_seqid(cstate, status);
>>>>>>> refs/remotes/origin/master
	if (!cstate->replay_owner)
		nfs4_unlock_state();
	return status;
}

<<<<<<< HEAD
void nfsd4_purge_closed_stateid(struct nfs4_stateowner *so)
{
	struct nfs4_openowner *oo;
	struct nfs4_ol_stateid *s;

	if (!so->so_is_open_owner)
		return;
	oo = openowner(so);
	s = oo->oo_last_closed_stid;
	if (!s)
		return;
	if (!(oo->oo_flags & NFS4_OO_PURGE_CLOSE)) {
		/* Release the last_closed_stid on the next seqid bump: */
		oo->oo_flags |= NFS4_OO_PURGE_CLOSE;
		return;
	}
	oo->oo_flags &= ~NFS4_OO_PURGE_CLOSE;
	release_last_closed_stateid(oo);
}

=======
>>>>>>> refs/remotes/origin/master
static void nfsd4_close_open_stateid(struct nfs4_ol_stateid *s)
{
	unhash_open_stateid(s);
	s->st_stid.sc_type = NFS4_CLOSED_STID;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * nfs4_unlock_state() called after encode
 */
__be32
nfsd4_close(struct svc_rqst *rqstp, struct nfsd4_compound_state *cstate,
	    struct nfsd4_close *close)
{
	__be32 status;
<<<<<<< HEAD
<<<<<<< HEAD
	struct nfs4_stateid *stp;
=======
	struct nfs4_openowner *oo;
	struct nfs4_ol_stateid *stp;
>>>>>>> refs/remotes/origin/cm-10.0

	dprintk("NFSD: nfsd4_close on file %.*s\n", 
			(int)cstate->current_fh.fh_dentry->d_name.len,
			cstate->current_fh.fh_dentry->d_name.name);

	nfs4_lock_state();
<<<<<<< HEAD
	/* check close_lru for replay */
	if ((status = nfs4_preprocess_seqid_op(cstate,
					close->cl_seqid,
					&close->cl_stateid, 
					OPEN_STATE | CLOSE_STATE,
					&close->cl_stateowner, &stp, NULL)))
		goto out; 
	status = nfs_ok;
	update_stateid(&stp->st_stateid);
	memcpy(&close->cl_stateid, &stp->st_stateid, sizeof(stateid_t));

	/* release_stateid() calls nfsd_close() if needed */
	release_open_stateid(stp);
=======
	status = nfs4_preprocess_seqid_op(cstate, close->cl_seqid,
					&close->cl_stateid,
					NFS4_OPEN_STID|NFS4_CLOSED_STID,
					&stp);
	if (status)
		goto out; 
	oo = openowner(stp->st_stateowner);
	status = nfs_ok;
=======
	struct nfs4_openowner *oo;
	struct nfs4_ol_stateid *stp;
	struct net *net = SVC_NET(rqstp);
	struct nfsd_net *nn = net_generic(net, nfsd_net_id);

	dprintk("NFSD: nfsd4_close on file %pd\n", 
			cstate->current_fh.fh_dentry);

	nfs4_lock_state();
	status = nfs4_preprocess_seqid_op(cstate, close->cl_seqid,
					&close->cl_stateid,
					NFS4_OPEN_STID|NFS4_CLOSED_STID,
					&stp, nn);
	nfsd4_bump_seqid(cstate, status);
	if (status)
		goto out; 
	oo = openowner(stp->st_stateowner);
>>>>>>> refs/remotes/origin/master
	update_stateid(&stp->st_stid.sc_stateid);
	memcpy(&close->cl_stateid, &stp->st_stid.sc_stateid, sizeof(stateid_t));

	nfsd4_close_open_stateid(stp);
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	release_last_closed_stateid(oo);
	oo->oo_last_closed_stid = stp;
>>>>>>> refs/remotes/origin/cm-10.0

	/* place unused nfs4_stateowners on so_close_lru list to be
	 * released by the laundromat service after the lease period
	 * to enable us to handle CLOSE replay
	 */
<<<<<<< HEAD
	if (list_empty(&close->cl_stateowner->so_stateids))
		move_to_close_lru(close->cl_stateowner);
out:
	if (close->cl_stateowner) {
		nfs4_get_stateowner(close->cl_stateowner);
		cstate->replay_owner = close->cl_stateowner;
	}
	nfs4_unlock_state();
=======
	if (list_empty(&oo->oo_owner.so_stateids))
		move_to_close_lru(oo);
out:
	if (!cstate->replay_owner)
		nfs4_unlock_state();
>>>>>>> refs/remotes/origin/cm-10.0
=======

	if (cstate->minorversion)
		free_generic_stateid(stp);
	else
		oo->oo_last_closed_stid = stp;

	if (list_empty(&oo->oo_owner.so_stateids)) {
		if (cstate->minorversion)
			release_openowner(oo);
		else {
			/*
			 * In the 4.0 case we need to keep the owners around a
			 * little while to handle CLOSE replay.
			 */
			move_to_close_lru(oo, SVC_NET(rqstp));
		}
	}
out:
	if (!cstate->replay_owner)
		nfs4_unlock_state();
>>>>>>> refs/remotes/origin/master
	return status;
}

__be32
nfsd4_delegreturn(struct svc_rqst *rqstp, struct nfsd4_compound_state *cstate,
		  struct nfsd4_delegreturn *dr)
{
	struct nfs4_delegation *dp;
	stateid_t *stateid = &dr->dr_stateid;
<<<<<<< HEAD
<<<<<<< HEAD
	struct inode *inode;
	__be32 status;
	int flags = 0;
=======
	struct nfs4_stid *s;
	struct inode *inode;
	__be32 status;
>>>>>>> refs/remotes/origin/cm-10.0

	if ((status = fh_verify(rqstp, &cstate->current_fh, S_IFREG, 0)))
		return status;
	inode = cstate->current_fh.fh_dentry->d_inode;

<<<<<<< HEAD
	if (nfsd4_has_session(cstate))
		flags |= HAS_SESSION;
	nfs4_lock_state();
	status = nfserr_bad_stateid;
	if (ZERO_STATEID(stateid) || ONE_STATEID(stateid))
		goto out;
	status = nfserr_stale_stateid;
	if (STALE_STATEID(stateid))
		goto out;
	status = nfserr_bad_stateid;
	if (!is_delegation_stateid(stateid))
		goto out;
	status = nfserr_expired;
	dp = find_delegation_stateid(inode, stateid);
	if (!dp)
		goto out;
	status = check_stateid_generation(stateid, &dp->dl_stateid, flags);
	if (status)
		goto out;
	renew_client(dp->dl_client);
=======
	nfs4_lock_state();
	status = nfsd4_lookup_stateid(stateid, NFS4_DELEG_STID, &s);
=======
	struct nfs4_stid *s;
	__be32 status;
	struct nfsd_net *nn = net_generic(SVC_NET(rqstp), nfsd_net_id);

	if ((status = fh_verify(rqstp, &cstate->current_fh, S_IFREG, 0)))
		return status;

	nfs4_lock_state();
	status = nfsd4_lookup_stateid(stateid, NFS4_DELEG_STID, &s,
				      cstate->minorversion, nn);
>>>>>>> refs/remotes/origin/master
	if (status)
		goto out;
	dp = delegstateid(s);
	status = check_stateid_generation(stateid, &dp->dl_stid.sc_stateid, nfsd4_has_session(cstate));
	if (status)
		goto out;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

	unhash_delegation(dp);
=======

	destroy_delegation(dp);
>>>>>>> refs/remotes/origin/master
out:
	nfs4_unlock_state();

	return status;
}


<<<<<<< HEAD
<<<<<<< HEAD
/* 
 * Lock owner state (byte-range locks)
 */
#define LOFF_OVERFLOW(start, len)      ((u64)(len) > ~(u64)(start))
#define LOCK_HASH_BITS              8
#define LOCK_HASH_SIZE             (1 << LOCK_HASH_BITS)
#define LOCK_HASH_MASK             (LOCK_HASH_SIZE - 1)
=======
#define LOFF_OVERFLOW(start, len)      ((u64)(len) > ~(u64)(start))

#define LOCKOWNER_INO_HASH_BITS 8
#define LOCKOWNER_INO_HASH_SIZE (1 << LOCKOWNER_INO_HASH_BITS)
#define LOCKOWNER_INO_HASH_MASK (LOCKOWNER_INO_HASH_SIZE - 1)
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define LOFF_OVERFLOW(start, len)      ((u64)(len) > ~(u64)(start))

#define LOCKOWNER_INO_HASH_MASK (LOCKOWNER_INO_HASH_SIZE - 1)
>>>>>>> refs/remotes/origin/master

static inline u64
end_offset(u64 start, u64 len)
{
	u64 end;

	end = start + len;
	return end >= start ? end: NFS4_MAX_UINT64;
}

/* last octet in a range */
static inline u64
last_byte_offset(u64 start, u64 len)
{
	u64 end;

<<<<<<< HEAD
	BUG_ON(!len);
=======
	WARN_ON_ONCE(!len);
>>>>>>> refs/remotes/origin/master
	end = start + len;
	return end > start ? end - 1: NFS4_MAX_UINT64;
}

<<<<<<< HEAD
<<<<<<< HEAD
#define lockownerid_hashval(id) \
        ((id) & LOCK_HASH_MASK)

static inline unsigned int
lock_ownerstr_hashval(struct inode *inode, u32 cl_id,
		struct xdr_netobj *ownername)
{
	return (file_hashval(inode) + cl_id
			+ opaque_hashval(ownername->data, ownername->len))
		& LOCK_HASH_MASK;
}

static struct list_head lock_ownerid_hashtbl[LOCK_HASH_SIZE];
static struct list_head	lock_ownerstr_hashtbl[LOCK_HASH_SIZE];
static struct list_head lockstateid_hashtbl[STATEID_HASH_SIZE];

static struct nfs4_stateid *
find_stateid(stateid_t *stid, int flags)
{
	struct nfs4_stateid *local;
	u32 st_id = stid->si_stateownerid;
	u32 f_id = stid->si_fileid;
	unsigned int hashval;

	dprintk("NFSD: find_stateid flags 0x%x\n",flags);
	if (flags & (LOCK_STATE | RD_STATE | WR_STATE)) {
		hashval = stateid_hashval(st_id, f_id);
		list_for_each_entry(local, &lockstateid_hashtbl[hashval], st_hash) {
			if ((local->st_stateid.si_stateownerid == st_id) &&
			    (local->st_stateid.si_fileid == f_id))
				return local;
		}
	} 

	if (flags & (OPEN_STATE | RD_STATE | WR_STATE)) {
		hashval = stateid_hashval(st_id, f_id);
		list_for_each_entry(local, &stateid_hashtbl[hashval], st_hash) {
			if ((local->st_stateid.si_stateownerid == st_id) &&
			    (local->st_stateid.si_fileid == f_id))
				return local;
		}
	}
	return NULL;
}

static struct nfs4_delegation *
find_delegation_stateid(struct inode *ino, stateid_t *stid)
{
	struct nfs4_file *fp;
	struct nfs4_delegation *dl;

	dprintk("NFSD: %s: stateid=" STATEID_FMT "\n", __func__,
		STATEID_VAL(stid));

	fp = find_file(ino);
	if (!fp)
		return NULL;
	dl = find_delegation_file(fp, stid);
	put_nfs4_file(fp);
	return dl;
}
=======
=======
>>>>>>> refs/remotes/origin/master
static unsigned int lockowner_ino_hashval(struct inode *inode, u32 cl_id, struct xdr_netobj *ownername)
{
	return (file_hashval(inode) + cl_id
			+ opaque_hashval(ownername->data, ownername->len))
		& LOCKOWNER_INO_HASH_MASK;
}

<<<<<<< HEAD
static struct list_head lockowner_ino_hashtbl[LOCKOWNER_INO_HASH_SIZE];
>>>>>>> refs/remotes/origin/cm-10.0

=======
>>>>>>> refs/remotes/origin/master
/*
 * TODO: Linux file offsets are _signed_ 64-bit quantities, which means that
 * we can't properly handle lock requests that go beyond the (2^63 - 1)-th
 * byte, because of sign extension problems.  Since NFSv4 calls for 64-bit
 * locking, this prevents us from being completely protocol-compliant.  The
 * real solution to this problem is to start using unsigned file offsets in
 * the VFS, but this is a very deep change!
 */
static inline void
nfs4_transform_lock_offset(struct file_lock *lock)
{
	if (lock->fl_start < 0)
		lock->fl_start = OFFSET_MAX;
	if (lock->fl_end < 0)
		lock->fl_end = OFFSET_MAX;
}

/* Hack!: For now, we're defining this just so we can use a pointer to it
 * as a unique cookie to identify our (NFSv4's) posix locks. */
static const struct lock_manager_operations nfsd_posix_mng_ops  = {
};

static inline void
nfs4_set_lock_denied(struct file_lock *fl, struct nfsd4_lock_denied *deny)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct nfs4_stateowner *sop;

	if (fl->fl_lmops == &nfsd_posix_mng_ops) {
		sop = (struct nfs4_stateowner *) fl->fl_owner;
		kref_get(&sop->so_ref);
		deny->ld_sop = sop;
		deny->ld_clientid = sop->so_client->cl_clientid;
	} else {
		deny->ld_sop = NULL;
=======
=======
>>>>>>> refs/remotes/origin/master
	struct nfs4_lockowner *lo;

	if (fl->fl_lmops == &nfsd_posix_mng_ops) {
		lo = (struct nfs4_lockowner *) fl->fl_owner;
		deny->ld_owner.data = kmemdup(lo->lo_owner.so_owner.data,
					lo->lo_owner.so_owner.len, GFP_KERNEL);
		if (!deny->ld_owner.data)
			/* We just don't care that much */
			goto nevermind;
		deny->ld_owner.len = lo->lo_owner.so_owner.len;
		deny->ld_clientid = lo->lo_owner.so_client->cl_clientid;
	} else {
nevermind:
		deny->ld_owner.len = 0;
		deny->ld_owner.data = NULL;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		deny->ld_clientid.cl_boot = 0;
		deny->ld_clientid.cl_id = 0;
	}
	deny->ld_start = fl->fl_start;
	deny->ld_length = NFS4_MAX_UINT64;
	if (fl->fl_end != NFS4_MAX_UINT64)
		deny->ld_length = fl->fl_end - fl->fl_start + 1;        
	deny->ld_type = NFS4_READ_LT;
	if (fl->fl_type != F_RDLCK)
		deny->ld_type = NFS4_WRITE_LT;
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct nfs4_stateowner *
find_lockstateowner_str(struct inode *inode, clientid_t *clid,
		struct xdr_netobj *owner)
{
	unsigned int hashval = lock_ownerstr_hashval(inode, clid->cl_id, owner);
	struct nfs4_stateowner *op;

	list_for_each_entry(op, &lock_ownerstr_hashtbl[hashval], so_strhash) {
		if (same_owner_str(op, owner, clid))
			return op;
=======
=======
>>>>>>> refs/remotes/origin/master
static bool same_lockowner_ino(struct nfs4_lockowner *lo, struct inode *inode, clientid_t *clid, struct xdr_netobj *owner)
{
	struct nfs4_ol_stateid *lst;

	if (!same_owner_str(&lo->lo_owner, owner, clid))
		return false;
	lst = list_first_entry(&lo->lo_owner.so_stateids,
			       struct nfs4_ol_stateid, st_perstateowner);
	return lst->st_file->fi_inode == inode;
}

static struct nfs4_lockowner *
find_lockowner_str(struct inode *inode, clientid_t *clid,
<<<<<<< HEAD
		struct xdr_netobj *owner)
=======
		   struct xdr_netobj *owner, struct nfsd_net *nn)
>>>>>>> refs/remotes/origin/master
{
	unsigned int hashval = lockowner_ino_hashval(inode, clid->cl_id, owner);
	struct nfs4_lockowner *lo;

<<<<<<< HEAD
	list_for_each_entry(lo, &lockowner_ino_hashtbl[hashval], lo_owner_ino_hash) {
		if (same_lockowner_ino(lo, inode, clid, owner))
			return lo;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	list_for_each_entry(lo, &nn->lockowner_ino_hashtbl[hashval], lo_owner_ino_hash) {
		if (same_lockowner_ino(lo, inode, clid, owner))
			return lo;
>>>>>>> refs/remotes/origin/master
	}
	return NULL;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static void hash_lockowner(struct nfs4_lockowner *lo, unsigned int strhashval, struct nfs4_client *clp, struct nfs4_ol_stateid *open_stp)
{
	struct inode *inode = open_stp->st_file->fi_inode;
	unsigned int inohash = lockowner_ino_hashval(inode,
			clp->cl_clientid.cl_id, &lo->lo_owner.so_owner);
<<<<<<< HEAD

	list_add(&lo->lo_owner.so_strhash, &ownerstr_hashtbl[strhashval]);
	list_add(&lo->lo_owner_ino_hash, &lockowner_ino_hashtbl[inohash]);
	list_add(&lo->lo_perstateid, &open_stp->st_lockowners);
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct nfsd_net *nn = net_generic(clp->net, nfsd_net_id);

	list_add(&lo->lo_owner.so_strhash, &nn->ownerstr_hashtbl[strhashval]);
	list_add(&lo->lo_owner_ino_hash, &nn->lockowner_ino_hashtbl[inohash]);
	list_add(&lo->lo_perstateid, &open_stp->st_lockowners);
}

>>>>>>> refs/remotes/origin/master
/*
 * Alloc a lock owner structure.
 * Called in nfsd4_lock - therefore, OPEN and OPEN_CONFIRM (if needed) has 
 * occurred. 
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * strhashval = lock_ownerstr_hashval 
 */

static struct nfs4_stateowner *
alloc_init_lock_stateowner(unsigned int strhashval, struct nfs4_client *clp, struct nfs4_stateid *open_stp, struct nfsd4_lock *lock) {
	struct nfs4_stateowner *sop;
	struct nfs4_replay *rp;
	unsigned int idhashval;

	if (!(sop = alloc_stateowner(&lock->lk_new_owner)))
		return NULL;
	idhashval = lockownerid_hashval(current_ownerid);
	INIT_LIST_HEAD(&sop->so_idhash);
	INIT_LIST_HEAD(&sop->so_strhash);
	INIT_LIST_HEAD(&sop->so_perclient);
	INIT_LIST_HEAD(&sop->so_stateids);
	INIT_LIST_HEAD(&sop->so_perstateid);
	INIT_LIST_HEAD(&sop->so_close_lru); /* not used */
	sop->so_time = 0;
	list_add(&sop->so_idhash, &lock_ownerid_hashtbl[idhashval]);
	list_add(&sop->so_strhash, &lock_ownerstr_hashtbl[strhashval]);
	list_add(&sop->so_perstateid, &open_stp->st_lockowners);
	sop->so_is_open_owner = 0;
	sop->so_id = current_ownerid++;
	sop->so_client = clp;
	/* It is the openowner seqid that will be incremented in encode in the
	 * case of new lockowners; so increment the lock seqid manually: */
	sop->so_seqid = lock->lk_new_lock_seqid + 1;
	sop->so_confirmed = 1;
	rp = &sop->so_replay;
	rp->rp_status = nfserr_serverfault;
	rp->rp_buflen = 0;
	rp->rp_buf = rp->rp_ibuf;
	return sop;
}

static struct nfs4_stateid *
alloc_init_lock_stateid(struct nfs4_stateowner *sop, struct nfs4_file *fp, struct nfs4_stateid *open_stp)
{
	struct nfs4_stateid *stp;
	unsigned int hashval = stateid_hashval(sop->so_id, fp->fi_id);

	stp = nfs4_alloc_stateid();
	if (stp == NULL)
		goto out;
	INIT_LIST_HEAD(&stp->st_hash);
	INIT_LIST_HEAD(&stp->st_perfile);
	INIT_LIST_HEAD(&stp->st_perstateowner);
	INIT_LIST_HEAD(&stp->st_lockowners); /* not used */
	list_add(&stp->st_hash, &lockstateid_hashtbl[hashval]);
	list_add(&stp->st_perfile, &fp->fi_stateids);
	list_add(&stp->st_perstateowner, &sop->so_stateids);
	stp->st_stateowner = sop;
	get_nfs4_file(fp);
	stp->st_file = fp;
	stp->st_stateid.si_boot = boot_time;
	stp->st_stateid.si_stateownerid = sop->so_id;
	stp->st_stateid.si_fileid = fp->fi_id;
	stp->st_stateid.si_generation = 0;
	stp->st_access_bmap = 0;
	stp->st_deny_bmap = open_stp->st_deny_bmap;
	stp->st_openstp = open_stp;

out:
=======
=======
>>>>>>> refs/remotes/origin/master
 * strhashval = ownerstr_hashval
 */

static struct nfs4_lockowner *
alloc_init_lock_stateowner(unsigned int strhashval, struct nfs4_client *clp, struct nfs4_ol_stateid *open_stp, struct nfsd4_lock *lock) {
	struct nfs4_lockowner *lo;

	lo = alloc_stateowner(lockowner_slab, &lock->lk_new_owner, clp);
	if (!lo)
		return NULL;
	INIT_LIST_HEAD(&lo->lo_owner.so_stateids);
	lo->lo_owner.so_is_open_owner = 0;
	/* It is the openowner seqid that will be incremented in encode in the
	 * case of new lockowners; so increment the lock seqid manually: */
	lo->lo_owner.so_seqid = lock->lk_new_lock_seqid + 1;
	hash_lockowner(lo, strhashval, clp, open_stp);
	return lo;
}

static struct nfs4_ol_stateid *
alloc_init_lock_stateid(struct nfs4_lockowner *lo, struct nfs4_file *fp, struct nfs4_ol_stateid *open_stp)
{
	struct nfs4_ol_stateid *stp;
	struct nfs4_client *clp = lo->lo_owner.so_client;

	stp = nfs4_alloc_stateid(clp);
	if (stp == NULL)
		return NULL;
<<<<<<< HEAD
	init_stid(&stp->st_stid, clp, NFS4_LOCK_STID);
=======
	stp->st_stid.sc_type = NFS4_LOCK_STID;
>>>>>>> refs/remotes/origin/master
	list_add(&stp->st_perfile, &fp->fi_stateids);
	list_add(&stp->st_perstateowner, &lo->lo_owner.so_stateids);
	stp->st_stateowner = &lo->lo_owner;
	get_nfs4_file(fp);
	stp->st_file = fp;
	stp->st_access_bmap = 0;
	stp->st_deny_bmap = open_stp->st_deny_bmap;
	stp->st_openstp = open_stp;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return stp;
}

static int
check_lock_length(u64 offset, u64 length)
{
	return ((length == 0)  || ((length != NFS4_MAX_UINT64) &&
	     LOFF_OVERFLOW(offset, length)));
}

<<<<<<< HEAD
<<<<<<< HEAD
static void get_lock_access(struct nfs4_stateid *lock_stp, u32 access)
=======
static void get_lock_access(struct nfs4_ol_stateid *lock_stp, u32 access)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void get_lock_access(struct nfs4_ol_stateid *lock_stp, u32 access)
>>>>>>> refs/remotes/origin/master
{
	struct nfs4_file *fp = lock_stp->st_file;
	int oflag = nfs4_access_to_omode(access);

<<<<<<< HEAD
	if (test_bit(access, &lock_stp->st_access_bmap))
		return;
	nfs4_file_get_access(fp, oflag);
	__set_bit(access, &lock_stp->st_access_bmap);
}

<<<<<<< HEAD
=======
__be32 lookup_or_create_lock_state(struct nfsd4_compound_state *cstate, struct nfs4_ol_stateid *ost, struct nfsd4_lock *lock, struct nfs4_ol_stateid **lst, bool *new)
=======
	if (test_access(access, lock_stp))
		return;
	nfs4_file_get_access(fp, oflag);
	set_access(access, lock_stp);
}

static __be32 lookup_or_create_lock_state(struct nfsd4_compound_state *cstate, struct nfs4_ol_stateid *ost, struct nfsd4_lock *lock, struct nfs4_ol_stateid **lst, bool *new)
>>>>>>> refs/remotes/origin/master
{
	struct nfs4_file *fi = ost->st_file;
	struct nfs4_openowner *oo = openowner(ost->st_stateowner);
	struct nfs4_client *cl = oo->oo_owner.so_client;
	struct nfs4_lockowner *lo;
	unsigned int strhashval;
<<<<<<< HEAD

	lo = find_lockowner_str(fi->fi_inode, &cl->cl_clientid, &lock->v.new.owner);
=======
	struct nfsd_net *nn = net_generic(cl->net, nfsd_net_id);

	lo = find_lockowner_str(fi->fi_inode, &cl->cl_clientid,
				&lock->v.new.owner, nn);
>>>>>>> refs/remotes/origin/master
	if (lo) {
		if (!cstate->minorversion)
			return nfserr_bad_seqid;
		/* XXX: a lockowner always has exactly one stateid: */
		*lst = list_first_entry(&lo->lo_owner.so_stateids,
				struct nfs4_ol_stateid, st_perstateowner);
		return nfs_ok;
	}
	strhashval = ownerstr_hashval(cl->cl_clientid.cl_id,
			&lock->v.new.owner);
	lo = alloc_init_lock_stateowner(strhashval, cl, ost, lock);
	if (lo == NULL)
		return nfserr_jukebox;
	*lst = alloc_init_lock_stateid(lo, fi, ost);
	if (*lst == NULL) {
		release_lockowner(lo);
		return nfserr_jukebox;
	}
	*new = true;
	return nfs_ok;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 *  LOCK operation 
 */
__be32
nfsd4_lock(struct svc_rqst *rqstp, struct nfsd4_compound_state *cstate,
	   struct nfsd4_lock *lock)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct nfs4_stateowner *open_sop = NULL;
	struct nfs4_stateowner *lock_sop = NULL;
	struct nfs4_stateid *lock_stp;
=======
	struct nfs4_openowner *open_sop = NULL;
	struct nfs4_lockowner *lock_sop = NULL;
	struct nfs4_ol_stateid *lock_stp;
>>>>>>> refs/remotes/origin/cm-10.0
	struct nfs4_file *fp;
	struct file *filp = NULL;
	struct file_lock file_lock;
	struct file_lock conflock;
	__be32 status = 0;
<<<<<<< HEAD
	unsigned int strhashval;
=======
	bool new_state = false;
	int lkflg;
>>>>>>> refs/remotes/origin/cm-10.0
	int err;
=======
	struct nfs4_openowner *open_sop = NULL;
	struct nfs4_lockowner *lock_sop = NULL;
	struct nfs4_ol_stateid *lock_stp;
	struct file *filp = NULL;
	struct file_lock *file_lock = NULL;
	struct file_lock *conflock = NULL;
	__be32 status = 0;
	bool new_state = false;
	int lkflg;
	int err;
	struct net *net = SVC_NET(rqstp);
	struct nfsd_net *nn = net_generic(net, nfsd_net_id);
>>>>>>> refs/remotes/origin/master

	dprintk("NFSD: nfsd4_lock: start=%Ld length=%Ld\n",
		(long long) lock->lk_offset,
		(long long) lock->lk_length);

	if (check_lock_length(lock->lk_offset, lock->lk_length))
		 return nfserr_inval;

	if ((status = fh_verify(rqstp, &cstate->current_fh,
				S_IFREG, NFSD_MAY_LOCK))) {
		dprintk("NFSD: nfsd4_lock: permission denied!\n");
		return status;
	}

	nfs4_lock_state();

	if (lock->lk_is_new) {
<<<<<<< HEAD
		/*
		 * Client indicates that this is a new lockowner.
		 * Use open owner and open stateid to create lock owner and
		 * lock stateid.
		 */
<<<<<<< HEAD
		struct nfs4_stateid *open_stp = NULL;
		
		status = nfserr_stale_clientid;
		if (!nfsd4_has_session(cstate) &&
		    STALE_CLIENTID(&lock->lk_new_clientid))
			goto out;

		/* validate and update open stateid and open seqid */
		status = nfs4_preprocess_seqid_op(cstate,
				        lock->lk_new_open_seqid,
		                        &lock->lk_new_open_stateid,
					OPEN_STATE,
		                        &lock->lk_replay_owner, &open_stp,
					lock);
		if (status)
			goto out;
		open_sop = lock->lk_replay_owner;
		/* create lockowner and lock stateid */
		fp = open_stp->st_file;
		strhashval = lock_ownerstr_hashval(fp->fi_inode, 
				open_sop->so_client->cl_clientid.cl_id, 
				&lock->v.new.owner);
		/* XXX: Do we need to check for duplicate stateowners on
		 * the same file, or should they just be allowed (and
		 * create new stateids)? */
		status = nfserr_jukebox;
		lock_sop = alloc_init_lock_stateowner(strhashval,
				open_sop->so_client, open_stp, lock);
		if (lock_sop == NULL)
			goto out;
		lock_stp = alloc_init_lock_stateid(lock_sop, fp, open_stp);
		if (lock_stp == NULL)
=======
=======
>>>>>>> refs/remotes/origin/master
		struct nfs4_ol_stateid *open_stp = NULL;

		if (nfsd4_has_session(cstate))
			/* See rfc 5661 18.10.3: given clientid is ignored: */
			memcpy(&lock->v.new.clientid,
				&cstate->session->se_client->cl_clientid,
				sizeof(clientid_t));

		status = nfserr_stale_clientid;
<<<<<<< HEAD
		if (STALE_CLIENTID(&lock->lk_new_clientid))
=======
		if (STALE_CLIENTID(&lock->lk_new_clientid, nn))
>>>>>>> refs/remotes/origin/master
			goto out;

		/* validate and update open stateid and open seqid */
		status = nfs4_preprocess_confirmed_seqid_op(cstate,
				        lock->lk_new_open_seqid,
		                        &lock->lk_new_open_stateid,
<<<<<<< HEAD
					&open_stp);
=======
					&open_stp, nn);
>>>>>>> refs/remotes/origin/master
		if (status)
			goto out;
		open_sop = openowner(open_stp->st_stateowner);
		status = nfserr_bad_stateid;
		if (!same_clid(&open_sop->oo_owner.so_client->cl_clientid,
						&lock->v.new.clientid))
			goto out;
		status = lookup_or_create_lock_state(cstate, open_stp, lock,
							&lock_stp, &new_state);
<<<<<<< HEAD
		if (status)
>>>>>>> refs/remotes/origin/cm-10.0
			goto out;
	} else {
		/* lock (lock owner + lock stateid) already exists */
		status = nfs4_preprocess_seqid_op(cstate,
<<<<<<< HEAD
				       lock->lk_old_lock_seqid, 
				       &lock->lk_old_lock_stateid, 
				       LOCK_STATE,
				       &lock->lk_replay_owner, &lock_stp, lock);
		if (status)
			goto out;
		lock_sop = lock->lk_replay_owner;
		fp = lock_stp->st_file;
	}
	/* lock->lk_replay_owner and lock_stp have been created or found */
=======
				       lock->lk_old_lock_seqid,
				       &lock->lk_old_lock_stateid,
				       NFS4_LOCK_STID, &lock_stp);
		if (status)
			goto out;
	}
	lock_sop = lockowner(lock_stp->st_stateowner);
	fp = lock_stp->st_file;
=======
	} else
		status = nfs4_preprocess_seqid_op(cstate,
				       lock->lk_old_lock_seqid,
				       &lock->lk_old_lock_stateid,
				       NFS4_LOCK_STID, &lock_stp, nn);
	if (status)
		goto out;
	lock_sop = lockowner(lock_stp->st_stateowner);
>>>>>>> refs/remotes/origin/master

	lkflg = setlkflg(lock->lk_type);
	status = nfs4_check_openmode(lock_stp, lkflg);
	if (status)
		goto out;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

	status = nfserr_grace;
	if (locks_in_grace() && !lock->lk_reclaim)
		goto out;
	status = nfserr_no_grace;
	if (!locks_in_grace() && lock->lk_reclaim)
		goto out;

	locks_init_lock(&file_lock);
=======

	status = nfserr_grace;
	if (locks_in_grace(net) && !lock->lk_reclaim)
		goto out;
	status = nfserr_no_grace;
	if (!locks_in_grace(net) && lock->lk_reclaim)
		goto out;

	file_lock = locks_alloc_lock();
	if (!file_lock) {
		dprintk("NFSD: %s: unable to allocate lock!\n", __func__);
		status = nfserr_jukebox;
		goto out;
	}

	locks_init_lock(file_lock);
>>>>>>> refs/remotes/origin/master
	switch (lock->lk_type) {
		case NFS4_READ_LT:
		case NFS4_READW_LT:
			filp = find_readable_file(lock_stp->st_file);
			if (filp)
				get_lock_access(lock_stp, NFS4_SHARE_ACCESS_READ);
<<<<<<< HEAD
			file_lock.fl_type = F_RDLCK;
=======
			file_lock->fl_type = F_RDLCK;
>>>>>>> refs/remotes/origin/master
			break;
		case NFS4_WRITE_LT:
		case NFS4_WRITEW_LT:
			filp = find_writeable_file(lock_stp->st_file);
			if (filp)
				get_lock_access(lock_stp, NFS4_SHARE_ACCESS_WRITE);
<<<<<<< HEAD
			file_lock.fl_type = F_WRLCK;
=======
			file_lock->fl_type = F_WRLCK;
>>>>>>> refs/remotes/origin/master
			break;
		default:
			status = nfserr_inval;
		goto out;
	}
	if (!filp) {
		status = nfserr_openmode;
		goto out;
	}
<<<<<<< HEAD
	file_lock.fl_owner = (fl_owner_t)lock_sop;
	file_lock.fl_pid = current->tgid;
	file_lock.fl_file = filp;
	file_lock.fl_flags = FL_POSIX;
	file_lock.fl_lmops = &nfsd_posix_mng_ops;

	file_lock.fl_start = lock->lk_offset;
	file_lock.fl_end = last_byte_offset(lock->lk_offset, lock->lk_length);
	nfs4_transform_lock_offset(&file_lock);

	/*
	* Try to lock the file in the VFS.
	* Note: locks.c uses the BKL to protect the inode's lock list.
	*/

	err = vfs_lock_file(filp, F_SETLK, &file_lock, &conflock);
	switch (-err) {
	case 0: /* success! */
<<<<<<< HEAD
		update_stateid(&lock_stp->st_stateid);
		memcpy(&lock->lk_resp_stateid, &lock_stp->st_stateid, 
=======
		update_stateid(&lock_stp->st_stid.sc_stateid);
		memcpy(&lock->lk_resp_stateid, &lock_stp->st_stid.sc_stateid, 
>>>>>>> refs/remotes/origin/cm-10.0
=======
	file_lock->fl_owner = (fl_owner_t)lock_sop;
	file_lock->fl_pid = current->tgid;
	file_lock->fl_file = filp;
	file_lock->fl_flags = FL_POSIX;
	file_lock->fl_lmops = &nfsd_posix_mng_ops;
	file_lock->fl_start = lock->lk_offset;
	file_lock->fl_end = last_byte_offset(lock->lk_offset, lock->lk_length);
	nfs4_transform_lock_offset(file_lock);

	conflock = locks_alloc_lock();
	if (!conflock) {
		dprintk("NFSD: %s: unable to allocate lock!\n", __func__);
		status = nfserr_jukebox;
		goto out;
	}

	err = vfs_lock_file(filp, F_SETLK, file_lock, conflock);
	switch (-err) {
	case 0: /* success! */
		update_stateid(&lock_stp->st_stid.sc_stateid);
		memcpy(&lock->lk_resp_stateid, &lock_stp->st_stid.sc_stateid, 
>>>>>>> refs/remotes/origin/master
				sizeof(stateid_t));
		status = 0;
		break;
	case (EAGAIN):		/* conflock holds conflicting lock */
		status = nfserr_denied;
		dprintk("NFSD: nfsd4_lock: conflicting lock found!\n");
<<<<<<< HEAD
		nfs4_set_lock_denied(&conflock, &lock->lk_denied);
=======
		nfs4_set_lock_denied(conflock, &lock->lk_denied);
>>>>>>> refs/remotes/origin/master
		break;
	case (EDEADLK):
		status = nfserr_deadlock;
		break;
	default:
		dprintk("NFSD: nfsd4_lock: vfs_lock_file() failed! status %d\n",err);
		status = nfserrno(err);
		break;
	}
out:
<<<<<<< HEAD
<<<<<<< HEAD
	if (status && lock->lk_is_new && lock_sop)
		release_lockowner(lock_sop);
	if (lock->lk_replay_owner) {
		nfs4_get_stateowner(lock->lk_replay_owner);
		cstate->replay_owner = lock->lk_replay_owner;
	}
	nfs4_unlock_state();
=======
	if (status && new_state)
		release_lockowner(lock_sop);
	if (!cstate->replay_owner)
		nfs4_unlock_state();
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (status && new_state)
		release_lockowner(lock_sop);
	nfsd4_bump_seqid(cstate, status);
	if (!cstate->replay_owner)
		nfs4_unlock_state();
	if (file_lock)
		locks_free_lock(file_lock);
	if (conflock)
		locks_free_lock(conflock);
>>>>>>> refs/remotes/origin/master
	return status;
}

/*
 * The NFSv4 spec allows a client to do a LOCKT without holding an OPEN,
 * so we do a temporary open here just to get an open file to pass to
 * vfs_test_lock.  (Arguably perhaps test_lock should be done with an
 * inode operation.)
 */
static __be32 nfsd_test_lock(struct svc_rqst *rqstp, struct svc_fh *fhp, struct file_lock *lock)
{
	struct file *file;
	__be32 err = nfsd_open(rqstp, fhp, S_IFREG, NFSD_MAY_READ, &file);
	if (!err) {
		err = nfserrno(vfs_test_lock(file, lock));
		nfsd_close(file);
	}
	return err;
}

/*
 * LOCKT operation
 */
__be32
nfsd4_lockt(struct svc_rqst *rqstp, struct nfsd4_compound_state *cstate,
	    struct nfsd4_lockt *lockt)
{
	struct inode *inode;
<<<<<<< HEAD
	struct file_lock file_lock;
<<<<<<< HEAD
=======
	struct nfs4_lockowner *lo;
>>>>>>> refs/remotes/origin/cm-10.0
	__be32 status;

	if (locks_in_grace())
=======
	struct file_lock *file_lock = NULL;
	struct nfs4_lockowner *lo;
	__be32 status;
	struct nfsd_net *nn = net_generic(SVC_NET(rqstp), nfsd_net_id);

	if (locks_in_grace(SVC_NET(rqstp)))
>>>>>>> refs/remotes/origin/master
		return nfserr_grace;

	if (check_lock_length(lockt->lt_offset, lockt->lt_length))
		 return nfserr_inval;

<<<<<<< HEAD
<<<<<<< HEAD
	lockt->lt_stateowner = NULL;
=======
>>>>>>> refs/remotes/origin/cm-10.0
	nfs4_lock_state();

	status = nfserr_stale_clientid;
	if (!nfsd4_has_session(cstate) && STALE_CLIENTID(&lockt->lt_clientid))
		goto out;

<<<<<<< HEAD
	if ((status = fh_verify(rqstp, &cstate->current_fh, S_IFREG, 0))) {
		dprintk("NFSD: nfsd4_lockt: fh_verify() failed!\n");
		if (status == nfserr_symlink)
			status = nfserr_inval;
		goto out;
	}
=======
	if ((status = fh_verify(rqstp, &cstate->current_fh, S_IFREG, 0)))
		goto out;
>>>>>>> refs/remotes/origin/cm-10.0

	inode = cstate->current_fh.fh_dentry->d_inode;
	locks_init_lock(&file_lock);
	switch (lockt->lt_type) {
		case NFS4_READ_LT:
		case NFS4_READW_LT:
			file_lock.fl_type = F_RDLCK;
		break;
		case NFS4_WRITE_LT:
		case NFS4_WRITEW_LT:
			file_lock.fl_type = F_WRLCK;
=======
	nfs4_lock_state();

	if (!nfsd4_has_session(cstate)) {
		status = lookup_clientid(&lockt->lt_clientid, false, nn, NULL);
		if (status)
			goto out;
	}

	if ((status = fh_verify(rqstp, &cstate->current_fh, S_IFREG, 0)))
		goto out;

	inode = cstate->current_fh.fh_dentry->d_inode;
	file_lock = locks_alloc_lock();
	if (!file_lock) {
		dprintk("NFSD: %s: unable to allocate lock!\n", __func__);
		status = nfserr_jukebox;
		goto out;
	}
	locks_init_lock(file_lock);
	switch (lockt->lt_type) {
		case NFS4_READ_LT:
		case NFS4_READW_LT:
			file_lock->fl_type = F_RDLCK;
		break;
		case NFS4_WRITE_LT:
		case NFS4_WRITEW_LT:
			file_lock->fl_type = F_WRLCK;
>>>>>>> refs/remotes/origin/master
		break;
		default:
			dprintk("NFSD: nfs4_lockt: bad lock type!\n");
			status = nfserr_inval;
		goto out;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	lockt->lt_stateowner = find_lockstateowner_str(inode,
			&lockt->lt_clientid, &lockt->lt_owner);
	if (lockt->lt_stateowner)
		file_lock.fl_owner = (fl_owner_t)lockt->lt_stateowner;
=======
	lo = find_lockowner_str(inode, &lockt->lt_clientid, &lockt->lt_owner);
	if (lo)
		file_lock.fl_owner = (fl_owner_t)lo;
>>>>>>> refs/remotes/origin/cm-10.0
	file_lock.fl_pid = current->tgid;
	file_lock.fl_flags = FL_POSIX;

	file_lock.fl_start = lockt->lt_offset;
	file_lock.fl_end = last_byte_offset(lockt->lt_offset, lockt->lt_length);

	nfs4_transform_lock_offset(&file_lock);

	status = nfsd_test_lock(rqstp, &cstate->current_fh, &file_lock);
	if (status)
		goto out;

	if (file_lock.fl_type != F_UNLCK) {
		status = nfserr_denied;
		nfs4_set_lock_denied(&file_lock, &lockt->lt_denied);
	}
out:
	nfs4_unlock_state();
=======
	lo = find_lockowner_str(inode, &lockt->lt_clientid, &lockt->lt_owner, nn);
	if (lo)
		file_lock->fl_owner = (fl_owner_t)lo;
	file_lock->fl_pid = current->tgid;
	file_lock->fl_flags = FL_POSIX;

	file_lock->fl_start = lockt->lt_offset;
	file_lock->fl_end = last_byte_offset(lockt->lt_offset, lockt->lt_length);

	nfs4_transform_lock_offset(file_lock);

	status = nfsd_test_lock(rqstp, &cstate->current_fh, file_lock);
	if (status)
		goto out;

	if (file_lock->fl_type != F_UNLCK) {
		status = nfserr_denied;
		nfs4_set_lock_denied(file_lock, &lockt->lt_denied);
	}
out:
	nfs4_unlock_state();
	if (file_lock)
		locks_free_lock(file_lock);
>>>>>>> refs/remotes/origin/master
	return status;
}

__be32
nfsd4_locku(struct svc_rqst *rqstp, struct nfsd4_compound_state *cstate,
	    struct nfsd4_locku *locku)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct nfs4_stateid *stp;
=======
	struct nfs4_ol_stateid *stp;
>>>>>>> refs/remotes/origin/cm-10.0
	struct file *filp = NULL;
	struct file_lock file_lock;
	__be32 status;
	int err;
						        
=======
	struct nfs4_ol_stateid *stp;
	struct file *filp = NULL;
	struct file_lock *file_lock = NULL;
	__be32 status;
	int err;
	struct nfsd_net *nn = net_generic(SVC_NET(rqstp), nfsd_net_id);

>>>>>>> refs/remotes/origin/master
	dprintk("NFSD: nfsd4_locku: start=%Ld length=%Ld\n",
		(long long) locku->lu_offset,
		(long long) locku->lu_length);

	if (check_lock_length(locku->lu_offset, locku->lu_length))
		 return nfserr_inval;

	nfs4_lock_state();
									        
<<<<<<< HEAD
<<<<<<< HEAD
	if ((status = nfs4_preprocess_seqid_op(cstate,
					locku->lu_seqid, 
					&locku->lu_stateid, 
					LOCK_STATE,
					&locku->lu_stateowner, &stp, NULL)))
		goto out;

=======
	status = nfs4_preprocess_seqid_op(cstate, locku->lu_seqid,
					&locku->lu_stateid, NFS4_LOCK_STID, &stp);
	if (status)
		goto out;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	status = nfs4_preprocess_seqid_op(cstate, locku->lu_seqid,
					&locku->lu_stateid, NFS4_LOCK_STID,
					&stp, nn);
	if (status)
		goto out;
>>>>>>> refs/remotes/origin/master
	filp = find_any_file(stp->st_file);
	if (!filp) {
		status = nfserr_lock_range;
		goto out;
	}
<<<<<<< HEAD
	BUG_ON(!filp);
	locks_init_lock(&file_lock);
	file_lock.fl_type = F_UNLCK;
<<<<<<< HEAD
	file_lock.fl_owner = (fl_owner_t) locku->lu_stateowner;
=======
	file_lock.fl_owner = (fl_owner_t)lockowner(stp->st_stateowner);
>>>>>>> refs/remotes/origin/cm-10.0
	file_lock.fl_pid = current->tgid;
	file_lock.fl_file = filp;
	file_lock.fl_flags = FL_POSIX; 
	file_lock.fl_lmops = &nfsd_posix_mng_ops;
	file_lock.fl_start = locku->lu_offset;

	file_lock.fl_end = last_byte_offset(locku->lu_offset, locku->lu_length);
	nfs4_transform_lock_offset(&file_lock);

	/*
	*  Try to unlock the file in the VFS.
	*/
	err = vfs_lock_file(filp, F_SETLK, &file_lock, NULL);
=======
	file_lock = locks_alloc_lock();
	if (!file_lock) {
		dprintk("NFSD: %s: unable to allocate lock!\n", __func__);
		status = nfserr_jukebox;
		goto out;
	}
	locks_init_lock(file_lock);
	file_lock->fl_type = F_UNLCK;
	file_lock->fl_owner = (fl_owner_t)lockowner(stp->st_stateowner);
	file_lock->fl_pid = current->tgid;
	file_lock->fl_file = filp;
	file_lock->fl_flags = FL_POSIX;
	file_lock->fl_lmops = &nfsd_posix_mng_ops;
	file_lock->fl_start = locku->lu_offset;

	file_lock->fl_end = last_byte_offset(locku->lu_offset,
						locku->lu_length);
	nfs4_transform_lock_offset(file_lock);

	err = vfs_lock_file(filp, F_SETLK, file_lock, NULL);
>>>>>>> refs/remotes/origin/master
	if (err) {
		dprintk("NFSD: nfs4_locku: vfs_lock_file failed!\n");
		goto out_nfserr;
	}
<<<<<<< HEAD
	/*
	* OK, unlock succeeded; the only thing left to do is update the stateid.
	*/
<<<<<<< HEAD
	update_stateid(&stp->st_stateid);
	memcpy(&locku->lu_stateid, &stp->st_stateid, sizeof(stateid_t));

out:
	if (locku->lu_stateowner) {
		nfs4_get_stateowner(locku->lu_stateowner);
		cstate->replay_owner = locku->lu_stateowner;
	}
	nfs4_unlock_state();
=======
=======
>>>>>>> refs/remotes/origin/master
	update_stateid(&stp->st_stid.sc_stateid);
	memcpy(&locku->lu_stateid, &stp->st_stid.sc_stateid, sizeof(stateid_t));

out:
<<<<<<< HEAD
	if (!cstate->replay_owner)
		nfs4_unlock_state();
>>>>>>> refs/remotes/origin/cm-10.0
=======
	nfsd4_bump_seqid(cstate, status);
	if (!cstate->replay_owner)
		nfs4_unlock_state();
	if (file_lock)
		locks_free_lock(file_lock);
>>>>>>> refs/remotes/origin/master
	return status;

out_nfserr:
	status = nfserrno(err);
	goto out;
}

/*
 * returns
 * 	1: locks held by lockowner
 * 	0: no locks held by lockowner
 */
static int
<<<<<<< HEAD
<<<<<<< HEAD
check_for_locks(struct nfs4_file *filp, struct nfs4_stateowner *lowner)
=======
check_for_locks(struct nfs4_file *filp, struct nfs4_lockowner *lowner)
>>>>>>> refs/remotes/origin/cm-10.0
=======
check_for_locks(struct nfs4_file *filp, struct nfs4_lockowner *lowner)
>>>>>>> refs/remotes/origin/master
{
	struct file_lock **flpp;
	struct inode *inode = filp->fi_inode;
	int status = 0;

<<<<<<< HEAD
	lock_flocks();
=======
	spin_lock(&inode->i_lock);
>>>>>>> refs/remotes/origin/master
	for (flpp = &inode->i_flock; *flpp != NULL; flpp = &(*flpp)->fl_next) {
		if ((*flpp)->fl_owner == (fl_owner_t)lowner) {
			status = 1;
			goto out;
		}
	}
out:
<<<<<<< HEAD
	unlock_flocks();
=======
	spin_unlock(&inode->i_lock);
>>>>>>> refs/remotes/origin/master
	return status;
}

__be32
nfsd4_release_lockowner(struct svc_rqst *rqstp,
			struct nfsd4_compound_state *cstate,
			struct nfsd4_release_lockowner *rlockowner)
{
	clientid_t *clid = &rlockowner->rl_clientid;
	struct nfs4_stateowner *sop;
<<<<<<< HEAD
<<<<<<< HEAD
	struct nfs4_stateid *stp;
	struct xdr_netobj *owner = &rlockowner->rl_owner;
	struct list_head matches;
	int i;
=======
=======
>>>>>>> refs/remotes/origin/master
	struct nfs4_lockowner *lo;
	struct nfs4_ol_stateid *stp;
	struct xdr_netobj *owner = &rlockowner->rl_owner;
	struct list_head matches;
	unsigned int hashval = ownerstr_hashval(clid->cl_id, owner);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	__be32 status;
=======
	__be32 status;
	struct nfsd_net *nn = net_generic(SVC_NET(rqstp), nfsd_net_id);
>>>>>>> refs/remotes/origin/master

	dprintk("nfsd4_release_lockowner clientid: (%08x/%08x):\n",
		clid->cl_boot, clid->cl_id);

<<<<<<< HEAD
	/* XXX check for lease expiration */

	status = nfserr_stale_clientid;
	if (STALE_CLIENTID(clid))
		return status;

	nfs4_lock_state();

	status = nfserr_locks_held;
<<<<<<< HEAD
	/* XXX: we're doing a linear search through all the lockowners.
	 * Yipes!  For now we'll just hope clients aren't really using
	 * release_lockowner much, but eventually we have to fix these
	 * data structures. */
	INIT_LIST_HEAD(&matches);
	for (i = 0; i < LOCK_HASH_SIZE; i++) {
		list_for_each_entry(sop, &lock_ownerid_hashtbl[i], so_idhash) {
			if (!same_owner_str(sop, owner, clid))
				continue;
			list_for_each_entry(stp, &sop->so_stateids,
					st_perstateowner) {
				if (check_for_locks(stp->st_file, sop))
					goto out;
				/* Note: so_perclient unused for lockowners,
				 * so it's OK to fool with here. */
				list_add(&sop->so_perclient, &matches);
			}
=======
	INIT_LIST_HEAD(&matches);

	list_for_each_entry(sop, &ownerstr_hashtbl[hashval], so_strhash) {
=======
	nfs4_lock_state();

	status = lookup_clientid(clid, cstate->minorversion, nn, NULL);
	if (status)
		goto out;

	status = nfserr_locks_held;
	INIT_LIST_HEAD(&matches);

	list_for_each_entry(sop, &nn->ownerstr_hashtbl[hashval], so_strhash) {
>>>>>>> refs/remotes/origin/master
		if (sop->so_is_open_owner)
			continue;
		if (!same_owner_str(sop, owner, clid))
			continue;
		list_for_each_entry(stp, &sop->so_stateids,
				st_perstateowner) {
			lo = lockowner(sop);
			if (check_for_locks(stp->st_file, lo))
				goto out;
			list_add(&lo->lo_list, &matches);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		}
	}
	/* Clients probably won't expect us to return with some (but not all)
	 * of the lockowner state released; so don't release any until all
	 * have been checked. */
	status = nfs_ok;
	while (!list_empty(&matches)) {
<<<<<<< HEAD
<<<<<<< HEAD
		sop = list_entry(matches.next, struct nfs4_stateowner,
								so_perclient);
		/* unhash_stateowner deletes so_perclient only
		 * for openowners. */
		list_del(&sop->so_perclient);
		release_lockowner(sop);
=======
=======
>>>>>>> refs/remotes/origin/master
		lo = list_entry(matches.next, struct nfs4_lockowner,
								lo_list);
		/* unhash_stateowner deletes so_perclient only
		 * for openowners. */
		list_del(&lo->lo_list);
		release_lockowner(lo);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}
out:
	nfs4_unlock_state();
	return status;
}

static inline struct nfs4_client_reclaim *
alloc_reclaim(void)
{
	return kmalloc(sizeof(struct nfs4_client_reclaim), GFP_KERNEL);
}

<<<<<<< HEAD
int
nfs4_has_reclaimed_state(const char *name, bool use_exchange_id)
{
	unsigned int strhashval = clientstr_hashval(name);
	struct nfs4_client *clp;

	clp = find_confirmed_client_by_str(name, strhashval);
<<<<<<< HEAD
	return clp ? 1 : 0;
=======
	if (!clp)
		return 0;
	return test_bit(NFSD4_CLIENT_STABLE, &clp->cl_flags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
bool
nfs4_has_reclaimed_state(const char *name, struct nfsd_net *nn)
{
	struct nfs4_client_reclaim *crp;

	crp = nfsd4_find_reclaim_client(name, nn);
	return (crp && crp->cr_clp);
>>>>>>> refs/remotes/origin/master
}

/*
 * failure => all reset bets are off, nfserr_no_grace...
 */
<<<<<<< HEAD
int
nfs4_client_to_reclaim(const char *name)
{
	unsigned int strhashval;
	struct nfs4_client_reclaim *crp = NULL;

	dprintk("NFSD nfs4_client_to_reclaim NAME: %.*s\n", HEXDIR_LEN, name);
	crp = alloc_reclaim();
	if (!crp)
		return 0;
	strhashval = clientstr_hashval(name);
	INIT_LIST_HEAD(&crp->cr_strhash);
	list_add(&crp->cr_strhash, &reclaim_str_hashtbl[strhashval]);
	memcpy(crp->cr_recdir, name, HEXDIR_LEN);
	reclaim_str_hashtbl_size++;
	return 1;
}

<<<<<<< HEAD
static void
=======
void
>>>>>>> refs/remotes/origin/cm-10.0
nfs4_release_reclaim(void)
=======
struct nfs4_client_reclaim *
nfs4_client_to_reclaim(const char *name, struct nfsd_net *nn)
{
	unsigned int strhashval;
	struct nfs4_client_reclaim *crp;

	dprintk("NFSD nfs4_client_to_reclaim NAME: %.*s\n", HEXDIR_LEN, name);
	crp = alloc_reclaim();
	if (crp) {
		strhashval = clientstr_hashval(name);
		INIT_LIST_HEAD(&crp->cr_strhash);
		list_add(&crp->cr_strhash, &nn->reclaim_str_hashtbl[strhashval]);
		memcpy(crp->cr_recdir, name, HEXDIR_LEN);
		crp->cr_clp = NULL;
		nn->reclaim_str_hashtbl_size++;
	}
	return crp;
}

void
nfs4_remove_reclaim_record(struct nfs4_client_reclaim *crp, struct nfsd_net *nn)
{
	list_del(&crp->cr_strhash);
	kfree(crp);
	nn->reclaim_str_hashtbl_size--;
}

void
nfs4_release_reclaim(struct nfsd_net *nn)
>>>>>>> refs/remotes/origin/master
{
	struct nfs4_client_reclaim *crp = NULL;
	int i;

	for (i = 0; i < CLIENT_HASH_SIZE; i++) {
<<<<<<< HEAD
		while (!list_empty(&reclaim_str_hashtbl[i])) {
			crp = list_entry(reclaim_str_hashtbl[i].next,
			                struct nfs4_client_reclaim, cr_strhash);
			list_del(&crp->cr_strhash);
			kfree(crp);
			reclaim_str_hashtbl_size--;
		}
	}
	BUG_ON(reclaim_str_hashtbl_size);
=======
		while (!list_empty(&nn->reclaim_str_hashtbl[i])) {
			crp = list_entry(nn->reclaim_str_hashtbl[i].next,
			                struct nfs4_client_reclaim, cr_strhash);
			nfs4_remove_reclaim_record(crp, nn);
		}
	}
	WARN_ON_ONCE(nn->reclaim_str_hashtbl_size);
>>>>>>> refs/remotes/origin/master
}

/*
 * called from OPEN, CLAIM_PREVIOUS with a new clientid. */
<<<<<<< HEAD
<<<<<<< HEAD
static struct nfs4_client_reclaim *
nfs4_find_reclaim_client(clientid_t *clid)
{
	unsigned int strhashval;
	struct nfs4_client *clp;
	struct nfs4_client_reclaim *crp = NULL;


	/* find clientid in conf_id_hashtbl */
	clp = find_confirmed_client(clid);
	if (clp == NULL)
		return NULL;

=======
struct nfs4_client_reclaim *
nfsd4_find_reclaim_client(struct nfs4_client *clp)
=======
struct nfs4_client_reclaim *
nfsd4_find_reclaim_client(const char *recdir, struct nfsd_net *nn)
>>>>>>> refs/remotes/origin/master
{
	unsigned int strhashval;
	struct nfs4_client_reclaim *crp = NULL;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	dprintk("NFSD: nfs4_find_reclaim_client for %.*s with recdir %s\n",
		            clp->cl_name.len, clp->cl_name.data,
			    clp->cl_recdir);

	/* find clp->cl_name in reclaim_str_hashtbl */
	strhashval = clientstr_hashval(clp->cl_recdir);
	list_for_each_entry(crp, &reclaim_str_hashtbl[strhashval], cr_strhash) {
		if (same_name(crp->cr_recdir, clp->cl_recdir)) {
=======
	dprintk("NFSD: nfs4_find_reclaim_client for recdir %s\n", recdir);

	strhashval = clientstr_hashval(recdir);
	list_for_each_entry(crp, &nn->reclaim_str_hashtbl[strhashval], cr_strhash) {
		if (same_name(crp->cr_recdir, recdir)) {
>>>>>>> refs/remotes/origin/master
			return crp;
		}
	}
	return NULL;
}

/*
* Called from OPEN. Look for clientid in reclaim list.
*/
__be32
<<<<<<< HEAD
nfs4_check_open_reclaim(clientid_t *clid)
{
<<<<<<< HEAD
	return nfs4_find_reclaim_client(clid) ? nfs_ok : nfserr_reclaim_bad;
}

/* initialization to perform at module load time: */

int
nfs4_state_init(void)
{
	int i, status;

	status = nfsd4_init_slabs();
	if (status)
		return status;
=======
	struct nfs4_client *clp;

	/* find clientid in conf_id_hashtbl */
	clp = find_confirmed_client(clid);
=======
nfs4_check_open_reclaim(clientid_t *clid, bool sessions, struct nfsd_net *nn)
{
	struct nfs4_client *clp;

	/* find clientid in conf_id_hashtbl */
	clp = find_confirmed_client(clid, sessions, nn);
>>>>>>> refs/remotes/origin/master
	if (clp == NULL)
		return nfserr_reclaim_bad;

	return nfsd4_client_record_check(clp) ? nfserr_reclaim_bad : nfs_ok;
}

#ifdef CONFIG_NFSD_FAULT_INJECTION

<<<<<<< HEAD
void nfsd_forget_clients(u64 num)
{
	struct nfs4_client *clp, *next;
	int count = 0;

	nfs4_lock_state();
	list_for_each_entry_safe(clp, next, &client_lru, cl_lru) {
		nfsd4_client_record_remove(clp);
		expire_client(clp);
		if (++count == num)
			break;
	}
	nfs4_unlock_state();

	printk(KERN_INFO "NFSD: Forgot %d clients", count);
}

static void release_lockowner_sop(struct nfs4_stateowner *sop)
{
	release_lockowner(lockowner(sop));
}

static void release_openowner_sop(struct nfs4_stateowner *sop)
{
	release_openowner(openowner(sop));
}

static int nfsd_release_n_owners(u64 num, bool is_open_owner,
				void (*release_sop)(struct nfs4_stateowner *))
{
	int i, count = 0;
	struct nfs4_stateowner *sop, *next;

	for (i = 0; i < OWNER_HASH_SIZE; i++) {
		list_for_each_entry_safe(sop, next, &ownerstr_hashtbl[i], so_strhash) {
			if (sop->so_is_open_owner != is_open_owner)
				continue;
			release_sop(sop);
			if (++count == num)
				return count;
		}
	}
	return count;
}

void nfsd_forget_locks(u64 num)
{
	int count;

	nfs4_lock_state();
	count = nfsd_release_n_owners(num, false, release_lockowner_sop);
	nfs4_unlock_state();

	printk(KERN_INFO "NFSD: Forgot %d locks", count);
}

void nfsd_forget_openowners(u64 num)
{
	int count;

	nfs4_lock_state();
	count = nfsd_release_n_owners(num, true, release_openowner_sop);
	nfs4_unlock_state();

	printk(KERN_INFO "NFSD: Forgot %d open owners", count);
}

int nfsd_process_n_delegations(u64 num, void (*deleg_func)(struct nfs4_delegation *))
{
	int i, count = 0;
	struct nfs4_file *fp, *fnext;
	struct nfs4_delegation *dp, *dnext;

	for (i = 0; i < FILE_HASH_SIZE; i++) {
		list_for_each_entry_safe(fp, fnext, &file_hashtbl[i], fi_hash) {
			list_for_each_entry_safe(dp, dnext, &fp->fi_delegations, dl_perfile) {
				deleg_func(dp);
				if (++count == num)
					return count;
			}
		}
	}

	return count;
}

void nfsd_forget_delegations(u64 num)
{
	unsigned int count;

	nfs4_lock_state();
	count = nfsd_process_n_delegations(num, unhash_delegation);
	nfs4_unlock_state();

	printk(KERN_INFO "NFSD: Forgot %d delegations", count);
}

void nfsd_recall_delegations(u64 num)
{
	unsigned int count;

	nfs4_lock_state();
	spin_lock(&recall_lock);
	count = nfsd_process_n_delegations(num, nfsd_break_one_deleg);
	spin_unlock(&recall_lock);
	nfs4_unlock_state();

	printk(KERN_INFO "NFSD: Recalled %d delegations", count);
}

#endif /* CONFIG_NFSD_FAULT_INJECTION */

/* initialization to perform at module load time: */

void
nfs4_state_init(void)
{
	int i;

>>>>>>> refs/remotes/origin/cm-10.0
	for (i = 0; i < CLIENT_HASH_SIZE; i++) {
		INIT_LIST_HEAD(&conf_id_hashtbl[i]);
		INIT_LIST_HEAD(&conf_str_hashtbl[i]);
		INIT_LIST_HEAD(&unconf_str_hashtbl[i]);
		INIT_LIST_HEAD(&unconf_id_hashtbl[i]);
		INIT_LIST_HEAD(&reclaim_str_hashtbl[i]);
	}
	for (i = 0; i < SESSION_HASH_SIZE; i++)
		INIT_LIST_HEAD(&sessionid_hashtbl[i]);
	for (i = 0; i < FILE_HASH_SIZE; i++) {
		INIT_LIST_HEAD(&file_hashtbl[i]);
	}
	for (i = 0; i < OWNER_HASH_SIZE; i++) {
		INIT_LIST_HEAD(&ownerstr_hashtbl[i]);
<<<<<<< HEAD
		INIT_LIST_HEAD(&ownerid_hashtbl[i]);
	}
	for (i = 0; i < STATEID_HASH_SIZE; i++) {
		INIT_LIST_HEAD(&stateid_hashtbl[i]);
		INIT_LIST_HEAD(&lockstateid_hashtbl[i]);
	}
	for (i = 0; i < LOCK_HASH_SIZE; i++) {
		INIT_LIST_HEAD(&lock_ownerid_hashtbl[i]);
		INIT_LIST_HEAD(&lock_ownerstr_hashtbl[i]);
	}
	memset(&onestateid, ~0, sizeof(stateid_t));
=======
	}
	for (i = 0; i < LOCKOWNER_INO_HASH_SIZE; i++)
		INIT_LIST_HEAD(&lockowner_ino_hashtbl[i]);
>>>>>>> refs/remotes/origin/cm-10.0
	INIT_LIST_HEAD(&close_lru);
	INIT_LIST_HEAD(&client_lru);
	INIT_LIST_HEAD(&del_recall_lru);
	reclaim_str_hashtbl_size = 0;
<<<<<<< HEAD
	return 0;
}

static void
nfsd4_load_reboot_recovery_data(void)
{
	int status;

	nfs4_lock_state();
	nfsd4_init_recdir(user_recovery_dirname);
	status = nfsd4_recdir_load();
	nfs4_unlock_state();
	if (status)
		printk("NFSD: Failure reading reboot recovery data\n");
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
u64 nfsd_forget_client(struct nfs4_client *clp, u64 max)
{
	if (mark_client_expired(clp))
		return 0;
	expire_client(clp);
	return 1;
}

u64 nfsd_print_client(struct nfs4_client *clp, u64 num)
{
	char buf[INET6_ADDRSTRLEN];
	rpc_ntop((struct sockaddr *)&clp->cl_addr, buf, sizeof(buf));
	printk(KERN_INFO "NFS Client: %s\n", buf);
	return 1;
}

static void nfsd_print_count(struct nfs4_client *clp, unsigned int count,
			     const char *type)
{
	char buf[INET6_ADDRSTRLEN];
	rpc_ntop((struct sockaddr *)&clp->cl_addr, buf, sizeof(buf));
	printk(KERN_INFO "NFS Client: %s has %u %s\n", buf, count, type);
}

static u64 nfsd_foreach_client_lock(struct nfs4_client *clp, u64 max, void (*func)(struct nfs4_lockowner *))
{
	struct nfs4_openowner *oop;
	struct nfs4_lockowner *lop, *lo_next;
	struct nfs4_ol_stateid *stp, *st_next;
	u64 count = 0;

	list_for_each_entry(oop, &clp->cl_openowners, oo_perclient) {
		list_for_each_entry_safe(stp, st_next, &oop->oo_owner.so_stateids, st_perstateowner) {
			list_for_each_entry_safe(lop, lo_next, &stp->st_lockowners, lo_perstateid) {
				if (func)
					func(lop);
				if (++count == max)
					return count;
			}
		}
	}

	return count;
}

u64 nfsd_forget_client_locks(struct nfs4_client *clp, u64 max)
{
	return nfsd_foreach_client_lock(clp, max, release_lockowner);
}

u64 nfsd_print_client_locks(struct nfs4_client *clp, u64 max)
{
	u64 count = nfsd_foreach_client_lock(clp, max, NULL);
	nfsd_print_count(clp, count, "locked files");
	return count;
}

static u64 nfsd_foreach_client_open(struct nfs4_client *clp, u64 max, void (*func)(struct nfs4_openowner *))
{
	struct nfs4_openowner *oop, *next;
	u64 count = 0;

	list_for_each_entry_safe(oop, next, &clp->cl_openowners, oo_perclient) {
		if (func)
			func(oop);
		if (++count == max)
			break;
	}

	return count;
}

u64 nfsd_forget_client_openowners(struct nfs4_client *clp, u64 max)
{
	return nfsd_foreach_client_open(clp, max, release_openowner);
}

u64 nfsd_print_client_openowners(struct nfs4_client *clp, u64 max)
{
	u64 count = nfsd_foreach_client_open(clp, max, NULL);
	nfsd_print_count(clp, count, "open files");
	return count;
}

static u64 nfsd_find_all_delegations(struct nfs4_client *clp, u64 max,
				     struct list_head *victims)
{
	struct nfs4_delegation *dp, *next;
	u64 count = 0;

	list_for_each_entry_safe(dp, next, &clp->cl_delegations, dl_perclnt) {
		if (victims)
			list_move(&dp->dl_recall_lru, victims);
		if (++count == max)
			break;
	}
	return count;
}

u64 nfsd_forget_client_delegations(struct nfs4_client *clp, u64 max)
{
	struct nfs4_delegation *dp, *next;
	LIST_HEAD(victims);
	u64 count;

	spin_lock(&recall_lock);
	count = nfsd_find_all_delegations(clp, max, &victims);
	spin_unlock(&recall_lock);

	list_for_each_entry_safe(dp, next, &victims, dl_recall_lru)
		revoke_delegation(dp);

	return count;
}

u64 nfsd_recall_client_delegations(struct nfs4_client *clp, u64 max)
{
	struct nfs4_delegation *dp, *next;
	LIST_HEAD(victims);
	u64 count;

	spin_lock(&recall_lock);
	count = nfsd_find_all_delegations(clp, max, &victims);
	list_for_each_entry_safe(dp, next, &victims, dl_recall_lru)
		nfsd_break_one_deleg(dp);
	spin_unlock(&recall_lock);

	return count;
}

u64 nfsd_print_client_delegations(struct nfs4_client *clp, u64 max)
{
	u64 count = 0;

	spin_lock(&recall_lock);
	count = nfsd_find_all_delegations(clp, max, NULL);
	spin_unlock(&recall_lock);

	nfsd_print_count(clp, count, "delegations");
	return count;
}

u64 nfsd_for_n_state(u64 max, u64 (*func)(struct nfs4_client *, u64))
{
	struct nfs4_client *clp, *next;
	u64 count = 0;
	struct nfsd_net *nn = net_generic(current->nsproxy->net_ns, nfsd_net_id);

	if (!nfsd_netns_ready(nn))
		return 0;

	list_for_each_entry_safe(clp, next, &nn->client_lru, cl_lru) {
		count += func(clp, max - count);
		if ((max != 0) && (count >= max))
			break;
	}

	return count;
}

struct nfs4_client *nfsd_find_client(struct sockaddr_storage *addr, size_t addr_size)
{
	struct nfs4_client *clp;
	struct nfsd_net *nn = net_generic(current->nsproxy->net_ns, nfsd_net_id);

	if (!nfsd_netns_ready(nn))
		return NULL;

	list_for_each_entry(clp, &nn->client_lru, cl_lru) {
		if (memcmp(&clp->cl_addr, addr, addr_size) == 0)
			return clp;
	}
	return NULL;
}

#endif /* CONFIG_NFSD_FAULT_INJECTION */

/* initialization to perform at module load time: */

void
nfs4_state_init(void)
{
>>>>>>> refs/remotes/origin/master
}

/*
 * Since the lifetime of a delegation isn't limited to that of an open, a
 * client may quite reasonably hang on to a delegation as long as it has
 * the inode cached.  This becomes an obvious problem the first time a
 * client's inode cache approaches the size of the server's total memory.
 *
 * For now we avoid this problem by imposing a hard limit on the number
 * of delegations, which varies according to the server's memory size.
 */
static void
set_max_delegations(void)
{
	/*
	 * Allow at most 4 delegations per megabyte of RAM.  Quick
	 * estimates suggest that in the worst case (where every delegation
	 * is for a different inode), a delegation could take about 1.5K,
	 * giving a worst case usage of about 6% of memory.
	 */
	max_delegations = nr_free_buffer_pages() >> (20 - 2 - PAGE_SHIFT);
}

<<<<<<< HEAD
/* initialization to perform when the nfsd service is started: */

<<<<<<< HEAD
static int
__nfs4_state_start(void)
{
	int ret;

=======
=======
static int nfs4_state_create_net(struct net *net)
{
	struct nfsd_net *nn = net_generic(net, nfsd_net_id);
	int i;

	nn->conf_id_hashtbl = kmalloc(sizeof(struct list_head) *
			CLIENT_HASH_SIZE, GFP_KERNEL);
	if (!nn->conf_id_hashtbl)
		goto err;
	nn->unconf_id_hashtbl = kmalloc(sizeof(struct list_head) *
			CLIENT_HASH_SIZE, GFP_KERNEL);
	if (!nn->unconf_id_hashtbl)
		goto err_unconf_id;
	nn->ownerstr_hashtbl = kmalloc(sizeof(struct list_head) *
			OWNER_HASH_SIZE, GFP_KERNEL);
	if (!nn->ownerstr_hashtbl)
		goto err_ownerstr;
	nn->lockowner_ino_hashtbl = kmalloc(sizeof(struct list_head) *
			LOCKOWNER_INO_HASH_SIZE, GFP_KERNEL);
	if (!nn->lockowner_ino_hashtbl)
		goto err_lockowner_ino;
	nn->sessionid_hashtbl = kmalloc(sizeof(struct list_head) *
			SESSION_HASH_SIZE, GFP_KERNEL);
	if (!nn->sessionid_hashtbl)
		goto err_sessionid;

	for (i = 0; i < CLIENT_HASH_SIZE; i++) {
		INIT_LIST_HEAD(&nn->conf_id_hashtbl[i]);
		INIT_LIST_HEAD(&nn->unconf_id_hashtbl[i]);
	}
	for (i = 0; i < OWNER_HASH_SIZE; i++)
		INIT_LIST_HEAD(&nn->ownerstr_hashtbl[i]);
	for (i = 0; i < LOCKOWNER_INO_HASH_SIZE; i++)
		INIT_LIST_HEAD(&nn->lockowner_ino_hashtbl[i]);
	for (i = 0; i < SESSION_HASH_SIZE; i++)
		INIT_LIST_HEAD(&nn->sessionid_hashtbl[i]);
	nn->conf_name_tree = RB_ROOT;
	nn->unconf_name_tree = RB_ROOT;
	INIT_LIST_HEAD(&nn->client_lru);
	INIT_LIST_HEAD(&nn->close_lru);
	INIT_LIST_HEAD(&nn->del_recall_lru);
	spin_lock_init(&nn->client_lock);

	INIT_DELAYED_WORK(&nn->laundromat_work, laundromat_main);
	get_net(net);

	return 0;

err_sessionid:
	kfree(nn->lockowner_ino_hashtbl);
err_lockowner_ino:
	kfree(nn->ownerstr_hashtbl);
err_ownerstr:
	kfree(nn->unconf_id_hashtbl);
err_unconf_id:
	kfree(nn->conf_id_hashtbl);
err:
	return -ENOMEM;
}

static void
nfs4_state_destroy_net(struct net *net)
{
	int i;
	struct nfs4_client *clp = NULL;
	struct nfsd_net *nn = net_generic(net, nfsd_net_id);
	struct rb_node *node, *tmp;

	for (i = 0; i < CLIENT_HASH_SIZE; i++) {
		while (!list_empty(&nn->conf_id_hashtbl[i])) {
			clp = list_entry(nn->conf_id_hashtbl[i].next, struct nfs4_client, cl_idhash);
			destroy_client(clp);
		}
	}

	node = rb_first(&nn->unconf_name_tree);
	while (node != NULL) {
		tmp = node;
		node = rb_next(tmp);
		clp = rb_entry(tmp, struct nfs4_client, cl_namenode);
		rb_erase(tmp, &nn->unconf_name_tree);
		destroy_client(clp);
	}

	kfree(nn->sessionid_hashtbl);
	kfree(nn->lockowner_ino_hashtbl);
	kfree(nn->ownerstr_hashtbl);
	kfree(nn->unconf_id_hashtbl);
	kfree(nn->conf_id_hashtbl);
	put_net(net);
}

int
nfs4_state_start_net(struct net *net)
{
	struct nfsd_net *nn = net_generic(net, nfsd_net_id);
	int ret;

	ret = nfs4_state_create_net(net);
	if (ret)
		return ret;
	nfsd4_client_tracking_init(net);
	nn->boot_time = get_seconds();
	locks_start_grace(net, &nn->nfsd4_manager);
	nn->grace_ended = false;
	printk(KERN_INFO "NFSD: starting %ld-second grace period (net %p)\n",
	       nn->nfsd4_grace, net);
	queue_delayed_work(laundry_wq, &nn->laundromat_work, nn->nfsd4_grace * HZ);
	return 0;
}

/* initialization to perform when the nfsd service is started: */

>>>>>>> refs/remotes/origin/master
int
nfs4_state_start(void)
{
	int ret;

<<<<<<< HEAD
	/*
	 * FIXME: For now, we hang most of the pernet global stuff off of
	 * init_net until nfsd is fully containerized. Eventually, we'll
	 * need to pass a net pointer into this function, take a reference
	 * to that instead and then do most of the rest of this on a per-net
	 * basis.
	 */
	get_net(&init_net);
	nfsd4_client_tracking_init(&init_net);
>>>>>>> refs/remotes/origin/cm-10.0
	boot_time = get_seconds();
	locks_start_grace(&nfsd4_manager);
	printk(KERN_INFO "NFSD: starting %ld-second grace period\n",
	       nfsd4_grace);
	ret = set_callback_cred();
<<<<<<< HEAD
	if (ret)
		return -ENOMEM;
	laundry_wq = create_singlethread_workqueue("nfsd4");
	if (laundry_wq == NULL)
		return -ENOMEM;
=======
	if (ret) {
		ret = -ENOMEM;
		goto out_recovery;
	}
	laundry_wq = create_singlethread_workqueue("nfsd4");
=======
	ret = set_callback_cred();
	if (ret)
		return -ENOMEM;
	laundry_wq = create_singlethread_workqueue("nfsd4");
>>>>>>> refs/remotes/origin/master
	if (laundry_wq == NULL) {
		ret = -ENOMEM;
		goto out_recovery;
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	ret = nfsd4_create_callback_queue();
	if (ret)
		goto out_free_laundry;
	queue_delayed_work(laundry_wq, &laundromat_work, nfsd4_grace * HZ);
	set_max_delegations();
	return 0;
out_free_laundry:
	destroy_workqueue(laundry_wq);
<<<<<<< HEAD
	return ret;
}

int
nfs4_state_start(void)
{
	nfsd4_load_reboot_recovery_data();
	return __nfs4_state_start();
}

=======
out_recovery:
	nfsd4_client_tracking_exit(&init_net);
	put_net(&init_net);
	return ret;
}

>>>>>>> refs/remotes/origin/cm-10.0
static void
__nfs4_state_shutdown(void)
{
	int i;
	struct nfs4_client *clp = NULL;
	struct nfs4_delegation *dp = NULL;
	struct list_head *pos, *next, reaplist;

	for (i = 0; i < CLIENT_HASH_SIZE; i++) {
		while (!list_empty(&conf_id_hashtbl[i])) {
			clp = list_entry(conf_id_hashtbl[i].next, struct nfs4_client, cl_idhash);
			expire_client(clp);
		}
		while (!list_empty(&unconf_str_hashtbl[i])) {
			clp = list_entry(unconf_str_hashtbl[i].next, struct nfs4_client, cl_strhash);
			expire_client(clp);
		}
	}
	INIT_LIST_HEAD(&reaplist);
	spin_lock(&recall_lock);
	list_for_each_safe(pos, next, &del_recall_lru) {
=======
	ret = nfsd4_create_callback_queue();
	if (ret)
		goto out_free_laundry;

	set_max_delegations();

	return 0;

out_free_laundry:
	destroy_workqueue(laundry_wq);
out_recovery:
	return ret;
}

void
nfs4_state_shutdown_net(struct net *net)
{
	struct nfs4_delegation *dp = NULL;
	struct list_head *pos, *next, reaplist;
	struct nfsd_net *nn = net_generic(net, nfsd_net_id);

	cancel_delayed_work_sync(&nn->laundromat_work);
	locks_end_grace(&nn->nfsd4_manager);

	nfs4_lock_state();
	INIT_LIST_HEAD(&reaplist);
	spin_lock(&recall_lock);
	list_for_each_safe(pos, next, &nn->del_recall_lru) {
>>>>>>> refs/remotes/origin/master
		dp = list_entry (pos, struct nfs4_delegation, dl_recall_lru);
		list_move(&dp->dl_recall_lru, &reaplist);
	}
	spin_unlock(&recall_lock);
	list_for_each_safe(pos, next, &reaplist) {
		dp = list_entry (pos, struct nfs4_delegation, dl_recall_lru);
<<<<<<< HEAD
<<<<<<< HEAD
		list_del_init(&dp->dl_recall_lru);
		unhash_delegation(dp);
	}

	nfsd4_shutdown_recdir();
=======
		unhash_delegation(dp);
	}

	nfsd4_client_tracking_exit(&init_net);
	put_net(&init_net);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		destroy_delegation(dp);
	}

	nfsd4_client_tracking_exit(net);
	nfs4_state_destroy_net(net);
	nfs4_unlock_state();
>>>>>>> refs/remotes/origin/master
}

void
nfs4_state_shutdown(void)
{
<<<<<<< HEAD
	cancel_delayed_work_sync(&laundromat_work);
	destroy_workqueue(laundry_wq);
	locks_end_grace(&nfsd4_manager);
	nfs4_lock_state();
<<<<<<< HEAD
	nfs4_release_reclaim();
=======
>>>>>>> refs/remotes/origin/cm-10.0
	__nfs4_state_shutdown();
	nfs4_unlock_state();
	nfsd4_destroy_callback_queue();
}

<<<<<<< HEAD
/*
 * user_recovery_dirname is protected by the nfsd_mutex since it's only
 * accessed when nfsd is starting.
 */
static void
nfs4_set_recdir(char *recdir)
{
	strcpy(user_recovery_dirname, recdir);
}

/*
 * Change the NFSv4 recovery directory to recdir.
 */
int
nfs4_reset_recoverydir(char *recdir)
{
	int status;
	struct path path;

	status = kern_path(recdir, LOOKUP_FOLLOW, &path);
	if (status)
		return status;
	status = -ENOTDIR;
	if (S_ISDIR(path.dentry->d_inode->i_mode)) {
		nfs4_set_recdir(recdir);
		status = 0;
	}
	path_put(&path);
	return status;
}

char *
nfs4_recoverydir(void)
{
	return user_recovery_dirname;
=======
=======
	destroy_workqueue(laundry_wq);
	nfsd4_destroy_callback_queue();
}

>>>>>>> refs/remotes/origin/master
static void
get_stateid(struct nfsd4_compound_state *cstate, stateid_t *stateid)
{
	if (HAS_STATE_ID(cstate, CURRENT_STATE_ID_FLAG) && CURRENT_STATEID(stateid))
		memcpy(stateid, &cstate->current_stateid, sizeof(stateid_t));
}

static void
put_stateid(struct nfsd4_compound_state *cstate, stateid_t *stateid)
{
	if (cstate->minorversion) {
		memcpy(&cstate->current_stateid, stateid, sizeof(stateid_t));
		SET_STATE_ID(cstate, CURRENT_STATE_ID_FLAG);
	}
}

void
clear_current_stateid(struct nfsd4_compound_state *cstate)
{
	CLEAR_STATE_ID(cstate, CURRENT_STATE_ID_FLAG);
}

/*
 * functions to set current state id
 */
void
nfsd4_set_opendowngradestateid(struct nfsd4_compound_state *cstate, struct nfsd4_open_downgrade *odp)
{
	put_stateid(cstate, &odp->od_stateid);
}

void
nfsd4_set_openstateid(struct nfsd4_compound_state *cstate, struct nfsd4_open *open)
{
	put_stateid(cstate, &open->op_stateid);
}

void
nfsd4_set_closestateid(struct nfsd4_compound_state *cstate, struct nfsd4_close *close)
{
	put_stateid(cstate, &close->cl_stateid);
}

void
nfsd4_set_lockstateid(struct nfsd4_compound_state *cstate, struct nfsd4_lock *lock)
{
	put_stateid(cstate, &lock->lk_resp_stateid);
}

/*
 * functions to consume current state id
 */

void
nfsd4_get_opendowngradestateid(struct nfsd4_compound_state *cstate, struct nfsd4_open_downgrade *odp)
{
	get_stateid(cstate, &odp->od_stateid);
}

void
nfsd4_get_delegreturnstateid(struct nfsd4_compound_state *cstate, struct nfsd4_delegreturn *drp)
{
	get_stateid(cstate, &drp->dr_stateid);
}

void
nfsd4_get_freestateid(struct nfsd4_compound_state *cstate, struct nfsd4_free_stateid *fsp)
{
	get_stateid(cstate, &fsp->fr_stateid);
}

void
nfsd4_get_setattrstateid(struct nfsd4_compound_state *cstate, struct nfsd4_setattr *setattr)
{
	get_stateid(cstate, &setattr->sa_stateid);
}

void
nfsd4_get_closestateid(struct nfsd4_compound_state *cstate, struct nfsd4_close *close)
{
	get_stateid(cstate, &close->cl_stateid);
}

void
nfsd4_get_lockustateid(struct nfsd4_compound_state *cstate, struct nfsd4_locku *locku)
{
	get_stateid(cstate, &locku->lu_stateid);
}

void
nfsd4_get_readstateid(struct nfsd4_compound_state *cstate, struct nfsd4_read *read)
{
	get_stateid(cstate, &read->rd_stateid);
}

void
nfsd4_get_writestateid(struct nfsd4_compound_state *cstate, struct nfsd4_write *write)
{
	get_stateid(cstate, &write->wr_stateid);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}
