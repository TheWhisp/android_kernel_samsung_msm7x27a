/*
<<<<<<< HEAD
<<<<<<< HEAD
 * security/tomoyo/common.c
 *
 * Securityfs interface for TOMOYO.
 *
 * Copyright (C) 2005-2010  NTT DATA CORPORATION
=======
 * security/tomoyo/securityfs_if.c
 *
 * Copyright (C) 2005-2011  NTT DATA CORPORATION
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * security/tomoyo/securityfs_if.c
 *
 * Copyright (C) 2005-2011  NTT DATA CORPORATION
>>>>>>> refs/remotes/origin/master
 */

#include <linux/security.h>
#include "common.h"

/**
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
 * tomoyo_check_task_acl - Check permission for task operation.
 *
 * @r:   Pointer to "struct tomoyo_request_info".
 * @ptr: Pointer to "struct tomoyo_acl_info".
 *
 * Returns true if granted, false otherwise.
 */
static bool tomoyo_check_task_acl(struct tomoyo_request_info *r,
				  const struct tomoyo_acl_info *ptr)
{
	const struct tomoyo_task_acl *acl = container_of(ptr, typeof(*acl),
							 head);
	return !tomoyo_pathcmp(r->param.task.domainname, acl->domainname);
}

/**
 * tomoyo_write_self - write() for /sys/kernel/security/tomoyo/self_domain interface.
 *
 * @file:  Pointer to "struct file".
 * @buf:   Domainname to transit to.
 * @count: Size of @buf.
 * @ppos:  Unused.
 *
 * Returns @count on success, negative value otherwise.
 *
 * If domain transition was permitted but the domain transition failed, this
 * function returns error rather than terminating current thread with SIGKILL.
 */
static ssize_t tomoyo_write_self(struct file *file, const char __user *buf,
			      size_t count, loff_t *ppos)
{
	char *data;
	int error;
	if (!count || count >= TOMOYO_EXEC_TMPSIZE - 10)
		return -ENOMEM;
	data = kzalloc(count + 1, GFP_NOFS);
	if (!data)
		return -ENOMEM;
	if (copy_from_user(data, buf, count)) {
		error = -EFAULT;
		goto out;
	}
	tomoyo_normalize_line(data);
	if (tomoyo_correct_domain(data)) {
		const int idx = tomoyo_read_lock();
		struct tomoyo_path_info name;
		struct tomoyo_request_info r;
		name.name = data;
		tomoyo_fill_path_info(&name);
		/* Check "task manual_domain_transition" permission. */
		tomoyo_init_request_info(&r, NULL, TOMOYO_MAC_FILE_EXECUTE);
		r.param_type = TOMOYO_TYPE_MANUAL_TASK_ACL;
		r.param.task.domainname = &name;
		tomoyo_check_acl(&r, tomoyo_check_task_acl);
		if (!r.granted)
			error = -EPERM;
		else {
			struct tomoyo_domain_info *new_domain =
				tomoyo_assign_domain(data, true);
			if (!new_domain) {
				error = -ENOENT;
			} else {
				struct cred *cred = prepare_creds();
				if (!cred) {
					error = -ENOMEM;
				} else {
					struct tomoyo_domain_info *old_domain =
						cred->security;
					cred->security = new_domain;
					atomic_inc(&new_domain->users);
					atomic_dec(&old_domain->users);
					commit_creds(cred);
					error = 0;
				}
			}
		}
		tomoyo_read_unlock(idx);
	} else
		error = -EINVAL;
out:
	kfree(data);
	return error ? error : count;
}

/**
 * tomoyo_read_self - read() for /sys/kernel/security/tomoyo/self_domain interface.
 *
 * @file:  Pointer to "struct file".
 * @buf:   Domainname which current thread belongs to.
 * @count: Size of @buf.
 * @ppos:  Bytes read by now.
 *
 * Returns read size on success, negative value otherwise.
 */
static ssize_t tomoyo_read_self(struct file *file, char __user *buf,
				size_t count, loff_t *ppos)
{
	const char *domain = tomoyo_domain()->domainname->name;
	loff_t len = strlen(domain);
	loff_t pos = *ppos;
	if (pos >= len || !count)
		return 0;
	len -= pos;
	if (count < len)
		len = count;
	if (copy_to_user(buf, domain + pos, len))
		return -EFAULT;
	*ppos += len;
	return len;
}

/* Operations for /sys/kernel/security/tomoyo/self_domain interface. */
static const struct file_operations tomoyo_self_operations = {
	.write = tomoyo_write_self,
	.read  = tomoyo_read_self,
};

/**
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 * tomoyo_open - open() for /sys/kernel/security/tomoyo/ interface.
 *
 * @inode: Pointer to "struct inode".
 * @file:  Pointer to "struct file".
 *
 * Returns 0 on success, negative value otherwise.
 */
static int tomoyo_open(struct inode *inode, struct file *file)
{
<<<<<<< HEAD
	const int key = ((u8 *) file->f_path.dentry->d_inode->i_private)
=======
	const int key = ((u8 *) file_inode(file)->i_private)
>>>>>>> refs/remotes/origin/master
		- ((u8 *) NULL);
	return tomoyo_open_control(key, file);
}

/**
 * tomoyo_release - close() for /sys/kernel/security/tomoyo/ interface.
 *
<<<<<<< HEAD
 * @inode: Pointer to "struct inode".
 * @file:  Pointer to "struct file".
 *
 * Returns 0 on success, negative value otherwise.
 */
static int tomoyo_release(struct inode *inode, struct file *file)
{
<<<<<<< HEAD
	return tomoyo_close_control(file);
}

/**
 * tomoyo_poll - poll() for /proc/ccs/ interface.
 *
 * @file: Pointer to "struct file".
 * @wait: Pointer to "poll_table".
 *
 * Returns 0 on success, negative value otherwise.
=======
	return tomoyo_close_control(file->private_data);
=======
 * @file:  Pointer to "struct file".
 *
 */
static int tomoyo_release(struct inode *inode, struct file *file)
{
	tomoyo_close_control(file->private_data);
	return 0;
>>>>>>> refs/remotes/origin/master
}

/**
 * tomoyo_poll - poll() for /sys/kernel/security/tomoyo/ interface.
 *
 * @file: Pointer to "struct file".
 * @wait: Pointer to "poll_table". Maybe NULL.
 *
 * Returns POLLIN | POLLRDNORM | POLLOUT | POLLWRNORM if ready to read/write,
 * POLLOUT | POLLWRNORM otherwise.
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 */
static unsigned int tomoyo_poll(struct file *file, poll_table *wait)
{
	return tomoyo_poll_control(file, wait);
}

/**
 * tomoyo_read - read() for /sys/kernel/security/tomoyo/ interface.
 *
 * @file:  Pointer to "struct file".
 * @buf:   Pointer to buffer.
 * @count: Size of @buf.
 * @ppos:  Unused.
 *
 * Returns bytes read on success, negative value otherwise.
 */
static ssize_t tomoyo_read(struct file *file, char __user *buf, size_t count,
			   loff_t *ppos)
{
<<<<<<< HEAD
<<<<<<< HEAD
	return tomoyo_read_control(file, buf, count);
=======
	return tomoyo_read_control(file->private_data, buf, count);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return tomoyo_read_control(file->private_data, buf, count);
>>>>>>> refs/remotes/origin/master
}

/**
 * tomoyo_write - write() for /sys/kernel/security/tomoyo/ interface.
 *
 * @file:  Pointer to "struct file".
 * @buf:   Pointer to buffer.
 * @count: Size of @buf.
 * @ppos:  Unused.
 *
 * Returns @count on success, negative value otherwise.
 */
static ssize_t tomoyo_write(struct file *file, const char __user *buf,
			    size_t count, loff_t *ppos)
{
<<<<<<< HEAD
<<<<<<< HEAD
	return tomoyo_write_control(file, buf, count);
=======
	return tomoyo_write_control(file->private_data, buf, count);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return tomoyo_write_control(file->private_data, buf, count);
>>>>>>> refs/remotes/origin/master
}

/*
 * tomoyo_operations is a "struct file_operations" which is used for handling
 * /sys/kernel/security/tomoyo/ interface.
 *
 * Some files under /sys/kernel/security/tomoyo/ directory accept open(O_RDWR).
 * See tomoyo_io_buffer for internals.
 */
static const struct file_operations tomoyo_operations = {
	.open    = tomoyo_open,
	.release = tomoyo_release,
	.poll    = tomoyo_poll,
	.read    = tomoyo_read,
	.write   = tomoyo_write,
	.llseek  = noop_llseek,
};

/**
 * tomoyo_create_entry - Create interface files under /sys/kernel/security/tomoyo/ directory.
 *
 * @name:   The name of the interface file.
 * @mode:   The permission of the interface file.
 * @parent: The parent directory.
 * @key:    Type of interface.
 *
 * Returns nothing.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static void __init tomoyo_create_entry(const char *name, const mode_t mode,
=======
static void __init tomoyo_create_entry(const char *name, const umode_t mode,
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void __init tomoyo_create_entry(const char *name, const umode_t mode,
>>>>>>> refs/remotes/origin/master
				       struct dentry *parent, const u8 key)
{
	securityfs_create_file(name, mode, parent, ((u8 *) NULL) + key,
			       &tomoyo_operations);
}

/**
 * tomoyo_initerface_init - Initialize /sys/kernel/security/tomoyo/ interface.
 *
 * Returns 0.
 */
static int __init tomoyo_initerface_init(void)
{
	struct dentry *tomoyo_dir;

	/* Don't create securityfs entries unless registered. */
	if (current_cred()->security != &tomoyo_kernel_domain)
		return 0;

	tomoyo_dir = securityfs_create_dir("tomoyo", NULL);
	tomoyo_create_entry("query",            0600, tomoyo_dir,
			    TOMOYO_QUERY);
	tomoyo_create_entry("domain_policy",    0600, tomoyo_dir,
			    TOMOYO_DOMAINPOLICY);
	tomoyo_create_entry("exception_policy", 0600, tomoyo_dir,
			    TOMOYO_EXCEPTIONPOLICY);
<<<<<<< HEAD
<<<<<<< HEAD
	tomoyo_create_entry("self_domain",      0400, tomoyo_dir,
			    TOMOYO_SELFDOMAIN);
	tomoyo_create_entry(".domain_status",   0600, tomoyo_dir,
			    TOMOYO_DOMAIN_STATUS);
	tomoyo_create_entry(".process_status",  0600, tomoyo_dir,
			    TOMOYO_PROCESS_STATUS);
	tomoyo_create_entry("meminfo",          0600, tomoyo_dir,
			    TOMOYO_MEMINFO);
=======
=======
>>>>>>> refs/remotes/origin/master
	tomoyo_create_entry("audit",            0400, tomoyo_dir,
			    TOMOYO_AUDIT);
	tomoyo_create_entry(".process_status",  0600, tomoyo_dir,
			    TOMOYO_PROCESS_STATUS);
	tomoyo_create_entry("stat",             0644, tomoyo_dir,
			    TOMOYO_STAT);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	tomoyo_create_entry("profile",          0600, tomoyo_dir,
			    TOMOYO_PROFILE);
	tomoyo_create_entry("manager",          0600, tomoyo_dir,
			    TOMOYO_MANAGER);
	tomoyo_create_entry("version",          0400, tomoyo_dir,
			    TOMOYO_VERSION);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	securityfs_create_file("self_domain", 0666, tomoyo_dir, NULL,
			       &tomoyo_self_operations);
	tomoyo_load_builtin_policy();
>>>>>>> refs/remotes/origin/cm-10.0
=======
	securityfs_create_file("self_domain", 0666, tomoyo_dir, NULL,
			       &tomoyo_self_operations);
	tomoyo_load_builtin_policy();
>>>>>>> refs/remotes/origin/master
	return 0;
}

fs_initcall(tomoyo_initerface_init);
