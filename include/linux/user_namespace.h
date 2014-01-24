#ifndef _LINUX_USER_NAMESPACE_H
#define _LINUX_USER_NAMESPACE_H

#include <linux/kref.h>
#include <linux/nsproxy.h>
#include <linux/sched.h>
#include <linux/err.h>

<<<<<<< HEAD
#define UIDHASH_BITS	(CONFIG_BASE_SMALL ? 3 : 7)
#define UIDHASH_SZ	(1 << UIDHASH_BITS)

struct user_namespace {
	struct kref		kref;
	struct hlist_head	uidhash_table[UIDHASH_SZ];
	struct user_struct	*creator;
	struct work_struct	destroyer;
=======
#define UID_GID_MAP_MAX_EXTENTS 5

struct uid_gid_map {	/* 64 bytes -- 1 cache line */
	u32 nr_extents;
	struct uid_gid_extent {
		u32 first;
		u32 lower_first;
		u32 count;
	} extent[UID_GID_MAP_MAX_EXTENTS];
};

struct user_namespace {
	struct uid_gid_map	uid_map;
	struct uid_gid_map	gid_map;
	struct uid_gid_map	projid_map;
	atomic_t		count;
	struct user_namespace	*parent;
	int			level;
	kuid_t			owner;
	kgid_t			group;
	unsigned int		proc_inum;

	/* Register of per-UID persistent keyrings for this namespace */
#ifdef CONFIG_PERSISTENT_KEYRINGS
	struct key		*persistent_keyring_register;
	struct rw_semaphore	persistent_keyring_register_sem;
#endif
>>>>>>> refs/remotes/origin/master
};

extern struct user_namespace init_user_ns;

#ifdef CONFIG_USER_NS

static inline struct user_namespace *get_user_ns(struct user_namespace *ns)
{
	if (ns)
<<<<<<< HEAD
		kref_get(&ns->kref);
=======
		atomic_inc(&ns->count);
>>>>>>> refs/remotes/origin/master
	return ns;
}

extern int create_user_ns(struct cred *new);
<<<<<<< HEAD
extern void free_user_ns(struct kref *kref);

static inline void put_user_ns(struct user_namespace *ns)
{
	if (ns)
		kref_put(&ns->kref, free_user_ns);
}

uid_t user_ns_map_uid(struct user_namespace *to, const struct cred *cred, uid_t uid);
gid_t user_ns_map_gid(struct user_namespace *to, const struct cred *cred, gid_t gid);

=======
extern int unshare_userns(unsigned long unshare_flags, struct cred **new_cred);
extern void free_user_ns(struct user_namespace *ns);

static inline void put_user_ns(struct user_namespace *ns)
{
	if (ns && atomic_dec_and_test(&ns->count))
		free_user_ns(ns);
}

struct seq_operations;
extern struct seq_operations proc_uid_seq_operations;
extern struct seq_operations proc_gid_seq_operations;
extern struct seq_operations proc_projid_seq_operations;
extern ssize_t proc_uid_map_write(struct file *, const char __user *, size_t, loff_t *);
extern ssize_t proc_gid_map_write(struct file *, const char __user *, size_t, loff_t *);
extern ssize_t proc_projid_map_write(struct file *, const char __user *, size_t, loff_t *);
>>>>>>> refs/remotes/origin/master
#else

static inline struct user_namespace *get_user_ns(struct user_namespace *ns)
{
	return &init_user_ns;
}

static inline int create_user_ns(struct cred *new)
{
	return -EINVAL;
}

<<<<<<< HEAD
static inline void put_user_ns(struct user_namespace *ns)
{
}

static inline uid_t user_ns_map_uid(struct user_namespace *to,
	const struct cred *cred, uid_t uid)
{
	return uid;
}
static inline gid_t user_ns_map_gid(struct user_namespace *to,
	const struct cred *cred, gid_t gid)
{
	return gid;
=======
static inline int unshare_userns(unsigned long unshare_flags,
				 struct cred **new_cred)
{
	if (unshare_flags & CLONE_NEWUSER)
		return -EINVAL;
	return 0;
}

static inline void put_user_ns(struct user_namespace *ns)
{
>>>>>>> refs/remotes/origin/master
}

#endif

#endif /* _LINUX_USER_H */
