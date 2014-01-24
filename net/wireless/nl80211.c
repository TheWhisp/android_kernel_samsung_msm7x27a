/*
 * This is the new netlink-based wireless configuration interface.
 *
 * Copyright 2006-2010	Johannes Berg <johannes@sipsolutions.net>
 */

#include <linux/if.h>
#include <linux/module.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/if_ether.h>
#include <linux/ieee80211.h>
#include <linux/nl80211.h>
#include <linux/rtnetlink.h>
#include <linux/netlink.h>
#include <linux/etherdevice.h>
#include <net/net_namespace.h>
#include <net/genetlink.h>
#include <net/cfg80211.h>
#include <net/sock.h>
<<<<<<< HEAD
#include "core.h"
#include "nl80211.h"
#include "reg.h"

static bool nl80211_valid_auth_type(enum nl80211_auth_type auth_type);
=======
#include <net/inet_connection_sock.h>
#include "core.h"
#include "nl80211.h"
#include "reg.h"
#include "rdev-ops.h"

>>>>>>> refs/remotes/origin/master
static int nl80211_crypto_settings(struct cfg80211_registered_device *rdev,
				   struct genl_info *info,
				   struct cfg80211_crypto_settings *settings,
				   int cipher_limit);

<<<<<<< HEAD
static int nl80211_pre_doit(struct genl_ops *ops, struct sk_buff *skb,
			    struct genl_info *info);
static void nl80211_post_doit(struct genl_ops *ops, struct sk_buff *skb,
=======
static int nl80211_pre_doit(const struct genl_ops *ops, struct sk_buff *skb,
			    struct genl_info *info);
static void nl80211_post_doit(const struct genl_ops *ops, struct sk_buff *skb,
>>>>>>> refs/remotes/origin/master
			      struct genl_info *info);

/* the netlink family */
static struct genl_family nl80211_fam = {
<<<<<<< HEAD
	.id = GENL_ID_GENERATE,	/* don't bother with a hardcoded ID */
	.name = "nl80211",	/* have users key off the name instead */
	.hdrsize = 0,		/* no private header */
	.version = 1,		/* no particular meaning now */
=======
	.id = GENL_ID_GENERATE,		/* don't bother with a hardcoded ID */
	.name = NL80211_GENL_NAME,	/* have users key off the name instead */
	.hdrsize = 0,			/* no private header */
	.version = 1,			/* no particular meaning now */
>>>>>>> refs/remotes/origin/master
	.maxattr = NL80211_ATTR_MAX,
	.netnsok = true,
	.pre_doit = nl80211_pre_doit,
	.post_doit = nl80211_post_doit,
};

<<<<<<< HEAD
/* internal helper: get rdev and dev */
<<<<<<< HEAD
static int get_rdev_dev_by_info_ifindex(struct genl_info *info,
				       struct cfg80211_registered_device **rdev,
				       struct net_device **dev)
{
	struct nlattr **attrs = info->attrs;
=======
static int get_rdev_dev_by_ifindex(struct net *netns, struct nlattr **attrs,
				   struct cfg80211_registered_device **rdev,
				   struct net_device **dev)
{
>>>>>>> refs/remotes/origin/cm-10.0
	int ifindex;

	if (!attrs[NL80211_ATTR_IFINDEX])
		return -EINVAL;

	ifindex = nla_get_u32(attrs[NL80211_ATTR_IFINDEX]);
<<<<<<< HEAD
	*dev = dev_get_by_index(genl_info_net(info), ifindex);
	if (!*dev)
		return -ENODEV;

	*rdev = cfg80211_get_dev_from_ifindex(genl_info_net(info), ifindex);
=======
	*dev = dev_get_by_index(netns, ifindex);
	if (!*dev)
		return -ENODEV;

	*rdev = cfg80211_get_dev_from_ifindex(netns, ifindex);
>>>>>>> refs/remotes/origin/cm-10.0
	if (IS_ERR(*rdev)) {
		dev_put(*dev);
		return PTR_ERR(*rdev);
	}

	return 0;
=======
/* multicast groups */
enum nl80211_multicast_groups {
	NL80211_MCGRP_CONFIG,
	NL80211_MCGRP_SCAN,
	NL80211_MCGRP_REGULATORY,
	NL80211_MCGRP_MLME,
	NL80211_MCGRP_TESTMODE /* keep last - ifdef! */
};

static const struct genl_multicast_group nl80211_mcgrps[] = {
	[NL80211_MCGRP_CONFIG] = { .name = "config", },
	[NL80211_MCGRP_SCAN] = { .name = "scan", },
	[NL80211_MCGRP_REGULATORY] = { .name = "regulatory", },
	[NL80211_MCGRP_MLME] = { .name = "mlme", },
#ifdef CONFIG_NL80211_TESTMODE
	[NL80211_MCGRP_TESTMODE] = { .name = "testmode", }
#endif
};

/* returns ERR_PTR values */
static struct wireless_dev *
__cfg80211_wdev_from_attrs(struct net *netns, struct nlattr **attrs)
{
	struct cfg80211_registered_device *rdev;
	struct wireless_dev *result = NULL;
	bool have_ifidx = attrs[NL80211_ATTR_IFINDEX];
	bool have_wdev_id = attrs[NL80211_ATTR_WDEV];
	u64 wdev_id;
	int wiphy_idx = -1;
	int ifidx = -1;

	ASSERT_RTNL();

	if (!have_ifidx && !have_wdev_id)
		return ERR_PTR(-EINVAL);

	if (have_ifidx)
		ifidx = nla_get_u32(attrs[NL80211_ATTR_IFINDEX]);
	if (have_wdev_id) {
		wdev_id = nla_get_u64(attrs[NL80211_ATTR_WDEV]);
		wiphy_idx = wdev_id >> 32;
	}

	list_for_each_entry(rdev, &cfg80211_rdev_list, list) {
		struct wireless_dev *wdev;

		if (wiphy_net(&rdev->wiphy) != netns)
			continue;

		if (have_wdev_id && rdev->wiphy_idx != wiphy_idx)
			continue;

		list_for_each_entry(wdev, &rdev->wdev_list, list) {
			if (have_ifidx && wdev->netdev &&
			    wdev->netdev->ifindex == ifidx) {
				result = wdev;
				break;
			}
			if (have_wdev_id && wdev->identifier == (u32)wdev_id) {
				result = wdev;
				break;
			}
		}

		if (result)
			break;
	}

	if (result)
		return result;
	return ERR_PTR(-ENODEV);
}

static struct cfg80211_registered_device *
__cfg80211_rdev_from_attrs(struct net *netns, struct nlattr **attrs)
{
	struct cfg80211_registered_device *rdev = NULL, *tmp;
	struct net_device *netdev;

	ASSERT_RTNL();

	if (!attrs[NL80211_ATTR_WIPHY] &&
	    !attrs[NL80211_ATTR_IFINDEX] &&
	    !attrs[NL80211_ATTR_WDEV])
		return ERR_PTR(-EINVAL);

	if (attrs[NL80211_ATTR_WIPHY])
		rdev = cfg80211_rdev_by_wiphy_idx(
				nla_get_u32(attrs[NL80211_ATTR_WIPHY]));

	if (attrs[NL80211_ATTR_WDEV]) {
		u64 wdev_id = nla_get_u64(attrs[NL80211_ATTR_WDEV]);
		struct wireless_dev *wdev;
		bool found = false;

		tmp = cfg80211_rdev_by_wiphy_idx(wdev_id >> 32);
		if (tmp) {
			/* make sure wdev exists */
			list_for_each_entry(wdev, &tmp->wdev_list, list) {
				if (wdev->identifier != (u32)wdev_id)
					continue;
				found = true;
				break;
			}

			if (!found)
				tmp = NULL;

			if (rdev && tmp != rdev)
				return ERR_PTR(-EINVAL);
			rdev = tmp;
		}
	}

	if (attrs[NL80211_ATTR_IFINDEX]) {
		int ifindex = nla_get_u32(attrs[NL80211_ATTR_IFINDEX]);
		netdev = dev_get_by_index(netns, ifindex);
		if (netdev) {
			if (netdev->ieee80211_ptr)
				tmp = wiphy_to_dev(
						netdev->ieee80211_ptr->wiphy);
			else
				tmp = NULL;

			dev_put(netdev);

			/* not wireless device -- return error */
			if (!tmp)
				return ERR_PTR(-EINVAL);

			/* mismatch -- return error */
			if (rdev && tmp != rdev)
				return ERR_PTR(-EINVAL);

			rdev = tmp;
		}
	}

	if (!rdev)
		return ERR_PTR(-ENODEV);

	if (netns != wiphy_net(&rdev->wiphy))
		return ERR_PTR(-ENODEV);

	return rdev;
}

/*
 * This function returns a pointer to the driver
 * that the genl_info item that is passed refers to.
 *
 * The result of this can be a PTR_ERR and hence must
 * be checked with IS_ERR() for errors.
 */
static struct cfg80211_registered_device *
cfg80211_get_dev_from_info(struct net *netns, struct genl_info *info)
{
	return __cfg80211_rdev_from_attrs(netns, info->attrs);
>>>>>>> refs/remotes/origin/master
}

/* policy for the attributes */
static const struct nla_policy nl80211_policy[NL80211_ATTR_MAX+1] = {
	[NL80211_ATTR_WIPHY] = { .type = NLA_U32 },
	[NL80211_ATTR_WIPHY_NAME] = { .type = NLA_NUL_STRING,
				      .len = 20-1 },
	[NL80211_ATTR_WIPHY_TXQ_PARAMS] = { .type = NLA_NESTED },
<<<<<<< HEAD
	[NL80211_ATTR_WIPHY_FREQ] = { .type = NLA_U32 },
	[NL80211_ATTR_WIPHY_CHANNEL_TYPE] = { .type = NLA_U32 },
=======

	[NL80211_ATTR_WIPHY_FREQ] = { .type = NLA_U32 },
	[NL80211_ATTR_WIPHY_CHANNEL_TYPE] = { .type = NLA_U32 },
	[NL80211_ATTR_CHANNEL_WIDTH] = { .type = NLA_U32 },
	[NL80211_ATTR_CENTER_FREQ1] = { .type = NLA_U32 },
	[NL80211_ATTR_CENTER_FREQ2] = { .type = NLA_U32 },

>>>>>>> refs/remotes/origin/master
	[NL80211_ATTR_WIPHY_RETRY_SHORT] = { .type = NLA_U8 },
	[NL80211_ATTR_WIPHY_RETRY_LONG] = { .type = NLA_U8 },
	[NL80211_ATTR_WIPHY_FRAG_THRESHOLD] = { .type = NLA_U32 },
	[NL80211_ATTR_WIPHY_RTS_THRESHOLD] = { .type = NLA_U32 },
	[NL80211_ATTR_WIPHY_COVERAGE_CLASS] = { .type = NLA_U8 },

	[NL80211_ATTR_IFTYPE] = { .type = NLA_U32 },
	[NL80211_ATTR_IFINDEX] = { .type = NLA_U32 },
	[NL80211_ATTR_IFNAME] = { .type = NLA_NUL_STRING, .len = IFNAMSIZ-1 },

	[NL80211_ATTR_MAC] = { .len = ETH_ALEN },
	[NL80211_ATTR_PREV_BSSID] = { .len = ETH_ALEN },

	[NL80211_ATTR_KEY] = { .type = NLA_NESTED, },
	[NL80211_ATTR_KEY_DATA] = { .type = NLA_BINARY,
				    .len = WLAN_MAX_KEY_LEN },
	[NL80211_ATTR_KEY_IDX] = { .type = NLA_U8 },
	[NL80211_ATTR_KEY_CIPHER] = { .type = NLA_U32 },
	[NL80211_ATTR_KEY_DEFAULT] = { .type = NLA_FLAG },
	[NL80211_ATTR_KEY_SEQ] = { .type = NLA_BINARY, .len = 16 },
	[NL80211_ATTR_KEY_TYPE] = { .type = NLA_U32 },

	[NL80211_ATTR_BEACON_INTERVAL] = { .type = NLA_U32 },
	[NL80211_ATTR_DTIM_PERIOD] = { .type = NLA_U32 },
	[NL80211_ATTR_BEACON_HEAD] = { .type = NLA_BINARY,
				       .len = IEEE80211_MAX_DATA_LEN },
	[NL80211_ATTR_BEACON_TAIL] = { .type = NLA_BINARY,
				       .len = IEEE80211_MAX_DATA_LEN },
	[NL80211_ATTR_STA_AID] = { .type = NLA_U16 },
	[NL80211_ATTR_STA_FLAGS] = { .type = NLA_NESTED },
	[NL80211_ATTR_STA_LISTEN_INTERVAL] = { .type = NLA_U16 },
	[NL80211_ATTR_STA_SUPPORTED_RATES] = { .type = NLA_BINARY,
					       .len = NL80211_MAX_SUPP_RATES },
	[NL80211_ATTR_STA_PLINK_ACTION] = { .type = NLA_U8 },
	[NL80211_ATTR_STA_VLAN] = { .type = NLA_U32 },
	[NL80211_ATTR_MNTR_FLAGS] = { /* NLA_NESTED can't be empty */ },
	[NL80211_ATTR_MESH_ID] = { .type = NLA_BINARY,
<<<<<<< HEAD
				.len = IEEE80211_MAX_MESH_ID_LEN },
=======
				   .len = IEEE80211_MAX_MESH_ID_LEN },
>>>>>>> refs/remotes/origin/master
	[NL80211_ATTR_MPATH_NEXT_HOP] = { .type = NLA_U32 },

	[NL80211_ATTR_REG_ALPHA2] = { .type = NLA_STRING, .len = 2 },
	[NL80211_ATTR_REG_RULES] = { .type = NLA_NESTED },

	[NL80211_ATTR_BSS_CTS_PROT] = { .type = NLA_U8 },
	[NL80211_ATTR_BSS_SHORT_PREAMBLE] = { .type = NLA_U8 },
	[NL80211_ATTR_BSS_SHORT_SLOT_TIME] = { .type = NLA_U8 },
	[NL80211_ATTR_BSS_BASIC_RATES] = { .type = NLA_BINARY,
					   .len = NL80211_MAX_SUPP_RATES },
	[NL80211_ATTR_BSS_HT_OPMODE] = { .type = NLA_U16 },

	[NL80211_ATTR_MESH_CONFIG] = { .type = NLA_NESTED },
	[NL80211_ATTR_SUPPORT_MESH_AUTH] = { .type = NLA_FLAG },

	[NL80211_ATTR_HT_CAPABILITY] = { .len = NL80211_HT_CAPABILITY_LEN },

	[NL80211_ATTR_MGMT_SUBTYPE] = { .type = NLA_U8 },
	[NL80211_ATTR_IE] = { .type = NLA_BINARY,
			      .len = IEEE80211_MAX_DATA_LEN },
	[NL80211_ATTR_SCAN_FREQUENCIES] = { .type = NLA_NESTED },
	[NL80211_ATTR_SCAN_SSIDS] = { .type = NLA_NESTED },

	[NL80211_ATTR_SSID] = { .type = NLA_BINARY,
				.len = IEEE80211_MAX_SSID_LEN },
	[NL80211_ATTR_AUTH_TYPE] = { .type = NLA_U32 },
	[NL80211_ATTR_REASON_CODE] = { .type = NLA_U16 },
	[NL80211_ATTR_FREQ_FIXED] = { .type = NLA_FLAG },
	[NL80211_ATTR_TIMED_OUT] = { .type = NLA_FLAG },
	[NL80211_ATTR_USE_MFP] = { .type = NLA_U32 },
	[NL80211_ATTR_STA_FLAGS2] = {
		.len = sizeof(struct nl80211_sta_flag_update),
	},
	[NL80211_ATTR_CONTROL_PORT] = { .type = NLA_FLAG },
	[NL80211_ATTR_CONTROL_PORT_ETHERTYPE] = { .type = NLA_U16 },
	[NL80211_ATTR_CONTROL_PORT_NO_ENCRYPT] = { .type = NLA_FLAG },
	[NL80211_ATTR_PRIVACY] = { .type = NLA_FLAG },
	[NL80211_ATTR_CIPHER_SUITE_GROUP] = { .type = NLA_U32 },
	[NL80211_ATTR_WPA_VERSIONS] = { .type = NLA_U32 },
	[NL80211_ATTR_PID] = { .type = NLA_U32 },
	[NL80211_ATTR_4ADDR] = { .type = NLA_U8 },
	[NL80211_ATTR_PMKID] = { .type = NLA_BINARY,
				 .len = WLAN_PMKID_LEN },
	[NL80211_ATTR_DURATION] = { .type = NLA_U32 },
	[NL80211_ATTR_COOKIE] = { .type = NLA_U64 },
	[NL80211_ATTR_TX_RATES] = { .type = NLA_NESTED },
	[NL80211_ATTR_FRAME] = { .type = NLA_BINARY,
				 .len = IEEE80211_MAX_DATA_LEN },
	[NL80211_ATTR_FRAME_MATCH] = { .type = NLA_BINARY, },
	[NL80211_ATTR_PS_STATE] = { .type = NLA_U32 },
	[NL80211_ATTR_CQM] = { .type = NLA_NESTED, },
	[NL80211_ATTR_LOCAL_STATE_CHANGE] = { .type = NLA_FLAG },
	[NL80211_ATTR_AP_ISOLATE] = { .type = NLA_U8 },
	[NL80211_ATTR_WIPHY_TX_POWER_SETTING] = { .type = NLA_U32 },
	[NL80211_ATTR_WIPHY_TX_POWER_LEVEL] = { .type = NLA_U32 },
	[NL80211_ATTR_FRAME_TYPE] = { .type = NLA_U16 },
	[NL80211_ATTR_WIPHY_ANTENNA_TX] = { .type = NLA_U32 },
	[NL80211_ATTR_WIPHY_ANTENNA_RX] = { .type = NLA_U32 },
	[NL80211_ATTR_MCAST_RATE] = { .type = NLA_U32 },
	[NL80211_ATTR_OFFCHANNEL_TX_OK] = { .type = NLA_FLAG },
	[NL80211_ATTR_KEY_DEFAULT_TYPES] = { .type = NLA_NESTED },
	[NL80211_ATTR_WOWLAN_TRIGGERS] = { .type = NLA_NESTED },
	[NL80211_ATTR_STA_PLINK_STATE] = { .type = NLA_U8 },
	[NL80211_ATTR_SCHED_SCAN_INTERVAL] = { .type = NLA_U32 },
	[NL80211_ATTR_REKEY_DATA] = { .type = NLA_NESTED },
	[NL80211_ATTR_SCAN_SUPP_RATES] = { .type = NLA_NESTED },
	[NL80211_ATTR_HIDDEN_SSID] = { .type = NLA_U32 },
	[NL80211_ATTR_IE_PROBE_RESP] = { .type = NLA_BINARY,
					 .len = IEEE80211_MAX_DATA_LEN },
	[NL80211_ATTR_IE_ASSOC_RESP] = { .type = NLA_BINARY,
					 .len = IEEE80211_MAX_DATA_LEN },
<<<<<<< HEAD
<<<<<<< HEAD
	[NL80211_ATTR_TX_NO_CCK_RATE] = { .type = NLA_FLAG },
	[NL80211_ATTR_ROAM_SUPPORT] = { .type = NLA_FLAG },
	[NL80211_ATTR_SCHED_SCAN_MATCH] = { .type = NLA_NESTED },
=======
=======
>>>>>>> refs/remotes/origin/master
	[NL80211_ATTR_ROAM_SUPPORT] = { .type = NLA_FLAG },
	[NL80211_ATTR_SCHED_SCAN_MATCH] = { .type = NLA_NESTED },
	[NL80211_ATTR_TX_NO_CCK_RATE] = { .type = NLA_FLAG },
	[NL80211_ATTR_TDLS_ACTION] = { .type = NLA_U8 },
	[NL80211_ATTR_TDLS_DIALOG_TOKEN] = { .type = NLA_U8 },
	[NL80211_ATTR_TDLS_OPERATION] = { .type = NLA_U8 },
	[NL80211_ATTR_TDLS_SUPPORT] = { .type = NLA_FLAG },
	[NL80211_ATTR_TDLS_EXTERNAL_SETUP] = { .type = NLA_FLAG },
	[NL80211_ATTR_DONT_WAIT_FOR_ACK] = { .type = NLA_FLAG },
	[NL80211_ATTR_PROBE_RESP] = { .type = NLA_BINARY,
				      .len = IEEE80211_MAX_DATA_LEN },
	[NL80211_ATTR_DFS_REGION] = { .type = NLA_U8 },
	[NL80211_ATTR_DISABLE_HT] = { .type = NLA_FLAG },
	[NL80211_ATTR_HT_CAPABILITY_MASK] = {
		.len = NL80211_HT_CAPABILITY_LEN
	},
	[NL80211_ATTR_NOACK_MAP] = { .type = NLA_U16 },
	[NL80211_ATTR_INACTIVITY_TIMEOUT] = { .type = NLA_U16 },
	[NL80211_ATTR_BG_SCAN_PERIOD] = { .type = NLA_U16 },
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
	[NL80211_ATTR_WDEV] = { .type = NLA_U64 },
	[NL80211_ATTR_USER_REG_HINT_TYPE] = { .type = NLA_U32 },
	[NL80211_ATTR_SAE_DATA] = { .type = NLA_BINARY, },
	[NL80211_ATTR_VHT_CAPABILITY] = { .len = NL80211_VHT_CAPABILITY_LEN },
	[NL80211_ATTR_SCAN_FLAGS] = { .type = NLA_U32 },
	[NL80211_ATTR_P2P_CTWINDOW] = { .type = NLA_U8 },
	[NL80211_ATTR_P2P_OPPPS] = { .type = NLA_U8 },
	[NL80211_ATTR_ACL_POLICY] = {. type = NLA_U32 },
	[NL80211_ATTR_MAC_ADDRS] = { .type = NLA_NESTED },
	[NL80211_ATTR_STA_CAPABILITY] = { .type = NLA_U16 },
	[NL80211_ATTR_STA_EXT_CAPABILITY] = { .type = NLA_BINARY, },
	[NL80211_ATTR_SPLIT_WIPHY_DUMP] = { .type = NLA_FLAG, },
	[NL80211_ATTR_DISABLE_VHT] = { .type = NLA_FLAG },
	[NL80211_ATTR_VHT_CAPABILITY_MASK] = {
		.len = NL80211_VHT_CAPABILITY_LEN,
	},
	[NL80211_ATTR_MDID] = { .type = NLA_U16 },
	[NL80211_ATTR_IE_RIC] = { .type = NLA_BINARY,
				  .len = IEEE80211_MAX_DATA_LEN },
	[NL80211_ATTR_PEER_AID] = { .type = NLA_U16 },
	[NL80211_ATTR_CH_SWITCH_COUNT] = { .type = NLA_U32 },
	[NL80211_ATTR_CH_SWITCH_BLOCK_TX] = { .type = NLA_FLAG },
	[NL80211_ATTR_CSA_IES] = { .type = NLA_NESTED },
	[NL80211_ATTR_CSA_C_OFF_BEACON] = { .type = NLA_U16 },
	[NL80211_ATTR_CSA_C_OFF_PRESP] = { .type = NLA_U16 },
	[NL80211_ATTR_STA_SUPPORTED_CHANNELS] = { .type = NLA_BINARY },
	[NL80211_ATTR_STA_SUPPORTED_OPER_CLASSES] = { .type = NLA_BINARY },
	[NL80211_ATTR_HANDLE_DFS] = { .type = NLA_FLAG },
>>>>>>> refs/remotes/origin/master
};

/* policy for the key attributes */
static const struct nla_policy nl80211_key_policy[NL80211_KEY_MAX + 1] = {
	[NL80211_KEY_DATA] = { .type = NLA_BINARY, .len = WLAN_MAX_KEY_LEN },
	[NL80211_KEY_IDX] = { .type = NLA_U8 },
	[NL80211_KEY_CIPHER] = { .type = NLA_U32 },
	[NL80211_KEY_SEQ] = { .type = NLA_BINARY, .len = 16 },
	[NL80211_KEY_DEFAULT] = { .type = NLA_FLAG },
	[NL80211_KEY_DEFAULT_MGMT] = { .type = NLA_FLAG },
	[NL80211_KEY_TYPE] = { .type = NLA_U32 },
	[NL80211_KEY_DEFAULT_TYPES] = { .type = NLA_NESTED },
};

/* policy for the key default flags */
static const struct nla_policy
nl80211_key_default_policy[NUM_NL80211_KEY_DEFAULT_TYPES] = {
	[NL80211_KEY_DEFAULT_TYPE_UNICAST] = { .type = NLA_FLAG },
	[NL80211_KEY_DEFAULT_TYPE_MULTICAST] = { .type = NLA_FLAG },
};

/* policy for WoWLAN attributes */
static const struct nla_policy
nl80211_wowlan_policy[NUM_NL80211_WOWLAN_TRIG] = {
	[NL80211_WOWLAN_TRIG_ANY] = { .type = NLA_FLAG },
	[NL80211_WOWLAN_TRIG_DISCONNECT] = { .type = NLA_FLAG },
	[NL80211_WOWLAN_TRIG_MAGIC_PKT] = { .type = NLA_FLAG },
	[NL80211_WOWLAN_TRIG_PKT_PATTERN] = { .type = NLA_NESTED },
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	[NL80211_WOWLAN_TRIG_GTK_REKEY_FAILURE] = { .type = NLA_FLAG },
	[NL80211_WOWLAN_TRIG_EAP_IDENT_REQUEST] = { .type = NLA_FLAG },
	[NL80211_WOWLAN_TRIG_4WAY_HANDSHAKE] = { .type = NLA_FLAG },
	[NL80211_WOWLAN_TRIG_RFKILL_RELEASE] = { .type = NLA_FLAG },
<<<<<<< HEAD
=======
	[NL80211_WOWLAN_TRIG_TCP_CONNECTION] = { .type = NLA_NESTED },
};

static const struct nla_policy
nl80211_wowlan_tcp_policy[NUM_NL80211_WOWLAN_TCP] = {
	[NL80211_WOWLAN_TCP_SRC_IPV4] = { .type = NLA_U32 },
	[NL80211_WOWLAN_TCP_DST_IPV4] = { .type = NLA_U32 },
	[NL80211_WOWLAN_TCP_DST_MAC] = { .len = ETH_ALEN },
	[NL80211_WOWLAN_TCP_SRC_PORT] = { .type = NLA_U16 },
	[NL80211_WOWLAN_TCP_DST_PORT] = { .type = NLA_U16 },
	[NL80211_WOWLAN_TCP_DATA_PAYLOAD] = { .len = 1 },
	[NL80211_WOWLAN_TCP_DATA_PAYLOAD_SEQ] = {
		.len = sizeof(struct nl80211_wowlan_tcp_data_seq)
	},
	[NL80211_WOWLAN_TCP_DATA_PAYLOAD_TOKEN] = {
		.len = sizeof(struct nl80211_wowlan_tcp_data_token)
	},
	[NL80211_WOWLAN_TCP_DATA_INTERVAL] = { .type = NLA_U32 },
	[NL80211_WOWLAN_TCP_WAKE_PAYLOAD] = { .len = 1 },
	[NL80211_WOWLAN_TCP_WAKE_MASK] = { .len = 1 },
};

/* policy for coalesce rule attributes */
static const struct nla_policy
nl80211_coalesce_policy[NUM_NL80211_ATTR_COALESCE_RULE] = {
	[NL80211_ATTR_COALESCE_RULE_DELAY] = { .type = NLA_U32 },
	[NL80211_ATTR_COALESCE_RULE_CONDITION] = { .type = NLA_U32 },
	[NL80211_ATTR_COALESCE_RULE_PKT_PATTERN] = { .type = NLA_NESTED },
>>>>>>> refs/remotes/origin/master
};

/* policy for GTK rekey offload attributes */
static const struct nla_policy
nl80211_rekey_policy[NUM_NL80211_REKEY_DATA] = {
	[NL80211_REKEY_DATA_KEK] = { .len = NL80211_KEK_LEN },
	[NL80211_REKEY_DATA_KCK] = { .len = NL80211_KCK_LEN },
	[NL80211_REKEY_DATA_REPLAY_CTR] = { .len = NL80211_REPLAY_CTR_LEN },
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};

static const struct nla_policy
nl80211_match_policy[NL80211_SCHED_SCAN_MATCH_ATTR_MAX + 1] = {
<<<<<<< HEAD
	[NL80211_ATTR_SCHED_SCAN_MATCH_SSID] = { .type = NLA_BINARY,
						 .len = IEEE80211_MAX_SSID_LEN },
};

/* ifidx get helper */
static int nl80211_get_ifidx(struct netlink_callback *cb)
{
	int res;

	res = nlmsg_parse(cb->nlh, GENL_HDRLEN + nl80211_fam.hdrsize,
			  nl80211_fam.attrbuf, nl80211_fam.maxattr,
			  nl80211_policy);
	if (res)
		return res;

	if (!nl80211_fam.attrbuf[NL80211_ATTR_IFINDEX])
		return -EINVAL;

	res = nla_get_u32(nl80211_fam.attrbuf[NL80211_ATTR_IFINDEX]);
	if (!res)
		return -EINVAL;
	return res;
}

static int nl80211_prepare_netdev_dump(struct sk_buff *skb,
				       struct netlink_callback *cb,
				       struct cfg80211_registered_device **rdev,
				       struct net_device **dev)
{
	int ifidx = cb->args[0];
	int err;

	if (!ifidx)
		ifidx = nl80211_get_ifidx(cb);
	if (ifidx < 0)
		return ifidx;

	cb->args[0] = ifidx;

	rtnl_lock();

	*dev = __dev_get_by_index(sock_net(skb->sk), ifidx);
	if (!*dev) {
		err = -ENODEV;
		goto out_rtnl;
	}

	*rdev = cfg80211_get_dev_from_ifindex(sock_net(skb->sk), ifidx);
	if (IS_ERR(*rdev)) {
		err = PTR_ERR(*rdev);
		goto out_rtnl;
	}

	return 0;
 out_rtnl:
=======
	[NL80211_SCHED_SCAN_MATCH_ATTR_SSID] = { .type = NLA_BINARY,
						 .len = IEEE80211_MAX_SSID_LEN },
	[NL80211_SCHED_SCAN_MATCH_ATTR_RSSI] = { .type = NLA_U32 },
};

static int nl80211_prepare_wdev_dump(struct sk_buff *skb,
				     struct netlink_callback *cb,
				     struct cfg80211_registered_device **rdev,
				     struct wireless_dev **wdev)
{
	int err;

	rtnl_lock();

	if (!cb->args[0]) {
		err = nlmsg_parse(cb->nlh, GENL_HDRLEN + nl80211_fam.hdrsize,
				  nl80211_fam.attrbuf, nl80211_fam.maxattr,
				  nl80211_policy);
		if (err)
			goto out_unlock;

		*wdev = __cfg80211_wdev_from_attrs(sock_net(skb->sk),
						   nl80211_fam.attrbuf);
		if (IS_ERR(*wdev)) {
			err = PTR_ERR(*wdev);
			goto out_unlock;
		}
		*rdev = wiphy_to_dev((*wdev)->wiphy);
		/* 0 is the first index - add 1 to parse only once */
		cb->args[0] = (*rdev)->wiphy_idx + 1;
		cb->args[1] = (*wdev)->identifier;
	} else {
		/* subtract the 1 again here */
		struct wiphy *wiphy = wiphy_idx_to_wiphy(cb->args[0] - 1);
		struct wireless_dev *tmp;

		if (!wiphy) {
			err = -ENODEV;
			goto out_unlock;
		}
		*rdev = wiphy_to_dev(wiphy);
		*wdev = NULL;

		list_for_each_entry(tmp, &(*rdev)->wdev_list, list) {
			if (tmp->identifier == cb->args[1]) {
				*wdev = tmp;
				break;
			}
		}

		if (!*wdev) {
			err = -ENODEV;
			goto out_unlock;
		}
	}

	return 0;
 out_unlock:
>>>>>>> refs/remotes/origin/master
	rtnl_unlock();
	return err;
}

<<<<<<< HEAD
static void nl80211_finish_netdev_dump(struct cfg80211_registered_device *rdev)
{
	cfg80211_unlock_rdev(rdev);
=======
static void nl80211_finish_wdev_dump(struct cfg80211_registered_device *rdev)
{
>>>>>>> refs/remotes/origin/master
	rtnl_unlock();
}

/* IE validation */
static bool is_valid_ie_attr(const struct nlattr *attr)
{
	const u8 *pos;
	int len;

	if (!attr)
		return true;

	pos = nla_data(attr);
	len = nla_len(attr);

	while (len) {
		u8 elemlen;

		if (len < 2)
			return false;
		len -= 2;

		elemlen = pos[1];
		if (elemlen > len)
			return false;

		len -= elemlen;
		pos += 2 + elemlen;
	}

	return true;
}

/* message building helper */
<<<<<<< HEAD
static inline void *nl80211hdr_put(struct sk_buff *skb, u32 pid, u32 seq,
				   int flags, u8 cmd)
{
	/* since there is no private header just add the generic one */
	return genlmsg_put(skb, pid, seq, &nl80211_fam, flags, cmd);
}

static int nl80211_msg_put_channel(struct sk_buff *msg,
				   struct ieee80211_channel *chan)
{
	NLA_PUT_U32(msg, NL80211_FREQUENCY_ATTR_FREQ,
		    chan->center_freq);

	if (chan->flags & IEEE80211_CHAN_DISABLED)
		NLA_PUT_FLAG(msg, NL80211_FREQUENCY_ATTR_DISABLED);
	if (chan->flags & IEEE80211_CHAN_PASSIVE_SCAN)
		NLA_PUT_FLAG(msg, NL80211_FREQUENCY_ATTR_PASSIVE_SCAN);
	if (chan->flags & IEEE80211_CHAN_NO_IBSS)
		NLA_PUT_FLAG(msg, NL80211_FREQUENCY_ATTR_NO_IBSS);
	if (chan->flags & IEEE80211_CHAN_RADAR)
		NLA_PUT_FLAG(msg, NL80211_FREQUENCY_ATTR_RADAR);

	NLA_PUT_U32(msg, NL80211_FREQUENCY_ATTR_MAX_TX_POWER,
		    DBM_TO_MBM(chan->max_power));
=======
static inline void *nl80211hdr_put(struct sk_buff *skb, u32 portid, u32 seq,
				   int flags, u8 cmd)
{
	/* since there is no private header just add the generic one */
	return genlmsg_put(skb, portid, seq, &nl80211_fam, flags, cmd);
}

static int nl80211_msg_put_channel(struct sk_buff *msg,
				   struct ieee80211_channel *chan,
				   bool large)
{
	if (nla_put_u32(msg, NL80211_FREQUENCY_ATTR_FREQ,
			chan->center_freq))
		goto nla_put_failure;

	if ((chan->flags & IEEE80211_CHAN_DISABLED) &&
	    nla_put_flag(msg, NL80211_FREQUENCY_ATTR_DISABLED))
		goto nla_put_failure;
	if ((chan->flags & IEEE80211_CHAN_PASSIVE_SCAN) &&
	    nla_put_flag(msg, NL80211_FREQUENCY_ATTR_PASSIVE_SCAN))
		goto nla_put_failure;
	if ((chan->flags & IEEE80211_CHAN_NO_IBSS) &&
	    nla_put_flag(msg, NL80211_FREQUENCY_ATTR_NO_IBSS))
		goto nla_put_failure;
	if (chan->flags & IEEE80211_CHAN_RADAR) {
		if (nla_put_flag(msg, NL80211_FREQUENCY_ATTR_RADAR))
			goto nla_put_failure;
		if (large) {
			u32 time;

			time = elapsed_jiffies_msecs(chan->dfs_state_entered);

			if (nla_put_u32(msg, NL80211_FREQUENCY_ATTR_DFS_STATE,
					chan->dfs_state))
				goto nla_put_failure;
			if (nla_put_u32(msg, NL80211_FREQUENCY_ATTR_DFS_TIME,
					time))
				goto nla_put_failure;
		}
	}

	if (large) {
		if ((chan->flags & IEEE80211_CHAN_NO_HT40MINUS) &&
		    nla_put_flag(msg, NL80211_FREQUENCY_ATTR_NO_HT40_MINUS))
			goto nla_put_failure;
		if ((chan->flags & IEEE80211_CHAN_NO_HT40PLUS) &&
		    nla_put_flag(msg, NL80211_FREQUENCY_ATTR_NO_HT40_PLUS))
			goto nla_put_failure;
		if ((chan->flags & IEEE80211_CHAN_NO_80MHZ) &&
		    nla_put_flag(msg, NL80211_FREQUENCY_ATTR_NO_80MHZ))
			goto nla_put_failure;
		if ((chan->flags & IEEE80211_CHAN_NO_160MHZ) &&
		    nla_put_flag(msg, NL80211_FREQUENCY_ATTR_NO_160MHZ))
			goto nla_put_failure;
	}

	if (nla_put_u32(msg, NL80211_FREQUENCY_ATTR_MAX_TX_POWER,
			DBM_TO_MBM(chan->max_power)))
		goto nla_put_failure;
>>>>>>> refs/remotes/origin/master

	return 0;

 nla_put_failure:
	return -ENOBUFS;
}

/* netlink command implementations */

struct key_parse {
	struct key_params p;
	int idx;
	int type;
	bool def, defmgmt;
	bool def_uni, def_multi;
};

static int nl80211_parse_key_new(struct nlattr *key, struct key_parse *k)
{
	struct nlattr *tb[NL80211_KEY_MAX + 1];
	int err = nla_parse_nested(tb, NL80211_KEY_MAX, key,
				   nl80211_key_policy);
	if (err)
		return err;

	k->def = !!tb[NL80211_KEY_DEFAULT];
	k->defmgmt = !!tb[NL80211_KEY_DEFAULT_MGMT];

	if (k->def) {
		k->def_uni = true;
		k->def_multi = true;
	}
	if (k->defmgmt)
		k->def_multi = true;

	if (tb[NL80211_KEY_IDX])
		k->idx = nla_get_u8(tb[NL80211_KEY_IDX]);

	if (tb[NL80211_KEY_DATA]) {
		k->p.key = nla_data(tb[NL80211_KEY_DATA]);
		k->p.key_len = nla_len(tb[NL80211_KEY_DATA]);
	}

	if (tb[NL80211_KEY_SEQ]) {
		k->p.seq = nla_data(tb[NL80211_KEY_SEQ]);
		k->p.seq_len = nla_len(tb[NL80211_KEY_SEQ]);
	}

	if (tb[NL80211_KEY_CIPHER])
		k->p.cipher = nla_get_u32(tb[NL80211_KEY_CIPHER]);

	if (tb[NL80211_KEY_TYPE]) {
		k->type = nla_get_u32(tb[NL80211_KEY_TYPE]);
		if (k->type < 0 || k->type >= NUM_NL80211_KEYTYPES)
			return -EINVAL;
	}

	if (tb[NL80211_KEY_DEFAULT_TYPES]) {
		struct nlattr *kdt[NUM_NL80211_KEY_DEFAULT_TYPES];
<<<<<<< HEAD
<<<<<<< HEAD
		int err = nla_parse_nested(kdt,
					   NUM_NL80211_KEY_DEFAULT_TYPES - 1,
					   tb[NL80211_KEY_DEFAULT_TYPES],
					   nl80211_key_default_policy);
=======
		err = nla_parse_nested(kdt, NUM_NL80211_KEY_DEFAULT_TYPES - 1,
				       tb[NL80211_KEY_DEFAULT_TYPES],
				       nl80211_key_default_policy);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		err = nla_parse_nested(kdt, NUM_NL80211_KEY_DEFAULT_TYPES - 1,
				       tb[NL80211_KEY_DEFAULT_TYPES],
				       nl80211_key_default_policy);
>>>>>>> refs/remotes/origin/master
		if (err)
			return err;

		k->def_uni = kdt[NL80211_KEY_DEFAULT_TYPE_UNICAST];
		k->def_multi = kdt[NL80211_KEY_DEFAULT_TYPE_MULTICAST];
	}

	return 0;
}

static int nl80211_parse_key_old(struct genl_info *info, struct key_parse *k)
{
	if (info->attrs[NL80211_ATTR_KEY_DATA]) {
		k->p.key = nla_data(info->attrs[NL80211_ATTR_KEY_DATA]);
		k->p.key_len = nla_len(info->attrs[NL80211_ATTR_KEY_DATA]);
	}

	if (info->attrs[NL80211_ATTR_KEY_SEQ]) {
		k->p.seq = nla_data(info->attrs[NL80211_ATTR_KEY_SEQ]);
		k->p.seq_len = nla_len(info->attrs[NL80211_ATTR_KEY_SEQ]);
	}

	if (info->attrs[NL80211_ATTR_KEY_IDX])
		k->idx = nla_get_u8(info->attrs[NL80211_ATTR_KEY_IDX]);

	if (info->attrs[NL80211_ATTR_KEY_CIPHER])
		k->p.cipher = nla_get_u32(info->attrs[NL80211_ATTR_KEY_CIPHER]);

	k->def = !!info->attrs[NL80211_ATTR_KEY_DEFAULT];
	k->defmgmt = !!info->attrs[NL80211_ATTR_KEY_DEFAULT_MGMT];

	if (k->def) {
		k->def_uni = true;
		k->def_multi = true;
	}
	if (k->defmgmt)
		k->def_multi = true;

	if (info->attrs[NL80211_ATTR_KEY_TYPE]) {
		k->type = nla_get_u32(info->attrs[NL80211_ATTR_KEY_TYPE]);
		if (k->type < 0 || k->type >= NUM_NL80211_KEYTYPES)
			return -EINVAL;
	}

	if (info->attrs[NL80211_ATTR_KEY_DEFAULT_TYPES]) {
		struct nlattr *kdt[NUM_NL80211_KEY_DEFAULT_TYPES];
		int err = nla_parse_nested(
				kdt, NUM_NL80211_KEY_DEFAULT_TYPES - 1,
				info->attrs[NL80211_ATTR_KEY_DEFAULT_TYPES],
				nl80211_key_default_policy);
		if (err)
			return err;

		k->def_uni = kdt[NL80211_KEY_DEFAULT_TYPE_UNICAST];
		k->def_multi = kdt[NL80211_KEY_DEFAULT_TYPE_MULTICAST];
	}

	return 0;
}

static int nl80211_parse_key(struct genl_info *info, struct key_parse *k)
{
	int err;

	memset(k, 0, sizeof(*k));
	k->idx = -1;
	k->type = -1;

	if (info->attrs[NL80211_ATTR_KEY])
		err = nl80211_parse_key_new(info->attrs[NL80211_ATTR_KEY], k);
	else
		err = nl80211_parse_key_old(info, k);

	if (err)
		return err;

	if (k->def && k->defmgmt)
		return -EINVAL;

	if (k->defmgmt) {
		if (k->def_uni || !k->def_multi)
			return -EINVAL;
	}

	if (k->idx != -1) {
		if (k->defmgmt) {
			if (k->idx < 4 || k->idx > 5)
				return -EINVAL;
		} else if (k->def) {
			if (k->idx < 0 || k->idx > 3)
				return -EINVAL;
		} else {
			if (k->idx < 0 || k->idx > 5)
				return -EINVAL;
		}
	}

	return 0;
}

static struct cfg80211_cached_keys *
nl80211_parse_connkeys(struct cfg80211_registered_device *rdev,
<<<<<<< HEAD
		       struct nlattr *keys)
=======
		       struct nlattr *keys, bool *no_ht)
>>>>>>> refs/remotes/origin/master
{
	struct key_parse parse;
	struct nlattr *key;
	struct cfg80211_cached_keys *result;
	int rem, err, def = 0;

	result = kzalloc(sizeof(*result), GFP_KERNEL);
	if (!result)
		return ERR_PTR(-ENOMEM);

	result->def = -1;
	result->defmgmt = -1;

	nla_for_each_nested(key, keys, rem) {
		memset(&parse, 0, sizeof(parse));
		parse.idx = -1;

		err = nl80211_parse_key_new(key, &parse);
		if (err)
			goto error;
		err = -EINVAL;
		if (!parse.p.key)
			goto error;
		if (parse.idx < 0 || parse.idx > 4)
			goto error;
		if (parse.def) {
			if (def)
				goto error;
			def = 1;
			result->def = parse.idx;
			if (!parse.def_uni || !parse.def_multi)
				goto error;
		} else if (parse.defmgmt)
			goto error;
		err = cfg80211_validate_key_settings(rdev, &parse.p,
						     parse.idx, false, NULL);
		if (err)
			goto error;
		result->params[parse.idx].cipher = parse.p.cipher;
		result->params[parse.idx].key_len = parse.p.key_len;
		result->params[parse.idx].key = result->data[parse.idx];
		memcpy(result->data[parse.idx], parse.p.key, parse.p.key_len);
<<<<<<< HEAD
=======

		if (parse.p.cipher == WLAN_CIPHER_SUITE_WEP40 ||
		    parse.p.cipher == WLAN_CIPHER_SUITE_WEP104) {
			if (no_ht)
				*no_ht = true;
		}
>>>>>>> refs/remotes/origin/master
	}

	return result;
 error:
	kfree(result);
	return ERR_PTR(err);
}

static int nl80211_key_allowed(struct wireless_dev *wdev)
{
	ASSERT_WDEV_LOCK(wdev);

	switch (wdev->iftype) {
	case NL80211_IFTYPE_AP:
	case NL80211_IFTYPE_AP_VLAN:
	case NL80211_IFTYPE_P2P_GO:
	case NL80211_IFTYPE_MESH_POINT:
		break;
	case NL80211_IFTYPE_ADHOC:
<<<<<<< HEAD
		if (!wdev->current_bss)
			return -ENOLINK;
		break;
	case NL80211_IFTYPE_STATION:
	case NL80211_IFTYPE_P2P_CLIENT:
		if (wdev->sme_state != CFG80211_SME_CONNECTED)
=======
	case NL80211_IFTYPE_STATION:
	case NL80211_IFTYPE_P2P_CLIENT:
		if (!wdev->current_bss)
>>>>>>> refs/remotes/origin/master
			return -ENOLINK;
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static int nl80211_put_iftypes(struct sk_buff *msg, u32 attr, u16 ifmodes)
{
	struct nlattr *nl_modes = nla_nest_start(msg, attr);
	int i;

	if (!nl_modes)
		goto nla_put_failure;

	i = 0;
	while (ifmodes) {
<<<<<<< HEAD
		if (ifmodes & 1)
			NLA_PUT_FLAG(msg, i);
=======
		if ((ifmodes & 1) && nla_put_flag(msg, i))
			goto nla_put_failure;
>>>>>>> refs/remotes/origin/master
		ifmodes >>= 1;
		i++;
	}

	nla_nest_end(msg, nl_modes);
	return 0;

nla_put_failure:
	return -ENOBUFS;
}

static int nl80211_put_iface_combinations(struct wiphy *wiphy,
<<<<<<< HEAD
					  struct sk_buff *msg)
=======
					  struct sk_buff *msg,
					  bool large)
>>>>>>> refs/remotes/origin/master
{
	struct nlattr *nl_combis;
	int i, j;

	nl_combis = nla_nest_start(msg,
				NL80211_ATTR_INTERFACE_COMBINATIONS);
	if (!nl_combis)
		goto nla_put_failure;

	for (i = 0; i < wiphy->n_iface_combinations; i++) {
		const struct ieee80211_iface_combination *c;
		struct nlattr *nl_combi, *nl_limits;

		c = &wiphy->iface_combinations[i];

		nl_combi = nla_nest_start(msg, i + 1);
		if (!nl_combi)
			goto nla_put_failure;

		nl_limits = nla_nest_start(msg, NL80211_IFACE_COMB_LIMITS);
		if (!nl_limits)
			goto nla_put_failure;

		for (j = 0; j < c->n_limits; j++) {
			struct nlattr *nl_limit;

			nl_limit = nla_nest_start(msg, j + 1);
			if (!nl_limit)
				goto nla_put_failure;
<<<<<<< HEAD
			NLA_PUT_U32(msg, NL80211_IFACE_LIMIT_MAX,
				    c->limits[j].max);
=======
			if (nla_put_u32(msg, NL80211_IFACE_LIMIT_MAX,
					c->limits[j].max))
				goto nla_put_failure;
>>>>>>> refs/remotes/origin/master
			if (nl80211_put_iftypes(msg, NL80211_IFACE_LIMIT_TYPES,
						c->limits[j].types))
				goto nla_put_failure;
			nla_nest_end(msg, nl_limit);
		}

		nla_nest_end(msg, nl_limits);

<<<<<<< HEAD
		if (c->beacon_int_infra_match)
			NLA_PUT_FLAG(msg,
				NL80211_IFACE_COMB_STA_AP_BI_MATCH);
		NLA_PUT_U32(msg, NL80211_IFACE_COMB_NUM_CHANNELS,
			    c->num_different_channels);
		NLA_PUT_U32(msg, NL80211_IFACE_COMB_MAXNUM,
			    c->max_interfaces);
=======
		if (c->beacon_int_infra_match &&
		    nla_put_flag(msg, NL80211_IFACE_COMB_STA_AP_BI_MATCH))
			goto nla_put_failure;
		if (nla_put_u32(msg, NL80211_IFACE_COMB_NUM_CHANNELS,
				c->num_different_channels) ||
		    nla_put_u32(msg, NL80211_IFACE_COMB_MAXNUM,
				c->max_interfaces))
			goto nla_put_failure;
		if (large &&
		    nla_put_u32(msg, NL80211_IFACE_COMB_RADAR_DETECT_WIDTHS,
				c->radar_detect_widths))
			goto nla_put_failure;
>>>>>>> refs/remotes/origin/master

		nla_nest_end(msg, nl_combi);
	}

	nla_nest_end(msg, nl_combis);

	return 0;
nla_put_failure:
	return -ENOBUFS;
}

<<<<<<< HEAD
static int nl80211_send_wiphy(struct sk_buff *msg, u32 pid, u32 seq, int flags,
			      struct cfg80211_registered_device *dev)
{
	void *hdr;
	struct nlattr *nl_bands, *nl_band;
	struct nlattr *nl_freqs, *nl_freq;
	struct nlattr *nl_rates, *nl_rate;
	struct nlattr *nl_cmds;
	enum ieee80211_band band;
	struct ieee80211_channel *chan;
	struct ieee80211_rate *rate;
	int i;
	const struct ieee80211_txrx_stypes *mgmt_stypes =
				dev->wiphy.mgmt_stypes;

	hdr = nl80211hdr_put(msg, pid, seq, flags, NL80211_CMD_NEW_WIPHY);
	if (!hdr)
		return -1;

	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY, dev->wiphy_idx);
	NLA_PUT_STRING(msg, NL80211_ATTR_WIPHY_NAME, wiphy_name(&dev->wiphy));

	NLA_PUT_U32(msg, NL80211_ATTR_GENERATION,
		    cfg80211_rdev_list_generation);

	NLA_PUT_U8(msg, NL80211_ATTR_WIPHY_RETRY_SHORT,
		   dev->wiphy.retry_short);
	NLA_PUT_U8(msg, NL80211_ATTR_WIPHY_RETRY_LONG,
		   dev->wiphy.retry_long);
	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY_FRAG_THRESHOLD,
		    dev->wiphy.frag_threshold);
	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY_RTS_THRESHOLD,
		    dev->wiphy.rts_threshold);
	NLA_PUT_U8(msg, NL80211_ATTR_WIPHY_COVERAGE_CLASS,
		    dev->wiphy.coverage_class);
	NLA_PUT_U8(msg, NL80211_ATTR_MAX_NUM_SCAN_SSIDS,
		   dev->wiphy.max_scan_ssids);
	NLA_PUT_U8(msg, NL80211_ATTR_MAX_NUM_SCHED_SCAN_SSIDS,
		   dev->wiphy.max_sched_scan_ssids);
	NLA_PUT_U16(msg, NL80211_ATTR_MAX_SCAN_IE_LEN,
		    dev->wiphy.max_scan_ie_len);
	NLA_PUT_U16(msg, NL80211_ATTR_MAX_SCHED_SCAN_IE_LEN,
		    dev->wiphy.max_sched_scan_ie_len);
	NLA_PUT_U8(msg, NL80211_ATTR_MAX_MATCH_SETS,
		   dev->wiphy.max_match_sets);

	if (dev->wiphy.flags & WIPHY_FLAG_IBSS_RSN)
		NLA_PUT_FLAG(msg, NL80211_ATTR_SUPPORT_IBSS_RSN);
	if (dev->wiphy.flags & WIPHY_FLAG_MESH_AUTH)
		NLA_PUT_FLAG(msg, NL80211_ATTR_SUPPORT_MESH_AUTH);
<<<<<<< HEAD
=======
	if (dev->wiphy.flags & WIPHY_FLAG_AP_UAPSD)
		NLA_PUT_FLAG(msg, NL80211_ATTR_SUPPORT_AP_UAPSD);
	if (dev->wiphy.flags & WIPHY_FLAG_SUPPORTS_FW_ROAM)
		NLA_PUT_FLAG(msg, NL80211_ATTR_ROAM_SUPPORT);
	if (dev->wiphy.flags & WIPHY_FLAG_SUPPORTS_TDLS)
		NLA_PUT_FLAG(msg, NL80211_ATTR_TDLS_SUPPORT);
	if (dev->wiphy.flags & WIPHY_FLAG_TDLS_EXTERNAL_SETUP)
		NLA_PUT_FLAG(msg, NL80211_ATTR_TDLS_EXTERNAL_SETUP);
>>>>>>> refs/remotes/origin/cm-10.0

	NLA_PUT(msg, NL80211_ATTR_CIPHER_SUITES,
		sizeof(u32) * dev->wiphy.n_cipher_suites,
		dev->wiphy.cipher_suites);

	NLA_PUT_U8(msg, NL80211_ATTR_MAX_NUM_PMKIDS,
		   dev->wiphy.max_num_pmkids);

	if (dev->wiphy.flags & WIPHY_FLAG_CONTROL_PORT_PROTOCOL)
		NLA_PUT_FLAG(msg, NL80211_ATTR_CONTROL_PORT_ETHERTYPE);

	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY_ANTENNA_AVAIL_TX,
		    dev->wiphy.available_antennas_tx);
	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY_ANTENNA_AVAIL_RX,
		    dev->wiphy.available_antennas_rx);

<<<<<<< HEAD
=======
	if (dev->wiphy.flags & WIPHY_FLAG_AP_PROBE_RESP_OFFLOAD)
		NLA_PUT_U32(msg, NL80211_ATTR_PROBE_RESP_OFFLOAD,
			    dev->wiphy.probe_resp_offload);

>>>>>>> refs/remotes/origin/cm-10.0
	if ((dev->wiphy.available_antennas_tx ||
	     dev->wiphy.available_antennas_rx) && dev->ops->get_antenna) {
		u32 tx_ant = 0, rx_ant = 0;
		int res;
		res = dev->ops->get_antenna(&dev->wiphy, &tx_ant, &rx_ant);
		if (!res) {
			NLA_PUT_U32(msg, NL80211_ATTR_WIPHY_ANTENNA_TX, tx_ant);
			NLA_PUT_U32(msg, NL80211_ATTR_WIPHY_ANTENNA_RX, rx_ant);
		}
	}

	if (nl80211_put_iftypes(msg, NL80211_ATTR_SUPPORTED_IFTYPES,
				dev->wiphy.interface_modes))
		goto nla_put_failure;

	nl_bands = nla_nest_start(msg, NL80211_ATTR_WIPHY_BANDS);
	if (!nl_bands)
		goto nla_put_failure;

	for (band = 0; band < IEEE80211_NUM_BANDS; band++) {
		if (!dev->wiphy.bands[band])
			continue;

		nl_band = nla_nest_start(msg, band);
		if (!nl_band)
			goto nla_put_failure;

		/* add HT info */
		if (dev->wiphy.bands[band]->ht_cap.ht_supported) {
			NLA_PUT(msg, NL80211_BAND_ATTR_HT_MCS_SET,
				sizeof(dev->wiphy.bands[band]->ht_cap.mcs),
				&dev->wiphy.bands[band]->ht_cap.mcs);
			NLA_PUT_U16(msg, NL80211_BAND_ATTR_HT_CAPA,
				dev->wiphy.bands[band]->ht_cap.cap);
			NLA_PUT_U8(msg, NL80211_BAND_ATTR_HT_AMPDU_FACTOR,
				dev->wiphy.bands[band]->ht_cap.ampdu_factor);
			NLA_PUT_U8(msg, NL80211_BAND_ATTR_HT_AMPDU_DENSITY,
				dev->wiphy.bands[band]->ht_cap.ampdu_density);
		}

		/* add frequencies */
		nl_freqs = nla_nest_start(msg, NL80211_BAND_ATTR_FREQS);
		if (!nl_freqs)
			goto nla_put_failure;

		for (i = 0; i < dev->wiphy.bands[band]->n_channels; i++) {
			nl_freq = nla_nest_start(msg, i);
			if (!nl_freq)
				goto nla_put_failure;

			chan = &dev->wiphy.bands[band]->channels[i];

			if (nl80211_msg_put_channel(msg, chan))
				goto nla_put_failure;

			nla_nest_end(msg, nl_freq);
		}

		nla_nest_end(msg, nl_freqs);

		/* add bitrates */
		nl_rates = nla_nest_start(msg, NL80211_BAND_ATTR_RATES);
		if (!nl_rates)
			goto nla_put_failure;

		for (i = 0; i < dev->wiphy.bands[band]->n_bitrates; i++) {
			nl_rate = nla_nest_start(msg, i);
			if (!nl_rate)
				goto nla_put_failure;

			rate = &dev->wiphy.bands[band]->bitrates[i];
			NLA_PUT_U32(msg, NL80211_BITRATE_ATTR_RATE,
				    rate->bitrate);
			if (rate->flags & IEEE80211_RATE_SHORT_PREAMBLE)
				NLA_PUT_FLAG(msg,
					NL80211_BITRATE_ATTR_2GHZ_SHORTPREAMBLE);

			nla_nest_end(msg, nl_rate);
		}

		nla_nest_end(msg, nl_rates);

		nla_nest_end(msg, nl_band);
	}
	nla_nest_end(msg, nl_bands);

	nl_cmds = nla_nest_start(msg, NL80211_ATTR_SUPPORTED_COMMANDS);
	if (!nl_cmds)
		goto nla_put_failure;

	i = 0;
#define CMD(op, n)						\
	 do {							\
		if (dev->ops->op) {				\
			i++;					\
			NLA_PUT_U32(msg, i, NL80211_CMD_ ## n);	\
		}						\
	} while (0)

	CMD(add_virtual_intf, NEW_INTERFACE);
	CMD(change_virtual_intf, SET_INTERFACE);
	CMD(add_key, NEW_KEY);
<<<<<<< HEAD
	CMD(add_beacon, NEW_BEACON);
=======
	CMD(start_ap, START_AP);
>>>>>>> refs/remotes/origin/cm-10.0
	CMD(add_station, NEW_STATION);
	CMD(add_mpath, NEW_MPATH);
	CMD(update_mesh_config, SET_MESH_CONFIG);
	CMD(change_bss, SET_BSS);
	CMD(auth, AUTHENTICATE);
	CMD(assoc, ASSOCIATE);
	CMD(deauth, DEAUTHENTICATE);
	CMD(disassoc, DISASSOCIATE);
	CMD(join_ibss, JOIN_IBSS);
	CMD(join_mesh, JOIN_MESH);
	CMD(set_pmksa, SET_PMKSA);
	CMD(del_pmksa, DEL_PMKSA);
	CMD(flush_pmksa, FLUSH_PMKSA);
<<<<<<< HEAD
	CMD(remain_on_channel, REMAIN_ON_CHANNEL);
=======
	if (dev->wiphy.flags & WIPHY_FLAG_HAS_REMAIN_ON_CHANNEL)
		CMD(remain_on_channel, REMAIN_ON_CHANNEL);
>>>>>>> refs/remotes/origin/cm-10.0
	CMD(set_bitrate_mask, SET_TX_BITRATE_MASK);
	CMD(mgmt_tx, FRAME);
	CMD(mgmt_tx_cancel_wait, FRAME_WAIT_CANCEL);
	if (dev->wiphy.flags & WIPHY_FLAG_NETNS_OK) {
		i++;
		NLA_PUT_U32(msg, i, NL80211_CMD_SET_WIPHY_NETNS);
	}
	CMD(set_channel, SET_CHANNEL);
	CMD(set_wds_peer, SET_WDS_PEER);
<<<<<<< HEAD
	if (dev->wiphy.flags & WIPHY_FLAG_SUPPORTS_SCHED_SCAN)
		CMD(sched_scan_start, START_SCHED_SCAN);
=======
	if (dev->wiphy.flags & WIPHY_FLAG_SUPPORTS_TDLS) {
		CMD(tdls_mgmt, TDLS_MGMT);
		CMD(tdls_oper, TDLS_OPER);
	}
	if (dev->wiphy.flags & WIPHY_FLAG_SUPPORTS_SCHED_SCAN)
		CMD(sched_scan_start, START_SCHED_SCAN);
	CMD(probe_client, PROBE_CLIENT);
	CMD(set_noack_map, SET_NOACK_MAP);
	if (dev->wiphy.flags & WIPHY_FLAG_REPORTS_OBSS) {
		i++;
		NLA_PUT_U32(msg, i, NL80211_CMD_REGISTER_BEACONS);
	}

#ifdef CONFIG_NL80211_TESTMODE
	CMD(testmode_cmd, TESTMODE);
#endif
>>>>>>> refs/remotes/origin/cm-10.0

#undef CMD

	if (dev->ops->connect || dev->ops->auth) {
		i++;
		NLA_PUT_U32(msg, i, NL80211_CMD_CONNECT);
	}

	if (dev->ops->disconnect || dev->ops->deauth) {
		i++;
		NLA_PUT_U32(msg, i, NL80211_CMD_DISCONNECT);
	}

	nla_nest_end(msg, nl_cmds);

<<<<<<< HEAD
	if (dev->ops->remain_on_channel)
		NLA_PUT_U32(msg, NL80211_ATTR_MAX_REMAIN_ON_CHANNEL_DURATION,
			    dev->wiphy.max_remain_on_channel_duration);

	/* for now at least assume all drivers have it */
	if (dev->ops->mgmt_tx)
=======
	if (dev->ops->remain_on_channel &&
	    dev->wiphy.flags & WIPHY_FLAG_HAS_REMAIN_ON_CHANNEL)
		NLA_PUT_U32(msg, NL80211_ATTR_MAX_REMAIN_ON_CHANNEL_DURATION,
			    dev->wiphy.max_remain_on_channel_duration);

	if (dev->wiphy.flags & WIPHY_FLAG_OFFCHAN_TX)
>>>>>>> refs/remotes/origin/cm-10.0
		NLA_PUT_FLAG(msg, NL80211_ATTR_OFFCHANNEL_TX_OK);

	if (mgmt_stypes) {
		u16 stypes;
		struct nlattr *nl_ftypes, *nl_ifs;
		enum nl80211_iftype ift;

		nl_ifs = nla_nest_start(msg, NL80211_ATTR_TX_FRAME_TYPES);
		if (!nl_ifs)
			goto nla_put_failure;

		for (ift = 0; ift < NUM_NL80211_IFTYPES; ift++) {
			nl_ftypes = nla_nest_start(msg, ift);
			if (!nl_ftypes)
				goto nla_put_failure;
			i = 0;
			stypes = mgmt_stypes[ift].tx;
			while (stypes) {
				if (stypes & 1)
					NLA_PUT_U16(msg, NL80211_ATTR_FRAME_TYPE,
						    (i << 4) | IEEE80211_FTYPE_MGMT);
				stypes >>= 1;
				i++;
			}
			nla_nest_end(msg, nl_ftypes);
		}

		nla_nest_end(msg, nl_ifs);

		nl_ifs = nla_nest_start(msg, NL80211_ATTR_RX_FRAME_TYPES);
		if (!nl_ifs)
			goto nla_put_failure;

		for (ift = 0; ift < NUM_NL80211_IFTYPES; ift++) {
			nl_ftypes = nla_nest_start(msg, ift);
			if (!nl_ftypes)
				goto nla_put_failure;
			i = 0;
			stypes = mgmt_stypes[ift].rx;
			while (stypes) {
				if (stypes & 1)
					NLA_PUT_U16(msg, NL80211_ATTR_FRAME_TYPE,
						    (i << 4) | IEEE80211_FTYPE_MGMT);
				stypes >>= 1;
				i++;
			}
			nla_nest_end(msg, nl_ftypes);
		}
		nla_nest_end(msg, nl_ifs);
	}

	if (dev->wiphy.wowlan.flags || dev->wiphy.wowlan.n_patterns) {
		struct nlattr *nl_wowlan;

		nl_wowlan = nla_nest_start(msg,
				NL80211_ATTR_WOWLAN_TRIGGERS_SUPPORTED);
		if (!nl_wowlan)
			goto nla_put_failure;

		if (dev->wiphy.wowlan.flags & WIPHY_WOWLAN_ANY)
			NLA_PUT_FLAG(msg, NL80211_WOWLAN_TRIG_ANY);
		if (dev->wiphy.wowlan.flags & WIPHY_WOWLAN_DISCONNECT)
			NLA_PUT_FLAG(msg, NL80211_WOWLAN_TRIG_DISCONNECT);
		if (dev->wiphy.wowlan.flags & WIPHY_WOWLAN_MAGIC_PKT)
			NLA_PUT_FLAG(msg, NL80211_WOWLAN_TRIG_MAGIC_PKT);
<<<<<<< HEAD
=======
		if (dev->wiphy.wowlan.flags & WIPHY_WOWLAN_SUPPORTS_GTK_REKEY)
			NLA_PUT_FLAG(msg, NL80211_WOWLAN_TRIG_GTK_REKEY_SUPPORTED);
		if (dev->wiphy.wowlan.flags & WIPHY_WOWLAN_GTK_REKEY_FAILURE)
			NLA_PUT_FLAG(msg, NL80211_WOWLAN_TRIG_GTK_REKEY_FAILURE);
		if (dev->wiphy.wowlan.flags & WIPHY_WOWLAN_EAP_IDENTITY_REQ)
			NLA_PUT_FLAG(msg, NL80211_WOWLAN_TRIG_EAP_IDENT_REQUEST);
		if (dev->wiphy.wowlan.flags & WIPHY_WOWLAN_4WAY_HANDSHAKE)
			NLA_PUT_FLAG(msg, NL80211_WOWLAN_TRIG_4WAY_HANDSHAKE);
		if (dev->wiphy.wowlan.flags & WIPHY_WOWLAN_RFKILL_RELEASE)
			NLA_PUT_FLAG(msg, NL80211_WOWLAN_TRIG_RFKILL_RELEASE);
>>>>>>> refs/remotes/origin/cm-10.0
		if (dev->wiphy.wowlan.n_patterns) {
			struct nl80211_wowlan_pattern_support pat = {
				.max_patterns = dev->wiphy.wowlan.n_patterns,
				.min_pattern_len =
					dev->wiphy.wowlan.pattern_min_len,
				.max_pattern_len =
					dev->wiphy.wowlan.pattern_max_len,
			};
			NLA_PUT(msg, NL80211_WOWLAN_TRIG_PKT_PATTERN,
				sizeof(pat), &pat);
		}

		nla_nest_end(msg, nl_wowlan);
	}

	if (nl80211_put_iftypes(msg, NL80211_ATTR_SOFTWARE_IFTYPES,
				dev->wiphy.software_iftypes))
		goto nla_put_failure;

	if (nl80211_put_iface_combinations(&dev->wiphy, msg))
		goto nla_put_failure;

<<<<<<< HEAD
=======
	if (dev->wiphy.flags & WIPHY_FLAG_HAVE_AP_SME)
		NLA_PUT_U32(msg, NL80211_ATTR_DEVICE_AP_SME,
			    dev->wiphy.ap_sme_capa);

	NLA_PUT_U32(msg, NL80211_ATTR_FEATURE_FLAGS, dev->wiphy.features);

	if (dev->wiphy.ht_capa_mod_mask)
		NLA_PUT(msg, NL80211_ATTR_HT_CAPABILITY_MASK,
			sizeof(*dev->wiphy.ht_capa_mod_mask),
			dev->wiphy.ht_capa_mod_mask);

>>>>>>> refs/remotes/origin/cm-10.0
	return genlmsg_end(msg, hdr);

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	return -EMSGSIZE;
}

static int nl80211_dump_wiphy(struct sk_buff *skb, struct netlink_callback *cb)
{
	int idx = 0;
	int start = cb->args[0];
	struct cfg80211_registered_device *dev;

	mutex_lock(&cfg80211_mutex);
	list_for_each_entry(dev, &cfg80211_rdev_list, list) {
		if (!net_eq(wiphy_net(&dev->wiphy), sock_net(skb->sk)))
			continue;
		if (++idx <= start)
			continue;
		if (nl80211_send_wiphy(skb, NETLINK_CB(cb->skb).pid,
				       cb->nlh->nlmsg_seq, NLM_F_MULTI,
				       dev) < 0) {
			idx--;
			break;
		}
	}
	mutex_unlock(&cfg80211_mutex);

	cb->args[0] = idx;
=======
#ifdef CONFIG_PM
static int nl80211_send_wowlan_tcp_caps(struct cfg80211_registered_device *rdev,
					struct sk_buff *msg)
{
	const struct wiphy_wowlan_tcp_support *tcp = rdev->wiphy.wowlan->tcp;
	struct nlattr *nl_tcp;

	if (!tcp)
		return 0;

	nl_tcp = nla_nest_start(msg, NL80211_WOWLAN_TRIG_TCP_CONNECTION);
	if (!nl_tcp)
		return -ENOBUFS;

	if (nla_put_u32(msg, NL80211_WOWLAN_TCP_DATA_PAYLOAD,
			tcp->data_payload_max))
		return -ENOBUFS;

	if (nla_put_u32(msg, NL80211_WOWLAN_TCP_DATA_PAYLOAD,
			tcp->data_payload_max))
		return -ENOBUFS;

	if (tcp->seq && nla_put_flag(msg, NL80211_WOWLAN_TCP_DATA_PAYLOAD_SEQ))
		return -ENOBUFS;

	if (tcp->tok && nla_put(msg, NL80211_WOWLAN_TCP_DATA_PAYLOAD_TOKEN,
				sizeof(*tcp->tok), tcp->tok))
		return -ENOBUFS;

	if (nla_put_u32(msg, NL80211_WOWLAN_TCP_DATA_INTERVAL,
			tcp->data_interval_max))
		return -ENOBUFS;

	if (nla_put_u32(msg, NL80211_WOWLAN_TCP_WAKE_PAYLOAD,
			tcp->wake_payload_max))
		return -ENOBUFS;

	nla_nest_end(msg, nl_tcp);
	return 0;
}

static int nl80211_send_wowlan(struct sk_buff *msg,
			       struct cfg80211_registered_device *dev,
			       bool large)
{
	struct nlattr *nl_wowlan;

	if (!dev->wiphy.wowlan)
		return 0;

	nl_wowlan = nla_nest_start(msg, NL80211_ATTR_WOWLAN_TRIGGERS_SUPPORTED);
	if (!nl_wowlan)
		return -ENOBUFS;

	if (((dev->wiphy.wowlan->flags & WIPHY_WOWLAN_ANY) &&
	     nla_put_flag(msg, NL80211_WOWLAN_TRIG_ANY)) ||
	    ((dev->wiphy.wowlan->flags & WIPHY_WOWLAN_DISCONNECT) &&
	     nla_put_flag(msg, NL80211_WOWLAN_TRIG_DISCONNECT)) ||
	    ((dev->wiphy.wowlan->flags & WIPHY_WOWLAN_MAGIC_PKT) &&
	     nla_put_flag(msg, NL80211_WOWLAN_TRIG_MAGIC_PKT)) ||
	    ((dev->wiphy.wowlan->flags & WIPHY_WOWLAN_SUPPORTS_GTK_REKEY) &&
	     nla_put_flag(msg, NL80211_WOWLAN_TRIG_GTK_REKEY_SUPPORTED)) ||
	    ((dev->wiphy.wowlan->flags & WIPHY_WOWLAN_GTK_REKEY_FAILURE) &&
	     nla_put_flag(msg, NL80211_WOWLAN_TRIG_GTK_REKEY_FAILURE)) ||
	    ((dev->wiphy.wowlan->flags & WIPHY_WOWLAN_EAP_IDENTITY_REQ) &&
	     nla_put_flag(msg, NL80211_WOWLAN_TRIG_EAP_IDENT_REQUEST)) ||
	    ((dev->wiphy.wowlan->flags & WIPHY_WOWLAN_4WAY_HANDSHAKE) &&
	     nla_put_flag(msg, NL80211_WOWLAN_TRIG_4WAY_HANDSHAKE)) ||
	    ((dev->wiphy.wowlan->flags & WIPHY_WOWLAN_RFKILL_RELEASE) &&
	     nla_put_flag(msg, NL80211_WOWLAN_TRIG_RFKILL_RELEASE)))
		return -ENOBUFS;

	if (dev->wiphy.wowlan->n_patterns) {
		struct nl80211_pattern_support pat = {
			.max_patterns = dev->wiphy.wowlan->n_patterns,
			.min_pattern_len = dev->wiphy.wowlan->pattern_min_len,
			.max_pattern_len = dev->wiphy.wowlan->pattern_max_len,
			.max_pkt_offset = dev->wiphy.wowlan->max_pkt_offset,
		};

		if (nla_put(msg, NL80211_WOWLAN_TRIG_PKT_PATTERN,
			    sizeof(pat), &pat))
			return -ENOBUFS;
	}

	if (large && nl80211_send_wowlan_tcp_caps(dev, msg))
		return -ENOBUFS;

	nla_nest_end(msg, nl_wowlan);

	return 0;
}
#endif

static int nl80211_send_coalesce(struct sk_buff *msg,
				 struct cfg80211_registered_device *dev)
{
	struct nl80211_coalesce_rule_support rule;

	if (!dev->wiphy.coalesce)
		return 0;

	rule.max_rules = dev->wiphy.coalesce->n_rules;
	rule.max_delay = dev->wiphy.coalesce->max_delay;
	rule.pat.max_patterns = dev->wiphy.coalesce->n_patterns;
	rule.pat.min_pattern_len = dev->wiphy.coalesce->pattern_min_len;
	rule.pat.max_pattern_len = dev->wiphy.coalesce->pattern_max_len;
	rule.pat.max_pkt_offset = dev->wiphy.coalesce->max_pkt_offset;

	if (nla_put(msg, NL80211_ATTR_COALESCE_RULE, sizeof(rule), &rule))
		return -ENOBUFS;

	return 0;
}

static int nl80211_send_band_rateinfo(struct sk_buff *msg,
				      struct ieee80211_supported_band *sband)
{
	struct nlattr *nl_rates, *nl_rate;
	struct ieee80211_rate *rate;
	int i;

	/* add HT info */
	if (sband->ht_cap.ht_supported &&
	    (nla_put(msg, NL80211_BAND_ATTR_HT_MCS_SET,
		     sizeof(sband->ht_cap.mcs),
		     &sband->ht_cap.mcs) ||
	     nla_put_u16(msg, NL80211_BAND_ATTR_HT_CAPA,
			 sband->ht_cap.cap) ||
	     nla_put_u8(msg, NL80211_BAND_ATTR_HT_AMPDU_FACTOR,
			sband->ht_cap.ampdu_factor) ||
	     nla_put_u8(msg, NL80211_BAND_ATTR_HT_AMPDU_DENSITY,
			sband->ht_cap.ampdu_density)))
		return -ENOBUFS;

	/* add VHT info */
	if (sband->vht_cap.vht_supported &&
	    (nla_put(msg, NL80211_BAND_ATTR_VHT_MCS_SET,
		     sizeof(sband->vht_cap.vht_mcs),
		     &sband->vht_cap.vht_mcs) ||
	     nla_put_u32(msg, NL80211_BAND_ATTR_VHT_CAPA,
			 sband->vht_cap.cap)))
		return -ENOBUFS;

	/* add bitrates */
	nl_rates = nla_nest_start(msg, NL80211_BAND_ATTR_RATES);
	if (!nl_rates)
		return -ENOBUFS;

	for (i = 0; i < sband->n_bitrates; i++) {
		nl_rate = nla_nest_start(msg, i);
		if (!nl_rate)
			return -ENOBUFS;

		rate = &sband->bitrates[i];
		if (nla_put_u32(msg, NL80211_BITRATE_ATTR_RATE,
				rate->bitrate))
			return -ENOBUFS;
		if ((rate->flags & IEEE80211_RATE_SHORT_PREAMBLE) &&
		    nla_put_flag(msg,
				 NL80211_BITRATE_ATTR_2GHZ_SHORTPREAMBLE))
			return -ENOBUFS;

		nla_nest_end(msg, nl_rate);
	}

	nla_nest_end(msg, nl_rates);

	return 0;
}

static int
nl80211_send_mgmt_stypes(struct sk_buff *msg,
			 const struct ieee80211_txrx_stypes *mgmt_stypes)
{
	u16 stypes;
	struct nlattr *nl_ftypes, *nl_ifs;
	enum nl80211_iftype ift;
	int i;

	if (!mgmt_stypes)
		return 0;

	nl_ifs = nla_nest_start(msg, NL80211_ATTR_TX_FRAME_TYPES);
	if (!nl_ifs)
		return -ENOBUFS;

	for (ift = 0; ift < NUM_NL80211_IFTYPES; ift++) {
		nl_ftypes = nla_nest_start(msg, ift);
		if (!nl_ftypes)
			return -ENOBUFS;
		i = 0;
		stypes = mgmt_stypes[ift].tx;
		while (stypes) {
			if ((stypes & 1) &&
			    nla_put_u16(msg, NL80211_ATTR_FRAME_TYPE,
					(i << 4) | IEEE80211_FTYPE_MGMT))
				return -ENOBUFS;
			stypes >>= 1;
			i++;
		}
		nla_nest_end(msg, nl_ftypes);
	}

	nla_nest_end(msg, nl_ifs);

	nl_ifs = nla_nest_start(msg, NL80211_ATTR_RX_FRAME_TYPES);
	if (!nl_ifs)
		return -ENOBUFS;

	for (ift = 0; ift < NUM_NL80211_IFTYPES; ift++) {
		nl_ftypes = nla_nest_start(msg, ift);
		if (!nl_ftypes)
			return -ENOBUFS;
		i = 0;
		stypes = mgmt_stypes[ift].rx;
		while (stypes) {
			if ((stypes & 1) &&
			    nla_put_u16(msg, NL80211_ATTR_FRAME_TYPE,
					(i << 4) | IEEE80211_FTYPE_MGMT))
				return -ENOBUFS;
			stypes >>= 1;
			i++;
		}
		nla_nest_end(msg, nl_ftypes);
	}
	nla_nest_end(msg, nl_ifs);

	return 0;
}

struct nl80211_dump_wiphy_state {
	s64 filter_wiphy;
	long start;
	long split_start, band_start, chan_start;
	bool split;
};

static int nl80211_send_wiphy(struct cfg80211_registered_device *dev,
			      struct sk_buff *msg, u32 portid, u32 seq,
			      int flags, struct nl80211_dump_wiphy_state *state)
{
	void *hdr;
	struct nlattr *nl_bands, *nl_band;
	struct nlattr *nl_freqs, *nl_freq;
	struct nlattr *nl_cmds;
	enum ieee80211_band band;
	struct ieee80211_channel *chan;
	int i;
	const struct ieee80211_txrx_stypes *mgmt_stypes =
				dev->wiphy.mgmt_stypes;
	u32 features;

	hdr = nl80211hdr_put(msg, portid, seq, flags, NL80211_CMD_NEW_WIPHY);
	if (!hdr)
		return -ENOBUFS;

	if (WARN_ON(!state))
		return -EINVAL;

	if (nla_put_u32(msg, NL80211_ATTR_WIPHY, dev->wiphy_idx) ||
	    nla_put_string(msg, NL80211_ATTR_WIPHY_NAME,
			   wiphy_name(&dev->wiphy)) ||
	    nla_put_u32(msg, NL80211_ATTR_GENERATION,
			cfg80211_rdev_list_generation))
		goto nla_put_failure;

	switch (state->split_start) {
	case 0:
		if (nla_put_u8(msg, NL80211_ATTR_WIPHY_RETRY_SHORT,
			       dev->wiphy.retry_short) ||
		    nla_put_u8(msg, NL80211_ATTR_WIPHY_RETRY_LONG,
			       dev->wiphy.retry_long) ||
		    nla_put_u32(msg, NL80211_ATTR_WIPHY_FRAG_THRESHOLD,
				dev->wiphy.frag_threshold) ||
		    nla_put_u32(msg, NL80211_ATTR_WIPHY_RTS_THRESHOLD,
				dev->wiphy.rts_threshold) ||
		    nla_put_u8(msg, NL80211_ATTR_WIPHY_COVERAGE_CLASS,
			       dev->wiphy.coverage_class) ||
		    nla_put_u8(msg, NL80211_ATTR_MAX_NUM_SCAN_SSIDS,
			       dev->wiphy.max_scan_ssids) ||
		    nla_put_u8(msg, NL80211_ATTR_MAX_NUM_SCHED_SCAN_SSIDS,
			       dev->wiphy.max_sched_scan_ssids) ||
		    nla_put_u16(msg, NL80211_ATTR_MAX_SCAN_IE_LEN,
				dev->wiphy.max_scan_ie_len) ||
		    nla_put_u16(msg, NL80211_ATTR_MAX_SCHED_SCAN_IE_LEN,
				dev->wiphy.max_sched_scan_ie_len) ||
		    nla_put_u8(msg, NL80211_ATTR_MAX_MATCH_SETS,
			       dev->wiphy.max_match_sets))
			goto nla_put_failure;

		if ((dev->wiphy.flags & WIPHY_FLAG_IBSS_RSN) &&
		    nla_put_flag(msg, NL80211_ATTR_SUPPORT_IBSS_RSN))
			goto nla_put_failure;
		if ((dev->wiphy.flags & WIPHY_FLAG_MESH_AUTH) &&
		    nla_put_flag(msg, NL80211_ATTR_SUPPORT_MESH_AUTH))
			goto nla_put_failure;
		if ((dev->wiphy.flags & WIPHY_FLAG_AP_UAPSD) &&
		    nla_put_flag(msg, NL80211_ATTR_SUPPORT_AP_UAPSD))
			goto nla_put_failure;
		if ((dev->wiphy.flags & WIPHY_FLAG_SUPPORTS_FW_ROAM) &&
		    nla_put_flag(msg, NL80211_ATTR_ROAM_SUPPORT))
			goto nla_put_failure;
		if ((dev->wiphy.flags & WIPHY_FLAG_SUPPORTS_TDLS) &&
		    nla_put_flag(msg, NL80211_ATTR_TDLS_SUPPORT))
			goto nla_put_failure;
		if ((dev->wiphy.flags & WIPHY_FLAG_TDLS_EXTERNAL_SETUP) &&
		    nla_put_flag(msg, NL80211_ATTR_TDLS_EXTERNAL_SETUP))
			goto nla_put_failure;
		if ((dev->wiphy.flags & WIPHY_FLAG_SUPPORTS_5_10_MHZ) &&
		    nla_put_flag(msg, WIPHY_FLAG_SUPPORTS_5_10_MHZ))
			goto nla_put_failure;

		state->split_start++;
		if (state->split)
			break;
	case 1:
		if (nla_put(msg, NL80211_ATTR_CIPHER_SUITES,
			    sizeof(u32) * dev->wiphy.n_cipher_suites,
			    dev->wiphy.cipher_suites))
			goto nla_put_failure;

		if (nla_put_u8(msg, NL80211_ATTR_MAX_NUM_PMKIDS,
			       dev->wiphy.max_num_pmkids))
			goto nla_put_failure;

		if ((dev->wiphy.flags & WIPHY_FLAG_CONTROL_PORT_PROTOCOL) &&
		    nla_put_flag(msg, NL80211_ATTR_CONTROL_PORT_ETHERTYPE))
			goto nla_put_failure;

		if (nla_put_u32(msg, NL80211_ATTR_WIPHY_ANTENNA_AVAIL_TX,
				dev->wiphy.available_antennas_tx) ||
		    nla_put_u32(msg, NL80211_ATTR_WIPHY_ANTENNA_AVAIL_RX,
				dev->wiphy.available_antennas_rx))
			goto nla_put_failure;

		if ((dev->wiphy.flags & WIPHY_FLAG_AP_PROBE_RESP_OFFLOAD) &&
		    nla_put_u32(msg, NL80211_ATTR_PROBE_RESP_OFFLOAD,
				dev->wiphy.probe_resp_offload))
			goto nla_put_failure;

		if ((dev->wiphy.available_antennas_tx ||
		     dev->wiphy.available_antennas_rx) &&
		    dev->ops->get_antenna) {
			u32 tx_ant = 0, rx_ant = 0;
			int res;
			res = rdev_get_antenna(dev, &tx_ant, &rx_ant);
			if (!res) {
				if (nla_put_u32(msg,
						NL80211_ATTR_WIPHY_ANTENNA_TX,
						tx_ant) ||
				    nla_put_u32(msg,
						NL80211_ATTR_WIPHY_ANTENNA_RX,
						rx_ant))
					goto nla_put_failure;
			}
		}

		state->split_start++;
		if (state->split)
			break;
	case 2:
		if (nl80211_put_iftypes(msg, NL80211_ATTR_SUPPORTED_IFTYPES,
					dev->wiphy.interface_modes))
				goto nla_put_failure;
		state->split_start++;
		if (state->split)
			break;
	case 3:
		nl_bands = nla_nest_start(msg, NL80211_ATTR_WIPHY_BANDS);
		if (!nl_bands)
			goto nla_put_failure;

		for (band = state->band_start;
		     band < IEEE80211_NUM_BANDS; band++) {
			struct ieee80211_supported_band *sband;

			sband = dev->wiphy.bands[band];

			if (!sband)
				continue;

			nl_band = nla_nest_start(msg, band);
			if (!nl_band)
				goto nla_put_failure;

			switch (state->chan_start) {
			case 0:
				if (nl80211_send_band_rateinfo(msg, sband))
					goto nla_put_failure;
				state->chan_start++;
				if (state->split)
					break;
			default:
				/* add frequencies */
				nl_freqs = nla_nest_start(
					msg, NL80211_BAND_ATTR_FREQS);
				if (!nl_freqs)
					goto nla_put_failure;

				for (i = state->chan_start - 1;
				     i < sband->n_channels;
				     i++) {
					nl_freq = nla_nest_start(msg, i);
					if (!nl_freq)
						goto nla_put_failure;

					chan = &sband->channels[i];

					if (nl80211_msg_put_channel(
							msg, chan,
							state->split))
						goto nla_put_failure;

					nla_nest_end(msg, nl_freq);
					if (state->split)
						break;
				}
				if (i < sband->n_channels)
					state->chan_start = i + 2;
				else
					state->chan_start = 0;
				nla_nest_end(msg, nl_freqs);
			}

			nla_nest_end(msg, nl_band);

			if (state->split) {
				/* start again here */
				if (state->chan_start)
					band--;
				break;
			}
		}
		nla_nest_end(msg, nl_bands);

		if (band < IEEE80211_NUM_BANDS)
			state->band_start = band + 1;
		else
			state->band_start = 0;

		/* if bands & channels are done, continue outside */
		if (state->band_start == 0 && state->chan_start == 0)
			state->split_start++;
		if (state->split)
			break;
	case 4:
		nl_cmds = nla_nest_start(msg, NL80211_ATTR_SUPPORTED_COMMANDS);
		if (!nl_cmds)
			goto nla_put_failure;

		i = 0;
#define CMD(op, n)							\
		 do {							\
			if (dev->ops->op) {				\
				i++;					\
				if (nla_put_u32(msg, i, NL80211_CMD_ ## n)) \
					goto nla_put_failure;		\
			}						\
		} while (0)

		CMD(add_virtual_intf, NEW_INTERFACE);
		CMD(change_virtual_intf, SET_INTERFACE);
		CMD(add_key, NEW_KEY);
		CMD(start_ap, START_AP);
		CMD(add_station, NEW_STATION);
		CMD(add_mpath, NEW_MPATH);
		CMD(update_mesh_config, SET_MESH_CONFIG);
		CMD(change_bss, SET_BSS);
		CMD(auth, AUTHENTICATE);
		CMD(assoc, ASSOCIATE);
		CMD(deauth, DEAUTHENTICATE);
		CMD(disassoc, DISASSOCIATE);
		CMD(join_ibss, JOIN_IBSS);
		CMD(join_mesh, JOIN_MESH);
		CMD(set_pmksa, SET_PMKSA);
		CMD(del_pmksa, DEL_PMKSA);
		CMD(flush_pmksa, FLUSH_PMKSA);
		if (dev->wiphy.flags & WIPHY_FLAG_HAS_REMAIN_ON_CHANNEL)
			CMD(remain_on_channel, REMAIN_ON_CHANNEL);
		CMD(set_bitrate_mask, SET_TX_BITRATE_MASK);
		CMD(mgmt_tx, FRAME);
		CMD(mgmt_tx_cancel_wait, FRAME_WAIT_CANCEL);
		if (dev->wiphy.flags & WIPHY_FLAG_NETNS_OK) {
			i++;
			if (nla_put_u32(msg, i, NL80211_CMD_SET_WIPHY_NETNS))
				goto nla_put_failure;
		}
		if (dev->ops->set_monitor_channel || dev->ops->start_ap ||
		    dev->ops->join_mesh) {
			i++;
			if (nla_put_u32(msg, i, NL80211_CMD_SET_CHANNEL))
				goto nla_put_failure;
		}
		CMD(set_wds_peer, SET_WDS_PEER);
		if (dev->wiphy.flags & WIPHY_FLAG_SUPPORTS_TDLS) {
			CMD(tdls_mgmt, TDLS_MGMT);
			CMD(tdls_oper, TDLS_OPER);
		}
		if (dev->wiphy.flags & WIPHY_FLAG_SUPPORTS_SCHED_SCAN)
			CMD(sched_scan_start, START_SCHED_SCAN);
		CMD(probe_client, PROBE_CLIENT);
		CMD(set_noack_map, SET_NOACK_MAP);
		if (dev->wiphy.flags & WIPHY_FLAG_REPORTS_OBSS) {
			i++;
			if (nla_put_u32(msg, i, NL80211_CMD_REGISTER_BEACONS))
				goto nla_put_failure;
		}
		CMD(start_p2p_device, START_P2P_DEVICE);
		CMD(set_mcast_rate, SET_MCAST_RATE);
		if (state->split) {
			CMD(crit_proto_start, CRIT_PROTOCOL_START);
			CMD(crit_proto_stop, CRIT_PROTOCOL_STOP);
			if (dev->wiphy.flags & WIPHY_FLAG_HAS_CHANNEL_SWITCH)
				CMD(channel_switch, CHANNEL_SWITCH);
		}

#ifdef CONFIG_NL80211_TESTMODE
		CMD(testmode_cmd, TESTMODE);
#endif

#undef CMD

		if (dev->ops->connect || dev->ops->auth) {
			i++;
			if (nla_put_u32(msg, i, NL80211_CMD_CONNECT))
				goto nla_put_failure;
		}

		if (dev->ops->disconnect || dev->ops->deauth) {
			i++;
			if (nla_put_u32(msg, i, NL80211_CMD_DISCONNECT))
				goto nla_put_failure;
		}

		nla_nest_end(msg, nl_cmds);
		state->split_start++;
		if (state->split)
			break;
	case 5:
		if (dev->ops->remain_on_channel &&
		    (dev->wiphy.flags & WIPHY_FLAG_HAS_REMAIN_ON_CHANNEL) &&
		    nla_put_u32(msg,
				NL80211_ATTR_MAX_REMAIN_ON_CHANNEL_DURATION,
				dev->wiphy.max_remain_on_channel_duration))
			goto nla_put_failure;

		if ((dev->wiphy.flags & WIPHY_FLAG_OFFCHAN_TX) &&
		    nla_put_flag(msg, NL80211_ATTR_OFFCHANNEL_TX_OK))
			goto nla_put_failure;

		if (nl80211_send_mgmt_stypes(msg, mgmt_stypes))
			goto nla_put_failure;
		state->split_start++;
		if (state->split)
			break;
	case 6:
#ifdef CONFIG_PM
		if (nl80211_send_wowlan(msg, dev, state->split))
			goto nla_put_failure;
		state->split_start++;
		if (state->split)
			break;
#else
		state->split_start++;
#endif
	case 7:
		if (nl80211_put_iftypes(msg, NL80211_ATTR_SOFTWARE_IFTYPES,
					dev->wiphy.software_iftypes))
			goto nla_put_failure;

		if (nl80211_put_iface_combinations(&dev->wiphy, msg,
						   state->split))
			goto nla_put_failure;

		state->split_start++;
		if (state->split)
			break;
	case 8:
		if ((dev->wiphy.flags & WIPHY_FLAG_HAVE_AP_SME) &&
		    nla_put_u32(msg, NL80211_ATTR_DEVICE_AP_SME,
				dev->wiphy.ap_sme_capa))
			goto nla_put_failure;

		features = dev->wiphy.features;
		/*
		 * We can only add the per-channel limit information if the
		 * dump is split, otherwise it makes it too big. Therefore
		 * only advertise it in that case.
		 */
		if (state->split)
			features |= NL80211_FEATURE_ADVERTISE_CHAN_LIMITS;
		if (nla_put_u32(msg, NL80211_ATTR_FEATURE_FLAGS, features))
			goto nla_put_failure;

		if (dev->wiphy.ht_capa_mod_mask &&
		    nla_put(msg, NL80211_ATTR_HT_CAPABILITY_MASK,
			    sizeof(*dev->wiphy.ht_capa_mod_mask),
			    dev->wiphy.ht_capa_mod_mask))
			goto nla_put_failure;

		if (dev->wiphy.flags & WIPHY_FLAG_HAVE_AP_SME &&
		    dev->wiphy.max_acl_mac_addrs &&
		    nla_put_u32(msg, NL80211_ATTR_MAC_ACL_MAX,
				dev->wiphy.max_acl_mac_addrs))
			goto nla_put_failure;

		/*
		 * Any information below this point is only available to
		 * applications that can deal with it being split. This
		 * helps ensure that newly added capabilities don't break
		 * older tools by overrunning their buffers.
		 *
		 * We still increment split_start so that in the split
		 * case we'll continue with more data in the next round,
		 * but break unconditionally so unsplit data stops here.
		 */
		state->split_start++;
		break;
	case 9:
		if (dev->wiphy.extended_capabilities &&
		    (nla_put(msg, NL80211_ATTR_EXT_CAPA,
			     dev->wiphy.extended_capabilities_len,
			     dev->wiphy.extended_capabilities) ||
		     nla_put(msg, NL80211_ATTR_EXT_CAPA_MASK,
			     dev->wiphy.extended_capabilities_len,
			     dev->wiphy.extended_capabilities_mask)))
			goto nla_put_failure;

		if (dev->wiphy.vht_capa_mod_mask &&
		    nla_put(msg, NL80211_ATTR_VHT_CAPABILITY_MASK,
			    sizeof(*dev->wiphy.vht_capa_mod_mask),
			    dev->wiphy.vht_capa_mod_mask))
			goto nla_put_failure;

		state->split_start++;
		break;
	case 10:
		if (nl80211_send_coalesce(msg, dev))
			goto nla_put_failure;

		/* done */
		state->split_start = 0;
		break;
	}
	return genlmsg_end(msg, hdr);

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	return -EMSGSIZE;
}

static int nl80211_dump_wiphy_parse(struct sk_buff *skb,
				    struct netlink_callback *cb,
				    struct nl80211_dump_wiphy_state *state)
{
	struct nlattr **tb = nl80211_fam.attrbuf;
	int ret = nlmsg_parse(cb->nlh, GENL_HDRLEN + nl80211_fam.hdrsize,
			      tb, nl80211_fam.maxattr, nl80211_policy);
	/* ignore parse errors for backward compatibility */
	if (ret)
		return 0;

	state->split = tb[NL80211_ATTR_SPLIT_WIPHY_DUMP];
	if (tb[NL80211_ATTR_WIPHY])
		state->filter_wiphy = nla_get_u32(tb[NL80211_ATTR_WIPHY]);
	if (tb[NL80211_ATTR_WDEV])
		state->filter_wiphy = nla_get_u64(tb[NL80211_ATTR_WDEV]) >> 32;
	if (tb[NL80211_ATTR_IFINDEX]) {
		struct net_device *netdev;
		struct cfg80211_registered_device *rdev;
		int ifidx = nla_get_u32(tb[NL80211_ATTR_IFINDEX]);

		netdev = dev_get_by_index(sock_net(skb->sk), ifidx);
		if (!netdev)
			return -ENODEV;
		if (netdev->ieee80211_ptr) {
			rdev = wiphy_to_dev(
				netdev->ieee80211_ptr->wiphy);
			state->filter_wiphy = rdev->wiphy_idx;
		}
		dev_put(netdev);
	}

	return 0;
}

static int nl80211_dump_wiphy(struct sk_buff *skb, struct netlink_callback *cb)
{
	int idx = 0, ret;
	struct nl80211_dump_wiphy_state *state = (void *)cb->args[0];
	struct cfg80211_registered_device *dev;

	rtnl_lock();
	if (!state) {
		state = kzalloc(sizeof(*state), GFP_KERNEL);
		if (!state) {
			rtnl_unlock();
			return -ENOMEM;
		}
		state->filter_wiphy = -1;
		ret = nl80211_dump_wiphy_parse(skb, cb, state);
		if (ret) {
			kfree(state);
			rtnl_unlock();
			return ret;
		}
		cb->args[0] = (long)state;
	}

	list_for_each_entry(dev, &cfg80211_rdev_list, list) {
		if (!net_eq(wiphy_net(&dev->wiphy), sock_net(skb->sk)))
			continue;
		if (++idx <= state->start)
			continue;
		if (state->filter_wiphy != -1 &&
		    state->filter_wiphy != dev->wiphy_idx)
			continue;
		/* attempt to fit multiple wiphy data chunks into the skb */
		do {
			ret = nl80211_send_wiphy(dev, skb,
						 NETLINK_CB(cb->skb).portid,
						 cb->nlh->nlmsg_seq,
						 NLM_F_MULTI, state);
			if (ret < 0) {
				/*
				 * If sending the wiphy data didn't fit (ENOBUFS
				 * or EMSGSIZE returned), this SKB is still
				 * empty (so it's not too big because another
				 * wiphy dataset is already in the skb) and
				 * we've not tried to adjust the dump allocation
				 * yet ... then adjust the alloc size to be
				 * bigger, and return 1 but with the empty skb.
				 * This results in an empty message being RX'ed
				 * in userspace, but that is ignored.
				 *
				 * We can then retry with the larger buffer.
				 */
				if ((ret == -ENOBUFS || ret == -EMSGSIZE) &&
				    !skb->len &&
				    cb->min_dump_alloc < 4096) {
					cb->min_dump_alloc = 4096;
					rtnl_unlock();
					return 1;
				}
				idx--;
				break;
			}
		} while (state->split_start > 0);
		break;
	}
	rtnl_unlock();

	state->start = idx;
>>>>>>> refs/remotes/origin/master

	return skb->len;
}

<<<<<<< HEAD
=======
static int nl80211_dump_wiphy_done(struct netlink_callback *cb)
{
	kfree((void *)cb->args[0]);
	return 0;
}

>>>>>>> refs/remotes/origin/master
static int nl80211_get_wiphy(struct sk_buff *skb, struct genl_info *info)
{
	struct sk_buff *msg;
	struct cfg80211_registered_device *dev = info->user_ptr[0];
<<<<<<< HEAD

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (!msg)
		return -ENOMEM;

	if (nl80211_send_wiphy(msg, info->snd_pid, info->snd_seq, 0, dev) < 0) {
=======
	struct nl80211_dump_wiphy_state state = {};

	msg = nlmsg_new(4096, GFP_KERNEL);
	if (!msg)
		return -ENOMEM;

	if (nl80211_send_wiphy(dev, msg, info->snd_portid, info->snd_seq, 0,
			       &state) < 0) {
>>>>>>> refs/remotes/origin/master
		nlmsg_free(msg);
		return -ENOBUFS;
	}

	return genlmsg_reply(msg, info);
}

static const struct nla_policy txq_params_policy[NL80211_TXQ_ATTR_MAX + 1] = {
	[NL80211_TXQ_ATTR_QUEUE]		= { .type = NLA_U8 },
	[NL80211_TXQ_ATTR_TXOP]			= { .type = NLA_U16 },
	[NL80211_TXQ_ATTR_CWMIN]		= { .type = NLA_U16 },
	[NL80211_TXQ_ATTR_CWMAX]		= { .type = NLA_U16 },
	[NL80211_TXQ_ATTR_AIFS]			= { .type = NLA_U8 },
};

static int parse_txq_params(struct nlattr *tb[],
			    struct ieee80211_txq_params *txq_params)
{
<<<<<<< HEAD
	if (!tb[NL80211_TXQ_ATTR_QUEUE] || !tb[NL80211_TXQ_ATTR_TXOP] ||
=======
	if (!tb[NL80211_TXQ_ATTR_AC] || !tb[NL80211_TXQ_ATTR_TXOP] ||
>>>>>>> refs/remotes/origin/master
	    !tb[NL80211_TXQ_ATTR_CWMIN] || !tb[NL80211_TXQ_ATTR_CWMAX] ||
	    !tb[NL80211_TXQ_ATTR_AIFS])
		return -EINVAL;

<<<<<<< HEAD
	txq_params->queue = nla_get_u8(tb[NL80211_TXQ_ATTR_QUEUE]);
=======
	txq_params->ac = nla_get_u8(tb[NL80211_TXQ_ATTR_AC]);
>>>>>>> refs/remotes/origin/master
	txq_params->txop = nla_get_u16(tb[NL80211_TXQ_ATTR_TXOP]);
	txq_params->cwmin = nla_get_u16(tb[NL80211_TXQ_ATTR_CWMIN]);
	txq_params->cwmax = nla_get_u16(tb[NL80211_TXQ_ATTR_CWMAX]);
	txq_params->aifs = nla_get_u8(tb[NL80211_TXQ_ATTR_AIFS]);

<<<<<<< HEAD
=======
	if (txq_params->ac >= NL80211_NUM_ACS)
		return -EINVAL;

>>>>>>> refs/remotes/origin/master
	return 0;
}

static bool nl80211_can_set_dev_channel(struct wireless_dev *wdev)
{
	/*
<<<<<<< HEAD
	 * You can only set the channel explicitly for AP, mesh
	 * and WDS type interfaces; all others have their channel
	 * managed via their respective "establish a connection"
	 * command (connect, join, ...)
	 *
	 * Monitors are special as they are normally slaved to
	 * whatever else is going on, so they behave as though
	 * you tried setting the wiphy channel itself.
	 */
	return !wdev ||
		wdev->iftype == NL80211_IFTYPE_AP ||
		wdev->iftype == NL80211_IFTYPE_WDS ||
=======
	 * You can only set the channel explicitly for WDS interfaces,
	 * all others have their channel managed via their respective
	 * "establish a connection" command (connect, join, ...)
	 *
	 * For AP/GO and mesh mode, the channel can be set with the
	 * channel userspace API, but is only stored and passed to the
	 * low-level driver when the AP starts or the mesh is joined.
	 * This is for backward compatibility, userspace can also give
	 * the channel in the start-ap or join-mesh commands instead.
	 *
	 * Monitors are special as they are normally slaved to
	 * whatever else is going on, so they have their own special
	 * operation to set the monitor channel if possible.
	 */
	return !wdev ||
		wdev->iftype == NL80211_IFTYPE_AP ||
>>>>>>> refs/remotes/origin/master
		wdev->iftype == NL80211_IFTYPE_MESH_POINT ||
		wdev->iftype == NL80211_IFTYPE_MONITOR ||
		wdev->iftype == NL80211_IFTYPE_P2P_GO;
}

<<<<<<< HEAD
=======
static int nl80211_parse_chandef(struct cfg80211_registered_device *rdev,
				 struct genl_info *info,
				 struct cfg80211_chan_def *chandef)
{
	u32 control_freq;

	if (!info->attrs[NL80211_ATTR_WIPHY_FREQ])
		return -EINVAL;

	control_freq = nla_get_u32(info->attrs[NL80211_ATTR_WIPHY_FREQ]);

	chandef->chan = ieee80211_get_channel(&rdev->wiphy, control_freq);
	chandef->width = NL80211_CHAN_WIDTH_20_NOHT;
	chandef->center_freq1 = control_freq;
	chandef->center_freq2 = 0;

	/* Primary channel not allowed */
	if (!chandef->chan || chandef->chan->flags & IEEE80211_CHAN_DISABLED)
		return -EINVAL;

	if (info->attrs[NL80211_ATTR_WIPHY_CHANNEL_TYPE]) {
		enum nl80211_channel_type chantype;

		chantype = nla_get_u32(
				info->attrs[NL80211_ATTR_WIPHY_CHANNEL_TYPE]);

		switch (chantype) {
		case NL80211_CHAN_NO_HT:
		case NL80211_CHAN_HT20:
		case NL80211_CHAN_HT40PLUS:
		case NL80211_CHAN_HT40MINUS:
			cfg80211_chandef_create(chandef, chandef->chan,
						chantype);
			break;
		default:
			return -EINVAL;
		}
	} else if (info->attrs[NL80211_ATTR_CHANNEL_WIDTH]) {
		chandef->width =
			nla_get_u32(info->attrs[NL80211_ATTR_CHANNEL_WIDTH]);
		if (info->attrs[NL80211_ATTR_CENTER_FREQ1])
			chandef->center_freq1 =
				nla_get_u32(
					info->attrs[NL80211_ATTR_CENTER_FREQ1]);
		if (info->attrs[NL80211_ATTR_CENTER_FREQ2])
			chandef->center_freq2 =
				nla_get_u32(
					info->attrs[NL80211_ATTR_CENTER_FREQ2]);
	}

	if (!cfg80211_chandef_valid(chandef))
		return -EINVAL;

	if (!cfg80211_chandef_usable(&rdev->wiphy, chandef,
				     IEEE80211_CHAN_DISABLED))
		return -EINVAL;

	if ((chandef->width == NL80211_CHAN_WIDTH_5 ||
	     chandef->width == NL80211_CHAN_WIDTH_10) &&
	    !(rdev->wiphy.flags & WIPHY_FLAG_SUPPORTS_5_10_MHZ))
		return -EINVAL;

	return 0;
}

>>>>>>> refs/remotes/origin/master
static int __nl80211_set_channel(struct cfg80211_registered_device *rdev,
				 struct wireless_dev *wdev,
				 struct genl_info *info)
{
<<<<<<< HEAD
	enum nl80211_channel_type channel_type = NL80211_CHAN_NO_HT;
	u32 freq;
	int result;

	if (!info->attrs[NL80211_ATTR_WIPHY_FREQ])
		return -EINVAL;
=======
	struct cfg80211_chan_def chandef;
	int result;
	enum nl80211_iftype iftype = NL80211_IFTYPE_MONITOR;

	if (wdev)
		iftype = wdev->iftype;
>>>>>>> refs/remotes/origin/master

	if (!nl80211_can_set_dev_channel(wdev))
		return -EOPNOTSUPP;

<<<<<<< HEAD
	if (info->attrs[NL80211_ATTR_WIPHY_CHANNEL_TYPE]) {
		channel_type = nla_get_u32(info->attrs[
				   NL80211_ATTR_WIPHY_CHANNEL_TYPE]);
		if (channel_type != NL80211_CHAN_NO_HT &&
		    channel_type != NL80211_CHAN_HT20 &&
		    channel_type != NL80211_CHAN_HT40PLUS &&
		    channel_type != NL80211_CHAN_HT40MINUS)
			return -EINVAL;
	}

	freq = nla_get_u32(info->attrs[NL80211_ATTR_WIPHY_FREQ]);

	mutex_lock(&rdev->devlist_mtx);
	if (wdev) {
		wdev_lock(wdev);
		result = cfg80211_set_freq(rdev, wdev, freq, channel_type);
		wdev_unlock(wdev);
	} else {
		result = cfg80211_set_freq(rdev, NULL, freq, channel_type);
	}
	mutex_unlock(&rdev->devlist_mtx);
=======
	result = nl80211_parse_chandef(rdev, info, &chandef);
	if (result)
		return result;

	switch (iftype) {
	case NL80211_IFTYPE_AP:
	case NL80211_IFTYPE_P2P_GO:
		if (wdev->beacon_interval) {
			result = -EBUSY;
			break;
		}
		if (!cfg80211_reg_can_beacon(&rdev->wiphy, &chandef)) {
			result = -EINVAL;
			break;
		}
		wdev->preset_chandef = chandef;
		result = 0;
		break;
	case NL80211_IFTYPE_MESH_POINT:
		result = cfg80211_set_mesh_channel(rdev, wdev, &chandef);
		break;
	case NL80211_IFTYPE_MONITOR:
		result = cfg80211_set_monitor_channel(rdev, &chandef);
		break;
	default:
		result = -EINVAL;
	}
>>>>>>> refs/remotes/origin/master

	return result;
}

static int nl80211_set_channel(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *netdev = info->user_ptr[1];

	return __nl80211_set_channel(rdev, netdev->ieee80211_ptr, info);
}

static int nl80211_set_wds_peer(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	const u8 *bssid;

	if (!info->attrs[NL80211_ATTR_MAC])
		return -EINVAL;

	if (netif_running(dev))
		return -EBUSY;

	if (!rdev->ops->set_wds_peer)
		return -EOPNOTSUPP;

	if (wdev->iftype != NL80211_IFTYPE_WDS)
		return -EOPNOTSUPP;

	bssid = nla_data(info->attrs[NL80211_ATTR_MAC]);
<<<<<<< HEAD
	return rdev->ops->set_wds_peer(wdev->wiphy, dev, bssid);
=======
	return rdev_set_wds_peer(rdev, dev, bssid);
>>>>>>> refs/remotes/origin/master
}


static int nl80211_set_wiphy(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev;
	struct net_device *netdev = NULL;
	struct wireless_dev *wdev;
	int result = 0, rem_txq_params = 0;
	struct nlattr *nl_txq_params;
	u32 changed;
	u8 retry_short = 0, retry_long = 0;
	u32 frag_threshold = 0, rts_threshold = 0;
	u8 coverage_class = 0;

<<<<<<< HEAD
=======
	ASSERT_RTNL();

>>>>>>> refs/remotes/origin/master
	/*
	 * Try to find the wiphy and netdev. Normally this
	 * function shouldn't need the netdev, but this is
	 * done for backward compatibility -- previously
	 * setting the channel was done per wiphy, but now
	 * it is per netdev. Previous userland like hostapd
	 * also passed a netdev to set_wiphy, so that it is
	 * possible to let that go to the right netdev!
	 */
<<<<<<< HEAD
	mutex_lock(&cfg80211_mutex);
=======
>>>>>>> refs/remotes/origin/master

	if (info->attrs[NL80211_ATTR_IFINDEX]) {
		int ifindex = nla_get_u32(info->attrs[NL80211_ATTR_IFINDEX]);

		netdev = dev_get_by_index(genl_info_net(info), ifindex);
<<<<<<< HEAD
		if (netdev && netdev->ieee80211_ptr) {
			rdev = wiphy_to_dev(netdev->ieee80211_ptr->wiphy);
			mutex_lock(&rdev->mtx);
		} else
=======
		if (netdev && netdev->ieee80211_ptr)
			rdev = wiphy_to_dev(netdev->ieee80211_ptr->wiphy);
		else
>>>>>>> refs/remotes/origin/master
			netdev = NULL;
	}

	if (!netdev) {
<<<<<<< HEAD
		rdev = __cfg80211_rdev_from_info(info);
		if (IS_ERR(rdev)) {
			mutex_unlock(&cfg80211_mutex);
			return PTR_ERR(rdev);
		}
		wdev = NULL;
		netdev = NULL;
		result = 0;

		mutex_lock(&rdev->mtx);
	} else if (netif_running(netdev) &&
		   nl80211_can_set_dev_channel(netdev->ieee80211_ptr))
		wdev = netdev->ieee80211_ptr;
	else
		wdev = NULL;
=======
		rdev = __cfg80211_rdev_from_attrs(genl_info_net(info),
						  info->attrs);
		if (IS_ERR(rdev))
			return PTR_ERR(rdev);
		wdev = NULL;
		netdev = NULL;
		result = 0;
	} else
		wdev = netdev->ieee80211_ptr;
>>>>>>> refs/remotes/origin/master

	/*
	 * end workaround code, by now the rdev is available
	 * and locked, and wdev may or may not be NULL.
	 */

	if (info->attrs[NL80211_ATTR_WIPHY_NAME])
		result = cfg80211_dev_rename(
			rdev, nla_data(info->attrs[NL80211_ATTR_WIPHY_NAME]));

<<<<<<< HEAD
	mutex_unlock(&cfg80211_mutex);

=======
>>>>>>> refs/remotes/origin/master
	if (result)
		goto bad_res;

	if (info->attrs[NL80211_ATTR_WIPHY_TXQ_PARAMS]) {
		struct ieee80211_txq_params txq_params;
		struct nlattr *tb[NL80211_TXQ_ATTR_MAX + 1];

		if (!rdev->ops->set_txq_params) {
			result = -EOPNOTSUPP;
			goto bad_res;
		}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
		if (!netdev) {
			result = -EINVAL;
			goto bad_res;
		}

		if (netdev->ieee80211_ptr->iftype != NL80211_IFTYPE_AP &&
		    netdev->ieee80211_ptr->iftype != NL80211_IFTYPE_P2P_GO) {
			result = -EINVAL;
			goto bad_res;
		}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		if (!netif_running(netdev)) {
			result = -ENETDOWN;
			goto bad_res;
		}

		nla_for_each_nested(nl_txq_params,
				    info->attrs[NL80211_ATTR_WIPHY_TXQ_PARAMS],
				    rem_txq_params) {
			nla_parse(tb, NL80211_TXQ_ATTR_MAX,
				  nla_data(nl_txq_params),
				  nla_len(nl_txq_params),
				  txq_params_policy);
			result = parse_txq_params(tb, &txq_params);
			if (result)
				goto bad_res;

<<<<<<< HEAD
			result = rdev->ops->set_txq_params(&rdev->wiphy,
<<<<<<< HEAD
=======
							   netdev,
>>>>>>> refs/remotes/origin/cm-10.0
							   &txq_params);
=======
			result = rdev_set_txq_params(rdev, netdev,
						     &txq_params);
>>>>>>> refs/remotes/origin/master
			if (result)
				goto bad_res;
		}
	}

	if (info->attrs[NL80211_ATTR_WIPHY_FREQ]) {
<<<<<<< HEAD
		result = __nl80211_set_channel(rdev, wdev, info);
=======
		result = __nl80211_set_channel(rdev,
				nl80211_can_set_dev_channel(wdev) ? wdev : NULL,
				info);
>>>>>>> refs/remotes/origin/master
		if (result)
			goto bad_res;
	}

	if (info->attrs[NL80211_ATTR_WIPHY_TX_POWER_SETTING]) {
<<<<<<< HEAD
		enum nl80211_tx_power_setting type;
		int idx, mbm = 0;

=======
		struct wireless_dev *txp_wdev = wdev;
		enum nl80211_tx_power_setting type;
		int idx, mbm = 0;

		if (!(rdev->wiphy.features & NL80211_FEATURE_VIF_TXPOWER))
			txp_wdev = NULL;

>>>>>>> refs/remotes/origin/master
		if (!rdev->ops->set_tx_power) {
			result = -EOPNOTSUPP;
			goto bad_res;
		}

		idx = NL80211_ATTR_WIPHY_TX_POWER_SETTING;
		type = nla_get_u32(info->attrs[idx]);

		if (!info->attrs[NL80211_ATTR_WIPHY_TX_POWER_LEVEL] &&
		    (type != NL80211_TX_POWER_AUTOMATIC)) {
			result = -EINVAL;
			goto bad_res;
		}

		if (type != NL80211_TX_POWER_AUTOMATIC) {
			idx = NL80211_ATTR_WIPHY_TX_POWER_LEVEL;
			mbm = nla_get_u32(info->attrs[idx]);
		}

<<<<<<< HEAD
		result = rdev->ops->set_tx_power(&rdev->wiphy, type, mbm);
=======
		result = rdev_set_tx_power(rdev, txp_wdev, type, mbm);
>>>>>>> refs/remotes/origin/master
		if (result)
			goto bad_res;
	}

	if (info->attrs[NL80211_ATTR_WIPHY_ANTENNA_TX] &&
	    info->attrs[NL80211_ATTR_WIPHY_ANTENNA_RX]) {
		u32 tx_ant, rx_ant;
		if ((!rdev->wiphy.available_antennas_tx &&
		     !rdev->wiphy.available_antennas_rx) ||
		    !rdev->ops->set_antenna) {
			result = -EOPNOTSUPP;
			goto bad_res;
		}

		tx_ant = nla_get_u32(info->attrs[NL80211_ATTR_WIPHY_ANTENNA_TX]);
		rx_ant = nla_get_u32(info->attrs[NL80211_ATTR_WIPHY_ANTENNA_RX]);

		/* reject antenna configurations which don't match the
		 * available antenna masks, except for the "all" mask */
		if ((~tx_ant && (tx_ant & ~rdev->wiphy.available_antennas_tx)) ||
		    (~rx_ant && (rx_ant & ~rdev->wiphy.available_antennas_rx))) {
			result = -EINVAL;
			goto bad_res;
		}

		tx_ant = tx_ant & rdev->wiphy.available_antennas_tx;
		rx_ant = rx_ant & rdev->wiphy.available_antennas_rx;

<<<<<<< HEAD
		result = rdev->ops->set_antenna(&rdev->wiphy, tx_ant, rx_ant);
=======
		result = rdev_set_antenna(rdev, tx_ant, rx_ant);
>>>>>>> refs/remotes/origin/master
		if (result)
			goto bad_res;
	}

	changed = 0;

	if (info->attrs[NL80211_ATTR_WIPHY_RETRY_SHORT]) {
		retry_short = nla_get_u8(
			info->attrs[NL80211_ATTR_WIPHY_RETRY_SHORT]);
		if (retry_short == 0) {
			result = -EINVAL;
			goto bad_res;
		}
		changed |= WIPHY_PARAM_RETRY_SHORT;
	}

	if (info->attrs[NL80211_ATTR_WIPHY_RETRY_LONG]) {
		retry_long = nla_get_u8(
			info->attrs[NL80211_ATTR_WIPHY_RETRY_LONG]);
		if (retry_long == 0) {
			result = -EINVAL;
			goto bad_res;
		}
		changed |= WIPHY_PARAM_RETRY_LONG;
	}

	if (info->attrs[NL80211_ATTR_WIPHY_FRAG_THRESHOLD]) {
		frag_threshold = nla_get_u32(
			info->attrs[NL80211_ATTR_WIPHY_FRAG_THRESHOLD]);
		if (frag_threshold < 256) {
			result = -EINVAL;
			goto bad_res;
		}
		if (frag_threshold != (u32) -1) {
			/*
			 * Fragments (apart from the last one) are required to
			 * have even length. Make the fragmentation code
			 * simpler by stripping LSB should someone try to use
			 * odd threshold value.
			 */
			frag_threshold &= ~0x1;
		}
		changed |= WIPHY_PARAM_FRAG_THRESHOLD;
	}

	if (info->attrs[NL80211_ATTR_WIPHY_RTS_THRESHOLD]) {
		rts_threshold = nla_get_u32(
			info->attrs[NL80211_ATTR_WIPHY_RTS_THRESHOLD]);
		changed |= WIPHY_PARAM_RTS_THRESHOLD;
	}

	if (info->attrs[NL80211_ATTR_WIPHY_COVERAGE_CLASS]) {
		coverage_class = nla_get_u8(
			info->attrs[NL80211_ATTR_WIPHY_COVERAGE_CLASS]);
		changed |= WIPHY_PARAM_COVERAGE_CLASS;
	}

	if (changed) {
		u8 old_retry_short, old_retry_long;
		u32 old_frag_threshold, old_rts_threshold;
		u8 old_coverage_class;

		if (!rdev->ops->set_wiphy_params) {
			result = -EOPNOTSUPP;
			goto bad_res;
		}

		old_retry_short = rdev->wiphy.retry_short;
		old_retry_long = rdev->wiphy.retry_long;
		old_frag_threshold = rdev->wiphy.frag_threshold;
		old_rts_threshold = rdev->wiphy.rts_threshold;
		old_coverage_class = rdev->wiphy.coverage_class;

		if (changed & WIPHY_PARAM_RETRY_SHORT)
			rdev->wiphy.retry_short = retry_short;
		if (changed & WIPHY_PARAM_RETRY_LONG)
			rdev->wiphy.retry_long = retry_long;
		if (changed & WIPHY_PARAM_FRAG_THRESHOLD)
			rdev->wiphy.frag_threshold = frag_threshold;
		if (changed & WIPHY_PARAM_RTS_THRESHOLD)
			rdev->wiphy.rts_threshold = rts_threshold;
		if (changed & WIPHY_PARAM_COVERAGE_CLASS)
			rdev->wiphy.coverage_class = coverage_class;

<<<<<<< HEAD
		result = rdev->ops->set_wiphy_params(&rdev->wiphy, changed);
=======
		result = rdev_set_wiphy_params(rdev, changed);
>>>>>>> refs/remotes/origin/master
		if (result) {
			rdev->wiphy.retry_short = old_retry_short;
			rdev->wiphy.retry_long = old_retry_long;
			rdev->wiphy.frag_threshold = old_frag_threshold;
			rdev->wiphy.rts_threshold = old_rts_threshold;
			rdev->wiphy.coverage_class = old_coverage_class;
		}
	}

 bad_res:
<<<<<<< HEAD
	mutex_unlock(&rdev->mtx);
=======
>>>>>>> refs/remotes/origin/master
	if (netdev)
		dev_put(netdev);
	return result;
}

<<<<<<< HEAD

static int nl80211_send_iface(struct sk_buff *msg, u32 pid, u32 seq, int flags,
			      struct cfg80211_registered_device *rdev,
			      struct net_device *dev)
{
	void *hdr;

	hdr = nl80211hdr_put(msg, pid, seq, flags, NL80211_CMD_NEW_INTERFACE);
	if (!hdr)
		return -1;

	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, dev->ifindex);
	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx);
	NLA_PUT_STRING(msg, NL80211_ATTR_IFNAME, dev->name);
	NLA_PUT_U32(msg, NL80211_ATTR_IFTYPE, dev->ieee80211_ptr->iftype);

	NLA_PUT_U32(msg, NL80211_ATTR_GENERATION,
		    rdev->devlist_generation ^
			(cfg80211_rdev_list_generation << 2));
=======
static inline u64 wdev_id(struct wireless_dev *wdev)
{
	return (u64)wdev->identifier |
	       ((u64)wiphy_to_dev(wdev->wiphy)->wiphy_idx << 32);
}

static int nl80211_send_chandef(struct sk_buff *msg,
				 struct cfg80211_chan_def *chandef)
{
	WARN_ON(!cfg80211_chandef_valid(chandef));

	if (nla_put_u32(msg, NL80211_ATTR_WIPHY_FREQ,
			chandef->chan->center_freq))
		return -ENOBUFS;
	switch (chandef->width) {
	case NL80211_CHAN_WIDTH_20_NOHT:
	case NL80211_CHAN_WIDTH_20:
	case NL80211_CHAN_WIDTH_40:
		if (nla_put_u32(msg, NL80211_ATTR_WIPHY_CHANNEL_TYPE,
				cfg80211_get_chandef_type(chandef)))
			return -ENOBUFS;
		break;
	default:
		break;
	}
	if (nla_put_u32(msg, NL80211_ATTR_CHANNEL_WIDTH, chandef->width))
		return -ENOBUFS;
	if (nla_put_u32(msg, NL80211_ATTR_CENTER_FREQ1, chandef->center_freq1))
		return -ENOBUFS;
	if (chandef->center_freq2 &&
	    nla_put_u32(msg, NL80211_ATTR_CENTER_FREQ2, chandef->center_freq2))
		return -ENOBUFS;
	return 0;
}

static int nl80211_send_iface(struct sk_buff *msg, u32 portid, u32 seq, int flags,
			      struct cfg80211_registered_device *rdev,
			      struct wireless_dev *wdev)
{
	struct net_device *dev = wdev->netdev;
	void *hdr;

	hdr = nl80211hdr_put(msg, portid, seq, flags, NL80211_CMD_NEW_INTERFACE);
	if (!hdr)
		return -1;

	if (dev &&
	    (nla_put_u32(msg, NL80211_ATTR_IFINDEX, dev->ifindex) ||
	     nla_put_string(msg, NL80211_ATTR_IFNAME, dev->name)))
		goto nla_put_failure;

	if (nla_put_u32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx) ||
	    nla_put_u32(msg, NL80211_ATTR_IFTYPE, wdev->iftype) ||
	    nla_put_u64(msg, NL80211_ATTR_WDEV, wdev_id(wdev)) ||
	    nla_put(msg, NL80211_ATTR_MAC, ETH_ALEN, wdev_address(wdev)) ||
	    nla_put_u32(msg, NL80211_ATTR_GENERATION,
			rdev->devlist_generation ^
			(cfg80211_rdev_list_generation << 2)))
		goto nla_put_failure;

	if (rdev->ops->get_channel) {
		int ret;
		struct cfg80211_chan_def chandef;

		ret = rdev_get_channel(rdev, wdev, &chandef);
		if (ret == 0) {
			if (nl80211_send_chandef(msg, &chandef))
				goto nla_put_failure;
		}
	}

	if (wdev->ssid_len) {
		if (nla_put(msg, NL80211_ATTR_SSID, wdev->ssid_len, wdev->ssid))
			goto nla_put_failure;
	}
>>>>>>> refs/remotes/origin/master

	return genlmsg_end(msg, hdr);

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	return -EMSGSIZE;
}

static int nl80211_dump_interface(struct sk_buff *skb, struct netlink_callback *cb)
{
	int wp_idx = 0;
	int if_idx = 0;
	int wp_start = cb->args[0];
	int if_start = cb->args[1];
	struct cfg80211_registered_device *rdev;
	struct wireless_dev *wdev;

<<<<<<< HEAD
	mutex_lock(&cfg80211_mutex);
=======
	rtnl_lock();
>>>>>>> refs/remotes/origin/master
	list_for_each_entry(rdev, &cfg80211_rdev_list, list) {
		if (!net_eq(wiphy_net(&rdev->wiphy), sock_net(skb->sk)))
			continue;
		if (wp_idx < wp_start) {
			wp_idx++;
			continue;
		}
		if_idx = 0;

<<<<<<< HEAD
		mutex_lock(&rdev->devlist_mtx);
		list_for_each_entry(wdev, &rdev->netdev_list, list) {
=======
		list_for_each_entry(wdev, &rdev->wdev_list, list) {
>>>>>>> refs/remotes/origin/master
			if (if_idx < if_start) {
				if_idx++;
				continue;
			}
<<<<<<< HEAD
			if (nl80211_send_iface(skb, NETLINK_CB(cb->skb).pid,
					       cb->nlh->nlmsg_seq, NLM_F_MULTI,
					       rdev, wdev->netdev) < 0) {
				mutex_unlock(&rdev->devlist_mtx);
=======
			if (nl80211_send_iface(skb, NETLINK_CB(cb->skb).portid,
					       cb->nlh->nlmsg_seq, NLM_F_MULTI,
					       rdev, wdev) < 0) {
>>>>>>> refs/remotes/origin/master
				goto out;
			}
			if_idx++;
		}
<<<<<<< HEAD
		mutex_unlock(&rdev->devlist_mtx);
=======
>>>>>>> refs/remotes/origin/master

		wp_idx++;
	}
 out:
<<<<<<< HEAD
	mutex_unlock(&cfg80211_mutex);
=======
	rtnl_unlock();
>>>>>>> refs/remotes/origin/master

	cb->args[0] = wp_idx;
	cb->args[1] = if_idx;

	return skb->len;
}

static int nl80211_get_interface(struct sk_buff *skb, struct genl_info *info)
{
	struct sk_buff *msg;
	struct cfg80211_registered_device *dev = info->user_ptr[0];
<<<<<<< HEAD
	struct net_device *netdev = info->user_ptr[1];
=======
	struct wireless_dev *wdev = info->user_ptr[1];
>>>>>>> refs/remotes/origin/master

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (!msg)
		return -ENOMEM;

<<<<<<< HEAD
	if (nl80211_send_iface(msg, info->snd_pid, info->snd_seq, 0,
			       dev, netdev) < 0) {
=======
	if (nl80211_send_iface(msg, info->snd_portid, info->snd_seq, 0,
			       dev, wdev) < 0) {
>>>>>>> refs/remotes/origin/master
		nlmsg_free(msg);
		return -ENOBUFS;
	}

	return genlmsg_reply(msg, info);
}

static const struct nla_policy mntr_flags_policy[NL80211_MNTR_FLAG_MAX + 1] = {
	[NL80211_MNTR_FLAG_FCSFAIL] = { .type = NLA_FLAG },
	[NL80211_MNTR_FLAG_PLCPFAIL] = { .type = NLA_FLAG },
	[NL80211_MNTR_FLAG_CONTROL] = { .type = NLA_FLAG },
	[NL80211_MNTR_FLAG_OTHER_BSS] = { .type = NLA_FLAG },
	[NL80211_MNTR_FLAG_COOK_FRAMES] = { .type = NLA_FLAG },
<<<<<<< HEAD
=======
	[NL80211_MNTR_FLAG_ACTIVE] = { .type = NLA_FLAG },
>>>>>>> refs/remotes/origin/master
};

static int parse_monitor_flags(struct nlattr *nla, u32 *mntrflags)
{
	struct nlattr *flags[NL80211_MNTR_FLAG_MAX + 1];
	int flag;

	*mntrflags = 0;

	if (!nla)
		return -EINVAL;

	if (nla_parse_nested(flags, NL80211_MNTR_FLAG_MAX,
			     nla, mntr_flags_policy))
		return -EINVAL;

	for (flag = 1; flag <= NL80211_MNTR_FLAG_MAX; flag++)
		if (flags[flag])
			*mntrflags |= (1<<flag);

	return 0;
}

static int nl80211_valid_4addr(struct cfg80211_registered_device *rdev,
			       struct net_device *netdev, u8 use_4addr,
			       enum nl80211_iftype iftype)
{
	if (!use_4addr) {
		if (netdev && (netdev->priv_flags & IFF_BRIDGE_PORT))
			return -EBUSY;
		return 0;
	}

	switch (iftype) {
	case NL80211_IFTYPE_AP_VLAN:
		if (rdev->wiphy.flags & WIPHY_FLAG_4ADDR_AP)
			return 0;
		break;
	case NL80211_IFTYPE_STATION:
		if (rdev->wiphy.flags & WIPHY_FLAG_4ADDR_STATION)
			return 0;
		break;
	default:
		break;
	}

	return -EOPNOTSUPP;
}

static int nl80211_set_interface(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct vif_params params;
	int err;
	enum nl80211_iftype otype, ntype;
	struct net_device *dev = info->user_ptr[1];
	u32 _flags, *flags = NULL;
	bool change = false;

	memset(&params, 0, sizeof(params));

	otype = ntype = dev->ieee80211_ptr->iftype;

	if (info->attrs[NL80211_ATTR_IFTYPE]) {
		ntype = nla_get_u32(info->attrs[NL80211_ATTR_IFTYPE]);
		if (otype != ntype)
			change = true;
		if (ntype > NL80211_IFTYPE_MAX)
			return -EINVAL;
	}

	if (info->attrs[NL80211_ATTR_MESH_ID]) {
		struct wireless_dev *wdev = dev->ieee80211_ptr;

		if (ntype != NL80211_IFTYPE_MESH_POINT)
			return -EINVAL;
		if (netif_running(dev))
			return -EBUSY;

		wdev_lock(wdev);
		BUILD_BUG_ON(IEEE80211_MAX_SSID_LEN !=
			     IEEE80211_MAX_MESH_ID_LEN);
		wdev->mesh_id_up_len =
			nla_len(info->attrs[NL80211_ATTR_MESH_ID]);
		memcpy(wdev->ssid, nla_data(info->attrs[NL80211_ATTR_MESH_ID]),
		       wdev->mesh_id_up_len);
		wdev_unlock(wdev);
	}

	if (info->attrs[NL80211_ATTR_4ADDR]) {
		params.use_4addr = !!nla_get_u8(info->attrs[NL80211_ATTR_4ADDR]);
		change = true;
		err = nl80211_valid_4addr(rdev, dev, params.use_4addr, ntype);
		if (err)
			return err;
	} else {
		params.use_4addr = -1;
	}

	if (info->attrs[NL80211_ATTR_MNTR_FLAGS]) {
		if (ntype != NL80211_IFTYPE_MONITOR)
			return -EINVAL;
		err = parse_monitor_flags(info->attrs[NL80211_ATTR_MNTR_FLAGS],
					  &_flags);
		if (err)
			return err;

		flags = &_flags;
		change = true;
	}

<<<<<<< HEAD
=======
	if (flags && (*flags & MONITOR_FLAG_ACTIVE) &&
	    !(rdev->wiphy.features & NL80211_FEATURE_ACTIVE_MONITOR))
		return -EOPNOTSUPP;

>>>>>>> refs/remotes/origin/master
	if (change)
		err = cfg80211_change_iface(rdev, dev, ntype, flags, &params);
	else
		err = 0;

	if (!err && params.use_4addr != -1)
		dev->ieee80211_ptr->use_4addr = params.use_4addr;

	return err;
}

static int nl80211_new_interface(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct vif_params params;
<<<<<<< HEAD
	struct net_device *dev;
=======
	struct wireless_dev *wdev;
	struct sk_buff *msg;
>>>>>>> refs/remotes/origin/master
	int err;
	enum nl80211_iftype type = NL80211_IFTYPE_UNSPECIFIED;
	u32 flags;

	memset(&params, 0, sizeof(params));

	if (!info->attrs[NL80211_ATTR_IFNAME])
		return -EINVAL;

	if (info->attrs[NL80211_ATTR_IFTYPE]) {
		type = nla_get_u32(info->attrs[NL80211_ATTR_IFTYPE]);
		if (type > NL80211_IFTYPE_MAX)
			return -EINVAL;
	}

	if (!rdev->ops->add_virtual_intf ||
	    !(rdev->wiphy.interface_modes & (1 << type)))
		return -EOPNOTSUPP;

<<<<<<< HEAD
=======
	if (type == NL80211_IFTYPE_P2P_DEVICE && info->attrs[NL80211_ATTR_MAC]) {
		nla_memcpy(params.macaddr, info->attrs[NL80211_ATTR_MAC],
			   ETH_ALEN);
		if (!is_valid_ether_addr(params.macaddr))
			return -EADDRNOTAVAIL;
	}

>>>>>>> refs/remotes/origin/master
	if (info->attrs[NL80211_ATTR_4ADDR]) {
		params.use_4addr = !!nla_get_u8(info->attrs[NL80211_ATTR_4ADDR]);
		err = nl80211_valid_4addr(rdev, NULL, params.use_4addr, type);
		if (err)
			return err;
	}

<<<<<<< HEAD
	err = parse_monitor_flags(type == NL80211_IFTYPE_MONITOR ?
				  info->attrs[NL80211_ATTR_MNTR_FLAGS] : NULL,
				  &flags);
	dev = rdev->ops->add_virtual_intf(&rdev->wiphy,
		nla_data(info->attrs[NL80211_ATTR_IFNAME]),
		type, err ? NULL : &flags, &params);
	if (IS_ERR(dev))
		return PTR_ERR(dev);

	if (type == NL80211_IFTYPE_MESH_POINT &&
	    info->attrs[NL80211_ATTR_MESH_ID]) {
		struct wireless_dev *wdev = dev->ieee80211_ptr;

=======
	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (!msg)
		return -ENOMEM;

	err = parse_monitor_flags(type == NL80211_IFTYPE_MONITOR ?
				  info->attrs[NL80211_ATTR_MNTR_FLAGS] : NULL,
				  &flags);

	if (!err && (flags & MONITOR_FLAG_ACTIVE) &&
	    !(rdev->wiphy.features & NL80211_FEATURE_ACTIVE_MONITOR))
		return -EOPNOTSUPP;

	wdev = rdev_add_virtual_intf(rdev,
				nla_data(info->attrs[NL80211_ATTR_IFNAME]),
				type, err ? NULL : &flags, &params);
	if (IS_ERR(wdev)) {
		nlmsg_free(msg);
		return PTR_ERR(wdev);
	}

	switch (type) {
	case NL80211_IFTYPE_MESH_POINT:
		if (!info->attrs[NL80211_ATTR_MESH_ID])
			break;
>>>>>>> refs/remotes/origin/master
		wdev_lock(wdev);
		BUILD_BUG_ON(IEEE80211_MAX_SSID_LEN !=
			     IEEE80211_MAX_MESH_ID_LEN);
		wdev->mesh_id_up_len =
			nla_len(info->attrs[NL80211_ATTR_MESH_ID]);
		memcpy(wdev->ssid, nla_data(info->attrs[NL80211_ATTR_MESH_ID]),
		       wdev->mesh_id_up_len);
		wdev_unlock(wdev);
<<<<<<< HEAD
	}

	return 0;
=======
		break;
	case NL80211_IFTYPE_P2P_DEVICE:
		/*
		 * P2P Device doesn't have a netdev, so doesn't go
		 * through the netdev notifier and must be added here
		 */
		mutex_init(&wdev->mtx);
		INIT_LIST_HEAD(&wdev->event_list);
		spin_lock_init(&wdev->event_lock);
		INIT_LIST_HEAD(&wdev->mgmt_registrations);
		spin_lock_init(&wdev->mgmt_registrations_lock);

		wdev->identifier = ++rdev->wdev_id;
		list_add_rcu(&wdev->list, &rdev->wdev_list);
		rdev->devlist_generation++;
		break;
	default:
		break;
	}

	if (nl80211_send_iface(msg, info->snd_portid, info->snd_seq, 0,
			       rdev, wdev) < 0) {
		nlmsg_free(msg);
		return -ENOBUFS;
	}

	return genlmsg_reply(msg, info);
>>>>>>> refs/remotes/origin/master
}

static int nl80211_del_interface(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
<<<<<<< HEAD
	struct net_device *dev = info->user_ptr[1];
=======
	struct wireless_dev *wdev = info->user_ptr[1];
>>>>>>> refs/remotes/origin/master

	if (!rdev->ops->del_virtual_intf)
		return -EOPNOTSUPP;

<<<<<<< HEAD
	return rdev->ops->del_virtual_intf(&rdev->wiphy, dev);
}

<<<<<<< HEAD
=======
=======
	/*
	 * If we remove a wireless device without a netdev then clear
	 * user_ptr[1] so that nl80211_post_doit won't dereference it
	 * to check if it needs to do dev_put(). Otherwise it crashes
	 * since the wdev has been freed, unlike with a netdev where
	 * we need the dev_put() for the netdev to really be freed.
	 */
	if (!wdev->netdev)
		info->user_ptr[1] = NULL;

	return rdev_del_virtual_intf(rdev, wdev);
}

>>>>>>> refs/remotes/origin/master
static int nl80211_set_noack_map(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];
	u16 noack_map;

	if (!info->attrs[NL80211_ATTR_NOACK_MAP])
		return -EINVAL;

	if (!rdev->ops->set_noack_map)
		return -EOPNOTSUPP;

	noack_map = nla_get_u16(info->attrs[NL80211_ATTR_NOACK_MAP]);

<<<<<<< HEAD
	return rdev->ops->set_noack_map(&rdev->wiphy, dev, noack_map);
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
	return rdev_set_noack_map(rdev, dev, noack_map);
}

>>>>>>> refs/remotes/origin/master
struct get_key_cookie {
	struct sk_buff *msg;
	int error;
	int idx;
};

static void get_key_callback(void *c, struct key_params *params)
{
	struct nlattr *key;
	struct get_key_cookie *cookie = c;

<<<<<<< HEAD
	if (params->key)
		NLA_PUT(cookie->msg, NL80211_ATTR_KEY_DATA,
			params->key_len, params->key);

	if (params->seq)
		NLA_PUT(cookie->msg, NL80211_ATTR_KEY_SEQ,
			params->seq_len, params->seq);

	if (params->cipher)
		NLA_PUT_U32(cookie->msg, NL80211_ATTR_KEY_CIPHER,
			    params->cipher);
=======
	if ((params->key &&
	     nla_put(cookie->msg, NL80211_ATTR_KEY_DATA,
		     params->key_len, params->key)) ||
	    (params->seq &&
	     nla_put(cookie->msg, NL80211_ATTR_KEY_SEQ,
		     params->seq_len, params->seq)) ||
	    (params->cipher &&
	     nla_put_u32(cookie->msg, NL80211_ATTR_KEY_CIPHER,
			 params->cipher)))
		goto nla_put_failure;
>>>>>>> refs/remotes/origin/master

	key = nla_nest_start(cookie->msg, NL80211_ATTR_KEY);
	if (!key)
		goto nla_put_failure;

<<<<<<< HEAD
	if (params->key)
		NLA_PUT(cookie->msg, NL80211_KEY_DATA,
			params->key_len, params->key);

	if (params->seq)
		NLA_PUT(cookie->msg, NL80211_KEY_SEQ,
			params->seq_len, params->seq);

	if (params->cipher)
		NLA_PUT_U32(cookie->msg, NL80211_KEY_CIPHER,
			    params->cipher);

	NLA_PUT_U8(cookie->msg, NL80211_ATTR_KEY_IDX, cookie->idx);
=======
	if ((params->key &&
	     nla_put(cookie->msg, NL80211_KEY_DATA,
		     params->key_len, params->key)) ||
	    (params->seq &&
	     nla_put(cookie->msg, NL80211_KEY_SEQ,
		     params->seq_len, params->seq)) ||
	    (params->cipher &&
	     nla_put_u32(cookie->msg, NL80211_KEY_CIPHER,
			 params->cipher)))
		goto nla_put_failure;

	if (nla_put_u8(cookie->msg, NL80211_ATTR_KEY_IDX, cookie->idx))
		goto nla_put_failure;
>>>>>>> refs/remotes/origin/master

	nla_nest_end(cookie->msg, key);

	return;
 nla_put_failure:
	cookie->error = 1;
}

static int nl80211_get_key(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	int err;
	struct net_device *dev = info->user_ptr[1];
	u8 key_idx = 0;
	const u8 *mac_addr = NULL;
	bool pairwise;
	struct get_key_cookie cookie = {
		.error = 0,
	};
	void *hdr;
	struct sk_buff *msg;

	if (info->attrs[NL80211_ATTR_KEY_IDX])
		key_idx = nla_get_u8(info->attrs[NL80211_ATTR_KEY_IDX]);

	if (key_idx > 5)
		return -EINVAL;

	if (info->attrs[NL80211_ATTR_MAC])
		mac_addr = nla_data(info->attrs[NL80211_ATTR_MAC]);

	pairwise = !!mac_addr;
	if (info->attrs[NL80211_ATTR_KEY_TYPE]) {
		u32 kt = nla_get_u32(info->attrs[NL80211_ATTR_KEY_TYPE]);
		if (kt >= NUM_NL80211_KEYTYPES)
			return -EINVAL;
		if (kt != NL80211_KEYTYPE_GROUP &&
		    kt != NL80211_KEYTYPE_PAIRWISE)
			return -EINVAL;
		pairwise = kt == NL80211_KEYTYPE_PAIRWISE;
	}

	if (!rdev->ops->get_key)
		return -EOPNOTSUPP;

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (!msg)
		return -ENOMEM;

<<<<<<< HEAD
	hdr = nl80211hdr_put(msg, info->snd_pid, info->snd_seq, 0,
			     NL80211_CMD_NEW_KEY);
	if (IS_ERR(hdr))
		return PTR_ERR(hdr);
=======
	hdr = nl80211hdr_put(msg, info->snd_portid, info->snd_seq, 0,
			     NL80211_CMD_NEW_KEY);
	if (!hdr)
		goto nla_put_failure;
>>>>>>> refs/remotes/origin/master

	cookie.msg = msg;
	cookie.idx = key_idx;

<<<<<<< HEAD
	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, dev->ifindex);
	NLA_PUT_U8(msg, NL80211_ATTR_KEY_IDX, key_idx);
	if (mac_addr)
		NLA_PUT(msg, NL80211_ATTR_MAC, ETH_ALEN, mac_addr);
=======
	if (nla_put_u32(msg, NL80211_ATTR_IFINDEX, dev->ifindex) ||
	    nla_put_u8(msg, NL80211_ATTR_KEY_IDX, key_idx))
		goto nla_put_failure;
	if (mac_addr &&
	    nla_put(msg, NL80211_ATTR_MAC, ETH_ALEN, mac_addr))
		goto nla_put_failure;
>>>>>>> refs/remotes/origin/master

	if (pairwise && mac_addr &&
	    !(rdev->wiphy.flags & WIPHY_FLAG_IBSS_RSN))
		return -ENOENT;

<<<<<<< HEAD
	err = rdev->ops->get_key(&rdev->wiphy, dev, key_idx, pairwise,
				 mac_addr, &cookie, get_key_callback);
=======
	err = rdev_get_key(rdev, dev, key_idx, pairwise, mac_addr, &cookie,
			   get_key_callback);
>>>>>>> refs/remotes/origin/master

	if (err)
		goto free_msg;

	if (cookie.error)
		goto nla_put_failure;

	genlmsg_end(msg, hdr);
	return genlmsg_reply(msg, info);

 nla_put_failure:
	err = -ENOBUFS;
 free_msg:
	nlmsg_free(msg);
	return err;
}

static int nl80211_set_key(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct key_parse key;
	int err;
	struct net_device *dev = info->user_ptr[1];

	err = nl80211_parse_key(info, &key);
	if (err)
		return err;

	if (key.idx < 0)
		return -EINVAL;

	/* only support setting default key */
	if (!key.def && !key.defmgmt)
		return -EINVAL;

	wdev_lock(dev->ieee80211_ptr);

	if (key.def) {
		if (!rdev->ops->set_default_key) {
			err = -EOPNOTSUPP;
			goto out;
		}

		err = nl80211_key_allowed(dev->ieee80211_ptr);
		if (err)
			goto out;

<<<<<<< HEAD
		err = rdev->ops->set_default_key(&rdev->wiphy, dev, key.idx,
=======
		err = rdev_set_default_key(rdev, dev, key.idx,
>>>>>>> refs/remotes/origin/master
						 key.def_uni, key.def_multi);

		if (err)
			goto out;

#ifdef CONFIG_CFG80211_WEXT
		dev->ieee80211_ptr->wext.default_key = key.idx;
#endif
	} else {
		if (key.def_uni || !key.def_multi) {
			err = -EINVAL;
			goto out;
		}

		if (!rdev->ops->set_default_mgmt_key) {
			err = -EOPNOTSUPP;
			goto out;
		}

		err = nl80211_key_allowed(dev->ieee80211_ptr);
		if (err)
			goto out;

<<<<<<< HEAD
		err = rdev->ops->set_default_mgmt_key(&rdev->wiphy,
						      dev, key.idx);
=======
		err = rdev_set_default_mgmt_key(rdev, dev, key.idx);
>>>>>>> refs/remotes/origin/master
		if (err)
			goto out;

#ifdef CONFIG_CFG80211_WEXT
		dev->ieee80211_ptr->wext.default_mgmt_key = key.idx;
#endif
	}

 out:
	wdev_unlock(dev->ieee80211_ptr);

	return err;
}

static int nl80211_new_key(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	int err;
	struct net_device *dev = info->user_ptr[1];
	struct key_parse key;
	const u8 *mac_addr = NULL;

	err = nl80211_parse_key(info, &key);
	if (err)
		return err;

	if (!key.p.key)
		return -EINVAL;

	if (info->attrs[NL80211_ATTR_MAC])
		mac_addr = nla_data(info->attrs[NL80211_ATTR_MAC]);

	if (key.type == -1) {
		if (mac_addr)
			key.type = NL80211_KEYTYPE_PAIRWISE;
		else
			key.type = NL80211_KEYTYPE_GROUP;
	}

	/* for now */
	if (key.type != NL80211_KEYTYPE_PAIRWISE &&
	    key.type != NL80211_KEYTYPE_GROUP)
		return -EINVAL;

	if (!rdev->ops->add_key)
		return -EOPNOTSUPP;

	if (cfg80211_validate_key_settings(rdev, &key.p, key.idx,
					   key.type == NL80211_KEYTYPE_PAIRWISE,
					   mac_addr))
		return -EINVAL;

	wdev_lock(dev->ieee80211_ptr);
	err = nl80211_key_allowed(dev->ieee80211_ptr);
	if (!err)
<<<<<<< HEAD
		err = rdev->ops->add_key(&rdev->wiphy, dev, key.idx,
					 key.type == NL80211_KEYTYPE_PAIRWISE,
					 mac_addr, &key.p);
=======
		err = rdev_add_key(rdev, dev, key.idx,
				   key.type == NL80211_KEYTYPE_PAIRWISE,
				    mac_addr, &key.p);
>>>>>>> refs/remotes/origin/master
	wdev_unlock(dev->ieee80211_ptr);

	return err;
}

static int nl80211_del_key(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	int err;
	struct net_device *dev = info->user_ptr[1];
	u8 *mac_addr = NULL;
	struct key_parse key;

	err = nl80211_parse_key(info, &key);
	if (err)
		return err;

	if (info->attrs[NL80211_ATTR_MAC])
		mac_addr = nla_data(info->attrs[NL80211_ATTR_MAC]);

	if (key.type == -1) {
		if (mac_addr)
			key.type = NL80211_KEYTYPE_PAIRWISE;
		else
			key.type = NL80211_KEYTYPE_GROUP;
	}

	/* for now */
	if (key.type != NL80211_KEYTYPE_PAIRWISE &&
	    key.type != NL80211_KEYTYPE_GROUP)
		return -EINVAL;

	if (!rdev->ops->del_key)
		return -EOPNOTSUPP;

	wdev_lock(dev->ieee80211_ptr);
	err = nl80211_key_allowed(dev->ieee80211_ptr);

	if (key.type == NL80211_KEYTYPE_PAIRWISE && mac_addr &&
	    !(rdev->wiphy.flags & WIPHY_FLAG_IBSS_RSN))
		err = -ENOENT;

	if (!err)
<<<<<<< HEAD
		err = rdev->ops->del_key(&rdev->wiphy, dev, key.idx,
					 key.type == NL80211_KEYTYPE_PAIRWISE,
					 mac_addr);
=======
		err = rdev_del_key(rdev, dev, key.idx,
				   key.type == NL80211_KEYTYPE_PAIRWISE,
				   mac_addr);
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_CFG80211_WEXT
	if (!err) {
		if (key.idx == dev->ieee80211_ptr->wext.default_key)
			dev->ieee80211_ptr->wext.default_key = -1;
		else if (key.idx == dev->ieee80211_ptr->wext.default_mgmt_key)
			dev->ieee80211_ptr->wext.default_mgmt_key = -1;
	}
#endif
	wdev_unlock(dev->ieee80211_ptr);

	return err;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int nl80211_addset_beacon(struct sk_buff *skb, struct genl_info *info)
{
        int (*call)(struct wiphy *wiphy, struct net_device *dev,
		    struct beacon_parameters *info);
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct beacon_parameters params;
	int haveinfo = 0, err;
=======
static int nl80211_parse_beacon(struct genl_info *info,
				struct cfg80211_beacon_data *bcn)
{
	bool haveinfo = false;
>>>>>>> refs/remotes/origin/cm-10.0

	if (!is_valid_ie_attr(info->attrs[NL80211_ATTR_BEACON_TAIL]) ||
	    !is_valid_ie_attr(info->attrs[NL80211_ATTR_IE]) ||
	    !is_valid_ie_attr(info->attrs[NL80211_ATTR_IE_PROBE_RESP]) ||
	    !is_valid_ie_attr(info->attrs[NL80211_ATTR_IE_ASSOC_RESP]))
		return -EINVAL;

<<<<<<< HEAD
	if (dev->ieee80211_ptr->iftype != NL80211_IFTYPE_AP &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_P2P_GO)
		return -EOPNOTSUPP;

	memset(&params, 0, sizeof(params));

	switch (info->genlhdr->cmd) {
	case NL80211_CMD_NEW_BEACON:
		/* these are required for NEW_BEACON */
		if (!info->attrs[NL80211_ATTR_BEACON_INTERVAL] ||
		    !info->attrs[NL80211_ATTR_DTIM_PERIOD] ||
		    !info->attrs[NL80211_ATTR_BEACON_HEAD])
			return -EINVAL;

		params.interval =
			nla_get_u32(info->attrs[NL80211_ATTR_BEACON_INTERVAL]);
		params.dtim_period =
			nla_get_u32(info->attrs[NL80211_ATTR_DTIM_PERIOD]);

		err = cfg80211_validate_beacon_int(rdev, params.interval);
		if (err)
			return err;

		/*
		 * In theory, some of these attributes could be required for
		 * NEW_BEACON, but since they were not used when the command was
		 * originally added, keep them optional for old user space
		 * programs to work with drivers that do not need the additional
		 * information.
		 */
		if (info->attrs[NL80211_ATTR_SSID]) {
			params.ssid = nla_data(info->attrs[NL80211_ATTR_SSID]);
			params.ssid_len =
				nla_len(info->attrs[NL80211_ATTR_SSID]);
			if (params.ssid_len == 0 ||
			    params.ssid_len > IEEE80211_MAX_SSID_LEN)
				return -EINVAL;
		}

		if (info->attrs[NL80211_ATTR_HIDDEN_SSID]) {
			params.hidden_ssid = nla_get_u32(
				info->attrs[NL80211_ATTR_HIDDEN_SSID]);
			if (params.hidden_ssid !=
			    NL80211_HIDDEN_SSID_NOT_IN_USE &&
			    params.hidden_ssid !=
			    NL80211_HIDDEN_SSID_ZERO_LEN &&
			    params.hidden_ssid !=
			    NL80211_HIDDEN_SSID_ZERO_CONTENTS)
				return -EINVAL;
		}

		params.privacy = !!info->attrs[NL80211_ATTR_PRIVACY];

		if (info->attrs[NL80211_ATTR_AUTH_TYPE]) {
			params.auth_type = nla_get_u32(
				info->attrs[NL80211_ATTR_AUTH_TYPE]);
			if (!nl80211_valid_auth_type(params.auth_type))
				return -EINVAL;
		} else
			params.auth_type = NL80211_AUTHTYPE_AUTOMATIC;

		err = nl80211_crypto_settings(rdev, info, &params.crypto,
					      NL80211_MAX_NR_CIPHER_SUITES);
		if (err)
			return err;

		call = rdev->ops->add_beacon;
		break;
	case NL80211_CMD_SET_BEACON:
		call = rdev->ops->set_beacon;
		break;
	default:
		WARN_ON(1);
		return -EOPNOTSUPP;
	}

	if (!call)
		return -EOPNOTSUPP;

	if (info->attrs[NL80211_ATTR_BEACON_HEAD]) {
		params.head = nla_data(info->attrs[NL80211_ATTR_BEACON_HEAD]);
		params.head_len =
		    nla_len(info->attrs[NL80211_ATTR_BEACON_HEAD]);
		haveinfo = 1;
	}

	if (info->attrs[NL80211_ATTR_BEACON_TAIL]) {
		params.tail = nla_data(info->attrs[NL80211_ATTR_BEACON_TAIL]);
		params.tail_len =
		    nla_len(info->attrs[NL80211_ATTR_BEACON_TAIL]);
		haveinfo = 1;
=======
	memset(bcn, 0, sizeof(*bcn));

	if (info->attrs[NL80211_ATTR_BEACON_HEAD]) {
		bcn->head = nla_data(info->attrs[NL80211_ATTR_BEACON_HEAD]);
		bcn->head_len = nla_len(info->attrs[NL80211_ATTR_BEACON_HEAD]);
=======
/* This function returns an error or the number of nested attributes */
static int validate_acl_mac_addrs(struct nlattr *nl_attr)
{
	struct nlattr *attr;
	int n_entries = 0, tmp;

	nla_for_each_nested(attr, nl_attr, tmp) {
		if (nla_len(attr) != ETH_ALEN)
			return -EINVAL;

		n_entries++;
	}

	return n_entries;
}

/*
 * This function parses ACL information and allocates memory for ACL data.
 * On successful return, the calling function is responsible to free the
 * ACL buffer returned by this function.
 */
static struct cfg80211_acl_data *parse_acl_data(struct wiphy *wiphy,
						struct genl_info *info)
{
	enum nl80211_acl_policy acl_policy;
	struct nlattr *attr;
	struct cfg80211_acl_data *acl;
	int i = 0, n_entries, tmp;

	if (!wiphy->max_acl_mac_addrs)
		return ERR_PTR(-EOPNOTSUPP);

	if (!info->attrs[NL80211_ATTR_ACL_POLICY])
		return ERR_PTR(-EINVAL);

	acl_policy = nla_get_u32(info->attrs[NL80211_ATTR_ACL_POLICY]);
	if (acl_policy != NL80211_ACL_POLICY_ACCEPT_UNLESS_LISTED &&
	    acl_policy != NL80211_ACL_POLICY_DENY_UNLESS_LISTED)
		return ERR_PTR(-EINVAL);

	if (!info->attrs[NL80211_ATTR_MAC_ADDRS])
		return ERR_PTR(-EINVAL);

	n_entries = validate_acl_mac_addrs(info->attrs[NL80211_ATTR_MAC_ADDRS]);
	if (n_entries < 0)
		return ERR_PTR(n_entries);

	if (n_entries > wiphy->max_acl_mac_addrs)
		return ERR_PTR(-ENOTSUPP);

	acl = kzalloc(sizeof(*acl) + (sizeof(struct mac_address) * n_entries),
		      GFP_KERNEL);
	if (!acl)
		return ERR_PTR(-ENOMEM);

	nla_for_each_nested(attr, info->attrs[NL80211_ATTR_MAC_ADDRS], tmp) {
		memcpy(acl->mac_addrs[i].addr, nla_data(attr), ETH_ALEN);
		i++;
	}

	acl->n_acl_entries = n_entries;
	acl->acl_policy = acl_policy;

	return acl;
}

static int nl80211_set_mac_acl(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];
	struct cfg80211_acl_data *acl;
	int err;

	if (dev->ieee80211_ptr->iftype != NL80211_IFTYPE_AP &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_P2P_GO)
		return -EOPNOTSUPP;

	if (!dev->ieee80211_ptr->beacon_interval)
		return -EINVAL;

	acl = parse_acl_data(&rdev->wiphy, info);
	if (IS_ERR(acl))
		return PTR_ERR(acl);

	err = rdev_set_mac_acl(rdev, dev, acl);

	kfree(acl);

	return err;
}

static int nl80211_parse_beacon(struct nlattr *attrs[],
				struct cfg80211_beacon_data *bcn)
{
	bool haveinfo = false;

	if (!is_valid_ie_attr(attrs[NL80211_ATTR_BEACON_TAIL]) ||
	    !is_valid_ie_attr(attrs[NL80211_ATTR_IE]) ||
	    !is_valid_ie_attr(attrs[NL80211_ATTR_IE_PROBE_RESP]) ||
	    !is_valid_ie_attr(attrs[NL80211_ATTR_IE_ASSOC_RESP]))
		return -EINVAL;

	memset(bcn, 0, sizeof(*bcn));

	if (attrs[NL80211_ATTR_BEACON_HEAD]) {
		bcn->head = nla_data(attrs[NL80211_ATTR_BEACON_HEAD]);
		bcn->head_len = nla_len(attrs[NL80211_ATTR_BEACON_HEAD]);
>>>>>>> refs/remotes/origin/master
		if (!bcn->head_len)
			return -EINVAL;
		haveinfo = true;
	}

<<<<<<< HEAD
	if (info->attrs[NL80211_ATTR_BEACON_TAIL]) {
		bcn->tail = nla_data(info->attrs[NL80211_ATTR_BEACON_TAIL]);
		bcn->tail_len =
		    nla_len(info->attrs[NL80211_ATTR_BEACON_TAIL]);
		haveinfo = true;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (attrs[NL80211_ATTR_BEACON_TAIL]) {
		bcn->tail = nla_data(attrs[NL80211_ATTR_BEACON_TAIL]);
		bcn->tail_len = nla_len(attrs[NL80211_ATTR_BEACON_TAIL]);
		haveinfo = true;
>>>>>>> refs/remotes/origin/master
	}

	if (!haveinfo)
		return -EINVAL;

<<<<<<< HEAD
	if (info->attrs[NL80211_ATTR_IE]) {
<<<<<<< HEAD
		params.beacon_ies = nla_data(info->attrs[NL80211_ATTR_IE]);
		params.beacon_ies_len = nla_len(info->attrs[NL80211_ATTR_IE]);
	}

	if (info->attrs[NL80211_ATTR_IE_PROBE_RESP]) {
		params.proberesp_ies =
			nla_data(info->attrs[NL80211_ATTR_IE_PROBE_RESP]);
		params.proberesp_ies_len =
=======
		bcn->beacon_ies = nla_data(info->attrs[NL80211_ATTR_IE]);
		bcn->beacon_ies_len = nla_len(info->attrs[NL80211_ATTR_IE]);
	}

	if (info->attrs[NL80211_ATTR_IE_PROBE_RESP]) {
		bcn->proberesp_ies =
			nla_data(info->attrs[NL80211_ATTR_IE_PROBE_RESP]);
		bcn->proberesp_ies_len =
>>>>>>> refs/remotes/origin/cm-10.0
			nla_len(info->attrs[NL80211_ATTR_IE_PROBE_RESP]);
	}

	if (info->attrs[NL80211_ATTR_IE_ASSOC_RESP]) {
<<<<<<< HEAD
		params.assocresp_ies =
			nla_data(info->attrs[NL80211_ATTR_IE_ASSOC_RESP]);
		params.assocresp_ies_len =
			nla_len(info->attrs[NL80211_ATTR_IE_ASSOC_RESP]);
	}

	err = call(&rdev->wiphy, dev, &params);
	if (!err && params.interval)
		wdev->beacon_interval = params.interval;
	return err;
}

static int nl80211_del_beacon(struct sk_buff *skb, struct genl_info *info)
=======
		bcn->assocresp_ies =
			nla_data(info->attrs[NL80211_ATTR_IE_ASSOC_RESP]);
		bcn->assocresp_ies_len =
			nla_len(info->attrs[NL80211_ATTR_IE_ASSOC_RESP]);
	}

	if (info->attrs[NL80211_ATTR_PROBE_RESP]) {
		bcn->probe_resp =
			nla_data(info->attrs[NL80211_ATTR_PROBE_RESP]);
		bcn->probe_resp_len =
			nla_len(info->attrs[NL80211_ATTR_PROBE_RESP]);
=======
	if (attrs[NL80211_ATTR_IE]) {
		bcn->beacon_ies = nla_data(attrs[NL80211_ATTR_IE]);
		bcn->beacon_ies_len = nla_len(attrs[NL80211_ATTR_IE]);
	}

	if (attrs[NL80211_ATTR_IE_PROBE_RESP]) {
		bcn->proberesp_ies =
			nla_data(attrs[NL80211_ATTR_IE_PROBE_RESP]);
		bcn->proberesp_ies_len =
			nla_len(attrs[NL80211_ATTR_IE_PROBE_RESP]);
	}

	if (attrs[NL80211_ATTR_IE_ASSOC_RESP]) {
		bcn->assocresp_ies =
			nla_data(attrs[NL80211_ATTR_IE_ASSOC_RESP]);
		bcn->assocresp_ies_len =
			nla_len(attrs[NL80211_ATTR_IE_ASSOC_RESP]);
	}

	if (attrs[NL80211_ATTR_PROBE_RESP]) {
		bcn->probe_resp = nla_data(attrs[NL80211_ATTR_PROBE_RESP]);
		bcn->probe_resp_len = nla_len(attrs[NL80211_ATTR_PROBE_RESP]);
>>>>>>> refs/remotes/origin/master
	}

	return 0;
}

<<<<<<< HEAD
=======
static bool nl80211_get_ap_channel(struct cfg80211_registered_device *rdev,
				   struct cfg80211_ap_settings *params)
{
	struct wireless_dev *wdev;
	bool ret = false;

	list_for_each_entry(wdev, &rdev->wdev_list, list) {
		if (wdev->iftype != NL80211_IFTYPE_AP &&
		    wdev->iftype != NL80211_IFTYPE_P2P_GO)
			continue;

		if (!wdev->preset_chandef.chan)
			continue;

		params->chandef = wdev->preset_chandef;
		ret = true;
		break;
	}

	return ret;
}

static bool nl80211_valid_auth_type(struct cfg80211_registered_device *rdev,
				    enum nl80211_auth_type auth_type,
				    enum nl80211_commands cmd)
{
	if (auth_type > NL80211_AUTHTYPE_MAX)
		return false;

	switch (cmd) {
	case NL80211_CMD_AUTHENTICATE:
		if (!(rdev->wiphy.features & NL80211_FEATURE_SAE) &&
		    auth_type == NL80211_AUTHTYPE_SAE)
			return false;
		return true;
	case NL80211_CMD_CONNECT:
	case NL80211_CMD_START_AP:
		/* SAE not supported yet */
		if (auth_type == NL80211_AUTHTYPE_SAE)
			return false;
		return true;
	default:
		return false;
	}
}

>>>>>>> refs/remotes/origin/master
static int nl80211_start_ap(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct cfg80211_ap_settings params;
	int err;
<<<<<<< HEAD
=======
	u8 radar_detect_width = 0;
>>>>>>> refs/remotes/origin/master

	if (dev->ieee80211_ptr->iftype != NL80211_IFTYPE_AP &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_P2P_GO)
		return -EOPNOTSUPP;

	if (!rdev->ops->start_ap)
		return -EOPNOTSUPP;

	if (wdev->beacon_interval)
		return -EALREADY;

	memset(&params, 0, sizeof(params));

	/* these are required for START_AP */
	if (!info->attrs[NL80211_ATTR_BEACON_INTERVAL] ||
	    !info->attrs[NL80211_ATTR_DTIM_PERIOD] ||
	    !info->attrs[NL80211_ATTR_BEACON_HEAD])
		return -EINVAL;

<<<<<<< HEAD
	err = nl80211_parse_beacon(info, &params.beacon);
=======
	err = nl80211_parse_beacon(info->attrs, &params.beacon);
>>>>>>> refs/remotes/origin/master
	if (err)
		return err;

	params.beacon_interval =
		nla_get_u32(info->attrs[NL80211_ATTR_BEACON_INTERVAL]);
	params.dtim_period =
		nla_get_u32(info->attrs[NL80211_ATTR_DTIM_PERIOD]);

	err = cfg80211_validate_beacon_int(rdev, params.beacon_interval);
	if (err)
		return err;

	/*
	 * In theory, some of these attributes should be required here
	 * but since they were not used when the command was originally
	 * added, keep them optional for old user space programs to let
	 * them continue to work with drivers that do not need the
	 * additional information -- drivers must check!
	 */
	if (info->attrs[NL80211_ATTR_SSID]) {
		params.ssid = nla_data(info->attrs[NL80211_ATTR_SSID]);
		params.ssid_len =
			nla_len(info->attrs[NL80211_ATTR_SSID]);
		if (params.ssid_len == 0 ||
		    params.ssid_len > IEEE80211_MAX_SSID_LEN)
			return -EINVAL;
	}

	if (info->attrs[NL80211_ATTR_HIDDEN_SSID]) {
		params.hidden_ssid = nla_get_u32(
			info->attrs[NL80211_ATTR_HIDDEN_SSID]);
		if (params.hidden_ssid != NL80211_HIDDEN_SSID_NOT_IN_USE &&
		    params.hidden_ssid != NL80211_HIDDEN_SSID_ZERO_LEN &&
		    params.hidden_ssid != NL80211_HIDDEN_SSID_ZERO_CONTENTS)
			return -EINVAL;
	}

	params.privacy = !!info->attrs[NL80211_ATTR_PRIVACY];

	if (info->attrs[NL80211_ATTR_AUTH_TYPE]) {
		params.auth_type = nla_get_u32(
			info->attrs[NL80211_ATTR_AUTH_TYPE]);
<<<<<<< HEAD
		if (!nl80211_valid_auth_type(params.auth_type))
=======
		if (!nl80211_valid_auth_type(rdev, params.auth_type,
					     NL80211_CMD_START_AP))
>>>>>>> refs/remotes/origin/master
			return -EINVAL;
	} else
		params.auth_type = NL80211_AUTHTYPE_AUTOMATIC;

	err = nl80211_crypto_settings(rdev, info, &params.crypto,
				      NL80211_MAX_NR_CIPHER_SUITES);
	if (err)
		return err;

	if (info->attrs[NL80211_ATTR_INACTIVITY_TIMEOUT]) {
		if (!(rdev->wiphy.features & NL80211_FEATURE_INACTIVITY_TIMER))
			return -EOPNOTSUPP;
		params.inactivity_timeout = nla_get_u16(
			info->attrs[NL80211_ATTR_INACTIVITY_TIMEOUT]);
	}

<<<<<<< HEAD
	err = rdev->ops->start_ap(&rdev->wiphy, dev, &params);
	if (!err)
		wdev->beacon_interval = params.beacon_interval;
=======
	if (info->attrs[NL80211_ATTR_P2P_CTWINDOW]) {
		if (dev->ieee80211_ptr->iftype != NL80211_IFTYPE_P2P_GO)
			return -EINVAL;
		params.p2p_ctwindow =
			nla_get_u8(info->attrs[NL80211_ATTR_P2P_CTWINDOW]);
		if (params.p2p_ctwindow > 127)
			return -EINVAL;
		if (params.p2p_ctwindow != 0 &&
		    !(rdev->wiphy.features & NL80211_FEATURE_P2P_GO_CTWIN))
			return -EINVAL;
	}

	if (info->attrs[NL80211_ATTR_P2P_OPPPS]) {
		u8 tmp;

		if (dev->ieee80211_ptr->iftype != NL80211_IFTYPE_P2P_GO)
			return -EINVAL;
		tmp = nla_get_u8(info->attrs[NL80211_ATTR_P2P_OPPPS]);
		if (tmp > 1)
			return -EINVAL;
		params.p2p_opp_ps = tmp;
		if (params.p2p_opp_ps != 0 &&
		    !(rdev->wiphy.features & NL80211_FEATURE_P2P_GO_OPPPS))
			return -EINVAL;
	}

	if (info->attrs[NL80211_ATTR_WIPHY_FREQ]) {
		err = nl80211_parse_chandef(rdev, info, &params.chandef);
		if (err)
			return err;
	} else if (wdev->preset_chandef.chan) {
		params.chandef = wdev->preset_chandef;
	} else if (!nl80211_get_ap_channel(rdev, &params))
		return -EINVAL;

	if (!cfg80211_reg_can_beacon(&rdev->wiphy, &params.chandef))
		return -EINVAL;

	err = cfg80211_chandef_dfs_required(wdev->wiphy, &params.chandef);
	if (err < 0)
		return err;
	if (err) {
		radar_detect_width = BIT(params.chandef.width);
		params.radar_required = true;
	}

	err = cfg80211_can_use_iftype_chan(rdev, wdev, wdev->iftype,
					   params.chandef.chan,
					   CHAN_MODE_SHARED,
					   radar_detect_width);
	if (err)
		return err;

	if (info->attrs[NL80211_ATTR_ACL_POLICY]) {
		params.acl = parse_acl_data(&rdev->wiphy, info);
		if (IS_ERR(params.acl))
			return PTR_ERR(params.acl);
	}

	err = rdev_start_ap(rdev, dev, &params);
	if (!err) {
		wdev->preset_chandef = params.chandef;
		wdev->beacon_interval = params.beacon_interval;
		wdev->channel = params.chandef.chan;
		wdev->ssid_len = params.ssid_len;
		memcpy(wdev->ssid, params.ssid, wdev->ssid_len);
	}

	kfree(params.acl);

>>>>>>> refs/remotes/origin/master
	return err;
}

static int nl80211_set_beacon(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct cfg80211_beacon_data params;
	int err;

	if (dev->ieee80211_ptr->iftype != NL80211_IFTYPE_AP &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_P2P_GO)
		return -EOPNOTSUPP;

	if (!rdev->ops->change_beacon)
		return -EOPNOTSUPP;

	if (!wdev->beacon_interval)
		return -EINVAL;

<<<<<<< HEAD
	err = nl80211_parse_beacon(info, &params);
	if (err)
		return err;

	return rdev->ops->change_beacon(&rdev->wiphy, dev, &params);
}

static int nl80211_stop_ap(struct sk_buff *skb, struct genl_info *info)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	int err;

<<<<<<< HEAD
	if (!rdev->ops->del_beacon)
=======
	if (!rdev->ops->stop_ap)
>>>>>>> refs/remotes/origin/cm-10.0
		return -EOPNOTSUPP;

	if (dev->ieee80211_ptr->iftype != NL80211_IFTYPE_AP &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_P2P_GO)
		return -EOPNOTSUPP;

<<<<<<< HEAD
	err = rdev->ops->del_beacon(&rdev->wiphy, dev);
=======
	if (!wdev->beacon_interval)
		return -ENOENT;

	err = rdev->ops->stop_ap(&rdev->wiphy, dev);
>>>>>>> refs/remotes/origin/cm-10.0
	if (!err)
		wdev->beacon_interval = 0;
	return err;
=======
	err = nl80211_parse_beacon(info->attrs, &params);
	if (err)
		return err;

	return rdev_change_beacon(rdev, dev, &params);
}

static int nl80211_stop_ap(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];

	return cfg80211_stop_ap(rdev, dev);
>>>>>>> refs/remotes/origin/master
}

static const struct nla_policy sta_flags_policy[NL80211_STA_FLAG_MAX + 1] = {
	[NL80211_STA_FLAG_AUTHORIZED] = { .type = NLA_FLAG },
	[NL80211_STA_FLAG_SHORT_PREAMBLE] = { .type = NLA_FLAG },
	[NL80211_STA_FLAG_WME] = { .type = NLA_FLAG },
	[NL80211_STA_FLAG_MFP] = { .type = NLA_FLAG },
	[NL80211_STA_FLAG_AUTHENTICATED] = { .type = NLA_FLAG },
<<<<<<< HEAD
<<<<<<< HEAD
};

static int parse_station_flags(struct genl_info *info,
=======
=======
>>>>>>> refs/remotes/origin/master
	[NL80211_STA_FLAG_TDLS_PEER] = { .type = NLA_FLAG },
};

static int parse_station_flags(struct genl_info *info,
			       enum nl80211_iftype iftype,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			       struct station_parameters *params)
{
	struct nlattr *flags[NL80211_STA_FLAG_MAX + 1];
	struct nlattr *nla;
	int flag;

	/*
	 * Try parsing the new attribute first so userspace
	 * can specify both for older kernels.
	 */
	nla = info->attrs[NL80211_ATTR_STA_FLAGS2];
	if (nla) {
		struct nl80211_sta_flag_update *sta_flags;

		sta_flags = nla_data(nla);
		params->sta_flags_mask = sta_flags->mask;
		params->sta_flags_set = sta_flags->set;
<<<<<<< HEAD
=======
		params->sta_flags_set &= params->sta_flags_mask;
>>>>>>> refs/remotes/origin/master
		if ((params->sta_flags_mask |
		     params->sta_flags_set) & BIT(__NL80211_STA_FLAG_INVALID))
			return -EINVAL;
		return 0;
	}

	/* if present, parse the old attribute */

	nla = info->attrs[NL80211_ATTR_STA_FLAGS];
	if (!nla)
		return 0;

	if (nla_parse_nested(flags, NL80211_STA_FLAG_MAX,
			     nla, sta_flags_policy))
		return -EINVAL;

<<<<<<< HEAD
<<<<<<< HEAD
	params->sta_flags_mask = (1 << __NL80211_STA_FLAG_AFTER_LAST) - 1;
	params->sta_flags_mask &= ~1;
=======
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * Only allow certain flags for interface types so that
	 * other attributes are silently ignored. Remember that
	 * this is backward compatibility code with old userspace
	 * and shouldn't be hit in other cases anyway.
	 */
	switch (iftype) {
	case NL80211_IFTYPE_AP:
	case NL80211_IFTYPE_AP_VLAN:
	case NL80211_IFTYPE_P2P_GO:
		params->sta_flags_mask = BIT(NL80211_STA_FLAG_AUTHORIZED) |
					 BIT(NL80211_STA_FLAG_SHORT_PREAMBLE) |
					 BIT(NL80211_STA_FLAG_WME) |
					 BIT(NL80211_STA_FLAG_MFP);
		break;
	case NL80211_IFTYPE_P2P_CLIENT:
	case NL80211_IFTYPE_STATION:
		params->sta_flags_mask = BIT(NL80211_STA_FLAG_AUTHORIZED) |
					 BIT(NL80211_STA_FLAG_TDLS_PEER);
		break;
	case NL80211_IFTYPE_MESH_POINT:
		params->sta_flags_mask = BIT(NL80211_STA_FLAG_AUTHENTICATED) |
					 BIT(NL80211_STA_FLAG_MFP) |
					 BIT(NL80211_STA_FLAG_AUTHORIZED);
	default:
		return -EINVAL;
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

	for (flag = 1; flag <= NL80211_STA_FLAG_MAX; flag++)
		if (flags[flag])
			params->sta_flags_set |= (1<<flag);

=======

	for (flag = 1; flag <= NL80211_STA_FLAG_MAX; flag++) {
		if (flags[flag]) {
			params->sta_flags_set |= (1<<flag);

			/* no longer support new API additions in old API */
			if (flag > NL80211_STA_FLAG_MAX_OLD_API)
				return -EINVAL;
		}
	}

>>>>>>> refs/remotes/origin/master
	return 0;
}

static bool nl80211_put_sta_rate(struct sk_buff *msg, struct rate_info *info,
				 int attr)
{
	struct nlattr *rate;
<<<<<<< HEAD
	u16 bitrate;

	rate = nla_nest_start(msg, attr);
	if (!rate)
		goto nla_put_failure;

	/* cfg80211_calculate_bitrate will return 0 for mcs >= 32 */
	bitrate = cfg80211_calculate_bitrate(info);
	if (bitrate > 0)
		NLA_PUT_U16(msg, NL80211_RATE_INFO_BITRATE, bitrate);

	if (info->flags & RATE_INFO_FLAGS_MCS)
		NLA_PUT_U8(msg, NL80211_RATE_INFO_MCS, info->mcs);
	if (info->flags & RATE_INFO_FLAGS_40_MHZ_WIDTH)
		NLA_PUT_FLAG(msg, NL80211_RATE_INFO_40_MHZ_WIDTH);
	if (info->flags & RATE_INFO_FLAGS_SHORT_GI)
		NLA_PUT_FLAG(msg, NL80211_RATE_INFO_SHORT_GI);

	nla_nest_end(msg, rate);
	return true;

nla_put_failure:
	return false;
}

static int nl80211_send_station(struct sk_buff *msg, u32 pid, u32 seq,
<<<<<<< HEAD
				int flags, struct net_device *dev,
=======
				int flags,
				struct cfg80211_registered_device *rdev,
				struct net_device *dev,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	u32 bitrate;
	u16 bitrate_compat;

	rate = nla_nest_start(msg, attr);
	if (!rate)
		return false;

	/* cfg80211_calculate_bitrate will return 0 for mcs >= 32 */
	bitrate = cfg80211_calculate_bitrate(info);
	/* report 16-bit bitrate only if we can */
	bitrate_compat = bitrate < (1UL << 16) ? bitrate : 0;
	if (bitrate > 0 &&
	    nla_put_u32(msg, NL80211_RATE_INFO_BITRATE32, bitrate))
		return false;
	if (bitrate_compat > 0 &&
	    nla_put_u16(msg, NL80211_RATE_INFO_BITRATE, bitrate_compat))
		return false;

	if (info->flags & RATE_INFO_FLAGS_MCS) {
		if (nla_put_u8(msg, NL80211_RATE_INFO_MCS, info->mcs))
			return false;
		if (info->flags & RATE_INFO_FLAGS_40_MHZ_WIDTH &&
		    nla_put_flag(msg, NL80211_RATE_INFO_40_MHZ_WIDTH))
			return false;
		if (info->flags & RATE_INFO_FLAGS_SHORT_GI &&
		    nla_put_flag(msg, NL80211_RATE_INFO_SHORT_GI))
			return false;
	} else if (info->flags & RATE_INFO_FLAGS_VHT_MCS) {
		if (nla_put_u8(msg, NL80211_RATE_INFO_VHT_MCS, info->mcs))
			return false;
		if (nla_put_u8(msg, NL80211_RATE_INFO_VHT_NSS, info->nss))
			return false;
		if (info->flags & RATE_INFO_FLAGS_40_MHZ_WIDTH &&
		    nla_put_flag(msg, NL80211_RATE_INFO_40_MHZ_WIDTH))
			return false;
		if (info->flags & RATE_INFO_FLAGS_80_MHZ_WIDTH &&
		    nla_put_flag(msg, NL80211_RATE_INFO_80_MHZ_WIDTH))
			return false;
		if (info->flags & RATE_INFO_FLAGS_80P80_MHZ_WIDTH &&
		    nla_put_flag(msg, NL80211_RATE_INFO_80P80_MHZ_WIDTH))
			return false;
		if (info->flags & RATE_INFO_FLAGS_160_MHZ_WIDTH &&
		    nla_put_flag(msg, NL80211_RATE_INFO_160_MHZ_WIDTH))
			return false;
		if (info->flags & RATE_INFO_FLAGS_SHORT_GI &&
		    nla_put_flag(msg, NL80211_RATE_INFO_SHORT_GI))
			return false;
	}

	nla_nest_end(msg, rate);
	return true;
}

static bool nl80211_put_signal(struct sk_buff *msg, u8 mask, s8 *signal,
			       int id)
{
	void *attr;
	int i = 0;

	if (!mask)
		return true;

	attr = nla_nest_start(msg, id);
	if (!attr)
		return false;

	for (i = 0; i < IEEE80211_MAX_CHAINS; i++) {
		if (!(mask & BIT(i)))
			continue;

		if (nla_put_u8(msg, i, signal[i]))
			return false;
	}

	nla_nest_end(msg, attr);

	return true;
}

static int nl80211_send_station(struct sk_buff *msg, u32 portid, u32 seq,
				int flags,
				struct cfg80211_registered_device *rdev,
				struct net_device *dev,
>>>>>>> refs/remotes/origin/master
				const u8 *mac_addr, struct station_info *sinfo)
{
	void *hdr;
	struct nlattr *sinfoattr, *bss_param;

<<<<<<< HEAD
	hdr = nl80211hdr_put(msg, pid, seq, flags, NL80211_CMD_NEW_STATION);
	if (!hdr)
		return -1;

	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, dev->ifindex);
	NLA_PUT(msg, NL80211_ATTR_MAC, ETH_ALEN, mac_addr);

	NLA_PUT_U32(msg, NL80211_ATTR_GENERATION, sinfo->generation);
=======
	hdr = nl80211hdr_put(msg, portid, seq, flags, NL80211_CMD_NEW_STATION);
	if (!hdr)
		return -1;

	if (nla_put_u32(msg, NL80211_ATTR_IFINDEX, dev->ifindex) ||
	    nla_put(msg, NL80211_ATTR_MAC, ETH_ALEN, mac_addr) ||
	    nla_put_u32(msg, NL80211_ATTR_GENERATION, sinfo->generation))
		goto nla_put_failure;
>>>>>>> refs/remotes/origin/master

	sinfoattr = nla_nest_start(msg, NL80211_ATTR_STA_INFO);
	if (!sinfoattr)
		goto nla_put_failure;
<<<<<<< HEAD
	if (sinfo->filled & STATION_INFO_CONNECTED_TIME)
		NLA_PUT_U32(msg, NL80211_STA_INFO_CONNECTED_TIME,
			    sinfo->connected_time);
	if (sinfo->filled & STATION_INFO_INACTIVE_TIME)
		NLA_PUT_U32(msg, NL80211_STA_INFO_INACTIVE_TIME,
			    sinfo->inactive_time);
	if (sinfo->filled & STATION_INFO_RX_BYTES)
		NLA_PUT_U32(msg, NL80211_STA_INFO_RX_BYTES,
			    sinfo->rx_bytes);
	if (sinfo->filled & STATION_INFO_TX_BYTES)
		NLA_PUT_U32(msg, NL80211_STA_INFO_TX_BYTES,
			    sinfo->tx_bytes);
	if (sinfo->filled & STATION_INFO_LLID)
		NLA_PUT_U16(msg, NL80211_STA_INFO_LLID,
			    sinfo->llid);
	if (sinfo->filled & STATION_INFO_PLID)
		NLA_PUT_U16(msg, NL80211_STA_INFO_PLID,
			    sinfo->plid);
	if (sinfo->filled & STATION_INFO_PLINK_STATE)
		NLA_PUT_U8(msg, NL80211_STA_INFO_PLINK_STATE,
			    sinfo->plink_state);
<<<<<<< HEAD
	if (sinfo->filled & STATION_INFO_SIGNAL)
		NLA_PUT_U8(msg, NL80211_STA_INFO_SIGNAL,
			   sinfo->signal);
	if (sinfo->filled & STATION_INFO_SIGNAL_AVG)
		NLA_PUT_U8(msg, NL80211_STA_INFO_SIGNAL_AVG,
			   sinfo->signal_avg);
=======
	switch (rdev->wiphy.signal_type) {
	case CFG80211_SIGNAL_TYPE_MBM:
		if (sinfo->filled & STATION_INFO_SIGNAL)
			NLA_PUT_U8(msg, NL80211_STA_INFO_SIGNAL,
				   sinfo->signal);
		if (sinfo->filled & STATION_INFO_SIGNAL_AVG)
			NLA_PUT_U8(msg, NL80211_STA_INFO_SIGNAL_AVG,
				   sinfo->signal_avg);
=======
	if ((sinfo->filled & STATION_INFO_CONNECTED_TIME) &&
	    nla_put_u32(msg, NL80211_STA_INFO_CONNECTED_TIME,
			sinfo->connected_time))
		goto nla_put_failure;
	if ((sinfo->filled & STATION_INFO_INACTIVE_TIME) &&
	    nla_put_u32(msg, NL80211_STA_INFO_INACTIVE_TIME,
			sinfo->inactive_time))
		goto nla_put_failure;
	if ((sinfo->filled & (STATION_INFO_RX_BYTES |
			      STATION_INFO_RX_BYTES64)) &&
	    nla_put_u32(msg, NL80211_STA_INFO_RX_BYTES,
			(u32)sinfo->rx_bytes))
		goto nla_put_failure;
	if ((sinfo->filled & (STATION_INFO_TX_BYTES |
			      STATION_INFO_TX_BYTES64)) &&
	    nla_put_u32(msg, NL80211_STA_INFO_TX_BYTES,
			(u32)sinfo->tx_bytes))
		goto nla_put_failure;
	if ((sinfo->filled & STATION_INFO_RX_BYTES64) &&
	    nla_put_u64(msg, NL80211_STA_INFO_RX_BYTES64,
			sinfo->rx_bytes))
		goto nla_put_failure;
	if ((sinfo->filled & STATION_INFO_TX_BYTES64) &&
	    nla_put_u64(msg, NL80211_STA_INFO_TX_BYTES64,
			sinfo->tx_bytes))
		goto nla_put_failure;
	if ((sinfo->filled & STATION_INFO_LLID) &&
	    nla_put_u16(msg, NL80211_STA_INFO_LLID, sinfo->llid))
		goto nla_put_failure;
	if ((sinfo->filled & STATION_INFO_PLID) &&
	    nla_put_u16(msg, NL80211_STA_INFO_PLID, sinfo->plid))
		goto nla_put_failure;
	if ((sinfo->filled & STATION_INFO_PLINK_STATE) &&
	    nla_put_u8(msg, NL80211_STA_INFO_PLINK_STATE,
		       sinfo->plink_state))
		goto nla_put_failure;
	switch (rdev->wiphy.signal_type) {
	case CFG80211_SIGNAL_TYPE_MBM:
		if ((sinfo->filled & STATION_INFO_SIGNAL) &&
		    nla_put_u8(msg, NL80211_STA_INFO_SIGNAL,
			       sinfo->signal))
			goto nla_put_failure;
		if ((sinfo->filled & STATION_INFO_SIGNAL_AVG) &&
		    nla_put_u8(msg, NL80211_STA_INFO_SIGNAL_AVG,
			       sinfo->signal_avg))
			goto nla_put_failure;
>>>>>>> refs/remotes/origin/master
		break;
	default:
		break;
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (sinfo->filled & STATION_INFO_CHAIN_SIGNAL) {
		if (!nl80211_put_signal(msg, sinfo->chains,
					sinfo->chain_signal,
					NL80211_STA_INFO_CHAIN_SIGNAL))
			goto nla_put_failure;
	}
	if (sinfo->filled & STATION_INFO_CHAIN_SIGNAL_AVG) {
		if (!nl80211_put_signal(msg, sinfo->chains,
					sinfo->chain_signal_avg,
					NL80211_STA_INFO_CHAIN_SIGNAL_AVG))
			goto nla_put_failure;
	}
>>>>>>> refs/remotes/origin/master
	if (sinfo->filled & STATION_INFO_TX_BITRATE) {
		if (!nl80211_put_sta_rate(msg, &sinfo->txrate,
					  NL80211_STA_INFO_TX_BITRATE))
			goto nla_put_failure;
	}
	if (sinfo->filled & STATION_INFO_RX_BITRATE) {
		if (!nl80211_put_sta_rate(msg, &sinfo->rxrate,
					  NL80211_STA_INFO_RX_BITRATE))
			goto nla_put_failure;
	}
<<<<<<< HEAD
	if (sinfo->filled & STATION_INFO_RX_PACKETS)
		NLA_PUT_U32(msg, NL80211_STA_INFO_RX_PACKETS,
			    sinfo->rx_packets);
	if (sinfo->filled & STATION_INFO_TX_PACKETS)
		NLA_PUT_U32(msg, NL80211_STA_INFO_TX_PACKETS,
			    sinfo->tx_packets);
	if (sinfo->filled & STATION_INFO_TX_RETRIES)
		NLA_PUT_U32(msg, NL80211_STA_INFO_TX_RETRIES,
			    sinfo->tx_retries);
	if (sinfo->filled & STATION_INFO_TX_FAILED)
		NLA_PUT_U32(msg, NL80211_STA_INFO_TX_FAILED,
			    sinfo->tx_failed);
<<<<<<< HEAD
=======
	if (sinfo->filled & STATION_INFO_BEACON_LOSS_COUNT)
		NLA_PUT_U32(msg, NL80211_STA_INFO_BEACON_LOSS,
			    sinfo->beacon_loss_count);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if ((sinfo->filled & STATION_INFO_RX_PACKETS) &&
	    nla_put_u32(msg, NL80211_STA_INFO_RX_PACKETS,
			sinfo->rx_packets))
		goto nla_put_failure;
	if ((sinfo->filled & STATION_INFO_TX_PACKETS) &&
	    nla_put_u32(msg, NL80211_STA_INFO_TX_PACKETS,
			sinfo->tx_packets))
		goto nla_put_failure;
	if ((sinfo->filled & STATION_INFO_TX_RETRIES) &&
	    nla_put_u32(msg, NL80211_STA_INFO_TX_RETRIES,
			sinfo->tx_retries))
		goto nla_put_failure;
	if ((sinfo->filled & STATION_INFO_TX_FAILED) &&
	    nla_put_u32(msg, NL80211_STA_INFO_TX_FAILED,
			sinfo->tx_failed))
		goto nla_put_failure;
	if ((sinfo->filled & STATION_INFO_BEACON_LOSS_COUNT) &&
	    nla_put_u32(msg, NL80211_STA_INFO_BEACON_LOSS,
			sinfo->beacon_loss_count))
		goto nla_put_failure;
	if ((sinfo->filled & STATION_INFO_LOCAL_PM) &&
	    nla_put_u32(msg, NL80211_STA_INFO_LOCAL_PM,
			sinfo->local_pm))
		goto nla_put_failure;
	if ((sinfo->filled & STATION_INFO_PEER_PM) &&
	    nla_put_u32(msg, NL80211_STA_INFO_PEER_PM,
			sinfo->peer_pm))
		goto nla_put_failure;
	if ((sinfo->filled & STATION_INFO_NONPEER_PM) &&
	    nla_put_u32(msg, NL80211_STA_INFO_NONPEER_PM,
			sinfo->nonpeer_pm))
		goto nla_put_failure;
>>>>>>> refs/remotes/origin/master
	if (sinfo->filled & STATION_INFO_BSS_PARAM) {
		bss_param = nla_nest_start(msg, NL80211_STA_INFO_BSS_PARAM);
		if (!bss_param)
			goto nla_put_failure;

<<<<<<< HEAD
		if (sinfo->bss_param.flags & BSS_PARAM_FLAGS_CTS_PROT)
			NLA_PUT_FLAG(msg, NL80211_STA_BSS_PARAM_CTS_PROT);
		if (sinfo->bss_param.flags & BSS_PARAM_FLAGS_SHORT_PREAMBLE)
			NLA_PUT_FLAG(msg, NL80211_STA_BSS_PARAM_SHORT_PREAMBLE);
		if (sinfo->bss_param.flags & BSS_PARAM_FLAGS_SHORT_SLOT_TIME)
			NLA_PUT_FLAG(msg,
				     NL80211_STA_BSS_PARAM_SHORT_SLOT_TIME);
		NLA_PUT_U8(msg, NL80211_STA_BSS_PARAM_DTIM_PERIOD,
			   sinfo->bss_param.dtim_period);
		NLA_PUT_U16(msg, NL80211_STA_BSS_PARAM_BEACON_INTERVAL,
			    sinfo->bss_param.beacon_interval);

		nla_nest_end(msg, bss_param);
	}
<<<<<<< HEAD
=======
	if (sinfo->filled & STATION_INFO_STA_FLAGS)
		NLA_PUT(msg, NL80211_STA_INFO_STA_FLAGS,
			sizeof(struct nl80211_sta_flag_update),
			&sinfo->sta_flags);
>>>>>>> refs/remotes/origin/cm-10.0
	nla_nest_end(msg, sinfoattr);

	if (sinfo->filled & STATION_INFO_ASSOC_REQ_IES)
		NLA_PUT(msg, NL80211_ATTR_IE, sinfo->assoc_req_ies_len,
			sinfo->assoc_req_ies);
=======
		if (((sinfo->bss_param.flags & BSS_PARAM_FLAGS_CTS_PROT) &&
		     nla_put_flag(msg, NL80211_STA_BSS_PARAM_CTS_PROT)) ||
		    ((sinfo->bss_param.flags & BSS_PARAM_FLAGS_SHORT_PREAMBLE) &&
		     nla_put_flag(msg, NL80211_STA_BSS_PARAM_SHORT_PREAMBLE)) ||
		    ((sinfo->bss_param.flags & BSS_PARAM_FLAGS_SHORT_SLOT_TIME) &&
		     nla_put_flag(msg, NL80211_STA_BSS_PARAM_SHORT_SLOT_TIME)) ||
		    nla_put_u8(msg, NL80211_STA_BSS_PARAM_DTIM_PERIOD,
			       sinfo->bss_param.dtim_period) ||
		    nla_put_u16(msg, NL80211_STA_BSS_PARAM_BEACON_INTERVAL,
				sinfo->bss_param.beacon_interval))
			goto nla_put_failure;

		nla_nest_end(msg, bss_param);
	}
	if ((sinfo->filled & STATION_INFO_STA_FLAGS) &&
	    nla_put(msg, NL80211_STA_INFO_STA_FLAGS,
		    sizeof(struct nl80211_sta_flag_update),
		    &sinfo->sta_flags))
		goto nla_put_failure;
	if ((sinfo->filled & STATION_INFO_T_OFFSET) &&
		nla_put_u64(msg, NL80211_STA_INFO_T_OFFSET,
			    sinfo->t_offset))
		goto nla_put_failure;
	nla_nest_end(msg, sinfoattr);

	if ((sinfo->filled & STATION_INFO_ASSOC_REQ_IES) &&
	    nla_put(msg, NL80211_ATTR_IE, sinfo->assoc_req_ies_len,
		    sinfo->assoc_req_ies))
		goto nla_put_failure;
>>>>>>> refs/remotes/origin/master

	return genlmsg_end(msg, hdr);

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	return -EMSGSIZE;
}

static int nl80211_dump_station(struct sk_buff *skb,
				struct netlink_callback *cb)
{
	struct station_info sinfo;
	struct cfg80211_registered_device *dev;
<<<<<<< HEAD
	struct net_device *netdev;
	u8 mac_addr[ETH_ALEN];
	int sta_idx = cb->args[1];
	int err;

	err = nl80211_prepare_netdev_dump(skb, cb, &dev, &netdev);
	if (err)
		return err;

	if (!dev->ops->dump_station) {
=======
	struct wireless_dev *wdev;
	u8 mac_addr[ETH_ALEN];
	int sta_idx = cb->args[2];
	int err;

	err = nl80211_prepare_wdev_dump(skb, cb, &dev, &wdev);
	if (err)
		return err;

	if (!wdev->netdev) {
		err = -EINVAL;
		goto out_err;
	}

	if (!dev->ops->dump_station) {
>>>>>>> refs/remotes/origin/master
		err = -EOPNOTSUPP;
		goto out_err;
	}

	while (1) {
		memset(&sinfo, 0, sizeof(sinfo));
<<<<<<< HEAD
		err = dev->ops->dump_station(&dev->wiphy, netdev, sta_idx,
					     mac_addr, &sinfo);
=======
		err = rdev_dump_station(dev, wdev->netdev, sta_idx,
					mac_addr, &sinfo);
>>>>>>> refs/remotes/origin/master
		if (err == -ENOENT)
			break;
		if (err)
			goto out_err;

		if (nl80211_send_station(skb,
<<<<<<< HEAD
				NETLINK_CB(cb->skb).pid,
				cb->nlh->nlmsg_seq, NLM_F_MULTI,
<<<<<<< HEAD
				netdev, mac_addr,
=======
				dev, netdev, mac_addr,
>>>>>>> refs/remotes/origin/cm-10.0
=======
				NETLINK_CB(cb->skb).portid,
				cb->nlh->nlmsg_seq, NLM_F_MULTI,
				dev, wdev->netdev, mac_addr,
>>>>>>> refs/remotes/origin/master
				&sinfo) < 0)
			goto out;

		sta_idx++;
	}


 out:
<<<<<<< HEAD
	cb->args[1] = sta_idx;
	err = skb->len;
 out_err:
	nl80211_finish_netdev_dump(dev);
=======
	cb->args[2] = sta_idx;
	err = skb->len;
 out_err:
	nl80211_finish_wdev_dump(dev);
>>>>>>> refs/remotes/origin/master

	return err;
}

static int nl80211_get_station(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];
	struct station_info sinfo;
	struct sk_buff *msg;
	u8 *mac_addr = NULL;
	int err;

	memset(&sinfo, 0, sizeof(sinfo));

	if (!info->attrs[NL80211_ATTR_MAC])
		return -EINVAL;

	mac_addr = nla_data(info->attrs[NL80211_ATTR_MAC]);

	if (!rdev->ops->get_station)
		return -EOPNOTSUPP;

<<<<<<< HEAD
	err = rdev->ops->get_station(&rdev->wiphy, dev, mac_addr, &sinfo);
=======
	err = rdev_get_station(rdev, dev, mac_addr, &sinfo);
>>>>>>> refs/remotes/origin/master
	if (err)
		return err;

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (!msg)
		return -ENOMEM;

<<<<<<< HEAD
	if (nl80211_send_station(msg, info->snd_pid, info->snd_seq, 0,
<<<<<<< HEAD
				 dev, mac_addr, &sinfo) < 0) {
=======
				 rdev, dev, mac_addr, &sinfo) < 0) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (nl80211_send_station(msg, info->snd_portid, info->snd_seq, 0,
				 rdev, dev, mac_addr, &sinfo) < 0) {
>>>>>>> refs/remotes/origin/master
		nlmsg_free(msg);
		return -ENOBUFS;
	}

	return genlmsg_reply(msg, info);
}

<<<<<<< HEAD
/*
 * Get vlan interface making sure it is running and on the right wiphy.
 */
<<<<<<< HEAD
static int get_vlan(struct genl_info *info,
		    struct cfg80211_registered_device *rdev,
		    struct net_device **vlan)
{
	struct nlattr *vlanattr = info->attrs[NL80211_ATTR_STA_VLAN];
	*vlan = NULL;

	if (vlanattr) {
		*vlan = dev_get_by_index(genl_info_net(info),
					 nla_get_u32(vlanattr));
		if (!*vlan)
			return -ENODEV;
		if (!(*vlan)->ieee80211_ptr)
			return -EINVAL;
		if ((*vlan)->ieee80211_ptr->wiphy != &rdev->wiphy)
			return -EINVAL;
		if (!netif_running(*vlan))
			return -ENETDOWN;
	}
	return 0;
=======
=======
int cfg80211_check_station_change(struct wiphy *wiphy,
				  struct station_parameters *params,
				  enum cfg80211_station_type statype)
{
	if (params->listen_interval != -1)
		return -EINVAL;
	if (params->aid)
		return -EINVAL;

	/* When you run into this, adjust the code below for the new flag */
	BUILD_BUG_ON(NL80211_STA_FLAG_MAX != 7);

	switch (statype) {
	case CFG80211_STA_MESH_PEER_KERNEL:
	case CFG80211_STA_MESH_PEER_USER:
		/*
		 * No ignoring the TDLS flag here -- the userspace mesh
		 * code doesn't have the bug of including TDLS in the
		 * mask everywhere.
		 */
		if (params->sta_flags_mask &
				~(BIT(NL80211_STA_FLAG_AUTHENTICATED) |
				  BIT(NL80211_STA_FLAG_MFP) |
				  BIT(NL80211_STA_FLAG_AUTHORIZED)))
			return -EINVAL;
		break;
	case CFG80211_STA_TDLS_PEER_SETUP:
	case CFG80211_STA_TDLS_PEER_ACTIVE:
		if (!(params->sta_flags_set & BIT(NL80211_STA_FLAG_TDLS_PEER)))
			return -EINVAL;
		/* ignore since it can't change */
		params->sta_flags_mask &= ~BIT(NL80211_STA_FLAG_TDLS_PEER);
		break;
	default:
		/* disallow mesh-specific things */
		if (params->plink_action != NL80211_PLINK_ACTION_NO_ACTION)
			return -EINVAL;
		if (params->local_pm)
			return -EINVAL;
		if (params->sta_modify_mask & STATION_PARAM_APPLY_PLINK_STATE)
			return -EINVAL;
	}

	if (statype != CFG80211_STA_TDLS_PEER_SETUP &&
	    statype != CFG80211_STA_TDLS_PEER_ACTIVE) {
		/* TDLS can't be set, ... */
		if (params->sta_flags_set & BIT(NL80211_STA_FLAG_TDLS_PEER))
			return -EINVAL;
		/*
		 * ... but don't bother the driver with it. This works around
		 * a hostapd/wpa_supplicant issue -- it always includes the
		 * TLDS_PEER flag in the mask even for AP mode.
		 */
		params->sta_flags_mask &= ~BIT(NL80211_STA_FLAG_TDLS_PEER);
	}

	if (statype != CFG80211_STA_TDLS_PEER_SETUP) {
		/* reject other things that can't change */
		if (params->sta_modify_mask & STATION_PARAM_APPLY_UAPSD)
			return -EINVAL;
		if (params->sta_modify_mask & STATION_PARAM_APPLY_CAPABILITY)
			return -EINVAL;
		if (params->supported_rates)
			return -EINVAL;
		if (params->ext_capab || params->ht_capa || params->vht_capa)
			return -EINVAL;
	}

	if (statype != CFG80211_STA_AP_CLIENT) {
		if (params->vlan)
			return -EINVAL;
	}

	switch (statype) {
	case CFG80211_STA_AP_MLME_CLIENT:
		/* Use this only for authorizing/unauthorizing a station */
		if (!(params->sta_flags_mask & BIT(NL80211_STA_FLAG_AUTHORIZED)))
			return -EOPNOTSUPP;
		break;
	case CFG80211_STA_AP_CLIENT:
		/* accept only the listed bits */
		if (params->sta_flags_mask &
				~(BIT(NL80211_STA_FLAG_AUTHORIZED) |
				  BIT(NL80211_STA_FLAG_AUTHENTICATED) |
				  BIT(NL80211_STA_FLAG_ASSOCIATED) |
				  BIT(NL80211_STA_FLAG_SHORT_PREAMBLE) |
				  BIT(NL80211_STA_FLAG_WME) |
				  BIT(NL80211_STA_FLAG_MFP)))
			return -EINVAL;

		/* but authenticated/associated only if driver handles it */
		if (!(wiphy->features & NL80211_FEATURE_FULL_AP_CLIENT_STATE) &&
		    params->sta_flags_mask &
				(BIT(NL80211_STA_FLAG_AUTHENTICATED) |
				 BIT(NL80211_STA_FLAG_ASSOCIATED)))
			return -EINVAL;
		break;
	case CFG80211_STA_IBSS:
	case CFG80211_STA_AP_STA:
		/* reject any changes other than AUTHORIZED */
		if (params->sta_flags_mask & ~BIT(NL80211_STA_FLAG_AUTHORIZED))
			return -EINVAL;
		break;
	case CFG80211_STA_TDLS_PEER_SETUP:
		/* reject any changes other than AUTHORIZED or WME */
		if (params->sta_flags_mask & ~(BIT(NL80211_STA_FLAG_AUTHORIZED) |
					       BIT(NL80211_STA_FLAG_WME)))
			return -EINVAL;
		/* force (at least) rates when authorizing */
		if (params->sta_flags_set & BIT(NL80211_STA_FLAG_AUTHORIZED) &&
		    !params->supported_rates)
			return -EINVAL;
		break;
	case CFG80211_STA_TDLS_PEER_ACTIVE:
		/* reject any changes */
		return -EINVAL;
	case CFG80211_STA_MESH_PEER_KERNEL:
		if (params->sta_modify_mask & STATION_PARAM_APPLY_PLINK_STATE)
			return -EINVAL;
		break;
	case CFG80211_STA_MESH_PEER_USER:
		if (params->plink_action != NL80211_PLINK_ACTION_NO_ACTION)
			return -EINVAL;
		break;
	}

	return 0;
}
EXPORT_SYMBOL(cfg80211_check_station_change);

/*
 * Get vlan interface making sure it is running and on the right wiphy.
 */
>>>>>>> refs/remotes/origin/master
static struct net_device *get_vlan(struct genl_info *info,
				   struct cfg80211_registered_device *rdev)
{
	struct nlattr *vlanattr = info->attrs[NL80211_ATTR_STA_VLAN];
	struct net_device *v;
	int ret;

	if (!vlanattr)
		return NULL;

	v = dev_get_by_index(genl_info_net(info), nla_get_u32(vlanattr));
	if (!v)
		return ERR_PTR(-ENODEV);

	if (!v->ieee80211_ptr || v->ieee80211_ptr->wiphy != &rdev->wiphy) {
		ret = -EINVAL;
		goto error;
	}

<<<<<<< HEAD
=======
	if (v->ieee80211_ptr->iftype != NL80211_IFTYPE_AP_VLAN &&
	    v->ieee80211_ptr->iftype != NL80211_IFTYPE_AP &&
	    v->ieee80211_ptr->iftype != NL80211_IFTYPE_P2P_GO) {
		ret = -EINVAL;
		goto error;
	}

>>>>>>> refs/remotes/origin/master
	if (!netif_running(v)) {
		ret = -ENETDOWN;
		goto error;
	}

	return v;
 error:
	dev_put(v);
	return ERR_PTR(ret);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
}

static struct nla_policy
nl80211_sta_wme_policy[NL80211_STA_WME_MAX + 1] __read_mostly = {
	[NL80211_STA_WME_UAPSD_QUEUES] = { .type = NLA_U8 },
	[NL80211_STA_WME_MAX_SP] = { .type = NLA_U8 },
};

static int nl80211_parse_sta_wme(struct genl_info *info,
				 struct station_parameters *params)
{
	struct nlattr *tb[NL80211_STA_WME_MAX + 1];
	struct nlattr *nla;
	int err;

	/* parse WME attributes if present */
	if (!info->attrs[NL80211_ATTR_STA_WME])
		return 0;

	nla = info->attrs[NL80211_ATTR_STA_WME];
	err = nla_parse_nested(tb, NL80211_STA_WME_MAX, nla,
			       nl80211_sta_wme_policy);
	if (err)
		return err;

	if (tb[NL80211_STA_WME_UAPSD_QUEUES])
		params->uapsd_queues = nla_get_u8(
			tb[NL80211_STA_WME_UAPSD_QUEUES]);
	if (params->uapsd_queues & ~IEEE80211_WMM_IE_STA_QOSINFO_AC_MASK)
		return -EINVAL;

	if (tb[NL80211_STA_WME_MAX_SP])
		params->max_sp = nla_get_u8(tb[NL80211_STA_WME_MAX_SP]);

	if (params->max_sp & ~IEEE80211_WMM_IE_STA_QOSINFO_SP_MASK)
		return -EINVAL;

	params->sta_modify_mask |= STATION_PARAM_APPLY_UAPSD;

	return 0;
}

static int nl80211_parse_sta_channel_info(struct genl_info *info,
				      struct station_parameters *params)
{
	if (info->attrs[NL80211_ATTR_STA_SUPPORTED_CHANNELS]) {
		params->supported_channels =
		     nla_data(info->attrs[NL80211_ATTR_STA_SUPPORTED_CHANNELS]);
		params->supported_channels_len =
		     nla_len(info->attrs[NL80211_ATTR_STA_SUPPORTED_CHANNELS]);
		/*
		 * Need to include at least one (first channel, number of
		 * channels) tuple for each subband, and must have proper
		 * tuples for the rest of the data as well.
		 */
		if (params->supported_channels_len < 2)
			return -EINVAL;
		if (params->supported_channels_len % 2)
			return -EINVAL;
	}

	if (info->attrs[NL80211_ATTR_STA_SUPPORTED_OPER_CLASSES]) {
		params->supported_oper_classes =
		 nla_data(info->attrs[NL80211_ATTR_STA_SUPPORTED_OPER_CLASSES]);
		params->supported_oper_classes_len =
		  nla_len(info->attrs[NL80211_ATTR_STA_SUPPORTED_OPER_CLASSES]);
		/*
		 * The value of the Length field of the Supported Operating
		 * Classes element is between 2 and 253.
		 */
		if (params->supported_oper_classes_len < 2 ||
		    params->supported_oper_classes_len > 253)
			return -EINVAL;
	}
	return 0;
}

static int nl80211_set_station_tdls(struct genl_info *info,
				    struct station_parameters *params)
{
	int err;
	/* Dummy STA entry gets updated once the peer capabilities are known */
	if (info->attrs[NL80211_ATTR_PEER_AID])
		params->aid = nla_get_u16(info->attrs[NL80211_ATTR_PEER_AID]);
	if (info->attrs[NL80211_ATTR_HT_CAPABILITY])
		params->ht_capa =
			nla_data(info->attrs[NL80211_ATTR_HT_CAPABILITY]);
	if (info->attrs[NL80211_ATTR_VHT_CAPABILITY])
		params->vht_capa =
			nla_data(info->attrs[NL80211_ATTR_VHT_CAPABILITY]);

	err = nl80211_parse_sta_channel_info(info, params);
	if (err)
		return err;

	return nl80211_parse_sta_wme(info, params);
>>>>>>> refs/remotes/origin/master
}

static int nl80211_set_station(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
<<<<<<< HEAD
	int err;
	struct net_device *dev = info->user_ptr[1];
	struct station_parameters params;
	u8 *mac_addr = NULL;
=======
	struct net_device *dev = info->user_ptr[1];
	struct station_parameters params;
	u8 *mac_addr;
	int err;
>>>>>>> refs/remotes/origin/master

	memset(&params, 0, sizeof(params));

	params.listen_interval = -1;
<<<<<<< HEAD
	params.plink_state = -1;
=======

	if (!rdev->ops->change_station)
		return -EOPNOTSUPP;
>>>>>>> refs/remotes/origin/master

	if (info->attrs[NL80211_ATTR_STA_AID])
		return -EINVAL;

	if (!info->attrs[NL80211_ATTR_MAC])
		return -EINVAL;

	mac_addr = nla_data(info->attrs[NL80211_ATTR_MAC]);

	if (info->attrs[NL80211_ATTR_STA_SUPPORTED_RATES]) {
		params.supported_rates =
			nla_data(info->attrs[NL80211_ATTR_STA_SUPPORTED_RATES]);
		params.supported_rates_len =
			nla_len(info->attrs[NL80211_ATTR_STA_SUPPORTED_RATES]);
	}

<<<<<<< HEAD
	if (info->attrs[NL80211_ATTR_STA_LISTEN_INTERVAL])
		params.listen_interval =
		    nla_get_u16(info->attrs[NL80211_ATTR_STA_LISTEN_INTERVAL]);

	if (info->attrs[NL80211_ATTR_HT_CAPABILITY])
		params.ht_capa =
			nla_data(info->attrs[NL80211_ATTR_HT_CAPABILITY]);

<<<<<<< HEAD
	if (parse_station_flags(info, &params))
=======
	if (!rdev->ops->change_station)
		return -EOPNOTSUPP;

	if (parse_station_flags(info, dev->ieee80211_ptr->iftype, &params))
>>>>>>> refs/remotes/origin/cm-10.0
		return -EINVAL;

	if (info->attrs[NL80211_ATTR_STA_PLINK_ACTION])
		params.plink_action =
		    nla_get_u8(info->attrs[NL80211_ATTR_STA_PLINK_ACTION]);

	if (info->attrs[NL80211_ATTR_STA_PLINK_STATE])
		params.plink_state =
		    nla_get_u8(info->attrs[NL80211_ATTR_STA_PLINK_STATE]);

<<<<<<< HEAD
	err = get_vlan(info, rdev, &params.vlan);
	if (err)
		goto out;

	/* validate settings */
	err = 0;

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (info->attrs[NL80211_ATTR_STA_CAPABILITY]) {
		params.capability =
			nla_get_u16(info->attrs[NL80211_ATTR_STA_CAPABILITY]);
		params.sta_modify_mask |= STATION_PARAM_APPLY_CAPABILITY;
	}

	if (info->attrs[NL80211_ATTR_STA_EXT_CAPABILITY]) {
		params.ext_capab =
			nla_data(info->attrs[NL80211_ATTR_STA_EXT_CAPABILITY]);
		params.ext_capab_len =
			nla_len(info->attrs[NL80211_ATTR_STA_EXT_CAPABILITY]);
	}

	if (info->attrs[NL80211_ATTR_STA_LISTEN_INTERVAL])
		return -EINVAL;

	if (parse_station_flags(info, dev->ieee80211_ptr->iftype, &params))
		return -EINVAL;

	if (info->attrs[NL80211_ATTR_STA_PLINK_ACTION]) {
		params.plink_action =
			nla_get_u8(info->attrs[NL80211_ATTR_STA_PLINK_ACTION]);
		if (params.plink_action >= NUM_NL80211_PLINK_ACTIONS)
			return -EINVAL;
	}

	if (info->attrs[NL80211_ATTR_STA_PLINK_STATE]) {
		params.plink_state =
			nla_get_u8(info->attrs[NL80211_ATTR_STA_PLINK_STATE]);
		if (params.plink_state >= NUM_NL80211_PLINK_STATES)
			return -EINVAL;
		params.sta_modify_mask |= STATION_PARAM_APPLY_PLINK_STATE;
	}

	if (info->attrs[NL80211_ATTR_LOCAL_MESH_POWER_MODE]) {
		enum nl80211_mesh_power_mode pm = nla_get_u32(
			info->attrs[NL80211_ATTR_LOCAL_MESH_POWER_MODE]);

		if (pm <= NL80211_MESH_POWER_UNKNOWN ||
		    pm > NL80211_MESH_POWER_MAX)
			return -EINVAL;

		params.local_pm = pm;
	}

	/* Include parameters for TDLS peer (will check later) */
	err = nl80211_set_station_tdls(info, &params);
	if (err)
		return err;

	params.vlan = get_vlan(info, rdev);
	if (IS_ERR(params.vlan))
		return PTR_ERR(params.vlan);

>>>>>>> refs/remotes/origin/master
	switch (dev->ieee80211_ptr->iftype) {
	case NL80211_IFTYPE_AP:
	case NL80211_IFTYPE_AP_VLAN:
	case NL80211_IFTYPE_P2P_GO:
<<<<<<< HEAD
		/* disallow mesh-specific things */
		if (params.plink_action)
<<<<<<< HEAD
			err = -EINVAL;
		break;
	case NL80211_IFTYPE_P2P_CLIENT:
	case NL80211_IFTYPE_STATION:
		/* disallow everything but AUTHORIZED flag */
		if (params.plink_action)
			err = -EINVAL;
		if (params.vlan)
			err = -EINVAL;
		if (params.supported_rates)
			err = -EINVAL;
		if (params.ht_capa)
			err = -EINVAL;
		if (params.listen_interval >= 0)
			err = -EINVAL;
		if (params.sta_flags_mask & ~BIT(NL80211_STA_FLAG_AUTHORIZED))
			err = -EINVAL;
=======
			return -EINVAL;

		/* TDLS can't be set, ... */
		if (params.sta_flags_set & BIT(NL80211_STA_FLAG_TDLS_PEER))
			return -EINVAL;
		/*
		 * ... but don't bother the driver with it. This works around
		 * a hostapd/wpa_supplicant issue -- it always includes the
		 * TLDS_PEER flag in the mask even for AP mode.
		 */
		params.sta_flags_mask &= ~BIT(NL80211_STA_FLAG_TDLS_PEER);

		/* accept only the listed bits */
		if (params.sta_flags_mask &
				~(BIT(NL80211_STA_FLAG_AUTHORIZED) |
				  BIT(NL80211_STA_FLAG_SHORT_PREAMBLE) |
				  BIT(NL80211_STA_FLAG_WME) |
				  BIT(NL80211_STA_FLAG_MFP)))
			return -EINVAL;

		/* must be last in here for error handling */
		params.vlan = get_vlan(info, rdev);
		if (IS_ERR(params.vlan))
			return PTR_ERR(params.vlan);
		break;
	case NL80211_IFTYPE_P2P_CLIENT:
	case NL80211_IFTYPE_STATION:
		/*
		 * Don't allow userspace to change the TDLS_PEER flag,
		 * but silently ignore attempts to change it since we
		 * don't have state here to verify that it doesn't try
		 * to change the flag.
		 */
		params.sta_flags_mask &= ~BIT(NL80211_STA_FLAG_TDLS_PEER);
		/* fall through */
	case NL80211_IFTYPE_ADHOC:
		/* disallow things sta doesn't support */
		if (params.plink_action)
			return -EINVAL;
		if (params.ht_capa)
			return -EINVAL;
		if (params.listen_interval >= 0)
			return -EINVAL;
		/* reject any changes other than AUTHORIZED */
		if (params.sta_flags_mask & ~BIT(NL80211_STA_FLAG_AUTHORIZED))
			return -EINVAL;
>>>>>>> refs/remotes/origin/cm-10.0
		break;
	case NL80211_IFTYPE_MESH_POINT:
		/* disallow things mesh doesn't support */
		if (params.vlan)
<<<<<<< HEAD
			err = -EINVAL;
		if (params.ht_capa)
			err = -EINVAL;
		if (params.listen_interval >= 0)
			err = -EINVAL;
=======
			return -EINVAL;
		if (params.ht_capa)
			return -EINVAL;
		if (params.listen_interval >= 0)
			return -EINVAL;
		/*
		 * No special handling for TDLS here -- the userspace
		 * mesh code doesn't have this bug.
		 */
>>>>>>> refs/remotes/origin/cm-10.0
		if (params.sta_flags_mask &
				~(BIT(NL80211_STA_FLAG_AUTHENTICATED) |
				  BIT(NL80211_STA_FLAG_MFP) |
				  BIT(NL80211_STA_FLAG_AUTHORIZED)))
<<<<<<< HEAD
			err = -EINVAL;
		break;
	default:
		err = -EINVAL;
	}

	if (err)
		goto out;

	if (!rdev->ops->change_station) {
		err = -EOPNOTSUPP;
		goto out;
	}

	err = rdev->ops->change_station(&rdev->wiphy, dev, mac_addr, &params);

 out:
=======
			return -EINVAL;
		break;
	default:
		return -EOPNOTSUPP;
	}

	/* be aware of params.vlan when changing code here */

	err = rdev->ops->change_station(&rdev->wiphy, dev, mac_addr, &params);

>>>>>>> refs/remotes/origin/cm-10.0
=======
	case NL80211_IFTYPE_P2P_CLIENT:
	case NL80211_IFTYPE_STATION:
	case NL80211_IFTYPE_ADHOC:
	case NL80211_IFTYPE_MESH_POINT:
		break;
	default:
		err = -EOPNOTSUPP;
		goto out_put_vlan;
	}

	/* driver will call cfg80211_check_station_change() */
	err = rdev_change_station(rdev, dev, mac_addr, &params);

 out_put_vlan:
>>>>>>> refs/remotes/origin/master
	if (params.vlan)
		dev_put(params.vlan);

	return err;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
static struct nla_policy
nl80211_sta_wme_policy[NL80211_STA_WME_MAX + 1] __read_mostly = {
	[NL80211_STA_WME_UAPSD_QUEUES] = { .type = NLA_U8 },
	[NL80211_STA_WME_MAX_SP] = { .type = NLA_U8 },
};

>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static int nl80211_new_station(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	int err;
	struct net_device *dev = info->user_ptr[1];
	struct station_parameters params;
	u8 *mac_addr = NULL;

	memset(&params, 0, sizeof(params));

<<<<<<< HEAD
=======
	if (!rdev->ops->add_station)
		return -EOPNOTSUPP;

>>>>>>> refs/remotes/origin/master
	if (!info->attrs[NL80211_ATTR_MAC])
		return -EINVAL;

	if (!info->attrs[NL80211_ATTR_STA_LISTEN_INTERVAL])
		return -EINVAL;

	if (!info->attrs[NL80211_ATTR_STA_SUPPORTED_RATES])
		return -EINVAL;

<<<<<<< HEAD
	if (!info->attrs[NL80211_ATTR_STA_AID])
=======
	if (!info->attrs[NL80211_ATTR_STA_AID] &&
	    !info->attrs[NL80211_ATTR_PEER_AID])
>>>>>>> refs/remotes/origin/master
		return -EINVAL;

	mac_addr = nla_data(info->attrs[NL80211_ATTR_MAC]);
	params.supported_rates =
		nla_data(info->attrs[NL80211_ATTR_STA_SUPPORTED_RATES]);
	params.supported_rates_len =
		nla_len(info->attrs[NL80211_ATTR_STA_SUPPORTED_RATES]);
	params.listen_interval =
		nla_get_u16(info->attrs[NL80211_ATTR_STA_LISTEN_INTERVAL]);

<<<<<<< HEAD
	params.aid = nla_get_u16(info->attrs[NL80211_ATTR_STA_AID]);
	if (!params.aid || params.aid > IEEE80211_MAX_AID)
		return -EINVAL;

=======
	if (info->attrs[NL80211_ATTR_PEER_AID])
		params.aid = nla_get_u16(info->attrs[NL80211_ATTR_PEER_AID]);
	else
		params.aid = nla_get_u16(info->attrs[NL80211_ATTR_STA_AID]);
	if (!params.aid || params.aid > IEEE80211_MAX_AID)
		return -EINVAL;

	if (info->attrs[NL80211_ATTR_STA_CAPABILITY]) {
		params.capability =
			nla_get_u16(info->attrs[NL80211_ATTR_STA_CAPABILITY]);
		params.sta_modify_mask |= STATION_PARAM_APPLY_CAPABILITY;
	}

	if (info->attrs[NL80211_ATTR_STA_EXT_CAPABILITY]) {
		params.ext_capab =
			nla_data(info->attrs[NL80211_ATTR_STA_EXT_CAPABILITY]);
		params.ext_capab_len =
			nla_len(info->attrs[NL80211_ATTR_STA_EXT_CAPABILITY]);
	}

>>>>>>> refs/remotes/origin/master
	if (info->attrs[NL80211_ATTR_HT_CAPABILITY])
		params.ht_capa =
			nla_data(info->attrs[NL80211_ATTR_HT_CAPABILITY]);

<<<<<<< HEAD
	if (info->attrs[NL80211_ATTR_STA_PLINK_ACTION])
		params.plink_action =
		    nla_get_u8(info->attrs[NL80211_ATTR_STA_PLINK_ACTION]);

<<<<<<< HEAD
	if (parse_station_flags(info, &params))
		return -EINVAL;

	if (dev->ieee80211_ptr->iftype != NL80211_IFTYPE_AP &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_AP_VLAN &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_MESH_POINT &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_P2P_GO)
		return -EINVAL;

	err = get_vlan(info, rdev, &params.vlan);
	if (err)
		goto out;

	/* validate settings */
	err = 0;

	if (!rdev->ops->add_station) {
		err = -EOPNOTSUPP;
		goto out;
	}

	err = rdev->ops->add_station(&rdev->wiphy, dev, mac_addr, &params);

 out:
	if (params.vlan)
=======
	if (!rdev->ops->add_station)
		return -EOPNOTSUPP;
=======
	if (info->attrs[NL80211_ATTR_VHT_CAPABILITY])
		params.vht_capa =
			nla_data(info->attrs[NL80211_ATTR_VHT_CAPABILITY]);

	if (info->attrs[NL80211_ATTR_STA_PLINK_ACTION]) {
		params.plink_action =
			nla_get_u8(info->attrs[NL80211_ATTR_STA_PLINK_ACTION]);
		if (params.plink_action >= NUM_NL80211_PLINK_ACTIONS)
			return -EINVAL;
	}

	err = nl80211_parse_sta_channel_info(info, &params);
	if (err)
		return err;

	err = nl80211_parse_sta_wme(info, &params);
	if (err)
		return err;
>>>>>>> refs/remotes/origin/master

	if (parse_station_flags(info, dev->ieee80211_ptr->iftype, &params))
		return -EINVAL;

<<<<<<< HEAD
=======
	/* When you run into this, adjust the code below for the new flag */
	BUILD_BUG_ON(NL80211_STA_FLAG_MAX != 7);

>>>>>>> refs/remotes/origin/master
	switch (dev->ieee80211_ptr->iftype) {
	case NL80211_IFTYPE_AP:
	case NL80211_IFTYPE_AP_VLAN:
	case NL80211_IFTYPE_P2P_GO:
<<<<<<< HEAD
		/* parse WME attributes if sta is WME capable */
		if ((rdev->wiphy.flags & WIPHY_FLAG_AP_UAPSD) &&
		    (params.sta_flags_set & BIT(NL80211_STA_FLAG_WME)) &&
		    info->attrs[NL80211_ATTR_STA_WME]) {
			struct nlattr *tb[NL80211_STA_WME_MAX + 1];
			struct nlattr *nla;

			nla = info->attrs[NL80211_ATTR_STA_WME];
			err = nla_parse_nested(tb, NL80211_STA_WME_MAX, nla,
					       nl80211_sta_wme_policy);
			if (err)
				return err;

			if (tb[NL80211_STA_WME_UAPSD_QUEUES])
				params.uapsd_queues =
				     nla_get_u8(tb[NL80211_STA_WME_UAPSD_QUEUES]);
			if (params.uapsd_queues &
					~IEEE80211_WMM_IE_STA_QOSINFO_AC_MASK)
				return -EINVAL;

			if (tb[NL80211_STA_WME_MAX_SP])
				params.max_sp =
				     nla_get_u8(tb[NL80211_STA_WME_MAX_SP]);

			if (params.max_sp &
					~IEEE80211_WMM_IE_STA_QOSINFO_SP_MASK)
				return -EINVAL;

			params.sta_modify_mask |= STATION_PARAM_APPLY_UAPSD;
		}
		/* TDLS peers cannot be added */
		if (params.sta_flags_set & BIT(NL80211_STA_FLAG_TDLS_PEER))
=======
		/* ignore WME attributes if iface/sta is not capable */
		if (!(rdev->wiphy.flags & WIPHY_FLAG_AP_UAPSD) ||
		    !(params.sta_flags_set & BIT(NL80211_STA_FLAG_WME)))
			params.sta_modify_mask &= ~STATION_PARAM_APPLY_UAPSD;

		/* TDLS peers cannot be added */
		if ((params.sta_flags_set & BIT(NL80211_STA_FLAG_TDLS_PEER)) ||
		    info->attrs[NL80211_ATTR_PEER_AID])
>>>>>>> refs/remotes/origin/master
			return -EINVAL;
		/* but don't bother the driver with it */
		params.sta_flags_mask &= ~BIT(NL80211_STA_FLAG_TDLS_PEER);

<<<<<<< HEAD
=======
		/* allow authenticated/associated only if driver handles it */
		if (!(rdev->wiphy.features &
				NL80211_FEATURE_FULL_AP_CLIENT_STATE) &&
		    params.sta_flags_mask &
				(BIT(NL80211_STA_FLAG_AUTHENTICATED) |
				 BIT(NL80211_STA_FLAG_ASSOCIATED)))
			return -EINVAL;

>>>>>>> refs/remotes/origin/master
		/* must be last in here for error handling */
		params.vlan = get_vlan(info, rdev);
		if (IS_ERR(params.vlan))
			return PTR_ERR(params.vlan);
		break;
	case NL80211_IFTYPE_MESH_POINT:
<<<<<<< HEAD
		/* TDLS peers cannot be added */
		if (params.sta_flags_set & BIT(NL80211_STA_FLAG_TDLS_PEER))
			return -EINVAL;
		break;
	case NL80211_IFTYPE_STATION:
=======
		/* ignore uAPSD data */
		params.sta_modify_mask &= ~STATION_PARAM_APPLY_UAPSD;

		/* associated is disallowed */
		if (params.sta_flags_mask & BIT(NL80211_STA_FLAG_ASSOCIATED))
			return -EINVAL;
		/* TDLS peers cannot be added */
		if ((params.sta_flags_set & BIT(NL80211_STA_FLAG_TDLS_PEER)) ||
		    info->attrs[NL80211_ATTR_PEER_AID])
			return -EINVAL;
		break;
	case NL80211_IFTYPE_STATION:
	case NL80211_IFTYPE_P2P_CLIENT:
		/* ignore uAPSD data */
		params.sta_modify_mask &= ~STATION_PARAM_APPLY_UAPSD;

		/* these are disallowed */
		if (params.sta_flags_mask &
				(BIT(NL80211_STA_FLAG_ASSOCIATED) |
				 BIT(NL80211_STA_FLAG_AUTHENTICATED)))
			return -EINVAL;
>>>>>>> refs/remotes/origin/master
		/* Only TDLS peers can be added */
		if (!(params.sta_flags_set & BIT(NL80211_STA_FLAG_TDLS_PEER)))
			return -EINVAL;
		/* Can only add if TDLS ... */
		if (!(rdev->wiphy.flags & WIPHY_FLAG_SUPPORTS_TDLS))
			return -EOPNOTSUPP;
		/* ... with external setup is supported */
		if (!(rdev->wiphy.flags & WIPHY_FLAG_TDLS_EXTERNAL_SETUP))
			return -EOPNOTSUPP;
<<<<<<< HEAD
=======
		/*
		 * Older wpa_supplicant versions always mark the TDLS peer
		 * as authorized, but it shouldn't yet be.
		 */
		params.sta_flags_mask &= ~BIT(NL80211_STA_FLAG_AUTHORIZED);
>>>>>>> refs/remotes/origin/master
		break;
	default:
		return -EOPNOTSUPP;
	}

	/* be aware of params.vlan when changing code here */

<<<<<<< HEAD
	err = rdev->ops->add_station(&rdev->wiphy, dev, mac_addr, &params);

	if (params.vlan)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	err = rdev_add_station(rdev, dev, mac_addr, &params);

	if (params.vlan)
>>>>>>> refs/remotes/origin/master
		dev_put(params.vlan);
	return err;
}

static int nl80211_del_station(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];
	u8 *mac_addr = NULL;

	if (info->attrs[NL80211_ATTR_MAC])
		mac_addr = nla_data(info->attrs[NL80211_ATTR_MAC]);

	if (dev->ieee80211_ptr->iftype != NL80211_IFTYPE_AP &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_AP_VLAN &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_MESH_POINT &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_P2P_GO)
		return -EINVAL;

	if (!rdev->ops->del_station)
		return -EOPNOTSUPP;

<<<<<<< HEAD
	return rdev->ops->del_station(&rdev->wiphy, dev, mac_addr);
}

static int nl80211_send_mpath(struct sk_buff *msg, u32 pid, u32 seq,
=======
	return rdev_del_station(rdev, dev, mac_addr);
}

static int nl80211_send_mpath(struct sk_buff *msg, u32 portid, u32 seq,
>>>>>>> refs/remotes/origin/master
				int flags, struct net_device *dev,
				u8 *dst, u8 *next_hop,
				struct mpath_info *pinfo)
{
	void *hdr;
	struct nlattr *pinfoattr;

<<<<<<< HEAD
	hdr = nl80211hdr_put(msg, pid, seq, flags, NL80211_CMD_NEW_STATION);
	if (!hdr)
		return -1;

	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, dev->ifindex);
	NLA_PUT(msg, NL80211_ATTR_MAC, ETH_ALEN, dst);
	NLA_PUT(msg, NL80211_ATTR_MPATH_NEXT_HOP, ETH_ALEN, next_hop);

	NLA_PUT_U32(msg, NL80211_ATTR_GENERATION, pinfo->generation);
=======
	hdr = nl80211hdr_put(msg, portid, seq, flags, NL80211_CMD_NEW_STATION);
	if (!hdr)
		return -1;

	if (nla_put_u32(msg, NL80211_ATTR_IFINDEX, dev->ifindex) ||
	    nla_put(msg, NL80211_ATTR_MAC, ETH_ALEN, dst) ||
	    nla_put(msg, NL80211_ATTR_MPATH_NEXT_HOP, ETH_ALEN, next_hop) ||
	    nla_put_u32(msg, NL80211_ATTR_GENERATION, pinfo->generation))
		goto nla_put_failure;
>>>>>>> refs/remotes/origin/master

	pinfoattr = nla_nest_start(msg, NL80211_ATTR_MPATH_INFO);
	if (!pinfoattr)
		goto nla_put_failure;
<<<<<<< HEAD
	if (pinfo->filled & MPATH_INFO_FRAME_QLEN)
		NLA_PUT_U32(msg, NL80211_MPATH_INFO_FRAME_QLEN,
			    pinfo->frame_qlen);
	if (pinfo->filled & MPATH_INFO_SN)
		NLA_PUT_U32(msg, NL80211_MPATH_INFO_SN,
			    pinfo->sn);
	if (pinfo->filled & MPATH_INFO_METRIC)
		NLA_PUT_U32(msg, NL80211_MPATH_INFO_METRIC,
			    pinfo->metric);
	if (pinfo->filled & MPATH_INFO_EXPTIME)
		NLA_PUT_U32(msg, NL80211_MPATH_INFO_EXPTIME,
			    pinfo->exptime);
	if (pinfo->filled & MPATH_INFO_FLAGS)
		NLA_PUT_U8(msg, NL80211_MPATH_INFO_FLAGS,
			    pinfo->flags);
	if (pinfo->filled & MPATH_INFO_DISCOVERY_TIMEOUT)
		NLA_PUT_U32(msg, NL80211_MPATH_INFO_DISCOVERY_TIMEOUT,
			    pinfo->discovery_timeout);
	if (pinfo->filled & MPATH_INFO_DISCOVERY_RETRIES)
		NLA_PUT_U8(msg, NL80211_MPATH_INFO_DISCOVERY_RETRIES,
			    pinfo->discovery_retries);
=======
	if ((pinfo->filled & MPATH_INFO_FRAME_QLEN) &&
	    nla_put_u32(msg, NL80211_MPATH_INFO_FRAME_QLEN,
			pinfo->frame_qlen))
		goto nla_put_failure;
	if (((pinfo->filled & MPATH_INFO_SN) &&
	     nla_put_u32(msg, NL80211_MPATH_INFO_SN, pinfo->sn)) ||
	    ((pinfo->filled & MPATH_INFO_METRIC) &&
	     nla_put_u32(msg, NL80211_MPATH_INFO_METRIC,
			 pinfo->metric)) ||
	    ((pinfo->filled & MPATH_INFO_EXPTIME) &&
	     nla_put_u32(msg, NL80211_MPATH_INFO_EXPTIME,
			 pinfo->exptime)) ||
	    ((pinfo->filled & MPATH_INFO_FLAGS) &&
	     nla_put_u8(msg, NL80211_MPATH_INFO_FLAGS,
			pinfo->flags)) ||
	    ((pinfo->filled & MPATH_INFO_DISCOVERY_TIMEOUT) &&
	     nla_put_u32(msg, NL80211_MPATH_INFO_DISCOVERY_TIMEOUT,
			 pinfo->discovery_timeout)) ||
	    ((pinfo->filled & MPATH_INFO_DISCOVERY_RETRIES) &&
	     nla_put_u8(msg, NL80211_MPATH_INFO_DISCOVERY_RETRIES,
			pinfo->discovery_retries)))
		goto nla_put_failure;
>>>>>>> refs/remotes/origin/master

	nla_nest_end(msg, pinfoattr);

	return genlmsg_end(msg, hdr);

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	return -EMSGSIZE;
}

static int nl80211_dump_mpath(struct sk_buff *skb,
			      struct netlink_callback *cb)
{
	struct mpath_info pinfo;
	struct cfg80211_registered_device *dev;
<<<<<<< HEAD
	struct net_device *netdev;
	u8 dst[ETH_ALEN];
	u8 next_hop[ETH_ALEN];
	int path_idx = cb->args[1];
	int err;

	err = nl80211_prepare_netdev_dump(skb, cb, &dev, &netdev);
=======
	struct wireless_dev *wdev;
	u8 dst[ETH_ALEN];
	u8 next_hop[ETH_ALEN];
	int path_idx = cb->args[2];
	int err;

	err = nl80211_prepare_wdev_dump(skb, cb, &dev, &wdev);
>>>>>>> refs/remotes/origin/master
	if (err)
		return err;

	if (!dev->ops->dump_mpath) {
		err = -EOPNOTSUPP;
		goto out_err;
	}

<<<<<<< HEAD
	if (netdev->ieee80211_ptr->iftype != NL80211_IFTYPE_MESH_POINT) {
=======
	if (wdev->iftype != NL80211_IFTYPE_MESH_POINT) {
>>>>>>> refs/remotes/origin/master
		err = -EOPNOTSUPP;
		goto out_err;
	}

	while (1) {
<<<<<<< HEAD
		err = dev->ops->dump_mpath(&dev->wiphy, netdev, path_idx,
					   dst, next_hop, &pinfo);
=======
		err = rdev_dump_mpath(dev, wdev->netdev, path_idx, dst,
				      next_hop, &pinfo);
>>>>>>> refs/remotes/origin/master
		if (err == -ENOENT)
			break;
		if (err)
			goto out_err;

<<<<<<< HEAD
		if (nl80211_send_mpath(skb, NETLINK_CB(cb->skb).pid,
				       cb->nlh->nlmsg_seq, NLM_F_MULTI,
				       netdev, dst, next_hop,
=======
		if (nl80211_send_mpath(skb, NETLINK_CB(cb->skb).portid,
				       cb->nlh->nlmsg_seq, NLM_F_MULTI,
				       wdev->netdev, dst, next_hop,
>>>>>>> refs/remotes/origin/master
				       &pinfo) < 0)
			goto out;

		path_idx++;
	}


 out:
<<<<<<< HEAD
	cb->args[1] = path_idx;
	err = skb->len;
 out_err:
	nl80211_finish_netdev_dump(dev);
=======
	cb->args[2] = path_idx;
	err = skb->len;
 out_err:
	nl80211_finish_wdev_dump(dev);
>>>>>>> refs/remotes/origin/master
	return err;
}

static int nl80211_get_mpath(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	int err;
	struct net_device *dev = info->user_ptr[1];
	struct mpath_info pinfo;
	struct sk_buff *msg;
	u8 *dst = NULL;
	u8 next_hop[ETH_ALEN];

	memset(&pinfo, 0, sizeof(pinfo));

	if (!info->attrs[NL80211_ATTR_MAC])
		return -EINVAL;

	dst = nla_data(info->attrs[NL80211_ATTR_MAC]);

	if (!rdev->ops->get_mpath)
		return -EOPNOTSUPP;

	if (dev->ieee80211_ptr->iftype != NL80211_IFTYPE_MESH_POINT)
		return -EOPNOTSUPP;

<<<<<<< HEAD
	err = rdev->ops->get_mpath(&rdev->wiphy, dev, dst, next_hop, &pinfo);
=======
	err = rdev_get_mpath(rdev, dev, dst, next_hop, &pinfo);
>>>>>>> refs/remotes/origin/master
	if (err)
		return err;

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (!msg)
		return -ENOMEM;

<<<<<<< HEAD
	if (nl80211_send_mpath(msg, info->snd_pid, info->snd_seq, 0,
=======
	if (nl80211_send_mpath(msg, info->snd_portid, info->snd_seq, 0,
>>>>>>> refs/remotes/origin/master
				 dev, dst, next_hop, &pinfo) < 0) {
		nlmsg_free(msg);
		return -ENOBUFS;
	}

	return genlmsg_reply(msg, info);
}

static int nl80211_set_mpath(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];
	u8 *dst = NULL;
	u8 *next_hop = NULL;

	if (!info->attrs[NL80211_ATTR_MAC])
		return -EINVAL;

	if (!info->attrs[NL80211_ATTR_MPATH_NEXT_HOP])
		return -EINVAL;

	dst = nla_data(info->attrs[NL80211_ATTR_MAC]);
	next_hop = nla_data(info->attrs[NL80211_ATTR_MPATH_NEXT_HOP]);

	if (!rdev->ops->change_mpath)
		return -EOPNOTSUPP;

	if (dev->ieee80211_ptr->iftype != NL80211_IFTYPE_MESH_POINT)
		return -EOPNOTSUPP;

<<<<<<< HEAD
	return rdev->ops->change_mpath(&rdev->wiphy, dev, dst, next_hop);
=======
	return rdev_change_mpath(rdev, dev, dst, next_hop);
>>>>>>> refs/remotes/origin/master
}

static int nl80211_new_mpath(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];
	u8 *dst = NULL;
	u8 *next_hop = NULL;

	if (!info->attrs[NL80211_ATTR_MAC])
		return -EINVAL;

	if (!info->attrs[NL80211_ATTR_MPATH_NEXT_HOP])
		return -EINVAL;

	dst = nla_data(info->attrs[NL80211_ATTR_MAC]);
	next_hop = nla_data(info->attrs[NL80211_ATTR_MPATH_NEXT_HOP]);

	if (!rdev->ops->add_mpath)
		return -EOPNOTSUPP;

	if (dev->ieee80211_ptr->iftype != NL80211_IFTYPE_MESH_POINT)
		return -EOPNOTSUPP;

<<<<<<< HEAD
	return rdev->ops->add_mpath(&rdev->wiphy, dev, dst, next_hop);
=======
	return rdev_add_mpath(rdev, dev, dst, next_hop);
>>>>>>> refs/remotes/origin/master
}

static int nl80211_del_mpath(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];
	u8 *dst = NULL;

	if (info->attrs[NL80211_ATTR_MAC])
		dst = nla_data(info->attrs[NL80211_ATTR_MAC]);

	if (!rdev->ops->del_mpath)
		return -EOPNOTSUPP;

<<<<<<< HEAD
	return rdev->ops->del_mpath(&rdev->wiphy, dev, dst);
=======
	return rdev_del_mpath(rdev, dev, dst);
>>>>>>> refs/remotes/origin/master
}

static int nl80211_set_bss(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];
	struct bss_parameters params;

	memset(&params, 0, sizeof(params));
	/* default to not changing parameters */
	params.use_cts_prot = -1;
	params.use_short_preamble = -1;
	params.use_short_slot_time = -1;
	params.ap_isolate = -1;
	params.ht_opmode = -1;
<<<<<<< HEAD
=======
	params.p2p_ctwindow = -1;
	params.p2p_opp_ps = -1;
>>>>>>> refs/remotes/origin/master

	if (info->attrs[NL80211_ATTR_BSS_CTS_PROT])
		params.use_cts_prot =
		    nla_get_u8(info->attrs[NL80211_ATTR_BSS_CTS_PROT]);
	if (info->attrs[NL80211_ATTR_BSS_SHORT_PREAMBLE])
		params.use_short_preamble =
		    nla_get_u8(info->attrs[NL80211_ATTR_BSS_SHORT_PREAMBLE]);
	if (info->attrs[NL80211_ATTR_BSS_SHORT_SLOT_TIME])
		params.use_short_slot_time =
		    nla_get_u8(info->attrs[NL80211_ATTR_BSS_SHORT_SLOT_TIME]);
	if (info->attrs[NL80211_ATTR_BSS_BASIC_RATES]) {
		params.basic_rates =
			nla_data(info->attrs[NL80211_ATTR_BSS_BASIC_RATES]);
		params.basic_rates_len =
			nla_len(info->attrs[NL80211_ATTR_BSS_BASIC_RATES]);
	}
	if (info->attrs[NL80211_ATTR_AP_ISOLATE])
		params.ap_isolate = !!nla_get_u8(info->attrs[NL80211_ATTR_AP_ISOLATE]);
	if (info->attrs[NL80211_ATTR_BSS_HT_OPMODE])
		params.ht_opmode =
			nla_get_u16(info->attrs[NL80211_ATTR_BSS_HT_OPMODE]);

<<<<<<< HEAD
=======
	if (info->attrs[NL80211_ATTR_P2P_CTWINDOW]) {
		if (dev->ieee80211_ptr->iftype != NL80211_IFTYPE_P2P_GO)
			return -EINVAL;
		params.p2p_ctwindow =
			nla_get_s8(info->attrs[NL80211_ATTR_P2P_CTWINDOW]);
		if (params.p2p_ctwindow < 0)
			return -EINVAL;
		if (params.p2p_ctwindow != 0 &&
		    !(rdev->wiphy.features & NL80211_FEATURE_P2P_GO_CTWIN))
			return -EINVAL;
	}

	if (info->attrs[NL80211_ATTR_P2P_OPPPS]) {
		u8 tmp;

		if (dev->ieee80211_ptr->iftype != NL80211_IFTYPE_P2P_GO)
			return -EINVAL;
		tmp = nla_get_u8(info->attrs[NL80211_ATTR_P2P_OPPPS]);
		if (tmp > 1)
			return -EINVAL;
		params.p2p_opp_ps = tmp;
		if (params.p2p_opp_ps &&
		    !(rdev->wiphy.features & NL80211_FEATURE_P2P_GO_OPPPS))
			return -EINVAL;
	}

>>>>>>> refs/remotes/origin/master
	if (!rdev->ops->change_bss)
		return -EOPNOTSUPP;

	if (dev->ieee80211_ptr->iftype != NL80211_IFTYPE_AP &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_P2P_GO)
		return -EOPNOTSUPP;

<<<<<<< HEAD
	return rdev->ops->change_bss(&rdev->wiphy, dev, &params);
=======
	return rdev_change_bss(rdev, dev, &params);
>>>>>>> refs/remotes/origin/master
}

static const struct nla_policy reg_rule_policy[NL80211_REG_RULE_ATTR_MAX + 1] = {
	[NL80211_ATTR_REG_RULE_FLAGS]		= { .type = NLA_U32 },
	[NL80211_ATTR_FREQ_RANGE_START]		= { .type = NLA_U32 },
	[NL80211_ATTR_FREQ_RANGE_END]		= { .type = NLA_U32 },
	[NL80211_ATTR_FREQ_RANGE_MAX_BW]	= { .type = NLA_U32 },
	[NL80211_ATTR_POWER_RULE_MAX_ANT_GAIN]	= { .type = NLA_U32 },
	[NL80211_ATTR_POWER_RULE_MAX_EIRP]	= { .type = NLA_U32 },
};

static int parse_reg_rule(struct nlattr *tb[],
	struct ieee80211_reg_rule *reg_rule)
{
	struct ieee80211_freq_range *freq_range = &reg_rule->freq_range;
	struct ieee80211_power_rule *power_rule = &reg_rule->power_rule;

	if (!tb[NL80211_ATTR_REG_RULE_FLAGS])
		return -EINVAL;
	if (!tb[NL80211_ATTR_FREQ_RANGE_START])
		return -EINVAL;
	if (!tb[NL80211_ATTR_FREQ_RANGE_END])
		return -EINVAL;
	if (!tb[NL80211_ATTR_FREQ_RANGE_MAX_BW])
		return -EINVAL;
	if (!tb[NL80211_ATTR_POWER_RULE_MAX_EIRP])
		return -EINVAL;

	reg_rule->flags = nla_get_u32(tb[NL80211_ATTR_REG_RULE_FLAGS]);

	freq_range->start_freq_khz =
		nla_get_u32(tb[NL80211_ATTR_FREQ_RANGE_START]);
	freq_range->end_freq_khz =
		nla_get_u32(tb[NL80211_ATTR_FREQ_RANGE_END]);
	freq_range->max_bandwidth_khz =
		nla_get_u32(tb[NL80211_ATTR_FREQ_RANGE_MAX_BW]);

	power_rule->max_eirp =
		nla_get_u32(tb[NL80211_ATTR_POWER_RULE_MAX_EIRP]);

	if (tb[NL80211_ATTR_POWER_RULE_MAX_ANT_GAIN])
		power_rule->max_antenna_gain =
			nla_get_u32(tb[NL80211_ATTR_POWER_RULE_MAX_ANT_GAIN]);

	return 0;
}

static int nl80211_req_set_reg(struct sk_buff *skb, struct genl_info *info)
{
	int r;
	char *data = NULL;
<<<<<<< HEAD
=======
	enum nl80211_user_reg_hint_type user_reg_hint_type;
>>>>>>> refs/remotes/origin/master

	/*
	 * You should only get this when cfg80211 hasn't yet initialized
	 * completely when built-in to the kernel right between the time
	 * window between nl80211_init() and regulatory_init(), if that is
	 * even possible.
	 */
<<<<<<< HEAD
	mutex_lock(&cfg80211_mutex);
	if (unlikely(!cfg80211_regdomain)) {
		mutex_unlock(&cfg80211_mutex);
		return -EINPROGRESS;
	}
	mutex_unlock(&cfg80211_mutex);
=======
	if (unlikely(!rcu_access_pointer(cfg80211_regdomain)))
		return -EINPROGRESS;
>>>>>>> refs/remotes/origin/master

	if (!info->attrs[NL80211_ATTR_REG_ALPHA2])
		return -EINVAL;

	data = nla_data(info->attrs[NL80211_ATTR_REG_ALPHA2]);

<<<<<<< HEAD
	r = regulatory_hint_user(data);
=======
	if (info->attrs[NL80211_ATTR_USER_REG_HINT_TYPE])
		user_reg_hint_type =
		  nla_get_u32(info->attrs[NL80211_ATTR_USER_REG_HINT_TYPE]);
	else
		user_reg_hint_type = NL80211_USER_REG_HINT_USER;

	switch (user_reg_hint_type) {
	case NL80211_USER_REG_HINT_USER:
	case NL80211_USER_REG_HINT_CELL_BASE:
		break;
	default:
		return -EINVAL;
	}

	r = regulatory_hint_user(data, user_reg_hint_type);
>>>>>>> refs/remotes/origin/master

	return r;
}

static int nl80211_get_mesh_config(struct sk_buff *skb,
				   struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct mesh_config cur_params;
	int err = 0;
	void *hdr;
	struct nlattr *pinfoattr;
	struct sk_buff *msg;

	if (wdev->iftype != NL80211_IFTYPE_MESH_POINT)
		return -EOPNOTSUPP;

	if (!rdev->ops->get_mesh_config)
		return -EOPNOTSUPP;

	wdev_lock(wdev);
	/* If not connected, get default parameters */
	if (!wdev->mesh_id_len)
		memcpy(&cur_params, &default_mesh_config, sizeof(cur_params));
	else
<<<<<<< HEAD
		err = rdev->ops->get_mesh_config(&rdev->wiphy, dev,
						 &cur_params);
=======
		err = rdev_get_mesh_config(rdev, dev, &cur_params);
>>>>>>> refs/remotes/origin/master
	wdev_unlock(wdev);

	if (err)
		return err;

	/* Draw up a netlink message to send back */
	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (!msg)
		return -ENOMEM;
<<<<<<< HEAD
	hdr = nl80211hdr_put(msg, info->snd_pid, info->snd_seq, 0,
=======
	hdr = nl80211hdr_put(msg, info->snd_portid, info->snd_seq, 0,
>>>>>>> refs/remotes/origin/master
			     NL80211_CMD_GET_MESH_CONFIG);
	if (!hdr)
		goto out;
	pinfoattr = nla_nest_start(msg, NL80211_ATTR_MESH_CONFIG);
	if (!pinfoattr)
		goto nla_put_failure;
<<<<<<< HEAD
	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, dev->ifindex);
	NLA_PUT_U16(msg, NL80211_MESHCONF_RETRY_TIMEOUT,
			cur_params.dot11MeshRetryTimeout);
	NLA_PUT_U16(msg, NL80211_MESHCONF_CONFIRM_TIMEOUT,
			cur_params.dot11MeshConfirmTimeout);
	NLA_PUT_U16(msg, NL80211_MESHCONF_HOLDING_TIMEOUT,
			cur_params.dot11MeshHoldingTimeout);
	NLA_PUT_U16(msg, NL80211_MESHCONF_MAX_PEER_LINKS,
			cur_params.dot11MeshMaxPeerLinks);
	NLA_PUT_U8(msg, NL80211_MESHCONF_MAX_RETRIES,
			cur_params.dot11MeshMaxRetries);
	NLA_PUT_U8(msg, NL80211_MESHCONF_TTL,
			cur_params.dot11MeshTTL);
	NLA_PUT_U8(msg, NL80211_MESHCONF_ELEMENT_TTL,
			cur_params.element_ttl);
	NLA_PUT_U8(msg, NL80211_MESHCONF_AUTO_OPEN_PLINKS,
			cur_params.auto_open_plinks);
	NLA_PUT_U8(msg, NL80211_MESHCONF_HWMP_MAX_PREQ_RETRIES,
			cur_params.dot11MeshHWMPmaxPREQretries);
	NLA_PUT_U32(msg, NL80211_MESHCONF_PATH_REFRESH_TIME,
			cur_params.path_refresh_time);
	NLA_PUT_U16(msg, NL80211_MESHCONF_MIN_DISCOVERY_TIMEOUT,
			cur_params.min_discovery_timeout);
	NLA_PUT_U32(msg, NL80211_MESHCONF_HWMP_ACTIVE_PATH_TIMEOUT,
			cur_params.dot11MeshHWMPactivePathTimeout);
	NLA_PUT_U16(msg, NL80211_MESHCONF_HWMP_PREQ_MIN_INTERVAL,
			cur_params.dot11MeshHWMPpreqMinInterval);
<<<<<<< HEAD
=======
	NLA_PUT_U16(msg, NL80211_MESHCONF_HWMP_PERR_MIN_INTERVAL,
			cur_params.dot11MeshHWMPperrMinInterval);
>>>>>>> refs/remotes/origin/cm-10.0
	NLA_PUT_U16(msg, NL80211_MESHCONF_HWMP_NET_DIAM_TRVS_TIME,
			cur_params.dot11MeshHWMPnetDiameterTraversalTime);
	NLA_PUT_U8(msg, NL80211_MESHCONF_HWMP_ROOTMODE,
			cur_params.dot11MeshHWMPRootMode);
<<<<<<< HEAD
=======
	NLA_PUT_U16(msg, NL80211_MESHCONF_HWMP_RANN_INTERVAL,
			cur_params.dot11MeshHWMPRannInterval);
	NLA_PUT_U8(msg, NL80211_MESHCONF_GATE_ANNOUNCEMENTS,
			cur_params.dot11MeshGateAnnouncementProtocol);
	NLA_PUT_U8(msg, NL80211_MESHCONF_FORWARDING,
			cur_params.dot11MeshForwarding);
	NLA_PUT_U32(msg, NL80211_MESHCONF_RSSI_THRESHOLD,
			cur_params.rssi_threshold);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (nla_put_u32(msg, NL80211_ATTR_IFINDEX, dev->ifindex) ||
	    nla_put_u16(msg, NL80211_MESHCONF_RETRY_TIMEOUT,
			cur_params.dot11MeshRetryTimeout) ||
	    nla_put_u16(msg, NL80211_MESHCONF_CONFIRM_TIMEOUT,
			cur_params.dot11MeshConfirmTimeout) ||
	    nla_put_u16(msg, NL80211_MESHCONF_HOLDING_TIMEOUT,
			cur_params.dot11MeshHoldingTimeout) ||
	    nla_put_u16(msg, NL80211_MESHCONF_MAX_PEER_LINKS,
			cur_params.dot11MeshMaxPeerLinks) ||
	    nla_put_u8(msg, NL80211_MESHCONF_MAX_RETRIES,
		       cur_params.dot11MeshMaxRetries) ||
	    nla_put_u8(msg, NL80211_MESHCONF_TTL,
		       cur_params.dot11MeshTTL) ||
	    nla_put_u8(msg, NL80211_MESHCONF_ELEMENT_TTL,
		       cur_params.element_ttl) ||
	    nla_put_u8(msg, NL80211_MESHCONF_AUTO_OPEN_PLINKS,
		       cur_params.auto_open_plinks) ||
	    nla_put_u32(msg, NL80211_MESHCONF_SYNC_OFFSET_MAX_NEIGHBOR,
			cur_params.dot11MeshNbrOffsetMaxNeighbor) ||
	    nla_put_u8(msg, NL80211_MESHCONF_HWMP_MAX_PREQ_RETRIES,
		       cur_params.dot11MeshHWMPmaxPREQretries) ||
	    nla_put_u32(msg, NL80211_MESHCONF_PATH_REFRESH_TIME,
			cur_params.path_refresh_time) ||
	    nla_put_u16(msg, NL80211_MESHCONF_MIN_DISCOVERY_TIMEOUT,
			cur_params.min_discovery_timeout) ||
	    nla_put_u32(msg, NL80211_MESHCONF_HWMP_ACTIVE_PATH_TIMEOUT,
			cur_params.dot11MeshHWMPactivePathTimeout) ||
	    nla_put_u16(msg, NL80211_MESHCONF_HWMP_PREQ_MIN_INTERVAL,
			cur_params.dot11MeshHWMPpreqMinInterval) ||
	    nla_put_u16(msg, NL80211_MESHCONF_HWMP_PERR_MIN_INTERVAL,
			cur_params.dot11MeshHWMPperrMinInterval) ||
	    nla_put_u16(msg, NL80211_MESHCONF_HWMP_NET_DIAM_TRVS_TIME,
			cur_params.dot11MeshHWMPnetDiameterTraversalTime) ||
	    nla_put_u8(msg, NL80211_MESHCONF_HWMP_ROOTMODE,
		       cur_params.dot11MeshHWMPRootMode) ||
	    nla_put_u16(msg, NL80211_MESHCONF_HWMP_RANN_INTERVAL,
			cur_params.dot11MeshHWMPRannInterval) ||
	    nla_put_u8(msg, NL80211_MESHCONF_GATE_ANNOUNCEMENTS,
		       cur_params.dot11MeshGateAnnouncementProtocol) ||
	    nla_put_u8(msg, NL80211_MESHCONF_FORWARDING,
		       cur_params.dot11MeshForwarding) ||
	    nla_put_u32(msg, NL80211_MESHCONF_RSSI_THRESHOLD,
			cur_params.rssi_threshold) ||
	    nla_put_u32(msg, NL80211_MESHCONF_HT_OPMODE,
			cur_params.ht_opmode) ||
	    nla_put_u32(msg, NL80211_MESHCONF_HWMP_PATH_TO_ROOT_TIMEOUT,
			cur_params.dot11MeshHWMPactivePathToRootTimeout) ||
	    nla_put_u16(msg, NL80211_MESHCONF_HWMP_ROOT_INTERVAL,
			cur_params.dot11MeshHWMProotInterval) ||
	    nla_put_u16(msg, NL80211_MESHCONF_HWMP_CONFIRMATION_INTERVAL,
			cur_params.dot11MeshHWMPconfirmationInterval) ||
	    nla_put_u32(msg, NL80211_MESHCONF_POWER_MODE,
			cur_params.power_mode) ||
	    nla_put_u16(msg, NL80211_MESHCONF_AWAKE_WINDOW,
			cur_params.dot11MeshAwakeWindowDuration) ||
	    nla_put_u32(msg, NL80211_MESHCONF_PLINK_TIMEOUT,
			cur_params.plink_timeout))
		goto nla_put_failure;
>>>>>>> refs/remotes/origin/master
	nla_nest_end(msg, pinfoattr);
	genlmsg_end(msg, hdr);
	return genlmsg_reply(msg, info);

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
 out:
	nlmsg_free(msg);
	return -ENOBUFS;
}

static const struct nla_policy nl80211_meshconf_params_policy[NL80211_MESHCONF_ATTR_MAX+1] = {
	[NL80211_MESHCONF_RETRY_TIMEOUT] = { .type = NLA_U16 },
	[NL80211_MESHCONF_CONFIRM_TIMEOUT] = { .type = NLA_U16 },
	[NL80211_MESHCONF_HOLDING_TIMEOUT] = { .type = NLA_U16 },
	[NL80211_MESHCONF_MAX_PEER_LINKS] = { .type = NLA_U16 },
	[NL80211_MESHCONF_MAX_RETRIES] = { .type = NLA_U8 },
	[NL80211_MESHCONF_TTL] = { .type = NLA_U8 },
	[NL80211_MESHCONF_ELEMENT_TTL] = { .type = NLA_U8 },
	[NL80211_MESHCONF_AUTO_OPEN_PLINKS] = { .type = NLA_U8 },
<<<<<<< HEAD

=======
	[NL80211_MESHCONF_SYNC_OFFSET_MAX_NEIGHBOR] = { .type = NLA_U32 },
>>>>>>> refs/remotes/origin/master
	[NL80211_MESHCONF_HWMP_MAX_PREQ_RETRIES] = { .type = NLA_U8 },
	[NL80211_MESHCONF_PATH_REFRESH_TIME] = { .type = NLA_U32 },
	[NL80211_MESHCONF_MIN_DISCOVERY_TIMEOUT] = { .type = NLA_U16 },
	[NL80211_MESHCONF_HWMP_ACTIVE_PATH_TIMEOUT] = { .type = NLA_U32 },
	[NL80211_MESHCONF_HWMP_PREQ_MIN_INTERVAL] = { .type = NLA_U16 },
<<<<<<< HEAD
<<<<<<< HEAD
	[NL80211_MESHCONF_HWMP_NET_DIAM_TRVS_TIME] = { .type = NLA_U16 },
=======
=======
>>>>>>> refs/remotes/origin/master
	[NL80211_MESHCONF_HWMP_PERR_MIN_INTERVAL] = { .type = NLA_U16 },
	[NL80211_MESHCONF_HWMP_NET_DIAM_TRVS_TIME] = { .type = NLA_U16 },
	[NL80211_MESHCONF_HWMP_ROOTMODE] = { .type = NLA_U8 },
	[NL80211_MESHCONF_HWMP_RANN_INTERVAL] = { .type = NLA_U16 },
	[NL80211_MESHCONF_GATE_ANNOUNCEMENTS] = { .type = NLA_U8 },
	[NL80211_MESHCONF_FORWARDING] = { .type = NLA_U8 },
<<<<<<< HEAD
	[NL80211_MESHCONF_RSSI_THRESHOLD] = { .type = NLA_U32},
>>>>>>> refs/remotes/origin/cm-10.0
=======
	[NL80211_MESHCONF_RSSI_THRESHOLD] = { .type = NLA_U32 },
	[NL80211_MESHCONF_HT_OPMODE] = { .type = NLA_U16 },
	[NL80211_MESHCONF_HWMP_PATH_TO_ROOT_TIMEOUT] = { .type = NLA_U32 },
	[NL80211_MESHCONF_HWMP_ROOT_INTERVAL] = { .type = NLA_U16 },
	[NL80211_MESHCONF_HWMP_CONFIRMATION_INTERVAL] = { .type = NLA_U16 },
	[NL80211_MESHCONF_POWER_MODE] = { .type = NLA_U32 },
	[NL80211_MESHCONF_AWAKE_WINDOW] = { .type = NLA_U16 },
	[NL80211_MESHCONF_PLINK_TIMEOUT] = { .type = NLA_U32 },
>>>>>>> refs/remotes/origin/master
};

static const struct nla_policy
	nl80211_mesh_setup_params_policy[NL80211_MESH_SETUP_ATTR_MAX+1] = {
<<<<<<< HEAD
	[NL80211_MESH_SETUP_ENABLE_VENDOR_PATH_SEL] = { .type = NLA_U8 },
	[NL80211_MESH_SETUP_ENABLE_VENDOR_METRIC] = { .type = NLA_U8 },
	[NL80211_MESH_SETUP_USERSPACE_AUTH] = { .type = NLA_FLAG },
	[NL80211_MESH_SETUP_IE] = { .type = NLA_BINARY,
		.len = IEEE80211_MAX_DATA_LEN },
=======
	[NL80211_MESH_SETUP_ENABLE_VENDOR_SYNC] = { .type = NLA_U8 },
	[NL80211_MESH_SETUP_ENABLE_VENDOR_PATH_SEL] = { .type = NLA_U8 },
	[NL80211_MESH_SETUP_ENABLE_VENDOR_METRIC] = { .type = NLA_U8 },
	[NL80211_MESH_SETUP_USERSPACE_AUTH] = { .type = NLA_FLAG },
	[NL80211_MESH_SETUP_AUTH_PROTOCOL] = { .type = NLA_U8 },
	[NL80211_MESH_SETUP_USERSPACE_MPM] = { .type = NLA_FLAG },
	[NL80211_MESH_SETUP_IE] = { .type = NLA_BINARY,
				    .len = IEEE80211_MAX_DATA_LEN },
>>>>>>> refs/remotes/origin/master
	[NL80211_MESH_SETUP_USERSPACE_AMPE] = { .type = NLA_FLAG },
};

static int nl80211_parse_mesh_config(struct genl_info *info,
				     struct mesh_config *cfg,
				     u32 *mask_out)
{
	struct nlattr *tb[NL80211_MESHCONF_ATTR_MAX + 1];
	u32 mask = 0;

<<<<<<< HEAD
#define FILL_IN_MESH_PARAM_IF_SET(table, cfg, param, mask, attr_num, nla_fn) \
do {\
	if (table[attr_num]) {\
		cfg->param = nla_fn(table[attr_num]); \
		mask |= (1 << (attr_num - 1)); \
	} \
} while (0);\
=======
#define FILL_IN_MESH_PARAM_IF_SET(tb, cfg, param, min, max, mask, attr, fn) \
do {									    \
	if (tb[attr]) {							    \
		if (fn(tb[attr]) < min || fn(tb[attr]) > max)		    \
			return -EINVAL;					    \
		cfg->param = fn(tb[attr]);				    \
		mask |= (1 << (attr - 1));				    \
	}								    \
} while (0)
>>>>>>> refs/remotes/origin/master


	if (!info->attrs[NL80211_ATTR_MESH_CONFIG])
		return -EINVAL;
	if (nla_parse_nested(tb, NL80211_MESHCONF_ATTR_MAX,
			     info->attrs[NL80211_ATTR_MESH_CONFIG],
			     nl80211_meshconf_params_policy))
		return -EINVAL;

	/* This makes sure that there aren't more than 32 mesh config
	 * parameters (otherwise our bitfield scheme would not work.) */
	BUILD_BUG_ON(NL80211_MESHCONF_ATTR_MAX > 32);

	/* Fill in the params struct */
<<<<<<< HEAD
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, dot11MeshRetryTimeout,
			mask, NL80211_MESHCONF_RETRY_TIMEOUT, nla_get_u16);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, dot11MeshConfirmTimeout,
			mask, NL80211_MESHCONF_CONFIRM_TIMEOUT, nla_get_u16);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, dot11MeshHoldingTimeout,
			mask, NL80211_MESHCONF_HOLDING_TIMEOUT, nla_get_u16);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, dot11MeshMaxPeerLinks,
			mask, NL80211_MESHCONF_MAX_PEER_LINKS, nla_get_u16);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, dot11MeshMaxRetries,
			mask, NL80211_MESHCONF_MAX_RETRIES, nla_get_u8);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, dot11MeshTTL,
			mask, NL80211_MESHCONF_TTL, nla_get_u8);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, element_ttl,
			mask, NL80211_MESHCONF_ELEMENT_TTL, nla_get_u8);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, auto_open_plinks,
			mask, NL80211_MESHCONF_AUTO_OPEN_PLINKS, nla_get_u8);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, dot11MeshHWMPmaxPREQretries,
			mask, NL80211_MESHCONF_HWMP_MAX_PREQ_RETRIES,
			nla_get_u8);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, path_refresh_time,
			mask, NL80211_MESHCONF_PATH_REFRESH_TIME, nla_get_u32);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, min_discovery_timeout,
			mask, NL80211_MESHCONF_MIN_DISCOVERY_TIMEOUT,
			nla_get_u16);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, dot11MeshHWMPactivePathTimeout,
			mask, NL80211_MESHCONF_HWMP_ACTIVE_PATH_TIMEOUT,
			nla_get_u32);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, dot11MeshHWMPpreqMinInterval,
			mask, NL80211_MESHCONF_HWMP_PREQ_MIN_INTERVAL,
			nla_get_u16);
<<<<<<< HEAD
=======
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, dot11MeshHWMPperrMinInterval,
			mask, NL80211_MESHCONF_HWMP_PERR_MIN_INTERVAL,
			nla_get_u16);
>>>>>>> refs/remotes/origin/cm-10.0
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg,
			dot11MeshHWMPnetDiameterTraversalTime,
			mask, NL80211_MESHCONF_HWMP_NET_DIAM_TRVS_TIME,
			nla_get_u16);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg,
			dot11MeshHWMPRootMode, mask,
			NL80211_MESHCONF_HWMP_ROOTMODE,
			nla_get_u8);
<<<<<<< HEAD
=======
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg,
			dot11MeshHWMPRannInterval, mask,
			NL80211_MESHCONF_HWMP_RANN_INTERVAL,
			nla_get_u16);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg,
			dot11MeshGateAnnouncementProtocol, mask,
			NL80211_MESHCONF_GATE_ANNOUNCEMENTS,
			nla_get_u8);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, dot11MeshForwarding,
			mask, NL80211_MESHCONF_FORWARDING, nla_get_u8);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, rssi_threshold,
			mask, NL80211_MESHCONF_RSSI_THRESHOLD, nla_get_u32);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, dot11MeshRetryTimeout, 1, 255,
				  mask, NL80211_MESHCONF_RETRY_TIMEOUT,
				  nla_get_u16);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, dot11MeshConfirmTimeout, 1, 255,
				  mask, NL80211_MESHCONF_CONFIRM_TIMEOUT,
				  nla_get_u16);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, dot11MeshHoldingTimeout, 1, 255,
				  mask, NL80211_MESHCONF_HOLDING_TIMEOUT,
				  nla_get_u16);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, dot11MeshMaxPeerLinks, 0, 255,
				  mask, NL80211_MESHCONF_MAX_PEER_LINKS,
				  nla_get_u16);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, dot11MeshMaxRetries, 0, 16,
				  mask, NL80211_MESHCONF_MAX_RETRIES,
				  nla_get_u8);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, dot11MeshTTL, 1, 255,
				  mask, NL80211_MESHCONF_TTL, nla_get_u8);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, element_ttl, 1, 255,
				  mask, NL80211_MESHCONF_ELEMENT_TTL,
				  nla_get_u8);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, auto_open_plinks, 0, 1,
				  mask, NL80211_MESHCONF_AUTO_OPEN_PLINKS,
				  nla_get_u8);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, dot11MeshNbrOffsetMaxNeighbor,
				  1, 255, mask,
				  NL80211_MESHCONF_SYNC_OFFSET_MAX_NEIGHBOR,
				  nla_get_u32);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, dot11MeshHWMPmaxPREQretries, 0, 255,
				  mask, NL80211_MESHCONF_HWMP_MAX_PREQ_RETRIES,
				  nla_get_u8);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, path_refresh_time, 1, 65535,
				  mask, NL80211_MESHCONF_PATH_REFRESH_TIME,
				  nla_get_u32);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, min_discovery_timeout, 1, 65535,
				  mask, NL80211_MESHCONF_MIN_DISCOVERY_TIMEOUT,
				  nla_get_u16);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, dot11MeshHWMPactivePathTimeout,
				  1, 65535, mask,
				  NL80211_MESHCONF_HWMP_ACTIVE_PATH_TIMEOUT,
				  nla_get_u32);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, dot11MeshHWMPpreqMinInterval,
				  1, 65535, mask,
				  NL80211_MESHCONF_HWMP_PREQ_MIN_INTERVAL,
				  nla_get_u16);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, dot11MeshHWMPperrMinInterval,
				  1, 65535, mask,
				  NL80211_MESHCONF_HWMP_PERR_MIN_INTERVAL,
				  nla_get_u16);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg,
				  dot11MeshHWMPnetDiameterTraversalTime,
				  1, 65535, mask,
				  NL80211_MESHCONF_HWMP_NET_DIAM_TRVS_TIME,
				  nla_get_u16);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, dot11MeshHWMPRootMode, 0, 4,
				  mask, NL80211_MESHCONF_HWMP_ROOTMODE,
				  nla_get_u8);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, dot11MeshHWMPRannInterval, 1, 65535,
				  mask, NL80211_MESHCONF_HWMP_RANN_INTERVAL,
				  nla_get_u16);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg,
				  dot11MeshGateAnnouncementProtocol, 0, 1,
				  mask, NL80211_MESHCONF_GATE_ANNOUNCEMENTS,
				  nla_get_u8);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, dot11MeshForwarding, 0, 1,
				  mask, NL80211_MESHCONF_FORWARDING,
				  nla_get_u8);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, rssi_threshold, -255, 0,
				  mask, NL80211_MESHCONF_RSSI_THRESHOLD,
				  nla_get_s32);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, ht_opmode, 0, 16,
				  mask, NL80211_MESHCONF_HT_OPMODE,
				  nla_get_u16);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, dot11MeshHWMPactivePathToRootTimeout,
				  1, 65535, mask,
				  NL80211_MESHCONF_HWMP_PATH_TO_ROOT_TIMEOUT,
				  nla_get_u32);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, dot11MeshHWMProotInterval, 1, 65535,
				  mask, NL80211_MESHCONF_HWMP_ROOT_INTERVAL,
				  nla_get_u16);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg,
				  dot11MeshHWMPconfirmationInterval,
				  1, 65535, mask,
				  NL80211_MESHCONF_HWMP_CONFIRMATION_INTERVAL,
				  nla_get_u16);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, power_mode,
				  NL80211_MESH_POWER_ACTIVE,
				  NL80211_MESH_POWER_MAX,
				  mask, NL80211_MESHCONF_POWER_MODE,
				  nla_get_u32);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, dot11MeshAwakeWindowDuration,
				  0, 65535, mask,
				  NL80211_MESHCONF_AWAKE_WINDOW, nla_get_u16);
	FILL_IN_MESH_PARAM_IF_SET(tb, cfg, plink_timeout, 1, 0xffffffff,
				  mask, NL80211_MESHCONF_PLINK_TIMEOUT,
				  nla_get_u32);
>>>>>>> refs/remotes/origin/master
	if (mask_out)
		*mask_out = mask;

	return 0;

#undef FILL_IN_MESH_PARAM_IF_SET
}

static int nl80211_parse_mesh_setup(struct genl_info *info,
				     struct mesh_setup *setup)
{
<<<<<<< HEAD
=======
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
>>>>>>> refs/remotes/origin/master
	struct nlattr *tb[NL80211_MESH_SETUP_ATTR_MAX + 1];

	if (!info->attrs[NL80211_ATTR_MESH_SETUP])
		return -EINVAL;
	if (nla_parse_nested(tb, NL80211_MESH_SETUP_ATTR_MAX,
			     info->attrs[NL80211_ATTR_MESH_SETUP],
			     nl80211_mesh_setup_params_policy))
		return -EINVAL;

<<<<<<< HEAD
=======
	if (tb[NL80211_MESH_SETUP_ENABLE_VENDOR_SYNC])
		setup->sync_method =
		(nla_get_u8(tb[NL80211_MESH_SETUP_ENABLE_VENDOR_SYNC])) ?
		 IEEE80211_SYNC_METHOD_VENDOR :
		 IEEE80211_SYNC_METHOD_NEIGHBOR_OFFSET;

>>>>>>> refs/remotes/origin/master
	if (tb[NL80211_MESH_SETUP_ENABLE_VENDOR_PATH_SEL])
		setup->path_sel_proto =
		(nla_get_u8(tb[NL80211_MESH_SETUP_ENABLE_VENDOR_PATH_SEL])) ?
		 IEEE80211_PATH_PROTOCOL_VENDOR :
		 IEEE80211_PATH_PROTOCOL_HWMP;

	if (tb[NL80211_MESH_SETUP_ENABLE_VENDOR_METRIC])
		setup->path_metric =
		(nla_get_u8(tb[NL80211_MESH_SETUP_ENABLE_VENDOR_METRIC])) ?
		 IEEE80211_PATH_METRIC_VENDOR :
		 IEEE80211_PATH_METRIC_AIRTIME;


	if (tb[NL80211_MESH_SETUP_IE]) {
		struct nlattr *ieattr =
			tb[NL80211_MESH_SETUP_IE];
		if (!is_valid_ie_attr(ieattr))
			return -EINVAL;
		setup->ie = nla_data(ieattr);
		setup->ie_len = nla_len(ieattr);
	}
<<<<<<< HEAD
	setup->is_authenticated = nla_get_flag(tb[NL80211_MESH_SETUP_USERSPACE_AUTH]);
	setup->is_secure = nla_get_flag(tb[NL80211_MESH_SETUP_USERSPACE_AMPE]);
=======
	if (tb[NL80211_MESH_SETUP_USERSPACE_MPM] &&
	    !(rdev->wiphy.features & NL80211_FEATURE_USERSPACE_MPM))
		return -EINVAL;
	setup->user_mpm = nla_get_flag(tb[NL80211_MESH_SETUP_USERSPACE_MPM]);
	setup->is_authenticated = nla_get_flag(tb[NL80211_MESH_SETUP_USERSPACE_AUTH]);
	setup->is_secure = nla_get_flag(tb[NL80211_MESH_SETUP_USERSPACE_AMPE]);
	if (setup->is_secure)
		setup->user_mpm = true;

	if (tb[NL80211_MESH_SETUP_AUTH_PROTOCOL]) {
		if (!setup->user_mpm)
			return -EINVAL;
		setup->auth_id =
			nla_get_u8(tb[NL80211_MESH_SETUP_AUTH_PROTOCOL]);
	}
>>>>>>> refs/remotes/origin/master

	return 0;
}

static int nl80211_update_mesh_config(struct sk_buff *skb,
				      struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct mesh_config cfg;
	u32 mask;
	int err;

	if (wdev->iftype != NL80211_IFTYPE_MESH_POINT)
		return -EOPNOTSUPP;

	if (!rdev->ops->update_mesh_config)
		return -EOPNOTSUPP;

	err = nl80211_parse_mesh_config(info, &cfg, &mask);
	if (err)
		return err;

	wdev_lock(wdev);
	if (!wdev->mesh_id_len)
		err = -ENOLINK;

	if (!err)
<<<<<<< HEAD
		err = rdev->ops->update_mesh_config(&rdev->wiphy, dev,
						    mask, &cfg);
=======
		err = rdev_update_mesh_config(rdev, dev, mask, &cfg);
>>>>>>> refs/remotes/origin/master

	wdev_unlock(wdev);

	return err;
}

static int nl80211_get_reg(struct sk_buff *skb, struct genl_info *info)
{
<<<<<<< HEAD
=======
	const struct ieee80211_regdomain *regdom;
>>>>>>> refs/remotes/origin/master
	struct sk_buff *msg;
	void *hdr = NULL;
	struct nlattr *nl_reg_rules;
	unsigned int i;
<<<<<<< HEAD
	int err = -EINVAL;

	mutex_lock(&cfg80211_mutex);

	if (!cfg80211_regdomain)
		goto out;

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (!msg) {
		err = -ENOBUFS;
		goto out;
	}

	hdr = nl80211hdr_put(msg, info->snd_pid, info->snd_seq, 0,
=======

	if (!cfg80211_regdomain)
		return -EINVAL;

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (!msg)
		return -ENOBUFS;

	hdr = nl80211hdr_put(msg, info->snd_portid, info->snd_seq, 0,
>>>>>>> refs/remotes/origin/master
			     NL80211_CMD_GET_REG);
	if (!hdr)
		goto put_failure;

<<<<<<< HEAD
	NLA_PUT_STRING(msg, NL80211_ATTR_REG_ALPHA2,
		cfg80211_regdomain->alpha2);
<<<<<<< HEAD
=======
	if (cfg80211_regdomain->dfs_region)
		NLA_PUT_U8(msg, NL80211_ATTR_DFS_REGION,
			   cfg80211_regdomain->dfs_region);
>>>>>>> refs/remotes/origin/cm-10.0

	nl_reg_rules = nla_nest_start(msg, NL80211_ATTR_REG_RULES);
	if (!nl_reg_rules)
		goto nla_put_failure;

	for (i = 0; i < cfg80211_regdomain->n_reg_rules; i++) {
=======
	if (reg_last_request_cell_base() &&
	    nla_put_u32(msg, NL80211_ATTR_USER_REG_HINT_TYPE,
			NL80211_USER_REG_HINT_CELL_BASE))
		goto nla_put_failure;

	rcu_read_lock();
	regdom = rcu_dereference(cfg80211_regdomain);

	if (nla_put_string(msg, NL80211_ATTR_REG_ALPHA2, regdom->alpha2) ||
	    (regdom->dfs_region &&
	     nla_put_u8(msg, NL80211_ATTR_DFS_REGION, regdom->dfs_region)))
		goto nla_put_failure_rcu;

	nl_reg_rules = nla_nest_start(msg, NL80211_ATTR_REG_RULES);
	if (!nl_reg_rules)
		goto nla_put_failure_rcu;

	for (i = 0; i < regdom->n_reg_rules; i++) {
>>>>>>> refs/remotes/origin/master
		struct nlattr *nl_reg_rule;
		const struct ieee80211_reg_rule *reg_rule;
		const struct ieee80211_freq_range *freq_range;
		const struct ieee80211_power_rule *power_rule;

<<<<<<< HEAD
		reg_rule = &cfg80211_regdomain->reg_rules[i];
=======
		reg_rule = &regdom->reg_rules[i];
>>>>>>> refs/remotes/origin/master
		freq_range = &reg_rule->freq_range;
		power_rule = &reg_rule->power_rule;

		nl_reg_rule = nla_nest_start(msg, i);
		if (!nl_reg_rule)
<<<<<<< HEAD
			goto nla_put_failure;

		NLA_PUT_U32(msg, NL80211_ATTR_REG_RULE_FLAGS,
			reg_rule->flags);
		NLA_PUT_U32(msg, NL80211_ATTR_FREQ_RANGE_START,
			freq_range->start_freq_khz);
		NLA_PUT_U32(msg, NL80211_ATTR_FREQ_RANGE_END,
			freq_range->end_freq_khz);
		NLA_PUT_U32(msg, NL80211_ATTR_FREQ_RANGE_MAX_BW,
			freq_range->max_bandwidth_khz);
		NLA_PUT_U32(msg, NL80211_ATTR_POWER_RULE_MAX_ANT_GAIN,
			power_rule->max_antenna_gain);
		NLA_PUT_U32(msg, NL80211_ATTR_POWER_RULE_MAX_EIRP,
			power_rule->max_eirp);

		nla_nest_end(msg, nl_reg_rule);
	}
=======
			goto nla_put_failure_rcu;

		if (nla_put_u32(msg, NL80211_ATTR_REG_RULE_FLAGS,
				reg_rule->flags) ||
		    nla_put_u32(msg, NL80211_ATTR_FREQ_RANGE_START,
				freq_range->start_freq_khz) ||
		    nla_put_u32(msg, NL80211_ATTR_FREQ_RANGE_END,
				freq_range->end_freq_khz) ||
		    nla_put_u32(msg, NL80211_ATTR_FREQ_RANGE_MAX_BW,
				freq_range->max_bandwidth_khz) ||
		    nla_put_u32(msg, NL80211_ATTR_POWER_RULE_MAX_ANT_GAIN,
				power_rule->max_antenna_gain) ||
		    nla_put_u32(msg, NL80211_ATTR_POWER_RULE_MAX_EIRP,
				power_rule->max_eirp))
			goto nla_put_failure_rcu;

		nla_nest_end(msg, nl_reg_rule);
	}
	rcu_read_unlock();
>>>>>>> refs/remotes/origin/master

	nla_nest_end(msg, nl_reg_rules);

	genlmsg_end(msg, hdr);
<<<<<<< HEAD
	err = genlmsg_reply(msg, info);
	goto out;

=======
	return genlmsg_reply(msg, info);

nla_put_failure_rcu:
	rcu_read_unlock();
>>>>>>> refs/remotes/origin/master
nla_put_failure:
	genlmsg_cancel(msg, hdr);
put_failure:
	nlmsg_free(msg);
<<<<<<< HEAD
	err = -EMSGSIZE;
out:
	mutex_unlock(&cfg80211_mutex);
	return err;
=======
	return -EMSGSIZE;
>>>>>>> refs/remotes/origin/master
}

static int nl80211_set_reg(struct sk_buff *skb, struct genl_info *info)
{
	struct nlattr *tb[NL80211_REG_RULE_ATTR_MAX + 1];
	struct nlattr *nl_reg_rule;
	char *alpha2 = NULL;
	int rem_reg_rules = 0, r = 0;
	u32 num_rules = 0, rule_idx = 0, size_of_regd;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	u8 dfs_region = 0;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	u8 dfs_region = 0;
>>>>>>> refs/remotes/origin/master
	struct ieee80211_regdomain *rd = NULL;

	if (!info->attrs[NL80211_ATTR_REG_ALPHA2])
		return -EINVAL;

	if (!info->attrs[NL80211_ATTR_REG_RULES])
		return -EINVAL;

	alpha2 = nla_data(info->attrs[NL80211_ATTR_REG_ALPHA2]);

<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (info->attrs[NL80211_ATTR_DFS_REGION])
		dfs_region = nla_get_u8(info->attrs[NL80211_ATTR_DFS_REGION]);

>>>>>>> refs/remotes/origin/cm-10.0
	nla_for_each_nested(nl_reg_rule, info->attrs[NL80211_ATTR_REG_RULES],
			rem_reg_rules) {
=======
	if (info->attrs[NL80211_ATTR_DFS_REGION])
		dfs_region = nla_get_u8(info->attrs[NL80211_ATTR_DFS_REGION]);

	nla_for_each_nested(nl_reg_rule, info->attrs[NL80211_ATTR_REG_RULES],
			    rem_reg_rules) {
>>>>>>> refs/remotes/origin/master
		num_rules++;
		if (num_rules > NL80211_MAX_SUPP_REG_RULES)
			return -EINVAL;
	}

<<<<<<< HEAD
	mutex_lock(&cfg80211_mutex);

	if (!reg_is_valid_request(alpha2)) {
		r = -EINVAL;
		goto bad_reg;
	}

	size_of_regd = sizeof(struct ieee80211_regdomain) +
		(num_rules * sizeof(struct ieee80211_reg_rule));

	rd = kzalloc(size_of_regd, GFP_KERNEL);
	if (!rd) {
		r = -ENOMEM;
		goto bad_reg;
	}
=======
	size_of_regd = sizeof(struct ieee80211_regdomain) +
		       num_rules * sizeof(struct ieee80211_reg_rule);

	rd = kzalloc(size_of_regd, GFP_KERNEL);
	if (!rd)
		return -ENOMEM;
>>>>>>> refs/remotes/origin/master

	rd->n_reg_rules = num_rules;
	rd->alpha2[0] = alpha2[0];
	rd->alpha2[1] = alpha2[1];

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * Disable DFS master mode if the DFS region was
	 * not supported or known on this kernel.
	 */
	if (reg_supported_dfs_region(dfs_region))
		rd->dfs_region = dfs_region;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	nla_for_each_nested(nl_reg_rule, info->attrs[NL80211_ATTR_REG_RULES],
			rem_reg_rules) {
		nla_parse(tb, NL80211_REG_RULE_ATTR_MAX,
			nla_data(nl_reg_rule), nla_len(nl_reg_rule),
			reg_rule_policy);
=======
	nla_for_each_nested(nl_reg_rule, info->attrs[NL80211_ATTR_REG_RULES],
			    rem_reg_rules) {
		nla_parse(tb, NL80211_REG_RULE_ATTR_MAX,
			  nla_data(nl_reg_rule), nla_len(nl_reg_rule),
			  reg_rule_policy);
>>>>>>> refs/remotes/origin/master
		r = parse_reg_rule(tb, &rd->reg_rules[rule_idx]);
		if (r)
			goto bad_reg;

		rule_idx++;

		if (rule_idx > NL80211_MAX_SUPP_REG_RULES) {
			r = -EINVAL;
			goto bad_reg;
		}
	}

<<<<<<< HEAD
	BUG_ON(rule_idx != num_rules);

	r = set_regdom(rd);

	mutex_unlock(&cfg80211_mutex);

	return r;

 bad_reg:
	mutex_unlock(&cfg80211_mutex);
=======
	r = set_regdom(rd);
	/* set_regdom took ownership */
	rd = NULL;

 bad_reg:
>>>>>>> refs/remotes/origin/master
	kfree(rd);
	return r;
}

static int validate_scan_freqs(struct nlattr *freqs)
{
	struct nlattr *attr1, *attr2;
	int n_channels = 0, tmp1, tmp2;

	nla_for_each_nested(attr1, freqs, tmp1) {
		n_channels++;
		/*
		 * Some hardware has a limited channel list for
		 * scanning, and it is pretty much nonsensical
		 * to scan for a channel twice, so disallow that
		 * and don't require drivers to check that the
		 * channel list they get isn't longer than what
		 * they can scan, as long as they can scan all
		 * the channels they registered at once.
		 */
		nla_for_each_nested(attr2, freqs, tmp2)
			if (attr1 != attr2 &&
			    nla_get_u32(attr1) == nla_get_u32(attr2))
				return 0;
	}

	return n_channels;
}

static int nl80211_trigger_scan(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
<<<<<<< HEAD
	struct net_device *dev = info->user_ptr[1];
=======
	struct wireless_dev *wdev = info->user_ptr[1];
>>>>>>> refs/remotes/origin/master
	struct cfg80211_scan_request *request;
	struct nlattr *attr;
	struct wiphy *wiphy;
	int err, tmp, n_ssids = 0, n_channels, i;
	size_t ie_len;

	if (!is_valid_ie_attr(info->attrs[NL80211_ATTR_IE]))
		return -EINVAL;

	wiphy = &rdev->wiphy;

	if (!rdev->ops->scan)
		return -EOPNOTSUPP;

<<<<<<< HEAD
	if (rdev->scan_req)
		return -EBUSY;
=======
	if (rdev->scan_req) {
		err = -EBUSY;
		goto unlock;
	}
>>>>>>> refs/remotes/origin/master

	if (info->attrs[NL80211_ATTR_SCAN_FREQUENCIES]) {
		n_channels = validate_scan_freqs(
				info->attrs[NL80211_ATTR_SCAN_FREQUENCIES]);
<<<<<<< HEAD
		if (!n_channels)
			return -EINVAL;
=======
		if (!n_channels) {
			err = -EINVAL;
			goto unlock;
		}
>>>>>>> refs/remotes/origin/master
	} else {
		enum ieee80211_band band;
		n_channels = 0;

		for (band = 0; band < IEEE80211_NUM_BANDS; band++)
			if (wiphy->bands[band])
				n_channels += wiphy->bands[band]->n_channels;
	}

	if (info->attrs[NL80211_ATTR_SCAN_SSIDS])
		nla_for_each_nested(attr, info->attrs[NL80211_ATTR_SCAN_SSIDS], tmp)
			n_ssids++;

<<<<<<< HEAD
	if (n_ssids > wiphy->max_scan_ssids)
		return -EINVAL;
=======
	if (n_ssids > wiphy->max_scan_ssids) {
		err = -EINVAL;
		goto unlock;
	}
>>>>>>> refs/remotes/origin/master

	if (info->attrs[NL80211_ATTR_IE])
		ie_len = nla_len(info->attrs[NL80211_ATTR_IE]);
	else
		ie_len = 0;

<<<<<<< HEAD
	if (ie_len > wiphy->max_scan_ie_len)
		return -EINVAL;
=======
	if (ie_len > wiphy->max_scan_ie_len) {
		err = -EINVAL;
		goto unlock;
	}
>>>>>>> refs/remotes/origin/master

	request = kzalloc(sizeof(*request)
			+ sizeof(*request->ssids) * n_ssids
			+ sizeof(*request->channels) * n_channels
			+ ie_len, GFP_KERNEL);
<<<<<<< HEAD
	if (!request)
		return -ENOMEM;
=======
	if (!request) {
		err = -ENOMEM;
		goto unlock;
	}
>>>>>>> refs/remotes/origin/master

	if (n_ssids)
		request->ssids = (void *)&request->channels[n_channels];
	request->n_ssids = n_ssids;
	if (ie_len) {
		if (request->ssids)
			request->ie = (void *)(request->ssids + n_ssids);
		else
			request->ie = (void *)(request->channels + n_channels);
	}

	i = 0;
	if (info->attrs[NL80211_ATTR_SCAN_FREQUENCIES]) {
		/* user specified, bail out if channel not found */
		nla_for_each_nested(attr, info->attrs[NL80211_ATTR_SCAN_FREQUENCIES], tmp) {
			struct ieee80211_channel *chan;

			chan = ieee80211_get_channel(wiphy, nla_get_u32(attr));

			if (!chan) {
				err = -EINVAL;
				goto out_free;
			}

			/* ignore disabled channels */
			if (chan->flags & IEEE80211_CHAN_DISABLED)
				continue;

			request->channels[i] = chan;
			i++;
		}
	} else {
		enum ieee80211_band band;

		/* all channels */
		for (band = 0; band < IEEE80211_NUM_BANDS; band++) {
			int j;
			if (!wiphy->bands[band])
				continue;
			for (j = 0; j < wiphy->bands[band]->n_channels; j++) {
				struct ieee80211_channel *chan;

				chan = &wiphy->bands[band]->channels[j];

				if (chan->flags & IEEE80211_CHAN_DISABLED)
					continue;

				request->channels[i] = chan;
				i++;
			}
		}
	}

	if (!i) {
		err = -EINVAL;
		goto out_free;
	}

	request->n_channels = i;

	i = 0;
	if (info->attrs[NL80211_ATTR_SCAN_SSIDS]) {
		nla_for_each_nested(attr, info->attrs[NL80211_ATTR_SCAN_SSIDS], tmp) {
			if (nla_len(attr) > IEEE80211_MAX_SSID_LEN) {
				err = -EINVAL;
				goto out_free;
			}
			request->ssids[i].ssid_len = nla_len(attr);
			memcpy(request->ssids[i].ssid, nla_data(attr), nla_len(attr));
			i++;
		}
	}

	if (info->attrs[NL80211_ATTR_IE]) {
		request->ie_len = nla_len(info->attrs[NL80211_ATTR_IE]);
		memcpy((void *)request->ie,
		       nla_data(info->attrs[NL80211_ATTR_IE]),
		       request->ie_len);
	}
<<<<<<< HEAD
	for (i = 0; i < IEEE80211_NUM_BANDS; i++)
		if (wiphy->bands[i])
			request->rates[i] =
				(1 << wiphy->bands[i]->n_bitrates) - 1;
	if (info->attrs[NL80211_ATTR_SCAN_SUPP_RATES]) {
		nla_for_each_nested(attr,
				    info->attrs[NL80211_ATTR_SCAN_SUPP_RATES],
				    tmp) {
			enum ieee80211_band band = nla_type(attr);
			if (band < 0 || band >= IEEE80211_NUM_BANDS) {
				err = -EINVAL;
				goto out_free;
			}
			err = ieee80211_get_ratemask(wiphy->bands[band],
						     nla_data(attr),
						     nla_len(attr),
						     &request->rates[band]);
			if (err)
				goto out_free;
		}
	}

<<<<<<< HEAD
=======
=======

>>>>>>> refs/remotes/origin/master
	for (i = 0; i < IEEE80211_NUM_BANDS; i++)
		if (wiphy->bands[i])
			request->rates[i] =
				(1 << wiphy->bands[i]->n_bitrates) - 1;

	if (info->attrs[NL80211_ATTR_SCAN_SUPP_RATES]) {
		nla_for_each_nested(attr,
				    info->attrs[NL80211_ATTR_SCAN_SUPP_RATES],
				    tmp) {
			enum ieee80211_band band = nla_type(attr);

			if (band < 0 || band >= IEEE80211_NUM_BANDS) {
				err = -EINVAL;
				goto out_free;
			}
<<<<<<< HEAD
=======

			if (!wiphy->bands[band])
				continue;

>>>>>>> refs/remotes/origin/master
			err = ieee80211_get_ratemask(wiphy->bands[band],
						     nla_data(attr),
						     nla_len(attr),
						     &request->rates[band]);
			if (err)
				goto out_free;
		}
	}

<<<<<<< HEAD
	request->no_cck =
		nla_get_flag(info->attrs[NL80211_ATTR_TX_NO_CCK_RATE]);

>>>>>>> refs/remotes/origin/cm-10.0
	request->dev = dev;
	request->wiphy = &rdev->wiphy;
	request->no_cck =
		nla_get_flag(info->attrs[NL80211_ATTR_TX_NO_CCK_RATE]);

	rdev->scan_req = request;
	err = rdev->ops->scan(&rdev->wiphy, dev, request);

	if (!err) {
		nl80211_send_scan_start(rdev, dev);
		dev_hold(dev);
=======
	if (info->attrs[NL80211_ATTR_SCAN_FLAGS]) {
		request->flags = nla_get_u32(
			info->attrs[NL80211_ATTR_SCAN_FLAGS]);
		if (((request->flags & NL80211_SCAN_FLAG_LOW_PRIORITY) &&
		     !(wiphy->features & NL80211_FEATURE_LOW_PRIORITY_SCAN)) ||
		    ((request->flags & NL80211_SCAN_FLAG_FLUSH) &&
		     !(wiphy->features & NL80211_FEATURE_SCAN_FLUSH))) {
			err = -EOPNOTSUPP;
			goto out_free;
		}
	}

	request->no_cck =
		nla_get_flag(info->attrs[NL80211_ATTR_TX_NO_CCK_RATE]);

	request->wdev = wdev;
	request->wiphy = &rdev->wiphy;
	request->scan_start = jiffies;

	rdev->scan_req = request;
	err = rdev_scan(rdev, request);

	if (!err) {
		nl80211_send_scan_start(rdev, wdev);
		if (wdev->netdev)
			dev_hold(wdev->netdev);
>>>>>>> refs/remotes/origin/master
	} else {
 out_free:
		rdev->scan_req = NULL;
		kfree(request);
	}

<<<<<<< HEAD
=======
 unlock:
>>>>>>> refs/remotes/origin/master
	return err;
}

static int nl80211_start_sched_scan(struct sk_buff *skb,
				    struct genl_info *info)
{
	struct cfg80211_sched_scan_request *request;
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];
	struct nlattr *attr;
	struct wiphy *wiphy;
	int err, tmp, n_ssids = 0, n_match_sets = 0, n_channels, i;
	u32 interval;
	enum ieee80211_band band;
	size_t ie_len;
	struct nlattr *tb[NL80211_SCHED_SCAN_MATCH_ATTR_MAX + 1];

	if (!(rdev->wiphy.flags & WIPHY_FLAG_SUPPORTS_SCHED_SCAN) ||
	    !rdev->ops->sched_scan_start)
		return -EOPNOTSUPP;

	if (!is_valid_ie_attr(info->attrs[NL80211_ATTR_IE]))
		return -EINVAL;

	if (!info->attrs[NL80211_ATTR_SCHED_SCAN_INTERVAL])
		return -EINVAL;

	interval = nla_get_u32(info->attrs[NL80211_ATTR_SCHED_SCAN_INTERVAL]);
	if (interval == 0)
		return -EINVAL;

	wiphy = &rdev->wiphy;

	if (info->attrs[NL80211_ATTR_SCAN_FREQUENCIES]) {
		n_channels = validate_scan_freqs(
				info->attrs[NL80211_ATTR_SCAN_FREQUENCIES]);
		if (!n_channels)
			return -EINVAL;
	} else {
		n_channels = 0;

		for (band = 0; band < IEEE80211_NUM_BANDS; band++)
			if (wiphy->bands[band])
				n_channels += wiphy->bands[band]->n_channels;
	}

	if (info->attrs[NL80211_ATTR_SCAN_SSIDS])
		nla_for_each_nested(attr, info->attrs[NL80211_ATTR_SCAN_SSIDS],
				    tmp)
			n_ssids++;

	if (n_ssids > wiphy->max_sched_scan_ssids)
		return -EINVAL;

	if (info->attrs[NL80211_ATTR_SCHED_SCAN_MATCH])
		nla_for_each_nested(attr,
				    info->attrs[NL80211_ATTR_SCHED_SCAN_MATCH],
				    tmp)
			n_match_sets++;

	if (n_match_sets > wiphy->max_match_sets)
		return -EINVAL;

	if (info->attrs[NL80211_ATTR_IE])
		ie_len = nla_len(info->attrs[NL80211_ATTR_IE]);
	else
		ie_len = 0;

	if (ie_len > wiphy->max_sched_scan_ie_len)
		return -EINVAL;

<<<<<<< HEAD
	mutex_lock(&rdev->sched_scan_mtx);

=======
>>>>>>> refs/remotes/origin/master
	if (rdev->sched_scan_req) {
		err = -EINPROGRESS;
		goto out;
	}

	request = kzalloc(sizeof(*request)
			+ sizeof(*request->ssids) * n_ssids
			+ sizeof(*request->match_sets) * n_match_sets
			+ sizeof(*request->channels) * n_channels
			+ ie_len, GFP_KERNEL);
	if (!request) {
		err = -ENOMEM;
		goto out;
	}

	if (n_ssids)
		request->ssids = (void *)&request->channels[n_channels];
	request->n_ssids = n_ssids;
	if (ie_len) {
		if (request->ssids)
			request->ie = (void *)(request->ssids + n_ssids);
		else
			request->ie = (void *)(request->channels + n_channels);
	}

	if (n_match_sets) {
		if (request->ie)
			request->match_sets = (void *)(request->ie + ie_len);
		else if (request->ssids)
			request->match_sets =
				(void *)(request->ssids + n_ssids);
		else
			request->match_sets =
				(void *)(request->channels + n_channels);
	}
	request->n_match_sets = n_match_sets;

	i = 0;
	if (info->attrs[NL80211_ATTR_SCAN_FREQUENCIES]) {
		/* user specified, bail out if channel not found */
		nla_for_each_nested(attr,
				    info->attrs[NL80211_ATTR_SCAN_FREQUENCIES],
				    tmp) {
			struct ieee80211_channel *chan;

			chan = ieee80211_get_channel(wiphy, nla_get_u32(attr));

			if (!chan) {
				err = -EINVAL;
				goto out_free;
			}

			/* ignore disabled channels */
			if (chan->flags & IEEE80211_CHAN_DISABLED)
				continue;

			request->channels[i] = chan;
			i++;
		}
	} else {
		/* all channels */
		for (band = 0; band < IEEE80211_NUM_BANDS; band++) {
			int j;
			if (!wiphy->bands[band])
				continue;
			for (j = 0; j < wiphy->bands[band]->n_channels; j++) {
				struct ieee80211_channel *chan;

				chan = &wiphy->bands[band]->channels[j];

				if (chan->flags & IEEE80211_CHAN_DISABLED)
					continue;

				request->channels[i] = chan;
				i++;
			}
		}
	}

	if (!i) {
		err = -EINVAL;
		goto out_free;
	}

	request->n_channels = i;

	i = 0;
	if (info->attrs[NL80211_ATTR_SCAN_SSIDS]) {
		nla_for_each_nested(attr, info->attrs[NL80211_ATTR_SCAN_SSIDS],
				    tmp) {
			if (nla_len(attr) > IEEE80211_MAX_SSID_LEN) {
				err = -EINVAL;
				goto out_free;
			}
			request->ssids[i].ssid_len = nla_len(attr);
			memcpy(request->ssids[i].ssid, nla_data(attr),
			       nla_len(attr));
			i++;
		}
	}

	i = 0;
	if (info->attrs[NL80211_ATTR_SCHED_SCAN_MATCH]) {
		nla_for_each_nested(attr,
				    info->attrs[NL80211_ATTR_SCHED_SCAN_MATCH],
				    tmp) {
<<<<<<< HEAD
			struct nlattr *ssid;
=======
			struct nlattr *ssid, *rssi;
>>>>>>> refs/remotes/origin/master

			nla_parse(tb, NL80211_SCHED_SCAN_MATCH_ATTR_MAX,
				  nla_data(attr), nla_len(attr),
				  nl80211_match_policy);
<<<<<<< HEAD
			ssid = tb[NL80211_ATTR_SCHED_SCAN_MATCH_SSID];
=======
			ssid = tb[NL80211_SCHED_SCAN_MATCH_ATTR_SSID];
>>>>>>> refs/remotes/origin/master
			if (ssid) {
				if (nla_len(ssid) > IEEE80211_MAX_SSID_LEN) {
					err = -EINVAL;
					goto out_free;
				}
				memcpy(request->match_sets[i].ssid.ssid,
				       nla_data(ssid), nla_len(ssid));
				request->match_sets[i].ssid.ssid_len =
					nla_len(ssid);
			}
<<<<<<< HEAD
=======
			rssi = tb[NL80211_SCHED_SCAN_MATCH_ATTR_RSSI];
			if (rssi)
				request->rssi_thold = nla_get_u32(rssi);
			else
				request->rssi_thold =
						   NL80211_SCAN_RSSI_THOLD_OFF;
>>>>>>> refs/remotes/origin/master
			i++;
		}
	}

	if (info->attrs[NL80211_ATTR_IE]) {
		request->ie_len = nla_len(info->attrs[NL80211_ATTR_IE]);
		memcpy((void *)request->ie,
		       nla_data(info->attrs[NL80211_ATTR_IE]),
		       request->ie_len);
	}

<<<<<<< HEAD
	request->dev = dev;
	request->wiphy = &rdev->wiphy;
	request->interval = interval;

	err = rdev->ops->sched_scan_start(&rdev->wiphy, dev, request);
=======
	if (info->attrs[NL80211_ATTR_SCAN_FLAGS]) {
		request->flags = nla_get_u32(
			info->attrs[NL80211_ATTR_SCAN_FLAGS]);
		if (((request->flags & NL80211_SCAN_FLAG_LOW_PRIORITY) &&
		     !(wiphy->features & NL80211_FEATURE_LOW_PRIORITY_SCAN)) ||
		    ((request->flags & NL80211_SCAN_FLAG_FLUSH) &&
		     !(wiphy->features & NL80211_FEATURE_SCAN_FLUSH))) {
			err = -EOPNOTSUPP;
			goto out_free;
		}
	}

	request->dev = dev;
	request->wiphy = &rdev->wiphy;
	request->interval = interval;
	request->scan_start = jiffies;

	err = rdev_sched_scan_start(rdev, dev, request);
>>>>>>> refs/remotes/origin/master
	if (!err) {
		rdev->sched_scan_req = request;
		nl80211_send_sched_scan(rdev, dev,
					NL80211_CMD_START_SCHED_SCAN);
		goto out;
	}

out_free:
	kfree(request);
out:
<<<<<<< HEAD
	mutex_unlock(&rdev->sched_scan_mtx);
=======
>>>>>>> refs/remotes/origin/master
	return err;
}

static int nl80211_stop_sched_scan(struct sk_buff *skb,
				   struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
<<<<<<< HEAD
	int err;
=======
>>>>>>> refs/remotes/origin/master

	if (!(rdev->wiphy.flags & WIPHY_FLAG_SUPPORTS_SCHED_SCAN) ||
	    !rdev->ops->sched_scan_stop)
		return -EOPNOTSUPP;

<<<<<<< HEAD
	mutex_lock(&rdev->sched_scan_mtx);
	err = __cfg80211_stop_sched_scan(rdev, false);
	mutex_unlock(&rdev->sched_scan_mtx);

	return err;
}

<<<<<<< HEAD
static int nl80211_send_bss(struct sk_buff *msg, u32 pid, u32 seq, int flags,
=======
static int nl80211_send_bss(struct sk_buff *msg, struct netlink_callback *cb,
			    u32 seq, int flags,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return __cfg80211_stop_sched_scan(rdev, false);
}

static int nl80211_start_radar_detection(struct sk_buff *skb,
					 struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct cfg80211_chan_def chandef;
	int err;

	err = nl80211_parse_chandef(rdev, info, &chandef);
	if (err)
		return err;

	if (netif_carrier_ok(dev))
		return -EBUSY;

	if (wdev->cac_started)
		return -EBUSY;

	err = cfg80211_chandef_dfs_required(wdev->wiphy, &chandef);
	if (err < 0)
		return err;

	if (err == 0)
		return -EINVAL;

	if (chandef.chan->dfs_state != NL80211_DFS_USABLE)
		return -EINVAL;

	if (!rdev->ops->start_radar_detection)
		return -EOPNOTSUPP;

	err = cfg80211_can_use_iftype_chan(rdev, wdev, wdev->iftype,
					   chandef.chan, CHAN_MODE_SHARED,
					   BIT(chandef.width));
	if (err)
		return err;

	err = rdev->ops->start_radar_detection(&rdev->wiphy, dev, &chandef);
	if (!err) {
		wdev->channel = chandef.chan;
		wdev->cac_started = true;
		wdev->cac_start_time = jiffies;
	}
	return err;
}

static int nl80211_channel_switch(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct cfg80211_csa_settings params;
	/* csa_attrs is defined static to avoid waste of stack size - this
	 * function is called under RTNL lock, so this should not be a problem.
	 */
	static struct nlattr *csa_attrs[NL80211_ATTR_MAX+1];
	u8 radar_detect_width = 0;
	int err;
	bool need_new_beacon = false;

	if (!rdev->ops->channel_switch ||
	    !(rdev->wiphy.flags & WIPHY_FLAG_HAS_CHANNEL_SWITCH))
		return -EOPNOTSUPP;

	switch (dev->ieee80211_ptr->iftype) {
	case NL80211_IFTYPE_AP:
	case NL80211_IFTYPE_P2P_GO:
		need_new_beacon = true;

		/* useless if AP is not running */
		if (!wdev->beacon_interval)
			return -EINVAL;
		break;
	case NL80211_IFTYPE_ADHOC:
	case NL80211_IFTYPE_MESH_POINT:
		break;
	default:
		return -EOPNOTSUPP;
	}

	memset(&params, 0, sizeof(params));

	if (!info->attrs[NL80211_ATTR_WIPHY_FREQ] ||
	    !info->attrs[NL80211_ATTR_CH_SWITCH_COUNT])
		return -EINVAL;

	/* only important for AP, IBSS and mesh create IEs internally */
	if (need_new_beacon && !info->attrs[NL80211_ATTR_CSA_IES])
		return -EINVAL;

	params.count = nla_get_u32(info->attrs[NL80211_ATTR_CH_SWITCH_COUNT]);

	if (!need_new_beacon)
		goto skip_beacons;

	err = nl80211_parse_beacon(info->attrs, &params.beacon_after);
	if (err)
		return err;

	err = nla_parse_nested(csa_attrs, NL80211_ATTR_MAX,
			       info->attrs[NL80211_ATTR_CSA_IES],
			       nl80211_policy);
	if (err)
		return err;

	err = nl80211_parse_beacon(csa_attrs, &params.beacon_csa);
	if (err)
		return err;

	if (!csa_attrs[NL80211_ATTR_CSA_C_OFF_BEACON])
		return -EINVAL;

	params.counter_offset_beacon =
		nla_get_u16(csa_attrs[NL80211_ATTR_CSA_C_OFF_BEACON]);
	if (params.counter_offset_beacon >= params.beacon_csa.tail_len)
		return -EINVAL;

	/* sanity check - counters should be the same */
	if (params.beacon_csa.tail[params.counter_offset_beacon] !=
	    params.count)
		return -EINVAL;

	if (csa_attrs[NL80211_ATTR_CSA_C_OFF_PRESP]) {
		params.counter_offset_presp =
			nla_get_u16(csa_attrs[NL80211_ATTR_CSA_C_OFF_PRESP]);
		if (params.counter_offset_presp >=
		    params.beacon_csa.probe_resp_len)
			return -EINVAL;

		if (params.beacon_csa.probe_resp[params.counter_offset_presp] !=
		    params.count)
			return -EINVAL;
	}

skip_beacons:
	err = nl80211_parse_chandef(rdev, info, &params.chandef);
	if (err)
		return err;

	if (!cfg80211_reg_can_beacon(&rdev->wiphy, &params.chandef))
		return -EINVAL;

	if (dev->ieee80211_ptr->iftype == NL80211_IFTYPE_AP ||
	    dev->ieee80211_ptr->iftype == NL80211_IFTYPE_P2P_GO ||
	    dev->ieee80211_ptr->iftype == NL80211_IFTYPE_ADHOC) {
		err = cfg80211_chandef_dfs_required(wdev->wiphy,
						    &params.chandef);
		if (err < 0) {
			return err;
		} else if (err) {
			radar_detect_width = BIT(params.chandef.width);
			params.radar_required = true;
		}
	}

	err = cfg80211_can_use_iftype_chan(rdev, wdev, wdev->iftype,
					   params.chandef.chan,
					   CHAN_MODE_SHARED,
					   radar_detect_width);
	if (err)
		return err;

	if (info->attrs[NL80211_ATTR_CH_SWITCH_BLOCK_TX])
		params.block_tx = true;

	return rdev_channel_switch(rdev, dev, &params);
}

static int nl80211_send_bss(struct sk_buff *msg, struct netlink_callback *cb,
			    u32 seq, int flags,
>>>>>>> refs/remotes/origin/master
			    struct cfg80211_registered_device *rdev,
			    struct wireless_dev *wdev,
			    struct cfg80211_internal_bss *intbss)
{
	struct cfg80211_bss *res = &intbss->pub;
<<<<<<< HEAD
	void *hdr;
	struct nlattr *bss;
<<<<<<< HEAD
	int i;

	ASSERT_WDEV_LOCK(wdev);

	hdr = nl80211hdr_put(msg, pid, seq, flags,
=======

	ASSERT_WDEV_LOCK(wdev);

	hdr = nl80211hdr_put(msg, NETLINK_CB(cb->skb).pid, seq, flags,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	const struct cfg80211_bss_ies *ies;
	void *hdr;
	struct nlattr *bss;
	bool tsf = false;

	ASSERT_WDEV_LOCK(wdev);

	hdr = nl80211hdr_put(msg, NETLINK_CB(cb->skb).portid, seq, flags,
>>>>>>> refs/remotes/origin/master
			     NL80211_CMD_NEW_SCAN_RESULTS);
	if (!hdr)
		return -1;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	genl_dump_check_consistent(cb, hdr, &nl80211_fam);

>>>>>>> refs/remotes/origin/cm-10.0
	NLA_PUT_U32(msg, NL80211_ATTR_GENERATION, rdev->bss_generation);
	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, wdev->netdev->ifindex);
=======
	genl_dump_check_consistent(cb, hdr, &nl80211_fam);

	if (nla_put_u32(msg, NL80211_ATTR_GENERATION, rdev->bss_generation))
		goto nla_put_failure;
	if (wdev->netdev &&
	    nla_put_u32(msg, NL80211_ATTR_IFINDEX, wdev->netdev->ifindex))
		goto nla_put_failure;
	if (nla_put_u64(msg, NL80211_ATTR_WDEV, wdev_id(wdev)))
		goto nla_put_failure;
>>>>>>> refs/remotes/origin/master

	bss = nla_nest_start(msg, NL80211_ATTR_BSS);
	if (!bss)
		goto nla_put_failure;
<<<<<<< HEAD
	if (!is_zero_ether_addr(res->bssid))
		NLA_PUT(msg, NL80211_BSS_BSSID, ETH_ALEN, res->bssid);
	if (res->information_elements && res->len_information_elements)
		NLA_PUT(msg, NL80211_BSS_INFORMATION_ELEMENTS,
			res->len_information_elements,
			res->information_elements);
	if (res->beacon_ies && res->len_beacon_ies &&
	    res->beacon_ies != res->information_elements)
		NLA_PUT(msg, NL80211_BSS_BEACON_IES,
			res->len_beacon_ies, res->beacon_ies);
	if (res->tsf)
		NLA_PUT_U64(msg, NL80211_BSS_TSF, res->tsf);
	if (res->beacon_interval)
		NLA_PUT_U16(msg, NL80211_BSS_BEACON_INTERVAL, res->beacon_interval);
	NLA_PUT_U16(msg, NL80211_BSS_CAPABILITY, res->capability);
	NLA_PUT_U32(msg, NL80211_BSS_FREQUENCY, res->channel->center_freq);
	NLA_PUT_U32(msg, NL80211_BSS_SEEN_MS_AGO,
		jiffies_to_msecs(jiffies - intbss->ts));

	switch (rdev->wiphy.signal_type) {
	case CFG80211_SIGNAL_TYPE_MBM:
		NLA_PUT_U32(msg, NL80211_BSS_SIGNAL_MBM, res->signal);
		break;
	case CFG80211_SIGNAL_TYPE_UNSPEC:
		NLA_PUT_U8(msg, NL80211_BSS_SIGNAL_UNSPEC, res->signal);
=======
	if ((!is_zero_ether_addr(res->bssid) &&
	     nla_put(msg, NL80211_BSS_BSSID, ETH_ALEN, res->bssid)))
		goto nla_put_failure;

	rcu_read_lock();
	ies = rcu_dereference(res->ies);
	if (ies) {
		if (nla_put_u64(msg, NL80211_BSS_TSF, ies->tsf))
			goto fail_unlock_rcu;
		tsf = true;
		if (ies->len && nla_put(msg, NL80211_BSS_INFORMATION_ELEMENTS,
					ies->len, ies->data))
			goto fail_unlock_rcu;
	}
	ies = rcu_dereference(res->beacon_ies);
	if (ies) {
		if (!tsf && nla_put_u64(msg, NL80211_BSS_TSF, ies->tsf))
			goto fail_unlock_rcu;
		if (ies->len && nla_put(msg, NL80211_BSS_BEACON_IES,
					ies->len, ies->data))
			goto fail_unlock_rcu;
	}
	rcu_read_unlock();

	if (res->beacon_interval &&
	    nla_put_u16(msg, NL80211_BSS_BEACON_INTERVAL, res->beacon_interval))
		goto nla_put_failure;
	if (nla_put_u16(msg, NL80211_BSS_CAPABILITY, res->capability) ||
	    nla_put_u32(msg, NL80211_BSS_FREQUENCY, res->channel->center_freq) ||
	    nla_put_u32(msg, NL80211_BSS_CHAN_WIDTH, res->scan_width) ||
	    nla_put_u32(msg, NL80211_BSS_SEEN_MS_AGO,
			jiffies_to_msecs(jiffies - intbss->ts)))
		goto nla_put_failure;

	switch (rdev->wiphy.signal_type) {
	case CFG80211_SIGNAL_TYPE_MBM:
		if (nla_put_u32(msg, NL80211_BSS_SIGNAL_MBM, res->signal))
			goto nla_put_failure;
		break;
	case CFG80211_SIGNAL_TYPE_UNSPEC:
		if (nla_put_u8(msg, NL80211_BSS_SIGNAL_UNSPEC, res->signal))
			goto nla_put_failure;
>>>>>>> refs/remotes/origin/master
		break;
	default:
		break;
	}

	switch (wdev->iftype) {
	case NL80211_IFTYPE_P2P_CLIENT:
	case NL80211_IFTYPE_STATION:
<<<<<<< HEAD
		if (intbss == wdev->current_bss)
			NLA_PUT_U32(msg, NL80211_BSS_STATUS,
				    NL80211_BSS_STATUS_ASSOCIATED);
<<<<<<< HEAD
		else for (i = 0; i < MAX_AUTH_BSSES; i++) {
			if (intbss != wdev->auth_bsses[i])
				continue;
			NLA_PUT_U32(msg, NL80211_BSS_STATUS,
				    NL80211_BSS_STATUS_AUTHENTICATED);
			break;
		}
=======
>>>>>>> refs/remotes/origin/cm-10.0
		break;
	case NL80211_IFTYPE_ADHOC:
		if (intbss == wdev->current_bss)
			NLA_PUT_U32(msg, NL80211_BSS_STATUS,
				    NL80211_BSS_STATUS_IBSS_JOINED);
=======
		if (intbss == wdev->current_bss &&
		    nla_put_u32(msg, NL80211_BSS_STATUS,
				NL80211_BSS_STATUS_ASSOCIATED))
			goto nla_put_failure;
		break;
	case NL80211_IFTYPE_ADHOC:
		if (intbss == wdev->current_bss &&
		    nla_put_u32(msg, NL80211_BSS_STATUS,
				NL80211_BSS_STATUS_IBSS_JOINED))
			goto nla_put_failure;
>>>>>>> refs/remotes/origin/master
		break;
	default:
		break;
	}

	nla_nest_end(msg, bss);

	return genlmsg_end(msg, hdr);

<<<<<<< HEAD
=======
 fail_unlock_rcu:
	rcu_read_unlock();
>>>>>>> refs/remotes/origin/master
 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	return -EMSGSIZE;
}

<<<<<<< HEAD
static int nl80211_dump_scan(struct sk_buff *skb,
			     struct netlink_callback *cb)
{
	struct cfg80211_registered_device *rdev;
	struct net_device *dev;
	struct cfg80211_internal_bss *scan;
	struct wireless_dev *wdev;
	int start = cb->args[1], idx = 0;
	int err;

	err = nl80211_prepare_netdev_dump(skb, cb, &rdev, &dev);
	if (err)
		return err;

	wdev = dev->ieee80211_ptr;

=======
static int nl80211_dump_scan(struct sk_buff *skb, struct netlink_callback *cb)
{
	struct cfg80211_registered_device *rdev;
	struct cfg80211_internal_bss *scan;
	struct wireless_dev *wdev;
	int start = cb->args[2], idx = 0;
	int err;

	err = nl80211_prepare_wdev_dump(skb, cb, &rdev, &wdev);
	if (err)
		return err;

>>>>>>> refs/remotes/origin/master
	wdev_lock(wdev);
	spin_lock_bh(&rdev->bss_lock);
	cfg80211_bss_expire(rdev);

<<<<<<< HEAD
<<<<<<< HEAD
	list_for_each_entry(scan, &rdev->bss_list, list) {
		if (++idx <= start)
			continue;
		if (nl80211_send_bss(skb,
				NETLINK_CB(cb->skb).pid,
=======
=======
>>>>>>> refs/remotes/origin/master
	cb->seq = rdev->bss_generation;

	list_for_each_entry(scan, &rdev->bss_list, list) {
		if (++idx <= start)
			continue;
		if (nl80211_send_bss(skb, cb,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				cb->nlh->nlmsg_seq, NLM_F_MULTI,
				rdev, wdev, scan) < 0) {
			idx--;
			break;
		}
	}

	spin_unlock_bh(&rdev->bss_lock);
	wdev_unlock(wdev);

<<<<<<< HEAD
	cb->args[1] = idx;
	nl80211_finish_netdev_dump(rdev);
=======
	cb->args[2] = idx;
	nl80211_finish_wdev_dump(rdev);
>>>>>>> refs/remotes/origin/master

	return skb->len;
}

<<<<<<< HEAD
static int nl80211_send_survey(struct sk_buff *msg, u32 pid, u32 seq,
=======
static int nl80211_send_survey(struct sk_buff *msg, u32 portid, u32 seq,
>>>>>>> refs/remotes/origin/master
				int flags, struct net_device *dev,
				struct survey_info *survey)
{
	void *hdr;
	struct nlattr *infoattr;

<<<<<<< HEAD
<<<<<<< HEAD
	/* Survey without a channel doesn't make sense */
	if (!survey->channel)
		return -EINVAL;

=======
>>>>>>> refs/remotes/origin/cm-10.0
	hdr = nl80211hdr_put(msg, pid, seq, flags,
=======
	hdr = nl80211hdr_put(msg, portid, seq, flags,
>>>>>>> refs/remotes/origin/master
			     NL80211_CMD_NEW_SURVEY_RESULTS);
	if (!hdr)
		return -ENOMEM;

<<<<<<< HEAD
	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, dev->ifindex);
=======
	if (nla_put_u32(msg, NL80211_ATTR_IFINDEX, dev->ifindex))
		goto nla_put_failure;
>>>>>>> refs/remotes/origin/master

	infoattr = nla_nest_start(msg, NL80211_ATTR_SURVEY_INFO);
	if (!infoattr)
		goto nla_put_failure;

<<<<<<< HEAD
	NLA_PUT_U32(msg, NL80211_SURVEY_INFO_FREQUENCY,
		    survey->channel->center_freq);
	if (survey->filled & SURVEY_INFO_NOISE_DBM)
		NLA_PUT_U8(msg, NL80211_SURVEY_INFO_NOISE,
			    survey->noise);
	if (survey->filled & SURVEY_INFO_IN_USE)
		NLA_PUT_FLAG(msg, NL80211_SURVEY_INFO_IN_USE);
	if (survey->filled & SURVEY_INFO_CHANNEL_TIME)
		NLA_PUT_U64(msg, NL80211_SURVEY_INFO_CHANNEL_TIME,
			    survey->channel_time);
	if (survey->filled & SURVEY_INFO_CHANNEL_TIME_BUSY)
		NLA_PUT_U64(msg, NL80211_SURVEY_INFO_CHANNEL_TIME_BUSY,
			    survey->channel_time_busy);
	if (survey->filled & SURVEY_INFO_CHANNEL_TIME_EXT_BUSY)
		NLA_PUT_U64(msg, NL80211_SURVEY_INFO_CHANNEL_TIME_EXT_BUSY,
			    survey->channel_time_ext_busy);
	if (survey->filled & SURVEY_INFO_CHANNEL_TIME_RX)
		NLA_PUT_U64(msg, NL80211_SURVEY_INFO_CHANNEL_TIME_RX,
			    survey->channel_time_rx);
	if (survey->filled & SURVEY_INFO_CHANNEL_TIME_TX)
		NLA_PUT_U64(msg, NL80211_SURVEY_INFO_CHANNEL_TIME_TX,
			    survey->channel_time_tx);
=======
	if (nla_put_u32(msg, NL80211_SURVEY_INFO_FREQUENCY,
			survey->channel->center_freq))
		goto nla_put_failure;

	if ((survey->filled & SURVEY_INFO_NOISE_DBM) &&
	    nla_put_u8(msg, NL80211_SURVEY_INFO_NOISE, survey->noise))
		goto nla_put_failure;
	if ((survey->filled & SURVEY_INFO_IN_USE) &&
	    nla_put_flag(msg, NL80211_SURVEY_INFO_IN_USE))
		goto nla_put_failure;
	if ((survey->filled & SURVEY_INFO_CHANNEL_TIME) &&
	    nla_put_u64(msg, NL80211_SURVEY_INFO_CHANNEL_TIME,
			survey->channel_time))
		goto nla_put_failure;
	if ((survey->filled & SURVEY_INFO_CHANNEL_TIME_BUSY) &&
	    nla_put_u64(msg, NL80211_SURVEY_INFO_CHANNEL_TIME_BUSY,
			survey->channel_time_busy))
		goto nla_put_failure;
	if ((survey->filled & SURVEY_INFO_CHANNEL_TIME_EXT_BUSY) &&
	    nla_put_u64(msg, NL80211_SURVEY_INFO_CHANNEL_TIME_EXT_BUSY,
			survey->channel_time_ext_busy))
		goto nla_put_failure;
	if ((survey->filled & SURVEY_INFO_CHANNEL_TIME_RX) &&
	    nla_put_u64(msg, NL80211_SURVEY_INFO_CHANNEL_TIME_RX,
			survey->channel_time_rx))
		goto nla_put_failure;
	if ((survey->filled & SURVEY_INFO_CHANNEL_TIME_TX) &&
	    nla_put_u64(msg, NL80211_SURVEY_INFO_CHANNEL_TIME_TX,
			survey->channel_time_tx))
		goto nla_put_failure;
>>>>>>> refs/remotes/origin/master

	nla_nest_end(msg, infoattr);

	return genlmsg_end(msg, hdr);

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	return -EMSGSIZE;
}

static int nl80211_dump_survey(struct sk_buff *skb,
			struct netlink_callback *cb)
{
	struct survey_info survey;
	struct cfg80211_registered_device *dev;
<<<<<<< HEAD
	struct net_device *netdev;
	int survey_idx = cb->args[1];
	int res;

	res = nl80211_prepare_netdev_dump(skb, cb, &dev, &netdev);
	if (res)
		return res;

=======
	struct wireless_dev *wdev;
	int survey_idx = cb->args[2];
	int res;

	res = nl80211_prepare_wdev_dump(skb, cb, &dev, &wdev);
	if (res)
		return res;

	if (!wdev->netdev) {
		res = -EINVAL;
		goto out_err;
	}

>>>>>>> refs/remotes/origin/master
	if (!dev->ops->dump_survey) {
		res = -EOPNOTSUPP;
		goto out_err;
	}

	while (1) {
<<<<<<< HEAD
<<<<<<< HEAD
=======
		struct ieee80211_channel *chan;

>>>>>>> refs/remotes/origin/cm-10.0
		res = dev->ops->dump_survey(&dev->wiphy, netdev, survey_idx,
					    &survey);
=======
		struct ieee80211_channel *chan;

		res = rdev_dump_survey(dev, wdev->netdev, survey_idx, &survey);
>>>>>>> refs/remotes/origin/master
		if (res == -ENOENT)
			break;
		if (res)
			goto out_err;

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
		/* Survey without a channel doesn't make sense */
		if (!survey.channel) {
			res = -EINVAL;
			goto out;
		}

		chan = ieee80211_get_channel(&dev->wiphy,
					     survey.channel->center_freq);
		if (!chan || chan->flags & IEEE80211_CHAN_DISABLED) {
			survey_idx++;
			continue;
		}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		if (nl80211_send_survey(skb,
				NETLINK_CB(cb->skb).pid,
				cb->nlh->nlmsg_seq, NLM_F_MULTI,
				netdev,
				&survey) < 0)
=======
		if (nl80211_send_survey(skb,
				NETLINK_CB(cb->skb).portid,
				cb->nlh->nlmsg_seq, NLM_F_MULTI,
				wdev->netdev, &survey) < 0)
>>>>>>> refs/remotes/origin/master
			goto out;
		survey_idx++;
	}

 out:
<<<<<<< HEAD
	cb->args[1] = survey_idx;
	res = skb->len;
 out_err:
	nl80211_finish_netdev_dump(dev);
	return res;
}

static bool nl80211_valid_auth_type(enum nl80211_auth_type auth_type)
{
	return auth_type <= NL80211_AUTHTYPE_MAX;
}

=======
	cb->args[2] = survey_idx;
	res = skb->len;
 out_err:
	nl80211_finish_wdev_dump(dev);
	return res;
}

>>>>>>> refs/remotes/origin/master
static bool nl80211_valid_wpa_versions(u32 wpa_versions)
{
	return !(wpa_versions & ~(NL80211_WPA_VERSION_1 |
				  NL80211_WPA_VERSION_2));
}

static int nl80211_authenticate(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];
	struct ieee80211_channel *chan;
<<<<<<< HEAD
	const u8 *bssid, *ssid, *ie = NULL;
	int err, ssid_len, ie_len = 0;
=======
	const u8 *bssid, *ssid, *ie = NULL, *sae_data = NULL;
	int err, ssid_len, ie_len = 0, sae_data_len = 0;
>>>>>>> refs/remotes/origin/master
	enum nl80211_auth_type auth_type;
	struct key_parse key;
	bool local_state_change;

	if (!is_valid_ie_attr(info->attrs[NL80211_ATTR_IE]))
		return -EINVAL;

	if (!info->attrs[NL80211_ATTR_MAC])
		return -EINVAL;

	if (!info->attrs[NL80211_ATTR_AUTH_TYPE])
		return -EINVAL;

	if (!info->attrs[NL80211_ATTR_SSID])
		return -EINVAL;

	if (!info->attrs[NL80211_ATTR_WIPHY_FREQ])
		return -EINVAL;

	err = nl80211_parse_key(info, &key);
	if (err)
		return err;

	if (key.idx >= 0) {
		if (key.type != -1 && key.type != NL80211_KEYTYPE_GROUP)
			return -EINVAL;
		if (!key.p.key || !key.p.key_len)
			return -EINVAL;
		if ((key.p.cipher != WLAN_CIPHER_SUITE_WEP40 ||
		     key.p.key_len != WLAN_KEY_LEN_WEP40) &&
		    (key.p.cipher != WLAN_CIPHER_SUITE_WEP104 ||
		     key.p.key_len != WLAN_KEY_LEN_WEP104))
			return -EINVAL;
		if (key.idx > 4)
			return -EINVAL;
	} else {
		key.p.key_len = 0;
		key.p.key = NULL;
	}

	if (key.idx >= 0) {
		int i;
		bool ok = false;
		for (i = 0; i < rdev->wiphy.n_cipher_suites; i++) {
			if (key.p.cipher == rdev->wiphy.cipher_suites[i]) {
				ok = true;
				break;
			}
		}
		if (!ok)
			return -EINVAL;
	}

	if (!rdev->ops->auth)
		return -EOPNOTSUPP;

	if (dev->ieee80211_ptr->iftype != NL80211_IFTYPE_STATION &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_P2P_CLIENT)
		return -EOPNOTSUPP;

	bssid = nla_data(info->attrs[NL80211_ATTR_MAC]);
	chan = ieee80211_get_channel(&rdev->wiphy,
		nla_get_u32(info->attrs[NL80211_ATTR_WIPHY_FREQ]));
	if (!chan || (chan->flags & IEEE80211_CHAN_DISABLED))
		return -EINVAL;

	ssid = nla_data(info->attrs[NL80211_ATTR_SSID]);
	ssid_len = nla_len(info->attrs[NL80211_ATTR_SSID]);

	if (info->attrs[NL80211_ATTR_IE]) {
		ie = nla_data(info->attrs[NL80211_ATTR_IE]);
		ie_len = nla_len(info->attrs[NL80211_ATTR_IE]);
	}

	auth_type = nla_get_u32(info->attrs[NL80211_ATTR_AUTH_TYPE]);
<<<<<<< HEAD
	if (!nl80211_valid_auth_type(auth_type))
		return -EINVAL;

	local_state_change = !!info->attrs[NL80211_ATTR_LOCAL_STATE_CHANGE];

<<<<<<< HEAD
	return cfg80211_mlme_auth(rdev, dev, chan, auth_type, bssid,
				  ssid, ssid_len, ie, ie_len,
				  key.p.key, key.p.key_len, key.idx,
				  local_state_change);
=======
=======
	if (!nl80211_valid_auth_type(rdev, auth_type, NL80211_CMD_AUTHENTICATE))
		return -EINVAL;

	if (auth_type == NL80211_AUTHTYPE_SAE &&
	    !info->attrs[NL80211_ATTR_SAE_DATA])
		return -EINVAL;

	if (info->attrs[NL80211_ATTR_SAE_DATA]) {
		if (auth_type != NL80211_AUTHTYPE_SAE)
			return -EINVAL;
		sae_data = nla_data(info->attrs[NL80211_ATTR_SAE_DATA]);
		sae_data_len = nla_len(info->attrs[NL80211_ATTR_SAE_DATA]);
		/* need to include at least Auth Transaction and Status Code */
		if (sae_data_len < 4)
			return -EINVAL;
	}

	local_state_change = !!info->attrs[NL80211_ATTR_LOCAL_STATE_CHANGE];

>>>>>>> refs/remotes/origin/master
	/*
	 * Since we no longer track auth state, ignore
	 * requests to only change local state.
	 */
	if (local_state_change)
		return 0;

<<<<<<< HEAD
	return cfg80211_mlme_auth(rdev, dev, chan, auth_type, bssid,
				  ssid, ssid_len, ie, ie_len,
				  key.p.key, key.p.key_len, key.idx);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	wdev_lock(dev->ieee80211_ptr);
	err = cfg80211_mlme_auth(rdev, dev, chan, auth_type, bssid,
				 ssid, ssid_len, ie, ie_len,
				 key.p.key, key.p.key_len, key.idx,
				 sae_data, sae_data_len);
	wdev_unlock(dev->ieee80211_ptr);
	return err;
>>>>>>> refs/remotes/origin/master
}

static int nl80211_crypto_settings(struct cfg80211_registered_device *rdev,
				   struct genl_info *info,
				   struct cfg80211_crypto_settings *settings,
				   int cipher_limit)
{
	memset(settings, 0, sizeof(*settings));

	settings->control_port = info->attrs[NL80211_ATTR_CONTROL_PORT];

	if (info->attrs[NL80211_ATTR_CONTROL_PORT_ETHERTYPE]) {
		u16 proto;
		proto = nla_get_u16(
			info->attrs[NL80211_ATTR_CONTROL_PORT_ETHERTYPE]);
		settings->control_port_ethertype = cpu_to_be16(proto);
		if (!(rdev->wiphy.flags & WIPHY_FLAG_CONTROL_PORT_PROTOCOL) &&
		    proto != ETH_P_PAE)
			return -EINVAL;
		if (info->attrs[NL80211_ATTR_CONTROL_PORT_NO_ENCRYPT])
			settings->control_port_no_encrypt = true;
	} else
		settings->control_port_ethertype = cpu_to_be16(ETH_P_PAE);

	if (info->attrs[NL80211_ATTR_CIPHER_SUITES_PAIRWISE]) {
		void *data;
		int len, i;

		data = nla_data(info->attrs[NL80211_ATTR_CIPHER_SUITES_PAIRWISE]);
		len = nla_len(info->attrs[NL80211_ATTR_CIPHER_SUITES_PAIRWISE]);
		settings->n_ciphers_pairwise = len / sizeof(u32);

		if (len % sizeof(u32))
			return -EINVAL;

		if (settings->n_ciphers_pairwise > cipher_limit)
			return -EINVAL;

		memcpy(settings->ciphers_pairwise, data, len);

		for (i = 0; i < settings->n_ciphers_pairwise; i++)
			if (!cfg80211_supported_cipher_suite(
<<<<<<< HEAD
<<<<<<< HEAD
				&rdev->wiphy,
				settings->ciphers_pairwise[i]))
=======
					&rdev->wiphy,
					settings->ciphers_pairwise[i]))
>>>>>>> refs/remotes/origin/cm-10.0
=======
					&rdev->wiphy,
					settings->ciphers_pairwise[i]))
>>>>>>> refs/remotes/origin/master
				return -EINVAL;
	}

	if (info->attrs[NL80211_ATTR_CIPHER_SUITE_GROUP]) {
		settings->cipher_group =
			nla_get_u32(info->attrs[NL80211_ATTR_CIPHER_SUITE_GROUP]);
		if (!cfg80211_supported_cipher_suite(&rdev->wiphy,
<<<<<<< HEAD
<<<<<<< HEAD
					settings->cipher_group))
=======
						     settings->cipher_group))
>>>>>>> refs/remotes/origin/cm-10.0
=======
						     settings->cipher_group))
>>>>>>> refs/remotes/origin/master
			return -EINVAL;
	}

	if (info->attrs[NL80211_ATTR_WPA_VERSIONS]) {
		settings->wpa_versions =
			nla_get_u32(info->attrs[NL80211_ATTR_WPA_VERSIONS]);
		if (!nl80211_valid_wpa_versions(settings->wpa_versions))
			return -EINVAL;
	}

	if (info->attrs[NL80211_ATTR_AKM_SUITES]) {
		void *data;
		int len;

		data = nla_data(info->attrs[NL80211_ATTR_AKM_SUITES]);
		len = nla_len(info->attrs[NL80211_ATTR_AKM_SUITES]);
		settings->n_akm_suites = len / sizeof(u32);

		if (len % sizeof(u32))
			return -EINVAL;

		if (settings->n_akm_suites > NL80211_MAX_NR_AKM_SUITES)
			return -EINVAL;

		memcpy(settings->akm_suites, data, len);
	}

	return 0;
}

static int nl80211_associate(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];
<<<<<<< HEAD
	struct cfg80211_crypto_settings crypto;
	struct ieee80211_channel *chan;
	const u8 *bssid, *ssid, *ie = NULL, *prev_bssid = NULL;
	int err, ssid_len, ie_len = 0;
	bool use_mfp = false;
<<<<<<< HEAD
=======
	u32 flags = 0;
	struct ieee80211_ht_cap *ht_capa = NULL;
	struct ieee80211_ht_cap *ht_capa_mask = NULL;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct ieee80211_channel *chan;
	struct cfg80211_assoc_request req = {};
	const u8 *bssid, *ssid;
	int err, ssid_len = 0;
>>>>>>> refs/remotes/origin/master

	if (!is_valid_ie_attr(info->attrs[NL80211_ATTR_IE]))
		return -EINVAL;

	if (!info->attrs[NL80211_ATTR_MAC] ||
	    !info->attrs[NL80211_ATTR_SSID] ||
	    !info->attrs[NL80211_ATTR_WIPHY_FREQ])
		return -EINVAL;

	if (!rdev->ops->assoc)
		return -EOPNOTSUPP;

	if (dev->ieee80211_ptr->iftype != NL80211_IFTYPE_STATION &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_P2P_CLIENT)
		return -EOPNOTSUPP;

	bssid = nla_data(info->attrs[NL80211_ATTR_MAC]);

	chan = ieee80211_get_channel(&rdev->wiphy,
		nla_get_u32(info->attrs[NL80211_ATTR_WIPHY_FREQ]));
	if (!chan || (chan->flags & IEEE80211_CHAN_DISABLED))
		return -EINVAL;

	ssid = nla_data(info->attrs[NL80211_ATTR_SSID]);
	ssid_len = nla_len(info->attrs[NL80211_ATTR_SSID]);

	if (info->attrs[NL80211_ATTR_IE]) {
<<<<<<< HEAD
		ie = nla_data(info->attrs[NL80211_ATTR_IE]);
		ie_len = nla_len(info->attrs[NL80211_ATTR_IE]);
=======
		req.ie = nla_data(info->attrs[NL80211_ATTR_IE]);
		req.ie_len = nla_len(info->attrs[NL80211_ATTR_IE]);
>>>>>>> refs/remotes/origin/master
	}

	if (info->attrs[NL80211_ATTR_USE_MFP]) {
		enum nl80211_mfp mfp =
			nla_get_u32(info->attrs[NL80211_ATTR_USE_MFP]);
		if (mfp == NL80211_MFP_REQUIRED)
<<<<<<< HEAD
			use_mfp = true;
=======
			req.use_mfp = true;
>>>>>>> refs/remotes/origin/master
		else if (mfp != NL80211_MFP_NO)
			return -EINVAL;
	}

	if (info->attrs[NL80211_ATTR_PREV_BSSID])
<<<<<<< HEAD
		prev_bssid = nla_data(info->attrs[NL80211_ATTR_PREV_BSSID]);

<<<<<<< HEAD
=======
	if (nla_get_flag(info->attrs[NL80211_ATTR_DISABLE_HT]))
		flags |= ASSOC_REQ_DISABLE_HT;

	if (info->attrs[NL80211_ATTR_HT_CAPABILITY_MASK])
		ht_capa_mask =
			nla_data(info->attrs[NL80211_ATTR_HT_CAPABILITY_MASK]);

	if (info->attrs[NL80211_ATTR_HT_CAPABILITY]) {
		if (!ht_capa_mask)
			return -EINVAL;
		ht_capa = nla_data(info->attrs[NL80211_ATTR_HT_CAPABILITY]);
	}

>>>>>>> refs/remotes/origin/cm-10.0
	err = nl80211_crypto_settings(rdev, info, &crypto, 1);
	if (!err)
		err = cfg80211_mlme_assoc(rdev, dev, chan, bssid, prev_bssid,
					  ssid, ssid_len, ie, ie_len, use_mfp,
<<<<<<< HEAD
					  &crypto);
=======
					  &crypto, flags, ht_capa,
					  ht_capa_mask);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		req.prev_bssid = nla_data(info->attrs[NL80211_ATTR_PREV_BSSID]);

	if (nla_get_flag(info->attrs[NL80211_ATTR_DISABLE_HT]))
		req.flags |= ASSOC_REQ_DISABLE_HT;

	if (info->attrs[NL80211_ATTR_HT_CAPABILITY_MASK])
		memcpy(&req.ht_capa_mask,
		       nla_data(info->attrs[NL80211_ATTR_HT_CAPABILITY_MASK]),
		       sizeof(req.ht_capa_mask));

	if (info->attrs[NL80211_ATTR_HT_CAPABILITY]) {
		if (!info->attrs[NL80211_ATTR_HT_CAPABILITY_MASK])
			return -EINVAL;
		memcpy(&req.ht_capa,
		       nla_data(info->attrs[NL80211_ATTR_HT_CAPABILITY]),
		       sizeof(req.ht_capa));
	}

	if (nla_get_flag(info->attrs[NL80211_ATTR_DISABLE_VHT]))
		req.flags |= ASSOC_REQ_DISABLE_VHT;

	if (info->attrs[NL80211_ATTR_VHT_CAPABILITY_MASK])
		memcpy(&req.vht_capa_mask,
		       nla_data(info->attrs[NL80211_ATTR_VHT_CAPABILITY_MASK]),
		       sizeof(req.vht_capa_mask));

	if (info->attrs[NL80211_ATTR_VHT_CAPABILITY]) {
		if (!info->attrs[NL80211_ATTR_VHT_CAPABILITY_MASK])
			return -EINVAL;
		memcpy(&req.vht_capa,
		       nla_data(info->attrs[NL80211_ATTR_VHT_CAPABILITY]),
		       sizeof(req.vht_capa));
	}

	err = nl80211_crypto_settings(rdev, info, &req.crypto, 1);
	if (!err) {
		wdev_lock(dev->ieee80211_ptr);
		err = cfg80211_mlme_assoc(rdev, dev, chan, bssid,
					  ssid, ssid_len, &req);
		wdev_unlock(dev->ieee80211_ptr);
	}
>>>>>>> refs/remotes/origin/master

	return err;
}

static int nl80211_deauthenticate(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];
	const u8 *ie = NULL, *bssid;
<<<<<<< HEAD
	int ie_len = 0;
=======
	int ie_len = 0, err;
>>>>>>> refs/remotes/origin/master
	u16 reason_code;
	bool local_state_change;

	if (!is_valid_ie_attr(info->attrs[NL80211_ATTR_IE]))
		return -EINVAL;

	if (!info->attrs[NL80211_ATTR_MAC])
		return -EINVAL;

	if (!info->attrs[NL80211_ATTR_REASON_CODE])
		return -EINVAL;

	if (!rdev->ops->deauth)
		return -EOPNOTSUPP;

	if (dev->ieee80211_ptr->iftype != NL80211_IFTYPE_STATION &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_P2P_CLIENT)
		return -EOPNOTSUPP;

	bssid = nla_data(info->attrs[NL80211_ATTR_MAC]);

	reason_code = nla_get_u16(info->attrs[NL80211_ATTR_REASON_CODE]);
	if (reason_code == 0) {
		/* Reason Code 0 is reserved */
		return -EINVAL;
	}

	if (info->attrs[NL80211_ATTR_IE]) {
		ie = nla_data(info->attrs[NL80211_ATTR_IE]);
		ie_len = nla_len(info->attrs[NL80211_ATTR_IE]);
	}

	local_state_change = !!info->attrs[NL80211_ATTR_LOCAL_STATE_CHANGE];

<<<<<<< HEAD
	return cfg80211_mlme_deauth(rdev, dev, bssid, ie, ie_len, reason_code,
				    local_state_change);
=======
	wdev_lock(dev->ieee80211_ptr);
	err = cfg80211_mlme_deauth(rdev, dev, bssid, ie, ie_len, reason_code,
				   local_state_change);
	wdev_unlock(dev->ieee80211_ptr);
	return err;
>>>>>>> refs/remotes/origin/master
}

static int nl80211_disassociate(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];
	const u8 *ie = NULL, *bssid;
<<<<<<< HEAD
	int ie_len = 0;
=======
	int ie_len = 0, err;
>>>>>>> refs/remotes/origin/master
	u16 reason_code;
	bool local_state_change;

	if (!is_valid_ie_attr(info->attrs[NL80211_ATTR_IE]))
		return -EINVAL;

	if (!info->attrs[NL80211_ATTR_MAC])
		return -EINVAL;

	if (!info->attrs[NL80211_ATTR_REASON_CODE])
		return -EINVAL;

	if (!rdev->ops->disassoc)
		return -EOPNOTSUPP;

	if (dev->ieee80211_ptr->iftype != NL80211_IFTYPE_STATION &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_P2P_CLIENT)
		return -EOPNOTSUPP;

	bssid = nla_data(info->attrs[NL80211_ATTR_MAC]);

	reason_code = nla_get_u16(info->attrs[NL80211_ATTR_REASON_CODE]);
	if (reason_code == 0) {
		/* Reason Code 0 is reserved */
		return -EINVAL;
	}

	if (info->attrs[NL80211_ATTR_IE]) {
		ie = nla_data(info->attrs[NL80211_ATTR_IE]);
		ie_len = nla_len(info->attrs[NL80211_ATTR_IE]);
	}

	local_state_change = !!info->attrs[NL80211_ATTR_LOCAL_STATE_CHANGE];

<<<<<<< HEAD
	return cfg80211_mlme_disassoc(rdev, dev, bssid, ie, ie_len, reason_code,
				      local_state_change);
=======
	wdev_lock(dev->ieee80211_ptr);
	err = cfg80211_mlme_disassoc(rdev, dev, bssid, ie, ie_len, reason_code,
				     local_state_change);
	wdev_unlock(dev->ieee80211_ptr);
	return err;
>>>>>>> refs/remotes/origin/master
}

static bool
nl80211_parse_mcast_rate(struct cfg80211_registered_device *rdev,
			 int mcast_rate[IEEE80211_NUM_BANDS],
			 int rateval)
{
	struct wiphy *wiphy = &rdev->wiphy;
	bool found = false;
	int band, i;

	for (band = 0; band < IEEE80211_NUM_BANDS; band++) {
		struct ieee80211_supported_band *sband;

		sband = wiphy->bands[band];
		if (!sband)
			continue;

		for (i = 0; i < sband->n_bitrates; i++) {
			if (sband->bitrates[i].bitrate == rateval) {
				mcast_rate[band] = i + 1;
				found = true;
				break;
			}
		}
	}

	return found;
}

static int nl80211_join_ibss(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];
	struct cfg80211_ibss_params ibss;
	struct wiphy *wiphy;
	struct cfg80211_cached_keys *connkeys = NULL;
	int err;

	memset(&ibss, 0, sizeof(ibss));

	if (!is_valid_ie_attr(info->attrs[NL80211_ATTR_IE]))
		return -EINVAL;

<<<<<<< HEAD
	if (!info->attrs[NL80211_ATTR_WIPHY_FREQ] ||
	    !info->attrs[NL80211_ATTR_SSID] ||
=======
	if (!info->attrs[NL80211_ATTR_SSID] ||
>>>>>>> refs/remotes/origin/master
	    !nla_len(info->attrs[NL80211_ATTR_SSID]))
		return -EINVAL;

	ibss.beacon_interval = 100;

	if (info->attrs[NL80211_ATTR_BEACON_INTERVAL]) {
		ibss.beacon_interval =
			nla_get_u32(info->attrs[NL80211_ATTR_BEACON_INTERVAL]);
		if (ibss.beacon_interval < 1 || ibss.beacon_interval > 10000)
			return -EINVAL;
	}

	if (!rdev->ops->join_ibss)
		return -EOPNOTSUPP;

	if (dev->ieee80211_ptr->iftype != NL80211_IFTYPE_ADHOC)
		return -EOPNOTSUPP;

	wiphy = &rdev->wiphy;

<<<<<<< HEAD
<<<<<<< HEAD
	if (info->attrs[NL80211_ATTR_MAC])
		ibss.bssid = nla_data(info->attrs[NL80211_ATTR_MAC]);
=======
=======
>>>>>>> refs/remotes/origin/master
	if (info->attrs[NL80211_ATTR_MAC]) {
		ibss.bssid = nla_data(info->attrs[NL80211_ATTR_MAC]);

		if (!is_valid_ether_addr(ibss.bssid))
			return -EINVAL;
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	ibss.ssid = nla_data(info->attrs[NL80211_ATTR_SSID]);
	ibss.ssid_len = nla_len(info->attrs[NL80211_ATTR_SSID]);

	if (info->attrs[NL80211_ATTR_IE]) {
		ibss.ie = nla_data(info->attrs[NL80211_ATTR_IE]);
		ibss.ie_len = nla_len(info->attrs[NL80211_ATTR_IE]);
	}

<<<<<<< HEAD
<<<<<<< HEAD
	ibss.channel = ieee80211_get_channel(wiphy,
		nla_get_u32(info->attrs[NL80211_ATTR_WIPHY_FREQ]));
=======
	if (info->attrs[NL80211_ATTR_WIPHY_CHANNEL_TYPE]) {
		enum nl80211_channel_type channel_type;

		channel_type = nla_get_u32(
				info->attrs[NL80211_ATTR_WIPHY_CHANNEL_TYPE]);
		if (channel_type != NL80211_CHAN_NO_HT &&
		    channel_type != NL80211_CHAN_HT20 &&
		    channel_type != NL80211_CHAN_HT40MINUS &&
		    channel_type != NL80211_CHAN_HT40PLUS)
			return -EINVAL;

		if (channel_type != NL80211_CHAN_NO_HT &&
		    !(wiphy->features & NL80211_FEATURE_HT_IBSS))
			return -EINVAL;

		ibss.channel_type = channel_type;
	} else {
		ibss.channel_type = NL80211_CHAN_NO_HT;
	}

	ibss.channel = rdev_freq_to_chan(rdev,
		nla_get_u32(info->attrs[NL80211_ATTR_WIPHY_FREQ]),
		ibss.channel_type);
>>>>>>> refs/remotes/origin/cm-10.0
	if (!ibss.channel ||
	    ibss.channel->flags & IEEE80211_CHAN_NO_IBSS ||
	    ibss.channel->flags & IEEE80211_CHAN_DISABLED)
		return -EINVAL;

<<<<<<< HEAD
=======
	/* Both channels should be able to initiate communication */
	if ((ibss.channel_type == NL80211_CHAN_HT40PLUS ||
	     ibss.channel_type == NL80211_CHAN_HT40MINUS) &&
	    !cfg80211_can_beacon_sec_chan(&rdev->wiphy, ibss.channel,
					  ibss.channel_type))
		return -EINVAL;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	err = nl80211_parse_chandef(rdev, info, &ibss.chandef);
	if (err)
		return err;

	if (!cfg80211_reg_can_beacon(&rdev->wiphy, &ibss.chandef))
		return -EINVAL;

	switch (ibss.chandef.width) {
	case NL80211_CHAN_WIDTH_5:
	case NL80211_CHAN_WIDTH_10:
	case NL80211_CHAN_WIDTH_20_NOHT:
		break;
	case NL80211_CHAN_WIDTH_20:
	case NL80211_CHAN_WIDTH_40:
		if (rdev->wiphy.features & NL80211_FEATURE_HT_IBSS)
			break;
	default:
		return -EINVAL;
	}

>>>>>>> refs/remotes/origin/master
	ibss.channel_fixed = !!info->attrs[NL80211_ATTR_FREQ_FIXED];
	ibss.privacy = !!info->attrs[NL80211_ATTR_PRIVACY];

	if (info->attrs[NL80211_ATTR_BSS_BASIC_RATES]) {
		u8 *rates =
			nla_data(info->attrs[NL80211_ATTR_BSS_BASIC_RATES]);
		int n_rates =
			nla_len(info->attrs[NL80211_ATTR_BSS_BASIC_RATES]);
		struct ieee80211_supported_band *sband =
<<<<<<< HEAD
			wiphy->bands[ibss.channel->band];
<<<<<<< HEAD
		int err;
=======

>>>>>>> refs/remotes/origin/cm-10.0
=======
			wiphy->bands[ibss.chandef.chan->band];

>>>>>>> refs/remotes/origin/master
		err = ieee80211_get_ratemask(sband, rates, n_rates,
					     &ibss.basic_rates);
		if (err)
			return err;
	}

<<<<<<< HEAD
=======
	if (info->attrs[NL80211_ATTR_HT_CAPABILITY_MASK])
		memcpy(&ibss.ht_capa_mask,
		       nla_data(info->attrs[NL80211_ATTR_HT_CAPABILITY_MASK]),
		       sizeof(ibss.ht_capa_mask));

	if (info->attrs[NL80211_ATTR_HT_CAPABILITY]) {
		if (!info->attrs[NL80211_ATTR_HT_CAPABILITY_MASK])
			return -EINVAL;
		memcpy(&ibss.ht_capa,
		       nla_data(info->attrs[NL80211_ATTR_HT_CAPABILITY]),
		       sizeof(ibss.ht_capa));
	}

>>>>>>> refs/remotes/origin/master
	if (info->attrs[NL80211_ATTR_MCAST_RATE] &&
	    !nl80211_parse_mcast_rate(rdev, ibss.mcast_rate,
			nla_get_u32(info->attrs[NL80211_ATTR_MCAST_RATE])))
		return -EINVAL;

	if (ibss.privacy && info->attrs[NL80211_ATTR_KEYS]) {
<<<<<<< HEAD
		connkeys = nl80211_parse_connkeys(rdev,
					info->attrs[NL80211_ATTR_KEYS]);
		if (IS_ERR(connkeys))
			return PTR_ERR(connkeys);
	}

<<<<<<< HEAD
=======
	ibss.control_port =
		nla_get_flag(info->attrs[NL80211_ATTR_CONTROL_PORT]);

>>>>>>> refs/remotes/origin/cm-10.0
=======
		bool no_ht = false;

		connkeys = nl80211_parse_connkeys(rdev,
					  info->attrs[NL80211_ATTR_KEYS],
					  &no_ht);
		if (IS_ERR(connkeys))
			return PTR_ERR(connkeys);

		if ((ibss.chandef.width != NL80211_CHAN_WIDTH_20_NOHT) &&
		    no_ht) {
			kfree(connkeys);
			return -EINVAL;
		}
	}

	ibss.control_port =
		nla_get_flag(info->attrs[NL80211_ATTR_CONTROL_PORT]);

	ibss.userspace_handles_dfs =
		nla_get_flag(info->attrs[NL80211_ATTR_HANDLE_DFS]);

>>>>>>> refs/remotes/origin/master
	err = cfg80211_join_ibss(rdev, dev, &ibss, connkeys);
	if (err)
		kfree(connkeys);
	return err;
}

static int nl80211_leave_ibss(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];

	if (!rdev->ops->leave_ibss)
		return -EOPNOTSUPP;

	if (dev->ieee80211_ptr->iftype != NL80211_IFTYPE_ADHOC)
		return -EOPNOTSUPP;

	return cfg80211_leave_ibss(rdev, dev, false);
}

<<<<<<< HEAD
#ifdef CONFIG_NL80211_TESTMODE
static struct genl_multicast_group nl80211_testmode_mcgrp = {
	.name = "testmode",
};

static int nl80211_testmode_do(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	int err;

	if (!info->attrs[NL80211_ATTR_TESTDATA])
		return -EINVAL;

	err = -EOPNOTSUPP;
	if (rdev->ops->testmode_cmd) {
		rdev->testmode_info = info;
		err = rdev->ops->testmode_cmd(&rdev->wiphy,
				nla_data(info->attrs[NL80211_ATTR_TESTDATA]),
				nla_len(info->attrs[NL80211_ATTR_TESTDATA]));
		rdev->testmode_info = NULL;
	}
=======
static int nl80211_set_mcast_rate(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];
	int mcast_rate[IEEE80211_NUM_BANDS];
	u32 nla_rate;
	int err;

	if (dev->ieee80211_ptr->iftype != NL80211_IFTYPE_ADHOC &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_MESH_POINT)
		return -EOPNOTSUPP;

	if (!rdev->ops->set_mcast_rate)
		return -EOPNOTSUPP;

	memset(mcast_rate, 0, sizeof(mcast_rate));

	if (!info->attrs[NL80211_ATTR_MCAST_RATE])
		return -EINVAL;

	nla_rate = nla_get_u32(info->attrs[NL80211_ATTR_MCAST_RATE]);
	if (!nl80211_parse_mcast_rate(rdev, mcast_rate, nla_rate))
		return -EINVAL;

	err = rdev->ops->set_mcast_rate(&rdev->wiphy, dev, mcast_rate);

	return err;
}


#ifdef CONFIG_NL80211_TESTMODE
static int nl80211_testmode_do(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct wireless_dev *wdev =
		__cfg80211_wdev_from_attrs(genl_info_net(info), info->attrs);
	int err;

	if (!rdev->ops->testmode_cmd)
		return -EOPNOTSUPP;

	if (IS_ERR(wdev)) {
		err = PTR_ERR(wdev);
		if (err != -EINVAL)
			return err;
		wdev = NULL;
	} else if (wdev->wiphy != &rdev->wiphy) {
		return -EINVAL;
	}

	if (!info->attrs[NL80211_ATTR_TESTDATA])
		return -EINVAL;

	rdev->testmode_info = info;
	err = rdev_testmode_cmd(rdev, wdev,
				nla_data(info->attrs[NL80211_ATTR_TESTDATA]),
				nla_len(info->attrs[NL80211_ATTR_TESTDATA]));
	rdev->testmode_info = NULL;
>>>>>>> refs/remotes/origin/master

	return err;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static int nl80211_testmode_dump(struct sk_buff *skb,
				 struct netlink_callback *cb)
{
	struct cfg80211_registered_device *rdev;
	int err;
	long phy_idx;
	void *data = NULL;
	int data_len = 0;

<<<<<<< HEAD
=======
	rtnl_lock();

>>>>>>> refs/remotes/origin/master
	if (cb->args[0]) {
		/*
		 * 0 is a valid index, but not valid for args[0],
		 * so we need to offset by 1.
		 */
		phy_idx = cb->args[0] - 1;
	} else {
		err = nlmsg_parse(cb->nlh, GENL_HDRLEN + nl80211_fam.hdrsize,
				  nl80211_fam.attrbuf, nl80211_fam.maxattr,
				  nl80211_policy);
		if (err)
<<<<<<< HEAD
			return err;
		if (nl80211_fam.attrbuf[NL80211_ATTR_WIPHY]) {
			phy_idx = nla_get_u32(
				nl80211_fam.attrbuf[NL80211_ATTR_WIPHY]);
		} else {
			struct net_device *netdev;

			err = get_rdev_dev_by_ifindex(sock_net(skb->sk),
						      nl80211_fam.attrbuf,
						      &rdev, &netdev);
			if (err)
				return err;
			dev_put(netdev);
			phy_idx = rdev->wiphy_idx;
			cfg80211_unlock_rdev(rdev);
		}
=======
			goto out_err;

		rdev = __cfg80211_rdev_from_attrs(sock_net(skb->sk),
						  nl80211_fam.attrbuf);
		if (IS_ERR(rdev)) {
			err = PTR_ERR(rdev);
			goto out_err;
		}
		phy_idx = rdev->wiphy_idx;
		rdev = NULL;

>>>>>>> refs/remotes/origin/master
		if (nl80211_fam.attrbuf[NL80211_ATTR_TESTDATA])
			cb->args[1] =
				(long)nl80211_fam.attrbuf[NL80211_ATTR_TESTDATA];
	}

	if (cb->args[1]) {
		data = nla_data((void *)cb->args[1]);
		data_len = nla_len((void *)cb->args[1]);
	}

<<<<<<< HEAD
	mutex_lock(&cfg80211_mutex);
	rdev = cfg80211_rdev_by_wiphy_idx(phy_idx);
	if (!rdev) {
		mutex_unlock(&cfg80211_mutex);
		return -ENOENT;
	}
	cfg80211_lock_rdev(rdev);
	mutex_unlock(&cfg80211_mutex);
=======
	rdev = cfg80211_rdev_by_wiphy_idx(phy_idx);
	if (!rdev) {
		err = -ENOENT;
		goto out_err;
	}
>>>>>>> refs/remotes/origin/master

	if (!rdev->ops->testmode_dump) {
		err = -EOPNOTSUPP;
		goto out_err;
	}

	while (1) {
<<<<<<< HEAD
		void *hdr = nl80211hdr_put(skb, NETLINK_CB(cb->skb).pid,
=======
		void *hdr = nl80211hdr_put(skb, NETLINK_CB(cb->skb).portid,
>>>>>>> refs/remotes/origin/master
					   cb->nlh->nlmsg_seq, NLM_F_MULTI,
					   NL80211_CMD_TESTMODE);
		struct nlattr *tmdata;

<<<<<<< HEAD
		if (nla_put_u32(skb, NL80211_ATTR_WIPHY, phy_idx) < 0) {
=======
		if (!hdr)
			break;

		if (nla_put_u32(skb, NL80211_ATTR_WIPHY, phy_idx)) {
>>>>>>> refs/remotes/origin/master
			genlmsg_cancel(skb, hdr);
			break;
		}

		tmdata = nla_nest_start(skb, NL80211_ATTR_TESTDATA);
		if (!tmdata) {
			genlmsg_cancel(skb, hdr);
			break;
		}
<<<<<<< HEAD
		err = rdev->ops->testmode_dump(&rdev->wiphy, skb, cb,
					       data, data_len);
=======
		err = rdev_testmode_dump(rdev, skb, cb, data, data_len);
>>>>>>> refs/remotes/origin/master
		nla_nest_end(skb, tmdata);

		if (err == -ENOBUFS || err == -ENOENT) {
			genlmsg_cancel(skb, hdr);
			break;
		} else if (err) {
			genlmsg_cancel(skb, hdr);
			goto out_err;
		}

		genlmsg_end(skb, hdr);
	}

	err = skb->len;
	/* see above */
	cb->args[0] = phy_idx + 1;
 out_err:
<<<<<<< HEAD
	cfg80211_unlock_rdev(rdev);
	return err;
}

>>>>>>> refs/remotes/origin/cm-10.0
static struct sk_buff *
__cfg80211_testmode_alloc_skb(struct cfg80211_registered_device *rdev,
			      int approxlen, u32 pid, u32 seq, gfp_t gfp)
=======
	rtnl_unlock();
	return err;
}

static struct sk_buff *
__cfg80211_testmode_alloc_skb(struct cfg80211_registered_device *rdev,
			      int approxlen, u32 portid, u32 seq, gfp_t gfp)
>>>>>>> refs/remotes/origin/master
{
	struct sk_buff *skb;
	void *hdr;
	struct nlattr *data;

	skb = nlmsg_new(approxlen + 100, gfp);
	if (!skb)
		return NULL;

<<<<<<< HEAD
	hdr = nl80211hdr_put(skb, pid, seq, 0, NL80211_CMD_TESTMODE);
=======
	hdr = nl80211hdr_put(skb, portid, seq, 0, NL80211_CMD_TESTMODE);
>>>>>>> refs/remotes/origin/master
	if (!hdr) {
		kfree_skb(skb);
		return NULL;
	}

<<<<<<< HEAD
	NLA_PUT_U32(skb, NL80211_ATTR_WIPHY, rdev->wiphy_idx);
=======
	if (nla_put_u32(skb, NL80211_ATTR_WIPHY, rdev->wiphy_idx))
		goto nla_put_failure;
>>>>>>> refs/remotes/origin/master
	data = nla_nest_start(skb, NL80211_ATTR_TESTDATA);

	((void **)skb->cb)[0] = rdev;
	((void **)skb->cb)[1] = hdr;
	((void **)skb->cb)[2] = data;

	return skb;

 nla_put_failure:
	kfree_skb(skb);
	return NULL;
}

struct sk_buff *cfg80211_testmode_alloc_reply_skb(struct wiphy *wiphy,
						  int approxlen)
{
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);

	if (WARN_ON(!rdev->testmode_info))
		return NULL;

	return __cfg80211_testmode_alloc_skb(rdev, approxlen,
<<<<<<< HEAD
				rdev->testmode_info->snd_pid,
=======
				rdev->testmode_info->snd_portid,
>>>>>>> refs/remotes/origin/master
				rdev->testmode_info->snd_seq,
				GFP_KERNEL);
}
EXPORT_SYMBOL(cfg80211_testmode_alloc_reply_skb);

int cfg80211_testmode_reply(struct sk_buff *skb)
{
	struct cfg80211_registered_device *rdev = ((void **)skb->cb)[0];
	void *hdr = ((void **)skb->cb)[1];
	struct nlattr *data = ((void **)skb->cb)[2];

	if (WARN_ON(!rdev->testmode_info)) {
		kfree_skb(skb);
		return -EINVAL;
	}

	nla_nest_end(skb, data);
	genlmsg_end(skb, hdr);
	return genlmsg_reply(skb, rdev->testmode_info);
}
EXPORT_SYMBOL(cfg80211_testmode_reply);

struct sk_buff *cfg80211_testmode_alloc_event_skb(struct wiphy *wiphy,
						  int approxlen, gfp_t gfp)
{
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);

	return __cfg80211_testmode_alloc_skb(rdev, approxlen, 0, 0, gfp);
}
EXPORT_SYMBOL(cfg80211_testmode_alloc_event_skb);

void cfg80211_testmode_event(struct sk_buff *skb, gfp_t gfp)
{
	struct cfg80211_registered_device *rdev = ((void **)skb->cb)[0];
	void *hdr = ((void **)skb->cb)[1];
	struct nlattr *data = ((void **)skb->cb)[2];

	nla_nest_end(skb, data);
	genlmsg_end(skb, hdr);
<<<<<<< HEAD
	genlmsg_multicast_netns(wiphy_net(&rdev->wiphy), skb, 0,
				nl80211_testmode_mcgrp.id, gfp);
=======
	genlmsg_multicast_netns(&nl80211_fam, wiphy_net(&rdev->wiphy), skb, 0,
				NL80211_MCGRP_TESTMODE, gfp);
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL(cfg80211_testmode_event);
#endif

static int nl80211_connect(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];
	struct cfg80211_connect_params connect;
	struct wiphy *wiphy;
	struct cfg80211_cached_keys *connkeys = NULL;
	int err;

	memset(&connect, 0, sizeof(connect));

	if (!is_valid_ie_attr(info->attrs[NL80211_ATTR_IE]))
		return -EINVAL;

	if (!info->attrs[NL80211_ATTR_SSID] ||
	    !nla_len(info->attrs[NL80211_ATTR_SSID]))
		return -EINVAL;

	if (info->attrs[NL80211_ATTR_AUTH_TYPE]) {
		connect.auth_type =
			nla_get_u32(info->attrs[NL80211_ATTR_AUTH_TYPE]);
<<<<<<< HEAD
		if (!nl80211_valid_auth_type(connect.auth_type))
=======
		if (!nl80211_valid_auth_type(rdev, connect.auth_type,
					     NL80211_CMD_CONNECT))
>>>>>>> refs/remotes/origin/master
			return -EINVAL;
	} else
		connect.auth_type = NL80211_AUTHTYPE_AUTOMATIC;

	connect.privacy = info->attrs[NL80211_ATTR_PRIVACY];

	err = nl80211_crypto_settings(rdev, info, &connect.crypto,
				      NL80211_MAX_NR_CIPHER_SUITES);
	if (err)
		return err;

	if (dev->ieee80211_ptr->iftype != NL80211_IFTYPE_STATION &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_P2P_CLIENT)
		return -EOPNOTSUPP;

	wiphy = &rdev->wiphy;

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	connect.bg_scan_period = -1;
	if (info->attrs[NL80211_ATTR_BG_SCAN_PERIOD] &&
		(wiphy->flags & WIPHY_FLAG_SUPPORTS_FW_ROAM)) {
		connect.bg_scan_period =
			nla_get_u16(info->attrs[NL80211_ATTR_BG_SCAN_PERIOD]);
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (info->attrs[NL80211_ATTR_MAC])
		connect.bssid = nla_data(info->attrs[NL80211_ATTR_MAC]);
	connect.ssid = nla_data(info->attrs[NL80211_ATTR_SSID]);
	connect.ssid_len = nla_len(info->attrs[NL80211_ATTR_SSID]);

	if (info->attrs[NL80211_ATTR_IE]) {
		connect.ie = nla_data(info->attrs[NL80211_ATTR_IE]);
		connect.ie_len = nla_len(info->attrs[NL80211_ATTR_IE]);
	}

<<<<<<< HEAD
=======
	if (info->attrs[NL80211_ATTR_USE_MFP]) {
		connect.mfp = nla_get_u32(info->attrs[NL80211_ATTR_USE_MFP]);
		if (connect.mfp != NL80211_MFP_REQUIRED &&
		    connect.mfp != NL80211_MFP_NO)
			return -EINVAL;
	} else {
		connect.mfp = NL80211_MFP_NO;
	}

>>>>>>> refs/remotes/origin/master
	if (info->attrs[NL80211_ATTR_WIPHY_FREQ]) {
		connect.channel =
			ieee80211_get_channel(wiphy,
			    nla_get_u32(info->attrs[NL80211_ATTR_WIPHY_FREQ]));
		if (!connect.channel ||
		    connect.channel->flags & IEEE80211_CHAN_DISABLED)
			return -EINVAL;
	}

	if (connect.privacy && info->attrs[NL80211_ATTR_KEYS]) {
		connkeys = nl80211_parse_connkeys(rdev,
<<<<<<< HEAD
					info->attrs[NL80211_ATTR_KEYS]);
=======
					  info->attrs[NL80211_ATTR_KEYS], NULL);
>>>>>>> refs/remotes/origin/master
		if (IS_ERR(connkeys))
			return PTR_ERR(connkeys);
	}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	if (nla_get_flag(info->attrs[NL80211_ATTR_DISABLE_HT]))
		connect.flags |= ASSOC_REQ_DISABLE_HT;

	if (info->attrs[NL80211_ATTR_HT_CAPABILITY_MASK])
		memcpy(&connect.ht_capa_mask,
		       nla_data(info->attrs[NL80211_ATTR_HT_CAPABILITY_MASK]),
		       sizeof(connect.ht_capa_mask));

	if (info->attrs[NL80211_ATTR_HT_CAPABILITY]) {
<<<<<<< HEAD
		if (!info->attrs[NL80211_ATTR_HT_CAPABILITY_MASK])
			return -EINVAL;
=======
		if (!info->attrs[NL80211_ATTR_HT_CAPABILITY_MASK]) {
			kfree(connkeys);
			return -EINVAL;
		}
>>>>>>> refs/remotes/origin/master
		memcpy(&connect.ht_capa,
		       nla_data(info->attrs[NL80211_ATTR_HT_CAPABILITY]),
		       sizeof(connect.ht_capa));
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	err = cfg80211_connect(rdev, dev, &connect, connkeys);
=======
	if (nla_get_flag(info->attrs[NL80211_ATTR_DISABLE_VHT]))
		connect.flags |= ASSOC_REQ_DISABLE_VHT;

	if (info->attrs[NL80211_ATTR_VHT_CAPABILITY_MASK])
		memcpy(&connect.vht_capa_mask,
		       nla_data(info->attrs[NL80211_ATTR_VHT_CAPABILITY_MASK]),
		       sizeof(connect.vht_capa_mask));

	if (info->attrs[NL80211_ATTR_VHT_CAPABILITY]) {
		if (!info->attrs[NL80211_ATTR_VHT_CAPABILITY_MASK]) {
			kfree(connkeys);
			return -EINVAL;
		}
		memcpy(&connect.vht_capa,
		       nla_data(info->attrs[NL80211_ATTR_VHT_CAPABILITY]),
		       sizeof(connect.vht_capa));
	}

	wdev_lock(dev->ieee80211_ptr);
	err = cfg80211_connect(rdev, dev, &connect, connkeys, NULL);
	wdev_unlock(dev->ieee80211_ptr);
>>>>>>> refs/remotes/origin/master
	if (err)
		kfree(connkeys);
	return err;
}

static int nl80211_disconnect(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];
	u16 reason;
<<<<<<< HEAD
=======
	int ret;
>>>>>>> refs/remotes/origin/master

	if (!info->attrs[NL80211_ATTR_REASON_CODE])
		reason = WLAN_REASON_DEAUTH_LEAVING;
	else
		reason = nla_get_u16(info->attrs[NL80211_ATTR_REASON_CODE]);

	if (reason == 0)
		return -EINVAL;

	if (dev->ieee80211_ptr->iftype != NL80211_IFTYPE_STATION &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_P2P_CLIENT)
		return -EOPNOTSUPP;

<<<<<<< HEAD
	return cfg80211_disconnect(rdev, dev, reason, true);
=======
	wdev_lock(dev->ieee80211_ptr);
	ret = cfg80211_disconnect(rdev, dev, reason, true);
	wdev_unlock(dev->ieee80211_ptr);
	return ret;
>>>>>>> refs/remotes/origin/master
}

static int nl80211_wiphy_netns(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net *net;
	int err;
	u32 pid;

	if (!info->attrs[NL80211_ATTR_PID])
		return -EINVAL;

	pid = nla_get_u32(info->attrs[NL80211_ATTR_PID]);

	net = get_net_ns_by_pid(pid);
	if (IS_ERR(net))
		return PTR_ERR(net);

	err = 0;

	/* check if anything to do */
	if (!net_eq(wiphy_net(&rdev->wiphy), net))
		err = cfg80211_switch_netns(rdev, net);

	put_net(net);
	return err;
}

static int nl80211_setdel_pmksa(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	int (*rdev_ops)(struct wiphy *wiphy, struct net_device *dev,
			struct cfg80211_pmksa *pmksa) = NULL;
	struct net_device *dev = info->user_ptr[1];
	struct cfg80211_pmksa pmksa;

	memset(&pmksa, 0, sizeof(struct cfg80211_pmksa));

	if (!info->attrs[NL80211_ATTR_MAC])
		return -EINVAL;

	if (!info->attrs[NL80211_ATTR_PMKID])
		return -EINVAL;

	pmksa.pmkid = nla_data(info->attrs[NL80211_ATTR_PMKID]);
	pmksa.bssid = nla_data(info->attrs[NL80211_ATTR_MAC]);

	if (dev->ieee80211_ptr->iftype != NL80211_IFTYPE_STATION &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_P2P_CLIENT)
		return -EOPNOTSUPP;

	switch (info->genlhdr->cmd) {
	case NL80211_CMD_SET_PMKSA:
		rdev_ops = rdev->ops->set_pmksa;
		break;
	case NL80211_CMD_DEL_PMKSA:
		rdev_ops = rdev->ops->del_pmksa;
		break;
	default:
		WARN_ON(1);
		break;
	}

	if (!rdev_ops)
		return -EOPNOTSUPP;

	return rdev_ops(&rdev->wiphy, dev, &pmksa);
}

static int nl80211_flush_pmksa(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];

	if (dev->ieee80211_ptr->iftype != NL80211_IFTYPE_STATION &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_P2P_CLIENT)
		return -EOPNOTSUPP;

	if (!rdev->ops->flush_pmksa)
		return -EOPNOTSUPP;

<<<<<<< HEAD
	return rdev->ops->flush_pmksa(&rdev->wiphy, dev);
}

<<<<<<< HEAD
=======
=======
	return rdev_flush_pmksa(rdev, dev);
}

>>>>>>> refs/remotes/origin/master
static int nl80211_tdls_mgmt(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];
	u8 action_code, dialog_token;
	u16 status_code;
	u8 *peer;

	if (!(rdev->wiphy.flags & WIPHY_FLAG_SUPPORTS_TDLS) ||
	    !rdev->ops->tdls_mgmt)
		return -EOPNOTSUPP;

	if (!info->attrs[NL80211_ATTR_TDLS_ACTION] ||
	    !info->attrs[NL80211_ATTR_STATUS_CODE] ||
	    !info->attrs[NL80211_ATTR_TDLS_DIALOG_TOKEN] ||
	    !info->attrs[NL80211_ATTR_IE] ||
	    !info->attrs[NL80211_ATTR_MAC])
		return -EINVAL;

	peer = nla_data(info->attrs[NL80211_ATTR_MAC]);
	action_code = nla_get_u8(info->attrs[NL80211_ATTR_TDLS_ACTION]);
	status_code = nla_get_u16(info->attrs[NL80211_ATTR_STATUS_CODE]);
	dialog_token = nla_get_u8(info->attrs[NL80211_ATTR_TDLS_DIALOG_TOKEN]);

<<<<<<< HEAD
	return rdev->ops->tdls_mgmt(&rdev->wiphy, dev, peer, action_code,
				    dialog_token, status_code,
				    nla_data(info->attrs[NL80211_ATTR_IE]),
				    nla_len(info->attrs[NL80211_ATTR_IE]));
=======
	return rdev_tdls_mgmt(rdev, dev, peer, action_code,
			      dialog_token, status_code,
			      nla_data(info->attrs[NL80211_ATTR_IE]),
			      nla_len(info->attrs[NL80211_ATTR_IE]));
>>>>>>> refs/remotes/origin/master
}

static int nl80211_tdls_oper(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];
	enum nl80211_tdls_operation operation;
	u8 *peer;

	if (!(rdev->wiphy.flags & WIPHY_FLAG_SUPPORTS_TDLS) ||
	    !rdev->ops->tdls_oper)
		return -EOPNOTSUPP;

	if (!info->attrs[NL80211_ATTR_TDLS_OPERATION] ||
	    !info->attrs[NL80211_ATTR_MAC])
		return -EINVAL;

	operation = nla_get_u8(info->attrs[NL80211_ATTR_TDLS_OPERATION]);
	peer = nla_data(info->attrs[NL80211_ATTR_MAC]);

<<<<<<< HEAD
	return rdev->ops->tdls_oper(&rdev->wiphy, dev, peer, operation);
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
	return rdev_tdls_oper(rdev, dev, peer, operation);
}

>>>>>>> refs/remotes/origin/master
static int nl80211_remain_on_channel(struct sk_buff *skb,
				     struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
<<<<<<< HEAD
	struct net_device *dev = info->user_ptr[1];
	struct ieee80211_channel *chan;
	struct sk_buff *msg;
	void *hdr;
	u64 cookie;
	enum nl80211_channel_type channel_type = NL80211_CHAN_NO_HT;
	u32 freq, duration;
=======
	struct wireless_dev *wdev = info->user_ptr[1];
	struct cfg80211_chan_def chandef;
	struct sk_buff *msg;
	void *hdr;
	u64 cookie;
	u32 duration;
>>>>>>> refs/remotes/origin/master
	int err;

	if (!info->attrs[NL80211_ATTR_WIPHY_FREQ] ||
	    !info->attrs[NL80211_ATTR_DURATION])
		return -EINVAL;

	duration = nla_get_u32(info->attrs[NL80211_ATTR_DURATION]);

<<<<<<< HEAD
	/*
	 * We should be on that channel for at least one jiffie,
	 * and more than 5 seconds seems excessive.
	 */
	if (!duration || !msecs_to_jiffies(duration) ||
	    duration > rdev->wiphy.max_remain_on_channel_duration)
		return -EINVAL;

<<<<<<< HEAD
	if (!rdev->ops->remain_on_channel)
=======
	if (!rdev->ops->remain_on_channel ||
	    !(rdev->wiphy.flags & WIPHY_FLAG_HAS_REMAIN_ON_CHANNEL))
>>>>>>> refs/remotes/origin/cm-10.0
		return -EOPNOTSUPP;

	if (info->attrs[NL80211_ATTR_WIPHY_CHANNEL_TYPE]) {
		channel_type = nla_get_u32(
			info->attrs[NL80211_ATTR_WIPHY_CHANNEL_TYPE]);
		if (channel_type != NL80211_CHAN_NO_HT &&
		    channel_type != NL80211_CHAN_HT20 &&
		    channel_type != NL80211_CHAN_HT40PLUS &&
		    channel_type != NL80211_CHAN_HT40MINUS)
			return -EINVAL;
	}

	freq = nla_get_u32(info->attrs[NL80211_ATTR_WIPHY_FREQ]);
	chan = rdev_freq_to_chan(rdev, freq, channel_type);
	if (chan == NULL)
		return -EINVAL;

=======
	if (!rdev->ops->remain_on_channel ||
	    !(rdev->wiphy.flags & WIPHY_FLAG_HAS_REMAIN_ON_CHANNEL))
		return -EOPNOTSUPP;

	/*
	 * We should be on that channel for at least a minimum amount of
	 * time (10ms) but no longer than the driver supports.
	 */
	if (duration < NL80211_MIN_REMAIN_ON_CHANNEL_TIME ||
	    duration > rdev->wiphy.max_remain_on_channel_duration)
		return -EINVAL;

	err = nl80211_parse_chandef(rdev, info, &chandef);
	if (err)
		return err;

>>>>>>> refs/remotes/origin/master
	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (!msg)
		return -ENOMEM;

<<<<<<< HEAD
	hdr = nl80211hdr_put(msg, info->snd_pid, info->snd_seq, 0,
			     NL80211_CMD_REMAIN_ON_CHANNEL);

	if (IS_ERR(hdr)) {
		err = PTR_ERR(hdr);
		goto free_msg;
	}

	err = rdev->ops->remain_on_channel(&rdev->wiphy, dev, chan,
					   channel_type, duration, &cookie);
=======
	hdr = nl80211hdr_put(msg, info->snd_portid, info->snd_seq, 0,
			     NL80211_CMD_REMAIN_ON_CHANNEL);
	if (!hdr) {
		err = -ENOBUFS;
		goto free_msg;
	}

	err = rdev_remain_on_channel(rdev, wdev, chandef.chan,
				     duration, &cookie);
>>>>>>> refs/remotes/origin/master

	if (err)
		goto free_msg;

<<<<<<< HEAD
	NLA_PUT_U64(msg, NL80211_ATTR_COOKIE, cookie);
=======
	if (nla_put_u64(msg, NL80211_ATTR_COOKIE, cookie))
		goto nla_put_failure;
>>>>>>> refs/remotes/origin/master

	genlmsg_end(msg, hdr);

	return genlmsg_reply(msg, info);

 nla_put_failure:
	err = -ENOBUFS;
 free_msg:
	nlmsg_free(msg);
	return err;
}

static int nl80211_cancel_remain_on_channel(struct sk_buff *skb,
					    struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
<<<<<<< HEAD
	struct net_device *dev = info->user_ptr[1];
=======
	struct wireless_dev *wdev = info->user_ptr[1];
>>>>>>> refs/remotes/origin/master
	u64 cookie;

	if (!info->attrs[NL80211_ATTR_COOKIE])
		return -EINVAL;

	if (!rdev->ops->cancel_remain_on_channel)
		return -EOPNOTSUPP;

	cookie = nla_get_u64(info->attrs[NL80211_ATTR_COOKIE]);

<<<<<<< HEAD
	return rdev->ops->cancel_remain_on_channel(&rdev->wiphy, dev, cookie);
=======
	return rdev_cancel_remain_on_channel(rdev, wdev, cookie);
>>>>>>> refs/remotes/origin/master
}

static u32 rateset_to_mask(struct ieee80211_supported_band *sband,
			   u8 *rates, u8 rates_len)
{
	u8 i;
	u32 mask = 0;

	for (i = 0; i < rates_len; i++) {
		int rate = (rates[i] & 0x7f) * 5;
		int ridx;
		for (ridx = 0; ridx < sband->n_bitrates; ridx++) {
			struct ieee80211_rate *srate =
				&sband->bitrates[ridx];
			if (rate == srate->bitrate) {
				mask |= 1 << ridx;
				break;
			}
		}
		if (ridx == sband->n_bitrates)
			return 0; /* rate not found */
	}

	return mask;
}

<<<<<<< HEAD
<<<<<<< HEAD
static const struct nla_policy nl80211_txattr_policy[NL80211_TXRATE_MAX + 1] = {
	[NL80211_TXRATE_LEGACY] = { .type = NLA_BINARY,
				    .len = NL80211_MAX_SUPP_RATES },
=======
=======
>>>>>>> refs/remotes/origin/master
static bool ht_rateset_to_mask(struct ieee80211_supported_band *sband,
			       u8 *rates, u8 rates_len,
			       u8 mcs[IEEE80211_HT_MCS_MASK_LEN])
{
	u8 i;

	memset(mcs, 0, IEEE80211_HT_MCS_MASK_LEN);

	for (i = 0; i < rates_len; i++) {
		int ridx, rbit;

		ridx = rates[i] / 8;
		rbit = BIT(rates[i] % 8);

		/* check validity */
		if ((ridx < 0) || (ridx >= IEEE80211_HT_MCS_MASK_LEN))
			return false;

		/* check availability */
		if (sband->ht_cap.mcs.rx_mask[ridx] & rbit)
			mcs[ridx] |= rbit;
		else
			return false;
	}

	return true;
}

static const struct nla_policy nl80211_txattr_policy[NL80211_TXRATE_MAX + 1] = {
	[NL80211_TXRATE_LEGACY] = { .type = NLA_BINARY,
				    .len = NL80211_MAX_SUPP_RATES },
	[NL80211_TXRATE_MCS] = { .type = NLA_BINARY,
				 .len = NL80211_MAX_SUPP_HT_RATES },
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};

static int nl80211_set_tx_bitrate_mask(struct sk_buff *skb,
				       struct genl_info *info)
{
	struct nlattr *tb[NL80211_TXRATE_MAX + 1];
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct cfg80211_bitrate_mask mask;
	int rem, i;
	struct net_device *dev = info->user_ptr[1];
	struct nlattr *tx_rates;
	struct ieee80211_supported_band *sband;

	if (info->attrs[NL80211_ATTR_TX_RATES] == NULL)
		return -EINVAL;

	if (!rdev->ops->set_bitrate_mask)
		return -EOPNOTSUPP;

	memset(&mask, 0, sizeof(mask));
	/* Default to all rates enabled */
	for (i = 0; i < IEEE80211_NUM_BANDS; i++) {
		sband = rdev->wiphy.bands[i];
		mask.control[i].legacy =
			sband ? (1 << sband->n_bitrates) - 1 : 0;
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
		if (sband)
			memcpy(mask.control[i].mcs,
			       sband->ht_cap.mcs.rx_mask,
			       sizeof(mask.control[i].mcs));
		else
			memset(mask.control[i].mcs, 0,
			       sizeof(mask.control[i].mcs));
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	/*
	 * The nested attribute uses enum nl80211_band as the index. This maps
	 * directly to the enum ieee80211_band values used in cfg80211.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
=======
	BUILD_BUG_ON(NL80211_MAX_SUPP_HT_RATES > IEEE80211_HT_MCS_MASK_LEN * 8);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	BUILD_BUG_ON(NL80211_MAX_SUPP_HT_RATES > IEEE80211_HT_MCS_MASK_LEN * 8);
>>>>>>> refs/remotes/origin/master
	nla_for_each_nested(tx_rates, info->attrs[NL80211_ATTR_TX_RATES], rem)
	{
		enum ieee80211_band band = nla_type(tx_rates);
		if (band < 0 || band >= IEEE80211_NUM_BANDS)
			return -EINVAL;
		sband = rdev->wiphy.bands[band];
		if (sband == NULL)
			return -EINVAL;
		nla_parse(tb, NL80211_TXRATE_MAX, nla_data(tx_rates),
			  nla_len(tx_rates), nl80211_txattr_policy);
		if (tb[NL80211_TXRATE_LEGACY]) {
			mask.control[band].legacy = rateset_to_mask(
				sband,
				nla_data(tb[NL80211_TXRATE_LEGACY]),
				nla_len(tb[NL80211_TXRATE_LEGACY]));
<<<<<<< HEAD
<<<<<<< HEAD
			if (mask.control[band].legacy == 0)
=======
=======
			if ((mask.control[band].legacy == 0) &&
			    nla_len(tb[NL80211_TXRATE_LEGACY]))
				return -EINVAL;
>>>>>>> refs/remotes/origin/master
		}
		if (tb[NL80211_TXRATE_MCS]) {
			if (!ht_rateset_to_mask(
					sband,
					nla_data(tb[NL80211_TXRATE_MCS]),
					nla_len(tb[NL80211_TXRATE_MCS]),
					mask.control[band].mcs))
				return -EINVAL;
		}

		if (mask.control[band].legacy == 0) {
			/* don't allow empty legacy rates if HT
			 * is not even supported. */
			if (!rdev->wiphy.bands[band]->ht_cap.ht_supported)
				return -EINVAL;

			for (i = 0; i < IEEE80211_HT_MCS_MASK_LEN; i++)
				if (mask.control[band].mcs[i])
					break;

			/* legacy and mcs rates may not be both empty */
			if (i == IEEE80211_HT_MCS_MASK_LEN)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				return -EINVAL;
		}
	}

<<<<<<< HEAD
	return rdev->ops->set_bitrate_mask(&rdev->wiphy, dev, NULL, &mask);
=======
	return rdev_set_bitrate_mask(rdev, dev, NULL, &mask);
>>>>>>> refs/remotes/origin/master
}

static int nl80211_register_mgmt(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
<<<<<<< HEAD
	struct net_device *dev = info->user_ptr[1];
=======
	struct wireless_dev *wdev = info->user_ptr[1];
>>>>>>> refs/remotes/origin/master
	u16 frame_type = IEEE80211_FTYPE_MGMT | IEEE80211_STYPE_ACTION;

	if (!info->attrs[NL80211_ATTR_FRAME_MATCH])
		return -EINVAL;

	if (info->attrs[NL80211_ATTR_FRAME_TYPE])
		frame_type = nla_get_u16(info->attrs[NL80211_ATTR_FRAME_TYPE]);

<<<<<<< HEAD
	if (dev->ieee80211_ptr->iftype != NL80211_IFTYPE_STATION &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_ADHOC &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_P2P_CLIENT &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_AP &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_AP_VLAN &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_MESH_POINT &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_P2P_GO)
		return -EOPNOTSUPP;
=======
	switch (wdev->iftype) {
	case NL80211_IFTYPE_STATION:
	case NL80211_IFTYPE_ADHOC:
	case NL80211_IFTYPE_P2P_CLIENT:
	case NL80211_IFTYPE_AP:
	case NL80211_IFTYPE_AP_VLAN:
	case NL80211_IFTYPE_MESH_POINT:
	case NL80211_IFTYPE_P2P_GO:
	case NL80211_IFTYPE_P2P_DEVICE:
		break;
	default:
		return -EOPNOTSUPP;
	}
>>>>>>> refs/remotes/origin/master

	/* not much point in registering if we can't reply */
	if (!rdev->ops->mgmt_tx)
		return -EOPNOTSUPP;

<<<<<<< HEAD
	return cfg80211_mlme_register_mgmt(dev->ieee80211_ptr, info->snd_pid,
			frame_type,
=======
	return cfg80211_mlme_register_mgmt(wdev, info->snd_portid, frame_type,
>>>>>>> refs/remotes/origin/master
			nla_data(info->attrs[NL80211_ATTR_FRAME_MATCH]),
			nla_len(info->attrs[NL80211_ATTR_FRAME_MATCH]));
}

static int nl80211_tx_mgmt(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
<<<<<<< HEAD
	struct net_device *dev = info->user_ptr[1];
	struct ieee80211_channel *chan;
	enum nl80211_channel_type channel_type = NL80211_CHAN_NO_HT;
	bool channel_type_valid = false;
	u32 freq;
	int err;
<<<<<<< HEAD
	void *hdr;
	u64 cookie;
	struct sk_buff *msg;
	unsigned int wait = 0;
	bool offchan;
=======
=======
	struct wireless_dev *wdev = info->user_ptr[1];
	struct cfg80211_chan_def chandef;
	int err;
>>>>>>> refs/remotes/origin/master
	void *hdr = NULL;
	u64 cookie;
	struct sk_buff *msg = NULL;
	unsigned int wait = 0;
	bool offchan, no_cck, dont_wait_for_ack;

	dont_wait_for_ack = info->attrs[NL80211_ATTR_DONT_WAIT_FOR_ACK];
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

	if (!info->attrs[NL80211_ATTR_FRAME] ||
	    !info->attrs[NL80211_ATTR_WIPHY_FREQ])
=======

	if (!info->attrs[NL80211_ATTR_FRAME])
>>>>>>> refs/remotes/origin/master
		return -EINVAL;

	if (!rdev->ops->mgmt_tx)
		return -EOPNOTSUPP;

<<<<<<< HEAD
	if (dev->ieee80211_ptr->iftype != NL80211_IFTYPE_STATION &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_ADHOC &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_P2P_CLIENT &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_AP &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_AP_VLAN &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_MESH_POINT &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_P2P_GO)
		return -EOPNOTSUPP;

	if (info->attrs[NL80211_ATTR_DURATION]) {
<<<<<<< HEAD
		if (!rdev->ops->mgmt_tx_cancel_wait)
=======
		if (!(rdev->wiphy.flags & WIPHY_FLAG_OFFCHAN_TX))
>>>>>>> refs/remotes/origin/cm-10.0
			return -EINVAL;
		wait = nla_get_u32(info->attrs[NL80211_ATTR_DURATION]);
	}

	if (info->attrs[NL80211_ATTR_WIPHY_CHANNEL_TYPE]) {
		channel_type = nla_get_u32(
			info->attrs[NL80211_ATTR_WIPHY_CHANNEL_TYPE]);
		if (channel_type != NL80211_CHAN_NO_HT &&
		    channel_type != NL80211_CHAN_HT20 &&
		    channel_type != NL80211_CHAN_HT40PLUS &&
		    channel_type != NL80211_CHAN_HT40MINUS)
			return -EINVAL;
		channel_type_valid = true;
=======
	switch (wdev->iftype) {
	case NL80211_IFTYPE_P2P_DEVICE:
		if (!info->attrs[NL80211_ATTR_WIPHY_FREQ])
			return -EINVAL;
	case NL80211_IFTYPE_STATION:
	case NL80211_IFTYPE_ADHOC:
	case NL80211_IFTYPE_P2P_CLIENT:
	case NL80211_IFTYPE_AP:
	case NL80211_IFTYPE_AP_VLAN:
	case NL80211_IFTYPE_MESH_POINT:
	case NL80211_IFTYPE_P2P_GO:
		break;
	default:
		return -EOPNOTSUPP;
	}

	if (info->attrs[NL80211_ATTR_DURATION]) {
		if (!(rdev->wiphy.flags & WIPHY_FLAG_OFFCHAN_TX))
			return -EINVAL;
		wait = nla_get_u32(info->attrs[NL80211_ATTR_DURATION]);

		/*
		 * We should wait on the channel for at least a minimum amount
		 * of time (10ms) but no longer than the driver supports.
		 */
		if (wait < NL80211_MIN_REMAIN_ON_CHANNEL_TIME ||
		    wait > rdev->wiphy.max_remain_on_channel_duration)
			return -EINVAL;

>>>>>>> refs/remotes/origin/master
	}

	offchan = info->attrs[NL80211_ATTR_OFFCHANNEL_TX_OK];

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	if (offchan && !(rdev->wiphy.flags & WIPHY_FLAG_OFFCHAN_TX))
		return -EINVAL;

	no_cck = nla_get_flag(info->attrs[NL80211_ATTR_TX_NO_CCK_RATE]);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	freq = nla_get_u32(info->attrs[NL80211_ATTR_WIPHY_FREQ]);
	chan = rdev_freq_to_chan(rdev, freq, channel_type);
	if (chan == NULL)
		return -EINVAL;

<<<<<<< HEAD
	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (!msg)
		return -ENOMEM;

	hdr = nl80211hdr_put(msg, info->snd_pid, info->snd_seq, 0,
			     NL80211_CMD_FRAME);

	if (IS_ERR(hdr)) {
		err = PTR_ERR(hdr);
		goto free_msg;
	}
=======
=======
	/* get the channel if any has been specified, otherwise pass NULL to
	 * the driver. The latter will use the current one
	 */
	chandef.chan = NULL;
	if (info->attrs[NL80211_ATTR_WIPHY_FREQ]) {
		err = nl80211_parse_chandef(rdev, info, &chandef);
		if (err)
			return err;
	}

	if (!chandef.chan && offchan)
		return -EINVAL;

>>>>>>> refs/remotes/origin/master
	if (!dont_wait_for_ack) {
		msg = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
		if (!msg)
			return -ENOMEM;

<<<<<<< HEAD
		hdr = nl80211hdr_put(msg, info->snd_pid, info->snd_seq, 0,
				     NL80211_CMD_FRAME);

		if (IS_ERR(hdr)) {
			err = PTR_ERR(hdr);
=======
		hdr = nl80211hdr_put(msg, info->snd_portid, info->snd_seq, 0,
				     NL80211_CMD_FRAME);
		if (!hdr) {
			err = -ENOBUFS;
>>>>>>> refs/remotes/origin/master
			goto free_msg;
		}
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	err = cfg80211_mlme_mgmt_tx(rdev, dev, chan, offchan, channel_type,
				    channel_type_valid, wait,
				    nla_data(info->attrs[NL80211_ATTR_FRAME]),
				    nla_len(info->attrs[NL80211_ATTR_FRAME]),
<<<<<<< HEAD
				    &cookie);
	if (err)
		goto free_msg;

	NLA_PUT_U64(msg, NL80211_ATTR_COOKIE, cookie);

	genlmsg_end(msg, hdr);
	return genlmsg_reply(msg, info);
=======
=======
	err = cfg80211_mlme_mgmt_tx(rdev, wdev, chandef.chan, offchan, wait,
				    nla_data(info->attrs[NL80211_ATTR_FRAME]),
				    nla_len(info->attrs[NL80211_ATTR_FRAME]),
>>>>>>> refs/remotes/origin/master
				    no_cck, dont_wait_for_ack, &cookie);
	if (err)
		goto free_msg;

	if (msg) {
<<<<<<< HEAD
		NLA_PUT_U64(msg, NL80211_ATTR_COOKIE, cookie);
=======
		if (nla_put_u64(msg, NL80211_ATTR_COOKIE, cookie))
			goto nla_put_failure;
>>>>>>> refs/remotes/origin/master

		genlmsg_end(msg, hdr);
		return genlmsg_reply(msg, info);
	}

	return 0;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

 nla_put_failure:
	err = -ENOBUFS;
 free_msg:
	nlmsg_free(msg);
	return err;
}

static int nl80211_tx_mgmt_cancel_wait(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
<<<<<<< HEAD
	struct net_device *dev = info->user_ptr[1];
=======
	struct wireless_dev *wdev = info->user_ptr[1];
>>>>>>> refs/remotes/origin/master
	u64 cookie;

	if (!info->attrs[NL80211_ATTR_COOKIE])
		return -EINVAL;

	if (!rdev->ops->mgmt_tx_cancel_wait)
		return -EOPNOTSUPP;

<<<<<<< HEAD
	if (dev->ieee80211_ptr->iftype != NL80211_IFTYPE_STATION &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_ADHOC &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_P2P_CLIENT &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_AP &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_AP_VLAN &&
	    dev->ieee80211_ptr->iftype != NL80211_IFTYPE_P2P_GO)
		return -EOPNOTSUPP;

	cookie = nla_get_u64(info->attrs[NL80211_ATTR_COOKIE]);

	return rdev->ops->mgmt_tx_cancel_wait(&rdev->wiphy, dev, cookie);
=======
	switch (wdev->iftype) {
	case NL80211_IFTYPE_STATION:
	case NL80211_IFTYPE_ADHOC:
	case NL80211_IFTYPE_P2P_CLIENT:
	case NL80211_IFTYPE_AP:
	case NL80211_IFTYPE_AP_VLAN:
	case NL80211_IFTYPE_P2P_GO:
	case NL80211_IFTYPE_P2P_DEVICE:
		break;
	default:
		return -EOPNOTSUPP;
	}

	cookie = nla_get_u64(info->attrs[NL80211_ATTR_COOKIE]);

	return rdev_mgmt_tx_cancel_wait(rdev, wdev, cookie);
>>>>>>> refs/remotes/origin/master
}

static int nl80211_set_power_save(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct wireless_dev *wdev;
	struct net_device *dev = info->user_ptr[1];
	u8 ps_state;
	bool state;
	int err;

	if (!info->attrs[NL80211_ATTR_PS_STATE])
		return -EINVAL;

	ps_state = nla_get_u32(info->attrs[NL80211_ATTR_PS_STATE]);

	if (ps_state != NL80211_PS_DISABLED && ps_state != NL80211_PS_ENABLED)
		return -EINVAL;

	wdev = dev->ieee80211_ptr;

	if (!rdev->ops->set_power_mgmt)
		return -EOPNOTSUPP;

	state = (ps_state == NL80211_PS_ENABLED) ? true : false;

	if (state == wdev->ps)
		return 0;

<<<<<<< HEAD
	err = rdev->ops->set_power_mgmt(wdev->wiphy, dev, state,
					wdev->ps_timeout);
=======
	err = rdev_set_power_mgmt(rdev, dev, state, wdev->ps_timeout);
>>>>>>> refs/remotes/origin/master
	if (!err)
		wdev->ps = state;
	return err;
}

static int nl80211_get_power_save(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	enum nl80211_ps_state ps_state;
	struct wireless_dev *wdev;
	struct net_device *dev = info->user_ptr[1];
	struct sk_buff *msg;
	void *hdr;
	int err;

	wdev = dev->ieee80211_ptr;

	if (!rdev->ops->set_power_mgmt)
		return -EOPNOTSUPP;

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (!msg)
		return -ENOMEM;

<<<<<<< HEAD
	hdr = nl80211hdr_put(msg, info->snd_pid, info->snd_seq, 0,
=======
	hdr = nl80211hdr_put(msg, info->snd_portid, info->snd_seq, 0,
>>>>>>> refs/remotes/origin/master
			     NL80211_CMD_GET_POWER_SAVE);
	if (!hdr) {
		err = -ENOBUFS;
		goto free_msg;
	}

	if (wdev->ps)
		ps_state = NL80211_PS_ENABLED;
	else
		ps_state = NL80211_PS_DISABLED;

<<<<<<< HEAD
	NLA_PUT_U32(msg, NL80211_ATTR_PS_STATE, ps_state);
=======
	if (nla_put_u32(msg, NL80211_ATTR_PS_STATE, ps_state))
		goto nla_put_failure;
>>>>>>> refs/remotes/origin/master

	genlmsg_end(msg, hdr);
	return genlmsg_reply(msg, info);

 nla_put_failure:
	err = -ENOBUFS;
 free_msg:
	nlmsg_free(msg);
	return err;
}

static struct nla_policy
nl80211_attr_cqm_policy[NL80211_ATTR_CQM_MAX + 1] __read_mostly = {
	[NL80211_ATTR_CQM_RSSI_THOLD] = { .type = NLA_U32 },
	[NL80211_ATTR_CQM_RSSI_HYST] = { .type = NLA_U32 },
	[NL80211_ATTR_CQM_RSSI_THRESHOLD_EVENT] = { .type = NLA_U32 },
<<<<<<< HEAD
};

=======
	[NL80211_ATTR_CQM_TXE_RATE] = { .type = NLA_U32 },
	[NL80211_ATTR_CQM_TXE_PKTS] = { .type = NLA_U32 },
	[NL80211_ATTR_CQM_TXE_INTVL] = { .type = NLA_U32 },
};

static int nl80211_set_cqm_txe(struct genl_info *info,
			       u32 rate, u32 pkts, u32 intvl)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];
	struct wireless_dev *wdev = dev->ieee80211_ptr;

	if (rate > 100 || intvl > NL80211_CQM_TXE_MAX_INTVL)
		return -EINVAL;

	if (!rdev->ops->set_cqm_txe_config)
		return -EOPNOTSUPP;

	if (wdev->iftype != NL80211_IFTYPE_STATION &&
	    wdev->iftype != NL80211_IFTYPE_P2P_CLIENT)
		return -EOPNOTSUPP;

	return rdev_set_cqm_txe_config(rdev, dev, rate, pkts, intvl);
}

>>>>>>> refs/remotes/origin/master
static int nl80211_set_cqm_rssi(struct genl_info *info,
				s32 threshold, u32 hysteresis)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
<<<<<<< HEAD
	struct wireless_dev *wdev;
	struct net_device *dev = info->user_ptr[1];
=======
	struct net_device *dev = info->user_ptr[1];
	struct wireless_dev *wdev = dev->ieee80211_ptr;
>>>>>>> refs/remotes/origin/master

	if (threshold > 0)
		return -EINVAL;

<<<<<<< HEAD
	wdev = dev->ieee80211_ptr;
=======
	/* disabling - hysteresis should also be zero then */
	if (threshold == 0)
		hysteresis = 0;
>>>>>>> refs/remotes/origin/master

	if (!rdev->ops->set_cqm_rssi_config)
		return -EOPNOTSUPP;

	if (wdev->iftype != NL80211_IFTYPE_STATION &&
	    wdev->iftype != NL80211_IFTYPE_P2P_CLIENT)
		return -EOPNOTSUPP;

<<<<<<< HEAD
	return rdev->ops->set_cqm_rssi_config(wdev->wiphy, dev,
					      threshold, hysteresis);
=======
	return rdev_set_cqm_rssi_config(rdev, dev, threshold, hysteresis);
>>>>>>> refs/remotes/origin/master
}

static int nl80211_set_cqm(struct sk_buff *skb, struct genl_info *info)
{
	struct nlattr *attrs[NL80211_ATTR_CQM_MAX + 1];
	struct nlattr *cqm;
	int err;

	cqm = info->attrs[NL80211_ATTR_CQM];
<<<<<<< HEAD
	if (!cqm) {
		err = -EINVAL;
		goto out;
	}
=======
	if (!cqm)
		return -EINVAL;
>>>>>>> refs/remotes/origin/master

	err = nla_parse_nested(attrs, NL80211_ATTR_CQM_MAX, cqm,
			       nl80211_attr_cqm_policy);
	if (err)
<<<<<<< HEAD
		goto out;

	if (attrs[NL80211_ATTR_CQM_RSSI_THOLD] &&
	    attrs[NL80211_ATTR_CQM_RSSI_HYST]) {
		s32 threshold;
		u32 hysteresis;
		threshold = nla_get_u32(attrs[NL80211_ATTR_CQM_RSSI_THOLD]);
		hysteresis = nla_get_u32(attrs[NL80211_ATTR_CQM_RSSI_HYST]);
		err = nl80211_set_cqm_rssi(info, threshold, hysteresis);
	} else
		err = -EINVAL;

out:
	return err;
=======
		return err;

	if (attrs[NL80211_ATTR_CQM_RSSI_THOLD] &&
	    attrs[NL80211_ATTR_CQM_RSSI_HYST]) {
		s32 threshold = nla_get_s32(attrs[NL80211_ATTR_CQM_RSSI_THOLD]);
		u32 hysteresis = nla_get_u32(attrs[NL80211_ATTR_CQM_RSSI_HYST]);

		return nl80211_set_cqm_rssi(info, threshold, hysteresis);
	}

	if (attrs[NL80211_ATTR_CQM_TXE_RATE] &&
	    attrs[NL80211_ATTR_CQM_TXE_PKTS] &&
	    attrs[NL80211_ATTR_CQM_TXE_INTVL]) {
		u32 rate = nla_get_u32(attrs[NL80211_ATTR_CQM_TXE_RATE]);
		u32 pkts = nla_get_u32(attrs[NL80211_ATTR_CQM_TXE_PKTS]);
		u32 intvl = nla_get_u32(attrs[NL80211_ATTR_CQM_TXE_INTVL]);

		return nl80211_set_cqm_txe(info, rate, pkts, intvl);
	}

	return -EINVAL;
>>>>>>> refs/remotes/origin/master
}

static int nl80211_join_mesh(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];
	struct mesh_config cfg;
	struct mesh_setup setup;
	int err;

	/* start with default */
	memcpy(&cfg, &default_mesh_config, sizeof(cfg));
	memcpy(&setup, &default_mesh_setup, sizeof(setup));

	if (info->attrs[NL80211_ATTR_MESH_CONFIG]) {
		/* and parse parameters if given */
		err = nl80211_parse_mesh_config(info, &cfg, NULL);
		if (err)
			return err;
	}

	if (!info->attrs[NL80211_ATTR_MESH_ID] ||
	    !nla_len(info->attrs[NL80211_ATTR_MESH_ID]))
		return -EINVAL;

	setup.mesh_id = nla_data(info->attrs[NL80211_ATTR_MESH_ID]);
	setup.mesh_id_len = nla_len(info->attrs[NL80211_ATTR_MESH_ID]);

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	if (info->attrs[NL80211_ATTR_MCAST_RATE] &&
	    !nl80211_parse_mcast_rate(rdev, setup.mcast_rate,
			    nla_get_u32(info->attrs[NL80211_ATTR_MCAST_RATE])))
			return -EINVAL;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (info->attrs[NL80211_ATTR_BEACON_INTERVAL]) {
		setup.beacon_interval =
			nla_get_u32(info->attrs[NL80211_ATTR_BEACON_INTERVAL]);
		if (setup.beacon_interval < 10 ||
		    setup.beacon_interval > 10000)
			return -EINVAL;
	}

	if (info->attrs[NL80211_ATTR_DTIM_PERIOD]) {
		setup.dtim_period =
			nla_get_u32(info->attrs[NL80211_ATTR_DTIM_PERIOD]);
		if (setup.dtim_period < 1 || setup.dtim_period > 100)
			return -EINVAL;
	}

>>>>>>> refs/remotes/origin/master
	if (info->attrs[NL80211_ATTR_MESH_SETUP]) {
		/* parse additional setup parameters if given */
		err = nl80211_parse_mesh_setup(info, &setup);
		if (err)
			return err;
	}

<<<<<<< HEAD
=======
	if (setup.user_mpm)
		cfg.auto_open_plinks = false;

	if (info->attrs[NL80211_ATTR_WIPHY_FREQ]) {
		err = nl80211_parse_chandef(rdev, info, &setup.chandef);
		if (err)
			return err;
	} else {
		/* cfg80211_join_mesh() will sort it out */
		setup.chandef.chan = NULL;
	}

	if (info->attrs[NL80211_ATTR_BSS_BASIC_RATES]) {
		u8 *rates = nla_data(info->attrs[NL80211_ATTR_BSS_BASIC_RATES]);
		int n_rates =
			nla_len(info->attrs[NL80211_ATTR_BSS_BASIC_RATES]);
		struct ieee80211_supported_band *sband;

		if (!setup.chandef.chan)
			return -EINVAL;

		sband = rdev->wiphy.bands[setup.chandef.chan->band];

		err = ieee80211_get_ratemask(sband, rates, n_rates,
					     &setup.basic_rates);
		if (err)
			return err;
	}

>>>>>>> refs/remotes/origin/master
	return cfg80211_join_mesh(rdev, dev, &setup, &cfg);
}

static int nl80211_leave_mesh(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];

	return cfg80211_leave_mesh(rdev, dev);
}

<<<<<<< HEAD
=======
#ifdef CONFIG_PM
static int nl80211_send_wowlan_patterns(struct sk_buff *msg,
					struct cfg80211_registered_device *rdev)
{
	struct cfg80211_wowlan *wowlan = rdev->wiphy.wowlan_config;
	struct nlattr *nl_pats, *nl_pat;
	int i, pat_len;

	if (!wowlan->n_patterns)
		return 0;

	nl_pats = nla_nest_start(msg, NL80211_WOWLAN_TRIG_PKT_PATTERN);
	if (!nl_pats)
		return -ENOBUFS;

	for (i = 0; i < wowlan->n_patterns; i++) {
		nl_pat = nla_nest_start(msg, i + 1);
		if (!nl_pat)
			return -ENOBUFS;
		pat_len = wowlan->patterns[i].pattern_len;
		if (nla_put(msg, NL80211_PKTPAT_MASK, DIV_ROUND_UP(pat_len, 8),
			    wowlan->patterns[i].mask) ||
		    nla_put(msg, NL80211_PKTPAT_PATTERN, pat_len,
			    wowlan->patterns[i].pattern) ||
		    nla_put_u32(msg, NL80211_PKTPAT_OFFSET,
				wowlan->patterns[i].pkt_offset))
			return -ENOBUFS;
		nla_nest_end(msg, nl_pat);
	}
	nla_nest_end(msg, nl_pats);

	return 0;
}

static int nl80211_send_wowlan_tcp(struct sk_buff *msg,
				   struct cfg80211_wowlan_tcp *tcp)
{
	struct nlattr *nl_tcp;

	if (!tcp)
		return 0;

	nl_tcp = nla_nest_start(msg, NL80211_WOWLAN_TRIG_TCP_CONNECTION);
	if (!nl_tcp)
		return -ENOBUFS;

	if (nla_put_be32(msg, NL80211_WOWLAN_TCP_SRC_IPV4, tcp->src) ||
	    nla_put_be32(msg, NL80211_WOWLAN_TCP_DST_IPV4, tcp->dst) ||
	    nla_put(msg, NL80211_WOWLAN_TCP_DST_MAC, ETH_ALEN, tcp->dst_mac) ||
	    nla_put_u16(msg, NL80211_WOWLAN_TCP_SRC_PORT, tcp->src_port) ||
	    nla_put_u16(msg, NL80211_WOWLAN_TCP_DST_PORT, tcp->dst_port) ||
	    nla_put(msg, NL80211_WOWLAN_TCP_DATA_PAYLOAD,
		    tcp->payload_len, tcp->payload) ||
	    nla_put_u32(msg, NL80211_WOWLAN_TCP_DATA_INTERVAL,
			tcp->data_interval) ||
	    nla_put(msg, NL80211_WOWLAN_TCP_WAKE_PAYLOAD,
		    tcp->wake_len, tcp->wake_data) ||
	    nla_put(msg, NL80211_WOWLAN_TCP_WAKE_MASK,
		    DIV_ROUND_UP(tcp->wake_len, 8), tcp->wake_mask))
		return -ENOBUFS;

	if (tcp->payload_seq.len &&
	    nla_put(msg, NL80211_WOWLAN_TCP_DATA_PAYLOAD_SEQ,
		    sizeof(tcp->payload_seq), &tcp->payload_seq))
		return -ENOBUFS;

	if (tcp->payload_tok.len &&
	    nla_put(msg, NL80211_WOWLAN_TCP_DATA_PAYLOAD_TOKEN,
		    sizeof(tcp->payload_tok) + tcp->tokens_size,
		    &tcp->payload_tok))
		return -ENOBUFS;

	nla_nest_end(msg, nl_tcp);

	return 0;
}

>>>>>>> refs/remotes/origin/master
static int nl80211_get_wowlan(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct sk_buff *msg;
	void *hdr;
<<<<<<< HEAD

	if (!rdev->wiphy.wowlan.flags && !rdev->wiphy.wowlan.n_patterns)
		return -EOPNOTSUPP;

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (!msg)
		return -ENOMEM;

	hdr = nl80211hdr_put(msg, info->snd_pid, info->snd_seq, 0,
=======
	u32 size = NLMSG_DEFAULT_SIZE;

	if (!rdev->wiphy.wowlan)
		return -EOPNOTSUPP;

	if (rdev->wiphy.wowlan_config && rdev->wiphy.wowlan_config->tcp) {
		/* adjust size to have room for all the data */
		size += rdev->wiphy.wowlan_config->tcp->tokens_size +
			rdev->wiphy.wowlan_config->tcp->payload_len +
			rdev->wiphy.wowlan_config->tcp->wake_len +
			rdev->wiphy.wowlan_config->tcp->wake_len / 8;
	}

	msg = nlmsg_new(size, GFP_KERNEL);
	if (!msg)
		return -ENOMEM;

	hdr = nl80211hdr_put(msg, info->snd_portid, info->snd_seq, 0,
>>>>>>> refs/remotes/origin/master
			     NL80211_CMD_GET_WOWLAN);
	if (!hdr)
		goto nla_put_failure;

<<<<<<< HEAD
	if (rdev->wowlan) {
=======
	if (rdev->wiphy.wowlan_config) {
>>>>>>> refs/remotes/origin/master
		struct nlattr *nl_wowlan;

		nl_wowlan = nla_nest_start(msg, NL80211_ATTR_WOWLAN_TRIGGERS);
		if (!nl_wowlan)
			goto nla_put_failure;

<<<<<<< HEAD
		if (rdev->wowlan->any)
			NLA_PUT_FLAG(msg, NL80211_WOWLAN_TRIG_ANY);
		if (rdev->wowlan->disconnect)
			NLA_PUT_FLAG(msg, NL80211_WOWLAN_TRIG_DISCONNECT);
		if (rdev->wowlan->magic_pkt)
			NLA_PUT_FLAG(msg, NL80211_WOWLAN_TRIG_MAGIC_PKT);
<<<<<<< HEAD
=======
		if (rdev->wowlan->gtk_rekey_failure)
			NLA_PUT_FLAG(msg, NL80211_WOWLAN_TRIG_GTK_REKEY_FAILURE);
		if (rdev->wowlan->eap_identity_req)
			NLA_PUT_FLAG(msg, NL80211_WOWLAN_TRIG_EAP_IDENT_REQUEST);
		if (rdev->wowlan->four_way_handshake)
			NLA_PUT_FLAG(msg, NL80211_WOWLAN_TRIG_4WAY_HANDSHAKE);
		if (rdev->wowlan->rfkill_release)
			NLA_PUT_FLAG(msg, NL80211_WOWLAN_TRIG_RFKILL_RELEASE);
>>>>>>> refs/remotes/origin/cm-10.0
		if (rdev->wowlan->n_patterns) {
			struct nlattr *nl_pats, *nl_pat;
			int i, pat_len;

			nl_pats = nla_nest_start(msg,
					NL80211_WOWLAN_TRIG_PKT_PATTERN);
			if (!nl_pats)
				goto nla_put_failure;

			for (i = 0; i < rdev->wowlan->n_patterns; i++) {
				nl_pat = nla_nest_start(msg, i + 1);
				if (!nl_pat)
					goto nla_put_failure;
				pat_len = rdev->wowlan->patterns[i].pattern_len;
				NLA_PUT(msg, NL80211_WOWLAN_PKTPAT_MASK,
					DIV_ROUND_UP(pat_len, 8),
					rdev->wowlan->patterns[i].mask);
				NLA_PUT(msg, NL80211_WOWLAN_PKTPAT_PATTERN,
					pat_len,
					rdev->wowlan->patterns[i].pattern);
				nla_nest_end(msg, nl_pat);
			}
			nla_nest_end(msg, nl_pats);
		}
=======
		if ((rdev->wiphy.wowlan_config->any &&
		     nla_put_flag(msg, NL80211_WOWLAN_TRIG_ANY)) ||
		    (rdev->wiphy.wowlan_config->disconnect &&
		     nla_put_flag(msg, NL80211_WOWLAN_TRIG_DISCONNECT)) ||
		    (rdev->wiphy.wowlan_config->magic_pkt &&
		     nla_put_flag(msg, NL80211_WOWLAN_TRIG_MAGIC_PKT)) ||
		    (rdev->wiphy.wowlan_config->gtk_rekey_failure &&
		     nla_put_flag(msg, NL80211_WOWLAN_TRIG_GTK_REKEY_FAILURE)) ||
		    (rdev->wiphy.wowlan_config->eap_identity_req &&
		     nla_put_flag(msg, NL80211_WOWLAN_TRIG_EAP_IDENT_REQUEST)) ||
		    (rdev->wiphy.wowlan_config->four_way_handshake &&
		     nla_put_flag(msg, NL80211_WOWLAN_TRIG_4WAY_HANDSHAKE)) ||
		    (rdev->wiphy.wowlan_config->rfkill_release &&
		     nla_put_flag(msg, NL80211_WOWLAN_TRIG_RFKILL_RELEASE)))
			goto nla_put_failure;

		if (nl80211_send_wowlan_patterns(msg, rdev))
			goto nla_put_failure;

		if (nl80211_send_wowlan_tcp(msg,
					    rdev->wiphy.wowlan_config->tcp))
			goto nla_put_failure;
>>>>>>> refs/remotes/origin/master

		nla_nest_end(msg, nl_wowlan);
	}

	genlmsg_end(msg, hdr);
	return genlmsg_reply(msg, info);

nla_put_failure:
	nlmsg_free(msg);
	return -ENOBUFS;
}

<<<<<<< HEAD
=======
static int nl80211_parse_wowlan_tcp(struct cfg80211_registered_device *rdev,
				    struct nlattr *attr,
				    struct cfg80211_wowlan *trig)
{
	struct nlattr *tb[NUM_NL80211_WOWLAN_TCP];
	struct cfg80211_wowlan_tcp *cfg;
	struct nl80211_wowlan_tcp_data_token *tok = NULL;
	struct nl80211_wowlan_tcp_data_seq *seq = NULL;
	u32 size;
	u32 data_size, wake_size, tokens_size = 0, wake_mask_size;
	int err, port;

	if (!rdev->wiphy.wowlan->tcp)
		return -EINVAL;

	err = nla_parse(tb, MAX_NL80211_WOWLAN_TCP,
			nla_data(attr), nla_len(attr),
			nl80211_wowlan_tcp_policy);
	if (err)
		return err;

	if (!tb[NL80211_WOWLAN_TCP_SRC_IPV4] ||
	    !tb[NL80211_WOWLAN_TCP_DST_IPV4] ||
	    !tb[NL80211_WOWLAN_TCP_DST_MAC] ||
	    !tb[NL80211_WOWLAN_TCP_DST_PORT] ||
	    !tb[NL80211_WOWLAN_TCP_DATA_PAYLOAD] ||
	    !tb[NL80211_WOWLAN_TCP_DATA_INTERVAL] ||
	    !tb[NL80211_WOWLAN_TCP_WAKE_PAYLOAD] ||
	    !tb[NL80211_WOWLAN_TCP_WAKE_MASK])
		return -EINVAL;

	data_size = nla_len(tb[NL80211_WOWLAN_TCP_DATA_PAYLOAD]);
	if (data_size > rdev->wiphy.wowlan->tcp->data_payload_max)
		return -EINVAL;

	if (nla_get_u32(tb[NL80211_WOWLAN_TCP_DATA_INTERVAL]) >
			rdev->wiphy.wowlan->tcp->data_interval_max ||
	    nla_get_u32(tb[NL80211_WOWLAN_TCP_DATA_INTERVAL]) == 0)
		return -EINVAL;

	wake_size = nla_len(tb[NL80211_WOWLAN_TCP_WAKE_PAYLOAD]);
	if (wake_size > rdev->wiphy.wowlan->tcp->wake_payload_max)
		return -EINVAL;

	wake_mask_size = nla_len(tb[NL80211_WOWLAN_TCP_WAKE_MASK]);
	if (wake_mask_size != DIV_ROUND_UP(wake_size, 8))
		return -EINVAL;

	if (tb[NL80211_WOWLAN_TCP_DATA_PAYLOAD_TOKEN]) {
		u32 tokln = nla_len(tb[NL80211_WOWLAN_TCP_DATA_PAYLOAD_TOKEN]);

		tok = nla_data(tb[NL80211_WOWLAN_TCP_DATA_PAYLOAD_TOKEN]);
		tokens_size = tokln - sizeof(*tok);

		if (!tok->len || tokens_size % tok->len)
			return -EINVAL;
		if (!rdev->wiphy.wowlan->tcp->tok)
			return -EINVAL;
		if (tok->len > rdev->wiphy.wowlan->tcp->tok->max_len)
			return -EINVAL;
		if (tok->len < rdev->wiphy.wowlan->tcp->tok->min_len)
			return -EINVAL;
		if (tokens_size > rdev->wiphy.wowlan->tcp->tok->bufsize)
			return -EINVAL;
		if (tok->offset + tok->len > data_size)
			return -EINVAL;
	}

	if (tb[NL80211_WOWLAN_TCP_DATA_PAYLOAD_SEQ]) {
		seq = nla_data(tb[NL80211_WOWLAN_TCP_DATA_PAYLOAD_SEQ]);
		if (!rdev->wiphy.wowlan->tcp->seq)
			return -EINVAL;
		if (seq->len == 0 || seq->len > 4)
			return -EINVAL;
		if (seq->len + seq->offset > data_size)
			return -EINVAL;
	}

	size = sizeof(*cfg);
	size += data_size;
	size += wake_size + wake_mask_size;
	size += tokens_size;

	cfg = kzalloc(size, GFP_KERNEL);
	if (!cfg)
		return -ENOMEM;
	cfg->src = nla_get_be32(tb[NL80211_WOWLAN_TCP_SRC_IPV4]);
	cfg->dst = nla_get_be32(tb[NL80211_WOWLAN_TCP_DST_IPV4]);
	memcpy(cfg->dst_mac, nla_data(tb[NL80211_WOWLAN_TCP_DST_MAC]),
	       ETH_ALEN);
	if (tb[NL80211_WOWLAN_TCP_SRC_PORT])
		port = nla_get_u16(tb[NL80211_WOWLAN_TCP_SRC_PORT]);
	else
		port = 0;
#ifdef CONFIG_INET
	/* allocate a socket and port for it and use it */
	err = __sock_create(wiphy_net(&rdev->wiphy), PF_INET, SOCK_STREAM,
			    IPPROTO_TCP, &cfg->sock, 1);
	if (err) {
		kfree(cfg);
		return err;
	}
	if (inet_csk_get_port(cfg->sock->sk, port)) {
		sock_release(cfg->sock);
		kfree(cfg);
		return -EADDRINUSE;
	}
	cfg->src_port = inet_sk(cfg->sock->sk)->inet_num;
#else
	if (!port) {
		kfree(cfg);
		return -EINVAL;
	}
	cfg->src_port = port;
#endif

	cfg->dst_port = nla_get_u16(tb[NL80211_WOWLAN_TCP_DST_PORT]);
	cfg->payload_len = data_size;
	cfg->payload = (u8 *)cfg + sizeof(*cfg) + tokens_size;
	memcpy((void *)cfg->payload,
	       nla_data(tb[NL80211_WOWLAN_TCP_DATA_PAYLOAD]),
	       data_size);
	if (seq)
		cfg->payload_seq = *seq;
	cfg->data_interval = nla_get_u32(tb[NL80211_WOWLAN_TCP_DATA_INTERVAL]);
	cfg->wake_len = wake_size;
	cfg->wake_data = (u8 *)cfg + sizeof(*cfg) + tokens_size + data_size;
	memcpy((void *)cfg->wake_data,
	       nla_data(tb[NL80211_WOWLAN_TCP_WAKE_PAYLOAD]),
	       wake_size);
	cfg->wake_mask = (u8 *)cfg + sizeof(*cfg) + tokens_size +
			 data_size + wake_size;
	memcpy((void *)cfg->wake_mask,
	       nla_data(tb[NL80211_WOWLAN_TCP_WAKE_MASK]),
	       wake_mask_size);
	if (tok) {
		cfg->tokens_size = tokens_size;
		memcpy(&cfg->payload_tok, tok, sizeof(*tok) + tokens_size);
	}

	trig->tcp = cfg;

	return 0;
}

>>>>>>> refs/remotes/origin/master
static int nl80211_set_wowlan(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct nlattr *tb[NUM_NL80211_WOWLAN_TRIG];
<<<<<<< HEAD
	struct cfg80211_wowlan no_triggers = {};
	struct cfg80211_wowlan new_triggers = {};
	struct wiphy_wowlan_support *wowlan = &rdev->wiphy.wowlan;
	int err, i;

	if (!rdev->wiphy.wowlan.flags && !rdev->wiphy.wowlan.n_patterns)
		return -EOPNOTSUPP;

	if (!info->attrs[NL80211_ATTR_WOWLAN_TRIGGERS])
		goto no_triggers;
=======
	struct cfg80211_wowlan new_triggers = {};
	struct cfg80211_wowlan *ntrig;
	const struct wiphy_wowlan_support *wowlan = rdev->wiphy.wowlan;
	int err, i;
	bool prev_enabled = rdev->wiphy.wowlan_config;

	if (!wowlan)
		return -EOPNOTSUPP;

	if (!info->attrs[NL80211_ATTR_WOWLAN_TRIGGERS]) {
		cfg80211_rdev_free_wowlan(rdev);
		rdev->wiphy.wowlan_config = NULL;
		goto set_wakeup;
	}
>>>>>>> refs/remotes/origin/master

	err = nla_parse(tb, MAX_NL80211_WOWLAN_TRIG,
			nla_data(info->attrs[NL80211_ATTR_WOWLAN_TRIGGERS]),
			nla_len(info->attrs[NL80211_ATTR_WOWLAN_TRIGGERS]),
			nl80211_wowlan_policy);
	if (err)
		return err;

	if (tb[NL80211_WOWLAN_TRIG_ANY]) {
		if (!(wowlan->flags & WIPHY_WOWLAN_ANY))
			return -EINVAL;
		new_triggers.any = true;
	}

	if (tb[NL80211_WOWLAN_TRIG_DISCONNECT]) {
		if (!(wowlan->flags & WIPHY_WOWLAN_DISCONNECT))
			return -EINVAL;
		new_triggers.disconnect = true;
	}

	if (tb[NL80211_WOWLAN_TRIG_MAGIC_PKT]) {
		if (!(wowlan->flags & WIPHY_WOWLAN_MAGIC_PKT))
			return -EINVAL;
		new_triggers.magic_pkt = true;
	}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	if (tb[NL80211_WOWLAN_TRIG_GTK_REKEY_SUPPORTED])
		return -EINVAL;

	if (tb[NL80211_WOWLAN_TRIG_GTK_REKEY_FAILURE]) {
		if (!(wowlan->flags & WIPHY_WOWLAN_GTK_REKEY_FAILURE))
			return -EINVAL;
		new_triggers.gtk_rekey_failure = true;
	}

	if (tb[NL80211_WOWLAN_TRIG_EAP_IDENT_REQUEST]) {
		if (!(wowlan->flags & WIPHY_WOWLAN_EAP_IDENTITY_REQ))
			return -EINVAL;
		new_triggers.eap_identity_req = true;
	}

	if (tb[NL80211_WOWLAN_TRIG_4WAY_HANDSHAKE]) {
		if (!(wowlan->flags & WIPHY_WOWLAN_4WAY_HANDSHAKE))
			return -EINVAL;
		new_triggers.four_way_handshake = true;
	}

	if (tb[NL80211_WOWLAN_TRIG_RFKILL_RELEASE]) {
		if (!(wowlan->flags & WIPHY_WOWLAN_RFKILL_RELEASE))
			return -EINVAL;
		new_triggers.rfkill_release = true;
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	if (tb[NL80211_WOWLAN_TRIG_PKT_PATTERN]) {
		struct nlattr *pat;
		int n_patterns = 0;
		int rem, pat_len, mask_len;
		struct nlattr *pat_tb[NUM_NL80211_WOWLAN_PKTPAT];
=======
	if (tb[NL80211_WOWLAN_TRIG_PKT_PATTERN]) {
		struct nlattr *pat;
		int n_patterns = 0;
		int rem, pat_len, mask_len, pkt_offset;
		struct nlattr *pat_tb[NUM_NL80211_PKTPAT];
>>>>>>> refs/remotes/origin/master

		nla_for_each_nested(pat, tb[NL80211_WOWLAN_TRIG_PKT_PATTERN],
				    rem)
			n_patterns++;
		if (n_patterns > wowlan->n_patterns)
			return -EINVAL;

		new_triggers.patterns = kcalloc(n_patterns,
						sizeof(new_triggers.patterns[0]),
						GFP_KERNEL);
		if (!new_triggers.patterns)
			return -ENOMEM;

		new_triggers.n_patterns = n_patterns;
		i = 0;

		nla_for_each_nested(pat, tb[NL80211_WOWLAN_TRIG_PKT_PATTERN],
				    rem) {
<<<<<<< HEAD
			nla_parse(pat_tb, MAX_NL80211_WOWLAN_PKTPAT,
				  nla_data(pat), nla_len(pat), NULL);
			err = -EINVAL;
			if (!pat_tb[NL80211_WOWLAN_PKTPAT_MASK] ||
			    !pat_tb[NL80211_WOWLAN_PKTPAT_PATTERN])
				goto error;
			pat_len = nla_len(pat_tb[NL80211_WOWLAN_PKTPAT_PATTERN]);
			mask_len = DIV_ROUND_UP(pat_len, 8);
			if (nla_len(pat_tb[NL80211_WOWLAN_PKTPAT_MASK]) !=
			    mask_len)
=======
			nla_parse(pat_tb, MAX_NL80211_PKTPAT, nla_data(pat),
				  nla_len(pat), NULL);
			err = -EINVAL;
			if (!pat_tb[NL80211_PKTPAT_MASK] ||
			    !pat_tb[NL80211_PKTPAT_PATTERN])
				goto error;
			pat_len = nla_len(pat_tb[NL80211_PKTPAT_PATTERN]);
			mask_len = DIV_ROUND_UP(pat_len, 8);
			if (nla_len(pat_tb[NL80211_PKTPAT_MASK]) != mask_len)
>>>>>>> refs/remotes/origin/master
				goto error;
			if (pat_len > wowlan->pattern_max_len ||
			    pat_len < wowlan->pattern_min_len)
				goto error;

<<<<<<< HEAD
=======
			if (!pat_tb[NL80211_PKTPAT_OFFSET])
				pkt_offset = 0;
			else
				pkt_offset = nla_get_u32(
					pat_tb[NL80211_PKTPAT_OFFSET]);
			if (pkt_offset > wowlan->max_pkt_offset)
				goto error;
			new_triggers.patterns[i].pkt_offset = pkt_offset;

>>>>>>> refs/remotes/origin/master
			new_triggers.patterns[i].mask =
				kmalloc(mask_len + pat_len, GFP_KERNEL);
			if (!new_triggers.patterns[i].mask) {
				err = -ENOMEM;
				goto error;
			}
			new_triggers.patterns[i].pattern =
				new_triggers.patterns[i].mask + mask_len;
			memcpy(new_triggers.patterns[i].mask,
<<<<<<< HEAD
			       nla_data(pat_tb[NL80211_WOWLAN_PKTPAT_MASK]),
			       mask_len);
			new_triggers.patterns[i].pattern_len = pat_len;
			memcpy(new_triggers.patterns[i].pattern,
			       nla_data(pat_tb[NL80211_WOWLAN_PKTPAT_PATTERN]),
=======
			       nla_data(pat_tb[NL80211_PKTPAT_MASK]),
			       mask_len);
			new_triggers.patterns[i].pattern_len = pat_len;
			memcpy(new_triggers.patterns[i].pattern,
			       nla_data(pat_tb[NL80211_PKTPAT_PATTERN]),
>>>>>>> refs/remotes/origin/master
			       pat_len);
			i++;
		}
	}

<<<<<<< HEAD
	if (memcmp(&new_triggers, &no_triggers, sizeof(new_triggers))) {
		struct cfg80211_wowlan *ntrig;
		ntrig = kmemdup(&new_triggers, sizeof(new_triggers),
				GFP_KERNEL);
		if (!ntrig) {
			err = -ENOMEM;
			goto error;
		}
		cfg80211_rdev_free_wowlan(rdev);
		rdev->wowlan = ntrig;
	} else {
 no_triggers:
		cfg80211_rdev_free_wowlan(rdev);
		rdev->wowlan = NULL;
	}

	return 0;
 error:
	for (i = 0; i < new_triggers.n_patterns; i++)
		kfree(new_triggers.patterns[i].mask);
	kfree(new_triggers.patterns);
	return err;
}

<<<<<<< HEAD
#define NL80211_FLAG_NEED_WIPHY		0x01
#define NL80211_FLAG_NEED_NETDEV	0x02
#define NL80211_FLAG_NEED_RTNL		0x04
#define NL80211_FLAG_CHECK_NETDEV_UP	0x08
#define NL80211_FLAG_NEED_NETDEV_UP	(NL80211_FLAG_NEED_NETDEV |\
					 NL80211_FLAG_CHECK_NETDEV_UP)

static int nl80211_pre_doit(struct genl_ops *ops, struct sk_buff *skb,
			    struct genl_info *info)
{
	struct cfg80211_registered_device *rdev;
	struct net_device *dev;
	int err;
=======
=======
	if (tb[NL80211_WOWLAN_TRIG_TCP_CONNECTION]) {
		err = nl80211_parse_wowlan_tcp(
			rdev, tb[NL80211_WOWLAN_TRIG_TCP_CONNECTION],
			&new_triggers);
		if (err)
			goto error;
	}

	ntrig = kmemdup(&new_triggers, sizeof(new_triggers), GFP_KERNEL);
	if (!ntrig) {
		err = -ENOMEM;
		goto error;
	}
	cfg80211_rdev_free_wowlan(rdev);
	rdev->wiphy.wowlan_config = ntrig;

 set_wakeup:
	if (rdev->ops->set_wakeup &&
	    prev_enabled != !!rdev->wiphy.wowlan_config)
		rdev_set_wakeup(rdev, rdev->wiphy.wowlan_config);

	return 0;
 error:
	for (i = 0; i < new_triggers.n_patterns; i++)
		kfree(new_triggers.patterns[i].mask);
	kfree(new_triggers.patterns);
	if (new_triggers.tcp && new_triggers.tcp->sock)
		sock_release(new_triggers.tcp->sock);
	kfree(new_triggers.tcp);
	return err;
}
#endif

static int nl80211_send_coalesce_rules(struct sk_buff *msg,
				       struct cfg80211_registered_device *rdev)
{
	struct nlattr *nl_pats, *nl_pat, *nl_rule, *nl_rules;
	int i, j, pat_len;
	struct cfg80211_coalesce_rules *rule;

	if (!rdev->coalesce->n_rules)
		return 0;

	nl_rules = nla_nest_start(msg, NL80211_ATTR_COALESCE_RULE);
	if (!nl_rules)
		return -ENOBUFS;

	for (i = 0; i < rdev->coalesce->n_rules; i++) {
		nl_rule = nla_nest_start(msg, i + 1);
		if (!nl_rule)
			return -ENOBUFS;

		rule = &rdev->coalesce->rules[i];
		if (nla_put_u32(msg, NL80211_ATTR_COALESCE_RULE_DELAY,
				rule->delay))
			return -ENOBUFS;

		if (nla_put_u32(msg, NL80211_ATTR_COALESCE_RULE_CONDITION,
				rule->condition))
			return -ENOBUFS;

		nl_pats = nla_nest_start(msg,
				NL80211_ATTR_COALESCE_RULE_PKT_PATTERN);
		if (!nl_pats)
			return -ENOBUFS;

		for (j = 0; j < rule->n_patterns; j++) {
			nl_pat = nla_nest_start(msg, j + 1);
			if (!nl_pat)
				return -ENOBUFS;
			pat_len = rule->patterns[j].pattern_len;
			if (nla_put(msg, NL80211_PKTPAT_MASK,
				    DIV_ROUND_UP(pat_len, 8),
				    rule->patterns[j].mask) ||
			    nla_put(msg, NL80211_PKTPAT_PATTERN, pat_len,
				    rule->patterns[j].pattern) ||
			    nla_put_u32(msg, NL80211_PKTPAT_OFFSET,
					rule->patterns[j].pkt_offset))
				return -ENOBUFS;
			nla_nest_end(msg, nl_pat);
		}
		nla_nest_end(msg, nl_pats);
		nla_nest_end(msg, nl_rule);
	}
	nla_nest_end(msg, nl_rules);

	return 0;
}

static int nl80211_get_coalesce(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct sk_buff *msg;
	void *hdr;

	if (!rdev->wiphy.coalesce)
		return -EOPNOTSUPP;

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (!msg)
		return -ENOMEM;

	hdr = nl80211hdr_put(msg, info->snd_portid, info->snd_seq, 0,
			     NL80211_CMD_GET_COALESCE);
	if (!hdr)
		goto nla_put_failure;

	if (rdev->coalesce && nl80211_send_coalesce_rules(msg, rdev))
		goto nla_put_failure;

	genlmsg_end(msg, hdr);
	return genlmsg_reply(msg, info);

nla_put_failure:
	nlmsg_free(msg);
	return -ENOBUFS;
}

void cfg80211_rdev_free_coalesce(struct cfg80211_registered_device *rdev)
{
	struct cfg80211_coalesce *coalesce = rdev->coalesce;
	int i, j;
	struct cfg80211_coalesce_rules *rule;

	if (!coalesce)
		return;

	for (i = 0; i < coalesce->n_rules; i++) {
		rule = &coalesce->rules[i];
		for (j = 0; j < rule->n_patterns; j++)
			kfree(rule->patterns[j].mask);
		kfree(rule->patterns);
	}
	kfree(coalesce->rules);
	kfree(coalesce);
	rdev->coalesce = NULL;
}

static int nl80211_parse_coalesce_rule(struct cfg80211_registered_device *rdev,
				       struct nlattr *rule,
				       struct cfg80211_coalesce_rules *new_rule)
{
	int err, i;
	const struct wiphy_coalesce_support *coalesce = rdev->wiphy.coalesce;
	struct nlattr *tb[NUM_NL80211_ATTR_COALESCE_RULE], *pat;
	int rem, pat_len, mask_len, pkt_offset, n_patterns = 0;
	struct nlattr *pat_tb[NUM_NL80211_PKTPAT];

	err = nla_parse(tb, NL80211_ATTR_COALESCE_RULE_MAX, nla_data(rule),
			nla_len(rule), nl80211_coalesce_policy);
	if (err)
		return err;

	if (tb[NL80211_ATTR_COALESCE_RULE_DELAY])
		new_rule->delay =
			nla_get_u32(tb[NL80211_ATTR_COALESCE_RULE_DELAY]);
	if (new_rule->delay > coalesce->max_delay)
		return -EINVAL;

	if (tb[NL80211_ATTR_COALESCE_RULE_CONDITION])
		new_rule->condition =
			nla_get_u32(tb[NL80211_ATTR_COALESCE_RULE_CONDITION]);
	if (new_rule->condition != NL80211_COALESCE_CONDITION_MATCH &&
	    new_rule->condition != NL80211_COALESCE_CONDITION_NO_MATCH)
		return -EINVAL;

	if (!tb[NL80211_ATTR_COALESCE_RULE_PKT_PATTERN])
		return -EINVAL;

	nla_for_each_nested(pat, tb[NL80211_ATTR_COALESCE_RULE_PKT_PATTERN],
			    rem)
		n_patterns++;
	if (n_patterns > coalesce->n_patterns)
		return -EINVAL;

	new_rule->patterns = kcalloc(n_patterns, sizeof(new_rule->patterns[0]),
				     GFP_KERNEL);
	if (!new_rule->patterns)
		return -ENOMEM;

	new_rule->n_patterns = n_patterns;
	i = 0;

	nla_for_each_nested(pat, tb[NL80211_ATTR_COALESCE_RULE_PKT_PATTERN],
			    rem) {
		nla_parse(pat_tb, MAX_NL80211_PKTPAT, nla_data(pat),
			  nla_len(pat), NULL);
		if (!pat_tb[NL80211_PKTPAT_MASK] ||
		    !pat_tb[NL80211_PKTPAT_PATTERN])
			return -EINVAL;
		pat_len = nla_len(pat_tb[NL80211_PKTPAT_PATTERN]);
		mask_len = DIV_ROUND_UP(pat_len, 8);
		if (nla_len(pat_tb[NL80211_PKTPAT_MASK]) != mask_len)
			return -EINVAL;
		if (pat_len > coalesce->pattern_max_len ||
		    pat_len < coalesce->pattern_min_len)
			return -EINVAL;

		if (!pat_tb[NL80211_PKTPAT_OFFSET])
			pkt_offset = 0;
		else
			pkt_offset = nla_get_u32(pat_tb[NL80211_PKTPAT_OFFSET]);
		if (pkt_offset > coalesce->max_pkt_offset)
			return -EINVAL;
		new_rule->patterns[i].pkt_offset = pkt_offset;

		new_rule->patterns[i].mask =
			kmalloc(mask_len + pat_len, GFP_KERNEL);
		if (!new_rule->patterns[i].mask)
			return -ENOMEM;
		new_rule->patterns[i].pattern =
			new_rule->patterns[i].mask + mask_len;
		memcpy(new_rule->patterns[i].mask,
		       nla_data(pat_tb[NL80211_PKTPAT_MASK]), mask_len);
		new_rule->patterns[i].pattern_len = pat_len;
		memcpy(new_rule->patterns[i].pattern,
		       nla_data(pat_tb[NL80211_PKTPAT_PATTERN]), pat_len);
		i++;
	}

	return 0;
}

static int nl80211_set_coalesce(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	const struct wiphy_coalesce_support *coalesce = rdev->wiphy.coalesce;
	struct cfg80211_coalesce new_coalesce = {};
	struct cfg80211_coalesce *n_coalesce;
	int err, rem_rule, n_rules = 0, i, j;
	struct nlattr *rule;
	struct cfg80211_coalesce_rules *tmp_rule;

	if (!rdev->wiphy.coalesce || !rdev->ops->set_coalesce)
		return -EOPNOTSUPP;

	if (!info->attrs[NL80211_ATTR_COALESCE_RULE]) {
		cfg80211_rdev_free_coalesce(rdev);
		rdev->ops->set_coalesce(&rdev->wiphy, NULL);
		return 0;
	}

	nla_for_each_nested(rule, info->attrs[NL80211_ATTR_COALESCE_RULE],
			    rem_rule)
		n_rules++;
	if (n_rules > coalesce->n_rules)
		return -EINVAL;

	new_coalesce.rules = kcalloc(n_rules, sizeof(new_coalesce.rules[0]),
				     GFP_KERNEL);
	if (!new_coalesce.rules)
		return -ENOMEM;

	new_coalesce.n_rules = n_rules;
	i = 0;

	nla_for_each_nested(rule, info->attrs[NL80211_ATTR_COALESCE_RULE],
			    rem_rule) {
		err = nl80211_parse_coalesce_rule(rdev, rule,
						  &new_coalesce.rules[i]);
		if (err)
			goto error;

		i++;
	}

	err = rdev->ops->set_coalesce(&rdev->wiphy, &new_coalesce);
	if (err)
		goto error;

	n_coalesce = kmemdup(&new_coalesce, sizeof(new_coalesce), GFP_KERNEL);
	if (!n_coalesce) {
		err = -ENOMEM;
		goto error;
	}
	cfg80211_rdev_free_coalesce(rdev);
	rdev->coalesce = n_coalesce;

	return 0;
error:
	for (i = 0; i < new_coalesce.n_rules; i++) {
		tmp_rule = &new_coalesce.rules[i];
		for (j = 0; j < tmp_rule->n_patterns; j++)
			kfree(tmp_rule->patterns[j].mask);
		kfree(tmp_rule->patterns);
	}
	kfree(new_coalesce.rules);

	return err;
}

>>>>>>> refs/remotes/origin/master
static int nl80211_set_rekey_data(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct nlattr *tb[NUM_NL80211_REKEY_DATA];
	struct cfg80211_gtk_rekey_data rekey_data;
	int err;

	if (!info->attrs[NL80211_ATTR_REKEY_DATA])
		return -EINVAL;

	err = nla_parse(tb, MAX_NL80211_REKEY_DATA,
			nla_data(info->attrs[NL80211_ATTR_REKEY_DATA]),
			nla_len(info->attrs[NL80211_ATTR_REKEY_DATA]),
			nl80211_rekey_policy);
	if (err)
		return err;

	if (nla_len(tb[NL80211_REKEY_DATA_REPLAY_CTR]) != NL80211_REPLAY_CTR_LEN)
		return -ERANGE;
	if (nla_len(tb[NL80211_REKEY_DATA_KEK]) != NL80211_KEK_LEN)
		return -ERANGE;
	if (nla_len(tb[NL80211_REKEY_DATA_KCK]) != NL80211_KCK_LEN)
		return -ERANGE;

	memcpy(rekey_data.kek, nla_data(tb[NL80211_REKEY_DATA_KEK]),
	       NL80211_KEK_LEN);
	memcpy(rekey_data.kck, nla_data(tb[NL80211_REKEY_DATA_KCK]),
	       NL80211_KCK_LEN);
	memcpy(rekey_data.replay_ctr,
	       nla_data(tb[NL80211_REKEY_DATA_REPLAY_CTR]),
	       NL80211_REPLAY_CTR_LEN);

	wdev_lock(wdev);
	if (!wdev->current_bss) {
		err = -ENOTCONN;
		goto out;
	}

	if (!rdev->ops->set_rekey_data) {
		err = -EOPNOTSUPP;
		goto out;
	}

<<<<<<< HEAD
	err = rdev->ops->set_rekey_data(&rdev->wiphy, dev, &rekey_data);
=======
	err = rdev_set_rekey_data(rdev, dev, &rekey_data);
>>>>>>> refs/remotes/origin/master
 out:
	wdev_unlock(wdev);
	return err;
}

static int nl80211_register_unexpected_frame(struct sk_buff *skb,
					     struct genl_info *info)
{
	struct net_device *dev = info->user_ptr[1];
	struct wireless_dev *wdev = dev->ieee80211_ptr;

	if (wdev->iftype != NL80211_IFTYPE_AP &&
	    wdev->iftype != NL80211_IFTYPE_P2P_GO)
		return -EINVAL;

<<<<<<< HEAD
	if (wdev->ap_unexpected_nlpid)
		return -EBUSY;

	wdev->ap_unexpected_nlpid = info->snd_pid;
=======
	if (wdev->ap_unexpected_nlportid)
		return -EBUSY;

	wdev->ap_unexpected_nlportid = info->snd_portid;
>>>>>>> refs/remotes/origin/master
	return 0;
}

static int nl80211_probe_client(struct sk_buff *skb,
				struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct net_device *dev = info->user_ptr[1];
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct sk_buff *msg;
	void *hdr;
	const u8 *addr;
	u64 cookie;
	int err;

	if (wdev->iftype != NL80211_IFTYPE_AP &&
	    wdev->iftype != NL80211_IFTYPE_P2P_GO)
		return -EOPNOTSUPP;

	if (!info->attrs[NL80211_ATTR_MAC])
		return -EINVAL;

	if (!rdev->ops->probe_client)
		return -EOPNOTSUPP;

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (!msg)
		return -ENOMEM;

<<<<<<< HEAD
	hdr = nl80211hdr_put(msg, info->snd_pid, info->snd_seq, 0,
			     NL80211_CMD_PROBE_CLIENT);

	if (IS_ERR(hdr)) {
		err = PTR_ERR(hdr);
=======
	hdr = nl80211hdr_put(msg, info->snd_portid, info->snd_seq, 0,
			     NL80211_CMD_PROBE_CLIENT);
	if (!hdr) {
		err = -ENOBUFS;
>>>>>>> refs/remotes/origin/master
		goto free_msg;
	}

	addr = nla_data(info->attrs[NL80211_ATTR_MAC]);

<<<<<<< HEAD
	err = rdev->ops->probe_client(&rdev->wiphy, dev, addr, &cookie);
	if (err)
		goto free_msg;

	NLA_PUT_U64(msg, NL80211_ATTR_COOKIE, cookie);
=======
	err = rdev_probe_client(rdev, dev, addr, &cookie);
	if (err)
		goto free_msg;

	if (nla_put_u64(msg, NL80211_ATTR_COOKIE, cookie))
		goto nla_put_failure;
>>>>>>> refs/remotes/origin/master

	genlmsg_end(msg, hdr);

	return genlmsg_reply(msg, info);

 nla_put_failure:
	err = -ENOBUFS;
 free_msg:
	nlmsg_free(msg);
	return err;
}

static int nl80211_register_beacons(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
<<<<<<< HEAD
=======
	struct cfg80211_beacon_registration *reg, *nreg;
	int rv;
>>>>>>> refs/remotes/origin/master

	if (!(rdev->wiphy.flags & WIPHY_FLAG_REPORTS_OBSS))
		return -EOPNOTSUPP;

<<<<<<< HEAD
	if (rdev->ap_beacons_nlpid)
		return -EBUSY;

	rdev->ap_beacons_nlpid = info->snd_pid;

=======
	nreg = kzalloc(sizeof(*nreg), GFP_KERNEL);
	if (!nreg)
		return -ENOMEM;

	/* First, check if already registered. */
	spin_lock_bh(&rdev->beacon_registrations_lock);
	list_for_each_entry(reg, &rdev->beacon_registrations, list) {
		if (reg->nlportid == info->snd_portid) {
			rv = -EALREADY;
			goto out_err;
		}
	}
	/* Add it to the list */
	nreg->nlportid = info->snd_portid;
	list_add(&nreg->list, &rdev->beacon_registrations);

	spin_unlock_bh(&rdev->beacon_registrations_lock);

	return 0;
out_err:
	spin_unlock_bh(&rdev->beacon_registrations_lock);
	kfree(nreg);
	return rv;
}

static int nl80211_start_p2p_device(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct wireless_dev *wdev = info->user_ptr[1];
	int err;

	if (!rdev->ops->start_p2p_device)
		return -EOPNOTSUPP;

	if (wdev->iftype != NL80211_IFTYPE_P2P_DEVICE)
		return -EOPNOTSUPP;

	if (wdev->p2p_started)
		return 0;

	err = cfg80211_can_add_interface(rdev, wdev->iftype);
	if (err)
		return err;

	err = rdev_start_p2p_device(rdev, wdev);
	if (err)
		return err;

	wdev->p2p_started = true;
	rdev->opencount++;

	return 0;
}

static int nl80211_stop_p2p_device(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct wireless_dev *wdev = info->user_ptr[1];

	if (wdev->iftype != NL80211_IFTYPE_P2P_DEVICE)
		return -EOPNOTSUPP;

	if (!rdev->ops->stop_p2p_device)
		return -EOPNOTSUPP;

	cfg80211_stop_p2p_device(rdev, wdev);

	return 0;
}

static int nl80211_get_protocol_features(struct sk_buff *skb,
					 struct genl_info *info)
{
	void *hdr;
	struct sk_buff *msg;

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (!msg)
		return -ENOMEM;

	hdr = nl80211hdr_put(msg, info->snd_portid, info->snd_seq, 0,
			     NL80211_CMD_GET_PROTOCOL_FEATURES);
	if (!hdr)
		goto nla_put_failure;

	if (nla_put_u32(msg, NL80211_ATTR_PROTOCOL_FEATURES,
			NL80211_PROTOCOL_FEATURE_SPLIT_WIPHY_DUMP))
		goto nla_put_failure;

	genlmsg_end(msg, hdr);
	return genlmsg_reply(msg, info);

 nla_put_failure:
	kfree_skb(msg);
	return -ENOBUFS;
}

static int nl80211_update_ft_ies(struct sk_buff *skb, struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct cfg80211_update_ft_ies_params ft_params;
	struct net_device *dev = info->user_ptr[1];

	if (!rdev->ops->update_ft_ies)
		return -EOPNOTSUPP;

	if (!info->attrs[NL80211_ATTR_MDID] ||
	    !is_valid_ie_attr(info->attrs[NL80211_ATTR_IE]))
		return -EINVAL;

	memset(&ft_params, 0, sizeof(ft_params));
	ft_params.md = nla_get_u16(info->attrs[NL80211_ATTR_MDID]);
	ft_params.ie = nla_data(info->attrs[NL80211_ATTR_IE]);
	ft_params.ie_len = nla_len(info->attrs[NL80211_ATTR_IE]);

	return rdev_update_ft_ies(rdev, dev, &ft_params);
}

static int nl80211_crit_protocol_start(struct sk_buff *skb,
				       struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct wireless_dev *wdev = info->user_ptr[1];
	enum nl80211_crit_proto_id proto = NL80211_CRIT_PROTO_UNSPEC;
	u16 duration;
	int ret;

	if (!rdev->ops->crit_proto_start)
		return -EOPNOTSUPP;

	if (WARN_ON(!rdev->ops->crit_proto_stop))
		return -EINVAL;

	if (rdev->crit_proto_nlportid)
		return -EBUSY;

	/* determine protocol if provided */
	if (info->attrs[NL80211_ATTR_CRIT_PROT_ID])
		proto = nla_get_u16(info->attrs[NL80211_ATTR_CRIT_PROT_ID]);

	if (proto >= NUM_NL80211_CRIT_PROTO)
		return -EINVAL;

	/* timeout must be provided */
	if (!info->attrs[NL80211_ATTR_MAX_CRIT_PROT_DURATION])
		return -EINVAL;

	duration =
		nla_get_u16(info->attrs[NL80211_ATTR_MAX_CRIT_PROT_DURATION]);

	if (duration > NL80211_CRIT_PROTO_MAX_DURATION)
		return -ERANGE;

	ret = rdev_crit_proto_start(rdev, wdev, proto, duration);
	if (!ret)
		rdev->crit_proto_nlportid = info->snd_portid;

	return ret;
}

static int nl80211_crit_protocol_stop(struct sk_buff *skb,
				      struct genl_info *info)
{
	struct cfg80211_registered_device *rdev = info->user_ptr[0];
	struct wireless_dev *wdev = info->user_ptr[1];

	if (!rdev->ops->crit_proto_stop)
		return -EOPNOTSUPP;

	if (rdev->crit_proto_nlportid) {
		rdev->crit_proto_nlportid = 0;
		rdev_crit_proto_stop(rdev, wdev);
	}
>>>>>>> refs/remotes/origin/master
	return 0;
}

#define NL80211_FLAG_NEED_WIPHY		0x01
#define NL80211_FLAG_NEED_NETDEV	0x02
#define NL80211_FLAG_NEED_RTNL		0x04
#define NL80211_FLAG_CHECK_NETDEV_UP	0x08
#define NL80211_FLAG_NEED_NETDEV_UP	(NL80211_FLAG_NEED_NETDEV |\
					 NL80211_FLAG_CHECK_NETDEV_UP)
<<<<<<< HEAD

static int nl80211_pre_doit(struct genl_ops *ops, struct sk_buff *skb,
			    struct genl_info *info)
{
	struct cfg80211_registered_device *rdev;
	struct net_device *dev;
	int err;
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define NL80211_FLAG_NEED_WDEV		0x10
/* If a netdev is associated, it must be UP, P2P must be started */
#define NL80211_FLAG_NEED_WDEV_UP	(NL80211_FLAG_NEED_WDEV |\
					 NL80211_FLAG_CHECK_NETDEV_UP)

static int nl80211_pre_doit(const struct genl_ops *ops, struct sk_buff *skb,
			    struct genl_info *info)
{
	struct cfg80211_registered_device *rdev;
	struct wireless_dev *wdev;
	struct net_device *dev;
>>>>>>> refs/remotes/origin/master
	bool rtnl = ops->internal_flags & NL80211_FLAG_NEED_RTNL;

	if (rtnl)
		rtnl_lock();

	if (ops->internal_flags & NL80211_FLAG_NEED_WIPHY) {
<<<<<<< HEAD
		rdev = cfg80211_get_dev_from_info(info);
=======
		rdev = cfg80211_get_dev_from_info(genl_info_net(info), info);
>>>>>>> refs/remotes/origin/master
		if (IS_ERR(rdev)) {
			if (rtnl)
				rtnl_unlock();
			return PTR_ERR(rdev);
		}
		info->user_ptr[0] = rdev;
<<<<<<< HEAD
	} else if (ops->internal_flags & NL80211_FLAG_NEED_NETDEV) {
<<<<<<< HEAD
		err = get_rdev_dev_by_info_ifindex(info, &rdev, &dev);
=======
		err = get_rdev_dev_by_ifindex(genl_info_net(info), info->attrs,
					      &rdev, &dev);
>>>>>>> refs/remotes/origin/cm-10.0
		if (err) {
			if (rtnl)
				rtnl_unlock();
			return err;
		}
		if (ops->internal_flags & NL80211_FLAG_CHECK_NETDEV_UP &&
		    !netif_running(dev)) {
			cfg80211_unlock_rdev(rdev);
			dev_put(dev);
			if (rtnl)
				rtnl_unlock();
			return -ENETDOWN;
		}
		info->user_ptr[0] = rdev;
		info->user_ptr[1] = dev;
=======
	} else if (ops->internal_flags & NL80211_FLAG_NEED_NETDEV ||
		   ops->internal_flags & NL80211_FLAG_NEED_WDEV) {
		ASSERT_RTNL();

		wdev = __cfg80211_wdev_from_attrs(genl_info_net(info),
						  info->attrs);
		if (IS_ERR(wdev)) {
			if (rtnl)
				rtnl_unlock();
			return PTR_ERR(wdev);
		}

		dev = wdev->netdev;
		rdev = wiphy_to_dev(wdev->wiphy);

		if (ops->internal_flags & NL80211_FLAG_NEED_NETDEV) {
			if (!dev) {
				if (rtnl)
					rtnl_unlock();
				return -EINVAL;
			}

			info->user_ptr[1] = dev;
		} else {
			info->user_ptr[1] = wdev;
		}

		if (dev) {
			if (ops->internal_flags & NL80211_FLAG_CHECK_NETDEV_UP &&
			    !netif_running(dev)) {
				if (rtnl)
					rtnl_unlock();
				return -ENETDOWN;
			}

			dev_hold(dev);
		} else if (ops->internal_flags & NL80211_FLAG_CHECK_NETDEV_UP) {
			if (!wdev->p2p_started) {
				if (rtnl)
					rtnl_unlock();
				return -ENETDOWN;
			}
		}

		info->user_ptr[0] = rdev;
>>>>>>> refs/remotes/origin/master
	}

	return 0;
}

<<<<<<< HEAD
static void nl80211_post_doit(struct genl_ops *ops, struct sk_buff *skb,
			      struct genl_info *info)
{
	if (info->user_ptr[0])
		cfg80211_unlock_rdev(info->user_ptr[0]);
	if (info->user_ptr[1])
		dev_put(info->user_ptr[1]);
=======
static void nl80211_post_doit(const struct genl_ops *ops, struct sk_buff *skb,
			      struct genl_info *info)
{
	if (info->user_ptr[1]) {
		if (ops->internal_flags & NL80211_FLAG_NEED_WDEV) {
			struct wireless_dev *wdev = info->user_ptr[1];

			if (wdev->netdev)
				dev_put(wdev->netdev);
		} else {
			dev_put(info->user_ptr[1]);
		}
	}
>>>>>>> refs/remotes/origin/master
	if (ops->internal_flags & NL80211_FLAG_NEED_RTNL)
		rtnl_unlock();
}

<<<<<<< HEAD
static struct genl_ops nl80211_ops[] = {
=======
static const struct genl_ops nl80211_ops[] = {
>>>>>>> refs/remotes/origin/master
	{
		.cmd = NL80211_CMD_GET_WIPHY,
		.doit = nl80211_get_wiphy,
		.dumpit = nl80211_dump_wiphy,
<<<<<<< HEAD
		.policy = nl80211_policy,
		/* can be retrieved by unprivileged users */
		.internal_flags = NL80211_FLAG_NEED_WIPHY,
=======
		.done = nl80211_dump_wiphy_done,
		.policy = nl80211_policy,
		/* can be retrieved by unprivileged users */
		.internal_flags = NL80211_FLAG_NEED_WIPHY |
				  NL80211_FLAG_NEED_RTNL,
>>>>>>> refs/remotes/origin/master
	},
	{
		.cmd = NL80211_CMD_SET_WIPHY,
		.doit = nl80211_set_wiphy,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_GET_INTERFACE,
		.doit = nl80211_get_interface,
		.dumpit = nl80211_dump_interface,
		.policy = nl80211_policy,
		/* can be retrieved by unprivileged users */
<<<<<<< HEAD
		.internal_flags = NL80211_FLAG_NEED_NETDEV,
=======
		.internal_flags = NL80211_FLAG_NEED_WDEV |
				  NL80211_FLAG_NEED_RTNL,
>>>>>>> refs/remotes/origin/master
	},
	{
		.cmd = NL80211_CMD_SET_INTERFACE,
		.doit = nl80211_set_interface,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_NEW_INTERFACE,
		.doit = nl80211_new_interface,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_WIPHY |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_DEL_INTERFACE,
		.doit = nl80211_del_interface,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
<<<<<<< HEAD
		.internal_flags = NL80211_FLAG_NEED_NETDEV |
=======
		.internal_flags = NL80211_FLAG_NEED_WDEV |
>>>>>>> refs/remotes/origin/master
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_GET_KEY,
		.doit = nl80211_get_key,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_SET_KEY,
		.doit = nl80211_set_key,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_NEW_KEY,
		.doit = nl80211_new_key,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_DEL_KEY,
		.doit = nl80211_del_key,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_SET_BEACON,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
<<<<<<< HEAD
<<<<<<< HEAD
		.doit = nl80211_addset_beacon,
=======
		.doit = nl80211_set_beacon,
>>>>>>> refs/remotes/origin/cm-10.0
=======
		.doit = nl80211_set_beacon,
>>>>>>> refs/remotes/origin/master
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
<<<<<<< HEAD
<<<<<<< HEAD
		.cmd = NL80211_CMD_NEW_BEACON,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.doit = nl80211_addset_beacon,
=======
=======
>>>>>>> refs/remotes/origin/master
		.cmd = NL80211_CMD_START_AP,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.doit = nl80211_start_ap,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
<<<<<<< HEAD
<<<<<<< HEAD
		.cmd = NL80211_CMD_DEL_BEACON,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.doit = nl80211_del_beacon,
		.internal_flags = NL80211_FLAG_NEED_NETDEV |
=======
=======
>>>>>>> refs/remotes/origin/master
		.cmd = NL80211_CMD_STOP_AP,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.doit = nl80211_stop_ap,
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_GET_STATION,
		.doit = nl80211_get_station,
		.dumpit = nl80211_dump_station,
		.policy = nl80211_policy,
		.internal_flags = NL80211_FLAG_NEED_NETDEV |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_SET_STATION,
		.doit = nl80211_set_station,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_NEW_STATION,
		.doit = nl80211_new_station,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_DEL_STATION,
		.doit = nl80211_del_station,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_GET_MPATH,
		.doit = nl80211_get_mpath,
		.dumpit = nl80211_dump_mpath,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_SET_MPATH,
		.doit = nl80211_set_mpath,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_NEW_MPATH,
		.doit = nl80211_new_mpath,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_DEL_MPATH,
		.doit = nl80211_del_mpath,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_SET_BSS,
		.doit = nl80211_set_bss,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_GET_REG,
		.doit = nl80211_get_reg,
		.policy = nl80211_policy,
<<<<<<< HEAD
=======
		.internal_flags = NL80211_FLAG_NEED_RTNL,
>>>>>>> refs/remotes/origin/master
		/* can be retrieved by unprivileged users */
	},
	{
		.cmd = NL80211_CMD_SET_REG,
		.doit = nl80211_set_reg,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
<<<<<<< HEAD
=======
		.internal_flags = NL80211_FLAG_NEED_RTNL,
>>>>>>> refs/remotes/origin/master
	},
	{
		.cmd = NL80211_CMD_REQ_SET_REG,
		.doit = nl80211_req_set_reg,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
	},
	{
		.cmd = NL80211_CMD_GET_MESH_CONFIG,
		.doit = nl80211_get_mesh_config,
		.policy = nl80211_policy,
		/* can be retrieved by unprivileged users */
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_SET_MESH_CONFIG,
		.doit = nl80211_update_mesh_config,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_TRIGGER_SCAN,
		.doit = nl80211_trigger_scan,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
<<<<<<< HEAD
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
=======
		.internal_flags = NL80211_FLAG_NEED_WDEV_UP |
>>>>>>> refs/remotes/origin/master
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_GET_SCAN,
		.policy = nl80211_policy,
		.dumpit = nl80211_dump_scan,
	},
	{
		.cmd = NL80211_CMD_START_SCHED_SCAN,
		.doit = nl80211_start_sched_scan,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_STOP_SCHED_SCAN,
		.doit = nl80211_stop_sched_scan,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_AUTHENTICATE,
		.doit = nl80211_authenticate,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_ASSOCIATE,
		.doit = nl80211_associate,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_DEAUTHENTICATE,
		.doit = nl80211_deauthenticate,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_DISASSOCIATE,
		.doit = nl80211_disassociate,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_JOIN_IBSS,
		.doit = nl80211_join_ibss,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_LEAVE_IBSS,
		.doit = nl80211_leave_ibss,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
#ifdef CONFIG_NL80211_TESTMODE
	{
		.cmd = NL80211_CMD_TESTMODE,
		.doit = nl80211_testmode_do,
<<<<<<< HEAD
<<<<<<< HEAD
=======
		.dumpit = nl80211_testmode_dump,
>>>>>>> refs/remotes/origin/cm-10.0
=======
		.dumpit = nl80211_testmode_dump,
>>>>>>> refs/remotes/origin/master
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_WIPHY |
				  NL80211_FLAG_NEED_RTNL,
	},
#endif
	{
		.cmd = NL80211_CMD_CONNECT,
		.doit = nl80211_connect,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_DISCONNECT,
		.doit = nl80211_disconnect,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_SET_WIPHY_NETNS,
		.doit = nl80211_wiphy_netns,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_WIPHY |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_GET_SURVEY,
		.policy = nl80211_policy,
		.dumpit = nl80211_dump_survey,
	},
	{
		.cmd = NL80211_CMD_SET_PMKSA,
		.doit = nl80211_setdel_pmksa,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_DEL_PMKSA,
		.doit = nl80211_setdel_pmksa,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_FLUSH_PMKSA,
		.doit = nl80211_flush_pmksa,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_REMAIN_ON_CHANNEL,
		.doit = nl80211_remain_on_channel,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
<<<<<<< HEAD
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
=======
		.internal_flags = NL80211_FLAG_NEED_WDEV_UP |
>>>>>>> refs/remotes/origin/master
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_CANCEL_REMAIN_ON_CHANNEL,
		.doit = nl80211_cancel_remain_on_channel,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
<<<<<<< HEAD
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
=======
		.internal_flags = NL80211_FLAG_NEED_WDEV_UP |
>>>>>>> refs/remotes/origin/master
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_SET_TX_BITRATE_MASK,
		.doit = nl80211_set_tx_bitrate_mask,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_REGISTER_FRAME,
		.doit = nl80211_register_mgmt,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
<<<<<<< HEAD
		.internal_flags = NL80211_FLAG_NEED_NETDEV |
=======
		.internal_flags = NL80211_FLAG_NEED_WDEV |
>>>>>>> refs/remotes/origin/master
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_FRAME,
		.doit = nl80211_tx_mgmt,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
<<<<<<< HEAD
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
=======
		.internal_flags = NL80211_FLAG_NEED_WDEV_UP |
>>>>>>> refs/remotes/origin/master
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_FRAME_WAIT_CANCEL,
		.doit = nl80211_tx_mgmt_cancel_wait,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
<<<<<<< HEAD
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
=======
		.internal_flags = NL80211_FLAG_NEED_WDEV_UP |
>>>>>>> refs/remotes/origin/master
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_SET_POWER_SAVE,
		.doit = nl80211_set_power_save,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_GET_POWER_SAVE,
		.doit = nl80211_get_power_save,
		.policy = nl80211_policy,
		/* can be retrieved by unprivileged users */
		.internal_flags = NL80211_FLAG_NEED_NETDEV |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_SET_CQM,
		.doit = nl80211_set_cqm,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_SET_CHANNEL,
		.doit = nl80211_set_channel,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_SET_WDS_PEER,
		.doit = nl80211_set_wds_peer,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
<<<<<<< HEAD
<<<<<<< HEAD
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
=======
		.internal_flags = NL80211_FLAG_NEED_NETDEV |
>>>>>>> refs/remotes/origin/cm-10.0
=======
		.internal_flags = NL80211_FLAG_NEED_NETDEV |
>>>>>>> refs/remotes/origin/master
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_JOIN_MESH,
		.doit = nl80211_join_mesh,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_LEAVE_MESH,
		.doit = nl80211_leave_mesh,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
<<<<<<< HEAD
=======
#ifdef CONFIG_PM
>>>>>>> refs/remotes/origin/master
	{
		.cmd = NL80211_CMD_GET_WOWLAN,
		.doit = nl80211_get_wowlan,
		.policy = nl80211_policy,
		/* can be retrieved by unprivileged users */
		.internal_flags = NL80211_FLAG_NEED_WIPHY |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_SET_WOWLAN,
		.doit = nl80211_set_wowlan,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_WIPHY |
				  NL80211_FLAG_NEED_RTNL,
	},
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
#endif
>>>>>>> refs/remotes/origin/master
	{
		.cmd = NL80211_CMD_SET_REKEY_OFFLOAD,
		.doit = nl80211_set_rekey_data,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_TDLS_MGMT,
		.doit = nl80211_tdls_mgmt,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_TDLS_OPER,
		.doit = nl80211_tdls_oper,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_UNEXPECTED_FRAME,
		.doit = nl80211_register_unexpected_frame,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_PROBE_CLIENT,
		.doit = nl80211_probe_client,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
<<<<<<< HEAD
		.cmd = NL80211_CMD_REGISTER_BEACONS,
		.doit = nl80211_register_beacons,
=======
		.cmd = NL80211_CMD_REGISTER_BEACONS,
		.doit = nl80211_register_beacons,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_WIPHY |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_SET_NOACK_MAP,
		.doit = nl80211_set_noack_map,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_START_P2P_DEVICE,
		.doit = nl80211_start_p2p_device,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_WDEV |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_STOP_P2P_DEVICE,
		.doit = nl80211_stop_p2p_device,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_WDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_SET_MCAST_RATE,
		.doit = nl80211_set_mcast_rate,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_SET_MAC_ACL,
		.doit = nl80211_set_mac_acl,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_RADAR_DETECT,
		.doit = nl80211_start_radar_detection,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_GET_PROTOCOL_FEATURES,
		.doit = nl80211_get_protocol_features,
		.policy = nl80211_policy,
	},
	{
		.cmd = NL80211_CMD_UPDATE_FT_IES,
		.doit = nl80211_update_ft_ies,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_CRIT_PROTOCOL_START,
		.doit = nl80211_crit_protocol_start,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_WDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_CRIT_PROTOCOL_STOP,
		.doit = nl80211_crit_protocol_stop,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_WDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_GET_COALESCE,
		.doit = nl80211_get_coalesce,
		.policy = nl80211_policy,
		.internal_flags = NL80211_FLAG_NEED_WIPHY |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
		.cmd = NL80211_CMD_SET_COALESCE,
		.doit = nl80211_set_coalesce,
>>>>>>> refs/remotes/origin/master
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_WIPHY |
				  NL80211_FLAG_NEED_RTNL,
	},
	{
<<<<<<< HEAD
		.cmd = NL80211_CMD_SET_NOACK_MAP,
		.doit = nl80211_set_noack_map,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV |
				  NL80211_FLAG_NEED_RTNL,
	},

>>>>>>> refs/remotes/origin/cm-10.0
};

static struct genl_multicast_group nl80211_mlme_mcgrp = {
	.name = "mlme",
};

/* multicast groups */
static struct genl_multicast_group nl80211_config_mcgrp = {
	.name = "config",
};
static struct genl_multicast_group nl80211_scan_mcgrp = {
	.name = "scan",
};
static struct genl_multicast_group nl80211_regulatory_mcgrp = {
	.name = "regulatory",
=======
		.cmd = NL80211_CMD_CHANNEL_SWITCH,
		.doit = nl80211_channel_switch,
		.policy = nl80211_policy,
		.flags = GENL_ADMIN_PERM,
		.internal_flags = NL80211_FLAG_NEED_NETDEV_UP |
				  NL80211_FLAG_NEED_RTNL,
	},
>>>>>>> refs/remotes/origin/master
};

/* notification functions */

void nl80211_notify_dev_rename(struct cfg80211_registered_device *rdev)
{
	struct sk_buff *msg;
<<<<<<< HEAD
=======
	struct nl80211_dump_wiphy_state state = {};
>>>>>>> refs/remotes/origin/master

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (!msg)
		return;

<<<<<<< HEAD
	if (nl80211_send_wiphy(msg, 0, 0, 0, rdev) < 0) {
=======
	if (nl80211_send_wiphy(rdev, msg, 0, 0, 0, &state) < 0) {
>>>>>>> refs/remotes/origin/master
		nlmsg_free(msg);
		return;
	}

<<<<<<< HEAD
	genlmsg_multicast_netns(wiphy_net(&rdev->wiphy), msg, 0,
				nl80211_config_mcgrp.id, GFP_KERNEL);
=======
	genlmsg_multicast_netns(&nl80211_fam, wiphy_net(&rdev->wiphy), msg, 0,
				NL80211_MCGRP_CONFIG, GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
}

static int nl80211_add_scan_req(struct sk_buff *msg,
				struct cfg80211_registered_device *rdev)
{
	struct cfg80211_scan_request *req = rdev->scan_req;
	struct nlattr *nest;
	int i;

<<<<<<< HEAD
	ASSERT_RDEV_LOCK(rdev);

=======
>>>>>>> refs/remotes/origin/master
	if (WARN_ON(!req))
		return 0;

	nest = nla_nest_start(msg, NL80211_ATTR_SCAN_SSIDS);
	if (!nest)
		goto nla_put_failure;
<<<<<<< HEAD
	for (i = 0; i < req->n_ssids; i++)
		NLA_PUT(msg, i, req->ssids[i].ssid_len, req->ssids[i].ssid);
=======
	for (i = 0; i < req->n_ssids; i++) {
		if (nla_put(msg, i, req->ssids[i].ssid_len, req->ssids[i].ssid))
			goto nla_put_failure;
	}
>>>>>>> refs/remotes/origin/master
	nla_nest_end(msg, nest);

	nest = nla_nest_start(msg, NL80211_ATTR_SCAN_FREQUENCIES);
	if (!nest)
		goto nla_put_failure;
<<<<<<< HEAD
	for (i = 0; i < req->n_channels; i++)
		NLA_PUT_U32(msg, i, req->channels[i]->center_freq);
	nla_nest_end(msg, nest);

	if (req->ie)
		NLA_PUT(msg, NL80211_ATTR_IE, req->ie_len, req->ie);
=======
	for (i = 0; i < req->n_channels; i++) {
		if (nla_put_u32(msg, i, req->channels[i]->center_freq))
			goto nla_put_failure;
	}
	nla_nest_end(msg, nest);

	if (req->ie &&
	    nla_put(msg, NL80211_ATTR_IE, req->ie_len, req->ie))
		goto nla_put_failure;

	if (req->flags &&
	    nla_put_u32(msg, NL80211_ATTR_SCAN_FLAGS, req->flags))
		goto nla_put_failure;
>>>>>>> refs/remotes/origin/master

	return 0;
 nla_put_failure:
	return -ENOBUFS;
}

static int nl80211_send_scan_msg(struct sk_buff *msg,
				 struct cfg80211_registered_device *rdev,
<<<<<<< HEAD
				 struct net_device *netdev,
				 u32 pid, u32 seq, int flags,
=======
				 struct wireless_dev *wdev,
				 u32 portid, u32 seq, int flags,
>>>>>>> refs/remotes/origin/master
				 u32 cmd)
{
	void *hdr;

<<<<<<< HEAD
	hdr = nl80211hdr_put(msg, pid, seq, flags, cmd);
	if (!hdr)
		return -1;

	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx);
	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, netdev->ifindex);
=======
	hdr = nl80211hdr_put(msg, portid, seq, flags, cmd);
	if (!hdr)
		return -1;

	if (nla_put_u32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx) ||
	    (wdev->netdev && nla_put_u32(msg, NL80211_ATTR_IFINDEX,
					 wdev->netdev->ifindex)) ||
	    nla_put_u64(msg, NL80211_ATTR_WDEV, wdev_id(wdev)))
		goto nla_put_failure;
>>>>>>> refs/remotes/origin/master

	/* ignore errors and send incomplete event anyway */
	nl80211_add_scan_req(msg, rdev);

	return genlmsg_end(msg, hdr);

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	return -EMSGSIZE;
}

static int
nl80211_send_sched_scan_msg(struct sk_buff *msg,
			    struct cfg80211_registered_device *rdev,
			    struct net_device *netdev,
<<<<<<< HEAD
			    u32 pid, u32 seq, int flags, u32 cmd)
{
	void *hdr;

	hdr = nl80211hdr_put(msg, pid, seq, flags, cmd);
	if (!hdr)
		return -1;

	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx);
	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, netdev->ifindex);
=======
			    u32 portid, u32 seq, int flags, u32 cmd)
{
	void *hdr;

	hdr = nl80211hdr_put(msg, portid, seq, flags, cmd);
	if (!hdr)
		return -1;

	if (nla_put_u32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx) ||
	    nla_put_u32(msg, NL80211_ATTR_IFINDEX, netdev->ifindex))
		goto nla_put_failure;
>>>>>>> refs/remotes/origin/master

	return genlmsg_end(msg, hdr);

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	return -EMSGSIZE;
}

void nl80211_send_scan_start(struct cfg80211_registered_device *rdev,
<<<<<<< HEAD
			     struct net_device *netdev)
{
	struct sk_buff *msg;

	msg = nlmsg_new(NLMSG_GOODSIZE, GFP_KERNEL);
	if (!msg)
		return;

	if (nl80211_send_scan_msg(msg, rdev, netdev, 0, 0, 0,
=======
			     struct wireless_dev *wdev)
{
	struct sk_buff *msg;

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (!msg)
		return;

	if (nl80211_send_scan_msg(msg, rdev, wdev, 0, 0, 0,
>>>>>>> refs/remotes/origin/master
				  NL80211_CMD_TRIGGER_SCAN) < 0) {
		nlmsg_free(msg);
		return;
	}

<<<<<<< HEAD
	genlmsg_multicast_netns(wiphy_net(&rdev->wiphy), msg, 0,
				nl80211_scan_mcgrp.id, GFP_KERNEL);
}

void nl80211_send_scan_done(struct cfg80211_registered_device *rdev,
			    struct net_device *netdev)
=======
	genlmsg_multicast_netns(&nl80211_fam, wiphy_net(&rdev->wiphy), msg, 0,
				NL80211_MCGRP_SCAN, GFP_KERNEL);
}

void nl80211_send_scan_done(struct cfg80211_registered_device *rdev,
			    struct wireless_dev *wdev)
>>>>>>> refs/remotes/origin/master
{
	struct sk_buff *msg;

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (!msg)
		return;

<<<<<<< HEAD
	if (nl80211_send_scan_msg(msg, rdev, netdev, 0, 0, 0,
=======
	if (nl80211_send_scan_msg(msg, rdev, wdev, 0, 0, 0,
>>>>>>> refs/remotes/origin/master
				  NL80211_CMD_NEW_SCAN_RESULTS) < 0) {
		nlmsg_free(msg);
		return;
	}

<<<<<<< HEAD
	genlmsg_multicast_netns(wiphy_net(&rdev->wiphy), msg, 0,
				nl80211_scan_mcgrp.id, GFP_KERNEL);
}

void nl80211_send_scan_aborted(struct cfg80211_registered_device *rdev,
			       struct net_device *netdev)
=======
	genlmsg_multicast_netns(&nl80211_fam, wiphy_net(&rdev->wiphy), msg, 0,
				NL80211_MCGRP_SCAN, GFP_KERNEL);
}

void nl80211_send_scan_aborted(struct cfg80211_registered_device *rdev,
			       struct wireless_dev *wdev)
>>>>>>> refs/remotes/origin/master
{
	struct sk_buff *msg;

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (!msg)
		return;

<<<<<<< HEAD
	if (nl80211_send_scan_msg(msg, rdev, netdev, 0, 0, 0,
=======
	if (nl80211_send_scan_msg(msg, rdev, wdev, 0, 0, 0,
>>>>>>> refs/remotes/origin/master
				  NL80211_CMD_SCAN_ABORTED) < 0) {
		nlmsg_free(msg);
		return;
	}

<<<<<<< HEAD
	genlmsg_multicast_netns(wiphy_net(&rdev->wiphy), msg, 0,
				nl80211_scan_mcgrp.id, GFP_KERNEL);
=======
	genlmsg_multicast_netns(&nl80211_fam, wiphy_net(&rdev->wiphy), msg, 0,
				NL80211_MCGRP_SCAN, GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
}

void nl80211_send_sched_scan_results(struct cfg80211_registered_device *rdev,
				     struct net_device *netdev)
{
	struct sk_buff *msg;

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (!msg)
		return;

	if (nl80211_send_sched_scan_msg(msg, rdev, netdev, 0, 0, 0,
					NL80211_CMD_SCHED_SCAN_RESULTS) < 0) {
		nlmsg_free(msg);
		return;
	}

<<<<<<< HEAD
	genlmsg_multicast_netns(wiphy_net(&rdev->wiphy), msg, 0,
				nl80211_scan_mcgrp.id, GFP_KERNEL);
=======
	genlmsg_multicast_netns(&nl80211_fam, wiphy_net(&rdev->wiphy), msg, 0,
				NL80211_MCGRP_SCAN, GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
}

void nl80211_send_sched_scan(struct cfg80211_registered_device *rdev,
			     struct net_device *netdev, u32 cmd)
{
	struct sk_buff *msg;

<<<<<<< HEAD
	msg = nlmsg_new(NLMSG_GOODSIZE, GFP_KERNEL);
=======
	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (!msg)
		return;

	if (nl80211_send_sched_scan_msg(msg, rdev, netdev, 0, 0, 0, cmd) < 0) {
		nlmsg_free(msg);
		return;
	}

<<<<<<< HEAD
	genlmsg_multicast_netns(wiphy_net(&rdev->wiphy), msg, 0,
				nl80211_scan_mcgrp.id, GFP_KERNEL);
=======
	genlmsg_multicast_netns(&nl80211_fam, wiphy_net(&rdev->wiphy), msg, 0,
				NL80211_MCGRP_SCAN, GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
}

/*
 * This can happen on global regulatory changes or device specific settings
 * based on custom world regulatory domains.
 */
void nl80211_send_reg_change_event(struct regulatory_request *request)
{
	struct sk_buff *msg;
	void *hdr;

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (!msg)
		return;

	hdr = nl80211hdr_put(msg, 0, 0, 0, NL80211_CMD_REG_CHANGE);
	if (!hdr) {
		nlmsg_free(msg);
		return;
	}

	/* Userspace can always count this one always being set */
<<<<<<< HEAD
	NLA_PUT_U8(msg, NL80211_ATTR_REG_INITIATOR, request->initiator);

	if (request->alpha2[0] == '0' && request->alpha2[1] == '0')
		NLA_PUT_U8(msg, NL80211_ATTR_REG_TYPE,
			   NL80211_REGDOM_TYPE_WORLD);
	else if (request->alpha2[0] == '9' && request->alpha2[1] == '9')
		NLA_PUT_U8(msg, NL80211_ATTR_REG_TYPE,
			   NL80211_REGDOM_TYPE_CUSTOM_WORLD);
	else if ((request->alpha2[0] == '9' && request->alpha2[1] == '8') ||
		 request->intersect)
		NLA_PUT_U8(msg, NL80211_ATTR_REG_TYPE,
			   NL80211_REGDOM_TYPE_INTERSECTION);
	else {
		NLA_PUT_U8(msg, NL80211_ATTR_REG_TYPE,
			   NL80211_REGDOM_TYPE_COUNTRY);
		NLA_PUT_STRING(msg, NL80211_ATTR_REG_ALPHA2, request->alpha2);
	}

	if (wiphy_idx_valid(request->wiphy_idx))
		NLA_PUT_U32(msg, NL80211_ATTR_WIPHY, request->wiphy_idx);

<<<<<<< HEAD
	if (genlmsg_end(msg, hdr) < 0) {
		nlmsg_free(msg);
		return;
	}
=======
	genlmsg_end(msg, hdr);
>>>>>>> refs/remotes/origin/cm-10.0

	rcu_read_lock();
	genlmsg_multicast_allns(msg, 0, nl80211_regulatory_mcgrp.id,
				GFP_ATOMIC);
=======
	if (nla_put_u8(msg, NL80211_ATTR_REG_INITIATOR, request->initiator))
		goto nla_put_failure;

	if (request->alpha2[0] == '0' && request->alpha2[1] == '0') {
		if (nla_put_u8(msg, NL80211_ATTR_REG_TYPE,
			       NL80211_REGDOM_TYPE_WORLD))
			goto nla_put_failure;
	} else if (request->alpha2[0] == '9' && request->alpha2[1] == '9') {
		if (nla_put_u8(msg, NL80211_ATTR_REG_TYPE,
			       NL80211_REGDOM_TYPE_CUSTOM_WORLD))
			goto nla_put_failure;
	} else if ((request->alpha2[0] == '9' && request->alpha2[1] == '8') ||
		   request->intersect) {
		if (nla_put_u8(msg, NL80211_ATTR_REG_TYPE,
			       NL80211_REGDOM_TYPE_INTERSECTION))
			goto nla_put_failure;
	} else {
		if (nla_put_u8(msg, NL80211_ATTR_REG_TYPE,
			       NL80211_REGDOM_TYPE_COUNTRY) ||
		    nla_put_string(msg, NL80211_ATTR_REG_ALPHA2,
				   request->alpha2))
			goto nla_put_failure;
	}

	if (request->wiphy_idx != WIPHY_IDX_INVALID &&
	    nla_put_u32(msg, NL80211_ATTR_WIPHY, request->wiphy_idx))
		goto nla_put_failure;

	genlmsg_end(msg, hdr);

	rcu_read_lock();
	genlmsg_multicast_allns(&nl80211_fam, msg, 0,
				NL80211_MCGRP_REGULATORY, GFP_ATOMIC);
>>>>>>> refs/remotes/origin/master
	rcu_read_unlock();

	return;

nla_put_failure:
	genlmsg_cancel(msg, hdr);
	nlmsg_free(msg);
}

static void nl80211_send_mlme_event(struct cfg80211_registered_device *rdev,
				    struct net_device *netdev,
				    const u8 *buf, size_t len,
				    enum nl80211_commands cmd, gfp_t gfp)
{
	struct sk_buff *msg;
	void *hdr;

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, gfp);
	if (!msg)
		return;

	hdr = nl80211hdr_put(msg, 0, 0, 0, cmd);
	if (!hdr) {
		nlmsg_free(msg);
		return;
	}

<<<<<<< HEAD
	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx);
	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, netdev->ifindex);
	NLA_PUT(msg, NL80211_ATTR_FRAME, len, buf);

<<<<<<< HEAD
	if (genlmsg_end(msg, hdr) < 0) {
		nlmsg_free(msg);
		return;
	}
=======
	genlmsg_end(msg, hdr);
>>>>>>> refs/remotes/origin/cm-10.0

	genlmsg_multicast_netns(wiphy_net(&rdev->wiphy), msg, 0,
				nl80211_mlme_mcgrp.id, gfp);
=======
	if (nla_put_u32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx) ||
	    nla_put_u32(msg, NL80211_ATTR_IFINDEX, netdev->ifindex) ||
	    nla_put(msg, NL80211_ATTR_FRAME, len, buf))
		goto nla_put_failure;

	genlmsg_end(msg, hdr);

	genlmsg_multicast_netns(&nl80211_fam, wiphy_net(&rdev->wiphy), msg, 0,
				NL80211_MCGRP_MLME, gfp);
>>>>>>> refs/remotes/origin/master
	return;

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	nlmsg_free(msg);
}

void nl80211_send_rx_auth(struct cfg80211_registered_device *rdev,
			  struct net_device *netdev, const u8 *buf,
			  size_t len, gfp_t gfp)
{
	nl80211_send_mlme_event(rdev, netdev, buf, len,
				NL80211_CMD_AUTHENTICATE, gfp);
}

void nl80211_send_rx_assoc(struct cfg80211_registered_device *rdev,
			   struct net_device *netdev, const u8 *buf,
			   size_t len, gfp_t gfp)
{
	nl80211_send_mlme_event(rdev, netdev, buf, len,
				NL80211_CMD_ASSOCIATE, gfp);
}

void nl80211_send_deauth(struct cfg80211_registered_device *rdev,
			 struct net_device *netdev, const u8 *buf,
			 size_t len, gfp_t gfp)
{
	nl80211_send_mlme_event(rdev, netdev, buf, len,
				NL80211_CMD_DEAUTHENTICATE, gfp);
}

void nl80211_send_disassoc(struct cfg80211_registered_device *rdev,
			   struct net_device *netdev, const u8 *buf,
			   size_t len, gfp_t gfp)
{
	nl80211_send_mlme_event(rdev, netdev, buf, len,
				NL80211_CMD_DISASSOCIATE, gfp);
}

<<<<<<< HEAD
void nl80211_send_unprot_deauth(struct cfg80211_registered_device *rdev,
				struct net_device *netdev, const u8 *buf,
				size_t len, gfp_t gfp)
{
	nl80211_send_mlme_event(rdev, netdev, buf, len,
				NL80211_CMD_UNPROT_DEAUTHENTICATE, gfp);
}

void nl80211_send_unprot_disassoc(struct cfg80211_registered_device *rdev,
				  struct net_device *netdev, const u8 *buf,
				  size_t len, gfp_t gfp)
{
	nl80211_send_mlme_event(rdev, netdev, buf, len,
				NL80211_CMD_UNPROT_DISASSOCIATE, gfp);
}
=======
void cfg80211_rx_unprot_mlme_mgmt(struct net_device *dev, const u8 *buf,
				  size_t len)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);
	const struct ieee80211_mgmt *mgmt = (void *)buf;
	u32 cmd;

	if (WARN_ON(len < 2))
		return;

	if (ieee80211_is_deauth(mgmt->frame_control))
		cmd = NL80211_CMD_UNPROT_DEAUTHENTICATE;
	else
		cmd = NL80211_CMD_UNPROT_DISASSOCIATE;

	trace_cfg80211_rx_unprot_mlme_mgmt(dev, buf, len);
	nl80211_send_mlme_event(rdev, dev, buf, len, cmd, GFP_ATOMIC);
}
EXPORT_SYMBOL(cfg80211_rx_unprot_mlme_mgmt);
>>>>>>> refs/remotes/origin/master

static void nl80211_send_mlme_timeout(struct cfg80211_registered_device *rdev,
				      struct net_device *netdev, int cmd,
				      const u8 *addr, gfp_t gfp)
{
	struct sk_buff *msg;
	void *hdr;

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, gfp);
	if (!msg)
		return;

	hdr = nl80211hdr_put(msg, 0, 0, 0, cmd);
	if (!hdr) {
		nlmsg_free(msg);
		return;
	}

<<<<<<< HEAD
	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx);
	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, netdev->ifindex);
	NLA_PUT_FLAG(msg, NL80211_ATTR_TIMED_OUT);
	NLA_PUT(msg, NL80211_ATTR_MAC, ETH_ALEN, addr);

<<<<<<< HEAD
	if (genlmsg_end(msg, hdr) < 0) {
		nlmsg_free(msg);
		return;
	}
=======
	genlmsg_end(msg, hdr);
>>>>>>> refs/remotes/origin/cm-10.0

	genlmsg_multicast_netns(wiphy_net(&rdev->wiphy), msg, 0,
				nl80211_mlme_mcgrp.id, gfp);
=======
	if (nla_put_u32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx) ||
	    nla_put_u32(msg, NL80211_ATTR_IFINDEX, netdev->ifindex) ||
	    nla_put_flag(msg, NL80211_ATTR_TIMED_OUT) ||
	    nla_put(msg, NL80211_ATTR_MAC, ETH_ALEN, addr))
		goto nla_put_failure;

	genlmsg_end(msg, hdr);

	genlmsg_multicast_netns(&nl80211_fam, wiphy_net(&rdev->wiphy), msg, 0,
				NL80211_MCGRP_MLME, gfp);
>>>>>>> refs/remotes/origin/master
	return;

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	nlmsg_free(msg);
}

void nl80211_send_auth_timeout(struct cfg80211_registered_device *rdev,
			       struct net_device *netdev, const u8 *addr,
			       gfp_t gfp)
{
	nl80211_send_mlme_timeout(rdev, netdev, NL80211_CMD_AUTHENTICATE,
				  addr, gfp);
}

void nl80211_send_assoc_timeout(struct cfg80211_registered_device *rdev,
				struct net_device *netdev, const u8 *addr,
				gfp_t gfp)
{
	nl80211_send_mlme_timeout(rdev, netdev, NL80211_CMD_ASSOCIATE,
				  addr, gfp);
}

void nl80211_send_connect_result(struct cfg80211_registered_device *rdev,
				 struct net_device *netdev, const u8 *bssid,
				 const u8 *req_ie, size_t req_ie_len,
				 const u8 *resp_ie, size_t resp_ie_len,
				 u16 status, gfp_t gfp)
{
	struct sk_buff *msg;
	void *hdr;

<<<<<<< HEAD
	msg = nlmsg_new(NLMSG_GOODSIZE, gfp);
=======
	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, gfp);
>>>>>>> refs/remotes/origin/master
	if (!msg)
		return;

	hdr = nl80211hdr_put(msg, 0, 0, 0, NL80211_CMD_CONNECT);
	if (!hdr) {
		nlmsg_free(msg);
		return;
	}

<<<<<<< HEAD
	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx);
	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, netdev->ifindex);
	if (bssid)
		NLA_PUT(msg, NL80211_ATTR_MAC, ETH_ALEN, bssid);
	NLA_PUT_U16(msg, NL80211_ATTR_STATUS_CODE, status);
	if (req_ie)
		NLA_PUT(msg, NL80211_ATTR_REQ_IE, req_ie_len, req_ie);
	if (resp_ie)
		NLA_PUT(msg, NL80211_ATTR_RESP_IE, resp_ie_len, resp_ie);

<<<<<<< HEAD
	if (genlmsg_end(msg, hdr) < 0) {
		nlmsg_free(msg);
		return;
	}
=======
	genlmsg_end(msg, hdr);
>>>>>>> refs/remotes/origin/cm-10.0

	genlmsg_multicast_netns(wiphy_net(&rdev->wiphy), msg, 0,
				nl80211_mlme_mcgrp.id, gfp);
=======
	if (nla_put_u32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx) ||
	    nla_put_u32(msg, NL80211_ATTR_IFINDEX, netdev->ifindex) ||
	    (bssid && nla_put(msg, NL80211_ATTR_MAC, ETH_ALEN, bssid)) ||
	    nla_put_u16(msg, NL80211_ATTR_STATUS_CODE, status) ||
	    (req_ie &&
	     nla_put(msg, NL80211_ATTR_REQ_IE, req_ie_len, req_ie)) ||
	    (resp_ie &&
	     nla_put(msg, NL80211_ATTR_RESP_IE, resp_ie_len, resp_ie)))
		goto nla_put_failure;

	genlmsg_end(msg, hdr);

	genlmsg_multicast_netns(&nl80211_fam, wiphy_net(&rdev->wiphy), msg, 0,
				NL80211_MCGRP_MLME, gfp);
>>>>>>> refs/remotes/origin/master
	return;

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	nlmsg_free(msg);

}

void nl80211_send_roamed(struct cfg80211_registered_device *rdev,
			 struct net_device *netdev, const u8 *bssid,
			 const u8 *req_ie, size_t req_ie_len,
			 const u8 *resp_ie, size_t resp_ie_len, gfp_t gfp)
{
	struct sk_buff *msg;
	void *hdr;

<<<<<<< HEAD
	msg = nlmsg_new(NLMSG_GOODSIZE, gfp);
=======
	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, gfp);
>>>>>>> refs/remotes/origin/master
	if (!msg)
		return;

	hdr = nl80211hdr_put(msg, 0, 0, 0, NL80211_CMD_ROAM);
	if (!hdr) {
		nlmsg_free(msg);
		return;
	}

<<<<<<< HEAD
	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx);
	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, netdev->ifindex);
	NLA_PUT(msg, NL80211_ATTR_MAC, ETH_ALEN, bssid);
	if (req_ie)
		NLA_PUT(msg, NL80211_ATTR_REQ_IE, req_ie_len, req_ie);
	if (resp_ie)
		NLA_PUT(msg, NL80211_ATTR_RESP_IE, resp_ie_len, resp_ie);

<<<<<<< HEAD
	if (genlmsg_end(msg, hdr) < 0) {
		nlmsg_free(msg);
		return;
	}
=======
	genlmsg_end(msg, hdr);
>>>>>>> refs/remotes/origin/cm-10.0

	genlmsg_multicast_netns(wiphy_net(&rdev->wiphy), msg, 0,
				nl80211_mlme_mcgrp.id, gfp);
=======
	if (nla_put_u32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx) ||
	    nla_put_u32(msg, NL80211_ATTR_IFINDEX, netdev->ifindex) ||
	    nla_put(msg, NL80211_ATTR_MAC, ETH_ALEN, bssid) ||
	    (req_ie &&
	     nla_put(msg, NL80211_ATTR_REQ_IE, req_ie_len, req_ie)) ||
	    (resp_ie &&
	     nla_put(msg, NL80211_ATTR_RESP_IE, resp_ie_len, resp_ie)))
		goto nla_put_failure;

	genlmsg_end(msg, hdr);

	genlmsg_multicast_netns(&nl80211_fam, wiphy_net(&rdev->wiphy), msg, 0,
				NL80211_MCGRP_MLME, gfp);
>>>>>>> refs/remotes/origin/master
	return;

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	nlmsg_free(msg);

}

void nl80211_send_disconnected(struct cfg80211_registered_device *rdev,
			       struct net_device *netdev, u16 reason,
			       const u8 *ie, size_t ie_len, bool from_ap)
{
	struct sk_buff *msg;
	void *hdr;

<<<<<<< HEAD
	msg = nlmsg_new(NLMSG_GOODSIZE, GFP_KERNEL);
=======
	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (!msg)
		return;

	hdr = nl80211hdr_put(msg, 0, 0, 0, NL80211_CMD_DISCONNECT);
	if (!hdr) {
		nlmsg_free(msg);
		return;
	}

<<<<<<< HEAD
	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx);
	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, netdev->ifindex);
	if (from_ap && reason)
		NLA_PUT_U16(msg, NL80211_ATTR_REASON_CODE, reason);
	if (from_ap)
		NLA_PUT_FLAG(msg, NL80211_ATTR_DISCONNECTED_BY_AP);
	if (ie)
		NLA_PUT(msg, NL80211_ATTR_IE, ie_len, ie);

<<<<<<< HEAD
	if (genlmsg_end(msg, hdr) < 0) {
		nlmsg_free(msg);
		return;
	}
=======
	genlmsg_end(msg, hdr);
>>>>>>> refs/remotes/origin/cm-10.0

	genlmsg_multicast_netns(wiphy_net(&rdev->wiphy), msg, 0,
				nl80211_mlme_mcgrp.id, GFP_KERNEL);
=======
	if (nla_put_u32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx) ||
	    nla_put_u32(msg, NL80211_ATTR_IFINDEX, netdev->ifindex) ||
	    (from_ap && reason &&
	     nla_put_u16(msg, NL80211_ATTR_REASON_CODE, reason)) ||
	    (from_ap &&
	     nla_put_flag(msg, NL80211_ATTR_DISCONNECTED_BY_AP)) ||
	    (ie && nla_put(msg, NL80211_ATTR_IE, ie_len, ie)))
		goto nla_put_failure;

	genlmsg_end(msg, hdr);

	genlmsg_multicast_netns(&nl80211_fam, wiphy_net(&rdev->wiphy), msg, 0,
				NL80211_MCGRP_MLME, GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	return;

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	nlmsg_free(msg);

}

void nl80211_send_ibss_bssid(struct cfg80211_registered_device *rdev,
			     struct net_device *netdev, const u8 *bssid,
			     gfp_t gfp)
{
	struct sk_buff *msg;
	void *hdr;

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, gfp);
	if (!msg)
		return;

	hdr = nl80211hdr_put(msg, 0, 0, 0, NL80211_CMD_JOIN_IBSS);
	if (!hdr) {
		nlmsg_free(msg);
		return;
	}

<<<<<<< HEAD
	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx);
	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, netdev->ifindex);
	NLA_PUT(msg, NL80211_ATTR_MAC, ETH_ALEN, bssid);

<<<<<<< HEAD
	if (genlmsg_end(msg, hdr) < 0) {
		nlmsg_free(msg);
		return;
	}
=======
	genlmsg_end(msg, hdr);
>>>>>>> refs/remotes/origin/cm-10.0

	genlmsg_multicast_netns(wiphy_net(&rdev->wiphy), msg, 0,
				nl80211_mlme_mcgrp.id, gfp);
=======
	if (nla_put_u32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx) ||
	    nla_put_u32(msg, NL80211_ATTR_IFINDEX, netdev->ifindex) ||
	    nla_put(msg, NL80211_ATTR_MAC, ETH_ALEN, bssid))
		goto nla_put_failure;

	genlmsg_end(msg, hdr);

	genlmsg_multicast_netns(&nl80211_fam, wiphy_net(&rdev->wiphy), msg, 0,
				NL80211_MCGRP_MLME, gfp);
>>>>>>> refs/remotes/origin/master
	return;

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	nlmsg_free(msg);
}

<<<<<<< HEAD
void nl80211_send_new_peer_candidate(struct cfg80211_registered_device *rdev,
		struct net_device *netdev,
		const u8 *macaddr, const u8* ie, u8 ie_len,
		gfp_t gfp)
{
	struct sk_buff *msg;
	void *hdr;

=======
void cfg80211_notify_new_peer_candidate(struct net_device *dev, const u8 *addr,
					const u8* ie, u8 ie_len, gfp_t gfp)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wdev->wiphy);
	struct sk_buff *msg;
	void *hdr;

	if (WARN_ON(wdev->iftype != NL80211_IFTYPE_MESH_POINT))
		return;

	trace_cfg80211_notify_new_peer_candidate(dev, addr);

>>>>>>> refs/remotes/origin/master
	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, gfp);
	if (!msg)
		return;

	hdr = nl80211hdr_put(msg, 0, 0, 0, NL80211_CMD_NEW_PEER_CANDIDATE);
	if (!hdr) {
		nlmsg_free(msg);
		return;
	}

<<<<<<< HEAD
	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx);
	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, netdev->ifindex);
	NLA_PUT(msg, NL80211_ATTR_MAC, ETH_ALEN, macaddr);
	if (ie_len && ie)
		NLA_PUT(msg, NL80211_ATTR_IE, ie_len , ie);

<<<<<<< HEAD
	if (genlmsg_end(msg, hdr) < 0) {
		nlmsg_free(msg);
		return;
	}
=======
	genlmsg_end(msg, hdr);
>>>>>>> refs/remotes/origin/cm-10.0

	genlmsg_multicast_netns(wiphy_net(&rdev->wiphy), msg, 0,
				nl80211_mlme_mcgrp.id, gfp);
=======
	if (nla_put_u32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx) ||
	    nla_put_u32(msg, NL80211_ATTR_IFINDEX, dev->ifindex) ||
	    nla_put(msg, NL80211_ATTR_MAC, ETH_ALEN, addr) ||
	    (ie_len && ie &&
	     nla_put(msg, NL80211_ATTR_IE, ie_len , ie)))
		goto nla_put_failure;

	genlmsg_end(msg, hdr);

	genlmsg_multicast_netns(&nl80211_fam, wiphy_net(&rdev->wiphy), msg, 0,
				NL80211_MCGRP_MLME, gfp);
>>>>>>> refs/remotes/origin/master
	return;

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	nlmsg_free(msg);
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL(cfg80211_notify_new_peer_candidate);
>>>>>>> refs/remotes/origin/master

void nl80211_michael_mic_failure(struct cfg80211_registered_device *rdev,
				 struct net_device *netdev, const u8 *addr,
				 enum nl80211_key_type key_type, int key_id,
				 const u8 *tsc, gfp_t gfp)
{
	struct sk_buff *msg;
	void *hdr;

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, gfp);
	if (!msg)
		return;

	hdr = nl80211hdr_put(msg, 0, 0, 0, NL80211_CMD_MICHAEL_MIC_FAILURE);
	if (!hdr) {
		nlmsg_free(msg);
		return;
	}

<<<<<<< HEAD
	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx);
	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, netdev->ifindex);
	if (addr)
		NLA_PUT(msg, NL80211_ATTR_MAC, ETH_ALEN, addr);
	NLA_PUT_U32(msg, NL80211_ATTR_KEY_TYPE, key_type);
	if (key_id != -1)
		NLA_PUT_U8(msg, NL80211_ATTR_KEY_IDX, key_id);
	if (tsc)
		NLA_PUT(msg, NL80211_ATTR_KEY_SEQ, 6, tsc);

<<<<<<< HEAD
	if (genlmsg_end(msg, hdr) < 0) {
		nlmsg_free(msg);
		return;
	}
=======
	genlmsg_end(msg, hdr);
>>>>>>> refs/remotes/origin/cm-10.0

	genlmsg_multicast_netns(wiphy_net(&rdev->wiphy), msg, 0,
				nl80211_mlme_mcgrp.id, gfp);
=======
	if (nla_put_u32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx) ||
	    nla_put_u32(msg, NL80211_ATTR_IFINDEX, netdev->ifindex) ||
	    (addr && nla_put(msg, NL80211_ATTR_MAC, ETH_ALEN, addr)) ||
	    nla_put_u32(msg, NL80211_ATTR_KEY_TYPE, key_type) ||
	    (key_id != -1 &&
	     nla_put_u8(msg, NL80211_ATTR_KEY_IDX, key_id)) ||
	    (tsc && nla_put(msg, NL80211_ATTR_KEY_SEQ, 6, tsc)))
		goto nla_put_failure;

	genlmsg_end(msg, hdr);

	genlmsg_multicast_netns(&nl80211_fam, wiphy_net(&rdev->wiphy), msg, 0,
				NL80211_MCGRP_MLME, gfp);
>>>>>>> refs/remotes/origin/master
	return;

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	nlmsg_free(msg);
}

void nl80211_send_beacon_hint_event(struct wiphy *wiphy,
				    struct ieee80211_channel *channel_before,
				    struct ieee80211_channel *channel_after)
{
	struct sk_buff *msg;
	void *hdr;
	struct nlattr *nl_freq;

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_ATOMIC);
	if (!msg)
		return;

	hdr = nl80211hdr_put(msg, 0, 0, 0, NL80211_CMD_REG_BEACON_HINT);
	if (!hdr) {
		nlmsg_free(msg);
		return;
	}

	/*
	 * Since we are applying the beacon hint to a wiphy we know its
	 * wiphy_idx is valid
	 */
<<<<<<< HEAD
	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY, get_wiphy_idx(wiphy));
=======
	if (nla_put_u32(msg, NL80211_ATTR_WIPHY, get_wiphy_idx(wiphy)))
		goto nla_put_failure;
>>>>>>> refs/remotes/origin/master

	/* Before */
	nl_freq = nla_nest_start(msg, NL80211_ATTR_FREQ_BEFORE);
	if (!nl_freq)
		goto nla_put_failure;
<<<<<<< HEAD
	if (nl80211_msg_put_channel(msg, channel_before))
=======
	if (nl80211_msg_put_channel(msg, channel_before, false))
>>>>>>> refs/remotes/origin/master
		goto nla_put_failure;
	nla_nest_end(msg, nl_freq);

	/* After */
	nl_freq = nla_nest_start(msg, NL80211_ATTR_FREQ_AFTER);
	if (!nl_freq)
		goto nla_put_failure;
<<<<<<< HEAD
	if (nl80211_msg_put_channel(msg, channel_after))
		goto nla_put_failure;
	nla_nest_end(msg, nl_freq);

<<<<<<< HEAD
	if (genlmsg_end(msg, hdr) < 0) {
		nlmsg_free(msg);
		return;
	}
=======
	genlmsg_end(msg, hdr);
>>>>>>> refs/remotes/origin/cm-10.0

	rcu_read_lock();
	genlmsg_multicast_allns(msg, 0, nl80211_regulatory_mcgrp.id,
				GFP_ATOMIC);
=======
	if (nl80211_msg_put_channel(msg, channel_after, false))
		goto nla_put_failure;
	nla_nest_end(msg, nl_freq);

	genlmsg_end(msg, hdr);

	rcu_read_lock();
	genlmsg_multicast_allns(&nl80211_fam, msg, 0,
				NL80211_MCGRP_REGULATORY, GFP_ATOMIC);
>>>>>>> refs/remotes/origin/master
	rcu_read_unlock();

	return;

nla_put_failure:
	genlmsg_cancel(msg, hdr);
	nlmsg_free(msg);
}

static void nl80211_send_remain_on_chan_event(
	int cmd, struct cfg80211_registered_device *rdev,
<<<<<<< HEAD
	struct net_device *netdev, u64 cookie,
	struct ieee80211_channel *chan,
	enum nl80211_channel_type channel_type,
=======
	struct wireless_dev *wdev, u64 cookie,
	struct ieee80211_channel *chan,
>>>>>>> refs/remotes/origin/master
	unsigned int duration, gfp_t gfp)
{
	struct sk_buff *msg;
	void *hdr;

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, gfp);
	if (!msg)
		return;

	hdr = nl80211hdr_put(msg, 0, 0, 0, cmd);
	if (!hdr) {
		nlmsg_free(msg);
		return;
	}

<<<<<<< HEAD
	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx);
	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, netdev->ifindex);
	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY_FREQ, chan->center_freq);
	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY_CHANNEL_TYPE, channel_type);
	NLA_PUT_U64(msg, NL80211_ATTR_COOKIE, cookie);

	if (cmd == NL80211_CMD_REMAIN_ON_CHANNEL)
		NLA_PUT_U32(msg, NL80211_ATTR_DURATION, duration);

<<<<<<< HEAD
	if (genlmsg_end(msg, hdr) < 0) {
		nlmsg_free(msg);
		return;
	}
=======
	genlmsg_end(msg, hdr);
>>>>>>> refs/remotes/origin/cm-10.0

	genlmsg_multicast_netns(wiphy_net(&rdev->wiphy), msg, 0,
				nl80211_mlme_mcgrp.id, gfp);
=======
	if (nla_put_u32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx) ||
	    (wdev->netdev && nla_put_u32(msg, NL80211_ATTR_IFINDEX,
					 wdev->netdev->ifindex)) ||
	    nla_put_u64(msg, NL80211_ATTR_WDEV, wdev_id(wdev)) ||
	    nla_put_u32(msg, NL80211_ATTR_WIPHY_FREQ, chan->center_freq) ||
	    nla_put_u32(msg, NL80211_ATTR_WIPHY_CHANNEL_TYPE,
			NL80211_CHAN_NO_HT) ||
	    nla_put_u64(msg, NL80211_ATTR_COOKIE, cookie))
		goto nla_put_failure;

	if (cmd == NL80211_CMD_REMAIN_ON_CHANNEL &&
	    nla_put_u32(msg, NL80211_ATTR_DURATION, duration))
		goto nla_put_failure;

	genlmsg_end(msg, hdr);

	genlmsg_multicast_netns(&nl80211_fam, wiphy_net(&rdev->wiphy), msg, 0,
				NL80211_MCGRP_MLME, gfp);
>>>>>>> refs/remotes/origin/master
	return;

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	nlmsg_free(msg);
}

<<<<<<< HEAD
void nl80211_send_remain_on_channel(struct cfg80211_registered_device *rdev,
				    struct net_device *netdev, u64 cookie,
				    struct ieee80211_channel *chan,
				    enum nl80211_channel_type channel_type,
				    unsigned int duration, gfp_t gfp)
{
	nl80211_send_remain_on_chan_event(NL80211_CMD_REMAIN_ON_CHANNEL,
					  rdev, netdev, cookie, chan,
					  channel_type, duration, gfp);
}

void nl80211_send_remain_on_channel_cancel(
	struct cfg80211_registered_device *rdev, struct net_device *netdev,
	u64 cookie, struct ieee80211_channel *chan,
	enum nl80211_channel_type channel_type, gfp_t gfp)
{
	nl80211_send_remain_on_chan_event(NL80211_CMD_CANCEL_REMAIN_ON_CHANNEL,
					  rdev, netdev, cookie, chan,
					  channel_type, 0, gfp);
}

void nl80211_send_sta_event(struct cfg80211_registered_device *rdev,
			    struct net_device *dev, const u8 *mac_addr,
			    struct station_info *sinfo, gfp_t gfp)
{
	struct sk_buff *msg;

	msg = nlmsg_new(NLMSG_GOODSIZE, gfp);
	if (!msg)
		return;

<<<<<<< HEAD
	if (nl80211_send_station(msg, 0, 0, 0, dev, mac_addr, sinfo) < 0) {
=======
	if (nl80211_send_station(msg, 0, 0, 0,
				 rdev, dev, mac_addr, sinfo) < 0) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
void cfg80211_ready_on_channel(struct wireless_dev *wdev, u64 cookie,
			       struct ieee80211_channel *chan,
			       unsigned int duration, gfp_t gfp)
{
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);

	trace_cfg80211_ready_on_channel(wdev, cookie, chan, duration);
	nl80211_send_remain_on_chan_event(NL80211_CMD_REMAIN_ON_CHANNEL,
					  rdev, wdev, cookie, chan,
					  duration, gfp);
}
EXPORT_SYMBOL(cfg80211_ready_on_channel);

void cfg80211_remain_on_channel_expired(struct wireless_dev *wdev, u64 cookie,
					struct ieee80211_channel *chan,
					gfp_t gfp)
{
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);

	trace_cfg80211_ready_on_channel_expired(wdev, cookie, chan);
	nl80211_send_remain_on_chan_event(NL80211_CMD_CANCEL_REMAIN_ON_CHANNEL,
					  rdev, wdev, cookie, chan, 0, gfp);
}
EXPORT_SYMBOL(cfg80211_remain_on_channel_expired);

void cfg80211_new_sta(struct net_device *dev, const u8 *mac_addr,
		      struct station_info *sinfo, gfp_t gfp)
{
	struct wiphy *wiphy = dev->ieee80211_ptr->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);
	struct sk_buff *msg;

	trace_cfg80211_new_sta(dev, mac_addr, sinfo);

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, gfp);
	if (!msg)
		return;

	if (nl80211_send_station(msg, 0, 0, 0,
				 rdev, dev, mac_addr, sinfo) < 0) {
>>>>>>> refs/remotes/origin/master
		nlmsg_free(msg);
		return;
	}

<<<<<<< HEAD
	genlmsg_multicast_netns(wiphy_net(&rdev->wiphy), msg, 0,
				nl80211_mlme_mcgrp.id, gfp);
}

void nl80211_send_sta_del_event(struct cfg80211_registered_device *rdev,
				struct net_device *dev, const u8 *mac_addr,
				gfp_t gfp)
{
	struct sk_buff *msg;
	void *hdr;

	msg = nlmsg_new(NLMSG_GOODSIZE, gfp);
=======
	genlmsg_multicast_netns(&nl80211_fam, wiphy_net(&rdev->wiphy), msg, 0,
				NL80211_MCGRP_MLME, gfp);
}
EXPORT_SYMBOL(cfg80211_new_sta);

void cfg80211_del_sta(struct net_device *dev, const u8 *mac_addr, gfp_t gfp)
{
	struct wiphy *wiphy = dev->ieee80211_ptr->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);
	struct sk_buff *msg;
	void *hdr;

	trace_cfg80211_del_sta(dev, mac_addr);

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, gfp);
>>>>>>> refs/remotes/origin/master
	if (!msg)
		return;

	hdr = nl80211hdr_put(msg, 0, 0, 0, NL80211_CMD_DEL_STATION);
	if (!hdr) {
		nlmsg_free(msg);
		return;
	}

<<<<<<< HEAD
	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, dev->ifindex);
	NLA_PUT(msg, NL80211_ATTR_MAC, ETH_ALEN, mac_addr);

<<<<<<< HEAD
	if (genlmsg_end(msg, hdr) < 0) {
		nlmsg_free(msg);
		return;
	}
=======
	genlmsg_end(msg, hdr);
>>>>>>> refs/remotes/origin/cm-10.0

	genlmsg_multicast_netns(wiphy_net(&rdev->wiphy), msg, 0,
				nl80211_mlme_mcgrp.id, gfp);
=======
	if (nla_put_u32(msg, NL80211_ATTR_IFINDEX, dev->ifindex) ||
	    nla_put(msg, NL80211_ATTR_MAC, ETH_ALEN, mac_addr))
		goto nla_put_failure;

	genlmsg_end(msg, hdr);

	genlmsg_multicast_netns(&nl80211_fam, wiphy_net(&rdev->wiphy), msg, 0,
				NL80211_MCGRP_MLME, gfp);
>>>>>>> refs/remotes/origin/master
	return;

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	nlmsg_free(msg);
}
<<<<<<< HEAD

<<<<<<< HEAD
int nl80211_send_mgmt(struct cfg80211_registered_device *rdev,
		      struct net_device *netdev, u32 nlpid,
		      int freq, const u8 *buf, size_t len, gfp_t gfp)
{
	struct sk_buff *msg;
	void *hdr;
	int err;
=======
=======
EXPORT_SYMBOL(cfg80211_del_sta);

void cfg80211_conn_failed(struct net_device *dev, const u8 *mac_addr,
			  enum nl80211_connect_failed_reason reason,
			  gfp_t gfp)
{
	struct wiphy *wiphy = dev->ieee80211_ptr->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);
	struct sk_buff *msg;
	void *hdr;

	msg = nlmsg_new(NLMSG_GOODSIZE, gfp);
	if (!msg)
		return;

	hdr = nl80211hdr_put(msg, 0, 0, 0, NL80211_CMD_CONN_FAILED);
	if (!hdr) {
		nlmsg_free(msg);
		return;
	}

	if (nla_put_u32(msg, NL80211_ATTR_IFINDEX, dev->ifindex) ||
	    nla_put(msg, NL80211_ATTR_MAC, ETH_ALEN, mac_addr) ||
	    nla_put_u32(msg, NL80211_ATTR_CONN_FAILED_REASON, reason))
		goto nla_put_failure;

	genlmsg_end(msg, hdr);

	genlmsg_multicast_netns(&nl80211_fam, wiphy_net(&rdev->wiphy), msg, 0,
				NL80211_MCGRP_MLME, gfp);
	return;

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	nlmsg_free(msg);
}
EXPORT_SYMBOL(cfg80211_conn_failed);

>>>>>>> refs/remotes/origin/master
static bool __nl80211_unexpected_frame(struct net_device *dev, u8 cmd,
				       const u8 *addr, gfp_t gfp)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wdev->wiphy);
	struct sk_buff *msg;
	void *hdr;
<<<<<<< HEAD
	int err;
	u32 nlpid = ACCESS_ONCE(wdev->ap_unexpected_nlpid);

	if (!nlpid)
=======
	u32 nlportid = ACCESS_ONCE(wdev->ap_unexpected_nlportid);

	if (!nlportid)
>>>>>>> refs/remotes/origin/master
		return false;

	msg = nlmsg_new(100, gfp);
	if (!msg)
		return true;

	hdr = nl80211hdr_put(msg, 0, 0, 0, cmd);
	if (!hdr) {
		nlmsg_free(msg);
		return true;
	}

<<<<<<< HEAD
	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx);
	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, dev->ifindex);
	NLA_PUT(msg, NL80211_ATTR_MAC, ETH_ALEN, addr);

	err = genlmsg_end(msg, hdr);
	if (err < 0) {
		nlmsg_free(msg);
		return true;
	}

	genlmsg_unicast(wiphy_net(&rdev->wiphy), msg, nlpid);
=======
	if (nla_put_u32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx) ||
	    nla_put_u32(msg, NL80211_ATTR_IFINDEX, dev->ifindex) ||
	    nla_put(msg, NL80211_ATTR_MAC, ETH_ALEN, addr))
		goto nla_put_failure;

	genlmsg_end(msg, hdr);
	genlmsg_unicast(wiphy_net(&rdev->wiphy), msg, nlportid);
>>>>>>> refs/remotes/origin/master
	return true;

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	nlmsg_free(msg);
	return true;
}

<<<<<<< HEAD
bool nl80211_unexpected_frame(struct net_device *dev, const u8 *addr, gfp_t gfp)
{
	return __nl80211_unexpected_frame(dev, NL80211_CMD_UNEXPECTED_FRAME,
					  addr, gfp);
}

bool nl80211_unexpected_4addr_frame(struct net_device *dev,
				    const u8 *addr, gfp_t gfp)
{
	return __nl80211_unexpected_frame(dev,
					  NL80211_CMD_UNEXPECTED_4ADDR_FRAME,
					  addr, gfp);
}

int nl80211_send_mgmt(struct cfg80211_registered_device *rdev,
		      struct net_device *netdev, u32 nlpid,
		      int freq, int sig_dbm,
		      const u8 *buf, size_t len, gfp_t gfp)
{
	struct sk_buff *msg;
	void *hdr;
>>>>>>> refs/remotes/origin/cm-10.0
=======
bool cfg80211_rx_spurious_frame(struct net_device *dev,
				const u8 *addr, gfp_t gfp)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	bool ret;

	trace_cfg80211_rx_spurious_frame(dev, addr);

	if (WARN_ON(wdev->iftype != NL80211_IFTYPE_AP &&
		    wdev->iftype != NL80211_IFTYPE_P2P_GO)) {
		trace_cfg80211_return_bool(false);
		return false;
	}
	ret = __nl80211_unexpected_frame(dev, NL80211_CMD_UNEXPECTED_FRAME,
					 addr, gfp);
	trace_cfg80211_return_bool(ret);
	return ret;
}
EXPORT_SYMBOL(cfg80211_rx_spurious_frame);

bool cfg80211_rx_unexpected_4addr_frame(struct net_device *dev,
					const u8 *addr, gfp_t gfp)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	bool ret;

	trace_cfg80211_rx_unexpected_4addr_frame(dev, addr);

	if (WARN_ON(wdev->iftype != NL80211_IFTYPE_AP &&
		    wdev->iftype != NL80211_IFTYPE_P2P_GO &&
		    wdev->iftype != NL80211_IFTYPE_AP_VLAN)) {
		trace_cfg80211_return_bool(false);
		return false;
	}
	ret = __nl80211_unexpected_frame(dev,
					 NL80211_CMD_UNEXPECTED_4ADDR_FRAME,
					 addr, gfp);
	trace_cfg80211_return_bool(ret);
	return ret;
}
EXPORT_SYMBOL(cfg80211_rx_unexpected_4addr_frame);

int nl80211_send_mgmt(struct cfg80211_registered_device *rdev,
		      struct wireless_dev *wdev, u32 nlportid,
		      int freq, int sig_dbm,
		      const u8 *buf, size_t len, u32 flags, gfp_t gfp)
{
	struct net_device *netdev = wdev->netdev;
	struct sk_buff *msg;
	void *hdr;
>>>>>>> refs/remotes/origin/master

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, gfp);
	if (!msg)
		return -ENOMEM;

	hdr = nl80211hdr_put(msg, 0, 0, 0, NL80211_CMD_FRAME);
	if (!hdr) {
		nlmsg_free(msg);
		return -ENOMEM;
	}

<<<<<<< HEAD
	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx);
	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, netdev->ifindex);
	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY_FREQ, freq);
<<<<<<< HEAD
	NLA_PUT(msg, NL80211_ATTR_FRAME, len, buf);

	err = genlmsg_end(msg, hdr);
	if (err < 0) {
		nlmsg_free(msg);
		return err;
	}

	err = genlmsg_unicast(wiphy_net(&rdev->wiphy), msg, nlpid);
	if (err < 0)
		return err;
	return 0;
=======
	if (sig_dbm)
		NLA_PUT_U32(msg, NL80211_ATTR_RX_SIGNAL_DBM, sig_dbm);
	NLA_PUT(msg, NL80211_ATTR_FRAME, len, buf);

	genlmsg_end(msg, hdr);

	return genlmsg_unicast(wiphy_net(&rdev->wiphy), msg, nlpid);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (nla_put_u32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx) ||
	    (netdev && nla_put_u32(msg, NL80211_ATTR_IFINDEX,
					netdev->ifindex)) ||
	    nla_put_u64(msg, NL80211_ATTR_WDEV, wdev_id(wdev)) ||
	    nla_put_u32(msg, NL80211_ATTR_WIPHY_FREQ, freq) ||
	    (sig_dbm &&
	     nla_put_u32(msg, NL80211_ATTR_RX_SIGNAL_DBM, sig_dbm)) ||
	    nla_put(msg, NL80211_ATTR_FRAME, len, buf) ||
	    (flags &&
	     nla_put_u32(msg, NL80211_ATTR_RXMGMT_FLAGS, flags)))
		goto nla_put_failure;

	genlmsg_end(msg, hdr);

	return genlmsg_unicast(wiphy_net(&rdev->wiphy), msg, nlportid);

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	nlmsg_free(msg);
	return -ENOBUFS;
}

void cfg80211_mgmt_tx_status(struct wireless_dev *wdev, u64 cookie,
			     const u8 *buf, size_t len, bool ack, gfp_t gfp)
{
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);
	struct net_device *netdev = wdev->netdev;
	struct sk_buff *msg;
	void *hdr;

	trace_cfg80211_mgmt_tx_status(wdev, cookie, ack);

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, gfp);
	if (!msg)
		return;

	hdr = nl80211hdr_put(msg, 0, 0, 0, NL80211_CMD_FRAME_TX_STATUS);
	if (!hdr) {
		nlmsg_free(msg);
		return;
	}

	if (nla_put_u32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx) ||
	    (netdev && nla_put_u32(msg, NL80211_ATTR_IFINDEX,
				   netdev->ifindex)) ||
	    nla_put_u64(msg, NL80211_ATTR_WDEV, wdev_id(wdev)) ||
	    nla_put(msg, NL80211_ATTR_FRAME, len, buf) ||
	    nla_put_u64(msg, NL80211_ATTR_COOKIE, cookie) ||
	    (ack && nla_put_flag(msg, NL80211_ATTR_ACK)))
		goto nla_put_failure;

	genlmsg_end(msg, hdr);

	genlmsg_multicast_netns(&nl80211_fam, wiphy_net(&rdev->wiphy), msg, 0,
				NL80211_MCGRP_MLME, gfp);
	return;

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	nlmsg_free(msg);
}
EXPORT_SYMBOL(cfg80211_mgmt_tx_status);

void cfg80211_cqm_rssi_notify(struct net_device *dev,
			      enum nl80211_cqm_rssi_threshold_event rssi_event,
			      gfp_t gfp)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);
	struct sk_buff *msg;
	struct nlattr *pinfoattr;
	void *hdr;

	trace_cfg80211_cqm_rssi_notify(dev, rssi_event);

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, gfp);
	if (!msg)
		return;

	hdr = nl80211hdr_put(msg, 0, 0, 0, NL80211_CMD_NOTIFY_CQM);
	if (!hdr) {
		nlmsg_free(msg);
		return;
	}

	if (nla_put_u32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx) ||
	    nla_put_u32(msg, NL80211_ATTR_IFINDEX, dev->ifindex))
		goto nla_put_failure;

	pinfoattr = nla_nest_start(msg, NL80211_ATTR_CQM);
	if (!pinfoattr)
		goto nla_put_failure;

	if (nla_put_u32(msg, NL80211_ATTR_CQM_RSSI_THRESHOLD_EVENT,
			rssi_event))
		goto nla_put_failure;

	nla_nest_end(msg, pinfoattr);

	genlmsg_end(msg, hdr);

	genlmsg_multicast_netns(&nl80211_fam, wiphy_net(&rdev->wiphy), msg, 0,
				NL80211_MCGRP_MLME, gfp);
	return;

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	nlmsg_free(msg);
}
EXPORT_SYMBOL(cfg80211_cqm_rssi_notify);

static void nl80211_gtk_rekey_notify(struct cfg80211_registered_device *rdev,
				     struct net_device *netdev, const u8 *bssid,
				     const u8 *replay_ctr, gfp_t gfp)
{
	struct sk_buff *msg;
	struct nlattr *rekey_attr;
	void *hdr;

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, gfp);
	if (!msg)
		return;

	hdr = nl80211hdr_put(msg, 0, 0, 0, NL80211_CMD_SET_REKEY_OFFLOAD);
	if (!hdr) {
		nlmsg_free(msg);
		return;
	}

	if (nla_put_u32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx) ||
	    nla_put_u32(msg, NL80211_ATTR_IFINDEX, netdev->ifindex) ||
	    nla_put(msg, NL80211_ATTR_MAC, ETH_ALEN, bssid))
		goto nla_put_failure;

	rekey_attr = nla_nest_start(msg, NL80211_ATTR_REKEY_DATA);
	if (!rekey_attr)
		goto nla_put_failure;

	if (nla_put(msg, NL80211_REKEY_DATA_REPLAY_CTR,
		    NL80211_REPLAY_CTR_LEN, replay_ctr))
		goto nla_put_failure;

	nla_nest_end(msg, rekey_attr);

	genlmsg_end(msg, hdr);

	genlmsg_multicast_netns(&nl80211_fam, wiphy_net(&rdev->wiphy), msg, 0,
				NL80211_MCGRP_MLME, gfp);
	return;
>>>>>>> refs/remotes/origin/master

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	nlmsg_free(msg);
<<<<<<< HEAD
	return -ENOBUFS;
}

void nl80211_send_mgmt_tx_status(struct cfg80211_registered_device *rdev,
				 struct net_device *netdev, u64 cookie,
				 const u8 *buf, size_t len, bool ack,
				 gfp_t gfp)
{
	struct sk_buff *msg;
=======
}

void cfg80211_gtk_rekey_notify(struct net_device *dev, const u8 *bssid,
			       const u8 *replay_ctr, gfp_t gfp)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);

	trace_cfg80211_gtk_rekey_notify(dev, bssid);
	nl80211_gtk_rekey_notify(rdev, dev, bssid, replay_ctr, gfp);
}
EXPORT_SYMBOL(cfg80211_gtk_rekey_notify);

static void
nl80211_pmksa_candidate_notify(struct cfg80211_registered_device *rdev,
			       struct net_device *netdev, int index,
			       const u8 *bssid, bool preauth, gfp_t gfp)
{
	struct sk_buff *msg;
	struct nlattr *attr;
>>>>>>> refs/remotes/origin/master
	void *hdr;

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, gfp);
	if (!msg)
		return;

<<<<<<< HEAD
	hdr = nl80211hdr_put(msg, 0, 0, 0, NL80211_CMD_FRAME_TX_STATUS);
=======
	hdr = nl80211hdr_put(msg, 0, 0, 0, NL80211_CMD_PMKSA_CANDIDATE);
>>>>>>> refs/remotes/origin/master
	if (!hdr) {
		nlmsg_free(msg);
		return;
	}

<<<<<<< HEAD
	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx);
	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, netdev->ifindex);
	NLA_PUT(msg, NL80211_ATTR_FRAME, len, buf);
	NLA_PUT_U64(msg, NL80211_ATTR_COOKIE, cookie);
	if (ack)
		NLA_PUT_FLAG(msg, NL80211_ATTR_ACK);

<<<<<<< HEAD
	if (genlmsg_end(msg, hdr) < 0) {
		nlmsg_free(msg);
		return;
	}
=======
	genlmsg_end(msg, hdr);
>>>>>>> refs/remotes/origin/cm-10.0

	genlmsg_multicast_netns(wiphy_net(&rdev->wiphy), msg, 0,
				nl80211_mlme_mcgrp.id, gfp);
=======
	if (nla_put_u32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx) ||
	    nla_put_u32(msg, NL80211_ATTR_IFINDEX, netdev->ifindex))
		goto nla_put_failure;

	attr = nla_nest_start(msg, NL80211_ATTR_PMKSA_CANDIDATE);
	if (!attr)
		goto nla_put_failure;

	if (nla_put_u32(msg, NL80211_PMKSA_CANDIDATE_INDEX, index) ||
	    nla_put(msg, NL80211_PMKSA_CANDIDATE_BSSID, ETH_ALEN, bssid) ||
	    (preauth &&
	     nla_put_flag(msg, NL80211_PMKSA_CANDIDATE_PREAUTH)))
		goto nla_put_failure;

	nla_nest_end(msg, attr);

	genlmsg_end(msg, hdr);

	genlmsg_multicast_netns(&nl80211_fam, wiphy_net(&rdev->wiphy), msg, 0,
				NL80211_MCGRP_MLME, gfp);
>>>>>>> refs/remotes/origin/master
	return;

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	nlmsg_free(msg);
}

<<<<<<< HEAD
void
nl80211_send_cqm_rssi_notify(struct cfg80211_registered_device *rdev,
			     struct net_device *netdev,
			     enum nl80211_cqm_rssi_threshold_event rssi_event,
			     gfp_t gfp)
{
	struct sk_buff *msg;
	struct nlattr *pinfoattr;
	void *hdr;

	msg = nlmsg_new(NLMSG_GOODSIZE, gfp);
	if (!msg)
		return;

	hdr = nl80211hdr_put(msg, 0, 0, 0, NL80211_CMD_NOTIFY_CQM);
=======
void cfg80211_pmksa_candidate_notify(struct net_device *dev, int index,
				     const u8 *bssid, bool preauth, gfp_t gfp)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);

	trace_cfg80211_pmksa_candidate_notify(dev, index, bssid, preauth);
	nl80211_pmksa_candidate_notify(rdev, dev, index, bssid, preauth, gfp);
}
EXPORT_SYMBOL(cfg80211_pmksa_candidate_notify);

static void nl80211_ch_switch_notify(struct cfg80211_registered_device *rdev,
				     struct net_device *netdev,
				     struct cfg80211_chan_def *chandef,
				     gfp_t gfp)
{
	struct sk_buff *msg;
	void *hdr;

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, gfp);
	if (!msg)
		return;

	hdr = nl80211hdr_put(msg, 0, 0, 0, NL80211_CMD_CH_SWITCH_NOTIFY);
>>>>>>> refs/remotes/origin/master
	if (!hdr) {
		nlmsg_free(msg);
		return;
	}

<<<<<<< HEAD
	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx);
	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, netdev->ifindex);

	pinfoattr = nla_nest_start(msg, NL80211_ATTR_CQM);
	if (!pinfoattr)
		goto nla_put_failure;

	NLA_PUT_U32(msg, NL80211_ATTR_CQM_RSSI_THRESHOLD_EVENT,
		    rssi_event);

	nla_nest_end(msg, pinfoattr);

<<<<<<< HEAD
	if (genlmsg_end(msg, hdr) < 0) {
=======
	genlmsg_end(msg, hdr);

	genlmsg_multicast_netns(wiphy_net(&rdev->wiphy), msg, 0,
				nl80211_mlme_mcgrp.id, gfp);
=======
	if (nla_put_u32(msg, NL80211_ATTR_IFINDEX, netdev->ifindex))
		goto nla_put_failure;

	if (nl80211_send_chandef(msg, chandef))
		goto nla_put_failure;

	genlmsg_end(msg, hdr);

	genlmsg_multicast_netns(&nl80211_fam, wiphy_net(&rdev->wiphy), msg, 0,
				NL80211_MCGRP_MLME, gfp);
>>>>>>> refs/remotes/origin/master
	return;

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	nlmsg_free(msg);
}

<<<<<<< HEAD
void nl80211_gtk_rekey_notify(struct cfg80211_registered_device *rdev,
			      struct net_device *netdev, const u8 *bssid,
			      const u8 *replay_ctr, gfp_t gfp)
{
	struct sk_buff *msg;
	struct nlattr *rekey_attr;
=======
void cfg80211_ch_switch_notify(struct net_device *dev,
			       struct cfg80211_chan_def *chandef)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);

	trace_cfg80211_ch_switch_notify(dev, chandef);

	wdev_lock(wdev);

	if (WARN_ON(wdev->iftype != NL80211_IFTYPE_AP &&
		    wdev->iftype != NL80211_IFTYPE_P2P_GO &&
		    wdev->iftype != NL80211_IFTYPE_ADHOC &&
		    wdev->iftype != NL80211_IFTYPE_MESH_POINT))
		goto out;

	wdev->channel = chandef->chan;
	nl80211_ch_switch_notify(rdev, dev, chandef, GFP_KERNEL);
out:
	wdev_unlock(wdev);
	return;
}
EXPORT_SYMBOL(cfg80211_ch_switch_notify);

void cfg80211_cqm_txe_notify(struct net_device *dev,
			     const u8 *peer, u32 num_packets,
			     u32 rate, u32 intvl, gfp_t gfp)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);
	struct sk_buff *msg;
	struct nlattr *pinfoattr;
>>>>>>> refs/remotes/origin/master
	void *hdr;

	msg = nlmsg_new(NLMSG_GOODSIZE, gfp);
	if (!msg)
		return;

<<<<<<< HEAD
	hdr = nl80211hdr_put(msg, 0, 0, 0, NL80211_CMD_SET_REKEY_OFFLOAD);
=======
	hdr = nl80211hdr_put(msg, 0, 0, 0, NL80211_CMD_NOTIFY_CQM);
>>>>>>> refs/remotes/origin/master
	if (!hdr) {
		nlmsg_free(msg);
		return;
	}

<<<<<<< HEAD
	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx);
	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, netdev->ifindex);
	NLA_PUT(msg, NL80211_ATTR_MAC, ETH_ALEN, bssid);

	rekey_attr = nla_nest_start(msg, NL80211_ATTR_REKEY_DATA);
	if (!rekey_attr)
		goto nla_put_failure;

	NLA_PUT(msg, NL80211_REKEY_DATA_REPLAY_CTR,
		NL80211_REPLAY_CTR_LEN, replay_ctr);

	nla_nest_end(msg, rekey_attr);

	genlmsg_end(msg, hdr);

	genlmsg_multicast_netns(wiphy_net(&rdev->wiphy), msg, 0,
				nl80211_mlme_mcgrp.id, gfp);
=======
	if (nla_put_u32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx) ||
	    nla_put_u32(msg, NL80211_ATTR_IFINDEX, dev->ifindex) ||
	    nla_put(msg, NL80211_ATTR_MAC, ETH_ALEN, peer))
		goto nla_put_failure;

	pinfoattr = nla_nest_start(msg, NL80211_ATTR_CQM);
	if (!pinfoattr)
		goto nla_put_failure;

	if (nla_put_u32(msg, NL80211_ATTR_CQM_TXE_PKTS, num_packets))
		goto nla_put_failure;

	if (nla_put_u32(msg, NL80211_ATTR_CQM_TXE_RATE, rate))
		goto nla_put_failure;

	if (nla_put_u32(msg, NL80211_ATTR_CQM_TXE_INTVL, intvl))
		goto nla_put_failure;

	nla_nest_end(msg, pinfoattr);

	genlmsg_end(msg, hdr);

	genlmsg_multicast_netns(&nl80211_fam, wiphy_net(&rdev->wiphy), msg, 0,
				NL80211_MCGRP_MLME, gfp);
>>>>>>> refs/remotes/origin/master
	return;

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	nlmsg_free(msg);
}
<<<<<<< HEAD

void nl80211_pmksa_candidate_notify(struct cfg80211_registered_device *rdev,
				    struct net_device *netdev, int index,
				    const u8 *bssid, bool preauth, gfp_t gfp)
{
	struct sk_buff *msg;
	struct nlattr *attr;
	void *hdr;

	msg = nlmsg_new(NLMSG_GOODSIZE, gfp);
	if (!msg)
		return;

	hdr = nl80211hdr_put(msg, 0, 0, 0, NL80211_CMD_PMKSA_CANDIDATE);
	if (!hdr) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
EXPORT_SYMBOL(cfg80211_cqm_txe_notify);

void
nl80211_radar_notify(struct cfg80211_registered_device *rdev,
		     struct cfg80211_chan_def *chandef,
		     enum nl80211_radar_event event,
		     struct net_device *netdev, gfp_t gfp)
{
	struct sk_buff *msg;
	void *hdr;

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, gfp);
	if (!msg)
		return;

	hdr = nl80211hdr_put(msg, 0, 0, 0, NL80211_CMD_RADAR_DETECT);
	if (!hdr) {
>>>>>>> refs/remotes/origin/master
		nlmsg_free(msg);
		return;
	}

<<<<<<< HEAD
<<<<<<< HEAD
=======
	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx);
	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, netdev->ifindex);

	attr = nla_nest_start(msg, NL80211_ATTR_PMKSA_CANDIDATE);
	if (!attr)
		goto nla_put_failure;

	NLA_PUT_U32(msg, NL80211_PMKSA_CANDIDATE_INDEX, index);
	NLA_PUT(msg, NL80211_PMKSA_CANDIDATE_BSSID, ETH_ALEN, bssid);
	if (preauth)
		NLA_PUT_FLAG(msg, NL80211_PMKSA_CANDIDATE_PREAUTH);

	nla_nest_end(msg, attr);

	genlmsg_end(msg, hdr);

>>>>>>> refs/remotes/origin/cm-10.0
	genlmsg_multicast_netns(wiphy_net(&rdev->wiphy), msg, 0,
				nl80211_mlme_mcgrp.id, gfp);
=======
	if (nla_put_u32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx))
		goto nla_put_failure;

	/* NOP and radar events don't need a netdev parameter */
	if (netdev) {
		struct wireless_dev *wdev = netdev->ieee80211_ptr;

		if (nla_put_u32(msg, NL80211_ATTR_IFINDEX, netdev->ifindex) ||
		    nla_put_u64(msg, NL80211_ATTR_WDEV, wdev_id(wdev)))
			goto nla_put_failure;
	}

	if (nla_put_u32(msg, NL80211_ATTR_RADAR_EVENT, event))
		goto nla_put_failure;

	if (nl80211_send_chandef(msg, chandef))
		goto nla_put_failure;

	genlmsg_end(msg, hdr);

	genlmsg_multicast_netns(&nl80211_fam, wiphy_net(&rdev->wiphy), msg, 0,
				NL80211_MCGRP_MLME, gfp);
>>>>>>> refs/remotes/origin/master
	return;

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	nlmsg_free(msg);
}

<<<<<<< HEAD
void
nl80211_send_cqm_pktloss_notify(struct cfg80211_registered_device *rdev,
				struct net_device *netdev, const u8 *peer,
				u32 num_packets, gfp_t gfp)
{
=======
void cfg80211_cqm_pktloss_notify(struct net_device *dev,
				 const u8 *peer, u32 num_packets, gfp_t gfp)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct wiphy *wiphy = wdev->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);
>>>>>>> refs/remotes/origin/master
	struct sk_buff *msg;
	struct nlattr *pinfoattr;
	void *hdr;

<<<<<<< HEAD
	msg = nlmsg_new(NLMSG_GOODSIZE, gfp);
=======
	trace_cfg80211_cqm_pktloss_notify(dev, peer, num_packets);

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, gfp);
>>>>>>> refs/remotes/origin/master
	if (!msg)
		return;

	hdr = nl80211hdr_put(msg, 0, 0, 0, NL80211_CMD_NOTIFY_CQM);
	if (!hdr) {
		nlmsg_free(msg);
		return;
	}

<<<<<<< HEAD
	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx);
	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, netdev->ifindex);
	NLA_PUT(msg, NL80211_ATTR_MAC, ETH_ALEN, peer);
=======
	if (nla_put_u32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx) ||
	    nla_put_u32(msg, NL80211_ATTR_IFINDEX, dev->ifindex) ||
	    nla_put(msg, NL80211_ATTR_MAC, ETH_ALEN, peer))
		goto nla_put_failure;
>>>>>>> refs/remotes/origin/master

	pinfoattr = nla_nest_start(msg, NL80211_ATTR_CQM);
	if (!pinfoattr)
		goto nla_put_failure;

<<<<<<< HEAD
	NLA_PUT_U32(msg, NL80211_ATTR_CQM_PKT_LOSS_EVENT, num_packets);

	nla_nest_end(msg, pinfoattr);

<<<<<<< HEAD
	if (genlmsg_end(msg, hdr) < 0) {
=======
	genlmsg_end(msg, hdr);

	genlmsg_multicast_netns(wiphy_net(&rdev->wiphy), msg, 0,
				nl80211_mlme_mcgrp.id, gfp);
=======
	if (nla_put_u32(msg, NL80211_ATTR_CQM_PKT_LOSS_EVENT, num_packets))
		goto nla_put_failure;

	nla_nest_end(msg, pinfoattr);

	genlmsg_end(msg, hdr);

	genlmsg_multicast_netns(&nl80211_fam, wiphy_net(&rdev->wiphy), msg, 0,
				NL80211_MCGRP_MLME, gfp);
>>>>>>> refs/remotes/origin/master
	return;

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	nlmsg_free(msg);
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL(cfg80211_cqm_pktloss_notify);
>>>>>>> refs/remotes/origin/master

void cfg80211_probe_status(struct net_device *dev, const u8 *addr,
			   u64 cookie, bool acked, gfp_t gfp)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wdev->wiphy);
	struct sk_buff *msg;
	void *hdr;
<<<<<<< HEAD
	int err;

	msg = nlmsg_new(NLMSG_GOODSIZE, gfp);
=======

	trace_cfg80211_probe_status(dev, addr, cookie, acked);

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, gfp);

>>>>>>> refs/remotes/origin/master
	if (!msg)
		return;

	hdr = nl80211hdr_put(msg, 0, 0, 0, NL80211_CMD_PROBE_CLIENT);
	if (!hdr) {
		nlmsg_free(msg);
		return;
	}

<<<<<<< HEAD
	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx);
	NLA_PUT_U32(msg, NL80211_ATTR_IFINDEX, dev->ifindex);
	NLA_PUT(msg, NL80211_ATTR_MAC, ETH_ALEN, addr);
	NLA_PUT_U64(msg, NL80211_ATTR_COOKIE, cookie);
	if (acked)
		NLA_PUT_FLAG(msg, NL80211_ATTR_ACK);

	err = genlmsg_end(msg, hdr);
	if (err < 0) {
>>>>>>> refs/remotes/origin/cm-10.0
		nlmsg_free(msg);
		return;
	}

	genlmsg_multicast_netns(wiphy_net(&rdev->wiphy), msg, 0,
				nl80211_mlme_mcgrp.id, gfp);
=======
	if (nla_put_u32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx) ||
	    nla_put_u32(msg, NL80211_ATTR_IFINDEX, dev->ifindex) ||
	    nla_put(msg, NL80211_ATTR_MAC, ETH_ALEN, addr) ||
	    nla_put_u64(msg, NL80211_ATTR_COOKIE, cookie) ||
	    (acked && nla_put_flag(msg, NL80211_ATTR_ACK)))
		goto nla_put_failure;

	genlmsg_end(msg, hdr);

	genlmsg_multicast_netns(&nl80211_fam, wiphy_net(&rdev->wiphy), msg, 0,
				NL80211_MCGRP_MLME, gfp);
>>>>>>> refs/remotes/origin/master
	return;

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	nlmsg_free(msg);
}
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
EXPORT_SYMBOL(cfg80211_probe_status);

void cfg80211_report_obss_beacon(struct wiphy *wiphy,
				 const u8 *frame, size_t len,
<<<<<<< HEAD
				 int freq, int sig_dbm, gfp_t gfp)
=======
				 int freq, int sig_dbm)
>>>>>>> refs/remotes/origin/master
{
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);
	struct sk_buff *msg;
	void *hdr;
<<<<<<< HEAD
	u32 nlpid = ACCESS_ONCE(rdev->ap_beacons_nlpid);

	if (!nlpid)
		return;

	msg = nlmsg_new(len + 100, gfp);
	if (!msg)
		return;

	hdr = nl80211hdr_put(msg, 0, 0, 0, NL80211_CMD_FRAME);
=======
	struct cfg80211_beacon_registration *reg;

	trace_cfg80211_report_obss_beacon(wiphy, frame, len, freq, sig_dbm);

	spin_lock_bh(&rdev->beacon_registrations_lock);
	list_for_each_entry(reg, &rdev->beacon_registrations, list) {
		msg = nlmsg_new(len + 100, GFP_ATOMIC);
		if (!msg) {
			spin_unlock_bh(&rdev->beacon_registrations_lock);
			return;
		}

		hdr = nl80211hdr_put(msg, 0, 0, 0, NL80211_CMD_FRAME);
		if (!hdr)
			goto nla_put_failure;

		if (nla_put_u32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx) ||
		    (freq &&
		     nla_put_u32(msg, NL80211_ATTR_WIPHY_FREQ, freq)) ||
		    (sig_dbm &&
		     nla_put_u32(msg, NL80211_ATTR_RX_SIGNAL_DBM, sig_dbm)) ||
		    nla_put(msg, NL80211_ATTR_FRAME, len, frame))
			goto nla_put_failure;

		genlmsg_end(msg, hdr);

		genlmsg_unicast(wiphy_net(&rdev->wiphy), msg, reg->nlportid);
	}
	spin_unlock_bh(&rdev->beacon_registrations_lock);
	return;

 nla_put_failure:
	spin_unlock_bh(&rdev->beacon_registrations_lock);
	if (hdr)
		genlmsg_cancel(msg, hdr);
	nlmsg_free(msg);
}
EXPORT_SYMBOL(cfg80211_report_obss_beacon);

#ifdef CONFIG_PM
void cfg80211_report_wowlan_wakeup(struct wireless_dev *wdev,
				   struct cfg80211_wowlan_wakeup *wakeup,
				   gfp_t gfp)
{
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wdev->wiphy);
	struct sk_buff *msg;
	void *hdr;
	int size = 200;

	trace_cfg80211_report_wowlan_wakeup(wdev->wiphy, wdev, wakeup);

	if (wakeup)
		size += wakeup->packet_present_len;

	msg = nlmsg_new(size, gfp);
	if (!msg)
		return;

	hdr = nl80211hdr_put(msg, 0, 0, 0, NL80211_CMD_SET_WOWLAN);
	if (!hdr)
		goto free_msg;

	if (nla_put_u32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx) ||
	    nla_put_u64(msg, NL80211_ATTR_WDEV, wdev_id(wdev)))
		goto free_msg;

	if (wdev->netdev && nla_put_u32(msg, NL80211_ATTR_IFINDEX,
					wdev->netdev->ifindex))
		goto free_msg;

	if (wakeup) {
		struct nlattr *reasons;

		reasons = nla_nest_start(msg, NL80211_ATTR_WOWLAN_TRIGGERS);
		if (!reasons)
			goto free_msg;

		if (wakeup->disconnect &&
		    nla_put_flag(msg, NL80211_WOWLAN_TRIG_DISCONNECT))
			goto free_msg;
		if (wakeup->magic_pkt &&
		    nla_put_flag(msg, NL80211_WOWLAN_TRIG_MAGIC_PKT))
			goto free_msg;
		if (wakeup->gtk_rekey_failure &&
		    nla_put_flag(msg, NL80211_WOWLAN_TRIG_GTK_REKEY_FAILURE))
			goto free_msg;
		if (wakeup->eap_identity_req &&
		    nla_put_flag(msg, NL80211_WOWLAN_TRIG_EAP_IDENT_REQUEST))
			goto free_msg;
		if (wakeup->four_way_handshake &&
		    nla_put_flag(msg, NL80211_WOWLAN_TRIG_4WAY_HANDSHAKE))
			goto free_msg;
		if (wakeup->rfkill_release &&
		    nla_put_flag(msg, NL80211_WOWLAN_TRIG_RFKILL_RELEASE))
			goto free_msg;

		if (wakeup->pattern_idx >= 0 &&
		    nla_put_u32(msg, NL80211_WOWLAN_TRIG_PKT_PATTERN,
				wakeup->pattern_idx))
			goto free_msg;

		if (wakeup->tcp_match &&
		    nla_put_flag(msg, NL80211_WOWLAN_TRIG_WAKEUP_TCP_MATCH))
			goto free_msg;

		if (wakeup->tcp_connlost &&
		    nla_put_flag(msg, NL80211_WOWLAN_TRIG_WAKEUP_TCP_CONNLOST))
			goto free_msg;

		if (wakeup->tcp_nomoretokens &&
		    nla_put_flag(msg,
				 NL80211_WOWLAN_TRIG_WAKEUP_TCP_NOMORETOKENS))
			goto free_msg;

		if (wakeup->packet) {
			u32 pkt_attr = NL80211_WOWLAN_TRIG_WAKEUP_PKT_80211;
			u32 len_attr = NL80211_WOWLAN_TRIG_WAKEUP_PKT_80211_LEN;

			if (!wakeup->packet_80211) {
				pkt_attr =
					NL80211_WOWLAN_TRIG_WAKEUP_PKT_8023;
				len_attr =
					NL80211_WOWLAN_TRIG_WAKEUP_PKT_8023_LEN;
			}

			if (wakeup->packet_len &&
			    nla_put_u32(msg, len_attr, wakeup->packet_len))
				goto free_msg;

			if (nla_put(msg, pkt_attr, wakeup->packet_present_len,
				    wakeup->packet))
				goto free_msg;
		}

		nla_nest_end(msg, reasons);
	}

	genlmsg_end(msg, hdr);

	genlmsg_multicast_netns(&nl80211_fam, wiphy_net(&rdev->wiphy), msg, 0,
				NL80211_MCGRP_MLME, gfp);
	return;

 free_msg:
	nlmsg_free(msg);
}
EXPORT_SYMBOL(cfg80211_report_wowlan_wakeup);
#endif

void cfg80211_tdls_oper_request(struct net_device *dev, const u8 *peer,
				enum nl80211_tdls_operation oper,
				u16 reason_code, gfp_t gfp)
{
	struct wireless_dev *wdev = dev->ieee80211_ptr;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wdev->wiphy);
	struct sk_buff *msg;
	void *hdr;

	trace_cfg80211_tdls_oper_request(wdev->wiphy, dev, peer, oper,
					 reason_code);

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, gfp);
	if (!msg)
		return;

	hdr = nl80211hdr_put(msg, 0, 0, 0, NL80211_CMD_TDLS_OPER);
>>>>>>> refs/remotes/origin/master
	if (!hdr) {
		nlmsg_free(msg);
		return;
	}

<<<<<<< HEAD
	NLA_PUT_U32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx);
	if (freq)
		NLA_PUT_U32(msg, NL80211_ATTR_WIPHY_FREQ, freq);
	if (sig_dbm)
		NLA_PUT_U32(msg, NL80211_ATTR_RX_SIGNAL_DBM, sig_dbm);
	NLA_PUT(msg, NL80211_ATTR_FRAME, len, frame);

	genlmsg_end(msg, hdr);

	genlmsg_unicast(wiphy_net(&rdev->wiphy), msg, nlpid);
=======
	if (nla_put_u32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx) ||
	    nla_put_u32(msg, NL80211_ATTR_IFINDEX, dev->ifindex) ||
	    nla_put_u8(msg, NL80211_ATTR_TDLS_OPERATION, oper) ||
	    nla_put(msg, NL80211_ATTR_MAC, ETH_ALEN, peer) ||
	    (reason_code > 0 &&
	     nla_put_u16(msg, NL80211_ATTR_REASON_CODE, reason_code)))
		goto nla_put_failure;

	genlmsg_end(msg, hdr);

	genlmsg_multicast_netns(&nl80211_fam, wiphy_net(&rdev->wiphy), msg, 0,
				NL80211_MCGRP_MLME, gfp);
>>>>>>> refs/remotes/origin/master
	return;

 nla_put_failure:
	genlmsg_cancel(msg, hdr);
	nlmsg_free(msg);
}
<<<<<<< HEAD
EXPORT_SYMBOL(cfg80211_report_obss_beacon);
>>>>>>> refs/remotes/origin/cm-10.0
=======
EXPORT_SYMBOL(cfg80211_tdls_oper_request);
>>>>>>> refs/remotes/origin/master

static int nl80211_netlink_notify(struct notifier_block * nb,
				  unsigned long state,
				  void *_notify)
{
	struct netlink_notify *notify = _notify;
	struct cfg80211_registered_device *rdev;
	struct wireless_dev *wdev;
<<<<<<< HEAD
=======
	struct cfg80211_beacon_registration *reg, *tmp;
>>>>>>> refs/remotes/origin/master

	if (state != NETLINK_URELEASE)
		return NOTIFY_DONE;

	rcu_read_lock();

<<<<<<< HEAD
<<<<<<< HEAD
	list_for_each_entry_rcu(rdev, &cfg80211_rdev_list, list)
		list_for_each_entry_rcu(wdev, &rdev->netdev_list, list)
			cfg80211_mlme_unregister_socket(wdev, notify->pid);
=======
	list_for_each_entry_rcu(rdev, &cfg80211_rdev_list, list) {
		list_for_each_entry_rcu(wdev, &rdev->netdev_list, list)
			cfg80211_mlme_unregister_socket(wdev, notify->pid);
		if (rdev->ap_beacons_nlpid == notify->pid)
			rdev->ap_beacons_nlpid = 0;
	}
>>>>>>> refs/remotes/origin/cm-10.0
=======
	list_for_each_entry_rcu(rdev, &cfg80211_rdev_list, list) {
		list_for_each_entry_rcu(wdev, &rdev->wdev_list, list)
			cfg80211_mlme_unregister_socket(wdev, notify->portid);

		spin_lock_bh(&rdev->beacon_registrations_lock);
		list_for_each_entry_safe(reg, tmp, &rdev->beacon_registrations,
					 list) {
			if (reg->nlportid == notify->portid) {
				list_del(&reg->list);
				kfree(reg);
				break;
			}
		}
		spin_unlock_bh(&rdev->beacon_registrations_lock);
	}
>>>>>>> refs/remotes/origin/master

	rcu_read_unlock();

	return NOTIFY_DONE;
}

static struct notifier_block nl80211_netlink_notifier = {
	.notifier_call = nl80211_netlink_notify,
};

<<<<<<< HEAD
/* initialisation/exit functions */

int nl80211_init(void)
{
	int err;

	err = genl_register_family_with_ops(&nl80211_fam,
		nl80211_ops, ARRAY_SIZE(nl80211_ops));
	if (err)
		return err;

	err = genl_register_mc_group(&nl80211_fam, &nl80211_config_mcgrp);
	if (err)
		goto err_out;

	err = genl_register_mc_group(&nl80211_fam, &nl80211_scan_mcgrp);
	if (err)
		goto err_out;

	err = genl_register_mc_group(&nl80211_fam, &nl80211_regulatory_mcgrp);
	if (err)
		goto err_out;

	err = genl_register_mc_group(&nl80211_fam, &nl80211_mlme_mcgrp);
	if (err)
		goto err_out;

#ifdef CONFIG_NL80211_TESTMODE
	err = genl_register_mc_group(&nl80211_fam, &nl80211_testmode_mcgrp);
	if (err)
		goto err_out;
#endif
=======
void cfg80211_ft_event(struct net_device *netdev,
		       struct cfg80211_ft_event_params *ft_event)
{
	struct wiphy *wiphy = netdev->ieee80211_ptr->wiphy;
	struct cfg80211_registered_device *rdev = wiphy_to_dev(wiphy);
	struct sk_buff *msg;
	void *hdr;

	trace_cfg80211_ft_event(wiphy, netdev, ft_event);

	if (!ft_event->target_ap)
		return;

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (!msg)
		return;

	hdr = nl80211hdr_put(msg, 0, 0, 0, NL80211_CMD_FT_EVENT);
	if (!hdr)
		goto out;

	if (nla_put_u32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx) ||
	    nla_put_u32(msg, NL80211_ATTR_IFINDEX, netdev->ifindex) ||
	    nla_put(msg, NL80211_ATTR_MAC, ETH_ALEN, ft_event->target_ap))
		goto out;

	if (ft_event->ies &&
	    nla_put(msg, NL80211_ATTR_IE, ft_event->ies_len, ft_event->ies))
		goto out;
	if (ft_event->ric_ies &&
	    nla_put(msg, NL80211_ATTR_IE_RIC, ft_event->ric_ies_len,
		    ft_event->ric_ies))
		goto out;

	genlmsg_end(msg, hdr);

	genlmsg_multicast_netns(&nl80211_fam, wiphy_net(&rdev->wiphy), msg, 0,
				NL80211_MCGRP_MLME, GFP_KERNEL);
	return;
 out:
	nlmsg_free(msg);
}
EXPORT_SYMBOL(cfg80211_ft_event);

void cfg80211_crit_proto_stopped(struct wireless_dev *wdev, gfp_t gfp)
{
	struct cfg80211_registered_device *rdev;
	struct sk_buff *msg;
	void *hdr;
	u32 nlportid;

	rdev = wiphy_to_dev(wdev->wiphy);
	if (!rdev->crit_proto_nlportid)
		return;

	nlportid = rdev->crit_proto_nlportid;
	rdev->crit_proto_nlportid = 0;

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, gfp);
	if (!msg)
		return;

	hdr = nl80211hdr_put(msg, 0, 0, 0, NL80211_CMD_CRIT_PROTOCOL_STOP);
	if (!hdr)
		goto nla_put_failure;

	if (nla_put_u32(msg, NL80211_ATTR_WIPHY, rdev->wiphy_idx) ||
	    nla_put_u64(msg, NL80211_ATTR_WDEV, wdev_id(wdev)))
		goto nla_put_failure;

	genlmsg_end(msg, hdr);

	genlmsg_unicast(wiphy_net(&rdev->wiphy), msg, nlportid);
	return;

 nla_put_failure:
	if (hdr)
		genlmsg_cancel(msg, hdr);
	nlmsg_free(msg);

}
EXPORT_SYMBOL(cfg80211_crit_proto_stopped);

/* initialisation/exit functions */

int nl80211_init(void)
{
	int err;

	err = genl_register_family_with_ops_groups(&nl80211_fam, nl80211_ops,
						   nl80211_mcgrps);
	if (err)
		return err;
>>>>>>> refs/remotes/origin/master

	err = netlink_register_notifier(&nl80211_netlink_notifier);
	if (err)
		goto err_out;

	return 0;
 err_out:
	genl_unregister_family(&nl80211_fam);
	return err;
}

void nl80211_exit(void)
{
	netlink_unregister_notifier(&nl80211_netlink_notifier);
	genl_unregister_family(&nl80211_fam);
}
