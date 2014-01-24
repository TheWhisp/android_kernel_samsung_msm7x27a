/*
 * security/tomoyo/mount.c
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright (C) 2005-2010  NTT DATA CORPORATION
=======
 * Copyright (C) 2005-2011  NTT DATA CORPORATION
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Copyright (C) 2005-2011  NTT DATA CORPORATION
>>>>>>> refs/remotes/origin/master
 */

#include <linux/slab.h>
#include "common.h"

<<<<<<< HEAD
<<<<<<< HEAD
/* Keywords for mount restrictions. */

/* Allow to call 'mount --bind /source_dir /dest_dir' */
#define TOMOYO_MOUNT_BIND_KEYWORD                        "--bind"
/* Allow to call 'mount --move /old_dir    /new_dir ' */
#define TOMOYO_MOUNT_MOVE_KEYWORD                        "--move"
/* Allow to call 'mount -o remount /dir             ' */
#define TOMOYO_MOUNT_REMOUNT_KEYWORD                     "--remount"
/* Allow to call 'mount --make-unbindable /dir'       */
#define TOMOYO_MOUNT_MAKE_UNBINDABLE_KEYWORD             "--make-unbindable"
/* Allow to call 'mount --make-private /dir'          */
#define TOMOYO_MOUNT_MAKE_PRIVATE_KEYWORD                "--make-private"
/* Allow to call 'mount --make-slave /dir'            */
#define TOMOYO_MOUNT_MAKE_SLAVE_KEYWORD                  "--make-slave"
/* Allow to call 'mount --make-shared /dir'           */
#define TOMOYO_MOUNT_MAKE_SHARED_KEYWORD                 "--make-shared"
=======
=======
>>>>>>> refs/remotes/origin/master
/* String table for special mount operations. */
static const char * const tomoyo_mounts[TOMOYO_MAX_SPECIAL_MOUNT] = {
	[TOMOYO_MOUNT_BIND]            = "--bind",
	[TOMOYO_MOUNT_MOVE]            = "--move",
	[TOMOYO_MOUNT_REMOUNT]         = "--remount",
	[TOMOYO_MOUNT_MAKE_UNBINDABLE] = "--make-unbindable",
	[TOMOYO_MOUNT_MAKE_PRIVATE]    = "--make-private",
	[TOMOYO_MOUNT_MAKE_SLAVE]      = "--make-slave",
	[TOMOYO_MOUNT_MAKE_SHARED]     = "--make-shared",
};
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

/**
 * tomoyo_audit_mount_log - Audit mount log.
 *
 * @r: Pointer to "struct tomoyo_request_info".
 *
 * Returns 0 on success, negative value otherwise.
 */
static int tomoyo_audit_mount_log(struct tomoyo_request_info *r)
{
<<<<<<< HEAD
<<<<<<< HEAD
	const char *dev = r->param.mount.dev->name;
	const char *dir = r->param.mount.dir->name;
	const char *type = r->param.mount.type->name;
	const unsigned long flags = r->param.mount.flags;
	if (r->granted)
		return 0;
	if (!strcmp(type, TOMOYO_MOUNT_REMOUNT_KEYWORD))
		tomoyo_warn_log(r, "mount -o remount %s 0x%lX", dir, flags);
	else if (!strcmp(type, TOMOYO_MOUNT_BIND_KEYWORD)
		 || !strcmp(type, TOMOYO_MOUNT_MOVE_KEYWORD))
		tomoyo_warn_log(r, "mount %s %s %s 0x%lX", type, dev, dir,
				flags);
	else if (!strcmp(type, TOMOYO_MOUNT_MAKE_UNBINDABLE_KEYWORD) ||
		 !strcmp(type, TOMOYO_MOUNT_MAKE_PRIVATE_KEYWORD) ||
		 !strcmp(type, TOMOYO_MOUNT_MAKE_SLAVE_KEYWORD) ||
		 !strcmp(type, TOMOYO_MOUNT_MAKE_SHARED_KEYWORD))
		tomoyo_warn_log(r, "mount %s %s 0x%lX", type, dir, flags);
	else
		tomoyo_warn_log(r, "mount -t %s %s %s 0x%lX", type, dev, dir,
				flags);
	return tomoyo_supervisor(r,
				 TOMOYO_KEYWORD_ALLOW_MOUNT "%s %s %s 0x%lX\n",
				 tomoyo_pattern(r->param.mount.dev),
				 tomoyo_pattern(r->param.mount.dir), type,
				 flags);
}

=======
=======
>>>>>>> refs/remotes/origin/master
	return tomoyo_supervisor(r, "file mount %s %s %s 0x%lX\n",
				 r->param.mount.dev->name,
				 r->param.mount.dir->name,
				 r->param.mount.type->name,
				 r->param.mount.flags);
}

/**
 * tomoyo_check_mount_acl - Check permission for path path path number operation.
 *
 * @r:   Pointer to "struct tomoyo_request_info".
 * @ptr: Pointer to "struct tomoyo_acl_info".
 *
 * Returns true if granted, false otherwise.
 */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static bool tomoyo_check_mount_acl(struct tomoyo_request_info *r,
				   const struct tomoyo_acl_info *ptr)
{
	const struct tomoyo_mount_acl *acl =
		container_of(ptr, typeof(*acl), head);
<<<<<<< HEAD
<<<<<<< HEAD
	return tomoyo_compare_number_union(r->param.mount.flags, &acl->flags) &&
		tomoyo_compare_name_union(r->param.mount.type, &acl->fs_type) &&
		tomoyo_compare_name_union(r->param.mount.dir, &acl->dir_name) &&
		(!r->param.mount.need_dev ||
		 tomoyo_compare_name_union(r->param.mount.dev, &acl->dev_name));
=======
=======
>>>>>>> refs/remotes/origin/master
	return tomoyo_compare_number_union(r->param.mount.flags,
					   &acl->flags) &&
		tomoyo_compare_name_union(r->param.mount.type,
					  &acl->fs_type) &&
		tomoyo_compare_name_union(r->param.mount.dir,
					  &acl->dir_name) &&
		(!r->param.mount.need_dev ||
		 tomoyo_compare_name_union(r->param.mount.dev,
					   &acl->dev_name));
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

/**
 * tomoyo_mount_acl - Check permission for mount() operation.
 *
 * @r:        Pointer to "struct tomoyo_request_info".
<<<<<<< HEAD
<<<<<<< HEAD
 * @dev_name: Name of device file.
=======
 * @dev_name: Name of device file. Maybe NULL.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * @dev_name: Name of device file. Maybe NULL.
>>>>>>> refs/remotes/origin/master
 * @dir:      Pointer to "struct path".
 * @type:     Name of filesystem type.
 * @flags:    Mount options.
 *
 * Returns 0 on success, negative value otherwise.
 *
 * Caller holds tomoyo_read_lock().
 */
<<<<<<< HEAD
static int tomoyo_mount_acl(struct tomoyo_request_info *r, char *dev_name,
<<<<<<< HEAD
			    struct path *dir, char *type, unsigned long flags)
{
=======
=======
static int tomoyo_mount_acl(struct tomoyo_request_info *r,
			    const char *dev_name,
>>>>>>> refs/remotes/origin/master
			    struct path *dir, const char *type,
			    unsigned long flags)
{
	struct tomoyo_obj_info obj = { };
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	struct path path;
	struct file_system_type *fstype = NULL;
	const char *requested_type = NULL;
	const char *requested_dir_name = NULL;
	const char *requested_dev_name = NULL;
	struct tomoyo_path_info rtype;
	struct tomoyo_path_info rdev;
	struct tomoyo_path_info rdir;
	int need_dev = 0;
	int error = -ENOMEM;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	r->obj = &obj;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	r->obj = &obj;
>>>>>>> refs/remotes/origin/master

	/* Get fstype. */
	requested_type = tomoyo_encode(type);
	if (!requested_type)
		goto out;
	rtype.name = requested_type;
	tomoyo_fill_path_info(&rtype);

	/* Get mount point. */
<<<<<<< HEAD
<<<<<<< HEAD
=======
	obj.path2 = *dir;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	obj.path2 = *dir;
>>>>>>> refs/remotes/origin/master
	requested_dir_name = tomoyo_realpath_from_path(dir);
	if (!requested_dir_name) {
		error = -ENOMEM;
		goto out;
	}
	rdir.name = requested_dir_name;
	tomoyo_fill_path_info(&rdir);

	/* Compare fs name. */
<<<<<<< HEAD
<<<<<<< HEAD
	if (!strcmp(type, TOMOYO_MOUNT_REMOUNT_KEYWORD)) {
		/* dev_name is ignored. */
	} else if (!strcmp(type, TOMOYO_MOUNT_MAKE_UNBINDABLE_KEYWORD) ||
		   !strcmp(type, TOMOYO_MOUNT_MAKE_PRIVATE_KEYWORD) ||
		   !strcmp(type, TOMOYO_MOUNT_MAKE_SLAVE_KEYWORD) ||
		   !strcmp(type, TOMOYO_MOUNT_MAKE_SHARED_KEYWORD)) {
		/* dev_name is ignored. */
	} else if (!strcmp(type, TOMOYO_MOUNT_BIND_KEYWORD) ||
		   !strcmp(type, TOMOYO_MOUNT_MOVE_KEYWORD)) {
=======
=======
>>>>>>> refs/remotes/origin/master
	if (type == tomoyo_mounts[TOMOYO_MOUNT_REMOUNT]) {
		/* dev_name is ignored. */
	} else if (type == tomoyo_mounts[TOMOYO_MOUNT_MAKE_UNBINDABLE] ||
		   type == tomoyo_mounts[TOMOYO_MOUNT_MAKE_PRIVATE] ||
		   type == tomoyo_mounts[TOMOYO_MOUNT_MAKE_SLAVE] ||
		   type == tomoyo_mounts[TOMOYO_MOUNT_MAKE_SHARED]) {
		/* dev_name is ignored. */
	} else if (type == tomoyo_mounts[TOMOYO_MOUNT_BIND] ||
		   type == tomoyo_mounts[TOMOYO_MOUNT_MOVE]) {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		need_dev = -1; /* dev_name is a directory */
	} else {
		fstype = get_fs_type(type);
		if (!fstype) {
			error = -ENODEV;
			goto out;
		}
		if (fstype->fs_flags & FS_REQUIRES_DEV)
			/* dev_name is a block device file. */
			need_dev = 1;
	}
	if (need_dev) {
		/* Get mount point or device file. */
		if (!dev_name || kern_path(dev_name, LOOKUP_FOLLOW, &path)) {
			error = -ENOENT;
			goto out;
		}
<<<<<<< HEAD
<<<<<<< HEAD
		requested_dev_name = tomoyo_realpath_from_path(&path);
		path_put(&path);
=======
		obj.path1 = path;
		requested_dev_name = tomoyo_realpath_from_path(&path);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		obj.path1 = path;
		requested_dev_name = tomoyo_realpath_from_path(&path);
>>>>>>> refs/remotes/origin/master
		if (!requested_dev_name) {
			error = -ENOENT;
			goto out;
		}
	} else {
		/* Map dev_name to "<NULL>" if no dev_name given. */
		if (!dev_name)
			dev_name = "<NULL>";
		requested_dev_name = tomoyo_encode(dev_name);
		if (!requested_dev_name) {
			error = -ENOMEM;
			goto out;
		}
	}
	rdev.name = requested_dev_name;
	tomoyo_fill_path_info(&rdev);
	r->param_type = TOMOYO_TYPE_MOUNT_ACL;
	r->param.mount.need_dev = need_dev;
	r->param.mount.dev = &rdev;
	r->param.mount.dir = &rdir;
	r->param.mount.type = &rtype;
	r->param.mount.flags = flags;
	do {
		tomoyo_check_acl(r, tomoyo_check_mount_acl);
		error = tomoyo_audit_mount_log(r);
	} while (error == TOMOYO_RETRY_REQUEST);
 out:
	kfree(requested_dev_name);
	kfree(requested_dir_name);
	if (fstype)
		put_filesystem(fstype);
	kfree(requested_type);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	/* Drop refcount obtained by kern_path(). */
	if (obj.path1.dentry)
		path_put(&obj.path1);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* Drop refcount obtained by kern_path(). */
	if (obj.path1.dentry)
		path_put(&obj.path1);
>>>>>>> refs/remotes/origin/master
	return error;
}

/**
 * tomoyo_mount_permission - Check permission for mount() operation.
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * @dev_name:  Name of device file.
 * @path:      Pointer to "struct path".
 * @type:      Name of filesystem type. May be NULL.
 * @flags:     Mount options.
 * @data_page: Optional data. May be NULL.
 *
 * Returns 0 on success, negative value otherwise.
 */
int tomoyo_mount_permission(char *dev_name, struct path *path, char *type,
			    unsigned long flags, void *data_page)
=======
=======
>>>>>>> refs/remotes/origin/master
 * @dev_name:  Name of device file. Maybe NULL.
 * @path:      Pointer to "struct path".
 * @type:      Name of filesystem type. Maybe NULL.
 * @flags:     Mount options.
 * @data_page: Optional data. Maybe NULL.
 *
 * Returns 0 on success, negative value otherwise.
 */
<<<<<<< HEAD
int tomoyo_mount_permission(char *dev_name, struct path *path,
			    const char *type, unsigned long flags,
			    void *data_page)
>>>>>>> refs/remotes/origin/cm-10.0
=======
int tomoyo_mount_permission(const char *dev_name, struct path *path,
			    const char *type, unsigned long flags,
			    void *data_page)
>>>>>>> refs/remotes/origin/master
{
	struct tomoyo_request_info r;
	int error;
	int idx;

	if (tomoyo_init_request_info(&r, NULL, TOMOYO_MAC_FILE_MOUNT)
	    == TOMOYO_CONFIG_DISABLED)
		return 0;
	if ((flags & MS_MGC_MSK) == MS_MGC_VAL)
		flags &= ~MS_MGC_MSK;
	if (flags & MS_REMOUNT) {
<<<<<<< HEAD
<<<<<<< HEAD
		type = TOMOYO_MOUNT_REMOUNT_KEYWORD;
		flags &= ~MS_REMOUNT;
	} else if (flags & MS_BIND) {
		type = TOMOYO_MOUNT_BIND_KEYWORD;
=======
=======
>>>>>>> refs/remotes/origin/master
		type = tomoyo_mounts[TOMOYO_MOUNT_REMOUNT];
		flags &= ~MS_REMOUNT;
	} else if (flags & MS_BIND) {
		type = tomoyo_mounts[TOMOYO_MOUNT_BIND];
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		flags &= ~MS_BIND;
	} else if (flags & MS_SHARED) {
		if (flags & (MS_PRIVATE | MS_SLAVE | MS_UNBINDABLE))
			return -EINVAL;
<<<<<<< HEAD
<<<<<<< HEAD
		type = TOMOYO_MOUNT_MAKE_SHARED_KEYWORD;
=======
		type = tomoyo_mounts[TOMOYO_MOUNT_MAKE_SHARED];
>>>>>>> refs/remotes/origin/cm-10.0
=======
		type = tomoyo_mounts[TOMOYO_MOUNT_MAKE_SHARED];
>>>>>>> refs/remotes/origin/master
		flags &= ~MS_SHARED;
	} else if (flags & MS_PRIVATE) {
		if (flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE))
			return -EINVAL;
<<<<<<< HEAD
<<<<<<< HEAD
		type = TOMOYO_MOUNT_MAKE_PRIVATE_KEYWORD;
=======
		type = tomoyo_mounts[TOMOYO_MOUNT_MAKE_PRIVATE];
>>>>>>> refs/remotes/origin/cm-10.0
=======
		type = tomoyo_mounts[TOMOYO_MOUNT_MAKE_PRIVATE];
>>>>>>> refs/remotes/origin/master
		flags &= ~MS_PRIVATE;
	} else if (flags & MS_SLAVE) {
		if (flags & (MS_SHARED | MS_PRIVATE | MS_UNBINDABLE))
			return -EINVAL;
<<<<<<< HEAD
<<<<<<< HEAD
		type = TOMOYO_MOUNT_MAKE_SLAVE_KEYWORD;
=======
		type = tomoyo_mounts[TOMOYO_MOUNT_MAKE_SLAVE];
>>>>>>> refs/remotes/origin/cm-10.0
=======
		type = tomoyo_mounts[TOMOYO_MOUNT_MAKE_SLAVE];
>>>>>>> refs/remotes/origin/master
		flags &= ~MS_SLAVE;
	} else if (flags & MS_UNBINDABLE) {
		if (flags & (MS_SHARED | MS_PRIVATE | MS_SLAVE))
			return -EINVAL;
<<<<<<< HEAD
<<<<<<< HEAD
		type = TOMOYO_MOUNT_MAKE_UNBINDABLE_KEYWORD;
		flags &= ~MS_UNBINDABLE;
	} else if (flags & MS_MOVE) {
		type = TOMOYO_MOUNT_MOVE_KEYWORD;
=======
=======
>>>>>>> refs/remotes/origin/master
		type = tomoyo_mounts[TOMOYO_MOUNT_MAKE_UNBINDABLE];
		flags &= ~MS_UNBINDABLE;
	} else if (flags & MS_MOVE) {
		type = tomoyo_mounts[TOMOYO_MOUNT_MOVE];
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		flags &= ~MS_MOVE;
	}
	if (!type)
		type = "<NULL>";
	idx = tomoyo_read_lock();
	error = tomoyo_mount_acl(&r, dev_name, path, type, flags);
	tomoyo_read_unlock(idx);
	return error;
}
<<<<<<< HEAD
<<<<<<< HEAD

static bool tomoyo_same_mount_acl(const struct tomoyo_acl_info *a,
				  const struct tomoyo_acl_info *b)
{
	const struct tomoyo_mount_acl *p1 = container_of(a, typeof(*p1), head);
	const struct tomoyo_mount_acl *p2 = container_of(b, typeof(*p2), head);
	return tomoyo_same_acl_head(&p1->head, &p2->head) &&
		tomoyo_same_name_union(&p1->dev_name, &p2->dev_name) &&
		tomoyo_same_name_union(&p1->dir_name, &p2->dir_name) &&
		tomoyo_same_name_union(&p1->fs_type, &p2->fs_type) &&
		tomoyo_same_number_union(&p1->flags, &p2->flags);
}

/**
 * tomoyo_write_mount - Write "struct tomoyo_mount_acl" list.
 *
 * @data:      String to parse.
 * @domain:    Pointer to "struct tomoyo_domain_info".
 * @is_delete: True if it is a delete request.
 *
 * Returns 0 on success, negative value otherwise.
 *
 * Caller holds tomoyo_read_lock().
 */
int tomoyo_write_mount(char *data, struct tomoyo_domain_info *domain,
		       const bool is_delete)
{
	struct tomoyo_mount_acl e = { .head.type = TOMOYO_TYPE_MOUNT_ACL };
	int error = is_delete ? -ENOENT : -ENOMEM;
	char *w[4];
	if (!tomoyo_tokenize(data, w, sizeof(w)) || !w[3][0])
		return -EINVAL;
	if (!tomoyo_parse_name_union(w[0], &e.dev_name) ||
	    !tomoyo_parse_name_union(w[1], &e.dir_name) ||
	    !tomoyo_parse_name_union(w[2], &e.fs_type) ||
	    !tomoyo_parse_number_union(w[3], &e.flags))
		goto out;
	error = tomoyo_update_domain(&e.head, sizeof(e), is_delete, domain,
				     tomoyo_same_mount_acl, NULL);
 out:
	tomoyo_put_name_union(&e.dev_name);
	tomoyo_put_name_union(&e.dir_name);
	tomoyo_put_name_union(&e.fs_type);
	tomoyo_put_number_union(&e.flags);
	return error;
}
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
