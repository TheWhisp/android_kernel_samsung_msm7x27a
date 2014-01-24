/*
 * fs/inotify_user.c - inotify support for userspace
 *
 * Authors:
 *	John McCutchan	<ttb@tentacle.dhs.org>
 *	Robert Love	<rml@novell.com>
 *
 * Copyright (C) 2005 John McCutchan
 * Copyright 2006 Hewlett-Packard Development Company, L.P.
 *
 * Copyright (C) 2009 Eric Paris <Red Hat Inc>
 * inotify was largely rewriten to make use of the fsnotify infrastructure
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 */

#include <linux/dcache.h> /* d_unlinked */
#include <linux/fs.h> /* struct inode */
#include <linux/fsnotify_backend.h>
#include <linux/inotify.h>
#include <linux/path.h> /* struct path */
#include <linux/slab.h> /* kmem_* */
#include <linux/types.h>
#include <linux/sched.h>

#include "inotify.h"

/*
<<<<<<< HEAD
 * Check if 2 events contain the same information.  We do not compare private data
 * but at this moment that isn't a problem for any know fsnotify listeners.
 */
static bool event_compare(struct fsnotify_event *old, struct fsnotify_event *new)
{
	if ((old->mask == new->mask) &&
	    (old->to_tell == new->to_tell) &&
	    (old->data_type == new->data_type) &&
	    (old->name_len == new->name_len)) {
		switch (old->data_type) {
		case (FSNOTIFY_EVENT_INODE):
			/* remember, after old was put on the wait_q we aren't
			 * allowed to look at the inode any more, only thing
			 * left to check was if the file_name is the same */
			if (!old->name_len ||
			    !strcmp(old->file_name, new->file_name))
				return true;
			break;
		case (FSNOTIFY_EVENT_PATH):
			if ((old->path.mnt == new->path.mnt) &&
			    (old->path.dentry == new->path.dentry))
				return true;
			break;
		case (FSNOTIFY_EVENT_NONE):
			if (old->mask & FS_Q_OVERFLOW)
				return true;
			else if (old->mask & FS_IN_IGNORED)
				return false;
			return true;
		};
	}
=======
 * Check if 2 events contain the same information.
 */
static bool event_compare(struct fsnotify_event *old_fsn,
			  struct fsnotify_event *new_fsn)
{
	struct inotify_event_info *old, *new;

	if (old_fsn->mask & FS_IN_IGNORED)
		return false;
	old = INOTIFY_E(old_fsn);
	new = INOTIFY_E(new_fsn);
	if ((old_fsn->mask == new_fsn->mask) &&
	    (old_fsn->inode == new_fsn->inode) &&
	    (old->name_len == new->name_len) &&
	    (!old->name_len || !strcmp(old->name, new->name)))
		return true;
>>>>>>> refs/remotes/origin/master
	return false;
}

static struct fsnotify_event *inotify_merge(struct list_head *list,
					    struct fsnotify_event *event)
{
<<<<<<< HEAD
	struct fsnotify_event_holder *last_holder;
	struct fsnotify_event *last_event;

	/* and the list better be locked by something too */
	spin_lock(&event->lock);

	last_holder = list_entry(list->prev, struct fsnotify_event_holder, event_list);
	last_event = last_holder->event;
	if (event_compare(last_event, event))
		fsnotify_get_event(last_event);
	else
		last_event = NULL;

	spin_unlock(&event->lock);

	return last_event;
}

static int inotify_handle_event(struct fsnotify_group *group,
				struct fsnotify_mark *inode_mark,
				struct fsnotify_mark *vfsmount_mark,
				struct fsnotify_event *event)
{
	struct inotify_inode_mark *i_mark;
	struct inode *to_tell;
	struct inotify_event_private_data *event_priv;
	struct fsnotify_event_private_data *fsn_event_priv;
	struct fsnotify_event *added_event;
	int wd, ret = 0;

	BUG_ON(vfsmount_mark);

	pr_debug("%s: group=%p event=%p to_tell=%p mask=%x\n", __func__, group,
		 event, event->to_tell, event->mask);

	to_tell = event->to_tell;

	i_mark = container_of(inode_mark, struct inotify_inode_mark,
			      fsn_mark);
	wd = i_mark->wd;

	event_priv = kmem_cache_alloc(event_priv_cachep, GFP_KERNEL);
	if (unlikely(!event_priv))
		return -ENOMEM;

	fsn_event_priv = &event_priv->fsnotify_event_priv_data;

	fsn_event_priv->group = group;
	event_priv->wd = wd;

	added_event = fsnotify_add_notify_event(group, event, fsn_event_priv, inotify_merge);
	if (added_event) {
		inotify_free_event_priv(fsn_event_priv);
		if (!IS_ERR(added_event))
			fsnotify_put_event(added_event);
		else
=======
	struct fsnotify_event *last_event;

	last_event = list_entry(list->prev, struct fsnotify_event, list);
	if (!event_compare(last_event, event))
		return NULL;
	return last_event;
}

int inotify_handle_event(struct fsnotify_group *group,
			 struct inode *inode,
			 struct fsnotify_mark *inode_mark,
			 struct fsnotify_mark *vfsmount_mark,
			 u32 mask, void *data, int data_type,
			 const unsigned char *file_name)
{
	struct inotify_inode_mark *i_mark;
	struct inotify_event_info *event;
	struct fsnotify_event *added_event;
	struct fsnotify_event *fsn_event;
	int ret = 0;
	int len = 0;
	int alloc_len = sizeof(struct inotify_event_info);

	BUG_ON(vfsmount_mark);

	if ((inode_mark->mask & FS_EXCL_UNLINK) &&
	    (data_type == FSNOTIFY_EVENT_PATH)) {
		struct path *path = data;

		if (d_unlinked(path->dentry))
			return 0;
	}
	if (file_name) {
		len = strlen(file_name);
		alloc_len += len + 1;
	}

	pr_debug("%s: group=%p inode=%p mask=%x\n", __func__, group, inode,
		 mask);

	i_mark = container_of(inode_mark, struct inotify_inode_mark,
			      fsn_mark);

	event = kmalloc(alloc_len, GFP_KERNEL);
	if (unlikely(!event))
		return -ENOMEM;

	fsn_event = &event->fse;
	fsnotify_init_event(fsn_event, inode, mask);
	event->wd = i_mark->wd;
	event->name_len = len;
	if (len)
		strcpy(event->name, file_name);

	added_event = fsnotify_add_notify_event(group, fsn_event, inotify_merge);
	if (added_event) {
		/* Our event wasn't used in the end. Free it. */
		fsnotify_destroy_event(group, fsn_event);
		if (IS_ERR(added_event))
>>>>>>> refs/remotes/origin/master
			ret = PTR_ERR(added_event);
	}

	if (inode_mark->mask & IN_ONESHOT)
<<<<<<< HEAD
		fsnotify_destroy_mark(inode_mark);
=======
		fsnotify_destroy_mark(inode_mark, group);
>>>>>>> refs/remotes/origin/master

	return ret;
}

static void inotify_freeing_mark(struct fsnotify_mark *fsn_mark, struct fsnotify_group *group)
{
	inotify_ignored_and_remove_idr(fsn_mark, group);
}

<<<<<<< HEAD
static bool inotify_should_send_event(struct fsnotify_group *group, struct inode *inode,
				      struct fsnotify_mark *inode_mark,
				      struct fsnotify_mark *vfsmount_mark,
				      __u32 mask, void *data, int data_type)
{
	if ((inode_mark->mask & FS_EXCL_UNLINK) &&
	    (data_type == FSNOTIFY_EVENT_PATH)) {
		struct path *path = data;

		if (d_unlinked(path->dentry))
			return false;
	}

	return true;
}

=======
>>>>>>> refs/remotes/origin/master
/*
 * This is NEVER supposed to be called.  Inotify marks should either have been
 * removed from the idr when the watch was removed or in the
 * fsnotify_destroy_mark_by_group() call when the inotify instance was being
 * torn down.  This is only called if the idr is about to be freed but there
 * are still marks in it.
 */
static int idr_callback(int id, void *p, void *data)
{
	struct fsnotify_mark *fsn_mark;
	struct inotify_inode_mark *i_mark;
	static bool warned = false;

	if (warned)
		return 0;

	warned = true;
	fsn_mark = p;
	i_mark = container_of(fsn_mark, struct inotify_inode_mark, fsn_mark);

	WARN(1, "inotify closing but id=%d for fsn_mark=%p in group=%p still in "
		"idr.  Probably leaking memory\n", id, p, data);

	/*
	 * I'm taking the liberty of assuming that the mark in question is a
	 * valid address and I'm dereferencing it.  This might help to figure
	 * out why we got here and the panic is no worse than the original
	 * BUG() that was here.
	 */
	if (fsn_mark)
		printk(KERN_WARNING "fsn_mark->group=%p inode=%p wd=%d\n",
			fsn_mark->group, fsn_mark->i.inode, i_mark->wd);
	return 0;
}

static void inotify_free_group_priv(struct fsnotify_group *group)
{
	/* ideally the idr is empty and we won't hit the BUG in the callback */
	idr_for_each(&group->inotify_data.idr, idr_callback, group);
<<<<<<< HEAD
	idr_remove_all(&group->inotify_data.idr);
=======
>>>>>>> refs/remotes/origin/master
	idr_destroy(&group->inotify_data.idr);
	atomic_dec(&group->inotify_data.user->inotify_devs);
	free_uid(group->inotify_data.user);
}

<<<<<<< HEAD
void inotify_free_event_priv(struct fsnotify_event_private_data *fsn_event_priv)
{
	struct inotify_event_private_data *event_priv;


	event_priv = container_of(fsn_event_priv, struct inotify_event_private_data,
				  fsnotify_event_priv_data);

	kmem_cache_free(event_priv_cachep, event_priv);
=======
static void inotify_free_event(struct fsnotify_event *fsn_event)
{
	kfree(INOTIFY_E(fsn_event));
>>>>>>> refs/remotes/origin/master
}

const struct fsnotify_ops inotify_fsnotify_ops = {
	.handle_event = inotify_handle_event,
<<<<<<< HEAD
	.should_send_event = inotify_should_send_event,
	.free_group_priv = inotify_free_group_priv,
	.free_event_priv = inotify_free_event_priv,
=======
	.free_group_priv = inotify_free_group_priv,
	.free_event = inotify_free_event,
>>>>>>> refs/remotes/origin/master
	.freeing_mark = inotify_freeing_mark,
};
