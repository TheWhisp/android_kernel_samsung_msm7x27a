/*
 * QLogic iSCSI HBA Driver
<<<<<<< HEAD
 * Copyright (c)  2003-2010 QLogic Corporation
=======
 * Copyright (c)  2003-2013 QLogic Corporation
>>>>>>> refs/remotes/origin/master
 *
 * See LICENSE.qla4xxx for copyright and licensing details.
 */

#include <scsi/iscsi_if.h>
#include "ql4_def.h"
#include "ql4_glbl.h"
#include "ql4_dbg.h"
#include "ql4_inline.h"

<<<<<<< HEAD
<<<<<<< HEAD
static struct ddb_entry *qla4xxx_alloc_ddb(struct scsi_qla_host *ha,
					   uint32_t fw_ddb_index);

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static void ql4xxx_set_mac_number(struct scsi_qla_host *ha)
{
	uint32_t value;
	uint8_t func_number;
	unsigned long flags;

	/* Get the function number */
	spin_lock_irqsave(&ha->hardware_lock, flags);
	value = readw(&ha->reg->ctrl_status);
	spin_unlock_irqrestore(&ha->hardware_lock, flags);

	func_number = (uint8_t) ((value >> 4) & 0x30);
	switch (value & ISP_CONTROL_FN_MASK) {
	case ISP_CONTROL_FN0_SCSI:
		ha->mac_index = 1;
		break;
	case ISP_CONTROL_FN1_SCSI:
		ha->mac_index = 3;
		break;
	default:
		DEBUG2(printk("scsi%ld: %s: Invalid function number, "
			      "ispControlStatus = 0x%x\n", ha->host_no,
			      __func__, value));
		break;
	}
	DEBUG2(printk("scsi%ld: %s: mac_index %d.\n", ha->host_no, __func__,
		      ha->mac_index));
}

/**
 * qla4xxx_free_ddb - deallocate ddb
 * @ha: pointer to host adapter structure.
 * @ddb_entry: pointer to device database entry
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * This routine deallocates and unlinks the specified ddb_entry from the
 * adapter's
=======
 * This routine marks a DDB entry INVALID
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * This routine marks a DDB entry INVALID
>>>>>>> refs/remotes/origin/master
 **/
void qla4xxx_free_ddb(struct scsi_qla_host *ha,
    struct ddb_entry *ddb_entry)
{
<<<<<<< HEAD
<<<<<<< HEAD
	/* Remove device entry from list */
	list_del_init(&ddb_entry->list);

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	/* Remove device pointer from index mapping arrays */
	ha->fw_ddb_index_map[ddb_entry->fw_ddb_index] =
		(struct ddb_entry *) INVALID_ENTRY;
	ha->tot_ddbs--;
<<<<<<< HEAD
<<<<<<< HEAD

	/* Free memory and scsi-ml struct for device entry */
	qla4xxx_destroy_sess(ddb_entry);
}

/**
 * qla4xxx_free_ddb_list - deallocate all ddbs
 * @ha: pointer to host adapter structure.
 *
 * This routine deallocates and removes all devices on the sppecified adapter.
 **/
void qla4xxx_free_ddb_list(struct scsi_qla_host *ha)
{
	struct list_head *ptr;
	struct ddb_entry *ddb_entry;

	while (!list_empty(&ha->ddb_list)) {
		ptr = ha->ddb_list.next;
		/* Free memory for device entry and remove */
		ddb_entry = list_entry(ptr, struct ddb_entry, list);
		qla4xxx_free_ddb(ha, ddb_entry);
	}
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

/**
 * qla4xxx_init_response_q_entries() - Initializes response queue entries.
 * @ha: HA context
 *
 * Beginning of request ring has initialization control block already built
 * by nvram config routine.
 **/
static void qla4xxx_init_response_q_entries(struct scsi_qla_host *ha)
{
	uint16_t cnt;
	struct response *pkt;

	pkt = (struct response *)ha->response_ptr;
	for (cnt = 0; cnt < RESPONSE_QUEUE_DEPTH; cnt++) {
		pkt->signature = RESPONSE_PROCESSED;
		pkt++;
	}
}

/**
 * qla4xxx_init_rings - initialize hw queues
 * @ha: pointer to host adapter structure.
 *
 * This routine initializes the internal queues for the specified adapter.
 * The QLA4010 requires us to restart the queues at index 0.
 * The QLA4000 doesn't care, so just default to QLA4010's requirement.
 **/
int qla4xxx_init_rings(struct scsi_qla_host *ha)
{
	unsigned long flags = 0;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	int i;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	int i;
>>>>>>> refs/remotes/origin/master

	/* Initialize request queue. */
	spin_lock_irqsave(&ha->hardware_lock, flags);
	ha->request_out = 0;
	ha->request_in = 0;
	ha->request_ptr = &ha->request_ring[ha->request_in];
	ha->req_q_count = REQUEST_QUEUE_DEPTH;

	/* Initialize response queue. */
	ha->response_in = 0;
	ha->response_out = 0;
	ha->response_ptr = &ha->response_ring[ha->response_out];

	if (is_qla8022(ha)) {
		writel(0,
<<<<<<< HEAD
		    (unsigned long  __iomem *)&ha->qla4_8xxx_reg->req_q_out);
		writel(0,
		    (unsigned long  __iomem *)&ha->qla4_8xxx_reg->rsp_q_in);
		writel(0,
		    (unsigned long  __iomem *)&ha->qla4_8xxx_reg->rsp_q_out);
=======
		    (unsigned long  __iomem *)&ha->qla4_82xx_reg->req_q_out);
		writel(0,
		    (unsigned long  __iomem *)&ha->qla4_82xx_reg->rsp_q_in);
		writel(0,
		    (unsigned long  __iomem *)&ha->qla4_82xx_reg->rsp_q_out);
	} else if (is_qla8032(ha) || is_qla8042(ha)) {
		writel(0,
		       (unsigned long __iomem *)&ha->qla4_83xx_reg->req_q_in);
		writel(0,
		       (unsigned long __iomem *)&ha->qla4_83xx_reg->rsp_q_in);
		writel(0,
		       (unsigned long __iomem *)&ha->qla4_83xx_reg->rsp_q_out);
>>>>>>> refs/remotes/origin/master
	} else {
		/*
		 * Initialize DMA Shadow registers.  The firmware is really
		 * supposed to take care of this, but on some uniprocessor
		 * systems, the shadow registers aren't cleared-- causing
		 * the interrupt_handler to think there are responses to be
		 * processed when there aren't.
		 */
		ha->shadow_regs->req_q_out = __constant_cpu_to_le32(0);
		ha->shadow_regs->rsp_q_in = __constant_cpu_to_le32(0);
		wmb();

		writel(0, &ha->reg->req_q_in);
		writel(0, &ha->reg->rsp_q_out);
		readl(&ha->reg->rsp_q_out);
	}

	qla4xxx_init_response_q_entries(ha);

<<<<<<< HEAD
<<<<<<< HEAD
=======
	/* Initialize mabilbox active array */
	for (i = 0; i < MAX_MRB; i++)
		ha->active_mrb_array[i] = NULL;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* Initialize mailbox active array */
	for (i = 0; i < MAX_MRB; i++)
		ha->active_mrb_array[i] = NULL;

>>>>>>> refs/remotes/origin/master
	spin_unlock_irqrestore(&ha->hardware_lock, flags);

	return QLA_SUCCESS;
}

/**
 * qla4xxx_get_sys_info - validate adapter MAC address(es)
 * @ha: pointer to host adapter structure.
 *
 **/
int qla4xxx_get_sys_info(struct scsi_qla_host *ha)
{
	struct flash_sys_info *sys_info;
	dma_addr_t sys_info_dma;
	int status = QLA_ERROR;

	sys_info = dma_alloc_coherent(&ha->pdev->dev, sizeof(*sys_info),
				      &sys_info_dma, GFP_KERNEL);
	if (sys_info == NULL) {
		DEBUG2(printk("scsi%ld: %s: Unable to allocate dma buffer.\n",
			      ha->host_no, __func__));

		goto exit_get_sys_info_no_free;
	}
	memset(sys_info, 0, sizeof(*sys_info));

	/* Get flash sys info */
	if (qla4xxx_get_flash(ha, sys_info_dma, FLASH_OFFSET_SYS_INFO,
			      sizeof(*sys_info)) != QLA_SUCCESS) {
		DEBUG2(printk("scsi%ld: %s: get_flash FLASH_OFFSET_SYS_INFO "
			      "failed\n", ha->host_no, __func__));

		goto exit_get_sys_info;
	}

	/* Save M.A.C. address & serial_number */
	memcpy(ha->my_mac, &sys_info->physAddr[0].address[0],
	       min(sizeof(ha->my_mac),
		   sizeof(sys_info->physAddr[0].address)));
	memcpy(ha->serial_number, &sys_info->acSerialNumber,
	       min(sizeof(ha->serial_number),
		   sizeof(sys_info->acSerialNumber)));

	status = QLA_SUCCESS;

exit_get_sys_info:
	dma_free_coherent(&ha->pdev->dev, sizeof(*sys_info), sys_info,
			  sys_info_dma);

exit_get_sys_info_no_free:
	return status;
}

/**
 * qla4xxx_init_local_data - initialize adapter specific local data
 * @ha: pointer to host adapter structure.
 *
 **/
<<<<<<< HEAD
static int qla4xxx_init_local_data(struct scsi_qla_host *ha)
{
	/* Initialize aen queue */
	ha->aen_q_count = MAX_AEN_ENTRIES;

	return qla4xxx_get_firmware_status(ha);
=======
static void qla4xxx_init_local_data(struct scsi_qla_host *ha)
{
	/* Initialize aen queue */
	ha->aen_q_count = MAX_AEN_ENTRIES;
>>>>>>> refs/remotes/origin/master
}

static uint8_t
qla4xxx_wait_for_ip_config(struct scsi_qla_host *ha)
{
	uint8_t ipv4_wait = 0;
	uint8_t ipv6_wait = 0;
	int8_t ip_address[IPv6_ADDR_LEN] = {0} ;

	/* If both IPv4 & IPv6 are enabled, possibly only one
	 * IP address may be acquired, so check to see if we
	 * need to wait for another */
	if (is_ipv4_enabled(ha) && is_ipv6_enabled(ha)) {
		if (((ha->addl_fw_state & FW_ADDSTATE_DHCPv4_ENABLED) != 0) &&
		    ((ha->addl_fw_state &
				    FW_ADDSTATE_DHCPv4_LEASE_ACQUIRED) == 0)) {
			ipv4_wait = 1;
		}
<<<<<<< HEAD
<<<<<<< HEAD
		if (((ha->ipv6_addl_options &
			    IPV6_ADDOPT_NEIGHBOR_DISCOVERY_ADDR_ENABLE) != 0) &&
		    ((ha->ipv6_link_local_state == IP_ADDRSTATE_ACQUIRING) ||
		     (ha->ipv6_addr0_state == IP_ADDRSTATE_ACQUIRING) ||
		     (ha->ipv6_addr1_state == IP_ADDRSTATE_ACQUIRING))) {

			ipv6_wait = 1;

			if ((ha->ipv6_link_local_state ==
						     IP_ADDRSTATE_PREFERRED) ||
			    (ha->ipv6_addr0_state == IP_ADDRSTATE_PREFERRED) ||
			    (ha->ipv6_addr1_state == IP_ADDRSTATE_PREFERRED)) {
=======
=======
>>>>>>> refs/remotes/origin/master
		if (((ha->ip_config.ipv6_addl_options &
		      IPV6_ADDOPT_NEIGHBOR_DISCOVERY_ADDR_ENABLE) != 0) &&
		    ((ha->ip_config.ipv6_link_local_state ==
		      IP_ADDRSTATE_ACQUIRING) ||
		     (ha->ip_config.ipv6_addr0_state ==
		      IP_ADDRSTATE_ACQUIRING) ||
		     (ha->ip_config.ipv6_addr1_state ==
		      IP_ADDRSTATE_ACQUIRING))) {

			ipv6_wait = 1;

			if ((ha->ip_config.ipv6_link_local_state ==
			     IP_ADDRSTATE_PREFERRED) ||
			    (ha->ip_config.ipv6_addr0_state ==
			     IP_ADDRSTATE_PREFERRED) ||
			    (ha->ip_config.ipv6_addr1_state ==
			     IP_ADDRSTATE_PREFERRED)) {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				DEBUG2(printk(KERN_INFO "scsi%ld: %s: "
					      "Preferred IP configured."
					      " Don't wait!\n", ha->host_no,
					      __func__));
				ipv6_wait = 0;
			}
<<<<<<< HEAD
<<<<<<< HEAD
			if (memcmp(&ha->ipv6_default_router_addr, ip_address,
				IPv6_ADDR_LEN) == 0) {
=======
			if (memcmp(&ha->ip_config.ipv6_default_router_addr,
				   ip_address, IPv6_ADDR_LEN) == 0) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
			if (memcmp(&ha->ip_config.ipv6_default_router_addr,
				   ip_address, IPv6_ADDR_LEN) == 0) {
>>>>>>> refs/remotes/origin/master
				DEBUG2(printk(KERN_INFO "scsi%ld: %s: "
					      "No Router configured. "
					      "Don't wait!\n", ha->host_no,
					      __func__));
				ipv6_wait = 0;
			}
<<<<<<< HEAD
<<<<<<< HEAD
			if ((ha->ipv6_default_router_state ==
						IPV6_RTRSTATE_MANUAL) &&
			    (ha->ipv6_link_local_state ==
						IP_ADDRSTATE_TENTATIVE) &&
			    (memcmp(&ha->ipv6_link_local_addr,
				    &ha->ipv6_default_router_addr, 4) == 0)) {
=======
=======
>>>>>>> refs/remotes/origin/master
			if ((ha->ip_config.ipv6_default_router_state ==
			     IPV6_RTRSTATE_MANUAL) &&
			    (ha->ip_config.ipv6_link_local_state ==
			     IP_ADDRSTATE_TENTATIVE) &&
			    (memcmp(&ha->ip_config.ipv6_link_local_addr,
			     &ha->ip_config.ipv6_default_router_addr, 4) ==
			     0)) {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				DEBUG2(printk("scsi%ld: %s: LinkLocal Router & "
					"IP configured. Don't wait!\n",
					ha->host_no, __func__));
				ipv6_wait = 0;
			}
		}
		if (ipv4_wait || ipv6_wait) {
			DEBUG2(printk("scsi%ld: %s: Wait for additional "
				      "IP(s) \"", ha->host_no, __func__));
			if (ipv4_wait)
				DEBUG2(printk("IPv4 "));
<<<<<<< HEAD
<<<<<<< HEAD
			if (ha->ipv6_link_local_state == IP_ADDRSTATE_ACQUIRING)
				DEBUG2(printk("IPv6LinkLocal "));
			if (ha->ipv6_addr0_state == IP_ADDRSTATE_ACQUIRING)
				DEBUG2(printk("IPv6Addr0 "));
			if (ha->ipv6_addr1_state == IP_ADDRSTATE_ACQUIRING)
=======
=======
>>>>>>> refs/remotes/origin/master
			if (ha->ip_config.ipv6_link_local_state ==
			    IP_ADDRSTATE_ACQUIRING)
				DEBUG2(printk("IPv6LinkLocal "));
			if (ha->ip_config.ipv6_addr0_state ==
			    IP_ADDRSTATE_ACQUIRING)
				DEBUG2(printk("IPv6Addr0 "));
			if (ha->ip_config.ipv6_addr1_state ==
			    IP_ADDRSTATE_ACQUIRING)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				DEBUG2(printk("IPv6Addr1 "));
			DEBUG2(printk("\"\n"));
		}
	}

	return ipv4_wait|ipv6_wait;
}

<<<<<<< HEAD
=======
/**
 * qla4xxx_alloc_fw_dump - Allocate memory for minidump data.
 * @ha: pointer to host adapter structure.
 **/
void qla4xxx_alloc_fw_dump(struct scsi_qla_host *ha)
{
	int status;
	uint32_t capture_debug_level;
	int hdr_entry_bit, k;
	void *md_tmp;
	dma_addr_t md_tmp_dma;
	struct qla4_8xxx_minidump_template_hdr *md_hdr;

	if (ha->fw_dump) {
		ql4_printk(KERN_WARNING, ha,
			   "Firmware dump previously allocated.\n");
		return;
	}

	status = qla4xxx_req_template_size(ha);
	if (status != QLA_SUCCESS) {
		ql4_printk(KERN_INFO, ha,
			   "scsi%ld: Failed to get template size\n",
			   ha->host_no);
		return;
	}

	clear_bit(AF_82XX_FW_DUMPED, &ha->flags);

	/* Allocate memory for saving the template */
	md_tmp = dma_alloc_coherent(&ha->pdev->dev, ha->fw_dump_tmplt_size,
				    &md_tmp_dma, GFP_KERNEL);

	/* Request template */
	status =  qla4xxx_get_minidump_template(ha, md_tmp_dma);
	if (status != QLA_SUCCESS) {
		ql4_printk(KERN_INFO, ha,
			   "scsi%ld: Failed to get minidump template\n",
			   ha->host_no);
		goto alloc_cleanup;
	}

	md_hdr = (struct qla4_8xxx_minidump_template_hdr *)md_tmp;

	capture_debug_level = md_hdr->capture_debug_level;

	/* Get capture mask based on module loadtime setting. */
	if (ql4xmdcapmask >= 0x3 && ql4xmdcapmask <= 0x7F)
		ha->fw_dump_capture_mask = ql4xmdcapmask;
	else
		ha->fw_dump_capture_mask = capture_debug_level;

	md_hdr->driver_capture_mask = ha->fw_dump_capture_mask;

	DEBUG2(ql4_printk(KERN_INFO, ha, "Minimum num of entries = %d\n",
			  md_hdr->num_of_entries));
	DEBUG2(ql4_printk(KERN_INFO, ha, "Dump template size  = %d\n",
			  ha->fw_dump_tmplt_size));
	DEBUG2(ql4_printk(KERN_INFO, ha, "Selected Capture mask =0x%x\n",
			  ha->fw_dump_capture_mask));

	/* Calculate fw_dump_size */
	for (hdr_entry_bit = 0x2, k = 1; (hdr_entry_bit & 0xFF);
	     hdr_entry_bit <<= 1, k++) {
		if (hdr_entry_bit & ha->fw_dump_capture_mask)
			ha->fw_dump_size += md_hdr->capture_size_array[k];
	}

	/* Total firmware dump size including command header */
	ha->fw_dump_size += ha->fw_dump_tmplt_size;
	ha->fw_dump = vmalloc(ha->fw_dump_size);
	if (!ha->fw_dump)
		goto alloc_cleanup;

	DEBUG2(ql4_printk(KERN_INFO, ha,
			  "Minidump Tempalate Size = 0x%x KB\n",
			  ha->fw_dump_tmplt_size));
	DEBUG2(ql4_printk(KERN_INFO, ha,
			  "Total Minidump size = 0x%x KB\n", ha->fw_dump_size));

	memcpy(ha->fw_dump, md_tmp, ha->fw_dump_tmplt_size);
	ha->fw_dump_tmplt_hdr = ha->fw_dump;

alloc_cleanup:
	dma_free_coherent(&ha->pdev->dev, ha->fw_dump_tmplt_size,
			  md_tmp, md_tmp_dma);
}

>>>>>>> refs/remotes/origin/master
static int qla4xxx_fw_ready(struct scsi_qla_host *ha)
{
	uint32_t timeout_count;
	int ready = 0;

	DEBUG2(ql4_printk(KERN_INFO, ha, "Waiting for Firmware Ready..\n"));
	for (timeout_count = ADAPTER_INIT_TOV; timeout_count > 0;
	     timeout_count--) {
		if (test_and_clear_bit(DPC_GET_DHCP_IP_ADDR, &ha->dpc_flags))
			qla4xxx_get_dhcp_ip_address(ha);

		/* Get firmware state. */
		if (qla4xxx_get_firmware_state(ha) != QLA_SUCCESS) {
			DEBUG2(printk("scsi%ld: %s: unable to get firmware "
				      "state\n", ha->host_no, __func__));
			break;
		}

		if (ha->firmware_state & FW_STATE_ERROR) {
			DEBUG2(printk("scsi%ld: %s: an unrecoverable error has"
				      " occurred\n", ha->host_no, __func__));
			break;

		}
		if (ha->firmware_state & FW_STATE_CONFIG_WAIT) {
			/*
			 * The firmware has not yet been issued an Initialize
			 * Firmware command, so issue it now.
			 */
			if (qla4xxx_initialize_fw_cb(ha) == QLA_ERROR)
				break;

			/* Go back and test for ready state - no wait. */
			continue;
		}

		if (ha->firmware_state & FW_STATE_WAIT_AUTOCONNECT) {
			DEBUG2(printk(KERN_INFO "scsi%ld: %s: fwstate:"
				      "AUTOCONNECT in progress\n",
				      ha->host_no, __func__));
		}

		if (ha->firmware_state & FW_STATE_CONFIGURING_IP) {
			DEBUG2(printk(KERN_INFO "scsi%ld: %s: fwstate:"
				      " CONFIGURING IP\n",
				      ha->host_no, __func__));
			/*
			 * Check for link state after 15 secs and if link is
			 * still DOWN then, cable is unplugged. Ignore "DHCP
			 * in Progress/CONFIGURING IP" bit to check if firmware
			 * is in ready state or not after 15 secs.
			 * This is applicable for both 2.x & 3.x firmware
			 */
			if (timeout_count <= (ADAPTER_INIT_TOV - 15)) {
				if (ha->addl_fw_state & FW_ADDSTATE_LINK_UP) {
					DEBUG2(printk(KERN_INFO "scsi%ld: %s:"
						  " LINK UP (Cable plugged)\n",
						  ha->host_no, __func__));
				} else if (ha->firmware_state &
					  (FW_STATE_CONFIGURING_IP |
							     FW_STATE_READY)) {
					DEBUG2(printk(KERN_INFO "scsi%ld: %s: "
						"LINK DOWN (Cable unplugged)\n",
						ha->host_no, __func__));
					ha->firmware_state = FW_STATE_READY;
				}
			}
		}

		if (ha->firmware_state == FW_STATE_READY) {
			/* If DHCP IP Addr is available, retrieve it now. */
			if (test_and_clear_bit(DPC_GET_DHCP_IP_ADDR,
								&ha->dpc_flags))
				qla4xxx_get_dhcp_ip_address(ha);

			if (!qla4xxx_wait_for_ip_config(ha) ||
							timeout_count == 1) {
				DEBUG2(ql4_printk(KERN_INFO, ha,
				    "Firmware Ready..\n"));
				/* The firmware is ready to process SCSI
				   commands. */
				DEBUG2(ql4_printk(KERN_INFO, ha,
					"scsi%ld: %s: MEDIA TYPE"
					" - %s\n", ha->host_no,
					__func__, (ha->addl_fw_state &
					FW_ADDSTATE_OPTICAL_MEDIA)
					!= 0 ? "OPTICAL" : "COPPER"));
				DEBUG2(ql4_printk(KERN_INFO, ha,
					"scsi%ld: %s: DHCPv4 STATE"
					" Enabled %s\n", ha->host_no,
					 __func__, (ha->addl_fw_state &
					 FW_ADDSTATE_DHCPv4_ENABLED) != 0 ?
					"YES" : "NO"));
				DEBUG2(ql4_printk(KERN_INFO, ha,
					"scsi%ld: %s: LINK %s\n",
					ha->host_no, __func__,
					(ha->addl_fw_state &
					 FW_ADDSTATE_LINK_UP) != 0 ?
					"UP" : "DOWN"));
				DEBUG2(ql4_printk(KERN_INFO, ha,
					"scsi%ld: %s: iSNS Service "
					"Started %s\n",
					ha->host_no, __func__,
					(ha->addl_fw_state &
					 FW_ADDSTATE_ISNS_SVC_ENABLED) != 0 ?
					"YES" : "NO"));

				ready = 1;
				break;
			}
		}
		DEBUG2(printk("scsi%ld: %s: waiting on fw, state=%x:%x - "
			      "seconds expired= %d\n", ha->host_no, __func__,
			      ha->firmware_state, ha->addl_fw_state,
			      timeout_count));
		if (is_qla4032(ha) &&
			!(ha->addl_fw_state & FW_ADDSTATE_LINK_UP) &&
			(timeout_count < ADAPTER_INIT_TOV - 5)) {
			break;
		}

		msleep(1000);
	}			/* end of for */

	if (timeout_count <= 0)
		DEBUG2(printk("scsi%ld: %s: FW Initialization timed out!\n",
			      ha->host_no, __func__));

	if (ha->firmware_state & FW_STATE_CONFIGURING_IP) {
		DEBUG2(printk("scsi%ld: %s: FW initialized, but is reporting "
			      "it's waiting to configure an IP address\n",
			       ha->host_no, __func__));
		ready = 1;
	} else if (ha->firmware_state & FW_STATE_WAIT_AUTOCONNECT) {
		DEBUG2(printk("scsi%ld: %s: FW initialized, but "
			      "auto-discovery still in process\n",
			       ha->host_no, __func__));
		ready = 1;
	}

	return ready;
}

/**
 * qla4xxx_init_firmware - initializes the firmware.
 * @ha: pointer to host adapter structure.
 *
 **/
static int qla4xxx_init_firmware(struct scsi_qla_host *ha)
{
	int status = QLA_ERROR;

	if (is_aer_supported(ha) &&
	    test_bit(AF_PCI_CHANNEL_IO_PERM_FAILURE, &ha->flags))
		return status;

	/* For 82xx, stop firmware before initializing because if BIOS
	 * has previously initialized firmware, then driver's initialize
	 * firmware will fail. */
<<<<<<< HEAD
	if (is_qla8022(ha))
=======
	if (is_qla80XX(ha))
>>>>>>> refs/remotes/origin/master
		qla4_8xxx_stop_firmware(ha);

	ql4_printk(KERN_INFO, ha, "Initializing firmware..\n");
	if (qla4xxx_initialize_fw_cb(ha) == QLA_ERROR) {
		DEBUG2(printk("scsi%ld: %s: Failed to initialize firmware "
			      "control block\n", ha->host_no, __func__));
		return status;
	}
<<<<<<< HEAD
	if (!qla4xxx_fw_ready(ha))
		return status;

	return qla4xxx_get_firmware_status(ha);
}

<<<<<<< HEAD
static struct ddb_entry* qla4xxx_get_ddb_entry(struct scsi_qla_host *ha,
						uint32_t fw_ddb_index,
						uint32_t *new_tgt)
{
	struct dev_db_entry *fw_ddb_entry = NULL;
	dma_addr_t fw_ddb_entry_dma;
	struct ddb_entry *ddb_entry = NULL;
	int found = 0;
	uint32_t device_state;

	*new_tgt = 0;
	/* Make sure the dma buffer is valid */
	fw_ddb_entry = dma_alloc_coherent(&ha->pdev->dev,
					  sizeof(*fw_ddb_entry),
					  &fw_ddb_entry_dma, GFP_KERNEL);
	if (fw_ddb_entry == NULL) {
		DEBUG2(printk("scsi%ld: %s: Unable to allocate dma buffer.\n",
			      ha->host_no, __func__));
		goto exit_get_ddb_entry_no_free;
	}

	if (qla4xxx_get_fwddb_entry(ha, fw_ddb_index, fw_ddb_entry,
				    fw_ddb_entry_dma, NULL, NULL,
				    &device_state, NULL, NULL, NULL) ==
				    QLA_ERROR) {
		DEBUG2(printk("scsi%ld: %s: failed get_ddb_entry for "
			      "fw_ddb_index %d\n", ha->host_no, __func__,
			      fw_ddb_index));
		goto exit_get_ddb_entry;
	}

	/* Allocate DDB if not already allocated. */
	DEBUG2(printk("scsi%ld: %s: Looking for ddb[%d]\n", ha->host_no,
		      __func__, fw_ddb_index));
	list_for_each_entry(ddb_entry, &ha->ddb_list, list) {
		if ((memcmp(ddb_entry->iscsi_name, fw_ddb_entry->iscsi_name,
			   ISCSI_NAME_SIZE) == 0) &&
			(ddb_entry->tpgt ==
				le32_to_cpu(fw_ddb_entry->tgt_portal_grp)) &&
			(memcmp(ddb_entry->isid, fw_ddb_entry->isid,
				sizeof(ddb_entry->isid)) == 0)) {
			found++;
			break;
		}
	}

	/* if not found allocate new ddb */
	if (!found) {
		DEBUG2(printk("scsi%ld: %s: ddb[%d] not found - allocating "
			      "new ddb\n", ha->host_no, __func__,
			      fw_ddb_index));
		*new_tgt = 1;
		ddb_entry = qla4xxx_alloc_ddb(ha, fw_ddb_index);
	}

exit_get_ddb_entry:
	dma_free_coherent(&ha->pdev->dev, sizeof(*fw_ddb_entry), fw_ddb_entry,
			  fw_ddb_entry_dma);

exit_get_ddb_entry_no_free:
	return ddb_entry;
}

/**
 * qla4xxx_update_ddb_entry - update driver's internal ddb
 * @ha: pointer to host adapter structure.
 * @ddb_entry: pointer to device database structure to be filled
 * @fw_ddb_index: index of the ddb entry in fw ddb table
 *
 * This routine updates the driver's internal device database entry
 * with information retrieved from the firmware's device database
 * entry for the specified device. The ddb_entry->fw_ddb_index field
 * must be initialized prior to	calling this routine
 *
 **/
static int qla4xxx_update_ddb_entry(struct scsi_qla_host *ha,
				    struct ddb_entry *ddb_entry,
				    uint32_t fw_ddb_index)
{
	struct dev_db_entry *fw_ddb_entry = NULL;
	dma_addr_t fw_ddb_entry_dma;
	int status = QLA_ERROR;
	uint32_t conn_err;

	if (ddb_entry == NULL) {
		DEBUG2(printk("scsi%ld: %s: ddb_entry is NULL\n", ha->host_no,
			      __func__));

		goto exit_update_ddb_no_free;
	}

	/* Make sure the dma buffer is valid */
	fw_ddb_entry = dma_alloc_coherent(&ha->pdev->dev,
					  sizeof(*fw_ddb_entry),
					  &fw_ddb_entry_dma, GFP_KERNEL);
	if (fw_ddb_entry == NULL) {
		DEBUG2(printk("scsi%ld: %s: Unable to allocate dma buffer.\n",
			      ha->host_no, __func__));

		goto exit_update_ddb_no_free;
	}

	if (qla4xxx_get_fwddb_entry(ha, fw_ddb_index, fw_ddb_entry,
				    fw_ddb_entry_dma, NULL, NULL,
				    &ddb_entry->fw_ddb_device_state, &conn_err,
				    &ddb_entry->tcp_source_port_num,
				    &ddb_entry->connection_id) ==
				    QLA_ERROR) {
		DEBUG2(printk("scsi%ld: %s: failed get_ddb_entry for "
			      "fw_ddb_index %d\n", ha->host_no, __func__,
			      fw_ddb_index));

		goto exit_update_ddb;
	}

	status = QLA_SUCCESS;
	ddb_entry->options = le16_to_cpu(fw_ddb_entry->options);
	ddb_entry->target_session_id = le16_to_cpu(fw_ddb_entry->tsid);
	ddb_entry->task_mgmt_timeout =
		le16_to_cpu(fw_ddb_entry->def_timeout);
	ddb_entry->CmdSn = 0;
	ddb_entry->exe_throttle = le16_to_cpu(fw_ddb_entry->exec_throttle);
	ddb_entry->default_relogin_timeout =
		le16_to_cpu(fw_ddb_entry->def_timeout);
	ddb_entry->default_time2wait = le16_to_cpu(fw_ddb_entry->iscsi_def_time2wait);

	/* Update index in case it changed */
	ddb_entry->fw_ddb_index = fw_ddb_index;
	ha->fw_ddb_index_map[fw_ddb_index] = ddb_entry;

	ddb_entry->port = le16_to_cpu(fw_ddb_entry->port);
	ddb_entry->tpgt = le32_to_cpu(fw_ddb_entry->tgt_portal_grp);
	memcpy(ddb_entry->isid, fw_ddb_entry->isid, sizeof(ddb_entry->isid));

	memcpy(&ddb_entry->iscsi_name[0], &fw_ddb_entry->iscsi_name[0],
	       min(sizeof(ddb_entry->iscsi_name),
		   sizeof(fw_ddb_entry->iscsi_name)));
	memcpy(&ddb_entry->iscsi_alias[0], &fw_ddb_entry->iscsi_alias[0],
	       min(sizeof(ddb_entry->iscsi_alias),
		   sizeof(fw_ddb_entry->iscsi_alias)));
	memcpy(&ddb_entry->ip_addr[0], &fw_ddb_entry->ip_addr[0],
	       min(sizeof(ddb_entry->ip_addr), sizeof(fw_ddb_entry->ip_addr)));

	ddb_entry->iscsi_max_burst_len = fw_ddb_entry->iscsi_max_burst_len;
	ddb_entry->iscsi_max_outsnd_r2t = fw_ddb_entry->iscsi_max_outsnd_r2t;
	ddb_entry->iscsi_first_burst_len = fw_ddb_entry->iscsi_first_burst_len;
	ddb_entry->iscsi_max_rcv_data_seg_len =
				fw_ddb_entry->iscsi_max_rcv_data_seg_len;
	ddb_entry->iscsi_max_snd_data_seg_len =
				fw_ddb_entry->iscsi_max_snd_data_seg_len;

	if (ddb_entry->options & DDB_OPT_IPV6_DEVICE) {
		memcpy(&ddb_entry->remote_ipv6_addr,
			fw_ddb_entry->ip_addr,
			min(sizeof(ddb_entry->remote_ipv6_addr),
			sizeof(fw_ddb_entry->ip_addr)));
		memcpy(&ddb_entry->link_local_ipv6_addr,
			fw_ddb_entry->link_local_ipv6_addr,
			min(sizeof(ddb_entry->link_local_ipv6_addr),
			sizeof(fw_ddb_entry->link_local_ipv6_addr)));

		DEBUG2(ql4_printk(KERN_INFO, ha, "%s: DDB[%d] State %04x"
					" ConnErr %08x IP %pI6 "
					":%04d \"%s\"\n",
					__func__, fw_ddb_index,
					ddb_entry->fw_ddb_device_state,
					conn_err, fw_ddb_entry->ip_addr,
					le16_to_cpu(fw_ddb_entry->port),
					fw_ddb_entry->iscsi_name));
	} else
		DEBUG2(ql4_printk(KERN_INFO, ha, "%s: DDB[%d] State %04x"
					" ConnErr %08x IP %pI4 "
					":%04d \"%s\"\n",
					__func__, fw_ddb_index,
					ddb_entry->fw_ddb_device_state,
					conn_err, fw_ddb_entry->ip_addr,
					le16_to_cpu(fw_ddb_entry->port),
					fw_ddb_entry->iscsi_name));
exit_update_ddb:
	if (fw_ddb_entry)
		dma_free_coherent(&ha->pdev->dev, sizeof(*fw_ddb_entry),
				  fw_ddb_entry, fw_ddb_entry_dma);

exit_update_ddb_no_free:
	return status;
}

/**
 * qla4xxx_alloc_ddb - allocate device database entry
 * @ha: Pointer to host adapter structure.
 * @fw_ddb_index: Firmware's device database index
 *
 * This routine allocates a ddb_entry, ititializes some values, and
 * inserts it into the ddb list.
 **/
static struct ddb_entry * qla4xxx_alloc_ddb(struct scsi_qla_host *ha,
					    uint32_t fw_ddb_index)
{
	struct ddb_entry *ddb_entry;

	DEBUG2(printk("scsi%ld: %s: fw_ddb_index [%d]\n", ha->host_no,
		      __func__, fw_ddb_index));

	ddb_entry = qla4xxx_alloc_sess(ha);
	if (ddb_entry == NULL) {
		DEBUG2(printk("scsi%ld: %s: Unable to allocate memory "
			      "to add fw_ddb_index [%d]\n",
			      ha->host_no, __func__, fw_ddb_index));
		return ddb_entry;
	}

	ddb_entry->fw_ddb_index = fw_ddb_index;
	atomic_set(&ddb_entry->retry_relogin_timer, INVALID_ENTRY);
	atomic_set(&ddb_entry->relogin_timer, 0);
	atomic_set(&ddb_entry->relogin_retry_count, 0);
	atomic_set(&ddb_entry->state, DDB_STATE_ONLINE);
	list_add_tail(&ddb_entry->list, &ha->ddb_list);
	ha->fw_ddb_index_map[fw_ddb_index] = ddb_entry;
	ha->tot_ddbs++;

	return ddb_entry;
}

/**
 * qla4_is_relogin_allowed - Are we allowed to login?
 * @ha: Pointer to host adapter structure.
 * @conn_err: Last connection error associated with the ddb
 *
 * This routine tests the given connection error to determine if
 * we are allowed to login.
 **/
int qla4_is_relogin_allowed(struct scsi_qla_host *ha, uint32_t conn_err)
{
	uint32_t err_code, login_rsp_sts_class;
	int relogin = 1;

	err_code = ((conn_err & 0x00ff0000) >> 16);
	login_rsp_sts_class = ((conn_err & 0x0000ff00) >> 8);
	if (err_code == 0x1c || err_code == 0x06) {
		DEBUG2(ql4_printk(KERN_INFO, ha,
		    ": conn_err=0x%08x, send target completed"
		    " or access denied failure\n", conn_err));
		relogin = 0;
	}
	if ((err_code == 0x08) && (login_rsp_sts_class == 0x02)) {
		/* Login Response PDU returned an error.
		   Login Response Status in Error Code Detail
		   indicates login should not be retried.*/
		DEBUG2(ql4_printk(KERN_INFO, ha,
		    ": conn_err=0x%08x, do not retry relogin\n",
		    conn_err));
		relogin = 0;
	}

	return relogin;
}

static void qla4xxx_flush_AENS(struct scsi_qla_host *ha)
{
	unsigned long wtime;

	/* Flush the 0x8014 AEN from the firmware as a result of
	 * Auto connect. We are basically doing get_firmware_ddb()
	 * to determine whether we need to log back in or not.
	 * Trying to do a set ddb before we have processed 0x8014
	 * will result in another set_ddb() for the same ddb. In other
	 * words there will be stale entries in the aen_q.
	 */
	wtime = jiffies + (2 * HZ);
	do {
		if (qla4xxx_get_firmware_state(ha) == QLA_SUCCESS)
			if (ha->firmware_state & (BIT_2 | BIT_0))
				return;

		if (test_and_clear_bit(DPC_AEN, &ha->dpc_flags))
			qla4xxx_process_aen(ha, FLUSH_DDB_CHANGED_AENS);

		msleep(1000);
	} while (!time_after_eq(jiffies, wtime));
}

/**
 * qla4xxx_build_ddb_list - builds driver ddb list
 * @ha: Pointer to host adapter structure.
 *
 * This routine searches for all valid firmware ddb entries and builds
 * an internal ddb list. Ddbs that are considered valid are those with
 * a device state of SESSION_ACTIVE.
 * A relogin (set_ddb) is issued for DDBs that are not online.
 **/
static int qla4xxx_build_ddb_list(struct scsi_qla_host *ha)
{
	int status = QLA_ERROR;
	uint32_t fw_ddb_index = 0;
	uint32_t next_fw_ddb_index = 0;
	uint32_t ddb_state;
	uint32_t conn_err;
	struct ddb_entry *ddb_entry;
	struct dev_db_entry *fw_ddb_entry = NULL;
	dma_addr_t fw_ddb_entry_dma;
	uint32_t ipv6_device;
	uint32_t new_tgt;

	qla4xxx_flush_AENS(ha);

	fw_ddb_entry = dma_alloc_coherent(&ha->pdev->dev, sizeof(*fw_ddb_entry),
			&fw_ddb_entry_dma, GFP_KERNEL);
	if (fw_ddb_entry == NULL) {
		DEBUG2(ql4_printk(KERN_INFO, ha, "%s: DMA alloc failed\n",
				__func__));

		goto exit_build_ddb_list_no_free;
	}

	ql4_printk(KERN_INFO, ha, "Initializing DDBs ...\n");
	for (fw_ddb_index = 0; fw_ddb_index < MAX_DDB_ENTRIES;
	     fw_ddb_index = next_fw_ddb_index) {
		/* First, let's see if a device exists here */
		if (qla4xxx_get_fwddb_entry(ha, fw_ddb_index, fw_ddb_entry,
					    0, NULL, &next_fw_ddb_index,
					    &ddb_state, &conn_err,
					    NULL, NULL) ==
					    QLA_ERROR) {
			DEBUG2(printk("scsi%ld: %s: get_ddb_entry, "
				      "fw_ddb_index %d failed", ha->host_no,
				      __func__, fw_ddb_index));
			goto exit_build_ddb_list;
		}

		DEBUG2(printk("scsi%ld: %s: Getting DDB[%d] ddbstate=0x%x, "
			      "next_fw_ddb_index=%d.\n", ha->host_no, __func__,
			      fw_ddb_index, ddb_state, next_fw_ddb_index));

		/* Issue relogin, if necessary. */
		if (ddb_state == DDB_DS_SESSION_FAILED ||
		    ddb_state == DDB_DS_NO_CONNECTION_ACTIVE) {
			/* Try and login to device */
			DEBUG2(printk("scsi%ld: %s: Login to DDB[%d]\n",
				      ha->host_no, __func__, fw_ddb_index));
			ipv6_device = le16_to_cpu(fw_ddb_entry->options) &
					DDB_OPT_IPV6_DEVICE;
			if (qla4_is_relogin_allowed(ha, conn_err) &&
					((!ipv6_device &&
					  *((uint32_t *)fw_ddb_entry->ip_addr))
					 || ipv6_device)) {
				qla4xxx_set_ddb_entry(ha, fw_ddb_index, 0);
				if (qla4xxx_get_fwddb_entry(ha, fw_ddb_index,
							NULL, 0, NULL,
							&next_fw_ddb_index,
							&ddb_state, &conn_err,
							NULL, NULL)
						== QLA_ERROR) {
					DEBUG2(printk("scsi%ld: %s:"
						"get_ddb_entry %d failed\n",
						ha->host_no,
						__func__, fw_ddb_index));
					goto exit_build_ddb_list;
				}
			}
		}

		if (ddb_state != DDB_DS_SESSION_ACTIVE)
			goto next_one;
		/*
		 * if fw_ddb with session active state found,
		 * add to ddb_list
		 */
		DEBUG2(printk("scsi%ld: %s: DDB[%d] added to list\n",
			      ha->host_no, __func__, fw_ddb_index));

		/* Add DDB to internal our ddb list. */
		ddb_entry = qla4xxx_get_ddb_entry(ha, fw_ddb_index, &new_tgt);
		if (ddb_entry == NULL) {
			DEBUG2(printk("scsi%ld: %s: Unable to allocate memory "
				      "for device at fw_ddb_index %d\n",
				      ha->host_no, __func__, fw_ddb_index));
			goto exit_build_ddb_list;
		}
		/* Fill in the device structure */
		if (qla4xxx_update_ddb_entry(ha, ddb_entry, fw_ddb_index) ==
		    QLA_ERROR) {
			ha->fw_ddb_index_map[fw_ddb_index] =
				(struct ddb_entry *)INVALID_ENTRY;

			DEBUG2(printk("scsi%ld: %s: update_ddb_entry failed "
				      "for fw_ddb_index %d.\n",
				      ha->host_no, __func__, fw_ddb_index));
			goto exit_build_ddb_list;
		}

next_one:
		/* We know we've reached the last device when
		 * next_fw_ddb_index is 0 */
		if (next_fw_ddb_index == 0)
			break;
	}

	status = QLA_SUCCESS;
	ql4_printk(KERN_INFO, ha, "DDB list done..\n");

exit_build_ddb_list:
	dma_free_coherent(&ha->pdev->dev, sizeof(*fw_ddb_entry), fw_ddb_entry,
		fw_ddb_entry_dma);

exit_build_ddb_list_no_free:
	return status;
}

static int qla4xxx_initialize_ddb_list(struct scsi_qla_host *ha)
{
	uint16_t fw_ddb_index;
	int status = QLA_SUCCESS;

	/* free the ddb list if is not empty */
	if (!list_empty(&ha->ddb_list))
		qla4xxx_free_ddb_list(ha);

	for (fw_ddb_index = 0; fw_ddb_index < MAX_DDB_ENTRIES; fw_ddb_index++)
		ha->fw_ddb_index_map[fw_ddb_index] =
		    (struct ddb_entry *)INVALID_ENTRY;

	ha->tot_ddbs = 0;

	/* Perform device discovery and build ddb list. */
	status = qla4xxx_build_ddb_list(ha);

	return status;
}

/**
 * qla4xxx_reinitialize_ddb_list - update the driver ddb list
 * @ha: pointer to host adapter structure.
 *
 * This routine obtains device information from the F/W database after
 * firmware or adapter resets.  The device table is preserved.
 **/
int qla4xxx_reinitialize_ddb_list(struct scsi_qla_host *ha)
{
	int status = QLA_SUCCESS;
	struct ddb_entry *ddb_entry, *detemp;

	/* Update the device information for all devices. */
	list_for_each_entry_safe(ddb_entry, detemp, &ha->ddb_list, list) {
		qla4xxx_update_ddb_entry(ha, ddb_entry,
					 ddb_entry->fw_ddb_index);
		if (ddb_entry->fw_ddb_device_state == DDB_DS_SESSION_ACTIVE) {
			atomic_set(&ddb_entry->state, DDB_STATE_ONLINE);
			DEBUG2(printk ("scsi%ld: %s: ddb index [%d] marked "
				       "ONLINE\n", ha->host_no, __func__,
				       ddb_entry->fw_ddb_index));
			iscsi_unblock_session(ddb_entry->sess);
		} else if (atomic_read(&ddb_entry->state) == DDB_STATE_ONLINE)
			qla4xxx_mark_device_missing(ha, ddb_entry);
	}
	return status;
}

/**
 * qla4xxx_relogin_device - re-establish session
 * @ha: Pointer to host adapter structure.
 * @ddb_entry: Pointer to device database entry
 *
 * This routine does a session relogin with the specified device.
 * The ddb entry must be assigned prior to making this call.
 **/
int qla4xxx_relogin_device(struct scsi_qla_host *ha,
			   struct ddb_entry * ddb_entry)
{
	uint16_t relogin_timer;

	relogin_timer = max(ddb_entry->default_relogin_timeout,
			    (uint16_t)RELOGIN_TOV);
	atomic_set(&ddb_entry->relogin_timer, relogin_timer);

	DEBUG2(printk("scsi%ld: Relogin ddb [%d]. TOV=%d\n", ha->host_no,
		      ddb_entry->fw_ddb_index, relogin_timer));

	qla4xxx_set_ddb_entry(ha, ddb_entry->fw_ddb_index, 0);

	return QLA_SUCCESS;
=======
=======

	if (!qla4xxx_fw_ready(ha))
		return status;

	if (is_qla80XX(ha) && !test_bit(AF_INIT_DONE, &ha->flags))
		qla4xxx_alloc_fw_dump(ha);

	return qla4xxx_get_firmware_status(ha);
}

>>>>>>> refs/remotes/origin/master
static void qla4xxx_set_model_info(struct scsi_qla_host *ha)
{
	uint16_t board_id_string[8];
	int i;
	int size = sizeof(ha->nvram->isp4022.boardIdStr);
	int offset = offsetof(struct eeprom_data, isp4022.boardIdStr) / 2;

	for (i = 0; i < (size / 2) ; i++) {
		board_id_string[i] = rd_nvram_word(ha, offset);
		offset += 1;
	}

	memcpy(ha->model_name, board_id_string, size);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static int qla4xxx_config_nvram(struct scsi_qla_host *ha)
{
	unsigned long flags;
	union external_hw_config_reg extHwConfig;

	DEBUG2(printk("scsi%ld: %s: Get EEProm parameters \n", ha->host_no,
		      __func__));
	if (ql4xxx_lock_flash(ha) != QLA_SUCCESS)
		return QLA_ERROR;
	if (ql4xxx_lock_nvram(ha) != QLA_SUCCESS) {
		ql4xxx_unlock_flash(ha);
		return QLA_ERROR;
	}

	/* Get EEPRom Parameters from NVRAM and validate */
	ql4_printk(KERN_INFO, ha, "Configuring NVRAM ...\n");
	if (qla4xxx_is_nvram_configuration_valid(ha) == QLA_SUCCESS) {
		spin_lock_irqsave(&ha->hardware_lock, flags);
		extHwConfig.Asuint32_t =
			rd_nvram_word(ha, eeprom_ext_hw_conf_offset(ha));
		spin_unlock_irqrestore(&ha->hardware_lock, flags);
	} else {
		ql4_printk(KERN_WARNING, ha,
		    "scsi%ld: %s: EEProm checksum invalid.  "
		    "Please update your EEPROM\n", ha->host_no,
		    __func__);

		/* Attempt to set defaults */
		if (is_qla4010(ha))
			extHwConfig.Asuint32_t = 0x1912;
		else if (is_qla4022(ha) | is_qla4032(ha))
			extHwConfig.Asuint32_t = 0x0023;
		else
			return QLA_ERROR;
	}
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master

	if (is_qla4022(ha) || is_qla4032(ha))
		qla4xxx_set_model_info(ha);
	else
		strcpy(ha->model_name, "QLA4010");

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	DEBUG(printk("scsi%ld: %s: Setting extHwConfig to 0xFFFF%04x\n",
		     ha->host_no, __func__, extHwConfig.Asuint32_t));

	spin_lock_irqsave(&ha->hardware_lock, flags);
	writel((0xFFFF << 16) | extHwConfig.Asuint32_t, isp_ext_hw_conf(ha));
	readl(isp_ext_hw_conf(ha));
	spin_unlock_irqrestore(&ha->hardware_lock, flags);

	ql4xxx_unlock_nvram(ha);
	ql4xxx_unlock_flash(ha);

	return QLA_SUCCESS;
}

/**
 * qla4_8xxx_pci_config() - Setup ISP82xx PCI configuration registers.
 * @ha: HA context
 */
void qla4_8xxx_pci_config(struct scsi_qla_host *ha)
{
	pci_set_master(ha->pdev);
}

void qla4xxx_pci_config(struct scsi_qla_host *ha)
{
	uint16_t w;
	int status;

	ql4_printk(KERN_INFO, ha, "Configuring PCI space...\n");

	pci_set_master(ha->pdev);
	status = pci_set_mwi(ha->pdev);
	/*
	 * We want to respect framework's setting of PCI configuration space
	 * command register and also want to make sure that all bits of
	 * interest to us are properly set in command register.
	 */
	pci_read_config_word(ha->pdev, PCI_COMMAND, &w);
	w |= PCI_COMMAND_PARITY | PCI_COMMAND_SERR;
	w &= ~PCI_COMMAND_INTX_DISABLE;
	pci_write_config_word(ha->pdev, PCI_COMMAND, w);
}

static int qla4xxx_start_firmware_from_flash(struct scsi_qla_host *ha)
{
	int status = QLA_ERROR;
	unsigned long max_wait_time;
	unsigned long flags;
	uint32_t mbox_status;

	ql4_printk(KERN_INFO, ha, "Starting firmware ...\n");

	/*
	 * Start firmware from flash ROM
	 *
	 * WORKAROUND: Stuff a non-constant value that the firmware can
	 * use as a seed for a random number generator in MB7 prior to
	 * setting BOOT_ENABLE.	 Fixes problem where the TCP
	 * connections use the same TCP ports after each reboot,
	 * causing some connections to not get re-established.
	 */
	DEBUG(printk("scsi%d: %s: Start firmware from flash ROM\n",
		     ha->host_no, __func__));

	spin_lock_irqsave(&ha->hardware_lock, flags);
	writel(jiffies, &ha->reg->mailbox[7]);
	if (is_qla4022(ha) | is_qla4032(ha))
		writel(set_rmask(NVR_WRITE_ENABLE),
		       &ha->reg->u1.isp4022.nvram);

        writel(2, &ha->reg->mailbox[6]);
        readl(&ha->reg->mailbox[6]);

	writel(set_rmask(CSR_BOOT_ENABLE), &ha->reg->ctrl_status);
	readl(&ha->reg->ctrl_status);
	spin_unlock_irqrestore(&ha->hardware_lock, flags);

	/* Wait for firmware to come UP. */
	DEBUG2(printk(KERN_INFO "scsi%ld: %s: Wait up to %d seconds for "
		      "boot firmware to complete...\n",
		      ha->host_no, __func__, FIRMWARE_UP_TOV));
	max_wait_time = jiffies + (FIRMWARE_UP_TOV * HZ);
	do {
		uint32_t ctrl_status;

		spin_lock_irqsave(&ha->hardware_lock, flags);
		ctrl_status = readw(&ha->reg->ctrl_status);
		mbox_status = readw(&ha->reg->mailbox[0]);
		spin_unlock_irqrestore(&ha->hardware_lock, flags);

		if (ctrl_status & set_rmask(CSR_SCSI_PROCESSOR_INTR))
			break;
		if (mbox_status == MBOX_STS_COMMAND_COMPLETE)
			break;

		DEBUG2(printk(KERN_INFO "scsi%ld: %s: Waiting for boot "
		    "firmware to complete... ctrl_sts=0x%x, remaining=%ld\n",
		    ha->host_no, __func__, ctrl_status, max_wait_time));

		msleep_interruptible(250);
	} while (!time_after_eq(jiffies, max_wait_time));

	if (mbox_status == MBOX_STS_COMMAND_COMPLETE) {
		DEBUG(printk(KERN_INFO "scsi%ld: %s: Firmware has started\n",
			     ha->host_no, __func__));

		spin_lock_irqsave(&ha->hardware_lock, flags);
		writel(set_rmask(CSR_SCSI_PROCESSOR_INTR),
		       &ha->reg->ctrl_status);
		readl(&ha->reg->ctrl_status);
		spin_unlock_irqrestore(&ha->hardware_lock, flags);

		status = QLA_SUCCESS;
	} else {
		printk(KERN_INFO "scsi%ld: %s: Boot firmware failed "
		       "-  mbox status 0x%x\n", ha->host_no, __func__,
		       mbox_status);
		status = QLA_ERROR;
	}
	return status;
}

int ql4xxx_lock_drvr_wait(struct scsi_qla_host *a)
{
#define QL4_LOCK_DRVR_WAIT	60
#define QL4_LOCK_DRVR_SLEEP	1

	int drvr_wait = QL4_LOCK_DRVR_WAIT;
	while (drvr_wait) {
		if (ql4xxx_lock_drvr(a) == 0) {
			ssleep(QL4_LOCK_DRVR_SLEEP);
			if (drvr_wait) {
				DEBUG2(printk("scsi%ld: %s: Waiting for "
					      "Global Init Semaphore(%d)...\n",
					      a->host_no,
					      __func__, drvr_wait));
			}
			drvr_wait -= QL4_LOCK_DRVR_SLEEP;
		} else {
			DEBUG2(printk("scsi%ld: %s: Global Init Semaphore "
				      "acquired\n", a->host_no, __func__));
			return QLA_SUCCESS;
		}
	}
	return QLA_ERROR;
}

/**
 * qla4xxx_start_firmware - starts qla4xxx firmware
 * @ha: Pointer to host adapter structure.
 *
 * This routine performs the necessary steps to start the firmware for
 * the QLA4010 adapter.
 **/
int qla4xxx_start_firmware(struct scsi_qla_host *ha)
{
	unsigned long flags = 0;
	uint32_t mbox_status;
	int status = QLA_ERROR;
	int soft_reset = 1;
	int config_chip = 0;

	if (is_qla4022(ha) | is_qla4032(ha))
		ql4xxx_set_mac_number(ha);

	if (ql4xxx_lock_drvr_wait(ha) != QLA_SUCCESS)
		return QLA_ERROR;

	spin_lock_irqsave(&ha->hardware_lock, flags);

	DEBUG2(printk("scsi%ld: %s: port_ctrl	= 0x%08X\n", ha->host_no,
		      __func__, readw(isp_port_ctrl(ha))));
	DEBUG(printk("scsi%ld: %s: port_status = 0x%08X\n", ha->host_no,
		     __func__, readw(isp_port_status(ha))));

	/* Is Hardware already initialized? */
	if ((readw(isp_port_ctrl(ha)) & 0x8000) != 0) {
		DEBUG(printk("scsi%ld: %s: Hardware has already been "
			     "initialized\n", ha->host_no, __func__));

		/* Receive firmware boot acknowledgement */
		mbox_status = readw(&ha->reg->mailbox[0]);

		DEBUG2(printk("scsi%ld: %s: H/W Config complete - mbox[0]= "
			      "0x%x\n", ha->host_no, __func__, mbox_status));

		/* Is firmware already booted? */
		if (mbox_status == 0) {
			/* F/W not running, must be config by net driver */
			config_chip = 1;
			soft_reset = 0;
		} else {
			writel(set_rmask(CSR_SCSI_PROCESSOR_INTR),
			       &ha->reg->ctrl_status);
			readl(&ha->reg->ctrl_status);
<<<<<<< HEAD
<<<<<<< HEAD
=======
			writel(set_rmask(CSR_SCSI_COMPLETION_INTR),
			       &ha->reg->ctrl_status);
			readl(&ha->reg->ctrl_status);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			writel(set_rmask(CSR_SCSI_COMPLETION_INTR),
			       &ha->reg->ctrl_status);
			readl(&ha->reg->ctrl_status);
>>>>>>> refs/remotes/origin/master
			spin_unlock_irqrestore(&ha->hardware_lock, flags);
			if (qla4xxx_get_firmware_state(ha) == QLA_SUCCESS) {
				DEBUG2(printk("scsi%ld: %s: Get firmware "
					      "state -- state = 0x%x\n",
					      ha->host_no,
					      __func__, ha->firmware_state));
				/* F/W is running */
				if (ha->firmware_state &
				    FW_STATE_CONFIG_WAIT) {
					DEBUG2(printk("scsi%ld: %s: Firmware "
						      "in known state -- "
						      "config and "
						      "boot, state = 0x%x\n",
						      ha->host_no, __func__,
						      ha->firmware_state));
					config_chip = 1;
					soft_reset = 0;
				}
			} else {
				DEBUG2(printk("scsi%ld: %s: Firmware in "
					      "unknown state -- resetting,"
					      " state = "
					      "0x%x\n", ha->host_no, __func__,
					      ha->firmware_state));
			}
			spin_lock_irqsave(&ha->hardware_lock, flags);
		}
	} else {
		DEBUG(printk("scsi%ld: %s: H/W initialization hasn't been "
			     "started - resetting\n", ha->host_no, __func__));
	}
	spin_unlock_irqrestore(&ha->hardware_lock, flags);

	DEBUG(printk("scsi%ld: %s: Flags soft_rest=%d, config= %d\n ",
		     ha->host_no, __func__, soft_reset, config_chip));
	if (soft_reset) {
		DEBUG(printk("scsi%ld: %s: Issue Soft Reset\n", ha->host_no,
			     __func__));
		status = qla4xxx_soft_reset(ha);	/* NOTE: acquires drvr
							 * lock again, but ok */
		if (status == QLA_ERROR) {
			DEBUG(printk("scsi%d: %s: Soft Reset failed!\n",
				     ha->host_no, __func__));
			ql4xxx_unlock_drvr(ha);
			return QLA_ERROR;
		}
		config_chip = 1;

		/* Reset clears the semaphore, so acquire again */
		if (ql4xxx_lock_drvr_wait(ha) != QLA_SUCCESS)
			return QLA_ERROR;
	}

	if (config_chip) {
		if ((status = qla4xxx_config_nvram(ha)) == QLA_SUCCESS)
			status = qla4xxx_start_firmware_from_flash(ha);
	}

	ql4xxx_unlock_drvr(ha);
	if (status == QLA_SUCCESS) {
		if (test_and_clear_bit(AF_GET_CRASH_RECORD, &ha->flags))
			qla4xxx_get_crash_record(ha);
	} else {
		DEBUG(printk("scsi%ld: %s: Firmware has NOT started\n",
			     ha->host_no, __func__));
	}
	return status;
}
<<<<<<< HEAD
<<<<<<< HEAD

=======
=======
>>>>>>> refs/remotes/origin/master
/**
 * qla4xxx_free_ddb_index - Free DDBs reserved by firmware
 * @ha: pointer to adapter structure
 *
 * Since firmware is not running in autoconnect mode the DDB indices should
 * be freed so that when login happens from user space there are free DDB
 * indices available.
 **/
void qla4xxx_free_ddb_index(struct scsi_qla_host *ha)
{
	int max_ddbs;
	int ret;
	uint32_t idx = 0, next_idx = 0;
	uint32_t state = 0, conn_err = 0;

	max_ddbs =  is_qla40XX(ha) ? MAX_DEV_DB_ENTRIES_40XX :
				     MAX_DEV_DB_ENTRIES;

	for (idx = 0; idx < max_ddbs; idx = next_idx) {
		ret = qla4xxx_get_fwddb_entry(ha, idx, NULL, 0, NULL,
					      &next_idx, &state, &conn_err,
						NULL, NULL);
		if (ret == QLA_ERROR) {
			next_idx++;
			continue;
		}
		if (state == DDB_DS_NO_CONNECTION_ACTIVE ||
		    state == DDB_DS_SESSION_FAILED) {
			DEBUG2(ql4_printk(KERN_INFO, ha,
					  "Freeing DDB index = 0x%x\n", idx));
			ret = qla4xxx_clear_ddb_entry(ha, idx);
			if (ret == QLA_ERROR)
				ql4_printk(KERN_ERR, ha,
					   "Unable to clear DDB index = "
					   "0x%x\n", idx);
		}
		if (next_idx == 0)
			break;
	}
}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

/**
 * qla4xxx_initialize_adapter - initiailizes hba
 * @ha: Pointer to host adapter structure.
<<<<<<< HEAD
<<<<<<< HEAD
 * @renew_ddb_list: Indicates what to do with the adapter's ddb list
 *	after adapter recovery has completed.
 *	0=preserve ddb list, 1=destroy and rebuild ddb list
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 *
 * This routine parforms all of the steps necessary to initialize the adapter.
 *
 **/
<<<<<<< HEAD
<<<<<<< HEAD
int qla4xxx_initialize_adapter(struct scsi_qla_host *ha,
			       uint8_t renew_ddb_list)
{
	int status = QLA_ERROR;
	int8_t ip_address[IP_ADDR_LEN] = {0} ;
=======
int qla4xxx_initialize_adapter(struct scsi_qla_host *ha, int is_reset)
{
	int status = QLA_ERROR;
>>>>>>> refs/remotes/origin/cm-10.0
=======
int qla4xxx_initialize_adapter(struct scsi_qla_host *ha, int is_reset)
{
	int status = QLA_ERROR;
>>>>>>> refs/remotes/origin/master

	ha->eeprom_cmd_data = 0;

	ql4_printk(KERN_INFO, ha, "Configuring PCI space...\n");
	ha->isp_ops->pci_config(ha);

	ha->isp_ops->disable_intrs(ha);

	/* Initialize the Host adapter request/response queues and firmware */
	if (ha->isp_ops->start_firmware(ha) == QLA_ERROR)
		goto exit_init_hba;

<<<<<<< HEAD
=======
	/*
	 * For ISP83XX, mailbox and IOCB interrupts are enabled separately.
	 * Mailbox interrupts must be enabled prior to issuing any mailbox
	 * command in order to prevent the possibility of losing interrupts
	 * while switching from polling to interrupt mode. IOCB interrupts are
	 * enabled via isp_ops->enable_intrs.
	 */
	if (is_qla8032(ha) || is_qla8042(ha))
		qla4_83xx_enable_mbox_intrs(ha);

>>>>>>> refs/remotes/origin/master
	if (qla4xxx_about_firmware(ha) == QLA_ERROR)
		goto exit_init_hba;

	if (ha->isp_ops->get_sys_info(ha) == QLA_ERROR)
		goto exit_init_hba;

<<<<<<< HEAD
	if (qla4xxx_init_local_data(ha) == QLA_ERROR)
		goto exit_init_hba;
=======
	qla4xxx_init_local_data(ha);
>>>>>>> refs/remotes/origin/master

	status = qla4xxx_init_firmware(ha);
	if (status == QLA_ERROR)
		goto exit_init_hba;

<<<<<<< HEAD
<<<<<<< HEAD
	/*
	 * FW is waiting to get an IP address from DHCP server: Skip building
	 * the ddb_list and wait for DHCP lease acquired aen to come in
	 * followed by 0x8014 aen" to trigger the tgt discovery process.
	 */
	if (ha->firmware_state & FW_STATE_CONFIGURING_IP)
		goto exit_init_online;

	/* Skip device discovery if ip and subnet is zero */
	if (memcmp(ha->ip_address, ip_address, IP_ADDR_LEN) == 0 ||
	    memcmp(ha->subnet_mask, ip_address, IP_ADDR_LEN) == 0)
		goto exit_init_online;

	if (renew_ddb_list == PRESERVE_DDB_LIST) {
		/*
		 * We want to preserve lun states (i.e. suspended, etc.)
		 * for recovery initiated by the driver.  So just update
		 * the device states for the existing ddb_list.
		 */
		qla4xxx_reinitialize_ddb_list(ha);
	} else if (renew_ddb_list == REBUILD_DDB_LIST) {
		/*
		 * We want to build the ddb_list from scratch during
		 * driver initialization and recovery initiated by the
		 * INT_HBA_RESET IOCTL.
		 */
		status = qla4xxx_initialize_ddb_list(ha);
		if (status == QLA_ERROR) {
			DEBUG2(printk("%s(%ld) Error occurred during build"
				      "ddb list\n", __func__, ha->host_no));
			goto exit_init_hba;
		}

	}
	if (!ha->tot_ddbs) {
		DEBUG2(printk("scsi%ld: Failed to initialize devices or none "
			      "present in Firmware device database\n",
			      ha->host_no));
	}

exit_init_online:
=======
	if (is_reset == RESET_ADAPTER)
		qla4xxx_build_ddb_list(ha, is_reset);

>>>>>>> refs/remotes/origin/cm-10.0
	set_bit(AF_ONLINE, &ha->flags);
exit_init_hba:
	if (is_qla8022(ha) && (status == QLA_ERROR)) {
		/* Since interrupts are registered in start_firmware for
		 * 82xx, release them here if initialize_adapter fails */
=======
	if (is_reset == RESET_ADAPTER)
		qla4xxx_build_ddb_list(ha, is_reset);

	set_bit(AF_ONLINE, &ha->flags);
exit_init_hba:
	if (is_qla80XX(ha) && (status == QLA_ERROR)) {
		/* Since interrupts are registered in start_firmware for
		 * 80XX, release them here if initialize_adapter fails */
>>>>>>> refs/remotes/origin/master
		qla4xxx_free_irqs(ha);
	}

	DEBUG2(printk("scsi%ld: initialize adapter: %s\n", ha->host_no,
	    status == QLA_ERROR ? "FAILED" : "SUCCEEDED"));
	return status;
}

<<<<<<< HEAD
<<<<<<< HEAD
/**
 * qla4xxx_add_device_dynamically - ddb addition due to an AEN
 * @ha:  Pointer to host adapter structure.
 * @fw_ddb_index:  Firmware's device database index
 *
 * This routine processes adds a device as a result of an 8014h AEN.
 **/
static void qla4xxx_add_device_dynamically(struct scsi_qla_host *ha,
					   uint32_t fw_ddb_index)
{
	struct ddb_entry * ddb_entry;
	uint32_t new_tgt;

	/* First allocate a device structure */
	ddb_entry = qla4xxx_get_ddb_entry(ha, fw_ddb_index, &new_tgt);
	if (ddb_entry == NULL) {
		DEBUG2(printk(KERN_WARNING
			      "scsi%ld: Unable to allocate memory to add "
			      "fw_ddb_index %d\n", ha->host_no, fw_ddb_index));
		return;
	}

	if (!new_tgt && (ddb_entry->fw_ddb_index != fw_ddb_index)) {
		/* Target has been bound to a new fw_ddb_index */
		qla4xxx_free_ddb(ha, ddb_entry);
		ddb_entry = qla4xxx_alloc_ddb(ha, fw_ddb_index);
		if (ddb_entry == NULL) {
			DEBUG2(printk(KERN_WARNING
				"scsi%ld: Unable to allocate memory"
				" to add fw_ddb_index %d\n",
				ha->host_no, fw_ddb_index));
			return;
		}
	}
	if (qla4xxx_update_ddb_entry(ha, ddb_entry, fw_ddb_index) ==
				    QLA_ERROR) {
		ha->fw_ddb_index_map[fw_ddb_index] =
					(struct ddb_entry *)INVALID_ENTRY;
		DEBUG2(printk(KERN_WARNING
			      "scsi%ld: failed to add new device at index "
			      "[%d]\n Unable to retrieve fw ddb entry\n",
			      ha->host_no, fw_ddb_index));
		qla4xxx_free_ddb(ha, ddb_entry);
		return;
	}

	if (qla4xxx_add_sess(ddb_entry)) {
		DEBUG2(printk(KERN_WARNING
			      "scsi%ld: failed to add new device at index "
			      "[%d]\n Unable to add connection and session\n",
			      ha->host_no, fw_ddb_index));
		qla4xxx_free_ddb(ha, ddb_entry);
	}
=======
=======
>>>>>>> refs/remotes/origin/master
int qla4xxx_ddb_change(struct scsi_qla_host *ha, uint32_t fw_ddb_index,
		       struct ddb_entry *ddb_entry, uint32_t state)
{
	uint32_t old_fw_ddb_device_state;
	int status = QLA_ERROR;

	old_fw_ddb_device_state = ddb_entry->fw_ddb_device_state;
	DEBUG2(ql4_printk(KERN_INFO, ha,
			  "%s: DDB - old state = 0x%x, new state = 0x%x for "
			  "index [%d]\n", __func__,
			  ddb_entry->fw_ddb_device_state, state, fw_ddb_index));

	ddb_entry->fw_ddb_device_state = state;

	switch (old_fw_ddb_device_state) {
	case DDB_DS_LOGIN_IN_PROCESS:
		switch (state) {
		case DDB_DS_SESSION_ACTIVE:
		case DDB_DS_DISCOVERY:
<<<<<<< HEAD
			ddb_entry->unblock_sess(ddb_entry->sess);
			qla4xxx_update_session_conn_param(ha, ddb_entry);
=======
			qla4xxx_update_session_conn_param(ha, ddb_entry);
			ddb_entry->unblock_sess(ddb_entry->sess);
>>>>>>> refs/remotes/origin/master
			status = QLA_SUCCESS;
			break;
		case DDB_DS_SESSION_FAILED:
		case DDB_DS_NO_CONNECTION_ACTIVE:
			iscsi_conn_login_event(ddb_entry->conn,
					       ISCSI_CONN_STATE_FREE);
			status = QLA_SUCCESS;
			break;
		}
		break;
	case DDB_DS_SESSION_ACTIVE:
<<<<<<< HEAD
=======
	case DDB_DS_DISCOVERY:
>>>>>>> refs/remotes/origin/master
		switch (state) {
		case DDB_DS_SESSION_FAILED:
			/*
			 * iscsi_session failure  will cause userspace to
			 * stop the connection which in turn would block the
			 * iscsi_session and start relogin
			 */
			iscsi_session_failure(ddb_entry->sess->dd_data,
					      ISCSI_ERR_CONN_FAILED);
			status = QLA_SUCCESS;
			break;
		case DDB_DS_NO_CONNECTION_ACTIVE:
			clear_bit(fw_ddb_index, ha->ddb_idx_map);
			status = QLA_SUCCESS;
			break;
		}
		break;
	case DDB_DS_SESSION_FAILED:
		switch (state) {
		case DDB_DS_SESSION_ACTIVE:
		case DDB_DS_DISCOVERY:
			ddb_entry->unblock_sess(ddb_entry->sess);
			qla4xxx_update_session_conn_param(ha, ddb_entry);
			status = QLA_SUCCESS;
			break;
		case DDB_DS_SESSION_FAILED:
			iscsi_session_failure(ddb_entry->sess->dd_data,
					      ISCSI_ERR_CONN_FAILED);
			status = QLA_SUCCESS;
			break;
		}
		break;
	default:
		DEBUG2(ql4_printk(KERN_INFO, ha, "%s: Unknown Event\n",
				__func__));
		break;
	}
	return status;
}

void qla4xxx_arm_relogin_timer(struct ddb_entry *ddb_entry)
{
	/*
	 * This triggers a relogin.  After the relogin_timer
	 * expires, the relogin gets scheduled.  We must wait a
	 * minimum amount of time since receiving an 0x8014 AEN
	 * with failed device_state or a logout response before
	 * we can issue another relogin.
	 *
	 * Firmware pads this timeout: (time2wait +1).
	 * Driver retry to login should be longer than F/W.
	 * Otherwise F/W will fail
	 * set_ddb() mbx cmd with 0x4005 since it still
	 * counting down its time2wait.
	 */
	atomic_set(&ddb_entry->relogin_timer, 0);
	atomic_set(&ddb_entry->retry_relogin_timer,
		   ddb_entry->default_time2wait + 4);

}

int qla4xxx_flash_ddb_change(struct scsi_qla_host *ha, uint32_t fw_ddb_index,
			     struct ddb_entry *ddb_entry, uint32_t state)
{
	uint32_t old_fw_ddb_device_state;
	int status = QLA_ERROR;

	old_fw_ddb_device_state = ddb_entry->fw_ddb_device_state;
	DEBUG2(ql4_printk(KERN_INFO, ha,
			  "%s: DDB - old state = 0x%x, new state = 0x%x for "
			  "index [%d]\n", __func__,
			  ddb_entry->fw_ddb_device_state, state, fw_ddb_index));

	ddb_entry->fw_ddb_device_state = state;

	switch (old_fw_ddb_device_state) {
	case DDB_DS_LOGIN_IN_PROCESS:
	case DDB_DS_NO_CONNECTION_ACTIVE:
		switch (state) {
		case DDB_DS_SESSION_ACTIVE:
			ddb_entry->unblock_sess(ddb_entry->sess);
			qla4xxx_update_session_conn_fwddb_param(ha, ddb_entry);
			status = QLA_SUCCESS;
			break;
		case DDB_DS_SESSION_FAILED:
			iscsi_block_session(ddb_entry->sess);
			if (!test_bit(DF_RELOGIN, &ddb_entry->flags))
				qla4xxx_arm_relogin_timer(ddb_entry);
			status = QLA_SUCCESS;
			break;
		}
		break;
	case DDB_DS_SESSION_ACTIVE:
		switch (state) {
		case DDB_DS_SESSION_FAILED:
			iscsi_block_session(ddb_entry->sess);
			if (!test_bit(DF_RELOGIN, &ddb_entry->flags))
				qla4xxx_arm_relogin_timer(ddb_entry);
			status = QLA_SUCCESS;
			break;
		}
		break;
	case DDB_DS_SESSION_FAILED:
		switch (state) {
		case DDB_DS_SESSION_ACTIVE:
			ddb_entry->unblock_sess(ddb_entry->sess);
			qla4xxx_update_session_conn_fwddb_param(ha, ddb_entry);
			status = QLA_SUCCESS;
			break;
		case DDB_DS_SESSION_FAILED:
			if (!test_bit(DF_RELOGIN, &ddb_entry->flags))
				qla4xxx_arm_relogin_timer(ddb_entry);
			status = QLA_SUCCESS;
			break;
		}
		break;
	default:
		DEBUG2(ql4_printk(KERN_INFO, ha, "%s: Unknown Event\n",
				  __func__));
		break;
	}
	return status;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

/**
 * qla4xxx_process_ddb_changed - process ddb state change
 * @ha - Pointer to host adapter structure.
 * @fw_ddb_index - Firmware's device database index
 * @state - Device state
 *
 * This routine processes a Decive Database Changed AEN Event.
 **/
<<<<<<< HEAD
<<<<<<< HEAD
int qla4xxx_process_ddb_changed(struct scsi_qla_host *ha, uint32_t fw_ddb_index,
		uint32_t state, uint32_t conn_err)
{
	struct ddb_entry * ddb_entry;

	/* check for out of range index */
	if (fw_ddb_index >= MAX_DDB_ENTRIES)
		return QLA_ERROR;
=======
=======
>>>>>>> refs/remotes/origin/master
int qla4xxx_process_ddb_changed(struct scsi_qla_host *ha,
				uint32_t fw_ddb_index,
				uint32_t state, uint32_t conn_err)
{
	struct ddb_entry *ddb_entry;
	int status = QLA_ERROR;

	/* check for out of range index */
	if (fw_ddb_index >= MAX_DDB_ENTRIES)
		goto exit_ddb_event;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	/* Get the corresponging ddb entry */
	ddb_entry = qla4xxx_lookup_ddb_by_fw_index(ha, fw_ddb_index);
	/* Device does not currently exist in our database. */
	if (ddb_entry == NULL) {
<<<<<<< HEAD
<<<<<<< HEAD
		if (state == DDB_DS_SESSION_ACTIVE)
			qla4xxx_add_device_dynamically(ha, fw_ddb_index);
		return QLA_SUCCESS;
	}

	/* Device already exists in our database. */
	DEBUG2(printk("scsi%ld: %s DDB - old state= 0x%x, new state=0x%x for "
		      "index [%d]\n", ha->host_no, __func__,
		      ddb_entry->fw_ddb_device_state, state, fw_ddb_index));

	ddb_entry->fw_ddb_device_state = state;
	/* Device is back online. */
	if ((ddb_entry->fw_ddb_device_state == DDB_DS_SESSION_ACTIVE) &&
	   (atomic_read(&ddb_entry->state) != DDB_STATE_ONLINE)) {
		atomic_set(&ddb_entry->state, DDB_STATE_ONLINE);
		atomic_set(&ddb_entry->relogin_retry_count, 0);
		atomic_set(&ddb_entry->relogin_timer, 0);
		clear_bit(DF_RELOGIN, &ddb_entry->flags);
		iscsi_unblock_session(ddb_entry->sess);
		iscsi_session_event(ddb_entry->sess,
				    ISCSI_KEVENT_CREATE_SESSION);
		/*
		 * Change the lun state to READY in case the lun TIMEOUT before
		 * the device came back.
		 */
	} else if (ddb_entry->fw_ddb_device_state != DDB_DS_SESSION_ACTIVE) {
		/* Device went away, mark device missing */
		if (atomic_read(&ddb_entry->state) == DDB_STATE_ONLINE) {
			DEBUG2(ql4_printk(KERN_INFO, ha, "%s mark missing "
					"ddb_entry 0x%p sess 0x%p conn 0x%p\n",
					__func__, ddb_entry,
					ddb_entry->sess, ddb_entry->conn));
			qla4xxx_mark_device_missing(ha, ddb_entry);
		}

		/*
		 * Relogin if device state changed to a not active state.
		 * However, do not relogin if a RELOGIN is in process, or
		 * we are not allowed to relogin to this DDB.
		 */
		if (ddb_entry->fw_ddb_device_state == DDB_DS_SESSION_FAILED &&
		    !test_bit(DF_RELOGIN, &ddb_entry->flags) &&
		    qla4_is_relogin_allowed(ha, conn_err)) {
			/*
			 * This triggers a relogin.  After the relogin_timer
			 * expires, the relogin gets scheduled.	 We must wait a
			 * minimum amount of time since receiving an 0x8014 AEN
			 * with failed device_state or a logout response before
			 * we can issue another relogin.
			 */
			/* Firmware pads this timeout: (time2wait +1).
			 * Driver retry to login should be longer than F/W.
			 * Otherwise F/W will fail
			 * set_ddb() mbx cmd with 0x4005 since it still
			 * counting down its time2wait.
			 */
			atomic_set(&ddb_entry->relogin_timer, 0);
			atomic_set(&ddb_entry->retry_relogin_timer,
				   ddb_entry->default_time2wait + 4);
			DEBUG(printk("scsi%ld: %s: ddb[%d] "
			    "initiate relogin after %d seconds\n",
			    ha->host_no, __func__,
			    ddb_entry->fw_ddb_index,
			    ddb_entry->default_time2wait + 4));
		} else {
			DEBUG(printk("scsi%ld: %s: ddb[%d] "
			    "relogin not initiated, state = %d, "
			    "ddb_entry->flags = 0x%lx\n",
			    ha->host_no, __func__,
			    ddb_entry->fw_ddb_index,
			    ddb_entry->fw_ddb_device_state,
			    ddb_entry->flags));
		}
	}
	return QLA_SUCCESS;
}
=======
=======
>>>>>>> refs/remotes/origin/master
		ql4_printk(KERN_ERR, ha, "%s: No ddb_entry at FW index [%d]\n",
			   __func__, fw_ddb_index);

		if (state == DDB_DS_NO_CONNECTION_ACTIVE)
			clear_bit(fw_ddb_index, ha->ddb_idx_map);

		goto exit_ddb_event;
	}

	ddb_entry->ddb_change(ha, fw_ddb_index, ddb_entry, state);

exit_ddb_event:
	return status;
}

/**
 * qla4xxx_login_flash_ddb - Login to target (DDB)
 * @cls_session: Pointer to the session to login
 *
 * This routine logins to the target.
 * Issues setddb and conn open mbx
 **/
void qla4xxx_login_flash_ddb(struct iscsi_cls_session *cls_session)
{
	struct iscsi_session *sess;
	struct ddb_entry *ddb_entry;
	struct scsi_qla_host *ha;
	struct dev_db_entry *fw_ddb_entry = NULL;
	dma_addr_t fw_ddb_dma;
	uint32_t mbx_sts = 0;
	int ret;

	sess = cls_session->dd_data;
	ddb_entry = sess->dd_data;
	ha =  ddb_entry->ha;

	if (!test_bit(AF_LINK_UP, &ha->flags))
		return;

	if (ddb_entry->ddb_type != FLASH_DDB) {
		DEBUG2(ql4_printk(KERN_INFO, ha,
				  "Skipping login to non FLASH DB"));
		goto exit_login;
	}

	fw_ddb_entry = dma_pool_alloc(ha->fw_ddb_dma_pool, GFP_KERNEL,
				      &fw_ddb_dma);
	if (fw_ddb_entry == NULL) {
		DEBUG2(ql4_printk(KERN_ERR, ha, "Out of memory\n"));
		goto exit_login;
	}

	if (ddb_entry->fw_ddb_index == INVALID_ENTRY) {
		ret = qla4xxx_get_ddb_index(ha, &ddb_entry->fw_ddb_index);
		if (ret == QLA_ERROR)
			goto exit_login;

		ha->fw_ddb_index_map[ddb_entry->fw_ddb_index] = ddb_entry;
		ha->tot_ddbs++;
	}

	memcpy(fw_ddb_entry, &ddb_entry->fw_ddb_entry,
	       sizeof(struct dev_db_entry));
	ddb_entry->sess->target_id = ddb_entry->fw_ddb_index;

	ret = qla4xxx_set_ddb_entry(ha, ddb_entry->fw_ddb_index,
				    fw_ddb_dma, &mbx_sts);
	if (ret == QLA_ERROR) {
		DEBUG2(ql4_printk(KERN_ERR, ha, "Set DDB failed\n"));
		goto exit_login;
	}

	ddb_entry->fw_ddb_device_state = DDB_DS_LOGIN_IN_PROCESS;
	ret = qla4xxx_conn_open(ha, ddb_entry->fw_ddb_index);
	if (ret == QLA_ERROR) {
		ql4_printk(KERN_ERR, ha, "%s: Login failed: %s\n", __func__,
			   sess->targetname);
		goto exit_login;
	}

exit_login:
	if (fw_ddb_entry)
		dma_pool_free(ha->fw_ddb_dma_pool, fw_ddb_entry, fw_ddb_dma);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
