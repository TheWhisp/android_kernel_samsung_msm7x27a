#ifndef _LINUX_PID_NS_H
#define _LINUX_PID_NS_H

#include <linux/sched.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/bug.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/mm.h>
=======
#include <linux/bug.h>
#include <linux/mm.h>
#include <linux/workqueue.h>
>>>>>>> refs/remotes/origin/master
#include <linux/threads.h>
#include <linux/nsproxy.h>
#include <linux/kref.h>

struct pidmap {
       atomic_t nr_free;
       void *page;
};

<<<<<<< HEAD
#define PIDMAP_ENTRIES         ((PID_MAX_LIMIT + 8*PAGE_SIZE - 1)/PAGE_SIZE/8)
=======
#define BITS_PER_PAGE		(PAGE_SIZE * 8)
#define BITS_PER_PAGE_MASK	(BITS_PER_PAGE-1)
#define PIDMAP_ENTRIES		((PID_MAX_LIMIT+BITS_PER_PAGE-1)/BITS_PER_PAGE)
>>>>>>> refs/remotes/origin/master

struct bsd_acct_struct;

struct pid_namespace {
	struct kref kref;
	struct pidmap pidmap[PIDMAP_ENTRIES];
<<<<<<< HEAD
	int last_pid;
=======
	struct rcu_head rcu;
	int last_pid;
	unsigned int nr_hashed;
>>>>>>> refs/remotes/origin/master
	struct task_struct *child_reaper;
	struct kmem_cache *pid_cachep;
	unsigned int level;
	struct pid_namespace *parent;
#ifdef CONFIG_PROC_FS
	struct vfsmount *proc_mnt;
<<<<<<< HEAD
=======
	struct dentry *proc_self;
>>>>>>> refs/remotes/origin/master
#endif
#ifdef CONFIG_BSD_PROCESS_ACCT
	struct bsd_acct_struct *bacct;
#endif
<<<<<<< HEAD
<<<<<<< HEAD
=======
	gid_t pid_gid;
	int hide_pid;
	int reboot;	/* group exit code if this pidns was rebooted */
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct user_namespace *user_ns;
	struct work_struct proc_work;
	kgid_t pid_gid;
	int hide_pid;
	int reboot;	/* group exit code if this pidns was rebooted */
	unsigned int proc_inum;
>>>>>>> refs/remotes/origin/master
};

extern struct pid_namespace init_pid_ns;

<<<<<<< HEAD
=======
#define PIDNS_HASH_ADDING (1U << 31)

>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_PID_NS
static inline struct pid_namespace *get_pid_ns(struct pid_namespace *ns)
{
	if (ns != &init_pid_ns)
		kref_get(&ns->kref);
	return ns;
}

<<<<<<< HEAD
extern struct pid_namespace *copy_pid_ns(unsigned long flags, struct pid_namespace *ns);
extern void free_pid_ns(struct kref *kref);
extern void zap_pid_ns_processes(struct pid_namespace *pid_ns);
<<<<<<< HEAD
=======
extern int reboot_pid_ns(struct pid_namespace *pid_ns, int cmd);
>>>>>>> refs/remotes/origin/cm-10.0

static inline void put_pid_ns(struct pid_namespace *ns)
{
	if (ns != &init_pid_ns)
		kref_put(&ns->kref, free_pid_ns);
}
=======
extern struct pid_namespace *copy_pid_ns(unsigned long flags,
	struct user_namespace *user_ns, struct pid_namespace *ns);
extern void zap_pid_ns_processes(struct pid_namespace *pid_ns);
extern int reboot_pid_ns(struct pid_namespace *pid_ns, int cmd);
extern void put_pid_ns(struct pid_namespace *ns);
>>>>>>> refs/remotes/origin/master

#else /* !CONFIG_PID_NS */
#include <linux/err.h>

static inline struct pid_namespace *get_pid_ns(struct pid_namespace *ns)
{
	return ns;
}

<<<<<<< HEAD
static inline struct pid_namespace *
copy_pid_ns(unsigned long flags, struct pid_namespace *ns)
=======
static inline struct pid_namespace *copy_pid_ns(unsigned long flags,
	struct user_namespace *user_ns, struct pid_namespace *ns)
>>>>>>> refs/remotes/origin/master
{
	if (flags & CLONE_NEWPID)
		ns = ERR_PTR(-EINVAL);
	return ns;
}

static inline void put_pid_ns(struct pid_namespace *ns)
{
}

<<<<<<< HEAD
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static inline void zap_pid_ns_processes(struct pid_namespace *ns)
{
	BUG();
}
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master

static inline int reboot_pid_ns(struct pid_namespace *pid_ns, int cmd)
{
	return 0;
}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#endif /* CONFIG_PID_NS */

extern struct pid_namespace *task_active_pid_ns(struct task_struct *tsk);
void pidhash_init(void);
void pidmap_init(void);

#endif /* _LINUX_PID_NS_H */
