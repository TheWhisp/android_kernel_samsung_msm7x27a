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

<<<<<<< HEAD
#include <linux/slab.h>
=======
>>>>>>> refs/remotes/origin/master
#include <linux/vmalloc.h>
#include <linux/interrupt.h>

#include "qlcnic.h"
<<<<<<< HEAD

#include <linux/swab.h>
#include <linux/dma-mapping.h>
#include <net/ip.h>
#include <linux/ipv6.h>
#include <linux/inetdevice.h>
#include <linux/sysfs.h>
#include <linux/aer.h>
#include <linux/log2.h>
=======
#include "qlcnic_sriov.h"
#include "qlcnic_hw.h"

#include <linux/swab.h>
#include <linux/dma-mapping.h>
#include <linux/if_vlan.h>
#include <net/ip.h>
#include <linux/ipv6.h>
#include <linux/inetdevice.h>
#include <linux/aer.h>
#include <linux/log2.h>
#include <linux/pci.h>
>>>>>>> refs/remotes/origin/master

MODULE_DESCRIPTION("QLogic 1/10 GbE Converged/Intelligent Ethernet Driver");
MODULE_LICENSE("GPL");
MODULE_VERSION(QLCNIC_LINUX_VERSIONID);
MODULE_FIRMWARE(QLCNIC_UNIFIED_ROMIMAGE_NAME);

char qlcnic_driver_name[] = "qlcnic";
static const char qlcnic_driver_string[] = "QLogic 1/10 GbE "
	"Converged/Intelligent Ethernet Driver v" QLCNIC_LINUX_VERSIONID;

<<<<<<< HEAD
static struct workqueue_struct *qlcnic_wq;
static int qlcnic_mac_learn;
module_param(qlcnic_mac_learn, int, 0444);
MODULE_PARM_DESC(qlcnic_mac_learn, "Mac Filter (0=disabled, 1=enabled)");

static int use_msi = 1;
module_param(use_msi, int, 0444);
MODULE_PARM_DESC(use_msi, "MSI interrupt (0=disabled, 1=enabled");

static int use_msi_x = 1;
module_param(use_msi_x, int, 0444);
MODULE_PARM_DESC(use_msi_x, "MSI-X interrupt (0=disabled, 1=enabled");

static int auto_fw_reset = 1;
module_param(auto_fw_reset, int, 0644);
MODULE_PARM_DESC(auto_fw_reset, "Auto firmware reset (0=disabled, 1=enabled");

static int load_fw_file;
module_param(load_fw_file, int, 0444);
MODULE_PARM_DESC(load_fw_file, "Load firmware from (0=flash, 1=file");

static int qlcnic_config_npars;
module_param(qlcnic_config_npars, int, 0444);
MODULE_PARM_DESC(qlcnic_config_npars, "Configure NPARs (0=disabled, 1=enabled");

static int __devinit qlcnic_probe(struct pci_dev *pdev,
		const struct pci_device_id *ent);
static void __devexit qlcnic_remove(struct pci_dev *pdev);
=======
static int qlcnic_mac_learn;
module_param(qlcnic_mac_learn, int, 0444);
MODULE_PARM_DESC(qlcnic_mac_learn,
		 "Mac Filter (0=learning is disabled, 1=Driver learning is enabled, 2=FDB learning is enabled)");

int qlcnic_use_msi = 1;
MODULE_PARM_DESC(use_msi, "MSI interrupt (0=disabled, 1=enabled)");
module_param_named(use_msi, qlcnic_use_msi, int, 0444);

int qlcnic_use_msi_x = 1;
MODULE_PARM_DESC(use_msi_x, "MSI-X interrupt (0=disabled, 1=enabled)");
module_param_named(use_msi_x, qlcnic_use_msi_x, int, 0444);

int qlcnic_auto_fw_reset = 1;
MODULE_PARM_DESC(auto_fw_reset, "Auto firmware reset (0=disabled, 1=enabled)");
module_param_named(auto_fw_reset, qlcnic_auto_fw_reset, int, 0644);

int qlcnic_load_fw_file;
MODULE_PARM_DESC(load_fw_file, "Load firmware from (0=flash, 1=file)");
module_param_named(load_fw_file, qlcnic_load_fw_file, int, 0444);

static int qlcnic_probe(struct pci_dev *pdev, const struct pci_device_id *ent);
static void qlcnic_remove(struct pci_dev *pdev);
>>>>>>> refs/remotes/origin/master
static int qlcnic_open(struct net_device *netdev);
static int qlcnic_close(struct net_device *netdev);
static void qlcnic_tx_timeout(struct net_device *netdev);
static void qlcnic_attach_work(struct work_struct *work);
static void qlcnic_fwinit_work(struct work_struct *work);
<<<<<<< HEAD
static void qlcnic_fw_poll_work(struct work_struct *work);
static void qlcnic_schedule_work(struct qlcnic_adapter *adapter,
		work_func_t func, int delay);
static void qlcnic_cancel_fw_work(struct qlcnic_adapter *adapter);
static int qlcnic_poll(struct napi_struct *napi, int budget);
static int qlcnic_rx_poll(struct napi_struct *napi, int budget);
=======
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_NET_POLL_CONTROLLER
static void qlcnic_poll_controller(struct net_device *netdev);
#endif

<<<<<<< HEAD
static void qlcnic_create_sysfs_entries(struct qlcnic_adapter *adapter);
static void qlcnic_remove_sysfs_entries(struct qlcnic_adapter *adapter);
static void qlcnic_create_diag_entries(struct qlcnic_adapter *adapter);
static void qlcnic_remove_diag_entries(struct qlcnic_adapter *adapter);

static void qlcnic_idc_debug_info(struct qlcnic_adapter *adapter, u8 encoding);
static void qlcnic_clr_all_drv_state(struct qlcnic_adapter *adapter, u8);
=======
static void qlcnic_idc_debug_info(struct qlcnic_adapter *adapter, u8 encoding);
>>>>>>> refs/remotes/origin/master
static int qlcnic_can_start_firmware(struct qlcnic_adapter *adapter);

static irqreturn_t qlcnic_tmp_intr(int irq, void *data);
static irqreturn_t qlcnic_intr(int irq, void *data);
static irqreturn_t qlcnic_msi_intr(int irq, void *data);
static irqreturn_t qlcnic_msix_intr(int irq, void *data);
<<<<<<< HEAD

static struct net_device_stats *qlcnic_get_stats(struct net_device *netdev);
static void qlcnic_restore_indev_addr(struct net_device *dev, unsigned long);
=======
static irqreturn_t qlcnic_msix_tx_intr(int irq, void *data);

static struct net_device_stats *qlcnic_get_stats(struct net_device *netdev);
>>>>>>> refs/remotes/origin/master
static int qlcnic_start_firmware(struct qlcnic_adapter *);

static void qlcnic_free_lb_filters_mem(struct qlcnic_adapter *adapter);
static void qlcnic_dev_set_npar_ready(struct qlcnic_adapter *);
<<<<<<< HEAD
static int qlcnicvf_config_led(struct qlcnic_adapter *, u32, u32);
static int qlcnicvf_config_bridged_mode(struct qlcnic_adapter *, u32);
static int qlcnicvf_start_firmware(struct qlcnic_adapter *);
static void qlcnic_set_netdev_features(struct qlcnic_adapter *,
				struct qlcnic_esw_func_cfg *);
static int qlcnic_vlan_rx_add(struct net_device *, u16);
static int qlcnic_vlan_rx_del(struct net_device *, u16);
=======
static int qlcnicvf_start_firmware(struct qlcnic_adapter *);
static int qlcnic_vlan_rx_add(struct net_device *, __be16, u16);
static int qlcnic_vlan_rx_del(struct net_device *, __be16, u16);

static u32 qlcnic_vlan_tx_check(struct qlcnic_adapter *adapter)
{
	struct qlcnic_hardware_context *ahw = adapter->ahw;

	if (adapter->pdev->device == PCI_DEVICE_ID_QLOGIC_QLE824X)
		return ahw->capabilities & QLCNIC_FW_CAPABILITY_FVLANTX;
	else
		return 1;
}
>>>>>>> refs/remotes/origin/master

/*  PCI Device ID Table  */
#define ENTRY(device) \
	{PCI_DEVICE(PCI_VENDOR_ID_QLOGIC, (device)), \
	.class = PCI_CLASS_NETWORK_ETHERNET << 8, .class_mask = ~0}

<<<<<<< HEAD
#define PCI_DEVICE_ID_QLOGIC_QLE824X  0x8020

static DEFINE_PCI_DEVICE_TABLE(qlcnic_pci_tbl) = {
	ENTRY(PCI_DEVICE_ID_QLOGIC_QLE824X),
=======
static DEFINE_PCI_DEVICE_TABLE(qlcnic_pci_tbl) = {
	ENTRY(PCI_DEVICE_ID_QLOGIC_QLE824X),
	ENTRY(PCI_DEVICE_ID_QLOGIC_QLE834X),
	ENTRY(PCI_DEVICE_ID_QLOGIC_VF_QLE834X),
	ENTRY(PCI_DEVICE_ID_QLOGIC_QLE844X),
	ENTRY(PCI_DEVICE_ID_QLOGIC_VF_QLE844X),
>>>>>>> refs/remotes/origin/master
	{0,}
};

MODULE_DEVICE_TABLE(pci, qlcnic_pci_tbl);


<<<<<<< HEAD
inline void
qlcnic_update_cmd_producer(struct qlcnic_adapter *adapter,
		struct qlcnic_host_tx_ring *tx_ring)
=======
inline void qlcnic_update_cmd_producer(struct qlcnic_host_tx_ring *tx_ring)
>>>>>>> refs/remotes/origin/master
{
	writel(tx_ring->producer, tx_ring->crb_cmd_producer);
}

static const u32 msi_tgt_status[8] = {
	ISR_INT_TARGET_STATUS, ISR_INT_TARGET_STATUS_F1,
	ISR_INT_TARGET_STATUS_F2, ISR_INT_TARGET_STATUS_F3,
	ISR_INT_TARGET_STATUS_F4, ISR_INT_TARGET_STATUS_F5,
	ISR_INT_TARGET_STATUS_F6, ISR_INT_TARGET_STATUS_F7
};

<<<<<<< HEAD
static const
struct qlcnic_legacy_intr_set legacy_intr[] = QLCNIC_LEGACY_INTR_CONFIG;

static inline void qlcnic_disable_int(struct qlcnic_host_sds_ring *sds_ring)
{
	writel(0, sds_ring->crb_intr_mask);
}

static inline void qlcnic_enable_int(struct qlcnic_host_sds_ring *sds_ring)
{
	struct qlcnic_adapter *adapter = sds_ring->adapter;

	writel(0x1, sds_ring->crb_intr_mask);

	if (!QLCNIC_IS_MSI_FAMILY(adapter))
		writel(0xfbff, adapter->tgt_mask_reg);
}

static int
qlcnic_alloc_sds_rings(struct qlcnic_recv_context *recv_ctx, int count)
=======
static const u32 qlcnic_reg_tbl[] = {
	0x1B20A8,	/* PEG_HALT_STAT1 */
	0x1B20AC,	/* PEG_HALT_STAT2 */
	0x1B20B0,	/* FW_HEARTBEAT */
	0x1B2100,	/* LOCK ID */
	0x1B2128,	/* FW_CAPABILITIES */
	0x1B2138,	/* drv active */
	0x1B2140,	/* dev state */
	0x1B2144,	/* drv state */
	0x1B2148,	/* drv scratch */
	0x1B214C,	/* dev partition info */
	0x1B2174,	/* drv idc ver */
	0x1B2150,	/* fw version major */
	0x1B2154,	/* fw version minor */
	0x1B2158,	/* fw version sub */
	0x1B219C,	/* npar state */
	0x1B21FC,	/* FW_IMG_VALID */
	0x1B2250,	/* CMD_PEG_STATE */
	0x1B233C,	/* RCV_PEG_STATE */
	0x1B23B4,	/* ASIC TEMP */
	0x1B216C,	/* FW api */
	0x1B2170,	/* drv op mode */
	0x13C010,	/* flash lock */
	0x13C014,	/* flash unlock */
};

static const struct qlcnic_board_info qlcnic_boards[] = {
	{ PCI_VENDOR_ID_QLOGIC,
	  PCI_DEVICE_ID_QLOGIC_QLE844X,
	  0x0,
	  0x0,
	  "8400 series 10GbE Converged Network Adapter (TCP/IP Networking)" },
	{ PCI_VENDOR_ID_QLOGIC,
	  PCI_DEVICE_ID_QLOGIC_QLE834X,
	  PCI_VENDOR_ID_QLOGIC,
	  0x24e,
	  "8300 Series Dual Port 10GbE Converged Network Adapter "
	  "(TCP/IP Networking)" },
	{ PCI_VENDOR_ID_QLOGIC,
	  PCI_DEVICE_ID_QLOGIC_QLE834X,
	  PCI_VENDOR_ID_QLOGIC,
	  0x243,
	  "8300 Series Single Port 10GbE Converged Network Adapter "
	  "(TCP/IP Networking)" },
	{ PCI_VENDOR_ID_QLOGIC,
	  PCI_DEVICE_ID_QLOGIC_QLE834X,
	  PCI_VENDOR_ID_QLOGIC,
	  0x24a,
	  "8300 Series Dual Port 10GbE Converged Network Adapter "
	  "(TCP/IP Networking)" },
	{ PCI_VENDOR_ID_QLOGIC,
	  PCI_DEVICE_ID_QLOGIC_QLE834X,
	  PCI_VENDOR_ID_QLOGIC,
	  0x246,
	  "8300 Series Dual Port 10GbE Converged Network Adapter "
	  "(TCP/IP Networking)" },
	{ PCI_VENDOR_ID_QLOGIC,
	  PCI_DEVICE_ID_QLOGIC_QLE834X,
	  PCI_VENDOR_ID_QLOGIC,
	  0x252,
	  "8300 Series Dual Port 10GbE Converged Network Adapter "
	  "(TCP/IP Networking)" },
	{ PCI_VENDOR_ID_QLOGIC,
	  PCI_DEVICE_ID_QLOGIC_QLE834X,
	  PCI_VENDOR_ID_QLOGIC,
	  0x26e,
	  "8300 Series Dual Port 10GbE Converged Network Adapter "
	  "(TCP/IP Networking)" },
	{ PCI_VENDOR_ID_QLOGIC,
	  PCI_DEVICE_ID_QLOGIC_QLE834X,
	  PCI_VENDOR_ID_QLOGIC,
	  0x260,
	  "8300 Series Dual Port 10GbE Converged Network Adapter "
	  "(TCP/IP Networking)" },
	{ PCI_VENDOR_ID_QLOGIC,
	  PCI_DEVICE_ID_QLOGIC_QLE834X,
	  PCI_VENDOR_ID_QLOGIC,
	  0x266,
	  "8300 Series Single Port 10GbE Converged Network Adapter "
	  "(TCP/IP Networking)" },
	{ PCI_VENDOR_ID_QLOGIC,
	  PCI_DEVICE_ID_QLOGIC_QLE834X,
	  PCI_VENDOR_ID_QLOGIC,
	  0x269,
	  "8300 Series Dual Port 10GbE Converged Network Adapter "
	  "(TCP/IP Networking)" },
	{ PCI_VENDOR_ID_QLOGIC,
	  PCI_DEVICE_ID_QLOGIC_QLE834X,
	  PCI_VENDOR_ID_QLOGIC,
	  0x271,
	  "8300 Series Dual Port 10GbE Converged Network Adapter "
	  "(TCP/IP Networking)" },
	{ PCI_VENDOR_ID_QLOGIC,
	  PCI_DEVICE_ID_QLOGIC_QLE834X,
	  0x0, 0x0, "8300 Series 1/10GbE Controller" },
	{ PCI_VENDOR_ID_QLOGIC,
	  PCI_DEVICE_ID_QLOGIC_QLE824X,
	  PCI_VENDOR_ID_QLOGIC,
	  0x203,
	  "8200 Series Single Port 10GbE Converged Network Adapter"
	  "(TCP/IP Networking)" },
	{ PCI_VENDOR_ID_QLOGIC,
	  PCI_DEVICE_ID_QLOGIC_QLE824X,
	  PCI_VENDOR_ID_QLOGIC,
	  0x207,
	  "8200 Series Dual Port 10GbE Converged Network Adapter"
	  "(TCP/IP Networking)" },
	{ PCI_VENDOR_ID_QLOGIC,
	  PCI_DEVICE_ID_QLOGIC_QLE824X,
	  PCI_VENDOR_ID_QLOGIC,
	  0x20b,
	  "3200 Series Dual Port 10Gb Intelligent Ethernet Adapter" },
	{ PCI_VENDOR_ID_QLOGIC,
	  PCI_DEVICE_ID_QLOGIC_QLE824X,
	  PCI_VENDOR_ID_QLOGIC,
	  0x20c,
	  "3200 Series Quad Port 1Gb Intelligent Ethernet Adapter" },
	{ PCI_VENDOR_ID_QLOGIC,
	  PCI_DEVICE_ID_QLOGIC_QLE824X,
	  PCI_VENDOR_ID_QLOGIC,
	  0x20f,
	  "3200 Series Single Port 10Gb Intelligent Ethernet Adapter" },
	{ PCI_VENDOR_ID_QLOGIC,
	  PCI_DEVICE_ID_QLOGIC_QLE824X,
	  0x103c, 0x3733,
	  "NC523SFP 10Gb 2-port Server Adapter" },
	{ PCI_VENDOR_ID_QLOGIC,
	  PCI_DEVICE_ID_QLOGIC_QLE824X,
	  0x103c, 0x3346,
	  "CN1000Q Dual Port Converged Network Adapter" },
	{ PCI_VENDOR_ID_QLOGIC,
	  PCI_DEVICE_ID_QLOGIC_QLE824X,
	  PCI_VENDOR_ID_QLOGIC,
	  0x210,
	  "QME8242-k 10GbE Dual Port Mezzanine Card" },
	{ PCI_VENDOR_ID_QLOGIC,
	  PCI_DEVICE_ID_QLOGIC_QLE824X,
	  0x0, 0x0, "cLOM8214 1/10GbE Controller" },
};

#define NUM_SUPPORTED_BOARDS ARRAY_SIZE(qlcnic_boards)

static const
struct qlcnic_legacy_intr_set legacy_intr[] = QLCNIC_LEGACY_INTR_CONFIG;

int qlcnic_alloc_sds_rings(struct qlcnic_recv_context *recv_ctx, int count)
>>>>>>> refs/remotes/origin/master
{
	int size = sizeof(struct qlcnic_host_sds_ring) * count;

	recv_ctx->sds_rings = kzalloc(size, GFP_KERNEL);

	return recv_ctx->sds_rings == NULL;
}

<<<<<<< HEAD
static void
qlcnic_free_sds_rings(struct qlcnic_recv_context *recv_ctx)
=======
void qlcnic_free_sds_rings(struct qlcnic_recv_context *recv_ctx)
>>>>>>> refs/remotes/origin/master
{
	if (recv_ctx->sds_rings != NULL)
		kfree(recv_ctx->sds_rings);

	recv_ctx->sds_rings = NULL;
}

<<<<<<< HEAD
static int
qlcnic_napi_add(struct qlcnic_adapter *adapter, struct net_device *netdev)
{
	int ring;
	struct qlcnic_host_sds_ring *sds_ring;
	struct qlcnic_recv_context *recv_ctx = adapter->recv_ctx;

	if (qlcnic_alloc_sds_rings(recv_ctx, adapter->max_sds_rings))
		return -ENOMEM;

	for (ring = 0; ring < adapter->max_sds_rings; ring++) {
		sds_ring = &recv_ctx->sds_rings[ring];

		if (ring == adapter->max_sds_rings - 1)
			netif_napi_add(netdev, &sds_ring->napi, qlcnic_poll,
				QLCNIC_NETDEV_WEIGHT/adapter->max_sds_rings);
		else
			netif_napi_add(netdev, &sds_ring->napi,
				qlcnic_rx_poll, QLCNIC_NETDEV_WEIGHT*2);
	}
=======
int qlcnic_read_mac_addr(struct qlcnic_adapter *adapter)
{
	struct net_device *netdev = adapter->netdev;
	struct pci_dev *pdev = adapter->pdev;
	u8 mac_addr[ETH_ALEN];
	int ret;

	ret = qlcnic_get_mac_address(adapter, mac_addr,
				     adapter->ahw->pci_func);
	if (ret)
		return ret;

	memcpy(netdev->dev_addr, mac_addr, ETH_ALEN);
	memcpy(adapter->mac_addr, netdev->dev_addr, netdev->addr_len);

	/* set station address */

	if (!is_valid_ether_addr(netdev->dev_addr))
		dev_warn(&pdev->dev, "Bad MAC address %pM.\n",
					netdev->dev_addr);
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
static void
qlcnic_napi_del(struct qlcnic_adapter *adapter)
{
	int ring;
	struct qlcnic_host_sds_ring *sds_ring;
	struct qlcnic_recv_context *recv_ctx = adapter->recv_ctx;

	for (ring = 0; ring < adapter->max_sds_rings; ring++) {
		sds_ring = &recv_ctx->sds_rings[ring];
		netif_napi_del(&sds_ring->napi);
	}

	qlcnic_free_sds_rings(adapter->recv_ctx);
}

static void
qlcnic_napi_enable(struct qlcnic_adapter *adapter)
{
	int ring;
	struct qlcnic_host_sds_ring *sds_ring;
	struct qlcnic_recv_context *recv_ctx = adapter->recv_ctx;

	if (adapter->is_up != QLCNIC_ADAPTER_UP_MAGIC)
		return;

	for (ring = 0; ring < adapter->max_sds_rings; ring++) {
		sds_ring = &recv_ctx->sds_rings[ring];
		napi_enable(&sds_ring->napi);
		qlcnic_enable_int(sds_ring);
	}
}

static void
qlcnic_napi_disable(struct qlcnic_adapter *adapter)
{
	int ring;
	struct qlcnic_host_sds_ring *sds_ring;
	struct qlcnic_recv_context *recv_ctx = adapter->recv_ctx;

	if (adapter->is_up != QLCNIC_ADAPTER_UP_MAGIC)
		return;

	for (ring = 0; ring < adapter->max_sds_rings; ring++) {
		sds_ring = &recv_ctx->sds_rings[ring];
		qlcnic_disable_int(sds_ring);
		napi_synchronize(&sds_ring->napi);
		napi_disable(&sds_ring->napi);
	}
}

static void qlcnic_clear_stats(struct qlcnic_adapter *adapter)
{
	memset(&adapter->stats, 0, sizeof(adapter->stats));
}

static void qlcnic_set_msix_bit(struct pci_dev *pdev, int enable)
{
	u32 control;
	int pos;

	pos = pci_find_capability(pdev, PCI_CAP_ID_MSIX);
	if (pos) {
		pci_read_config_dword(pdev, pos, &control);
		if (enable)
			control |= PCI_MSIX_FLAGS_ENABLE;
		else
			control = 0;
		pci_write_config_dword(pdev, pos, control);
	}
}

static void qlcnic_init_msix_entries(struct qlcnic_adapter *adapter, int count)
{
	int i;

	for (i = 0; i < count; i++)
		adapter->msix_entries[i].entry = i;
}

static int
qlcnic_read_mac_addr(struct qlcnic_adapter *adapter)
{
	u8 mac_addr[ETH_ALEN];
	struct net_device *netdev = adapter->netdev;
	struct pci_dev *pdev = adapter->pdev;

	if (qlcnic_get_mac_address(adapter, mac_addr) != 0)
		return -EIO;

	memcpy(netdev->dev_addr, mac_addr, ETH_ALEN);
	memcpy(netdev->perm_addr, netdev->dev_addr, netdev->addr_len);
	memcpy(adapter->mac_addr, netdev->dev_addr, netdev->addr_len);

	/* set station address */

	if (!is_valid_ether_addr(netdev->perm_addr))
		dev_warn(&pdev->dev, "Bad MAC address %pM.\n",
					netdev->dev_addr);

	return 0;
}

static int qlcnic_set_mac(struct net_device *netdev, void *p)
{
	struct qlcnic_adapter *adapter = netdev_priv(netdev);
	struct sockaddr *addr = p;

	if ((adapter->flags & QLCNIC_MAC_OVERRIDE_DISABLED))
		return -EOPNOTSUPP;

	if (!is_valid_ether_addr(addr->sa_data))
		return -EADDRNOTAVAIL;

	if (test_bit(__QLCNIC_DEV_UP, &adapter->state)) {
		netif_device_detach(netdev);
		qlcnic_napi_disable(adapter);
	}

	memcpy(adapter->mac_addr, addr->sa_data, netdev->addr_len);
	memcpy(netdev->dev_addr, addr->sa_data, netdev->addr_len);
	qlcnic_set_multi(adapter->netdev);

	if (test_bit(__QLCNIC_DEV_UP, &adapter->state)) {
		netif_device_attach(netdev);
		qlcnic_napi_enable(adapter);
	}
=======
static void qlcnic_delete_adapter_mac(struct qlcnic_adapter *adapter)
{
	struct qlcnic_mac_list_s *cur;
	struct list_head *head;

	list_for_each(head, &adapter->mac_list) {
		cur = list_entry(head, struct qlcnic_mac_list_s, list);
		if (!memcmp(adapter->mac_addr, cur->mac_addr, ETH_ALEN)) {
			qlcnic_sre_macaddr_change(adapter, cur->mac_addr,
						  0, QLCNIC_MAC_DEL);
			list_del(&cur->list);
			kfree(cur);
			return;
		}
	}
}

static int qlcnic_set_mac(struct net_device *netdev, void *p)
{
	struct qlcnic_adapter *adapter = netdev_priv(netdev);
	struct sockaddr *addr = p;

	if (qlcnic_sriov_vf_check(adapter))
		return -EINVAL;

	if ((adapter->flags & QLCNIC_MAC_OVERRIDE_DISABLED))
		return -EOPNOTSUPP;

	if (!is_valid_ether_addr(addr->sa_data))
		return -EINVAL;

	if (!memcmp(adapter->mac_addr, addr->sa_data, ETH_ALEN))
		return 0;

	if (test_bit(__QLCNIC_DEV_UP, &adapter->state)) {
		netif_device_detach(netdev);
		qlcnic_napi_disable(adapter);
	}

	qlcnic_delete_adapter_mac(adapter);
	memcpy(adapter->mac_addr, addr->sa_data, netdev->addr_len);
	memcpy(netdev->dev_addr, addr->sa_data, netdev->addr_len);
	qlcnic_set_multi(adapter->netdev);

	if (test_bit(__QLCNIC_DEV_UP, &adapter->state)) {
		netif_device_attach(netdev);
		qlcnic_napi_enable(adapter);
	}
	return 0;
}

static int qlcnic_fdb_del(struct ndmsg *ndm, struct nlattr *tb[],
			struct net_device *netdev, const unsigned char *addr)
{
	struct qlcnic_adapter *adapter = netdev_priv(netdev);
	int err = -EOPNOTSUPP;

	if (!adapter->fdb_mac_learn)
		return ndo_dflt_fdb_del(ndm, tb, netdev, addr);

	if (adapter->flags & QLCNIC_ESWITCH_ENABLED) {
		if (is_unicast_ether_addr(addr)) {
			err = dev_uc_del(netdev, addr);
			if (!err)
				err = qlcnic_nic_del_mac(adapter, addr);
		} else if (is_multicast_ether_addr(addr)) {
			err = dev_mc_del(netdev, addr);
		} else {
			err =  -EINVAL;
		}
	}
	return err;
}

static int qlcnic_fdb_add(struct ndmsg *ndm, struct nlattr *tb[],
			struct net_device *netdev,
			const unsigned char *addr, u16 flags)
{
	struct qlcnic_adapter *adapter = netdev_priv(netdev);
	int err = 0;

	if (!adapter->fdb_mac_learn)
		return ndo_dflt_fdb_add(ndm, tb, netdev, addr, flags);

	if (!(adapter->flags & QLCNIC_ESWITCH_ENABLED)) {
		pr_info("%s: FDB e-switch is not enabled\n", __func__);
		return -EOPNOTSUPP;
	}

	if (ether_addr_equal(addr, adapter->mac_addr))
		return err;

	if (is_unicast_ether_addr(addr)) {
		if (netdev_uc_count(netdev) < adapter->ahw->max_uc_count)
			err = dev_uc_add_excl(netdev, addr);
		else
			err = -ENOMEM;
	} else if (is_multicast_ether_addr(addr)) {
		err = dev_mc_add_excl(netdev, addr);
	} else {
		err = -EINVAL;
	}

	return err;
}

static int qlcnic_fdb_dump(struct sk_buff *skb, struct netlink_callback *ncb,
			struct net_device *netdev, int idx)
{
	struct qlcnic_adapter *adapter = netdev_priv(netdev);

	if (!adapter->fdb_mac_learn)
		return ndo_dflt_fdb_dump(skb, ncb, netdev, idx);

	if (adapter->flags & QLCNIC_ESWITCH_ENABLED)
		idx = ndo_dflt_fdb_dump(skb, ncb, netdev, idx);

	return idx;
}

static void qlcnic_82xx_cancel_idc_work(struct qlcnic_adapter *adapter)
{
	while (test_and_set_bit(__QLCNIC_RESETTING, &adapter->state))
		usleep_range(10000, 11000);

	if (!adapter->fw_work.work.func)
		return;

	cancel_delayed_work_sync(&adapter->fw_work);
}

static int qlcnic_get_phys_port_id(struct net_device *netdev,
				   struct netdev_phys_port_id *ppid)
{
	struct qlcnic_adapter *adapter = netdev_priv(netdev);
	struct qlcnic_hardware_context *ahw = adapter->ahw;

	if (!(adapter->flags & QLCNIC_HAS_PHYS_PORT_ID))
		return -EOPNOTSUPP;

	ppid->id_len = sizeof(ahw->phys_port_id);
	memcpy(ppid->id, ahw->phys_port_id, ppid->id_len);

>>>>>>> refs/remotes/origin/master
	return 0;
}

static const struct net_device_ops qlcnic_netdev_ops = {
	.ndo_open	   = qlcnic_open,
	.ndo_stop	   = qlcnic_close,
	.ndo_start_xmit    = qlcnic_xmit_frame,
	.ndo_get_stats	   = qlcnic_get_stats,
	.ndo_validate_addr = eth_validate_addr,
	.ndo_set_rx_mode   = qlcnic_set_multi,
	.ndo_set_mac_address    = qlcnic_set_mac,
	.ndo_change_mtu	   = qlcnic_change_mtu,
	.ndo_fix_features  = qlcnic_fix_features,
	.ndo_set_features  = qlcnic_set_features,
	.ndo_tx_timeout	   = qlcnic_tx_timeout,
	.ndo_vlan_rx_add_vid	= qlcnic_vlan_rx_add,
	.ndo_vlan_rx_kill_vid	= qlcnic_vlan_rx_del,
<<<<<<< HEAD
#ifdef CONFIG_NET_POLL_CONTROLLER
	.ndo_poll_controller = qlcnic_poll_controller,
#endif
};

static struct qlcnic_nic_template qlcnic_ops = {
	.config_bridged_mode = qlcnic_config_bridged_mode,
	.config_led = qlcnic_config_led,
	.start_firmware = qlcnic_start_firmware
};

static struct qlcnic_nic_template qlcnic_vf_ops = {
	.config_bridged_mode = qlcnicvf_config_bridged_mode,
	.config_led = qlcnicvf_config_led,
	.start_firmware = qlcnicvf_start_firmware
};

static int qlcnic_enable_msix(struct qlcnic_adapter *adapter, u32 num_msix)
{
	struct pci_dev *pdev = adapter->pdev;
	int err = -1;

	adapter->max_sds_rings = 1;
	adapter->flags &= ~(QLCNIC_MSI_ENABLED | QLCNIC_MSIX_ENABLED);
	qlcnic_set_msix_bit(pdev, 0);

	if (adapter->msix_supported) {
 enable_msix:
		qlcnic_init_msix_entries(adapter, num_msix);
		err = pci_enable_msix(pdev, adapter->msix_entries, num_msix);
		if (err == 0) {
			adapter->flags |= QLCNIC_MSIX_ENABLED;
			qlcnic_set_msix_bit(pdev, 1);

			adapter->max_sds_rings = num_msix;

			dev_info(&pdev->dev, "using msi-x interrupts\n");
			return err;
		}
		if (err > 0) {
			num_msix = rounddown_pow_of_two(err);
			if (num_msix)
				goto enable_msix;
		}
	}
	return err;
}


static void qlcnic_enable_msi_legacy(struct qlcnic_adapter *adapter)
{
	const struct qlcnic_legacy_intr_set *legacy_intrp;
	struct pci_dev *pdev = adapter->pdev;

	if (use_msi && !pci_enable_msi(pdev)) {
		adapter->flags |= QLCNIC_MSI_ENABLED;
		adapter->tgt_status_reg = qlcnic_get_ioaddr(adapter,
				msi_tgt_status[adapter->ahw->pci_func]);
		dev_info(&pdev->dev, "using msi interrupts\n");
		adapter->msix_entries[0].vector = pdev->irq;
		return;
	}

	legacy_intrp = &legacy_intr[adapter->ahw->pci_func];

	adapter->int_vec_bit = legacy_intrp->int_vec_bit;
	adapter->tgt_status_reg = qlcnic_get_ioaddr(adapter,
			legacy_intrp->tgt_status_reg);
	adapter->tgt_mask_reg = qlcnic_get_ioaddr(adapter,
			legacy_intrp->tgt_mask_reg);
	adapter->isr_int_vec = qlcnic_get_ioaddr(adapter, ISR_INT_VECTOR);

	adapter->crb_int_state_reg = qlcnic_get_ioaddr(adapter,
			ISR_INT_STATE_REG);
	dev_info(&pdev->dev, "using legacy interrupts\n");
	adapter->msix_entries[0].vector = pdev->irq;
}

static void
qlcnic_setup_intr(struct qlcnic_adapter *adapter)
{
	int num_msix;

	if (adapter->msix_supported) {
		num_msix = rounddown_pow_of_two(min_t(int, num_online_cpus(),
				QLCNIC_DEF_NUM_STS_DESC_RINGS));
	} else
		num_msix = 1;

	if (!qlcnic_enable_msix(adapter, num_msix))
		return;

	qlcnic_enable_msi_legacy(adapter);
}

static void
qlcnic_teardown_intr(struct qlcnic_adapter *adapter)
=======
	.ndo_fdb_add		= qlcnic_fdb_add,
	.ndo_fdb_del		= qlcnic_fdb_del,
	.ndo_fdb_dump		= qlcnic_fdb_dump,
	.ndo_get_phys_port_id	= qlcnic_get_phys_port_id,
#ifdef CONFIG_NET_POLL_CONTROLLER
	.ndo_poll_controller = qlcnic_poll_controller,
#endif
#ifdef CONFIG_QLCNIC_SRIOV
	.ndo_set_vf_mac		= qlcnic_sriov_set_vf_mac,
	.ndo_set_vf_tx_rate	= qlcnic_sriov_set_vf_tx_rate,
	.ndo_get_vf_config	= qlcnic_sriov_get_vf_config,
	.ndo_set_vf_vlan	= qlcnic_sriov_set_vf_vlan,
	.ndo_set_vf_spoofchk	= qlcnic_sriov_set_vf_spoofchk,
#endif
};

static const struct net_device_ops qlcnic_netdev_failed_ops = {
	.ndo_open	   = qlcnic_open,
};

static struct qlcnic_nic_template qlcnic_ops = {
	.config_bridged_mode	= qlcnic_config_bridged_mode,
	.config_led		= qlcnic_82xx_config_led,
	.start_firmware		= qlcnic_82xx_start_firmware,
	.request_reset		= qlcnic_82xx_dev_request_reset,
	.cancel_idc_work	= qlcnic_82xx_cancel_idc_work,
	.napi_add		= qlcnic_82xx_napi_add,
	.napi_del		= qlcnic_82xx_napi_del,
	.config_ipaddr		= qlcnic_82xx_config_ipaddr,
	.shutdown		= qlcnic_82xx_shutdown,
	.resume			= qlcnic_82xx_resume,
	.clear_legacy_intr	= qlcnic_82xx_clear_legacy_intr,
};

struct qlcnic_nic_template qlcnic_vf_ops = {
	.config_bridged_mode	= qlcnicvf_config_bridged_mode,
	.config_led		= qlcnicvf_config_led,
	.start_firmware		= qlcnicvf_start_firmware
};

static struct qlcnic_hardware_ops qlcnic_hw_ops = {
	.read_crb			= qlcnic_82xx_read_crb,
	.write_crb			= qlcnic_82xx_write_crb,
	.read_reg			= qlcnic_82xx_hw_read_wx_2M,
	.write_reg			= qlcnic_82xx_hw_write_wx_2M,
	.get_mac_address		= qlcnic_82xx_get_mac_address,
	.setup_intr			= qlcnic_82xx_setup_intr,
	.alloc_mbx_args			= qlcnic_82xx_alloc_mbx_args,
	.mbx_cmd			= qlcnic_82xx_issue_cmd,
	.get_func_no			= qlcnic_82xx_get_func_no,
	.api_lock			= qlcnic_82xx_api_lock,
	.api_unlock			= qlcnic_82xx_api_unlock,
	.add_sysfs			= qlcnic_82xx_add_sysfs,
	.remove_sysfs			= qlcnic_82xx_remove_sysfs,
	.process_lb_rcv_ring_diag	= qlcnic_82xx_process_rcv_ring_diag,
	.create_rx_ctx			= qlcnic_82xx_fw_cmd_create_rx_ctx,
	.create_tx_ctx			= qlcnic_82xx_fw_cmd_create_tx_ctx,
	.del_rx_ctx			= qlcnic_82xx_fw_cmd_del_rx_ctx,
	.del_tx_ctx			= qlcnic_82xx_fw_cmd_del_tx_ctx,
	.setup_link_event		= qlcnic_82xx_linkevent_request,
	.get_nic_info			= qlcnic_82xx_get_nic_info,
	.get_pci_info			= qlcnic_82xx_get_pci_info,
	.set_nic_info			= qlcnic_82xx_set_nic_info,
	.change_macvlan			= qlcnic_82xx_sre_macaddr_change,
	.napi_enable			= qlcnic_82xx_napi_enable,
	.napi_disable			= qlcnic_82xx_napi_disable,
	.config_intr_coal		= qlcnic_82xx_config_intr_coalesce,
	.config_rss			= qlcnic_82xx_config_rss,
	.config_hw_lro			= qlcnic_82xx_config_hw_lro,
	.config_loopback		= qlcnic_82xx_set_lb_mode,
	.clear_loopback			= qlcnic_82xx_clear_lb_mode,
	.config_promisc_mode		= qlcnic_82xx_nic_set_promisc,
	.change_l2_filter		= qlcnic_82xx_change_filter,
	.get_board_info			= qlcnic_82xx_get_board_info,
	.set_mac_filter_count		= qlcnic_82xx_set_mac_filter_count,
	.free_mac_list			= qlcnic_82xx_free_mac_list,
	.read_phys_port_id		= qlcnic_82xx_read_phys_port_id,
	.io_error_detected		= qlcnic_82xx_io_error_detected,
	.io_slot_reset			= qlcnic_82xx_io_slot_reset,
	.io_resume			= qlcnic_82xx_io_resume,
};

static int qlcnic_check_multi_tx_capability(struct qlcnic_adapter *adapter)
{
	struct qlcnic_hardware_context *ahw = adapter->ahw;

	if (qlcnic_82xx_check(adapter) &&
	    (ahw->extra_capability[0] & QLCNIC_FW_CAPABILITY_2_MULTI_TX)) {
		test_and_set_bit(__QLCNIC_MULTI_TX_UNIQUE, &adapter->state);
		return 0;
	} else {
		return 1;
	}
}

static int qlcnic_max_rings(struct qlcnic_adapter *adapter, u8 ring_cnt,
			    int queue_type)
{
	int num_rings, max_rings = QLCNIC_MAX_SDS_RINGS;

	if (queue_type == QLCNIC_RX_QUEUE)
		max_rings = adapter->max_sds_rings;
	else if (queue_type == QLCNIC_TX_QUEUE)
		max_rings = adapter->max_tx_rings;

	num_rings = rounddown_pow_of_two(min_t(int, num_online_cpus(),
					      max_rings));

	if (ring_cnt > num_rings)
		return num_rings;
	else
		return ring_cnt;
}

void qlcnic_set_tx_ring_count(struct qlcnic_adapter *adapter, u8 tx_cnt)
{
	/* 83xx adapter does not have max_tx_rings intialized in probe */
	if (adapter->max_tx_rings)
		adapter->drv_tx_rings = qlcnic_max_rings(adapter, tx_cnt,
							 QLCNIC_TX_QUEUE);
	else
		adapter->drv_tx_rings = tx_cnt;

	dev_info(&adapter->pdev->dev, "Set %d Tx rings\n",
		 adapter->drv_tx_rings);
}

void qlcnic_set_sds_ring_count(struct qlcnic_adapter *adapter, u8 rx_cnt)
{
	/* 83xx adapter does not have max_sds_rings intialized in probe */
	if (adapter->max_sds_rings)
		adapter->drv_sds_rings = qlcnic_max_rings(adapter, rx_cnt,
							  QLCNIC_RX_QUEUE);
	else
		adapter->drv_sds_rings = rx_cnt;

	dev_info(&adapter->pdev->dev, "Set %d SDS rings\n",
		 adapter->drv_sds_rings);
}

int qlcnic_enable_msix(struct qlcnic_adapter *adapter, u32 num_msix)
{
	struct pci_dev *pdev = adapter->pdev;
	int drv_tx_rings, drv_sds_rings, tx_vector;
	int err = -1, i;

	if (adapter->flags & QLCNIC_TX_INTR_SHARED) {
		drv_tx_rings = 0;
		tx_vector = 0;
	} else {
		drv_tx_rings = adapter->drv_tx_rings;
		tx_vector = 1;
	}

	if (!adapter->msix_entries) {
		adapter->msix_entries = kcalloc(num_msix,
						sizeof(struct msix_entry),
						GFP_KERNEL);
		if (!adapter->msix_entries)
			return -ENOMEM;
	}

	adapter->drv_sds_rings = QLCNIC_SINGLE_RING;
	adapter->flags &= ~(QLCNIC_MSI_ENABLED | QLCNIC_MSIX_ENABLED);

	if (adapter->ahw->msix_supported) {
 enable_msix:
		for (i = 0; i < num_msix; i++)
			adapter->msix_entries[i].entry = i;
		err = pci_enable_msix(pdev, adapter->msix_entries, num_msix);
		if (err == 0) {
			adapter->flags |= QLCNIC_MSIX_ENABLED;
			if (qlcnic_83xx_check(adapter)) {
				adapter->ahw->num_msix = num_msix;
				/* subtract mail box and tx ring vectors */
				adapter->drv_sds_rings = num_msix -
							 drv_tx_rings - 1;
			} else {
				adapter->ahw->num_msix = num_msix;
				if (qlcnic_check_multi_tx(adapter) &&
				    !adapter->ahw->diag_test &&
				    (adapter->drv_tx_rings > 1))
					drv_sds_rings = num_msix - drv_tx_rings;
				else
					drv_sds_rings = num_msix;

				adapter->drv_sds_rings = drv_sds_rings;
			}
			dev_info(&pdev->dev, "using msi-x interrupts\n");
			return err;
		} else if (err > 0) {
			dev_info(&pdev->dev,
				 "Unable to allocate %d MSI-X interrupt vectors\n",
				 num_msix);
			if (qlcnic_83xx_check(adapter)) {
				if (err < (QLC_83XX_MINIMUM_VECTOR - tx_vector))
					return err;
				err -= drv_tx_rings + 1;
				num_msix = rounddown_pow_of_two(err);
				num_msix += drv_tx_rings + 1;
			} else {
				num_msix = rounddown_pow_of_two(err);
				if (qlcnic_check_multi_tx(adapter))
					num_msix += drv_tx_rings;
			}

			if (num_msix) {
				dev_info(&pdev->dev,
					 "Trying to allocate %d MSI-X interrupt vectors\n",
					 num_msix);
				goto enable_msix;
			}
		} else {
			dev_info(&pdev->dev,
				 "Unable to allocate %d MSI-X interrupt vectors\n",
				 num_msix);
		}
	}

	return err;
}

static int qlcnic_enable_msi_legacy(struct qlcnic_adapter *adapter)
{
	int err = 0;
	u32 offset, mask_reg;
	const struct qlcnic_legacy_intr_set *legacy_intrp;
	struct qlcnic_hardware_context *ahw = adapter->ahw;
	struct pci_dev *pdev = adapter->pdev;

	if (qlcnic_use_msi && !pci_enable_msi(pdev)) {
		adapter->flags |= QLCNIC_MSI_ENABLED;
		offset = msi_tgt_status[adapter->ahw->pci_func];
		adapter->tgt_status_reg = qlcnic_get_ioaddr(adapter->ahw,
							    offset);
		dev_info(&pdev->dev, "using msi interrupts\n");
		adapter->msix_entries[0].vector = pdev->irq;
		return err;
	}

	if (qlcnic_use_msi || qlcnic_use_msi_x)
		return -EOPNOTSUPP;

	legacy_intrp = &legacy_intr[adapter->ahw->pci_func];
	adapter->ahw->int_vec_bit = legacy_intrp->int_vec_bit;
	offset = legacy_intrp->tgt_status_reg;
	adapter->tgt_status_reg = qlcnic_get_ioaddr(ahw, offset);
	mask_reg = legacy_intrp->tgt_mask_reg;
	adapter->tgt_mask_reg = qlcnic_get_ioaddr(ahw, mask_reg);
	adapter->isr_int_vec = qlcnic_get_ioaddr(ahw, ISR_INT_VECTOR);
	adapter->crb_int_state_reg = qlcnic_get_ioaddr(ahw, ISR_INT_STATE_REG);
	dev_info(&pdev->dev, "using legacy interrupts\n");
	adapter->msix_entries[0].vector = pdev->irq;
	return err;
}

int qlcnic_82xx_setup_intr(struct qlcnic_adapter *adapter)
{
	int num_msix, err = 0;

	num_msix = adapter->drv_sds_rings;

	if (qlcnic_check_multi_tx(adapter))
		num_msix += adapter->drv_tx_rings;

	err = qlcnic_enable_msix(adapter, num_msix);
	if (err == -ENOMEM)
		return err;

	if (!(adapter->flags & QLCNIC_MSIX_ENABLED)) {
		qlcnic_disable_multi_tx(adapter);

		err = qlcnic_enable_msi_legacy(adapter);
		if (!err)
			return err;
	}

	return 0;
}

int qlcnic_82xx_mq_intrpt(struct qlcnic_adapter *adapter, int op_type)
{
	struct qlcnic_hardware_context *ahw = adapter->ahw;
	int err, i;

	if (qlcnic_check_multi_tx(adapter) &&
	    !ahw->diag_test &&
	    (adapter->flags & QLCNIC_MSIX_ENABLED)) {
		ahw->intr_tbl = vzalloc(ahw->num_msix *
					sizeof(struct qlcnic_intrpt_config));
		if (!ahw->intr_tbl)
			return -ENOMEM;

		for (i = 0; i < ahw->num_msix; i++) {
			ahw->intr_tbl[i].type = QLCNIC_INTRPT_MSIX;
			ahw->intr_tbl[i].id = i;
			ahw->intr_tbl[i].src = 0;
		}

		err = qlcnic_82xx_config_intrpt(adapter, 1);
		if (err)
			dev_err(&adapter->pdev->dev,
				"Failed to configure Interrupt for %d vector\n",
				ahw->num_msix);
		return err;
	}

	return 0;
}

void qlcnic_teardown_intr(struct qlcnic_adapter *adapter)
>>>>>>> refs/remotes/origin/master
{
	if (adapter->flags & QLCNIC_MSIX_ENABLED)
		pci_disable_msix(adapter->pdev);
	if (adapter->flags & QLCNIC_MSI_ENABLED)
		pci_disable_msi(adapter->pdev);
<<<<<<< HEAD
}

static void
qlcnic_cleanup_pci_map(struct qlcnic_adapter *adapter)
{
	if (adapter->ahw->pci_base0 != NULL)
		iounmap(adapter->ahw->pci_base0);
}

static int
qlcnic_init_pci_info(struct qlcnic_adapter *adapter)
{
	struct qlcnic_pci_info *pci_info;
	int i, ret = 0;
	u8 pfn;
=======

	kfree(adapter->msix_entries);
	adapter->msix_entries = NULL;

	if (adapter->ahw->intr_tbl) {
		vfree(adapter->ahw->intr_tbl);
		adapter->ahw->intr_tbl = NULL;
	}
}

static void qlcnic_cleanup_pci_map(struct qlcnic_hardware_context *ahw)
{
	if (ahw->pci_base0 != NULL)
		iounmap(ahw->pci_base0);
}

static int qlcnic_get_act_pci_func(struct qlcnic_adapter *adapter)
{
	struct qlcnic_pci_info *pci_info;
	int ret;

	if (!(adapter->flags & QLCNIC_ESWITCH_ENABLED)) {
		switch (adapter->ahw->port_type) {
		case QLCNIC_GBE:
			adapter->ahw->act_pci_func = QLCNIC_NIU_MAX_GBE_PORTS;
			break;
		case QLCNIC_XGBE:
			adapter->ahw->act_pci_func = QLCNIC_NIU_MAX_XG_PORTS;
			break;
		}
		return 0;
	}

	if (adapter->ahw->op_mode == QLCNIC_MGMT_FUNC)
		return 0;
>>>>>>> refs/remotes/origin/master

	pci_info = kcalloc(QLCNIC_MAX_PCI_FUNC, sizeof(*pci_info), GFP_KERNEL);
	if (!pci_info)
		return -ENOMEM;

<<<<<<< HEAD
	adapter->npars = kzalloc(sizeof(struct qlcnic_npar_info) *
				QLCNIC_MAX_PCI_FUNC, GFP_KERNEL);
	if (!adapter->npars) {
		ret = -ENOMEM;
		goto err_pci_info;
	}

	adapter->eswitch = kzalloc(sizeof(struct qlcnic_eswitch) *
=======
	ret = qlcnic_get_pci_info(adapter, pci_info);
	kfree(pci_info);
	return ret;
}

static bool qlcnic_port_eswitch_cfg_capability(struct qlcnic_adapter *adapter)
{
	bool ret = false;

	if (qlcnic_84xx_check(adapter)) {
		ret = true;
	} else if (qlcnic_83xx_check(adapter)) {
		if (adapter->ahw->extra_capability[0] &
		    QLCNIC_FW_CAPABILITY_2_PER_PORT_ESWITCH_CFG)
			ret = true;
		else
			ret = false;
	}

	return ret;
}

int qlcnic_init_pci_info(struct qlcnic_adapter *adapter)
{
	struct qlcnic_pci_info *pci_info;
	int i, id = 0, ret = 0, j = 0;
	u16 act_pci_func;
	u8 pfn;

	pci_info = kcalloc(QLCNIC_MAX_PCI_FUNC, sizeof(*pci_info), GFP_KERNEL);
	if (!pci_info)
		return -ENOMEM;

	ret = qlcnic_get_pci_info(adapter, pci_info);
	if (ret)
		goto err_pci_info;

	act_pci_func = adapter->ahw->act_pci_func;

	adapter->npars = kzalloc(sizeof(struct qlcnic_npar_info) *
				 act_pci_func, GFP_KERNEL);
	if (!adapter->npars) {
		ret = -ENOMEM;
		goto err_pci_info;
	}

	adapter->eswitch = kzalloc(sizeof(struct qlcnic_eswitch) *
>>>>>>> refs/remotes/origin/master
				QLCNIC_NIU_MAX_XG_PORTS, GFP_KERNEL);
	if (!adapter->eswitch) {
		ret = -ENOMEM;
		goto err_npars;
	}

<<<<<<< HEAD
	ret = qlcnic_get_pci_info(adapter, pci_info);
	if (ret)
		goto err_eswitch;

	for (i = 0; i < QLCNIC_MAX_PCI_FUNC; i++) {
		pfn = pci_info[i].id;
		if (pfn > QLCNIC_MAX_PCI_FUNC) {
			ret = QL_STATUS_INVALID_PARAM;
			goto err_eswitch;
		}
		adapter->npars[pfn].active = (u8)pci_info[i].active;
		adapter->npars[pfn].type = (u8)pci_info[i].type;
		adapter->npars[pfn].phy_port = (u8)pci_info[i].default_port;
		adapter->npars[pfn].min_bw = pci_info[i].tx_min_bw;
		adapter->npars[pfn].max_bw = pci_info[i].tx_max_bw;
	}

	for (i = 0; i < QLCNIC_NIU_MAX_XG_PORTS; i++)
		adapter->eswitch[i].flags |= QLCNIC_SWITCH_ENABLE;
=======
	for (i = 0; i < QLCNIC_MAX_PCI_FUNC; i++) {
		pfn = pci_info[i].id;

		if (pfn >= QLCNIC_MAX_PCI_FUNC) {
			ret = QL_STATUS_INVALID_PARAM;
			goto err_eswitch;
		}

		if (!pci_info[i].active ||
		    (pci_info[i].type != QLCNIC_TYPE_NIC))
			continue;

		if (qlcnic_port_eswitch_cfg_capability(adapter)) {
			if (!qlcnic_83xx_set_port_eswitch_status(adapter, pfn,
								 &id))
				adapter->npars[j].eswitch_status = true;
			else
				continue;
		} else {
			adapter->npars[j].eswitch_status = true;
		}

		adapter->npars[j].pci_func = pfn;
		adapter->npars[j].active = (u8)pci_info[i].active;
		adapter->npars[j].type = (u8)pci_info[i].type;
		adapter->npars[j].phy_port = (u8)pci_info[i].default_port;
		adapter->npars[j].min_bw = pci_info[i].tx_min_bw;
		adapter->npars[j].max_bw = pci_info[i].tx_max_bw;

		memcpy(&adapter->npars[j].mac, &pci_info[i].mac, ETH_ALEN);
		j++;
	}

	/* Update eSwitch status for adapters without per port eSwitch
	 * configuration capability
	 */
	if (!qlcnic_port_eswitch_cfg_capability(adapter)) {
		for (i = 0; i < QLCNIC_NIU_MAX_XG_PORTS; i++)
			adapter->eswitch[i].flags |= QLCNIC_SWITCH_ENABLE;
	}
>>>>>>> refs/remotes/origin/master

	kfree(pci_info);
	return 0;

err_eswitch:
	kfree(adapter->eswitch);
	adapter->eswitch = NULL;
err_npars:
	kfree(adapter->npars);
	adapter->npars = NULL;
err_pci_info:
	kfree(pci_info);

	return ret;
}

static int
qlcnic_set_function_modes(struct qlcnic_adapter *adapter)
{
	u8 id;
<<<<<<< HEAD
	u32 ref_count;
	int i, ret = 1;
	u32 data = QLCNIC_MGMT_FUNC;
	void __iomem *priv_op = adapter->ahw->pci_base0 + QLCNIC_DRV_OP_MODE;

	/* If other drivers are not in use set their privilege level */
	ref_count = QLCRD32(adapter, QLCNIC_CRB_DRV_ACTIVE);
=======
	int ret;
	u32 data = QLCNIC_MGMT_FUNC;
	struct qlcnic_hardware_context *ahw = adapter->ahw;

>>>>>>> refs/remotes/origin/master
	ret = qlcnic_api_lock(adapter);
	if (ret)
		goto err_lock;

<<<<<<< HEAD
	if (qlcnic_config_npars) {
		for (i = 0; i < QLCNIC_MAX_PCI_FUNC; i++) {
			id = i;
			if (adapter->npars[i].type != QLCNIC_TYPE_NIC ||
				id == adapter->ahw->pci_func)
				continue;
			data |= (qlcnic_config_npars &
					QLC_DEV_SET_DRV(0xf, id));
		}
	} else {
		data = readl(priv_op);
		data = (data & ~QLC_DEV_SET_DRV(0xf, adapter->ahw->pci_func)) |
			(QLC_DEV_SET_DRV(QLCNIC_MGMT_FUNC,
			adapter->ahw->pci_func));
	}
	writel(data, priv_op);
=======
	id = ahw->pci_func;
	data = QLC_SHARED_REG_RD32(adapter, QLCNIC_DRV_OP_MODE);
	data = (data & ~QLC_DEV_SET_DRV(0xf, id)) |
	       QLC_DEV_SET_DRV(QLCNIC_MGMT_FUNC, id);
	QLC_SHARED_REG_WR32(adapter, QLCNIC_DRV_OP_MODE, data);
>>>>>>> refs/remotes/origin/master
	qlcnic_api_unlock(adapter);
err_lock:
	return ret;
}

<<<<<<< HEAD
static void
qlcnic_check_vf(struct qlcnic_adapter *adapter)
{
	void __iomem *msix_base_addr;
	void __iomem *priv_op;
	u32 func;
	u32 msix_base;
	u32 op_mode, priv_level;

	/* Determine FW API version */
	adapter->fw_hal_version = readl(adapter->ahw->pci_base0 +
					QLCNIC_FW_API);

	/* Find PCI function number */
	pci_read_config_dword(adapter->pdev, QLCNIC_MSIX_TABLE_OFFSET, &func);
	msix_base_addr = adapter->ahw->pci_base0 + QLCNIC_MSIX_BASE;
	msix_base = readl(msix_base_addr);
	func = (func - msix_base)/QLCNIC_MSIX_TBL_PGSIZE;
	adapter->ahw->pci_func = func;

	/* Determine function privilege level */
	priv_op = adapter->ahw->pci_base0 + QLCNIC_DRV_OP_MODE;
	op_mode = readl(priv_op);
=======
static void qlcnic_check_vf(struct qlcnic_adapter *adapter,
			    const struct pci_device_id *ent)
{
	u32 op_mode, priv_level;

	/* Determine FW API version */
	adapter->ahw->fw_hal_version = QLC_SHARED_REG_RD32(adapter,
							   QLCNIC_FW_API);

	/* Find PCI function number */
	qlcnic_get_func_no(adapter);

	/* Determine function privilege level */
	op_mode = QLC_SHARED_REG_RD32(adapter, QLCNIC_DRV_OP_MODE);
>>>>>>> refs/remotes/origin/master
	if (op_mode == QLC_DEV_DRV_DEFAULT)
		priv_level = QLCNIC_MGMT_FUNC;
	else
		priv_level = QLC_DEV_GET_DRV(op_mode, adapter->ahw->pci_func);

	if (priv_level == QLCNIC_NON_PRIV_FUNC) {
<<<<<<< HEAD
		adapter->op_mode = QLCNIC_NON_PRIV_FUNC;
		dev_info(&adapter->pdev->dev,
			"HAL Version: %d Non Privileged function\n",
			adapter->fw_hal_version);
=======
		adapter->ahw->op_mode = QLCNIC_NON_PRIV_FUNC;
		dev_info(&adapter->pdev->dev,
			"HAL Version: %d Non Privileged function\n",
			 adapter->ahw->fw_hal_version);
>>>>>>> refs/remotes/origin/master
		adapter->nic_ops = &qlcnic_vf_ops;
	} else
		adapter->nic_ops = &qlcnic_ops;
}

<<<<<<< HEAD
static int
qlcnic_setup_pci_map(struct qlcnic_adapter *adapter)
{
	void __iomem *mem_ptr0 = NULL;
	resource_size_t mem_base;
	unsigned long mem_len, pci_len0 = 0;

	struct pci_dev *pdev = adapter->pdev;

	/* remap phys address */
	mem_base = pci_resource_start(pdev, 0);	/* 0 is for BAR 0 */
	mem_len = pci_resource_len(pdev, 0);

	if (mem_len == QLCNIC_PCI_2MB_SIZE) {
=======
#define QLCNIC_82XX_BAR0_LENGTH 0x00200000UL
#define QLCNIC_83XX_BAR0_LENGTH 0x4000
static void qlcnic_get_bar_length(u32 dev_id, ulong *bar)
{
	switch (dev_id) {
	case PCI_DEVICE_ID_QLOGIC_QLE824X:
		*bar = QLCNIC_82XX_BAR0_LENGTH;
		break;
	case PCI_DEVICE_ID_QLOGIC_QLE834X:
	case PCI_DEVICE_ID_QLOGIC_QLE844X:
	case PCI_DEVICE_ID_QLOGIC_VF_QLE834X:
	case PCI_DEVICE_ID_QLOGIC_VF_QLE844X:
		*bar = QLCNIC_83XX_BAR0_LENGTH;
		break;
	default:
		*bar = 0;
	}
}

static int qlcnic_setup_pci_map(struct pci_dev *pdev,
				struct qlcnic_hardware_context *ahw)
{
	u32 offset;
	void __iomem *mem_ptr0 = NULL;
	unsigned long mem_len, pci_len0 = 0, bar0_len;

	/* remap phys address */
	mem_len = pci_resource_len(pdev, 0);

	qlcnic_get_bar_length(pdev->device, &bar0_len);
	if (mem_len >= bar0_len) {
>>>>>>> refs/remotes/origin/master

		mem_ptr0 = pci_ioremap_bar(pdev, 0);
		if (mem_ptr0 == NULL) {
			dev_err(&pdev->dev, "failed to map PCI bar 0\n");
			return -EIO;
		}
		pci_len0 = mem_len;
	} else {
		return -EIO;
	}

<<<<<<< HEAD
	dev_info(&pdev->dev, "%dMB memory map\n", (int)(mem_len>>20));

	adapter->ahw->pci_base0 = mem_ptr0;
	adapter->ahw->pci_len0 = pci_len0;

	qlcnic_check_vf(adapter);

	adapter->ahw->ocm_win_crb = qlcnic_get_ioaddr(adapter,
		QLCNIC_PCIX_PS_REG(PCIX_OCM_WINDOW_REG(
			adapter->ahw->pci_func)));

	return 0;
}

static void get_brd_name(struct qlcnic_adapter *adapter, char *name)
=======
	dev_info(&pdev->dev, "%dKB memory map\n", (int)(mem_len >> 10));

	ahw->pci_base0 = mem_ptr0;
	ahw->pci_len0 = pci_len0;
	offset = QLCNIC_PCIX_PS_REG(PCIX_OCM_WINDOW_REG(ahw->pci_func));
	qlcnic_get_ioaddr(ahw, offset);

	return 0;
}

static bool qlcnic_validate_subsystem_id(struct qlcnic_adapter *adapter,
					 int index)
{
	struct pci_dev *pdev = adapter->pdev;
	unsigned short subsystem_vendor;
	bool ret = true;

	subsystem_vendor = pdev->subsystem_vendor;

	if (pdev->device == PCI_DEVICE_ID_QLOGIC_QLE824X ||
	    pdev->device == PCI_DEVICE_ID_QLOGIC_QLE834X) {
		if (qlcnic_boards[index].sub_vendor == subsystem_vendor &&
		    qlcnic_boards[index].sub_device == pdev->subsystem_device)
			ret = true;
		else
			ret = false;
	}

	return ret;
}

static void qlcnic_get_board_name(struct qlcnic_adapter *adapter, char *name)
>>>>>>> refs/remotes/origin/master
{
	struct pci_dev *pdev = adapter->pdev;
	int i, found = 0;

	for (i = 0; i < NUM_SUPPORTED_BOARDS; ++i) {
		if (qlcnic_boards[i].vendor == pdev->vendor &&
<<<<<<< HEAD
			qlcnic_boards[i].device == pdev->device &&
			qlcnic_boards[i].sub_vendor == pdev->subsystem_vendor &&
			qlcnic_boards[i].sub_device == pdev->subsystem_device) {
				sprintf(name, "%pM: %s" ,
					adapter->mac_addr,
					qlcnic_boards[i].short_name);
				found = 1;
				break;
		}

=======
		    qlcnic_boards[i].device == pdev->device &&
		    qlcnic_validate_subsystem_id(adapter, i)) {
			found = 1;
			break;
		}
>>>>>>> refs/remotes/origin/master
	}

	if (!found)
		sprintf(name, "%pM Gigabit Ethernet", adapter->mac_addr);
<<<<<<< HEAD
=======
	else
		sprintf(name, "%pM: %s" , adapter->mac_addr,
			qlcnic_boards[i].short_name);
>>>>>>> refs/remotes/origin/master
}

static void
qlcnic_check_options(struct qlcnic_adapter *adapter)
{
<<<<<<< HEAD
	u32 fw_major, fw_minor, fw_build, prev_fw_version;
	struct pci_dev *pdev = adapter->pdev;
	struct qlcnic_fw_dump *fw_dump = &adapter->ahw->fw_dump;

	prev_fw_version = adapter->fw_version;

	fw_major = QLCRD32(adapter, QLCNIC_FW_VERSION_MAJOR);
	fw_minor = QLCRD32(adapter, QLCNIC_FW_VERSION_MINOR);
	fw_build = QLCRD32(adapter, QLCNIC_FW_VERSION_SUB);

	adapter->fw_version = QLCNIC_VERSION_CODE(fw_major, fw_minor, fw_build);

	if (adapter->op_mode != QLCNIC_NON_PRIV_FUNC) {
=======
	int err;
	u32 fw_major, fw_minor, fw_build, prev_fw_version;
	struct pci_dev *pdev = adapter->pdev;
	struct qlcnic_hardware_context *ahw = adapter->ahw;
	struct qlcnic_fw_dump *fw_dump = &ahw->fw_dump;

	prev_fw_version = adapter->fw_version;

	fw_major = QLC_SHARED_REG_RD32(adapter, QLCNIC_FW_VERSION_MAJOR);
	fw_minor = QLC_SHARED_REG_RD32(adapter, QLCNIC_FW_VERSION_MINOR);
	fw_build = QLC_SHARED_REG_RD32(adapter, QLCNIC_FW_VERSION_SUB);

	adapter->fw_version = QLCNIC_VERSION_CODE(fw_major, fw_minor, fw_build);

	err = qlcnic_get_board_info(adapter);
	if (err) {
		dev_err(&pdev->dev, "Error getting board config info.\n");
		return;
	}
	if (ahw->op_mode != QLCNIC_NON_PRIV_FUNC) {
>>>>>>> refs/remotes/origin/master
		if (fw_dump->tmpl_hdr == NULL ||
				adapter->fw_version > prev_fw_version) {
			if (fw_dump->tmpl_hdr)
				vfree(fw_dump->tmpl_hdr);
			if (!qlcnic_fw_cmd_get_minidump_temp(adapter))
				dev_info(&pdev->dev,
					"Supports FW dump capability\n");
		}
	}

<<<<<<< HEAD
	dev_info(&pdev->dev, "firmware v%d.%d.%d\n",
			fw_major, fw_minor, fw_build);
=======
	dev_info(&pdev->dev, "Driver v%s, firmware v%d.%d.%d\n",
		 QLCNIC_LINUX_VERSIONID, fw_major, fw_minor, fw_build);

>>>>>>> refs/remotes/origin/master
	if (adapter->ahw->port_type == QLCNIC_XGBE) {
		if (adapter->flags & QLCNIC_ESWITCH_ENABLED) {
			adapter->num_rxd = DEFAULT_RCV_DESCRIPTORS_VF;
			adapter->max_rxd = MAX_RCV_DESCRIPTORS_VF;
		} else {
			adapter->num_rxd = DEFAULT_RCV_DESCRIPTORS_10G;
			adapter->max_rxd = MAX_RCV_DESCRIPTORS_10G;
		}

		adapter->num_jumbo_rxd = MAX_JUMBO_RCV_DESCRIPTORS_10G;
		adapter->max_jumbo_rxd = MAX_JUMBO_RCV_DESCRIPTORS_10G;

	} else if (adapter->ahw->port_type == QLCNIC_GBE) {
		adapter->num_rxd = DEFAULT_RCV_DESCRIPTORS_1G;
		adapter->num_jumbo_rxd = MAX_JUMBO_RCV_DESCRIPTORS_1G;
		adapter->max_jumbo_rxd = MAX_JUMBO_RCV_DESCRIPTORS_1G;
		adapter->max_rxd = MAX_RCV_DESCRIPTORS_1G;
	}

<<<<<<< HEAD
	adapter->msix_supported = !!use_msi_x;
=======
	adapter->ahw->msix_supported = !!qlcnic_use_msi_x;
>>>>>>> refs/remotes/origin/master

	adapter->num_txd = MAX_CMD_DESCRIPTORS;

	adapter->max_rds_rings = MAX_RDS_RINGS;
}

static int
qlcnic_initialize_nic(struct qlcnic_adapter *adapter)
{
<<<<<<< HEAD
	int err;
	struct qlcnic_info nic_info;

=======
	struct qlcnic_info nic_info;
	int err = 0;

	memset(&nic_info, 0, sizeof(struct qlcnic_info));
>>>>>>> refs/remotes/origin/master
	err = qlcnic_get_nic_info(adapter, &nic_info, adapter->ahw->pci_func);
	if (err)
		return err;

<<<<<<< HEAD
	adapter->physical_port = (u8)nic_info.phys_port;
	adapter->switch_mode = nic_info.switch_mode;
	adapter->max_tx_ques = nic_info.max_tx_ques;
	adapter->max_rx_ques = nic_info.max_rx_ques;
	adapter->capabilities = nic_info.capabilities;
	adapter->max_mac_filters = nic_info.max_mac_filters;
	adapter->max_mtu = nic_info.max_mtu;

	if (adapter->capabilities & BIT_6)
		adapter->flags |= QLCNIC_ESWITCH_ENABLED;
	else
		adapter->flags &= ~QLCNIC_ESWITCH_ENABLED;
=======
	adapter->ahw->physical_port = (u8)nic_info.phys_port;
	adapter->ahw->switch_mode = nic_info.switch_mode;
	adapter->ahw->max_tx_ques = nic_info.max_tx_ques;
	adapter->ahw->max_rx_ques = nic_info.max_rx_ques;
	adapter->ahw->capabilities = nic_info.capabilities;

	if (adapter->ahw->capabilities & QLCNIC_FW_CAPABILITY_MORE_CAPS) {
		u32 temp;
		temp = QLCRD32(adapter, CRB_FW_CAPABILITIES_2, &err);
		if (err == -EIO)
			return err;
		adapter->ahw->extra_capability[0] = temp;
	} else {
		adapter->ahw->extra_capability[0] = 0;
	}

	adapter->ahw->max_mac_filters = nic_info.max_mac_filters;
	adapter->ahw->max_mtu = nic_info.max_mtu;

	if (adapter->ahw->capabilities & BIT_6) {
		adapter->flags |= QLCNIC_ESWITCH_ENABLED;
		adapter->ahw->nic_mode = QLCNIC_VNIC_MODE;
		adapter->max_tx_rings = QLCNIC_MAX_HW_VNIC_TX_RINGS;
		adapter->max_sds_rings = QLCNIC_MAX_VNIC_SDS_RINGS;

		dev_info(&adapter->pdev->dev, "vNIC mode enabled.\n");
	} else {
		adapter->ahw->nic_mode = QLCNIC_DEFAULT_MODE;
		adapter->max_tx_rings = QLCNIC_MAX_HW_TX_RINGS;
		adapter->max_sds_rings = QLCNIC_MAX_SDS_RINGS;
		adapter->flags &= ~QLCNIC_ESWITCH_ENABLED;
	}
>>>>>>> refs/remotes/origin/master

	return err;
}

<<<<<<< HEAD
static void
qlcnic_set_vlan_config(struct qlcnic_adapter *adapter,
		struct qlcnic_esw_func_cfg *esw_cfg)
=======
void qlcnic_set_vlan_config(struct qlcnic_adapter *adapter,
			    struct qlcnic_esw_func_cfg *esw_cfg)
>>>>>>> refs/remotes/origin/master
{
	if (esw_cfg->discard_tagged)
		adapter->flags &= ~QLCNIC_TAGGING_ENABLED;
	else
		adapter->flags |= QLCNIC_TAGGING_ENABLED;

<<<<<<< HEAD
	if (esw_cfg->vlan_id)
		adapter->pvid = esw_cfg->vlan_id;
	else
		adapter->pvid = 0;
}

static int
qlcnic_vlan_rx_add(struct net_device *netdev, u16 vid)
{
	struct qlcnic_adapter *adapter = netdev_priv(netdev);
=======
	if (esw_cfg->vlan_id) {
		adapter->rx_pvid = esw_cfg->vlan_id;
		adapter->tx_pvid = esw_cfg->vlan_id;
	} else {
		adapter->rx_pvid = 0;
		adapter->tx_pvid = 0;
	}
}

static int
qlcnic_vlan_rx_add(struct net_device *netdev, __be16 proto, u16 vid)
{
	struct qlcnic_adapter *adapter = netdev_priv(netdev);
	int err;

	if (qlcnic_sriov_vf_check(adapter)) {
		err = qlcnic_sriov_cfg_vf_guest_vlan(adapter, vid, 1);
		if (err) {
			netdev_err(netdev,
				   "Cannot add VLAN filter for VLAN id %d, err=%d",
				   vid, err);
			return err;
		}
	}

>>>>>>> refs/remotes/origin/master
	set_bit(vid, adapter->vlans);
	return 0;
}

static int
<<<<<<< HEAD
qlcnic_vlan_rx_del(struct net_device *netdev, u16 vid)
{
	struct qlcnic_adapter *adapter = netdev_priv(netdev);
=======
qlcnic_vlan_rx_del(struct net_device *netdev, __be16 proto, u16 vid)
{
	struct qlcnic_adapter *adapter = netdev_priv(netdev);
	int err;

	if (qlcnic_sriov_vf_check(adapter)) {
		err = qlcnic_sriov_cfg_vf_guest_vlan(adapter, vid, 0);
		if (err) {
			netdev_err(netdev,
				   "Cannot delete VLAN filter for VLAN id %d, err=%d",
				   vid, err);
			return err;
		}
	}
>>>>>>> refs/remotes/origin/master

	qlcnic_restore_indev_addr(netdev, NETDEV_DOWN);
	clear_bit(vid, adapter->vlans);
	return 0;
}

<<<<<<< HEAD
static void
qlcnic_set_eswitch_port_features(struct qlcnic_adapter *adapter,
		struct qlcnic_esw_func_cfg *esw_cfg)
=======
void qlcnic_set_eswitch_port_features(struct qlcnic_adapter *adapter,
				      struct qlcnic_esw_func_cfg *esw_cfg)
>>>>>>> refs/remotes/origin/master
{
	adapter->flags &= ~(QLCNIC_MACSPOOF | QLCNIC_MAC_OVERRIDE_DISABLED |
				QLCNIC_PROMISC_DISABLED);

	if (esw_cfg->mac_anti_spoof)
		adapter->flags |= QLCNIC_MACSPOOF;

	if (!esw_cfg->mac_override)
		adapter->flags |= QLCNIC_MAC_OVERRIDE_DISABLED;

	if (!esw_cfg->promisc_mode)
		adapter->flags |= QLCNIC_PROMISC_DISABLED;
<<<<<<< HEAD

	qlcnic_set_netdev_features(adapter, esw_cfg);
}

static int
qlcnic_set_eswitch_port_config(struct qlcnic_adapter *adapter)
=======
}

int qlcnic_set_eswitch_port_config(struct qlcnic_adapter *adapter)
>>>>>>> refs/remotes/origin/master
{
	struct qlcnic_esw_func_cfg esw_cfg;

	if (!(adapter->flags & QLCNIC_ESWITCH_ENABLED))
		return 0;

	esw_cfg.pci_func = adapter->ahw->pci_func;
	if (qlcnic_get_eswitch_port_config(adapter, &esw_cfg))
			return -EIO;
	qlcnic_set_vlan_config(adapter, &esw_cfg);
	qlcnic_set_eswitch_port_features(adapter, &esw_cfg);
<<<<<<< HEAD
=======
	qlcnic_set_netdev_features(adapter, &esw_cfg);
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
static void
qlcnic_set_netdev_features(struct qlcnic_adapter *adapter,
		struct qlcnic_esw_func_cfg *esw_cfg)
{
	struct net_device *netdev = adapter->netdev;
	netdev_features_t features, vlan_features;

	features = (NETIF_F_SG | NETIF_F_IP_CSUM | NETIF_F_RXCSUM |
			NETIF_F_IPV6_CSUM | NETIF_F_GRO);
	vlan_features = (NETIF_F_SG | NETIF_F_IP_CSUM |
			NETIF_F_IPV6_CSUM | NETIF_F_HW_VLAN_FILTER);

	if (adapter->capabilities & QLCNIC_FW_CAPABILITY_TSO) {
		features |= (NETIF_F_TSO | NETIF_F_TSO6);
		vlan_features |= (NETIF_F_TSO | NETIF_F_TSO6);
	}

	if (netdev->features & NETIF_F_LRO)
		features |= NETIF_F_LRO;

	if (esw_cfg->offload_flags & BIT_0) {
		netdev->features |= features;
		if (!(esw_cfg->offload_flags & BIT_1))
			netdev->features &= ~NETIF_F_TSO;
		if (!(esw_cfg->offload_flags & BIT_2))
			netdev->features &= ~NETIF_F_TSO6;
	} else {
		netdev->features &= ~features;
	}

	netdev->vlan_features = (features & vlan_features);
=======
void qlcnic_set_netdev_features(struct qlcnic_adapter *adapter,
				struct qlcnic_esw_func_cfg *esw_cfg)
{
	struct net_device *netdev = adapter->netdev;

	if (qlcnic_83xx_check(adapter))
		return;

	adapter->offload_flags = esw_cfg->offload_flags;
	adapter->flags |= QLCNIC_APP_CHANGED_FLAGS;
	netdev_update_features(netdev);
	adapter->flags &= ~QLCNIC_APP_CHANGED_FLAGS;
>>>>>>> refs/remotes/origin/master
}

static int
qlcnic_check_eswitch_mode(struct qlcnic_adapter *adapter)
{
<<<<<<< HEAD
	void __iomem *priv_op;
=======
>>>>>>> refs/remotes/origin/master
	u32 op_mode, priv_level;
	int err = 0;

	err = qlcnic_initialize_nic(adapter);
	if (err)
		return err;

	if (adapter->flags & QLCNIC_ADAPTER_INITIALIZED)
		return 0;

<<<<<<< HEAD
	priv_op = adapter->ahw->pci_base0 + QLCNIC_DRV_OP_MODE;
	op_mode = readl(priv_op);
=======
	op_mode = QLC_SHARED_REG_RD32(adapter, QLCNIC_DRV_OP_MODE);
>>>>>>> refs/remotes/origin/master
	priv_level = QLC_DEV_GET_DRV(op_mode, adapter->ahw->pci_func);

	if (op_mode == QLC_DEV_DRV_DEFAULT)
		priv_level = QLCNIC_MGMT_FUNC;
	else
		priv_level = QLC_DEV_GET_DRV(op_mode, adapter->ahw->pci_func);

	if (adapter->flags & QLCNIC_ESWITCH_ENABLED) {
		if (priv_level == QLCNIC_MGMT_FUNC) {
<<<<<<< HEAD
			adapter->op_mode = QLCNIC_MGMT_FUNC;
=======
			adapter->ahw->op_mode = QLCNIC_MGMT_FUNC;
>>>>>>> refs/remotes/origin/master
			err = qlcnic_init_pci_info(adapter);
			if (err)
				return err;
			/* Set privilege level for other functions */
			qlcnic_set_function_modes(adapter);
			dev_info(&adapter->pdev->dev,
				"HAL Version: %d, Management function\n",
<<<<<<< HEAD
				adapter->fw_hal_version);
		} else if (priv_level == QLCNIC_PRIV_FUNC) {
			adapter->op_mode = QLCNIC_PRIV_FUNC;
			dev_info(&adapter->pdev->dev,
				"HAL Version: %d, Privileged function\n",
				adapter->fw_hal_version);
		}
=======
				 adapter->ahw->fw_hal_version);
		} else if (priv_level == QLCNIC_PRIV_FUNC) {
			adapter->ahw->op_mode = QLCNIC_PRIV_FUNC;
			dev_info(&adapter->pdev->dev,
				"HAL Version: %d, Privileged function\n",
				 adapter->ahw->fw_hal_version);
		}
	} else {
		adapter->ahw->nic_mode = QLCNIC_DEFAULT_MODE;
>>>>>>> refs/remotes/origin/master
	}

	adapter->flags |= QLCNIC_ADAPTER_INITIALIZED;

	return err;
}

<<<<<<< HEAD
static int
qlcnic_set_default_offload_settings(struct qlcnic_adapter *adapter)
=======
int qlcnic_set_default_offload_settings(struct qlcnic_adapter *adapter)
>>>>>>> refs/remotes/origin/master
{
	struct qlcnic_esw_func_cfg esw_cfg;
	struct qlcnic_npar_info *npar;
	u8 i;

	if (adapter->need_fw_reset)
		return 0;

<<<<<<< HEAD
	for (i = 0; i < QLCNIC_MAX_PCI_FUNC; i++) {
		if (adapter->npars[i].type != QLCNIC_TYPE_NIC)
			continue;
		memset(&esw_cfg, 0, sizeof(struct qlcnic_esw_func_cfg));
		esw_cfg.pci_func = i;
		esw_cfg.offload_flags = BIT_0;
		esw_cfg.mac_override = BIT_0;
		esw_cfg.promisc_mode = BIT_0;
		if (adapter->capabilities  & QLCNIC_FW_CAPABILITY_TSO)
			esw_cfg.offload_flags |= (BIT_1 | BIT_2);
=======
	for (i = 0; i < adapter->ahw->act_pci_func; i++) {
		if (!adapter->npars[i].eswitch_status)
			continue;

		memset(&esw_cfg, 0, sizeof(struct qlcnic_esw_func_cfg));
		esw_cfg.pci_func = adapter->npars[i].pci_func;
		esw_cfg.mac_override = BIT_0;
		esw_cfg.promisc_mode = BIT_0;
		if (qlcnic_82xx_check(adapter)) {
			esw_cfg.offload_flags = BIT_0;
			if (QLCNIC_IS_TSO_CAPABLE(adapter))
				esw_cfg.offload_flags |= (BIT_1 | BIT_2);
		}
>>>>>>> refs/remotes/origin/master
		if (qlcnic_config_switch_port(adapter, &esw_cfg))
			return -EIO;
		npar = &adapter->npars[i];
		npar->pvid = esw_cfg.vlan_id;
		npar->mac_override = esw_cfg.mac_override;
		npar->mac_anti_spoof = esw_cfg.mac_anti_spoof;
		npar->discard_tagged = esw_cfg.discard_tagged;
		npar->promisc_mode = esw_cfg.promisc_mode;
		npar->offload_flags = esw_cfg.offload_flags;
	}

	return 0;
}

<<<<<<< HEAD
=======

>>>>>>> refs/remotes/origin/master
static int
qlcnic_reset_eswitch_config(struct qlcnic_adapter *adapter,
			struct qlcnic_npar_info *npar, int pci_func)
{
	struct qlcnic_esw_func_cfg esw_cfg;
	esw_cfg.op_mode = QLCNIC_PORT_DEFAULTS;
	esw_cfg.pci_func = pci_func;
	esw_cfg.vlan_id = npar->pvid;
	esw_cfg.mac_override = npar->mac_override;
	esw_cfg.discard_tagged = npar->discard_tagged;
	esw_cfg.mac_anti_spoof = npar->mac_anti_spoof;
	esw_cfg.offload_flags = npar->offload_flags;
	esw_cfg.promisc_mode = npar->promisc_mode;
	if (qlcnic_config_switch_port(adapter, &esw_cfg))
		return -EIO;

	esw_cfg.op_mode = QLCNIC_ADD_VLAN;
	if (qlcnic_config_switch_port(adapter, &esw_cfg))
		return -EIO;

	return 0;
}

<<<<<<< HEAD
static int
qlcnic_reset_npar_config(struct qlcnic_adapter *adapter)
=======
int qlcnic_reset_npar_config(struct qlcnic_adapter *adapter)
>>>>>>> refs/remotes/origin/master
{
	int i, err;
	struct qlcnic_npar_info *npar;
	struct qlcnic_info nic_info;
<<<<<<< HEAD

	if (!adapter->need_fw_reset)
		return 0;

	/* Set the NPAR config data after FW reset */
	for (i = 0; i < QLCNIC_MAX_PCI_FUNC; i++) {
		npar = &adapter->npars[i];
		if (npar->type != QLCNIC_TYPE_NIC)
			continue;
		err = qlcnic_get_nic_info(adapter, &nic_info, i);
=======
	u8 pci_func;

	if (qlcnic_82xx_check(adapter))
		if (!adapter->need_fw_reset)
			return 0;

	/* Set the NPAR config data after FW reset */
	for (i = 0; i < adapter->ahw->act_pci_func; i++) {
		npar = &adapter->npars[i];
		pci_func = npar->pci_func;
		if (!adapter->npars[i].eswitch_status)
			continue;

		memset(&nic_info, 0, sizeof(struct qlcnic_info));
		err = qlcnic_get_nic_info(adapter, &nic_info, pci_func);
>>>>>>> refs/remotes/origin/master
		if (err)
			return err;
		nic_info.min_tx_bw = npar->min_bw;
		nic_info.max_tx_bw = npar->max_bw;
		err = qlcnic_set_nic_info(adapter, &nic_info);
		if (err)
			return err;

		if (npar->enable_pm) {
			err = qlcnic_config_port_mirroring(adapter,
<<<<<<< HEAD
							npar->dest_npar, 1, i);
			if (err)
				return err;
		}
		err = qlcnic_reset_eswitch_config(adapter, npar, i);
=======
							   npar->dest_npar, 1,
							   pci_func);
			if (err)
				return err;
		}
		err = qlcnic_reset_eswitch_config(adapter, npar, pci_func);
>>>>>>> refs/remotes/origin/master
		if (err)
			return err;
	}
	return 0;
}

static int qlcnic_check_npar_opertional(struct qlcnic_adapter *adapter)
{
	u8 npar_opt_timeo = QLCNIC_DEV_NPAR_OPER_TIMEO;
	u32 npar_state;

<<<<<<< HEAD
	if (adapter->op_mode == QLCNIC_MGMT_FUNC)
		return 0;

	npar_state = QLCRD32(adapter, QLCNIC_CRB_DEV_NPAR_STATE);
	while (npar_state != QLCNIC_DEV_NPAR_OPER && --npar_opt_timeo) {
		msleep(1000);
		npar_state = QLCRD32(adapter, QLCNIC_CRB_DEV_NPAR_STATE);
	}
	if (!npar_opt_timeo) {
		dev_err(&adapter->pdev->dev,
			"Waiting for NPAR state to opertional timeout\n");
=======
	if (adapter->ahw->op_mode == QLCNIC_MGMT_FUNC)
		return 0;

	npar_state = QLC_SHARED_REG_RD32(adapter,
					 QLCNIC_CRB_DEV_NPAR_STATE);
	while (npar_state != QLCNIC_DEV_NPAR_OPER && --npar_opt_timeo) {
		msleep(1000);
		npar_state = QLC_SHARED_REG_RD32(adapter,
						 QLCNIC_CRB_DEV_NPAR_STATE);
	}
	if (!npar_opt_timeo) {
		dev_err(&adapter->pdev->dev,
			"Waiting for NPAR state to operational timeout\n");
>>>>>>> refs/remotes/origin/master
		return -EIO;
	}
	return 0;
}

static int
qlcnic_set_mgmt_operations(struct qlcnic_adapter *adapter)
{
	int err;

	if (!(adapter->flags & QLCNIC_ESWITCH_ENABLED) ||
<<<<<<< HEAD
		    adapter->op_mode != QLCNIC_MGMT_FUNC)
=======
	    adapter->ahw->op_mode != QLCNIC_MGMT_FUNC)
>>>>>>> refs/remotes/origin/master
		return 0;

	err = qlcnic_set_default_offload_settings(adapter);
	if (err)
		return err;

	err = qlcnic_reset_npar_config(adapter);
	if (err)
		return err;

	qlcnic_dev_set_npar_ready(adapter);

	return err;
}

<<<<<<< HEAD
static int
qlcnic_start_firmware(struct qlcnic_adapter *adapter)
=======
int qlcnic_82xx_start_firmware(struct qlcnic_adapter *adapter)
>>>>>>> refs/remotes/origin/master
{
	int err;

	err = qlcnic_can_start_firmware(adapter);
	if (err < 0)
		return err;
	else if (!err)
		goto check_fw_status;

<<<<<<< HEAD
	if (load_fw_file)
=======
	if (qlcnic_load_fw_file)
>>>>>>> refs/remotes/origin/master
		qlcnic_request_firmware(adapter);
	else {
		err = qlcnic_check_flash_fw_ver(adapter);
		if (err)
			goto err_out;

<<<<<<< HEAD
		adapter->fw_type = QLCNIC_FLASH_ROMIMAGE;
=======
		adapter->ahw->fw_type = QLCNIC_FLASH_ROMIMAGE;
>>>>>>> refs/remotes/origin/master
	}

	err = qlcnic_need_fw_reset(adapter);
	if (err == 0)
		goto check_fw_status;

	err = qlcnic_pinit_from_rom(adapter);
	if (err)
		goto err_out;

	err = qlcnic_load_firmware(adapter);
	if (err)
		goto err_out;

	qlcnic_release_firmware(adapter);
	QLCWR32(adapter, CRB_DRIVER_VERSION, QLCNIC_DRIVER_VERSION);

check_fw_status:
	err = qlcnic_check_fw_status(adapter);
	if (err)
		goto err_out;

<<<<<<< HEAD
	QLCWR32(adapter, QLCNIC_CRB_DEV_STATE, QLCNIC_DEV_READY);
	qlcnic_idc_debug_info(adapter, 1);

=======
	QLC_SHARED_REG_WR32(adapter, QLCNIC_CRB_DEV_STATE, QLCNIC_DEV_READY);
	qlcnic_idc_debug_info(adapter, 1);
>>>>>>> refs/remotes/origin/master
	err = qlcnic_check_eswitch_mode(adapter);
	if (err) {
		dev_err(&adapter->pdev->dev,
			"Memory allocation failed for eswitch\n");
		goto err_out;
	}
	err = qlcnic_set_mgmt_operations(adapter);
	if (err)
		goto err_out;

	qlcnic_check_options(adapter);
	adapter->need_fw_reset = 0;

	qlcnic_release_firmware(adapter);
	return 0;

err_out:
<<<<<<< HEAD
	QLCWR32(adapter, QLCNIC_CRB_DEV_STATE, QLCNIC_DEV_FAILED);
=======
	QLC_SHARED_REG_WR32(adapter, QLCNIC_CRB_DEV_STATE, QLCNIC_DEV_FAILED);
>>>>>>> refs/remotes/origin/master
	dev_err(&adapter->pdev->dev, "Device state set to failed\n");

	qlcnic_release_firmware(adapter);
	return err;
}

static int
qlcnic_request_irq(struct qlcnic_adapter *adapter)
{
	irq_handler_t handler;
	struct qlcnic_host_sds_ring *sds_ring;
<<<<<<< HEAD
	int err, ring;
=======
	struct qlcnic_host_tx_ring *tx_ring;
	int err, ring, num_sds_rings;
>>>>>>> refs/remotes/origin/master

	unsigned long flags = 0;
	struct net_device *netdev = adapter->netdev;
	struct qlcnic_recv_context *recv_ctx = adapter->recv_ctx;

<<<<<<< HEAD
	if (adapter->diag_test == QLCNIC_INTERRUPT_TEST) {
		handler = qlcnic_tmp_intr;
=======
	if (adapter->ahw->diag_test == QLCNIC_INTERRUPT_TEST) {
		if (qlcnic_82xx_check(adapter))
			handler = qlcnic_tmp_intr;
		else
			handler = qlcnic_83xx_tmp_intr;
>>>>>>> refs/remotes/origin/master
		if (!QLCNIC_IS_MSI_FAMILY(adapter))
			flags |= IRQF_SHARED;

	} else {
		if (adapter->flags & QLCNIC_MSIX_ENABLED)
			handler = qlcnic_msix_intr;
		else if (adapter->flags & QLCNIC_MSI_ENABLED)
			handler = qlcnic_msi_intr;
		else {
			flags |= IRQF_SHARED;
<<<<<<< HEAD
			handler = qlcnic_intr;
=======
			if (qlcnic_82xx_check(adapter))
				handler = qlcnic_intr;
			else
				handler = qlcnic_83xx_intr;
>>>>>>> refs/remotes/origin/master
		}
	}
	adapter->irq = netdev->irq;

<<<<<<< HEAD
	for (ring = 0; ring < adapter->max_sds_rings; ring++) {
		sds_ring = &recv_ctx->sds_rings[ring];
		sprintf(sds_ring->name, "%s[%d]", netdev->name, ring);
		err = request_irq(sds_ring->irq, handler,
				  flags, sds_ring->name, sds_ring);
		if (err)
			return err;
	}

=======
	if (adapter->ahw->diag_test != QLCNIC_LOOPBACK_TEST) {
		if (qlcnic_82xx_check(adapter) ||
		    (qlcnic_83xx_check(adapter) &&
		     (adapter->flags & QLCNIC_MSIX_ENABLED))) {
			num_sds_rings = adapter->drv_sds_rings;
			for (ring = 0; ring < num_sds_rings; ring++) {
				sds_ring = &recv_ctx->sds_rings[ring];
				if (qlcnic_82xx_check(adapter) &&
				    !qlcnic_check_multi_tx(adapter) &&
				    (ring == (num_sds_rings - 1))) {
					if (!(adapter->flags &
					      QLCNIC_MSIX_ENABLED))
						snprintf(sds_ring->name,
							 sizeof(sds_ring->name),
							 "qlcnic");
					else
						snprintf(sds_ring->name,
							 sizeof(sds_ring->name),
							 "%s-tx-0-rx-%d",
							 netdev->name, ring);
				} else {
					snprintf(sds_ring->name,
						 sizeof(sds_ring->name),
						 "%s-rx-%d",
						 netdev->name, ring);
				}
				err = request_irq(sds_ring->irq, handler, flags,
						  sds_ring->name, sds_ring);
				if (err)
					return err;
			}
		}
		if ((qlcnic_82xx_check(adapter) &&
		     qlcnic_check_multi_tx(adapter)) ||
		    (qlcnic_83xx_check(adapter) &&
		     (adapter->flags & QLCNIC_MSIX_ENABLED) &&
		     !(adapter->flags & QLCNIC_TX_INTR_SHARED))) {
			handler = qlcnic_msix_tx_intr;
			for (ring = 0; ring < adapter->drv_tx_rings;
			     ring++) {
				tx_ring = &adapter->tx_ring[ring];
				snprintf(tx_ring->name, sizeof(tx_ring->name),
					 "%s-tx-%d", netdev->name, ring);
				err = request_irq(tx_ring->irq, handler, flags,
						  tx_ring->name, tx_ring);
				if (err)
					return err;
			}
		}
	}
>>>>>>> refs/remotes/origin/master
	return 0;
}

static void
qlcnic_free_irq(struct qlcnic_adapter *adapter)
{
	int ring;
	struct qlcnic_host_sds_ring *sds_ring;
<<<<<<< HEAD

	struct qlcnic_recv_context *recv_ctx = adapter->recv_ctx;

	for (ring = 0; ring < adapter->max_sds_rings; ring++) {
		sds_ring = &recv_ctx->sds_rings[ring];
		free_irq(sds_ring->irq, sds_ring);
	}
}

static int
__qlcnic_up(struct qlcnic_adapter *adapter, struct net_device *netdev)
=======
	struct qlcnic_host_tx_ring *tx_ring;

	struct qlcnic_recv_context *recv_ctx = adapter->recv_ctx;

	if (adapter->ahw->diag_test != QLCNIC_LOOPBACK_TEST) {
		if (qlcnic_82xx_check(adapter) ||
		    (qlcnic_83xx_check(adapter) &&
		     (adapter->flags & QLCNIC_MSIX_ENABLED))) {
			for (ring = 0; ring < adapter->drv_sds_rings; ring++) {
				sds_ring = &recv_ctx->sds_rings[ring];
				free_irq(sds_ring->irq, sds_ring);
			}
		}
		if ((qlcnic_83xx_check(adapter) &&
		     !(adapter->flags & QLCNIC_TX_INTR_SHARED)) ||
		    (qlcnic_82xx_check(adapter) &&
		     qlcnic_check_multi_tx(adapter))) {
			for (ring = 0; ring < adapter->drv_tx_rings;
			     ring++) {
				tx_ring = &adapter->tx_ring[ring];
				if (tx_ring->irq)
					free_irq(tx_ring->irq, tx_ring);
			}
		}
	}
}

static void qlcnic_get_lro_mss_capability(struct qlcnic_adapter *adapter)
{
	u32 capab = 0;

	if (qlcnic_82xx_check(adapter)) {
		if (adapter->ahw->extra_capability[0] &
		    QLCNIC_FW_CAPABILITY_2_LRO_MAX_TCP_SEG)
			adapter->flags |= QLCNIC_FW_LRO_MSS_CAP;
	} else {
		capab = adapter->ahw->capabilities;
		if (QLC_83XX_GET_FW_LRO_MSS_CAPABILITY(capab))
			adapter->flags |= QLCNIC_FW_LRO_MSS_CAP;
	}
}

int __qlcnic_up(struct qlcnic_adapter *adapter, struct net_device *netdev)
>>>>>>> refs/remotes/origin/master
{
	int ring;
	struct qlcnic_host_rds_ring *rds_ring;

	if (adapter->is_up != QLCNIC_ADAPTER_UP_MAGIC)
		return -EIO;

	if (test_bit(__QLCNIC_DEV_UP, &adapter->state))
		return 0;
<<<<<<< HEAD
	if (qlcnic_set_eswitch_port_config(adapter))
		return -EIO;

=======

	if (qlcnic_set_eswitch_port_config(adapter))
		return -EIO;

	qlcnic_get_lro_mss_capability(adapter);

>>>>>>> refs/remotes/origin/master
	if (qlcnic_fw_create_ctx(adapter))
		return -EIO;

	for (ring = 0; ring < adapter->max_rds_rings; ring++) {
		rds_ring = &adapter->recv_ctx->rds_rings[ring];
<<<<<<< HEAD
		qlcnic_post_rx_buffers(adapter, rds_ring);
=======
		qlcnic_post_rx_buffers(adapter, rds_ring, ring);
>>>>>>> refs/remotes/origin/master
	}

	qlcnic_set_multi(netdev);
	qlcnic_fw_cmd_set_mtu(adapter, netdev->mtu);

	adapter->ahw->linkup = 0;

<<<<<<< HEAD
	if (adapter->max_sds_rings > 1)
=======
	if (adapter->drv_sds_rings > 1)
>>>>>>> refs/remotes/origin/master
		qlcnic_config_rss(adapter, 1);

	qlcnic_config_intr_coalesce(adapter);

	if (netdev->features & NETIF_F_LRO)
		qlcnic_config_hw_lro(adapter, QLCNIC_LRO_ENABLED);

<<<<<<< HEAD
=======
	set_bit(__QLCNIC_DEV_UP, &adapter->state);
>>>>>>> refs/remotes/origin/master
	qlcnic_napi_enable(adapter);

	qlcnic_linkevent_request(adapter, 1);

<<<<<<< HEAD
	adapter->reset_context = 0;
	set_bit(__QLCNIC_DEV_UP, &adapter->state);
	return 0;
}

/* Usage: During resume and firmware recovery module.*/

static int
qlcnic_up(struct qlcnic_adapter *adapter, struct net_device *netdev)
=======
	adapter->ahw->reset_context = 0;
	return 0;
}

int qlcnic_up(struct qlcnic_adapter *adapter, struct net_device *netdev)
>>>>>>> refs/remotes/origin/master
{
	int err = 0;

	rtnl_lock();
	if (netif_running(netdev))
		err = __qlcnic_up(adapter, netdev);
	rtnl_unlock();

	return err;
}

<<<<<<< HEAD
static void
__qlcnic_down(struct qlcnic_adapter *adapter, struct net_device *netdev)
{
=======
void __qlcnic_down(struct qlcnic_adapter *adapter, struct net_device *netdev)
{
	int ring;

>>>>>>> refs/remotes/origin/master
	if (adapter->is_up != QLCNIC_ADAPTER_UP_MAGIC)
		return;

	if (!test_and_clear_bit(__QLCNIC_DEV_UP, &adapter->state))
		return;

<<<<<<< HEAD
	smp_mb();
	spin_lock(&adapter->tx_clean_lock);
	netif_carrier_off(netdev);
=======
	if (qlcnic_sriov_vf_check(adapter))
		qlcnic_sriov_cleanup_async_list(&adapter->ahw->sriov->bc);
	smp_mb();
	netif_carrier_off(netdev);
	adapter->ahw->linkup = 0;
>>>>>>> refs/remotes/origin/master
	netif_tx_disable(netdev);

	qlcnic_free_mac_list(adapter);

	if (adapter->fhash.fnum)
		qlcnic_delete_lb_filters(adapter);

	qlcnic_nic_set_promisc(adapter, QLCNIC_NIU_NON_PROMISC_MODE);

	qlcnic_napi_disable(adapter);

	qlcnic_fw_destroy_ctx(adapter);
<<<<<<< HEAD

	qlcnic_reset_rx_buffers_list(adapter);
	qlcnic_release_tx_buffers(adapter);
	spin_unlock(&adapter->tx_clean_lock);
=======
	adapter->flags &= ~QLCNIC_FW_LRO_MSS_CAP;

	qlcnic_reset_rx_buffers_list(adapter);

	for (ring = 0; ring < adapter->drv_tx_rings; ring++)
		qlcnic_release_tx_buffers(adapter, &adapter->tx_ring[ring]);
>>>>>>> refs/remotes/origin/master
}

/* Usage: During suspend and firmware recovery module */

<<<<<<< HEAD
static void
qlcnic_down(struct qlcnic_adapter *adapter, struct net_device *netdev)
=======
void qlcnic_down(struct qlcnic_adapter *adapter, struct net_device *netdev)
>>>>>>> refs/remotes/origin/master
{
	rtnl_lock();
	if (netif_running(netdev))
		__qlcnic_down(adapter, netdev);
	rtnl_unlock();

}

<<<<<<< HEAD
static int
=======
int
>>>>>>> refs/remotes/origin/master
qlcnic_attach(struct qlcnic_adapter *adapter)
{
	struct net_device *netdev = adapter->netdev;
	struct pci_dev *pdev = adapter->pdev;
	int err;

	if (adapter->is_up == QLCNIC_ADAPTER_UP_MAGIC)
		return 0;

	err = qlcnic_napi_add(adapter, netdev);
	if (err)
		return err;

	err = qlcnic_alloc_sw_resources(adapter);
	if (err) {
		dev_err(&pdev->dev, "Error in setting sw resources\n");
		goto err_out_napi_del;
	}

	err = qlcnic_alloc_hw_resources(adapter);
	if (err) {
		dev_err(&pdev->dev, "Error in setting hw resources\n");
		goto err_out_free_sw;
	}

	err = qlcnic_request_irq(adapter);
	if (err) {
		dev_err(&pdev->dev, "failed to setup interrupt\n");
		goto err_out_free_hw;
	}

	qlcnic_create_sysfs_entries(adapter);

	adapter->is_up = QLCNIC_ADAPTER_UP_MAGIC;
	return 0;

err_out_free_hw:
	qlcnic_free_hw_resources(adapter);
err_out_free_sw:
	qlcnic_free_sw_resources(adapter);
err_out_napi_del:
	qlcnic_napi_del(adapter);
	return err;
}

<<<<<<< HEAD
static void
qlcnic_detach(struct qlcnic_adapter *adapter)
=======
void qlcnic_detach(struct qlcnic_adapter *adapter)
>>>>>>> refs/remotes/origin/master
{
	if (adapter->is_up != QLCNIC_ADAPTER_UP_MAGIC)
		return;

	qlcnic_remove_sysfs_entries(adapter);

	qlcnic_free_hw_resources(adapter);
	qlcnic_release_rx_buffers(adapter);
	qlcnic_free_irq(adapter);
	qlcnic_napi_del(adapter);
	qlcnic_free_sw_resources(adapter);

	adapter->is_up = 0;
}

<<<<<<< HEAD
void qlcnic_diag_free_res(struct net_device *netdev, int max_sds_rings)
{
	struct qlcnic_adapter *adapter = netdev_priv(netdev);
	struct qlcnic_host_sds_ring *sds_ring;
	int ring;

	clear_bit(__QLCNIC_DEV_UP, &adapter->state);
	if (adapter->diag_test == QLCNIC_INTERRUPT_TEST) {
		for (ring = 0; ring < adapter->max_sds_rings; ring++) {
=======
void qlcnic_diag_free_res(struct net_device *netdev, int drv_sds_rings)
{
	struct qlcnic_adapter *adapter = netdev_priv(netdev);
	struct qlcnic_host_sds_ring *sds_ring;
	int drv_tx_rings = adapter->drv_tx_rings;
	int ring;

	clear_bit(__QLCNIC_DEV_UP, &adapter->state);
	if (adapter->ahw->diag_test == QLCNIC_INTERRUPT_TEST) {
		for (ring = 0; ring < adapter->drv_sds_rings; ring++) {
>>>>>>> refs/remotes/origin/master
			sds_ring = &adapter->recv_ctx->sds_rings[ring];
			qlcnic_disable_int(sds_ring);
		}
	}

	qlcnic_fw_destroy_ctx(adapter);

	qlcnic_detach(adapter);

<<<<<<< HEAD
	adapter->diag_test = 0;
	adapter->max_sds_rings = max_sds_rings;
=======
	adapter->ahw->diag_test = 0;
	adapter->drv_sds_rings = drv_sds_rings;
	adapter->drv_tx_rings = drv_tx_rings;
>>>>>>> refs/remotes/origin/master

	if (qlcnic_attach(adapter))
		goto out;

	if (netif_running(netdev))
		__qlcnic_up(adapter, netdev);
out:
	netif_device_attach(netdev);
}

static int qlcnic_alloc_adapter_resources(struct qlcnic_adapter *adapter)
{
<<<<<<< HEAD
	int err = 0;
	adapter->ahw = kzalloc(sizeof(struct qlcnic_hardware_context),
				GFP_KERNEL);
	if (!adapter->ahw) {
		dev_err(&adapter->pdev->dev,
			"Failed to allocate recv ctx resources for adapter\n");
		err = -ENOMEM;
		goto err_out;
	}
	adapter->recv_ctx = kzalloc(sizeof(struct qlcnic_recv_context),
				GFP_KERNEL);
	if (!adapter->recv_ctx) {
		dev_err(&adapter->pdev->dev,
			"Failed to allocate recv ctx resources for adapter\n");
		kfree(adapter->ahw);
		adapter->ahw = NULL;
=======
	struct qlcnic_hardware_context *ahw = adapter->ahw;
	int err = 0;

	adapter->recv_ctx = kzalloc(sizeof(struct qlcnic_recv_context),
				GFP_KERNEL);
	if (!adapter->recv_ctx) {
>>>>>>> refs/remotes/origin/master
		err = -ENOMEM;
		goto err_out;
	}
	/* Initialize interrupt coalesce parameters */
<<<<<<< HEAD
	adapter->ahw->coal.flag = QLCNIC_INTR_DEFAULT;
	adapter->ahw->coal.rx_time_us = QLCNIC_DEFAULT_INTR_COALESCE_RX_TIME_US;
	adapter->ahw->coal.rx_packets = QLCNIC_DEFAULT_INTR_COALESCE_RX_PACKETS;
=======
	ahw->coal.flag = QLCNIC_INTR_DEFAULT;
	ahw->coal.type = QLCNIC_INTR_COAL_TYPE_RX;
	ahw->coal.rx_time_us = QLCNIC_DEF_INTR_COALESCE_RX_TIME_US;
	ahw->coal.rx_packets = QLCNIC_DEF_INTR_COALESCE_RX_PACKETS;
	if (qlcnic_83xx_check(adapter)) {
		ahw->coal.tx_time_us = QLCNIC_DEF_INTR_COALESCE_TX_TIME_US;
		ahw->coal.tx_packets = QLCNIC_DEF_INTR_COALESCE_TX_PACKETS;
	}
	/* clear stats */
	memset(&adapter->stats, 0, sizeof(adapter->stats));
>>>>>>> refs/remotes/origin/master
err_out:
	return err;
}

static void qlcnic_free_adapter_resources(struct qlcnic_adapter *adapter)
{
	kfree(adapter->recv_ctx);
	adapter->recv_ctx = NULL;

	if (adapter->ahw->fw_dump.tmpl_hdr) {
		vfree(adapter->ahw->fw_dump.tmpl_hdr);
		adapter->ahw->fw_dump.tmpl_hdr = NULL;
	}
<<<<<<< HEAD
	kfree(adapter->ahw);
	adapter->ahw = NULL;
=======

	kfree(adapter->ahw->reset.buff);
	adapter->ahw->fw_dump.tmpl_hdr = NULL;
>>>>>>> refs/remotes/origin/master
}

int qlcnic_diag_alloc_res(struct net_device *netdev, int test)
{
	struct qlcnic_adapter *adapter = netdev_priv(netdev);
	struct qlcnic_host_sds_ring *sds_ring;
	struct qlcnic_host_rds_ring *rds_ring;
	int ring;
	int ret;

	netif_device_detach(netdev);

	if (netif_running(netdev))
		__qlcnic_down(adapter, netdev);

	qlcnic_detach(adapter);

<<<<<<< HEAD
	adapter->max_sds_rings = 1;
	adapter->diag_test = test;
=======
	adapter->drv_sds_rings = QLCNIC_SINGLE_RING;
	adapter->ahw->diag_test = test;
	adapter->ahw->linkup = 0;
>>>>>>> refs/remotes/origin/master

	ret = qlcnic_attach(adapter);
	if (ret) {
		netif_device_attach(netdev);
		return ret;
	}

	ret = qlcnic_fw_create_ctx(adapter);
	if (ret) {
		qlcnic_detach(adapter);
		netif_device_attach(netdev);
		return ret;
	}

	for (ring = 0; ring < adapter->max_rds_rings; ring++) {
		rds_ring = &adapter->recv_ctx->rds_rings[ring];
<<<<<<< HEAD
		qlcnic_post_rx_buffers(adapter, rds_ring);
	}

	if (adapter->diag_test == QLCNIC_INTERRUPT_TEST) {
		for (ring = 0; ring < adapter->max_sds_rings; ring++) {
=======
		qlcnic_post_rx_buffers(adapter, rds_ring, ring);
	}

	if (adapter->ahw->diag_test == QLCNIC_INTERRUPT_TEST) {
		for (ring = 0; ring < adapter->drv_sds_rings; ring++) {
>>>>>>> refs/remotes/origin/master
			sds_ring = &adapter->recv_ctx->sds_rings[ring];
			qlcnic_enable_int(sds_ring);
		}
	}

<<<<<<< HEAD
	if (adapter->diag_test == QLCNIC_LOOPBACK_TEST) {
=======
	if (adapter->ahw->diag_test == QLCNIC_LOOPBACK_TEST) {
>>>>>>> refs/remotes/origin/master
		adapter->ahw->loopback_state = 0;
		qlcnic_linkevent_request(adapter, 1);
	}

	set_bit(__QLCNIC_DEV_UP, &adapter->state);

	return 0;
}

/* Reset context in hardware only */
static int
qlcnic_reset_hw_context(struct qlcnic_adapter *adapter)
{
	struct net_device *netdev = adapter->netdev;

	if (test_and_set_bit(__QLCNIC_RESETTING, &adapter->state))
		return -EBUSY;

	netif_device_detach(netdev);

	qlcnic_down(adapter, netdev);

	qlcnic_up(adapter, netdev);

	netif_device_attach(netdev);

	clear_bit(__QLCNIC_RESETTING, &adapter->state);
<<<<<<< HEAD
=======
	dev_err(&adapter->pdev->dev, "%s:\n", __func__);
>>>>>>> refs/remotes/origin/master
	return 0;
}

int
qlcnic_reset_context(struct qlcnic_adapter *adapter)
{
	int err = 0;
	struct net_device *netdev = adapter->netdev;

	if (test_and_set_bit(__QLCNIC_RESETTING, &adapter->state))
		return -EBUSY;

	if (adapter->is_up == QLCNIC_ADAPTER_UP_MAGIC) {

		netif_device_detach(netdev);

		if (netif_running(netdev))
			__qlcnic_down(adapter, netdev);

		qlcnic_detach(adapter);

		if (netif_running(netdev)) {
			err = qlcnic_attach(adapter);
			if (!err) {
				__qlcnic_up(adapter, netdev);
				qlcnic_restore_indev_addr(netdev, NETDEV_UP);
			}
		}

		netif_device_attach(netdev);
	}

	clear_bit(__QLCNIC_RESETTING, &adapter->state);
	return err;
}

<<<<<<< HEAD
static int
qlcnic_setup_netdev(struct qlcnic_adapter *adapter,
		struct net_device *netdev, u8 pci_using_dac)
=======
void qlcnic_82xx_set_mac_filter_count(struct qlcnic_adapter *adapter)
{
	struct qlcnic_hardware_context *ahw = adapter->ahw;
	u16 act_pci_fn = ahw->act_pci_func;
	u16 count;

	ahw->max_mc_count = QLCNIC_MAX_MC_COUNT;
	if (act_pci_fn <= 2)
		count = (QLCNIC_MAX_UC_COUNT - QLCNIC_MAX_MC_COUNT) /
			 act_pci_fn;
	else
		count = (QLCNIC_LB_MAX_FILTERS - QLCNIC_MAX_MC_COUNT) /
			 act_pci_fn;
	ahw->max_uc_count = count;
}

int
qlcnic_setup_netdev(struct qlcnic_adapter *adapter, struct net_device *netdev,
		    int pci_using_dac)
>>>>>>> refs/remotes/origin/master
{
	int err;
	struct pci_dev *pdev = adapter->pdev;

<<<<<<< HEAD
	adapter->mc_enabled = 0;
	adapter->max_mc_count = 38;

	netdev->netdev_ops	   = &qlcnic_netdev_ops;
	netdev->watchdog_timeo     = 5*HZ;

	qlcnic_change_mtu(netdev, netdev->mtu);

	SET_ETHTOOL_OPS(netdev, &qlcnic_ethtool_ops);

	netdev->hw_features = NETIF_F_SG | NETIF_F_IP_CSUM |
		NETIF_F_IPV6_CSUM | NETIF_F_RXCSUM;

	if (adapter->capabilities & QLCNIC_FW_CAPABILITY_TSO)
		netdev->hw_features |= NETIF_F_TSO | NETIF_F_TSO6;
	if (pci_using_dac)
		netdev->hw_features |= NETIF_F_HIGHDMA;

	netdev->vlan_features = netdev->hw_features;

	if (adapter->capabilities & QLCNIC_FW_CAPABILITY_FVLANTX)
		netdev->hw_features |= NETIF_F_HW_VLAN_TX;
	if (adapter->capabilities & QLCNIC_FW_CAPABILITY_HW_LRO)
		netdev->hw_features |= NETIF_F_LRO;

	netdev->features |= netdev->hw_features |
		NETIF_F_HW_VLAN_RX | NETIF_F_HW_VLAN_FILTER;

	netdev->irq = adapter->msix_entries[0].vector;

=======
	adapter->rx_csum = 1;
	adapter->ahw->mc_enabled = 0;
	qlcnic_set_mac_filter_count(adapter);

	netdev->netdev_ops	   = &qlcnic_netdev_ops;
	netdev->watchdog_timeo     = QLCNIC_WATCHDOG_TIMEOUTVALUE * HZ;

	qlcnic_change_mtu(netdev, netdev->mtu);

	if (qlcnic_sriov_vf_check(adapter))
		SET_ETHTOOL_OPS(netdev, &qlcnic_sriov_vf_ethtool_ops);
	else
		SET_ETHTOOL_OPS(netdev, &qlcnic_ethtool_ops);

	netdev->features |= (NETIF_F_SG | NETIF_F_IP_CSUM | NETIF_F_RXCSUM |
			     NETIF_F_IPV6_CSUM | NETIF_F_GRO |
			     NETIF_F_HW_VLAN_CTAG_RX);
	netdev->vlan_features |= (NETIF_F_SG | NETIF_F_IP_CSUM |
				  NETIF_F_IPV6_CSUM);

	if (QLCNIC_IS_TSO_CAPABLE(adapter)) {
		netdev->features |= (NETIF_F_TSO | NETIF_F_TSO6);
		netdev->vlan_features |= (NETIF_F_TSO | NETIF_F_TSO6);
	}

	if (pci_using_dac) {
		netdev->features |= NETIF_F_HIGHDMA;
		netdev->vlan_features |= NETIF_F_HIGHDMA;
	}

	if (qlcnic_vlan_tx_check(adapter))
		netdev->features |= (NETIF_F_HW_VLAN_CTAG_TX);

	if (qlcnic_sriov_vf_check(adapter))
		netdev->features |= NETIF_F_HW_VLAN_CTAG_FILTER;

	if (adapter->ahw->capabilities & QLCNIC_FW_CAPABILITY_HW_LRO)
		netdev->features |= NETIF_F_LRO;

	netdev->hw_features = netdev->features;
	netdev->priv_flags |= IFF_UNICAST_FLT;
	netdev->irq = adapter->msix_entries[0].vector;

	err = qlcnic_set_real_num_queues(adapter, netdev);
	if (err)
		return err;

>>>>>>> refs/remotes/origin/master
	err = register_netdev(netdev);
	if (err) {
		dev_err(&pdev->dev, "failed to register net device\n");
		return err;
	}

<<<<<<< HEAD
	return 0;
}

static int qlcnic_set_dma_mask(struct pci_dev *pdev, u8 *pci_using_dac)
=======
	qlcnic_dcb_init_dcbnl_ops(adapter->dcb);

	return 0;
}

static int qlcnic_set_dma_mask(struct pci_dev *pdev, int *pci_using_dac)
>>>>>>> refs/remotes/origin/master
{
	if (!pci_set_dma_mask(pdev, DMA_BIT_MASK(64)) &&
			!pci_set_consistent_dma_mask(pdev, DMA_BIT_MASK(64)))
		*pci_using_dac = 1;
	else if (!pci_set_dma_mask(pdev, DMA_BIT_MASK(32)) &&
			!pci_set_consistent_dma_mask(pdev, DMA_BIT_MASK(32)))
		*pci_using_dac = 0;
	else {
		dev_err(&pdev->dev, "Unable to set DMA mask, aborting\n");
		return -EIO;
	}

	return 0;
}

<<<<<<< HEAD
static int
qlcnic_alloc_msix_entries(struct qlcnic_adapter *adapter, u16 count)
{
	adapter->msix_entries = kcalloc(count, sizeof(struct msix_entry),
					GFP_KERNEL);

	if (adapter->msix_entries)
		return 0;

	dev_err(&adapter->pdev->dev, "failed allocating msix_entries\n");
	return -ENOMEM;
}

static int __devinit
=======
void qlcnic_free_tx_rings(struct qlcnic_adapter *adapter)
{
	int ring;
	struct qlcnic_host_tx_ring *tx_ring;

	for (ring = 0; ring < adapter->drv_tx_rings; ring++) {
		tx_ring = &adapter->tx_ring[ring];
		if (tx_ring && tx_ring->cmd_buf_arr != NULL) {
			vfree(tx_ring->cmd_buf_arr);
			tx_ring->cmd_buf_arr = NULL;
		}
	}
	if (adapter->tx_ring != NULL)
		kfree(adapter->tx_ring);
}

int qlcnic_alloc_tx_rings(struct qlcnic_adapter *adapter,
			  struct net_device *netdev)
{
	int ring, vector, index;
	struct qlcnic_host_tx_ring *tx_ring;
	struct qlcnic_cmd_buffer *cmd_buf_arr;

	tx_ring = kcalloc(adapter->drv_tx_rings,
			  sizeof(struct qlcnic_host_tx_ring), GFP_KERNEL);
	if (tx_ring == NULL)
		return -ENOMEM;

	adapter->tx_ring = tx_ring;

	for (ring = 0; ring < adapter->drv_tx_rings; ring++) {
		tx_ring = &adapter->tx_ring[ring];
		tx_ring->num_desc = adapter->num_txd;
		tx_ring->txq = netdev_get_tx_queue(netdev, ring);
		cmd_buf_arr = vzalloc(TX_BUFF_RINGSIZE(tx_ring));
		if (cmd_buf_arr == NULL) {
			qlcnic_free_tx_rings(adapter);
			return -ENOMEM;
		}
		memset(cmd_buf_arr, 0, TX_BUFF_RINGSIZE(tx_ring));
		tx_ring->cmd_buf_arr = cmd_buf_arr;
		spin_lock_init(&tx_ring->tx_clean_lock);
	}

	if (qlcnic_83xx_check(adapter) ||
	    (qlcnic_82xx_check(adapter) && qlcnic_check_multi_tx(adapter))) {
		for (ring = 0; ring < adapter->drv_tx_rings; ring++) {
			tx_ring = &adapter->tx_ring[ring];
			tx_ring->adapter = adapter;
			if (adapter->flags & QLCNIC_MSIX_ENABLED) {
				index = adapter->drv_sds_rings + ring;
				vector = adapter->msix_entries[index].vector;
				tx_ring->irq = vector;
			}
		}
	}

	return 0;
}

void qlcnic_set_drv_version(struct qlcnic_adapter *adapter)
{
	struct qlcnic_hardware_context *ahw = adapter->ahw;
	u32 fw_cmd = 0;

	if (qlcnic_82xx_check(adapter))
		fw_cmd = QLCNIC_CMD_82XX_SET_DRV_VER;
	else if (qlcnic_83xx_check(adapter))
		fw_cmd = QLCNIC_CMD_83XX_SET_DRV_VER;

	if (ahw->extra_capability[0] & QLCNIC_FW_CAPABILITY_SET_DRV_VER)
		qlcnic_fw_cmd_set_drv_version(adapter, fw_cmd);
}

static int
>>>>>>> refs/remotes/origin/master
qlcnic_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
{
	struct net_device *netdev = NULL;
	struct qlcnic_adapter *adapter = NULL;
<<<<<<< HEAD
	int err;
	uint8_t revision_id;
	uint8_t pci_using_dac;
	char brd_name[QLCNIC_MAX_BOARD_NAME_LEN];
=======
	struct qlcnic_hardware_context *ahw;
	int err, pci_using_dac = -1;
	char board_name[QLCNIC_MAX_BOARD_NAME_LEN + 19]; /* MAC + ": " + name */
	struct qlcnic_dcb *dcb;

	if (pdev->is_virtfn)
		return -ENODEV;
>>>>>>> refs/remotes/origin/master

	err = pci_enable_device(pdev);
	if (err)
		return err;

	if (!(pci_resource_flags(pdev, 0) & IORESOURCE_MEM)) {
		err = -ENODEV;
		goto err_out_disable_pdev;
	}

	err = qlcnic_set_dma_mask(pdev, &pci_using_dac);
	if (err)
		goto err_out_disable_pdev;

	err = pci_request_regions(pdev, qlcnic_driver_name);
	if (err)
		goto err_out_disable_pdev;

	pci_set_master(pdev);
	pci_enable_pcie_error_reporting(pdev);

<<<<<<< HEAD
	netdev = alloc_etherdev(sizeof(struct qlcnic_adapter));
	if (!netdev) {
=======
	ahw = kzalloc(sizeof(struct qlcnic_hardware_context), GFP_KERNEL);
	if (!ahw) {
>>>>>>> refs/remotes/origin/master
		err = -ENOMEM;
		goto err_out_free_res;
	}

<<<<<<< HEAD
=======
	switch (ent->device) {
	case PCI_DEVICE_ID_QLOGIC_QLE824X:
		ahw->hw_ops = &qlcnic_hw_ops;
		ahw->reg_tbl = (u32 *) qlcnic_reg_tbl;
		break;
	case PCI_DEVICE_ID_QLOGIC_QLE834X:
	case PCI_DEVICE_ID_QLOGIC_QLE844X:
		qlcnic_83xx_register_map(ahw);
		break;
	case PCI_DEVICE_ID_QLOGIC_VF_QLE834X:
	case PCI_DEVICE_ID_QLOGIC_VF_QLE844X:
		qlcnic_sriov_vf_register_map(ahw);
		break;
	default:
		goto err_out_free_hw_res;
	}

	err = qlcnic_setup_pci_map(pdev, ahw);
	if (err)
		goto err_out_free_hw_res;

	netdev = alloc_etherdev_mq(sizeof(struct qlcnic_adapter),
				   QLCNIC_MAX_TX_RINGS);
	if (!netdev) {
		err = -ENOMEM;
		goto err_out_iounmap;
	}

>>>>>>> refs/remotes/origin/master
	SET_NETDEV_DEV(netdev, &pdev->dev);

	adapter = netdev_priv(netdev);
	adapter->netdev  = netdev;
	adapter->pdev    = pdev;
<<<<<<< HEAD

	if (qlcnic_alloc_adapter_resources(adapter))
		goto err_out_free_netdev;

	adapter->dev_rst_time = jiffies;
	revision_id = pdev->revision;
	adapter->ahw->revision_id = revision_id;
	adapter->mac_learn = qlcnic_mac_learn;
=======
	adapter->ahw = ahw;

	adapter->qlcnic_wq = create_singlethread_workqueue("qlcnic");
	if (adapter->qlcnic_wq == NULL) {
		err = -ENOMEM;
		dev_err(&pdev->dev, "Failed to create workqueue\n");
		goto err_out_free_netdev;
	}

	err = qlcnic_alloc_adapter_resources(adapter);
	if (err)
		goto err_out_free_wq;

	adapter->dev_rst_time = jiffies;
	adapter->ahw->revision_id = pdev->revision;
	if (qlcnic_mac_learn == FDB_MAC_LEARN)
		adapter->fdb_mac_learn = true;
	else if (qlcnic_mac_learn == DRV_MAC_LEARN)
		adapter->drv_mac_learn = true;
>>>>>>> refs/remotes/origin/master

	rwlock_init(&adapter->ahw->crb_lock);
	mutex_init(&adapter->ahw->mem_lock);

<<<<<<< HEAD
	spin_lock_init(&adapter->tx_clean_lock);
	INIT_LIST_HEAD(&adapter->mac_list);

	err = qlcnic_setup_pci_map(adapter);
	if (err)
		goto err_out_free_hw;

	/* This will be reset for mezz cards  */
	adapter->portnum = adapter->ahw->pci_func;

	err = qlcnic_get_board_info(adapter);
	if (err) {
		dev_err(&pdev->dev, "Error getting board config info.\n");
		goto err_out_iounmap;
	}

	err = qlcnic_setup_idc_param(adapter);
	if (err)
		goto err_out_iounmap;

	adapter->flags |= QLCNIC_NEED_FLR;

	err = adapter->nic_ops->start_firmware(adapter);
	if (err) {
		dev_err(&pdev->dev, "Loading fw failed.Please Reboot\n");
		goto err_out_decr_ref;
=======
	INIT_LIST_HEAD(&adapter->mac_list);

	qlcnic_register_dcb(adapter);

	if (qlcnic_82xx_check(adapter)) {
		qlcnic_check_vf(adapter, ent);
		adapter->portnum = adapter->ahw->pci_func;
		err = qlcnic_start_firmware(adapter);
		if (err) {
			dev_err(&pdev->dev, "Loading fw failed.Please Reboot\n"
				"\t\tIf reboot doesn't help, try flashing the card\n");
			goto err_out_maintenance_mode;
		}

		/* compute and set default and max tx/sds rings */
		if (adapter->ahw->msix_supported) {
			if (qlcnic_check_multi_tx_capability(adapter) == 1)
				qlcnic_set_tx_ring_count(adapter,
							 QLCNIC_SINGLE_RING);
			else
				qlcnic_set_tx_ring_count(adapter,
							 QLCNIC_DEF_TX_RINGS);
			qlcnic_set_sds_ring_count(adapter,
						  QLCNIC_DEF_SDS_RINGS);
		} else {
			qlcnic_set_tx_ring_count(adapter, QLCNIC_SINGLE_RING);
			qlcnic_set_sds_ring_count(adapter, QLCNIC_SINGLE_RING);
		}

		err = qlcnic_setup_idc_param(adapter);
		if (err)
			goto err_out_free_hw;

		adapter->flags |= QLCNIC_NEED_FLR;

		dcb = adapter->dcb;

		if (dcb && qlcnic_dcb_attach(dcb))
			qlcnic_clear_dcb_ops(dcb);
	} else if (qlcnic_83xx_check(adapter)) {
		qlcnic_83xx_check_vf(adapter, ent);
		adapter->portnum = adapter->ahw->pci_func;
		err = qlcnic_83xx_init(adapter, pci_using_dac);
		if (err) {
			switch (err) {
			case -ENOTRECOVERABLE:
				dev_err(&pdev->dev, "Adapter initialization failed due to a faulty hardware. Please reboot\n");
				dev_err(&pdev->dev, "If reboot doesn't help, please replace the adapter with new one and return the faulty adapter for repair\n");
				goto err_out_free_hw;
			case -ENOMEM:
				dev_err(&pdev->dev, "Adapter initialization failed. Please reboot\n");
				goto err_out_free_hw;
			default:
				dev_err(&pdev->dev, "Adapter initialization failed. A reboot may be required to recover from this failure\n");
				dev_err(&pdev->dev, "If reboot does not help to recover from this failure, try a flash update of the adapter\n");
				goto err_out_maintenance_mode;
			}
		}

		if (qlcnic_sriov_vf_check(adapter))
			return 0;
	} else {
		dev_err(&pdev->dev,
			"%s: failed. Please Reboot\n", __func__);
		goto err_out_free_hw;
>>>>>>> refs/remotes/origin/master
	}

	if (qlcnic_read_mac_addr(adapter))
		dev_warn(&pdev->dev, "failed to read mac addr\n");

<<<<<<< HEAD
	if (adapter->portnum == 0) {
		get_brd_name(adapter, brd_name);

		pr_info("%s: %s Board Chip rev 0x%x\n",
				module_name(THIS_MODULE),
				brd_name, adapter->ahw->revision_id);
	}

	qlcnic_clear_stats(adapter);

	err = qlcnic_alloc_msix_entries(adapter, adapter->max_rx_ques);
	if (err)
		goto err_out_decr_ref;

	qlcnic_setup_intr(adapter);

	err = qlcnic_setup_netdev(adapter, netdev, pci_using_dac);
	if (err)
		goto err_out_disable_msi;

	pci_set_drvdata(pdev, adapter);

	qlcnic_schedule_work(adapter, qlcnic_fw_poll_work, FW_POLL_DELAY);
=======
	qlcnic_read_phys_port_id(adapter);

	if (adapter->portnum == 0) {
		qlcnic_get_board_name(adapter, board_name);

		pr_info("%s: %s Board Chip rev 0x%x\n",
			module_name(THIS_MODULE),
			board_name, adapter->ahw->revision_id);
	}

	if (qlcnic_83xx_check(adapter) && !qlcnic_use_msi_x &&
	    !!qlcnic_use_msi)
		dev_warn(&pdev->dev,
			 "Device does not support MSI interrupts\n");

	if (qlcnic_82xx_check(adapter)) {
		err = qlcnic_setup_intr(adapter);
		if (err) {
			dev_err(&pdev->dev, "Failed to setup interrupt\n");
			goto err_out_disable_msi;
		}
	}

	err = qlcnic_get_act_pci_func(adapter);
	if (err)
		goto err_out_disable_mbx_intr;

	err = qlcnic_setup_netdev(adapter, netdev, pci_using_dac);
	if (err)
		goto err_out_disable_mbx_intr;

	if (adapter->portnum == 0)
		qlcnic_set_drv_version(adapter);

	pci_set_drvdata(pdev, adapter);

	if (qlcnic_82xx_check(adapter))
		qlcnic_schedule_work(adapter, qlcnic_fw_poll_work,
				     FW_POLL_DELAY);
>>>>>>> refs/remotes/origin/master

	switch (adapter->ahw->port_type) {
	case QLCNIC_GBE:
		dev_info(&adapter->pdev->dev, "%s: GbE port initialized\n",
				adapter->netdev->name);
		break;
	case QLCNIC_XGBE:
		dev_info(&adapter->pdev->dev, "%s: XGbE port initialized\n",
				adapter->netdev->name);
		break;
	}

<<<<<<< HEAD
	if (adapter->mac_learn)
		qlcnic_alloc_lb_filters_mem(adapter);

	qlcnic_create_diag_entries(adapter);

	return 0;

err_out_disable_msi:
	qlcnic_teardown_intr(adapter);
	kfree(adapter->msix_entries);

err_out_decr_ref:
	qlcnic_clr_all_drv_state(adapter, 0);

err_out_iounmap:
	qlcnic_cleanup_pci_map(adapter);

err_out_free_hw:
	qlcnic_free_adapter_resources(adapter);

err_out_free_netdev:
	free_netdev(netdev);

=======
	if (adapter->drv_mac_learn)
		qlcnic_alloc_lb_filters_mem(adapter);

	qlcnic_add_sysfs(adapter);

	return 0;

err_out_disable_mbx_intr:
	if (qlcnic_83xx_check(adapter))
		qlcnic_83xx_free_mbx_intr(adapter);

err_out_disable_msi:
	qlcnic_teardown_intr(adapter);
	qlcnic_cancel_idc_work(adapter);
	qlcnic_clr_all_drv_state(adapter, 0);

err_out_free_hw:
	qlcnic_free_adapter_resources(adapter);

err_out_free_wq:
	destroy_workqueue(adapter->qlcnic_wq);

err_out_free_netdev:
	free_netdev(netdev);

err_out_iounmap:
	qlcnic_cleanup_pci_map(ahw);

err_out_free_hw_res:
	kfree(ahw);

>>>>>>> refs/remotes/origin/master
err_out_free_res:
	pci_release_regions(pdev);

err_out_disable_pdev:
<<<<<<< HEAD
	pci_set_drvdata(pdev, NULL);
	pci_disable_device(pdev);
	return err;
}

static void __devexit qlcnic_remove(struct pci_dev *pdev)
{
	struct qlcnic_adapter *adapter;
	struct net_device *netdev;
=======
	pci_disable_device(pdev);
	return err;

err_out_maintenance_mode:
	set_bit(__QLCNIC_MAINTENANCE_MODE, &adapter->state);
	netdev->netdev_ops = &qlcnic_netdev_failed_ops;
	SET_ETHTOOL_OPS(netdev, &qlcnic_ethtool_failed_ops);
	ahw->port_type = QLCNIC_XGBE;

	if (qlcnic_83xx_check(adapter))
		adapter->tgt_status_reg = NULL;
	else
		ahw->board_type = QLCNIC_BRDTYPE_P3P_10G_SFP_PLUS;

	err = register_netdev(netdev);

	if (err) {
		dev_err(&pdev->dev, "Failed to register net device\n");
		qlcnic_clr_all_drv_state(adapter, 0);
		goto err_out_free_hw;
	}

	pci_set_drvdata(pdev, adapter);
	qlcnic_add_sysfs(adapter);

	return 0;
}

static void qlcnic_remove(struct pci_dev *pdev)
{
	struct qlcnic_adapter *adapter;
	struct net_device *netdev;
	struct qlcnic_hardware_context *ahw;
>>>>>>> refs/remotes/origin/master

	adapter = pci_get_drvdata(pdev);
	if (adapter == NULL)
		return;

	netdev = adapter->netdev;
<<<<<<< HEAD

	qlcnic_cancel_fw_work(adapter);

	unregister_netdev(netdev);
=======
	qlcnic_sriov_pf_disable(adapter);

	qlcnic_cancel_idc_work(adapter);
	ahw = adapter->ahw;

	qlcnic_dcb_free(adapter->dcb);

	unregister_netdev(netdev);
	qlcnic_sriov_cleanup(adapter);

	if (qlcnic_83xx_check(adapter)) {
		qlcnic_83xx_register_nic_idc_func(adapter, 0);
		cancel_delayed_work_sync(&adapter->idc_aen_work);
		qlcnic_83xx_free_mbx_intr(adapter);
		qlcnic_83xx_detach_mailbox_work(adapter);
		qlcnic_83xx_free_mailbox(ahw->mailbox);
		kfree(ahw->fw_info);
	}
>>>>>>> refs/remotes/origin/master

	qlcnic_detach(adapter);

	if (adapter->npars != NULL)
		kfree(adapter->npars);
	if (adapter->eswitch != NULL)
		kfree(adapter->eswitch);

<<<<<<< HEAD
	qlcnic_clr_all_drv_state(adapter, 0);
=======
	if (qlcnic_82xx_check(adapter))
		qlcnic_clr_all_drv_state(adapter, 0);
>>>>>>> refs/remotes/origin/master

	clear_bit(__QLCNIC_RESETTING, &adapter->state);

	qlcnic_free_lb_filters_mem(adapter);

	qlcnic_teardown_intr(adapter);
<<<<<<< HEAD
	kfree(adapter->msix_entries);

	qlcnic_remove_diag_entries(adapter);

	qlcnic_cleanup_pci_map(adapter);
=======

	qlcnic_remove_sysfs(adapter);

	qlcnic_cleanup_pci_map(adapter->ahw);
>>>>>>> refs/remotes/origin/master

	qlcnic_release_firmware(adapter);

	pci_disable_pcie_error_reporting(pdev);
	pci_release_regions(pdev);
	pci_disable_device(pdev);
<<<<<<< HEAD
	pci_set_drvdata(pdev, NULL);

	qlcnic_free_adapter_resources(adapter);
	free_netdev(netdev);
}
static int __qlcnic_shutdown(struct pci_dev *pdev)
{
	struct qlcnic_adapter *adapter = pci_get_drvdata(pdev);
	struct net_device *netdev = adapter->netdev;
	int retval;

	netif_device_detach(netdev);

	qlcnic_cancel_fw_work(adapter);

	if (netif_running(netdev))
		qlcnic_down(adapter, netdev);

	qlcnic_clr_all_drv_state(adapter, 0);

	clear_bit(__QLCNIC_RESETTING, &adapter->state);

	retval = pci_save_state(pdev);
	if (retval)
		return retval;

	if (qlcnic_wol_supported(adapter)) {
		pci_enable_wake(pdev, PCI_D3cold, 1);
		pci_enable_wake(pdev, PCI_D3hot, 1);
	}

	return 0;
=======

	if (adapter->qlcnic_wq) {
		destroy_workqueue(adapter->qlcnic_wq);
		adapter->qlcnic_wq = NULL;
	}

	qlcnic_free_adapter_resources(adapter);
	kfree(ahw);
	free_netdev(netdev);
>>>>>>> refs/remotes/origin/master
}

static void qlcnic_shutdown(struct pci_dev *pdev)
{
	if (__qlcnic_shutdown(pdev))
		return;

	pci_disable_device(pdev);
}

#ifdef CONFIG_PM
<<<<<<< HEAD
static int
qlcnic_suspend(struct pci_dev *pdev, pm_message_t state)
=======
static int qlcnic_suspend(struct pci_dev *pdev, pm_message_t state)
>>>>>>> refs/remotes/origin/master
{
	int retval;

	retval = __qlcnic_shutdown(pdev);
	if (retval)
		return retval;

	pci_set_power_state(pdev, pci_choose_state(pdev, state));
	return 0;
}

<<<<<<< HEAD
static int
qlcnic_resume(struct pci_dev *pdev)
{
	struct qlcnic_adapter *adapter = pci_get_drvdata(pdev);
	struct net_device *netdev = adapter->netdev;
=======
static int qlcnic_resume(struct pci_dev *pdev)
{
	struct qlcnic_adapter *adapter = pci_get_drvdata(pdev);
>>>>>>> refs/remotes/origin/master
	int err;

	err = pci_enable_device(pdev);
	if (err)
		return err;

	pci_set_power_state(pdev, PCI_D0);
	pci_set_master(pdev);
	pci_restore_state(pdev);

<<<<<<< HEAD
	err = adapter->nic_ops->start_firmware(adapter);
	if (err) {
		dev_err(&pdev->dev, "failed to start firmware\n");
		return err;
	}

	if (netif_running(netdev)) {
		err = qlcnic_up(adapter, netdev);
		if (err)
			goto done;

		qlcnic_restore_indev_addr(netdev, NETDEV_UP);
	}
done:
	netif_device_attach(netdev);
	qlcnic_schedule_work(adapter, qlcnic_fw_poll_work, FW_POLL_DELAY);
	return 0;
=======
	return  __qlcnic_resume(adapter);
>>>>>>> refs/remotes/origin/master
}
#endif

static int qlcnic_open(struct net_device *netdev)
{
	struct qlcnic_adapter *adapter = netdev_priv(netdev);
	int err;

<<<<<<< HEAD
=======
	if (test_bit(__QLCNIC_MAINTENANCE_MODE, &adapter->state)) {
		netdev_err(netdev, "%s: Device is in non-operational state\n",
			   __func__);

		return -EIO;
	}

>>>>>>> refs/remotes/origin/master
	netif_carrier_off(netdev);

	err = qlcnic_attach(adapter);
	if (err)
		return err;

	err = __qlcnic_up(adapter, netdev);
	if (err)
		goto err_out;

<<<<<<< HEAD
	netif_start_queue(netdev);
=======
	netif_tx_start_all_queues(netdev);
>>>>>>> refs/remotes/origin/master

	return 0;

err_out:
	qlcnic_detach(adapter);
	return err;
}

/*
 * qlcnic_close - Disables a network interface entry point
 */
static int qlcnic_close(struct net_device *netdev)
{
	struct qlcnic_adapter *adapter = netdev_priv(netdev);

	__qlcnic_down(adapter, netdev);
<<<<<<< HEAD
=======

>>>>>>> refs/remotes/origin/master
	return 0;
}

void qlcnic_alloc_lb_filters_mem(struct qlcnic_adapter *adapter)
{
	void *head;
	int i;
<<<<<<< HEAD
=======
	struct net_device *netdev = adapter->netdev;
	u32 filter_size = 0;
	u16 act_pci_func = 0;
>>>>>>> refs/remotes/origin/master

	if (adapter->fhash.fmax && adapter->fhash.fhead)
		return;

<<<<<<< HEAD
	spin_lock_init(&adapter->mac_learn_lock);

	head = kcalloc(QLCNIC_LB_MAX_FILTERS, sizeof(struct hlist_head),
								GFP_KERNEL);
	if (!head)
		return;

	adapter->fhash.fmax = QLCNIC_LB_MAX_FILTERS;
	adapter->fhash.fhead = head;

	for (i = 0; i < adapter->fhash.fmax; i++)
		INIT_HLIST_HEAD(&adapter->fhash.fhead[i]);
}

static void qlcnic_free_lb_filters_mem(struct qlcnic_adapter *adapter)
{
	if (adapter->fhash.fmax && adapter->fhash.fhead)
		kfree(adapter->fhash.fhead);

	adapter->fhash.fhead = NULL;
	adapter->fhash.fmax = 0;
}

static void qlcnic_change_filter(struct qlcnic_adapter *adapter,
		u64 uaddr, __le16 vlan_id, struct qlcnic_host_tx_ring *tx_ring)
{
	struct cmd_desc_type0 *hwdesc;
	struct qlcnic_nic_req *req;
	struct qlcnic_mac_req *mac_req;
	struct qlcnic_vlan_req *vlan_req;
	u32 producer;
	u64 word;

	producer = tx_ring->producer;
	hwdesc = &tx_ring->desc_head[tx_ring->producer];

	req = (struct qlcnic_nic_req *)hwdesc;
	memset(req, 0, sizeof(struct qlcnic_nic_req));
	req->qhdr = cpu_to_le64(QLCNIC_REQUEST << 23);

	word = QLCNIC_MAC_EVENT | ((u64)(adapter->portnum) << 16);
	req->req_hdr = cpu_to_le64(word);

	mac_req = (struct qlcnic_mac_req *)&(req->words[0]);
	mac_req->op = vlan_id ? QLCNIC_MAC_VLAN_ADD : QLCNIC_MAC_ADD;
	memcpy(mac_req->mac_addr, &uaddr, ETH_ALEN);

	vlan_req = (struct qlcnic_vlan_req *)&req->words[1];
	vlan_req->vlan_id = vlan_id;

	tx_ring->producer = get_next_index(producer, tx_ring->num_desc);
	smp_mb();
}

#define QLCNIC_MAC_HASH(MAC)\
	((((MAC) & 0x70000) >> 0x10) | (((MAC) & 0x70000000000ULL) >> 0x25))

static void
qlcnic_send_filter(struct qlcnic_adapter *adapter,
		struct qlcnic_host_tx_ring *tx_ring,
		struct cmd_desc_type0 *first_desc,
		struct sk_buff *skb)
{
	struct ethhdr *phdr = (struct ethhdr *)(skb->data);
	struct qlcnic_filter *fil, *tmp_fil;
	struct hlist_node *tmp_hnode, *n;
	struct hlist_head *head;
	u64 src_addr = 0;
	__le16 vlan_id = 0;
	u8 hindex;

	if (!compare_ether_addr(phdr->h_source, adapter->mac_addr))
		return;

	if (adapter->fhash.fnum >= adapter->fhash.fmax)
		return;

	/* Only NPAR capable devices support vlan based learning*/
	if (adapter->flags & QLCNIC_ESWITCH_ENABLED)
		vlan_id = first_desc->vlan_TCI;
	memcpy(&src_addr, phdr->h_source, ETH_ALEN);
	hindex = QLCNIC_MAC_HASH(src_addr) & (QLCNIC_LB_MAX_FILTERS - 1);
	head = &(adapter->fhash.fhead[hindex]);

	hlist_for_each_entry_safe(tmp_fil, tmp_hnode, n, head, fnode) {
		if (!memcmp(tmp_fil->faddr, &src_addr, ETH_ALEN) &&
			    tmp_fil->vlan_id == vlan_id) {

			if (jiffies >
			    (QLCNIC_READD_AGE * HZ + tmp_fil->ftime))
				qlcnic_change_filter(adapter, src_addr, vlan_id,
								tx_ring);
			tmp_fil->ftime = jiffies;
			return;
		}
	}

	fil = kzalloc(sizeof(struct qlcnic_filter), GFP_ATOMIC);
	if (!fil)
		return;

	qlcnic_change_filter(adapter, src_addr, vlan_id, tx_ring);

	fil->ftime = jiffies;
	fil->vlan_id = vlan_id;
	memcpy(fil->faddr, &src_addr, ETH_ALEN);
	spin_lock(&adapter->mac_learn_lock);
	hlist_add_head(&(fil->fnode), head);
	adapter->fhash.fnum++;
	spin_unlock(&adapter->mac_learn_lock);
}

static int
qlcnic_tx_pkt(struct qlcnic_adapter *adapter,
		struct cmd_desc_type0 *first_desc,
		struct sk_buff *skb)
{
	u8 opcode = 0, hdr_len = 0;
	u16 flags = 0, vlan_tci = 0;
	int copied, offset, copy_len;
	struct cmd_desc_type0 *hwdesc;
	struct vlan_ethhdr *vh;
	struct qlcnic_host_tx_ring *tx_ring = adapter->tx_ring;
	u16 protocol = ntohs(skb->protocol);
	u32 producer = tx_ring->producer;

	if (protocol == ETH_P_8021Q) {
		vh = (struct vlan_ethhdr *)skb->data;
		flags = FLAGS_VLAN_TAGGED;
		vlan_tci = vh->h_vlan_TCI;
	} else if (vlan_tx_tag_present(skb)) {
		flags = FLAGS_VLAN_OOB;
		vlan_tci = vlan_tx_tag_get(skb);
	}
	if (unlikely(adapter->pvid)) {
		if (vlan_tci && !(adapter->flags & QLCNIC_TAGGING_ENABLED))
			return -EIO;
		if (vlan_tci && (adapter->flags & QLCNIC_TAGGING_ENABLED))
			goto set_flags;

		flags = FLAGS_VLAN_OOB;
		vlan_tci = adapter->pvid;
	}
set_flags:
	qlcnic_set_tx_vlan_tci(first_desc, vlan_tci);
	qlcnic_set_tx_flags_opcode(first_desc, flags, opcode);

	if (*(skb->data) & BIT_0) {
		flags |= BIT_0;
		memcpy(&first_desc->eth_addr, skb->data, ETH_ALEN);
	}
	opcode = TX_ETHER_PKT;
	if ((adapter->netdev->features & (NETIF_F_TSO | NETIF_F_TSO6)) &&
			skb_shinfo(skb)->gso_size > 0) {

		hdr_len = skb_transport_offset(skb) + tcp_hdrlen(skb);

		first_desc->mss = cpu_to_le16(skb_shinfo(skb)->gso_size);
		first_desc->total_hdr_length = hdr_len;

		opcode = (protocol == ETH_P_IPV6) ? TX_TCP_LSO6 : TX_TCP_LSO;

		/* For LSO, we need to copy the MAC/IP/TCP headers into
		* the descriptor ring */
		copied = 0;
		offset = 2;

		if (flags & FLAGS_VLAN_OOB) {
			first_desc->total_hdr_length += VLAN_HLEN;
			first_desc->tcp_hdr_offset = VLAN_HLEN;
			first_desc->ip_hdr_offset = VLAN_HLEN;
			/* Only in case of TSO on vlan device */
			flags |= FLAGS_VLAN_TAGGED;

			/* Create a TSO vlan header template for firmware */

			hwdesc = &tx_ring->desc_head[producer];
			tx_ring->cmd_buf_arr[producer].skb = NULL;

			copy_len = min((int)sizeof(struct cmd_desc_type0) -
				offset, hdr_len + VLAN_HLEN);

			vh = (struct vlan_ethhdr *)((char *) hwdesc + 2);
			skb_copy_from_linear_data(skb, vh, 12);
			vh->h_vlan_proto = htons(ETH_P_8021Q);
			vh->h_vlan_TCI = htons(vlan_tci);

			skb_copy_from_linear_data_offset(skb, 12,
				(char *)vh + 16, copy_len - 16);

			copied = copy_len - VLAN_HLEN;
			offset = 0;

			producer = get_next_index(producer, tx_ring->num_desc);
		}

		while (copied < hdr_len) {

			copy_len = min((int)sizeof(struct cmd_desc_type0) -
				offset, (hdr_len - copied));

			hwdesc = &tx_ring->desc_head[producer];
			tx_ring->cmd_buf_arr[producer].skb = NULL;

			skb_copy_from_linear_data_offset(skb, copied,
				 (char *) hwdesc + offset, copy_len);

			copied += copy_len;
			offset = 0;

			producer = get_next_index(producer, tx_ring->num_desc);
		}

		tx_ring->producer = producer;
		smp_mb();
		adapter->stats.lso_frames++;

	} else if (skb->ip_summed == CHECKSUM_PARTIAL) {
		u8 l4proto;

		if (protocol == ETH_P_IP) {
			l4proto = ip_hdr(skb)->protocol;

			if (l4proto == IPPROTO_TCP)
				opcode = TX_TCP_PKT;
			else if (l4proto == IPPROTO_UDP)
				opcode = TX_UDP_PKT;
		} else if (protocol == ETH_P_IPV6) {
			l4proto = ipv6_hdr(skb)->nexthdr;

			if (l4proto == IPPROTO_TCP)
				opcode = TX_TCPV6_PKT;
			else if (l4proto == IPPROTO_UDP)
				opcode = TX_UDPV6_PKT;
		}
	}
	first_desc->tcp_hdr_offset += skb_transport_offset(skb);
	first_desc->ip_hdr_offset += skb_network_offset(skb);
	qlcnic_set_tx_flags_opcode(first_desc, flags, opcode);

	return 0;
}

static int
qlcnic_map_tx_skb(struct pci_dev *pdev,
		struct sk_buff *skb, struct qlcnic_cmd_buffer *pbuf)
{
	struct qlcnic_skb_frag *nf;
	struct skb_frag_struct *frag;
	int i, nr_frags;
	dma_addr_t map;

	nr_frags = skb_shinfo(skb)->nr_frags;
	nf = &pbuf->frag_array[0];

	map = pci_map_single(pdev, skb->data,
			skb_headlen(skb), PCI_DMA_TODEVICE);
	if (pci_dma_mapping_error(pdev, map))
		goto out_err;

	nf->dma = map;
	nf->length = skb_headlen(skb);

	for (i = 0; i < nr_frags; i++) {
		frag = &skb_shinfo(skb)->frags[i];
		nf = &pbuf->frag_array[i+1];

		map = skb_frag_dma_map(&pdev->dev, frag, 0, skb_frag_size(frag),
				       DMA_TO_DEVICE);
		if (dma_mapping_error(&pdev->dev, map))
			goto unwind;

		nf->dma = map;
		nf->length = skb_frag_size(frag);
	}

	return 0;

unwind:
	while (--i >= 0) {
		nf = &pbuf->frag_array[i+1];
		pci_unmap_page(pdev, nf->dma, nf->length, PCI_DMA_TODEVICE);
	}

	nf = &pbuf->frag_array[0];
	pci_unmap_single(pdev, nf->dma, skb_headlen(skb), PCI_DMA_TODEVICE);

out_err:
	return -ENOMEM;
}

static void
qlcnic_unmap_buffers(struct pci_dev *pdev, struct sk_buff *skb,
			struct qlcnic_cmd_buffer *pbuf)
{
	struct qlcnic_skb_frag *nf = &pbuf->frag_array[0];
	int nr_frags = skb_shinfo(skb)->nr_frags;
	int i;

	for (i = 0; i < nr_frags; i++) {
		nf = &pbuf->frag_array[i+1];
		pci_unmap_page(pdev, nf->dma, nf->length, PCI_DMA_TODEVICE);
	}

	nf = &pbuf->frag_array[0];
	pci_unmap_single(pdev, nf->dma, skb_headlen(skb), PCI_DMA_TODEVICE);
	pbuf->skb = NULL;
}

static inline void
qlcnic_clear_cmddesc(u64 *desc)
{
	desc[0] = 0ULL;
	desc[2] = 0ULL;
	desc[7] = 0ULL;
}

netdev_tx_t
qlcnic_xmit_frame(struct sk_buff *skb, struct net_device *netdev)
{
	struct qlcnic_adapter *adapter = netdev_priv(netdev);
	struct qlcnic_host_tx_ring *tx_ring = adapter->tx_ring;
	struct qlcnic_cmd_buffer *pbuf;
	struct qlcnic_skb_frag *buffrag;
	struct cmd_desc_type0 *hwdesc, *first_desc;
	struct pci_dev *pdev;
	struct ethhdr *phdr;
	int delta = 0;
	int i, k;

	u32 producer;
	int frag_count;
	u32 num_txd = tx_ring->num_desc;

	if (!test_bit(__QLCNIC_DEV_UP, &adapter->state)) {
		netif_stop_queue(netdev);
		return NETDEV_TX_BUSY;
	}

	if (adapter->flags & QLCNIC_MACSPOOF) {
		phdr = (struct ethhdr *)skb->data;
		if (compare_ether_addr(phdr->h_source,
					adapter->mac_addr))
			goto drop_packet;
	}

	frag_count = skb_shinfo(skb)->nr_frags + 1;
	/* 14 frags supported for normal packet and
	 * 32 frags supported for TSO packet
	 */
	if (!skb_is_gso(skb) && frag_count > QLCNIC_MAX_FRAGS_PER_TX) {

		for (i = 0; i < (frag_count - QLCNIC_MAX_FRAGS_PER_TX); i++)
			delta += skb_frag_size(&skb_shinfo(skb)->frags[i]);

		if (!__pskb_pull_tail(skb, delta))
			goto drop_packet;

		frag_count = 1 + skb_shinfo(skb)->nr_frags;
	}

	if (unlikely(qlcnic_tx_avail(tx_ring) <= TX_STOP_THRESH)) {
		netif_stop_queue(netdev);
		if (qlcnic_tx_avail(tx_ring) > TX_STOP_THRESH)
			netif_start_queue(netdev);
		else {
			adapter->stats.xmit_off++;
			return NETDEV_TX_BUSY;
		}
	}

	producer = tx_ring->producer;
	pbuf = &tx_ring->cmd_buf_arr[producer];

	pdev = adapter->pdev;

	first_desc = hwdesc = &tx_ring->desc_head[producer];
	qlcnic_clear_cmddesc((u64 *)hwdesc);

	if (qlcnic_map_tx_skb(pdev, skb, pbuf)) {
		adapter->stats.tx_dma_map_error++;
		goto drop_packet;
	}

	pbuf->skb = skb;
	pbuf->frag_count = frag_count;

	qlcnic_set_tx_frags_len(first_desc, frag_count, skb->len);
	qlcnic_set_tx_port(first_desc, adapter->portnum);

	for (i = 0; i < frag_count; i++) {

		k = i % 4;

		if ((k == 0) && (i > 0)) {
			/* move to next desc.*/
			producer = get_next_index(producer, num_txd);
			hwdesc = &tx_ring->desc_head[producer];
			qlcnic_clear_cmddesc((u64 *)hwdesc);
			tx_ring->cmd_buf_arr[producer].skb = NULL;
		}

		buffrag = &pbuf->frag_array[i];

		hwdesc->buffer_length[k] = cpu_to_le16(buffrag->length);
		switch (k) {
		case 0:
			hwdesc->addr_buffer1 = cpu_to_le64(buffrag->dma);
			break;
		case 1:
			hwdesc->addr_buffer2 = cpu_to_le64(buffrag->dma);
			break;
		case 2:
			hwdesc->addr_buffer3 = cpu_to_le64(buffrag->dma);
			break;
		case 3:
			hwdesc->addr_buffer4 = cpu_to_le64(buffrag->dma);
			break;
		}
	}

	tx_ring->producer = get_next_index(producer, num_txd);
	smp_mb();

	if (unlikely(qlcnic_tx_pkt(adapter, first_desc, skb)))
		goto unwind_buff;

	if (adapter->mac_learn)
		qlcnic_send_filter(adapter, tx_ring, first_desc, skb);

	adapter->stats.txbytes += skb->len;
	adapter->stats.xmitcalled++;

	qlcnic_update_cmd_producer(adapter, tx_ring);

	return NETDEV_TX_OK;

unwind_buff:
	qlcnic_unmap_buffers(pdev, skb, pbuf);
drop_packet:
	adapter->stats.txdropped++;
	dev_kfree_skb_any(skb);
	return NETDEV_TX_OK;
}

static int qlcnic_check_temp(struct qlcnic_adapter *adapter)
{
	struct net_device *netdev = adapter->netdev;
	u32 temp, temp_state, temp_val;
	int rv = 0;

	temp = QLCRD32(adapter, CRB_TEMP_STATE);
=======
	act_pci_func = adapter->ahw->act_pci_func;
	spin_lock_init(&adapter->mac_learn_lock);
	spin_lock_init(&adapter->rx_mac_learn_lock);

	if (qlcnic_82xx_check(adapter)) {
		filter_size = QLCNIC_LB_MAX_FILTERS;
		adapter->fhash.fbucket_size = QLCNIC_LB_BUCKET_SIZE;
	} else {
		filter_size = QLC_83XX_LB_MAX_FILTERS;
		adapter->fhash.fbucket_size = QLC_83XX_LB_BUCKET_SIZE;
	}

	head = kcalloc(adapter->fhash.fbucket_size,
		       sizeof(struct hlist_head), GFP_ATOMIC);

	if (!head)
		return;

	adapter->fhash.fmax = (filter_size / act_pci_func);
	adapter->fhash.fhead = head;

	netdev_info(netdev, "active nic func = %d, mac filter size=%d\n",
		    act_pci_func, adapter->fhash.fmax);

	for (i = 0; i < adapter->fhash.fbucket_size; i++)
		INIT_HLIST_HEAD(&adapter->fhash.fhead[i]);

	adapter->rx_fhash.fbucket_size = adapter->fhash.fbucket_size;

	head = kcalloc(adapter->rx_fhash.fbucket_size,
		       sizeof(struct hlist_head), GFP_ATOMIC);

	if (!head)
		return;

	adapter->rx_fhash.fmax = (filter_size / act_pci_func);
	adapter->rx_fhash.fhead = head;

	for (i = 0; i < adapter->rx_fhash.fbucket_size; i++)
		INIT_HLIST_HEAD(&adapter->rx_fhash.fhead[i]);
}

static void qlcnic_free_lb_filters_mem(struct qlcnic_adapter *adapter)
{
	if (adapter->fhash.fmax && adapter->fhash.fhead)
		kfree(adapter->fhash.fhead);

	adapter->fhash.fhead = NULL;
	adapter->fhash.fmax = 0;

	if (adapter->rx_fhash.fmax && adapter->rx_fhash.fhead)
		kfree(adapter->rx_fhash.fhead);

	adapter->rx_fhash.fmax = 0;
	adapter->rx_fhash.fhead = NULL;
}

int qlcnic_check_temp(struct qlcnic_adapter *adapter)
{
	struct net_device *netdev = adapter->netdev;
	u32 temp_state, temp_val, temp = 0;
	int rv = 0;

	if (qlcnic_83xx_check(adapter))
		temp = QLCRDX(adapter->ahw, QLC_83XX_ASIC_TEMP);

	if (qlcnic_82xx_check(adapter))
		temp = QLC_SHARED_REG_RD32(adapter, QLCNIC_ASIC_TEMP);
>>>>>>> refs/remotes/origin/master

	temp_state = qlcnic_get_temp_state(temp);
	temp_val = qlcnic_get_temp_val(temp);

	if (temp_state == QLCNIC_TEMP_PANIC) {
		dev_err(&netdev->dev,
		       "Device temperature %d degrees C exceeds"
		       " maximum allowed. Hardware has been shut down.\n",
		       temp_val);
		rv = 1;
	} else if (temp_state == QLCNIC_TEMP_WARN) {
<<<<<<< HEAD
		if (adapter->temp == QLCNIC_TEMP_NORMAL) {
=======
		if (adapter->ahw->temp == QLCNIC_TEMP_NORMAL) {
>>>>>>> refs/remotes/origin/master
			dev_err(&netdev->dev,
			       "Device temperature %d degrees C "
			       "exceeds operating range."
			       " Immediate action needed.\n",
			       temp_val);
		}
	} else {
<<<<<<< HEAD
		if (adapter->temp == QLCNIC_TEMP_WARN) {
=======
		if (adapter->ahw->temp == QLCNIC_TEMP_WARN) {
>>>>>>> refs/remotes/origin/master
			dev_info(&netdev->dev,
			       "Device temperature is now %d degrees C"
			       " in normal range.\n", temp_val);
		}
	}
<<<<<<< HEAD
	adapter->temp = temp_state;
	return rv;
}

void qlcnic_advert_link_change(struct qlcnic_adapter *adapter, int linkup)
{
	struct net_device *netdev = adapter->netdev;

	if (adapter->ahw->linkup && !linkup) {
		netdev_info(netdev, "NIC Link is down\n");
		adapter->ahw->linkup = 0;
		if (netif_running(netdev)) {
			netif_carrier_off(netdev);
			netif_stop_queue(netdev);
		}
	} else if (!adapter->ahw->linkup && linkup) {
		netdev_info(netdev, "NIC Link is up\n");
		adapter->ahw->linkup = 1;
		if (netif_running(netdev)) {
			netif_carrier_on(netdev);
			netif_wake_queue(netdev);
		}
	}
}

static void qlcnic_tx_timeout(struct net_device *netdev)
{
	struct qlcnic_adapter *adapter = netdev_priv(netdev);
=======
	adapter->ahw->temp = temp_state;
	return rv;
}

static void qlcnic_tx_timeout(struct net_device *netdev)
{
	struct qlcnic_adapter *adapter = netdev_priv(netdev);
	struct qlcnic_host_tx_ring *tx_ring;
	int ring;
>>>>>>> refs/remotes/origin/master

	if (test_bit(__QLCNIC_RESETTING, &adapter->state))
		return;

<<<<<<< HEAD
	dev_err(&netdev->dev, "transmit timeout, resetting.\n");

	if (++adapter->tx_timeo_cnt >= QLCNIC_MAX_TX_TIMEOUTS)
		adapter->need_fw_reset = 1;
	else
		adapter->reset_context = 1;
=======
	if (++adapter->tx_timeo_cnt >= QLCNIC_MAX_TX_TIMEOUTS) {
		netdev_info(netdev, "Tx timeout, reset the adapter.\n");
		if (qlcnic_82xx_check(adapter))
			adapter->need_fw_reset = 1;
		else if (qlcnic_83xx_check(adapter))
			qlcnic_83xx_idc_request_reset(adapter,
						      QLCNIC_FORCE_FW_DUMP_KEY);
	} else {
		netdev_info(netdev, "Tx timeout, reset adapter context.\n");
		for (ring = 0; ring < adapter->drv_tx_rings; ring++) {
			tx_ring = &adapter->tx_ring[ring];
			netdev_info(netdev, "Tx ring=%d\n", ring);
			netdev_info(netdev,
				    "crb_intr_mask=%d, producer=%d, sw_consumer=%d, hw_consumer=%d\n",
				    readl(tx_ring->crb_intr_mask),
				    readl(tx_ring->crb_cmd_producer),
				    tx_ring->sw_consumer,
				    le32_to_cpu(*(tx_ring->hw_consumer)));
			netdev_info(netdev,
				    "xmit_finished=%llu, xmit_called=%llu, xmit_on=%llu, xmit_off=%llu\n",
				    tx_ring->tx_stats.xmit_finished,
				    tx_ring->tx_stats.xmit_called,
				    tx_ring->tx_stats.xmit_on,
				    tx_ring->tx_stats.xmit_off);
		}
		adapter->ahw->reset_context = 1;
	}
>>>>>>> refs/remotes/origin/master
}

static struct net_device_stats *qlcnic_get_stats(struct net_device *netdev)
{
	struct qlcnic_adapter *adapter = netdev_priv(netdev);
	struct net_device_stats *stats = &netdev->stats;

<<<<<<< HEAD
=======
	if (test_bit(__QLCNIC_DEV_UP, &adapter->state))
		qlcnic_update_stats(adapter);

>>>>>>> refs/remotes/origin/master
	stats->rx_packets = adapter->stats.rx_pkts + adapter->stats.lro_pkts;
	stats->tx_packets = adapter->stats.xmitfinished;
	stats->rx_bytes = adapter->stats.rxbytes + adapter->stats.lrobytes;
	stats->tx_bytes = adapter->stats.txbytes;
	stats->rx_dropped = adapter->stats.rxdropped;
	stats->tx_dropped = adapter->stats.txdropped;

	return stats;
}

<<<<<<< HEAD
static irqreturn_t qlcnic_clear_legacy_intr(struct qlcnic_adapter *adapter)
=======
irqreturn_t qlcnic_82xx_clear_legacy_intr(struct qlcnic_adapter *adapter)
>>>>>>> refs/remotes/origin/master
{
	u32 status;

	status = readl(adapter->isr_int_vec);

<<<<<<< HEAD
	if (!(status & adapter->int_vec_bit))
=======
	if (!(status & adapter->ahw->int_vec_bit))
>>>>>>> refs/remotes/origin/master
		return IRQ_NONE;

	/* check interrupt state machine, to be sure */
	status = readl(adapter->crb_int_state_reg);
	if (!ISR_LEGACY_INT_TRIGGERED(status))
		return IRQ_NONE;

	writel(0xffffffff, adapter->tgt_status_reg);
	/* read twice to ensure write is flushed */
	readl(adapter->isr_int_vec);
	readl(adapter->isr_int_vec);

	return IRQ_HANDLED;
}

static irqreturn_t qlcnic_tmp_intr(int irq, void *data)
{
	struct qlcnic_host_sds_ring *sds_ring = data;
	struct qlcnic_adapter *adapter = sds_ring->adapter;

	if (adapter->flags & QLCNIC_MSIX_ENABLED)
		goto done;
	else if (adapter->flags & QLCNIC_MSI_ENABLED) {
		writel(0xffffffff, adapter->tgt_status_reg);
		goto done;
	}

	if (qlcnic_clear_legacy_intr(adapter) == IRQ_NONE)
		return IRQ_NONE;

done:
<<<<<<< HEAD
	adapter->diag_cnt++;
=======
	adapter->ahw->diag_cnt++;
>>>>>>> refs/remotes/origin/master
	qlcnic_enable_int(sds_ring);
	return IRQ_HANDLED;
}

static irqreturn_t qlcnic_intr(int irq, void *data)
{
	struct qlcnic_host_sds_ring *sds_ring = data;
	struct qlcnic_adapter *adapter = sds_ring->adapter;

	if (qlcnic_clear_legacy_intr(adapter) == IRQ_NONE)
		return IRQ_NONE;

	napi_schedule(&sds_ring->napi);

	return IRQ_HANDLED;
}

static irqreturn_t qlcnic_msi_intr(int irq, void *data)
{
	struct qlcnic_host_sds_ring *sds_ring = data;
	struct qlcnic_adapter *adapter = sds_ring->adapter;

	/* clear interrupt */
	writel(0xffffffff, adapter->tgt_status_reg);

	napi_schedule(&sds_ring->napi);
	return IRQ_HANDLED;
}

static irqreturn_t qlcnic_msix_intr(int irq, void *data)
{
	struct qlcnic_host_sds_ring *sds_ring = data;

	napi_schedule(&sds_ring->napi);
	return IRQ_HANDLED;
}

<<<<<<< HEAD
static int qlcnic_process_cmd_ring(struct qlcnic_adapter *adapter)
{
	u32 sw_consumer, hw_consumer;
	int count = 0, i;
	struct qlcnic_cmd_buffer *buffer;
	struct pci_dev *pdev = adapter->pdev;
	struct net_device *netdev = adapter->netdev;
	struct qlcnic_skb_frag *frag;
	int done;
	struct qlcnic_host_tx_ring *tx_ring = adapter->tx_ring;

	if (!spin_trylock(&adapter->tx_clean_lock))
		return 1;

	sw_consumer = tx_ring->sw_consumer;
	hw_consumer = le32_to_cpu(*(tx_ring->hw_consumer));

	while (sw_consumer != hw_consumer) {
		buffer = &tx_ring->cmd_buf_arr[sw_consumer];
		if (buffer->skb) {
			frag = &buffer->frag_array[0];
			pci_unmap_single(pdev, frag->dma, frag->length,
					 PCI_DMA_TODEVICE);
			frag->dma = 0ULL;
			for (i = 1; i < buffer->frag_count; i++) {
				frag++;
				pci_unmap_page(pdev, frag->dma, frag->length,
					       PCI_DMA_TODEVICE);
				frag->dma = 0ULL;
			}

			adapter->stats.xmitfinished++;
			dev_kfree_skb_any(buffer->skb);
			buffer->skb = NULL;
		}

		sw_consumer = get_next_index(sw_consumer, tx_ring->num_desc);
		if (++count >= MAX_STATUS_HANDLE)
			break;
	}

	if (count && netif_running(netdev)) {
		tx_ring->sw_consumer = sw_consumer;

		smp_mb();

		if (netif_queue_stopped(netdev) && netif_carrier_ok(netdev)) {
			if (qlcnic_tx_avail(tx_ring) > TX_STOP_THRESH) {
				netif_wake_queue(netdev);
				adapter->stats.xmit_on++;
			}
		}
		adapter->tx_timeo_cnt = 0;
	}
	/*
	 * If everything is freed up to consumer then check if the ring is full
	 * If the ring is full then check if more needs to be freed and
	 * schedule the call back again.
	 *
	 * This happens when there are 2 CPUs. One could be freeing and the
	 * other filling it. If the ring is full when we get out of here and
	 * the card has already interrupted the host then the host can miss the
	 * interrupt.
	 *
	 * There is still a possible race condition and the host could miss an
	 * interrupt. The card has to take care of this.
	 */
	hw_consumer = le32_to_cpu(*(tx_ring->hw_consumer));
	done = (sw_consumer == hw_consumer);
	spin_unlock(&adapter->tx_clean_lock);

	return done;
}

static int qlcnic_poll(struct napi_struct *napi, int budget)
{
	struct qlcnic_host_sds_ring *sds_ring =
		container_of(napi, struct qlcnic_host_sds_ring, napi);

	struct qlcnic_adapter *adapter = sds_ring->adapter;

	int tx_complete;
	int work_done;

	tx_complete = qlcnic_process_cmd_ring(adapter);

	work_done = qlcnic_process_rcv_ring(sds_ring, budget);

	if ((work_done < budget) && tx_complete) {
		napi_complete(&sds_ring->napi);
		if (test_bit(__QLCNIC_DEV_UP, &adapter->state))
			qlcnic_enable_int(sds_ring);
	}

	return work_done;
}

static int qlcnic_rx_poll(struct napi_struct *napi, int budget)
{
	struct qlcnic_host_sds_ring *sds_ring =
		container_of(napi, struct qlcnic_host_sds_ring, napi);

	struct qlcnic_adapter *adapter = sds_ring->adapter;
	int work_done;

	work_done = qlcnic_process_rcv_ring(sds_ring, budget);

	if (work_done < budget) {
		napi_complete(&sds_ring->napi);
		if (test_bit(__QLCNIC_DEV_UP, &adapter->state))
			qlcnic_enable_int(sds_ring);
	}

	return work_done;
}
=======
static irqreturn_t qlcnic_msix_tx_intr(int irq, void *data)
{
	struct qlcnic_host_tx_ring *tx_ring = data;

	napi_schedule(&tx_ring->napi);
	return IRQ_HANDLED;
}
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_NET_POLL_CONTROLLER
static void qlcnic_poll_controller(struct net_device *netdev)
{
	int ring;
	struct qlcnic_host_sds_ring *sds_ring;
	struct qlcnic_adapter *adapter = netdev_priv(netdev);
	struct qlcnic_recv_context *recv_ctx = adapter->recv_ctx;

	disable_irq(adapter->irq);
<<<<<<< HEAD
	for (ring = 0; ring < adapter->max_sds_rings; ring++) {
=======
	for (ring = 0; ring < adapter->drv_sds_rings; ring++) {
>>>>>>> refs/remotes/origin/master
		sds_ring = &recv_ctx->sds_rings[ring];
		qlcnic_intr(adapter->irq, sds_ring);
	}
	enable_irq(adapter->irq);
}
#endif

static void
qlcnic_idc_debug_info(struct qlcnic_adapter *adapter, u8 encoding)
{
	u32 val;

	val = adapter->portnum & 0xf;
	val |= encoding << 7;
	val |= (jiffies - adapter->dev_rst_time) << 8;

<<<<<<< HEAD
	QLCWR32(adapter, QLCNIC_CRB_DRV_SCRATCH, val);
=======
	QLC_SHARED_REG_WR32(adapter, QLCNIC_CRB_DRV_SCRATCH, val);
>>>>>>> refs/remotes/origin/master
	adapter->dev_rst_time = jiffies;
}

static int
qlcnic_set_drv_state(struct qlcnic_adapter *adapter, u8 state)
{
	u32  val;

	WARN_ON(state != QLCNIC_DEV_NEED_RESET &&
			state != QLCNIC_DEV_NEED_QUISCENT);

	if (qlcnic_api_lock(adapter))
		return -EIO;

<<<<<<< HEAD
	val = QLCRD32(adapter, QLCNIC_CRB_DRV_STATE);
=======
	val = QLC_SHARED_REG_RD32(adapter, QLCNIC_CRB_DRV_STATE);
>>>>>>> refs/remotes/origin/master

	if (state == QLCNIC_DEV_NEED_RESET)
		QLC_DEV_SET_RST_RDY(val, adapter->portnum);
	else if (state == QLCNIC_DEV_NEED_QUISCENT)
		QLC_DEV_SET_QSCNT_RDY(val, adapter->portnum);

<<<<<<< HEAD
	QLCWR32(adapter, QLCNIC_CRB_DRV_STATE, val);
=======
	QLC_SHARED_REG_WR32(adapter, QLCNIC_CRB_DRV_STATE, val);
>>>>>>> refs/remotes/origin/master

	qlcnic_api_unlock(adapter);

	return 0;
}

static int
qlcnic_clr_drv_state(struct qlcnic_adapter *adapter)
{
	u32  val;

	if (qlcnic_api_lock(adapter))
		return -EBUSY;

<<<<<<< HEAD
	val = QLCRD32(adapter, QLCNIC_CRB_DRV_STATE);
	QLC_DEV_CLR_RST_QSCNT(val, adapter->portnum);
	QLCWR32(adapter, QLCNIC_CRB_DRV_STATE, val);
=======
	val = QLC_SHARED_REG_RD32(adapter, QLCNIC_CRB_DRV_STATE);
	QLC_DEV_CLR_RST_QSCNT(val, adapter->portnum);
	QLC_SHARED_REG_WR32(adapter, QLCNIC_CRB_DRV_STATE, val);
>>>>>>> refs/remotes/origin/master

	qlcnic_api_unlock(adapter);

	return 0;
}

<<<<<<< HEAD
static void
qlcnic_clr_all_drv_state(struct qlcnic_adapter *adapter, u8 failed)
=======
void qlcnic_clr_all_drv_state(struct qlcnic_adapter *adapter, u8 failed)
>>>>>>> refs/remotes/origin/master
{
	u32  val;

	if (qlcnic_api_lock(adapter))
		goto err;

<<<<<<< HEAD
	val = QLCRD32(adapter, QLCNIC_CRB_DRV_ACTIVE);
	QLC_DEV_CLR_REF_CNT(val, adapter->portnum);
	QLCWR32(adapter, QLCNIC_CRB_DRV_ACTIVE, val);

	if (failed) {
		QLCWR32(adapter, QLCNIC_CRB_DEV_STATE, QLCNIC_DEV_FAILED);
		dev_info(&adapter->pdev->dev,
				"Device state set to Failed. Please Reboot\n");
	} else if (!(val & 0x11111111))
		QLCWR32(adapter, QLCNIC_CRB_DEV_STATE, QLCNIC_DEV_COLD);

	val = QLCRD32(adapter, QLCNIC_CRB_DRV_STATE);
	QLC_DEV_CLR_RST_QSCNT(val, adapter->portnum);
	QLCWR32(adapter, QLCNIC_CRB_DRV_STATE, val);
=======
	val = QLC_SHARED_REG_RD32(adapter, QLCNIC_CRB_DRV_ACTIVE);
	QLC_DEV_CLR_REF_CNT(val, adapter->portnum);
	QLC_SHARED_REG_WR32(adapter, QLCNIC_CRB_DRV_ACTIVE, val);

	if (failed) {
		QLC_SHARED_REG_WR32(adapter, QLCNIC_CRB_DEV_STATE,
				    QLCNIC_DEV_FAILED);
		dev_info(&adapter->pdev->dev,
				"Device state set to Failed. Please Reboot\n");
	} else if (!(val & 0x11111111))
		QLC_SHARED_REG_WR32(adapter, QLCNIC_CRB_DEV_STATE,
				    QLCNIC_DEV_COLD);

	val = QLC_SHARED_REG_RD32(adapter, QLCNIC_CRB_DRV_STATE);
	QLC_DEV_CLR_RST_QSCNT(val, adapter->portnum);
	QLC_SHARED_REG_WR32(adapter, QLCNIC_CRB_DRV_STATE, val);
>>>>>>> refs/remotes/origin/master

	qlcnic_api_unlock(adapter);
err:
	adapter->fw_fail_cnt = 0;
	adapter->flags &= ~QLCNIC_FW_HANG;
	clear_bit(__QLCNIC_START_FW, &adapter->state);
	clear_bit(__QLCNIC_RESETTING, &adapter->state);
}

/* Grab api lock, before checking state */
static int
qlcnic_check_drv_state(struct qlcnic_adapter *adapter)
{
	int act, state, active_mask;
<<<<<<< HEAD

	state = QLCRD32(adapter, QLCNIC_CRB_DRV_STATE);
	act = QLCRD32(adapter, QLCNIC_CRB_DRV_ACTIVE);

	if (adapter->flags & QLCNIC_FW_RESET_OWNER) {
		active_mask = (~(1 << (adapter->ahw->pci_func * 4)));
=======
	struct qlcnic_hardware_context *ahw = adapter->ahw;

	state = QLC_SHARED_REG_RD32(adapter, QLCNIC_CRB_DRV_STATE);
	act = QLC_SHARED_REG_RD32(adapter, QLCNIC_CRB_DRV_ACTIVE);

	if (adapter->flags & QLCNIC_FW_RESET_OWNER) {
		active_mask = (~(1 << (ahw->pci_func * 4)));
>>>>>>> refs/remotes/origin/master
		act = act & active_mask;
	}

	if (((state & 0x11111111) == (act & 0x11111111)) ||
			((act & 0x11111111) == ((state >> 1) & 0x11111111)))
		return 0;
	else
		return 1;
}

static int qlcnic_check_idc_ver(struct qlcnic_adapter *adapter)
{
<<<<<<< HEAD
	u32 val = QLCRD32(adapter, QLCNIC_CRB_DRV_IDC_VER);
=======
	u32 val = QLC_SHARED_REG_RD32(adapter, QLCNIC_CRB_DRV_IDC_VER);
>>>>>>> refs/remotes/origin/master

	if (val != QLCNIC_DRV_IDC_VER) {
		dev_warn(&adapter->pdev->dev, "IDC Version mismatch, driver's"
			" idc ver = %x; reqd = %x\n", QLCNIC_DRV_IDC_VER, val);
	}

	return 0;
}

static int
qlcnic_can_start_firmware(struct qlcnic_adapter *adapter)
{
	u32 val, prev_state;
	u8 dev_init_timeo = adapter->dev_init_timeo;
	u8 portnum = adapter->portnum;
	u8 ret;

	if (test_and_clear_bit(__QLCNIC_START_FW, &adapter->state))
		return 1;

	if (qlcnic_api_lock(adapter))
		return -1;

<<<<<<< HEAD
	val = QLCRD32(adapter, QLCNIC_CRB_DRV_ACTIVE);
	if (!(val & (1 << (portnum * 4)))) {
		QLC_DEV_SET_REF_CNT(val, portnum);
		QLCWR32(adapter, QLCNIC_CRB_DRV_ACTIVE, val);
	}

	prev_state = QLCRD32(adapter, QLCNIC_CRB_DEV_STATE);
=======
	val = QLC_SHARED_REG_RD32(adapter, QLCNIC_CRB_DRV_ACTIVE);
	if (!(val & (1 << (portnum * 4)))) {
		QLC_DEV_SET_REF_CNT(val, portnum);
		QLC_SHARED_REG_WR32(adapter, QLCNIC_CRB_DRV_ACTIVE, val);
	}

	prev_state = QLC_SHARED_REG_RD32(adapter, QLCNIC_CRB_DEV_STATE);
>>>>>>> refs/remotes/origin/master
	QLCDB(adapter, HW, "Device state = %u\n", prev_state);

	switch (prev_state) {
	case QLCNIC_DEV_COLD:
<<<<<<< HEAD
		QLCWR32(adapter, QLCNIC_CRB_DEV_STATE, QLCNIC_DEV_INITIALIZING);
		QLCWR32(adapter, QLCNIC_CRB_DRV_IDC_VER, QLCNIC_DRV_IDC_VER);
=======
		QLC_SHARED_REG_WR32(adapter, QLCNIC_CRB_DEV_STATE,
				    QLCNIC_DEV_INITIALIZING);
		QLC_SHARED_REG_WR32(adapter, QLCNIC_CRB_DRV_IDC_VER,
				    QLCNIC_DRV_IDC_VER);
>>>>>>> refs/remotes/origin/master
		qlcnic_idc_debug_info(adapter, 0);
		qlcnic_api_unlock(adapter);
		return 1;

	case QLCNIC_DEV_READY:
		ret = qlcnic_check_idc_ver(adapter);
		qlcnic_api_unlock(adapter);
		return ret;

	case QLCNIC_DEV_NEED_RESET:
<<<<<<< HEAD
		val = QLCRD32(adapter, QLCNIC_CRB_DRV_STATE);
		QLC_DEV_SET_RST_RDY(val, portnum);
		QLCWR32(adapter, QLCNIC_CRB_DRV_STATE, val);
		break;

	case QLCNIC_DEV_NEED_QUISCENT:
		val = QLCRD32(adapter, QLCNIC_CRB_DRV_STATE);
		QLC_DEV_SET_QSCNT_RDY(val, portnum);
		QLCWR32(adapter, QLCNIC_CRB_DRV_STATE, val);
=======
		val = QLC_SHARED_REG_RD32(adapter, QLCNIC_CRB_DRV_STATE);
		QLC_DEV_SET_RST_RDY(val, portnum);
		QLC_SHARED_REG_WR32(adapter, QLCNIC_CRB_DRV_STATE, val);
		break;

	case QLCNIC_DEV_NEED_QUISCENT:
		val = QLC_SHARED_REG_RD32(adapter, QLCNIC_CRB_DRV_STATE);
		QLC_DEV_SET_QSCNT_RDY(val, portnum);
		QLC_SHARED_REG_WR32(adapter, QLCNIC_CRB_DRV_STATE, val);
>>>>>>> refs/remotes/origin/master
		break;

	case QLCNIC_DEV_FAILED:
		dev_err(&adapter->pdev->dev, "Device in failed state.\n");
		qlcnic_api_unlock(adapter);
		return -1;

	case QLCNIC_DEV_INITIALIZING:
	case QLCNIC_DEV_QUISCENT:
		break;
	}

	qlcnic_api_unlock(adapter);

	do {
		msleep(1000);
<<<<<<< HEAD
		prev_state = QLCRD32(adapter, QLCNIC_CRB_DEV_STATE);
=======
		prev_state = QLC_SHARED_REG_RD32(adapter, QLCNIC_CRB_DEV_STATE);
>>>>>>> refs/remotes/origin/master

		if (prev_state == QLCNIC_DEV_QUISCENT)
			continue;
	} while ((prev_state != QLCNIC_DEV_READY) && --dev_init_timeo);

	if (!dev_init_timeo) {
		dev_err(&adapter->pdev->dev,
			"Waiting for device to initialize timeout\n");
		return -1;
	}

	if (qlcnic_api_lock(adapter))
		return -1;

<<<<<<< HEAD
	val = QLCRD32(adapter, QLCNIC_CRB_DRV_STATE);
	QLC_DEV_CLR_RST_QSCNT(val, portnum);
	QLCWR32(adapter, QLCNIC_CRB_DRV_STATE, val);
=======
	val = QLC_SHARED_REG_RD32(adapter, QLCNIC_CRB_DRV_STATE);
	QLC_DEV_CLR_RST_QSCNT(val, portnum);
	QLC_SHARED_REG_WR32(adapter, QLCNIC_CRB_DRV_STATE, val);
>>>>>>> refs/remotes/origin/master

	ret = qlcnic_check_idc_ver(adapter);
	qlcnic_api_unlock(adapter);

	return ret;
}

static void
qlcnic_fwinit_work(struct work_struct *work)
{
	struct qlcnic_adapter *adapter = container_of(work,
			struct qlcnic_adapter, fw_work.work);
	u32 dev_state = 0xf;
	u32 val;

	if (qlcnic_api_lock(adapter))
		goto err_ret;

<<<<<<< HEAD
	dev_state = QLCRD32(adapter, QLCNIC_CRB_DEV_STATE);
=======
	dev_state = QLC_SHARED_REG_RD32(adapter, QLCNIC_CRB_DEV_STATE);
>>>>>>> refs/remotes/origin/master
	if (dev_state == QLCNIC_DEV_QUISCENT ||
	    dev_state == QLCNIC_DEV_NEED_QUISCENT) {
		qlcnic_api_unlock(adapter);
		qlcnic_schedule_work(adapter, qlcnic_fwinit_work,
						FW_POLL_DELAY * 2);
		return;
	}

<<<<<<< HEAD
	if (adapter->op_mode == QLCNIC_NON_PRIV_FUNC) {
=======
	if (adapter->ahw->op_mode == QLCNIC_NON_PRIV_FUNC) {
>>>>>>> refs/remotes/origin/master
		qlcnic_api_unlock(adapter);
		goto wait_npar;
	}

	if (dev_state == QLCNIC_DEV_INITIALIZING ||
	    dev_state == QLCNIC_DEV_READY) {
		dev_info(&adapter->pdev->dev, "Detected state change from "
				"DEV_NEED_RESET, skipping ack check\n");
		goto skip_ack_check;
	}

	if (adapter->fw_wait_cnt++ > adapter->reset_ack_timeo) {
		dev_info(&adapter->pdev->dev, "Reset:Failed to get ack %d sec\n",
					adapter->reset_ack_timeo);
		goto skip_ack_check;
	}

	if (!qlcnic_check_drv_state(adapter)) {
skip_ack_check:
<<<<<<< HEAD
		dev_state = QLCRD32(adapter, QLCNIC_CRB_DEV_STATE);

		if (dev_state == QLCNIC_DEV_NEED_RESET) {
			QLCWR32(adapter, QLCNIC_CRB_DEV_STATE,
						QLCNIC_DEV_INITIALIZING);
			set_bit(__QLCNIC_START_FW, &adapter->state);
			QLCDB(adapter, DRV, "Restarting fw\n");
			qlcnic_idc_debug_info(adapter, 0);
			val = QLCRD32(adapter, QLCNIC_CRB_DRV_STATE);
			QLC_DEV_SET_RST_RDY(val, adapter->portnum);
			QLCWR32(adapter, QLCNIC_CRB_DRV_STATE, val);
=======
		dev_state = QLC_SHARED_REG_RD32(adapter, QLCNIC_CRB_DEV_STATE);

		if (dev_state == QLCNIC_DEV_NEED_RESET) {
			QLC_SHARED_REG_WR32(adapter, QLCNIC_CRB_DEV_STATE,
					    QLCNIC_DEV_INITIALIZING);
			set_bit(__QLCNIC_START_FW, &adapter->state);
			QLCDB(adapter, DRV, "Restarting fw\n");
			qlcnic_idc_debug_info(adapter, 0);
			val = QLC_SHARED_REG_RD32(adapter,
						  QLCNIC_CRB_DRV_STATE);
			QLC_DEV_SET_RST_RDY(val, adapter->portnum);
			QLC_SHARED_REG_WR32(adapter,
					    QLCNIC_CRB_DRV_STATE, val);
>>>>>>> refs/remotes/origin/master
		}

		qlcnic_api_unlock(adapter);

		rtnl_lock();
<<<<<<< HEAD
		if (adapter->ahw->fw_dump.enable &&
=======
		if (qlcnic_check_fw_dump_state(adapter) &&
>>>>>>> refs/remotes/origin/master
		    (adapter->flags & QLCNIC_FW_RESET_OWNER)) {
			QLCDB(adapter, DRV, "Take FW dump\n");
			qlcnic_dump_fw(adapter);
			adapter->flags |= QLCNIC_FW_HANG;
		}
		rtnl_unlock();

		adapter->flags &= ~QLCNIC_FW_RESET_OWNER;
		if (!adapter->nic_ops->start_firmware(adapter)) {
			qlcnic_schedule_work(adapter, qlcnic_attach_work, 0);
			adapter->fw_wait_cnt = 0;
			return;
		}
		goto err_ret;
	}

	qlcnic_api_unlock(adapter);

wait_npar:
<<<<<<< HEAD
	dev_state = QLCRD32(adapter, QLCNIC_CRB_DEV_STATE);
=======
	dev_state = QLC_SHARED_REG_RD32(adapter, QLCNIC_CRB_DEV_STATE);
>>>>>>> refs/remotes/origin/master
	QLCDB(adapter, HW, "Func waiting: Device state=%u\n", dev_state);

	switch (dev_state) {
	case QLCNIC_DEV_READY:
<<<<<<< HEAD
		if (!adapter->nic_ops->start_firmware(adapter)) {
=======
		if (!qlcnic_start_firmware(adapter)) {
>>>>>>> refs/remotes/origin/master
			qlcnic_schedule_work(adapter, qlcnic_attach_work, 0);
			adapter->fw_wait_cnt = 0;
			return;
		}
	case QLCNIC_DEV_FAILED:
		break;
	default:
		qlcnic_schedule_work(adapter,
			qlcnic_fwinit_work, FW_POLL_DELAY);
		return;
	}

err_ret:
	dev_err(&adapter->pdev->dev, "Fwinit work failed state=%u "
		"fw_wait_cnt=%u\n", dev_state, adapter->fw_wait_cnt);
	netif_device_attach(adapter->netdev);
	qlcnic_clr_all_drv_state(adapter, 0);
}

static void
qlcnic_detach_work(struct work_struct *work)
{
	struct qlcnic_adapter *adapter = container_of(work,
			struct qlcnic_adapter, fw_work.work);
	struct net_device *netdev = adapter->netdev;
	u32 status;

	netif_device_detach(netdev);

	/* Dont grab rtnl lock during Quiscent mode */
	if (adapter->dev_state == QLCNIC_DEV_NEED_QUISCENT) {
		if (netif_running(netdev))
			__qlcnic_down(adapter, netdev);
	} else
		qlcnic_down(adapter, netdev);

<<<<<<< HEAD
	status = QLCRD32(adapter, QLCNIC_PEG_HALT_STATUS1);
=======
	status = QLC_SHARED_REG_RD32(adapter, QLCNIC_PEG_HALT_STATUS1);
>>>>>>> refs/remotes/origin/master

	if (status & QLCNIC_RCODE_FATAL_ERROR) {
		dev_err(&adapter->pdev->dev,
			"Detaching the device: peg halt status1=0x%x\n",
					status);

		if (QLCNIC_FWERROR_CODE(status) == QLCNIC_FWERROR_FAN_FAILURE) {
			dev_err(&adapter->pdev->dev,
			"On board active cooling fan failed. "
				"Device has been halted.\n");
			dev_err(&adapter->pdev->dev,
				"Replace the adapter.\n");
		}

		goto err_ret;
	}

<<<<<<< HEAD
	if (adapter->temp == QLCNIC_TEMP_PANIC) {
		dev_err(&adapter->pdev->dev, "Detaching the device: temp=%d\n",
			adapter->temp);
=======
	if (adapter->ahw->temp == QLCNIC_TEMP_PANIC) {
		dev_err(&adapter->pdev->dev, "Detaching the device: temp=%d\n",
			adapter->ahw->temp);
>>>>>>> refs/remotes/origin/master
		goto err_ret;
	}

	/* Dont ack if this instance is the reset owner */
	if (!(adapter->flags & QLCNIC_FW_RESET_OWNER)) {
		if (qlcnic_set_drv_state(adapter, adapter->dev_state)) {
			dev_err(&adapter->pdev->dev,
				"Failed to set driver state,"
					"detaching the device.\n");
			goto err_ret;
		}
	}

	adapter->fw_wait_cnt = 0;

	qlcnic_schedule_work(adapter, qlcnic_fwinit_work, FW_POLL_DELAY);

	return;

err_ret:
	netif_device_attach(netdev);
	qlcnic_clr_all_drv_state(adapter, 1);
}

/*Transit NPAR state to NON Operational */
static void
qlcnic_set_npar_non_operational(struct qlcnic_adapter *adapter)
{
	u32 state;

<<<<<<< HEAD
	state = QLCRD32(adapter, QLCNIC_CRB_DEV_NPAR_STATE);
=======
	state = QLC_SHARED_REG_RD32(adapter, QLCNIC_CRB_DEV_NPAR_STATE);
>>>>>>> refs/remotes/origin/master
	if (state == QLCNIC_DEV_NPAR_NON_OPER)
		return;

	if (qlcnic_api_lock(adapter))
		return;
<<<<<<< HEAD
	QLCWR32(adapter, QLCNIC_CRB_DEV_NPAR_STATE, QLCNIC_DEV_NPAR_NON_OPER);
	qlcnic_api_unlock(adapter);
}

/*Transit to RESET state from READY state only */
void
qlcnic_dev_request_reset(struct qlcnic_adapter *adapter)
=======
	QLC_SHARED_REG_WR32(adapter, QLCNIC_CRB_DEV_NPAR_STATE,
			    QLCNIC_DEV_NPAR_NON_OPER);
	qlcnic_api_unlock(adapter);
}

void qlcnic_82xx_dev_request_reset(struct qlcnic_adapter *adapter, u32 key)
>>>>>>> refs/remotes/origin/master
{
	u32 state, xg_val = 0, gb_val = 0;

	qlcnic_xg_set_xg0_mask(xg_val);
	qlcnic_xg_set_xg1_mask(xg_val);
	QLCWR32(adapter, QLCNIC_NIU_XG_PAUSE_CTL, xg_val);
	qlcnic_gb_set_gb0_mask(gb_val);
	qlcnic_gb_set_gb1_mask(gb_val);
	qlcnic_gb_set_gb2_mask(gb_val);
	qlcnic_gb_set_gb3_mask(gb_val);
	QLCWR32(adapter, QLCNIC_NIU_GB_PAUSE_CTL, gb_val);
	dev_info(&adapter->pdev->dev, "Pause control frames disabled"
				" on all ports\n");
	adapter->need_fw_reset = 1;
<<<<<<< HEAD
	if (qlcnic_api_lock(adapter))
		return;

	state = QLCRD32(adapter, QLCNIC_CRB_DEV_STATE);

	if (state == QLCNIC_DEV_READY) {
		QLCWR32(adapter, QLCNIC_CRB_DEV_STATE, QLCNIC_DEV_NEED_RESET);
=======

	if (qlcnic_api_lock(adapter))
		return;

	state = QLC_SHARED_REG_RD32(adapter, QLCNIC_CRB_DEV_STATE);

	if (test_bit(__QLCNIC_MAINTENANCE_MODE, &adapter->state)) {
		netdev_err(adapter->netdev, "%s: Device is in non-operational state\n",
			   __func__);
		qlcnic_api_unlock(adapter);

		return;
	}

	if (state == QLCNIC_DEV_READY) {
		QLC_SHARED_REG_WR32(adapter, QLCNIC_CRB_DEV_STATE,
				    QLCNIC_DEV_NEED_RESET);
>>>>>>> refs/remotes/origin/master
		adapter->flags |= QLCNIC_FW_RESET_OWNER;
		QLCDB(adapter, DRV, "NEED_RESET state set\n");
		qlcnic_idc_debug_info(adapter, 0);
	}

<<<<<<< HEAD
	QLCWR32(adapter, QLCNIC_CRB_DEV_NPAR_STATE, QLCNIC_DEV_NPAR_NON_OPER);
=======
	QLC_SHARED_REG_WR32(adapter, QLCNIC_CRB_DEV_NPAR_STATE,
			    QLCNIC_DEV_NPAR_NON_OPER);
>>>>>>> refs/remotes/origin/master
	qlcnic_api_unlock(adapter);
}

/* Transit to NPAR READY state from NPAR NOT READY state */
static void
qlcnic_dev_set_npar_ready(struct qlcnic_adapter *adapter)
{
	if (qlcnic_api_lock(adapter))
		return;

<<<<<<< HEAD
	QLCWR32(adapter, QLCNIC_CRB_DEV_NPAR_STATE, QLCNIC_DEV_NPAR_OPER);
=======
	QLC_SHARED_REG_WR32(adapter, QLCNIC_CRB_DEV_NPAR_STATE,
			    QLCNIC_DEV_NPAR_OPER);
>>>>>>> refs/remotes/origin/master
	QLCDB(adapter, DRV, "NPAR operational state set\n");

	qlcnic_api_unlock(adapter);
}

<<<<<<< HEAD
static void
qlcnic_schedule_work(struct qlcnic_adapter *adapter,
		work_func_t func, int delay)
=======
void qlcnic_schedule_work(struct qlcnic_adapter *adapter,
			  work_func_t func, int delay)
>>>>>>> refs/remotes/origin/master
{
	if (test_bit(__QLCNIC_AER, &adapter->state))
		return;

	INIT_DELAYED_WORK(&adapter->fw_work, func);
<<<<<<< HEAD
	queue_delayed_work(qlcnic_wq, &adapter->fw_work,
					round_jiffies_relative(delay));
}

static void
qlcnic_cancel_fw_work(struct qlcnic_adapter *adapter)
{
	while (test_and_set_bit(__QLCNIC_RESETTING, &adapter->state))
		msleep(10);

	cancel_delayed_work_sync(&adapter->fw_work);
=======
	queue_delayed_work(adapter->qlcnic_wq, &adapter->fw_work,
			   round_jiffies_relative(delay));
>>>>>>> refs/remotes/origin/master
}

static void
qlcnic_attach_work(struct work_struct *work)
{
	struct qlcnic_adapter *adapter = container_of(work,
				struct qlcnic_adapter, fw_work.work);
	struct net_device *netdev = adapter->netdev;
	u32 npar_state;

<<<<<<< HEAD
	if (adapter->op_mode != QLCNIC_MGMT_FUNC) {
		npar_state = QLCRD32(adapter, QLCNIC_CRB_DEV_NPAR_STATE);
=======
	if (adapter->ahw->op_mode != QLCNIC_MGMT_FUNC) {
		npar_state = QLC_SHARED_REG_RD32(adapter,
						 QLCNIC_CRB_DEV_NPAR_STATE);
>>>>>>> refs/remotes/origin/master
		if (adapter->fw_wait_cnt++ > QLCNIC_DEV_NPAR_OPER_TIMEO)
			qlcnic_clr_all_drv_state(adapter, 0);
		else if (npar_state != QLCNIC_DEV_NPAR_OPER)
			qlcnic_schedule_work(adapter, qlcnic_attach_work,
							FW_POLL_DELAY);
		else
			goto attach;
		QLCDB(adapter, DRV, "Waiting for NPAR state to operational\n");
		return;
	}
attach:
<<<<<<< HEAD
=======
	qlcnic_dcb_get_info(adapter->dcb);

>>>>>>> refs/remotes/origin/master
	if (netif_running(netdev)) {
		if (qlcnic_up(adapter, netdev))
			goto done;

		qlcnic_restore_indev_addr(netdev, NETDEV_UP);
	}

done:
	netif_device_attach(netdev);
	adapter->fw_fail_cnt = 0;
	adapter->flags &= ~QLCNIC_FW_HANG;
	clear_bit(__QLCNIC_RESETTING, &adapter->state);
<<<<<<< HEAD
=======
	if (adapter->portnum == 0)
		qlcnic_set_drv_version(adapter);
>>>>>>> refs/remotes/origin/master

	if (!qlcnic_clr_drv_state(adapter))
		qlcnic_schedule_work(adapter, qlcnic_fw_poll_work,
							FW_POLL_DELAY);
}

static int
qlcnic_check_health(struct qlcnic_adapter *adapter)
{
<<<<<<< HEAD
	u32 state = 0, heartbeat;
	u32 peg_status;
=======
	struct qlcnic_hardware_context *ahw = adapter->ahw;
	struct qlcnic_fw_dump *fw_dump = &ahw->fw_dump;
	u32 state = 0, heartbeat;
	u32 peg_status;
	int err = 0;
>>>>>>> refs/remotes/origin/master

	if (qlcnic_check_temp(adapter))
		goto detach;

	if (adapter->need_fw_reset)
<<<<<<< HEAD
		qlcnic_dev_request_reset(adapter);

	state = QLCRD32(adapter, QLCNIC_CRB_DEV_STATE);
=======
		qlcnic_dev_request_reset(adapter, 0);

	state = QLC_SHARED_REG_RD32(adapter, QLCNIC_CRB_DEV_STATE);
>>>>>>> refs/remotes/origin/master
	if (state == QLCNIC_DEV_NEED_RESET) {
		qlcnic_set_npar_non_operational(adapter);
		adapter->need_fw_reset = 1;
	} else if (state == QLCNIC_DEV_NEED_QUISCENT)
		goto detach;

<<<<<<< HEAD
	heartbeat = QLCRD32(adapter, QLCNIC_PEG_ALIVE_COUNTER);
=======
	heartbeat = QLC_SHARED_REG_RD32(adapter, QLCNIC_PEG_ALIVE_COUNTER);
>>>>>>> refs/remotes/origin/master
	if (heartbeat != adapter->heartbeat) {
		adapter->heartbeat = heartbeat;
		adapter->fw_fail_cnt = 0;
		if (adapter->need_fw_reset)
			goto detach;

<<<<<<< HEAD
		if (adapter->reset_context && auto_fw_reset) {
			qlcnic_reset_hw_context(adapter);
			adapter->netdev->trans_start = jiffies;
		}
=======
		if (ahw->reset_context && qlcnic_auto_fw_reset)
			qlcnic_reset_hw_context(adapter);
>>>>>>> refs/remotes/origin/master

		return 0;
	}

	if (++adapter->fw_fail_cnt < FW_FAIL_THRESH)
		return 0;

	adapter->flags |= QLCNIC_FW_HANG;

<<<<<<< HEAD
	qlcnic_dev_request_reset(adapter);

	if (auto_fw_reset)
		clear_bit(__QLCNIC_FW_ATTACHED, &adapter->state);

	dev_err(&adapter->pdev->dev, "firmware hang detected\n");
=======
	qlcnic_dev_request_reset(adapter, 0);

	if (qlcnic_auto_fw_reset)
		clear_bit(__QLCNIC_FW_ATTACHED, &adapter->state);

	dev_err(&adapter->pdev->dev, "firmware hang detected\n");
	peg_status = QLC_SHARED_REG_RD32(adapter, QLCNIC_PEG_HALT_STATUS1);
>>>>>>> refs/remotes/origin/master
	dev_err(&adapter->pdev->dev, "Dumping hw/fw registers\n"
			"PEG_HALT_STATUS1: 0x%x, PEG_HALT_STATUS2: 0x%x,\n"
			"PEG_NET_0_PC: 0x%x, PEG_NET_1_PC: 0x%x,\n"
			"PEG_NET_2_PC: 0x%x, PEG_NET_3_PC: 0x%x,\n"
			"PEG_NET_4_PC: 0x%x\n",
<<<<<<< HEAD
			QLCRD32(adapter, QLCNIC_PEG_HALT_STATUS1),
			QLCRD32(adapter, QLCNIC_PEG_HALT_STATUS2),
			QLCRD32(adapter, QLCNIC_CRB_PEG_NET_0 + 0x3c),
			QLCRD32(adapter, QLCNIC_CRB_PEG_NET_1 + 0x3c),
			QLCRD32(adapter, QLCNIC_CRB_PEG_NET_2 + 0x3c),
			QLCRD32(adapter, QLCNIC_CRB_PEG_NET_3 + 0x3c),
			QLCRD32(adapter, QLCNIC_CRB_PEG_NET_4 + 0x3c));
	peg_status = QLCRD32(adapter, QLCNIC_PEG_HALT_STATUS1);
=======
			peg_status,
			QLC_SHARED_REG_RD32(adapter, QLCNIC_PEG_HALT_STATUS2),
			QLCRD32(adapter, QLCNIC_CRB_PEG_NET_0 + 0x3c, &err),
			QLCRD32(adapter, QLCNIC_CRB_PEG_NET_1 + 0x3c, &err),
			QLCRD32(adapter, QLCNIC_CRB_PEG_NET_2 + 0x3c, &err),
			QLCRD32(adapter, QLCNIC_CRB_PEG_NET_3 + 0x3c, &err),
			QLCRD32(adapter, QLCNIC_CRB_PEG_NET_4 + 0x3c, &err));
>>>>>>> refs/remotes/origin/master
	if (QLCNIC_FWERROR_CODE(peg_status) == 0x67)
		dev_err(&adapter->pdev->dev,
			"Firmware aborted with error code 0x00006700. "
				"Device is being reset.\n");
detach:
	adapter->dev_state = (state == QLCNIC_DEV_NEED_QUISCENT) ? state :
		QLCNIC_DEV_NEED_RESET;

<<<<<<< HEAD
	if (auto_fw_reset &&
		!test_and_set_bit(__QLCNIC_RESETTING, &adapter->state)) {

		qlcnic_schedule_work(adapter, qlcnic_detach_work, 0);
		QLCDB(adapter, DRV, "fw recovery scheduled.\n");
=======
	if (qlcnic_auto_fw_reset && !test_and_set_bit(__QLCNIC_RESETTING,
						      &adapter->state)) {

		qlcnic_schedule_work(adapter, qlcnic_detach_work, 0);
		QLCDB(adapter, DRV, "fw recovery scheduled.\n");
	} else if (!qlcnic_auto_fw_reset && fw_dump->enable &&
		   adapter->flags & QLCNIC_FW_RESET_OWNER) {
		qlcnic_dump_fw(adapter);
>>>>>>> refs/remotes/origin/master
	}

	return 1;
}

<<<<<<< HEAD
static void
qlcnic_fw_poll_work(struct work_struct *work)
=======
void qlcnic_fw_poll_work(struct work_struct *work)
>>>>>>> refs/remotes/origin/master
{
	struct qlcnic_adapter *adapter = container_of(work,
				struct qlcnic_adapter, fw_work.work);

	if (test_bit(__QLCNIC_RESETTING, &adapter->state))
		goto reschedule;


	if (qlcnic_check_health(adapter))
		return;

	if (adapter->fhash.fnum)
		qlcnic_prune_lb_filters(adapter);

reschedule:
	qlcnic_schedule_work(adapter, qlcnic_fw_poll_work, FW_POLL_DELAY);
}

static int qlcnic_is_first_func(struct pci_dev *pdev)
{
	struct pci_dev *oth_pdev;
	int val = pdev->devfn;

	while (val-- > 0) {
		oth_pdev = pci_get_domain_bus_and_slot(pci_domain_nr
			(pdev->bus), pdev->bus->number,
			PCI_DEVFN(PCI_SLOT(pdev->devfn), val));
		if (!oth_pdev)
			continue;

		if (oth_pdev->current_state != PCI_D3cold) {
<<<<<<< HEAD
			pci_dev_put(oth_pdev);
			return 0;
		}
		pci_dev_put(oth_pdev);
	}
	return 1;
}

static int qlcnic_attach_func(struct pci_dev *pdev)
{
	int err, first_func;
	struct qlcnic_adapter *adapter = pci_get_drvdata(pdev);
	struct net_device *netdev = adapter->netdev;

	pdev->error_state = pci_channel_io_normal;

	err = pci_enable_device(pdev);
	if (err)
		return err;

	pci_set_power_state(pdev, PCI_D0);
	pci_set_master(pdev);
	pci_restore_state(pdev);

	first_func = qlcnic_is_first_func(pdev);

	if (qlcnic_api_lock(adapter))
		return -EINVAL;

	if (adapter->op_mode != QLCNIC_NON_PRIV_FUNC && first_func) {
		adapter->need_fw_reset = 1;
		set_bit(__QLCNIC_START_FW, &adapter->state);
		QLCWR32(adapter, QLCNIC_CRB_DEV_STATE, QLCNIC_DEV_INITIALIZING);
		QLCDB(adapter, DRV, "Restarting fw\n");
	}
	qlcnic_api_unlock(adapter);

	err = adapter->nic_ops->start_firmware(adapter);
	if (err)
		return err;

	qlcnic_clr_drv_state(adapter);
	qlcnic_setup_intr(adapter);

	if (netif_running(netdev)) {
		err = qlcnic_attach(adapter);
		if (err) {
			qlcnic_clr_all_drv_state(adapter, 1);
			clear_bit(__QLCNIC_AER, &adapter->state);
			netif_device_attach(netdev);
			return err;
		}

		err = qlcnic_up(adapter, netdev);
		if (err)
			goto done;

		qlcnic_restore_indev_addr(netdev, NETDEV_UP);
	}
 done:
	netif_device_attach(netdev);
	return err;
}

static pci_ers_result_t qlcnic_io_error_detected(struct pci_dev *pdev,
						pci_channel_state_t state)
{
	struct qlcnic_adapter *adapter = pci_get_drvdata(pdev);
	struct net_device *netdev = adapter->netdev;

	if (state == pci_channel_io_perm_failure)
		return PCI_ERS_RESULT_DISCONNECT;

	if (state == pci_channel_io_normal)
		return PCI_ERS_RESULT_RECOVERED;

	set_bit(__QLCNIC_AER, &adapter->state);
	netif_device_detach(netdev);

	cancel_delayed_work_sync(&adapter->fw_work);

	if (netif_running(netdev))
		qlcnic_down(adapter, netdev);

	qlcnic_detach(adapter);
	qlcnic_teardown_intr(adapter);

	clear_bit(__QLCNIC_RESETTING, &adapter->state);

	pci_save_state(pdev);
	pci_disable_device(pdev);

	return PCI_ERS_RESULT_NEED_RESET;
}

static pci_ers_result_t qlcnic_io_slot_reset(struct pci_dev *pdev)
{
	return qlcnic_attach_func(pdev) ? PCI_ERS_RESULT_DISCONNECT :
				PCI_ERS_RESULT_RECOVERED;
}

static void qlcnic_io_resume(struct pci_dev *pdev)
{
	struct qlcnic_adapter *adapter = pci_get_drvdata(pdev);

	pci_cleanup_aer_uncorrect_error_status(pdev);

	if (QLCRD32(adapter, QLCNIC_CRB_DEV_STATE) == QLCNIC_DEV_READY &&
	    test_and_clear_bit(__QLCNIC_AER, &adapter->state))
		qlcnic_schedule_work(adapter, qlcnic_fw_poll_work,
						FW_POLL_DELAY);
}

static int
qlcnicvf_start_firmware(struct qlcnic_adapter *adapter)
{
	int err;

	err = qlcnic_can_start_firmware(adapter);
	if (err)
		return err;

	err = qlcnic_check_npar_opertional(adapter);
	if (err)
		return err;

	err = qlcnic_initialize_nic(adapter);
	if (err)
		return err;

	qlcnic_check_options(adapter);

	err = qlcnic_set_eswitch_port_config(adapter);
	if (err)
		return err;

	adapter->need_fw_reset = 0;

	return err;
}

static int
qlcnicvf_config_bridged_mode(struct qlcnic_adapter *adapter, u32 enable)
{
	return -EOPNOTSUPP;
}

static int
qlcnicvf_config_led(struct qlcnic_adapter *adapter, u32 state, u32 rate)
{
	return -EOPNOTSUPP;
}

static ssize_t
qlcnic_store_bridged_mode(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t len)
{
	struct qlcnic_adapter *adapter = dev_get_drvdata(dev);
	unsigned long new;
	int ret = -EINVAL;

	if (!(adapter->capabilities & QLCNIC_FW_CAPABILITY_BDG))
		goto err_out;

	if (!test_bit(__QLCNIC_DEV_UP, &adapter->state))
		goto err_out;

	if (strict_strtoul(buf, 2, &new))
		goto err_out;

	if (!adapter->nic_ops->config_bridged_mode(adapter, !!new))
		ret = len;

err_out:
	return ret;
}

static ssize_t
qlcnic_show_bridged_mode(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct qlcnic_adapter *adapter = dev_get_drvdata(dev);
	int bridged_mode = 0;

	if (adapter->capabilities & QLCNIC_FW_CAPABILITY_BDG)
		bridged_mode = !!(adapter->flags & QLCNIC_BRIDGE_ENABLED);

	return sprintf(buf, "%d\n", bridged_mode);
}

static struct device_attribute dev_attr_bridged_mode = {
       .attr = {.name = "bridged_mode", .mode = (S_IRUGO | S_IWUSR)},
       .show = qlcnic_show_bridged_mode,
       .store = qlcnic_store_bridged_mode,
};

static ssize_t
qlcnic_store_diag_mode(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t len)
{
	struct qlcnic_adapter *adapter = dev_get_drvdata(dev);
	unsigned long new;

	if (strict_strtoul(buf, 2, &new))
		return -EINVAL;

	if (!!new != !!(adapter->flags & QLCNIC_DIAG_ENABLED))
		adapter->flags ^= QLCNIC_DIAG_ENABLED;

	return len;
}

static ssize_t
qlcnic_show_diag_mode(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct qlcnic_adapter *adapter = dev_get_drvdata(dev);

	return sprintf(buf, "%d\n",
			!!(adapter->flags & QLCNIC_DIAG_ENABLED));
}

static struct device_attribute dev_attr_diag_mode = {
	.attr = {.name = "diag_mode", .mode = (S_IRUGO | S_IWUSR)},
	.show = qlcnic_show_diag_mode,
	.store = qlcnic_store_diag_mode,
};

int qlcnic_validate_max_rss(struct net_device *netdev, u8 max_hw, u8 val)
{
	if (!use_msi_x && !use_msi) {
		netdev_info(netdev, "no msix or msi support, hence no rss\n");
		return -EINVAL;
	}

	if ((val > max_hw) || (val <  2) || !is_power_of_2(val)) {
		netdev_info(netdev, "rss_ring valid range [2 - %x] in "
			" powers of 2\n", max_hw);
		return -EINVAL;
	}
	return 0;

}

int qlcnic_set_max_rss(struct qlcnic_adapter *adapter, u8 data)
{
	struct net_device *netdev = adapter->netdev;
	int err = 0;

	if (test_and_set_bit(__QLCNIC_RESETTING, &adapter->state))
		return -EBUSY;

	netif_device_detach(netdev);
	if (netif_running(netdev))
		__qlcnic_down(adapter, netdev);
	qlcnic_detach(adapter);
	qlcnic_teardown_intr(adapter);

	if (qlcnic_enable_msix(adapter, data)) {
		netdev_info(netdev, "failed setting max_rss; rss disabled\n");
		qlcnic_enable_msi_legacy(adapter);
	}

	if (netif_running(netdev)) {
		err = qlcnic_attach(adapter);
		if (err)
			goto done;
		err = __qlcnic_up(adapter, netdev);
		if (err)
			goto done;
		qlcnic_restore_indev_addr(netdev, NETDEV_UP);
	}
 done:
	netif_device_attach(netdev);
	clear_bit(__QLCNIC_RESETTING, &adapter->state);
	return err;
}

static int
qlcnic_validate_beacon(struct qlcnic_adapter *adapter, u16 beacon, u8 *state,
			u8 *rate)
{
	*rate = LSB(beacon);
	*state = MSB(beacon);

	QLCDB(adapter, DRV, "rate %x state %x\n", *rate, *state);

	if (!*state) {
		*rate = __QLCNIC_MAX_LED_RATE;
		return 0;
	} else if (*state > __QLCNIC_MAX_LED_STATE)
		return -EINVAL;

	if ((!*rate) || (*rate > __QLCNIC_MAX_LED_RATE))
		return -EINVAL;

	return 0;
}

static ssize_t
qlcnic_store_beacon(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t len)
{
	struct qlcnic_adapter *adapter = dev_get_drvdata(dev);
	int max_sds_rings = adapter->max_sds_rings;
	u16 beacon;
	u8 b_state, b_rate;
	int err;

	if (adapter->op_mode == QLCNIC_NON_PRIV_FUNC) {
		dev_warn(dev, "LED test not supported for non "
				"privilege function\n");
		return -EOPNOTSUPP;
	}

	if (len != sizeof(u16))
		return QL_STATUS_INVALID_PARAM;

	memcpy(&beacon, buf, sizeof(u16));
	err = qlcnic_validate_beacon(adapter, beacon, &b_state, &b_rate);
	if (err)
		return err;

	if (adapter->ahw->beacon_state == b_state)
		return len;

	rtnl_lock();

	if (!adapter->ahw->beacon_state)
		if (test_and_set_bit(__QLCNIC_LED_ENABLE, &adapter->state)) {
			rtnl_unlock();
			return -EBUSY;
		}

	if (test_bit(__QLCNIC_RESETTING, &adapter->state)) {
		err = -EIO;
		goto out;
	}

	if (!test_bit(__QLCNIC_DEV_UP, &adapter->state)) {
		err = qlcnic_diag_alloc_res(adapter->netdev, QLCNIC_LED_TEST);
		if (err)
			goto out;
		set_bit(__QLCNIC_DIAG_RES_ALLOC, &adapter->state);
	}

	err = qlcnic_config_led(adapter, b_state, b_rate);

	if (!err) {
		err = len;
		adapter->ahw->beacon_state = b_state;
	}

	if (test_and_clear_bit(__QLCNIC_DIAG_RES_ALLOC, &adapter->state))
		qlcnic_diag_free_res(adapter->netdev, max_sds_rings);

 out:
	if (!adapter->ahw->beacon_state)
		clear_bit(__QLCNIC_LED_ENABLE, &adapter->state);
	rtnl_unlock();

	return err;
}

static ssize_t
qlcnic_show_beacon(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct qlcnic_adapter *adapter = dev_get_drvdata(dev);

	return sprintf(buf, "%d\n", adapter->ahw->beacon_state);
}

static struct device_attribute dev_attr_beacon = {
	.attr = {.name = "beacon", .mode = (S_IRUGO | S_IWUSR)},
	.show = qlcnic_show_beacon,
	.store = qlcnic_store_beacon,
};

static int
qlcnic_sysfs_validate_crb(struct qlcnic_adapter *adapter,
		loff_t offset, size_t size)
{
	size_t crb_size = 4;

	if (!(adapter->flags & QLCNIC_DIAG_ENABLED))
		return -EIO;

	if (offset < QLCNIC_PCI_CRBSPACE) {
		if (ADDR_IN_RANGE(offset, QLCNIC_PCI_CAMQM,
					QLCNIC_PCI_CAMQM_END))
			crb_size = 8;
		else
			return -EINVAL;
	}

	if ((size != crb_size) || (offset & (crb_size-1)))
		return  -EINVAL;

	return 0;
}

static ssize_t
qlcnic_sysfs_read_crb(struct file *filp, struct kobject *kobj,
		struct bin_attribute *attr,
		char *buf, loff_t offset, size_t size)
{
	struct device *dev = container_of(kobj, struct device, kobj);
	struct qlcnic_adapter *adapter = dev_get_drvdata(dev);
	u32 data;
	u64 qmdata;
	int ret;

	ret = qlcnic_sysfs_validate_crb(adapter, offset, size);
	if (ret != 0)
		return ret;

	if (ADDR_IN_RANGE(offset, QLCNIC_PCI_CAMQM, QLCNIC_PCI_CAMQM_END)) {
		qlcnic_pci_camqm_read_2M(adapter, offset, &qmdata);
		memcpy(buf, &qmdata, size);
	} else {
		data = QLCRD32(adapter, offset);
		memcpy(buf, &data, size);
	}
	return size;
}

static ssize_t
qlcnic_sysfs_write_crb(struct file *filp, struct kobject *kobj,
		struct bin_attribute *attr,
		char *buf, loff_t offset, size_t size)
{
	struct device *dev = container_of(kobj, struct device, kobj);
	struct qlcnic_adapter *adapter = dev_get_drvdata(dev);
	u32 data;
	u64 qmdata;
	int ret;

	ret = qlcnic_sysfs_validate_crb(adapter, offset, size);
	if (ret != 0)
		return ret;

	if (ADDR_IN_RANGE(offset, QLCNIC_PCI_CAMQM, QLCNIC_PCI_CAMQM_END)) {
		memcpy(&qmdata, buf, size);
		qlcnic_pci_camqm_write_2M(adapter, offset, qmdata);
	} else {
		memcpy(&data, buf, size);
		QLCWR32(adapter, offset, data);
	}
	return size;
}

static int
qlcnic_sysfs_validate_mem(struct qlcnic_adapter *adapter,
		loff_t offset, size_t size)
{
	if (!(adapter->flags & QLCNIC_DIAG_ENABLED))
		return -EIO;

	if ((size != 8) || (offset & 0x7))
		return  -EIO;

	return 0;
}

static ssize_t
qlcnic_sysfs_read_mem(struct file *filp, struct kobject *kobj,
		struct bin_attribute *attr,
		char *buf, loff_t offset, size_t size)
{
	struct device *dev = container_of(kobj, struct device, kobj);
	struct qlcnic_adapter *adapter = dev_get_drvdata(dev);
	u64 data;
	int ret;

	ret = qlcnic_sysfs_validate_mem(adapter, offset, size);
	if (ret != 0)
		return ret;

	if (qlcnic_pci_mem_read_2M(adapter, offset, &data))
		return -EIO;

	memcpy(buf, &data, size);

	return size;
}

static ssize_t
qlcnic_sysfs_write_mem(struct file *filp, struct kobject *kobj,
		struct bin_attribute *attr,
		char *buf, loff_t offset, size_t size)
{
	struct device *dev = container_of(kobj, struct device, kobj);
	struct qlcnic_adapter *adapter = dev_get_drvdata(dev);
	u64 data;
	int ret;

	ret = qlcnic_sysfs_validate_mem(adapter, offset, size);
	if (ret != 0)
		return ret;

	memcpy(&data, buf, size);

	if (qlcnic_pci_mem_write_2M(adapter, offset, data))
		return -EIO;

	return size;
}

static struct bin_attribute bin_attr_crb = {
	.attr = {.name = "crb", .mode = (S_IRUGO | S_IWUSR)},
	.size = 0,
	.read = qlcnic_sysfs_read_crb,
	.write = qlcnic_sysfs_write_crb,
};

static struct bin_attribute bin_attr_mem = {
	.attr = {.name = "mem", .mode = (S_IRUGO | S_IWUSR)},
	.size = 0,
	.read = qlcnic_sysfs_read_mem,
	.write = qlcnic_sysfs_write_mem,
};

static int
validate_pm_config(struct qlcnic_adapter *adapter,
			struct qlcnic_pm_func_cfg *pm_cfg, int count)
{

	u8 src_pci_func, s_esw_id, d_esw_id;
	u8 dest_pci_func;
	int i;

	for (i = 0; i < count; i++) {
		src_pci_func = pm_cfg[i].pci_func;
		dest_pci_func = pm_cfg[i].dest_npar;
		if (src_pci_func >= QLCNIC_MAX_PCI_FUNC
				|| dest_pci_func >= QLCNIC_MAX_PCI_FUNC)
			return QL_STATUS_INVALID_PARAM;

		if (adapter->npars[src_pci_func].type != QLCNIC_TYPE_NIC)
			return QL_STATUS_INVALID_PARAM;

		if (adapter->npars[dest_pci_func].type != QLCNIC_TYPE_NIC)
			return QL_STATUS_INVALID_PARAM;

		s_esw_id = adapter->npars[src_pci_func].phy_port;
		d_esw_id = adapter->npars[dest_pci_func].phy_port;

		if (s_esw_id != d_esw_id)
			return QL_STATUS_INVALID_PARAM;

	}
	return 0;

}

static ssize_t
qlcnic_sysfs_write_pm_config(struct file *filp, struct kobject *kobj,
	struct bin_attribute *attr, char *buf, loff_t offset, size_t size)
{
	struct device *dev = container_of(kobj, struct device, kobj);
	struct qlcnic_adapter *adapter = dev_get_drvdata(dev);
	struct qlcnic_pm_func_cfg *pm_cfg;
	u32 id, action, pci_func;
	int count, rem, i, ret;

	count	= size / sizeof(struct qlcnic_pm_func_cfg);
	rem	= size % sizeof(struct qlcnic_pm_func_cfg);
	if (rem)
		return QL_STATUS_INVALID_PARAM;

	pm_cfg = (struct qlcnic_pm_func_cfg *) buf;

	ret = validate_pm_config(adapter, pm_cfg, count);
	if (ret)
		return ret;
	for (i = 0; i < count; i++) {
		pci_func = pm_cfg[i].pci_func;
		action = !!pm_cfg[i].action;
		id = adapter->npars[pci_func].phy_port;
		ret = qlcnic_config_port_mirroring(adapter, id,
						action, pci_func);
		if (ret)
			return ret;
	}

	for (i = 0; i < count; i++) {
		pci_func = pm_cfg[i].pci_func;
		id = adapter->npars[pci_func].phy_port;
		adapter->npars[pci_func].enable_pm = !!pm_cfg[i].action;
		adapter->npars[pci_func].dest_npar = id;
	}
	return size;
}

static ssize_t
qlcnic_sysfs_read_pm_config(struct file *filp, struct kobject *kobj,
	struct bin_attribute *attr, char *buf, loff_t offset, size_t size)
{
	struct device *dev = container_of(kobj, struct device, kobj);
	struct qlcnic_adapter *adapter = dev_get_drvdata(dev);
	struct qlcnic_pm_func_cfg pm_cfg[QLCNIC_MAX_PCI_FUNC];
	int i;

	if (size != sizeof(pm_cfg))
		return QL_STATUS_INVALID_PARAM;

	for (i = 0; i < QLCNIC_MAX_PCI_FUNC; i++) {
		if (adapter->npars[i].type != QLCNIC_TYPE_NIC)
			continue;
		pm_cfg[i].action = adapter->npars[i].enable_pm;
		pm_cfg[i].dest_npar = 0;
		pm_cfg[i].pci_func = i;
	}
	memcpy(buf, &pm_cfg, size);

	return size;
}

static int
validate_esw_config(struct qlcnic_adapter *adapter,
	struct qlcnic_esw_func_cfg *esw_cfg, int count)
{
	u32 op_mode;
	u8 pci_func;
	int i;

	op_mode = readl(adapter->ahw->pci_base0 + QLCNIC_DRV_OP_MODE);

	for (i = 0; i < count; i++) {
		pci_func = esw_cfg[i].pci_func;
		if (pci_func >= QLCNIC_MAX_PCI_FUNC)
			return QL_STATUS_INVALID_PARAM;

		if (adapter->op_mode == QLCNIC_MGMT_FUNC)
			if (adapter->npars[pci_func].type != QLCNIC_TYPE_NIC)
				return QL_STATUS_INVALID_PARAM;

		switch (esw_cfg[i].op_mode) {
		case QLCNIC_PORT_DEFAULTS:
			if (QLC_DEV_GET_DRV(op_mode, pci_func) !=
						QLCNIC_NON_PRIV_FUNC) {
				if (esw_cfg[i].mac_anti_spoof != 0)
					return QL_STATUS_INVALID_PARAM;
				if (esw_cfg[i].mac_override != 1)
					return QL_STATUS_INVALID_PARAM;
				if (esw_cfg[i].promisc_mode != 1)
					return QL_STATUS_INVALID_PARAM;
			}
			break;
		case QLCNIC_ADD_VLAN:
			if (!IS_VALID_VLAN(esw_cfg[i].vlan_id))
				return QL_STATUS_INVALID_PARAM;
			if (!esw_cfg[i].op_type)
				return QL_STATUS_INVALID_PARAM;
			break;
		case QLCNIC_DEL_VLAN:
			if (!esw_cfg[i].op_type)
				return QL_STATUS_INVALID_PARAM;
			break;
		default:
			return QL_STATUS_INVALID_PARAM;
		}
	}
	return 0;
}

static ssize_t
qlcnic_sysfs_write_esw_config(struct file *file, struct kobject *kobj,
	struct bin_attribute *attr, char *buf, loff_t offset, size_t size)
{
	struct device *dev = container_of(kobj, struct device, kobj);
	struct qlcnic_adapter *adapter = dev_get_drvdata(dev);
	struct qlcnic_esw_func_cfg *esw_cfg;
	struct qlcnic_npar_info *npar;
	int count, rem, i, ret;
	u8 pci_func, op_mode = 0;

	count	= size / sizeof(struct qlcnic_esw_func_cfg);
	rem	= size % sizeof(struct qlcnic_esw_func_cfg);
	if (rem)
		return QL_STATUS_INVALID_PARAM;

	esw_cfg = (struct qlcnic_esw_func_cfg *) buf;
	ret = validate_esw_config(adapter, esw_cfg, count);
	if (ret)
		return ret;

	for (i = 0; i < count; i++) {
		if (adapter->op_mode == QLCNIC_MGMT_FUNC)
			if (qlcnic_config_switch_port(adapter, &esw_cfg[i]))
				return QL_STATUS_INVALID_PARAM;

		if (adapter->ahw->pci_func != esw_cfg[i].pci_func)
			continue;

		op_mode = esw_cfg[i].op_mode;
		qlcnic_get_eswitch_port_config(adapter, &esw_cfg[i]);
		esw_cfg[i].op_mode = op_mode;
		esw_cfg[i].pci_func = adapter->ahw->pci_func;

		switch (esw_cfg[i].op_mode) {
		case QLCNIC_PORT_DEFAULTS:
			qlcnic_set_eswitch_port_features(adapter, &esw_cfg[i]);
			break;
		case QLCNIC_ADD_VLAN:
			qlcnic_set_vlan_config(adapter, &esw_cfg[i]);
			break;
		case QLCNIC_DEL_VLAN:
			esw_cfg[i].vlan_id = 0;
			qlcnic_set_vlan_config(adapter, &esw_cfg[i]);
			break;
		}
	}

	if (adapter->op_mode != QLCNIC_MGMT_FUNC)
		goto out;

	for (i = 0; i < count; i++) {
		pci_func = esw_cfg[i].pci_func;
		npar = &adapter->npars[pci_func];
		switch (esw_cfg[i].op_mode) {
		case QLCNIC_PORT_DEFAULTS:
			npar->promisc_mode = esw_cfg[i].promisc_mode;
			npar->mac_override = esw_cfg[i].mac_override;
			npar->offload_flags = esw_cfg[i].offload_flags;
			npar->mac_anti_spoof = esw_cfg[i].mac_anti_spoof;
			npar->discard_tagged = esw_cfg[i].discard_tagged;
			break;
		case QLCNIC_ADD_VLAN:
			npar->pvid = esw_cfg[i].vlan_id;
			break;
		case QLCNIC_DEL_VLAN:
			npar->pvid = 0;
			break;
		}
	}
out:
	return size;
}

static ssize_t
qlcnic_sysfs_read_esw_config(struct file *file, struct kobject *kobj,
	struct bin_attribute *attr, char *buf, loff_t offset, size_t size)
{
	struct device *dev = container_of(kobj, struct device, kobj);
	struct qlcnic_adapter *adapter = dev_get_drvdata(dev);
	struct qlcnic_esw_func_cfg esw_cfg[QLCNIC_MAX_PCI_FUNC];
	u8 i;

	if (size != sizeof(esw_cfg))
		return QL_STATUS_INVALID_PARAM;

	for (i = 0; i < QLCNIC_MAX_PCI_FUNC; i++) {
		if (adapter->npars[i].type != QLCNIC_TYPE_NIC)
			continue;
		esw_cfg[i].pci_func = i;
		if (qlcnic_get_eswitch_port_config(adapter, &esw_cfg[i]))
			return QL_STATUS_INVALID_PARAM;
	}
	memcpy(buf, &esw_cfg, size);

	return size;
}

static int
validate_npar_config(struct qlcnic_adapter *adapter,
				struct qlcnic_npar_func_cfg *np_cfg, int count)
{
	u8 pci_func, i;

	for (i = 0; i < count; i++) {
		pci_func = np_cfg[i].pci_func;
		if (pci_func >= QLCNIC_MAX_PCI_FUNC)
			return QL_STATUS_INVALID_PARAM;

		if (adapter->npars[pci_func].type != QLCNIC_TYPE_NIC)
			return QL_STATUS_INVALID_PARAM;

		if (!IS_VALID_BW(np_cfg[i].min_bw) ||
		    !IS_VALID_BW(np_cfg[i].max_bw))
			return QL_STATUS_INVALID_PARAM;
	}
	return 0;
}

static ssize_t
qlcnic_sysfs_write_npar_config(struct file *file, struct kobject *kobj,
	struct bin_attribute *attr, char *buf, loff_t offset, size_t size)
{
	struct device *dev = container_of(kobj, struct device, kobj);
	struct qlcnic_adapter *adapter = dev_get_drvdata(dev);
	struct qlcnic_info nic_info;
	struct qlcnic_npar_func_cfg *np_cfg;
	int i, count, rem, ret;
	u8 pci_func;

	count	= size / sizeof(struct qlcnic_npar_func_cfg);
	rem	= size % sizeof(struct qlcnic_npar_func_cfg);
	if (rem)
		return QL_STATUS_INVALID_PARAM;

	np_cfg = (struct qlcnic_npar_func_cfg *) buf;
	ret = validate_npar_config(adapter, np_cfg, count);
	if (ret)
		return ret;

	for (i = 0; i < count ; i++) {
		pci_func = np_cfg[i].pci_func;
		ret = qlcnic_get_nic_info(adapter, &nic_info, pci_func);
		if (ret)
			return ret;
		nic_info.pci_func = pci_func;
		nic_info.min_tx_bw = np_cfg[i].min_bw;
		nic_info.max_tx_bw = np_cfg[i].max_bw;
		ret = qlcnic_set_nic_info(adapter, &nic_info);
		if (ret)
			return ret;
		adapter->npars[i].min_bw = nic_info.min_tx_bw;
		adapter->npars[i].max_bw = nic_info.max_tx_bw;
	}

	return size;

}
static ssize_t
qlcnic_sysfs_read_npar_config(struct file *file, struct kobject *kobj,
	struct bin_attribute *attr, char *buf, loff_t offset, size_t size)
{
	struct device *dev = container_of(kobj, struct device, kobj);
	struct qlcnic_adapter *adapter = dev_get_drvdata(dev);
	struct qlcnic_info nic_info;
	struct qlcnic_npar_func_cfg np_cfg[QLCNIC_MAX_PCI_FUNC];
	int i, ret;

	if (size != sizeof(np_cfg))
		return QL_STATUS_INVALID_PARAM;

	for (i = 0; i < QLCNIC_MAX_PCI_FUNC ; i++) {
		if (adapter->npars[i].type != QLCNIC_TYPE_NIC)
			continue;
		ret = qlcnic_get_nic_info(adapter, &nic_info, i);
		if (ret)
			return ret;

		np_cfg[i].pci_func = i;
		np_cfg[i].op_mode = (u8)nic_info.op_mode;
		np_cfg[i].port_num = nic_info.phys_port;
		np_cfg[i].fw_capab = nic_info.capabilities;
		np_cfg[i].min_bw = nic_info.min_tx_bw ;
		np_cfg[i].max_bw = nic_info.max_tx_bw;
		np_cfg[i].max_tx_queues = nic_info.max_tx_ques;
		np_cfg[i].max_rx_queues = nic_info.max_rx_ques;
	}
	memcpy(buf, &np_cfg, size);
	return size;
}

static ssize_t
qlcnic_sysfs_get_port_stats(struct file *file, struct kobject *kobj,
	struct bin_attribute *attr, char *buf, loff_t offset, size_t size)
{
	struct device *dev = container_of(kobj, struct device, kobj);
	struct qlcnic_adapter *adapter = dev_get_drvdata(dev);
	struct qlcnic_esw_statistics port_stats;
	int ret;

	if (size != sizeof(struct qlcnic_esw_statistics))
		return QL_STATUS_INVALID_PARAM;

	if (offset >= QLCNIC_MAX_PCI_FUNC)
		return QL_STATUS_INVALID_PARAM;

	memset(&port_stats, 0, size);
	ret = qlcnic_get_port_stats(adapter, offset, QLCNIC_QUERY_RX_COUNTER,
								&port_stats.rx);
	if (ret)
		return ret;

	ret = qlcnic_get_port_stats(adapter, offset, QLCNIC_QUERY_TX_COUNTER,
								&port_stats.tx);
	if (ret)
		return ret;

	memcpy(buf, &port_stats, size);
	return size;
}

static ssize_t
qlcnic_sysfs_get_esw_stats(struct file *file, struct kobject *kobj,
	struct bin_attribute *attr, char *buf, loff_t offset, size_t size)
{
	struct device *dev = container_of(kobj, struct device, kobj);
	struct qlcnic_adapter *adapter = dev_get_drvdata(dev);
	struct qlcnic_esw_statistics esw_stats;
	int ret;

	if (size != sizeof(struct qlcnic_esw_statistics))
		return QL_STATUS_INVALID_PARAM;

	if (offset >= QLCNIC_NIU_MAX_XG_PORTS)
		return QL_STATUS_INVALID_PARAM;

	memset(&esw_stats, 0, size);
	ret = qlcnic_get_eswitch_stats(adapter, offset, QLCNIC_QUERY_RX_COUNTER,
								&esw_stats.rx);
	if (ret)
		return ret;

	ret = qlcnic_get_eswitch_stats(adapter, offset, QLCNIC_QUERY_TX_COUNTER,
								&esw_stats.tx);
	if (ret)
		return ret;

	memcpy(buf, &esw_stats, size);
	return size;
}

static ssize_t
qlcnic_sysfs_clear_esw_stats(struct file *file, struct kobject *kobj,
	struct bin_attribute *attr, char *buf, loff_t offset, size_t size)
{
	struct device *dev = container_of(kobj, struct device, kobj);
	struct qlcnic_adapter *adapter = dev_get_drvdata(dev);
	int ret;

	if (offset >= QLCNIC_NIU_MAX_XG_PORTS)
		return QL_STATUS_INVALID_PARAM;

	ret = qlcnic_clear_esw_stats(adapter, QLCNIC_STATS_ESWITCH, offset,
						QLCNIC_QUERY_RX_COUNTER);
	if (ret)
		return ret;

	ret = qlcnic_clear_esw_stats(adapter, QLCNIC_STATS_ESWITCH, offset,
						QLCNIC_QUERY_TX_COUNTER);
	if (ret)
		return ret;

	return size;
}

static ssize_t
qlcnic_sysfs_clear_port_stats(struct file *file, struct kobject *kobj,
	struct bin_attribute *attr, char *buf, loff_t offset, size_t size)
{

	struct device *dev = container_of(kobj, struct device, kobj);
	struct qlcnic_adapter *adapter = dev_get_drvdata(dev);
	int ret;

	if (offset >= QLCNIC_MAX_PCI_FUNC)
		return QL_STATUS_INVALID_PARAM;

	ret = qlcnic_clear_esw_stats(adapter, QLCNIC_STATS_PORT, offset,
						QLCNIC_QUERY_RX_COUNTER);
	if (ret)
		return ret;

	ret = qlcnic_clear_esw_stats(adapter, QLCNIC_STATS_PORT, offset,
						QLCNIC_QUERY_TX_COUNTER);
	if (ret)
		return ret;

	return size;
}

static ssize_t
qlcnic_sysfs_read_pci_config(struct file *file, struct kobject *kobj,
	struct bin_attribute *attr, char *buf, loff_t offset, size_t size)
{
	struct device *dev = container_of(kobj, struct device, kobj);
	struct qlcnic_adapter *adapter = dev_get_drvdata(dev);
	struct qlcnic_pci_func_cfg pci_cfg[QLCNIC_MAX_PCI_FUNC];
	struct qlcnic_pci_info *pci_info;
	int i, ret;

	if (size != sizeof(pci_cfg))
		return QL_STATUS_INVALID_PARAM;

	pci_info = kcalloc(QLCNIC_MAX_PCI_FUNC, sizeof(*pci_info), GFP_KERNEL);
	if (!pci_info)
		return -ENOMEM;

	ret = qlcnic_get_pci_info(adapter, pci_info);
	if (ret) {
		kfree(pci_info);
		return ret;
	}

	for (i = 0; i < QLCNIC_MAX_PCI_FUNC ; i++) {
		pci_cfg[i].pci_func = pci_info[i].id;
		pci_cfg[i].func_type = pci_info[i].type;
		pci_cfg[i].port_num = pci_info[i].default_port;
		pci_cfg[i].min_bw = pci_info[i].tx_min_bw;
		pci_cfg[i].max_bw = pci_info[i].tx_max_bw;
		memcpy(&pci_cfg[i].def_mac_addr, &pci_info[i].mac, ETH_ALEN);
	}
	memcpy(buf, &pci_cfg, size);
	kfree(pci_info);
	return size;
}
static struct bin_attribute bin_attr_npar_config = {
	.attr = {.name = "npar_config", .mode = (S_IRUGO | S_IWUSR)},
	.size = 0,
	.read = qlcnic_sysfs_read_npar_config,
	.write = qlcnic_sysfs_write_npar_config,
};

static struct bin_attribute bin_attr_pci_config = {
	.attr = {.name = "pci_config", .mode = (S_IRUGO | S_IWUSR)},
	.size = 0,
	.read = qlcnic_sysfs_read_pci_config,
	.write = NULL,
};

static struct bin_attribute bin_attr_port_stats = {
	.attr = {.name = "port_stats", .mode = (S_IRUGO | S_IWUSR)},
	.size = 0,
	.read = qlcnic_sysfs_get_port_stats,
	.write = qlcnic_sysfs_clear_port_stats,
};

static struct bin_attribute bin_attr_esw_stats = {
	.attr = {.name = "esw_stats", .mode = (S_IRUGO | S_IWUSR)},
	.size = 0,
	.read = qlcnic_sysfs_get_esw_stats,
	.write = qlcnic_sysfs_clear_esw_stats,
};

static struct bin_attribute bin_attr_esw_config = {
	.attr = {.name = "esw_config", .mode = (S_IRUGO | S_IWUSR)},
	.size = 0,
	.read = qlcnic_sysfs_read_esw_config,
	.write = qlcnic_sysfs_write_esw_config,
};

static struct bin_attribute bin_attr_pm_config = {
	.attr = {.name = "pm_config", .mode = (S_IRUGO | S_IWUSR)},
	.size = 0,
	.read = qlcnic_sysfs_read_pm_config,
	.write = qlcnic_sysfs_write_pm_config,
};

static void
qlcnic_create_sysfs_entries(struct qlcnic_adapter *adapter)
{
	struct device *dev = &adapter->pdev->dev;

	if (adapter->capabilities & QLCNIC_FW_CAPABILITY_BDG)
		if (device_create_file(dev, &dev_attr_bridged_mode))
			dev_warn(dev,
				"failed to create bridged_mode sysfs entry\n");
}

static void
qlcnic_remove_sysfs_entries(struct qlcnic_adapter *adapter)
{
	struct device *dev = &adapter->pdev->dev;

	if (adapter->capabilities & QLCNIC_FW_CAPABILITY_BDG)
		device_remove_file(dev, &dev_attr_bridged_mode);
}

static void
qlcnic_create_diag_entries(struct qlcnic_adapter *adapter)
{
	struct device *dev = &adapter->pdev->dev;

	if (device_create_bin_file(dev, &bin_attr_port_stats))
		dev_info(dev, "failed to create port stats sysfs entry");

	if (adapter->op_mode == QLCNIC_NON_PRIV_FUNC)
		return;
	if (device_create_file(dev, &dev_attr_diag_mode))
		dev_info(dev, "failed to create diag_mode sysfs entry\n");
	if (device_create_file(dev, &dev_attr_beacon))
		dev_info(dev, "failed to create beacon sysfs entry");
	if (device_create_bin_file(dev, &bin_attr_crb))
		dev_info(dev, "failed to create crb sysfs entry\n");
	if (device_create_bin_file(dev, &bin_attr_mem))
		dev_info(dev, "failed to create mem sysfs entry\n");
	if (device_create_bin_file(dev, &bin_attr_pci_config))
		dev_info(dev, "failed to create pci config sysfs entry");
	if (!(adapter->flags & QLCNIC_ESWITCH_ENABLED))
		return;
	if (device_create_bin_file(dev, &bin_attr_esw_config))
		dev_info(dev, "failed to create esw config sysfs entry");
	if (adapter->op_mode != QLCNIC_MGMT_FUNC)
		return;
	if (device_create_bin_file(dev, &bin_attr_npar_config))
		dev_info(dev, "failed to create npar config sysfs entry");
	if (device_create_bin_file(dev, &bin_attr_pm_config))
		dev_info(dev, "failed to create pm config sysfs entry");
	if (device_create_bin_file(dev, &bin_attr_esw_stats))
		dev_info(dev, "failed to create eswitch stats sysfs entry");
}

static void
qlcnic_remove_diag_entries(struct qlcnic_adapter *adapter)
{
	struct device *dev = &adapter->pdev->dev;

	device_remove_bin_file(dev, &bin_attr_port_stats);

	if (adapter->op_mode == QLCNIC_NON_PRIV_FUNC)
		return;
	device_remove_file(dev, &dev_attr_diag_mode);
	device_remove_file(dev, &dev_attr_beacon);
	device_remove_bin_file(dev, &bin_attr_crb);
	device_remove_bin_file(dev, &bin_attr_mem);
	device_remove_bin_file(dev, &bin_attr_pci_config);
	if (!(adapter->flags & QLCNIC_ESWITCH_ENABLED))
		return;
	device_remove_bin_file(dev, &bin_attr_esw_config);
	if (adapter->op_mode != QLCNIC_MGMT_FUNC)
		return;
	device_remove_bin_file(dev, &bin_attr_npar_config);
	device_remove_bin_file(dev, &bin_attr_pm_config);
	device_remove_bin_file(dev, &bin_attr_esw_stats);
=======
			pci_dev_put(oth_pdev);
			return 0;
		}
		pci_dev_put(oth_pdev);
	}
	return 1;
}

static int qlcnic_attach_func(struct pci_dev *pdev)
{
	int err, first_func;
	struct qlcnic_adapter *adapter = pci_get_drvdata(pdev);
	struct net_device *netdev = adapter->netdev;

	pdev->error_state = pci_channel_io_normal;

	err = pci_enable_device(pdev);
	if (err)
		return err;

	pci_set_master(pdev);
	pci_restore_state(pdev);

	first_func = qlcnic_is_first_func(pdev);

	if (qlcnic_api_lock(adapter))
		return -EINVAL;

	if (adapter->ahw->op_mode != QLCNIC_NON_PRIV_FUNC && first_func) {
		adapter->need_fw_reset = 1;
		set_bit(__QLCNIC_START_FW, &adapter->state);
		QLC_SHARED_REG_WR32(adapter, QLCNIC_CRB_DEV_STATE,
				    QLCNIC_DEV_INITIALIZING);
		QLCDB(adapter, DRV, "Restarting fw\n");
	}
	qlcnic_api_unlock(adapter);

	err = qlcnic_start_firmware(adapter);
	if (err)
		return err;

	qlcnic_clr_drv_state(adapter);
	kfree(adapter->msix_entries);
	adapter->msix_entries = NULL;
	err = qlcnic_setup_intr(adapter);

	if (err) {
		kfree(adapter->msix_entries);
		netdev_err(netdev, "failed to setup interrupt\n");
		return err;
	}

	if (netif_running(netdev)) {
		err = qlcnic_attach(adapter);
		if (err) {
			qlcnic_clr_all_drv_state(adapter, 1);
			clear_bit(__QLCNIC_AER, &adapter->state);
			netif_device_attach(netdev);
			return err;
		}

		err = qlcnic_up(adapter, netdev);
		if (err)
			goto done;

		qlcnic_restore_indev_addr(netdev, NETDEV_UP);
	}
 done:
	netif_device_attach(netdev);
	return err;
}

pci_ers_result_t qlcnic_82xx_io_error_detected(struct pci_dev *pdev,
					       pci_channel_state_t state)
{
	struct qlcnic_adapter *adapter = pci_get_drvdata(pdev);
	struct net_device *netdev = adapter->netdev;

	if (state == pci_channel_io_perm_failure)
		return PCI_ERS_RESULT_DISCONNECT;

	if (state == pci_channel_io_normal)
		return PCI_ERS_RESULT_RECOVERED;

	set_bit(__QLCNIC_AER, &adapter->state);
	netif_device_detach(netdev);

	cancel_delayed_work_sync(&adapter->fw_work);

	if (netif_running(netdev))
		qlcnic_down(adapter, netdev);

	qlcnic_detach(adapter);
	qlcnic_teardown_intr(adapter);

	clear_bit(__QLCNIC_RESETTING, &adapter->state);

	pci_save_state(pdev);
	pci_disable_device(pdev);

	return PCI_ERS_RESULT_NEED_RESET;
}

pci_ers_result_t qlcnic_82xx_io_slot_reset(struct pci_dev *pdev)
{
	return qlcnic_attach_func(pdev) ? PCI_ERS_RESULT_DISCONNECT :
				PCI_ERS_RESULT_RECOVERED;
}

void qlcnic_82xx_io_resume(struct pci_dev *pdev)
{
	u32 state;
	struct qlcnic_adapter *adapter = pci_get_drvdata(pdev);

	pci_cleanup_aer_uncorrect_error_status(pdev);
	state = QLC_SHARED_REG_RD32(adapter, QLCNIC_CRB_DEV_STATE);
	if (state == QLCNIC_DEV_READY && test_and_clear_bit(__QLCNIC_AER,
							    &adapter->state))
		qlcnic_schedule_work(adapter, qlcnic_fw_poll_work,
				     FW_POLL_DELAY);
}

static pci_ers_result_t qlcnic_io_error_detected(struct pci_dev *pdev,
						 pci_channel_state_t state)
{
	struct qlcnic_adapter *adapter = pci_get_drvdata(pdev);
	struct qlcnic_hardware_ops *hw_ops = adapter->ahw->hw_ops;

	if (hw_ops->io_error_detected) {
		return hw_ops->io_error_detected(pdev, state);
	} else {
		dev_err(&pdev->dev, "AER error_detected handler not registered.\n");
		return PCI_ERS_RESULT_DISCONNECT;
	}
}

static pci_ers_result_t qlcnic_io_slot_reset(struct pci_dev *pdev)
{
	struct qlcnic_adapter *adapter = pci_get_drvdata(pdev);
	struct qlcnic_hardware_ops *hw_ops = adapter->ahw->hw_ops;

	if (hw_ops->io_slot_reset) {
		return hw_ops->io_slot_reset(pdev);
	} else {
		dev_err(&pdev->dev, "AER slot_reset handler not registered.\n");
		return PCI_ERS_RESULT_DISCONNECT;
	}
}

static void qlcnic_io_resume(struct pci_dev *pdev)
{
	struct qlcnic_adapter *adapter = pci_get_drvdata(pdev);
	struct qlcnic_hardware_ops *hw_ops = adapter->ahw->hw_ops;

	if (hw_ops->io_resume)
		hw_ops->io_resume(pdev);
	else
		dev_err(&pdev->dev, "AER resume handler not registered.\n");
}


static int
qlcnicvf_start_firmware(struct qlcnic_adapter *adapter)
{
	int err;

	err = qlcnic_can_start_firmware(adapter);
	if (err)
		return err;

	err = qlcnic_check_npar_opertional(adapter);
	if (err)
		return err;

	err = qlcnic_initialize_nic(adapter);
	if (err)
		return err;

	qlcnic_check_options(adapter);

	err = qlcnic_set_eswitch_port_config(adapter);
	if (err)
		return err;

	adapter->need_fw_reset = 0;

	return err;
}

int qlcnic_validate_rings(struct qlcnic_adapter *adapter, __u32 ring_cnt,
			  int queue_type)
{
	struct net_device *netdev = adapter->netdev;
	u8 max_hw_rings = 0;
	char buf[8];
	int cur_rings;

	if (queue_type == QLCNIC_RX_QUEUE) {
		max_hw_rings = adapter->max_sds_rings;
		cur_rings = adapter->drv_sds_rings;
		strcpy(buf, "SDS");
	} else if (queue_type == QLCNIC_TX_QUEUE) {
		max_hw_rings = adapter->max_tx_rings;
		cur_rings = adapter->drv_tx_rings;
		strcpy(buf, "Tx");
	}

	if (!qlcnic_use_msi_x && !qlcnic_use_msi) {
		netdev_err(netdev, "No RSS/TSS support in INT-x mode\n");
		return -EINVAL;
	}

	if (adapter->flags & QLCNIC_MSI_ENABLED) {
		netdev_err(netdev, "No RSS/TSS support in MSI mode\n");
		return -EINVAL;
	}

	if (ring_cnt < 2) {
		netdev_err(netdev,
			   "%s rings value should not be lower than 2\n", buf);
		return -EINVAL;
	}

	if (!is_power_of_2(ring_cnt)) {
		netdev_err(netdev, "%s rings value should be a power of 2\n",
			   buf);
		return -EINVAL;
	}

	if (qlcnic_82xx_check(adapter) && (queue_type == QLCNIC_TX_QUEUE) &&
	    !qlcnic_check_multi_tx(adapter)) {
			netdev_err(netdev, "No Multi Tx queue support\n");
			return -EINVAL;
	}

	if (ring_cnt > num_online_cpus()) {
		netdev_err(netdev,
			   "%s value[%u] should not be higher than, number of online CPUs\n",
			   buf, num_online_cpus());
		return -EINVAL;
	}

	return 0;
}

int qlcnic_setup_rings(struct qlcnic_adapter *adapter, u8 rx_cnt, u8 tx_cnt)
{
	struct net_device *netdev = adapter->netdev;
	int err;

	if (test_bit(__QLCNIC_RESETTING, &adapter->state))
		return -EBUSY;

	netif_device_detach(netdev);
	if (netif_running(netdev))
		__qlcnic_down(adapter, netdev);

	qlcnic_detach(adapter);

	if (qlcnic_83xx_check(adapter)) {
		qlcnic_83xx_free_mbx_intr(adapter);
		qlcnic_83xx_enable_mbx_poll(adapter);
	}

	qlcnic_teardown_intr(adapter);

	/* compute and set default and max tx/sds rings */
	qlcnic_set_tx_ring_count(adapter, tx_cnt);
	qlcnic_set_sds_ring_count(adapter, rx_cnt);

	netif_set_real_num_tx_queues(netdev, adapter->drv_tx_rings);

	err = qlcnic_setup_intr(adapter);
	if (err) {
		kfree(adapter->msix_entries);
		netdev_err(netdev, "failed to setup interrupt\n");
		return err;
	}

	if (qlcnic_83xx_check(adapter)) {
		/* register for NIC IDC AEN Events */
		qlcnic_83xx_register_nic_idc_func(adapter, 1);
		err = qlcnic_83xx_setup_mbx_intr(adapter);
		qlcnic_83xx_disable_mbx_poll(adapter);
		if (err) {
			dev_err(&adapter->pdev->dev,
				"failed to setup mbx interrupt\n");
			goto done;
		}
	}

	if (netif_running(netdev)) {
		err = qlcnic_attach(adapter);
		if (err)
			goto done;
		err = __qlcnic_up(adapter, netdev);
		if (err)
			goto done;
		qlcnic_restore_indev_addr(netdev, NETDEV_UP);
	}
done:
	netif_device_attach(netdev);
	clear_bit(__QLCNIC_RESETTING, &adapter->state);
	return err;
>>>>>>> refs/remotes/origin/master
}

#ifdef CONFIG_INET

#define is_qlcnic_netdev(dev) (dev->netdev_ops == &qlcnic_netdev_ops)

static void
qlcnic_config_indev_addr(struct qlcnic_adapter *adapter,
			struct net_device *dev, unsigned long event)
{
	struct in_device *indev;

	indev = in_dev_get(dev);
	if (!indev)
		return;

	for_ifa(indev) {
		switch (event) {
		case NETDEV_UP:
			qlcnic_config_ipaddr(adapter,
					ifa->ifa_address, QLCNIC_IP_UP);
			break;
		case NETDEV_DOWN:
			qlcnic_config_ipaddr(adapter,
					ifa->ifa_address, QLCNIC_IP_DOWN);
			break;
		default:
			break;
		}
	} endfor_ifa(indev);

	in_dev_put(indev);
}

<<<<<<< HEAD
static void
qlcnic_restore_indev_addr(struct net_device *netdev, unsigned long event)
=======
void qlcnic_restore_indev_addr(struct net_device *netdev, unsigned long event)
>>>>>>> refs/remotes/origin/master
{
	struct qlcnic_adapter *adapter = netdev_priv(netdev);
	struct net_device *dev;
	u16 vid;

	qlcnic_config_indev_addr(adapter, netdev, event);

<<<<<<< HEAD
	for_each_set_bit(vid, adapter->vlans, VLAN_N_VID) {
		dev = __vlan_find_dev_deep(netdev, vid);
=======
	rcu_read_lock();
	for_each_set_bit(vid, adapter->vlans, VLAN_N_VID) {
		dev = __vlan_find_dev_deep(netdev, htons(ETH_P_8021Q), vid);
>>>>>>> refs/remotes/origin/master
		if (!dev)
			continue;
		qlcnic_config_indev_addr(adapter, dev, event);
	}
<<<<<<< HEAD
=======
	rcu_read_unlock();
>>>>>>> refs/remotes/origin/master
}

static int qlcnic_netdev_event(struct notifier_block *this,
				 unsigned long event, void *ptr)
{
	struct qlcnic_adapter *adapter;
<<<<<<< HEAD
	struct net_device *dev = (struct net_device *)ptr;
=======
	struct net_device *dev = netdev_notifier_info_to_dev(ptr);
>>>>>>> refs/remotes/origin/master

recheck:
	if (dev == NULL)
		goto done;

	if (dev->priv_flags & IFF_802_1Q_VLAN) {
		dev = vlan_dev_real_dev(dev);
		goto recheck;
	}

	if (!is_qlcnic_netdev(dev))
		goto done;

	adapter = netdev_priv(dev);

	if (!adapter)
		goto done;

	if (!test_bit(__QLCNIC_DEV_UP, &adapter->state))
		goto done;

	qlcnic_config_indev_addr(adapter, dev, event);
done:
	return NOTIFY_DONE;
}

static int
qlcnic_inetaddr_event(struct notifier_block *this,
		unsigned long event, void *ptr)
{
	struct qlcnic_adapter *adapter;
	struct net_device *dev;

	struct in_ifaddr *ifa = (struct in_ifaddr *)ptr;

	dev = ifa->ifa_dev ? ifa->ifa_dev->dev : NULL;

recheck:
	if (dev == NULL)
		goto done;

	if (dev->priv_flags & IFF_802_1Q_VLAN) {
		dev = vlan_dev_real_dev(dev);
		goto recheck;
	}

	if (!is_qlcnic_netdev(dev))
		goto done;

	adapter = netdev_priv(dev);

	if (!adapter)
		goto done;

	if (!test_bit(__QLCNIC_DEV_UP, &adapter->state))
		goto done;

	switch (event) {
	case NETDEV_UP:
		qlcnic_config_ipaddr(adapter, ifa->ifa_address, QLCNIC_IP_UP);
<<<<<<< HEAD
		break;
	case NETDEV_DOWN:
		qlcnic_config_ipaddr(adapter, ifa->ifa_address, QLCNIC_IP_DOWN);
=======

		break;
	case NETDEV_DOWN:
		qlcnic_config_ipaddr(adapter, ifa->ifa_address, QLCNIC_IP_DOWN);

>>>>>>> refs/remotes/origin/master
		break;
	default:
		break;
	}

done:
	return NOTIFY_DONE;
}

static struct notifier_block	qlcnic_netdev_cb = {
	.notifier_call = qlcnic_netdev_event,
};

static struct notifier_block qlcnic_inetaddr_cb = {
	.notifier_call = qlcnic_inetaddr_event,
};
#else
<<<<<<< HEAD
static void
qlcnic_restore_indev_addr(struct net_device *dev, unsigned long event)
{ }
#endif
static struct pci_error_handlers qlcnic_err_handler = {
=======
void qlcnic_restore_indev_addr(struct net_device *dev, unsigned long event)
{ }
#endif
static const struct pci_error_handlers qlcnic_err_handler = {
>>>>>>> refs/remotes/origin/master
	.error_detected = qlcnic_io_error_detected,
	.slot_reset = qlcnic_io_slot_reset,
	.resume = qlcnic_io_resume,
};

static struct pci_driver qlcnic_driver = {
	.name = qlcnic_driver_name,
	.id_table = qlcnic_pci_tbl,
	.probe = qlcnic_probe,
<<<<<<< HEAD
	.remove = __devexit_p(qlcnic_remove),
=======
	.remove = qlcnic_remove,
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_PM
	.suspend = qlcnic_suspend,
	.resume = qlcnic_resume,
#endif
	.shutdown = qlcnic_shutdown,
<<<<<<< HEAD
	.err_handler = &qlcnic_err_handler
=======
	.err_handler = &qlcnic_err_handler,
#ifdef CONFIG_QLCNIC_SRIOV
	.sriov_configure = qlcnic_pci_sriov_configure,
#endif
>>>>>>> refs/remotes/origin/master

};

static int __init qlcnic_init_module(void)
{
	int ret;

	printk(KERN_INFO "%s\n", qlcnic_driver_string);

<<<<<<< HEAD
	qlcnic_wq = create_singlethread_workqueue("qlcnic");
	if (qlcnic_wq == NULL) {
		printk(KERN_ERR "qlcnic: cannot create workqueue\n");
		return -ENOMEM;
	}

=======
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_INET
	register_netdevice_notifier(&qlcnic_netdev_cb);
	register_inetaddr_notifier(&qlcnic_inetaddr_cb);
#endif

	ret = pci_register_driver(&qlcnic_driver);
	if (ret) {
#ifdef CONFIG_INET
		unregister_inetaddr_notifier(&qlcnic_inetaddr_cb);
		unregister_netdevice_notifier(&qlcnic_netdev_cb);
#endif
<<<<<<< HEAD
		destroy_workqueue(qlcnic_wq);
=======
>>>>>>> refs/remotes/origin/master
	}

	return ret;
}

module_init(qlcnic_init_module);

static void __exit qlcnic_exit_module(void)
{
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
	pci_unregister_driver(&qlcnic_driver);

#ifdef CONFIG_INET
	unregister_inetaddr_notifier(&qlcnic_inetaddr_cb);
	unregister_netdevice_notifier(&qlcnic_netdev_cb);
#endif
<<<<<<< HEAD
	destroy_workqueue(qlcnic_wq);
=======
>>>>>>> refs/remotes/origin/master
}

module_exit(qlcnic_exit_module);
