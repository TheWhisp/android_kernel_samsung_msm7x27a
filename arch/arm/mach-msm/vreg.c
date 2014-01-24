/* arch/arm/mach-msm/vreg.c
 *
 * Copyright (C) 2008 Google, Inc.
<<<<<<< HEAD
 * Copyright (c) 2009-2012 The Linux Foundation. All rights reserved.
=======
 * Copyright (c) 2009, Code Aurora Forum. All rights reserved.
>>>>>>> refs/remotes/origin/master
 * Author: Brian Swetland <swetland@google.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/kernel.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/device.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/atomic.h>
#include <linux/debugfs.h>
#include <linux/regulator/consumer.h>
#include <linux/string.h>
#include <mach/vreg.h>
<<<<<<< HEAD

#include "proc_comm.h"
=======
#include <mach/proc_comm.h>
>>>>>>> refs/remotes/origin/cm-10.0

#if defined(CONFIG_MSM_VREG_SWITCH_INVERTED)
#define VREG_SWITCH_ENABLE 0
#define VREG_SWITCH_DISABLE 1
#else
#define VREG_SWITCH_ENABLE 1
#define VREG_SWITCH_DISABLE 0
#endif

struct vreg {
	struct list_head	list;
	struct mutex		lock;
	const char		*name;
	u64			refcnt;
	unsigned		mv;
	struct regulator	*reg;
};

static LIST_HEAD(vreg_list);
static DEFINE_MUTEX(vreg_lock);

#ifdef CONFIG_DEBUG_FS
static void vreg_add_debugfs(struct vreg *vreg);
#else
static inline void vreg_add_debugfs(struct vreg *vreg) { }
#endif

static struct vreg *vreg_create(const char *id)
{
	int rc;
	struct vreg *vreg;

	vreg = kzalloc(sizeof(*vreg), GFP_KERNEL);
	if (!vreg) {
		rc = -ENOMEM;
		goto error;
	}

	INIT_LIST_HEAD(&vreg->list);
	mutex_init(&vreg->lock);

	vreg->reg = regulator_get(NULL, id);
	if (IS_ERR(vreg->reg)) {
		rc = PTR_ERR(vreg->reg);
		goto free_vreg;
	}

	vreg->name = kstrdup(id, GFP_KERNEL);
	if (!vreg->name) {
		rc = -ENOMEM;
		goto put_reg;
	}

	list_add_tail(&vreg->list, &vreg_list);
	vreg_add_debugfs(vreg);

	return vreg;

put_reg:
	regulator_put(vreg->reg);
free_vreg:
	kfree(vreg);
error:
	return ERR_PTR(rc);
}

static void vreg_destroy(struct vreg *vreg)
{
	if (!vreg)
		return;

	if (vreg->refcnt)
		regulator_disable(vreg->reg);

	kfree(vreg->name);
	regulator_put(vreg->reg);
	kfree(vreg);
}

struct vreg *vreg_get(struct device *dev, const char *id)
{
	struct vreg *vreg = NULL;

	if (!id)
		return ERR_PTR(-EINVAL);

	mutex_lock(&vreg_lock);
	list_for_each_entry(vreg, &vreg_list, list) {
		if (!strncmp(vreg->name, id, 10))
			goto ret;
	}

	vreg = vreg_create(id);

ret:
	mutex_unlock(&vreg_lock);
	return vreg;
}
EXPORT_SYMBOL(vreg_get);

void vreg_put(struct vreg *vreg)
{
	kfree(vreg->name);
	regulator_put(vreg->reg);
	list_del(&vreg->list);
	kfree(vreg);
}

int vreg_enable(struct vreg *vreg)
{
	int rc = 0;
	if (!vreg)
		return -ENODEV;

	mutex_lock(&vreg->lock);
	if (vreg->refcnt == 0) {
		rc = regulator_enable(vreg->reg);
		if (!rc)
			vreg->refcnt++;
	} else {
		rc = 0;
		if (vreg->refcnt < UINT_MAX)
			vreg->refcnt++;
	}
	mutex_unlock(&vreg->lock);

	return rc;
}
EXPORT_SYMBOL(vreg_enable);

int vreg_disable(struct vreg *vreg)
{
	int rc = 0;
	if (!vreg)
		return -ENODEV;

	mutex_lock(&vreg->lock);
	if (vreg->refcnt == 0) {
		pr_warn("%s: unbalanced disables for vreg %s\n",
				__func__, vreg->name);
		rc = -EINVAL;
	} else if (vreg->refcnt == 1) {
		rc = regulator_disable(vreg->reg);
		if (!rc)
			vreg->refcnt--;
	} else {
		rc = 0;
		vreg->refcnt--;
	}
	mutex_unlock(&vreg->lock);

	return rc;
}
EXPORT_SYMBOL(vreg_disable);

int vreg_set_level(struct vreg *vreg, unsigned mv)
{
	unsigned uv;
	int rc;

	if (!vreg)
		return -EINVAL;

	if (mv > (UINT_MAX / 1000))
		return -ERANGE;

	uv = mv * 1000;

	mutex_lock(&vreg->lock);
	rc = regulator_set_voltage(vreg->reg, uv, uv);
	if (!rc)
		vreg->mv = mv;
	mutex_unlock(&vreg->lock);

	return rc;
}
EXPORT_SYMBOL(vreg_set_level);

#if defined(CONFIG_DEBUG_FS)

static int vreg_debug_enabled_set(void *data, u64 val)
{
	struct vreg *vreg = data;

	if (val == 0)
		return vreg_disable(vreg);
	else if (val == 1)
		return vreg_enable(vreg);
	else
		return -EINVAL;
}

static int vreg_debug_enabled_get(void *data, u64 *val)
{
	struct vreg *vreg = data;

	*val = vreg->refcnt;
=======
#include <linux/device.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/module.h>
#include <linux/string.h>
#include <mach/vreg.h>

#include "proc_comm.h"

struct vreg {
	const char *name;
	unsigned id;
	int status;
	unsigned refcnt;
};

#define VREG(_name, _id, _status, _refcnt) \
	{ .name = _name, .id = _id, .status = _status, .refcnt = _refcnt }

static struct vreg vregs[] = {
	VREG("msma",	0, 0, 0),
	VREG("msmp",	1, 0, 0),
	VREG("msme1",	2, 0, 0),
	VREG("msmc1",	3, 0, 0),
	VREG("msmc2",	4, 0, 0),
	VREG("gp3",	5, 0, 0),
	VREG("msme2",	6, 0, 0),
	VREG("gp4",	7, 0, 0),
	VREG("gp1",	8, 0, 0),
	VREG("tcxo",	9, 0, 0),
	VREG("pa",	10, 0, 0),
	VREG("rftx",	11, 0, 0),
	VREG("rfrx1",	12, 0, 0),
	VREG("rfrx2",	13, 0, 0),
	VREG("synt",	14, 0, 0),
	VREG("wlan",	15, 0, 0),
	VREG("usb",	16, 0, 0),
	VREG("boost",	17, 0, 0),
	VREG("mmc",	18, 0, 0),
	VREG("ruim",	19, 0, 0),
	VREG("msmc0",	20, 0, 0),
	VREG("gp2",	21, 0, 0),
	VREG("gp5",	22, 0, 0),
	VREG("gp6",	23, 0, 0),
	VREG("rf",	24, 0, 0),
	VREG("rf_vco",	26, 0, 0),
	VREG("mpll",	27, 0, 0),
	VREG("s2",	28, 0, 0),
	VREG("s3",	29, 0, 0),
	VREG("rfubm",	30, 0, 0),
	VREG("ncp",	31, 0, 0),
	VREG("gp7",	32, 0, 0),
	VREG("gp8",	33, 0, 0),
	VREG("gp9",	34, 0, 0),
	VREG("gp10",	35, 0, 0),
	VREG("gp11",	36, 0, 0),
	VREG("gp12",	37, 0, 0),
	VREG("gp13",	38, 0, 0),
	VREG("gp14",	39, 0, 0),
	VREG("gp15",	40, 0, 0),
	VREG("gp16",	41, 0, 0),
	VREG("gp17",	42, 0, 0),
	VREG("s4",	43, 0, 0),
	VREG("usb2",	44, 0, 0),
	VREG("wlan2",	45, 0, 0),
	VREG("xo_out",	46, 0, 0),
	VREG("lvsw0",	47, 0, 0),
	VREG("lvsw1",	48, 0, 0),
};

struct vreg *vreg_get(struct device *dev, const char *id)
{
	int n;
	for (n = 0; n < ARRAY_SIZE(vregs); n++) {
		if (!strcmp(vregs[n].name, id))
			return vregs + n;
	}
	return ERR_PTR(-ENOENT);
}

void vreg_put(struct vreg *vreg)
{
}

int vreg_enable(struct vreg *vreg)
{
	unsigned id = vreg->id;
	unsigned enable = 1;

	if (vreg->refcnt == 0)
		vreg->status = msm_proc_comm(PCOM_VREG_SWITCH, &id, &enable);

	if ((vreg->refcnt < UINT_MAX) && (!vreg->status))
		vreg->refcnt++;

	return vreg->status;
}

int vreg_disable(struct vreg *vreg)
{
	unsigned id = vreg->id;
	unsigned enable = 0;

	if (!vreg->refcnt)
		return 0;

	if (vreg->refcnt == 1)
		vreg->status = msm_proc_comm(PCOM_VREG_SWITCH, &id, &enable);

	if (!vreg->status)
		vreg->refcnt--;

	return vreg->status;
}

int vreg_set_level(struct vreg *vreg, unsigned mv)
{
	unsigned id = vreg->id;

	vreg->status = msm_proc_comm(PCOM_VREG_SET_LEVEL, &id, &mv);
	return vreg->status;
}

#if defined(CONFIG_DEBUG_FS)

static int vreg_debug_set(void *data, u64 val)
{
	struct vreg *vreg = data;
	switch (val) {
	case 0:
		vreg_disable(vreg);
		break;
	case 1:
		vreg_enable(vreg);
		break;
	default:
		vreg_set_level(vreg, val);
		break;
	}
	return 0;
}

static int vreg_debug_get(void *data, u64 *val)
{
	struct vreg *vreg = data;

	if (!vreg->status)
		*val = 0;
	else
		*val = 1;
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
static int vreg_debug_voltage_set(void *data, u64 val)
{
	struct vreg *vreg = data;
	return vreg_set_level(vreg, val);
}

static int vreg_debug_voltage_get(void *data, u64 *val)
{
	struct vreg *vreg = data;
	*val = vreg->mv;
	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(vreg_debug_enabled, vreg_debug_enabled_get,
		vreg_debug_enabled_set, "%llu");
DEFINE_SIMPLE_ATTRIBUTE(vreg_debug_voltage, vreg_debug_voltage_get,
		vreg_debug_voltage_set, "%llu");

static struct dentry *root;

static void vreg_add_debugfs(struct vreg *vreg)
{
	struct dentry *dir;

	if (!root)
		return;

	dir = debugfs_create_dir(vreg->name, root);

	if (IS_ERR_OR_NULL(dir))
		goto err;

	if (IS_ERR_OR_NULL(debugfs_create_file("enabled", 0644,	dir, vreg,
					&vreg_debug_enabled)))
		goto destroy;

	if (IS_ERR_OR_NULL(debugfs_create_file("voltage", 0644, dir, vreg,
					&vreg_debug_voltage)))
		goto destroy;

	return;

destroy:
	debugfs_remove_recursive(dir);
err:
	pr_warn("%s: could not create debugfs for vreg %s\n",
			__func__, vreg->name);
}

static int __devinit vreg_debug_init(void)
{
	root = debugfs_create_dir("vreg", NULL);

	if (IS_ERR_OR_NULL(root)) {
		pr_debug("%s: error initializing debugfs: %ld - "
				"disabling debugfs\n",
				__func__, root ? PTR_ERR(root) : 0);
		root = NULL;
=======
static int vreg_debug_count_set(void *data, u64 val)
{
	struct vreg *vreg = data;
	if (val > UINT_MAX)
		val = UINT_MAX;
	vreg->refcnt = val;
	return 0;
}

static int vreg_debug_count_get(void *data, u64 *val)
{
	struct vreg *vreg = data;

	*val = vreg->refcnt;

	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(vreg_fops, vreg_debug_get, vreg_debug_set, "%llu\n");
DEFINE_SIMPLE_ATTRIBUTE(vreg_count_fops, vreg_debug_count_get,
			vreg_debug_count_set, "%llu\n");

static int __init vreg_debug_init(void)
{
	struct dentry *dent;
	int n;
	char name[32];
	const char *refcnt_name = "_refcnt";

	dent = debugfs_create_dir("vreg", 0);
	if (IS_ERR(dent))
		return 0;

	for (n = 0; n < ARRAY_SIZE(vregs); n++) {
		(void) debugfs_create_file(vregs[n].name, 0644,
					   dent, vregs + n, &vreg_fops);

		strlcpy(name, vregs[n].name, sizeof(name));
		strlcat(name, refcnt_name, sizeof(name));
		(void) debugfs_create_file(name, 0644,
					   dent, vregs + n, &vreg_count_fops);
>>>>>>> refs/remotes/origin/master
	}

	return 0;
}
<<<<<<< HEAD
static void __devexit vreg_debug_exit(void)
{
	if (root)
		debugfs_remove_recursive(root);
	root = NULL;
}
#else
static inline int __init vreg_debug_init(void) { return 0; }
static inline void __exit vreg_debug_exit(void) { return 0; }
#endif

static int __init vreg_init(void)
{
	return vreg_debug_init();
}
module_init(vreg_init);

static void __exit vreg_exit(void)
{
	struct vreg *vreg, *next;
	vreg_debug_exit();

	mutex_lock(&vreg_lock);
	list_for_each_entry_safe(vreg, next, &vreg_list, list)
		vreg_destroy(vreg);
	mutex_unlock(&vreg_lock);
}
module_exit(vreg_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("vreg.c regulator shim");
MODULE_VERSION("1.0");
=======

device_initcall(vreg_debug_init);
#endif
>>>>>>> refs/remotes/origin/master
