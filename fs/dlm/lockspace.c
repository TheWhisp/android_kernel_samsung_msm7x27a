/******************************************************************************
*******************************************************************************
**
**  Copyright (C) Sistina Software, Inc.  1997-2003  All rights reserved.
<<<<<<< HEAD
<<<<<<< HEAD
**  Copyright (C) 2004-2008 Red Hat, Inc.  All rights reserved.
=======
**  Copyright (C) 2004-2011 Red Hat, Inc.  All rights reserved.
>>>>>>> refs/remotes/origin/cm-10.0
=======
**  Copyright (C) 2004-2011 Red Hat, Inc.  All rights reserved.
>>>>>>> refs/remotes/origin/master
**
**  This copyrighted material is made available to anyone wishing to use,
**  modify, copy, or redistribute it subject to the terms and conditions
**  of the GNU General Public License v.2.
**
*******************************************************************************
******************************************************************************/

#include "dlm_internal.h"
#include "lockspace.h"
#include "member.h"
#include "recoverd.h"
<<<<<<< HEAD
<<<<<<< HEAD
#include "ast.h"
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#include "dir.h"
#include "lowcomms.h"
#include "config.h"
#include "memory.h"
#include "lock.h"
#include "recover.h"
#include "requestqueue.h"
#include "user.h"
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include "ast.h"
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include "ast.h"
>>>>>>> refs/remotes/origin/master

static int			ls_count;
static struct mutex		ls_lock;
static struct list_head		lslist;
static spinlock_t		lslist_lock;
static struct task_struct *	scand_task;


static ssize_t dlm_control_store(struct dlm_ls *ls, const char *buf, size_t len)
{
	ssize_t ret = len;
	int n = simple_strtol(buf, NULL, 0);

	ls = dlm_find_lockspace_local(ls->ls_local_handle);
	if (!ls)
		return -EINVAL;

	switch (n) {
	case 0:
		dlm_ls_stop(ls);
		break;
	case 1:
		dlm_ls_start(ls);
		break;
	default:
		ret = -EINVAL;
	}
	dlm_put_lockspace(ls);
	return ret;
}

static ssize_t dlm_event_store(struct dlm_ls *ls, const char *buf, size_t len)
{
	ls->ls_uevent_result = simple_strtol(buf, NULL, 0);
	set_bit(LSFL_UEVENT_WAIT, &ls->ls_flags);
	wake_up(&ls->ls_uevent_wait);
	return len;
}

static ssize_t dlm_id_show(struct dlm_ls *ls, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%u\n", ls->ls_global_id);
}

static ssize_t dlm_id_store(struct dlm_ls *ls, const char *buf, size_t len)
{
	ls->ls_global_id = simple_strtoul(buf, NULL, 0);
	return len;
}

<<<<<<< HEAD
=======
static ssize_t dlm_nodir_show(struct dlm_ls *ls, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%u\n", dlm_no_directory(ls));
}

static ssize_t dlm_nodir_store(struct dlm_ls *ls, const char *buf, size_t len)
{
	int val = simple_strtoul(buf, NULL, 0);
	if (val == 1)
		set_bit(LSFL_NODIR, &ls->ls_flags);
	return len;
}

>>>>>>> refs/remotes/origin/master
static ssize_t dlm_recover_status_show(struct dlm_ls *ls, char *buf)
{
	uint32_t status = dlm_recover_status(ls);
	return snprintf(buf, PAGE_SIZE, "%x\n", status);
}

static ssize_t dlm_recover_nodeid_show(struct dlm_ls *ls, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", ls->ls_recover_nodeid);
}

struct dlm_attr {
	struct attribute attr;
	ssize_t (*show)(struct dlm_ls *, char *);
	ssize_t (*store)(struct dlm_ls *, const char *, size_t);
};

static struct dlm_attr dlm_attr_control = {
	.attr  = {.name = "control", .mode = S_IWUSR},
	.store = dlm_control_store
};

static struct dlm_attr dlm_attr_event = {
	.attr  = {.name = "event_done", .mode = S_IWUSR},
	.store = dlm_event_store
};

static struct dlm_attr dlm_attr_id = {
	.attr  = {.name = "id", .mode = S_IRUGO | S_IWUSR},
	.show  = dlm_id_show,
	.store = dlm_id_store
};

<<<<<<< HEAD
=======
static struct dlm_attr dlm_attr_nodir = {
	.attr  = {.name = "nodir", .mode = S_IRUGO | S_IWUSR},
	.show  = dlm_nodir_show,
	.store = dlm_nodir_store
};

>>>>>>> refs/remotes/origin/master
static struct dlm_attr dlm_attr_recover_status = {
	.attr  = {.name = "recover_status", .mode = S_IRUGO},
	.show  = dlm_recover_status_show
};

static struct dlm_attr dlm_attr_recover_nodeid = {
	.attr  = {.name = "recover_nodeid", .mode = S_IRUGO},
	.show  = dlm_recover_nodeid_show
};

static struct attribute *dlm_attrs[] = {
	&dlm_attr_control.attr,
	&dlm_attr_event.attr,
	&dlm_attr_id.attr,
<<<<<<< HEAD
=======
	&dlm_attr_nodir.attr,
>>>>>>> refs/remotes/origin/master
	&dlm_attr_recover_status.attr,
	&dlm_attr_recover_nodeid.attr,
	NULL,
};

static ssize_t dlm_attr_show(struct kobject *kobj, struct attribute *attr,
			     char *buf)
{
	struct dlm_ls *ls  = container_of(kobj, struct dlm_ls, ls_kobj);
	struct dlm_attr *a = container_of(attr, struct dlm_attr, attr);
	return a->show ? a->show(ls, buf) : 0;
}

static ssize_t dlm_attr_store(struct kobject *kobj, struct attribute *attr,
			      const char *buf, size_t len)
{
	struct dlm_ls *ls  = container_of(kobj, struct dlm_ls, ls_kobj);
	struct dlm_attr *a = container_of(attr, struct dlm_attr, attr);
	return a->store ? a->store(ls, buf, len) : len;
}

static void lockspace_kobj_release(struct kobject *k)
{
	struct dlm_ls *ls  = container_of(k, struct dlm_ls, ls_kobj);
	kfree(ls);
}

static const struct sysfs_ops dlm_attr_ops = {
	.show  = dlm_attr_show,
	.store = dlm_attr_store,
};

static struct kobj_type dlm_ktype = {
	.default_attrs = dlm_attrs,
	.sysfs_ops     = &dlm_attr_ops,
	.release       = lockspace_kobj_release,
};

static struct kset *dlm_kset;

static int do_uevent(struct dlm_ls *ls, int in)
{
	int error;

	if (in)
		kobject_uevent(&ls->ls_kobj, KOBJ_ONLINE);
	else
		kobject_uevent(&ls->ls_kobj, KOBJ_OFFLINE);

	log_debug(ls, "%s the lockspace group...", in ? "joining" : "leaving");

	/* dlm_controld will see the uevent, do the necessary group management
	   and then write to sysfs to wake us */

	error = wait_event_interruptible(ls->ls_uevent_wait,
			test_and_clear_bit(LSFL_UEVENT_WAIT, &ls->ls_flags));

	log_debug(ls, "group event done %d %d", error, ls->ls_uevent_result);

	if (error)
		goto out;

	error = ls->ls_uevent_result;
 out:
	if (error)
		log_error(ls, "group %s failed %d %d", in ? "join" : "leave",
			  error, ls->ls_uevent_result);
	return error;
}

static int dlm_uevent(struct kset *kset, struct kobject *kobj,
		      struct kobj_uevent_env *env)
{
	struct dlm_ls *ls = container_of(kobj, struct dlm_ls, ls_kobj);

	add_uevent_var(env, "LOCKSPACE=%s", ls->ls_name);
	return 0;
}

static struct kset_uevent_ops dlm_uevent_ops = {
	.uevent = dlm_uevent,
};

int __init dlm_lockspace_init(void)
{
	ls_count = 0;
	mutex_init(&ls_lock);
	INIT_LIST_HEAD(&lslist);
	spin_lock_init(&lslist_lock);

	dlm_kset = kset_create_and_add("dlm", &dlm_uevent_ops, kernel_kobj);
	if (!dlm_kset) {
		printk(KERN_WARNING "%s: can not create kset\n", __func__);
		return -ENOMEM;
	}
	return 0;
}

void dlm_lockspace_exit(void)
{
	kset_unregister(dlm_kset);
}

static struct dlm_ls *find_ls_to_scan(void)
{
	struct dlm_ls *ls;

	spin_lock(&lslist_lock);
	list_for_each_entry(ls, &lslist, ls_list) {
		if (time_after_eq(jiffies, ls->ls_scan_time +
					    dlm_config.ci_scan_secs * HZ)) {
			spin_unlock(&lslist_lock);
			return ls;
		}
	}
	spin_unlock(&lslist_lock);
	return NULL;
}

static int dlm_scand(void *data)
{
	struct dlm_ls *ls;

	while (!kthread_should_stop()) {
		ls = find_ls_to_scan();
		if (ls) {
			if (dlm_lock_recovery_try(ls)) {
				ls->ls_scan_time = jiffies;
				dlm_scan_rsbs(ls);
				dlm_scan_timeout(ls);
				dlm_scan_waiters(ls);
				dlm_unlock_recovery(ls);
			} else {
				ls->ls_scan_time += HZ;
			}
			continue;
		}
		schedule_timeout_interruptible(dlm_config.ci_scan_secs * HZ);
	}
	return 0;
}

static int dlm_scand_start(void)
{
	struct task_struct *p;
	int error = 0;

	p = kthread_run(dlm_scand, NULL, "dlm_scand");
	if (IS_ERR(p))
		error = PTR_ERR(p);
	else
		scand_task = p;
	return error;
}

static void dlm_scand_stop(void)
{
	kthread_stop(scand_task);
}

struct dlm_ls *dlm_find_lockspace_global(uint32_t id)
{
	struct dlm_ls *ls;

	spin_lock(&lslist_lock);

	list_for_each_entry(ls, &lslist, ls_list) {
		if (ls->ls_global_id == id) {
			ls->ls_count++;
			goto out;
		}
	}
	ls = NULL;
 out:
	spin_unlock(&lslist_lock);
	return ls;
}

struct dlm_ls *dlm_find_lockspace_local(dlm_lockspace_t *lockspace)
{
	struct dlm_ls *ls;

	spin_lock(&lslist_lock);
	list_for_each_entry(ls, &lslist, ls_list) {
		if (ls->ls_local_handle == lockspace) {
			ls->ls_count++;
			goto out;
		}
	}
	ls = NULL;
 out:
	spin_unlock(&lslist_lock);
	return ls;
}

struct dlm_ls *dlm_find_lockspace_device(int minor)
{
	struct dlm_ls *ls;

	spin_lock(&lslist_lock);
	list_for_each_entry(ls, &lslist, ls_list) {
		if (ls->ls_device.minor == minor) {
			ls->ls_count++;
			goto out;
		}
	}
	ls = NULL;
 out:
	spin_unlock(&lslist_lock);
	return ls;
}

void dlm_put_lockspace(struct dlm_ls *ls)
{
	spin_lock(&lslist_lock);
	ls->ls_count--;
	spin_unlock(&lslist_lock);
}

static void remove_lockspace(struct dlm_ls *ls)
{
	for (;;) {
		spin_lock(&lslist_lock);
		if (ls->ls_count == 0) {
			WARN_ON(ls->ls_create_count != 0);
			list_del(&ls->ls_list);
			spin_unlock(&lslist_lock);
			return;
		}
		spin_unlock(&lslist_lock);
		ssleep(1);
	}
}

static int threads_start(void)
{
	int error;

<<<<<<< HEAD
<<<<<<< HEAD
	/* Thread which process lock requests for all lockspace's */
	error = dlm_astd_start();
	if (error) {
		log_print("cannot start dlm_astd thread %d", error);
		goto fail;
	}

	error = dlm_scand_start();
	if (error) {
		log_print("cannot start dlm_scand thread %d", error);
		goto astd_fail;
=======
=======
>>>>>>> refs/remotes/origin/master
	error = dlm_scand_start();
	if (error) {
		log_print("cannot start dlm_scand thread %d", error);
		goto fail;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	/* Thread for sending/receiving messages for all lockspace's */
	error = dlm_lowcomms_start();
	if (error) {
		log_print("cannot start dlm lowcomms %d", error);
		goto scand_fail;
	}

	return 0;

 scand_fail:
	dlm_scand_stop();
<<<<<<< HEAD
<<<<<<< HEAD
 astd_fail:
	dlm_astd_stop();
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 fail:
	return error;
}

static void threads_stop(void)
{
	dlm_scand_stop();
	dlm_lowcomms_stop();
<<<<<<< HEAD
<<<<<<< HEAD
	dlm_astd_stop();
}

static int new_lockspace(const char *name, int namelen, void **lockspace,
			 uint32_t flags, int lvblen)
=======
=======
>>>>>>> refs/remotes/origin/master
}

static int new_lockspace(const char *name, const char *cluster,
			 uint32_t flags, int lvblen,
			 const struct dlm_lockspace_ops *ops, void *ops_arg,
			 int *ops_result, dlm_lockspace_t **lockspace)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	struct dlm_ls *ls;
	int i, size, error;
	int do_unreg = 0;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	int namelen = strlen(name);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	int namelen = strlen(name);
>>>>>>> refs/remotes/origin/master

	if (namelen > DLM_LOCKSPACE_LEN)
		return -EINVAL;

	if (!lvblen || (lvblen % 8))
		return -EINVAL;

	if (!try_module_get(THIS_MODULE))
		return -EINVAL;

	if (!dlm_user_daemon_available()) {
<<<<<<< HEAD
<<<<<<< HEAD
		module_put(THIS_MODULE);
		return -EUNATCH;
=======
=======
>>>>>>> refs/remotes/origin/master
		log_print("dlm user daemon not available");
		error = -EUNATCH;
		goto out;
	}

	if (ops && ops_result) {
	       	if (!dlm_config.ci_recover_callbacks)
			*ops_result = -EOPNOTSUPP;
		else
			*ops_result = 0;
	}

	if (dlm_config.ci_recover_callbacks && cluster &&
	    strncmp(cluster, dlm_config.ci_cluster_name, DLM_LOCKSPACE_LEN)) {
		log_print("dlm cluster name %s mismatch %s",
			  dlm_config.ci_cluster_name, cluster);
		error = -EBADR;
		goto out;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	error = 0;

	spin_lock(&lslist_lock);
	list_for_each_entry(ls, &lslist, ls_list) {
		WARN_ON(ls->ls_create_count <= 0);
		if (ls->ls_namelen != namelen)
			continue;
		if (memcmp(ls->ls_name, name, namelen))
			continue;
		if (flags & DLM_LSFL_NEWEXCL) {
			error = -EEXIST;
			break;
		}
		ls->ls_create_count++;
		*lockspace = ls;
		error = 1;
		break;
	}
	spin_unlock(&lslist_lock);

	if (error)
		goto out;

	error = -ENOMEM;

	ls = kzalloc(sizeof(struct dlm_ls) + namelen, GFP_NOFS);
	if (!ls)
		goto out;
	memcpy(ls->ls_name, name, namelen);
	ls->ls_namelen = namelen;
	ls->ls_lvblen = lvblen;
	ls->ls_count = 0;
	ls->ls_flags = 0;
	ls->ls_scan_time = jiffies;

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	if (ops && dlm_config.ci_recover_callbacks) {
		ls->ls_ops = ops;
		ls->ls_ops_arg = ops_arg;
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (flags & DLM_LSFL_TIMEWARN)
		set_bit(LSFL_TIMEWARN, &ls->ls_flags);

	/* ls_exflags are forced to match among nodes, and we don't
	   need to require all nodes to have some flags set */
	ls->ls_exflags = (flags & ~(DLM_LSFL_TIMEWARN | DLM_LSFL_FS |
				    DLM_LSFL_NEWEXCL));

	size = dlm_config.ci_rsbtbl_size;
	ls->ls_rsbtbl_size = size;

<<<<<<< HEAD
<<<<<<< HEAD
	ls->ls_rsbtbl = kmalloc(sizeof(struct dlm_rsbtable) * size, GFP_NOFS);
	if (!ls->ls_rsbtbl)
		goto out_lsfree;
	for (i = 0; i < size; i++) {
		INIT_LIST_HEAD(&ls->ls_rsbtbl[i].list);
		INIT_LIST_HEAD(&ls->ls_rsbtbl[i].toss);
		spin_lock_init(&ls->ls_rsbtbl[i].lock);
	}

	size = dlm_config.ci_lkbtbl_size;
	ls->ls_lkbtbl_size = size;

	ls->ls_lkbtbl = kmalloc(sizeof(struct dlm_lkbtable) * size, GFP_NOFS);
	if (!ls->ls_lkbtbl)
		goto out_rsbfree;
	for (i = 0; i < size; i++) {
		INIT_LIST_HEAD(&ls->ls_lkbtbl[i].list);
		rwlock_init(&ls->ls_lkbtbl[i].lock);
		ls->ls_lkbtbl[i].counter = 1;
	}
=======
=======
>>>>>>> refs/remotes/origin/master
	ls->ls_rsbtbl = vmalloc(sizeof(struct dlm_rsbtable) * size);
	if (!ls->ls_rsbtbl)
		goto out_lsfree;
	for (i = 0; i < size; i++) {
		ls->ls_rsbtbl[i].keep.rb_node = NULL;
		ls->ls_rsbtbl[i].toss.rb_node = NULL;
		spin_lock_init(&ls->ls_rsbtbl[i].lock);
	}

<<<<<<< HEAD
	idr_init(&ls->ls_lkbidr);
	spin_lock_init(&ls->ls_lkbidr_spin);
>>>>>>> refs/remotes/origin/cm-10.0

	size = dlm_config.ci_dirtbl_size;
	ls->ls_dirtbl_size = size;

<<<<<<< HEAD
	ls->ls_dirtbl = kmalloc(sizeof(struct dlm_dirtable) * size, GFP_NOFS);
=======
	ls->ls_dirtbl = vmalloc(sizeof(struct dlm_dirtable) * size);
>>>>>>> refs/remotes/origin/cm-10.0
	if (!ls->ls_dirtbl)
		goto out_lkbfree;
	for (i = 0; i < size; i++) {
		INIT_LIST_HEAD(&ls->ls_dirtbl[i].list);
		spin_lock_init(&ls->ls_dirtbl[i].lock);
	}
=======
	spin_lock_init(&ls->ls_remove_spin);

	for (i = 0; i < DLM_REMOVE_NAMES_MAX; i++) {
		ls->ls_remove_names[i] = kzalloc(DLM_RESNAME_MAXLEN+1,
						 GFP_KERNEL);
		if (!ls->ls_remove_names[i])
			goto out_rsbtbl;
	}

	idr_init(&ls->ls_lkbidr);
	spin_lock_init(&ls->ls_lkbidr_spin);
>>>>>>> refs/remotes/origin/master

	INIT_LIST_HEAD(&ls->ls_waiters);
	mutex_init(&ls->ls_waiters_mutex);
	INIT_LIST_HEAD(&ls->ls_orphans);
	mutex_init(&ls->ls_orphans_mutex);
	INIT_LIST_HEAD(&ls->ls_timeout);
	mutex_init(&ls->ls_timeout_mutex);

<<<<<<< HEAD
<<<<<<< HEAD
=======
	INIT_LIST_HEAD(&ls->ls_new_rsb);
	spin_lock_init(&ls->ls_new_rsb_spin);

>>>>>>> refs/remotes/origin/cm-10.0
=======
	INIT_LIST_HEAD(&ls->ls_new_rsb);
	spin_lock_init(&ls->ls_new_rsb_spin);

>>>>>>> refs/remotes/origin/master
	INIT_LIST_HEAD(&ls->ls_nodes);
	INIT_LIST_HEAD(&ls->ls_nodes_gone);
	ls->ls_num_nodes = 0;
	ls->ls_low_nodeid = 0;
	ls->ls_total_weight = 0;
	ls->ls_node_array = NULL;

	memset(&ls->ls_stub_rsb, 0, sizeof(struct dlm_rsb));
	ls->ls_stub_rsb.res_ls = ls;

	ls->ls_debug_rsb_dentry = NULL;
	ls->ls_debug_waiters_dentry = NULL;

	init_waitqueue_head(&ls->ls_uevent_wait);
	ls->ls_uevent_result = 0;
	init_completion(&ls->ls_members_done);
	ls->ls_members_result = -1;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	mutex_init(&ls->ls_cb_mutex);
	INIT_LIST_HEAD(&ls->ls_cb_delay);

>>>>>>> refs/remotes/origin/cm-10.0
=======
	mutex_init(&ls->ls_cb_mutex);
	INIT_LIST_HEAD(&ls->ls_cb_delay);

>>>>>>> refs/remotes/origin/master
	ls->ls_recoverd_task = NULL;
	mutex_init(&ls->ls_recoverd_active);
	spin_lock_init(&ls->ls_recover_lock);
	spin_lock_init(&ls->ls_rcom_spin);
	get_random_bytes(&ls->ls_rcom_seq, sizeof(uint64_t));
	ls->ls_recover_status = 0;
	ls->ls_recover_seq = 0;
	ls->ls_recover_args = NULL;
	init_rwsem(&ls->ls_in_recovery);
	init_rwsem(&ls->ls_recv_active);
	INIT_LIST_HEAD(&ls->ls_requestqueue);
	mutex_init(&ls->ls_requestqueue_mutex);
	mutex_init(&ls->ls_clear_proc_locks);

	ls->ls_recover_buf = kmalloc(dlm_config.ci_buffer_size, GFP_NOFS);
	if (!ls->ls_recover_buf)
<<<<<<< HEAD
		goto out_dirfree;

<<<<<<< HEAD
=======
=======
		goto out_lkbidr;

>>>>>>> refs/remotes/origin/master
	ls->ls_slot = 0;
	ls->ls_num_slots = 0;
	ls->ls_slots_size = 0;
	ls->ls_slots = NULL;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	INIT_LIST_HEAD(&ls->ls_recover_list);
	spin_lock_init(&ls->ls_recover_list_lock);
=======
	INIT_LIST_HEAD(&ls->ls_recover_list);
	spin_lock_init(&ls->ls_recover_list_lock);
	idr_init(&ls->ls_recover_idr);
	spin_lock_init(&ls->ls_recover_idr_lock);
>>>>>>> refs/remotes/origin/master
	ls->ls_recover_list_count = 0;
	ls->ls_local_handle = ls;
	init_waitqueue_head(&ls->ls_wait_general);
	INIT_LIST_HEAD(&ls->ls_root_list);
	init_rwsem(&ls->ls_root_sem);

<<<<<<< HEAD
	down_write(&ls->ls_in_recovery);

=======
>>>>>>> refs/remotes/origin/master
	spin_lock(&lslist_lock);
	ls->ls_create_count = 1;
	list_add(&ls->ls_list, &lslist);
	spin_unlock(&lslist_lock);

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	if (flags & DLM_LSFL_FS) {
		error = dlm_callback_start(ls);
		if (error) {
			log_error(ls, "can't start dlm_callback %d", error);
			goto out_delist;
		}
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	/* needs to find ls in lslist */
	error = dlm_recoverd_start(ls);
	if (error) {
		log_error(ls, "can't start dlm_recoverd %d", error);
<<<<<<< HEAD
		goto out_delist;
=======
		goto out_callback;
>>>>>>> refs/remotes/origin/cm-10.0
	}

=======
	init_waitqueue_head(&ls->ls_recover_lock_wait);

	/*
	 * Once started, dlm_recoverd first looks for ls in lslist, then
	 * initializes ls_in_recovery as locked in "down" mode.  We need
	 * to wait for the wakeup from dlm_recoverd because in_recovery
	 * has to start out in down mode.
	 */

	error = dlm_recoverd_start(ls);
	if (error) {
		log_error(ls, "can't start dlm_recoverd %d", error);
		goto out_callback;
	}

	wait_event(ls->ls_recover_lock_wait,
		   test_bit(LSFL_RECOVER_LOCK, &ls->ls_flags));

>>>>>>> refs/remotes/origin/master
	ls->ls_kobj.kset = dlm_kset;
	error = kobject_init_and_add(&ls->ls_kobj, &dlm_ktype, NULL,
				     "%s", ls->ls_name);
	if (error)
<<<<<<< HEAD
<<<<<<< HEAD
		goto out_stop;
=======
		goto out_recoverd;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		goto out_recoverd;
>>>>>>> refs/remotes/origin/master
	kobject_uevent(&ls->ls_kobj, KOBJ_ADD);

	/* let kobject handle freeing of ls if there's an error */
	do_unreg = 1;

	/* This uevent triggers dlm_controld in userspace to add us to the
	   group of nodes that are members of this lockspace (managed by the
	   cluster infrastructure.)  Once it's done that, it tells us who the
	   current lockspace members are (via configfs) and then tells the
	   lockspace to start running (via sysfs) in dlm_ls_start(). */

	error = do_uevent(ls, 1);
	if (error)
<<<<<<< HEAD
<<<<<<< HEAD
		goto out_stop;
=======
		goto out_recoverd;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		goto out_recoverd;
>>>>>>> refs/remotes/origin/master

	wait_for_completion(&ls->ls_members_done);
	error = ls->ls_members_result;
	if (error)
		goto out_members;

	dlm_create_debug_file(ls);

	log_debug(ls, "join complete");
	*lockspace = ls;
	return 0;

 out_members:
	do_uevent(ls, 0);
	dlm_clear_members(ls);
	kfree(ls->ls_node_array);
<<<<<<< HEAD
<<<<<<< HEAD
 out_stop:
	dlm_recoverd_stop(ls);
=======
=======
>>>>>>> refs/remotes/origin/master
 out_recoverd:
	dlm_recoverd_stop(ls);
 out_callback:
	dlm_callback_stop(ls);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 out_delist:
	spin_lock(&lslist_lock);
	list_del(&ls->ls_list);
	spin_unlock(&lslist_lock);
<<<<<<< HEAD
	kfree(ls->ls_recover_buf);
 out_dirfree:
<<<<<<< HEAD
	kfree(ls->ls_dirtbl);
 out_lkbfree:
	kfree(ls->ls_lkbtbl);
 out_rsbfree:
	kfree(ls->ls_rsbtbl);
=======
	vfree(ls->ls_dirtbl);
 out_lkbfree:
	idr_destroy(&ls->ls_lkbidr);
	vfree(ls->ls_rsbtbl);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	idr_destroy(&ls->ls_recover_idr);
	kfree(ls->ls_recover_buf);
 out_lkbidr:
	idr_destroy(&ls->ls_lkbidr);
	for (i = 0; i < DLM_REMOVE_NAMES_MAX; i++) {
		if (ls->ls_remove_names[i])
			kfree(ls->ls_remove_names[i]);
	}
 out_rsbtbl:
	vfree(ls->ls_rsbtbl);
>>>>>>> refs/remotes/origin/master
 out_lsfree:
	if (do_unreg)
		kobject_put(&ls->ls_kobj);
	else
		kfree(ls);
 out:
	module_put(THIS_MODULE);
	return error;
}

<<<<<<< HEAD
<<<<<<< HEAD
int dlm_new_lockspace(const char *name, int namelen, void **lockspace,
		      uint32_t flags, int lvblen)
=======
=======
>>>>>>> refs/remotes/origin/master
int dlm_new_lockspace(const char *name, const char *cluster,
		      uint32_t flags, int lvblen,
		      const struct dlm_lockspace_ops *ops, void *ops_arg,
		      int *ops_result, dlm_lockspace_t **lockspace)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	int error = 0;

	mutex_lock(&ls_lock);
	if (!ls_count)
		error = threads_start();
	if (error)
		goto out;

<<<<<<< HEAD
<<<<<<< HEAD
	error = new_lockspace(name, namelen, lockspace, flags, lvblen);
=======
	error = new_lockspace(name, cluster, flags, lvblen, ops, ops_arg,
			      ops_result, lockspace);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	error = new_lockspace(name, cluster, flags, lvblen, ops, ops_arg,
			      ops_result, lockspace);
>>>>>>> refs/remotes/origin/master
	if (!error)
		ls_count++;
	if (error > 0)
		error = 0;
	if (!ls_count)
		threads_stop();
 out:
	mutex_unlock(&ls_lock);
	return error;
}

<<<<<<< HEAD
<<<<<<< HEAD
/* Return 1 if the lockspace still has active remote locks,
 *        2 if the lockspace still has active local locks.
 */
static int lockspace_busy(struct dlm_ls *ls)
{
	int i, lkb_found = 0;
	struct dlm_lkb *lkb;

	/* NOTE: We check the lockidtbl here rather than the resource table.
	   This is because there may be LKBs queued as ASTs that have been
	   unlinked from their RSBs and are pending deletion once the AST has
	   been delivered */

	for (i = 0; i < ls->ls_lkbtbl_size; i++) {
		read_lock(&ls->ls_lkbtbl[i].lock);
		if (!list_empty(&ls->ls_lkbtbl[i].list)) {
			lkb_found = 1;
			list_for_each_entry(lkb, &ls->ls_lkbtbl[i].list,
					    lkb_idtbl_list) {
				if (!lkb->lkb_nodeid) {
					read_unlock(&ls->ls_lkbtbl[i].lock);
					return 2;
				}
			}
		}
		read_unlock(&ls->ls_lkbtbl[i].lock);
	}
	return lkb_found;
=======
=======
>>>>>>> refs/remotes/origin/master
static int lkb_idr_is_local(int id, void *p, void *data)
{
	struct dlm_lkb *lkb = p;

<<<<<<< HEAD
	if (!lkb->lkb_nodeid)
		return 1;
	return 0;
=======
	return lkb->lkb_nodeid == 0 && lkb->lkb_grmode != DLM_LOCK_IV;
>>>>>>> refs/remotes/origin/master
}

static int lkb_idr_is_any(int id, void *p, void *data)
{
	return 1;
}

static int lkb_idr_free(int id, void *p, void *data)
{
	struct dlm_lkb *lkb = p;

	if (lkb->lkb_lvbptr && lkb->lkb_flags & DLM_IFL_MSTCPY)
		dlm_free_lvb(lkb->lkb_lvbptr);

	dlm_free_lkb(lkb);
	return 0;
}

/* NOTE: We check the lkbidr here rather than the resource table.
   This is because there may be LKBs queued as ASTs that have been unlinked
   from their RSBs and are pending deletion once the AST has been delivered */

static int lockspace_busy(struct dlm_ls *ls, int force)
{
	int rv;

	spin_lock(&ls->ls_lkbidr_spin);
	if (force == 0) {
		rv = idr_for_each(&ls->ls_lkbidr, lkb_idr_is_any, ls);
	} else if (force == 1) {
		rv = idr_for_each(&ls->ls_lkbidr, lkb_idr_is_local, ls);
	} else {
		rv = 0;
	}
	spin_unlock(&ls->ls_lkbidr_spin);
	return rv;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static int release_lockspace(struct dlm_ls *ls, int force)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct dlm_lkb *lkb;
	struct dlm_rsb *rsb;
	struct list_head *head;
	int i, busy, rv;

	busy = lockspace_busy(ls);

	spin_lock(&lslist_lock);
	if (ls->ls_create_count == 1) {
		if (busy > force)
			rv = -EBUSY;
		else {
=======
=======
>>>>>>> refs/remotes/origin/master
	struct dlm_rsb *rsb;
	struct rb_node *n;
	int i, busy, rv;

	busy = lockspace_busy(ls, force);

	spin_lock(&lslist_lock);
	if (ls->ls_create_count == 1) {
		if (busy) {
			rv = -EBUSY;
		} else {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			/* remove_lockspace takes ls off lslist */
			ls->ls_create_count = 0;
			rv = 0;
		}
	} else if (ls->ls_create_count > 1) {
		rv = --ls->ls_create_count;
	} else {
		rv = -EINVAL;
	}
	spin_unlock(&lslist_lock);

	if (rv) {
		log_debug(ls, "release_lockspace no remove %d", rv);
		return rv;
	}

	dlm_device_deregister(ls);

	if (force < 3 && dlm_user_daemon_available())
		do_uevent(ls, 0);

	dlm_recoverd_stop(ls);

<<<<<<< HEAD
<<<<<<< HEAD
=======
	dlm_callback_stop(ls);

>>>>>>> refs/remotes/origin/cm-10.0
=======
	dlm_callback_stop(ls);

>>>>>>> refs/remotes/origin/master
	remove_lockspace(ls);

	dlm_delete_debug_file(ls);

<<<<<<< HEAD
<<<<<<< HEAD
	dlm_astd_suspend();

=======
>>>>>>> refs/remotes/origin/cm-10.0
	kfree(ls->ls_recover_buf);

	/*
	 * Free direntry structs.
	 */

	dlm_dir_clear(ls);
<<<<<<< HEAD
	kfree(ls->ls_dirtbl);

	/*
	 * Free all lkb's on lkbtbl[] lists.
	 */

	for (i = 0; i < ls->ls_lkbtbl_size; i++) {
		head = &ls->ls_lkbtbl[i].list;
		while (!list_empty(head)) {
			lkb = list_entry(head->next, struct dlm_lkb,
					 lkb_idtbl_list);

			list_del(&lkb->lkb_idtbl_list);

			dlm_del_ast(lkb);

			if (lkb->lkb_lvbptr && lkb->lkb_flags & DLM_IFL_MSTCPY)
				dlm_free_lvb(lkb->lkb_lvbptr);

			dlm_free_lkb(lkb);
		}
	}
	dlm_astd_resume();

	kfree(ls->ls_lkbtbl);
=======
	vfree(ls->ls_dirtbl);

	/*
=======
	kfree(ls->ls_recover_buf);

	/*
>>>>>>> refs/remotes/origin/master
	 * Free all lkb's in idr
	 */

	idr_for_each(&ls->ls_lkbidr, lkb_idr_free, ls);
<<<<<<< HEAD
	idr_remove_all(&ls->ls_lkbidr);
	idr_destroy(&ls->ls_lkbidr);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	idr_destroy(&ls->ls_lkbidr);
>>>>>>> refs/remotes/origin/master

	/*
	 * Free all rsb's on rsbtbl[] lists
	 */

	for (i = 0; i < ls->ls_rsbtbl_size; i++) {
<<<<<<< HEAD
<<<<<<< HEAD
		head = &ls->ls_rsbtbl[i].list;
		while (!list_empty(head)) {
			rsb = list_entry(head->next, struct dlm_rsb,
					 res_hashchain);

			list_del(&rsb->res_hashchain);
			dlm_free_rsb(rsb);
		}

		head = &ls->ls_rsbtbl[i].toss;
		while (!list_empty(head)) {
			rsb = list_entry(head->next, struct dlm_rsb,
					 res_hashchain);
			list_del(&rsb->res_hashchain);
=======
=======
>>>>>>> refs/remotes/origin/master
		while ((n = rb_first(&ls->ls_rsbtbl[i].keep))) {
			rsb = rb_entry(n, struct dlm_rsb, res_hashnode);
			rb_erase(n, &ls->ls_rsbtbl[i].keep);
			dlm_free_rsb(rsb);
		}

		while ((n = rb_first(&ls->ls_rsbtbl[i].toss))) {
			rsb = rb_entry(n, struct dlm_rsb, res_hashnode);
			rb_erase(n, &ls->ls_rsbtbl[i].toss);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			dlm_free_rsb(rsb);
		}
	}

<<<<<<< HEAD
<<<<<<< HEAD
	kfree(ls->ls_rsbtbl);
=======
	vfree(ls->ls_rsbtbl);

=======
	vfree(ls->ls_rsbtbl);

	for (i = 0; i < DLM_REMOVE_NAMES_MAX; i++)
		kfree(ls->ls_remove_names[i]);

>>>>>>> refs/remotes/origin/master
	while (!list_empty(&ls->ls_new_rsb)) {
		rsb = list_first_entry(&ls->ls_new_rsb, struct dlm_rsb,
				       res_hashchain);
		list_del(&rsb->res_hashchain);
		dlm_free_rsb(rsb);
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	/*
	 * Free structures on any other lists
	 */

	dlm_purge_requestqueue(ls);
	kfree(ls->ls_recover_args);
<<<<<<< HEAD
	dlm_clear_free_entries(ls);
=======
>>>>>>> refs/remotes/origin/master
	dlm_clear_members(ls);
	dlm_clear_members_gone(ls);
	kfree(ls->ls_node_array);
	log_debug(ls, "release_lockspace final free");
	kobject_put(&ls->ls_kobj);
	/* The ls structure will be freed when the kobject is done with */

	module_put(THIS_MODULE);
	return 0;
}

/*
 * Called when a system has released all its locks and is not going to use the
 * lockspace any longer.  We free everything we're managing for this lockspace.
 * Remaining nodes will go through the recovery process as if we'd died.  The
 * lockspace must continue to function as usual, participating in recoveries,
 * until this returns.
 *
 * Force has 4 possible values:
 * 0 - don't destroy locksapce if it has any LKBs
 * 1 - destroy lockspace if it has remote LKBs but not if it has local LKBs
 * 2 - destroy lockspace regardless of LKBs
 * 3 - destroy lockspace as part of a forced shutdown
 */

int dlm_release_lockspace(void *lockspace, int force)
{
	struct dlm_ls *ls;
	int error;

	ls = dlm_find_lockspace_local(lockspace);
	if (!ls)
		return -EINVAL;
	dlm_put_lockspace(ls);

	mutex_lock(&ls_lock);
	error = release_lockspace(ls, force);
	if (!error)
		ls_count--;
	if (!ls_count)
		threads_stop();
	mutex_unlock(&ls_lock);

	return error;
}

void dlm_stop_lockspaces(void)
{
	struct dlm_ls *ls;
<<<<<<< HEAD

 restart:
	spin_lock(&lslist_lock);
	list_for_each_entry(ls, &lslist, ls_list) {
		if (!test_bit(LSFL_RUNNING, &ls->ls_flags))
			continue;
=======
	int count;

 restart:
	count = 0;
	spin_lock(&lslist_lock);
	list_for_each_entry(ls, &lslist, ls_list) {
		if (!test_bit(LSFL_RUNNING, &ls->ls_flags)) {
			count++;
			continue;
		}
>>>>>>> refs/remotes/origin/master
		spin_unlock(&lslist_lock);
		log_error(ls, "no userland control daemon, stopping lockspace");
		dlm_ls_stop(ls);
		goto restart;
	}
	spin_unlock(&lslist_lock);
<<<<<<< HEAD
=======

	if (count)
		log_print("dlm user daemon left %d lockspaces", count);
>>>>>>> refs/remotes/origin/master
}

