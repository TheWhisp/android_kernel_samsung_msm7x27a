/* arch/arm/mach-msm/pm2.c
 *
 * MSM Power Management Routines
 *
 * Copyright (C) 2007 Google, Inc.
<<<<<<< HEAD
 * Copyright (c) 2008-2011 The Linux Foundation. All rights reserved.
=======
 * Copyright (c) 2008-2013 The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-10.0
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

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/init.h>
<<<<<<< HEAD
#include <linux/pm.h>
#include <linux/pm_qos_params.h>
#include <linux/proc_fs.h>
#include <linux/suspend.h>
#include <linux/reboot.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/memory.h>
#ifdef CONFIG_HAS_WAKELOCK
#include <linux/wakelock.h>
#endif
=======
#include <linux/pm_qos.h>
#include <linux/suspend.h>
#include <linux/io.h>
#include <linux/tick.h>
#include <linux/memory.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <mach/msm_iomap.h>
#include <mach/system.h>
#ifdef CONFIG_CPU_V7
#include <asm/pgtable.h>
#include <asm/pgalloc.h>
#endif
#ifdef CONFIG_CACHE_L2X0
#include <asm/hardware/cache-l2x0.h>
#endif
#ifdef CONFIG_VFP
#include <asm/vfp.h>
#endif

#ifdef CONFIG_MSM_MEMORY_LOW_POWER_MODE_SUSPEND_DEEP_POWER_DOWN
#include <mach/msm_migrate_pages.h>
#endif
<<<<<<< HEAD
=======
#include <mach/socinfo.h>
#include <mach/proc_comm.h>
#include <asm/smp_scu.h>
>>>>>>> refs/remotes/origin/cm-10.0

#include "smd_private.h"
#include "smd_rpcrouter.h"
#include "acpuclock.h"
#include "clock.h"
<<<<<<< HEAD
#include "proc_comm.h"
=======
>>>>>>> refs/remotes/origin/cm-10.0
#include "idle.h"
#include "irq.h"
#include "gpio.h"
#include "timer.h"
#include "pm.h"
#include "spm.h"
#include "sirc.h"
#include "pm-boot.h"
<<<<<<< HEAD

#include <linux/sec_param.h>
=======
#include "devices-msm7x2xa.h"
>>>>>>> refs/remotes/origin/cm-10.0

/******************************************************************************
 * Debug Definitions
 *****************************************************************************/

enum {
<<<<<<< HEAD
	MSM_PM_DEBUG_SUSPEND = 1U << 0,
	MSM_PM_DEBUG_POWER_COLLAPSE = 1U << 1,
	MSM_PM_DEBUG_STATE = 1U << 2,
	MSM_PM_DEBUG_CLOCK = 1U << 3,
	MSM_PM_DEBUG_RESET_VECTOR = 1U << 4,
	MSM_PM_DEBUG_SMSM_STATE = 1U << 5,
	MSM_PM_DEBUG_IDLE = 1U << 6,
};

static int msm_pm_debug_mask = MSM_PM_DEBUG_POWER_COLLAPSE
					| MSM_PM_DEBUG_SMSM_STATE;
=======
	MSM_PM_DEBUG_SUSPEND = BIT(0),
	MSM_PM_DEBUG_POWER_COLLAPSE = BIT(1),
	MSM_PM_DEBUG_STATE = BIT(2),
	MSM_PM_DEBUG_CLOCK = BIT(3),
	MSM_PM_DEBUG_RESET_VECTOR = BIT(4),
	MSM_PM_DEBUG_SMSM_STATE = BIT(5),
	MSM_PM_DEBUG_IDLE = BIT(6),
	MSM_PM_DEBUG_HOTPLUG = BIT(7),
};

DEFINE_PER_CPU(int, power_collapsed);

static int msm_pm_debug_mask;
>>>>>>> refs/remotes/origin/cm-10.0
module_param_named(
	debug_mask, msm_pm_debug_mask, int, S_IRUGO | S_IWUSR | S_IWGRP
);

#define MSM_PM_DPRINTK(mask, level, message, ...) \
	do { \
		if ((mask) & msm_pm_debug_mask) \
			printk(level message, ## __VA_ARGS__); \
	} while (0)

#define MSM_PM_DEBUG_PRINT_STATE(tag) \
	do { \
		MSM_PM_DPRINTK(MSM_PM_DEBUG_STATE, \
			KERN_INFO, "%s: " \
			"APPS_CLK_SLEEP_EN %x, APPS_PWRDOWN %x, " \
			"SMSM_POWER_MASTER_DEM %x, SMSM_MODEM_STATE %x, " \
			"SMSM_APPS_DEM %x\n", \
			tag, \
			__raw_readl(APPS_CLK_SLEEP_EN), \
			__raw_readl(APPS_PWRDOWN), \
			smsm_get_state(SMSM_POWER_MASTER_DEM), \
			smsm_get_state(SMSM_MODEM_STATE), \
			smsm_get_state(SMSM_APPS_DEM)); \
	} while (0)

#define MSM_PM_DEBUG_PRINT_SLEEP_INFO() \
	do { \
		if (msm_pm_debug_mask & MSM_PM_DEBUG_SMSM_STATE) \
			smsm_print_sleep_info(msm_pm_smem_data->sleep_time, \
				msm_pm_smem_data->resources_used, \
				msm_pm_smem_data->irq_mask, \
				msm_pm_smem_data->wakeup_reason, \
				msm_pm_smem_data->pending_irqs); \
	} while (0)


/******************************************************************************
 * Sleep Modes and Parameters
 *****************************************************************************/

<<<<<<< HEAD
static int msm_pm_sleep_mode = CONFIG_MSM7X00A_SLEEP_MODE;
module_param_named(
	sleep_mode, msm_pm_sleep_mode,
	int, S_IRUGO | S_IWUSR | S_IWGRP
);

static int msm_pm_idle_sleep_mode = CONFIG_MSM7X00A_IDLE_SLEEP_MODE;
module_param_named(
	idle_sleep_mode, msm_pm_idle_sleep_mode,
	int, S_IRUGO | S_IWUSR | S_IWGRP
);

=======
>>>>>>> refs/remotes/origin/cm-10.0
static int msm_pm_idle_sleep_min_time = CONFIG_MSM7X00A_IDLE_SLEEP_MIN_TIME;
module_param_named(
	idle_sleep_min_time, msm_pm_idle_sleep_min_time,
	int, S_IRUGO | S_IWUSR | S_IWGRP
);

enum {
	MSM_PM_MODE_ATTR_SUSPEND,
	MSM_PM_MODE_ATTR_IDLE,
	MSM_PM_MODE_ATTR_LATENCY,
	MSM_PM_MODE_ATTR_RESIDENCY,
	MSM_PM_MODE_ATTR_NR,
};

static char *msm_pm_mode_attr_labels[MSM_PM_MODE_ATTR_NR] = {
	[MSM_PM_MODE_ATTR_SUSPEND] = "suspend_enabled",
	[MSM_PM_MODE_ATTR_IDLE] = "idle_enabled",
	[MSM_PM_MODE_ATTR_LATENCY] = "latency",
	[MSM_PM_MODE_ATTR_RESIDENCY] = "residency",
};

static char *msm_pm_sleep_mode_labels[MSM_PM_SLEEP_MODE_NR] = {
	[MSM_PM_SLEEP_MODE_POWER_COLLAPSE_SUSPEND] = " ",
	[MSM_PM_SLEEP_MODE_POWER_COLLAPSE] = "power_collapse",
<<<<<<< HEAD
	[MSM_PM_SLEEP_MODE_APPS_SLEEP] = "apps_sleep",
=======
>>>>>>> refs/remotes/origin/cm-10.0
	[MSM_PM_SLEEP_MODE_RAMP_DOWN_AND_WAIT_FOR_INTERRUPT] =
		"ramp_down_and_wfi",
	[MSM_PM_SLEEP_MODE_WAIT_FOR_INTERRUPT] = "wfi",
	[MSM_PM_SLEEP_MODE_POWER_COLLAPSE_NO_XO_SHUTDOWN] =
		"power_collapse_no_xo_shutdown",
	[MSM_PM_SLEEP_MODE_POWER_COLLAPSE_STANDALONE] =
		"standalone_power_collapse",
};

static struct msm_pm_platform_data *msm_pm_modes;
<<<<<<< HEAD

static struct kobject *msm_pm_mode_kobjs[MSM_PM_SLEEP_MODE_NR];
static struct attribute_group *msm_pm_mode_attr_group[MSM_PM_SLEEP_MODE_NR];
static struct attribute **msm_pm_mode_attrs[MSM_PM_SLEEP_MODE_NR];
static struct kobj_attribute *msm_pm_mode_kobj_attrs[MSM_PM_SLEEP_MODE_NR];

static DEFINE_SPINLOCK(pm_lock);
int power_off_done;
=======
static struct msm_pm_irq_calls *msm_pm_irq_extns;
static struct msm_pm_cpr_ops *msm_cpr_ops;

struct msm_pm_kobj_attribute {
	unsigned int cpu;
	struct kobj_attribute ka;
};

struct msm8x25q_ahb_registers {
	uint32_t sel;
	uint32_t cntl;
};

static struct msm8x25q_ahb_registers msm8x25q_ahb;

#define GET_CPU_OF_ATTR(attr) \
	(container_of(attr, struct msm_pm_kobj_attribute, ka)->cpu)

struct msm_pm_sysfs_sleep_mode {
	struct kobject *kobj;
	struct attribute_group attr_group;
	struct attribute *attrs[MSM_PM_MODE_ATTR_NR + 1];
	struct msm_pm_kobj_attribute kas[MSM_PM_MODE_ATTR_NR];
};
>>>>>>> refs/remotes/origin/cm-10.0

/*
 * Write out the attribute.
 */
static ssize_t msm_pm_mode_attr_show(
	struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	int ret = -EINVAL;
	int i;

	for (i = 0; i < MSM_PM_SLEEP_MODE_NR; i++) {
		struct kernel_param kp;
<<<<<<< HEAD
=======
		unsigned int cpu;
		struct msm_pm_platform_data *mode;
>>>>>>> refs/remotes/origin/cm-10.0

		if (msm_pm_sleep_mode_labels[i] == NULL)
			continue;

		if (strcmp(kobj->name, msm_pm_sleep_mode_labels[i]))
			continue;

<<<<<<< HEAD
		if (!strcmp(attr->attr.name,
			msm_pm_mode_attr_labels[MSM_PM_MODE_ATTR_SUSPEND])) {
			u32 arg = msm_pm_modes[i].suspend_enabled;
=======
		cpu = GET_CPU_OF_ATTR(attr);
		mode = &msm_pm_modes[MSM_PM_MODE(cpu, i)];

		if (!strcmp(attr->attr.name,
			msm_pm_mode_attr_labels[MSM_PM_MODE_ATTR_SUSPEND])) {
			u32 arg = mode->suspend_enabled;
>>>>>>> refs/remotes/origin/cm-10.0
			kp.arg = &arg;
			ret = param_get_ulong(buf, &kp);
		} else if (!strcmp(attr->attr.name,
			msm_pm_mode_attr_labels[MSM_PM_MODE_ATTR_IDLE])) {
<<<<<<< HEAD
			u32 arg = msm_pm_modes[i].idle_enabled;
=======
			u32 arg = mode->idle_enabled;
>>>>>>> refs/remotes/origin/cm-10.0
			kp.arg = &arg;
			ret = param_get_ulong(buf, &kp);
		} else if (!strcmp(attr->attr.name,
			msm_pm_mode_attr_labels[MSM_PM_MODE_ATTR_LATENCY])) {
<<<<<<< HEAD
			kp.arg = &msm_pm_modes[i].latency;
			ret = param_get_ulong(buf, &kp);
		} else if (!strcmp(attr->attr.name,
			msm_pm_mode_attr_labels[MSM_PM_MODE_ATTR_RESIDENCY])) {
			kp.arg = &msm_pm_modes[i].residency;
=======
			u32 arg = mode->latency;
			kp.arg = &arg;
			ret = param_get_ulong(buf, &kp);
		} else if (!strcmp(attr->attr.name,
			msm_pm_mode_attr_labels[MSM_PM_MODE_ATTR_RESIDENCY])) {
			u32 arg = mode->residency;
			kp.arg = &arg;
>>>>>>> refs/remotes/origin/cm-10.0
			ret = param_get_ulong(buf, &kp);
		}

		break;
	}

	if (ret > 0) {
<<<<<<< HEAD
		strcat(buf, "\n");
=======
		strlcat(buf, "\n", PAGE_SIZE);
>>>>>>> refs/remotes/origin/cm-10.0
		ret++;
	}

	return ret;
}

/*
 * Read in the new attribute value.
 */
static ssize_t msm_pm_mode_attr_store(struct kobject *kobj,
	struct kobj_attribute *attr, const char *buf, size_t count)
{
	int ret = -EINVAL;
	int i;

	for (i = 0; i < MSM_PM_SLEEP_MODE_NR; i++) {
		struct kernel_param kp;
<<<<<<< HEAD
=======
		unsigned int cpu;
		struct msm_pm_platform_data *mode;
>>>>>>> refs/remotes/origin/cm-10.0

		if (msm_pm_sleep_mode_labels[i] == NULL)
			continue;

		if (strcmp(kobj->name, msm_pm_sleep_mode_labels[i]))
			continue;

<<<<<<< HEAD
		if (!strcmp(attr->attr.name,
			msm_pm_mode_attr_labels[MSM_PM_MODE_ATTR_SUSPEND])) {
			kp.arg = &msm_pm_modes[i].suspend_enabled;
			ret = param_set_byte(buf, &kp);
		} else if (!strcmp(attr->attr.name,
			msm_pm_mode_attr_labels[MSM_PM_MODE_ATTR_IDLE])) {
			kp.arg = &msm_pm_modes[i].idle_enabled;
			ret = param_set_byte(buf, &kp);
		} else if (!strcmp(attr->attr.name,
			msm_pm_mode_attr_labels[MSM_PM_MODE_ATTR_LATENCY])) {
			kp.arg = &msm_pm_modes[i].latency;
			ret = param_set_ulong(buf, &kp);
		} else if (!strcmp(attr->attr.name,
			msm_pm_mode_attr_labels[MSM_PM_MODE_ATTR_RESIDENCY])) {
			kp.arg = &msm_pm_modes[i].residency;
=======
		cpu = GET_CPU_OF_ATTR(attr);
		mode = &msm_pm_modes[MSM_PM_MODE(cpu, i)];

		if (!strcmp(attr->attr.name,
			msm_pm_mode_attr_labels[MSM_PM_MODE_ATTR_SUSPEND])) {
			kp.arg = &mode->suspend_enabled;
			ret = param_set_byte(buf, &kp);
		} else if (!strcmp(attr->attr.name,
			msm_pm_mode_attr_labels[MSM_PM_MODE_ATTR_IDLE])) {
			kp.arg = &mode->idle_enabled;
			ret = param_set_byte(buf, &kp);
		} else if (!strcmp(attr->attr.name,
			msm_pm_mode_attr_labels[MSM_PM_MODE_ATTR_LATENCY])) {
			kp.arg = &mode->latency;
			ret = param_set_ulong(buf, &kp);
		} else if (!strcmp(attr->attr.name,
			msm_pm_mode_attr_labels[MSM_PM_MODE_ATTR_RESIDENCY])) {
			kp.arg = &mode->residency;
>>>>>>> refs/remotes/origin/cm-10.0
			ret = param_set_ulong(buf, &kp);
		}

		break;
	}

	return ret ? ret : count;
}

<<<<<<< HEAD
/*
 * Add sysfs entries for the sleep modes.
 */
static int __init msm_pm_mode_sysfs_add(void)
{
	struct kobject *module_kobj = NULL;
	struct kobject *modes_kobj = NULL;

	struct kobject *kobj;
	struct attribute_group *attr_group;
	struct attribute **attrs;
	struct kobj_attribute *kobj_attrs;

	int i, j, k;
	int ret;

	module_kobj = kset_find_obj(module_kset, KBUILD_MODNAME);
	if (!module_kobj) {
		printk(KERN_ERR "%s: cannot find kobject for module %s\n",
			__func__, KBUILD_MODNAME);
		ret = -ENOENT;
		goto mode_sysfs_add_cleanup;
	}

	modes_kobj = kobject_create_and_add("modes", module_kobj);
	if (!modes_kobj) {
		printk(KERN_ERR "%s: cannot create modes kobject\n", __func__);
		ret = -ENOMEM;
		goto mode_sysfs_add_cleanup;
	}

	for (i = 0; i < ARRAY_SIZE(msm_pm_mode_kobjs); i++) {
		if (!msm_pm_modes[i].suspend_supported &&
				!msm_pm_modes[i].idle_supported)
			continue;

		kobj = kobject_create_and_add(
				msm_pm_sleep_mode_labels[i], modes_kobj);
		attr_group = kzalloc(sizeof(*attr_group), GFP_KERNEL);
		attrs = kzalloc(sizeof(*attrs) * (MSM_PM_MODE_ATTR_NR + 1),
				GFP_KERNEL);
		kobj_attrs = kzalloc(sizeof(*kobj_attrs) * MSM_PM_MODE_ATTR_NR,
				GFP_KERNEL);

		if (!kobj || !attr_group || !attrs || !kobj_attrs) {
			printk(KERN_ERR
				"%s: cannot create kobject or attributes\n",
				__func__);
			ret = -ENOMEM;
			goto mode_sysfs_add_abort;
		}

		for (k = 0, j = 0; k < MSM_PM_MODE_ATTR_NR; k++) {
			if ((k == MSM_PM_MODE_ATTR_SUSPEND) &&
				(!msm_pm_modes[i].suspend_supported))
				continue;
			if ((k == MSM_PM_MODE_ATTR_IDLE) &&
				(!msm_pm_modes[i].idle_supported))
				continue;

			kobj_attrs[j].attr.mode = 0644;
			kobj_attrs[j].show = msm_pm_mode_attr_show;
			kobj_attrs[j].store = msm_pm_mode_attr_store;
			kobj_attrs[j].attr.name = msm_pm_mode_attr_labels[k];
			attrs[j] = &kobj_attrs[j].attr;
			j++;
		}
		attrs[j] = NULL;

		attr_group->attrs = attrs;
		ret = sysfs_create_group(kobj, attr_group);
=======
 /* Add sysfs entries for one cpu. */
static int __init msm_pm_mode_sysfs_add_cpu(
	unsigned int cpu, struct kobject *modes_kobj)
{
	char cpu_name[8];
	struct kobject *cpu_kobj;
	struct msm_pm_sysfs_sleep_mode *mode = NULL;
	int i, j, k;
	int ret;

	snprintf(cpu_name, sizeof(cpu_name), "cpu%u", cpu);
	cpu_kobj = kobject_create_and_add(cpu_name, modes_kobj);
	if (!cpu_kobj) {
		pr_err("%s: cannot create %s kobject\n", __func__, cpu_name);
		ret = -ENOMEM;
		goto mode_sysfs_add_cpu_exit;
	}

	for (i = 0; i < MSM_PM_SLEEP_MODE_NR; i++) {
		int idx = MSM_PM_MODE(cpu, i);

		if ((!msm_pm_modes[idx].suspend_supported) &&
				(!msm_pm_modes[idx].idle_supported))
			continue;

		mode = kzalloc(sizeof(*mode), GFP_KERNEL);
		if (!mode) {
			pr_err("%s: cannot allocate memory for attributes\n",
				__func__);
			ret = -ENOMEM;
			goto mode_sysfs_add_cpu_exit;
		}

		mode->kobj = kobject_create_and_add(
				msm_pm_sleep_mode_labels[i], cpu_kobj);
		if (!mode->kobj) {
			pr_err("%s: cannot create kobject\n", __func__);
			ret = -ENOMEM;
			goto mode_sysfs_add_cpu_exit;
		}

		for (k = 0, j = 0; k < MSM_PM_MODE_ATTR_NR; k++) {
			if ((k == MSM_PM_MODE_ATTR_IDLE) &&
				!msm_pm_modes[idx].idle_supported)
				continue;
			if ((k == MSM_PM_MODE_ATTR_SUSPEND) &&
			     !msm_pm_modes[idx].suspend_supported)
				continue;
			mode->kas[j].cpu = cpu;
			mode->kas[j].ka.attr.mode = 0644;
			mode->kas[j].ka.show = msm_pm_mode_attr_show;
			mode->kas[j].ka.store = msm_pm_mode_attr_store;
			mode->kas[j].ka.attr.name = msm_pm_mode_attr_labels[k];
			mode->attrs[j] = &mode->kas[j].ka.attr;
			j++;
		}
		mode->attrs[j] = NULL;

		mode->attr_group.attrs = mode->attrs;
		ret = sysfs_create_group(mode->kobj, &mode->attr_group);
>>>>>>> refs/remotes/origin/cm-10.0
		if (ret) {
			printk(KERN_ERR
				"%s: cannot create kobject attribute group\n",
				__func__);
<<<<<<< HEAD
			goto mode_sysfs_add_abort;
		}

		msm_pm_mode_kobjs[i] = kobj;
		msm_pm_mode_attr_group[i] = attr_group;
		msm_pm_mode_attrs[i] = attrs;
		msm_pm_mode_kobj_attrs[i] = kobj_attrs;
	}

	return 0;

mode_sysfs_add_abort:
	kfree(kobj_attrs);
	kfree(attrs);
	kfree(attr_group);
	kobject_put(kobj);

mode_sysfs_add_cleanup:
	for (i = ARRAY_SIZE(msm_pm_mode_kobjs) - 1; i >= 0; i--) {
		if (!msm_pm_mode_kobjs[i])
			continue;

		sysfs_remove_group(
			msm_pm_mode_kobjs[i], msm_pm_mode_attr_group[i]);

		kfree(msm_pm_mode_kobj_attrs[i]);
		kfree(msm_pm_mode_attrs[i]);
		kfree(msm_pm_mode_attr_group[i]);
		kobject_put(msm_pm_mode_kobjs[i]);
	}

	return ret;
}

void __init msm_pm_set_platform_data(
	struct msm_pm_platform_data *data, int count)
{
	BUG_ON(MSM_PM_SLEEP_MODE_NR != count);
	msm_pm_modes = data;
}

=======
			goto mode_sysfs_add_cpu_exit;
		}
	}

	ret = 0;

mode_sysfs_add_cpu_exit:
	if (ret) {
		if (mode && mode->kobj)
			kobject_del(mode->kobj);
		kfree(mode);
	}

	return ret;
}

/*
 * Add sysfs entries for the sleep modes.
 */
static int __init msm_pm_mode_sysfs_add(void)
{
	struct kobject *module_kobj = NULL;
	struct kobject *modes_kobj = NULL;
	unsigned int cpu;
	int ret;

	module_kobj = kset_find_obj(module_kset, KBUILD_MODNAME);
	if (!module_kobj) {
		printk(KERN_ERR "%s: cannot find kobject for module %s\n",
			__func__, KBUILD_MODNAME);
		ret = -ENOENT;
		goto mode_sysfs_add_exit;
	}

	modes_kobj = kobject_create_and_add("modes", module_kobj);
	if (!modes_kobj) {
		printk(KERN_ERR "%s: cannot create modes kobject\n", __func__);
		ret = -ENOMEM;
		goto mode_sysfs_add_exit;
	}

	for_each_possible_cpu(cpu) {
		ret = msm_pm_mode_sysfs_add_cpu(cpu, modes_kobj);
		if (ret)
			goto mode_sysfs_add_exit;
	}

	ret = 0;

mode_sysfs_add_exit:
	return ret;
}

s32 msm_cpuidle_get_deep_idle_latency(void)
{
	int i = MSM_PM_MODE(0, MSM_PM_SLEEP_MODE_POWER_COLLAPSE_NO_XO_SHUTDOWN);
	return msm_pm_modes[i].latency - 1;
}

void __init msm_pm_set_platform_data(
	struct msm_pm_platform_data *data, int count)
{
	BUG_ON(MSM_PM_SLEEP_MODE_NR * num_possible_cpus() > count);
	msm_pm_modes = data;
}

void __init msm_pm_set_irq_extns(struct msm_pm_irq_calls *irq_calls)
{
	/* sanity check */
	BUG_ON(irq_calls == NULL || irq_calls->irq_pending == NULL ||
		irq_calls->idle_sleep_allowed == NULL ||
		irq_calls->enter_sleep1 == NULL ||
		irq_calls->enter_sleep2 == NULL ||
		irq_calls->exit_sleep1 == NULL ||
		irq_calls->exit_sleep2 == NULL ||
		irq_calls->exit_sleep3 == NULL);

	msm_pm_irq_extns = irq_calls;
}

void __init msm_pm_set_cpr_ops(struct msm_pm_cpr_ops *ops)
{
	msm_cpr_ops = ops;
}
>>>>>>> refs/remotes/origin/cm-10.0

/******************************************************************************
 * Sleep Limitations
 *****************************************************************************/
enum {
	SLEEP_LIMIT_NONE = 0,
	SLEEP_LIMIT_NO_TCXO_SHUTDOWN = 2,
	SLEEP_LIMIT_MASK = 0x03,
};

<<<<<<< HEAD
=======
static uint32_t msm_pm_sleep_limit = SLEEP_LIMIT_NONE;
>>>>>>> refs/remotes/origin/cm-10.0
#ifdef CONFIG_MSM_MEMORY_LOW_POWER_MODE
enum {
	SLEEP_RESOURCE_MEMORY_BIT0 = 0x0200,
	SLEEP_RESOURCE_MEMORY_BIT1 = 0x0010,
};
#endif


/******************************************************************************
 * Configure Hardware for Power Down/Up
 *****************************************************************************/

#if defined(CONFIG_ARCH_MSM7X30)
<<<<<<< HEAD
#define APPS_CLK_SLEEP_EN (MSM_GCC_BASE + 0x020)
#define APPS_PWRDOWN      (MSM_ACC_BASE + 0x01c)
#define APPS_SECOP        (MSM_TCSR_BASE + 0x038)
#else /* defined(CONFIG_ARCH_MSM7X30) */
#define APPS_CLK_SLEEP_EN (MSM_CSR_BASE + 0x11c)
#define APPS_PWRDOWN      (MSM_CSR_BASE + 0x440)
#define APPS_STANDBY_CTL  (MSM_CSR_BASE + 0x108)
#endif /* defined(CONFIG_ARCH_MSM7X30) */
=======
#define APPS_CLK_SLEEP_EN (MSM_APCS_GCC_BASE + 0x020)
#define APPS_PWRDOWN      (MSM_ACC0_BASE + 0x01c)
#define APPS_SECOP        (MSM_TCSR_BASE + 0x038)
#define APPS_STANDBY_CTL  NULL
#else
#define APPS_CLK_SLEEP_EN (MSM_CSR_BASE + 0x11c)
#define APPS_PWRDOWN      (MSM_CSR_BASE + 0x440)
#define APPS_STANDBY_CTL  (MSM_CSR_BASE + 0x108)
#define APPS_SECOP	  NULL
#define A11S_CLK_CNTL_ADDR	(MSM_CSR_BASE + 0x100)
#define A11S_CLK_SEL_ADDR	(MSM_CSR_BASE + 0x104)

#endif
>>>>>>> refs/remotes/origin/cm-10.0

/*
 * Configure hardware registers in preparation for Apps power down.
 */
static void msm_pm_config_hw_before_power_down(void)
{
<<<<<<< HEAD
#if defined(CONFIG_ARCH_MSM7X30)
	__raw_writel(1, APPS_PWRDOWN);
	mb();
	__raw_writel(4, APPS_SECOP);
	mb();
#elif defined(CONFIG_ARCH_MSM7X27)
	__raw_writel(0x1f, APPS_CLK_SLEEP_EN);
	mb();
	__raw_writel(1, APPS_PWRDOWN);
	mb();
#elif defined(CONFIG_ARCH_MSM7x27A)
	__raw_writel(0x7, APPS_CLK_SLEEP_EN);
	mb();
	__raw_writel(1, APPS_PWRDOWN);
	mb();
#else
	__raw_writel(0x1f, APPS_CLK_SLEEP_EN);
	mb();
	__raw_writel(1, APPS_PWRDOWN);
	mb();
	__raw_writel(0, APPS_STANDBY_CTL);
	mb();
#endif
=======
	if (cpu_is_msm7x30() || cpu_is_msm8x55()) {
		__raw_writel(4, APPS_SECOP);
	} else if (cpu_is_msm7x27()) {
		__raw_writel(0x1f, APPS_CLK_SLEEP_EN);
	} else if (cpu_is_msm7x27a() || cpu_is_msm7x27aa() ||
		   cpu_is_msm7x25a() || cpu_is_msm7x25aa() ||
		   cpu_is_msm7x25ab()) {
		__raw_writel(0x7, APPS_CLK_SLEEP_EN);
	} else if (cpu_is_qsd8x50()) {
		__raw_writel(0x1f, APPS_CLK_SLEEP_EN);
		mb();
		__raw_writel(0, APPS_STANDBY_CTL);
	}
	mb();
	__raw_writel(1, APPS_PWRDOWN);
	mb();
}

/*
 * Program the top csr from core0 context to put the
 * core1 into GDFS, as core1 is not running yet.
 */
static void msm_pm_configure_top_csr(void)
{
	/*
	 * Enable TCSR for core
	 * Set reset bit for SPM
	 * Set CLK_OFF bit
	 * Set clamps bit
	 * Set power_up bit
	 * Disable TSCR for core
	 */
	uint32_t bit_pos[][6] = {
		/* c2 */
		{17, 15, 13, 16, 14, 17},
		/* c1 & c3*/
		{22, 20, 18, 21, 19, 22},
	};
	uint32_t mpa5_cfg_ctl[2] = {0x30, 0x48};
	void __iomem *base_ptr;
	unsigned int value = 0;
	unsigned int cpu;
	int i;

	/* Initialize all the SPM registers */
	msm_spm_reinit();

	/* enable TCSR for core1 */
	value = __raw_readl((MSM_CFG_CTL_BASE + MPA5_CFG_CTL_REG));
	value |= BIT(22);
	__raw_writel(value,  MSM_CFG_CTL_BASE + MPA5_CFG_CTL_REG);
	mb();

		base_ptr = core_reset_base(cpu);
		if (!base_ptr)
			return;

		/* bring the core out of reset */
		__raw_writel(0x3, base_ptr);
		mb();

		/*
		 * i == 0, Enable TCSR for core
		 * i == 1, Set reset bit for SPM
		 * i == 2, Set CLK_OFF bit
		 * i == 3, Set clamps bit
		 * i == 4, Set power_up bit
		 */
		for (i = 0; i < 5; i++) {
			value = __raw_readl(MSM_CFG_CTL_BASE +
							mpa5_cfg_ctl[cpu/2]);
			value |= BIT(bit_pos[cpu%2][i]);
			__raw_writel(value,  MSM_CFG_CTL_BASE +
							mpa5_cfg_ctl[cpu/2]);
			mb();
		}

		/* i == 5, Disable TCSR for core */
		value = __raw_readl(MSM_CFG_CTL_BASE +
						mpa5_cfg_ctl[cpu/2]);
		value &= ~BIT(bit_pos[cpu%2][i]);
		__raw_writel(value,  MSM_CFG_CTL_BASE +
						mpa5_cfg_ctl[cpu/2]);
		mb();

	/* Disable TSCR for core0 */
	value = __raw_readl((MSM_CFG_CTL_BASE + MPA5_CFG_CTL_REG));
	value &= ~BIT(22);
	__raw_writel(value,  MSM_CFG_CTL_BASE + MPA5_CFG_CTL_REG);
	mb();
	__raw_writel(0x0, base_ptr);
	mb();
>>>>>>> refs/remotes/origin/cm-10.0
}

/*
 * Clear hardware registers after Apps powers up.
 */
static void msm_pm_config_hw_after_power_up(void)
{
<<<<<<< HEAD
#if defined(CONFIG_ARCH_MSM7X30)
	__raw_writel(0, APPS_SECOP);
	mb();
	__raw_writel(0, APPS_PWRDOWN);
	mb();
	msm_spm_reinit();
#elif defined(CONFIG_ARCH_MSM7x27A)
	__raw_writel(0, APPS_PWRDOWN);
	mb();
	__raw_writel(0, APPS_CLK_SLEEP_EN);
	mb();
#else
	__raw_writel(0, APPS_PWRDOWN);
	mb();
	__raw_writel(0, APPS_CLK_SLEEP_EN);
	mb();
#endif
=======

	if (cpu_is_msm7x30() || cpu_is_msm8x55()) {
		__raw_writel(0, APPS_SECOP);
		mb();
		__raw_writel(0, APPS_PWRDOWN);
		mb();
		msm_spm_reinit();
	} else if (cpu_is_msm8625() || cpu_is_msm8625q()) {
		__raw_writel(0, APPS_PWRDOWN);
		mb();

		if (per_cpu(power_collapsed, 1)) {
			/*
			 * enable the SCU while coming out of power
			 * collapse.
			 */
			/*
			 * Program the top csr to put the core1 into GDFS.
			 */
			msm_pm_configure_top_csr();
		}
	} else {
		__raw_writel(0, APPS_PWRDOWN);
		mb();
		__raw_writel(0, APPS_CLK_SLEEP_EN);
		mb();
	}
>>>>>>> refs/remotes/origin/cm-10.0
}

/*
 * Configure hardware registers in preparation for SWFI.
 */
static void msm_pm_config_hw_before_swfi(void)
{
<<<<<<< HEAD
#if defined(CONFIG_ARCH_QSD8X50)
	__raw_writel(0x1f, APPS_CLK_SLEEP_EN);
	mb();
#elif defined(CONFIG_ARCH_MSM7X27)
	__raw_writel(0x0f, APPS_CLK_SLEEP_EN);
	mb();
#elif defined(CONFIG_ARCH_MSM7X27A)
	__raw_writel(0x7, APPS_CLK_SLEEP_EN);
	mb();
#endif
=======
	if (cpu_is_qsd8x50()) {
		__raw_writel(0x1f, APPS_CLK_SLEEP_EN);
		mb();
	} else if (cpu_is_msm7x27()) {
		__raw_writel(0x0f, APPS_CLK_SLEEP_EN);
		mb();
	} else if (cpu_is_msm7x27a() || cpu_is_msm7x27aa() ||
		   cpu_is_msm7x25a() || cpu_is_msm7x25aa() ||
		   cpu_is_msm7x25ab()) {
		__raw_writel(0x7, APPS_CLK_SLEEP_EN);
		mb();
	}
>>>>>>> refs/remotes/origin/cm-10.0
}

/*
 * Respond to timing out waiting for Modem
 *
 * NOTE: The function never returns.
 */
static void msm_pm_timeout(void)
{
#if defined(CONFIG_MSM_PM_TIMEOUT_RESET_CHIP)
	printk(KERN_EMERG "%s(): resetting chip\n", __func__);
	msm_proc_comm(PCOM_RESET_CHIP_IMM, NULL, NULL);
#elif defined(CONFIG_MSM_PM_TIMEOUT_RESET_MODEM)
	printk(KERN_EMERG "%s(): resetting modem\n", __func__);
	msm_proc_comm_reset_modem_now();
#elif defined(CONFIG_MSM_PM_TIMEOUT_HALT)
	printk(KERN_EMERG "%s(): halting\n", __func__);
#endif
	for (;;)
		;
}


/******************************************************************************
 * State Polling Definitions
 *****************************************************************************/

struct msm_pm_polled_group {
	uint32_t group_id;

	uint32_t bits_all_set;
	uint32_t bits_all_clear;
	uint32_t bits_any_set;
	uint32_t bits_any_clear;

	uint32_t value_read;
};

/*
 * Return true if all bits indicated by flag are set in source.
 */
static inline bool msm_pm_all_set(uint32_t source, uint32_t flag)
{
	return (source & flag) == flag;
}

/*
 * Return true if any bit indicated by flag are set in source.
 */
static inline bool msm_pm_any_set(uint32_t source, uint32_t flag)
{
	return !flag || (source & flag);
}

/*
 * Return true if all bits indicated by flag are cleared in source.
 */
static inline bool msm_pm_all_clear(uint32_t source, uint32_t flag)
{
	return (~source & flag) == flag;
}

/*
 * Return true if any bit indicated by flag are cleared in source.
 */
static inline bool msm_pm_any_clear(uint32_t source, uint32_t flag)
{
	return !flag || (~source & flag);
}

/*
 * Poll the shared memory states as indicated by the poll groups.
 *
 * nr_grps: number of groups in the array
 * grps: array of groups
 *
 * The function returns when conditions specified by any of the poll
 * groups become true.  The conditions specified by a poll group are
 * deemed true when 1) at least one bit from bits_any_set is set OR one
 * bit from bits_any_clear is cleared; and 2) all bits in bits_all_set
 * are set; and 3) all bits in bits_all_clear are cleared.
 *
 * Return value:
 *      >=0: index of the poll group whose conditions have become true
 *      -ETIMEDOUT: timed out
 */
static int msm_pm_poll_state(int nr_grps, struct msm_pm_polled_group *grps)
{
	int i, k;

	for (i = 0; i < 50000; i++) {
		for (k = 0; k < nr_grps; k++) {
			bool all_set, all_clear;
			bool any_set, any_clear;

			grps[k].value_read = smsm_get_state(grps[k].group_id);

			all_set = msm_pm_all_set(grps[k].value_read,
					grps[k].bits_all_set);
			all_clear = msm_pm_all_clear(grps[k].value_read,
					grps[k].bits_all_clear);
			any_set = msm_pm_any_set(grps[k].value_read,
					grps[k].bits_any_set);
			any_clear = msm_pm_any_clear(grps[k].value_read,
					grps[k].bits_any_clear);

			if (all_set && all_clear && (any_set || any_clear))
				return k;
		}
		udelay(50);
	}

	printk(KERN_ERR "%s failed:\n", __func__);
	for (k = 0; k < nr_grps; k++)
		printk(KERN_ERR "(%x, %x, %x, %x) %x\n",
			grps[k].bits_all_set, grps[k].bits_all_clear,
			grps[k].bits_any_set, grps[k].bits_any_clear,
			grps[k].value_read);

	return -ETIMEDOUT;
}


/******************************************************************************
 * Suspend Max Sleep Time
 *****************************************************************************/

#define SCLK_HZ (32768)
#define MSM_PM_SLEEP_TICK_LIMIT (0x6DDD000)

#ifdef CONFIG_MSM_SLEEP_TIME_OVERRIDE
static int msm_pm_sleep_time_override;
module_param_named(sleep_time_override,
	msm_pm_sleep_time_override, int, S_IRUGO | S_IWUSR | S_IWGRP);
#endif

static uint32_t msm_pm_max_sleep_time;

/*
 * Convert time from nanoseconds to slow clock ticks, then cap it to the
 * specified limit
 */
static int64_t msm_pm_convert_and_cap_time(int64_t time_ns, int64_t limit)
{
	do_div(time_ns, NSEC_PER_SEC / SCLK_HZ);
	return (time_ns > limit) ? limit : time_ns;
}

/*
 * Set the sleep time for suspend.  0 means infinite sleep time.
 */
void msm_pm_set_max_sleep_time(int64_t max_sleep_time_ns)
{
	unsigned long flags;

	local_irq_save(flags);
	if (max_sleep_time_ns == 0) {
		msm_pm_max_sleep_time = 0;
	} else {
		msm_pm_max_sleep_time = (uint32_t)msm_pm_convert_and_cap_time(
			max_sleep_time_ns, MSM_PM_SLEEP_TICK_LIMIT);

		if (msm_pm_max_sleep_time == 0)
			msm_pm_max_sleep_time = 1;
	}

	MSM_PM_DPRINTK(MSM_PM_DEBUG_SUSPEND, KERN_INFO,
		"%s(): Requested %lld ns Giving %u sclk ticks\n", __func__,
		max_sleep_time_ns, msm_pm_max_sleep_time);
	local_irq_restore(flags);
}
EXPORT_SYMBOL(msm_pm_set_max_sleep_time);


/******************************************************************************
<<<<<<< HEAD
 * CONFIG_MSM_IDLE_STATS
 *****************************************************************************/

#ifdef CONFIG_MSM_IDLE_STATS
enum msm_pm_time_stats_id {
	MSM_PM_STAT_REQUESTED_IDLE,
	MSM_PM_STAT_IDLE_SPIN,
	MSM_PM_STAT_IDLE_WFI,
	MSM_PM_STAT_IDLE_STANDALONE_POWER_COLLAPSE,
	MSM_PM_STAT_IDLE_FAILED_STANDALONE_POWER_COLLAPSE,
	MSM_PM_STAT_IDLE_SLEEP,
	MSM_PM_STAT_IDLE_FAILED_SLEEP,
	MSM_PM_STAT_IDLE_POWER_COLLAPSE,
	MSM_PM_STAT_IDLE_FAILED_POWER_COLLAPSE,
	MSM_PM_STAT_SUSPEND,
	MSM_PM_STAT_FAILED_SUSPEND,
	MSM_PM_STAT_NOT_IDLE,
	MSM_PM_STAT_COUNT
};

static struct msm_pm_time_stats {
	const char *name;
	int64_t first_bucket_time;
	int bucket[CONFIG_MSM_IDLE_STATS_BUCKET_COUNT];
	int64_t min_time[CONFIG_MSM_IDLE_STATS_BUCKET_COUNT];
	int64_t max_time[CONFIG_MSM_IDLE_STATS_BUCKET_COUNT];
	int count;
	int64_t total_time;
} msm_pm_stats[MSM_PM_STAT_COUNT] = {
	[MSM_PM_STAT_REQUESTED_IDLE].name = "idle-request",
	[MSM_PM_STAT_REQUESTED_IDLE].first_bucket_time =
		CONFIG_MSM_IDLE_STATS_FIRST_BUCKET,

	[MSM_PM_STAT_IDLE_SPIN].name = "idle-spin",
	[MSM_PM_STAT_IDLE_SPIN].first_bucket_time =
		CONFIG_MSM_IDLE_STATS_FIRST_BUCKET,

	[MSM_PM_STAT_IDLE_WFI].name = "idle-wfi",
	[MSM_PM_STAT_IDLE_WFI].first_bucket_time =
		CONFIG_MSM_IDLE_STATS_FIRST_BUCKET,

	[MSM_PM_STAT_IDLE_STANDALONE_POWER_COLLAPSE].name =
		"idle-standalone-power-collapse",
	[MSM_PM_STAT_IDLE_STANDALONE_POWER_COLLAPSE].first_bucket_time =
		CONFIG_MSM_IDLE_STATS_FIRST_BUCKET,

	[MSM_PM_STAT_IDLE_FAILED_STANDALONE_POWER_COLLAPSE].name =
		"idle-failed-standalone-power-collapse",
	[MSM_PM_STAT_IDLE_FAILED_STANDALONE_POWER_COLLAPSE].first_bucket_time =
		CONFIG_MSM_IDLE_STATS_FIRST_BUCKET,

	[MSM_PM_STAT_IDLE_SLEEP].name = "idle-sleep",
	[MSM_PM_STAT_IDLE_SLEEP].first_bucket_time =
		CONFIG_MSM_IDLE_STATS_FIRST_BUCKET,

	[MSM_PM_STAT_IDLE_FAILED_SLEEP].name = "idle-failed-sleep",
	[MSM_PM_STAT_IDLE_FAILED_SLEEP].first_bucket_time =
		CONFIG_MSM_IDLE_STATS_FIRST_BUCKET,

	[MSM_PM_STAT_IDLE_POWER_COLLAPSE].name = "idle-power-collapse",
	[MSM_PM_STAT_IDLE_POWER_COLLAPSE].first_bucket_time =
		CONFIG_MSM_IDLE_STATS_FIRST_BUCKET,

	[MSM_PM_STAT_IDLE_FAILED_POWER_COLLAPSE].name =
		"idle-failed-power-collapse",
	[MSM_PM_STAT_IDLE_FAILED_POWER_COLLAPSE].first_bucket_time =
		CONFIG_MSM_IDLE_STATS_FIRST_BUCKET,

	[MSM_PM_STAT_SUSPEND].name = "suspend",
	[MSM_PM_STAT_SUSPEND].first_bucket_time =
		CONFIG_MSM_SUSPEND_STATS_FIRST_BUCKET,

	[MSM_PM_STAT_FAILED_SUSPEND].name = "failed-suspend",
	[MSM_PM_STAT_FAILED_SUSPEND].first_bucket_time =
		CONFIG_MSM_IDLE_STATS_FIRST_BUCKET,

	[MSM_PM_STAT_NOT_IDLE].name = "not-idle",
	[MSM_PM_STAT_NOT_IDLE].first_bucket_time =
		CONFIG_MSM_IDLE_STATS_FIRST_BUCKET,
};

static uint32_t msm_pm_sleep_limit = SLEEP_LIMIT_NONE;

/*
 * Add the given time data to the statistics collection.
 */
static void msm_pm_add_stat(enum msm_pm_time_stats_id id, int64_t t)
{
	int i;
	int64_t bt;

	msm_pm_stats[id].total_time += t;
	msm_pm_stats[id].count++;

	bt = t;
	do_div(bt, msm_pm_stats[id].first_bucket_time);

	if (bt < 1ULL << (CONFIG_MSM_IDLE_STATS_BUCKET_SHIFT *
				(CONFIG_MSM_IDLE_STATS_BUCKET_COUNT - 1)))
		i = DIV_ROUND_UP(fls((uint32_t)bt),
					CONFIG_MSM_IDLE_STATS_BUCKET_SHIFT);
	else
		i = CONFIG_MSM_IDLE_STATS_BUCKET_COUNT - 1;

	msm_pm_stats[id].bucket[i]++;

	if (t < msm_pm_stats[id].min_time[i] || !msm_pm_stats[id].max_time[i])
		msm_pm_stats[id].min_time[i] = t;
	if (t > msm_pm_stats[id].max_time[i])
		msm_pm_stats[id].max_time[i] = t;
}

/*
 * Helper function of snprintf where buf is auto-incremented, size is auto-
 * decremented, and there is no return value.
 *
 * NOTE: buf and size must be l-values (e.g. variables)
 */
#define SNPRINTF(buf, size, format, ...) \
	do { \
		if (size > 0) { \
			int ret; \
			ret = snprintf(buf, size, format, ## __VA_ARGS__); \
			if (ret > size) { \
				buf += size; \
				size = 0; \
			} else { \
				buf += ret; \
				size -= ret; \
			} \
		} \
	} while (0)

/*
 * Write out the power management statistics.
 */
static int msm_pm_read_proc
	(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	int i;
	char *p = page;

	if (count < 1024) {
		*start = (char *) 0;
		*eof = 0;
		return 0;
	}

	if (!off) {
		SNPRINTF(p, count, "Last power collapse voted ");
		if ((msm_pm_sleep_limit & SLEEP_LIMIT_MASK) ==
			SLEEP_LIMIT_NONE)
			SNPRINTF(p, count, "for TCXO shutdown\n\n");
		else
			SNPRINTF(p, count, "against TCXO shutdown\n\n");

		*start = (char *) 1;
		*eof = 0;
	} else if (--off < ARRAY_SIZE(msm_pm_stats)) {
		int64_t bucket_time;
		int64_t s;
		uint32_t ns;

		s = msm_pm_stats[off].total_time;
		ns = do_div(s, NSEC_PER_SEC);
		SNPRINTF(p, count,
			"%s:\n"
			"  count: %7d\n"
			"  total_time: %lld.%09u\n",
			msm_pm_stats[off].name,
			msm_pm_stats[off].count,
			s, ns);

		bucket_time = msm_pm_stats[off].first_bucket_time;
		for (i = 0; i < CONFIG_MSM_IDLE_STATS_BUCKET_COUNT - 1; i++) {
			s = bucket_time;
			ns = do_div(s, NSEC_PER_SEC);
			SNPRINTF(p, count,
				"   <%6lld.%09u: %7d (%lld-%lld)\n",
				s, ns, msm_pm_stats[off].bucket[i],
				msm_pm_stats[off].min_time[i],
				msm_pm_stats[off].max_time[i]);

			bucket_time <<= CONFIG_MSM_IDLE_STATS_BUCKET_SHIFT;
		}

		SNPRINTF(p, count, "  >=%6lld.%09u: %7d (%lld-%lld)\n",
			s, ns, msm_pm_stats[off].bucket[i],
			msm_pm_stats[off].min_time[i],
			msm_pm_stats[off].max_time[i]);

		*start = (char *) 1;
		*eof = (off + 1 >= ARRAY_SIZE(msm_pm_stats));
	}

	return p - page;
}
#undef SNPRINTF

#define MSM_PM_STATS_RESET "reset"

/*
 * Reset the power management statistics values.
 */
static int msm_pm_write_proc(struct file *file, const char __user *buffer,
	unsigned long count, void *data)
{
	char buf[sizeof(MSM_PM_STATS_RESET)];
	int ret;
	unsigned long flags;
	int i;

	if (count < strlen(MSM_PM_STATS_RESET)) {
		ret = -EINVAL;
		goto write_proc_failed;
	}

	if (copy_from_user(buf, buffer, strlen(MSM_PM_STATS_RESET))) {
		ret = -EFAULT;
		goto write_proc_failed;
	}

	if (memcmp(buf, MSM_PM_STATS_RESET, strlen(MSM_PM_STATS_RESET))) {
		ret = -EINVAL;
		goto write_proc_failed;
	}

	local_irq_save(flags);
	for (i = 0; i < ARRAY_SIZE(msm_pm_stats); i++) {
		memset(msm_pm_stats[i].bucket,
			0, sizeof(msm_pm_stats[i].bucket));
		memset(msm_pm_stats[i].min_time,
			0, sizeof(msm_pm_stats[i].min_time));
		memset(msm_pm_stats[i].max_time,
			0, sizeof(msm_pm_stats[i].max_time));
		msm_pm_stats[i].count = 0;
		msm_pm_stats[i].total_time = 0;
	}

	msm_pm_sleep_limit = SLEEP_LIMIT_NONE;
	local_irq_restore(flags);

	return count;

write_proc_failed:
	return ret;
}
#undef MSM_PM_STATS_RESET
#endif /* CONFIG_MSM_IDLE_STATS */


/******************************************************************************
=======
>>>>>>> refs/remotes/origin/cm-10.0
 * Shared Memory Bits
 *****************************************************************************/

#define DEM_MASTER_BITS_PER_CPU             6

/* Power Master State Bits - Per CPU */
#define DEM_MASTER_SMSM_RUN \
	(0x01UL << (DEM_MASTER_BITS_PER_CPU * SMSM_APPS_STATE))
#define DEM_MASTER_SMSM_RSA \
	(0x02UL << (DEM_MASTER_BITS_PER_CPU * SMSM_APPS_STATE))
#define DEM_MASTER_SMSM_PWRC_EARLY_EXIT \
	(0x04UL << (DEM_MASTER_BITS_PER_CPU * SMSM_APPS_STATE))
#define DEM_MASTER_SMSM_SLEEP_EXIT \
	(0x08UL << (DEM_MASTER_BITS_PER_CPU * SMSM_APPS_STATE))
#define DEM_MASTER_SMSM_READY \
	(0x10UL << (DEM_MASTER_BITS_PER_CPU * SMSM_APPS_STATE))
#define DEM_MASTER_SMSM_SLEEP \
	(0x20UL << (DEM_MASTER_BITS_PER_CPU * SMSM_APPS_STATE))

/* Power Slave State Bits */
#define DEM_SLAVE_SMSM_RUN                  (0x0001)
#define DEM_SLAVE_SMSM_PWRC                 (0x0002)
#define DEM_SLAVE_SMSM_PWRC_DELAY           (0x0004)
#define DEM_SLAVE_SMSM_PWRC_EARLY_EXIT      (0x0008)
#define DEM_SLAVE_SMSM_WFPI                 (0x0010)
#define DEM_SLAVE_SMSM_SLEEP                (0x0020)
#define DEM_SLAVE_SMSM_SLEEP_EXIT           (0x0040)
#define DEM_SLAVE_SMSM_MSGS_REDUCED         (0x0080)
#define DEM_SLAVE_SMSM_RESET                (0x0100)
#define DEM_SLAVE_SMSM_PWRC_SUSPEND         (0x0200)


/******************************************************************************
 * Shared Memory Data
 *****************************************************************************/

#define DEM_MAX_PORT_NAME_LEN (20)

struct msm_pm_smem_t {
	uint32_t sleep_time;
	uint32_t irq_mask;
	uint32_t resources_used;
	uint32_t reserved1;

	uint32_t wakeup_reason;
	uint32_t pending_irqs;
	uint32_t rpc_prog;
	uint32_t rpc_proc;
	char     smd_port_name[DEM_MAX_PORT_NAME_LEN];
	uint32_t reserved2;
};


/******************************************************************************
 *
 *****************************************************************************/
static struct msm_pm_smem_t *msm_pm_smem_data;
static atomic_t msm_pm_init_done = ATOMIC_INIT(0);

static int msm_pm_modem_busy(void)
{
	if (!(smsm_get_state(SMSM_POWER_MASTER_DEM) & DEM_MASTER_SMSM_READY)) {
		MSM_PM_DPRINTK(MSM_PM_DEBUG_POWER_COLLAPSE,
			KERN_INFO, "%s(): master not ready\n", __func__);
		return -EBUSY;
	}

	return 0;
}

/*
 * Power collapse the Apps processor.  This function executes the handshake
 * protocol with Modem.
 *
 * Return value:
 *      -EAGAIN: modem reset occurred or early exit from power collapse
 *      -EBUSY: modem not ready for our power collapse -- no power loss
 *      -ETIMEDOUT: timed out waiting for modem's handshake -- no power loss
 *      0: success
 */
static int msm_pm_power_collapse
	(bool from_idle, uint32_t sleep_delay, uint32_t sleep_limit)
{
	struct msm_pm_polled_group state_grps[2];
	unsigned long saved_acpuclk_rate;
	int collapsed = 0;
	int ret;
<<<<<<< HEAD
=======
	int val;
	int modem_early_exit = 0;
>>>>>>> refs/remotes/origin/cm-10.0

	MSM_PM_DPRINTK(MSM_PM_DEBUG_SUSPEND|MSM_PM_DEBUG_POWER_COLLAPSE,
		KERN_INFO, "%s(): idle %d, delay %u, limit %u\n", __func__,
		(int)from_idle, sleep_delay, sleep_limit);

	if (!(smsm_get_state(SMSM_POWER_MASTER_DEM) & DEM_MASTER_SMSM_READY)) {
		MSM_PM_DPRINTK(
			MSM_PM_DEBUG_SUSPEND | MSM_PM_DEBUG_POWER_COLLAPSE,
			KERN_INFO, "%s(): master not ready\n", __func__);
		ret = -EBUSY;
		goto power_collapse_bail;
	}

	memset(msm_pm_smem_data, 0, sizeof(*msm_pm_smem_data));

<<<<<<< HEAD
	msm_irq_enter_sleep1(true, from_idle, &msm_pm_smem_data->irq_mask);
=======
	if (msm_cpr_ops && msm_cpr_ops->cpr_suspend()) {
		ret = -EAGAIN;
		goto power_collapse_bail;
	}

	if (cpu_is_msm8625() || cpu_is_msm8625q()) {
		/* Program the SPM */
		ret = msm_spm_set_low_power_mode(MSM_SPM_MODE_POWER_COLLAPSE,
									false);
		WARN_ON(ret);
	}

	msm_pm_irq_extns->enter_sleep1(true, from_idle,
						&msm_pm_smem_data->irq_mask);
>>>>>>> refs/remotes/origin/cm-10.0
	msm_sirc_enter_sleep();
	msm_gpio_enter_sleep(from_idle);

	msm_pm_smem_data->sleep_time = sleep_delay;
	msm_pm_smem_data->resources_used = sleep_limit;

	/* Enter PWRC/PWRC_SUSPEND */

	if (from_idle)
		smsm_change_state(SMSM_APPS_DEM, DEM_SLAVE_SMSM_RUN,
			DEM_SLAVE_SMSM_PWRC);
	else
		smsm_change_state(SMSM_APPS_DEM, DEM_SLAVE_SMSM_RUN,
			DEM_SLAVE_SMSM_PWRC | DEM_SLAVE_SMSM_PWRC_SUSPEND);

	MSM_PM_DEBUG_PRINT_STATE("msm_pm_power_collapse(): PWRC");
	MSM_PM_DEBUG_PRINT_SLEEP_INFO();

	memset(state_grps, 0, sizeof(state_grps));
	state_grps[0].group_id = SMSM_POWER_MASTER_DEM;
	state_grps[0].bits_all_set = DEM_MASTER_SMSM_RSA;
	state_grps[1].group_id = SMSM_MODEM_STATE;
	state_grps[1].bits_all_set = SMSM_RESET;

	ret = msm_pm_poll_state(ARRAY_SIZE(state_grps), state_grps);

	if (ret < 0) {
		printk(KERN_EMERG "%s(): power collapse entry "
			"timed out waiting for Modem's response\n", __func__);
		msm_pm_timeout();
	}

	if (ret == 1) {
		MSM_PM_DPRINTK(
			MSM_PM_DEBUG_SUSPEND|MSM_PM_DEBUG_POWER_COLLAPSE,
			KERN_INFO,
			"%s(): msm_pm_poll_state detected Modem reset\n",
			__func__);
		goto power_collapse_early_exit;
	}

	/* DEM Master in RSA */

	MSM_PM_DEBUG_PRINT_STATE("msm_pm_power_collapse(): PWRC RSA");

<<<<<<< HEAD
	ret = msm_irq_enter_sleep2(true, from_idle);
=======
	ret = msm_pm_irq_extns->enter_sleep2(true, from_idle);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret < 0) {
		MSM_PM_DPRINTK(
			MSM_PM_DEBUG_SUSPEND|MSM_PM_DEBUG_POWER_COLLAPSE,
			KERN_INFO,
			"%s(): msm_irq_enter_sleep2 aborted, %d\n", __func__,
			ret);
		goto power_collapse_early_exit;
	}

	msm_pm_config_hw_before_power_down();
	MSM_PM_DEBUG_PRINT_STATE("msm_pm_power_collapse(): pre power down");

	saved_acpuclk_rate = acpuclk_power_collapse();
	MSM_PM_DPRINTK(MSM_PM_DEBUG_CLOCK, KERN_INFO,
		"%s(): change clock rate (old rate = %lu)\n", __func__,
		saved_acpuclk_rate);

	if (saved_acpuclk_rate == 0) {
		msm_pm_config_hw_after_power_up();
<<<<<<< HEAD
		goto power_collapse_early_exit;
=======
		goto acpu_switch_fail;
	}

	/* save the AHB clock registers */
	if (cpu_is_msm8625q()) {
		msm8x25q_ahb.sel = readl_relaxed(A11S_CLK_SEL_ADDR);
		msm8x25q_ahb.cntl = readl_relaxed(A11S_CLK_CNTL_ADDR);
>>>>>>> refs/remotes/origin/cm-10.0
	}

	msm_pm_boot_config_before_pc(smp_processor_id(),
			virt_to_phys(msm_pm_collapse_exit));

#ifdef CONFIG_VFP
	if (from_idle)
<<<<<<< HEAD
		vfp_flush_context();
#endif

#ifdef CONFIG_CACHE_L2X0
	l2x0_suspend();
=======
		vfp_pm_suspend();
#endif

#ifdef CONFIG_CACHE_L2X0
	if (!cpu_is_msm8625() && !cpu_is_msm8625q())
		l2cc_suspend();
	else
		apps_power_collapse = 1;
>>>>>>> refs/remotes/origin/cm-10.0
#endif

	collapsed = msm_pm_collapse();

<<<<<<< HEAD
#ifdef CONFIG_CACHE_L2X0
	l2x0_resume(collapsed);
=======
	/*
	 * TBD: Currently recognise the MODEM early exit
	 * path by reading the MPA5_GDFS_CNT_VAL register.
	 */
	if (cpu_is_msm8625() || cpu_is_msm8625q()) {
		int cpu;
		/*
		 * on system reset, default value of MPA5_GDFS_CNT_VAL
		 * is = 0x0, later modem reprogram this value to
		 * 0x00030004/0x000F0004(8x25Q). Once APPS did
		 * a power collapse and coming out of it expected value
		 * of this register always be 0x00030004/0x000F0004(8x25Q).
		 * Incase if APPS sees the value as 0x00030002/0x000F0002(8x25Q)
		 * consider this case as a modem early exit.
		 */
		val = __raw_readl(MSM_CFG_CTL_BASE + 0x38);

		/* 8x25Q */
		if (cpu_is_msm8625q()) {
			if (val != 0x000F0002) {
				for_each_possible_cpu(cpu) {
					if (!cpu)
						continue;
					per_cpu(power_collapsed, cpu) = 1;
				}
				/*
				 * override DBGNOPOWERDN and program the GDFS
				 * count val
				 */
				 __raw_writel(0x000F0002,
						 (MSM_CFG_CTL_BASE + 0x38));
			} else
				modem_early_exit = 1;
		} else {
			if (val != 0x00030002) {
				for_each_possible_cpu(cpu) {
					if (!cpu)
						continue;
					per_cpu(power_collapsed, cpu) = 1;
				}
				/*
				 * override DBGNOPOWERDN and program the GDFS
				 * count val
				 */
				 __raw_writel(0x00030002,
						 (MSM_CFG_CTL_BASE + 0x38));
			} else
				modem_early_exit = 1;
		}
	}

#ifdef CONFIG_CACHE_L2X0
	if (!cpu_is_msm8625() && !cpu_is_msm8625q())
		l2cc_resume();
	else
		apps_power_collapse = 0;
>>>>>>> refs/remotes/origin/cm-10.0
#endif

	msm_pm_boot_config_after_pc(smp_processor_id());

	if (collapsed) {
#ifdef CONFIG_VFP
		if (from_idle)
<<<<<<< HEAD
			vfp_reinit();
=======
			vfp_pm_resume();
>>>>>>> refs/remotes/origin/cm-10.0
#endif
		cpu_init();
		local_fiq_enable();
	}

	MSM_PM_DPRINTK(MSM_PM_DEBUG_SUSPEND | MSM_PM_DEBUG_POWER_COLLAPSE,
		KERN_INFO,
		"%s(): msm_pm_collapse returned %d\n", __func__, collapsed);

	MSM_PM_DPRINTK(MSM_PM_DEBUG_CLOCK, KERN_INFO,
		"%s(): restore clock rate to %lu\n", __func__,
		saved_acpuclk_rate);
	if (acpuclk_set_rate(smp_processor_id(), saved_acpuclk_rate,
			SETRATE_PC) < 0)
		printk(KERN_ERR "%s(): failed to restore clock rate(%lu)\n",
			__func__, saved_acpuclk_rate);

<<<<<<< HEAD
	msm_irq_exit_sleep1(msm_pm_smem_data->irq_mask,
=======
	msm_pm_irq_extns->exit_sleep1(msm_pm_smem_data->irq_mask,
>>>>>>> refs/remotes/origin/cm-10.0
		msm_pm_smem_data->wakeup_reason,
		msm_pm_smem_data->pending_irqs);

	msm_pm_config_hw_after_power_up();
	MSM_PM_DEBUG_PRINT_STATE("msm_pm_power_collapse(): post power up");

	memset(state_grps, 0, sizeof(state_grps));
	state_grps[0].group_id = SMSM_POWER_MASTER_DEM;
	state_grps[0].bits_any_set =
		DEM_MASTER_SMSM_RSA | DEM_MASTER_SMSM_PWRC_EARLY_EXIT;
	state_grps[1].group_id = SMSM_MODEM_STATE;
	state_grps[1].bits_all_set = SMSM_RESET;

	ret = msm_pm_poll_state(ARRAY_SIZE(state_grps), state_grps);

	if (ret < 0) {
		printk(KERN_EMERG "%s(): power collapse exit "
			"timed out waiting for Modem's response\n", __func__);
		msm_pm_timeout();
	}

	if (ret == 1) {
		MSM_PM_DPRINTK(
			MSM_PM_DEBUG_SUSPEND|MSM_PM_DEBUG_POWER_COLLAPSE,
			KERN_INFO,
			"%s(): msm_pm_poll_state detected Modem reset\n",
			__func__);
		goto power_collapse_early_exit;
	}

	/* Sanity check */
<<<<<<< HEAD
	if (collapsed) {
=======
	if (collapsed && !modem_early_exit) {
>>>>>>> refs/remotes/origin/cm-10.0
		BUG_ON(!(state_grps[0].value_read & DEM_MASTER_SMSM_RSA));
	} else {
		BUG_ON(!(state_grps[0].value_read &
			DEM_MASTER_SMSM_PWRC_EARLY_EXIT));
		goto power_collapse_early_exit;
	}

	/* Enter WFPI */

	smsm_change_state(SMSM_APPS_DEM,
		DEM_SLAVE_SMSM_PWRC | DEM_SLAVE_SMSM_PWRC_SUSPEND,
		DEM_SLAVE_SMSM_WFPI);

	MSM_PM_DEBUG_PRINT_STATE("msm_pm_power_collapse(): WFPI");

	memset(state_grps, 0, sizeof(state_grps));
	state_grps[0].group_id = SMSM_POWER_MASTER_DEM;
	state_grps[0].bits_all_set = DEM_MASTER_SMSM_RUN;
	state_grps[1].group_id = SMSM_MODEM_STATE;
	state_grps[1].bits_all_set = SMSM_RESET;

	ret = msm_pm_poll_state(ARRAY_SIZE(state_grps), state_grps);

	if (ret < 0) {
		printk(KERN_EMERG "%s(): power collapse WFPI "
			"timed out waiting for Modem's response\n", __func__);
		msm_pm_timeout();
	}

	if (ret == 1) {
		MSM_PM_DPRINTK(
			MSM_PM_DEBUG_SUSPEND|MSM_PM_DEBUG_POWER_COLLAPSE,
			KERN_INFO,
			"%s(): msm_pm_poll_state detected Modem reset\n",
			__func__);
		ret = -EAGAIN;
		goto power_collapse_restore_gpio_bail;
	}

	/* DEM Master == RUN */

	MSM_PM_DEBUG_PRINT_STATE("msm_pm_power_collapse(): WFPI RUN");
	MSM_PM_DEBUG_PRINT_SLEEP_INFO();

<<<<<<< HEAD
	msm_irq_exit_sleep2(msm_pm_smem_data->irq_mask,
		msm_pm_smem_data->wakeup_reason,
		msm_pm_smem_data->pending_irqs);
	msm_irq_exit_sleep3(msm_pm_smem_data->irq_mask,
=======
	msm_pm_irq_extns->exit_sleep2(msm_pm_smem_data->irq_mask,
		msm_pm_smem_data->wakeup_reason,
		msm_pm_smem_data->pending_irqs);
	msm_pm_irq_extns->exit_sleep3(msm_pm_smem_data->irq_mask,
>>>>>>> refs/remotes/origin/cm-10.0
		msm_pm_smem_data->wakeup_reason,
		msm_pm_smem_data->pending_irqs);
	msm_gpio_exit_sleep();
	msm_sirc_exit_sleep();

	smsm_change_state(SMSM_APPS_DEM,
		DEM_SLAVE_SMSM_WFPI, DEM_SLAVE_SMSM_RUN);

	MSM_PM_DEBUG_PRINT_STATE("msm_pm_power_collapse(): RUN");

	smd_sleep_exit();
<<<<<<< HEAD
	return 0;

=======

	if (cpu_is_msm8625()) {
		ret = msm_spm_set_low_power_mode(MSM_SPM_MODE_CLOCK_GATING,
									false);
		WARN_ON(ret);
	}

	if (msm_cpr_ops)
		msm_cpr_ops->cpr_resume();

	return 0;

acpu_switch_fail:
	msm_pm_irq_extns->exit_sleep1(msm_pm_smem_data->irq_mask,
		msm_pm_smem_data->wakeup_reason,
		msm_pm_smem_data->pending_irqs);

>>>>>>> refs/remotes/origin/cm-10.0
power_collapse_early_exit:
	/* Enter PWRC_EARLY_EXIT */

	smsm_change_state(SMSM_APPS_DEM,
		DEM_SLAVE_SMSM_PWRC | DEM_SLAVE_SMSM_PWRC_SUSPEND,
		DEM_SLAVE_SMSM_PWRC_EARLY_EXIT);

	MSM_PM_DEBUG_PRINT_STATE("msm_pm_power_collapse(): EARLY_EXIT");

	memset(state_grps, 0, sizeof(state_grps));
	state_grps[0].group_id = SMSM_POWER_MASTER_DEM;
	state_grps[0].bits_all_set = DEM_MASTER_SMSM_PWRC_EARLY_EXIT;
	state_grps[1].group_id = SMSM_MODEM_STATE;
	state_grps[1].bits_all_set = SMSM_RESET;

	ret = msm_pm_poll_state(ARRAY_SIZE(state_grps), state_grps);
	MSM_PM_DEBUG_PRINT_STATE("msm_pm_power_collapse(): EARLY_EXIT EE");

	if (ret < 0) {
		printk(KERN_EMERG "%s(): power collapse EARLY_EXIT "
			"timed out waiting for Modem's response\n", __func__);
		msm_pm_timeout();
	}

	if (ret == 1) {
		MSM_PM_DPRINTK(
			MSM_PM_DEBUG_SUSPEND|MSM_PM_DEBUG_POWER_COLLAPSE,
			KERN_INFO,
			"%s(): msm_pm_poll_state detected Modem reset\n",
			__func__);
	}

	/* DEM Master == RESET or PWRC_EARLY_EXIT */

	ret = -EAGAIN;

power_collapse_restore_gpio_bail:
	msm_gpio_exit_sleep();
	msm_sirc_exit_sleep();

	/* Enter RUN */
	smsm_change_state(SMSM_APPS_DEM,
		DEM_SLAVE_SMSM_PWRC | DEM_SLAVE_SMSM_PWRC_SUSPEND |
		DEM_SLAVE_SMSM_PWRC_EARLY_EXIT, DEM_SLAVE_SMSM_RUN);

	MSM_PM_DEBUG_PRINT_STATE("msm_pm_power_collapse(): RUN");

	if (collapsed)
		smd_sleep_exit();

<<<<<<< HEAD
power_collapse_bail:
=======
	if (msm_cpr_ops)
		msm_cpr_ops->cpr_resume();

power_collapse_bail:
	if (cpu_is_msm8625()) {
		ret = msm_spm_set_low_power_mode(MSM_SPM_MODE_CLOCK_GATING,
									false);
		WARN_ON(ret);
	}

>>>>>>> refs/remotes/origin/cm-10.0
	return ret;
}

/*
 * Power collapse the Apps processor without involving Modem.
 *
 * Return value:
 *      0: success
 */
<<<<<<< HEAD
static int msm_pm_power_collapse_standalone(void)
{
	int collapsed = 0;
	int ret;
=======
static int __ref msm_pm_power_collapse_standalone(bool from_idle)
{
	int collapsed = 0;
	int ret;
	void *entry;
>>>>>>> refs/remotes/origin/cm-10.0

	MSM_PM_DPRINTK(MSM_PM_DEBUG_SUSPEND|MSM_PM_DEBUG_POWER_COLLAPSE,
		KERN_INFO, "%s()\n", __func__);

	ret = msm_spm_set_low_power_mode(MSM_SPM_MODE_POWER_COLLAPSE, false);
	WARN_ON(ret);

<<<<<<< HEAD
	msm_pm_boot_config_before_pc(smp_processor_id(),
			virt_to_phys(msm_pm_collapse_exit));

#ifdef CONFIG_VFP
	vfp_flush_context();
#endif

#ifdef CONFIG_CACHE_L2X0
	l2x0_suspend();
=======
	entry = (!smp_processor_id() || from_idle) ?
			msm_pm_collapse_exit : msm_secondary_startup;

	msm_pm_boot_config_before_pc(smp_processor_id(),
						virt_to_phys(entry));

#ifdef CONFIG_VFP
	vfp_pm_suspend();
#endif

#ifdef CONFIG_CACHE_L2X0
	if (!cpu_is_msm8625() && !cpu_is_msm8625q())
		l2cc_suspend();
>>>>>>> refs/remotes/origin/cm-10.0
#endif

	collapsed = msm_pm_collapse();

#ifdef CONFIG_CACHE_L2X0
<<<<<<< HEAD
	l2x0_resume(collapsed);
=======
	if (!cpu_is_msm8625() && !cpu_is_msm8625q())
		l2cc_resume();
>>>>>>> refs/remotes/origin/cm-10.0
#endif

	msm_pm_boot_config_after_pc(smp_processor_id());

	if (collapsed) {
#ifdef CONFIG_VFP
<<<<<<< HEAD
		vfp_reinit();
=======
		vfp_pm_resume();
>>>>>>> refs/remotes/origin/cm-10.0
#endif
		cpu_init();
		local_fiq_enable();
	}

	MSM_PM_DPRINTK(MSM_PM_DEBUG_SUSPEND | MSM_PM_DEBUG_POWER_COLLAPSE,
		KERN_INFO,
		"%s(): msm_pm_collapse returned %d\n", __func__, collapsed);

	ret = msm_spm_set_low_power_mode(MSM_SPM_MODE_CLOCK_GATING, false);
	WARN_ON(ret);

<<<<<<< HEAD
	return 0;
}

/*
 * Apps-sleep the Apps processor.  This function execute the handshake
 * protocol with Modem.
 *
 * Return value:
 *      -ENOSYS: function not implemented yet
 */
static int msm_pm_apps_sleep(uint32_t sleep_delay, uint32_t sleep_limit)
{
	return -ENOSYS;
=======
	return !collapsed;
>>>>>>> refs/remotes/origin/cm-10.0
}

/*
 * Bring the Apps processor to SWFI.
 *
 * Return value:
 *      -EIO: could not ramp Apps processor clock
 *      0: success
 */
static int msm_pm_swfi(bool ramp_acpu)
{
	unsigned long saved_acpuclk_rate = 0;

	if (ramp_acpu) {
		saved_acpuclk_rate = acpuclk_wait_for_irq();
		MSM_PM_DPRINTK(MSM_PM_DEBUG_CLOCK, KERN_INFO,
			"%s(): change clock rate (old rate = %lu)\n", __func__,
			saved_acpuclk_rate);

		if (!saved_acpuclk_rate)
			return -EIO;
	}

<<<<<<< HEAD
	msm_pm_config_hw_before_swfi();
=======
	if (!cpu_is_msm8625() && !cpu_is_msm8625q())
		msm_pm_config_hw_before_swfi();

>>>>>>> refs/remotes/origin/cm-10.0
	msm_arch_idle();

	if (ramp_acpu) {
		MSM_PM_DPRINTK(MSM_PM_DEBUG_CLOCK, KERN_INFO,
			"%s(): restore clock rate to %lu\n", __func__,
			saved_acpuclk_rate);
		if (acpuclk_set_rate(smp_processor_id(), saved_acpuclk_rate,
				SETRATE_SWFI) < 0)
			printk(KERN_ERR
				"%s(): failed to restore clock rate(%lu)\n",
				__func__, saved_acpuclk_rate);
	}

	return 0;
}

<<<<<<< HEAD
=======
static int64_t msm_pm_timer_enter_suspend(int64_t *period)
{
	int time = 0;

	time = msm_timer_get_sclk_time(period);
	if (!time)
		pr_err("%s: Unable to read sclk.\n", __func__);
		return time;
}

static int64_t msm_pm_timer_exit_suspend(int64_t time, int64_t period)
{

	if (time != 0) {
		int64_t end_time = msm_timer_get_sclk_time(NULL);
		if (end_time != 0) {
			time = end_time - time;
			if (time < 0)
				time += period;
			} else
				time = 0;
		}
	return time;
}
>>>>>>> refs/remotes/origin/cm-10.0

/******************************************************************************
 * External Idle/Suspend Functions
 *****************************************************************************/

/*
 * Put CPU in low power mode.
 */
void arch_idle(void)
{
	bool allow[MSM_PM_SLEEP_MODE_NR];
	uint32_t sleep_limit = SLEEP_LIMIT_NONE;

<<<<<<< HEAD
	int latency_qos;
	int64_t timer_expiration;

	int low_power;
	int ret;
	int i;

#ifdef CONFIG_MSM_IDLE_STATS
	int64_t t1;
	static int64_t t2;
	int exit_stat;
#endif /* CONFIG_MSM_IDLE_STATS */
=======
	int64_t timer_expiration;
	int latency_qos;
	int ret;
	int i;
	unsigned int cpu;
	int64_t t1;
	static DEFINE_PER_CPU(int64_t, t2);
	volatile int exit_stat;
	bool low_power = false;
>>>>>>> refs/remotes/origin/cm-10.0

	if (!atomic_read(&msm_pm_init_done))
		return;

<<<<<<< HEAD
	latency_qos = pm_qos_request(PM_QOS_CPU_DMA_LATENCY);
	timer_expiration = msm_timer_enter_idle();

#ifdef CONFIG_MSM_IDLE_STATS
	t1 = ktime_to_ns(ktime_get());
	msm_pm_add_stat(MSM_PM_STAT_NOT_IDLE, t1 - t2);
	msm_pm_add_stat(MSM_PM_STAT_REQUESTED_IDLE, timer_expiration);
#endif /* CONFIG_MSM_IDLE_STATS */
=======
	cpu = smp_processor_id();
	latency_qos = pm_qos_request(PM_QOS_CPU_DMA_LATENCY);
	/* get the next timer expiration */
	timer_expiration = ktime_to_ns(tick_nohz_get_sleep_length());

	t1 = ktime_to_ns(ktime_get());
	msm_pm_add_stat(MSM_PM_STAT_NOT_IDLE, t1 - __get_cpu_var(t2));
	msm_pm_add_stat(MSM_PM_STAT_REQUESTED_IDLE, timer_expiration);
	exit_stat = MSM_PM_STAT_IDLE_SPIN;
>>>>>>> refs/remotes/origin/cm-10.0

	for (i = 0; i < ARRAY_SIZE(allow); i++)
		allow[i] = true;

<<<<<<< HEAD
	switch (msm_pm_idle_sleep_mode) {
	case MSM_PM_SLEEP_MODE_WAIT_FOR_INTERRUPT:
		allow[MSM_PM_SLEEP_MODE_RAMP_DOWN_AND_WAIT_FOR_INTERRUPT] =
			false;
		/* fall through */
	case MSM_PM_SLEEP_MODE_RAMP_DOWN_AND_WAIT_FOR_INTERRUPT:
		allow[MSM_PM_SLEEP_MODE_POWER_COLLAPSE_STANDALONE] = false;
		/* fall through */
	case MSM_PM_SLEEP_MODE_POWER_COLLAPSE_STANDALONE:
		allow[MSM_PM_SLEEP_MODE_APPS_SLEEP] = false;
		/* fall through */
	case MSM_PM_SLEEP_MODE_APPS_SLEEP:
		allow[MSM_PM_SLEEP_MODE_POWER_COLLAPSE_NO_XO_SHUTDOWN] = false;
		allow[MSM_PM_SLEEP_MODE_POWER_COLLAPSE] = false;
		/* fall through */
	case MSM_PM_SLEEP_MODE_POWER_COLLAPSE_SUSPEND:
	case MSM_PM_SLEEP_MODE_POWER_COLLAPSE:
		break;
	default:
		printk(KERN_ERR "idle sleep mode is invalid: %d\n",
			msm_pm_idle_sleep_mode);
#ifdef CONFIG_MSM_IDLE_STATS
		exit_stat = MSM_PM_STAT_IDLE_SPIN;
#endif /* CONFIG_MSM_IDLE_STATS */
		low_power = 0;
		goto arch_idle_exit;
	}

	if ((timer_expiration < msm_pm_idle_sleep_min_time) ||
#ifdef CONFIG_HAS_WAKELOCK
		has_wake_lock(WAKE_LOCK_IDLE) ||
#endif
		!msm_irq_idle_sleep_allowed()) {
		allow[MSM_PM_SLEEP_MODE_POWER_COLLAPSE] = false;
		allow[MSM_PM_SLEEP_MODE_POWER_COLLAPSE_NO_XO_SHUTDOWN] = false;
		allow[MSM_PM_SLEEP_MODE_APPS_SLEEP] = false;
	}

	for (i = 0; i < ARRAY_SIZE(allow); i++) {
		struct msm_pm_platform_data *mode = &msm_pm_modes[i];
=======
	if (num_online_cpus() > 1 ||
		(timer_expiration < msm_pm_idle_sleep_min_time) ||
		!msm_pm_irq_extns->idle_sleep_allowed()) {
		allow[MSM_PM_SLEEP_MODE_POWER_COLLAPSE] = false;
		allow[MSM_PM_SLEEP_MODE_POWER_COLLAPSE_NO_XO_SHUTDOWN] = false;
	}

	for (i = 0; i < ARRAY_SIZE(allow); i++) {
		struct msm_pm_platform_data *mode =
					&msm_pm_modes[MSM_PM_MODE(cpu, i)];
>>>>>>> refs/remotes/origin/cm-10.0
		if (!mode->idle_supported || !mode->idle_enabled ||
			mode->latency >= latency_qos ||
			mode->residency * 1000ULL >= timer_expiration)
			allow[i] = false;
	}

	if (allow[MSM_PM_SLEEP_MODE_POWER_COLLAPSE] ||
		allow[MSM_PM_SLEEP_MODE_POWER_COLLAPSE_NO_XO_SHUTDOWN]) {
		uint32_t wait_us = CONFIG_MSM_IDLE_WAIT_ON_MODEM;
		while (msm_pm_modem_busy() && wait_us) {
			if (wait_us > 100) {
				udelay(100);
				wait_us -= 100;
			} else {
				udelay(wait_us);
				wait_us = 0;
			}
		}

		if (msm_pm_modem_busy()) {
			allow[MSM_PM_SLEEP_MODE_POWER_COLLAPSE] = false;
			allow[MSM_PM_SLEEP_MODE_POWER_COLLAPSE_NO_XO_SHUTDOWN]
				= false;
		}
	}

	MSM_PM_DPRINTK(MSM_PM_DEBUG_IDLE, KERN_INFO,
		"%s(): latency qos %d, next timer %lld, sleep limit %u\n",
		__func__, latency_qos, timer_expiration, sleep_limit);

	for (i = 0; i < ARRAY_SIZE(allow); i++)
		MSM_PM_DPRINTK(MSM_PM_DEBUG_IDLE, KERN_INFO,
			"%s(): allow %s: %d\n", __func__,
			msm_pm_sleep_mode_labels[i], (int)allow[i]);

	if (allow[MSM_PM_SLEEP_MODE_POWER_COLLAPSE] ||
		allow[MSM_PM_SLEEP_MODE_POWER_COLLAPSE_NO_XO_SHUTDOWN]) {
<<<<<<< HEAD
		uint32_t sleep_delay;

		sleep_delay = (uint32_t) msm_pm_convert_and_cap_time(
			timer_expiration, MSM_PM_SLEEP_TICK_LIMIT);
=======
		/* Sync the timer with SCLK, it is needed only for modem
		 * assissted pollapse case.
		 */
		int64_t next_timer_exp = msm_timer_enter_idle();
		uint32_t sleep_delay;
		bool low_power = false;

		sleep_delay = (uint32_t) msm_pm_convert_and_cap_time(
			next_timer_exp, MSM_PM_SLEEP_TICK_LIMIT);

>>>>>>> refs/remotes/origin/cm-10.0
		if (sleep_delay == 0) /* 0 would mean infinite time */
			sleep_delay = 1;

		if (!allow[MSM_PM_SLEEP_MODE_POWER_COLLAPSE])
			sleep_limit = SLEEP_LIMIT_NO_TCXO_SHUTDOWN;

#if defined(CONFIG_MSM_MEMORY_LOW_POWER_MODE_IDLE_ACTIVE)
		sleep_limit |= SLEEP_RESOURCE_MEMORY_BIT1;
#elif defined(CONFIG_MSM_MEMORY_LOW_POWER_MODE_IDLE_RETENTION)
		sleep_limit |= SLEEP_RESOURCE_MEMORY_BIT0;
#endif

		ret = msm_pm_power_collapse(true, sleep_delay, sleep_limit);
		low_power = (ret != -EBUSY && ret != -ETIMEDOUT);
<<<<<<< HEAD

#ifdef CONFIG_MSM_IDLE_STATS
=======
		msm_timer_exit_idle(low_power);

>>>>>>> refs/remotes/origin/cm-10.0
		if (ret)
			exit_stat = MSM_PM_STAT_IDLE_FAILED_POWER_COLLAPSE;
		else {
			exit_stat = MSM_PM_STAT_IDLE_POWER_COLLAPSE;
			msm_pm_sleep_limit = sleep_limit;
		}
<<<<<<< HEAD
#endif /* CONFIG_MSM_IDLE_STATS */
	} else if (allow[MSM_PM_SLEEP_MODE_APPS_SLEEP]) {
		uint32_t sleep_delay;

		sleep_delay = (uint32_t) msm_pm_convert_and_cap_time(
			timer_expiration, MSM_PM_SLEEP_TICK_LIMIT);
		if (sleep_delay == 0) /* 0 would mean infinite time */
			sleep_delay = 1;

		ret = msm_pm_apps_sleep(sleep_delay, sleep_limit);
		low_power = 0;

#ifdef CONFIG_MSM_IDLE_STATS
		if (ret)
			exit_stat = MSM_PM_STAT_IDLE_FAILED_SLEEP;
		else
			exit_stat = MSM_PM_STAT_IDLE_SLEEP;
#endif /* CONFIG_MSM_IDLE_STATS */
	} else if (allow[MSM_PM_SLEEP_MODE_POWER_COLLAPSE_STANDALONE]) {
		ret = msm_pm_power_collapse_standalone();
		low_power = 0;
#ifdef CONFIG_MSM_IDLE_STATS
		exit_stat = ret ?
			MSM_PM_STAT_IDLE_FAILED_STANDALONE_POWER_COLLAPSE :
			MSM_PM_STAT_IDLE_STANDALONE_POWER_COLLAPSE;
#endif /* CONFIG_MSM_IDLE_STATS */
	} else if (allow[MSM_PM_SLEEP_MODE_RAMP_DOWN_AND_WAIT_FOR_INTERRUPT]) {
		ret = msm_pm_swfi(true);
		if (ret)
			while (!msm_irq_pending())
				udelay(1);
		low_power = 0;
#ifdef CONFIG_MSM_IDLE_STATS
		exit_stat = ret ? MSM_PM_STAT_IDLE_SPIN : MSM_PM_STAT_IDLE_WFI;
#endif /* CONFIG_MSM_IDLE_STATS */
	} else if (allow[MSM_PM_SLEEP_MODE_WAIT_FOR_INTERRUPT]) {
		msm_pm_swfi(false);
		low_power = 0;
#ifdef CONFIG_MSM_IDLE_STATS
		exit_stat = MSM_PM_STAT_IDLE_WFI;
#endif /* CONFIG_MSM_IDLE_STATS */
	} else {
		while (!msm_irq_pending())
			udelay(1);
		low_power = 0;
#ifdef CONFIG_MSM_IDLE_STATS
		exit_stat = MSM_PM_STAT_IDLE_SPIN;
#endif /* CONFIG_MSM_IDLE_STATS */
	}

arch_idle_exit:
	msm_timer_exit_idle(low_power);

#ifdef CONFIG_MSM_IDLE_STATS
	t2 = ktime_to_ns(ktime_get());
	msm_pm_add_stat(exit_stat, t2 - t1);
#endif /* CONFIG_MSM_IDLE_STATS */
=======
	} else if (allow[MSM_PM_SLEEP_MODE_POWER_COLLAPSE_STANDALONE]) {
		ret = msm_pm_power_collapse_standalone(true);
		exit_stat = ret ?
			MSM_PM_STAT_IDLE_FAILED_STANDALONE_POWER_COLLAPSE :
			MSM_PM_STAT_IDLE_STANDALONE_POWER_COLLAPSE;
	} else if (allow[MSM_PM_SLEEP_MODE_RAMP_DOWN_AND_WAIT_FOR_INTERRUPT]) {
		ret = msm_pm_swfi(true);
		if (ret)
			while (!msm_pm_irq_extns->irq_pending())
				udelay(1);
		exit_stat = ret ? MSM_PM_STAT_IDLE_SPIN : MSM_PM_STAT_IDLE_WFI;
	} else if (allow[MSM_PM_SLEEP_MODE_WAIT_FOR_INTERRUPT]) {
		msm_pm_swfi(false);
		exit_stat = MSM_PM_STAT_IDLE_WFI;
	} else {
		while (!msm_pm_irq_extns->irq_pending())
			udelay(1);
		exit_stat = MSM_PM_STAT_IDLE_SPIN;
	}

	__get_cpu_var(t2) = ktime_to_ns(ktime_get());
	msm_pm_add_stat(exit_stat, __get_cpu_var(t2) - t1);
>>>>>>> refs/remotes/origin/cm-10.0
}

/*
 * Suspend the Apps processor.
 *
 * Return value:
<<<<<<< HEAD
=======
 *	-EPERM: Suspend happened by a not permitted core
>>>>>>> refs/remotes/origin/cm-10.0
 *      -EAGAIN: modem reset occurred or early exit from suspend
 *      -EBUSY: modem not ready for our suspend
 *      -EINVAL: invalid sleep mode
 *      -EIO: could not ramp Apps processor clock
 *      -ETIMEDOUT: timed out waiting for modem's handshake
 *      0: success
 */
static int msm_pm_enter(suspend_state_t state)
{
	bool allow[MSM_PM_SLEEP_MODE_NR];
	uint32_t sleep_limit = SLEEP_LIMIT_NONE;
<<<<<<< HEAD
	int ret;
	int i;

#ifdef CONFIG_MSM_IDLE_STATS
	int64_t period = 0;
	int64_t time = 0;

	time = msm_timer_get_sclk_time(&period);
#endif
=======
	int ret = -EPERM;
	int i;
	int64_t period = 0;
	int64_t time = 0;

	/* Must executed by CORE0 */
	if (smp_processor_id()) {
		__WARN();
		goto suspend_exit;
	}

	time = msm_pm_timer_enter_suspend(&period);
>>>>>>> refs/remotes/origin/cm-10.0

	MSM_PM_DPRINTK(MSM_PM_DEBUG_SUSPEND, KERN_INFO,
		"%s(): sleep limit %u\n", __func__, sleep_limit);

	for (i = 0; i < ARRAY_SIZE(allow); i++)
		allow[i] = true;

<<<<<<< HEAD
	switch (msm_pm_sleep_mode) {
	case MSM_PM_SLEEP_MODE_WAIT_FOR_INTERRUPT:
		allow[MSM_PM_SLEEP_MODE_RAMP_DOWN_AND_WAIT_FOR_INTERRUPT] =
			false;
		/* fall through */
	case MSM_PM_SLEEP_MODE_RAMP_DOWN_AND_WAIT_FOR_INTERRUPT:
		allow[MSM_PM_SLEEP_MODE_POWER_COLLAPSE_STANDALONE] = false;
		/* fall through */
	case MSM_PM_SLEEP_MODE_POWER_COLLAPSE_STANDALONE:
		allow[MSM_PM_SLEEP_MODE_APPS_SLEEP] = false;
		/* fall through */
	case MSM_PM_SLEEP_MODE_APPS_SLEEP:
		allow[MSM_PM_SLEEP_MODE_POWER_COLLAPSE_NO_XO_SHUTDOWN] = false;
		allow[MSM_PM_SLEEP_MODE_POWER_COLLAPSE] = false;
		/* fall through */
	case MSM_PM_SLEEP_MODE_POWER_COLLAPSE_SUSPEND:
	case MSM_PM_SLEEP_MODE_POWER_COLLAPSE:
		break;
	default:
		printk(KERN_ERR "suspend sleep mode is invalid: %d\n",
			msm_pm_sleep_mode);
		return -EINVAL;
	}

	for (i = 0; i < ARRAY_SIZE(allow); i++) {
		struct msm_pm_platform_data *mode = &msm_pm_modes[i];
=======
	for (i = 0; i < ARRAY_SIZE(allow); i++) {
		struct msm_pm_platform_data *mode;
		mode = &msm_pm_modes[MSM_PM_MODE(0, i)];
>>>>>>> refs/remotes/origin/cm-10.0
		if (!mode->suspend_supported || !mode->suspend_enabled)
			allow[i] = false;
	}

<<<<<<< HEAD
	ret = 0;

	if (allow[MSM_PM_SLEEP_MODE_POWER_COLLAPSE] ||
		allow[MSM_PM_SLEEP_MODE_POWER_COLLAPSE_NO_XO_SHUTDOWN]) {
#ifdef CONFIG_MSM_IDLE_STATS
		enum msm_pm_time_stats_id id;
		int64_t end_time;
#endif
=======
	if (allow[MSM_PM_SLEEP_MODE_POWER_COLLAPSE] ||
		allow[MSM_PM_SLEEP_MODE_POWER_COLLAPSE_NO_XO_SHUTDOWN]) {
		enum msm_pm_time_stats_id id;
>>>>>>> refs/remotes/origin/cm-10.0

		clock_debug_print_enabled();

#ifdef CONFIG_MSM_SLEEP_TIME_OVERRIDE
		if (msm_pm_sleep_time_override > 0) {
			int64_t ns;
			ns = NSEC_PER_SEC * (int64_t)msm_pm_sleep_time_override;
			msm_pm_set_max_sleep_time(ns);
			msm_pm_sleep_time_override = 0;
		}
#endif
		if (!allow[MSM_PM_SLEEP_MODE_POWER_COLLAPSE])
			sleep_limit = SLEEP_LIMIT_NO_TCXO_SHUTDOWN;

#if defined(CONFIG_MSM_MEMORY_LOW_POWER_MODE_SUSPEND_ACTIVE)
		sleep_limit |= SLEEP_RESOURCE_MEMORY_BIT1;
#elif defined(CONFIG_MSM_MEMORY_LOW_POWER_MODE_SUSPEND_RETENTION)
		sleep_limit |= SLEEP_RESOURCE_MEMORY_BIT0;
#elif defined(CONFIG_MSM_MEMORY_LOW_POWER_MODE_SUSPEND_DEEP_POWER_DOWN)
		if (get_msm_migrate_pages_status() != MEM_OFFLINE)
			sleep_limit |= SLEEP_RESOURCE_MEMORY_BIT0;
#endif

		for (i = 0; i < 30 && msm_pm_modem_busy(); i++)
			udelay(500);

		ret = msm_pm_power_collapse(
			false, msm_pm_max_sleep_time, sleep_limit);

<<<<<<< HEAD
#ifdef CONFIG_MSM_IDLE_STATS
=======
>>>>>>> refs/remotes/origin/cm-10.0
		if (ret)
			id = MSM_PM_STAT_FAILED_SUSPEND;
		else {
			id = MSM_PM_STAT_SUSPEND;
			msm_pm_sleep_limit = sleep_limit;
		}

<<<<<<< HEAD
		if (time != 0) {
			end_time = msm_timer_get_sclk_time(NULL);
			if (end_time != 0) {
				time = end_time - time;
				if (time < 0)
					time += period;
			} else
				time = 0;
		}

		msm_pm_add_stat(id, time);
#endif
	} else if (allow[MSM_PM_SLEEP_MODE_POWER_COLLAPSE_STANDALONE]) {
		ret = msm_pm_power_collapse_standalone();
	} else if (allow[MSM_PM_SLEEP_MODE_RAMP_DOWN_AND_WAIT_FOR_INTERRUPT]) {
		ret = msm_pm_swfi(true);
		if (ret)
			while (!msm_irq_pending())
=======
		time = msm_pm_timer_exit_suspend(time, period);
		msm_pm_add_stat(id, time);
	} else if (allow[MSM_PM_SLEEP_MODE_POWER_COLLAPSE_STANDALONE]) {
		ret = msm_pm_power_collapse_standalone(false);
	} else if (allow[MSM_PM_SLEEP_MODE_RAMP_DOWN_AND_WAIT_FOR_INTERRUPT]) {
		ret = msm_pm_swfi(true);
		if (ret)
			while (!msm_pm_irq_extns->irq_pending())
>>>>>>> refs/remotes/origin/cm-10.0
				udelay(1);
	} else if (allow[MSM_PM_SLEEP_MODE_WAIT_FOR_INTERRUPT]) {
		msm_pm_swfi(false);
	}

<<<<<<< HEAD
=======
suspend_exit:
>>>>>>> refs/remotes/origin/cm-10.0
	MSM_PM_DPRINTK(MSM_PM_DEBUG_SUSPEND, KERN_INFO,
		"%s(): return %d\n", __func__, ret);

	return ret;
}

static struct platform_suspend_ops msm_pm_ops = {
	.enter = msm_pm_enter,
	.valid = suspend_valid_only_mem,
};

<<<<<<< HEAD

/******************************************************************************
 * Restart Definitions
 *****************************************************************************/

static uint32_t restart_reason = 0x776655AA;

static void msm_pm_power_off(void)
{
	unsigned long flags;
	unsigned int power_off_reason;

	printk(KERN_INFO " %s\n", __func__);
	msm_rpcrouter_close();

#if defined(CONFIG_SEC_DEBUG) && defined(CONFIG_SEC_MISC)
	sec_get_param(param_power_off_reason, &power_off_reason);
	power_off_reason = power_off_reason | 0x40;
	sec_set_param(param_power_off_reason, &power_off_reason);
#endif
	printk(KERN_INFO " %s before proc_comm\n", __func__);
	msm_proc_comm(PCOM_POWER_DOWN, 0, 0);

	power_off_done = 1;

	spin_lock_irqsave(&pm_lock, flags);
	for (;;)
		;
	spin_unlock_irqrestore(&pm_lock, flags);
}

static void msm_pm_restart(char str, const char *cmd)
{
	unsigned size;
	unsigned long flags;
	unsigned int power_off_reason;
	samsung_vendor1_id *smem_vendor1 = \
		(samsung_vendor1_id *)smem_get_entry(SMEM_ID_VENDOR1, &size);

	if (smem_vendor1) {
		smem_vendor1->silent_reset = 0xAEAEAEAE;
		smem_vendor1->reboot_reason = restart_reason;
	} else {
		printk(KERN_EMERG "smem_flag is NULL\n");
	}

#if !defined(CONFIG_MACH_JENA)
	msm_rpcrouter_close();
#endif

#if defined(CONFIG_SEC_DEBUG) && defined(CONFIG_SEC_MISC)
	sec_get_param(param_power_off_reason, &power_off_reason);
	power_off_reason = power_off_reason | 0x10;
	sec_set_param(param_power_off_reason, &power_off_reason);
#endif

#if defined(CONFIG_MACH_JENA)
	msm_rpcrouter_close();
#endif

	msm_proc_comm(PCOM_RESET_CHIP_IMM, &restart_reason, 0);

	power_off_done = 1;

	spin_lock_irqsave(&pm_lock, flags);
	for (;;)
		;
	spin_unlock_irqrestore(&pm_lock, flags);
}

static int msm_reboot_call
	(struct notifier_block *this, unsigned long code, void *_cmd)
{
	unsigned int debug_level;

	if ((code == SYS_RESTART) && _cmd) {
		char *cmd = _cmd;
		if (!strcmp(cmd, "bootloader")) {
			restart_reason = 0x77665500;
		} else if (!strcmp(cmd, "recovery")) {
			restart_reason = 0x77665502;
		} else if (!strcmp(cmd, "recovery_done")) {
#if defined(CONFIG_SEC_MISC)
			sec_get_param(param_index_debuglevel, &debug_level);
			debug_level = 0;
			sec_set_param(param_index_debuglevel, &debug_level);
#endif
			restart_reason = 0x77665503;			
		} else if (!strcmp(cmd, "eraseflash")) {
			restart_reason = 0x776655EF;
		} else if (!strcmp(cmd, "download")) {
			restart_reason = 0x776655FF;	
		} else if (!strncmp(cmd, "oem-", 4)) {
			unsigned code = simple_strtoul(cmd + 4, 0, 16) & 0xff;
			restart_reason = 0x6f656d00 | code;
		} else if (!strncmp(cmd, "arm11_fota", 10)) {
			restart_reason = 0x77665504;
		} else {
			restart_reason = 0x77665501;
		}
	}
	return NOTIFY_DONE;
}

static struct notifier_block msm_reboot_notifier = {
	.notifier_call = msm_reboot_call,
};


/******************************************************************************
 *
 *****************************************************************************/

=======
/* Hotplug the "non boot" CPU's and put
 * the cores into low power mode
 */
void msm_pm_cpu_enter_lowpower(unsigned int cpu)
{
	bool allow[MSM_PM_SLEEP_MODE_NR];
	int i;

	for (i = 0; i < MSM_PM_SLEEP_MODE_NR; i++) {
		struct msm_pm_platform_data *mode;

		mode = &msm_pm_modes[MSM_PM_MODE(cpu, i)];
		allow[i] = mode->suspend_supported && mode->suspend_enabled;
	}

	MSM_PM_DPRINTK(MSM_PM_DEBUG_HOTPLUG, KERN_INFO,
		"CPU%u: %s: shutting down cpu\n", cpu, __func__);

	if (allow[MSM_PM_SLEEP_MODE_POWER_COLLAPSE_STANDALONE]) {
		msm_pm_power_collapse_standalone(false);
	} else if (allow[MSM_PM_SLEEP_MODE_WAIT_FOR_INTERRUPT]) {
		msm_pm_swfi(false);
	} else {
		MSM_PM_DPRINTK(MSM_PM_DEBUG_HOTPLUG, KERN_INFO,
			"CPU%u: %s: shutting down failed!!!\n", cpu, __func__);
	}
}

>>>>>>> refs/remotes/origin/cm-10.0
/*
 * Initialize the power management subsystem.
 *
 * Return value:
 *      -ENODEV: initialization failed
 *      0: success
 */
static int __init msm_pm_init(void)
{
<<<<<<< HEAD
#ifdef CONFIG_MSM_IDLE_STATS
	struct proc_dir_entry *d_entry;
#endif
	int ret;
=======
	int ret;
	int val;
	enum msm_pm_time_stats_id enable_stats[] = {
		MSM_PM_STAT_REQUESTED_IDLE,
		MSM_PM_STAT_IDLE_SPIN,
		MSM_PM_STAT_IDLE_WFI,
		MSM_PM_STAT_IDLE_STANDALONE_POWER_COLLAPSE,
		MSM_PM_STAT_IDLE_FAILED_STANDALONE_POWER_COLLAPSE,
		MSM_PM_STAT_IDLE_POWER_COLLAPSE,
		MSM_PM_STAT_IDLE_FAILED_POWER_COLLAPSE,
		MSM_PM_STAT_SUSPEND,
		MSM_PM_STAT_FAILED_SUSPEND,
		MSM_PM_STAT_NOT_IDLE,
	};

>>>>>>> refs/remotes/origin/cm-10.0
#ifdef CONFIG_CPU_V7
	pgd_t *pc_pgd;
	pmd_t *pmd;
	unsigned long pmdval;
<<<<<<< HEAD
=======
	unsigned long exit_phys;

	exit_phys = virt_to_phys(msm_pm_collapse_exit);
>>>>>>> refs/remotes/origin/cm-10.0

	/* Page table for cores to come back up safely. */
	pc_pgd = pgd_alloc(&init_mm);
	if (!pc_pgd)
		return -ENOMEM;
<<<<<<< HEAD
	pmd = pmd_offset(pc_pgd +
			 pgd_index(virt_to_phys(msm_pm_collapse_exit)),
			 virt_to_phys(msm_pm_collapse_exit));
	pmdval = (virt_to_phys(msm_pm_collapse_exit) & PGDIR_MASK) |
=======
	pmd = pmd_offset(pud_offset(pc_pgd + pgd_index(exit_phys), exit_phys),
			 exit_phys);
	pmdval = (exit_phys & PGDIR_MASK) |
>>>>>>> refs/remotes/origin/cm-10.0
		     PMD_TYPE_SECT | PMD_SECT_AP_WRITE;
	pmd[0] = __pmd(pmdval);
	pmd[1] = __pmd(pmdval + (1 << (PGDIR_SHIFT - 1)));

<<<<<<< HEAD
=======
	msm_saved_state_phys =
		allocate_contiguous_ebi_nomap(CPU_SAVED_STATE_SIZE *
					      num_possible_cpus(), 4);
	if (!msm_saved_state_phys)
		return -ENOMEM;
	msm_saved_state = ioremap_nocache(msm_saved_state_phys,
					  CPU_SAVED_STATE_SIZE *
					  num_possible_cpus());
	if (!msm_saved_state)
		return -ENOMEM;

>>>>>>> refs/remotes/origin/cm-10.0
	/* It is remotely possible that the code in msm_pm_collapse_exit()
	 * which turns on the MMU with this mapping is in the
	 * next even-numbered megabyte beyond the
	 * start of msm_pm_collapse_exit().
	 * Map this megabyte in as well.
	 */
	pmd[2] = __pmd(pmdval + (2 << (PGDIR_SHIFT - 1)));
	flush_pmd_entry(pmd);
	msm_pm_pc_pgd = virt_to_phys(pc_pgd);
<<<<<<< HEAD
#endif

	pm_power_off = msm_pm_power_off;
	arm_pm_restart = msm_pm_restart;
	register_reboot_notifier(&msm_reboot_notifier);

=======
	clean_caches((unsigned long)&msm_pm_pc_pgd, sizeof(msm_pm_pc_pgd),
		     virt_to_phys(&msm_pm_pc_pgd));
#endif

>>>>>>> refs/remotes/origin/cm-10.0
	msm_pm_smem_data = smem_alloc(SMEM_APPS_DEM_SLAVE_DATA,
		sizeof(*msm_pm_smem_data));
	if (msm_pm_smem_data == NULL) {
		printk(KERN_ERR "%s: failed to get smsm_data\n", __func__);
		return -ENODEV;
	}

	ret = msm_timer_init_time_sync(msm_pm_timeout);
	if (ret)
		return ret;

	ret = smsm_change_intr_mask(SMSM_POWER_MASTER_DEM, 0xFFFFFFFF, 0);
	if (ret) {
		printk(KERN_ERR "%s: failed to clear interrupt mask, %d\n",
			__func__, ret);
		return ret;
	}

<<<<<<< HEAD
=======
	if (cpu_is_msm8625() || cpu_is_msm8625q()) {
		target_type = TARGET_IS_8625;
		clean_caches((unsigned long)&target_type, sizeof(target_type),
				virt_to_phys(&target_type));

		/*
		 * Configure the MPA5_GDFS_CNT_VAL register for
		 * DBGPWRUPEREQ_OVERRIDE[19:16] = Override the
		 * DBGNOPOWERDN for each cpu.
		 * MPA5_GDFS_CNT_VAL[9:0] = Delay counter for
		 * GDFS control.
		 */
		if (cpu_is_msm8625q())
			val = 0x000F0002;
		else
			val = 0x00030002;

		__raw_writel(val, (MSM_CFG_CTL_BASE + 0x38));

		l2x0_base_addr = MSM_L2CC_BASE;
	}

>>>>>>> refs/remotes/origin/cm-10.0
#ifdef CONFIG_MSM_MEMORY_LOW_POWER_MODE
	/* The wakeup_reason field is overloaded during initialization time
	   to signal Modem that Apps will control the low power modes of
	   the memory.
	 */
	msm_pm_smem_data->wakeup_reason = 1;
	smsm_change_state(SMSM_APPS_DEM, 0, DEM_SLAVE_SMSM_RUN);
#endif

	BUG_ON(msm_pm_modes == NULL);

<<<<<<< HEAD
	atomic_set(&msm_pm_init_done, 1);
	suspend_set_ops(&msm_pm_ops);

	msm_pm_mode_sysfs_add();
#ifdef CONFIG_MSM_IDLE_STATS
	d_entry = create_proc_entry("msm_pm_stats",
			S_IRUGO | S_IWUSR | S_IWGRP, NULL);
	if (d_entry) {
		d_entry->read_proc = msm_pm_read_proc;
		d_entry->write_proc = msm_pm_write_proc;
		d_entry->data = NULL;
	}
#endif

=======
	suspend_set_ops(&msm_pm_ops);

	msm_pm_mode_sysfs_add();
	msm_pm_add_stats(enable_stats, ARRAY_SIZE(enable_stats));

	atomic_set(&msm_pm_init_done, 1);
>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
}

late_initcall_sync(msm_pm_init);
