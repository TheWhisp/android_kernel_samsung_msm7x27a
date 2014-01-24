/*
 * AppArmor security module
 *
 * This file contains basic common functions used in AppArmor
 *
 * Copyright (C) 1998-2008 Novell/SUSE
 * Copyright 2009-2010 Canonical Ltd.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, version 2 of the
 * License.
 */

<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/mm.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/mm.h>
>>>>>>> refs/remotes/origin/master
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/vmalloc.h>

#include "include/audit.h"
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include "include/apparmor.h"
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include "include/apparmor.h"
>>>>>>> refs/remotes/origin/master


/**
 * aa_split_fqname - split a fqname into a profile and namespace name
 * @fqname: a full qualified name in namespace profile format (NOT NULL)
 * @ns_name: pointer to portion of the string containing the ns name (NOT NULL)
 *
 * Returns: profile name or NULL if one is not specified
 *
 * Split a namespace name from a profile name (see policy.c for naming
 * description).  If a portion of the name is missing it returns NULL for
 * that portion.
 *
 * NOTE: may modify the @fqname string.  The pointers returned point
 *       into the @fqname string.
 */
char *aa_split_fqname(char *fqname, char **ns_name)
{
	char *name = strim(fqname);

	*ns_name = NULL;
	if (name[0] == ':') {
		char *split = strchr(&name[1], ':');
		*ns_name = skip_spaces(&name[1]);
		if (split) {
			/* overwrite ':' with \0 */
<<<<<<< HEAD
			*split = 0;
			name = skip_spaces(split + 1);
=======
			*split++ = 0;
			if (strncmp(split, "//", 2) == 0)
				split += 2;
			name = skip_spaces(split);
>>>>>>> refs/remotes/origin/master
		} else
			/* a ns name without a following profile is allowed */
			name = NULL;
	}
	if (name && *name == 0)
		name = NULL;

	return name;
}

/**
 * aa_info_message - log a none profile related status message
 * @str: message to log
 */
void aa_info_message(const char *str)
{
	if (audit_enabled) {
		struct common_audit_data sa;
<<<<<<< HEAD
<<<<<<< HEAD
		COMMON_AUDIT_DATA_INIT(&sa, NONE);
		sa.aad.info = str;
=======
		struct apparmor_audit_data aad = {0,};
		COMMON_AUDIT_DATA_INIT(&sa, NONE);
		sa.aad = &aad;
		aad.info = str;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		struct apparmor_audit_data aad = {0,};
		sa.type = LSM_AUDIT_DATA_NONE;
		sa.aad = &aad;
		aad.info = str;
>>>>>>> refs/remotes/origin/master
		aa_audit_msg(AUDIT_APPARMOR_STATUS, &sa, NULL);
	}
	printk(KERN_INFO "AppArmor: %s\n", str);
}

/**
<<<<<<< HEAD
 * kvmalloc - do allocation preferring kmalloc but falling back to vmalloc
 * @size: size of allocation
=======
 * __aa_kvmalloc - do allocation preferring kmalloc but falling back to vmalloc
 * @size: how many bytes of memory are required
 * @flags: the type of memory to allocate (see kmalloc).
>>>>>>> refs/remotes/origin/master
 *
 * Return: allocated buffer or NULL if failed
 *
 * It is possible that policy being loaded from the user is larger than
 * what can be allocated by kmalloc, in those cases fall back to vmalloc.
 */
<<<<<<< HEAD
void *kvmalloc(size_t size)
=======
void *__aa_kvmalloc(size_t size, gfp_t flags)
>>>>>>> refs/remotes/origin/master
{
	void *buffer = NULL;

	if (size == 0)
		return NULL;

	/* do not attempt kmalloc if we need more than 16 pages at once */
	if (size <= (16*PAGE_SIZE))
<<<<<<< HEAD
		buffer = kmalloc(size, GFP_NOIO | __GFP_NOWARN);
	if (!buffer) {
		/* see kvfree for why size must be at least work_struct size
		 * when allocated via vmalloc
		 */
		if (size < sizeof(struct work_struct))
			size = sizeof(struct work_struct);
		buffer = vmalloc(size);
=======
		buffer = kmalloc(size, flags | GFP_NOIO | __GFP_NOWARN);
	if (!buffer) {
		if (flags & __GFP_ZERO)
			buffer = vzalloc(size);
		else
			buffer = vmalloc(size);
>>>>>>> refs/remotes/origin/master
	}
	return buffer;
}

/**
<<<<<<< HEAD
 * do_vfree - workqueue routine for freeing vmalloced memory
 * @work: data to be freed
 *
 * The work_struct is overlaid to the data being freed, as at the point
 * the work is scheduled the data is no longer valid, be its freeing
 * needs to be delayed until safe.
 */
static void do_vfree(struct work_struct *work)
{
	vfree(work);
}

/**
=======
>>>>>>> refs/remotes/origin/master
 * kvfree - free an allocation do by kvmalloc
 * @buffer: buffer to free (MAYBE_NULL)
 *
 * Free a buffer allocated by kvmalloc
 */
void kvfree(void *buffer)
{
<<<<<<< HEAD
	if (is_vmalloc_addr(buffer)) {
		/* Data is no longer valid so just use the allocated space
		 * as the work_struct
		 */
		struct work_struct *work = (struct work_struct *) buffer;
		INIT_WORK(work, do_vfree);
		schedule_work(work);
	} else
=======
	if (is_vmalloc_addr(buffer))
		vfree(buffer);
	else
>>>>>>> refs/remotes/origin/master
		kfree(buffer);
}
