/*
 * devtmpfs - kernel-maintained tmpfs-based /dev
 *
 * Copyright (C) 2009, Kay Sievers <kay.sievers@vrfy.org>
 *
 * During bootup, before any driver core device is registered,
 * devtmpfs, a tmpfs-based filesystem is created. Every driver-core
 * device which requests a device node, will add a node in this
 * filesystem.
 * By default, all devices are named after the the name of the
 * device, owned by root and have a default mode of 0600. Subsystems
 * can overwrite the default setting if needed.
 */

#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/mount.h>
#include <linux/device.h>
#include <linux/genhd.h>
#include <linux/namei.h>
#include <linux/fs.h>
#include <linux/shmem_fs.h>
#include <linux/ramfs.h>
<<<<<<< HEAD
#include <linux/cred.h>
#include <linux/sched.h>
#include <linux/init_task.h>
#include <linux/slab.h>

static struct vfsmount *dev_mnt;
=======
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/kthread.h>

static struct task_struct *thread;
>>>>>>> refs/remotes/origin/cm-10.0

#if defined CONFIG_DEVTMPFS_MOUNT
static int mount_dev = 1;
#else
static int mount_dev;
#endif

<<<<<<< HEAD
static DEFINE_MUTEX(dirlock);
=======
static DEFINE_SPINLOCK(req_lock);

static struct req {
	struct req *next;
	struct completion done;
	int err;
	const char *name;
	umode_t mode;	/* 0 => delete */
	struct device *dev;
} *requests;
>>>>>>> refs/remotes/origin/cm-10.0

static int __init mount_param(char *str)
{
	mount_dev = simple_strtoul(str, NULL, 0);
	return 1;
}
__setup("devtmpfs.mount=", mount_param);

static struct dentry *dev_mount(struct file_system_type *fs_type, int flags,
		      const char *dev_name, void *data)
{
#ifdef CONFIG_TMPFS
	return mount_single(fs_type, flags, data, shmem_fill_super);
#else
	return mount_single(fs_type, flags, data, ramfs_fill_super);
#endif
}

static struct file_system_type dev_fs_type = {
	.name = "devtmpfs",
	.mount = dev_mount,
	.kill_sb = kill_litter_super,
};

#ifdef CONFIG_BLOCK
static inline int is_blockdev(struct device *dev)
{
	return dev->class == &block_class;
}
#else
static inline int is_blockdev(struct device *dev) { return 0; }
#endif

<<<<<<< HEAD
static int dev_mkdir(const char *name, mode_t mode)
{
	struct nameidata nd;
	struct dentry *dentry;
	int err;

	err = vfs_path_lookup(dev_mnt->mnt_root, dev_mnt,
			      name, LOOKUP_PARENT, &nd);
	if (err)
		return err;

	dentry = lookup_create(&nd, 1);
	if (!IS_ERR(dentry)) {
		err = vfs_mkdir(nd.path.dentry->d_inode, dentry, mode);
		if (!err)
			/* mark as kernel-created inode */
			dentry->d_inode->i_private = &dev_mnt;
		dput(dentry);
	} else {
		err = PTR_ERR(dentry);
	}

	mutex_unlock(&nd.path.dentry->d_inode->i_mutex);
	path_put(&nd.path);
	return err;
}

static int create_path(const char *nodepath)
{
	int err;

	mutex_lock(&dirlock);
	err = dev_mkdir(nodepath, 0755);
	if (err == -ENOENT) {
		char *path;
		char *s;

		/* parent directories do not exist, create them */
		path = kstrdup(nodepath, GFP_KERNEL);
		if (!path) {
			err = -ENOMEM;
			goto out;
		}
		s = path;
		for (;;) {
			s = strchr(s, '/');
			if (!s)
				break;
			s[0] = '\0';
			err = dev_mkdir(path, 0755);
			if (err && err != -EEXIST)
				break;
			s[0] = '/';
			s++;
		}
		kfree(path);
	}
out:
	mutex_unlock(&dirlock);
	return err;
}

int devtmpfs_create_node(struct device *dev)
{
	const char *tmp = NULL;
	const char *nodename;
	const struct cred *curr_cred;
	mode_t mode = 0;
	struct nameidata nd;
	struct dentry *dentry;
	int err;

	if (!dev_mnt)
		return 0;

	nodename = device_get_devnode(dev, &mode, &tmp);
	if (!nodename)
		return -ENOMEM;

	if (mode == 0)
		mode = 0600;
	if (is_blockdev(dev))
		mode |= S_IFBLK;
	else
		mode |= S_IFCHR;

	curr_cred = override_creds(&init_cred);

	err = vfs_path_lookup(dev_mnt->mnt_root, dev_mnt,
			      nodename, LOOKUP_PARENT, &nd);
	if (err == -ENOENT) {
		create_path(nodename);
		err = vfs_path_lookup(dev_mnt->mnt_root, dev_mnt,
				      nodename, LOOKUP_PARENT, &nd);
	}
	if (err)
		goto out;

	dentry = lookup_create(&nd, 0);
	if (!IS_ERR(dentry)) {
		err = vfs_mknod(nd.path.dentry->d_inode,
				dentry, mode, dev->devt);
		if (!err) {
			struct iattr newattrs;

			/* fixup possibly umasked mode */
			newattrs.ia_mode = mode;
			newattrs.ia_valid = ATTR_MODE;
			mutex_lock(&dentry->d_inode->i_mutex);
			notify_change(dentry, &newattrs);
			mutex_unlock(&dentry->d_inode->i_mutex);

			/* mark as kernel-created inode */
			dentry->d_inode->i_private = &dev_mnt;
		}
		dput(dentry);
	} else {
		err = PTR_ERR(dentry);
	}

	mutex_unlock(&nd.path.dentry->d_inode->i_mutex);
	path_put(&nd.path);
out:
	kfree(tmp);
	revert_creds(curr_cred);
=======
int devtmpfs_create_node(struct device *dev)
{
	const char *tmp = NULL;
	struct req req;

	if (!thread)
		return 0;

	req.mode = 0;
	req.name = device_get_devnode(dev, &req.mode, &tmp);
	if (!req.name)
		return -ENOMEM;

	if (req.mode == 0)
		req.mode = 0600;
	if (is_blockdev(dev))
		req.mode |= S_IFBLK;
	else
		req.mode |= S_IFCHR;

	req.dev = dev;

	init_completion(&req.done);

	spin_lock(&req_lock);
	req.next = requests;
	requests = &req;
	spin_unlock(&req_lock);

	wake_up_process(thread);
	wait_for_completion(&req.done);

	kfree(tmp);

	return req.err;
}

int devtmpfs_delete_node(struct device *dev)
{
	const char *tmp = NULL;
	struct req req;

	if (!thread)
		return 0;

	req.name = device_get_devnode(dev, NULL, &tmp);
	if (!req.name)
		return -ENOMEM;

	req.mode = 0;
	req.dev = dev;

	init_completion(&req.done);

	spin_lock(&req_lock);
	req.next = requests;
	requests = &req;
	spin_unlock(&req_lock);

	wake_up_process(thread);
	wait_for_completion(&req.done);

	kfree(tmp);
	return req.err;
}

static int dev_mkdir(const char *name, umode_t mode)
{
	struct dentry *dentry;
	struct path path;
	int err;

	dentry = kern_path_create(AT_FDCWD, name, &path, 1);
	if (IS_ERR(dentry))
		return PTR_ERR(dentry);

	err = vfs_mkdir(path.dentry->d_inode, dentry, mode);
	if (!err)
		/* mark as kernel-created inode */
		dentry->d_inode->i_private = &thread;
	dput(dentry);
	mutex_unlock(&path.dentry->d_inode->i_mutex);
	path_put(&path);
	return err;
}

static int create_path(const char *nodepath)
{
	char *path;
	char *s;
	int err = 0;

	/* parent directories do not exist, create them */
	path = kstrdup(nodepath, GFP_KERNEL);
	if (!path)
		return -ENOMEM;

	s = path;
	for (;;) {
		s = strchr(s, '/');
		if (!s)
			break;
		s[0] = '\0';
		err = dev_mkdir(path, 0755);
		if (err && err != -EEXIST)
			break;
		s[0] = '/';
		s++;
	}
	kfree(path);
	return err;
}

static int handle_create(const char *nodename, umode_t mode, struct device *dev)
{
	struct dentry *dentry;
	struct path path;
	int err;

	dentry = kern_path_create(AT_FDCWD, nodename, &path, 0);
	if (dentry == ERR_PTR(-ENOENT)) {
		create_path(nodename);
		dentry = kern_path_create(AT_FDCWD, nodename, &path, 0);
	}
	if (IS_ERR(dentry))
		return PTR_ERR(dentry);

	err = vfs_mknod(path.dentry->d_inode,
			dentry, mode, dev->devt);
	if (!err) {
		struct iattr newattrs;

		/* fixup possibly umasked mode */
		newattrs.ia_mode = mode;
		newattrs.ia_valid = ATTR_MODE;
		mutex_lock(&dentry->d_inode->i_mutex);
		notify_change(dentry, &newattrs);
		mutex_unlock(&dentry->d_inode->i_mutex);

		/* mark as kernel-created inode */
		dentry->d_inode->i_private = &thread;
	}
	dput(dentry);

	mutex_unlock(&path.dentry->d_inode->i_mutex);
	path_put(&path);
>>>>>>> refs/remotes/origin/cm-10.0
	return err;
}

static int dev_rmdir(const char *name)
{
	struct nameidata nd;
	struct dentry *dentry;
	int err;

<<<<<<< HEAD
	err = vfs_path_lookup(dev_mnt->mnt_root, dev_mnt,
			      name, LOOKUP_PARENT, &nd);
=======
	err = kern_path_parent(name, &nd);
>>>>>>> refs/remotes/origin/cm-10.0
	if (err)
		return err;

	mutex_lock_nested(&nd.path.dentry->d_inode->i_mutex, I_MUTEX_PARENT);
	dentry = lookup_one_len(nd.last.name, nd.path.dentry, nd.last.len);
	if (!IS_ERR(dentry)) {
		if (dentry->d_inode) {
<<<<<<< HEAD
			if (dentry->d_inode->i_private == &dev_mnt)
=======
			if (dentry->d_inode->i_private == &thread)
>>>>>>> refs/remotes/origin/cm-10.0
				err = vfs_rmdir(nd.path.dentry->d_inode,
						dentry);
			else
				err = -EPERM;
		} else {
			err = -ENOENT;
		}
		dput(dentry);
	} else {
		err = PTR_ERR(dentry);
	}

	mutex_unlock(&nd.path.dentry->d_inode->i_mutex);
	path_put(&nd.path);
	return err;
}

static int delete_path(const char *nodepath)
{
	const char *path;
	int err = 0;

	path = kstrdup(nodepath, GFP_KERNEL);
	if (!path)
		return -ENOMEM;

<<<<<<< HEAD
	mutex_lock(&dirlock);
=======
>>>>>>> refs/remotes/origin/cm-10.0
	for (;;) {
		char *base;

		base = strrchr(path, '/');
		if (!base)
			break;
		base[0] = '\0';
		err = dev_rmdir(path);
		if (err)
			break;
	}
<<<<<<< HEAD
	mutex_unlock(&dirlock);
=======
>>>>>>> refs/remotes/origin/cm-10.0

	kfree(path);
	return err;
}

static int dev_mynode(struct device *dev, struct inode *inode, struct kstat *stat)
{
	/* did we create it */
<<<<<<< HEAD
	if (inode->i_private != &dev_mnt)
=======
	if (inode->i_private != &thread)
>>>>>>> refs/remotes/origin/cm-10.0
		return 0;

	/* does the dev_t match */
	if (is_blockdev(dev)) {
		if (!S_ISBLK(stat->mode))
			return 0;
	} else {
		if (!S_ISCHR(stat->mode))
			return 0;
	}
	if (stat->rdev != dev->devt)
		return 0;

	/* ours */
	return 1;
}

<<<<<<< HEAD
int devtmpfs_delete_node(struct device *dev)
{
	const char *tmp = NULL;
	const char *nodename;
	const struct cred *curr_cred;
=======
static int handle_remove(const char *nodename, struct device *dev)
{
>>>>>>> refs/remotes/origin/cm-10.0
	struct nameidata nd;
	struct dentry *dentry;
	struct kstat stat;
	int deleted = 1;
	int err;

<<<<<<< HEAD
	if (!dev_mnt)
		return 0;

	nodename = device_get_devnode(dev, NULL, &tmp);
	if (!nodename)
		return -ENOMEM;

	curr_cred = override_creds(&init_cred);
	err = vfs_path_lookup(dev_mnt->mnt_root, dev_mnt,
			      nodename, LOOKUP_PARENT, &nd);
	if (err)
		goto out;
=======
	err = kern_path_parent(nodename, &nd);
	if (err)
		return err;
>>>>>>> refs/remotes/origin/cm-10.0

	mutex_lock_nested(&nd.path.dentry->d_inode->i_mutex, I_MUTEX_PARENT);
	dentry = lookup_one_len(nd.last.name, nd.path.dentry, nd.last.len);
	if (!IS_ERR(dentry)) {
		if (dentry->d_inode) {
			err = vfs_getattr(nd.path.mnt, dentry, &stat);
			if (!err && dev_mynode(dev, dentry->d_inode, &stat)) {
				struct iattr newattrs;
				/*
				 * before unlinking this node, reset permissions
				 * of possible references like hardlinks
				 */
				newattrs.ia_uid = 0;
				newattrs.ia_gid = 0;
				newattrs.ia_mode = stat.mode & ~0777;
				newattrs.ia_valid =
					ATTR_UID|ATTR_GID|ATTR_MODE;
				mutex_lock(&dentry->d_inode->i_mutex);
				notify_change(dentry, &newattrs);
				mutex_unlock(&dentry->d_inode->i_mutex);
				err = vfs_unlink(nd.path.dentry->d_inode,
						 dentry);
				if (!err || err == -ENOENT)
					deleted = 1;
			}
		} else {
			err = -ENOENT;
		}
		dput(dentry);
	} else {
		err = PTR_ERR(dentry);
	}
	mutex_unlock(&nd.path.dentry->d_inode->i_mutex);

	path_put(&nd.path);
	if (deleted && strchr(nodename, '/'))
		delete_path(nodename);
<<<<<<< HEAD
out:
	kfree(tmp);
	revert_creds(curr_cred);
=======
>>>>>>> refs/remotes/origin/cm-10.0
	return err;
}

/*
 * If configured, or requested by the commandline, devtmpfs will be
 * auto-mounted after the kernel mounted the root filesystem.
 */
int devtmpfs_mount(const char *mntdir)
{
	int err;

	if (!mount_dev)
		return 0;

<<<<<<< HEAD
	if (!dev_mnt)
=======
	if (!thread)
>>>>>>> refs/remotes/origin/cm-10.0
		return 0;

	err = sys_mount("devtmpfs", (char *)mntdir, "devtmpfs", MS_SILENT, NULL);
	if (err)
		printk(KERN_INFO "devtmpfs: error mounting %i\n", err);
	else
		printk(KERN_INFO "devtmpfs: mounted\n");
	return err;
}

<<<<<<< HEAD
=======
static DECLARE_COMPLETION(setup_done);

static int handle(const char *name, umode_t mode, struct device *dev)
{
	if (mode)
		return handle_create(name, mode, dev);
	else
		return handle_remove(name, dev);
}

static int devtmpfsd(void *p)
{
	char options[] = "mode=0755";
	int *err = p;
	*err = sys_unshare(CLONE_NEWNS);
	if (*err)
		goto out;
	*err = sys_mount("devtmpfs", "/", "devtmpfs", MS_SILENT, options);
	if (*err)
		goto out;
	sys_chdir("/.."); /* will traverse into overmounted root */
	sys_chroot(".");
	complete(&setup_done);
	while (1) {
		spin_lock(&req_lock);
		while (requests) {
			struct req *req = requests;
			requests = NULL;
			spin_unlock(&req_lock);
			while (req) {
				struct req *next = req->next;
				req->err = handle(req->name, req->mode, req->dev);
				complete(&req->done);
				req = next;
			}
			spin_lock(&req_lock);
		}
		__set_current_state(TASK_INTERRUPTIBLE);
		spin_unlock(&req_lock);
		schedule();
	}
	return 0;
out:
	complete(&setup_done);
	return *err;
}

>>>>>>> refs/remotes/origin/cm-10.0
/*
 * Create devtmpfs instance, driver-core devices will add their device
 * nodes here.
 */
int __init devtmpfs_init(void)
{
<<<<<<< HEAD
	int err;
	struct vfsmount *mnt;
	char options[] = "mode=0755";

	err = register_filesystem(&dev_fs_type);
=======
	int err = register_filesystem(&dev_fs_type);
>>>>>>> refs/remotes/origin/cm-10.0
	if (err) {
		printk(KERN_ERR "devtmpfs: unable to register devtmpfs "
		       "type %i\n", err);
		return err;
	}

<<<<<<< HEAD
	mnt = kern_mount_data(&dev_fs_type, options);
	if (IS_ERR(mnt)) {
		err = PTR_ERR(mnt);
=======
	thread = kthread_run(devtmpfsd, &err, "kdevtmpfs");
	if (!IS_ERR(thread)) {
		wait_for_completion(&setup_done);
	} else {
		err = PTR_ERR(thread);
		thread = NULL;
	}

	if (err) {
>>>>>>> refs/remotes/origin/cm-10.0
		printk(KERN_ERR "devtmpfs: unable to create devtmpfs %i\n", err);
		unregister_filesystem(&dev_fs_type);
		return err;
	}
<<<<<<< HEAD
	dev_mnt = mnt;
=======
>>>>>>> refs/remotes/origin/cm-10.0

	printk(KERN_INFO "devtmpfs: initialized\n");
	return 0;
}
