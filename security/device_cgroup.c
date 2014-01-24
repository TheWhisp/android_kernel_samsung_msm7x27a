/*
 * device_cgroup.c - device cgroup subsystem
 *
 * Copyright 2007 IBM Corp
 */

#include <linux/device_cgroup.h>
#include <linux/cgroup.h>
#include <linux/ctype.h>
#include <linux/list.h>
#include <linux/uaccess.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/rcupdate.h>
#include <linux/mutex.h>

#define ACC_MKNOD 1
#define ACC_READ  2
#define ACC_WRITE 4
#define ACC_MASK (ACC_MKNOD | ACC_READ | ACC_WRITE)

#define DEV_BLOCK 1
#define DEV_CHAR  2
#define DEV_ALL   4  /* this represents all devices */

static DEFINE_MUTEX(devcgroup_mutex);

<<<<<<< HEAD
/*
 * whitelist locking rules:
=======
enum devcg_behavior {
	DEVCG_DEFAULT_NONE,
	DEVCG_DEFAULT_ALLOW,
	DEVCG_DEFAULT_DENY,
};

/*
 * exception list locking rules:
>>>>>>> refs/remotes/origin/master
 * hold devcgroup_mutex for update/read.
 * hold rcu_read_lock() for read.
 */

<<<<<<< HEAD
struct dev_whitelist_item {
=======
struct dev_exception_item {
>>>>>>> refs/remotes/origin/master
	u32 major, minor;
	short type;
	short access;
	struct list_head list;
	struct rcu_head rcu;
};

struct dev_cgroup {
	struct cgroup_subsys_state css;
<<<<<<< HEAD
	struct list_head whitelist;
=======
	struct list_head exceptions;
	enum devcg_behavior behavior;
>>>>>>> refs/remotes/origin/master
};

static inline struct dev_cgroup *css_to_devcgroup(struct cgroup_subsys_state *s)
{
<<<<<<< HEAD
	return container_of(s, struct dev_cgroup, css);
}

static inline struct dev_cgroup *cgroup_to_devcgroup(struct cgroup *cgroup)
{
	return css_to_devcgroup(cgroup_subsys_state(cgroup, devices_subsys_id));
=======
	return s ? container_of(s, struct dev_cgroup, css) : NULL;
>>>>>>> refs/remotes/origin/master
}

static inline struct dev_cgroup *task_devcgroup(struct task_struct *task)
{
<<<<<<< HEAD
	return css_to_devcgroup(task_subsys_state(task, devices_subsys_id));
=======
	return css_to_devcgroup(task_css(task, devices_subsys_id));
>>>>>>> refs/remotes/origin/master
}

struct cgroup_subsys devices_subsys;

<<<<<<< HEAD
<<<<<<< HEAD
static int devcgroup_can_attach(struct cgroup_subsys *ss,
		struct cgroup *new_cgroup, struct task_struct *task)
{
	if (current != task && !capable(CAP_SYS_ADMIN))
			return -EPERM;

=======
static int devcgroup_can_attach(struct cgroup *new_cgrp,
				struct cgroup_taskset *set)
{
	struct task_struct *task = cgroup_taskset_first(set);

	if (current != task && !capable(CAP_SYS_ADMIN))
		return -EPERM;
>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
}

/*
 * called under devcgroup_mutex
 */
static int dev_whitelist_copy(struct list_head *dest, struct list_head *orig)
{
	struct dev_whitelist_item *wh, *tmp, *new;

	list_for_each_entry(wh, orig, list) {
		new = kmemdup(wh, sizeof(*wh), GFP_KERNEL);
=======
/*
 * called under devcgroup_mutex
 */
static int dev_exceptions_copy(struct list_head *dest, struct list_head *orig)
{
	struct dev_exception_item *ex, *tmp, *new;

	lockdep_assert_held(&devcgroup_mutex);

	list_for_each_entry(ex, orig, list) {
		new = kmemdup(ex, sizeof(*ex), GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
		if (!new)
			goto free_and_exit;
		list_add_tail(&new->list, dest);
	}

	return 0;

free_and_exit:
<<<<<<< HEAD
	list_for_each_entry_safe(wh, tmp, dest, list) {
		list_del(&wh->list);
		kfree(wh);
=======
	list_for_each_entry_safe(ex, tmp, dest, list) {
		list_del(&ex->list);
		kfree(ex);
>>>>>>> refs/remotes/origin/master
	}
	return -ENOMEM;
}

<<<<<<< HEAD
/* Stupid prototype - don't bother combining existing entries */
/*
 * called under devcgroup_mutex
 */
static int dev_whitelist_add(struct dev_cgroup *dev_cgroup,
			struct dev_whitelist_item *wh)
{
	struct dev_whitelist_item *whcopy, *walk;

	whcopy = kmemdup(wh, sizeof(*wh), GFP_KERNEL);
	if (!whcopy)
		return -ENOMEM;

	list_for_each_entry(walk, &dev_cgroup->whitelist, list) {
		if (walk->type != wh->type)
			continue;
		if (walk->major != wh->major)
			continue;
		if (walk->minor != wh->minor)
			continue;

		walk->access |= wh->access;
		kfree(whcopy);
		whcopy = NULL;
	}

	if (whcopy != NULL)
		list_add_tail_rcu(&whcopy->list, &dev_cgroup->whitelist);
	return 0;
}

<<<<<<< HEAD
static void whitelist_item_free(struct rcu_head *rcu)
{
	struct dev_whitelist_item *item;

	item = container_of(rcu, struct dev_whitelist_item, rcu);
	kfree(item);
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
/*
 * called under devcgroup_mutex
 */
static void dev_whitelist_rm(struct dev_cgroup *dev_cgroup,
			struct dev_whitelist_item *wh)
{
	struct dev_whitelist_item *walk, *tmp;

	list_for_each_entry_safe(walk, tmp, &dev_cgroup->whitelist, list) {
		if (walk->type == DEV_ALL)
			goto remove;
		if (walk->type != wh->type)
			continue;
		if (walk->major != ~0 && walk->major != wh->major)
			continue;
		if (walk->minor != ~0 && walk->minor != wh->minor)
			continue;

remove:
		walk->access &= ~wh->access;
		if (!walk->access) {
			list_del_rcu(&walk->list);
<<<<<<< HEAD
			call_rcu(&walk->rcu, whitelist_item_free);
=======
			kfree_rcu(walk, rcu);
>>>>>>> refs/remotes/origin/cm-10.0
=======
/*
 * called under devcgroup_mutex
 */
static int dev_exception_add(struct dev_cgroup *dev_cgroup,
			     struct dev_exception_item *ex)
{
	struct dev_exception_item *excopy, *walk;

	lockdep_assert_held(&devcgroup_mutex);

	excopy = kmemdup(ex, sizeof(*ex), GFP_KERNEL);
	if (!excopy)
		return -ENOMEM;

	list_for_each_entry(walk, &dev_cgroup->exceptions, list) {
		if (walk->type != ex->type)
			continue;
		if (walk->major != ex->major)
			continue;
		if (walk->minor != ex->minor)
			continue;

		walk->access |= ex->access;
		kfree(excopy);
		excopy = NULL;
	}

	if (excopy != NULL)
		list_add_tail_rcu(&excopy->list, &dev_cgroup->exceptions);
	return 0;
}

/*
 * called under devcgroup_mutex
 */
static void dev_exception_rm(struct dev_cgroup *dev_cgroup,
			     struct dev_exception_item *ex)
{
	struct dev_exception_item *walk, *tmp;

	lockdep_assert_held(&devcgroup_mutex);

	list_for_each_entry_safe(walk, tmp, &dev_cgroup->exceptions, list) {
		if (walk->type != ex->type)
			continue;
		if (walk->major != ex->major)
			continue;
		if (walk->minor != ex->minor)
			continue;

		walk->access &= ~ex->access;
		if (!walk->access) {
			list_del_rcu(&walk->list);
			kfree_rcu(walk, rcu);
>>>>>>> refs/remotes/origin/master
		}
	}
}

<<<<<<< HEAD
/*
 * called from kernel/cgroup.c with cgroup_lock() held.
 */
<<<<<<< HEAD
static struct cgroup_subsys_state *devcgroup_create(struct cgroup_subsys *ss,
						struct cgroup *cgroup)
=======
static struct cgroup_subsys_state *devcgroup_create(struct cgroup *cgroup)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct dev_cgroup *dev_cgroup, *parent_dev_cgroup;
	struct cgroup *parent_cgroup;
	int ret;
=======
static void __dev_exception_clean(struct dev_cgroup *dev_cgroup)
{
	struct dev_exception_item *ex, *tmp;

	list_for_each_entry_safe(ex, tmp, &dev_cgroup->exceptions, list) {
		list_del_rcu(&ex->list);
		kfree_rcu(ex, rcu);
	}
}

/**
 * dev_exception_clean - frees all entries of the exception list
 * @dev_cgroup: dev_cgroup with the exception list to be cleaned
 *
 * called under devcgroup_mutex
 */
static void dev_exception_clean(struct dev_cgroup *dev_cgroup)
{
	lockdep_assert_held(&devcgroup_mutex);

	__dev_exception_clean(dev_cgroup);
}

static inline bool is_devcg_online(const struct dev_cgroup *devcg)
{
	return (devcg->behavior != DEVCG_DEFAULT_NONE);
}

/**
 * devcgroup_online - initializes devcgroup's behavior and exceptions based on
 * 		      parent's
 * @css: css getting online
 * returns 0 in case of success, error code otherwise
 */
static int devcgroup_online(struct cgroup_subsys_state *css)
{
	struct dev_cgroup *dev_cgroup = css_to_devcgroup(css);
	struct dev_cgroup *parent_dev_cgroup = css_to_devcgroup(css_parent(css));
	int ret = 0;

	mutex_lock(&devcgroup_mutex);

	if (parent_dev_cgroup == NULL)
		dev_cgroup->behavior = DEVCG_DEFAULT_ALLOW;
	else {
		ret = dev_exceptions_copy(&dev_cgroup->exceptions,
					  &parent_dev_cgroup->exceptions);
		if (!ret)
			dev_cgroup->behavior = parent_dev_cgroup->behavior;
	}
	mutex_unlock(&devcgroup_mutex);

	return ret;
}

static void devcgroup_offline(struct cgroup_subsys_state *css)
{
	struct dev_cgroup *dev_cgroup = css_to_devcgroup(css);

	mutex_lock(&devcgroup_mutex);
	dev_cgroup->behavior = DEVCG_DEFAULT_NONE;
	mutex_unlock(&devcgroup_mutex);
}

/*
 * called from kernel/cgroup.c with cgroup_lock() held.
 */
static struct cgroup_subsys_state *
devcgroup_css_alloc(struct cgroup_subsys_state *parent_css)
{
	struct dev_cgroup *dev_cgroup;
>>>>>>> refs/remotes/origin/master

	dev_cgroup = kzalloc(sizeof(*dev_cgroup), GFP_KERNEL);
	if (!dev_cgroup)
		return ERR_PTR(-ENOMEM);
<<<<<<< HEAD
	INIT_LIST_HEAD(&dev_cgroup->whitelist);
	parent_cgroup = cgroup->parent;

	if (parent_cgroup == NULL) {
		struct dev_whitelist_item *wh;
		wh = kmalloc(sizeof(*wh), GFP_KERNEL);
		if (!wh) {
			kfree(dev_cgroup);
			return ERR_PTR(-ENOMEM);
		}
		wh->minor = wh->major = ~0;
		wh->type = DEV_ALL;
		wh->access = ACC_MASK;
		list_add(&wh->list, &dev_cgroup->whitelist);
	} else {
		parent_dev_cgroup = cgroup_to_devcgroup(parent_cgroup);
		mutex_lock(&devcgroup_mutex);
		ret = dev_whitelist_copy(&dev_cgroup->whitelist,
				&parent_dev_cgroup->whitelist);
		mutex_unlock(&devcgroup_mutex);
		if (ret) {
			kfree(dev_cgroup);
			return ERR_PTR(ret);
		}
	}
=======
	INIT_LIST_HEAD(&dev_cgroup->exceptions);
	dev_cgroup->behavior = DEVCG_DEFAULT_NONE;
>>>>>>> refs/remotes/origin/master

	return &dev_cgroup->css;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void devcgroup_destroy(struct cgroup_subsys *ss,
			struct cgroup *cgroup)
=======
static void devcgroup_destroy(struct cgroup *cgroup)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct dev_cgroup *dev_cgroup;
	struct dev_whitelist_item *wh, *tmp;

	dev_cgroup = cgroup_to_devcgroup(cgroup);
	list_for_each_entry_safe(wh, tmp, &dev_cgroup->whitelist, list) {
		list_del(&wh->list);
		kfree(wh);
	}
=======
static void devcgroup_css_free(struct cgroup_subsys_state *css)
{
	struct dev_cgroup *dev_cgroup = css_to_devcgroup(css);

	__dev_exception_clean(dev_cgroup);
>>>>>>> refs/remotes/origin/master
	kfree(dev_cgroup);
}

#define DEVCG_ALLOW 1
#define DEVCG_DENY 2
#define DEVCG_LIST 3

#define MAJMINLEN 13
#define ACCLEN 4

static void set_access(char *acc, short access)
{
	int idx = 0;
	memset(acc, 0, ACCLEN);
	if (access & ACC_READ)
		acc[idx++] = 'r';
	if (access & ACC_WRITE)
		acc[idx++] = 'w';
	if (access & ACC_MKNOD)
		acc[idx++] = 'm';
}

static char type_to_char(short type)
{
	if (type == DEV_ALL)
		return 'a';
	if (type == DEV_CHAR)
		return 'c';
	if (type == DEV_BLOCK)
		return 'b';
	return 'X';
}

static void set_majmin(char *str, unsigned m)
{
	if (m == ~0)
		strcpy(str, "*");
	else
		sprintf(str, "%u", m);
}

<<<<<<< HEAD
static int devcgroup_seq_read(struct cgroup *cgroup, struct cftype *cft,
				struct seq_file *m)
{
	struct dev_cgroup *devcgroup = cgroup_to_devcgroup(cgroup);
	struct dev_whitelist_item *wh;
	char maj[MAJMINLEN], min[MAJMINLEN], acc[ACCLEN];

	rcu_read_lock();
	list_for_each_entry_rcu(wh, &devcgroup->whitelist, list) {
		set_access(acc, wh->access);
		set_majmin(maj, wh->major);
		set_majmin(min, wh->minor);
		seq_printf(m, "%c %s:%s %s\n", type_to_char(wh->type),
			   maj, min, acc);
=======
static int devcgroup_seq_show(struct seq_file *m, void *v)
{
	struct dev_cgroup *devcgroup = css_to_devcgroup(seq_css(m));
	struct dev_exception_item *ex;
	char maj[MAJMINLEN], min[MAJMINLEN], acc[ACCLEN];

	rcu_read_lock();
	/*
	 * To preserve the compatibility:
	 * - Only show the "all devices" when the default policy is to allow
	 * - List the exceptions in case the default policy is to deny
	 * This way, the file remains as a "whitelist of devices"
	 */
	if (devcgroup->behavior == DEVCG_DEFAULT_ALLOW) {
		set_access(acc, ACC_MASK);
		set_majmin(maj, ~0);
		set_majmin(min, ~0);
		seq_printf(m, "%c %s:%s %s\n", type_to_char(DEV_ALL),
			   maj, min, acc);
	} else {
		list_for_each_entry_rcu(ex, &devcgroup->exceptions, list) {
			set_access(acc, ex->access);
			set_majmin(maj, ex->major);
			set_majmin(min, ex->minor);
			seq_printf(m, "%c %s:%s %s\n", type_to_char(ex->type),
				   maj, min, acc);
		}
>>>>>>> refs/remotes/origin/master
	}
	rcu_read_unlock();

	return 0;
}

<<<<<<< HEAD
/*
 * may_access_whitelist:
 * does the access granted to dev_cgroup c contain the access
 * requested in whitelist item refwh.
 * return 1 if yes, 0 if no.
 * call with devcgroup_mutex held
 */
static int may_access_whitelist(struct dev_cgroup *c,
				       struct dev_whitelist_item *refwh)
{
	struct dev_whitelist_item *whitem;

	list_for_each_entry(whitem, &c->whitelist, list) {
		if (whitem->type & DEV_ALL)
			return 1;
		if ((refwh->type & DEV_BLOCK) && !(whitem->type & DEV_BLOCK))
			continue;
		if ((refwh->type & DEV_CHAR) && !(whitem->type & DEV_CHAR))
			continue;
		if (whitem->major != ~0 && whitem->major != refwh->major)
			continue;
		if (whitem->minor != ~0 && whitem->minor != refwh->minor)
			continue;
		if (refwh->access & (~whitem->access))
			continue;
		return 1;
	}
	return 0;
=======
/**
 * may_access - verifies if a new exception is part of what is allowed
 *		by a dev cgroup based on the default policy +
 *		exceptions. This is used to make sure a child cgroup
 *		won't have more privileges than its parent or to
 *		verify if a certain access is allowed.
 * @dev_cgroup: dev cgroup to be tested against
 * @refex: new exception
 * @behavior: behavior of the exception
 */
static bool may_access(struct dev_cgroup *dev_cgroup,
		       struct dev_exception_item *refex,
		       enum devcg_behavior behavior)
{
	struct dev_exception_item *ex;
	bool match = false;

	rcu_lockdep_assert(rcu_read_lock_held() ||
			   lockdep_is_held(&devcgroup_mutex),
			   "device_cgroup::may_access() called without proper synchronization");

	list_for_each_entry_rcu(ex, &dev_cgroup->exceptions, list) {
		if ((refex->type & DEV_BLOCK) && !(ex->type & DEV_BLOCK))
			continue;
		if ((refex->type & DEV_CHAR) && !(ex->type & DEV_CHAR))
			continue;
		if (ex->major != ~0 && ex->major != refex->major)
			continue;
		if (ex->minor != ~0 && ex->minor != refex->minor)
			continue;
		if (refex->access & (~ex->access))
			continue;
		match = true;
		break;
	}

	if (dev_cgroup->behavior == DEVCG_DEFAULT_ALLOW) {
		if (behavior == DEVCG_DEFAULT_ALLOW) {
			/* the exception will deny access to certain devices */
			return true;
		} else {
			/* the exception will allow access to certain devices */
			if (match)
				/*
				 * a new exception allowing access shouldn't
				 * match an parent's exception
				 */
				return false;
			return true;
		}
	} else {
		/* only behavior == DEVCG_DEFAULT_DENY allowed here */
		if (match)
			/* parent has an exception that matches the proposed */
			return true;
		else
			return false;
	}
	return false;
>>>>>>> refs/remotes/origin/master
}

/*
 * parent_has_perm:
<<<<<<< HEAD
 * when adding a new allow rule to a device whitelist, the rule
 * must be allowed in the parent device
 */
static int parent_has_perm(struct dev_cgroup *childcg,
				  struct dev_whitelist_item *wh)
{
	struct cgroup *pcg = childcg->css.cgroup->parent;
	struct dev_cgroup *parent;

	if (!pcg)
		return 1;
	parent = cgroup_to_devcgroup(pcg);
	return may_access_whitelist(parent, wh);
}

/*
 * Modify the whitelist using allow/deny rules.
 * CAP_SYS_ADMIN is needed for this.  It's at least separate from CAP_MKNOD
 * so we can give a container CAP_MKNOD to let it create devices but not
 * modify the whitelist.
 * It seems likely we'll want to add a CAP_CONTAINER capability to allow
 * us to also grant CAP_SYS_ADMIN to containers without giving away the
 * device whitelist controls, but for now we'll stick with CAP_SYS_ADMIN
=======
 * when adding a new allow rule to a device exception list, the rule
 * must be allowed in the parent device
 */
static int parent_has_perm(struct dev_cgroup *childcg,
				  struct dev_exception_item *ex)
{
	struct dev_cgroup *parent = css_to_devcgroup(css_parent(&childcg->css));

	if (!parent)
		return 1;
	return may_access(parent, ex, childcg->behavior);
}

/**
 * may_allow_all - checks if it's possible to change the behavior to
 *		   allow based on parent's rules.
 * @parent: device cgroup's parent
 * returns: != 0 in case it's allowed, 0 otherwise
 */
static inline int may_allow_all(struct dev_cgroup *parent)
{
	if (!parent)
		return 1;
	return parent->behavior == DEVCG_DEFAULT_ALLOW;
}

/**
 * revalidate_active_exceptions - walks through the active exception list and
 * 				  revalidates the exceptions based on parent's
 * 				  behavior and exceptions. The exceptions that
 * 				  are no longer valid will be removed.
 * 				  Called with devcgroup_mutex held.
 * @devcg: cgroup which exceptions will be checked
 *
 * This is one of the three key functions for hierarchy implementation.
 * This function is responsible for re-evaluating all the cgroup's active
 * exceptions due to a parent's exception change.
 * Refer to Documentation/cgroups/devices.txt for more details.
 */
static void revalidate_active_exceptions(struct dev_cgroup *devcg)
{
	struct dev_exception_item *ex;
	struct list_head *this, *tmp;

	list_for_each_safe(this, tmp, &devcg->exceptions) {
		ex = container_of(this, struct dev_exception_item, list);
		if (!parent_has_perm(devcg, ex))
			dev_exception_rm(devcg, ex);
	}
}

/**
 * propagate_exception - propagates a new exception to the children
 * @devcg_root: device cgroup that added a new exception
 * @ex: new exception to be propagated
 *
 * returns: 0 in case of success, != 0 in case of error
 */
static int propagate_exception(struct dev_cgroup *devcg_root,
			       struct dev_exception_item *ex)
{
	struct cgroup_subsys_state *pos;
	int rc = 0;

	rcu_read_lock();

	css_for_each_descendant_pre(pos, &devcg_root->css) {
		struct dev_cgroup *devcg = css_to_devcgroup(pos);

		/*
		 * Because devcgroup_mutex is held, no devcg will become
		 * online or offline during the tree walk (see on/offline
		 * methods), and online ones are safe to access outside RCU
		 * read lock without bumping refcnt.
		 */
		if (pos == &devcg_root->css || !is_devcg_online(devcg))
			continue;

		rcu_read_unlock();

		/*
		 * in case both root's behavior and devcg is allow, a new
		 * restriction means adding to the exception list
		 */
		if (devcg_root->behavior == DEVCG_DEFAULT_ALLOW &&
		    devcg->behavior == DEVCG_DEFAULT_ALLOW) {
			rc = dev_exception_add(devcg, ex);
			if (rc)
				break;
		} else {
			/*
			 * in the other possible cases:
			 * root's behavior: allow, devcg's: deny
			 * root's behavior: deny, devcg's: deny
			 * the exception will be removed
			 */
			dev_exception_rm(devcg, ex);
		}
		revalidate_active_exceptions(devcg);

		rcu_read_lock();
	}

	rcu_read_unlock();
	return rc;
}

static inline bool has_children(struct dev_cgroup *devcgroup)
{
	struct cgroup *cgrp = devcgroup->css.cgroup;

	return !list_empty(&cgrp->children);
}

/*
 * Modify the exception list using allow/deny rules.
 * CAP_SYS_ADMIN is needed for this.  It's at least separate from CAP_MKNOD
 * so we can give a container CAP_MKNOD to let it create devices but not
 * modify the exception list.
 * It seems likely we'll want to add a CAP_CONTAINER capability to allow
 * us to also grant CAP_SYS_ADMIN to containers without giving away the
 * device exception list controls, but for now we'll stick with CAP_SYS_ADMIN
>>>>>>> refs/remotes/origin/master
 *
 * Taking rules away is always allowed (given CAP_SYS_ADMIN).  Granting
 * new access is only allowed if you're in the top-level cgroup, or your
 * parent cgroup has the access you're asking for.
 */
static int devcgroup_update_access(struct dev_cgroup *devcgroup,
				   int filetype, const char *buffer)
{
	const char *b;
<<<<<<< HEAD
	char *endp;
	int count;
	struct dev_whitelist_item wh;
=======
	char temp[12];		/* 11 + 1 characters needed for a u32 */
	int count, rc = 0;
	struct dev_exception_item ex;
	struct dev_cgroup *parent = css_to_devcgroup(css_parent(&devcgroup->css));
>>>>>>> refs/remotes/origin/master

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

<<<<<<< HEAD
	memset(&wh, 0, sizeof(wh));
=======
	memset(&ex, 0, sizeof(ex));
>>>>>>> refs/remotes/origin/master
	b = buffer;

	switch (*b) {
	case 'a':
<<<<<<< HEAD
		wh.type = DEV_ALL;
		wh.access = ACC_MASK;
		wh.major = ~0;
		wh.minor = ~0;
		goto handle;
	case 'b':
		wh.type = DEV_BLOCK;
		break;
	case 'c':
		wh.type = DEV_CHAR;
=======
		switch (filetype) {
		case DEVCG_ALLOW:
			if (has_children(devcgroup))
				return -EINVAL;

			if (!may_allow_all(parent))
				return -EPERM;
			dev_exception_clean(devcgroup);
			devcgroup->behavior = DEVCG_DEFAULT_ALLOW;
			if (!parent)
				break;

			rc = dev_exceptions_copy(&devcgroup->exceptions,
						 &parent->exceptions);
			if (rc)
				return rc;
			break;
		case DEVCG_DENY:
			if (has_children(devcgroup))
				return -EINVAL;

			dev_exception_clean(devcgroup);
			devcgroup->behavior = DEVCG_DEFAULT_DENY;
			break;
		default:
			return -EINVAL;
		}
		return 0;
	case 'b':
		ex.type = DEV_BLOCK;
		break;
	case 'c':
		ex.type = DEV_CHAR;
>>>>>>> refs/remotes/origin/master
		break;
	default:
		return -EINVAL;
	}
	b++;
	if (!isspace(*b))
		return -EINVAL;
	b++;
	if (*b == '*') {
<<<<<<< HEAD
		wh.major = ~0;
		b++;
	} else if (isdigit(*b)) {
		wh.major = simple_strtoul(b, &endp, 10);
		b = endp;
=======
		ex.major = ~0;
		b++;
	} else if (isdigit(*b)) {
		memset(temp, 0, sizeof(temp));
		for (count = 0; count < sizeof(temp) - 1; count++) {
			temp[count] = *b;
			b++;
			if (!isdigit(*b))
				break;
		}
		rc = kstrtou32(temp, 10, &ex.major);
		if (rc)
			return -EINVAL;
>>>>>>> refs/remotes/origin/master
	} else {
		return -EINVAL;
	}
	if (*b != ':')
		return -EINVAL;
	b++;

	/* read minor */
	if (*b == '*') {
<<<<<<< HEAD
		wh.minor = ~0;
		b++;
	} else if (isdigit(*b)) {
		wh.minor = simple_strtoul(b, &endp, 10);
		b = endp;
=======
		ex.minor = ~0;
		b++;
	} else if (isdigit(*b)) {
		memset(temp, 0, sizeof(temp));
		for (count = 0; count < sizeof(temp) - 1; count++) {
			temp[count] = *b;
			b++;
			if (!isdigit(*b))
				break;
		}
		rc = kstrtou32(temp, 10, &ex.minor);
		if (rc)
			return -EINVAL;
>>>>>>> refs/remotes/origin/master
	} else {
		return -EINVAL;
	}
	if (!isspace(*b))
		return -EINVAL;
	for (b++, count = 0; count < 3; count++, b++) {
		switch (*b) {
		case 'r':
<<<<<<< HEAD
			wh.access |= ACC_READ;
			break;
		case 'w':
			wh.access |= ACC_WRITE;
			break;
		case 'm':
			wh.access |= ACC_MKNOD;
=======
			ex.access |= ACC_READ;
			break;
		case 'w':
			ex.access |= ACC_WRITE;
			break;
		case 'm':
			ex.access |= ACC_MKNOD;
>>>>>>> refs/remotes/origin/master
			break;
		case '\n':
		case '\0':
			count = 3;
			break;
		default:
			return -EINVAL;
		}
	}

<<<<<<< HEAD
handle:
	switch (filetype) {
	case DEVCG_ALLOW:
		if (!parent_has_perm(devcgroup, &wh))
			return -EPERM;
		return dev_whitelist_add(devcgroup, &wh);
	case DEVCG_DENY:
		dev_whitelist_rm(devcgroup, &wh);
		break;
	default:
		return -EINVAL;
	}
	return 0;
}

static int devcgroup_access_write(struct cgroup *cgrp, struct cftype *cft,
				  const char *buffer)
=======
	switch (filetype) {
	case DEVCG_ALLOW:
		if (!parent_has_perm(devcgroup, &ex))
			return -EPERM;
		/*
		 * If the default policy is to allow by default, try to remove
		 * an matching exception instead. And be silent about it: we
		 * don't want to break compatibility
		 */
		if (devcgroup->behavior == DEVCG_DEFAULT_ALLOW) {
			dev_exception_rm(devcgroup, &ex);
			return 0;
		}
		rc = dev_exception_add(devcgroup, &ex);
		break;
	case DEVCG_DENY:
		/*
		 * If the default policy is to deny by default, try to remove
		 * an matching exception instead. And be silent about it: we
		 * don't want to break compatibility
		 */
		if (devcgroup->behavior == DEVCG_DEFAULT_DENY)
			dev_exception_rm(devcgroup, &ex);
		else
			rc = dev_exception_add(devcgroup, &ex);

		if (rc)
			break;
		/* we only propagate new restrictions */
		rc = propagate_exception(devcgroup, &ex);
		break;
	default:
		rc = -EINVAL;
	}
	return rc;
}

static int devcgroup_access_write(struct cgroup_subsys_state *css,
				  struct cftype *cft, const char *buffer)
>>>>>>> refs/remotes/origin/master
{
	int retval;

	mutex_lock(&devcgroup_mutex);
<<<<<<< HEAD
	retval = devcgroup_update_access(cgroup_to_devcgroup(cgrp),
=======
	retval = devcgroup_update_access(css_to_devcgroup(css),
>>>>>>> refs/remotes/origin/master
					 cft->private, buffer);
	mutex_unlock(&devcgroup_mutex);
	return retval;
}

static struct cftype dev_cgroup_files[] = {
	{
		.name = "allow",
		.write_string  = devcgroup_access_write,
		.private = DEVCG_ALLOW,
	},
	{
		.name = "deny",
		.write_string = devcgroup_access_write,
		.private = DEVCG_DENY,
	},
	{
		.name = "list",
<<<<<<< HEAD
		.read_seq_string = devcgroup_seq_read,
		.private = DEVCG_LIST,
	},
};

static int devcgroup_populate(struct cgroup_subsys *ss,
				struct cgroup *cgroup)
{
	return cgroup_add_files(cgroup, ss, dev_cgroup_files,
					ARRAY_SIZE(dev_cgroup_files));
}

struct cgroup_subsys devices_subsys = {
	.name = "devices",
	.can_attach = devcgroup_can_attach,
	.create = devcgroup_create,
	.destroy = devcgroup_destroy,
	.populate = devcgroup_populate,
	.subsys_id = devices_subsys_id,
};

int __devcgroup_inode_permission(struct inode *inode, int mask)
{
	struct dev_cgroup *dev_cgroup;
	struct dev_whitelist_item *wh;

	rcu_read_lock();

	dev_cgroup = task_devcgroup(current);

	list_for_each_entry_rcu(wh, &dev_cgroup->whitelist, list) {
		if (wh->type & DEV_ALL)
			goto found;
		if ((wh->type & DEV_BLOCK) && !S_ISBLK(inode->i_mode))
			continue;
		if ((wh->type & DEV_CHAR) && !S_ISCHR(inode->i_mode))
			continue;
		if (wh->major != ~0 && wh->major != imajor(inode))
			continue;
		if (wh->minor != ~0 && wh->minor != iminor(inode))
			continue;

		if ((mask & MAY_WRITE) && !(wh->access & ACC_WRITE))
			continue;
		if ((mask & MAY_READ) && !(wh->access & ACC_READ))
			continue;
found:
		rcu_read_unlock();
		return 0;
	}

	rcu_read_unlock();

	return -EPERM;
=======
		.seq_show = devcgroup_seq_show,
		.private = DEVCG_LIST,
	},
	{ }	/* terminate */
};

struct cgroup_subsys devices_subsys = {
	.name = "devices",
	.css_alloc = devcgroup_css_alloc,
	.css_free = devcgroup_css_free,
	.css_online = devcgroup_online,
	.css_offline = devcgroup_offline,
	.subsys_id = devices_subsys_id,
	.base_cftypes = dev_cgroup_files,
};

/**
 * __devcgroup_check_permission - checks if an inode operation is permitted
 * @dev_cgroup: the dev cgroup to be tested against
 * @type: device type
 * @major: device major number
 * @minor: device minor number
 * @access: combination of ACC_WRITE, ACC_READ and ACC_MKNOD
 *
 * returns 0 on success, -EPERM case the operation is not permitted
 */
static int __devcgroup_check_permission(short type, u32 major, u32 minor,
				        short access)
{
	struct dev_cgroup *dev_cgroup;
	struct dev_exception_item ex;
	int rc;

	memset(&ex, 0, sizeof(ex));
	ex.type = type;
	ex.major = major;
	ex.minor = minor;
	ex.access = access;

	rcu_read_lock();
	dev_cgroup = task_devcgroup(current);
	rc = may_access(dev_cgroup, &ex, dev_cgroup->behavior);
	rcu_read_unlock();

	if (!rc)
		return -EPERM;

	return 0;
}

int __devcgroup_inode_permission(struct inode *inode, int mask)
{
	short type, access = 0;

	if (S_ISBLK(inode->i_mode))
		type = DEV_BLOCK;
	if (S_ISCHR(inode->i_mode))
		type = DEV_CHAR;
	if (mask & MAY_WRITE)
		access |= ACC_WRITE;
	if (mask & MAY_READ)
		access |= ACC_READ;

	return __devcgroup_check_permission(type, imajor(inode), iminor(inode),
			access);
>>>>>>> refs/remotes/origin/master
}

int devcgroup_inode_mknod(int mode, dev_t dev)
{
<<<<<<< HEAD
	struct dev_cgroup *dev_cgroup;
	struct dev_whitelist_item *wh;
=======
	short type;
>>>>>>> refs/remotes/origin/master

	if (!S_ISBLK(mode) && !S_ISCHR(mode))
		return 0;

<<<<<<< HEAD
	rcu_read_lock();

	dev_cgroup = task_devcgroup(current);

	list_for_each_entry_rcu(wh, &dev_cgroup->whitelist, list) {
		if (wh->type & DEV_ALL)
			goto found;
		if ((wh->type & DEV_BLOCK) && !S_ISBLK(mode))
			continue;
		if ((wh->type & DEV_CHAR) && !S_ISCHR(mode))
			continue;
		if (wh->major != ~0 && wh->major != MAJOR(dev))
			continue;
		if (wh->minor != ~0 && wh->minor != MINOR(dev))
			continue;

		if (!(wh->access & ACC_MKNOD))
			continue;
found:
		rcu_read_unlock();
		return 0;
	}

	rcu_read_unlock();

	return -EPERM;
=======
	if (S_ISBLK(mode))
		type = DEV_BLOCK;
	else
		type = DEV_CHAR;

	return __devcgroup_check_permission(type, MAJOR(dev), MINOR(dev),
			ACC_MKNOD);

>>>>>>> refs/remotes/origin/master
}
