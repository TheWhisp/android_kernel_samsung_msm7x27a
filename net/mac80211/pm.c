#include <net/mac80211.h>
#include <net/rtnetlink.h>

#include "ieee80211_i.h"
#include "mesh.h"
#include "driver-ops.h"
#include "led.h"

<<<<<<< HEAD
<<<<<<< HEAD
=======
/* return value indicates whether the driver should be further notified */
static bool ieee80211_quiesce(struct ieee80211_sub_if_data *sdata)
{
	switch (sdata->vif.type) {
	case NL80211_IFTYPE_STATION:
		ieee80211_sta_quiesce(sdata);
		return true;
	case NL80211_IFTYPE_ADHOC:
		ieee80211_ibss_quiesce(sdata);
		return true;
	case NL80211_IFTYPE_MESH_POINT:
		ieee80211_mesh_quiesce(sdata);
		return true;
	case NL80211_IFTYPE_AP_VLAN:
	case NL80211_IFTYPE_MONITOR:
		/* don't tell driver about this */
		return false;
	default:
		return true;
	}
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
int __ieee80211_suspend(struct ieee80211_hw *hw, struct cfg80211_wowlan *wowlan)
{
	struct ieee80211_local *local = hw_to_local(hw);
	struct ieee80211_sub_if_data *sdata;
	struct sta_info *sta;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (!local->open_count)
		goto suspend;

>>>>>>> refs/remotes/origin/cm-10.0
	ieee80211_scan_cancel(local);

	if (hw->flags & IEEE80211_HW_AMPDU_AGGREGATION) {
		mutex_lock(&local->sta_mtx);
		list_for_each_entry(sta, &local->sta_list, list) {
<<<<<<< HEAD
			set_sta_flags(sta, WLAN_STA_BLOCK_BA);
=======
			set_sta_flag(sta, WLAN_STA_BLOCK_BA);
>>>>>>> refs/remotes/origin/cm-10.0
			ieee80211_sta_tear_down_BA_sessions(sta, true);
=======
	if (!local->open_count)
		goto suspend;

	ieee80211_scan_cancel(local);

	ieee80211_dfs_cac_cancel(local);

	ieee80211_roc_purge(local, NULL);

	ieee80211_del_virtual_monitor(local);

	if (hw->flags & IEEE80211_HW_AMPDU_AGGREGATION) {
		mutex_lock(&local->sta_mtx);
		list_for_each_entry(sta, &local->sta_list, list) {
			set_sta_flag(sta, WLAN_STA_BLOCK_BA);
			ieee80211_sta_tear_down_BA_sessions(
					sta, AGG_STOP_LOCAL_REQUEST);
>>>>>>> refs/remotes/origin/master
		}
		mutex_unlock(&local->sta_mtx);
	}

	ieee80211_stop_queues_by_reason(hw,
<<<<<<< HEAD
			IEEE80211_QUEUE_STOP_REASON_SUSPEND);

	/* flush out all packets */
	synchronize_net();

	drv_flush(local, false);
=======
					IEEE80211_MAX_QUEUE_MAP,
					IEEE80211_QUEUE_STOP_REASON_SUSPEND);

	/* flush out all packets and station cleanup call_rcu()s */
	synchronize_net();
	rcu_barrier();

	ieee80211_flush_queues(local, NULL);
>>>>>>> refs/remotes/origin/master

	local->quiescing = true;
	/* make quiescing visible to timers everywhere */
	mb();

	flush_workqueue(local->workqueue);

	/* Don't try to run timers while suspended. */
	del_timer_sync(&local->sta_cleanup);

	 /*
	 * Note that this particular timer doesn't need to be
	 * restarted at resume.
	 */
	cancel_work_sync(&local->dynamic_ps_enable_work);
	del_timer_sync(&local->dynamic_ps_timer);

	local->wowlan = wowlan && local->open_count;
	if (local->wowlan) {
		int err = drv_suspend(local, wowlan);
<<<<<<< HEAD
<<<<<<< HEAD
		if (err) {
			local->quiescing = false;
			return err;
		}
		goto suspend;
=======
		if (err < 0) {
			local->quiescing = false;
			return err;
		} else if (err > 0) {
			WARN_ON(err != 1);
			local->wowlan = false;
		} else {
			list_for_each_entry(sdata, &local->interfaces, list) {
				cancel_work_sync(&sdata->work);
				ieee80211_quiesce(sdata);
			}
			goto suspend;
		}
>>>>>>> refs/remotes/origin/cm-10.0
	}

	/* disable keys */
	list_for_each_entry(sdata, &local->interfaces, list)
		ieee80211_disable_keys(sdata);

=======
		if (err < 0) {
			local->quiescing = false;
			local->wowlan = false;
			if (hw->flags & IEEE80211_HW_AMPDU_AGGREGATION) {
				mutex_lock(&local->sta_mtx);
				list_for_each_entry(sta,
						    &local->sta_list, list) {
					clear_sta_flag(sta, WLAN_STA_BLOCK_BA);
				}
				mutex_unlock(&local->sta_mtx);
			}
			ieee80211_wake_queues_by_reason(hw,
					IEEE80211_MAX_QUEUE_MAP,
					IEEE80211_QUEUE_STOP_REASON_SUSPEND);
			return err;
		} else if (err > 0) {
			WARN_ON(err != 1);
			return err;
		} else {
			goto suspend;
		}
	}

>>>>>>> refs/remotes/origin/master
	/* tear down aggregation sessions and remove STAs */
	mutex_lock(&local->sta_mtx);
	list_for_each_entry(sta, &local->sta_list, list) {
		if (sta->uploaded) {
<<<<<<< HEAD
<<<<<<< HEAD
			sdata = sta->sdata;
			if (sdata->vif.type == NL80211_IFTYPE_AP_VLAN)
				sdata = container_of(sdata->bss,
					     struct ieee80211_sub_if_data,
					     u.ap);

			drv_sta_remove(local, sdata, &sta->sta);
=======
=======
>>>>>>> refs/remotes/origin/master
			enum ieee80211_sta_state state;

			state = sta->sta_state;
			for (; state > IEEE80211_STA_NOTEXIST; state--)
				WARN_ON(drv_sta_state(local, sta->sdata, sta,
						      state, state - 1));
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		}

		mesh_plink_quiesce(sta);
	}
	mutex_unlock(&local->sta_mtx);

	/* remove all interfaces */
	list_for_each_entry(sdata, &local->interfaces, list) {
		cancel_work_sync(&sdata->work);

<<<<<<< HEAD
		switch(sdata->vif.type) {
		case NL80211_IFTYPE_STATION:
			ieee80211_sta_quiesce(sdata);
			break;
		case NL80211_IFTYPE_ADHOC:
			ieee80211_ibss_quiesce(sdata);
			break;
		case NL80211_IFTYPE_MESH_POINT:
			ieee80211_mesh_quiesce(sdata);
			break;
		case NL80211_IFTYPE_AP_VLAN:
		case NL80211_IFTYPE_MONITOR:
			/* don't tell driver about this */
			continue;
		default:
			break;
		}
=======
		if (!ieee80211_quiesce(sdata))
			continue;
>>>>>>> refs/remotes/origin/cm-10.0

		if (!ieee80211_sdata_running(sdata))
			continue;

		/* disable beaconing */
		ieee80211_bss_info_change_notify(sdata,
			BSS_CHANGED_BEACON_ENABLED);

<<<<<<< HEAD
		drv_remove_interface(local, &sdata->vif);
=======
		drv_remove_interface(local, sdata);
>>>>>>> refs/remotes/origin/cm-10.0
	}

=======
		}
	}
	mutex_unlock(&local->sta_mtx);

	/* remove all interfaces that were created in the driver */
	list_for_each_entry(sdata, &local->interfaces, list) {
		if (!ieee80211_sdata_running(sdata) ||
		    sdata->vif.type == NL80211_IFTYPE_AP_VLAN ||
		    sdata->vif.type == NL80211_IFTYPE_MONITOR)
			continue;

		drv_remove_interface(local, sdata);
	}

	/*
	 * We disconnected on all interfaces before suspend, all channel
	 * contexts should be released.
	 */
	WARN_ON(!list_empty(&local->chanctx_list));

>>>>>>> refs/remotes/origin/master
	/* stop hardware - this must stop RX */
	if (local->open_count)
		ieee80211_stop_device(local);

 suspend:
	local->suspended = true;
	/* need suspended to be visible before quiescing is false */
	barrier();
	local->quiescing = false;

	return 0;
}

/*
 * __ieee80211_resume() is a static inline which just calls
 * ieee80211_reconfig(), which is also needed for hardware
 * hang/firmware failure/etc. recovery.
 */
<<<<<<< HEAD
=======

void ieee80211_report_wowlan_wakeup(struct ieee80211_vif *vif,
				    struct cfg80211_wowlan_wakeup *wakeup,
				    gfp_t gfp)
{
	struct ieee80211_sub_if_data *sdata = vif_to_sdata(vif);

	cfg80211_report_wowlan_wakeup(&sdata->wdev, wakeup, gfp);
}
EXPORT_SYMBOL(ieee80211_report_wowlan_wakeup);
>>>>>>> refs/remotes/origin/master
