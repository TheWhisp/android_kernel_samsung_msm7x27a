/*
 * Copyright 2002-2005, Instant802 Networks, Inc.
 * Copyright 2006-2007	Jiri Benc <jbenc@suse.cz>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/init.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/etherdevice.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/etherdevice.h>
>>>>>>> refs/remotes/origin/master
#include <linux/netdevice.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/skbuff.h>
#include <linux/if_arp.h>
#include <linux/timer.h>
#include <linux/rtnetlink.h>

#include <net/mac80211.h>
#include "ieee80211_i.h"
#include "driver-ops.h"
#include "rate.h"
#include "sta_info.h"
#include "debugfs_sta.h"
#include "mesh.h"
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include "wme.h"
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include "wme.h"
>>>>>>> refs/remotes/origin/master

/**
 * DOC: STA information lifetime rules
 *
 * STA info structures (&struct sta_info) are managed in a hash table
 * for faster lookup and a list for iteration. They are managed using
 * RCU, i.e. access to the list and hash table is protected by RCU.
 *
 * Upon allocating a STA info structure with sta_info_alloc(), the caller
 * owns that structure. It must then insert it into the hash table using
 * either sta_info_insert() or sta_info_insert_rcu(); only in the latter
 * case (which acquires an rcu read section but must not be called from
 * within one) will the pointer still be valid after the call. Note that
 * the caller may not do much with the STA info before inserting it, in
 * particular, it may not start any mesh peer link management or add
 * encryption keys.
 *
 * When the insertion fails (sta_info_insert()) returns non-zero), the
 * structure will have been freed by sta_info_insert()!
 *
 * Station entries are added by mac80211 when you establish a link with a
 * peer. This means different things for the different type of interfaces
 * we support. For a regular station this mean we add the AP sta when we
 * receive an association response from the AP. For IBSS this occurs when
 * get to know about a peer on the same IBSS. For WDS we add the sta for
 * the peer immediately upon device open. When using AP mode we add stations
 * for each respective station upon request from userspace through nl80211.
 *
 * In order to remove a STA info structure, various sta_info_destroy_*()
 * calls are available.
 *
 * There is no concept of ownership on a STA entry, each structure is
 * owned by the global hash table/list until it is removed. All users of
 * the structure need to be RCU protected so that the structure won't be
 * freed before they are done using it.
 */

<<<<<<< HEAD
<<<<<<< HEAD
/* Caller must hold local->sta_lock */
=======
/* Caller must hold local->sta_mtx */
>>>>>>> refs/remotes/origin/cm-10.0
=======
/* Caller must hold local->sta_mtx */
>>>>>>> refs/remotes/origin/master
static int sta_info_hash_del(struct ieee80211_local *local,
			     struct sta_info *sta)
{
	struct sta_info *s;

	s = rcu_dereference_protected(local->sta_hash[STA_HASH(sta->sta.addr)],
<<<<<<< HEAD
<<<<<<< HEAD
				      lockdep_is_held(&local->sta_lock));
=======
				      lockdep_is_held(&local->sta_mtx));
>>>>>>> refs/remotes/origin/cm-10.0
=======
				      lockdep_is_held(&local->sta_mtx));
>>>>>>> refs/remotes/origin/master
	if (!s)
		return -ENOENT;
	if (s == sta) {
		rcu_assign_pointer(local->sta_hash[STA_HASH(sta->sta.addr)],
				   s->hnext);
		return 0;
	}

	while (rcu_access_pointer(s->hnext) &&
	       rcu_access_pointer(s->hnext) != sta)
		s = rcu_dereference_protected(s->hnext,
<<<<<<< HEAD
<<<<<<< HEAD
					lockdep_is_held(&local->sta_lock));
=======
					lockdep_is_held(&local->sta_mtx));
>>>>>>> refs/remotes/origin/cm-10.0
=======
					lockdep_is_held(&local->sta_mtx));
>>>>>>> refs/remotes/origin/master
	if (rcu_access_pointer(s->hnext)) {
		rcu_assign_pointer(s->hnext, sta->hnext);
		return 0;
	}

	return -ENOENT;
}

<<<<<<< HEAD
=======
static void cleanup_single_sta(struct sta_info *sta)
{
	int ac, i;
	struct tid_ampdu_tx *tid_tx;
	struct ieee80211_sub_if_data *sdata = sta->sdata;
	struct ieee80211_local *local = sdata->local;
	struct ps_data *ps;

	/*
	 * At this point, when being called as call_rcu callback,
	 * neither mac80211 nor the driver can reference this
	 * sta struct any more except by still existing timers
	 * associated with this station that we clean up below.
	 *
	 * Note though that this still uses the sdata and even
	 * calls the driver in AP and mesh mode, so interfaces
	 * of those types mush use call sta_info_flush_cleanup()
	 * (typically via sta_info_flush()) before deconfiguring
	 * the driver.
	 *
	 * In station mode, nothing happens here so it doesn't
	 * have to (and doesn't) do that, this is intentional to
	 * speed up roaming.
	 */

	if (test_sta_flag(sta, WLAN_STA_PS_STA)) {
		if (sta->sdata->vif.type == NL80211_IFTYPE_AP ||
		    sta->sdata->vif.type == NL80211_IFTYPE_AP_VLAN)
			ps = &sdata->bss->ps;
		else if (ieee80211_vif_is_mesh(&sdata->vif))
			ps = &sdata->u.mesh.ps;
		else
			return;

		clear_sta_flag(sta, WLAN_STA_PS_STA);

		atomic_dec(&ps->num_sta_ps);
		sta_info_recalc_tim(sta);
	}

	for (ac = 0; ac < IEEE80211_NUM_ACS; ac++) {
		local->total_ps_buffered -= skb_queue_len(&sta->ps_tx_buf[ac]);
		ieee80211_purge_tx_queue(&local->hw, &sta->ps_tx_buf[ac]);
		ieee80211_purge_tx_queue(&local->hw, &sta->tx_filtered[ac]);
	}

	if (ieee80211_vif_is_mesh(&sdata->vif))
		mesh_sta_cleanup(sta);

	cancel_work_sync(&sta->drv_unblock_wk);

	/*
	 * Destroy aggregation state here. It would be nice to wait for the
	 * driver to finish aggregation stop and then clean up, but for now
	 * drivers have to handle aggregation stop being requested, followed
	 * directly by station destruction.
	 */
	for (i = 0; i < IEEE80211_NUM_TIDS; i++) {
		kfree(sta->ampdu_mlme.tid_start_tx[i]);
		tid_tx = rcu_dereference_raw(sta->ampdu_mlme.tid_tx[i]);
		if (!tid_tx)
			continue;
		ieee80211_purge_tx_queue(&local->hw, &tid_tx->pending);
		kfree(tid_tx);
	}

	sta_info_free(local, sta);
}

void ieee80211_cleanup_sdata_stas(struct ieee80211_sub_if_data *sdata)
{
	struct sta_info *sta;

	spin_lock_bh(&sdata->cleanup_stations_lock);
	while (!list_empty(&sdata->cleanup_stations)) {
		sta = list_first_entry(&sdata->cleanup_stations,
				       struct sta_info, list);
		list_del(&sta->list);
		spin_unlock_bh(&sdata->cleanup_stations_lock);

		cleanup_single_sta(sta);

		spin_lock_bh(&sdata->cleanup_stations_lock);
	}

	spin_unlock_bh(&sdata->cleanup_stations_lock);
}

static void free_sta_rcu(struct rcu_head *h)
{
	struct sta_info *sta = container_of(h, struct sta_info, rcu_head);
	struct ieee80211_sub_if_data *sdata = sta->sdata;

	spin_lock(&sdata->cleanup_stations_lock);
	list_add_tail(&sta->list, &sdata->cleanup_stations);
	spin_unlock(&sdata->cleanup_stations_lock);

	ieee80211_queue_work(&sdata->local->hw, &sdata->cleanup_stations_wk);
}

>>>>>>> refs/remotes/origin/master
/* protected by RCU */
struct sta_info *sta_info_get(struct ieee80211_sub_if_data *sdata,
			      const u8 *addr)
{
	struct ieee80211_local *local = sdata->local;
	struct sta_info *sta;

	sta = rcu_dereference_check(local->sta_hash[STA_HASH(addr)],
<<<<<<< HEAD
<<<<<<< HEAD
				    rcu_read_lock_held() ||
				    lockdep_is_held(&local->sta_lock) ||
				    lockdep_is_held(&local->sta_mtx));
	while (sta) {
		if (sta->sdata == sdata &&
		    memcmp(sta->sta.addr, addr, ETH_ALEN) == 0)
			break;
		sta = rcu_dereference_check(sta->hnext,
					    rcu_read_lock_held() ||
					    lockdep_is_held(&local->sta_lock) ||
=======
				    lockdep_is_held(&local->sta_mtx));
	while (sta) {
		if (sta->sdata == sdata &&
		    compare_ether_addr(sta->sta.addr, addr) == 0)
			break;
		sta = rcu_dereference_check(sta->hnext,
>>>>>>> refs/remotes/origin/cm-10.0
=======
				    lockdep_is_held(&local->sta_mtx));
	while (sta) {
		if (sta->sdata == sdata &&
		    ether_addr_equal(sta->sta.addr, addr))
			break;
		sta = rcu_dereference_check(sta->hnext,
>>>>>>> refs/remotes/origin/master
					    lockdep_is_held(&local->sta_mtx));
	}
	return sta;
}

/*
 * Get sta info either from the specified interface
 * or from one of its vlans
 */
struct sta_info *sta_info_get_bss(struct ieee80211_sub_if_data *sdata,
				  const u8 *addr)
{
	struct ieee80211_local *local = sdata->local;
	struct sta_info *sta;

	sta = rcu_dereference_check(local->sta_hash[STA_HASH(addr)],
<<<<<<< HEAD
<<<<<<< HEAD
				    rcu_read_lock_held() ||
				    lockdep_is_held(&local->sta_lock) ||
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				    lockdep_is_held(&local->sta_mtx));
	while (sta) {
		if ((sta->sdata == sdata ||
		     (sta->sdata->bss && sta->sdata->bss == sdata->bss)) &&
<<<<<<< HEAD
<<<<<<< HEAD
		    memcmp(sta->sta.addr, addr, ETH_ALEN) == 0)
			break;
		sta = rcu_dereference_check(sta->hnext,
					    rcu_read_lock_held() ||
					    lockdep_is_held(&local->sta_lock) ||
=======
		    compare_ether_addr(sta->sta.addr, addr) == 0)
			break;
		sta = rcu_dereference_check(sta->hnext,
>>>>>>> refs/remotes/origin/cm-10.0
=======
		    ether_addr_equal(sta->sta.addr, addr))
			break;
		sta = rcu_dereference_check(sta->hnext,
>>>>>>> refs/remotes/origin/master
					    lockdep_is_held(&local->sta_mtx));
	}
	return sta;
}

struct sta_info *sta_info_get_by_idx(struct ieee80211_sub_if_data *sdata,
				     int idx)
{
	struct ieee80211_local *local = sdata->local;
	struct sta_info *sta;
	int i = 0;

	list_for_each_entry_rcu(sta, &local->sta_list, list) {
		if (sdata != sta->sdata)
			continue;
		if (i < idx) {
			++i;
			continue;
		}
		return sta;
	}

	return NULL;
}

/**
<<<<<<< HEAD
<<<<<<< HEAD
 * __sta_info_free - internal STA free helper
=======
 * sta_info_free - free STA
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * sta_info_free - free STA
>>>>>>> refs/remotes/origin/master
 *
 * @local: pointer to the global information
 * @sta: STA info to free
 *
 * This function must undo everything done by sta_info_alloc()
<<<<<<< HEAD
<<<<<<< HEAD
 * that may happen before sta_info_insert().
 */
static void __sta_info_free(struct ieee80211_local *local,
			    struct sta_info *sta)
{
	if (sta->rate_ctrl) {
		rate_control_free_sta(sta);
		rate_control_put(sta->rate_ctrl);
	}
=======
=======
>>>>>>> refs/remotes/origin/master
 * that may happen before sta_info_insert(). It may only be
 * called when sta_info_insert() has not been attempted (and
 * if that fails, the station is freed anyway.)
 */
void sta_info_free(struct ieee80211_local *local, struct sta_info *sta)
{
	if (sta->rate_ctrl)
		rate_control_free_sta(sta);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

#ifdef CONFIG_MAC80211_VERBOSE_DEBUG
	wiphy_debug(local->hw.wiphy, "Destroyed STA %pM\n", sta->sta.addr);
#endif /* CONFIG_MAC80211_VERBOSE_DEBUG */
=======

	sta_dbg(sta->sdata, "Destroyed STA %pM\n", sta->sta.addr);
>>>>>>> refs/remotes/origin/master

	kfree(sta);
}

<<<<<<< HEAD
<<<<<<< HEAD
/* Caller must hold local->sta_lock */
static void sta_info_hash_add(struct ieee80211_local *local,
			      struct sta_info *sta)
{
=======
=======
>>>>>>> refs/remotes/origin/master
/* Caller must hold local->sta_mtx */
static void sta_info_hash_add(struct ieee80211_local *local,
			      struct sta_info *sta)
{
	lockdep_assert_held(&local->sta_mtx);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	sta->hnext = local->sta_hash[STA_HASH(sta->sta.addr)];
	rcu_assign_pointer(local->sta_hash[STA_HASH(sta->sta.addr)], sta);
}

static void sta_unblock(struct work_struct *wk)
{
	struct sta_info *sta;

	sta = container_of(wk, struct sta_info, drv_unblock_wk);

	if (sta->dead)
		return;

<<<<<<< HEAD
<<<<<<< HEAD
	if (!test_sta_flags(sta, WLAN_STA_PS_STA))
		ieee80211_sta_ps_deliver_wakeup(sta);
	else if (test_and_clear_sta_flags(sta, WLAN_STA_PSPOLL)) {
		clear_sta_flags(sta, WLAN_STA_PS_DRIVER);
		ieee80211_sta_ps_deliver_poll_response(sta);
	} else
		clear_sta_flags(sta, WLAN_STA_PS_DRIVER);
=======
=======
>>>>>>> refs/remotes/origin/master
	if (!test_sta_flag(sta, WLAN_STA_PS_STA)) {
		local_bh_disable();
		ieee80211_sta_ps_deliver_wakeup(sta);
		local_bh_enable();
	} else if (test_and_clear_sta_flag(sta, WLAN_STA_PSPOLL)) {
		clear_sta_flag(sta, WLAN_STA_PS_DRIVER);

		local_bh_disable();
		ieee80211_sta_ps_deliver_poll_response(sta);
		local_bh_enable();
	} else if (test_and_clear_sta_flag(sta, WLAN_STA_UAPSD)) {
		clear_sta_flag(sta, WLAN_STA_PS_DRIVER);

		local_bh_disable();
		ieee80211_sta_ps_deliver_uapsd(sta);
		local_bh_enable();
	} else
		clear_sta_flag(sta, WLAN_STA_PS_DRIVER);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static int sta_prepare_rate_control(struct ieee80211_local *local,
				    struct sta_info *sta, gfp_t gfp)
{
	if (local->hw.flags & IEEE80211_HW_HAS_RATE_CONTROL)
		return 0;

<<<<<<< HEAD
<<<<<<< HEAD
	sta->rate_ctrl = rate_control_get(local->rate_ctrl);
	sta->rate_ctrl_priv = rate_control_alloc_sta(sta->rate_ctrl,
						     &sta->sta, gfp);
	if (!sta->rate_ctrl_priv) {
		rate_control_put(sta->rate_ctrl);
		return -ENOMEM;
	}
=======
=======
>>>>>>> refs/remotes/origin/master
	sta->rate_ctrl = local->rate_ctrl;
	sta->rate_ctrl_priv = rate_control_alloc_sta(sta->rate_ctrl,
						     &sta->sta, gfp);
	if (!sta->rate_ctrl_priv)
		return -ENOMEM;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return 0;
}

struct sta_info *sta_info_alloc(struct ieee80211_sub_if_data *sdata,
<<<<<<< HEAD
<<<<<<< HEAD
				u8 *addr, gfp_t gfp)
=======
				const u8 *addr, gfp_t gfp)
>>>>>>> refs/remotes/origin/cm-10.0
=======
				const u8 *addr, gfp_t gfp)
>>>>>>> refs/remotes/origin/master
{
	struct ieee80211_local *local = sdata->local;
	struct sta_info *sta;
	struct timespec uptime;
	int i;

	sta = kzalloc(sizeof(*sta) + local->hw.sta_data_size, gfp);
	if (!sta)
		return NULL;

	spin_lock_init(&sta->lock);
<<<<<<< HEAD
<<<<<<< HEAD
	spin_lock_init(&sta->flaglock);
=======
>>>>>>> refs/remotes/origin/cm-10.0
	INIT_WORK(&sta->drv_unblock_wk, sta_unblock);
	INIT_WORK(&sta->ampdu_mlme.work, ieee80211_ba_session_work);
	mutex_init(&sta->ampdu_mlme.mtx);
=======
	INIT_WORK(&sta->drv_unblock_wk, sta_unblock);
	INIT_WORK(&sta->ampdu_mlme.work, ieee80211_ba_session_work);
	mutex_init(&sta->ampdu_mlme.mtx);
#ifdef CONFIG_MAC80211_MESH
	if (ieee80211_vif_is_mesh(&sdata->vif) &&
	    !sdata->u.mesh.user_mpm)
		init_timer(&sta->plink_timer);
	sta->nonpeer_pm = NL80211_MESH_POWER_ACTIVE;
#endif
>>>>>>> refs/remotes/origin/master

	memcpy(sta->sta.addr, addr, ETH_ALEN);
	sta->local = local;
	sta->sdata = sdata;
	sta->last_rx = jiffies;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	sta->sta_state = IEEE80211_STA_NONE;

>>>>>>> refs/remotes/origin/cm-10.0
	do_posix_clock_monotonic_gettime(&uptime);
	sta->last_connected = uptime.tv_sec;
	ewma_init(&sta->avg_signal, 1024, 8);
=======
	sta->sta_state = IEEE80211_STA_NONE;

	do_posix_clock_monotonic_gettime(&uptime);
	sta->last_connected = uptime.tv_sec;
	ewma_init(&sta->avg_signal, 1024, 8);
	for (i = 0; i < ARRAY_SIZE(sta->chain_signal_avg); i++)
		ewma_init(&sta->chain_signal_avg[i], 1024, 8);
>>>>>>> refs/remotes/origin/master

	if (sta_prepare_rate_control(local, sta, gfp)) {
		kfree(sta);
		return NULL;
	}

<<<<<<< HEAD
	for (i = 0; i < STA_TID_NUM; i++) {
=======
	for (i = 0; i < IEEE80211_NUM_TIDS; i++) {
>>>>>>> refs/remotes/origin/master
		/*
		 * timer_to_tid must be initialized with identity mapping
		 * to enable session_timer's data differentiation. See
		 * sta_rx_agg_session_timer_expired for usage.
		 */
		sta->timer_to_tid[i] = i;
	}
<<<<<<< HEAD
<<<<<<< HEAD
	skb_queue_head_init(&sta->ps_tx_buf);
	skb_queue_head_init(&sta->tx_filtered);
=======
=======
>>>>>>> refs/remotes/origin/master
	for (i = 0; i < IEEE80211_NUM_ACS; i++) {
		skb_queue_head_init(&sta->ps_tx_buf[i]);
		skb_queue_head_init(&sta->tx_filtered[i]);
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

	for (i = 0; i < NUM_RX_DATA_QUEUES; i++)
		sta->last_seq_ctrl[i] = cpu_to_le16(USHRT_MAX);

#ifdef CONFIG_MAC80211_VERBOSE_DEBUG
	wiphy_debug(local->hw.wiphy, "Allocated STA %pM\n", sta->sta.addr);
#endif /* CONFIG_MAC80211_VERBOSE_DEBUG */

#ifdef CONFIG_MAC80211_MESH
	sta->plink_state = NL80211_PLINK_LISTEN;
	init_timer(&sta->plink_timer);
#endif
=======

	for (i = 0; i < IEEE80211_NUM_TIDS; i++)
		sta->last_seq_ctrl[i] = cpu_to_le16(USHRT_MAX);

	sta->sta.smps_mode = IEEE80211_SMPS_OFF;
	if (sdata->vif.type == NL80211_IFTYPE_AP ||
	    sdata->vif.type == NL80211_IFTYPE_AP_VLAN) {
		struct ieee80211_supported_band *sband =
			local->hw.wiphy->bands[ieee80211_get_sdata_band(sdata)];
		u8 smps = (sband->ht_cap.cap & IEEE80211_HT_CAP_SM_PS) >>
				IEEE80211_HT_CAP_SM_PS_SHIFT;
		/*
		 * Assume that hostapd advertises our caps in the beacon and
		 * this is the known_smps_mode for a station that just assciated
		 */
		switch (smps) {
		case WLAN_HT_SMPS_CONTROL_DISABLED:
			sta->known_smps_mode = IEEE80211_SMPS_OFF;
			break;
		case WLAN_HT_SMPS_CONTROL_STATIC:
			sta->known_smps_mode = IEEE80211_SMPS_STATIC;
			break;
		case WLAN_HT_SMPS_CONTROL_DYNAMIC:
			sta->known_smps_mode = IEEE80211_SMPS_DYNAMIC;
			break;
		default:
			WARN_ON(1);
		}
	}

	sta_dbg(sdata, "Allocated STA %pM\n", sta->sta.addr);
>>>>>>> refs/remotes/origin/master

	return sta;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int sta_info_finish_insert(struct sta_info *sta, bool async)
{
	struct ieee80211_local *local = sta->local;
	struct ieee80211_sub_if_data *sdata = sta->sdata;
	struct station_info sinfo;
	unsigned long flags;
	int err = 0;

	lockdep_assert_held(&local->sta_mtx);

	/* notify driver */
	if (sdata->vif.type == NL80211_IFTYPE_AP_VLAN)
		sdata = container_of(sdata->bss,
				     struct ieee80211_sub_if_data,
				     u.ap);
	err = drv_sta_add(local, sdata, &sta->sta);
	if (err) {
		if (!async)
			return err;
		printk(KERN_DEBUG "%s: failed to add IBSS STA %pM to driver (%d)"
				  " - keeping it anyway.\n",
		       sdata->name, sta->sta.addr, err);
	} else {
		sta->uploaded = true;
#ifdef CONFIG_MAC80211_VERBOSE_DEBUG
		if (async)
			wiphy_debug(local->hw.wiphy,
				    "Finished adding IBSS STA %pM\n",
				    sta->sta.addr);
#endif
	}

	sdata = sta->sdata;

	if (!async) {
		local->num_sta++;
		local->sta_generation++;
		smp_mb();

		/* make the station visible */
		spin_lock_irqsave(&local->sta_lock, flags);
		sta_info_hash_add(local, sta);
		spin_unlock_irqrestore(&local->sta_lock, flags);
	}

	list_add(&sta->list, &local->sta_list);

	ieee80211_sta_debugfs_add(sta);
	rate_control_add_sta_debugfs(sta);

	memset(&sinfo, 0, sizeof(sinfo));
	sinfo.filled = 0;
	sinfo.generation = local->sta_generation;
	cfg80211_new_sta(sdata->dev, sta->sta.addr, &sinfo, GFP_KERNEL);

=======
=======
>>>>>>> refs/remotes/origin/master
static int sta_info_insert_check(struct sta_info *sta)
{
	struct ieee80211_sub_if_data *sdata = sta->sdata;

	/*
	 * Can't be a WARN_ON because it can be triggered through a race:
	 * something inserts a STA (on one CPU) without holding the RTNL
	 * and another CPU turns off the net device.
	 */
	if (unlikely(!ieee80211_sdata_running(sdata)))
		return -ENETDOWN;

<<<<<<< HEAD
	if (WARN_ON(compare_ether_addr(sta->sta.addr, sdata->vif.addr) == 0 ||
		    is_multicast_ether_addr(sta->sta.addr)))
		return -EINVAL;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (WARN_ON(ether_addr_equal(sta->sta.addr, sdata->vif.addr) ||
		    is_multicast_ether_addr(sta->sta.addr)))
		return -EINVAL;
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void sta_info_finish_pending(struct ieee80211_local *local)
{
	struct sta_info *sta;
	unsigned long flags;

	spin_lock_irqsave(&local->sta_lock, flags);
	while (!list_empty(&local->sta_pending_list)) {
		sta = list_first_entry(&local->sta_pending_list,
				       struct sta_info, list);
		list_del(&sta->list);
		spin_unlock_irqrestore(&local->sta_lock, flags);

		sta_info_finish_insert(sta, true);

		spin_lock_irqsave(&local->sta_lock, flags);
	}
	spin_unlock_irqrestore(&local->sta_lock, flags);
}

static void sta_info_finish_work(struct work_struct *work)
{
	struct ieee80211_local *local =
		container_of(work, struct ieee80211_local, sta_finish_work);

	mutex_lock(&local->sta_mtx);
	sta_info_finish_pending(local);
	mutex_unlock(&local->sta_mtx);
}

int sta_info_insert_rcu(struct sta_info *sta) __acquires(RCU)
{
	struct ieee80211_local *local = sta->local;
	struct ieee80211_sub_if_data *sdata = sta->sdata;
	unsigned long flags;
	int err = 0;

	/*
	 * Can't be a WARN_ON because it can be triggered through a race:
	 * something inserts a STA (on one CPU) without holding the RTNL
	 * and another CPU turns off the net device.
	 */
	if (unlikely(!ieee80211_sdata_running(sdata))) {
		err = -ENETDOWN;
		rcu_read_lock();
		goto out_free;
	}

	if (WARN_ON(compare_ether_addr(sta->sta.addr, sdata->vif.addr) == 0 ||
		    is_multicast_ether_addr(sta->sta.addr))) {
		err = -EINVAL;
		rcu_read_lock();
		goto out_free;
	}

	/*
	 * In ad-hoc mode, we sometimes need to insert stations
	 * from tasklet context from the RX path. To avoid races,
	 * always do so in that case -- see the comment below.
	 */
	if (sdata->vif.type == NL80211_IFTYPE_ADHOC) {
		spin_lock_irqsave(&local->sta_lock, flags);
		/* check if STA exists already */
		if (sta_info_get_bss(sdata, sta->sta.addr)) {
			spin_unlock_irqrestore(&local->sta_lock, flags);
			rcu_read_lock();
			err = -EEXIST;
			goto out_free;
		}

		local->num_sta++;
		local->sta_generation++;
		smp_mb();
		sta_info_hash_add(local, sta);

		list_add_tail(&sta->list, &local->sta_pending_list);

		rcu_read_lock();
		spin_unlock_irqrestore(&local->sta_lock, flags);

#ifdef CONFIG_MAC80211_VERBOSE_DEBUG
		wiphy_debug(local->hw.wiphy, "Added IBSS STA %pM\n",
			    sta->sta.addr);
#endif /* CONFIG_MAC80211_VERBOSE_DEBUG */

		ieee80211_queue_work(&local->hw, &local->sta_finish_work);

		return 0;
	}

	/*
	 * On first glance, this will look racy, because the code
	 * below this point, which inserts a station with sleeping,
	 * unlocks the sta_lock between checking existence in the
	 * hash table and inserting into it.
	 *
	 * However, it is not racy against itself because it keeps
	 * the mutex locked. It still seems to race against the
	 * above code that atomically inserts the station... That,
	 * however, is not true because the above code can only
	 * be invoked for IBSS interfaces, and the below code will
	 * not be -- and the two do not race against each other as
	 * the hash table also keys off the interface.
	 */

	might_sleep();

	mutex_lock(&local->sta_mtx);

	spin_lock_irqsave(&local->sta_lock, flags);
	/* check if STA exists already */
	if (sta_info_get_bss(sdata, sta->sta.addr)) {
		spin_unlock_irqrestore(&local->sta_lock, flags);
		mutex_unlock(&local->sta_mtx);
		rcu_read_lock();
		err = -EEXIST;
		goto out_free;
	}

	spin_unlock_irqrestore(&local->sta_lock, flags);

	err = sta_info_finish_insert(sta, false);
	if (err) {
		mutex_unlock(&local->sta_mtx);
=======
=======
>>>>>>> refs/remotes/origin/master
static int sta_info_insert_drv_state(struct ieee80211_local *local,
				     struct ieee80211_sub_if_data *sdata,
				     struct sta_info *sta)
{
	enum ieee80211_sta_state state;
	int err = 0;

	for (state = IEEE80211_STA_NOTEXIST; state < sta->sta_state; state++) {
		err = drv_sta_state(local, sdata, sta, state, state + 1);
		if (err)
			break;
	}

	if (!err) {
		/*
		 * Drivers using legacy sta_add/sta_remove callbacks only
		 * get uploaded set to true after sta_add is called.
		 */
		if (!local->ops->sta_add)
			sta->uploaded = true;
		return 0;
	}

	if (sdata->vif.type == NL80211_IFTYPE_ADHOC) {
<<<<<<< HEAD
		printk(KERN_DEBUG
		       "%s: failed to move IBSS STA %pM to state %d (%d) - keeping it anyway.\n",
		       sdata->name, sta->sta.addr, state + 1, err);
=======
		sdata_info(sdata,
			   "failed to move IBSS STA %pM to state %d (%d) - keeping it anyway\n",
			   sta->sta.addr, state + 1, err);
>>>>>>> refs/remotes/origin/master
		err = 0;
	}

	/* unwind on error */
	for (; state > IEEE80211_STA_NOTEXIST; state--)
		WARN_ON(drv_sta_state(local, sdata, sta, state, state - 1));

	return err;
}

/*
 * should be called with sta_mtx locked
 * this function replaces the mutex lock
 * with a RCU lock
 */
static int sta_info_insert_finish(struct sta_info *sta) __acquires(RCU)
{
	struct ieee80211_local *local = sta->local;
	struct ieee80211_sub_if_data *sdata = sta->sdata;
	struct station_info sinfo;
	int err = 0;

	lockdep_assert_held(&local->sta_mtx);

	/* check if STA exists already */
	if (sta_info_get_bss(sdata, sta->sta.addr)) {
		err = -EEXIST;
		goto out_err;
	}

	/* notify driver */
	err = sta_info_insert_drv_state(local, sdata, sta);
	if (err)
		goto out_err;

	local->num_sta++;
	local->sta_generation++;
	smp_mb();

	/* make the station visible */
	sta_info_hash_add(local, sta);

	list_add_rcu(&sta->list, &local->sta_list);

	set_sta_flag(sta, WLAN_STA_INSERTED);

	ieee80211_sta_debugfs_add(sta);
	rate_control_add_sta_debugfs(sta);

	memset(&sinfo, 0, sizeof(sinfo));
	sinfo.filled = 0;
	sinfo.generation = local->sta_generation;
	cfg80211_new_sta(sdata->dev, sta->sta.addr, &sinfo, GFP_KERNEL);

<<<<<<< HEAD
#ifdef CONFIG_MAC80211_VERBOSE_DEBUG
	wiphy_debug(local->hw.wiphy, "Inserted STA %pM\n", sta->sta.addr);
#endif /* CONFIG_MAC80211_VERBOSE_DEBUG */
=======
	sta_dbg(sdata, "Inserted STA %pM\n", sta->sta.addr);
>>>>>>> refs/remotes/origin/master

	/* move reference to rcu-protected */
	rcu_read_lock();
	mutex_unlock(&local->sta_mtx);

	if (ieee80211_vif_is_mesh(&sdata->vif))
		mesh_accept_plinks_update(sdata);

	return 0;
 out_err:
	mutex_unlock(&local->sta_mtx);
	rcu_read_lock();
	return err;
}

int sta_info_insert_rcu(struct sta_info *sta) __acquires(RCU)
{
	struct ieee80211_local *local = sta->local;
	int err = 0;

	might_sleep();

	err = sta_info_insert_check(sta);
	if (err) {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		rcu_read_lock();
		goto out_free;
	}

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_MAC80211_VERBOSE_DEBUG
	wiphy_debug(local->hw.wiphy, "Inserted STA %pM\n", sta->sta.addr);
#endif /* CONFIG_MAC80211_VERBOSE_DEBUG */

	/* move reference to rcu-protected */
	rcu_read_lock();
	mutex_unlock(&local->sta_mtx);

	if (ieee80211_vif_is_mesh(&sdata->vif))
		mesh_accept_plinks_update(sdata);
=======
=======
>>>>>>> refs/remotes/origin/master
	mutex_lock(&local->sta_mtx);

	err = sta_info_insert_finish(sta);
	if (err)
		goto out_free;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return 0;
 out_free:
	BUG_ON(!err);
<<<<<<< HEAD
<<<<<<< HEAD
	__sta_info_free(local, sta);
=======
	sta_info_free(local, sta);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	sta_info_free(local, sta);
>>>>>>> refs/remotes/origin/master
	return err;
}

int sta_info_insert(struct sta_info *sta)
{
	int err = sta_info_insert_rcu(sta);

	rcu_read_unlock();

	return err;
}

<<<<<<< HEAD
static inline void __bss_tim_set(struct ieee80211_if_ap *bss, u16 aid)
=======
static inline void __bss_tim_set(u8 *tim, u16 id)
>>>>>>> refs/remotes/origin/master
{
	/*
	 * This format has been mandated by the IEEE specifications,
	 * so this line may not be changed to use the __set_bit() format.
	 */
<<<<<<< HEAD
	bss->tim[aid / 8] |= (1 << (aid % 8));
}

static inline void __bss_tim_clear(struct ieee80211_if_ap *bss, u16 aid)
=======
	tim[id / 8] |= (1 << (id % 8));
}

static inline void __bss_tim_clear(u8 *tim, u16 id)
>>>>>>> refs/remotes/origin/master
{
	/*
	 * This format has been mandated by the IEEE specifications,
	 * so this line may not be changed to use the __clear_bit() format.
	 */
<<<<<<< HEAD
	bss->tim[aid / 8] &= ~(1 << (aid % 8));
}

<<<<<<< HEAD
static void __sta_info_set_tim_bit(struct ieee80211_if_ap *bss,
				   struct sta_info *sta)
{
	BUG_ON(!bss);

	__bss_tim_set(bss, sta->sta.aid);

	if (sta->local->ops->set_tim) {
		sta->local->tim_in_locked_section = true;
		drv_set_tim(sta->local, &sta->sta, true);
		sta->local->tim_in_locked_section = false;
	}
}

void sta_info_set_tim_bit(struct sta_info *sta)
{
	unsigned long flags;

	BUG_ON(!sta->sdata->bss);

	spin_lock_irqsave(&sta->local->sta_lock, flags);
	__sta_info_set_tim_bit(sta->sdata->bss, sta);
	spin_unlock_irqrestore(&sta->local->sta_lock, flags);
}

static void __sta_info_clear_tim_bit(struct ieee80211_if_ap *bss,
				     struct sta_info *sta)
{
	BUG_ON(!bss);

	__bss_tim_clear(bss, sta->sta.aid);

	if (sta->local->ops->set_tim) {
		sta->local->tim_in_locked_section = true;
		drv_set_tim(sta->local, &sta->sta, false);
		sta->local->tim_in_locked_section = false;
	}
}

void sta_info_clear_tim_bit(struct sta_info *sta)
{
	unsigned long flags;

	BUG_ON(!sta->sdata->bss);

	spin_lock_irqsave(&sta->local->sta_lock, flags);
	__sta_info_clear_tim_bit(sta->sdata->bss, sta);
	spin_unlock_irqrestore(&sta->local->sta_lock, flags);
}

static int sta_info_buffer_expired(struct sta_info *sta,
				   struct sk_buff *skb)
=======
=======
	tim[id / 8] &= ~(1 << (id % 8));
}

static inline bool __bss_tim_get(u8 *tim, u16 id)
{
	/*
	 * This format has been mandated by the IEEE specifications,
	 * so this line may not be changed to use the test_bit() format.
	 */
	return tim[id / 8] & (1 << (id % 8));
}

>>>>>>> refs/remotes/origin/master
static unsigned long ieee80211_tids_for_ac(int ac)
{
	/* If we ever support TIDs > 7, this obviously needs to be adjusted */
	switch (ac) {
	case IEEE80211_AC_VO:
		return BIT(6) | BIT(7);
	case IEEE80211_AC_VI:
		return BIT(4) | BIT(5);
	case IEEE80211_AC_BE:
		return BIT(0) | BIT(3);
	case IEEE80211_AC_BK:
		return BIT(1) | BIT(2);
	default:
		WARN_ON(1);
		return 0;
	}
}

void sta_info_recalc_tim(struct sta_info *sta)
{
	struct ieee80211_local *local = sta->local;
<<<<<<< HEAD
	struct ieee80211_if_ap *bss = sta->sdata->bss;
	unsigned long flags;
	bool indicate_tim = false;
	u8 ignore_for_tim = sta->sta.uapsd_queues;
	int ac;

	if (WARN_ON_ONCE(!sta->sdata->bss))
		return;
=======
	struct ps_data *ps;
	bool indicate_tim = false;
	u8 ignore_for_tim = sta->sta.uapsd_queues;
	int ac;
	u16 id;

	if (sta->sdata->vif.type == NL80211_IFTYPE_AP ||
	    sta->sdata->vif.type == NL80211_IFTYPE_AP_VLAN) {
		if (WARN_ON_ONCE(!sta->sdata->bss))
			return;

		ps = &sta->sdata->bss->ps;
		id = sta->sta.aid;
#ifdef CONFIG_MAC80211_MESH
	} else if (ieee80211_vif_is_mesh(&sta->sdata->vif)) {
		ps = &sta->sdata->u.mesh.ps;
		/* TIM map only for PLID <= IEEE80211_MAX_AID */
		id = le16_to_cpu(sta->plid) % IEEE80211_MAX_AID;
#endif
	} else {
		return;
	}
>>>>>>> refs/remotes/origin/master

	/* No need to do anything if the driver does all */
	if (local->hw.flags & IEEE80211_HW_AP_LINK_PS)
		return;

	if (sta->dead)
		goto done;

	/*
	 * If all ACs are delivery-enabled then we should build
	 * the TIM bit for all ACs anyway; if only some are then
	 * we ignore those and build the TIM bit using only the
	 * non-enabled ones.
	 */
	if (ignore_for_tim == BIT(IEEE80211_NUM_ACS) - 1)
		ignore_for_tim = 0;

	for (ac = 0; ac < IEEE80211_NUM_ACS; ac++) {
		unsigned long tids;

		if (ignore_for_tim & BIT(ac))
			continue;

		indicate_tim |= !skb_queue_empty(&sta->tx_filtered[ac]) ||
				!skb_queue_empty(&sta->ps_tx_buf[ac]);
		if (indicate_tim)
			break;

		tids = ieee80211_tids_for_ac(ac);

		indicate_tim |=
			sta->driver_buffered_tids & tids;
	}

 done:
<<<<<<< HEAD
	spin_lock_irqsave(&local->tim_lock, flags);

	if (indicate_tim)
		__bss_tim_set(bss, sta->sta.aid);
	else
		__bss_tim_clear(bss, sta->sta.aid);
=======
	spin_lock_bh(&local->tim_lock);

	if (indicate_tim == __bss_tim_get(ps->tim, id))
		goto out_unlock;

	if (indicate_tim)
		__bss_tim_set(ps->tim, id);
	else
		__bss_tim_clear(ps->tim, id);
>>>>>>> refs/remotes/origin/master

	if (local->ops->set_tim) {
		local->tim_in_locked_section = true;
		drv_set_tim(local, &sta->sta, indicate_tim);
		local->tim_in_locked_section = false;
	}

<<<<<<< HEAD
	spin_unlock_irqrestore(&local->tim_lock, flags);
}

static bool sta_info_buffer_expired(struct sta_info *sta, struct sk_buff *skb)
>>>>>>> refs/remotes/origin/cm-10.0
=======
out_unlock:
	spin_unlock_bh(&local->tim_lock);
}

static bool sta_info_buffer_expired(struct sta_info *sta, struct sk_buff *skb)
>>>>>>> refs/remotes/origin/master
{
	struct ieee80211_tx_info *info;
	int timeout;

	if (!skb)
<<<<<<< HEAD
<<<<<<< HEAD
		return 0;
=======
		return false;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		return false;
>>>>>>> refs/remotes/origin/master

	info = IEEE80211_SKB_CB(skb);

	/* Timeout: (2 * listen_interval * beacon_int * 1024 / 1000000) sec */
	timeout = (sta->listen_interval *
		   sta->sdata->vif.bss_conf.beacon_int *
		   32 / 15625) * HZ;
	if (timeout < STA_TX_BUFFER_EXPIRE)
		timeout = STA_TX_BUFFER_EXPIRE;
	return time_after(jiffies, info->control.jiffies + timeout);
}


<<<<<<< HEAD
<<<<<<< HEAD
static bool sta_info_cleanup_expire_buffered(struct ieee80211_local *local,
					     struct sta_info *sta)
=======
static bool sta_info_cleanup_expire_buffered_ac(struct ieee80211_local *local,
						struct sta_info *sta, int ac)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static bool sta_info_cleanup_expire_buffered_ac(struct ieee80211_local *local,
						struct sta_info *sta, int ac)
>>>>>>> refs/remotes/origin/master
{
	unsigned long flags;
	struct sk_buff *skb;

<<<<<<< HEAD
<<<<<<< HEAD
	if (skb_queue_empty(&sta->ps_tx_buf))
		return false;

	for (;;) {
		spin_lock_irqsave(&sta->ps_tx_buf.lock, flags);
		skb = skb_peek(&sta->ps_tx_buf);
		if (sta_info_buffer_expired(sta, skb))
			skb = __skb_dequeue(&sta->ps_tx_buf);
		else
			skb = NULL;
		spin_unlock_irqrestore(&sta->ps_tx_buf.lock, flags);

=======
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * First check for frames that should expire on the filtered
	 * queue. Frames here were rejected by the driver and are on
	 * a separate queue to avoid reordering with normal PS-buffered
	 * frames. They also aren't accounted for right now in the
	 * total_ps_buffered counter.
	 */
	for (;;) {
		spin_lock_irqsave(&sta->tx_filtered[ac].lock, flags);
		skb = skb_peek(&sta->tx_filtered[ac]);
		if (sta_info_buffer_expired(sta, skb))
			skb = __skb_dequeue(&sta->tx_filtered[ac]);
		else
			skb = NULL;
		spin_unlock_irqrestore(&sta->tx_filtered[ac].lock, flags);

		/*
		 * Frames are queued in order, so if this one
		 * hasn't expired yet we can stop testing. If
		 * we actually reached the end of the queue we
		 * also need to stop, of course.
		 */
		if (!skb)
			break;
<<<<<<< HEAD
		dev_kfree_skb(skb);
=======
		ieee80211_free_txskb(&local->hw, skb);
>>>>>>> refs/remotes/origin/master
	}

	/*
	 * Now also check the normal PS-buffered queue, this will
	 * only find something if the filtered queue was emptied
	 * since the filtered frames are all before the normal PS
	 * buffered frames.
	 */
	for (;;) {
		spin_lock_irqsave(&sta->ps_tx_buf[ac].lock, flags);
		skb = skb_peek(&sta->ps_tx_buf[ac]);
		if (sta_info_buffer_expired(sta, skb))
			skb = __skb_dequeue(&sta->ps_tx_buf[ac]);
		else
			skb = NULL;
		spin_unlock_irqrestore(&sta->ps_tx_buf[ac].lock, flags);

		/*
		 * frames are queued in order, so if this one
		 * hasn't expired yet (or we reached the end of
		 * the queue) we can stop testing
		 */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		if (!skb)
			break;

		local->total_ps_buffered--;
<<<<<<< HEAD
#ifdef CONFIG_MAC80211_VERBOSE_PS_DEBUG
		printk(KERN_DEBUG "Buffered frame expired (STA %pM)\n",
		       sta->sta.addr);
#endif
		dev_kfree_skb(skb);
<<<<<<< HEAD

		if (skb_queue_empty(&sta->ps_tx_buf) &&
		    !test_sta_flags(sta, WLAN_STA_PS_DRIVER_BUF))
			sta_info_clear_tim_bit(sta);
	}

	return true;
}

static int __must_check __sta_info_destroy(struct sta_info *sta)
{
	struct ieee80211_local *local;
	struct ieee80211_sub_if_data *sdata;
	struct sk_buff *skb;
	unsigned long flags;
	int ret, i;
=======
=======
		ps_dbg(sta->sdata, "Buffered frame expired (STA %pM)\n",
		       sta->sta.addr);
		ieee80211_free_txskb(&local->hw, skb);
>>>>>>> refs/remotes/origin/master
	}

	/*
	 * Finally, recalculate the TIM bit for this station -- it might
	 * now be clear because the station was too slow to retrieve its
	 * frames.
	 */
	sta_info_recalc_tim(sta);

	/*
	 * Return whether there are any frames still buffered, this is
	 * used to check whether the cleanup timer still needs to run,
	 * if there are no frames we don't need to rearm the timer.
	 */
	return !(skb_queue_empty(&sta->ps_tx_buf[ac]) &&
		 skb_queue_empty(&sta->tx_filtered[ac]));
}

static bool sta_info_cleanup_expire_buffered(struct ieee80211_local *local,
					     struct sta_info *sta)
{
	bool have_buffered = false;
	int ac;

<<<<<<< HEAD
	/* This is only necessary for stations on BSS interfaces */
	if (!sta->sdata->bss)
=======
	/* This is only necessary for stations on BSS/MBSS interfaces */
	if (!sta->sdata->bss &&
	    !ieee80211_vif_is_mesh(&sta->sdata->vif))
>>>>>>> refs/remotes/origin/master
		return false;

	for (ac = 0; ac < IEEE80211_NUM_ACS; ac++)
		have_buffered |=
			sta_info_cleanup_expire_buffered_ac(local, sta, ac);

	return have_buffered;
}

int __must_check __sta_info_destroy(struct sta_info *sta)
{
	struct ieee80211_local *local;
	struct ieee80211_sub_if_data *sdata;
<<<<<<< HEAD
	int ret, i, ac;
	struct tid_ampdu_tx *tid_tx;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	int ret;
>>>>>>> refs/remotes/origin/master

	might_sleep();

	if (!sta)
		return -ENOENT;

	local = sta->local;
	sdata = sta->sdata;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	lockdep_assert_held(&local->sta_mtx);

>>>>>>> refs/remotes/origin/cm-10.0
=======
	lockdep_assert_held(&local->sta_mtx);

>>>>>>> refs/remotes/origin/master
	/*
	 * Before removing the station from the driver and
	 * rate control, it might still start new aggregation
	 * sessions -- block that to make sure the tear-down
	 * will be sufficient.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	set_sta_flags(sta, WLAN_STA_BLOCK_BA);
	ieee80211_sta_tear_down_BA_sessions(sta, true);

	spin_lock_irqsave(&local->sta_lock, flags);
	ret = sta_info_hash_del(local, sta);
	/* this might still be the pending list ... which is fine */
	if (!ret)
		list_del(&sta->list);
	spin_unlock_irqrestore(&local->sta_lock, flags);
	if (ret)
		return ret;

=======
	set_sta_flag(sta, WLAN_STA_BLOCK_BA);
	ieee80211_sta_tear_down_BA_sessions(sta, true);
=======
	set_sta_flag(sta, WLAN_STA_BLOCK_BA);
	ieee80211_sta_tear_down_BA_sessions(sta, AGG_STOP_DESTROY_STA);
>>>>>>> refs/remotes/origin/master

	ret = sta_info_hash_del(local, sta);
	if (ret)
		return ret;

	list_del_rcu(&sta->list);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	mutex_lock(&local->key_mtx);
	for (i = 0; i < NUM_DEFAULT_KEYS; i++)
		__ieee80211_key_free(key_mtx_dereference(local, sta->gtk[i]));
	if (sta->ptk)
		__ieee80211_key_free(key_mtx_dereference(local, sta->ptk));
	mutex_unlock(&local->key_mtx);

	sta->dead = true;

<<<<<<< HEAD
	if (test_and_clear_sta_flags(sta,
				WLAN_STA_PS_STA | WLAN_STA_PS_DRIVER)) {
		BUG_ON(!sdata->bss);

		atomic_dec(&sdata->bss->num_sta_ps);
		sta_info_clear_tim_bit(sta);
	}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* this always calls synchronize_net() */
	ieee80211_free_sta_keys(local, sta);

	sta->dead = true;

>>>>>>> refs/remotes/origin/master
	local->num_sta--;
	local->sta_generation++;

	if (sdata->vif.type == NL80211_IFTYPE_AP_VLAN)
<<<<<<< HEAD
<<<<<<< HEAD
		rcu_assign_pointer(sdata->u.vlan.sta, NULL);

	if (sta->uploaded) {
		if (sdata->vif.type == NL80211_IFTYPE_AP_VLAN)
			sdata = container_of(sdata->bss,
					     struct ieee80211_sub_if_data,
					     u.ap);
		drv_sta_remove(local, sdata, &sta->sta);
		sdata = sta->sdata;
=======
=======
>>>>>>> refs/remotes/origin/master
		RCU_INIT_POINTER(sdata->u.vlan.sta, NULL);

	while (sta->sta_state > IEEE80211_STA_NONE) {
		ret = sta_info_move_state(sta, sta->sta_state - 1);
		if (ret) {
			WARN_ON_ONCE(1);
			break;
		}
	}

	if (sta->uploaded) {
		ret = drv_sta_state(local, sdata, sta, IEEE80211_STA_NONE,
				    IEEE80211_STA_NOTEXIST);
		WARN_ON_ONCE(ret != 0);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	}

	/*
	 * At this point, after we wait for an RCU grace period,
	 * neither mac80211 nor the driver can reference this
	 * sta struct any more except by still existing timers
	 * associated with this station that we clean up below.
	 */
	synchronize_rcu();

<<<<<<< HEAD
=======
	if (test_sta_flag(sta, WLAN_STA_PS_STA)) {
		BUG_ON(!sdata->bss);

		clear_sta_flag(sta, WLAN_STA_PS_STA);

		atomic_dec(&sdata->bss->num_sta_ps);
		sta_info_recalc_tim(sta);
	}

	for (ac = 0; ac < IEEE80211_NUM_ACS; ac++) {
		local->total_ps_buffered -= skb_queue_len(&sta->ps_tx_buf[ac]);
		ieee80211_purge_tx_queue(&local->hw, &sta->ps_tx_buf[ac]);
		ieee80211_purge_tx_queue(&local->hw, &sta->tx_filtered[ac]);
	}

>>>>>>> refs/remotes/origin/cm-10.0
#ifdef CONFIG_MAC80211_MESH
	if (ieee80211_vif_is_mesh(&sdata->vif))
		mesh_accept_plinks_update(sdata);
#endif

#ifdef CONFIG_MAC80211_VERBOSE_DEBUG
	wiphy_debug(local->hw.wiphy, "Removed STA %pM\n", sta->sta.addr);
#endif /* CONFIG_MAC80211_VERBOSE_DEBUG */
	cancel_work_sync(&sta->drv_unblock_wk);
=======
	}

	sta_dbg(sdata, "Removed STA %pM\n", sta->sta.addr);
>>>>>>> refs/remotes/origin/master

	cfg80211_del_sta(sdata->dev, sta->sta.addr, GFP_KERNEL);

	rate_control_remove_sta_debugfs(sta);
	ieee80211_sta_debugfs_remove(sta);

<<<<<<< HEAD
#ifdef CONFIG_MAC80211_MESH
	if (ieee80211_vif_is_mesh(&sta->sdata->vif)) {
		mesh_plink_deactivate(sta);
		del_timer_sync(&sta->plink_timer);
	}
#endif

<<<<<<< HEAD
	while ((skb = skb_dequeue(&sta->ps_tx_buf)) != NULL) {
		local->total_ps_buffered--;
		dev_kfree_skb_any(skb);
	}

	while ((skb = skb_dequeue(&sta->tx_filtered)) != NULL)
		dev_kfree_skb_any(skb);

	__sta_info_free(local, sta);
=======
	/*
	 * Destroy aggregation state here. It would be nice to wait for the
	 * driver to finish aggregation stop and then clean up, but for now
	 * drivers have to handle aggregation stop being requested, followed
	 * directly by station destruction.
	 */
	for (i = 0; i < STA_TID_NUM; i++) {
		tid_tx = rcu_dereference_raw(sta->ampdu_mlme.tid_tx[i]);
		if (!tid_tx)
			continue;
		ieee80211_purge_tx_queue(&local->hw, &tid_tx->pending);
		kfree(tid_tx);
	}

	sta_info_free(local, sta);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	call_rcu(&sta->rcu_head, free_sta_rcu);
>>>>>>> refs/remotes/origin/master

	return 0;
}

int sta_info_destroy_addr(struct ieee80211_sub_if_data *sdata, const u8 *addr)
{
	struct sta_info *sta;
	int ret;

	mutex_lock(&sdata->local->sta_mtx);
	sta = sta_info_get(sdata, addr);
	ret = __sta_info_destroy(sta);
	mutex_unlock(&sdata->local->sta_mtx);

	return ret;
}

int sta_info_destroy_addr_bss(struct ieee80211_sub_if_data *sdata,
			      const u8 *addr)
{
	struct sta_info *sta;
	int ret;

	mutex_lock(&sdata->local->sta_mtx);
	sta = sta_info_get_bss(sdata, addr);
	ret = __sta_info_destroy(sta);
	mutex_unlock(&sdata->local->sta_mtx);

	return ret;
}

static void sta_info_cleanup(unsigned long data)
{
	struct ieee80211_local *local = (struct ieee80211_local *) data;
	struct sta_info *sta;
	bool timer_needed = false;

	rcu_read_lock();
	list_for_each_entry_rcu(sta, &local->sta_list, list)
		if (sta_info_cleanup_expire_buffered(local, sta))
			timer_needed = true;
	rcu_read_unlock();

	if (local->quiescing)
		return;

	if (!timer_needed)
		return;

	mod_timer(&local->sta_cleanup,
		  round_jiffies(jiffies + STA_INFO_CLEANUP_INTERVAL));
}

void sta_info_init(struct ieee80211_local *local)
{
<<<<<<< HEAD
<<<<<<< HEAD
	spin_lock_init(&local->sta_lock);
	mutex_init(&local->sta_mtx);
	INIT_LIST_HEAD(&local->sta_list);
	INIT_LIST_HEAD(&local->sta_pending_list);
	INIT_WORK(&local->sta_finish_work, sta_info_finish_work);
=======
	spin_lock_init(&local->tim_lock);
	mutex_init(&local->sta_mtx);
	INIT_LIST_HEAD(&local->sta_list);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	spin_lock_init(&local->tim_lock);
	mutex_init(&local->sta_mtx);
	INIT_LIST_HEAD(&local->sta_list);
>>>>>>> refs/remotes/origin/master

	setup_timer(&local->sta_cleanup, sta_info_cleanup,
		    (unsigned long)local);
}

void sta_info_stop(struct ieee80211_local *local)
{
	del_timer_sync(&local->sta_cleanup);
<<<<<<< HEAD
	sta_info_flush(local, NULL);
}

/**
 * sta_info_flush - flush matching STA entries from the STA table
 *
 * Returns the number of removed STA entries.
 *
 * @local: local interface data
 * @sdata: matching rule for the net device (sta->dev) or %NULL to match all STAs
 */
int sta_info_flush(struct ieee80211_local *local,
		   struct ieee80211_sub_if_data *sdata)
{
=======
}


int sta_info_flush_defer(struct ieee80211_sub_if_data *sdata)
{
	struct ieee80211_local *local = sdata->local;
>>>>>>> refs/remotes/origin/master
	struct sta_info *sta, *tmp;
	int ret = 0;

	might_sleep();

	mutex_lock(&local->sta_mtx);
<<<<<<< HEAD
<<<<<<< HEAD

	sta_info_finish_pending(local);

	list_for_each_entry_safe(sta, tmp, &local->sta_list, list) {
		if (!sdata || sdata == sta->sdata)
			WARN_ON(__sta_info_destroy(sta));
=======
	list_for_each_entry_safe(sta, tmp, &local->sta_list, list) {
		if (!sdata || sdata == sta->sdata) {
			WARN_ON(__sta_info_destroy(sta));
			ret++;
		}
>>>>>>> refs/remotes/origin/cm-10.0
=======
	list_for_each_entry_safe(sta, tmp, &local->sta_list, list) {
		if (sdata == sta->sdata) {
			WARN_ON(__sta_info_destroy(sta));
			ret++;
		}
>>>>>>> refs/remotes/origin/master
	}
	mutex_unlock(&local->sta_mtx);

	return ret;
}

<<<<<<< HEAD
=======
void sta_info_flush_cleanup(struct ieee80211_sub_if_data *sdata)
{
	ieee80211_cleanup_sdata_stas(sdata);
	cancel_work_sync(&sdata->cleanup_stations_wk);
}

>>>>>>> refs/remotes/origin/master
void ieee80211_sta_expire(struct ieee80211_sub_if_data *sdata,
			  unsigned long exp_time)
{
	struct ieee80211_local *local = sdata->local;
	struct sta_info *sta, *tmp;

	mutex_lock(&local->sta_mtx);
<<<<<<< HEAD
<<<<<<< HEAD
	list_for_each_entry_safe(sta, tmp, &local->sta_list, list)
=======
=======
>>>>>>> refs/remotes/origin/master

	list_for_each_entry_safe(sta, tmp, &local->sta_list, list) {
		if (sdata != sta->sdata)
			continue;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		if (time_after(jiffies, sta->last_rx + exp_time)) {
#ifdef CONFIG_MAC80211_IBSS_DEBUG
			printk(KERN_DEBUG "%s: expiring inactive STA %pM\n",
			       sdata->name, sta->sta.addr);
#endif
			WARN_ON(__sta_info_destroy(sta));
		}
<<<<<<< HEAD
=======
	}

>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (time_after(jiffies, sta->last_rx + exp_time)) {
			sta_dbg(sta->sdata, "expiring inactive STA %pM\n",
				sta->sta.addr);

			if (ieee80211_vif_is_mesh(&sdata->vif) &&
			    test_sta_flag(sta, WLAN_STA_PS_STA))
				atomic_dec(&sdata->u.mesh.ps.num_sta_ps);

			WARN_ON(__sta_info_destroy(sta));
		}
	}

>>>>>>> refs/remotes/origin/master
	mutex_unlock(&local->sta_mtx);
}

struct ieee80211_sta *ieee80211_find_sta_by_ifaddr(struct ieee80211_hw *hw,
					       const u8 *addr,
					       const u8 *localaddr)
{
	struct sta_info *sta, *nxt;

	/*
	 * Just return a random station if localaddr is NULL
	 * ... first in list.
	 */
	for_each_sta_info(hw_to_local(hw), addr, sta, nxt) {
		if (localaddr &&
<<<<<<< HEAD
		    compare_ether_addr(sta->sdata->vif.addr, localaddr) != 0)
=======
		    !ether_addr_equal(sta->sdata->vif.addr, localaddr))
>>>>>>> refs/remotes/origin/master
			continue;
		if (!sta->uploaded)
			return NULL;
		return &sta->sta;
	}

	return NULL;
}
EXPORT_SYMBOL_GPL(ieee80211_find_sta_by_ifaddr);

struct ieee80211_sta *ieee80211_find_sta(struct ieee80211_vif *vif,
					 const u8 *addr)
{
	struct sta_info *sta;

	if (!vif)
		return NULL;

	sta = sta_info_get_bss(vif_to_sdata(vif), addr);
	if (!sta)
		return NULL;

	if (!sta->uploaded)
		return NULL;

	return &sta->sta;
}
EXPORT_SYMBOL(ieee80211_find_sta);

static void clear_sta_ps_flags(void *_sta)
{
	struct sta_info *sta = _sta;
<<<<<<< HEAD
<<<<<<< HEAD

	clear_sta_flags(sta, WLAN_STA_PS_DRIVER | WLAN_STA_PS_STA);
=======
	struct ieee80211_sub_if_data *sdata = sta->sdata;

	clear_sta_flag(sta, WLAN_STA_PS_DRIVER);
	if (test_and_clear_sta_flag(sta, WLAN_STA_PS_STA))
		atomic_dec(&sdata->bss->num_sta_ps);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct ieee80211_sub_if_data *sdata = sta->sdata;
	struct ps_data *ps;

	if (sdata->vif.type == NL80211_IFTYPE_AP ||
	    sdata->vif.type == NL80211_IFTYPE_AP_VLAN)
		ps = &sdata->bss->ps;
	else if (ieee80211_vif_is_mesh(&sdata->vif))
		ps = &sdata->u.mesh.ps;
	else
		return;

	clear_sta_flag(sta, WLAN_STA_PS_DRIVER);
	if (test_and_clear_sta_flag(sta, WLAN_STA_PS_STA))
		atomic_dec(&ps->num_sta_ps);
>>>>>>> refs/remotes/origin/master
}

/* powersave support code */
void ieee80211_sta_ps_deliver_wakeup(struct sta_info *sta)
{
	struct ieee80211_sub_if_data *sdata = sta->sdata;
	struct ieee80211_local *local = sdata->local;
<<<<<<< HEAD
<<<<<<< HEAD
	int sent, buffered;

	clear_sta_flags(sta, WLAN_STA_PS_DRIVER_BUF);
	if (!(local->hw.flags & IEEE80211_HW_AP_LINK_PS))
		drv_sta_notify(local, sdata, STA_NOTIFY_AWAKE, &sta->sta);

	if (!skb_queue_empty(&sta->ps_tx_buf))
		sta_info_clear_tim_bit(sta);

	/* Send all buffered frames to the station */
	sent = ieee80211_add_pending_skbs(local, &sta->tx_filtered);
	buffered = ieee80211_add_pending_skbs_fn(local, &sta->ps_tx_buf,
						 clear_sta_ps_flags, sta);
	sent += buffered;
	local->total_ps_buffered -= buffered;

#ifdef CONFIG_MAC80211_VERBOSE_PS_DEBUG
	printk(KERN_DEBUG "%s: STA %pM aid %d sending %d filtered/%d PS frames "
	       "since STA not sleeping anymore\n", sdata->name,
	       sta->sta.addr, sta->sta.aid, sent - buffered, buffered);
#endif /* CONFIG_MAC80211_VERBOSE_PS_DEBUG */
}

void ieee80211_sta_ps_deliver_poll_response(struct sta_info *sta)
{
	struct ieee80211_sub_if_data *sdata = sta->sdata;
	struct ieee80211_local *local = sdata->local;
	struct sk_buff *skb;
	int no_pending_pkts;

	skb = skb_dequeue(&sta->tx_filtered);
	if (!skb) {
		skb = skb_dequeue(&sta->ps_tx_buf);
		if (skb)
			local->total_ps_buffered--;
	}
	no_pending_pkts = skb_queue_empty(&sta->tx_filtered) &&
		skb_queue_empty(&sta->ps_tx_buf);

	if (skb) {
		struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
		struct ieee80211_hdr *hdr =
			(struct ieee80211_hdr *) skb->data;

		/*
		 * Tell TX path to send this frame even though the STA may
		 * still remain is PS mode after this frame exchange.
		 */
		info->flags |= IEEE80211_TX_CTL_PSPOLL_RESPONSE;

#ifdef CONFIG_MAC80211_VERBOSE_PS_DEBUG
		printk(KERN_DEBUG "STA %pM aid %d: PS Poll (entries after %d)\n",
		       sta->sta.addr, sta->sta.aid,
		       skb_queue_len(&sta->ps_tx_buf));
#endif /* CONFIG_MAC80211_VERBOSE_PS_DEBUG */

		/* Use MoreData flag to indicate whether there are more
		 * buffered frames for this STA */
		if (no_pending_pkts)
			hdr->frame_control &= cpu_to_le16(~IEEE80211_FCTL_MOREDATA);
		else
			hdr->frame_control |= cpu_to_le16(IEEE80211_FCTL_MOREDATA);

		ieee80211_add_pending_skb(local, skb);

		if (no_pending_pkts)
			sta_info_clear_tim_bit(sta);
#ifdef CONFIG_MAC80211_VERBOSE_PS_DEBUG
	} else {
		/*
		 * FIXME: This can be the result of a race condition between
		 *	  us expiring a frame and the station polling for it.
		 *	  Should we send it a null-func frame indicating we
		 *	  have nothing buffered for it?
		 */
		printk(KERN_DEBUG "%s: STA %pM sent PS Poll even "
		       "though there are no buffered frames for it\n",
		       sdata->name, sta->sta.addr);
#endif /* CONFIG_MAC80211_VERBOSE_PS_DEBUG */
	}
}

=======
=======
>>>>>>> refs/remotes/origin/master
	struct sk_buff_head pending;
	int filtered = 0, buffered = 0, ac;
	unsigned long flags;

	clear_sta_flag(sta, WLAN_STA_SP);

<<<<<<< HEAD
	BUILD_BUG_ON(BITS_TO_LONGS(STA_TID_NUM) > 1);
=======
	BUILD_BUG_ON(BITS_TO_LONGS(IEEE80211_NUM_TIDS) > 1);
>>>>>>> refs/remotes/origin/master
	sta->driver_buffered_tids = 0;

	if (!(local->hw.flags & IEEE80211_HW_AP_LINK_PS))
		drv_sta_notify(local, sdata, STA_NOTIFY_AWAKE, &sta->sta);

	skb_queue_head_init(&pending);

	/* Send all buffered frames to the station */
	for (ac = 0; ac < IEEE80211_NUM_ACS; ac++) {
		int count = skb_queue_len(&pending), tmp;

		spin_lock_irqsave(&sta->tx_filtered[ac].lock, flags);
		skb_queue_splice_tail_init(&sta->tx_filtered[ac], &pending);
		spin_unlock_irqrestore(&sta->tx_filtered[ac].lock, flags);
		tmp = skb_queue_len(&pending);
		filtered += tmp - count;
		count = tmp;

		spin_lock_irqsave(&sta->ps_tx_buf[ac].lock, flags);
		skb_queue_splice_tail_init(&sta->ps_tx_buf[ac], &pending);
		spin_unlock_irqrestore(&sta->ps_tx_buf[ac].lock, flags);
		tmp = skb_queue_len(&pending);
		buffered += tmp - count;
	}

	ieee80211_add_pending_skbs_fn(local, &pending, clear_sta_ps_flags, sta);

<<<<<<< HEAD
=======
	/* This station just woke up and isn't aware of our SMPS state */
	if (!ieee80211_smps_is_restrictive(sta->known_smps_mode,
					   sdata->smps_mode) &&
	    sta->known_smps_mode != sdata->bss->req_smps &&
	    sta_info_tx_streams(sta) != 1) {
		ht_dbg(sdata,
		       "%pM just woke up and MIMO capable - update SMPS\n",
		       sta->sta.addr);
		ieee80211_send_smps_action(sdata, sdata->bss->req_smps,
					   sta->sta.addr,
					   sdata->vif.bss_conf.bssid);
	}

>>>>>>> refs/remotes/origin/master
	local->total_ps_buffered -= buffered;

	sta_info_recalc_tim(sta);

<<<<<<< HEAD
#ifdef CONFIG_MAC80211_VERBOSE_PS_DEBUG
	printk(KERN_DEBUG "%s: STA %pM aid %d sending %d filtered/%d PS frames "
	       "since STA not sleeping anymore\n", sdata->name,
	       sta->sta.addr, sta->sta.aid, filtered, buffered);
#endif /* CONFIG_MAC80211_VERBOSE_PS_DEBUG */
=======
	ps_dbg(sdata,
	       "STA %pM aid %d sending %d filtered/%d PS frames since STA not sleeping anymore\n",
	       sta->sta.addr, sta->sta.aid, filtered, buffered);
>>>>>>> refs/remotes/origin/master
}

static void ieee80211_send_null_response(struct ieee80211_sub_if_data *sdata,
					 struct sta_info *sta, int tid,
					 enum ieee80211_frame_release_type reason)
{
	struct ieee80211_local *local = sdata->local;
	struct ieee80211_qos_hdr *nullfunc;
	struct sk_buff *skb;
	int size = sizeof(*nullfunc);
	__le16 fc;
	bool qos = test_sta_flag(sta, WLAN_STA_WME);
	struct ieee80211_tx_info *info;
<<<<<<< HEAD
=======
	struct ieee80211_chanctx_conf *chanctx_conf;
>>>>>>> refs/remotes/origin/master

	if (qos) {
		fc = cpu_to_le16(IEEE80211_FTYPE_DATA |
				 IEEE80211_STYPE_QOS_NULLFUNC |
				 IEEE80211_FCTL_FROMDS);
	} else {
		size -= 2;
		fc = cpu_to_le16(IEEE80211_FTYPE_DATA |
				 IEEE80211_STYPE_NULLFUNC |
				 IEEE80211_FCTL_FROMDS);
	}

	skb = dev_alloc_skb(local->hw.extra_tx_headroom + size);
	if (!skb)
		return;

	skb_reserve(skb, local->hw.extra_tx_headroom);

	nullfunc = (void *) skb_put(skb, size);
	nullfunc->frame_control = fc;
	nullfunc->duration_id = 0;
	memcpy(nullfunc->addr1, sta->sta.addr, ETH_ALEN);
	memcpy(nullfunc->addr2, sdata->vif.addr, ETH_ALEN);
	memcpy(nullfunc->addr3, sdata->vif.addr, ETH_ALEN);

	skb->priority = tid;
	skb_set_queue_mapping(skb, ieee802_1d_to_ac[tid]);
	if (qos) {
		nullfunc->qos_ctrl = cpu_to_le16(tid);

		if (reason == IEEE80211_FRAME_RELEASE_UAPSD)
			nullfunc->qos_ctrl |=
				cpu_to_le16(IEEE80211_QOS_CTL_EOSP);
	}

	info = IEEE80211_SKB_CB(skb);

	/*
	 * Tell TX path to send this frame even though the
	 * STA may still remain is PS mode after this frame
	 * exchange. Also set EOSP to indicate this packet
	 * ends the poll/service period.
	 */
	info->flags |= IEEE80211_TX_CTL_NO_PS_BUFFER |
<<<<<<< HEAD
=======
		       IEEE80211_TX_CTL_PS_RESPONSE |
>>>>>>> refs/remotes/origin/master
		       IEEE80211_TX_STATUS_EOSP |
		       IEEE80211_TX_CTL_REQ_TX_STATUS;

	drv_allow_buffered_frames(local, sta, BIT(tid), 1, reason, false);

<<<<<<< HEAD
	ieee80211_xmit(sdata, skb);
=======
	skb->dev = sdata->dev;

	rcu_read_lock();
	chanctx_conf = rcu_dereference(sdata->vif.chanctx_conf);
	if (WARN_ON(!chanctx_conf)) {
		rcu_read_unlock();
		kfree_skb(skb);
		return;
	}

	ieee80211_xmit(sdata, skb, chanctx_conf->def.chan->band);
	rcu_read_unlock();
>>>>>>> refs/remotes/origin/master
}

static void
ieee80211_sta_ps_deliver_response(struct sta_info *sta,
				  int n_frames, u8 ignored_acs,
				  enum ieee80211_frame_release_type reason)
{
	struct ieee80211_sub_if_data *sdata = sta->sdata;
	struct ieee80211_local *local = sdata->local;
	bool found = false;
	bool more_data = false;
	int ac;
	unsigned long driver_release_tids = 0;
	struct sk_buff_head frames;

	/* Service or PS-Poll period starts */
	set_sta_flag(sta, WLAN_STA_SP);

	__skb_queue_head_init(&frames);

	/*
	 * Get response frame(s) and more data bit for it.
	 */
	for (ac = 0; ac < IEEE80211_NUM_ACS; ac++) {
		unsigned long tids;

		if (ignored_acs & BIT(ac))
			continue;

		tids = ieee80211_tids_for_ac(ac);

		if (!found) {
			driver_release_tids = sta->driver_buffered_tids & tids;
			if (driver_release_tids) {
				found = true;
			} else {
				struct sk_buff *skb;

				while (n_frames > 0) {
					skb = skb_dequeue(&sta->tx_filtered[ac]);
					if (!skb) {
						skb = skb_dequeue(
							&sta->ps_tx_buf[ac]);
						if (skb)
							local->total_ps_buffered--;
					}
					if (!skb)
						break;
					n_frames--;
					found = true;
					__skb_queue_tail(&frames, skb);
				}
			}

			/*
			 * If the driver has data on more than one TID then
			 * certainly there's more data if we release just a
			 * single frame now (from a single TID).
			 */
			if (reason == IEEE80211_FRAME_RELEASE_PSPOLL &&
			    hweight16(driver_release_tids) > 1) {
				more_data = true;
				driver_release_tids =
					BIT(ffs(driver_release_tids) - 1);
				break;
			}
		}

		if (!skb_queue_empty(&sta->tx_filtered[ac]) ||
		    !skb_queue_empty(&sta->ps_tx_buf[ac])) {
			more_data = true;
			break;
		}
	}

	if (!found) {
		int tid;

		/*
		 * For PS-Poll, this can only happen due to a race condition
		 * when we set the TIM bit and the station notices it, but
		 * before it can poll for the frame we expire it.
		 *
		 * For uAPSD, this is said in the standard (11.2.1.5 h):
		 *	At each unscheduled SP for a non-AP STA, the AP shall
		 *	attempt to transmit at least one MSDU or MMPDU, but no
		 *	more than the value specified in the Max SP Length field
		 *	in the QoS Capability element from delivery-enabled ACs,
		 *	that are destined for the non-AP STA.
		 *
		 * Since we have no other MSDU/MMPDU, transmit a QoS null frame.
		 */

		/* This will evaluate to 1, 3, 5 or 7. */
		tid = 7 - ((ffs(~ignored_acs) - 1) << 1);

		ieee80211_send_null_response(sdata, sta, tid, reason);
		return;
	}

	if (!driver_release_tids) {
		struct sk_buff_head pending;
		struct sk_buff *skb;
		int num = 0;
		u16 tids = 0;

		skb_queue_head_init(&pending);

		while ((skb = __skb_dequeue(&frames))) {
			struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
			struct ieee80211_hdr *hdr = (void *) skb->data;
			u8 *qoshdr = NULL;

			num++;

			/*
			 * Tell TX path to send this frame even though the
			 * STA may still remain is PS mode after this frame
			 * exchange.
			 */
<<<<<<< HEAD
			info->flags |= IEEE80211_TX_CTL_NO_PS_BUFFER;
=======
			info->flags |= IEEE80211_TX_CTL_NO_PS_BUFFER |
				       IEEE80211_TX_CTL_PS_RESPONSE;
>>>>>>> refs/remotes/origin/master

			/*
			 * Use MoreData flag to indicate whether there are
			 * more buffered frames for this STA
			 */
			if (more_data || !skb_queue_empty(&frames))
				hdr->frame_control |=
					cpu_to_le16(IEEE80211_FCTL_MOREDATA);
			else
				hdr->frame_control &=
					cpu_to_le16(~IEEE80211_FCTL_MOREDATA);

			if (ieee80211_is_data_qos(hdr->frame_control) ||
			    ieee80211_is_qos_nullfunc(hdr->frame_control))
				qoshdr = ieee80211_get_qos_ctl(hdr);

<<<<<<< HEAD
			/* set EOSP for the frame */
			if (reason == IEEE80211_FRAME_RELEASE_UAPSD &&
			    qoshdr && skb_queue_empty(&frames))
				*qoshdr |= IEEE80211_QOS_CTL_EOSP;

			info->flags |= IEEE80211_TX_STATUS_EOSP |
				       IEEE80211_TX_CTL_REQ_TX_STATUS;
=======
			/* end service period after last frame */
			if (skb_queue_empty(&frames)) {
				if (reason == IEEE80211_FRAME_RELEASE_UAPSD &&
				    qoshdr)
					*qoshdr |= IEEE80211_QOS_CTL_EOSP;

				info->flags |= IEEE80211_TX_STATUS_EOSP |
					       IEEE80211_TX_CTL_REQ_TX_STATUS;
			}
>>>>>>> refs/remotes/origin/master

			if (qoshdr)
				tids |= BIT(*qoshdr & IEEE80211_QOS_CTL_TID_MASK);
			else
				tids |= BIT(0);

			__skb_queue_tail(&pending, skb);
		}

		drv_allow_buffered_frames(local, sta, tids, num,
					  reason, more_data);

		ieee80211_add_pending_skbs(local, &pending);

		sta_info_recalc_tim(sta);
	} else {
		/*
		 * We need to release a frame that is buffered somewhere in the
		 * driver ... it'll have to handle that.
		 * Note that, as per the comment above, it'll also have to see
		 * if there is more than just one frame on the specific TID that
		 * we're releasing from, and it needs to set the more-data bit
		 * accordingly if we tell it that there's no more data. If we do
		 * tell it there's more data, then of course the more-data bit
		 * needs to be set anyway.
		 */
		drv_release_buffered_frames(local, sta, driver_release_tids,
					    n_frames, reason, more_data);

		/*
		 * Note that we don't recalculate the TIM bit here as it would
		 * most likely have no effect at all unless the driver told us
		 * that the TID became empty before returning here from the
		 * release function.
		 * Either way, however, when the driver tells us that the TID
		 * became empty we'll do the TIM recalculation.
		 */
	}
}

void ieee80211_sta_ps_deliver_poll_response(struct sta_info *sta)
{
	u8 ignore_for_response = sta->sta.uapsd_queues;

	/*
	 * If all ACs are delivery-enabled then we should reply
	 * from any of them, if only some are enabled we reply
	 * only from the non-enabled ones.
	 */
	if (ignore_for_response == BIT(IEEE80211_NUM_ACS) - 1)
		ignore_for_response = 0;

	ieee80211_sta_ps_deliver_response(sta, 1, ignore_for_response,
					  IEEE80211_FRAME_RELEASE_PSPOLL);
}

void ieee80211_sta_ps_deliver_uapsd(struct sta_info *sta)
{
	int n_frames = sta->sta.max_sp;
	u8 delivery_enabled = sta->sta.uapsd_queues;

	/*
	 * If we ever grow support for TSPEC this might happen if
	 * the TSPEC update from hostapd comes in between a trigger
	 * frame setting WLAN_STA_UAPSD in the RX path and this
	 * actually getting called.
	 */
	if (!delivery_enabled)
		return;

	switch (sta->sta.max_sp) {
	case 1:
		n_frames = 2;
		break;
	case 2:
		n_frames = 4;
		break;
	case 3:
		n_frames = 6;
		break;
	case 0:
		/* XXX: what is a good value? */
		n_frames = 8;
		break;
	}

	ieee80211_sta_ps_deliver_response(sta, n_frames, ~delivery_enabled,
					  IEEE80211_FRAME_RELEASE_UAPSD);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
void ieee80211_sta_block_awake(struct ieee80211_hw *hw,
			       struct ieee80211_sta *pubsta, bool block)
{
	struct sta_info *sta = container_of(pubsta, struct sta_info, sta);

	trace_api_sta_block_awake(sta->local, pubsta, block);

	if (block)
<<<<<<< HEAD
<<<<<<< HEAD
		set_sta_flags(sta, WLAN_STA_PS_DRIVER);
	else if (test_sta_flags(sta, WLAN_STA_PS_DRIVER))
=======
		set_sta_flag(sta, WLAN_STA_PS_DRIVER);
	else if (test_sta_flag(sta, WLAN_STA_PS_DRIVER))
>>>>>>> refs/remotes/origin/cm-10.0
=======
		set_sta_flag(sta, WLAN_STA_PS_DRIVER);
	else if (test_sta_flag(sta, WLAN_STA_PS_DRIVER))
>>>>>>> refs/remotes/origin/master
		ieee80211_queue_work(hw, &sta->drv_unblock_wk);
}
EXPORT_SYMBOL(ieee80211_sta_block_awake);

<<<<<<< HEAD
<<<<<<< HEAD
void ieee80211_sta_set_tim(struct ieee80211_sta *pubsta)
{
	struct sta_info *sta = container_of(pubsta, struct sta_info, sta);

	set_sta_flags(sta, WLAN_STA_PS_DRIVER_BUF);
	sta_info_set_tim_bit(sta);
}
EXPORT_SYMBOL(ieee80211_sta_set_tim);
=======
void ieee80211_sta_eosp_irqsafe(struct ieee80211_sta *pubsta)
{
	struct sta_info *sta = container_of(pubsta, struct sta_info, sta);
	struct ieee80211_local *local = sta->local;
	struct sk_buff *skb;
	struct skb_eosp_msg_data *data;

	trace_api_eosp(local, pubsta);

	skb = alloc_skb(0, GFP_ATOMIC);
	if (!skb) {
		/* too bad ... but race is better than loss */
		clear_sta_flag(sta, WLAN_STA_SP);
		return;
	}

	data = (void *)skb->cb;
	memcpy(data->sta, pubsta->addr, ETH_ALEN);
	memcpy(data->iface, sta->sdata->vif.addr, ETH_ALEN);
	skb->pkt_type = IEEE80211_EOSP_MSG;
	skb_queue_tail(&local->skb_queue, skb);
	tasklet_schedule(&local->tasklet);
}
EXPORT_SYMBOL(ieee80211_sta_eosp_irqsafe);
=======
void ieee80211_sta_eosp(struct ieee80211_sta *pubsta)
{
	struct sta_info *sta = container_of(pubsta, struct sta_info, sta);
	struct ieee80211_local *local = sta->local;

	trace_api_eosp(local, pubsta);

	clear_sta_flag(sta, WLAN_STA_SP);
}
EXPORT_SYMBOL(ieee80211_sta_eosp);
>>>>>>> refs/remotes/origin/master

void ieee80211_sta_set_buffered(struct ieee80211_sta *pubsta,
				u8 tid, bool buffered)
{
	struct sta_info *sta = container_of(pubsta, struct sta_info, sta);

<<<<<<< HEAD
	if (WARN_ON(tid >= STA_TID_NUM))
=======
	if (WARN_ON(tid >= IEEE80211_NUM_TIDS))
>>>>>>> refs/remotes/origin/master
		return;

	if (buffered)
		set_bit(tid, &sta->driver_buffered_tids);
	else
		clear_bit(tid, &sta->driver_buffered_tids);

	sta_info_recalc_tim(sta);
}
EXPORT_SYMBOL(ieee80211_sta_set_buffered);

int sta_info_move_state(struct sta_info *sta,
			enum ieee80211_sta_state new_state)
{
	might_sleep();

	if (sta->sta_state == new_state)
		return 0;

	/* check allowed transitions first */

	switch (new_state) {
	case IEEE80211_STA_NONE:
		if (sta->sta_state != IEEE80211_STA_AUTH)
			return -EINVAL;
		break;
	case IEEE80211_STA_AUTH:
		if (sta->sta_state != IEEE80211_STA_NONE &&
		    sta->sta_state != IEEE80211_STA_ASSOC)
			return -EINVAL;
		break;
	case IEEE80211_STA_ASSOC:
		if (sta->sta_state != IEEE80211_STA_AUTH &&
		    sta->sta_state != IEEE80211_STA_AUTHORIZED)
			return -EINVAL;
		break;
	case IEEE80211_STA_AUTHORIZED:
		if (sta->sta_state != IEEE80211_STA_ASSOC)
			return -EINVAL;
		break;
	default:
		WARN(1, "invalid state %d", new_state);
		return -EINVAL;
	}

<<<<<<< HEAD
#ifdef CONFIG_MAC80211_VERBOSE_DEBUG
	printk(KERN_DEBUG "%s: moving STA %pM to state %d\n",
		sta->sdata->name, sta->sta.addr, new_state);
#endif
=======
	sta_dbg(sta->sdata, "moving STA %pM to state %d\n",
		sta->sta.addr, new_state);
>>>>>>> refs/remotes/origin/master

	/*
	 * notify the driver before the actual changes so it can
	 * fail the transition
	 */
	if (test_sta_flag(sta, WLAN_STA_INSERTED)) {
		int err = drv_sta_state(sta->local, sta->sdata, sta,
					sta->sta_state, new_state);
		if (err)
			return err;
	}

	/* reflect the change in all state variables */

	switch (new_state) {
	case IEEE80211_STA_NONE:
		if (sta->sta_state == IEEE80211_STA_AUTH)
			clear_bit(WLAN_STA_AUTH, &sta->_flags);
		break;
	case IEEE80211_STA_AUTH:
		if (sta->sta_state == IEEE80211_STA_NONE)
			set_bit(WLAN_STA_AUTH, &sta->_flags);
		else if (sta->sta_state == IEEE80211_STA_ASSOC)
			clear_bit(WLAN_STA_ASSOC, &sta->_flags);
		break;
	case IEEE80211_STA_ASSOC:
		if (sta->sta_state == IEEE80211_STA_AUTH) {
			set_bit(WLAN_STA_ASSOC, &sta->_flags);
		} else if (sta->sta_state == IEEE80211_STA_AUTHORIZED) {
<<<<<<< HEAD
			if (sta->sdata->vif.type == NL80211_IFTYPE_AP)
				atomic_dec(&sta->sdata->u.ap.num_sta_authorized);
=======
			if (sta->sdata->vif.type == NL80211_IFTYPE_AP ||
			    (sta->sdata->vif.type == NL80211_IFTYPE_AP_VLAN &&
			     !sta->sdata->u.vlan.sta))
				atomic_dec(&sta->sdata->bss->num_mcast_sta);
>>>>>>> refs/remotes/origin/master
			clear_bit(WLAN_STA_AUTHORIZED, &sta->_flags);
		}
		break;
	case IEEE80211_STA_AUTHORIZED:
		if (sta->sta_state == IEEE80211_STA_ASSOC) {
<<<<<<< HEAD
			if (sta->sdata->vif.type == NL80211_IFTYPE_AP)
				atomic_inc(&sta->sdata->u.ap.num_sta_authorized);
=======
			if (sta->sdata->vif.type == NL80211_IFTYPE_AP ||
			    (sta->sdata->vif.type == NL80211_IFTYPE_AP_VLAN &&
			     !sta->sdata->u.vlan.sta))
				atomic_inc(&sta->sdata->bss->num_mcast_sta);
>>>>>>> refs/remotes/origin/master
			set_bit(WLAN_STA_AUTHORIZED, &sta->_flags);
		}
		break;
	default:
		break;
	}

	sta->sta_state = new_state;

	return 0;
}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======

u8 sta_info_tx_streams(struct sta_info *sta)
{
	struct ieee80211_sta_ht_cap *ht_cap = &sta->sta.ht_cap;
	u8 rx_streams;

	if (!sta->sta.ht_cap.ht_supported)
		return 1;

	if (sta->sta.vht_cap.vht_supported) {
		int i;
		u16 tx_mcs_map =
			le16_to_cpu(sta->sta.vht_cap.vht_mcs.tx_mcs_map);

		for (i = 7; i >= 0; i--)
			if ((tx_mcs_map & (0x3 << (i * 2))) !=
			    IEEE80211_VHT_MCS_NOT_SUPPORTED)
				return i + 1;
	}

	if (ht_cap->mcs.rx_mask[3])
		rx_streams = 4;
	else if (ht_cap->mcs.rx_mask[2])
		rx_streams = 3;
	else if (ht_cap->mcs.rx_mask[1])
		rx_streams = 2;
	else
		rx_streams = 1;

	if (!(ht_cap->mcs.tx_params & IEEE80211_HT_MCS_TX_RX_DIFF))
		return rx_streams;

	return ((ht_cap->mcs.tx_params & IEEE80211_HT_MCS_TX_MAX_STREAMS_MASK)
			>> IEEE80211_HT_MCS_TX_MAX_STREAMS_SHIFT) + 1;
}
>>>>>>> refs/remotes/origin/master
