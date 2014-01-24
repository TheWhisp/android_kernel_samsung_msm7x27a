/*
 * SELinux support for the XFRM LSM hooks
 *
 * Author : Trent Jaeger, <jaegert@us.ibm.com>
 * Updated : Venkat Yekkirala, <vyekkirala@TrustedCS.com>
 */
#ifndef _SELINUX_XFRM_H_
#define _SELINUX_XFRM_H_

<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <net/flow.h>

>>>>>>> refs/remotes/origin/cm-10.0
int selinux_xfrm_policy_alloc(struct xfrm_sec_ctx **ctxp,
			      struct xfrm_user_sec_ctx *sec_ctx);
=======
#include <net/flow.h>

int selinux_xfrm_policy_alloc(struct xfrm_sec_ctx **ctxp,
			      struct xfrm_user_sec_ctx *uctx);
>>>>>>> refs/remotes/origin/master
int selinux_xfrm_policy_clone(struct xfrm_sec_ctx *old_ctx,
			      struct xfrm_sec_ctx **new_ctxp);
void selinux_xfrm_policy_free(struct xfrm_sec_ctx *ctx);
int selinux_xfrm_policy_delete(struct xfrm_sec_ctx *ctx);
int selinux_xfrm_state_alloc(struct xfrm_state *x,
<<<<<<< HEAD
	struct xfrm_user_sec_ctx *sec_ctx, u32 secid);
=======
			     struct xfrm_user_sec_ctx *uctx);
int selinux_xfrm_state_alloc_acquire(struct xfrm_state *x,
				     struct xfrm_sec_ctx *polsec, u32 secid);
>>>>>>> refs/remotes/origin/master
void selinux_xfrm_state_free(struct xfrm_state *x);
int selinux_xfrm_state_delete(struct xfrm_state *x);
int selinux_xfrm_policy_lookup(struct xfrm_sec_ctx *ctx, u32 fl_secid, u8 dir);
int selinux_xfrm_state_pol_flow_match(struct xfrm_state *x,
<<<<<<< HEAD
			struct xfrm_policy *xp, const struct flowi *fl);

/*
 * Extract the security blob from the sock (it's actually on the socket)
 */
static inline struct inode_security_struct *get_sock_isec(struct sock *sk)
{
	if (!sk->sk_socket)
		return NULL;

	return SOCK_INODE(sk->sk_socket)->i_security;
}
=======
				      struct xfrm_policy *xp,
				      const struct flowi *fl);
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_SECURITY_NETWORK_XFRM
extern atomic_t selinux_xfrm_refcount;

static inline int selinux_xfrm_enabled(void)
{
	return (atomic_read(&selinux_xfrm_refcount) > 0);
}

<<<<<<< HEAD
int selinux_xfrm_sock_rcv_skb(u32 sid, struct sk_buff *skb,
			struct common_audit_data *ad);
int selinux_xfrm_postroute_last(u32 isec_sid, struct sk_buff *skb,
			struct common_audit_data *ad, u8 proto);
int selinux_xfrm_decode_session(struct sk_buff *skb, u32 *sid, int ckall);
<<<<<<< HEAD
<<<<<<< HEAD
int selinux_xfrm_skb_sid(struct sk_buff *skb, u32 *sid);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
int selinux_xfrm_skb_sid(struct sk_buff *skb, u32 *sid);
>>>>>>> refs/remotes/origin/cm-11.0

static inline void selinux_xfrm_notify_policyload(void)
{
	atomic_inc(&flow_cache_genid);
=======
int selinux_xfrm_sock_rcv_skb(u32 sk_sid, struct sk_buff *skb,
			      struct common_audit_data *ad);
int selinux_xfrm_postroute_last(u32 sk_sid, struct sk_buff *skb,
				struct common_audit_data *ad, u8 proto);
int selinux_xfrm_decode_session(struct sk_buff *skb, u32 *sid, int ckall);
int selinux_xfrm_skb_sid(struct sk_buff *skb, u32 *sid);

static inline void selinux_xfrm_notify_policyload(void)
{
	struct net *net;

	atomic_inc(&flow_cache_genid);
	rtnl_lock();
	for_each_net(net)
		rt_genid_bump_all(net);
	rtnl_unlock();
>>>>>>> refs/remotes/origin/master
}
#else
static inline int selinux_xfrm_enabled(void)
{
	return 0;
}

<<<<<<< HEAD
static inline int selinux_xfrm_sock_rcv_skb(u32 isec_sid, struct sk_buff *skb,
			struct common_audit_data *ad)
=======
static inline int selinux_xfrm_sock_rcv_skb(u32 sk_sid, struct sk_buff *skb,
					    struct common_audit_data *ad)
>>>>>>> refs/remotes/origin/master
{
	return 0;
}

<<<<<<< HEAD
static inline int selinux_xfrm_postroute_last(u32 isec_sid, struct sk_buff *skb,
			struct common_audit_data *ad, u8 proto)
=======
static inline int selinux_xfrm_postroute_last(u32 sk_sid, struct sk_buff *skb,
					      struct common_audit_data *ad,
					      u8 proto)
>>>>>>> refs/remotes/origin/master
{
	return 0;
}

<<<<<<< HEAD
static inline int selinux_xfrm_decode_session(struct sk_buff *skb, u32 *sid, int ckall)
=======
static inline int selinux_xfrm_decode_session(struct sk_buff *skb, u32 *sid,
					      int ckall)
>>>>>>> refs/remotes/origin/master
{
	*sid = SECSID_NULL;
	return 0;
}

static inline void selinux_xfrm_notify_policyload(void)
{
}
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/master

static inline int selinux_xfrm_skb_sid(struct sk_buff *skb, u32 *sid)
{
	*sid = SECSID_NULL;
	return 0;
}
#endif
<<<<<<< HEAD
=======
#endif
=======
>>>>>>> refs/remotes/origin/cm-11.0

static inline int selinux_xfrm_skb_sid(struct sk_buff *skb, u32 *sid)
{
	*sid = SECSID_NULL;
	return 0;
}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
#endif
>>>>>>> refs/remotes/origin/cm-11.0

#endif /* _SELINUX_XFRM_H_ */
