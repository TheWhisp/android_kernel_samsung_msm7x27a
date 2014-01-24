/*
 * linux/fs/nfs/callback.c
 *
 * Copyright (C) 2004 Trond Myklebust
 *
 * NFSv4 callback handling
 */

#include <linux/completion.h>
#include <linux/ip.h>
#include <linux/module.h>
#include <linux/sunrpc/svc.h>
#include <linux/sunrpc/svcsock.h>
#include <linux/nfs_fs.h>
<<<<<<< HEAD
=======
#include <linux/errno.h>
>>>>>>> refs/remotes/origin/master
#include <linux/mutex.h>
#include <linux/freezer.h>
#include <linux/kthread.h>
#include <linux/sunrpc/svcauth_gss.h>
#include <linux/sunrpc/bc_xprt.h>

#include <net/inet_sock.h>

#include "nfs4_fs.h"
#include "callback.h"
#include "internal.h"
<<<<<<< HEAD
=======
#include "netns.h"
>>>>>>> refs/remotes/origin/master

#define NFSDBG_FACILITY NFSDBG_CALLBACK

struct nfs_callback_data {
	unsigned int users;
	struct svc_serv *serv;
	struct svc_rqst *rqst;
	struct task_struct *task;
};

static struct nfs_callback_data nfs_callback_info[NFS4_MAX_MINOR_VERSION + 1];
static DEFINE_MUTEX(nfs_callback_mutex);
static struct svc_program nfs4_callback_program;

<<<<<<< HEAD
unsigned int nfs_callback_set_tcpport;
unsigned short nfs_callback_tcpport;
unsigned short nfs_callback_tcpport6;
#define NFS_CALLBACK_MAXPORTNR (65535U)

static int param_set_portnr(const char *val, const struct kernel_param *kp)
{
	unsigned long num;
	int ret;

	if (!val)
		return -EINVAL;
	ret = strict_strtoul(val, 0, &num);
	if (ret == -EINVAL || num > NFS_CALLBACK_MAXPORTNR)
		return -EINVAL;
	*((unsigned int *)kp->arg) = num;
	return 0;
}
static struct kernel_param_ops param_ops_portnr = {
	.set = param_set_portnr,
	.get = param_get_uint,
};
#define param_check_portnr(name, p) __param_check(name, p, unsigned int);

module_param_named(callback_tcpport, nfs_callback_set_tcpport, portnr, 0644);
=======
static int nfs4_callback_up_net(struct svc_serv *serv, struct net *net)
{
	int ret;
	struct nfs_net *nn = net_generic(net, nfs_net_id);

	ret = svc_create_xprt(serv, "tcp", net, PF_INET,
				nfs_callback_set_tcpport, SVC_SOCK_ANONYMOUS);
	if (ret <= 0)
		goto out_err;
	nn->nfs_callback_tcpport = ret;
	dprintk("NFS: Callback listener port = %u (af %u, net %p)\n",
			nn->nfs_callback_tcpport, PF_INET, net);

	ret = svc_create_xprt(serv, "tcp", net, PF_INET6,
				nfs_callback_set_tcpport, SVC_SOCK_ANONYMOUS);
	if (ret > 0) {
		nn->nfs_callback_tcpport6 = ret;
		dprintk("NFS: Callback listener port = %u (af %u, net %p)\n",
				nn->nfs_callback_tcpport6, PF_INET6, net);
	} else if (ret != -EAFNOSUPPORT)
		goto out_err;
	return 0;

out_err:
	return (ret) ? ret : -ENOMEM;
}
>>>>>>> refs/remotes/origin/master

/*
 * This is the NFSv4 callback kernel thread.
 */
static int
nfs4_callback_svc(void *vrqstp)
{
<<<<<<< HEAD
	int err, preverr = 0;
=======
	int err;
>>>>>>> refs/remotes/origin/master
	struct svc_rqst *rqstp = vrqstp;

	set_freezable();

	while (!kthread_should_stop()) {
		/*
		 * Listen for a request on the socket
		 */
		err = svc_recv(rqstp, MAX_SCHEDULE_TIMEOUT);
<<<<<<< HEAD
		if (err == -EAGAIN || err == -EINTR) {
			preverr = err;
			continue;
		}
		if (err < 0) {
			if (err != preverr) {
<<<<<<< HEAD
				printk(KERN_WARNING "%s: unexpected error "
=======
				printk(KERN_WARNING "NFS: %s: unexpected error "
>>>>>>> refs/remotes/origin/cm-10.0
					"from svc_recv (%d)\n", __func__, err);
				preverr = err;
			}
			schedule_timeout_uninterruptible(HZ);
			continue;
		}
		preverr = err;
=======
		if (err == -EAGAIN || err == -EINTR)
			continue;
>>>>>>> refs/remotes/origin/master
		svc_process(rqstp);
	}
	return 0;
}

/*
 * Prepare to bring up the NFSv4 callback service
 */
<<<<<<< HEAD
<<<<<<< HEAD
struct svc_rqst *
nfs4_callback_up(struct svc_serv *serv)
=======
static struct svc_rqst *
nfs4_callback_up(struct svc_serv *serv, struct rpc_xprt *xprt)
>>>>>>> refs/remotes/origin/cm-10.0
{
	int ret;

	ret = svc_create_xprt(serv, "tcp", &init_net, PF_INET,
				nfs_callback_set_tcpport, SVC_SOCK_ANONYMOUS);
	if (ret <= 0)
		goto out_err;
	nfs_callback_tcpport = ret;
	dprintk("NFS: Callback listener port = %u (af %u)\n",
			nfs_callback_tcpport, PF_INET);

	ret = svc_create_xprt(serv, "tcp", &init_net, PF_INET6,
				nfs_callback_set_tcpport, SVC_SOCK_ANONYMOUS);
	if (ret > 0) {
		nfs_callback_tcpport6 = ret;
		dprintk("NFS: Callback listener port = %u (af %u)\n",
				nfs_callback_tcpport6, PF_INET6);
	} else if (ret == -EAFNOSUPPORT)
		ret = 0;
	else
		goto out_err;

<<<<<<< HEAD
	return svc_prepare_thread(serv, &serv->sv_pools[0]);
=======
	return svc_prepare_thread(serv, &serv->sv_pools[0], NUMA_NO_NODE);
>>>>>>> refs/remotes/origin/cm-10.0

out_err:
	if (ret == 0)
		ret = -ENOMEM;
	return ERR_PTR(ret);
}

#if defined(CONFIG_NFS_V4_1)
=======
static struct svc_rqst *
nfs4_callback_up(struct svc_serv *serv)
{
	return svc_prepare_thread(serv, &serv->sv_pools[0], NUMA_NO_NODE);
}

#if defined(CONFIG_NFS_V4_1)
static int nfs41_callback_up_net(struct svc_serv *serv, struct net *net)
{
	/*
	 * Create an svc_sock for the back channel service that shares the
	 * fore channel connection.
	 * Returns the input port (0) and sets the svc_serv bc_xprt on success
	 */
	return svc_create_xprt(serv, "tcp-bc", net, PF_INET, 0,
			      SVC_SOCK_ANONYMOUS);
}

>>>>>>> refs/remotes/origin/master
/*
 * The callback service for NFSv4.1 callbacks
 */
static int
nfs41_callback_svc(void *vrqstp)
{
	struct svc_rqst *rqstp = vrqstp;
	struct svc_serv *serv = rqstp->rq_server;
	struct rpc_rqst *req;
	int error;
	DEFINE_WAIT(wq);

	set_freezable();

	while (!kthread_should_stop()) {
<<<<<<< HEAD
=======
		if (try_to_freeze())
			continue;

>>>>>>> refs/remotes/origin/master
		prepare_to_wait(&serv->sv_cb_waitq, &wq, TASK_INTERRUPTIBLE);
		spin_lock_bh(&serv->sv_cb_lock);
		if (!list_empty(&serv->sv_cb_list)) {
			req = list_first_entry(&serv->sv_cb_list,
					struct rpc_rqst, rq_bc_list);
			list_del(&req->rq_bc_list);
			spin_unlock_bh(&serv->sv_cb_lock);
			dprintk("Invoking bc_svc_process()\n");
			error = bc_svc_process(serv, req, rqstp);
			dprintk("bc_svc_process() returned w/ error code= %d\n",
				error);
		} else {
			spin_unlock_bh(&serv->sv_cb_lock);
			schedule();
		}
		finish_wait(&serv->sv_cb_waitq, &wq);
	}
	return 0;
}

/*
 * Bring up the NFSv4.1 callback service
 */
<<<<<<< HEAD
<<<<<<< HEAD
struct svc_rqst *
=======
static struct svc_rqst *
>>>>>>> refs/remotes/origin/cm-10.0
nfs41_callback_up(struct svc_serv *serv, struct rpc_xprt *xprt)
{
	struct svc_rqst *rqstp;
	int ret;

	/*
	 * Create an svc_sock for the back channel service that shares the
	 * fore channel connection.
	 * Returns the input port (0) and sets the svc_serv bc_xprt on success
	 */
	ret = svc_create_xprt(serv, "tcp-bc", &init_net, PF_INET, 0,
			      SVC_SOCK_ANONYMOUS);
	if (ret < 0) {
		rqstp = ERR_PTR(ret);
		goto out;
	}

	/*
	 * Save the svc_serv in the transport so that it can
	 * be referenced when the session backchannel is initialized
	 */
	xprt->bc_serv = serv;
=======
static struct svc_rqst *
nfs41_callback_up(struct svc_serv *serv)
{
	struct svc_rqst *rqstp;
>>>>>>> refs/remotes/origin/master

	INIT_LIST_HEAD(&serv->sv_cb_list);
	spin_lock_init(&serv->sv_cb_lock);
	init_waitqueue_head(&serv->sv_cb_waitq);
<<<<<<< HEAD
<<<<<<< HEAD
	rqstp = svc_prepare_thread(serv, &serv->sv_pools[0]);
=======
	rqstp = svc_prepare_thread(serv, &serv->sv_pools[0], NUMA_NO_NODE);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	rqstp = svc_prepare_thread(serv, &serv->sv_pools[0], NUMA_NO_NODE);
>>>>>>> refs/remotes/origin/master
	if (IS_ERR(rqstp)) {
		svc_xprt_put(serv->sv_bc_xprt);
		serv->sv_bc_xprt = NULL;
	}
<<<<<<< HEAD
out:
	dprintk("--> %s return %ld\n", __func__,
		IS_ERR(rqstp) ? PTR_ERR(rqstp) : 0);
	return rqstp;
}

static inline int nfs_minorversion_callback_svc_setup(u32 minorversion,
		struct svc_serv *serv, struct rpc_xprt *xprt,
		struct svc_rqst **rqstpp, int (**callback_svc)(void *vrqstp))
{
	if (minorversion) {
		*rqstpp = nfs41_callback_up(serv, xprt);
		*callback_svc = nfs41_callback_svc;
	}
	return minorversion;
}

static inline void nfs_callback_bc_serv(u32 minorversion, struct rpc_xprt *xprt,
		struct nfs_callback_data *cb_info)
{
	if (minorversion)
		xprt->bc_serv = cb_info->serv;
}
#else
static inline int nfs_minorversion_callback_svc_setup(u32 minorversion,
		struct svc_serv *serv, struct rpc_xprt *xprt,
		struct svc_rqst **rqstpp, int (**callback_svc)(void *vrqstp))
=======
	dprintk("--> %s return %d\n", __func__, PTR_ERR_OR_ZERO(rqstp));
	return rqstp;
}

static void nfs_minorversion_callback_svc_setup(struct svc_serv *serv,
		struct svc_rqst **rqstpp, int (**callback_svc)(void *vrqstp))
{
	*rqstpp = nfs41_callback_up(serv);
	*callback_svc = nfs41_callback_svc;
}

static inline void nfs_callback_bc_serv(u32 minorversion, struct rpc_xprt *xprt,
		struct svc_serv *serv)
{
	if (minorversion)
		/*
		 * Save the svc_serv in the transport so that it can
		 * be referenced when the session backchannel is initialized
		 */
		xprt->bc_serv = serv;
}
#else
static int nfs41_callback_up_net(struct svc_serv *serv, struct net *net)
>>>>>>> refs/remotes/origin/master
{
	return 0;
}

<<<<<<< HEAD
static inline void nfs_callback_bc_serv(u32 minorversion, struct rpc_xprt *xprt,
		struct nfs_callback_data *cb_info)
=======
static void nfs_minorversion_callback_svc_setup(struct svc_serv *serv,
		struct svc_rqst **rqstpp, int (**callback_svc)(void *vrqstp))
{
	*rqstpp = ERR_PTR(-ENOTSUPP);
	*callback_svc = ERR_PTR(-ENOTSUPP);
}

static inline void nfs_callback_bc_serv(u32 minorversion, struct rpc_xprt *xprt,
		struct svc_serv *serv)
>>>>>>> refs/remotes/origin/master
{
}
#endif /* CONFIG_NFS_V4_1 */

<<<<<<< HEAD
/*
 * Bring up the callback thread if it is not already up.
 */
int nfs_callback_up(u32 minorversion, struct rpc_xprt *xprt)
{
	struct svc_serv *serv = NULL;
	struct svc_rqst *rqstp;
	int (*callback_svc)(void *vrqstp);
	struct nfs_callback_data *cb_info = &nfs_callback_info[minorversion];
	char svc_name[12];
	int ret = 0;
	int minorversion_setup;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct net *net = &init_net;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct net *net = &init_net;
>>>>>>> refs/remotes/origin/cm-11.0

	mutex_lock(&nfs_callback_mutex);
	if (cb_info->users++ || cb_info->task != NULL) {
		nfs_callback_bc_serv(minorversion, xprt, cb_info);
		goto out;
	}
	serv = svc_create(&nfs4_callback_program, NFS4_CALLBACK_BUFSIZE, NULL);
	if (!serv) {
		ret = -ENOMEM;
		goto out_err;
	}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	ret = svc_bind(serv, net);
	if (ret < 0) {
		printk(KERN_WARNING "NFS: bind callback service failed\n");
		goto out_err;
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
	minorversion_setup =  nfs_minorversion_callback_svc_setup(minorversion,
					serv, xprt, &rqstp, &callback_svc);
	if (!minorversion_setup) {
		/* v4.0 callback setup */
<<<<<<< HEAD
		rqstp = nfs4_callback_up(serv);
=======
		rqstp = nfs4_callback_up(serv, xprt);
>>>>>>> refs/remotes/origin/cm-10.0
		callback_svc = nfs4_callback_svc;
	}

	if (IS_ERR(rqstp)) {
		ret = PTR_ERR(rqstp);
		goto out_err;
	}

	svc_sock_update_bufs(serv);

	sprintf(svc_name, "nfsv4.%u-svc", minorversion);
	cb_info->serv = serv;
	cb_info->rqst = rqstp;
	cb_info->task = kthread_run(callback_svc, cb_info->rqst, svc_name);
=======
static int nfs_callback_start_svc(int minorversion, struct rpc_xprt *xprt,
				  struct svc_serv *serv)
{
	struct svc_rqst *rqstp;
	int (*callback_svc)(void *vrqstp);
	struct nfs_callback_data *cb_info = &nfs_callback_info[minorversion];
	int ret;

	nfs_callback_bc_serv(minorversion, xprt, serv);

	if (cb_info->task)
		return 0;

	switch (minorversion) {
	case 0:
		/* v4.0 callback setup */
		rqstp = nfs4_callback_up(serv);
		callback_svc = nfs4_callback_svc;
		break;
	default:
		nfs_minorversion_callback_svc_setup(serv,
				&rqstp, &callback_svc);
	}

	if (IS_ERR(rqstp))
		return PTR_ERR(rqstp);

	svc_sock_update_bufs(serv);

	cb_info->serv = serv;
	cb_info->rqst = rqstp;
	cb_info->task = kthread_run(callback_svc, cb_info->rqst,
				    "nfsv4.%u-svc", minorversion);
>>>>>>> refs/remotes/origin/master
	if (IS_ERR(cb_info->task)) {
		ret = PTR_ERR(cb_info->task);
		svc_exit_thread(cb_info->rqst);
		cb_info->rqst = NULL;
		cb_info->task = NULL;
<<<<<<< HEAD
		goto out_err;
	}
out:
=======
		return ret;
	}
	dprintk("nfs_callback_up: service started\n");
	return 0;
}

static void nfs_callback_down_net(u32 minorversion, struct svc_serv *serv, struct net *net)
{
	struct nfs_net *nn = net_generic(net, nfs_net_id);

	if (--nn->cb_users[minorversion])
		return;

	dprintk("NFS: destroy per-net callback data; net=%p\n", net);
	svc_shutdown_net(serv, net);
}

static int nfs_callback_up_net(int minorversion, struct svc_serv *serv, struct net *net)
{
	struct nfs_net *nn = net_generic(net, nfs_net_id);
	int ret;

	if (nn->cb_users[minorversion]++)
		return 0;

	dprintk("NFS: create per-net callback data; net=%p\n", net);

	ret = svc_bind(serv, net);
	if (ret < 0) {
		printk(KERN_WARNING "NFS: bind callback service failed\n");
		goto err_bind;
	}

	switch (minorversion) {
		case 0:
			ret = nfs4_callback_up_net(serv, net);
			break;
		case 1:
		case 2:
			ret = nfs41_callback_up_net(serv, net);
			break;
		default:
			printk(KERN_ERR "NFS: unknown callback version: %d\n",
					minorversion);
			ret = -EINVAL;
			break;
	}

	if (ret < 0) {
		printk(KERN_ERR "NFS: callback service start failed\n");
		goto err_socks;
	}
	return 0;

err_socks:
	svc_rpcb_cleanup(serv, net);
err_bind:
	dprintk("NFS: Couldn't create callback socket: err = %d; "
			"net = %p\n", ret, net);
	return ret;
}

static struct svc_serv *nfs_callback_create_svc(int minorversion)
{
	struct nfs_callback_data *cb_info = &nfs_callback_info[minorversion];
	struct svc_serv *serv;

	/*
	 * Check whether we're already up and running.
	 */
	if (cb_info->task) {
		/*
		 * Note: increase service usage, because later in case of error
		 * svc_destroy() will be called.
		 */
		svc_get(cb_info->serv);
		return cb_info->serv;
	}

	/*
	 * Sanity check: if there's no task,
	 * we should be the first user ...
	 */
	if (cb_info->users)
		printk(KERN_WARNING "nfs_callback_create_svc: no kthread, %d users??\n",
			cb_info->users);

	serv = svc_create(&nfs4_callback_program, NFS4_CALLBACK_BUFSIZE, NULL);
	if (!serv) {
		printk(KERN_ERR "nfs_callback_create_svc: create service failed\n");
		return ERR_PTR(-ENOMEM);
	}
	/* As there is only one thread we need to over-ride the
	 * default maximum of 80 connections
	 */
	serv->sv_maxconn = 1024;
	dprintk("nfs_callback_create_svc: service created\n");
	return serv;
}

/*
 * Bring up the callback thread if it is not already up.
 */
int nfs_callback_up(u32 minorversion, struct rpc_xprt *xprt)
{
	struct svc_serv *serv;
	struct nfs_callback_data *cb_info = &nfs_callback_info[minorversion];
	int ret;
	struct net *net = xprt->xprt_net;

	mutex_lock(&nfs_callback_mutex);

	serv = nfs_callback_create_svc(minorversion);
	if (IS_ERR(serv)) {
		ret = PTR_ERR(serv);
		goto err_create;
	}

	ret = nfs_callback_up_net(minorversion, serv, net);
	if (ret < 0)
		goto err_net;

	ret = nfs_callback_start_svc(minorversion, xprt, serv);
	if (ret < 0)
		goto err_start;

	cb_info->users++;
>>>>>>> refs/remotes/origin/master
	/*
	 * svc_create creates the svc_serv with sv_nrthreads == 1, and then
	 * svc_prepare_thread increments that. So we need to call svc_destroy
	 * on both success and failure so that the refcount is 1 when the
	 * thread exits.
	 */
<<<<<<< HEAD
	if (serv)
		svc_destroy(serv);
	mutex_unlock(&nfs_callback_mutex);
	return ret;
out_err:
	dprintk("NFS: Couldn't create callback socket or server thread; "
		"err = %d\n", ret);
	cb_info->users--;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (serv)
		svc_shutdown_net(serv, net);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (serv)
		svc_shutdown_net(serv, net);
>>>>>>> refs/remotes/origin/cm-11.0
	goto out;
=======
err_net:
	svc_destroy(serv);
err_create:
	mutex_unlock(&nfs_callback_mutex);
	return ret;

err_start:
	nfs_callback_down_net(minorversion, serv, net);
	dprintk("NFS: Couldn't create server thread; err = %d\n", ret);
	goto err_net;
>>>>>>> refs/remotes/origin/master
}

/*
 * Kill the callback thread if it's no longer being used.
 */
<<<<<<< HEAD
void nfs_callback_down(int minorversion)
=======
void nfs_callback_down(int minorversion, struct net *net)
>>>>>>> refs/remotes/origin/master
{
	struct nfs_callback_data *cb_info = &nfs_callback_info[minorversion];

	mutex_lock(&nfs_callback_mutex);
<<<<<<< HEAD
	cb_info->users--;
	if (cb_info->users == 0 && cb_info->task != NULL) {
		kthread_stop(cb_info->task);
<<<<<<< HEAD
<<<<<<< HEAD
=======
		svc_shutdown_net(cb_info->serv, &init_net);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		svc_shutdown_net(cb_info->serv, &init_net);
>>>>>>> refs/remotes/origin/cm-11.0
		svc_exit_thread(cb_info->rqst);
=======
	nfs_callback_down_net(minorversion, cb_info->serv, net);
	cb_info->users--;
	if (cb_info->users == 0 && cb_info->task != NULL) {
		kthread_stop(cb_info->task);
		dprintk("nfs_callback_down: service stopped\n");
		svc_exit_thread(cb_info->rqst);
		dprintk("nfs_callback_down: service destroyed\n");
>>>>>>> refs/remotes/origin/master
		cb_info->serv = NULL;
		cb_info->rqst = NULL;
		cb_info->task = NULL;
	}
	mutex_unlock(&nfs_callback_mutex);
}

/* Boolean check of RPC_AUTH_GSS principal */
int
check_gss_callback_principal(struct nfs_client *clp, struct svc_rqst *rqstp)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct rpc_clnt *r = clp->cl_rpcclient;
=======
>>>>>>> refs/remotes/origin/cm-10.0
	char *p = svc_gss_principal(rqstp);
=======
	char *p = rqstp->rq_cred.cr_principal;
>>>>>>> refs/remotes/origin/master

	if (rqstp->rq_authop->flavour != RPC_AUTH_GSS)
		return 1;

	/* No RPC_AUTH_GSS on NFSv4.1 back channel yet */
	if (clp->cl_minorversion != 0)
		return 0;
	/*
	 * It might just be a normal user principal, in which case
	 * userspace won't bother to tell us the name at all.
	 */
	if (p == NULL)
		return 0;

	/* Expect a GSS_C_NT_HOSTBASED_NAME like "nfs@serverhostname" */

	if (memcmp(p, "nfs@", 4) != 0)
		return 0;
	p += 4;
<<<<<<< HEAD
<<<<<<< HEAD
	if (strcmp(p, r->cl_server) != 0)
=======
	if (strcmp(p, clp->cl_hostname) != 0)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (strcmp(p, clp->cl_hostname) != 0)
>>>>>>> refs/remotes/origin/master
		return 0;
	return 1;
}

/*
 * pg_authenticate method for nfsv4 callback threads.
 *
 * The authflavor has been negotiated, so an incorrect flavor is a server
 * bug. Drop packets with incorrect authflavor.
 *
 * All other checking done after NFS decoding where the nfs_client can be
 * found in nfs4_callback_compound
 */
static int nfs_callback_authenticate(struct svc_rqst *rqstp)
{
	switch (rqstp->rq_authop->flavour) {
	case RPC_AUTH_NULL:
		if (rqstp->rq_proc != CB_NULL)
			return SVC_DROP;
		break;
	case RPC_AUTH_GSS:
		/* No RPC_AUTH_GSS support yet in NFSv4.1 */
		 if (svc_is_backchannel(rqstp))
			return SVC_DROP;
	}
	return SVC_OK;
}

/*
 * Define NFS4 callback program
 */
static struct svc_version *nfs4_callback_version[] = {
	[1] = &nfs4_callback_version1,
	[4] = &nfs4_callback_version4,
};

static struct svc_stat nfs4_callback_stats;

static struct svc_program nfs4_callback_program = {
	.pg_prog = NFS4_CALLBACK,			/* RPC service number */
	.pg_nvers = ARRAY_SIZE(nfs4_callback_version),	/* Number of entries */
	.pg_vers = nfs4_callback_version,		/* version table */
	.pg_name = "NFSv4 callback",			/* service name */
	.pg_class = "nfs",				/* authentication class */
	.pg_stats = &nfs4_callback_stats,
	.pg_authenticate = nfs_callback_authenticate,
};
