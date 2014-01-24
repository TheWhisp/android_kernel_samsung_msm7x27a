/*
 * linux/fs/nfs/nfs4_fs.h
 *
 * Copyright (C) 2005 Trond Myklebust
 *
 * NFSv4-specific filesystem definitions and declarations
 */

#ifndef __LINUX_FS_NFS_NFS4_FS_H
#define __LINUX_FS_NFS_NFS4_FS_H

<<<<<<< HEAD
#ifdef CONFIG_NFS_V4

struct idmap;

<<<<<<< HEAD
/*
 * In a seqid-mutating op, this macro controls which error return
 * values trigger incrementation of the seqid.
 *
 * from rfc 3010:
 * The client MUST monotonically increment the sequence number for the
 * CLOSE, LOCK, LOCKU, OPEN, OPEN_CONFIRM, and OPEN_DOWNGRADE
 * operations.  This is true even in the event that the previous
 * operation that used the sequence number received an error.  The only
 * exception to this rule is if the previous operation received one of
 * the following errors: NFSERR_STALE_CLIENTID, NFSERR_STALE_STATEID,
 * NFSERR_BAD_STATEID, NFSERR_BAD_SEQID, NFSERR_BADXDR,
 * NFSERR_RESOURCE, NFSERR_NOFILEHANDLE.
 *
 */
#define seqid_mutating_err(err)       \
(((err) != NFSERR_STALE_CLIENTID) &&  \
 ((err) != NFSERR_STALE_STATEID)  &&  \
 ((err) != NFSERR_BAD_STATEID)    &&  \
 ((err) != NFSERR_BAD_SEQID)      &&  \
 ((err) != NFSERR_BAD_XDR)        &&  \
 ((err) != NFSERR_RESOURCE)       &&  \
 ((err) != NFSERR_NOFILEHANDLE))

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
#if defined(CONFIG_NFS_V4_2)
#define NFS4_MAX_MINOR_VERSION 2
#elif defined(CONFIG_NFS_V4_1)
#define NFS4_MAX_MINOR_VERSION 1
#else
#define NFS4_MAX_MINOR_VERSION 0
#endif

#if IS_ENABLED(CONFIG_NFS_V4)

#define NFS4_MAX_LOOP_ON_RECOVER (10)

#include <linux/seqlock.h>

struct idmap;

>>>>>>> refs/remotes/origin/master
enum nfs4_client_state {
	NFS4CLNT_MANAGER_RUNNING  = 0,
	NFS4CLNT_CHECK_LEASE,
	NFS4CLNT_LEASE_EXPIRED,
	NFS4CLNT_RECLAIM_REBOOT,
	NFS4CLNT_RECLAIM_NOGRACE,
	NFS4CLNT_DELEGRETURN,
<<<<<<< HEAD
<<<<<<< HEAD
	NFS4CLNT_LAYOUTRECALL,
	NFS4CLNT_SESSION_RESET,
	NFS4CLNT_RECALL_SLOT,
	NFS4CLNT_LEASE_CONFIRM,
=======
	NFS4CLNT_SESSION_RESET,
	NFS4CLNT_RECALL_SLOT,
	NFS4CLNT_LEASE_CONFIRM,
	NFS4CLNT_SERVER_SCOPE_MISMATCH,
>>>>>>> refs/remotes/origin/cm-10.0
};

enum nfs4_session_state {
	NFS4_SESSION_INITING,
	NFS4_SESSION_DRAINING,
};

<<<<<<< HEAD
=======
#define NFS4_RENEW_TIMEOUT		0x01
#define NFS4_RENEW_DELEGATION_CB	0x02

>>>>>>> refs/remotes/origin/cm-10.0
struct nfs4_minor_version_ops {
	u32	minor_version;

	int	(*call_sync)(struct rpc_clnt *clnt,
			struct nfs_server *server,
			struct rpc_message *msg,
			struct nfs4_sequence_args *args,
			struct nfs4_sequence_res *res,
			int cache_reply);
<<<<<<< HEAD
	int	(*validate_stateid)(struct nfs_delegation *,
			const nfs4_stateid *);
=======
=======
	NFS4CLNT_SESSION_RESET,
	NFS4CLNT_LEASE_CONFIRM,
	NFS4CLNT_SERVER_SCOPE_MISMATCH,
	NFS4CLNT_PURGE_STATE,
	NFS4CLNT_BIND_CONN_TO_SESSION,
	NFS4CLNT_MOVED,
	NFS4CLNT_LEASE_MOVED,
};

#define NFS4_RENEW_TIMEOUT		0x01
#define NFS4_RENEW_DELEGATION_CB	0x02

struct nfs4_minor_version_ops {
	u32	minor_version;
	unsigned init_caps;

	int	(*init_client)(struct nfs_client *);
	void	(*shutdown_client)(struct nfs_client *);
>>>>>>> refs/remotes/origin/master
	bool	(*match_stateid)(const nfs4_stateid *,
			const nfs4_stateid *);
	int	(*find_root_sec)(struct nfs_server *, struct nfs_fh *,
			struct nfs_fsinfo *);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	const struct nfs4_state_recovery_ops *reboot_recovery_ops;
	const struct nfs4_state_recovery_ops *nograce_recovery_ops;
	const struct nfs4_state_maintenance_ops *state_renewal_ops;
};

<<<<<<< HEAD
/*
 * struct rpc_sequence ensures that RPC calls are sent in the exact
 * order that they appear on the list.
 */
struct rpc_sequence {
	struct rpc_wait_queue	wait;	/* RPC call delay queue */
	spinlock_t lock;		/* Protects the list */
	struct list_head list;		/* Defines sequence of RPC calls */
=======
struct nfs_unique_id {
	struct rb_node rb_node;
	__u64 id;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	int	(*free_lock_state)(struct nfs_server *,
			struct nfs4_lock_state *);
	const struct rpc_call_ops *call_sync_ops;
	const struct nfs4_state_recovery_ops *reboot_recovery_ops;
	const struct nfs4_state_recovery_ops *nograce_recovery_ops;
	const struct nfs4_state_maintenance_ops *state_renewal_ops;
	const struct nfs4_mig_recovery_ops *mig_recovery_ops;
>>>>>>> refs/remotes/origin/master
};

#define NFS_SEQID_CONFIRMED 1
struct nfs_seqid_counter {
<<<<<<< HEAD
<<<<<<< HEAD
	struct rpc_sequence *sequence;
	int flags;
	u32 counter;
=======
=======
>>>>>>> refs/remotes/origin/master
	ktime_t create_time;
	int owner_id;
	int flags;
	u32 counter;
	spinlock_t lock;		/* Protects the list */
	struct list_head list;		/* Defines sequence of RPC calls */
	struct rpc_wait_queue	wait;	/* RPC call delay queue */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};

struct nfs_seqid {
	struct nfs_seqid_counter *sequence;
	struct list_head list;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct rpc_task *task;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct rpc_task *task;
>>>>>>> refs/remotes/origin/master
};

static inline void nfs_confirm_seqid(struct nfs_seqid_counter *seqid, int status)
{
	if (seqid_mutating_err(-status))
		seqid->flags |= NFS_SEQID_CONFIRMED;
}

<<<<<<< HEAD
<<<<<<< HEAD
struct nfs_unique_id {
	struct rb_node rb_node;
	__u64 id;
};

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * NFS4 state_owners and lock_owners are simply labels for ordered
 * sequences of RPC calls. Their sole purpose is to provide once-only
 * semantics by allowing the server to identify replayed requests.
 */
struct nfs4_state_owner {
<<<<<<< HEAD
<<<<<<< HEAD
	struct nfs_unique_id so_owner_id;
	struct nfs_server    *so_server;
=======
	struct nfs_server    *so_server;
	struct list_head     so_lru;
	unsigned long        so_expires;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct nfs_server    *so_server;
	struct list_head     so_lru;
	unsigned long        so_expires;
>>>>>>> refs/remotes/origin/master
	struct rb_node	     so_server_node;

	struct rpc_cred	     *so_cred;	 /* Associated cred */

	spinlock_t	     so_lock;
	atomic_t	     so_count;
	unsigned long	     so_flags;
	struct list_head     so_states;
	struct nfs_seqid_counter so_seqid;
<<<<<<< HEAD
<<<<<<< HEAD
	struct rpc_sequence  so_sequence;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	seqcount_t	     so_reclaim_seqcount;
	struct mutex	     so_delegreturn_mutex;
>>>>>>> refs/remotes/origin/master
};

enum {
	NFS_OWNER_RECLAIM_REBOOT,
	NFS_OWNER_RECLAIM_NOGRACE
};

#define NFS_LOCK_NEW		0
#define NFS_LOCK_RECLAIM	1
#define NFS_LOCK_EXPIRED	2

/*
 * struct nfs4_state maintains the client-side state for a given
 * (state_owner,inode) tuple (OPEN) or state_owner (LOCK).
 *
 * OPEN:
 * In order to know when to OPEN_DOWNGRADE or CLOSE the state on the server,
 * we need to know how many files are open for reading or writing on a
 * given inode. This information too is stored here.
 *
 * LOCK: one nfs4_state (LOCK) to hold the lock stateid nfs4_state(OPEN)
 */

struct nfs4_lock_owner {
	unsigned int lo_type;
#define NFS4_ANY_LOCK_TYPE	(0U)
#define NFS4_FLOCK_LOCK_TYPE	(1U << 0)
#define NFS4_POSIX_LOCK_TYPE	(1U << 1)
	union {
		fl_owner_t posix_owner;
		pid_t flock_owner;
	} lo_u;
};

struct nfs4_lock_state {
	struct list_head	ls_locks;	/* Other lock stateids */
	struct nfs4_state *	ls_state;	/* Pointer to open state */
<<<<<<< HEAD
#define NFS_LOCK_INITIALIZED 1
	int			ls_flags;
	struct nfs_seqid_counter	ls_seqid;
<<<<<<< HEAD
	struct rpc_sequence	ls_sequence;
	struct nfs_unique_id	ls_id;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define NFS_LOCK_INITIALIZED 0
#define NFS_LOCK_LOST        1
	unsigned long		ls_flags;
	struct nfs_seqid_counter	ls_seqid;
>>>>>>> refs/remotes/origin/master
	nfs4_stateid		ls_stateid;
	atomic_t		ls_count;
	struct nfs4_lock_owner	ls_owner;
};

/* bits for nfs4_state->flags */
enum {
	LK_STATE_IN_USE,
	NFS_DELEGATED_STATE,		/* Current stateid is delegation */
<<<<<<< HEAD
=======
	NFS_OPEN_STATE,			/* OPEN stateid is set */
>>>>>>> refs/remotes/origin/master
	NFS_O_RDONLY_STATE,		/* OPEN stateid has read-only state */
	NFS_O_WRONLY_STATE,		/* OPEN stateid has write-only state */
	NFS_O_RDWR_STATE,		/* OPEN stateid has read/write state */
	NFS_STATE_RECLAIM_REBOOT,	/* OPEN stateid server rebooted */
	NFS_STATE_RECLAIM_NOGRACE,	/* OPEN stateid needs to recover state */
	NFS_STATE_POSIX_LOCKS,		/* Posix locks are supported */
<<<<<<< HEAD
=======
	NFS_STATE_RECOVERY_FAILED,	/* OPEN stateid state recovery failed */
>>>>>>> refs/remotes/origin/master
};

struct nfs4_state {
	struct list_head open_states;	/* List of states for the same state_owner */
	struct list_head inode_states;	/* List of states for the same inode */
	struct list_head lock_states;	/* List of subservient lock stateids */

	struct nfs4_state_owner *owner;	/* Pointer to the open owner */
	struct inode *inode;		/* Pointer to the inode */

	unsigned long flags;		/* Do we hold any locks? */
	spinlock_t state_lock;		/* Protects the lock_states list */

	seqlock_t seqlock;		/* Protects the stateid/open_stateid */
	nfs4_stateid stateid;		/* Current stateid: may be delegation */
	nfs4_stateid open_stateid;	/* OPEN stateid */

	/* The following 3 fields are protected by owner->so_lock */
	unsigned int n_rdonly;		/* Number of read-only references */
	unsigned int n_wronly;		/* Number of write-only references */
	unsigned int n_rdwr;		/* Number of read/write references */
	fmode_t state;			/* State on the server (R,W, or RW) */
	atomic_t count;
};


struct nfs4_exception {
	long timeout;
	int retry;
	struct nfs4_state *state;
	struct inode *inode;
};

struct nfs4_state_recovery_ops {
	int owner_flag_bit;
	int state_flag_bit;
	int (*recover_open)(struct nfs4_state_owner *, struct nfs4_state *);
	int (*recover_lock)(struct nfs4_state *, struct file_lock *);
	int (*establish_clid)(struct nfs_client *, struct rpc_cred *);
<<<<<<< HEAD
	struct rpc_cred * (*get_clid_cred)(struct nfs_client *);
	int (*reclaim_complete)(struct nfs_client *);
};

struct nfs4_state_maintenance_ops {
<<<<<<< HEAD
	int (*sched_state_renewal)(struct nfs_client *, struct rpc_cred *);
=======
	int (*sched_state_renewal)(struct nfs_client *, struct rpc_cred *, unsigned);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	int (*reclaim_complete)(struct nfs_client *, struct rpc_cred *);
	int (*detect_trunking)(struct nfs_client *, struct nfs_client **,
		struct rpc_cred *);
};

struct nfs4_state_maintenance_ops {
	int (*sched_state_renewal)(struct nfs_client *, struct rpc_cred *, unsigned);
>>>>>>> refs/remotes/origin/master
	struct rpc_cred * (*get_state_renewal_cred_locked)(struct nfs_client *);
	int (*renew_lease)(struct nfs_client *, struct rpc_cred *);
};

<<<<<<< HEAD
extern const struct dentry_operations nfs4_dentry_operations;
extern const struct inode_operations nfs4_dir_inode_operations;

<<<<<<< HEAD
=======
/* nfs4namespace.c */
struct rpc_clnt *nfs4_create_sec_client(struct rpc_clnt *, struct inode *, struct qstr *);

>>>>>>> refs/remotes/origin/cm-10.0
/* nfs4proc.c */
extern int nfs4_proc_setclientid(struct nfs_client *, u32, unsigned short, struct rpc_cred *, struct nfs4_setclientid_res *);
extern int nfs4_proc_setclientid_confirm(struct nfs_client *, struct nfs4_setclientid_res *arg, struct rpc_cred *);
extern int nfs4_proc_exchange_id(struct nfs_client *clp, struct rpc_cred *cred);
<<<<<<< HEAD
extern int nfs4_proc_async_renew(struct nfs_client *, struct rpc_cred *);
extern int nfs4_proc_renew(struct nfs_client *, struct rpc_cred *);
extern int nfs4_init_clientid(struct nfs_client *, struct rpc_cred *);
extern int nfs41_init_clientid(struct nfs_client *, struct rpc_cred *);
extern int nfs4_do_close(struct path *path, struct nfs4_state *state, gfp_t gfp_mask, int wait, bool roc);
extern int nfs4_server_capabilities(struct nfs_server *server, struct nfs_fh *fhandle);
extern int nfs4_proc_fs_locations(struct inode *dir, const struct qstr *name,
		struct nfs4_fs_locations *fs_locations, struct page *page);
extern void nfs4_release_lockowner(const struct nfs4_lock_state *);
=======
extern int nfs4_init_clientid(struct nfs_client *, struct rpc_cred *);
extern int nfs41_init_clientid(struct nfs_client *, struct rpc_cred *);
extern int nfs4_do_close(struct nfs4_state *state, gfp_t gfp_mask, int wait, bool roc);
extern int nfs4_server_capabilities(struct nfs_server *server, struct nfs_fh *fhandle);
extern int nfs4_proc_fs_locations(struct rpc_clnt *, struct inode *, const struct qstr *,
				  struct nfs4_fs_locations *, struct page *);
extern struct rpc_clnt *nfs4_proc_lookup_mountpoint(struct inode *, struct qstr *,
			    struct nfs_fh *, struct nfs_fattr *);
extern int nfs4_proc_secinfo(struct inode *, const struct qstr *, struct nfs4_secinfo_flavors *);
extern int nfs4_release_lockowner(struct nfs4_lock_state *);
>>>>>>> refs/remotes/origin/cm-10.0
extern const struct xattr_handler *nfs4_xattr_handlers[];
=======
struct nfs4_mig_recovery_ops {
	int (*get_locations)(struct inode *, struct nfs4_fs_locations *,
		struct page *, struct rpc_cred *);
	int (*fsid_present)(struct inode *, struct rpc_cred *);
};

extern const struct dentry_operations nfs4_dentry_operations;

/* dir.c */
int nfs_atomic_open(struct inode *, struct dentry *, struct file *,
		    unsigned, umode_t, int *);

/* super.c */
extern struct file_system_type nfs4_fs_type;

/* nfs4namespace.c */
struct rpc_clnt *nfs4_create_sec_client(struct rpc_clnt *, struct inode *, struct qstr *);
struct vfsmount *nfs4_submount(struct nfs_server *, struct dentry *,
			       struct nfs_fh *, struct nfs_fattr *);
int nfs4_replace_transport(struct nfs_server *server,
				const struct nfs4_fs_locations *locations);

/* nfs4proc.c */
extern int nfs4_proc_setclientid(struct nfs_client *, u32, unsigned short, struct rpc_cred *, struct nfs4_setclientid_res *);
extern int nfs4_proc_setclientid_confirm(struct nfs_client *, struct nfs4_setclientid_res *arg, struct rpc_cred *);
extern int nfs4_proc_get_rootfh(struct nfs_server *, struct nfs_fh *, struct nfs_fsinfo *, bool);
extern int nfs4_proc_bind_conn_to_session(struct nfs_client *, struct rpc_cred *cred);
extern int nfs4_proc_exchange_id(struct nfs_client *clp, struct rpc_cred *cred);
extern int nfs4_destroy_clientid(struct nfs_client *clp);
extern int nfs4_init_clientid(struct nfs_client *, struct rpc_cred *);
extern int nfs41_init_clientid(struct nfs_client *, struct rpc_cred *);
extern int nfs4_do_close(struct nfs4_state *state, gfp_t gfp_mask, int wait);
extern int nfs4_server_capabilities(struct nfs_server *server, struct nfs_fh *fhandle);
extern int nfs4_proc_fs_locations(struct rpc_clnt *, struct inode *, const struct qstr *,
				  struct nfs4_fs_locations *, struct page *);
extern int nfs4_proc_get_locations(struct inode *, struct nfs4_fs_locations *,
		struct page *page, struct rpc_cred *);
extern int nfs4_proc_fsid_present(struct inode *, struct rpc_cred *);
extern struct rpc_clnt *nfs4_proc_lookup_mountpoint(struct inode *, struct qstr *,
			    struct nfs_fh *, struct nfs_fattr *);
extern int nfs4_proc_secinfo(struct inode *, const struct qstr *, struct nfs4_secinfo_flavors *);
extern const struct xattr_handler *nfs4_xattr_handlers[];
extern int nfs4_set_rw_stateid(nfs4_stateid *stateid,
		const struct nfs_open_context *ctx,
		const struct nfs_lock_context *l_ctx,
		fmode_t fmode);
>>>>>>> refs/remotes/origin/master

#if defined(CONFIG_NFS_V4_1)
static inline struct nfs4_session *nfs4_get_session(const struct nfs_server *server)
{
	return server->nfs_client->cl_session;
}

<<<<<<< HEAD
<<<<<<< HEAD
extern int nfs4_setup_sequence(const struct nfs_server *server,
		struct nfs4_sequence_args *args, struct nfs4_sequence_res *res,
		int cache_reply, struct rpc_task *task);
extern int nfs41_setup_sequence(struct nfs4_session *session,
		struct nfs4_sequence_args *args, struct nfs4_sequence_res *res,
		int cache_reply, struct rpc_task *task);
=======
extern bool nfs4_set_task_privileged(struct rpc_task *task, void *dummy);
extern int nfs4_setup_sequence(const struct nfs_server *server,
		struct nfs4_sequence_args *args, struct nfs4_sequence_res *res,
		struct rpc_task *task);
extern int nfs41_setup_sequence(struct nfs4_session *session,
		struct nfs4_sequence_args *args, struct nfs4_sequence_res *res,
		struct rpc_task *task);
>>>>>>> refs/remotes/origin/cm-10.0
extern void nfs4_destroy_session(struct nfs4_session *session);
extern struct nfs4_session *nfs4_alloc_session(struct nfs_client *clp);
extern int nfs4_proc_create_session(struct nfs_client *);
extern int nfs4_proc_destroy_session(struct nfs4_session *);
extern int nfs4_init_session(struct nfs_server *server);
=======
extern int nfs41_setup_sequence(struct nfs4_session *session,
		struct nfs4_sequence_args *args, struct nfs4_sequence_res *res,
		struct rpc_task *task);
extern int nfs4_proc_create_session(struct nfs_client *, struct rpc_cred *);
extern int nfs4_proc_destroy_session(struct nfs4_session *, struct rpc_cred *);
>>>>>>> refs/remotes/origin/master
extern int nfs4_proc_get_lease_time(struct nfs_client *clp,
		struct nfs_fsinfo *fsinfo);
extern int nfs4_proc_layoutcommit(struct nfs4_layoutcommit_data *data,
				  bool sync);

static inline bool
is_ds_only_client(struct nfs_client *clp)
{
	return (clp->cl_exchange_flags & EXCHGID4_FLAG_MASK_PNFS) ==
		EXCHGID4_FLAG_USE_PNFS_DS;
}

static inline bool
is_ds_client(struct nfs_client *clp)
{
	return clp->cl_exchange_flags & EXCHGID4_FLAG_USE_PNFS_DS;
}
<<<<<<< HEAD
#else /* CONFIG_NFS_v4_1 */
static inline struct nfs4_session *nfs4_get_session(const struct nfs_server *server)
{
	return NULL;
}

static inline int nfs4_setup_sequence(const struct nfs_server *server,
		struct nfs4_sequence_args *args, struct nfs4_sequence_res *res,
<<<<<<< HEAD
		int cache_reply, struct rpc_task *task)
=======
		struct rpc_task *task)
>>>>>>> refs/remotes/origin/cm-10.0
{
	return 0;
}

static inline int nfs4_init_session(struct nfs_server *server)
{
	return 0;
=======

static inline bool
_nfs4_state_protect(struct nfs_client *clp, unsigned long sp4_mode,
		    struct rpc_clnt **clntp, struct rpc_message *msg)
{
	struct rpc_cred *newcred = NULL;
	rpc_authflavor_t flavor;

	if (test_bit(sp4_mode, &clp->cl_sp4_flags)) {
		spin_lock(&clp->cl_lock);
		if (clp->cl_machine_cred != NULL)
			/* don't call get_rpccred on the machine cred -
			 * a reference will be held for life of clp */
			newcred = clp->cl_machine_cred;
		spin_unlock(&clp->cl_lock);
		msg->rpc_cred = newcred;

		flavor = clp->cl_rpcclient->cl_auth->au_flavor;
		WARN_ON_ONCE(flavor != RPC_AUTH_GSS_KRB5I &&
			     flavor != RPC_AUTH_GSS_KRB5P);
		*clntp = clp->cl_rpcclient;

		return true;
	}
	return false;
}

/*
 * Function responsible for determining if an rpc_message should use the
 * machine cred under SP4_MACH_CRED and if so switching the credential and
 * authflavor (using the nfs_client's rpc_clnt which will be krb5i/p).
 * Should be called before rpc_call_sync/rpc_call_async.
 */
static inline void
nfs4_state_protect(struct nfs_client *clp, unsigned long sp4_mode,
		   struct rpc_clnt **clntp, struct rpc_message *msg)
{
	_nfs4_state_protect(clp, sp4_mode, clntp, msg);
}

/*
 * Special wrapper to nfs4_state_protect for write.
 * If WRITE can use machine cred but COMMIT cannot, make sure all writes
 * that use machine cred use NFS_FILE_SYNC.
 */
static inline void
nfs4_state_protect_write(struct nfs_client *clp, struct rpc_clnt **clntp,
			 struct rpc_message *msg, struct nfs_write_data *wdata)
{
	if (_nfs4_state_protect(clp, NFS_SP4_MACH_CRED_WRITE, clntp, msg) &&
	    !test_bit(NFS_SP4_MACH_CRED_COMMIT, &clp->cl_sp4_flags))
		wdata->args.stable = NFS_FILE_SYNC;
}
#else /* CONFIG_NFS_v4_1 */
static inline struct nfs4_session *nfs4_get_session(const struct nfs_server *server)
{
	return NULL;
>>>>>>> refs/remotes/origin/master
}

static inline bool
is_ds_only_client(struct nfs_client *clp)
{
	return false;
}

static inline bool
is_ds_client(struct nfs_client *clp)
{
	return false;
}
<<<<<<< HEAD
=======

static inline void
nfs4_state_protect(struct nfs_client *clp, unsigned long sp4_flags,
		   struct rpc_clnt **clntp, struct rpc_message *msg)
{
}

static inline void
nfs4_state_protect_write(struct nfs_client *clp, struct rpc_clnt **clntp,
			 struct rpc_message *msg, struct nfs_write_data *wdata)
{
}
>>>>>>> refs/remotes/origin/master
#endif /* CONFIG_NFS_V4_1 */

extern const struct nfs4_minor_version_ops *nfs_v4_minor_ops[];

<<<<<<< HEAD
extern const u32 nfs4_fattr_bitmap[2];
extern const u32 nfs4_statfs_bitmap[2];
extern const u32 nfs4_pathconf_bitmap[2];
<<<<<<< HEAD
extern const u32 nfs4_fsinfo_bitmap[2];
=======
extern const u32 nfs4_fsinfo_bitmap[3];
>>>>>>> refs/remotes/origin/cm-10.0
extern const u32 nfs4_fs_locations_bitmap[2];
=======
extern const u32 nfs4_fattr_bitmap[3];
extern const u32 nfs4_statfs_bitmap[3];
extern const u32 nfs4_pathconf_bitmap[3];
extern const u32 nfs4_fsinfo_bitmap[3];
extern const u32 nfs4_fs_locations_bitmap[3];

void nfs40_shutdown_client(struct nfs_client *);
void nfs41_shutdown_client(struct nfs_client *);
int nfs40_init_client(struct nfs_client *);
int nfs41_init_client(struct nfs_client *);
void nfs4_free_client(struct nfs_client *);

struct nfs_client *nfs4_alloc_client(const struct nfs_client_initdata *);
>>>>>>> refs/remotes/origin/master

/* nfs4renewd.c */
extern void nfs4_schedule_state_renewal(struct nfs_client *);
extern void nfs4_renewd_prepare_shutdown(struct nfs_server *);
extern void nfs4_kill_renewd(struct nfs_client *);
extern void nfs4_renew_state(struct work_struct *);

/* nfs4state.c */
<<<<<<< HEAD
struct rpc_cred *nfs4_get_setclientid_cred(struct nfs_client *clp);
struct rpc_cred *nfs4_get_renew_cred_locked(struct nfs_client *clp);
#if defined(CONFIG_NFS_V4_1)
struct rpc_cred *nfs4_get_machine_cred_locked(struct nfs_client *clp);
struct rpc_cred *nfs4_get_exchange_id_cred(struct nfs_client *clp);
extern void nfs4_schedule_session_recovery(struct nfs4_session *);
#else
static inline void nfs4_schedule_session_recovery(struct nfs4_session *session)
=======
struct rpc_cred *nfs4_get_clid_cred(struct nfs_client *clp);
struct rpc_cred *nfs4_get_machine_cred_locked(struct nfs_client *clp);
struct rpc_cred *nfs4_get_renew_cred_locked(struct nfs_client *clp);
int nfs4_discover_server_trunking(struct nfs_client *clp,
			struct nfs_client **);
int nfs40_discover_server_trunking(struct nfs_client *clp,
			struct nfs_client **, struct rpc_cred *);
#if defined(CONFIG_NFS_V4_1)
int nfs41_discover_server_trunking(struct nfs_client *clp,
			struct nfs_client **, struct rpc_cred *);
extern void nfs4_schedule_session_recovery(struct nfs4_session *, int);
extern void nfs41_server_notify_target_slotid_update(struct nfs_client *clp);
extern void nfs41_server_notify_highest_slotid_update(struct nfs_client *clp);

#else
static inline void nfs4_schedule_session_recovery(struct nfs4_session *session, int err)
>>>>>>> refs/remotes/origin/master
{
}
#endif /* CONFIG_NFS_V4_1 */

<<<<<<< HEAD
<<<<<<< HEAD
extern struct nfs4_state_owner * nfs4_get_state_owner(struct nfs_server *, struct rpc_cred *);
extern void nfs4_put_state_owner(struct nfs4_state_owner *);
extern struct nfs4_state * nfs4_get_open_state(struct inode *, struct nfs4_state_owner *);
extern void nfs4_put_open_state(struct nfs4_state *);
extern void nfs4_close_state(struct path *, struct nfs4_state *, fmode_t);
extern void nfs4_close_sync(struct path *, struct nfs4_state *, fmode_t);
=======
=======
>>>>>>> refs/remotes/origin/master
extern struct nfs4_state_owner *nfs4_get_state_owner(struct nfs_server *, struct rpc_cred *, gfp_t);
extern void nfs4_put_state_owner(struct nfs4_state_owner *);
extern void nfs4_purge_state_owners(struct nfs_server *);
extern struct nfs4_state * nfs4_get_open_state(struct inode *, struct nfs4_state_owner *);
extern void nfs4_put_open_state(struct nfs4_state *);
extern void nfs4_close_state(struct nfs4_state *, fmode_t);
extern void nfs4_close_sync(struct nfs4_state *, fmode_t);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
extern void nfs4_state_set_mode_locked(struct nfs4_state *, fmode_t);
extern void nfs_inode_find_state_and_recover(struct inode *inode,
		const nfs4_stateid *stateid);
extern void nfs4_schedule_lease_recovery(struct nfs_client *);
<<<<<<< HEAD
extern void nfs4_schedule_state_manager(struct nfs_client *);
<<<<<<< HEAD
extern void nfs4_schedule_stateid_recovery(const struct nfs_server *, struct nfs4_state *);
extern void nfs41_handle_sequence_flag_errors(struct nfs_client *clp, u32 flags);
extern void nfs41_handle_recall_slot(struct nfs_client *clp);
extern void nfs4_put_lock_state(struct nfs4_lock_state *lsp);
extern int nfs4_set_lock_state(struct nfs4_state *state, struct file_lock *fl);
extern void nfs4_copy_stateid(nfs4_stateid *, struct nfs4_state *, fl_owner_t, pid_t);
=======
extern void nfs4_schedule_path_down_recovery(struct nfs_client *clp);
extern void nfs4_schedule_stateid_recovery(const struct nfs_server *, struct nfs4_state *);
extern void nfs41_handle_sequence_flag_errors(struct nfs_client *clp, u32 flags);
extern void nfs41_handle_recall_slot(struct nfs_client *clp);
extern void nfs41_handle_server_scope(struct nfs_client *,
				      struct server_scope **);
extern void nfs4_put_lock_state(struct nfs4_lock_state *lsp);
extern int nfs4_set_lock_state(struct nfs4_state *state, struct file_lock *fl);
extern void nfs4_select_rw_stateid(nfs4_stateid *, struct nfs4_state *,
		fmode_t, fl_owner_t, pid_t);
>>>>>>> refs/remotes/origin/cm-10.0
=======
extern int nfs4_wait_clnt_recover(struct nfs_client *clp);
extern int nfs4_client_recover_expired_lease(struct nfs_client *clp);
extern void nfs4_schedule_state_manager(struct nfs_client *);
extern void nfs4_schedule_path_down_recovery(struct nfs_client *clp);
extern int nfs4_schedule_stateid_recovery(const struct nfs_server *, struct nfs4_state *);
extern int nfs4_schedule_migration_recovery(const struct nfs_server *);
extern void nfs4_schedule_lease_moved_recovery(struct nfs_client *);
extern void nfs41_handle_sequence_flag_errors(struct nfs_client *clp, u32 flags);
extern void nfs41_handle_server_scope(struct nfs_client *,
				      struct nfs41_server_scope **);
extern void nfs4_put_lock_state(struct nfs4_lock_state *lsp);
extern int nfs4_set_lock_state(struct nfs4_state *state, struct file_lock *fl);
extern int nfs4_select_rw_stateid(nfs4_stateid *, struct nfs4_state *,
		fmode_t, const struct nfs_lockowner *);
>>>>>>> refs/remotes/origin/master

extern struct nfs_seqid *nfs_alloc_seqid(struct nfs_seqid_counter *counter, gfp_t gfp_mask);
extern int nfs_wait_on_sequence(struct nfs_seqid *seqid, struct rpc_task *task);
extern void nfs_increment_open_seqid(int status, struct nfs_seqid *seqid);
extern void nfs_increment_lock_seqid(int status, struct nfs_seqid *seqid);
extern void nfs_release_seqid(struct nfs_seqid *seqid);
extern void nfs_free_seqid(struct nfs_seqid *seqid);

<<<<<<< HEAD
<<<<<<< HEAD
=======
extern void nfs4_free_lock_state(struct nfs_server *server, struct nfs4_lock_state *lsp);

>>>>>>> refs/remotes/origin/cm-10.0
extern const nfs4_stateid zero_stateid;

=======
extern void nfs4_free_lock_state(struct nfs_server *server, struct nfs4_lock_state *lsp);

extern const nfs4_stateid zero_stateid;

/* nfs4super.c */
struct nfs_mount_info;
extern struct nfs_subversion nfs_v4;
struct dentry *nfs4_try_mount(int, const char *, struct nfs_mount_info *, struct nfs_subversion *);
extern bool nfs4_disable_idmapping;
extern unsigned short max_session_slots;
extern unsigned short send_implementation_id;
extern bool recover_lost_locks;

#define NFS4_CLIENT_ID_UNIQ_LEN		(64)
extern char nfs4_client_id_uniquifier[NFS4_CLIENT_ID_UNIQ_LEN];

/* nfs4sysctl.c */
#ifdef CONFIG_SYSCTL
int nfs4_register_sysctl(void);
void nfs4_unregister_sysctl(void);
#else
static inline int nfs4_register_sysctl(void)
{
	return 0;
}

static inline void nfs4_unregister_sysctl(void)
{
}
#endif

>>>>>>> refs/remotes/origin/master
/* nfs4xdr.c */
extern struct rpc_procinfo nfs4_procedures[];

struct nfs4_mount_data;

/* callback_xdr.c */
extern struct svc_version nfs4_callback_version1;
extern struct svc_version nfs4_callback_version4;

<<<<<<< HEAD
<<<<<<< HEAD
#else

#define nfs4_close_state(a, b, c) do { } while (0)
#define nfs4_close_sync(a, b, c) do { } while (0)
=======
=======
>>>>>>> refs/remotes/origin/master
static inline void nfs4_stateid_copy(nfs4_stateid *dst, const nfs4_stateid *src)
{
	memcpy(dst, src, sizeof(*dst));
}

static inline bool nfs4_stateid_match(const nfs4_stateid *dst, const nfs4_stateid *src)
{
	return memcmp(dst, src, sizeof(*dst)) == 0;
}

<<<<<<< HEAD
=======
static inline bool nfs4_valid_open_stateid(const struct nfs4_state *state)
{
	return test_bit(NFS_STATE_RECOVERY_FAILED, &state->flags) == 0;
}

>>>>>>> refs/remotes/origin/master
#else

#define nfs4_close_state(a, b) do { } while (0)
#define nfs4_close_sync(a, b) do { } while (0)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define nfs4_state_protect(a, b, c, d) do { } while (0)
#define nfs4_state_protect_write(a, b, c, d) do { } while (0)
>>>>>>> refs/remotes/origin/master

#endif /* CONFIG_NFS_V4 */
#endif /* __LINUX_FS_NFS_NFS4_FS.H */
