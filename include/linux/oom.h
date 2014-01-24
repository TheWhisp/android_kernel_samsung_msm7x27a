#ifndef __INCLUDE_LINUX_OOM_H
#define __INCLUDE_LINUX_OOM_H

<<<<<<< HEAD
/*
 * /proc/<pid>/oom_adj is deprecated, see
 * Documentation/feature-removal-schedule.txt.
 *
 * /proc/<pid>/oom_adj set to -17 protects from the oom-killer
 */
#define OOM_DISABLE (-17)
/* inclusive */
#define OOM_ADJUST_MIN (-16)
#define OOM_ADJUST_MAX 15

/*
 * /proc/<pid>/oom_score_adj set to OOM_SCORE_ADJ_MIN disables oom killing for
 * pid.
 */
#define OOM_SCORE_ADJ_MIN	(-1000)
#define OOM_SCORE_ADJ_MAX	1000

#ifdef __KERNEL__
=======
>>>>>>> refs/remotes/origin/master

#include <linux/sched.h>
#include <linux/types.h>
#include <linux/nodemask.h>
<<<<<<< HEAD
=======
#include <uapi/linux/oom.h>
>>>>>>> refs/remotes/origin/master

struct zonelist;
struct notifier_block;
struct mem_cgroup;
struct task_struct;

/*
 * Types of limitations to the nodes from which allocations may occur
 */
enum oom_constraint {
	CONSTRAINT_NONE,
	CONSTRAINT_CPUSET,
	CONSTRAINT_MEMORY_POLICY,
	CONSTRAINT_MEMCG,
};

<<<<<<< HEAD
<<<<<<< HEAD
extern int test_set_oom_score_adj(int new_val);

extern unsigned int oom_badness(struct task_struct *p, struct mem_cgroup *mem,
=======
extern void compare_swap_oom_score_adj(int old_val, int new_val);
extern int test_set_oom_score_adj(int new_val);

extern unsigned int oom_badness(struct task_struct *p, struct mem_cgroup *memcg,
>>>>>>> refs/remotes/origin/cm-10.0
			const nodemask_t *nodemask, unsigned long totalpages);
extern int try_set_zonelist_oom(struct zonelist *zonelist, gfp_t gfp_flags);
extern void clear_zonelist_oom(struct zonelist *zonelist, gfp_t gfp_flags);

extern void out_of_memory(struct zonelist *zonelist, gfp_t gfp_mask,
<<<<<<< HEAD
		int order, nodemask_t *mask);
=======
		int order, nodemask_t *mask, bool force_kill);
>>>>>>> refs/remotes/origin/cm-10.0
=======
enum oom_scan_t {
	OOM_SCAN_OK,		/* scan thread and find its badness */
	OOM_SCAN_CONTINUE,	/* do not consider thread for oom kill */
	OOM_SCAN_ABORT,		/* abort the iteration and return */
	OOM_SCAN_SELECT,	/* always select this thread first */
};

/* Thread is the potential origin of an oom condition; kill first on oom */
#define OOM_FLAG_ORIGIN		((__force oom_flags_t)0x1)

static inline void set_current_oom_origin(void)
{
	current->signal->oom_flags |= OOM_FLAG_ORIGIN;
}

static inline void clear_current_oom_origin(void)
{
	current->signal->oom_flags &= ~OOM_FLAG_ORIGIN;
}

static inline bool oom_task_origin(const struct task_struct *p)
{
	return !!(p->signal->oom_flags & OOM_FLAG_ORIGIN);
}

extern unsigned long oom_badness(struct task_struct *p,
		struct mem_cgroup *memcg, const nodemask_t *nodemask,
		unsigned long totalpages);
extern void oom_kill_process(struct task_struct *p, gfp_t gfp_mask, int order,
			     unsigned int points, unsigned long totalpages,
			     struct mem_cgroup *memcg, nodemask_t *nodemask,
			     const char *message);

extern int try_set_zonelist_oom(struct zonelist *zonelist, gfp_t gfp_flags);
extern void clear_zonelist_oom(struct zonelist *zonelist, gfp_t gfp_flags);

extern void check_panic_on_oom(enum oom_constraint constraint, gfp_t gfp_mask,
			       int order, const nodemask_t *nodemask);

extern enum oom_scan_t oom_scan_process_thread(struct task_struct *task,
		unsigned long totalpages, const nodemask_t *nodemask,
		bool force_kill);

extern void out_of_memory(struct zonelist *zonelist, gfp_t gfp_mask,
		int order, nodemask_t *mask, bool force_kill);
>>>>>>> refs/remotes/origin/master
extern int register_oom_notifier(struct notifier_block *nb);
extern int unregister_oom_notifier(struct notifier_block *nb);

extern bool oom_killer_disabled;

static inline void oom_killer_disable(void)
{
	oom_killer_disabled = true;
}

static inline void oom_killer_enable(void)
{
	oom_killer_disabled = false;
}

<<<<<<< HEAD
<<<<<<< HEAD
/* The badness from the OOM killer */
extern unsigned long badness(struct task_struct *p, struct mem_cgroup *mem,
		      const nodemask_t *nodemask, unsigned long uptime);

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
static inline bool oom_gfp_allowed(gfp_t gfp_mask)
{
	return (gfp_mask & __GFP_FS) && !(gfp_mask & __GFP_NORETRY);
}

>>>>>>> refs/remotes/origin/master
extern struct task_struct *find_lock_task_mm(struct task_struct *p);

/* sysctls */
extern int sysctl_oom_dump_tasks;
extern int sysctl_oom_kill_allocating_task;
extern int sysctl_panic_on_oom;
<<<<<<< HEAD
#endif /* __KERNEL__*/
=======
>>>>>>> refs/remotes/origin/master
#endif /* _INCLUDE_LINUX_OOM_H */
