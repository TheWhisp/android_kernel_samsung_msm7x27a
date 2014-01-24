/*
 * inet_diag.c	Module for monitoring INET transport protocols sockets.
 *
 * Authors:	Alexey Kuznetsov, <kuznet@ms2.inr.ac.ru>
 *
 *	This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/random.h>
#include <linux/slab.h>
#include <linux/cache.h>
#include <linux/init.h>
#include <linux/time.h>

#include <net/icmp.h>
#include <net/tcp.h>
#include <net/ipv6.h>
#include <net/inet_common.h>
#include <net/inet_connection_sock.h>
#include <net/inet_hashtables.h>
#include <net/inet_timewait_sock.h>
#include <net/inet6_hashtables.h>
#include <net/netlink.h>

#include <linux/inet.h>
#include <linux/stddef.h>

#include <linux/inet_diag.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/sock_diag.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/sock_diag.h>
>>>>>>> refs/remotes/origin/master

static const struct inet_diag_handler **inet_diag_table;

struct inet_diag_entry {
	__be32 *saddr;
	__be32 *daddr;
	u16 sport;
	u16 dport;
	u16 family;
	u16 userlocks;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
};

static struct sock *idiagnl;

=======
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
#if IS_ENABLED(CONFIG_IPV6)
	struct in6_addr saddr_storage;	/* for IPv4-mapped-IPv6 addresses */
	struct in6_addr daddr_storage;	/* for IPv4-mapped-IPv6 addresses */
#endif
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
#define INET_DIAG_PUT(skb, attrtype, attrlen) \
	RTA_DATA(__RTA_PUT(skb, attrtype, attrlen))

static DEFINE_MUTEX(inet_diag_table_mutex);

<<<<<<< HEAD
static const struct inet_diag_handler *inet_diag_lock_handler(int type)
{
	if (!inet_diag_table[type])
		request_module("net-pf-%d-proto-%d-type-%d", PF_NETLINK,
			       NETLINK_INET_DIAG, type);

	mutex_lock(&inet_diag_table_mutex);
	if (!inet_diag_table[type])
		return ERR_PTR(-ENOENT);

	return inet_diag_table[type];
=======
=======
static DEFINE_MUTEX(inet_diag_table_mutex);

>>>>>>> refs/remotes/origin/master
static const struct inet_diag_handler *inet_diag_lock_handler(int proto)
{
	if (!inet_diag_table[proto])
		request_module("net-pf-%d-proto-%d-type-%d-%d", PF_NETLINK,
			       NETLINK_SOCK_DIAG, AF_INET, proto);

	mutex_lock(&inet_diag_table_mutex);
	if (!inet_diag_table[proto])
		return ERR_PTR(-ENOENT);

	return inet_diag_table[proto];
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static inline void inet_diag_unlock_handler(
	const struct inet_diag_handler *handler)
{
	mutex_unlock(&inet_diag_table_mutex);
}

<<<<<<< HEAD
<<<<<<< HEAD
static int inet_csk_diag_fill(struct sock *sk,
			      struct sk_buff *skb,
			      int ext, u32 pid, u32 seq, u16 nlmsg_flags,
			      const struct nlmsghdr *unlh)
{
	const struct inet_sock *inet = inet_sk(sk);
	const struct inet_connection_sock *icsk = inet_csk(sk);
=======
int inet_sk_diag_fill(struct sock *sk, struct inet_connection_sock *icsk,
			      struct sk_buff *skb, struct inet_diag_req_v2 *req,
			      u32 pid, u32 seq, u16 nlmsg_flags,
			      const struct nlmsghdr *unlh)
{
	const struct inet_sock *inet = inet_sk(sk);
>>>>>>> refs/remotes/origin/cm-10.0
	struct inet_diag_msg *r;
	struct nlmsghdr  *nlh;
	void *info = NULL;
	struct inet_diag_meminfo  *minfo = NULL;
	unsigned char	 *b = skb_tail_pointer(skb);
	const struct inet_diag_handler *handler;
<<<<<<< HEAD

	handler = inet_diag_table[unlh->nlmsg_type];
=======
	int ext = req->idiag_ext;

	handler = inet_diag_table[req->sdiag_protocol];
>>>>>>> refs/remotes/origin/cm-10.0
	BUG_ON(handler == NULL);

	nlh = NLMSG_PUT(skb, pid, seq, unlh->nlmsg_type, sizeof(*r));
	nlh->nlmsg_flags = nlmsg_flags;

	r = NLMSG_DATA(nlh);
	BUG_ON(sk->sk_state == TCP_TIME_WAIT);

	if (ext & (1 << (INET_DIAG_MEMINFO - 1)))
		minfo = INET_DIAG_PUT(skb, INET_DIAG_MEMINFO, sizeof(*minfo));

<<<<<<< HEAD
	if (ext & (1 << (INET_DIAG_INFO - 1)))
		info = INET_DIAG_PUT(skb, INET_DIAG_INFO,
				     handler->idiag_info_size);

	if ((ext & (1 << (INET_DIAG_CONG - 1))) && icsk->icsk_ca_ops) {
		const size_t len = strlen(icsk->icsk_ca_ops->name);

		strcpy(INET_DIAG_PUT(skb, INET_DIAG_CONG, len + 1),
		       icsk->icsk_ca_ops->name);
	}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
int inet_sk_diag_fill(struct sock *sk, struct inet_connection_sock *icsk,
			      struct sk_buff *skb, struct inet_diag_req_v2 *req,
			      struct user_namespace *user_ns,		      	
			      u32 portid, u32 seq, u16 nlmsg_flags,
			      const struct nlmsghdr *unlh)
{
	const struct inet_sock *inet = inet_sk(sk);
	struct inet_diag_msg *r;
	struct nlmsghdr  *nlh;
	struct nlattr *attr;
	void *info = NULL;
	const struct inet_diag_handler *handler;
	int ext = req->idiag_ext;

	handler = inet_diag_table[req->sdiag_protocol];
	BUG_ON(handler == NULL);

	nlh = nlmsg_put(skb, portid, seq, unlh->nlmsg_type, sizeof(*r),
			nlmsg_flags);
	if (!nlh)
		return -EMSGSIZE;

	r = nlmsg_data(nlh);
	BUG_ON(sk->sk_state == TCP_TIME_WAIT);

>>>>>>> refs/remotes/origin/master
	r->idiag_family = sk->sk_family;
	r->idiag_state = sk->sk_state;
	r->idiag_timer = 0;
	r->idiag_retrans = 0;

	r->id.idiag_if = sk->sk_bound_dev_if;
<<<<<<< HEAD
<<<<<<< HEAD
	r->id.idiag_cookie[0] = (u32)(unsigned long)sk;
	r->id.idiag_cookie[1] = (u32)(((unsigned long)sk >> 31) >> 1);
=======
	sock_diag_save_cookie(sk, r->id.idiag_cookie);
>>>>>>> refs/remotes/origin/cm-10.0

	r->id.idiag_sport = inet->inet_sport;
	r->id.idiag_dport = inet->inet_dport;
	r->id.idiag_src[0] = inet->inet_rcv_saddr;
	r->id.idiag_dst[0] = inet->inet_daddr;

<<<<<<< HEAD
#if defined(CONFIG_IPV6) || defined (CONFIG_IPV6_MODULE)
	if (r->idiag_family == AF_INET6) {
		const struct ipv6_pinfo *np = inet6_sk(sk);

		ipv6_addr_copy((struct in6_addr *)r->id.idiag_src,
			       &np->rcv_saddr);
		ipv6_addr_copy((struct in6_addr *)r->id.idiag_dst,
			       &np->daddr);
	}
#endif

=======
=======
	sock_diag_save_cookie(sk, r->id.idiag_cookie);

	r->id.idiag_sport = inet->inet_sport;
	r->id.idiag_dport = inet->inet_dport;

	memset(&r->id.idiag_src, 0, sizeof(r->id.idiag_src));
	memset(&r->id.idiag_dst, 0, sizeof(r->id.idiag_dst));

	r->id.idiag_src[0] = inet->inet_rcv_saddr;
	r->id.idiag_dst[0] = inet->inet_daddr;

	if (nla_put_u8(skb, INET_DIAG_SHUTDOWN, sk->sk_shutdown))
		goto errout;

>>>>>>> refs/remotes/origin/master
	/* IPv6 dual-stack sockets use inet->tos for IPv4 connections,
	 * hence this needs to be included regardless of socket family.
	 */
	if (ext & (1 << (INET_DIAG_TOS - 1)))
<<<<<<< HEAD
		RTA_PUT_U8(skb, INET_DIAG_TOS, inet->tos);

#if IS_ENABLED(CONFIG_IPV6)
	if (r->idiag_family == AF_INET6) {
		const struct ipv6_pinfo *np = inet6_sk(sk);

		*(struct in6_addr *)r->id.idiag_src = np->rcv_saddr;
		*(struct in6_addr *)r->id.idiag_dst = np->daddr;
		if (ext & (1 << (INET_DIAG_TCLASS - 1)))
			RTA_PUT_U8(skb, INET_DIAG_TCLASS, np->tclass);
	}
#endif

	r->idiag_uid = sock_i_uid(sk);
	r->idiag_inode = sock_i_ino(sk);

	if (minfo) {
		minfo->idiag_rmem = sk_rmem_alloc_get(sk);
		minfo->idiag_wmem = sk->sk_wmem_queued;
		minfo->idiag_fmem = sk->sk_forward_alloc;
		minfo->idiag_tmem = sk_wmem_alloc_get(sk);
=======
		if (nla_put_u8(skb, INET_DIAG_TOS, inet->tos) < 0)
			goto errout;

#if IS_ENABLED(CONFIG_IPV6)
	if (r->idiag_family == AF_INET6) {

		*(struct in6_addr *)r->id.idiag_src = sk->sk_v6_rcv_saddr;
		*(struct in6_addr *)r->id.idiag_dst = sk->sk_v6_daddr;

		if (ext & (1 << (INET_DIAG_TCLASS - 1)))
			if (nla_put_u8(skb, INET_DIAG_TCLASS,
				       inet6_sk(sk)->tclass) < 0)
				goto errout;
	}
#endif

	r->idiag_uid = from_kuid_munged(user_ns, sock_i_uid(sk));
	r->idiag_inode = sock_i_ino(sk);

	if (ext & (1 << (INET_DIAG_MEMINFO - 1))) {
		struct inet_diag_meminfo minfo = {
			.idiag_rmem = sk_rmem_alloc_get(sk),
			.idiag_wmem = sk->sk_wmem_queued,
			.idiag_fmem = sk->sk_forward_alloc,
			.idiag_tmem = sk_wmem_alloc_get(sk),
		};

		if (nla_put(skb, INET_DIAG_MEMINFO, sizeof(minfo), &minfo) < 0)
			goto errout;
>>>>>>> refs/remotes/origin/master
	}

	if (ext & (1 << (INET_DIAG_SKMEMINFO - 1)))
		if (sock_diag_put_meminfo(sk, skb, INET_DIAG_SKMEMINFO))
<<<<<<< HEAD
			goto rtattr_failure;
=======
			goto errout;
>>>>>>> refs/remotes/origin/master

	if (icsk == NULL) {
		handler->idiag_get_info(sk, r, NULL);
		goto out;
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
#define EXPIRES_IN_MS(tmo)  DIV_ROUND_UP((tmo - jiffies) * 1000, HZ)

	if (icsk->icsk_pending == ICSK_TIME_RETRANS) {
=======
#define EXPIRES_IN_MS(tmo)  DIV_ROUND_UP((tmo - jiffies) * 1000, HZ)

	if (icsk->icsk_pending == ICSK_TIME_RETRANS ||
	    icsk->icsk_pending == ICSK_TIME_EARLY_RETRANS ||
	    icsk->icsk_pending == ICSK_TIME_LOSS_PROBE) {
>>>>>>> refs/remotes/origin/master
		r->idiag_timer = 1;
		r->idiag_retrans = icsk->icsk_retransmits;
		r->idiag_expires = EXPIRES_IN_MS(icsk->icsk_timeout);
	} else if (icsk->icsk_pending == ICSK_TIME_PROBE0) {
		r->idiag_timer = 4;
		r->idiag_retrans = icsk->icsk_probes_out;
		r->idiag_expires = EXPIRES_IN_MS(icsk->icsk_timeout);
	} else if (timer_pending(&sk->sk_timer)) {
		r->idiag_timer = 2;
		r->idiag_retrans = icsk->icsk_probes_out;
		r->idiag_expires = EXPIRES_IN_MS(sk->sk_timer.expires);
	} else {
		r->idiag_timer = 0;
		r->idiag_expires = 0;
	}
#undef EXPIRES_IN_MS

<<<<<<< HEAD
<<<<<<< HEAD
	r->idiag_uid = sock_i_uid(sk);
	r->idiag_inode = sock_i_ino(sk);

	if (minfo) {
		minfo->idiag_rmem = sk_rmem_alloc_get(sk);
		minfo->idiag_wmem = sk->sk_wmem_queued;
		minfo->idiag_fmem = sk->sk_forward_alloc;
		minfo->idiag_tmem = sk_wmem_alloc_get(sk);
=======
	if (ext & (1 << (INET_DIAG_INFO - 1)))
		info = INET_DIAG_PUT(skb, INET_DIAG_INFO, sizeof(struct tcp_info));

	if ((ext & (1 << (INET_DIAG_CONG - 1))) && icsk->icsk_ca_ops) {
		const size_t len = strlen(icsk->icsk_ca_ops->name);

		strcpy(INET_DIAG_PUT(skb, INET_DIAG_CONG, len + 1),
		       icsk->icsk_ca_ops->name);
>>>>>>> refs/remotes/origin/cm-10.0
	}

=======
	if (ext & (1 << (INET_DIAG_INFO - 1))) {
		attr = nla_reserve(skb, INET_DIAG_INFO,
				   sizeof(struct tcp_info));
		if (!attr)
			goto errout;

		info = nla_data(attr);
	}

	if ((ext & (1 << (INET_DIAG_CONG - 1))) && icsk->icsk_ca_ops)
		if (nla_put_string(skb, INET_DIAG_CONG,
				   icsk->icsk_ca_ops->name) < 0)
			goto errout;

>>>>>>> refs/remotes/origin/master
	handler->idiag_get_info(sk, r, info);

	if (sk->sk_state < TCP_TIME_WAIT &&
	    icsk->icsk_ca_ops && icsk->icsk_ca_ops->get_info)
		icsk->icsk_ca_ops->get_info(sk, ext, skb);

<<<<<<< HEAD
<<<<<<< HEAD
=======
out:
>>>>>>> refs/remotes/origin/cm-10.0
	nlh->nlmsg_len = skb_tail_pointer(skb) - b;
	return skb->len;

rtattr_failure:
nlmsg_failure:
	nlmsg_trim(skb, b);
	return -EMSGSIZE;
}
<<<<<<< HEAD

static int inet_twsk_diag_fill(struct inet_timewait_sock *tw,
			       struct sk_buff *skb, int ext, u32 pid,
			       u32 seq, u16 nlmsg_flags,
=======
=======
out:
	return nlmsg_end(skb, nlh);

errout:
	nlmsg_cancel(skb, nlh);
	return -EMSGSIZE;
}
>>>>>>> refs/remotes/origin/master
EXPORT_SYMBOL_GPL(inet_sk_diag_fill);

static int inet_csk_diag_fill(struct sock *sk,
			      struct sk_buff *skb, struct inet_diag_req_v2 *req,
<<<<<<< HEAD
			      u32 pid, u32 seq, u16 nlmsg_flags,
			      const struct nlmsghdr *unlh)
{
	return inet_sk_diag_fill(sk, inet_csk(sk),
			skb, req, pid, seq, nlmsg_flags, unlh);
=======
			      struct user_namespace *user_ns,
			      u32 portid, u32 seq, u16 nlmsg_flags,
			      const struct nlmsghdr *unlh)
{
	return inet_sk_diag_fill(sk, inet_csk(sk),
			skb, req, user_ns, portid, seq, nlmsg_flags, unlh);
>>>>>>> refs/remotes/origin/master
}

static int inet_twsk_diag_fill(struct inet_timewait_sock *tw,
			       struct sk_buff *skb, struct inet_diag_req_v2 *req,
<<<<<<< HEAD
			       u32 pid, u32 seq, u16 nlmsg_flags,
>>>>>>> refs/remotes/origin/cm-10.0
			       const struct nlmsghdr *unlh)
{
	long tmo;
	struct inet_diag_msg *r;
	const unsigned char *previous_tail = skb_tail_pointer(skb);
	struct nlmsghdr *nlh = NLMSG_PUT(skb, pid, seq,
					 unlh->nlmsg_type, sizeof(*r));

	r = NLMSG_DATA(nlh);
	BUG_ON(tw->tw_state != TCP_TIME_WAIT);

	nlh->nlmsg_flags = nlmsg_flags;

	tmo = tw->tw_ttd - jiffies;
=======
			       u32 portid, u32 seq, u16 nlmsg_flags,
			       const struct nlmsghdr *unlh)
{
	s32 tmo;
	struct inet_diag_msg *r;
	struct nlmsghdr *nlh;

	nlh = nlmsg_put(skb, portid, seq, unlh->nlmsg_type, sizeof(*r),
			nlmsg_flags);
	if (!nlh)
		return -EMSGSIZE;

	r = nlmsg_data(nlh);
	BUG_ON(tw->tw_state != TCP_TIME_WAIT);

	tmo = tw->tw_ttd - inet_tw_time_stamp();
>>>>>>> refs/remotes/origin/master
	if (tmo < 0)
		tmo = 0;

	r->idiag_family	      = tw->tw_family;
	r->idiag_retrans      = 0;
<<<<<<< HEAD
	r->id.idiag_if	      = tw->tw_bound_dev_if;
<<<<<<< HEAD
	r->id.idiag_cookie[0] = (u32)(unsigned long)tw;
	r->id.idiag_cookie[1] = (u32)(((unsigned long)tw >> 31) >> 1);
=======
	sock_diag_save_cookie(tw, r->id.idiag_cookie);
>>>>>>> refs/remotes/origin/cm-10.0
	r->id.idiag_sport     = tw->tw_sport;
	r->id.idiag_dport     = tw->tw_dport;
	r->id.idiag_src[0]    = tw->tw_rcv_saddr;
	r->id.idiag_dst[0]    = tw->tw_daddr;
	r->idiag_state	      = tw->tw_substate;
	r->idiag_timer	      = 3;
	r->idiag_expires      = DIV_ROUND_UP(tmo * 1000, HZ);
=======

	r->id.idiag_if	      = tw->tw_bound_dev_if;
	sock_diag_save_cookie(tw, r->id.idiag_cookie);

	r->id.idiag_sport     = tw->tw_sport;
	r->id.idiag_dport     = tw->tw_dport;

	memset(&r->id.idiag_src, 0, sizeof(r->id.idiag_src));
	memset(&r->id.idiag_dst, 0, sizeof(r->id.idiag_dst));

	r->id.idiag_src[0]    = tw->tw_rcv_saddr;
	r->id.idiag_dst[0]    = tw->tw_daddr;

	r->idiag_state	      = tw->tw_substate;
	r->idiag_timer	      = 3;
	r->idiag_expires      = jiffies_to_msecs(tmo);
>>>>>>> refs/remotes/origin/master
	r->idiag_rqueue	      = 0;
	r->idiag_wqueue	      = 0;
	r->idiag_uid	      = 0;
	r->idiag_inode	      = 0;
<<<<<<< HEAD
<<<<<<< HEAD
#if defined(CONFIG_IPV6) || defined (CONFIG_IPV6_MODULE)
=======
#if IS_ENABLED(CONFIG_IPV6)
>>>>>>> refs/remotes/origin/cm-10.0
	if (tw->tw_family == AF_INET6) {
		const struct inet6_timewait_sock *tw6 =
						inet6_twsk((struct sock *)tw);

<<<<<<< HEAD
		ipv6_addr_copy((struct in6_addr *)r->id.idiag_src,
			       &tw6->tw_v6_rcv_saddr);
		ipv6_addr_copy((struct in6_addr *)r->id.idiag_dst,
			       &tw6->tw_v6_daddr);
=======
		*(struct in6_addr *)r->id.idiag_src = tw6->tw_v6_rcv_saddr;
		*(struct in6_addr *)r->id.idiag_dst = tw6->tw_v6_daddr;
>>>>>>> refs/remotes/origin/cm-10.0
	}
#endif
	nlh->nlmsg_len = skb_tail_pointer(skb) - previous_tail;
	return skb->len;
nlmsg_failure:
	nlmsg_trim(skb, previous_tail);
	return -EMSGSIZE;
}

static int sk_diag_fill(struct sock *sk, struct sk_buff *skb,
<<<<<<< HEAD
			int ext, u32 pid, u32 seq, u16 nlmsg_flags,
=======
			struct inet_diag_req_v2 *r, u32 pid, u32 seq, u16 nlmsg_flags,
>>>>>>> refs/remotes/origin/cm-10.0
			const struct nlmsghdr *unlh)
{
	if (sk->sk_state == TCP_TIME_WAIT)
		return inet_twsk_diag_fill((struct inet_timewait_sock *)sk,
<<<<<<< HEAD
					   skb, ext, pid, seq, nlmsg_flags,
					   unlh);
	return inet_csk_diag_fill(sk, skb, ext, pid, seq, nlmsg_flags, unlh);
}

static int inet_diag_get_exact(struct sk_buff *in_skb,
			       const struct nlmsghdr *nlh)
{
	int err;
	struct sock *sk;
	struct inet_diag_req *req = NLMSG_DATA(nlh);
	struct sk_buff *rep;
	struct inet_hashinfo *hashinfo;
	const struct inet_diag_handler *handler;

	handler = inet_diag_lock_handler(nlh->nlmsg_type);
	if (IS_ERR(handler)) {
		err = PTR_ERR(handler);
		goto unlock;
	}

	hashinfo = handler->idiag_hashinfo;
	err = -EINVAL;

	if (req->idiag_family == AF_INET) {
=======
					   skb, r, pid, seq, nlmsg_flags,
					   unlh);
	return inet_csk_diag_fill(sk, skb, r, pid, seq, nlmsg_flags, unlh);
=======
#if IS_ENABLED(CONFIG_IPV6)
	if (tw->tw_family == AF_INET6) {
		*(struct in6_addr *)r->id.idiag_src = tw->tw_v6_rcv_saddr;
		*(struct in6_addr *)r->id.idiag_dst = tw->tw_v6_daddr;
	}
#endif

	return nlmsg_end(skb, nlh);
}

static int sk_diag_fill(struct sock *sk, struct sk_buff *skb,
			struct inet_diag_req_v2 *r,
			struct user_namespace *user_ns,
			u32 portid, u32 seq, u16 nlmsg_flags,
			const struct nlmsghdr *unlh)
{
	if (sk->sk_state == TCP_TIME_WAIT)
		return inet_twsk_diag_fill(inet_twsk(sk), skb, r, portid, seq,
					   nlmsg_flags, unlh);

	return inet_csk_diag_fill(sk, skb, r, user_ns, portid, seq,
				  nlmsg_flags, unlh);
>>>>>>> refs/remotes/origin/master
}

int inet_diag_dump_one_icsk(struct inet_hashinfo *hashinfo, struct sk_buff *in_skb,
		const struct nlmsghdr *nlh, struct inet_diag_req_v2 *req)
{
	int err;
	struct sock *sk;
	struct sk_buff *rep;
<<<<<<< HEAD

	err = -EINVAL;
	if (req->sdiag_family == AF_INET) {
>>>>>>> refs/remotes/origin/cm-10.0
		sk = inet_lookup(&init_net, hashinfo, req->id.idiag_dst[0],
				 req->id.idiag_dport, req->id.idiag_src[0],
				 req->id.idiag_sport, req->id.idiag_if);
	}
<<<<<<< HEAD
#if defined(CONFIG_IPV6) || defined (CONFIG_IPV6_MODULE)
	else if (req->idiag_family == AF_INET6) {
=======
#if IS_ENABLED(CONFIG_IPV6)
	else if (req->sdiag_family == AF_INET6) {
>>>>>>> refs/remotes/origin/cm-10.0
		sk = inet6_lookup(&init_net, hashinfo,
=======
	struct net *net = sock_net(in_skb->sk);

	err = -EINVAL;
	if (req->sdiag_family == AF_INET) {
		sk = inet_lookup(net, hashinfo, req->id.idiag_dst[0],
				 req->id.idiag_dport, req->id.idiag_src[0],
				 req->id.idiag_sport, req->id.idiag_if);
	}
#if IS_ENABLED(CONFIG_IPV6)
	else if (req->sdiag_family == AF_INET6) {
		sk = inet6_lookup(net, hashinfo,
>>>>>>> refs/remotes/origin/master
				  (struct in6_addr *)req->id.idiag_dst,
				  req->id.idiag_dport,
				  (struct in6_addr *)req->id.idiag_src,
				  req->id.idiag_sport,
				  req->id.idiag_if);
	}
#endif
	else {
<<<<<<< HEAD
<<<<<<< HEAD
		goto unlock;
=======
		goto out_nosk;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		goto out_nosk;
>>>>>>> refs/remotes/origin/master
	}

	err = -ENOENT;
	if (sk == NULL)
<<<<<<< HEAD
<<<<<<< HEAD
		goto unlock;

	err = -ESTALE;
	if ((req->id.idiag_cookie[0] != INET_DIAG_NOCOOKIE ||
	     req->id.idiag_cookie[1] != INET_DIAG_NOCOOKIE) &&
	    ((u32)(unsigned long)sk != req->id.idiag_cookie[0] ||
	     (u32)((((unsigned long)sk) >> 31) >> 1) != req->id.idiag_cookie[1]))
=======
=======
>>>>>>> refs/remotes/origin/master
		goto out_nosk;

	err = sock_diag_check_cookie(sk, req->id.idiag_cookie);
	if (err)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		goto out;

	err = -ENOMEM;
	rep = alloc_skb(NLMSG_SPACE((sizeof(struct inet_diag_msg) +
				     sizeof(struct inet_diag_meminfo) +
<<<<<<< HEAD
				     handler->idiag_info_size + 64)),
=======
				     sizeof(struct tcp_info) + 64)),
>>>>>>> refs/remotes/origin/cm-10.0
			GFP_KERNEL);
	if (!rep)
		goto out;

<<<<<<< HEAD
	err = sk_diag_fill(sk, rep, req->idiag_ext,
=======
	err = sk_diag_fill(sk, rep, req,
>>>>>>> refs/remotes/origin/cm-10.0
			   NETLINK_CB(in_skb).pid,
			   nlh->nlmsg_seq, 0, nlh);
	if (err < 0) {
		WARN_ON(err == -EMSGSIZE);
		kfree_skb(rep);
		goto out;
	}
<<<<<<< HEAD
	err = netlink_unicast(idiagnl, rep, NETLINK_CB(in_skb).pid,
=======
	err = netlink_unicast(sock_diag_nlsk, rep, NETLINK_CB(in_skb).pid,
>>>>>>> refs/remotes/origin/cm-10.0
=======
		goto out;

	rep = nlmsg_new(sizeof(struct inet_diag_msg) +
			sizeof(struct inet_diag_meminfo) +
			sizeof(struct tcp_info) + 64, GFP_KERNEL);
	if (!rep) {
		err = -ENOMEM;
		goto out;
	}

	err = sk_diag_fill(sk, rep, req,
			   sk_user_ns(NETLINK_CB(in_skb).sk),
			   NETLINK_CB(in_skb).portid,
			   nlh->nlmsg_seq, 0, nlh);
	if (err < 0) {
		WARN_ON(err == -EMSGSIZE);
		nlmsg_free(rep);
		goto out;
	}
	err = netlink_unicast(net->diag_nlsk, rep, NETLINK_CB(in_skb).portid,
>>>>>>> refs/remotes/origin/master
			      MSG_DONTWAIT);
	if (err > 0)
		err = 0;

out:
<<<<<<< HEAD
	if (sk) {
		if (sk->sk_state == TCP_TIME_WAIT)
			inet_twsk_put((struct inet_timewait_sock *)sk);
		else
			sock_put(sk);
	}
<<<<<<< HEAD
unlock:
	inet_diag_unlock_handler(handler);
=======
=======
	if (sk)
		sock_gen_put(sk);

>>>>>>> refs/remotes/origin/master
out_nosk:
	return err;
}
EXPORT_SYMBOL_GPL(inet_diag_dump_one_icsk);

static int inet_diag_get_exact(struct sk_buff *in_skb,
			       const struct nlmsghdr *nlh,
			       struct inet_diag_req_v2 *req)
{
	const struct inet_diag_handler *handler;
	int err;

	handler = inet_diag_lock_handler(req->sdiag_protocol);
	if (IS_ERR(handler))
		err = PTR_ERR(handler);
	else
		err = handler->dump_one(in_skb, nlh, req);
	inet_diag_unlock_handler(handler);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return err;
}

static int bitstring_match(const __be32 *a1, const __be32 *a2, int bits)
{
	int words = bits >> 5;

	bits &= 0x1f;

	if (words) {
		if (memcmp(a1, a2, words << 2))
			return 0;
	}
	if (bits) {
		__be32 w1, w2;
		__be32 mask;

		w1 = a1[words];
		w2 = a2[words];

		mask = htonl((0xffffffff) << (32 - bits));

		if ((w1 ^ w2) & mask)
			return 0;
	}

	return 1;
}


<<<<<<< HEAD
<<<<<<< HEAD
static int inet_diag_bc_run(const void *bc, int len,
			    const struct inet_diag_entry *entry)
{
=======
=======
>>>>>>> refs/remotes/origin/master
static int inet_diag_bc_run(const struct nlattr *_bc,
		const struct inet_diag_entry *entry)
{
	const void *bc = nla_data(_bc);
	int len = nla_len(_bc);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	while (len > 0) {
		int yes = 1;
		const struct inet_diag_bc_op *op = bc;

		switch (op->code) {
		case INET_DIAG_BC_NOP:
			break;
		case INET_DIAG_BC_JMP:
			yes = 0;
			break;
		case INET_DIAG_BC_S_GE:
			yes = entry->sport >= op[1].no;
			break;
		case INET_DIAG_BC_S_LE:
			yes = entry->sport <= op[1].no;
			break;
		case INET_DIAG_BC_D_GE:
			yes = entry->dport >= op[1].no;
			break;
		case INET_DIAG_BC_D_LE:
			yes = entry->dport <= op[1].no;
			break;
		case INET_DIAG_BC_AUTO:
			yes = !(entry->userlocks & SOCK_BINDPORT_LOCK);
			break;
		case INET_DIAG_BC_S_COND:
		case INET_DIAG_BC_D_COND: {
			struct inet_diag_hostcond *cond;
			__be32 *addr;

			cond = (struct inet_diag_hostcond *)(op + 1);
			if (cond->port != -1 &&
			    cond->port != (op->code == INET_DIAG_BC_S_COND ?
					     entry->sport : entry->dport)) {
				yes = 0;
				break;
			}

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
			if (cond->prefix_len == 0)
				break;

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
			if (op->code == INET_DIAG_BC_S_COND)
				addr = entry->saddr;
			else
				addr = entry->daddr;

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
			if (bitstring_match(addr, cond->addr,
					    cond->prefix_len))
				break;
			if (entry->family == AF_INET6 &&
			    cond->family == AF_INET) {
				if (addr[0] == 0 && addr[1] == 0 &&
				    addr[2] == htonl(0xffff) &&
				    bitstring_match(addr + 3, cond->addr,
						    cond->prefix_len))
					break;
			}
=======
=======
>>>>>>> refs/remotes/origin/master
			if (cond->family != AF_UNSPEC &&
			    cond->family != entry->family) {
				if (entry->family == AF_INET6 &&
				    cond->family == AF_INET) {
					if (addr[0] == 0 && addr[1] == 0 &&
					    addr[2] == htonl(0xffff) &&
					    bitstring_match(addr + 3,
							    cond->addr,
							    cond->prefix_len))
						break;
				}
				yes = 0;
				break;
			}

			if (cond->prefix_len == 0)
				break;
			if (bitstring_match(addr, cond->addr,
					    cond->prefix_len))
				break;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
			if (cond->family != AF_UNSPEC &&
			    cond->family != entry->family) {
				if (entry->family == AF_INET6 &&
				    cond->family == AF_INET) {
					if (addr[0] == 0 && addr[1] == 0 &&
					    addr[2] == htonl(0xffff) &&
					    bitstring_match(addr + 3,
							    cond->addr,
							    cond->prefix_len))
						break;
				}
				yes = 0;
				break;
			}

			if (cond->prefix_len == 0)
				break;
			if (bitstring_match(addr, cond->addr,
					    cond->prefix_len))
				break;
>>>>>>> refs/remotes/origin/cm-11.0
			yes = 0;
			break;
		}
		}

		if (yes) {
			len -= op->yes;
			bc += op->yes;
		} else {
			len -= op->no;
			bc += op->no;
		}
	}
	return len == 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
int inet_diag_bc_sk(const struct nlattr *bc, struct sock *sk)
{
	struct inet_diag_entry entry;
	struct inet_sock *inet = inet_sk(sk);

	if (bc == NULL)
		return 1;

	entry.family = sk->sk_family;
#if IS_ENABLED(CONFIG_IPV6)
	if (entry.family == AF_INET6) {
<<<<<<< HEAD
		struct ipv6_pinfo *np = inet6_sk(sk);

		entry.saddr = np->rcv_saddr.s6_addr32;
		entry.daddr = np->daddr.s6_addr32;
=======

		entry.saddr = sk->sk_v6_rcv_saddr.s6_addr32;
		entry.daddr = sk->sk_v6_daddr.s6_addr32;
>>>>>>> refs/remotes/origin/master
	} else
#endif
	{
		entry.saddr = &inet->inet_rcv_saddr;
		entry.daddr = &inet->inet_daddr;
	}
	entry.sport = inet->inet_num;
	entry.dport = ntohs(inet->inet_dport);
	entry.userlocks = sk->sk_userlocks;

	return inet_diag_bc_run(bc, &entry);
}
EXPORT_SYMBOL_GPL(inet_diag_bc_sk);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static int valid_cc(const void *bc, int len, int cc)
{
	while (len >= 0) {
		const struct inet_diag_bc_op *op = bc;

		if (cc > len)
			return 0;
		if (cc == len)
			return 1;
		if (op->yes < 4 || op->yes & 3)
			return 0;
		len -= op->yes;
		bc  += op->yes;
	}
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
/* Validate an inet_diag_hostcond. */
static bool valid_hostcond(const struct inet_diag_bc_op *op, int len,
			   int *min_len)
{
	int addr_len;
	struct inet_diag_hostcond *cond;

	/* Check hostcond space. */
	*min_len += sizeof(struct inet_diag_hostcond);
	if (len < *min_len)
		return false;
	cond = (struct inet_diag_hostcond *)(op + 1);

	/* Check address family and address length. */
	switch (cond->family) {
	case AF_UNSPEC:
		addr_len = 0;
		break;
	case AF_INET:
		addr_len = sizeof(struct in_addr);
		break;
	case AF_INET6:
		addr_len = sizeof(struct in6_addr);
		break;
	default:
		return false;
	}
	*min_len += addr_len;
	if (len < *min_len)
		return false;

	/* Check prefix length (in bits) vs address length (in bytes). */
	if (cond->prefix_len > 8 * addr_len)
		return false;

	return true;
}

/* Validate a port comparison operator. */
static inline bool valid_port_comparison(const struct inet_diag_bc_op *op,
					 int len, int *min_len)
{
	/* Port comparisons put the port in a follow-on inet_diag_bc_op. */
	*min_len += sizeof(struct inet_diag_bc_op);
	if (len < *min_len)
		return false;
	return true;
}

<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
static int inet_diag_bc_audit(const void *bytecode, int bytecode_len)
{
	const void *bc = bytecode;
	int  len = bytecode_len;

	while (len > 0) {
		const struct inet_diag_bc_op *op = bc;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
		int min_len = sizeof(struct inet_diag_bc_op);
>>>>>>> refs/remotes/origin/cm-11.0

//printk("BC: %d %d %d {%d} / %d\n", op->code, op->yes, op->no, op[1].no, len);
		switch (op->code) {
		case INET_DIAG_BC_S_COND:
		case INET_DIAG_BC_D_COND:
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
		int min_len = sizeof(struct inet_diag_bc_op);

//printk("BC: %d %d %d {%d} / %d\n", op->code, op->yes, op->no, op[1].no, len);
		switch (op->code) {
		case INET_DIAG_BC_S_COND:
		case INET_DIAG_BC_D_COND:
			if (!valid_hostcond(bc, len, &min_len))
				return -EINVAL;
			break;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
			if (!valid_hostcond(bc, len, &min_len))
				return -EINVAL;
			break;
>>>>>>> refs/remotes/origin/cm-11.0
		case INET_DIAG_BC_S_GE:
		case INET_DIAG_BC_S_LE:
		case INET_DIAG_BC_D_GE:
		case INET_DIAG_BC_D_LE:
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
		case INET_DIAG_BC_JMP:
			if (op->no < 4 || op->no > len + 4 || op->no & 3)
				return -EINVAL;
			if (op->no < len &&
			    !valid_cc(bytecode, bytecode_len, len - op->no))
				return -EINVAL;
			break;
=======
=======
>>>>>>> refs/remotes/origin/master
			if (!valid_port_comparison(bc, len, &min_len))
				return -EINVAL;
			break;
		case INET_DIAG_BC_AUTO:
		case INET_DIAG_BC_JMP:
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
			if (!valid_port_comparison(bc, len, &min_len))
				return -EINVAL;
			break;
		case INET_DIAG_BC_AUTO:
		case INET_DIAG_BC_JMP:
>>>>>>> refs/remotes/origin/cm-11.0
		case INET_DIAG_BC_NOP:
			break;
		default:
			return -EINVAL;
		}
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
		if (op->yes < 4 || op->yes > len + 4 || op->yes & 3)
=======
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

		if (op->code != INET_DIAG_BC_NOP) {
			if (op->no < min_len || op->no > len + 4 || op->no & 3)
				return -EINVAL;
			if (op->no < len &&
			    !valid_cc(bytecode, bytecode_len, len - op->no))
				return -EINVAL;
		}

		if (op->yes < min_len || op->yes > len + 4 || op->yes & 3)
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
			return -EINVAL;
		bc  += op->yes;
		len -= op->yes;
	}
	return len == 0 ? 0 : -EINVAL;
}

static int inet_csk_diag_dump(struct sock *sk,
			      struct sk_buff *skb,
<<<<<<< HEAD
<<<<<<< HEAD
			      struct netlink_callback *cb)
{
	struct inet_diag_req *r = NLMSG_DATA(cb->nlh);

	if (nlmsg_attrlen(cb->nlh, sizeof(*r))) {
		struct inet_diag_entry entry;
		const struct nlattr *bc = nlmsg_find_attr(cb->nlh,
							  sizeof(*r),
							  INET_DIAG_REQ_BYTECODE);
		struct inet_sock *inet = inet_sk(sk);

		entry.family = sk->sk_family;
#if defined(CONFIG_IPV6) || defined (CONFIG_IPV6_MODULE)
		if (entry.family == AF_INET6) {
			struct ipv6_pinfo *np = inet6_sk(sk);

			entry.saddr = np->rcv_saddr.s6_addr32;
			entry.daddr = np->daddr.s6_addr32;
		} else
#endif
		{
			entry.saddr = &inet->inet_rcv_saddr;
			entry.daddr = &inet->inet_daddr;
		}
		entry.sport = inet->inet_num;
		entry.dport = ntohs(inet->inet_dport);
		entry.userlocks = sk->sk_userlocks;

		if (!inet_diag_bc_run(nla_data(bc), nla_len(bc), &entry))
			return 0;
	}

	return inet_csk_diag_fill(sk, skb, r->idiag_ext,
=======
=======
>>>>>>> refs/remotes/origin/master
			      struct netlink_callback *cb,
			      struct inet_diag_req_v2 *r,
			      const struct nlattr *bc)
{
	if (!inet_diag_bc_sk(bc, sk))
		return 0;

	return inet_csk_diag_fill(sk, skb, r,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
				  NETLINK_CB(cb->skb).pid,
				  cb->nlh->nlmsg_seq, NLM_F_MULTI, cb->nlh);
}

static int inet_twsk_diag_dump(struct inet_timewait_sock *tw,
			       struct sk_buff *skb,
<<<<<<< HEAD
			       struct netlink_callback *cb)
{
	struct inet_diag_req *r = NLMSG_DATA(cb->nlh);

	if (nlmsg_attrlen(cb->nlh, sizeof(*r))) {
		struct inet_diag_entry entry;
		const struct nlattr *bc = nlmsg_find_attr(cb->nlh,
							  sizeof(*r),
							  INET_DIAG_REQ_BYTECODE);

		entry.family = tw->tw_family;
#if defined(CONFIG_IPV6) || defined (CONFIG_IPV6_MODULE)
=======
=======
				  sk_user_ns(NETLINK_CB(cb->skb).sk),
				  NETLINK_CB(cb->skb).portid,
				  cb->nlh->nlmsg_seq, NLM_F_MULTI, cb->nlh);
}

static int inet_twsk_diag_dump(struct sock *sk,
			       struct sk_buff *skb,
>>>>>>> refs/remotes/origin/master
			       struct netlink_callback *cb,
			       struct inet_diag_req_v2 *r,
			       const struct nlattr *bc)
{
<<<<<<< HEAD
=======
	struct inet_timewait_sock *tw = inet_twsk(sk);

>>>>>>> refs/remotes/origin/master
	if (bc != NULL) {
		struct inet_diag_entry entry;

		entry.family = tw->tw_family;
#if IS_ENABLED(CONFIG_IPV6)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		if (tw->tw_family == AF_INET6) {
			struct inet6_timewait_sock *tw6 =
						inet6_twsk((struct sock *)tw);
			entry.saddr = tw6->tw_v6_rcv_saddr.s6_addr32;
			entry.daddr = tw6->tw_v6_daddr.s6_addr32;
=======
		if (tw->tw_family == AF_INET6) {
			entry.saddr = tw->tw_v6_rcv_saddr.s6_addr32;
			entry.daddr = tw->tw_v6_daddr.s6_addr32;
>>>>>>> refs/remotes/origin/master
		} else
#endif
		{
			entry.saddr = &tw->tw_rcv_saddr;
			entry.daddr = &tw->tw_daddr;
		}
		entry.sport = tw->tw_num;
		entry.dport = ntohs(tw->tw_dport);
		entry.userlocks = 0;

<<<<<<< HEAD
<<<<<<< HEAD
		if (!inet_diag_bc_run(nla_data(bc), nla_len(bc), &entry))
			return 0;
	}

	return inet_twsk_diag_fill(tw, skb, r->idiag_ext,
=======
=======
>>>>>>> refs/remotes/origin/master
		if (!inet_diag_bc_run(bc, &entry))
			return 0;
	}

	return inet_twsk_diag_fill(tw, skb, r,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
				   NETLINK_CB(cb->skb).pid,
				   cb->nlh->nlmsg_seq, NLM_F_MULTI, cb->nlh);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
				   NETLINK_CB(cb->skb).portid,
				   cb->nlh->nlmsg_seq, NLM_F_MULTI, cb->nlh);
}

>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
/* Get the IPv4, IPv6, or IPv4-mapped-IPv6 local and remote addresses
 * from a request_sock. For IPv4-mapped-IPv6 we must map IPv4 to IPv6.
 */
static inline void inet_diag_req_addrs(const struct sock *sk,
				       const struct request_sock *req,
				       struct inet_diag_entry *entry)
{
	struct inet_request_sock *ireq = inet_rsk(req);

#if IS_ENABLED(CONFIG_IPV6)
	if (sk->sk_family == AF_INET6) {
		if (req->rsk_ops->family == AF_INET6) {
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
			entry->saddr = inet6_rsk(req)->loc_addr.s6_addr32;
			entry->daddr = inet6_rsk(req)->rmt_addr.s6_addr32;
		} else if (req->rsk_ops->family == AF_INET) {
			ipv6_addr_set_v4mapped(ireq->loc_addr,
					       &entry->saddr_storage);
			ipv6_addr_set_v4mapped(ireq->rmt_addr,
<<<<<<< HEAD
=======
			entry->saddr = ireq->ir_v6_loc_addr.s6_addr32;
			entry->daddr = ireq->ir_v6_rmt_addr.s6_addr32;
		} else if (req->rsk_ops->family == AF_INET) {
			ipv6_addr_set_v4mapped(ireq->ir_loc_addr,
					       &entry->saddr_storage);
			ipv6_addr_set_v4mapped(ireq->ir_rmt_addr,
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
					       &entry->daddr_storage);
			entry->saddr = entry->saddr_storage.s6_addr32;
			entry->daddr = entry->daddr_storage.s6_addr32;
		}
	} else
#endif
	{
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
		entry->saddr = &ireq->loc_addr;
		entry->daddr = &ireq->rmt_addr;
	}
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
static int inet_diag_fill_req(struct sk_buff *skb, struct sock *sk,
			      struct request_sock *req, u32 pid, u32 seq,
=======
		entry->saddr = &ireq->ir_loc_addr;
		entry->daddr = &ireq->ir_rmt_addr;
	}
}

static int inet_diag_fill_req(struct sk_buff *skb, struct sock *sk,
			      struct request_sock *req,
			      struct user_namespace *user_ns,
			      u32 portid, u32 seq,
>>>>>>> refs/remotes/origin/master
			      const struct nlmsghdr *unlh)
{
	const struct inet_request_sock *ireq = inet_rsk(req);
	struct inet_sock *inet = inet_sk(sk);
<<<<<<< HEAD
	unsigned char *b = skb_tail_pointer(skb);
=======
>>>>>>> refs/remotes/origin/master
	struct inet_diag_msg *r;
	struct nlmsghdr *nlh;
	long tmo;

<<<<<<< HEAD
	nlh = NLMSG_PUT(skb, pid, seq, unlh->nlmsg_type, sizeof(*r));
	nlh->nlmsg_flags = NLM_F_MULTI;
	r = NLMSG_DATA(nlh);

	r->idiag_family = sk->sk_family;
	r->idiag_state = TCP_SYN_RECV;
	r->idiag_timer = 1;
	r->idiag_retrans = req->retrans;

	r->id.idiag_if = sk->sk_bound_dev_if;
<<<<<<< HEAD
	r->id.idiag_cookie[0] = (u32)(unsigned long)req;
	r->id.idiag_cookie[1] = (u32)(((unsigned long)req >> 31) >> 1);
=======
	sock_diag_save_cookie(req, r->id.idiag_cookie);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	nlh = nlmsg_put(skb, portid, seq, unlh->nlmsg_type, sizeof(*r),
			NLM_F_MULTI);
	if (!nlh)
		return -EMSGSIZE;

	r = nlmsg_data(nlh);
	r->idiag_family = sk->sk_family;
	r->idiag_state = TCP_SYN_RECV;
	r->idiag_timer = 1;
	r->idiag_retrans = req->num_retrans;

	r->id.idiag_if = sk->sk_bound_dev_if;
	sock_diag_save_cookie(req, r->id.idiag_cookie);
>>>>>>> refs/remotes/origin/master

	tmo = req->expires - jiffies;
	if (tmo < 0)
		tmo = 0;

	r->id.idiag_sport = inet->inet_sport;
<<<<<<< HEAD
	r->id.idiag_dport = ireq->rmt_port;
	r->id.idiag_src[0] = ireq->loc_addr;
	r->id.idiag_dst[0] = ireq->rmt_addr;
	r->idiag_expires = jiffies_to_msecs(tmo);
	r->idiag_rqueue = 0;
	r->idiag_wqueue = 0;
	r->idiag_uid = sock_i_uid(sk);
	r->idiag_inode = 0;
<<<<<<< HEAD
#if defined(CONFIG_IPV6) || defined (CONFIG_IPV6_MODULE)
	if (r->idiag_family == AF_INET6) {
		ipv6_addr_copy((struct in6_addr *)r->id.idiag_src,
			       &inet6_rsk(req)->loc_addr);
		ipv6_addr_copy((struct in6_addr *)r->id.idiag_dst,
			       &inet6_rsk(req)->rmt_addr);
=======
=======
	r->id.idiag_dport = ireq->ir_rmt_port;

	memset(&r->id.idiag_src, 0, sizeof(r->id.idiag_src));
	memset(&r->id.idiag_dst, 0, sizeof(r->id.idiag_dst));

	r->id.idiag_src[0] = ireq->ir_loc_addr;
	r->id.idiag_dst[0] = ireq->ir_rmt_addr;

	r->idiag_expires = jiffies_to_msecs(tmo);
	r->idiag_rqueue = 0;
	r->idiag_wqueue = 0;
	r->idiag_uid = from_kuid_munged(user_ns, sock_i_uid(sk));
	r->idiag_inode = 0;
>>>>>>> refs/remotes/origin/master
#if IS_ENABLED(CONFIG_IPV6)
	if (r->idiag_family == AF_INET6) {
		struct inet_diag_entry entry;
		inet_diag_req_addrs(sk, req, &entry);
		memcpy(r->id.idiag_src, entry.saddr, sizeof(struct in6_addr));
		memcpy(r->id.idiag_dst, entry.daddr, sizeof(struct in6_addr));
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
	}
#endif
	nlh->nlmsg_len = skb_tail_pointer(skb) - b;

	return skb->len;

nlmsg_failure:
	nlmsg_trim(skb, b);
	return -1;
}

static int inet_diag_dump_reqs(struct sk_buff *skb, struct sock *sk,
<<<<<<< HEAD
			       struct netlink_callback *cb)
{
	struct inet_diag_entry entry;
	struct inet_diag_req *r = NLMSG_DATA(cb->nlh);
	struct inet_connection_sock *icsk = inet_csk(sk);
	struct listen_sock *lopt;
	const struct nlattr *bc = NULL;
=======
=======
	}
#endif

	return nlmsg_end(skb, nlh);
}

static int inet_diag_dump_reqs(struct sk_buff *skb, struct sock *sk,
>>>>>>> refs/remotes/origin/master
			       struct netlink_callback *cb,
			       struct inet_diag_req_v2 *r,
			       const struct nlattr *bc)
{
	struct inet_diag_entry entry;
	struct inet_connection_sock *icsk = inet_csk(sk);
	struct listen_sock *lopt;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	struct inet_sock *inet = inet_sk(sk);
	int j, s_j;
	int reqnum, s_reqnum;
	int err = 0;

	s_j = cb->args[3];
	s_reqnum = cb->args[4];

	if (s_j > 0)
		s_j--;

	entry.family = sk->sk_family;

	read_lock_bh(&icsk->icsk_accept_queue.syn_wait_lock);

	lopt = icsk->icsk_accept_queue.listen_opt;
	if (!lopt || !lopt->qlen)
		goto out;

<<<<<<< HEAD
<<<<<<< HEAD
	if (nlmsg_attrlen(cb->nlh, sizeof(*r))) {
		bc = nlmsg_find_attr(cb->nlh, sizeof(*r),
				     INET_DIAG_REQ_BYTECODE);
=======
	if (bc != NULL) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (bc != NULL) {
>>>>>>> refs/remotes/origin/master
		entry.sport = inet->inet_num;
		entry.userlocks = sk->sk_userlocks;
	}

	for (j = s_j; j < lopt->nr_table_entries; j++) {
		struct request_sock *req, *head = lopt->syn_table[j];

		reqnum = 0;
		for (req = head; req; reqnum++, req = req->dl_next) {
			struct inet_request_sock *ireq = inet_rsk(req);

			if (reqnum < s_reqnum)
				continue;
<<<<<<< HEAD
			if (r->id.idiag_dport != ireq->rmt_port &&
=======
			if (r->id.idiag_dport != ireq->ir_rmt_port &&
>>>>>>> refs/remotes/origin/master
			    r->id.idiag_dport)
				continue;

			if (bc) {
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
				entry.saddr =
#if defined(CONFIG_IPV6) || defined (CONFIG_IPV6_MODULE)
					(entry.family == AF_INET6) ?
					inet6_rsk(req)->loc_addr.s6_addr32 :
#endif
					&ireq->loc_addr;
				entry.daddr =
#if defined(CONFIG_IPV6) || defined (CONFIG_IPV6_MODULE)
					(entry.family == AF_INET6) ?
					inet6_rsk(req)->rmt_addr.s6_addr32 :
#endif
					&ireq->rmt_addr;
=======
				inet_diag_req_addrs(sk, req, &entry);
>>>>>>> refs/remotes/origin/cm-11.0
				entry.dport = ntohs(ireq->rmt_port);

				if (!inet_diag_bc_run(nla_data(bc),
						      nla_len(bc), &entry))
=======
				inet_diag_req_addrs(sk, req, &entry);
				entry.dport = ntohs(ireq->rmt_port);

				if (!inet_diag_bc_run(bc, &entry))
>>>>>>> refs/remotes/origin/cm-10.0
=======
				inet_diag_req_addrs(sk, req, &entry);
				entry.dport = ntohs(ireq->ir_rmt_port);

				if (!inet_diag_bc_run(bc, &entry))
>>>>>>> refs/remotes/origin/master
					continue;
			}

			err = inet_diag_fill_req(skb, sk, req,
<<<<<<< HEAD
					       NETLINK_CB(cb->skb).pid,
=======
					       sk_user_ns(NETLINK_CB(cb->skb).sk),
					       NETLINK_CB(cb->skb).portid,
>>>>>>> refs/remotes/origin/master
					       cb->nlh->nlmsg_seq, cb->nlh);
			if (err < 0) {
				cb->args[3] = j + 1;
				cb->args[4] = reqnum;
				goto out;
			}
		}

		s_reqnum = 0;
	}

out:
	read_unlock_bh(&icsk->icsk_accept_queue.syn_wait_lock);

	return err;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int inet_diag_dump(struct sk_buff *skb, struct netlink_callback *cb)
{
	int i, num;
	int s_i, s_num;
	struct inet_diag_req *r = NLMSG_DATA(cb->nlh);
	const struct inet_diag_handler *handler;
	struct inet_hashinfo *hashinfo;

	handler = inet_diag_lock_handler(cb->nlh->nlmsg_type);
	if (IS_ERR(handler))
		goto unlock;

	hashinfo = handler->idiag_hashinfo;
=======
=======
>>>>>>> refs/remotes/origin/master
void inet_diag_dump_icsk(struct inet_hashinfo *hashinfo, struct sk_buff *skb,
		struct netlink_callback *cb, struct inet_diag_req_v2 *r, struct nlattr *bc)
{
	int i, num;
	int s_i, s_num;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct net *net = sock_net(skb->sk);
>>>>>>> refs/remotes/origin/master

	s_i = cb->args[1];
	s_num = num = cb->args[2];

	if (cb->args[0] == 0) {
		if (!(r->idiag_states & (TCPF_LISTEN | TCPF_SYN_RECV)))
			goto skip_listen_ht;

		for (i = s_i; i < INET_LHTABLE_SIZE; i++) {
			struct sock *sk;
			struct hlist_nulls_node *node;
			struct inet_listen_hashbucket *ilb;

			num = 0;
			ilb = &hashinfo->listening_hash[i];
			spin_lock_bh(&ilb->lock);
			sk_nulls_for_each(sk, node, &ilb->head) {
				struct inet_sock *inet = inet_sk(sk);

<<<<<<< HEAD
=======
				if (!net_eq(sock_net(sk), net))
					continue;

>>>>>>> refs/remotes/origin/master
				if (num < s_num) {
					num++;
					continue;
				}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
				if (r->sdiag_family != AF_UNSPEC &&
						sk->sk_family != r->sdiag_family)
					goto next_listen;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				if (r->id.idiag_sport != inet->inet_sport &&
				    r->id.idiag_sport)
					goto next_listen;

				if (!(r->idiag_states & TCPF_LISTEN) ||
				    r->id.idiag_dport ||
				    cb->args[3] > 0)
					goto syn_recv;

<<<<<<< HEAD
<<<<<<< HEAD
				if (inet_csk_diag_dump(sk, skb, cb) < 0) {
=======
				if (inet_csk_diag_dump(sk, skb, cb, r, bc) < 0) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
				if (inet_csk_diag_dump(sk, skb, cb, r, bc) < 0) {
>>>>>>> refs/remotes/origin/master
					spin_unlock_bh(&ilb->lock);
					goto done;
				}

syn_recv:
				if (!(r->idiag_states & TCPF_SYN_RECV))
					goto next_listen;

<<<<<<< HEAD
<<<<<<< HEAD
				if (inet_diag_dump_reqs(skb, sk, cb) < 0) {
=======
				if (inet_diag_dump_reqs(skb, sk, cb, r, bc) < 0) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
				if (inet_diag_dump_reqs(skb, sk, cb, r, bc) < 0) {
>>>>>>> refs/remotes/origin/master
					spin_unlock_bh(&ilb->lock);
					goto done;
				}

next_listen:
				cb->args[3] = 0;
				cb->args[4] = 0;
				++num;
			}
			spin_unlock_bh(&ilb->lock);

			s_num = 0;
			cb->args[3] = 0;
			cb->args[4] = 0;
		}
skip_listen_ht:
		cb->args[0] = 1;
		s_i = num = s_num = 0;
	}

	if (!(r->idiag_states & ~(TCPF_LISTEN | TCPF_SYN_RECV)))
<<<<<<< HEAD
<<<<<<< HEAD
		goto unlock;
=======
		goto out;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		goto out;
>>>>>>> refs/remotes/origin/master

	for (i = s_i; i <= hashinfo->ehash_mask; i++) {
		struct inet_ehash_bucket *head = &hashinfo->ehash[i];
		spinlock_t *lock = inet_ehash_lockp(hashinfo, i);
		struct sock *sk;
		struct hlist_nulls_node *node;

		num = 0;

<<<<<<< HEAD
		if (hlist_nulls_empty(&head->chain) &&
			hlist_nulls_empty(&head->twchain))
=======
		if (hlist_nulls_empty(&head->chain))
>>>>>>> refs/remotes/origin/master
			continue;

		if (i > s_i)
			s_num = 0;

		spin_lock_bh(lock);
		sk_nulls_for_each(sk, node, &head->chain) {
<<<<<<< HEAD
			struct inet_sock *inet = inet_sk(sk);

			if (num < s_num)
				goto next_normal;
			if (!(r->idiag_states & (1 << sk->sk_state)))
				goto next_normal;
<<<<<<< HEAD
=======
			if (r->sdiag_family != AF_UNSPEC &&
					sk->sk_family != r->sdiag_family)
				goto next_normal;
>>>>>>> refs/remotes/origin/cm-10.0
			if (r->id.idiag_sport != inet->inet_sport &&
			    r->id.idiag_sport)
				goto next_normal;
			if (r->id.idiag_dport != inet->inet_dport &&
			    r->id.idiag_dport)
				goto next_normal;
<<<<<<< HEAD
			if (inet_csk_diag_dump(sk, skb, cb) < 0) {
=======
			if (inet_csk_diag_dump(sk, skb, cb, r, bc) < 0) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
			int res;
			int state;

			if (!net_eq(sock_net(sk), net))
				continue;
			if (num < s_num)
				goto next_normal;
			state = (sk->sk_state == TCP_TIME_WAIT) ?
				inet_twsk(sk)->tw_substate : sk->sk_state;
			if (!(r->idiag_states & (1 << state)))
				goto next_normal;
			if (r->sdiag_family != AF_UNSPEC &&
			    sk->sk_family != r->sdiag_family)
				goto next_normal;
			if (r->id.idiag_sport != htons(sk->sk_num) &&
			    r->id.idiag_sport)
				goto next_normal;
			if (r->id.idiag_dport != sk->sk_dport &&
			    r->id.idiag_dport)
				goto next_normal;
			if (sk->sk_state == TCP_TIME_WAIT)
				res = inet_twsk_diag_dump(sk, skb, cb, r, bc);
			else
				res = inet_csk_diag_dump(sk, skb, cb, r, bc);
			if (res < 0) {
>>>>>>> refs/remotes/origin/master
				spin_unlock_bh(lock);
				goto done;
			}
next_normal:
			++num;
		}

<<<<<<< HEAD
		if (r->idiag_states & TCPF_TIME_WAIT) {
			struct inet_timewait_sock *tw;

			inet_twsk_for_each(tw, node,
				    &head->twchain) {

				if (num < s_num)
					goto next_dying;
<<<<<<< HEAD
=======
				if (r->sdiag_family != AF_UNSPEC &&
						tw->tw_family != r->sdiag_family)
					goto next_dying;
>>>>>>> refs/remotes/origin/cm-10.0
				if (r->id.idiag_sport != tw->tw_sport &&
				    r->id.idiag_sport)
					goto next_dying;
				if (r->id.idiag_dport != tw->tw_dport &&
				    r->id.idiag_dport)
					goto next_dying;
<<<<<<< HEAD
				if (inet_twsk_diag_dump(tw, skb, cb) < 0) {
=======
				if (inet_twsk_diag_dump(tw, skb, cb, r, bc) < 0) {
>>>>>>> refs/remotes/origin/cm-10.0
					spin_unlock_bh(lock);
					goto done;
				}
next_dying:
				++num;
			}
		}
=======
>>>>>>> refs/remotes/origin/master
		spin_unlock_bh(lock);
	}

done:
	cb->args[1] = i;
	cb->args[2] = num;
<<<<<<< HEAD
<<<<<<< HEAD
unlock:
	inet_diag_unlock_handler(handler);
	return skb->len;
}

static int inet_diag_rcv_msg(struct sk_buff *skb, struct nlmsghdr *nlh)
=======
=======
>>>>>>> refs/remotes/origin/master
out:
	;
}
EXPORT_SYMBOL_GPL(inet_diag_dump_icsk);

static int __inet_diag_dump(struct sk_buff *skb, struct netlink_callback *cb,
		struct inet_diag_req_v2 *r, struct nlattr *bc)
{
	const struct inet_diag_handler *handler;
	int err = 0;

	handler = inet_diag_lock_handler(r->sdiag_protocol);
	if (!IS_ERR(handler))
		handler->dump(skb, cb, r, bc);
	else
		err = PTR_ERR(handler);
	inet_diag_unlock_handler(handler);

	return err ? : skb->len;
}

static int inet_diag_dump(struct sk_buff *skb, struct netlink_callback *cb)
{
	struct nlattr *bc = NULL;
	int hdrlen = sizeof(struct inet_diag_req_v2);

	if (nlmsg_attrlen(cb->nlh, hdrlen))
		bc = nlmsg_find_attr(cb->nlh, hdrlen, INET_DIAG_REQ_BYTECODE);

<<<<<<< HEAD
	return __inet_diag_dump(skb, cb, (struct inet_diag_req_v2 *)NLMSG_DATA(cb->nlh), bc);
=======
	return __inet_diag_dump(skb, cb, nlmsg_data(cb->nlh), bc);
>>>>>>> refs/remotes/origin/master
}

static inline int inet_diag_type2proto(int type)
{
	switch (type) {
	case TCPDIAG_GETSOCK:
		return IPPROTO_TCP;
	case DCCPDIAG_GETSOCK:
		return IPPROTO_DCCP;
	default:
		return 0;
	}
}

static int inet_diag_dump_compat(struct sk_buff *skb, struct netlink_callback *cb)
{
<<<<<<< HEAD
	struct inet_diag_req *rc = NLMSG_DATA(cb->nlh);
=======
	struct inet_diag_req *rc = nlmsg_data(cb->nlh);
>>>>>>> refs/remotes/origin/master
	struct inet_diag_req_v2 req;
	struct nlattr *bc = NULL;
	int hdrlen = sizeof(struct inet_diag_req);

	req.sdiag_family = AF_UNSPEC; /* compatibility */
	req.sdiag_protocol = inet_diag_type2proto(cb->nlh->nlmsg_type);
	req.idiag_ext = rc->idiag_ext;
	req.idiag_states = rc->idiag_states;
	req.id = rc->id;

	if (nlmsg_attrlen(cb->nlh, hdrlen))
		bc = nlmsg_find_attr(cb->nlh, hdrlen, INET_DIAG_REQ_BYTECODE);

	return __inet_diag_dump(skb, cb, &req, bc);
}

static int inet_diag_get_exact_compat(struct sk_buff *in_skb,
			       const struct nlmsghdr *nlh)
{
<<<<<<< HEAD
	struct inet_diag_req *rc = NLMSG_DATA(nlh);
=======
	struct inet_diag_req *rc = nlmsg_data(nlh);
>>>>>>> refs/remotes/origin/master
	struct inet_diag_req_v2 req;

	req.sdiag_family = rc->idiag_family;
	req.sdiag_protocol = inet_diag_type2proto(nlh->nlmsg_type);
	req.idiag_ext = rc->idiag_ext;
	req.idiag_states = rc->idiag_states;
	req.id = rc->id;

	return inet_diag_get_exact(in_skb, nlh, &req);
}

static int inet_diag_rcv_msg_compat(struct sk_buff *skb, struct nlmsghdr *nlh)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
{
	int hdrlen = sizeof(struct inet_diag_req);
=======
{
	int hdrlen = sizeof(struct inet_diag_req);
	struct net *net = sock_net(skb->sk);
>>>>>>> refs/remotes/origin/master

	if (nlh->nlmsg_type >= INET_DIAG_GETSOCK_MAX ||
	    nlmsg_len(nlh) < hdrlen)
		return -EINVAL;

	if (nlh->nlmsg_flags & NLM_F_DUMP) {
		if (nlmsg_attrlen(nlh, hdrlen)) {
			struct nlattr *attr;

			attr = nlmsg_find_attr(nlh, hdrlen,
					       INET_DIAG_REQ_BYTECODE);
			if (attr == NULL ||
			    nla_len(attr) < sizeof(struct inet_diag_bc_op) ||
			    inet_diag_bc_audit(nla_data(attr), nla_len(attr)))
				return -EINVAL;
		}
<<<<<<< HEAD
<<<<<<< HEAD

		return netlink_dump_start(idiagnl, skb, nlh,
					  inet_diag_dump, NULL, 0);
	}

	return inet_diag_get_exact(skb, nlh);
}

static DEFINE_MUTEX(inet_diag_mutex);

static void inet_diag_rcv(struct sk_buff *skb)
{
	mutex_lock(&inet_diag_mutex);
	netlink_rcv_skb(skb, &inet_diag_rcv_msg);
	mutex_unlock(&inet_diag_mutex);
}

=======
=======
>>>>>>> refs/remotes/origin/master
		{
			struct netlink_dump_control c = {
				.dump = inet_diag_dump_compat,
			};
<<<<<<< HEAD
			return netlink_dump_start(sock_diag_nlsk, skb, nlh, &c);
=======
			return netlink_dump_start(net->diag_nlsk, skb, nlh, &c);
>>>>>>> refs/remotes/origin/master
		}
	}

	return inet_diag_get_exact_compat(skb, nlh);
}

static int inet_diag_handler_dump(struct sk_buff *skb, struct nlmsghdr *h)
{
	int hdrlen = sizeof(struct inet_diag_req_v2);
<<<<<<< HEAD
=======
	struct net *net = sock_net(skb->sk);
>>>>>>> refs/remotes/origin/master

	if (nlmsg_len(h) < hdrlen)
		return -EINVAL;

	if (h->nlmsg_flags & NLM_F_DUMP) {
		if (nlmsg_attrlen(h, hdrlen)) {
			struct nlattr *attr;
			attr = nlmsg_find_attr(h, hdrlen,
					       INET_DIAG_REQ_BYTECODE);
			if (attr == NULL ||
			    nla_len(attr) < sizeof(struct inet_diag_bc_op) ||
			    inet_diag_bc_audit(nla_data(attr), nla_len(attr)))
				return -EINVAL;
		}
		{
			struct netlink_dump_control c = {
				.dump = inet_diag_dump,
			};
<<<<<<< HEAD
			return netlink_dump_start(sock_diag_nlsk, skb, h, &c);
		}
	}

	return inet_diag_get_exact(skb, h, (struct inet_diag_req_v2 *)NLMSG_DATA(h));
}

static struct sock_diag_handler inet_diag_handler = {
=======
			return netlink_dump_start(net->diag_nlsk, skb, h, &c);
		}
	}

	return inet_diag_get_exact(skb, h, nlmsg_data(h));
}

static const struct sock_diag_handler inet_diag_handler = {
>>>>>>> refs/remotes/origin/master
	.family = AF_INET,
	.dump = inet_diag_handler_dump,
};

<<<<<<< HEAD
static struct sock_diag_handler inet6_diag_handler = {
=======
static const struct sock_diag_handler inet6_diag_handler = {
>>>>>>> refs/remotes/origin/master
	.family = AF_INET6,
	.dump = inet_diag_handler_dump,
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
int inet_diag_register(const struct inet_diag_handler *h)
{
	const __u16 type = h->idiag_type;
	int err = -EINVAL;

<<<<<<< HEAD
<<<<<<< HEAD
	if (type >= INET_DIAG_GETSOCK_MAX)
=======
	if (type >= IPPROTO_MAX)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (type >= IPPROTO_MAX)
>>>>>>> refs/remotes/origin/master
		goto out;

	mutex_lock(&inet_diag_table_mutex);
	err = -EEXIST;
	if (inet_diag_table[type] == NULL) {
		inet_diag_table[type] = h;
		err = 0;
	}
	mutex_unlock(&inet_diag_table_mutex);
out:
	return err;
}
EXPORT_SYMBOL_GPL(inet_diag_register);

void inet_diag_unregister(const struct inet_diag_handler *h)
{
	const __u16 type = h->idiag_type;

<<<<<<< HEAD
<<<<<<< HEAD
	if (type >= INET_DIAG_GETSOCK_MAX)
=======
	if (type >= IPPROTO_MAX)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (type >= IPPROTO_MAX)
>>>>>>> refs/remotes/origin/master
		return;

	mutex_lock(&inet_diag_table_mutex);
	inet_diag_table[type] = NULL;
	mutex_unlock(&inet_diag_table_mutex);
}
EXPORT_SYMBOL_GPL(inet_diag_unregister);

static int __init inet_diag_init(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	const int inet_diag_table_size = (INET_DIAG_GETSOCK_MAX *
=======
	const int inet_diag_table_size = (IPPROTO_MAX *
>>>>>>> refs/remotes/origin/cm-10.0
=======
	const int inet_diag_table_size = (IPPROTO_MAX *
>>>>>>> refs/remotes/origin/master
					  sizeof(struct inet_diag_handler *));
	int err = -ENOMEM;

	inet_diag_table = kzalloc(inet_diag_table_size, GFP_KERNEL);
	if (!inet_diag_table)
		goto out;

<<<<<<< HEAD
<<<<<<< HEAD
	idiagnl = netlink_kernel_create(&init_net, NETLINK_INET_DIAG, 0,
					inet_diag_rcv, NULL, THIS_MODULE);
	if (idiagnl == NULL)
		goto out_free_table;
	err = 0;
out:
	return err;
out_free_table:
=======
=======
>>>>>>> refs/remotes/origin/master
	err = sock_diag_register(&inet_diag_handler);
	if (err)
		goto out_free_nl;

	err = sock_diag_register(&inet6_diag_handler);
	if (err)
		goto out_free_inet;

	sock_diag_register_inet_compat(inet_diag_rcv_msg_compat);
out:
	return err;

out_free_inet:
	sock_diag_unregister(&inet_diag_handler);
out_free_nl:
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	kfree(inet_diag_table);
	goto out;
}

static void __exit inet_diag_exit(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	netlink_kernel_release(idiagnl);
=======
	sock_diag_unregister(&inet6_diag_handler);
	sock_diag_unregister(&inet_diag_handler);
	sock_diag_unregister_inet_compat(inet_diag_rcv_msg_compat);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	sock_diag_unregister(&inet6_diag_handler);
	sock_diag_unregister(&inet_diag_handler);
	sock_diag_unregister_inet_compat(inet_diag_rcv_msg_compat);
>>>>>>> refs/remotes/origin/master
	kfree(inet_diag_table);
}

module_init(inet_diag_init);
module_exit(inet_diag_exit);
MODULE_LICENSE("GPL");
<<<<<<< HEAD
<<<<<<< HEAD
MODULE_ALIAS_NET_PF_PROTO(PF_NETLINK, NETLINK_INET_DIAG);
=======
MODULE_ALIAS_NET_PF_PROTO_TYPE(PF_NETLINK, NETLINK_SOCK_DIAG, 2 /* AF_INET */);
MODULE_ALIAS_NET_PF_PROTO_TYPE(PF_NETLINK, NETLINK_SOCK_DIAG, 10 /* AF_INET6 */);
>>>>>>> refs/remotes/origin/cm-10.0
=======
MODULE_ALIAS_NET_PF_PROTO_TYPE(PF_NETLINK, NETLINK_SOCK_DIAG, 2 /* AF_INET */);
MODULE_ALIAS_NET_PF_PROTO_TYPE(PF_NETLINK, NETLINK_SOCK_DIAG, 10 /* AF_INET6 */);
>>>>>>> refs/remotes/origin/master
