/*
 * VLAN		An implementation of 802.1Q VLAN tagging.
 *
 * Authors:	Ben Greear <greearb@candelatech.com>
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 */
<<<<<<< HEAD

#ifndef _LINUX_IF_VLAN_H_
#define _LINUX_IF_VLAN_H_

#ifdef __KERNEL__
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/rtnetlink.h>
<<<<<<< HEAD

#define VLAN_HLEN	4		/* The additional bytes (on top of the Ethernet header)
					 * that VLAN requires.
					 */
#define VLAN_ETH_ALEN	6		/* Octets in one ethernet addr	 */
=======
#include <linux/bug.h>
=======
#ifndef _LINUX_IF_VLAN_H_
#define _LINUX_IF_VLAN_H_

#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/rtnetlink.h>
#include <linux/bug.h>
#include <uapi/linux/if_vlan.h>
>>>>>>> refs/remotes/origin/master

#define VLAN_HLEN	4		/* The additional bytes required by VLAN
					 * (in addition to the Ethernet header)
					 */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#define VLAN_ETH_HLEN	18		/* Total octets in header.	 */
#define VLAN_ETH_ZLEN	64		/* Min. octets in frame sans FCS */

/*
 * According to 802.3ac, the packet can be 4 bytes longer. --Klika Jan
 */
#define VLAN_ETH_DATA_LEN	1500	/* Max. octets in payload	 */
#define VLAN_ETH_FRAME_LEN	1518	/* Max. octets in frame sans FCS */

/*
 * 	struct vlan_hdr - vlan header
 * 	@h_vlan_TCI: priority and VLAN ID
 *	@h_vlan_encapsulated_proto: packet type ID or len
 */
struct vlan_hdr {
	__be16	h_vlan_TCI;
	__be16	h_vlan_encapsulated_proto;
};

/**
 *	struct vlan_ethhdr - vlan ethernet header (ethhdr + vlan_hdr)
 *	@h_dest: destination ethernet address
 *	@h_source: source ethernet address
<<<<<<< HEAD
 *	@h_vlan_proto: ethernet protocol (always 0x8100)
=======
 *	@h_vlan_proto: ethernet protocol
>>>>>>> refs/remotes/origin/master
 *	@h_vlan_TCI: priority and VLAN ID
 *	@h_vlan_encapsulated_proto: packet type ID or len
 */
struct vlan_ethhdr {
	unsigned char	h_dest[ETH_ALEN];
	unsigned char	h_source[ETH_ALEN];
	__be16		h_vlan_proto;
	__be16		h_vlan_TCI;
	__be16		h_vlan_encapsulated_proto;
};

#include <linux/skbuff.h>

static inline struct vlan_ethhdr *vlan_eth_hdr(const struct sk_buff *skb)
{
	return (struct vlan_ethhdr *)skb_mac_header(skb);
}

#define VLAN_PRIO_MASK		0xe000 /* Priority Code Point */
#define VLAN_PRIO_SHIFT		13
#define VLAN_CFI_MASK		0x1000 /* Canonical Format Indicator */
#define VLAN_TAG_PRESENT	VLAN_CFI_MASK
#define VLAN_VID_MASK		0x0fff /* VLAN Identifier */
#define VLAN_N_VID		4096

/* found in socket.c */
extern void vlan_ioctl_set(int (*hook)(struct net *, void __user *));

<<<<<<< HEAD
<<<<<<< HEAD
/* if this changes, algorithm will have to be reworked because this
 * depends on completely exhausting the VLAN identifier space.  Thus
 * it gives constant time look-up, but in many cases it wastes memory.
 */
#define VLAN_GROUP_ARRAY_SPLIT_PARTS  8
#define VLAN_GROUP_ARRAY_PART_LEN     (VLAN_N_VID/VLAN_GROUP_ARRAY_SPLIT_PARTS)

struct vlan_group {
	struct net_device	*real_dev; /* The ethernet(like) device
					    * the vlan is attached to.
					    */
	unsigned int		nr_vlans;
	struct hlist_node	hlist;	/* linked list */
	struct net_device **vlan_devices_arrays[VLAN_GROUP_ARRAY_SPLIT_PARTS];
	struct rcu_head		rcu;
};

static inline struct net_device *vlan_group_get_device(struct vlan_group *vg,
						       u16 vlan_id)
{
	struct net_device **array;
	array = vg->vlan_devices_arrays[vlan_id / VLAN_GROUP_ARRAY_PART_LEN];
	return array ? array[vlan_id % VLAN_GROUP_ARRAY_PART_LEN] : NULL;
}

static inline void vlan_group_set_device(struct vlan_group *vg,
					 u16 vlan_id,
					 struct net_device *dev)
{
	struct net_device **array;
	if (!vg)
		return;
	array = vg->vlan_devices_arrays[vlan_id / VLAN_GROUP_ARRAY_PART_LEN];
	array[vlan_id % VLAN_GROUP_ARRAY_PART_LEN] = dev;
}
=======
struct vlan_info;
>>>>>>> refs/remotes/origin/cm-10.0

=======
>>>>>>> refs/remotes/origin/master
static inline int is_vlan_dev(struct net_device *dev)
{
        return dev->priv_flags & IFF_802_1Q_VLAN;
}

#define vlan_tx_tag_present(__skb)	((__skb)->vlan_tci & VLAN_TAG_PRESENT)
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
#define vlan_tx_tag_get(__skb)		((__skb)->vlan_tci & ~VLAN_TAG_PRESENT)

#if defined(CONFIG_VLAN_8021Q) || defined(CONFIG_VLAN_8021Q_MODULE)
/* Must be invoked with rcu_read_lock or with RTNL. */
static inline struct net_device *vlan_find_dev(struct net_device *real_dev,
					       u16 vlan_id)
{
	struct vlan_group *grp = rcu_dereference_rtnl(real_dev->vlgrp);

	if (grp)
		return vlan_group_get_device(grp, vlan_id);

	return NULL;
}

extern struct net_device *vlan_dev_real_dev(const struct net_device *dev);
extern u16 vlan_dev_vlan_id(const struct net_device *dev);

extern int __vlan_hwaccel_rx(struct sk_buff *skb, struct vlan_group *grp,
			     u16 vlan_tci, int polling);
extern bool vlan_do_receive(struct sk_buff **skb);
extern struct sk_buff *vlan_untag(struct sk_buff *skb);
extern gro_result_t
vlan_gro_receive(struct napi_struct *napi, struct vlan_group *grp,
		 unsigned int vlan_tci, struct sk_buff *skb);
extern gro_result_t
vlan_gro_frags(struct napi_struct *napi, struct vlan_group *grp,
	       unsigned int vlan_tci);

#else
static inline struct net_device *vlan_find_dev(struct net_device *real_dev,
					       u16 vlan_id)
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
#define vlan_tx_nonzero_tag_present(__skb) \
	(vlan_tx_tag_present(__skb) && ((__skb)->vlan_tci & VLAN_VID_MASK))
#define vlan_tx_tag_get(__skb)		((__skb)->vlan_tci & ~VLAN_TAG_PRESENT)

#if defined(CONFIG_VLAN_8021Q) || defined(CONFIG_VLAN_8021Q_MODULE)

extern struct net_device *__vlan_find_dev_deep(struct net_device *real_dev,
					       u16 vlan_id);
extern struct net_device *vlan_dev_real_dev(const struct net_device *dev);
extern u16 vlan_dev_vlan_id(const struct net_device *dev);
=======
#define vlan_tx_tag_get(__skb)		((__skb)->vlan_tci & ~VLAN_TAG_PRESENT)
#define vlan_tx_tag_get_id(__skb)	((__skb)->vlan_tci & VLAN_VID_MASK)

#if defined(CONFIG_VLAN_8021Q) || defined(CONFIG_VLAN_8021Q_MODULE)

extern struct net_device *__vlan_find_dev_deep(struct net_device *real_dev,
					       __be16 vlan_proto, u16 vlan_id);
extern struct net_device *vlan_dev_real_dev(const struct net_device *dev);
extern u16 vlan_dev_vlan_id(const struct net_device *dev);

/**
 *	struct vlan_priority_tci_mapping - vlan egress priority mappings
 *	@priority: skb priority
 *	@vlan_qos: vlan priority: (skb->priority << 13) & 0xE000
 *	@next: pointer to next struct
 */
struct vlan_priority_tci_mapping {
	u32					priority;
	u16					vlan_qos;
	struct vlan_priority_tci_mapping	*next;
};

/**
 *	struct vlan_pcpu_stats - VLAN percpu rx/tx stats
 *	@rx_packets: number of received packets
 *	@rx_bytes: number of received bytes
 *	@rx_multicast: number of received multicast packets
 *	@tx_packets: number of transmitted packets
 *	@tx_bytes: number of transmitted bytes
 *	@syncp: synchronization point for 64bit counters
 *	@rx_errors: number of rx errors
 *	@tx_dropped: number of tx drops
 */
struct vlan_pcpu_stats {
	u64			rx_packets;
	u64			rx_bytes;
	u64			rx_multicast;
	u64			tx_packets;
	u64			tx_bytes;
	struct u64_stats_sync	syncp;
	u32			rx_errors;
	u32			tx_dropped;
};

struct proc_dir_entry;
struct netpoll;

/**
 *	struct vlan_dev_priv - VLAN private device data
 *	@nr_ingress_mappings: number of ingress priority mappings
 *	@ingress_priority_map: ingress priority mappings
 *	@nr_egress_mappings: number of egress priority mappings
 *	@egress_priority_map: hash of egress priority mappings
 *	@vlan_proto: VLAN encapsulation protocol
 *	@vlan_id: VLAN identifier
 *	@flags: device flags
 *	@real_dev: underlying netdevice
 *	@real_dev_addr: address of underlying netdevice
 *	@dent: proc dir entry
 *	@vlan_pcpu_stats: ptr to percpu rx stats
 */
struct vlan_dev_priv {
	unsigned int				nr_ingress_mappings;
	u32					ingress_priority_map[8];
	unsigned int				nr_egress_mappings;
	struct vlan_priority_tci_mapping	*egress_priority_map[16];

	__be16					vlan_proto;
	u16					vlan_id;
	u16					flags;

	struct net_device			*real_dev;
	unsigned char				real_dev_addr[ETH_ALEN];

	struct proc_dir_entry			*dent;
	struct vlan_pcpu_stats __percpu		*vlan_pcpu_stats;
#ifdef CONFIG_NET_POLL_CONTROLLER
	struct netpoll				*netpoll;
#endif
};

<<<<<<< HEAD
static inline struct vlan_dev_priv *vlan_dev_priv(const struct net_device *dev)
{
	return netdev_priv(dev);
}

static inline u16
vlan_dev_get_egress_qos_mask(struct net_device *dev, u32 skprio)
{
	struct vlan_priority_tci_mapping *mp;

	smp_rmb(); /* coupled with smp_wmb() in vlan_dev_set_egress_priority() */

	mp = vlan_dev_priv(dev)->egress_priority_map[(skprio & 0xF)];
	while (mp) {
		if (mp->priority == skprio) {
			return mp->vlan_qos; /* This should already be shifted
					      * to mask correctly with the
					      * VLAN's TCI */
		}
		mp = mp->next;
	}
	return 0;
}
>>>>>>> refs/remotes/origin/master

=======
>>>>>>> refs/remotes/origin/cm-11.0
extern bool vlan_do_receive(struct sk_buff **skb);
extern struct sk_buff *vlan_untag(struct sk_buff *skb);

<<<<<<< HEAD
extern int vlan_vid_add(struct net_device *dev, unsigned short vid);
extern void vlan_vid_del(struct net_device *dev, unsigned short vid);
=======
extern int vlan_vid_add(struct net_device *dev, __be16 proto, u16 vid);
extern void vlan_vid_del(struct net_device *dev, __be16 proto, u16 vid);
>>>>>>> refs/remotes/origin/master

extern int vlan_vids_add_by_dev(struct net_device *dev,
				const struct net_device *by_dev);
extern void vlan_vids_del_by_dev(struct net_device *dev,
				 const struct net_device *by_dev);
<<<<<<< HEAD
#else
static inline struct net_device *
__vlan_find_dev_deep(struct net_device *real_dev, u16 vlan_id)
>>>>>>> refs/remotes/origin/cm-10.0
=======

extern bool vlan_uses_dev(const struct net_device *dev);
#else
static inline struct net_device *
__vlan_find_dev_deep(struct net_device *real_dev,
		     __be16 vlan_proto, u16 vlan_id)
>>>>>>> refs/remotes/origin/master
{
	return NULL;
}

static inline struct net_device *vlan_dev_real_dev(const struct net_device *dev)
{
	BUG();
	return NULL;
}

static inline u16 vlan_dev_vlan_id(const struct net_device *dev)
{
	BUG();
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
static inline int __vlan_hwaccel_rx(struct sk_buff *skb, struct vlan_group *grp,
				    u16 vlan_tci, int polling)
{
	BUG();
	return NET_XMIT_SUCCESS;
=======
static inline u16 vlan_dev_get_egress_qos_mask(struct net_device *dev,
					       u32 skprio)
{
	return 0;
>>>>>>> refs/remotes/origin/master
}

static inline bool vlan_do_receive(struct sk_buff **skb)
{
<<<<<<< HEAD
	if ((*skb)->vlan_tci & VLAN_VID_MASK)
		(*skb)->pkt_type = PACKET_OTHERHOST;
=======
static inline bool vlan_do_receive(struct sk_buff **skb)
{
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
static inline bool vlan_do_receive(struct sk_buff **skb)
{
>>>>>>> refs/remotes/origin/cm-11.0
	return false;
}

static inline struct sk_buff *vlan_untag(struct sk_buff *skb)
{
	return skb;
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline gro_result_t
vlan_gro_receive(struct napi_struct *napi, struct vlan_group *grp,
		 unsigned int vlan_tci, struct sk_buff *skb)
{
	return GRO_DROP;
}

static inline gro_result_t
vlan_gro_frags(struct napi_struct *napi, struct vlan_group *grp,
	       unsigned int vlan_tci)
{
	return GRO_DROP;
}
#endif

/**
 * vlan_hwaccel_rx - netif_rx wrapper for VLAN RX acceleration
 * @skb: buffer
 * @grp: vlan group
 * @vlan_tci: VLAN TCI as received from the card
 */
static inline int vlan_hwaccel_rx(struct sk_buff *skb,
				  struct vlan_group *grp,
				  u16 vlan_tci)
{
	return __vlan_hwaccel_rx(skb, grp, vlan_tci, 0);
}

/**
 * vlan_hwaccel_receive_skb - netif_receive_skb wrapper for VLAN RX acceleration
 * @skb: buffer
 * @grp: vlan group
 * @vlan_tci: VLAN TCI as received from the card
 */
static inline int vlan_hwaccel_receive_skb(struct sk_buff *skb,
					   struct vlan_group *grp,
					   u16 vlan_tci)
{
	return __vlan_hwaccel_rx(skb, grp, vlan_tci, 1);
}
=======
static inline int vlan_vid_add(struct net_device *dev, unsigned short vid)
=======
static inline int vlan_vid_add(struct net_device *dev, __be16 proto, u16 vid)
>>>>>>> refs/remotes/origin/master
{
	return 0;
}

<<<<<<< HEAD
static inline void vlan_vid_del(struct net_device *dev, unsigned short vid)
=======
static inline void vlan_vid_del(struct net_device *dev, __be16 proto, u16 vid)
>>>>>>> refs/remotes/origin/master
{
}

static inline int vlan_vids_add_by_dev(struct net_device *dev,
				       const struct net_device *by_dev)
{
	return 0;
}

static inline void vlan_vids_del_by_dev(struct net_device *dev,
					const struct net_device *by_dev)
{
}
<<<<<<< HEAD
#endif
>>>>>>> refs/remotes/origin/cm-10.0
=======

static inline bool vlan_uses_dev(const struct net_device *dev)
{
	return false;
}
#endif

static inline bool vlan_hw_offload_capable(netdev_features_t features,
					   __be16 proto)
{
	if (proto == htons(ETH_P_8021Q) && features & NETIF_F_HW_VLAN_CTAG_TX)
		return true;
	if (proto == htons(ETH_P_8021AD) && features & NETIF_F_HW_VLAN_STAG_TX)
		return true;
	return false;
}
>>>>>>> refs/remotes/origin/master

/**
 * vlan_insert_tag - regular VLAN tag inserting
 * @skb: skbuff to tag
<<<<<<< HEAD
=======
 * @vlan_proto: VLAN encapsulation protocol
>>>>>>> refs/remotes/origin/master
 * @vlan_tci: VLAN TCI to insert
 *
 * Inserts the VLAN tag into @skb as part of the payload
 * Returns a VLAN tagged skb. If a new skb is created, @skb is freed.
 *
 * Following the skb_unshare() example, in case of error, the calling function
 * doesn't have to worry about freeing the original skb.
 *
 * Does not change skb->protocol so this function can be used during receive.
 */
<<<<<<< HEAD
static inline struct sk_buff *vlan_insert_tag(struct sk_buff *skb, u16 vlan_tci)
=======
static inline struct sk_buff *vlan_insert_tag(struct sk_buff *skb,
					      __be16 vlan_proto, u16 vlan_tci)
>>>>>>> refs/remotes/origin/master
{
	struct vlan_ethhdr *veth;

	if (skb_cow_head(skb, VLAN_HLEN) < 0) {
		kfree_skb(skb);
		return NULL;
	}
	veth = (struct vlan_ethhdr *)skb_push(skb, VLAN_HLEN);

	/* Move the mac addresses to the beginning of the new header. */
<<<<<<< HEAD
<<<<<<< HEAD
	memmove(skb->data, skb->data + VLAN_HLEN, 2 * VLAN_ETH_ALEN);
=======
	memmove(skb->data, skb->data + VLAN_HLEN, 2 * ETH_ALEN);
>>>>>>> refs/remotes/origin/cm-10.0
	skb->mac_header -= VLAN_HLEN;

	/* first, the ethernet type */
	veth->h_vlan_proto = htons(ETH_P_8021Q);
=======
	memmove(skb->data, skb->data + VLAN_HLEN, 2 * ETH_ALEN);
	skb->mac_header -= VLAN_HLEN;

	/* first, the ethernet type */
	veth->h_vlan_proto = vlan_proto;
>>>>>>> refs/remotes/origin/master

	/* now, the TCI */
	veth->h_vlan_TCI = htons(vlan_tci);

	return skb;
}

/**
 * __vlan_put_tag - regular VLAN tag inserting
 * @skb: skbuff to tag
 * @vlan_tci: VLAN TCI to insert
 *
 * Inserts the VLAN tag into @skb as part of the payload
 * Returns a VLAN tagged skb. If a new skb is created, @skb is freed.
 *
 * Following the skb_unshare() example, in case of error, the calling function
 * doesn't have to worry about freeing the original skb.
 */
<<<<<<< HEAD
static inline struct sk_buff *__vlan_put_tag(struct sk_buff *skb, u16 vlan_tci)
{
	skb = vlan_insert_tag(skb, vlan_tci);
	if (skb)
		skb->protocol = htons(ETH_P_8021Q);
=======
static inline struct sk_buff *__vlan_put_tag(struct sk_buff *skb,
					     __be16 vlan_proto, u16 vlan_tci)
{
	skb = vlan_insert_tag(skb, vlan_proto, vlan_tci);
	if (skb)
		skb->protocol = vlan_proto;
>>>>>>> refs/remotes/origin/master
	return skb;
}

/**
 * __vlan_hwaccel_put_tag - hardware accelerated VLAN inserting
 * @skb: skbuff to tag
<<<<<<< HEAD
=======
 * @vlan_proto: VLAN encapsulation protocol
>>>>>>> refs/remotes/origin/master
 * @vlan_tci: VLAN TCI to insert
 *
 * Puts the VLAN TCI in @skb->vlan_tci and lets the device do the rest
 */
static inline struct sk_buff *__vlan_hwaccel_put_tag(struct sk_buff *skb,
<<<<<<< HEAD
						     u16 vlan_tci)
{
=======
						     __be16 vlan_proto,
						     u16 vlan_tci)
{
	skb->vlan_proto = vlan_proto;
>>>>>>> refs/remotes/origin/master
	skb->vlan_tci = VLAN_TAG_PRESENT | vlan_tci;
	return skb;
}

<<<<<<< HEAD
#define HAVE_VLAN_PUT_TAG

=======
>>>>>>> refs/remotes/origin/master
/**
 * vlan_put_tag - inserts VLAN tag according to device features
 * @skb: skbuff to tag
 * @vlan_tci: VLAN TCI to insert
 *
 * Assumes skb->dev is the target that will xmit this frame.
 * Returns a VLAN tagged skb.
 */
<<<<<<< HEAD
static inline struct sk_buff *vlan_put_tag(struct sk_buff *skb, u16 vlan_tci)
{
	if (skb->dev->features & NETIF_F_HW_VLAN_TX) {
		return __vlan_hwaccel_put_tag(skb, vlan_tci);
	} else {
		return __vlan_put_tag(skb, vlan_tci);
=======
static inline struct sk_buff *vlan_put_tag(struct sk_buff *skb,
					   __be16 vlan_proto, u16 vlan_tci)
{
	if (vlan_hw_offload_capable(skb->dev->features, vlan_proto)) {
		return __vlan_hwaccel_put_tag(skb, vlan_proto, vlan_tci);
	} else {
		return __vlan_put_tag(skb, vlan_proto, vlan_tci);
>>>>>>> refs/remotes/origin/master
	}
}

/**
 * __vlan_get_tag - get the VLAN ID that is part of the payload
 * @skb: skbuff to query
 * @vlan_tci: buffer to store vlaue
 *
 * Returns error if the skb is not of VLAN type
 */
static inline int __vlan_get_tag(const struct sk_buff *skb, u16 *vlan_tci)
{
	struct vlan_ethhdr *veth = (struct vlan_ethhdr *)skb->data;

<<<<<<< HEAD
	if (veth->h_vlan_proto != htons(ETH_P_8021Q)) {
		return -EINVAL;
	}
=======
	if (veth->h_vlan_proto != htons(ETH_P_8021Q) &&
	    veth->h_vlan_proto != htons(ETH_P_8021AD))
		return -EINVAL;
>>>>>>> refs/remotes/origin/master

	*vlan_tci = ntohs(veth->h_vlan_TCI);
	return 0;
}

/**
 * __vlan_hwaccel_get_tag - get the VLAN ID that is in @skb->cb[]
 * @skb: skbuff to query
 * @vlan_tci: buffer to store vlaue
 *
 * Returns error if @skb->vlan_tci is not set correctly
 */
static inline int __vlan_hwaccel_get_tag(const struct sk_buff *skb,
					 u16 *vlan_tci)
{
	if (vlan_tx_tag_present(skb)) {
		*vlan_tci = vlan_tx_tag_get(skb);
		return 0;
	} else {
		*vlan_tci = 0;
		return -EINVAL;
	}
}

#define HAVE_VLAN_GET_TAG

/**
 * vlan_get_tag - get the VLAN ID from the skb
 * @skb: skbuff to query
 * @vlan_tci: buffer to store vlaue
 *
 * Returns error if the skb is not VLAN tagged
 */
static inline int vlan_get_tag(const struct sk_buff *skb, u16 *vlan_tci)
{
<<<<<<< HEAD
	if (skb->dev->features & NETIF_F_HW_VLAN_TX) {
=======
	if (skb->dev->features & NETIF_F_HW_VLAN_CTAG_TX) {
>>>>>>> refs/remotes/origin/master
		return __vlan_hwaccel_get_tag(skb, vlan_tci);
	} else {
		return __vlan_get_tag(skb, vlan_tci);
	}
}

/**
 * vlan_get_protocol - get protocol EtherType.
 * @skb: skbuff to query
 *
 * Returns the EtherType of the packet, regardless of whether it is
 * vlan encapsulated (normal or hardware accelerated) or not.
 */
static inline __be16 vlan_get_protocol(const struct sk_buff *skb)
{
	__be16 protocol = 0;

	if (vlan_tx_tag_present(skb) ||
	     skb->protocol != cpu_to_be16(ETH_P_8021Q))
		protocol = skb->protocol;
	else {
		__be16 proto, *protop;
		protop = skb_header_pointer(skb, offsetof(struct vlan_ethhdr,
						h_vlan_encapsulated_proto),
						sizeof(proto), &proto);
		if (likely(protop))
			protocol = *protop;
	}

	return protocol;
}
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master

static inline void vlan_set_encap_proto(struct sk_buff *skb,
					struct vlan_hdr *vhdr)
{
	__be16 proto;
	unsigned short *rawp;

	/*
	 * Was a VLAN packet, grab the encapsulated protocol, which the layer
	 * three protocols care about.
	 */

	proto = vhdr->h_vlan_encapsulated_proto;
<<<<<<< HEAD
	if (ntohs(proto) >= 1536) {
=======
	if (ntohs(proto) >= ETH_P_802_3_MIN) {
>>>>>>> refs/remotes/origin/master
		skb->protocol = proto;
		return;
	}

	rawp = (unsigned short *)(vhdr + 1);
	if (*rawp == 0xFFFF)
		/*
		 * This is a magic hack to spot IPX packets. Older Novell
		 * breaks the protocol design and runs IPX over 802.3 without
		 * an 802.2 LLC layer. We look for FFFF which isn't a used
		 * 802.2 SSAP/DSAP. This won't work for fault tolerant netware
		 * but does for the rest.
		 */
		skb->protocol = htons(ETH_P_802_3);
	else
		/*
		 * Real 802.2 LLC
		 */
		skb->protocol = htons(ETH_P_802_2);
}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
#endif /* __KERNEL__ */

/* VLAN IOCTLs are found in sockios.h */

/* Passed in vlan_ioctl_args structure to determine behaviour. */
enum vlan_ioctl_cmds {
	ADD_VLAN_CMD,
	DEL_VLAN_CMD,
	SET_VLAN_INGRESS_PRIORITY_CMD,
	SET_VLAN_EGRESS_PRIORITY_CMD,
	GET_VLAN_INGRESS_PRIORITY_CMD,
	GET_VLAN_EGRESS_PRIORITY_CMD,
	SET_VLAN_NAME_TYPE_CMD,
	SET_VLAN_FLAG_CMD,
	GET_VLAN_REALDEV_NAME_CMD, /* If this works, you know it's a VLAN device, btw */
	GET_VLAN_VID_CMD /* Get the VID of this VLAN (specified by name) */
};

enum vlan_flags {
	VLAN_FLAG_REORDER_HDR	= 0x1,
	VLAN_FLAG_GVRP		= 0x2,
	VLAN_FLAG_LOOSE_BINDING	= 0x4,
};

enum vlan_name_types {
	VLAN_NAME_TYPE_PLUS_VID, /* Name will look like:  vlan0005 */
	VLAN_NAME_TYPE_RAW_PLUS_VID, /* name will look like:  eth1.0005 */
	VLAN_NAME_TYPE_PLUS_VID_NO_PAD, /* Name will look like:  vlan5 */
	VLAN_NAME_TYPE_RAW_PLUS_VID_NO_PAD, /* Name will look like:  eth0.5 */
	VLAN_NAME_TYPE_HIGHEST
};

struct vlan_ioctl_args {
	int cmd; /* Should be one of the vlan_ioctl_cmds enum above. */
	char device1[24];

        union {
		char device2[24];
		int VID;
		unsigned int skb_priority;
		unsigned int name_type;
		unsigned int bind_type;
<<<<<<< HEAD
		unsigned int flag; /* Matches vlan_dev_info flags */
=======
		unsigned int flag; /* Matches vlan_dev_priv flags */
>>>>>>> refs/remotes/origin/cm-10.0
        } u;

	short vlan_qos;   
};

=======
>>>>>>> refs/remotes/origin/master
#endif /* !(_LINUX_IF_VLAN_H_) */
