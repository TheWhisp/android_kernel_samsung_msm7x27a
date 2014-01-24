/******************************************************************************
 *
 * This file is provided under a dual BSD/GPLv2 license.  When using or
 * redistributing this file, you may do so under either license.
 *
 * GPL LICENSE SUMMARY
 *
<<<<<<< HEAD
 * Copyright(c) 2008 - 2011 Intel Corporation. All rights reserved.
=======
 * Copyright(c) 2008 - 2012 Intel Corporation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-10.0
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110,
 * USA
 *
 * The full GNU General Public License is included in this distribution
 * in the file called LICENSE.GPL.
 *
 * Contact Information:
 *  Intel Linux Wireless <ilw@linux.intel.com>
 * Intel Corporation, 5200 N.E. Elam Young Parkway, Hillsboro, OR 97124-6497
 *
 * BSD LICENSE
 *
<<<<<<< HEAD
 * Copyright(c) 2005 - 2011 Intel Corporation. All rights reserved.
=======
 * Copyright(c) 2005 - 2012 Intel Corporation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-10.0
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  * Neither the name Intel Corporation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#ifndef __iwl_agn_h__
#define __iwl_agn_h__

#include "iwl-dev.h"

<<<<<<< HEAD
/* configuration for the _agn devices */
extern struct iwl_cfg iwl5300_agn_cfg;
extern struct iwl_cfg iwl5100_agn_cfg;
extern struct iwl_cfg iwl5350_agn_cfg;
extern struct iwl_cfg iwl5100_bgn_cfg;
extern struct iwl_cfg iwl5100_abg_cfg;
extern struct iwl_cfg iwl5150_agn_cfg;
extern struct iwl_cfg iwl5150_abg_cfg;
extern struct iwl_cfg iwl6005_2agn_cfg;
extern struct iwl_cfg iwl6005_2abg_cfg;
extern struct iwl_cfg iwl6005_2bg_cfg;
extern struct iwl_cfg iwl1030_bgn_cfg;
extern struct iwl_cfg iwl1030_bg_cfg;
extern struct iwl_cfg iwl6030_2agn_cfg;
extern struct iwl_cfg iwl6030_2abg_cfg;
extern struct iwl_cfg iwl6030_2bgn_cfg;
extern struct iwl_cfg iwl6030_2bg_cfg;
extern struct iwl_cfg iwl6000i_2agn_cfg;
extern struct iwl_cfg iwl6000i_2abg_cfg;
extern struct iwl_cfg iwl6000i_2bg_cfg;
extern struct iwl_cfg iwl6000_3agn_cfg;
extern struct iwl_cfg iwl6050_2agn_cfg;
extern struct iwl_cfg iwl6050_2abg_cfg;
extern struct iwl_cfg iwl6150_bgn_cfg;
extern struct iwl_cfg iwl6150_bg_cfg;
extern struct iwl_cfg iwl1000_bgn_cfg;
extern struct iwl_cfg iwl1000_bg_cfg;
extern struct iwl_cfg iwl100_bgn_cfg;
extern struct iwl_cfg iwl100_bg_cfg;
extern struct iwl_cfg iwl130_bgn_cfg;
extern struct iwl_cfg iwl130_bg_cfg;
extern struct iwl_cfg iwl2000_2bgn_cfg;
extern struct iwl_cfg iwl2000_2bg_cfg;
extern struct iwl_cfg iwl2030_2bgn_cfg;
extern struct iwl_cfg iwl2030_2bg_cfg;
extern struct iwl_cfg iwl6035_2agn_cfg;
extern struct iwl_cfg iwl6035_2abg_cfg;
extern struct iwl_cfg iwl6035_2bg_cfg;
extern struct iwl_cfg iwl105_bg_cfg;
extern struct iwl_cfg iwl105_bgn_cfg;
extern struct iwl_cfg iwl135_bg_cfg;
extern struct iwl_cfg iwl135_bgn_cfg;

extern struct iwl_mod_params iwlagn_mod_params;
extern struct iwl_hcmd_ops iwlagn_hcmd;
extern struct iwl_hcmd_ops iwlagn_bt_hcmd;
extern struct iwl_hcmd_utils_ops iwlagn_hcmd_utils;

extern struct ieee80211_ops iwlagn_hw_ops;

int iwl_reset_ict(struct iwl_priv *priv);
void iwl_disable_ict(struct iwl_priv *priv);
int iwl_alloc_isr_ict(struct iwl_priv *priv);
void iwl_free_isr_ict(struct iwl_priv *priv);
irqreturn_t iwl_isr_ict(int irq, void *data);

/* call this function to flush any scheduled tasklet */
static inline void iwl_synchronize_irq(struct iwl_priv *priv)
{
	/* wait to make sure we flush pending tasklet*/
	synchronize_irq(priv->pci_dev->irq);
	tasklet_kill(&priv->irq_tasklet);
}

int iwl_prepare_card_hw(struct iwl_priv *priv);

int iwlagn_start_device(struct iwl_priv *priv);
void iwlagn_stop_device(struct iwl_priv *priv);

/* tx queue */
void iwlagn_set_wr_ptrs(struct iwl_priv *priv,
		     int txq_id, u32 index);
void iwlagn_tx_queue_set_status(struct iwl_priv *priv,
			     struct iwl_tx_queue *txq,
			     int tx_fifo_id, int scd_retry);
void iwlagn_txq_set_sched(struct iwl_priv *priv, u32 mask);
void iwl_free_tfds_in_queue(struct iwl_priv *priv,
			    int sta_id, int tid, int freed);

/* RXON */
=======
struct iwl_ucode_capabilities;

extern struct ieee80211_ops iwlagn_hw_ops;

static inline void iwl_set_calib_hdr(struct iwl_calib_hdr *hdr, u8 cmd)
{
	hdr->op_code = cmd;
	hdr->first_group = 0;
	hdr->groups_num = 1;
	hdr->data_valid = 1;
}

void iwl_down(struct iwl_priv *priv);
void iwl_cancel_deferred_work(struct iwl_priv *priv);
void iwlagn_prepare_restart(struct iwl_priv *priv);
void iwl_free_skb(struct iwl_op_mode *op_mode, struct sk_buff *skb);
int __must_check iwl_rx_dispatch(struct iwl_op_mode *op_mode,
				 struct iwl_rx_cmd_buffer *rxb,
				 struct iwl_device_cmd *cmd);
void iwl_set_hw_rfkill_state(struct iwl_op_mode *op_mode, bool state);
void iwl_nic_error(struct iwl_op_mode *op_mode);

bool iwl_check_for_ct_kill(struct iwl_priv *priv);

void iwlagn_lift_passive_no_rx(struct iwl_priv *priv);

/* MAC80211 */
struct ieee80211_hw *iwl_alloc_all(void);
int iwlagn_mac_setup_register(struct iwl_priv *priv,
			      const struct iwl_ucode_capabilities *capa);
void iwlagn_mac_unregister(struct iwl_priv *priv);

/* commands */
int iwl_dvm_send_cmd(struct iwl_priv *priv, struct iwl_host_cmd *cmd);
int iwl_dvm_send_cmd_pdu(struct iwl_priv *priv, u8 id,
			 u32 flags, u16 len, const void *data);

/* RXON */
int iwlagn_set_pan_params(struct iwl_priv *priv);
>>>>>>> refs/remotes/origin/cm-10.0
int iwlagn_commit_rxon(struct iwl_priv *priv, struct iwl_rxon_context *ctx);
void iwlagn_set_rxon_chain(struct iwl_priv *priv, struct iwl_rxon_context *ctx);
int iwlagn_mac_config(struct ieee80211_hw *hw, u32 changed);
void iwlagn_bss_info_changed(struct ieee80211_hw *hw,
			     struct ieee80211_vif *vif,
			     struct ieee80211_bss_conf *bss_conf,
			     u32 changes);
void iwlagn_config_ht40(struct ieee80211_conf *conf,
			struct iwl_rxon_context *ctx);

/* uCode */
<<<<<<< HEAD
void iwlagn_rx_calib_result(struct iwl_priv *priv,
			 struct iwl_rx_mem_buffer *rxb);
int iwlagn_send_bt_env(struct iwl_priv *priv, u8 action, u8 type);
void iwlagn_send_prio_tbl(struct iwl_priv *priv);
int iwlagn_run_init_ucode(struct iwl_priv *priv);
int iwlagn_load_ucode_wait_alive(struct iwl_priv *priv,
				 struct fw_img *image,
				 int subtype, int alternate_subtype);

/* lib */
void iwl_check_abort_status(struct iwl_priv *priv,
			    u8 frame_count, u32 status);
void iwlagn_rx_handler_setup(struct iwl_priv *priv);
void iwlagn_setup_deferred_work(struct iwl_priv *priv);
int iwlagn_hw_valid_rtc_data_addr(u32 addr);
int iwlagn_send_tx_power(struct iwl_priv *priv);
void iwlagn_temperature(struct iwl_priv *priv);
u16 iwlagn_eeprom_calib_version(struct iwl_priv *priv);
const u8 *iwlagn_eeprom_query_addr(const struct iwl_priv *priv,
				   size_t offset);
void iwlagn_rx_queue_reset(struct iwl_priv *priv, struct iwl_rx_queue *rxq);
int iwlagn_rx_init(struct iwl_priv *priv, struct iwl_rx_queue *rxq);
int iwlagn_hw_nic_init(struct iwl_priv *priv);
int iwlagn_wait_tx_queue_empty(struct iwl_priv *priv);
int iwlagn_txfifo_flush(struct iwl_priv *priv, u16 flush_control);
void iwlagn_dev_txfifo_flush(struct iwl_priv *priv, u16 flush_control);

/* rx */
void iwlagn_rx_queue_restock(struct iwl_priv *priv);
void iwlagn_rx_allocate(struct iwl_priv *priv, gfp_t priority);
void iwlagn_rx_replenish(struct iwl_priv *priv);
void iwlagn_rx_replenish_now(struct iwl_priv *priv);
void iwlagn_rx_queue_free(struct iwl_priv *priv, struct iwl_rx_queue *rxq);
int iwlagn_rxq_stop(struct iwl_priv *priv);
int iwlagn_hwrate_to_mac80211_idx(u32 rate_n_flags, enum ieee80211_band band);
void iwl_setup_rx_handlers(struct iwl_priv *priv);

/* tx */
void iwlagn_txq_free_tfd(struct iwl_priv *priv, struct iwl_tx_queue *txq);
int iwlagn_txq_attach_buf_to_tfd(struct iwl_priv *priv,
				 struct iwl_tx_queue *txq,
				 dma_addr_t addr, u16 len, u8 reset);
void iwlagn_hwrate_to_tx_control(struct iwl_priv *priv, u32 rate_n_flags,
			      struct ieee80211_tx_info *info);
int iwlagn_tx_skb(struct iwl_priv *priv, struct sk_buff *skb);
int iwlagn_tx_agg_start(struct iwl_priv *priv, struct ieee80211_vif *vif,
			struct ieee80211_sta *sta, u16 tid, u16 *ssn);
int iwlagn_tx_agg_stop(struct iwl_priv *priv, struct ieee80211_vif *vif,
		       struct ieee80211_sta *sta, u16 tid);
void iwlagn_txq_agg_queue_setup(struct iwl_priv *priv,
				struct ieee80211_sta *sta,
				int tid, int frame_limit);
int iwlagn_txq_check_empty(struct iwl_priv *priv,
			   int sta_id, u8 tid, int txq_id);
void iwlagn_rx_reply_compressed_ba(struct iwl_priv *priv,
				struct iwl_rx_mem_buffer *rxb);
int iwlagn_tx_queue_reclaim(struct iwl_priv *priv, int txq_id, int index);
void iwlagn_hw_txq_ctx_free(struct iwl_priv *priv);
int iwlagn_txq_ctx_alloc(struct iwl_priv *priv);
void iwlagn_txq_ctx_reset(struct iwl_priv *priv);
void iwlagn_txq_ctx_stop(struct iwl_priv *priv);
=======
int iwlagn_rx_calib_result(struct iwl_priv *priv,
			    struct iwl_rx_cmd_buffer *rxb,
			    struct iwl_device_cmd *cmd);
int iwl_send_bt_env(struct iwl_priv *priv, u8 action, u8 type);
void iwl_send_prio_tbl(struct iwl_priv *priv);
int iwl_init_alive_start(struct iwl_priv *priv);
int iwl_run_init_ucode(struct iwl_priv *priv);
int iwl_load_ucode_wait_alive(struct iwl_priv *priv,
			      enum iwl_ucode_type ucode_type);
int iwl_send_calib_results(struct iwl_priv *priv);
int iwl_calib_set(struct iwl_priv *priv,
		  const struct iwl_calib_hdr *cmd, int len);
void iwl_calib_free_results(struct iwl_priv *priv);

/* lib */
int iwlagn_send_tx_power(struct iwl_priv *priv);
void iwlagn_temperature(struct iwl_priv *priv);
u16 iwl_eeprom_calib_version(struct iwl_shared *shrd);
int iwlagn_txfifo_flush(struct iwl_priv *priv, u16 flush_control);
void iwlagn_dev_txfifo_flush(struct iwl_priv *priv, u16 flush_control);
int iwlagn_send_beacon_cmd(struct iwl_priv *priv);
#ifdef CONFIG_PM_SLEEP
int iwlagn_send_patterns(struct iwl_priv *priv,
			 struct cfg80211_wowlan *wowlan);
int iwlagn_suspend(struct iwl_priv *priv, struct cfg80211_wowlan *wowlan);
#endif

/* rx */
int iwlagn_hwrate_to_mac80211_idx(u32 rate_n_flags, enum ieee80211_band band);
void iwl_setup_rx_handlers(struct iwl_priv *priv);


/* tx */
int iwlagn_tx_skb(struct iwl_priv *priv, struct sk_buff *skb);
int iwlagn_tx_agg_start(struct iwl_priv *priv, struct ieee80211_vif *vif,
			struct ieee80211_sta *sta, u16 tid, u16 *ssn);
int iwlagn_tx_agg_oper(struct iwl_priv *priv, struct ieee80211_vif *vif,
			struct ieee80211_sta *sta, u16 tid, u8 buf_size);
int iwlagn_tx_agg_stop(struct iwl_priv *priv, struct ieee80211_vif *vif,
		       struct ieee80211_sta *sta, u16 tid);
int iwlagn_rx_reply_compressed_ba(struct iwl_priv *priv,
				   struct iwl_rx_cmd_buffer *rxb,
				   struct iwl_device_cmd *cmd);
int iwlagn_rx_reply_tx(struct iwl_priv *priv, struct iwl_rx_cmd_buffer *rxb,
			       struct iwl_device_cmd *cmd);
>>>>>>> refs/remotes/origin/cm-10.0

static inline u32 iwl_tx_status_to_mac80211(u32 status)
{
	status &= TX_STATUS_MSK;

	switch (status) {
	case TX_STATUS_SUCCESS:
	case TX_STATUS_DIRECT_DONE:
		return IEEE80211_TX_STAT_ACK;
	case TX_STATUS_FAIL_DEST_PS:
	case TX_STATUS_FAIL_PASSIVE_NO_RX:
		return IEEE80211_TX_STAT_TX_FILTERED;
	default:
		return 0;
	}
}

static inline bool iwl_is_tx_success(u32 status)
{
	status &= TX_STATUS_MSK;
	return (status == TX_STATUS_SUCCESS) ||
	       (status == TX_STATUS_DIRECT_DONE);
}

u8 iwl_toggle_tx_ant(struct iwl_priv *priv, u8 ant_idx, u8 valid);

/* scan */
<<<<<<< HEAD
int iwlagn_request_scan(struct iwl_priv *priv, struct ieee80211_vif *vif);
void iwlagn_post_scan(struct iwl_priv *priv);

/* station mgmt */
int iwlagn_manage_ibss_station(struct iwl_priv *priv,
			       struct ieee80211_vif *vif, bool add);

/* hcmd */
int iwlagn_send_tx_ant_config(struct iwl_priv *priv, u8 valid_tx_ant);
int iwlagn_send_beacon_cmd(struct iwl_priv *priv);

/* bt coex */
void iwlagn_send_advance_bt_config(struct iwl_priv *priv);
void iwlagn_bt_coex_profile_notif(struct iwl_priv *priv,
				  struct iwl_rx_mem_buffer *rxb);
void iwlagn_bt_rx_handler_setup(struct iwl_priv *priv);
void iwlagn_bt_setup_deferred_work(struct iwl_priv *priv);
void iwlagn_bt_cancel_deferred_work(struct iwl_priv *priv);
=======
void iwlagn_post_scan(struct iwl_priv *priv);
void iwlagn_disable_roc(struct iwl_priv *priv);

/* bt coex */
void iwlagn_send_advance_bt_config(struct iwl_priv *priv);
int iwlagn_bt_coex_profile_notif(struct iwl_priv *priv,
				  struct iwl_rx_cmd_buffer *rxb,
				  struct iwl_device_cmd *cmd);
void iwlagn_bt_rx_handler_setup(struct iwl_priv *priv);
void iwlagn_bt_setup_deferred_work(struct iwl_priv *priv);
void iwlagn_bt_cancel_deferred_work(struct iwl_priv *priv);
void iwlagn_bt_coex_rssi_monitor(struct iwl_priv *priv);
void iwlagn_bt_adjust_rssi_monitor(struct iwl_priv *priv, bool rssi_ena);
>>>>>>> refs/remotes/origin/cm-10.0

#ifdef CONFIG_IWLWIFI_DEBUG
const char *iwl_get_tx_fail_reason(u32 status);
const char *iwl_get_agg_tx_fail_reason(u16 status);
#else
static inline const char *iwl_get_tx_fail_reason(u32 status) { return ""; }
static inline const char *iwl_get_agg_tx_fail_reason(u16 status) { return ""; }
#endif

<<<<<<< HEAD
/* station management */
=======

/* station management */
int iwlagn_manage_ibss_station(struct iwl_priv *priv,
			       struct ieee80211_vif *vif, bool add);
#define IWL_STA_DRIVER_ACTIVE BIT(0) /* driver entry is active */
#define IWL_STA_UCODE_ACTIVE  BIT(1) /* ucode entry is active */
#define IWL_STA_UCODE_INPROGRESS  BIT(2) /* ucode entry is in process of
					    being activated */
#define IWL_STA_LOCAL BIT(3) /* station state not directed by mac80211;
				(this is for the IBSS BSSID stations) */
#define IWL_STA_BCAST BIT(4) /* this station is the special bcast station */


void iwl_restore_stations(struct iwl_priv *priv, struct iwl_rxon_context *ctx);
void iwl_clear_ucode_stations(struct iwl_priv *priv,
			      struct iwl_rxon_context *ctx);
void iwl_dealloc_bcast_stations(struct iwl_priv *priv);
int iwl_get_free_ucode_key_offset(struct iwl_priv *priv);
int iwl_send_add_sta(struct iwl_priv *priv,
		     struct iwl_addsta_cmd *sta, u8 flags);
int iwl_add_station_common(struct iwl_priv *priv, struct iwl_rxon_context *ctx,
			   const u8 *addr, bool is_ap,
			   struct ieee80211_sta *sta, u8 *sta_id_r);
int iwl_remove_station(struct iwl_priv *priv, const u8 sta_id,
		       const u8 *addr);
void iwl_deactivate_station(struct iwl_priv *priv, const u8 sta_id,
			    const u8 *addr);
u8 iwl_prep_station(struct iwl_priv *priv, struct iwl_rxon_context *ctx,
		    const u8 *addr, bool is_ap, struct ieee80211_sta *sta);

void iwl_sta_fill_lq(struct iwl_priv *priv, struct iwl_rxon_context *ctx,
		     u8 sta_id, struct iwl_link_quality_cmd *link_cmd);
int iwl_send_lq_cmd(struct iwl_priv *priv, struct iwl_rxon_context *ctx,
		    struct iwl_link_quality_cmd *lq, u8 flags, bool init);
int iwl_add_sta_callback(struct iwl_priv *priv, struct iwl_rx_cmd_buffer *rxb,
			       struct iwl_device_cmd *cmd);
int iwl_sta_update_ht(struct iwl_priv *priv, struct iwl_rxon_context *ctx,
		      struct ieee80211_sta *sta);


static inline int iwl_sta_id(struct ieee80211_sta *sta)
{
	if (WARN_ON(!sta))
		return IWL_INVALID_STATION;

	return ((struct iwl_station_priv *)sta->drv_priv)->sta_id;
}

>>>>>>> refs/remotes/origin/cm-10.0
int iwlagn_alloc_bcast_station(struct iwl_priv *priv,
			       struct iwl_rxon_context *ctx);
int iwlagn_add_bssid_station(struct iwl_priv *priv, struct iwl_rxon_context *ctx,
			     const u8 *addr, u8 *sta_id_r);
int iwl_remove_default_wep_key(struct iwl_priv *priv,
			       struct iwl_rxon_context *ctx,
			       struct ieee80211_key_conf *key);
int iwl_set_default_wep_key(struct iwl_priv *priv,
			    struct iwl_rxon_context *ctx,
			    struct ieee80211_key_conf *key);
int iwl_restore_default_wep_keys(struct iwl_priv *priv,
				 struct iwl_rxon_context *ctx);
int iwl_set_dynamic_key(struct iwl_priv *priv, struct iwl_rxon_context *ctx,
<<<<<<< HEAD
			struct ieee80211_key_conf *key, u8 sta_id);
int iwl_remove_dynamic_key(struct iwl_priv *priv, struct iwl_rxon_context *ctx,
			   struct ieee80211_key_conf *key, u8 sta_id);
void iwl_update_tkip_key(struct iwl_priv *priv,
			 struct iwl_rxon_context *ctx,
=======
			struct ieee80211_key_conf *key,
			struct ieee80211_sta *sta);
int iwl_remove_dynamic_key(struct iwl_priv *priv, struct iwl_rxon_context *ctx,
			   struct ieee80211_key_conf *key,
			   struct ieee80211_sta *sta);
void iwl_update_tkip_key(struct iwl_priv *priv,
			 struct ieee80211_vif *vif,
>>>>>>> refs/remotes/origin/cm-10.0
			 struct ieee80211_key_conf *keyconf,
			 struct ieee80211_sta *sta, u32 iv32, u16 *phase1key);
int iwl_sta_tx_modify_enable_tid(struct iwl_priv *priv, int sta_id, int tid);
int iwl_sta_rx_agg_start(struct iwl_priv *priv, struct ieee80211_sta *sta,
			 int tid, u16 ssn);
int iwl_sta_rx_agg_stop(struct iwl_priv *priv, struct ieee80211_sta *sta,
			int tid);
void iwl_sta_modify_sleep_tx_count(struct iwl_priv *priv, int sta_id, int cnt);
<<<<<<< HEAD
int iwl_update_bcast_stations(struct iwl_priv *priv);
void iwlagn_mac_sta_notify(struct ieee80211_hw *hw,
			   struct ieee80211_vif *vif,
			   enum sta_notify_cmd cmd,
			   struct ieee80211_sta *sta);
=======
int iwl_update_bcast_station(struct iwl_priv *priv,
			     struct iwl_rxon_context *ctx);
int iwl_update_bcast_stations(struct iwl_priv *priv);
>>>>>>> refs/remotes/origin/cm-10.0

/* rate */
static inline u32 iwl_ant_idx_to_flags(u8 ant_idx)
{
	return BIT(ant_idx) << RATE_MCS_ANT_POS;
}

static inline u8 iwl_hw_get_rate(__le32 rate_n_flags)
{
	return le32_to_cpu(rate_n_flags) & RATE_MCS_RATE_MSK;
}

static inline __le32 iwl_hw_set_rate_n_flags(u8 rate, u32 flags)
{
	return cpu_to_le32(flags|(u32)rate);
}

/* eeprom */
<<<<<<< HEAD
void iwlcore_eeprom_enhanced_txpower(struct iwl_priv *priv);
void iwl_eeprom_get_mac(const struct iwl_priv *priv, u8 *mac);

/* notification wait support */
void __acquires(wait_entry)
iwlagn_init_notification_wait(struct iwl_priv *priv,
			      struct iwl_notification_wait *wait_entry,
			      u8 cmd,
			      void (*fn)(struct iwl_priv *priv,
					 struct iwl_rx_packet *pkt,
					 void *data),
			      void *fn_data);
int __must_check __releases(wait_entry)
iwlagn_wait_notification(struct iwl_priv *priv,
			 struct iwl_notification_wait *wait_entry,
			 unsigned long timeout);
void __releases(wait_entry)
iwlagn_remove_notification(struct iwl_priv *priv,
			   struct iwl_notification_wait *wait_entry);
extern int iwlagn_init_alive_start(struct iwl_priv *priv);
extern int iwl_alive_start(struct iwl_priv *priv);
/* svtool */
#ifdef CONFIG_IWLWIFI_DEVICE_SVTOOL
extern int iwl_testmode_cmd(struct ieee80211_hw *hw, void *data, int len);
=======
void iwl_eeprom_get_mac(const struct iwl_shared *shrd, u8 *mac);

extern int iwl_alive_start(struct iwl_priv *priv);
/* svtool */
#ifdef CONFIG_IWLWIFI_DEVICE_TESTMODE
extern int iwlagn_mac_testmode_cmd(struct ieee80211_hw *hw, void *data,
				   int len);
extern int iwlagn_mac_testmode_dump(struct ieee80211_hw *hw,
				    struct sk_buff *skb,
				    struct netlink_callback *cb,
				    void *data, int len);
>>>>>>> refs/remotes/origin/cm-10.0
extern void iwl_testmode_init(struct iwl_priv *priv);
extern void iwl_testmode_cleanup(struct iwl_priv *priv);
#else
static inline
<<<<<<< HEAD
int iwl_testmode_cmd(struct ieee80211_hw *hw, void *data, int len)
=======
int iwlagn_mac_testmode_cmd(struct ieee80211_hw *hw, void *data, int len)
{
	return -ENOSYS;
}
static inline
int iwlagn_mac_testmode_dump(struct ieee80211_hw *hw, struct sk_buff *skb,
		      struct netlink_callback *cb,
		      void *data, int len)
>>>>>>> refs/remotes/origin/cm-10.0
{
	return -ENOSYS;
}
static inline
void iwl_testmode_init(struct iwl_priv *priv)
{
}
static inline
void iwl_testmode_cleanup(struct iwl_priv *priv)
{
}
#endif

<<<<<<< HEAD
=======
#ifdef CONFIG_IWLWIFI_DEBUG
void iwl_print_rx_config_cmd(struct iwl_priv *priv,
			     enum iwl_rxon_context_id ctxid);
#else
static inline void iwl_print_rx_config_cmd(struct iwl_priv *priv,
					   enum iwl_rxon_context_id ctxid)
{
}
#endif

/* status checks */

static inline int iwl_is_ready(struct iwl_priv *priv)
{
	/* The adapter is 'ready' if READY and GEO_CONFIGURED bits are
	 * set but EXIT_PENDING is not */
	return test_bit(STATUS_READY, &priv->status) &&
	       test_bit(STATUS_GEO_CONFIGURED, &priv->status) &&
	       !test_bit(STATUS_EXIT_PENDING, &priv->status);
}

static inline int iwl_is_alive(struct iwl_priv *priv)
{
	return test_bit(STATUS_ALIVE, &priv->status);
}

static inline int iwl_is_rfkill(struct iwl_priv *priv)
{
	return test_bit(STATUS_RF_KILL_HW, &priv->status);
}

static inline int iwl_is_ctkill(struct iwl_priv *priv)
{
	return test_bit(STATUS_CT_KILL, &priv->status);
}

static inline int iwl_is_ready_rf(struct iwl_priv *priv)
{
	if (iwl_is_rfkill(priv))
		return 0;

	return iwl_is_ready(priv);
}

#ifdef CONFIG_IWLWIFI_DEBUG
#define IWL_DEBUG_QUIET_RFKILL(m, fmt, args...)	\
do {									\
	if (!iwl_is_rfkill((m)))					\
		IWL_ERR(m, fmt, ##args);				\
	else								\
		__iwl_err(trans(m)->dev, true,				\
			  !iwl_have_debug_level(IWL_DL_RADIO),		\
			  fmt, ##args);					\
} while (0)
#else
#define IWL_DEBUG_QUIET_RFKILL(m, fmt, args...)	\
do {									\
	if (!iwl_is_rfkill((m)))					\
		IWL_ERR(m, fmt, ##args);				\
	else								\
		__iwl_err(trans(m)->dev, true, true, fmt, ##args);	\
} while (0)
#endif				/* CONFIG_IWLWIFI_DEBUG */

>>>>>>> refs/remotes/origin/cm-10.0
#endif /* __iwl_agn_h__ */
