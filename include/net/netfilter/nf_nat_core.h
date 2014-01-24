#ifndef _NF_NAT_CORE_H
#define _NF_NAT_CORE_H
#include <linux/list.h>
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_nat.h>

/* This header used to share core functionality between the standalone
   NAT module, and the compatibility layer's use of NAT for masquerading. */

<<<<<<< HEAD
extern unsigned int nf_nat_packet(struct nf_conn *ct,
				  enum ip_conntrack_info ctinfo,
				  unsigned int hooknum,
				  struct sk_buff *skb);

extern int nf_nat_icmp_reply_translation(struct nf_conn *ct,
					 enum ip_conntrack_info ctinfo,
					 unsigned int hooknum,
					 struct sk_buff *skb);
=======
unsigned int nf_nat_packet(struct nf_conn *ct, enum ip_conntrack_info ctinfo,
			   unsigned int hooknum, struct sk_buff *skb);

int nf_xfrm_me_harder(struct sk_buff *skb, unsigned int family);
>>>>>>> refs/remotes/origin/master

static inline int nf_nat_initialized(struct nf_conn *ct,
				     enum nf_nat_manip_type manip)
{
<<<<<<< HEAD
<<<<<<< HEAD
	if (manip == IP_NAT_MANIP_SRC)
=======
	if (manip == NF_NAT_MANIP_SRC)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (manip == NF_NAT_MANIP_SRC)
>>>>>>> refs/remotes/origin/master
		return ct->status & IPS_SRC_NAT_DONE;
	else
		return ct->status & IPS_DST_NAT_DONE;
}

struct nlattr;

extern int
(*nfnetlink_parse_nat_setup_hook)(struct nf_conn *ct,
				  enum nf_nat_manip_type manip,
				  const struct nlattr *attr);

#endif /* _NF_NAT_CORE_H */
