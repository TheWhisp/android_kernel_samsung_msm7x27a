/*
 * Common code for low-level network console, dump, and debugger code
 *
 * Derived from netconsole, kgdb-over-ethernet, and netdump patches
 */

#ifndef _LINUX_NETPOLL_H
#define _LINUX_NETPOLL_H

#include <linux/netdevice.h>
#include <linux/interrupt.h>
#include <linux/rcupdate.h>
#include <linux/list.h>

<<<<<<< HEAD
=======
union inet_addr {
	__u32		all[4];
	__be32		ip;
	__be32		ip6[4];
	struct in_addr	in;
	struct in6_addr	in6;
};

>>>>>>> refs/remotes/origin/master
struct netpoll {
	struct net_device *dev;
	char dev_name[IFNAMSIZ];
	const char *name;
<<<<<<< HEAD
	void (*rx_hook)(struct netpoll *, int, char *, int);

	__be32 local_ip, remote_ip;
=======
	void (*rx_skb_hook)(struct netpoll *np, int source, struct sk_buff *skb,
			    int offset, int len);

	union inet_addr local_ip, remote_ip;
	bool ipv6;
>>>>>>> refs/remotes/origin/master
	u16 local_port, remote_port;
	u8 remote_mac[ETH_ALEN];

	struct list_head rx; /* rx_np list element */
<<<<<<< HEAD
=======
	struct work_struct cleanup_work;
>>>>>>> refs/remotes/origin/master
};

struct netpoll_info {
	atomic_t refcnt;

<<<<<<< HEAD
	int rx_flags;
	spinlock_t rx_lock;
	struct list_head rx_np; /* netpolls that registered an rx_hook */

	struct sk_buff_head arp_tx; /* list of arp requests to reply to */
=======
	unsigned long rx_flags;
	spinlock_t rx_lock;
	struct semaphore dev_lock;
	struct list_head rx_np; /* netpolls that registered an rx_skb_hook */

	struct sk_buff_head neigh_tx; /* list of neigh requests to reply to */
>>>>>>> refs/remotes/origin/master
	struct sk_buff_head txq;

	struct delayed_work tx_work;

	struct netpoll *netpoll;
<<<<<<< HEAD
};

<<<<<<< HEAD
void netpoll_poll_dev(struct net_device *dev);
void netpoll_poll(struct netpoll *np);
=======
>>>>>>> refs/remotes/origin/cm-10.0
void netpoll_send_udp(struct netpoll *np, const char *msg, int len);
void netpoll_print_options(struct netpoll *np);
int netpoll_parse_options(struct netpoll *np, char *opt);
int __netpoll_setup(struct netpoll *np);
=======
	struct rcu_head rcu;
};

#ifdef CONFIG_NETPOLL
extern void netpoll_rx_disable(struct net_device *dev);
extern void netpoll_rx_enable(struct net_device *dev);
#else
static inline void netpoll_rx_disable(struct net_device *dev) { return; }
static inline void netpoll_rx_enable(struct net_device *dev) { return; }
#endif

void netpoll_send_udp(struct netpoll *np, const char *msg, int len);
void netpoll_print_options(struct netpoll *np);
int netpoll_parse_options(struct netpoll *np, char *opt);
int __netpoll_setup(struct netpoll *np, struct net_device *ndev, gfp_t gfp);
>>>>>>> refs/remotes/origin/master
int netpoll_setup(struct netpoll *np);
int netpoll_trap(void);
void netpoll_set_trap(int trap);
void __netpoll_cleanup(struct netpoll *np);
<<<<<<< HEAD
void netpoll_cleanup(struct netpoll *np);
int __netpoll_rx(struct sk_buff *skb);
=======
void __netpoll_free_async(struct netpoll *np);
void netpoll_cleanup(struct netpoll *np);
int __netpoll_rx(struct sk_buff *skb, struct netpoll_info *npinfo);
>>>>>>> refs/remotes/origin/master
void netpoll_send_skb_on_dev(struct netpoll *np, struct sk_buff *skb,
			     struct net_device *dev);
static inline void netpoll_send_skb(struct netpoll *np, struct sk_buff *skb)
{
<<<<<<< HEAD
	netpoll_send_skb_on_dev(np, skb, np->dev);
=======
	unsigned long flags;
	local_irq_save(flags);
	netpoll_send_skb_on_dev(np, skb, np->dev);
	local_irq_restore(flags);
>>>>>>> refs/remotes/origin/master
}



#ifdef CONFIG_NETPOLL
<<<<<<< HEAD
=======
static inline bool netpoll_rx_on(struct sk_buff *skb)
{
	struct netpoll_info *npinfo = rcu_dereference_bh(skb->dev->npinfo);

	return npinfo && (!list_empty(&npinfo->rx_np) || npinfo->rx_flags);
}

>>>>>>> refs/remotes/origin/master
static inline bool netpoll_rx(struct sk_buff *skb)
{
	struct netpoll_info *npinfo;
	unsigned long flags;
	bool ret = false;

	local_irq_save(flags);
<<<<<<< HEAD
	npinfo = rcu_dereference_bh(skb->dev->npinfo);

	if (!npinfo || (list_empty(&npinfo->rx_np) && !npinfo->rx_flags))
		goto out;

	spin_lock(&npinfo->rx_lock);
	/* check rx_flags again with the lock held */
	if (npinfo->rx_flags && __netpoll_rx(skb))
=======

	if (!netpoll_rx_on(skb))
		goto out;

	npinfo = rcu_dereference_bh(skb->dev->npinfo);
	spin_lock(&npinfo->rx_lock);
	/* check rx_flags again with the lock held */
	if (npinfo->rx_flags && __netpoll_rx(skb, npinfo))
>>>>>>> refs/remotes/origin/master
		ret = true;
	spin_unlock(&npinfo->rx_lock);

out:
	local_irq_restore(flags);
	return ret;
}

<<<<<<< HEAD
static inline int netpoll_rx_on(struct sk_buff *skb)
{
	struct netpoll_info *npinfo = rcu_dereference_bh(skb->dev->npinfo);

	return npinfo && (!list_empty(&npinfo->rx_np) || npinfo->rx_flags);
}

=======
>>>>>>> refs/remotes/origin/master
static inline int netpoll_receive_skb(struct sk_buff *skb)
{
	if (!list_empty(&skb->dev->napi_list))
		return netpoll_rx(skb);
	return 0;
}

static inline void *netpoll_poll_lock(struct napi_struct *napi)
{
	struct net_device *dev = napi->dev;

	if (dev && dev->npinfo) {
		spin_lock(&napi->poll_lock);
		napi->poll_owner = smp_processor_id();
		return napi;
	}
	return NULL;
}

static inline void netpoll_poll_unlock(void *have)
{
	struct napi_struct *napi = have;

	if (napi) {
		napi->poll_owner = -1;
		spin_unlock(&napi->poll_lock);
	}
}

<<<<<<< HEAD
static inline int netpoll_tx_running(struct net_device *dev)
=======
static inline bool netpoll_tx_running(struct net_device *dev)
>>>>>>> refs/remotes/origin/master
{
	return irqs_disabled();
}

#else
static inline bool netpoll_rx(struct sk_buff *skb)
{
<<<<<<< HEAD
	return 0;
}
static inline int netpoll_rx_on(struct sk_buff *skb)
{
	return 0;
=======
	return false;
}
static inline bool netpoll_rx_on(struct sk_buff *skb)
{
	return false;
>>>>>>> refs/remotes/origin/master
}
static inline int netpoll_receive_skb(struct sk_buff *skb)
{
	return 0;
}
static inline void *netpoll_poll_lock(struct napi_struct *napi)
{
	return NULL;
}
static inline void netpoll_poll_unlock(void *have)
{
}
static inline void netpoll_netdev_init(struct net_device *dev)
{
}
<<<<<<< HEAD
static inline int netpoll_tx_running(struct net_device *dev)
{
	return 0;
=======
static inline bool netpoll_tx_running(struct net_device *dev)
{
	return false;
>>>>>>> refs/remotes/origin/master
}
#endif

#endif
