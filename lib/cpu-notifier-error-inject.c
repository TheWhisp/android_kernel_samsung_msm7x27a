#include <linux/kernel.h>
<<<<<<< HEAD
#include <linux/cpu.h>
#include <linux/module.h>
#include <linux/notifier.h>

static int priority;
static int cpu_up_prepare_error;
static int cpu_down_prepare_error;

module_param(priority, int, 0);
MODULE_PARM_DESC(priority, "specify cpu notifier priority");

module_param(cpu_up_prepare_error, int, 0644);
MODULE_PARM_DESC(cpu_up_prepare_error,
		"specify error code to inject CPU_UP_PREPARE action");

module_param(cpu_down_prepare_error, int, 0644);
MODULE_PARM_DESC(cpu_down_prepare_error,
		"specify error code to inject CPU_DOWN_PREPARE action");

static int err_inject_cpu_callback(struct notifier_block *nfb,
				unsigned long action, void *hcpu)
{
	int err = 0;

	switch (action) {
	case CPU_UP_PREPARE:
	case CPU_UP_PREPARE_FROZEN:
		err = cpu_up_prepare_error;
		break;
	case CPU_DOWN_PREPARE:
	case CPU_DOWN_PREPARE_FROZEN:
		err = cpu_down_prepare_error;
		break;
	}
	if (err)
		printk(KERN_INFO "Injecting error (%d) at cpu notifier\n", err);

	return notifier_from_errno(err);
}

static struct notifier_block err_inject_cpu_notifier = {
	.notifier_call = err_inject_cpu_callback,
};

static int err_inject_init(void)
{
	err_inject_cpu_notifier.priority = priority;

	return register_hotcpu_notifier(&err_inject_cpu_notifier);
=======
#include <linux/module.h>
#include <linux/cpu.h>

#include "notifier-error-inject.h"

static int priority;
module_param(priority, int, 0);
MODULE_PARM_DESC(priority, "specify cpu notifier priority");

static struct notifier_err_inject cpu_notifier_err_inject = {
	.actions = {
		{ NOTIFIER_ERR_INJECT_ACTION(CPU_UP_PREPARE) },
		{ NOTIFIER_ERR_INJECT_ACTION(CPU_UP_PREPARE_FROZEN) },
		{ NOTIFIER_ERR_INJECT_ACTION(CPU_DOWN_PREPARE) },
		{ NOTIFIER_ERR_INJECT_ACTION(CPU_DOWN_PREPARE_FROZEN) },
		{}
	}
};

static struct dentry *dir;

static int err_inject_init(void)
{
	int err;

	dir = notifier_err_inject_init("cpu", notifier_err_inject_dir,
					&cpu_notifier_err_inject, priority);
	if (IS_ERR(dir))
		return PTR_ERR(dir);

	err = register_hotcpu_notifier(&cpu_notifier_err_inject.nb);
	if (err)
		debugfs_remove_recursive(dir);

	return err;
>>>>>>> refs/remotes/origin/master
}

static void err_inject_exit(void)
{
<<<<<<< HEAD
	unregister_hotcpu_notifier(&err_inject_cpu_notifier);
=======
	unregister_hotcpu_notifier(&cpu_notifier_err_inject.nb);
	debugfs_remove_recursive(dir);
>>>>>>> refs/remotes/origin/master
}

module_init(err_inject_init);
module_exit(err_inject_exit);

MODULE_DESCRIPTION("CPU notifier error injection module");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Akinobu Mita <akinobu.mita@gmail.com>");
