/*
  File: linux/posix_acl.h

  (C) 2002 Andreas Gruenbacher, <a.gruenbacher@computer.org>
*/


#ifndef __LINUX_POSIX_ACL_H
#define __LINUX_POSIX_ACL_H

<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/slab.h>
=======
#include <linux/bug.h>
#include <linux/slab.h>
#include <linux/rcupdate.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/bug.h>
#include <linux/slab.h>
#include <linux/rcupdate.h>
>>>>>>> refs/remotes/origin/master

#define ACL_UNDEFINED_ID	(-1)

/* a_type field in acl_user_posix_entry_t */
#define ACL_TYPE_ACCESS		(0x8000)
#define ACL_TYPE_DEFAULT	(0x4000)

/* e_tag entry in struct posix_acl_entry */
#define ACL_USER_OBJ		(0x01)
#define ACL_USER		(0x02)
#define ACL_GROUP_OBJ		(0x04)
#define ACL_GROUP		(0x08)
#define ACL_MASK		(0x10)
#define ACL_OTHER		(0x20)

/* permissions in the e_perm field */
#define ACL_READ		(0x04)
#define ACL_WRITE		(0x02)
#define ACL_EXECUTE		(0x01)
//#define ACL_ADD		(0x08)
//#define ACL_DELETE		(0x10)

struct posix_acl_entry {
	short			e_tag;
	unsigned short		e_perm;
<<<<<<< HEAD
	unsigned int		e_id;
};

struct posix_acl {
<<<<<<< HEAD
	atomic_t		a_refcount;
=======
=======
	union {
		kuid_t		e_uid;
		kgid_t		e_gid;
#ifndef CONFIG_UIDGID_STRICT_TYPE_CHECKS
		unsigned int	e_id;
#endif
	};
};

struct posix_acl {
>>>>>>> refs/remotes/origin/master
	union {
		atomic_t		a_refcount;
		struct rcu_head		a_rcu;
	};
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	unsigned int		a_count;
	struct posix_acl_entry	a_entries[0];
};

#define FOREACH_ACL_ENTRY(pa, acl, pe) \
	for(pa=(acl)->a_entries, pe=pa+(acl)->a_count; pa<pe; pa++)


/*
 * Duplicate an ACL handle.
 */
static inline struct posix_acl *
posix_acl_dup(struct posix_acl *acl)
{
	if (acl)
		atomic_inc(&acl->a_refcount);
	return acl;
}

/*
 * Free an ACL handle.
 */
static inline void
posix_acl_release(struct posix_acl *acl)
{
	if (acl && atomic_dec_and_test(&acl->a_refcount))
<<<<<<< HEAD
<<<<<<< HEAD
		kfree(acl);
=======
		kfree_rcu(acl, a_rcu);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		kfree_rcu(acl, a_rcu);
>>>>>>> refs/remotes/origin/master
}


/* posix_acl.c */

extern void posix_acl_init(struct posix_acl *, int);
extern struct posix_acl *posix_acl_alloc(int, gfp_t);
<<<<<<< HEAD
<<<<<<< HEAD
extern struct posix_acl *posix_acl_clone(const struct posix_acl *, gfp_t);
extern int posix_acl_valid(const struct posix_acl *);
extern int posix_acl_permission(struct inode *, const struct posix_acl *, int);
extern struct posix_acl *posix_acl_from_mode(mode_t, gfp_t);
extern int posix_acl_equiv_mode(const struct posix_acl *, mode_t *);
extern int posix_acl_create_masq(struct posix_acl *, mode_t *);
extern int posix_acl_chmod_masq(struct posix_acl *, mode_t);
=======
=======
>>>>>>> refs/remotes/origin/master
extern int posix_acl_valid(const struct posix_acl *);
extern int posix_acl_permission(struct inode *, const struct posix_acl *, int);
extern struct posix_acl *posix_acl_from_mode(umode_t, gfp_t);
extern int posix_acl_equiv_mode(const struct posix_acl *, umode_t *);
extern int posix_acl_create(struct posix_acl **, gfp_t, umode_t *);
extern int posix_acl_chmod(struct posix_acl **, gfp_t, umode_t);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

extern struct posix_acl *get_posix_acl(struct inode *, int);
extern int set_posix_acl(struct inode *, int, struct posix_acl *);

<<<<<<< HEAD
#ifdef CONFIG_FS_POSIX_ACL
<<<<<<< HEAD
static inline struct posix_acl *get_cached_acl(struct inode *inode, int type)
{
	struct posix_acl **p, *acl;
	switch (type) {
	case ACL_TYPE_ACCESS:
		p = &inode->i_acl;
		break;
	case ACL_TYPE_DEFAULT:
		p = &inode->i_default_acl;
		break;
	default:
		return ERR_PTR(-EINVAL);
	}
	acl = ACCESS_ONCE(*p);
=======
static inline struct posix_acl **acl_by_type(struct inode *inode, int type)
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

static inline struct posix_acl *get_cached_acl(struct inode *inode, int type)
{
	struct posix_acl **p = acl_by_type(inode, type);
	struct posix_acl *acl = ACCESS_ONCE(*p);
>>>>>>> refs/remotes/origin/cm-10.0
	if (acl) {
		spin_lock(&inode->i_lock);
		acl = *p;
		if (acl != ACL_NOT_CACHED)
			acl = posix_acl_dup(acl);
		spin_unlock(&inode->i_lock);
	}
	return acl;
}

<<<<<<< HEAD
static inline int negative_cached_acl(struct inode *inode, int type)
{
	struct posix_acl **p, *acl;
	switch (type) {
	case ACL_TYPE_ACCESS:
		p = &inode->i_acl;
		break;
	case ACL_TYPE_DEFAULT:
		p = &inode->i_default_acl;
		break;
	default:
		BUG();
	}
	acl = ACCESS_ONCE(*p);
	if (acl)
		return 0;
	return 1;
=======
static inline struct posix_acl *get_cached_acl_rcu(struct inode *inode, int type)
{
	return rcu_dereference(*acl_by_type(inode, type));
>>>>>>> refs/remotes/origin/cm-10.0
}

static inline void set_cached_acl(struct inode *inode,
				  int type,
				  struct posix_acl *acl)
{
<<<<<<< HEAD
	struct posix_acl *old = NULL;
	spin_lock(&inode->i_lock);
	switch (type) {
	case ACL_TYPE_ACCESS:
		old = inode->i_acl;
		inode->i_acl = posix_acl_dup(acl);
		break;
	case ACL_TYPE_DEFAULT:
		old = inode->i_default_acl;
		inode->i_default_acl = posix_acl_dup(acl);
		break;
	}
=======
	struct posix_acl **p = acl_by_type(inode, type);
	struct posix_acl *old;
	spin_lock(&inode->i_lock);
	old = *p;
	rcu_assign_pointer(*p, posix_acl_dup(acl));
>>>>>>> refs/remotes/origin/cm-10.0
	spin_unlock(&inode->i_lock);
	if (old != ACL_NOT_CACHED)
		posix_acl_release(old);
}

static inline void forget_cached_acl(struct inode *inode, int type)
{
<<<<<<< HEAD
	struct posix_acl *old = NULL;
	spin_lock(&inode->i_lock);
	switch (type) {
	case ACL_TYPE_ACCESS:
		old = inode->i_acl;
		inode->i_acl = ACL_NOT_CACHED;
		break;
	case ACL_TYPE_DEFAULT:
		old = inode->i_default_acl;
		inode->i_default_acl = ACL_NOT_CACHED;
		break;
	}
=======
	struct posix_acl **p = acl_by_type(inode, type);
	struct posix_acl *old;
	spin_lock(&inode->i_lock);
	old = *p;
	*p = ACL_NOT_CACHED;
>>>>>>> refs/remotes/origin/cm-10.0
	spin_unlock(&inode->i_lock);
	if (old != ACL_NOT_CACHED)
		posix_acl_release(old);
}

static inline void forget_all_cached_acls(struct inode *inode)
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
#endif
=======
struct posix_acl **acl_by_type(struct inode *inode, int type);
struct posix_acl *get_cached_acl(struct inode *inode, int type);
struct posix_acl *get_cached_acl_rcu(struct inode *inode, int type);
void set_cached_acl(struct inode *inode, int type, struct posix_acl *acl);
void forget_cached_acl(struct inode *inode, int type);
void forget_all_cached_acls(struct inode *inode);
>>>>>>> refs/remotes/origin/master

static inline void cache_no_acl(struct inode *inode)
{
#ifdef CONFIG_FS_POSIX_ACL
	inode->i_acl = NULL;
	inode->i_default_acl = NULL;
#endif
}

#endif  /* __LINUX_POSIX_ACL_H */
