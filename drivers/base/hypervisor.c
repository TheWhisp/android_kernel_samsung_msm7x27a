/*
 * hypervisor.c - /sys/hypervisor subsystem.
 *
 * Copyright (C) IBM Corp. 2006
 * Copyright (C) 2007 Greg Kroah-Hartman <gregkh@suse.de>
 * Copyright (C) 2007 Novell Inc.
 *
 * This file is released under the GPLv2
 */

#include <linux/kobject.h>
#include <linux/device.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master
#include "base.h"

struct kobject *hypervisor_kobj;
EXPORT_SYMBOL_GPL(hypervisor_kobj);

int __init hypervisor_init(void)
{
	hypervisor_kobj = kobject_create_and_add("hypervisor", NULL);
	if (!hypervisor_kobj)
		return -ENOMEM;
	return 0;
}
