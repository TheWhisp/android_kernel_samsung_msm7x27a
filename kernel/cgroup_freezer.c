/*
 * cgroup_freezer.c -  control group freezer subsystem
 *
 * Copyright IBM Corporation, 2007
 *
 * Author : Cedric Le Goater <clg@fr.ibm.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2.1 of the GNU Lesser General Public License
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/module.h>
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master
#include <linux/slab.h>
#include <linux/cgroup.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/freezer.h>
#include <linux/seq_file.h>

<<<<<<< HEAD
enum freezer_state {
	CGROUP_THAWED = 0,
	CGROUP_FREEZING,
	CGROUP_FROZEN,
};

struct freezer {
	struct cgroup_subsys_state css;
	enum freezer_state state;
	spinlock_t lock; /* protects _writes_ to state */
};

static inline struct freezer *cgroup_freezer(
		struct cgroup *cgroup)
{
	return container_of(
		cgroup_subsys_state(cgroup, freezer_subsys_id),
		struct freezer, css);
=======
/*
 * A cgroup is freezing if any FREEZING flags are set.  FREEZING_SELF is
 * set if "FROZEN" is written to freezer.state cgroupfs file, and cleared
 * for "THAWED".  FREEZING_PARENT is set if the parent freezer is FREEZING
 * for whatever reason.  IOW, a cgroup has FREEZING_PARENT set if one of
 * its ancestors has FREEZING_SELF set.
 */
enum freezer_state_flags {
	CGROUP_FREEZER_ONLINE	= (1 << 0), /* freezer is fully online */
	CGROUP_FREEZING_SELF	= (1 << 1), /* this freezer is freezing */
	CGROUP_FREEZING_PARENT	= (1 << 2), /* the parent freezer is freezing */
	CGROUP_FROZEN		= (1 << 3), /* this and its descendants frozen */

	/* mask for all FREEZING flags */
	CGROUP_FREEZING		= CGROUP_FREEZING_SELF | CGROUP_FREEZING_PARENT,
};

struct freezer {
	struct cgroup_subsys_state	css;
	unsigned int			state;
	spinlock_t			lock;
};

static inline struct freezer *css_freezer(struct cgroup_subsys_state *css)
{
	return css ? container_of(css, struct freezer, css) : NULL;
>>>>>>> refs/remotes/origin/master
}

static inline struct freezer *task_freezer(struct task_struct *task)
{
<<<<<<< HEAD
	return container_of(task_subsys_state(task, freezer_subsys_id),
			    struct freezer, css);
}

<<<<<<< HEAD
static inline int __cgroup_freezing_or_frozen(struct task_struct *task)
{
	enum freezer_state state = task_freezer(task)->state;
	return (state == CGROUP_FREEZING) || (state == CGROUP_FROZEN);
}

int cgroup_freezing_or_frozen(struct task_struct *task)
{
	int result;
	task_lock(task);
	result = __cgroup_freezing_or_frozen(task);
	task_unlock(task);
	return result;
=======
bool cgroup_freezing(struct task_struct *task)
{
	enum freezer_state state;
	bool ret;

	rcu_read_lock();
	state = task_freezer(task)->state;
	ret = state == CGROUP_FREEZING || state == CGROUP_FROZEN;
	rcu_read_unlock();

	return ret;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return css_freezer(task_css(task, freezer_subsys_id));
}

static struct freezer *parent_freezer(struct freezer *freezer)
{
	return css_freezer(css_parent(&freezer->css));
}

bool cgroup_freezing(struct task_struct *task)
{
	bool ret;

	rcu_read_lock();
	ret = task_freezer(task)->state & CGROUP_FREEZING;
	rcu_read_unlock();

	return ret;
>>>>>>> refs/remotes/origin/master
}

/*
 * cgroups_write_string() limits the size of freezer state strings to
 * CGROUP_LOCAL_BUFFER_SIZE
 */
<<<<<<< HEAD
static const char *freezer_state_strs[] = {
	"THAWED",
	"FREEZING",
	"FROZEN",
};

/*
 * State diagram
 * Transitions are caused by userspace writes to the freezer.state file.
 * The values in parenthesis are state labels. The rest are edge labels.
 *
 * (THAWED) --FROZEN--> (FREEZING) --FROZEN--> (FROZEN)
 *    ^ ^                    |                     |
 *    | \_______THAWED_______/                     |
 *    \__________________________THAWED____________/
 */

struct cgroup_subsys freezer_subsys;

/* Locks taken and their ordering
 * ------------------------------
 * cgroup_mutex (AKA cgroup_lock)
 * freezer->lock
 * css_set_lock
 * task->alloc_lock (AKA task_lock)
 * task->sighand->siglock
 *
 * cgroup code forces css_set_lock to be taken before task->alloc_lock
 *
 * freezer_create(), freezer_destroy():
 * cgroup_mutex [ by cgroup core ]
 *
 * freezer_can_attach():
 * cgroup_mutex (held by caller of can_attach)
 *
<<<<<<< HEAD
 * cgroup_freezing_or_frozen():
 * task->alloc_lock (to get task's cgroup)
 *
=======
>>>>>>> refs/remotes/origin/cm-10.0
 * freezer_fork() (preserving fork() performance means can't take cgroup_mutex):
 * freezer->lock
 *  sighand->siglock (if the cgroup is freezing)
 *
 * freezer_read():
 * cgroup_mutex
 *  freezer->lock
 *   write_lock css_set_lock (cgroup iterator start)
 *    task->alloc_lock
 *   read_lock css_set_lock (cgroup iterator start)
 *
 * freezer_write() (freeze):
 * cgroup_mutex
 *  freezer->lock
 *   write_lock css_set_lock (cgroup iterator start)
 *    task->alloc_lock
 *   read_lock css_set_lock (cgroup iterator start)
 *    sighand->siglock (fake signal delivery inside freeze_task())
 *
 * freezer_write() (unfreeze):
 * cgroup_mutex
 *  freezer->lock
 *   write_lock css_set_lock (cgroup iterator start)
 *    task->alloc_lock
 *   read_lock css_set_lock (cgroup iterator start)
<<<<<<< HEAD
 *    task->alloc_lock (inside thaw_process(), prevents race with refrigerator())
 *     sighand->siglock
 */
static struct cgroup_subsys_state *freezer_create(struct cgroup_subsys *ss,
						  struct cgroup *cgroup)
=======
 *    task->alloc_lock (inside __thaw_task(), prevents race with refrigerator())
 *     sighand->siglock
 */
static struct cgroup_subsys_state *freezer_create(struct cgroup *cgroup)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static const char *freezer_state_strs(unsigned int state)
{
	if (state & CGROUP_FROZEN)
		return "FROZEN";
	if (state & CGROUP_FREEZING)
		return "FREEZING";
	return "THAWED";
};

struct cgroup_subsys freezer_subsys;

static struct cgroup_subsys_state *
freezer_css_alloc(struct cgroup_subsys_state *parent_css)
>>>>>>> refs/remotes/origin/master
{
	struct freezer *freezer;

	freezer = kzalloc(sizeof(struct freezer), GFP_KERNEL);
	if (!freezer)
		return ERR_PTR(-ENOMEM);

	spin_lock_init(&freezer->lock);
<<<<<<< HEAD
	freezer->state = CGROUP_THAWED;
	return &freezer->css;
}

<<<<<<< HEAD
static void freezer_destroy(struct cgroup_subsys *ss,
			    struct cgroup *cgroup)
{
	kfree(cgroup_freezer(cgroup));
=======
static void freezer_destroy(struct cgroup *cgroup)
{
	struct freezer *freezer = cgroup_freezer(cgroup);

	if (freezer->state != CGROUP_THAWED)
		atomic_dec(&system_freezing_cnt);
	kfree(freezer);
>>>>>>> refs/remotes/origin/cm-10.0
}

/* task is frozen or will freeze immediately when next it gets woken */
static bool is_task_frozen_enough(struct task_struct *task)
{
	return frozen(task) ||
		(task_is_stopped_or_traced(task) && freezing(task));
}

/*
 * The call to cgroup_lock() in the freezer.state write method prevents
 * a write to that file racing against an attach, and hence the
 * can_attach() result will remain valid until the attach completes.
 */
<<<<<<< HEAD
static int freezer_can_attach(struct cgroup_subsys *ss,
			      struct cgroup *new_cgroup,
			      struct task_struct *task)
{
	struct freezer *freezer;
=======
static int freezer_can_attach(struct cgroup *new_cgroup,
			      struct cgroup_taskset *tset)
{
	struct freezer *freezer;
	struct task_struct *task;
>>>>>>> refs/remotes/origin/cm-10.0

	if ((current != task) && (!capable(CAP_SYS_ADMIN))) {
		const struct cred *cred = current_cred(), *tcred;

		tcred = __task_cred(task);
		if (cred->euid != tcred->uid && cred->euid != tcred->suid)
			return -EPERM;
	}

	/*
	 * Anything frozen can't move or be moved to/from.
	 */
<<<<<<< HEAD
=======
	cgroup_taskset_for_each(task, new_cgroup, tset)
		if (cgroup_freezing(task))
			return -EBUSY;
>>>>>>> refs/remotes/origin/cm-10.0

	freezer = cgroup_freezer(new_cgroup);
	if (freezer->state != CGROUP_THAWED)
		return -EBUSY;

	return 0;
}

<<<<<<< HEAD
static int freezer_can_attach_task(struct cgroup *cgrp, struct task_struct *tsk)
{
	rcu_read_lock();
	if (__cgroup_freezing_or_frozen(tsk)) {
		rcu_read_unlock();
		return -EBUSY;
	}
	rcu_read_unlock();
	return 0;
}

static void freezer_fork(struct cgroup_subsys *ss, struct task_struct *task)
=======
static void freezer_fork(struct task_struct *task)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct freezer *freezer;

	/*
	 * No lock is needed, since the task isn't on tasklist yet,
	 * so it can't be moved to another cgroup, which means the
	 * freezer won't be removed and will be valid during this
	 * function call.  Nevertheless, apply RCU read-side critical
	 * section to suppress RCU lockdep false positives.
	 */
	rcu_read_lock();
	freezer = task_freezer(task);
	rcu_read_unlock();
=======
	return &freezer->css;
}

/**
 * freezer_css_online - commit creation of a freezer css
 * @css: css being created
 *
 * We're committing to creation of @css.  Mark it online and inherit
 * parent's freezing state while holding both parent's and our
 * freezer->lock.
 */
static int freezer_css_online(struct cgroup_subsys_state *css)
{
	struct freezer *freezer = css_freezer(css);
	struct freezer *parent = parent_freezer(freezer);

	/*
	 * The following double locking and freezing state inheritance
	 * guarantee that @cgroup can never escape ancestors' freezing
	 * states.  See css_for_each_descendant_pre() for details.
	 */
	if (parent)
		spin_lock_irq(&parent->lock);
	spin_lock_nested(&freezer->lock, SINGLE_DEPTH_NESTING);

	freezer->state |= CGROUP_FREEZER_ONLINE;

	if (parent && (parent->state & CGROUP_FREEZING)) {
		freezer->state |= CGROUP_FREEZING_PARENT | CGROUP_FROZEN;
		atomic_inc(&system_freezing_cnt);
	}

	spin_unlock(&freezer->lock);
	if (parent)
		spin_unlock_irq(&parent->lock);

	return 0;
}

/**
 * freezer_css_offline - initiate destruction of a freezer css
 * @css: css being destroyed
 *
 * @css is going away.  Mark it dead and decrement system_freezing_count if
 * it was holding one.
 */
static void freezer_css_offline(struct cgroup_subsys_state *css)
{
	struct freezer *freezer = css_freezer(css);

	spin_lock_irq(&freezer->lock);

	if (freezer->state & CGROUP_FREEZING)
		atomic_dec(&system_freezing_cnt);

	freezer->state = 0;

	spin_unlock_irq(&freezer->lock);
}

static void freezer_css_free(struct cgroup_subsys_state *css)
{
	kfree(css_freezer(css));
}

/*
 * Tasks can be migrated into a different freezer anytime regardless of its
 * current state.  freezer_attach() is responsible for making new tasks
 * conform to the current state.
 *
 * Freezer state changes and task migration are synchronized via
 * @freezer->lock.  freezer_attach() makes the new tasks conform to the
 * current state and all following state changes can see the new tasks.
 */
static void freezer_attach(struct cgroup_subsys_state *new_css,
			   struct cgroup_taskset *tset)
{
	struct freezer *freezer = css_freezer(new_css);
	struct task_struct *task;
	bool clear_frozen = false;

	spin_lock_irq(&freezer->lock);

	/*
	 * Make the new tasks conform to the current state of @new_css.
	 * For simplicity, when migrating any task to a FROZEN cgroup, we
	 * revert it to FREEZING and let update_if_frozen() determine the
	 * correct state later.
	 *
	 * Tasks in @tset are on @new_css but may not conform to its
	 * current state before executing the following - !frozen tasks may
	 * be visible in a FROZEN cgroup and frozen tasks in a THAWED one.
	 */
	cgroup_taskset_for_each(task, new_css, tset) {
		if (!(freezer->state & CGROUP_FREEZING)) {
			__thaw_task(task);
		} else {
			freeze_task(task);
			freezer->state &= ~CGROUP_FROZEN;
			clear_frozen = true;
		}
	}

	spin_unlock_irq(&freezer->lock);

	/*
	 * Propagate FROZEN clearing upwards.  We may race with
	 * update_if_frozen(), but as long as both work bottom-up, either
	 * update_if_frozen() sees child's FROZEN cleared or we clear the
	 * parent's FROZEN later.  No parent w/ !FROZEN children can be
	 * left FROZEN.
	 */
	while (clear_frozen && (freezer = parent_freezer(freezer))) {
		spin_lock_irq(&freezer->lock);
		freezer->state &= ~CGROUP_FROZEN;
		clear_frozen = freezer->state & CGROUP_FREEZING;
		spin_unlock_irq(&freezer->lock);
	}
}

static void freezer_fork(struct task_struct *task)
{
	struct freezer *freezer;

	rcu_read_lock();
	freezer = task_freezer(task);
>>>>>>> refs/remotes/origin/master

	/*
	 * The root cgroup is non-freezable, so we can skip the
	 * following check.
	 */
<<<<<<< HEAD
	if (!freezer->css.cgroup->parent)
		return;

	spin_lock_irq(&freezer->lock);
	BUG_ON(freezer->state == CGROUP_FROZEN);

	/* Locking avoids race with FREEZING -> THAWED transitions. */
	if (freezer->state == CGROUP_FREEZING)
<<<<<<< HEAD
		freeze_task(task, true);
=======
		freeze_task(task);
>>>>>>> refs/remotes/origin/cm-10.0
	spin_unlock_irq(&freezer->lock);
}

/*
 * caller must hold freezer->lock
 */
static void update_if_frozen(struct cgroup *cgroup,
				 struct freezer *freezer)
{
	struct cgroup_iter it;
	struct task_struct *task;
	unsigned int nfrozen = 0, ntotal = 0;
	enum freezer_state old_state = freezer->state;

	cgroup_iter_start(cgroup, &it);
	while ((task = cgroup_iter_next(cgroup, &it))) {
		ntotal++;
<<<<<<< HEAD
		if (is_task_frozen_enough(task))
=======
		if (freezing(task) && is_task_frozen_enough(task))
>>>>>>> refs/remotes/origin/cm-10.0
			nfrozen++;
	}

	if (old_state == CGROUP_THAWED) {
		BUG_ON(nfrozen > 0);
	} else if (old_state == CGROUP_FREEZING) {
		if (nfrozen == ntotal)
			freezer->state = CGROUP_FROZEN;
	} else { /* old_state == CGROUP_FROZEN */
		BUG_ON(nfrozen != ntotal);
	}

	cgroup_iter_end(cgroup, &it);
}

static int freezer_read(struct cgroup *cgroup, struct cftype *cft,
			struct seq_file *m)
{
	struct freezer *freezer;
	enum freezer_state state;

	if (!cgroup_lock_live_group(cgroup))
		return -ENODEV;

	freezer = cgroup_freezer(cgroup);
	spin_lock_irq(&freezer->lock);
	state = freezer->state;
	if (state == CGROUP_FREEZING) {
		/* We change from FREEZING to FROZEN lazily if the cgroup was
		 * only partially frozen when we exitted write. */
		update_if_frozen(cgroup, freezer);
		state = freezer->state;
	}
	spin_unlock_irq(&freezer->lock);
	cgroup_unlock();

	seq_puts(m, freezer_state_strs[state]);
=======
	if (!parent_freezer(freezer))
		goto out;

	spin_lock_irq(&freezer->lock);
	if (freezer->state & CGROUP_FREEZING)
		freeze_task(task);
	spin_unlock_irq(&freezer->lock);
out:
	rcu_read_unlock();
}

/**
 * update_if_frozen - update whether a cgroup finished freezing
 * @css: css of interest
 *
 * Once FREEZING is initiated, transition to FROZEN is lazily updated by
 * calling this function.  If the current state is FREEZING but not FROZEN,
 * this function checks whether all tasks of this cgroup and the descendant
 * cgroups finished freezing and, if so, sets FROZEN.
 *
 * The caller is responsible for grabbing RCU read lock and calling
 * update_if_frozen() on all descendants prior to invoking this function.
 *
 * Task states and freezer state might disagree while tasks are being
 * migrated into or out of @css, so we can't verify task states against
 * @freezer state here.  See freezer_attach() for details.
 */
static void update_if_frozen(struct cgroup_subsys_state *css)
{
	struct freezer *freezer = css_freezer(css);
	struct cgroup_subsys_state *pos;
	struct css_task_iter it;
	struct task_struct *task;

	WARN_ON_ONCE(!rcu_read_lock_held());

	spin_lock_irq(&freezer->lock);

	if (!(freezer->state & CGROUP_FREEZING) ||
	    (freezer->state & CGROUP_FROZEN))
		goto out_unlock;

	/* are all (live) children frozen? */
	css_for_each_child(pos, css) {
		struct freezer *child = css_freezer(pos);

		if ((child->state & CGROUP_FREEZER_ONLINE) &&
		    !(child->state & CGROUP_FROZEN))
			goto out_unlock;
	}

	/* are all tasks frozen? */
	css_task_iter_start(css, &it);

	while ((task = css_task_iter_next(&it))) {
		if (freezing(task)) {
			/*
			 * freezer_should_skip() indicates that the task
			 * should be skipped when determining freezing
			 * completion.  Consider it frozen in addition to
			 * the usual frozen condition.
			 */
			if (!frozen(task) && !freezer_should_skip(task))
				goto out_iter_end;
		}
	}

	freezer->state |= CGROUP_FROZEN;
out_iter_end:
	css_task_iter_end(&it);
out_unlock:
	spin_unlock_irq(&freezer->lock);
}

static int freezer_read(struct seq_file *m, void *v)
{
	struct cgroup_subsys_state *css = seq_css(m), *pos;

	rcu_read_lock();

	/* update states bottom-up */
	css_for_each_descendant_post(pos, css)
		update_if_frozen(pos);

	rcu_read_unlock();

	seq_puts(m, freezer_state_strs(css_freezer(css)->state));
>>>>>>> refs/remotes/origin/master
	seq_putc(m, '\n');
	return 0;
}

<<<<<<< HEAD
static int try_to_freeze_cgroup(struct cgroup *cgroup, struct freezer *freezer)
{
	struct cgroup_iter it;
	struct task_struct *task;
	unsigned int num_cant_freeze_now = 0;

<<<<<<< HEAD
	freezer->state = CGROUP_FREEZING;
	cgroup_iter_start(cgroup, &it);
	while ((task = cgroup_iter_next(cgroup, &it))) {
		if (!freeze_task(task, true))
=======
	cgroup_iter_start(cgroup, &it);
	while ((task = cgroup_iter_next(cgroup, &it))) {
		if (!freeze_task(task))
>>>>>>> refs/remotes/origin/cm-10.0
			continue;
		if (is_task_frozen_enough(task))
			continue;
		if (!freezing(task) && !freezer_should_skip(task))
			num_cant_freeze_now++;
	}
	cgroup_iter_end(cgroup, &it);

	return num_cant_freeze_now ? -EBUSY : 0;
}

static void unfreeze_cgroup(struct cgroup *cgroup, struct freezer *freezer)
{
	struct cgroup_iter it;
	struct task_struct *task;

	cgroup_iter_start(cgroup, &it);
<<<<<<< HEAD
	while ((task = cgroup_iter_next(cgroup, &it))) {
		thaw_process(task);
	}
	cgroup_iter_end(cgroup, &it);

	freezer->state = CGROUP_THAWED;
=======
	while ((task = cgroup_iter_next(cgroup, &it)))
		__thaw_task(task);
	cgroup_iter_end(cgroup, &it);
>>>>>>> refs/remotes/origin/cm-10.0
}

static int freezer_change_state(struct cgroup *cgroup,
				enum freezer_state goal_state)
{
	struct freezer *freezer;
	int retval = 0;

	freezer = cgroup_freezer(cgroup);

	spin_lock_irq(&freezer->lock);

	update_if_frozen(cgroup, freezer);
<<<<<<< HEAD
	if (goal_state == freezer->state)
		goto out;

	switch (goal_state) {
	case CGROUP_THAWED:
		unfreeze_cgroup(cgroup, freezer);
		break;
	case CGROUP_FROZEN:
=======

	switch (goal_state) {
	case CGROUP_THAWED:
		if (freezer->state != CGROUP_THAWED)
			atomic_dec(&system_freezing_cnt);
		freezer->state = CGROUP_THAWED;
		unfreeze_cgroup(cgroup, freezer);
		break;
	case CGROUP_FROZEN:
		if (freezer->state == CGROUP_THAWED)
			atomic_inc(&system_freezing_cnt);
		freezer->state = CGROUP_FREEZING;
>>>>>>> refs/remotes/origin/cm-10.0
		retval = try_to_freeze_cgroup(cgroup, freezer);
		break;
	default:
		BUG();
	}
<<<<<<< HEAD
out:
=======

>>>>>>> refs/remotes/origin/cm-10.0
	spin_unlock_irq(&freezer->lock);

	return retval;
}

static int freezer_write(struct cgroup *cgroup,
			 struct cftype *cft,
			 const char *buffer)
{
	int retval;
	enum freezer_state goal_state;

	if (strcmp(buffer, freezer_state_strs[CGROUP_THAWED]) == 0)
		goal_state = CGROUP_THAWED;
	else if (strcmp(buffer, freezer_state_strs[CGROUP_FROZEN]) == 0)
		goal_state = CGROUP_FROZEN;
	else
		return -EINVAL;

	if (!cgroup_lock_live_group(cgroup))
		return -ENODEV;
	retval = freezer_change_state(cgroup, goal_state);
	cgroup_unlock();
	return retval;
=======
static void freeze_cgroup(struct freezer *freezer)
{
	struct css_task_iter it;
	struct task_struct *task;

	css_task_iter_start(&freezer->css, &it);
	while ((task = css_task_iter_next(&it)))
		freeze_task(task);
	css_task_iter_end(&it);
}

static void unfreeze_cgroup(struct freezer *freezer)
{
	struct css_task_iter it;
	struct task_struct *task;

	css_task_iter_start(&freezer->css, &it);
	while ((task = css_task_iter_next(&it)))
		__thaw_task(task);
	css_task_iter_end(&it);
}

/**
 * freezer_apply_state - apply state change to a single cgroup_freezer
 * @freezer: freezer to apply state change to
 * @freeze: whether to freeze or unfreeze
 * @state: CGROUP_FREEZING_* flag to set or clear
 *
 * Set or clear @state on @cgroup according to @freeze, and perform
 * freezing or thawing as necessary.
 */
static void freezer_apply_state(struct freezer *freezer, bool freeze,
				unsigned int state)
{
	/* also synchronizes against task migration, see freezer_attach() */
	lockdep_assert_held(&freezer->lock);

	if (!(freezer->state & CGROUP_FREEZER_ONLINE))
		return;

	if (freeze) {
		if (!(freezer->state & CGROUP_FREEZING))
			atomic_inc(&system_freezing_cnt);
		freezer->state |= state;
		freeze_cgroup(freezer);
	} else {
		bool was_freezing = freezer->state & CGROUP_FREEZING;

		freezer->state &= ~state;

		if (!(freezer->state & CGROUP_FREEZING)) {
			if (was_freezing)
				atomic_dec(&system_freezing_cnt);
			freezer->state &= ~CGROUP_FROZEN;
			unfreeze_cgroup(freezer);
		}
	}
}

/**
 * freezer_change_state - change the freezing state of a cgroup_freezer
 * @freezer: freezer of interest
 * @freeze: whether to freeze or thaw
 *
 * Freeze or thaw @freezer according to @freeze.  The operations are
 * recursive - all descendants of @freezer will be affected.
 */
static void freezer_change_state(struct freezer *freezer, bool freeze)
{
	struct cgroup_subsys_state *pos;

	/*
	 * Update all its descendants in pre-order traversal.  Each
	 * descendant will try to inherit its parent's FREEZING state as
	 * CGROUP_FREEZING_PARENT.
	 */
	rcu_read_lock();
	css_for_each_descendant_pre(pos, &freezer->css) {
		struct freezer *pos_f = css_freezer(pos);
		struct freezer *parent = parent_freezer(pos_f);

		spin_lock_irq(&pos_f->lock);

		if (pos_f == freezer) {
			freezer_apply_state(pos_f, freeze,
					    CGROUP_FREEZING_SELF);
		} else {
			/*
			 * Our update to @parent->state is already visible
			 * which is all we need.  No need to lock @parent.
			 * For more info on synchronization, see
			 * freezer_post_create().
			 */
			freezer_apply_state(pos_f,
					    parent->state & CGROUP_FREEZING,
					    CGROUP_FREEZING_PARENT);
		}

		spin_unlock_irq(&pos_f->lock);
	}
	rcu_read_unlock();
}

static int freezer_write(struct cgroup_subsys_state *css, struct cftype *cft,
			 const char *buffer)
{
	bool freeze;

	if (strcmp(buffer, freezer_state_strs(0)) == 0)
		freeze = false;
	else if (strcmp(buffer, freezer_state_strs(CGROUP_FROZEN)) == 0)
		freeze = true;
	else
		return -EINVAL;

	freezer_change_state(css_freezer(css), freeze);
	return 0;
}

static u64 freezer_self_freezing_read(struct cgroup_subsys_state *css,
				      struct cftype *cft)
{
	struct freezer *freezer = css_freezer(css);

	return (bool)(freezer->state & CGROUP_FREEZING_SELF);
}

static u64 freezer_parent_freezing_read(struct cgroup_subsys_state *css,
					struct cftype *cft)
{
	struct freezer *freezer = css_freezer(css);

	return (bool)(freezer->state & CGROUP_FREEZING_PARENT);
>>>>>>> refs/remotes/origin/master
}

static struct cftype files[] = {
	{
		.name = "state",
<<<<<<< HEAD
		.read_seq_string = freezer_read,
		.write_string = freezer_write,
	},
};

static int freezer_populate(struct cgroup_subsys *ss, struct cgroup *cgroup)
{
	if (!cgroup->parent)
		return 0;
	return cgroup_add_files(cgroup, ss, files, ARRAY_SIZE(files));
}

struct cgroup_subsys freezer_subsys = {
	.name		= "freezer",
	.create		= freezer_create,
	.destroy	= freezer_destroy,
	.populate	= freezer_populate,
	.subsys_id	= freezer_subsys_id,
	.can_attach	= freezer_can_attach,
<<<<<<< HEAD
	.can_attach_task = freezer_can_attach_task,
	.pre_attach	= NULL,
	.attach_task	= NULL,
	.attach		= NULL,
	.fork		= freezer_fork,
	.exit		= NULL,
=======
	.fork		= freezer_fork,
>>>>>>> refs/remotes/origin/cm-10.0
=======
		.flags = CFTYPE_NOT_ON_ROOT,
		.seq_show = freezer_read,
		.write_string = freezer_write,
	},
	{
		.name = "self_freezing",
		.flags = CFTYPE_NOT_ON_ROOT,
		.read_u64 = freezer_self_freezing_read,
	},
	{
		.name = "parent_freezing",
		.flags = CFTYPE_NOT_ON_ROOT,
		.read_u64 = freezer_parent_freezing_read,
	},
	{ }	/* terminate */
};

struct cgroup_subsys freezer_subsys = {
	.name		= "freezer",
	.css_alloc	= freezer_css_alloc,
	.css_online	= freezer_css_online,
	.css_offline	= freezer_css_offline,
	.css_free	= freezer_css_free,
	.subsys_id	= freezer_subsys_id,
	.attach		= freezer_attach,
	.fork		= freezer_fork,
	.base_cftypes	= files,
>>>>>>> refs/remotes/origin/master
};
