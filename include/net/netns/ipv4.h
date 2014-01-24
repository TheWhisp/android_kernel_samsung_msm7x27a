/*
 * ipv4 in net namespaces
 */

#ifndef __NETNS_IPV4_H__
#define __NETNS_IPV4_H__

<<<<<<< HEAD
#include <net/inet_frag.h>

=======
#include <linux/uidgid.h>
#include <net/inet_frag.h>

struct tcpm_hash_bucket;
>>>>>>> refs/remotes/origin/master
struct ctl_table_header;
struct ipv4_devconf;
struct fib_rules_ops;
struct hlist_head;
<<<<<<< HEAD
struct sock;
=======
struct fib_table;
struct sock;
struct local_ports {
	seqlock_t	lock;
	int		range[2];
};
>>>>>>> refs/remotes/origin/master

struct netns_ipv4 {
#ifdef CONFIG_SYSCTL
	struct ctl_table_header	*forw_hdr;
	struct ctl_table_header	*frags_hdr;
	struct ctl_table_header	*ipv4_hdr;
	struct ctl_table_header *route_hdr;
<<<<<<< HEAD
=======
	struct ctl_table_header *xfrm4_hdr;
>>>>>>> refs/remotes/origin/master
#endif
	struct ipv4_devconf	*devconf_all;
	struct ipv4_devconf	*devconf_dflt;
#ifdef CONFIG_IP_MULTIPLE_TABLES
	struct fib_rules_ops	*rules_ops;
<<<<<<< HEAD
=======
	bool			fib_has_custom_rules;
	struct fib_table	*fib_local;
	struct fib_table	*fib_main;
	struct fib_table	*fib_default;
#endif
#ifdef CONFIG_IP_ROUTE_CLASSID
	int			fib_num_tclassid_users;
>>>>>>> refs/remotes/origin/master
#endif
	struct hlist_head	*fib_table_hash;
	struct sock		*fibnl;

	struct sock		**icmp_sk;
<<<<<<< HEAD
	struct sock		*tcp_sock;

=======
	struct inet_peer_base	*peers;
	struct tcpm_hash_bucket	*tcp_metrics_hash;
	unsigned int		tcp_metrics_hash_log;
>>>>>>> refs/remotes/origin/master
	struct netns_frags	frags;
#ifdef CONFIG_NETFILTER
	struct xt_table		*iptable_filter;
	struct xt_table		*iptable_mangle;
	struct xt_table		*iptable_raw;
	struct xt_table		*arptable_filter;
#ifdef CONFIG_SECURITY
	struct xt_table		*iptable_security;
#endif
	struct xt_table		*nat_table;
<<<<<<< HEAD
	struct hlist_head	*nat_bysource;
	unsigned int		nat_htable_size;
=======
>>>>>>> refs/remotes/origin/master
#endif

	int sysctl_icmp_echo_ignore_all;
	int sysctl_icmp_echo_ignore_broadcasts;
	int sysctl_icmp_ignore_bogus_error_responses;
	int sysctl_icmp_ratelimit;
	int sysctl_icmp_ratemask;
	int sysctl_icmp_errors_use_inbound_ifaddr;
<<<<<<< HEAD
	int sysctl_rt_cache_rebuild_count;
	int current_rt_cache_rebuild_count;

	unsigned int sysctl_ping_group_range[2];
<<<<<<< HEAD
=======
	long sysctl_tcp_mem[3];
>>>>>>> refs/remotes/origin/cm-10.0

	atomic_t rt_genid;
=======

	struct local_ports sysctl_local_ports;

	int sysctl_tcp_ecn;

	kgid_t sysctl_ping_group_range[2];

>>>>>>> refs/remotes/origin/master
	atomic_t dev_addr_genid;

#ifdef CONFIG_IP_MROUTE
#ifndef CONFIG_IP_MROUTE_MULTIPLE_TABLES
	struct mr_table		*mrt;
#else
	struct list_head	mr_tables;
	struct fib_rules_ops	*mr_rules_ops;
#endif
#endif
<<<<<<< HEAD
=======
	atomic_t	rt_genid;
>>>>>>> refs/remotes/origin/master
};
#endif
