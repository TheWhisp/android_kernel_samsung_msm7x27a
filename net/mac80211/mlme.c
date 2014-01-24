/*
 * BSS client mode implementation
 * Copyright 2003-2008, Jouni Malinen <j@w1.fi>
 * Copyright 2004, Instant802 Networks, Inc.
 * Copyright 2005, Devicescape Software, Inc.
 * Copyright 2006-2007	Jiri Benc <jbenc@suse.cz>
 * Copyright 2007, Michael Wu <flamingice@sourmilk.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/delay.h>
#include <linux/if_ether.h>
#include <linux/skbuff.h>
#include <linux/if_arp.h>
#include <linux/etherdevice.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/rtnetlink.h>
#include <linux/pm_qos_params.h>
#include <linux/crc32.h>
#include <linux/slab.h>
=======
=======
>>>>>>> refs/remotes/origin/master
#include <linux/moduleparam.h>
#include <linux/rtnetlink.h>
#include <linux/pm_qos.h>
#include <linux/crc32.h>
#include <linux/slab.h>
#include <linux/export.h>
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#include <net/mac80211.h>
#include <asm/unaligned.h>

#include "ieee80211_i.h"
#include "driver-ops.h"
#include "rate.h"
#include "led.h"

<<<<<<< HEAD
<<<<<<< HEAD
=======
#define IEEE80211_AUTH_TIMEOUT (HZ / 5)
#define IEEE80211_AUTH_MAX_TRIES 3
#define IEEE80211_AUTH_WAIT_ASSOC (HZ * 5)
#define IEEE80211_ASSOC_TIMEOUT (HZ / 5)
#define IEEE80211_ASSOC_MAX_TRIES 3

>>>>>>> refs/remotes/origin/cm-10.0
=======
#define IEEE80211_AUTH_TIMEOUT		(HZ / 5)
#define IEEE80211_AUTH_TIMEOUT_LONG	(HZ / 2)
#define IEEE80211_AUTH_TIMEOUT_SHORT	(HZ / 10)
#define IEEE80211_AUTH_MAX_TRIES	3
#define IEEE80211_AUTH_WAIT_ASSOC	(HZ * 5)
#define IEEE80211_ASSOC_TIMEOUT		(HZ / 5)
#define IEEE80211_ASSOC_TIMEOUT_LONG	(HZ / 2)
#define IEEE80211_ASSOC_TIMEOUT_SHORT	(HZ / 10)
#define IEEE80211_ASSOC_MAX_TRIES	3

>>>>>>> refs/remotes/origin/master
static int max_nullfunc_tries = 2;
module_param(max_nullfunc_tries, int, 0644);
MODULE_PARM_DESC(max_nullfunc_tries,
		 "Maximum nullfunc tx tries before disconnecting (reason 4).");

static int max_probe_tries = 5;
module_param(max_probe_tries, int, 0644);
MODULE_PARM_DESC(max_probe_tries,
		 "Maximum probe tries before disconnecting (reason 4).");

/*
 * Beacon loss timeout is calculated as N frames times the
 * advertised beacon interval.  This may need to be somewhat
 * higher than what hardware might detect to account for
 * delays in the host processing frames. But since we also
 * probe on beacon miss before declaring the connection lost
 * default to what we want.
 */
<<<<<<< HEAD
#define IEEE80211_BEACON_LOSS_COUNT	7
=======
static int beacon_loss_count = 7;
module_param(beacon_loss_count, int, 0644);
MODULE_PARM_DESC(beacon_loss_count,
		 "Number of beacon intervals before we decide beacon was lost.");
>>>>>>> refs/remotes/origin/master

/*
 * Time the connection can be idle before we probe
 * it to see if we can still talk to the AP.
 */
#define IEEE80211_CONNECTION_IDLE_TIME	(30 * HZ)
/*
 * Time we wait for a probe response after sending
 * a probe request because of beacon loss or for
 * checking the connection still works.
 */
static int probe_wait_ms = 500;
module_param(probe_wait_ms, int, 0644);
MODULE_PARM_DESC(probe_wait_ms,
		 "Maximum time(ms) to wait for probe response"
		 " before disconnecting (reason 4).");

/*
 * Weight given to the latest Beacon frame when calculating average signal
 * strength for Beacon frames received in the current BSS. This must be
 * between 1 and 15.
 */
#define IEEE80211_SIGNAL_AVE_WEIGHT	3

/*
 * How many Beacon frames need to have been used in average signal strength
 * before starting to indicate signal change events.
 */
#define IEEE80211_SIGNAL_AVE_MIN_COUNT	4

<<<<<<< HEAD
#define TMR_RUNNING_TIMER	0
#define TMR_RUNNING_CHANSW	1

<<<<<<< HEAD
=======
#define DEAUTH_DISASSOC_LEN	(24 /* hdr */ + 2 /* reason */)

>>>>>>> refs/remotes/origin/cm-10.0
/*
 * All cfg80211 functions have to be called outside a locked
 * section so that they can acquire a lock themselves... This
 * is much simpler than queuing up things in cfg80211, but we
 * do need some indirection for that here.
 */
enum rx_mgmt_action {
	/* no action required */
	RX_MGMT_NONE,

	/* caller must call cfg80211_send_deauth() */
	RX_MGMT_CFG80211_DEAUTH,

	/* caller must call cfg80211_send_disassoc() */
	RX_MGMT_CFG80211_DISASSOC,
<<<<<<< HEAD
=======

	/* caller must call cfg80211_send_rx_auth() */
	RX_MGMT_CFG80211_RX_AUTH,

	/* caller must call cfg80211_send_rx_assoc() */
	RX_MGMT_CFG80211_RX_ASSOC,

	/* caller must call cfg80211_send_assoc_timeout() */
	RX_MGMT_CFG80211_ASSOC_TIMEOUT,
>>>>>>> refs/remotes/origin/cm-10.0
};

/* utils */
static inline void ASSERT_MGD_MTX(struct ieee80211_if_managed *ifmgd)
{
	lockdep_assert_held(&ifmgd->mtx);
}

=======
>>>>>>> refs/remotes/origin/master
/*
 * We can have multiple work items (and connection probing)
 * scheduling this timer, but we need to take care to only
 * reschedule it when it should fire _earlier_ than it was
 * asked for before, or if it's not pending right now. This
 * function ensures that. Note that it then is required to
 * run this function for all timeouts after the first one
 * has happened -- the work that runs from this timer will
 * do that.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static void run_again(struct ieee80211_if_managed *ifmgd,
			     unsigned long timeout)
=======
static void run_again(struct ieee80211_if_managed *ifmgd, unsigned long timeout)
>>>>>>> refs/remotes/origin/cm-10.0
{
	ASSERT_MGD_MTX(ifmgd);

	if (!timer_pending(&ifmgd->timer) ||
	    time_before(timeout, ifmgd->timer.expires))
		mod_timer(&ifmgd->timer, timeout);
=======
static void run_again(struct ieee80211_sub_if_data *sdata,
		      unsigned long timeout)
{
	sdata_assert_lock(sdata);

	if (!timer_pending(&sdata->u.mgd.timer) ||
	    time_before(timeout, sdata->u.mgd.timer.expires))
		mod_timer(&sdata->u.mgd.timer, timeout);
>>>>>>> refs/remotes/origin/master
}

void ieee80211_sta_reset_beacon_monitor(struct ieee80211_sub_if_data *sdata)
{
<<<<<<< HEAD
<<<<<<< HEAD
	if (sdata->local->hw.flags & IEEE80211_HW_BEACON_FILTER)
=======
	if (sdata->vif.driver_flags & IEEE80211_VIF_BEACON_FILTER)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (sdata->vif.driver_flags & IEEE80211_VIF_BEACON_FILTER)
		return;

	if (sdata->local->hw.flags & IEEE80211_HW_CONNECTION_MONITOR)
>>>>>>> refs/remotes/origin/master
		return;

	mod_timer(&sdata->u.mgd.bcn_mon_timer,
		  round_jiffies_up(jiffies + sdata->u.mgd.beacon_timeout));
}

void ieee80211_sta_reset_conn_monitor(struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;

	if (unlikely(!sdata->u.mgd.associated))
		return;

	if (sdata->local->hw.flags & IEEE80211_HW_CONNECTION_MONITOR)
		return;

	mod_timer(&sdata->u.mgd.conn_mon_timer,
		  round_jiffies_up(jiffies + IEEE80211_CONNECTION_IDLE_TIME));

	ifmgd->probe_send_count = 0;
}

static int ecw2cw(int ecw)
{
	return (1 << ecw) - 1;
}

<<<<<<< HEAD
/*
 * ieee80211_enable_ht should be called only after the operating band
 * has been determined as ht configuration depends on the hw's
 * HT abilities for a specific band.
 */
static u32 ieee80211_enable_ht(struct ieee80211_sub_if_data *sdata,
			       struct ieee80211_ht_info *hti,
<<<<<<< HEAD
			       const u8 *bssid, u16 ap_ht_cap_flags)
=======
			       const u8 *bssid, u16 ap_ht_cap_flags,
			       bool beacon_htcap_ie)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_supported_band *sband;
	struct sta_info *sta;
	u32 changed = 0;
	int hti_cfreq;
	u16 ht_opmode;
<<<<<<< HEAD
	bool enable_ht = true;
	enum nl80211_channel_type prev_chantype;
	enum nl80211_channel_type channel_type = NL80211_CHAN_NO_HT;

	sband = local->hw.wiphy->bands[local->hw.conf.channel->band];

	prev_chantype = sdata->vif.bss_conf.channel_type;

	/* HT is not supported */
	if (!sband->ht_cap.ht_supported)
		enable_ht = false;

	if (enable_ht) {
		hti_cfreq = ieee80211_channel_to_frequency(hti->control_chan,
							   sband->band);
		/* check that channel matches the right operating channel */
		if (local->hw.conf.channel->center_freq != hti_cfreq) {
			/* Some APs mess this up, evidently.
			 * Netgear WNDR3700 sometimes reports 4 higher than
			 * the actual channel, for instance.
			 */
			printk(KERN_DEBUG
			       "%s: Wrong control channel in association"
			       " response: configured center-freq: %d"
			       " hti-cfreq: %d  hti->control_chan: %d"
			       " band: %d.  Disabling HT.\n",
			       sdata->name,
			       local->hw.conf.channel->center_freq,
			       hti_cfreq, hti->control_chan,
			       sband->band);
			enable_ht = false;
		}
	}

	if (enable_ht) {
		channel_type = NL80211_CHAN_HT20;

		if (!(ap_ht_cap_flags & IEEE80211_HT_CAP_40MHZ_INTOLERANT) &&
=======
	bool enable_ht = true, queues_stopped = false;
	enum nl80211_channel_type prev_chantype;
	enum nl80211_channel_type rx_channel_type = NL80211_CHAN_NO_HT;
	enum nl80211_channel_type tx_channel_type;

	sband = local->hw.wiphy->bands[local->hw.conf.channel->band];
	prev_chantype = sdata->vif.bss_conf.channel_type;


	hti_cfreq = ieee80211_channel_to_frequency(hti->control_chan,
						   sband->band);
	/* check that channel matches the right operating channel */
	if (local->hw.conf.channel->center_freq != hti_cfreq) {
		/* Some APs mess this up, evidently.
		 * Netgear WNDR3700 sometimes reports 4 higher than
		 * the actual channel, for instance.
		 */
		printk(KERN_DEBUG
		       "%s: Wrong control channel in association"
		       " response: configured center-freq: %d"
		       " hti-cfreq: %d  hti->control_chan: %d"
		       " band: %d.  Disabling HT.\n",
		       sdata->name,
		       local->hw.conf.channel->center_freq,
		       hti_cfreq, hti->control_chan,
		       sband->band);
		enable_ht = false;
	}

	if (enable_ht) {
		rx_channel_type = NL80211_CHAN_HT20;

		if (!(ap_ht_cap_flags & IEEE80211_HT_CAP_40MHZ_INTOLERANT) &&
		    !ieee80111_cfg_override_disables_ht40(sdata) &&
>>>>>>> refs/remotes/origin/cm-10.0
		    (sband->ht_cap.cap & IEEE80211_HT_CAP_SUP_WIDTH_20_40) &&
		    (hti->ht_param & IEEE80211_HT_PARAM_CHAN_WIDTH_ANY)) {
			switch(hti->ht_param & IEEE80211_HT_PARAM_CHA_SEC_OFFSET) {
			case IEEE80211_HT_PARAM_CHA_SEC_ABOVE:
<<<<<<< HEAD
				if (!(local->hw.conf.channel->flags &
				    IEEE80211_CHAN_NO_HT40PLUS))
					channel_type = NL80211_CHAN_HT40PLUS;
				break;
			case IEEE80211_HT_PARAM_CHA_SEC_BELOW:
				if (!(local->hw.conf.channel->flags &
				    IEEE80211_CHAN_NO_HT40MINUS))
					channel_type = NL80211_CHAN_HT40MINUS;
=======
				rx_channel_type = NL80211_CHAN_HT40PLUS;
				break;
			case IEEE80211_HT_PARAM_CHA_SEC_BELOW:
				rx_channel_type = NL80211_CHAN_HT40MINUS;
>>>>>>> refs/remotes/origin/cm-10.0
				break;
			}
		}
	}

<<<<<<< HEAD
	if (local->tmp_channel)
		local->tmp_channel_type = channel_type;

	if (!ieee80211_set_channel_type(local, sdata, channel_type)) {
		/* can only fail due to HT40+/- mismatch */
		channel_type = NL80211_CHAN_HT20;
		WARN_ON(!ieee80211_set_channel_type(local, sdata, channel_type));
=======
	tx_channel_type = ieee80211_get_tx_channel_type(local, rx_channel_type);

	if (local->tmp_channel)
		local->tmp_channel_type = rx_channel_type;

	if (!ieee80211_set_channel_type(local, sdata, rx_channel_type)) {
		/* can only fail due to HT40+/- mismatch */
		rx_channel_type = NL80211_CHAN_HT20;
		WARN_ON(!ieee80211_set_channel_type(local, sdata,
						    rx_channel_type));
	}

	if (beacon_htcap_ie && (prev_chantype != rx_channel_type)) {
		/*
		 * Whenever the AP announces the HT mode change that can be
		 * 40MHz intolerant or etc., it would be safer to stop tx
		 * queues before doing hw config to avoid buffer overflow.
		 */
		ieee80211_stop_queues_by_reason(&sdata->local->hw,
				IEEE80211_QUEUE_STOP_REASON_CHTYPE_CHANGE);
		queues_stopped = true;

		/* flush out all packets */
		synchronize_net();

		drv_flush(local, false);
>>>>>>> refs/remotes/origin/cm-10.0
	}

	/* channel_type change automatically detected */
	ieee80211_hw_config(local, 0);

<<<<<<< HEAD
	if (prev_chantype != channel_type) {
=======
	if (prev_chantype != tx_channel_type) {
>>>>>>> refs/remotes/origin/cm-10.0
		rcu_read_lock();
		sta = sta_info_get(sdata, bssid);
		if (sta)
			rate_control_rate_update(local, sband, sta,
						 IEEE80211_RC_HT_CHANGED,
<<<<<<< HEAD
						 channel_type);
		rcu_read_unlock();
	}

=======
						 tx_channel_type);
		rcu_read_unlock();
	}

	if (queues_stopped)
		ieee80211_wake_queues_by_reason(&sdata->local->hw,
			IEEE80211_QUEUE_STOP_REASON_CHTYPE_CHANGE);

>>>>>>> refs/remotes/origin/cm-10.0
	ht_opmode = le16_to_cpu(hti->operation_mode);

	/* if bss configuration changed store the new one */
	if (sdata->ht_opmode_valid != enable_ht ||
	    sdata->vif.bss_conf.ht_operation_mode != ht_opmode ||
<<<<<<< HEAD
	    prev_chantype != channel_type) {
=======
	    prev_chantype != rx_channel_type) {
>>>>>>> refs/remotes/origin/cm-10.0
		changed |= BSS_CHANGED_HT;
		sdata->vif.bss_conf.ht_operation_mode = ht_opmode;
		sdata->ht_opmode_valid = enable_ht;
	}

	return changed;
}

/* frame sending functions */

<<<<<<< HEAD
static void ieee80211_send_deauth_disassoc(struct ieee80211_sub_if_data *sdata,
					   const u8 *bssid, u16 stype, u16 reason,
					   void *cookie, bool send_frame)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct sk_buff *skb;
	struct ieee80211_mgmt *mgmt;

	skb = dev_alloc_skb(local->hw.extra_tx_headroom + sizeof(*mgmt));
	if (!skb) {
		printk(KERN_DEBUG "%s: failed to allocate buffer for "
		       "deauth/disassoc frame\n", sdata->name);
		return;
	}
	skb_reserve(skb, local->hw.extra_tx_headroom);

	mgmt = (struct ieee80211_mgmt *) skb_put(skb, 24);
	memset(mgmt, 0, 24);
	memcpy(mgmt->da, bssid, ETH_ALEN);
	memcpy(mgmt->sa, sdata->vif.addr, ETH_ALEN);
	memcpy(mgmt->bssid, bssid, ETH_ALEN);
	mgmt->frame_control = cpu_to_le16(IEEE80211_FTYPE_MGMT | stype);
	skb_put(skb, 2);
	/* u.deauth.reason_code == u.disassoc.reason_code */
	mgmt->u.deauth.reason_code = cpu_to_le16(reason);

	if (stype == IEEE80211_STYPE_DEAUTH)
		if (cookie)
			__cfg80211_send_deauth(sdata->dev, (u8 *)mgmt, skb->len);
		else
			cfg80211_send_deauth(sdata->dev, (u8 *)mgmt, skb->len);
	else
		if (cookie)
			__cfg80211_send_disassoc(sdata->dev, (u8 *)mgmt, skb->len);
		else
			cfg80211_send_disassoc(sdata->dev, (u8 *)mgmt, skb->len);
	if (!(ifmgd->flags & IEEE80211_STA_MFP_ENABLED))
		IEEE80211_SKB_CB(skb)->flags |= IEEE80211_TX_INTFL_DONT_ENCRYPT;

	if (send_frame)
		ieee80211_tx_skb(sdata, skb);
	else
		kfree_skb(skb);
=======
static int ieee80211_compatible_rates(const u8 *supp_rates, int supp_rates_len,
				      struct ieee80211_supported_band *sband,
				      u32 *rates)
{
	int i, j, count;
	*rates = 0;
	count = 0;
	for (i = 0; i < supp_rates_len; i++) {
		int rate = (supp_rates[i] & 0x7F) * 5;

		for (j = 0; j < sband->n_bitrates; j++)
			if (sband->bitrates[j].bitrate == rate) {
				*rates |= BIT(j);
				count++;
				break;
			}
	}

	return count;
}

static void ieee80211_add_ht_ie(struct ieee80211_sub_if_data *sdata,
				struct sk_buff *skb, const u8 *ht_info_ie,
=======
static u32
ieee80211_determine_chantype(struct ieee80211_sub_if_data *sdata,
			     struct ieee80211_supported_band *sband,
			     struct ieee80211_channel *channel,
			     const struct ieee80211_ht_operation *ht_oper,
			     const struct ieee80211_vht_operation *vht_oper,
			     struct cfg80211_chan_def *chandef, bool tracking)
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct cfg80211_chan_def vht_chandef;
	u32 ht_cfreq, ret;

	chandef->chan = channel;
	chandef->width = NL80211_CHAN_WIDTH_20_NOHT;
	chandef->center_freq1 = channel->center_freq;
	chandef->center_freq2 = 0;

	if (!ht_oper || !sband->ht_cap.ht_supported) {
		ret = IEEE80211_STA_DISABLE_HT | IEEE80211_STA_DISABLE_VHT;
		goto out;
	}

	chandef->width = NL80211_CHAN_WIDTH_20;

	ht_cfreq = ieee80211_channel_to_frequency(ht_oper->primary_chan,
						  channel->band);
	/* check that channel matches the right operating channel */
	if (!tracking && channel->center_freq != ht_cfreq) {
		/*
		 * It's possible that some APs are confused here;
		 * Netgear WNDR3700 sometimes reports 4 higher than
		 * the actual channel in association responses, but
		 * since we look at probe response/beacon data here
		 * it should be OK.
		 */
		sdata_info(sdata,
			   "Wrong control channel: center-freq: %d ht-cfreq: %d ht->primary_chan: %d band: %d - Disabling HT\n",
			   channel->center_freq, ht_cfreq,
			   ht_oper->primary_chan, channel->band);
		ret = IEEE80211_STA_DISABLE_HT | IEEE80211_STA_DISABLE_VHT;
		goto out;
	}

	/* check 40 MHz support, if we have it */
	if (sband->ht_cap.cap & IEEE80211_HT_CAP_SUP_WIDTH_20_40) {
		switch (ht_oper->ht_param & IEEE80211_HT_PARAM_CHA_SEC_OFFSET) {
		case IEEE80211_HT_PARAM_CHA_SEC_ABOVE:
			chandef->width = NL80211_CHAN_WIDTH_40;
			chandef->center_freq1 += 10;
			break;
		case IEEE80211_HT_PARAM_CHA_SEC_BELOW:
			chandef->width = NL80211_CHAN_WIDTH_40;
			chandef->center_freq1 -= 10;
			break;
		}
	} else {
		/* 40 MHz (and 80 MHz) must be supported for VHT */
		ret = IEEE80211_STA_DISABLE_VHT;
		/* also mark 40 MHz disabled */
		ret |= IEEE80211_STA_DISABLE_40MHZ;
		goto out;
	}

	if (!vht_oper || !sband->vht_cap.vht_supported) {
		ret = IEEE80211_STA_DISABLE_VHT;
		goto out;
	}

	vht_chandef.chan = channel;
	vht_chandef.center_freq1 =
		ieee80211_channel_to_frequency(vht_oper->center_freq_seg1_idx,
					       channel->band);
	vht_chandef.center_freq2 = 0;

	switch (vht_oper->chan_width) {
	case IEEE80211_VHT_CHANWIDTH_USE_HT:
		vht_chandef.width = chandef->width;
		break;
	case IEEE80211_VHT_CHANWIDTH_80MHZ:
		vht_chandef.width = NL80211_CHAN_WIDTH_80;
		break;
	case IEEE80211_VHT_CHANWIDTH_160MHZ:
		vht_chandef.width = NL80211_CHAN_WIDTH_160;
		break;
	case IEEE80211_VHT_CHANWIDTH_80P80MHZ:
		vht_chandef.width = NL80211_CHAN_WIDTH_80P80;
		vht_chandef.center_freq2 =
			ieee80211_channel_to_frequency(
				vht_oper->center_freq_seg2_idx,
				channel->band);
		break;
	default:
		if (!(ifmgd->flags & IEEE80211_STA_DISABLE_VHT))
			sdata_info(sdata,
				   "AP VHT operation IE has invalid channel width (%d), disable VHT\n",
				   vht_oper->chan_width);
		ret = IEEE80211_STA_DISABLE_VHT;
		goto out;
	}

	if (!cfg80211_chandef_valid(&vht_chandef)) {
		if (!(ifmgd->flags & IEEE80211_STA_DISABLE_VHT))
			sdata_info(sdata,
				   "AP VHT information is invalid, disable VHT\n");
		ret = IEEE80211_STA_DISABLE_VHT;
		goto out;
	}

	if (cfg80211_chandef_identical(chandef, &vht_chandef)) {
		ret = 0;
		goto out;
	}

	if (!cfg80211_chandef_compatible(chandef, &vht_chandef)) {
		if (!(ifmgd->flags & IEEE80211_STA_DISABLE_VHT))
			sdata_info(sdata,
				   "AP VHT information doesn't match HT, disable VHT\n");
		ret = IEEE80211_STA_DISABLE_VHT;
		goto out;
	}

	*chandef = vht_chandef;

	ret = 0;

out:
	/* don't print the message below for VHT mismatch if VHT is disabled */
	if (ret & IEEE80211_STA_DISABLE_VHT)
		vht_chandef = *chandef;

	/*
	 * Ignore the DISABLED flag when we're already connected and only
	 * tracking the APs beacon for bandwidth changes - otherwise we
	 * might get disconnected here if we connect to an AP, update our
	 * regulatory information based on the AP's country IE and the
	 * information we have is wrong/outdated and disables the channel
	 * that we're actually using for the connection to the AP.
	 */
	while (!cfg80211_chandef_usable(sdata->local->hw.wiphy, chandef,
					tracking ? 0 :
						   IEEE80211_CHAN_DISABLED)) {
		if (WARN_ON(chandef->width == NL80211_CHAN_WIDTH_20_NOHT)) {
			ret = IEEE80211_STA_DISABLE_HT |
			      IEEE80211_STA_DISABLE_VHT;
			break;
		}

		ret |= ieee80211_chandef_downgrade(chandef);
	}

	if (chandef->width != vht_chandef.width && !tracking)
		sdata_info(sdata,
			   "capabilities/regulatory prevented using AP HT/VHT configuration, downgraded\n");

	WARN_ON_ONCE(!cfg80211_chandef_valid(chandef));
	return ret;
}

static int ieee80211_config_bw(struct ieee80211_sub_if_data *sdata,
			       struct sta_info *sta,
			       const struct ieee80211_ht_operation *ht_oper,
			       const struct ieee80211_vht_operation *vht_oper,
			       const u8 *bssid, u32 *changed)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct ieee80211_supported_band *sband;
	struct ieee80211_channel *chan;
	struct cfg80211_chan_def chandef;
	u16 ht_opmode;
	u32 flags;
	enum ieee80211_sta_rx_bandwidth new_sta_bw;
	int ret;

	/* if HT was/is disabled, don't track any bandwidth changes */
	if (ifmgd->flags & IEEE80211_STA_DISABLE_HT || !ht_oper)
		return 0;

	/* don't check VHT if we associated as non-VHT station */
	if (ifmgd->flags & IEEE80211_STA_DISABLE_VHT)
		vht_oper = NULL;

	if (WARN_ON_ONCE(!sta))
		return -EINVAL;

	chan = sdata->vif.bss_conf.chandef.chan;
	sband = local->hw.wiphy->bands[chan->band];

	/* calculate new channel (type) based on HT/VHT operation IEs */
	flags = ieee80211_determine_chantype(sdata, sband, chan, ht_oper,
					     vht_oper, &chandef, true);

	/*
	 * Downgrade the new channel if we associated with restricted
	 * capabilities. For example, if we associated as a 20 MHz STA
	 * to a 40 MHz AP (due to regulatory, capabilities or config
	 * reasons) then switching to a 40 MHz channel now won't do us
	 * any good -- we couldn't use it with the AP.
	 */
	if (ifmgd->flags & IEEE80211_STA_DISABLE_80P80MHZ &&
	    chandef.width == NL80211_CHAN_WIDTH_80P80)
		flags |= ieee80211_chandef_downgrade(&chandef);
	if (ifmgd->flags & IEEE80211_STA_DISABLE_160MHZ &&
	    chandef.width == NL80211_CHAN_WIDTH_160)
		flags |= ieee80211_chandef_downgrade(&chandef);
	if (ifmgd->flags & IEEE80211_STA_DISABLE_40MHZ &&
	    chandef.width > NL80211_CHAN_WIDTH_20)
		flags |= ieee80211_chandef_downgrade(&chandef);

	if (cfg80211_chandef_identical(&chandef, &sdata->vif.bss_conf.chandef))
		return 0;

	sdata_info(sdata,
		   "AP %pM changed bandwidth, new config is %d MHz, width %d (%d/%d MHz)\n",
		   ifmgd->bssid, chandef.chan->center_freq, chandef.width,
		   chandef.center_freq1, chandef.center_freq2);

	if (flags != (ifmgd->flags & (IEEE80211_STA_DISABLE_HT |
				      IEEE80211_STA_DISABLE_VHT |
				      IEEE80211_STA_DISABLE_40MHZ |
				      IEEE80211_STA_DISABLE_80P80MHZ |
				      IEEE80211_STA_DISABLE_160MHZ)) ||
	    !cfg80211_chandef_valid(&chandef)) {
		sdata_info(sdata,
			   "AP %pM changed bandwidth in a way we can't support - disconnect\n",
			   ifmgd->bssid);
		return -EINVAL;
	}

	switch (chandef.width) {
	case NL80211_CHAN_WIDTH_20_NOHT:
	case NL80211_CHAN_WIDTH_20:
		new_sta_bw = IEEE80211_STA_RX_BW_20;
		break;
	case NL80211_CHAN_WIDTH_40:
		new_sta_bw = IEEE80211_STA_RX_BW_40;
		break;
	case NL80211_CHAN_WIDTH_80:
		new_sta_bw = IEEE80211_STA_RX_BW_80;
		break;
	case NL80211_CHAN_WIDTH_80P80:
	case NL80211_CHAN_WIDTH_160:
		new_sta_bw = IEEE80211_STA_RX_BW_160;
		break;
	default:
		return -EINVAL;
	}

	if (new_sta_bw > sta->cur_max_bandwidth)
		new_sta_bw = sta->cur_max_bandwidth;

	if (new_sta_bw < sta->sta.bandwidth) {
		sta->sta.bandwidth = new_sta_bw;
		rate_control_rate_update(local, sband, sta,
					 IEEE80211_RC_BW_CHANGED);
	}

	ret = ieee80211_vif_change_bandwidth(sdata, &chandef, changed);
	if (ret) {
		sdata_info(sdata,
			   "AP %pM changed bandwidth to incompatible one - disconnect\n",
			   ifmgd->bssid);
		return ret;
	}

	if (new_sta_bw > sta->sta.bandwidth) {
		sta->sta.bandwidth = new_sta_bw;
		rate_control_rate_update(local, sband, sta,
					 IEEE80211_RC_BW_CHANGED);
	}

	ht_opmode = le16_to_cpu(ht_oper->operation_mode);

	/* if bss configuration changed store the new one */
	if (sdata->vif.bss_conf.ht_operation_mode != ht_opmode) {
		*changed |= BSS_CHANGED_HT;
		sdata->vif.bss_conf.ht_operation_mode = ht_opmode;
	}

	return 0;
}

/* frame sending functions */

static void ieee80211_add_ht_ie(struct ieee80211_sub_if_data *sdata,
				struct sk_buff *skb, u8 ap_ht_param,
>>>>>>> refs/remotes/origin/master
				struct ieee80211_supported_band *sband,
				struct ieee80211_channel *channel,
				enum ieee80211_smps_mode smps)
{
<<<<<<< HEAD
	struct ieee80211_ht_info *ht_info;
=======
>>>>>>> refs/remotes/origin/master
	u8 *pos;
	u32 flags = channel->flags;
	u16 cap;
	struct ieee80211_sta_ht_cap ht_cap;

	BUILD_BUG_ON(sizeof(ht_cap) != sizeof(sband->ht_cap));

<<<<<<< HEAD
	if (!ht_info_ie)
		return;

	if (ht_info_ie[1] < sizeof(struct ieee80211_ht_info))
		return;

	memcpy(&ht_cap, &sband->ht_cap, sizeof(ht_cap));
	ieee80211_apply_htcap_overrides(sdata, &ht_cap);

	ht_info = (struct ieee80211_ht_info *)(ht_info_ie + 2);

	/* determine capability flags */
	cap = ht_cap.cap;

	switch (ht_info->ht_param & IEEE80211_HT_PARAM_CHA_SEC_OFFSET) {
=======
	memcpy(&ht_cap, &sband->ht_cap, sizeof(ht_cap));
	ieee80211_apply_htcap_overrides(sdata, &ht_cap);

	/* determine capability flags */
	cap = ht_cap.cap;

	switch (ap_ht_param & IEEE80211_HT_PARAM_CHA_SEC_OFFSET) {
>>>>>>> refs/remotes/origin/master
	case IEEE80211_HT_PARAM_CHA_SEC_ABOVE:
		if (flags & IEEE80211_CHAN_NO_HT40PLUS) {
			cap &= ~IEEE80211_HT_CAP_SUP_WIDTH_20_40;
			cap &= ~IEEE80211_HT_CAP_SGI_40;
		}
		break;
	case IEEE80211_HT_PARAM_CHA_SEC_BELOW:
		if (flags & IEEE80211_CHAN_NO_HT40MINUS) {
			cap &= ~IEEE80211_HT_CAP_SUP_WIDTH_20_40;
			cap &= ~IEEE80211_HT_CAP_SGI_40;
		}
		break;
	}

<<<<<<< HEAD
=======
	/*
	 * If 40 MHz was disabled associate as though we weren't
	 * capable of 40 MHz -- some broken APs will never fall
	 * back to trying to transmit in 20 MHz.
	 */
	if (sdata->u.mgd.flags & IEEE80211_STA_DISABLE_40MHZ) {
		cap &= ~IEEE80211_HT_CAP_SUP_WIDTH_20_40;
		cap &= ~IEEE80211_HT_CAP_SGI_40;
	}

>>>>>>> refs/remotes/origin/master
	/* set SM PS mode properly */
	cap &= ~IEEE80211_HT_CAP_SM_PS;
	switch (smps) {
	case IEEE80211_SMPS_AUTOMATIC:
	case IEEE80211_SMPS_NUM_MODES:
		WARN_ON(1);
	case IEEE80211_SMPS_OFF:
		cap |= WLAN_HT_CAP_SM_PS_DISABLED <<
			IEEE80211_HT_CAP_SM_PS_SHIFT;
		break;
	case IEEE80211_SMPS_STATIC:
		cap |= WLAN_HT_CAP_SM_PS_STATIC <<
			IEEE80211_HT_CAP_SM_PS_SHIFT;
		break;
	case IEEE80211_SMPS_DYNAMIC:
		cap |= WLAN_HT_CAP_SM_PS_DYNAMIC <<
			IEEE80211_HT_CAP_SM_PS_SHIFT;
		break;
	}

	/* reserve and fill IE */
	pos = skb_put(skb, sizeof(struct ieee80211_ht_cap) + 2);
	ieee80211_ie_build_ht_cap(pos, &ht_cap, cap);
}

<<<<<<< HEAD
=======
static void ieee80211_add_vht_ie(struct ieee80211_sub_if_data *sdata,
				 struct sk_buff *skb,
				 struct ieee80211_supported_band *sband,
				 struct ieee80211_vht_cap *ap_vht_cap)
{
	u8 *pos;
	u32 cap;
	struct ieee80211_sta_vht_cap vht_cap;

	BUILD_BUG_ON(sizeof(vht_cap) != sizeof(sband->vht_cap));

	memcpy(&vht_cap, &sband->vht_cap, sizeof(vht_cap));
	ieee80211_apply_vhtcap_overrides(sdata, &vht_cap);

	/* determine capability flags */
	cap = vht_cap.cap;

	if (sdata->u.mgd.flags & IEEE80211_STA_DISABLE_80P80MHZ) {
		cap &= ~IEEE80211_VHT_CAP_SUPP_CHAN_WIDTH_160_80PLUS80MHZ;
		cap |= IEEE80211_VHT_CAP_SUPP_CHAN_WIDTH_160MHZ;
	}

	if (sdata->u.mgd.flags & IEEE80211_STA_DISABLE_160MHZ) {
		cap &= ~IEEE80211_VHT_CAP_SHORT_GI_160;
		cap &= ~IEEE80211_VHT_CAP_SUPP_CHAN_WIDTH_160MHZ;
	}

	/*
	 * Some APs apparently get confused if our capabilities are better
	 * than theirs, so restrict what we advertise in the assoc request.
	 */
	if (!(ap_vht_cap->vht_cap_info &
			cpu_to_le32(IEEE80211_VHT_CAP_SU_BEAMFORMER_CAPABLE)))
		cap &= ~IEEE80211_VHT_CAP_SU_BEAMFORMEE_CAPABLE;

	/* reserve and fill IE */
	pos = skb_put(skb, sizeof(struct ieee80211_vht_cap) + 2);
	ieee80211_ie_build_vht_cap(pos, &vht_cap, cap);
}

>>>>>>> refs/remotes/origin/master
static void ieee80211_send_assoc(struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct ieee80211_mgd_assoc_data *assoc_data = ifmgd->assoc_data;
	struct sk_buff *skb;
	struct ieee80211_mgmt *mgmt;
	u8 *pos, qos_info;
	size_t offset = 0, noffset;
<<<<<<< HEAD
	int i, count, rates_len, supp_rates_len;
	u16 capab;
	struct ieee80211_supported_band *sband;
	u32 rates = 0;

	lockdep_assert_held(&ifmgd->mtx);

	sband = local->hw.wiphy->bands[local->oper_channel->band];
=======
	int i, count, rates_len, supp_rates_len, shift;
	u16 capab;
	struct ieee80211_supported_band *sband;
	struct ieee80211_chanctx_conf *chanctx_conf;
	struct ieee80211_channel *chan;
	u32 rate_flags, rates = 0;

	sdata_assert_lock(sdata);

	rcu_read_lock();
	chanctx_conf = rcu_dereference(sdata->vif.chanctx_conf);
	if (WARN_ON(!chanctx_conf)) {
		rcu_read_unlock();
		return;
	}
	chan = chanctx_conf->def.chan;
	rate_flags = ieee80211_chandef_rate_flags(&chanctx_conf->def);
	rcu_read_unlock();
	sband = local->hw.wiphy->bands[chan->band];
	shift = ieee80211_vif_get_shift(&sdata->vif);
>>>>>>> refs/remotes/origin/master

	if (assoc_data->supp_rates_len) {
		/*
		 * Get all rates supported by the device and the AP as
		 * some APs don't like getting a superset of their rates
		 * in the association request (e.g. D-Link DAP 1353 in
		 * b-only mode)...
		 */
<<<<<<< HEAD
		rates_len = ieee80211_compatible_rates(assoc_data->supp_rates,
						       assoc_data->supp_rates_len,
						       sband, &rates);
=======
		rates_len = ieee80211_parse_bitrates(&chanctx_conf->def, sband,
						     assoc_data->supp_rates,
						     assoc_data->supp_rates_len,
						     &rates);
>>>>>>> refs/remotes/origin/master
	} else {
		/*
		 * In case AP not provide any supported rates information
		 * before association, we send information element(s) with
		 * all rates that we support.
		 */
<<<<<<< HEAD
		rates = ~0;
		rates_len = sband->n_bitrates;
=======
		rates_len = 0;
		for (i = 0; i < sband->n_bitrates; i++) {
			if ((rate_flags & sband->bitrates[i].flags)
			    != rate_flags)
				continue;
			rates |= BIT(i);
			rates_len++;
		}
>>>>>>> refs/remotes/origin/master
	}

	skb = alloc_skb(local->hw.extra_tx_headroom +
			sizeof(*mgmt) + /* bit too much but doesn't matter */
			2 + assoc_data->ssid_len + /* SSID */
			4 + rates_len + /* (extended) rates */
			4 + /* power capability */
			2 + 2 * sband->n_channels + /* supported channels */
			2 + sizeof(struct ieee80211_ht_cap) + /* HT */
<<<<<<< HEAD
=======
			2 + sizeof(struct ieee80211_vht_cap) + /* VHT */
>>>>>>> refs/remotes/origin/master
			assoc_data->ie_len + /* extra IEs */
			9, /* WMM */
			GFP_KERNEL);
	if (!skb)
		return;

	skb_reserve(skb, local->hw.extra_tx_headroom);

	capab = WLAN_CAPABILITY_ESS;

	if (sband->band == IEEE80211_BAND_2GHZ) {
		if (!(local->hw.flags & IEEE80211_HW_2GHZ_SHORT_SLOT_INCAPABLE))
			capab |= WLAN_CAPABILITY_SHORT_SLOT_TIME;
		if (!(local->hw.flags & IEEE80211_HW_2GHZ_SHORT_PREAMBLE_INCAPABLE))
			capab |= WLAN_CAPABILITY_SHORT_PREAMBLE;
	}

	if (assoc_data->capability & WLAN_CAPABILITY_PRIVACY)
		capab |= WLAN_CAPABILITY_PRIVACY;

	if ((assoc_data->capability & WLAN_CAPABILITY_SPECTRUM_MGMT) &&
	    (local->hw.flags & IEEE80211_HW_SPECTRUM_MGMT))
		capab |= WLAN_CAPABILITY_SPECTRUM_MGMT;

	mgmt = (struct ieee80211_mgmt *) skb_put(skb, 24);
	memset(mgmt, 0, 24);
	memcpy(mgmt->da, assoc_data->bss->bssid, ETH_ALEN);
	memcpy(mgmt->sa, sdata->vif.addr, ETH_ALEN);
	memcpy(mgmt->bssid, assoc_data->bss->bssid, ETH_ALEN);

	if (!is_zero_ether_addr(assoc_data->prev_bssid)) {
		skb_put(skb, 10);
		mgmt->frame_control = cpu_to_le16(IEEE80211_FTYPE_MGMT |
						  IEEE80211_STYPE_REASSOC_REQ);
		mgmt->u.reassoc_req.capab_info = cpu_to_le16(capab);
		mgmt->u.reassoc_req.listen_interval =
				cpu_to_le16(local->hw.conf.listen_interval);
		memcpy(mgmt->u.reassoc_req.current_ap, assoc_data->prev_bssid,
		       ETH_ALEN);
	} else {
		skb_put(skb, 4);
		mgmt->frame_control = cpu_to_le16(IEEE80211_FTYPE_MGMT |
						  IEEE80211_STYPE_ASSOC_REQ);
		mgmt->u.assoc_req.capab_info = cpu_to_le16(capab);
		mgmt->u.assoc_req.listen_interval =
				cpu_to_le16(local->hw.conf.listen_interval);
	}

	/* SSID */
	pos = skb_put(skb, 2 + assoc_data->ssid_len);
	*pos++ = WLAN_EID_SSID;
	*pos++ = assoc_data->ssid_len;
	memcpy(pos, assoc_data->ssid, assoc_data->ssid_len);

	/* add all rates which were marked to be used above */
	supp_rates_len = rates_len;
	if (supp_rates_len > 8)
		supp_rates_len = 8;

	pos = skb_put(skb, supp_rates_len + 2);
	*pos++ = WLAN_EID_SUPP_RATES;
	*pos++ = supp_rates_len;

	count = 0;
	for (i = 0; i < sband->n_bitrates; i++) {
		if (BIT(i) & rates) {
<<<<<<< HEAD
			int rate = sband->bitrates[i].bitrate;
			*pos++ = (u8) (rate / 5);
=======
			int rate = DIV_ROUND_UP(sband->bitrates[i].bitrate,
						5 * (1 << shift));
			*pos++ = (u8) rate;
>>>>>>> refs/remotes/origin/master
			if (++count == 8)
				break;
		}
	}

	if (rates_len > count) {
		pos = skb_put(skb, rates_len - count + 2);
		*pos++ = WLAN_EID_EXT_SUPP_RATES;
		*pos++ = rates_len - count;

		for (i++; i < sband->n_bitrates; i++) {
			if (BIT(i) & rates) {
<<<<<<< HEAD
				int rate = sband->bitrates[i].bitrate;
				*pos++ = (u8) (rate / 5);
=======
				int rate;
				rate = DIV_ROUND_UP(sband->bitrates[i].bitrate,
						    5 * (1 << shift));
				*pos++ = (u8) rate;
>>>>>>> refs/remotes/origin/master
			}
		}
	}

	if (capab & WLAN_CAPABILITY_SPECTRUM_MGMT) {
		/* 1. power capabilities */
		pos = skb_put(skb, 4);
		*pos++ = WLAN_EID_PWR_CAPABILITY;
		*pos++ = 2;
		*pos++ = 0; /* min tx power */
<<<<<<< HEAD
		*pos++ = local->oper_channel->max_power; /* max tx power */
=======
		 /* max tx power */
		*pos++ = ieee80211_chandef_max_power(&chanctx_conf->def);
>>>>>>> refs/remotes/origin/master

		/* 2. supported channels */
		/* TODO: get this in reg domain format */
		pos = skb_put(skb, 2 * sband->n_channels + 2);
		*pos++ = WLAN_EID_SUPPORTED_CHANNELS;
		*pos++ = 2 * sband->n_channels;
		for (i = 0; i < sband->n_channels; i++) {
			*pos++ = ieee80211_frequency_to_channel(
					sband->channels[i].center_freq);
			*pos++ = 1; /* one channel in the subband*/
		}
	}

	/* if present, add any custom IEs that go before HT */
	if (assoc_data->ie_len && assoc_data->ie) {
		static const u8 before_ht[] = {
			WLAN_EID_SSID,
			WLAN_EID_SUPP_RATES,
			WLAN_EID_EXT_SUPP_RATES,
			WLAN_EID_PWR_CAPABILITY,
			WLAN_EID_SUPPORTED_CHANNELS,
			WLAN_EID_RSN,
			WLAN_EID_QOS_CAPA,
			WLAN_EID_RRM_ENABLED_CAPABILITIES,
			WLAN_EID_MOBILITY_DOMAIN,
			WLAN_EID_SUPPORTED_REGULATORY_CLASSES,
		};
		noffset = ieee80211_ie_split(assoc_data->ie, assoc_data->ie_len,
					     before_ht, ARRAY_SIZE(before_ht),
					     offset);
		pos = skb_put(skb, noffset - offset);
		memcpy(pos, assoc_data->ie + offset, noffset - offset);
		offset = noffset;
	}

<<<<<<< HEAD
	if (!(ifmgd->flags & IEEE80211_STA_DISABLE_11N))
		ieee80211_add_ht_ie(sdata, skb, assoc_data->ht_information_ie,
				    sband, local->oper_channel, ifmgd->ap_smps);
=======
	if (WARN_ON_ONCE((ifmgd->flags & IEEE80211_STA_DISABLE_HT) &&
			 !(ifmgd->flags & IEEE80211_STA_DISABLE_VHT)))
		ifmgd->flags |= IEEE80211_STA_DISABLE_VHT;

	if (!(ifmgd->flags & IEEE80211_STA_DISABLE_HT))
		ieee80211_add_ht_ie(sdata, skb, assoc_data->ap_ht_param,
				    sband, chan, sdata->smps_mode);

	if (!(ifmgd->flags & IEEE80211_STA_DISABLE_VHT))
		ieee80211_add_vht_ie(sdata, skb, sband,
				     &assoc_data->ap_vht_cap);
>>>>>>> refs/remotes/origin/master

	/* if present, add any custom non-vendor IEs that go after HT */
	if (assoc_data->ie_len && assoc_data->ie) {
		noffset = ieee80211_ie_split_vendor(assoc_data->ie,
						    assoc_data->ie_len,
						    offset);
		pos = skb_put(skb, noffset - offset);
		memcpy(pos, assoc_data->ie + offset, noffset - offset);
		offset = noffset;
	}

	if (assoc_data->wmm) {
		if (assoc_data->uapsd) {
			qos_info = ifmgd->uapsd_queues;
			qos_info |= (ifmgd->uapsd_max_sp_len <<
				     IEEE80211_WMM_IE_STA_QOSINFO_SP_SHIFT);
		} else {
			qos_info = 0;
		}

		pos = skb_put(skb, 9);
		*pos++ = WLAN_EID_VENDOR_SPECIFIC;
		*pos++ = 7; /* len */
		*pos++ = 0x00; /* Microsoft OUI 00:50:F2 */
		*pos++ = 0x50;
		*pos++ = 0xf2;
		*pos++ = 2; /* WME */
		*pos++ = 0; /* WME info */
		*pos++ = 1; /* WME ver */
		*pos++ = qos_info;
	}

	/* add any remaining custom (i.e. vendor specific here) IEs */
	if (assoc_data->ie_len && assoc_data->ie) {
		noffset = assoc_data->ie_len;
		pos = skb_put(skb, noffset - offset);
		memcpy(pos, assoc_data->ie + offset, noffset - offset);
	}

<<<<<<< HEAD
	IEEE80211_SKB_CB(skb)->flags |= IEEE80211_TX_INTFL_DONT_ENCRYPT;
	ieee80211_tx_skb(sdata, skb);
}

static void ieee80211_send_deauth_disassoc(struct ieee80211_sub_if_data *sdata,
					   const u8 *bssid, u16 stype,
					   u16 reason, bool send_frame,
					   u8 *frame_buf)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct sk_buff *skb;
	struct ieee80211_mgmt *mgmt = (void *)frame_buf;

	/* build frame */
	mgmt->frame_control = cpu_to_le16(IEEE80211_FTYPE_MGMT | stype);
	mgmt->duration = 0; /* initialize only */
	mgmt->seq_ctrl = 0; /* initialize only */
	memcpy(mgmt->da, bssid, ETH_ALEN);
	memcpy(mgmt->sa, sdata->vif.addr, ETH_ALEN);
	memcpy(mgmt->bssid, bssid, ETH_ALEN);
	/* u.deauth.reason_code == u.disassoc.reason_code */
	mgmt->u.deauth.reason_code = cpu_to_le16(reason);

	if (send_frame) {
		skb = dev_alloc_skb(local->hw.extra_tx_headroom +
				    DEAUTH_DISASSOC_LEN);
		if (!skb)
			return;

		skb_reserve(skb, local->hw.extra_tx_headroom);

		/* copy in frame */
		memcpy(skb_put(skb, DEAUTH_DISASSOC_LEN),
		       mgmt, DEAUTH_DISASSOC_LEN);

		if (!(ifmgd->flags & IEEE80211_STA_MFP_ENABLED))
			IEEE80211_SKB_CB(skb)->flags |=
				IEEE80211_TX_INTFL_DONT_ENCRYPT;
		ieee80211_tx_skb(sdata, skb);
	}
>>>>>>> refs/remotes/origin/cm-10.0
}

=======
	drv_mgd_prepare_tx(local, sdata);

	IEEE80211_SKB_CB(skb)->flags |= IEEE80211_TX_INTFL_DONT_ENCRYPT;
	if (local->hw.flags & IEEE80211_HW_REPORTS_TX_ACK_STATUS)
		IEEE80211_SKB_CB(skb)->flags |= IEEE80211_TX_CTL_REQ_TX_STATUS |
						IEEE80211_TX_INTFL_MLME_CONN_TX;
	ieee80211_tx_skb(sdata, skb);
}

>>>>>>> refs/remotes/origin/master
void ieee80211_send_pspoll(struct ieee80211_local *local,
			   struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_pspoll *pspoll;
	struct sk_buff *skb;

	skb = ieee80211_pspoll_get(&local->hw, &sdata->vif);
	if (!skb)
		return;

	pspoll = (struct ieee80211_pspoll *) skb->data;
	pspoll->frame_control |= cpu_to_le16(IEEE80211_FCTL_PM);

	IEEE80211_SKB_CB(skb)->flags |= IEEE80211_TX_INTFL_DONT_ENCRYPT;
	ieee80211_tx_skb(sdata, skb);
}

void ieee80211_send_nullfunc(struct ieee80211_local *local,
			     struct ieee80211_sub_if_data *sdata,
			     int powersave)
{
	struct sk_buff *skb;
	struct ieee80211_hdr_3addr *nullfunc;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
>>>>>>> refs/remotes/origin/master

	skb = ieee80211_nullfunc_get(&local->hw, &sdata->vif);
	if (!skb)
		return;

	nullfunc = (struct ieee80211_hdr_3addr *) skb->data;
	if (powersave)
		nullfunc->frame_control |= cpu_to_le16(IEEE80211_FCTL_PM);

<<<<<<< HEAD
	IEEE80211_SKB_CB(skb)->flags |= IEEE80211_TX_INTFL_DONT_ENCRYPT;
<<<<<<< HEAD
=======
	if (ifmgd->flags & (IEEE80211_STA_BEACON_POLL |
			    IEEE80211_STA_CONNECTION_POLL))
		IEEE80211_SKB_CB(skb)->flags |= IEEE80211_TX_CTL_USE_MINRATE;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	IEEE80211_SKB_CB(skb)->flags |= IEEE80211_TX_INTFL_DONT_ENCRYPT |
					IEEE80211_TX_INTFL_OFFCHAN_TX_OK;

	if (local->hw.flags & IEEE80211_HW_REPORTS_TX_ACK_STATUS)
		IEEE80211_SKB_CB(skb)->flags |= IEEE80211_TX_CTL_REQ_TX_STATUS;

	if (ifmgd->flags & IEEE80211_STA_CONNECTION_POLL)
		IEEE80211_SKB_CB(skb)->flags |= IEEE80211_TX_CTL_USE_MINRATE;

>>>>>>> refs/remotes/origin/master
	ieee80211_tx_skb(sdata, skb);
}

static void ieee80211_send_4addr_nullfunc(struct ieee80211_local *local,
					  struct ieee80211_sub_if_data *sdata)
{
	struct sk_buff *skb;
	struct ieee80211_hdr *nullfunc;
	__le16 fc;

	if (WARN_ON(sdata->vif.type != NL80211_IFTYPE_STATION))
		return;

	skb = dev_alloc_skb(local->hw.extra_tx_headroom + 30);
<<<<<<< HEAD
<<<<<<< HEAD
	if (!skb) {
		printk(KERN_DEBUG "%s: failed to allocate buffer for 4addr "
		       "nullfunc frame\n", sdata->name);
		return;
	}
=======
	if (!skb)
		return;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!skb)
		return;

>>>>>>> refs/remotes/origin/master
	skb_reserve(skb, local->hw.extra_tx_headroom);

	nullfunc = (struct ieee80211_hdr *) skb_put(skb, 30);
	memset(nullfunc, 0, 30);
	fc = cpu_to_le16(IEEE80211_FTYPE_DATA | IEEE80211_STYPE_NULLFUNC |
			 IEEE80211_FCTL_FROMDS | IEEE80211_FCTL_TODS);
	nullfunc->frame_control = fc;
	memcpy(nullfunc->addr1, sdata->u.mgd.bssid, ETH_ALEN);
	memcpy(nullfunc->addr2, sdata->vif.addr, ETH_ALEN);
	memcpy(nullfunc->addr3, sdata->u.mgd.bssid, ETH_ALEN);
	memcpy(nullfunc->addr4, sdata->vif.addr, ETH_ALEN);

	IEEE80211_SKB_CB(skb)->flags |= IEEE80211_TX_INTFL_DONT_ENCRYPT;
	ieee80211_tx_skb(sdata, skb);
}

/* spectrum management related things */
static void ieee80211_chswitch_work(struct work_struct *work)
{
	struct ieee80211_sub_if_data *sdata =
		container_of(work, struct ieee80211_sub_if_data, u.mgd.chswitch_work);
<<<<<<< HEAD
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
=======
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	u32 changed = 0;
	int ret;
>>>>>>> refs/remotes/origin/master

	if (!ieee80211_sdata_running(sdata))
		return;

<<<<<<< HEAD
	mutex_lock(&ifmgd->mtx);
	if (!ifmgd->associated)
		goto out;

	sdata->local->oper_channel = sdata->local->csa_channel;
	if (!sdata->local->ops->channel_switch) {
		/* call "hw_config" only if doing sw channel switch */
		ieee80211_hw_config(sdata->local,
			IEEE80211_CONF_CHANGE_CHANNEL);
<<<<<<< HEAD
=======
	} else {
		/* update the device channel directly */
		sdata->local->hw.conf.channel = sdata->local->oper_channel;
>>>>>>> refs/remotes/origin/cm-10.0
	}

	/* XXX: shouldn't really modify cfg80211-owned data! */
	ifmgd->associated->channel = sdata->local->oper_channel;

	ieee80211_wake_queues_by_reason(&sdata->local->hw,
					IEEE80211_QUEUE_STOP_REASON_CSA);
 out:
	ifmgd->flags &= ~IEEE80211_STA_CSA_RECEIVED;
	mutex_unlock(&ifmgd->mtx);
=======
	sdata_lock(sdata);
	if (!ifmgd->associated)
		goto out;

	ret = ieee80211_vif_change_channel(sdata, &local->csa_chandef,
					   &changed);
	if (ret) {
		sdata_info(sdata,
			   "vif channel switch failed, disconnecting\n");
		ieee80211_queue_work(&sdata->local->hw,
				     &ifmgd->csa_connection_drop_work);
		goto out;
	}

	if (!local->use_chanctx) {
		local->_oper_chandef = local->csa_chandef;
		/* Call "hw_config" only if doing sw channel switch.
		 * Otherwise update the channel directly
		 */
		if (!local->ops->channel_switch)
			ieee80211_hw_config(local, 0);
		else
			local->hw.conf.chandef = local->_oper_chandef;
	}

	/* XXX: shouldn't really modify cfg80211-owned data! */
	ifmgd->associated->channel = local->csa_chandef.chan;

	/* XXX: wait for a beacon first? */
	ieee80211_wake_queues_by_reason(&local->hw,
					IEEE80211_MAX_QUEUE_MAP,
					IEEE80211_QUEUE_STOP_REASON_CSA);

	ieee80211_bss_info_change_notify(sdata, changed);

 out:
	sdata->vif.csa_active = false;
	ifmgd->flags &= ~IEEE80211_STA_CSA_RECEIVED;
	sdata_unlock(sdata);
>>>>>>> refs/remotes/origin/master
}

void ieee80211_chswitch_done(struct ieee80211_vif *vif, bool success)
{
<<<<<<< HEAD
	struct ieee80211_sub_if_data *sdata;
	struct ieee80211_if_managed *ifmgd;

	sdata = vif_to_sdata(vif);
	ifmgd = &sdata->u.mgd;

	trace_api_chswitch_done(sdata, success);
	if (!success) {
		/*
		 * If the channel switch was not successful, stay
		 * around on the old channel. We currently lack
		 * good handling of this situation, possibly we
		 * should just drop the association.
		 */
		sdata->local->csa_channel = sdata->local->oper_channel;
	}

	ieee80211_queue_work(&sdata->local->hw, &ifmgd->chswitch_work);
=======
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;

	trace_api_chswitch_done(sdata, success);
	if (!success) {
		sdata_info(sdata,
			   "driver channel switch failed, disconnecting\n");
		ieee80211_queue_work(&sdata->local->hw,
				     &ifmgd->csa_connection_drop_work);
	} else {
		ieee80211_queue_work(&sdata->local->hw, &ifmgd->chswitch_work);
	}
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL(ieee80211_chswitch_done);

static void ieee80211_chswitch_timer(unsigned long data)
{
	struct ieee80211_sub_if_data *sdata =
		(struct ieee80211_sub_if_data *) data;
<<<<<<< HEAD
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;

	if (sdata->local->quiescing) {
		set_bit(TMR_RUNNING_CHANSW, &ifmgd->timers_running);
		return;
	}

	ieee80211_queue_work(&sdata->local->hw, &ifmgd->chswitch_work);
}

void ieee80211_sta_process_chanswitch(struct ieee80211_sub_if_data *sdata,
				      struct ieee80211_channel_sw_ie *sw_elem,
				      struct ieee80211_bss *bss,
				      u64 timestamp)
{
	struct cfg80211_bss *cbss =
		container_of((void *)bss, struct cfg80211_bss, priv);
	struct ieee80211_channel *new_ch;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	int new_freq = ieee80211_channel_to_frequency(sw_elem->new_ch_num,
						      cbss->channel->band);

	ASSERT_MGD_MTX(ifmgd);

	if (!ifmgd->associated)
		return;

	if (sdata->local->scanning)
		return;

	/* Disregard subsequent beacons if we are already running a timer
	   processing a CSA */

	if (ifmgd->flags & IEEE80211_STA_CSA_RECEIVED)
		return;

	new_ch = ieee80211_get_channel(sdata->local->hw.wiphy, new_freq);
	if (!new_ch || new_ch->flags & IEEE80211_CHAN_DISABLED)
		return;

	sdata->local->csa_channel = new_ch;

	if (sdata->local->ops->channel_switch) {
		/* use driver's channel switch callback */
		struct ieee80211_channel_switch ch_switch;
		memset(&ch_switch, 0, sizeof(ch_switch));
		ch_switch.timestamp = timestamp;
		if (sw_elem->mode) {
			ch_switch.block_tx = true;
			ieee80211_stop_queues_by_reason(&sdata->local->hw,
					IEEE80211_QUEUE_STOP_REASON_CSA);
		}
		ch_switch.channel = new_ch;
		ch_switch.count = sw_elem->count;
		ifmgd->flags |= IEEE80211_STA_CSA_RECEIVED;
		drv_channel_switch(sdata->local, &ch_switch);
		return;
	}

	/* channel switch handled in software */
	if (sw_elem->count <= 1) {
		ieee80211_queue_work(&sdata->local->hw, &ifmgd->chswitch_work);
	} else {
		if (sw_elem->mode)
			ieee80211_stop_queues_by_reason(&sdata->local->hw,
					IEEE80211_QUEUE_STOP_REASON_CSA);
		ifmgd->flags |= IEEE80211_STA_CSA_RECEIVED;
		mod_timer(&ifmgd->chswitch_timer,
			  jiffies +
			  msecs_to_jiffies(sw_elem->count *
					   cbss->beacon_interval));
	}
}

static void ieee80211_handle_pwr_constr(struct ieee80211_sub_if_data *sdata,
					u16 capab_info, u8 *pwr_constr_elem,
					u8 pwr_constr_elem_len)
{
	struct ieee80211_conf *conf = &sdata->local->hw.conf;

	if (!(capab_info & WLAN_CAPABILITY_SPECTRUM_MGMT))
		return;

	/* Power constraint IE length should be 1 octet */
	if (pwr_constr_elem_len != 1)
		return;

<<<<<<< HEAD
	if ((*pwr_constr_elem <= conf->channel->max_power) &&
=======
	if ((*pwr_constr_elem <= conf->channel->max_reg_power) &&
>>>>>>> refs/remotes/origin/cm-10.0
	    (*pwr_constr_elem != sdata->local->power_constr_level)) {
		sdata->local->power_constr_level = *pwr_constr_elem;
		ieee80211_hw_config(sdata->local, 0);
	}
}

void ieee80211_enable_dyn_ps(struct ieee80211_vif *vif)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_conf *conf = &local->hw.conf;

	WARN_ON(sdata->vif.type != NL80211_IFTYPE_STATION ||
		!(local->hw.flags & IEEE80211_HW_SUPPORTS_PS) ||
		(local->hw.flags & IEEE80211_HW_SUPPORTS_DYNAMIC_PS));

	local->disable_dynamic_ps = false;
	conf->dynamic_ps_timeout = local->dynamic_ps_user_timeout;
}
EXPORT_SYMBOL(ieee80211_enable_dyn_ps);

void ieee80211_disable_dyn_ps(struct ieee80211_vif *vif)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_conf *conf = &local->hw.conf;

	WARN_ON(sdata->vif.type != NL80211_IFTYPE_STATION ||
		!(local->hw.flags & IEEE80211_HW_SUPPORTS_PS) ||
		(local->hw.flags & IEEE80211_HW_SUPPORTS_DYNAMIC_PS));

	local->disable_dynamic_ps = true;
	conf->dynamic_ps_timeout = 0;
	del_timer_sync(&local->dynamic_ps_timer);
	ieee80211_queue_work(&local->hw,
			     &local->dynamic_ps_enable_work);
}
EXPORT_SYMBOL(ieee80211_disable_dyn_ps);
=======

	ieee80211_queue_work(&sdata->local->hw, &sdata->u.mgd.chswitch_work);
}

static void
ieee80211_sta_process_chanswitch(struct ieee80211_sub_if_data *sdata,
				 u64 timestamp, struct ieee802_11_elems *elems,
				 bool beacon)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct cfg80211_bss *cbss = ifmgd->associated;
	struct ieee80211_chanctx *chanctx;
	enum ieee80211_band current_band;
	struct ieee80211_csa_ie csa_ie;
	int res;

	sdata_assert_lock(sdata);

	if (!cbss)
		return;

	if (local->scanning)
		return;

	/* disregard subsequent announcements if we are already processing */
	if (ifmgd->flags & IEEE80211_STA_CSA_RECEIVED)
		return;

	current_band = cbss->channel->band;
	memset(&csa_ie, 0, sizeof(csa_ie));
	res = ieee80211_parse_ch_switch_ie(sdata, elems, beacon, current_band,
					   ifmgd->flags,
					   ifmgd->associated->bssid, &csa_ie);
	if (res	< 0)
		ieee80211_queue_work(&local->hw,
				     &ifmgd->csa_connection_drop_work);
	if (res)
		return;

	if (!cfg80211_chandef_usable(local->hw.wiphy, &csa_ie.chandef,
				     IEEE80211_CHAN_DISABLED)) {
		sdata_info(sdata,
			   "AP %pM switches to unsupported channel (%d MHz, width:%d, CF1/2: %d/%d MHz), disconnecting\n",
			   ifmgd->associated->bssid,
			   csa_ie.chandef.chan->center_freq,
			   csa_ie.chandef.width, csa_ie.chandef.center_freq1,
			   csa_ie.chandef.center_freq2);
		ieee80211_queue_work(&local->hw,
				     &ifmgd->csa_connection_drop_work);
		return;
	}

	ifmgd->flags |= IEEE80211_STA_CSA_RECEIVED;
	sdata->vif.csa_active = true;

	mutex_lock(&local->chanctx_mtx);
	if (local->use_chanctx) {
		u32 num_chanctx = 0;
		list_for_each_entry(chanctx, &local->chanctx_list, list)
		       num_chanctx++;

		if (num_chanctx > 1 ||
		    !(local->hw.flags & IEEE80211_HW_CHANCTX_STA_CSA)) {
			sdata_info(sdata,
				   "not handling chan-switch with channel contexts\n");
			ieee80211_queue_work(&local->hw,
					     &ifmgd->csa_connection_drop_work);
			mutex_unlock(&local->chanctx_mtx);
			return;
		}
	}

	if (WARN_ON(!rcu_access_pointer(sdata->vif.chanctx_conf))) {
		ieee80211_queue_work(&local->hw,
				     &ifmgd->csa_connection_drop_work);
		mutex_unlock(&local->chanctx_mtx);
		return;
	}
	chanctx = container_of(rcu_access_pointer(sdata->vif.chanctx_conf),
			       struct ieee80211_chanctx, conf);
	if (chanctx->refcount > 1) {
		sdata_info(sdata,
			   "channel switch with multiple interfaces on the same channel, disconnecting\n");
		ieee80211_queue_work(&local->hw,
				     &ifmgd->csa_connection_drop_work);
		mutex_unlock(&local->chanctx_mtx);
		return;
	}
	mutex_unlock(&local->chanctx_mtx);

	local->csa_chandef = csa_ie.chandef;

	if (csa_ie.mode)
		ieee80211_stop_queues_by_reason(&local->hw,
				IEEE80211_MAX_QUEUE_MAP,
				IEEE80211_QUEUE_STOP_REASON_CSA);

	if (local->ops->channel_switch) {
		/* use driver's channel switch callback */
		struct ieee80211_channel_switch ch_switch = {
			.timestamp = timestamp,
			.block_tx = csa_ie.mode,
			.chandef = csa_ie.chandef,
			.count = csa_ie.count,
		};

		drv_channel_switch(local, &ch_switch);
		return;
	}

	/* channel switch handled in software */
	if (csa_ie.count <= 1)
		ieee80211_queue_work(&local->hw, &ifmgd->chswitch_work);
	else
		mod_timer(&ifmgd->chswitch_timer,
			  TU_TO_EXP_TIME(csa_ie.count * cbss->beacon_interval));
}

static u32 ieee80211_handle_pwr_constr(struct ieee80211_sub_if_data *sdata,
				       struct ieee80211_channel *channel,
				       const u8 *country_ie, u8 country_ie_len,
				       const u8 *pwr_constr_elem)
{
	struct ieee80211_country_ie_triplet *triplet;
	int chan = ieee80211_frequency_to_channel(channel->center_freq);
	int i, chan_pwr, chan_increment, new_ap_level;
	bool have_chan_pwr = false;

	/* Invalid IE */
	if (country_ie_len % 2 || country_ie_len < IEEE80211_COUNTRY_IE_MIN_LEN)
		return 0;

	triplet = (void *)(country_ie + 3);
	country_ie_len -= 3;

	switch (channel->band) {
	default:
		WARN_ON_ONCE(1);
		/* fall through */
	case IEEE80211_BAND_2GHZ:
	case IEEE80211_BAND_60GHZ:
		chan_increment = 1;
		break;
	case IEEE80211_BAND_5GHZ:
		chan_increment = 4;
		break;
	}

	/* find channel */
	while (country_ie_len >= 3) {
		u8 first_channel = triplet->chans.first_channel;

		if (first_channel >= IEEE80211_COUNTRY_EXTENSION_ID)
			goto next;

		for (i = 0; i < triplet->chans.num_channels; i++) {
			if (first_channel + i * chan_increment == chan) {
				have_chan_pwr = true;
				chan_pwr = triplet->chans.max_power;
				break;
			}
		}
		if (have_chan_pwr)
			break;

 next:
		triplet++;
		country_ie_len -= 3;
	}

	if (!have_chan_pwr)
		return 0;

	new_ap_level = max_t(int, 0, chan_pwr - *pwr_constr_elem);

	if (sdata->ap_power_level == new_ap_level)
		return 0;

	sdata_info(sdata,
		   "Limiting TX power to %d (%d - %d) dBm as advertised by %pM\n",
		   new_ap_level, chan_pwr, *pwr_constr_elem,
		   sdata->u.mgd.bssid);
	sdata->ap_power_level = new_ap_level;
	if (__ieee80211_recalc_txpower(sdata))
		return BSS_CHANGED_TXPOWER;
	return 0;
}
>>>>>>> refs/remotes/origin/master

/* powersave */
static void ieee80211_enable_ps(struct ieee80211_local *local,
				struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_conf *conf = &local->hw.conf;

	/*
	 * If we are scanning right now then the parameters will
	 * take effect when scan finishes.
	 */
	if (local->scanning)
		return;

	if (conf->dynamic_ps_timeout > 0 &&
	    !(local->hw.flags & IEEE80211_HW_SUPPORTS_DYNAMIC_PS)) {
		mod_timer(&local->dynamic_ps_timer, jiffies +
			  msecs_to_jiffies(conf->dynamic_ps_timeout));
	} else {
		if (local->hw.flags & IEEE80211_HW_PS_NULLFUNC_STACK)
			ieee80211_send_nullfunc(local, sdata, 1);

		if ((local->hw.flags & IEEE80211_HW_PS_NULLFUNC_STACK) &&
		    (local->hw.flags & IEEE80211_HW_REPORTS_TX_ACK_STATUS))
			return;

		conf->flags |= IEEE80211_CONF_PS;
		ieee80211_hw_config(local, IEEE80211_CONF_CHANGE_PS);
	}
}

static void ieee80211_change_ps(struct ieee80211_local *local)
{
	struct ieee80211_conf *conf = &local->hw.conf;

	if (local->ps_sdata) {
		ieee80211_enable_ps(local, local->ps_sdata);
	} else if (conf->flags & IEEE80211_CONF_PS) {
		conf->flags &= ~IEEE80211_CONF_PS;
		ieee80211_hw_config(local, IEEE80211_CONF_CHANGE_PS);
		del_timer_sync(&local->dynamic_ps_timer);
		cancel_work_sync(&local->dynamic_ps_enable_work);
	}
}

static bool ieee80211_powersave_allowed(struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_if_managed *mgd = &sdata->u.mgd;
	struct sta_info *sta = NULL;
<<<<<<< HEAD
<<<<<<< HEAD
	u32 sta_flags = 0;
=======
	bool authorized = false;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	bool authorized = false;
>>>>>>> refs/remotes/origin/master

	if (!mgd->powersave)
		return false;

	if (mgd->broken_ap)
		return false;

	if (!mgd->associated)
		return false;

<<<<<<< HEAD
	if (!mgd->associated->beacon_ies)
		return false;

	if (mgd->flags & (IEEE80211_STA_BEACON_POLL |
			  IEEE80211_STA_CONNECTION_POLL))
=======
	if (mgd->flags & IEEE80211_STA_CONNECTION_POLL)
		return false;

	if (!mgd->have_beacon)
>>>>>>> refs/remotes/origin/master
		return false;

	rcu_read_lock();
	sta = sta_info_get(sdata, mgd->bssid);
	if (sta)
<<<<<<< HEAD
<<<<<<< HEAD
		sta_flags = get_sta_flags(sta);
	rcu_read_unlock();

	if (!(sta_flags & WLAN_STA_AUTHORIZED))
		return false;

	return true;
=======
=======
>>>>>>> refs/remotes/origin/master
		authorized = test_sta_flag(sta, WLAN_STA_AUTHORIZED);
	rcu_read_unlock();

	return authorized;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

/* need to hold RTNL or interface lock */
void ieee80211_recalc_ps(struct ieee80211_local *local, s32 latency)
{
	struct ieee80211_sub_if_data *sdata, *found = NULL;
	int count = 0;
	int timeout;

	if (!(local->hw.flags & IEEE80211_HW_SUPPORTS_PS)) {
		local->ps_sdata = NULL;
		return;
	}

<<<<<<< HEAD
	if (!list_empty(&local->work_list)) {
		local->ps_sdata = NULL;
		goto change;
	}

=======
>>>>>>> refs/remotes/origin/master
	list_for_each_entry(sdata, &local->interfaces, list) {
		if (!ieee80211_sdata_running(sdata))
			continue;
		if (sdata->vif.type == NL80211_IFTYPE_AP) {
			/* If an AP vif is found, then disable PS
			 * by setting the count to zero thereby setting
			 * ps_sdata to NULL.
			 */
			count = 0;
			break;
		}
		if (sdata->vif.type != NL80211_IFTYPE_STATION)
			continue;
		found = sdata;
		count++;
	}

	if (count == 1 && ieee80211_powersave_allowed(found)) {
<<<<<<< HEAD
		struct ieee80211_conf *conf = &local->hw.conf;
=======
>>>>>>> refs/remotes/origin/master
		s32 beaconint_us;

		if (latency < 0)
			latency = pm_qos_request(PM_QOS_NETWORK_LATENCY);

		beaconint_us = ieee80211_tu_to_usec(
					found->vif.bss_conf.beacon_int);

		timeout = local->dynamic_ps_forced_timeout;
		if (timeout < 0) {
			/*
			 * Go to full PSM if the user configures a very low
			 * latency requirement.
			 * The 2000 second value is there for compatibility
			 * until the PM_QOS_NETWORK_LATENCY is configured
			 * with real values.
			 */
			if (latency > (1900 * USEC_PER_MSEC) &&
			    latency != (2000 * USEC_PER_SEC))
				timeout = 0;
			else
				timeout = 100;
		}
<<<<<<< HEAD
		local->dynamic_ps_user_timeout = timeout;
		if (!local->disable_dynamic_ps)
			conf->dynamic_ps_timeout =
				local->dynamic_ps_user_timeout;
=======
		local->hw.conf.dynamic_ps_timeout = timeout;
>>>>>>> refs/remotes/origin/master

		if (beaconint_us > latency) {
			local->ps_sdata = NULL;
		} else {
<<<<<<< HEAD
			struct ieee80211_bss *bss;
			int maxslp = 1;
			u8 dtimper;

			bss = (void *)found->u.mgd.associated->priv;
			dtimper = bss->dtim_period;
=======
			int maxslp = 1;
			u8 dtimper = found->u.mgd.dtim_period;
>>>>>>> refs/remotes/origin/master

			/* If the TIM IE is invalid, pretend the value is 1 */
			if (!dtimper)
				dtimper = 1;
			else if (dtimper > 1)
				maxslp = min_t(int, dtimper,
						    latency / beaconint_us);

			local->hw.conf.max_sleep_period = maxslp;
			local->hw.conf.ps_dtim_period = dtimper;
			local->ps_sdata = found;
		}
	} else {
		local->ps_sdata = NULL;
	}

<<<<<<< HEAD
 change:
	ieee80211_change_ps(local);
}

=======
	ieee80211_change_ps(local);
}

void ieee80211_recalc_ps_vif(struct ieee80211_sub_if_data *sdata)
{
	bool ps_allowed = ieee80211_powersave_allowed(sdata);

	if (sdata->vif.bss_conf.ps != ps_allowed) {
		sdata->vif.bss_conf.ps = ps_allowed;
		ieee80211_bss_info_change_notify(sdata, BSS_CHANGED_PS);
	}
}

>>>>>>> refs/remotes/origin/master
void ieee80211_dynamic_ps_disable_work(struct work_struct *work)
{
	struct ieee80211_local *local =
		container_of(work, struct ieee80211_local,
			     dynamic_ps_disable_work);

	if (local->hw.conf.flags & IEEE80211_CONF_PS) {
		local->hw.conf.flags &= ~IEEE80211_CONF_PS;
		ieee80211_hw_config(local, IEEE80211_CONF_CHANGE_PS);
	}

	ieee80211_wake_queues_by_reason(&local->hw,
<<<<<<< HEAD
=======
					IEEE80211_MAX_QUEUE_MAP,
>>>>>>> refs/remotes/origin/master
					IEEE80211_QUEUE_STOP_REASON_PS);
}

void ieee80211_dynamic_ps_enable_work(struct work_struct *work)
{
	struct ieee80211_local *local =
		container_of(work, struct ieee80211_local,
			     dynamic_ps_enable_work);
	struct ieee80211_sub_if_data *sdata = local->ps_sdata;
<<<<<<< HEAD
<<<<<<< HEAD
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
=======
	struct ieee80211_if_managed *ifmgd;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct ieee80211_if_managed *ifmgd;
>>>>>>> refs/remotes/origin/master
	unsigned long flags;
	int q;

	/* can only happen when PS was just disabled anyway */
	if (!sdata)
		return;

<<<<<<< HEAD
<<<<<<< HEAD
	if (local->hw.conf.flags & IEEE80211_CONF_PS)
		return;

	/*
	 * transmission can be stopped by others which leads to
	 * dynamic_ps_timer expiry. Postpond the ps timer if it
	 * is not the actual idle state.
	 */
	spin_lock_irqsave(&local->queue_stop_reason_lock, flags);
	for (q = 0; q < local->hw.queues; q++) {
		if (local->queue_stop_reasons[q]) {
			spin_unlock_irqrestore(&local->queue_stop_reason_lock,
					       flags);
=======
=======
>>>>>>> refs/remotes/origin/master
	ifmgd = &sdata->u.mgd;

	if (local->hw.conf.flags & IEEE80211_CONF_PS)
		return;

<<<<<<< HEAD
	if (!local->disable_dynamic_ps &&
	    local->hw.conf.dynamic_ps_timeout > 0) {
		/* don't enter PS if TX frames are pending */
		if (drv_tx_frames_pending(local)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (local->hw.conf.dynamic_ps_timeout > 0) {
		/* don't enter PS if TX frames are pending */
		if (drv_tx_frames_pending(local)) {
>>>>>>> refs/remotes/origin/master
			mod_timer(&local->dynamic_ps_timer, jiffies +
				  msecs_to_jiffies(
				  local->hw.conf.dynamic_ps_timeout));
			return;
		}
<<<<<<< HEAD
<<<<<<< HEAD
	}
	spin_unlock_irqrestore(&local->queue_stop_reason_lock, flags);

	if ((local->hw.flags & IEEE80211_HW_PS_NULLFUNC_STACK) &&
	    (!(ifmgd->flags & IEEE80211_STA_NULLFUNC_ACKED))) {
=======
=======
>>>>>>> refs/remotes/origin/master

		/*
		 * transmission can be stopped by others which leads to
		 * dynamic_ps_timer expiry. Postpone the ps timer if it
		 * is not the actual idle state.
		 */
		spin_lock_irqsave(&local->queue_stop_reason_lock, flags);
		for (q = 0; q < local->hw.queues; q++) {
			if (local->queue_stop_reasons[q]) {
				spin_unlock_irqrestore(&local->queue_stop_reason_lock,
						       flags);
				mod_timer(&local->dynamic_ps_timer, jiffies +
					  msecs_to_jiffies(
					  local->hw.conf.dynamic_ps_timeout));
				return;
			}
		}
		spin_unlock_irqrestore(&local->queue_stop_reason_lock, flags);
	}

	if ((local->hw.flags & IEEE80211_HW_PS_NULLFUNC_STACK) &&
	    !(ifmgd->flags & IEEE80211_STA_NULLFUNC_ACKED)) {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		netif_tx_stop_all_queues(sdata->dev);

		if (drv_tx_frames_pending(local))
			mod_timer(&local->dynamic_ps_timer, jiffies +
				  msecs_to_jiffies(
				  local->hw.conf.dynamic_ps_timeout));
		else {
			ieee80211_send_nullfunc(local, sdata, 1);
			/* Flush to get the tx status of nullfunc frame */
			drv_flush(local, false);
=======
		if (drv_tx_frames_pending(local)) {
			mod_timer(&local->dynamic_ps_timer, jiffies +
				  msecs_to_jiffies(
				  local->hw.conf.dynamic_ps_timeout));
		} else {
			ieee80211_send_nullfunc(local, sdata, 1);
			/* Flush to get the tx status of nullfunc frame */
			ieee80211_flush_queues(local, sdata);
>>>>>>> refs/remotes/origin/master
		}
	}

	if (!((local->hw.flags & IEEE80211_HW_REPORTS_TX_ACK_STATUS) &&
	      (local->hw.flags & IEEE80211_HW_PS_NULLFUNC_STACK)) ||
	    (ifmgd->flags & IEEE80211_STA_NULLFUNC_ACKED)) {
		ifmgd->flags &= ~IEEE80211_STA_NULLFUNC_ACKED;
		local->hw.conf.flags |= IEEE80211_CONF_PS;
		ieee80211_hw_config(local, IEEE80211_CONF_CHANGE_PS);
	}
<<<<<<< HEAD

<<<<<<< HEAD
	netif_tx_wake_all_queues(sdata->dev);
=======
	if (local->hw.flags & IEEE80211_HW_PS_NULLFUNC_STACK)
		netif_tx_wake_all_queues(sdata->dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

void ieee80211_dynamic_ps_timer(unsigned long data)
{
	struct ieee80211_local *local = (void *) data;

	if (local->quiescing || local->suspended)
		return;

	ieee80211_queue_work(&local->hw, &local->dynamic_ps_enable_work);
}

<<<<<<< HEAD
/* MLME */
static void ieee80211_sta_wmm_params(struct ieee80211_local *local,
				     struct ieee80211_sub_if_data *sdata,
				     u8 *wmm_param, size_t wmm_param_len)
=======
void ieee80211_dfs_cac_timer_work(struct work_struct *work)
{
	struct delayed_work *delayed_work =
		container_of(work, struct delayed_work, work);
	struct ieee80211_sub_if_data *sdata =
		container_of(delayed_work, struct ieee80211_sub_if_data,
			     dfs_cac_timer_work);

	ieee80211_vif_release_channel(sdata);

	cfg80211_cac_event(sdata->dev, NL80211_RADAR_CAC_FINISHED, GFP_KERNEL);
}

/* MLME */
static bool ieee80211_sta_wmm_params(struct ieee80211_local *local,
				     struct ieee80211_sub_if_data *sdata,
				     const u8 *wmm_param, size_t wmm_param_len)
>>>>>>> refs/remotes/origin/master
{
	struct ieee80211_tx_queue_params params;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	size_t left;
	int count;
<<<<<<< HEAD
	u8 *pos, uapsd_queues = 0;

	if (!local->ops->conf_tx)
		return;

	if (local->hw.queues < 4)
		return;

	if (!wmm_param)
		return;

	if (wmm_param_len < 8 || wmm_param[5] /* version */ != 1)
		return;

	if (ifmgd->flags & IEEE80211_STA_UAPSD_ENABLED)
<<<<<<< HEAD
		uapsd_queues = local->uapsd_queues;
=======
		uapsd_queues = ifmgd->uapsd_queues;
>>>>>>> refs/remotes/origin/cm-10.0

	count = wmm_param[6] & 0x0f;
	if (count == ifmgd->wmm_last_param_set)
		return;
=======
	const u8 *pos;
	u8 uapsd_queues = 0;

	if (!local->ops->conf_tx)
		return false;

	if (local->hw.queues < IEEE80211_NUM_ACS)
		return false;

	if (!wmm_param)
		return false;

	if (wmm_param_len < 8 || wmm_param[5] /* version */ != 1)
		return false;

	if (ifmgd->flags & IEEE80211_STA_UAPSD_ENABLED)
		uapsd_queues = ifmgd->uapsd_queues;

	count = wmm_param[6] & 0x0f;
	if (count == ifmgd->wmm_last_param_set)
		return false;
>>>>>>> refs/remotes/origin/master
	ifmgd->wmm_last_param_set = count;

	pos = wmm_param + 8;
	left = wmm_param_len - 8;

	memset(&params, 0, sizeof(params));

<<<<<<< HEAD
	local->wmm_acm = 0;
=======
	sdata->wmm_acm = 0;
>>>>>>> refs/remotes/origin/master
	for (; left >= 4; left -= 4, pos += 4) {
		int aci = (pos[0] >> 5) & 0x03;
		int acm = (pos[0] >> 4) & 0x01;
		bool uapsd = false;
		int queue;

		switch (aci) {
		case 1: /* AC_BK */
			queue = 3;
			if (acm)
<<<<<<< HEAD
				local->wmm_acm |= BIT(1) | BIT(2); /* BK/- */
=======
				sdata->wmm_acm |= BIT(1) | BIT(2); /* BK/- */
>>>>>>> refs/remotes/origin/master
			if (uapsd_queues & IEEE80211_WMM_IE_STA_QOSINFO_AC_BK)
				uapsd = true;
			break;
		case 2: /* AC_VI */
			queue = 1;
			if (acm)
<<<<<<< HEAD
				local->wmm_acm |= BIT(4) | BIT(5); /* CL/VI */
=======
				sdata->wmm_acm |= BIT(4) | BIT(5); /* CL/VI */
>>>>>>> refs/remotes/origin/master
			if (uapsd_queues & IEEE80211_WMM_IE_STA_QOSINFO_AC_VI)
				uapsd = true;
			break;
		case 3: /* AC_VO */
			queue = 0;
			if (acm)
<<<<<<< HEAD
				local->wmm_acm |= BIT(6) | BIT(7); /* VO/NC */
=======
				sdata->wmm_acm |= BIT(6) | BIT(7); /* VO/NC */
>>>>>>> refs/remotes/origin/master
			if (uapsd_queues & IEEE80211_WMM_IE_STA_QOSINFO_AC_VO)
				uapsd = true;
			break;
		case 0: /* AC_BE */
		default:
			queue = 2;
			if (acm)
<<<<<<< HEAD
				local->wmm_acm |= BIT(0) | BIT(3); /* BE/EE */
=======
				sdata->wmm_acm |= BIT(0) | BIT(3); /* BE/EE */
>>>>>>> refs/remotes/origin/master
			if (uapsd_queues & IEEE80211_WMM_IE_STA_QOSINFO_AC_BE)
				uapsd = true;
			break;
		}

		params.aifs = pos[0] & 0x0f;
		params.cw_max = ecw2cw((pos[1] & 0xf0) >> 4);
		params.cw_min = ecw2cw(pos[1] & 0x0f);
		params.txop = get_unaligned_le16(pos + 2);
<<<<<<< HEAD
		params.uapsd = uapsd;

#ifdef CONFIG_MAC80211_VERBOSE_DEBUG
		wiphy_debug(local->hw.wiphy,
			    "WMM queue=%d aci=%d acm=%d aifs=%d "
			    "cWmin=%d cWmax=%d txop=%d uapsd=%d\n",
			    queue, aci, acm,
			    params.aifs, params.cw_min, params.cw_max,
			    params.txop, params.uapsd);
#endif
<<<<<<< HEAD
		if (drv_conf_tx(local, queue, &params))
=======
		sdata->tx_conf[queue] = params;
		if (drv_conf_tx(local, sdata, queue, &params))
>>>>>>> refs/remotes/origin/cm-10.0
			wiphy_debug(local->hw.wiphy,
				    "failed to set TX queue parameters for queue %d\n",
				    queue);
=======
		params.acm = acm;
		params.uapsd = uapsd;

		mlme_dbg(sdata,
			 "WMM queue=%d aci=%d acm=%d aifs=%d cWmin=%d cWmax=%d txop=%d uapsd=%d\n",
			 queue, aci, acm,
			 params.aifs, params.cw_min, params.cw_max,
			 params.txop, params.uapsd);
		sdata->tx_conf[queue] = params;
		if (drv_conf_tx(local, sdata, queue, &params))
			sdata_err(sdata,
				  "failed to set TX queue parameters for queue %d\n",
				  queue);
>>>>>>> refs/remotes/origin/master
	}

	/* enable WMM or activate new settings */
	sdata->vif.bss_conf.qos = true;
<<<<<<< HEAD
<<<<<<< HEAD
	ieee80211_bss_info_change_notify(sdata, BSS_CHANGED_QOS);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return true;
}

static void __ieee80211_stop_poll(struct ieee80211_sub_if_data *sdata)
{
	lockdep_assert_held(&sdata->local->mtx);

	sdata->u.mgd.flags &= ~IEEE80211_STA_CONNECTION_POLL;
	ieee80211_run_deferred_scan(sdata->local);
}

static void ieee80211_stop_poll(struct ieee80211_sub_if_data *sdata)
{
	mutex_lock(&sdata->local->mtx);
	__ieee80211_stop_poll(sdata);
	mutex_unlock(&sdata->local->mtx);
>>>>>>> refs/remotes/origin/master
}

static u32 ieee80211_handle_bss_capability(struct ieee80211_sub_if_data *sdata,
					   u16 capab, bool erp_valid, u8 erp)
{
	struct ieee80211_bss_conf *bss_conf = &sdata->vif.bss_conf;
	u32 changed = 0;
	bool use_protection;
	bool use_short_preamble;
	bool use_short_slot;

	if (erp_valid) {
		use_protection = (erp & WLAN_ERP_USE_PROTECTION) != 0;
		use_short_preamble = (erp & WLAN_ERP_BARKER_PREAMBLE) == 0;
	} else {
		use_protection = false;
		use_short_preamble = !!(capab & WLAN_CAPABILITY_SHORT_PREAMBLE);
	}

	use_short_slot = !!(capab & WLAN_CAPABILITY_SHORT_SLOT_TIME);
<<<<<<< HEAD
	if (sdata->local->hw.conf.channel->band == IEEE80211_BAND_5GHZ)
=======
	if (ieee80211_get_sdata_band(sdata) == IEEE80211_BAND_5GHZ)
>>>>>>> refs/remotes/origin/master
		use_short_slot = true;

	if (use_protection != bss_conf->use_cts_prot) {
		bss_conf->use_cts_prot = use_protection;
		changed |= BSS_CHANGED_ERP_CTS_PROT;
	}

	if (use_short_preamble != bss_conf->use_short_preamble) {
		bss_conf->use_short_preamble = use_short_preamble;
		changed |= BSS_CHANGED_ERP_PREAMBLE;
	}

	if (use_short_slot != bss_conf->use_short_slot) {
		bss_conf->use_short_slot = use_short_slot;
		changed |= BSS_CHANGED_ERP_SLOT;
	}

	return changed;
}

static void ieee80211_set_associated(struct ieee80211_sub_if_data *sdata,
				     struct cfg80211_bss *cbss,
				     u32 bss_info_changed)
{
	struct ieee80211_bss *bss = (void *)cbss->priv;
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_bss_conf *bss_conf = &sdata->vif.bss_conf;

	bss_info_changed |= BSS_CHANGED_ASSOC;
<<<<<<< HEAD
	/* set timing information */
	bss_conf->beacon_int = cbss->beacon_interval;
<<<<<<< HEAD
	bss_conf->timestamp = cbss->tsf;
=======
	bss_conf->last_tsf = cbss->tsf;
>>>>>>> refs/remotes/origin/cm-10.0

	bss_info_changed |= BSS_CHANGED_BEACON_INT;
	bss_info_changed |= ieee80211_handle_bss_capability(sdata,
		cbss->capability, bss->has_erp_value, bss->erp_value);

	sdata->u.mgd.beacon_timeout = usecs_to_jiffies(ieee80211_tu_to_usec(
		IEEE80211_BEACON_LOSS_COUNT * bss_conf->beacon_int));
=======
	bss_info_changed |= ieee80211_handle_bss_capability(sdata,
		bss_conf->assoc_capability, bss->has_erp_value, bss->erp_value);

	sdata->u.mgd.beacon_timeout = usecs_to_jiffies(ieee80211_tu_to_usec(
		beacon_loss_count * bss_conf->beacon_int));
>>>>>>> refs/remotes/origin/master

	sdata->u.mgd.associated = cbss;
	memcpy(sdata->u.mgd.bssid, cbss->bssid, ETH_ALEN);

	sdata->u.mgd.flags |= IEEE80211_STA_RESET_SIGNAL_AVE;

<<<<<<< HEAD
	/* just to be sure */
	sdata->u.mgd.flags &= ~(IEEE80211_STA_CONNECTION_POLL |
				IEEE80211_STA_BEACON_POLL);

	ieee80211_led_assoc(local, 1);

	if (local->hw.flags & IEEE80211_HW_NEED_DTIM_PERIOD)
		bss_conf->dtim_period = bss->dtim_period;
	else
		bss_conf->dtim_period = 0;

	bss_conf->assoc = 1;
<<<<<<< HEAD
	/*
	 * For now just always ask the driver to update the basic rateset
	 * when we have associated, we aren't checking whether it actually
	 * changed or not.
	 */
	bss_info_changed |= BSS_CHANGED_BASIC_RATES;

	/* And the BSSID changed - we're associated now */
	bss_info_changed |= BSS_CHANGED_BSSID;

	/* Tell the driver to monitor connection quality (if supported) */
	if ((local->hw.flags & IEEE80211_HW_SUPPORTS_CQM_RSSI) &&
=======

	/* Tell the driver to monitor connection quality (if supported) */
	if (sdata->vif.driver_flags & IEEE80211_VIF_SUPPORTS_CQM_RSSI &&
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (sdata->vif.p2p) {
		const struct cfg80211_bss_ies *ies;

		rcu_read_lock();
		ies = rcu_dereference(cbss->ies);
		if (ies) {
			int ret;

			ret = cfg80211_get_p2p_attr(
					ies->data, ies->len,
					IEEE80211_P2P_ATTR_ABSENCE_NOTICE,
					(u8 *) &bss_conf->p2p_noa_attr,
					sizeof(bss_conf->p2p_noa_attr));
			if (ret >= 2) {
				sdata->u.mgd.p2p_noa_index =
					bss_conf->p2p_noa_attr.index;
				bss_info_changed |= BSS_CHANGED_P2P_PS;
			}
		}
		rcu_read_unlock();
	}

	/* just to be sure */
	ieee80211_stop_poll(sdata);

	ieee80211_led_assoc(local, 1);

	if (sdata->u.mgd.have_beacon) {
		/*
		 * If the AP is buggy we may get here with no DTIM period
		 * known, so assume it's 1 which is the only safe assumption
		 * in that case, although if the TIM IE is broken powersave
		 * probably just won't work at all.
		 */
		bss_conf->dtim_period = sdata->u.mgd.dtim_period ?: 1;
		bss_conf->beacon_rate = bss->beacon_rate;
		bss_info_changed |= BSS_CHANGED_BEACON_INFO;
	} else {
		bss_conf->beacon_rate = NULL;
		bss_conf->dtim_period = 0;
	}

	bss_conf->assoc = 1;

	/* Tell the driver to monitor connection quality (if supported) */
	if (sdata->vif.driver_flags & IEEE80211_VIF_SUPPORTS_CQM_RSSI &&
>>>>>>> refs/remotes/origin/master
	    bss_conf->cqm_rssi_thold)
		bss_info_changed |= BSS_CHANGED_CQM;

	/* Enable ARP filtering */
<<<<<<< HEAD
	if (bss_conf->arp_filter_enabled != sdata->arp_filter_state) {
		bss_conf->arp_filter_enabled = sdata->arp_filter_state;
		bss_info_changed |= BSS_CHANGED_ARP_FILTER;
	}
=======
	if (bss_conf->arp_addr_cnt)
		bss_info_changed |= BSS_CHANGED_ARP_FILTER;
>>>>>>> refs/remotes/origin/master

	ieee80211_bss_info_change_notify(sdata, bss_info_changed);

	mutex_lock(&local->iflist_mtx);
	ieee80211_recalc_ps(local, -1);
<<<<<<< HEAD
	ieee80211_recalc_smps(local);
	mutex_unlock(&local->iflist_mtx);

	netif_tx_start_all_queues(sdata->dev);
=======
	mutex_unlock(&local->iflist_mtx);

	ieee80211_recalc_smps(sdata);
	ieee80211_recalc_ps_vif(sdata);

>>>>>>> refs/remotes/origin/master
	netif_carrier_on(sdata->dev);
}

static void ieee80211_set_disassoc(struct ieee80211_sub_if_data *sdata,
<<<<<<< HEAD
<<<<<<< HEAD
				   bool remove_sta, bool tx)
=======
				   u16 stype, u16 reason, bool tx,
				   u8 *frame_buf)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct ieee80211_local *local = sdata->local;
	struct sta_info *sta;
<<<<<<< HEAD
	u32 changed = 0, config_changed = 0;
	u8 bssid[ETH_ALEN];

	ASSERT_MGD_MTX(ifmgd);

	if (WARN_ON(!ifmgd->associated))
		return;

	memcpy(bssid, ifmgd->associated->bssid, ETH_ALEN);

	ifmgd->associated = NULL;
	memset(ifmgd->bssid, 0, ETH_ALEN);
=======
	u32 changed = 0;

	ASSERT_MGD_MTX(ifmgd);
=======
				   u16 stype, u16 reason, bool tx,
				   u8 *frame_buf)
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct ieee80211_local *local = sdata->local;
	u32 changed = 0;

	sdata_assert_lock(sdata);
>>>>>>> refs/remotes/origin/master

	if (WARN_ON_ONCE(tx && !frame_buf))
		return;

	if (WARN_ON(!ifmgd->associated))
		return;

<<<<<<< HEAD
	ifmgd->associated = NULL;
>>>>>>> refs/remotes/origin/cm-10.0

	/*
	 * we need to commit the associated = NULL change because the
	 * scan code uses that to determine whether this iface should
	 * go to/wake up from powersave or not -- and could otherwise
	 * wake the queues erroneously.
	 */
	smp_mb();

	/*
	 * Thus, we can only afterwards stop the queues -- to account
	 * for the case where another CPU is finishing a scan at this
	 * time -- we don't want the scan code to enable queues.
	 */

	netif_tx_stop_all_queues(sdata->dev);
	netif_carrier_off(sdata->dev);

	mutex_lock(&local->sta_mtx);
<<<<<<< HEAD
	sta = sta_info_get(sdata, bssid);
	if (sta) {
		set_sta_flags(sta, WLAN_STA_BLOCK_BA);
=======
	sta = sta_info_get(sdata, ifmgd->bssid);
	if (sta) {
		set_sta_flag(sta, WLAN_STA_BLOCK_BA);
>>>>>>> refs/remotes/origin/cm-10.0
		ieee80211_sta_tear_down_BA_sessions(sta, tx);
	}
	mutex_unlock(&local->sta_mtx);

<<<<<<< HEAD
=======
=======
	ieee80211_stop_poll(sdata);

	ifmgd->associated = NULL;
	netif_carrier_off(sdata->dev);

	/*
	 * if we want to get out of ps before disassoc (why?) we have
	 * to do it before sending disassoc, as otherwise the null-packet
	 * won't be valid.
	 */
	if (local->hw.conf.flags & IEEE80211_CONF_PS) {
		local->hw.conf.flags &= ~IEEE80211_CONF_PS;
		ieee80211_hw_config(local, IEEE80211_CONF_CHANGE_PS);
	}
	local->ps_sdata = NULL;

	/* disable per-vif ps */
	ieee80211_recalc_ps_vif(sdata);

	/* flush out any pending frame (e.g. DELBA) before deauth/disassoc */
	if (tx)
		ieee80211_flush_queues(local, sdata);

>>>>>>> refs/remotes/origin/master
	/* deauthenticate/disassociate now */
	if (tx || frame_buf)
		ieee80211_send_deauth_disassoc(sdata, ifmgd->bssid, stype,
					       reason, tx, frame_buf);

	/* flush out frame */
	if (tx)
<<<<<<< HEAD
		drv_flush(local, false);
=======
		ieee80211_flush_queues(local, sdata);
>>>>>>> refs/remotes/origin/master

	/* clear bssid only after building the needed mgmt frames */
	memset(ifmgd->bssid, 0, ETH_ALEN);

	/* remove AP and TDLS peers */
<<<<<<< HEAD
	sta_info_flush(local, sdata);

	/* finally reset all BSS / config parameters */
>>>>>>> refs/remotes/origin/cm-10.0
=======
	sta_info_flush_defer(sdata);

	/* finally reset all BSS / config parameters */
>>>>>>> refs/remotes/origin/master
	changed |= ieee80211_reset_erp_info(sdata);

	ieee80211_led_assoc(local, 0);
	changed |= BSS_CHANGED_ASSOC;
	sdata->vif.bss_conf.assoc = false;

<<<<<<< HEAD
<<<<<<< HEAD
	ieee80211_set_wmm_default(sdata);

	/* channel(_type) changes are handled by ieee80211_hw_config */
	WARN_ON(!ieee80211_set_channel_type(local, sdata, NL80211_CHAN_NO_HT));

	/* on the next assoc, re-program HT parameters */
	sdata->ht_opmode_valid = false;
=======
	/* on the next assoc, re-program HT parameters */
	sdata->ht_opmode_valid = false;
	memset(&ifmgd->ht_capa, 0, sizeof(ifmgd->ht_capa));
	memset(&ifmgd->ht_capa_mask, 0, sizeof(ifmgd->ht_capa_mask));
>>>>>>> refs/remotes/origin/cm-10.0

	local->power_constr_level = 0;
=======
	ifmgd->p2p_noa_index = -1;
	memset(&sdata->vif.bss_conf.p2p_noa_attr, 0,
	       sizeof(sdata->vif.bss_conf.p2p_noa_attr));

	/* on the next assoc, re-program HT/VHT parameters */
	memset(&ifmgd->ht_capa, 0, sizeof(ifmgd->ht_capa));
	memset(&ifmgd->ht_capa_mask, 0, sizeof(ifmgd->ht_capa_mask));
	memset(&ifmgd->vht_capa, 0, sizeof(ifmgd->vht_capa));
	memset(&ifmgd->vht_capa_mask, 0, sizeof(ifmgd->vht_capa_mask));

	sdata->ap_power_level = IEEE80211_UNSET_POWER_LEVEL;
>>>>>>> refs/remotes/origin/master

	del_timer_sync(&local->dynamic_ps_timer);
	cancel_work_sync(&local->dynamic_ps_enable_work);

<<<<<<< HEAD
	if (local->hw.conf.flags & IEEE80211_CONF_PS) {
		local->hw.conf.flags &= ~IEEE80211_CONF_PS;
<<<<<<< HEAD
		config_changed |= IEEE80211_CONF_CHANGE_PS;
	}
	local->ps_sdata = NULL;

	ieee80211_hw_config(local, config_changed);

=======
		ieee80211_hw_config(local, IEEE80211_CONF_CHANGE_PS);
	}
	local->ps_sdata = NULL;

>>>>>>> refs/remotes/origin/cm-10.0
	/* Disable ARP filtering */
	if (sdata->vif.bss_conf.arp_filter_enabled) {
		sdata->vif.bss_conf.arp_filter_enabled = false;
		changed |= BSS_CHANGED_ARP_FILTER;
	}

<<<<<<< HEAD
=======
	sdata->vif.bss_conf.qos = false;
	changed |= BSS_CHANGED_QOS;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* Disable ARP filtering */
	if (sdata->vif.bss_conf.arp_addr_cnt)
		changed |= BSS_CHANGED_ARP_FILTER;

	sdata->vif.bss_conf.qos = false;
	changed |= BSS_CHANGED_QOS;

>>>>>>> refs/remotes/origin/master
	/* The BSSID (not really interesting) and HT changed */
	changed |= BSS_CHANGED_BSSID | BSS_CHANGED_HT;
	ieee80211_bss_info_change_notify(sdata, changed);

<<<<<<< HEAD
<<<<<<< HEAD
	if (remove_sta)
		sta_info_destroy_addr(sdata, bssid);
=======
	/* channel(_type) changes are handled by ieee80211_hw_config */
	WARN_ON(!ieee80211_set_channel_type(local, sdata, NL80211_CHAN_NO_HT));
	ieee80211_hw_config(local, 0);

	/* disassociated - set to defaults now */
	ieee80211_set_wmm_default(sdata, false);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* disassociated - set to defaults now */
	ieee80211_set_wmm_default(sdata, false);
>>>>>>> refs/remotes/origin/master

	del_timer_sync(&sdata->u.mgd.conn_mon_timer);
	del_timer_sync(&sdata->u.mgd.bcn_mon_timer);
	del_timer_sync(&sdata->u.mgd.timer);
	del_timer_sync(&sdata->u.mgd.chswitch_timer);
<<<<<<< HEAD
=======

	sdata->vif.bss_conf.dtim_period = 0;
	sdata->vif.bss_conf.beacon_rate = NULL;

	ifmgd->have_beacon = false;

	ifmgd->flags = 0;
	ieee80211_vif_release_channel(sdata);
>>>>>>> refs/remotes/origin/master
}

void ieee80211_sta_rx_notify(struct ieee80211_sub_if_data *sdata,
			     struct ieee80211_hdr *hdr)
{
	/*
	 * We can postpone the mgd.timer whenever receiving unicast frames
	 * from AP because we know that the connection is working both ways
	 * at that time. But multicast frames (and hence also beacons) must
	 * be ignored here, because we need to trigger the timer during
	 * data idle periods for sending the periodic probe request to the
	 * AP we're connected to.
	 */
	if (is_multicast_ether_addr(hdr->addr1))
		return;

	ieee80211_sta_reset_conn_monitor(sdata);
}

static void ieee80211_reset_ap_probe(struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
<<<<<<< HEAD

	if (!(ifmgd->flags & (IEEE80211_STA_BEACON_POLL |
			      IEEE80211_STA_CONNECTION_POLL)))
	    return;

	ifmgd->flags &= ~(IEEE80211_STA_CONNECTION_POLL |
			  IEEE80211_STA_BEACON_POLL);
	mutex_lock(&sdata->local->iflist_mtx);
	ieee80211_recalc_ps(sdata->local, -1);
	mutex_unlock(&sdata->local->iflist_mtx);

	if (sdata->local->hw.flags & IEEE80211_HW_CONNECTION_MONITOR)
		return;
=======
	struct ieee80211_local *local = sdata->local;

	mutex_lock(&local->mtx);
	if (!(ifmgd->flags & IEEE80211_STA_CONNECTION_POLL))
		goto out;

	__ieee80211_stop_poll(sdata);

	mutex_lock(&local->iflist_mtx);
	ieee80211_recalc_ps(local, -1);
	mutex_unlock(&local->iflist_mtx);

	if (sdata->local->hw.flags & IEEE80211_HW_CONNECTION_MONITOR)
		goto out;
>>>>>>> refs/remotes/origin/master

	/*
	 * We've received a probe response, but are not sure whether
	 * we have or will be receiving any beacons or data, so let's
	 * schedule the timers again, just in case.
	 */
	ieee80211_sta_reset_beacon_monitor(sdata);

	mod_timer(&ifmgd->conn_mon_timer,
		  round_jiffies_up(jiffies +
				   IEEE80211_CONNECTION_IDLE_TIME));
<<<<<<< HEAD
=======
out:
	mutex_unlock(&local->mtx);
>>>>>>> refs/remotes/origin/master
}

void ieee80211_sta_tx_notify(struct ieee80211_sub_if_data *sdata,
			     struct ieee80211_hdr *hdr, bool ack)
{
	if (!ieee80211_is_data(hdr->frame_control))
	    return;

<<<<<<< HEAD
	if (ack)
		ieee80211_sta_reset_conn_monitor(sdata);

	if (ieee80211_is_nullfunc(hdr->frame_control) &&
	    sdata->u.mgd.probe_send_count > 0) {
		if (ack)
			sdata->u.mgd.probe_send_count = 0;
		else
			sdata->u.mgd.nullfunc_failed = true;
		ieee80211_queue_work(&sdata->local->hw, &sdata->work);
	}
=======
	if (ieee80211_is_nullfunc(hdr->frame_control) &&
	    sdata->u.mgd.probe_send_count > 0) {
		if (ack)
			ieee80211_sta_reset_conn_monitor(sdata);
		else
			sdata->u.mgd.nullfunc_failed = true;
		ieee80211_queue_work(&sdata->local->hw, &sdata->work);
		return;
	}

	if (ack)
		ieee80211_sta_reset_conn_monitor(sdata);
>>>>>>> refs/remotes/origin/master
}

static void ieee80211_mgd_probe_ap_send(struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	const u8 *ssid;
	u8 *dst = ifmgd->associated->bssid;
	u8 unicast_limit = max(1, max_probe_tries - 3);

	/*
	 * Try sending broadcast probe requests for the last three
	 * probe requests after the first ones failed since some
	 * buggy APs only support broadcast probe requests.
	 */
	if (ifmgd->probe_send_count >= unicast_limit)
		dst = NULL;

	/*
	 * When the hardware reports an accurate Tx ACK status, it's
	 * better to send a nullfunc frame instead of a probe request,
	 * as it will kick us off the AP quickly if we aren't associated
	 * anymore. The timeout will be reset if the frame is ACKed by
	 * the AP.
	 */
<<<<<<< HEAD
=======
	ifmgd->probe_send_count++;

>>>>>>> refs/remotes/origin/master
	if (sdata->local->hw.flags & IEEE80211_HW_REPORTS_TX_ACK_STATUS) {
		ifmgd->nullfunc_failed = false;
		ieee80211_send_nullfunc(sdata->local, sdata, 0);
	} else {
<<<<<<< HEAD
		ssid = ieee80211_bss_get_ie(ifmgd->associated, WLAN_EID_SSID);
<<<<<<< HEAD
		ieee80211_send_probe_req(sdata, dst, ssid + 2, ssid[1], NULL, 0);
=======
		ieee80211_send_probe_req(sdata, dst, ssid + 2, ssid[1], NULL, 0,
					 (u32) -1, true, false);
>>>>>>> refs/remotes/origin/cm-10.0
	}

	ifmgd->probe_send_count++;
	ifmgd->probe_timeout = jiffies + msecs_to_jiffies(probe_wait_ms);
	run_again(ifmgd, ifmgd->probe_timeout);
=======
		int ssid_len;

		rcu_read_lock();
		ssid = ieee80211_bss_get_ie(ifmgd->associated, WLAN_EID_SSID);
		if (WARN_ON_ONCE(ssid == NULL))
			ssid_len = 0;
		else
			ssid_len = ssid[1];

		ieee80211_send_probe_req(sdata, dst, ssid + 2, ssid_len, NULL,
					 0, (u32) -1, true, 0,
					 ifmgd->associated->channel, false);
		rcu_read_unlock();
	}

	ifmgd->probe_timeout = jiffies + msecs_to_jiffies(probe_wait_ms);
	run_again(sdata, ifmgd->probe_timeout);
	if (sdata->local->hw.flags & IEEE80211_HW_REPORTS_TX_ACK_STATUS)
		ieee80211_flush_queues(sdata->local, sdata);
>>>>>>> refs/remotes/origin/master
}

static void ieee80211_mgd_probe_ap(struct ieee80211_sub_if_data *sdata,
				   bool beacon)
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	bool already = false;

	if (!ieee80211_sdata_running(sdata))
		return;

<<<<<<< HEAD
	if (sdata->local->scanning)
		return;

	if (sdata->local->tmp_channel)
		return;

	mutex_lock(&ifmgd->mtx);

	if (!ifmgd->associated)
		goto out;

#ifdef CONFIG_MAC80211_VERBOSE_DEBUG
	if (beacon && net_ratelimit())
		printk(KERN_DEBUG "%s: detected beacon loss from AP "
		       "- sending probe request\n", sdata->name);
#endif
=======
	sdata_lock(sdata);

	if (!ifmgd->associated)
		goto out;

	mutex_lock(&sdata->local->mtx);

	if (sdata->local->tmp_channel || sdata->local->scanning) {
		mutex_unlock(&sdata->local->mtx);
		goto out;
	}

	if (beacon) {
		mlme_dbg_ratelimited(sdata,
				     "detected beacon loss from AP (missed %d beacons) - probing\n",
				     beacon_loss_count);

		ieee80211_cqm_rssi_notify(&sdata->vif,
					  NL80211_CQM_RSSI_BEACON_LOSS_EVENT,
					  GFP_KERNEL);
	}
>>>>>>> refs/remotes/origin/master

	/*
	 * The driver/our work has already reported this event or the
	 * connection monitoring has kicked in and we have already sent
	 * a probe request. Or maybe the AP died and the driver keeps
	 * reporting until we disassociate...
	 *
	 * In either case we have to ignore the current call to this
	 * function (except for setting the correct probe reason bit)
	 * because otherwise we would reset the timer every time and
	 * never check whether we received a probe response!
	 */
<<<<<<< HEAD
	if (ifmgd->flags & (IEEE80211_STA_BEACON_POLL |
			    IEEE80211_STA_CONNECTION_POLL))
		already = true;

	if (beacon)
		ifmgd->flags |= IEEE80211_STA_BEACON_POLL;
	else
		ifmgd->flags |= IEEE80211_STA_CONNECTION_POLL;
=======
	if (ifmgd->flags & IEEE80211_STA_CONNECTION_POLL)
		already = true;

	ifmgd->flags |= IEEE80211_STA_CONNECTION_POLL;

	mutex_unlock(&sdata->local->mtx);
>>>>>>> refs/remotes/origin/master

	if (already)
		goto out;

	mutex_lock(&sdata->local->iflist_mtx);
	ieee80211_recalc_ps(sdata->local, -1);
	mutex_unlock(&sdata->local->iflist_mtx);

	ifmgd->probe_send_count = 0;
	ieee80211_mgd_probe_ap_send(sdata);
 out:
<<<<<<< HEAD
	mutex_unlock(&ifmgd->mtx);
=======
	sdata_unlock(sdata);
>>>>>>> refs/remotes/origin/master
}

struct sk_buff *ieee80211_ap_probereq_get(struct ieee80211_hw *hw,
					  struct ieee80211_vif *vif)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
<<<<<<< HEAD
	struct sk_buff *skb;
	const u8 *ssid;
=======
	struct cfg80211_bss *cbss;
	struct sk_buff *skb;
	const u8 *ssid;
	int ssid_len;
>>>>>>> refs/remotes/origin/master

	if (WARN_ON(sdata->vif.type != NL80211_IFTYPE_STATION))
		return NULL;

<<<<<<< HEAD
	ASSERT_MGD_MTX(ifmgd);

	if (!ifmgd->associated)
		return NULL;

	ssid = ieee80211_bss_get_ie(ifmgd->associated, WLAN_EID_SSID);
	skb = ieee80211_build_probe_req(sdata, ifmgd->associated->bssid,
<<<<<<< HEAD
					ssid + 2, ssid[1], NULL, 0);
=======
					(u32) -1, ssid + 2, ssid[1],
					NULL, 0, true);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	sdata_assert_lock(sdata);

	if (ifmgd->associated)
		cbss = ifmgd->associated;
	else if (ifmgd->auth_data)
		cbss = ifmgd->auth_data->bss;
	else if (ifmgd->assoc_data)
		cbss = ifmgd->assoc_data->bss;
	else
		return NULL;

	rcu_read_lock();
	ssid = ieee80211_bss_get_ie(cbss, WLAN_EID_SSID);
	if (WARN_ON_ONCE(ssid == NULL))
		ssid_len = 0;
	else
		ssid_len = ssid[1];

	skb = ieee80211_build_probe_req(sdata, cbss->bssid,
					(u32) -1, cbss->channel,
					ssid + 2, ssid_len,
					NULL, 0, true);
	rcu_read_unlock();
>>>>>>> refs/remotes/origin/master

	return skb;
}
EXPORT_SYMBOL(ieee80211_ap_probereq_get);

<<<<<<< HEAD
static void __ieee80211_connection_loss(struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct ieee80211_local *local = sdata->local;
	u8 bssid[ETH_ALEN];
<<<<<<< HEAD
=======
	u8 frame_buf[DEAUTH_DISASSOC_LEN];
>>>>>>> refs/remotes/origin/cm-10.0

	mutex_lock(&ifmgd->mtx);
	if (!ifmgd->associated) {
		mutex_unlock(&ifmgd->mtx);
		return;
	}

	memcpy(bssid, ifmgd->associated->bssid, ETH_ALEN);

	printk(KERN_DEBUG "%s: Connection to AP %pM lost.\n",
	       sdata->name, bssid);

<<<<<<< HEAD
	ieee80211_set_disassoc(sdata, true, true);
	mutex_unlock(&ifmgd->mtx);

	mutex_lock(&local->mtx);
	ieee80211_recalc_idle(local);
	mutex_unlock(&local->mtx);
=======
	ieee80211_set_disassoc(sdata, IEEE80211_STYPE_DEAUTH,
			       WLAN_REASON_DISASSOC_DUE_TO_INACTIVITY,
			       false, frame_buf);
	mutex_unlock(&ifmgd->mtx);

>>>>>>> refs/remotes/origin/cm-10.0
	/*
	 * must be outside lock due to cfg80211,
	 * but that's not a problem.
	 */
<<<<<<< HEAD
	ieee80211_send_deauth_disassoc(sdata, bssid,
				       IEEE80211_STYPE_DEAUTH,
				       WLAN_REASON_DISASSOC_DUE_TO_INACTIVITY,
				       NULL, true);
=======
	cfg80211_send_deauth(sdata->dev, frame_buf, DEAUTH_DISASSOC_LEN);

	mutex_lock(&local->mtx);
	ieee80211_recalc_idle(local);
	mutex_unlock(&local->mtx);
>>>>>>> refs/remotes/origin/cm-10.0
}

void ieee80211_beacon_connection_loss_work(struct work_struct *work)
=======
static void __ieee80211_disconnect(struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	u8 frame_buf[IEEE80211_DEAUTH_FRAME_LEN];

	sdata_lock(sdata);
	if (!ifmgd->associated) {
		sdata_unlock(sdata);
		return;
	}

	ieee80211_set_disassoc(sdata, IEEE80211_STYPE_DEAUTH,
			       WLAN_REASON_DISASSOC_DUE_TO_INACTIVITY,
			       true, frame_buf);
	ifmgd->flags &= ~IEEE80211_STA_CSA_RECEIVED;
	sdata->vif.csa_active = false;
	ieee80211_wake_queues_by_reason(&sdata->local->hw,
					IEEE80211_MAX_QUEUE_MAP,
					IEEE80211_QUEUE_STOP_REASON_CSA);

	cfg80211_tx_mlme_mgmt(sdata->dev, frame_buf,
			      IEEE80211_DEAUTH_FRAME_LEN);
	sdata_unlock(sdata);
}

static void ieee80211_beacon_connection_loss_work(struct work_struct *work)
>>>>>>> refs/remotes/origin/master
{
	struct ieee80211_sub_if_data *sdata =
		container_of(work, struct ieee80211_sub_if_data,
			     u.mgd.beacon_connection_loss_work);
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct sta_info *sta;

	if (ifmgd->associated) {
		rcu_read_lock();
		sta = sta_info_get(sdata, ifmgd->bssid);
		if (sta)
			sta->beacon_loss_count++;
		rcu_read_unlock();
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

	if (sdata->local->hw.flags & IEEE80211_HW_CONNECTION_MONITOR)
		__ieee80211_connection_loss(sdata);
	else
		ieee80211_mgd_probe_ap(sdata, true);
=======

	if (ifmgd->connection_loss) {
		sdata_info(sdata, "Connection to AP %pM lost\n",
			   ifmgd->bssid);
		__ieee80211_disconnect(sdata);
	} else {
		ieee80211_mgd_probe_ap(sdata, true);
	}
}

static void ieee80211_csa_connection_drop_work(struct work_struct *work)
{
	struct ieee80211_sub_if_data *sdata =
		container_of(work, struct ieee80211_sub_if_data,
			     u.mgd.csa_connection_drop_work);

	__ieee80211_disconnect(sdata);
>>>>>>> refs/remotes/origin/master
}

void ieee80211_beacon_loss(struct ieee80211_vif *vif)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);
	struct ieee80211_hw *hw = &sdata->local->hw;

	trace_api_beacon_loss(sdata);

<<<<<<< HEAD
	WARN_ON(hw->flags & IEEE80211_HW_CONNECTION_MONITOR);
=======
	sdata->u.mgd.connection_loss = false;
>>>>>>> refs/remotes/origin/master
	ieee80211_queue_work(hw, &sdata->u.mgd.beacon_connection_loss_work);
}
EXPORT_SYMBOL(ieee80211_beacon_loss);

void ieee80211_connection_loss(struct ieee80211_vif *vif)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);
	struct ieee80211_hw *hw = &sdata->local->hw;

	trace_api_connection_loss(sdata);

<<<<<<< HEAD
	WARN_ON(!(hw->flags & IEEE80211_HW_CONNECTION_MONITOR));
=======
	sdata->u.mgd.connection_loss = true;
>>>>>>> refs/remotes/origin/master
	ieee80211_queue_work(hw, &sdata->u.mgd.beacon_connection_loss_work);
}
EXPORT_SYMBOL(ieee80211_connection_loss);


<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static void ieee80211_destroy_auth_data(struct ieee80211_sub_if_data *sdata,
					bool assoc)
{
	struct ieee80211_mgd_auth_data *auth_data = sdata->u.mgd.auth_data;

<<<<<<< HEAD
	lockdep_assert_held(&sdata->u.mgd.mtx);
=======
	sdata_assert_lock(sdata);
>>>>>>> refs/remotes/origin/master

	if (!assoc) {
		sta_info_destroy_addr(sdata, auth_data->bss->bssid);

		memset(sdata->u.mgd.bssid, 0, ETH_ALEN);
		ieee80211_bss_info_change_notify(sdata, BSS_CHANGED_BSSID);
<<<<<<< HEAD
	}

	cfg80211_put_bss(auth_data->bss);
=======
		sdata->u.mgd.flags = 0;
		ieee80211_vif_release_channel(sdata);
	}

	cfg80211_put_bss(sdata->local->hw.wiphy, auth_data->bss);
>>>>>>> refs/remotes/origin/master
	kfree(auth_data);
	sdata->u.mgd.auth_data = NULL;
}

static void ieee80211_auth_challenge(struct ieee80211_sub_if_data *sdata,
				     struct ieee80211_mgmt *mgmt, size_t len)
{
<<<<<<< HEAD
	struct ieee80211_mgd_auth_data *auth_data = sdata->u.mgd.auth_data;
	u8 *pos;
	struct ieee802_11_elems elems;

	pos = mgmt->u.auth.variable;
	ieee802_11_parse_elems(pos, len - (pos - (u8 *) mgmt), &elems);
	if (!elems.challenge)
		return;
	auth_data->expected_transaction = 4;
	ieee80211_send_auth(sdata, 3, auth_data->algorithm,
			    elems.challenge - 2, elems.challenge_len + 2,
			    auth_data->bss->bssid, auth_data->bss->bssid,
			    auth_data->key, auth_data->key_len,
			    auth_data->key_idx);
}

static enum rx_mgmt_action __must_check
ieee80211_rx_mgmt_auth(struct ieee80211_sub_if_data *sdata,
		       struct ieee80211_mgmt *mgmt, size_t len)
=======
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_mgd_auth_data *auth_data = sdata->u.mgd.auth_data;
	u8 *pos;
	struct ieee802_11_elems elems;
	u32 tx_flags = 0;

	pos = mgmt->u.auth.variable;
	ieee802_11_parse_elems(pos, len - (pos - (u8 *) mgmt), false, &elems);
	if (!elems.challenge)
		return;
	auth_data->expected_transaction = 4;
	drv_mgd_prepare_tx(sdata->local, sdata);
	if (local->hw.flags & IEEE80211_HW_REPORTS_TX_ACK_STATUS)
		tx_flags = IEEE80211_TX_CTL_REQ_TX_STATUS |
			   IEEE80211_TX_INTFL_MLME_CONN_TX;
	ieee80211_send_auth(sdata, 3, auth_data->algorithm, 0,
			    elems.challenge - 2, elems.challenge_len + 2,
			    auth_data->bss->bssid, auth_data->bss->bssid,
			    auth_data->key, auth_data->key_len,
			    auth_data->key_idx, tx_flags);
}

static void ieee80211_rx_mgmt_auth(struct ieee80211_sub_if_data *sdata,
				   struct ieee80211_mgmt *mgmt, size_t len)
>>>>>>> refs/remotes/origin/master
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	u8 bssid[ETH_ALEN];
	u16 auth_alg, auth_transaction, status_code;
	struct sta_info *sta;

<<<<<<< HEAD
	lockdep_assert_held(&ifmgd->mtx);

	if (len < 24 + 6)
		return RX_MGMT_NONE;

	if (!ifmgd->auth_data || ifmgd->auth_data->done)
		return RX_MGMT_NONE;

	memcpy(bssid, ifmgd->auth_data->bss->bssid, ETH_ALEN);

	if (compare_ether_addr(bssid, mgmt->bssid))
		return RX_MGMT_NONE;
=======
	sdata_assert_lock(sdata);

	if (len < 24 + 6)
		return;

	if (!ifmgd->auth_data || ifmgd->auth_data->done)
		return;

	memcpy(bssid, ifmgd->auth_data->bss->bssid, ETH_ALEN);

	if (!ether_addr_equal(bssid, mgmt->bssid))
		return;
>>>>>>> refs/remotes/origin/master

	auth_alg = le16_to_cpu(mgmt->u.auth.auth_alg);
	auth_transaction = le16_to_cpu(mgmt->u.auth.auth_transaction);
	status_code = le16_to_cpu(mgmt->u.auth.status_code);

	if (auth_alg != ifmgd->auth_data->algorithm ||
<<<<<<< HEAD
	    auth_transaction != ifmgd->auth_data->expected_transaction)
		return RX_MGMT_NONE;

	if (status_code != WLAN_STATUS_SUCCESS) {
		printk(KERN_DEBUG "%s: %pM denied authentication (status %d)\n",
		       sdata->name, mgmt->sa, status_code);
		ieee80211_destroy_auth_data(sdata, false);
		return RX_MGMT_CFG80211_RX_AUTH;
=======
	    auth_transaction != ifmgd->auth_data->expected_transaction) {
		sdata_info(sdata, "%pM unexpected authentication state: alg %d (expected %d) transact %d (expected %d)\n",
			   mgmt->sa, auth_alg, ifmgd->auth_data->algorithm,
			   auth_transaction,
			   ifmgd->auth_data->expected_transaction);
		return;
	}

	if (status_code != WLAN_STATUS_SUCCESS) {
		sdata_info(sdata, "%pM denied authentication (status %d)\n",
			   mgmt->sa, status_code);
		ieee80211_destroy_auth_data(sdata, false);
		cfg80211_rx_mlme_mgmt(sdata->dev, (u8 *)mgmt, len);
		return;
>>>>>>> refs/remotes/origin/master
	}

	switch (ifmgd->auth_data->algorithm) {
	case WLAN_AUTH_OPEN:
	case WLAN_AUTH_LEAP:
	case WLAN_AUTH_FT:
<<<<<<< HEAD
=======
	case WLAN_AUTH_SAE:
>>>>>>> refs/remotes/origin/master
		break;
	case WLAN_AUTH_SHARED_KEY:
		if (ifmgd->auth_data->expected_transaction != 4) {
			ieee80211_auth_challenge(sdata, mgmt, len);
			/* need another frame */
<<<<<<< HEAD
			return RX_MGMT_NONE;
=======
			return;
>>>>>>> refs/remotes/origin/master
		}
		break;
	default:
		WARN_ONCE(1, "invalid auth alg %d",
			  ifmgd->auth_data->algorithm);
<<<<<<< HEAD
		return RX_MGMT_NONE;
	}

	printk(KERN_DEBUG "%s: authenticated\n", sdata->name);
	ifmgd->auth_data->done = true;
	ifmgd->auth_data->timeout = jiffies + IEEE80211_AUTH_WAIT_ASSOC;
	run_again(ifmgd, ifmgd->auth_data->timeout);
=======
		return;
	}

	sdata_info(sdata, "authenticated\n");
	ifmgd->auth_data->done = true;
	ifmgd->auth_data->timeout = jiffies + IEEE80211_AUTH_WAIT_ASSOC;
	ifmgd->auth_data->timeout_started = true;
	run_again(sdata, ifmgd->auth_data->timeout);

	if (ifmgd->auth_data->algorithm == WLAN_AUTH_SAE &&
	    ifmgd->auth_data->expected_transaction != 2) {
		/*
		 * Report auth frame to user space for processing since another
		 * round of Authentication frames is still needed.
		 */
		cfg80211_rx_mlme_mgmt(sdata->dev, (u8 *)mgmt, len);
		return;
	}
>>>>>>> refs/remotes/origin/master

	/* move station state to auth */
	mutex_lock(&sdata->local->sta_mtx);
	sta = sta_info_get(sdata, bssid);
	if (!sta) {
		WARN_ONCE(1, "%s: STA %pM not found", sdata->name, bssid);
		goto out_err;
	}
	if (sta_info_move_state(sta, IEEE80211_STA_AUTH)) {
<<<<<<< HEAD
		printk(KERN_DEBUG "%s: failed moving %pM to auth\n",
		       sdata->name, bssid);
=======
		sdata_info(sdata, "failed moving %pM to auth\n", bssid);
>>>>>>> refs/remotes/origin/master
		goto out_err;
	}
	mutex_unlock(&sdata->local->sta_mtx);

<<<<<<< HEAD
	return RX_MGMT_CFG80211_RX_AUTH;
 out_err:
	mutex_unlock(&sdata->local->sta_mtx);
	/* ignore frame -- wait for timeout */
	return RX_MGMT_NONE;
}


>>>>>>> refs/remotes/origin/cm-10.0
static enum rx_mgmt_action __must_check
ieee80211_rx_mgmt_deauth(struct ieee80211_sub_if_data *sdata,
			 struct ieee80211_mgmt *mgmt, size_t len)
=======
	cfg80211_rx_mlme_mgmt(sdata->dev, (u8 *)mgmt, len);
	return;
 out_err:
	mutex_unlock(&sdata->local->sta_mtx);
	/* ignore frame -- wait for timeout */
}


static void ieee80211_rx_mgmt_deauth(struct ieee80211_sub_if_data *sdata,
				     struct ieee80211_mgmt *mgmt, size_t len)
>>>>>>> refs/remotes/origin/master
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	const u8 *bssid = NULL;
	u16 reason_code;

<<<<<<< HEAD
<<<<<<< HEAD
	if (len < 24 + 2)
		return RX_MGMT_NONE;

	ASSERT_MGD_MTX(ifmgd);
=======
	lockdep_assert_held(&ifmgd->mtx);

	if (len < 24 + 2)
		return RX_MGMT_NONE;

	if (!ifmgd->associated ||
	    compare_ether_addr(mgmt->bssid, ifmgd->associated->bssid))
		return RX_MGMT_NONE;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	sdata_assert_lock(sdata);

	if (len < 24 + 2)
		return;

	if (!ifmgd->associated ||
	    !ether_addr_equal(mgmt->bssid, ifmgd->associated->bssid))
		return;
>>>>>>> refs/remotes/origin/master

	bssid = ifmgd->associated->bssid;

	reason_code = le16_to_cpu(mgmt->u.deauth.reason_code);

<<<<<<< HEAD
	printk(KERN_DEBUG "%s: deauthenticated from %pM (Reason: %u)\n",
			sdata->name, bssid, reason_code);

<<<<<<< HEAD
	ieee80211_set_disassoc(sdata, true, false);
=======
	ieee80211_set_disassoc(sdata, 0, 0, false, NULL);

>>>>>>> refs/remotes/origin/cm-10.0
	mutex_lock(&sdata->local->mtx);
	ieee80211_recalc_idle(sdata->local);
	mutex_unlock(&sdata->local->mtx);

	return RX_MGMT_CFG80211_DEAUTH;
}


static enum rx_mgmt_action __must_check
ieee80211_rx_mgmt_disassoc(struct ieee80211_sub_if_data *sdata,
			   struct ieee80211_mgmt *mgmt, size_t len)
=======
	sdata_info(sdata, "deauthenticated from %pM (Reason: %u)\n",
		   bssid, reason_code);

	ieee80211_set_disassoc(sdata, 0, 0, false, NULL);

	cfg80211_rx_mlme_mgmt(sdata->dev, (u8 *)mgmt, len);
}


static void ieee80211_rx_mgmt_disassoc(struct ieee80211_sub_if_data *sdata,
				       struct ieee80211_mgmt *mgmt, size_t len)
>>>>>>> refs/remotes/origin/master
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	u16 reason_code;

<<<<<<< HEAD
<<<<<<< HEAD
	if (len < 24 + 2)
		return RX_MGMT_NONE;

	ASSERT_MGD_MTX(ifmgd);

	if (WARN_ON(!ifmgd->associated))
		return RX_MGMT_NONE;

	if (WARN_ON(memcmp(ifmgd->associated->bssid, mgmt->sa, ETH_ALEN)))
=======
	lockdep_assert_held(&ifmgd->mtx);

	if (len < 24 + 2)
		return RX_MGMT_NONE;

	if (!ifmgd->associated ||
	    compare_ether_addr(mgmt->bssid, ifmgd->associated->bssid))
>>>>>>> refs/remotes/origin/cm-10.0
		return RX_MGMT_NONE;

	reason_code = le16_to_cpu(mgmt->u.disassoc.reason_code);

	printk(KERN_DEBUG "%s: disassociated from %pM (Reason: %u)\n",
			sdata->name, mgmt->sa, reason_code);

<<<<<<< HEAD
	ieee80211_set_disassoc(sdata, true, false);
	mutex_lock(&sdata->local->mtx);
	ieee80211_recalc_idle(sdata->local);
	mutex_unlock(&sdata->local->mtx);
	return RX_MGMT_CFG80211_DISASSOC;
}


static bool ieee80211_assoc_success(struct ieee80211_work *wk,
				    struct ieee80211_mgmt *mgmt, size_t len)
{
	struct ieee80211_sub_if_data *sdata = wk->sdata;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_supported_band *sband;
	struct sta_info *sta;
	struct cfg80211_bss *cbss = wk->assoc.bss;
	u8 *pos;
	u32 rates, basic_rates;
	u16 capab_info, aid;
	struct ieee802_11_elems elems;
	struct ieee80211_bss_conf *bss_conf = &sdata->vif.bss_conf;
	u32 changed = 0;
	int i, j, err;
	bool have_higher_than_11mbit = false;
=======
	ieee80211_set_disassoc(sdata, 0, 0, false, NULL);

	mutex_lock(&sdata->local->mtx);
	ieee80211_recalc_idle(sdata->local);
	mutex_unlock(&sdata->local->mtx);

	return RX_MGMT_CFG80211_DISASSOC;
=======
	sdata_assert_lock(sdata);

	if (len < 24 + 2)
		return;

	if (!ifmgd->associated ||
	    !ether_addr_equal(mgmt->bssid, ifmgd->associated->bssid))
		return;

	reason_code = le16_to_cpu(mgmt->u.disassoc.reason_code);

	sdata_info(sdata, "disassociated from %pM (Reason: %u)\n",
		   mgmt->sa, reason_code);

	ieee80211_set_disassoc(sdata, 0, 0, false, NULL);

	cfg80211_rx_mlme_mgmt(sdata->dev, (u8 *)mgmt, len);
>>>>>>> refs/remotes/origin/master
}

static void ieee80211_get_rates(struct ieee80211_supported_band *sband,
				u8 *supp_rates, unsigned int supp_rates_len,
				u32 *rates, u32 *basic_rates,
				bool *have_higher_than_11mbit,
<<<<<<< HEAD
				int *min_rate, int *min_rate_index)
=======
				int *min_rate, int *min_rate_index,
				int shift, u32 rate_flags)
>>>>>>> refs/remotes/origin/master
{
	int i, j;

	for (i = 0; i < supp_rates_len; i++) {
<<<<<<< HEAD
		int rate = (supp_rates[i] & 0x7f) * 5;
		bool is_basic = !!(supp_rates[i] & 0x80);

		if (rate > 110)
=======
		int rate = supp_rates[i] & 0x7f;
		bool is_basic = !!(supp_rates[i] & 0x80);

		if ((rate * 5 * (1 << shift)) > 110)
>>>>>>> refs/remotes/origin/master
			*have_higher_than_11mbit = true;

		/*
		 * BSS_MEMBERSHIP_SELECTOR_HT_PHY is defined in 802.11n-2009
		 * 7.3.2.2 as a magic value instead of a rate. Hence, skip it.
		 *
		 * Note: Even through the membership selector and the basic
		 *	 rate flag share the same bit, they are not exactly
		 *	 the same.
		 */
		if (!!(supp_rates[i] & 0x80) &&
		    (supp_rates[i] & 0x7f) == BSS_MEMBERSHIP_SELECTOR_HT_PHY)
			continue;

		for (j = 0; j < sband->n_bitrates; j++) {
<<<<<<< HEAD
			if (sband->bitrates[j].bitrate == rate) {
				*rates |= BIT(j);
				if (is_basic)
					*basic_rates |= BIT(j);
				if (rate < *min_rate) {
					*min_rate = rate;
=======
			struct ieee80211_rate *br;
			int brate;

			br = &sband->bitrates[j];
			if ((rate_flags & br->flags) != rate_flags)
				continue;

			brate = DIV_ROUND_UP(br->bitrate, (1 << shift) * 5);
			if (brate == rate) {
				*rates |= BIT(j);
				if (is_basic)
					*basic_rates |= BIT(j);
				if ((rate * 5) < *min_rate) {
					*min_rate = rate * 5;
>>>>>>> refs/remotes/origin/master
					*min_rate_index = j;
				}
				break;
			}
		}
	}
}

static void ieee80211_destroy_assoc_data(struct ieee80211_sub_if_data *sdata,
					 bool assoc)
{
	struct ieee80211_mgd_assoc_data *assoc_data = sdata->u.mgd.assoc_data;

<<<<<<< HEAD
	lockdep_assert_held(&sdata->u.mgd.mtx);
=======
	sdata_assert_lock(sdata);
>>>>>>> refs/remotes/origin/master

	if (!assoc) {
		sta_info_destroy_addr(sdata, assoc_data->bss->bssid);

		memset(sdata->u.mgd.bssid, 0, ETH_ALEN);
		ieee80211_bss_info_change_notify(sdata, BSS_CHANGED_BSSID);
<<<<<<< HEAD
=======
		sdata->u.mgd.flags = 0;
		ieee80211_vif_release_channel(sdata);
>>>>>>> refs/remotes/origin/master
	}

	kfree(assoc_data);
	sdata->u.mgd.assoc_data = NULL;
}

static bool ieee80211_assoc_success(struct ieee80211_sub_if_data *sdata,
				    struct cfg80211_bss *cbss,
				    struct ieee80211_mgmt *mgmt, size_t len)
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_supported_band *sband;
	struct sta_info *sta;
	u8 *pos;
	u16 capab_info, aid;
	struct ieee802_11_elems elems;
	struct ieee80211_bss_conf *bss_conf = &sdata->vif.bss_conf;
<<<<<<< HEAD
	u32 changed = 0;
	int err;
>>>>>>> refs/remotes/origin/cm-10.0
	u16 ap_ht_cap_flags;
=======
	const struct cfg80211_bss_ies *bss_ies = NULL;
	struct ieee80211_mgd_assoc_data *assoc_data = ifmgd->assoc_data;
	u32 changed = 0;
	int err;
	bool ret;
>>>>>>> refs/remotes/origin/master

	/* AssocResp and ReassocResp have identical structure */

	aid = le16_to_cpu(mgmt->u.assoc_resp.aid);
	capab_info = le16_to_cpu(mgmt->u.assoc_resp.capab_info);

	if ((aid & (BIT(15) | BIT(14))) != (BIT(15) | BIT(14)))
<<<<<<< HEAD
		printk(KERN_DEBUG
		       "%s: invalid AID value 0x%x; bits 15:14 not set\n",
		       sdata->name, aid);
=======
		sdata_info(sdata, "invalid AID value 0x%x; bits 15:14 not set\n",
			   aid);
>>>>>>> refs/remotes/origin/master
	aid &= ~(BIT(15) | BIT(14));

	ifmgd->broken_ap = false;

	if (aid == 0 || aid > IEEE80211_MAX_AID) {
<<<<<<< HEAD
		printk(KERN_DEBUG
		       "%s: invalid AID value %d (out of range), turn off PS\n",
		       sdata->name, aid);
=======
		sdata_info(sdata, "invalid AID value %d (out of range), turn off PS\n",
			   aid);
>>>>>>> refs/remotes/origin/master
		aid = 0;
		ifmgd->broken_ap = true;
	}

	pos = mgmt->u.assoc_resp.variable;
<<<<<<< HEAD
	ieee802_11_parse_elems(pos, len - (pos - (u8 *) mgmt), &elems);

	if (!elems.supp_rates) {
		printk(KERN_DEBUG "%s: no SuppRates element in AssocResp\n",
		       sdata->name);
=======
	ieee802_11_parse_elems(pos, len - (pos - (u8 *) mgmt), false, &elems);

	if (!elems.supp_rates) {
		sdata_info(sdata, "no SuppRates element in AssocResp\n");
>>>>>>> refs/remotes/origin/master
		return false;
	}

	ifmgd->aid = aid;

<<<<<<< HEAD
<<<<<<< HEAD
	sta = sta_info_alloc(sdata, cbss->bssid, GFP_KERNEL);
	if (!sta) {
		printk(KERN_DEBUG "%s: failed to alloc STA entry for"
		       " the AP\n", sdata->name);
		return false;
	}

	set_sta_flags(sta, WLAN_STA_AUTH | WLAN_STA_ASSOC |
			   WLAN_STA_ASSOC_AP);
	if (!(ifmgd->flags & IEEE80211_STA_CONTROL_PORT))
		set_sta_flags(sta, WLAN_STA_AUTHORIZED);

	rates = 0;
	basic_rates = 0;
	sband = local->hw.wiphy->bands[wk->chan->band];

	for (i = 0; i < elems.supp_rates_len; i++) {
		int rate = (elems.supp_rates[i] & 0x7f) * 5;
		bool is_basic = !!(elems.supp_rates[i] & 0x80);

		if (rate > 110)
			have_higher_than_11mbit = true;

		for (j = 0; j < sband->n_bitrates; j++) {
			if (sband->bitrates[j].bitrate == rate) {
				rates |= BIT(j);
				if (is_basic)
					basic_rates |= BIT(j);
				break;
			}
		}
	}

	for (i = 0; i < elems.ext_supp_rates_len; i++) {
		int rate = (elems.ext_supp_rates[i] & 0x7f) * 5;
		bool is_basic = !!(elems.ext_supp_rates[i] & 0x80);

		if (rate > 110)
			have_higher_than_11mbit = true;

		for (j = 0; j < sband->n_bitrates; j++) {
			if (sband->bitrates[j].bitrate == rate) {
				rates |= BIT(j);
				if (is_basic)
					basic_rates |= BIT(j);
				break;
			}
		}
	}

	sta->sta.supp_rates[wk->chan->band] = rates;
	sdata->vif.bss_conf.basic_rates = basic_rates;

	/* cf. IEEE 802.11 9.2.12 */
	if (wk->chan->band == IEEE80211_BAND_2GHZ &&
	    have_higher_than_11mbit)
		sdata->flags |= IEEE80211_SDATA_OPERATING_GMODE;
	else
		sdata->flags &= ~IEEE80211_SDATA_OPERATING_GMODE;

	if (elems.ht_cap_elem && !(ifmgd->flags & IEEE80211_STA_DISABLE_11N))
		ieee80211_ht_cap_ie_to_sta_ht_cap(sband,
=======
=======
	/*
	 * Some APs are erroneously not including some information in their
	 * (re)association response frames. Try to recover by using the data
	 * from the beacon or probe response. This seems to afflict mobile
	 * 2G/3G/4G wifi routers, reported models include the "Onda PN51T",
	 * "Vodafone PocketWiFi 2", "ZTE MF60" and a similar T-Mobile device.
	 */
	if ((assoc_data->wmm && !elems.wmm_param) ||
	    (!(ifmgd->flags & IEEE80211_STA_DISABLE_HT) &&
	     (!elems.ht_cap_elem || !elems.ht_operation)) ||
	    (!(ifmgd->flags & IEEE80211_STA_DISABLE_VHT) &&
	     (!elems.vht_cap_elem || !elems.vht_operation))) {
		const struct cfg80211_bss_ies *ies;
		struct ieee802_11_elems bss_elems;

		rcu_read_lock();
		ies = rcu_dereference(cbss->ies);
		if (ies)
			bss_ies = kmemdup(ies, sizeof(*ies) + ies->len,
					  GFP_ATOMIC);
		rcu_read_unlock();
		if (!bss_ies)
			return false;

		ieee802_11_parse_elems(bss_ies->data, bss_ies->len,
				       false, &bss_elems);
		if (assoc_data->wmm &&
		    !elems.wmm_param && bss_elems.wmm_param) {
			elems.wmm_param = bss_elems.wmm_param;
			sdata_info(sdata,
				   "AP bug: WMM param missing from AssocResp\n");
		}

		/*
		 * Also check if we requested HT/VHT, otherwise the AP doesn't
		 * have to include the IEs in the (re)association response.
		 */
		if (!elems.ht_cap_elem && bss_elems.ht_cap_elem &&
		    !(ifmgd->flags & IEEE80211_STA_DISABLE_HT)) {
			elems.ht_cap_elem = bss_elems.ht_cap_elem;
			sdata_info(sdata,
				   "AP bug: HT capability missing from AssocResp\n");
		}
		if (!elems.ht_operation && bss_elems.ht_operation &&
		    !(ifmgd->flags & IEEE80211_STA_DISABLE_HT)) {
			elems.ht_operation = bss_elems.ht_operation;
			sdata_info(sdata,
				   "AP bug: HT operation missing from AssocResp\n");
		}
		if (!elems.vht_cap_elem && bss_elems.vht_cap_elem &&
		    !(ifmgd->flags & IEEE80211_STA_DISABLE_VHT)) {
			elems.vht_cap_elem = bss_elems.vht_cap_elem;
			sdata_info(sdata,
				   "AP bug: VHT capa missing from AssocResp\n");
		}
		if (!elems.vht_operation && bss_elems.vht_operation &&
		    !(ifmgd->flags & IEEE80211_STA_DISABLE_VHT)) {
			elems.vht_operation = bss_elems.vht_operation;
			sdata_info(sdata,
				   "AP bug: VHT operation missing from AssocResp\n");
		}
	}

	/*
	 * We previously checked these in the beacon/probe response, so
	 * they should be present here. This is just a safety net.
	 */
	if (!(ifmgd->flags & IEEE80211_STA_DISABLE_HT) &&
	    (!elems.wmm_param || !elems.ht_cap_elem || !elems.ht_operation)) {
		sdata_info(sdata,
			   "HT AP is missing WMM params or HT capability/operation\n");
		ret = false;
		goto out;
	}

	if (!(ifmgd->flags & IEEE80211_STA_DISABLE_VHT) &&
	    (!elems.vht_cap_elem || !elems.vht_operation)) {
		sdata_info(sdata,
			   "VHT AP is missing VHT capability/operation\n");
		ret = false;
		goto out;
	}

>>>>>>> refs/remotes/origin/master
	mutex_lock(&sdata->local->sta_mtx);
	/*
	 * station info was already allocated and inserted before
	 * the association and should be available to us
	 */
	sta = sta_info_get(sdata, cbss->bssid);
	if (WARN_ON(!sta)) {
		mutex_unlock(&sdata->local->sta_mtx);
<<<<<<< HEAD
		return false;
	}

	sband = local->hw.wiphy->bands[local->oper_channel->band];

	if (elems.ht_cap_elem && !(ifmgd->flags & IEEE80211_STA_DISABLE_11N))
		ieee80211_ht_cap_ie_to_sta_ht_cap(sdata, sband,
>>>>>>> refs/remotes/origin/cm-10.0
				elems.ht_cap_elem, &sta->sta.ht_cap);

	ap_ht_cap_flags = sta->sta.ht_cap.cap;

	rate_control_rate_init(sta);

	if (ifmgd->flags & IEEE80211_STA_MFP_ENABLED)
<<<<<<< HEAD
		set_sta_flags(sta, WLAN_STA_MFP);

	if (elems.wmm_param)
		set_sta_flags(sta, WLAN_STA_WME);

	err = sta_info_insert(sta);
	sta = NULL;
	if (err) {
		printk(KERN_DEBUG "%s: failed to insert STA entry for"
		       " the AP (error %d)\n", sdata->name, err);
		return false;
	}

=======
=======
		ret = false;
		goto out;
	}

	sband = local->hw.wiphy->bands[ieee80211_get_sdata_band(sdata)];

	/* Set up internal HT/VHT capabilities */
	if (elems.ht_cap_elem && !(ifmgd->flags & IEEE80211_STA_DISABLE_HT))
		ieee80211_ht_cap_ie_to_sta_ht_cap(sdata, sband,
						  elems.ht_cap_elem, sta);

	if (elems.vht_cap_elem && !(ifmgd->flags & IEEE80211_STA_DISABLE_VHT))
		ieee80211_vht_cap_ie_to_sta_vht_cap(sdata, sband,
						    elems.vht_cap_elem, sta);

	/*
	 * Some APs, e.g. Netgear WNDR3700, report invalid HT operation data
	 * in their association response, so ignore that data for our own
	 * configuration. If it changed since the last beacon, we'll get the
	 * next beacon and update then.
	 */

	/*
	 * If an operating mode notification IE is present, override the
	 * NSS calculation (that would be done in rate_control_rate_init())
	 * and use the # of streams from that element.
	 */
	if (elems.opmode_notif &&
	    !(*elems.opmode_notif & IEEE80211_OPMODE_NOTIF_RX_NSS_TYPE_BF)) {
		u8 nss;

		nss = *elems.opmode_notif & IEEE80211_OPMODE_NOTIF_RX_NSS_MASK;
		nss >>= IEEE80211_OPMODE_NOTIF_RX_NSS_SHIFT;
		nss += 1;
		sta->sta.rx_nss = nss;
	}

	rate_control_rate_init(sta);

	if (ifmgd->flags & IEEE80211_STA_MFP_ENABLED)
>>>>>>> refs/remotes/origin/master
		set_sta_flag(sta, WLAN_STA_MFP);

	if (elems.wmm_param)
		set_sta_flag(sta, WLAN_STA_WME);

<<<<<<< HEAD
	err = sta_info_move_state(sta, IEEE80211_STA_AUTH);
	if (!err)
		err = sta_info_move_state(sta, IEEE80211_STA_ASSOC);
	if (!err && !(ifmgd->flags & IEEE80211_STA_CONTROL_PORT))
		err = sta_info_move_state(sta, IEEE80211_STA_AUTHORIZED);
	if (err) {
		printk(KERN_DEBUG
		       "%s: failed to move station %pM to desired state\n",
		       sdata->name, sta->sta.addr);
		WARN_ON(__sta_info_destroy(sta));
		mutex_unlock(&sdata->local->sta_mtx);
		return false;
=======
	err = sta_info_move_state(sta, IEEE80211_STA_ASSOC);
	if (!err && !(ifmgd->flags & IEEE80211_STA_CONTROL_PORT))
		err = sta_info_move_state(sta, IEEE80211_STA_AUTHORIZED);
	if (err) {
		sdata_info(sdata,
			   "failed to move station %pM to desired state\n",
			   sta->sta.addr);
		WARN_ON(__sta_info_destroy(sta));
		mutex_unlock(&sdata->local->sta_mtx);
		ret = false;
		goto out;
>>>>>>> refs/remotes/origin/master
	}

	mutex_unlock(&sdata->local->sta_mtx);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * Always handle WMM once after association regardless
	 * of the first value the AP uses. Setting -1 here has
	 * that effect because the AP values is an unsigned
	 * 4-bit value.
	 */
	ifmgd->wmm_last_param_set = -1;

<<<<<<< HEAD
	if (elems.wmm_param)
		ieee80211_sta_wmm_params(local, sdata, elems.wmm_param,
					 elems.wmm_param_len);
	else
<<<<<<< HEAD
		ieee80211_set_wmm_default(sdata);

	local->oper_channel = wk->chan;

	if (elems.ht_info_elem && elems.wmm_param &&
	    (sdata->local->hw.queues >= 4) &&
	    !(ifmgd->flags & IEEE80211_STA_DISABLE_11N))
		changed |= ieee80211_enable_ht(sdata, elems.ht_info_elem,
					       cbss->bssid, ap_ht_cap_flags);
=======
		ieee80211_set_wmm_default(sdata, false);
	changed |= BSS_CHANGED_QOS;

	if (elems.ht_info_elem && elems.wmm_param &&
	    !(ifmgd->flags & IEEE80211_STA_DISABLE_11N))
		changed |= ieee80211_enable_ht(sdata, elems.ht_info_elem,
					       cbss->bssid, ap_ht_cap_flags,
					       false);
>>>>>>> refs/remotes/origin/cm-10.0

=======
	if (!(ifmgd->flags & IEEE80211_STA_DISABLE_WMM) && elems.wmm_param)
		ieee80211_sta_wmm_params(local, sdata, elems.wmm_param,
					 elems.wmm_param_len);
	else
		ieee80211_set_wmm_default(sdata, false);
	changed |= BSS_CHANGED_QOS;

>>>>>>> refs/remotes/origin/master
	/* set AID and assoc capability,
	 * ieee80211_set_associated() will tell the driver */
	bss_conf->aid = aid;
	bss_conf->assoc_capability = capab_info;
	ieee80211_set_associated(sdata, cbss, changed);

	/*
	 * If we're using 4-addr mode, let the AP know that we're
	 * doing so, so that it can create the STA VLAN on its side
	 */
	if (ifmgd->use_4addr)
		ieee80211_send_4addr_nullfunc(local, sdata);

	/*
	 * Start timer to probe the connection to the AP now.
	 * Also start the timer that will detect beacon loss.
	 */
	ieee80211_sta_rx_notify(sdata, (struct ieee80211_hdr *)mgmt);
	ieee80211_sta_reset_beacon_monitor(sdata);

<<<<<<< HEAD
	return true;
}

<<<<<<< HEAD

=======
static enum rx_mgmt_action __must_check
ieee80211_rx_mgmt_assoc_resp(struct ieee80211_sub_if_data *sdata,
			     struct ieee80211_mgmt *mgmt, size_t len,
			     struct cfg80211_bss **bss)
=======
	ret = true;
 out:
	kfree(bss_ies);
	return ret;
}

static void ieee80211_rx_mgmt_assoc_resp(struct ieee80211_sub_if_data *sdata,
					 struct ieee80211_mgmt *mgmt,
					 size_t len)
>>>>>>> refs/remotes/origin/master
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct ieee80211_mgd_assoc_data *assoc_data = ifmgd->assoc_data;
	u16 capab_info, status_code, aid;
	struct ieee802_11_elems elems;
	u8 *pos;
	bool reassoc;
<<<<<<< HEAD

	lockdep_assert_held(&ifmgd->mtx);

	if (!assoc_data)
		return RX_MGMT_NONE;
	if (compare_ether_addr(assoc_data->bss->bssid, mgmt->bssid))
		return RX_MGMT_NONE;
=======
	struct cfg80211_bss *bss;

	sdata_assert_lock(sdata);

	if (!assoc_data)
		return;
	if (!ether_addr_equal(assoc_data->bss->bssid, mgmt->bssid))
		return;
>>>>>>> refs/remotes/origin/master

	/*
	 * AssocResp and ReassocResp have identical structure, so process both
	 * of them in this function.
	 */

	if (len < 24 + 6)
<<<<<<< HEAD
		return RX_MGMT_NONE;
=======
		return;
>>>>>>> refs/remotes/origin/master

	reassoc = ieee80211_is_reassoc_req(mgmt->frame_control);
	capab_info = le16_to_cpu(mgmt->u.assoc_resp.capab_info);
	status_code = le16_to_cpu(mgmt->u.assoc_resp.status_code);
	aid = le16_to_cpu(mgmt->u.assoc_resp.aid);

<<<<<<< HEAD
	printk(KERN_DEBUG "%s: RX %sssocResp from %pM (capab=0x%x "
	       "status=%d aid=%d)\n",
	       sdata->name, reassoc ? "Rea" : "A", mgmt->sa,
	       capab_info, status_code, (u16)(aid & ~(BIT(15) | BIT(14))));

	pos = mgmt->u.assoc_resp.variable;
	ieee802_11_parse_elems(pos, len - (pos - (u8 *) mgmt), &elems);

	if (status_code == WLAN_STATUS_ASSOC_REJECTED_TEMPORARILY &&
	    elems.timeout_int && elems.timeout_int_len == 5 &&
	    elems.timeout_int[0] == WLAN_TIMEOUT_ASSOC_COMEBACK) {
		u32 tu, ms;
		tu = get_unaligned_le32(elems.timeout_int + 1);
		ms = tu * 1024 / 1000;
		printk(KERN_DEBUG "%s: %pM rejected association temporarily; "
		       "comeback duration %u TU (%u ms)\n",
		       sdata->name, mgmt->sa, tu, ms);
		assoc_data->timeout = jiffies + msecs_to_jiffies(ms);
		if (ms > IEEE80211_ASSOC_TIMEOUT)
			run_again(ifmgd, assoc_data->timeout);
		return RX_MGMT_NONE;
	}

	*bss = assoc_data->bss;

	if (status_code != WLAN_STATUS_SUCCESS) {
		printk(KERN_DEBUG "%s: %pM denied association (code=%d)\n",
		       sdata->name, mgmt->sa, status_code);
		ieee80211_destroy_assoc_data(sdata, false);
	} else {
		if (!ieee80211_assoc_success(sdata, *bss, mgmt, len)) {
			/* oops -- internal error -- send timeout for now */
			ieee80211_destroy_assoc_data(sdata, false);
			cfg80211_put_bss(*bss);
			return RX_MGMT_CFG80211_ASSOC_TIMEOUT;
		}
		printk(KERN_DEBUG "%s: associated\n", sdata->name);
=======
	sdata_info(sdata,
		   "RX %sssocResp from %pM (capab=0x%x status=%d aid=%d)\n",
		   reassoc ? "Rea" : "A", mgmt->sa,
		   capab_info, status_code, (u16)(aid & ~(BIT(15) | BIT(14))));

	pos = mgmt->u.assoc_resp.variable;
	ieee802_11_parse_elems(pos, len - (pos - (u8 *) mgmt), false, &elems);

	if (status_code == WLAN_STATUS_ASSOC_REJECTED_TEMPORARILY &&
	    elems.timeout_int &&
	    elems.timeout_int->type == WLAN_TIMEOUT_ASSOC_COMEBACK) {
		u32 tu, ms;
		tu = le32_to_cpu(elems.timeout_int->value);
		ms = tu * 1024 / 1000;
		sdata_info(sdata,
			   "%pM rejected association temporarily; comeback duration %u TU (%u ms)\n",
			   mgmt->sa, tu, ms);
		assoc_data->timeout = jiffies + msecs_to_jiffies(ms);
		assoc_data->timeout_started = true;
		if (ms > IEEE80211_ASSOC_TIMEOUT)
			run_again(sdata, assoc_data->timeout);
		return;
	}

	bss = assoc_data->bss;

	if (status_code != WLAN_STATUS_SUCCESS) {
		sdata_info(sdata, "%pM denied association (code=%d)\n",
			   mgmt->sa, status_code);
		ieee80211_destroy_assoc_data(sdata, false);
	} else {
		if (!ieee80211_assoc_success(sdata, bss, mgmt, len)) {
			/* oops -- internal error -- send timeout for now */
			ieee80211_destroy_assoc_data(sdata, false);
			cfg80211_assoc_timeout(sdata->dev, bss);
			return;
		}
		sdata_info(sdata, "associated\n");
>>>>>>> refs/remotes/origin/master

		/*
		 * destroy assoc_data afterwards, as otherwise an idle
		 * recalc after assoc_data is NULL but before associated
		 * is set can cause the interface to go idle
		 */
		ieee80211_destroy_assoc_data(sdata, true);
	}

<<<<<<< HEAD
	return RX_MGMT_CFG80211_RX_ASSOC;
}
>>>>>>> refs/remotes/origin/cm-10.0
static void ieee80211_rx_bss_info(struct ieee80211_sub_if_data *sdata,
				  struct ieee80211_mgmt *mgmt,
				  size_t len,
				  struct ieee80211_rx_status *rx_status,
				  struct ieee802_11_elems *elems,
				  bool beacon)
=======
	cfg80211_rx_assoc_resp(sdata->dev, bss, (u8 *)mgmt, len);
}

static void ieee80211_rx_bss_info(struct ieee80211_sub_if_data *sdata,
				  struct ieee80211_mgmt *mgmt, size_t len,
				  struct ieee80211_rx_status *rx_status,
				  struct ieee802_11_elems *elems)
>>>>>>> refs/remotes/origin/master
{
	struct ieee80211_local *local = sdata->local;
	int freq;
	struct ieee80211_bss *bss;
	struct ieee80211_channel *channel;
<<<<<<< HEAD
	bool need_ps = false;

<<<<<<< HEAD
	if (sdata->u.mgd.associated) {
=======
	if (sdata->u.mgd.associated &&
	    compare_ether_addr(mgmt->bssid, sdata->u.mgd.associated->bssid)
	    == 0) {
>>>>>>> refs/remotes/origin/cm-10.0
		bss = (void *)sdata->u.mgd.associated->priv;
		/* not previously set so we may need to recalc */
		need_ps = !bss->dtim_period;
	}

	if (elems->ds_params && elems->ds_params_len == 1)
=======

	sdata_assert_lock(sdata);

	if (elems->ds_params)
>>>>>>> refs/remotes/origin/master
		freq = ieee80211_channel_to_frequency(elems->ds_params[0],
						      rx_status->band);
	else
		freq = rx_status->freq;

	channel = ieee80211_get_channel(local->hw.wiphy, freq);

	if (!channel || channel->flags & IEEE80211_CHAN_DISABLED)
		return;

	bss = ieee80211_bss_info_update(local, rx_status, mgmt, len, elems,
<<<<<<< HEAD
					channel, beacon);
	if (bss)
		ieee80211_rx_bss_put(local, bss);

	if (!sdata->u.mgd.associated)
		return;

	if (need_ps) {
		mutex_lock(&local->iflist_mtx);
		ieee80211_recalc_ps(local, -1);
		mutex_unlock(&local->iflist_mtx);
	}

	if (elems->ch_switch_elem && (elems->ch_switch_elem_len == 3) &&
	    (memcmp(mgmt->bssid, sdata->u.mgd.associated->bssid,
							ETH_ALEN) == 0)) {
		struct ieee80211_channel_sw_ie *sw_elem =
			(struct ieee80211_channel_sw_ie *)elems->ch_switch_elem;
		ieee80211_sta_process_chanswitch(sdata, sw_elem,
						 bss, rx_status->mactime);
=======
					channel);
	if (bss) {
		ieee80211_rx_bss_put(local, bss);
		sdata->vif.bss_conf.beacon_rate = bss->beacon_rate;
>>>>>>> refs/remotes/origin/master
	}
}


static void ieee80211_rx_mgmt_probe_resp(struct ieee80211_sub_if_data *sdata,
					 struct sk_buff *skb)
{
	struct ieee80211_mgmt *mgmt = (void *)skb->data;
	struct ieee80211_if_managed *ifmgd;
	struct ieee80211_rx_status *rx_status = (void *) skb->cb;
	size_t baselen, len = skb->len;
	struct ieee802_11_elems elems;

	ifmgd = &sdata->u.mgd;

<<<<<<< HEAD
	ASSERT_MGD_MTX(ifmgd);

<<<<<<< HEAD
	if (memcmp(mgmt->da, sdata->vif.addr, ETH_ALEN))
=======
	if (compare_ether_addr(mgmt->da, sdata->vif.addr))
>>>>>>> refs/remotes/origin/cm-10.0
=======
	sdata_assert_lock(sdata);

	if (!ether_addr_equal(mgmt->da, sdata->vif.addr))
>>>>>>> refs/remotes/origin/master
		return; /* ignore ProbeResp to foreign address */

	baselen = (u8 *) mgmt->u.probe_resp.variable - (u8 *) mgmt;
	if (baselen > len)
		return;

	ieee802_11_parse_elems(mgmt->u.probe_resp.variable, len - baselen,
<<<<<<< HEAD
				&elems);

	ieee80211_rx_bss_info(sdata, mgmt, len, rx_status, &elems, false);

	if (ifmgd->associated &&
<<<<<<< HEAD
	    memcmp(mgmt->bssid, ifmgd->associated->bssid, ETH_ALEN) == 0)
		ieee80211_reset_ap_probe(sdata);
=======
	    compare_ether_addr(mgmt->bssid, ifmgd->associated->bssid) == 0)
		ieee80211_reset_ap_probe(sdata);

	if (ifmgd->auth_data && !ifmgd->auth_data->bss->proberesp_ies &&
	    compare_ether_addr(mgmt->bssid, ifmgd->auth_data->bss->bssid)
	    == 0) {
		/* got probe response, continue with auth */
		printk(KERN_DEBUG "%s: direct probe responded\n", sdata->name);
		ifmgd->auth_data->tries = 0;
		ifmgd->auth_data->timeout = jiffies;
		run_again(ifmgd, ifmgd->auth_data->timeout);
	}
>>>>>>> refs/remotes/origin/cm-10.0
=======
			       false, &elems);

	ieee80211_rx_bss_info(sdata, mgmt, len, rx_status, &elems);

	if (ifmgd->associated &&
	    ether_addr_equal(mgmt->bssid, ifmgd->associated->bssid))
		ieee80211_reset_ap_probe(sdata);

	if (ifmgd->auth_data && !ifmgd->auth_data->bss->proberesp_ies &&
	    ether_addr_equal(mgmt->bssid, ifmgd->auth_data->bss->bssid)) {
		/* got probe response, continue with auth */
		sdata_info(sdata, "direct probe responded\n");
		ifmgd->auth_data->tries = 0;
		ifmgd->auth_data->timeout = jiffies;
		ifmgd->auth_data->timeout_started = true;
		run_again(sdata, ifmgd->auth_data->timeout);
	}
>>>>>>> refs/remotes/origin/master
}

/*
 * This is the canonical list of information elements we care about,
 * the filter code also gives us all changes to the Microsoft OUI
 * (00:50:F2) vendor IE which is used for WMM which we need to track.
 *
 * We implement beacon filtering in software since that means we can
 * avoid processing the frame here and in cfg80211, and userspace
 * will not be able to tell whether the hardware supports it or not.
 *
 * XXX: This list needs to be dynamic -- userspace needs to be able to
 *	add items it requires. It also needs to be able to tell us to
 *	look out for other vendor IEs.
 */
static const u64 care_about_ies =
	(1ULL << WLAN_EID_COUNTRY) |
	(1ULL << WLAN_EID_ERP_INFO) |
	(1ULL << WLAN_EID_CHANNEL_SWITCH) |
	(1ULL << WLAN_EID_PWR_CONSTRAINT) |
	(1ULL << WLAN_EID_HT_CAPABILITY) |
<<<<<<< HEAD
	(1ULL << WLAN_EID_HT_INFORMATION);

static void ieee80211_rx_mgmt_beacon(struct ieee80211_sub_if_data *sdata,
				     struct ieee80211_mgmt *mgmt,
				     size_t len,
=======
	(1ULL << WLAN_EID_HT_OPERATION);

static void ieee80211_rx_mgmt_beacon(struct ieee80211_sub_if_data *sdata,
				     struct ieee80211_mgmt *mgmt, size_t len,
>>>>>>> refs/remotes/origin/master
				     struct ieee80211_rx_status *rx_status)
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct ieee80211_bss_conf *bss_conf = &sdata->vif.bss_conf;
	size_t baselen;
	struct ieee802_11_elems elems;
	struct ieee80211_local *local = sdata->local;
<<<<<<< HEAD
	u32 changed = 0;
	bool erp_valid, directed_tim = false;
	u8 erp_value = 0;
	u32 ncrc;
	u8 *bssid;

<<<<<<< HEAD
	ASSERT_MGD_MTX(ifmgd);
=======
	lockdep_assert_held(&ifmgd->mtx);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct ieee80211_chanctx_conf *chanctx_conf;
	struct ieee80211_channel *chan;
	struct sta_info *sta;
	u32 changed = 0;
	bool erp_valid;
	u8 erp_value = 0;
	u32 ncrc;
	u8 *bssid;
	u8 deauth_buf[IEEE80211_DEAUTH_FRAME_LEN];

	sdata_assert_lock(sdata);
>>>>>>> refs/remotes/origin/master

	/* Process beacon from the current BSS */
	baselen = (u8 *) mgmt->u.beacon.variable - (u8 *) mgmt;
	if (baselen > len)
		return;

<<<<<<< HEAD
	if (rx_status->freq != local->hw.conf.channel->center_freq)
		return;

<<<<<<< HEAD
	/*
	 * We might have received a number of frames, among them a
	 * disassoc frame and a beacon...
	 */
	if (!ifmgd->associated)
		return;

	bssid = ifmgd->associated->bssid;

	/*
	 * And in theory even frames from a different AP we were just
	 * associated to a split-second ago!
	 */
	if (memcmp(bssid, mgmt->bssid, ETH_ALEN) != 0)
		return;
=======
	if (ifmgd->assoc_data && !ifmgd->assoc_data->have_beacon &&
	    compare_ether_addr(mgmt->bssid, ifmgd->assoc_data->bss->bssid)
	    == 0) {
		ieee802_11_parse_elems(mgmt->u.beacon.variable,
				       len - baselen, &elems);

		ieee80211_rx_bss_info(sdata, mgmt, len, rx_status, &elems,
				      false);
		ifmgd->assoc_data->have_beacon = true;
		ifmgd->assoc_data->sent_assoc = false;
		/* continue assoc process */
		ifmgd->assoc_data->timeout = jiffies;
		run_again(ifmgd, ifmgd->assoc_data->timeout);
=======
	rcu_read_lock();
	chanctx_conf = rcu_dereference(sdata->vif.chanctx_conf);
	if (!chanctx_conf) {
		rcu_read_unlock();
		return;
	}

	if (rx_status->freq != chanctx_conf->def.chan->center_freq) {
		rcu_read_unlock();
		return;
	}
	chan = chanctx_conf->def.chan;
	rcu_read_unlock();

	if (ifmgd->assoc_data && ifmgd->assoc_data->need_beacon &&
	    ether_addr_equal(mgmt->bssid, ifmgd->assoc_data->bss->bssid)) {
		ieee802_11_parse_elems(mgmt->u.beacon.variable,
				       len - baselen, false, &elems);

		ieee80211_rx_bss_info(sdata, mgmt, len, rx_status, &elems);
		if (elems.tim && !elems.parse_error) {
			const struct ieee80211_tim_ie *tim_ie = elems.tim;
			ifmgd->dtim_period = tim_ie->dtim_period;
		}
		ifmgd->have_beacon = true;
		ifmgd->assoc_data->need_beacon = false;
		if (local->hw.flags & IEEE80211_HW_TIMING_BEACON_ONLY) {
			sdata->vif.bss_conf.sync_tsf =
				le64_to_cpu(mgmt->u.beacon.timestamp);
			sdata->vif.bss_conf.sync_device_ts =
				rx_status->device_timestamp;
			if (elems.tim)
				sdata->vif.bss_conf.sync_dtim_count =
					elems.tim->dtim_count;
			else
				sdata->vif.bss_conf.sync_dtim_count = 0;
		}
		/* continue assoc process */
		ifmgd->assoc_data->timeout = jiffies;
		ifmgd->assoc_data->timeout_started = true;
		run_again(sdata, ifmgd->assoc_data->timeout);
>>>>>>> refs/remotes/origin/master
		return;
	}

	if (!ifmgd->associated ||
<<<<<<< HEAD
	    compare_ether_addr(mgmt->bssid, ifmgd->associated->bssid))
		return;
	bssid = ifmgd->associated->bssid;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	    !ether_addr_equal(mgmt->bssid, ifmgd->associated->bssid))
		return;
	bssid = ifmgd->associated->bssid;
>>>>>>> refs/remotes/origin/master

	/* Track average RSSI from the Beacon frames of the current AP */
	ifmgd->last_beacon_signal = rx_status->signal;
	if (ifmgd->flags & IEEE80211_STA_RESET_SIGNAL_AVE) {
		ifmgd->flags &= ~IEEE80211_STA_RESET_SIGNAL_AVE;
		ifmgd->ave_beacon_signal = rx_status->signal * 16;
		ifmgd->last_cqm_event_signal = 0;
		ifmgd->count_beacon_signal = 1;
<<<<<<< HEAD
<<<<<<< HEAD
=======
		ifmgd->last_ave_beacon_signal = 0;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		ifmgd->last_ave_beacon_signal = 0;
>>>>>>> refs/remotes/origin/master
	} else {
		ifmgd->ave_beacon_signal =
			(IEEE80211_SIGNAL_AVE_WEIGHT * rx_status->signal * 16 +
			 (16 - IEEE80211_SIGNAL_AVE_WEIGHT) *
			 ifmgd->ave_beacon_signal) / 16;
		ifmgd->count_beacon_signal++;
	}
<<<<<<< HEAD
<<<<<<< HEAD
	if (bss_conf->cqm_rssi_thold &&
	    ifmgd->count_beacon_signal >= IEEE80211_SIGNAL_AVE_MIN_COUNT &&
	    !(local->hw.flags & IEEE80211_HW_SUPPORTS_CQM_RSSI)) {
=======
=======
>>>>>>> refs/remotes/origin/master

	if (ifmgd->rssi_min_thold != ifmgd->rssi_max_thold &&
	    ifmgd->count_beacon_signal >= IEEE80211_SIGNAL_AVE_MIN_COUNT) {
		int sig = ifmgd->ave_beacon_signal;
		int last_sig = ifmgd->last_ave_beacon_signal;

		/*
		 * if signal crosses either of the boundaries, invoke callback
		 * with appropriate parameters
		 */
		if (sig > ifmgd->rssi_max_thold &&
		    (last_sig <= ifmgd->rssi_min_thold || last_sig == 0)) {
			ifmgd->last_ave_beacon_signal = sig;
<<<<<<< HEAD
			drv_rssi_callback(local, RSSI_EVENT_HIGH);
=======
			drv_rssi_callback(local, sdata, RSSI_EVENT_HIGH);
>>>>>>> refs/remotes/origin/master
		} else if (sig < ifmgd->rssi_min_thold &&
			   (last_sig >= ifmgd->rssi_max_thold ||
			   last_sig == 0)) {
			ifmgd->last_ave_beacon_signal = sig;
<<<<<<< HEAD
			drv_rssi_callback(local, RSSI_EVENT_LOW);
=======
			drv_rssi_callback(local, sdata, RSSI_EVENT_LOW);
>>>>>>> refs/remotes/origin/master
		}
	}

	if (bss_conf->cqm_rssi_thold &&
	    ifmgd->count_beacon_signal >= IEEE80211_SIGNAL_AVE_MIN_COUNT &&
	    !(sdata->vif.driver_flags & IEEE80211_VIF_SUPPORTS_CQM_RSSI)) {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		int sig = ifmgd->ave_beacon_signal / 16;
		int last_event = ifmgd->last_cqm_event_signal;
		int thold = bss_conf->cqm_rssi_thold;
		int hyst = bss_conf->cqm_rssi_hyst;
		if (sig < thold &&
		    (last_event == 0 || sig < last_event - hyst)) {
			ifmgd->last_cqm_event_signal = sig;
			ieee80211_cqm_rssi_notify(
				&sdata->vif,
				NL80211_CQM_RSSI_THRESHOLD_EVENT_LOW,
				GFP_KERNEL);
		} else if (sig > thold &&
			   (last_event == 0 || sig > last_event + hyst)) {
			ifmgd->last_cqm_event_signal = sig;
			ieee80211_cqm_rssi_notify(
				&sdata->vif,
				NL80211_CQM_RSSI_THRESHOLD_EVENT_HIGH,
				GFP_KERNEL);
		}
	}

<<<<<<< HEAD
	if (ifmgd->flags & IEEE80211_STA_BEACON_POLL) {
#ifdef CONFIG_MAC80211_VERBOSE_DEBUG
		if (net_ratelimit()) {
			printk(KERN_DEBUG "%s: cancelling probereq poll due "
			       "to a received beacon\n", sdata->name);
		}
#endif
		ifmgd->flags &= ~IEEE80211_STA_BEACON_POLL;
		mutex_lock(&local->iflist_mtx);
		ieee80211_recalc_ps(local, -1);
		mutex_unlock(&local->iflist_mtx);
=======
	if (ifmgd->flags & IEEE80211_STA_CONNECTION_POLL) {
		mlme_dbg_ratelimited(sdata,
				     "cancelling AP probe due to a received beacon\n");
		ieee80211_reset_ap_probe(sdata);
>>>>>>> refs/remotes/origin/master
	}

	/*
	 * Push the beacon loss detection into the future since
	 * we are processing a beacon from the AP just now.
	 */
	ieee80211_sta_reset_beacon_monitor(sdata);

	ncrc = crc32_be(0, (void *)&mgmt->u.beacon.beacon_int, 4);
	ncrc = ieee802_11_parse_elems_crc(mgmt->u.beacon.variable,
<<<<<<< HEAD
					  len - baselen, &elems,
					  care_about_ies, ncrc);

	if (local->hw.flags & IEEE80211_HW_PS_NULLFUNC_STACK)
		directed_tim = ieee80211_check_tim(elems.tim, elems.tim_len,
						   ifmgd->aid);

	if (ncrc != ifmgd->beacon_crc || !ifmgd->beacon_crc_valid) {
		ieee80211_rx_bss_info(sdata, mgmt, len, rx_status, &elems,
				      true);

		ieee80211_sta_wmm_params(local, sdata, elems.wmm_param,
					 elems.wmm_param_len);
	}

	if (local->hw.flags & IEEE80211_HW_PS_NULLFUNC_STACK) {
		if (directed_tim) {
			if (local->hw.conf.dynamic_ps_timeout > 0) {
				local->hw.conf.flags &= ~IEEE80211_CONF_PS;
				ieee80211_hw_config(local,
						    IEEE80211_CONF_CHANGE_PS);
				ieee80211_send_nullfunc(local, sdata, 0);
			} else {
=======
					  len - baselen, false, &elems,
					  care_about_ies, ncrc);

	if (local->hw.flags & IEEE80211_HW_PS_NULLFUNC_STACK) {
		bool directed_tim = ieee80211_check_tim(elems.tim,
							elems.tim_len,
							ifmgd->aid);
		if (directed_tim) {
			if (local->hw.conf.dynamic_ps_timeout > 0) {
				if (local->hw.conf.flags & IEEE80211_CONF_PS) {
					local->hw.conf.flags &= ~IEEE80211_CONF_PS;
					ieee80211_hw_config(local,
							    IEEE80211_CONF_CHANGE_PS);
				}
				ieee80211_send_nullfunc(local, sdata, 0);
			} else if (!local->pspolling && sdata->u.mgd.powersave) {
>>>>>>> refs/remotes/origin/master
				local->pspolling = true;

				/*
				 * Here is assumed that the driver will be
				 * able to send ps-poll frame and receive a
				 * response even though power save mode is
				 * enabled, but some drivers might require
				 * to disable power save here. This needs
				 * to be investigated.
				 */
				ieee80211_send_pspoll(local, sdata);
			}
		}
	}

<<<<<<< HEAD
=======
	if (sdata->vif.p2p) {
		struct ieee80211_p2p_noa_attr noa = {};
		int ret;

		ret = cfg80211_get_p2p_attr(mgmt->u.beacon.variable,
					    len - baselen,
					    IEEE80211_P2P_ATTR_ABSENCE_NOTICE,
					    (u8 *) &noa, sizeof(noa));
		if (ret >= 2) {
			if (sdata->u.mgd.p2p_noa_index != noa.index) {
				/* valid noa_attr and index changed */
				sdata->u.mgd.p2p_noa_index = noa.index;
				memcpy(&bss_conf->p2p_noa_attr, &noa, sizeof(noa));
				changed |= BSS_CHANGED_P2P_PS;
				/*
				 * make sure we update all information, the CRC
				 * mechanism doesn't look at P2P attributes.
				 */
				ifmgd->beacon_crc_valid = false;
			}
		} else if (sdata->u.mgd.p2p_noa_index != -1) {
			/* noa_attr not found and we had valid noa_attr before */
			sdata->u.mgd.p2p_noa_index = -1;
			memset(&bss_conf->p2p_noa_attr, 0, sizeof(bss_conf->p2p_noa_attr));
			changed |= BSS_CHANGED_P2P_PS;
			ifmgd->beacon_crc_valid = false;
		}
	}

>>>>>>> refs/remotes/origin/master
	if (ncrc == ifmgd->beacon_crc && ifmgd->beacon_crc_valid)
		return;
	ifmgd->beacon_crc = ncrc;
	ifmgd->beacon_crc_valid = true;

<<<<<<< HEAD
	if (elems.erp_info && elems.erp_info_len >= 1) {
=======
	ieee80211_rx_bss_info(sdata, mgmt, len, rx_status, &elems);

	ieee80211_sta_process_chanswitch(sdata, rx_status->mactime,
					 &elems, true);

	if (!(ifmgd->flags & IEEE80211_STA_DISABLE_WMM) &&
	    ieee80211_sta_wmm_params(local, sdata, elems.wmm_param,
				     elems.wmm_param_len))
		changed |= BSS_CHANGED_QOS;

	/*
	 * If we haven't had a beacon before, tell the driver about the
	 * DTIM period (and beacon timing if desired) now.
	 */
	if (!ifmgd->have_beacon) {
		/* a few bogus AP send dtim_period = 0 or no TIM IE */
		if (elems.tim)
			bss_conf->dtim_period = elems.tim->dtim_period ?: 1;
		else
			bss_conf->dtim_period = 1;

		if (local->hw.flags & IEEE80211_HW_TIMING_BEACON_ONLY) {
			sdata->vif.bss_conf.sync_tsf =
				le64_to_cpu(mgmt->u.beacon.timestamp);
			sdata->vif.bss_conf.sync_device_ts =
				rx_status->device_timestamp;
			if (elems.tim)
				sdata->vif.bss_conf.sync_dtim_count =
					elems.tim->dtim_count;
			else
				sdata->vif.bss_conf.sync_dtim_count = 0;
		}

		changed |= BSS_CHANGED_BEACON_INFO;
		ifmgd->have_beacon = true;

		mutex_lock(&local->iflist_mtx);
		ieee80211_recalc_ps(local, -1);
		mutex_unlock(&local->iflist_mtx);

		ieee80211_recalc_ps_vif(sdata);
	}

	if (elems.erp_info) {
>>>>>>> refs/remotes/origin/master
		erp_valid = true;
		erp_value = elems.erp_info[0];
	} else {
		erp_valid = false;
	}
	changed |= ieee80211_handle_bss_capability(sdata,
			le16_to_cpu(mgmt->u.beacon.capab_info),
			erp_valid, erp_value);

<<<<<<< HEAD

	if (elems.ht_cap_elem && elems.ht_info_elem && elems.wmm_param &&
	    !(ifmgd->flags & IEEE80211_STA_DISABLE_11N)) {
		struct sta_info *sta;
		struct ieee80211_supported_band *sband;
		u16 ap_ht_cap_flags;

		rcu_read_lock();

		sta = sta_info_get(sdata, bssid);
		if (WARN_ON(!sta)) {
			rcu_read_unlock();
			return;
		}

		sband = local->hw.wiphy->bands[local->hw.conf.channel->band];

<<<<<<< HEAD
		ieee80211_ht_cap_ie_to_sta_ht_cap(sband,
=======
		ieee80211_ht_cap_ie_to_sta_ht_cap(sdata, sband,
>>>>>>> refs/remotes/origin/cm-10.0
				elems.ht_cap_elem, &sta->sta.ht_cap);

		ap_ht_cap_flags = sta->sta.ht_cap.cap;

		rcu_read_unlock();

		changed |= ieee80211_enable_ht(sdata, elems.ht_info_elem,
<<<<<<< HEAD
					       bssid, ap_ht_cap_flags);
=======
					       bssid, ap_ht_cap_flags, true);
>>>>>>> refs/remotes/origin/cm-10.0
	}

	/* Note: country IE parsing is done for us by cfg80211 */
	if (elems.country_elem) {
		/* TODO: IBSS also needs this */
		if (elems.pwr_constr_elem)
			ieee80211_handle_pwr_constr(sdata,
				le16_to_cpu(mgmt->u.probe_resp.capab_info),
				elems.pwr_constr_elem,
				elems.pwr_constr_elem_len);
	}
=======
	mutex_lock(&local->sta_mtx);
	sta = sta_info_get(sdata, bssid);

	if (ieee80211_config_bw(sdata, sta, elems.ht_operation,
				elems.vht_operation, bssid, &changed)) {
		mutex_unlock(&local->sta_mtx);
		ieee80211_set_disassoc(sdata, IEEE80211_STYPE_DEAUTH,
				       WLAN_REASON_DEAUTH_LEAVING,
				       true, deauth_buf);
		cfg80211_tx_mlme_mgmt(sdata->dev, deauth_buf,
				      sizeof(deauth_buf));
		return;
	}

	if (sta && elems.opmode_notif)
		ieee80211_vht_handle_opmode(sdata, sta, *elems.opmode_notif,
					    rx_status->band, true);
	mutex_unlock(&local->sta_mtx);

	if (elems.country_elem && elems.pwr_constr_elem &&
	    mgmt->u.probe_resp.capab_info &
				cpu_to_le16(WLAN_CAPABILITY_SPECTRUM_MGMT))
		changed |= ieee80211_handle_pwr_constr(sdata, chan,
						       elems.country_elem,
						       elems.country_elem_len,
						       elems.pwr_constr_elem);
>>>>>>> refs/remotes/origin/master

	ieee80211_bss_info_change_notify(sdata, changed);
}

void ieee80211_sta_rx_queued_mgmt(struct ieee80211_sub_if_data *sdata,
				  struct sk_buff *skb)
{
<<<<<<< HEAD
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct ieee80211_rx_status *rx_status;
	struct ieee80211_mgmt *mgmt;
<<<<<<< HEAD
=======
	struct cfg80211_bss *bss = NULL;
>>>>>>> refs/remotes/origin/cm-10.0
	enum rx_mgmt_action rma = RX_MGMT_NONE;
	u16 fc;
=======
	struct ieee80211_rx_status *rx_status;
	struct ieee80211_mgmt *mgmt;
	u16 fc;
	struct ieee802_11_elems elems;
	int ies_len;
>>>>>>> refs/remotes/origin/master

	rx_status = (struct ieee80211_rx_status *) skb->cb;
	mgmt = (struct ieee80211_mgmt *) skb->data;
	fc = le16_to_cpu(mgmt->frame_control);

<<<<<<< HEAD
	mutex_lock(&ifmgd->mtx);

<<<<<<< HEAD
	if (ifmgd->associated &&
	    memcmp(ifmgd->associated->bssid, mgmt->bssid, ETH_ALEN) == 0) {
		switch (fc & IEEE80211_FCTL_STYPE) {
		case IEEE80211_STYPE_BEACON:
			ieee80211_rx_mgmt_beacon(sdata, mgmt, skb->len,
						 rx_status);
			break;
		case IEEE80211_STYPE_PROBE_RESP:
			ieee80211_rx_mgmt_probe_resp(sdata, skb);
			break;
		case IEEE80211_STYPE_DEAUTH:
			rma = ieee80211_rx_mgmt_deauth(sdata, mgmt, skb->len);
			break;
		case IEEE80211_STYPE_DISASSOC:
			rma = ieee80211_rx_mgmt_disassoc(sdata, mgmt, skb->len);
			break;
		case IEEE80211_STYPE_ACTION:
			switch (mgmt->u.action.category) {
			case WLAN_CATEGORY_SPECTRUM_MGMT:
				ieee80211_sta_process_chanswitch(sdata,
						&mgmt->u.action.u.chan_switch.sw_elem,
						(void *)ifmgd->associated->priv,
						rx_status->mactime);
				break;
			}
		}
		mutex_unlock(&ifmgd->mtx);

		switch (rma) {
		case RX_MGMT_NONE:
			/* no action */
			break;
		case RX_MGMT_CFG80211_DEAUTH:
			cfg80211_send_deauth(sdata->dev, (u8 *)mgmt, skb->len);
			break;
		case RX_MGMT_CFG80211_DISASSOC:
			cfg80211_send_disassoc(sdata->dev, (u8 *)mgmt, skb->len);
			break;
		default:
			WARN(1, "unexpected: %d", rma);
		}
		return;
	}

	mutex_unlock(&ifmgd->mtx);

	if (skb->len >= 24 + 2 /* mgmt + deauth reason */ &&
	    (fc & IEEE80211_FCTL_STYPE) == IEEE80211_STYPE_DEAUTH) {
		struct ieee80211_local *local = sdata->local;
		struct ieee80211_work *wk;

		mutex_lock(&local->mtx);
		list_for_each_entry(wk, &local->work_list, list) {
			if (wk->sdata != sdata)
				continue;

			if (wk->type != IEEE80211_WORK_ASSOC &&
			    wk->type != IEEE80211_WORK_ASSOC_BEACON_WAIT)
				continue;

			if (memcmp(mgmt->bssid, wk->filter_ta, ETH_ALEN))
				continue;
			if (memcmp(mgmt->sa, wk->filter_ta, ETH_ALEN))
				continue;

			/*
			 * Printing the message only here means we can't
			 * spuriously print it, but it also means that it
			 * won't be printed when the frame comes in before
			 * we even tried to associate or in similar cases.
			 *
			 * Ultimately, I suspect cfg80211 should print the
			 * messages instead.
			 */
			printk(KERN_DEBUG
			       "%s: deauthenticated from %pM (Reason: %u)\n",
			       sdata->name, mgmt->bssid,
			       le16_to_cpu(mgmt->u.deauth.reason_code));

			list_del_rcu(&wk->list);
			free_work(wk);
			break;
		}
		mutex_unlock(&local->mtx);

		cfg80211_send_deauth(sdata->dev, (u8 *)mgmt, skb->len);
=======
=======
	sdata_lock(sdata);

>>>>>>> refs/remotes/origin/master
	switch (fc & IEEE80211_FCTL_STYPE) {
	case IEEE80211_STYPE_BEACON:
		ieee80211_rx_mgmt_beacon(sdata, mgmt, skb->len, rx_status);
		break;
	case IEEE80211_STYPE_PROBE_RESP:
		ieee80211_rx_mgmt_probe_resp(sdata, skb);
		break;
	case IEEE80211_STYPE_AUTH:
<<<<<<< HEAD
		rma = ieee80211_rx_mgmt_auth(sdata, mgmt, skb->len);
		break;
	case IEEE80211_STYPE_DEAUTH:
		rma = ieee80211_rx_mgmt_deauth(sdata, mgmt, skb->len);
		break;
	case IEEE80211_STYPE_DISASSOC:
		rma = ieee80211_rx_mgmt_disassoc(sdata, mgmt, skb->len);
		break;
	case IEEE80211_STYPE_ASSOC_RESP:
	case IEEE80211_STYPE_REASSOC_RESP:
		rma = ieee80211_rx_mgmt_assoc_resp(sdata, mgmt, skb->len, &bss);
		break;
	case IEEE80211_STYPE_ACTION:
		switch (mgmt->u.action.category) {
		case WLAN_CATEGORY_SPECTRUM_MGMT:
			ieee80211_sta_process_chanswitch(sdata,
					&mgmt->u.action.u.chan_switch.sw_elem,
					(void *)ifmgd->associated->priv,
					rx_status->mactime);
			break;
		}
	}
	mutex_unlock(&ifmgd->mtx);

	switch (rma) {
	case RX_MGMT_NONE:
		/* no action */
		break;
	case RX_MGMT_CFG80211_DEAUTH:
		cfg80211_send_deauth(sdata->dev, (u8 *)mgmt, skb->len);
		break;
	case RX_MGMT_CFG80211_DISASSOC:
		cfg80211_send_disassoc(sdata->dev, (u8 *)mgmt, skb->len);
		break;
	case RX_MGMT_CFG80211_RX_AUTH:
		cfg80211_send_rx_auth(sdata->dev, (u8 *)mgmt, skb->len);
		break;
	case RX_MGMT_CFG80211_RX_ASSOC:
		cfg80211_send_rx_assoc(sdata->dev, bss, (u8 *)mgmt, skb->len);
		break;
	case RX_MGMT_CFG80211_ASSOC_TIMEOUT:
		cfg80211_send_assoc_timeout(sdata->dev, mgmt->bssid);
		break;
	default:
		WARN(1, "unexpected: %d", rma);
>>>>>>> refs/remotes/origin/cm-10.0
	}
=======
		ieee80211_rx_mgmt_auth(sdata, mgmt, skb->len);
		break;
	case IEEE80211_STYPE_DEAUTH:
		ieee80211_rx_mgmt_deauth(sdata, mgmt, skb->len);
		break;
	case IEEE80211_STYPE_DISASSOC:
		ieee80211_rx_mgmt_disassoc(sdata, mgmt, skb->len);
		break;
	case IEEE80211_STYPE_ASSOC_RESP:
	case IEEE80211_STYPE_REASSOC_RESP:
		ieee80211_rx_mgmt_assoc_resp(sdata, mgmt, skb->len);
		break;
	case IEEE80211_STYPE_ACTION:
		if (mgmt->u.action.category == WLAN_CATEGORY_SPECTRUM_MGMT) {
			ies_len = skb->len -
				  offsetof(struct ieee80211_mgmt,
					   u.action.u.chan_switch.variable);

			if (ies_len < 0)
				break;

			ieee802_11_parse_elems(
				mgmt->u.action.u.chan_switch.variable,
				ies_len, true, &elems);

			if (elems.parse_error)
				break;

			ieee80211_sta_process_chanswitch(sdata,
							 rx_status->mactime,
							 &elems, false);
		} else if (mgmt->u.action.category == WLAN_CATEGORY_PUBLIC) {
			ies_len = skb->len -
				  offsetof(struct ieee80211_mgmt,
					   u.action.u.ext_chan_switch.variable);

			if (ies_len < 0)
				break;

			ieee802_11_parse_elems(
				mgmt->u.action.u.ext_chan_switch.variable,
				ies_len, true, &elems);

			if (elems.parse_error)
				break;

			/* for the handling code pretend this was also an IE */
			elems.ext_chansw_ie =
				&mgmt->u.action.u.ext_chan_switch.data;

			ieee80211_sta_process_chanswitch(sdata,
							 rx_status->mactime,
							 &elems, false);
		}
		break;
	}
	sdata_unlock(sdata);
>>>>>>> refs/remotes/origin/master
}

static void ieee80211_sta_timer(unsigned long data)
{
	struct ieee80211_sub_if_data *sdata =
		(struct ieee80211_sub_if_data *) data;
<<<<<<< HEAD
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct ieee80211_local *local = sdata->local;

	if (local->quiescing) {
		set_bit(TMR_RUNNING_TIMER, &ifmgd->timers_running);
		return;
	}

	ieee80211_queue_work(&local->hw, &sdata->work);
}

static void ieee80211_sta_connection_lost(struct ieee80211_sub_if_data *sdata,
<<<<<<< HEAD
					  u8 *bssid)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
=======
					  u8 *bssid, u8 reason)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	u8 frame_buf[DEAUTH_DISASSOC_LEN];
>>>>>>> refs/remotes/origin/cm-10.0

	ifmgd->flags &= ~(IEEE80211_STA_CONNECTION_POLL |
			  IEEE80211_STA_BEACON_POLL);

<<<<<<< HEAD
	ieee80211_set_disassoc(sdata, true, true);
	mutex_unlock(&ifmgd->mtx);
	mutex_lock(&local->mtx);
	ieee80211_recalc_idle(local);
	mutex_unlock(&local->mtx);
=======
	ieee80211_set_disassoc(sdata, IEEE80211_STYPE_DEAUTH, reason,
			       false, frame_buf);
	mutex_unlock(&ifmgd->mtx);

>>>>>>> refs/remotes/origin/cm-10.0
	/*
	 * must be outside lock due to cfg80211,
	 * but that's not a problem.
	 */
<<<<<<< HEAD
	ieee80211_send_deauth_disassoc(sdata, bssid,
			IEEE80211_STYPE_DEAUTH,
			WLAN_REASON_DISASSOC_DUE_TO_INACTIVITY,
			NULL, true);
	mutex_lock(&ifmgd->mtx);
}

=======
	cfg80211_send_deauth(sdata->dev, frame_buf, DEAUTH_DISASSOC_LEN);

	mutex_lock(&local->mtx);
	ieee80211_recalc_idle(local);
	mutex_unlock(&local->mtx);

	mutex_lock(&ifmgd->mtx);
=======

	ieee80211_queue_work(&sdata->local->hw, &sdata->work);
}

static void ieee80211_sta_connection_lost(struct ieee80211_sub_if_data *sdata,
					  u8 *bssid, u8 reason, bool tx)
{
	u8 frame_buf[IEEE80211_DEAUTH_FRAME_LEN];

	ieee80211_set_disassoc(sdata, IEEE80211_STYPE_DEAUTH, reason,
			       tx, frame_buf);

	cfg80211_tx_mlme_mgmt(sdata->dev, frame_buf,
			      IEEE80211_DEAUTH_FRAME_LEN);
>>>>>>> refs/remotes/origin/master
}

static int ieee80211_probe_auth(struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct ieee80211_mgd_auth_data *auth_data = ifmgd->auth_data;
<<<<<<< HEAD

	lockdep_assert_held(&ifmgd->mtx);
=======
	u32 tx_flags = 0;

	sdata_assert_lock(sdata);
>>>>>>> refs/remotes/origin/master

	if (WARN_ON_ONCE(!auth_data))
		return -EINVAL;

	auth_data->tries++;

	if (auth_data->tries > IEEE80211_AUTH_MAX_TRIES) {
<<<<<<< HEAD
		printk(KERN_DEBUG "%s: authentication with %pM timed out\n",
		       sdata->name, auth_data->bss->bssid);
=======
		sdata_info(sdata, "authentication with %pM timed out\n",
			   auth_data->bss->bssid);
>>>>>>> refs/remotes/origin/master

		/*
		 * Most likely AP is not in the range so remove the
		 * bss struct for that AP.
		 */
		cfg80211_unlink_bss(local->hw.wiphy, auth_data->bss);

		return -ETIMEDOUT;
	}

<<<<<<< HEAD
	if (auth_data->bss->proberesp_ies) {
		printk(KERN_DEBUG "%s: send auth to %pM (try %d/%d)\n",
		       sdata->name, auth_data->bss->bssid, auth_data->tries,
		       IEEE80211_AUTH_MAX_TRIES);

		auth_data->expected_transaction = 2;
		ieee80211_send_auth(sdata, 1, auth_data->algorithm,
				    auth_data->ie, auth_data->ie_len,
				    auth_data->bss->bssid,
				    auth_data->bss->bssid, NULL, 0, 0);
	} else {
		const u8 *ssidie;

		printk(KERN_DEBUG "%s: direct probe to %pM (try %d/%i)\n",
		       sdata->name, auth_data->bss->bssid, auth_data->tries,
		       IEEE80211_AUTH_MAX_TRIES);

		ssidie = ieee80211_bss_get_ie(auth_data->bss, WLAN_EID_SSID);
		if (!ssidie)
			return -EINVAL;
=======
	drv_mgd_prepare_tx(local, sdata);

	if (auth_data->bss->proberesp_ies) {
		u16 trans = 1;
		u16 status = 0;

		sdata_info(sdata, "send auth to %pM (try %d/%d)\n",
			   auth_data->bss->bssid, auth_data->tries,
			   IEEE80211_AUTH_MAX_TRIES);

		auth_data->expected_transaction = 2;

		if (auth_data->algorithm == WLAN_AUTH_SAE) {
			trans = auth_data->sae_trans;
			status = auth_data->sae_status;
			auth_data->expected_transaction = trans;
		}

		if (local->hw.flags & IEEE80211_HW_REPORTS_TX_ACK_STATUS)
			tx_flags = IEEE80211_TX_CTL_REQ_TX_STATUS |
				   IEEE80211_TX_INTFL_MLME_CONN_TX;

		ieee80211_send_auth(sdata, trans, auth_data->algorithm, status,
				    auth_data->data, auth_data->data_len,
				    auth_data->bss->bssid,
				    auth_data->bss->bssid, NULL, 0, 0,
				    tx_flags);
	} else {
		const u8 *ssidie;

		sdata_info(sdata, "direct probe to %pM (try %d/%i)\n",
			   auth_data->bss->bssid, auth_data->tries,
			   IEEE80211_AUTH_MAX_TRIES);

		rcu_read_lock();
		ssidie = ieee80211_bss_get_ie(auth_data->bss, WLAN_EID_SSID);
		if (!ssidie) {
			rcu_read_unlock();
			return -EINVAL;
		}
>>>>>>> refs/remotes/origin/master
		/*
		 * Direct probe is sent to broadcast address as some APs
		 * will not answer to direct packet in unassociated state.
		 */
		ieee80211_send_probe_req(sdata, NULL, ssidie + 2, ssidie[1],
<<<<<<< HEAD
					 NULL, 0, (u32) -1, true, false);
	}

	auth_data->timeout = jiffies + IEEE80211_AUTH_TIMEOUT;
	run_again(ifmgd, auth_data->timeout);
=======
					 NULL, 0, (u32) -1, true, 0,
					 auth_data->bss->channel, false);
		rcu_read_unlock();
	}

	if (tx_flags == 0) {
		auth_data->timeout = jiffies + IEEE80211_AUTH_TIMEOUT;
		auth_data->timeout_started = true;
		run_again(sdata, auth_data->timeout);
	} else {
		auth_data->timeout =
			round_jiffies_up(jiffies + IEEE80211_AUTH_TIMEOUT_LONG);
		auth_data->timeout_started = true;
		run_again(sdata, auth_data->timeout);
	}
>>>>>>> refs/remotes/origin/master

	return 0;
}

static int ieee80211_do_assoc(struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_mgd_assoc_data *assoc_data = sdata->u.mgd.assoc_data;
	struct ieee80211_local *local = sdata->local;

<<<<<<< HEAD
	lockdep_assert_held(&sdata->u.mgd.mtx);

	assoc_data->tries++;
	if (assoc_data->tries > IEEE80211_ASSOC_MAX_TRIES) {
		printk(KERN_DEBUG "%s: association with %pM timed out\n",
		       sdata->name, assoc_data->bss->bssid);
=======
	sdata_assert_lock(sdata);

	assoc_data->tries++;
	if (assoc_data->tries > IEEE80211_ASSOC_MAX_TRIES) {
		sdata_info(sdata, "association with %pM timed out\n",
			   assoc_data->bss->bssid);
>>>>>>> refs/remotes/origin/master

		/*
		 * Most likely AP is not in the range so remove the
		 * bss struct for that AP.
		 */
		cfg80211_unlink_bss(local->hw.wiphy, assoc_data->bss);

		return -ETIMEDOUT;
	}

<<<<<<< HEAD
	printk(KERN_DEBUG "%s: associate with %pM (try %d/%d)\n",
	       sdata->name, assoc_data->bss->bssid, assoc_data->tries,
	       IEEE80211_ASSOC_MAX_TRIES);
	ieee80211_send_assoc(sdata);

	assoc_data->timeout = jiffies + IEEE80211_ASSOC_TIMEOUT;
	run_again(&sdata->u.mgd, assoc_data->timeout);
=======
	sdata_info(sdata, "associate with %pM (try %d/%d)\n",
		   assoc_data->bss->bssid, assoc_data->tries,
		   IEEE80211_ASSOC_MAX_TRIES);
	ieee80211_send_assoc(sdata);

	if (!(local->hw.flags & IEEE80211_HW_REPORTS_TX_ACK_STATUS)) {
		assoc_data->timeout = jiffies + IEEE80211_ASSOC_TIMEOUT;
		assoc_data->timeout_started = true;
		run_again(sdata, assoc_data->timeout);
	} else {
		assoc_data->timeout =
			round_jiffies_up(jiffies +
					 IEEE80211_ASSOC_TIMEOUT_LONG);
		assoc_data->timeout_started = true;
		run_again(sdata, assoc_data->timeout);
	}
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
void ieee80211_mgd_conn_tx_status(struct ieee80211_sub_if_data *sdata,
				  __le16 fc, bool acked)
{
	struct ieee80211_local *local = sdata->local;

	sdata->u.mgd.status_fc = fc;
	sdata->u.mgd.status_acked = acked;
	sdata->u.mgd.status_received = true;

	ieee80211_queue_work(&local->hw, &sdata->work);
}

>>>>>>> refs/remotes/origin/master
void ieee80211_sta_work(struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;

<<<<<<< HEAD
<<<<<<< HEAD
	/* then process the rest of the work */
	mutex_lock(&ifmgd->mtx);

=======
	mutex_lock(&ifmgd->mtx);

	if (ifmgd->auth_data &&
	    time_after(jiffies, ifmgd->auth_data->timeout)) {
		if (ifmgd->auth_data->done) {
			/*
=======
	sdata_lock(sdata);

	if (ifmgd->status_received) {
		__le16 fc = ifmgd->status_fc;
		bool status_acked = ifmgd->status_acked;

		ifmgd->status_received = false;
		if (ifmgd->auth_data &&
		    (ieee80211_is_probe_req(fc) || ieee80211_is_auth(fc))) {
			if (status_acked) {
				ifmgd->auth_data->timeout =
					jiffies + IEEE80211_AUTH_TIMEOUT_SHORT;
				run_again(sdata, ifmgd->auth_data->timeout);
			} else {
				ifmgd->auth_data->timeout = jiffies - 1;
			}
			ifmgd->auth_data->timeout_started = true;
		} else if (ifmgd->assoc_data &&
			   (ieee80211_is_assoc_req(fc) ||
			    ieee80211_is_reassoc_req(fc))) {
			if (status_acked) {
				ifmgd->assoc_data->timeout =
					jiffies + IEEE80211_ASSOC_TIMEOUT_SHORT;
				run_again(sdata, ifmgd->assoc_data->timeout);
			} else {
				ifmgd->assoc_data->timeout = jiffies - 1;
			}
			ifmgd->assoc_data->timeout_started = true;
		}
	}

	if (ifmgd->auth_data && ifmgd->auth_data->timeout_started &&
	    time_after(jiffies, ifmgd->auth_data->timeout)) {
		if (ifmgd->auth_data->done) {
			/*
>>>>>>> refs/remotes/origin/master
			 * ok ... we waited for assoc but userspace didn't,
			 * so let's just kill the auth data
			 */
			ieee80211_destroy_auth_data(sdata, false);
		} else if (ieee80211_probe_auth(sdata)) {
			u8 bssid[ETH_ALEN];

			memcpy(bssid, ifmgd->auth_data->bss->bssid, ETH_ALEN);

			ieee80211_destroy_auth_data(sdata, false);

<<<<<<< HEAD
			mutex_unlock(&ifmgd->mtx);
			cfg80211_send_auth_timeout(sdata->dev, bssid);
			mutex_lock(&ifmgd->mtx);
		}
	} else if (ifmgd->auth_data)
		run_again(ifmgd, ifmgd->auth_data->timeout);

	if (ifmgd->assoc_data &&
	    time_after(jiffies, ifmgd->assoc_data->timeout)) {
		if (!ifmgd->assoc_data->have_beacon ||
		    ieee80211_do_assoc(sdata)) {
			u8 bssid[ETH_ALEN];

			memcpy(bssid, ifmgd->assoc_data->bss->bssid, ETH_ALEN);

			ieee80211_destroy_assoc_data(sdata, false);

			mutex_unlock(&ifmgd->mtx);
			cfg80211_send_assoc_timeout(sdata->dev, bssid);
			mutex_lock(&ifmgd->mtx);
		}
	} else if (ifmgd->assoc_data)
		run_again(ifmgd, ifmgd->assoc_data->timeout);

>>>>>>> refs/remotes/origin/cm-10.0
	if (ifmgd->flags & (IEEE80211_STA_BEACON_POLL |
			    IEEE80211_STA_CONNECTION_POLL) &&
=======
			cfg80211_auth_timeout(sdata->dev, bssid);
		}
	} else if (ifmgd->auth_data && ifmgd->auth_data->timeout_started)
		run_again(sdata, ifmgd->auth_data->timeout);

	if (ifmgd->assoc_data && ifmgd->assoc_data->timeout_started &&
	    time_after(jiffies, ifmgd->assoc_data->timeout)) {
		if ((ifmgd->assoc_data->need_beacon && !ifmgd->have_beacon) ||
		    ieee80211_do_assoc(sdata)) {
			struct cfg80211_bss *bss = ifmgd->assoc_data->bss;

			ieee80211_destroy_assoc_data(sdata, false);
			cfg80211_assoc_timeout(sdata->dev, bss);
		}
	} else if (ifmgd->assoc_data && ifmgd->assoc_data->timeout_started)
		run_again(sdata, ifmgd->assoc_data->timeout);

	if (ifmgd->flags & IEEE80211_STA_CONNECTION_POLL &&
>>>>>>> refs/remotes/origin/master
	    ifmgd->associated) {
		u8 bssid[ETH_ALEN];
		int max_tries;

		memcpy(bssid, ifmgd->associated->bssid, ETH_ALEN);

		if (local->hw.flags & IEEE80211_HW_REPORTS_TX_ACK_STATUS)
			max_tries = max_nullfunc_tries;
		else
			max_tries = max_probe_tries;

		/* ACK received for nullfunc probing frame */
		if (!ifmgd->probe_send_count)
			ieee80211_reset_ap_probe(sdata);
		else if (ifmgd->nullfunc_failed) {
			if (ifmgd->probe_send_count < max_tries) {
<<<<<<< HEAD
#ifdef CONFIG_MAC80211_VERBOSE_DEBUG
				wiphy_debug(local->hw.wiphy,
					    "%s: No ack for nullfunc frame to"
					    " AP %pM, try %d/%i\n",
					    sdata->name, bssid,
					    ifmgd->probe_send_count, max_tries);
#endif
				ieee80211_mgd_probe_ap_send(sdata);
			} else {
#ifdef CONFIG_MAC80211_VERBOSE_DEBUG
				wiphy_debug(local->hw.wiphy,
					    "%s: No ack for nullfunc frame to"
					    " AP %pM, disconnecting.\n",
					    sdata->name, bssid);
#endif
<<<<<<< HEAD
				ieee80211_sta_connection_lost(sdata, bssid);
=======
				ieee80211_sta_connection_lost(sdata, bssid,
					WLAN_REASON_DISASSOC_DUE_TO_INACTIVITY);
>>>>>>> refs/remotes/origin/cm-10.0
			}
		} else if (time_is_after_jiffies(ifmgd->probe_timeout))
			run_again(ifmgd, ifmgd->probe_timeout);
		else if (local->hw.flags & IEEE80211_HW_REPORTS_TX_ACK_STATUS) {
#ifdef CONFIG_MAC80211_VERBOSE_DEBUG
			wiphy_debug(local->hw.wiphy,
				    "%s: Failed to send nullfunc to AP %pM"
				    " after %dms, disconnecting.\n",
				    sdata->name,
				    bssid, probe_wait_ms);
#endif
<<<<<<< HEAD
			ieee80211_sta_connection_lost(sdata, bssid);
=======
			ieee80211_sta_connection_lost(sdata, bssid,
				WLAN_REASON_DISASSOC_DUE_TO_INACTIVITY);
>>>>>>> refs/remotes/origin/cm-10.0
		} else if (ifmgd->probe_send_count < max_tries) {
#ifdef CONFIG_MAC80211_VERBOSE_DEBUG
			wiphy_debug(local->hw.wiphy,
				    "%s: No probe response from AP %pM"
				    " after %dms, try %d/%i\n",
				    sdata->name,
				    bssid, probe_wait_ms,
				    ifmgd->probe_send_count, max_tries);
#endif
=======
				mlme_dbg(sdata,
					 "No ack for nullfunc frame to AP %pM, try %d/%i\n",
					 bssid, ifmgd->probe_send_count,
					 max_tries);
				ieee80211_mgd_probe_ap_send(sdata);
			} else {
				mlme_dbg(sdata,
					 "No ack for nullfunc frame to AP %pM, disconnecting.\n",
					 bssid);
				ieee80211_sta_connection_lost(sdata, bssid,
					WLAN_REASON_DISASSOC_DUE_TO_INACTIVITY,
					false);
			}
		} else if (time_is_after_jiffies(ifmgd->probe_timeout))
			run_again(sdata, ifmgd->probe_timeout);
		else if (local->hw.flags & IEEE80211_HW_REPORTS_TX_ACK_STATUS) {
			mlme_dbg(sdata,
				 "Failed to send nullfunc to AP %pM after %dms, disconnecting\n",
				 bssid, probe_wait_ms);
			ieee80211_sta_connection_lost(sdata, bssid,
				WLAN_REASON_DISASSOC_DUE_TO_INACTIVITY, false);
		} else if (ifmgd->probe_send_count < max_tries) {
			mlme_dbg(sdata,
				 "No probe response from AP %pM after %dms, try %d/%i\n",
				 bssid, probe_wait_ms,
				 ifmgd->probe_send_count, max_tries);
>>>>>>> refs/remotes/origin/master
			ieee80211_mgd_probe_ap_send(sdata);
		} else {
			/*
			 * We actually lost the connection ... or did we?
			 * Let's make sure!
			 */
			wiphy_debug(local->hw.wiphy,
				    "%s: No probe response from AP %pM"
				    " after %dms, disconnecting.\n",
				    sdata->name,
				    bssid, probe_wait_ms);

<<<<<<< HEAD
<<<<<<< HEAD
			ieee80211_sta_connection_lost(sdata, bssid);
=======
			ieee80211_sta_connection_lost(sdata, bssid,
				WLAN_REASON_DISASSOC_DUE_TO_INACTIVITY);
>>>>>>> refs/remotes/origin/cm-10.0
		}
	}

	mutex_unlock(&ifmgd->mtx);
<<<<<<< HEAD
=======

	mutex_lock(&local->mtx);
	ieee80211_recalc_idle(local);
	mutex_unlock(&local->mtx);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			ieee80211_sta_connection_lost(sdata, bssid,
				WLAN_REASON_DISASSOC_DUE_TO_INACTIVITY, false);
		}
	}

	sdata_unlock(sdata);
>>>>>>> refs/remotes/origin/master
}

static void ieee80211_sta_bcn_mon_timer(unsigned long data)
{
	struct ieee80211_sub_if_data *sdata =
		(struct ieee80211_sub_if_data *) data;
	struct ieee80211_local *local = sdata->local;

	if (local->quiescing)
		return;

<<<<<<< HEAD
=======
	sdata->u.mgd.connection_loss = false;
>>>>>>> refs/remotes/origin/master
	ieee80211_queue_work(&sdata->local->hw,
			     &sdata->u.mgd.beacon_connection_loss_work);
}

static void ieee80211_sta_conn_mon_timer(unsigned long data)
{
	struct ieee80211_sub_if_data *sdata =
		(struct ieee80211_sub_if_data *) data;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct ieee80211_local *local = sdata->local;

	if (local->quiescing)
		return;

	ieee80211_queue_work(&local->hw, &ifmgd->monitor_work);
}

static void ieee80211_sta_monitor_work(struct work_struct *work)
{
	struct ieee80211_sub_if_data *sdata =
		container_of(work, struct ieee80211_sub_if_data,
			     u.mgd.monitor_work);

	ieee80211_mgd_probe_ap(sdata, false);
}

static void ieee80211_restart_sta_timer(struct ieee80211_sub_if_data *sdata)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
	u32 flags;

>>>>>>> refs/remotes/origin/cm-10.0
	if (sdata->vif.type == NL80211_IFTYPE_STATION) {
		sdata->u.mgd.flags &= ~(IEEE80211_STA_BEACON_POLL |
					IEEE80211_STA_CONNECTION_POLL);

		/* let's probe the connection once */
<<<<<<< HEAD
		ieee80211_queue_work(&sdata->local->hw,
			   &sdata->u.mgd.monitor_work);
=======
=======
	u32 flags;

	if (sdata->vif.type == NL80211_IFTYPE_STATION) {
		__ieee80211_stop_poll(sdata);

		/* let's probe the connection once */
>>>>>>> refs/remotes/origin/master
		flags = sdata->local->hw.flags;
		if (!(flags & IEEE80211_HW_CONNECTION_MONITOR))
			ieee80211_queue_work(&sdata->local->hw,
					     &sdata->u.mgd.monitor_work);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		/* and do all the other regular work too */
		ieee80211_queue_work(&sdata->local->hw, &sdata->work);
	}
}

#ifdef CONFIG_PM
<<<<<<< HEAD
void ieee80211_sta_quiesce(struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;

	/*
	 * we need to use atomic bitops for the running bits
	 * only because both timers might fire at the same
	 * time -- the code here is properly synchronised.
	 */

	cancel_work_sync(&ifmgd->request_smps_work);

<<<<<<< HEAD
=======
	cancel_work_sync(&ifmgd->monitor_work);
>>>>>>> refs/remotes/origin/cm-10.0
	cancel_work_sync(&ifmgd->beacon_connection_loss_work);
	if (del_timer_sync(&ifmgd->timer))
		set_bit(TMR_RUNNING_TIMER, &ifmgd->timers_running);

	cancel_work_sync(&ifmgd->chswitch_work);
	if (del_timer_sync(&ifmgd->chswitch_timer))
		set_bit(TMR_RUNNING_CHANSW, &ifmgd->timers_running);

<<<<<<< HEAD
	cancel_work_sync(&ifmgd->monitor_work);
=======
>>>>>>> refs/remotes/origin/cm-10.0
	/* these will just be re-established on connection */
	del_timer_sync(&ifmgd->conn_mon_timer);
	del_timer_sync(&ifmgd->bcn_mon_timer);
}

=======
>>>>>>> refs/remotes/origin/master
void ieee80211_sta_restart(struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;

<<<<<<< HEAD
	if (!ifmgd->associated)
		return;

<<<<<<< HEAD
=======
	if (sdata->flags & IEEE80211_SDATA_DISCONNECT_RESUME) {
		sdata->flags &= ~IEEE80211_SDATA_DISCONNECT_RESUME;
		mutex_lock(&ifmgd->mtx);
		if (ifmgd->associated) {
#ifdef CONFIG_MAC80211_VERBOSE_DEBUG
			wiphy_debug(sdata->local->hw.wiphy,
				    "%s: driver requested disconnect after resume.\n",
				    sdata->name);
#endif
			ieee80211_sta_connection_lost(sdata,
				ifmgd->associated->bssid,
				WLAN_REASON_UNSPECIFIED);
			mutex_unlock(&ifmgd->mtx);
			return;
		}
		mutex_unlock(&ifmgd->mtx);
	}

>>>>>>> refs/remotes/origin/cm-10.0
	if (test_and_clear_bit(TMR_RUNNING_TIMER, &ifmgd->timers_running))
		add_timer(&ifmgd->timer);
	if (test_and_clear_bit(TMR_RUNNING_CHANSW, &ifmgd->timers_running))
		add_timer(&ifmgd->chswitch_timer);
	ieee80211_sta_reset_beacon_monitor(sdata);
	ieee80211_restart_sta_timer(sdata);
=======
	sdata_lock(sdata);
	if (!ifmgd->associated) {
		sdata_unlock(sdata);
		return;
	}

	if (sdata->flags & IEEE80211_SDATA_DISCONNECT_RESUME) {
		sdata->flags &= ~IEEE80211_SDATA_DISCONNECT_RESUME;
		mlme_dbg(sdata, "driver requested disconnect after resume\n");
		ieee80211_sta_connection_lost(sdata,
					      ifmgd->associated->bssid,
					      WLAN_REASON_UNSPECIFIED,
					      true);
		sdata_unlock(sdata);
		return;
	}
	sdata_unlock(sdata);
>>>>>>> refs/remotes/origin/master
}
#endif

/* interface setup */
void ieee80211_sta_setup_sdata(struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_if_managed *ifmgd;

	ifmgd = &sdata->u.mgd;
	INIT_WORK(&ifmgd->monitor_work, ieee80211_sta_monitor_work);
	INIT_WORK(&ifmgd->chswitch_work, ieee80211_chswitch_work);
	INIT_WORK(&ifmgd->beacon_connection_loss_work,
		  ieee80211_beacon_connection_loss_work);
<<<<<<< HEAD
	INIT_WORK(&ifmgd->request_smps_work, ieee80211_request_smps_work);
=======
	INIT_WORK(&ifmgd->csa_connection_drop_work,
		  ieee80211_csa_connection_drop_work);
	INIT_WORK(&ifmgd->request_smps_work, ieee80211_request_smps_mgd_work);
>>>>>>> refs/remotes/origin/master
	setup_timer(&ifmgd->timer, ieee80211_sta_timer,
		    (unsigned long) sdata);
	setup_timer(&ifmgd->bcn_mon_timer, ieee80211_sta_bcn_mon_timer,
		    (unsigned long) sdata);
	setup_timer(&ifmgd->conn_mon_timer, ieee80211_sta_conn_mon_timer,
		    (unsigned long) sdata);
	setup_timer(&ifmgd->chswitch_timer, ieee80211_chswitch_timer,
		    (unsigned long) sdata);

	ifmgd->flags = 0;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	ifmgd->powersave = sdata->wdev.ps;
	ifmgd->uapsd_queues = IEEE80211_DEFAULT_UAPSD_QUEUES;
	ifmgd->uapsd_max_sp_len = IEEE80211_DEFAULT_MAX_SP_LEN;
>>>>>>> refs/remotes/origin/cm-10.0

	mutex_init(&ifmgd->mtx);
=======
	ifmgd->powersave = sdata->wdev.ps;
	ifmgd->uapsd_queues = sdata->local->hw.uapsd_queues;
	ifmgd->uapsd_max_sp_len = sdata->local->hw.uapsd_max_sp_len;
	ifmgd->p2p_noa_index = -1;
>>>>>>> refs/remotes/origin/master

	if (sdata->local->hw.flags & IEEE80211_HW_SUPPORTS_DYNAMIC_SMPS)
		ifmgd->req_smps = IEEE80211_SMPS_AUTOMATIC;
	else
		ifmgd->req_smps = IEEE80211_SMPS_OFF;
}

/* scan finished notification */
void ieee80211_mlme_notify_scan_completed(struct ieee80211_local *local)
{
<<<<<<< HEAD
	struct ieee80211_sub_if_data *sdata = local->scan_sdata;

	/* Restart STA timers */
	rcu_read_lock();
	list_for_each_entry_rcu(sdata, &local->interfaces, list)
		ieee80211_restart_sta_timer(sdata);
=======
	struct ieee80211_sub_if_data *sdata;

	/* Restart STA timers */
	rcu_read_lock();
	list_for_each_entry_rcu(sdata, &local->interfaces, list) {
		if (ieee80211_sdata_running(sdata))
			ieee80211_restart_sta_timer(sdata);
	}
>>>>>>> refs/remotes/origin/master
	rcu_read_unlock();
}

int ieee80211_max_network_latency(struct notifier_block *nb,
				  unsigned long data, void *dummy)
{
	s32 latency_usec = (s32) data;
	struct ieee80211_local *local =
		container_of(nb, struct ieee80211_local,
			     network_latency_notifier);

	mutex_lock(&local->iflist_mtx);
	ieee80211_recalc_ps(local, latency_usec);
	mutex_unlock(&local->iflist_mtx);

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
/* config hooks */
static enum work_done_result
ieee80211_probe_auth_done(struct ieee80211_work *wk,
			  struct sk_buff *skb)
{
	if (!skb) {
		cfg80211_send_auth_timeout(wk->sdata->dev, wk->filter_ta);
		return WORK_DONE_DESTROY;
	}

	if (wk->type == IEEE80211_WORK_AUTH) {
		cfg80211_send_rx_auth(wk->sdata->dev, skb->data, skb->len);
		return WORK_DONE_DESTROY;
	}

	mutex_lock(&wk->sdata->u.mgd.mtx);
	ieee80211_rx_mgmt_probe_resp(wk->sdata, skb);
	mutex_unlock(&wk->sdata->u.mgd.mtx);

	wk->type = IEEE80211_WORK_AUTH;
	wk->probe_auth.tries = 0;
	return WORK_DONE_REQUEUE;
}

int ieee80211_mgd_auth(struct ieee80211_sub_if_data *sdata,
		       struct cfg80211_auth_request *req)
{
	const u8 *ssid;
	struct ieee80211_work *wk;
	u16 auth_alg;

	if (req->local_state_change)
		return 0; /* no need to update mac80211 state */
=======
=======
static u8 ieee80211_ht_vht_rx_chains(struct ieee80211_sub_if_data *sdata,
				     struct cfg80211_bss *cbss)
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	const u8 *ht_cap_ie, *vht_cap_ie;
	const struct ieee80211_ht_cap *ht_cap;
	const struct ieee80211_vht_cap *vht_cap;
	u8 chains = 1;

	if (ifmgd->flags & IEEE80211_STA_DISABLE_HT)
		return chains;

	ht_cap_ie = ieee80211_bss_get_ie(cbss, WLAN_EID_HT_CAPABILITY);
	if (ht_cap_ie && ht_cap_ie[1] >= sizeof(*ht_cap)) {
		ht_cap = (void *)(ht_cap_ie + 2);
		chains = ieee80211_mcs_to_chains(&ht_cap->mcs);
		/*
		 * TODO: use "Tx Maximum Number Spatial Streams Supported" and
		 *	 "Tx Unequal Modulation Supported" fields.
		 */
	}

	if (ifmgd->flags & IEEE80211_STA_DISABLE_VHT)
		return chains;

	vht_cap_ie = ieee80211_bss_get_ie(cbss, WLAN_EID_VHT_CAPABILITY);
	if (vht_cap_ie && vht_cap_ie[1] >= sizeof(*vht_cap)) {
		u8 nss;
		u16 tx_mcs_map;

		vht_cap = (void *)(vht_cap_ie + 2);
		tx_mcs_map = le16_to_cpu(vht_cap->supp_mcs.tx_mcs_map);
		for (nss = 8; nss > 0; nss--) {
			if (((tx_mcs_map >> (2 * (nss - 1))) & 3) !=
					IEEE80211_VHT_MCS_NOT_SUPPORTED)
				break;
		}
		/* TODO: use "Tx Highest Supported Long GI Data Rate" field? */
		chains = max(chains, nss);
	}

	return chains;
}

static int ieee80211_prep_channel(struct ieee80211_sub_if_data *sdata,
				  struct cfg80211_bss *cbss)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	const struct ieee80211_ht_operation *ht_oper = NULL;
	const struct ieee80211_vht_operation *vht_oper = NULL;
	struct ieee80211_supported_band *sband;
	struct cfg80211_chan_def chandef;
	int ret;

	sband = local->hw.wiphy->bands[cbss->channel->band];

	ifmgd->flags &= ~(IEEE80211_STA_DISABLE_40MHZ |
			  IEEE80211_STA_DISABLE_80P80MHZ |
			  IEEE80211_STA_DISABLE_160MHZ);

	rcu_read_lock();

	if (!(ifmgd->flags & IEEE80211_STA_DISABLE_HT) &&
	    sband->ht_cap.ht_supported) {
		const u8 *ht_oper_ie, *ht_cap;

		ht_oper_ie = ieee80211_bss_get_ie(cbss, WLAN_EID_HT_OPERATION);
		if (ht_oper_ie && ht_oper_ie[1] >= sizeof(*ht_oper))
			ht_oper = (void *)(ht_oper_ie + 2);

		ht_cap = ieee80211_bss_get_ie(cbss, WLAN_EID_HT_CAPABILITY);
		if (!ht_cap || ht_cap[1] < sizeof(struct ieee80211_ht_cap)) {
			ifmgd->flags |= IEEE80211_STA_DISABLE_HT;
			ht_oper = NULL;
		}
	}

	if (!(ifmgd->flags & IEEE80211_STA_DISABLE_VHT) &&
	    sband->vht_cap.vht_supported) {
		const u8 *vht_oper_ie, *vht_cap;

		vht_oper_ie = ieee80211_bss_get_ie(cbss,
						   WLAN_EID_VHT_OPERATION);
		if (vht_oper_ie && vht_oper_ie[1] >= sizeof(*vht_oper))
			vht_oper = (void *)(vht_oper_ie + 2);
		if (vht_oper && !ht_oper) {
			vht_oper = NULL;
			sdata_info(sdata,
				   "AP advertised VHT without HT, disabling both\n");
			ifmgd->flags |= IEEE80211_STA_DISABLE_HT;
			ifmgd->flags |= IEEE80211_STA_DISABLE_VHT;
		}

		vht_cap = ieee80211_bss_get_ie(cbss, WLAN_EID_VHT_CAPABILITY);
		if (!vht_cap || vht_cap[1] < sizeof(struct ieee80211_vht_cap)) {
			ifmgd->flags |= IEEE80211_STA_DISABLE_VHT;
			vht_oper = NULL;
		}
	}

	ifmgd->flags |= ieee80211_determine_chantype(sdata, sband,
						     cbss->channel,
						     ht_oper, vht_oper,
						     &chandef, false);

	sdata->needed_rx_chains = min(ieee80211_ht_vht_rx_chains(sdata, cbss),
				      local->rx_chains);

	rcu_read_unlock();

	/* will change later if needed */
	sdata->smps_mode = IEEE80211_SMPS_OFF;

	/*
	 * If this fails (possibly due to channel context sharing
	 * on incompatible channels, e.g. 80+80 and 160 sharing the
	 * same control channel) try to use a smaller bandwidth.
	 */
	ret = ieee80211_vif_use_channel(sdata, &chandef,
					IEEE80211_CHANCTX_SHARED);

	/* don't downgrade for 5 and 10 MHz channels, though. */
	if (chandef.width == NL80211_CHAN_WIDTH_5 ||
	    chandef.width == NL80211_CHAN_WIDTH_10)
		return ret;

	while (ret && chandef.width != NL80211_CHAN_WIDTH_20_NOHT) {
		ifmgd->flags |= ieee80211_chandef_downgrade(&chandef);
		ret = ieee80211_vif_use_channel(sdata, &chandef,
						IEEE80211_CHANCTX_SHARED);
	}
	return ret;
}

>>>>>>> refs/remotes/origin/master
static int ieee80211_prep_connection(struct ieee80211_sub_if_data *sdata,
				     struct cfg80211_bss *cbss, bool assoc)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct ieee80211_bss *bss = (void *)cbss->priv;
<<<<<<< HEAD
	struct sta_info *sta;
=======
	struct sta_info *new_sta = NULL;
>>>>>>> refs/remotes/origin/master
	bool have_sta = false;
	int err;

	if (WARN_ON(!ifmgd->auth_data && !ifmgd->assoc_data))
		return -EINVAL;

	if (assoc) {
		rcu_read_lock();
		have_sta = sta_info_get(sdata, cbss->bssid);
		rcu_read_unlock();
	}

	if (!have_sta) {
<<<<<<< HEAD
		sta = sta_info_alloc(sdata, cbss->bssid, GFP_KERNEL);
		if (!sta)
			return -ENOMEM;
	}

	mutex_lock(&local->mtx);
	ieee80211_recalc_idle(sdata->local);
	mutex_unlock(&local->mtx);

	/* switch to the right channel */
	local->oper_channel = cbss->channel;
	ieee80211_hw_config(local, IEEE80211_CONF_CHANGE_CHANNEL);

	if (!have_sta) {
		struct ieee80211_supported_band *sband;
		u32 rates = 0, basic_rates = 0;
		bool have_higher_than_11mbit;
		int min_rate = INT_MAX, min_rate_index = -1;

		sband = sdata->local->hw.wiphy->bands[cbss->channel->band];
=======
		new_sta = sta_info_alloc(sdata, cbss->bssid, GFP_KERNEL);
		if (!new_sta)
			return -ENOMEM;
	}
	if (new_sta) {
		u32 rates = 0, basic_rates = 0;
		bool have_higher_than_11mbit;
		int min_rate = INT_MAX, min_rate_index = -1;
		struct ieee80211_chanctx_conf *chanctx_conf;
		struct ieee80211_supported_band *sband;
		const struct cfg80211_bss_ies *ies;
		int shift;
		u32 rate_flags;

		sband = local->hw.wiphy->bands[cbss->channel->band];

		err = ieee80211_prep_channel(sdata, cbss);
		if (err) {
			sta_info_free(local, new_sta);
			return -EINVAL;
		}
		shift = ieee80211_vif_get_shift(&sdata->vif);

		rcu_read_lock();
		chanctx_conf = rcu_dereference(sdata->vif.chanctx_conf);
		if (WARN_ON(!chanctx_conf)) {
			rcu_read_unlock();
			return -EINVAL;
		}
		rate_flags = ieee80211_chandef_rate_flags(&chanctx_conf->def);
		rcu_read_unlock();
>>>>>>> refs/remotes/origin/master

		ieee80211_get_rates(sband, bss->supp_rates,
				    bss->supp_rates_len,
				    &rates, &basic_rates,
				    &have_higher_than_11mbit,
<<<<<<< HEAD
				    &min_rate, &min_rate_index);
=======
				    &min_rate, &min_rate_index,
				    shift, rate_flags);
>>>>>>> refs/remotes/origin/master

		/*
		 * This used to be a workaround for basic rates missing
		 * in the association response frame. Now that we no
		 * longer use the basic rates from there, it probably
		 * doesn't happen any more, but keep the workaround so
		 * in case some *other* APs are buggy in different ways
		 * we can connect -- with a warning.
		 */
		if (!basic_rates && min_rate_index >= 0) {
<<<<<<< HEAD
			printk(KERN_DEBUG
			       "%s: No basic rates, using min rate instead.\n",
			       sdata->name);
			basic_rates = BIT(min_rate_index);
		}

		sta->sta.supp_rates[cbss->channel->band] = rates;
		sdata->vif.bss_conf.basic_rates = basic_rates;

		/* cf. IEEE 802.11 9.2.12 */
		if (local->oper_channel->band == IEEE80211_BAND_2GHZ &&
=======
			sdata_info(sdata,
				   "No basic rates, using min rate instead\n");
			basic_rates = BIT(min_rate_index);
		}

		new_sta->sta.supp_rates[cbss->channel->band] = rates;
		sdata->vif.bss_conf.basic_rates = basic_rates;

		/* cf. IEEE 802.11 9.2.12 */
		if (cbss->channel->band == IEEE80211_BAND_2GHZ &&
>>>>>>> refs/remotes/origin/master
		    have_higher_than_11mbit)
			sdata->flags |= IEEE80211_SDATA_OPERATING_GMODE;
		else
			sdata->flags &= ~IEEE80211_SDATA_OPERATING_GMODE;

		memcpy(ifmgd->bssid, cbss->bssid, ETH_ALEN);

<<<<<<< HEAD
		/* tell driver about BSSID and basic rates */
		ieee80211_bss_info_change_notify(sdata,
			BSS_CHANGED_BSSID | BSS_CHANGED_BASIC_RATES);

		if (assoc)
			sta_info_pre_move_state(sta, IEEE80211_STA_AUTH);

		err = sta_info_insert(sta);
		sta = NULL;
		if (err) {
			printk(KERN_DEBUG
			       "%s: failed to insert STA entry for the AP (error %d)\n",
			       sdata->name, err);
			return err;
		}
	} else
		WARN_ON_ONCE(compare_ether_addr(ifmgd->bssid, cbss->bssid));
=======
		/* set timing information */
		sdata->vif.bss_conf.beacon_int = cbss->beacon_interval;
		rcu_read_lock();
		ies = rcu_dereference(cbss->beacon_ies);
		if (ies) {
			const u8 *tim_ie;

			sdata->vif.bss_conf.sync_tsf = ies->tsf;
			sdata->vif.bss_conf.sync_device_ts =
				bss->device_ts_beacon;
			tim_ie = cfg80211_find_ie(WLAN_EID_TIM,
						  ies->data, ies->len);
			if (tim_ie && tim_ie[1] >= 2)
				sdata->vif.bss_conf.sync_dtim_count = tim_ie[2];
			else
				sdata->vif.bss_conf.sync_dtim_count = 0;
		} else if (!(local->hw.flags &
					IEEE80211_HW_TIMING_BEACON_ONLY)) {
			ies = rcu_dereference(cbss->proberesp_ies);
			/* must be non-NULL since beacon IEs were NULL */
			sdata->vif.bss_conf.sync_tsf = ies->tsf;
			sdata->vif.bss_conf.sync_device_ts =
				bss->device_ts_presp;
			sdata->vif.bss_conf.sync_dtim_count = 0;
		} else {
			sdata->vif.bss_conf.sync_tsf = 0;
			sdata->vif.bss_conf.sync_device_ts = 0;
			sdata->vif.bss_conf.sync_dtim_count = 0;
		}
		rcu_read_unlock();

		/* tell driver about BSSID, basic rates and timing */
		ieee80211_bss_info_change_notify(sdata,
			BSS_CHANGED_BSSID | BSS_CHANGED_BASIC_RATES |
			BSS_CHANGED_BEACON_INT);

		if (assoc)
			sta_info_pre_move_state(new_sta, IEEE80211_STA_AUTH);

		err = sta_info_insert(new_sta);
		new_sta = NULL;
		if (err) {
			sdata_info(sdata,
				   "failed to insert STA entry for the AP (error %d)\n",
				   err);
			return err;
		}
	} else
		WARN_ON_ONCE(!ether_addr_equal(ifmgd->bssid, cbss->bssid));
>>>>>>> refs/remotes/origin/master

	return 0;
}

/* config hooks */
int ieee80211_mgd_auth(struct ieee80211_sub_if_data *sdata,
		       struct cfg80211_auth_request *req)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct ieee80211_mgd_auth_data *auth_data;
	u16 auth_alg;
	int err;

	/* prepare auth data structure */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	switch (req->auth_type) {
	case NL80211_AUTHTYPE_OPEN_SYSTEM:
		auth_alg = WLAN_AUTH_OPEN;
		break;
	case NL80211_AUTHTYPE_SHARED_KEY:
<<<<<<< HEAD
<<<<<<< HEAD
		if (IS_ERR(sdata->local->wep_tx_tfm))
=======
		if (IS_ERR(local->wep_tx_tfm))
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (IS_ERR(local->wep_tx_tfm))
>>>>>>> refs/remotes/origin/master
			return -EOPNOTSUPP;
		auth_alg = WLAN_AUTH_SHARED_KEY;
		break;
	case NL80211_AUTHTYPE_FT:
		auth_alg = WLAN_AUTH_FT;
		break;
	case NL80211_AUTHTYPE_NETWORK_EAP:
		auth_alg = WLAN_AUTH_LEAP;
		break;
<<<<<<< HEAD
=======
	case NL80211_AUTHTYPE_SAE:
		auth_alg = WLAN_AUTH_SAE;
		break;
>>>>>>> refs/remotes/origin/master
	default:
		return -EOPNOTSUPP;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	wk = kzalloc(sizeof(*wk) + req->ie_len, GFP_KERNEL);
	if (!wk)
		return -ENOMEM;

	memcpy(wk->filter_ta, req->bss->bssid, ETH_ALEN);

	if (req->ie && req->ie_len) {
		memcpy(wk->ie, req->ie, req->ie_len);
		wk->ie_len = req->ie_len;
	}

	if (req->key && req->key_len) {
		wk->probe_auth.key_len = req->key_len;
		wk->probe_auth.key_idx = req->key_idx;
		memcpy(wk->probe_auth.key, req->key, req->key_len);
	}

	ssid = ieee80211_bss_get_ie(req->bss, WLAN_EID_SSID);
	memcpy(wk->probe_auth.ssid, ssid + 2, ssid[1]);
	wk->probe_auth.ssid_len = ssid[1];

	wk->probe_auth.algorithm = auth_alg;
	wk->probe_auth.privacy = req->bss->capability & WLAN_CAPABILITY_PRIVACY;

	/* if we already have a probe, don't probe again */
	if (req->bss->proberesp_ies)
		wk->type = IEEE80211_WORK_AUTH;
	else
		wk->type = IEEE80211_WORK_DIRECT_PROBE;
	wk->chan = req->bss->channel;
	wk->chan_type = NL80211_CHAN_NO_HT;
	wk->sdata = sdata;
	wk->done = ieee80211_probe_auth_done;

	ieee80211_add_work(wk);
	return 0;
}

static enum work_done_result ieee80211_assoc_done(struct ieee80211_work *wk,
						  struct sk_buff *skb)
{
	struct ieee80211_mgmt *mgmt;
	struct ieee80211_rx_status *rx_status;
	struct ieee802_11_elems elems;
	u16 status;

	if (!skb) {
		cfg80211_send_assoc_timeout(wk->sdata->dev, wk->filter_ta);
		return WORK_DONE_DESTROY;
	}

	if (wk->type == IEEE80211_WORK_ASSOC_BEACON_WAIT) {
		mutex_lock(&wk->sdata->u.mgd.mtx);
		rx_status = (void *) skb->cb;
		ieee802_11_parse_elems(skb->data + 24 + 12, skb->len - 24 - 12, &elems);
		ieee80211_rx_bss_info(wk->sdata, (void *)skb->data, skb->len, rx_status,
				      &elems, true);
		mutex_unlock(&wk->sdata->u.mgd.mtx);

		wk->type = IEEE80211_WORK_ASSOC;
		/* not really done yet */
		return WORK_DONE_REQUEUE;
	}

	mgmt = (void *)skb->data;
	status = le16_to_cpu(mgmt->u.assoc_resp.status_code);

	if (status == WLAN_STATUS_SUCCESS) {
		mutex_lock(&wk->sdata->u.mgd.mtx);
		if (!ieee80211_assoc_success(wk, mgmt, skb->len)) {
			mutex_unlock(&wk->sdata->u.mgd.mtx);
			/* oops -- internal error -- send timeout for now */
			cfg80211_send_assoc_timeout(wk->sdata->dev,
						    wk->filter_ta);
			return WORK_DONE_DESTROY;
		}

		mutex_unlock(&wk->sdata->u.mgd.mtx);
	}

	cfg80211_send_rx_assoc(wk->sdata->dev, skb->data, skb->len);
	return WORK_DONE_DESTROY;
=======
	auth_data = kzalloc(sizeof(*auth_data) + req->ie_len, GFP_KERNEL);
=======
	auth_data = kzalloc(sizeof(*auth_data) + req->sae_data_len +
			    req->ie_len, GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (!auth_data)
		return -ENOMEM;

	auth_data->bss = req->bss;

<<<<<<< HEAD
	if (req->ie && req->ie_len) {
		memcpy(auth_data->ie, req->ie, req->ie_len);
		auth_data->ie_len = req->ie_len;
=======
	if (req->sae_data_len >= 4) {
		__le16 *pos = (__le16 *) req->sae_data;
		auth_data->sae_trans = le16_to_cpu(pos[0]);
		auth_data->sae_status = le16_to_cpu(pos[1]);
		memcpy(auth_data->data, req->sae_data + 4,
		       req->sae_data_len - 4);
		auth_data->data_len += req->sae_data_len - 4;
	}

	if (req->ie && req->ie_len) {
		memcpy(&auth_data->data[auth_data->data_len],
		       req->ie, req->ie_len);
		auth_data->data_len += req->ie_len;
>>>>>>> refs/remotes/origin/master
	}

	if (req->key && req->key_len) {
		auth_data->key_len = req->key_len;
		auth_data->key_idx = req->key_idx;
		memcpy(auth_data->key, req->key, req->key_len);
	}

	auth_data->algorithm = auth_alg;

	/* try to authenticate/probe */

<<<<<<< HEAD
	mutex_lock(&ifmgd->mtx);

=======
>>>>>>> refs/remotes/origin/master
	if ((ifmgd->auth_data && !ifmgd->auth_data->done) ||
	    ifmgd->assoc_data) {
		err = -EBUSY;
		goto err_free;
	}

	if (ifmgd->auth_data)
		ieee80211_destroy_auth_data(sdata, false);

	/* prep auth_data so we don't go into idle on disassoc */
	ifmgd->auth_data = auth_data;

<<<<<<< HEAD
	if (ifmgd->associated)
		ieee80211_set_disassoc(sdata, 0, 0, false, NULL);

	printk(KERN_DEBUG "%s: authenticate with %pM\n",
	       sdata->name, req->bss->bssid);
=======
	if (ifmgd->associated) {
		u8 frame_buf[IEEE80211_DEAUTH_FRAME_LEN];

		ieee80211_set_disassoc(sdata, IEEE80211_STYPE_DEAUTH,
				       WLAN_REASON_UNSPECIFIED,
				       false, frame_buf);

		cfg80211_tx_mlme_mgmt(sdata->dev, frame_buf,
				      sizeof(frame_buf));
	}

	sdata_info(sdata, "authenticate with %pM\n", req->bss->bssid);
>>>>>>> refs/remotes/origin/master

	err = ieee80211_prep_connection(sdata, req->bss, false);
	if (err)
		goto err_clear;

	err = ieee80211_probe_auth(sdata);
	if (err) {
		sta_info_destroy_addr(sdata, req->bss->bssid);
		goto err_clear;
	}

	/* hold our own reference */
<<<<<<< HEAD
	cfg80211_ref_bss(auth_data->bss);
	err = 0;
	goto out_unlock;
=======
	cfg80211_ref_bss(local->hw.wiphy, auth_data->bss);
	return 0;
>>>>>>> refs/remotes/origin/master

 err_clear:
	memset(ifmgd->bssid, 0, ETH_ALEN);
	ieee80211_bss_info_change_notify(sdata, BSS_CHANGED_BSSID);
	ifmgd->auth_data = NULL;
 err_free:
	kfree(auth_data);
<<<<<<< HEAD
 out_unlock:
	mutex_unlock(&ifmgd->mtx);

	return err;
>>>>>>> refs/remotes/origin/cm-10.0
}

int ieee80211_mgd_assoc(struct ieee80211_sub_if_data *sdata,
			struct cfg80211_assoc_request *req)
{
<<<<<<< HEAD
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct ieee80211_bss *bss = (void *)req->bss->priv;
	struct ieee80211_work *wk;
	const u8 *ssid;
	int i;

	mutex_lock(&ifmgd->mtx);
	if (ifmgd->associated) {
		if (!req->prev_bssid ||
		    memcmp(req->prev_bssid, ifmgd->associated->bssid,
			   ETH_ALEN)) {
			/*
			 * We are already associated and the request was not a
			 * reassociation request from the current BSS, so
			 * reject it.
			 */
			mutex_unlock(&ifmgd->mtx);
			return -EALREADY;
		}

		/* Trying to reassociate - clear previous association state */
		ieee80211_set_disassoc(sdata, true, false);
	}
	mutex_unlock(&ifmgd->mtx);

	wk = kzalloc(sizeof(*wk) + req->ie_len, GFP_KERNEL);
	if (!wk)
		return -ENOMEM;
=======
=======
	return err;
}

static bool ieee80211_usable_wmm_params(struct ieee80211_sub_if_data *sdata,
					const u8 *wmm_param, int len)
{
	const u8 *pos;
	size_t left;

	if (len < 8)
		return false;

	if (wmm_param[5] != 1 /* version */)
		return false;

	pos = wmm_param + 8;
	left = len - 8;

	for (; left >= 4; left -= 4, pos += 4) {
		u8 aifsn = pos[0] & 0x0f;
		u8 ecwmin = pos[1] & 0x0f;
		u8 ecwmax = (pos[1] & 0xf0) >> 4;
		int aci = (pos[0] >> 5) & 0x03;

		if (aifsn < 2) {
			sdata_info(sdata,
				   "AP has invalid WMM params (AIFSN=%d for ACI %d), disabling WMM\n",
				   aifsn, aci);
			return false;
		}
		if (ecwmin > ecwmax) {
			sdata_info(sdata,
				   "AP has invalid WMM params (ECWmin/max=%d/%d for ACI %d), disabling WMM\n",
				   ecwmin, ecwmax, aci);
			return false;
		}
	}

	return true;
}

int ieee80211_mgd_assoc(struct ieee80211_sub_if_data *sdata,
			struct cfg80211_assoc_request *req)
{
>>>>>>> refs/remotes/origin/master
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct ieee80211_bss *bss = (void *)req->bss->priv;
	struct ieee80211_mgd_assoc_data *assoc_data;
<<<<<<< HEAD
	struct ieee80211_supported_band *sband;
	const u8 *ssidie;
	int i, err;

	ssidie = ieee80211_bss_get_ie(req->bss, WLAN_EID_SSID);
	if (!ssidie)
		return -EINVAL;

=======
	const struct cfg80211_bss_ies *beacon_ies;
	struct ieee80211_supported_band *sband;
	const u8 *ssidie, *ht_ie, *vht_ie;
	int i, err;

>>>>>>> refs/remotes/origin/master
	assoc_data = kzalloc(sizeof(*assoc_data) + req->ie_len, GFP_KERNEL);
	if (!assoc_data)
		return -ENOMEM;

<<<<<<< HEAD
	mutex_lock(&ifmgd->mtx);

	if (ifmgd->associated)
		ieee80211_set_disassoc(sdata, 0, 0, false, NULL);
=======
	rcu_read_lock();
	ssidie = ieee80211_bss_get_ie(req->bss, WLAN_EID_SSID);
	if (!ssidie) {
		rcu_read_unlock();
		kfree(assoc_data);
		return -EINVAL;
	}
	memcpy(assoc_data->ssid, ssidie + 2, ssidie[1]);
	assoc_data->ssid_len = ssidie[1];
	rcu_read_unlock();

	if (ifmgd->associated) {
		u8 frame_buf[IEEE80211_DEAUTH_FRAME_LEN];

		ieee80211_set_disassoc(sdata, IEEE80211_STYPE_DEAUTH,
				       WLAN_REASON_UNSPECIFIED,
				       false, frame_buf);

		cfg80211_tx_mlme_mgmt(sdata->dev, frame_buf,
				      sizeof(frame_buf));
	}
>>>>>>> refs/remotes/origin/master

	if (ifmgd->auth_data && !ifmgd->auth_data->done) {
		err = -EBUSY;
		goto err_free;
	}

	if (ifmgd->assoc_data) {
		err = -EBUSY;
		goto err_free;
	}

	if (ifmgd->auth_data) {
		bool match;

		/* keep sta info, bssid if matching */
<<<<<<< HEAD
		match = compare_ether_addr(ifmgd->bssid, req->bss->bssid) == 0;
=======
		match = ether_addr_equal(ifmgd->bssid, req->bss->bssid);
>>>>>>> refs/remotes/origin/master
		ieee80211_destroy_auth_data(sdata, match);
	}

	/* prepare assoc data */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

	ifmgd->flags &= ~IEEE80211_STA_DISABLE_11N;
	ifmgd->flags &= ~IEEE80211_STA_NULLFUNC_ACKED;

	ifmgd->beacon_crc_valid = false;

<<<<<<< HEAD
=======
=======

	ifmgd->beacon_crc_valid = false;

	assoc_data->wmm = bss->wmm_used &&
			  (local->hw.queues >= IEEE80211_NUM_ACS);
	if (assoc_data->wmm) {
		/* try to check validity of WMM params IE */
		const struct cfg80211_bss_ies *ies;
		const u8 *wp, *start, *end;

		rcu_read_lock();
		ies = rcu_dereference(req->bss->ies);
		start = ies->data;
		end = start + ies->len;

		while (true) {
			wp = cfg80211_find_vendor_ie(
				WLAN_OUI_MICROSOFT,
				WLAN_OUI_TYPE_MICROSOFT_WMM,
				start, end - start);
			if (!wp)
				break;
			start = wp + wp[1] + 2;
			/* if this IE is too short, try the next */
			if (wp[1] <= 4)
				continue;
			/* if this IE is WMM params, we found what we wanted */
			if (wp[6] == 1)
				break;
		}

		if (!wp || !ieee80211_usable_wmm_params(sdata, wp + 2,
							wp[1] - 2)) {
			assoc_data->wmm = false;
			ifmgd->flags |= IEEE80211_STA_DISABLE_WMM;
		}
		rcu_read_unlock();
	}

>>>>>>> refs/remotes/origin/master
	/*
	 * IEEE802.11n does not allow TKIP/WEP as pairwise ciphers in HT mode.
	 * We still associate in non-HT mode (11a/b/g) if any one of these
	 * ciphers is configured as pairwise.
	 * We can set this to true for non-11n hardware, that'll be checked
	 * separately along with the peer capabilities.
	 */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	for (i = 0; i < req->crypto.n_ciphers_pairwise; i++)
		if (req->crypto.ciphers_pairwise[i] == WLAN_CIPHER_SUITE_WEP40 ||
		    req->crypto.ciphers_pairwise[i] == WLAN_CIPHER_SUITE_TKIP ||
		    req->crypto.ciphers_pairwise[i] == WLAN_CIPHER_SUITE_WEP104)
			ifmgd->flags |= IEEE80211_STA_DISABLE_11N;

<<<<<<< HEAD

	if (req->ie && req->ie_len) {
		memcpy(wk->ie, req->ie, req->ie_len);
		wk->ie_len = req->ie_len;
	} else
		wk->ie_len = 0;

	wk->assoc.bss = req->bss;

	memcpy(wk->filter_ta, req->bss->bssid, ETH_ALEN);

	/* new association always uses requested smps mode */
=======
	if (req->flags & ASSOC_REQ_DISABLE_HT)
		ifmgd->flags |= IEEE80211_STA_DISABLE_11N;
=======
	for (i = 0; i < req->crypto.n_ciphers_pairwise; i++) {
		if (req->crypto.ciphers_pairwise[i] == WLAN_CIPHER_SUITE_WEP40 ||
		    req->crypto.ciphers_pairwise[i] == WLAN_CIPHER_SUITE_TKIP ||
		    req->crypto.ciphers_pairwise[i] == WLAN_CIPHER_SUITE_WEP104) {
			ifmgd->flags |= IEEE80211_STA_DISABLE_HT;
			ifmgd->flags |= IEEE80211_STA_DISABLE_VHT;
			netdev_info(sdata->dev,
				    "disabling HT/VHT due to WEP/TKIP use\n");
		}
	}

	if (req->flags & ASSOC_REQ_DISABLE_HT) {
		ifmgd->flags |= IEEE80211_STA_DISABLE_HT;
		ifmgd->flags |= IEEE80211_STA_DISABLE_VHT;
	}

	if (req->flags & ASSOC_REQ_DISABLE_VHT)
		ifmgd->flags |= IEEE80211_STA_DISABLE_VHT;
>>>>>>> refs/remotes/origin/master

	/* Also disable HT if we don't support it or the AP doesn't use WMM */
	sband = local->hw.wiphy->bands[req->bss->channel->band];
	if (!sband->ht_cap.ht_supported ||
<<<<<<< HEAD
	    local->hw.queues < 4 || !bss->wmm_used)
		ifmgd->flags |= IEEE80211_STA_DISABLE_11N;
=======
	    local->hw.queues < IEEE80211_NUM_ACS || !bss->wmm_used ||
	    ifmgd->flags & IEEE80211_STA_DISABLE_WMM) {
		ifmgd->flags |= IEEE80211_STA_DISABLE_HT;
		if (!bss->wmm_used &&
		    !(ifmgd->flags & IEEE80211_STA_DISABLE_WMM))
			netdev_info(sdata->dev,
				    "disabling HT as WMM/QoS is not supported by the AP\n");
	}

	/* disable VHT if we don't support it or the AP doesn't use WMM */
	if (!sband->vht_cap.vht_supported ||
	    local->hw.queues < IEEE80211_NUM_ACS || !bss->wmm_used ||
	    ifmgd->flags & IEEE80211_STA_DISABLE_WMM) {
		ifmgd->flags |= IEEE80211_STA_DISABLE_VHT;
		if (!bss->wmm_used &&
		    !(ifmgd->flags & IEEE80211_STA_DISABLE_WMM))
			netdev_info(sdata->dev,
				    "disabling VHT as WMM/QoS is not supported by the AP\n");
	}
>>>>>>> refs/remotes/origin/master

	memcpy(&ifmgd->ht_capa, &req->ht_capa, sizeof(ifmgd->ht_capa));
	memcpy(&ifmgd->ht_capa_mask, &req->ht_capa_mask,
	       sizeof(ifmgd->ht_capa_mask));

<<<<<<< HEAD
=======
	memcpy(&ifmgd->vht_capa, &req->vht_capa, sizeof(ifmgd->vht_capa));
	memcpy(&ifmgd->vht_capa_mask, &req->vht_capa_mask,
	       sizeof(ifmgd->vht_capa_mask));

>>>>>>> refs/remotes/origin/master
	if (req->ie && req->ie_len) {
		memcpy(assoc_data->ie, req->ie, req->ie_len);
		assoc_data->ie_len = req->ie_len;
	}

	assoc_data->bss = req->bss;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	if (ifmgd->req_smps == IEEE80211_SMPS_AUTOMATIC) {
		if (ifmgd->powersave)
			ifmgd->ap_smps = IEEE80211_SMPS_DYNAMIC;
		else
			ifmgd->ap_smps = IEEE80211_SMPS_OFF;
	} else
		ifmgd->ap_smps = ifmgd->req_smps;

<<<<<<< HEAD
	wk->assoc.smps = ifmgd->ap_smps;
	/*
	 * IEEE802.11n does not allow TKIP/WEP as pairwise ciphers in HT mode.
	 * We still associate in non-HT mode (11a/b/g) if any one of these
	 * ciphers is configured as pairwise.
	 * We can set this to true for non-11n hardware, that'll be checked
	 * separately along with the peer capabilities.
	 */
	wk->assoc.use_11n = !(ifmgd->flags & IEEE80211_STA_DISABLE_11N);
	wk->assoc.capability = req->bss->capability;
	wk->assoc.wmm_used = bss->wmm_used;
	wk->assoc.supp_rates = bss->supp_rates;
	wk->assoc.supp_rates_len = bss->supp_rates_len;
	wk->assoc.ht_information_ie =
=======
	assoc_data->capability = req->bss->capability;
	assoc_data->wmm = bss->wmm_used && (local->hw.queues >= 4);
	assoc_data->supp_rates = bss->supp_rates;
	assoc_data->supp_rates_len = bss->supp_rates_len;
	assoc_data->ht_information_ie =
>>>>>>> refs/remotes/origin/cm-10.0
		ieee80211_bss_get_ie(req->bss, WLAN_EID_HT_INFORMATION);

	if (bss->wmm_used && bss->uapsd_supported &&
	    (sdata->local->hw.flags & IEEE80211_HW_SUPPORTS_UAPSD)) {
<<<<<<< HEAD
		wk->assoc.uapsd_used = true;
		ifmgd->flags |= IEEE80211_STA_UAPSD_ENABLED;
	} else {
		wk->assoc.uapsd_used = false;
		ifmgd->flags &= ~IEEE80211_STA_UAPSD_ENABLED;
	}

	ssid = ieee80211_bss_get_ie(req->bss, WLAN_EID_SSID);
	memcpy(wk->assoc.ssid, ssid + 2, ssid[1]);
	wk->assoc.ssid_len = ssid[1];

	if (req->prev_bssid)
		memcpy(wk->assoc.prev_bssid, req->prev_bssid, ETH_ALEN);

	wk->chan = req->bss->channel;
	wk->chan_type = NL80211_CHAN_NO_HT;
	wk->sdata = sdata;
	wk->done = ieee80211_assoc_done;
	if (!bss->dtim_period &&
	    sdata->local->hw.flags & IEEE80211_HW_NEED_DTIM_PERIOD)
		wk->type = IEEE80211_WORK_ASSOC_BEACON_WAIT;
	else
		wk->type = IEEE80211_WORK_ASSOC;
=======
=======
	if (ifmgd->req_smps == IEEE80211_SMPS_AUTOMATIC) {
		if (ifmgd->powersave)
			sdata->smps_mode = IEEE80211_SMPS_DYNAMIC;
		else
			sdata->smps_mode = IEEE80211_SMPS_OFF;
	} else
		sdata->smps_mode = ifmgd->req_smps;

	assoc_data->capability = req->bss->capability;
	assoc_data->supp_rates = bss->supp_rates;
	assoc_data->supp_rates_len = bss->supp_rates_len;

	rcu_read_lock();
	ht_ie = ieee80211_bss_get_ie(req->bss, WLAN_EID_HT_OPERATION);
	if (ht_ie && ht_ie[1] >= sizeof(struct ieee80211_ht_operation))
		assoc_data->ap_ht_param =
			((struct ieee80211_ht_operation *)(ht_ie + 2))->ht_param;
	else
		ifmgd->flags |= IEEE80211_STA_DISABLE_HT;
	vht_ie = ieee80211_bss_get_ie(req->bss, WLAN_EID_VHT_CAPABILITY);
	if (vht_ie && vht_ie[1] >= sizeof(struct ieee80211_vht_cap))
		memcpy(&assoc_data->ap_vht_cap, vht_ie + 2,
		       sizeof(struct ieee80211_vht_cap));
	else
		ifmgd->flags |= IEEE80211_STA_DISABLE_VHT;
	rcu_read_unlock();

	if (bss->wmm_used && bss->uapsd_supported &&
	    (sdata->local->hw.flags & IEEE80211_HW_SUPPORTS_UAPSD) &&
	    sdata->wmm_acm != 0xff) {
>>>>>>> refs/remotes/origin/master
		assoc_data->uapsd = true;
		ifmgd->flags |= IEEE80211_STA_UAPSD_ENABLED;
	} else {
		assoc_data->uapsd = false;
		ifmgd->flags &= ~IEEE80211_STA_UAPSD_ENABLED;
	}

<<<<<<< HEAD
	memcpy(assoc_data->ssid, ssidie + 2, ssidie[1]);
	assoc_data->ssid_len = ssidie[1];

	if (req->prev_bssid)
		memcpy(assoc_data->prev_bssid, req->prev_bssid, ETH_ALEN);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (req->prev_bssid)
		memcpy(assoc_data->prev_bssid, req->prev_bssid, ETH_ALEN);
>>>>>>> refs/remotes/origin/master

	if (req->use_mfp) {
		ifmgd->mfp = IEEE80211_MFP_REQUIRED;
		ifmgd->flags |= IEEE80211_STA_MFP_ENABLED;
	} else {
		ifmgd->mfp = IEEE80211_MFP_DISABLED;
		ifmgd->flags &= ~IEEE80211_STA_MFP_ENABLED;
	}

	if (req->crypto.control_port)
		ifmgd->flags |= IEEE80211_STA_CONTROL_PORT;
	else
		ifmgd->flags &= ~IEEE80211_STA_CONTROL_PORT;

	sdata->control_port_protocol = req->crypto.control_port_ethertype;
	sdata->control_port_no_encrypt = req->crypto.control_port_no_encrypt;

<<<<<<< HEAD
<<<<<<< HEAD
	ieee80211_add_work(wk);
	return 0;
}

int ieee80211_mgd_deauth(struct ieee80211_sub_if_data *sdata,
			 struct cfg80211_deauth_request *req,
			 void *cookie)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	struct ieee80211_work *wk;
	u8 bssid[ETH_ALEN];
	bool assoc_bss = false;

	mutex_lock(&ifmgd->mtx);

	memcpy(bssid, req->bss->bssid, ETH_ALEN);
	if (ifmgd->associated == req->bss) {
		ieee80211_set_disassoc(sdata, false, true);
		mutex_unlock(&ifmgd->mtx);
		assoc_bss = true;
	} else {
		bool not_auth_yet = false;

		mutex_unlock(&ifmgd->mtx);

		mutex_lock(&local->mtx);
		list_for_each_entry(wk, &local->work_list, list) {
			if (wk->sdata != sdata)
				continue;

			if (wk->type != IEEE80211_WORK_DIRECT_PROBE &&
			    wk->type != IEEE80211_WORK_AUTH &&
			    wk->type != IEEE80211_WORK_ASSOC &&
			    wk->type != IEEE80211_WORK_ASSOC_BEACON_WAIT)
				continue;

			if (memcmp(req->bss->bssid, wk->filter_ta, ETH_ALEN))
				continue;

			not_auth_yet = wk->type == IEEE80211_WORK_DIRECT_PROBE;
			list_del_rcu(&wk->list);
			free_work(wk);
			break;
		}
		mutex_unlock(&local->mtx);

		/*
		 * If somebody requests authentication and we haven't
		 * sent out an auth frame yet there's no need to send
		 * out a deauth frame either. If the state was PROBE,
		 * then this is the case. If it's AUTH we have sent a
		 * frame, and if it's IDLE we have completed the auth
		 * process already.
		 */
		if (not_auth_yet) {
			__cfg80211_auth_canceled(sdata->dev, bssid);
			return 0;
		}
	}

	printk(KERN_DEBUG "%s: deauthenticating from %pM by local choice (reason=%d)\n",
	       sdata->name, bssid, req->reason_code);

	ieee80211_send_deauth_disassoc(sdata, bssid, IEEE80211_STYPE_DEAUTH,
				       req->reason_code, cookie,
				       !req->local_state_change);
	if (assoc_bss)
		sta_info_destroy_addr(sdata, bssid);
=======
	/* kick off associate process */

	ifmgd->assoc_data = assoc_data;
=======
	/* kick off associate process */

	ifmgd->assoc_data = assoc_data;
	ifmgd->dtim_period = 0;
	ifmgd->have_beacon = false;
>>>>>>> refs/remotes/origin/master

	err = ieee80211_prep_connection(sdata, req->bss, true);
	if (err)
		goto err_clear;

<<<<<<< HEAD
	if (!bss->dtim_period &&
	    sdata->local->hw.flags & IEEE80211_HW_NEED_DTIM_PERIOD) {
=======
	rcu_read_lock();
	beacon_ies = rcu_dereference(req->bss->beacon_ies);

	if (sdata->local->hw.flags & IEEE80211_HW_NEED_DTIM_BEFORE_ASSOC &&
	    !beacon_ies) {
>>>>>>> refs/remotes/origin/master
		/*
		 * Wait up to one beacon interval ...
		 * should this be more if we miss one?
		 */
<<<<<<< HEAD
		printk(KERN_DEBUG "%s: waiting for beacon from %pM\n",
		       sdata->name, ifmgd->bssid);
		assoc_data->timeout = TU_TO_EXP_TIME(req->bss->beacon_interval);
	} else {
		assoc_data->have_beacon = true;
		assoc_data->sent_assoc = false;
		assoc_data->timeout = jiffies;
	}
	run_again(ifmgd, assoc_data->timeout);
=======
		sdata_info(sdata, "waiting for beacon from %pM\n",
			   ifmgd->bssid);
		assoc_data->timeout = TU_TO_EXP_TIME(req->bss->beacon_interval);
		assoc_data->timeout_started = true;
		assoc_data->need_beacon = true;
	} else if (beacon_ies) {
		const u8 *tim_ie = cfg80211_find_ie(WLAN_EID_TIM,
						    beacon_ies->data,
						    beacon_ies->len);
		u8 dtim_count = 0;

		if (tim_ie && tim_ie[1] >= sizeof(struct ieee80211_tim_ie)) {
			const struct ieee80211_tim_ie *tim;
			tim = (void *)(tim_ie + 2);
			ifmgd->dtim_period = tim->dtim_period;
			dtim_count = tim->dtim_count;
		}
		ifmgd->have_beacon = true;
		assoc_data->timeout = jiffies;
		assoc_data->timeout_started = true;

		if (local->hw.flags & IEEE80211_HW_TIMING_BEACON_ONLY) {
			sdata->vif.bss_conf.sync_tsf = beacon_ies->tsf;
			sdata->vif.bss_conf.sync_device_ts =
				bss->device_ts_beacon;
			sdata->vif.bss_conf.sync_dtim_count = dtim_count;
		}
	} else {
		assoc_data->timeout = jiffies;
		assoc_data->timeout_started = true;
	}
	rcu_read_unlock();

	run_again(sdata, assoc_data->timeout);
>>>>>>> refs/remotes/origin/master

	if (bss->corrupt_data) {
		char *corrupt_type = "data";
		if (bss->corrupt_data & IEEE80211_BSS_CORRUPT_BEACON) {
			if (bss->corrupt_data &
					IEEE80211_BSS_CORRUPT_PROBE_RESP)
				corrupt_type = "beacon and probe response";
			else
				corrupt_type = "beacon";
		} else if (bss->corrupt_data & IEEE80211_BSS_CORRUPT_PROBE_RESP)
			corrupt_type = "probe response";
<<<<<<< HEAD
		printk(KERN_DEBUG "%s: associating with AP with corrupt %s\n",
		       sdata->name, corrupt_type);
	}

	err = 0;
	goto out;
=======
		sdata_info(sdata, "associating with AP with corrupt %s\n",
			   corrupt_type);
	}

	return 0;
>>>>>>> refs/remotes/origin/master
 err_clear:
	memset(ifmgd->bssid, 0, ETH_ALEN);
	ieee80211_bss_info_change_notify(sdata, BSS_CHANGED_BSSID);
	ifmgd->assoc_data = NULL;
 err_free:
	kfree(assoc_data);
<<<<<<< HEAD
 out:
	mutex_unlock(&ifmgd->mtx);

=======
>>>>>>> refs/remotes/origin/master
	return err;
}

int ieee80211_mgd_deauth(struct ieee80211_sub_if_data *sdata,
			 struct cfg80211_deauth_request *req)
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
<<<<<<< HEAD
	u8 frame_buf[DEAUTH_DISASSOC_LEN];

	mutex_lock(&ifmgd->mtx);

	if (ifmgd->auth_data) {
		ieee80211_destroy_auth_data(sdata, false);
		mutex_unlock(&ifmgd->mtx);
		return 0;
	}

	printk(KERN_DEBUG
	       "%s: deauthenticating from %pM by local choice (reason=%d)\n",
	       sdata->name, req->bssid, req->reason_code);

	if (ifmgd->associated &&
	    compare_ether_addr(ifmgd->associated->bssid, req->bssid) == 0)
		ieee80211_set_disassoc(sdata, IEEE80211_STYPE_DEAUTH,
				       req->reason_code, true, frame_buf);
	else
		ieee80211_send_deauth_disassoc(sdata, req->bssid,
					       IEEE80211_STYPE_DEAUTH,
					       req->reason_code, true,
					       frame_buf);
	mutex_unlock(&ifmgd->mtx);

	__cfg80211_send_deauth(sdata->dev, frame_buf, DEAUTH_DISASSOC_LEN);
>>>>>>> refs/remotes/origin/cm-10.0

	mutex_lock(&sdata->local->mtx);
	ieee80211_recalc_idle(sdata->local);
	mutex_unlock(&sdata->local->mtx);
=======
	u8 frame_buf[IEEE80211_DEAUTH_FRAME_LEN];
	bool tx = !req->local_state_change;
	bool report_frame = false;

	sdata_info(sdata,
		   "deauthenticating from %pM by local choice (reason=%d)\n",
		   req->bssid, req->reason_code);

	if (ifmgd->auth_data) {
		drv_mgd_prepare_tx(sdata->local, sdata);
		ieee80211_send_deauth_disassoc(sdata, req->bssid,
					       IEEE80211_STYPE_DEAUTH,
					       req->reason_code, tx,
					       frame_buf);
		ieee80211_destroy_auth_data(sdata, false);

		report_frame = true;
		goto out;
	}

	if (ifmgd->associated &&
	    ether_addr_equal(ifmgd->associated->bssid, req->bssid)) {
		ieee80211_set_disassoc(sdata, IEEE80211_STYPE_DEAUTH,
				       req->reason_code, tx, frame_buf);
		report_frame = true;
	}

 out:
	if (report_frame)
		cfg80211_tx_mlme_mgmt(sdata->dev, frame_buf,
				      IEEE80211_DEAUTH_FRAME_LEN);
>>>>>>> refs/remotes/origin/master

	return 0;
}

int ieee80211_mgd_disassoc(struct ieee80211_sub_if_data *sdata,
<<<<<<< HEAD
<<<<<<< HEAD
			   struct cfg80211_disassoc_request *req,
			   void *cookie)
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	u8 bssid[ETH_ALEN];
=======
=======
>>>>>>> refs/remotes/origin/master
			   struct cfg80211_disassoc_request *req)
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;
	u8 bssid[ETH_ALEN];
<<<<<<< HEAD
	u8 frame_buf[DEAUTH_DISASSOC_LEN];
>>>>>>> refs/remotes/origin/cm-10.0

	mutex_lock(&ifmgd->mtx);
=======
	u8 frame_buf[IEEE80211_DEAUTH_FRAME_LEN];
>>>>>>> refs/remotes/origin/master

	/*
	 * cfg80211 should catch this ... but it's racy since
	 * we can receive a disassoc frame, process it, hand it
	 * to cfg80211 while that's in a locked section already
	 * trying to tell us that the user wants to disconnect.
	 */
<<<<<<< HEAD
	if (ifmgd->associated != req->bss) {
		mutex_unlock(&ifmgd->mtx);
		return -ENOLINK;
	}

	printk(KERN_DEBUG "%s: disassociating from %pM by local choice (reason=%d)\n",
	       sdata->name, req->bss->bssid, req->reason_code);

	memcpy(bssid, req->bss->bssid, ETH_ALEN);
<<<<<<< HEAD
	ieee80211_set_disassoc(sdata, false, true);

	mutex_unlock(&ifmgd->mtx);

	ieee80211_send_deauth_disassoc(sdata, req->bss->bssid,
			IEEE80211_STYPE_DISASSOC, req->reason_code,
			cookie, !req->local_state_change);
	sta_info_destroy_addr(sdata, bssid);
=======
	ieee80211_set_disassoc(sdata, IEEE80211_STYPE_DISASSOC,
			       req->reason_code, !req->local_state_change,
			       frame_buf);
	mutex_unlock(&ifmgd->mtx);

	__cfg80211_send_disassoc(sdata->dev, frame_buf, DEAUTH_DISASSOC_LEN);
>>>>>>> refs/remotes/origin/cm-10.0

	mutex_lock(&sdata->local->mtx);
	ieee80211_recalc_idle(sdata->local);
	mutex_unlock(&sdata->local->mtx);
=======
	if (ifmgd->associated != req->bss)
		return -ENOLINK;

	sdata_info(sdata,
		   "disassociating from %pM by local choice (reason=%d)\n",
		   req->bss->bssid, req->reason_code);

	memcpy(bssid, req->bss->bssid, ETH_ALEN);
	ieee80211_set_disassoc(sdata, IEEE80211_STYPE_DISASSOC,
			       req->reason_code, !req->local_state_change,
			       frame_buf);

	cfg80211_tx_mlme_mgmt(sdata->dev, frame_buf,
			      IEEE80211_DEAUTH_FRAME_LEN);
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
void ieee80211_mgd_stop(struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_if_managed *ifmgd = &sdata->u.mgd;

<<<<<<< HEAD
	mutex_lock(&ifmgd->mtx);
	if (ifmgd->assoc_data)
		ieee80211_destroy_assoc_data(sdata, false);
	if (ifmgd->auth_data)
		ieee80211_destroy_auth_data(sdata, false);
	del_timer_sync(&ifmgd->timer);
	mutex_unlock(&ifmgd->mtx);
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
	/*
	 * Make sure some work items will not run after this,
	 * they will not do anything but might not have been
	 * cancelled when disconnecting.
	 */
	cancel_work_sync(&ifmgd->monitor_work);
	cancel_work_sync(&ifmgd->beacon_connection_loss_work);
	cancel_work_sync(&ifmgd->request_smps_work);
	cancel_work_sync(&ifmgd->csa_connection_drop_work);
	cancel_work_sync(&ifmgd->chswitch_work);

	sdata_lock(sdata);
	if (ifmgd->assoc_data) {
		struct cfg80211_bss *bss = ifmgd->assoc_data->bss;
		ieee80211_destroy_assoc_data(sdata, false);
		cfg80211_assoc_timeout(sdata->dev, bss);
	}
	if (ifmgd->auth_data)
		ieee80211_destroy_auth_data(sdata, false);
	del_timer_sync(&ifmgd->timer);
	sdata_unlock(sdata);
}

>>>>>>> refs/remotes/origin/master
void ieee80211_cqm_rssi_notify(struct ieee80211_vif *vif,
			       enum nl80211_cqm_rssi_threshold_event rssi_event,
			       gfp_t gfp)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);

	trace_api_cqm_rssi_notify(sdata, rssi_event);

	cfg80211_cqm_rssi_notify(sdata->dev, rssi_event, gfp);
}
EXPORT_SYMBOL(ieee80211_cqm_rssi_notify);
<<<<<<< HEAD
<<<<<<< HEAD
=======

unsigned char ieee80211_get_operstate(struct ieee80211_vif *vif)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);
	return sdata->dev->operstate;
}
EXPORT_SYMBOL(ieee80211_get_operstate);
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
