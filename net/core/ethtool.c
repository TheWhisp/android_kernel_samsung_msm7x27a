/*
 * net/core/ethtool.c - Ethtool ioctl handler
 * Copyright (c) 2003 Matthew Wilcox <matthew@wil.cx>
 *
 * This file is where we call all the ethtool_ops commands to get
 * the information ethtool needs.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/capability.h>
#include <linux/errno.h>
#include <linux/ethtool.h>
#include <linux/netdevice.h>
<<<<<<< HEAD
=======
#include <linux/net_tstamp.h>
#include <linux/phy.h>
>>>>>>> refs/remotes/origin/master
#include <linux/bitops.h>
#include <linux/uaccess.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/rtnetlink.h>
#include <linux/sched.h>

/*
 * Some useful ethtool_ops methods that're device independent.
 * If we find that all drivers want to do the same thing here,
 * we can turn these into dev_() function calls.
 */

u32 ethtool_op_get_link(struct net_device *dev)
{
	return netif_carrier_ok(dev) ? 1 : 0;
}
EXPORT_SYMBOL(ethtool_op_get_link);

<<<<<<< HEAD
<<<<<<< HEAD
u32 ethtool_op_get_tx_csum(struct net_device *dev)
{
	return (dev->features & NETIF_F_ALL_CSUM) != 0;
}
EXPORT_SYMBOL(ethtool_op_get_tx_csum);

int ethtool_op_set_tx_csum(struct net_device *dev, u32 data)
{
	if (data)
		dev->features |= NETIF_F_IP_CSUM;
	else
		dev->features &= ~NETIF_F_IP_CSUM;

	return 0;
}
EXPORT_SYMBOL(ethtool_op_set_tx_csum);

int ethtool_op_set_tx_hw_csum(struct net_device *dev, u32 data)
{
	if (data)
		dev->features |= NETIF_F_HW_CSUM;
	else
		dev->features &= ~NETIF_F_HW_CSUM;

	return 0;
}
EXPORT_SYMBOL(ethtool_op_set_tx_hw_csum);

int ethtool_op_set_tx_ipv6_csum(struct net_device *dev, u32 data)
{
	if (data)
		dev->features |= NETIF_F_IP_CSUM | NETIF_F_IPV6_CSUM;
	else
		dev->features &= ~(NETIF_F_IP_CSUM | NETIF_F_IPV6_CSUM);

	return 0;
}
EXPORT_SYMBOL(ethtool_op_set_tx_ipv6_csum);

u32 ethtool_op_get_sg(struct net_device *dev)
{
	return (dev->features & NETIF_F_SG) != 0;
}
EXPORT_SYMBOL(ethtool_op_get_sg);

int ethtool_op_set_sg(struct net_device *dev, u32 data)
{
	if (data)
		dev->features |= NETIF_F_SG;
	else
		dev->features &= ~NETIF_F_SG;

	return 0;
}
EXPORT_SYMBOL(ethtool_op_set_sg);

u32 ethtool_op_get_tso(struct net_device *dev)
{
	return (dev->features & NETIF_F_TSO) != 0;
}
EXPORT_SYMBOL(ethtool_op_get_tso);

int ethtool_op_set_tso(struct net_device *dev, u32 data)
{
	if (data)
		dev->features |= NETIF_F_TSO;
	else
		dev->features &= ~NETIF_F_TSO;

	return 0;
}
EXPORT_SYMBOL(ethtool_op_set_tso);

u32 ethtool_op_get_ufo(struct net_device *dev)
{
	return (dev->features & NETIF_F_UFO) != 0;
}
EXPORT_SYMBOL(ethtool_op_get_ufo);

int ethtool_op_set_ufo(struct net_device *dev, u32 data)
{
	if (data)
		dev->features |= NETIF_F_UFO;
	else
		dev->features &= ~NETIF_F_UFO;
	return 0;
}
EXPORT_SYMBOL(ethtool_op_set_ufo);

/* the following list of flags are the same as their associated
 * NETIF_F_xxx values in include/linux/netdevice.h
 */
static const u32 flags_dup_features =
	(ETH_FLAG_LRO | ETH_FLAG_RXVLAN | ETH_FLAG_TXVLAN | ETH_FLAG_NTUPLE |
	 ETH_FLAG_RXHASH);

u32 ethtool_op_get_flags(struct net_device *dev)
{
	/* in the future, this function will probably contain additional
	 * handling for flags which are not so easily handled
	 * by a simple masking operation
	 */

	return dev->features & flags_dup_features;
}
EXPORT_SYMBOL(ethtool_op_get_flags);

/* Check if device can enable (or disable) particular feature coded in "data"
 * argument. Flags "supported" describe features that can be toggled by device.
 * If feature can not be toggled, it state (enabled or disabled) must match
 * hardcoded device features state, otherwise flags are marked as invalid.
 */
bool ethtool_invalid_flags(struct net_device *dev, u32 data, u32 supported)
{
	u32 features = dev->features & flags_dup_features;
	/* "data" can contain only flags_dup_features bits,
	 * see __ethtool_set_flags */

	return (features & ~supported) != (data & ~supported);
}
EXPORT_SYMBOL(ethtool_invalid_flags);

int ethtool_op_set_flags(struct net_device *dev, u32 data, u32 supported)
{
	if (ethtool_invalid_flags(dev, data, supported))
		return -EINVAL;

	dev->features = ((dev->features & ~flags_dup_features) |
			 (data & flags_dup_features));
	return 0;
}
EXPORT_SYMBOL(ethtool_op_set_flags);

void ethtool_ntuple_flush(struct net_device *dev)
{
	struct ethtool_rx_ntuple_flow_spec_container *fsc, *f;

	list_for_each_entry_safe(fsc, f, &dev->ethtool_ntuple_list.list, list) {
		list_del(&fsc->list);
		kfree(fsc);
	}
	dev->ethtool_ntuple_list.count = 0;
}
EXPORT_SYMBOL(ethtool_ntuple_flush);

/* Handlers for each ethtool command */

#define ETHTOOL_DEV_FEATURE_WORDS	1

static void ethtool_get_features_compat(struct net_device *dev,
	struct ethtool_get_features_block *features)
{
	if (!dev->ethtool_ops)
		return;

	/* getting RX checksum */
	if (dev->ethtool_ops->get_rx_csum)
		if (dev->ethtool_ops->get_rx_csum(dev))
			features[0].active |= NETIF_F_RXCSUM;

	/* mark legacy-changeable features */
	if (dev->ethtool_ops->set_sg)
		features[0].available |= NETIF_F_SG;
	if (dev->ethtool_ops->set_tx_csum)
		features[0].available |= NETIF_F_ALL_CSUM;
	if (dev->ethtool_ops->set_tso)
		features[0].available |= NETIF_F_ALL_TSO;
	if (dev->ethtool_ops->set_rx_csum)
		features[0].available |= NETIF_F_RXCSUM;
	if (dev->ethtool_ops->set_flags)
		features[0].available |= flags_dup_features;
}

static int ethtool_set_feature_compat(struct net_device *dev,
	int (*legacy_set)(struct net_device *, u32),
	struct ethtool_set_features_block *features, u32 mask)
{
	u32 do_set;

	if (!legacy_set)
		return 0;

	if (!(features[0].valid & mask))
		return 0;

	features[0].valid &= ~mask;

	do_set = !!(features[0].requested & mask);

	if (legacy_set(dev, do_set) < 0)
		netdev_info(dev,
			"Legacy feature change (%s) failed for 0x%08x\n",
			do_set ? "set" : "clear", mask);

	return 1;
}

static int ethtool_set_flags_compat(struct net_device *dev,
	int (*legacy_set)(struct net_device *, u32),
	struct ethtool_set_features_block *features, u32 mask)
{
	u32 value;

	if (!legacy_set)
		return 0;

	if (!(features[0].valid & mask))
		return 0;

	value = dev->features & ~features[0].valid;
	value |= features[0].requested;

	features[0].valid &= ~mask;

	if (legacy_set(dev, value & mask) < 0)
		netdev_info(dev, "Legacy flags change failed\n");

	return 1;
}

static int ethtool_set_features_compat(struct net_device *dev,
	struct ethtool_set_features_block *features)
{
	int compat;

	if (!dev->ethtool_ops)
		return 0;

	compat  = ethtool_set_feature_compat(dev, dev->ethtool_ops->set_sg,
		features, NETIF_F_SG);
	compat |= ethtool_set_feature_compat(dev, dev->ethtool_ops->set_tx_csum,
		features, NETIF_F_ALL_CSUM);
	compat |= ethtool_set_feature_compat(dev, dev->ethtool_ops->set_tso,
		features, NETIF_F_ALL_TSO);
	compat |= ethtool_set_feature_compat(dev, dev->ethtool_ops->set_rx_csum,
		features, NETIF_F_RXCSUM);
	compat |= ethtool_set_flags_compat(dev, dev->ethtool_ops->set_flags,
		features, flags_dup_features);

	return compat;
}
=======
=======
int ethtool_op_get_ts_info(struct net_device *dev, struct ethtool_ts_info *info)
{
	info->so_timestamping =
		SOF_TIMESTAMPING_TX_SOFTWARE |
		SOF_TIMESTAMPING_RX_SOFTWARE |
		SOF_TIMESTAMPING_SOFTWARE;
	info->phc_index = -1;
	return 0;
}
EXPORT_SYMBOL(ethtool_op_get_ts_info);

>>>>>>> refs/remotes/origin/master
/* Handlers for each ethtool command */

#define ETHTOOL_DEV_FEATURE_WORDS	((NETDEV_FEATURE_COUNT + 31) / 32)

static const char netdev_features_strings[NETDEV_FEATURE_COUNT][ETH_GSTRING_LEN] = {
	[NETIF_F_SG_BIT] =               "tx-scatter-gather",
	[NETIF_F_IP_CSUM_BIT] =          "tx-checksum-ipv4",
	[NETIF_F_HW_CSUM_BIT] =          "tx-checksum-ip-generic",
	[NETIF_F_IPV6_CSUM_BIT] =        "tx-checksum-ipv6",
	[NETIF_F_HIGHDMA_BIT] =          "highdma",
	[NETIF_F_FRAGLIST_BIT] =         "tx-scatter-gather-fraglist",
<<<<<<< HEAD
	[NETIF_F_HW_VLAN_TX_BIT] =       "tx-vlan-hw-insert",

	[NETIF_F_HW_VLAN_RX_BIT] =       "rx-vlan-hw-parse",
	[NETIF_F_HW_VLAN_FILTER_BIT] =   "rx-vlan-filter",
=======
	[NETIF_F_HW_VLAN_CTAG_TX_BIT] =  "tx-vlan-hw-insert",

	[NETIF_F_HW_VLAN_CTAG_RX_BIT] =  "rx-vlan-hw-parse",
	[NETIF_F_HW_VLAN_CTAG_FILTER_BIT] = "rx-vlan-filter",
	[NETIF_F_HW_VLAN_STAG_TX_BIT] =  "tx-vlan-stag-hw-insert",
	[NETIF_F_HW_VLAN_STAG_RX_BIT] =  "rx-vlan-stag-hw-parse",
	[NETIF_F_HW_VLAN_STAG_FILTER_BIT] = "rx-vlan-stag-filter",
>>>>>>> refs/remotes/origin/master
	[NETIF_F_VLAN_CHALLENGED_BIT] =  "vlan-challenged",
	[NETIF_F_GSO_BIT] =              "tx-generic-segmentation",
	[NETIF_F_LLTX_BIT] =             "tx-lockless",
	[NETIF_F_NETNS_LOCAL_BIT] =      "netns-local",
	[NETIF_F_GRO_BIT] =              "rx-gro",
	[NETIF_F_LRO_BIT] =              "rx-lro",

	[NETIF_F_TSO_BIT] =              "tx-tcp-segmentation",
	[NETIF_F_UFO_BIT] =              "tx-udp-fragmentation",
	[NETIF_F_GSO_ROBUST_BIT] =       "tx-gso-robust",
	[NETIF_F_TSO_ECN_BIT] =          "tx-tcp-ecn-segmentation",
	[NETIF_F_TSO6_BIT] =             "tx-tcp6-segmentation",
	[NETIF_F_FSO_BIT] =              "tx-fcoe-segmentation",
<<<<<<< HEAD
=======
	[NETIF_F_GSO_GRE_BIT] =		 "tx-gre-segmentation",
	[NETIF_F_GSO_IPIP_BIT] =	 "tx-ipip-segmentation",
	[NETIF_F_GSO_SIT_BIT] =		 "tx-sit-segmentation",
	[NETIF_F_GSO_UDP_TUNNEL_BIT] =	 "tx-udp_tnl-segmentation",
	[NETIF_F_GSO_MPLS_BIT] =	 "tx-mpls-segmentation",
>>>>>>> refs/remotes/origin/master

	[NETIF_F_FCOE_CRC_BIT] =         "tx-checksum-fcoe-crc",
	[NETIF_F_SCTP_CSUM_BIT] =        "tx-checksum-sctp",
	[NETIF_F_FCOE_MTU_BIT] =         "fcoe-mtu",
	[NETIF_F_NTUPLE_BIT] =           "rx-ntuple-filter",
	[NETIF_F_RXHASH_BIT] =           "rx-hashing",
	[NETIF_F_RXCSUM_BIT] =           "rx-checksum",
	[NETIF_F_NOCACHE_COPY_BIT] =     "tx-nocache-copy",
	[NETIF_F_LOOPBACK_BIT] =         "loopback",
	[NETIF_F_RXFCS_BIT] =            "rx-fcs",
	[NETIF_F_RXALL_BIT] =            "rx-all",
<<<<<<< HEAD
};
>>>>>>> refs/remotes/origin/cm-10.0
=======
	[NETIF_F_HW_L2FW_DOFFLOAD_BIT] = "l2-fwd-offload",
};
>>>>>>> refs/remotes/origin/master

static int ethtool_get_features(struct net_device *dev, void __user *useraddr)
{
	struct ethtool_gfeatures cmd = {
		.cmd = ETHTOOL_GFEATURES,
		.size = ETHTOOL_DEV_FEATURE_WORDS,
	};
<<<<<<< HEAD
<<<<<<< HEAD
	struct ethtool_get_features_block features[ETHTOOL_DEV_FEATURE_WORDS] = {
		{
			.available = dev->hw_features,
			.requested = dev->wanted_features,
			.active = dev->features,
			.never_changed = NETIF_F_NEVER_CHANGE,
		},
	};
	u32 __user *sizeaddr;
	u32 copy_size;

	ethtool_get_features_compat(dev, features);
=======
=======
>>>>>>> refs/remotes/origin/master
	struct ethtool_get_features_block features[ETHTOOL_DEV_FEATURE_WORDS];
	u32 __user *sizeaddr;
	u32 copy_size;
	int i;

	/* in case feature bits run out again */
	BUILD_BUG_ON(ETHTOOL_DEV_FEATURE_WORDS * sizeof(u32) > sizeof(netdev_features_t));

	for (i = 0; i < ETHTOOL_DEV_FEATURE_WORDS; ++i) {
		features[i].available = (u32)(dev->hw_features >> (32 * i));
		features[i].requested = (u32)(dev->wanted_features >> (32 * i));
		features[i].active = (u32)(dev->features >> (32 * i));
		features[i].never_changed =
			(u32)(NETIF_F_NEVER_CHANGE >> (32 * i));
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	sizeaddr = useraddr + offsetof(struct ethtool_gfeatures, size);
	if (get_user(copy_size, sizeaddr))
		return -EFAULT;

	if (copy_size > ETHTOOL_DEV_FEATURE_WORDS)
		copy_size = ETHTOOL_DEV_FEATURE_WORDS;

	if (copy_to_user(useraddr, &cmd, sizeof(cmd)))
		return -EFAULT;
	useraddr += sizeof(cmd);
	if (copy_to_user(useraddr, features, copy_size * sizeof(*features)))
		return -EFAULT;

	return 0;
}

static int ethtool_set_features(struct net_device *dev, void __user *useraddr)
{
	struct ethtool_sfeatures cmd;
	struct ethtool_set_features_block features[ETHTOOL_DEV_FEATURE_WORDS];
<<<<<<< HEAD
<<<<<<< HEAD
	int ret = 0;
=======
	netdev_features_t wanted = 0, valid = 0;
	int i, ret = 0;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	netdev_features_t wanted = 0, valid = 0;
	int i, ret = 0;
>>>>>>> refs/remotes/origin/master

	if (copy_from_user(&cmd, useraddr, sizeof(cmd)))
		return -EFAULT;
	useraddr += sizeof(cmd);

	if (cmd.size != ETHTOOL_DEV_FEATURE_WORDS)
		return -EINVAL;

	if (copy_from_user(features, useraddr, sizeof(features)))
		return -EFAULT;

<<<<<<< HEAD
<<<<<<< HEAD
	if (features[0].valid & ~NETIF_F_ETHTOOL_BITS)
		return -EINVAL;

	if (ethtool_set_features_compat(dev, features))
		ret |= ETHTOOL_F_COMPAT;

	if (features[0].valid & ~dev->hw_features) {
		features[0].valid &= dev->hw_features;
		ret |= ETHTOOL_F_UNSUPPORTED;
	}

	dev->wanted_features &= ~features[0].valid;
	dev->wanted_features |= features[0].valid & features[0].requested;
	__netdev_update_features(dev);

	if ((dev->wanted_features ^ dev->features) & features[0].valid)
=======
=======
>>>>>>> refs/remotes/origin/master
	for (i = 0; i < ETHTOOL_DEV_FEATURE_WORDS; ++i) {
		valid |= (netdev_features_t)features[i].valid << (32 * i);
		wanted |= (netdev_features_t)features[i].requested << (32 * i);
	}

	if (valid & ~NETIF_F_ETHTOOL_BITS)
		return -EINVAL;

	if (valid & ~dev->hw_features) {
		valid &= dev->hw_features;
		ret |= ETHTOOL_F_UNSUPPORTED;
	}

	dev->wanted_features &= ~valid;
	dev->wanted_features |= wanted & valid;
	__netdev_update_features(dev);

	if ((dev->wanted_features ^ dev->features) & valid)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		ret |= ETHTOOL_F_WISH;

	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
static const char netdev_features_strings[ETHTOOL_DEV_FEATURE_WORDS * 32][ETH_GSTRING_LEN] = {
	/* NETIF_F_SG */              "tx-scatter-gather",
	/* NETIF_F_IP_CSUM */         "tx-checksum-ipv4",
	/* NETIF_F_NO_CSUM */         "tx-checksum-unneeded",
	/* NETIF_F_HW_CSUM */         "tx-checksum-ip-generic",
	/* NETIF_F_IPV6_CSUM */       "tx-checksum-ipv6",
	/* NETIF_F_HIGHDMA */         "highdma",
	/* NETIF_F_FRAGLIST */        "tx-scatter-gather-fraglist",
	/* NETIF_F_HW_VLAN_TX */      "tx-vlan-hw-insert",

	/* NETIF_F_HW_VLAN_RX */      "rx-vlan-hw-parse",
	/* NETIF_F_HW_VLAN_FILTER */  "rx-vlan-filter",
	/* NETIF_F_VLAN_CHALLENGED */ "vlan-challenged",
	/* NETIF_F_GSO */             "tx-generic-segmentation",
	/* NETIF_F_LLTX */            "tx-lockless",
	/* NETIF_F_NETNS_LOCAL */     "netns-local",
	/* NETIF_F_GRO */             "rx-gro",
	/* NETIF_F_LRO */             "rx-lro",

	/* NETIF_F_TSO */             "tx-tcp-segmentation",
	/* NETIF_F_UFO */             "tx-udp-fragmentation",
	/* NETIF_F_GSO_ROBUST */      "tx-gso-robust",
	/* NETIF_F_TSO_ECN */         "tx-tcp-ecn-segmentation",
	/* NETIF_F_TSO6 */            "tx-tcp6-segmentation",
	/* NETIF_F_FSO */             "tx-fcoe-segmentation",
	"",
	"",

	/* NETIF_F_FCOE_CRC */        "tx-checksum-fcoe-crc",
	/* NETIF_F_SCTP_CSUM */       "tx-checksum-sctp",
	/* NETIF_F_FCOE_MTU */        "fcoe-mtu",
	/* NETIF_F_NTUPLE */          "rx-ntuple-filter",
	/* NETIF_F_RXHASH */          "rx-hashing",
	/* NETIF_F_RXCSUM */          "rx-checksum",
	/* NETIF_F_NOCACHE_COPY */    "tx-nocache-copy",
	/* NETIF_F_LOOPBACK */        "loopback",
};

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static int __ethtool_get_sset_count(struct net_device *dev, int sset)
{
	const struct ethtool_ops *ops = dev->ethtool_ops;

	if (sset == ETH_SS_FEATURES)
		return ARRAY_SIZE(netdev_features_strings);

<<<<<<< HEAD
	if (ops && ops->get_sset_count && ops->get_strings)
=======
	if (ops->get_sset_count && ops->get_strings)
>>>>>>> refs/remotes/origin/master
		return ops->get_sset_count(dev, sset);
	else
		return -EOPNOTSUPP;
}

static void __ethtool_get_strings(struct net_device *dev,
	u32 stringset, u8 *data)
{
	const struct ethtool_ops *ops = dev->ethtool_ops;

	if (stringset == ETH_SS_FEATURES)
		memcpy(data, netdev_features_strings,
			sizeof(netdev_features_strings));
	else
		/* ops->get_strings is valid because checked earlier */
		ops->get_strings(dev, stringset, data);
}

<<<<<<< HEAD
<<<<<<< HEAD
static u32 ethtool_get_feature_mask(u32 eth_cmd)
=======
static netdev_features_t ethtool_get_feature_mask(u32 eth_cmd)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static netdev_features_t ethtool_get_feature_mask(u32 eth_cmd)
>>>>>>> refs/remotes/origin/master
{
	/* feature masks of legacy discrete ethtool ops */

	switch (eth_cmd) {
	case ETHTOOL_GTXCSUM:
	case ETHTOOL_STXCSUM:
		return NETIF_F_ALL_CSUM | NETIF_F_SCTP_CSUM;
	case ETHTOOL_GRXCSUM:
	case ETHTOOL_SRXCSUM:
		return NETIF_F_RXCSUM;
	case ETHTOOL_GSG:
	case ETHTOOL_SSG:
		return NETIF_F_SG;
	case ETHTOOL_GTSO:
	case ETHTOOL_STSO:
		return NETIF_F_ALL_TSO;
	case ETHTOOL_GUFO:
	case ETHTOOL_SUFO:
		return NETIF_F_UFO;
	case ETHTOOL_GGSO:
	case ETHTOOL_SGSO:
		return NETIF_F_GSO;
	case ETHTOOL_GGRO:
	case ETHTOOL_SGRO:
		return NETIF_F_GRO;
	default:
		BUG();
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
static void *__ethtool_get_one_feature_actor(struct net_device *dev, u32 ethcmd)
{
	const struct ethtool_ops *ops = dev->ethtool_ops;

	if (!ops)
		return NULL;

	switch (ethcmd) {
	case ETHTOOL_GTXCSUM:
		return ops->get_tx_csum;
	case ETHTOOL_GRXCSUM:
		return ops->get_rx_csum;
	case ETHTOOL_SSG:
		return ops->get_sg;
	case ETHTOOL_STSO:
		return ops->get_tso;
	case ETHTOOL_SUFO:
		return ops->get_ufo;
	default:
		return NULL;
	}
}

static u32 __ethtool_get_rx_csum_oldbug(struct net_device *dev)
{
	return !!(dev->features & NETIF_F_ALL_CSUM);
}

static int ethtool_get_one_feature(struct net_device *dev,
	char __user *useraddr, u32 ethcmd)
{
	u32 mask = ethtool_get_feature_mask(ethcmd);
=======
=======
>>>>>>> refs/remotes/origin/master
static int ethtool_get_one_feature(struct net_device *dev,
	char __user *useraddr, u32 ethcmd)
{
	netdev_features_t mask = ethtool_get_feature_mask(ethcmd);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	struct ethtool_value edata = {
		.cmd = ethcmd,
		.data = !!(dev->features & mask),
	};

<<<<<<< HEAD
<<<<<<< HEAD
	/* compatibility with discrete get_ ops */
	if (!(dev->hw_features & mask)) {
		u32 (*actor)(struct net_device *);

		actor = __ethtool_get_one_feature_actor(dev, ethcmd);

		/* bug compatibility with old get_rx_csum */
		if (ethcmd == ETHTOOL_GRXCSUM && !actor)
			actor = __ethtool_get_rx_csum_oldbug;

		if (actor)
			edata.data = actor(dev);
	}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (copy_to_user(useraddr, &edata, sizeof(edata)))
		return -EFAULT;
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int __ethtool_set_tx_csum(struct net_device *dev, u32 data);
static int __ethtool_set_rx_csum(struct net_device *dev, u32 data);
static int __ethtool_set_sg(struct net_device *dev, u32 data);
static int __ethtool_set_tso(struct net_device *dev, u32 data);
static int __ethtool_set_ufo(struct net_device *dev, u32 data);

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static int ethtool_set_one_feature(struct net_device *dev,
	void __user *useraddr, u32 ethcmd)
{
	struct ethtool_value edata;
<<<<<<< HEAD
<<<<<<< HEAD
	u32 mask;
=======
	netdev_features_t mask;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	netdev_features_t mask;
>>>>>>> refs/remotes/origin/master

	if (copy_from_user(&edata, useraddr, sizeof(edata)))
		return -EFAULT;

	mask = ethtool_get_feature_mask(ethcmd);
	mask &= dev->hw_features;
<<<<<<< HEAD
<<<<<<< HEAD
	if (mask) {
		if (edata.data)
			dev->wanted_features |= mask;
		else
			dev->wanted_features &= ~mask;

		__netdev_update_features(dev);
		return 0;
	}

	/* Driver is not converted to ndo_fix_features or does not
	 * support changing this offload. In the latter case it won't
	 * have corresponding ethtool_ops field set.
	 *
	 * Following part is to be removed after all drivers advertise
	 * their changeable features in netdev->hw_features and stop
	 * using discrete offload setting ops.
	 */

	switch (ethcmd) {
	case ETHTOOL_STXCSUM:
		return __ethtool_set_tx_csum(dev, edata.data);
	case ETHTOOL_SRXCSUM:
		return __ethtool_set_rx_csum(dev, edata.data);
	case ETHTOOL_SSG:
		return __ethtool_set_sg(dev, edata.data);
	case ETHTOOL_STSO:
		return __ethtool_set_tso(dev, edata.data);
	case ETHTOOL_SUFO:
		return __ethtool_set_ufo(dev, edata.data);
	default:
		return -EOPNOTSUPP;
	}
}

int __ethtool_set_flags(struct net_device *dev, u32 data)
{
	u32 changed;

	if (data & ~flags_dup_features)
		return -EINVAL;

	/* legacy set_flags() op */
	if (dev->ethtool_ops->set_flags) {
		if (unlikely(dev->hw_features & flags_dup_features))
			netdev_warn(dev,
				"driver BUG: mixed hw_features and set_flags()\n");
		return dev->ethtool_ops->set_flags(dev, data);
	}

	/* allow changing only bits set in hw_features */
	changed = (data ^ dev->features) & flags_dup_features;
=======
=======
>>>>>>> refs/remotes/origin/master
	if (!mask)
		return -EOPNOTSUPP;

	if (edata.data)
		dev->wanted_features |= mask;
	else
		dev->wanted_features &= ~mask;

	__netdev_update_features(dev);

	return 0;
}

#define ETH_ALL_FLAGS    (ETH_FLAG_LRO | ETH_FLAG_RXVLAN | ETH_FLAG_TXVLAN | \
			  ETH_FLAG_NTUPLE | ETH_FLAG_RXHASH)
<<<<<<< HEAD
#define ETH_ALL_FEATURES (NETIF_F_LRO | NETIF_F_HW_VLAN_RX | \
			  NETIF_F_HW_VLAN_TX | NETIF_F_NTUPLE | NETIF_F_RXHASH)
=======
#define ETH_ALL_FEATURES (NETIF_F_LRO | NETIF_F_HW_VLAN_CTAG_RX | \
			  NETIF_F_HW_VLAN_CTAG_TX | NETIF_F_NTUPLE | \
			  NETIF_F_RXHASH)
>>>>>>> refs/remotes/origin/master

static u32 __ethtool_get_flags(struct net_device *dev)
{
	u32 flags = 0;

<<<<<<< HEAD
	if (dev->features & NETIF_F_LRO)	flags |= ETH_FLAG_LRO;
	if (dev->features & NETIF_F_HW_VLAN_RX)	flags |= ETH_FLAG_RXVLAN;
	if (dev->features & NETIF_F_HW_VLAN_TX)	flags |= ETH_FLAG_TXVLAN;
	if (dev->features & NETIF_F_NTUPLE)	flags |= ETH_FLAG_NTUPLE;
	if (dev->features & NETIF_F_RXHASH)	flags |= ETH_FLAG_RXHASH;
=======
	if (dev->features & NETIF_F_LRO)
		flags |= ETH_FLAG_LRO;
	if (dev->features & NETIF_F_HW_VLAN_CTAG_RX)
		flags |= ETH_FLAG_RXVLAN;
	if (dev->features & NETIF_F_HW_VLAN_CTAG_TX)
		flags |= ETH_FLAG_TXVLAN;
	if (dev->features & NETIF_F_NTUPLE)
		flags |= ETH_FLAG_NTUPLE;
	if (dev->features & NETIF_F_RXHASH)
		flags |= ETH_FLAG_RXHASH;
>>>>>>> refs/remotes/origin/master

	return flags;
}

static int __ethtool_set_flags(struct net_device *dev, u32 data)
{
	netdev_features_t features = 0, changed;

	if (data & ~ETH_ALL_FLAGS)
		return -EINVAL;

<<<<<<< HEAD
	if (data & ETH_FLAG_LRO)	features |= NETIF_F_LRO;
	if (data & ETH_FLAG_RXVLAN)	features |= NETIF_F_HW_VLAN_RX;
	if (data & ETH_FLAG_TXVLAN)	features |= NETIF_F_HW_VLAN_TX;
	if (data & ETH_FLAG_NTUPLE)	features |= NETIF_F_NTUPLE;
	if (data & ETH_FLAG_RXHASH)	features |= NETIF_F_RXHASH;

	/* allow changing only bits set in hw_features */
	changed = (features ^ dev->features) & ETH_ALL_FEATURES;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (data & ETH_FLAG_LRO)
		features |= NETIF_F_LRO;
	if (data & ETH_FLAG_RXVLAN)
		features |= NETIF_F_HW_VLAN_CTAG_RX;
	if (data & ETH_FLAG_TXVLAN)
		features |= NETIF_F_HW_VLAN_CTAG_TX;
	if (data & ETH_FLAG_NTUPLE)
		features |= NETIF_F_NTUPLE;
	if (data & ETH_FLAG_RXHASH)
		features |= NETIF_F_RXHASH;

	/* allow changing only bits set in hw_features */
	changed = (features ^ dev->features) & ETH_ALL_FEATURES;
>>>>>>> refs/remotes/origin/master
	if (changed & ~dev->hw_features)
		return (changed & dev->hw_features) ? -EINVAL : -EOPNOTSUPP;

	dev->wanted_features =
<<<<<<< HEAD
<<<<<<< HEAD
		(dev->wanted_features & ~changed) | (data & dev->hw_features);
=======
		(dev->wanted_features & ~changed) | (features & changed);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		(dev->wanted_features & ~changed) | (features & changed);
>>>>>>> refs/remotes/origin/master

	__netdev_update_features(dev);

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int ethtool_get_settings(struct net_device *dev, void __user *useraddr)
{
	struct ethtool_cmd cmd = { .cmd = ETHTOOL_GSET };
	int err;

	if (!dev->ethtool_ops->get_settings)
		return -EOPNOTSUPP;

	err = dev->ethtool_ops->get_settings(dev, &cmd);
=======
=======
>>>>>>> refs/remotes/origin/master
int __ethtool_get_settings(struct net_device *dev, struct ethtool_cmd *cmd)
{
	ASSERT_RTNL();

<<<<<<< HEAD
	if (!dev->ethtool_ops || !dev->ethtool_ops->get_settings)
=======
	if (!dev->ethtool_ops->get_settings)
>>>>>>> refs/remotes/origin/master
		return -EOPNOTSUPP;

	memset(cmd, 0, sizeof(struct ethtool_cmd));
	cmd->cmd = ETHTOOL_GSET;
	return dev->ethtool_ops->get_settings(dev, cmd);
}
EXPORT_SYMBOL(__ethtool_get_settings);

static int ethtool_get_settings(struct net_device *dev, void __user *useraddr)
{
	int err;
	struct ethtool_cmd cmd;

	err = __ethtool_get_settings(dev, &cmd);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (err < 0)
		return err;

	if (copy_to_user(useraddr, &cmd, sizeof(cmd)))
		return -EFAULT;
	return 0;
}

static int ethtool_set_settings(struct net_device *dev, void __user *useraddr)
{
	struct ethtool_cmd cmd;

	if (!dev->ethtool_ops->set_settings)
		return -EOPNOTSUPP;

	if (copy_from_user(&cmd, useraddr, sizeof(cmd)))
		return -EFAULT;

	return dev->ethtool_ops->set_settings(dev, &cmd);
}

static noinline_for_stack int ethtool_get_drvinfo(struct net_device *dev,
						  void __user *useraddr)
{
	struct ethtool_drvinfo info;
	const struct ethtool_ops *ops = dev->ethtool_ops;

	memset(&info, 0, sizeof(info));
	info.cmd = ETHTOOL_GDRVINFO;
<<<<<<< HEAD
	if (ops && ops->get_drvinfo) {
=======
	if (ops->get_drvinfo) {
>>>>>>> refs/remotes/origin/master
		ops->get_drvinfo(dev, &info);
	} else if (dev->dev.parent && dev->dev.parent->driver) {
		strlcpy(info.bus_info, dev_name(dev->dev.parent),
			sizeof(info.bus_info));
		strlcpy(info.driver, dev->dev.parent->driver->name,
			sizeof(info.driver));
	} else {
		return -EOPNOTSUPP;
	}

	/*
	 * this method of obtaining string set info is deprecated;
	 * Use ETHTOOL_GSSET_INFO instead.
	 */
<<<<<<< HEAD
	if (ops && ops->get_sset_count) {
=======
	if (ops->get_sset_count) {
>>>>>>> refs/remotes/origin/master
		int rc;

		rc = ops->get_sset_count(dev, ETH_SS_TEST);
		if (rc >= 0)
			info.testinfo_len = rc;
		rc = ops->get_sset_count(dev, ETH_SS_STATS);
		if (rc >= 0)
			info.n_stats = rc;
		rc = ops->get_sset_count(dev, ETH_SS_PRIV_FLAGS);
		if (rc >= 0)
			info.n_priv_flags = rc;
	}
<<<<<<< HEAD
	if (ops && ops->get_regs_len)
		info.regdump_len = ops->get_regs_len(dev);
	if (ops && ops->get_eeprom_len)
=======
	if (ops->get_regs_len)
		info.regdump_len = ops->get_regs_len(dev);
	if (ops->get_eeprom_len)
>>>>>>> refs/remotes/origin/master
		info.eedump_len = ops->get_eeprom_len(dev);

	if (copy_to_user(useraddr, &info, sizeof(info)))
		return -EFAULT;
	return 0;
}

static noinline_for_stack int ethtool_get_sset_info(struct net_device *dev,
						    void __user *useraddr)
{
	struct ethtool_sset_info info;
	u64 sset_mask;
	int i, idx = 0, n_bits = 0, ret, rc;
	u32 *info_buf = NULL;

	if (copy_from_user(&info, useraddr, sizeof(info)))
		return -EFAULT;

	/* store copy of mask, because we zero struct later on */
	sset_mask = info.sset_mask;
	if (!sset_mask)
		return 0;

	/* calculate size of return buffer */
	n_bits = hweight64(sset_mask);

	memset(&info, 0, sizeof(info));
	info.cmd = ETHTOOL_GSSET_INFO;

	info_buf = kzalloc(n_bits * sizeof(u32), GFP_USER);
	if (!info_buf)
		return -ENOMEM;

	/*
	 * fill return buffer based on input bitmask and successful
	 * get_sset_count return
	 */
	for (i = 0; i < 64; i++) {
		if (!(sset_mask & (1ULL << i)))
			continue;

		rc = __ethtool_get_sset_count(dev, i);
		if (rc >= 0) {
			info.sset_mask |= (1ULL << i);
			info_buf[idx++] = rc;
		}
	}

	ret = -EFAULT;
	if (copy_to_user(useraddr, &info, sizeof(info)))
		goto out;

	useraddr += offsetof(struct ethtool_sset_info, data);
	if (copy_to_user(useraddr, info_buf, idx * sizeof(u32)))
		goto out;

	ret = 0;

out:
	kfree(info_buf);
	return ret;
}

static noinline_for_stack int ethtool_set_rxnfc(struct net_device *dev,
						u32 cmd, void __user *useraddr)
{
	struct ethtool_rxnfc info;
	size_t info_size = sizeof(info);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	int rc;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	int rc;
>>>>>>> refs/remotes/origin/master

	if (!dev->ethtool_ops->set_rxnfc)
		return -EOPNOTSUPP;

	/* struct ethtool_rxnfc was originally defined for
	 * ETHTOOL_{G,S}RXFH with only the cmd, flow_type and data
	 * members.  User-space might still be using that
	 * definition. */
	if (cmd == ETHTOOL_SRXFH)
		info_size = (offsetof(struct ethtool_rxnfc, data) +
			     sizeof(info.data));

	if (copy_from_user(&info, useraddr, info_size))
		return -EFAULT;

<<<<<<< HEAD
<<<<<<< HEAD
	return dev->ethtool_ops->set_rxnfc(dev, &info);
=======
=======
>>>>>>> refs/remotes/origin/master
	rc = dev->ethtool_ops->set_rxnfc(dev, &info);
	if (rc)
		return rc;

	if (cmd == ETHTOOL_SRXCLSRLINS &&
	    copy_to_user(useraddr, &info, info_size))
		return -EFAULT;

	return 0;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static noinline_for_stack int ethtool_get_rxnfc(struct net_device *dev,
						u32 cmd, void __user *useraddr)
{
	struct ethtool_rxnfc info;
	size_t info_size = sizeof(info);
	const struct ethtool_ops *ops = dev->ethtool_ops;
	int ret;
	void *rule_buf = NULL;

	if (!ops->get_rxnfc)
		return -EOPNOTSUPP;

	/* struct ethtool_rxnfc was originally defined for
	 * ETHTOOL_{G,S}RXFH with only the cmd, flow_type and data
	 * members.  User-space might still be using that
	 * definition. */
	if (cmd == ETHTOOL_GRXFH)
		info_size = (offsetof(struct ethtool_rxnfc, data) +
			     sizeof(info.data));

	if (copy_from_user(&info, useraddr, info_size))
		return -EFAULT;

	if (info.cmd == ETHTOOL_GRXCLSRLALL) {
		if (info.rule_cnt > 0) {
			if (info.rule_cnt <= KMALLOC_MAX_SIZE / sizeof(u32))
				rule_buf = kzalloc(info.rule_cnt * sizeof(u32),
						   GFP_USER);
			if (!rule_buf)
				return -ENOMEM;
		}
	}

	ret = ops->get_rxnfc(dev, &info, rule_buf);
	if (ret < 0)
		goto err_out;

	ret = -EFAULT;
	if (copy_to_user(useraddr, &info, info_size))
		goto err_out;

	if (rule_buf) {
		useraddr += offsetof(struct ethtool_rxnfc, rule_locs);
		if (copy_to_user(useraddr, rule_buf,
				 info.rule_cnt * sizeof(u32)))
			goto err_out;
	}
	ret = 0;

err_out:
	kfree(rule_buf);

	return ret;
}

static noinline_for_stack int ethtool_get_rxfh_indir(struct net_device *dev,
						     void __user *useraddr)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct ethtool_rxfh_indir *indir;
	u32 table_size;
	size_t full_size;
	int ret;

	if (!dev->ethtool_ops->get_rxfh_indir)
		return -EOPNOTSUPP;

	if (copy_from_user(&table_size,
			   useraddr + offsetof(struct ethtool_rxfh_indir, size),
			   sizeof(table_size)))
		return -EFAULT;

	if (table_size >
	    (KMALLOC_MAX_SIZE - sizeof(*indir)) / sizeof(*indir->ring_index))
		return -ENOMEM;
	full_size = sizeof(*indir) + sizeof(*indir->ring_index) * table_size;
	indir = kzalloc(full_size, GFP_USER);
	if (!indir)
		return -ENOMEM;

	indir->cmd = ETHTOOL_GRXFHINDIR;
	indir->size = table_size;
=======
=======
>>>>>>> refs/remotes/origin/master
	u32 user_size, dev_size;
	u32 *indir;
	int ret;

	if (!dev->ethtool_ops->get_rxfh_indir_size ||
	    !dev->ethtool_ops->get_rxfh_indir)
		return -EOPNOTSUPP;
	dev_size = dev->ethtool_ops->get_rxfh_indir_size(dev);
	if (dev_size == 0)
		return -EOPNOTSUPP;

	if (copy_from_user(&user_size,
			   useraddr + offsetof(struct ethtool_rxfh_indir, size),
			   sizeof(user_size)))
		return -EFAULT;

	if (copy_to_user(useraddr + offsetof(struct ethtool_rxfh_indir, size),
			 &dev_size, sizeof(dev_size)))
		return -EFAULT;

	/* If the user buffer size is 0, this is just a query for the
	 * device table size.  Otherwise, if it's smaller than the
	 * device table size it's an error.
	 */
	if (user_size < dev_size)
		return user_size == 0 ? 0 : -EINVAL;

	indir = kcalloc(dev_size, sizeof(indir[0]), GFP_USER);
	if (!indir)
		return -ENOMEM;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	ret = dev->ethtool_ops->get_rxfh_indir(dev, indir);
	if (ret)
		goto out;

<<<<<<< HEAD
<<<<<<< HEAD
	if (copy_to_user(useraddr, indir, full_size))
=======
	if (copy_to_user(useraddr +
			 offsetof(struct ethtool_rxfh_indir, ring_index[0]),
			 indir, dev_size * sizeof(indir[0])))
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (copy_to_user(useraddr +
			 offsetof(struct ethtool_rxfh_indir, ring_index[0]),
			 indir, dev_size * sizeof(indir[0])))
>>>>>>> refs/remotes/origin/master
		ret = -EFAULT;

out:
	kfree(indir);
	return ret;
}

static noinline_for_stack int ethtool_set_rxfh_indir(struct net_device *dev,
						     void __user *useraddr)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct ethtool_rxfh_indir *indir;
	u32 table_size;
	size_t full_size;
	int ret;

	if (!dev->ethtool_ops->set_rxfh_indir)
		return -EOPNOTSUPP;

	if (copy_from_user(&table_size,
			   useraddr + offsetof(struct ethtool_rxfh_indir, size),
			   sizeof(table_size)))
		return -EFAULT;

	if (table_size >
	    (KMALLOC_MAX_SIZE - sizeof(*indir)) / sizeof(*indir->ring_index))
		return -ENOMEM;
	full_size = sizeof(*indir) + sizeof(*indir->ring_index) * table_size;
	indir = kmalloc(full_size, GFP_USER);
	if (!indir)
		return -ENOMEM;

	if (copy_from_user(indir, useraddr, full_size)) {
		ret = -EFAULT;
		goto out;
	}

	ret = dev->ethtool_ops->set_rxfh_indir(dev, indir);

out:
	kfree(indir);
	return ret;
}

static void __rx_ntuple_filter_add(struct ethtool_rx_ntuple_list *list,
			struct ethtool_rx_ntuple_flow_spec *spec,
			struct ethtool_rx_ntuple_flow_spec_container *fsc)
{

	/* don't add filters forever */
	if (list->count >= ETHTOOL_MAX_NTUPLE_LIST_ENTRY) {
		/* free the container */
		kfree(fsc);
		return;
	}

	/* Copy the whole filter over */
	fsc->fs.flow_type = spec->flow_type;
	memcpy(&fsc->fs.h_u, &spec->h_u, sizeof(spec->h_u));
	memcpy(&fsc->fs.m_u, &spec->m_u, sizeof(spec->m_u));

	fsc->fs.vlan_tag = spec->vlan_tag;
	fsc->fs.vlan_tag_mask = spec->vlan_tag_mask;
	fsc->fs.data = spec->data;
	fsc->fs.data_mask = spec->data_mask;
	fsc->fs.action = spec->action;

	/* add to the list */
	list_add_tail_rcu(&fsc->list, &list->list);
	list->count++;
}

/*
 * ethtool does not (or did not) set masks for flow parameters that are
 * not specified, so if both value and mask are 0 then this must be
 * treated as equivalent to a mask with all bits set.  Implement that
 * here rather than in drivers.
 */
static void rx_ntuple_fix_masks(struct ethtool_rx_ntuple_flow_spec *fs)
{
	struct ethtool_tcpip4_spec *entry = &fs->h_u.tcp_ip4_spec;
	struct ethtool_tcpip4_spec *mask = &fs->m_u.tcp_ip4_spec;

	if (fs->flow_type != TCP_V4_FLOW &&
	    fs->flow_type != UDP_V4_FLOW &&
	    fs->flow_type != SCTP_V4_FLOW)
		return;

	if (!(entry->ip4src | mask->ip4src))
		mask->ip4src = htonl(0xffffffff);
	if (!(entry->ip4dst | mask->ip4dst))
		mask->ip4dst = htonl(0xffffffff);
	if (!(entry->psrc | mask->psrc))
		mask->psrc = htons(0xffff);
	if (!(entry->pdst | mask->pdst))
		mask->pdst = htons(0xffff);
	if (!(entry->tos | mask->tos))
		mask->tos = 0xff;
	if (!(fs->vlan_tag | fs->vlan_tag_mask))
		fs->vlan_tag_mask = 0xffff;
	if (!(fs->data | fs->data_mask))
		fs->data_mask = 0xffffffffffffffffULL;
}

static noinline_for_stack int ethtool_set_rx_ntuple(struct net_device *dev,
						    void __user *useraddr)
{
	struct ethtool_rx_ntuple cmd;
	const struct ethtool_ops *ops = dev->ethtool_ops;
	struct ethtool_rx_ntuple_flow_spec_container *fsc = NULL;
	int ret;

	if (!ops->set_rx_ntuple)
		return -EOPNOTSUPP;

	if (!(dev->features & NETIF_F_NTUPLE))
		return -EINVAL;

	if (copy_from_user(&cmd, useraddr, sizeof(cmd)))
		return -EFAULT;

	rx_ntuple_fix_masks(&cmd.fs);

	/*
	 * Cache filter in dev struct for GET operation only if
	 * the underlying driver doesn't have its own GET operation, and
	 * only if the filter was added successfully.  First make sure we
	 * can allocate the filter, then continue if successful.
	 */
	if (!ops->get_rx_ntuple) {
		fsc = kmalloc(sizeof(*fsc), GFP_ATOMIC);
		if (!fsc)
			return -ENOMEM;
	}

	ret = ops->set_rx_ntuple(dev, &cmd);
	if (ret) {
		kfree(fsc);
		return ret;
	}

	if (!ops->get_rx_ntuple)
		__rx_ntuple_filter_add(&dev->ethtool_ntuple_list, &cmd.fs, fsc);

	return ret;
}

static int ethtool_get_rx_ntuple(struct net_device *dev, void __user *useraddr)
{
	struct ethtool_gstrings gstrings;
	const struct ethtool_ops *ops = dev->ethtool_ops;
	struct ethtool_rx_ntuple_flow_spec_container *fsc;
	u8 *data;
	char *p;
	int ret, i, num_strings = 0;

	if (!ops->get_sset_count)
		return -EOPNOTSUPP;

	if (copy_from_user(&gstrings, useraddr, sizeof(gstrings)))
		return -EFAULT;

	ret = ops->get_sset_count(dev, gstrings.string_set);
	if (ret < 0)
		return ret;

	gstrings.len = ret;

	data = kzalloc(gstrings.len * ETH_GSTRING_LEN, GFP_USER);
	if (!data)
		return -ENOMEM;

	if (ops->get_rx_ntuple) {
		/* driver-specific filter grab */
		ret = ops->get_rx_ntuple(dev, gstrings.string_set, data);
		goto copy;
	}

	/* default ethtool filter grab */
	i = 0;
	p = (char *)data;
	list_for_each_entry(fsc, &dev->ethtool_ntuple_list.list, list) {
		sprintf(p, "Filter %d:\n", i);
		p += ETH_GSTRING_LEN;
		num_strings++;

		switch (fsc->fs.flow_type) {
		case TCP_V4_FLOW:
			sprintf(p, "\tFlow Type: TCP\n");
			p += ETH_GSTRING_LEN;
			num_strings++;
			break;
		case UDP_V4_FLOW:
			sprintf(p, "\tFlow Type: UDP\n");
			p += ETH_GSTRING_LEN;
			num_strings++;
			break;
		case SCTP_V4_FLOW:
			sprintf(p, "\tFlow Type: SCTP\n");
			p += ETH_GSTRING_LEN;
			num_strings++;
			break;
		case AH_ESP_V4_FLOW:
			sprintf(p, "\tFlow Type: AH ESP\n");
			p += ETH_GSTRING_LEN;
			num_strings++;
			break;
		case ESP_V4_FLOW:
			sprintf(p, "\tFlow Type: ESP\n");
			p += ETH_GSTRING_LEN;
			num_strings++;
			break;
		case IP_USER_FLOW:
			sprintf(p, "\tFlow Type: Raw IP\n");
			p += ETH_GSTRING_LEN;
			num_strings++;
			break;
		case IPV4_FLOW:
			sprintf(p, "\tFlow Type: IPv4\n");
			p += ETH_GSTRING_LEN;
			num_strings++;
			break;
		default:
			sprintf(p, "\tFlow Type: Unknown\n");
			p += ETH_GSTRING_LEN;
			num_strings++;
			goto unknown_filter;
		}

		/* now the rest of the filters */
		switch (fsc->fs.flow_type) {
		case TCP_V4_FLOW:
		case UDP_V4_FLOW:
		case SCTP_V4_FLOW:
			sprintf(p, "\tSrc IP addr: 0x%x\n",
				fsc->fs.h_u.tcp_ip4_spec.ip4src);
			p += ETH_GSTRING_LEN;
			num_strings++;
			sprintf(p, "\tSrc IP mask: 0x%x\n",
				fsc->fs.m_u.tcp_ip4_spec.ip4src);
			p += ETH_GSTRING_LEN;
			num_strings++;
			sprintf(p, "\tDest IP addr: 0x%x\n",
				fsc->fs.h_u.tcp_ip4_spec.ip4dst);
			p += ETH_GSTRING_LEN;
			num_strings++;
			sprintf(p, "\tDest IP mask: 0x%x\n",
				fsc->fs.m_u.tcp_ip4_spec.ip4dst);
			p += ETH_GSTRING_LEN;
			num_strings++;
			sprintf(p, "\tSrc Port: %d, mask: 0x%x\n",
				fsc->fs.h_u.tcp_ip4_spec.psrc,
				fsc->fs.m_u.tcp_ip4_spec.psrc);
			p += ETH_GSTRING_LEN;
			num_strings++;
			sprintf(p, "\tDest Port: %d, mask: 0x%x\n",
				fsc->fs.h_u.tcp_ip4_spec.pdst,
				fsc->fs.m_u.tcp_ip4_spec.pdst);
			p += ETH_GSTRING_LEN;
			num_strings++;
			sprintf(p, "\tTOS: %d, mask: 0x%x\n",
				fsc->fs.h_u.tcp_ip4_spec.tos,
				fsc->fs.m_u.tcp_ip4_spec.tos);
			p += ETH_GSTRING_LEN;
			num_strings++;
			break;
		case AH_ESP_V4_FLOW:
		case ESP_V4_FLOW:
			sprintf(p, "\tSrc IP addr: 0x%x\n",
				fsc->fs.h_u.ah_ip4_spec.ip4src);
			p += ETH_GSTRING_LEN;
			num_strings++;
			sprintf(p, "\tSrc IP mask: 0x%x\n",
				fsc->fs.m_u.ah_ip4_spec.ip4src);
			p += ETH_GSTRING_LEN;
			num_strings++;
			sprintf(p, "\tDest IP addr: 0x%x\n",
				fsc->fs.h_u.ah_ip4_spec.ip4dst);
			p += ETH_GSTRING_LEN;
			num_strings++;
			sprintf(p, "\tDest IP mask: 0x%x\n",
				fsc->fs.m_u.ah_ip4_spec.ip4dst);
			p += ETH_GSTRING_LEN;
			num_strings++;
			sprintf(p, "\tSPI: %d, mask: 0x%x\n",
				fsc->fs.h_u.ah_ip4_spec.spi,
				fsc->fs.m_u.ah_ip4_spec.spi);
			p += ETH_GSTRING_LEN;
			num_strings++;
			sprintf(p, "\tTOS: %d, mask: 0x%x\n",
				fsc->fs.h_u.ah_ip4_spec.tos,
				fsc->fs.m_u.ah_ip4_spec.tos);
			p += ETH_GSTRING_LEN;
			num_strings++;
			break;
		case IP_USER_FLOW:
			sprintf(p, "\tSrc IP addr: 0x%x\n",
				fsc->fs.h_u.usr_ip4_spec.ip4src);
			p += ETH_GSTRING_LEN;
			num_strings++;
			sprintf(p, "\tSrc IP mask: 0x%x\n",
				fsc->fs.m_u.usr_ip4_spec.ip4src);
			p += ETH_GSTRING_LEN;
			num_strings++;
			sprintf(p, "\tDest IP addr: 0x%x\n",
				fsc->fs.h_u.usr_ip4_spec.ip4dst);
			p += ETH_GSTRING_LEN;
			num_strings++;
			sprintf(p, "\tDest IP mask: 0x%x\n",
				fsc->fs.m_u.usr_ip4_spec.ip4dst);
			p += ETH_GSTRING_LEN;
			num_strings++;
			break;
		case IPV4_FLOW:
			sprintf(p, "\tSrc IP addr: 0x%x\n",
				fsc->fs.h_u.usr_ip4_spec.ip4src);
			p += ETH_GSTRING_LEN;
			num_strings++;
			sprintf(p, "\tSrc IP mask: 0x%x\n",
				fsc->fs.m_u.usr_ip4_spec.ip4src);
			p += ETH_GSTRING_LEN;
			num_strings++;
			sprintf(p, "\tDest IP addr: 0x%x\n",
				fsc->fs.h_u.usr_ip4_spec.ip4dst);
			p += ETH_GSTRING_LEN;
			num_strings++;
			sprintf(p, "\tDest IP mask: 0x%x\n",
				fsc->fs.m_u.usr_ip4_spec.ip4dst);
			p += ETH_GSTRING_LEN;
			num_strings++;
			sprintf(p, "\tL4 bytes: 0x%x, mask: 0x%x\n",
				fsc->fs.h_u.usr_ip4_spec.l4_4_bytes,
				fsc->fs.m_u.usr_ip4_spec.l4_4_bytes);
			p += ETH_GSTRING_LEN;
			num_strings++;
			sprintf(p, "\tTOS: %d, mask: 0x%x\n",
				fsc->fs.h_u.usr_ip4_spec.tos,
				fsc->fs.m_u.usr_ip4_spec.tos);
			p += ETH_GSTRING_LEN;
			num_strings++;
			sprintf(p, "\tIP Version: %d, mask: 0x%x\n",
				fsc->fs.h_u.usr_ip4_spec.ip_ver,
				fsc->fs.m_u.usr_ip4_spec.ip_ver);
			p += ETH_GSTRING_LEN;
			num_strings++;
			sprintf(p, "\tProtocol: %d, mask: 0x%x\n",
				fsc->fs.h_u.usr_ip4_spec.proto,
				fsc->fs.m_u.usr_ip4_spec.proto);
			p += ETH_GSTRING_LEN;
			num_strings++;
			break;
		}
		sprintf(p, "\tVLAN: %d, mask: 0x%x\n",
			fsc->fs.vlan_tag, fsc->fs.vlan_tag_mask);
		p += ETH_GSTRING_LEN;
		num_strings++;
		sprintf(p, "\tUser-defined: 0x%Lx\n", fsc->fs.data);
		p += ETH_GSTRING_LEN;
		num_strings++;
		sprintf(p, "\tUser-defined mask: 0x%Lx\n", fsc->fs.data_mask);
		p += ETH_GSTRING_LEN;
		num_strings++;
		if (fsc->fs.action == ETHTOOL_RXNTUPLE_ACTION_DROP)
			sprintf(p, "\tAction: Drop\n");
		else
			sprintf(p, "\tAction: Direct to queue %d\n",
				fsc->fs.action);
		p += ETH_GSTRING_LEN;
		num_strings++;
unknown_filter:
		i++;
	}
copy:
	/* indicate to userspace how many strings we actually have */
	gstrings.len = num_strings;
	ret = -EFAULT;
	if (copy_to_user(useraddr, &gstrings, sizeof(gstrings)))
		goto out;
	useraddr += sizeof(gstrings);
	if (copy_to_user(useraddr, data, gstrings.len * ETH_GSTRING_LEN))
		goto out;
	ret = 0;

out:
	kfree(data);
=======
	struct ethtool_rxnfc rx_rings;
	u32 user_size, dev_size, i;
	u32 *indir;
	int ret;

	if (!dev->ethtool_ops->get_rxfh_indir_size ||
	    !dev->ethtool_ops->set_rxfh_indir ||
	    !dev->ethtool_ops->get_rxnfc)
		return -EOPNOTSUPP;
	dev_size = dev->ethtool_ops->get_rxfh_indir_size(dev);
=======
	struct ethtool_rxnfc rx_rings;
	u32 user_size, dev_size, i;
	u32 *indir;
	const struct ethtool_ops *ops = dev->ethtool_ops;
	int ret;

	if (!ops->get_rxfh_indir_size || !ops->set_rxfh_indir ||
	    !ops->get_rxnfc)
		return -EOPNOTSUPP;

	dev_size = ops->get_rxfh_indir_size(dev);
>>>>>>> refs/remotes/origin/master
	if (dev_size == 0)
		return -EOPNOTSUPP;

	if (copy_from_user(&user_size,
			   useraddr + offsetof(struct ethtool_rxfh_indir, size),
			   sizeof(user_size)))
		return -EFAULT;

	if (user_size != 0 && user_size != dev_size)
		return -EINVAL;

	indir = kcalloc(dev_size, sizeof(indir[0]), GFP_USER);
	if (!indir)
		return -ENOMEM;

	rx_rings.cmd = ETHTOOL_GRXRINGS;
<<<<<<< HEAD
	ret = dev->ethtool_ops->get_rxnfc(dev, &rx_rings, NULL);
=======
	ret = ops->get_rxnfc(dev, &rx_rings, NULL);
>>>>>>> refs/remotes/origin/master
	if (ret)
		goto out;

	if (user_size == 0) {
		for (i = 0; i < dev_size; i++)
			indir[i] = ethtool_rxfh_indir_default(i, rx_rings.data);
	} else {
		if (copy_from_user(indir,
				  useraddr +
				  offsetof(struct ethtool_rxfh_indir,
					   ring_index[0]),
				  dev_size * sizeof(indir[0]))) {
			ret = -EFAULT;
			goto out;
		}

		/* Validate ring indices */
		for (i = 0; i < dev_size; i++) {
			if (indir[i] >= rx_rings.data) {
				ret = -EINVAL;
				goto out;
			}
		}
	}

<<<<<<< HEAD
	ret = dev->ethtool_ops->set_rxfh_indir(dev, indir);

out:
	kfree(indir);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ret = ops->set_rxfh_indir(dev, indir);

out:
	kfree(indir);
>>>>>>> refs/remotes/origin/master
	return ret;
}

static int ethtool_get_regs(struct net_device *dev, char __user *useraddr)
{
	struct ethtool_regs regs;
	const struct ethtool_ops *ops = dev->ethtool_ops;
	void *regbuf;
	int reglen, ret;

	if (!ops->get_regs || !ops->get_regs_len)
		return -EOPNOTSUPP;

	if (copy_from_user(&regs, useraddr, sizeof(regs)))
		return -EFAULT;

	reglen = ops->get_regs_len(dev);
	if (regs.len > reglen)
		regs.len = reglen;

	regbuf = vzalloc(reglen);
	if (reglen && !regbuf)
		return -ENOMEM;

	ops->get_regs(dev, &regs, regbuf);

	ret = -EFAULT;
	if (copy_to_user(useraddr, &regs, sizeof(regs)))
		goto out;
	useraddr += offsetof(struct ethtool_regs, data);
	if (regbuf && copy_to_user(useraddr, regbuf, regs.len))
		goto out;
	ret = 0;

 out:
	vfree(regbuf);
	return ret;
}

static int ethtool_reset(struct net_device *dev, char __user *useraddr)
{
	struct ethtool_value reset;
	int ret;

	if (!dev->ethtool_ops->reset)
		return -EOPNOTSUPP;

	if (copy_from_user(&reset, useraddr, sizeof(reset)))
		return -EFAULT;

	ret = dev->ethtool_ops->reset(dev, &reset.data);
	if (ret)
		return ret;

	if (copy_to_user(useraddr, &reset, sizeof(reset)))
		return -EFAULT;
	return 0;
}

static int ethtool_get_wol(struct net_device *dev, char __user *useraddr)
{
	struct ethtool_wolinfo wol = { .cmd = ETHTOOL_GWOL };

	if (!dev->ethtool_ops->get_wol)
		return -EOPNOTSUPP;

	dev->ethtool_ops->get_wol(dev, &wol);

	if (copy_to_user(useraddr, &wol, sizeof(wol)))
		return -EFAULT;
	return 0;
}

static int ethtool_set_wol(struct net_device *dev, char __user *useraddr)
{
	struct ethtool_wolinfo wol;

	if (!dev->ethtool_ops->set_wol)
		return -EOPNOTSUPP;

	if (copy_from_user(&wol, useraddr, sizeof(wol)))
		return -EFAULT;

	return dev->ethtool_ops->set_wol(dev, &wol);
}

<<<<<<< HEAD
=======
static int ethtool_get_eee(struct net_device *dev, char __user *useraddr)
{
	struct ethtool_eee edata;
	int rc;

	if (!dev->ethtool_ops->get_eee)
		return -EOPNOTSUPP;

	memset(&edata, 0, sizeof(struct ethtool_eee));
	edata.cmd = ETHTOOL_GEEE;
	rc = dev->ethtool_ops->get_eee(dev, &edata);

	if (rc)
		return rc;

	if (copy_to_user(useraddr, &edata, sizeof(edata)))
		return -EFAULT;

	return 0;
}

static int ethtool_set_eee(struct net_device *dev, char __user *useraddr)
{
	struct ethtool_eee edata;

	if (!dev->ethtool_ops->set_eee)
		return -EOPNOTSUPP;

	if (copy_from_user(&edata, useraddr, sizeof(edata)))
		return -EFAULT;

	return dev->ethtool_ops->set_eee(dev, &edata);
}

>>>>>>> refs/remotes/origin/master
static int ethtool_nway_reset(struct net_device *dev)
{
	if (!dev->ethtool_ops->nway_reset)
		return -EOPNOTSUPP;

	return dev->ethtool_ops->nway_reset(dev);
}

static int ethtool_get_link(struct net_device *dev, char __user *useraddr)
{
	struct ethtool_value edata = { .cmd = ETHTOOL_GLINK };

	if (!dev->ethtool_ops->get_link)
		return -EOPNOTSUPP;

	edata.data = netif_running(dev) && dev->ethtool_ops->get_link(dev);

	if (copy_to_user(useraddr, &edata, sizeof(edata)))
		return -EFAULT;
	return 0;
}

<<<<<<< HEAD
static int ethtool_get_eeprom(struct net_device *dev, void __user *useraddr)
{
	struct ethtool_eeprom eeprom;
	const struct ethtool_ops *ops = dev->ethtool_ops;
=======
static int ethtool_get_any_eeprom(struct net_device *dev, void __user *useraddr,
				  int (*getter)(struct net_device *,
						struct ethtool_eeprom *, u8 *),
				  u32 total_len)
{
	struct ethtool_eeprom eeprom;
>>>>>>> refs/remotes/origin/master
	void __user *userbuf = useraddr + sizeof(eeprom);
	u32 bytes_remaining;
	u8 *data;
	int ret = 0;

<<<<<<< HEAD
	if (!ops->get_eeprom || !ops->get_eeprom_len)
		return -EOPNOTSUPP;

=======
>>>>>>> refs/remotes/origin/master
	if (copy_from_user(&eeprom, useraddr, sizeof(eeprom)))
		return -EFAULT;

	/* Check for wrap and zero */
	if (eeprom.offset + eeprom.len <= eeprom.offset)
		return -EINVAL;

	/* Check for exceeding total eeprom len */
<<<<<<< HEAD
	if (eeprom.offset + eeprom.len > ops->get_eeprom_len(dev))
=======
	if (eeprom.offset + eeprom.len > total_len)
>>>>>>> refs/remotes/origin/master
		return -EINVAL;

	data = kmalloc(PAGE_SIZE, GFP_USER);
	if (!data)
		return -ENOMEM;

	bytes_remaining = eeprom.len;
	while (bytes_remaining > 0) {
		eeprom.len = min(bytes_remaining, (u32)PAGE_SIZE);

<<<<<<< HEAD
		ret = ops->get_eeprom(dev, &eeprom, data);
=======
		ret = getter(dev, &eeprom, data);
>>>>>>> refs/remotes/origin/master
		if (ret)
			break;
		if (copy_to_user(userbuf, data, eeprom.len)) {
			ret = -EFAULT;
			break;
		}
		userbuf += eeprom.len;
		eeprom.offset += eeprom.len;
		bytes_remaining -= eeprom.len;
	}

	eeprom.len = userbuf - (useraddr + sizeof(eeprom));
	eeprom.offset -= eeprom.len;
	if (copy_to_user(useraddr, &eeprom, sizeof(eeprom)))
		ret = -EFAULT;

	kfree(data);
	return ret;
}

<<<<<<< HEAD
=======
static int ethtool_get_eeprom(struct net_device *dev, void __user *useraddr)
{
	const struct ethtool_ops *ops = dev->ethtool_ops;

	if (!ops->get_eeprom || !ops->get_eeprom_len)
		return -EOPNOTSUPP;

	return ethtool_get_any_eeprom(dev, useraddr, ops->get_eeprom,
				      ops->get_eeprom_len(dev));
}

>>>>>>> refs/remotes/origin/master
static int ethtool_set_eeprom(struct net_device *dev, void __user *useraddr)
{
	struct ethtool_eeprom eeprom;
	const struct ethtool_ops *ops = dev->ethtool_ops;
	void __user *userbuf = useraddr + sizeof(eeprom);
	u32 bytes_remaining;
	u8 *data;
	int ret = 0;

	if (!ops->set_eeprom || !ops->get_eeprom_len)
		return -EOPNOTSUPP;

	if (copy_from_user(&eeprom, useraddr, sizeof(eeprom)))
		return -EFAULT;

	/* Check for wrap and zero */
	if (eeprom.offset + eeprom.len <= eeprom.offset)
		return -EINVAL;

	/* Check for exceeding total eeprom len */
	if (eeprom.offset + eeprom.len > ops->get_eeprom_len(dev))
		return -EINVAL;

	data = kmalloc(PAGE_SIZE, GFP_USER);
	if (!data)
		return -ENOMEM;

	bytes_remaining = eeprom.len;
	while (bytes_remaining > 0) {
		eeprom.len = min(bytes_remaining, (u32)PAGE_SIZE);

		if (copy_from_user(data, userbuf, eeprom.len)) {
			ret = -EFAULT;
			break;
		}
		ret = ops->set_eeprom(dev, &eeprom, data);
		if (ret)
			break;
		userbuf += eeprom.len;
		eeprom.offset += eeprom.len;
		bytes_remaining -= eeprom.len;
	}

	kfree(data);
	return ret;
}

static noinline_for_stack int ethtool_get_coalesce(struct net_device *dev,
						   void __user *useraddr)
{
	struct ethtool_coalesce coalesce = { .cmd = ETHTOOL_GCOALESCE };

	if (!dev->ethtool_ops->get_coalesce)
		return -EOPNOTSUPP;

	dev->ethtool_ops->get_coalesce(dev, &coalesce);

	if (copy_to_user(useraddr, &coalesce, sizeof(coalesce)))
		return -EFAULT;
	return 0;
}

static noinline_for_stack int ethtool_set_coalesce(struct net_device *dev,
						   void __user *useraddr)
{
	struct ethtool_coalesce coalesce;

	if (!dev->ethtool_ops->set_coalesce)
		return -EOPNOTSUPP;

	if (copy_from_user(&coalesce, useraddr, sizeof(coalesce)))
		return -EFAULT;

	return dev->ethtool_ops->set_coalesce(dev, &coalesce);
}

static int ethtool_get_ringparam(struct net_device *dev, void __user *useraddr)
{
	struct ethtool_ringparam ringparam = { .cmd = ETHTOOL_GRINGPARAM };

	if (!dev->ethtool_ops->get_ringparam)
		return -EOPNOTSUPP;

	dev->ethtool_ops->get_ringparam(dev, &ringparam);

	if (copy_to_user(useraddr, &ringparam, sizeof(ringparam)))
		return -EFAULT;
	return 0;
}

static int ethtool_set_ringparam(struct net_device *dev, void __user *useraddr)
{
	struct ethtool_ringparam ringparam;

	if (!dev->ethtool_ops->set_ringparam)
		return -EOPNOTSUPP;

	if (copy_from_user(&ringparam, useraddr, sizeof(ringparam)))
		return -EFAULT;

	return dev->ethtool_ops->set_ringparam(dev, &ringparam);
}

static noinline_for_stack int ethtool_get_channels(struct net_device *dev,
						   void __user *useraddr)
{
	struct ethtool_channels channels = { .cmd = ETHTOOL_GCHANNELS };

	if (!dev->ethtool_ops->get_channels)
		return -EOPNOTSUPP;

	dev->ethtool_ops->get_channels(dev, &channels);

	if (copy_to_user(useraddr, &channels, sizeof(channels)))
		return -EFAULT;
	return 0;
}

static noinline_for_stack int ethtool_set_channels(struct net_device *dev,
						   void __user *useraddr)
{
	struct ethtool_channels channels;

	if (!dev->ethtool_ops->set_channels)
		return -EOPNOTSUPP;

	if (copy_from_user(&channels, useraddr, sizeof(channels)))
		return -EFAULT;

	return dev->ethtool_ops->set_channels(dev, &channels);
}

static int ethtool_get_pauseparam(struct net_device *dev, void __user *useraddr)
{
	struct ethtool_pauseparam pauseparam = { ETHTOOL_GPAUSEPARAM };

	if (!dev->ethtool_ops->get_pauseparam)
		return -EOPNOTSUPP;

	dev->ethtool_ops->get_pauseparam(dev, &pauseparam);

	if (copy_to_user(useraddr, &pauseparam, sizeof(pauseparam)))
		return -EFAULT;
	return 0;
}

static int ethtool_set_pauseparam(struct net_device *dev, void __user *useraddr)
{
	struct ethtool_pauseparam pauseparam;

	if (!dev->ethtool_ops->set_pauseparam)
		return -EOPNOTSUPP;

	if (copy_from_user(&pauseparam, useraddr, sizeof(pauseparam)))
		return -EFAULT;

	return dev->ethtool_ops->set_pauseparam(dev, &pauseparam);
}

<<<<<<< HEAD
<<<<<<< HEAD
static int __ethtool_set_sg(struct net_device *dev, u32 data)
{
	int err;

	if (!dev->ethtool_ops->set_sg)
		return -EOPNOTSUPP;

	if (data && !(dev->features & NETIF_F_ALL_CSUM))
		return -EINVAL;

	if (!data && dev->ethtool_ops->set_tso) {
		err = dev->ethtool_ops->set_tso(dev, 0);
		if (err)
			return err;
	}

	if (!data && dev->ethtool_ops->set_ufo) {
		err = dev->ethtool_ops->set_ufo(dev, 0);
		if (err)
			return err;
	}
	return dev->ethtool_ops->set_sg(dev, data);
}

static int __ethtool_set_tx_csum(struct net_device *dev, u32 data)
{
	int err;

	if (!dev->ethtool_ops->set_tx_csum)
		return -EOPNOTSUPP;

	if (!data && dev->ethtool_ops->set_sg) {
		err = __ethtool_set_sg(dev, 0);
		if (err)
			return err;
	}

	return dev->ethtool_ops->set_tx_csum(dev, data);
}

static int __ethtool_set_rx_csum(struct net_device *dev, u32 data)
{
	if (!dev->ethtool_ops->set_rx_csum)
		return -EOPNOTSUPP;

	if (!data)
		dev->features &= ~NETIF_F_GRO;

	return dev->ethtool_ops->set_rx_csum(dev, data);
}

static int __ethtool_set_tso(struct net_device *dev, u32 data)
{
	if (!dev->ethtool_ops->set_tso)
		return -EOPNOTSUPP;

	if (data && !(dev->features & NETIF_F_SG))
		return -EINVAL;

	return dev->ethtool_ops->set_tso(dev, data);
}

static int __ethtool_set_ufo(struct net_device *dev, u32 data)
{
	if (!dev->ethtool_ops->set_ufo)
		return -EOPNOTSUPP;
	if (data && !(dev->features & NETIF_F_SG))
		return -EINVAL;
	if (data && !((dev->features & NETIF_F_GEN_CSUM) ||
		(dev->features & (NETIF_F_IP_CSUM|NETIF_F_IPV6_CSUM))
			== (NETIF_F_IP_CSUM|NETIF_F_IPV6_CSUM)))
		return -EINVAL;
	return dev->ethtool_ops->set_ufo(dev, data);
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static int ethtool_self_test(struct net_device *dev, char __user *useraddr)
{
	struct ethtool_test test;
	const struct ethtool_ops *ops = dev->ethtool_ops;
	u64 *data;
	int ret, test_len;

	if (!ops->self_test || !ops->get_sset_count)
		return -EOPNOTSUPP;

	test_len = ops->get_sset_count(dev, ETH_SS_TEST);
	if (test_len < 0)
		return test_len;
	WARN_ON(test_len == 0);

	if (copy_from_user(&test, useraddr, sizeof(test)))
		return -EFAULT;

	test.len = test_len;
	data = kmalloc(test_len * sizeof(u64), GFP_USER);
	if (!data)
		return -ENOMEM;

	ops->self_test(dev, &test, data);

	ret = -EFAULT;
	if (copy_to_user(useraddr, &test, sizeof(test)))
		goto out;
	useraddr += sizeof(test);
	if (copy_to_user(useraddr, data, test.len * sizeof(u64)))
		goto out;
	ret = 0;

 out:
	kfree(data);
	return ret;
}

static int ethtool_get_strings(struct net_device *dev, void __user *useraddr)
{
	struct ethtool_gstrings gstrings;
	u8 *data;
	int ret;

	if (copy_from_user(&gstrings, useraddr, sizeof(gstrings)))
		return -EFAULT;

	ret = __ethtool_get_sset_count(dev, gstrings.string_set);
	if (ret < 0)
		return ret;

	gstrings.len = ret;

	data = kmalloc(gstrings.len * ETH_GSTRING_LEN, GFP_USER);
	if (!data)
		return -ENOMEM;

	__ethtool_get_strings(dev, gstrings.string_set, data);

	ret = -EFAULT;
	if (copy_to_user(useraddr, &gstrings, sizeof(gstrings)))
		goto out;
	useraddr += sizeof(gstrings);
	if (copy_to_user(useraddr, data, gstrings.len * ETH_GSTRING_LEN))
		goto out;
	ret = 0;

out:
	kfree(data);
	return ret;
}

static int ethtool_phys_id(struct net_device *dev, void __user *useraddr)
{
	struct ethtool_value id;
	static bool busy;
<<<<<<< HEAD
	int rc;

	if (!dev->ethtool_ops->set_phys_id)
=======
	const struct ethtool_ops *ops = dev->ethtool_ops;
	int rc;

	if (!ops->set_phys_id)
>>>>>>> refs/remotes/origin/master
		return -EOPNOTSUPP;

	if (busy)
		return -EBUSY;

	if (copy_from_user(&id, useraddr, sizeof(id)))
		return -EFAULT;

<<<<<<< HEAD
	rc = dev->ethtool_ops->set_phys_id(dev, ETHTOOL_ID_ACTIVE);
=======
	rc = ops->set_phys_id(dev, ETHTOOL_ID_ACTIVE);
>>>>>>> refs/remotes/origin/master
	if (rc < 0)
		return rc;

	/* Drop the RTNL lock while waiting, but prevent reentry or
	 * removal of the device.
	 */
	busy = true;
	dev_hold(dev);
	rtnl_unlock();

	if (rc == 0) {
		/* Driver will handle this itself */
		schedule_timeout_interruptible(
			id.data ? (id.data * HZ) : MAX_SCHEDULE_TIMEOUT);
	} else {
		/* Driver expects to be called at twice the frequency in rc */
		int n = rc * 2, i, interval = HZ / n;

		/* Count down seconds */
		do {
			/* Count down iterations per second */
			i = n;
			do {
				rtnl_lock();
<<<<<<< HEAD
				rc = dev->ethtool_ops->set_phys_id(dev,
=======
				rc = ops->set_phys_id(dev,
>>>>>>> refs/remotes/origin/master
				    (i & 1) ? ETHTOOL_ID_OFF : ETHTOOL_ID_ON);
				rtnl_unlock();
				if (rc)
					break;
				schedule_timeout_interruptible(interval);
			} while (!signal_pending(current) && --i != 0);
		} while (!signal_pending(current) &&
			 (id.data == 0 || --id.data != 0));
	}

	rtnl_lock();
	dev_put(dev);
	busy = false;

<<<<<<< HEAD
	(void)dev->ethtool_ops->set_phys_id(dev, ETHTOOL_ID_INACTIVE);
=======
	(void) ops->set_phys_id(dev, ETHTOOL_ID_INACTIVE);
>>>>>>> refs/remotes/origin/master
	return rc;
}

static int ethtool_get_stats(struct net_device *dev, void __user *useraddr)
{
	struct ethtool_stats stats;
	const struct ethtool_ops *ops = dev->ethtool_ops;
	u64 *data;
	int ret, n_stats;

	if (!ops->get_ethtool_stats || !ops->get_sset_count)
		return -EOPNOTSUPP;

	n_stats = ops->get_sset_count(dev, ETH_SS_STATS);
	if (n_stats < 0)
		return n_stats;
	WARN_ON(n_stats == 0);

	if (copy_from_user(&stats, useraddr, sizeof(stats)))
		return -EFAULT;

	stats.n_stats = n_stats;
	data = kmalloc(n_stats * sizeof(u64), GFP_USER);
	if (!data)
		return -ENOMEM;

	ops->get_ethtool_stats(dev, &stats, data);

	ret = -EFAULT;
	if (copy_to_user(useraddr, &stats, sizeof(stats)))
		goto out;
	useraddr += sizeof(stats);
	if (copy_to_user(useraddr, data, stats.n_stats * sizeof(u64)))
		goto out;
	ret = 0;

 out:
	kfree(data);
	return ret;
}

static int ethtool_get_perm_addr(struct net_device *dev, void __user *useraddr)
{
	struct ethtool_perm_addr epaddr;

	if (copy_from_user(&epaddr, useraddr, sizeof(epaddr)))
		return -EFAULT;

	if (epaddr.size < dev->addr_len)
		return -ETOOSMALL;
	epaddr.size = dev->addr_len;

	if (copy_to_user(useraddr, &epaddr, sizeof(epaddr)))
		return -EFAULT;
	useraddr += sizeof(epaddr);
	if (copy_to_user(useraddr, dev->perm_addr, epaddr.size))
		return -EFAULT;
	return 0;
}

static int ethtool_get_value(struct net_device *dev, char __user *useraddr,
			     u32 cmd, u32 (*actor)(struct net_device *))
{
	struct ethtool_value edata = { .cmd = cmd };

	if (!actor)
		return -EOPNOTSUPP;

	edata.data = actor(dev);

	if (copy_to_user(useraddr, &edata, sizeof(edata)))
		return -EFAULT;
	return 0;
}

static int ethtool_set_value_void(struct net_device *dev, char __user *useraddr,
			     void (*actor)(struct net_device *, u32))
{
	struct ethtool_value edata;

	if (!actor)
		return -EOPNOTSUPP;

	if (copy_from_user(&edata, useraddr, sizeof(edata)))
		return -EFAULT;

	actor(dev, edata.data);
	return 0;
}

static int ethtool_set_value(struct net_device *dev, char __user *useraddr,
			     int (*actor)(struct net_device *, u32))
{
	struct ethtool_value edata;

	if (!actor)
		return -EOPNOTSUPP;

	if (copy_from_user(&edata, useraddr, sizeof(edata)))
		return -EFAULT;

	return actor(dev, edata.data);
}

static noinline_for_stack int ethtool_flash_device(struct net_device *dev,
						   char __user *useraddr)
{
	struct ethtool_flash efl;

	if (copy_from_user(&efl, useraddr, sizeof(efl)))
		return -EFAULT;

	if (!dev->ethtool_ops->flash_device)
		return -EOPNOTSUPP;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	efl.data[ETHTOOL_FLASH_MAX_FILENAME - 1] = 0;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	efl.data[ETHTOOL_FLASH_MAX_FILENAME - 1] = 0;

>>>>>>> refs/remotes/origin/master
	return dev->ethtool_ops->flash_device(dev, &efl);
}

static int ethtool_set_dump(struct net_device *dev,
			void __user *useraddr)
{
	struct ethtool_dump dump;

	if (!dev->ethtool_ops->set_dump)
		return -EOPNOTSUPP;

	if (copy_from_user(&dump, useraddr, sizeof(dump)))
		return -EFAULT;

	return dev->ethtool_ops->set_dump(dev, &dump);
}

static int ethtool_get_dump_flag(struct net_device *dev,
				void __user *useraddr)
{
	int ret;
	struct ethtool_dump dump;
	const struct ethtool_ops *ops = dev->ethtool_ops;

<<<<<<< HEAD
	if (!dev->ethtool_ops->get_dump_flag)
=======
	if (!ops->get_dump_flag)
>>>>>>> refs/remotes/origin/master
		return -EOPNOTSUPP;

	if (copy_from_user(&dump, useraddr, sizeof(dump)))
		return -EFAULT;

	ret = ops->get_dump_flag(dev, &dump);
	if (ret)
		return ret;

	if (copy_to_user(useraddr, &dump, sizeof(dump)))
		return -EFAULT;
	return 0;
}

static int ethtool_get_dump_data(struct net_device *dev,
				void __user *useraddr)
{
	int ret;
	__u32 len;
	struct ethtool_dump dump, tmp;
	const struct ethtool_ops *ops = dev->ethtool_ops;
	void *data = NULL;

<<<<<<< HEAD
	if (!dev->ethtool_ops->get_dump_data ||
		!dev->ethtool_ops->get_dump_flag)
=======
	if (!ops->get_dump_data || !ops->get_dump_flag)
>>>>>>> refs/remotes/origin/master
		return -EOPNOTSUPP;

	if (copy_from_user(&dump, useraddr, sizeof(dump)))
		return -EFAULT;

	memset(&tmp, 0, sizeof(tmp));
	tmp.cmd = ETHTOOL_GET_DUMP_FLAG;
	ret = ops->get_dump_flag(dev, &tmp);
	if (ret)
		return ret;

<<<<<<< HEAD
	len = (tmp.len > dump.len) ? dump.len : tmp.len;
	if (!len)
		return -EFAULT;

=======
	len = min(tmp.len, dump.len);
	if (!len)
		return -EFAULT;

	/* Don't ever let the driver think there's more space available
	 * than it requested with .get_dump_flag().
	 */
	dump.len = len;

	/* Always allocate enough space to hold the whole thing so that the
	 * driver does not need to check the length and bother with partial
	 * dumping.
	 */
>>>>>>> refs/remotes/origin/master
	data = vzalloc(tmp.len);
	if (!data)
		return -ENOMEM;
	ret = ops->get_dump_data(dev, &dump, data);
	if (ret)
		goto out;

<<<<<<< HEAD
=======
	/* There are two sane possibilities:
	 * 1. The driver's .get_dump_data() does not touch dump.len.
	 * 2. Or it may set dump.len to how much it really writes, which
	 *    should be tmp.len (or len if it can do a partial dump).
	 * In any case respond to userspace with the actual length of data
	 * it's receiving.
	 */
	WARN_ON(dump.len != len && dump.len != tmp.len);
	dump.len = len;

>>>>>>> refs/remotes/origin/master
	if (copy_to_user(useraddr, &dump, sizeof(dump))) {
		ret = -EFAULT;
		goto out;
	}
	useraddr += offsetof(struct ethtool_dump, data);
	if (copy_to_user(useraddr, data, len))
		ret = -EFAULT;
out:
	vfree(data);
	return ret;
}

<<<<<<< HEAD
/* The main entry point in this file.  Called from net/core/dev.c */
=======
static int ethtool_get_ts_info(struct net_device *dev, void __user *useraddr)
{
	int err = 0;
	struct ethtool_ts_info info;
	const struct ethtool_ops *ops = dev->ethtool_ops;
	struct phy_device *phydev = dev->phydev;

	memset(&info, 0, sizeof(info));
	info.cmd = ETHTOOL_GET_TS_INFO;

	if (phydev && phydev->drv && phydev->drv->ts_info) {
		err = phydev->drv->ts_info(phydev, &info);
	} else if (ops->get_ts_info) {
		err = ops->get_ts_info(dev, &info);
	} else {
		info.so_timestamping =
			SOF_TIMESTAMPING_RX_SOFTWARE |
			SOF_TIMESTAMPING_SOFTWARE;
		info.phc_index = -1;
	}

	if (err)
		return err;

	if (copy_to_user(useraddr, &info, sizeof(info)))
		err = -EFAULT;

	return err;
}

static int ethtool_get_module_info(struct net_device *dev,
				   void __user *useraddr)
{
	int ret;
	struct ethtool_modinfo modinfo;
	const struct ethtool_ops *ops = dev->ethtool_ops;

	if (!ops->get_module_info)
		return -EOPNOTSUPP;

	if (copy_from_user(&modinfo, useraddr, sizeof(modinfo)))
		return -EFAULT;

	ret = ops->get_module_info(dev, &modinfo);
	if (ret)
		return ret;

	if (copy_to_user(useraddr, &modinfo, sizeof(modinfo)))
		return -EFAULT;

	return 0;
}

static int ethtool_get_module_eeprom(struct net_device *dev,
				     void __user *useraddr)
{
	int ret;
	struct ethtool_modinfo modinfo;
	const struct ethtool_ops *ops = dev->ethtool_ops;

	if (!ops->get_module_info || !ops->get_module_eeprom)
		return -EOPNOTSUPP;

	ret = ops->get_module_info(dev, &modinfo);
	if (ret)
		return ret;

	return ethtool_get_any_eeprom(dev, useraddr, ops->get_module_eeprom,
				      modinfo.eeprom_len);
}

/* The main entry point in this file.  Called from net/core/dev_ioctl.c */
>>>>>>> refs/remotes/origin/master

int dev_ethtool(struct net *net, struct ifreq *ifr)
{
	struct net_device *dev = __dev_get_by_name(net, ifr->ifr_name);
	void __user *useraddr = ifr->ifr_data;
	u32 ethcmd;
	int rc;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	u32 old_features;
=======
	netdev_features_t old_features;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	netdev_features_t old_features;
>>>>>>> refs/remotes/origin/master
=======
	netdev_features_t old_features;
>>>>>>> refs/remotes/origin/cm-11.0

	if (!dev || !netif_device_present(dev))
		return -ENODEV;

	if (copy_from_user(&ethcmd, useraddr, sizeof(ethcmd)))
		return -EFAULT;

<<<<<<< HEAD
	if (!dev->ethtool_ops) {
		/* ETHTOOL_GDRVINFO does not require any driver support.
		 * It is also unprivileged and does not change anything,
		 * so we can take a shortcut to it. */
		if (ethcmd == ETHTOOL_GDRVINFO)
			return ethtool_get_drvinfo(dev, useraddr);
		else
			return -EOPNOTSUPP;
	}

=======
>>>>>>> refs/remotes/origin/master
	/* Allow some commands to be done by anyone */
	switch (ethcmd) {
	case ETHTOOL_GSET:
	case ETHTOOL_GDRVINFO:
	case ETHTOOL_GMSGLVL:
<<<<<<< HEAD
=======
	case ETHTOOL_GLINK:
>>>>>>> refs/remotes/origin/master
	case ETHTOOL_GCOALESCE:
	case ETHTOOL_GRINGPARAM:
	case ETHTOOL_GPAUSEPARAM:
	case ETHTOOL_GRXCSUM:
	case ETHTOOL_GTXCSUM:
	case ETHTOOL_GSG:
	case ETHTOOL_GSSET_INFO:
	case ETHTOOL_GSTRINGS:
<<<<<<< HEAD
=======
	case ETHTOOL_GSTATS:
>>>>>>> refs/remotes/origin/master
	case ETHTOOL_GTSO:
	case ETHTOOL_GPERMADDR:
	case ETHTOOL_GUFO:
	case ETHTOOL_GGSO:
	case ETHTOOL_GGRO:
	case ETHTOOL_GFLAGS:
	case ETHTOOL_GPFLAGS:
	case ETHTOOL_GRXFH:
	case ETHTOOL_GRXRINGS:
	case ETHTOOL_GRXCLSRLCNT:
	case ETHTOOL_GRXCLSRULE:
	case ETHTOOL_GRXCLSRLALL:
<<<<<<< HEAD
	case ETHTOOL_GFEATURES:
		break;
	default:
		if (!capable(CAP_NET_ADMIN))
=======
	case ETHTOOL_GRXFHINDIR:
	case ETHTOOL_GFEATURES:
	case ETHTOOL_GCHANNELS:
	case ETHTOOL_GET_TS_INFO:
	case ETHTOOL_GEEE:
		break;
	default:
		if (!ns_capable(net->user_ns, CAP_NET_ADMIN))
>>>>>>> refs/remotes/origin/master
			return -EPERM;
	}

	if (dev->ethtool_ops->begin) {
		rc = dev->ethtool_ops->begin(dev);
		if (rc  < 0)
			return rc;
	}
	old_features = dev->features;

	switch (ethcmd) {
	case ETHTOOL_GSET:
		rc = ethtool_get_settings(dev, useraddr);
		break;
	case ETHTOOL_SSET:
		rc = ethtool_set_settings(dev, useraddr);
		break;
	case ETHTOOL_GDRVINFO:
		rc = ethtool_get_drvinfo(dev, useraddr);
		break;
	case ETHTOOL_GREGS:
		rc = ethtool_get_regs(dev, useraddr);
		break;
	case ETHTOOL_GWOL:
		rc = ethtool_get_wol(dev, useraddr);
		break;
	case ETHTOOL_SWOL:
		rc = ethtool_set_wol(dev, useraddr);
		break;
	case ETHTOOL_GMSGLVL:
		rc = ethtool_get_value(dev, useraddr, ethcmd,
				       dev->ethtool_ops->get_msglevel);
		break;
	case ETHTOOL_SMSGLVL:
		rc = ethtool_set_value_void(dev, useraddr,
				       dev->ethtool_ops->set_msglevel);
		break;
<<<<<<< HEAD
=======
	case ETHTOOL_GEEE:
		rc = ethtool_get_eee(dev, useraddr);
		break;
	case ETHTOOL_SEEE:
		rc = ethtool_set_eee(dev, useraddr);
		break;
>>>>>>> refs/remotes/origin/master
	case ETHTOOL_NWAY_RST:
		rc = ethtool_nway_reset(dev);
		break;
	case ETHTOOL_GLINK:
		rc = ethtool_get_link(dev, useraddr);
		break;
	case ETHTOOL_GEEPROM:
		rc = ethtool_get_eeprom(dev, useraddr);
		break;
	case ETHTOOL_SEEPROM:
		rc = ethtool_set_eeprom(dev, useraddr);
		break;
	case ETHTOOL_GCOALESCE:
		rc = ethtool_get_coalesce(dev, useraddr);
		break;
	case ETHTOOL_SCOALESCE:
		rc = ethtool_set_coalesce(dev, useraddr);
		break;
	case ETHTOOL_GRINGPARAM:
		rc = ethtool_get_ringparam(dev, useraddr);
		break;
	case ETHTOOL_SRINGPARAM:
		rc = ethtool_set_ringparam(dev, useraddr);
		break;
	case ETHTOOL_GPAUSEPARAM:
		rc = ethtool_get_pauseparam(dev, useraddr);
		break;
	case ETHTOOL_SPAUSEPARAM:
		rc = ethtool_set_pauseparam(dev, useraddr);
		break;
	case ETHTOOL_TEST:
		rc = ethtool_self_test(dev, useraddr);
		break;
	case ETHTOOL_GSTRINGS:
		rc = ethtool_get_strings(dev, useraddr);
		break;
	case ETHTOOL_PHYS_ID:
		rc = ethtool_phys_id(dev, useraddr);
		break;
	case ETHTOOL_GSTATS:
		rc = ethtool_get_stats(dev, useraddr);
		break;
	case ETHTOOL_GPERMADDR:
		rc = ethtool_get_perm_addr(dev, useraddr);
		break;
	case ETHTOOL_GFLAGS:
		rc = ethtool_get_value(dev, useraddr, ethcmd,
<<<<<<< HEAD
<<<<<<< HEAD
				       (dev->ethtool_ops->get_flags ?
					dev->ethtool_ops->get_flags :
					ethtool_op_get_flags));
=======
					__ethtool_get_flags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
					__ethtool_get_flags);
>>>>>>> refs/remotes/origin/master
		break;
	case ETHTOOL_SFLAGS:
		rc = ethtool_set_value(dev, useraddr, __ethtool_set_flags);
		break;
	case ETHTOOL_GPFLAGS:
		rc = ethtool_get_value(dev, useraddr, ethcmd,
				       dev->ethtool_ops->get_priv_flags);
		break;
	case ETHTOOL_SPFLAGS:
		rc = ethtool_set_value(dev, useraddr,
				       dev->ethtool_ops->set_priv_flags);
		break;
	case ETHTOOL_GRXFH:
	case ETHTOOL_GRXRINGS:
	case ETHTOOL_GRXCLSRLCNT:
	case ETHTOOL_GRXCLSRULE:
	case ETHTOOL_GRXCLSRLALL:
		rc = ethtool_get_rxnfc(dev, ethcmd, useraddr);
		break;
	case ETHTOOL_SRXFH:
	case ETHTOOL_SRXCLSRLDEL:
	case ETHTOOL_SRXCLSRLINS:
		rc = ethtool_set_rxnfc(dev, ethcmd, useraddr);
		break;
	case ETHTOOL_FLASHDEV:
		rc = ethtool_flash_device(dev, useraddr);
		break;
	case ETHTOOL_RESET:
		rc = ethtool_reset(dev, useraddr);
		break;
<<<<<<< HEAD
<<<<<<< HEAD
	case ETHTOOL_SRXNTUPLE:
		rc = ethtool_set_rx_ntuple(dev, useraddr);
		break;
	case ETHTOOL_GRXNTUPLE:
		rc = ethtool_get_rx_ntuple(dev, useraddr);
		break;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	case ETHTOOL_GSSET_INFO:
		rc = ethtool_get_sset_info(dev, useraddr);
		break;
	case ETHTOOL_GRXFHINDIR:
		rc = ethtool_get_rxfh_indir(dev, useraddr);
		break;
	case ETHTOOL_SRXFHINDIR:
		rc = ethtool_set_rxfh_indir(dev, useraddr);
		break;
	case ETHTOOL_GFEATURES:
		rc = ethtool_get_features(dev, useraddr);
		break;
	case ETHTOOL_SFEATURES:
		rc = ethtool_set_features(dev, useraddr);
		break;
	case ETHTOOL_GTXCSUM:
	case ETHTOOL_GRXCSUM:
	case ETHTOOL_GSG:
	case ETHTOOL_GTSO:
	case ETHTOOL_GUFO:
	case ETHTOOL_GGSO:
	case ETHTOOL_GGRO:
		rc = ethtool_get_one_feature(dev, useraddr, ethcmd);
		break;
	case ETHTOOL_STXCSUM:
	case ETHTOOL_SRXCSUM:
	case ETHTOOL_SSG:
	case ETHTOOL_STSO:
	case ETHTOOL_SUFO:
	case ETHTOOL_SGSO:
	case ETHTOOL_SGRO:
		rc = ethtool_set_one_feature(dev, useraddr, ethcmd);
		break;
	case ETHTOOL_GCHANNELS:
		rc = ethtool_get_channels(dev, useraddr);
		break;
	case ETHTOOL_SCHANNELS:
		rc = ethtool_set_channels(dev, useraddr);
		break;
	case ETHTOOL_SET_DUMP:
		rc = ethtool_set_dump(dev, useraddr);
		break;
	case ETHTOOL_GET_DUMP_FLAG:
		rc = ethtool_get_dump_flag(dev, useraddr);
		break;
	case ETHTOOL_GET_DUMP_DATA:
		rc = ethtool_get_dump_data(dev, useraddr);
		break;
<<<<<<< HEAD
=======
	case ETHTOOL_GET_TS_INFO:
		rc = ethtool_get_ts_info(dev, useraddr);
		break;
	case ETHTOOL_GMODULEINFO:
		rc = ethtool_get_module_info(dev, useraddr);
		break;
	case ETHTOOL_GMODULEEEPROM:
		rc = ethtool_get_module_eeprom(dev, useraddr);
		break;
>>>>>>> refs/remotes/origin/master
	default:
		rc = -EOPNOTSUPP;
	}

	if (dev->ethtool_ops->complete)
		dev->ethtool_ops->complete(dev);

	if (old_features != dev->features)
		netdev_features_change(dev);

	return rc;
}
