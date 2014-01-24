/*
 * Marvell Wireless LAN device driver: major functions
 *
 * Copyright (C) 2011, Marvell International Ltd.
 *
 * This software file (the "File") is distributed by Marvell International
 * Ltd. under the terms of the GNU General Public License Version 2, June 1991
 * (the "License").  You may use, redistribute and/or modify this File in
 * accordance with the terms and conditions of the License, a copy of which
 * is available by writing to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA or on the
 * worldwide web at http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt.
 *
 * THE FILE IS DISTRIBUTED AS-IS, WITHOUT WARRANTY OF ANY KIND, AND THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE
 * ARE EXPRESSLY DISCLAIMED.  The License provides additional details about
 * this warranty disclaimer.
 */

#include "main.h"
#include "wmm.h"
#include "cfg80211.h"
#include "11n.h"

#define VERSION	"1.0"

const char driver_version[] = "mwifiex " VERSION " (%s) ";
<<<<<<< HEAD

<<<<<<< HEAD
struct mwifiex_adapter *g_adapter;
EXPORT_SYMBOL_GPL(g_adapter);

static struct mwifiex_bss_attr mwifiex_bss_sta[] = {
	{MWIFIEX_BSS_TYPE_STA, MWIFIEX_DATA_FRAME_TYPE_ETH_II, true, 0, 0},
};

static int drv_mode = DRV_MODE_STA;

static char fw_name[32] = DEFAULT_FW_NAME;

/* Supported drv_mode table */
static struct mwifiex_drv_mode mwifiex_drv_mode_tbl[] = {
	{
		.drv_mode = DRV_MODE_STA,
		.intf_num = ARRAY_SIZE(mwifiex_bss_sta),
		.bss_attr = mwifiex_bss_sta,
	},
};

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
static char *cal_data_cfg;
module_param(cal_data_cfg, charp, 0);

static void scan_delay_timer_fn(unsigned long data)
{
	struct mwifiex_private *priv = (struct mwifiex_private *)data;
	struct mwifiex_adapter *adapter = priv->adapter;
	struct cmd_ctrl_node *cmd_node, *tmp_node;
	unsigned long flags;

	if (adapter->surprise_removed)
		return;

	if (adapter->scan_delay_cnt == MWIFIEX_MAX_SCAN_DELAY_CNT) {
		/*
		 * Abort scan operation by cancelling all pending scan
		 * commands
		 */
		spin_lock_irqsave(&adapter->scan_pending_q_lock, flags);
		list_for_each_entry_safe(cmd_node, tmp_node,
					 &adapter->scan_pending_q, list) {
			list_del(&cmd_node->list);
			mwifiex_insert_cmd_to_free_q(adapter, cmd_node);
		}
		spin_unlock_irqrestore(&adapter->scan_pending_q_lock, flags);

		spin_lock_irqsave(&adapter->mwifiex_cmd_lock, flags);
		adapter->scan_processing = false;
		adapter->scan_delay_cnt = 0;
		adapter->empty_tx_q_cnt = 0;
		spin_unlock_irqrestore(&adapter->mwifiex_cmd_lock, flags);

		if (priv->scan_request) {
			dev_dbg(adapter->dev, "info: aborting scan\n");
			cfg80211_scan_done(priv->scan_request, 1);
			priv->scan_request = NULL;
		} else {
			priv->scan_aborting = false;
			dev_dbg(adapter->dev, "info: scan already aborted\n");
		}
		goto done;
	}

	if (!atomic_read(&priv->adapter->is_tx_received)) {
		adapter->empty_tx_q_cnt++;
		if (adapter->empty_tx_q_cnt == MWIFIEX_MAX_EMPTY_TX_Q_CNT) {
			/*
			 * No Tx traffic for 200msec. Get scan command from
			 * scan pending queue and put to cmd pending queue to
			 * resume scan operation
			 */
			adapter->scan_delay_cnt = 0;
			adapter->empty_tx_q_cnt = 0;
			spin_lock_irqsave(&adapter->scan_pending_q_lock, flags);
			cmd_node = list_first_entry(&adapter->scan_pending_q,
						    struct cmd_ctrl_node, list);
			list_del(&cmd_node->list);
			spin_unlock_irqrestore(&adapter->scan_pending_q_lock,
					       flags);

			mwifiex_insert_cmd_to_pending_q(adapter, cmd_node,
							true);
			queue_work(adapter->workqueue, &adapter->main_work);
			goto done;
		}
	} else {
		adapter->empty_tx_q_cnt = 0;
	}

	/* Delay scan operation further by 20msec */
	mod_timer(&priv->scan_delay_timer, jiffies +
		  msecs_to_jiffies(MWIFIEX_SCAN_DELAY_MSEC));
	adapter->scan_delay_cnt++;

done:
	if (atomic_read(&priv->adapter->is_tx_received))
		atomic_set(&priv->adapter->is_tx_received, false);

	return;
}

>>>>>>> refs/remotes/origin/master
/*
 * This function registers the device and performs all the necessary
 * initializations.
 *
 * The following initialization operations are performed -
 *      - Allocate adapter structure
 *      - Save interface specific operations table in adapter
 *      - Call interface specific initialization routine
 *      - Allocate private structures
 *      - Set default adapter structure parameters
 *      - Initialize locks
 *
 * In case of any errors during inittialization, this function also ensures
 * proper cleanup before exiting.
 */
static int mwifiex_register(void *card, struct mwifiex_if_ops *if_ops,
<<<<<<< HEAD
<<<<<<< HEAD
			    struct mwifiex_drv_mode *drv_mode_ptr)
=======
			    void **padapter)
>>>>>>> refs/remotes/origin/cm-10.0
=======
			    void **padapter)
>>>>>>> refs/remotes/origin/master
{
	struct mwifiex_adapter *adapter;
	int i;

	adapter = kzalloc(sizeof(struct mwifiex_adapter), GFP_KERNEL);
	if (!adapter)
		return -ENOMEM;

<<<<<<< HEAD
<<<<<<< HEAD
	g_adapter = adapter;
=======
	*padapter = adapter;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	*padapter = adapter;
>>>>>>> refs/remotes/origin/master
	adapter->card = card;

	/* Save interface specific operations in adapter */
	memmove(&adapter->if_ops, if_ops, sizeof(struct mwifiex_if_ops));

	/* card specific initialization has been deferred until now .. */
<<<<<<< HEAD
	if (adapter->if_ops.init_if(adapter))
		goto error;

	adapter->priv_num = 0;
<<<<<<< HEAD
	for (i = 0; i < drv_mode_ptr->intf_num; i++) {
		adapter->priv[i] = NULL;

		if (!drv_mode_ptr->bss_attr[i].active)
			continue;

		/* Allocate memory for private structure */
		adapter->priv[i] = kzalloc(sizeof(struct mwifiex_private),
				GFP_KERNEL);
		if (!adapter->priv[i]) {
			dev_err(adapter->dev, "%s: failed to alloc priv[%d]\n",
			       __func__, i);
			goto error;
		}

		adapter->priv_num++;
		adapter->priv[i]->adapter = adapter;
		/* Save bss_type, frame_type & bss_priority */
		adapter->priv[i]->bss_type = drv_mode_ptr->bss_attr[i].bss_type;
		adapter->priv[i]->frame_type =
					drv_mode_ptr->bss_attr[i].frame_type;
		adapter->priv[i]->bss_priority =
					drv_mode_ptr->bss_attr[i].bss_priority;

		if (drv_mode_ptr->bss_attr[i].bss_type == MWIFIEX_BSS_TYPE_STA)
			adapter->priv[i]->bss_role = MWIFIEX_BSS_ROLE_STA;
		else if (drv_mode_ptr->bss_attr[i].bss_type ==
							MWIFIEX_BSS_TYPE_UAP)
			adapter->priv[i]->bss_role = MWIFIEX_BSS_ROLE_UAP;

		/* Save bss_index & bss_num */
		adapter->priv[i]->bss_index = i;
		adapter->priv[i]->bss_num = drv_mode_ptr->bss_attr[i].bss_num;
	}
	adapter->drv_mode = drv_mode_ptr;

	if (mwifiex_init_lock_list(adapter))
		goto error;
=======

	/* Allocate memory for private structure */
	adapter->priv[0] = kzalloc(sizeof(struct mwifiex_private), GFP_KERNEL);
	if (!adapter->priv[0]) {
		dev_err(adapter->dev,
			"%s: failed to alloc priv[0]\n", __func__);
		goto error;
	}

	adapter->priv_num++;

	adapter->priv[0]->adapter = adapter;
	mwifiex_init_lock_list(adapter);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (adapter->if_ops.init_if)
		if (adapter->if_ops.init_if(adapter))
			goto error;

	adapter->priv_num = 0;

	for (i = 0; i < MWIFIEX_MAX_BSS_NUM; i++) {
		/* Allocate memory for private structure */
		adapter->priv[i] =
			kzalloc(sizeof(struct mwifiex_private), GFP_KERNEL);
		if (!adapter->priv[i])
			goto error;

		adapter->priv[i]->adapter = adapter;
		adapter->priv_num++;

		setup_timer(&adapter->priv[i]->scan_delay_timer,
			    scan_delay_timer_fn,
			    (unsigned long)adapter->priv[i]);
	}
	mwifiex_init_lock_list(adapter);
>>>>>>> refs/remotes/origin/master

	init_timer(&adapter->cmd_timer);
	adapter->cmd_timer.function = mwifiex_cmd_timeout_func;
	adapter->cmd_timer.data = (unsigned long) adapter;

	return 0;

error:
	dev_dbg(adapter->dev, "info: leave mwifiex_register with error\n");

<<<<<<< HEAD
<<<<<<< HEAD
	mwifiex_free_lock_list(adapter);
	for (i = 0; i < drv_mode_ptr->intf_num; i++)
		kfree(adapter->priv[i]);
=======
	for (i = 0; i < adapter->priv_num; i++)
		kfree(adapter->priv[i]);

>>>>>>> refs/remotes/origin/cm-10.0
=======
	for (i = 0; i < adapter->priv_num; i++)
		kfree(adapter->priv[i]);

>>>>>>> refs/remotes/origin/master
	kfree(adapter);

	return -1;
}

/*
 * This function unregisters the device and performs all the necessary
 * cleanups.
 *
 * The following cleanup operations are performed -
 *      - Free the timers
 *      - Free beacon buffers
 *      - Free private structures
 *      - Free adapter structure
 */
static int mwifiex_unregister(struct mwifiex_adapter *adapter)
{
	s32 i;

<<<<<<< HEAD
=======
	if (adapter->if_ops.cleanup_if)
		adapter->if_ops.cleanup_if(adapter);

>>>>>>> refs/remotes/origin/master
	del_timer(&adapter->cmd_timer);

	/* Free private structures */
	for (i = 0; i < adapter->priv_num; i++) {
		if (adapter->priv[i]) {
			mwifiex_free_curr_bcn(adapter->priv[i]);
<<<<<<< HEAD
=======
			del_timer_sync(&adapter->priv[i]->scan_delay_timer);
>>>>>>> refs/remotes/origin/master
			kfree(adapter->priv[i]);
		}
	}

	kfree(adapter);
	return 0;
}

/*
 * The main process.
 *
 * This function is the main procedure of the driver and handles various driver
 * operations. It runs in a loop and provides the core functionalities.
 *
 * The main responsibilities of this function are -
 *      - Ensure concurrency control
 *      - Handle pending interrupts and call interrupt handlers
 *      - Wake up the card if required
 *      - Handle command responses and call response handlers
 *      - Handle events and call event handlers
 *      - Execute pending commands
 *      - Transmit pending data packets
 */
int mwifiex_main_process(struct mwifiex_adapter *adapter)
{
	int ret = 0;
	unsigned long flags;
<<<<<<< HEAD
=======
	struct sk_buff *skb;
>>>>>>> refs/remotes/origin/master

	spin_lock_irqsave(&adapter->main_proc_lock, flags);

	/* Check if already processing */
	if (adapter->mwifiex_processing) {
		spin_unlock_irqrestore(&adapter->main_proc_lock, flags);
		goto exit_main_proc;
	} else {
		adapter->mwifiex_processing = true;
		spin_unlock_irqrestore(&adapter->main_proc_lock, flags);
	}
process_start:
	do {
		if ((adapter->hw_status == MWIFIEX_HW_STATUS_CLOSING) ||
		    (adapter->hw_status == MWIFIEX_HW_STATUS_NOT_READY))
			break;

		/* Handle pending interrupt if any */
		if (adapter->int_status) {
			if (adapter->hs_activated)
				mwifiex_process_hs_config(adapter);
<<<<<<< HEAD
			adapter->if_ops.process_int_status(adapter);
=======
			if (adapter->if_ops.process_int_status)
				adapter->if_ops.process_int_status(adapter);
>>>>>>> refs/remotes/origin/master
		}

		/* Need to wake up the card ? */
		if ((adapter->ps_state == PS_STATE_SLEEP) &&
		    (adapter->pm_wakeup_card_req &&
		     !adapter->pm_wakeup_fw_try) &&
<<<<<<< HEAD
<<<<<<< HEAD
		    (is_command_pending(adapter)
		     || !mwifiex_wmm_lists_empty(adapter))) {
=======
		    (is_command_pending(adapter) ||
		     !mwifiex_wmm_lists_empty(adapter))) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
		    (is_command_pending(adapter) ||
		     !mwifiex_wmm_lists_empty(adapter))) {
>>>>>>> refs/remotes/origin/master
			adapter->pm_wakeup_fw_try = true;
			adapter->if_ops.wakeup(adapter);
			continue;
		}
<<<<<<< HEAD
=======

>>>>>>> refs/remotes/origin/master
		if (IS_CARD_RX_RCVD(adapter)) {
			adapter->pm_wakeup_fw_try = false;
			if (adapter->ps_state == PS_STATE_SLEEP)
				adapter->ps_state = PS_STATE_AWAKE;
		} else {
			/* We have tried to wakeup the card already */
			if (adapter->pm_wakeup_fw_try)
				break;
			if (adapter->ps_state != PS_STATE_AWAKE ||
			    adapter->tx_lock_flag)
				break;

<<<<<<< HEAD
<<<<<<< HEAD
			if (adapter->scan_processing || adapter->data_sent
			    || mwifiex_wmm_lists_empty(adapter)) {
				if (adapter->cmd_sent || adapter->curr_cmd
				    || (!is_command_pending(adapter)))
=======
			if (adapter->scan_processing || adapter->data_sent ||
			    mwifiex_wmm_lists_empty(adapter)) {
				if (adapter->cmd_sent || adapter->curr_cmd ||
				    (!is_command_pending(adapter)))
>>>>>>> refs/remotes/origin/cm-10.0
=======
			if ((adapter->scan_processing &&
			     !adapter->scan_delay_cnt) || adapter->data_sent ||
			    mwifiex_wmm_lists_empty(adapter)) {
				if (adapter->cmd_sent || adapter->curr_cmd ||
				    (!is_command_pending(adapter)))
>>>>>>> refs/remotes/origin/master
					break;
			}
		}

<<<<<<< HEAD
=======
		/* Check Rx data for USB */
		if (adapter->iface_type == MWIFIEX_USB)
			while ((skb = skb_dequeue(&adapter->usb_rx_data_q)))
				mwifiex_handle_rx_packet(adapter, skb);

>>>>>>> refs/remotes/origin/master
		/* Check for Cmd Resp */
		if (adapter->cmd_resp_received) {
			adapter->cmd_resp_received = false;
			mwifiex_process_cmdresp(adapter);

			/* call mwifiex back when init_fw is done */
			if (adapter->hw_status == MWIFIEX_HW_STATUS_INIT_DONE) {
				adapter->hw_status = MWIFIEX_HW_STATUS_READY;
				mwifiex_init_fw_complete(adapter);
			}
		}

		/* Check for event */
		if (adapter->event_received) {
			adapter->event_received = false;
			mwifiex_process_event(adapter);
		}

		/* Check if we need to confirm Sleep Request
		   received previously */
		if (adapter->ps_state == PS_STATE_PRE_SLEEP) {
			if (!adapter->cmd_sent && !adapter->curr_cmd)
				mwifiex_check_ps_cond(adapter);
		}

		/* * The ps_state may have been changed during processing of
		 * Sleep Request event.
		 */
<<<<<<< HEAD
<<<<<<< HEAD
		if ((adapter->ps_state == PS_STATE_SLEEP)
		    || (adapter->ps_state == PS_STATE_PRE_SLEEP)
		    || (adapter->ps_state == PS_STATE_SLEEP_CFM)
		    || adapter->tx_lock_flag)
=======
=======
>>>>>>> refs/remotes/origin/master
		if ((adapter->ps_state == PS_STATE_SLEEP) ||
		    (adapter->ps_state == PS_STATE_PRE_SLEEP) ||
		    (adapter->ps_state == PS_STATE_SLEEP_CFM) ||
		    adapter->tx_lock_flag)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			continue;

		if (!adapter->cmd_sent && !adapter->curr_cmd) {
			if (mwifiex_exec_next_cmd(adapter) == -1) {
				ret = -1;
				break;
			}
		}

<<<<<<< HEAD
		if (!adapter->scan_processing && !adapter->data_sent &&
		    !mwifiex_wmm_lists_empty(adapter)) {
=======
		if ((!adapter->scan_processing || adapter->scan_delay_cnt) &&
		    !adapter->data_sent && !mwifiex_wmm_lists_empty(adapter)) {
>>>>>>> refs/remotes/origin/master
			mwifiex_wmm_process_tx(adapter);
			if (adapter->hs_activated) {
				adapter->is_hs_configured = false;
				mwifiex_hs_activated_event
					(mwifiex_get_priv
					 (adapter, MWIFIEX_BSS_ROLE_ANY),
					 false);
			}
		}

		if (adapter->delay_null_pkt && !adapter->cmd_sent &&
<<<<<<< HEAD
<<<<<<< HEAD
		    !adapter->curr_cmd && !is_command_pending(adapter)
		    && mwifiex_wmm_lists_empty(adapter)) {
=======
		    !adapter->curr_cmd && !is_command_pending(adapter) &&
		    mwifiex_wmm_lists_empty(adapter)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
		    !adapter->curr_cmd && !is_command_pending(adapter) &&
		    mwifiex_wmm_lists_empty(adapter)) {
>>>>>>> refs/remotes/origin/master
			if (!mwifiex_send_null_packet
			    (mwifiex_get_priv(adapter, MWIFIEX_BSS_ROLE_STA),
			     MWIFIEX_TxPD_POWER_MGMT_NULL_PACKET |
			     MWIFIEX_TxPD_POWER_MGMT_LAST_PACKET)) {
				adapter->delay_null_pkt = false;
				adapter->ps_state = PS_STATE_SLEEP;
			}
			break;
		}
	} while (true);

<<<<<<< HEAD
	if ((adapter->int_status) || IS_CARD_RX_RCVD(adapter))
		goto process_start;

	spin_lock_irqsave(&adapter->main_proc_lock, flags);
=======
	spin_lock_irqsave(&adapter->main_proc_lock, flags);
	if ((adapter->int_status) || IS_CARD_RX_RCVD(adapter)) {
		spin_unlock_irqrestore(&adapter->main_proc_lock, flags);
		goto process_start;
	}

>>>>>>> refs/remotes/origin/master
	adapter->mwifiex_processing = false;
	spin_unlock_irqrestore(&adapter->main_proc_lock, flags);

exit_main_proc:
	if (adapter->hw_status == MWIFIEX_HW_STATUS_CLOSING)
		mwifiex_shutdown_drv(adapter);
	return ret;
}
<<<<<<< HEAD

/*
<<<<<<< HEAD
 * This function initializes the software.
 *
 * The main work includes allocating and initializing the adapter structure
 * and initializing the private structures.
 */
static int
mwifiex_init_sw(void *card, struct mwifiex_if_ops *if_ops)
{
	int i;
	struct mwifiex_drv_mode *drv_mode_ptr;

	/* find mwifiex_drv_mode entry from mwifiex_drv_mode_tbl */
	drv_mode_ptr = NULL;
	for (i = 0; i < ARRAY_SIZE(mwifiex_drv_mode_tbl); i++) {
		if (mwifiex_drv_mode_tbl[i].drv_mode == drv_mode) {
			drv_mode_ptr = &mwifiex_drv_mode_tbl[i];
			break;
		}
	}

	if (!drv_mode_ptr) {
		pr_err("invalid drv_mode=%d\n", drv_mode);
		return -1;
	}

	if (mwifiex_register(card, if_ops, drv_mode_ptr))
		return -1;

	return 0;
}

/*
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
EXPORT_SYMBOL_GPL(mwifiex_main_process);

/*
>>>>>>> refs/remotes/origin/master
 * This function frees the adapter structure.
 *
 * Additionally, this closes the netlink socket, frees the timers
 * and private structures.
 */
static void mwifiex_free_adapter(struct mwifiex_adapter *adapter)
{
	if (!adapter) {
		pr_err("%s: adapter is NULL\n", __func__);
		return;
	}

	mwifiex_unregister(adapter);
	pr_debug("info: %s: free adapter\n", __func__);
}

/*
<<<<<<< HEAD
 * This function initializes the hardware and firmware.
 *
 * The main initialization steps followed are -
 *      - Download the correct firmware to card
 *      - Allocate and initialize the adapter structure
 *      - Initialize the private structures
 *      - Issue the init commands to firmware
 */
static int mwifiex_init_hw_fw(struct mwifiex_adapter *adapter)
{
	int ret, err;
	struct mwifiex_fw_image fw;

	memset(&fw, 0, sizeof(struct mwifiex_fw_image));

<<<<<<< HEAD
	switch (adapter->revision_id) {
	case SD8787_W0:
	case SD8787_W1:
		strcpy(fw_name, SD8787_W1_FW_NAME);
		break;
	case SD8787_A0:
	case SD8787_A1:
		strcpy(fw_name, SD8787_AX_FW_NAME);
		break;
	default:
		break;
	}

	err = request_firmware(&adapter->firmware, fw_name, adapter->dev);
=======
	err = request_firmware(&adapter->firmware, adapter->fw_name,
			       adapter->dev);
>>>>>>> refs/remotes/origin/cm-10.0
	if (err < 0) {
		dev_err(adapter->dev, "request_firmware() returned"
				" error code %#x\n", err);
		ret = -1;
		goto done;
	}
	fw.fw_buf = (u8 *) adapter->firmware->data;
	fw.fw_len = adapter->firmware->size;

	ret = mwifiex_dnld_fw(adapter, &fw);
	if (ret == -1)
		goto done;

	dev_notice(adapter->dev, "WLAN FW is active\n");

	adapter->init_wait_q_woken = false;
	ret = mwifiex_init_fw(adapter);
	if (ret == -1) {
		goto done;
=======
 * This function cancels all works in the queue and destroys
 * the main workqueue.
 */
static void mwifiex_terminate_workqueue(struct mwifiex_adapter *adapter)
{
	flush_workqueue(adapter->workqueue);
	destroy_workqueue(adapter->workqueue);
	adapter->workqueue = NULL;
}

/*
 * This function gets firmware and initializes it.
 *
 * The main initialization steps followed are -
 *      - Download the correct firmware to card
 *      - Issue the init commands to firmware
 */
static void mwifiex_fw_dpc(const struct firmware *firmware, void *context)
{
	int ret;
	char fmt[64];
	struct mwifiex_private *priv;
	struct mwifiex_adapter *adapter = context;
	struct mwifiex_fw_image fw;
	struct semaphore *sem = adapter->card_sem;
	bool init_failed = false;
	struct wireless_dev *wdev;

	if (!firmware) {
		dev_err(adapter->dev,
			"Failed to get firmware %s\n", adapter->fw_name);
		goto err_dnld_fw;
	}

	memset(&fw, 0, sizeof(struct mwifiex_fw_image));
	adapter->firmware = firmware;
	fw.fw_buf = (u8 *) adapter->firmware->data;
	fw.fw_len = adapter->firmware->size;

	if (adapter->if_ops.dnld_fw)
		ret = adapter->if_ops.dnld_fw(adapter, &fw);
	else
		ret = mwifiex_dnld_fw(adapter, &fw);
	if (ret == -1)
		goto err_dnld_fw;

	dev_notice(adapter->dev, "WLAN FW is active\n");

	if (cal_data_cfg) {
		if ((request_firmware(&adapter->cal_data, cal_data_cfg,
				      adapter->dev)) < 0)
			dev_err(adapter->dev,
				"Cal data request_firmware() failed\n");
	}

	/* enable host interrupt after fw dnld is successful */
	if (adapter->if_ops.enable_int) {
		if (adapter->if_ops.enable_int(adapter))
			goto err_dnld_fw;
	}

	adapter->init_wait_q_woken = false;
	ret = mwifiex_init_fw(adapter);
	if (ret == -1) {
		goto err_init_fw;
>>>>>>> refs/remotes/origin/master
	} else if (!ret) {
		adapter->hw_status = MWIFIEX_HW_STATUS_READY;
		goto done;
	}
	/* Wait for mwifiex_init to complete */
	wait_event_interruptible(adapter->init_wait_q,
				 adapter->init_wait_q_woken);
<<<<<<< HEAD
	if (adapter->hw_status != MWIFIEX_HW_STATUS_READY) {
		ret = -1;
		goto done;
	}
	ret = 0;

done:
	if (adapter->firmware)
		release_firmware(adapter->firmware);
	if (ret)
		ret = -1;
	return ret;
}

/*
 * This function fills a driver buffer.
 *
 * The function associates a given SKB with the provided driver buffer
 * and also updates some of the SKB parameters, including IP header,
 * priority and timestamp.
 */
static void
mwifiex_fill_buffer(struct sk_buff *skb)
{
	struct ethhdr *eth;
	struct iphdr *iph;
	struct timeval tv;
	u8 tid = 0;

	eth = (struct ethhdr *) skb->data;
	switch (eth->h_proto) {
	case __constant_htons(ETH_P_IP):
		iph = ip_hdr(skb);
		tid = IPTOS_PREC(iph->tos);
		pr_debug("data: packet type ETH_P_IP: %04x, tid=%#x prio=%#x\n",
<<<<<<< HEAD
		       eth->h_proto, tid, skb->priority);
=======
			 eth->h_proto, tid, skb->priority);
>>>>>>> refs/remotes/origin/cm-10.0
		break;
	case __constant_htons(ETH_P_ARP):
		pr_debug("data: ARP packet: %04x\n", eth->h_proto);
	default:
		break;
	}
/* Offset for TOS field in the IP header */
#define IPTOS_OFFSET 5
	tid = (tid >> IPTOS_OFFSET);
	skb->priority = tid;
	/* Record the current time the packet was queued; used to
	   determine the amount of time the packet was queued in
	   the driver before it was sent to the firmware.
	   The delay is then sent along with the packet to the
	   firmware for aggregate delay calculation for stats and
	   MSDU lifetime expiry.
	 */
	do_gettimeofday(&tv);
	skb->tstamp = timeval_to_ktime(tv);
=======
	if (adapter->hw_status != MWIFIEX_HW_STATUS_READY)
		goto err_init_fw;

	priv = adapter->priv[MWIFIEX_BSS_ROLE_STA];
	if (mwifiex_register_cfg80211(adapter)) {
		dev_err(adapter->dev, "cannot register with cfg80211\n");
		goto err_init_fw;
	}

	rtnl_lock();
	/* Create station interface by default */
	wdev = mwifiex_add_virtual_intf(adapter->wiphy, "mlan%d",
					NL80211_IFTYPE_STATION, NULL, NULL);
	if (IS_ERR(wdev)) {
		dev_err(adapter->dev, "cannot create default STA interface\n");
		rtnl_unlock();
		goto err_add_intf;
	}
	rtnl_unlock();

	mwifiex_drv_get_driver_version(adapter, fmt, sizeof(fmt) - 1);
	dev_notice(adapter->dev, "driver_version = %s\n", fmt);
	goto done;

err_add_intf:
	wiphy_unregister(adapter->wiphy);
	wiphy_free(adapter->wiphy);
err_init_fw:
	if (adapter->if_ops.disable_int)
		adapter->if_ops.disable_int(adapter);
err_dnld_fw:
	pr_debug("info: %s: unregister device\n", __func__);
	if (adapter->if_ops.unregister_dev)
		adapter->if_ops.unregister_dev(adapter);

	if ((adapter->hw_status == MWIFIEX_HW_STATUS_FW_READY) ||
	    (adapter->hw_status == MWIFIEX_HW_STATUS_READY)) {
		pr_debug("info: %s: shutdown mwifiex\n", __func__);
		adapter->init_wait_q_woken = false;

		if (mwifiex_shutdown_drv(adapter) == -EINPROGRESS)
			wait_event_interruptible(adapter->init_wait_q,
						 adapter->init_wait_q_woken);
	}
	adapter->surprise_removed = true;
	mwifiex_terminate_workqueue(adapter);
	init_failed = true;
done:
	if (adapter->cal_data) {
		release_firmware(adapter->cal_data);
		adapter->cal_data = NULL;
	}
	if (adapter->firmware) {
		release_firmware(adapter->firmware);
		adapter->firmware = NULL;
	}
	complete(&adapter->fw_load);
	if (init_failed)
		mwifiex_free_adapter(adapter);
	up(sem);
	return;
}

/*
 * This function initializes the hardware and gets firmware.
 */
static int mwifiex_init_hw_fw(struct mwifiex_adapter *adapter)
{
	int ret;

	init_completion(&adapter->fw_load);
	ret = request_firmware_nowait(THIS_MODULE, 1, adapter->fw_name,
				      adapter->dev, GFP_KERNEL, adapter,
				      mwifiex_fw_dpc);
	if (ret < 0)
		dev_err(adapter->dev,
			"request_firmware_nowait() returned error %d\n", ret);
	return ret;
>>>>>>> refs/remotes/origin/master
}

/*
 * CFG802.11 network device handler for open.
 *
 * Starts the data queue.
 */
static int
mwifiex_open(struct net_device *dev)
{
<<<<<<< HEAD
<<<<<<< HEAD
	netif_start_queue(dev);
=======
	netif_tx_start_all_queues(dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	netif_tx_start_all_queues(dev);
>>>>>>> refs/remotes/origin/master
	return 0;
}

/*
 * CFG802.11 network device handler for close.
 */
static int
mwifiex_close(struct net_device *dev)
{
<<<<<<< HEAD
=======
	struct mwifiex_private *priv = mwifiex_netdev_get_priv(dev);

	if (priv->scan_request) {
		dev_dbg(priv->adapter->dev, "aborting scan on ndo_stop\n");
		cfg80211_scan_done(priv->scan_request, 1);
		priv->scan_request = NULL;
		priv->scan_aborting = true;
	}

	return 0;
}

/*
 * Add buffer into wmm tx queue and queue work to transmit it.
 */
int mwifiex_queue_tx_pkt(struct mwifiex_private *priv, struct sk_buff *skb)
{
	struct netdev_queue *txq;
	int index = mwifiex_1d_to_wmm_queue[skb->priority];

	if (atomic_inc_return(&priv->wmm_tx_pending[index]) >= MAX_TX_PENDING) {
		txq = netdev_get_tx_queue(priv->netdev, index);
		if (!netif_tx_queue_stopped(txq)) {
			netif_tx_stop_queue(txq);
			dev_dbg(priv->adapter->dev, "stop queue: %d\n", index);
		}
	}

	atomic_inc(&priv->adapter->tx_pending);
	mwifiex_wmm_add_buf_txqueue(priv, skb);

	if (priv->adapter->scan_delay_cnt)
		atomic_set(&priv->adapter->is_tx_received, true);

	queue_work(priv->adapter->workqueue, &priv->adapter->main_work);

>>>>>>> refs/remotes/origin/master
	return 0;
}

/*
 * CFG802.11 network device handler for data transmission.
 */
static int
mwifiex_hard_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct mwifiex_private *priv = mwifiex_netdev_get_priv(dev);
	struct sk_buff *new_skb;
	struct mwifiex_txinfo *tx_info;
<<<<<<< HEAD

<<<<<<< HEAD
	dev_dbg(priv->adapter->dev, "data: %lu BSS(%d): Data <= kernel\n",
				jiffies, priv->bss_index);
=======
	dev_dbg(priv->adapter->dev, "data: %lu BSS(%d-%d): Data <= kernel\n",
		jiffies, priv->bss_type, priv->bss_num);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct timeval tv;

	dev_dbg(priv->adapter->dev, "data: %lu BSS(%d-%d): Data <= kernel\n",
		jiffies, priv->bss_type, priv->bss_num);
>>>>>>> refs/remotes/origin/master

	if (priv->adapter->surprise_removed) {
		kfree_skb(skb);
		priv->stats.tx_dropped++;
		return 0;
	}
	if (!skb->len || (skb->len > ETH_FRAME_LEN)) {
		dev_err(priv->adapter->dev, "Tx: bad skb len %d\n", skb->len);
		kfree_skb(skb);
		priv->stats.tx_dropped++;
		return 0;
	}
	if (skb_headroom(skb) < MWIFIEX_MIN_DATA_HEADER_LEN) {
		dev_dbg(priv->adapter->dev,
			"data: Tx: insufficient skb headroom %d\n",
<<<<<<< HEAD
<<<<<<< HEAD
		       skb_headroom(skb));
=======
			skb_headroom(skb));
>>>>>>> refs/remotes/origin/cm-10.0
=======
			skb_headroom(skb));
>>>>>>> refs/remotes/origin/master
		/* Insufficient skb headroom - allocate a new skb */
		new_skb =
			skb_realloc_headroom(skb, MWIFIEX_MIN_DATA_HEADER_LEN);
		if (unlikely(!new_skb)) {
			dev_err(priv->adapter->dev, "Tx: cannot alloca new_skb\n");
			kfree_skb(skb);
			priv->stats.tx_dropped++;
			return 0;
		}
		kfree_skb(skb);
		skb = new_skb;
		dev_dbg(priv->adapter->dev, "info: new skb headroomd %d\n",
<<<<<<< HEAD
<<<<<<< HEAD
				skb_headroom(skb));
	}

	tx_info = MWIFIEX_SKB_TXCB(skb);
	tx_info->bss_index = priv->bss_index;
	mwifiex_fill_buffer(skb);

	mwifiex_wmm_add_buf_txqueue(priv->adapter, skb);
	atomic_inc(&priv->adapter->tx_pending);

	if (atomic_read(&priv->adapter->tx_pending) >= MAX_TX_PENDING) {
		netif_stop_queue(priv->netdev);
		dev->trans_start = jiffies;
=======
=======
>>>>>>> refs/remotes/origin/master
			skb_headroom(skb));
	}

	tx_info = MWIFIEX_SKB_TXCB(skb);
	tx_info->bss_num = priv->bss_num;
	tx_info->bss_type = priv->bss_type;
<<<<<<< HEAD
	mwifiex_fill_buffer(skb);

	mwifiex_wmm_add_buf_txqueue(priv, skb);
	atomic_inc(&priv->adapter->tx_pending);

	if (atomic_read(&priv->adapter->tx_pending) >= MAX_TX_PENDING) {
		mwifiex_set_trans_start(dev);
		mwifiex_stop_net_dev_queue(priv->netdev, priv->adapter);
>>>>>>> refs/remotes/origin/cm-10.0
	}

	queue_work(priv->adapter->workqueue, &priv->adapter->main_work);
=======

	/* Record the current time the packet was queued; used to
	 * determine the amount of time the packet was queued in
	 * the driver before it was sent to the firmware.
	 * The delay is then sent along with the packet to the
	 * firmware for aggregate delay calculation for stats and
	 * MSDU lifetime expiry.
	 */
	do_gettimeofday(&tv);
	skb->tstamp = timeval_to_ktime(tv);

	mwifiex_queue_tx_pkt(priv, skb);
>>>>>>> refs/remotes/origin/master

	return 0;
}

/*
 * CFG802.11 network device handler for setting MAC address.
 */
static int
mwifiex_set_mac_address(struct net_device *dev, void *addr)
{
	struct mwifiex_private *priv = mwifiex_netdev_get_priv(dev);
<<<<<<< HEAD
<<<<<<< HEAD
	struct sockaddr *hw_addr = (struct sockaddr *) addr;
=======
	struct sockaddr *hw_addr = addr;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct sockaddr *hw_addr = addr;
>>>>>>> refs/remotes/origin/master
	int ret;

	memcpy(priv->curr_addr, hw_addr->sa_data, ETH_ALEN);

	/* Send request to firmware */
	ret = mwifiex_send_cmd_sync(priv, HostCmd_CMD_802_11_MAC_ADDRESS,
				    HostCmd_ACT_GEN_SET, 0, NULL);

	if (!ret)
		memcpy(priv->netdev->dev_addr, priv->curr_addr, ETH_ALEN);
	else
<<<<<<< HEAD
<<<<<<< HEAD
		dev_err(priv->adapter->dev, "set mac address failed: ret=%d"
					    "\n", ret);
=======
		dev_err(priv->adapter->dev,
			"set mac address failed: ret=%d\n", ret);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		dev_err(priv->adapter->dev,
			"set mac address failed: ret=%d\n", ret);
>>>>>>> refs/remotes/origin/master

	memcpy(dev->dev_addr, priv->curr_addr, ETH_ALEN);

	return ret;
}

/*
 * CFG802.11 network device handler for setting multicast list.
 */
static void mwifiex_set_multicast_list(struct net_device *dev)
{
	struct mwifiex_private *priv = mwifiex_netdev_get_priv(dev);
	struct mwifiex_multicast_list mcast_list;

	if (dev->flags & IFF_PROMISC) {
		mcast_list.mode = MWIFIEX_PROMISC_MODE;
	} else if (dev->flags & IFF_ALLMULTI ||
		   netdev_mc_count(dev) > MWIFIEX_MAX_MULTICAST_LIST_SIZE) {
		mcast_list.mode = MWIFIEX_ALL_MULTI_MODE;
	} else {
		mcast_list.mode = MWIFIEX_MULTICAST_MODE;
<<<<<<< HEAD
		if (netdev_mc_count(dev))
			mcast_list.num_multicast_addr =
				mwifiex_copy_mcast_addr(&mcast_list, dev);
=======
		mcast_list.num_multicast_addr =
			mwifiex_copy_mcast_addr(&mcast_list, dev);
>>>>>>> refs/remotes/origin/master
	}
	mwifiex_request_set_multicast_list(priv, &mcast_list);
}

/*
 * CFG802.11 network device handler for transmission timeout.
 */
static void
mwifiex_tx_timeout(struct net_device *dev)
{
	struct mwifiex_private *priv = mwifiex_netdev_get_priv(dev);

<<<<<<< HEAD
<<<<<<< HEAD
	dev_err(priv->adapter->dev, "%lu : Tx timeout, bss_index=%d\n",
				jiffies, priv->bss_index);
	dev->trans_start = jiffies;
=======
	dev_err(priv->adapter->dev, "%lu : Tx timeout, bss_type-num = %d-%d\n",
		jiffies, priv->bss_type, priv->bss_num);
	mwifiex_set_trans_start(dev);
>>>>>>> refs/remotes/origin/cm-10.0
	priv->num_tx_timeout++;
=======
	priv->num_tx_timeout++;
	priv->tx_timeout_cnt++;
	dev_err(priv->adapter->dev,
		"%lu : Tx timeout(#%d), bss_type-num = %d-%d\n",
		jiffies, priv->tx_timeout_cnt, priv->bss_type, priv->bss_num);
	mwifiex_set_trans_start(dev);

	if (priv->tx_timeout_cnt > TX_TIMEOUT_THRESHOLD &&
	    priv->adapter->if_ops.card_reset) {
		dev_err(priv->adapter->dev,
			"tx_timeout_cnt exceeds threshold. Triggering card reset!\n");
		priv->adapter->if_ops.card_reset(priv->adapter);
	}
>>>>>>> refs/remotes/origin/master
}

/*
 * CFG802.11 network device handler for statistics retrieval.
 */
static struct net_device_stats *mwifiex_get_stats(struct net_device *dev)
{
	struct mwifiex_private *priv = mwifiex_netdev_get_priv(dev);

	return &priv->stats;
}

<<<<<<< HEAD
=======
static u16
mwifiex_netdev_select_wmm_queue(struct net_device *dev, struct sk_buff *skb,
				void *accel_priv)
{
	skb->priority = cfg80211_classify8021d(skb);
	return mwifiex_1d_to_wmm_queue[skb->priority];
}

>>>>>>> refs/remotes/origin/master
/* Network device handlers */
static const struct net_device_ops mwifiex_netdev_ops = {
	.ndo_open = mwifiex_open,
	.ndo_stop = mwifiex_close,
	.ndo_start_xmit = mwifiex_hard_start_xmit,
	.ndo_set_mac_address = mwifiex_set_mac_address,
	.ndo_tx_timeout = mwifiex_tx_timeout,
	.ndo_get_stats = mwifiex_get_stats,
<<<<<<< HEAD
<<<<<<< HEAD
	.ndo_set_multicast_list = mwifiex_set_multicast_list,
=======
	.ndo_set_rx_mode = mwifiex_set_multicast_list,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.ndo_set_rx_mode = mwifiex_set_multicast_list,
	.ndo_select_queue = mwifiex_netdev_select_wmm_queue,
>>>>>>> refs/remotes/origin/master
};

/*
 * This function initializes the private structure parameters.
 *
 * The following wait queues are initialized -
 *      - IOCTL wait queue
 *      - Command wait queue
 *      - Statistics wait queue
 *
 * ...and the following default parameters are set -
 *      - Current key index     : Set to 0
 *      - Rate index            : Set to auto
 *      - Media connected       : Set to disconnected
 *      - Adhoc link sensed     : Set to false
 *      - Nick name             : Set to null
 *      - Number of Tx timeout  : Set to 0
 *      - Device address        : Set to current address
 *
 * In addition, the CFG80211 work queue is also created.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static void
mwifiex_init_priv_params(struct mwifiex_private *priv, struct net_device *dev)
=======
void mwifiex_init_priv_params(struct mwifiex_private *priv,
						struct net_device *dev)
>>>>>>> refs/remotes/origin/cm-10.0
{
	dev->netdev_ops = &mwifiex_netdev_ops;
=======
void mwifiex_init_priv_params(struct mwifiex_private *priv,
						struct net_device *dev)
{
	dev->netdev_ops = &mwifiex_netdev_ops;
	dev->destructor = free_netdev;
>>>>>>> refs/remotes/origin/master
	/* Initialize private structure */
	priv->current_key_index = 0;
	priv->media_connected = false;
	memset(&priv->nick_name, 0, sizeof(priv->nick_name));
<<<<<<< HEAD
	priv->num_tx_timeout = 0;
<<<<<<< HEAD
	priv->workqueue = create_singlethread_workqueue("cfg80211_wq");
	INIT_WORK(&priv->cfg_workqueue, mwifiex_cfg80211_results);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	memset(priv->mgmt_ie, 0,
	       sizeof(struct mwifiex_ie) * MAX_MGMT_IE_INDEX);
	priv->beacon_idx = MWIFIEX_AUTO_IDX_MASK;
	priv->proberesp_idx = MWIFIEX_AUTO_IDX_MASK;
	priv->assocresp_idx = MWIFIEX_AUTO_IDX_MASK;
	priv->rsn_idx = MWIFIEX_AUTO_IDX_MASK;
	priv->num_tx_timeout = 0;
>>>>>>> refs/remotes/origin/master
	memcpy(dev->dev_addr, priv->curr_addr, ETH_ALEN);
}

/*
<<<<<<< HEAD
<<<<<<< HEAD
 * This function adds a new logical interface.
 *
 * It allocates, initializes and registers the interface by performing
 * the following opearations -
 *      - Allocate a new net device structure
 *      - Assign device name
 *      - Register the new device with CFG80211 subsystem
 *      - Initialize semaphore and private structure
 *      - Register the new device with kernel
 *      - Create the complete debug FS structure if configured
 */
static struct mwifiex_private *mwifiex_add_interface(
			struct mwifiex_adapter *adapter,
			u8 bss_index, u8 bss_type)
{
	struct net_device *dev;
	struct mwifiex_private *priv;
	void *mdev_priv;

	dev = alloc_netdev_mq(sizeof(struct mwifiex_private *), "mlan%d",
			      ether_setup, 1);
	if (!dev) {
		dev_err(adapter->dev, "no memory available for netdevice\n");
		goto error;
	}

	if (mwifiex_register_cfg80211(dev, adapter->priv[bss_index]->curr_addr,
				      adapter->priv[bss_index]) != 0) {
		dev_err(adapter->dev, "cannot register netdevice with cfg80211\n");
		goto error;
	}
	/* Save the priv pointer in netdev */
	priv = adapter->priv[bss_index];
	mdev_priv = netdev_priv(dev);
	*((unsigned long *) mdev_priv) = (unsigned long) priv;

	priv->netdev = dev;

	sema_init(&priv->async_sem, 1);
	priv->scan_pending_on_block = false;

	mwifiex_init_priv_params(priv, dev);

	SET_NETDEV_DEV(dev, adapter->dev);

	/* Register network device */
	if (register_netdev(dev)) {
		dev_err(adapter->dev, "cannot register virtual network device\n");
		goto error;
	}

	dev_dbg(adapter->dev, "info: %s: Marvell 802.11 Adapter\n", dev->name);
#ifdef CONFIG_DEBUG_FS
	mwifiex_dev_debugfs_init(priv);
#endif
	return priv;
error:
	if (dev)
		free_netdev(dev);
	return NULL;
}

/*
 * This function removes a logical interface.
 *
 * It deregisters, resets and frees the interface by performing
 * the following operations -
 *      - Disconnect the device if connected, send wireless event to
 *        notify applications.
 *      - Remove the debug FS structure if configured
 *      - Unregister the device from kernel
 *      - Free the net device structure
 *      - Cancel all works and destroy work queue
 *      - Unregister and free the wireless device from CFG80211 subsystem
 */
static void
mwifiex_remove_interface(struct mwifiex_adapter *adapter, u8 bss_index)
{
	struct net_device *dev;
	struct mwifiex_private *priv = adapter->priv[bss_index];

	if (!priv)
		return;
	dev = priv->netdev;

	if (priv->media_connected)
		priv->media_connected = false;

#ifdef CONFIG_DEBUG_FS
	mwifiex_dev_debugfs_remove(priv);
#endif
	/* Last reference is our one */
	dev_dbg(adapter->dev, "info: %s: refcnt = %d\n",
				dev->name, netdev_refcnt_read(dev));

	if (dev->reg_state == NETREG_REGISTERED)
		unregister_netdev(dev);

	/* Clear the priv in adapter */
	priv->netdev = NULL;
	if (dev)
		free_netdev(dev);

	cancel_work_sync(&priv->cfg_workqueue);
	flush_workqueue(priv->workqueue);
	destroy_workqueue(priv->workqueue);
	wiphy_unregister(priv->wdev->wiphy);
	wiphy_free(priv->wdev->wiphy);
	kfree(priv->wdev);
}

/*
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 * This function check if command is pending.
 */
int is_command_pending(struct mwifiex_adapter *adapter)
{
	unsigned long flags;
	int is_cmd_pend_q_empty;

	spin_lock_irqsave(&adapter->cmd_pending_q_lock, flags);
	is_cmd_pend_q_empty = list_empty(&adapter->cmd_pending_q);
	spin_unlock_irqrestore(&adapter->cmd_pending_q_lock, flags);

	return !is_cmd_pend_q_empty;
}

/*
<<<<<<< HEAD
<<<<<<< HEAD
 * This function returns the correct private structure pointer based
 * upon the BSS number.
 */
struct mwifiex_private *
mwifiex_bss_index_to_priv(struct mwifiex_adapter *adapter, u8 bss_index)
{
	if (!adapter || (bss_index >= adapter->priv_num))
		return NULL;
	return adapter->priv[bss_index];
}

/*
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 * This is the main work queue function.
 *
 * It handles the main process, which in turn handles the complete
 * driver operations.
 */
static void mwifiex_main_work_queue(struct work_struct *work)
{
	struct mwifiex_adapter *adapter =
		container_of(work, struct mwifiex_adapter, main_work);

	if (adapter->surprise_removed)
		return;
	mwifiex_main_process(adapter);
}

/*
<<<<<<< HEAD
 * This function cancels all works in the queue and destroys
 * the main workqueue.
 */
static void
mwifiex_terminate_workqueue(struct mwifiex_adapter *adapter)
{
	flush_workqueue(adapter->workqueue);
	destroy_workqueue(adapter->workqueue);
	adapter->workqueue = NULL;
}

/*
=======
>>>>>>> refs/remotes/origin/master
 * This function adds the card.
 *
 * This function follows the following major steps to set up the device -
 *      - Initialize software. This includes probing the card, registering
 *        the interface operations table, and allocating/initializing the
 *        adapter structure
 *      - Set up the netlink socket
 *      - Create and start the main work queue
 *      - Register the device
 *      - Initialize firmware and hardware
 *      - Add logical interfaces
 */
int
mwifiex_add_card(void *card, struct semaphore *sem,
<<<<<<< HEAD
<<<<<<< HEAD
		 struct mwifiex_if_ops *if_ops)
{
	int i;
	struct mwifiex_adapter *adapter;
=======
		 struct mwifiex_if_ops *if_ops, u8 iface_type)
{
	struct mwifiex_adapter *adapter;
	char fmt[64];
	struct mwifiex_private *priv;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		 struct mwifiex_if_ops *if_ops, u8 iface_type)
{
	struct mwifiex_adapter *adapter;
>>>>>>> refs/remotes/origin/master

	if (down_interruptible(sem))
		goto exit_sem_err;

<<<<<<< HEAD
<<<<<<< HEAD
	if (mwifiex_init_sw(card, if_ops)) {
=======
	if (mwifiex_register(card, if_ops, (void **)&adapter)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (mwifiex_register(card, if_ops, (void **)&adapter)) {
>>>>>>> refs/remotes/origin/master
		pr_err("%s: software init failed\n", __func__);
		goto err_init_sw;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	adapter = g_adapter;
=======
	adapter->iface_type = iface_type;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	adapter->iface_type = iface_type;
	adapter->card_sem = sem;
>>>>>>> refs/remotes/origin/master

	adapter->hw_status = MWIFIEX_HW_STATUS_INITIALIZING;
	adapter->surprise_removed = false;
	init_waitqueue_head(&adapter->init_wait_q);
	adapter->is_suspended = false;
	adapter->hs_activated = false;
	init_waitqueue_head(&adapter->hs_activate_wait_q);
	adapter->cmd_wait_q_required = false;
	init_waitqueue_head(&adapter->cmd_wait_q.wait);
<<<<<<< HEAD
<<<<<<< HEAD
	adapter->cmd_wait_q.condition = false;
	adapter->cmd_wait_q.status = 0;
=======
	adapter->cmd_wait_q.status = 0;
	adapter->scan_wait_q_woken = false;
>>>>>>> refs/remotes/origin/cm-10.0

	adapter->workqueue = create_workqueue("MWIFIEX_WORK_QUEUE");
=======
	adapter->cmd_wait_q.status = 0;
	adapter->scan_wait_q_woken = false;

	adapter->workqueue =
		alloc_workqueue("MWIFIEX_WORK_QUEUE",
				WQ_HIGHPRI | WQ_MEM_RECLAIM | WQ_UNBOUND, 1);
>>>>>>> refs/remotes/origin/master
	if (!adapter->workqueue)
		goto err_kmalloc;

	INIT_WORK(&adapter->main_work, mwifiex_main_work_queue);

	/* Register the device. Fill up the private data structure with relevant
<<<<<<< HEAD
	   information from the card and request for the required IRQ. */
=======
	   information from the card. */
>>>>>>> refs/remotes/origin/master
	if (adapter->if_ops.register_dev(adapter)) {
		pr_err("%s: failed to register mwifiex device\n", __func__);
		goto err_registerdev;
	}

	if (mwifiex_init_hw_fw(adapter)) {
		pr_err("%s: firmware init failed\n", __func__);
		goto err_init_fw;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	/* Add interfaces */
	for (i = 0; i < adapter->drv_mode->intf_num; i++) {
		if (!mwifiex_add_interface(adapter, i,
				adapter->drv_mode->bss_attr[i].bss_type)) {
			goto err_add_intf;
		}
	}

	up(sem);

	return 0;

err_add_intf:
	for (i = 0; i < adapter->priv_num; i++)
		mwifiex_remove_interface(adapter, i);
=======
	priv = adapter->priv[0];

	if (mwifiex_register_cfg80211(priv) != 0) {
		dev_err(adapter->dev, "cannot register netdevice"
			       " with cfg80211\n");
			goto err_init_fw;
	}

	rtnl_lock();
	/* Create station interface by default */
	if (!mwifiex_add_virtual_intf(priv->wdev->wiphy, "mlan%d",
				      NL80211_IFTYPE_STATION, NULL, NULL)) {
		rtnl_unlock();
		dev_err(adapter->dev, "cannot create default station"
				" interface\n");
		goto err_add_intf;
	}

	rtnl_unlock();

	up(sem);

	mwifiex_drv_get_driver_version(adapter, fmt, sizeof(fmt) - 1);
	dev_notice(adapter->dev, "driver_version = %s\n", fmt);

	return 0;

err_add_intf:
	rtnl_lock();
	mwifiex_del_virtual_intf(priv->wdev->wiphy, priv->netdev);
	rtnl_unlock();
>>>>>>> refs/remotes/origin/cm-10.0
err_init_fw:
	pr_debug("info: %s: unregister device\n", __func__);
	adapter->if_ops.unregister_dev(adapter);
err_registerdev:
	adapter->surprise_removed = true;
	mwifiex_terminate_workqueue(adapter);
err_kmalloc:
=======
	return 0;

err_init_fw:
	pr_debug("info: %s: unregister device\n", __func__);
	if (adapter->if_ops.unregister_dev)
		adapter->if_ops.unregister_dev(adapter);
>>>>>>> refs/remotes/origin/master
	if ((adapter->hw_status == MWIFIEX_HW_STATUS_FW_READY) ||
	    (adapter->hw_status == MWIFIEX_HW_STATUS_READY)) {
		pr_debug("info: %s: shutdown mwifiex\n", __func__);
		adapter->init_wait_q_woken = false;

		if (mwifiex_shutdown_drv(adapter) == -EINPROGRESS)
			wait_event_interruptible(adapter->init_wait_q,
						 adapter->init_wait_q_woken);
	}
<<<<<<< HEAD

=======
err_registerdev:
	adapter->surprise_removed = true;
	mwifiex_terminate_workqueue(adapter);
err_kmalloc:
>>>>>>> refs/remotes/origin/master
	mwifiex_free_adapter(adapter);

err_init_sw:
	up(sem);

exit_sem_err:
	return -1;
}
EXPORT_SYMBOL_GPL(mwifiex_add_card);

/*
 * This function removes the card.
 *
 * This function follows the following major steps to remove the device -
 *      - Stop data traffic
 *      - Shutdown firmware
 *      - Remove the logical interfaces
 *      - Terminate the work queue
 *      - Unregister the device
 *      - Free the adapter structure
 */
int mwifiex_remove_card(struct mwifiex_adapter *adapter, struct semaphore *sem)
{
	struct mwifiex_private *priv = NULL;
	int i;

	if (down_interruptible(sem))
		goto exit_sem_err;

	if (!adapter)
		goto exit_remove;

<<<<<<< HEAD
=======
	/* We can no longer handle interrupts once we start doing the teardown
	 * below. */
	if (adapter->if_ops.disable_int)
		adapter->if_ops.disable_int(adapter);

>>>>>>> refs/remotes/origin/master
	adapter->surprise_removed = true;

	/* Stop data */
	for (i = 0; i < adapter->priv_num; i++) {
		priv = adapter->priv[i];
<<<<<<< HEAD
<<<<<<< HEAD
		if (priv) {
			if (!netif_queue_stopped(priv->netdev))
				netif_stop_queue(priv->netdev);
=======
		if (priv && priv->netdev) {
			if (!netif_queue_stopped(priv->netdev))
				mwifiex_stop_net_dev_queue(priv->netdev,
							   adapter);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (priv && priv->netdev) {
			mwifiex_stop_net_dev_queue(priv->netdev, adapter);
>>>>>>> refs/remotes/origin/master
			if (netif_carrier_ok(priv->netdev))
				netif_carrier_off(priv->netdev);
		}
	}

	dev_dbg(adapter->dev, "cmd: calling mwifiex_shutdown_drv...\n");
	adapter->init_wait_q_woken = false;

	if (mwifiex_shutdown_drv(adapter) == -EINPROGRESS)
		wait_event_interruptible(adapter->init_wait_q,
					 adapter->init_wait_q_woken);
	dev_dbg(adapter->dev, "cmd: mwifiex_shutdown_drv done\n");
	if (atomic_read(&adapter->rx_pending) ||
	    atomic_read(&adapter->tx_pending) ||
	    atomic_read(&adapter->cmd_pending)) {
		dev_err(adapter->dev, "rx_pending=%d, tx_pending=%d, "
		       "cmd_pending=%d\n",
		       atomic_read(&adapter->rx_pending),
		       atomic_read(&adapter->tx_pending),
		       atomic_read(&adapter->cmd_pending));
	}

<<<<<<< HEAD
<<<<<<< HEAD
	/* Remove interface */
	for (i = 0; i < adapter->priv_num; i++)
		mwifiex_remove_interface(adapter, i);
=======
=======
>>>>>>> refs/remotes/origin/master
	for (i = 0; i < adapter->priv_num; i++) {
		priv = adapter->priv[i];

		if (!priv)
			continue;

		rtnl_lock();
		if (priv->wdev && priv->netdev)
<<<<<<< HEAD
			mwifiex_del_virtual_intf(priv->wdev->wiphy,
						 priv->netdev);
=======
			mwifiex_del_virtual_intf(adapter->wiphy, priv->wdev);
>>>>>>> refs/remotes/origin/master
		rtnl_unlock();
	}

	priv = adapter->priv[0];
<<<<<<< HEAD
	if (!priv)
		goto exit_remove;

	if (priv->wdev) {
		wiphy_unregister(priv->wdev->wiphy);
		wiphy_free(priv->wdev->wiphy);
		kfree(priv->wdev);
	}
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!priv || !priv->wdev)
		goto exit_remove;

	wiphy_unregister(priv->wdev->wiphy);
	wiphy_free(priv->wdev->wiphy);
>>>>>>> refs/remotes/origin/master

	mwifiex_terminate_workqueue(adapter);

	/* Unregister device */
	dev_dbg(adapter->dev, "info: unregister device\n");
<<<<<<< HEAD
	adapter->if_ops.unregister_dev(adapter);
=======
	if (adapter->if_ops.unregister_dev)
		adapter->if_ops.unregister_dev(adapter);
>>>>>>> refs/remotes/origin/master
	/* Free adapter structure */
	dev_dbg(adapter->dev, "info: free adapter\n");
	mwifiex_free_adapter(adapter);

exit_remove:
	up(sem);
exit_sem_err:
	return 0;
}
EXPORT_SYMBOL_GPL(mwifiex_remove_card);

/*
 * This function initializes the module.
 *
 * The debug FS is also initialized if configured.
 */
static int
mwifiex_init_module(void)
{
#ifdef CONFIG_DEBUG_FS
	mwifiex_debugfs_init();
#endif
	return 0;
}

/*
 * This function cleans up the module.
 *
 * The debug FS is removed if available.
 */
static void
mwifiex_cleanup_module(void)
{
#ifdef CONFIG_DEBUG_FS
	mwifiex_debugfs_remove();
#endif
}

module_init(mwifiex_init_module);
module_exit(mwifiex_cleanup_module);

MODULE_AUTHOR("Marvell International Ltd.");
MODULE_DESCRIPTION("Marvell WiFi-Ex Driver version " VERSION);
MODULE_VERSION(VERSION);
MODULE_LICENSE("GPL v2");
