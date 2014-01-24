/******************************************************************************
*******************************************************************************
**
**  Copyright (C) Sistina Software, Inc.  1997-2003  All rights reserved.
**  Copyright (C) 2004-2007 Red Hat, Inc.  All rights reserved.
**
**  This copyrighted material is made available to anyone wishing to use,
**  modify, copy, or redistribute it subject to the terms and conditions
**  of the GNU General Public License v.2.
**
*******************************************************************************
******************************************************************************/

#include "dlm_internal.h"
#include "config.h"
#include "memory.h"

static struct kmem_cache *lkb_cache;
<<<<<<< HEAD
<<<<<<< HEAD
=======
static struct kmem_cache *rsb_cache;
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct kmem_cache *rsb_cache;
>>>>>>> refs/remotes/origin/master


int __init dlm_memory_init(void)
{
<<<<<<< HEAD
	int ret = 0;

	lkb_cache = kmem_cache_create("dlm_lkb", sizeof(struct dlm_lkb),
				__alignof__(struct dlm_lkb), 0, NULL);
	if (!lkb_cache)
		ret = -ENOMEM;
<<<<<<< HEAD
=======
=======
	lkb_cache = kmem_cache_create("dlm_lkb", sizeof(struct dlm_lkb),
				__alignof__(struct dlm_lkb), 0, NULL);
	if (!lkb_cache)
		return -ENOMEM;
>>>>>>> refs/remotes/origin/master

	rsb_cache = kmem_cache_create("dlm_rsb", sizeof(struct dlm_rsb),
				__alignof__(struct dlm_rsb), 0, NULL);
	if (!rsb_cache) {
		kmem_cache_destroy(lkb_cache);
<<<<<<< HEAD
		ret = -ENOMEM;
	}

>>>>>>> refs/remotes/origin/cm-10.0
	return ret;
=======
		return -ENOMEM;
	}

	return 0;
>>>>>>> refs/remotes/origin/master
}

void dlm_memory_exit(void)
{
	if (lkb_cache)
		kmem_cache_destroy(lkb_cache);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (rsb_cache)
		kmem_cache_destroy(rsb_cache);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (rsb_cache)
		kmem_cache_destroy(rsb_cache);
>>>>>>> refs/remotes/origin/master
}

char *dlm_allocate_lvb(struct dlm_ls *ls)
{
	char *p;

	p = kzalloc(ls->ls_lvblen, GFP_NOFS);
	return p;
}

void dlm_free_lvb(char *p)
{
	kfree(p);
}

<<<<<<< HEAD
<<<<<<< HEAD
/* FIXME: have some minimal space built-in to rsb for the name and
   kmalloc a separate name if needed, like dentries are done */

struct dlm_rsb *dlm_allocate_rsb(struct dlm_ls *ls, int namelen)
{
	struct dlm_rsb *r;

	DLM_ASSERT(namelen <= DLM_RESNAME_MAXLEN,);

	r = kzalloc(sizeof(*r) + namelen, GFP_NOFS);
=======
=======
>>>>>>> refs/remotes/origin/master
struct dlm_rsb *dlm_allocate_rsb(struct dlm_ls *ls)
{
	struct dlm_rsb *r;

	r = kmem_cache_zalloc(rsb_cache, GFP_NOFS);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return r;
}

void dlm_free_rsb(struct dlm_rsb *r)
{
	if (r->res_lvbptr)
		dlm_free_lvb(r->res_lvbptr);
<<<<<<< HEAD
<<<<<<< HEAD
	kfree(r);
=======
	kmem_cache_free(rsb_cache, r);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	kmem_cache_free(rsb_cache, r);
>>>>>>> refs/remotes/origin/master
}

struct dlm_lkb *dlm_allocate_lkb(struct dlm_ls *ls)
{
	struct dlm_lkb *lkb;

	lkb = kmem_cache_zalloc(lkb_cache, GFP_NOFS);
	return lkb;
}

void dlm_free_lkb(struct dlm_lkb *lkb)
{
	if (lkb->lkb_flags & DLM_IFL_USER) {
		struct dlm_user_args *ua;
		ua = lkb->lkb_ua;
		if (ua) {
			if (ua->lksb.sb_lvbptr)
				kfree(ua->lksb.sb_lvbptr);
			kfree(ua);
		}
	}
	kmem_cache_free(lkb_cache, lkb);
}

