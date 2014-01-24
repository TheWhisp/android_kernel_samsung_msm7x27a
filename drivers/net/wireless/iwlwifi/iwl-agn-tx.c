/******************************************************************************
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
 *****************************************************************************/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
<<<<<<< HEAD

#include "iwl-dev.h"
#include "iwl-core.h"
#include "iwl-sta.h"
#include "iwl-io.h"
#include "iwl-helpers.h"
#include "iwl-agn-hw.h"
#include "iwl-agn.h"

/*
 * mac80211 queues, ACs, hardware queues, FIFOs.
 *
 * Cf. http://wireless.kernel.org/en/developers/Documentation/mac80211/queues
 *
 * Mac80211 uses the following numbers, which we get as from it
 * by way of skb_get_queue_mapping(skb):
 *
 *	VO	0
 *	VI	1
 *	BE	2
 *	BK	3
 *
 *
 * Regular (not A-MPDU) frames are put into hardware queues corresponding
 * to the FIFOs, see comments in iwl-prph.h. Aggregated frames get their
 * own queue per aggregation session (RA/TID combination), such queues are
 * set up to map into FIFOs too, for which we need an AC->FIFO mapping. In
 * order to map frames to the right queue, we also need an AC->hw queue
 * mapping. This is implemented here.
 *
 * Due to the way hw queues are set up (by the hw specific modules like
 * iwl-4965.c, iwl-5000.c etc.), the AC->hw queue mapping is the identity
 * mapping.
 */

static const u8 tid_to_ac[] = {
	IEEE80211_AC_BE,
	IEEE80211_AC_BK,
	IEEE80211_AC_BK,
	IEEE80211_AC_BE,
	IEEE80211_AC_VI,
	IEEE80211_AC_VI,
	IEEE80211_AC_VO,
	IEEE80211_AC_VO
};

static inline int get_ac_from_tid(u16 tid)
{
	if (likely(tid < ARRAY_SIZE(tid_to_ac)))
		return tid_to_ac[tid];

	/* no support for TIDs 8-15 yet */
	return -EINVAL;
}

static inline int get_fifo_from_tid(struct iwl_rxon_context *ctx, u16 tid)
{
	if (likely(tid < ARRAY_SIZE(tid_to_ac)))
		return ctx->ac_to_fifo[tid_to_ac[tid]];

	/* no support for TIDs 8-15 yet */
	return -EINVAL;
}

/**
 * iwlagn_txq_update_byte_cnt_tbl - Set up entry in Tx byte-count array
 */
static void iwlagn_txq_update_byte_cnt_tbl(struct iwl_priv *priv,
					   struct iwl_tx_queue *txq,
					   u16 byte_cnt)
{
	struct iwlagn_scd_bc_tbl *scd_bc_tbl = priv->scd_bc_tbls.addr;
	int write_ptr = txq->q.write_ptr;
	int txq_id = txq->q.id;
	u8 sec_ctl = 0;
	u8 sta_id = 0;
	u16 len = byte_cnt + IWL_TX_CRC_SIZE + IWL_TX_DELIMITER_SIZE;
	__le16 bc_ent;

	WARN_ON(len > 0xFFF || write_ptr >= TFD_QUEUE_SIZE_MAX);

	sta_id = txq->cmd[txq->q.write_ptr]->cmd.tx.sta_id;
	sec_ctl = txq->cmd[txq->q.write_ptr]->cmd.tx.sec_ctl;

	switch (sec_ctl & TX_CMD_SEC_MSK) {
	case TX_CMD_SEC_CCM:
		len += CCMP_MIC_LEN;
		break;
	case TX_CMD_SEC_TKIP:
		len += TKIP_ICV_LEN;
		break;
	case TX_CMD_SEC_WEP:
		len += WEP_IV_LEN + WEP_ICV_LEN;
		break;
	}

	bc_ent = cpu_to_le16((len & 0xFFF) | (sta_id << 12));

	scd_bc_tbl[txq_id].tfd_offset[write_ptr] = bc_ent;

	if (write_ptr < TFD_QUEUE_SIZE_BC_DUP)
		scd_bc_tbl[txq_id].
			tfd_offset[TFD_QUEUE_SIZE_MAX + write_ptr] = bc_ent;
}

static void iwlagn_txq_inval_byte_cnt_tbl(struct iwl_priv *priv,
					  struct iwl_tx_queue *txq)
{
	struct iwlagn_scd_bc_tbl *scd_bc_tbl = priv->scd_bc_tbls.addr;
	int txq_id = txq->q.id;
	int read_ptr = txq->q.read_ptr;
	u8 sta_id = 0;
	__le16 bc_ent;

	WARN_ON(read_ptr >= TFD_QUEUE_SIZE_MAX);

	if (txq_id != priv->cmd_queue)
		sta_id = txq->cmd[read_ptr]->cmd.tx.sta_id;

	bc_ent = cpu_to_le16(1 | (sta_id << 12));
	scd_bc_tbl[txq_id].tfd_offset[read_ptr] = bc_ent;

	if (read_ptr < TFD_QUEUE_SIZE_BC_DUP)
		scd_bc_tbl[txq_id].
			tfd_offset[TFD_QUEUE_SIZE_MAX + read_ptr] = bc_ent;
}

static int iwlagn_tx_queue_set_q2ratid(struct iwl_priv *priv, u16 ra_tid,
					u16 txq_id)
{
	u32 tbl_dw_addr;
	u32 tbl_dw;
	u16 scd_q2ratid;

	scd_q2ratid = ra_tid & IWL_SCD_QUEUE_RA_TID_MAP_RATID_MSK;

	tbl_dw_addr = priv->scd_base_addr +
			IWLAGN_SCD_TRANSLATE_TBL_OFFSET_QUEUE(txq_id);

	tbl_dw = iwl_read_targ_mem(priv, tbl_dw_addr);

	if (txq_id & 0x1)
		tbl_dw = (scd_q2ratid << 16) | (tbl_dw & 0x0000FFFF);
	else
		tbl_dw = scd_q2ratid | (tbl_dw & 0xFFFF0000);

	iwl_write_targ_mem(priv, tbl_dw_addr, tbl_dw);

	return 0;
}

static void iwlagn_tx_queue_stop_scheduler(struct iwl_priv *priv, u16 txq_id)
{
	/* Simply stop the queue, but don't change any configuration;
	 * the SCD_ACT_EN bit is the write-enable mask for the ACTIVE bit. */
	iwl_write_prph(priv,
		IWLAGN_SCD_QUEUE_STATUS_BITS(txq_id),
		(0 << IWLAGN_SCD_QUEUE_STTS_REG_POS_ACTIVE)|
		(1 << IWLAGN_SCD_QUEUE_STTS_REG_POS_SCD_ACT_EN));
}

void iwlagn_set_wr_ptrs(struct iwl_priv *priv,
				int txq_id, u32 index)
{
	iwl_write_direct32(priv, HBUS_TARG_WRPTR,
			(index & 0xff) | (txq_id << 8));
	iwl_write_prph(priv, IWLAGN_SCD_QUEUE_RDPTR(txq_id), index);
}

void iwlagn_tx_queue_set_status(struct iwl_priv *priv,
					struct iwl_tx_queue *txq,
					int tx_fifo_id, int scd_retry)
{
	int txq_id = txq->q.id;
	int active = test_bit(txq_id, &priv->txq_ctx_active_msk) ? 1 : 0;

	iwl_write_prph(priv, IWLAGN_SCD_QUEUE_STATUS_BITS(txq_id),
			(active << IWLAGN_SCD_QUEUE_STTS_REG_POS_ACTIVE) |
			(tx_fifo_id << IWLAGN_SCD_QUEUE_STTS_REG_POS_TXF) |
			(1 << IWLAGN_SCD_QUEUE_STTS_REG_POS_WSL) |
			IWLAGN_SCD_QUEUE_STTS_REG_MSK);

	txq->sched_retry = scd_retry;

	IWL_DEBUG_INFO(priv, "%s %s Queue %d on FIFO %d\n",
		       active ? "Activate" : "Deactivate",
		       scd_retry ? "BA" : "AC/CMD", txq_id, tx_fifo_id);
}

static int iwlagn_txq_agg_enable(struct iwl_priv *priv, int txq_id, int sta_id, int tid)
{
	if ((IWLAGN_FIRST_AMPDU_QUEUE > txq_id) ||
	    (IWLAGN_FIRST_AMPDU_QUEUE +
		priv->cfg->base_params->num_of_ampdu_queues <= txq_id)) {
		IWL_WARN(priv,
			"queue number out of range: %d, must be %d to %d\n",
			txq_id, IWLAGN_FIRST_AMPDU_QUEUE,
			IWLAGN_FIRST_AMPDU_QUEUE +
			priv->cfg->base_params->num_of_ampdu_queues - 1);
		return -EINVAL;
	}

	/* Modify device's station table to Tx this TID */
	return iwl_sta_tx_modify_enable_tid(priv, sta_id, tid);
}

void iwlagn_txq_agg_queue_setup(struct iwl_priv *priv,
				struct ieee80211_sta *sta,
				int tid, int frame_limit)
{
	int sta_id, tx_fifo, txq_id, ssn_idx;
	u16 ra_tid;
	unsigned long flags;
	struct iwl_tid_data *tid_data;

	sta_id = iwl_sta_id(sta);
	if (WARN_ON(sta_id == IWL_INVALID_STATION))
		return;
	if (WARN_ON(tid >= MAX_TID_COUNT))
		return;

	spin_lock_irqsave(&priv->sta_lock, flags);
	tid_data = &priv->stations[sta_id].tid[tid];
	ssn_idx = SEQ_TO_SN(tid_data->seq_number);
	txq_id = tid_data->agg.txq_id;
	tx_fifo = tid_data->agg.tx_fifo;
	spin_unlock_irqrestore(&priv->sta_lock, flags);

	ra_tid = BUILD_RAxTID(sta_id, tid);

	spin_lock_irqsave(&priv->lock, flags);

	/* Stop this Tx queue before configuring it */
	iwlagn_tx_queue_stop_scheduler(priv, txq_id);

	/* Map receiver-address / traffic-ID to this queue */
	iwlagn_tx_queue_set_q2ratid(priv, ra_tid, txq_id);

	/* Set this queue as a chain-building queue */
	iwl_set_bits_prph(priv, IWLAGN_SCD_QUEUECHAIN_SEL, (1<<txq_id));

	/* enable aggregations for the queue */
	iwl_set_bits_prph(priv, IWLAGN_SCD_AGGR_SEL, (1<<txq_id));

	/* Place first TFD at index corresponding to start sequence number.
	 * Assumes that ssn_idx is valid (!= 0xFFF) */
	priv->txq[txq_id].q.read_ptr = (ssn_idx & 0xff);
	priv->txq[txq_id].q.write_ptr = (ssn_idx & 0xff);
	iwlagn_set_wr_ptrs(priv, txq_id, ssn_idx);

	/* Set up Tx window size and frame limit for this queue */
	iwl_write_targ_mem(priv, priv->scd_base_addr +
			IWLAGN_SCD_CONTEXT_QUEUE_OFFSET(txq_id) +
			sizeof(u32),
			((frame_limit <<
			IWLAGN_SCD_QUEUE_CTX_REG2_WIN_SIZE_POS) &
			IWLAGN_SCD_QUEUE_CTX_REG2_WIN_SIZE_MSK) |
			((frame_limit <<
			IWLAGN_SCD_QUEUE_CTX_REG2_FRAME_LIMIT_POS) &
			IWLAGN_SCD_QUEUE_CTX_REG2_FRAME_LIMIT_MSK));

	iwl_set_bits_prph(priv, IWLAGN_SCD_INTERRUPT_MASK, (1 << txq_id));

	/* Set up Status area in SRAM, map to Tx DMA/FIFO, activate the queue */
	iwlagn_tx_queue_set_status(priv, &priv->txq[txq_id], tx_fifo, 1);

	spin_unlock_irqrestore(&priv->lock, flags);
}

static int iwlagn_txq_agg_disable(struct iwl_priv *priv, u16 txq_id,
				  u16 ssn_idx, u8 tx_fifo)
{
	if ((IWLAGN_FIRST_AMPDU_QUEUE > txq_id) ||
	    (IWLAGN_FIRST_AMPDU_QUEUE +
		priv->cfg->base_params->num_of_ampdu_queues <= txq_id)) {
		IWL_ERR(priv,
			"queue number out of range: %d, must be %d to %d\n",
			txq_id, IWLAGN_FIRST_AMPDU_QUEUE,
			IWLAGN_FIRST_AMPDU_QUEUE +
			priv->cfg->base_params->num_of_ampdu_queues - 1);
		return -EINVAL;
	}

	iwlagn_tx_queue_stop_scheduler(priv, txq_id);

	iwl_clear_bits_prph(priv, IWLAGN_SCD_AGGR_SEL, (1 << txq_id));

	priv->txq[txq_id].q.read_ptr = (ssn_idx & 0xff);
	priv->txq[txq_id].q.write_ptr = (ssn_idx & 0xff);
	/* supposes that ssn_idx is valid (!= 0xFFF) */
	iwlagn_set_wr_ptrs(priv, txq_id, ssn_idx);

	iwl_clear_bits_prph(priv, IWLAGN_SCD_INTERRUPT_MASK, (1 << txq_id));
	iwl_txq_ctx_deactivate(priv, txq_id);
	iwlagn_tx_queue_set_status(priv, &priv->txq[txq_id], tx_fifo, 0);

	return 0;
}

/*
 * Activate/Deactivate Tx DMA/FIFO channels according tx fifos mask
 * must be called under priv->lock and mac access
 */
void iwlagn_txq_set_sched(struct iwl_priv *priv, u32 mask)
{
	iwl_write_prph(priv, IWLAGN_SCD_TXFACT, mask);
=======
#include <linux/ieee80211.h>

#include "iwl-dev.h"
#include "iwl-core.h"
#include "iwl-io.h"
#include "iwl-agn-hw.h"
#include "iwl-agn.h"
#include "iwl-trans.h"

static void iwlagn_tx_cmd_protection(struct iwl_priv *priv,
				     struct ieee80211_tx_info *info,
				     __le16 fc, __le32 *tx_flags)
{
	if (info->control.rates[0].flags & IEEE80211_TX_RC_USE_RTS_CTS ||
	    info->control.rates[0].flags & IEEE80211_TX_RC_USE_CTS_PROTECT ||
	    info->flags & IEEE80211_TX_CTL_AMPDU)
		*tx_flags |= TX_CMD_FLG_PROT_REQUIRE_MSK;
>>>>>>> refs/remotes/origin/cm-10.0
}

/*
 * handle build REPLY_TX command notification.
 */
static void iwlagn_tx_cmd_build_basic(struct iwl_priv *priv,
<<<<<<< HEAD
					struct sk_buff *skb,
					struct iwl_tx_cmd *tx_cmd,
					struct ieee80211_tx_info *info,
					struct ieee80211_hdr *hdr,
					u8 std_id)
=======
				      struct sk_buff *skb,
				      struct iwl_tx_cmd *tx_cmd,
				      struct ieee80211_tx_info *info,
				      struct ieee80211_hdr *hdr, u8 sta_id)
>>>>>>> refs/remotes/origin/cm-10.0
{
	__le16 fc = hdr->frame_control;
	__le32 tx_flags = tx_cmd->tx_flags;

	tx_cmd->stop_time.life_time = TX_CMD_LIFE_TIME_INFINITE;
<<<<<<< HEAD
	if (!(info->flags & IEEE80211_TX_CTL_NO_ACK)) {
		tx_flags |= TX_CMD_FLG_ACK_MSK;
		if (ieee80211_is_mgmt(fc))
			tx_flags |= TX_CMD_FLG_SEQ_CTL_MSK;
		if (ieee80211_is_probe_resp(fc) &&
		    !(le16_to_cpu(hdr->seq_ctrl) & 0xf))
			tx_flags |= TX_CMD_FLG_TSF_MSK;
	} else {
		tx_flags &= (~TX_CMD_FLG_ACK_MSK);
		tx_flags |= TX_CMD_FLG_SEQ_CTL_MSK;
	}

	if (ieee80211_is_back_req(fc))
		tx_flags |= TX_CMD_FLG_ACK_MSK | TX_CMD_FLG_IMM_BA_RSP_MASK;
	else if (info->band == IEEE80211_BAND_2GHZ &&
		 priv->cfg->bt_params &&
		 priv->cfg->bt_params->advanced_bt_coexist &&
=======

	if (!(info->flags & IEEE80211_TX_CTL_NO_ACK))
		tx_flags |= TX_CMD_FLG_ACK_MSK;
	else
		tx_flags &= ~TX_CMD_FLG_ACK_MSK;

	if (ieee80211_is_probe_resp(fc))
		tx_flags |= TX_CMD_FLG_TSF_MSK;
	else if (ieee80211_is_back_req(fc))
		tx_flags |= TX_CMD_FLG_ACK_MSK | TX_CMD_FLG_IMM_BA_RSP_MASK;
	else if (info->band == IEEE80211_BAND_2GHZ &&
		 cfg(priv)->bt_params &&
		 cfg(priv)->bt_params->advanced_bt_coexist &&
>>>>>>> refs/remotes/origin/cm-10.0
		 (ieee80211_is_auth(fc) || ieee80211_is_assoc_req(fc) ||
		 ieee80211_is_reassoc_req(fc) ||
		 skb->protocol == cpu_to_be16(ETH_P_PAE)))
		tx_flags |= TX_CMD_FLG_IGNORE_BT;


<<<<<<< HEAD
	tx_cmd->sta_id = std_id;
=======
	tx_cmd->sta_id = sta_id;
>>>>>>> refs/remotes/origin/cm-10.0
	if (ieee80211_has_morefrags(fc))
		tx_flags |= TX_CMD_FLG_MORE_FRAG_MSK;

	if (ieee80211_is_data_qos(fc)) {
		u8 *qc = ieee80211_get_qos_ctl(hdr);
		tx_cmd->tid_tspec = qc[0] & 0xf;
		tx_flags &= ~TX_CMD_FLG_SEQ_CTL_MSK;
	} else {
<<<<<<< HEAD
=======
		tx_cmd->tid_tspec = IWL_TID_NON_QOS;
>>>>>>> refs/remotes/origin/cm-10.0
		if (info->flags & IEEE80211_TX_CTL_ASSIGN_SEQ)
			tx_flags |= TX_CMD_FLG_SEQ_CTL_MSK;
		else
			tx_flags &= ~TX_CMD_FLG_SEQ_CTL_MSK;
	}

<<<<<<< HEAD
	priv->cfg->ops->utils->tx_cmd_protection(priv, info, fc, &tx_flags);
=======
	iwlagn_tx_cmd_protection(priv, info, fc, &tx_flags);
>>>>>>> refs/remotes/origin/cm-10.0

	tx_flags &= ~(TX_CMD_FLG_ANT_SEL_MSK);
	if (ieee80211_is_mgmt(fc)) {
		if (ieee80211_is_assoc_req(fc) || ieee80211_is_reassoc_req(fc))
			tx_cmd->timeout.pm_frame_timeout = cpu_to_le16(3);
		else
			tx_cmd->timeout.pm_frame_timeout = cpu_to_le16(2);
	} else {
		tx_cmd->timeout.pm_frame_timeout = 0;
	}

	tx_cmd->driver_txop = 0;
	tx_cmd->tx_flags = tx_flags;
	tx_cmd->next_frame_len = 0;
}

<<<<<<< HEAD
#define RTS_DFAULT_RETRY_LIMIT		60

static void iwlagn_tx_cmd_build_rate(struct iwl_priv *priv,
			      struct iwl_tx_cmd *tx_cmd,
			      struct ieee80211_tx_info *info,
			      __le16 fc)
=======
static void iwlagn_tx_cmd_build_rate(struct iwl_priv *priv,
				     struct iwl_tx_cmd *tx_cmd,
				     struct ieee80211_tx_info *info,
				     __le16 fc)
>>>>>>> refs/remotes/origin/cm-10.0
{
	u32 rate_flags;
	int rate_idx;
	u8 rts_retry_limit;
	u8 data_retry_limit;
	u8 rate_plcp;

<<<<<<< HEAD
	/* Set retry limit on DATA packets and Probe Responses*/
	if (ieee80211_is_probe_resp(fc))
		data_retry_limit = 3;
	else
		data_retry_limit = IWLAGN_DEFAULT_TX_RETRY;
	tx_cmd->data_retry_limit = data_retry_limit;

	/* Set retry limit on RTS packets */
	rts_retry_limit = RTS_DFAULT_RETRY_LIMIT;
	if (data_retry_limit < rts_retry_limit)
		rts_retry_limit = data_retry_limit;
=======
	if (priv->wowlan) {
		rts_retry_limit = IWLAGN_LOW_RETRY_LIMIT;
		data_retry_limit = IWLAGN_LOW_RETRY_LIMIT;
	} else {
		/* Set retry limit on RTS packets */
		rts_retry_limit = IWLAGN_RTS_DFAULT_RETRY_LIMIT;

		/* Set retry limit on DATA packets and Probe Responses*/
		if (ieee80211_is_probe_resp(fc)) {
			data_retry_limit = IWLAGN_MGMT_DFAULT_RETRY_LIMIT;
			rts_retry_limit =
				min(data_retry_limit, rts_retry_limit);
		} else if (ieee80211_is_back_req(fc))
			data_retry_limit = IWLAGN_BAR_DFAULT_RETRY_LIMIT;
		else
			data_retry_limit = IWLAGN_DEFAULT_TX_RETRY;
	}

	tx_cmd->data_retry_limit = data_retry_limit;
>>>>>>> refs/remotes/origin/cm-10.0
	tx_cmd->rts_retry_limit = rts_retry_limit;

	/* DATA packets will use the uCode station table for rate/antenna
	 * selection */
	if (ieee80211_is_data(fc)) {
		tx_cmd->initial_rate_index = 0;
		tx_cmd->tx_flags |= TX_CMD_FLG_STA_RATE_MSK;
<<<<<<< HEAD
		return;
	}
=======
#ifdef CONFIG_IWLWIFI_DEVICE_TESTMODE
		if (priv->tm_fixed_rate) {
			/*
			 * rate overwrite by testmode
			 * we not only send lq command to change rate
			 * we also re-enforce per data pkt base.
			 */
			tx_cmd->tx_flags &= ~TX_CMD_FLG_STA_RATE_MSK;
			memcpy(&tx_cmd->rate_n_flags, &priv->tm_fixed_rate,
			       sizeof(tx_cmd->rate_n_flags));
		}
#endif
		return;
	} else if (ieee80211_is_back_req(fc))
		tx_cmd->tx_flags |= TX_CMD_FLG_STA_RATE_MSK;
>>>>>>> refs/remotes/origin/cm-10.0

	/**
	 * If the current TX rate stored in mac80211 has the MCS bit set, it's
	 * not really a TX rate.  Thus, we use the lowest supported rate for
	 * this band.  Also use the lowest supported rate if the stored rate
	 * index is invalid.
	 */
	rate_idx = info->control.rates[0].idx;
	if (info->control.rates[0].flags & IEEE80211_TX_RC_MCS ||
			(rate_idx < 0) || (rate_idx > IWL_RATE_COUNT_LEGACY))
		rate_idx = rate_lowest_index(&priv->bands[info->band],
				info->control.sta);
	/* For 5 GHZ band, remap mac80211 rate indices into driver indices */
	if (info->band == IEEE80211_BAND_5GHZ)
		rate_idx += IWL_FIRST_OFDM_RATE;
	/* Get PLCP rate for tx_cmd->rate_n_flags */
	rate_plcp = iwl_rates[rate_idx].plcp;
	/* Zero out flags for this packet */
	rate_flags = 0;

	/* Set CCK flag as needed */
	if ((rate_idx >= IWL_FIRST_CCK_RATE) && (rate_idx <= IWL_LAST_CCK_RATE))
		rate_flags |= RATE_MCS_CCK_MSK;

	/* Set up antennas */
<<<<<<< HEAD
	 if (priv->cfg->bt_params &&
	     priv->cfg->bt_params->advanced_bt_coexist &&
	     priv->bt_full_concurrent) {
		/* operated as 1x1 in full concurrency mode */
		priv->mgmt_tx_ant = iwl_toggle_tx_ant(priv, priv->mgmt_tx_ant,
				first_antenna(priv->hw_params.valid_tx_ant));
	} else
		priv->mgmt_tx_ant = iwl_toggle_tx_ant(priv, priv->mgmt_tx_ant,
					      priv->hw_params.valid_tx_ant);
=======
	 if (cfg(priv)->bt_params &&
	     cfg(priv)->bt_params->advanced_bt_coexist &&
	     priv->bt_full_concurrent) {
		/* operated as 1x1 in full concurrency mode */
		priv->mgmt_tx_ant = iwl_toggle_tx_ant(priv, priv->mgmt_tx_ant,
				first_antenna(hw_params(priv).valid_tx_ant));
	} else
		priv->mgmt_tx_ant = iwl_toggle_tx_ant(priv, priv->mgmt_tx_ant,
						hw_params(priv).valid_tx_ant);
>>>>>>> refs/remotes/origin/cm-10.0
	rate_flags |= iwl_ant_idx_to_flags(priv->mgmt_tx_ant);

	/* Set the rate in the TX cmd */
	tx_cmd->rate_n_flags = iwl_hw_set_rate_n_flags(rate_plcp, rate_flags);
}

static void iwlagn_tx_cmd_build_hwcrypto(struct iwl_priv *priv,
<<<<<<< HEAD
				      struct ieee80211_tx_info *info,
				      struct iwl_tx_cmd *tx_cmd,
				      struct sk_buff *skb_frag,
				      int sta_id)
=======
					 struct ieee80211_tx_info *info,
					 struct iwl_tx_cmd *tx_cmd,
					 struct sk_buff *skb_frag)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct ieee80211_key_conf *keyconf = info->control.hw_key;

	switch (keyconf->cipher) {
	case WLAN_CIPHER_SUITE_CCMP:
		tx_cmd->sec_ctl = TX_CMD_SEC_CCM;
		memcpy(tx_cmd->key, keyconf->key, keyconf->keylen);
		if (info->flags & IEEE80211_TX_CTL_AMPDU)
			tx_cmd->tx_flags |= TX_CMD_FLG_AGG_CCMP_MSK;
		IWL_DEBUG_TX(priv, "tx_cmd with AES hwcrypto\n");
		break;

	case WLAN_CIPHER_SUITE_TKIP:
		tx_cmd->sec_ctl = TX_CMD_SEC_TKIP;
<<<<<<< HEAD
		ieee80211_get_tkip_key(keyconf, skb_frag,
			IEEE80211_TKIP_P2_KEY, tx_cmd->key);
=======
		ieee80211_get_tkip_p2k(keyconf, skb_frag, tx_cmd->key);
>>>>>>> refs/remotes/origin/cm-10.0
		IWL_DEBUG_TX(priv, "tx_cmd with tkip hwcrypto\n");
		break;

	case WLAN_CIPHER_SUITE_WEP104:
		tx_cmd->sec_ctl |= TX_CMD_SEC_KEY128;
		/* fall through */
	case WLAN_CIPHER_SUITE_WEP40:
		tx_cmd->sec_ctl |= (TX_CMD_SEC_WEP |
			(keyconf->keyidx & TX_CMD_SEC_MSK) << TX_CMD_SEC_SHIFT);

		memcpy(&tx_cmd->key[3], keyconf->key, keyconf->keylen);

		IWL_DEBUG_TX(priv, "Configuring packet for WEP encryption "
			     "with key %d\n", keyconf->keyidx);
		break;

	default:
		IWL_ERR(priv, "Unknown encode cipher %x\n", keyconf->cipher);
		break;
	}
}

<<<<<<< HEAD
=======
/**
 * iwl_sta_id_or_broadcast - return sta_id or broadcast sta
 * @context: the current context
 * @sta: mac80211 station
 *
 * In certain circumstances mac80211 passes a station pointer
 * that may be %NULL, for example during TX or key setup. In
 * that case, we need to use the broadcast station, so this
 * inline wraps that pattern.
 */
static int iwl_sta_id_or_broadcast(struct iwl_rxon_context *context,
				   struct ieee80211_sta *sta)
{
	int sta_id;

	if (!sta)
		return context->bcast_sta_id;

	sta_id = iwl_sta_id(sta);

	/*
	 * mac80211 should not be passing a partially
	 * initialised station!
	 */
	WARN_ON(sta_id == IWL_INVALID_STATION);

	return sta_id;
}

>>>>>>> refs/remotes/origin/cm-10.0
/*
 * start REPLY_TX command process
 */
int iwlagn_tx_skb(struct iwl_priv *priv, struct sk_buff *skb)
{
	struct ieee80211_hdr *hdr = (struct ieee80211_hdr *)skb->data;
	struct ieee80211_tx_info *info = IEEE80211_SKB_CB(skb);
<<<<<<< HEAD
	struct ieee80211_sta *sta = info->control.sta;
	struct iwl_station_priv *sta_priv = NULL;
	struct iwl_tx_queue *txq;
	struct iwl_queue *q;
	struct iwl_device_cmd *out_cmd;
	struct iwl_cmd_meta *out_meta;
	struct iwl_tx_cmd *tx_cmd;
	struct iwl_rxon_context *ctx = &priv->contexts[IWL_RXON_CTX_BSS];
	int txq_id;
	dma_addr_t phys_addr = 0;
	dma_addr_t txcmd_phys;
	dma_addr_t scratch_phys;
	u16 len, firstlen, secondlen;
	u16 seq_number = 0;
	__le16 fc;
	u8 hdr_len;
	u8 sta_id;
	u8 wait_write_ptr = 0;
	u8 tid = 0;
	u8 *qc = NULL;
	unsigned long flags;
	bool is_agg = false;

	/*
	 * If the frame needs to go out off-channel, then
	 * we'll have put the PAN context to that channel,
	 * so make the frame go out there.
	 */
	if (info->flags & IEEE80211_TX_CTL_TX_OFFCHAN)
		ctx = &priv->contexts[IWL_RXON_CTX_PAN];
	else if (info->control.vif)
		ctx = iwl_rxon_ctx_from_vif(info->control.vif);

	spin_lock_irqsave(&priv->lock, flags);
=======
	struct iwl_station_priv *sta_priv = NULL;
	struct iwl_rxon_context *ctx = &priv->contexts[IWL_RXON_CTX_BSS];
	struct iwl_device_cmd *dev_cmd = NULL;
	struct iwl_tx_cmd *tx_cmd;
	__le16 fc;
	u8 hdr_len;
	u16 len, seq_number = 0;
	u8 sta_id, tid = IWL_MAX_TID_COUNT;
	bool is_agg = false;

	if (info->control.vif)
		ctx = iwl_rxon_ctx_from_vif(info->control.vif);

>>>>>>> refs/remotes/origin/cm-10.0
	if (iwl_is_rfkill(priv)) {
		IWL_DEBUG_DROP(priv, "Dropping - RF KILL\n");
		goto drop_unlock_priv;
	}

	fc = hdr->frame_control;

#ifdef CONFIG_IWLWIFI_DEBUG
	if (ieee80211_is_auth(fc))
		IWL_DEBUG_TX(priv, "Sending AUTH frame\n");
	else if (ieee80211_is_assoc_req(fc))
		IWL_DEBUG_TX(priv, "Sending ASSOC frame\n");
	else if (ieee80211_is_reassoc_req(fc))
		IWL_DEBUG_TX(priv, "Sending REASSOC frame\n");
#endif

<<<<<<< HEAD
=======
	if (unlikely(ieee80211_is_probe_resp(fc))) {
		struct iwl_wipan_noa_data *noa_data =
			rcu_dereference(priv->noa_data);

		if (noa_data &&
		    pskb_expand_head(skb, 0, noa_data->length,
				     GFP_ATOMIC) == 0) {
			memcpy(skb_put(skb, noa_data->length),
			       noa_data->data, noa_data->length);
			hdr = (struct ieee80211_hdr *)skb->data;
		}
	}

>>>>>>> refs/remotes/origin/cm-10.0
	hdr_len = ieee80211_hdrlen(fc);

	/* For management frames use broadcast id to do not break aggregation */
	if (!ieee80211_is_data(fc))
		sta_id = ctx->bcast_sta_id;
	else {
		/* Find index into station table for destination station */
<<<<<<< HEAD
		sta_id = iwl_sta_id_or_broadcast(priv, ctx, info->control.sta);
=======
		sta_id = iwl_sta_id_or_broadcast(ctx, info->control.sta);
>>>>>>> refs/remotes/origin/cm-10.0
		if (sta_id == IWL_INVALID_STATION) {
			IWL_DEBUG_DROP(priv, "Dropping - INVALID STATION: %pM\n",
				       hdr->addr1);
			goto drop_unlock_priv;
		}
	}

	IWL_DEBUG_TX(priv, "station Id %d\n", sta_id);

<<<<<<< HEAD
	if (sta)
		sta_priv = (void *)sta->drv_priv;

	if (sta_priv && sta_priv->asleep &&
	    (info->flags & IEEE80211_TX_CTL_PSPOLL_RESPONSE)) {
=======
	if (info->control.sta)
		sta_priv = (void *)info->control.sta->drv_priv;

	if (sta_priv && sta_priv->asleep &&
	    (info->flags & IEEE80211_TX_CTL_NO_PS_BUFFER)) {
>>>>>>> refs/remotes/origin/cm-10.0
		/*
		 * This sends an asynchronous command to the device,
		 * but we can rely on it being processed before the
		 * next frame is processed -- and the next frame to
		 * this station is the one that will consume this
		 * counter.
		 * For now set the counter to just 1 since we do not
		 * support uAPSD yet.
<<<<<<< HEAD
=======
		 *
		 * FIXME: If we get two non-bufferable frames one
		 * after the other, we might only send out one of
		 * them because this is racy.
>>>>>>> refs/remotes/origin/cm-10.0
		 */
		iwl_sta_modify_sleep_tx_count(priv, sta_id, 1);
	}

<<<<<<< HEAD
	/*
	 * Send this frame after DTIM -- there's a special queue
	 * reserved for this for contexts that support AP mode.
	 */
	if (info->flags & IEEE80211_TX_CTL_SEND_AFTER_DTIM) {
		txq_id = ctx->mcast_queue;
		/*
		 * The microcode will clear the more data
		 * bit in the last frame it transmits.
		 */
		hdr->frame_control |=
			cpu_to_le16(IEEE80211_FCTL_MOREDATA);
	} else
		txq_id = ctx->ac_to_queue[skb_get_queue_mapping(skb)];

	/* irqs already disabled/saved above when locking priv->lock */
	spin_lock(&priv->sta_lock);

	if (ieee80211_is_data_qos(fc)) {
		qc = ieee80211_get_qos_ctl(hdr);
		tid = qc[0] & IEEE80211_QOS_CTL_TID_MASK;

		if (WARN_ON_ONCE(tid >= MAX_TID_COUNT))
			goto drop_unlock_sta;

		seq_number = priv->stations[sta_id].tid[tid].seq_number;
		seq_number &= IEEE80211_SCTL_SEQ;
		hdr->seq_ctrl = hdr->seq_ctrl &
				cpu_to_le16(IEEE80211_SCTL_FRAG);
		hdr->seq_ctrl |= cpu_to_le16(seq_number);
		seq_number += 0x10;
		/* aggregation is on for this <sta,tid> */
		if (info->flags & IEEE80211_TX_CTL_AMPDU &&
		    priv->stations[sta_id].tid[tid].agg.state == IWL_AGG_ON) {
			txq_id = priv->stations[sta_id].tid[tid].agg.txq_id;
			is_agg = true;
		}
	}

	txq = &priv->txq[txq_id];
	q = &txq->q;

	if (unlikely(iwl_queue_space(q) < q->high_mark))
		goto drop_unlock_sta;

	/* Set up driver data for this TFD */
	memset(&(txq->txb[q->write_ptr]), 0, sizeof(struct iwl_tx_info));
	txq->txb[q->write_ptr].skb = skb;
	txq->txb[q->write_ptr].ctx = ctx;

	/* Set up first empty entry in queue's array of Tx/cmd buffers */
	out_cmd = txq->cmd[q->write_ptr];
	out_meta = &txq->meta[q->write_ptr];
	tx_cmd = &out_cmd->cmd.tx;
	memset(&out_cmd->hdr, 0, sizeof(out_cmd->hdr));
	memset(tx_cmd, 0, sizeof(struct iwl_tx_cmd));

	/*
	 * Set up the Tx-command (not MAC!) header.
	 * Store the chosen Tx queue and TFD index within the sequence field;
	 * after Tx, uCode's Tx response will return this value so driver can
	 * locate the frame within the tx queue and do post-tx processing.
	 */
	out_cmd->hdr.cmd = REPLY_TX;
	out_cmd->hdr.sequence = cpu_to_le16((u16)(QUEUE_TO_SEQ(txq_id) |
				INDEX_TO_SEQ(q->write_ptr)));

	/* Copy MAC header from skb into command buffer */
	memcpy(tx_cmd->hdr, hdr, hdr_len);

=======
	if (info->flags & IEEE80211_TX_CTL_AMPDU)
		is_agg = true;

	dev_cmd = kmem_cache_alloc(iwl_tx_cmd_pool, GFP_ATOMIC);

	if (unlikely(!dev_cmd))
		goto drop_unlock_priv;

	memset(dev_cmd, 0, sizeof(*dev_cmd));
	tx_cmd = (struct iwl_tx_cmd *) dev_cmd->payload;
>>>>>>> refs/remotes/origin/cm-10.0

	/* Total # bytes to be transmitted */
	len = (u16)skb->len;
	tx_cmd->len = cpu_to_le16(len);

	if (info->control.hw_key)
<<<<<<< HEAD
		iwlagn_tx_cmd_build_hwcrypto(priv, info, tx_cmd, skb, sta_id);
=======
		iwlagn_tx_cmd_build_hwcrypto(priv, info, tx_cmd, skb);
>>>>>>> refs/remotes/origin/cm-10.0

	/* TODO need this for burst mode later on */
	iwlagn_tx_cmd_build_basic(priv, skb, tx_cmd, info, hdr, sta_id);
	iwl_dbg_log_tx_data_frame(priv, len, hdr);

	iwlagn_tx_cmd_build_rate(priv, tx_cmd, info, fc);

	iwl_update_stats(priv, true, fc, len);
<<<<<<< HEAD
	/*
	 * Use the first empty entry in this queue's command buffer array
	 * to contain the Tx command and MAC header concatenated together
	 * (payload data will be in another buffer).
	 * Size of this varies, due to varying MAC header length.
	 * If end is not dword aligned, we'll have 2 extra bytes at the end
	 * of the MAC header (device reads on dword boundaries).
	 * We'll tell device about this padding later.
	 */
	len = sizeof(struct iwl_tx_cmd) +
		sizeof(struct iwl_cmd_header) + hdr_len;
	firstlen = (len + 3) & ~3;

	/* Tell NIC about any 2-byte padding after MAC header */
	if (firstlen != len)
		tx_cmd->tx_flags |= TX_CMD_FLG_MH_PAD_MSK;

	/* Physical address of this Tx command's header (not MAC header!),
	 * within command buffer array. */
	txcmd_phys = pci_map_single(priv->pci_dev,
				    &out_cmd->hdr, firstlen,
				    PCI_DMA_BIDIRECTIONAL);
	if (unlikely(pci_dma_mapping_error(priv->pci_dev, txcmd_phys)))
		goto drop_unlock_sta;
	dma_unmap_addr_set(out_meta, mapping, txcmd_phys);
	dma_unmap_len_set(out_meta, len, firstlen);

	if (!ieee80211_has_morefrags(hdr->frame_control)) {
		txq->need_update = 1;
	} else {
		wait_write_ptr = 1;
		txq->need_update = 0;
	}

	/* Set up TFD's 2nd entry to point directly to remainder of skb,
	 * if any (802.11 null frames have no payload). */
	secondlen = skb->len - hdr_len;
	if (secondlen > 0) {
		phys_addr = pci_map_single(priv->pci_dev, skb->data + hdr_len,
					   secondlen, PCI_DMA_TODEVICE);
		if (unlikely(pci_dma_mapping_error(priv->pci_dev, phys_addr))) {
			pci_unmap_single(priv->pci_dev,
					 dma_unmap_addr(out_meta, mapping),
					 dma_unmap_len(out_meta, len),
					 PCI_DMA_BIDIRECTIONAL);
			goto drop_unlock_sta;
		}
	}

	if (ieee80211_is_data_qos(fc)) {
		priv->stations[sta_id].tid[tid].tfds_in_queue++;
		if (!ieee80211_has_morefrags(fc))
			priv->stations[sta_id].tid[tid].seq_number = seq_number;
	}

	spin_unlock(&priv->sta_lock);

	/* Attach buffers to TFD */
	iwlagn_txq_attach_buf_to_tfd(priv, txq, txcmd_phys, firstlen, 1);
	if (secondlen > 0)
		iwlagn_txq_attach_buf_to_tfd(priv, txq, phys_addr,
					     secondlen, 0);

	scratch_phys = txcmd_phys + sizeof(struct iwl_cmd_header) +
				offsetof(struct iwl_tx_cmd, scratch);

	/* take back ownership of DMA buffer to enable update */
	pci_dma_sync_single_for_cpu(priv->pci_dev, txcmd_phys,
				    firstlen, PCI_DMA_BIDIRECTIONAL);
	tx_cmd->dram_lsb_ptr = cpu_to_le32(scratch_phys);
	tx_cmd->dram_msb_ptr = iwl_get_dma_hi_addr(scratch_phys);

	IWL_DEBUG_TX(priv, "sequence nr = 0X%x\n",
		     le16_to_cpu(out_cmd->hdr.sequence));
	IWL_DEBUG_TX(priv, "tx_flags = 0X%x\n", le32_to_cpu(tx_cmd->tx_flags));
	iwl_print_hex_dump(priv, IWL_DL_TX, (u8 *)tx_cmd, sizeof(*tx_cmd));
	iwl_print_hex_dump(priv, IWL_DL_TX, (u8 *)tx_cmd->hdr, hdr_len);

	iwlagn_txq_update_byte_cnt_tbl(priv, txq, le16_to_cpu(tx_cmd->len));

	pci_dma_sync_single_for_device(priv->pci_dev, txcmd_phys,
				       firstlen, PCI_DMA_BIDIRECTIONAL);

	trace_iwlwifi_dev_tx(priv,
			     &((struct iwl_tfd *)txq->tfds)[txq->q.write_ptr],
			     sizeof(struct iwl_tfd),
			     &out_cmd->hdr, firstlen,
			     skb->data + hdr_len, secondlen);

	/* Tell device the write index *just past* this latest filled TFD */
	q->write_ptr = iwl_queue_inc_wrap(q->write_ptr, q->n_bd);
	iwl_txq_update_write_ptr(priv, txq);
	spin_unlock_irqrestore(&priv->lock, flags);

	/*
	 * At this point the frame is "transmitted" successfully
	 * and we will get a TX status notification eventually,
	 * regardless of the value of ret. "ret" only indicates
	 * whether or not we should update the write pointer.
	 */
=======

	memset(&info->status, 0, sizeof(info->status));

	info->driver_data[0] = ctx;
	info->driver_data[1] = dev_cmd;

	spin_lock(&priv->sta_lock);

	if (ieee80211_is_data_qos(fc) && !ieee80211_is_qos_nullfunc(fc)) {
		u8 *qc = NULL;
		struct iwl_tid_data *tid_data;
		qc = ieee80211_get_qos_ctl(hdr);
		tid = qc[0] & IEEE80211_QOS_CTL_TID_MASK;
		if (WARN_ON_ONCE(tid >= IWL_MAX_TID_COUNT))
			goto drop_unlock_sta;
		tid_data = &priv->tid_data[sta_id][tid];

		/* aggregation is on for this <sta,tid> */
		if (info->flags & IEEE80211_TX_CTL_AMPDU &&
		    tid_data->agg.state != IWL_AGG_ON) {
			IWL_ERR(priv, "TX_CTL_AMPDU while not in AGG:"
				" Tx flags = 0x%08x, agg.state = %d",
				info->flags, tid_data->agg.state);
			IWL_ERR(priv, "sta_id = %d, tid = %d seq_num = %d",
				sta_id, tid, SEQ_TO_SN(tid_data->seq_number));
			goto drop_unlock_sta;
		}

		/* We can receive packets from the stack in IWL_AGG_{ON,OFF}
		 * only. Check this here.
		 */
		if (WARN_ONCE(tid_data->agg.state != IWL_AGG_ON &&
		    tid_data->agg.state != IWL_AGG_OFF,
		    "Tx while agg.state = %d", tid_data->agg.state))
			goto drop_unlock_sta;

		seq_number = tid_data->seq_number;
		seq_number &= IEEE80211_SCTL_SEQ;
		hdr->seq_ctrl &= cpu_to_le16(IEEE80211_SCTL_FRAG);
		hdr->seq_ctrl |= cpu_to_le16(seq_number);
		seq_number += 0x10;
	}

	/* Copy MAC header from skb into command buffer */
	memcpy(tx_cmd->hdr, hdr, hdr_len);

	if (iwl_trans_tx(trans(priv), skb, dev_cmd, ctx->ctxid, sta_id, tid))
		goto drop_unlock_sta;

	if (ieee80211_is_data_qos(fc) && !ieee80211_is_qos_nullfunc(fc) &&
	    !ieee80211_has_morefrags(fc))
		priv->tid_data[sta_id][tid].seq_number = seq_number;

	spin_unlock(&priv->sta_lock);
>>>>>>> refs/remotes/origin/cm-10.0

	/*
	 * Avoid atomic ops if it isn't an associated client.
	 * Also, if this is a packet for aggregation, don't
	 * increase the counter because the ucode will stop
	 * aggregation queues when their respective station
	 * goes to sleep.
	 */
	if (sta_priv && sta_priv->client && !is_agg)
		atomic_inc(&sta_priv->pending_frames);

<<<<<<< HEAD
	if ((iwl_queue_space(q) < q->high_mark) && priv->mac80211_registered) {
		if (wait_write_ptr) {
			spin_lock_irqsave(&priv->lock, flags);
			txq->need_update = 1;
			iwl_txq_update_write_ptr(priv, txq);
			spin_unlock_irqrestore(&priv->lock, flags);
		} else {
			iwl_stop_queue(priv, txq);
		}
	}

	return 0;

drop_unlock_sta:
	spin_unlock(&priv->sta_lock);
drop_unlock_priv:
	spin_unlock_irqrestore(&priv->lock, flags);
	return -1;
}

static inline int iwlagn_alloc_dma_ptr(struct iwl_priv *priv,
				    struct iwl_dma_ptr *ptr, size_t size)
{
	ptr->addr = dma_alloc_coherent(&priv->pci_dev->dev, size, &ptr->dma,
				       GFP_KERNEL);
	if (!ptr->addr)
		return -ENOMEM;
	ptr->size = size;
	return 0;
}

static inline void iwlagn_free_dma_ptr(struct iwl_priv *priv,
				    struct iwl_dma_ptr *ptr)
{
	if (unlikely(!ptr->addr))
		return;

	dma_free_coherent(&priv->pci_dev->dev, ptr->size, ptr->addr, ptr->dma);
	memset(ptr, 0, sizeof(*ptr));
}

/**
 * iwlagn_hw_txq_ctx_free - Free TXQ Context
 *
 * Destroy all TX DMA queues and structures
 */
void iwlagn_hw_txq_ctx_free(struct iwl_priv *priv)
{
	int txq_id;

	/* Tx queues */
	if (priv->txq) {
		for (txq_id = 0; txq_id < priv->hw_params.max_txq_num; txq_id++)
			if (txq_id == priv->cmd_queue)
				iwl_cmd_queue_free(priv);
			else
				iwl_tx_queue_free(priv, txq_id);
	}
	iwlagn_free_dma_ptr(priv, &priv->kw);

	iwlagn_free_dma_ptr(priv, &priv->scd_bc_tbls);

	/* free tx queue structure */
	iwl_free_txq_mem(priv);
}

/**
 * iwlagn_txq_ctx_alloc - allocate TX queue context
 * Allocate all Tx DMA structures and initialize them
 *
 * @param priv
 * @return error code
 */
int iwlagn_txq_ctx_alloc(struct iwl_priv *priv)
{
	int ret;
	int txq_id, slots_num;
	unsigned long flags;

	/* Free all tx/cmd queues and keep-warm buffer */
	iwlagn_hw_txq_ctx_free(priv);

	ret = iwlagn_alloc_dma_ptr(priv, &priv->scd_bc_tbls,
				priv->hw_params.scd_bc_tbls_size);
	if (ret) {
		IWL_ERR(priv, "Scheduler BC Table allocation failed\n");
		goto error_bc_tbls;
	}
	/* Alloc keep-warm buffer */
	ret = iwlagn_alloc_dma_ptr(priv, &priv->kw, IWL_KW_SIZE);
	if (ret) {
		IWL_ERR(priv, "Keep Warm allocation failed\n");
		goto error_kw;
	}

	/* allocate tx queue structure */
	ret = iwl_alloc_txq_mem(priv);
	if (ret)
		goto error;

	spin_lock_irqsave(&priv->lock, flags);

	/* Turn off all Tx DMA fifos */
	iwlagn_txq_set_sched(priv, 0);

	/* Tell NIC where to find the "keep warm" buffer */
	iwl_write_direct32(priv, FH_KW_MEM_ADDR_REG, priv->kw.dma >> 4);

	spin_unlock_irqrestore(&priv->lock, flags);

	/* Alloc and init all Tx queues, including the command queue (#4/#9) */
	for (txq_id = 0; txq_id < priv->hw_params.max_txq_num; txq_id++) {
		slots_num = (txq_id == priv->cmd_queue) ?
					TFD_CMD_SLOTS : TFD_TX_CMD_SLOTS;
		ret = iwl_tx_queue_init(priv, &priv->txq[txq_id], slots_num,
				       txq_id);
		if (ret) {
			IWL_ERR(priv, "Tx %d queue init failed\n", txq_id);
			goto error;
		}
	}

	return ret;

 error:
	iwlagn_hw_txq_ctx_free(priv);
	iwlagn_free_dma_ptr(priv, &priv->kw);
 error_kw:
	iwlagn_free_dma_ptr(priv, &priv->scd_bc_tbls);
 error_bc_tbls:
	return ret;
}

void iwlagn_txq_ctx_reset(struct iwl_priv *priv)
{
	int txq_id, slots_num;
	unsigned long flags;

	spin_lock_irqsave(&priv->lock, flags);

	/* Turn off all Tx DMA fifos */
	iwlagn_txq_set_sched(priv, 0);

	/* Tell NIC where to find the "keep warm" buffer */
	iwl_write_direct32(priv, FH_KW_MEM_ADDR_REG, priv->kw.dma >> 4);

	spin_unlock_irqrestore(&priv->lock, flags);

	/* Alloc and init all Tx queues, including the command queue (#4) */
	for (txq_id = 0; txq_id < priv->hw_params.max_txq_num; txq_id++) {
		slots_num = txq_id == priv->cmd_queue ?
			    TFD_CMD_SLOTS : TFD_TX_CMD_SLOTS;
		iwl_tx_queue_reset(priv, &priv->txq[txq_id], slots_num, txq_id);
	}
}

/**
 * iwlagn_txq_ctx_stop - Stop all Tx DMA channels
 */
void iwlagn_txq_ctx_stop(struct iwl_priv *priv)
{
	int ch, txq_id;
	unsigned long flags;

	/* Turn off all Tx DMA fifos */
	spin_lock_irqsave(&priv->lock, flags);

	iwlagn_txq_set_sched(priv, 0);

	/* Stop each Tx DMA channel, and wait for it to be idle */
	for (ch = 0; ch < priv->hw_params.dma_chnl_num; ch++) {
		iwl_write_direct32(priv, FH_TCSR_CHNL_TX_CONFIG_REG(ch), 0x0);
		if (iwl_poll_direct_bit(priv, FH_TSSR_TX_STATUS_REG,
				    FH_TSSR_TX_STATUS_REG_MSK_CHNL_IDLE(ch),
				    1000))
			IWL_ERR(priv, "Failing on timeout while stopping"
			    " DMA channel %d [0x%08x]", ch,
			    iwl_read_direct32(priv, FH_TSSR_TX_STATUS_REG));
	}
	spin_unlock_irqrestore(&priv->lock, flags);

	if (!priv->txq)
		return;

	/* Unmap DMA from host system and free skb's */
	for (txq_id = 0; txq_id < priv->hw_params.max_txq_num; txq_id++)
		if (txq_id == priv->cmd_queue)
			iwl_cmd_queue_unmap(priv);
		else
			iwl_tx_queue_unmap(priv, txq_id);
}

/*
 * Find first available (lowest unused) Tx Queue, mark it "active".
 * Called only when finding queue for aggregation.
 * Should never return anything < 7, because they should already
 * be in use as EDCA AC (0-3), Command (4), reserved (5, 6)
 */
static int iwlagn_txq_ctx_activate_free(struct iwl_priv *priv)
{
	int txq_id;

	for (txq_id = 0; txq_id < priv->hw_params.max_txq_num; txq_id++)
		if (!test_and_set_bit(txq_id, &priv->txq_ctx_active_msk))
			return txq_id;
	return -1;
}
=======
	return 0;

drop_unlock_sta:
	if (dev_cmd)
		kmem_cache_free(iwl_tx_cmd_pool, dev_cmd);
	spin_unlock(&priv->sta_lock);
drop_unlock_priv:
	return -1;
}

int iwlagn_tx_agg_stop(struct iwl_priv *priv, struct ieee80211_vif *vif,
			struct ieee80211_sta *sta, u16 tid)
{
	struct iwl_tid_data *tid_data;
	int sta_id;

	sta_id = iwl_sta_id(sta);

	if (sta_id == IWL_INVALID_STATION) {
		IWL_ERR(priv, "Invalid station for AGG tid %d\n", tid);
		return -ENXIO;
	}

	spin_lock_bh(&priv->sta_lock);

	tid_data = &priv->tid_data[sta_id][tid];

	switch (priv->tid_data[sta_id][tid].agg.state) {
	case IWL_EMPTYING_HW_QUEUE_ADDBA:
		/*
		* This can happen if the peer stops aggregation
		* again before we've had a chance to drain the
		* queue we selected previously, i.e. before the
		* session was really started completely.
		*/
		IWL_DEBUG_HT(priv, "AGG stop before setup done\n");
		goto turn_off;
	case IWL_AGG_ON:
		break;
	default:
		IWL_WARN(priv, "Stopping AGG while state not ON "
			 "or starting for %d on %d (%d)\n", sta_id, tid,
			 priv->tid_data[sta_id][tid].agg.state);
		spin_unlock_bh(&priv->sta_lock);
		return 0;
	}

	tid_data->agg.ssn = SEQ_TO_SN(tid_data->seq_number);

	/* There are still packets for this RA / TID in the HW */
	if (tid_data->agg.ssn != tid_data->next_reclaimed) {
		IWL_DEBUG_TX_QUEUES(priv, "Can't proceed: ssn %d, "
				    "next_recl = %d",
				    tid_data->agg.ssn,
				    tid_data->next_reclaimed);
		priv->tid_data[sta_id][tid].agg.state =
			IWL_EMPTYING_HW_QUEUE_DELBA;
		spin_unlock_bh(&priv->sta_lock);
		return 0;
	}

	IWL_DEBUG_TX_QUEUES(priv, "Can proceed: ssn = next_recl = %d",
			    tid_data->agg.ssn);
turn_off:
	priv->tid_data[sta_id][tid].agg.state = IWL_AGG_OFF;

	spin_unlock_bh(&priv->sta_lock);

	iwl_trans_tx_agg_disable(trans(priv), sta_id, tid);

	ieee80211_stop_tx_ba_cb_irqsafe(vif, sta->addr, tid);

	return 0;
}
>>>>>>> refs/remotes/origin/cm-10.0

int iwlagn_tx_agg_start(struct iwl_priv *priv, struct ieee80211_vif *vif,
			struct ieee80211_sta *sta, u16 tid, u16 *ssn)
{
<<<<<<< HEAD
	int sta_id;
	int tx_fifo;
	int txq_id;
	int ret;
	unsigned long flags;
	struct iwl_tid_data *tid_data;

	tx_fifo = get_fifo_from_tid(iwl_rxon_ctx_from_vif(vif), tid);
	if (unlikely(tx_fifo < 0))
		return tx_fifo;

	IWL_WARN(priv, "%s on ra = %pM tid = %d\n",
			__func__, sta->addr, tid);
=======
	struct iwl_tid_data *tid_data;
	int sta_id;
	int ret;

	IWL_DEBUG_HT(priv, "TX AGG request on ra = %pM tid = %d\n",
		     sta->addr, tid);
>>>>>>> refs/remotes/origin/cm-10.0

	sta_id = iwl_sta_id(sta);
	if (sta_id == IWL_INVALID_STATION) {
		IWL_ERR(priv, "Start AGG on invalid station\n");
		return -ENXIO;
	}
<<<<<<< HEAD
	if (unlikely(tid >= MAX_TID_COUNT))
		return -EINVAL;

	if (priv->stations[sta_id].tid[tid].agg.state != IWL_AGG_OFF) {
=======
	if (unlikely(tid >= IWL_MAX_TID_COUNT))
		return -EINVAL;

	if (priv->tid_data[sta_id][tid].agg.state != IWL_AGG_OFF) {
>>>>>>> refs/remotes/origin/cm-10.0
		IWL_ERR(priv, "Start AGG when state is not IWL_AGG_OFF !\n");
		return -ENXIO;
	}

<<<<<<< HEAD
	txq_id = iwlagn_txq_ctx_activate_free(priv);
	if (txq_id == -1) {
		IWL_ERR(priv, "No free aggregation queue available\n");
		return -ENXIO;
	}

	spin_lock_irqsave(&priv->sta_lock, flags);
	tid_data = &priv->stations[sta_id].tid[tid];
	*ssn = SEQ_TO_SN(tid_data->seq_number);
	tid_data->agg.txq_id = txq_id;
	tid_data->agg.tx_fifo = tx_fifo;
	iwl_set_swq_id(&priv->txq[txq_id], get_ac_from_tid(tid), txq_id);
	spin_unlock_irqrestore(&priv->sta_lock, flags);

	ret = iwlagn_txq_agg_enable(priv, txq_id, sta_id, tid);
	if (ret)
		return ret;

	spin_lock_irqsave(&priv->sta_lock, flags);
	tid_data = &priv->stations[sta_id].tid[tid];
	if (tid_data->tfds_in_queue == 0) {
		IWL_DEBUG_HT(priv, "HW queue is empty\n");
		tid_data->agg.state = IWL_AGG_ON;
		ieee80211_start_tx_ba_cb_irqsafe(vif, sta->addr, tid);
	} else {
		IWL_DEBUG_HT(priv, "HW queue is NOT empty: %d packets in HW queue\n",
			     tid_data->tfds_in_queue);
		tid_data->agg.state = IWL_EMPTYING_HW_QUEUE_ADDBA;
	}
	spin_unlock_irqrestore(&priv->sta_lock, flags);
	return ret;
}

int iwlagn_tx_agg_stop(struct iwl_priv *priv, struct ieee80211_vif *vif,
		       struct ieee80211_sta *sta, u16 tid)
{
	int tx_fifo_id, txq_id, sta_id, ssn;
	struct iwl_tid_data *tid_data;
	int write_ptr, read_ptr;
	unsigned long flags;

	tx_fifo_id = get_fifo_from_tid(iwl_rxon_ctx_from_vif(vif), tid);
	if (unlikely(tx_fifo_id < 0))
		return tx_fifo_id;

	sta_id = iwl_sta_id(sta);

	if (sta_id == IWL_INVALID_STATION) {
		IWL_ERR(priv, "Invalid station for AGG tid %d\n", tid);
		return -ENXIO;
	}

	spin_lock_irqsave(&priv->sta_lock, flags);

	tid_data = &priv->stations[sta_id].tid[tid];
	ssn = (tid_data->seq_number & IEEE80211_SCTL_SEQ) >> 4;
	txq_id = tid_data->agg.txq_id;

	switch (priv->stations[sta_id].tid[tid].agg.state) {
	case IWL_EMPTYING_HW_QUEUE_ADDBA:
		/*
		 * This can happen if the peer stops aggregation
		 * again before we've had a chance to drain the
		 * queue we selected previously, i.e. before the
		 * session was really started completely.
		 */
		IWL_DEBUG_HT(priv, "AGG stop before setup done\n");
		goto turn_off;
	case IWL_AGG_ON:
		break;
	default:
		IWL_WARN(priv, "Stopping AGG while state not ON or starting\n");
	}

	write_ptr = priv->txq[txq_id].q.write_ptr;
	read_ptr = priv->txq[txq_id].q.read_ptr;

	/* The queue is not empty */
	if (write_ptr != read_ptr) {
		IWL_DEBUG_HT(priv, "Stopping a non empty AGG HW QUEUE\n");
		priv->stations[sta_id].tid[tid].agg.state =
				IWL_EMPTYING_HW_QUEUE_DELBA;
		spin_unlock_irqrestore(&priv->sta_lock, flags);
		return 0;
	}

	IWL_DEBUG_HT(priv, "HW queue is empty\n");
 turn_off:
	priv->stations[sta_id].tid[tid].agg.state = IWL_AGG_OFF;

	/* do not restore/save irqs */
	spin_unlock(&priv->sta_lock);
	spin_lock(&priv->lock);

	/*
	 * the only reason this call can fail is queue number out of range,
	 * which can happen if uCode is reloaded and all the station
	 * information are lost. if it is outside the range, there is no need
	 * to deactivate the uCode queue, just return "success" to allow
	 *  mac80211 to clean up it own data.
	 */
	iwlagn_txq_agg_disable(priv, txq_id, ssn, tx_fifo_id);
	spin_unlock_irqrestore(&priv->lock, flags);

	ieee80211_stop_tx_ba_cb_irqsafe(vif, sta->addr, tid);

	return 0;
}

int iwlagn_txq_check_empty(struct iwl_priv *priv,
			   int sta_id, u8 tid, int txq_id)
{
	struct iwl_queue *q = &priv->txq[txq_id].q;
	u8 *addr = priv->stations[sta_id].sta.sta.addr;
	struct iwl_tid_data *tid_data = &priv->stations[sta_id].tid[tid];
	struct iwl_rxon_context *ctx;

	ctx = &priv->contexts[priv->stations[sta_id].ctxid];

	lockdep_assert_held(&priv->sta_lock);

	switch (priv->stations[sta_id].tid[tid].agg.state) {
	case IWL_EMPTYING_HW_QUEUE_DELBA:
		/* We are reclaiming the last packet of the */
		/* aggregated HW queue */
		if ((txq_id  == tid_data->agg.txq_id) &&
		    (q->read_ptr == q->write_ptr)) {
			u16 ssn = SEQ_TO_SN(tid_data->seq_number);
			int tx_fifo = get_fifo_from_tid(ctx, tid);
			IWL_DEBUG_HT(priv, "HW queue empty: continue DELBA flow\n");
			iwlagn_txq_agg_disable(priv, txq_id, ssn, tx_fifo);
			tid_data->agg.state = IWL_AGG_OFF;
			ieee80211_stop_tx_ba_cb_irqsafe(ctx->vif, addr, tid);
		}
		break;
	case IWL_EMPTYING_HW_QUEUE_ADDBA:
		/* We are reclaiming the last packet of the queue */
		if (tid_data->tfds_in_queue == 0) {
			IWL_DEBUG_HT(priv, "HW queue empty: continue ADDBA flow\n");
			tid_data->agg.state = IWL_AGG_ON;
			ieee80211_start_tx_ba_cb_irqsafe(ctx->vif, addr, tid);
		}
		break;
	}

	return 0;
=======
	ret = iwl_sta_tx_modify_enable_tid(priv, sta_id, tid);
	if (ret)
		return ret;

	spin_lock_bh(&priv->sta_lock);

	tid_data = &priv->tid_data[sta_id][tid];
	tid_data->agg.ssn = SEQ_TO_SN(tid_data->seq_number);

	*ssn = tid_data->agg.ssn;

	ret = iwl_trans_tx_agg_alloc(trans(priv), sta_id, tid);
	if (ret) {
		spin_unlock_bh(&priv->sta_lock);
		return ret;
	}

	if (*ssn == tid_data->next_reclaimed) {
		IWL_DEBUG_TX_QUEUES(priv, "Can proceed: ssn = next_recl = %d",
				    tid_data->agg.ssn);
		tid_data->agg.state = IWL_AGG_ON;
		ieee80211_start_tx_ba_cb_irqsafe(vif, sta->addr, tid);
	} else {
		IWL_DEBUG_TX_QUEUES(priv, "Can't proceed: ssn %d, "
				    "next_reclaimed = %d",
				    tid_data->agg.ssn,
				    tid_data->next_reclaimed);
		tid_data->agg.state = IWL_EMPTYING_HW_QUEUE_ADDBA;
	}

	spin_unlock_bh(&priv->sta_lock);

	return ret;
}

int iwlagn_tx_agg_oper(struct iwl_priv *priv, struct ieee80211_vif *vif,
			struct ieee80211_sta *sta, u16 tid, u8 buf_size)
{
	struct iwl_station_priv *sta_priv = (void *) sta->drv_priv;
	struct iwl_rxon_context *ctx = iwl_rxon_ctx_from_vif(vif);
	u16 ssn;

	buf_size = min_t(int, buf_size, LINK_QUAL_AGG_FRAME_LIMIT_DEF);

	spin_lock_bh(&priv->sta_lock);
	ssn = priv->tid_data[sta_priv->sta_id][tid].agg.ssn;
	spin_unlock_bh(&priv->sta_lock);

	iwl_trans_tx_agg_setup(trans(priv), ctx->ctxid, sta_priv->sta_id, tid,
			       buf_size, ssn);

	/*
	 * If the limit is 0, then it wasn't initialised yet,
	 * use the default. We can do that since we take the
	 * minimum below, and we don't want to go above our
	 * default due to hardware restrictions.
	 */
	if (sta_priv->max_agg_bufsize == 0)
		sta_priv->max_agg_bufsize =
			LINK_QUAL_AGG_FRAME_LIMIT_DEF;

	/*
	 * Even though in theory the peer could have different
	 * aggregation reorder buffer sizes for different sessions,
	 * our ucode doesn't allow for that and has a global limit
	 * for each station. Therefore, use the minimum of all the
	 * aggregation sessions and our default value.
	 */
	sta_priv->max_agg_bufsize =
		min(sta_priv->max_agg_bufsize, buf_size);

	if (hw_params(priv).use_rts_for_aggregation) {
		/*
		 * switch to RTS/CTS if it is the prefer protection
		 * method for HT traffic
		 */

		sta_priv->lq_sta.lq.general_params.flags |=
			LINK_QUAL_FLAGS_SET_STA_TLC_RTS_MSK;
	}
	priv->agg_tids_count++;
	IWL_DEBUG_HT(priv, "priv->agg_tids_count = %u\n",
		     priv->agg_tids_count);

	sta_priv->lq_sta.lq.agg_params.agg_frame_cnt_limit =
		sta_priv->max_agg_bufsize;

	IWL_DEBUG_HT(priv, "Tx aggregation enabled on ra = %pM tid = %d\n",
		 sta->addr, tid);

	return iwl_send_lq_cmd(priv, ctx,
			&sta_priv->lq_sta.lq, CMD_ASYNC, false);
}

static void iwlagn_check_ratid_empty(struct iwl_priv *priv, int sta_id, u8 tid)
{
	struct iwl_tid_data *tid_data = &priv->tid_data[sta_id][tid];
	enum iwl_rxon_context_id ctx;
	struct ieee80211_vif *vif;
	u8 *addr;

	lockdep_assert_held(&priv->sta_lock);

	addr = priv->stations[sta_id].sta.sta.addr;
	ctx = priv->stations[sta_id].ctxid;
	vif = priv->contexts[ctx].vif;

	switch (priv->tid_data[sta_id][tid].agg.state) {
	case IWL_EMPTYING_HW_QUEUE_DELBA:
		/* There are no packets for this RA / TID in the HW any more */
		if (tid_data->agg.ssn == tid_data->next_reclaimed) {
			IWL_DEBUG_TX_QUEUES(priv,
				"Can continue DELBA flow ssn = next_recl ="
				" %d", tid_data->next_reclaimed);
			iwl_trans_tx_agg_disable(trans(priv), sta_id, tid);
			tid_data->agg.state = IWL_AGG_OFF;
			ieee80211_stop_tx_ba_cb_irqsafe(vif, addr, tid);
		}
		break;
	case IWL_EMPTYING_HW_QUEUE_ADDBA:
		/* There are no packets for this RA / TID in the HW any more */
		if (tid_data->agg.ssn == tid_data->next_reclaimed) {
			IWL_DEBUG_TX_QUEUES(priv,
				"Can continue ADDBA flow ssn = next_recl ="
				" %d", tid_data->next_reclaimed);
			tid_data->agg.state = IWL_AGG_ON;
			ieee80211_start_tx_ba_cb_irqsafe(vif, addr, tid);
		}
		break;
	default:
		break;
	}
>>>>>>> refs/remotes/origin/cm-10.0
}

static void iwlagn_non_agg_tx_status(struct iwl_priv *priv,
				     struct iwl_rxon_context *ctx,
				     const u8 *addr1)
{
	struct ieee80211_sta *sta;
	struct iwl_station_priv *sta_priv;

	rcu_read_lock();
	sta = ieee80211_find_sta(ctx->vif, addr1);
	if (sta) {
		sta_priv = (void *)sta->drv_priv;
		/* avoid atomic ops if this isn't a client */
		if (sta_priv->client &&
		    atomic_dec_return(&sta_priv->pending_frames) == 0)
			ieee80211_sta_block_awake(priv->hw, sta, false);
	}
	rcu_read_unlock();
}

<<<<<<< HEAD
static void iwlagn_tx_status(struct iwl_priv *priv, struct iwl_tx_info *tx_info,
			     bool is_agg)
{
	struct ieee80211_hdr *hdr = (struct ieee80211_hdr *) tx_info->skb->data;

	if (!is_agg)
		iwlagn_non_agg_tx_status(priv, tx_info->ctx, hdr->addr1);

	ieee80211_tx_status_irqsafe(priv->hw, tx_info->skb);
}

int iwlagn_tx_queue_reclaim(struct iwl_priv *priv, int txq_id, int index)
{
	struct iwl_tx_queue *txq = &priv->txq[txq_id];
	struct iwl_queue *q = &txq->q;
	struct iwl_tx_info *tx_info;
	int nfreed = 0;
	struct ieee80211_hdr *hdr;

	if ((index >= q->n_bd) || (iwl_queue_used(q, index) == 0)) {
		IWL_ERR(priv, "Read index for DMA queue txq id (%d), index %d, "
			  "is out of range [0-%d] %d %d.\n", txq_id,
			  index, q->n_bd, q->write_ptr, q->read_ptr);
		return 0;
	}

	for (index = iwl_queue_inc_wrap(index, q->n_bd);
	     q->read_ptr != index;
	     q->read_ptr = iwl_queue_inc_wrap(q->read_ptr, q->n_bd)) {

		tx_info = &txq->txb[txq->q.read_ptr];

		if (WARN_ON_ONCE(tx_info->skb == NULL))
			continue;

		hdr = (struct ieee80211_hdr *)tx_info->skb->data;
		if (ieee80211_is_data_qos(hdr->frame_control))
			nfreed++;

		iwlagn_tx_status(priv, tx_info,
				 txq_id >= IWLAGN_FIRST_AMPDU_QUEUE);
		tx_info->skb = NULL;

		iwlagn_txq_inval_byte_cnt_tbl(priv, txq);

		iwlagn_txq_free_tfd(priv, txq);
	}
	return nfreed;
}

/**
 * iwlagn_tx_status_reply_compressed_ba - Update tx status from block-ack
 *
 * Go through block-ack's bitmap of ACK'd frames, update driver's record of
 * ACK vs. not.  This gets sent to mac80211, then to rate scaling algo.
 */
static int iwlagn_tx_status_reply_compressed_ba(struct iwl_priv *priv,
				 struct iwl_ht_agg *agg,
				 struct iwl_compressed_ba_resp *ba_resp)

{
	int sh;
	u16 seq_ctl = le16_to_cpu(ba_resp->seq_ctl);
	u16 scd_flow = le16_to_cpu(ba_resp->scd_flow);
	struct ieee80211_tx_info *info;
	u64 bitmap, sent_bitmap;

	if (unlikely(!agg->wait_for_ba))  {
		if (unlikely(ba_resp->bitmap))
			IWL_ERR(priv, "Received BA when not expected\n");
		return -EINVAL;
	}

	/* Mark that the expected block-ack response arrived */
	agg->wait_for_ba = 0;
	IWL_DEBUG_TX_REPLY(priv, "BA %d %d\n", agg->start_idx, ba_resp->seq_ctl);

	/* Calculate shift to align block-ack bits with our Tx window bits */
	sh = agg->start_idx - SEQ_TO_INDEX(seq_ctl >> 4);
	if (sh < 0)
		sh += 0x100;

	/*
	 * Check for success or failure according to the
	 * transmitted bitmap and block-ack bitmap
	 */
	bitmap = le64_to_cpu(ba_resp->bitmap) >> sh;
	sent_bitmap = bitmap & agg->bitmap;

	/* Sanity check values reported by uCode */
	if (ba_resp->txed_2_done > ba_resp->txed) {
		IWL_DEBUG_TX_REPLY(priv,
			"bogus sent(%d) and ack(%d) count\n",
			ba_resp->txed, ba_resp->txed_2_done);
		/*
		 * set txed_2_done = txed,
		 * so it won't impact rate scale
		 */
		ba_resp->txed = ba_resp->txed_2_done;
	}
	IWL_DEBUG_HT(priv, "agg frames sent:%d, acked:%d\n",
			ba_resp->txed, ba_resp->txed_2_done);

	/* Find the first ACKed frame to store the TX status */
	while (sent_bitmap && !(sent_bitmap & 1)) {
		agg->start_idx = (agg->start_idx + 1) & 0xff;
		sent_bitmap >>= 1;
	}

	info = IEEE80211_SKB_CB(priv->txq[scd_flow].txb[agg->start_idx].skb);
	memset(&info->status, 0, sizeof(info->status));
	info->flags |= IEEE80211_TX_STAT_ACK;
	info->flags |= IEEE80211_TX_STAT_AMPDU;
	info->status.ampdu_ack_len = ba_resp->txed_2_done;
	info->status.ampdu_len = ba_resp->txed;
	iwlagn_hwrate_to_tx_control(priv, agg->rate_n_flags, info);

	return 0;
}

/**
 * translate ucode response to mac80211 tx status control values
 */
void iwlagn_hwrate_to_tx_control(struct iwl_priv *priv, u32 rate_n_flags,
				  struct ieee80211_tx_info *info)
{
	struct ieee80211_tx_rate *r = &info->control.rates[0];

	info->antenna_sel_tx =
		((rate_n_flags & RATE_MCS_ANT_ABC_MSK) >> RATE_MCS_ANT_POS);
	if (rate_n_flags & RATE_MCS_HT_MSK)
		r->flags |= IEEE80211_TX_RC_MCS;
	if (rate_n_flags & RATE_MCS_GF_MSK)
		r->flags |= IEEE80211_TX_RC_GREEN_FIELD;
	if (rate_n_flags & RATE_MCS_HT40_MSK)
		r->flags |= IEEE80211_TX_RC_40_MHZ_WIDTH;
	if (rate_n_flags & RATE_MCS_DUP_MSK)
		r->flags |= IEEE80211_TX_RC_DUP_DATA;
	if (rate_n_flags & RATE_MCS_SGI_MSK)
		r->flags |= IEEE80211_TX_RC_SHORT_GI;
	r->idx = iwlagn_hwrate_to_mac80211_idx(rate_n_flags, info->band);
=======
/**
 * translate ucode response to mac80211 tx status control values
 */
static void iwlagn_hwrate_to_tx_control(struct iwl_priv *priv, u32 rate_n_flags,
				  struct ieee80211_tx_info *info)
{
	struct ieee80211_tx_rate *r = &info->control.rates[0];

	info->antenna_sel_tx =
		((rate_n_flags & RATE_MCS_ANT_ABC_MSK) >> RATE_MCS_ANT_POS);
	if (rate_n_flags & RATE_MCS_HT_MSK)
		r->flags |= IEEE80211_TX_RC_MCS;
	if (rate_n_flags & RATE_MCS_GF_MSK)
		r->flags |= IEEE80211_TX_RC_GREEN_FIELD;
	if (rate_n_flags & RATE_MCS_HT40_MSK)
		r->flags |= IEEE80211_TX_RC_40_MHZ_WIDTH;
	if (rate_n_flags & RATE_MCS_DUP_MSK)
		r->flags |= IEEE80211_TX_RC_DUP_DATA;
	if (rate_n_flags & RATE_MCS_SGI_MSK)
		r->flags |= IEEE80211_TX_RC_SHORT_GI;
	r->idx = iwlagn_hwrate_to_mac80211_idx(rate_n_flags, info->band);
}

#ifdef CONFIG_IWLWIFI_DEBUG
const char *iwl_get_tx_fail_reason(u32 status)
{
#define TX_STATUS_FAIL(x) case TX_STATUS_FAIL_ ## x: return #x
#define TX_STATUS_POSTPONE(x) case TX_STATUS_POSTPONE_ ## x: return #x

	switch (status & TX_STATUS_MSK) {
	case TX_STATUS_SUCCESS:
		return "SUCCESS";
	TX_STATUS_POSTPONE(DELAY);
	TX_STATUS_POSTPONE(FEW_BYTES);
	TX_STATUS_POSTPONE(BT_PRIO);
	TX_STATUS_POSTPONE(QUIET_PERIOD);
	TX_STATUS_POSTPONE(CALC_TTAK);
	TX_STATUS_FAIL(INTERNAL_CROSSED_RETRY);
	TX_STATUS_FAIL(SHORT_LIMIT);
	TX_STATUS_FAIL(LONG_LIMIT);
	TX_STATUS_FAIL(FIFO_UNDERRUN);
	TX_STATUS_FAIL(DRAIN_FLOW);
	TX_STATUS_FAIL(RFKILL_FLUSH);
	TX_STATUS_FAIL(LIFE_EXPIRE);
	TX_STATUS_FAIL(DEST_PS);
	TX_STATUS_FAIL(HOST_ABORTED);
	TX_STATUS_FAIL(BT_RETRY);
	TX_STATUS_FAIL(STA_INVALID);
	TX_STATUS_FAIL(FRAG_DROPPED);
	TX_STATUS_FAIL(TID_DISABLE);
	TX_STATUS_FAIL(FIFO_FLUSHED);
	TX_STATUS_FAIL(INSUFFICIENT_CF_POLL);
	TX_STATUS_FAIL(PASSIVE_NO_RX);
	TX_STATUS_FAIL(NO_BEACON_ON_RADAR);
	}

	return "UNKNOWN";

#undef TX_STATUS_FAIL
#undef TX_STATUS_POSTPONE
}
#endif /* CONFIG_IWLWIFI_DEBUG */

static void iwlagn_count_agg_tx_err_status(struct iwl_priv *priv, u16 status)
{
	status &= AGG_TX_STATUS_MSK;

	switch (status) {
	case AGG_TX_STATE_UNDERRUN_MSK:
		priv->reply_agg_tx_stats.underrun++;
		break;
	case AGG_TX_STATE_BT_PRIO_MSK:
		priv->reply_agg_tx_stats.bt_prio++;
		break;
	case AGG_TX_STATE_FEW_BYTES_MSK:
		priv->reply_agg_tx_stats.few_bytes++;
		break;
	case AGG_TX_STATE_ABORT_MSK:
		priv->reply_agg_tx_stats.abort++;
		break;
	case AGG_TX_STATE_LAST_SENT_TTL_MSK:
		priv->reply_agg_tx_stats.last_sent_ttl++;
		break;
	case AGG_TX_STATE_LAST_SENT_TRY_CNT_MSK:
		priv->reply_agg_tx_stats.last_sent_try++;
		break;
	case AGG_TX_STATE_LAST_SENT_BT_KILL_MSK:
		priv->reply_agg_tx_stats.last_sent_bt_kill++;
		break;
	case AGG_TX_STATE_SCD_QUERY_MSK:
		priv->reply_agg_tx_stats.scd_query++;
		break;
	case AGG_TX_STATE_TEST_BAD_CRC32_MSK:
		priv->reply_agg_tx_stats.bad_crc32++;
		break;
	case AGG_TX_STATE_RESPONSE_MSK:
		priv->reply_agg_tx_stats.response++;
		break;
	case AGG_TX_STATE_DUMP_TX_MSK:
		priv->reply_agg_tx_stats.dump_tx++;
		break;
	case AGG_TX_STATE_DELAY_TX_MSK:
		priv->reply_agg_tx_stats.delay_tx++;
		break;
	default:
		priv->reply_agg_tx_stats.unknown++;
		break;
	}
}

static void iwl_rx_reply_tx_agg(struct iwl_priv *priv,
				struct iwlagn_tx_resp *tx_resp)
{
	struct agg_tx_status *frame_status = &tx_resp->status;
	int tid = (tx_resp->ra_tid & IWLAGN_TX_RES_TID_MSK) >>
		IWLAGN_TX_RES_TID_POS;
	int sta_id = (tx_resp->ra_tid & IWLAGN_TX_RES_RA_MSK) >>
		IWLAGN_TX_RES_RA_POS;
	struct iwl_ht_agg *agg = &priv->tid_data[sta_id][tid].agg;
	u32 status = le16_to_cpu(tx_resp->status.status);
	int i;

	WARN_ON(tid == IWL_TID_NON_QOS);

	if (agg->wait_for_ba)
		IWL_DEBUG_TX_REPLY(priv,
			"got tx response w/o block-ack\n");

	agg->rate_n_flags = le32_to_cpu(tx_resp->rate_n_flags);
	agg->wait_for_ba = (tx_resp->frame_count > 1);

	/*
	 * If the BT kill count is non-zero, we'll get this
	 * notification again.
	 */
	if (tx_resp->bt_kill_count && tx_resp->frame_count == 1 &&
	    cfg(priv)->bt_params &&
	    cfg(priv)->bt_params->advanced_bt_coexist) {
		IWL_DEBUG_COEX(priv, "receive reply tx w/ bt_kill\n");
	}

	if (tx_resp->frame_count == 1)
		return;

	/* Construct bit-map of pending frames within Tx window */
	for (i = 0; i < tx_resp->frame_count; i++) {
		u16 fstatus = le16_to_cpu(frame_status[i].status);

		if (status & AGG_TX_STATUS_MSK)
			iwlagn_count_agg_tx_err_status(priv, fstatus);

		if (status & (AGG_TX_STATE_FEW_BYTES_MSK |
			      AGG_TX_STATE_ABORT_MSK))
			continue;

		IWL_DEBUG_TX_REPLY(priv, "status %s (0x%08x), "
				   "try-count (0x%08x)\n",
				   iwl_get_agg_tx_fail_reason(fstatus),
				   fstatus & AGG_TX_STATUS_MSK,
				   fstatus & AGG_TX_TRY_MSK);
	}
}

#ifdef CONFIG_IWLWIFI_DEBUG
#define AGG_TX_STATE_FAIL(x) case AGG_TX_STATE_ ## x: return #x

const char *iwl_get_agg_tx_fail_reason(u16 status)
{
	status &= AGG_TX_STATUS_MSK;
	switch (status) {
	case AGG_TX_STATE_TRANSMITTED:
		return "SUCCESS";
		AGG_TX_STATE_FAIL(UNDERRUN_MSK);
		AGG_TX_STATE_FAIL(BT_PRIO_MSK);
		AGG_TX_STATE_FAIL(FEW_BYTES_MSK);
		AGG_TX_STATE_FAIL(ABORT_MSK);
		AGG_TX_STATE_FAIL(LAST_SENT_TTL_MSK);
		AGG_TX_STATE_FAIL(LAST_SENT_TRY_CNT_MSK);
		AGG_TX_STATE_FAIL(LAST_SENT_BT_KILL_MSK);
		AGG_TX_STATE_FAIL(SCD_QUERY_MSK);
		AGG_TX_STATE_FAIL(TEST_BAD_CRC32_MSK);
		AGG_TX_STATE_FAIL(RESPONSE_MSK);
		AGG_TX_STATE_FAIL(DUMP_TX_MSK);
		AGG_TX_STATE_FAIL(DELAY_TX_MSK);
	}

	return "UNKNOWN";
}
#endif /* CONFIG_IWLWIFI_DEBUG */

static inline u32 iwlagn_get_scd_ssn(struct iwlagn_tx_resp *tx_resp)
{
	return le32_to_cpup((__le32 *)&tx_resp->status +
			    tx_resp->frame_count) & MAX_SN;
}

static void iwlagn_count_tx_err_status(struct iwl_priv *priv, u16 status)
{
	status &= TX_STATUS_MSK;

	switch (status) {
	case TX_STATUS_POSTPONE_DELAY:
		priv->reply_tx_stats.pp_delay++;
		break;
	case TX_STATUS_POSTPONE_FEW_BYTES:
		priv->reply_tx_stats.pp_few_bytes++;
		break;
	case TX_STATUS_POSTPONE_BT_PRIO:
		priv->reply_tx_stats.pp_bt_prio++;
		break;
	case TX_STATUS_POSTPONE_QUIET_PERIOD:
		priv->reply_tx_stats.pp_quiet_period++;
		break;
	case TX_STATUS_POSTPONE_CALC_TTAK:
		priv->reply_tx_stats.pp_calc_ttak++;
		break;
	case TX_STATUS_FAIL_INTERNAL_CROSSED_RETRY:
		priv->reply_tx_stats.int_crossed_retry++;
		break;
	case TX_STATUS_FAIL_SHORT_LIMIT:
		priv->reply_tx_stats.short_limit++;
		break;
	case TX_STATUS_FAIL_LONG_LIMIT:
		priv->reply_tx_stats.long_limit++;
		break;
	case TX_STATUS_FAIL_FIFO_UNDERRUN:
		priv->reply_tx_stats.fifo_underrun++;
		break;
	case TX_STATUS_FAIL_DRAIN_FLOW:
		priv->reply_tx_stats.drain_flow++;
		break;
	case TX_STATUS_FAIL_RFKILL_FLUSH:
		priv->reply_tx_stats.rfkill_flush++;
		break;
	case TX_STATUS_FAIL_LIFE_EXPIRE:
		priv->reply_tx_stats.life_expire++;
		break;
	case TX_STATUS_FAIL_DEST_PS:
		priv->reply_tx_stats.dest_ps++;
		break;
	case TX_STATUS_FAIL_HOST_ABORTED:
		priv->reply_tx_stats.host_abort++;
		break;
	case TX_STATUS_FAIL_BT_RETRY:
		priv->reply_tx_stats.bt_retry++;
		break;
	case TX_STATUS_FAIL_STA_INVALID:
		priv->reply_tx_stats.sta_invalid++;
		break;
	case TX_STATUS_FAIL_FRAG_DROPPED:
		priv->reply_tx_stats.frag_drop++;
		break;
	case TX_STATUS_FAIL_TID_DISABLE:
		priv->reply_tx_stats.tid_disable++;
		break;
	case TX_STATUS_FAIL_FIFO_FLUSHED:
		priv->reply_tx_stats.fifo_flush++;
		break;
	case TX_STATUS_FAIL_INSUFFICIENT_CF_POLL:
		priv->reply_tx_stats.insuff_cf_poll++;
		break;
	case TX_STATUS_FAIL_PASSIVE_NO_RX:
		priv->reply_tx_stats.fail_hw_drop++;
		break;
	case TX_STATUS_FAIL_NO_BEACON_ON_RADAR:
		priv->reply_tx_stats.sta_color_mismatch++;
		break;
	default:
		priv->reply_tx_stats.unknown++;
		break;
	}
}

static void iwlagn_set_tx_status(struct iwl_priv *priv,
				 struct ieee80211_tx_info *info,
				 struct iwlagn_tx_resp *tx_resp,
				 bool is_agg)
{
	u16  status = le16_to_cpu(tx_resp->status.status);

	info->status.rates[0].count = tx_resp->failure_frame + 1;
	if (is_agg)
		info->flags &= ~IEEE80211_TX_CTL_AMPDU;
	info->flags |= iwl_tx_status_to_mac80211(status);
	iwlagn_hwrate_to_tx_control(priv, le32_to_cpu(tx_resp->rate_n_flags),
				    info);
	if (!iwl_is_tx_success(status))
		iwlagn_count_tx_err_status(priv, status);
}

static void iwl_check_abort_status(struct iwl_priv *priv,
			    u8 frame_count, u32 status)
{
	if (frame_count == 1 && status == TX_STATUS_FAIL_RFKILL_FLUSH) {
		IWL_ERR(priv, "Tx flush command to flush out all frames\n");
		if (!test_bit(STATUS_EXIT_PENDING, &priv->status))
			queue_work(priv->workqueue, &priv->tx_flush);
	}
}

int iwlagn_rx_reply_tx(struct iwl_priv *priv, struct iwl_rx_cmd_buffer *rxb,
			       struct iwl_device_cmd *cmd)
{
	struct iwl_rx_packet *pkt = rxb_addr(rxb);
	u16 sequence = le16_to_cpu(pkt->hdr.sequence);
	int txq_id = SEQ_TO_QUEUE(sequence);
	int cmd_index __maybe_unused = SEQ_TO_INDEX(sequence);
	struct iwlagn_tx_resp *tx_resp = (void *)pkt->data;
	struct ieee80211_hdr *hdr;
	u32 status = le16_to_cpu(tx_resp->status.status);
	u16 ssn = iwlagn_get_scd_ssn(tx_resp);
	int tid;
	int sta_id;
	int freed;
	struct ieee80211_tx_info *info;
	struct sk_buff_head skbs;
	struct sk_buff *skb;
	struct iwl_rxon_context *ctx;
	bool is_agg = (txq_id >= IWLAGN_FIRST_AMPDU_QUEUE);

	tid = (tx_resp->ra_tid & IWLAGN_TX_RES_TID_MSK) >>
		IWLAGN_TX_RES_TID_POS;
	sta_id = (tx_resp->ra_tid & IWLAGN_TX_RES_RA_MSK) >>
		IWLAGN_TX_RES_RA_POS;

	spin_lock(&priv->sta_lock);

	if (is_agg)
		iwl_rx_reply_tx_agg(priv, tx_resp);

	__skb_queue_head_init(&skbs);

	if (tx_resp->frame_count == 1) {
		u16 next_reclaimed = le16_to_cpu(tx_resp->seq_ctl);
		next_reclaimed = SEQ_TO_SN(next_reclaimed + 0x10);

		if (is_agg) {
			/* If this is an aggregation queue, we can rely on the
			 * ssn since the wifi sequence number corresponds to
			 * the index in the TFD ring (%256).
			 * The seq_ctl is the sequence control of the packet
			 * to which this Tx response relates. But if there is a
			 * hole in the bitmap of the BA we received, this Tx
			 * response may allow to reclaim the hole and all the
			 * subsequent packets that were already acked.
			 * In that case, seq_ctl != ssn, and the next packet
			 * to be reclaimed will be ssn and not seq_ctl.
			 */
			next_reclaimed = ssn;
		}

		if (tid != IWL_TID_NON_QOS) {
			priv->tid_data[sta_id][tid].next_reclaimed =
				next_reclaimed;
			IWL_DEBUG_TX_REPLY(priv, "Next reclaimed packet:%d",
						  next_reclaimed);
		}

		/*we can free until ssn % q.n_bd not inclusive */
		WARN_ON(iwl_trans_reclaim(trans(priv), sta_id, tid,
					  txq_id, ssn, &skbs));
		iwlagn_check_ratid_empty(priv, sta_id, tid);
		freed = 0;

		/* process frames */
		skb_queue_walk(&skbs, skb) {
			hdr = (struct ieee80211_hdr *)skb->data;

			if (!ieee80211_is_data_qos(hdr->frame_control))
				priv->last_seq_ctl = tx_resp->seq_ctl;

			info = IEEE80211_SKB_CB(skb);
			ctx = info->driver_data[0];
			kmem_cache_free(iwl_tx_cmd_pool,
					(info->driver_data[1]));

			memset(&info->status, 0, sizeof(info->status));

			if (status == TX_STATUS_FAIL_PASSIVE_NO_RX &&
			    iwl_is_associated_ctx(ctx) && ctx->vif &&
			    ctx->vif->type == NL80211_IFTYPE_STATION) {
				/* block and stop all queues */
				priv->passive_no_rx = true;
				IWL_DEBUG_TX_QUEUES(priv, "stop all queues: "
						    "passive channel");
				ieee80211_stop_queues(priv->hw);

				IWL_DEBUG_TX_REPLY(priv,
					   "TXQ %d status %s (0x%08x) "
					   "rate_n_flags 0x%x retries %d\n",
					   txq_id,
					   iwl_get_tx_fail_reason(status),
					   status,
					   le32_to_cpu(tx_resp->rate_n_flags),
					   tx_resp->failure_frame);

				IWL_DEBUG_TX_REPLY(priv,
					   "FrameCnt = %d, idx=%d\n",
					   tx_resp->frame_count, cmd_index);
			}

			/* check if BAR is needed */
			if (is_agg && !iwl_is_tx_success(status))
				info->flags |= IEEE80211_TX_STAT_AMPDU_NO_BACK;
			iwlagn_set_tx_status(priv, IEEE80211_SKB_CB(skb),
				     tx_resp, is_agg);
			if (!is_agg)
				iwlagn_non_agg_tx_status(priv, ctx, hdr->addr1);

			freed++;
		}

		WARN_ON(!is_agg && freed != 1);
	}

	iwl_check_abort_status(priv, tx_resp->frame_count, status);
	spin_unlock(&priv->sta_lock);

	while (!skb_queue_empty(&skbs)) {
		skb = __skb_dequeue(&skbs);
		ieee80211_tx_status(priv->hw, skb);
	}

	return 0;
>>>>>>> refs/remotes/origin/cm-10.0
}

/**
 * iwlagn_rx_reply_compressed_ba - Handler for REPLY_COMPRESSED_BA
 *
 * Handles block-acknowledge notification from device, which reports success
 * of frames sent via aggregation.
 */
<<<<<<< HEAD
void iwlagn_rx_reply_compressed_ba(struct iwl_priv *priv,
					   struct iwl_rx_mem_buffer *rxb)
{
	struct iwl_rx_packet *pkt = rxb_addr(rxb);
	struct iwl_compressed_ba_resp *ba_resp = &pkt->u.compressed_ba;
	struct iwl_tx_queue *txq = NULL;
	struct iwl_ht_agg *agg;
	int index;
	int sta_id;
	int tid;
	unsigned long flags;
=======
int iwlagn_rx_reply_compressed_ba(struct iwl_priv *priv,
				   struct iwl_rx_cmd_buffer *rxb,
				   struct iwl_device_cmd *cmd)
{
	struct iwl_rx_packet *pkt = rxb_addr(rxb);
	struct iwl_compressed_ba_resp *ba_resp = (void *)pkt->data;
	struct iwl_ht_agg *agg;
	struct sk_buff_head reclaimed_skbs;
	struct ieee80211_tx_info *info;
	struct ieee80211_hdr *hdr;
	struct sk_buff *skb;
	int sta_id;
	int tid;
	int freed;
>>>>>>> refs/remotes/origin/cm-10.0

	/* "flow" corresponds to Tx queue */
	u16 scd_flow = le16_to_cpu(ba_resp->scd_flow);

	/* "ssn" is start of block-ack Tx window, corresponds to index
	 * (in Tx queue's circular buffer) of first TFD/frame in window */
	u16 ba_resp_scd_ssn = le16_to_cpu(ba_resp->scd_ssn);

<<<<<<< HEAD
	if (scd_flow >= priv->hw_params.max_txq_num) {
		IWL_ERR(priv,
			"BUG_ON scd_flow is bigger than number of queues\n");
		return;
	}

	txq = &priv->txq[scd_flow];
	sta_id = ba_resp->sta_id;
	tid = ba_resp->tid;
	agg = &priv->stations[sta_id].tid[tid].agg;
	if (unlikely(agg->txq_id != scd_flow)) {
		/*
		 * FIXME: this is a uCode bug which need to be addressed,
		 * log the information and return for now!
		 * since it is possible happen very often and in order
		 * not to fill the syslog, don't enable the logging by default
		 */
		IWL_DEBUG_TX_REPLY(priv,
			"BA scd_flow %d does not match txq_id %d\n",
			scd_flow, agg->txq_id);
		return;
	}

	/* Find index just before block-ack window */
	index = iwl_queue_dec_wrap(ba_resp_scd_ssn & 0xff, txq->q.n_bd);

	spin_lock_irqsave(&priv->sta_lock, flags);
=======
	if (scd_flow >= cfg(priv)->base_params->num_of_queues) {
		IWL_ERR(priv,
			"BUG_ON scd_flow is bigger than number of queues\n");
		return 0;
	}

	sta_id = ba_resp->sta_id;
	tid = ba_resp->tid;
	agg = &priv->tid_data[sta_id][tid].agg;

	spin_lock(&priv->sta_lock);

	if (unlikely(!agg->wait_for_ba)) {
		if (unlikely(ba_resp->bitmap))
			IWL_ERR(priv, "Received BA when not expected\n");
		spin_unlock(&priv->sta_lock);
		return 0;
	}

	__skb_queue_head_init(&reclaimed_skbs);

	/* Release all TFDs before the SSN, i.e. all TFDs in front of
	 * block-ack window (we assume that they've been successfully
	 * transmitted ... if not, it's too late anyway). */
	if (iwl_trans_reclaim(trans(priv), sta_id, tid, scd_flow,
			      ba_resp_scd_ssn, &reclaimed_skbs)) {
		spin_unlock(&priv->sta_lock);
		return 0;
	}
>>>>>>> refs/remotes/origin/cm-10.0

	IWL_DEBUG_TX_REPLY(priv, "REPLY_COMPRESSED_BA [%d] Received from %pM, "
			   "sta_id = %d\n",
			   agg->wait_for_ba,
			   (u8 *) &ba_resp->sta_addr_lo32,
			   ba_resp->sta_id);
<<<<<<< HEAD
	IWL_DEBUG_TX_REPLY(priv, "TID = %d, SeqCtl = %d, bitmap = 0x%llx, scd_flow = "
			   "%d, scd_ssn = %d\n",
			   ba_resp->tid,
			   ba_resp->seq_ctl,
			   (unsigned long long)le64_to_cpu(ba_resp->bitmap),
			   ba_resp->scd_flow,
			   ba_resp->scd_ssn);
	IWL_DEBUG_TX_REPLY(priv, "DAT start_idx = %d, bitmap = 0x%llx\n",
			   agg->start_idx,
			   (unsigned long long)agg->bitmap);

	/* Update driver's record of ACK vs. not for each frame in window */
	iwlagn_tx_status_reply_compressed_ba(priv, agg, ba_resp);

	/* Release all TFDs before the SSN, i.e. all TFDs in front of
	 * block-ack window (we assume that they've been successfully
	 * transmitted ... if not, it's too late anyway). */
	if (txq->q.read_ptr != (ba_resp_scd_ssn & 0xff)) {
		/* calculate mac80211 ampdu sw queue to wake */
		int freed = iwlagn_tx_queue_reclaim(priv, scd_flow, index);
		iwl_free_tfds_in_queue(priv, sta_id, tid, freed);

		if ((iwl_queue_space(&txq->q) > txq->q.low_mark) &&
		    priv->mac80211_registered &&
		    (agg->state != IWL_EMPTYING_HW_QUEUE_DELBA))
			iwl_wake_queue(priv, txq);

		iwlagn_txq_check_empty(priv, sta_id, tid, scd_flow);
	}

	spin_unlock_irqrestore(&priv->sta_lock, flags);
}

#ifdef CONFIG_IWLWIFI_DEBUG
const char *iwl_get_tx_fail_reason(u32 status)
{
#define TX_STATUS_FAIL(x) case TX_STATUS_FAIL_ ## x: return #x
#define TX_STATUS_POSTPONE(x) case TX_STATUS_POSTPONE_ ## x: return #x

	switch (status & TX_STATUS_MSK) {
	case TX_STATUS_SUCCESS:
		return "SUCCESS";
	TX_STATUS_POSTPONE(DELAY);
	TX_STATUS_POSTPONE(FEW_BYTES);
	TX_STATUS_POSTPONE(BT_PRIO);
	TX_STATUS_POSTPONE(QUIET_PERIOD);
	TX_STATUS_POSTPONE(CALC_TTAK);
	TX_STATUS_FAIL(INTERNAL_CROSSED_RETRY);
	TX_STATUS_FAIL(SHORT_LIMIT);
	TX_STATUS_FAIL(LONG_LIMIT);
	TX_STATUS_FAIL(FIFO_UNDERRUN);
	TX_STATUS_FAIL(DRAIN_FLOW);
	TX_STATUS_FAIL(RFKILL_FLUSH);
	TX_STATUS_FAIL(LIFE_EXPIRE);
	TX_STATUS_FAIL(DEST_PS);
	TX_STATUS_FAIL(HOST_ABORTED);
	TX_STATUS_FAIL(BT_RETRY);
	TX_STATUS_FAIL(STA_INVALID);
	TX_STATUS_FAIL(FRAG_DROPPED);
	TX_STATUS_FAIL(TID_DISABLE);
	TX_STATUS_FAIL(FIFO_FLUSHED);
	TX_STATUS_FAIL(INSUFFICIENT_CF_POLL);
	TX_STATUS_FAIL(PASSIVE_NO_RX);
	TX_STATUS_FAIL(NO_BEACON_ON_RADAR);
	}

	return "UNKNOWN";

#undef TX_STATUS_FAIL
#undef TX_STATUS_POSTPONE
}
#endif /* CONFIG_IWLWIFI_DEBUG */
=======
	IWL_DEBUG_TX_REPLY(priv, "TID = %d, SeqCtl = %d, bitmap = 0x%llx, "
			   "scd_flow = %d, scd_ssn = %d\n",
			   ba_resp->tid, le16_to_cpu(ba_resp->seq_ctl),
			   (unsigned long long)le64_to_cpu(ba_resp->bitmap),
			   scd_flow, ba_resp_scd_ssn);

	/* Mark that the expected block-ack response arrived */
	agg->wait_for_ba = false;

	/* Sanity check values reported by uCode */
	if (ba_resp->txed_2_done > ba_resp->txed) {
		IWL_DEBUG_TX_REPLY(priv,
			"bogus sent(%d) and ack(%d) count\n",
			ba_resp->txed, ba_resp->txed_2_done);
		/*
		 * set txed_2_done = txed,
		 * so it won't impact rate scale
		 */
		ba_resp->txed = ba_resp->txed_2_done;
	}
	IWL_DEBUG_HT(priv, "agg frames sent:%d, acked:%d\n",
			ba_resp->txed, ba_resp->txed_2_done);

	priv->tid_data[sta_id][tid].next_reclaimed = ba_resp_scd_ssn;

	iwlagn_check_ratid_empty(priv, sta_id, tid);
	freed = 0;

	skb_queue_walk(&reclaimed_skbs, skb) {
		hdr = (struct ieee80211_hdr *)skb->data;

		if (ieee80211_is_data_qos(hdr->frame_control))
			freed++;
		else
			WARN_ON_ONCE(1);

		info = IEEE80211_SKB_CB(skb);
		kmem_cache_free(iwl_tx_cmd_pool, (info->driver_data[1]));

		if (freed == 1) {
			/* this is the first skb we deliver in this batch */
			/* put the rate scaling data there */
			info = IEEE80211_SKB_CB(skb);
			memset(&info->status, 0, sizeof(info->status));
			info->flags |= IEEE80211_TX_STAT_ACK;
			info->flags |= IEEE80211_TX_STAT_AMPDU;
			info->status.ampdu_ack_len = ba_resp->txed_2_done;
			info->status.ampdu_len = ba_resp->txed;
			iwlagn_hwrate_to_tx_control(priv, agg->rate_n_flags,
						    info);
		}
	}

	spin_unlock(&priv->sta_lock);

	while (!skb_queue_empty(&reclaimed_skbs)) {
		skb = __skb_dequeue(&reclaimed_skbs);
		ieee80211_tx_status(priv->hw, skb);
	}

	return 0;
}
>>>>>>> refs/remotes/origin/cm-10.0
