/*
 *  linux/include/linux/sunrpc/clnt.h
 *
 *  Declarations for the high-level RPC client interface
 *
 *  Copyright (C) 1995, 1996, Olaf Kirch <okir@monad.swb.de>
 */

#ifndef _LINUX_SUNRPC_CLNT_H
#define _LINUX_SUNRPC_CLNT_H

<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/types.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/types.h>
>>>>>>> refs/remotes/origin/master
#include <linux/socket.h>
#include <linux/in.h>
#include <linux/in6.h>

#include <linux/sunrpc/msg_prot.h>
#include <linux/sunrpc/sched.h>
#include <linux/sunrpc/xprt.h>
#include <linux/sunrpc/auth.h>
#include <linux/sunrpc/stats.h>
#include <linux/sunrpc/xdr.h>
#include <linux/sunrpc/timer.h>
<<<<<<< HEAD
=======
#include <linux/sunrpc/rpc_pipe_fs.h>
>>>>>>> refs/remotes/origin/master
#include <asm/signal.h>
#include <linux/path.h>
#include <net/ipv6.h>

struct rpc_inode;

/*
 * The high-level client handle
 */
struct rpc_clnt {
	atomic_t		cl_count;	/* Number of references */
<<<<<<< HEAD
	struct list_head	cl_clients;	/* Global list of clients */
	struct list_head	cl_tasks;	/* List of tasks */
	spinlock_t		cl_lock;	/* spinlock */
<<<<<<< HEAD
	struct rpc_xprt *	cl_xprt;	/* transport */
=======
	struct rpc_xprt __rcu *	cl_xprt;	/* transport */
>>>>>>> refs/remotes/origin/cm-10.0
=======
	unsigned int		cl_clid;	/* client id */
	struct list_head	cl_clients;	/* Global list of clients */
	struct list_head	cl_tasks;	/* List of tasks */
	spinlock_t		cl_lock;	/* spinlock */
	struct rpc_xprt __rcu *	cl_xprt;	/* transport */
>>>>>>> refs/remotes/origin/master
	struct rpc_procinfo *	cl_procinfo;	/* procedure info */
	u32			cl_prog,	/* RPC program number */
				cl_vers,	/* RPC version number */
				cl_maxproc;	/* max procedure number */

<<<<<<< HEAD
<<<<<<< HEAD
	char *			cl_server;	/* server machine name */
	char *			cl_protname;	/* protocol name */
=======
	const char *		cl_protname;	/* protocol name */
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	struct rpc_auth *	cl_auth;	/* authenticator */
	struct rpc_stat *	cl_stats;	/* per-program statistics */
	struct rpc_iostats *	cl_metrics;	/* per-client statistics */

	unsigned int		cl_softrtry : 1,/* soft timeouts */
				cl_discrtry : 1,/* disconnect before retry */
<<<<<<< HEAD
=======
				cl_noretranstimeo: 1,/* No retransmit timeouts */
>>>>>>> refs/remotes/origin/master
				cl_autobind : 1,/* use getport() */
				cl_chatty   : 1;/* be verbose */

	struct rpc_rtt *	cl_rtt;		/* RTO estimator data */
	const struct rpc_timeout *cl_timeout;	/* Timeout strategy */

	int			cl_nodelen;	/* nodename length */
	char 			cl_nodename[UNX_MAXNODENAME];
<<<<<<< HEAD
<<<<<<< HEAD
	struct path		cl_path;
	struct rpc_clnt *	cl_parent;	/* Points to parent of clones */
	struct rpc_rtt		cl_rtt_default;
	struct rpc_timeout	cl_timeout_default;
	struct rpc_program *	cl_program;
	char			cl_inline_name[32];
=======
	struct dentry *		cl_dentry;
=======
	struct rpc_pipe_dir_head cl_pipedir_objects;
>>>>>>> refs/remotes/origin/master
	struct rpc_clnt *	cl_parent;	/* Points to parent of clones */
	struct rpc_rtt		cl_rtt_default;
	struct rpc_timeout	cl_timeout_default;
	const struct rpc_program *cl_program;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	char			*cl_principal;	/* target to authenticate to */
=======
>>>>>>> refs/remotes/origin/master
};

/*
 * General RPC program info
 */
#define RPC_MAXVERSION		4
struct rpc_program {
<<<<<<< HEAD
<<<<<<< HEAD
	char *			name;		/* protocol name */
	u32			number;		/* program number */
	unsigned int		nrvers;		/* number of versions */
	struct rpc_version **	version;	/* version array */
	struct rpc_stat *	stats;		/* statistics */
	char *			pipe_dir_name;	/* path to rpc_pipefs dir */
=======
=======
>>>>>>> refs/remotes/origin/master
	const char *		name;		/* protocol name */
	u32			number;		/* program number */
	unsigned int		nrvers;		/* number of versions */
	const struct rpc_version **	version;	/* version array */
	struct rpc_stat *	stats;		/* statistics */
	const char *		pipe_dir_name;	/* path to rpc_pipefs dir */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};

struct rpc_version {
	u32			number;		/* version number */
	unsigned int		nrprocs;	/* number of procs */
	struct rpc_procinfo *	procs;		/* procedure array */
};

/*
 * Procedure information
 */
struct rpc_procinfo {
	u32			p_proc;		/* RPC procedure number */
	kxdreproc_t		p_encode;	/* XDR encode function */
	kxdrdproc_t		p_decode;	/* XDR decode function */
	unsigned int		p_arglen;	/* argument hdr length (u32) */
	unsigned int		p_replen;	/* reply hdr length (u32) */
	unsigned int		p_count;	/* call count */
	unsigned int		p_timer;	/* Which RTT timer to use */
	u32			p_statidx;	/* Which procedure to account */
<<<<<<< HEAD
<<<<<<< HEAD
	char *			p_name;		/* name of procedure */
=======
	const char *		p_name;		/* name of procedure */
>>>>>>> refs/remotes/origin/cm-10.0
=======
	const char *		p_name;		/* name of procedure */
>>>>>>> refs/remotes/origin/master
};

#ifdef __KERNEL__

struct rpc_create_args {
	struct net		*net;
	int			protocol;
	struct sockaddr		*address;
	size_t			addrsize;
	struct sockaddr		*saddress;
	const struct rpc_timeout *timeout;
<<<<<<< HEAD
<<<<<<< HEAD
	char			*servername;
	struct rpc_program	*program;
=======
	const char		*servername;
	const struct rpc_program *program;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	const char		*servername;
	const struct rpc_program *program;
>>>>>>> refs/remotes/origin/master
	u32			prognumber;	/* overrides program->number */
	u32			version;
	rpc_authflavor_t	authflavor;
	unsigned long		flags;
	char			*client_name;
	struct svc_xprt		*bc_xprt;	/* NFSv4.1 backchannel */
};

/* Values for "flags" field */
#define RPC_CLNT_CREATE_HARDRTRY	(1UL << 0)
#define RPC_CLNT_CREATE_AUTOBIND	(1UL << 2)
#define RPC_CLNT_CREATE_NONPRIVPORT	(1UL << 3)
#define RPC_CLNT_CREATE_NOPING		(1UL << 4)
#define RPC_CLNT_CREATE_DISCRTRY	(1UL << 5)
#define RPC_CLNT_CREATE_QUIET		(1UL << 6)
<<<<<<< HEAD

struct rpc_clnt *rpc_create(struct rpc_create_args *args);
struct rpc_clnt	*rpc_bind_new_program(struct rpc_clnt *,
<<<<<<< HEAD
				struct rpc_program *, u32);
=======
				const struct rpc_program *, u32);
>>>>>>> refs/remotes/origin/cm-10.0
void rpc_task_reset_client(struct rpc_task *task, struct rpc_clnt *clnt);
struct rpc_clnt *rpc_clone_client(struct rpc_clnt *);
=======
#define RPC_CLNT_CREATE_INFINITE_SLOTS	(1UL << 7)
#define RPC_CLNT_CREATE_NO_IDLE_TIMEOUT	(1UL << 8)
#define RPC_CLNT_CREATE_NO_RETRANS_TIMEOUT	(1UL << 9)

struct rpc_clnt *rpc_create(struct rpc_create_args *args);
struct rpc_clnt	*rpc_bind_new_program(struct rpc_clnt *,
				const struct rpc_program *, u32);
void rpc_task_reset_client(struct rpc_task *task, struct rpc_clnt *clnt);
struct rpc_clnt *rpc_clone_client(struct rpc_clnt *);
struct rpc_clnt *rpc_clone_client_set_auth(struct rpc_clnt *,
				rpc_authflavor_t);
int		rpc_switch_client_transport(struct rpc_clnt *,
				struct xprt_create *,
				const struct rpc_timeout *);

>>>>>>> refs/remotes/origin/master
void		rpc_shutdown_client(struct rpc_clnt *);
void		rpc_release_client(struct rpc_clnt *);
void		rpc_task_release_client(struct rpc_task *);

<<<<<<< HEAD
<<<<<<< HEAD
int		rpcb_register(u32, u32, int, unsigned short);
int		rpcb_v4_register(const u32 program, const u32 version,
=======
=======
>>>>>>> refs/remotes/origin/master
int		rpcb_create_local(struct net *);
void		rpcb_put_local(struct net *);
int		rpcb_register(struct net *, u32, u32, int, unsigned short);
int		rpcb_v4_register(struct net *net, const u32 program,
				 const u32 version,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				 const struct sockaddr *address,
				 const char *netid);
void		rpcb_getport_async(struct rpc_task *);

void		rpc_call_start(struct rpc_task *);
int		rpc_call_async(struct rpc_clnt *clnt,
			       const struct rpc_message *msg, int flags,
			       const struct rpc_call_ops *tk_ops,
			       void *calldata);
int		rpc_call_sync(struct rpc_clnt *clnt,
			      const struct rpc_message *msg, int flags);
struct rpc_task *rpc_call_null(struct rpc_clnt *clnt, struct rpc_cred *cred,
			       int flags);
int		rpc_restart_call_prepare(struct rpc_task *);
int		rpc_restart_call(struct rpc_task *);
void		rpc_setbufsize(struct rpc_clnt *, unsigned int, unsigned int);
<<<<<<< HEAD
<<<<<<< HEAD
=======
int		rpc_protocol(struct rpc_clnt *);
struct net *	rpc_net_ns(struct rpc_clnt *);
>>>>>>> refs/remotes/origin/cm-10.0
size_t		rpc_max_payload(struct rpc_clnt *);
void		rpc_force_rebind(struct rpc_clnt *);
size_t		rpc_peeraddr(struct rpc_clnt *, struct sockaddr *, size_t);
const char	*rpc_peeraddr2str(struct rpc_clnt *, enum rpc_display_format_t);
<<<<<<< HEAD

size_t		rpc_ntop(const struct sockaddr *, char *, const size_t);
size_t		rpc_pton(const char *, const size_t,
			 struct sockaddr *, const size_t);
char *		rpc_sockaddr2uaddr(const struct sockaddr *);
size_t		rpc_uaddr2sockaddr(const char *, const size_t,
=======
int		rpc_localaddr(struct rpc_clnt *, struct sockaddr *, size_t);

size_t		rpc_ntop(const struct sockaddr *, char *, const size_t);
size_t		rpc_pton(struct net *, const char *, const size_t,
			 struct sockaddr *, const size_t);
char *		rpc_sockaddr2uaddr(const struct sockaddr *, gfp_t);
size_t		rpc_uaddr2sockaddr(struct net *, const char *, const size_t,
>>>>>>> refs/remotes/origin/cm-10.0
				   struct sockaddr *, const size_t);

static inline unsigned short rpc_get_port(const struct sockaddr *sap)
{
	switch (sap->sa_family) {
	case AF_INET:
		return ntohs(((struct sockaddr_in *)sap)->sin_port);
	case AF_INET6:
		return ntohs(((struct sockaddr_in6 *)sap)->sin6_port);
	}
	return 0;
}

static inline void rpc_set_port(struct sockaddr *sap,
				const unsigned short port)
{
	switch (sap->sa_family) {
	case AF_INET:
		((struct sockaddr_in *)sap)->sin_port = htons(port);
		break;
	case AF_INET6:
		((struct sockaddr_in6 *)sap)->sin6_port = htons(port);
		break;
	}
}

#define IPV6_SCOPE_DELIMITER		'%'
#define IPV6_SCOPE_ID_LEN		sizeof("%nnnnnnnnnn")

static inline bool __rpc_cmp_addr4(const struct sockaddr *sap1,
				   const struct sockaddr *sap2)
{
	const struct sockaddr_in *sin1 = (const struct sockaddr_in *)sap1;
	const struct sockaddr_in *sin2 = (const struct sockaddr_in *)sap2;

	return sin1->sin_addr.s_addr == sin2->sin_addr.s_addr;
}

static inline bool __rpc_copy_addr4(struct sockaddr *dst,
				    const struct sockaddr *src)
{
	const struct sockaddr_in *ssin = (struct sockaddr_in *) src;
	struct sockaddr_in *dsin = (struct sockaddr_in *) dst;

	dsin->sin_family = ssin->sin_family;
	dsin->sin_addr.s_addr = ssin->sin_addr.s_addr;
	return true;
}

<<<<<<< HEAD
#if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
=======
#if IS_ENABLED(CONFIG_IPV6)
>>>>>>> refs/remotes/origin/cm-10.0
static inline bool __rpc_cmp_addr6(const struct sockaddr *sap1,
				   const struct sockaddr *sap2)
{
	const struct sockaddr_in6 *sin1 = (const struct sockaddr_in6 *)sap1;
	const struct sockaddr_in6 *sin2 = (const struct sockaddr_in6 *)sap2;
<<<<<<< HEAD
	return ipv6_addr_equal(&sin1->sin6_addr, &sin2->sin6_addr);
=======

	if (!ipv6_addr_equal(&sin1->sin6_addr, &sin2->sin6_addr))
		return false;
	else if (ipv6_addr_type(&sin1->sin6_addr) & IPV6_ADDR_LINKLOCAL)
		return sin1->sin6_scope_id == sin2->sin6_scope_id;

	return true;
>>>>>>> refs/remotes/origin/cm-10.0
}

static inline bool __rpc_copy_addr6(struct sockaddr *dst,
				    const struct sockaddr *src)
{
	const struct sockaddr_in6 *ssin6 = (const struct sockaddr_in6 *) src;
	struct sockaddr_in6 *dsin6 = (struct sockaddr_in6 *) dst;

	dsin6->sin6_family = ssin6->sin6_family;
<<<<<<< HEAD
	ipv6_addr_copy(&dsin6->sin6_addr, &ssin6->sin6_addr);
	return true;
}
#else	/* !(CONFIG_IPV6 || CONFIG_IPV6_MODULE) */
=======
	dsin6->sin6_addr = ssin6->sin6_addr;
	return true;
}
#else	/* !(IS_ENABLED(CONFIG_IPV6) */
>>>>>>> refs/remotes/origin/cm-10.0
static inline bool __rpc_cmp_addr6(const struct sockaddr *sap1,
				   const struct sockaddr *sap2)
{
	return false;
}

static inline bool __rpc_copy_addr6(struct sockaddr *dst,
				    const struct sockaddr *src)
{
	return false;
}
<<<<<<< HEAD
#endif	/* !(CONFIG_IPV6 || CONFIG_IPV6_MODULE) */
=======
#endif	/* !(IS_ENABLED(CONFIG_IPV6) */
>>>>>>> refs/remotes/origin/cm-10.0

/**
 * rpc_cmp_addr - compare the address portion of two sockaddrs.
 * @sap1: first sockaddr
 * @sap2: second sockaddr
 *
 * Just compares the family and address portion. Ignores port, scope, etc.
 * Returns true if the addrs are equal, false if they aren't.
 */
static inline bool rpc_cmp_addr(const struct sockaddr *sap1,
				const struct sockaddr *sap2)
{
	if (sap1->sa_family == sap2->sa_family) {
		switch (sap1->sa_family) {
		case AF_INET:
			return __rpc_cmp_addr4(sap1, sap2);
		case AF_INET6:
			return __rpc_cmp_addr6(sap1, sap2);
		}
	}
	return false;
}

/**
 * rpc_copy_addr - copy the address portion of one sockaddr to another
 * @dst: destination sockaddr
 * @src: source sockaddr
 *
 * Just copies the address portion and family. Ignores port, scope, etc.
 * Caller is responsible for making certain that dst is large enough to hold
 * the address in src. Returns true if address family is supported. Returns
 * false otherwise.
 */
static inline bool rpc_copy_addr(struct sockaddr *dst,
				 const struct sockaddr *src)
{
	switch (src->sa_family) {
	case AF_INET:
		return __rpc_copy_addr4(dst, src);
	case AF_INET6:
		return __rpc_copy_addr6(dst, src);
	}
	return false;
}

/**
 * rpc_get_scope_id - return scopeid for a given sockaddr
 * @sa: sockaddr to get scopeid from
 *
 * Returns the value of the sin6_scope_id for AF_INET6 addrs, or 0 if
 * not an AF_INET6 address.
 */
static inline u32 rpc_get_scope_id(const struct sockaddr *sa)
{
	if (sa->sa_family != AF_INET6)
		return 0;

	return ((struct sockaddr_in6 *) sa)->sin6_scope_id;
}

=======
int		rpc_protocol(struct rpc_clnt *);
struct net *	rpc_net_ns(struct rpc_clnt *);
size_t		rpc_max_payload(struct rpc_clnt *);
unsigned long	rpc_get_timeout(struct rpc_clnt *clnt);
void		rpc_force_rebind(struct rpc_clnt *);
size_t		rpc_peeraddr(struct rpc_clnt *, struct sockaddr *, size_t);
const char	*rpc_peeraddr2str(struct rpc_clnt *, enum rpc_display_format_t);
int		rpc_localaddr(struct rpc_clnt *, struct sockaddr *, size_t);

>>>>>>> refs/remotes/origin/master
#endif /* __KERNEL__ */
#endif /* _LINUX_SUNRPC_CLNT_H */
