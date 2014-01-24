#ifndef _NFS_FS_SB
#define _NFS_FS_SB

#include <linux/list.h>
#include <linux/backing-dev.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/idr.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/idr.h>
>>>>>>> refs/remotes/origin/master
#include <linux/wait.h>
#include <linux/nfs_xdr.h>
#include <linux/sunrpc/xprt.h>

<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/atomic.h>
=======
#include <linux/atomic.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/atomic.h>
>>>>>>> refs/remotes/origin/master

struct nfs4_session;
struct nfs_iostats;
struct nlm_host;
struct nfs4_sequence_args;
struct nfs4_sequence_res;
struct nfs_server;
struct nfs4_minor_version_ops;
<<<<<<< HEAD
<<<<<<< HEAD
=======
struct server_scope;
struct nfs41_impl_id;
>>>>>>> refs/remotes/origin/cm-10.0
=======
struct nfs41_server_scope;
struct nfs41_impl_id;
>>>>>>> refs/remotes/origin/master

/*
 * The nfs_client identifies our client state to the server.
 */
struct nfs_client {
	atomic_t		cl_count;
<<<<<<< HEAD
=======
	atomic_t		cl_mds_count;
>>>>>>> refs/remotes/origin/master
	int			cl_cons_state;	/* current construction state (-ve: init error) */
#define NFS_CS_READY		0		/* ready to be used */
#define NFS_CS_INITING		1		/* busy initialising */
#define NFS_CS_SESSION_INITING	2		/* busy initialising  session */
	unsigned long		cl_res_state;	/* NFS resources state */
#define NFS_CS_CALLBACK		1		/* - callback started */
#define NFS_CS_IDMAP		2		/* - idmap started */
#define NFS_CS_RENEWD		3		/* - renewd started */
#define NFS_CS_STOP_RENEW	4		/* no more state to renew */
#define NFS_CS_CHECK_LEASE_TIME	5		/* need to check lease time */
<<<<<<< HEAD
=======
	unsigned long		cl_flags;	/* behavior switches */
#define NFS_CS_NORESVPORT	0		/* - use ephemeral src port */
#define NFS_CS_DISCRTRY		1		/* - disconnect on RPC retry */
#define NFS_CS_MIGRATION	2		/* - transparent state migr */
#define NFS_CS_INFINITE_SLOTS	3		/* - don't limit TCP slots */
#define NFS_CS_NO_RETRANS_TIMEOUT	4	/* - Disable retransmit timeouts */
>>>>>>> refs/remotes/origin/master
	struct sockaddr_storage	cl_addr;	/* server identifier */
	size_t			cl_addrlen;
	char *			cl_hostname;	/* hostname of server */
	struct list_head	cl_share_link;	/* link in global client list */
	struct list_head	cl_superblocks;	/* List of nfs_server structs */

	struct rpc_clnt *	cl_rpcclient;
	const struct nfs_rpc_ops *rpc_ops;	/* NFS protocol vector */
	int			cl_proto;	/* Network transport protocol */
<<<<<<< HEAD
=======
	struct nfs_subversion *	cl_nfs_mod;	/* pointer to nfs version module */
>>>>>>> refs/remotes/origin/master

	u32			cl_minorversion;/* NFSv4 minorversion */
	struct rpc_cred		*cl_machine_cred;

<<<<<<< HEAD
#ifdef CONFIG_NFS_V4
=======
#if IS_ENABLED(CONFIG_NFS_V4)
	struct list_head	cl_ds_clients; /* auth flavor data servers */
>>>>>>> refs/remotes/origin/master
	u64			cl_clientid;	/* constant */
	nfs4_verifier		cl_confirm;	/* Clientid verifier */
	unsigned long		cl_state;

	spinlock_t		cl_lock;

	unsigned long		cl_lease_time;
	unsigned long		cl_last_renewal;
	struct delayed_work	cl_renewd;

	struct rpc_wait_queue	cl_rpcwaitq;

<<<<<<< HEAD
	/* used for the setclientid verifier */
	struct timespec		cl_boot_time;

=======
>>>>>>> refs/remotes/origin/master
	/* idmapper */
	struct idmap *		cl_idmap;

	/* Our own IP address, as a null-terminated string.
<<<<<<< HEAD
	 * This is used to generate the clientid, and the callback address.
	 */
	char			cl_ipaddr[48];
	unsigned char		cl_id_uniquifier;
	u32			cl_cb_ident;	/* v4.0 callback identifier */
	const struct nfs4_minor_version_ops *cl_mvops;
=======
	 * This is used to generate the mv0 callback address.
	 */
	char			cl_ipaddr[48];
	u32			cl_cb_ident;	/* v4.0 callback identifier */
	const struct nfs4_minor_version_ops *cl_mvops;
	unsigned long		cl_mig_gen;

	/* NFSv4.0 transport blocking */
	struct nfs4_slot_table	*cl_slot_tbl;
>>>>>>> refs/remotes/origin/master

	/* The sequence id to use for the next CREATE_SESSION */
	u32			cl_seqid;
	/* The flags used for obtaining the clientid during EXCHANGE_ID */
	u32			cl_exchange_flags;
<<<<<<< HEAD
	struct nfs4_session	*cl_session; 	/* sharred session */
<<<<<<< HEAD
	struct list_head	cl_layouts;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct nfs4_session	*cl_session;	/* shared session */
	bool			cl_preserve_clid;
	struct nfs41_server_owner *cl_serverowner;
	struct nfs41_server_scope *cl_serverscope;
	struct nfs41_impl_id	*cl_implid;
	/* nfs 4.1+ state protection modes: */
	unsigned long		cl_sp4_flags;
#define NFS_SP4_MACH_CRED_MINIMAL  1	/* Minimal sp4_mach_cred - state ops
					 * must use machine cred */
#define NFS_SP4_MACH_CRED_CLEANUP  2	/* CLOSE and LOCKU */
#define NFS_SP4_MACH_CRED_SECINFO  3	/* SECINFO and SECINFO_NO_NAME */
#define NFS_SP4_MACH_CRED_STATEID  4	/* TEST_STATEID and FREE_STATEID */
#define NFS_SP4_MACH_CRED_WRITE    5	/* WRITE */
#define NFS_SP4_MACH_CRED_COMMIT   6	/* COMMIT */
>>>>>>> refs/remotes/origin/master
#endif /* CONFIG_NFS_V4 */

#ifdef CONFIG_NFS_FSCACHE
	struct fscache_cookie	*fscache;	/* client index cache cookie */
#endif
<<<<<<< HEAD
<<<<<<< HEAD
=======

	struct server_scope	*server_scope;	/* from exchange_id */
	struct nfs41_impl_id	*impl_id;	/* from exchange_id */
	struct net		*net;
>>>>>>> refs/remotes/origin/cm-10.0
=======

	struct net		*cl_net;
>>>>>>> refs/remotes/origin/master
};

/*
 * NFS client parameters stored in the superblock.
 */
struct nfs_server {
	struct nfs_client *	nfs_client;	/* shared client and NFS4 state */
	struct list_head	client_link;	/* List of other nfs_server structs
						 * that share the same client
						 */
	struct list_head	master_link;	/* link in master servers list */
	struct rpc_clnt *	client;		/* RPC client handle */
	struct rpc_clnt *	client_acl;	/* ACL RPC client handle */
	struct nlm_host		*nlm_host;	/* NLM client handle */
	struct nfs_iostats __percpu *io_stats;	/* I/O statistics */
	struct backing_dev_info	backing_dev_info;
	atomic_long_t		writeback;	/* number of writeback pages */
	int			flags;		/* various flags */
	unsigned int		caps;		/* server capabilities */
	unsigned int		rsize;		/* read size */
	unsigned int		rpages;		/* read size (in pages) */
	unsigned int		wsize;		/* write size */
	unsigned int		wpages;		/* write size (in pages) */
	unsigned int		wtmult;		/* server disk block size */
	unsigned int		dtsize;		/* readdir size */
	unsigned short		port;		/* "port=" setting */
	unsigned int		bsize;		/* server block size */
	unsigned int		acregmin;	/* attr cache timeouts */
	unsigned int		acregmax;
	unsigned int		acdirmin;
	unsigned int		acdirmax;
	unsigned int		namelen;
	unsigned int		options;	/* extra options enabled by mount */
#define NFS_OPTION_FSCACHE	0x00000001	/* - local caching enabled */
<<<<<<< HEAD
=======
#define NFS_OPTION_MIGRATION	0x00000002	/* - NFSv4 migration enabled */
>>>>>>> refs/remotes/origin/master

	struct nfs_fsid		fsid;
	__u64			maxfilesize;	/* maximum file size */
	struct timespec		time_delta;	/* smallest time granularity */
	unsigned long		mount_time;	/* when this fs was mounted */
<<<<<<< HEAD
	dev_t			s_dev;		/* superblock dev numbers */
=======
	struct super_block	*super;		/* VFS super block */
	dev_t			s_dev;		/* superblock dev numbers */
	struct nfs_auth_info	auth_info;	/* parsed auth flavors */
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_NFS_FSCACHE
	struct nfs_fscache_key	*fscache_key;	/* unique key for superblock */
	struct fscache_cookie	*fscache;	/* superblock cookie */
#endif

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_NFS_V4
	u32			attr_bitmask[2];/* V4 bitmask representing the set
=======
	u32			pnfs_blksize;	/* layout_blksize attr */
#ifdef CONFIG_NFS_V4
	u32			attr_bitmask[3];/* V4 bitmask representing the set
>>>>>>> refs/remotes/origin/cm-10.0
						   of attributes supported on this
						   filesystem */
	u32			cache_consistency_bitmask[2];
=======
	u32			pnfs_blksize;	/* layout_blksize attr */
#if IS_ENABLED(CONFIG_NFS_V4)
	u32			attr_bitmask[3];/* V4 bitmask representing the set
						   of attributes supported on this
						   filesystem */
	u32			attr_bitmask_nl[3];
						/* V4 bitmask representing the
						   set of attributes supported
						   on this filesystem excluding
						   the label support bit. */
	u32			cache_consistency_bitmask[3];
>>>>>>> refs/remotes/origin/master
						/* V4 bitmask representing the subset
						   of change attribute, size, ctime
						   and mtime attributes supported by
						   the server */
	u32			acl_bitmask;	/* V4 bitmask representing the ACEs
						   that are supported on this
						   filesystem */
<<<<<<< HEAD
<<<<<<< HEAD
	struct pnfs_layoutdriver_type  *pnfs_curr_ld; /* Active layout driver */
	struct rpc_wait_queue	roc_rpcwaitq;

	/* the following fields are protected by nfs_client->cl_lock */
	struct rb_root		state_owners;
	struct rb_root		openowner_id;
	struct rb_root		lockowner_id;
#endif
=======
=======
>>>>>>> refs/remotes/origin/master
	u32			fh_expire_type;	/* V4 bitmask representing file
						   handle volatility type for
						   this filesystem */
	struct pnfs_layoutdriver_type  *pnfs_curr_ld; /* Active layout driver */
	struct rpc_wait_queue	roc_rpcwaitq;
	void			*pnfs_ld_data;	/* per mount point data */

	/* the following fields are protected by nfs_client->cl_lock */
	struct rb_root		state_owners;
#endif
	struct ida		openowner_id;
	struct ida		lockowner_id;
	struct list_head	state_owners_lru;
	struct list_head	layouts;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	struct list_head	delegations;
=======
	struct list_head	delegations;

	unsigned long		mig_gen;
	unsigned long		mig_status;
#define NFS_MIG_IN_TRANSITION		(1)
#define NFS_MIG_FAILED			(2)

>>>>>>> refs/remotes/origin/master
	void (*destroy)(struct nfs_server *);

	atomic_t active; /* Keep trace of any activity to this server */

	/* mountd-related mount options */
	struct sockaddr_storage	mountd_address;
	size_t			mountd_addrlen;
	u32			mountd_version;
	unsigned short		mountd_port;
	unsigned short		mountd_protocol;
};

/* Server capabilities */
#define NFS_CAP_READDIRPLUS	(1U << 0)
#define NFS_CAP_HARDLINKS	(1U << 1)
#define NFS_CAP_SYMLINKS	(1U << 2)
#define NFS_CAP_ACLS		(1U << 3)
#define NFS_CAP_ATOMIC_OPEN	(1U << 4)
#define NFS_CAP_CHANGE_ATTR	(1U << 5)
#define NFS_CAP_FILEID		(1U << 6)
#define NFS_CAP_MODE		(1U << 7)
#define NFS_CAP_NLINK		(1U << 8)
#define NFS_CAP_OWNER		(1U << 9)
#define NFS_CAP_OWNER_GROUP	(1U << 10)
#define NFS_CAP_ATIME		(1U << 11)
#define NFS_CAP_CTIME		(1U << 12)
#define NFS_CAP_MTIME		(1U << 13)
#define NFS_CAP_POSIX_LOCK	(1U << 14)
#define NFS_CAP_UIDGID_NOMAP	(1U << 15)
<<<<<<< HEAD


/* maximum number of slots to use */
<<<<<<< HEAD
#define NFS4_MAX_SLOT_TABLE RPC_MAX_SLOT_TABLE
=======
#define NFS4_DEF_SLOT_TABLE_SIZE (16U)
#define NFS4_MAX_SLOT_TABLE (256U)
#define NFS4_NO_SLOT ((u32)-1)
>>>>>>> refs/remotes/origin/cm-10.0

#if defined(CONFIG_NFS_V4)

/* Sessions */
<<<<<<< HEAD
#define SLOT_TABLE_SZ (NFS4_MAX_SLOT_TABLE/(8*sizeof(long)))
=======
#define SLOT_TABLE_SZ DIV_ROUND_UP(NFS4_MAX_SLOT_TABLE, 8*sizeof(long))
>>>>>>> refs/remotes/origin/cm-10.0
struct nfs4_slot_table {
	struct nfs4_slot *slots;		/* seqid per slot */
	unsigned long   used_slots[SLOT_TABLE_SZ]; /* used/unused bitmap */
	spinlock_t	slot_tbl_lock;
	struct rpc_wait_queue	slot_tbl_waitq;	/* allocators may wait here */
<<<<<<< HEAD
	int		max_slots;		/* # slots in table */
	int		highest_used_slotid;	/* sent to server on each SEQ.
						 * op for dynamic resizing */
	int		target_max_slots;	/* Set by CB_RECALL_SLOT as
=======
	u32		max_slots;		/* # slots in table */
	u32		highest_used_slotid;	/* sent to server on each SEQ.
						 * op for dynamic resizing */
	u32		target_max_slots;	/* Set by CB_RECALL_SLOT as
>>>>>>> refs/remotes/origin/cm-10.0
						 * the new max_slots */
	struct completion complete;
};

static inline int slot_idx(struct nfs4_slot_table *tbl, struct nfs4_slot *sp)
{
	return sp - tbl->slots;
}

/*
 * Session related parameters
 */
struct nfs4_session {
	struct nfs4_sessionid		sess_id;
	u32				flags;
	unsigned long			session_state;
	u32				hash_alg;
	u32				ssv_len;

	/* The fore and back channel */
	struct nfs4_channel_attrs	fc_attrs;
	struct nfs4_slot_table		fc_slot_table;
	struct nfs4_channel_attrs	bc_attrs;
	struct nfs4_slot_table		bc_slot_table;
	struct nfs_client		*clp;
};

#endif /* CONFIG_NFS_V4 */
=======
#define NFS_CAP_STATEID_NFSV41	(1U << 16)
#define NFS_CAP_ATOMIC_OPEN_V1	(1U << 17)
#define NFS_CAP_SECURITY_LABEL	(1U << 18)

>>>>>>> refs/remotes/origin/master
#endif
