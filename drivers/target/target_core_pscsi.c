/*******************************************************************************
 * Filename:  target_core_pscsi.c
 *
 * This file contains the generic target mode <-> Linux SCSI subsystem plugin.
 *
<<<<<<< HEAD
 * Copyright (c) 2003, 2004, 2005 PyX Technologies, Inc.
 * Copyright (c) 2005, 2006, 2007 SBE, Inc.
 * Copyright (c) 2007-2010 Rising Tide Systems
 * Copyright (c) 2008-2010 Linux-iSCSI.org
=======
 * (c) Copyright 2003-2013 Datera, Inc.
>>>>>>> refs/remotes/origin/master
 *
 * Nicholas A. Bellinger <nab@kernel.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 ******************************************************************************/

<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/version.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#include <linux/string.h>
#include <linux/parser.h>
#include <linux/timer.h>
#include <linux/blkdev.h>
#include <linux/blk_types.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/genhd.h>
#include <linux/cdrom.h>
<<<<<<< HEAD
#include <linux/file.h>
<<<<<<< HEAD
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/ratelimit.h>
#include <linux/module.h>
#include <asm/unaligned.h>

>>>>>>> refs/remotes/origin/master
#include <scsi/scsi.h>
#include <scsi/scsi_device.h>
#include <scsi/scsi_cmnd.h>
#include <scsi/scsi_host.h>
#include <scsi/scsi_tcq.h>

#include <target/target_core_base.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <target/target_core_device.h>
#include <target/target_core_transport.h>
=======
#include <target/target_core_backend.h>
>>>>>>> refs/remotes/origin/cm-10.0

=======
#include <target/target_core_backend.h>

#include "target_core_alua.h"
>>>>>>> refs/remotes/origin/master
#include "target_core_pscsi.h"

#define ISPRINT(a)  ((a >= ' ') && (a <= '~'))

<<<<<<< HEAD
static struct se_subsystem_api pscsi_template;

static void pscsi_req_done(struct request *, int);

<<<<<<< HEAD
/*	pscsi_get_sh():
 *
 *
 */
static struct Scsi_Host *pscsi_get_sh(u32 host_no)
{
	struct Scsi_Host *sh = NULL;

	sh = scsi_host_lookup(host_no);
	if (IS_ERR(sh)) {
		printk(KERN_ERR "Unable to locate SCSI HBA with Host ID:"
				" %u\n", host_no);
		return NULL;
	}

	return sh;
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
static inline struct pscsi_dev_virt *PSCSI_DEV(struct se_device *dev)
{
	return container_of(dev, struct pscsi_dev_virt, dev);
}

static struct se_subsystem_api pscsi_template;

static sense_reason_t pscsi_execute_cmd(struct se_cmd *cmd);
static void pscsi_req_done(struct request *, int);

>>>>>>> refs/remotes/origin/master
/*	pscsi_attach_hba():
 *
 * 	pscsi_get_sh() used scsi_host_lookup() to locate struct Scsi_Host.
 *	from the passed SCSI Host ID.
 */
static int pscsi_attach_hba(struct se_hba *hba, u32 host_id)
{
<<<<<<< HEAD
<<<<<<< HEAD
	int hba_depth;
	struct pscsi_hba_virt *phv;

	phv = kzalloc(sizeof(struct pscsi_hba_virt), GFP_KERNEL);
	if (!(phv)) {
		printk(KERN_ERR "Unable to allocate struct pscsi_hba_virt\n");
		return -1;
	}
	phv->phv_host_id = host_id;
	phv->phv_mode = PHV_VIRUTAL_HOST_ID;
	hba_depth = PSCSI_VIRTUAL_HBA_DEPTH;
	atomic_set(&hba->left_queue_depth, hba_depth);
	atomic_set(&hba->max_queue_depth, hba_depth);

	hba->hba_ptr = (void *)phv;

	printk(KERN_INFO "CORE_HBA[%d] - TCM SCSI HBA Driver %s on"
		" Generic Target Core Stack %s\n", hba->hba_id,
		PSCSI_VERSION, TARGET_CORE_MOD_VERSION);
	printk(KERN_INFO "CORE_HBA[%d] - Attached SCSI HBA to Generic"
		" Target Core with TCQ Depth: %d\n", hba->hba_id,
		atomic_read(&hba->max_queue_depth));
=======
=======
>>>>>>> refs/remotes/origin/master
	struct pscsi_hba_virt *phv;

	phv = kzalloc(sizeof(struct pscsi_hba_virt), GFP_KERNEL);
	if (!phv) {
		pr_err("Unable to allocate struct pscsi_hba_virt\n");
		return -ENOMEM;
	}
	phv->phv_host_id = host_id;
	phv->phv_mode = PHV_VIRTUAL_HOST_ID;

	hba->hba_ptr = phv;

	pr_debug("CORE_HBA[%d] - TCM SCSI HBA Driver %s on"
		" Generic Target Core Stack %s\n", hba->hba_id,
		PSCSI_VERSION, TARGET_CORE_MOD_VERSION);
	pr_debug("CORE_HBA[%d] - Attached SCSI HBA to Generic\n",
	       hba->hba_id);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return 0;
}

static void pscsi_detach_hba(struct se_hba *hba)
{
	struct pscsi_hba_virt *phv = hba->hba_ptr;
	struct Scsi_Host *scsi_host = phv->phv_lld_host;

	if (scsi_host) {
		scsi_host_put(scsi_host);

<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_INFO "CORE_HBA[%d] - Detached SCSI HBA: %s from"
=======
		pr_debug("CORE_HBA[%d] - Detached SCSI HBA: %s from"
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_debug("CORE_HBA[%d] - Detached SCSI HBA: %s from"
>>>>>>> refs/remotes/origin/master
			" Generic Target Core\n", hba->hba_id,
			(scsi_host->hostt->name) ? (scsi_host->hostt->name) :
			"Unknown");
	} else
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_INFO "CORE_HBA[%d] - Detached Virtual SCSI HBA"
=======
		pr_debug("CORE_HBA[%d] - Detached Virtual SCSI HBA"
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_debug("CORE_HBA[%d] - Detached Virtual SCSI HBA"
>>>>>>> refs/remotes/origin/master
			" from Generic Target Core\n", hba->hba_id);

	kfree(phv);
	hba->hba_ptr = NULL;
}

static int pscsi_pmode_enable_hba(struct se_hba *hba, unsigned long mode_flag)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct pscsi_hba_virt *phv = (struct pscsi_hba_virt *)hba->hba_ptr;
	struct Scsi_Host *sh = phv->phv_lld_host;
	int hba_depth = PSCSI_VIRTUAL_HBA_DEPTH;
	/*
	 * Release the struct Scsi_Host
	 */
	if (!(mode_flag)) {
		if (!(sh))
			return 0;

		phv->phv_lld_host = NULL;
		phv->phv_mode = PHV_VIRUTAL_HOST_ID;
		atomic_set(&hba->left_queue_depth, hba_depth);
		atomic_set(&hba->max_queue_depth, hba_depth);

		printk(KERN_INFO "CORE_HBA[%d] - Disabled pSCSI HBA Passthrough"
=======
=======
>>>>>>> refs/remotes/origin/master
	struct pscsi_hba_virt *phv = hba->hba_ptr;
	struct Scsi_Host *sh = phv->phv_lld_host;
	/*
	 * Release the struct Scsi_Host
	 */
	if (!mode_flag) {
		if (!sh)
			return 0;

		phv->phv_lld_host = NULL;
		phv->phv_mode = PHV_VIRTUAL_HOST_ID;

		pr_debug("CORE_HBA[%d] - Disabled pSCSI HBA Passthrough"
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			" %s\n", hba->hba_id, (sh->hostt->name) ?
			(sh->hostt->name) : "Unknown");

		scsi_host_put(sh);
		return 0;
	}
	/*
	 * Otherwise, locate struct Scsi_Host from the original passed
	 * pSCSI Host ID and enable for phba mode
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	sh = pscsi_get_sh(phv->phv_host_id);
	if (!(sh)) {
		printk(KERN_ERR "pSCSI: Unable to locate SCSI Host for"
			" phv_host_id: %d\n", phv->phv_host_id);
		return -1;
	}
	/*
	 * Usually the SCSI LLD will use the hostt->can_queue value to define
	 * its HBA TCQ depth.  Some other drivers (like 2.6 megaraid) don't set
	 * this at all and set sh->can_queue at runtime.
	 */
	hba_depth = (sh->hostt->can_queue > sh->can_queue) ?
		sh->hostt->can_queue : sh->can_queue;

	atomic_set(&hba->left_queue_depth, hba_depth);
	atomic_set(&hba->max_queue_depth, hba_depth);
=======
	sh = scsi_host_lookup(phv->phv_host_id);
	if (IS_ERR(sh)) {
		pr_err("pSCSI: Unable to locate SCSI Host for"
			" phv_host_id: %d\n", phv->phv_host_id);
		return PTR_ERR(sh);
	}
>>>>>>> refs/remotes/origin/cm-10.0
=======
	sh = scsi_host_lookup(phv->phv_host_id);
	if (!sh) {
		pr_err("pSCSI: Unable to locate SCSI Host for"
			" phv_host_id: %d\n", phv->phv_host_id);
		return -EINVAL;
	}
>>>>>>> refs/remotes/origin/master

	phv->phv_lld_host = sh;
	phv->phv_mode = PHV_LLD_SCSI_HOST_NO;

<<<<<<< HEAD
<<<<<<< HEAD
	printk(KERN_INFO "CORE_HBA[%d] - Enabled pSCSI HBA Passthrough %s\n",
=======
	pr_debug("CORE_HBA[%d] - Enabled pSCSI HBA Passthrough %s\n",
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pr_debug("CORE_HBA[%d] - Enabled pSCSI HBA Passthrough %s\n",
>>>>>>> refs/remotes/origin/master
		hba->hba_id, (sh->hostt->name) ? (sh->hostt->name) : "Unknown");

	return 1;
}

static void pscsi_tape_read_blocksize(struct se_device *dev,
		struct scsi_device *sdev)
{
	unsigned char cdb[MAX_COMMAND_SIZE], *buf;
	int ret;

	buf = kzalloc(12, GFP_KERNEL);
	if (!buf)
		return;

	memset(cdb, 0, MAX_COMMAND_SIZE);
	cdb[0] = MODE_SENSE;
	cdb[4] = 0x0c; /* 12 bytes */

	ret = scsi_execute_req(sdev, cdb, DMA_FROM_DEVICE, buf, 12, NULL,
			HZ, 1, NULL);
	if (ret)
		goto out_free;

	/*
	 * If MODE_SENSE still returns zero, set the default value to 1024.
	 */
	sdev->sector_size = (buf[9] << 16) | (buf[10] << 8) | (buf[11]);
	if (!sdev->sector_size)
		sdev->sector_size = 1024;
out_free:
	kfree(buf);
}

static void
pscsi_set_inquiry_info(struct scsi_device *sdev, struct t10_wwn *wwn)
{
	unsigned char *buf;

	if (sdev->inquiry_len < INQUIRY_LEN)
		return;

	buf = sdev->inquiry;
	if (!buf)
		return;
	/*
	 * Use sdev->inquiry from drivers/scsi/scsi_scan.c:scsi_alloc_sdev()
	 */
	memcpy(&wwn->vendor[0], &buf[8], sizeof(wwn->vendor));
	memcpy(&wwn->model[0], &buf[16], sizeof(wwn->model));
	memcpy(&wwn->revision[0], &buf[32], sizeof(wwn->revision));
}

static int
pscsi_get_inquiry_vpd_serial(struct scsi_device *sdev, struct t10_wwn *wwn)
{
	unsigned char cdb[MAX_COMMAND_SIZE], *buf;
	int ret;

	buf = kzalloc(INQUIRY_VPD_SERIAL_LEN, GFP_KERNEL);
	if (!buf)
<<<<<<< HEAD
<<<<<<< HEAD
		return -1;
=======
		return -ENOMEM;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		return -ENOMEM;
>>>>>>> refs/remotes/origin/master

	memset(cdb, 0, MAX_COMMAND_SIZE);
	cdb[0] = INQUIRY;
	cdb[1] = 0x01; /* Query VPD */
	cdb[2] = 0x80; /* Unit Serial Number */
	cdb[3] = (INQUIRY_VPD_SERIAL_LEN >> 8) & 0xff;
	cdb[4] = (INQUIRY_VPD_SERIAL_LEN & 0xff);

	ret = scsi_execute_req(sdev, cdb, DMA_FROM_DEVICE, buf,
			      INQUIRY_VPD_SERIAL_LEN, NULL, HZ, 1, NULL);
	if (ret)
		goto out_free;

	snprintf(&wwn->unit_serial[0], INQUIRY_VPD_SERIAL_LEN, "%s", &buf[4]);

<<<<<<< HEAD
	wwn->t10_sub_dev->su_dev_flags |= SDF_FIRMWARE_VPD_UNIT_SERIAL;
=======
	wwn->t10_dev->dev_flags |= DF_FIRMWARE_VPD_UNIT_SERIAL;
>>>>>>> refs/remotes/origin/master

	kfree(buf);
	return 0;

out_free:
	kfree(buf);
<<<<<<< HEAD
<<<<<<< HEAD
	return -1;
=======
	return -EPERM;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return -EPERM;
>>>>>>> refs/remotes/origin/master
}

static void
pscsi_get_inquiry_vpd_device_ident(struct scsi_device *sdev,
		struct t10_wwn *wwn)
{
	unsigned char cdb[MAX_COMMAND_SIZE], *buf, *page_83;
	int ident_len, page_len, off = 4, ret;
	struct t10_vpd *vpd;

	buf = kzalloc(INQUIRY_VPD_SERIAL_LEN, GFP_KERNEL);
	if (!buf)
		return;

	memset(cdb, 0, MAX_COMMAND_SIZE);
	cdb[0] = INQUIRY;
	cdb[1] = 0x01; /* Query VPD */
	cdb[2] = 0x83; /* Device Identifier */
	cdb[3] = (INQUIRY_VPD_DEVICE_IDENTIFIER_LEN >> 8) & 0xff;
	cdb[4] = (INQUIRY_VPD_DEVICE_IDENTIFIER_LEN & 0xff);

	ret = scsi_execute_req(sdev, cdb, DMA_FROM_DEVICE, buf,
			      INQUIRY_VPD_DEVICE_IDENTIFIER_LEN,
			      NULL, HZ, 1, NULL);
	if (ret)
		goto out;

	page_len = (buf[2] << 8) | buf[3];
	while (page_len > 0) {
		/* Grab a pointer to the Identification descriptor */
		page_83 = &buf[off];
		ident_len = page_83[3];
		if (!ident_len) {
<<<<<<< HEAD
<<<<<<< HEAD
			printk(KERN_ERR "page_83[3]: identifier"
					" length zero!\n");
			break;
		}
		printk(KERN_INFO "T10 VPD Identifer Length: %d\n", ident_len);

		vpd = kzalloc(sizeof(struct t10_vpd), GFP_KERNEL);
		if (!vpd) {
			printk(KERN_ERR "Unable to allocate memory for"
=======
=======
>>>>>>> refs/remotes/origin/master
			pr_err("page_83[3]: identifier"
					" length zero!\n");
			break;
		}
<<<<<<< HEAD
		pr_debug("T10 VPD Identifer Length: %d\n", ident_len);
=======
		pr_debug("T10 VPD Identifier Length: %d\n", ident_len);
>>>>>>> refs/remotes/origin/master

		vpd = kzalloc(sizeof(struct t10_vpd), GFP_KERNEL);
		if (!vpd) {
			pr_err("Unable to allocate memory for"
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
					" struct t10_vpd\n");
			goto out;
		}
		INIT_LIST_HEAD(&vpd->vpd_list);

		transport_set_vpd_proto_id(vpd, page_83);
		transport_set_vpd_assoc(vpd, page_83);

		if (transport_set_vpd_ident_type(vpd, page_83) < 0) {
			off += (ident_len + 4);
			page_len -= (ident_len + 4);
			kfree(vpd);
			continue;
		}
		if (transport_set_vpd_ident(vpd, page_83) < 0) {
			off += (ident_len + 4);
			page_len -= (ident_len + 4);
			kfree(vpd);
			continue;
		}

		list_add_tail(&vpd->vpd_list, &wwn->t10_vpd_list);
		off += (ident_len + 4);
		page_len -= (ident_len + 4);
	}

out:
	kfree(buf);
}

<<<<<<< HEAD
/*	pscsi_add_device_to_list():
 *
 *
 */
static struct se_device *pscsi_add_device_to_list(
	struct se_hba *hba,
	struct se_subsystem_dev *se_dev,
	struct pscsi_dev_virt *pdv,
	struct scsi_device *sd,
	int dev_flags)
{
	struct se_device *dev;
	struct se_dev_limits dev_limits;
	struct request_queue *q;
	struct queue_limits *limits;

	memset(&dev_limits, 0, sizeof(struct se_dev_limits));
=======
static int pscsi_add_device_to_list(struct se_device *dev,
		struct scsi_device *sd)
{
	struct pscsi_dev_virt *pdv = PSCSI_DEV(dev);
	struct request_queue *q = sd->request_queue;

	pdv->pdv_sd = sd;
>>>>>>> refs/remotes/origin/master

	if (!sd->queue_depth) {
		sd->queue_depth = PSCSI_DEFAULT_QUEUEDEPTH;

<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "Set broken SCSI Device %d:%d:%d"
=======
		pr_err("Set broken SCSI Device %d:%d:%d"
>>>>>>> refs/remotes/origin/cm-10.0
			" queue_depth to %d\n", sd->channel, sd->id,
				sd->lun, sd->queue_depth);
	}
	/*
	 * Setup the local scope queue_limits from struct request_queue->limits
	 * to pass into transport_add_device_to_core_hba() as struct se_dev_limits.
	 */
	q = sd->request_queue;
	limits = &dev_limits.limits;
	limits->logical_block_size = sd->sector_size;
<<<<<<< HEAD
	limits->max_hw_sectors = (sd->host->max_sectors > queue_max_hw_sectors(q)) ?
				  queue_max_hw_sectors(q) : sd->host->max_sectors;
	limits->max_sectors = (sd->host->max_sectors > queue_max_sectors(q)) ?
				  queue_max_sectors(q) : sd->host->max_sectors;
=======
	limits->max_hw_sectors = min_t(int, sd->host->max_sectors, queue_max_hw_sectors(q));
	limits->max_sectors = min_t(int, sd->host->max_sectors, queue_max_sectors(q));
>>>>>>> refs/remotes/origin/cm-10.0
	dev_limits.hw_queue_depth = sd->queue_depth;
	dev_limits.queue_depth = sd->queue_depth;
	/*
	 * Setup our standard INQUIRY info into se_dev->t10_wwn
	 */
	pscsi_set_inquiry_info(sd, &se_dev->t10_wwn);

	/*
	 * Set the pointer pdv->pdv_sd to from passed struct scsi_device,
	 * which has already been referenced with Linux SCSI code with
	 * scsi_device_get() in this file's pscsi_create_virtdevice().
	 *
	 * The passthrough operations called by the transport_add_device_*
	 * function below will require this pointer to be set for passthroug
	 *  ops.
	 *
	 * For the shutdown case in pscsi_free_device(), this struct
	 * scsi_device  reference is released with Linux SCSI code
	 * scsi_device_put() and the pdv->pdv_sd cleared.
	 */
	pdv->pdv_sd = sd;
<<<<<<< HEAD

	dev = transport_add_device_to_core_hba(hba, &pscsi_template,
				se_dev, dev_flags, (void *)pdv,
				&dev_limits, NULL, NULL);
	if (!(dev)) {
=======
	dev = transport_add_device_to_core_hba(hba, &pscsi_template,
				se_dev, dev_flags, pdv,
				&dev_limits, NULL, NULL);
	if (!dev) {
>>>>>>> refs/remotes/origin/cm-10.0
		pdv->pdv_sd = NULL;
		return NULL;
	}
=======
		pr_err("Set broken SCSI Device %d:%d:%d"
			" queue_depth to %d\n", sd->channel, sd->id,
				sd->lun, sd->queue_depth);
	}

	dev->dev_attrib.hw_block_size = sd->sector_size;
	dev->dev_attrib.hw_max_sectors =
		min_t(int, sd->host->max_sectors, queue_max_hw_sectors(q));
	dev->dev_attrib.hw_queue_depth = sd->queue_depth;

	/*
	 * Setup our standard INQUIRY info into se_dev->t10_wwn
	 */
	pscsi_set_inquiry_info(sd, &dev->t10_wwn);
>>>>>>> refs/remotes/origin/master

	/*
	 * Locate VPD WWN Information used for various purposes within
	 * the Storage Engine.
	 */
<<<<<<< HEAD
	if (!pscsi_get_inquiry_vpd_serial(sd, &se_dev->t10_wwn)) {
=======
	if (!pscsi_get_inquiry_vpd_serial(sd, &dev->t10_wwn)) {
>>>>>>> refs/remotes/origin/master
		/*
		 * If VPD Unit Serial returned GOOD status, try
		 * VPD Device Identification page (0x83).
		 */
<<<<<<< HEAD
		pscsi_get_inquiry_vpd_device_ident(sd, &se_dev->t10_wwn);
=======
		pscsi_get_inquiry_vpd_device_ident(sd, &dev->t10_wwn);
>>>>>>> refs/remotes/origin/master
	}

	/*
	 * For TYPE_TAPE, attempt to determine blocksize with MODE_SENSE.
	 */
	if (sd->type == TYPE_TAPE)
		pscsi_tape_read_blocksize(dev, sd);
<<<<<<< HEAD
	return dev;
}

static void *pscsi_allocate_virtdevice(struct se_hba *hba, const char *name)
=======
	return 0;
}

static struct se_device *pscsi_alloc_device(struct se_hba *hba,
		const char *name)
>>>>>>> refs/remotes/origin/master
{
	struct pscsi_dev_virt *pdv;

	pdv = kzalloc(sizeof(struct pscsi_dev_virt), GFP_KERNEL);
<<<<<<< HEAD
<<<<<<< HEAD
	if (!(pdv)) {
		printk(KERN_ERR "Unable to allocate memory for struct pscsi_dev_virt\n");
=======
	if (!pdv) {
		pr_err("Unable to allocate memory for struct pscsi_dev_virt\n");
>>>>>>> refs/remotes/origin/cm-10.0
		return NULL;
	}
	pdv->pdv_se_hba = hba;

<<<<<<< HEAD
	printk(KERN_INFO "PSCSI: Allocated pdv: %p for %s\n", pdv, name);
	return (void *)pdv;
=======
	pr_debug("PSCSI: Allocated pdv: %p for %s\n", pdv, name);
	return pdv;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!pdv) {
		pr_err("Unable to allocate memory for struct pscsi_dev_virt\n");
		return NULL;
	}

	pr_debug("PSCSI: Allocated pdv: %p for %s\n", pdv, name);
	return &pdv->dev;
>>>>>>> refs/remotes/origin/master
}

/*
 * Called with struct Scsi_Host->host_lock called.
 */
<<<<<<< HEAD
static struct se_device *pscsi_create_type_disk(
	struct scsi_device *sd,
	struct pscsi_dev_virt *pdv,
	struct se_subsystem_dev *se_dev,
	struct se_hba *hba)
	__releases(sh->host_lock)
{
	struct se_device *dev;
<<<<<<< HEAD
	struct pscsi_hba_virt *phv = (struct pscsi_hba_virt *)pdv->pdv_se_hba->hba_ptr;
=======
	struct pscsi_hba_virt *phv = pdv->pdv_se_hba->hba_ptr;
>>>>>>> refs/remotes/origin/cm-10.0
	struct Scsi_Host *sh = sd->host;
	struct block_device *bd;
	u32 dev_flags = 0;

	if (scsi_device_get(sd)) {
<<<<<<< HEAD
		printk(KERN_ERR "scsi_device_get() failed for %d:%d:%d:%d\n",
=======
		pr_err("scsi_device_get() failed for %d:%d:%d:%d\n",
>>>>>>> refs/remotes/origin/cm-10.0
			sh->host_no, sd->channel, sd->id, sd->lun);
		spin_unlock_irq(sh->host_lock);
		return NULL;
=======
static int pscsi_create_type_disk(struct se_device *dev, struct scsi_device *sd)
	__releases(sh->host_lock)
{
	struct pscsi_hba_virt *phv = dev->se_hba->hba_ptr;
	struct pscsi_dev_virt *pdv = PSCSI_DEV(dev);
	struct Scsi_Host *sh = sd->host;
	struct block_device *bd;
	int ret;

	if (scsi_device_get(sd)) {
		pr_err("scsi_device_get() failed for %d:%d:%d:%d\n",
			sh->host_no, sd->channel, sd->id, sd->lun);
		spin_unlock_irq(sh->host_lock);
		return -EIO;
>>>>>>> refs/remotes/origin/master
	}
	spin_unlock_irq(sh->host_lock);
	/*
	 * Claim exclusive struct block_device access to struct scsi_device
	 * for TYPE_DISK using supplied udev_path
	 */
<<<<<<< HEAD
	bd = blkdev_get_by_path(se_dev->se_dev_udev_path,
				FMODE_WRITE|FMODE_READ|FMODE_EXCL, pdv);
	if (IS_ERR(bd)) {
<<<<<<< HEAD
		printk(KERN_ERR "pSCSI: blkdev_get_by_path() failed\n");
=======
		pr_err("pSCSI: blkdev_get_by_path() failed\n");
>>>>>>> refs/remotes/origin/cm-10.0
		scsi_device_put(sd);
		return NULL;
	}
	pdv->pdv_bd = bd;

	dev = pscsi_add_device_to_list(hba, se_dev, pdv, sd, dev_flags);
<<<<<<< HEAD
	if (!(dev)) {
=======
	if (!dev) {
>>>>>>> refs/remotes/origin/cm-10.0
		blkdev_put(pdv->pdv_bd, FMODE_WRITE|FMODE_READ|FMODE_EXCL);
		scsi_device_put(sd);
		return NULL;
	}
<<<<<<< HEAD
	printk(KERN_INFO "CORE_PSCSI[%d] - Added TYPE_DISK for %d:%d:%d:%d\n",
=======
	pr_debug("CORE_PSCSI[%d] - Added TYPE_DISK for %d:%d:%d:%d\n",
>>>>>>> refs/remotes/origin/cm-10.0
		phv->phv_host_id, sh->host_no, sd->channel, sd->id, sd->lun);

	return dev;
=======
	bd = blkdev_get_by_path(dev->udev_path,
				FMODE_WRITE|FMODE_READ|FMODE_EXCL, pdv);
	if (IS_ERR(bd)) {
		pr_err("pSCSI: blkdev_get_by_path() failed\n");
		scsi_device_put(sd);
		return PTR_ERR(bd);
	}
	pdv->pdv_bd = bd;

	ret = pscsi_add_device_to_list(dev, sd);
	if (ret) {
		blkdev_put(pdv->pdv_bd, FMODE_WRITE|FMODE_READ|FMODE_EXCL);
		scsi_device_put(sd);
		return ret;
	}

	pr_debug("CORE_PSCSI[%d] - Added TYPE_DISK for %d:%d:%d:%d\n",
		phv->phv_host_id, sh->host_no, sd->channel, sd->id, sd->lun);
	return 0;
>>>>>>> refs/remotes/origin/master
}

/*
 * Called with struct Scsi_Host->host_lock called.
 */
<<<<<<< HEAD
static struct se_device *pscsi_create_type_rom(
	struct scsi_device *sd,
	struct pscsi_dev_virt *pdv,
	struct se_subsystem_dev *se_dev,
	struct se_hba *hba)
	__releases(sh->host_lock)
{
	struct se_device *dev;
<<<<<<< HEAD
	struct pscsi_hba_virt *phv = (struct pscsi_hba_virt *)pdv->pdv_se_hba->hba_ptr;
=======
	struct pscsi_hba_virt *phv = pdv->pdv_se_hba->hba_ptr;
>>>>>>> refs/remotes/origin/cm-10.0
	struct Scsi_Host *sh = sd->host;
	u32 dev_flags = 0;

	if (scsi_device_get(sd)) {
<<<<<<< HEAD
		printk(KERN_ERR "scsi_device_get() failed for %d:%d:%d:%d\n",
=======
		pr_err("scsi_device_get() failed for %d:%d:%d:%d\n",
>>>>>>> refs/remotes/origin/cm-10.0
			sh->host_no, sd->channel, sd->id, sd->lun);
		spin_unlock_irq(sh->host_lock);
		return NULL;
	}
	spin_unlock_irq(sh->host_lock);

	dev = pscsi_add_device_to_list(hba, se_dev, pdv, sd, dev_flags);
<<<<<<< HEAD
	if (!(dev)) {
		scsi_device_put(sd);
		return NULL;
	}
	printk(KERN_INFO "CORE_PSCSI[%d] - Added Type: %s for %d:%d:%d:%d\n",
=======
	if (!dev) {
		scsi_device_put(sd);
		return NULL;
	}
	pr_debug("CORE_PSCSI[%d] - Added Type: %s for %d:%d:%d:%d\n",
>>>>>>> refs/remotes/origin/cm-10.0
		phv->phv_host_id, scsi_device_type(sd->type), sh->host_no,
		sd->channel, sd->id, sd->lun);

	return dev;
}

/*
 *Called with struct Scsi_Host->host_lock called.
 */
static struct se_device *pscsi_create_type_other(
	struct scsi_device *sd,
	struct pscsi_dev_virt *pdv,
	struct se_subsystem_dev *se_dev,
	struct se_hba *hba)
	__releases(sh->host_lock)
{
	struct se_device *dev;
<<<<<<< HEAD
	struct pscsi_hba_virt *phv = (struct pscsi_hba_virt *)pdv->pdv_se_hba->hba_ptr;
=======
	struct pscsi_hba_virt *phv = pdv->pdv_se_hba->hba_ptr;
>>>>>>> refs/remotes/origin/cm-10.0
	struct Scsi_Host *sh = sd->host;
	u32 dev_flags = 0;

	spin_unlock_irq(sh->host_lock);
	dev = pscsi_add_device_to_list(hba, se_dev, pdv, sd, dev_flags);
<<<<<<< HEAD
	if (!(dev))
		return NULL;

	printk(KERN_INFO "CORE_PSCSI[%d] - Added Type: %s for %d:%d:%d:%d\n",
=======
	if (!dev)
		return NULL;

	pr_debug("CORE_PSCSI[%d] - Added Type: %s for %d:%d:%d:%d\n",
>>>>>>> refs/remotes/origin/cm-10.0
		phv->phv_host_id, scsi_device_type(sd->type), sh->host_no,
		sd->channel, sd->id, sd->lun);

	return dev;
}

static struct se_device *pscsi_create_virtdevice(
	struct se_hba *hba,
	struct se_subsystem_dev *se_dev,
	void *p)
{
<<<<<<< HEAD
	struct pscsi_dev_virt *pdv = (struct pscsi_dev_virt *)p;
	struct se_device *dev;
	struct scsi_device *sd;
	struct pscsi_hba_virt *phv = (struct pscsi_hba_virt *)hba->hba_ptr;
	struct Scsi_Host *sh = phv->phv_lld_host;
	int legacy_mode_enable = 0;

	if (!(pdv)) {
		printk(KERN_ERR "Unable to locate struct pscsi_dev_virt"
=======
	struct pscsi_dev_virt *pdv = p;
	struct se_device *dev;
	struct scsi_device *sd;
	struct pscsi_hba_virt *phv = hba->hba_ptr;
	struct Scsi_Host *sh = phv->phv_lld_host;
	int legacy_mode_enable = 0;

	if (!pdv) {
		pr_err("Unable to locate struct pscsi_dev_virt"
>>>>>>> refs/remotes/origin/cm-10.0
				" parameter\n");
		return ERR_PTR(-EINVAL);
	}
=======
static int pscsi_create_type_rom(struct se_device *dev, struct scsi_device *sd)
	__releases(sh->host_lock)
{
	struct pscsi_hba_virt *phv = dev->se_hba->hba_ptr;
	struct Scsi_Host *sh = sd->host;
	int ret;

	if (scsi_device_get(sd)) {
		pr_err("scsi_device_get() failed for %d:%d:%d:%d\n",
			sh->host_no, sd->channel, sd->id, sd->lun);
		spin_unlock_irq(sh->host_lock);
		return -EIO;
	}
	spin_unlock_irq(sh->host_lock);

	ret = pscsi_add_device_to_list(dev, sd);
	if (ret) {
		scsi_device_put(sd);
		return ret;
	}
	pr_debug("CORE_PSCSI[%d] - Added Type: %s for %d:%d:%d:%d\n",
		phv->phv_host_id, scsi_device_type(sd->type), sh->host_no,
		sd->channel, sd->id, sd->lun);

	return 0;
}

/*
 * Called with struct Scsi_Host->host_lock called.
 */
static int pscsi_create_type_other(struct se_device *dev,
		struct scsi_device *sd)
	__releases(sh->host_lock)
{
	struct pscsi_hba_virt *phv = dev->se_hba->hba_ptr;
	struct Scsi_Host *sh = sd->host;
	int ret;

	spin_unlock_irq(sh->host_lock);
	ret = pscsi_add_device_to_list(dev, sd);
	if (ret)
		return ret;

	pr_debug("CORE_PSCSI[%d] - Added Type: %s for %d:%d:%d:%d\n",
		phv->phv_host_id, scsi_device_type(sd->type), sh->host_no,
		sd->channel, sd->id, sd->lun);
	return 0;
}

static int pscsi_configure_device(struct se_device *dev)
{
	struct se_hba *hba = dev->se_hba;
	struct pscsi_dev_virt *pdv = PSCSI_DEV(dev);
	struct scsi_device *sd;
	struct pscsi_hba_virt *phv = dev->se_hba->hba_ptr;
	struct Scsi_Host *sh = phv->phv_lld_host;
	int legacy_mode_enable = 0;
	int ret;

	if (!(pdv->pdv_flags & PDF_HAS_CHANNEL_ID) ||
	    !(pdv->pdv_flags & PDF_HAS_TARGET_ID) ||
	    !(pdv->pdv_flags & PDF_HAS_LUN_ID)) {
		pr_err("Missing scsi_channel_id=, scsi_target_id= and"
			" scsi_lun_id= parameters\n");
		return -EINVAL;
	}

>>>>>>> refs/remotes/origin/master
	/*
	 * If not running in PHV_LLD_SCSI_HOST_NO mode, locate the
	 * struct Scsi_Host we will need to bring the TCM/pSCSI object online
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (!(sh)) {
		if (phv->phv_mode == PHV_LLD_SCSI_HOST_NO) {
			printk(KERN_ERR "pSCSI: Unable to locate struct"
=======
	if (!sh) {
		if (phv->phv_mode == PHV_LLD_SCSI_HOST_NO) {
			pr_err("pSCSI: Unable to locate struct"
>>>>>>> refs/remotes/origin/cm-10.0
				" Scsi_Host for PHV_LLD_SCSI_HOST_NO\n");
			return ERR_PTR(-ENODEV);
		}
		/*
<<<<<<< HEAD
		 * For the newer PHV_VIRUTAL_HOST_ID struct scsi_device
		 * reference, we enforce that udev_path has been set
		 */
		if (!(se_dev->su_dev_flags & SDF_USING_UDEV_PATH)) {
			printk(KERN_ERR "pSCSI: udev_path attribute has not"
=======
		 * For the newer PHV_VIRTUAL_HOST_ID struct scsi_device
		 * reference, we enforce that udev_path has been set
		 */
		if (!(se_dev->su_dev_flags & SDF_USING_UDEV_PATH)) {
			pr_err("pSCSI: udev_path attribute has not"
>>>>>>> refs/remotes/origin/cm-10.0
				" been set before ENABLE=1\n");
			return ERR_PTR(-EINVAL);
		}
		/*
<<<<<<< HEAD
		 * If no scsi_host_id= was passed for PHV_VIRUTAL_HOST_ID,
=======
		 * If no scsi_host_id= was passed for PHV_VIRTUAL_HOST_ID,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!sh) {
		if (phv->phv_mode == PHV_LLD_SCSI_HOST_NO) {
			pr_err("pSCSI: Unable to locate struct"
				" Scsi_Host for PHV_LLD_SCSI_HOST_NO\n");
			return -ENODEV;
		}
		/*
		 * For the newer PHV_VIRTUAL_HOST_ID struct scsi_device
		 * reference, we enforce that udev_path has been set
		 */
		if (!(dev->dev_flags & DF_USING_UDEV_PATH)) {
			pr_err("pSCSI: udev_path attribute has not"
				" been set before ENABLE=1\n");
			return -EINVAL;
		}
		/*
		 * If no scsi_host_id= was passed for PHV_VIRTUAL_HOST_ID,
>>>>>>> refs/remotes/origin/master
		 * use the original TCM hba ID to reference Linux/SCSI Host No
		 * and enable for PHV_LLD_SCSI_HOST_NO mode.
		 */
		if (!(pdv->pdv_flags & PDF_HAS_VIRT_HOST_ID)) {
<<<<<<< HEAD
			spin_lock(&hba->device_lock);
<<<<<<< HEAD
			if (!(list_empty(&hba->hba_dev_list))) {
				printk(KERN_ERR "pSCSI: Unable to set hba_mode"
=======
			if (!list_empty(&hba->hba_dev_list)) {
				pr_err("pSCSI: Unable to set hba_mode"
>>>>>>> refs/remotes/origin/cm-10.0
					" with active devices\n");
				spin_unlock(&hba->device_lock);
				return ERR_PTR(-EEXIST);
			}
			spin_unlock(&hba->device_lock);

			if (pscsi_pmode_enable_hba(hba, 1) != 1)
				return ERR_PTR(-ENODEV);
=======
			if (hba->dev_count) {
				pr_err("pSCSI: Unable to set hba_mode"
					" with active devices\n");
				return -EEXIST;
			}

			if (pscsi_pmode_enable_hba(hba, 1) != 1)
				return -ENODEV;
>>>>>>> refs/remotes/origin/master

			legacy_mode_enable = 1;
			hba->hba_flags |= HBA_FLAGS_PSCSI_MODE;
			sh = phv->phv_lld_host;
		} else {
<<<<<<< HEAD
<<<<<<< HEAD
			sh = pscsi_get_sh(pdv->pdv_host_id);
			if (!(sh)) {
				printk(KERN_ERR "pSCSI: Unable to locate"
					" pdv_host_id: %d\n", pdv->pdv_host_id);
				return ERR_PTR(-ENODEV);
			}
		}
	} else {
		if (phv->phv_mode == PHV_VIRUTAL_HOST_ID) {
			printk(KERN_ERR "pSCSI: PHV_VIRUTAL_HOST_ID set while"
=======
			sh = scsi_host_lookup(pdv->pdv_host_id);
			if (IS_ERR(sh)) {
				pr_err("pSCSI: Unable to locate"
					" pdv_host_id: %d\n", pdv->pdv_host_id);
				return ERR_CAST(sh);
=======
			sh = scsi_host_lookup(pdv->pdv_host_id);
			if (!sh) {
				pr_err("pSCSI: Unable to locate"
					" pdv_host_id: %d\n", pdv->pdv_host_id);
				return -EINVAL;
>>>>>>> refs/remotes/origin/master
			}
		}
	} else {
		if (phv->phv_mode == PHV_VIRTUAL_HOST_ID) {
			pr_err("pSCSI: PHV_VIRTUAL_HOST_ID set while"
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
				" struct Scsi_Host exists\n");
			return ERR_PTR(-EEXIST);
=======
				" struct Scsi_Host exists\n");
			return -EEXIST;
>>>>>>> refs/remotes/origin/master
		}
	}

	spin_lock_irq(sh->host_lock);
	list_for_each_entry(sd, &sh->__devices, siblings) {
		if ((pdv->pdv_channel_id != sd->channel) ||
		    (pdv->pdv_target_id != sd->id) ||
		    (pdv->pdv_lun_id != sd->lun))
			continue;
		/*
		 * Functions will release the held struct scsi_host->host_lock
		 * before calling calling pscsi_add_device_to_list() to register
		 * struct scsi_device with target_core_mod.
		 */
		switch (sd->type) {
		case TYPE_DISK:
<<<<<<< HEAD
			dev = pscsi_create_type_disk(sd, pdv, se_dev, hba);
			break;
		case TYPE_ROM:
			dev = pscsi_create_type_rom(sd, pdv, se_dev, hba);
			break;
		default:
			dev = pscsi_create_type_other(sd, pdv, se_dev, hba);
			break;
		}

<<<<<<< HEAD
		if (!(dev)) {
			if (phv->phv_mode == PHV_VIRUTAL_HOST_ID)
=======
		if (!dev) {
			if (phv->phv_mode == PHV_VIRTUAL_HOST_ID)
>>>>>>> refs/remotes/origin/cm-10.0
=======
			ret = pscsi_create_type_disk(dev, sd);
			break;
		case TYPE_ROM:
			ret = pscsi_create_type_rom(dev, sd);
			break;
		default:
			ret = pscsi_create_type_other(dev, sd);
			break;
		}

		if (ret) {
			if (phv->phv_mode == PHV_VIRTUAL_HOST_ID)
>>>>>>> refs/remotes/origin/master
				scsi_host_put(sh);
			else if (legacy_mode_enable) {
				pscsi_pmode_enable_hba(hba, 0);
				hba->hba_flags &= ~HBA_FLAGS_PSCSI_MODE;
			}
			pdv->pdv_sd = NULL;
<<<<<<< HEAD
			return ERR_PTR(-ENODEV);
		}
		return dev;
	}
	spin_unlock_irq(sh->host_lock);

<<<<<<< HEAD
	printk(KERN_ERR "pSCSI: Unable to locate %d:%d:%d:%d\n", sh->host_no,
		pdv->pdv_channel_id,  pdv->pdv_target_id, pdv->pdv_lun_id);

	if (phv->phv_mode == PHV_VIRUTAL_HOST_ID)
=======
=======
			return ret;
		}
		return 0;
	}
	spin_unlock_irq(sh->host_lock);

>>>>>>> refs/remotes/origin/master
	pr_err("pSCSI: Unable to locate %d:%d:%d:%d\n", sh->host_no,
		pdv->pdv_channel_id,  pdv->pdv_target_id, pdv->pdv_lun_id);

	if (phv->phv_mode == PHV_VIRTUAL_HOST_ID)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		scsi_host_put(sh);
	else if (legacy_mode_enable) {
		pscsi_pmode_enable_hba(hba, 0);
		hba->hba_flags &= ~HBA_FLAGS_PSCSI_MODE;
	}

<<<<<<< HEAD
	return ERR_PTR(-ENODEV);
}

/*	pscsi_free_device(): (Part of se_subsystem_api_t template)
 *
 *
 */
static void pscsi_free_device(void *p)
{
	struct pscsi_dev_virt *pdv = p;
	struct pscsi_hba_virt *phv = pdv->pdv_se_hba->hba_ptr;
=======
	return -ENODEV;
}

static void pscsi_free_device(struct se_device *dev)
{
	struct pscsi_dev_virt *pdv = PSCSI_DEV(dev);
	struct pscsi_hba_virt *phv = dev->se_hba->hba_ptr;
>>>>>>> refs/remotes/origin/master
	struct scsi_device *sd = pdv->pdv_sd;

	if (sd) {
		/*
		 * Release exclusive pSCSI internal struct block_device claim for
		 * struct scsi_device with TYPE_DISK from pscsi_create_type_disk()
		 */
		if ((sd->type == TYPE_DISK) && pdv->pdv_bd) {
			blkdev_put(pdv->pdv_bd,
				   FMODE_WRITE|FMODE_READ|FMODE_EXCL);
			pdv->pdv_bd = NULL;
		}
		/*
		 * For HBA mode PHV_LLD_SCSI_HOST_NO, release the reference
		 * to struct Scsi_Host now.
		 */
		if ((phv->phv_mode == PHV_LLD_SCSI_HOST_NO) &&
		    (phv->phv_lld_host != NULL))
			scsi_host_put(phv->phv_lld_host);

		if ((sd->type == TYPE_DISK) || (sd->type == TYPE_ROM))
			scsi_device_put(sd);

		pdv->pdv_sd = NULL;
	}

	kfree(pdv);
}

<<<<<<< HEAD
static inline struct pscsi_plugin_task *PSCSI_TASK(struct se_task *task)
{
	return container_of(task, struct pscsi_plugin_task, pscsi_task);
}


/*	pscsi_transport_complete():
 *
 *
 */
static int pscsi_transport_complete(struct se_task *task)
{
<<<<<<< HEAD
	struct pscsi_dev_virt *pdv = task->se_dev->dev_ptr;
=======
	struct pscsi_dev_virt *pdv = task->task_se_cmd->se_dev->dev_ptr;
>>>>>>> refs/remotes/origin/cm-10.0
	struct scsi_device *sd = pdv->pdv_sd;
	int result;
	struct pscsi_plugin_task *pt = PSCSI_TASK(task);
	unsigned char *cdb = &pt->pscsi_cdb[0];

=======
static void pscsi_transport_complete(struct se_cmd *cmd, struct scatterlist *sg,
				     unsigned char *sense_buffer)
{
	struct pscsi_dev_virt *pdv = PSCSI_DEV(cmd->se_dev);
	struct scsi_device *sd = pdv->pdv_sd;
	int result;
	struct pscsi_plugin_task *pt = cmd->priv;
	unsigned char *cdb;
	/*
	 * Special case for REPORT_LUNs handling where pscsi_plugin_task has
	 * not been allocated because TCM is handling the emulation directly.
	 */
	if (!pt)
		return;

	cdb = &pt->pscsi_cdb[0];
>>>>>>> refs/remotes/origin/master
	result = pt->pscsi_result;
	/*
	 * Hack to make sure that Write-Protect modepage is set if R/O mode is
	 * forced.
	 */
<<<<<<< HEAD
	if (((cdb[0] == MODE_SENSE) || (cdb[0] == MODE_SENSE_10)) &&
	     (status_byte(result) << 1) == SAM_STAT_GOOD) {
<<<<<<< HEAD
		if (!TASK_CMD(task)->se_deve)
			goto after_mode_sense;

		if (TASK_CMD(task)->se_deve->lun_flags &
				TRANSPORT_LUNFLAGS_READ_ONLY) {
			unsigned char *buf = (unsigned char *)
				T_TASK(task->task_se_cmd)->t_task_buf;
=======
		if (!task->task_se_cmd->se_deve)
			goto after_mode_sense;

		if (task->task_se_cmd->se_deve->lun_flags &
				TRANSPORT_LUNFLAGS_READ_ONLY) {
			unsigned char *buf = transport_kmap_data_sg(task->task_se_cmd);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!cmd->se_deve || !cmd->data_length)
		goto after_mode_sense;

	if (((cdb[0] == MODE_SENSE) || (cdb[0] == MODE_SENSE_10)) &&
	     (status_byte(result) << 1) == SAM_STAT_GOOD) {
		if (cmd->se_deve->lun_flags & TRANSPORT_LUNFLAGS_READ_ONLY) {
			unsigned char *buf;

			buf = transport_kmap_data_sg(cmd);
			if (!buf)
				; /* XXX: TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE */
>>>>>>> refs/remotes/origin/master

			if (cdb[0] == MODE_SENSE_10) {
				if (!(buf[3] & 0x80))
					buf[3] |= 0x80;
			} else {
				if (!(buf[2] & 0x80))
					buf[2] |= 0x80;
			}
<<<<<<< HEAD
<<<<<<< HEAD
=======

			transport_kunmap_data_sg(task->task_se_cmd);
>>>>>>> refs/remotes/origin/cm-10.0
=======

			transport_kunmap_data_sg(cmd);
>>>>>>> refs/remotes/origin/master
		}
	}
after_mode_sense:

<<<<<<< HEAD
	if (sd->type != TYPE_TAPE)
=======
	if (sd->type != TYPE_TAPE || !cmd->data_length)
>>>>>>> refs/remotes/origin/master
		goto after_mode_select;

	/*
	 * Hack to correctly obtain the initiator requested blocksize for
	 * TYPE_TAPE.  Since this value is dependent upon each tape media,
	 * struct scsi_device->sector_size will not contain the correct value
	 * by default, so we go ahead and set it so
	 * TRANSPORT(dev)->get_blockdev() returns the correct value to the
	 * storage engine.
	 */
	if (((cdb[0] == MODE_SELECT) || (cdb[0] == MODE_SELECT_10)) &&
	      (status_byte(result) << 1) == SAM_STAT_GOOD) {
		unsigned char *buf;
<<<<<<< HEAD
		struct scatterlist *sg = task->task_sg;
=======
>>>>>>> refs/remotes/origin/master
		u16 bdl;
		u32 blocksize;

		buf = sg_virt(&sg[0]);
<<<<<<< HEAD
<<<<<<< HEAD
		if (!(buf)) {
			printk(KERN_ERR "Unable to get buf for scatterlist\n");
=======
		if (!buf) {
			pr_err("Unable to get buf for scatterlist\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (!buf) {
			pr_err("Unable to get buf for scatterlist\n");
>>>>>>> refs/remotes/origin/master
			goto after_mode_select;
		}

		if (cdb[0] == MODE_SELECT)
			bdl = (buf[3]);
		else
			bdl = (buf[6] << 8) | (buf[7]);

		if (!bdl)
			goto after_mode_select;

		if (cdb[0] == MODE_SELECT)
			blocksize = (buf[9] << 16) | (buf[10] << 8) |
					(buf[11]);
		else
			blocksize = (buf[13] << 16) | (buf[14] << 8) |
					(buf[15]);

		sd->sector_size = blocksize;
	}
after_mode_select:

<<<<<<< HEAD
	if (status_byte(result) & CHECK_CONDITION)
		return 1;

	return 0;
}

static struct se_task *
<<<<<<< HEAD
pscsi_alloc_task(struct se_cmd *cmd)
{
	struct pscsi_plugin_task *pt;
	unsigned char *cdb = T_TASK(cmd)->t_task_cdb;

	pt = kzalloc(sizeof(struct pscsi_plugin_task), GFP_KERNEL);
	if (!pt) {
		printk(KERN_ERR "Unable to allocate struct pscsi_plugin_task\n");
		return NULL;
	}

	/*
	 * If TCM Core is signaling a > TCM_MAX_COMMAND_SIZE allocation,
	 * allocate the extended CDB buffer for per struct se_task context
	 * pt->pscsi_cdb now.
	 */
	if (T_TASK(cmd)->t_task_cdb != T_TASK(cmd)->__t_task_cdb) {

		pt->pscsi_cdb = kzalloc(scsi_command_size(cdb), GFP_KERNEL);
		if (!(pt->pscsi_cdb)) {
			printk(KERN_ERR "pSCSI: Unable to allocate extended"
					" pt->pscsi_cdb\n");
			kfree(pt);
			return NULL;
		}
	} else
		pt->pscsi_cdb = &pt->__pscsi_cdb[0];

	return &pt->pscsi_task;
}

static inline void pscsi_blk_init_request(
	struct se_task *task,
	struct pscsi_plugin_task *pt,
	struct request *req,
	int bidi_read)
{
	/*
	 * Defined as "scsi command" in include/linux/blkdev.h.
	 */
	req->cmd_type = REQ_TYPE_BLOCK_PC;
	/*
	 * For the extra BIDI-COMMAND READ struct request we do not
	 * need to setup the remaining structure members
	 */
	if (bidi_read)
		return;
	/*
	 * Setup the done function pointer for struct request,
	 * also set the end_io_data pointer.to struct se_task.
	 */
	req->end_io = pscsi_req_done;
	req->end_io_data = (void *)task;
	/*
	 * Load the referenced struct se_task's SCSI CDB into
	 * include/linux/blkdev.h:struct request->cmd
	 */
	req->cmd_len = scsi_command_size(pt->pscsi_cdb);
	req->cmd = &pt->pscsi_cdb[0];
	/*
	 * Setup pointer for outgoing sense data.
	 */
	req->sense = (void *)&pt->pscsi_sense[0];
	req->sense_len = 0;
}

/*
 * Used for pSCSI data payloads for all *NON* SCF_SCSI_DATA_SG_IO_CDB
*/
static int pscsi_blk_get_request(struct se_task *task)
{
	struct pscsi_plugin_task *pt = PSCSI_TASK(task);
	struct pscsi_dev_virt *pdv = task->se_dev->dev_ptr;

	pt->pscsi_req = blk_get_request(pdv->pdv_sd->request_queue,
			(task->task_data_direction == DMA_TO_DEVICE),
			GFP_KERNEL);
	if (!(pt->pscsi_req) || IS_ERR(pt->pscsi_req)) {
		printk(KERN_ERR "PSCSI: blk_get_request() failed: %ld\n",
				IS_ERR(pt->pscsi_req));
		return PYX_TRANSPORT_LU_COMM_FAILURE;
	}
	/*
	 * Setup the newly allocated struct request for REQ_TYPE_BLOCK_PC,
	 * and setup rq callback, CDB and sense.
	 */
	pscsi_blk_init_request(task, pt, pt->pscsi_req, 0);
	return 0;
}

/*      pscsi_do_task(): (Part of se_subsystem_api_t template)
 *
 *
 */
static int pscsi_do_task(struct se_task *task)
{
	struct pscsi_plugin_task *pt = PSCSI_TASK(task);
	struct pscsi_dev_virt *pdv = task->se_dev->dev_ptr;
	/*
	 * Set the struct request->timeout value based on peripheral
	 * device type from SCSI.
	 */
	if (pdv->pdv_sd->type == TYPE_DISK)
		pt->pscsi_req->timeout = PS_TIMEOUT_DISK;
	else
		pt->pscsi_req->timeout = PS_TIMEOUT_OTHER;

	pt->pscsi_req->retries = PS_RETRY;
	/*
	 * Queue the struct request into the struct scsi_device->request_queue.
	 * Also check for HEAD_OF_QUEUE SAM TASK attr from received se_cmd
	 * descriptor
	 */
	blk_execute_rq_nowait(pdv->pdv_sd->request_queue, NULL, pt->pscsi_req,
			(task->task_se_cmd->sam_task_attr == MSG_HEAD_TAG),
			pscsi_req_done);

	return PYX_TRANSPORT_SENT_TO_TRANSPORT;
}

static void pscsi_free_task(struct se_task *task)
{
	struct pscsi_plugin_task *pt = PSCSI_TASK(task);
	struct se_cmd *cmd = task->task_se_cmd;

	/*
	 * Release the extended CDB allocation from pscsi_alloc_task()
	 * if one exists.
	 */
	if (T_TASK(cmd)->t_task_cdb != T_TASK(cmd)->__t_task_cdb)
		kfree(pt->pscsi_cdb);
	/*
=======
pscsi_alloc_task(unsigned char *cdb)
{
	struct pscsi_plugin_task *pt;

	/*
	 * Dynamically alloc cdb space, since it may be larger than
	 * TCM_MAX_COMMAND_SIZE
	 */
	pt = kzalloc(sizeof(*pt) + scsi_command_size(cdb), GFP_KERNEL);
	if (!pt) {
		pr_err("Unable to allocate struct pscsi_plugin_task\n");
		return NULL;
	}

	return &pt->pscsi_task;
}

static void pscsi_free_task(struct se_task *task)
{
	struct pscsi_plugin_task *pt = PSCSI_TASK(task);

	/*
>>>>>>> refs/remotes/origin/cm-10.0
	 * We do not release the bio(s) here associated with this task, as
	 * this is handled by bio_put() and pscsi_bi_endio().
	 */
	kfree(pt);
=======
	if (sense_buffer && (status_byte(result) & CHECK_CONDITION)) {
		memcpy(sense_buffer, pt->pscsi_sense, TRANSPORT_SENSE_BUFFER);
		cmd->se_cmd_flags |= SCF_TRANSPORT_TASK_SENSE;
	}
>>>>>>> refs/remotes/origin/master
}

enum {
	Opt_scsi_host_id, Opt_scsi_channel_id, Opt_scsi_target_id,
	Opt_scsi_lun_id, Opt_err
};

static match_table_t tokens = {
	{Opt_scsi_host_id, "scsi_host_id=%d"},
	{Opt_scsi_channel_id, "scsi_channel_id=%d"},
	{Opt_scsi_target_id, "scsi_target_id=%d"},
	{Opt_scsi_lun_id, "scsi_lun_id=%d"},
	{Opt_err, NULL}
};

<<<<<<< HEAD
static ssize_t pscsi_set_configfs_dev_params(struct se_hba *hba,
	struct se_subsystem_dev *se_dev,
	const char *page,
	ssize_t count)
{
	struct pscsi_dev_virt *pdv = se_dev->se_dev_su_ptr;
	struct pscsi_hba_virt *phv = hba->hba_ptr;
=======
static ssize_t pscsi_set_configfs_dev_params(struct se_device *dev,
		const char *page, ssize_t count)
{
	struct pscsi_dev_virt *pdv = PSCSI_DEV(dev);
	struct pscsi_hba_virt *phv = dev->se_hba->hba_ptr;
>>>>>>> refs/remotes/origin/master
	char *orig, *ptr, *opts;
	substring_t args[MAX_OPT_ARGS];
	int ret = 0, arg, token;

	opts = kstrdup(page, GFP_KERNEL);
	if (!opts)
		return -ENOMEM;

	orig = opts;

<<<<<<< HEAD
<<<<<<< HEAD
	while ((ptr = strsep(&opts, ",")) != NULL) {
=======
	while ((ptr = strsep(&opts, ",\n")) != NULL) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	while ((ptr = strsep(&opts, ",\n")) != NULL) {
>>>>>>> refs/remotes/origin/master
		if (!*ptr)
			continue;

		token = match_token(ptr, tokens, args);
		switch (token) {
		case Opt_scsi_host_id:
			if (phv->phv_mode == PHV_LLD_SCSI_HOST_NO) {
<<<<<<< HEAD
<<<<<<< HEAD
				printk(KERN_ERR "PSCSI[%d]: Unable to accept"
=======
				pr_err("PSCSI[%d]: Unable to accept"
>>>>>>> refs/remotes/origin/cm-10.0
=======
				pr_err("PSCSI[%d]: Unable to accept"
>>>>>>> refs/remotes/origin/master
					" scsi_host_id while phv_mode =="
					" PHV_LLD_SCSI_HOST_NO\n",
					phv->phv_host_id);
				ret = -EINVAL;
				goto out;
			}
			match_int(args, &arg);
			pdv->pdv_host_id = arg;
<<<<<<< HEAD
<<<<<<< HEAD
			printk(KERN_INFO "PSCSI[%d]: Referencing SCSI Host ID:"
=======
			pr_debug("PSCSI[%d]: Referencing SCSI Host ID:"
>>>>>>> refs/remotes/origin/cm-10.0
=======
			pr_debug("PSCSI[%d]: Referencing SCSI Host ID:"
>>>>>>> refs/remotes/origin/master
				" %d\n", phv->phv_host_id, pdv->pdv_host_id);
			pdv->pdv_flags |= PDF_HAS_VIRT_HOST_ID;
			break;
		case Opt_scsi_channel_id:
			match_int(args, &arg);
			pdv->pdv_channel_id = arg;
<<<<<<< HEAD
<<<<<<< HEAD
			printk(KERN_INFO "PSCSI[%d]: Referencing SCSI Channel"
=======
			pr_debug("PSCSI[%d]: Referencing SCSI Channel"
>>>>>>> refs/remotes/origin/cm-10.0
=======
			pr_debug("PSCSI[%d]: Referencing SCSI Channel"
>>>>>>> refs/remotes/origin/master
				" ID: %d\n",  phv->phv_host_id,
				pdv->pdv_channel_id);
			pdv->pdv_flags |= PDF_HAS_CHANNEL_ID;
			break;
		case Opt_scsi_target_id:
			match_int(args, &arg);
			pdv->pdv_target_id = arg;
<<<<<<< HEAD
<<<<<<< HEAD
			printk(KERN_INFO "PSCSI[%d]: Referencing SCSI Target"
=======
			pr_debug("PSCSI[%d]: Referencing SCSI Target"
>>>>>>> refs/remotes/origin/cm-10.0
=======
			pr_debug("PSCSI[%d]: Referencing SCSI Target"
>>>>>>> refs/remotes/origin/master
				" ID: %d\n", phv->phv_host_id,
				pdv->pdv_target_id);
			pdv->pdv_flags |= PDF_HAS_TARGET_ID;
			break;
		case Opt_scsi_lun_id:
			match_int(args, &arg);
			pdv->pdv_lun_id = arg;
<<<<<<< HEAD
<<<<<<< HEAD
			printk(KERN_INFO "PSCSI[%d]: Referencing SCSI LUN ID:"
=======
			pr_debug("PSCSI[%d]: Referencing SCSI LUN ID:"
>>>>>>> refs/remotes/origin/cm-10.0
=======
			pr_debug("PSCSI[%d]: Referencing SCSI LUN ID:"
>>>>>>> refs/remotes/origin/master
				" %d\n", phv->phv_host_id, pdv->pdv_lun_id);
			pdv->pdv_flags |= PDF_HAS_LUN_ID;
			break;
		default:
			break;
		}
	}

out:
	kfree(orig);
	return (!ret) ? count : ret;
}

<<<<<<< HEAD
static ssize_t pscsi_check_configfs_dev_params(
	struct se_hba *hba,
	struct se_subsystem_dev *se_dev)
{
	struct pscsi_dev_virt *pdv = se_dev->se_dev_su_ptr;

	if (!(pdv->pdv_flags & PDF_HAS_CHANNEL_ID) ||
	    !(pdv->pdv_flags & PDF_HAS_TARGET_ID) ||
	    !(pdv->pdv_flags & PDF_HAS_LUN_ID)) {
<<<<<<< HEAD
		printk(KERN_ERR "Missing scsi_channel_id=, scsi_target_id= and"
			" scsi_lun_id= parameters\n");
		return -1;
=======
		pr_err("Missing scsi_channel_id=, scsi_target_id= and"
			" scsi_lun_id= parameters\n");
		return -EINVAL;
>>>>>>> refs/remotes/origin/cm-10.0
	}

	return 0;
}

static ssize_t pscsi_show_configfs_dev_params(struct se_hba *hba,
					      struct se_subsystem_dev *se_dev,
					      char *b)
{
	struct pscsi_hba_virt *phv = hba->hba_ptr;
        struct pscsi_dev_virt *pdv = se_dev->se_dev_su_ptr;
=======
static ssize_t pscsi_show_configfs_dev_params(struct se_device *dev, char *b)
{
	struct pscsi_hba_virt *phv = dev->se_hba->hba_ptr;
	struct pscsi_dev_virt *pdv = PSCSI_DEV(dev);
>>>>>>> refs/remotes/origin/master
	struct scsi_device *sd = pdv->pdv_sd;
	unsigned char host_id[16];
	ssize_t bl;
	int i;

<<<<<<< HEAD
<<<<<<< HEAD
	if (phv->phv_mode == PHV_VIRUTAL_HOST_ID)
=======
	if (phv->phv_mode == PHV_VIRTUAL_HOST_ID)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (phv->phv_mode == PHV_VIRTUAL_HOST_ID)
>>>>>>> refs/remotes/origin/master
		snprintf(host_id, 16, "%d", pdv->pdv_host_id);
	else
		snprintf(host_id, 16, "PHBA Mode");

	bl = sprintf(b, "SCSI Device Bus Location:"
		" Channel ID: %d Target ID: %d LUN: %d Host ID: %s\n",
		pdv->pdv_channel_id, pdv->pdv_target_id, pdv->pdv_lun_id,
		host_id);

	if (sd) {
		bl += sprintf(b + bl, "        ");
		bl += sprintf(b + bl, "Vendor: ");
		for (i = 0; i < 8; i++) {
			if (ISPRINT(sd->vendor[i]))   /* printable character? */
				bl += sprintf(b + bl, "%c", sd->vendor[i]);
			else
				bl += sprintf(b + bl, " ");
		}
		bl += sprintf(b + bl, " Model: ");
		for (i = 0; i < 16; i++) {
			if (ISPRINT(sd->model[i]))   /* printable character ? */
				bl += sprintf(b + bl, "%c", sd->model[i]);
			else
				bl += sprintf(b + bl, " ");
		}
		bl += sprintf(b + bl, " Rev: ");
		for (i = 0; i < 4; i++) {
			if (ISPRINT(sd->rev[i]))   /* printable character ? */
				bl += sprintf(b + bl, "%c", sd->rev[i]);
			else
				bl += sprintf(b + bl, " ");
		}
		bl += sprintf(b + bl, "\n");
	}
	return bl;
}

static void pscsi_bi_endio(struct bio *bio, int error)
{
	bio_put(bio);
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline struct bio *pscsi_get_bio(struct pscsi_dev_virt *pdv, int sg_num)
=======
static inline struct bio *pscsi_get_bio(int sg_num)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static inline struct bio *pscsi_get_bio(int nr_vecs)
>>>>>>> refs/remotes/origin/master
{
	struct bio *bio;
	/*
	 * Use bio_malloc() following the comment in for bio -> struct request
	 * in block/blk-core.c:blk_make_request()
	 */
<<<<<<< HEAD
	bio = bio_kmalloc(GFP_KERNEL, sg_num);
<<<<<<< HEAD
	if (!(bio)) {
		printk(KERN_ERR "PSCSI: bio_kmalloc() failed\n");
=======
	if (!bio) {
		pr_err("PSCSI: bio_kmalloc() failed\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
	bio = bio_kmalloc(GFP_KERNEL, nr_vecs);
	if (!bio) {
		pr_err("PSCSI: bio_kmalloc() failed\n");
>>>>>>> refs/remotes/origin/master
		return NULL;
	}
	bio->bi_end_io = pscsi_bi_endio;

	return bio;
}

<<<<<<< HEAD
<<<<<<< HEAD
#if 0
#define DEBUG_PSCSI(x...) printk(x)
#else
#define DEBUG_PSCSI(x...)
#endif

static int __pscsi_map_task_SG(
	struct se_task *task,
	struct scatterlist *task_sg,
	u32 task_sg_num,
	int bidi_read)
{
	struct pscsi_plugin_task *pt = PSCSI_TASK(task);
	struct pscsi_dev_virt *pdv = task->se_dev->dev_ptr;
	struct bio *bio = NULL, *hbio = NULL, *tbio = NULL;
=======
static int pscsi_map_sg(struct se_task *task, struct scatterlist *task_sg,
		struct bio **hbio)
{
	struct se_cmd *cmd = task->task_se_cmd;
	struct pscsi_dev_virt *pdv = task->task_se_cmd->se_dev->dev_ptr;
	u32 task_sg_num = task->task_sg_nents;
	struct bio *bio = NULL, *tbio = NULL;
>>>>>>> refs/remotes/origin/cm-10.0
	struct page *page;
	struct scatterlist *sg;
	u32 data_len = task->task_size, i, len, bytes, off;
	int nr_pages = (task->task_size + task_sg[0].offset +
			PAGE_SIZE - 1) >> PAGE_SHIFT;
<<<<<<< HEAD
	int nr_vecs = 0, rc, ret = PYX_TRANSPORT_OUT_OF_MEMORY_RESOURCES;
	int rw = (task->task_data_direction == DMA_TO_DEVICE);

	if (!task->task_size)
		return 0;
	/*
	 * For SCF_SCSI_DATA_SG_IO_CDB, Use fs/bio.c:bio_add_page() to setup
	 * the bio_vec maplist from TC< struct se_mem -> task->task_sg ->
	 * struct scatterlist memory.  The struct se_task->task_sg[] currently needs
	 * to be attached to struct bios for submission to Linux/SCSI using
	 * struct request to struct scsi_device->request_queue.
	 *
	 * Note that this will be changing post v2.6.28 as Target_Core_Mod/pSCSI
	 * is ported to upstream SCSI passthrough functionality that accepts
	 * struct scatterlist->page_link or struct page as a paraemeter.
	 */
	DEBUG_PSCSI("PSCSI: nr_pages: %d\n", nr_pages);
=======
	int nr_vecs = 0, rc;
	int rw = (task->task_data_direction == DMA_TO_DEVICE);
=======
static sense_reason_t
pscsi_map_sg(struct se_cmd *cmd, struct scatterlist *sgl, u32 sgl_nents,
		enum dma_data_direction data_direction, struct bio **hbio)
{
	struct pscsi_dev_virt *pdv = PSCSI_DEV(cmd->se_dev);
	struct bio *bio = NULL, *tbio = NULL;
	struct page *page;
	struct scatterlist *sg;
	u32 data_len = cmd->data_length, i, len, bytes, off;
	int nr_pages = (cmd->data_length + sgl[0].offset +
			PAGE_SIZE - 1) >> PAGE_SHIFT;
	int nr_vecs = 0, rc;
	int rw = (data_direction == DMA_TO_DEVICE);
>>>>>>> refs/remotes/origin/master

	*hbio = NULL;

	pr_debug("PSCSI: nr_pages: %d\n", nr_pages);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

	for_each_sg(task_sg, sg, task_sg_num, i) {
=======

	for_each_sg(sgl, sg, sgl_nents, i) {
>>>>>>> refs/remotes/origin/master
		page = sg_page(sg);
		off = sg->offset;
		len = sg->length;

<<<<<<< HEAD
<<<<<<< HEAD
		DEBUG_PSCSI("PSCSI: i: %d page: %p len: %d off: %d\n", i,
=======
		pr_debug("PSCSI: i: %d page: %p len: %d off: %d\n", i,
>>>>>>> refs/remotes/origin/cm-10.0
			page, len, off);

		while (len > 0 && data_len > 0) {
			bytes = min_t(unsigned int, len, PAGE_SIZE - off);
			bytes = min(bytes, data_len);

<<<<<<< HEAD
			if (!(bio)) {
=======
			if (!bio) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_debug("PSCSI: i: %d page: %p len: %d off: %d\n", i,
			page, len, off);

		/*
		 * We only have one page of data in each sg element,
		 * we can not cross a page boundary.
		 */
		if (off + len > PAGE_SIZE)
			goto fail;

		if (len > 0 && data_len > 0) {
			bytes = min_t(unsigned int, len, PAGE_SIZE - off);
			bytes = min(bytes, data_len);

			if (!bio) {
>>>>>>> refs/remotes/origin/master
				nr_vecs = min_t(int, BIO_MAX_PAGES, nr_pages);
				nr_pages -= nr_vecs;
				/*
				 * Calls bio_kmalloc() and sets bio->bi_end_io()
				 */
<<<<<<< HEAD
<<<<<<< HEAD
				bio = pscsi_get_bio(pdv, nr_vecs);
				if (!(bio))
=======
				bio = pscsi_get_bio(nr_vecs);
				if (!bio)
>>>>>>> refs/remotes/origin/cm-10.0
=======
				bio = pscsi_get_bio(nr_vecs);
				if (!bio)
>>>>>>> refs/remotes/origin/master
					goto fail;

				if (rw)
					bio->bi_rw |= REQ_WRITE;

<<<<<<< HEAD
<<<<<<< HEAD
				DEBUG_PSCSI("PSCSI: Allocated bio: %p,"
=======
				pr_debug("PSCSI: Allocated bio: %p,"
>>>>>>> refs/remotes/origin/cm-10.0
=======
				pr_debug("PSCSI: Allocated bio: %p,"
>>>>>>> refs/remotes/origin/master
					" dir: %s nr_vecs: %d\n", bio,
					(rw) ? "rw" : "r", nr_vecs);
				/*
				 * Set *hbio pointer to handle the case:
				 * nr_pages > BIO_MAX_PAGES, where additional
				 * bios need to be added to complete a given
<<<<<<< HEAD
				 * struct se_task
				 */
<<<<<<< HEAD
				if (!hbio)
					hbio = tbio = bio;
=======
				if (!*hbio)
					*hbio = tbio = bio;
>>>>>>> refs/remotes/origin/cm-10.0
=======
				 * command.
				 */
				if (!*hbio)
					*hbio = tbio = bio;
>>>>>>> refs/remotes/origin/master
				else
					tbio = tbio->bi_next = bio;
			}

<<<<<<< HEAD
<<<<<<< HEAD
			DEBUG_PSCSI("PSCSI: Calling bio_add_pc_page() i: %d"
=======
			pr_debug("PSCSI: Calling bio_add_pc_page() i: %d"
>>>>>>> refs/remotes/origin/cm-10.0
=======
			pr_debug("PSCSI: Calling bio_add_pc_page() i: %d"
>>>>>>> refs/remotes/origin/master
				" bio: %p page: %p len: %d off: %d\n", i, bio,
				page, len, off);

			rc = bio_add_pc_page(pdv->pdv_sd->request_queue,
					bio, page, bytes, off);
			if (rc != bytes)
				goto fail;

<<<<<<< HEAD
<<<<<<< HEAD
			DEBUG_PSCSI("PSCSI: bio->bi_vcnt: %d nr_vecs: %d\n",
				bio->bi_vcnt, nr_vecs);

			if (bio->bi_vcnt > nr_vecs) {
				DEBUG_PSCSI("PSCSI: Reached bio->bi_vcnt max:"
=======
=======
>>>>>>> refs/remotes/origin/master
			pr_debug("PSCSI: bio->bi_vcnt: %d nr_vecs: %d\n",
				bio->bi_vcnt, nr_vecs);

			if (bio->bi_vcnt > nr_vecs) {
				pr_debug("PSCSI: Reached bio->bi_vcnt max:"
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
					" %d i: %d bio: %p, allocating another"
					" bio\n", bio->bi_vcnt, i, bio);
				/*
				 * Clear the pointer so that another bio will
				 * be allocated with pscsi_get_bio() above, the
				 * current bio has already been set *tbio and
				 * bio->bi_next.
				 */
				bio = NULL;
			}

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
			len -= bytes;
			data_len -= bytes;
			off = 0;
		}
	}
<<<<<<< HEAD
	/*
	 * Setup the primary pt->pscsi_req used for non BIDI and BIDI-COMMAND
	 * primary SCSI WRITE poayload mapped for struct se_task->task_sg[]
	 */
	if (!(bidi_read)) {
		/*
		 * Starting with v2.6.31, call blk_make_request() passing in *hbio to
		 * allocate the pSCSI task a struct request.
		 */
		pt->pscsi_req = blk_make_request(pdv->pdv_sd->request_queue,
					hbio, GFP_KERNEL);
		if (!(pt->pscsi_req)) {
			printk(KERN_ERR "pSCSI: blk_make_request() failed\n");
			goto fail;
		}
		/*
		 * Setup the newly allocated struct request for REQ_TYPE_BLOCK_PC,
		 * and setup rq callback, CDB and sense.
		 */
		pscsi_blk_init_request(task, pt, pt->pscsi_req, 0);

		return task->task_sg_num;
	}
	/*
	 * Setup the secondary pt->pscsi_req->next_rq used for the extra BIDI-COMMAND
	 * SCSI READ paylaod mapped for struct se_task->task_sg_bidi[]
	 */
	pt->pscsi_req->next_rq = blk_make_request(pdv->pdv_sd->request_queue,
					hbio, GFP_KERNEL);
	if (!(pt->pscsi_req->next_rq)) {
		printk(KERN_ERR "pSCSI: blk_make_request() failed for BIDI\n");
		goto fail;
	}
	pscsi_blk_init_request(task, pt, pt->pscsi_req->next_rq, 1);

	return task->task_sg_num;
fail:
	while (hbio) {
		bio = hbio;
		hbio = hbio->bi_next;
		bio->bi_next = NULL;
		bio_endio(bio, 0);
	}
	return ret;
}

static int pscsi_map_task_SG(struct se_task *task)
{
	int ret;

	/*
	 * Setup the main struct request for the task->task_sg[] payload
	 */

	ret = __pscsi_map_task_SG(task, task->task_sg, task->task_sg_num, 0);
	if (ret >= 0 && task->task_sg_bidi) {
		/*
		 * If present, set up the extra BIDI-COMMAND SCSI READ
		 * struct request and payload.
		 */
		ret = __pscsi_map_task_SG(task, task->task_sg_bidi,
					task->task_sg_num, 1);
	}

	if (ret < 0)
		return PYX_TRANSPORT_LU_COMM_FAILURE;
	return 0;
}

/*	pscsi_map_task_non_SG():
 *
 *
 */
static int pscsi_map_task_non_SG(struct se_task *task)
{
	struct se_cmd *cmd = TASK_CMD(task);
	struct pscsi_plugin_task *pt = PSCSI_TASK(task);
	struct pscsi_dev_virt *pdv = task->se_dev->dev_ptr;
	int ret = 0;

	if (pscsi_blk_get_request(task) < 0)
		return PYX_TRANSPORT_LU_COMM_FAILURE;

	if (!task->task_size)
		return 0;

	ret = blk_rq_map_kern(pdv->pdv_sd->request_queue,
			pt->pscsi_req, T_TASK(cmd)->t_task_buf,
			task->task_size, GFP_KERNEL);
	if (ret < 0) {
		printk(KERN_ERR "PSCSI: blk_rq_map_kern() failed: %d\n", ret);
		return PYX_TRANSPORT_LU_COMM_FAILURE;
	}
	return 0;
}

static int pscsi_CDB_none(struct se_task *task)
{
	return pscsi_blk_get_request(task);
}

/*	pscsi_get_cdb():
 *
 *
 */
static unsigned char *pscsi_get_cdb(struct se_task *task)
{
	struct pscsi_plugin_task *pt = PSCSI_TASK(task);

	return pt->pscsi_cdb;
=======

	return task->task_sg_nents;
fail:
	while (*hbio) {
		bio = *hbio;
		*hbio = (*hbio)->bi_next;
		bio->bi_next = NULL;
		bio_endio(bio, 0);	/* XXX: should be error */
	}
	cmd->scsi_sense_reason = TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
	return -ENOMEM;
}

static int pscsi_do_task(struct se_task *task)
{
	struct se_cmd *cmd = task->task_se_cmd;
	struct pscsi_dev_virt *pdv = task->task_se_cmd->se_dev->dev_ptr;
	struct pscsi_plugin_task *pt = PSCSI_TASK(task);
	struct request *req;
	struct bio *hbio;
	int ret;

	target_get_task_cdb(task, pt->pscsi_cdb);

	if (task->task_se_cmd->se_cmd_flags & SCF_SCSI_NON_DATA_CDB) {
		req = blk_get_request(pdv->pdv_sd->request_queue,
				(task->task_data_direction == DMA_TO_DEVICE),
				GFP_KERNEL);
		if (!req || IS_ERR(req)) {
			pr_err("PSCSI: blk_get_request() failed: %ld\n",
					req ? IS_ERR(req) : -ENOMEM);
			cmd->scsi_sense_reason =
				TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
			return -ENODEV;
		}
	} else {
		BUG_ON(!task->task_size);

		/*
		 * Setup the main struct request for the task->task_sg[] payload
		 */
		ret = pscsi_map_sg(task, task->task_sg, &hbio);
		if (ret < 0) {
			cmd->scsi_sense_reason =
				TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
			return ret;
		}
=======
			data_len -= bytes;
		}
	}

	return 0;
fail:
	while (*hbio) {
		bio = *hbio;
		*hbio = (*hbio)->bi_next;
		bio_endio(bio, 0);	/* XXX: should be error */
	}
	return TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
}

/*
 * Clear a lun set in the cdb if the initiator talking to use spoke
 * and old standards version, as we can't assume the underlying device
 * won't choke up on it.
 */
static inline void pscsi_clear_cdb_lun(unsigned char *cdb)
{
	switch (cdb[0]) {
	case READ_10: /* SBC - RDProtect */
	case READ_12: /* SBC - RDProtect */
	case READ_16: /* SBC - RDProtect */
	case SEND_DIAGNOSTIC: /* SPC - SELF-TEST Code */
	case VERIFY: /* SBC - VRProtect */
	case VERIFY_16: /* SBC - VRProtect */
	case WRITE_VERIFY: /* SBC - VRProtect */
	case WRITE_VERIFY_12: /* SBC - VRProtect */
	case MAINTENANCE_IN: /* SPC - Parameter Data Format for SA RTPG */
		break;
	default:
		cdb[1] &= 0x1f; /* clear logical unit number */
		break;
	}
}

static sense_reason_t
pscsi_parse_cdb(struct se_cmd *cmd)
{
	unsigned char *cdb = cmd->t_task_cdb;

	if (cmd->se_cmd_flags & SCF_BIDI)
		return TCM_UNSUPPORTED_SCSI_OPCODE;

	pscsi_clear_cdb_lun(cdb);

	/*
	 * For REPORT LUNS we always need to emulate the response, for everything
	 * else the default for pSCSI is to pass the command to the underlying
	 * LLD / physical hardware.
	 */
	switch (cdb[0]) {
	case REPORT_LUNS:
		cmd->execute_cmd = spc_emulate_report_luns;
		return 0;
	case READ_6:
	case READ_10:
	case READ_12:
	case READ_16:
	case WRITE_6:
	case WRITE_10:
	case WRITE_12:
	case WRITE_16:
	case WRITE_VERIFY:
		cmd->se_cmd_flags |= SCF_SCSI_DATA_CDB;
		/* FALLTHROUGH*/
	default:
		cmd->execute_cmd = pscsi_execute_cmd;
		return 0;
	}
}

static sense_reason_t
pscsi_execute_cmd(struct se_cmd *cmd)
{
	struct scatterlist *sgl = cmd->t_data_sg;
	u32 sgl_nents = cmd->t_data_nents;
	enum dma_data_direction data_direction = cmd->data_direction;
	struct pscsi_dev_virt *pdv = PSCSI_DEV(cmd->se_dev);
	struct pscsi_plugin_task *pt;
	struct request *req;
	struct bio *hbio;
	sense_reason_t ret;

	/*
	 * Dynamically alloc cdb space, since it may be larger than
	 * TCM_MAX_COMMAND_SIZE
	 */
	pt = kzalloc(sizeof(*pt) + scsi_command_size(cmd->t_task_cdb), GFP_KERNEL);
	if (!pt) {
		return TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
	}
	cmd->priv = pt;

	memcpy(pt->pscsi_cdb, cmd->t_task_cdb,
		scsi_command_size(cmd->t_task_cdb));

	if (!sgl) {
		req = blk_get_request(pdv->pdv_sd->request_queue,
				(data_direction == DMA_TO_DEVICE),
				GFP_KERNEL);
		if (!req) {
			pr_err("PSCSI: blk_get_request() failed\n");
			ret = TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
			goto fail;
		}
	} else {
		BUG_ON(!cmd->data_length);

		ret = pscsi_map_sg(cmd, sgl, sgl_nents, data_direction, &hbio);
		if (ret)
			goto fail;
>>>>>>> refs/remotes/origin/master

		req = blk_make_request(pdv->pdv_sd->request_queue, hbio,
				       GFP_KERNEL);
		if (IS_ERR(req)) {
			pr_err("pSCSI: blk_make_request() failed\n");
<<<<<<< HEAD
			goto fail;
=======
			ret = TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
			goto fail_free_bio;
>>>>>>> refs/remotes/origin/master
		}
	}

	req->cmd_type = REQ_TYPE_BLOCK_PC;
	req->end_io = pscsi_req_done;
<<<<<<< HEAD
	req->end_io_data = task;
=======
	req->end_io_data = cmd;
>>>>>>> refs/remotes/origin/master
	req->cmd_len = scsi_command_size(pt->pscsi_cdb);
	req->cmd = &pt->pscsi_cdb[0];
	req->sense = &pt->pscsi_sense[0];
	req->sense_len = 0;
	if (pdv->pdv_sd->type == TYPE_DISK)
		req->timeout = PS_TIMEOUT_DISK;
	else
		req->timeout = PS_TIMEOUT_OTHER;
	req->retries = PS_RETRY;

	blk_execute_rq_nowait(pdv->pdv_sd->request_queue, NULL, req,
<<<<<<< HEAD
			(task->task_se_cmd->sam_task_attr == MSG_HEAD_TAG),
=======
			(cmd->sam_task_attr == MSG_HEAD_TAG),
>>>>>>> refs/remotes/origin/master
			pscsi_req_done);

	return 0;

<<<<<<< HEAD
fail:
	while (hbio) {
		struct bio *bio = hbio;
		hbio = hbio->bi_next;
		bio->bi_next = NULL;
		bio_endio(bio, 0);	/* XXX: should be error */
	}
	cmd->scsi_sense_reason = TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
	return -ENOMEM;
>>>>>>> refs/remotes/origin/cm-10.0
}

/*	pscsi_get_sense_buffer():
 *
 *
 */
static unsigned char *pscsi_get_sense_buffer(struct se_task *task)
{
	struct pscsi_plugin_task *pt = PSCSI_TASK(task);

<<<<<<< HEAD
	return (unsigned char *)&pt->pscsi_sense[0];
=======
	return pt->pscsi_sense;
>>>>>>> refs/remotes/origin/cm-10.0
}

/*	pscsi_get_device_rev():
 *
 *
 */
static u32 pscsi_get_device_rev(struct se_device *dev)
{
	struct pscsi_dev_virt *pdv = dev->dev_ptr;
	struct scsi_device *sd = pdv->pdv_sd;

	return (sd->scsi_level - 1) ? sd->scsi_level - 1 : 1;
=======
fail_free_bio:
	while (hbio) {
		struct bio *bio = hbio;
		hbio = hbio->bi_next;
		bio_endio(bio, 0);	/* XXX: should be error */
	}
	ret = TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
fail:
	kfree(pt);
	return ret;
>>>>>>> refs/remotes/origin/master
}

/*	pscsi_get_device_type():
 *
 *
 */
static u32 pscsi_get_device_type(struct se_device *dev)
{
<<<<<<< HEAD
	struct pscsi_dev_virt *pdv = dev->dev_ptr;
=======
	struct pscsi_dev_virt *pdv = PSCSI_DEV(dev);
>>>>>>> refs/remotes/origin/master
	struct scsi_device *sd = pdv->pdv_sd;

	return sd->type;
}

static sector_t pscsi_get_blocks(struct se_device *dev)
{
<<<<<<< HEAD
	struct pscsi_dev_virt *pdv = dev->dev_ptr;
=======
	struct pscsi_dev_virt *pdv = PSCSI_DEV(dev);
>>>>>>> refs/remotes/origin/master

	if (pdv->pdv_bd && pdv->pdv_bd->bd_part)
		return pdv->pdv_bd->bd_part->nr_sects;

	dump_stack();
	return 0;
}

<<<<<<< HEAD
/*	pscsi_handle_SAM_STATUS_failures():
 *
 *
 */
static inline void pscsi_process_SAM_status(
	struct se_task *task,
	struct pscsi_plugin_task *pt)
{
	task->task_scsi_status = status_byte(pt->pscsi_result);
<<<<<<< HEAD
	if ((task->task_scsi_status)) {
		task->task_scsi_status <<= 1;
		printk(KERN_INFO "PSCSI Status Byte exception at task: %p CDB:"
=======
	if (task->task_scsi_status) {
		task->task_scsi_status <<= 1;
		pr_debug("PSCSI Status Byte exception at task: %p CDB:"
>>>>>>> refs/remotes/origin/cm-10.0
			" 0x%02x Result: 0x%08x\n", task, pt->pscsi_cdb[0],
=======
static void pscsi_req_done(struct request *req, int uptodate)
{
	struct se_cmd *cmd = req->end_io_data;
	struct pscsi_plugin_task *pt = cmd->priv;

	pt->pscsi_result = req->errors;
	pt->pscsi_resid = req->resid_len;

	cmd->scsi_status = status_byte(pt->pscsi_result) << 1;
	if (cmd->scsi_status) {
		pr_debug("PSCSI Status Byte exception at cmd: %p CDB:"
			" 0x%02x Result: 0x%08x\n", cmd, pt->pscsi_cdb[0],
>>>>>>> refs/remotes/origin/master
			pt->pscsi_result);
	}

	switch (host_byte(pt->pscsi_result)) {
	case DID_OK:
<<<<<<< HEAD
		transport_complete_task(task, (!task->task_scsi_status));
		break;
	default:
<<<<<<< HEAD
		printk(KERN_INFO "PSCSI Host Byte exception at task: %p CDB:"
			" 0x%02x Result: 0x%08x\n", task, pt->pscsi_cdb[0],
			pt->pscsi_result);
		task->task_scsi_status = SAM_STAT_CHECK_CONDITION;
		task->task_error_status = PYX_TRANSPORT_UNKNOWN_SAM_OPCODE;
		TASK_CMD(task)->transport_error_status =
					PYX_TRANSPORT_UNKNOWN_SAM_OPCODE;
		transport_complete_task(task, 0);
		break;
	}

	return;
=======
		pr_debug("PSCSI Host Byte exception at task: %p CDB:"
			" 0x%02x Result: 0x%08x\n", task, pt->pscsi_cdb[0],
			pt->pscsi_result);
		task->task_scsi_status = SAM_STAT_CHECK_CONDITION;
		task->task_se_cmd->scsi_sense_reason =
					TCM_UNSUPPORTED_SCSI_OPCODE;
		transport_complete_task(task, 0);
		break;
	}
>>>>>>> refs/remotes/origin/cm-10.0
}

static void pscsi_req_done(struct request *req, int uptodate)
{
	struct se_task *task = req->end_io_data;
	struct pscsi_plugin_task *pt = PSCSI_TASK(task);

	pt->pscsi_result = req->errors;
	pt->pscsi_resid = req->resid_len;

	pscsi_process_SAM_status(task, pt);
<<<<<<< HEAD
	/*
	 * Release BIDI-READ if present
	 */
	if (req->next_rq != NULL)
		__blk_put_request(req->q, req->next_rq);

	__blk_put_request(req->q, req);
	pt->pscsi_req = NULL;
=======
	__blk_put_request(req->q, req);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		target_complete_cmd(cmd, cmd->scsi_status);
		break;
	default:
		pr_debug("PSCSI Host Byte exception at cmd: %p CDB:"
			" 0x%02x Result: 0x%08x\n", cmd, pt->pscsi_cdb[0],
			pt->pscsi_result);
		target_complete_cmd(cmd, SAM_STAT_CHECK_CONDITION);
		break;
	}

	__blk_put_request(req->q, req);
	kfree(pt);
>>>>>>> refs/remotes/origin/master
}

static struct se_subsystem_api pscsi_template = {
	.name			= "pscsi",
	.owner			= THIS_MODULE,
	.transport_type		= TRANSPORT_PLUGIN_PHBA_PDEV,
<<<<<<< HEAD
<<<<<<< HEAD
	.cdb_none		= pscsi_CDB_none,
	.map_task_non_SG	= pscsi_map_task_non_SG,
	.map_task_SG		= pscsi_map_task_SG,
=======
>>>>>>> refs/remotes/origin/cm-10.0
	.attach_hba		= pscsi_attach_hba,
	.detach_hba		= pscsi_detach_hba,
	.pmode_enable_hba	= pscsi_pmode_enable_hba,
	.allocate_virtdevice	= pscsi_allocate_virtdevice,
	.create_virtdevice	= pscsi_create_virtdevice,
	.free_device		= pscsi_free_device,
	.transport_complete	= pscsi_transport_complete,
	.alloc_task		= pscsi_alloc_task,
	.do_task		= pscsi_do_task,
	.free_task		= pscsi_free_task,
	.check_configfs_dev_params = pscsi_check_configfs_dev_params,
	.set_configfs_dev_params = pscsi_set_configfs_dev_params,
	.show_configfs_dev_params = pscsi_show_configfs_dev_params,
<<<<<<< HEAD
	.get_cdb		= pscsi_get_cdb,
=======
>>>>>>> refs/remotes/origin/cm-10.0
	.get_sense_buffer	= pscsi_get_sense_buffer,
	.get_device_rev		= pscsi_get_device_rev,
=======
	.attach_hba		= pscsi_attach_hba,
	.detach_hba		= pscsi_detach_hba,
	.pmode_enable_hba	= pscsi_pmode_enable_hba,
	.alloc_device		= pscsi_alloc_device,
	.configure_device	= pscsi_configure_device,
	.free_device		= pscsi_free_device,
	.transport_complete	= pscsi_transport_complete,
	.parse_cdb		= pscsi_parse_cdb,
	.set_configfs_dev_params = pscsi_set_configfs_dev_params,
	.show_configfs_dev_params = pscsi_show_configfs_dev_params,
>>>>>>> refs/remotes/origin/master
	.get_device_type	= pscsi_get_device_type,
	.get_blocks		= pscsi_get_blocks,
};

static int __init pscsi_module_init(void)
{
	return transport_subsystem_register(&pscsi_template);
}

<<<<<<< HEAD
static void pscsi_module_exit(void)
=======
static void __exit pscsi_module_exit(void)
>>>>>>> refs/remotes/origin/master
{
	transport_subsystem_release(&pscsi_template);
}

MODULE_DESCRIPTION("TCM PSCSI subsystem plugin");
MODULE_AUTHOR("nab@Linux-iSCSI.org");
MODULE_LICENSE("GPL");

module_init(pscsi_module_init);
module_exit(pscsi_module_exit);
