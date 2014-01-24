/*
 * f_fs.c -- user mode file system API for USB composite function controllers
 *
 * Copyright (C) 2010 Samsung Electronics
<<<<<<< HEAD
<<<<<<< HEAD
 * Author: Michal Nazarewicz <m.nazarewicz@samsung.com>
=======
 * Author: Michal Nazarewicz <mina86@mina86.com>
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Author: Michal Nazarewicz <mina86@mina86.com>
>>>>>>> refs/remotes/origin/master
 *
 * Based on inode.c (GadgetFS) which was:
 * Copyright (C) 2003-2004 David Brownell
 * Copyright (C) 2003 Agilent Technologies
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
<<<<<<< HEAD
<<<<<<< HEAD
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 */


/* #define DEBUG */
/* #define VERBOSE_DEBUG */

#include <linux/blkdev.h>
#include <linux/pagemap.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/export.h>
#include <linux/hid.h>
#include <linux/module.h>
>>>>>>> refs/remotes/origin/master
#include <asm/unaligned.h>

#include <linux/usb/composite.h>
#include <linux/usb/functionfs.h>

<<<<<<< HEAD

#define FUNCTIONFS_MAGIC	0xa647361 /* Chosen by a honest dice roll ;) */


/* Debugging ****************************************************************/

#ifdef VERBOSE_DEBUG
#  define pr_vdebug pr_debug
#  define ffs_dump_mem(prefix, ptr, len) \
	print_hex_dump_bytes(pr_fmt(prefix ": "), DUMP_PREFIX_NONE, ptr, len)
#else
#  define pr_vdebug(...)                 do { } while (0)
#  define ffs_dump_mem(prefix, ptr, len) do { } while (0)
#endif /* VERBOSE_DEBUG */

#define ENTER()    pr_vdebug("%s()\n", __func__)


/* The data structure and setup file ****************************************/

enum ffs_state {
	/*
	 * Waiting for descriptors and strings.
	 *
	 * In this state no open(2), read(2) or write(2) on epfiles
	 * may succeed (which should not be the problem as there
	 * should be no such files opened in the first place).
	 */
	FFS_READ_DESCRIPTORS,
	FFS_READ_STRINGS,

	/*
	 * We've got descriptors and strings.  We are or have called
	 * functionfs_ready_callback().  functionfs_bind() may have
	 * been called but we don't know.
	 *
	 * This is the only state in which operations on epfiles may
	 * succeed.
	 */
	FFS_ACTIVE,

	/*
	 * All endpoints have been closed.  This state is also set if
	 * we encounter an unrecoverable error.  The only
	 * unrecoverable error is situation when after reading strings
	 * from user space we fail to initialise epfiles or
	 * functionfs_ready_callback() returns with error (<0).
	 *
	 * In this state no open(2), read(2) or write(2) (both on ep0
	 * as well as epfile) may succeed (at this point epfiles are
	 * unlinked and all closed so this is not a problem; ep0 is
	 * also closed but ep0 file exists and so open(2) on ep0 must
	 * fail).
	 */
	FFS_CLOSING
};


enum ffs_setup_state {
	/* There is no setup request pending. */
	FFS_NO_SETUP,
	/*
	 * User has read events and there was a setup request event
	 * there.  The next read/write on ep0 will handle the
	 * request.
	 */
	FFS_SETUP_PENDING,
	/*
	 * There was event pending but before user space handled it
	 * some other event was introduced which canceled existing
	 * setup.  If this state is set read/write on ep0 return
	 * -EIDRM.  This state is only set when adding event.
	 */
	FFS_SETUP_CANCELED
};



struct ffs_epfile;
struct ffs_function;

struct ffs_data {
	struct usb_gadget		*gadget;

	/*
	 * Protect access read/write operations, only one read/write
	 * at a time.  As a consequence protects ep0req and company.
	 * While setup request is being processed (queued) this is
	 * held.
	 */
	struct mutex			mutex;

	/*
	 * Protect access to endpoint related structures (basically
	 * usb_ep_queue(), usb_ep_dequeue(), etc. calls) except for
	 * endpoint zero.
	 */
	spinlock_t			eps_lock;

	/*
	 * XXX REVISIT do we need our own request? Since we are not
	 * handling setup requests immediately user space may be so
	 * slow that another setup will be sent to the gadget but this
	 * time not to us but another function and then there could be
	 * a race.  Is that the case? Or maybe we can use cdev->req
	 * after all, maybe we just need some spinlock for that?
	 */
	struct usb_request		*ep0req;		/* P: mutex */
	struct completion		ep0req_completion;	/* P: mutex */
	int				ep0req_status;		/* P: mutex */

	/* reference counter */
	atomic_t			ref;
	/* how many files are opened (EP0 and others) */
	atomic_t			opened;

	/* EP0 state */
	enum ffs_state			state;

	/*
	 * Possible transitions:
	 * + FFS_NO_SETUP       -> FFS_SETUP_PENDING  -- P: ev.waitq.lock
	 *               happens only in ep0 read which is P: mutex
	 * + FFS_SETUP_PENDING  -> FFS_NO_SETUP       -- P: ev.waitq.lock
	 *               happens only in ep0 i/o  which is P: mutex
	 * + FFS_SETUP_PENDING  -> FFS_SETUP_CANCELED -- P: ev.waitq.lock
	 * + FFS_SETUP_CANCELED -> FFS_NO_SETUP       -- cmpxchg
	 */
	enum ffs_setup_state		setup_state;

#define FFS_SETUP_STATE(ffs)					\
	((enum ffs_setup_state)cmpxchg(&(ffs)->setup_state,	\
				       FFS_SETUP_CANCELED, FFS_NO_SETUP))

	/* Events & such. */
	struct {
		u8				types[4];
		unsigned short			count;
		/* XXX REVISIT need to update it in some places, or do we? */
		unsigned short			can_stall;
		struct usb_ctrlrequest		setup;

		wait_queue_head_t		waitq;
	} ev; /* the whole structure, P: ev.waitq.lock */

	/* Flags */
	unsigned long			flags;
#define FFS_FL_CALL_CLOSED_CALLBACK 0
#define FFS_FL_BOUND                1

	/* Active function */
	struct ffs_function		*func;

	/*
	 * Device name, write once when file system is mounted.
	 * Intended for user to read if she wants.
	 */
	const char			*dev_name;
	/* Private data for our user (ie. gadget).  Managed by user. */
	void				*private_data;

	/* filled by __ffs_data_got_descs() */
	/*
	 * Real descriptors are 16 bytes after raw_descs (so you need
	 * to skip 16 bytes (ie. ffs->raw_descs + 16) to get to the
	 * first full speed descriptor).  raw_descs_length and
	 * raw_fs_descs_length do not have those 16 bytes added.
	 */
	const void			*raw_descs;
	unsigned			raw_descs_length;
	unsigned			raw_fs_descs_length;
	unsigned			fs_descs_count;
	unsigned			hs_descs_count;

	unsigned short			strings_count;
	unsigned short			interfaces_count;
	unsigned short			eps_count;
	unsigned short			_pad1;

	/* filled by __ffs_data_got_strings() */
	/* ids in stringtabs are set in functionfs_bind() */
	const void			*raw_strings;
	struct usb_gadget_strings	**stringtabs;

	/*
	 * File system's super block, write once when file system is
	 * mounted.
	 */
	struct super_block		*sb;

	/* File permissions, written once when fs is mounted */
	struct ffs_file_perms {
		umode_t				mode;
		uid_t				uid;
		gid_t				gid;
	}				file_perms;

	/*
	 * The endpoint files, filled by ffs_epfiles_create(),
	 * destroyed by ffs_epfiles_destroy().
	 */
	struct ffs_epfile		*epfiles;
};
=======
#include "u_fs.h"
#include "configfs.h"

#define FUNCTIONFS_MAGIC	0xa647361 /* Chosen by a honest dice roll ;) */

/* Variable Length Array Macros **********************************************/
#define vla_group(groupname) size_t groupname##__next = 0
#define vla_group_size(groupname) groupname##__next

#define vla_item(groupname, type, name, n) \
	size_t groupname##_##name##__offset = ({			       \
		size_t align_mask = __alignof__(type) - 1;		       \
		size_t offset = (groupname##__next + align_mask) & ~align_mask;\
		size_t size = (n) * sizeof(type);			       \
		groupname##__next = offset + size;			       \
		offset;							       \
	})

#define vla_item_with_sz(groupname, type, name, n) \
	size_t groupname##_##name##__sz = (n) * sizeof(type);		       \
	size_t groupname##_##name##__offset = ({			       \
		size_t align_mask = __alignof__(type) - 1;		       \
		size_t offset = (groupname##__next + align_mask) & ~align_mask;\
		size_t size = groupname##_##name##__sz;			       \
		groupname##__next = offset + size;			       \
		offset;							       \
	})

#define vla_ptr(ptr, groupname, name) \
	((void *) ((char *)ptr + groupname##_##name##__offset))
>>>>>>> refs/remotes/origin/master

/* Reference counter handling */
static void ffs_data_get(struct ffs_data *ffs);
static void ffs_data_put(struct ffs_data *ffs);
/* Creates new ffs_data object. */
static struct ffs_data *__must_check ffs_data_new(void) __attribute__((malloc));

/* Opened counter handling. */
static void ffs_data_opened(struct ffs_data *ffs);
static void ffs_data_closed(struct ffs_data *ffs);

/* Called with ffs->mutex held; take over ownership of data. */
static int __must_check
__ffs_data_got_descs(struct ffs_data *ffs, char *data, size_t len);
static int __must_check
__ffs_data_got_strings(struct ffs_data *ffs, char *data, size_t len);


/* The function structure ***************************************************/

struct ffs_ep;

struct ffs_function {
	struct usb_configuration	*conf;
	struct usb_gadget		*gadget;
	struct ffs_data			*ffs;

	struct ffs_ep			*eps;
	u8				eps_revmap[16];
	short				*interfaces_nums;

	struct usb_function		function;
};


static struct ffs_function *ffs_func_from_usb(struct usb_function *f)
{
	return container_of(f, struct ffs_function, function);
}

<<<<<<< HEAD
static void ffs_func_free(struct ffs_function *func);
=======
>>>>>>> refs/remotes/origin/master

static void ffs_func_eps_disable(struct ffs_function *func);
static int __must_check ffs_func_eps_enable(struct ffs_function *func);

static int ffs_func_bind(struct usb_configuration *,
			 struct usb_function *);
<<<<<<< HEAD
static void ffs_func_unbind(struct usb_configuration *,
			    struct usb_function *);
=======
>>>>>>> refs/remotes/origin/master
static int ffs_func_set_alt(struct usb_function *, unsigned, unsigned);
static void ffs_func_disable(struct usb_function *);
static int ffs_func_setup(struct usb_function *,
			  const struct usb_ctrlrequest *);
static void ffs_func_suspend(struct usb_function *);
static void ffs_func_resume(struct usb_function *);


static int ffs_func_revmap_ep(struct ffs_function *func, u8 num);
static int ffs_func_revmap_intf(struct ffs_function *func, u8 intf);


/* The endpoints structures *************************************************/

struct ffs_ep {
	struct usb_ep			*ep;	/* P: ffs->eps_lock */
	struct usb_request		*req;	/* P: epfile->mutex */

	/* [0]: full speed, [1]: high speed */
	struct usb_endpoint_descriptor	*descs[2];

	u8				num;

	int				status;	/* P: epfile->mutex */
};

struct ffs_epfile {
	/* Protects ep->ep and ep->req. */
	struct mutex			mutex;
	wait_queue_head_t		wait;

	struct ffs_data			*ffs;
	struct ffs_ep			*ep;	/* P: ffs->eps_lock */

	struct dentry			*dentry;

	char				name[5];

	unsigned char			in;	/* P: ffs->eps_lock */
	unsigned char			isoc;	/* P: ffs->eps_lock */

	unsigned char			_pad;
};

static int  __must_check ffs_epfiles_create(struct ffs_data *ffs);
static void ffs_epfiles_destroy(struct ffs_epfile *epfiles, unsigned count);

static struct inode *__must_check
ffs_sb_create_file(struct super_block *sb, const char *name, void *data,
		   const struct file_operations *fops,
		   struct dentry **dentry_p);

<<<<<<< HEAD
=======
/* Devices management *******************************************************/

DEFINE_MUTEX(ffs_lock);
EXPORT_SYMBOL(ffs_lock);

static struct ffs_dev *ffs_find_dev(const char *name);
static int _ffs_name_dev(struct ffs_dev *dev, const char *name);
static void *ffs_acquire_dev(const char *dev_name);
static void ffs_release_dev(struct ffs_data *ffs_data);
static int ffs_ready(struct ffs_data *ffs);
static void ffs_closed(struct ffs_data *ffs);
>>>>>>> refs/remotes/origin/master

/* Misc helper functions ****************************************************/

static int ffs_mutex_lock(struct mutex *mutex, unsigned nonblock)
	__attribute__((warn_unused_result, nonnull));
<<<<<<< HEAD
static char *ffs_prepare_buffer(const char * __user buf, size_t len)
=======
static char *ffs_prepare_buffer(const char __user *buf, size_t len)
>>>>>>> refs/remotes/origin/master
	__attribute__((warn_unused_result, nonnull));


/* Control file aka ep0 *****************************************************/

static void ffs_ep0_complete(struct usb_ep *ep, struct usb_request *req)
{
	struct ffs_data *ffs = req->context;

	complete_all(&ffs->ep0req_completion);
}

static int __ffs_ep0_queue_wait(struct ffs_data *ffs, char *data, size_t len)
{
	struct usb_request *req = ffs->ep0req;
	int ret;

	req->zero     = len < le16_to_cpu(ffs->ev.setup.wLength);

	spin_unlock_irq(&ffs->ev.waitq.lock);

	req->buf      = data;
	req->length   = len;

	/*
	 * UDC layer requires to provide a buffer even for ZLP, but should
	 * not use it at all. Let's provide some poisoned pointer to catch
	 * possible bug in the driver.
	 */
	if (req->buf == NULL)
		req->buf = (void *)0xDEADBABE;

<<<<<<< HEAD
	INIT_COMPLETION(ffs->ep0req_completion);
=======
	reinit_completion(&ffs->ep0req_completion);
>>>>>>> refs/remotes/origin/master

	ret = usb_ep_queue(ffs->gadget->ep0, req, GFP_ATOMIC);
	if (unlikely(ret < 0))
		return ret;

	ret = wait_for_completion_interruptible(&ffs->ep0req_completion);
	if (unlikely(ret)) {
		usb_ep_dequeue(ffs->gadget->ep0, req);
		return -EINTR;
	}

	ffs->setup_state = FFS_NO_SETUP;
	return ffs->ep0req_status;
}

static int __ffs_ep0_stall(struct ffs_data *ffs)
{
	if (ffs->ev.can_stall) {
		pr_vdebug("ep0 stall\n");
		usb_ep_set_halt(ffs->gadget->ep0);
		ffs->setup_state = FFS_NO_SETUP;
		return -EL2HLT;
	} else {
		pr_debug("bogus ep0 stall!\n");
		return -ESRCH;
	}
}

static ssize_t ffs_ep0_write(struct file *file, const char __user *buf,
			     size_t len, loff_t *ptr)
{
	struct ffs_data *ffs = file->private_data;
	ssize_t ret;
	char *data;

	ENTER();

	/* Fast check if setup was canceled */
	if (FFS_SETUP_STATE(ffs) == FFS_SETUP_CANCELED)
		return -EIDRM;

	/* Acquire mutex */
	ret = ffs_mutex_lock(&ffs->mutex, file->f_flags & O_NONBLOCK);
	if (unlikely(ret < 0))
		return ret;

	/* Check state */
	switch (ffs->state) {
	case FFS_READ_DESCRIPTORS:
	case FFS_READ_STRINGS:
		/* Copy data */
		if (unlikely(len < 16)) {
			ret = -EINVAL;
			break;
		}

		data = ffs_prepare_buffer(buf, len);
		if (IS_ERR(data)) {
			ret = PTR_ERR(data);
			break;
		}

		/* Handle data */
		if (ffs->state == FFS_READ_DESCRIPTORS) {
			pr_info("read descriptors\n");
			ret = __ffs_data_got_descs(ffs, data, len);
			if (unlikely(ret < 0))
				break;

			ffs->state = FFS_READ_STRINGS;
			ret = len;
		} else {
			pr_info("read strings\n");
			ret = __ffs_data_got_strings(ffs, data, len);
			if (unlikely(ret < 0))
				break;

			ret = ffs_epfiles_create(ffs);
			if (unlikely(ret)) {
				ffs->state = FFS_CLOSING;
				break;
			}

			ffs->state = FFS_ACTIVE;
			mutex_unlock(&ffs->mutex);

<<<<<<< HEAD
			ret = functionfs_ready_callback(ffs);
=======
			ret = ffs_ready(ffs);
>>>>>>> refs/remotes/origin/master
			if (unlikely(ret < 0)) {
				ffs->state = FFS_CLOSING;
				return ret;
			}

			set_bit(FFS_FL_CALL_CLOSED_CALLBACK, &ffs->flags);
			return len;
		}
		break;

	case FFS_ACTIVE:
		data = NULL;
		/*
		 * We're called from user space, we can use _irq
		 * rather then _irqsave
		 */
		spin_lock_irq(&ffs->ev.waitq.lock);
		switch (FFS_SETUP_STATE(ffs)) {
		case FFS_SETUP_CANCELED:
			ret = -EIDRM;
			goto done_spin;

		case FFS_NO_SETUP:
			ret = -ESRCH;
			goto done_spin;

		case FFS_SETUP_PENDING:
			break;
		}

		/* FFS_SETUP_PENDING */
		if (!(ffs->ev.setup.bRequestType & USB_DIR_IN)) {
			spin_unlock_irq(&ffs->ev.waitq.lock);
			ret = __ffs_ep0_stall(ffs);
			break;
		}

		/* FFS_SETUP_PENDING and not stall */
		len = min(len, (size_t)le16_to_cpu(ffs->ev.setup.wLength));

		spin_unlock_irq(&ffs->ev.waitq.lock);

		data = ffs_prepare_buffer(buf, len);
		if (IS_ERR(data)) {
			ret = PTR_ERR(data);
			break;
		}

		spin_lock_irq(&ffs->ev.waitq.lock);

		/*
		 * We are guaranteed to be still in FFS_ACTIVE state
		 * but the state of setup could have changed from
		 * FFS_SETUP_PENDING to FFS_SETUP_CANCELED so we need
		 * to check for that.  If that happened we copied data
		 * from user space in vain but it's unlikely.
		 *
		 * For sure we are not in FFS_NO_SETUP since this is
		 * the only place FFS_SETUP_PENDING -> FFS_NO_SETUP
		 * transition can be performed and it's protected by
		 * mutex.
		 */
		if (FFS_SETUP_STATE(ffs) == FFS_SETUP_CANCELED) {
			ret = -EIDRM;
done_spin:
			spin_unlock_irq(&ffs->ev.waitq.lock);
		} else {
			/* unlocks spinlock */
			ret = __ffs_ep0_queue_wait(ffs, data, len);
		}
		kfree(data);
		break;

	default:
		ret = -EBADFD;
		break;
	}

	mutex_unlock(&ffs->mutex);
	return ret;
}

static ssize_t __ffs_ep0_read_events(struct ffs_data *ffs, char __user *buf,
				     size_t n)
{
	/*
	 * We are holding ffs->ev.waitq.lock and ffs->mutex and we need
	 * to release them.
	 */
	struct usb_functionfs_event events[n];
	unsigned i = 0;

	memset(events, 0, sizeof events);

	do {
		events[i].type = ffs->ev.types[i];
		if (events[i].type == FUNCTIONFS_SETUP) {
			events[i].u.setup = ffs->ev.setup;
			ffs->setup_state = FFS_SETUP_PENDING;
		}
	} while (++i < n);

	if (n < ffs->ev.count) {
		ffs->ev.count -= n;
		memmove(ffs->ev.types, ffs->ev.types + n,
			ffs->ev.count * sizeof *ffs->ev.types);
	} else {
		ffs->ev.count = 0;
	}

	spin_unlock_irq(&ffs->ev.waitq.lock);
	mutex_unlock(&ffs->mutex);

	return unlikely(__copy_to_user(buf, events, sizeof events))
		? -EFAULT : sizeof events;
}

static ssize_t ffs_ep0_read(struct file *file, char __user *buf,
			    size_t len, loff_t *ptr)
{
	struct ffs_data *ffs = file->private_data;
	char *data = NULL;
	size_t n;
	int ret;

	ENTER();

	/* Fast check if setup was canceled */
	if (FFS_SETUP_STATE(ffs) == FFS_SETUP_CANCELED)
		return -EIDRM;

	/* Acquire mutex */
	ret = ffs_mutex_lock(&ffs->mutex, file->f_flags & O_NONBLOCK);
	if (unlikely(ret < 0))
		return ret;

	/* Check state */
	if (ffs->state != FFS_ACTIVE) {
		ret = -EBADFD;
		goto done_mutex;
	}

	/*
	 * We're called from user space, we can use _irq rather then
	 * _irqsave
	 */
	spin_lock_irq(&ffs->ev.waitq.lock);

	switch (FFS_SETUP_STATE(ffs)) {
	case FFS_SETUP_CANCELED:
		ret = -EIDRM;
		break;

	case FFS_NO_SETUP:
		n = len / sizeof(struct usb_functionfs_event);
		if (unlikely(!n)) {
			ret = -EINVAL;
			break;
		}

		if ((file->f_flags & O_NONBLOCK) && !ffs->ev.count) {
			ret = -EAGAIN;
			break;
		}

		if (wait_event_interruptible_exclusive_locked_irq(ffs->ev.waitq,
							ffs->ev.count)) {
			ret = -EINTR;
			break;
		}

		return __ffs_ep0_read_events(ffs, buf,
					     min(n, (size_t)ffs->ev.count));

	case FFS_SETUP_PENDING:
		if (ffs->ev.setup.bRequestType & USB_DIR_IN) {
			spin_unlock_irq(&ffs->ev.waitq.lock);
			ret = __ffs_ep0_stall(ffs);
			goto done_mutex;
		}

		len = min(len, (size_t)le16_to_cpu(ffs->ev.setup.wLength));

		spin_unlock_irq(&ffs->ev.waitq.lock);

		if (likely(len)) {
			data = kmalloc(len, GFP_KERNEL);
			if (unlikely(!data)) {
				ret = -ENOMEM;
				goto done_mutex;
			}
		}

		spin_lock_irq(&ffs->ev.waitq.lock);

		/* See ffs_ep0_write() */
		if (FFS_SETUP_STATE(ffs) == FFS_SETUP_CANCELED) {
			ret = -EIDRM;
			break;
		}

		/* unlocks spinlock */
		ret = __ffs_ep0_queue_wait(ffs, data, len);
		if (likely(ret > 0) && unlikely(__copy_to_user(buf, data, len)))
			ret = -EFAULT;
		goto done_mutex;

	default:
		ret = -EBADFD;
		break;
	}

	spin_unlock_irq(&ffs->ev.waitq.lock);
done_mutex:
	mutex_unlock(&ffs->mutex);
	kfree(data);
	return ret;
}

static int ffs_ep0_open(struct inode *inode, struct file *file)
{
	struct ffs_data *ffs = inode->i_private;

	ENTER();

	if (unlikely(ffs->state == FFS_CLOSING))
		return -EBUSY;

	file->private_data = ffs;
	ffs_data_opened(ffs);

	return 0;
}

static int ffs_ep0_release(struct inode *inode, struct file *file)
{
	struct ffs_data *ffs = file->private_data;

	ENTER();

	ffs_data_closed(ffs);

	return 0;
}

static long ffs_ep0_ioctl(struct file *file, unsigned code, unsigned long value)
{
	struct ffs_data *ffs = file->private_data;
	struct usb_gadget *gadget = ffs->gadget;
	long ret;

	ENTER();

	if (code == FUNCTIONFS_INTERFACE_REVMAP) {
		struct ffs_function *func = ffs->func;
		ret = func ? ffs_func_revmap_intf(func, value) : -ENODEV;
	} else if (gadget && gadget->ops->ioctl) {
		ret = gadget->ops->ioctl(gadget, code, value);
	} else {
		ret = -ENOTTY;
	}

	return ret;
}

static const struct file_operations ffs_ep0_operations = {
<<<<<<< HEAD
	.owner =	THIS_MODULE,
=======
>>>>>>> refs/remotes/origin/master
	.llseek =	no_llseek,

	.open =		ffs_ep0_open,
	.write =	ffs_ep0_write,
	.read =		ffs_ep0_read,
	.release =	ffs_ep0_release,
	.unlocked_ioctl =	ffs_ep0_ioctl,
};


/* "Normal" endpoints operations ********************************************/

static void ffs_epfile_io_complete(struct usb_ep *_ep, struct usb_request *req)
{
	ENTER();
	if (likely(req->context)) {
		struct ffs_ep *ep = _ep->driver_data;
		ep->status = req->status ? req->status : req->actual;
		complete(req->context);
	}
}

static ssize_t ffs_epfile_io(struct file *file,
			     char __user *buf, size_t len, int read)
{
	struct ffs_epfile *epfile = file->private_data;
<<<<<<< HEAD
	struct ffs_ep *ep;
	char *data = NULL;
	ssize_t ret;
	int halt;

	goto first_try;
	do {
		spin_unlock_irq(&epfile->ffs->eps_lock);
		mutex_unlock(&epfile->mutex);

first_try:
		/* Are we still active? */
		if (WARN_ON(epfile->ffs->state != FFS_ACTIVE)) {
			ret = -ENODEV;
			goto error;
		}

		/* Wait for endpoint to be enabled */
		ep = epfile->ep;
		if (!ep) {
			if (file->f_flags & O_NONBLOCK) {
				ret = -EAGAIN;
				goto error;
			}

			if (wait_event_interruptible(epfile->wait,
						     (ep = epfile->ep))) {
				ret = -EINTR;
				goto error;
			}
		}

		/* Do we halt? */
		halt = !read == !epfile->in;
		if (halt && epfile->isoc) {
			ret = -EINVAL;
			goto error;
		}

		/* Allocate & copy */
		if (!halt && !data) {
			data = kzalloc(len, GFP_KERNEL);
			if (unlikely(!data))
				return -ENOMEM;

			if (!read &&
			    unlikely(__copy_from_user(data, buf, len))) {
				ret = -EFAULT;
				goto error;
			}
		}

		/* We will be using request */
		ret = ffs_mutex_lock(&epfile->mutex,
				     file->f_flags & O_NONBLOCK);
		if (unlikely(ret))
			goto error;

		/*
		 * We're called from user space, we can use _irq rather then
		 * _irqsave
		 */
		spin_lock_irq(&epfile->ffs->eps_lock);

		/*
		 * While we were acquiring mutex endpoint got disabled
		 * or changed?
		 */
	} while (unlikely(epfile->ep != ep));

	/* Halt */
	if (unlikely(halt)) {
=======
	struct usb_gadget *gadget = epfile->ffs->gadget;
	struct ffs_ep *ep;
	char *data = NULL;
	ssize_t ret, data_len;
	int halt;

	/* Are we still active? */
	if (WARN_ON(epfile->ffs->state != FFS_ACTIVE)) {
		ret = -ENODEV;
		goto error;
	}

	/* Wait for endpoint to be enabled */
	ep = epfile->ep;
	if (!ep) {
		if (file->f_flags & O_NONBLOCK) {
			ret = -EAGAIN;
			goto error;
		}

		ret = wait_event_interruptible(epfile->wait, (ep = epfile->ep));
		if (ret) {
			ret = -EINTR;
			goto error;
		}
	}

	/* Do we halt? */
	halt = !read == !epfile->in;
	if (halt && epfile->isoc) {
		ret = -EINVAL;
		goto error;
	}

	/* Allocate & copy */
	if (!halt) {
		/*
		 * Controller may require buffer size to be aligned to
		 * maxpacketsize of an out endpoint.
		 */
		data_len = read ? usb_ep_align_maybe(gadget, ep->ep, len) : len;

		data = kmalloc(data_len, GFP_KERNEL);
		if (unlikely(!data))
			return -ENOMEM;

		if (!read && unlikely(copy_from_user(data, buf, len))) {
			ret = -EFAULT;
			goto error;
		}
	}

	/* We will be using request */
	ret = ffs_mutex_lock(&epfile->mutex, file->f_flags & O_NONBLOCK);
	if (unlikely(ret))
		goto error;

	spin_lock_irq(&epfile->ffs->eps_lock);

	if (epfile->ep != ep) {
		/* In the meantime, endpoint got disabled or changed. */
		ret = -ESHUTDOWN;
		spin_unlock_irq(&epfile->ffs->eps_lock);
	} else if (halt) {
		/* Halt */
>>>>>>> refs/remotes/origin/master
		if (likely(epfile->ep == ep) && !WARN_ON(!ep->ep))
			usb_ep_set_halt(ep->ep);
		spin_unlock_irq(&epfile->ffs->eps_lock);
		ret = -EBADMSG;
	} else {
		/* Fire the request */
		DECLARE_COMPLETION_ONSTACK(done);

		struct usb_request *req = ep->req;
		req->context  = &done;
		req->complete = ffs_epfile_io_complete;
		req->buf      = data;
<<<<<<< HEAD
		req->length   = len;
=======
		req->length   = data_len;
>>>>>>> refs/remotes/origin/master

		ret = usb_ep_queue(ep->ep, req, GFP_ATOMIC);

		spin_unlock_irq(&epfile->ffs->eps_lock);

		if (unlikely(ret < 0)) {
			/* nop */
		} else if (unlikely(wait_for_completion_interruptible(&done))) {
			ret = -EINTR;
			usb_ep_dequeue(ep->ep, req);
		} else {
<<<<<<< HEAD
			ret = ep->status;
			if (read && ret > 0 &&
			    unlikely(copy_to_user(buf, data, ret)))
=======
			/*
			 * XXX We may end up silently droping data here.
			 * Since data_len (i.e. req->length) may be bigger
			 * than len (after being rounded up to maxpacketsize),
			 * we may end up with more data then user space has
			 * space for.
			 */
			ret = ep->status;
			if (read && ret > 0 &&
			    unlikely(copy_to_user(buf, data,
						  min_t(size_t, ret, len))))
>>>>>>> refs/remotes/origin/master
				ret = -EFAULT;
		}
	}

	mutex_unlock(&epfile->mutex);
error:
	kfree(data);
	return ret;
}

static ssize_t
ffs_epfile_write(struct file *file, const char __user *buf, size_t len,
		 loff_t *ptr)
{
	ENTER();

	return ffs_epfile_io(file, (char __user *)buf, len, 0);
}

static ssize_t
ffs_epfile_read(struct file *file, char __user *buf, size_t len, loff_t *ptr)
{
	ENTER();

	return ffs_epfile_io(file, buf, len, 1);
}

static int
ffs_epfile_open(struct inode *inode, struct file *file)
{
	struct ffs_epfile *epfile = inode->i_private;

	ENTER();

	if (WARN_ON(epfile->ffs->state != FFS_ACTIVE))
		return -ENODEV;

	file->private_data = epfile;
	ffs_data_opened(epfile->ffs);

	return 0;
}

static int
ffs_epfile_release(struct inode *inode, struct file *file)
{
	struct ffs_epfile *epfile = inode->i_private;

	ENTER();

	ffs_data_closed(epfile->ffs);

	return 0;
}

static long ffs_epfile_ioctl(struct file *file, unsigned code,
			     unsigned long value)
{
	struct ffs_epfile *epfile = file->private_data;
	int ret;

	ENTER();

	if (WARN_ON(epfile->ffs->state != FFS_ACTIVE))
		return -ENODEV;

	spin_lock_irq(&epfile->ffs->eps_lock);
	if (likely(epfile->ep)) {
		switch (code) {
		case FUNCTIONFS_FIFO_STATUS:
			ret = usb_ep_fifo_status(epfile->ep->ep);
			break;
		case FUNCTIONFS_FIFO_FLUSH:
			usb_ep_fifo_flush(epfile->ep->ep);
			ret = 0;
			break;
		case FUNCTIONFS_CLEAR_HALT:
			ret = usb_ep_clear_halt(epfile->ep->ep);
			break;
		case FUNCTIONFS_ENDPOINT_REVMAP:
			ret = epfile->ep->num;
			break;
		default:
			ret = -ENOTTY;
		}
	} else {
		ret = -ENODEV;
	}
	spin_unlock_irq(&epfile->ffs->eps_lock);

	return ret;
}

static const struct file_operations ffs_epfile_operations = {
<<<<<<< HEAD
	.owner =	THIS_MODULE,
=======
>>>>>>> refs/remotes/origin/master
	.llseek =	no_llseek,

	.open =		ffs_epfile_open,
	.write =	ffs_epfile_write,
	.read =		ffs_epfile_read,
	.release =	ffs_epfile_release,
	.unlocked_ioctl =	ffs_epfile_ioctl,
};


/* File system and super block operations ***********************************/

/*
 * Mounting the file system creates a controller file, used first for
 * function configuration then later for event monitoring.
 */

static struct inode *__must_check
ffs_sb_make_inode(struct super_block *sb, void *data,
		  const struct file_operations *fops,
		  const struct inode_operations *iops,
		  struct ffs_file_perms *perms)
{
	struct inode *inode;

	ENTER();

	inode = new_inode(sb);

	if (likely(inode)) {
		struct timespec current_time = CURRENT_TIME;

		inode->i_ino	 = get_next_ino();
		inode->i_mode    = perms->mode;
		inode->i_uid     = perms->uid;
		inode->i_gid     = perms->gid;
		inode->i_atime   = current_time;
		inode->i_mtime   = current_time;
		inode->i_ctime   = current_time;
		inode->i_private = data;
		if (fops)
			inode->i_fop = fops;
		if (iops)
			inode->i_op  = iops;
	}

	return inode;
}

/* Create "regular" file */
static struct inode *ffs_sb_create_file(struct super_block *sb,
					const char *name, void *data,
					const struct file_operations *fops,
					struct dentry **dentry_p)
{
	struct ffs_data	*ffs = sb->s_fs_info;
	struct dentry	*dentry;
	struct inode	*inode;

	ENTER();

	dentry = d_alloc_name(sb->s_root, name);
	if (unlikely(!dentry))
		return NULL;

	inode = ffs_sb_make_inode(sb, data, fops, NULL, &ffs->file_perms);
	if (unlikely(!inode)) {
		dput(dentry);
		return NULL;
	}

	d_add(dentry, inode);
	if (dentry_p)
		*dentry_p = dentry;

	return inode;
}

/* Super block */
static const struct super_operations ffs_sb_operations = {
	.statfs =	simple_statfs,
	.drop_inode =	generic_delete_inode,
};

struct ffs_sb_fill_data {
	struct ffs_file_perms perms;
	umode_t root_mode;
	const char *dev_name;
<<<<<<< HEAD
=======
	struct ffs_data *ffs_data;
>>>>>>> refs/remotes/origin/master
};

static int ffs_sb_fill(struct super_block *sb, void *_data, int silent)
{
	struct ffs_sb_fill_data *data = _data;
	struct inode	*inode;
<<<<<<< HEAD
<<<<<<< HEAD
	struct dentry	*d;
=======
>>>>>>> refs/remotes/origin/cm-10.0
	struct ffs_data	*ffs;

	ENTER();

	/* Initialise data */
	ffs = ffs_data_new();
	if (unlikely(!ffs))
<<<<<<< HEAD
		goto enomem0;
=======
		goto Enomem;
>>>>>>> refs/remotes/origin/cm-10.0

	ffs->sb              = sb;
	ffs->dev_name        = data->dev_name;
	ffs->file_perms      = data->perms;

=======
	struct ffs_data	*ffs = data->ffs_data;

	ENTER();

	ffs->sb              = sb;
	data->ffs_data       = NULL;
>>>>>>> refs/remotes/origin/master
	sb->s_fs_info        = ffs;
	sb->s_blocksize      = PAGE_CACHE_SIZE;
	sb->s_blocksize_bits = PAGE_CACHE_SHIFT;
	sb->s_magic          = FUNCTIONFS_MAGIC;
	sb->s_op             = &ffs_sb_operations;
	sb->s_time_gran      = 1;

	/* Root inode */
	data->perms.mode = data->root_mode;
	inode = ffs_sb_make_inode(sb, NULL,
				  &simple_dir_operations,
				  &simple_dir_inode_operations,
				  &data->perms);
<<<<<<< HEAD
<<<<<<< HEAD
	if (unlikely(!inode))
		goto enomem1;
	d = d_alloc_root(inode);
	if (unlikely(!d))
		goto enomem2;
	sb->s_root = d;
=======
	sb->s_root = d_make_root(inode);
	if (unlikely(!sb->s_root))
		goto Enomem;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	sb->s_root = d_make_root(inode);
	if (unlikely(!sb->s_root))
		return -ENOMEM;
>>>>>>> refs/remotes/origin/master

	/* EP0 file */
	if (unlikely(!ffs_sb_create_file(sb, "ep0", ffs,
					 &ffs_ep0_operations, NULL)))
<<<<<<< HEAD
<<<<<<< HEAD
		goto enomem3;

	return 0;

enomem3:
	dput(d);
enomem2:
	iput(inode);
enomem1:
	ffs_data_put(ffs);
enomem0:
=======
		goto Enomem;

	return 0;

Enomem:
>>>>>>> refs/remotes/origin/cm-10.0
	return -ENOMEM;
=======
		return -ENOMEM;

	return 0;
>>>>>>> refs/remotes/origin/master
}

static int ffs_fs_parse_opts(struct ffs_sb_fill_data *data, char *opts)
{
	ENTER();

	if (!opts || !*opts)
		return 0;

	for (;;) {
<<<<<<< HEAD
		char *end, *eq, *comma;
		unsigned long value;
=======
		unsigned long value;
		char *eq, *comma;
>>>>>>> refs/remotes/origin/master

		/* Option limit */
		comma = strchr(opts, ',');
		if (comma)
			*comma = 0;

		/* Value limit */
		eq = strchr(opts, '=');
		if (unlikely(!eq)) {
			pr_err("'=' missing in %s\n", opts);
			return -EINVAL;
		}
		*eq = 0;

		/* Parse value */
<<<<<<< HEAD
		value = simple_strtoul(eq + 1, &end, 0);
		if (unlikely(*end != ',' && *end != 0)) {
=======
		if (kstrtoul(eq + 1, 0, &value)) {
>>>>>>> refs/remotes/origin/master
			pr_err("%s: invalid value: %s\n", opts, eq + 1);
			return -EINVAL;
		}

		/* Interpret option */
		switch (eq - opts) {
		case 5:
			if (!memcmp(opts, "rmode", 5))
				data->root_mode  = (value & 0555) | S_IFDIR;
			else if (!memcmp(opts, "fmode", 5))
				data->perms.mode = (value & 0666) | S_IFREG;
			else
				goto invalid;
			break;

		case 4:
			if (!memcmp(opts, "mode", 4)) {
				data->root_mode  = (value & 0555) | S_IFDIR;
				data->perms.mode = (value & 0666) | S_IFREG;
			} else {
				goto invalid;
			}
			break;

		case 3:
<<<<<<< HEAD
			if (!memcmp(opts, "uid", 3))
				data->perms.uid = value;
			else if (!memcmp(opts, "gid", 3))
				data->perms.gid = value;
			else
				goto invalid;
=======
			if (!memcmp(opts, "uid", 3)) {
				data->perms.uid = make_kuid(current_user_ns(), value);
				if (!uid_valid(data->perms.uid)) {
					pr_err("%s: unmapped value: %lu\n", opts, value);
					return -EINVAL;
				}
			} else if (!memcmp(opts, "gid", 3)) {
				data->perms.gid = make_kgid(current_user_ns(), value);
				if (!gid_valid(data->perms.gid)) {
					pr_err("%s: unmapped value: %lu\n", opts, value);
					return -EINVAL;
				}
			} else {
				goto invalid;
			}
>>>>>>> refs/remotes/origin/master
			break;

		default:
invalid:
			pr_err("%s: invalid option\n", opts);
			return -EINVAL;
		}

		/* Next iteration */
		if (!comma)
			break;
		opts = comma + 1;
	}

	return 0;
}

/* "mount -t functionfs dev_name /dev/function" ends up here */

static struct dentry *
ffs_fs_mount(struct file_system_type *t, int flags,
	      const char *dev_name, void *opts)
{
	struct ffs_sb_fill_data data = {
		.perms = {
			.mode = S_IFREG | 0600,
<<<<<<< HEAD
			.uid = 0,
			.gid = 0
		},
		.root_mode = S_IFDIR | 0500,
	};
	int ret;

	ENTER();

	ret = functionfs_check_dev_callback(dev_name);
	if (unlikely(ret < 0))
		return ERR_PTR(ret);

=======
			.uid = GLOBAL_ROOT_UID,
			.gid = GLOBAL_ROOT_GID,
		},
		.root_mode = S_IFDIR | 0500,
	};
	struct dentry *rv;
	int ret;
	void *ffs_dev;
	struct ffs_data	*ffs;

	ENTER();

>>>>>>> refs/remotes/origin/master
	ret = ffs_fs_parse_opts(&data, opts);
	if (unlikely(ret < 0))
		return ERR_PTR(ret);

<<<<<<< HEAD
	data.dev_name = dev_name;
	return mount_single(t, flags, &data, ffs_sb_fill);
=======
	ffs = ffs_data_new();
	if (unlikely(!ffs))
		return ERR_PTR(-ENOMEM);
	ffs->file_perms = data.perms;

	ffs->dev_name = kstrdup(dev_name, GFP_KERNEL);
	if (unlikely(!ffs->dev_name)) {
		ffs_data_put(ffs);
		return ERR_PTR(-ENOMEM);
	}

	ffs_dev = ffs_acquire_dev(dev_name);
	if (IS_ERR(ffs_dev)) {
		ffs_data_put(ffs);
		return ERR_CAST(ffs_dev);
	}
	ffs->private_data = ffs_dev;
	data.ffs_data = ffs;

	rv = mount_nodev(t, flags, &data, ffs_sb_fill);
	if (IS_ERR(rv) && data.ffs_data) {
		ffs_release_dev(data.ffs_data);
		ffs_data_put(data.ffs_data);
	}
	return rv;
>>>>>>> refs/remotes/origin/master
}

static void
ffs_fs_kill_sb(struct super_block *sb)
{
<<<<<<< HEAD
<<<<<<< HEAD
	void *ptr;

	ENTER();

	kill_litter_super(sb);
	ptr = xchg(&sb->s_fs_info, NULL);
	if (ptr)
		ffs_data_put(ptr);
=======
	ENTER();

	kill_litter_super(sb);
	if (sb->s_fs_info)
		ffs_data_put(sb->s_fs_info);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ENTER();

	kill_litter_super(sb);
	if (sb->s_fs_info) {
		ffs_release_dev(sb->s_fs_info);
		ffs_data_put(sb->s_fs_info);
	}
>>>>>>> refs/remotes/origin/master
}

static struct file_system_type ffs_fs_type = {
	.owner		= THIS_MODULE,
	.name		= "functionfs",
	.mount		= ffs_fs_mount,
	.kill_sb	= ffs_fs_kill_sb,
};
<<<<<<< HEAD
=======
MODULE_ALIAS_FS("functionfs");
>>>>>>> refs/remotes/origin/master


/* Driver's main init/cleanup functions *************************************/

static int functionfs_init(void)
{
	int ret;

	ENTER();

	ret = register_filesystem(&ffs_fs_type);
	if (likely(!ret))
		pr_info("file system registered\n");
	else
		pr_err("failed registering file system (%d)\n", ret);

	return ret;
}

static void functionfs_cleanup(void)
{
	ENTER();

	pr_info("unloading\n");
	unregister_filesystem(&ffs_fs_type);
}


/* ffs_data and ffs_function construction and destruction code **************/

static void ffs_data_clear(struct ffs_data *ffs);
static void ffs_data_reset(struct ffs_data *ffs);

static void ffs_data_get(struct ffs_data *ffs)
{
	ENTER();

	atomic_inc(&ffs->ref);
}

static void ffs_data_opened(struct ffs_data *ffs)
{
	ENTER();

	atomic_inc(&ffs->ref);
	atomic_inc(&ffs->opened);
}

static void ffs_data_put(struct ffs_data *ffs)
{
	ENTER();

	if (unlikely(atomic_dec_and_test(&ffs->ref))) {
		pr_info("%s(): freeing\n", __func__);
		ffs_data_clear(ffs);
<<<<<<< HEAD
<<<<<<< HEAD
		BUG_ON(mutex_is_locked(&ffs->mutex) ||
		       spin_is_locked(&ffs->ev.waitq.lock) ||
		       waitqueue_active(&ffs->ev.waitq) ||
=======
		BUG_ON(waitqueue_active(&ffs->ev.waitq) ||
>>>>>>> refs/remotes/origin/cm-10.0
		       waitqueue_active(&ffs->ep0req_completion.wait));
=======
		BUG_ON(waitqueue_active(&ffs->ev.waitq) ||
		       waitqueue_active(&ffs->ep0req_completion.wait));
		kfree(ffs->dev_name);
>>>>>>> refs/remotes/origin/master
		kfree(ffs);
	}
}

static void ffs_data_closed(struct ffs_data *ffs)
{
	ENTER();

	if (atomic_dec_and_test(&ffs->opened)) {
		ffs->state = FFS_CLOSING;
		ffs_data_reset(ffs);
	}

	ffs_data_put(ffs);
}

static struct ffs_data *ffs_data_new(void)
{
	struct ffs_data *ffs = kzalloc(sizeof *ffs, GFP_KERNEL);
	if (unlikely(!ffs))
<<<<<<< HEAD
		return 0;
=======
		return NULL;
>>>>>>> refs/remotes/origin/master

	ENTER();

	atomic_set(&ffs->ref, 1);
	atomic_set(&ffs->opened, 0);
	ffs->state = FFS_READ_DESCRIPTORS;
	mutex_init(&ffs->mutex);
	spin_lock_init(&ffs->eps_lock);
	init_waitqueue_head(&ffs->ev.waitq);
	init_completion(&ffs->ep0req_completion);

	/* XXX REVISIT need to update it in some places, or do we? */
	ffs->ev.can_stall = 1;

	return ffs;
}

static void ffs_data_clear(struct ffs_data *ffs)
{
	ENTER();

	if (test_and_clear_bit(FFS_FL_CALL_CLOSED_CALLBACK, &ffs->flags))
<<<<<<< HEAD
		functionfs_closed_callback(ffs);
=======
		ffs_closed(ffs);
>>>>>>> refs/remotes/origin/master

	BUG_ON(ffs->gadget);

	if (ffs->epfiles)
		ffs_epfiles_destroy(ffs->epfiles, ffs->eps_count);

	kfree(ffs->raw_descs);
	kfree(ffs->raw_strings);
	kfree(ffs->stringtabs);
}

static void ffs_data_reset(struct ffs_data *ffs)
{
	ENTER();

	ffs_data_clear(ffs);

	ffs->epfiles = NULL;
	ffs->raw_descs = NULL;
	ffs->raw_strings = NULL;
	ffs->stringtabs = NULL;

	ffs->raw_descs_length = 0;
	ffs->raw_fs_descs_length = 0;
	ffs->fs_descs_count = 0;
	ffs->hs_descs_count = 0;

	ffs->strings_count = 0;
	ffs->interfaces_count = 0;
	ffs->eps_count = 0;

	ffs->ev.count = 0;

	ffs->state = FFS_READ_DESCRIPTORS;
	ffs->setup_state = FFS_NO_SETUP;
	ffs->flags = 0;
}


static int functionfs_bind(struct ffs_data *ffs, struct usb_composite_dev *cdev)
{
	struct usb_gadget_strings **lang;
	int first_id;

	ENTER();

	if (WARN_ON(ffs->state != FFS_ACTIVE
		 || test_and_set_bit(FFS_FL_BOUND, &ffs->flags)))
		return -EBADFD;

	first_id = usb_string_ids_n(cdev, ffs->strings_count);
	if (unlikely(first_id < 0))
		return first_id;

	ffs->ep0req = usb_ep_alloc_request(cdev->gadget->ep0, GFP_KERNEL);
	if (unlikely(!ffs->ep0req))
		return -ENOMEM;
	ffs->ep0req->complete = ffs_ep0_complete;
	ffs->ep0req->context = ffs;

	lang = ffs->stringtabs;
	for (lang = ffs->stringtabs; *lang; ++lang) {
		struct usb_string *str = (*lang)->strings;
		int id = first_id;
		for (; str->s; ++id, ++str)
			str->id = id;
	}

	ffs->gadget = cdev->gadget;
	ffs_data_get(ffs);
	return 0;
}

static void functionfs_unbind(struct ffs_data *ffs)
{
	ENTER();

	if (!WARN_ON(!ffs->gadget)) {
		usb_ep_free_request(ffs->gadget->ep0, ffs->ep0req);
		ffs->ep0req = NULL;
		ffs->gadget = NULL;
<<<<<<< HEAD
		ffs_data_put(ffs);
<<<<<<< HEAD
=======
		clear_bit(FFS_FL_BOUND, &ffs->flags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		clear_bit(FFS_FL_BOUND, &ffs->flags);
		ffs_data_put(ffs);
>>>>>>> refs/remotes/origin/master
	}
}

static int ffs_epfiles_create(struct ffs_data *ffs)
{
	struct ffs_epfile *epfile, *epfiles;
	unsigned i, count;

	ENTER();

	count = ffs->eps_count;
<<<<<<< HEAD
<<<<<<< HEAD
	epfiles = kzalloc(count * sizeof *epfiles, GFP_KERNEL);
=======
	epfiles = kcalloc(count, sizeof(*epfiles), GFP_KERNEL);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	epfiles = kcalloc(count, sizeof(*epfiles), GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (!epfiles)
		return -ENOMEM;

	epfile = epfiles;
	for (i = 1; i <= count; ++i, ++epfile) {
		epfile->ffs = ffs;
		mutex_init(&epfile->mutex);
		init_waitqueue_head(&epfile->wait);
		sprintf(epfiles->name, "ep%u",  i);
		if (!unlikely(ffs_sb_create_file(ffs->sb, epfiles->name, epfile,
						 &ffs_epfile_operations,
						 &epfile->dentry))) {
			ffs_epfiles_destroy(epfiles, i - 1);
			return -ENOMEM;
		}
	}

	ffs->epfiles = epfiles;
	return 0;
}

static void ffs_epfiles_destroy(struct ffs_epfile *epfiles, unsigned count)
{
	struct ffs_epfile *epfile = epfiles;

	ENTER();

	for (; count; --count, ++epfile) {
		BUG_ON(mutex_is_locked(&epfile->mutex) ||
		       waitqueue_active(&epfile->wait));
		if (epfile->dentry) {
			d_delete(epfile->dentry);
			dput(epfile->dentry);
			epfile->dentry = NULL;
		}
	}

	kfree(epfiles);
}

<<<<<<< HEAD
static int functionfs_bind_config(struct usb_composite_dev *cdev,
				  struct usb_configuration *c,
				  struct ffs_data *ffs)
{
	struct ffs_function *func;
	int ret;

	ENTER();

	func = kzalloc(sizeof *func, GFP_KERNEL);
	if (unlikely(!func))
		return -ENOMEM;

	func->function.name    = "Function FS Gadget";
	func->function.strings = ffs->stringtabs;

	func->function.bind    = ffs_func_bind;
	func->function.unbind  = ffs_func_unbind;
	func->function.set_alt = ffs_func_set_alt;
	func->function.disable = ffs_func_disable;
	func->function.setup   = ffs_func_setup;
	func->function.suspend = ffs_func_suspend;
	func->function.resume  = ffs_func_resume;

	func->conf   = c;
	func->gadget = cdev->gadget;
	func->ffs = ffs;
	ffs_data_get(ffs);

	ret = usb_add_function(c, &func->function);
	if (unlikely(ret))
		ffs_func_free(func);

	return ret;
}

static void ffs_func_free(struct ffs_function *func)
{
	ENTER();

	ffs_data_put(func->ffs);

	kfree(func->eps);
	/*
	 * eps and interfaces_nums are allocated in the same chunk so
	 * only one free is required.  Descriptors are also allocated
	 * in the same chunk.
	 */

	kfree(func);
}
=======
>>>>>>> refs/remotes/origin/master

static void ffs_func_eps_disable(struct ffs_function *func)
{
	struct ffs_ep *ep         = func->eps;
	struct ffs_epfile *epfile = func->ffs->epfiles;
	unsigned count            = func->ffs->eps_count;
	unsigned long flags;

	spin_lock_irqsave(&func->ffs->eps_lock, flags);
	do {
		/* pending requests get nuked */
		if (likely(ep->ep))
			usb_ep_disable(ep->ep);
		epfile->ep = NULL;

		++ep;
		++epfile;
	} while (--count);
	spin_unlock_irqrestore(&func->ffs->eps_lock, flags);
}

static int ffs_func_eps_enable(struct ffs_function *func)
{
	struct ffs_data *ffs      = func->ffs;
	struct ffs_ep *ep         = func->eps;
	struct ffs_epfile *epfile = ffs->epfiles;
	unsigned count            = ffs->eps_count;
	unsigned long flags;
	int ret = 0;

	spin_lock_irqsave(&func->ffs->eps_lock, flags);
	do {
		struct usb_endpoint_descriptor *ds;
		ds = ep->descs[ep->descs[1] ? 1 : 0];

		ep->ep->driver_data = ep;
<<<<<<< HEAD
<<<<<<< HEAD
		ret = usb_ep_enable(ep->ep, ds);
=======
		ep->ep->desc = ds;
		ret = usb_ep_enable(ep->ep);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		ep->ep->desc = ds;
		ret = usb_ep_enable(ep->ep);
>>>>>>> refs/remotes/origin/master
		if (likely(!ret)) {
			epfile->ep = ep;
			epfile->in = usb_endpoint_dir_in(ds);
			epfile->isoc = usb_endpoint_xfer_isoc(ds);
		} else {
			break;
		}

		wake_up(&epfile->wait);

		++ep;
		++epfile;
	} while (--count);
	spin_unlock_irqrestore(&func->ffs->eps_lock, flags);

	return ret;
}


/* Parsing and building descriptors and strings *****************************/

/*
 * This validates if data pointed by data is a valid USB descriptor as
 * well as record how many interfaces, endpoints and strings are
 * required by given configuration.  Returns address after the
 * descriptor or NULL if data is invalid.
 */

enum ffs_entity_type {
	FFS_DESCRIPTOR, FFS_INTERFACE, FFS_STRING, FFS_ENDPOINT
};

typedef int (*ffs_entity_callback)(enum ffs_entity_type entity,
				   u8 *valuep,
				   struct usb_descriptor_header *desc,
				   void *priv);

static int __must_check ffs_do_desc(char *data, unsigned len,
				    ffs_entity_callback entity, void *priv)
{
	struct usb_descriptor_header *_ds = (void *)data;
	u8 length;
	int ret;

	ENTER();

	/* At least two bytes are required: length and type */
	if (len < 2) {
		pr_vdebug("descriptor too short\n");
		return -EINVAL;
	}

	/* If we have at least as many bytes as the descriptor takes? */
	length = _ds->bLength;
	if (len < length) {
		pr_vdebug("descriptor longer then available data\n");
		return -EINVAL;
	}

#define __entity_check_INTERFACE(val)  1
#define __entity_check_STRING(val)     (val)
#define __entity_check_ENDPOINT(val)   ((val) & USB_ENDPOINT_NUMBER_MASK)
#define __entity(type, val) do {					\
		pr_vdebug("entity " #type "(%02x)\n", (val));		\
		if (unlikely(!__entity_check_ ##type(val))) {		\
			pr_vdebug("invalid entity's value\n");		\
			return -EINVAL;					\
		}							\
		ret = entity(FFS_ ##type, &val, _ds, priv);		\
		if (unlikely(ret < 0)) {				\
			pr_debug("entity " #type "(%02x); ret = %d\n",	\
				 (val), ret);				\
			return ret;					\
		}							\
	} while (0)

	/* Parse descriptor depending on type. */
	switch (_ds->bDescriptorType) {
	case USB_DT_DEVICE:
	case USB_DT_CONFIG:
	case USB_DT_STRING:
	case USB_DT_DEVICE_QUALIFIER:
		/* function can't have any of those */
		pr_vdebug("descriptor reserved for gadget: %d\n",
		      _ds->bDescriptorType);
		return -EINVAL;

	case USB_DT_INTERFACE: {
		struct usb_interface_descriptor *ds = (void *)_ds;
		pr_vdebug("interface descriptor\n");
		if (length != sizeof *ds)
			goto inv_length;

		__entity(INTERFACE, ds->bInterfaceNumber);
		if (ds->iInterface)
			__entity(STRING, ds->iInterface);
	}
		break;

	case USB_DT_ENDPOINT: {
		struct usb_endpoint_descriptor *ds = (void *)_ds;
		pr_vdebug("endpoint descriptor\n");
		if (length != USB_DT_ENDPOINT_SIZE &&
		    length != USB_DT_ENDPOINT_AUDIO_SIZE)
			goto inv_length;
		__entity(ENDPOINT, ds->bEndpointAddress);
	}
		break;

<<<<<<< HEAD
=======
	case HID_DT_HID:
		pr_vdebug("hid descriptor\n");
		if (length != sizeof(struct hid_descriptor))
			goto inv_length;
		break;

>>>>>>> refs/remotes/origin/master
	case USB_DT_OTG:
		if (length != sizeof(struct usb_otg_descriptor))
			goto inv_length;
		break;

	case USB_DT_INTERFACE_ASSOCIATION: {
		struct usb_interface_assoc_descriptor *ds = (void *)_ds;
		pr_vdebug("interface association descriptor\n");
		if (length != sizeof *ds)
			goto inv_length;
		if (ds->iFunction)
			__entity(STRING, ds->iFunction);
	}
		break;

	case USB_DT_OTHER_SPEED_CONFIG:
	case USB_DT_INTERFACE_POWER:
	case USB_DT_DEBUG:
	case USB_DT_SECURITY:
	case USB_DT_CS_RADIO_CONTROL:
		/* TODO */
		pr_vdebug("unimplemented descriptor: %d\n", _ds->bDescriptorType);
		return -EINVAL;

	default:
		/* We should never be here */
		pr_vdebug("unknown descriptor: %d\n", _ds->bDescriptorType);
		return -EINVAL;

inv_length:
		pr_vdebug("invalid length: %d (descriptor %d)\n",
			  _ds->bLength, _ds->bDescriptorType);
		return -EINVAL;
	}

#undef __entity
#undef __entity_check_DESCRIPTOR
#undef __entity_check_INTERFACE
#undef __entity_check_STRING
#undef __entity_check_ENDPOINT

	return length;
}

static int __must_check ffs_do_descs(unsigned count, char *data, unsigned len,
				     ffs_entity_callback entity, void *priv)
{
	const unsigned _len = len;
	unsigned long num = 0;

	ENTER();

	for (;;) {
		int ret;

		if (num == count)
			data = NULL;

		/* Record "descriptor" entity */
		ret = entity(FFS_DESCRIPTOR, (u8 *)num, (void *)data, priv);
		if (unlikely(ret < 0)) {
			pr_debug("entity DESCRIPTOR(%02lx); ret = %d\n",
				 num, ret);
			return ret;
		}

		if (!data)
			return _len - len;

		ret = ffs_do_desc(data, len, entity, priv);
		if (unlikely(ret < 0)) {
			pr_debug("%s returns %d\n", __func__, ret);
			return ret;
		}

		len -= ret;
		data += ret;
		++num;
	}
}

static int __ffs_data_do_entity(enum ffs_entity_type type,
				u8 *valuep, struct usb_descriptor_header *desc,
				void *priv)
{
	struct ffs_data *ffs = priv;

	ENTER();

	switch (type) {
	case FFS_DESCRIPTOR:
		break;

	case FFS_INTERFACE:
		/*
		 * Interfaces are indexed from zero so if we
		 * encountered interface "n" then there are at least
		 * "n+1" interfaces.
		 */
		if (*valuep >= ffs->interfaces_count)
			ffs->interfaces_count = *valuep + 1;
		break;

	case FFS_STRING:
		/*
		 * Strings are indexed from 1 (0 is magic ;) reserved
		 * for languages list or some such)
		 */
		if (*valuep > ffs->strings_count)
			ffs->strings_count = *valuep;
		break;

	case FFS_ENDPOINT:
		/* Endpoints are indexed from 1 as well. */
		if ((*valuep & USB_ENDPOINT_NUMBER_MASK) > ffs->eps_count)
			ffs->eps_count = (*valuep & USB_ENDPOINT_NUMBER_MASK);
		break;
	}

	return 0;
}

static int __ffs_data_got_descs(struct ffs_data *ffs,
				char *const _data, size_t len)
{
	unsigned fs_count, hs_count;
	int fs_len, ret = -EINVAL;
	char *data = _data;

	ENTER();

	if (unlikely(get_unaligned_le32(data) != FUNCTIONFS_DESCRIPTORS_MAGIC ||
		     get_unaligned_le32(data + 4) != len))
		goto error;
	fs_count = get_unaligned_le32(data +  8);
	hs_count = get_unaligned_le32(data + 12);

	if (!fs_count && !hs_count)
		goto einval;

	data += 16;
	len  -= 16;

	if (likely(fs_count)) {
		fs_len = ffs_do_descs(fs_count, data, len,
				      __ffs_data_do_entity, ffs);
		if (unlikely(fs_len < 0)) {
			ret = fs_len;
			goto error;
		}

		data += fs_len;
		len  -= fs_len;
	} else {
		fs_len = 0;
	}

	if (likely(hs_count)) {
		ret = ffs_do_descs(hs_count, data, len,
				   __ffs_data_do_entity, ffs);
		if (unlikely(ret < 0))
			goto error;
	} else {
		ret = 0;
	}

	if (unlikely(len != ret))
		goto einval;

	ffs->raw_fs_descs_length = fs_len;
	ffs->raw_descs_length    = fs_len + ret;
	ffs->raw_descs           = _data;
	ffs->fs_descs_count      = fs_count;
	ffs->hs_descs_count      = hs_count;

	return 0;

einval:
	ret = -EINVAL;
error:
	kfree(_data);
	return ret;
}

static int __ffs_data_got_strings(struct ffs_data *ffs,
				  char *const _data, size_t len)
{
	u32 str_count, needed_count, lang_count;
	struct usb_gadget_strings **stringtabs, *t;
	struct usb_string *strings, *s;
	const char *data = _data;

	ENTER();

	if (unlikely(get_unaligned_le32(data) != FUNCTIONFS_STRINGS_MAGIC ||
		     get_unaligned_le32(data + 4) != len))
		goto error;
	str_count  = get_unaligned_le32(data + 8);
	lang_count = get_unaligned_le32(data + 12);

	/* if one is zero the other must be zero */
	if (unlikely(!str_count != !lang_count))
		goto error;

	/* Do we have at least as many strings as descriptors need? */
	needed_count = ffs->strings_count;
	if (unlikely(str_count < needed_count))
		goto error;

	/*
	 * If we don't need any strings just return and free all
	 * memory.
	 */
	if (!needed_count) {
		kfree(_data);
		return 0;
	}

	/* Allocate everything in one chunk so there's less maintenance. */
	{
<<<<<<< HEAD
		struct {
			struct usb_gadget_strings *stringtabs[lang_count + 1];
			struct usb_gadget_strings stringtab[lang_count];
			struct usb_string strings[lang_count*(needed_count+1)];
		} *d;
		unsigned i = 0;

		d = kmalloc(sizeof *d, GFP_KERNEL);
		if (unlikely(!d)) {
=======
		unsigned i = 0;
		vla_group(d);
		vla_item(d, struct usb_gadget_strings *, stringtabs,
			lang_count + 1);
		vla_item(d, struct usb_gadget_strings, stringtab, lang_count);
		vla_item(d, struct usb_string, strings,
			lang_count*(needed_count+1));

		char *vlabuf = kmalloc(vla_group_size(d), GFP_KERNEL);

		if (unlikely(!vlabuf)) {
>>>>>>> refs/remotes/origin/master
			kfree(_data);
			return -ENOMEM;
		}

<<<<<<< HEAD
		stringtabs = d->stringtabs;
		t = d->stringtab;
=======
		/* Initialize the VLA pointers */
		stringtabs = vla_ptr(vlabuf, d, stringtabs);
		t = vla_ptr(vlabuf, d, stringtab);
>>>>>>> refs/remotes/origin/master
		i = lang_count;
		do {
			*stringtabs++ = t++;
		} while (--i);
		*stringtabs = NULL;

<<<<<<< HEAD
		stringtabs = d->stringtabs;
		t = d->stringtab;
		s = d->strings;
=======
		/* stringtabs = vlabuf = d_stringtabs for later kfree */
		stringtabs = vla_ptr(vlabuf, d, stringtabs);
		t = vla_ptr(vlabuf, d, stringtab);
		s = vla_ptr(vlabuf, d, strings);
>>>>>>> refs/remotes/origin/master
		strings = s;
	}

	/* For each language */
	data += 16;
	len -= 16;

	do { /* lang_count > 0 so we can use do-while */
		unsigned needed = needed_count;

		if (unlikely(len < 3))
			goto error_free;
		t->language = get_unaligned_le16(data);
		t->strings  = s;
		++t;

		data += 2;
		len -= 2;

		/* For each string */
		do { /* str_count > 0 so we can use do-while */
			size_t length = strnlen(data, len);

			if (unlikely(length == len))
				goto error_free;

			/*
			 * User may provide more strings then we need,
			 * if that's the case we simply ignore the
			 * rest
			 */
			if (likely(needed)) {
				/*
				 * s->id will be set while adding
				 * function to configuration so for
				 * now just leave garbage here.
				 */
				s->s = data;
				--needed;
				++s;
			}

			data += length + 1;
			len -= length + 1;
		} while (--str_count);

		s->id = 0;   /* terminator */
		s->s = NULL;
		++s;

	} while (--lang_count);

	/* Some garbage left? */
	if (unlikely(len))
		goto error_free;

	/* Done! */
	ffs->stringtabs = stringtabs;
	ffs->raw_strings = _data;

	return 0;

error_free:
	kfree(stringtabs);
error:
	kfree(_data);
	return -EINVAL;
}


/* Events handling and management *******************************************/

static void __ffs_event_add(struct ffs_data *ffs,
			    enum usb_functionfs_event_type type)
{
	enum usb_functionfs_event_type rem_type1, rem_type2 = type;
	int neg = 0;

	/*
	 * Abort any unhandled setup
	 *
	 * We do not need to worry about some cmpxchg() changing value
	 * of ffs->setup_state without holding the lock because when
	 * state is FFS_SETUP_PENDING cmpxchg() in several places in
	 * the source does nothing.
	 */
	if (ffs->setup_state == FFS_SETUP_PENDING)
		ffs->setup_state = FFS_SETUP_CANCELED;

	switch (type) {
	case FUNCTIONFS_RESUME:
		rem_type2 = FUNCTIONFS_SUSPEND;
		/* FALL THROUGH */
	case FUNCTIONFS_SUSPEND:
	case FUNCTIONFS_SETUP:
		rem_type1 = type;
		/* Discard all similar events */
		break;

	case FUNCTIONFS_BIND:
	case FUNCTIONFS_UNBIND:
	case FUNCTIONFS_DISABLE:
	case FUNCTIONFS_ENABLE:
		/* Discard everything other then power management. */
		rem_type1 = FUNCTIONFS_SUSPEND;
		rem_type2 = FUNCTIONFS_RESUME;
		neg = 1;
		break;

	default:
		BUG();
	}

	{
		u8 *ev  = ffs->ev.types, *out = ev;
		unsigned n = ffs->ev.count;
		for (; n; --n, ++ev)
			if ((*ev == rem_type1 || *ev == rem_type2) == neg)
				*out++ = *ev;
			else
				pr_vdebug("purging event %d\n", *ev);
		ffs->ev.count = out - ffs->ev.types;
	}

	pr_vdebug("adding event %d\n", type);
	ffs->ev.types[ffs->ev.count++] = type;
	wake_up_locked(&ffs->ev.waitq);
}

static void ffs_event_add(struct ffs_data *ffs,
			  enum usb_functionfs_event_type type)
{
	unsigned long flags;
	spin_lock_irqsave(&ffs->ev.waitq.lock, flags);
	__ffs_event_add(ffs, type);
	spin_unlock_irqrestore(&ffs->ev.waitq.lock, flags);
}


/* Bind/unbind USB function hooks *******************************************/

static int __ffs_func_bind_do_descs(enum ffs_entity_type type, u8 *valuep,
				    struct usb_descriptor_header *desc,
				    void *priv)
{
	struct usb_endpoint_descriptor *ds = (void *)desc;
	struct ffs_function *func = priv;
	struct ffs_ep *ffs_ep;

	/*
	 * If hs_descriptors is not NULL then we are reading hs
	 * descriptors now
	 */
	const int isHS = func->function.hs_descriptors != NULL;
	unsigned idx;

	if (type != FFS_DESCRIPTOR)
		return 0;

	if (isHS)
		func->function.hs_descriptors[(long)valuep] = desc;
	else
<<<<<<< HEAD
		func->function.descriptors[(long)valuep]    = desc;
=======
		func->function.fs_descriptors[(long)valuep]    = desc;
>>>>>>> refs/remotes/origin/master

	if (!desc || desc->bDescriptorType != USB_DT_ENDPOINT)
		return 0;

	idx = (ds->bEndpointAddress & USB_ENDPOINT_NUMBER_MASK) - 1;
	ffs_ep = func->eps + idx;

	if (unlikely(ffs_ep->descs[isHS])) {
		pr_vdebug("two %sspeed descriptors for EP %d\n",
			  isHS ? "high" : "full",
			  ds->bEndpointAddress & USB_ENDPOINT_NUMBER_MASK);
		return -EINVAL;
	}
	ffs_ep->descs[isHS] = ds;

	ffs_dump_mem(": Original  ep desc", ds, ds->bLength);
	if (ffs_ep->ep) {
		ds->bEndpointAddress = ffs_ep->descs[0]->bEndpointAddress;
		if (!ds->wMaxPacketSize)
			ds->wMaxPacketSize = ffs_ep->descs[0]->wMaxPacketSize;
	} else {
		struct usb_request *req;
		struct usb_ep *ep;

		pr_vdebug("autoconfig\n");
		ep = usb_ep_autoconfig(func->gadget, ds);
		if (unlikely(!ep))
			return -ENOTSUPP;
		ep->driver_data = func->eps + idx;

		req = usb_ep_alloc_request(ep, GFP_KERNEL);
		if (unlikely(!req))
			return -ENOMEM;

		ffs_ep->ep  = ep;
		ffs_ep->req = req;
		func->eps_revmap[ds->bEndpointAddress &
				 USB_ENDPOINT_NUMBER_MASK] = idx + 1;
	}
	ffs_dump_mem(": Rewritten ep desc", ds, ds->bLength);

	return 0;
}

static int __ffs_func_bind_do_nums(enum ffs_entity_type type, u8 *valuep,
				   struct usb_descriptor_header *desc,
				   void *priv)
{
	struct ffs_function *func = priv;
	unsigned idx;
	u8 newValue;

	switch (type) {
	default:
	case FFS_DESCRIPTOR:
		/* Handled in previous pass by __ffs_func_bind_do_descs() */
		return 0;

	case FFS_INTERFACE:
		idx = *valuep;
		if (func->interfaces_nums[idx] < 0) {
			int id = usb_interface_id(func->conf, &func->function);
			if (unlikely(id < 0))
				return id;
			func->interfaces_nums[idx] = id;
		}
		newValue = func->interfaces_nums[idx];
		break;

	case FFS_STRING:
		/* String' IDs are allocated when fsf_data is bound to cdev */
		newValue = func->ffs->stringtabs[0]->strings[*valuep - 1].id;
		break;

	case FFS_ENDPOINT:
		/*
		 * USB_DT_ENDPOINT are handled in
		 * __ffs_func_bind_do_descs().
		 */
		if (desc->bDescriptorType == USB_DT_ENDPOINT)
			return 0;

		idx = (*valuep & USB_ENDPOINT_NUMBER_MASK) - 1;
		if (unlikely(!func->eps[idx].ep))
			return -EINVAL;

		{
			struct usb_endpoint_descriptor **descs;
			descs = func->eps[idx].descs;
			newValue = descs[descs[0] ? 0 : 1]->bEndpointAddress;
		}
		break;
	}

	pr_vdebug("%02x -> %02x\n", *valuep, newValue);
	*valuep = newValue;
	return 0;
}

<<<<<<< HEAD
static int ffs_func_bind(struct usb_configuration *c,
			 struct usb_function *f)
=======
static inline struct f_fs_opts *ffs_do_functionfs_bind(struct usb_function *f,
						struct usb_configuration *c)
{
	struct ffs_function *func = ffs_func_from_usb(f);
	struct f_fs_opts *ffs_opts =
		container_of(f->fi, struct f_fs_opts, func_inst);
	int ret;

	ENTER();

	/*
	 * Legacy gadget triggers binding in functionfs_ready_callback,
	 * which already uses locking; taking the same lock here would
	 * cause a deadlock.
	 *
	 * Configfs-enabled gadgets however do need ffs_dev_lock.
	 */
	if (!ffs_opts->no_configfs)
		ffs_dev_lock();
	ret = ffs_opts->dev->desc_ready ? 0 : -ENODEV;
	func->ffs = ffs_opts->dev->ffs_data;
	if (!ffs_opts->no_configfs)
		ffs_dev_unlock();
	if (ret)
		return ERR_PTR(ret);

	func->conf = c;
	func->gadget = c->cdev->gadget;

	ffs_data_get(func->ffs);

	/*
	 * in drivers/usb/gadget/configfs.c:configfs_composite_bind()
	 * configurations are bound in sequence with list_for_each_entry,
	 * in each configuration its functions are bound in sequence
	 * with list_for_each_entry, so we assume no race condition
	 * with regard to ffs_opts->bound access
	 */
	if (!ffs_opts->refcnt) {
		ret = functionfs_bind(func->ffs, c->cdev);
		if (ret)
			return ERR_PTR(ret);
	}
	ffs_opts->refcnt++;
	func->function.strings = func->ffs->stringtabs;

	return ffs_opts;
}

static int _ffs_func_bind(struct usb_configuration *c,
			  struct usb_function *f)
>>>>>>> refs/remotes/origin/master
{
	struct ffs_function *func = ffs_func_from_usb(f);
	struct ffs_data *ffs = func->ffs;

	const int full = !!func->ffs->fs_descs_count;
	const int high = gadget_is_dualspeed(func->gadget) &&
		func->ffs->hs_descs_count;

	int ret;

	/* Make it a single chunk, less management later on */
<<<<<<< HEAD
	struct {
		struct ffs_ep eps[ffs->eps_count];
		struct usb_descriptor_header
			*fs_descs[full ? ffs->fs_descs_count + 1 : 0];
		struct usb_descriptor_header
			*hs_descs[high ? ffs->hs_descs_count + 1 : 0];
		short inums[ffs->interfaces_count];
		char raw_descs[high ? ffs->raw_descs_length
				    : ffs->raw_fs_descs_length];
	} *data;
=======
	vla_group(d);
	vla_item_with_sz(d, struct ffs_ep, eps, ffs->eps_count);
	vla_item_with_sz(d, struct usb_descriptor_header *, fs_descs,
		full ? ffs->fs_descs_count + 1 : 0);
	vla_item_with_sz(d, struct usb_descriptor_header *, hs_descs,
		high ? ffs->hs_descs_count + 1 : 0);
	vla_item_with_sz(d, short, inums, ffs->interfaces_count);
	vla_item_with_sz(d, char, raw_descs,
		high ? ffs->raw_descs_length : ffs->raw_fs_descs_length);
	char *vlabuf;
>>>>>>> refs/remotes/origin/master

	ENTER();

	/* Only high speed but not supported by gadget? */
	if (unlikely(!(full | high)))
		return -ENOTSUPP;

<<<<<<< HEAD
	/* Allocate */
	data = kmalloc(sizeof *data, GFP_KERNEL);
	if (unlikely(!data))
		return -ENOMEM;

	/* Zero */
	memset(data->eps, 0, sizeof data->eps);
	memcpy(data->raw_descs, ffs->raw_descs + 16, sizeof data->raw_descs);
	memset(data->inums, 0xff, sizeof data->inums);
	for (ret = ffs->eps_count; ret; --ret)
		data->eps[ret].num = -1;

	/* Save pointers */
	func->eps             = data->eps;
	func->interfaces_nums = data->inums;
=======
	/* Allocate a single chunk, less management later on */
	vlabuf = kmalloc(vla_group_size(d), GFP_KERNEL);
	if (unlikely(!vlabuf))
		return -ENOMEM;

	/* Zero */
	memset(vla_ptr(vlabuf, d, eps), 0, d_eps__sz);
	memcpy(vla_ptr(vlabuf, d, raw_descs), ffs->raw_descs + 16,
	       d_raw_descs__sz);
	memset(vla_ptr(vlabuf, d, inums), 0xff, d_inums__sz);
	for (ret = ffs->eps_count; ret; --ret) {
		struct ffs_ep *ptr;

		ptr = vla_ptr(vlabuf, d, eps);
		ptr[ret].num = -1;
	}

	/* Save pointers
	 * d_eps == vlabuf, func->eps used to kfree vlabuf later
	*/
	func->eps             = vla_ptr(vlabuf, d, eps);
	func->interfaces_nums = vla_ptr(vlabuf, d, inums);
>>>>>>> refs/remotes/origin/master

	/*
	 * Go through all the endpoint descriptors and allocate
	 * endpoints first, so that later we can rewrite the endpoint
	 * numbers without worrying that it may be described later on.
	 */
	if (likely(full)) {
<<<<<<< HEAD
		func->function.descriptors = data->fs_descs;
		ret = ffs_do_descs(ffs->fs_descs_count,
				   data->raw_descs,
				   sizeof data->raw_descs,
=======
		func->function.fs_descriptors = vla_ptr(vlabuf, d, fs_descs);
		ret = ffs_do_descs(ffs->fs_descs_count,
				   vla_ptr(vlabuf, d, raw_descs),
				   d_raw_descs__sz,
>>>>>>> refs/remotes/origin/master
				   __ffs_func_bind_do_descs, func);
		if (unlikely(ret < 0))
			goto error;
	} else {
		ret = 0;
	}

	if (likely(high)) {
<<<<<<< HEAD
		func->function.hs_descriptors = data->hs_descs;
		ret = ffs_do_descs(ffs->hs_descs_count,
				   data->raw_descs + ret,
				   (sizeof data->raw_descs) - ret,
				   __ffs_func_bind_do_descs, func);
=======
		func->function.hs_descriptors = vla_ptr(vlabuf, d, hs_descs);
		ret = ffs_do_descs(ffs->hs_descs_count,
				   vla_ptr(vlabuf, d, raw_descs) + ret,
				   d_raw_descs__sz - ret,
				   __ffs_func_bind_do_descs, func);
		if (unlikely(ret < 0))
			goto error;
>>>>>>> refs/remotes/origin/master
	}

	/*
	 * Now handle interface numbers allocation and interface and
	 * endpoint numbers rewriting.  We can do that in one go
	 * now.
	 */
	ret = ffs_do_descs(ffs->fs_descs_count +
			   (high ? ffs->hs_descs_count : 0),
<<<<<<< HEAD
			   data->raw_descs, sizeof data->raw_descs,
=======
			   vla_ptr(vlabuf, d, raw_descs), d_raw_descs__sz,
>>>>>>> refs/remotes/origin/master
			   __ffs_func_bind_do_nums, func);
	if (unlikely(ret < 0))
		goto error;

	/* And we're done */
	ffs_event_add(ffs, FUNCTIONFS_BIND);
	return 0;

error:
	/* XXX Do we need to release all claimed endpoints here? */
	return ret;
}

<<<<<<< HEAD

/* Other USB function hooks *************************************************/

static void ffs_func_unbind(struct usb_configuration *c,
			    struct usb_function *f)
{
	struct ffs_function *func = ffs_func_from_usb(f);
	struct ffs_data *ffs = func->ffs;

	ENTER();

	if (ffs->func == func) {
		ffs_func_eps_disable(func);
		ffs->func = NULL;
	}

	ffs_event_add(ffs, FUNCTIONFS_UNBIND);

	ffs_func_free(func);
}
=======
static int ffs_func_bind(struct usb_configuration *c,
			 struct usb_function *f)
{
	struct f_fs_opts *ffs_opts = ffs_do_functionfs_bind(f, c);

	if (IS_ERR(ffs_opts))
		return PTR_ERR(ffs_opts);

	return _ffs_func_bind(c, f);
}


/* Other USB function hooks *************************************************/
>>>>>>> refs/remotes/origin/master

static int ffs_func_set_alt(struct usb_function *f,
			    unsigned interface, unsigned alt)
{
	struct ffs_function *func = ffs_func_from_usb(f);
	struct ffs_data *ffs = func->ffs;
	int ret = 0, intf;

	if (alt != (unsigned)-1) {
		intf = ffs_func_revmap_intf(func, interface);
		if (unlikely(intf < 0))
			return intf;
	}

	if (ffs->func)
		ffs_func_eps_disable(ffs->func);

	if (ffs->state != FFS_ACTIVE)
		return -ENODEV;

	if (alt == (unsigned)-1) {
		ffs->func = NULL;
		ffs_event_add(ffs, FUNCTIONFS_DISABLE);
		return 0;
	}

	ffs->func = func;
	ret = ffs_func_eps_enable(func);
	if (likely(ret >= 0))
		ffs_event_add(ffs, FUNCTIONFS_ENABLE);
	return ret;
}

static void ffs_func_disable(struct usb_function *f)
{
	ffs_func_set_alt(f, 0, (unsigned)-1);
}

static int ffs_func_setup(struct usb_function *f,
			  const struct usb_ctrlrequest *creq)
{
	struct ffs_function *func = ffs_func_from_usb(f);
	struct ffs_data *ffs = func->ffs;
	unsigned long flags;
	int ret;

	ENTER();

	pr_vdebug("creq->bRequestType = %02x\n", creq->bRequestType);
	pr_vdebug("creq->bRequest     = %02x\n", creq->bRequest);
	pr_vdebug("creq->wValue       = %04x\n", le16_to_cpu(creq->wValue));
	pr_vdebug("creq->wIndex       = %04x\n", le16_to_cpu(creq->wIndex));
	pr_vdebug("creq->wLength      = %04x\n", le16_to_cpu(creq->wLength));

	/*
	 * Most requests directed to interface go through here
	 * (notable exceptions are set/get interface) so we need to
	 * handle them.  All other either handled by composite or
	 * passed to usb_configuration->setup() (if one is set).  No
	 * matter, we will handle requests directed to endpoint here
	 * as well (as it's straightforward) but what to do with any
	 * other request?
	 */
	if (ffs->state != FFS_ACTIVE)
		return -ENODEV;

	switch (creq->bRequestType & USB_RECIP_MASK) {
	case USB_RECIP_INTERFACE:
		ret = ffs_func_revmap_intf(func, le16_to_cpu(creq->wIndex));
		if (unlikely(ret < 0))
			return ret;
		break;

	case USB_RECIP_ENDPOINT:
		ret = ffs_func_revmap_ep(func, le16_to_cpu(creq->wIndex));
		if (unlikely(ret < 0))
			return ret;
		break;

	default:
		return -EOPNOTSUPP;
	}

	spin_lock_irqsave(&ffs->ev.waitq.lock, flags);
	ffs->ev.setup = *creq;
	ffs->ev.setup.wIndex = cpu_to_le16(ret);
	__ffs_event_add(ffs, FUNCTIONFS_SETUP);
	spin_unlock_irqrestore(&ffs->ev.waitq.lock, flags);

	return 0;
}

static void ffs_func_suspend(struct usb_function *f)
{
	ENTER();
	ffs_event_add(ffs_func_from_usb(f)->ffs, FUNCTIONFS_SUSPEND);
}

static void ffs_func_resume(struct usb_function *f)
{
	ENTER();
	ffs_event_add(ffs_func_from_usb(f)->ffs, FUNCTIONFS_RESUME);
}


/* Endpoint and interface numbers reverse mapping ***************************/

static int ffs_func_revmap_ep(struct ffs_function *func, u8 num)
{
	num = func->eps_revmap[num & USB_ENDPOINT_NUMBER_MASK];
	return num ? num : -EDOM;
}

static int ffs_func_revmap_intf(struct ffs_function *func, u8 intf)
{
	short *nums = func->interfaces_nums;
	unsigned count = func->ffs->interfaces_count;

	for (; count; --count, ++nums) {
		if (*nums >= 0 && *nums == intf)
			return nums - func->interfaces_nums;
	}

	return -EDOM;
}


<<<<<<< HEAD
=======
/* Devices management *******************************************************/

static LIST_HEAD(ffs_devices);

static struct ffs_dev *_ffs_find_dev(const char *name)
{
	struct ffs_dev *dev;

	list_for_each_entry(dev, &ffs_devices, entry) {
		if (!dev->name || !name)
			continue;
		if (strcmp(dev->name, name) == 0)
			return dev;
	}

	return NULL;
}

/*
 * ffs_lock must be taken by the caller of this function
 */
static struct ffs_dev *ffs_get_single_dev(void)
{
	struct ffs_dev *dev;

	if (list_is_singular(&ffs_devices)) {
		dev = list_first_entry(&ffs_devices, struct ffs_dev, entry);
		if (dev->single)
			return dev;
	}

	return NULL;
}

/*
 * ffs_lock must be taken by the caller of this function
 */
static struct ffs_dev *ffs_find_dev(const char *name)
{
	struct ffs_dev *dev;

	dev = ffs_get_single_dev();
	if (dev)
		return dev;

	return _ffs_find_dev(name);
}

/* Configfs support *********************************************************/

static inline struct f_fs_opts *to_ffs_opts(struct config_item *item)
{
	return container_of(to_config_group(item), struct f_fs_opts,
			    func_inst.group);
}

static void ffs_attr_release(struct config_item *item)
{
	struct f_fs_opts *opts = to_ffs_opts(item);

	usb_put_function_instance(&opts->func_inst);
}

static struct configfs_item_operations ffs_item_ops = {
	.release	= ffs_attr_release,
};

static struct config_item_type ffs_func_type = {
	.ct_item_ops	= &ffs_item_ops,
	.ct_owner	= THIS_MODULE,
};


/* Function registration interface ******************************************/

static void ffs_free_inst(struct usb_function_instance *f)
{
	struct f_fs_opts *opts;

	opts = to_f_fs_opts(f);
	ffs_dev_lock();
	ffs_free_dev(opts->dev);
	ffs_dev_unlock();
	kfree(opts);
}

#define MAX_INST_NAME_LEN	40

static int ffs_set_inst_name(struct usb_function_instance *fi, const char *name)
{
	struct f_fs_opts *opts;
	char *ptr;
	const char *tmp;
	int name_len, ret;

	name_len = strlen(name) + 1;
	if (name_len > MAX_INST_NAME_LEN)
		return -ENAMETOOLONG;

	ptr = kstrndup(name, name_len, GFP_KERNEL);
	if (!ptr)
		return -ENOMEM;

	opts = to_f_fs_opts(fi);
	tmp = NULL;

	ffs_dev_lock();

	tmp = opts->dev->name_allocated ? opts->dev->name : NULL;
	ret = _ffs_name_dev(opts->dev, ptr);
	if (ret) {
		kfree(ptr);
		ffs_dev_unlock();
		return ret;
	}
	opts->dev->name_allocated = true;

	ffs_dev_unlock();

	kfree(tmp);

	return 0;
}

static struct usb_function_instance *ffs_alloc_inst(void)
{
	struct f_fs_opts *opts;
	struct ffs_dev *dev;

	opts = kzalloc(sizeof(*opts), GFP_KERNEL);
	if (!opts)
		return ERR_PTR(-ENOMEM);

	opts->func_inst.set_inst_name = ffs_set_inst_name;
	opts->func_inst.free_func_inst = ffs_free_inst;
	ffs_dev_lock();
	dev = ffs_alloc_dev();
	ffs_dev_unlock();
	if (IS_ERR(dev)) {
		kfree(opts);
		return ERR_CAST(dev);
	}
	opts->dev = dev;
	dev->opts = opts;

	config_group_init_type_name(&opts->func_inst.group, "",
				    &ffs_func_type);
	return &opts->func_inst;
}

static void ffs_free(struct usb_function *f)
{
	kfree(ffs_func_from_usb(f));
}

static void ffs_func_unbind(struct usb_configuration *c,
			    struct usb_function *f)
{
	struct ffs_function *func = ffs_func_from_usb(f);
	struct ffs_data *ffs = func->ffs;
	struct f_fs_opts *opts =
		container_of(f->fi, struct f_fs_opts, func_inst);
	struct ffs_ep *ep = func->eps;
	unsigned count = ffs->eps_count;
	unsigned long flags;

	ENTER();
	if (ffs->func == func) {
		ffs_func_eps_disable(func);
		ffs->func = NULL;
	}

	if (!--opts->refcnt)
		functionfs_unbind(ffs);

	/* cleanup after autoconfig */
	spin_lock_irqsave(&func->ffs->eps_lock, flags);
	do {
		if (ep->ep && ep->req)
			usb_ep_free_request(ep->ep, ep->req);
		ep->req = NULL;
		++ep;
	} while (--count);
	spin_unlock_irqrestore(&func->ffs->eps_lock, flags);
	kfree(func->eps);
	func->eps = NULL;
	/*
	 * eps, descriptors and interfaces_nums are allocated in the
	 * same chunk so only one free is required.
	 */
	func->function.fs_descriptors = NULL;
	func->function.hs_descriptors = NULL;
	func->interfaces_nums = NULL;

	ffs_event_add(ffs, FUNCTIONFS_UNBIND);
}

static struct usb_function *ffs_alloc(struct usb_function_instance *fi)
{
	struct ffs_function *func;

	ENTER();

	func = kzalloc(sizeof(*func), GFP_KERNEL);
	if (unlikely(!func))
		return ERR_PTR(-ENOMEM);

	func->function.name    = "Function FS Gadget";

	func->function.bind    = ffs_func_bind;
	func->function.unbind  = ffs_func_unbind;
	func->function.set_alt = ffs_func_set_alt;
	func->function.disable = ffs_func_disable;
	func->function.setup   = ffs_func_setup;
	func->function.suspend = ffs_func_suspend;
	func->function.resume  = ffs_func_resume;
	func->function.free_func = ffs_free;

	return &func->function;
}

/*
 * ffs_lock must be taken by the caller of this function
 */
struct ffs_dev *ffs_alloc_dev(void)
{
	struct ffs_dev *dev;
	int ret;

	if (ffs_get_single_dev())
			return ERR_PTR(-EBUSY);

	dev = kzalloc(sizeof(*dev), GFP_KERNEL);
	if (!dev)
		return ERR_PTR(-ENOMEM);

	if (list_empty(&ffs_devices)) {
		ret = functionfs_init();
		if (ret) {
			kfree(dev);
			return ERR_PTR(ret);
		}
	}

	list_add(&dev->entry, &ffs_devices);

	return dev;
}

/*
 * ffs_lock must be taken by the caller of this function
 * The caller is responsible for "name" being available whenever f_fs needs it
 */
static int _ffs_name_dev(struct ffs_dev *dev, const char *name)
{
	struct ffs_dev *existing;

	existing = _ffs_find_dev(name);
	if (existing)
		return -EBUSY;
	
	dev->name = name;

	return 0;
}

/*
 * The caller is responsible for "name" being available whenever f_fs needs it
 */
int ffs_name_dev(struct ffs_dev *dev, const char *name)
{
	int ret;

	ffs_dev_lock();
	ret = _ffs_name_dev(dev, name);
	ffs_dev_unlock();

	return ret;
}
EXPORT_SYMBOL(ffs_name_dev);

int ffs_single_dev(struct ffs_dev *dev)
{
	int ret;

	ret = 0;
	ffs_dev_lock();

	if (!list_is_singular(&ffs_devices))
		ret = -EBUSY;
	else
		dev->single = true;

	ffs_dev_unlock();
	return ret;
}
EXPORT_SYMBOL(ffs_single_dev);

/*
 * ffs_lock must be taken by the caller of this function
 */
void ffs_free_dev(struct ffs_dev *dev)
{
	list_del(&dev->entry);
	if (dev->name_allocated)
		kfree(dev->name);
	kfree(dev);
	if (list_empty(&ffs_devices))
		functionfs_cleanup();
}

static void *ffs_acquire_dev(const char *dev_name)
{
	struct ffs_dev *ffs_dev;

	ENTER();
	ffs_dev_lock();

	ffs_dev = ffs_find_dev(dev_name);
	if (!ffs_dev)
		ffs_dev = ERR_PTR(-ENODEV);
	else if (ffs_dev->mounted)
		ffs_dev = ERR_PTR(-EBUSY);
	else if (ffs_dev->ffs_acquire_dev_callback &&
	    ffs_dev->ffs_acquire_dev_callback(ffs_dev))
		ffs_dev = ERR_PTR(-ENODEV);
	else
		ffs_dev->mounted = true;

	ffs_dev_unlock();
	return ffs_dev;
}

static void ffs_release_dev(struct ffs_data *ffs_data)
{
	struct ffs_dev *ffs_dev;

	ENTER();
	ffs_dev_lock();

	ffs_dev = ffs_data->private_data;
	if (ffs_dev)
		ffs_dev->mounted = false;
	
	if (ffs_dev->ffs_release_dev_callback)
		ffs_dev->ffs_release_dev_callback(ffs_dev);

	ffs_dev_unlock();
}

static int ffs_ready(struct ffs_data *ffs)
{
	struct ffs_dev *ffs_obj;
	int ret = 0;

	ENTER();
	ffs_dev_lock();

	ffs_obj = ffs->private_data;
	if (!ffs_obj) {
		ret = -EINVAL;
		goto done;
	}
	if (WARN_ON(ffs_obj->desc_ready)) {
		ret = -EBUSY;
		goto done;
	}

	ffs_obj->desc_ready = true;
	ffs_obj->ffs_data = ffs;

	if (ffs_obj->ffs_ready_callback)
		ret = ffs_obj->ffs_ready_callback(ffs);

done:
	ffs_dev_unlock();
	return ret;
}

static void ffs_closed(struct ffs_data *ffs)
{
	struct ffs_dev *ffs_obj;

	ENTER();
	ffs_dev_lock();

	ffs_obj = ffs->private_data;
	if (!ffs_obj)
		goto done;

	ffs_obj->desc_ready = false;

	if (ffs_obj->ffs_closed_callback)
		ffs_obj->ffs_closed_callback(ffs);

	if (!ffs_obj->opts || ffs_obj->opts->no_configfs
	    || !ffs_obj->opts->func_inst.group.cg_item.ci_parent)
		goto done;

	unregister_gadget_item(ffs_obj->opts->
			       func_inst.group.cg_item.ci_parent->ci_parent);
done:
	ffs_dev_unlock();
}

>>>>>>> refs/remotes/origin/master
/* Misc helper functions ****************************************************/

static int ffs_mutex_lock(struct mutex *mutex, unsigned nonblock)
{
	return nonblock
		? likely(mutex_trylock(mutex)) ? 0 : -EAGAIN
		: mutex_lock_interruptible(mutex);
}

<<<<<<< HEAD
static char *ffs_prepare_buffer(const char * __user buf, size_t len)
=======
static char *ffs_prepare_buffer(const char __user *buf, size_t len)
>>>>>>> refs/remotes/origin/master
{
	char *data;

	if (unlikely(!len))
		return NULL;

	data = kmalloc(len, GFP_KERNEL);
	if (unlikely(!data))
		return ERR_PTR(-ENOMEM);

	if (unlikely(__copy_from_user(data, buf, len))) {
		kfree(data);
		return ERR_PTR(-EFAULT);
	}

	pr_vdebug("Buffer from user space:\n");
	ffs_dump_mem("", data, len);

	return data;
}
<<<<<<< HEAD
=======

DECLARE_USB_FUNCTION_INIT(ffs, ffs_alloc_inst, ffs_alloc);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Michal Nazarewicz");
>>>>>>> refs/remotes/origin/master
