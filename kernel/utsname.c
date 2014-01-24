/*
 *  Copyright (C) 2004 IBM Corporation
 *
 *  Author: Serge Hallyn <serue@us.ibm.com>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation, version 2 of the
 *  License.
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
#include <linux/uts.h>
#include <linux/utsname.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/user_namespace.h>
<<<<<<< HEAD
#include <linux/proc_fs.h>
=======
#include <linux/proc_ns.h>
>>>>>>> refs/remotes/origin/master

static struct uts_namespace *create_uts_ns(void)
{
	struct uts_namespace *uts_ns;

	uts_ns = kmalloc(sizeof(struct uts_namespace), GFP_KERNEL);
	if (uts_ns)
		kref_init(&uts_ns->kref);
	return uts_ns;
}

/*
 * Clone a new ns copying an original utsname, setting refcount to 1
 * @old_ns: namespace to clone
<<<<<<< HEAD
 * Return NULL on error (failure to kmalloc), new ns otherwise
 */
static struct uts_namespace *clone_uts_ns(struct task_struct *tsk,
					  struct uts_namespace *old_ns)
{
	struct uts_namespace *ns;
=======
 * Return ERR_PTR(-ENOMEM) on error (failure to kmalloc), new ns otherwise
 */
static struct uts_namespace *clone_uts_ns(struct user_namespace *user_ns,
					  struct uts_namespace *old_ns)
{
	struct uts_namespace *ns;
	int err;
>>>>>>> refs/remotes/origin/master

	ns = create_uts_ns();
	if (!ns)
		return ERR_PTR(-ENOMEM);

<<<<<<< HEAD
	down_read(&uts_sem);
	memcpy(&ns->name, &old_ns->name, sizeof(ns->name));
	ns->user_ns = get_user_ns(task_cred_xxx(tsk, user)->user_ns);
=======
	err = proc_alloc_inum(&ns->proc_inum);
	if (err) {
		kfree(ns);
		return ERR_PTR(err);
	}

	down_read(&uts_sem);
	memcpy(&ns->name, &old_ns->name, sizeof(ns->name));
	ns->user_ns = get_user_ns(user_ns);
>>>>>>> refs/remotes/origin/master
	up_read(&uts_sem);
	return ns;
}

/*
 * Copy task tsk's utsname namespace, or clone it if flags
 * specifies CLONE_NEWUTS.  In latter case, changes to the
 * utsname of this process won't be seen by parent, and vice
 * versa.
 */
struct uts_namespace *copy_utsname(unsigned long flags,
<<<<<<< HEAD
				   struct task_struct *tsk)
{
	struct uts_namespace *old_ns = tsk->nsproxy->uts_ns;
=======
	struct user_namespace *user_ns, struct uts_namespace *old_ns)
{
>>>>>>> refs/remotes/origin/master
	struct uts_namespace *new_ns;

	BUG_ON(!old_ns);
	get_uts_ns(old_ns);

	if (!(flags & CLONE_NEWUTS))
		return old_ns;

<<<<<<< HEAD
	new_ns = clone_uts_ns(tsk, old_ns);
=======
	new_ns = clone_uts_ns(user_ns, old_ns);
>>>>>>> refs/remotes/origin/master

	put_uts_ns(old_ns);
	return new_ns;
}

void free_uts_ns(struct kref *kref)
{
	struct uts_namespace *ns;

	ns = container_of(kref, struct uts_namespace, kref);
	put_user_ns(ns->user_ns);
<<<<<<< HEAD
=======
	proc_free_inum(ns->proc_inum);
>>>>>>> refs/remotes/origin/master
	kfree(ns);
}

static void *utsns_get(struct task_struct *task)
{
	struct uts_namespace *ns = NULL;
	struct nsproxy *nsproxy;

	rcu_read_lock();
	nsproxy = task_nsproxy(task);
	if (nsproxy) {
		ns = nsproxy->uts_ns;
		get_uts_ns(ns);
	}
	rcu_read_unlock();

	return ns;
}

static void utsns_put(void *ns)
{
	put_uts_ns(ns);
}

<<<<<<< HEAD
static int utsns_install(struct nsproxy *nsproxy, void *ns)
{
=======
static int utsns_install(struct nsproxy *nsproxy, void *new)
{
	struct uts_namespace *ns = new;

	if (!ns_capable(ns->user_ns, CAP_SYS_ADMIN) ||
	    !ns_capable(current_user_ns(), CAP_SYS_ADMIN))
		return -EPERM;

>>>>>>> refs/remotes/origin/master
	get_uts_ns(ns);
	put_uts_ns(nsproxy->uts_ns);
	nsproxy->uts_ns = ns;
	return 0;
}

<<<<<<< HEAD
=======
static unsigned int utsns_inum(void *vp)
{
	struct uts_namespace *ns = vp;

	return ns->proc_inum;
}

>>>>>>> refs/remotes/origin/master
const struct proc_ns_operations utsns_operations = {
	.name		= "uts",
	.type		= CLONE_NEWUTS,
	.get		= utsns_get,
	.put		= utsns_put,
	.install	= utsns_install,
<<<<<<< HEAD
};

=======
	.inum		= utsns_inum,
};
>>>>>>> refs/remotes/origin/master
