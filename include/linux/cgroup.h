#ifndef _LINUX_CGROUP_H
#define _LINUX_CGROUP_H
/*
 *  cgroup interface
 *
 *  Copyright (C) 2003 BULL SA
 *  Copyright (C) 2004-2006 Silicon Graphics, Inc.
 *
 */

#include <linux/sched.h>
#include <linux/cpumask.h>
#include <linux/nodemask.h>
#include <linux/rcupdate.h>
<<<<<<< HEAD
=======
#include <linux/rculist.h>
>>>>>>> refs/remotes/origin/master
#include <linux/cgroupstats.h>
#include <linux/prio_heap.h>
#include <linux/rwsem.h>
#include <linux/idr.h>
<<<<<<< HEAD
=======
#include <linux/workqueue.h>
#include <linux/xattr.h>
#include <linux/fs.h>
#include <linux/percpu-refcount.h>
#include <linux/seq_file.h>
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_CGROUPS

struct cgroupfs_root;
struct cgroup_subsys;
struct inode;
struct cgroup;
<<<<<<< HEAD
struct css_id;

extern int cgroup_init_early(void);
extern int cgroup_init(void);
extern void cgroup_lock(void);
extern int cgroup_lock_is_held(void);
extern bool cgroup_lock_live_group(struct cgroup *cgrp);
extern void cgroup_unlock(void);
extern void cgroup_fork(struct task_struct *p);
extern void cgroup_fork_callbacks(struct task_struct *p);
=======

extern int cgroup_init_early(void);
extern int cgroup_init(void);
extern void cgroup_fork(struct task_struct *p);
>>>>>>> refs/remotes/origin/master
extern void cgroup_post_fork(struct task_struct *p);
extern void cgroup_exit(struct task_struct *p, int run_callbacks);
extern int cgroupstats_build(struct cgroupstats *stats,
				struct dentry *dentry);
extern int cgroup_load_subsys(struct cgroup_subsys *ss);
extern void cgroup_unload_subsys(struct cgroup_subsys *ss);

<<<<<<< HEAD
extern const struct file_operations proc_cgroup_operations;

/* Define the enumeration of all builtin cgroup subsystems */
#define SUBSYS(_x) _x ## _subsys_id,
enum cgroup_subsys_id {
#include <linux/cgroup_subsys.h>
	CGROUP_BUILTIN_SUBSYS_COUNT
};
#undef SUBSYS
/*
 * This define indicates the maximum number of subsystems that can be loaded
 * at once. We limit to this many since cgroupfs_root has subsys_bits to keep
 * track of all of them.
 */
#define CGROUP_SUBSYS_COUNT (BITS_PER_BYTE*sizeof(unsigned long))

/* Per-subsystem/per-cgroup state maintained by the system. */
struct cgroup_subsys_state {
	/*
	 * The cgroup that this subsystem is attached to. Useful
	 * for subsystems that want to know about the cgroup
	 * hierarchy structure
	 */
	struct cgroup *cgroup;

	/*
	 * State maintained by the cgroup system to allow subsystems
	 * to be "busy". Should be accessed via css_get(),
	 * css_tryget() and and css_put().
	 */

	atomic_t refcnt;

	unsigned long flags;
	/* ID for this css, if possible */
	struct css_id __rcu *id;
=======
extern int proc_cgroup_show(struct seq_file *, void *);

/*
 * Define the enumeration of all cgroup subsystems.
 *
 * We define ids for builtin subsystems and then modular ones.
 */
#define SUBSYS(_x) _x ## _subsys_id,
enum cgroup_subsys_id {
#define IS_SUBSYS_ENABLED(option) IS_BUILTIN(option)
#include <linux/cgroup_subsys.h>
#undef IS_SUBSYS_ENABLED
	CGROUP_BUILTIN_SUBSYS_COUNT,

	__CGROUP_SUBSYS_TEMP_PLACEHOLDER = CGROUP_BUILTIN_SUBSYS_COUNT - 1,

#define IS_SUBSYS_ENABLED(option) IS_MODULE(option)
#include <linux/cgroup_subsys.h>
#undef IS_SUBSYS_ENABLED
	CGROUP_SUBSYS_COUNT,
};
#undef SUBSYS

/* Per-subsystem/per-cgroup state maintained by the system. */
struct cgroup_subsys_state {
	/* the cgroup that this css is attached to */
	struct cgroup *cgroup;

	/* the cgroup subsystem that this css is attached to */
	struct cgroup_subsys *ss;

	/* reference count - access via css_[try]get() and css_put() */
	struct percpu_ref refcnt;

	/* the parent css */
	struct cgroup_subsys_state *parent;

	unsigned long flags;

	/* percpu_ref killing and RCU release */
	struct rcu_head rcu_head;
	struct work_struct destroy_work;
>>>>>>> refs/remotes/origin/master
};

/* bits in struct cgroup_subsys_state flags field */
enum {
<<<<<<< HEAD
	CSS_ROOT, /* This CSS is the root of the subsystem */
	CSS_REMOVED, /* This CSS is dead */
};

/*
 * Call css_get() to hold a reference on the css; it can be used
 * for a reference obtained via:
 * - an existing ref-counted reference to the css
 * - task->cgroups for a locked task
 */

extern void __css_get(struct cgroup_subsys_state *css, int count);
static inline void css_get(struct cgroup_subsys_state *css)
{
	/* We don't need to reference count the root state */
	if (!test_bit(CSS_ROOT, &css->flags))
		__css_get(css, 1);
}

static inline bool css_is_removed(struct cgroup_subsys_state *css)
{
	return test_bit(CSS_REMOVED, &css->flags);
}

/*
 * Call css_tryget() to take a reference on a css if your existing
 * (known-valid) reference isn't already ref-counted. Returns false if
 * the css has been destroyed.
 */

static inline bool css_tryget(struct cgroup_subsys_state *css)
{
	if (test_bit(CSS_ROOT, &css->flags))
		return true;
	while (!atomic_inc_not_zero(&css->refcnt)) {
		if (test_bit(CSS_REMOVED, &css->flags))
			return false;
		cpu_relax();
	}
	return true;
}

/*
 * css_put() should be called to release a reference taken by
 * css_get() or css_tryget()
 */

extern void __css_put(struct cgroup_subsys_state *css, int count);
static inline void css_put(struct cgroup_subsys_state *css)
{
	if (!test_bit(CSS_ROOT, &css->flags))
		__css_put(css, 1);
=======
	CSS_ROOT	= (1 << 0), /* this CSS is the root of the subsystem */
	CSS_ONLINE	= (1 << 1), /* between ->css_online() and ->css_offline() */
};

/**
 * css_get - obtain a reference on the specified css
 * @css: target css
 *
 * The caller must already have a reference.
 */
static inline void css_get(struct cgroup_subsys_state *css)
{
	/* We don't need to reference count the root state */
	if (!(css->flags & CSS_ROOT))
		percpu_ref_get(&css->refcnt);
}

/**
 * css_tryget - try to obtain a reference on the specified css
 * @css: target css
 *
 * Obtain a reference on @css if it's alive.  The caller naturally needs to
 * ensure that @css is accessible but doesn't have to be holding a
 * reference on it - IOW, RCU protected access is good enough for this
 * function.  Returns %true if a reference count was successfully obtained;
 * %false otherwise.
 */
static inline bool css_tryget(struct cgroup_subsys_state *css)
{
	if (css->flags & CSS_ROOT)
		return true;
	return percpu_ref_tryget(&css->refcnt);
}

/**
 * css_put - put a css reference
 * @css: target css
 *
 * Put a reference obtained via css_get() and css_tryget().
 */
static inline void css_put(struct cgroup_subsys_state *css)
{
	if (!(css->flags & CSS_ROOT))
		percpu_ref_put(&css->refcnt);
>>>>>>> refs/remotes/origin/master
}

/* bits in struct cgroup flags field */
enum {
	/* Control Group is dead */
<<<<<<< HEAD
	CGRP_REMOVED,
	/* Control Group has ever had a child cgroup or a task */
<<<<<<< HEAD
=======
	CGRP_DEAD,
	/*
	 * Control Group has previously had a child cgroup or a task,
	 * but no longer (only if CGRP_NOTIFY_ON_RELEASE is set)
	 */
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	CGRP_RELEASABLE,
	/* Control Group requires release notifications to userspace */
	CGRP_NOTIFY_ON_RELEASE,
	/*
<<<<<<< HEAD
	 * A thread in rmdir() is wating for this cgroup.
	 */
	CGRP_WAIT_ON_RMDIR,
	/*
	 * Clone cgroup values when creating a new child cgroup
	 */
	CGRP_CLONE_CHILDREN,
};

<<<<<<< HEAD
/* which pidlist file are we talking about? */
enum cgroup_filetype {
	CGROUP_FILE_PROCS,
	CGROUP_FILE_TASKS,
};

/*
 * A pidlist is a list of pids that virtually represents the contents of one
 * of the cgroup files ("procs" or "tasks"). We keep a list of such pidlists,
 * a pair (one each for procs, tasks) for each pid namespace that's relevant
 * to the cgroup.
 */
struct cgroup_pidlist {
	/*
	 * used to find which pidlist is wanted. doesn't change as long as
	 * this particular list stays in the list.
	 */
	struct { enum cgroup_filetype type; struct pid_namespace *ns; } key;
	/* array of xids */
	pid_t *list;
	/* how many elements the above list has */
	int length;
	/* how many files are using the current array */
	int use_count;
	/* each of these stored in a list by its cgroup */
	struct list_head links;
	/* pointer to the cgroup we belong to, for list removal purposes */
	struct cgroup *owner;
	/* protects the other fields */
	struct rw_semaphore mutex;
};

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	 * Clone the parent's configuration when creating a new child
	 * cpuset cgroup.  For historical reasons, this option can be
	 * specified at mount time and thus is implemented here.
	 */
	CGRP_CPUSET_CLONE_CHILDREN,
	/* see the comment above CGRP_ROOT_SANE_BEHAVIOR for details */
	CGRP_SANE_BEHAVIOR,
};

struct cgroup_name {
	struct rcu_head rcu_head;
	char name[];
};

>>>>>>> refs/remotes/origin/master
struct cgroup {
	unsigned long flags;		/* "unsigned long" so bitops work */

	/*
<<<<<<< HEAD
	 * count users of this cgroup. >0 means busy, but doesn't
	 * necessarily indicate the number of tasks in the cgroup
	 */
	atomic_t count;
=======
	 * idr allocated in-hierarchy ID.
	 *
	 * The ID of the root cgroup is always 0, and a new cgroup
	 * will be assigned with a smallest available ID.
	 */
	int id;

	/* the number of attached css's */
	int nr_css;
>>>>>>> refs/remotes/origin/master

	/*
	 * We link our 'sibling' struct into our parent's 'children'.
	 * Our children link their 'sibling' into our 'children'.
	 */
	struct list_head sibling;	/* my parent's children */
	struct list_head children;	/* my children */
<<<<<<< HEAD

	struct cgroup *parent;		/* my parent */
	struct dentry __rcu *dentry;	/* cgroup fs entry, RCU protected */

	/* Private pointers for each registered subsystem */
	struct cgroup_subsys_state *subsys[CGROUP_SUBSYS_COUNT];

	struct cgroupfs_root *root;
	struct cgroup *top_cgroup;

	/*
	 * List of cg_cgroup_links pointing at css_sets with
	 * tasks in this cgroup. Protected by css_set_lock
	 */
	struct list_head css_sets;
=======
	struct list_head files;		/* my files */

	struct cgroup *parent;		/* my parent */
	struct dentry *dentry;		/* cgroup fs entry, RCU protected */

	/*
	 * Monotonically increasing unique serial number which defines a
	 * uniform order among all cgroups.  It's guaranteed that all
	 * ->children lists are in the ascending order of ->serial_nr.
	 * It's used to allow interrupting and resuming iterations.
	 */
	u64 serial_nr;

	/*
	 * This is a copy of dentry->d_name, and it's needed because
	 * we can't use dentry->d_name in cgroup_path().
	 *
	 * You must acquire rcu_read_lock() to access cgrp->name, and
	 * the only place that can change it is rename(), which is
	 * protected by parent dir's i_mutex.
	 *
	 * Normally you should use cgroup_name() wrapper rather than
	 * access it directly.
	 */
	struct cgroup_name __rcu *name;

	/* Private pointers for each registered subsystem */
	struct cgroup_subsys_state __rcu *subsys[CGROUP_SUBSYS_COUNT];

	struct cgroupfs_root *root;

	/*
	 * List of cgrp_cset_links pointing at css_sets with tasks in this
	 * cgroup.  Protected by css_set_lock.
	 */
	struct list_head cset_links;
>>>>>>> refs/remotes/origin/master

	/*
	 * Linked list running through all cgroups that can
	 * potentially be reaped by the release agent. Protected by
	 * release_list_lock
	 */
	struct list_head release_list;

	/*
	 * list of pidlists, up to two for each namespace (one for procs, one
	 * for tasks); created on demand.
	 */
	struct list_head pidlists;
	struct mutex pidlist_mutex;

<<<<<<< HEAD
	/* For RCU-protected deletion */
	struct rcu_head rcu_head;

	/* List of events which userspace want to receive */
	struct list_head event_list;
	spinlock_t event_list_lock;
=======
	/* dummy css with NULL ->ss, points back to this cgroup */
	struct cgroup_subsys_state dummy_css;

	/* For css percpu_ref killing and RCU-protected deletion */
	struct rcu_head rcu_head;
	struct work_struct destroy_work;

	/* directory xattrs */
	struct simple_xattrs xattrs;
};

#define MAX_CGROUP_ROOT_NAMELEN 64

/* cgroupfs_root->flags */
enum {
	/*
	 * Unfortunately, cgroup core and various controllers are riddled
	 * with idiosyncrasies and pointless options.  The following flag,
	 * when set, will force sane behavior - some options are forced on,
	 * others are disallowed, and some controllers will change their
	 * hierarchical or other behaviors.
	 *
	 * The set of behaviors affected by this flag are still being
	 * determined and developed and the mount option for this flag is
	 * prefixed with __DEVEL__.  The prefix will be dropped once we
	 * reach the point where all behaviors are compatible with the
	 * planned unified hierarchy, which will automatically turn on this
	 * flag.
	 *
	 * The followings are the behaviors currently affected this flag.
	 *
	 * - Mount options "noprefix" and "clone_children" are disallowed.
	 *   Also, cgroupfs file cgroup.clone_children is not created.
	 *
	 * - When mounting an existing superblock, mount options should
	 *   match.
	 *
	 * - Remount is disallowed.
	 *
	 * - rename(2) is disallowed.
	 *
	 * - "tasks" is removed.  Everything should be at process
	 *   granularity.  Use "cgroup.procs" instead.
	 *
	 * - "cgroup.procs" is not sorted.  pids will be unique unless they
	 *   got recycled inbetween reads.
	 *
	 * - "release_agent" and "notify_on_release" are removed.
	 *   Replacement notification mechanism will be implemented.
	 *
	 * - cpuset: tasks will be kept in empty cpusets when hotplug happens
	 *   and take masks of ancestors with non-empty cpus/mems, instead of
	 *   being moved to an ancestor.
	 *
	 * - cpuset: a task can be moved into an empty cpuset, and again it
	 *   takes masks of ancestors.
	 *
	 * - memcg: use_hierarchy is on by default and the cgroup file for
	 *   the flag is not created.
	 *
	 * - blkcg: blk-throttle becomes properly hierarchical.
	 */
	CGRP_ROOT_SANE_BEHAVIOR	= (1 << 0),

	CGRP_ROOT_NOPREFIX	= (1 << 1), /* mounted subsystems have no named prefix */
	CGRP_ROOT_XATTR		= (1 << 2), /* supports extended attributes */

	/* mount options live below bit 16 */
	CGRP_ROOT_OPTION_MASK	= (1 << 16) - 1,

	CGRP_ROOT_SUBSYS_BOUND	= (1 << 16), /* subsystems finished binding */
};

/*
 * A cgroupfs_root represents the root of a cgroup hierarchy, and may be
 * associated with a superblock to form an active hierarchy.  This is
 * internal to cgroup core.  Don't access directly from controllers.
 */
struct cgroupfs_root {
	struct super_block *sb;

	/* The bitmask of subsystems attached to this hierarchy */
	unsigned long subsys_mask;

	/* Unique id for this hierarchy. */
	int hierarchy_id;

	/* The root cgroup for this hierarchy */
	struct cgroup top_cgroup;

	/* Tracks how many cgroups are currently defined in hierarchy.*/
	int number_of_cgroups;

	/* A list running through the active hierarchies */
	struct list_head root_list;

	/* Hierarchy-specific flags */
	unsigned long flags;

	/* IDs for cgroups in this hierarchy */
	struct idr cgroup_idr;

	/* The path to use for release notifications. */
	char release_agent_path[PATH_MAX];

	/* The name for this hierarchy - may be empty */
	char name[MAX_CGROUP_ROOT_NAMELEN];
>>>>>>> refs/remotes/origin/master
};

/*
 * A css_set is a structure holding pointers to a set of
 * cgroup_subsys_state objects. This saves space in the task struct
 * object and speeds up fork()/exit(), since a single inc/dec and a
 * list_add()/del() can bump the reference count on the entire cgroup
 * set for a task.
 */

struct css_set {

	/* Reference count */
	atomic_t refcount;

	/*
	 * List running through all cgroup groups in the same hash
	 * slot. Protected by css_set_lock
	 */
	struct hlist_node hlist;

	/*
	 * List running through all tasks using this cgroup
	 * group. Protected by css_set_lock
	 */
	struct list_head tasks;

	/*
<<<<<<< HEAD
	 * List of cg_cgroup_link objects on link chains from
	 * cgroups referenced from this css_set. Protected by
	 * css_set_lock
	 */
	struct list_head cg_links;
=======
	 * List of cgrp_cset_links pointing at cgroups referenced from this
	 * css_set.  Protected by css_set_lock.
	 */
	struct list_head cgrp_links;
>>>>>>> refs/remotes/origin/master

	/*
	 * Set of subsystem states, one for each subsystem. This array
	 * is immutable after creation apart from the init_css_set
	 * during subsystem registration (at boot time) and modular subsystem
	 * loading/unloading.
	 */
	struct cgroup_subsys_state *subsys[CGROUP_SUBSYS_COUNT];

	/* For RCU-protected deletion */
	struct rcu_head rcu_head;
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	struct work_struct work;
};

/*
 * cgroup_map_cb is an abstract callback API for reporting map-valued
 * control files
 */

struct cgroup_map_cb {
	int (*fill)(struct cgroup_map_cb *cb, const char *key, u64 value);
	void *state;
=======
>>>>>>> refs/remotes/origin/master
};

/*
 * struct cftype: handler definitions for cgroup control files
 *
 * When reading/writing to a file:
 *	- the cgroup to use is file->f_dentry->d_parent->d_fsdata
 *	- the 'cftype' of the file is file->f_dentry->d_fsdata
 */

<<<<<<< HEAD
#define MAX_CFTYPE_NAME 64
struct cftype {
	/*
	 * By convention, the name should begin with the name of the
	 * subsystem, followed by a period
=======
/* cftype->flags */
enum {
	CFTYPE_ONLY_ON_ROOT	= (1 << 0),	/* only create on root cgrp */
	CFTYPE_NOT_ON_ROOT	= (1 << 1),	/* don't create on root cgrp */
	CFTYPE_INSANE		= (1 << 2),	/* don't create if sane_behavior */
	CFTYPE_NO_PREFIX	= (1 << 3),	/* (DON'T USE FOR NEW FILES) no subsys prefix */
};

#define MAX_CFTYPE_NAME		64

struct cftype {
	/*
	 * By convention, the name should begin with the name of the
	 * subsystem, followed by a period.  Zero length string indicates
	 * end of cftype array.
>>>>>>> refs/remotes/origin/master
	 */
	char name[MAX_CFTYPE_NAME];
	int private;
	/*
	 * If not 0, file mode is set to this value, otherwise it will
	 * be figured out automatically
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	mode_t mode;
=======
	umode_t mode;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	umode_t mode;
>>>>>>> refs/remotes/origin/master

	/*
	 * If non-zero, defines the maximum length of string that can
	 * be passed to write_string; defaults to 64
	 */
	size_t max_write_len;

<<<<<<< HEAD
	int (*open)(struct inode *inode, struct file *file);
	ssize_t (*read)(struct cgroup *cgrp, struct cftype *cft,
			struct file *file,
			char __user *buf, size_t nbytes, loff_t *ppos);
=======
	/* CFTYPE_* flags */
	unsigned int flags;

	/*
	 * The subsys this file belongs to.  Initialized automatically
	 * during registration.  NULL for cgroup core files.
	 */
	struct cgroup_subsys *ss;

>>>>>>> refs/remotes/origin/master
	/*
	 * read_u64() is a shortcut for the common case of returning a
	 * single integer. Use it in place of read()
	 */
<<<<<<< HEAD
	u64 (*read_u64)(struct cgroup *cgrp, struct cftype *cft);
	/*
	 * read_s64() is a signed version of read_u64()
	 */
	s64 (*read_s64)(struct cgroup *cgrp, struct cftype *cft);
	/*
	 * read_map() is used for defining a map of key/value
	 * pairs. It should call cb->fill(cb, key, value) for each
	 * entry. The key/value pairs (and their ordering) should not
	 * change between reboots.
	 */
	int (*read_map)(struct cgroup *cont, struct cftype *cft,
			struct cgroup_map_cb *cb);
	/*
	 * read_seq_string() is used for outputting a simple sequence
	 * using seqfile.
	 */
	int (*read_seq_string)(struct cgroup *cont, struct cftype *cft,
			       struct seq_file *m);

	ssize_t (*write)(struct cgroup *cgrp, struct cftype *cft,
			 struct file *file,
			 const char __user *buf, size_t nbytes, loff_t *ppos);
=======
	u64 (*read_u64)(struct cgroup_subsys_state *css, struct cftype *cft);
	/*
	 * read_s64() is a signed version of read_u64()
	 */
	s64 (*read_s64)(struct cgroup_subsys_state *css, struct cftype *cft);

	/* generic seq_file read interface */
	int (*seq_show)(struct seq_file *sf, void *v);

	/* optional ops, implement all or none */
	void *(*seq_start)(struct seq_file *sf, loff_t *ppos);
	void *(*seq_next)(struct seq_file *sf, void *v, loff_t *ppos);
	void (*seq_stop)(struct seq_file *sf, void *v);
>>>>>>> refs/remotes/origin/master

	/*
	 * write_u64() is a shortcut for the common case of accepting
	 * a single integer (as parsed by simple_strtoull) from
	 * userspace. Use in place of write(); return 0 or error.
	 */
<<<<<<< HEAD
	int (*write_u64)(struct cgroup *cgrp, struct cftype *cft, u64 val);
	/*
	 * write_s64() is a signed version of write_u64()
	 */
	int (*write_s64)(struct cgroup *cgrp, struct cftype *cft, s64 val);
=======
	int (*write_u64)(struct cgroup_subsys_state *css, struct cftype *cft,
			 u64 val);
	/*
	 * write_s64() is a signed version of write_u64()
	 */
	int (*write_s64)(struct cgroup_subsys_state *css, struct cftype *cft,
			 s64 val);
>>>>>>> refs/remotes/origin/master

	/*
	 * write_string() is passed a nul-terminated kernelspace
	 * buffer of maximum length determined by max_write_len.
	 * Returns 0 or -ve error code.
	 */
<<<<<<< HEAD
	int (*write_string)(struct cgroup *cgrp, struct cftype *cft,
=======
	int (*write_string)(struct cgroup_subsys_state *css, struct cftype *cft,
>>>>>>> refs/remotes/origin/master
			    const char *buffer);
	/*
	 * trigger() callback can be used to get some kick from the
	 * userspace, when the actual string written is not important
	 * at all. The private field can be used to determine the
	 * kick type for multiplexing.
	 */
<<<<<<< HEAD
	int (*trigger)(struct cgroup *cgrp, unsigned int event);

	int (*release)(struct inode *inode, struct file *file);

	/*
	 * register_event() callback will be used to add new userspace
	 * waiter for changes related to the cftype. Implement it if
	 * you want to provide this functionality. Use eventfd_signal()
	 * on eventfd to send notification to userspace.
	 */
	int (*register_event)(struct cgroup *cgrp, struct cftype *cft,
			struct eventfd_ctx *eventfd, const char *args);
	/*
	 * unregister_event() callback will be called when userspace
	 * closes the eventfd or on cgroup removing.
	 * This callback must be implemented, if you want provide
	 * notification functionality.
	 */
	void (*unregister_event)(struct cgroup *cgrp, struct cftype *cft,
			struct eventfd_ctx *eventfd);
};

struct cgroup_scanner {
	struct cgroup *cg;
	int (*test_task)(struct task_struct *p, struct cgroup_scanner *scan);
	void (*process_task)(struct task_struct *p,
			struct cgroup_scanner *scan);
	struct ptr_heap *heap;
	void *data;
};

/*
 * Add a new file to the given cgroup directory. Should only be
 * called by subsystems from within a populate() method
 */
int cgroup_add_file(struct cgroup *cgrp, struct cgroup_subsys *subsys,
		       const struct cftype *cft);

/*
 * Add a set of new files to the given cgroup directory. Should
 * only be called by subsystems from within a populate() method
 */
int cgroup_add_files(struct cgroup *cgrp,
			struct cgroup_subsys *subsys,
			const struct cftype cft[],
			int count);

int cgroup_is_removed(const struct cgroup *cgrp);

int cgroup_path(const struct cgroup *cgrp, char *buf, int buflen);

int cgroup_task_count(const struct cgroup *cgrp);

/* Return true if cgrp is a descendant of the task's cgroup */
int cgroup_is_descendant(const struct cgroup *cgrp, struct task_struct *task);

/*
 * When the subsys has to access css and may add permanent refcnt to css,
 * it should take care of racy conditions with rmdir(). Following set of
 * functions, is for stop/restart rmdir if necessary.
 * Because these will call css_get/put, "css" should be alive css.
 *
 *  cgroup_exclude_rmdir();
 *  ...do some jobs which may access arbitrary empty cgroup
 *  cgroup_release_and_wakeup_rmdir();
 *
 *  When someone removes a cgroup while cgroup_exclude_rmdir() holds it,
 *  it sleeps and cgroup_release_and_wakeup_rmdir() will wake him up.
 */

void cgroup_exclude_rmdir(struct cgroup_subsys_state *css);
void cgroup_release_and_wakeup_rmdir(struct cgroup_subsys_state *css);

/*
<<<<<<< HEAD
=======
=======
	int (*trigger)(struct cgroup_subsys_state *css, unsigned int event);
};

/*
 * cftype_sets describe cftypes belonging to a subsystem and are chained at
 * cgroup_subsys->cftsets.  Each cftset points to an array of cftypes
 * terminated by zero length name.
 */
struct cftype_set {
	struct list_head		node;	/* chained at subsys->cftsets */
	struct cftype			*cfts;
};

/*
 * cgroupfs file entry, pointed to from leaf dentry->d_fsdata.  Don't
 * access directly.
 */
struct cfent {
	struct list_head		node;
	struct dentry			*dentry;
	struct cftype			*type;
	struct cgroup_subsys_state	*css;

	/* file xattrs */
	struct simple_xattrs		xattrs;
};

/* seq_file->private points to the following, only ->priv is public */
struct cgroup_open_file {
	struct cfent			*cfe;
	void				*priv;
};

/*
 * See the comment above CGRP_ROOT_SANE_BEHAVIOR for details.  This
 * function can be called as long as @cgrp is accessible.
 */
static inline bool cgroup_sane_behavior(const struct cgroup *cgrp)
{
	return cgrp->root->flags & CGRP_ROOT_SANE_BEHAVIOR;
}

/* Caller should hold rcu_read_lock() */
static inline const char *cgroup_name(const struct cgroup *cgrp)
{
	return rcu_dereference(cgrp->name)->name;
}

static inline struct cgroup_subsys_state *seq_css(struct seq_file *seq)
{
	struct cgroup_open_file *of = seq->private;
	return of->cfe->css;
}

static inline struct cftype *seq_cft(struct seq_file *seq)
{
	struct cgroup_open_file *of = seq->private;
	return of->cfe->type;
}

int cgroup_add_cftypes(struct cgroup_subsys *ss, struct cftype *cfts);
int cgroup_rm_cftypes(struct cftype *cfts);

bool cgroup_is_descendant(struct cgroup *cgrp, struct cgroup *ancestor);

int cgroup_path(const struct cgroup *cgrp, char *buf, int buflen);
int task_cgroup_path(struct task_struct *task, char *buf, size_t buflen);

int cgroup_task_count(const struct cgroup *cgrp);

/*
>>>>>>> refs/remotes/origin/master
 * Control Group taskset, used to pass around set of tasks to cgroup_subsys
 * methods.
 */
struct cgroup_taskset;
struct task_struct *cgroup_taskset_first(struct cgroup_taskset *tset);
struct task_struct *cgroup_taskset_next(struct cgroup_taskset *tset);
<<<<<<< HEAD
struct cgroup *cgroup_taskset_cur_cgroup(struct cgroup_taskset *tset);
=======
struct cgroup_subsys_state *cgroup_taskset_cur_css(struct cgroup_taskset *tset,
						   int subsys_id);
>>>>>>> refs/remotes/origin/master
int cgroup_taskset_size(struct cgroup_taskset *tset);

/**
 * cgroup_taskset_for_each - iterate cgroup_taskset
 * @task: the loop cursor
<<<<<<< HEAD
 * @skip_cgrp: skip if task's cgroup matches this, %NULL to iterate through all
 * @tset: taskset to iterate
 */
#define cgroup_taskset_for_each(task, skip_cgrp, tset)			\
	for ((task) = cgroup_taskset_first((tset)); (task);		\
	     (task) = cgroup_taskset_next((tset)))			\
		if (!(skip_cgrp) ||					\
		    cgroup_taskset_cur_cgroup((tset)) != (skip_cgrp))

/*
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * @skip_css: skip if task's css matches this, %NULL to iterate through all
 * @tset: taskset to iterate
 */
#define cgroup_taskset_for_each(task, skip_css, tset)			\
	for ((task) = cgroup_taskset_first((tset)); (task);		\
	     (task) = cgroup_taskset_next((tset)))			\
		if (!(skip_css) ||					\
		    cgroup_taskset_cur_css((tset),			\
			(skip_css)->ss->subsys_id) != (skip_css))

/*
>>>>>>> refs/remotes/origin/master
 * Control Group subsystem type.
 * See Documentation/cgroups/cgroups.txt for details
 */

struct cgroup_subsys {
<<<<<<< HEAD
<<<<<<< HEAD
	struct cgroup_subsys_state *(*create)(struct cgroup_subsys *ss,
						  struct cgroup *cgrp);
	int (*pre_destroy)(struct cgroup_subsys *ss, struct cgroup *cgrp);
	void (*destroy)(struct cgroup_subsys *ss, struct cgroup *cgrp);
	int (*allow_attach)(struct cgroup *cgrp, struct task_struct *tsk);
	int (*can_attach)(struct cgroup_subsys *ss, struct cgroup *cgrp,
			  struct task_struct *tsk);
	int (*can_attach_task)(struct cgroup *cgrp, struct task_struct *tsk);
	void (*cancel_attach)(struct cgroup_subsys *ss, struct cgroup *cgrp,
			      struct task_struct *tsk);
	void (*pre_attach)(struct cgroup *cgrp);
	void (*attach_task)(struct cgroup *cgrp, struct task_struct *tsk);
	void (*attach)(struct cgroup_subsys *ss, struct cgroup *cgrp,
		       struct cgroup *old_cgrp, struct task_struct *tsk);
	void (*fork)(struct cgroup_subsys *ss, struct task_struct *task);
	void (*exit)(struct cgroup_subsys *ss, struct cgroup *cgrp,
			struct cgroup *old_cgrp, struct task_struct *task);
	int (*populate)(struct cgroup_subsys *ss,
			struct cgroup *cgrp);
	void (*post_clone)(struct cgroup_subsys *ss, struct cgroup *cgrp);
	void (*bind)(struct cgroup_subsys *ss, struct cgroup *root);
=======
	struct cgroup_subsys_state *(*create)(struct cgroup *cgrp);
	int (*pre_destroy)(struct cgroup *cgrp);
	void (*destroy)(struct cgroup *cgrp);
	int (*allow_attach)(struct cgroup *cgrp, struct cgroup_taskset *tset);
	int (*can_attach)(struct cgroup *cgrp, struct cgroup_taskset *tset);
	void (*cancel_attach)(struct cgroup *cgrp, struct cgroup_taskset *tset);
	void (*attach)(struct cgroup *cgrp, struct cgroup_taskset *tset);
	void (*fork)(struct task_struct *task);
	void (*exit)(struct cgroup *cgrp, struct cgroup *old_cgrp,
		     struct task_struct *task);
	int (*populate)(struct cgroup_subsys *ss, struct cgroup *cgrp);
	void (*post_clone)(struct cgroup *cgrp);
	void (*bind)(struct cgroup *root);
>>>>>>> refs/remotes/origin/cm-10.0

	int subsys_id;
	int active;
	int disabled;
	int early_init;
	/*
	 * True if this subsys uses ID. ID is not available before cgroup_init()
	 * (not available in early_init time.)
	 */
	bool use_id;
#define MAX_CGROUP_TYPE_NAMELEN 32
	const char *name;

	/*
	 * Protects sibling/children links of cgroups in this
	 * hierarchy, plus protects which hierarchy (or none) the
	 * subsystem is a part of (i.e. root/sibling).  To avoid
	 * potential deadlocks, the following operations should not be
	 * undertaken while holding any hierarchy_mutex:
	 *
	 * - allocating memory
	 * - initiating hotplug events
	 */
	struct mutex hierarchy_mutex;
	struct lock_class_key subsys_key;

	/*
	 * Link to parent, and list entry in parent's children.
	 * Protected by this->hierarchy_mutex and cgroup_lock()
	 */
	struct cgroupfs_root *root;
	struct list_head sibling;
	/* used when use_id == true */
	struct idr idr;
	spinlock_t id_lock;
=======
	struct cgroup_subsys_state *(*css_alloc)(struct cgroup_subsys_state *parent_css);
	int (*css_online)(struct cgroup_subsys_state *css);
	void (*css_offline)(struct cgroup_subsys_state *css);
	void (*css_free)(struct cgroup_subsys_state *css);

	int (*can_attach)(struct cgroup_subsys_state *css,
			  struct cgroup_taskset *tset);
	void (*cancel_attach)(struct cgroup_subsys_state *css,
			      struct cgroup_taskset *tset);
	void (*attach)(struct cgroup_subsys_state *css,
		       struct cgroup_taskset *tset);
	void (*fork)(struct task_struct *task);
	void (*exit)(struct cgroup_subsys_state *css,
		     struct cgroup_subsys_state *old_css,
		     struct task_struct *task);
	void (*bind)(struct cgroup_subsys_state *root_css);

	int subsys_id;
	int disabled;
	int early_init;

	/*
	 * If %false, this subsystem is properly hierarchical -
	 * configuration, resource accounting and restriction on a parent
	 * cgroup cover those of its children.  If %true, hierarchy support
	 * is broken in some ways - some subsystems ignore hierarchy
	 * completely while others are only implemented half-way.
	 *
	 * It's now disallowed to create nested cgroups if the subsystem is
	 * broken and cgroup core will emit a warning message on such
	 * cases.  Eventually, all subsystems will be made properly
	 * hierarchical and this will go away.
	 */
	bool broken_hierarchy;
	bool warned_broken_hierarchy;

#define MAX_CGROUP_TYPE_NAMELEN 32
	const char *name;

	/* link to parent, protected by cgroup_lock() */
	struct cgroupfs_root *root;

	/* list of cftype_sets */
	struct list_head cftsets;

	/* base cftypes, automatically [de]registered with subsys itself */
	struct cftype *base_cftypes;
	struct cftype_set base_cftset;
>>>>>>> refs/remotes/origin/master

	/* should be defined only by modular subsystems */
	struct module *module;
};

#define SUBSYS(_x) extern struct cgroup_subsys _x ## _subsys;
<<<<<<< HEAD
#include <linux/cgroup_subsys.h>
#undef SUBSYS

static inline struct cgroup_subsys_state *cgroup_subsys_state(
	struct cgroup *cgrp, int subsys_id)
{
	return cgrp->subsys[subsys_id];
}

/*
 * function to get the cgroup_subsys_state which allows for extra
 * rcu_dereference_check() conditions, such as locks used during the
 * cgroup_subsys::attach() methods.
 */
#define task_subsys_state_check(task, subsys_id, __c)			\
	rcu_dereference_check(task->cgroups->subsys[subsys_id],		\
<<<<<<< HEAD
			      rcu_read_lock_held() ||			\
=======
>>>>>>> refs/remotes/origin/cm-10.0
			      lockdep_is_held(&task->alloc_lock) ||	\
			      cgroup_lock_is_held() || (__c))

static inline struct cgroup_subsys_state *
task_subsys_state(struct task_struct *task, int subsys_id)
{
	return task_subsys_state_check(task, subsys_id, false);
}

static inline struct cgroup* task_cgroup(struct task_struct *task,
					       int subsys_id)
{
	return task_subsys_state(task, subsys_id)->cgroup;
}

/* A cgroup_iter should be treated as an opaque object */
struct cgroup_iter {
	struct list_head *cg_link;
	struct list_head *task;
};

/*
 * To iterate across the tasks in a cgroup:
 *
 * 1) call cgroup_iter_start to initialize an iterator
 *
 * 2) call cgroup_iter_next() to retrieve member tasks until it
 *    returns NULL or until you want to end the iteration
 *
 * 3) call cgroup_iter_end() to destroy the iterator.
 *
 * Or, call cgroup_scan_tasks() to iterate through every task in a
 * cgroup - cgroup_scan_tasks() holds the css_set_lock when calling
 * the test_task() callback, but not while calling the process_task()
 * callback.
 */
void cgroup_iter_start(struct cgroup *cgrp, struct cgroup_iter *it);
struct task_struct *cgroup_iter_next(struct cgroup *cgrp,
					struct cgroup_iter *it);
void cgroup_iter_end(struct cgroup *cgrp, struct cgroup_iter *it);
int cgroup_scan_tasks(struct cgroup_scanner *scan);
int cgroup_attach_task(struct cgroup *, struct task_struct *);
int cgroup_attach_task_all(struct task_struct *from, struct task_struct *);

<<<<<<< HEAD
static inline int cgroup_attach_task_current_cg(struct task_struct *tsk)
{
	return cgroup_attach_task_all(current, tsk);
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
/*
 * CSS ID is ID for cgroup_subsys_state structs under subsys. This only works
 * if cgroup_subsys.use_id == true. It can be used for looking up and scanning.
 * CSS ID is assigned at cgroup allocation (create) automatically
 * and removed when subsys calls free_css_id() function. This is because
 * the lifetime of cgroup_subsys_state is subsys's matter.
 *
 * Looking up and scanning function should be called under rcu_read_lock().
 * Taking cgroup_mutex()/hierarchy_mutex() is not necessary for following calls.
 * But the css returned by this routine can be "not populated yet" or "being
 * destroyed". The caller should check css and cgroup's status.
 */

/*
 * Typically Called at ->destroy(), or somewhere the subsys frees
 * cgroup_subsys_state.
 */
void free_css_id(struct cgroup_subsys *ss, struct cgroup_subsys_state *css);

/* Find a cgroup_subsys_state which has given ID */

struct cgroup_subsys_state *css_lookup(struct cgroup_subsys *ss, int id);

/*
 * Get a cgroup whose id is greater than or equal to id under tree of root.
 * Returning a cgroup_subsys_state or NULL.
 */
struct cgroup_subsys_state *css_get_next(struct cgroup_subsys *ss, int id,
		struct cgroup_subsys_state *root, int *foundid);

/* Returns true if root is ancestor of cg */
bool css_is_ancestor(struct cgroup_subsys_state *cg,
		     const struct cgroup_subsys_state *root);

/* Get id and depth of css */
unsigned short css_id(struct cgroup_subsys_state *css);
unsigned short css_depth(struct cgroup_subsys_state *css);
struct cgroup_subsys_state *cgroup_css_from_dir(struct file *f, int id);
=======
#define IS_SUBSYS_ENABLED(option) IS_BUILTIN(option)
#include <linux/cgroup_subsys.h>
#undef IS_SUBSYS_ENABLED
#undef SUBSYS

/**
 * css_parent - find the parent css
 * @css: the target cgroup_subsys_state
 *
 * Return the parent css of @css.  This function is guaranteed to return
 * non-NULL parent as long as @css isn't the root.
 */
static inline
struct cgroup_subsys_state *css_parent(struct cgroup_subsys_state *css)
{
	return css->parent;
}

/**
 * task_css_set_check - obtain a task's css_set with extra access conditions
 * @task: the task to obtain css_set for
 * @__c: extra condition expression to be passed to rcu_dereference_check()
 *
 * A task's css_set is RCU protected, initialized and exited while holding
 * task_lock(), and can only be modified while holding both cgroup_mutex
 * and task_lock() while the task is alive.  This macro verifies that the
 * caller is inside proper critical section and returns @task's css_set.
 *
 * The caller can also specify additional allowed conditions via @__c, such
 * as locks used during the cgroup_subsys::attach() methods.
 */
#ifdef CONFIG_PROVE_RCU
extern struct mutex cgroup_mutex;
#define task_css_set_check(task, __c)					\
	rcu_dereference_check((task)->cgroups,				\
		lockdep_is_held(&(task)->alloc_lock) ||			\
		lockdep_is_held(&cgroup_mutex) || (__c))
#else
#define task_css_set_check(task, __c)					\
	rcu_dereference((task)->cgroups)
#endif

/**
 * task_css_check - obtain css for (task, subsys) w/ extra access conds
 * @task: the target task
 * @subsys_id: the target subsystem ID
 * @__c: extra condition expression to be passed to rcu_dereference_check()
 *
 * Return the cgroup_subsys_state for the (@task, @subsys_id) pair.  The
 * synchronization rules are the same as task_css_set_check().
 */
#define task_css_check(task, subsys_id, __c)				\
	task_css_set_check((task), (__c))->subsys[(subsys_id)]

/**
 * task_css_set - obtain a task's css_set
 * @task: the task to obtain css_set for
 *
 * See task_css_set_check().
 */
static inline struct css_set *task_css_set(struct task_struct *task)
{
	return task_css_set_check(task, false);
}

/**
 * task_css - obtain css for (task, subsys)
 * @task: the target task
 * @subsys_id: the target subsystem ID
 *
 * See task_css_check().
 */
static inline struct cgroup_subsys_state *task_css(struct task_struct *task,
						   int subsys_id)
{
	return task_css_check(task, subsys_id, false);
}

static inline struct cgroup *task_cgroup(struct task_struct *task,
					 int subsys_id)
{
	return task_css(task, subsys_id)->cgroup;
}

struct cgroup_subsys_state *css_next_child(struct cgroup_subsys_state *pos,
					   struct cgroup_subsys_state *parent);

struct cgroup_subsys_state *css_from_id(int id, struct cgroup_subsys *ss);

/**
 * css_for_each_child - iterate through children of a css
 * @pos: the css * to use as the loop cursor
 * @parent: css whose children to walk
 *
 * Walk @parent's children.  Must be called under rcu_read_lock().  A child
 * css which hasn't finished ->css_online() or already has finished
 * ->css_offline() may show up during traversal and it's each subsystem's
 * responsibility to verify that each @pos is alive.
 *
 * If a subsystem synchronizes against the parent in its ->css_online() and
 * before starting iterating, a css which finished ->css_online() is
 * guaranteed to be visible in the future iterations.
 *
 * It is allowed to temporarily drop RCU read lock during iteration.  The
 * caller is responsible for ensuring that @pos remains accessible until
 * the start of the next iteration by, for example, bumping the css refcnt.
 */
#define css_for_each_child(pos, parent)					\
	for ((pos) = css_next_child(NULL, (parent)); (pos);		\
	     (pos) = css_next_child((pos), (parent)))

struct cgroup_subsys_state *
css_next_descendant_pre(struct cgroup_subsys_state *pos,
			struct cgroup_subsys_state *css);

struct cgroup_subsys_state *
css_rightmost_descendant(struct cgroup_subsys_state *pos);

/**
 * css_for_each_descendant_pre - pre-order walk of a css's descendants
 * @pos: the css * to use as the loop cursor
 * @root: css whose descendants to walk
 *
 * Walk @root's descendants.  @root is included in the iteration and the
 * first node to be visited.  Must be called under rcu_read_lock().  A
 * descendant css which hasn't finished ->css_online() or already has
 * finished ->css_offline() may show up during traversal and it's each
 * subsystem's responsibility to verify that each @pos is alive.
 *
 * If a subsystem synchronizes against the parent in its ->css_online() and
 * before starting iterating, and synchronizes against @pos on each
 * iteration, any descendant css which finished ->css_online() is
 * guaranteed to be visible in the future iterations.
 *
 * In other words, the following guarantees that a descendant can't escape
 * state updates of its ancestors.
 *
 * my_online(@css)
 * {
 *	Lock @css's parent and @css;
 *	Inherit state from the parent;
 *	Unlock both.
 * }
 *
 * my_update_state(@css)
 * {
 *	css_for_each_descendant_pre(@pos, @css) {
 *		Lock @pos;
 *		if (@pos == @css)
 *			Update @css's state;
 *		else
 *			Verify @pos is alive and inherit state from its parent;
 *		Unlock @pos;
 *	}
 * }
 *
 * As long as the inheriting step, including checking the parent state, is
 * enclosed inside @pos locking, double-locking the parent isn't necessary
 * while inheriting.  The state update to the parent is guaranteed to be
 * visible by walking order and, as long as inheriting operations to the
 * same @pos are atomic to each other, multiple updates racing each other
 * still result in the correct state.  It's guaranateed that at least one
 * inheritance happens for any css after the latest update to its parent.
 *
 * If checking parent's state requires locking the parent, each inheriting
 * iteration should lock and unlock both @pos->parent and @pos.
 *
 * Alternatively, a subsystem may choose to use a single global lock to
 * synchronize ->css_online() and ->css_offline() against tree-walking
 * operations.
 *
 * It is allowed to temporarily drop RCU read lock during iteration.  The
 * caller is responsible for ensuring that @pos remains accessible until
 * the start of the next iteration by, for example, bumping the css refcnt.
 */
#define css_for_each_descendant_pre(pos, css)				\
	for ((pos) = css_next_descendant_pre(NULL, (css)); (pos);	\
	     (pos) = css_next_descendant_pre((pos), (css)))

struct cgroup_subsys_state *
css_next_descendant_post(struct cgroup_subsys_state *pos,
			 struct cgroup_subsys_state *css);

/**
 * css_for_each_descendant_post - post-order walk of a css's descendants
 * @pos: the css * to use as the loop cursor
 * @css: css whose descendants to walk
 *
 * Similar to css_for_each_descendant_pre() but performs post-order
 * traversal instead.  @root is included in the iteration and the last
 * node to be visited.  Note that the walk visibility guarantee described
 * in pre-order walk doesn't apply the same to post-order walks.
 */
#define css_for_each_descendant_post(pos, css)				\
	for ((pos) = css_next_descendant_post(NULL, (css)); (pos);	\
	     (pos) = css_next_descendant_post((pos), (css)))

/* A css_task_iter should be treated as an opaque object */
struct css_task_iter {
	struct cgroup_subsys_state	*origin_css;
	struct list_head		*cset_link;
	struct list_head		*task;
};

void css_task_iter_start(struct cgroup_subsys_state *css,
			 struct css_task_iter *it);
struct task_struct *css_task_iter_next(struct css_task_iter *it);
void css_task_iter_end(struct css_task_iter *it);

int css_scan_tasks(struct cgroup_subsys_state *css,
		   bool (*test)(struct task_struct *, void *),
		   void (*process)(struct task_struct *, void *),
		   void *data, struct ptr_heap *heap);

int cgroup_attach_task_all(struct task_struct *from, struct task_struct *);
int cgroup_transfer_tasks(struct cgroup *to, struct cgroup *from);

struct cgroup_subsys_state *css_from_dir(struct dentry *dentry,
					 struct cgroup_subsys *ss);
>>>>>>> refs/remotes/origin/master

#else /* !CONFIG_CGROUPS */

static inline int cgroup_init_early(void) { return 0; }
static inline int cgroup_init(void) { return 0; }
static inline void cgroup_fork(struct task_struct *p) {}
<<<<<<< HEAD
static inline void cgroup_fork_callbacks(struct task_struct *p) {}
static inline void cgroup_post_fork(struct task_struct *p) {}
static inline void cgroup_exit(struct task_struct *p, int callbacks) {}

static inline void cgroup_lock(void) {}
static inline void cgroup_unlock(void) {}
=======
static inline void cgroup_post_fork(struct task_struct *p) {}
static inline void cgroup_exit(struct task_struct *p, int callbacks) {}

>>>>>>> refs/remotes/origin/master
static inline int cgroupstats_build(struct cgroupstats *stats,
					struct dentry *dentry)
{
	return -EINVAL;
}

/* No cgroups - nothing to do */
static inline int cgroup_attach_task_all(struct task_struct *from,
					 struct task_struct *t)
{
	return 0;
}
<<<<<<< HEAD
<<<<<<< HEAD
static inline int cgroup_attach_task_current_cg(struct task_struct *t)
{
	return 0;
}
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#endif /* !CONFIG_CGROUPS */

#endif /* _LINUX_CGROUP_H */
