/*
   rbd.c -- Export ceph rados objects as a Linux block device


   based on drivers/block/osdblk.c:

   Copyright 2009 Red Hat, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.



   For usage instructions, please refer to:

                 Documentation/ABI/testing/sysfs-bus-rbd

 */

#include <linux/ceph/libceph.h>
#include <linux/ceph/osd_client.h>
#include <linux/ceph/mon_client.h>
#include <linux/ceph/decode.h>
#include <linux/parser.h>

#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/blkdev.h>

#include "rbd_types.h"

<<<<<<< HEAD
#define DRV_NAME "rbd"
#define DRV_NAME_LONG "rbd (rados block device)"

#define RBD_MINORS_PER_MAJOR	256		/* max minors per blkdev */

#define RBD_MAX_MD_NAME_LEN	(96 + sizeof(RBD_SUFFIX))
=======
/*
 * The basic unit of block I/O is a sector.  It is interpreted in a
 * number of contexts in Linux (blk, bio, genhd), but the default is
 * universally 512 bytes.  These symbols are just slightly more
 * meaningful than the bare numbers they represent.
 */
#define	SECTOR_SHIFT	9
#define	SECTOR_SIZE	(1ULL << SECTOR_SHIFT)

#define RBD_DRV_NAME "rbd"
#define RBD_DRV_NAME_LONG "rbd (rados block device)"

#define RBD_MINORS_PER_MAJOR	256		/* max minors per blkdev */

#define RBD_MAX_MD_NAME_LEN	(RBD_MAX_OBJ_NAME_LEN + sizeof(RBD_SUFFIX))
>>>>>>> refs/remotes/origin/cm-10.0
#define RBD_MAX_POOL_NAME_LEN	64
#define RBD_MAX_SNAP_NAME_LEN	32
#define RBD_MAX_OPT_LEN		1024

#define RBD_SNAP_HEAD_NAME	"-"

<<<<<<< HEAD
#define DEV_NAME_LEN		32

#define RBD_NOTIFY_TIMEOUT_DEFAULT 10
=======
/*
 * An RBD device name will be "rbd#", where the "rbd" comes from
 * RBD_DRV_NAME above, and # is a unique integer identifier.
 * MAX_INT_FORMAT_WIDTH is used in ensuring DEV_NAME_LEN is big
 * enough to hold all possible device names.
 */
#define DEV_NAME_LEN		32
#define MAX_INT_FORMAT_WIDTH	((5 * sizeof (int)) / 2 + 1)

#define RBD_READ_ONLY_DEFAULT		false
>>>>>>> refs/remotes/origin/cm-10.0

/*
 * block device image metadata (in-memory version)
 */
struct rbd_image_header {
	u64 image_size;
	char block_name[32];
	__u8 obj_order;
	__u8 crypt_type;
	__u8 comp_type;
<<<<<<< HEAD
	struct rw_semaphore snap_rwsem;
=======
>>>>>>> refs/remotes/origin/cm-10.0
	struct ceph_snap_context *snapc;
	size_t snap_names_len;
	u64 snap_seq;
	u32 total_snaps;

	char *snap_names;
	u64 *snap_sizes;

	u64 obj_version;
};

struct rbd_options {
<<<<<<< HEAD
	int	notify_timeout;
};

/*
 * an instance of the client.  multiple devices may share a client.
=======
	bool	read_only;
};

/*
 * an instance of the client.  multiple devices may share an rbd client.
>>>>>>> refs/remotes/origin/cm-10.0
 */
struct rbd_client {
	struct ceph_client	*client;
	struct rbd_options	*rbd_opts;
	struct kref		kref;
	struct list_head	node;
};

<<<<<<< HEAD
struct rbd_req_coll;

/*
 * a single io request
 */
struct rbd_request {
	struct request		*rq;		/* blk layer request */
	struct bio		*bio;		/* cloned bio */
	struct page		**pages;	/* list of used pages */
	u64			len;
	int			coll_index;
	struct rbd_req_coll	*coll;
};

=======
/*
 * a request completion status
 */
>>>>>>> refs/remotes/origin/cm-10.0
struct rbd_req_status {
	int done;
	int rc;
	u64 bytes;
};

/*
 * a collection of requests
 */
struct rbd_req_coll {
	int			total;
	int			num_done;
	struct kref		kref;
	struct rbd_req_status	status[0];
};

<<<<<<< HEAD
=======
/*
 * a single io request
 */
struct rbd_request {
	struct request		*rq;		/* blk layer request */
	struct bio		*bio;		/* cloned bio */
	struct page		**pages;	/* list of used pages */
	u64			len;
	int			coll_index;
	struct rbd_req_coll	*coll;
};

>>>>>>> refs/remotes/origin/cm-10.0
struct rbd_snap {
	struct	device		dev;
	const char		*name;
	size_t			size;
	struct list_head	node;
	u64			id;
};

/*
 * a single device
 */
struct rbd_device {
	int			id;		/* blkdev unique id */

	int			major;		/* blkdev assigned major */
	struct gendisk		*disk;		/* blkdev's gendisk and rq */
	struct request_queue	*q;

<<<<<<< HEAD
	struct ceph_client	*client;
=======
>>>>>>> refs/remotes/origin/cm-10.0
	struct rbd_client	*rbd_client;

	char			name[DEV_NAME_LEN]; /* blkdev name, e.g. rbd3 */

	spinlock_t		lock;		/* queue lock */

	struct rbd_image_header	header;
	char			obj[RBD_MAX_OBJ_NAME_LEN]; /* rbd image name */
	int			obj_len;
	char			obj_md_name[RBD_MAX_MD_NAME_LEN]; /* hdr nm. */
	char			pool_name[RBD_MAX_POOL_NAME_LEN];
	int			poolid;

	struct ceph_osd_event   *watch_event;
	struct ceph_osd_request *watch_request;

<<<<<<< HEAD
	char                    snap_name[RBD_MAX_SNAP_NAME_LEN];
	u32 cur_snap;	/* index+1 of current snapshot within snap context
			   0 - for the head */
	int read_only;
=======
	/* protects updating the header */
	struct rw_semaphore     header_rwsem;
	/* name of the snapshot this device reads from */
	char                    snap_name[RBD_MAX_SNAP_NAME_LEN];
	/* id of the snapshot this device reads from */
	u64                     snap_id;	/* current snapshot id */
	/* whether the snap_id this device reads from still exists */
	bool                    snap_exists;
	bool			read_only;
>>>>>>> refs/remotes/origin/cm-10.0

	struct list_head	node;

	/* list of snapshots */
	struct list_head	snaps;

	/* sysfs related */
	struct device		dev;
<<<<<<< HEAD
};

static struct bus_type rbd_bus_type = {
	.name		= "rbd",
};

static spinlock_t node_lock;      /* protects client get/put */

static DEFINE_MUTEX(ctl_mutex);	  /* Serialize open/close/setup/teardown */
static LIST_HEAD(rbd_dev_list);    /* devices */
static LIST_HEAD(rbd_client_list);      /* clients */

static int __rbd_init_snaps_header(struct rbd_device *rbd_dev);
static void rbd_dev_release(struct device *dev);
static ssize_t rbd_snap_rollback(struct device *dev,
				 struct device_attribute *attr,
				 const char *buf,
				 size_t size);
static ssize_t rbd_snap_add(struct device *dev,
			    struct device_attribute *attr,
			    const char *buf,
			    size_t count);
static void __rbd_remove_snap_dev(struct rbd_device *rbd_dev,
				  struct rbd_snap *snap);;


static struct rbd_device *dev_to_rbd(struct device *dev)
{
	return container_of(dev, struct rbd_device, dev);
}

=======
	unsigned long		open_count;
};

static DEFINE_MUTEX(ctl_mutex);	  /* Serialize open/close/setup/teardown */

static LIST_HEAD(rbd_dev_list);    /* devices */
static DEFINE_SPINLOCK(rbd_dev_list_lock);

static LIST_HEAD(rbd_client_list);		/* clients */
static DEFINE_SPINLOCK(rbd_client_list_lock);

static int __rbd_init_snaps_header(struct rbd_device *rbd_dev);
static void rbd_dev_release(struct device *dev);
static void __rbd_remove_snap_dev(struct rbd_device *rbd_dev,
				  struct rbd_snap *snap);

static ssize_t rbd_add(struct bus_type *bus, const char *buf,
		       size_t count);
static ssize_t rbd_remove(struct bus_type *bus, const char *buf,
			  size_t count);

static struct bus_attribute rbd_bus_attrs[] = {
	__ATTR(add, S_IWUSR, NULL, rbd_add),
	__ATTR(remove, S_IWUSR, NULL, rbd_remove),
	__ATTR_NULL
};

static struct bus_type rbd_bus_type = {
	.name		= "rbd",
	.bus_attrs	= rbd_bus_attrs,
};

static void rbd_root_dev_release(struct device *dev)
{
}

static struct device rbd_root_dev = {
	.init_name =    "rbd",
	.release =      rbd_root_dev_release,
};


>>>>>>> refs/remotes/origin/cm-10.0
static struct device *rbd_get_dev(struct rbd_device *rbd_dev)
{
	return get_device(&rbd_dev->dev);
}

static void rbd_put_dev(struct rbd_device *rbd_dev)
{
	put_device(&rbd_dev->dev);
}

static int __rbd_update_snaps(struct rbd_device *rbd_dev);

static int rbd_open(struct block_device *bdev, fmode_t mode)
{
<<<<<<< HEAD
	struct gendisk *disk = bdev->bd_disk;
	struct rbd_device *rbd_dev = disk->private_data;

	rbd_get_dev(rbd_dev);

	set_device_ro(bdev, rbd_dev->read_only);
=======
	struct rbd_device *rbd_dev = bdev->bd_disk->private_data;
>>>>>>> refs/remotes/origin/cm-10.0

	if ((mode & FMODE_WRITE) && rbd_dev->read_only)
		return -EROFS;

<<<<<<< HEAD
=======
	mutex_lock_nested(&ctl_mutex, SINGLE_DEPTH_NESTING);
	rbd_get_dev(rbd_dev);
	set_device_ro(bdev, rbd_dev->read_only);
	rbd_dev->open_count++;
	mutex_unlock(&ctl_mutex);

>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
}

static int rbd_release(struct gendisk *disk, fmode_t mode)
{
	struct rbd_device *rbd_dev = disk->private_data;

<<<<<<< HEAD
	rbd_put_dev(rbd_dev);
=======
	mutex_lock_nested(&ctl_mutex, SINGLE_DEPTH_NESTING);
	BUG_ON(!rbd_dev->open_count);
	rbd_dev->open_count--;
	rbd_put_dev(rbd_dev);
	mutex_unlock(&ctl_mutex);
>>>>>>> refs/remotes/origin/cm-10.0

	return 0;
}

static const struct block_device_operations rbd_bd_ops = {
	.owner			= THIS_MODULE,
	.open			= rbd_open,
	.release		= rbd_release,
};

/*
 * Initialize an rbd client instance.
 * We own *opt.
 */
static struct rbd_client *rbd_client_create(struct ceph_options *opt,
					    struct rbd_options *rbd_opts)
{
	struct rbd_client *rbdc;
	int ret = -ENOMEM;

	dout("rbd_client_create\n");
	rbdc = kmalloc(sizeof(struct rbd_client), GFP_KERNEL);
	if (!rbdc)
		goto out_opt;

	kref_init(&rbdc->kref);
	INIT_LIST_HEAD(&rbdc->node);

<<<<<<< HEAD
	rbdc->client = ceph_create_client(opt, rbdc);
	if (IS_ERR(rbdc->client))
		goto out_rbdc;
=======
	mutex_lock_nested(&ctl_mutex, SINGLE_DEPTH_NESTING);

	rbdc->client = ceph_create_client(opt, rbdc, 0, 0);
	if (IS_ERR(rbdc->client))
		goto out_mutex;
>>>>>>> refs/remotes/origin/cm-10.0
	opt = NULL; /* Now rbdc->client is responsible for opt */

	ret = ceph_open_session(rbdc->client);
	if (ret < 0)
		goto out_err;

	rbdc->rbd_opts = rbd_opts;

<<<<<<< HEAD
	spin_lock(&node_lock);
	list_add_tail(&rbdc->node, &rbd_client_list);
	spin_unlock(&node_lock);
=======
	spin_lock(&rbd_client_list_lock);
	list_add_tail(&rbdc->node, &rbd_client_list);
	spin_unlock(&rbd_client_list_lock);

	mutex_unlock(&ctl_mutex);
>>>>>>> refs/remotes/origin/cm-10.0

	dout("rbd_client_create created %p\n", rbdc);
	return rbdc;

out_err:
	ceph_destroy_client(rbdc->client);
<<<<<<< HEAD
out_rbdc:
=======
out_mutex:
	mutex_unlock(&ctl_mutex);
>>>>>>> refs/remotes/origin/cm-10.0
	kfree(rbdc);
out_opt:
	if (opt)
		ceph_destroy_options(opt);
	return ERR_PTR(ret);
}

/*
 * Find a ceph client with specific addr and configuration.
 */
static struct rbd_client *__rbd_client_find(struct ceph_options *opt)
{
	struct rbd_client *client_node;

	if (opt->flags & CEPH_OPT_NOSHARE)
		return NULL;

	list_for_each_entry(client_node, &rbd_client_list, node)
		if (ceph_compare_options(opt, client_node->client) == 0)
			return client_node;
	return NULL;
}

/*
 * mount options
 */
enum {
<<<<<<< HEAD
	Opt_notify_timeout,
=======
>>>>>>> refs/remotes/origin/cm-10.0
	Opt_last_int,
	/* int args above */
	Opt_last_string,
	/* string args above */
<<<<<<< HEAD
};

static match_table_t rbdopt_tokens = {
	{Opt_notify_timeout, "notify_timeout=%d"},
	/* int args above */
	/* string args above */
=======
	Opt_read_only,
	Opt_read_write,
	/* Boolean args above */
	Opt_last_bool,
};

static match_table_t rbdopt_tokens = {
	/* int args above */
	/* string args above */
	{Opt_read_only, "read_only"},
	{Opt_read_only, "ro"},		/* Alternate spelling */
	{Opt_read_write, "read_write"},
	{Opt_read_write, "rw"},		/* Alternate spelling */
	/* Boolean args above */
>>>>>>> refs/remotes/origin/cm-10.0
	{-1, NULL}
};

static int parse_rbd_opts_token(char *c, void *private)
{
	struct rbd_options *rbdopt = private;
	substring_t argstr[MAX_OPT_ARGS];
	int token, intval, ret;

<<<<<<< HEAD
	token = match_token((char *)c, rbdopt_tokens, argstr);
=======
	token = match_token(c, rbdopt_tokens, argstr);
>>>>>>> refs/remotes/origin/cm-10.0
	if (token < 0)
		return -EINVAL;

	if (token < Opt_last_int) {
		ret = match_int(&argstr[0], &intval);
		if (ret < 0) {
			pr_err("bad mount option arg (not int) "
			       "at '%s'\n", c);
			return ret;
		}
		dout("got int token %d val %d\n", token, intval);
	} else if (token > Opt_last_int && token < Opt_last_string) {
		dout("got string token %d val %s\n", token,
		     argstr[0].from);
<<<<<<< HEAD
=======
	} else if (token > Opt_last_string && token < Opt_last_bool) {
		dout("got Boolean token %d\n", token);
>>>>>>> refs/remotes/origin/cm-10.0
	} else {
		dout("got token %d\n", token);
	}

	switch (token) {
<<<<<<< HEAD
	case Opt_notify_timeout:
		rbdopt->notify_timeout = intval;
=======
	case Opt_read_only:
		rbdopt->read_only = true;
		break;
	case Opt_read_write:
		rbdopt->read_only = false;
>>>>>>> refs/remotes/origin/cm-10.0
		break;
	default:
		BUG_ON(token);
	}
	return 0;
}

/*
 * Get a ceph client with specific addr and configuration, if one does
 * not exist create it.
 */
<<<<<<< HEAD
static int rbd_get_client(struct rbd_device *rbd_dev, const char *mon_addr,
			  char *options)
{
	struct rbd_client *rbdc;
	struct ceph_options *opt;
	int ret;
=======
static struct rbd_client *rbd_get_client(const char *mon_addr,
					 size_t mon_addr_len,
					 char *options)
{
	struct rbd_client *rbdc;
	struct ceph_options *opt;
>>>>>>> refs/remotes/origin/cm-10.0
	struct rbd_options *rbd_opts;

	rbd_opts = kzalloc(sizeof(*rbd_opts), GFP_KERNEL);
	if (!rbd_opts)
<<<<<<< HEAD
		return -ENOMEM;

	rbd_opts->notify_timeout = RBD_NOTIFY_TIMEOUT_DEFAULT;

	ret = ceph_parse_options(&opt, options, mon_addr,
				 mon_addr + strlen(mon_addr), parse_rbd_opts_token, rbd_opts);
	if (ret < 0)
		goto done_err;

	spin_lock(&node_lock);
	rbdc = __rbd_client_find(opt);
	if (rbdc) {
		ceph_destroy_options(opt);

		/* using an existing client */
		kref_get(&rbdc->kref);
		rbd_dev->rbd_client = rbdc;
		rbd_dev->client = rbdc->client;
		spin_unlock(&node_lock);
		return 0;
	}
	spin_unlock(&node_lock);

	rbdc = rbd_client_create(opt, rbd_opts);
	if (IS_ERR(rbdc)) {
		ret = PTR_ERR(rbdc);
		goto done_err;
	}

	rbd_dev->rbd_client = rbdc;
	rbd_dev->client = rbdc->client;
	return 0;
done_err:
	kfree(rbd_opts);
	return ret;
=======
		return ERR_PTR(-ENOMEM);

	rbd_opts->read_only = RBD_READ_ONLY_DEFAULT;

	opt = ceph_parse_options(options, mon_addr,
				mon_addr + mon_addr_len,
				parse_rbd_opts_token, rbd_opts);
	if (IS_ERR(opt)) {
		kfree(rbd_opts);
		return ERR_CAST(opt);
	}

	spin_lock(&rbd_client_list_lock);
	rbdc = __rbd_client_find(opt);
	if (rbdc) {
		/* using an existing client */
		kref_get(&rbdc->kref);
		spin_unlock(&rbd_client_list_lock);

		ceph_destroy_options(opt);
		kfree(rbd_opts);

		return rbdc;
	}
	spin_unlock(&rbd_client_list_lock);

	rbdc = rbd_client_create(opt, rbd_opts);

	if (IS_ERR(rbdc))
		kfree(rbd_opts);

	return rbdc;
>>>>>>> refs/remotes/origin/cm-10.0
}

/*
 * Destroy ceph client
<<<<<<< HEAD
=======
 *
 * Caller must hold rbd_client_list_lock.
>>>>>>> refs/remotes/origin/cm-10.0
 */
static void rbd_client_release(struct kref *kref)
{
	struct rbd_client *rbdc = container_of(kref, struct rbd_client, kref);

	dout("rbd_release_client %p\n", rbdc);
<<<<<<< HEAD
	spin_lock(&node_lock);
	list_del(&rbdc->node);
	spin_unlock(&node_lock);
=======
	spin_lock(&rbd_client_list_lock);
	list_del(&rbdc->node);
	spin_unlock(&rbd_client_list_lock);
>>>>>>> refs/remotes/origin/cm-10.0

	ceph_destroy_client(rbdc->client);
	kfree(rbdc->rbd_opts);
	kfree(rbdc);
}

/*
 * Drop reference to ceph client node. If it's not referenced anymore, release
 * it.
 */
static void rbd_put_client(struct rbd_device *rbd_dev)
{
	kref_put(&rbd_dev->rbd_client->kref, rbd_client_release);
	rbd_dev->rbd_client = NULL;
<<<<<<< HEAD
	rbd_dev->client = NULL;
=======
>>>>>>> refs/remotes/origin/cm-10.0
}

/*
 * Destroy requests collection
 */
static void rbd_coll_release(struct kref *kref)
{
	struct rbd_req_coll *coll =
		container_of(kref, struct rbd_req_coll, kref);

	dout("rbd_coll_release %p\n", coll);
	kfree(coll);
}

/*
 * Create a new header structure, translate header format from the on-disk
 * header.
 */
static int rbd_header_from_disk(struct rbd_image_header *header,
				 struct rbd_image_header_ondisk *ondisk,
				 int allocated_snaps,
				 gfp_t gfp_flags)
{
	int i;
<<<<<<< HEAD
	u32 snap_count = le32_to_cpu(ondisk->snap_count);
	int ret = -ENOMEM;

	init_rwsem(&header->snap_rwsem);
	header->snap_names_len = le64_to_cpu(ondisk->snap_names_len);
	header->snapc = kmalloc(sizeof(struct ceph_snap_context) +
				snap_count *
				 sizeof(struct rbd_image_snap_ondisk),
				gfp_flags);
	if (!header->snapc)
		return -ENOMEM;
=======
	u32 snap_count;

	if (memcmp(ondisk, RBD_HEADER_TEXT, sizeof(RBD_HEADER_TEXT)))
		return -ENXIO;

	snap_count = le32_to_cpu(ondisk->snap_count);
	header->snapc = kmalloc(sizeof(struct ceph_snap_context) +
				snap_count * sizeof(u64),
				gfp_flags);
	if (!header->snapc)
		return -ENOMEM;

	header->snap_names_len = le64_to_cpu(ondisk->snap_names_len);
>>>>>>> refs/remotes/origin/cm-10.0
	if (snap_count) {
		header->snap_names = kmalloc(header->snap_names_len,
					     GFP_KERNEL);
		if (!header->snap_names)
			goto err_snapc;
		header->snap_sizes = kmalloc(snap_count * sizeof(u64),
					     GFP_KERNEL);
		if (!header->snap_sizes)
			goto err_names;
	} else {
		header->snap_names = NULL;
		header->snap_sizes = NULL;
	}
	memcpy(header->block_name, ondisk->block_name,
	       sizeof(ondisk->block_name));

	header->image_size = le64_to_cpu(ondisk->image_size);
	header->obj_order = ondisk->options.order;
	header->crypt_type = ondisk->options.crypt_type;
	header->comp_type = ondisk->options.comp_type;

	atomic_set(&header->snapc->nref, 1);
	header->snap_seq = le64_to_cpu(ondisk->snap_seq);
	header->snapc->num_snaps = snap_count;
	header->total_snaps = snap_count;

<<<<<<< HEAD
	if (snap_count &&
	    allocated_snaps == snap_count) {
=======
	if (snap_count && allocated_snaps == snap_count) {
>>>>>>> refs/remotes/origin/cm-10.0
		for (i = 0; i < snap_count; i++) {
			header->snapc->snaps[i] =
				le64_to_cpu(ondisk->snaps[i].id);
			header->snap_sizes[i] =
				le64_to_cpu(ondisk->snaps[i].image_size);
		}

		/* copy snapshot names */
		memcpy(header->snap_names, &ondisk->snaps[i],
			header->snap_names_len);
	}

	return 0;

err_names:
	kfree(header->snap_names);
err_snapc:
	kfree(header->snapc);
<<<<<<< HEAD
	return ret;
}

static int snap_index(struct rbd_image_header *header, int snap_num)
{
	return header->total_snaps - snap_num;
}

static u64 cur_snap_id(struct rbd_device *rbd_dev)
{
	struct rbd_image_header *header = &rbd_dev->header;

	if (!rbd_dev->cur_snap)
		return 0;

	return header->snapc->snaps[snap_index(header, rbd_dev->cur_snap)];
=======
	return -ENOMEM;
>>>>>>> refs/remotes/origin/cm-10.0
}

static int snap_by_name(struct rbd_image_header *header, const char *snap_name,
			u64 *seq, u64 *size)
{
	int i;
	char *p = header->snap_names;

<<<<<<< HEAD
	for (i = 0; i < header->total_snaps; i++, p += strlen(p) + 1) {
		if (strcmp(snap_name, p) == 0)
			break;
	}
	if (i == header->total_snaps)
		return -ENOENT;
	if (seq)
		*seq = header->snapc->snaps[i];

	if (size)
		*size = header->snap_sizes[i];

	return i;
}

static int rbd_header_set_snap(struct rbd_device *dev,
			       const char *snap_name,
			       u64 *size)
=======
	for (i = 0; i < header->total_snaps; i++) {
		if (!strcmp(snap_name, p)) {

			/* Found it.  Pass back its id and/or size */

			if (seq)
				*seq = header->snapc->snaps[i];
			if (size)
				*size = header->snap_sizes[i];
			return i;
		}
		p += strlen(p) + 1;	/* Skip ahead to the next name */
	}
	return -ENOENT;
}

static int rbd_header_set_snap(struct rbd_device *dev, u64 *size)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct rbd_image_header *header = &dev->header;
	struct ceph_snap_context *snapc = header->snapc;
	int ret = -ENOENT;

<<<<<<< HEAD
	down_write(&header->snap_rwsem);

	if (!snap_name ||
	    !*snap_name ||
	    strcmp(snap_name, "-") == 0 ||
	    strcmp(snap_name, RBD_SNAP_HEAD_NAME) == 0) {
=======
	BUILD_BUG_ON(sizeof (dev->snap_name) < sizeof (RBD_SNAP_HEAD_NAME));

	down_write(&dev->header_rwsem);

	if (!memcmp(dev->snap_name, RBD_SNAP_HEAD_NAME,
		    sizeof (RBD_SNAP_HEAD_NAME))) {
>>>>>>> refs/remotes/origin/cm-10.0
		if (header->total_snaps)
			snapc->seq = header->snap_seq;
		else
			snapc->seq = 0;
<<<<<<< HEAD
		dev->cur_snap = 0;
		dev->read_only = 0;
		if (size)
			*size = header->image_size;
	} else {
		ret = snap_by_name(header, snap_name, &snapc->seq, size);
		if (ret < 0)
			goto done;

		dev->cur_snap = header->total_snaps - ret;
		dev->read_only = 1;
=======
		dev->snap_id = CEPH_NOSNAP;
		dev->snap_exists = false;
		dev->read_only = dev->rbd_client->rbd_opts->read_only;
		if (size)
			*size = header->image_size;
	} else {
		ret = snap_by_name(header, dev->snap_name, &snapc->seq, size);
		if (ret < 0)
			goto done;
		dev->snap_id = snapc->seq;
		dev->snap_exists = true;
		dev->read_only = true;	/* No choice for snapshots */
>>>>>>> refs/remotes/origin/cm-10.0
	}

	ret = 0;
done:
<<<<<<< HEAD
	up_write(&header->snap_rwsem);
=======
	up_write(&dev->header_rwsem);
>>>>>>> refs/remotes/origin/cm-10.0
	return ret;
}

static void rbd_header_free(struct rbd_image_header *header)
{
<<<<<<< HEAD
	kfree(header->snapc);
=======
	ceph_put_snap_context(header->snapc);
>>>>>>> refs/remotes/origin/cm-10.0
	kfree(header->snap_names);
	kfree(header->snap_sizes);
}

/*
 * get the actual striped segment name, offset and length
 */
static u64 rbd_get_segment(struct rbd_image_header *header,
			   const char *block_name,
			   u64 ofs, u64 len,
			   char *seg_name, u64 *segofs)
{
	u64 seg = ofs >> header->obj_order;

	if (seg_name)
		snprintf(seg_name, RBD_MAX_SEG_NAME_LEN,
			 "%s.%012llx", block_name, seg);

	ofs = ofs & ((1 << header->obj_order) - 1);
	len = min_t(u64, len, (1 << header->obj_order) - ofs);

	if (segofs)
		*segofs = ofs;

	return len;
}

static int rbd_get_num_segments(struct rbd_image_header *header,
				u64 ofs, u64 len)
{
	u64 start_seg = ofs >> header->obj_order;
	u64 end_seg = (ofs + len - 1) >> header->obj_order;
	return end_seg - start_seg + 1;
}

/*
<<<<<<< HEAD
=======
 * returns the size of an object in the image
 */
static u64 rbd_obj_bytes(struct rbd_image_header *header)
{
	return 1 << header->obj_order;
}

/*
>>>>>>> refs/remotes/origin/cm-10.0
 * bio helpers
 */

static void bio_chain_put(struct bio *chain)
{
	struct bio *tmp;

	while (chain) {
		tmp = chain;
		chain = chain->bi_next;
		bio_put(tmp);
	}
}

/*
 * zeros a bio chain, starting at specific offset
 */
static void zero_bio_chain(struct bio *chain, int start_ofs)
{
	struct bio_vec *bv;
	unsigned long flags;
	void *buf;
	int i;
	int pos = 0;

	while (chain) {
		bio_for_each_segment(bv, chain, i) {
			if (pos + bv->bv_len > start_ofs) {
				int remainder = max(start_ofs - pos, 0);
				buf = bvec_kmap_irq(bv, &flags);
				memset(buf + remainder, 0,
				       bv->bv_len - remainder);
				bvec_kunmap_irq(buf, &flags);
			}
			pos += bv->bv_len;
		}

		chain = chain->bi_next;
	}
}

/*
 * bio_chain_clone - clone a chain of bios up to a certain length.
 * might return a bio_pair that will need to be released.
 */
static struct bio *bio_chain_clone(struct bio **old, struct bio **next,
				   struct bio_pair **bp,
				   int len, gfp_t gfpmask)
{
	struct bio *tmp, *old_chain = *old, *new_chain = NULL, *tail = NULL;
	int total = 0;

	if (*bp) {
		bio_pair_release(*bp);
		*bp = NULL;
	}

	while (old_chain && (total < len)) {
		tmp = bio_kmalloc(gfpmask, old_chain->bi_max_vecs);
		if (!tmp)
			goto err_out;

		if (total + old_chain->bi_size > len) {
			struct bio_pair *bp;

			/*
			 * this split can only happen with a single paged bio,
			 * split_bio will BUG_ON if this is not the case
			 */
			dout("bio_chain_clone split! total=%d remaining=%d"
			     "bi_size=%d\n",
			     (int)total, (int)len-total,
			     (int)old_chain->bi_size);

			/* split the bio. We'll release it either in the next
			   call, or it will have to be released outside */
<<<<<<< HEAD
			bp = bio_split(old_chain, (len - total) / 512ULL);
=======
			bp = bio_split(old_chain, (len - total) / SECTOR_SIZE);
>>>>>>> refs/remotes/origin/cm-10.0
			if (!bp)
				goto err_out;

			__bio_clone(tmp, &bp->bio1);

			*next = &bp->bio2;
		} else {
			__bio_clone(tmp, old_chain);
			*next = old_chain->bi_next;
		}

		tmp->bi_bdev = NULL;
		gfpmask &= ~__GFP_WAIT;
		tmp->bi_next = NULL;

		if (!new_chain) {
			new_chain = tail = tmp;
		} else {
			tail->bi_next = tmp;
			tail = tmp;
		}
		old_chain = old_chain->bi_next;

		total += tmp->bi_size;
	}

	BUG_ON(total < len);

	if (tail)
		tail->bi_next = NULL;

	*old = old_chain;

	return new_chain;

err_out:
	dout("bio_chain_clone with err\n");
	bio_chain_put(new_chain);
	return NULL;
}

/*
 * helpers for osd request op vectors.
 */
static int rbd_create_rw_ops(struct ceph_osd_req_op **ops,
			    int num_ops,
			    int opcode,
			    u32 payload_len)
{
	*ops = kzalloc(sizeof(struct ceph_osd_req_op) * (num_ops + 1),
		       GFP_NOIO);
	if (!*ops)
		return -ENOMEM;
	(*ops)[0].op = opcode;
	/*
	 * op extent offset and length will be set later on
	 * in calc_raw_layout()
	 */
	(*ops)[0].payload_len = payload_len;
	return 0;
}

static void rbd_destroy_ops(struct ceph_osd_req_op *ops)
{
	kfree(ops);
}

static void rbd_coll_end_req_index(struct request *rq,
				   struct rbd_req_coll *coll,
				   int index,
				   int ret, u64 len)
{
	struct request_queue *q;
	int min, max, i;

	dout("rbd_coll_end_req_index %p index %d ret %d len %lld\n",
	     coll, index, ret, len);

	if (!rq)
		return;

	if (!coll) {
		blk_end_request(rq, ret, len);
		return;
	}

	q = rq->q;

	spin_lock_irq(q->queue_lock);
	coll->status[index].done = 1;
	coll->status[index].rc = ret;
	coll->status[index].bytes = len;
	max = min = coll->num_done;
	while (max < coll->total && coll->status[max].done)
		max++;

	for (i = min; i<max; i++) {
		__blk_end_request(rq, coll->status[i].rc,
				  coll->status[i].bytes);
		coll->num_done++;
		kref_put(&coll->kref, rbd_coll_release);
	}
	spin_unlock_irq(q->queue_lock);
}

static void rbd_coll_end_req(struct rbd_request *req,
			     int ret, u64 len)
{
	rbd_coll_end_req_index(req->rq, req->coll, req->coll_index, ret, len);
}

/*
 * Send ceph osd request
 */
static int rbd_do_request(struct request *rq,
			  struct rbd_device *dev,
			  struct ceph_snap_context *snapc,
			  u64 snapid,
			  const char *obj, u64 ofs, u64 len,
			  struct bio *bio,
			  struct page **pages,
			  int num_pages,
			  int flags,
			  struct ceph_osd_req_op *ops,
			  int num_reply,
			  struct rbd_req_coll *coll,
			  int coll_index,
			  void (*rbd_cb)(struct ceph_osd_request *req,
					 struct ceph_msg *msg),
			  struct ceph_osd_request **linger_req,
			  u64 *ver)
{
	struct ceph_osd_request *req;
	struct ceph_file_layout *layout;
	int ret;
	u64 bno;
	struct timespec mtime = CURRENT_TIME;
	struct rbd_request *req_data;
	struct ceph_osd_request_head *reqhead;
<<<<<<< HEAD
	struct rbd_image_header *header = &dev->header;
=======
	struct ceph_osd_client *osdc;
>>>>>>> refs/remotes/origin/cm-10.0

	req_data = kzalloc(sizeof(*req_data), GFP_NOIO);
	if (!req_data) {
		if (coll)
			rbd_coll_end_req_index(rq, coll, coll_index,
					       -ENOMEM, len);
		return -ENOMEM;
	}

	if (coll) {
		req_data->coll = coll;
		req_data->coll_index = coll_index;
	}

	dout("rbd_do_request obj=%s ofs=%lld len=%lld\n", obj, len, ofs);

<<<<<<< HEAD
	down_read(&header->snap_rwsem);

	req = ceph_osdc_alloc_request(&dev->client->osdc, flags,
				      snapc,
				      ops,
				      false,
				      GFP_NOIO, pages, bio);
	if (!req) {
		up_read(&header->snap_rwsem);
=======
	osdc = &dev->rbd_client->client->osdc;
	req = ceph_osdc_alloc_request(osdc, flags, snapc, ops,
					false, GFP_NOIO, pages, bio);
	if (!req) {
>>>>>>> refs/remotes/origin/cm-10.0
		ret = -ENOMEM;
		goto done_pages;
	}

	req->r_callback = rbd_cb;

	req_data->rq = rq;
	req_data->bio = bio;
	req_data->pages = pages;
	req_data->len = len;

	req->r_priv = req_data;

	reqhead = req->r_request->front.iov_base;
	reqhead->snapid = cpu_to_le64(CEPH_NOSNAP);

	strncpy(req->r_oid, obj, sizeof(req->r_oid));
	req->r_oid_len = strlen(req->r_oid);

	layout = &req->r_file_layout;
	memset(layout, 0, sizeof(*layout));
	layout->fl_stripe_unit = cpu_to_le32(1 << RBD_MAX_OBJ_ORDER);
	layout->fl_stripe_count = cpu_to_le32(1);
	layout->fl_object_size = cpu_to_le32(1 << RBD_MAX_OBJ_ORDER);
	layout->fl_pg_preferred = cpu_to_le32(-1);
	layout->fl_pg_pool = cpu_to_le32(dev->poolid);
<<<<<<< HEAD
	ceph_calc_raw_layout(&dev->client->osdc, layout, snapid,
			     ofs, &len, &bno, req, ops);
=======
	ret = ceph_calc_raw_layout(osdc, layout, snapid, ofs, &len, &bno,
				   req, ops);
	BUG_ON(ret != 0);
>>>>>>> refs/remotes/origin/cm-10.0

	ceph_osdc_build_request(req, ofs, &len,
				ops,
				snapc,
				&mtime,
				req->r_oid, req->r_oid_len);
<<<<<<< HEAD
	up_read(&header->snap_rwsem);

	if (linger_req) {
		ceph_osdc_set_request_linger(&dev->client->osdc, req);
		*linger_req = req;
	}

	ret = ceph_osdc_start_request(&dev->client->osdc, req, false);
=======

	if (linger_req) {
		ceph_osdc_set_request_linger(osdc, req);
		*linger_req = req;
	}

	ret = ceph_osdc_start_request(osdc, req, false);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret < 0)
		goto done_err;

	if (!rbd_cb) {
<<<<<<< HEAD
		ret = ceph_osdc_wait_request(&dev->client->osdc, req);
=======
		ret = ceph_osdc_wait_request(osdc, req);
>>>>>>> refs/remotes/origin/cm-10.0
		if (ver)
			*ver = le64_to_cpu(req->r_reassert_version.version);
		dout("reassert_ver=%lld\n",
		     le64_to_cpu(req->r_reassert_version.version));
		ceph_osdc_put_request(req);
	}
	return ret;

done_err:
	bio_chain_put(req_data->bio);
	ceph_osdc_put_request(req);
done_pages:
	rbd_coll_end_req(req_data, ret, len);
	kfree(req_data);
	return ret;
}

/*
 * Ceph osd op callback
 */
static void rbd_req_cb(struct ceph_osd_request *req, struct ceph_msg *msg)
{
	struct rbd_request *req_data = req->r_priv;
	struct ceph_osd_reply_head *replyhead;
	struct ceph_osd_op *op;
	__s32 rc;
	u64 bytes;
	int read_op;

	/* parse reply */
	replyhead = msg->front.iov_base;
	WARN_ON(le32_to_cpu(replyhead->num_ops) == 0);
	op = (void *)(replyhead + 1);
	rc = le32_to_cpu(replyhead->result);
	bytes = le64_to_cpu(op->extent.length);
	read_op = (le32_to_cpu(op->op) == CEPH_OSD_OP_READ);

	dout("rbd_req_cb bytes=%lld readop=%d rc=%d\n", bytes, read_op, rc);

	if (rc == -ENOENT && read_op) {
		zero_bio_chain(req_data->bio, 0);
		rc = 0;
	} else if (rc == 0 && read_op && bytes < req_data->len) {
		zero_bio_chain(req_data->bio, bytes);
		bytes = req_data->len;
	}

	rbd_coll_end_req(req_data, rc, bytes);

	if (req_data->bio)
		bio_chain_put(req_data->bio);

	ceph_osdc_put_request(req);
	kfree(req_data);
}

static void rbd_simple_req_cb(struct ceph_osd_request *req, struct ceph_msg *msg)
{
	ceph_osdc_put_request(req);
}

/*
 * Do a synchronous ceph osd operation
 */
static int rbd_req_sync_op(struct rbd_device *dev,
			   struct ceph_snap_context *snapc,
			   u64 snapid,
			   int opcode,
			   int flags,
			   struct ceph_osd_req_op *orig_ops,
			   int num_reply,
			   const char *obj,
			   u64 ofs, u64 len,
			   char *buf,
			   struct ceph_osd_request **linger_req,
			   u64 *ver)
{
	int ret;
	struct page **pages;
	int num_pages;
	struct ceph_osd_req_op *ops = orig_ops;
	u32 payload_len;

	num_pages = calc_pages_for(ofs , len);
	pages = ceph_alloc_page_vector(num_pages, GFP_KERNEL);
	if (IS_ERR(pages))
		return PTR_ERR(pages);

	if (!orig_ops) {
		payload_len = (flags & CEPH_OSD_FLAG_WRITE ? len : 0);
		ret = rbd_create_rw_ops(&ops, 1, opcode, payload_len);
		if (ret < 0)
			goto done;

		if ((flags & CEPH_OSD_FLAG_WRITE) && buf) {
			ret = ceph_copy_to_page_vector(pages, buf, ofs, len);
			if (ret < 0)
				goto done_ops;
		}
	}

	ret = rbd_do_request(NULL, dev, snapc, snapid,
			  obj, ofs, len, NULL,
			  pages, num_pages,
			  flags,
			  ops,
			  2,
			  NULL, 0,
			  NULL,
			  linger_req, ver);
	if (ret < 0)
		goto done_ops;

	if ((flags & CEPH_OSD_FLAG_READ) && buf)
		ret = ceph_copy_from_page_vector(pages, buf, ofs, ret);

done_ops:
	if (!orig_ops)
		rbd_destroy_ops(ops);
done:
	ceph_release_page_vector(pages, num_pages);
	return ret;
}

/*
 * Do an asynchronous ceph osd operation
 */
static int rbd_do_op(struct request *rq,
		     struct rbd_device *rbd_dev ,
		     struct ceph_snap_context *snapc,
		     u64 snapid,
		     int opcode, int flags, int num_reply,
		     u64 ofs, u64 len,
		     struct bio *bio,
		     struct rbd_req_coll *coll,
		     int coll_index)
{
	char *seg_name;
	u64 seg_ofs;
	u64 seg_len;
	int ret;
	struct ceph_osd_req_op *ops;
	u32 payload_len;

	seg_name = kmalloc(RBD_MAX_SEG_NAME_LEN + 1, GFP_NOIO);
	if (!seg_name)
		return -ENOMEM;

	seg_len = rbd_get_segment(&rbd_dev->header,
				  rbd_dev->header.block_name,
				  ofs, len,
				  seg_name, &seg_ofs);

	payload_len = (flags & CEPH_OSD_FLAG_WRITE ? seg_len : 0);

	ret = rbd_create_rw_ops(&ops, 1, opcode, payload_len);
	if (ret < 0)
		goto done;

	/* we've taken care of segment sizes earlier when we
	   cloned the bios. We should never have a segment
	   truncated at this point */
	BUG_ON(seg_len < len);

	ret = rbd_do_request(rq, rbd_dev, snapc, snapid,
			     seg_name, seg_ofs, seg_len,
			     bio,
			     NULL, 0,
			     flags,
			     ops,
			     num_reply,
			     coll, coll_index,
			     rbd_req_cb, 0, NULL);

	rbd_destroy_ops(ops);
done:
	kfree(seg_name);
	return ret;
}

/*
 * Request async osd write
 */
static int rbd_req_write(struct request *rq,
			 struct rbd_device *rbd_dev,
			 struct ceph_snap_context *snapc,
			 u64 ofs, u64 len,
			 struct bio *bio,
			 struct rbd_req_coll *coll,
			 int coll_index)
{
	return rbd_do_op(rq, rbd_dev, snapc, CEPH_NOSNAP,
			 CEPH_OSD_OP_WRITE,
			 CEPH_OSD_FLAG_WRITE | CEPH_OSD_FLAG_ONDISK,
			 2,
			 ofs, len, bio, coll, coll_index);
}

/*
 * Request async osd read
 */
static int rbd_req_read(struct request *rq,
			 struct rbd_device *rbd_dev,
			 u64 snapid,
			 u64 ofs, u64 len,
			 struct bio *bio,
			 struct rbd_req_coll *coll,
			 int coll_index)
{
	return rbd_do_op(rq, rbd_dev, NULL,
			 (snapid ? snapid : CEPH_NOSNAP),
			 CEPH_OSD_OP_READ,
			 CEPH_OSD_FLAG_READ,
			 2,
			 ofs, len, bio, coll, coll_index);
}

/*
 * Request sync osd read
 */
static int rbd_req_sync_read(struct rbd_device *dev,
			  struct ceph_snap_context *snapc,
			  u64 snapid,
			  const char *obj,
			  u64 ofs, u64 len,
			  char *buf,
			  u64 *ver)
{
	return rbd_req_sync_op(dev, NULL,
			       (snapid ? snapid : CEPH_NOSNAP),
			       CEPH_OSD_OP_READ,
			       CEPH_OSD_FLAG_READ,
			       NULL,
			       1, obj, ofs, len, buf, NULL, ver);
}

/*
 * Request sync osd watch
 */
static int rbd_req_sync_notify_ack(struct rbd_device *dev,
				   u64 ver,
				   u64 notify_id,
				   const char *obj)
{
	struct ceph_osd_req_op *ops;
	struct page **pages = NULL;
	int ret;

	ret = rbd_create_rw_ops(&ops, 1, CEPH_OSD_OP_NOTIFY_ACK, 0);
	if (ret < 0)
		return ret;

<<<<<<< HEAD
	ops[0].watch.ver = cpu_to_le64(dev->header.obj_version);
=======
	ops[0].watch.ver = cpu_to_le64(ver);
>>>>>>> refs/remotes/origin/cm-10.0
	ops[0].watch.cookie = notify_id;
	ops[0].watch.flag = 0;

	ret = rbd_do_request(NULL, dev, NULL, CEPH_NOSNAP,
			  obj, 0, 0, NULL,
			  pages, 0,
			  CEPH_OSD_FLAG_READ,
			  ops,
			  1,
			  NULL, 0,
			  rbd_simple_req_cb, 0, NULL);

	rbd_destroy_ops(ops);
	return ret;
}

static void rbd_watch_cb(u64 ver, u64 notify_id, u8 opcode, void *data)
{
	struct rbd_device *dev = (struct rbd_device *)data;
<<<<<<< HEAD
=======
	u64 hver;
>>>>>>> refs/remotes/origin/cm-10.0
	int rc;

	if (!dev)
		return;

	dout("rbd_watch_cb %s notify_id=%lld opcode=%d\n", dev->obj_md_name,
		notify_id, (int)opcode);
	mutex_lock_nested(&ctl_mutex, SINGLE_DEPTH_NESTING);
	rc = __rbd_update_snaps(dev);
<<<<<<< HEAD
	mutex_unlock(&ctl_mutex);
	if (rc)
		pr_warning(DRV_NAME "%d got notification but failed to update"
			   " snaps: %d\n", dev->major, rc);

	rbd_req_sync_notify_ack(dev, ver, notify_id, dev->obj_md_name);
=======
	hver = dev->header.obj_version;
	mutex_unlock(&ctl_mutex);
	if (rc)
		pr_warning(RBD_DRV_NAME "%d got notification but failed to "
			   " update snaps: %d\n", dev->major, rc);

	rbd_req_sync_notify_ack(dev, hver, notify_id, dev->obj_md_name);
>>>>>>> refs/remotes/origin/cm-10.0
}

/*
 * Request sync osd watch
 */
static int rbd_req_sync_watch(struct rbd_device *dev,
			      const char *obj,
			      u64 ver)
{
	struct ceph_osd_req_op *ops;
<<<<<<< HEAD
	struct ceph_osd_client *osdc = &dev->client->osdc;
=======
	struct ceph_osd_client *osdc = &dev->rbd_client->client->osdc;
>>>>>>> refs/remotes/origin/cm-10.0

	int ret = rbd_create_rw_ops(&ops, 1, CEPH_OSD_OP_WATCH, 0);
	if (ret < 0)
		return ret;

	ret = ceph_osdc_create_event(osdc, rbd_watch_cb, 0,
				     (void *)dev, &dev->watch_event);
	if (ret < 0)
		goto fail;

	ops[0].watch.ver = cpu_to_le64(ver);
	ops[0].watch.cookie = cpu_to_le64(dev->watch_event->cookie);
	ops[0].watch.flag = 1;

	ret = rbd_req_sync_op(dev, NULL,
			      CEPH_NOSNAP,
			      0,
			      CEPH_OSD_FLAG_WRITE | CEPH_OSD_FLAG_ONDISK,
			      ops,
			      1, obj, 0, 0, NULL,
			      &dev->watch_request, NULL);

	if (ret < 0)
		goto fail_event;

	rbd_destroy_ops(ops);
	return 0;

fail_event:
	ceph_osdc_cancel_event(dev->watch_event);
	dev->watch_event = NULL;
fail:
	rbd_destroy_ops(ops);
	return ret;
}

<<<<<<< HEAD
struct rbd_notify_info {
	struct rbd_device *dev;
};

static void rbd_notify_cb(u64 ver, u64 notify_id, u8 opcode, void *data)
{
	struct rbd_device *dev = (struct rbd_device *)data;
	if (!dev)
		return;

	dout("rbd_notify_cb %s notify_id=%lld opcode=%d\n", dev->obj_md_name,
		notify_id, (int)opcode);
}

/*
 * Request sync osd notify
 */
static int rbd_req_sync_notify(struct rbd_device *dev,
		          const char *obj)
{
	struct ceph_osd_req_op *ops;
	struct ceph_osd_client *osdc = &dev->client->osdc;
	struct ceph_osd_event *event;
	struct rbd_notify_info info;
	int payload_len = sizeof(u32) + sizeof(u32);
	int ret;

	ret = rbd_create_rw_ops(&ops, 1, CEPH_OSD_OP_NOTIFY, payload_len);
	if (ret < 0)
		return ret;

	info.dev = dev;

	ret = ceph_osdc_create_event(osdc, rbd_notify_cb, 1,
				     (void *)&info, &event);
	if (ret < 0)
		goto fail;

	ops[0].watch.ver = 1;
	ops[0].watch.flag = 1;
	ops[0].watch.cookie = event->cookie;
	ops[0].watch.prot_ver = RADOS_NOTIFY_VER;
	ops[0].watch.timeout = 12;

	ret = rbd_req_sync_op(dev, NULL,
			       CEPH_NOSNAP,
			       0,
			       CEPH_OSD_FLAG_WRITE | CEPH_OSD_FLAG_ONDISK,
			       ops,
			       1, obj, 0, 0, NULL, NULL, NULL);
	if (ret < 0)
		goto fail_event;

	ret = ceph_osdc_wait_event(event, CEPH_OSD_TIMEOUT_DEFAULT);
	dout("ceph_osdc_wait_event returned %d\n", ret);
	rbd_destroy_ops(ops);
	return 0;

fail_event:
	ceph_osdc_cancel_event(event);
fail:
	rbd_destroy_ops(ops);
	return ret;
}

/*
 * Request sync osd rollback
 */
static int rbd_req_sync_rollback_obj(struct rbd_device *dev,
				     u64 snapid,
				     const char *obj)
{
	struct ceph_osd_req_op *ops;
	int ret = rbd_create_rw_ops(&ops, 1, CEPH_OSD_OP_ROLLBACK, 0);
	if (ret < 0)
		return ret;

	ops[0].snap.snapid = snapid;

	ret = rbd_req_sync_op(dev, NULL,
			       CEPH_NOSNAP,
			       0,
			       CEPH_OSD_FLAG_WRITE | CEPH_OSD_FLAG_ONDISK,
			       ops,
			       1, obj, 0, 0, NULL, NULL, NULL);

	rbd_destroy_ops(ops);

	return ret;
}

=======
/*
 * Request sync osd unwatch
 */
static int rbd_req_sync_unwatch(struct rbd_device *dev,
				const char *obj)
{
	struct ceph_osd_req_op *ops;

	int ret = rbd_create_rw_ops(&ops, 1, CEPH_OSD_OP_WATCH, 0);
	if (ret < 0)
		return ret;

	ops[0].watch.ver = 0;
	ops[0].watch.cookie = cpu_to_le64(dev->watch_event->cookie);
	ops[0].watch.flag = 0;

	ret = rbd_req_sync_op(dev, NULL,
			      CEPH_NOSNAP,
			      0,
			      CEPH_OSD_FLAG_WRITE | CEPH_OSD_FLAG_ONDISK,
			      ops,
			      1, obj, 0, 0, NULL, NULL, NULL);

	rbd_destroy_ops(ops);
	ceph_osdc_cancel_event(dev->watch_event);
	dev->watch_event = NULL;
	return ret;
}

#if 0
>>>>>>> refs/remotes/origin/cm-10.0
/*
 * Request sync osd read
 */
static int rbd_req_sync_exec(struct rbd_device *dev,
			     const char *obj,
			     const char *cls,
			     const char *method,
			     const char *data,
			     int len,
			     u64 *ver)
{
	struct ceph_osd_req_op *ops;
	int cls_len = strlen(cls);
	int method_len = strlen(method);
	int ret = rbd_create_rw_ops(&ops, 1, CEPH_OSD_OP_CALL,
				    cls_len + method_len + len);
	if (ret < 0)
		return ret;

	ops[0].cls.class_name = cls;
	ops[0].cls.class_len = (__u8)cls_len;
	ops[0].cls.method_name = method;
	ops[0].cls.method_len = (__u8)method_len;
	ops[0].cls.argc = 0;
	ops[0].cls.indata = data;
	ops[0].cls.indata_len = len;

	ret = rbd_req_sync_op(dev, NULL,
			       CEPH_NOSNAP,
			       0,
			       CEPH_OSD_FLAG_WRITE | CEPH_OSD_FLAG_ONDISK,
			       ops,
			       1, obj, 0, 0, NULL, NULL, ver);

	rbd_destroy_ops(ops);

	dout("cls_exec returned %d\n", ret);
	return ret;
}
<<<<<<< HEAD
=======
#endif
>>>>>>> refs/remotes/origin/cm-10.0

static struct rbd_req_coll *rbd_alloc_coll(int num_reqs)
{
	struct rbd_req_coll *coll =
			kzalloc(sizeof(struct rbd_req_coll) +
			        sizeof(struct rbd_req_status) * num_reqs,
				GFP_ATOMIC);

	if (!coll)
		return NULL;
	coll->total = num_reqs;
	kref_init(&coll->kref);
	return coll;
}

/*
 * block device queue callback
 */
static void rbd_rq_fn(struct request_queue *q)
{
	struct rbd_device *rbd_dev = q->queuedata;
	struct request *rq;
	struct bio_pair *bp = NULL;

<<<<<<< HEAD
	rq = blk_fetch_request(q);

	while (1) {
=======
	while ((rq = blk_fetch_request(q))) {
>>>>>>> refs/remotes/origin/cm-10.0
		struct bio *bio;
		struct bio *rq_bio, *next_bio = NULL;
		bool do_write;
		int size, op_size = 0;
		u64 ofs;
		int num_segs, cur_seg = 0;
		struct rbd_req_coll *coll;
<<<<<<< HEAD
=======
		struct ceph_snap_context *snapc;
>>>>>>> refs/remotes/origin/cm-10.0

		/* peek at request from block layer */
		if (!rq)
			break;

		dout("fetched request\n");

		/* filter out block requests we don't understand */
		if ((rq->cmd_type != REQ_TYPE_FS)) {
			__blk_end_request_all(rq, 0);
<<<<<<< HEAD
			goto next;
=======
			continue;
>>>>>>> refs/remotes/origin/cm-10.0
		}

		/* deduce our operation (read, write) */
		do_write = (rq_data_dir(rq) == WRITE);

		size = blk_rq_bytes(rq);
<<<<<<< HEAD
		ofs = blk_rq_pos(rq) * 512ULL;
		rq_bio = rq->bio;
		if (do_write && rbd_dev->read_only) {
			__blk_end_request_all(rq, -EROFS);
			goto next;
=======
		ofs = blk_rq_pos(rq) * SECTOR_SIZE;
		rq_bio = rq->bio;
		if (do_write && rbd_dev->read_only) {
			__blk_end_request_all(rq, -EROFS);
			continue;
>>>>>>> refs/remotes/origin/cm-10.0
		}

		spin_unlock_irq(q->queue_lock);

<<<<<<< HEAD
		dout("%s 0x%x bytes at 0x%llx\n",
		     do_write ? "write" : "read",
		     size, blk_rq_pos(rq) * 512ULL);
=======
		down_read(&rbd_dev->header_rwsem);

		if (rbd_dev->snap_id != CEPH_NOSNAP && !rbd_dev->snap_exists) {
			up_read(&rbd_dev->header_rwsem);
			dout("request for non-existent snapshot");
			spin_lock_irq(q->queue_lock);
			__blk_end_request_all(rq, -ENXIO);
			continue;
		}

		snapc = ceph_get_snap_context(rbd_dev->header.snapc);

		up_read(&rbd_dev->header_rwsem);

		dout("%s 0x%x bytes at 0x%llx\n",
		     do_write ? "write" : "read",
		     size, blk_rq_pos(rq) * SECTOR_SIZE);
>>>>>>> refs/remotes/origin/cm-10.0

		num_segs = rbd_get_num_segments(&rbd_dev->header, ofs, size);
		coll = rbd_alloc_coll(num_segs);
		if (!coll) {
			spin_lock_irq(q->queue_lock);
			__blk_end_request_all(rq, -ENOMEM);
<<<<<<< HEAD
			goto next;
=======
			ceph_put_snap_context(snapc);
			continue;
>>>>>>> refs/remotes/origin/cm-10.0
		}

		do {
			/* a bio clone to be passed down to OSD req */
			dout("rq->bio->bi_vcnt=%d\n", rq->bio->bi_vcnt);
			op_size = rbd_get_segment(&rbd_dev->header,
						  rbd_dev->header.block_name,
						  ofs, size,
						  NULL, NULL);
			kref_get(&coll->kref);
			bio = bio_chain_clone(&rq_bio, &next_bio, &bp,
					      op_size, GFP_ATOMIC);
			if (!bio) {
				rbd_coll_end_req_index(rq, coll, cur_seg,
						       -ENOMEM, op_size);
				goto next_seg;
			}


			/* init OSD command: write or read */
			if (do_write)
				rbd_req_write(rq, rbd_dev,
<<<<<<< HEAD
					      rbd_dev->header.snapc,
=======
					      snapc,
>>>>>>> refs/remotes/origin/cm-10.0
					      ofs,
					      op_size, bio,
					      coll, cur_seg);
			else
				rbd_req_read(rq, rbd_dev,
<<<<<<< HEAD
					     cur_snap_id(rbd_dev),
=======
					     rbd_dev->snap_id,
>>>>>>> refs/remotes/origin/cm-10.0
					     ofs,
					     op_size, bio,
					     coll, cur_seg);

next_seg:
			size -= op_size;
			ofs += op_size;

			cur_seg++;
			rq_bio = next_bio;
		} while (size > 0);
		kref_put(&coll->kref, rbd_coll_release);

		if (bp)
			bio_pair_release(bp);
		spin_lock_irq(q->queue_lock);
<<<<<<< HEAD
next:
		rq = blk_fetch_request(q);
=======

		ceph_put_snap_context(snapc);
>>>>>>> refs/remotes/origin/cm-10.0
	}
}

/*
 * a queue callback. Makes sure that we don't create a bio that spans across
 * multiple osd objects. One exception would be with a single page bios,
 * which we handle later at bio_chain_clone
 */
static int rbd_merge_bvec(struct request_queue *q, struct bvec_merge_data *bmd,
			  struct bio_vec *bvec)
{
	struct rbd_device *rbd_dev = q->queuedata;
<<<<<<< HEAD
	unsigned int chunk_sectors = 1 << (rbd_dev->header.obj_order - 9);
	sector_t sector = bmd->bi_sector + get_start_sect(bmd->bi_bdev);
	unsigned int bio_sectors = bmd->bi_size >> 9;
	int max;

	max =  (chunk_sectors - ((sector & (chunk_sectors - 1))
				 + bio_sectors)) << 9;
=======
	unsigned int chunk_sectors;
	sector_t sector;
	unsigned int bio_sectors;
	int max;

	chunk_sectors = 1 << (rbd_dev->header.obj_order - SECTOR_SHIFT);
	sector = bmd->bi_sector + get_start_sect(bmd->bi_bdev);
	bio_sectors = bmd->bi_size >> SECTOR_SHIFT;

	max =  (chunk_sectors - ((sector & (chunk_sectors - 1))
				 + bio_sectors)) << SECTOR_SHIFT;
>>>>>>> refs/remotes/origin/cm-10.0
	if (max < 0)
		max = 0; /* bio_add cannot handle a negative return */
	if (max <= bvec->bv_len && bio_sectors == 0)
		return bvec->bv_len;
	return max;
}

static void rbd_free_disk(struct rbd_device *rbd_dev)
{
	struct gendisk *disk = rbd_dev->disk;

	if (!disk)
		return;

	rbd_header_free(&rbd_dev->header);

	if (disk->flags & GENHD_FL_UP)
		del_gendisk(disk);
	if (disk->queue)
		blk_cleanup_queue(disk->queue);
	put_disk(disk);
}

/*
 * reload the ondisk the header 
 */
static int rbd_read_header(struct rbd_device *rbd_dev,
			   struct rbd_image_header *header)
{
	ssize_t rc;
	struct rbd_image_header_ondisk *dh;
	int snap_count = 0;
<<<<<<< HEAD
	u64 snap_names_len = 0;
	u64 ver;

	while (1) {
		int len = sizeof(*dh) +
			  snap_count * sizeof(struct rbd_image_snap_ondisk) +
			  snap_names_len;

		rc = -ENOMEM;
=======
	u64 ver;
	size_t len;

	/*
	 * First reads the fixed-size header to determine the number
	 * of snapshots, then re-reads it, along with all snapshot
	 * records as well as their stored names.
	 */
	len = sizeof (*dh);
	while (1) {
>>>>>>> refs/remotes/origin/cm-10.0
		dh = kmalloc(len, GFP_KERNEL);
		if (!dh)
			return -ENOMEM;

		rc = rbd_req_sync_read(rbd_dev,
				       NULL, CEPH_NOSNAP,
				       rbd_dev->obj_md_name,
				       0, len,
				       (char *)dh, &ver);
		if (rc < 0)
			goto out_dh;

		rc = rbd_header_from_disk(header, dh, snap_count, GFP_KERNEL);
<<<<<<< HEAD
		if (rc < 0)
			goto out_dh;

		if (snap_count != header->total_snaps) {
			snap_count = header->total_snaps;
			snap_names_len = header->snap_names_len;
			rbd_header_free(header);
			kfree(dh);
			continue;
		}
		break;
=======
		if (rc < 0) {
			if (rc == -ENXIO)
				pr_warning("unrecognized header format"
					   " for image %s", rbd_dev->obj);
			goto out_dh;
		}

		if (snap_count == header->total_snaps)
			break;

		snap_count = header->total_snaps;
		len = sizeof (*dh) +
			snap_count * sizeof(struct rbd_image_snap_ondisk) +
			header->snap_names_len;

		rbd_header_free(header);
		kfree(dh);
>>>>>>> refs/remotes/origin/cm-10.0
	}
	header->obj_version = ver;

out_dh:
	kfree(dh);
	return rc;
}

<<<<<<< HEAD
/*
 * create a snapshot
 */
static int rbd_header_add_snap(struct rbd_device *dev,
			       const char *snap_name,
			       gfp_t gfp_flags)
{
	int name_len = strlen(snap_name);
	u64 new_snapid;
	int ret;
	void *data, *p, *e;
	u64 ver;

	/* we should create a snapshot only if we're pointing at the head */
	if (dev->cur_snap)
		return -EINVAL;

	ret = ceph_monc_create_snapid(&dev->client->monc, dev->poolid,
				      &new_snapid);
	dout("created snapid=%lld\n", new_snapid);
	if (ret < 0)
		return ret;

	data = kmalloc(name_len + 16, gfp_flags);
	if (!data)
		return -ENOMEM;

	p = data;
	e = data + name_len + 16;

	ceph_encode_string_safe(&p, e, snap_name, name_len, bad);
	ceph_encode_64_safe(&p, e, new_snapid, bad);

	ret = rbd_req_sync_exec(dev, dev->obj_md_name, "rbd", "snap_add",
				data, p - data, &ver);

	kfree(data);

	if (ret < 0)
		return ret;

	dev->header.snapc->seq =  new_snapid;

	return 0;
bad:
	return -ERANGE;
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
static void __rbd_remove_all_snaps(struct rbd_device *rbd_dev)
{
	struct rbd_snap *snap;

	while (!list_empty(&rbd_dev->snaps)) {
		snap = list_first_entry(&rbd_dev->snaps, struct rbd_snap, node);
		__rbd_remove_snap_dev(rbd_dev, snap);
	}
}

/*
 * only read the first part of the ondisk header, without the snaps info
 */
static int __rbd_update_snaps(struct rbd_device *rbd_dev)
{
	int ret;
	struct rbd_image_header h;
	u64 snap_seq;
	int follow_seq = 0;

	ret = rbd_read_header(rbd_dev, &h);
	if (ret < 0)
		return ret;

<<<<<<< HEAD
	/* resized? */
	set_capacity(rbd_dev->disk, h.image_size / 512ULL);

	down_write(&rbd_dev->header.snap_rwsem);
=======
	down_write(&rbd_dev->header_rwsem);

	/* resized? */
	if (rbd_dev->snap_id == CEPH_NOSNAP) {
		sector_t size = (sector_t) h.image_size / SECTOR_SIZE;

		dout("setting size to %llu sectors", (unsigned long long) size);
		set_capacity(rbd_dev->disk, size);
	}
>>>>>>> refs/remotes/origin/cm-10.0

	snap_seq = rbd_dev->header.snapc->seq;
	if (rbd_dev->header.total_snaps &&
	    rbd_dev->header.snapc->snaps[0] == snap_seq)
		/* pointing at the head, will need to follow that
		   if head moves */
		follow_seq = 1;

<<<<<<< HEAD
	kfree(rbd_dev->header.snapc);
	kfree(rbd_dev->header.snap_names);
	kfree(rbd_dev->header.snap_sizes);

=======
	ceph_put_snap_context(rbd_dev->header.snapc);
	kfree(rbd_dev->header.snap_names);
	kfree(rbd_dev->header.snap_sizes);

	rbd_dev->header.obj_version = h.obj_version;
	rbd_dev->header.image_size = h.image_size;
>>>>>>> refs/remotes/origin/cm-10.0
	rbd_dev->header.total_snaps = h.total_snaps;
	rbd_dev->header.snapc = h.snapc;
	rbd_dev->header.snap_names = h.snap_names;
	rbd_dev->header.snap_names_len = h.snap_names_len;
	rbd_dev->header.snap_sizes = h.snap_sizes;
	if (follow_seq)
		rbd_dev->header.snapc->seq = rbd_dev->header.snapc->snaps[0];
	else
		rbd_dev->header.snapc->seq = snap_seq;

	ret = __rbd_init_snaps_header(rbd_dev);

<<<<<<< HEAD
	up_write(&rbd_dev->header.snap_rwsem);
=======
	up_write(&rbd_dev->header_rwsem);
>>>>>>> refs/remotes/origin/cm-10.0

	return ret;
}

static int rbd_init_disk(struct rbd_device *rbd_dev)
{
	struct gendisk *disk;
	struct request_queue *q;
	int rc;
<<<<<<< HEAD
=======
	u64 segment_size;
>>>>>>> refs/remotes/origin/cm-10.0
	u64 total_size = 0;

	/* contact OSD, request size info about the object being mapped */
	rc = rbd_read_header(rbd_dev, &rbd_dev->header);
	if (rc)
		return rc;

	/* no need to lock here, as rbd_dev is not registered yet */
	rc = __rbd_init_snaps_header(rbd_dev);
	if (rc)
		return rc;

<<<<<<< HEAD
	rc = rbd_header_set_snap(rbd_dev, rbd_dev->snap_name, &total_size);
=======
	rc = rbd_header_set_snap(rbd_dev, &total_size);
>>>>>>> refs/remotes/origin/cm-10.0
	if (rc)
		return rc;

	/* create gendisk info */
	rc = -ENOMEM;
	disk = alloc_disk(RBD_MINORS_PER_MAJOR);
	if (!disk)
		goto out;

<<<<<<< HEAD
	snprintf(disk->disk_name, sizeof(disk->disk_name), DRV_NAME "%d",
=======
	snprintf(disk->disk_name, sizeof(disk->disk_name), RBD_DRV_NAME "%d",
>>>>>>> refs/remotes/origin/cm-10.0
		 rbd_dev->id);
	disk->major = rbd_dev->major;
	disk->first_minor = 0;
	disk->fops = &rbd_bd_ops;
	disk->private_data = rbd_dev;

	/* init rq */
	rc = -ENOMEM;
	q = blk_init_queue(rbd_rq_fn, &rbd_dev->lock);
	if (!q)
		goto out_disk;
<<<<<<< HEAD
=======

	/* We use the default size, but let's be explicit about it. */
	blk_queue_physical_block_size(q, SECTOR_SIZE);

	/* set io sizes to object size */
	segment_size = rbd_obj_bytes(&rbd_dev->header);
	blk_queue_max_hw_sectors(q, segment_size / SECTOR_SIZE);
	blk_queue_max_segment_size(q, segment_size);
	blk_queue_io_min(q, segment_size);
	blk_queue_io_opt(q, segment_size);

>>>>>>> refs/remotes/origin/cm-10.0
	blk_queue_merge_bvec(q, rbd_merge_bvec);
	disk->queue = q;

	q->queuedata = rbd_dev;

	rbd_dev->disk = disk;
	rbd_dev->q = q;

	/* finally, announce the disk to the world */
<<<<<<< HEAD
	set_capacity(disk, total_size / 512ULL);
=======
	set_capacity(disk, total_size / SECTOR_SIZE);
>>>>>>> refs/remotes/origin/cm-10.0
	add_disk(disk);

	pr_info("%s: added with size 0x%llx\n",
		disk->disk_name, (unsigned long long)total_size);
	return 0;

out_disk:
	put_disk(disk);
out:
	return rc;
}

/*
  sysfs
*/

<<<<<<< HEAD
static ssize_t rbd_size_show(struct device *dev,
			     struct device_attribute *attr, char *buf)
{
	struct rbd_device *rbd_dev = dev_to_rbd(dev);

	return sprintf(buf, "%llu\n", (unsigned long long)rbd_dev->header.image_size);
=======
static struct rbd_device *dev_to_rbd_dev(struct device *dev)
{
	return container_of(dev, struct rbd_device, dev);
}

static ssize_t rbd_size_show(struct device *dev,
			     struct device_attribute *attr, char *buf)
{
	struct rbd_device *rbd_dev = dev_to_rbd_dev(dev);
	sector_t size;

	down_read(&rbd_dev->header_rwsem);
	size = get_capacity(rbd_dev->disk);
	up_read(&rbd_dev->header_rwsem);

	return sprintf(buf, "%llu\n", (unsigned long long) size * SECTOR_SIZE);
>>>>>>> refs/remotes/origin/cm-10.0
}

static ssize_t rbd_major_show(struct device *dev,
			      struct device_attribute *attr, char *buf)
{
<<<<<<< HEAD
	struct rbd_device *rbd_dev = dev_to_rbd(dev);
=======
	struct rbd_device *rbd_dev = dev_to_rbd_dev(dev);
>>>>>>> refs/remotes/origin/cm-10.0

	return sprintf(buf, "%d\n", rbd_dev->major);
}

static ssize_t rbd_client_id_show(struct device *dev,
				  struct device_attribute *attr, char *buf)
{
<<<<<<< HEAD
	struct rbd_device *rbd_dev = dev_to_rbd(dev);

	return sprintf(buf, "client%lld\n", ceph_client_id(rbd_dev->client));
=======
	struct rbd_device *rbd_dev = dev_to_rbd_dev(dev);

	return sprintf(buf, "client%lld\n",
			ceph_client_id(rbd_dev->rbd_client->client));
>>>>>>> refs/remotes/origin/cm-10.0
}

static ssize_t rbd_pool_show(struct device *dev,
			     struct device_attribute *attr, char *buf)
{
<<<<<<< HEAD
	struct rbd_device *rbd_dev = dev_to_rbd(dev);
=======
	struct rbd_device *rbd_dev = dev_to_rbd_dev(dev);
>>>>>>> refs/remotes/origin/cm-10.0

	return sprintf(buf, "%s\n", rbd_dev->pool_name);
}

static ssize_t rbd_name_show(struct device *dev,
			     struct device_attribute *attr, char *buf)
{
<<<<<<< HEAD
	struct rbd_device *rbd_dev = dev_to_rbd(dev);
=======
	struct rbd_device *rbd_dev = dev_to_rbd_dev(dev);
>>>>>>> refs/remotes/origin/cm-10.0

	return sprintf(buf, "%s\n", rbd_dev->obj);
}

static ssize_t rbd_snap_show(struct device *dev,
			     struct device_attribute *attr,
			     char *buf)
{
<<<<<<< HEAD
	struct rbd_device *rbd_dev = dev_to_rbd(dev);
=======
	struct rbd_device *rbd_dev = dev_to_rbd_dev(dev);
>>>>>>> refs/remotes/origin/cm-10.0

	return sprintf(buf, "%s\n", rbd_dev->snap_name);
}

static ssize_t rbd_image_refresh(struct device *dev,
				 struct device_attribute *attr,
				 const char *buf,
				 size_t size)
{
<<<<<<< HEAD
	struct rbd_device *rbd_dev = dev_to_rbd(dev);
=======
	struct rbd_device *rbd_dev = dev_to_rbd_dev(dev);
>>>>>>> refs/remotes/origin/cm-10.0
	int rc;
	int ret = size;

	mutex_lock_nested(&ctl_mutex, SINGLE_DEPTH_NESTING);

	rc = __rbd_update_snaps(rbd_dev);
	if (rc < 0)
		ret = rc;

	mutex_unlock(&ctl_mutex);
	return ret;
}

static DEVICE_ATTR(size, S_IRUGO, rbd_size_show, NULL);
static DEVICE_ATTR(major, S_IRUGO, rbd_major_show, NULL);
static DEVICE_ATTR(client_id, S_IRUGO, rbd_client_id_show, NULL);
static DEVICE_ATTR(pool, S_IRUGO, rbd_pool_show, NULL);
static DEVICE_ATTR(name, S_IRUGO, rbd_name_show, NULL);
static DEVICE_ATTR(refresh, S_IWUSR, NULL, rbd_image_refresh);
static DEVICE_ATTR(current_snap, S_IRUGO, rbd_snap_show, NULL);
<<<<<<< HEAD
static DEVICE_ATTR(create_snap, S_IWUSR, NULL, rbd_snap_add);
static DEVICE_ATTR(rollback_snap, S_IWUSR, NULL, rbd_snap_rollback);
=======
>>>>>>> refs/remotes/origin/cm-10.0

static struct attribute *rbd_attrs[] = {
	&dev_attr_size.attr,
	&dev_attr_major.attr,
	&dev_attr_client_id.attr,
	&dev_attr_pool.attr,
	&dev_attr_name.attr,
	&dev_attr_current_snap.attr,
	&dev_attr_refresh.attr,
<<<<<<< HEAD
	&dev_attr_create_snap.attr,
	&dev_attr_rollback_snap.attr,
=======
>>>>>>> refs/remotes/origin/cm-10.0
	NULL
};

static struct attribute_group rbd_attr_group = {
	.attrs = rbd_attrs,
};

static const struct attribute_group *rbd_attr_groups[] = {
	&rbd_attr_group,
	NULL
};

static void rbd_sysfs_dev_release(struct device *dev)
{
}

static struct device_type rbd_device_type = {
	.name		= "rbd",
	.groups		= rbd_attr_groups,
	.release	= rbd_sysfs_dev_release,
};


/*
  sysfs - snapshots
*/

static ssize_t rbd_snap_size_show(struct device *dev,
				  struct device_attribute *attr,
				  char *buf)
{
	struct rbd_snap *snap = container_of(dev, struct rbd_snap, dev);

<<<<<<< HEAD
	return sprintf(buf, "%lld\n", (long long)snap->size);
=======
	return sprintf(buf, "%zd\n", snap->size);
>>>>>>> refs/remotes/origin/cm-10.0
}

static ssize_t rbd_snap_id_show(struct device *dev,
				struct device_attribute *attr,
				char *buf)
{
	struct rbd_snap *snap = container_of(dev, struct rbd_snap, dev);

<<<<<<< HEAD
	return sprintf(buf, "%lld\n", (long long)snap->id);
=======
	return sprintf(buf, "%llu\n", (unsigned long long) snap->id);
>>>>>>> refs/remotes/origin/cm-10.0
}

static DEVICE_ATTR(snap_size, S_IRUGO, rbd_snap_size_show, NULL);
static DEVICE_ATTR(snap_id, S_IRUGO, rbd_snap_id_show, NULL);

static struct attribute *rbd_snap_attrs[] = {
	&dev_attr_snap_size.attr,
	&dev_attr_snap_id.attr,
	NULL,
};

static struct attribute_group rbd_snap_attr_group = {
	.attrs = rbd_snap_attrs,
};

static void rbd_snap_dev_release(struct device *dev)
{
	struct rbd_snap *snap = container_of(dev, struct rbd_snap, dev);
	kfree(snap->name);
	kfree(snap);
}

static const struct attribute_group *rbd_snap_attr_groups[] = {
	&rbd_snap_attr_group,
	NULL
};

static struct device_type rbd_snap_device_type = {
	.groups		= rbd_snap_attr_groups,
	.release	= rbd_snap_dev_release,
};

static void __rbd_remove_snap_dev(struct rbd_device *rbd_dev,
				  struct rbd_snap *snap)
{
	list_del(&snap->node);
	device_unregister(&snap->dev);
}

static int rbd_register_snap_dev(struct rbd_device *rbd_dev,
				  struct rbd_snap *snap,
				  struct device *parent)
{
	struct device *dev = &snap->dev;
	int ret;

	dev->type = &rbd_snap_device_type;
	dev->parent = parent;
	dev->release = rbd_snap_dev_release;
	dev_set_name(dev, "snap_%s", snap->name);
	ret = device_register(dev);

	return ret;
}

static int __rbd_add_snap_dev(struct rbd_device *rbd_dev,
			      int i, const char *name,
			      struct rbd_snap **snapp)
{
	int ret;
	struct rbd_snap *snap = kzalloc(sizeof(*snap), GFP_KERNEL);
	if (!snap)
		return -ENOMEM;
	snap->name = kstrdup(name, GFP_KERNEL);
	snap->size = rbd_dev->header.snap_sizes[i];
	snap->id = rbd_dev->header.snapc->snaps[i];
	if (device_is_registered(&rbd_dev->dev)) {
		ret = rbd_register_snap_dev(rbd_dev, snap,
					     &rbd_dev->dev);
		if (ret < 0)
			goto err;
	}
	*snapp = snap;
	return 0;
err:
	kfree(snap->name);
	kfree(snap);
	return ret;
}

/*
 * search for the previous snap in a null delimited string list
 */
const char *rbd_prev_snap_name(const char *name, const char *start)
{
	if (name < start + 2)
		return NULL;

	name -= 2;
	while (*name) {
		if (name == start)
			return start;
		name--;
	}
	return name + 1;
}

/*
 * compare the old list of snapshots that we have to what's in the header
 * and update it accordingly. Note that the header holds the snapshots
 * in a reverse order (from newest to oldest) and we need to go from
 * older to new so that we don't get a duplicate snap name when
 * doing the process (e.g., removed snapshot and recreated a new
 * one with the same name.
 */
static int __rbd_init_snaps_header(struct rbd_device *rbd_dev)
{
	const char *name, *first_name;
	int i = rbd_dev->header.total_snaps;
	struct rbd_snap *snap, *old_snap = NULL;
	int ret;
	struct list_head *p, *n;

	first_name = rbd_dev->header.snap_names;
	name = first_name + rbd_dev->header.snap_names_len;

	list_for_each_prev_safe(p, n, &rbd_dev->snaps) {
		u64 cur_id;

		old_snap = list_entry(p, struct rbd_snap, node);

		if (i)
			cur_id = rbd_dev->header.snapc->snaps[i - 1];

		if (!i || old_snap->id < cur_id) {
<<<<<<< HEAD
			/* old_snap->id was skipped, thus was removed */
=======
			/*
			 * old_snap->id was skipped, thus was
			 * removed.  If this rbd_dev is mapped to
			 * the removed snapshot, record that it no
			 * longer exists, to prevent further I/O.
			 */
			if (rbd_dev->snap_id == old_snap->id)
				rbd_dev->snap_exists = false;
>>>>>>> refs/remotes/origin/cm-10.0
			__rbd_remove_snap_dev(rbd_dev, old_snap);
			continue;
		}
		if (old_snap->id == cur_id) {
			/* we have this snapshot already */
			i--;
			name = rbd_prev_snap_name(name, first_name);
			continue;
		}
		for (; i > 0;
		     i--, name = rbd_prev_snap_name(name, first_name)) {
			if (!name) {
				WARN_ON(1);
				return -EINVAL;
			}
			cur_id = rbd_dev->header.snapc->snaps[i];
			/* snapshot removal? handle it above */
			if (cur_id >= old_snap->id)
				break;
			/* a new snapshot */
			ret = __rbd_add_snap_dev(rbd_dev, i - 1, name, &snap);
			if (ret < 0)
				return ret;

			/* note that we add it backward so using n and not p */
			list_add(&snap->node, n);
			p = &snap->node;
		}
	}
	/* we're done going over the old snap list, just add what's left */
	for (; i > 0; i--) {
		name = rbd_prev_snap_name(name, first_name);
		if (!name) {
			WARN_ON(1);
			return -EINVAL;
		}
		ret = __rbd_add_snap_dev(rbd_dev, i - 1, name, &snap);
		if (ret < 0)
			return ret;
		list_add(&snap->node, &rbd_dev->snaps);
	}

	return 0;
}

<<<<<<< HEAD

static void rbd_root_dev_release(struct device *dev)
{
}

static struct device rbd_root_dev = {
	.init_name =    "rbd",
	.release =      rbd_root_dev_release,
};

static int rbd_bus_add_dev(struct rbd_device *rbd_dev)
{
	int ret = -ENOMEM;
=======
static int rbd_bus_add_dev(struct rbd_device *rbd_dev)
{
	int ret;
>>>>>>> refs/remotes/origin/cm-10.0
	struct device *dev;
	struct rbd_snap *snap;

	mutex_lock_nested(&ctl_mutex, SINGLE_DEPTH_NESTING);
	dev = &rbd_dev->dev;

	dev->bus = &rbd_bus_type;
	dev->type = &rbd_device_type;
	dev->parent = &rbd_root_dev;
	dev->release = rbd_dev_release;
	dev_set_name(dev, "%d", rbd_dev->id);
	ret = device_register(dev);
	if (ret < 0)
<<<<<<< HEAD
		goto done_free;
=======
		goto out;
>>>>>>> refs/remotes/origin/cm-10.0

	list_for_each_entry(snap, &rbd_dev->snaps, node) {
		ret = rbd_register_snap_dev(rbd_dev, snap,
					     &rbd_dev->dev);
		if (ret < 0)
			break;
	}
<<<<<<< HEAD

	mutex_unlock(&ctl_mutex);
	return 0;
done_free:
=======
out:
>>>>>>> refs/remotes/origin/cm-10.0
	mutex_unlock(&ctl_mutex);
	return ret;
}

static void rbd_bus_del_dev(struct rbd_device *rbd_dev)
{
	device_unregister(&rbd_dev->dev);
}

static int rbd_init_watch_dev(struct rbd_device *rbd_dev)
{
	int ret, rc;

	do {
		ret = rbd_req_sync_watch(rbd_dev, rbd_dev->obj_md_name,
					 rbd_dev->header.obj_version);
		if (ret == -ERANGE) {
			mutex_lock_nested(&ctl_mutex, SINGLE_DEPTH_NESTING);
			rc = __rbd_update_snaps(rbd_dev);
			mutex_unlock(&ctl_mutex);
			if (rc < 0)
				return rc;
		}
	} while (ret == -ERANGE);

	return ret;
}

<<<<<<< HEAD
=======
static atomic64_t rbd_id_max = ATOMIC64_INIT(0);

/*
 * Get a unique rbd identifier for the given new rbd_dev, and add
 * the rbd_dev to the global list.  The minimum rbd id is 1.
 */
static void rbd_id_get(struct rbd_device *rbd_dev)
{
	rbd_dev->id = atomic64_inc_return(&rbd_id_max);

	spin_lock(&rbd_dev_list_lock);
	list_add_tail(&rbd_dev->node, &rbd_dev_list);
	spin_unlock(&rbd_dev_list_lock);
}

/*
 * Remove an rbd_dev from the global list, and record that its
 * identifier is no longer in use.
 */
static void rbd_id_put(struct rbd_device *rbd_dev)
{
	struct list_head *tmp;
	int rbd_id = rbd_dev->id;
	int max_id;

	BUG_ON(rbd_id < 1);

	spin_lock(&rbd_dev_list_lock);
	list_del_init(&rbd_dev->node);

	/*
	 * If the id being "put" is not the current maximum, there
	 * is nothing special we need to do.
	 */
	if (rbd_id != atomic64_read(&rbd_id_max)) {
		spin_unlock(&rbd_dev_list_lock);
		return;
	}

	/*
	 * We need to update the current maximum id.  Search the
	 * list to find out what it is.  We're more likely to find
	 * the maximum at the end, so search the list backward.
	 */
	max_id = 0;
	list_for_each_prev(tmp, &rbd_dev_list) {
		struct rbd_device *rbd_dev;

		rbd_dev = list_entry(tmp, struct rbd_device, node);
		if (rbd_dev->id > max_id)
			max_id = rbd_dev->id;
	}
	spin_unlock(&rbd_dev_list_lock);

	/*
	 * The max id could have been updated by rbd_id_get(), in
	 * which case it now accurately reflects the new maximum.
	 * Be careful not to overwrite the maximum value in that
	 * case.
	 */
	atomic64_cmpxchg(&rbd_id_max, rbd_id, max_id);
}

/*
 * Skips over white space at *buf, and updates *buf to point to the
 * first found non-space character (if any). Returns the length of
 * the token (string of non-white space characters) found.  Note
 * that *buf must be terminated with '\0'.
 */
static inline size_t next_token(const char **buf)
{
        /*
        * These are the characters that produce nonzero for
        * isspace() in the "C" and "POSIX" locales.
        */
        const char *spaces = " \f\n\r\t\v";

        *buf += strspn(*buf, spaces);	/* Find start of token */

	return strcspn(*buf, spaces);   /* Return token length */
}

/*
 * Finds the next token in *buf, and if the provided token buffer is
 * big enough, copies the found token into it.  The result, if
 * copied, is guaranteed to be terminated with '\0'.  Note that *buf
 * must be terminated with '\0' on entry.
 *
 * Returns the length of the token found (not including the '\0').
 * Return value will be 0 if no token is found, and it will be >=
 * token_size if the token would not fit.
 *
 * The *buf pointer will be updated to point beyond the end of the
 * found token.  Note that this occurs even if the token buffer is
 * too small to hold it.
 */
static inline size_t copy_token(const char **buf,
				char *token,
				size_t token_size)
{
        size_t len;

	len = next_token(buf);
	if (len < token_size) {
		memcpy(token, *buf, len);
		*(token + len) = '\0';
	}
	*buf += len;

        return len;
}

/*
 * This fills in the pool_name, obj, obj_len, snap_name, obj_len,
 * rbd_dev, rbd_md_name, and name fields of the given rbd_dev, based
 * on the list of monitor addresses and other options provided via
 * /sys/bus/rbd/add.
 */
static int rbd_add_parse_args(struct rbd_device *rbd_dev,
			      const char *buf,
			      const char **mon_addrs,
			      size_t *mon_addrs_size,
			      char *options,
			      size_t options_size)
{
	size_t	len;

	/* The first four tokens are required */

	len = next_token(&buf);
	if (!len)
		return -EINVAL;
	*mon_addrs_size = len + 1;
	*mon_addrs = buf;

	buf += len;

	len = copy_token(&buf, options, options_size);
	if (!len || len >= options_size)
		return -EINVAL;

	len = copy_token(&buf, rbd_dev->pool_name, sizeof (rbd_dev->pool_name));
	if (!len || len >= sizeof (rbd_dev->pool_name))
		return -EINVAL;

	len = copy_token(&buf, rbd_dev->obj, sizeof (rbd_dev->obj));
	if (!len || len >= sizeof (rbd_dev->obj))
		return -EINVAL;

	/* We have the object length in hand, save it. */

	rbd_dev->obj_len = len;

	BUILD_BUG_ON(RBD_MAX_MD_NAME_LEN
				< RBD_MAX_OBJ_NAME_LEN + sizeof (RBD_SUFFIX));
	sprintf(rbd_dev->obj_md_name, "%s%s", rbd_dev->obj, RBD_SUFFIX);

	/*
	 * The snapshot name is optional, but it's an error if it's
	 * too long.  If no snapshot is supplied, fill in the default.
	 */
	len = copy_token(&buf, rbd_dev->snap_name, sizeof (rbd_dev->snap_name));
	if (!len)
		memcpy(rbd_dev->snap_name, RBD_SNAP_HEAD_NAME,
			sizeof (RBD_SNAP_HEAD_NAME));
	else if (len >= sizeof (rbd_dev->snap_name))
		return -EINVAL;

	return 0;
}

>>>>>>> refs/remotes/origin/cm-10.0
static ssize_t rbd_add(struct bus_type *bus,
		       const char *buf,
		       size_t count)
{
<<<<<<< HEAD
	struct ceph_osd_client *osdc;
	struct rbd_device *rbd_dev;
	ssize_t rc = -ENOMEM;
	int irc, new_id = 0;
	struct list_head *tmp;
	char *mon_dev_name;
	char *options;
=======
	struct rbd_device *rbd_dev;
	const char *mon_addrs = NULL;
	size_t mon_addrs_size = 0;
	char *options = NULL;
	struct ceph_osd_client *osdc;
	int rc = -ENOMEM;
>>>>>>> refs/remotes/origin/cm-10.0

	if (!try_module_get(THIS_MODULE))
		return -ENODEV;

<<<<<<< HEAD
	mon_dev_name = kmalloc(RBD_MAX_OPT_LEN, GFP_KERNEL);
	if (!mon_dev_name)
		goto err_out_mod;

	options = kmalloc(RBD_MAX_OPT_LEN, GFP_KERNEL);
	if (!options)
		goto err_mon_dev;

	/* new rbd_device object */
	rbd_dev = kzalloc(sizeof(*rbd_dev), GFP_KERNEL);
	if (!rbd_dev)
		goto err_out_opt;
=======
	rbd_dev = kzalloc(sizeof(*rbd_dev), GFP_KERNEL);
	if (!rbd_dev)
		goto err_nomem;
	options = kmalloc(count, GFP_KERNEL);
	if (!options)
		goto err_nomem;
>>>>>>> refs/remotes/origin/cm-10.0

	/* static rbd_device initialization */
	spin_lock_init(&rbd_dev->lock);
	INIT_LIST_HEAD(&rbd_dev->node);
	INIT_LIST_HEAD(&rbd_dev->snaps);
<<<<<<< HEAD

	/* generate unique id: find highest unique id, add one */
	mutex_lock_nested(&ctl_mutex, SINGLE_DEPTH_NESTING);

	list_for_each(tmp, &rbd_dev_list) {
		struct rbd_device *rbd_dev;

		rbd_dev = list_entry(tmp, struct rbd_device, node);
		if (rbd_dev->id >= new_id)
			new_id = rbd_dev->id + 1;
	}

	rbd_dev->id = new_id;

	/* add to global list */
	list_add_tail(&rbd_dev->node, &rbd_dev_list);

	/* parse add command */
	if (sscanf(buf, "%" __stringify(RBD_MAX_OPT_LEN) "s "
		   "%" __stringify(RBD_MAX_OPT_LEN) "s "
		   "%" __stringify(RBD_MAX_POOL_NAME_LEN) "s "
		   "%" __stringify(RBD_MAX_OBJ_NAME_LEN) "s"
		   "%" __stringify(RBD_MAX_SNAP_NAME_LEN) "s",
		   mon_dev_name, options, rbd_dev->pool_name,
		   rbd_dev->obj, rbd_dev->snap_name) < 4) {
		rc = -EINVAL;
		goto err_out_slot;
	}

	if (rbd_dev->snap_name[0] == 0)
		rbd_dev->snap_name[0] = '-';

	rbd_dev->obj_len = strlen(rbd_dev->obj);
	snprintf(rbd_dev->obj_md_name, sizeof(rbd_dev->obj_md_name), "%s%s",
		 rbd_dev->obj, RBD_SUFFIX);

	/* initialize rest of new object */
	snprintf(rbd_dev->name, DEV_NAME_LEN, DRV_NAME "%d", rbd_dev->id);
	rc = rbd_get_client(rbd_dev, mon_dev_name, options);
	if (rc < 0)
		goto err_out_slot;

	mutex_unlock(&ctl_mutex);

	/* pick the pool */
	osdc = &rbd_dev->client->osdc;
=======
	init_rwsem(&rbd_dev->header_rwsem);

	init_rwsem(&rbd_dev->header_rwsem);

	/* generate unique id: find highest unique id, add one */
	rbd_id_get(rbd_dev);

	/* Fill in the device name, now that we have its id. */
	BUILD_BUG_ON(DEV_NAME_LEN
			< sizeof (RBD_DRV_NAME) + MAX_INT_FORMAT_WIDTH);
	sprintf(rbd_dev->name, "%s%d", RBD_DRV_NAME, rbd_dev->id);

	/* parse add command */
	rc = rbd_add_parse_args(rbd_dev, buf, &mon_addrs, &mon_addrs_size,
				options, count);
	if (rc)
		goto err_put_id;

	rbd_dev->rbd_client = rbd_get_client(mon_addrs, mon_addrs_size - 1,
						options);
	if (IS_ERR(rbd_dev->rbd_client)) {
		rc = PTR_ERR(rbd_dev->rbd_client);
		goto err_put_id;
	}

	/* pick the pool */
	osdc = &rbd_dev->rbd_client->client->osdc;
>>>>>>> refs/remotes/origin/cm-10.0
	rc = ceph_pg_poolid_by_name(osdc->osdmap, rbd_dev->pool_name);
	if (rc < 0)
		goto err_out_client;
	rbd_dev->poolid = rc;

	/* register our block device */
<<<<<<< HEAD
	irc = register_blkdev(0, rbd_dev->name);
	if (irc < 0) {
		rc = irc;
		goto err_out_client;
	}
	rbd_dev->major = irc;
=======
	rc = register_blkdev(0, rbd_dev->name);
	if (rc < 0)
		goto err_out_client;
	rbd_dev->major = rc;
>>>>>>> refs/remotes/origin/cm-10.0

	rc = rbd_bus_add_dev(rbd_dev);
	if (rc)
		goto err_out_blkdev;

<<<<<<< HEAD
	/* set up and announce blkdev mapping */
=======
	/*
	 * At this point cleanup in the event of an error is the job
	 * of the sysfs code (initiated by rbd_bus_del_dev()).
	 *
	 * Set up and announce blkdev mapping.
	 */
>>>>>>> refs/remotes/origin/cm-10.0
	rc = rbd_init_disk(rbd_dev);
	if (rc)
		goto err_out_bus;

	rc = rbd_init_watch_dev(rbd_dev);
	if (rc)
		goto err_out_bus;

	return count;

err_out_bus:
<<<<<<< HEAD
	mutex_lock_nested(&ctl_mutex, SINGLE_DEPTH_NESTING);
	list_del_init(&rbd_dev->node);
	mutex_unlock(&ctl_mutex);

=======
>>>>>>> refs/remotes/origin/cm-10.0
	/* this will also clean up rest of rbd_dev stuff */

	rbd_bus_del_dev(rbd_dev);
	kfree(options);
<<<<<<< HEAD
	kfree(mon_dev_name);
=======
>>>>>>> refs/remotes/origin/cm-10.0
	return rc;

err_out_blkdev:
	unregister_blkdev(rbd_dev->major, rbd_dev->name);
err_out_client:
	rbd_put_client(rbd_dev);
<<<<<<< HEAD
	mutex_lock_nested(&ctl_mutex, SINGLE_DEPTH_NESTING);
err_out_slot:
	list_del_init(&rbd_dev->node);
	mutex_unlock(&ctl_mutex);

	kfree(rbd_dev);
err_out_opt:
	kfree(options);
err_mon_dev:
	kfree(mon_dev_name);
err_out_mod:
	dout("Error adding device %s\n", buf);
	module_put(THIS_MODULE);
	return rc;
=======
err_put_id:
	rbd_id_put(rbd_dev);
err_nomem:
	kfree(options);
	kfree(rbd_dev);

	dout("Error adding device %s\n", buf);
	module_put(THIS_MODULE);

	return (ssize_t) rc;
>>>>>>> refs/remotes/origin/cm-10.0
}

static struct rbd_device *__rbd_get_dev(unsigned long id)
{
	struct list_head *tmp;
	struct rbd_device *rbd_dev;

<<<<<<< HEAD
	list_for_each(tmp, &rbd_dev_list) {
		rbd_dev = list_entry(tmp, struct rbd_device, node);
		if (rbd_dev->id == id)
			return rbd_dev;
	}
=======
	spin_lock(&rbd_dev_list_lock);
	list_for_each(tmp, &rbd_dev_list) {
		rbd_dev = list_entry(tmp, struct rbd_device, node);
		if (rbd_dev->id == id) {
			spin_unlock(&rbd_dev_list_lock);
			return rbd_dev;
		}
	}
	spin_unlock(&rbd_dev_list_lock);
>>>>>>> refs/remotes/origin/cm-10.0
	return NULL;
}

static void rbd_dev_release(struct device *dev)
{
<<<<<<< HEAD
	struct rbd_device *rbd_dev =
			container_of(dev, struct rbd_device, dev);

	if (rbd_dev->watch_request)
		ceph_osdc_unregister_linger_request(&rbd_dev->client->osdc,
						    rbd_dev->watch_request);
	if (rbd_dev->watch_event)
		ceph_osdc_cancel_event(rbd_dev->watch_event);
=======
	struct rbd_device *rbd_dev = dev_to_rbd_dev(dev);

	if (rbd_dev->watch_request) {
		struct ceph_client *client = rbd_dev->rbd_client->client;

		ceph_osdc_unregister_linger_request(&client->osdc,
						    rbd_dev->watch_request);
	}
	if (rbd_dev->watch_event)
		rbd_req_sync_unwatch(rbd_dev, rbd_dev->obj_md_name);
>>>>>>> refs/remotes/origin/cm-10.0

	rbd_put_client(rbd_dev);

	/* clean up and free blkdev */
	rbd_free_disk(rbd_dev);
	unregister_blkdev(rbd_dev->major, rbd_dev->name);
<<<<<<< HEAD
=======

	/* done with the id, and with the rbd_dev */
	rbd_id_put(rbd_dev);
>>>>>>> refs/remotes/origin/cm-10.0
	kfree(rbd_dev);

	/* release module ref */
	module_put(THIS_MODULE);
}

static ssize_t rbd_remove(struct bus_type *bus,
			  const char *buf,
			  size_t count)
{
	struct rbd_device *rbd_dev = NULL;
	int target_id, rc;
	unsigned long ul;
	int ret = count;

	rc = strict_strtoul(buf, 10, &ul);
	if (rc)
		return rc;

	/* convert to int; abort if we lost anything in the conversion */
	target_id = (int) ul;
	if (target_id != ul)
		return -EINVAL;

	mutex_lock_nested(&ctl_mutex, SINGLE_DEPTH_NESTING);

	rbd_dev = __rbd_get_dev(target_id);
	if (!rbd_dev) {
		ret = -ENOENT;
		goto done;
	}

<<<<<<< HEAD
	list_del_init(&rbd_dev->node);
=======
	if (rbd_dev->open_count) {
		ret = -EBUSY;
		goto done;
	}
>>>>>>> refs/remotes/origin/cm-10.0

	__rbd_remove_all_snaps(rbd_dev);
	rbd_bus_del_dev(rbd_dev);

done:
	mutex_unlock(&ctl_mutex);
	return ret;
}

<<<<<<< HEAD
static ssize_t rbd_snap_add(struct device *dev,
			    struct device_attribute *attr,
			    const char *buf,
			    size_t count)
{
	struct rbd_device *rbd_dev = dev_to_rbd(dev);
	int ret;
	char *name = kmalloc(count + 1, GFP_KERNEL);
	if (!name)
		return -ENOMEM;

	snprintf(name, count, "%s", buf);

	mutex_lock_nested(&ctl_mutex, SINGLE_DEPTH_NESTING);

	ret = rbd_header_add_snap(rbd_dev,
				  name, GFP_KERNEL);
	if (ret < 0)
		goto err_unlock;

	ret = __rbd_update_snaps(rbd_dev);
	if (ret < 0)
		goto err_unlock;

	/* shouldn't hold ctl_mutex when notifying.. notify might
	   trigger a watch callback that would need to get that mutex */
	mutex_unlock(&ctl_mutex);

	/* make a best effort, don't error if failed */
	rbd_req_sync_notify(rbd_dev, rbd_dev->obj_md_name);

	ret = count;
	kfree(name);
	return ret;

err_unlock:
	mutex_unlock(&ctl_mutex);
	kfree(name);
	return ret;
}

static ssize_t rbd_snap_rollback(struct device *dev,
				 struct device_attribute *attr,
				 const char *buf,
				 size_t count)
{
	struct rbd_device *rbd_dev = dev_to_rbd(dev);
	int ret;
	u64 snapid;
	u64 cur_ofs;
	char *seg_name = NULL;
	char *snap_name = kmalloc(count + 1, GFP_KERNEL);
	ret = -ENOMEM;
	if (!snap_name)
		return ret;

	/* parse snaps add command */
	snprintf(snap_name, count, "%s", buf);
	seg_name = kmalloc(RBD_MAX_SEG_NAME_LEN + 1, GFP_NOIO);
	if (!seg_name)
		goto done;

	mutex_lock_nested(&ctl_mutex, SINGLE_DEPTH_NESTING);

	ret = snap_by_name(&rbd_dev->header, snap_name, &snapid, NULL);
	if (ret < 0)
		goto done_unlock;

	dout("snapid=%lld\n", snapid);

	cur_ofs = 0;
	while (cur_ofs < rbd_dev->header.image_size) {
		cur_ofs += rbd_get_segment(&rbd_dev->header,
					   rbd_dev->obj,
					   cur_ofs, (u64)-1,
					   seg_name, NULL);
		dout("seg_name=%s\n", seg_name);

		ret = rbd_req_sync_rollback_obj(rbd_dev, snapid, seg_name);
		if (ret < 0)
			pr_warning("could not roll back obj %s err=%d\n",
				   seg_name, ret);
	}

	ret = __rbd_update_snaps(rbd_dev);
	if (ret < 0)
		goto done_unlock;

	ret = count;

done_unlock:
	mutex_unlock(&ctl_mutex);
done:
	kfree(seg_name);
	kfree(snap_name);

	return ret;
}

static struct bus_attribute rbd_bus_attrs[] = {
	__ATTR(add, S_IWUSR, NULL, rbd_add),
	__ATTR(remove, S_IWUSR, NULL, rbd_remove),
	__ATTR_NULL
};

=======
>>>>>>> refs/remotes/origin/cm-10.0
/*
 * create control files in sysfs
 * /sys/bus/rbd/...
 */
static int rbd_sysfs_init(void)
{
	int ret;

<<<<<<< HEAD
	rbd_bus_type.bus_attrs = rbd_bus_attrs;

	ret = bus_register(&rbd_bus_type);
	 if (ret < 0)
		return ret;

	ret = device_register(&rbd_root_dev);
=======
	ret = device_register(&rbd_root_dev);
	if (ret < 0)
		return ret;

	ret = bus_register(&rbd_bus_type);
	if (ret < 0)
		device_unregister(&rbd_root_dev);
>>>>>>> refs/remotes/origin/cm-10.0

	return ret;
}

static void rbd_sysfs_cleanup(void)
{
<<<<<<< HEAD
	device_unregister(&rbd_root_dev);
	bus_unregister(&rbd_bus_type);
=======
	bus_unregister(&rbd_bus_type);
	device_unregister(&rbd_root_dev);
>>>>>>> refs/remotes/origin/cm-10.0
}

int __init rbd_init(void)
{
	int rc;

	rc = rbd_sysfs_init();
	if (rc)
		return rc;
<<<<<<< HEAD
	spin_lock_init(&node_lock);
	pr_info("loaded " DRV_NAME_LONG "\n");
=======
	pr_info("loaded " RBD_DRV_NAME_LONG "\n");
>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
}

void __exit rbd_exit(void)
{
	rbd_sysfs_cleanup();
}

module_init(rbd_init);
module_exit(rbd_exit);

MODULE_AUTHOR("Sage Weil <sage@newdream.net>");
MODULE_AUTHOR("Yehuda Sadeh <yehuda@hq.newdream.net>");
MODULE_DESCRIPTION("rados block device");

/* following authorship retained from original osdblk.c */
MODULE_AUTHOR("Jeff Garzik <jeff@garzik.org>");

MODULE_LICENSE("GPL");
