/*******************************************************************************
 * Filename:  target_core_file.c
 *
 * This file contains the Storage Engine <-> FILEIO transport specific functions
 *
<<<<<<< HEAD
 * Copyright (c) 2005 PyX Technologies, Inc.
 * Copyright (c) 2005-2006 SBE, Inc.  All Rights Reserved.
 * Copyright (c) 2007-2010 Rising Tide Systems
 * Copyright (c) 2008-2010 Linux-iSCSI.org
=======
 * (c) Copyright 2005-2013 Datera, Inc.
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
#include <linux/slab.h>
#include <linux/spinlock.h>
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

#include "target_core_file.h"

#if 1
#define DEBUG_FD_CACHE(x...) printk(x)
#else
#define DEBUG_FD_CACHE(x...)
#endif

#if 1
#define DEBUG_FD_FUA(x...) printk(x)
#else
#define DEBUG_FD_FUA(x...)
#endif

=======
=======
#include <linux/module.h>
#include <linux/falloc.h>
#include <scsi/scsi.h>
#include <scsi/scsi_host.h>
#include <asm/unaligned.h>

#include <target/target_core_base.h>
>>>>>>> refs/remotes/origin/master
#include <target/target_core_backend.h>

#include "target_core_file.h"

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
static struct se_subsystem_api fileio_template;
=======
static inline struct fd_dev *FD_DEV(struct se_device *dev)
{
	return container_of(dev, struct fd_dev, dev);
}
>>>>>>> refs/remotes/origin/master

/*	fd_attach_hba(): (Part of se_subsystem_api_t template)
 *
 *
 */
static int fd_attach_hba(struct se_hba *hba, u32 host_id)
{
	struct fd_host *fd_host;

	fd_host = kzalloc(sizeof(struct fd_host), GFP_KERNEL);
<<<<<<< HEAD
<<<<<<< HEAD
	if (!(fd_host)) {
		printk(KERN_ERR "Unable to allocate memory for struct fd_host\n");
		return -1;
=======
	if (!fd_host) {
		pr_err("Unable to allocate memory for struct fd_host\n");
		return -ENOMEM;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!fd_host) {
		pr_err("Unable to allocate memory for struct fd_host\n");
		return -ENOMEM;
>>>>>>> refs/remotes/origin/master
	}

	fd_host->fd_host_id = host_id;

<<<<<<< HEAD
<<<<<<< HEAD
	atomic_set(&hba->left_queue_depth, FD_HBA_QUEUE_DEPTH);
	atomic_set(&hba->max_queue_depth, FD_HBA_QUEUE_DEPTH);
	hba->hba_ptr = (void *) fd_host;

	printk(KERN_INFO "CORE_HBA[%d] - TCM FILEIO HBA Driver %s on Generic"
		" Target Core Stack %s\n", hba->hba_id, FD_VERSION,
		TARGET_CORE_MOD_VERSION);
	printk(KERN_INFO "CORE_HBA[%d] - Attached FILEIO HBA: %u to Generic"
		" Target Core with TCQ Depth: %d MaxSectors: %u\n",
		hba->hba_id, fd_host->fd_host_id,
		atomic_read(&hba->max_queue_depth), FD_MAX_SECTORS);
=======
=======
>>>>>>> refs/remotes/origin/master
	hba->hba_ptr = fd_host;

	pr_debug("CORE_HBA[%d] - TCM FILEIO HBA Driver %s on Generic"
		" Target Core Stack %s\n", hba->hba_id, FD_VERSION,
		TARGET_CORE_MOD_VERSION);
<<<<<<< HEAD
	pr_debug("CORE_HBA[%d] - Attached FILEIO HBA: %u to Generic"
		" MaxSectors: %u\n",
		hba->hba_id, fd_host->fd_host_id, FD_MAX_SECTORS);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pr_debug("CORE_HBA[%d] - Attached FILEIO HBA: %u to Generic\n",
		hba->hba_id, fd_host->fd_host_id);
>>>>>>> refs/remotes/origin/master

	return 0;
}

static void fd_detach_hba(struct se_hba *hba)
{
	struct fd_host *fd_host = hba->hba_ptr;

<<<<<<< HEAD
<<<<<<< HEAD
	printk(KERN_INFO "CORE_HBA[%d] - Detached FILEIO HBA: %u from Generic"
=======
	pr_debug("CORE_HBA[%d] - Detached FILEIO HBA: %u from Generic"
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pr_debug("CORE_HBA[%d] - Detached FILEIO HBA: %u from Generic"
>>>>>>> refs/remotes/origin/master
		" Target Core\n", hba->hba_id, fd_host->fd_host_id);

	kfree(fd_host);
	hba->hba_ptr = NULL;
}

<<<<<<< HEAD
static void *fd_allocate_virtdevice(struct se_hba *hba, const char *name)
{
	struct fd_dev *fd_dev;
<<<<<<< HEAD
	struct fd_host *fd_host = (struct fd_host *) hba->hba_ptr;

	fd_dev = kzalloc(sizeof(struct fd_dev), GFP_KERNEL);
	if (!(fd_dev)) {
		printk(KERN_ERR "Unable to allocate memory for struct fd_dev\n");
=======
=======
static struct se_device *fd_alloc_device(struct se_hba *hba, const char *name)
{
	struct fd_dev *fd_dev;
>>>>>>> refs/remotes/origin/master
	struct fd_host *fd_host = hba->hba_ptr;

	fd_dev = kzalloc(sizeof(struct fd_dev), GFP_KERNEL);
	if (!fd_dev) {
		pr_err("Unable to allocate memory for struct fd_dev\n");
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		return NULL;
	}

	fd_dev->fd_host = fd_host;

<<<<<<< HEAD
<<<<<<< HEAD
	printk(KERN_INFO "FILEIO: Allocated fd_dev for %p\n", name);
=======
	pr_debug("FILEIO: Allocated fd_dev for %p\n", name);
>>>>>>> refs/remotes/origin/cm-10.0

	return fd_dev;
}

/*	fd_create_virtdevice(): (Part of se_subsystem_api_t template)
 *
 *
 */
static struct se_device *fd_create_virtdevice(
	struct se_hba *hba,
	struct se_subsystem_dev *se_dev,
	void *p)
{
	char *dev_p = NULL;
	struct se_device *dev;
	struct se_dev_limits dev_limits;
	struct queue_limits *limits;
<<<<<<< HEAD
	struct fd_dev *fd_dev = (struct fd_dev *) p;
	struct fd_host *fd_host = (struct fd_host *) hba->hba_ptr;
=======
	struct fd_dev *fd_dev = p;
	struct fd_host *fd_host = hba->hba_ptr;
>>>>>>> refs/remotes/origin/cm-10.0
	mm_segment_t old_fs;
	struct file *file;
	struct inode *inode = NULL;
	int dev_flags = 0, flags, ret = -EINVAL;

	memset(&dev_limits, 0, sizeof(struct se_dev_limits));

	old_fs = get_fs();
	set_fs(get_ds());
	dev_p = getname(fd_dev->fd_dev_name);
	set_fs(old_fs);

	if (IS_ERR(dev_p)) {
<<<<<<< HEAD
		printk(KERN_ERR "getname(%s) failed: %lu\n",
=======
		pr_err("getname(%s) failed: %lu\n",
>>>>>>> refs/remotes/origin/cm-10.0
			fd_dev->fd_dev_name, IS_ERR(dev_p));
		ret = PTR_ERR(dev_p);
		goto fail;
	}
#if 0
	if (di->no_create_file)
		flags = O_RDWR | O_LARGEFILE;
	else
		flags = O_RDWR | O_CREAT | O_LARGEFILE;
#else
	flags = O_RDWR | O_CREAT | O_LARGEFILE;
#endif
/*	flags |= O_DIRECT; */
	/*
	 * If fd_buffered_io=1 has not been set explicitly (the default),
	 * use O_SYNC to force FILEIO writes to disk.
	 */
	if (!(fd_dev->fbd_flags & FDBD_USE_BUFFERED_IO))
		flags |= O_SYNC;

	file = filp_open(dev_p, flags, 0600);
	if (IS_ERR(file)) {
<<<<<<< HEAD
		printk(KERN_ERR "filp_open(%s) failed\n", dev_p);
=======
		pr_err("filp_open(%s) failed\n", dev_p);
>>>>>>> refs/remotes/origin/cm-10.0
		ret = PTR_ERR(file);
		goto fail;
	}
	if (!file || !file->f_dentry) {
<<<<<<< HEAD
		printk(KERN_ERR "filp_open(%s) failed\n", dev_p);
=======
		pr_err("filp_open(%s) failed\n", dev_p);
>>>>>>> refs/remotes/origin/cm-10.0
		goto fail;
	}
=======
	pr_debug("FILEIO: Allocated fd_dev for %p\n", name);

	return &fd_dev->dev;
}

static int fd_configure_device(struct se_device *dev)
{
	struct fd_dev *fd_dev = FD_DEV(dev);
	struct fd_host *fd_host = dev->se_hba->hba_ptr;
	struct file *file;
	struct inode *inode = NULL;
	int flags, ret = -EINVAL;

	if (!(fd_dev->fbd_flags & FBDF_HAS_PATH)) {
		pr_err("Missing fd_dev_name=\n");
		return -EINVAL;
	}

	/*
	 * Use O_DSYNC by default instead of O_SYNC to forgo syncing
	 * of pure timestamp updates.
	 */
	flags = O_RDWR | O_CREAT | O_LARGEFILE | O_DSYNC;

	/*
	 * Optionally allow fd_buffered_io=1 to be enabled for people
	 * who want use the fs buffer cache as an WriteCache mechanism.
	 *
	 * This means that in event of a hard failure, there is a risk
	 * of silent data-loss if the SCSI client has *not* performed a
	 * forced unit access (FUA) write, or issued SYNCHRONIZE_CACHE
	 * to write-out the entire device cache.
	 */
	if (fd_dev->fbd_flags & FDBD_HAS_BUFFERED_IO_WCE) {
		pr_debug("FILEIO: Disabling O_DSYNC, using buffered FILEIO\n");
		flags &= ~O_DSYNC;
	}

	file = filp_open(fd_dev->fd_dev_name, flags, 0600);
	if (IS_ERR(file)) {
		pr_err("filp_open(%s) failed\n", fd_dev->fd_dev_name);
		ret = PTR_ERR(file);
		goto fail;
	}
>>>>>>> refs/remotes/origin/master
	fd_dev->fd_file = file;
	/*
	 * If using a block backend with this struct file, we extract
	 * fd_dev->fd_[block,dev]_size from struct block_device.
	 *
	 * Otherwise, we use the passed fd_size= from configfs
	 */
	inode = file->f_mapping->host;
	if (S_ISBLK(inode->i_mode)) {
<<<<<<< HEAD
		struct request_queue *q;
<<<<<<< HEAD
=======
		unsigned long long dev_size;
>>>>>>> refs/remotes/origin/cm-10.0
		/*
		 * Setup the local scope queue_limits from struct request_queue->limits
		 * to pass into transport_add_device_to_core_hba() as struct se_dev_limits.
		 */
		q = bdev_get_queue(inode->i_bdev);
		limits = &dev_limits.limits;
		limits->logical_block_size = bdev_logical_block_size(inode->i_bdev);
		limits->max_hw_sectors = queue_max_hw_sectors(q);
		limits->max_sectors = queue_max_sectors(q);
=======
		struct request_queue *q = bdev_get_queue(inode->i_bdev);
		unsigned long long dev_size;

		fd_dev->fd_block_size = bdev_logical_block_size(inode->i_bdev);
>>>>>>> refs/remotes/origin/master
		/*
		 * Determine the number of bytes from i_size_read() minus
		 * one (1) logical sector from underlying struct block_device
		 */
<<<<<<< HEAD
		fd_dev->fd_block_size = bdev_logical_block_size(inode->i_bdev);
<<<<<<< HEAD
		fd_dev->fd_dev_size = (i_size_read(file->f_mapping->host) -
				       fd_dev->fd_block_size);

		printk(KERN_INFO "FILEIO: Using size: %llu bytes from struct"
			" block_device blocks: %llu logical_block_size: %d\n",
			fd_dev->fd_dev_size,
			div_u64(fd_dev->fd_dev_size, fd_dev->fd_block_size),
			fd_dev->fd_block_size);
	} else {
		if (!(fd_dev->fbd_flags & FBDF_HAS_SIZE)) {
			printk(KERN_ERR "FILEIO: Missing fd_dev_size="
=======
=======
>>>>>>> refs/remotes/origin/master
		dev_size = (i_size_read(file->f_mapping->host) -
				       fd_dev->fd_block_size);

		pr_debug("FILEIO: Using size: %llu bytes from struct"
			" block_device blocks: %llu logical_block_size: %d\n",
			dev_size, div_u64(dev_size, fd_dev->fd_block_size),
			fd_dev->fd_block_size);
<<<<<<< HEAD
	} else {
		if (!(fd_dev->fbd_flags & FBDF_HAS_SIZE)) {
			pr_err("FILEIO: Missing fd_dev_size="
>>>>>>> refs/remotes/origin/cm-10.0
=======
		/*
		 * Check if the underlying struct block_device request_queue supports
		 * the QUEUE_FLAG_DISCARD bit for UNMAP/WRITE_SAME in SCSI + TRIM
		 * in ATA and we need to set TPE=1
		 */
		if (blk_queue_discard(q)) {
			dev->dev_attrib.max_unmap_lba_count =
				q->limits.max_discard_sectors;
			/*
			 * Currently hardcoded to 1 in Linux/SCSI code..
			 */
			dev->dev_attrib.max_unmap_block_desc_count = 1;
			dev->dev_attrib.unmap_granularity =
				q->limits.discard_granularity >> 9;
			dev->dev_attrib.unmap_granularity_alignment =
				q->limits.discard_alignment;
			pr_debug("IFILE: BLOCK Discard support available,"
					" disabled by default\n");
		}
		/*
		 * Enable write same emulation for IBLOCK and use 0xFFFF as
		 * the smaller WRITE_SAME(10) only has a two-byte block count.
		 */
		dev->dev_attrib.max_write_same_len = 0xFFFF;

		if (blk_queue_nonrot(q))
			dev->dev_attrib.is_nonrot = 1;
	} else {
		if (!(fd_dev->fbd_flags & FBDF_HAS_SIZE)) {
			pr_err("FILEIO: Missing fd_dev_size="
>>>>>>> refs/remotes/origin/master
				" parameter, and no backing struct"
				" block_device\n");
			goto fail;
		}

<<<<<<< HEAD
		limits = &dev_limits.limits;
		limits->logical_block_size = FD_BLOCKSIZE;
		limits->max_hw_sectors = FD_MAX_SECTORS;
		limits->max_sectors = FD_MAX_SECTORS;
		fd_dev->fd_block_size = FD_BLOCKSIZE;
	}

	dev_limits.hw_queue_depth = FD_MAX_DEVICE_QUEUE_DEPTH;
	dev_limits.queue_depth = FD_DEVICE_QUEUE_DEPTH;

	dev = transport_add_device_to_core_hba(hba, &fileio_template,
<<<<<<< HEAD
				se_dev, dev_flags, (void *)fd_dev,
				&dev_limits, "FILEIO", FD_VERSION);
	if (!(dev))
=======
				se_dev, dev_flags, fd_dev,
				&dev_limits, "FILEIO", FD_VERSION);
	if (!dev)
>>>>>>> refs/remotes/origin/cm-10.0
		goto fail;
=======
		fd_dev->fd_block_size = FD_BLOCKSIZE;
		/*
		 * Limit UNMAP emulation to 8k Number of LBAs (NoLB)
		 */
		dev->dev_attrib.max_unmap_lba_count = 0x2000;
		/*
		 * Currently hardcoded to 1 in Linux/SCSI code..
		 */
		dev->dev_attrib.max_unmap_block_desc_count = 1;
		dev->dev_attrib.unmap_granularity = 1;
		dev->dev_attrib.unmap_granularity_alignment = 0;

		/*
		 * Limit WRITE_SAME w/ UNMAP=0 emulation to 8k Number of LBAs (NoLB)
		 * based upon struct iovec limit for vfs_writev()
		 */
		dev->dev_attrib.max_write_same_len = 0x1000;
	}

	dev->dev_attrib.hw_block_size = fd_dev->fd_block_size;
	dev->dev_attrib.max_bytes_per_io = FD_MAX_BYTES;
	dev->dev_attrib.hw_max_sectors = FD_MAX_BYTES / fd_dev->fd_block_size;
	dev->dev_attrib.hw_queue_depth = FD_MAX_DEVICE_QUEUE_DEPTH;

	if (fd_dev->fbd_flags & FDBD_HAS_BUFFERED_IO_WCE) {
		pr_debug("FILEIO: Forcing setting of emulate_write_cache=1"
			" with FDBD_HAS_BUFFERED_IO_WCE\n");
		dev->dev_attrib.emulate_write_cache = 1;
	}
>>>>>>> refs/remotes/origin/master

	fd_dev->fd_dev_id = fd_host->fd_host_dev_id_count++;
	fd_dev->fd_queue_depth = dev->queue_depth;

<<<<<<< HEAD
<<<<<<< HEAD
	printk(KERN_INFO "CORE_FILE[%u] - Added TCM FILEIO Device ID: %u at %s,"
=======
	pr_debug("CORE_FILE[%u] - Added TCM FILEIO Device ID: %u at %s,"
>>>>>>> refs/remotes/origin/cm-10.0
		" %llu total bytes\n", fd_host->fd_host_id, fd_dev->fd_dev_id,
			fd_dev->fd_dev_name, fd_dev->fd_dev_size);

	putname(dev_p);
	return dev;
=======
	pr_debug("CORE_FILE[%u] - Added TCM FILEIO Device ID: %u at %s,"
		" %llu total bytes\n", fd_host->fd_host_id, fd_dev->fd_dev_id,
			fd_dev->fd_dev_name, fd_dev->fd_dev_size);

	return 0;
>>>>>>> refs/remotes/origin/master
fail:
	if (fd_dev->fd_file) {
		filp_close(fd_dev->fd_file, NULL);
		fd_dev->fd_file = NULL;
	}
<<<<<<< HEAD
	putname(dev_p);
	return ERR_PTR(ret);
}

/*	fd_free_device(): (Part of se_subsystem_api_t template)
 *
 *
 */
static void fd_free_device(void *p)
{
<<<<<<< HEAD
	struct fd_dev *fd_dev = (struct fd_dev *) p;
=======
	struct fd_dev *fd_dev = p;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return ret;
}

static void fd_free_device(struct se_device *dev)
{
	struct fd_dev *fd_dev = FD_DEV(dev);
>>>>>>> refs/remotes/origin/master

	if (fd_dev->fd_file) {
		filp_close(fd_dev->fd_file, NULL);
		fd_dev->fd_file = NULL;
	}

	kfree(fd_dev);
}

<<<<<<< HEAD
static inline struct fd_request *FILE_REQ(struct se_task *task)
{
	return container_of(task, struct fd_request, fd_task);
}


static struct se_task *
<<<<<<< HEAD
fd_alloc_task(struct se_cmd *cmd)
=======
fd_alloc_task(unsigned char *cdb)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct fd_request *fd_req;

	fd_req = kzalloc(sizeof(struct fd_request), GFP_KERNEL);
<<<<<<< HEAD
	if (!(fd_req)) {
		printk(KERN_ERR "Unable to allocate struct fd_request\n");
		return NULL;
	}

	fd_req->fd_dev = SE_DEV(cmd)->dev_ptr;

=======
	if (!fd_req) {
		pr_err("Unable to allocate struct fd_request\n");
		return NULL;
	}

>>>>>>> refs/remotes/origin/cm-10.0
	return &fd_req->fd_task;
}

static int fd_do_readv(struct se_task *task)
{
	struct fd_request *req = FILE_REQ(task);
<<<<<<< HEAD
	struct file *fd = req->fd_dev->fd_file;
	struct scatterlist *sg = task->task_sg;
	struct iovec *iov;
	mm_segment_t old_fs;
	loff_t pos = (task->task_lba * DEV_ATTRIB(task->se_dev)->block_size);
	int ret = 0, i;

	iov = kzalloc(sizeof(struct iovec) * task->task_sg_num, GFP_KERNEL);
	if (!(iov)) {
		printk(KERN_ERR "Unable to allocate fd_do_readv iov[]\n");
		return -1;
	}

	for (i = 0; i < task->task_sg_num; i++) {
		iov[i].iov_len = sg[i].length;
		iov[i].iov_base = sg_virt(&sg[i]);
=======
	struct se_device *se_dev = req->fd_task.task_se_cmd->se_dev;
	struct fd_dev *dev = se_dev->dev_ptr;
	struct file *fd = dev->fd_file;
	struct scatterlist *sg = task->task_sg;
	struct iovec *iov;
	mm_segment_t old_fs;
	loff_t pos = (task->task_lba *
		      se_dev->se_sub_dev->se_dev_attrib.block_size);
	int ret = 0, i;

	iov = kzalloc(sizeof(struct iovec) * task->task_sg_nents, GFP_KERNEL);
=======
static int fd_do_rw(struct se_cmd *cmd, struct scatterlist *sgl,
		u32 sgl_nents, int is_write)
{
	struct se_device *se_dev = cmd->se_dev;
	struct fd_dev *dev = FD_DEV(se_dev);
	struct file *fd = dev->fd_file;
	struct scatterlist *sg;
	struct iovec *iov;
	mm_segment_t old_fs;
	loff_t pos = (cmd->t_task_lba * se_dev->dev_attrib.block_size);
	int ret = 0, i;

	iov = kzalloc(sizeof(struct iovec) * sgl_nents, GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (!iov) {
		pr_err("Unable to allocate fd_do_readv iov[]\n");
		return -ENOMEM;
	}

<<<<<<< HEAD
	for_each_sg(task->task_sg, sg, task->task_sg_nents, i) {
		iov[i].iov_len = sg->length;
		iov[i].iov_base = sg_virt(sg);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	for_each_sg(sgl, sg, sgl_nents, i) {
		iov[i].iov_len = sg->length;
		iov[i].iov_base = kmap(sg_page(sg)) + sg->offset;
>>>>>>> refs/remotes/origin/master
	}

	old_fs = get_fs();
	set_fs(get_ds());
<<<<<<< HEAD
<<<<<<< HEAD
	ret = vfs_readv(fd, &iov[0], task->task_sg_num, &pos);
=======
	ret = vfs_readv(fd, &iov[0], task->task_sg_nents, &pos);
>>>>>>> refs/remotes/origin/cm-10.0
	set_fs(old_fs);

	kfree(iov);
	/*
	 * Return zeros and GOOD status even if the READ did not return
	 * the expected virt_size for struct file w/o a backing struct
	 * block_device.
	 */
	if (S_ISBLK(fd->f_dentry->d_inode->i_mode)) {
		if (ret < 0 || ret != task->task_size) {
<<<<<<< HEAD
			printk(KERN_ERR "vfs_readv() returned %d,"
				" expecting %d for S_ISBLK\n", ret,
				(int)task->task_size);
			return -1;
		}
	} else {
		if (ret < 0) {
			printk(KERN_ERR "vfs_readv() returned %d for non"
				" S_ISBLK\n", ret);
			return -1;
=======
			pr_err("vfs_readv() returned %d,"
				" expecting %d for S_ISBLK\n", ret,
				(int)task->task_size);
			return (ret < 0 ? ret : -EINVAL);
		}
	} else {
		if (ret < 0) {
			pr_err("vfs_readv() returned %d for non"
				" S_ISBLK\n", ret);
			return ret;
>>>>>>> refs/remotes/origin/cm-10.0
		}
	}

	return 1;
}

static int fd_do_writev(struct se_task *task)
{
	struct fd_request *req = FILE_REQ(task);
<<<<<<< HEAD
	struct file *fd = req->fd_dev->fd_file;
	struct scatterlist *sg = task->task_sg;
	struct iovec *iov;
	mm_segment_t old_fs;
	loff_t pos = (task->task_lba * DEV_ATTRIB(task->se_dev)->block_size);
	int ret, i = 0;

	iov = kzalloc(sizeof(struct iovec) * task->task_sg_num, GFP_KERNEL);
	if (!(iov)) {
		printk(KERN_ERR "Unable to allocate fd_do_writev iov[]\n");
		return -1;
	}

	for (i = 0; i < task->task_sg_num; i++) {
		iov[i].iov_len = sg[i].length;
		iov[i].iov_base = sg_virt(&sg[i]);
=======
	struct se_device *se_dev = req->fd_task.task_se_cmd->se_dev;
	struct fd_dev *dev = se_dev->dev_ptr;
	struct file *fd = dev->fd_file;
	struct scatterlist *sg = task->task_sg;
	struct iovec *iov;
	mm_segment_t old_fs;
	loff_t pos = (task->task_lba *
		      se_dev->se_sub_dev->se_dev_attrib.block_size);
	int ret, i = 0;

	iov = kzalloc(sizeof(struct iovec) * task->task_sg_nents, GFP_KERNEL);
	if (!iov) {
		pr_err("Unable to allocate fd_do_writev iov[]\n");
		return -ENOMEM;
	}

	for_each_sg(task->task_sg, sg, task->task_sg_nents, i) {
		iov[i].iov_len = sg->length;
		iov[i].iov_base = sg_virt(sg);
>>>>>>> refs/remotes/origin/cm-10.0
	}

	old_fs = get_fs();
	set_fs(get_ds());
<<<<<<< HEAD
	ret = vfs_writev(fd, &iov[0], task->task_sg_num, &pos);
=======
	ret = vfs_writev(fd, &iov[0], task->task_sg_nents, &pos);
>>>>>>> refs/remotes/origin/cm-10.0
	set_fs(old_fs);

	kfree(iov);

	if (ret < 0 || ret != task->task_size) {
<<<<<<< HEAD
		printk(KERN_ERR "vfs_writev() returned %d\n", ret);
		return -1;
=======
		pr_err("vfs_writev() returned %d\n", ret);
		return (ret < 0 ? ret : -EINVAL);
>>>>>>> refs/remotes/origin/cm-10.0
	}

	return 1;
}

static void fd_emulate_sync_cache(struct se_task *task)
{
<<<<<<< HEAD
	struct se_cmd *cmd = TASK_CMD(task);
	struct se_device *dev = cmd->se_dev;
	struct fd_dev *fd_dev = dev->dev_ptr;
	int immed = (cmd->t_task->t_task_cdb[1] & 0x2);
=======
	struct se_cmd *cmd = task->task_se_cmd;
	struct se_device *dev = cmd->se_dev;
	struct fd_dev *fd_dev = dev->dev_ptr;
	int immed = (cmd->t_task_cdb[1] & 0x2);
>>>>>>> refs/remotes/origin/cm-10.0
=======

	if (is_write)
		ret = vfs_writev(fd, &iov[0], sgl_nents, &pos);
	else
		ret = vfs_readv(fd, &iov[0], sgl_nents, &pos);

	set_fs(old_fs);

	for_each_sg(sgl, sg, sgl_nents, i)
		kunmap(sg_page(sg));

	kfree(iov);

	if (is_write) {
		if (ret < 0 || ret != cmd->data_length) {
			pr_err("%s() write returned %d\n", __func__, ret);
			return (ret < 0 ? ret : -EINVAL);
		}
	} else {
		/*
		 * Return zeros and GOOD status even if the READ did not return
		 * the expected virt_size for struct file w/o a backing struct
		 * block_device.
		 */
		if (S_ISBLK(file_inode(fd)->i_mode)) {
			if (ret < 0 || ret != cmd->data_length) {
				pr_err("%s() returned %d, expecting %u for "
						"S_ISBLK\n", __func__, ret,
						cmd->data_length);
				return (ret < 0 ? ret : -EINVAL);
			}
		} else {
			if (ret < 0) {
				pr_err("%s() returned %d for non S_ISBLK\n",
						__func__, ret);
				return ret;
			}
		}
	}
	return 1;
}

static sense_reason_t
fd_execute_sync_cache(struct se_cmd *cmd)
{
	struct se_device *dev = cmd->se_dev;
	struct fd_dev *fd_dev = FD_DEV(dev);
	int immed = (cmd->t_task_cdb[1] & 0x2);
>>>>>>> refs/remotes/origin/master
	loff_t start, end;
	int ret;

	/*
	 * If the Immediate bit is set, queue up the GOOD response
	 * for this SYNCHRONIZE_CACHE op
	 */
	if (immed)
<<<<<<< HEAD
		transport_complete_sync_cache(cmd, 1);
=======
		target_complete_cmd(cmd, SAM_STAT_GOOD);
>>>>>>> refs/remotes/origin/master

	/*
	 * Determine if we will be flushing the entire device.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (cmd->t_task->t_task_lba == 0 && cmd->data_length == 0) {
		start = 0;
		end = LLONG_MAX;
	} else {
		start = cmd->t_task->t_task_lba * DEV_ATTRIB(dev)->block_size;
=======
=======
>>>>>>> refs/remotes/origin/master
	if (cmd->t_task_lba == 0 && cmd->data_length == 0) {
		start = 0;
		end = LLONG_MAX;
	} else {
<<<<<<< HEAD
		start = cmd->t_task_lba * dev->se_sub_dev->se_dev_attrib.block_size;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		start = cmd->t_task_lba * dev->dev_attrib.block_size;
>>>>>>> refs/remotes/origin/master
		if (cmd->data_length)
			end = start + cmd->data_length;
		else
			end = LLONG_MAX;
	}

	ret = vfs_fsync_range(fd_dev->fd_file, start, end, 1);
	if (ret != 0)
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_ERR "FILEIO: vfs_fsync_range() failed: %d\n", ret);
=======
		pr_err("FILEIO: vfs_fsync_range() failed: %d\n", ret);
>>>>>>> refs/remotes/origin/cm-10.0

	if (!immed)
		transport_complete_sync_cache(cmd, ret == 0);
}

/*
<<<<<<< HEAD
 * Tell TCM Core that we are capable of WriteCache emulation for
 * an underlying struct se_device.
 */
static int fd_emulated_write_cache(struct se_device *dev)
{
	return 1;
}

static int fd_emulated_dpo(struct se_device *dev)
{
	return 0;
}
/*
 * Tell TCM Core that we will be emulating Forced Unit Access (FUA) for WRITEs
 * for TYPE_DISK.
 */
static int fd_emulated_fua_write(struct se_device *dev)
{
	return 1;
}

static int fd_emulated_fua_read(struct se_device *dev)
{
	return 0;
}

/*
=======
>>>>>>> refs/remotes/origin/cm-10.0
 * WRITE Force Unit Access (FUA) emulation on a per struct se_task
 * LBA range basis..
 */
static void fd_emulate_write_fua(struct se_cmd *cmd, struct se_task *task)
{
	struct se_device *dev = cmd->se_dev;
	struct fd_dev *fd_dev = dev->dev_ptr;
<<<<<<< HEAD
	loff_t start = task->task_lba * DEV_ATTRIB(dev)->block_size;
	loff_t end = start + task->task_size;
	int ret;

	DEBUG_FD_CACHE("FILEIO: FUA WRITE LBA: %llu, bytes: %u\n",
=======
	loff_t start = task->task_lba * dev->se_sub_dev->se_dev_attrib.block_size;
	loff_t end = start + task->task_size;
	int ret;

	pr_debug("FILEIO: FUA WRITE LBA: %llu, bytes: %u\n",
>>>>>>> refs/remotes/origin/cm-10.0
			task->task_lba, task->task_size);

	ret = vfs_fsync_range(fd_dev->fd_file, start, end, 1);
	if (ret != 0)
<<<<<<< HEAD
		printk(KERN_ERR "FILEIO: vfs_fsync_range() failed: %d\n", ret);
=======
		pr_err("FILEIO: vfs_fsync_range() failed: %d\n", ret);
>>>>>>> refs/remotes/origin/cm-10.0
}

static int fd_do_task(struct se_task *task)
{
	struct se_cmd *cmd = task->task_se_cmd;
=======
		pr_err("FILEIO: vfs_fsync_range() failed: %d\n", ret);

	if (immed)
		return 0;

	if (ret)
		target_complete_cmd(cmd, SAM_STAT_CHECK_CONDITION);
	else
		target_complete_cmd(cmd, SAM_STAT_GOOD);

	return 0;
}

static unsigned char *
fd_setup_write_same_buf(struct se_cmd *cmd, struct scatterlist *sg,
		    unsigned int len)
{
	struct se_device *se_dev = cmd->se_dev;
	unsigned int block_size = se_dev->dev_attrib.block_size;
	unsigned int i = 0, end;
	unsigned char *buf, *p, *kmap_buf;

	buf = kzalloc(min_t(unsigned int, len, PAGE_SIZE), GFP_KERNEL);
	if (!buf) {
		pr_err("Unable to allocate fd_execute_write_same buf\n");
		return NULL;
	}

	kmap_buf = kmap(sg_page(sg)) + sg->offset;
	if (!kmap_buf) {
		pr_err("kmap() failed in fd_setup_write_same\n");
		kfree(buf);
		return NULL;
	}
	/*
	 * Fill local *buf to contain multiple WRITE_SAME blocks up to
	 * min(len, PAGE_SIZE)
	 */
	p = buf;
	end = min_t(unsigned int, len, PAGE_SIZE);

	while (i < end) {
		memcpy(p, kmap_buf, block_size);

		i += block_size;
		p += block_size;
	}
	kunmap(sg_page(sg));

	return buf;
}

static sense_reason_t
fd_execute_write_same(struct se_cmd *cmd)
{
	struct se_device *se_dev = cmd->se_dev;
	struct fd_dev *fd_dev = FD_DEV(se_dev);
	struct file *f = fd_dev->fd_file;
	struct scatterlist *sg;
	struct iovec *iov;
	mm_segment_t old_fs;
	sector_t nolb = sbc_get_write_same_sectors(cmd);
	loff_t pos = cmd->t_task_lba * se_dev->dev_attrib.block_size;
	unsigned int len, len_tmp, iov_num;
	int i, rc;
	unsigned char *buf;

	if (!nolb) {
		target_complete_cmd(cmd, SAM_STAT_GOOD);
		return 0;
	}
	sg = &cmd->t_data_sg[0];

	if (cmd->t_data_nents > 1 ||
	    sg->length != cmd->se_dev->dev_attrib.block_size) {
		pr_err("WRITE_SAME: Illegal SGL t_data_nents: %u length: %u"
			" block_size: %u\n", cmd->t_data_nents, sg->length,
			cmd->se_dev->dev_attrib.block_size);
		return TCM_INVALID_CDB_FIELD;
	}

	len = len_tmp = nolb * se_dev->dev_attrib.block_size;
	iov_num = DIV_ROUND_UP(len, PAGE_SIZE);

	buf = fd_setup_write_same_buf(cmd, sg, len);
	if (!buf)
		return TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;

	iov = vzalloc(sizeof(struct iovec) * iov_num);
	if (!iov) {
		pr_err("Unable to allocate fd_execute_write_same iovecs\n");
		kfree(buf);
		return TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
	}
	/*
	 * Map the single fabric received scatterlist block now populated
	 * in *buf into each iovec for I/O submission.
	 */
	for (i = 0; i < iov_num; i++) {
		iov[i].iov_base = buf;
		iov[i].iov_len = min_t(unsigned int, len_tmp, PAGE_SIZE);
		len_tmp -= iov[i].iov_len;
	}

	old_fs = get_fs();
	set_fs(get_ds());
	rc = vfs_writev(f, &iov[0], iov_num, &pos);
	set_fs(old_fs);

	vfree(iov);
	kfree(buf);

	if (rc < 0 || rc != len) {
		pr_err("vfs_writev() returned %d for write same\n", rc);
		return TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
	}

	target_complete_cmd(cmd, SAM_STAT_GOOD);
	return 0;
}

static sense_reason_t
fd_do_unmap(struct se_cmd *cmd, void *priv, sector_t lba, sector_t nolb)
{
	struct file *file = priv;
	struct inode *inode = file->f_mapping->host;
	int ret;

	if (S_ISBLK(inode->i_mode)) {
		/* The backend is block device, use discard */
		struct block_device *bdev = inode->i_bdev;

		ret = blkdev_issue_discard(bdev, lba,
				nolb, GFP_KERNEL, 0);
		if (ret < 0) {
			pr_warn("FILEIO: blkdev_issue_discard() failed: %d\n",
				ret);
			return TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
		}
	} else {
		/* The backend is normal file, use fallocate */
		struct se_device *se_dev = cmd->se_dev;
		loff_t pos = lba * se_dev->dev_attrib.block_size;
		unsigned int len = nolb * se_dev->dev_attrib.block_size;
		int mode = FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE;

		if (!file->f_op->fallocate)
			return TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;

		ret = file->f_op->fallocate(file, mode, pos, len);
		if (ret < 0) {
			pr_warn("FILEIO: fallocate() failed: %d\n", ret);
			return TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
		}
	}

	return 0;
}

static sense_reason_t
fd_execute_write_same_unmap(struct se_cmd *cmd)
{
	struct se_device *se_dev = cmd->se_dev;
	struct fd_dev *fd_dev = FD_DEV(se_dev);
	struct file *file = fd_dev->fd_file;
	sector_t lba = cmd->t_task_lba;
	sector_t nolb = sbc_get_write_same_sectors(cmd);
	int ret;

	if (!nolb) {
		target_complete_cmd(cmd, SAM_STAT_GOOD);
		return 0;
	}

	ret = fd_do_unmap(cmd, file, lba, nolb);
	if (ret)
		return ret;

	target_complete_cmd(cmd, GOOD);
	return 0;
}

static sense_reason_t
fd_execute_unmap(struct se_cmd *cmd)
{
	struct file *file = FD_DEV(cmd->se_dev)->fd_file;

	return sbc_execute_unmap(cmd, fd_do_unmap, file);
}

static sense_reason_t
fd_execute_rw(struct se_cmd *cmd, struct scatterlist *sgl, u32 sgl_nents,
	      enum dma_data_direction data_direction)
{
>>>>>>> refs/remotes/origin/master
	struct se_device *dev = cmd->se_dev;
	int ret = 0;

	/*
	 * Call vectorized fileio functions to map struct scatterlist
	 * physical memory addresses to struct iovec virtual memory.
	 */
<<<<<<< HEAD
	if (task->task_data_direction == DMA_FROM_DEVICE) {
		ret = fd_do_readv(task);
	} else {
		ret = fd_do_writev(task);

		if (ret > 0 &&
<<<<<<< HEAD
		    DEV_ATTRIB(dev)->emulate_write_cache > 0 &&
		    DEV_ATTRIB(dev)->emulate_fua_write > 0 &&
		    T_TASK(cmd)->t_tasks_fua) {
=======
		    dev->se_sub_dev->se_dev_attrib.emulate_write_cache > 0 &&
		    dev->se_sub_dev->se_dev_attrib.emulate_fua_write > 0 &&
		    (cmd->se_cmd_flags & SCF_FUA)) {
>>>>>>> refs/remotes/origin/cm-10.0
			/*
			 * We might need to be a bit smarter here
			 * and return some sense data to let the initiator
			 * know the FUA WRITE cache sync failed..?
			 */
			fd_emulate_write_fua(cmd, task);
		}

	}

<<<<<<< HEAD
	if (ret < 0)
		return ret;
=======
	if (ret < 0) {
		cmd->scsi_sense_reason = TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;
		return ret;
	}
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret) {
		task->task_scsi_status = GOOD;
		transport_complete_task(task, 1);
	}
<<<<<<< HEAD
	return PYX_TRANSPORT_SENT_TO_TRANSPORT;
=======
	return 0;
>>>>>>> refs/remotes/origin/cm-10.0
}

/*	fd_free_task(): (Part of se_subsystem_api_t template)
 *
 *
 */
static void fd_free_task(struct se_task *task)
{
	struct fd_request *req = FILE_REQ(task);

	kfree(req);
=======
	if (data_direction == DMA_FROM_DEVICE) {
		ret = fd_do_rw(cmd, sgl, sgl_nents, 0);
	} else {
		ret = fd_do_rw(cmd, sgl, sgl_nents, 1);
		/*
		 * Perform implicit vfs_fsync_range() for fd_do_writev() ops
		 * for SCSI WRITEs with Forced Unit Access (FUA) set.
		 * Allow this to happen independent of WCE=0 setting.
		 */
		if (ret > 0 &&
		    dev->dev_attrib.emulate_fua_write > 0 &&
		    (cmd->se_cmd_flags & SCF_FUA)) {
			struct fd_dev *fd_dev = FD_DEV(dev);
			loff_t start = cmd->t_task_lba *
				dev->dev_attrib.block_size;
			loff_t end = start + cmd->data_length;

			vfs_fsync_range(fd_dev->fd_file, start, end, 1);
		}
	}

	if (ret < 0)
		return TCM_LOGICAL_UNIT_COMMUNICATION_FAILURE;

	if (ret)
		target_complete_cmd(cmd, SAM_STAT_GOOD);
	return 0;
>>>>>>> refs/remotes/origin/master
}

enum {
	Opt_fd_dev_name, Opt_fd_dev_size, Opt_fd_buffered_io, Opt_err
};

static match_table_t tokens = {
	{Opt_fd_dev_name, "fd_dev_name=%s"},
	{Opt_fd_dev_size, "fd_dev_size=%s"},
	{Opt_fd_buffered_io, "fd_buffered_io=%d"},
	{Opt_err, NULL}
};

<<<<<<< HEAD
static ssize_t fd_set_configfs_dev_params(
	struct se_hba *hba,
	struct se_subsystem_dev *se_dev,
	const char *page, ssize_t count)
{
	struct fd_dev *fd_dev = se_dev->se_dev_su_ptr;
=======
static ssize_t fd_set_configfs_dev_params(struct se_device *dev,
		const char *page, ssize_t count)
{
	struct fd_dev *fd_dev = FD_DEV(dev);
>>>>>>> refs/remotes/origin/master
	char *orig, *ptr, *arg_p, *opts;
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
		case Opt_fd_dev_name:
<<<<<<< HEAD
			arg_p = match_strdup(&args[0]);
			if (!arg_p) {
				ret = -ENOMEM;
				break;
			}
			snprintf(fd_dev->fd_dev_name, FD_MAX_DEV_NAME,
					"%s", arg_p);
			kfree(arg_p);
<<<<<<< HEAD
			printk(KERN_INFO "FILEIO: Referencing Path: %s\n",
=======
			pr_debug("FILEIO: Referencing Path: %s\n",
>>>>>>> refs/remotes/origin/cm-10.0
=======
			if (match_strlcpy(fd_dev->fd_dev_name, &args[0],
				FD_MAX_DEV_NAME) == 0) {
				ret = -EINVAL;
				break;
			}
			pr_debug("FILEIO: Referencing Path: %s\n",
>>>>>>> refs/remotes/origin/master
					fd_dev->fd_dev_name);
			fd_dev->fbd_flags |= FBDF_HAS_PATH;
			break;
		case Opt_fd_dev_size:
			arg_p = match_strdup(&args[0]);
			if (!arg_p) {
				ret = -ENOMEM;
				break;
			}
<<<<<<< HEAD
			ret = strict_strtoull(arg_p, 0, &fd_dev->fd_dev_size);
			kfree(arg_p);
			if (ret < 0) {
<<<<<<< HEAD
				printk(KERN_ERR "strict_strtoull() failed for"
						" fd_dev_size=\n");
				goto out;
			}
			printk(KERN_INFO "FILEIO: Referencing Size: %llu"
=======
				pr_err("strict_strtoull() failed for"
=======
			ret = kstrtoull(arg_p, 0, &fd_dev->fd_dev_size);
			kfree(arg_p);
			if (ret < 0) {
				pr_err("kstrtoull() failed for"
>>>>>>> refs/remotes/origin/master
						" fd_dev_size=\n");
				goto out;
			}
			pr_debug("FILEIO: Referencing Size: %llu"
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
					" bytes\n", fd_dev->fd_dev_size);
			fd_dev->fbd_flags |= FBDF_HAS_SIZE;
			break;
		case Opt_fd_buffered_io:
			match_int(args, &arg);
			if (arg != 1) {
<<<<<<< HEAD
<<<<<<< HEAD
				printk(KERN_ERR "bogus fd_buffered_io=%d value\n", arg);
=======
				pr_err("bogus fd_buffered_io=%d value\n", arg);
>>>>>>> refs/remotes/origin/cm-10.0
=======
				pr_err("bogus fd_buffered_io=%d value\n", arg);
>>>>>>> refs/remotes/origin/master
				ret = -EINVAL;
				goto out;
			}

<<<<<<< HEAD
<<<<<<< HEAD
			printk(KERN_INFO "FILEIO: Using buffered I/O"
=======
			pr_debug("FILEIO: Using buffered I/O"
>>>>>>> refs/remotes/origin/cm-10.0
				" operations for struct fd_dev\n");

			fd_dev->fbd_flags |= FDBD_USE_BUFFERED_IO;
=======
			pr_debug("FILEIO: Using buffered I/O"
				" operations for struct fd_dev\n");

			fd_dev->fbd_flags |= FDBD_HAS_BUFFERED_IO_WCE;
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
static ssize_t fd_check_configfs_dev_params(struct se_hba *hba, struct se_subsystem_dev *se_dev)
{
<<<<<<< HEAD
	struct fd_dev *fd_dev = (struct fd_dev *) se_dev->se_dev_su_ptr;

	if (!(fd_dev->fbd_flags & FBDF_HAS_PATH)) {
		printk(KERN_ERR "Missing fd_dev_name=\n");
		return -1;
=======
	struct fd_dev *fd_dev = se_dev->se_dev_su_ptr;

	if (!(fd_dev->fbd_flags & FBDF_HAS_PATH)) {
		pr_err("Missing fd_dev_name=\n");
		return -EINVAL;
>>>>>>> refs/remotes/origin/cm-10.0
	}

	return 0;
}

static ssize_t fd_show_configfs_dev_params(
	struct se_hba *hba,
	struct se_subsystem_dev *se_dev,
	char *b)
{
	struct fd_dev *fd_dev = se_dev->se_dev_su_ptr;
=======
static ssize_t fd_show_configfs_dev_params(struct se_device *dev, char *b)
{
	struct fd_dev *fd_dev = FD_DEV(dev);
>>>>>>> refs/remotes/origin/master
	ssize_t bl = 0;

	bl = sprintf(b + bl, "TCM FILEIO ID: %u", fd_dev->fd_dev_id);
	bl += sprintf(b + bl, "        File: %s  Size: %llu  Mode: %s\n",
		fd_dev->fd_dev_name, fd_dev->fd_dev_size,
<<<<<<< HEAD
		(fd_dev->fbd_flags & FDBD_USE_BUFFERED_IO) ?
		"Buffered" : "Synchronous");
	return bl;
}

<<<<<<< HEAD
/*	fd_get_cdb(): (Part of se_subsystem_api_t template)
 *
 *
 */
static unsigned char *fd_get_cdb(struct se_task *task)
{
	struct fd_request *req = FILE_REQ(task);

	return req->fd_scsi_cdb;
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
/*	fd_get_device_rev(): (Part of se_subsystem_api_t template)
 *
 *
 */
static u32 fd_get_device_rev(struct se_device *dev)
{
	return SCSI_SPC_2; /* Returns SPC-3 in Initiator Data */
}

/*	fd_get_device_type(): (Part of se_subsystem_api_t template)
 *
 *
 */
static u32 fd_get_device_type(struct se_device *dev)
{
	return TYPE_DISK;
}

static sector_t fd_get_blocks(struct se_device *dev)
{
	struct fd_dev *fd_dev = dev->dev_ptr;
<<<<<<< HEAD
	unsigned long long blocks_long = div_u64(fd_dev->fd_dev_size,
			DEV_ATTRIB(dev)->block_size);

	return blocks_long;
=======
=======
		(fd_dev->fbd_flags & FDBD_HAS_BUFFERED_IO_WCE) ?
		"Buffered-WCE" : "O_DSYNC");
	return bl;
}

static sector_t fd_get_blocks(struct se_device *dev)
{
	struct fd_dev *fd_dev = FD_DEV(dev);
>>>>>>> refs/remotes/origin/master
	struct file *f = fd_dev->fd_file;
	struct inode *i = f->f_mapping->host;
	unsigned long long dev_size;
	/*
	 * When using a file that references an underlying struct block_device,
	 * ensure dev_size is always based on the current inode size in order
	 * to handle underlying block_device resize operations.
	 */
	if (S_ISBLK(i->i_mode))
<<<<<<< HEAD
		dev_size = (i_size_read(i) - fd_dev->fd_block_size);
	else
		dev_size = fd_dev->fd_dev_size;

	return div_u64(dev_size, dev->se_sub_dev->se_dev_attrib.block_size);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		dev_size = i_size_read(i);
	else
		dev_size = fd_dev->fd_dev_size;

	return div_u64(dev_size - dev->dev_attrib.block_size,
		       dev->dev_attrib.block_size);
}

static struct sbc_ops fd_sbc_ops = {
	.execute_rw		= fd_execute_rw,
	.execute_sync_cache	= fd_execute_sync_cache,
	.execute_write_same	= fd_execute_write_same,
	.execute_write_same_unmap = fd_execute_write_same_unmap,
	.execute_unmap		= fd_execute_unmap,
};

static sense_reason_t
fd_parse_cdb(struct se_cmd *cmd)
{
	return sbc_parse_cdb(cmd, &fd_sbc_ops);
>>>>>>> refs/remotes/origin/master
}

static struct se_subsystem_api fileio_template = {
	.name			= "fileio",
<<<<<<< HEAD
	.owner			= THIS_MODULE,
	.transport_type		= TRANSPORT_PLUGIN_VHBA_PDEV,
<<<<<<< HEAD
=======
	.write_cache_emulated	= 1,
	.fua_write_emulated	= 1,
>>>>>>> refs/remotes/origin/cm-10.0
	.attach_hba		= fd_attach_hba,
	.detach_hba		= fd_detach_hba,
	.allocate_virtdevice	= fd_allocate_virtdevice,
	.create_virtdevice	= fd_create_virtdevice,
	.free_device		= fd_free_device,
<<<<<<< HEAD
	.dpo_emulated		= fd_emulated_dpo,
	.fua_write_emulated	= fd_emulated_fua_write,
	.fua_read_emulated	= fd_emulated_fua_read,
	.write_cache_emulated	= fd_emulated_write_cache,
=======
>>>>>>> refs/remotes/origin/cm-10.0
	.alloc_task		= fd_alloc_task,
	.do_task		= fd_do_task,
	.do_sync_cache		= fd_emulate_sync_cache,
	.free_task		= fd_free_task,
	.check_configfs_dev_params = fd_check_configfs_dev_params,
	.set_configfs_dev_params = fd_set_configfs_dev_params,
	.show_configfs_dev_params = fd_show_configfs_dev_params,
<<<<<<< HEAD
	.get_cdb		= fd_get_cdb,
=======
>>>>>>> refs/remotes/origin/cm-10.0
	.get_device_rev		= fd_get_device_rev,
	.get_device_type	= fd_get_device_type,
=======
	.inquiry_prod		= "FILEIO",
	.inquiry_rev		= FD_VERSION,
	.owner			= THIS_MODULE,
	.transport_type		= TRANSPORT_PLUGIN_VHBA_PDEV,
	.attach_hba		= fd_attach_hba,
	.detach_hba		= fd_detach_hba,
	.alloc_device		= fd_alloc_device,
	.configure_device	= fd_configure_device,
	.free_device		= fd_free_device,
	.parse_cdb		= fd_parse_cdb,
	.set_configfs_dev_params = fd_set_configfs_dev_params,
	.show_configfs_dev_params = fd_show_configfs_dev_params,
	.get_device_type	= sbc_get_device_type,
>>>>>>> refs/remotes/origin/master
	.get_blocks		= fd_get_blocks,
};

static int __init fileio_module_init(void)
{
	return transport_subsystem_register(&fileio_template);
}

<<<<<<< HEAD
static void fileio_module_exit(void)
=======
static void __exit fileio_module_exit(void)
>>>>>>> refs/remotes/origin/master
{
	transport_subsystem_release(&fileio_template);
}

MODULE_DESCRIPTION("TCM FILEIO subsystem plugin");
MODULE_AUTHOR("nab@Linux-iSCSI.org");
MODULE_LICENSE("GPL");

module_init(fileio_module_init);
module_exit(fileio_module_exit);
