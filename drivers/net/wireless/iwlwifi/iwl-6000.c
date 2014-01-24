/******************************************************************************
 *
<<<<<<< HEAD
 * Copyright(c) 2008 - 2011 Intel Corporation. All rights reserved.
=======
 * Copyright(c) 2008 - 2012 Intel Corporation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-10.0
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 * The full GNU General Public License is included in this distribution in the
 * file called LICENSE.
 *
 * Contact Information:
 *  Intel Linux Wireless <ilw@linux.intel.com>
 * Intel Corporation, 5200 N.E. Elam Young Parkway, Hillsboro, OR 97124-6497
 *
 *****************************************************************************/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
<<<<<<< HEAD
#include <linux/pci.h>
#include <linux/dma-mapping.h>
#include <linux/delay.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/wireless.h>
=======
#include <linux/delay.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <net/mac80211.h>
#include <linux/etherdevice.h>
#include <asm/unaligned.h>
#include <linux/stringify.h>

#include "iwl-eeprom.h"
#include "iwl-dev.h"
#include "iwl-core.h"
#include "iwl-io.h"
<<<<<<< HEAD
#include "iwl-sta.h"
#include "iwl-agn.h"
#include "iwl-helpers.h"
#include "iwl-agn-hw.h"
#include "iwl-6000-hw.h"

/* Highest firmware API version supported */
#define IWL6000_UCODE_API_MAX 4
#define IWL6050_UCODE_API_MAX 5
#define IWL6000G2_UCODE_API_MAX 5
=======
#include "iwl-agn.h"
#include "iwl-agn-hw.h"
#include "iwl-trans.h"
#include "iwl-shared.h"
#include "iwl-cfg.h"

/* Highest firmware API version supported */
#define IWL6000_UCODE_API_MAX 6
#define IWL6050_UCODE_API_MAX 5
#define IWL6000G2_UCODE_API_MAX 6
#define IWL6035_UCODE_API_MAX 6

/* Oldest version we won't warn about */
#define IWL6000_UCODE_API_OK 4
#define IWL6000G2_UCODE_API_OK 5
#define IWL6050_UCODE_API_OK 5
#define IWL6000G2B_UCODE_API_OK 6
#define IWL6035_UCODE_API_OK 6
>>>>>>> refs/remotes/origin/cm-10.0

/* Lowest firmware API version supported */
#define IWL6000_UCODE_API_MIN 4
#define IWL6050_UCODE_API_MIN 4
<<<<<<< HEAD
#define IWL6000G2_UCODE_API_MIN 4
=======
#define IWL6000G2_UCODE_API_MIN 5
#define IWL6035_UCODE_API_MIN 6
>>>>>>> refs/remotes/origin/cm-10.0

#define IWL6000_FW_PRE "iwlwifi-6000-"
#define IWL6000_MODULE_FIRMWARE(api) IWL6000_FW_PRE __stringify(api) ".ucode"

#define IWL6050_FW_PRE "iwlwifi-6050-"
#define IWL6050_MODULE_FIRMWARE(api) IWL6050_FW_PRE __stringify(api) ".ucode"

#define IWL6005_FW_PRE "iwlwifi-6000g2a-"
#define IWL6005_MODULE_FIRMWARE(api) IWL6005_FW_PRE __stringify(api) ".ucode"

#define IWL6030_FW_PRE "iwlwifi-6000g2b-"
#define IWL6030_MODULE_FIRMWARE(api) IWL6030_FW_PRE __stringify(api) ".ucode"

static void iwl6000_set_ct_threshold(struct iwl_priv *priv)
{
	/* want Celsius */
<<<<<<< HEAD
	priv->hw_params.ct_kill_threshold = CT_KILL_THRESHOLD;
	priv->hw_params.ct_kill_exit_threshold = CT_KILL_EXIT_THRESHOLD;
=======
	hw_params(priv).ct_kill_threshold = CT_KILL_THRESHOLD;
	hw_params(priv).ct_kill_exit_threshold = CT_KILL_EXIT_THRESHOLD;
>>>>>>> refs/remotes/origin/cm-10.0
}

static void iwl6050_additional_nic_config(struct iwl_priv *priv)
{
	/* Indicate calibration version to uCode. */
<<<<<<< HEAD
	if (iwlagn_eeprom_calib_version(priv) >= 6)
		iwl_set_bit(priv, CSR_GP_DRIVER_REG,
=======
	if (iwl_eeprom_calib_version(priv->shrd) >= 6)
		iwl_set_bit(trans(priv), CSR_GP_DRIVER_REG,
>>>>>>> refs/remotes/origin/cm-10.0
				CSR_GP_DRIVER_REG_BIT_CALIB_VERSION6);
}

static void iwl6150_additional_nic_config(struct iwl_priv *priv)
{
	/* Indicate calibration version to uCode. */
<<<<<<< HEAD
	if (iwlagn_eeprom_calib_version(priv) >= 6)
		iwl_set_bit(priv, CSR_GP_DRIVER_REG,
				CSR_GP_DRIVER_REG_BIT_CALIB_VERSION6);
	iwl_set_bit(priv, CSR_GP_DRIVER_REG,
		    CSR_GP_DRIVER_REG_BIT_6050_1x2);
}

/* NIC configuration for 6000 series */
static void iwl6000_nic_config(struct iwl_priv *priv)
{
	u16 radio_cfg;

	radio_cfg = iwl_eeprom_query16(priv, EEPROM_RADIO_CONFIG);

	/* write radio config values to register */
	if (EEPROM_RF_CFG_TYPE_MSK(radio_cfg) <= EEPROM_RF_CONFIG_TYPE_MAX)
		iwl_set_bit(priv, CSR_HW_IF_CONFIG_REG,
			    EEPROM_RF_CFG_TYPE_MSK(radio_cfg) |
			    EEPROM_RF_CFG_STEP_MSK(radio_cfg) |
			    EEPROM_RF_CFG_DASH_MSK(radio_cfg));

	/* set CSR_HW_CONFIG_REG for uCode use */
	iwl_set_bit(priv, CSR_HW_IF_CONFIG_REG,
		    CSR_HW_IF_CONFIG_REG_BIT_RADIO_SI |
		    CSR_HW_IF_CONFIG_REG_BIT_MAC_SI);

	/* no locking required for register write */
	if (priv->cfg->pa_type == IWL_PA_INTERNAL) {
		/* 2x2 IPA phy type */
		iwl_write32(priv, CSR_GP_DRIVER_REG,
			     CSR_GP_DRIVER_REG_BIT_RADIO_SKU_2x2_IPA);
	}
	/* do additional nic configuration if needed */
	if (priv->cfg->ops->nic &&
		priv->cfg->ops->nic->additional_nic_config) {
			priv->cfg->ops->nic->additional_nic_config(priv);
	}
}

static struct iwl_sensitivity_ranges iwl6000_sensitivity = {
	.min_nrg_cck = 97,
	.max_nrg_cck = 0, /* not used, set to 0 */
=======
	if (iwl_eeprom_calib_version(priv->shrd) >= 6)
		iwl_set_bit(trans(priv), CSR_GP_DRIVER_REG,
				CSR_GP_DRIVER_REG_BIT_CALIB_VERSION6);
	iwl_set_bit(trans(priv), CSR_GP_DRIVER_REG,
		    CSR_GP_DRIVER_REG_BIT_6050_1x2);
}

static void iwl6000i_additional_nic_config(struct iwl_priv *priv)
{
	/* 2x2 IPA phy type */
	iwl_write32(trans(priv), CSR_GP_DRIVER_REG,
		     CSR_GP_DRIVER_REG_BIT_RADIO_SKU_2x2_IPA);
}

/* NIC configuration for 6000 series */
static void iwl6000_nic_config(struct iwl_priv *priv)
{
	iwl_rf_config(priv);

	/* do additional nic configuration if needed */
	if (cfg(priv)->additional_nic_config)
		cfg(priv)->additional_nic_config(priv);
}

static const struct iwl_sensitivity_ranges iwl6000_sensitivity = {
	.min_nrg_cck = 110,
>>>>>>> refs/remotes/origin/cm-10.0
	.auto_corr_min_ofdm = 80,
	.auto_corr_min_ofdm_mrc = 128,
	.auto_corr_min_ofdm_x1 = 105,
	.auto_corr_min_ofdm_mrc_x1 = 192,

	.auto_corr_max_ofdm = 145,
	.auto_corr_max_ofdm_mrc = 232,
	.auto_corr_max_ofdm_x1 = 110,
	.auto_corr_max_ofdm_mrc_x1 = 232,

	.auto_corr_min_cck = 125,
	.auto_corr_max_cck = 175,
	.auto_corr_min_cck_mrc = 160,
	.auto_corr_max_cck_mrc = 310,
<<<<<<< HEAD
	.nrg_th_cck = 97,
	.nrg_th_ofdm = 100,

	.barker_corr_th_min = 190,
	.barker_corr_th_min_mrc = 390,
	.nrg_th_cca = 62,
};

static int iwl6000_hw_set_hw_params(struct iwl_priv *priv)
{
	if (iwlagn_mod_params.num_of_queues >= IWL_MIN_NUM_QUEUES &&
	    iwlagn_mod_params.num_of_queues <= IWLAGN_NUM_QUEUES)
		priv->cfg->base_params->num_of_queues =
			iwlagn_mod_params.num_of_queues;

	priv->hw_params.max_txq_num = priv->cfg->base_params->num_of_queues;
	priv->hw_params.dma_chnl_num = FH50_TCSR_CHNL_NUM;
	priv->hw_params.scd_bc_tbls_size =
			priv->cfg->base_params->num_of_queues *
			sizeof(struct iwlagn_scd_bc_tbl);
	priv->hw_params.tfd_size = sizeof(struct iwl_tfd);
	priv->hw_params.max_stations = IWLAGN_STATION_COUNT;
	priv->contexts[IWL_RXON_CTX_BSS].bcast_sta_id = IWLAGN_BROADCAST_ID;

	priv->hw_params.max_data_size = IWL60_RTC_DATA_SIZE;
	priv->hw_params.max_inst_size = IWL60_RTC_INST_SIZE;

	priv->hw_params.ht40_channel =  BIT(IEEE80211_BAND_2GHZ) |
					BIT(IEEE80211_BAND_5GHZ);
	priv->hw_params.rx_wrt_ptr_reg = FH_RSCSR_CHNL0_WPTR;

	priv->hw_params.tx_chains_num = num_of_ant(priv->cfg->valid_tx_ant);
	if (priv->cfg->rx_with_siso_diversity)
		priv->hw_params.rx_chains_num = 1;
	else
		priv->hw_params.rx_chains_num =
			num_of_ant(priv->cfg->valid_rx_ant);
	priv->hw_params.valid_tx_ant = priv->cfg->valid_tx_ant;
	priv->hw_params.valid_rx_ant = priv->cfg->valid_rx_ant;
=======
	.nrg_th_cck = 110,
	.nrg_th_ofdm = 110,

	.barker_corr_th_min = 190,
	.barker_corr_th_min_mrc = 336,
	.nrg_th_cca = 62,
};

static void iwl6000_hw_set_hw_params(struct iwl_priv *priv)
{
	hw_params(priv).ht40_channel =  BIT(IEEE80211_BAND_2GHZ) |
					BIT(IEEE80211_BAND_5GHZ);

	hw_params(priv).tx_chains_num =
		num_of_ant(hw_params(priv).valid_tx_ant);
	if (cfg(priv)->rx_with_siso_diversity)
		hw_params(priv).rx_chains_num = 1;
	else
		hw_params(priv).rx_chains_num =
			num_of_ant(hw_params(priv).valid_rx_ant);
>>>>>>> refs/remotes/origin/cm-10.0

	iwl6000_set_ct_threshold(priv);

	/* Set initial sensitivity parameters */
<<<<<<< HEAD
	/* Set initial calibration set */
	priv->hw_params.sens = &iwl6000_sensitivity;
	priv->hw_params.calib_init_cfg =
		BIT(IWL_CALIB_XTAL)		|
		BIT(IWL_CALIB_LO)		|
		BIT(IWL_CALIB_TX_IQ)		|
		BIT(IWL_CALIB_BASE_BAND);
	if (priv->cfg->need_dc_calib)
		priv->hw_params.calib_rt_cfg |= BIT(IWL_CALIB_CFG_DC_IDX);
	if (priv->cfg->need_temp_offset_calib)
		priv->hw_params.calib_init_cfg |= BIT(IWL_CALIB_TEMP_OFFSET);

	priv->hw_params.beacon_time_tsf_bits = IWLAGN_EXT_BEACON_TIME_POS;

	return 0;
=======
	hw_params(priv).sens = &iwl6000_sensitivity;

>>>>>>> refs/remotes/origin/cm-10.0
}

static int iwl6000_hw_channel_switch(struct iwl_priv *priv,
				     struct ieee80211_channel_switch *ch_switch)
{
	/*
	 * MULTI-FIXME
<<<<<<< HEAD
	 * See iwl_mac_channel_switch.
	 */
	struct iwl_rxon_context *ctx = &priv->contexts[IWL_RXON_CTX_BSS];
	struct iwl6000_channel_switch_cmd cmd;
=======
	 * See iwlagn_mac_channel_switch.
	 */
	struct iwl_rxon_context *ctx = &priv->contexts[IWL_RXON_CTX_BSS];
	struct iwl6000_channel_switch_cmd *cmd;
>>>>>>> refs/remotes/origin/cm-10.0
	const struct iwl_channel_info *ch_info;
	u32 switch_time_in_usec, ucode_switch_time;
	u16 ch;
	u32 tsf_low;
	u8 switch_count;
	u16 beacon_interval = le16_to_cpu(ctx->timing.beacon_interval);
	struct ieee80211_vif *vif = ctx->vif;
	struct iwl_host_cmd hcmd = {
		.id = REPLY_CHANNEL_SWITCH,
<<<<<<< HEAD
		.len = { sizeof(cmd), },
		.flags = CMD_SYNC,
		.data = { &cmd, },
	};

	cmd.band = priv->band == IEEE80211_BAND_2GHZ;
	ch = ch_switch->channel->hw_value;
	IWL_DEBUG_11H(priv, "channel switch from %u to %u\n",
		      ctx->active.channel, ch);
	cmd.channel = cpu_to_le16(ch);
	cmd.rxon_flags = ctx->staging.flags;
	cmd.rxon_filter_flags = ctx->staging.filter_flags;
=======
		.len = { sizeof(*cmd), },
		.flags = CMD_SYNC,
		.dataflags[0] = IWL_HCMD_DFL_NOCOPY,
	};
	int err;

	cmd = kzalloc(sizeof(*cmd), GFP_KERNEL);
	if (!cmd)
		return -ENOMEM;

	hcmd.data[0] = cmd;

	cmd->band = priv->band == IEEE80211_BAND_2GHZ;
	ch = ch_switch->channel->hw_value;
	IWL_DEBUG_11H(priv, "channel switch from %u to %u\n",
		      ctx->active.channel, ch);
	cmd->channel = cpu_to_le16(ch);
	cmd->rxon_flags = ctx->staging.flags;
	cmd->rxon_filter_flags = ctx->staging.filter_flags;
>>>>>>> refs/remotes/origin/cm-10.0
	switch_count = ch_switch->count;
	tsf_low = ch_switch->timestamp & 0x0ffffffff;
	/*
	 * calculate the ucode channel switch time
	 * adding TSF as one of the factor for when to switch
	 */
	if ((priv->ucode_beacon_time > tsf_low) && beacon_interval) {
		if (switch_count > ((priv->ucode_beacon_time - tsf_low) /
		    beacon_interval)) {
			switch_count -= (priv->ucode_beacon_time -
				tsf_low) / beacon_interval;
		} else
			switch_count = 0;
	}
	if (switch_count <= 1)
<<<<<<< HEAD
		cmd.switch_time = cpu_to_le32(priv->ucode_beacon_time);
=======
		cmd->switch_time = cpu_to_le32(priv->ucode_beacon_time);
>>>>>>> refs/remotes/origin/cm-10.0
	else {
		switch_time_in_usec =
			vif->bss_conf.beacon_int * switch_count * TIME_UNIT;
		ucode_switch_time = iwl_usecs_to_beacons(priv,
							 switch_time_in_usec,
							 beacon_interval);
<<<<<<< HEAD
		cmd.switch_time = iwl_add_beacon_time(priv,
=======
		cmd->switch_time = iwl_add_beacon_time(priv,
>>>>>>> refs/remotes/origin/cm-10.0
						      priv->ucode_beacon_time,
						      ucode_switch_time,
						      beacon_interval);
	}
	IWL_DEBUG_11H(priv, "uCode time for the switch is 0x%x\n",
<<<<<<< HEAD
		      cmd.switch_time);
	ch_info = iwl_get_channel_info(priv, priv->band, ch);
	if (ch_info)
		cmd.expect_beacon = is_channel_radar(ch_info);
=======
		      cmd->switch_time);
	ch_info = iwl_get_channel_info(priv, priv->band, ch);
	if (ch_info)
		cmd->expect_beacon = is_channel_radar(ch_info);
>>>>>>> refs/remotes/origin/cm-10.0
	else {
		IWL_ERR(priv, "invalid channel switch from %u to %u\n",
			ctx->active.channel, ch);
		return -EFAULT;
	}

<<<<<<< HEAD
	return iwl_send_cmd_sync(priv, &hcmd);
=======
	err = iwl_dvm_send_cmd(priv, &hcmd);
	kfree(cmd);
	return err;
>>>>>>> refs/remotes/origin/cm-10.0
}

static struct iwl_lib_ops iwl6000_lib = {
	.set_hw_params = iwl6000_hw_set_hw_params,
<<<<<<< HEAD
	.rx_handler_setup = iwlagn_rx_handler_setup,
	.setup_deferred_work = iwlagn_setup_deferred_work,
	.is_valid_rtc_data_addr = iwlagn_hw_valid_rtc_data_addr,
	.send_tx_power = iwlagn_send_tx_power,
	.update_chain_flags = iwl_update_chain_flags,
	.set_channel_switch = iwl6000_hw_channel_switch,
	.apm_ops = {
		.init = iwl_apm_init,
		.config = iwl6000_nic_config,
	},
=======
	.set_channel_switch = iwl6000_hw_channel_switch,
	.nic_config = iwl6000_nic_config,
>>>>>>> refs/remotes/origin/cm-10.0
	.eeprom_ops = {
		.regulatory_bands = {
			EEPROM_REG_BAND_1_CHANNELS,
			EEPROM_REG_BAND_2_CHANNELS,
			EEPROM_REG_BAND_3_CHANNELS,
			EEPROM_REG_BAND_4_CHANNELS,
			EEPROM_REG_BAND_5_CHANNELS,
			EEPROM_6000_REG_BAND_24_HT40_CHANNELS,
			EEPROM_REG_BAND_52_HT40_CHANNELS
		},
<<<<<<< HEAD
		.query_addr = iwlagn_eeprom_query_addr,
		.update_enhanced_txpower = iwlcore_eeprom_enhanced_txpower,
	},
	.temp_ops = {
		.temperature = iwlagn_temperature,
	 },
	.txfifo_flush = iwlagn_txfifo_flush,
	.dev_txfifo_flush = iwlagn_dev_txfifo_flush,
=======
		.enhanced_txpower = true,
	},
	.temperature = iwlagn_temperature,
>>>>>>> refs/remotes/origin/cm-10.0
};

static struct iwl_lib_ops iwl6030_lib = {
	.set_hw_params = iwl6000_hw_set_hw_params,
<<<<<<< HEAD
	.rx_handler_setup = iwlagn_bt_rx_handler_setup,
	.setup_deferred_work = iwlagn_bt_setup_deferred_work,
	.cancel_deferred_work = iwlagn_bt_cancel_deferred_work,
	.is_valid_rtc_data_addr = iwlagn_hw_valid_rtc_data_addr,
	.send_tx_power = iwlagn_send_tx_power,
	.update_chain_flags = iwl_update_chain_flags,
	.set_channel_switch = iwl6000_hw_channel_switch,
	.apm_ops = {
		.init = iwl_apm_init,
		.config = iwl6000_nic_config,
	},
=======
	.set_channel_switch = iwl6000_hw_channel_switch,
	.nic_config = iwl6000_nic_config,
>>>>>>> refs/remotes/origin/cm-10.0
	.eeprom_ops = {
		.regulatory_bands = {
			EEPROM_REG_BAND_1_CHANNELS,
			EEPROM_REG_BAND_2_CHANNELS,
			EEPROM_REG_BAND_3_CHANNELS,
			EEPROM_REG_BAND_4_CHANNELS,
			EEPROM_REG_BAND_5_CHANNELS,
			EEPROM_6000_REG_BAND_24_HT40_CHANNELS,
			EEPROM_REG_BAND_52_HT40_CHANNELS
		},
<<<<<<< HEAD
		.query_addr = iwlagn_eeprom_query_addr,
		.update_enhanced_txpower = iwlcore_eeprom_enhanced_txpower,
	},
	.temp_ops = {
		.temperature = iwlagn_temperature,
	 },
	.txfifo_flush = iwlagn_txfifo_flush,
	.dev_txfifo_flush = iwlagn_dev_txfifo_flush,
};

static struct iwl_nic_ops iwl6050_nic_ops = {
	.additional_nic_config = &iwl6050_additional_nic_config,
};

static struct iwl_nic_ops iwl6150_nic_ops = {
	.additional_nic_config = &iwl6150_additional_nic_config,
};

static const struct iwl_ops iwl6000_ops = {
	.lib = &iwl6000_lib,
	.hcmd = &iwlagn_hcmd,
	.utils = &iwlagn_hcmd_utils,
};

static const struct iwl_ops iwl6050_ops = {
	.lib = &iwl6000_lib,
	.hcmd = &iwlagn_hcmd,
	.utils = &iwlagn_hcmd_utils,
	.nic = &iwl6050_nic_ops,
};

static const struct iwl_ops iwl6150_ops = {
	.lib = &iwl6000_lib,
	.hcmd = &iwlagn_hcmd,
	.utils = &iwlagn_hcmd_utils,
	.nic = &iwl6150_nic_ops,
};

static const struct iwl_ops iwl6030_ops = {
	.lib = &iwl6030_lib,
	.hcmd = &iwlagn_bt_hcmd,
	.utils = &iwlagn_hcmd_utils,
};

static struct iwl_base_params iwl6000_base_params = {
=======
		.enhanced_txpower = true,
	},
	.temperature = iwlagn_temperature,
};

static const struct iwl_base_params iwl6000_base_params = {
>>>>>>> refs/remotes/origin/cm-10.0
	.eeprom_size = OTP_LOW_IMAGE_SIZE,
	.num_of_queues = IWLAGN_NUM_QUEUES,
	.num_of_ampdu_queues = IWLAGN_NUM_AMPDU_QUEUES,
	.pll_cfg_val = 0,
	.max_ll_items = OTP_MAX_LL_ITEMS_6x00,
	.shadow_ram_support = true,
	.led_compensation = 51,
<<<<<<< HEAD
	.chain_noise_num_beacons = IWL_CAL_NUM_BEACONS,
=======
>>>>>>> refs/remotes/origin/cm-10.0
	.adv_thermal_throttle = true,
	.support_ct_kill_exit = true,
	.plcp_delta_threshold = IWL_MAX_PLCP_ERR_THRESHOLD_DEF,
	.chain_noise_scale = 1000,
	.wd_timeout = IWL_DEF_WD_TIMEOUT,
	.max_event_log_size = 512,
<<<<<<< HEAD
	.shadow_reg_enable = true,
};

static struct iwl_base_params iwl6050_base_params = {
=======
	.shadow_reg_enable = false, /* TODO: fix bugs using this feature */
};

static const struct iwl_base_params iwl6050_base_params = {
>>>>>>> refs/remotes/origin/cm-10.0
	.eeprom_size = OTP_LOW_IMAGE_SIZE,
	.num_of_queues = IWLAGN_NUM_QUEUES,
	.num_of_ampdu_queues = IWLAGN_NUM_AMPDU_QUEUES,
	.pll_cfg_val = 0,
	.max_ll_items = OTP_MAX_LL_ITEMS_6x50,
	.shadow_ram_support = true,
	.led_compensation = 51,
<<<<<<< HEAD
	.chain_noise_num_beacons = IWL_CAL_NUM_BEACONS,
=======
>>>>>>> refs/remotes/origin/cm-10.0
	.adv_thermal_throttle = true,
	.support_ct_kill_exit = true,
	.plcp_delta_threshold = IWL_MAX_PLCP_ERR_THRESHOLD_DEF,
	.chain_noise_scale = 1500,
	.wd_timeout = IWL_DEF_WD_TIMEOUT,
	.max_event_log_size = 1024,
<<<<<<< HEAD
	.shadow_reg_enable = true,
};
static struct iwl_base_params iwl6000_g2_base_params = {
=======
	.shadow_reg_enable = false, /* TODO: fix bugs using this feature */
};

static const struct iwl_base_params iwl6000_g2_base_params = {
>>>>>>> refs/remotes/origin/cm-10.0
	.eeprom_size = OTP_LOW_IMAGE_SIZE,
	.num_of_queues = IWLAGN_NUM_QUEUES,
	.num_of_ampdu_queues = IWLAGN_NUM_AMPDU_QUEUES,
	.pll_cfg_val = 0,
	.max_ll_items = OTP_MAX_LL_ITEMS_6x00,
	.shadow_ram_support = true,
	.led_compensation = 57,
<<<<<<< HEAD
	.chain_noise_num_beacons = IWL_CAL_NUM_BEACONS,
=======
>>>>>>> refs/remotes/origin/cm-10.0
	.adv_thermal_throttle = true,
	.support_ct_kill_exit = true,
	.plcp_delta_threshold = IWL_MAX_PLCP_ERR_THRESHOLD_DEF,
	.chain_noise_scale = 1000,
	.wd_timeout = IWL_LONG_WD_TIMEOUT,
	.max_event_log_size = 512,
<<<<<<< HEAD
	.shadow_reg_enable = true,
};

static struct iwl_ht_params iwl6000_ht_params = {
=======
	.shadow_reg_enable = false, /* TODO: fix bugs using this feature */
};

static const struct iwl_ht_params iwl6000_ht_params = {
>>>>>>> refs/remotes/origin/cm-10.0
	.ht_greenfield_support = true,
	.use_rts_for_aggregation = true, /* use rts/cts protection */
};

<<<<<<< HEAD
static struct iwl_bt_params iwl6000_bt_params = {
=======
static const struct iwl_bt_params iwl6000_bt_params = {
>>>>>>> refs/remotes/origin/cm-10.0
	/* Due to bluetooth, we transmit 2.4 GHz probes only on antenna A */
	.advanced_bt_coexist = true,
	.agg_time_limit = BT_AGG_THRESHOLD_DEF,
	.bt_init_traffic_load = IWL_BT_COEX_TRAFFIC_LOAD_NONE,
	.bt_prio_boost = IWLAGN_BT_PRIO_BOOST_DEFAULT,
	.bt_sco_disable = true,
};

#define IWL_DEVICE_6005						\
<<<<<<< HEAD
	.fw_name_pre = IWL6005_FW_PRE,			\
	.ucode_api_max = IWL6000G2_UCODE_API_MAX,		\
	.ucode_api_min = IWL6000G2_UCODE_API_MIN,		\
	.eeprom_ver = EEPROM_6005_EEPROM_VERSION,		\
	.eeprom_calib_ver = EEPROM_6005_TX_POWER_VERSION,	\
	.ops = &iwl6000_ops,					\
	.base_params = &iwl6000_g2_base_params,			\
	.need_dc_calib = true,					\
	.need_temp_offset_calib = true,				\
	.led_mode = IWL_LED_RF_STATE

struct iwl_cfg iwl6005_2agn_cfg = {
=======
	.fw_name_pre = IWL6005_FW_PRE,				\
	.ucode_api_max = IWL6000G2_UCODE_API_MAX,		\
	.ucode_api_ok = IWL6000G2_UCODE_API_OK,			\
	.ucode_api_min = IWL6000G2_UCODE_API_MIN,		\
	.max_inst_size = IWL60_RTC_INST_SIZE,			\
	.max_data_size = IWL60_RTC_DATA_SIZE,			\
	.eeprom_ver = EEPROM_6005_EEPROM_VERSION,		\
	.eeprom_calib_ver = EEPROM_6005_TX_POWER_VERSION,	\
	.lib = &iwl6000_lib,					\
	.base_params = &iwl6000_g2_base_params,			\
	.need_temp_offset_calib = true,				\
	.led_mode = IWL_LED_RF_STATE

const struct iwl_cfg iwl6005_2agn_cfg = {
>>>>>>> refs/remotes/origin/cm-10.0
	.name = "Intel(R) Centrino(R) Advanced-N 6205 AGN",
	IWL_DEVICE_6005,
	.ht_params = &iwl6000_ht_params,
};

<<<<<<< HEAD
struct iwl_cfg iwl6005_2abg_cfg = {
=======
const struct iwl_cfg iwl6005_2abg_cfg = {
>>>>>>> refs/remotes/origin/cm-10.0
	.name = "Intel(R) Centrino(R) Advanced-N 6205 ABG",
	IWL_DEVICE_6005,
};

<<<<<<< HEAD
struct iwl_cfg iwl6005_2bg_cfg = {
=======
const struct iwl_cfg iwl6005_2bg_cfg = {
>>>>>>> refs/remotes/origin/cm-10.0
	.name = "Intel(R) Centrino(R) Advanced-N 6205 BG",
	IWL_DEVICE_6005,
};

<<<<<<< HEAD
#define IWL_DEVICE_6030						\
	.fw_name_pre = IWL6030_FW_PRE,			\
	.ucode_api_max = IWL6000G2_UCODE_API_MAX,		\
	.ucode_api_min = IWL6000G2_UCODE_API_MIN,		\
	.eeprom_ver = EEPROM_6030_EEPROM_VERSION,		\
	.eeprom_calib_ver = EEPROM_6030_TX_POWER_VERSION,	\
	.ops = &iwl6030_ops,					\
	.base_params = &iwl6000_g2_base_params,			\
	.bt_params = &iwl6000_bt_params,			\
	.need_dc_calib = true,					\
=======
const struct iwl_cfg iwl6005_2agn_sff_cfg = {
	.name = "Intel(R) Centrino(R) Advanced-N 6205S AGN",
	IWL_DEVICE_6005,
	.ht_params = &iwl6000_ht_params,
};

const struct iwl_cfg iwl6005_2agn_d_cfg = {
	.name = "Intel(R) Centrino(R) Advanced-N 6205D AGN",
	IWL_DEVICE_6005,
	.ht_params = &iwl6000_ht_params,
};

const struct iwl_cfg iwl6005_2agn_mow1_cfg = {
	.name = "Intel(R) Centrino(R) Advanced-N 6206 AGN",
	IWL_DEVICE_6005,
	.ht_params = &iwl6000_ht_params,
};

const struct iwl_cfg iwl6005_2agn_mow2_cfg = {
	.name = "Intel(R) Centrino(R) Advanced-N 6207 AGN",
	IWL_DEVICE_6005,
	.ht_params = &iwl6000_ht_params,
};

#define IWL_DEVICE_6030						\
	.fw_name_pre = IWL6030_FW_PRE,				\
	.ucode_api_max = IWL6000G2_UCODE_API_MAX,		\
	.ucode_api_ok = IWL6000G2B_UCODE_API_OK,		\
	.ucode_api_min = IWL6000G2_UCODE_API_MIN,		\
	.max_inst_size = IWL60_RTC_INST_SIZE,			\
	.max_data_size = IWL60_RTC_DATA_SIZE,			\
	.eeprom_ver = EEPROM_6030_EEPROM_VERSION,		\
	.eeprom_calib_ver = EEPROM_6030_TX_POWER_VERSION,	\
	.lib = &iwl6030_lib,					\
	.base_params = &iwl6000_g2_base_params,			\
	.bt_params = &iwl6000_bt_params,			\
>>>>>>> refs/remotes/origin/cm-10.0
	.need_temp_offset_calib = true,				\
	.led_mode = IWL_LED_RF_STATE,				\
	.adv_pm = true						\

<<<<<<< HEAD
struct iwl_cfg iwl6030_2agn_cfg = {
=======
const struct iwl_cfg iwl6030_2agn_cfg = {
>>>>>>> refs/remotes/origin/cm-10.0
	.name = "Intel(R) Centrino(R) Advanced-N 6230 AGN",
	IWL_DEVICE_6030,
	.ht_params = &iwl6000_ht_params,
};

<<<<<<< HEAD
struct iwl_cfg iwl6030_2abg_cfg = {
=======
const struct iwl_cfg iwl6030_2abg_cfg = {
>>>>>>> refs/remotes/origin/cm-10.0
	.name = "Intel(R) Centrino(R) Advanced-N 6230 ABG",
	IWL_DEVICE_6030,
};

<<<<<<< HEAD
struct iwl_cfg iwl6030_2bgn_cfg = {
=======
const struct iwl_cfg iwl6030_2bgn_cfg = {
>>>>>>> refs/remotes/origin/cm-10.0
	.name = "Intel(R) Centrino(R) Advanced-N 6230 BGN",
	IWL_DEVICE_6030,
	.ht_params = &iwl6000_ht_params,
};

<<<<<<< HEAD
struct iwl_cfg iwl6030_2bg_cfg = {
=======
const struct iwl_cfg iwl6030_2bg_cfg = {
>>>>>>> refs/remotes/origin/cm-10.0
	.name = "Intel(R) Centrino(R) Advanced-N 6230 BG",
	IWL_DEVICE_6030,
};

<<<<<<< HEAD
struct iwl_cfg iwl6035_2agn_cfg = {
	.name = "6035 Series 2x2 AGN/BT",
	IWL_DEVICE_6030,
	.ht_params = &iwl6000_ht_params,
};

struct iwl_cfg iwl6035_2abg_cfg = {
	.name = "6035 Series 2x2 ABG/BT",
	IWL_DEVICE_6030,
};

struct iwl_cfg iwl6035_2bg_cfg = {
	.name = "6035 Series 2x2 BG/BT",
	IWL_DEVICE_6030,
};

struct iwl_cfg iwl1030_bgn_cfg = {
=======
#define IWL_DEVICE_6035						\
	.fw_name_pre = IWL6030_FW_PRE,				\
	.ucode_api_max = IWL6035_UCODE_API_MAX,			\
	.ucode_api_ok = IWL6035_UCODE_API_OK,			\
	.ucode_api_min = IWL6035_UCODE_API_MIN,			\
	.max_inst_size = IWL60_RTC_INST_SIZE,			\
	.max_data_size = IWL60_RTC_DATA_SIZE,			\
	.eeprom_ver = EEPROM_6030_EEPROM_VERSION,		\
	.eeprom_calib_ver = EEPROM_6030_TX_POWER_VERSION,	\
	.lib = &iwl6030_lib,					\
	.base_params = &iwl6000_g2_base_params,			\
	.bt_params = &iwl6000_bt_params,			\
	.need_temp_offset_calib = true,				\
	.led_mode = IWL_LED_RF_STATE,				\
	.adv_pm = true

const struct iwl_cfg iwl6035_2agn_cfg = {
	.name = "Intel(R) Centrino(R) Advanced-N 6235 AGN",
	IWL_DEVICE_6035,
	.ht_params = &iwl6000_ht_params,
};

const struct iwl_cfg iwl1030_bgn_cfg = {
>>>>>>> refs/remotes/origin/cm-10.0
	.name = "Intel(R) Centrino(R) Wireless-N 1030 BGN",
	IWL_DEVICE_6030,
	.ht_params = &iwl6000_ht_params,
};

<<<<<<< HEAD
struct iwl_cfg iwl1030_bg_cfg = {
=======
const struct iwl_cfg iwl1030_bg_cfg = {
>>>>>>> refs/remotes/origin/cm-10.0
	.name = "Intel(R) Centrino(R) Wireless-N 1030 BG",
	IWL_DEVICE_6030,
};

<<<<<<< HEAD
struct iwl_cfg iwl130_bgn_cfg = {
=======
const struct iwl_cfg iwl130_bgn_cfg = {
>>>>>>> refs/remotes/origin/cm-10.0
	.name = "Intel(R) Centrino(R) Wireless-N 130 BGN",
	IWL_DEVICE_6030,
	.ht_params = &iwl6000_ht_params,
	.rx_with_siso_diversity = true,
};

<<<<<<< HEAD
struct iwl_cfg iwl130_bg_cfg = {
=======
const struct iwl_cfg iwl130_bg_cfg = {
>>>>>>> refs/remotes/origin/cm-10.0
	.name = "Intel(R) Centrino(R) Wireless-N 130 BG",
	IWL_DEVICE_6030,
	.rx_with_siso_diversity = true,
};

/*
 * "i": Internal configuration, use internal Power Amplifier
 */
#define IWL_DEVICE_6000i					\
	.fw_name_pre = IWL6000_FW_PRE,				\
	.ucode_api_max = IWL6000_UCODE_API_MAX,			\
<<<<<<< HEAD
	.ucode_api_min = IWL6000_UCODE_API_MIN,			\
=======
	.ucode_api_ok = IWL6000_UCODE_API_OK,			\
	.ucode_api_min = IWL6000_UCODE_API_MIN,			\
	.max_inst_size = IWL60_RTC_INST_SIZE,			\
	.max_data_size = IWL60_RTC_DATA_SIZE,			\
>>>>>>> refs/remotes/origin/cm-10.0
	.valid_tx_ant = ANT_BC,		/* .cfg overwrite */	\
	.valid_rx_ant = ANT_BC,		/* .cfg overwrite */	\
	.eeprom_ver = EEPROM_6000_EEPROM_VERSION,		\
	.eeprom_calib_ver = EEPROM_6000_TX_POWER_VERSION,	\
<<<<<<< HEAD
	.ops = &iwl6000_ops,					\
	.base_params = &iwl6000_base_params,			\
	.pa_type = IWL_PA_INTERNAL,				\
	.led_mode = IWL_LED_BLINK

struct iwl_cfg iwl6000i_2agn_cfg = {
=======
	.lib = &iwl6000_lib,					\
	.additional_nic_config = iwl6000i_additional_nic_config,\
	.base_params = &iwl6000_base_params,			\
	.led_mode = IWL_LED_BLINK

const struct iwl_cfg iwl6000i_2agn_cfg = {
>>>>>>> refs/remotes/origin/cm-10.0
	.name = "Intel(R) Centrino(R) Advanced-N 6200 AGN",
	IWL_DEVICE_6000i,
	.ht_params = &iwl6000_ht_params,
};

<<<<<<< HEAD
struct iwl_cfg iwl6000i_2abg_cfg = {
=======
const struct iwl_cfg iwl6000i_2abg_cfg = {
>>>>>>> refs/remotes/origin/cm-10.0
	.name = "Intel(R) Centrino(R) Advanced-N 6200 ABG",
	IWL_DEVICE_6000i,
};

<<<<<<< HEAD
struct iwl_cfg iwl6000i_2bg_cfg = {
=======
const struct iwl_cfg iwl6000i_2bg_cfg = {
>>>>>>> refs/remotes/origin/cm-10.0
	.name = "Intel(R) Centrino(R) Advanced-N 6200 BG",
	IWL_DEVICE_6000i,
};

#define IWL_DEVICE_6050						\
	.fw_name_pre = IWL6050_FW_PRE,				\
	.ucode_api_max = IWL6050_UCODE_API_MAX,			\
	.ucode_api_min = IWL6050_UCODE_API_MIN,			\
<<<<<<< HEAD
	.valid_tx_ant = ANT_AB,		/* .cfg overwrite */	\
	.valid_rx_ant = ANT_AB,		/* .cfg overwrite */	\
	.ops = &iwl6050_ops,					\
	.eeprom_ver = EEPROM_6050_EEPROM_VERSION,		\
	.eeprom_calib_ver = EEPROM_6050_TX_POWER_VERSION,	\
	.base_params = &iwl6050_base_params,			\
	.need_dc_calib = true,					\
	.led_mode = IWL_LED_BLINK,				\
	.internal_wimax_coex = true

struct iwl_cfg iwl6050_2agn_cfg = {
=======
	.max_inst_size = IWL60_RTC_INST_SIZE,			\
	.max_data_size = IWL60_RTC_DATA_SIZE,			\
	.valid_tx_ant = ANT_AB,		/* .cfg overwrite */	\
	.valid_rx_ant = ANT_AB,		/* .cfg overwrite */	\
	.lib = &iwl6000_lib,					\
	.additional_nic_config = iwl6050_additional_nic_config,	\
	.eeprom_ver = EEPROM_6050_EEPROM_VERSION,		\
	.eeprom_calib_ver = EEPROM_6050_TX_POWER_VERSION,	\
	.base_params = &iwl6050_base_params,			\
	.led_mode = IWL_LED_BLINK,				\
	.internal_wimax_coex = true

const struct iwl_cfg iwl6050_2agn_cfg = {
>>>>>>> refs/remotes/origin/cm-10.0
	.name = "Intel(R) Centrino(R) Advanced-N + WiMAX 6250 AGN",
	IWL_DEVICE_6050,
	.ht_params = &iwl6000_ht_params,
};

<<<<<<< HEAD
struct iwl_cfg iwl6050_2abg_cfg = {
=======
const struct iwl_cfg iwl6050_2abg_cfg = {
>>>>>>> refs/remotes/origin/cm-10.0
	.name = "Intel(R) Centrino(R) Advanced-N + WiMAX 6250 ABG",
	IWL_DEVICE_6050,
};

#define IWL_DEVICE_6150						\
	.fw_name_pre = IWL6050_FW_PRE,				\
	.ucode_api_max = IWL6050_UCODE_API_MAX,			\
	.ucode_api_min = IWL6050_UCODE_API_MIN,			\
<<<<<<< HEAD
	.ops = &iwl6150_ops,					\
	.eeprom_ver = EEPROM_6150_EEPROM_VERSION,		\
	.eeprom_calib_ver = EEPROM_6150_TX_POWER_VERSION,	\
	.base_params = &iwl6050_base_params,			\
	.need_dc_calib = true,					\
	.led_mode = IWL_LED_BLINK,				\
	.internal_wimax_coex = true

struct iwl_cfg iwl6150_bgn_cfg = {
=======
	.max_inst_size = IWL60_RTC_INST_SIZE,			\
	.max_data_size = IWL60_RTC_DATA_SIZE,			\
	.lib = &iwl6000_lib,					\
	.additional_nic_config = iwl6150_additional_nic_config,	\
	.eeprom_ver = EEPROM_6150_EEPROM_VERSION,		\
	.eeprom_calib_ver = EEPROM_6150_TX_POWER_VERSION,	\
	.base_params = &iwl6050_base_params,			\
	.led_mode = IWL_LED_BLINK,				\
	.internal_wimax_coex = true

const struct iwl_cfg iwl6150_bgn_cfg = {
>>>>>>> refs/remotes/origin/cm-10.0
	.name = "Intel(R) Centrino(R) Wireless-N + WiMAX 6150 BGN",
	IWL_DEVICE_6150,
	.ht_params = &iwl6000_ht_params,
};

<<<<<<< HEAD
struct iwl_cfg iwl6150_bg_cfg = {
=======
const struct iwl_cfg iwl6150_bg_cfg = {
>>>>>>> refs/remotes/origin/cm-10.0
	.name = "Intel(R) Centrino(R) Wireless-N + WiMAX 6150 BG",
	IWL_DEVICE_6150,
};

<<<<<<< HEAD
struct iwl_cfg iwl6000_3agn_cfg = {
	.name = "Intel(R) Centrino(R) Ultimate-N 6300 AGN",
	.fw_name_pre = IWL6000_FW_PRE,
	.ucode_api_max = IWL6000_UCODE_API_MAX,
	.ucode_api_min = IWL6000_UCODE_API_MIN,
	.eeprom_ver = EEPROM_6000_EEPROM_VERSION,
	.eeprom_calib_ver = EEPROM_6000_TX_POWER_VERSION,
	.ops = &iwl6000_ops,
	.base_params = &iwl6000_base_params,
	.ht_params = &iwl6000_ht_params,
	.need_dc_calib = true,
	.led_mode = IWL_LED_BLINK,
};

MODULE_FIRMWARE(IWL6000_MODULE_FIRMWARE(IWL6000_UCODE_API_MAX));
MODULE_FIRMWARE(IWL6050_MODULE_FIRMWARE(IWL6050_UCODE_API_MAX));
MODULE_FIRMWARE(IWL6005_MODULE_FIRMWARE(IWL6000G2_UCODE_API_MAX));
MODULE_FIRMWARE(IWL6030_MODULE_FIRMWARE(IWL6000G2_UCODE_API_MAX));
=======
const struct iwl_cfg iwl6000_3agn_cfg = {
	.name = "Intel(R) Centrino(R) Ultimate-N 6300 AGN",
	.fw_name_pre = IWL6000_FW_PRE,
	.ucode_api_max = IWL6000_UCODE_API_MAX,
	.ucode_api_ok = IWL6000_UCODE_API_OK,
	.ucode_api_min = IWL6000_UCODE_API_MIN,
	.max_inst_size = IWL60_RTC_INST_SIZE,
	.max_data_size = IWL60_RTC_DATA_SIZE,
	.eeprom_ver = EEPROM_6000_EEPROM_VERSION,
	.eeprom_calib_ver = EEPROM_6000_TX_POWER_VERSION,
	.lib = &iwl6000_lib,
	.base_params = &iwl6000_base_params,
	.ht_params = &iwl6000_ht_params,
	.led_mode = IWL_LED_BLINK,
};

MODULE_FIRMWARE(IWL6000_MODULE_FIRMWARE(IWL6000_UCODE_API_OK));
MODULE_FIRMWARE(IWL6050_MODULE_FIRMWARE(IWL6050_UCODE_API_OK));
MODULE_FIRMWARE(IWL6005_MODULE_FIRMWARE(IWL6000G2_UCODE_API_OK));
MODULE_FIRMWARE(IWL6030_MODULE_FIRMWARE(IWL6000G2B_UCODE_API_OK));
>>>>>>> refs/remotes/origin/cm-10.0
