/*
 * cls_cgroup.h			Control Group Classifier
 *
 * Authors:	Thomas Graf <tgraf@suug.ch>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 */

#ifndef _NET_CLS_CGROUP_H
#define _NET_CLS_CGROUP_H

#include <linux/cgroup.h>
#include <linux/hardirq.h>
#include <linux/rcupdate.h>

<<<<<<< HEAD
#ifdef CONFIG_CGROUPS
=======
#if IS_ENABLED(CONFIG_NET_CLS_CGROUP)
>>>>>>> refs/remotes/origin/master
struct cgroup_cls_state
{
	struct cgroup_subsys_state css;
	u32 classid;
};

<<<<<<< HEAD
#ifdef CONFIG_NET_CLS_CGROUP
static inline u32 task_cls_classid(struct task_struct *p)
{
	int classid;
=======
void sock_update_classid(struct sock *sk);

#if IS_BUILTIN(CONFIG_NET_CLS_CGROUP)
static inline u32 task_cls_classid(struct task_struct *p)
{
	u32 classid;
>>>>>>> refs/remotes/origin/master

	if (in_interrupt())
		return 0;

	rcu_read_lock();
<<<<<<< HEAD
	classid = container_of(task_subsys_state(p, net_cls_subsys_id),
=======
	classid = container_of(task_css(p, net_cls_subsys_id),
>>>>>>> refs/remotes/origin/master
			       struct cgroup_cls_state, css)->classid;
	rcu_read_unlock();

	return classid;
}
<<<<<<< HEAD
#else
extern int net_cls_subsys_id;

static inline u32 task_cls_classid(struct task_struct *p)
{
	int id;
=======
#elif IS_MODULE(CONFIG_NET_CLS_CGROUP)
static inline u32 task_cls_classid(struct task_struct *p)
{
	struct cgroup_subsys_state *css;
>>>>>>> refs/remotes/origin/master
	u32 classid = 0;

	if (in_interrupt())
		return 0;

	rcu_read_lock();
<<<<<<< HEAD
	id = rcu_dereference_index_check(net_cls_subsys_id,
					 rcu_read_lock_held());
	if (id >= 0)
		classid = container_of(task_subsys_state(p, id),
=======
	css = task_css(p, net_cls_subsys_id);
	if (css)
		classid = container_of(css,
>>>>>>> refs/remotes/origin/master
				       struct cgroup_cls_state, css)->classid;
	rcu_read_unlock();

	return classid;
}
#endif
<<<<<<< HEAD
#else
=======
#else /* !CGROUP_NET_CLS_CGROUP */
static inline void sock_update_classid(struct sock *sk)
{
}

>>>>>>> refs/remotes/origin/master
static inline u32 task_cls_classid(struct task_struct *p)
{
	return 0;
}
<<<<<<< HEAD
#endif
=======
#endif /* CGROUP_NET_CLS_CGROUP */
>>>>>>> refs/remotes/origin/master
#endif  /* _NET_CLS_CGROUP_H */
