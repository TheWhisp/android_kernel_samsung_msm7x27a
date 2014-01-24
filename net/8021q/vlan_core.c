#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/if_vlan.h>
#include <linux/netpoll.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master
#include "vlan.h"

bool vlan_do_receive(struct sk_buff **skbp)
{
	struct sk_buff *skb = *skbp;
<<<<<<< HEAD
	u16 vlan_id = skb->vlan_tci & VLAN_VID_MASK;
	struct net_device *vlan_dev;
	struct vlan_pcpu_stats *rx_stats;

	vlan_dev = vlan_find_dev(skb->dev, vlan_id);
<<<<<<< HEAD
<<<<<<< HEAD
	if (!vlan_dev) {
		if (vlan_id)
			skb->pkt_type = PACKET_OTHERHOST;
		return false;
	}
=======
	if (!vlan_dev)
		return false;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	__be16 vlan_proto = skb->vlan_proto;
	u16 vlan_id = vlan_tx_tag_get_id(skb);
	struct net_device *vlan_dev;
	struct vlan_pcpu_stats *rx_stats;

	vlan_dev = vlan_find_dev(skb->dev, vlan_proto, vlan_id);
	if (!vlan_dev)
		return false;
>>>>>>> refs/remotes/origin/master
=======
	if (!vlan_dev)
		return false;
>>>>>>> refs/remotes/origin/cm-11.0

	skb = *skbp = skb_share_check(skb, GFP_ATOMIC);
	if (unlikely(!skb))
		return false;

	skb->dev = vlan_dev;
	if (skb->pkt_type == PACKET_OTHERHOST) {
		/* Our lower layer thinks this is not local, let's make sure.
		 * This allows the VLAN to have a different MAC than the
		 * underlying device, and still route correctly. */
<<<<<<< HEAD
		if (!compare_ether_addr(eth_hdr(skb)->h_dest,
					vlan_dev->dev_addr))
			skb->pkt_type = PACKET_HOST;
	}

<<<<<<< HEAD
	if (!(vlan_dev_info(vlan_dev)->flags & VLAN_FLAG_REORDER_HDR)) {
=======
	if (!(vlan_dev_priv(vlan_dev)->flags & VLAN_FLAG_REORDER_HDR)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (ether_addr_equal(eth_hdr(skb)->h_dest, vlan_dev->dev_addr))
			skb->pkt_type = PACKET_HOST;
	}

	if (!(vlan_dev_priv(vlan_dev)->flags & VLAN_FLAG_REORDER_HDR)) {
>>>>>>> refs/remotes/origin/master
		unsigned int offset = skb->data - skb_mac_header(skb);

		/*
		 * vlan_insert_tag expect skb->data pointing to mac header.
		 * So change skb->data before calling it and change back to
		 * original position later
		 */
		skb_push(skb, offset);
<<<<<<< HEAD
		skb = *skbp = vlan_insert_tag(skb, skb->vlan_tci);
=======
		skb = *skbp = vlan_insert_tag(skb, skb->vlan_proto,
					      skb->vlan_tci);
>>>>>>> refs/remotes/origin/master
		if (!skb)
			return false;
		skb_pull(skb, offset + VLAN_HLEN);
		skb_reset_mac_len(skb);
	}

	skb->priority = vlan_get_ingress_priority(vlan_dev, skb->vlan_tci);
	skb->vlan_tci = 0;

<<<<<<< HEAD
<<<<<<< HEAD
	rx_stats = this_cpu_ptr(vlan_dev_info(vlan_dev)->vlan_pcpu_stats);
=======
	rx_stats = this_cpu_ptr(vlan_dev_priv(vlan_dev)->vlan_pcpu_stats);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	rx_stats = this_cpu_ptr(vlan_dev_priv(vlan_dev)->vlan_pcpu_stats);
>>>>>>> refs/remotes/origin/master

	u64_stats_update_begin(&rx_stats->syncp);
	rx_stats->rx_packets++;
	rx_stats->rx_bytes += skb->len;
	if (skb->pkt_type == PACKET_MULTICAST)
		rx_stats->rx_multicast++;
	u64_stats_update_end(&rx_stats->syncp);

	return true;
}

<<<<<<< HEAD
<<<<<<< HEAD
struct net_device *vlan_dev_real_dev(const struct net_device *dev)
{
	return vlan_dev_info(dev)->real_dev;
}
EXPORT_SYMBOL(vlan_dev_real_dev);

u16 vlan_dev_vlan_id(const struct net_device *dev)
{
	return vlan_dev_info(dev)->vlan_id;
}
EXPORT_SYMBOL(vlan_dev_vlan_id);

/* VLAN rx hw acceleration helper.  This acts like netif_{rx,receive_skb}(). */
int __vlan_hwaccel_rx(struct sk_buff *skb, struct vlan_group *grp,
		      u16 vlan_tci, int polling)
{
	__vlan_hwaccel_put_tag(skb, vlan_tci);
	return polling ? netif_receive_skb(skb) : netif_rx(skb);
}
EXPORT_SYMBOL(__vlan_hwaccel_rx);

gro_result_t vlan_gro_receive(struct napi_struct *napi, struct vlan_group *grp,
			      unsigned int vlan_tci, struct sk_buff *skb)
{
	__vlan_hwaccel_put_tag(skb, vlan_tci);
	return napi_gro_receive(napi, skb);
}
EXPORT_SYMBOL(vlan_gro_receive);

gro_result_t vlan_gro_frags(struct napi_struct *napi, struct vlan_group *grp,
			    unsigned int vlan_tci)
{
	__vlan_hwaccel_put_tag(napi->skb, vlan_tci);
	return napi_gro_frags(napi);
}
EXPORT_SYMBOL(vlan_gro_frags);
=======
/* Must be invoked with rcu_read_lock or with RTNL. */
struct net_device *__vlan_find_dev_deep(struct net_device *real_dev,
					u16 vlan_id)
{
	struct vlan_info *vlan_info = rcu_dereference_rtnl(real_dev->vlan_info);

	if (vlan_info) {
		return vlan_group_get_device(&vlan_info->grp, vlan_id);
	} else {
		/*
		 * Bonding slaves do not have grp assigned to themselves.
		 * Grp is assigned to bonding master instead.
		 */
		if (netif_is_bond_slave(real_dev))
			return __vlan_find_dev_deep(real_dev->master, vlan_id);
=======
/* Must be invoked with rcu_read_lock. */
struct net_device *__vlan_find_dev_deep(struct net_device *dev,
					__be16 vlan_proto, u16 vlan_id)
{
	struct vlan_info *vlan_info = rcu_dereference(dev->vlan_info);

	if (vlan_info) {
		return vlan_group_get_device(&vlan_info->grp,
					     vlan_proto, vlan_id);
	} else {
		/*
		 * Lower devices of master uppers (bonding, team) do not have
		 * grp assigned to themselves. Grp is assigned to upper device
		 * instead.
		 */
		struct net_device *upper_dev;

		upper_dev = netdev_master_upper_dev_get_rcu(dev);
		if (upper_dev)
			return __vlan_find_dev_deep(upper_dev,
						    vlan_proto, vlan_id);
>>>>>>> refs/remotes/origin/master
	}

	return NULL;
}
EXPORT_SYMBOL(__vlan_find_dev_deep);

struct net_device *vlan_dev_real_dev(const struct net_device *dev)
{
<<<<<<< HEAD
	return vlan_dev_priv(dev)->real_dev;
=======
	struct net_device *ret = vlan_dev_priv(dev)->real_dev;

	while (is_vlan_dev(ret))
		ret = vlan_dev_priv(ret)->real_dev;

	return ret;
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL(vlan_dev_real_dev);

u16 vlan_dev_vlan_id(const struct net_device *dev)
{
	return vlan_dev_priv(dev)->vlan_id;
}
EXPORT_SYMBOL(vlan_dev_vlan_id);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

static struct sk_buff *vlan_reorder_header(struct sk_buff *skb)
{
	if (skb_cow(skb, skb_headroom(skb)) < 0)
		return NULL;
	memmove(skb->data - ETH_HLEN, skb->data - VLAN_ETH_HLEN, 2 * ETH_ALEN);
	skb->mac_header += VLAN_HLEN;
	return skb;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void vlan_set_encap_proto(struct sk_buff *skb, struct vlan_hdr *vhdr)
{
	__be16 proto;
	unsigned char *rawp;

	/*
	 * Was a VLAN packet, grab the encapsulated protocol, which the layer
	 * three protocols care about.
	 */

	proto = vhdr->h_vlan_encapsulated_proto;
	if (ntohs(proto) >= 1536) {
		skb->protocol = proto;
		return;
	}

	rawp = skb->data;
	if (*(unsigned short *) rawp == 0xFFFF)
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

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
struct sk_buff *vlan_untag(struct sk_buff *skb)
{
	struct vlan_hdr *vhdr;
	u16 vlan_tci;

	if (unlikely(vlan_tx_tag_present(skb))) {
		/* vlan_tci is already set-up so leave this for another time */
		return skb;
	}

	skb = skb_share_check(skb, GFP_ATOMIC);
	if (unlikely(!skb))
		goto err_free;

	if (unlikely(!pskb_may_pull(skb, VLAN_HLEN)))
		goto err_free;

	vhdr = (struct vlan_hdr *) skb->data;
	vlan_tci = ntohs(vhdr->h_vlan_TCI);
<<<<<<< HEAD
	__vlan_hwaccel_put_tag(skb, vlan_tci);
=======
	__vlan_hwaccel_put_tag(skb, skb->protocol, vlan_tci);
>>>>>>> refs/remotes/origin/master

	skb_pull_rcsum(skb, VLAN_HLEN);
	vlan_set_encap_proto(skb, vhdr);

	skb = vlan_reorder_header(skb);
	if (unlikely(!skb))
		goto err_free;

	skb_reset_network_header(skb);
	skb_reset_transport_header(skb);
	skb_reset_mac_len(skb);

	return skb;

err_free:
	kfree_skb(skb);
	return NULL;
}
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
EXPORT_SYMBOL(vlan_untag);
>>>>>>> refs/remotes/origin/master


/*
 * vlan info and vid list
 */

static void vlan_group_free(struct vlan_group *grp)
{
<<<<<<< HEAD
	int i;

	for (i = 0; i < VLAN_GROUP_ARRAY_SPLIT_PARTS; i++)
		kfree(grp->vlan_devices_arrays[i]);
=======
	int i, j;

	for (i = 0; i < VLAN_PROTO_NUM; i++)
		for (j = 0; j < VLAN_GROUP_ARRAY_SPLIT_PARTS; j++)
			kfree(grp->vlan_devices_arrays[i][j]);
>>>>>>> refs/remotes/origin/master
}

static void vlan_info_free(struct vlan_info *vlan_info)
{
	vlan_group_free(&vlan_info->grp);
	kfree(vlan_info);
}

static void vlan_info_rcu_free(struct rcu_head *rcu)
{
	vlan_info_free(container_of(rcu, struct vlan_info, rcu));
}

static struct vlan_info *vlan_info_alloc(struct net_device *dev)
{
	struct vlan_info *vlan_info;

	vlan_info = kzalloc(sizeof(struct vlan_info), GFP_KERNEL);
	if (!vlan_info)
		return NULL;

	vlan_info->real_dev = dev;
	INIT_LIST_HEAD(&vlan_info->vid_list);
	return vlan_info;
}

struct vlan_vid_info {
	struct list_head list;
<<<<<<< HEAD
	unsigned short vid;
	int refcount;
};

static struct vlan_vid_info *vlan_vid_info_get(struct vlan_info *vlan_info,
					       unsigned short vid)
=======
	__be16 proto;
	u16 vid;
	int refcount;
};

static bool vlan_hw_filter_capable(const struct net_device *dev,
				     const struct vlan_vid_info *vid_info)
{
	if (vid_info->proto == htons(ETH_P_8021Q) &&
	    dev->features & NETIF_F_HW_VLAN_CTAG_FILTER)
		return true;
	if (vid_info->proto == htons(ETH_P_8021AD) &&
	    dev->features & NETIF_F_HW_VLAN_STAG_FILTER)
		return true;
	return false;
}

static struct vlan_vid_info *vlan_vid_info_get(struct vlan_info *vlan_info,
					       __be16 proto, u16 vid)
>>>>>>> refs/remotes/origin/master
{
	struct vlan_vid_info *vid_info;

	list_for_each_entry(vid_info, &vlan_info->vid_list, list) {
<<<<<<< HEAD
		if (vid_info->vid == vid)
=======
		if (vid_info->proto == proto && vid_info->vid == vid)
>>>>>>> refs/remotes/origin/master
			return vid_info;
	}
	return NULL;
}

<<<<<<< HEAD
static struct vlan_vid_info *vlan_vid_info_alloc(unsigned short vid)
=======
static struct vlan_vid_info *vlan_vid_info_alloc(__be16 proto, u16 vid)
>>>>>>> refs/remotes/origin/master
{
	struct vlan_vid_info *vid_info;

	vid_info = kzalloc(sizeof(struct vlan_vid_info), GFP_KERNEL);
	if (!vid_info)
		return NULL;
<<<<<<< HEAD
=======
	vid_info->proto = proto;
>>>>>>> refs/remotes/origin/master
	vid_info->vid = vid;

	return vid_info;
}

<<<<<<< HEAD
static int __vlan_vid_add(struct vlan_info *vlan_info, unsigned short vid,
=======
static int __vlan_vid_add(struct vlan_info *vlan_info, __be16 proto, u16 vid,
>>>>>>> refs/remotes/origin/master
			  struct vlan_vid_info **pvid_info)
{
	struct net_device *dev = vlan_info->real_dev;
	const struct net_device_ops *ops = dev->netdev_ops;
	struct vlan_vid_info *vid_info;
	int err;

<<<<<<< HEAD
	vid_info = vlan_vid_info_alloc(vid);
	if (!vid_info)
		return -ENOMEM;

	if ((dev->features & NETIF_F_HW_VLAN_FILTER) &&
	    ops->ndo_vlan_rx_add_vid) {
		err =  ops->ndo_vlan_rx_add_vid(dev, vid);
=======
	vid_info = vlan_vid_info_alloc(proto, vid);
	if (!vid_info)
		return -ENOMEM;

	if (vlan_hw_filter_capable(dev, vid_info)) {
		err =  ops->ndo_vlan_rx_add_vid(dev, proto, vid);
>>>>>>> refs/remotes/origin/master
		if (err) {
			kfree(vid_info);
			return err;
		}
	}
	list_add(&vid_info->list, &vlan_info->vid_list);
	vlan_info->nr_vids++;
	*pvid_info = vid_info;
	return 0;
}

<<<<<<< HEAD
int vlan_vid_add(struct net_device *dev, unsigned short vid)
=======
int vlan_vid_add(struct net_device *dev, __be16 proto, u16 vid)
>>>>>>> refs/remotes/origin/master
{
	struct vlan_info *vlan_info;
	struct vlan_vid_info *vid_info;
	bool vlan_info_created = false;
	int err;

	ASSERT_RTNL();

	vlan_info = rtnl_dereference(dev->vlan_info);
	if (!vlan_info) {
		vlan_info = vlan_info_alloc(dev);
		if (!vlan_info)
			return -ENOMEM;
		vlan_info_created = true;
	}
<<<<<<< HEAD
	vid_info = vlan_vid_info_get(vlan_info, vid);
	if (!vid_info) {
		err = __vlan_vid_add(vlan_info, vid, &vid_info);
=======
	vid_info = vlan_vid_info_get(vlan_info, proto, vid);
	if (!vid_info) {
		err = __vlan_vid_add(vlan_info, proto, vid, &vid_info);
>>>>>>> refs/remotes/origin/master
		if (err)
			goto out_free_vlan_info;
	}
	vid_info->refcount++;

	if (vlan_info_created)
		rcu_assign_pointer(dev->vlan_info, vlan_info);

	return 0;

out_free_vlan_info:
	if (vlan_info_created)
		kfree(vlan_info);
	return err;
}
EXPORT_SYMBOL(vlan_vid_add);

static void __vlan_vid_del(struct vlan_info *vlan_info,
			   struct vlan_vid_info *vid_info)
{
	struct net_device *dev = vlan_info->real_dev;
	const struct net_device_ops *ops = dev->netdev_ops;
<<<<<<< HEAD
	unsigned short vid = vid_info->vid;
	int err;

	if ((dev->features & NETIF_F_HW_VLAN_FILTER) &&
	     ops->ndo_vlan_rx_kill_vid) {
		err = ops->ndo_vlan_rx_kill_vid(dev, vid);
		if (err) {
			pr_warn("failed to kill vid %d for device %s\n",
				vid, dev->name);
=======
	__be16 proto = vid_info->proto;
	u16 vid = vid_info->vid;
	int err;

	if (vlan_hw_filter_capable(dev, vid_info)) {
		err = ops->ndo_vlan_rx_kill_vid(dev, proto, vid);
		if (err) {
			pr_warn("failed to kill vid %04x/%d for device %s\n",
				proto, vid, dev->name);
>>>>>>> refs/remotes/origin/master
		}
	}
	list_del(&vid_info->list);
	kfree(vid_info);
	vlan_info->nr_vids--;
}

<<<<<<< HEAD
void vlan_vid_del(struct net_device *dev, unsigned short vid)
=======
void vlan_vid_del(struct net_device *dev, __be16 proto, u16 vid)
>>>>>>> refs/remotes/origin/master
{
	struct vlan_info *vlan_info;
	struct vlan_vid_info *vid_info;

	ASSERT_RTNL();

	vlan_info = rtnl_dereference(dev->vlan_info);
	if (!vlan_info)
		return;

<<<<<<< HEAD
	vid_info = vlan_vid_info_get(vlan_info, vid);
=======
	vid_info = vlan_vid_info_get(vlan_info, proto, vid);
>>>>>>> refs/remotes/origin/master
	if (!vid_info)
		return;
	vid_info->refcount--;
	if (vid_info->refcount == 0) {
		__vlan_vid_del(vlan_info, vid_info);
		if (vlan_info->nr_vids == 0) {
			RCU_INIT_POINTER(dev->vlan_info, NULL);
			call_rcu(&vlan_info->rcu, vlan_info_rcu_free);
		}
	}
}
EXPORT_SYMBOL(vlan_vid_del);

int vlan_vids_add_by_dev(struct net_device *dev,
			 const struct net_device *by_dev)
{
	struct vlan_vid_info *vid_info;
	struct vlan_info *vlan_info;
	int err;

	ASSERT_RTNL();

	vlan_info = rtnl_dereference(by_dev->vlan_info);
	if (!vlan_info)
		return 0;

	list_for_each_entry(vid_info, &vlan_info->vid_list, list) {
<<<<<<< HEAD
		err = vlan_vid_add(dev, vid_info->vid);
=======
		err = vlan_vid_add(dev, vid_info->proto, vid_info->vid);
>>>>>>> refs/remotes/origin/master
		if (err)
			goto unwind;
	}
	return 0;

unwind:
	list_for_each_entry_continue_reverse(vid_info,
					     &vlan_info->vid_list,
					     list) {
<<<<<<< HEAD
		vlan_vid_del(dev, vid_info->vid);
=======
		vlan_vid_del(dev, vid_info->proto, vid_info->vid);
>>>>>>> refs/remotes/origin/master
	}

	return err;
}
EXPORT_SYMBOL(vlan_vids_add_by_dev);

void vlan_vids_del_by_dev(struct net_device *dev,
			  const struct net_device *by_dev)
{
	struct vlan_vid_info *vid_info;
	struct vlan_info *vlan_info;

	ASSERT_RTNL();

	vlan_info = rtnl_dereference(by_dev->vlan_info);
	if (!vlan_info)
		return;

	list_for_each_entry(vid_info, &vlan_info->vid_list, list)
<<<<<<< HEAD
		vlan_vid_del(dev, vid_info->vid);
}
EXPORT_SYMBOL(vlan_vids_del_by_dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		vlan_vid_del(dev, vid_info->proto, vid_info->vid);
}
EXPORT_SYMBOL(vlan_vids_del_by_dev);

bool vlan_uses_dev(const struct net_device *dev)
{
	struct vlan_info *vlan_info;

	ASSERT_RTNL();

	vlan_info = rtnl_dereference(dev->vlan_info);
	if (!vlan_info)
		return false;
	return vlan_info->grp.nr_vlan_devs ? true : false;
}
EXPORT_SYMBOL(vlan_uses_dev);
>>>>>>> refs/remotes/origin/master
