#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/random.h>
#include <linux/sched.h>
#include <linux/stat.h>
#include <linux/types.h>
#include <linux/fs.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/stacktrace.h>
#include <linux/kallsyms.h>
=======
#include <linux/export.h>
#include <linux/interrupt.h>
#include <linux/stacktrace.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/export.h>
#include <linux/interrupt.h>
#include <linux/stacktrace.h>
>>>>>>> refs/remotes/origin/master
#include <linux/fault-inject.h>

/*
 * setup_fault_attr() is a helper function for various __setup handlers, so it
 * returns 0 on error, because that is what __setup handlers do.
 */
<<<<<<< HEAD
<<<<<<< HEAD
int __init setup_fault_attr(struct fault_attr *attr, char *str)
=======
int setup_fault_attr(struct fault_attr *attr, char *str)
>>>>>>> refs/remotes/origin/cm-10.0
=======
int setup_fault_attr(struct fault_attr *attr, char *str)
>>>>>>> refs/remotes/origin/master
{
	unsigned long probability;
	unsigned long interval;
	int times;
	int space;

	/* "<interval>,<probability>,<space>,<times>" */
	if (sscanf(str, "%lu,%lu,%d,%d",
			&interval, &probability, &space, &times) < 4) {
		printk(KERN_WARNING
			"FAULT_INJECTION: failed to parse arguments\n");
		return 0;
	}

	attr->probability = probability;
	attr->interval = interval;
	atomic_set(&attr->times, times);
	atomic_set(&attr->space, space);

	return 1;
}
<<<<<<< HEAD
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(setup_fault_attr);
>>>>>>> refs/remotes/origin/cm-10.0
=======
EXPORT_SYMBOL_GPL(setup_fault_attr);
>>>>>>> refs/remotes/origin/master

static void fail_dump(struct fault_attr *attr)
{
	if (attr->verbose > 0)
		printk(KERN_NOTICE "FAULT_INJECTION: forcing a failure\n");
	if (attr->verbose > 1)
		dump_stack();
}

#define atomic_dec_not_zero(v)		atomic_add_unless((v), -1, 0)

static bool fail_task(struct fault_attr *attr, struct task_struct *task)
{
	return !in_interrupt() && task->make_it_fail;
}

#define MAX_STACK_TRACE_DEPTH 32

#ifdef CONFIG_FAULT_INJECTION_STACKTRACE_FILTER

static bool fail_stacktrace(struct fault_attr *attr)
{
	struct stack_trace trace;
	int depth = attr->stacktrace_depth;
	unsigned long entries[MAX_STACK_TRACE_DEPTH];
	int n;
	bool found = (attr->require_start == 0 && attr->require_end == ULONG_MAX);

	if (depth == 0)
		return found;

	trace.nr_entries = 0;
	trace.entries = entries;
	trace.max_entries = depth;
	trace.skip = 1;

	save_stack_trace(&trace);
	for (n = 0; n < trace.nr_entries; n++) {
		if (attr->reject_start <= entries[n] &&
			       entries[n] < attr->reject_end)
			return false;
		if (attr->require_start <= entries[n] &&
			       entries[n] < attr->require_end)
			found = true;
	}
	return found;
}

#else

static inline bool fail_stacktrace(struct fault_attr *attr)
{
	return true;
}

#endif /* CONFIG_FAULT_INJECTION_STACKTRACE_FILTER */

/*
 * This code is stolen from failmalloc-1.0
 * http://www.nongnu.org/failmalloc/
 */

bool should_fail(struct fault_attr *attr, ssize_t size)
{
<<<<<<< HEAD
=======
	/* No need to check any other properties if the probability is 0 */
	if (attr->probability == 0)
		return false;

>>>>>>> refs/remotes/origin/master
	if (attr->task_filter && !fail_task(attr, current))
		return false;

	if (atomic_read(&attr->times) == 0)
		return false;

	if (atomic_read(&attr->space) > size) {
		atomic_sub(size, &attr->space);
		return false;
	}

	if (attr->interval > 1) {
		attr->count++;
		if (attr->count % attr->interval)
			return false;
	}

<<<<<<< HEAD
	if (attr->probability <= random32() % 100)
=======
	if (attr->probability <= prandom_u32() % 100)
>>>>>>> refs/remotes/origin/master
		return false;

	if (!fail_stacktrace(attr))
		return false;

	fail_dump(attr);

	if (atomic_read(&attr->times) != -1)
		atomic_dec_not_zero(&attr->times);

	return true;
}
<<<<<<< HEAD
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(should_fail);
>>>>>>> refs/remotes/origin/cm-10.0
=======
EXPORT_SYMBOL_GPL(should_fail);
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_FAULT_INJECTION_DEBUG_FS

static int debugfs_ul_set(void *data, u64 val)
{
	*(unsigned long *)data = val;
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_FAULT_INJECTION_STACKTRACE_FILTER
static int debugfs_ul_set_MAX_STACK_TRACE_DEPTH(void *data, u64 val)
{
	*(unsigned long *)data =
		val < MAX_STACK_TRACE_DEPTH ?
		val : MAX_STACK_TRACE_DEPTH;
	return 0;
}
#endif /* CONFIG_FAULT_INJECTION_STACKTRACE_FILTER */

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static int debugfs_ul_get(void *data, u64 *val)
{
	*val = *(unsigned long *)data;
	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(fops_ul, debugfs_ul_get, debugfs_ul_set, "%llu\n");

<<<<<<< HEAD
<<<<<<< HEAD
static struct dentry *debugfs_create_ul(const char *name, mode_t mode,
=======
static struct dentry *debugfs_create_ul(const char *name, umode_t mode,
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct dentry *debugfs_create_ul(const char *name, umode_t mode,
>>>>>>> refs/remotes/origin/master
				struct dentry *parent, unsigned long *value)
{
	return debugfs_create_file(name, mode, parent, value, &fops_ul);
}

#ifdef CONFIG_FAULT_INJECTION_STACKTRACE_FILTER
<<<<<<< HEAD
<<<<<<< HEAD
DEFINE_SIMPLE_ATTRIBUTE(fops_ul_MAX_STACK_TRACE_DEPTH, debugfs_ul_get,
			debugfs_ul_set_MAX_STACK_TRACE_DEPTH, "%llu\n");

static struct dentry *debugfs_create_ul_MAX_STACK_TRACE_DEPTH(
	const char *name, mode_t mode,
	struct dentry *parent, unsigned long *value)
{
	return debugfs_create_file(name, mode, parent, value,
				   &fops_ul_MAX_STACK_TRACE_DEPTH);
}
=======
=======
>>>>>>> refs/remotes/origin/master

static int debugfs_stacktrace_depth_set(void *data, u64 val)
{
	*(unsigned long *)data =
		min_t(unsigned long, val, MAX_STACK_TRACE_DEPTH);

	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(fops_stacktrace_depth, debugfs_ul_get,
			debugfs_stacktrace_depth_set, "%llu\n");

static struct dentry *debugfs_create_stacktrace_depth(
	const char *name, umode_t mode,
	struct dentry *parent, unsigned long *value)
{
	return debugfs_create_file(name, mode, parent, value,
				   &fops_stacktrace_depth);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
#endif /* CONFIG_FAULT_INJECTION_STACKTRACE_FILTER */

static int debugfs_atomic_t_set(void *data, u64 val)
{
	atomic_set((atomic_t *)data, val);
	return 0;
}

static int debugfs_atomic_t_get(void *data, u64 *val)
{
	*val = atomic_read((atomic_t *)data);
	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(fops_atomic_t, debugfs_atomic_t_get,
			debugfs_atomic_t_set, "%lld\n");

<<<<<<< HEAD
static struct dentry *debugfs_create_atomic_t(const char *name, mode_t mode,
=======
static struct dentry *debugfs_create_atomic_t(const char *name, umode_t mode,
>>>>>>> refs/remotes/origin/cm-10.0
				struct dentry *parent, atomic_t *value)
{
	return debugfs_create_file(name, mode, parent, value, &fops_atomic_t);
}

<<<<<<< HEAD
void cleanup_fault_attr_dentries(struct fault_attr *attr)
{
	debugfs_remove(attr->dentries.probability_file);
	attr->dentries.probability_file = NULL;

	debugfs_remove(attr->dentries.interval_file);
	attr->dentries.interval_file = NULL;

	debugfs_remove(attr->dentries.times_file);
	attr->dentries.times_file = NULL;

	debugfs_remove(attr->dentries.space_file);
	attr->dentries.space_file = NULL;

	debugfs_remove(attr->dentries.verbose_file);
	attr->dentries.verbose_file = NULL;

	debugfs_remove(attr->dentries.task_filter_file);
	attr->dentries.task_filter_file = NULL;

#ifdef CONFIG_FAULT_INJECTION_STACKTRACE_FILTER

	debugfs_remove(attr->dentries.stacktrace_depth_file);
	attr->dentries.stacktrace_depth_file = NULL;

	debugfs_remove(attr->dentries.require_start_file);
	attr->dentries.require_start_file = NULL;

	debugfs_remove(attr->dentries.require_end_file);
	attr->dentries.require_end_file = NULL;

	debugfs_remove(attr->dentries.reject_start_file);
	attr->dentries.reject_start_file = NULL;

	debugfs_remove(attr->dentries.reject_end_file);
	attr->dentries.reject_end_file = NULL;

#endif /* CONFIG_FAULT_INJECTION_STACKTRACE_FILTER */

	if (attr->dentries.dir)
		WARN_ON(!simple_empty(attr->dentries.dir));

	debugfs_remove(attr->dentries.dir);
	attr->dentries.dir = NULL;
}

int init_fault_attr_dentries(struct fault_attr *attr, const char *name)
{
	mode_t mode = S_IFREG | S_IRUSR | S_IWUSR;
	struct dentry *dir;

	memset(&attr->dentries, 0, sizeof(attr->dentries));

	dir = debugfs_create_dir(name, NULL);
	if (!dir)
		goto fail;
	attr->dentries.dir = dir;

	attr->dentries.probability_file =
		debugfs_create_ul("probability", mode, dir, &attr->probability);

	attr->dentries.interval_file =
		debugfs_create_ul("interval", mode, dir, &attr->interval);

	attr->dentries.times_file =
		debugfs_create_atomic_t("times", mode, dir, &attr->times);

	attr->dentries.space_file =
		debugfs_create_atomic_t("space", mode, dir, &attr->space);

	attr->dentries.verbose_file =
		debugfs_create_ul("verbose", mode, dir, &attr->verbose);

	attr->dentries.task_filter_file = debugfs_create_bool("task-filter",
						mode, dir, &attr->task_filter);

	if (!attr->dentries.probability_file || !attr->dentries.interval_file ||
	    !attr->dentries.times_file || !attr->dentries.space_file ||
	    !attr->dentries.verbose_file || !attr->dentries.task_filter_file)
=======
=======
#endif /* CONFIG_FAULT_INJECTION_STACKTRACE_FILTER */

>>>>>>> refs/remotes/origin/master
struct dentry *fault_create_debugfs_attr(const char *name,
			struct dentry *parent, struct fault_attr *attr)
{
	umode_t mode = S_IFREG | S_IRUSR | S_IWUSR;
	struct dentry *dir;

	dir = debugfs_create_dir(name, parent);
	if (!dir)
		return ERR_PTR(-ENOMEM);

	if (!debugfs_create_ul("probability", mode, dir, &attr->probability))
		goto fail;
	if (!debugfs_create_ul("interval", mode, dir, &attr->interval))
		goto fail;
	if (!debugfs_create_atomic_t("times", mode, dir, &attr->times))
		goto fail;
	if (!debugfs_create_atomic_t("space", mode, dir, &attr->space))
		goto fail;
	if (!debugfs_create_ul("verbose", mode, dir, &attr->verbose))
		goto fail;
	if (!debugfs_create_bool("task-filter", mode, dir, &attr->task_filter))
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		goto fail;

#ifdef CONFIG_FAULT_INJECTION_STACKTRACE_FILTER

<<<<<<< HEAD
<<<<<<< HEAD
	attr->dentries.stacktrace_depth_file =
		debugfs_create_ul_MAX_STACK_TRACE_DEPTH(
			"stacktrace-depth", mode, dir, &attr->stacktrace_depth);

	attr->dentries.require_start_file =
		debugfs_create_ul("require-start", mode, dir, &attr->require_start);

	attr->dentries.require_end_file =
		debugfs_create_ul("require-end", mode, dir, &attr->require_end);

	attr->dentries.reject_start_file =
		debugfs_create_ul("reject-start", mode, dir, &attr->reject_start);

	attr->dentries.reject_end_file =
		debugfs_create_ul("reject-end", mode, dir, &attr->reject_end);

	if (!attr->dentries.stacktrace_depth_file ||
	    !attr->dentries.require_start_file ||
	    !attr->dentries.require_end_file ||
	    !attr->dentries.reject_start_file ||
	    !attr->dentries.reject_end_file)
=======
=======
>>>>>>> refs/remotes/origin/master
	if (!debugfs_create_stacktrace_depth("stacktrace-depth", mode, dir,
				&attr->stacktrace_depth))
		goto fail;
	if (!debugfs_create_ul("require-start", mode, dir,
				&attr->require_start))
		goto fail;
	if (!debugfs_create_ul("require-end", mode, dir, &attr->require_end))
		goto fail;
	if (!debugfs_create_ul("reject-start", mode, dir, &attr->reject_start))
		goto fail;
	if (!debugfs_create_ul("reject-end", mode, dir, &attr->reject_end))
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		goto fail;

#endif /* CONFIG_FAULT_INJECTION_STACKTRACE_FILTER */

<<<<<<< HEAD
<<<<<<< HEAD
	return 0;
fail:
	cleanup_fault_attr_dentries(attr);
	return -ENOMEM;
}
=======
=======
>>>>>>> refs/remotes/origin/master
	return dir;
fail:
	debugfs_remove_recursive(dir);

	return ERR_PTR(-ENOMEM);
}
EXPORT_SYMBOL_GPL(fault_create_debugfs_attr);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#endif /* CONFIG_FAULT_INJECTION_DEBUG_FS */
