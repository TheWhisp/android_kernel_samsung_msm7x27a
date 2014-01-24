<<<<<<< HEAD
#ifndef __LINUX_IP6_NETFILTER_H
#define __LINUX_IP6_NETFILTER_H

=======
>>>>>>> refs/remotes/origin/master
/* IPv6-specific defines for netfilter. 
 * (C)1998 Rusty Russell -- This code is GPL.
 * (C)1999 David Jeffery
 *   this header was blatantly ripped from netfilter_ipv4.h 
 *   it's amazing what adding a bunch of 6s can do =8^)
 */
<<<<<<< HEAD

#include <linux/netfilter.h>

/* only for userspace compatibility */
#ifndef __KERNEL__
<<<<<<< HEAD
=======

#include <limits.h> /* for INT_MIN, INT_MAX */

>>>>>>> refs/remotes/origin/cm-10.0
/* IP Cache bits. */
/* Src IP address. */
#define NFC_IP6_SRC              0x0001
/* Dest IP address. */
#define NFC_IP6_DST              0x0002
/* Input device. */
#define NFC_IP6_IF_IN            0x0004
/* Output device. */
#define NFC_IP6_IF_OUT           0x0008
/* TOS. */
#define NFC_IP6_TOS              0x0010
/* Protocol. */
#define NFC_IP6_PROTO            0x0020
/* IP options. */
#define NFC_IP6_OPTIONS          0x0040
/* Frag & flags. */
#define NFC_IP6_FRAG             0x0080


/* Per-protocol information: only matters if proto match. */
/* TCP flags. */
#define NFC_IP6_TCPFLAGS         0x0100
/* Source port. */
#define NFC_IP6_SRC_PT           0x0200
/* Dest port. */
#define NFC_IP6_DST_PT           0x0400
/* Something else about the proto */
#define NFC_IP6_PROTO_UNKNOWN    0x2000

/* IP6 Hooks */
/* After promisc drops, checksum checks. */
#define NF_IP6_PRE_ROUTING	0
/* If the packet is destined for this box. */
#define NF_IP6_LOCAL_IN		1
/* If the packet is destined for another interface. */
#define NF_IP6_FORWARD		2
/* Packets coming from a local process. */
#define NF_IP6_LOCAL_OUT		3
/* Packets about to hit the wire. */
#define NF_IP6_POST_ROUTING	4
#define NF_IP6_NUMHOOKS		5
#endif /* ! __KERNEL__ */


enum nf_ip6_hook_priorities {
	NF_IP6_PRI_FIRST = INT_MIN,
	NF_IP6_PRI_CONNTRACK_DEFRAG = -400,
	NF_IP6_PRI_RAW = -300,
	NF_IP6_PRI_SELINUX_FIRST = -225,
	NF_IP6_PRI_CONNTRACK = -200,
	NF_IP6_PRI_MANGLE = -150,
	NF_IP6_PRI_NAT_DST = -100,
	NF_IP6_PRI_FILTER = 0,
	NF_IP6_PRI_SECURITY = 50,
	NF_IP6_PRI_NAT_SRC = 100,
	NF_IP6_PRI_SELINUX_LAST = 225,
	NF_IP6_PRI_LAST = INT_MAX,
};

#ifdef  __KERNEL__

#ifdef CONFIG_NETFILTER
extern int ip6_route_me_harder(struct sk_buff *skb);
extern __sum16 nf_ip6_checksum(struct sk_buff *skb, unsigned int hook,
				    unsigned int dataoff, u_int8_t protocol);

extern int ipv6_netfilter_init(void);
extern void ipv6_netfilter_fini(void);
=======
#ifndef __LINUX_IP6_NETFILTER_H
#define __LINUX_IP6_NETFILTER_H

#include <uapi/linux/netfilter_ipv6.h>


#ifdef CONFIG_NETFILTER
int ip6_route_me_harder(struct sk_buff *skb);
__sum16 nf_ip6_checksum(struct sk_buff *skb, unsigned int hook,
			unsigned int dataoff, u_int8_t protocol);

int ipv6_netfilter_init(void);
void ipv6_netfilter_fini(void);

/*
 * Hook functions for ipv6 to allow xt_* modules to be built-in even
 * if IPv6 is a module.
 */
struct nf_ipv6_ops {
	int (*chk_addr)(struct net *net, const struct in6_addr *addr,
			const struct net_device *dev, int strict);
};

extern const struct nf_ipv6_ops __rcu *nf_ipv6_ops;
static inline const struct nf_ipv6_ops *nf_get_ipv6_ops(void)
{
	return rcu_dereference(nf_ipv6_ops);
}

>>>>>>> refs/remotes/origin/master
#else /* CONFIG_NETFILTER */
static inline int ipv6_netfilter_init(void) { return 0; }
static inline void ipv6_netfilter_fini(void) { return; }
#endif /* CONFIG_NETFILTER */

<<<<<<< HEAD
#endif /* __KERNEL__ */

=======
>>>>>>> refs/remotes/origin/master
#endif /*__LINUX_IP6_NETFILTER_H*/
