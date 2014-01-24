/*
 *  c 2001 PPC 64 Team, IBM Corp
 *
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 *
 * /proc/powerpc/rtas/firmware_flash interface
 *
 * This file implements a firmware_flash interface to pump a firmware
 * image into the kernel.  At reboot time rtas_restart() will see the
 * firmware image and flash it as it reboots (see rtas.c).
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/reboot.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <asm/delay.h>
#include <asm/uaccess.h>
#include <asm/rtas.h>
#include <asm/abs_addr.h>
=======
#include <linux/reboot.h>
#include <asm/delay.h>
#include <asm/uaccess.h>
#include <asm/rtas.h>
>>>>>>> refs/remotes/origin/master

#define MODULE_VERS "1.0"
#define MODULE_NAME "rtas_flash"

#define FIRMWARE_FLASH_NAME "firmware_flash"   
#define FIRMWARE_UPDATE_NAME "firmware_update"
#define MANAGE_FLASH_NAME "manage_flash"
#define VALIDATE_FLASH_NAME "validate_flash"

/* General RTAS Status Codes */
#define RTAS_RC_SUCCESS  0
#define RTAS_RC_HW_ERR	-1
#define RTAS_RC_BUSY	-2

/* Flash image status values */
#define FLASH_AUTH           -9002 /* RTAS Not Service Authority Partition */
#define FLASH_NO_OP          -1099 /* No operation initiated by user */	
#define FLASH_IMG_SHORT	     -1005 /* Flash image shorter than expected */
#define FLASH_IMG_BAD_LEN    -1004 /* Bad length value in flash list block */
#define FLASH_IMG_NULL_DATA  -1003 /* Bad data value in flash list block */
#define FLASH_IMG_READY      0     /* Firmware img ready for flash on reboot */

/* Manage image status values */
#define MANAGE_AUTH          -9002 /* RTAS Not Service Authority Partition */
#define MANAGE_ACTIVE_ERR    -9001 /* RTAS Cannot Overwrite Active Img */
#define MANAGE_NO_OP         -1099 /* No operation initiated by user */
#define MANAGE_PARAM_ERR     -3    /* RTAS Parameter Error */
#define MANAGE_HW_ERR        -1    /* RTAS Hardware Error */

/* Validate image status values */
#define VALIDATE_AUTH          -9002 /* RTAS Not Service Authority Partition */
#define VALIDATE_NO_OP         -1099 /* No operation initiated by the user */
#define VALIDATE_INCOMPLETE    -1002 /* User copied < VALIDATE_BUF_SIZE */
#define VALIDATE_READY	       -1001 /* Firmware image ready for validation */
#define VALIDATE_PARAM_ERR     -3    /* RTAS Parameter Error */
#define VALIDATE_HW_ERR        -1    /* RTAS Hardware Error */
<<<<<<< HEAD
#define VALIDATE_TMP_UPDATE    0     /* Validate Return Status */
#define VALIDATE_FLASH_AUTH    1     /* Validate Return Status */
#define VALIDATE_INVALID_IMG   2     /* Validate Return Status */
#define VALIDATE_CUR_UNKNOWN   3     /* Validate Return Status */
#define VALIDATE_TMP_COMMIT_DL 4     /* Validate Return Status */
#define VALIDATE_TMP_COMMIT    5     /* Validate Return Status */
#define VALIDATE_TMP_UPDATE_DL 6     /* Validate Return Status */
=======

/* ibm,validate-flash-image update result tokens */
#define VALIDATE_TMP_UPDATE    0     /* T side will be updated */
#define VALIDATE_FLASH_AUTH    1     /* Partition does not have authority */
#define VALIDATE_INVALID_IMG   2     /* Candidate image is not valid */
#define VALIDATE_CUR_UNKNOWN   3     /* Current fixpack level is unknown */
/*
 * Current T side will be committed to P side before being replace with new
 * image, and the new image is downlevel from current image
 */
#define VALIDATE_TMP_COMMIT_DL 4
/*
 * Current T side will be committed to P side before being replaced with new
 * image
 */
#define VALIDATE_TMP_COMMIT    5
/*
 * T side will be updated with a downlevel image
 */
#define VALIDATE_TMP_UPDATE_DL 6
/*
 * The candidate image's release date is later than the system's firmware
 * service entitlement date - service warranty period has expired
 */
#define VALIDATE_OUT_OF_WRNTY  7
>>>>>>> refs/remotes/origin/master

/* ibm,manage-flash-image operation tokens */
#define RTAS_REJECT_TMP_IMG   0
#define RTAS_COMMIT_TMP_IMG   1

/* Array sizes */
#define VALIDATE_BUF_SIZE 4096    
<<<<<<< HEAD
=======
#define VALIDATE_MSG_LEN  256
>>>>>>> refs/remotes/origin/master
#define RTAS_MSG_MAXLEN   64

/* Quirk - RTAS requires 4k list length and block size */
#define RTAS_BLKLIST_LENGTH 4096
#define RTAS_BLK_SIZE 4096

struct flash_block {
	char *data;
	unsigned long length;
};

/* This struct is very similar but not identical to
 * that needed by the rtas flash update.
 * All we need to do for rtas is rewrite num_blocks
 * into a version/length and translate the pointers
 * to absolute.
 */
#define FLASH_BLOCKS_PER_NODE ((RTAS_BLKLIST_LENGTH - 16) / sizeof(struct flash_block))
struct flash_block_list {
	unsigned long num_blocks;
	struct flash_block_list *next;
	struct flash_block blocks[FLASH_BLOCKS_PER_NODE];
};

static struct flash_block_list *rtas_firmware_flash_list;

/* Use slab cache to guarantee 4k alignment */
static struct kmem_cache *flash_block_cache = NULL;

#define FLASH_BLOCK_LIST_VERSION (1UL)

<<<<<<< HEAD
/* Local copy of the flash block list.
 * We only allow one open of the flash proc file and create this
 * list as we go.  The rtas_firmware_flash_list varable will be
=======
/*
 * Local copy of the flash block list.
 *
 * The rtas_firmware_flash_list varable will be
>>>>>>> refs/remotes/origin/master
 * set once the data is fully read.
 *
 * For convenience as we build the list we use virtual addrs,
 * we do not fill in the version number, and the length field
 * is treated as the number of entries currently in the block
 * (i.e. not a byte count).  This is all fixed when calling 
 * the flash routine.
 */

/* Status int must be first member of struct */
struct rtas_update_flash_t
{
	int status;			/* Flash update status */
	struct flash_block_list *flist; /* Local copy of flash block list */
};

/* Status int must be first member of struct */
struct rtas_manage_flash_t
{
	int status;			/* Returned status */
<<<<<<< HEAD
	unsigned int op;		/* Reject or commit image */
=======
>>>>>>> refs/remotes/origin/master
};

/* Status int must be first member of struct */
struct rtas_validate_flash_t
{
	int status;		 	/* Returned status */	
<<<<<<< HEAD
	char buf[VALIDATE_BUF_SIZE]; 	/* Candidate image buffer */
=======
	char *buf;			/* Candidate image buffer */
>>>>>>> refs/remotes/origin/master
	unsigned int buf_size;		/* Size of image buf */
	unsigned int update_results;	/* Update results token */
};

<<<<<<< HEAD
static DEFINE_SPINLOCK(flash_file_open_lock);
static struct proc_dir_entry *firmware_flash_pde;
static struct proc_dir_entry *firmware_update_pde;
static struct proc_dir_entry *validate_pde;
static struct proc_dir_entry *manage_pde;
=======
static struct rtas_update_flash_t rtas_update_flash_data;
static struct rtas_manage_flash_t rtas_manage_flash_data;
static struct rtas_validate_flash_t rtas_validate_flash_data;
static DEFINE_MUTEX(rtas_update_flash_mutex);
static DEFINE_MUTEX(rtas_manage_flash_mutex);
static DEFINE_MUTEX(rtas_validate_flash_mutex);
>>>>>>> refs/remotes/origin/master

/* Do simple sanity checks on the flash image. */
static int flash_list_valid(struct flash_block_list *flist)
{
	struct flash_block_list *f;
	int i;
	unsigned long block_size, image_size;

	/* Paranoid self test here.  We also collect the image size. */
	image_size = 0;
	for (f = flist; f; f = f->next) {
		for (i = 0; i < f->num_blocks; i++) {
			if (f->blocks[i].data == NULL) {
				return FLASH_IMG_NULL_DATA;
			}
			block_size = f->blocks[i].length;
			if (block_size <= 0 || block_size > RTAS_BLK_SIZE) {
				return FLASH_IMG_BAD_LEN;
			}
			image_size += block_size;
		}
	}

	if (image_size < (256 << 10)) {
		if (image_size < 2) 
			return FLASH_NO_OP;
	}

	printk(KERN_INFO "FLASH: flash image with %ld bytes stored for hardware flash on reboot\n", image_size);

	return FLASH_IMG_READY;
}

static void free_flash_list(struct flash_block_list *f)
{
	struct flash_block_list *next;
	int i;

	while (f) {
		for (i = 0; i < f->num_blocks; i++)
			kmem_cache_free(flash_block_cache, f->blocks[i].data);
		next = f->next;
		kmem_cache_free(flash_block_cache, f);
		f = next;
	}
}

static int rtas_flash_release(struct inode *inode, struct file *file)
{
<<<<<<< HEAD
	struct proc_dir_entry *dp = PDE(file->f_path.dentry->d_inode);
	struct rtas_update_flash_t *uf;
	
	uf = (struct rtas_update_flash_t *) dp->data;
=======
	struct rtas_update_flash_t *const uf = &rtas_update_flash_data;

	mutex_lock(&rtas_update_flash_mutex);

>>>>>>> refs/remotes/origin/master
	if (uf->flist) {    
		/* File was opened in write mode for a new flash attempt */
		/* Clear saved list */
		if (rtas_firmware_flash_list) {
			free_flash_list(rtas_firmware_flash_list);
			rtas_firmware_flash_list = NULL;
		}

		if (uf->status != FLASH_AUTH)  
			uf->status = flash_list_valid(uf->flist);

		if (uf->status == FLASH_IMG_READY) 
			rtas_firmware_flash_list = uf->flist;
		else
			free_flash_list(uf->flist);

		uf->flist = NULL;
	}

<<<<<<< HEAD
	atomic_dec(&dp->count);
	return 0;
}

static void get_flash_status_msg(int status, char *buf)
{
	char *msg;
=======
	mutex_unlock(&rtas_update_flash_mutex);
	return 0;
}

static size_t get_flash_status_msg(int status, char *buf)
{
	const char *msg;
	size_t len;
>>>>>>> refs/remotes/origin/master

	switch (status) {
	case FLASH_AUTH:
		msg = "error: this partition does not have service authority\n";
		break;
	case FLASH_NO_OP:
		msg = "info: no firmware image for flash\n";
		break;
	case FLASH_IMG_SHORT:
		msg = "error: flash image short\n";
		break;
	case FLASH_IMG_BAD_LEN:
		msg = "error: internal error bad length\n";
		break;
	case FLASH_IMG_NULL_DATA:
		msg = "error: internal error null data\n";
		break;
	case FLASH_IMG_READY:
		msg = "ready: firmware image ready for flash on reboot\n";
		break;
	default:
<<<<<<< HEAD
		sprintf(buf, "error: unexpected status value %d\n", status);
		return;
	}

	strcpy(buf, msg);	
}

/* Reading the proc file will show status (not the firmware contents) */
static ssize_t rtas_flash_read(struct file *file, char __user *buf,
			       size_t count, loff_t *ppos)
{
	struct proc_dir_entry *dp = PDE(file->f_path.dentry->d_inode);
	struct rtas_update_flash_t *uf;
	char msg[RTAS_MSG_MAXLEN];

	uf = dp->data;

	if (!strcmp(dp->name, FIRMWARE_FLASH_NAME)) {
		get_flash_status_msg(uf->status, msg);
	} else {	   /* FIRMWARE_UPDATE_NAME */
		sprintf(msg, "%d\n", uf->status);
	}

	return simple_read_from_buffer(buf, count, ppos, msg, strlen(msg));
}

/* constructor for flash_block_cache */
void rtas_block_ctor(void *ptr)
{
	memset(ptr, 0, RTAS_BLK_SIZE);
=======
		return sprintf(buf, "error: unexpected status value %d\n",
			       status);
	}

	len = strlen(msg);
	memcpy(buf, msg, len + 1);
	return len;
}

/* Reading the proc file will show status (not the firmware contents) */
static ssize_t rtas_flash_read_msg(struct file *file, char __user *buf,
				   size_t count, loff_t *ppos)
{
	struct rtas_update_flash_t *const uf = &rtas_update_flash_data;
	char msg[RTAS_MSG_MAXLEN];
	size_t len;
	int status;

	mutex_lock(&rtas_update_flash_mutex);
	status = uf->status;
	mutex_unlock(&rtas_update_flash_mutex);

	/* Read as text message */
	len = get_flash_status_msg(status, msg);
	return simple_read_from_buffer(buf, count, ppos, msg, len);
}

static ssize_t rtas_flash_read_num(struct file *file, char __user *buf,
				   size_t count, loff_t *ppos)
{
	struct rtas_update_flash_t *const uf = &rtas_update_flash_data;
	char msg[RTAS_MSG_MAXLEN];
	int status;

	mutex_lock(&rtas_update_flash_mutex);
	status = uf->status;
	mutex_unlock(&rtas_update_flash_mutex);

	/* Read as number */
	sprintf(msg, "%d\n", status);
	return simple_read_from_buffer(buf, count, ppos, msg, strlen(msg));
>>>>>>> refs/remotes/origin/master
}

/* We could be much more efficient here.  But to keep this function
 * simple we allocate a page to the block list no matter how small the
 * count is.  If the system is low on memory it will be just as well
 * that we fail....
 */
static ssize_t rtas_flash_write(struct file *file, const char __user *buffer,
				size_t count, loff_t *off)
{
<<<<<<< HEAD
	struct proc_dir_entry *dp = PDE(file->f_path.dentry->d_inode);
	struct rtas_update_flash_t *uf;
	char *p;
	int next_free;
	struct flash_block_list *fl;

	uf = (struct rtas_update_flash_t *) dp->data;

	if (uf->status == FLASH_AUTH || count == 0)
		return count;	/* discard data */
=======
	struct rtas_update_flash_t *const uf = &rtas_update_flash_data;
	char *p;
	int next_free, rc;
	struct flash_block_list *fl;

	mutex_lock(&rtas_update_flash_mutex);

	if (uf->status == FLASH_AUTH || count == 0)
		goto out;	/* discard data */
>>>>>>> refs/remotes/origin/master

	/* In the case that the image is not ready for flashing, the memory
	 * allocated for the block list will be freed upon the release of the 
	 * proc file
	 */
	if (uf->flist == NULL) {
<<<<<<< HEAD
		uf->flist = kmem_cache_alloc(flash_block_cache, GFP_KERNEL);
		if (!uf->flist)
			return -ENOMEM;
=======
		uf->flist = kmem_cache_zalloc(flash_block_cache, GFP_KERNEL);
		if (!uf->flist)
			goto nomem;
>>>>>>> refs/remotes/origin/master
	}

	fl = uf->flist;
	while (fl->next)
		fl = fl->next; /* seek to last block_list for append */
	next_free = fl->num_blocks;
	if (next_free == FLASH_BLOCKS_PER_NODE) {
		/* Need to allocate another block_list */
<<<<<<< HEAD
		fl->next = kmem_cache_alloc(flash_block_cache, GFP_KERNEL);
		if (!fl->next)
			return -ENOMEM;
=======
		fl->next = kmem_cache_zalloc(flash_block_cache, GFP_KERNEL);
		if (!fl->next)
			goto nomem;
>>>>>>> refs/remotes/origin/master
		fl = fl->next;
		next_free = 0;
	}

	if (count > RTAS_BLK_SIZE)
		count = RTAS_BLK_SIZE;
<<<<<<< HEAD
	p = kmem_cache_alloc(flash_block_cache, GFP_KERNEL);
	if (!p)
		return -ENOMEM;
	
	if(copy_from_user(p, buffer, count)) {
		kmem_cache_free(flash_block_cache, p);
		return -EFAULT;
=======
	p = kmem_cache_zalloc(flash_block_cache, GFP_KERNEL);
	if (!p)
		goto nomem;
	
	if(copy_from_user(p, buffer, count)) {
		kmem_cache_free(flash_block_cache, p);
		rc = -EFAULT;
		goto error;
>>>>>>> refs/remotes/origin/master
	}
	fl->blocks[next_free].data = p;
	fl->blocks[next_free].length = count;
	fl->num_blocks++;
<<<<<<< HEAD

	return count;
}

static int rtas_excl_open(struct inode *inode, struct file *file)
{
	struct proc_dir_entry *dp = PDE(inode);

	/* Enforce exclusive open with use count of PDE */
	spin_lock(&flash_file_open_lock);
	if (atomic_read(&dp->count) > 2) {
		spin_unlock(&flash_file_open_lock);
		return -EBUSY;
	}

	atomic_inc(&dp->count);
	spin_unlock(&flash_file_open_lock);
	
	return 0;
}

static int rtas_excl_release(struct inode *inode, struct file *file)
{
	struct proc_dir_entry *dp = PDE(inode);

	atomic_dec(&dp->count);

	return 0;
}

static void manage_flash(struct rtas_manage_flash_t *args_buf)
=======
out:
	mutex_unlock(&rtas_update_flash_mutex);
	return count;

nomem:
	rc = -ENOMEM;
error:
	mutex_unlock(&rtas_update_flash_mutex);
	return rc;
}

/*
 * Flash management routines.
 */
static void manage_flash(struct rtas_manage_flash_t *args_buf, unsigned int op)
>>>>>>> refs/remotes/origin/master
{
	s32 rc;

	do {
<<<<<<< HEAD
		rc = rtas_call(rtas_token("ibm,manage-flash-image"), 1, 
			       1, NULL, args_buf->op);
=======
		rc = rtas_call(rtas_token("ibm,manage-flash-image"), 1, 1,
			       NULL, op);
>>>>>>> refs/remotes/origin/master
	} while (rtas_busy_delay(rc));

	args_buf->status = rc;
}

static ssize_t manage_flash_read(struct file *file, char __user *buf,
			       size_t count, loff_t *ppos)
{
<<<<<<< HEAD
	struct proc_dir_entry *dp = PDE(file->f_path.dentry->d_inode);
	struct rtas_manage_flash_t *args_buf;
	char msg[RTAS_MSG_MAXLEN];
	int msglen;

	args_buf = dp->data;
	if (args_buf == NULL)
		return 0;

	msglen = sprintf(msg, "%d\n", args_buf->status);

=======
	struct rtas_manage_flash_t *const args_buf = &rtas_manage_flash_data;
	char msg[RTAS_MSG_MAXLEN];
	int msglen, status;

	mutex_lock(&rtas_manage_flash_mutex);
	status = args_buf->status;
	mutex_unlock(&rtas_manage_flash_mutex);

	msglen = sprintf(msg, "%d\n", status);
>>>>>>> refs/remotes/origin/master
	return simple_read_from_buffer(buf, count, ppos, msg, msglen);
}

static ssize_t manage_flash_write(struct file *file, const char __user *buf,
				size_t count, loff_t *off)
{
<<<<<<< HEAD
	struct proc_dir_entry *dp = PDE(file->f_path.dentry->d_inode);
	struct rtas_manage_flash_t *args_buf;
	const char reject_str[] = "0";
	const char commit_str[] = "1";
	char stkbuf[10];
	int op;

	args_buf = (struct rtas_manage_flash_t *) dp->data;
	if ((args_buf->status == MANAGE_AUTH) || (count == 0))
		return count;
=======
	struct rtas_manage_flash_t *const args_buf = &rtas_manage_flash_data;
	static const char reject_str[] = "0";
	static const char commit_str[] = "1";
	char stkbuf[10];
	int op, rc;

	mutex_lock(&rtas_manage_flash_mutex);

	if ((args_buf->status == MANAGE_AUTH) || (count == 0))
		goto out;
>>>>>>> refs/remotes/origin/master
		
	op = -1;
	if (buf) {
		if (count > 9) count = 9;
<<<<<<< HEAD
		if (copy_from_user (stkbuf, buf, count)) {
			return -EFAULT;
		}
=======
		rc = -EFAULT;
		if (copy_from_user (stkbuf, buf, count))
			goto error;
>>>>>>> refs/remotes/origin/master
		if (strncmp(stkbuf, reject_str, strlen(reject_str)) == 0) 
			op = RTAS_REJECT_TMP_IMG;
		else if (strncmp(stkbuf, commit_str, strlen(commit_str)) == 0) 
			op = RTAS_COMMIT_TMP_IMG;
	}
	
<<<<<<< HEAD
	if (op == -1)   /* buf is empty, or contains invalid string */
		return -EINVAL;

	args_buf->op = op;
	manage_flash(args_buf);

	return count;
}

=======
	if (op == -1) {   /* buf is empty, or contains invalid string */
		rc = -EINVAL;
		goto error;
	}

	manage_flash(args_buf, op);
out:
	mutex_unlock(&rtas_manage_flash_mutex);
	return count;

error:
	mutex_unlock(&rtas_manage_flash_mutex);
	return rc;
}

/*
 * Validation routines.
 */
>>>>>>> refs/remotes/origin/master
static void validate_flash(struct rtas_validate_flash_t *args_buf)
{
	int token = rtas_token("ibm,validate-flash-image");
	int update_results;
	s32 rc;	

	rc = 0;
	do {
		spin_lock(&rtas_data_buf_lock);
		memcpy(rtas_data_buf, args_buf->buf, VALIDATE_BUF_SIZE);
		rc = rtas_call(token, 2, 2, &update_results, 
			       (u32) __pa(rtas_data_buf), args_buf->buf_size);
		memcpy(args_buf->buf, rtas_data_buf, VALIDATE_BUF_SIZE);
		spin_unlock(&rtas_data_buf_lock);
	} while (rtas_busy_delay(rc));

	args_buf->status = rc;
	args_buf->update_results = update_results;
}

static int get_validate_flash_msg(struct rtas_validate_flash_t *args_buf, 
<<<<<<< HEAD
		                   char *msg)
=======
		                   char *msg, int msglen)
>>>>>>> refs/remotes/origin/master
{
	int n;

	if (args_buf->status >= VALIDATE_TMP_UPDATE) { 
		n = sprintf(msg, "%d\n", args_buf->update_results);
		if ((args_buf->update_results >= VALIDATE_CUR_UNKNOWN) ||
		    (args_buf->update_results == VALIDATE_TMP_UPDATE))
<<<<<<< HEAD
			n += sprintf(msg + n, "%s\n", args_buf->buf);
=======
			n += snprintf(msg + n, msglen - n, "%s\n",
					args_buf->buf);
>>>>>>> refs/remotes/origin/master
	} else {
		n = sprintf(msg, "%d\n", args_buf->status);
	}
	return n;
}

static ssize_t validate_flash_read(struct file *file, char __user *buf,
			       size_t count, loff_t *ppos)
{
<<<<<<< HEAD
	struct proc_dir_entry *dp = PDE(file->f_path.dentry->d_inode);
	struct rtas_validate_flash_t *args_buf;
	char msg[RTAS_MSG_MAXLEN];
	int msglen;

	args_buf = dp->data;

	msglen = get_validate_flash_msg(args_buf, msg);
=======
	struct rtas_validate_flash_t *const args_buf =
		&rtas_validate_flash_data;
	char msg[VALIDATE_MSG_LEN];
	int msglen;

	mutex_lock(&rtas_validate_flash_mutex);
	msglen = get_validate_flash_msg(args_buf, msg, VALIDATE_MSG_LEN);
	mutex_unlock(&rtas_validate_flash_mutex);
>>>>>>> refs/remotes/origin/master

	return simple_read_from_buffer(buf, count, ppos, msg, msglen);
}

static ssize_t validate_flash_write(struct file *file, const char __user *buf,
				    size_t count, loff_t *off)
{
<<<<<<< HEAD
	struct proc_dir_entry *dp = PDE(file->f_path.dentry->d_inode);
	struct rtas_validate_flash_t *args_buf;
	int rc;

	args_buf = (struct rtas_validate_flash_t *) dp->data;

	if (dp->data == NULL) {
		dp->data = kmalloc(sizeof(struct rtas_validate_flash_t), 
				GFP_KERNEL);
		if (dp->data == NULL) 
			return -ENOMEM;
	}
=======
	struct rtas_validate_flash_t *const args_buf =
		&rtas_validate_flash_data;
	int rc;

	mutex_lock(&rtas_validate_flash_mutex);
>>>>>>> refs/remotes/origin/master

	/* We are only interested in the first 4K of the
	 * candidate image */
	if ((*off >= VALIDATE_BUF_SIZE) || 
		(args_buf->status == VALIDATE_AUTH)) {
		*off += count;
<<<<<<< HEAD
=======
		mutex_unlock(&rtas_validate_flash_mutex);
>>>>>>> refs/remotes/origin/master
		return count;
	}

	if (*off + count >= VALIDATE_BUF_SIZE)  {
		count = VALIDATE_BUF_SIZE - *off;
		args_buf->status = VALIDATE_READY;	
	} else {
		args_buf->status = VALIDATE_INCOMPLETE;
	}

	if (!access_ok(VERIFY_READ, buf, count)) {
		rc = -EFAULT;
		goto done;
	}
	if (copy_from_user(args_buf->buf + *off, buf, count)) {
		rc = -EFAULT;
		goto done;
	}

	*off += count;
	rc = count;
done:
<<<<<<< HEAD
	if (rc < 0) {
		kfree(dp->data);
		dp->data = NULL;
	}
=======
	mutex_unlock(&rtas_validate_flash_mutex);
>>>>>>> refs/remotes/origin/master
	return rc;
}

static int validate_flash_release(struct inode *inode, struct file *file)
{
<<<<<<< HEAD
	struct proc_dir_entry *dp = PDE(file->f_path.dentry->d_inode);
	struct rtas_validate_flash_t *args_buf;

	args_buf = (struct rtas_validate_flash_t *) dp->data;
=======
	struct rtas_validate_flash_t *const args_buf =
		&rtas_validate_flash_data;

	mutex_lock(&rtas_validate_flash_mutex);
>>>>>>> refs/remotes/origin/master

	if (args_buf->status == VALIDATE_READY) {
		args_buf->buf_size = VALIDATE_BUF_SIZE;
		validate_flash(args_buf);
	}

<<<<<<< HEAD
	/* The matching atomic_inc was in rtas_excl_open() */
	atomic_dec(&dp->count);

	return 0;
}

=======
	mutex_unlock(&rtas_validate_flash_mutex);
	return 0;
}

/*
 * On-reboot flash update applicator.
 */
>>>>>>> refs/remotes/origin/master
static void rtas_flash_firmware(int reboot_type)
{
	unsigned long image_size;
	struct flash_block_list *f, *next, *flist;
	unsigned long rtas_block_list;
	int i, status, update_token;

	if (rtas_firmware_flash_list == NULL)
		return;		/* nothing to do */

	if (reboot_type != SYS_RESTART) {
		printk(KERN_ALERT "FLASH: firmware flash requires a reboot\n");
		printk(KERN_ALERT "FLASH: the firmware image will NOT be flashed\n");
		return;
	}

	update_token = rtas_token("ibm,update-flash-64-and-reboot");
	if (update_token == RTAS_UNKNOWN_SERVICE) {
		printk(KERN_ALERT "FLASH: ibm,update-flash-64-and-reboot "
		       "is not available -- not a service partition?\n");
		printk(KERN_ALERT "FLASH: firmware will not be flashed\n");
		return;
	}

	/*
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	 * Just before starting the firmware flash, cancel the event scan work
	 * to avoid any soft lockup issues.
	 */
	rtas_cancel_event_scan();

	/*
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	 * NOTE: the "first" block must be under 4GB, so we create
	 * an entry with no data blocks in the reserved buffer in
	 * the kernel data segment.
	 */
	spin_lock(&rtas_data_buf_lock);
	flist = (struct flash_block_list *)&rtas_data_buf[0];
	flist->num_blocks = 0;
	flist->next = rtas_firmware_flash_list;
<<<<<<< HEAD
	rtas_block_list = virt_to_abs(flist);
=======
	rtas_block_list = __pa(flist);
>>>>>>> refs/remotes/origin/master
	if (rtas_block_list >= 4UL*1024*1024*1024) {
		printk(KERN_ALERT "FLASH: kernel bug...flash list header addr above 4GB\n");
		spin_unlock(&rtas_data_buf_lock);
		return;
	}

	printk(KERN_ALERT "FLASH: preparing saved firmware image for flash\n");
	/* Update the block_list in place. */
	rtas_firmware_flash_list = NULL; /* too hard to backout on error */
	image_size = 0;
	for (f = flist; f; f = next) {
		/* Translate data addrs to absolute */
		for (i = 0; i < f->num_blocks; i++) {
<<<<<<< HEAD
			f->blocks[i].data = (char *)virt_to_abs(f->blocks[i].data);
=======
			f->blocks[i].data = (char *)__pa(f->blocks[i].data);
>>>>>>> refs/remotes/origin/master
			image_size += f->blocks[i].length;
		}
		next = f->next;
		/* Don't translate NULL pointer for last entry */
		if (f->next)
<<<<<<< HEAD
			f->next = (struct flash_block_list *)virt_to_abs(f->next);
=======
			f->next = (struct flash_block_list *)__pa(f->next);
>>>>>>> refs/remotes/origin/master
		else
			f->next = NULL;
		/* make num_blocks into the version/length field */
		f->num_blocks = (FLASH_BLOCK_LIST_VERSION << 56) | ((f->num_blocks+1)*16);
	}

	printk(KERN_ALERT "FLASH: flash image is %ld bytes\n", image_size);
	printk(KERN_ALERT "FLASH: performing flash and reboot\n");
	rtas_progress("Flashing        \n", 0x0);
	rtas_progress("Please Wait...  ", 0x0);
	printk(KERN_ALERT "FLASH: this will take several minutes.  Do not power off!\n");
	status = rtas_call(update_token, 1, 1, NULL, rtas_block_list);
	switch (status) {	/* should only get "bad" status */
	    case 0:
		printk(KERN_ALERT "FLASH: success\n");
		break;
	    case -1:
		printk(KERN_ALERT "FLASH: hardware error.  Firmware may not be not flashed\n");
		break;
	    case -3:
		printk(KERN_ALERT "FLASH: image is corrupt or not correct for this platform.  Firmware not flashed\n");
		break;
	    case -4:
		printk(KERN_ALERT "FLASH: flash failed when partially complete.  System may not reboot\n");
		break;
	    default:
		printk(KERN_ALERT "FLASH: unknown flash return code %d\n", status);
		break;
	}
	spin_unlock(&rtas_data_buf_lock);
}

<<<<<<< HEAD
static void remove_flash_pde(struct proc_dir_entry *dp)
{
	if (dp) {
		kfree(dp->data);
		remove_proc_entry(dp->name, dp->parent);
	}
}

static int initialize_flash_pde_data(const char *rtas_call_name,
				     size_t buf_size,
				     struct proc_dir_entry *dp)
{
	int *status;
	int token;

	dp->data = kzalloc(buf_size, GFP_KERNEL);
	if (dp->data == NULL) {
		remove_flash_pde(dp);
		return -ENOMEM;
	}

	/*
	 * This code assumes that the status int is the first member of the
	 * struct 
	 */
	status = (int *) dp->data;
	token = rtas_token(rtas_call_name);
	if (token == RTAS_UNKNOWN_SERVICE)
		*status = FLASH_AUTH;
	else
		*status = FLASH_NO_OP;

	return 0;
}

static struct proc_dir_entry *create_flash_pde(const char *filename,
					       const struct file_operations *fops)
{
	return proc_create(filename, S_IRUSR | S_IWUSR, NULL, fops);
}

static const struct file_operations rtas_flash_operations = {
	.owner		= THIS_MODULE,
	.read		= rtas_flash_read,
	.write		= rtas_flash_write,
	.open		= rtas_excl_open,
	.release	= rtas_flash_release,
	.llseek		= default_llseek,
};

static const struct file_operations manage_flash_operations = {
	.owner		= THIS_MODULE,
	.read		= manage_flash_read,
	.write		= manage_flash_write,
	.open		= rtas_excl_open,
	.release	= rtas_excl_release,
	.llseek		= default_llseek,
};

static const struct file_operations validate_flash_operations = {
	.owner		= THIS_MODULE,
	.read		= validate_flash_read,
	.write		= validate_flash_write,
	.open		= rtas_excl_open,
	.release	= validate_flash_release,
	.llseek		= default_llseek,
=======
/*
 * Manifest of proc files to create
 */
struct rtas_flash_file {
	const char *filename;
	const char *rtas_call_name;
	int *status;
	const struct file_operations fops;
};

static const struct rtas_flash_file rtas_flash_files[] = {
	{
		.filename	= "powerpc/rtas/" FIRMWARE_FLASH_NAME,
		.rtas_call_name	= "ibm,update-flash-64-and-reboot",
		.status		= &rtas_update_flash_data.status,
		.fops.read	= rtas_flash_read_msg,
		.fops.write	= rtas_flash_write,
		.fops.release	= rtas_flash_release,
		.fops.llseek	= default_llseek,
	},
	{
		.filename	= "powerpc/rtas/" FIRMWARE_UPDATE_NAME,
		.rtas_call_name	= "ibm,update-flash-64-and-reboot",
		.status		= &rtas_update_flash_data.status,
		.fops.read	= rtas_flash_read_num,
		.fops.write	= rtas_flash_write,
		.fops.release	= rtas_flash_release,
		.fops.llseek	= default_llseek,
	},
	{
		.filename	= "powerpc/rtas/" VALIDATE_FLASH_NAME,
		.rtas_call_name	= "ibm,validate-flash-image",
		.status		= &rtas_validate_flash_data.status,
		.fops.read	= validate_flash_read,
		.fops.write	= validate_flash_write,
		.fops.release	= validate_flash_release,
		.fops.llseek	= default_llseek,
	},
	{
		.filename	= "powerpc/rtas/" MANAGE_FLASH_NAME,
		.rtas_call_name	= "ibm,manage-flash-image",
		.status		= &rtas_manage_flash_data.status,
		.fops.read	= manage_flash_read,
		.fops.write	= manage_flash_write,
		.fops.llseek	= default_llseek,
	}
>>>>>>> refs/remotes/origin/master
};

static int __init rtas_flash_init(void)
{
<<<<<<< HEAD
	int rc;

	if (rtas_token("ibm,update-flash-64-and-reboot") ==
		       RTAS_UNKNOWN_SERVICE) {
		printk(KERN_ERR "rtas_flash: no firmware flash support\n");
		return 1;
	}

	firmware_flash_pde = create_flash_pde("powerpc/rtas/"
					      FIRMWARE_FLASH_NAME,
					      &rtas_flash_operations);
	if (firmware_flash_pde == NULL) {
		rc = -ENOMEM;
		goto cleanup;
	}

	rc = initialize_flash_pde_data("ibm,update-flash-64-and-reboot",
			 	       sizeof(struct rtas_update_flash_t), 
				       firmware_flash_pde);
	if (rc != 0)
		goto cleanup;

	firmware_update_pde = create_flash_pde("powerpc/rtas/"
					       FIRMWARE_UPDATE_NAME,
					       &rtas_flash_operations);
	if (firmware_update_pde == NULL) {
		rc = -ENOMEM;
		goto cleanup;
	}

	rc = initialize_flash_pde_data("ibm,update-flash-64-and-reboot",
			 	       sizeof(struct rtas_update_flash_t), 
				       firmware_update_pde);
	if (rc != 0)
		goto cleanup;

	validate_pde = create_flash_pde("powerpc/rtas/" VALIDATE_FLASH_NAME,
			      		&validate_flash_operations);
	if (validate_pde == NULL) {
		rc = -ENOMEM;
		goto cleanup;
	}

	rc = initialize_flash_pde_data("ibm,validate-flash-image",
		                       sizeof(struct rtas_validate_flash_t), 
				       validate_pde);
	if (rc != 0)
		goto cleanup;

	manage_pde = create_flash_pde("powerpc/rtas/" MANAGE_FLASH_NAME,
				      &manage_flash_operations);
	if (manage_pde == NULL) {
		rc = -ENOMEM;
		goto cleanup;
	}

	rc = initialize_flash_pde_data("ibm,manage-flash-image",
			               sizeof(struct rtas_manage_flash_t),
				       manage_pde);
	if (rc != 0)
		goto cleanup;

	rtas_flash_term_hook = rtas_flash_firmware;

	flash_block_cache = kmem_cache_create("rtas_flash_cache",
				RTAS_BLK_SIZE, RTAS_BLK_SIZE, 0,
				rtas_block_ctor);
	if (!flash_block_cache) {
		printk(KERN_ERR "%s: failed to create block cache\n",
				__func__);
		rc = -ENOMEM;
		goto cleanup;
	}
	return 0;

cleanup:
	remove_flash_pde(firmware_flash_pde);
	remove_flash_pde(firmware_update_pde);
	remove_flash_pde(validate_pde);
	remove_flash_pde(manage_pde);

	return rc;
=======
	int i;

	if (rtas_token("ibm,update-flash-64-and-reboot") ==
		       RTAS_UNKNOWN_SERVICE) {
		pr_info("rtas_flash: no firmware flash support\n");
		return 1;
	}

	rtas_validate_flash_data.buf = kzalloc(VALIDATE_BUF_SIZE, GFP_KERNEL);
	if (!rtas_validate_flash_data.buf)
		return -ENOMEM;

	flash_block_cache = kmem_cache_create("rtas_flash_cache",
					      RTAS_BLK_SIZE, RTAS_BLK_SIZE, 0,
					      NULL);
	if (!flash_block_cache) {
		printk(KERN_ERR "%s: failed to create block cache\n",
				__func__);
		goto enomem_buf;
	}

	for (i = 0; i < ARRAY_SIZE(rtas_flash_files); i++) {
		const struct rtas_flash_file *f = &rtas_flash_files[i];
		int token;

		if (!proc_create(f->filename, S_IRUSR | S_IWUSR, NULL, &f->fops))
			goto enomem;

		/*
		 * This code assumes that the status int is the first member of the
		 * struct
		 */
		token = rtas_token(f->rtas_call_name);
		if (token == RTAS_UNKNOWN_SERVICE)
			*f->status = FLASH_AUTH;
		else
			*f->status = FLASH_NO_OP;
	}

	rtas_flash_term_hook = rtas_flash_firmware;
	return 0;

enomem:
	while (--i >= 0) {
		const struct rtas_flash_file *f = &rtas_flash_files[i];
		remove_proc_entry(f->filename, NULL);
	}

	kmem_cache_destroy(flash_block_cache);
enomem_buf:
	kfree(rtas_validate_flash_data.buf);
	return -ENOMEM;
>>>>>>> refs/remotes/origin/master
}

static void __exit rtas_flash_cleanup(void)
{
<<<<<<< HEAD
	rtas_flash_term_hook = NULL;

	if (flash_block_cache)
		kmem_cache_destroy(flash_block_cache);

	remove_flash_pde(firmware_flash_pde);
	remove_flash_pde(firmware_update_pde);
	remove_flash_pde(validate_pde);
	remove_flash_pde(manage_pde);
=======
	int i;

	rtas_flash_term_hook = NULL;

	if (rtas_firmware_flash_list) {
		free_flash_list(rtas_firmware_flash_list);
		rtas_firmware_flash_list = NULL;
	}

	for (i = 0; i < ARRAY_SIZE(rtas_flash_files); i++) {
		const struct rtas_flash_file *f = &rtas_flash_files[i];
		remove_proc_entry(f->filename, NULL);
	}

	kmem_cache_destroy(flash_block_cache);
	kfree(rtas_validate_flash_data.buf);
>>>>>>> refs/remotes/origin/master
}

module_init(rtas_flash_init);
module_exit(rtas_flash_cleanup);
MODULE_LICENSE("GPL");
