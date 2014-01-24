/*
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  INET is implemented using the  BSD Socket
 *		interface as the means of communication with the user level.
 *
 *		Definitions for the protocol dispatcher.
 *
 * Version:	@(#)protocol.h	1.0.2	05/07/93
 *
 * Author:	Fred N. van Kempen, <waltje@uWalt.NL.Mugnet.ORG>
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 *	Changes:
 *		Alan Cox	:	Added a name field and a frag handler
 *					field for later.
 *		Alan Cox	:	Cleaned up, and sorted types.
 *		Pedro Roque	:	inet6 protocols
 */
 
#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#include <linux/in6.h>
<<<<<<< HEAD
<<<<<<< HEAD
#if defined(CONFIG_IPV6) || defined (CONFIG_IPV6_MODULE)
=======
#if IS_ENABLED(CONFIG_IPV6)
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/ipv6.h>
#endif

#define MAX_INET_PROTOS	256		/* Must be a power of 2		*/


/* This is used to register protocols. */
struct net_protocol {
	int			(*handler)(struct sk_buff *skb);
	void			(*err_handler)(struct sk_buff *skb, u32 info);
	int			(*gso_send_check)(struct sk_buff *skb);
	struct sk_buff	       *(*gso_segment)(struct sk_buff *skb,
<<<<<<< HEAD
					       u32 features);
=======
					       netdev_features_t features);
>>>>>>> refs/remotes/origin/cm-10.0
	struct sk_buff	      **(*gro_receive)(struct sk_buff **head,
					       struct sk_buff *skb);
	int			(*gro_complete)(struct sk_buff *skb);
=======
#include <linux/skbuff.h>
#if IS_ENABLED(CONFIG_IPV6)
#include <linux/ipv6.h>
#endif
#include <linux/netdevice.h>

/* This is one larger than the largest protocol value that can be
 * found in an ipv4 or ipv6 header.  Since in both cases the protocol
 * value is presented in a __u8, this is defined to be 256.
 */
#define MAX_INET_PROTOS		256

/* This is used to register protocols. */
struct net_protocol {
	void			(*early_demux)(struct sk_buff *skb);
	int			(*handler)(struct sk_buff *skb);
	void			(*err_handler)(struct sk_buff *skb, u32 info);
>>>>>>> refs/remotes/origin/master
	unsigned int		no_policy:1,
				netns_ok:1;
};

<<<<<<< HEAD
<<<<<<< HEAD
#if defined(CONFIG_IPV6) || defined (CONFIG_IPV6_MODULE)
=======
#if IS_ENABLED(CONFIG_IPV6)
>>>>>>> refs/remotes/origin/cm-10.0
struct inet6_protocol {
=======
#if IS_ENABLED(CONFIG_IPV6)
struct inet6_protocol {
	void	(*early_demux)(struct sk_buff *skb);

>>>>>>> refs/remotes/origin/master
	int	(*handler)(struct sk_buff *skb);

	void	(*err_handler)(struct sk_buff *skb,
			       struct inet6_skb_parm *opt,
			       u8 type, u8 code, int offset,
			       __be32 info);
<<<<<<< HEAD

	int	(*gso_send_check)(struct sk_buff *skb);
	struct sk_buff *(*gso_segment)(struct sk_buff *skb,
<<<<<<< HEAD
				       u32 features);
=======
				       netdev_features_t features);
>>>>>>> refs/remotes/origin/cm-10.0
	struct sk_buff **(*gro_receive)(struct sk_buff **head,
					struct sk_buff *skb);
	int	(*gro_complete)(struct sk_buff *skb);

=======
>>>>>>> refs/remotes/origin/master
	unsigned int	flags;	/* INET6_PROTO_xxx */
};

#define INET6_PROTO_NOPOLICY	0x1
#define INET6_PROTO_FINAL	0x2
<<<<<<< HEAD
/* This should be set for any extension header which is compatible with GSO. */
#define INET6_PROTO_GSO_EXTHDR	0x4
#endif

=======
#endif

struct net_offload {
	struct offload_callbacks callbacks;
	unsigned int		 flags;	/* Flags used by IPv6 for now */
};
/* This should be set for any extension header which is compatible with GSO. */
#define INET6_PROTO_GSO_EXTHDR	0x1

>>>>>>> refs/remotes/origin/master
/* This is used to register socket interfaces for IP protocols.  */
struct inet_protosw {
	struct list_head list;

        /* These two fields form the lookup key.  */
	unsigned short	 type;	   /* This is the 2nd argument to socket(2). */
	unsigned short	 protocol; /* This is the L4 protocol number.  */

	struct proto	 *prot;
	const struct proto_ops *ops;
  
	char             no_check;   /* checksum on rcv/xmit/none? */
	unsigned char	 flags;      /* See INET_PROTOSW_* below.  */
};
#define INET_PROTOSW_REUSE 0x01	     /* Are ports automatically reusable? */
#define INET_PROTOSW_PERMANENT 0x02  /* Permanent protocols are unremovable. */
#define INET_PROTOSW_ICSK      0x04  /* Is this an inet_connection_sock? */

extern const struct net_protocol __rcu *inet_protos[MAX_INET_PROTOS];
<<<<<<< HEAD

<<<<<<< HEAD
#if defined(CONFIG_IPV6) || defined (CONFIG_IPV6_MODULE)
=======
#if IS_ENABLED(CONFIG_IPV6)
>>>>>>> refs/remotes/origin/cm-10.0
extern const struct inet6_protocol __rcu *inet6_protos[MAX_INET_PROTOS];
#endif

extern int	inet_add_protocol(const struct net_protocol *prot, unsigned char num);
extern int	inet_del_protocol(const struct net_protocol *prot, unsigned char num);
extern void	inet_register_protosw(struct inet_protosw *p);
extern void	inet_unregister_protosw(struct inet_protosw *p);

<<<<<<< HEAD
#if defined(CONFIG_IPV6) || defined (CONFIG_IPV6_MODULE)
=======
#if IS_ENABLED(CONFIG_IPV6)
>>>>>>> refs/remotes/origin/cm-10.0
extern int	inet6_add_protocol(const struct inet6_protocol *prot, unsigned char num);
extern int	inet6_del_protocol(const struct inet6_protocol *prot, unsigned char num);
extern int	inet6_register_protosw(struct inet_protosw *p);
extern void	inet6_unregister_protosw(struct inet_protosw *p);
#endif
=======
extern const struct net_offload __rcu *inet_offloads[MAX_INET_PROTOS];
extern const struct net_offload __rcu *inet6_offloads[MAX_INET_PROTOS];

#if IS_ENABLED(CONFIG_IPV6)
extern const struct inet6_protocol __rcu *inet6_protos[MAX_INET_PROTOS];
#endif

int inet_add_protocol(const struct net_protocol *prot, unsigned char num);
int inet_del_protocol(const struct net_protocol *prot, unsigned char num);
int inet_add_offload(const struct net_offload *prot, unsigned char num);
int inet_del_offload(const struct net_offload *prot, unsigned char num);
void inet_register_protosw(struct inet_protosw *p);
void inet_unregister_protosw(struct inet_protosw *p);

#if IS_ENABLED(CONFIG_IPV6)
int inet6_add_protocol(const struct inet6_protocol *prot, unsigned char num);
int inet6_del_protocol(const struct inet6_protocol *prot, unsigned char num);
int inet6_register_protosw(struct inet_protosw *p);
void inet6_unregister_protosw(struct inet_protosw *p);
#endif
int inet6_add_offload(const struct net_offload *prot, unsigned char num);
int inet6_del_offload(const struct net_offload *prot, unsigned char num);
>>>>>>> refs/remotes/origin/master

#endif	/* _PROTOCOL_H */
