/*
 * linux/fs/posix_acl.c
 *
 *  Copyright (C) 2002 by Andreas Gruenbacher <a.gruenbacher@computer.org>
 *
 *  Fixes from William Schumacher incorporated on 15 March 2001.
 *     (Reported by Charles Bertsch, <CBertsch@microtest.com>).
 */

/*
 *  This file contains generic functions for manipulating
 *  POSIX 1003.1e draft standard 17 ACLs.
 */

#include <linux/kernel.h>
#include <linux/slab.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/atomic.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/posix_acl.h>
#include <linux/module.h>
=======
=======
>>>>>>> refs/remotes/origin/master
#include <linux/atomic.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/posix_acl.h>
#include <linux/export.h>
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

#include <linux/errno.h>

EXPORT_SYMBOL(posix_acl_init);
EXPORT_SYMBOL(posix_acl_alloc);
<<<<<<< HEAD
EXPORT_SYMBOL(posix_acl_clone);
EXPORT_SYMBOL(posix_acl_valid);
EXPORT_SYMBOL(posix_acl_equiv_mode);
EXPORT_SYMBOL(posix_acl_from_mode);
EXPORT_SYMBOL(posix_acl_create_masq);
EXPORT_SYMBOL(posix_acl_chmod_masq);
EXPORT_SYMBOL(posix_acl_permission);
=======
EXPORT_SYMBOL(posix_acl_valid);
EXPORT_SYMBOL(posix_acl_equiv_mode);
EXPORT_SYMBOL(posix_acl_from_mode);
>>>>>>> refs/remotes/origin/cm-10.0
=======

#include <linux/errno.h>

struct posix_acl **acl_by_type(struct inode *inode, int type)
{
	switch (type) {
	case ACL_TYPE_ACCESS:
		return &inode->i_acl;
	case ACL_TYPE_DEFAULT:
		return &inode->i_default_acl;
	default:
		BUG();
	}
}
EXPORT_SYMBOL(acl_by_type);

struct posix_acl *get_cached_acl(struct inode *inode, int type)
{
	struct posix_acl **p = acl_by_type(inode, type);
	struct posix_acl *acl = ACCESS_ONCE(*p);
	if (acl) {
		spin_lock(&inode->i_lock);
		acl = *p;
		if (acl != ACL_NOT_CACHED)
			acl = posix_acl_dup(acl);
		spin_unlock(&inode->i_lock);
	}
	return acl;
}
EXPORT_SYMBOL(get_cached_acl);

struct posix_acl *get_cached_acl_rcu(struct inode *inode, int type)
{
	return rcu_dereference(*acl_by_type(inode, type));
}
EXPORT_SYMBOL(get_cached_acl_rcu);

void set_cached_acl(struct inode *inode, int type, struct posix_acl *acl)
{
	struct posix_acl **p = acl_by_type(inode, type);
	struct posix_acl *old;
	spin_lock(&inode->i_lock);
	old = *p;
	rcu_assign_pointer(*p, posix_acl_dup(acl));
	spin_unlock(&inode->i_lock);
	if (old != ACL_NOT_CACHED)
		posix_acl_release(old);
}
EXPORT_SYMBOL(set_cached_acl);

void forget_cached_acl(struct inode *inode, int type)
{
	struct posix_acl **p = acl_by_type(inode, type);
	struct posix_acl *old;
	spin_lock(&inode->i_lock);
	old = *p;
	*p = ACL_NOT_CACHED;
	spin_unlock(&inode->i_lock);
	if (old != ACL_NOT_CACHED)
		posix_acl_release(old);
}
EXPORT_SYMBOL(forget_cached_acl);

void forget_all_cached_acls(struct inode *inode)
{
	struct posix_acl *old_access, *old_default;
	spin_lock(&inode->i_lock);
	old_access = inode->i_acl;
	old_default = inode->i_default_acl;
	inode->i_acl = inode->i_default_acl = ACL_NOT_CACHED;
	spin_unlock(&inode->i_lock);
	if (old_access != ACL_NOT_CACHED)
		posix_acl_release(old_access);
	if (old_default != ACL_NOT_CACHED)
		posix_acl_release(old_default);
}
EXPORT_SYMBOL(forget_all_cached_acls);
>>>>>>> refs/remotes/origin/master

/*
 * Init a fresh posix_acl
 */
void
posix_acl_init(struct posix_acl *acl, int count)
{
	atomic_set(&acl->a_refcount, 1);
	acl->a_count = count;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL(posix_acl_init);
>>>>>>> refs/remotes/origin/master

/*
 * Allocate a new ACL with the specified number of entries.
 */
struct posix_acl *
posix_acl_alloc(int count, gfp_t flags)
{
	const size_t size = sizeof(struct posix_acl) +
	                    count * sizeof(struct posix_acl_entry);
	struct posix_acl *acl = kmalloc(size, flags);
	if (acl)
		posix_acl_init(acl, count);
	return acl;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL(posix_acl_alloc);
>>>>>>> refs/remotes/origin/master

/*
 * Clone an ACL.
 */
<<<<<<< HEAD
<<<<<<< HEAD
struct posix_acl *
=======
static struct posix_acl *
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct posix_acl *
>>>>>>> refs/remotes/origin/master
posix_acl_clone(const struct posix_acl *acl, gfp_t flags)
{
	struct posix_acl *clone = NULL;

	if (acl) {
		int size = sizeof(struct posix_acl) + acl->a_count *
		           sizeof(struct posix_acl_entry);
		clone = kmemdup(acl, size, flags);
		if (clone)
			atomic_set(&clone->a_refcount, 1);
	}
	return clone;
}

/*
 * Check if an acl is valid. Returns 0 if it is, or -E... otherwise.
 */
int
posix_acl_valid(const struct posix_acl *acl)
{
	const struct posix_acl_entry *pa, *pe;
	int state = ACL_USER_OBJ;
<<<<<<< HEAD
	unsigned int id = 0;  /* keep gcc happy */
=======
	kuid_t prev_uid = INVALID_UID;
	kgid_t prev_gid = INVALID_GID;
>>>>>>> refs/remotes/origin/master
	int needs_mask = 0;

	FOREACH_ACL_ENTRY(pa, acl, pe) {
		if (pa->e_perm & ~(ACL_READ|ACL_WRITE|ACL_EXECUTE))
			return -EINVAL;
		switch (pa->e_tag) {
			case ACL_USER_OBJ:
				if (state == ACL_USER_OBJ) {
<<<<<<< HEAD
					id = 0;
=======
>>>>>>> refs/remotes/origin/master
					state = ACL_USER;
					break;
				}
				return -EINVAL;

			case ACL_USER:
				if (state != ACL_USER)
					return -EINVAL;
<<<<<<< HEAD
				if (pa->e_id == ACL_UNDEFINED_ID ||
				    pa->e_id < id)
					return -EINVAL;
				id = pa->e_id + 1;
=======
				if (!uid_valid(pa->e_uid))
					return -EINVAL;
				if (uid_valid(prev_uid) &&
				    uid_lte(pa->e_uid, prev_uid))
					return -EINVAL;
				prev_uid = pa->e_uid;
>>>>>>> refs/remotes/origin/master
				needs_mask = 1;
				break;

			case ACL_GROUP_OBJ:
				if (state == ACL_USER) {
<<<<<<< HEAD
					id = 0;
=======
>>>>>>> refs/remotes/origin/master
					state = ACL_GROUP;
					break;
				}
				return -EINVAL;

			case ACL_GROUP:
				if (state != ACL_GROUP)
					return -EINVAL;
<<<<<<< HEAD
				if (pa->e_id == ACL_UNDEFINED_ID ||
				    pa->e_id < id)
					return -EINVAL;
				id = pa->e_id + 1;
=======
				if (!gid_valid(pa->e_gid))
					return -EINVAL;
				if (gid_valid(prev_gid) &&
				    gid_lte(pa->e_gid, prev_gid))
					return -EINVAL;
				prev_gid = pa->e_gid;
>>>>>>> refs/remotes/origin/master
				needs_mask = 1;
				break;

			case ACL_MASK:
				if (state != ACL_GROUP)
					return -EINVAL;
				state = ACL_OTHER;
				break;

			case ACL_OTHER:
				if (state == ACL_OTHER ||
				    (state == ACL_GROUP && !needs_mask)) {
					state = 0;
					break;
				}
				return -EINVAL;

			default:
				return -EINVAL;
		}
	}
	if (state == 0)
		return 0;
	return -EINVAL;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL(posix_acl_valid);
>>>>>>> refs/remotes/origin/master

/*
 * Returns 0 if the acl can be exactly represented in the traditional
 * file mode permission bits, or else 1. Returns -E... on error.
 */
int
<<<<<<< HEAD
<<<<<<< HEAD
posix_acl_equiv_mode(const struct posix_acl *acl, mode_t *mode_p)
{
	const struct posix_acl_entry *pa, *pe;
	mode_t mode = 0;
=======
=======
>>>>>>> refs/remotes/origin/master
posix_acl_equiv_mode(const struct posix_acl *acl, umode_t *mode_p)
{
	const struct posix_acl_entry *pa, *pe;
	umode_t mode = 0;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	int not_equiv = 0;

	FOREACH_ACL_ENTRY(pa, acl, pe) {
		switch (pa->e_tag) {
			case ACL_USER_OBJ:
				mode |= (pa->e_perm & S_IRWXO) << 6;
				break;
			case ACL_GROUP_OBJ:
				mode |= (pa->e_perm & S_IRWXO) << 3;
				break;
			case ACL_OTHER:
				mode |= pa->e_perm & S_IRWXO;
				break;
			case ACL_MASK:
				mode = (mode & ~S_IRWXG) |
				       ((pa->e_perm & S_IRWXO) << 3);
				not_equiv = 1;
				break;
			case ACL_USER:
			case ACL_GROUP:
				not_equiv = 1;
				break;
			default:
				return -EINVAL;
		}
	}
        if (mode_p)
                *mode_p = (*mode_p & ~S_IRWXUGO) | mode;
        return not_equiv;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL(posix_acl_equiv_mode);
>>>>>>> refs/remotes/origin/master

/*
 * Create an ACL representing the file mode permission bits of an inode.
 */
struct posix_acl *
<<<<<<< HEAD
<<<<<<< HEAD
posix_acl_from_mode(mode_t mode, gfp_t flags)
=======
posix_acl_from_mode(umode_t mode, gfp_t flags)
>>>>>>> refs/remotes/origin/cm-10.0
=======
posix_acl_from_mode(umode_t mode, gfp_t flags)
>>>>>>> refs/remotes/origin/master
{
	struct posix_acl *acl = posix_acl_alloc(3, flags);
	if (!acl)
		return ERR_PTR(-ENOMEM);

	acl->a_entries[0].e_tag  = ACL_USER_OBJ;
<<<<<<< HEAD
	acl->a_entries[0].e_id   = ACL_UNDEFINED_ID;
	acl->a_entries[0].e_perm = (mode & S_IRWXU) >> 6;

	acl->a_entries[1].e_tag  = ACL_GROUP_OBJ;
	acl->a_entries[1].e_id   = ACL_UNDEFINED_ID;
	acl->a_entries[1].e_perm = (mode & S_IRWXG) >> 3;

	acl->a_entries[2].e_tag  = ACL_OTHER;
	acl->a_entries[2].e_id   = ACL_UNDEFINED_ID;
	acl->a_entries[2].e_perm = (mode & S_IRWXO);
	return acl;
}
=======
	acl->a_entries[0].e_perm = (mode & S_IRWXU) >> 6;

	acl->a_entries[1].e_tag  = ACL_GROUP_OBJ;
	acl->a_entries[1].e_perm = (mode & S_IRWXG) >> 3;

	acl->a_entries[2].e_tag  = ACL_OTHER;
	acl->a_entries[2].e_perm = (mode & S_IRWXO);
	return acl;
}
EXPORT_SYMBOL(posix_acl_from_mode);
>>>>>>> refs/remotes/origin/master

/*
 * Return 0 if current is granted want access to the inode
 * by the acl. Returns -E... otherwise.
 */
int
posix_acl_permission(struct inode *inode, const struct posix_acl *acl, int want)
{
	const struct posix_acl_entry *pa, *pe, *mask_obj;
	int found = 0;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	want &= MAY_READ | MAY_WRITE | MAY_EXEC | MAY_NOT_BLOCK;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	want &= MAY_READ | MAY_WRITE | MAY_EXEC | MAY_NOT_BLOCK;

>>>>>>> refs/remotes/origin/master
	FOREACH_ACL_ENTRY(pa, acl, pe) {
                switch(pa->e_tag) {
                        case ACL_USER_OBJ:
				/* (May have been checked already) */
<<<<<<< HEAD
				if (inode->i_uid == current_fsuid())
                                        goto check_perm;
                                break;
                        case ACL_USER:
				if (pa->e_id == current_fsuid())
=======
				if (uid_eq(inode->i_uid, current_fsuid()))
                                        goto check_perm;
                                break;
                        case ACL_USER:
				if (uid_eq(pa->e_uid, current_fsuid()))
>>>>>>> refs/remotes/origin/master
                                        goto mask;
				break;
                        case ACL_GROUP_OBJ:
                                if (in_group_p(inode->i_gid)) {
					found = 1;
					if ((pa->e_perm & want) == want)
						goto mask;
                                }
				break;
                        case ACL_GROUP:
<<<<<<< HEAD
                                if (in_group_p(pa->e_id)) {
=======
				if (in_group_p(pa->e_gid)) {
>>>>>>> refs/remotes/origin/master
					found = 1;
					if ((pa->e_perm & want) == want)
						goto mask;
                                }
                                break;
                        case ACL_MASK:
                                break;
                        case ACL_OTHER:
				if (found)
					return -EACCES;
				else
					goto check_perm;
			default:
				return -EIO;
                }
        }
	return -EIO;

mask:
	for (mask_obj = pa+1; mask_obj != pe; mask_obj++) {
		if (mask_obj->e_tag == ACL_MASK) {
			if ((pa->e_perm & mask_obj->e_perm & want) == want)
				return 0;
			return -EACCES;
		}
	}

check_perm:
	if ((pa->e_perm & want) == want)
		return 0;
	return -EACCES;
}

/*
 * Modify acl when creating a new inode. The caller must ensure the acl is
 * only referenced once.
 *
 * mode_p initially must contain the mode parameter to the open() / creat()
 * system calls. All permissions that are not granted by the acl are removed.
 * The permissions in the acl are changed to reflect the mode_p parameter.
 */
<<<<<<< HEAD
<<<<<<< HEAD
int
posix_acl_create_masq(struct posix_acl *acl, mode_t *mode_p)
{
	struct posix_acl_entry *pa, *pe;
	struct posix_acl_entry *group_obj = NULL, *mask_obj = NULL;
	mode_t mode = *mode_p;
=======
=======
>>>>>>> refs/remotes/origin/master
static int posix_acl_create_masq(struct posix_acl *acl, umode_t *mode_p)
{
	struct posix_acl_entry *pa, *pe;
	struct posix_acl_entry *group_obj = NULL, *mask_obj = NULL;
	umode_t mode = *mode_p;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	int not_equiv = 0;

	/* assert(atomic_read(acl->a_refcount) == 1); */

	FOREACH_ACL_ENTRY(pa, acl, pe) {
                switch(pa->e_tag) {
                        case ACL_USER_OBJ:
				pa->e_perm &= (mode >> 6) | ~S_IRWXO;
				mode &= (pa->e_perm << 6) | ~S_IRWXU;
				break;

			case ACL_USER:
			case ACL_GROUP:
				not_equiv = 1;
				break;

                        case ACL_GROUP_OBJ:
				group_obj = pa;
                                break;

                        case ACL_OTHER:
				pa->e_perm &= mode | ~S_IRWXO;
				mode &= pa->e_perm | ~S_IRWXO;
                                break;

                        case ACL_MASK:
				mask_obj = pa;
				not_equiv = 1;
                                break;

			default:
				return -EIO;
                }
        }

	if (mask_obj) {
		mask_obj->e_perm &= (mode >> 3) | ~S_IRWXO;
		mode &= (mask_obj->e_perm << 3) | ~S_IRWXG;
	} else {
		if (!group_obj)
			return -EIO;
		group_obj->e_perm &= (mode >> 3) | ~S_IRWXO;
		mode &= (group_obj->e_perm << 3) | ~S_IRWXG;
	}

	*mode_p = (*mode_p & ~S_IRWXUGO) | mode;
        return not_equiv;
}

/*
 * Modify the ACL for the chmod syscall.
 */
<<<<<<< HEAD
<<<<<<< HEAD
int
posix_acl_chmod_masq(struct posix_acl *acl, mode_t mode)
=======
static int posix_acl_chmod_masq(struct posix_acl *acl, umode_t mode)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int posix_acl_chmod_masq(struct posix_acl *acl, umode_t mode)
>>>>>>> refs/remotes/origin/master
{
	struct posix_acl_entry *group_obj = NULL, *mask_obj = NULL;
	struct posix_acl_entry *pa, *pe;

	/* assert(atomic_read(acl->a_refcount) == 1); */

	FOREACH_ACL_ENTRY(pa, acl, pe) {
		switch(pa->e_tag) {
			case ACL_USER_OBJ:
				pa->e_perm = (mode & S_IRWXU) >> 6;
				break;

			case ACL_USER:
			case ACL_GROUP:
				break;

			case ACL_GROUP_OBJ:
				group_obj = pa;
				break;

			case ACL_MASK:
				mask_obj = pa;
				break;

			case ACL_OTHER:
				pa->e_perm = (mode & S_IRWXO);
				break;

			default:
				return -EIO;
		}
	}

	if (mask_obj) {
		mask_obj->e_perm = (mode & S_IRWXG) >> 3;
	} else {
		if (!group_obj)
			return -EIO;
		group_obj->e_perm = (mode & S_IRWXG) >> 3;
	}

	return 0;
}
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master

int
posix_acl_create(struct posix_acl **acl, gfp_t gfp, umode_t *mode_p)
{
	struct posix_acl *clone = posix_acl_clone(*acl, gfp);
	int err = -ENOMEM;
	if (clone) {
		err = posix_acl_create_masq(clone, mode_p);
		if (err < 0) {
			posix_acl_release(clone);
			clone = NULL;
		}
	}
	posix_acl_release(*acl);
	*acl = clone;
	return err;
}
EXPORT_SYMBOL(posix_acl_create);

int
posix_acl_chmod(struct posix_acl **acl, gfp_t gfp, umode_t mode)
{
	struct posix_acl *clone = posix_acl_clone(*acl, gfp);
	int err = -ENOMEM;
	if (clone) {
		err = posix_acl_chmod_masq(clone, mode);
		if (err) {
			posix_acl_release(clone);
			clone = NULL;
		}
	}
	posix_acl_release(*acl);
	*acl = clone;
	return err;
}
EXPORT_SYMBOL(posix_acl_chmod);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
