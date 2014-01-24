/*
 * Copyright 2004, Instant802 Networks, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/module.h>
#include <linux/if_arp.h>
#include <linux/types.h>
#include <net/ip.h>
#include <net/pkt_sched.h>

#include <net/mac80211.h>
#include "ieee80211_i.h"
#include "wme.h"

/* Default mapping in classifier to work with default
 * queue setup.
 */
const int ieee802_1d_to_ac[8] = {
	IEEE80211_AC_BE,
	IEEE80211_AC_BK,
	IEEE80211_AC_BK,
	IEEE80211_AC_BE,
	IEEE80211_AC_VI,
	IEEE80211_AC_VI,
	IEEE80211_AC_VO,
	IEEE80211_AC_VO
};

static int wme_downgrade_ac(struct sk_buff *skb)
{
	switch (skb->priority) {
	case 6:
	case 7:
		skb->priority = 5; /* VO -> VI */
		return 0;
	case 4:
	case 5:
		skb->priority = 3; /* VI -> BE */
		return 0;
	case 0:
	case 3:
		skb->priority = 2; /* BE -> BK */
		return 0;
	default:
		return -1;
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
/* Indicate which queue to use for this fully formed 802.11 frame */
u16 ieee80211_select_queue_80211(struct ieee80211_local *local,
				 struct sk_buff *skb,
				 struct ieee80211_hdr *hdr)
{
	u8 *p;

	if (local->hw.queues < 4)
=======
static u16 ieee80211_downgrade_queue(struct ieee80211_sub_if_data *sdata,
				     struct sk_buff *skb)
{
	/* in case we are a client verify acm is not set for this ac */
	while (unlikely(sdata->wmm_acm & BIT(skb->priority))) {
		if (wme_downgrade_ac(skb)) {
			/*
			 * This should not really happen. The AP has marked all
			 * lower ACs to require admission control which is not
			 * a reasonable configuration. Allow the frame to be
			 * transmitted using AC_BK as a workaround.
			 */
			break;
		}
	}

	/* look up which queue to use for frames with this 1d tag */
	return ieee802_1d_to_ac[skb->priority];
}

/* Indicate which queue to use for this fully formed 802.11 frame */
u16 ieee80211_select_queue_80211(struct ieee80211_sub_if_data *sdata,
				 struct sk_buff *skb,
				 struct ieee80211_hdr *hdr)
{
	struct ieee80211_local *local = sdata->local;
	u8 *p;

	if (local->hw.queues < IEEE80211_NUM_ACS)
>>>>>>> refs/remotes/origin/master
		return 0;

	if (!ieee80211_is_data(hdr->frame_control)) {
		skb->priority = 7;
		return ieee802_1d_to_ac[skb->priority];
	}
	if (!ieee80211_is_data_qos(hdr->frame_control)) {
		skb->priority = 0;
		return ieee802_1d_to_ac[skb->priority];
	}

	p = ieee80211_get_qos_ctl(hdr);
	skb->priority = *p & IEEE80211_QOS_CTL_TAG1D_MASK;

<<<<<<< HEAD
	return ieee80211_downgrade_queue(local, skb);
}
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return ieee80211_downgrade_queue(sdata, skb);
}
>>>>>>> refs/remotes/origin/master

/* Indicate which queue to use. */
u16 ieee80211_select_queue(struct ieee80211_sub_if_data *sdata,
			   struct sk_buff *skb)
{
	struct ieee80211_local *local = sdata->local;
	struct sta_info *sta = NULL;
	const u8 *ra = NULL;
	bool qos = false;

<<<<<<< HEAD
	if (local->hw.queues < 4 || skb->len < 6) {
		skb->priority = 0; /* required for correct WPA/11i MIC */
		return min_t(u16, local->hw.queues - 1, IEEE80211_AC_BE);
=======
	if (local->hw.queues < IEEE80211_NUM_ACS || skb->len < 6) {
		skb->priority = 0; /* required for correct WPA/11i MIC */
		return 0;
>>>>>>> refs/remotes/origin/master
	}

	rcu_read_lock();
	switch (sdata->vif.type) {
	case NL80211_IFTYPE_AP_VLAN:
		sta = rcu_dereference(sdata->u.vlan.sta);
		if (sta) {
<<<<<<< HEAD
<<<<<<< HEAD
			qos = get_sta_flags(sta) & WLAN_STA_WME;
=======
			qos = test_sta_flag(sta, WLAN_STA_WME);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			qos = test_sta_flag(sta, WLAN_STA_WME);
>>>>>>> refs/remotes/origin/master
			break;
		}
	case NL80211_IFTYPE_AP:
		ra = skb->data;
		break;
	case NL80211_IFTYPE_WDS:
		ra = sdata->u.wds.remote_addr;
		break;
#ifdef CONFIG_MAC80211_MESH
	case NL80211_IFTYPE_MESH_POINT:
<<<<<<< HEAD
<<<<<<< HEAD
		/*
		 * XXX: This is clearly broken ... but already was before,
		 * because ieee80211_fill_mesh_addresses() would clear A1
		 * except for multicast addresses.
		 */
=======
		qos = true;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		qos = true;
>>>>>>> refs/remotes/origin/master
		break;
#endif
	case NL80211_IFTYPE_STATION:
		ra = sdata->u.mgd.bssid;
		break;
	case NL80211_IFTYPE_ADHOC:
		ra = skb->data;
		break;
	default:
		break;
	}

	if (!sta && ra && !is_multicast_ether_addr(ra)) {
		sta = sta_info_get(sdata, ra);
		if (sta)
<<<<<<< HEAD
<<<<<<< HEAD
			qos = get_sta_flags(sta) & WLAN_STA_WME;
=======
			qos = test_sta_flag(sta, WLAN_STA_WME);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			qos = test_sta_flag(sta, WLAN_STA_WME);
>>>>>>> refs/remotes/origin/master
	}
	rcu_read_unlock();

	if (!qos) {
		skb->priority = 0; /* required for correct WPA/11i MIC */
		return IEEE80211_AC_BE;
	}

	/* use the data classifier to determine what 802.1d tag the
	 * data frame has */
	skb->priority = cfg80211_classify8021d(skb);

<<<<<<< HEAD
	return ieee80211_downgrade_queue(local, skb);
}

u16 ieee80211_downgrade_queue(struct ieee80211_local *local,
			      struct sk_buff *skb)
{
	/* in case we are a client verify acm is not set for this ac */
	while (unlikely(local->wmm_acm & BIT(skb->priority))) {
		if (wme_downgrade_ac(skb)) {
			/*
			 * This should not really happen. The AP has marked all
			 * lower ACs to require admission control which is not
			 * a reasonable configuration. Allow the frame to be
			 * transmitted using AC_BK as a workaround.
			 */
			break;
		}
	}

	/* look up which queue to use for frames with this 1d tag */
	return ieee802_1d_to_ac[skb->priority];
}

<<<<<<< HEAD
void ieee80211_set_qos_hdr(struct ieee80211_local *local, struct sk_buff *skb)
{
	struct ieee80211_hdr *hdr = (void *)skb->data;
=======
=======
	return ieee80211_downgrade_queue(sdata, skb);
}

/**
 * ieee80211_set_qos_hdr - Fill in the QoS header if there is one.
 *
 * @sdata: local subif
 * @skb: packet to be updated
 */
>>>>>>> refs/remotes/origin/master
void ieee80211_set_qos_hdr(struct ieee80211_sub_if_data *sdata,
			   struct sk_buff *skb)
{
	struct ieee80211_hdr *hdr = (void *)skb->data;
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

	/* Fill in the QoS header if there is one. */
	if (ieee80211_is_data_qos(hdr->frame_control)) {
		u8 *p = ieee80211_get_qos_ctl(hdr);
<<<<<<< HEAD
		u8 ack_policy = 0, tid;

		tid = skb->priority & IEEE80211_QOS_CTL_TAG1D_MASK;

		if (unlikely(local->wifi_wme_noack_test))
			ack_policy |= QOS_CONTROL_ACK_POLICY_NOACK <<
					QOS_CONTROL_ACK_POLICY_SHIFT;
		/* qos header is 2 bytes, second reserved */
		*p++ = ack_policy | tid;
		*p = 0;
=======
		u8 ack_policy, tid;

		tid = skb->priority & IEEE80211_QOS_CTL_TAG1D_MASK;

		/* preserve EOSP bit */
		ack_policy = *p & IEEE80211_QOS_CTL_EOSP;

		if (is_multicast_ether_addr(hdr->addr1) ||
		    sdata->noack_map & BIT(tid)) {
			ack_policy |= IEEE80211_QOS_CTL_ACK_POLICY_NOACK;
			info->flags |= IEEE80211_TX_CTL_NO_ACK;
		}

		/* qos header is 2 bytes */
		*p++ = ack_policy | tid;
		*p = ieee80211_vif_is_mesh(&sdata->vif) ?
			(IEEE80211_QOS_CTL_MESH_CONTROL_PRESENT >> 8) : 0;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	u8 *p;
	u8 ack_policy, tid;

	if (!ieee80211_is_data_qos(hdr->frame_control))
		return;

	p = ieee80211_get_qos_ctl(hdr);
	tid = skb->priority & IEEE80211_QOS_CTL_TAG1D_MASK;

	/* preserve EOSP bit */
	ack_policy = *p & IEEE80211_QOS_CTL_EOSP;

	if (is_multicast_ether_addr(hdr->addr1) ||
	    sdata->noack_map & BIT(tid)) {
		ack_policy |= IEEE80211_QOS_CTL_ACK_POLICY_NOACK;
		info->flags |= IEEE80211_TX_CTL_NO_ACK;
	}

	/* qos header is 2 bytes */
	*p++ = ack_policy | tid;
	if (ieee80211_vif_is_mesh(&sdata->vif)) {
		/* preserve RSPI and Mesh PS Level bit */
		*p &= ((IEEE80211_QOS_CTL_RSPI |
			IEEE80211_QOS_CTL_MESH_PS_LEVEL) >> 8);

		/* Nulls don't have a mesh header (frame body) */
		if (!ieee80211_is_qos_nullfunc(hdr->frame_control))
			*p |= (IEEE80211_QOS_CTL_MESH_CONTROL_PRESENT >> 8);
	} else {
		*p = 0;
>>>>>>> refs/remotes/origin/master
	}
}
