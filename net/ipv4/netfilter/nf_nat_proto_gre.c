/*
 * nf_nat_proto_gre.c
 *
 * NAT protocol helper module for GRE.
 *
 * GRE is a generic encapsulation protocol, which is generally not very
 * suited for NAT, as it has no protocol-specific part as port numbers.
 *
 * It has an optional key field, which may help us distinguishing two
 * connections between the same two hosts.
 *
 * GRE is defined in RFC 1701 and RFC 1702, as well as RFC 2784
 *
 * PPTP is built on top of a modified version of GRE, and has a mandatory
 * field called "CallID", which serves us for the same purpose as the key
 * field in plain GRE.
 *
 * Documentation about PPTP can be found in RFC 2637
 *
 * (C) 2000-2005 by Harald Welte <laforge@gnumonks.org>
 *
 * Development of this code funded by Astaro AG (http://www.astaro.com/)
 *
<<<<<<< HEAD
=======
 * (C) 2006-2012 Patrick McHardy <kaber@trash.net>
 *
>>>>>>> refs/remotes/origin/master
 */

#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/ip.h>

#include <net/netfilter/nf_nat.h>
<<<<<<< HEAD
#include <net/netfilter/nf_nat_rule.h>
#include <net/netfilter/nf_nat_protocol.h>
=======
#include <net/netfilter/nf_nat_l4proto.h>
>>>>>>> refs/remotes/origin/master
#include <linux/netfilter/nf_conntrack_proto_gre.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Harald Welte <laforge@gnumonks.org>");
MODULE_DESCRIPTION("Netfilter NAT protocol helper module for GRE");

/* generate unique tuple ... */
static void
<<<<<<< HEAD
gre_unique_tuple(struct nf_conntrack_tuple *tuple,
<<<<<<< HEAD
		 const struct nf_nat_range *range,
=======
		 const struct nf_nat_ipv4_range *range,
>>>>>>> refs/remotes/origin/cm-10.0
=======
gre_unique_tuple(const struct nf_nat_l3proto *l3proto,
		 struct nf_conntrack_tuple *tuple,
		 const struct nf_nat_range *range,
>>>>>>> refs/remotes/origin/master
		 enum nf_nat_manip_type maniptype,
		 const struct nf_conn *ct)
{
	static u_int16_t key;
	__be16 *keyptr;
	unsigned int min, i, range_size;

	/* If there is no master conntrack we are not PPTP,
	   do not change tuples */
	if (!ct->master)
		return;

<<<<<<< HEAD
<<<<<<< HEAD
	if (maniptype == IP_NAT_MANIP_SRC)
=======
	if (maniptype == NF_NAT_MANIP_SRC)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (maniptype == NF_NAT_MANIP_SRC)
>>>>>>> refs/remotes/origin/master
		keyptr = &tuple->src.u.gre.key;
	else
		keyptr = &tuple->dst.u.gre.key;

<<<<<<< HEAD
<<<<<<< HEAD
	if (!(range->flags & IP_NAT_RANGE_PROTO_SPECIFIED)) {
=======
	if (!(range->flags & NF_NAT_RANGE_PROTO_SPECIFIED)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!(range->flags & NF_NAT_RANGE_PROTO_SPECIFIED)) {
>>>>>>> refs/remotes/origin/master
		pr_debug("%p: NATing GRE PPTP\n", ct);
		min = 1;
		range_size = 0xffff;
	} else {
<<<<<<< HEAD
		min = ntohs(range->min.gre.key);
		range_size = ntohs(range->max.gre.key) - min + 1;
=======
		min = ntohs(range->min_proto.gre.key);
		range_size = ntohs(range->max_proto.gre.key) - min + 1;
>>>>>>> refs/remotes/origin/master
	}

	pr_debug("min = %u, range_size = %u\n", min, range_size);

	for (i = 0; ; ++key) {
		*keyptr = htons(min + key % range_size);
		if (++i == range_size || !nf_nat_used_tuple(tuple, ct))
			return;
	}

	pr_debug("%p: no NAT mapping\n", ct);
	return;
}

/* manipulate a GRE packet according to maniptype */
static bool
<<<<<<< HEAD
gre_manip_pkt(struct sk_buff *skb, unsigned int iphdroff,
=======
gre_manip_pkt(struct sk_buff *skb,
	      const struct nf_nat_l3proto *l3proto,
	      unsigned int iphdroff, unsigned int hdroff,
>>>>>>> refs/remotes/origin/master
	      const struct nf_conntrack_tuple *tuple,
	      enum nf_nat_manip_type maniptype)
{
	const struct gre_hdr *greh;
	struct gre_hdr_pptp *pgreh;
<<<<<<< HEAD
	const struct iphdr *iph = (struct iphdr *)(skb->data + iphdroff);
	unsigned int hdroff = iphdroff + iph->ihl * 4;
=======
>>>>>>> refs/remotes/origin/master

	/* pgreh includes two optional 32bit fields which are not required
	 * to be there.  That's where the magic '8' comes from */
	if (!skb_make_writable(skb, hdroff + sizeof(*pgreh) - 8))
		return false;

	greh = (void *)skb->data + hdroff;
	pgreh = (struct gre_hdr_pptp *)greh;

	/* we only have destination manip of a packet, since 'source key'
	 * is not present in the packet itself */
<<<<<<< HEAD
<<<<<<< HEAD
	if (maniptype != IP_NAT_MANIP_DST)
=======
	if (maniptype != NF_NAT_MANIP_DST)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (maniptype != NF_NAT_MANIP_DST)
>>>>>>> refs/remotes/origin/master
		return true;
	switch (greh->version) {
	case GRE_VERSION_1701:
		/* We do not currently NAT any GREv0 packets.
		 * Try to behave like "nf_nat_proto_unknown" */
		break;
	case GRE_VERSION_PPTP:
		pr_debug("call_id -> 0x%04x\n", ntohs(tuple->dst.u.gre.key));
		pgreh->call_id = tuple->dst.u.gre.key;
		break;
	default:
		pr_debug("can't nat unknown GRE version\n");
		return false;
	}
	return true;
}

<<<<<<< HEAD
static const struct nf_nat_protocol gre = {
	.protonum		= IPPROTO_GRE,
<<<<<<< HEAD
	.me			= THIS_MODULE,
=======
>>>>>>> refs/remotes/origin/cm-10.0
	.manip_pkt		= gre_manip_pkt,
	.in_range		= nf_nat_proto_in_range,
	.unique_tuple		= gre_unique_tuple,
#if defined(CONFIG_NF_CT_NETLINK) || defined(CONFIG_NF_CT_NETLINK_MODULE)
<<<<<<< HEAD
	.range_to_nlattr	= nf_nat_proto_range_to_nlattr,
=======
>>>>>>> refs/remotes/origin/cm-10.0
	.nlattr_to_range	= nf_nat_proto_nlattr_to_range,
=======
static const struct nf_nat_l4proto gre = {
	.l4proto		= IPPROTO_GRE,
	.manip_pkt		= gre_manip_pkt,
	.in_range		= nf_nat_l4proto_in_range,
	.unique_tuple		= gre_unique_tuple,
#if defined(CONFIG_NF_CT_NETLINK) || defined(CONFIG_NF_CT_NETLINK_MODULE)
	.nlattr_to_range	= nf_nat_l4proto_nlattr_to_range,
>>>>>>> refs/remotes/origin/master
#endif
};

static int __init nf_nat_proto_gre_init(void)
{
<<<<<<< HEAD
	return nf_nat_protocol_register(&gre);
=======
	return nf_nat_l4proto_register(NFPROTO_IPV4, &gre);
>>>>>>> refs/remotes/origin/master
}

static void __exit nf_nat_proto_gre_fini(void)
{
<<<<<<< HEAD
	nf_nat_protocol_unregister(&gre);
=======
	nf_nat_l4proto_unregister(NFPROTO_IPV4, &gre);
>>>>>>> refs/remotes/origin/master
}

module_init(nf_nat_proto_gre_init);
module_exit(nf_nat_proto_gre_fini);

void nf_nat_need_gre(void)
{
	return;
}
EXPORT_SYMBOL_GPL(nf_nat_need_gre);
