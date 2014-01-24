#ifndef _LINUX_FAULT_INJECT_H
#define _LINUX_FAULT_INJECT_H

#ifdef CONFIG_FAULT_INJECTION

#include <linux/types.h>
#include <linux/debugfs.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/atomic.h>
=======
#include <linux/atomic.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/atomic.h>
>>>>>>> refs/remotes/origin/master

/*
 * For explanation of the elements of this struct, see
 * Documentation/fault-injection/fault-injection.txt
 */
struct fault_attr {
	unsigned long probability;
	unsigned long interval;
	atomic_t times;
	atomic_t space;
	unsigned long verbose;
	u32 task_filter;
	unsigned long stacktrace_depth;
	unsigned long require_start;
	unsigned long require_end;
	unsigned long reject_start;
	unsigned long reject_end;

	unsigned long count;
<<<<<<< HEAD
<<<<<<< HEAD

#ifdef CONFIG_FAULT_INJECTION_DEBUG_FS

	struct {
		struct dentry *dir;

		struct dentry *probability_file;
		struct dentry *interval_file;
		struct dentry *times_file;
		struct dentry *space_file;
		struct dentry *verbose_file;
		struct dentry *task_filter_file;
		struct dentry *stacktrace_depth_file;
		struct dentry *require_start_file;
		struct dentry *require_end_file;
		struct dentry *reject_start_file;
		struct dentry *reject_end_file;
	} dentries;

#endif
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};

#define FAULT_ATTR_INITIALIZER {				\
		.interval = 1,					\
		.times = ATOMIC_INIT(1),			\
		.require_end = ULONG_MAX,			\
		.stacktrace_depth = 32,				\
		.verbose = 2,					\
	}

#define DECLARE_FAULT_ATTR(name) struct fault_attr name = FAULT_ATTR_INITIALIZER
int setup_fault_attr(struct fault_attr *attr, char *str);
<<<<<<< HEAD
<<<<<<< HEAD
void should_fail_srandom(unsigned long entropy);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
bool should_fail(struct fault_attr *attr, ssize_t size);

#ifdef CONFIG_FAULT_INJECTION_DEBUG_FS

<<<<<<< HEAD
<<<<<<< HEAD
int init_fault_attr_dentries(struct fault_attr *attr, const char *name);
void cleanup_fault_attr_dentries(struct fault_attr *attr);

#else /* CONFIG_FAULT_INJECTION_DEBUG_FS */

static inline int init_fault_attr_dentries(struct fault_attr *attr,
					  const char *name)
{
	return -ENODEV;
}

static inline void cleanup_fault_attr_dentries(struct fault_attr *attr)
{
=======
=======
>>>>>>> refs/remotes/origin/master
struct dentry *fault_create_debugfs_attr(const char *name,
			struct dentry *parent, struct fault_attr *attr);

#else /* CONFIG_FAULT_INJECTION_DEBUG_FS */

static inline struct dentry *fault_create_debugfs_attr(const char *name,
			struct dentry *parent, struct fault_attr *attr)
{
	return ERR_PTR(-ENODEV);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

#endif /* CONFIG_FAULT_INJECTION_DEBUG_FS */

#endif /* CONFIG_FAULT_INJECTION */

#ifdef CONFIG_FAILSLAB
extern bool should_failslab(size_t size, gfp_t gfpflags, unsigned long flags);
#else
static inline bool should_failslab(size_t size, gfp_t gfpflags,
				unsigned long flags)
{
	return false;
}
#endif /* CONFIG_FAILSLAB */

#endif /* _LINUX_FAULT_INJECT_H */
