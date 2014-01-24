/*
 * QLogic qlcnic NIC Driver
<<<<<<< HEAD
 * Copyright (c)  2009-2010 QLogic Corporation
=======
 * Copyright (c) 2009-2013 QLogic Corporation
>>>>>>> refs/remotes/origin/master
 *
 * See LICENSE.qlcnic for copyright and licensing details.
 */

#include <linux/types.h>
#include <linux/delay.h>
#include <linux/pci.h>
#include <linux/io.h>
#include <linux/netdevice.h>
#include <linux/ethtool.h>

#include "qlcnic.h"

struct qlcnic_stats {
	char stat_string[ETH_GSTRING_LEN];
	int sizeof_stat;
	int stat_offset;
};

#define QLC_SIZEOF(m) FIELD_SIZEOF(struct qlcnic_adapter, m)
#define QLC_OFF(m) offsetof(struct qlcnic_adapter, m)
<<<<<<< HEAD

static const struct qlcnic_stats qlcnic_gstrings_stats[] = {
	{"xmit_called",
		QLC_SIZEOF(stats.xmitcalled), QLC_OFF(stats.xmitcalled)},
	{"xmit_finished",
		QLC_SIZEOF(stats.xmitfinished), QLC_OFF(stats.xmitfinished)},
	{"rx_dropped",
		QLC_SIZEOF(stats.rxdropped), QLC_OFF(stats.rxdropped)},
	{"tx_dropped",
		QLC_SIZEOF(stats.txdropped), QLC_OFF(stats.txdropped)},
	{"csummed",
		QLC_SIZEOF(stats.csummed), QLC_OFF(stats.csummed)},
	{"rx_pkts",
		QLC_SIZEOF(stats.rx_pkts), QLC_OFF(stats.rx_pkts)},
	{"lro_pkts",
		QLC_SIZEOF(stats.lro_pkts), QLC_OFF(stats.lro_pkts)},
	{"rx_bytes",
		QLC_SIZEOF(stats.rxbytes), QLC_OFF(stats.rxbytes)},
	{"tx_bytes",
		QLC_SIZEOF(stats.txbytes), QLC_OFF(stats.txbytes)},
	{"lrobytes",
		QLC_SIZEOF(stats.lrobytes), QLC_OFF(stats.lrobytes)},
	{"lso_frames",
		QLC_SIZEOF(stats.lso_frames), QLC_OFF(stats.lso_frames)},
	{"xmit_on",
		QLC_SIZEOF(stats.xmit_on), QLC_OFF(stats.xmit_on)},
	{"xmit_off",
		QLC_SIZEOF(stats.xmit_off), QLC_OFF(stats.xmit_off)},
	{"skb_alloc_failure", QLC_SIZEOF(stats.skb_alloc_failure),
		QLC_OFF(stats.skb_alloc_failure)},
	{"null rxbuf",
		QLC_SIZEOF(stats.null_rxbuf), QLC_OFF(stats.null_rxbuf)},
	{"rx dma map error", QLC_SIZEOF(stats.rx_dma_map_error),
					 QLC_OFF(stats.rx_dma_map_error)},
	{"tx dma map error", QLC_SIZEOF(stats.tx_dma_map_error),
					 QLC_OFF(stats.tx_dma_map_error)},
=======
static const u32 qlcnic_fw_dump_level[] = {
	0x3, 0x7, 0xf, 0x1f, 0x3f, 0x7f, 0xff
};

static const struct qlcnic_stats qlcnic_gstrings_stats[] = {
	{"xmit_on", QLC_SIZEOF(stats.xmit_on), QLC_OFF(stats.xmit_on)},
	{"xmit_off", QLC_SIZEOF(stats.xmit_off), QLC_OFF(stats.xmit_off)},
	{"xmit_called", QLC_SIZEOF(stats.xmitcalled),
	 QLC_OFF(stats.xmitcalled)},
	{"xmit_finished", QLC_SIZEOF(stats.xmitfinished),
	 QLC_OFF(stats.xmitfinished)},
	{"tx dma map error", QLC_SIZEOF(stats.tx_dma_map_error),
	 QLC_OFF(stats.tx_dma_map_error)},
	{"tx_bytes", QLC_SIZEOF(stats.txbytes), QLC_OFF(stats.txbytes)},
	{"tx_dropped", QLC_SIZEOF(stats.txdropped), QLC_OFF(stats.txdropped)},
	{"rx dma map error", QLC_SIZEOF(stats.rx_dma_map_error),
	 QLC_OFF(stats.rx_dma_map_error)},
	{"rx_pkts", QLC_SIZEOF(stats.rx_pkts), QLC_OFF(stats.rx_pkts)},
	{"rx_bytes", QLC_SIZEOF(stats.rxbytes), QLC_OFF(stats.rxbytes)},
	{"rx_dropped", QLC_SIZEOF(stats.rxdropped), QLC_OFF(stats.rxdropped)},
	{"null rxbuf", QLC_SIZEOF(stats.null_rxbuf), QLC_OFF(stats.null_rxbuf)},
	{"csummed", QLC_SIZEOF(stats.csummed), QLC_OFF(stats.csummed)},
	{"lro_pkts", QLC_SIZEOF(stats.lro_pkts), QLC_OFF(stats.lro_pkts)},
	{"lrobytes", QLC_SIZEOF(stats.lrobytes), QLC_OFF(stats.lrobytes)},
	{"lso_frames", QLC_SIZEOF(stats.lso_frames), QLC_OFF(stats.lso_frames)},
	{"skb_alloc_failure", QLC_SIZEOF(stats.skb_alloc_failure),
	 QLC_OFF(stats.skb_alloc_failure)},
	{"mac_filter_limit_overrun", QLC_SIZEOF(stats.mac_filter_limit_overrun),
	 QLC_OFF(stats.mac_filter_limit_overrun)},
	{"spurious intr", QLC_SIZEOF(stats.spurious_intr),
	 QLC_OFF(stats.spurious_intr)},
>>>>>>> refs/remotes/origin/master

};

static const char qlcnic_device_gstrings_stats[][ETH_GSTRING_LEN] = {
<<<<<<< HEAD
	"rx unicast frames",
	"rx multicast frames",
	"rx broadcast frames",
	"rx dropped frames",
	"rx errors",
	"rx local frames",
	"rx numbytes",
=======
>>>>>>> refs/remotes/origin/master
	"tx unicast frames",
	"tx multicast frames",
	"tx broadcast frames",
	"tx dropped frames",
	"tx errors",
	"tx local frames",
	"tx numbytes",
<<<<<<< HEAD
};

#define QLCNIC_STATS_LEN	ARRAY_SIZE(qlcnic_gstrings_stats)
#define QLCNIC_DEVICE_STATS_LEN	ARRAY_SIZE(qlcnic_device_gstrings_stats)
=======
	"rx unicast frames",
	"rx multicast frames",
	"rx broadcast frames",
	"rx dropped frames",
	"rx errors",
	"rx local frames",
	"rx numbytes",
};

static const char qlcnic_83xx_tx_stats_strings[][ETH_GSTRING_LEN] = {
	"ctx_tx_bytes",
	"ctx_tx_pkts",
	"ctx_tx_errors",
	"ctx_tx_dropped_pkts",
	"ctx_tx_num_buffers",
};

static const char qlcnic_83xx_mac_stats_strings[][ETH_GSTRING_LEN] = {
	"mac_tx_frames",
	"mac_tx_bytes",
	"mac_tx_mcast_pkts",
	"mac_tx_bcast_pkts",
	"mac_tx_pause_cnt",
	"mac_tx_ctrl_pkt",
	"mac_tx_lt_64b_pkts",
	"mac_tx_lt_127b_pkts",
	"mac_tx_lt_255b_pkts",
	"mac_tx_lt_511b_pkts",
	"mac_tx_lt_1023b_pkts",
	"mac_tx_lt_1518b_pkts",
	"mac_tx_gt_1518b_pkts",
	"mac_rx_frames",
	"mac_rx_bytes",
	"mac_rx_mcast_pkts",
	"mac_rx_bcast_pkts",
	"mac_rx_pause_cnt",
	"mac_rx_ctrl_pkt",
	"mac_rx_lt_64b_pkts",
	"mac_rx_lt_127b_pkts",
	"mac_rx_lt_255b_pkts",
	"mac_rx_lt_511b_pkts",
	"mac_rx_lt_1023b_pkts",
	"mac_rx_lt_1518b_pkts",
	"mac_rx_gt_1518b_pkts",
	"mac_rx_length_error",
	"mac_rx_length_small",
	"mac_rx_length_large",
	"mac_rx_jabber",
	"mac_rx_dropped",
	"mac_crc_error",
	"mac_align_error",
	"eswitch_frames",
	"eswitch_bytes",
	"eswitch_multicast_frames",
	"eswitch_broadcast_frames",
	"eswitch_unicast_frames",
	"eswitch_error_free_frames",
	"eswitch_error_free_bytes",
};

#define QLCNIC_STATS_LEN	ARRAY_SIZE(qlcnic_gstrings_stats)

static const char qlcnic_tx_queue_stats_strings[][ETH_GSTRING_LEN] = {
	"xmit_on",
	"xmit_off",
	"xmit_called",
	"xmit_finished",
	"tx_bytes",
};

#define QLCNIC_TX_STATS_LEN	ARRAY_SIZE(qlcnic_tx_queue_stats_strings)

static const char qlcnic_83xx_rx_stats_strings[][ETH_GSTRING_LEN] = {
	"ctx_rx_bytes",
	"ctx_rx_pkts",
	"ctx_lro_pkt_cnt",
	"ctx_ip_csum_error",
	"ctx_rx_pkts_wo_ctx",
	"ctx_rx_pkts_drop_wo_sds_on_card",
	"ctx_rx_pkts_drop_wo_sds_on_host",
	"ctx_rx_osized_pkts",
	"ctx_rx_pkts_dropped_wo_rds",
	"ctx_rx_unexpected_mcast_pkts",
	"ctx_invalid_mac_address",
	"ctx_rx_rds_ring_prim_attempted",
	"ctx_rx_rds_ring_prim_success",
	"ctx_num_lro_flows_added",
	"ctx_num_lro_flows_removed",
	"ctx_num_lro_flows_active",
	"ctx_pkts_dropped_unknown",
};
>>>>>>> refs/remotes/origin/master

static const char qlcnic_gstrings_test[][ETH_GSTRING_LEN] = {
	"Register_Test_on_offline",
	"Link_Test_on_offline",
	"Interrupt_Test_offline",
	"Internal_Loopback_offline",
<<<<<<< HEAD
	"External_Loopback_offline"
=======
	"External_Loopback_offline",
	"EEPROM_Test_offline"
>>>>>>> refs/remotes/origin/master
};

#define QLCNIC_TEST_LEN	ARRAY_SIZE(qlcnic_gstrings_test)

<<<<<<< HEAD
#define QLCNIC_RING_REGS_COUNT	20
#define QLCNIC_RING_REGS_LEN	(QLCNIC_RING_REGS_COUNT * sizeof(u32))
#define QLCNIC_MAX_EEPROM_LEN   1024

static const u32 diag_registers[] = {
	CRB_CMDPEG_STATE,
	CRB_RCVPEG_STATE,
	CRB_XG_STATE_P3P,
	CRB_FW_CAPABILITIES_1,
	ISR_INT_STATE_REG,
=======
static inline int qlcnic_82xx_statistics(struct qlcnic_adapter *adapter)
{
	return ARRAY_SIZE(qlcnic_gstrings_stats) +
	       ARRAY_SIZE(qlcnic_83xx_mac_stats_strings) +
	       QLCNIC_TX_STATS_LEN * adapter->drv_tx_rings;
}

static inline int qlcnic_83xx_statistics(struct qlcnic_adapter *adapter)
{
	return ARRAY_SIZE(qlcnic_gstrings_stats) +
	       ARRAY_SIZE(qlcnic_83xx_tx_stats_strings) +
	       ARRAY_SIZE(qlcnic_83xx_mac_stats_strings) +
	       ARRAY_SIZE(qlcnic_83xx_rx_stats_strings) +
	       QLCNIC_TX_STATS_LEN * adapter->drv_tx_rings;
}

static int qlcnic_dev_statistics_len(struct qlcnic_adapter *adapter)
{
	int len = -1;

	if (qlcnic_82xx_check(adapter)) {
		len = qlcnic_82xx_statistics(adapter);
		if (adapter->flags & QLCNIC_ESWITCH_ENABLED)
			len += ARRAY_SIZE(qlcnic_device_gstrings_stats);
	} else if (qlcnic_83xx_check(adapter)) {
		len = qlcnic_83xx_statistics(adapter);
	}

	return len;
}

#define	QLCNIC_TX_INTR_NOT_CONFIGURED	0X78563412

#define QLCNIC_MAX_EEPROM_LEN   1024

static const u32 diag_registers[] = {
	QLCNIC_CMDPEG_STATE,
	QLCNIC_RCVPEG_STATE,
	QLCNIC_FW_CAPABILITIES,
>>>>>>> refs/remotes/origin/master
	QLCNIC_CRB_DRV_ACTIVE,
	QLCNIC_CRB_DEV_STATE,
	QLCNIC_CRB_DRV_STATE,
	QLCNIC_CRB_DRV_SCRATCH,
	QLCNIC_CRB_DEV_PARTITION_INFO,
	QLCNIC_CRB_DRV_IDC_VER,
	QLCNIC_PEG_ALIVE_COUNTER,
	QLCNIC_PEG_HALT_STATUS1,
	QLCNIC_PEG_HALT_STATUS2,
<<<<<<< HEAD
=======
	-1
};


static const u32 ext_diag_registers[] = {
	CRB_XG_STATE_P3P,
	ISR_INT_STATE_REG,
>>>>>>> refs/remotes/origin/master
	QLCNIC_CRB_PEG_NET_0+0x3c,
	QLCNIC_CRB_PEG_NET_1+0x3c,
	QLCNIC_CRB_PEG_NET_2+0x3c,
	QLCNIC_CRB_PEG_NET_4+0x3c,
	-1
};

#define QLCNIC_MGMT_API_VERSION	2
<<<<<<< HEAD
#define QLCNIC_DEV_INFO_SIZE	1
#define QLCNIC_ETHTOOL_REGS_VER	2
static int qlcnic_get_regs_len(struct net_device *dev)
{
	return sizeof(diag_registers) + QLCNIC_RING_REGS_LEN +
				QLCNIC_DEV_INFO_SIZE + 1;
=======
#define QLCNIC_ETHTOOL_REGS_VER	4

static inline int qlcnic_get_ring_regs_len(struct qlcnic_adapter *adapter)
{
	int ring_regs_cnt = (adapter->drv_tx_rings * 5) +
			    (adapter->max_rds_rings * 2) +
			    (adapter->drv_sds_rings * 3) + 5;
	return ring_regs_cnt * sizeof(u32);
}

static int qlcnic_get_regs_len(struct net_device *dev)
{
	struct qlcnic_adapter *adapter = netdev_priv(dev);
	u32 len;

	if (qlcnic_83xx_check(adapter))
		len = qlcnic_83xx_get_regs_len(adapter);
	else
		len = sizeof(ext_diag_registers) + sizeof(diag_registers);

	len += ((QLCNIC_DEV_INFO_SIZE + 2) * sizeof(u32));
	len += qlcnic_get_ring_regs_len(adapter);
	return len;
>>>>>>> refs/remotes/origin/master
}

static int qlcnic_get_eeprom_len(struct net_device *dev)
{
	return QLCNIC_FLASH_TOTAL_SIZE;
}

static void
qlcnic_get_drvinfo(struct net_device *dev, struct ethtool_drvinfo *drvinfo)
{
	struct qlcnic_adapter *adapter = netdev_priv(dev);
	u32 fw_major, fw_minor, fw_build;
<<<<<<< HEAD

	fw_major = QLCRD32(adapter, QLCNIC_FW_VERSION_MAJOR);
	fw_minor = QLCRD32(adapter, QLCNIC_FW_VERSION_MINOR);
	fw_build = QLCRD32(adapter, QLCNIC_FW_VERSION_SUB);
=======
	fw_major = QLC_SHARED_REG_RD32(adapter, QLCNIC_FW_VERSION_MAJOR);
	fw_minor = QLC_SHARED_REG_RD32(adapter, QLCNIC_FW_VERSION_MINOR);
	fw_build = QLC_SHARED_REG_RD32(adapter, QLCNIC_FW_VERSION_SUB);
>>>>>>> refs/remotes/origin/master
	snprintf(drvinfo->fw_version, sizeof(drvinfo->fw_version),
		"%d.%d.%d", fw_major, fw_minor, fw_build);

	strlcpy(drvinfo->bus_info, pci_name(adapter->pdev),
		sizeof(drvinfo->bus_info));
	strlcpy(drvinfo->driver, qlcnic_driver_name, sizeof(drvinfo->driver));
	strlcpy(drvinfo->version, QLCNIC_LINUX_VERSIONID,
		sizeof(drvinfo->version));
}

static int
qlcnic_get_settings(struct net_device *dev, struct ethtool_cmd *ecmd)
{
	struct qlcnic_adapter *adapter = netdev_priv(dev);
<<<<<<< HEAD
	int check_sfp_module = 0;
=======

	if (qlcnic_82xx_check(adapter))
		return qlcnic_82xx_get_settings(adapter, ecmd);
	else if (qlcnic_83xx_check(adapter))
		return qlcnic_83xx_get_settings(adapter, ecmd);

	return -EIO;
}

int qlcnic_82xx_get_settings(struct qlcnic_adapter *adapter,
			     struct ethtool_cmd *ecmd)
{
	struct qlcnic_hardware_context *ahw = adapter->ahw;
	u32 speed, reg;
	int check_sfp_module = 0, err = 0;
	u16 pcifn = ahw->pci_func;
>>>>>>> refs/remotes/origin/master

	/* read which mode */
	if (adapter->ahw->port_type == QLCNIC_GBE) {
		ecmd->supported = (SUPPORTED_10baseT_Half |
				   SUPPORTED_10baseT_Full |
				   SUPPORTED_100baseT_Half |
				   SUPPORTED_100baseT_Full |
				   SUPPORTED_1000baseT_Half |
				   SUPPORTED_1000baseT_Full);

		ecmd->advertising = (ADVERTISED_100baseT_Half |
				     ADVERTISED_100baseT_Full |
				     ADVERTISED_1000baseT_Half |
				     ADVERTISED_1000baseT_Full);

<<<<<<< HEAD
		ethtool_cmd_speed_set(ecmd, adapter->link_speed);
		ecmd->duplex = adapter->link_duplex;
		ecmd->autoneg = adapter->link_autoneg;

	} else if (adapter->ahw->port_type == QLCNIC_XGBE) {
		u32 val;

		val = QLCRD32(adapter, QLCNIC_PORT_MODE_ADDR);
=======
		ethtool_cmd_speed_set(ecmd, adapter->ahw->link_speed);
		ecmd->duplex = adapter->ahw->link_duplex;
		ecmd->autoneg = adapter->ahw->link_autoneg;

	} else if (adapter->ahw->port_type == QLCNIC_XGBE) {
		u32 val = 0;
		val = QLCRD32(adapter, QLCNIC_PORT_MODE_ADDR, &err);

>>>>>>> refs/remotes/origin/master
		if (val == QLCNIC_PORT_MODE_802_3_AP) {
			ecmd->supported = SUPPORTED_1000baseT_Full;
			ecmd->advertising = ADVERTISED_1000baseT_Full;
		} else {
			ecmd->supported = SUPPORTED_10000baseT_Full;
			ecmd->advertising = ADVERTISED_10000baseT_Full;
		}

<<<<<<< HEAD
		if (netif_running(dev) && adapter->has_link_events) {
			ethtool_cmd_speed_set(ecmd, adapter->link_speed);
			ecmd->autoneg = adapter->link_autoneg;
			ecmd->duplex = adapter->link_duplex;
=======
		if (netif_running(adapter->netdev) && ahw->has_link_events) {
			if (ahw->linkup) {
				reg = QLCRD32(adapter,
					      P3P_LINK_SPEED_REG(pcifn), &err);
				speed = P3P_LINK_SPEED_VAL(pcifn, reg);
				ahw->link_speed = speed * P3P_LINK_SPEED_MHZ;
			}

			ethtool_cmd_speed_set(ecmd, ahw->link_speed);
			ecmd->autoneg = ahw->link_autoneg;
			ecmd->duplex = ahw->link_duplex;
>>>>>>> refs/remotes/origin/master
			goto skip;
		}

		ethtool_cmd_speed_set(ecmd, SPEED_UNKNOWN);
		ecmd->duplex = DUPLEX_UNKNOWN;
		ecmd->autoneg = AUTONEG_DISABLE;
	} else
		return -EIO;

skip:
<<<<<<< HEAD
	ecmd->phy_address = adapter->physical_port;
=======
	ecmd->phy_address = adapter->ahw->physical_port;
>>>>>>> refs/remotes/origin/master
	ecmd->transceiver = XCVR_EXTERNAL;

	switch (adapter->ahw->board_type) {
	case QLCNIC_BRDTYPE_P3P_REF_QG:
	case QLCNIC_BRDTYPE_P3P_4_GB:
	case QLCNIC_BRDTYPE_P3P_4_GB_MM:

		ecmd->supported |= SUPPORTED_Autoneg;
		ecmd->advertising |= ADVERTISED_Autoneg;
	case QLCNIC_BRDTYPE_P3P_10G_CX4:
	case QLCNIC_BRDTYPE_P3P_10G_CX4_LP:
	case QLCNIC_BRDTYPE_P3P_10000_BASE_T:
		ecmd->supported |= SUPPORTED_TP;
		ecmd->advertising |= ADVERTISED_TP;
		ecmd->port = PORT_TP;
<<<<<<< HEAD
		ecmd->autoneg =  adapter->link_autoneg;
=======
		ecmd->autoneg =  adapter->ahw->link_autoneg;
>>>>>>> refs/remotes/origin/master
		break;
	case QLCNIC_BRDTYPE_P3P_IMEZ:
	case QLCNIC_BRDTYPE_P3P_XG_LOM:
	case QLCNIC_BRDTYPE_P3P_HMEZ:
		ecmd->supported |= SUPPORTED_MII;
		ecmd->advertising |= ADVERTISED_MII;
		ecmd->port = PORT_MII;
		ecmd->autoneg = AUTONEG_DISABLE;
		break;
	case QLCNIC_BRDTYPE_P3P_10G_SFP_PLUS:
	case QLCNIC_BRDTYPE_P3P_10G_SFP_CT:
	case QLCNIC_BRDTYPE_P3P_10G_SFP_QT:
		ecmd->advertising |= ADVERTISED_TP;
		ecmd->supported |= SUPPORTED_TP;
<<<<<<< HEAD
		check_sfp_module = netif_running(dev) &&
			adapter->has_link_events;
=======
		check_sfp_module = netif_running(adapter->netdev) &&
				   ahw->has_link_events;
>>>>>>> refs/remotes/origin/master
	case QLCNIC_BRDTYPE_P3P_10G_XFP:
		ecmd->supported |= SUPPORTED_FIBRE;
		ecmd->advertising |= ADVERTISED_FIBRE;
		ecmd->port = PORT_FIBRE;
		ecmd->autoneg = AUTONEG_DISABLE;
		break;
	case QLCNIC_BRDTYPE_P3P_10G_TP:
		if (adapter->ahw->port_type == QLCNIC_XGBE) {
			ecmd->autoneg = AUTONEG_DISABLE;
			ecmd->supported |= (SUPPORTED_FIBRE | SUPPORTED_TP);
			ecmd->advertising |=
				(ADVERTISED_FIBRE | ADVERTISED_TP);
			ecmd->port = PORT_FIBRE;
<<<<<<< HEAD
			check_sfp_module = netif_running(dev) &&
				adapter->has_link_events;
=======
			check_sfp_module = netif_running(adapter->netdev) &&
					   ahw->has_link_events;
>>>>>>> refs/remotes/origin/master
		} else {
			ecmd->autoneg = AUTONEG_ENABLE;
			ecmd->supported |= (SUPPORTED_TP | SUPPORTED_Autoneg);
			ecmd->advertising |=
				(ADVERTISED_TP | ADVERTISED_Autoneg);
			ecmd->port = PORT_TP;
		}
		break;
	default:
		dev_err(&adapter->pdev->dev, "Unsupported board model %d\n",
			adapter->ahw->board_type);
		return -EIO;
	}

	if (check_sfp_module) {
<<<<<<< HEAD
		switch (adapter->module_type) {
=======
		switch (adapter->ahw->module_type) {
>>>>>>> refs/remotes/origin/master
		case LINKEVENT_MODULE_OPTICAL_UNKNOWN:
		case LINKEVENT_MODULE_OPTICAL_SRLR:
		case LINKEVENT_MODULE_OPTICAL_LRM:
		case LINKEVENT_MODULE_OPTICAL_SFP_1G:
			ecmd->port = PORT_FIBRE;
			break;
		case LINKEVENT_MODULE_TWINAX_UNSUPPORTED_CABLE:
		case LINKEVENT_MODULE_TWINAX_UNSUPPORTED_CABLELEN:
		case LINKEVENT_MODULE_TWINAX:
			ecmd->port = PORT_TP;
			break;
		default:
			ecmd->port = PORT_OTHER;
		}
	}

	return 0;
}

<<<<<<< HEAD
static int
qlcnic_set_settings(struct net_device *dev, struct ethtool_cmd *ecmd)
{
	u32 config = 0;
	u32 ret = 0;
	struct qlcnic_adapter *adapter = netdev_priv(dev);

	if (adapter->ahw->port_type != QLCNIC_GBE)
		return -EOPNOTSUPP;

=======
static int qlcnic_set_port_config(struct qlcnic_adapter *adapter,
				  struct ethtool_cmd *ecmd)
{
	u32 ret = 0, config = 0;
>>>>>>> refs/remotes/origin/master
	/* read which mode */
	if (ecmd->duplex)
		config |= 0x1;

	if (ecmd->autoneg)
		config |= 0x2;

	switch (ethtool_cmd_speed(ecmd)) {
	case SPEED_10:
		config |= (0 << 8);
		break;
	case SPEED_100:
		config |= (1 << 8);
		break;
	case SPEED_1000:
		config |= (10 << 8);
		break;
	default:
		return -EIO;
	}

	ret = qlcnic_fw_cmd_set_port(adapter, config);

	if (ret == QLCNIC_RCODE_NOT_SUPPORTED)
		return -EOPNOTSUPP;
	else if (ret)
		return -EIO;
<<<<<<< HEAD

	adapter->link_speed = ethtool_cmd_speed(ecmd);
	adapter->link_duplex = ecmd->duplex;
	adapter->link_autoneg = ecmd->autoneg;
=======
	return ret;
}

static int qlcnic_set_settings(struct net_device *dev, struct ethtool_cmd *ecmd)
{
	u32 ret = 0;
	struct qlcnic_adapter *adapter = netdev_priv(dev);

	if (adapter->ahw->port_type != QLCNIC_GBE)
		return -EOPNOTSUPP;

	if (qlcnic_83xx_check(adapter))
		ret = qlcnic_83xx_set_settings(adapter, ecmd);
	else
		ret = qlcnic_set_port_config(adapter, ecmd);

	if (!ret)
		return ret;

	adapter->ahw->link_speed = ethtool_cmd_speed(ecmd);
	adapter->ahw->link_duplex = ecmd->duplex;
	adapter->ahw->link_autoneg = ecmd->autoneg;
>>>>>>> refs/remotes/origin/master

	if (!netif_running(dev))
		return 0;

	dev->netdev_ops->ndo_stop(dev);
	return dev->netdev_ops->ndo_open(dev);
}

<<<<<<< HEAD
=======
static int qlcnic_82xx_get_registers(struct qlcnic_adapter *adapter,
				     u32 *regs_buff)
{
	int i, j = 0, err = 0;

	for (i = QLCNIC_DEV_INFO_SIZE + 1; diag_registers[j] != -1; j++, i++)
		regs_buff[i] = QLC_SHARED_REG_RD32(adapter, diag_registers[j]);
	j = 0;
	while (ext_diag_registers[j] != -1)
		regs_buff[i++] = QLCRD32(adapter, ext_diag_registers[j++],
					 &err);
	return i;
}

>>>>>>> refs/remotes/origin/master
static void
qlcnic_get_regs(struct net_device *dev, struct ethtool_regs *regs, void *p)
{
	struct qlcnic_adapter *adapter = netdev_priv(dev);
	struct qlcnic_recv_context *recv_ctx = adapter->recv_ctx;
	struct qlcnic_host_sds_ring *sds_ring;
<<<<<<< HEAD
	u32 *regs_buff = p;
	int ring, i = 0, j = 0;

	memset(p, 0, qlcnic_get_regs_len(dev));
=======
	struct qlcnic_host_rds_ring *rds_rings;
	struct qlcnic_host_tx_ring *tx_ring;
	u32 *regs_buff = p;
	int ring, i = 0;

	memset(p, 0, qlcnic_get_regs_len(dev));

>>>>>>> refs/remotes/origin/master
	regs->version = (QLCNIC_ETHTOOL_REGS_VER << 24) |
		(adapter->ahw->revision_id << 16) | (adapter->pdev)->device;

	regs_buff[0] = (0xcafe0000 | (QLCNIC_DEV_INFO_SIZE & 0xffff));
	regs_buff[1] = QLCNIC_MGMT_API_VERSION;

<<<<<<< HEAD
	for (i = QLCNIC_DEV_INFO_SIZE + 1; diag_registers[j] != -1; j++, i++)
		regs_buff[i] = QLCRD32(adapter, diag_registers[j]);
=======
	if (qlcnic_82xx_check(adapter))
		i = qlcnic_82xx_get_registers(adapter, regs_buff);
	else
		i = qlcnic_83xx_get_registers(adapter, regs_buff);
>>>>>>> refs/remotes/origin/master

	if (!test_bit(__QLCNIC_DEV_UP, &adapter->state))
		return;

<<<<<<< HEAD
	regs_buff[i++] = 0xFFEFCDAB; /* Marker btw regs and ring count*/

	regs_buff[i++] = 1; /* No. of tx ring */
	regs_buff[i++] = le32_to_cpu(*(adapter->tx_ring->hw_consumer));
	regs_buff[i++] = readl(adapter->tx_ring->crb_cmd_producer);

	regs_buff[i++] = 2; /* No. of rx ring */
	regs_buff[i++] = readl(recv_ctx->rds_rings[0].crb_rcv_producer);
	regs_buff[i++] = readl(recv_ctx->rds_rings[1].crb_rcv_producer);

	regs_buff[i++] = adapter->max_sds_rings;

	for (ring = 0; ring < adapter->max_sds_rings; ring++) {
		sds_ring = &(recv_ctx->sds_rings[ring]);
		regs_buff[i++] = readl(sds_ring->crb_sts_consumer);
=======
	/* Marker btw regs and TX ring count */
	regs_buff[i++] = 0xFFEFCDAB;

	regs_buff[i++] = adapter->drv_tx_rings; /* No. of TX ring */
	for (ring = 0; ring < adapter->drv_tx_rings; ring++) {
		tx_ring = &adapter->tx_ring[ring];
		regs_buff[i++] = le32_to_cpu(*(tx_ring->hw_consumer));
		regs_buff[i++] = tx_ring->sw_consumer;
		regs_buff[i++] = readl(tx_ring->crb_cmd_producer);
		regs_buff[i++] = tx_ring->producer;
		if (tx_ring->crb_intr_mask)
			regs_buff[i++] = readl(tx_ring->crb_intr_mask);
		else
			regs_buff[i++] = QLCNIC_TX_INTR_NOT_CONFIGURED;
	}

	regs_buff[i++] = adapter->max_rds_rings; /* No. of RX ring */
	for (ring = 0; ring < adapter->max_rds_rings; ring++) {
		rds_rings = &recv_ctx->rds_rings[ring];
		regs_buff[i++] = readl(rds_rings->crb_rcv_producer);
		regs_buff[i++] = rds_rings->producer;
	}

	regs_buff[i++] = adapter->drv_sds_rings; /* No. of SDS ring */
	for (ring = 0; ring < adapter->drv_sds_rings; ring++) {
		sds_ring = &(recv_ctx->sds_rings[ring]);
		regs_buff[i++] = readl(sds_ring->crb_sts_consumer);
		regs_buff[i++] = sds_ring->consumer;
		regs_buff[i++] = readl(sds_ring->crb_intr_mask);
>>>>>>> refs/remotes/origin/master
	}
}

static u32 qlcnic_test_link(struct net_device *dev)
{
	struct qlcnic_adapter *adapter = netdev_priv(dev);
<<<<<<< HEAD
	u32 val;

	val = QLCRD32(adapter, CRB_XG_STATE_P3P);
=======
	int err = 0;
	u32 val;

	if (qlcnic_83xx_check(adapter)) {
		val = qlcnic_83xx_test_link(adapter);
		return (val & 1) ? 0 : 1;
	}
	val = QLCRD32(adapter, CRB_XG_STATE_P3P, &err);
	if (err == -EIO)
		return err;
>>>>>>> refs/remotes/origin/master
	val = XG_LINK_STATE_P3P(adapter->ahw->pci_func, val);
	return (val == XG_LINK_UP_P3P) ? 0 : 1;
}

static int
qlcnic_get_eeprom(struct net_device *dev, struct ethtool_eeprom *eeprom,
		      u8 *bytes)
{
	struct qlcnic_adapter *adapter = netdev_priv(dev);
	int offset;
<<<<<<< HEAD
	int ret;

=======
	int ret = -1;

	if (qlcnic_83xx_check(adapter))
		return 0;
>>>>>>> refs/remotes/origin/master
	if (eeprom->len == 0)
		return -EINVAL;

	eeprom->magic = (adapter->pdev)->vendor |
			((adapter->pdev)->device << 16);
	offset = eeprom->offset;

<<<<<<< HEAD
	ret = qlcnic_rom_fast_read_words(adapter, offset, bytes,
						eeprom->len);
=======
	if (qlcnic_82xx_check(adapter))
		ret = qlcnic_rom_fast_read_words(adapter, offset, bytes,
						 eeprom->len);
>>>>>>> refs/remotes/origin/master
	if (ret < 0)
		return ret;

	return 0;
}

static void
qlcnic_get_ringparam(struct net_device *dev,
		struct ethtool_ringparam *ring)
{
	struct qlcnic_adapter *adapter = netdev_priv(dev);

	ring->rx_pending = adapter->num_rxd;
	ring->rx_jumbo_pending = adapter->num_jumbo_rxd;
	ring->tx_pending = adapter->num_txd;

	ring->rx_max_pending = adapter->max_rxd;
	ring->rx_jumbo_max_pending = adapter->max_jumbo_rxd;
	ring->tx_max_pending = MAX_CMD_DESCRIPTORS;
}

static u32
qlcnic_validate_ringparam(u32 val, u32 min, u32 max, char *r_name)
{
	u32 num_desc;
	num_desc = max(val, min);
	num_desc = min(num_desc, max);
	num_desc = roundup_pow_of_two(num_desc);

	if (val != num_desc) {
		printk(KERN_INFO "%s: setting %s ring size %d instead of %d\n",
		       qlcnic_driver_name, r_name, num_desc, val);
	}

	return num_desc;
}

static int
qlcnic_set_ringparam(struct net_device *dev,
		struct ethtool_ringparam *ring)
{
	struct qlcnic_adapter *adapter = netdev_priv(dev);
	u16 num_rxd, num_jumbo_rxd, num_txd;

	if (ring->rx_mini_pending)
		return -EOPNOTSUPP;

	num_rxd = qlcnic_validate_ringparam(ring->rx_pending,
			MIN_RCV_DESCRIPTORS, adapter->max_rxd, "rx");

	num_jumbo_rxd = qlcnic_validate_ringparam(ring->rx_jumbo_pending,
			MIN_JUMBO_DESCRIPTORS, adapter->max_jumbo_rxd,
						"rx jumbo");

	num_txd = qlcnic_validate_ringparam(ring->tx_pending,
			MIN_CMD_DESCRIPTORS, MAX_CMD_DESCRIPTORS, "tx");

	if (num_rxd == adapter->num_rxd && num_txd == adapter->num_txd &&
			num_jumbo_rxd == adapter->num_jumbo_rxd)
		return 0;

	adapter->num_rxd = num_rxd;
	adapter->num_jumbo_rxd = num_jumbo_rxd;
	adapter->num_txd = num_txd;

	return qlcnic_reset_context(adapter);
}

<<<<<<< HEAD
=======
static int qlcnic_validate_ring_count(struct qlcnic_adapter *adapter,
				      u8 rx_ring, u8 tx_ring)
{
	if (rx_ring == 0 || tx_ring == 0)
		return -EINVAL;

	if (rx_ring != 0) {
		if (rx_ring > adapter->max_sds_rings) {
			netdev_err(adapter->netdev,
				   "Invalid ring count, SDS ring count %d should not be greater than max %d driver sds rings.\n",
				   rx_ring, adapter->max_sds_rings);
			return -EINVAL;
		}
	}

	 if (tx_ring != 0) {
		if (tx_ring > adapter->max_tx_rings) {
			netdev_err(adapter->netdev,
				   "Invalid ring count, Tx ring count %d should not be greater than max %d driver Tx rings.\n",
				   tx_ring, adapter->max_tx_rings);
			return -EINVAL;
		}
	}

	return 0;
}

>>>>>>> refs/remotes/origin/master
static void qlcnic_get_channels(struct net_device *dev,
		struct ethtool_channels *channel)
{
	struct qlcnic_adapter *adapter = netdev_priv(dev);

<<<<<<< HEAD
	channel->max_rx = rounddown_pow_of_two(min_t(int,
			adapter->max_rx_ques, num_online_cpus()));
	channel->max_tx = adapter->max_tx_ques;

	channel->rx_count = adapter->max_sds_rings;
	channel->tx_count = adapter->max_tx_ques;
}

static int qlcnic_set_channels(struct net_device *dev,
		struct ethtool_channels *channel)
=======
	channel->max_rx = adapter->max_sds_rings;
	channel->max_tx = adapter->max_tx_rings;
	channel->rx_count = adapter->drv_sds_rings;
	channel->tx_count = adapter->drv_tx_rings;
}

static int qlcnic_set_channels(struct net_device *dev,
			       struct ethtool_channels *channel)
>>>>>>> refs/remotes/origin/master
{
	struct qlcnic_adapter *adapter = netdev_priv(dev);
	int err;

<<<<<<< HEAD
	if (channel->other_count || channel->combined_count ||
	    channel->tx_count != channel->max_tx)
		return -EINVAL;

	err = qlcnic_validate_max_rss(dev, channel->max_rx, channel->rx_count);
	if (err)
		return err;

	err = qlcnic_set_max_rss(adapter, channel->rx_count);
	netdev_info(dev, "allocated 0x%x sds rings\n",
				 adapter->max_sds_rings);
=======
	if (channel->other_count || channel->combined_count)
		return -EINVAL;

	err = qlcnic_validate_ring_count(adapter, channel->rx_count,
					 channel->tx_count);
	if (err)
		return err;

	if (channel->rx_count) {
		err = qlcnic_validate_rings(adapter, channel->rx_count,
					    QLCNIC_RX_QUEUE);
		if (err) {
			netdev_err(dev, "Unable to configure %u SDS rings\n",
				   channel->rx_count);
			return err;
		}
	}

	if (channel->tx_count) {
		err = qlcnic_validate_rings(adapter, channel->tx_count,
					    QLCNIC_TX_QUEUE);
		if (err) {
			netdev_err(dev, "Unable to configure %u Tx rings\n",
				   channel->tx_count);
			return err;
		}
	}

	err = qlcnic_setup_rings(adapter, channel->rx_count,
				 channel->tx_count);
	netdev_info(dev, "Allocated %d SDS rings and %d Tx rings\n",
		    adapter->drv_sds_rings, adapter->drv_tx_rings);

>>>>>>> refs/remotes/origin/master
	return err;
}

static void
qlcnic_get_pauseparam(struct net_device *netdev,
			  struct ethtool_pauseparam *pause)
{
	struct qlcnic_adapter *adapter = netdev_priv(netdev);
<<<<<<< HEAD
	int port = adapter->physical_port;
	__u32 val;

=======
	int port = adapter->ahw->physical_port;
	int err = 0;
	__u32 val;

	if (qlcnic_83xx_check(adapter)) {
		qlcnic_83xx_get_pauseparam(adapter, pause);
		return;
	}
>>>>>>> refs/remotes/origin/master
	if (adapter->ahw->port_type == QLCNIC_GBE) {
		if ((port < 0) || (port > QLCNIC_NIU_MAX_GBE_PORTS))
			return;
		/* get flow control settings */
<<<<<<< HEAD
		val = QLCRD32(adapter, QLCNIC_NIU_GB_MAC_CONFIG_0(port));
		pause->rx_pause = qlcnic_gb_get_rx_flowctl(val);
		val = QLCRD32(adapter, QLCNIC_NIU_GB_PAUSE_CTL);
=======
		val = QLCRD32(adapter, QLCNIC_NIU_GB_MAC_CONFIG_0(port), &err);
		if (err == -EIO)
			return;
		pause->rx_pause = qlcnic_gb_get_rx_flowctl(val);
		val = QLCRD32(adapter, QLCNIC_NIU_GB_PAUSE_CTL, &err);
		if (err == -EIO)
			return;
>>>>>>> refs/remotes/origin/master
		switch (port) {
		case 0:
			pause->tx_pause = !(qlcnic_gb_get_gb0_mask(val));
			break;
		case 1:
			pause->tx_pause = !(qlcnic_gb_get_gb1_mask(val));
			break;
		case 2:
			pause->tx_pause = !(qlcnic_gb_get_gb2_mask(val));
			break;
		case 3:
		default:
			pause->tx_pause = !(qlcnic_gb_get_gb3_mask(val));
			break;
		}
	} else if (adapter->ahw->port_type == QLCNIC_XGBE) {
		if ((port < 0) || (port > QLCNIC_NIU_MAX_XG_PORTS))
			return;
		pause->rx_pause = 1;
<<<<<<< HEAD
		val = QLCRD32(adapter, QLCNIC_NIU_XG_PAUSE_CTL);
=======
		val = QLCRD32(adapter, QLCNIC_NIU_XG_PAUSE_CTL, &err);
		if (err == -EIO)
			return;
>>>>>>> refs/remotes/origin/master
		if (port == 0)
			pause->tx_pause = !(qlcnic_xg_get_xg0_mask(val));
		else
			pause->tx_pause = !(qlcnic_xg_get_xg1_mask(val));
	} else {
		dev_err(&netdev->dev, "Unknown board type: %x\n",
					adapter->ahw->port_type);
	}
}

static int
qlcnic_set_pauseparam(struct net_device *netdev,
			  struct ethtool_pauseparam *pause)
{
	struct qlcnic_adapter *adapter = netdev_priv(netdev);
<<<<<<< HEAD
	int port = adapter->physical_port;
	__u32 val;

=======
	int port = adapter->ahw->physical_port;
	int err = 0;
	__u32 val;

	if (qlcnic_83xx_check(adapter))
		return qlcnic_83xx_set_pauseparam(adapter, pause);

>>>>>>> refs/remotes/origin/master
	/* read mode */
	if (adapter->ahw->port_type == QLCNIC_GBE) {
		if ((port < 0) || (port > QLCNIC_NIU_MAX_GBE_PORTS))
			return -EIO;
		/* set flow control */
<<<<<<< HEAD
		val = QLCRD32(adapter, QLCNIC_NIU_GB_MAC_CONFIG_0(port));
=======
		val = QLCRD32(adapter, QLCNIC_NIU_GB_MAC_CONFIG_0(port), &err);
		if (err == -EIO)
			return err;
>>>>>>> refs/remotes/origin/master

		if (pause->rx_pause)
			qlcnic_gb_rx_flowctl(val);
		else
			qlcnic_gb_unset_rx_flowctl(val);

		QLCWR32(adapter, QLCNIC_NIU_GB_MAC_CONFIG_0(port),
				val);
<<<<<<< HEAD
		/* set autoneg */
		val = QLCRD32(adapter, QLCNIC_NIU_GB_PAUSE_CTL);
=======
		QLCWR32(adapter, QLCNIC_NIU_GB_MAC_CONFIG_0(port), val);
		/* set autoneg */
		val = QLCRD32(adapter, QLCNIC_NIU_GB_PAUSE_CTL, &err);
		if (err == -EIO)
			return err;
>>>>>>> refs/remotes/origin/master
		switch (port) {
		case 0:
			if (pause->tx_pause)
				qlcnic_gb_unset_gb0_mask(val);
			else
				qlcnic_gb_set_gb0_mask(val);
			break;
		case 1:
			if (pause->tx_pause)
				qlcnic_gb_unset_gb1_mask(val);
			else
				qlcnic_gb_set_gb1_mask(val);
			break;
		case 2:
			if (pause->tx_pause)
				qlcnic_gb_unset_gb2_mask(val);
			else
				qlcnic_gb_set_gb2_mask(val);
			break;
		case 3:
		default:
			if (pause->tx_pause)
				qlcnic_gb_unset_gb3_mask(val);
			else
				qlcnic_gb_set_gb3_mask(val);
			break;
		}
		QLCWR32(adapter, QLCNIC_NIU_GB_PAUSE_CTL, val);
	} else if (adapter->ahw->port_type == QLCNIC_XGBE) {
		if (!pause->rx_pause || pause->autoneg)
			return -EOPNOTSUPP;

		if ((port < 0) || (port > QLCNIC_NIU_MAX_XG_PORTS))
			return -EIO;

<<<<<<< HEAD
		val = QLCRD32(adapter, QLCNIC_NIU_XG_PAUSE_CTL);
=======
		val = QLCRD32(adapter, QLCNIC_NIU_XG_PAUSE_CTL, &err);
		if (err == -EIO)
			return err;
>>>>>>> refs/remotes/origin/master
		if (port == 0) {
			if (pause->tx_pause)
				qlcnic_xg_unset_xg0_mask(val);
			else
				qlcnic_xg_set_xg0_mask(val);
		} else {
			if (pause->tx_pause)
				qlcnic_xg_unset_xg1_mask(val);
			else
				qlcnic_xg_set_xg1_mask(val);
		}
		QLCWR32(adapter, QLCNIC_NIU_XG_PAUSE_CTL, val);
	} else {
		dev_err(&netdev->dev, "Unknown board type: %x\n",
				adapter->ahw->port_type);
	}
	return 0;
}

static int qlcnic_reg_test(struct net_device *dev)
{
	struct qlcnic_adapter *adapter = netdev_priv(dev);
	u32 data_read;
<<<<<<< HEAD

	data_read = QLCRD32(adapter, QLCNIC_PCIX_PH_REG(0));
=======
	int err = 0;

	if (qlcnic_83xx_check(adapter))
		return qlcnic_83xx_reg_test(adapter);

	data_read = QLCRD32(adapter, QLCNIC_PCIX_PH_REG(0), &err);
	if (err == -EIO)
		return err;
>>>>>>> refs/remotes/origin/master
	if ((data_read & 0xffff) != adapter->pdev->vendor)
		return 1;

	return 0;
}

<<<<<<< HEAD
static int qlcnic_get_sset_count(struct net_device *dev, int sset)
{
=======
static int qlcnic_eeprom_test(struct net_device *dev)
{
	struct qlcnic_adapter *adapter = netdev_priv(dev);

	if (qlcnic_82xx_check(adapter))
		return 0;

	return qlcnic_83xx_flash_test(adapter);
}

static int qlcnic_get_sset_count(struct net_device *dev, int sset)
{

>>>>>>> refs/remotes/origin/master
	struct qlcnic_adapter *adapter = netdev_priv(dev);
	switch (sset) {
	case ETH_SS_TEST:
		return QLCNIC_TEST_LEN;
	case ETH_SS_STATS:
<<<<<<< HEAD
		if (adapter->flags & QLCNIC_ESWITCH_ENABLED)
			return QLCNIC_STATS_LEN + QLCNIC_DEVICE_STATS_LEN;
		return QLCNIC_STATS_LEN;
=======
		return qlcnic_dev_statistics_len(adapter);
>>>>>>> refs/remotes/origin/master
	default:
		return -EOPNOTSUPP;
	}
}

static int qlcnic_irq_test(struct net_device *netdev)
{
	struct qlcnic_adapter *adapter = netdev_priv(netdev);
<<<<<<< HEAD
	int max_sds_rings = adapter->max_sds_rings;
	int ret;
	struct qlcnic_cmd_args cmd;
=======
	struct qlcnic_hardware_context *ahw = adapter->ahw;
	struct qlcnic_cmd_args cmd;
	int ret, drv_sds_rings = adapter->drv_sds_rings;
	int drv_tx_rings = adapter->drv_tx_rings;

	if (qlcnic_83xx_check(adapter))
		return qlcnic_83xx_interrupt_test(netdev);
>>>>>>> refs/remotes/origin/master

	if (test_and_set_bit(__QLCNIC_RESETTING, &adapter->state))
		return -EIO;

	ret = qlcnic_diag_alloc_res(netdev, QLCNIC_INTERRUPT_TEST);
	if (ret)
<<<<<<< HEAD
		goto clear_it;

	adapter->diag_cnt = 0;
	memset(&cmd, 0, sizeof(cmd));
	cmd.req.cmd = QLCNIC_CDRP_CMD_INTRPT_TEST;
	cmd.req.arg1 = adapter->ahw->pci_func;
	qlcnic_issue_cmd(adapter, &cmd);
	ret = cmd.rsp.cmd;

	if (ret)
		goto done;

	msleep(10);

	ret = !adapter->diag_cnt;

done:
	qlcnic_diag_free_res(netdev, max_sds_rings);

clear_it:
	adapter->max_sds_rings = max_sds_rings;
	clear_bit(__QLCNIC_RESETTING, &adapter->state);
	return ret;
}

#define QLCNIC_ILB_PKT_SIZE 64
#define QLCNIC_NUM_ILB_PKT	16
#define QLCNIC_ILB_MAX_RCV_LOOP 10
=======
		goto clear_diag_irq;

	ahw->diag_cnt = 0;
	ret = qlcnic_alloc_mbx_args(&cmd, adapter, QLCNIC_CMD_INTRPT_TEST);
	if (ret)
		goto free_diag_res;

	cmd.req.arg[1] = ahw->pci_func;
	ret = qlcnic_issue_cmd(adapter, &cmd);
	if (ret)
		goto done;

	usleep_range(1000, 12000);
	ret = !ahw->diag_cnt;

done:
	qlcnic_free_mbx_args(&cmd);

free_diag_res:
	qlcnic_diag_free_res(netdev, drv_sds_rings);

clear_diag_irq:
	adapter->drv_sds_rings = drv_sds_rings;
	adapter->drv_tx_rings = drv_tx_rings;
	clear_bit(__QLCNIC_RESETTING, &adapter->state);

	return ret;
}

#define QLCNIC_ILB_PKT_SIZE		64
#define QLCNIC_NUM_ILB_PKT		16
#define QLCNIC_ILB_MAX_RCV_LOOP		10
#define QLCNIC_LB_PKT_POLL_DELAY_MSEC	1
#define QLCNIC_LB_PKT_POLL_COUNT	20
>>>>>>> refs/remotes/origin/master

static void qlcnic_create_loopback_buff(unsigned char *data, u8 mac[])
{
	unsigned char random_data[] = {0xa8, 0x06, 0x45, 0x00};

	memset(data, 0x4e, QLCNIC_ILB_PKT_SIZE);

	memcpy(data, mac, ETH_ALEN);
	memcpy(data + ETH_ALEN, mac, ETH_ALEN);

	memcpy(data + 2 * ETH_ALEN, random_data, sizeof(random_data));
}

int qlcnic_check_loopback_buff(unsigned char *data, u8 mac[])
{
	unsigned char buff[QLCNIC_ILB_PKT_SIZE];
	qlcnic_create_loopback_buff(buff, mac);
	return memcmp(data, buff, QLCNIC_ILB_PKT_SIZE);
}

<<<<<<< HEAD
static int qlcnic_do_lb_test(struct qlcnic_adapter *adapter, u8 mode)
=======
int qlcnic_do_lb_test(struct qlcnic_adapter *adapter, u8 mode)
>>>>>>> refs/remotes/origin/master
{
	struct qlcnic_recv_context *recv_ctx = adapter->recv_ctx;
	struct qlcnic_host_sds_ring *sds_ring = &recv_ctx->sds_rings[0];
	struct sk_buff *skb;
	int i, loop, cnt = 0;

	for (i = 0; i < QLCNIC_NUM_ILB_PKT; i++) {
		skb = netdev_alloc_skb(adapter->netdev, QLCNIC_ILB_PKT_SIZE);
		qlcnic_create_loopback_buff(skb->data, adapter->mac_addr);
		skb_put(skb, QLCNIC_ILB_PKT_SIZE);
<<<<<<< HEAD

		adapter->diag_cnt = 0;
		qlcnic_xmit_frame(skb, adapter->netdev);

		loop = 0;
		do {
			msleep(1);
			qlcnic_process_rcv_ring_diag(sds_ring);
			if (loop++ > QLCNIC_ILB_MAX_RCV_LOOP)
				break;
		} while (!adapter->diag_cnt);

		dev_kfree_skb_any(skb);

		if (!adapter->diag_cnt)
			QLCDB(adapter, DRV,
			"LB Test: packet #%d was not received\n", i + 1);
=======
		adapter->ahw->diag_cnt = 0;
		qlcnic_xmit_frame(skb, adapter->netdev);
		loop = 0;

		do {
			msleep(QLCNIC_LB_PKT_POLL_DELAY_MSEC);
			qlcnic_process_rcv_ring_diag(sds_ring);
			if (loop++ > QLCNIC_LB_PKT_POLL_COUNT)
				break;
		} while (!adapter->ahw->diag_cnt);

		dev_kfree_skb_any(skb);

		if (!adapter->ahw->diag_cnt)
			dev_warn(&adapter->pdev->dev,
				 "LB Test: packet #%d was not received\n",
				 i + 1);
>>>>>>> refs/remotes/origin/master
		else
			cnt++;
	}
	if (cnt != i) {
<<<<<<< HEAD
		dev_warn(&adapter->pdev->dev, "LB Test failed\n");
		if (mode != QLCNIC_ILB_MODE) {
			dev_warn(&adapter->pdev->dev,
				"WARNING: Please make sure external"
				"loopback connector is plugged in\n");
		}
=======
		dev_err(&adapter->pdev->dev,
			"LB Test: failed, TX[%d], RX[%d]\n", i, cnt);
		if (mode != QLCNIC_ILB_MODE)
			dev_warn(&adapter->pdev->dev,
				 "WARNING: Please check loopback cable\n");
>>>>>>> refs/remotes/origin/master
		return -1;
	}
	return 0;
}

<<<<<<< HEAD
static int qlcnic_loopback_test(struct net_device *netdev, u8 mode)
{
	struct qlcnic_adapter *adapter = netdev_priv(netdev);
	int max_sds_rings = adapter->max_sds_rings;
	struct qlcnic_host_sds_ring *sds_ring;
	int loop = 0;
	int ret;

	if (!(adapter->capabilities & QLCNIC_FW_CAPABILITY_MULTI_LOOPBACK)) {
		netdev_info(netdev, "Firmware is not loopback test capable\n");
		return -EOPNOTSUPP;
	}

	QLCDB(adapter, DRV, "%s loopback test in progress\n",
		   mode == QLCNIC_ILB_MODE ? "internal" : "external");
	if (adapter->op_mode == QLCNIC_NON_PRIV_FUNC) {
		netdev_warn(netdev, "Loopback test not supported for non "
				"privilege function\n");
=======
int qlcnic_loopback_test(struct net_device *netdev, u8 mode)
{
	struct qlcnic_adapter *adapter = netdev_priv(netdev);
	int drv_tx_rings = adapter->drv_tx_rings;
	int drv_sds_rings = adapter->drv_sds_rings;
	struct qlcnic_host_sds_ring *sds_ring;
	struct qlcnic_hardware_context *ahw = adapter->ahw;
	int loop = 0;
	int ret;

	if (qlcnic_83xx_check(adapter))
		return qlcnic_83xx_loopback_test(netdev, mode);

	if (!(ahw->capabilities & QLCNIC_FW_CAPABILITY_MULTI_LOOPBACK)) {
		dev_info(&adapter->pdev->dev,
			 "Firmware do not support loopback test\n");
		return -EOPNOTSUPP;
	}

	dev_warn(&adapter->pdev->dev, "%s loopback test in progress\n",
		 mode == QLCNIC_ILB_MODE ? "internal" : "external");
	if (ahw->op_mode == QLCNIC_NON_PRIV_FUNC) {
		dev_warn(&adapter->pdev->dev,
			 "Loopback test not supported in nonprivileged mode\n");
>>>>>>> refs/remotes/origin/master
		return 0;
	}

	if (test_and_set_bit(__QLCNIC_RESETTING, &adapter->state))
		return -EBUSY;

	ret = qlcnic_diag_alloc_res(netdev, QLCNIC_LOOPBACK_TEST);
	if (ret)
		goto clear_it;

	sds_ring = &adapter->recv_ctx->sds_rings[0];
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
	ret = qlcnic_set_lb_mode(adapter, mode);
	if (ret)
		goto free_res;

<<<<<<< HEAD
	adapter->diag_cnt = 0;
=======
	ahw->diag_cnt = 0;
>>>>>>> refs/remotes/origin/master
	do {
		msleep(500);
		qlcnic_process_rcv_ring_diag(sds_ring);
		if (loop++ > QLCNIC_ILB_MAX_RCV_LOOP) {
<<<<<<< HEAD
			netdev_info(netdev, "firmware didnt respond to loopback"
				" configure request\n");
			ret = -QLCNIC_FW_NOT_RESPOND;
			goto free_res;
		} else if (adapter->diag_cnt) {
			ret = adapter->diag_cnt;
			goto free_res;
		}
	} while (!QLCNIC_IS_LB_CONFIGURED(adapter->ahw->loopback_state));

	ret = qlcnic_do_lb_test(adapter, mode);

	qlcnic_clear_lb_mode(adapter);

 free_res:
	qlcnic_diag_free_res(netdev, max_sds_rings);

 clear_it:
	adapter->max_sds_rings = max_sds_rings;
=======
			netdev_info(netdev,
				    "Firmware didn't sent link up event to loopback request\n");
			ret = -ETIMEDOUT;
			goto free_res;
		} else if (adapter->ahw->diag_cnt) {
			ret = adapter->ahw->diag_cnt;
			goto free_res;
		}
	} while (!QLCNIC_IS_LB_CONFIGURED(ahw->loopback_state));

	ret = qlcnic_do_lb_test(adapter, mode);

	qlcnic_clear_lb_mode(adapter, mode);

 free_res:
	qlcnic_diag_free_res(netdev, drv_sds_rings);

 clear_it:
	adapter->drv_sds_rings = drv_sds_rings;
	adapter->drv_tx_rings = drv_tx_rings;
>>>>>>> refs/remotes/origin/master
	clear_bit(__QLCNIC_RESETTING, &adapter->state);
	return ret;
}

static void
qlcnic_diag_test(struct net_device *dev, struct ethtool_test *eth_test,
		     u64 *data)
{
	memset(data, 0, sizeof(u64) * QLCNIC_TEST_LEN);

	data[0] = qlcnic_reg_test(dev);
	if (data[0])
		eth_test->flags |= ETH_TEST_FL_FAILED;

	data[1] = (u64) qlcnic_test_link(dev);
	if (data[1])
		eth_test->flags |= ETH_TEST_FL_FAILED;

	if (eth_test->flags & ETH_TEST_FL_OFFLINE) {
		data[2] = qlcnic_irq_test(dev);
		if (data[2])
			eth_test->flags |= ETH_TEST_FL_FAILED;

		data[3] = qlcnic_loopback_test(dev, QLCNIC_ILB_MODE);
		if (data[3])
			eth_test->flags |= ETH_TEST_FL_FAILED;
<<<<<<< HEAD
=======

>>>>>>> refs/remotes/origin/master
		if (eth_test->flags & ETH_TEST_FL_EXTERNAL_LB) {
			data[4] = qlcnic_loopback_test(dev, QLCNIC_ELB_MODE);
			if (data[4])
				eth_test->flags |= ETH_TEST_FL_FAILED;
			eth_test->flags |= ETH_TEST_FL_EXTERNAL_LB_DONE;
		}
<<<<<<< HEAD
=======

		data[5] = qlcnic_eeprom_test(dev);
		if (data[5])
			eth_test->flags |= ETH_TEST_FL_FAILED;
>>>>>>> refs/remotes/origin/master
	}
}

static void
<<<<<<< HEAD
qlcnic_get_strings(struct net_device *dev, u32 stringset, u8 * data)
{
	struct qlcnic_adapter *adapter = netdev_priv(dev);
	int index, i;
=======
qlcnic_get_strings(struct net_device *dev, u32 stringset, u8 *data)
{
	struct qlcnic_adapter *adapter = netdev_priv(dev);
	int index, i, num_stats;
>>>>>>> refs/remotes/origin/master

	switch (stringset) {
	case ETH_SS_TEST:
		memcpy(data, *qlcnic_gstrings_test,
		       QLCNIC_TEST_LEN * ETH_GSTRING_LEN);
		break;
	case ETH_SS_STATS:
<<<<<<< HEAD
=======
		num_stats = ARRAY_SIZE(qlcnic_tx_queue_stats_strings);
		for (i = 0; i < adapter->drv_tx_rings; i++) {
			for (index = 0; index < num_stats; index++) {
				sprintf(data, "tx_queue_%d %s", i,
					qlcnic_tx_queue_stats_strings[index]);
				data += ETH_GSTRING_LEN;
			}
		}

>>>>>>> refs/remotes/origin/master
		for (index = 0; index < QLCNIC_STATS_LEN; index++) {
			memcpy(data + index * ETH_GSTRING_LEN,
			       qlcnic_gstrings_stats[index].stat_string,
			       ETH_GSTRING_LEN);
		}
<<<<<<< HEAD
		if (!(adapter->flags & QLCNIC_ESWITCH_ENABLED))
			return;
		for (i = 0; i < QLCNIC_DEVICE_STATS_LEN; index++, i++) {
=======

		if (qlcnic_83xx_check(adapter)) {
			num_stats = ARRAY_SIZE(qlcnic_83xx_tx_stats_strings);
			for (i = 0; i < num_stats; i++, index++)
				memcpy(data + index * ETH_GSTRING_LEN,
				       qlcnic_83xx_tx_stats_strings[i],
				       ETH_GSTRING_LEN);
			num_stats = ARRAY_SIZE(qlcnic_83xx_mac_stats_strings);
			for (i = 0; i < num_stats; i++, index++)
				memcpy(data + index * ETH_GSTRING_LEN,
				       qlcnic_83xx_mac_stats_strings[i],
				       ETH_GSTRING_LEN);
			num_stats = ARRAY_SIZE(qlcnic_83xx_rx_stats_strings);
			for (i = 0; i < num_stats; i++, index++)
				memcpy(data + index * ETH_GSTRING_LEN,
				       qlcnic_83xx_rx_stats_strings[i],
				       ETH_GSTRING_LEN);
			return;
		} else {
			num_stats = ARRAY_SIZE(qlcnic_83xx_mac_stats_strings);
			for (i = 0; i < num_stats; i++, index++)
				memcpy(data + index * ETH_GSTRING_LEN,
				       qlcnic_83xx_mac_stats_strings[i],
				       ETH_GSTRING_LEN);
		}
		if (!(adapter->flags & QLCNIC_ESWITCH_ENABLED))
			return;
		num_stats = ARRAY_SIZE(qlcnic_device_gstrings_stats);
		for (i = 0; i < num_stats; index++, i++) {
>>>>>>> refs/remotes/origin/master
			memcpy(data + index * ETH_GSTRING_LEN,
			       qlcnic_device_gstrings_stats[i],
			       ETH_GSTRING_LEN);
		}
	}
}

<<<<<<< HEAD
#define QLCNIC_FILL_ESWITCH_STATS(VAL1) \
	(((VAL1) == QLCNIC_ESW_STATS_NOT_AVAIL) ? 0 : VAL1)

static void
qlcnic_fill_device_stats(int *index, u64 *data,
		struct __qlcnic_esw_statistics *stats)
{
	int ind = *index;

	data[ind++] = QLCNIC_FILL_ESWITCH_STATS(stats->unicast_frames);
	data[ind++] = QLCNIC_FILL_ESWITCH_STATS(stats->multicast_frames);
	data[ind++] = QLCNIC_FILL_ESWITCH_STATS(stats->broadcast_frames);
	data[ind++] = QLCNIC_FILL_ESWITCH_STATS(stats->dropped_frames);
	data[ind++] = QLCNIC_FILL_ESWITCH_STATS(stats->errors);
	data[ind++] = QLCNIC_FILL_ESWITCH_STATS(stats->local_frames);
	data[ind++] = QLCNIC_FILL_ESWITCH_STATS(stats->numbytes);

	*index = ind;
}

static void
qlcnic_get_ethtool_stats(struct net_device *dev,
			     struct ethtool_stats *stats, u64 * data)
{
	struct qlcnic_adapter *adapter = netdev_priv(dev);
	struct qlcnic_esw_statistics port_stats;
	int index, ret;

	for (index = 0; index < QLCNIC_STATS_LEN; index++) {
		char *p =
		    (char *)adapter +
		    qlcnic_gstrings_stats[index].stat_offset;
		data[index] =
		    (qlcnic_gstrings_stats[index].sizeof_stat ==
		     sizeof(u64)) ? *(u64 *)p:(*(u32 *)p);
=======
static u64 *qlcnic_fill_stats(u64 *data, void *stats, int type)
{
	if (type == QLCNIC_MAC_STATS) {
		struct qlcnic_mac_statistics *mac_stats =
					(struct qlcnic_mac_statistics *)stats;
		*data++ = QLCNIC_FILL_STATS(mac_stats->mac_tx_frames);
		*data++ = QLCNIC_FILL_STATS(mac_stats->mac_tx_bytes);
		*data++ = QLCNIC_FILL_STATS(mac_stats->mac_tx_mcast_pkts);
		*data++ = QLCNIC_FILL_STATS(mac_stats->mac_tx_bcast_pkts);
		*data++ = QLCNIC_FILL_STATS(mac_stats->mac_tx_pause_cnt);
		*data++ = QLCNIC_FILL_STATS(mac_stats->mac_tx_ctrl_pkt);
		*data++ = QLCNIC_FILL_STATS(mac_stats->mac_tx_lt_64b_pkts);
		*data++ = QLCNIC_FILL_STATS(mac_stats->mac_tx_lt_127b_pkts);
		*data++ = QLCNIC_FILL_STATS(mac_stats->mac_tx_lt_255b_pkts);
		*data++ = QLCNIC_FILL_STATS(mac_stats->mac_tx_lt_511b_pkts);
		*data++ = QLCNIC_FILL_STATS(mac_stats->mac_tx_lt_1023b_pkts);
		*data++ = QLCNIC_FILL_STATS(mac_stats->mac_tx_lt_1518b_pkts);
		*data++ = QLCNIC_FILL_STATS(mac_stats->mac_tx_gt_1518b_pkts);
		*data++ = QLCNIC_FILL_STATS(mac_stats->mac_rx_frames);
		*data++ = QLCNIC_FILL_STATS(mac_stats->mac_rx_bytes);
		*data++ = QLCNIC_FILL_STATS(mac_stats->mac_rx_mcast_pkts);
		*data++ = QLCNIC_FILL_STATS(mac_stats->mac_rx_bcast_pkts);
		*data++ = QLCNIC_FILL_STATS(mac_stats->mac_rx_pause_cnt);
		*data++ = QLCNIC_FILL_STATS(mac_stats->mac_rx_ctrl_pkt);
		*data++ = QLCNIC_FILL_STATS(mac_stats->mac_rx_lt_64b_pkts);
		*data++ = QLCNIC_FILL_STATS(mac_stats->mac_rx_lt_127b_pkts);
		*data++ = QLCNIC_FILL_STATS(mac_stats->mac_rx_lt_255b_pkts);
		*data++ = QLCNIC_FILL_STATS(mac_stats->mac_rx_lt_511b_pkts);
		*data++ = QLCNIC_FILL_STATS(mac_stats->mac_rx_lt_1023b_pkts);
		*data++ = QLCNIC_FILL_STATS(mac_stats->mac_rx_lt_1518b_pkts);
		*data++ = QLCNIC_FILL_STATS(mac_stats->mac_rx_gt_1518b_pkts);
		*data++ = QLCNIC_FILL_STATS(mac_stats->mac_rx_length_error);
		*data++ = QLCNIC_FILL_STATS(mac_stats->mac_rx_length_small);
		*data++ = QLCNIC_FILL_STATS(mac_stats->mac_rx_length_large);
		*data++ = QLCNIC_FILL_STATS(mac_stats->mac_rx_jabber);
		*data++ = QLCNIC_FILL_STATS(mac_stats->mac_rx_dropped);
		*data++ = QLCNIC_FILL_STATS(mac_stats->mac_rx_crc_error);
		*data++ = QLCNIC_FILL_STATS(mac_stats->mac_align_error);
	} else if (type == QLCNIC_ESW_STATS) {
		struct __qlcnic_esw_statistics *esw_stats =
				(struct __qlcnic_esw_statistics *)stats;
		*data++ = QLCNIC_FILL_STATS(esw_stats->unicast_frames);
		*data++ = QLCNIC_FILL_STATS(esw_stats->multicast_frames);
		*data++ = QLCNIC_FILL_STATS(esw_stats->broadcast_frames);
		*data++ = QLCNIC_FILL_STATS(esw_stats->dropped_frames);
		*data++ = QLCNIC_FILL_STATS(esw_stats->errors);
		*data++ = QLCNIC_FILL_STATS(esw_stats->local_frames);
		*data++ = QLCNIC_FILL_STATS(esw_stats->numbytes);
	}
	return data;
}

void qlcnic_update_stats(struct qlcnic_adapter *adapter)
{
	struct qlcnic_host_tx_ring *tx_ring;
	int ring;

	for (ring = 0; ring < adapter->drv_tx_rings; ring++) {
		tx_ring = &adapter->tx_ring[ring];
		adapter->stats.xmit_on += tx_ring->tx_stats.xmit_on;
		adapter->stats.xmit_off += tx_ring->tx_stats.xmit_off;
		adapter->stats.xmitcalled += tx_ring->tx_stats.xmit_called;
		adapter->stats.xmitfinished += tx_ring->tx_stats.xmit_finished;
		adapter->stats.txbytes += tx_ring->tx_stats.tx_bytes;
	}
}

static u64 *qlcnic_fill_tx_queue_stats(u64 *data, void *stats)
{
	struct qlcnic_host_tx_ring *tx_ring;

	tx_ring = (struct qlcnic_host_tx_ring *)stats;

	*data++ = QLCNIC_FILL_STATS(tx_ring->tx_stats.xmit_on);
	*data++ = QLCNIC_FILL_STATS(tx_ring->tx_stats.xmit_off);
	*data++ = QLCNIC_FILL_STATS(tx_ring->tx_stats.xmit_called);
	*data++ = QLCNIC_FILL_STATS(tx_ring->tx_stats.xmit_finished);
	*data++ = QLCNIC_FILL_STATS(tx_ring->tx_stats.tx_bytes);

	return data;
}

static void qlcnic_get_ethtool_stats(struct net_device *dev,
				     struct ethtool_stats *stats, u64 *data)
{
	struct qlcnic_adapter *adapter = netdev_priv(dev);
	struct qlcnic_host_tx_ring *tx_ring;
	struct qlcnic_esw_statistics port_stats;
	struct qlcnic_mac_statistics mac_stats;
	int index, ret, length, size, tx_size, ring;
	char *p;

	tx_size = adapter->drv_tx_rings * QLCNIC_TX_STATS_LEN;

	memset(data, 0, tx_size * sizeof(u64));
	for (ring = 0, index = 0; ring < adapter->drv_tx_rings; ring++) {
		if (test_bit(__QLCNIC_DEV_UP, &adapter->state)) {
			tx_ring = &adapter->tx_ring[ring];
			data = qlcnic_fill_tx_queue_stats(data, tx_ring);
			qlcnic_update_stats(adapter);
		}
	}

	memset(data, 0, stats->n_stats * sizeof(u64));
	length = QLCNIC_STATS_LEN;
	for (index = 0; index < length; index++) {
		p = (char *)adapter + qlcnic_gstrings_stats[index].stat_offset;
		size = qlcnic_gstrings_stats[index].sizeof_stat;
		*data++ = (size == sizeof(u64)) ? (*(u64 *)p) : ((*(u32 *)p));
	}

	if (qlcnic_83xx_check(adapter)) {
		if (adapter->ahw->linkup)
			qlcnic_83xx_get_stats(adapter, data);
		return;
	} else {
		/* Retrieve MAC statistics from firmware */
		memset(&mac_stats, 0, sizeof(struct qlcnic_mac_statistics));
		qlcnic_get_mac_stats(adapter, &mac_stats);
		data = qlcnic_fill_stats(data, &mac_stats, QLCNIC_MAC_STATS);
>>>>>>> refs/remotes/origin/master
	}

	if (!(adapter->flags & QLCNIC_ESWITCH_ENABLED))
		return;

	memset(&port_stats, 0, sizeof(struct qlcnic_esw_statistics));
	ret = qlcnic_get_port_stats(adapter, adapter->ahw->pci_func,
			QLCNIC_QUERY_RX_COUNTER, &port_stats.rx);
	if (ret)
		return;

<<<<<<< HEAD
	qlcnic_fill_device_stats(&index, data, &port_stats.rx);

=======
	data = qlcnic_fill_stats(data, &port_stats.rx, QLCNIC_ESW_STATS);
>>>>>>> refs/remotes/origin/master
	ret = qlcnic_get_port_stats(adapter, adapter->ahw->pci_func,
			QLCNIC_QUERY_TX_COUNTER, &port_stats.tx);
	if (ret)
		return;

<<<<<<< HEAD
	qlcnic_fill_device_stats(&index, data, &port_stats.tx);
=======
	qlcnic_fill_stats(data, &port_stats.tx, QLCNIC_ESW_STATS);
>>>>>>> refs/remotes/origin/master
}

static int qlcnic_set_led(struct net_device *dev,
			  enum ethtool_phys_id_state state)
{
	struct qlcnic_adapter *adapter = netdev_priv(dev);
<<<<<<< HEAD
	int max_sds_rings = adapter->max_sds_rings;
	int err = -EIO, active = 1;

	if (adapter->op_mode == QLCNIC_NON_PRIV_FUNC) {
=======
	int drv_sds_rings = adapter->drv_sds_rings;
	int err = -EIO, active = 1;

	if (qlcnic_83xx_check(adapter))
		return qlcnic_83xx_set_led(dev, state);

	if (adapter->ahw->op_mode == QLCNIC_NON_PRIV_FUNC) {
>>>>>>> refs/remotes/origin/master
		netdev_warn(dev, "LED test not supported for non "
				"privilege function\n");
		return -EOPNOTSUPP;
	}

	switch (state) {
	case ETHTOOL_ID_ACTIVE:
		if (test_and_set_bit(__QLCNIC_LED_ENABLE, &adapter->state))
			return -EBUSY;

		if (test_bit(__QLCNIC_RESETTING, &adapter->state))
			break;

		if (!test_bit(__QLCNIC_DEV_UP, &adapter->state)) {
			if (qlcnic_diag_alloc_res(dev, QLCNIC_LED_TEST))
				break;
			set_bit(__QLCNIC_DIAG_RES_ALLOC, &adapter->state);
		}

		if (adapter->nic_ops->config_led(adapter, 1, 0xf) == 0) {
			err = 0;
			break;
		}

		dev_err(&adapter->pdev->dev,
			"Failed to set LED blink state.\n");
		break;

	case ETHTOOL_ID_INACTIVE:
		active = 0;

		if (test_bit(__QLCNIC_RESETTING, &adapter->state))
			break;

		if (!test_bit(__QLCNIC_DEV_UP, &adapter->state)) {
			if (qlcnic_diag_alloc_res(dev, QLCNIC_LED_TEST))
				break;
			set_bit(__QLCNIC_DIAG_RES_ALLOC, &adapter->state);
		}

		if (adapter->nic_ops->config_led(adapter, 0, 0xf))
			dev_err(&adapter->pdev->dev,
				"Failed to reset LED blink state.\n");

		break;

	default:
		return -EINVAL;
	}

	if (test_and_clear_bit(__QLCNIC_DIAG_RES_ALLOC, &adapter->state))
<<<<<<< HEAD
		qlcnic_diag_free_res(dev, max_sds_rings);
=======
		qlcnic_diag_free_res(dev, drv_sds_rings);
>>>>>>> refs/remotes/origin/master

	if (!active || err)
		clear_bit(__QLCNIC_LED_ENABLE, &adapter->state);

	return err;
}

static void
qlcnic_get_wol(struct net_device *dev, struct ethtool_wolinfo *wol)
{
	struct qlcnic_adapter *adapter = netdev_priv(dev);
	u32 wol_cfg;
<<<<<<< HEAD

	wol->supported = 0;
	wol->wolopts = 0;

	wol_cfg = QLCRD32(adapter, QLCNIC_WOL_CONFIG_NV);
	if (wol_cfg & (1UL << adapter->portnum))
		wol->supported |= WAKE_MAGIC;

	wol_cfg = QLCRD32(adapter, QLCNIC_WOL_CONFIG);
=======
	int err = 0;

	if (qlcnic_83xx_check(adapter))
		return;
	wol->supported = 0;
	wol->wolopts = 0;

	wol_cfg = QLCRD32(adapter, QLCNIC_WOL_CONFIG_NV, &err);
	if (err == -EIO)
		return;
	if (wol_cfg & (1UL << adapter->portnum))
		wol->supported |= WAKE_MAGIC;

	wol_cfg = QLCRD32(adapter, QLCNIC_WOL_CONFIG, &err);
>>>>>>> refs/remotes/origin/master
	if (wol_cfg & (1UL << adapter->portnum))
		wol->wolopts |= WAKE_MAGIC;
}

static int
qlcnic_set_wol(struct net_device *dev, struct ethtool_wolinfo *wol)
{
	struct qlcnic_adapter *adapter = netdev_priv(dev);
	u32 wol_cfg;
<<<<<<< HEAD

	if (wol->wolopts & ~WAKE_MAGIC)
		return -EOPNOTSUPP;

	wol_cfg = QLCRD32(adapter, QLCNIC_WOL_CONFIG_NV);
	if (!(wol_cfg & (1 << adapter->portnum)))
		return -EOPNOTSUPP;

	wol_cfg = QLCRD32(adapter, QLCNIC_WOL_CONFIG);
=======
	int err = 0;

	if (qlcnic_83xx_check(adapter))
		return -EOPNOTSUPP;
	if (wol->wolopts & ~WAKE_MAGIC)
		return -EINVAL;

	wol_cfg = QLCRD32(adapter, QLCNIC_WOL_CONFIG_NV, &err);
	if (err == -EIO)
		return err;
	if (!(wol_cfg & (1 << adapter->portnum)))
		return -EOPNOTSUPP;

	wol_cfg = QLCRD32(adapter, QLCNIC_WOL_CONFIG, &err);
	if (err == -EIO)
		return err;
>>>>>>> refs/remotes/origin/master
	if (wol->wolopts & WAKE_MAGIC)
		wol_cfg |= 1UL << adapter->portnum;
	else
		wol_cfg &= ~(1UL << adapter->portnum);

	QLCWR32(adapter, QLCNIC_WOL_CONFIG, wol_cfg);

	return 0;
}

/*
 * Set the coalescing parameters. Currently only normal is supported.
 * If rx_coalesce_usecs == 0 or rx_max_coalesced_frames == 0 then set the
 * firmware coalescing to default.
 */
static int qlcnic_set_intr_coalesce(struct net_device *netdev,
			struct ethtool_coalesce *ethcoal)
{
	struct qlcnic_adapter *adapter = netdev_priv(netdev);
<<<<<<< HEAD
=======
	struct qlcnic_nic_intr_coalesce *coal;
	u32 rx_coalesce_usecs, rx_max_frames;
	u32 tx_coalesce_usecs, tx_max_frames;
>>>>>>> refs/remotes/origin/master

	if (!test_bit(__QLCNIC_DEV_UP, &adapter->state))
		return -EINVAL;

	/*
	* Return Error if unsupported values or
	* unsupported parameters are set.
	*/
	if (ethcoal->rx_coalesce_usecs > 0xffff ||
		ethcoal->rx_max_coalesced_frames > 0xffff ||
<<<<<<< HEAD
		ethcoal->tx_coalesce_usecs ||
		ethcoal->tx_max_coalesced_frames ||
=======
		ethcoal->tx_coalesce_usecs > 0xffff ||
		ethcoal->tx_max_coalesced_frames > 0xffff ||
>>>>>>> refs/remotes/origin/master
		ethcoal->rx_coalesce_usecs_irq ||
		ethcoal->rx_max_coalesced_frames_irq ||
		ethcoal->tx_coalesce_usecs_irq ||
		ethcoal->tx_max_coalesced_frames_irq ||
		ethcoal->stats_block_coalesce_usecs ||
		ethcoal->use_adaptive_rx_coalesce ||
		ethcoal->use_adaptive_tx_coalesce ||
		ethcoal->pkt_rate_low ||
		ethcoal->rx_coalesce_usecs_low ||
		ethcoal->rx_max_coalesced_frames_low ||
		ethcoal->tx_coalesce_usecs_low ||
		ethcoal->tx_max_coalesced_frames_low ||
		ethcoal->pkt_rate_high ||
		ethcoal->rx_coalesce_usecs_high ||
		ethcoal->rx_max_coalesced_frames_high ||
		ethcoal->tx_coalesce_usecs_high ||
		ethcoal->tx_max_coalesced_frames_high)
		return -EINVAL;

<<<<<<< HEAD
	if (!ethcoal->rx_coalesce_usecs ||
		!ethcoal->rx_max_coalesced_frames) {
		adapter->ahw->coal.flag = QLCNIC_INTR_DEFAULT;
		adapter->ahw->coal.rx_time_us =
			QLCNIC_DEFAULT_INTR_COALESCE_RX_TIME_US;
		adapter->ahw->coal.rx_packets =
			QLCNIC_DEFAULT_INTR_COALESCE_RX_PACKETS;
	} else {
		adapter->ahw->coal.flag = 0;
		adapter->ahw->coal.rx_time_us = ethcoal->rx_coalesce_usecs;
		adapter->ahw->coal.rx_packets =
			ethcoal->rx_max_coalesced_frames;
=======
	coal = &adapter->ahw->coal;

	if (qlcnic_83xx_check(adapter)) {
		if (!ethcoal->tx_coalesce_usecs ||
		    !ethcoal->tx_max_coalesced_frames ||
		    !ethcoal->rx_coalesce_usecs ||
		    !ethcoal->rx_max_coalesced_frames) {
			coal->flag = QLCNIC_INTR_DEFAULT;
			coal->type = QLCNIC_INTR_COAL_TYPE_RX;
			coal->rx_time_us = QLCNIC_DEF_INTR_COALESCE_RX_TIME_US;
			coal->rx_packets = QLCNIC_DEF_INTR_COALESCE_RX_PACKETS;
			coal->tx_time_us = QLCNIC_DEF_INTR_COALESCE_TX_TIME_US;
			coal->tx_packets = QLCNIC_DEF_INTR_COALESCE_TX_PACKETS;
		} else {
			tx_coalesce_usecs = ethcoal->tx_coalesce_usecs;
			tx_max_frames = ethcoal->tx_max_coalesced_frames;
			rx_coalesce_usecs = ethcoal->rx_coalesce_usecs;
			rx_max_frames = ethcoal->rx_max_coalesced_frames;
			coal->flag = 0;

			if ((coal->rx_time_us == rx_coalesce_usecs) &&
			    (coal->rx_packets == rx_max_frames)) {
				coal->type = QLCNIC_INTR_COAL_TYPE_TX;
				coal->tx_time_us = tx_coalesce_usecs;
				coal->tx_packets = tx_max_frames;
			} else if ((coal->tx_time_us == tx_coalesce_usecs) &&
				   (coal->tx_packets == tx_max_frames)) {
				coal->type = QLCNIC_INTR_COAL_TYPE_RX;
				coal->rx_time_us = rx_coalesce_usecs;
				coal->rx_packets = rx_max_frames;
			} else {
				coal->type = QLCNIC_INTR_COAL_TYPE_RX;
				coal->rx_time_us = rx_coalesce_usecs;
				coal->rx_packets = rx_max_frames;
				coal->tx_time_us = tx_coalesce_usecs;
				coal->tx_packets = tx_max_frames;
			}
		}
	} else {
		if (!ethcoal->rx_coalesce_usecs ||
		    !ethcoal->rx_max_coalesced_frames) {
			coal->flag = QLCNIC_INTR_DEFAULT;
			coal->rx_time_us = QLCNIC_DEF_INTR_COALESCE_RX_TIME_US;
			coal->rx_packets = QLCNIC_DEF_INTR_COALESCE_RX_PACKETS;
		} else {
			coal->flag = 0;
			coal->rx_time_us = ethcoal->rx_coalesce_usecs;
			coal->rx_packets = ethcoal->rx_max_coalesced_frames;
		}
>>>>>>> refs/remotes/origin/master
	}

	qlcnic_config_intr_coalesce(adapter);

	return 0;
}

static int qlcnic_get_intr_coalesce(struct net_device *netdev,
			struct ethtool_coalesce *ethcoal)
{
	struct qlcnic_adapter *adapter = netdev_priv(netdev);

	if (adapter->is_up != QLCNIC_ADAPTER_UP_MAGIC)
		return -EINVAL;

	ethcoal->rx_coalesce_usecs = adapter->ahw->coal.rx_time_us;
	ethcoal->rx_max_coalesced_frames = adapter->ahw->coal.rx_packets;
<<<<<<< HEAD
=======
	ethcoal->tx_coalesce_usecs = adapter->ahw->coal.tx_time_us;
	ethcoal->tx_max_coalesced_frames = adapter->ahw->coal.tx_packets;
>>>>>>> refs/remotes/origin/master

	return 0;
}

static u32 qlcnic_get_msglevel(struct net_device *netdev)
{
	struct qlcnic_adapter *adapter = netdev_priv(netdev);

<<<<<<< HEAD
	return adapter->msg_enable;
=======
	return adapter->ahw->msg_enable;
>>>>>>> refs/remotes/origin/master
}

static void qlcnic_set_msglevel(struct net_device *netdev, u32 msglvl)
{
	struct qlcnic_adapter *adapter = netdev_priv(netdev);

<<<<<<< HEAD
	adapter->msg_enable = msglvl;
=======
	adapter->ahw->msg_enable = msglvl;
}

int qlcnic_enable_fw_dump_state(struct qlcnic_adapter *adapter)
{
	struct qlcnic_fw_dump *fw_dump = &adapter->ahw->fw_dump;
	u32 val;

	if (qlcnic_84xx_check(adapter)) {
		if (qlcnic_83xx_lock_driver(adapter))
			return -EBUSY;

		val = QLCRDX(adapter->ahw, QLC_83XX_IDC_CTRL);
		val &= ~QLC_83XX_IDC_DISABLE_FW_DUMP;
		QLCWRX(adapter->ahw, QLC_83XX_IDC_CTRL, val);

		qlcnic_83xx_unlock_driver(adapter);
	} else {
		fw_dump->enable = true;
	}

	dev_info(&adapter->pdev->dev, "FW dump enabled\n");

	return 0;
}

static int qlcnic_disable_fw_dump_state(struct qlcnic_adapter *adapter)
{
	struct qlcnic_fw_dump *fw_dump = &adapter->ahw->fw_dump;
	u32 val;

	if (qlcnic_84xx_check(adapter)) {
		if (qlcnic_83xx_lock_driver(adapter))
			return -EBUSY;

		val = QLCRDX(adapter->ahw, QLC_83XX_IDC_CTRL);
		val |= QLC_83XX_IDC_DISABLE_FW_DUMP;
		QLCWRX(adapter->ahw, QLC_83XX_IDC_CTRL, val);

		qlcnic_83xx_unlock_driver(adapter);
	} else {
		fw_dump->enable = false;
	}

	dev_info(&adapter->pdev->dev, "FW dump disabled\n");

	return 0;
}

bool qlcnic_check_fw_dump_state(struct qlcnic_adapter *adapter)
{
	struct qlcnic_fw_dump *fw_dump = &adapter->ahw->fw_dump;
	bool state;
	u32 val;

	if (qlcnic_84xx_check(adapter)) {
		val = QLCRDX(adapter->ahw, QLC_83XX_IDC_CTRL);
		state = (val & QLC_83XX_IDC_DISABLE_FW_DUMP) ? false : true;
	} else {
		state = fw_dump->enable;
	}

	return state;
>>>>>>> refs/remotes/origin/master
}

static int
qlcnic_get_dump_flag(struct net_device *netdev, struct ethtool_dump *dump)
{
	struct qlcnic_adapter *adapter = netdev_priv(netdev);
	struct qlcnic_fw_dump *fw_dump = &adapter->ahw->fw_dump;

<<<<<<< HEAD
=======
	if (!fw_dump->tmpl_hdr) {
		netdev_err(adapter->netdev, "FW Dump not supported\n");
		return -ENOTSUPP;
	}

>>>>>>> refs/remotes/origin/master
	if (fw_dump->clr)
		dump->len = fw_dump->tmpl_hdr->size + fw_dump->size;
	else
		dump->len = 0;
<<<<<<< HEAD
	dump->flag = fw_dump->tmpl_hdr->drv_cap_mask;
=======

	if (!qlcnic_check_fw_dump_state(adapter))
		dump->flag = ETH_FW_DUMP_DISABLE;
	else
		dump->flag = fw_dump->tmpl_hdr->drv_cap_mask;

>>>>>>> refs/remotes/origin/master
	dump->version = adapter->fw_version;
	return 0;
}

static int
qlcnic_get_dump_data(struct net_device *netdev, struct ethtool_dump *dump,
			void *buffer)
{
	int i, copy_sz;
<<<<<<< HEAD
	u32 *hdr_ptr, *data;
	struct qlcnic_adapter *adapter = netdev_priv(netdev);
	struct qlcnic_fw_dump *fw_dump = &adapter->ahw->fw_dump;

=======
	u32 *hdr_ptr;
	__le32 *data;
	struct qlcnic_adapter *adapter = netdev_priv(netdev);
	struct qlcnic_fw_dump *fw_dump = &adapter->ahw->fw_dump;

	if (!fw_dump->tmpl_hdr) {
		netdev_err(netdev, "FW Dump not supported\n");
		return -ENOTSUPP;
	}

>>>>>>> refs/remotes/origin/master
	if (!fw_dump->clr) {
		netdev_info(netdev, "Dump not available\n");
		return -EINVAL;
	}
	/* Copy template header first */
	copy_sz = fw_dump->tmpl_hdr->size;
	hdr_ptr = (u32 *) fw_dump->tmpl_hdr;
	data = buffer;
	for (i = 0; i < copy_sz/sizeof(u32); i++)
		*data++ = cpu_to_le32(*hdr_ptr++);

	/* Copy captured dump data */
	memcpy(buffer + copy_sz, fw_dump->data, fw_dump->size);
	dump->len = copy_sz + fw_dump->size;
	dump->flag = fw_dump->tmpl_hdr->drv_cap_mask;

	/* Free dump area once data has been captured */
	vfree(fw_dump->data);
	fw_dump->data = NULL;
	fw_dump->clr = 0;
	netdev_info(netdev, "extracted the FW dump Successfully\n");
	return 0;
}

<<<<<<< HEAD
static int
qlcnic_set_dump(struct net_device *netdev, struct ethtool_dump *val)
{
	int ret = 0;
	struct qlcnic_adapter *adapter = netdev_priv(netdev);
	struct qlcnic_fw_dump *fw_dump = &adapter->ahw->fw_dump;

	switch (val->flag) {
	case QLCNIC_FORCE_FW_DUMP_KEY:
		if (!fw_dump->enable) {
			netdev_info(netdev, "FW dump not enabled\n");
			return ret;
		}
		if (fw_dump->clr) {
			netdev_info(netdev,
			"Previous dump not cleared, not forcing dump\n");
			return ret;
		}
		netdev_info(netdev, "Forcing a FW dump\n");
		qlcnic_dev_request_reset(adapter);
		break;
	case QLCNIC_DISABLE_FW_DUMP:
		if (fw_dump->enable) {
			netdev_info(netdev, "Disabling FW dump\n");
			fw_dump->enable = 0;
		}
		break;
	case QLCNIC_ENABLE_FW_DUMP:
		if (!fw_dump->enable && fw_dump->tmpl_hdr) {
			netdev_info(netdev, "Enabling FW dump\n");
			fw_dump->enable = 1;
		}
		break;
	case QLCNIC_FORCE_FW_RESET:
		netdev_info(netdev, "Forcing a FW reset\n");
		qlcnic_dev_request_reset(adapter);
		adapter->flags &= ~QLCNIC_FW_RESET_OWNER;
		break;
	default:
		if (val->flag > QLCNIC_DUMP_MASK_MAX ||
			val->flag < QLCNIC_DUMP_MASK_MIN) {
				netdev_info(netdev,
				"Invalid dump level: 0x%x\n", val->flag);
				ret = -EINVAL;
				goto out;
		}
		fw_dump->tmpl_hdr->drv_cap_mask = val->flag & 0xff;
		netdev_info(netdev, "Driver mask changed to: 0x%x\n",
			fw_dump->tmpl_hdr->drv_cap_mask);
	}
out:
=======
static int qlcnic_set_dump_mask(struct qlcnic_adapter *adapter, u32 mask)
{
	struct qlcnic_fw_dump *fw_dump = &adapter->ahw->fw_dump;
	struct net_device *netdev = adapter->netdev;

	if (!qlcnic_check_fw_dump_state(adapter)) {
		netdev_info(netdev,
			    "Can not change driver mask to 0x%x. FW dump not enabled\n",
			    mask);
		return -EOPNOTSUPP;
	}

	fw_dump->tmpl_hdr->drv_cap_mask = mask;
	netdev_info(netdev, "Driver mask changed to: 0x%x\n", mask);
	return 0;
}

static int
qlcnic_set_dump(struct net_device *netdev, struct ethtool_dump *val)
{
	struct qlcnic_adapter *adapter = netdev_priv(netdev);
	struct qlcnic_fw_dump *fw_dump = &adapter->ahw->fw_dump;
	bool valid_mask = false;
	int i, ret = 0;

	switch (val->flag) {
	case QLCNIC_FORCE_FW_DUMP_KEY:
		if (!fw_dump->tmpl_hdr) {
			netdev_err(netdev, "FW dump not supported\n");
			ret = -EOPNOTSUPP;
			break;
		}

		if (!qlcnic_check_fw_dump_state(adapter)) {
			netdev_info(netdev, "FW dump not enabled\n");
			ret = -EOPNOTSUPP;
			break;
		}

		if (fw_dump->clr) {
			netdev_info(netdev,
				    "Previous dump not cleared, not forcing dump\n");
			break;
		}

		netdev_info(netdev, "Forcing a FW dump\n");
		qlcnic_dev_request_reset(adapter, val->flag);
		break;
	case QLCNIC_DISABLE_FW_DUMP:
		if (!fw_dump->tmpl_hdr) {
			netdev_err(netdev, "FW dump not supported\n");
			ret = -EOPNOTSUPP;
			break;
		}

		ret = qlcnic_disable_fw_dump_state(adapter);
		break;

	case QLCNIC_ENABLE_FW_DUMP:
		if (!fw_dump->tmpl_hdr) {
			netdev_err(netdev, "FW dump not supported\n");
			ret = -EOPNOTSUPP;
			break;
		}

		ret = qlcnic_enable_fw_dump_state(adapter);
		break;

	case QLCNIC_FORCE_FW_RESET:
		netdev_info(netdev, "Forcing a FW reset\n");
		qlcnic_dev_request_reset(adapter, val->flag);
		adapter->flags &= ~QLCNIC_FW_RESET_OWNER;
		break;

	case QLCNIC_SET_QUIESCENT:
	case QLCNIC_RESET_QUIESCENT:
		if (test_bit(__QLCNIC_MAINTENANCE_MODE, &adapter->state))
			netdev_info(netdev, "Device is in non-operational state\n");
		break;

	default:
		if (!fw_dump->tmpl_hdr) {
			netdev_err(netdev, "FW dump not supported\n");
			ret = -EOPNOTSUPP;
			break;
		}

		for (i = 0; i < ARRAY_SIZE(qlcnic_fw_dump_level); i++) {
			if (val->flag == qlcnic_fw_dump_level[i]) {
				valid_mask = true;
				break;
			}
		}

		if (valid_mask) {
			ret = qlcnic_set_dump_mask(adapter, val->flag);
		} else {
			netdev_info(netdev, "Invalid dump level: 0x%x\n",
				    val->flag);
			ret = -EINVAL;
		}
	}
>>>>>>> refs/remotes/origin/master
	return ret;
}

const struct ethtool_ops qlcnic_ethtool_ops = {
	.get_settings = qlcnic_get_settings,
	.set_settings = qlcnic_set_settings,
	.get_drvinfo = qlcnic_get_drvinfo,
	.get_regs_len = qlcnic_get_regs_len,
	.get_regs = qlcnic_get_regs,
	.get_link = ethtool_op_get_link,
	.get_eeprom_len = qlcnic_get_eeprom_len,
	.get_eeprom = qlcnic_get_eeprom,
	.get_ringparam = qlcnic_get_ringparam,
	.set_ringparam = qlcnic_set_ringparam,
	.get_channels = qlcnic_get_channels,
	.set_channels = qlcnic_set_channels,
	.get_pauseparam = qlcnic_get_pauseparam,
	.set_pauseparam = qlcnic_set_pauseparam,
	.get_wol = qlcnic_get_wol,
	.set_wol = qlcnic_set_wol,
	.self_test = qlcnic_diag_test,
	.get_strings = qlcnic_get_strings,
	.get_ethtool_stats = qlcnic_get_ethtool_stats,
	.get_sset_count = qlcnic_get_sset_count,
	.get_coalesce = qlcnic_get_intr_coalesce,
	.set_coalesce = qlcnic_set_intr_coalesce,
	.set_phys_id = qlcnic_set_led,
	.set_msglevel = qlcnic_set_msglevel,
	.get_msglevel = qlcnic_get_msglevel,
	.get_dump_flag = qlcnic_get_dump_flag,
	.get_dump_data = qlcnic_get_dump_data,
	.set_dump = qlcnic_set_dump,
};
<<<<<<< HEAD
=======

const struct ethtool_ops qlcnic_sriov_vf_ethtool_ops = {
	.get_settings		= qlcnic_get_settings,
	.get_drvinfo		= qlcnic_get_drvinfo,
	.get_regs_len		= qlcnic_get_regs_len,
	.get_regs		= qlcnic_get_regs,
	.get_link		= ethtool_op_get_link,
	.get_eeprom_len		= qlcnic_get_eeprom_len,
	.get_eeprom		= qlcnic_get_eeprom,
	.get_ringparam		= qlcnic_get_ringparam,
	.set_ringparam		= qlcnic_set_ringparam,
	.get_channels		= qlcnic_get_channels,
	.get_pauseparam		= qlcnic_get_pauseparam,
	.get_wol		= qlcnic_get_wol,
	.get_strings		= qlcnic_get_strings,
	.get_ethtool_stats	= qlcnic_get_ethtool_stats,
	.get_sset_count		= qlcnic_get_sset_count,
	.get_coalesce		= qlcnic_get_intr_coalesce,
	.set_coalesce		= qlcnic_set_intr_coalesce,
	.set_msglevel		= qlcnic_set_msglevel,
	.get_msglevel		= qlcnic_get_msglevel,
};

const struct ethtool_ops qlcnic_ethtool_failed_ops = {
	.get_settings		= qlcnic_get_settings,
	.get_drvinfo		= qlcnic_get_drvinfo,
	.set_msglevel		= qlcnic_set_msglevel,
	.get_msglevel		= qlcnic_get_msglevel,
	.set_dump		= qlcnic_set_dump,
};
>>>>>>> refs/remotes/origin/master
