/*******************************************************************************
 * Filename:  target_core_iblock.c
 *
 * This file contains the Storage Engine  <-> Linux BlockIO transport
 * specific functions.
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
#include <linux/fs.h>
#include <linux/blkdev.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/bio.h>
#include <linux/genhd.h>
#include <linux/file.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <scsi/scsi.h>
#include <scsi/scsi_host.h>

#include <target/target_core_base.h>
<<<<<<< HEAD
#include <target/target_core_device.h>
#include <target/target_core_transport.h>

#include "target_core_iblock.h"

#if 0
#define DEBUG_IBLOCK(x...) printk(x)
#else
#define DEBUG_IBLOCK(x...)
#endif
=======
=======
#include <linux/module.h>
#include <scsi/scsi.h>
#include <scsi/scsi_host.h>
#include <asm/unaligned.h>

#include <target/target_core_base.h>
>>>>>>> refs/remotes/origin/master
#include <target/target_core_backend.h>

#include "target_core_iblock.h"

#define IBLOCK_MAX_BIO_PER_TASK	 32	/* max # of bios to submit at a time */
#define IBLOCK_BIO_POOL_SIZE	128
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

static struct se_subsystem_api iblock_template;

static void iblock_bio_done(struct bio *, int);
=======

static inline struct iblock_dev *IBLOCK_DEV(struct se_device *dev)
{
	return container_of(dev, struct iblock_dev, dev);
}


static struct se_subsystem_api iblock_template;
>>>>>>> refs/remotes/origin/master

/*	iblock_attach_hba(): (Part of se_subsystem_api_t template)
 *
 *
 */
static int iblock_attach_hba(struct se_hba *hba, u32 host_id)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct iblock_hba *ib_host;

	ib_host = kzalloc(sizeof(struct iblock_hba), GFP_KERNEL);
	if (!(ib_host)) {
		printk(KERN_ERR "Unable to allocate memory for"
				" struct iblock_hba\n");
		return -ENOMEM;
	}

	ib_host->iblock_host_id = host_id;

	atomic_set(&hba->left_queue_depth, IBLOCK_HBA_QUEUE_DEPTH);
	atomic_set(&hba->max_queue_depth, IBLOCK_HBA_QUEUE_DEPTH);
	hba->hba_ptr = (void *) ib_host;

	printk(KERN_INFO "CORE_HBA[%d] - TCM iBlock HBA Driver %s on"
		" Generic Target Core Stack %s\n", hba->hba_id,
		IBLOCK_VERSION, TARGET_CORE_MOD_VERSION);

	printk(KERN_INFO "CORE_HBA[%d] - Attached iBlock HBA: %u to Generic"
		" Target Core TCQ Depth: %d\n", hba->hba_id,
		ib_host->iblock_host_id, atomic_read(&hba->max_queue_depth));

=======
	pr_debug("CORE_HBA[%d] - TCM iBlock HBA Driver %s on"
		" Generic Target Core Stack %s\n", hba->hba_id,
		IBLOCK_VERSION, TARGET_CORE_MOD_VERSION);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pr_debug("CORE_HBA[%d] - TCM iBlock HBA Driver %s on"
		" Generic Target Core Stack %s\n", hba->hba_id,
		IBLOCK_VERSION, TARGET_CORE_MOD_VERSION);
>>>>>>> refs/remotes/origin/master
	return 0;
}

static void iblock_detach_hba(struct se_hba *hba)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct iblock_hba *ib_host = hba->hba_ptr;

	printk(KERN_INFO "CORE_HBA[%d] - Detached iBlock HBA: %u from Generic"
		" Target Core\n", hba->hba_id, ib_host->iblock_host_id);

	kfree(ib_host);
	hba->hba_ptr = NULL;
=======
>>>>>>> refs/remotes/origin/cm-10.0
}

static void *iblock_allocate_virtdevice(struct se_hba *hba, const char *name)
{
	struct iblock_dev *ib_dev = NULL;
<<<<<<< HEAD
	struct iblock_hba *ib_host = hba->hba_ptr;

	ib_dev = kzalloc(sizeof(struct iblock_dev), GFP_KERNEL);
	if (!(ib_dev)) {
		printk(KERN_ERR "Unable to allocate struct iblock_dev\n");
		return NULL;
	}
	ib_dev->ibd_host = ib_host;

	printk(KERN_INFO  "IBLOCK: Allocated ib_dev for %s\n", name);
=======
=======
}

static struct se_device *iblock_alloc_device(struct se_hba *hba, const char *name)
{
	struct iblock_dev *ib_dev = NULL;
>>>>>>> refs/remotes/origin/master

	ib_dev = kzalloc(sizeof(struct iblock_dev), GFP_KERNEL);
	if (!ib_dev) {
		pr_err("Unable to allocate struct iblock_dev\n");
		return NULL;
	}

	pr_debug( "IBLOCK: Allocated ib_dev for %s\n", name);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

	return ib_dev;
}

static struct se_device *iblock_create_virtdevice(
	struct se_hba *hba,
	struct se_subsystem_dev *se_dev,
	void *p)
{
	struct iblock_dev *ib_dev = p;
	struct se_device *dev;
	struct se_dev_limits dev_limits;
	struct block_device *bd = NULL;
	struct request_queue *q;
	struct queue_limits *limits;
	u32 dev_flags = 0;
	int ret = -EINVAL;

<<<<<<< HEAD
	if (!(ib_dev)) {
		printk(KERN_ERR "Unable to locate struct iblock_dev parameter\n");
		return ERR_PTR(ret);
	}
	memset(&dev_limits, 0, sizeof(struct se_dev_limits));
	/*
	 * These settings need to be made tunable..
	 */
	ib_dev->ibd_bio_set = bioset_create(32, 64);
	if (!(ib_dev->ibd_bio_set)) {
		printk(KERN_ERR "IBLOCK: Unable to create bioset()\n");
		return ERR_PTR(-ENOMEM);
	}
	printk(KERN_INFO "IBLOCK: Created bio_set()\n");
=======
	if (!ib_dev) {
		pr_err("Unable to locate struct iblock_dev parameter\n");
		return ERR_PTR(ret);
	}
	memset(&dev_limits, 0, sizeof(struct se_dev_limits));

	ib_dev->ibd_bio_set = bioset_create(IBLOCK_BIO_POOL_SIZE, 0);
	if (!ib_dev->ibd_bio_set) {
		pr_err("IBLOCK: Unable to create bioset()\n");
		return ERR_PTR(-ENOMEM);
	}
	pr_debug("IBLOCK: Created bio_set()\n");
>>>>>>> refs/remotes/origin/cm-10.0
	/*
	 * iblock_check_configfs_dev_params() ensures that ib_dev->ibd_udev_path
	 * must already have been set in order for echo 1 > $HBA/$DEV/enable to run.
	 */
<<<<<<< HEAD
	printk(KERN_INFO  "IBLOCK: Claiming struct block_device: %s\n",
=======
	pr_debug( "IBLOCK: Claiming struct block_device: %s\n",
>>>>>>> refs/remotes/origin/cm-10.0
			ib_dev->ibd_udev_path);

	bd = blkdev_get_by_path(ib_dev->ibd_udev_path,
				FMODE_WRITE|FMODE_READ|FMODE_EXCL, ib_dev);
	if (IS_ERR(bd)) {
		ret = PTR_ERR(bd);
		goto failed;
	}
	/*
	 * Setup the local scope queue_limits from struct request_queue->limits
	 * to pass into transport_add_device_to_core_hba() as struct se_dev_limits.
	 */
	q = bdev_get_queue(bd);
	limits = &dev_limits.limits;
	limits->logical_block_size = bdev_logical_block_size(bd);
<<<<<<< HEAD
	limits->max_hw_sectors = queue_max_hw_sectors(q);
	limits->max_sectors = queue_max_sectors(q);
	dev_limits.hw_queue_depth = IBLOCK_MAX_DEVICE_QUEUE_DEPTH;
	dev_limits.queue_depth = IBLOCK_DEVICE_QUEUE_DEPTH;

	ib_dev->ibd_major = MAJOR(bd->bd_dev);
	ib_dev->ibd_minor = MINOR(bd->bd_dev);
	ib_dev->ibd_bd = bd;

	dev = transport_add_device_to_core_hba(hba,
			&iblock_template, se_dev, dev_flags, (void *)ib_dev,
			&dev_limits, "IBLOCK", IBLOCK_VERSION);
	if (!(dev))
		goto failed;

	ib_dev->ibd_depth = dev->queue_depth;

=======
	limits->max_hw_sectors = UINT_MAX;
	limits->max_sectors = UINT_MAX;
	dev_limits.hw_queue_depth = q->nr_requests;
	dev_limits.queue_depth = q->nr_requests;

	ib_dev->ibd_bd = bd;

	dev = transport_add_device_to_core_hba(hba,
			&iblock_template, se_dev, dev_flags, ib_dev,
			&dev_limits, "IBLOCK", IBLOCK_VERSION);
	if (!dev)
		goto failed;

>>>>>>> refs/remotes/origin/cm-10.0
=======

	return &ib_dev->dev;
}

static int iblock_configure_device(struct se_device *dev)
{
	struct iblock_dev *ib_dev = IBLOCK_DEV(dev);
	struct request_queue *q;
	struct block_device *bd = NULL;
	fmode_t mode;
	int ret = -ENOMEM;

	if (!(ib_dev->ibd_flags & IBDF_HAS_UDEV_PATH)) {
		pr_err("Missing udev_path= parameters for IBLOCK\n");
		return -EINVAL;
	}

	ib_dev->ibd_bio_set = bioset_create(IBLOCK_BIO_POOL_SIZE, 0);
	if (!ib_dev->ibd_bio_set) {
		pr_err("IBLOCK: Unable to create bioset\n");
		goto out;
	}

	pr_debug( "IBLOCK: Claiming struct block_device: %s\n",
			ib_dev->ibd_udev_path);

	mode = FMODE_READ|FMODE_EXCL;
	if (!ib_dev->ibd_readonly)
		mode |= FMODE_WRITE;

	bd = blkdev_get_by_path(ib_dev->ibd_udev_path, mode, ib_dev);
	if (IS_ERR(bd)) {
		ret = PTR_ERR(bd);
		goto out_free_bioset;
	}
	ib_dev->ibd_bd = bd;

	q = bdev_get_queue(bd);

	dev->dev_attrib.hw_block_size = bdev_logical_block_size(bd);
	dev->dev_attrib.hw_max_sectors = UINT_MAX;
	dev->dev_attrib.hw_queue_depth = q->nr_requests;

>>>>>>> refs/remotes/origin/master
	/*
	 * Check if the underlying struct block_device request_queue supports
	 * the QUEUE_FLAG_DISCARD bit for UNMAP/WRITE_SAME in SCSI + TRIM
	 * in ATA and we need to set TPE=1
	 */
	if (blk_queue_discard(q)) {
<<<<<<< HEAD
<<<<<<< HEAD
		DEV_ATTRIB(dev)->max_unmap_lba_count =
=======
		dev->se_sub_dev->se_dev_attrib.max_unmap_lba_count =
>>>>>>> refs/remotes/origin/cm-10.0
				q->limits.max_discard_sectors;
		/*
		 * Currently hardcoded to 1 in Linux/SCSI code..
		 */
<<<<<<< HEAD
		DEV_ATTRIB(dev)->max_unmap_block_desc_count = 1;
		DEV_ATTRIB(dev)->unmap_granularity =
				q->limits.discard_granularity;
		DEV_ATTRIB(dev)->unmap_granularity_alignment =
				q->limits.discard_alignment;

		printk(KERN_INFO "IBLOCK: BLOCK Discard support available,"
				" disabled by default\n");
	}

=======
		dev->se_sub_dev->se_dev_attrib.max_unmap_block_desc_count = 1;
		dev->se_sub_dev->se_dev_attrib.unmap_granularity =
				q->limits.discard_granularity >> 9;
		dev->se_sub_dev->se_dev_attrib.unmap_granularity_alignment =
=======
		dev->dev_attrib.max_unmap_lba_count =
				q->limits.max_discard_sectors;

		/*
		 * Currently hardcoded to 1 in Linux/SCSI code..
		 */
		dev->dev_attrib.max_unmap_block_desc_count = 1;
		dev->dev_attrib.unmap_granularity =
				q->limits.discard_granularity >> 9;
		dev->dev_attrib.unmap_granularity_alignment =
>>>>>>> refs/remotes/origin/master
				q->limits.discard_alignment;

		pr_debug("IBLOCK: BLOCK Discard support available,"
				" disabled by default\n");
	}
<<<<<<< HEAD

	if (blk_queue_nonrot(q))
		dev->se_sub_dev->se_dev_attrib.is_nonrot = 1;

>>>>>>> refs/remotes/origin/cm-10.0
	return dev;

failed:
	if (ib_dev->ibd_bio_set) {
		bioset_free(ib_dev->ibd_bio_set);
		ib_dev->ibd_bio_set = NULL;
	}
	ib_dev->ibd_bd = NULL;
<<<<<<< HEAD
	ib_dev->ibd_major = 0;
	ib_dev->ibd_minor = 0;
=======
>>>>>>> refs/remotes/origin/cm-10.0
	return ERR_PTR(ret);
}

static void iblock_free_device(void *p)
{
	struct iblock_dev *ib_dev = p;
=======
	/*
	 * Enable write same emulation for IBLOCK and use 0xFFFF as
	 * the smaller WRITE_SAME(10) only has a two-byte block count.
	 */
	dev->dev_attrib.max_write_same_len = 0xFFFF;

	if (blk_queue_nonrot(q))
		dev->dev_attrib.is_nonrot = 1;

	return 0;

out_free_bioset:
	bioset_free(ib_dev->ibd_bio_set);
	ib_dev->ibd_bio_set = NULL;
out:
	return ret;
}

static void iblock_free_device(struct se_device *dev)
{
	struct iblock_dev *ib_dev = IBLOCK_DEV(dev);
>>>>>>> refs/remotes/origin/master

	if (ib_dev->ibd_bd != NULL)
		blkdev_put(ib_dev->ibd_bd, FMODE_WRITE|FMODE_READ|FMODE_EXCL);
	if (ib_dev->ibd_bio_set != NULL)
		bioset_free(ib_dev->ibd_bio_set);
	kfree(ib_dev);
}

<<<<<<< HEAD
static inline struct iblock_req *IBLOCK_REQ(struct se_task *task)
{
	return container_of(task, struct iblock_req, ib_task);
}

static struct se_task *
<<<<<<< HEAD
iblock_alloc_task(struct se_cmd *cmd)
=======
iblock_alloc_task(unsigned char *cdb)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct iblock_req *ib_req;

	ib_req = kzalloc(sizeof(struct iblock_req), GFP_KERNEL);
<<<<<<< HEAD
	if (!(ib_req)) {
		printk(KERN_ERR "Unable to allocate memory for struct iblock_req\n");
		return NULL;
	}

	ib_req->ib_dev = SE_DEV(cmd)->dev_ptr;
	atomic_set(&ib_req->ib_bio_cnt, 0);
=======
	if (!ib_req) {
		pr_err("Unable to allocate memory for struct iblock_req\n");
		return NULL;
	}

	atomic_set(&ib_req->pending, 1);
>>>>>>> refs/remotes/origin/cm-10.0
	return &ib_req->ib_task;
}

=======
>>>>>>> refs/remotes/origin/master
static unsigned long long iblock_emulate_read_cap_with_block_size(
	struct se_device *dev,
	struct block_device *bd,
	struct request_queue *q)
{
	unsigned long long blocks_long = (div_u64(i_size_read(bd->bd_inode),
					bdev_logical_block_size(bd)) - 1);
	u32 block_size = bdev_logical_block_size(bd);

<<<<<<< HEAD
<<<<<<< HEAD
	if (block_size == DEV_ATTRIB(dev)->block_size)
=======
	if (block_size == dev->se_sub_dev->se_dev_attrib.block_size)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (block_size == dev->dev_attrib.block_size)
>>>>>>> refs/remotes/origin/master
		return blocks_long;

	switch (block_size) {
	case 4096:
<<<<<<< HEAD
<<<<<<< HEAD
		switch (DEV_ATTRIB(dev)->block_size) {
=======
		switch (dev->se_sub_dev->se_dev_attrib.block_size) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
		switch (dev->dev_attrib.block_size) {
>>>>>>> refs/remotes/origin/master
		case 2048:
			blocks_long <<= 1;
			break;
		case 1024:
			blocks_long <<= 2;
			break;
		case 512:
			blocks_long <<= 3;
		default:
			break;
		}
		break;
	case 2048:
<<<<<<< HEAD
<<<<<<< HEAD
		switch (DEV_ATTRIB(dev)->block_size) {
=======
		switch (dev->se_sub_dev->se_dev_attrib.block_size) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
		switch (dev->dev_attrib.block_size) {
>>>>>>> refs/remotes/origin/master
		case 4096:
			blocks_long >>= 1;
			break;
		case 1024:
			blocks_long <<= 1;
			break;
		case 512:
			blocks_long <<= 2;
			break;
		default:
			break;
		}
		break;
	case 1024:
<<<<<<< HEAD
<<<<<<< HEAD
		switch (DEV_ATTRIB(dev)->block_size) {
=======
		switch (dev->se_sub_dev->se_dev_attrib.block_size) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
		switch (dev->dev_attrib.block_size) {
>>>>>>> refs/remotes/origin/master
		case 4096:
			blocks_long >>= 2;
			break;
		case 2048:
			blocks_long >>= 1;
			break;
		case 512:
			blocks_long <<= 1;
			break;
		default:
			break;
		}
		break;
	case 512:
<<<<<<< HEAD
<<<<<<< HEAD
		switch (DEV_ATTRIB(dev)->block_size) {
=======
		switch (dev->se_sub_dev->se_dev_attrib.block_size) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
		switch (dev->dev_attrib.block_size) {
>>>>>>> refs/remotes/origin/master
		case 4096:
			blocks_long >>= 3;
			break;
		case 2048:
			blocks_long >>= 2;
			break;
		case 1024:
			blocks_long >>= 1;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	return blocks_long;
}

<<<<<<< HEAD
<<<<<<< HEAD
/*
 * Emulate SYCHRONIZE_CACHE_*
 */
static void iblock_emulate_sync_cache(struct se_task *task)
{
	struct se_cmd *cmd = TASK_CMD(task);
	struct iblock_dev *ib_dev = cmd->se_dev->dev_ptr;
	int immed = (T_TASK(cmd)->t_task_cdb[1] & 0x2);
	sector_t error_sector;
	int ret;

	/*
	 * If the Immediate bit is set, queue up the GOOD response
	 * for this SYNCHRONIZE_CACHE op
=======
=======
static void iblock_complete_cmd(struct se_cmd *cmd)
{
	struct iblock_req *ibr = cmd->priv;
	u8 status;

	if (!atomic_dec_and_test(&ibr->pending))
		return;

	if (atomic_read(&ibr->ib_bio_err_cnt))
		status = SAM_STAT_CHECK_CONDITION;
	else
		status = SAM_STAT_GOOD;

	target_complete_cmd(cmd, status);
	kfree(ibr);
}

static void iblock_bio_done(struct bio *bio, int err)
{
	struct se_cmd *cmd = bio->bi_private;
	struct iblock_req *ibr = cmd->priv;

	/*
	 * Set -EIO if !BIO_UPTODATE and the passed is still err=0
	 */
	if (!test_bit(BIO_UPTODATE, &bio->bi_flags) && !err)
		err = -EIO;

	if (err != 0) {
		pr_err("test_bit(BIO_UPTODATE) failed for bio: %p,"
			" err: %d\n", bio, err);
		/*
		 * Bump the ib_bio_err_cnt and release bio.
		 */
		atomic_inc(&ibr->ib_bio_err_cnt);
		smp_mb__after_atomic_inc();
	}

	bio_put(bio);

	iblock_complete_cmd(cmd);
}

static struct bio *
iblock_get_bio(struct se_cmd *cmd, sector_t lba, u32 sg_num)
{
	struct iblock_dev *ib_dev = IBLOCK_DEV(cmd->se_dev);
	struct bio *bio;

	/*
	 * Only allocate as many vector entries as the bio code allows us to,
	 * we'll loop later on until we have handled the whole request.
	 */
	if (sg_num > BIO_MAX_PAGES)
		sg_num = BIO_MAX_PAGES;

	bio = bio_alloc_bioset(GFP_NOIO, sg_num, ib_dev->ibd_bio_set);
	if (!bio) {
		pr_err("Unable to allocate memory for bio\n");
		return NULL;
	}

	bio->bi_bdev = ib_dev->ibd_bd;
	bio->bi_private = cmd;
	bio->bi_end_io = &iblock_bio_done;
	bio->bi_sector = lba;

	return bio;
}

static void iblock_submit_bios(struct bio_list *list, int rw)
{
	struct blk_plug plug;
	struct bio *bio;

	blk_start_plug(&plug);
	while ((bio = bio_list_pop(list)))
		submit_bio(rw, bio);
	blk_finish_plug(&plug);
}

>>>>>>> refs/remotes/origin/master
static void iblock_end_io_flush(struct bio *bio, int err)
{
	struct se_cmd *cmd = bio->bi_private;

	if (err)
		pr_err("IBLOCK: cache flush failed: %d\n", err);

<<<<<<< HEAD
	if (cmd)
		transport_complete_sync_cache(cmd, err == 0);
=======
	if (cmd) {
		if (err)
			target_complete_cmd(cmd, SAM_STAT_CHECK_CONDITION);
		else
			target_complete_cmd(cmd, SAM_STAT_GOOD);
	}

>>>>>>> refs/remotes/origin/master
	bio_put(bio);
}

/*
 * Implement SYCHRONIZE CACHE.  Note that we can't handle lba ranges and must
 * always flush the whole cache.
 */
<<<<<<< HEAD
static void iblock_emulate_sync_cache(struct se_task *task)
{
	struct se_cmd *cmd = task->task_se_cmd;
	struct iblock_dev *ib_dev = cmd->se_dev->dev_ptr;
=======
static sense_reason_t
iblock_execute_sync_cache(struct se_cmd *cmd)
{
	struct iblock_dev *ib_dev = IBLOCK_DEV(cmd->se_dev);
>>>>>>> refs/remotes/origin/master
	int immed = (cmd->t_task_cdb[1] & 0x2);
	struct bio *bio;

	/*
	 * If the Immediate bit is set, queue up the GOOD response
	 * for this SYNCHRONIZE_CACHE op.
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	 */
	if (immed)
		transport_complete_sync_cache(cmd, 1);

<<<<<<< HEAD
	/*
	 * blkdev_issue_flush() does not support a specifying a range, so
	 * we have to flush the entire cache.
	 */
	ret = blkdev_issue_flush(ib_dev->ibd_bd, GFP_KERNEL, &error_sector);
	if (ret != 0) {
		printk(KERN_ERR "IBLOCK: block_issue_flush() failed: %d "
			" error_sector: %llu\n", ret,
			(unsigned long long)error_sector);
	}

	if (!immed)
		transport_complete_sync_cache(cmd, ret == 0);
}

/*
 * Tell TCM Core that we are capable of WriteCache emulation for
 * an underlying struct se_device.
 */
static int iblock_emulated_write_cache(struct se_device *dev)
{
	return 1;
}

static int iblock_emulated_dpo(struct se_device *dev)
{
	return 0;
}

/*
 * Tell TCM Core that we will be emulating Forced Unit Access (FUA) for WRITEs
 * for TYPE_DISK.
 */
static int iblock_emulated_fua_write(struct se_device *dev)
{
	return 1;
}

static int iblock_emulated_fua_read(struct se_device *dev)
{
	return 0;
}

static int iblock_do_task(struct se_task *task)
{
	struct se_device *dev = task->task_se_cmd->se_dev;
	struct iblock_req *req = IBLOCK_REQ(task);
	struct bio *bio = req->ib_bio, *nbio = NULL;
	struct blk_plug plug;
	int rw;

	if (task->task_data_direction == DMA_TO_DEVICE) {
		/*
		 * Force data to disk if we pretend to not have a volatile
		 * write cache, or the initiator set the Force Unit Access bit.
		 */
		if (DEV_ATTRIB(dev)->emulate_write_cache == 0 ||
		    (DEV_ATTRIB(dev)->emulate_fua_write > 0 &&
		     T_TASK(task->task_se_cmd)->t_tasks_fua))
			rw = WRITE_FUA;
		else
			rw = WRITE;
	} else {
		rw = READ;
	}

	blk_start_plug(&plug);
	while (bio) {
		nbio = bio->bi_next;
		bio->bi_next = NULL;
		DEBUG_IBLOCK("Calling submit_bio() task: %p bio: %p"
			" bio->bi_sector: %llu\n", task, bio, bio->bi_sector);

		submit_bio(rw, bio);
		bio = nbio;
	}
	blk_finish_plug(&plug);

	return PYX_TRANSPORT_SENT_TO_TRANSPORT;
=======
	bio = bio_alloc(GFP_KERNEL, 0);
	bio->bi_end_io = iblock_end_io_flush;
	bio->bi_bdev = ib_dev->ibd_bd;
	if (!immed)
		bio->bi_private = cmd;
	submit_bio(WRITE_FLUSH, bio);
>>>>>>> refs/remotes/origin/cm-10.0
}

static int iblock_do_discard(struct se_device *dev, sector_t lba, u32 range)
{
	struct iblock_dev *ibd = dev->dev_ptr;
	struct block_device *bd = ibd->ibd_bd;
	int barrier = 0;

	return blkdev_issue_discard(bd, lba, range, GFP_KERNEL, barrier);
}

static void iblock_free_task(struct se_task *task)
{
<<<<<<< HEAD
	struct iblock_req *req = IBLOCK_REQ(task);
	struct bio *bio, *hbio = req->ib_bio;
	/*
	 * We only release the bio(s) here if iblock_bio_done() has not called
	 * bio_put() -> iblock_bio_destructor().
	 */
	while (hbio != NULL) {
		bio = hbio;
		hbio = hbio->bi_next;
		bio->bi_next = NULL;
		bio_put(bio);
	}

	kfree(req);
=======
	kfree(IBLOCK_REQ(task));
>>>>>>> refs/remotes/origin/cm-10.0
}

enum {
	Opt_udev_path, Opt_force, Opt_err
=======
	 */
	if (immed)
		target_complete_cmd(cmd, SAM_STAT_GOOD);

	bio = bio_alloc(GFP_KERNEL, 0);
	bio->bi_end_io = iblock_end_io_flush;
	bio->bi_bdev = ib_dev->ibd_bd;
	if (!immed)
		bio->bi_private = cmd;
	submit_bio(WRITE_FLUSH, bio);
	return 0;
}

static sense_reason_t
iblock_do_unmap(struct se_cmd *cmd, void *priv,
		sector_t lba, sector_t nolb)
{
	struct block_device *bdev = priv;
	int ret;

	ret = blkdev_issue_discard(bdev, lba, nolb, GFP_KERNEL, 0);
	if (ret < 0) {
		pr_err("blkdev_issue_discard() failed: %d\n", ret);
		return TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
	}

	return 0;
}

static sense_reason_t
iblock_execute_unmap(struct se_cmd *cmd)
{
	struct block_device *bdev = IBLOCK_DEV(cmd->se_dev)->ibd_bd;

	return sbc_execute_unmap(cmd, iblock_do_unmap, bdev);
}

static sense_reason_t
iblock_execute_write_same_unmap(struct se_cmd *cmd)
{
	struct block_device *bdev = IBLOCK_DEV(cmd->se_dev)->ibd_bd;
	sector_t lba = cmd->t_task_lba;
	sector_t nolb = sbc_get_write_same_sectors(cmd);
	int ret;

	ret = iblock_do_unmap(cmd, bdev, lba, nolb);
	if (ret)
		return ret;

	target_complete_cmd(cmd, GOOD);
	return 0;
}

static sense_reason_t
iblock_execute_write_same(struct se_cmd *cmd)
{
	struct iblock_req *ibr;
	struct scatterlist *sg;
	struct bio *bio;
	struct bio_list list;
	sector_t block_lba = cmd->t_task_lba;
	sector_t sectors = sbc_get_write_same_sectors(cmd);

	sg = &cmd->t_data_sg[0];

	if (cmd->t_data_nents > 1 ||
	    sg->length != cmd->se_dev->dev_attrib.block_size) {
		pr_err("WRITE_SAME: Illegal SGL t_data_nents: %u length: %u"
			" block_size: %u\n", cmd->t_data_nents, sg->length,
			cmd->se_dev->dev_attrib.block_size);
		return TCM_INVALID_CDB_FIELD;
	}

	ibr = kzalloc(sizeof(struct iblock_req), GFP_KERNEL);
	if (!ibr)
		goto fail;
	cmd->priv = ibr;

	bio = iblock_get_bio(cmd, block_lba, 1);
	if (!bio)
		goto fail_free_ibr;

	bio_list_init(&list);
	bio_list_add(&list, bio);

	atomic_set(&ibr->pending, 1);

	while (sectors) {
		while (bio_add_page(bio, sg_page(sg), sg->length, sg->offset)
				!= sg->length) {

			bio = iblock_get_bio(cmd, block_lba, 1);
			if (!bio)
				goto fail_put_bios;

			atomic_inc(&ibr->pending);
			bio_list_add(&list, bio);
		}

		/* Always in 512 byte units for Linux/Block */
		block_lba += sg->length >> IBLOCK_LBA_SHIFT;
		sectors -= 1;
	}

	iblock_submit_bios(&list, WRITE);
	return 0;

fail_put_bios:
	while ((bio = bio_list_pop(&list)))
		bio_put(bio);
fail_free_ibr:
	kfree(ibr);
fail:
	return TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
}

enum {
	Opt_udev_path, Opt_readonly, Opt_force, Opt_err
>>>>>>> refs/remotes/origin/master
};

static match_table_t tokens = {
	{Opt_udev_path, "udev_path=%s"},
<<<<<<< HEAD
=======
	{Opt_readonly, "readonly=%d"},
>>>>>>> refs/remotes/origin/master
	{Opt_force, "force=%d"},
	{Opt_err, NULL}
};

<<<<<<< HEAD
static ssize_t iblock_set_configfs_dev_params(struct se_hba *hba,
					       struct se_subsystem_dev *se_dev,
					       const char *page, ssize_t count)
{
	struct iblock_dev *ib_dev = se_dev->se_dev_su_ptr;
	char *orig, *ptr, *arg_p, *opts;
	substring_t args[MAX_OPT_ARGS];
<<<<<<< HEAD
	int ret = 0, arg, token;
=======
	int ret = 0, token;
>>>>>>> refs/remotes/origin/cm-10.0
=======
static ssize_t iblock_set_configfs_dev_params(struct se_device *dev,
		const char *page, ssize_t count)
{
	struct iblock_dev *ib_dev = IBLOCK_DEV(dev);
	char *orig, *ptr, *arg_p, *opts;
	substring_t args[MAX_OPT_ARGS];
	int ret = 0, token;
	unsigned long tmp_readonly;
>>>>>>> refs/remotes/origin/master

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
		case Opt_udev_path:
			if (ib_dev->ibd_bd) {
<<<<<<< HEAD
<<<<<<< HEAD
				printk(KERN_ERR "Unable to set udev_path= while"
=======
				pr_err("Unable to set udev_path= while"
>>>>>>> refs/remotes/origin/cm-10.0
=======
				pr_err("Unable to set udev_path= while"
>>>>>>> refs/remotes/origin/master
					" ib_dev->ibd_bd exists\n");
				ret = -EEXIST;
				goto out;
			}
<<<<<<< HEAD
=======
			if (match_strlcpy(ib_dev->ibd_udev_path, &args[0],
				SE_UDEV_PATH_LEN) == 0) {
				ret = -EINVAL;
				break;
			}
			pr_debug("IBLOCK: Referencing UDEV path: %s\n",
					ib_dev->ibd_udev_path);
			ib_dev->ibd_flags |= IBDF_HAS_UDEV_PATH;
			break;
		case Opt_readonly:
>>>>>>> refs/remotes/origin/master
			arg_p = match_strdup(&args[0]);
			if (!arg_p) {
				ret = -ENOMEM;
				break;
			}
<<<<<<< HEAD
			snprintf(ib_dev->ibd_udev_path, SE_UDEV_PATH_LEN,
					"%s", arg_p);
			kfree(arg_p);
<<<<<<< HEAD
			printk(KERN_INFO "IBLOCK: Referencing UDEV path: %s\n",
=======
			pr_debug("IBLOCK: Referencing UDEV path: %s\n",
>>>>>>> refs/remotes/origin/cm-10.0
					ib_dev->ibd_udev_path);
			ib_dev->ibd_flags |= IBDF_HAS_UDEV_PATH;
			break;
		case Opt_force:
<<<<<<< HEAD
			match_int(args, &arg);
			ib_dev->ibd_force = arg;
			printk(KERN_INFO "IBLOCK: Set force=%d\n",
				ib_dev->ibd_force);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
			ret = kstrtoul(arg_p, 0, &tmp_readonly);
			kfree(arg_p);
			if (ret < 0) {
				pr_err("kstrtoul() failed for"
						" readonly=\n");
				goto out;
			}
			ib_dev->ibd_readonly = tmp_readonly;
			pr_debug("IBLOCK: readonly: %d\n", ib_dev->ibd_readonly);
			break;
		case Opt_force:
>>>>>>> refs/remotes/origin/master
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
static ssize_t iblock_check_configfs_dev_params(
	struct se_hba *hba,
	struct se_subsystem_dev *se_dev)
{
	struct iblock_dev *ibd = se_dev->se_dev_su_ptr;

	if (!(ibd->ibd_flags & IBDF_HAS_UDEV_PATH)) {
<<<<<<< HEAD
		printk(KERN_ERR "Missing udev_path= parameters for IBLOCK\n");
		return -1;
=======
		pr_err("Missing udev_path= parameters for IBLOCK\n");
		return -EINVAL;
>>>>>>> refs/remotes/origin/cm-10.0
	}

	return 0;
}

static ssize_t iblock_show_configfs_dev_params(
	struct se_hba *hba,
	struct se_subsystem_dev *se_dev,
	char *b)
{
	struct iblock_dev *ibd = se_dev->se_dev_su_ptr;
	struct block_device *bd = ibd->ibd_bd;
=======
static ssize_t iblock_show_configfs_dev_params(struct se_device *dev, char *b)
{
	struct iblock_dev *ib_dev = IBLOCK_DEV(dev);
	struct block_device *bd = ib_dev->ibd_bd;
>>>>>>> refs/remotes/origin/master
	char buf[BDEVNAME_SIZE];
	ssize_t bl = 0;

	if (bd)
		bl += sprintf(b + bl, "iBlock device: %s",
				bdevname(bd, buf));
<<<<<<< HEAD
	if (ibd->ibd_flags & IBDF_HAS_UDEV_PATH) {
		bl += sprintf(b + bl, "  UDEV PATH: %s\n",
				ibd->ibd_udev_path);
	} else
		bl += sprintf(b + bl, "\n");
=======
	if (ib_dev->ibd_flags & IBDF_HAS_UDEV_PATH)
		bl += sprintf(b + bl, "  UDEV PATH: %s",
				ib_dev->ibd_udev_path);
	bl += sprintf(b + bl, "  readonly: %d\n", ib_dev->ibd_readonly);
>>>>>>> refs/remotes/origin/master

	bl += sprintf(b + bl, "        ");
	if (bd) {
		bl += sprintf(b + bl, "Major: %d Minor: %d  %s\n",
<<<<<<< HEAD
<<<<<<< HEAD
			ibd->ibd_major, ibd->ibd_minor, (!bd->bd_contains) ?
			"" : (bd->bd_holder == (struct iblock_dev *)ibd) ?
			"CLAIMED: IBLOCK" : "CLAIMED: OS");
	} else {
		bl += sprintf(b + bl, "Major: %d Minor: %d\n",
			ibd->ibd_major, ibd->ibd_minor);
=======
			MAJOR(bd->bd_dev), MINOR(bd->bd_dev), (!bd->bd_contains) ?
			"" : (bd->bd_holder == ibd) ?
			"CLAIMED: IBLOCK" : "CLAIMED: OS");
	} else {
		bl += sprintf(b + bl, "Major: 0 Minor: 0\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
			MAJOR(bd->bd_dev), MINOR(bd->bd_dev), (!bd->bd_contains) ?
			"" : (bd->bd_holder == ib_dev) ?
			"CLAIMED: IBLOCK" : "CLAIMED: OS");
	} else {
		bl += sprintf(b + bl, "Major: 0 Minor: 0\n");
>>>>>>> refs/remotes/origin/master
	}

	return bl;
}

<<<<<<< HEAD
static void iblock_bio_destructor(struct bio *bio)
{
	struct se_task *task = bio->bi_private;
<<<<<<< HEAD
	struct iblock_dev *ib_dev = task->se_dev->dev_ptr;
=======
	struct iblock_dev *ib_dev = task->task_se_cmd->se_dev->dev_ptr;
>>>>>>> refs/remotes/origin/cm-10.0

	bio_free(bio, ib_dev->ibd_bio_set);
}

<<<<<<< HEAD
static struct bio *iblock_get_bio(
	struct se_task *task,
	struct iblock_req *ib_req,
	struct iblock_dev *ib_dev,
	int *ret,
	sector_t lba,
	u32 sg_num)
{
	struct bio *bio;

	bio = bio_alloc_bioset(GFP_NOIO, sg_num, ib_dev->ibd_bio_set);
	if (!(bio)) {
		printk(KERN_ERR "Unable to allocate memory for bio\n");
		*ret = PYX_TRANSPORT_OUT_OF_MEMORY_RESOURCES;
		return NULL;
	}

	DEBUG_IBLOCK("Allocated bio: %p task_sg_num: %u using ibd_bio_set:"
		" %p\n", bio, task->task_sg_num, ib_dev->ibd_bio_set);
	DEBUG_IBLOCK("Allocated bio: %p task_size: %u\n", bio, task->task_size);

	bio->bi_bdev = ib_dev->ibd_bd;
	bio->bi_private = (void *) task;
	bio->bi_destructor = iblock_bio_destructor;
	bio->bi_end_io = &iblock_bio_done;
	bio->bi_sector = lba;
	atomic_inc(&ib_req->ib_bio_cnt);

	DEBUG_IBLOCK("Set bio->bi_sector: %llu\n", bio->bi_sector);
	DEBUG_IBLOCK("Set ib_req->ib_bio_cnt: %d\n",
			atomic_read(&ib_req->ib_bio_cnt));
	return bio;
}

static int iblock_map_task_SG(struct se_task *task)
{
	struct se_cmd *cmd = task->task_se_cmd;
	struct se_device *dev = SE_DEV(cmd);
	struct iblock_dev *ib_dev = task->se_dev->dev_ptr;
	struct iblock_req *ib_req = IBLOCK_REQ(task);
	struct bio *bio = NULL, *hbio = NULL, *tbio = NULL;
	struct scatterlist *sg;
	int ret = 0;
	u32 i, sg_num = task->task_sg_num;
	sector_t block_lba;
=======
static struct bio *
iblock_get_bio(struct se_task *task, sector_t lba, u32 sg_num)
{
	struct iblock_dev *ib_dev = task->task_se_cmd->se_dev->dev_ptr;
	struct iblock_req *ib_req = IBLOCK_REQ(task);
	struct bio *bio;

	/*
	 * Only allocate as many vector entries as the bio code allows us to,
	 * we'll loop later on until we have handled the whole request.
	 */
	if (sg_num > BIO_MAX_PAGES)
		sg_num = BIO_MAX_PAGES;

	bio = bio_alloc_bioset(GFP_NOIO, sg_num, ib_dev->ibd_bio_set);
	if (!bio) {
		pr_err("Unable to allocate memory for bio\n");
		return NULL;
	}

	pr_debug("Allocated bio: %p task_sg_nents: %u using ibd_bio_set:"
		" %p\n", bio, task->task_sg_nents, ib_dev->ibd_bio_set);
	pr_debug("Allocated bio: %p task_size: %u\n", bio, task->task_size);

	bio->bi_bdev = ib_dev->ibd_bd;
	bio->bi_private = task;
	bio->bi_destructor = iblock_bio_destructor;
	bio->bi_end_io = &iblock_bio_done;
	bio->bi_sector = lba;
	atomic_inc(&ib_req->pending);

	pr_debug("Set bio->bi_sector: %llu\n", (unsigned long long)bio->bi_sector);
	pr_debug("Set ib_req->pending: %d\n", atomic_read(&ib_req->pending));
	return bio;
}

static void iblock_submit_bios(struct bio_list *list, int rw)
{
	struct blk_plug plug;
	struct bio *bio;

	blk_start_plug(&plug);
	while ((bio = bio_list_pop(list)))
		submit_bio(rw, bio);
	blk_finish_plug(&plug);
}

static int iblock_do_task(struct se_task *task)
{
	struct se_cmd *cmd = task->task_se_cmd;
	struct se_device *dev = cmd->se_dev;
	struct iblock_req *ibr = IBLOCK_REQ(task);
	struct bio *bio;
	struct bio_list list;
	struct scatterlist *sg;
	u32 i, sg_num = task->task_sg_nents;
	sector_t block_lba;
	unsigned bio_cnt;
	int rw;

	if (task->task_data_direction == DMA_TO_DEVICE) {
		/*
		 * Force data to disk if we pretend to not have a volatile
		 * write cache, or the initiator set the Force Unit Access bit.
		 */
		if (dev->se_sub_dev->se_dev_attrib.emulate_write_cache == 0 ||
		    (dev->se_sub_dev->se_dev_attrib.emulate_fua_write > 0 &&
		     (cmd->se_cmd_flags & SCF_FUA)))
			rw = WRITE_FUA;
		else
			rw = WRITE;
=======
static sense_reason_t
iblock_execute_rw(struct se_cmd *cmd, struct scatterlist *sgl, u32 sgl_nents,
		  enum dma_data_direction data_direction)
{
	struct se_device *dev = cmd->se_dev;
	struct iblock_req *ibr;
	struct bio *bio;
	struct bio_list list;
	struct scatterlist *sg;
	u32 sg_num = sgl_nents;
	sector_t block_lba;
	unsigned bio_cnt;
	int rw = 0;
	int i;

	if (data_direction == DMA_TO_DEVICE) {
		struct iblock_dev *ib_dev = IBLOCK_DEV(dev);
		struct request_queue *q = bdev_get_queue(ib_dev->ibd_bd);
		/*
		 * Force writethrough using WRITE_FUA if a volatile write cache
		 * is not enabled, or if initiator set the Force Unit Access bit.
		 */
		if (q->flush_flags & REQ_FUA) {
			if (cmd->se_cmd_flags & SCF_FUA)
				rw = WRITE_FUA;
			else if (!(q->flush_flags & REQ_FLUSH))
				rw = WRITE_FUA;
			else
				rw = WRITE;
		} else {
			rw = WRITE;
		}
>>>>>>> refs/remotes/origin/master
	} else {
		rw = READ;
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	/*
	 * Do starting conversion up from non 512-byte blocksize with
	 * struct se_task SCSI blocksize into Linux/Block 512 units for BIO.
	 */
<<<<<<< HEAD
	if (DEV_ATTRIB(dev)->block_size == 4096)
		block_lba = (task->task_lba << 3);
	else if (DEV_ATTRIB(dev)->block_size == 2048)
		block_lba = (task->task_lba << 2);
	else if (DEV_ATTRIB(dev)->block_size == 1024)
		block_lba = (task->task_lba << 1);
	else if (DEV_ATTRIB(dev)->block_size == 512)
		block_lba = task->task_lba;
	else {
		printk(KERN_ERR "Unsupported SCSI -> BLOCK LBA conversion:"
				" %u\n", DEV_ATTRIB(dev)->block_size);
		return PYX_TRANSPORT_LU_COMM_FAILURE;
	}

	bio = iblock_get_bio(task, ib_req, ib_dev, &ret, block_lba, sg_num);
	if (!(bio))
		return ret;

	ib_req->ib_bio = bio;
	hbio = tbio = bio;
	/*
	 * Use fs/bio.c:bio_add_pages() to setup the bio_vec maplist
	 * from TCM struct se_mem -> task->task_sg -> struct scatterlist memory.
	 */
	for_each_sg(task->task_sg, sg, task->task_sg_num, i) {
		DEBUG_IBLOCK("task: %p bio: %p Calling bio_add_page(): page:"
			" %p len: %u offset: %u\n", task, bio, sg_page(sg),
				sg->length, sg->offset);
again:
		ret = bio_add_page(bio, sg_page(sg), sg->length, sg->offset);
		if (ret != sg->length) {

			DEBUG_IBLOCK("*** Set bio->bi_sector: %llu\n",
					bio->bi_sector);
			DEBUG_IBLOCK("** task->task_size: %u\n",
					task->task_size);
			DEBUG_IBLOCK("*** bio->bi_max_vecs: %u\n",
					bio->bi_max_vecs);
			DEBUG_IBLOCK("*** bio->bi_vcnt: %u\n",
					bio->bi_vcnt);

			bio = iblock_get_bio(task, ib_req, ib_dev, &ret,
						block_lba, sg_num);
			if (!(bio))
				goto fail;

			tbio = tbio->bi_next = bio;
			DEBUG_IBLOCK("-----------------> Added +1 bio: %p to"
				" list, Going to again\n", bio);
			goto again;
		}
		/* Always in 512 byte units for Linux/Block */
		block_lba += sg->length >> IBLOCK_LBA_SHIFT;
		sg_num--;
		DEBUG_IBLOCK("task: %p bio-add_page() passed!, decremented"
			" sg_num to %u\n", task, sg_num);
		DEBUG_IBLOCK("task: %p bio_add_page() passed!, increased lba"
				" to %llu\n", task, block_lba);
		DEBUG_IBLOCK("task: %p bio_add_page() passed!, bio->bi_vcnt:"
				" %u\n", task, bio->bi_vcnt);
	}

	return 0;
fail:
	while (hbio) {
		bio = hbio;
		hbio = hbio->bi_next;
		bio->bi_next = NULL;
		bio_put(bio);
	}
	return ret;
}

static unsigned char *iblock_get_cdb(struct se_task *task)
{
	return IBLOCK_REQ(task)->ib_scsi_cdb;
=======
	if (dev->se_sub_dev->se_dev_attrib.block_size == 4096)
		block_lba = (task->task_lba << 3);
	else if (dev->se_sub_dev->se_dev_attrib.block_size == 2048)
		block_lba = (task->task_lba << 2);
	else if (dev->se_sub_dev->se_dev_attrib.block_size == 1024)
		block_lba = (task->task_lba << 1);
	else if (dev->se_sub_dev->se_dev_attrib.block_size == 512)
		block_lba = task->task_lba;
	else {
		pr_err("Unsupported SCSI -> BLOCK LBA conversion:"
				" %u\n", dev->se_sub_dev->se_dev_attrib.block_size);
		cmd->scsi_sense_reason = TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
		return -ENOSYS;
	}

	bio = iblock_get_bio(task, block_lba, sg_num);
	if (!bio) {
		cmd->scsi_sense_reason = TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
		return -ENOMEM;
	}

	bio_list_init(&list);
	bio_list_add(&list, bio);
	bio_cnt = 1;

	for_each_sg(task->task_sg, sg, task->task_sg_nents, i) {
=======
	/*
	 * Convert the blocksize advertised to the initiator to the 512 byte
	 * units unconditionally used by the Linux block layer.
	 */
	if (dev->dev_attrib.block_size == 4096)
		block_lba = (cmd->t_task_lba << 3);
	else if (dev->dev_attrib.block_size == 2048)
		block_lba = (cmd->t_task_lba << 2);
	else if (dev->dev_attrib.block_size == 1024)
		block_lba = (cmd->t_task_lba << 1);
	else if (dev->dev_attrib.block_size == 512)
		block_lba = cmd->t_task_lba;
	else {
		pr_err("Unsupported SCSI -> BLOCK LBA conversion:"
				" %u\n", dev->dev_attrib.block_size);
		return TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
	}

	ibr = kzalloc(sizeof(struct iblock_req), GFP_KERNEL);
	if (!ibr)
		goto fail;
	cmd->priv = ibr;

	if (!sgl_nents) {
		atomic_set(&ibr->pending, 1);
		iblock_complete_cmd(cmd);
		return 0;
	}

	bio = iblock_get_bio(cmd, block_lba, sgl_nents);
	if (!bio)
		goto fail_free_ibr;

	bio_list_init(&list);
	bio_list_add(&list, bio);

	atomic_set(&ibr->pending, 2);
	bio_cnt = 1;

	for_each_sg(sgl, sg, sgl_nents, i) {
>>>>>>> refs/remotes/origin/master
		/*
		 * XXX: if the length the device accepts is shorter than the
		 *	length of the S/G list entry this will cause and
		 *	endless loop.  Better hope no driver uses huge pages.
		 */
		while (bio_add_page(bio, sg_page(sg), sg->length, sg->offset)
				!= sg->length) {
			if (bio_cnt >= IBLOCK_MAX_BIO_PER_TASK) {
				iblock_submit_bios(&list, rw);
				bio_cnt = 0;
			}

<<<<<<< HEAD
			bio = iblock_get_bio(task, block_lba, sg_num);
			if (!bio)
				goto fail;
=======
			bio = iblock_get_bio(cmd, block_lba, sg_num);
			if (!bio)
				goto fail_put_bios;

			atomic_inc(&ibr->pending);
>>>>>>> refs/remotes/origin/master
			bio_list_add(&list, bio);
			bio_cnt++;
		}

		/* Always in 512 byte units for Linux/Block */
		block_lba += sg->length >> IBLOCK_LBA_SHIFT;
		sg_num--;
	}

	iblock_submit_bios(&list, rw);
<<<<<<< HEAD

	if (atomic_dec_and_test(&ibr->pending)) {
		transport_complete_task(task,
				!atomic_read(&ibr->ib_bio_err_cnt));
	}
	return 0;

fail:
	while ((bio = bio_list_pop(&list)))
		bio_put(bio);
	cmd->scsi_sense_reason = TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
	return -ENOMEM;
>>>>>>> refs/remotes/origin/cm-10.0
}

static u32 iblock_get_device_rev(struct se_device *dev)
{
	return SCSI_SPC_2; /* Returns SPC-3 in Initiator Data */
}

static u32 iblock_get_device_type(struct se_device *dev)
{
	return TYPE_DISK;
}

static sector_t iblock_get_blocks(struct se_device *dev)
{
	struct iblock_dev *ibd = dev->dev_ptr;
	struct block_device *bd = ibd->ibd_bd;
	struct request_queue *q = bdev_get_queue(bd);

	return iblock_emulate_read_cap_with_block_size(dev, bd, q);
}

static void iblock_bio_done(struct bio *bio, int err)
{
	struct se_task *task = bio->bi_private;
	struct iblock_req *ibr = IBLOCK_REQ(task);
<<<<<<< HEAD
	/*
	 * Set -EIO if !BIO_UPTODATE and the passed is still err=0
	 */
	if (!(test_bit(BIO_UPTODATE, &bio->bi_flags)) && !(err))
		err = -EIO;

	if (err != 0) {
		printk(KERN_ERR "test_bit(BIO_UPTODATE) failed for bio: %p,"
=======

	/*
	 * Set -EIO if !BIO_UPTODATE and the passed is still err=0
	 */
	if (!test_bit(BIO_UPTODATE, &bio->bi_flags) && !err)
		err = -EIO;

	if (err != 0) {
		pr_err("test_bit(BIO_UPTODATE) failed for bio: %p,"
>>>>>>> refs/remotes/origin/cm-10.0
			" err: %d\n", bio, err);
		/*
		 * Bump the ib_bio_err_cnt and release bio.
		 */
		atomic_inc(&ibr->ib_bio_err_cnt);
		smp_mb__after_atomic_inc();
<<<<<<< HEAD
		bio_put(bio);
		/*
		 * Wait to complete the task until the last bio as completed.
		 */
		if (!(atomic_dec_and_test(&ibr->ib_bio_cnt)))
			return;

		ibr->ib_bio = NULL;
		transport_complete_task(task, 0);
		return;
	}
	DEBUG_IBLOCK("done[%p] bio: %p task_lba: %llu bio_lba: %llu err=%d\n",
		task, bio, task->task_lba, bio->bi_sector, err);
	/*
	 * bio_put() will call iblock_bio_destructor() to release the bio back
	 * to ibr->ib_bio_set.
	 */
	bio_put(bio);
	/*
	 * Wait to complete the task until the last bio as completed.
	 */
	if (!(atomic_dec_and_test(&ibr->ib_bio_cnt)))
		return;
	/*
	 * Return GOOD status for task if zero ib_bio_err_cnt exists.
	 */
	ibr->ib_bio = NULL;
	transport_complete_task(task, (!atomic_read(&ibr->ib_bio_err_cnt)));
=======
	}

	bio_put(bio);

	if (!atomic_dec_and_test(&ibr->pending))
		return;

	pr_debug("done[%p] bio: %p task_lba: %llu bio_lba: %llu err=%d\n",
		 task, bio, task->task_lba,
		 (unsigned long long)bio->bi_sector, err);

	transport_complete_task(task, !atomic_read(&ibr->ib_bio_err_cnt));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	iblock_complete_cmd(cmd);
	return 0;

fail_put_bios:
	while ((bio = bio_list_pop(&list)))
		bio_put(bio);
fail_free_ibr:
	kfree(ibr);
fail:
	return TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
}

static sector_t iblock_get_blocks(struct se_device *dev)
{
	struct iblock_dev *ib_dev = IBLOCK_DEV(dev);
	struct block_device *bd = ib_dev->ibd_bd;
	struct request_queue *q = bdev_get_queue(bd);

	return iblock_emulate_read_cap_with_block_size(dev, bd, q);
}

static sector_t iblock_get_alignment_offset_lbas(struct se_device *dev)
{
	struct iblock_dev *ib_dev = IBLOCK_DEV(dev);
	struct block_device *bd = ib_dev->ibd_bd;
	int ret;

	ret = bdev_alignment_offset(bd);
	if (ret == -1)
		return 0;

	/* convert offset-bytes to offset-lbas */
	return ret / bdev_logical_block_size(bd);
}

static unsigned int iblock_get_lbppbe(struct se_device *dev)
{
	struct iblock_dev *ib_dev = IBLOCK_DEV(dev);
	struct block_device *bd = ib_dev->ibd_bd;
	int logs_per_phys = bdev_physical_block_size(bd) / bdev_logical_block_size(bd);

	return ilog2(logs_per_phys);
}

static unsigned int iblock_get_io_min(struct se_device *dev)
{
	struct iblock_dev *ib_dev = IBLOCK_DEV(dev);
	struct block_device *bd = ib_dev->ibd_bd;

	return bdev_io_min(bd);
}

static unsigned int iblock_get_io_opt(struct se_device *dev)
{
	struct iblock_dev *ib_dev = IBLOCK_DEV(dev);
	struct block_device *bd = ib_dev->ibd_bd;

	return bdev_io_opt(bd);
}

static struct sbc_ops iblock_sbc_ops = {
	.execute_rw		= iblock_execute_rw,
	.execute_sync_cache	= iblock_execute_sync_cache,
	.execute_write_same	= iblock_execute_write_same,
	.execute_write_same_unmap = iblock_execute_write_same_unmap,
	.execute_unmap		= iblock_execute_unmap,
};

static sense_reason_t
iblock_parse_cdb(struct se_cmd *cmd)
{
	return sbc_parse_cdb(cmd, &iblock_sbc_ops);
}

bool iblock_get_write_cache(struct se_device *dev)
{
	struct iblock_dev *ib_dev = IBLOCK_DEV(dev);
	struct block_device *bd = ib_dev->ibd_bd;
	struct request_queue *q = bdev_get_queue(bd);

	return q->flush_flags & REQ_FLUSH;
>>>>>>> refs/remotes/origin/master
}

static struct se_subsystem_api iblock_template = {
	.name			= "iblock",
<<<<<<< HEAD
	.owner			= THIS_MODULE,
	.transport_type		= TRANSPORT_PLUGIN_VHBA_PDEV,
<<<<<<< HEAD
	.map_task_SG		= iblock_map_task_SG,
=======
	.write_cache_emulated	= 1,
	.fua_write_emulated	= 1,
>>>>>>> refs/remotes/origin/cm-10.0
	.attach_hba		= iblock_attach_hba,
	.detach_hba		= iblock_detach_hba,
	.allocate_virtdevice	= iblock_allocate_virtdevice,
	.create_virtdevice	= iblock_create_virtdevice,
	.free_device		= iblock_free_device,
<<<<<<< HEAD
	.dpo_emulated		= iblock_emulated_dpo,
	.fua_write_emulated	= iblock_emulated_fua_write,
	.fua_read_emulated	= iblock_emulated_fua_read,
	.write_cache_emulated	= iblock_emulated_write_cache,
=======
>>>>>>> refs/remotes/origin/cm-10.0
	.alloc_task		= iblock_alloc_task,
	.do_task		= iblock_do_task,
	.do_discard		= iblock_do_discard,
	.do_sync_cache		= iblock_emulate_sync_cache,
	.free_task		= iblock_free_task,
	.check_configfs_dev_params = iblock_check_configfs_dev_params,
	.set_configfs_dev_params = iblock_set_configfs_dev_params,
	.show_configfs_dev_params = iblock_show_configfs_dev_params,
<<<<<<< HEAD
	.get_cdb		= iblock_get_cdb,
=======
>>>>>>> refs/remotes/origin/cm-10.0
	.get_device_rev		= iblock_get_device_rev,
	.get_device_type	= iblock_get_device_type,
	.get_blocks		= iblock_get_blocks,
=======
	.inquiry_prod		= "IBLOCK",
	.inquiry_rev		= IBLOCK_VERSION,
	.owner			= THIS_MODULE,
	.transport_type		= TRANSPORT_PLUGIN_VHBA_PDEV,
	.attach_hba		= iblock_attach_hba,
	.detach_hba		= iblock_detach_hba,
	.alloc_device		= iblock_alloc_device,
	.configure_device	= iblock_configure_device,
	.free_device		= iblock_free_device,
	.parse_cdb		= iblock_parse_cdb,
	.set_configfs_dev_params = iblock_set_configfs_dev_params,
	.show_configfs_dev_params = iblock_show_configfs_dev_params,
	.get_device_type	= sbc_get_device_type,
	.get_blocks		= iblock_get_blocks,
	.get_alignment_offset_lbas = iblock_get_alignment_offset_lbas,
	.get_lbppbe		= iblock_get_lbppbe,
	.get_io_min		= iblock_get_io_min,
	.get_io_opt		= iblock_get_io_opt,
	.get_write_cache	= iblock_get_write_cache,
>>>>>>> refs/remotes/origin/master
};

static int __init iblock_module_init(void)
{
	return transport_subsystem_register(&iblock_template);
}

<<<<<<< HEAD
static void iblock_module_exit(void)
=======
static void __exit iblock_module_exit(void)
>>>>>>> refs/remotes/origin/master
{
	transport_subsystem_release(&iblock_template);
}

MODULE_DESCRIPTION("TCM IBLOCK subsystem plugin");
MODULE_AUTHOR("nab@Linux-iSCSI.org");
MODULE_LICENSE("GPL");

module_init(iblock_module_init);
module_exit(iblock_module_exit);
