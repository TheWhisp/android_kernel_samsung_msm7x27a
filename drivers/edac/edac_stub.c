/*
 * common EDAC components that must be in kernel
 *
 * Author: Dave Jiang <djiang@mvista.com>
 *
 * 2007 (c) MontaVista Software, Inc.
 * 2010 (c) Advanced Micro Devices Inc.
<<<<<<< HEAD
 *	    Borislav Petkov <borislav.petkov@amd.com>
=======
 *	    Borislav Petkov <bp@alien8.de>
>>>>>>> refs/remotes/origin/master
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 *
 */
#include <linux/module.h>
#include <linux/edac.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/atomic.h>
=======
#include <linux/atomic.h>
#include <linux/device.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/atomic.h>
#include <linux/device.h>
>>>>>>> refs/remotes/origin/master
#include <asm/edac.h>

int edac_op_state = EDAC_OPSTATE_INVAL;
EXPORT_SYMBOL_GPL(edac_op_state);

atomic_t edac_handlers = ATOMIC_INIT(0);
EXPORT_SYMBOL_GPL(edac_handlers);

int edac_err_assert = 0;
EXPORT_SYMBOL_GPL(edac_err_assert);

<<<<<<< HEAD
<<<<<<< HEAD
static atomic_t edac_class_valid = ATOMIC_INIT(0);
=======
static atomic_t edac_subsys_valid = ATOMIC_INIT(0);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static atomic_t edac_subsys_valid = ATOMIC_INIT(0);

int edac_report_status = EDAC_REPORTING_ENABLED;
EXPORT_SYMBOL_GPL(edac_report_status);

static int __init edac_report_setup(char *str)
{
	if (!str)
		return -EINVAL;

	if (!strncmp(str, "on", 2))
		set_edac_report_status(EDAC_REPORTING_ENABLED);
	else if (!strncmp(str, "off", 3))
		set_edac_report_status(EDAC_REPORTING_DISABLED);
	else if (!strncmp(str, "force", 5))
		set_edac_report_status(EDAC_REPORTING_FORCE);

	return 0;
}
__setup("edac_report=", edac_report_setup);
>>>>>>> refs/remotes/origin/master

/*
 * called to determine if there is an EDAC driver interested in
 * knowing an event (such as NMI) occurred
 */
int edac_handler_set(void)
{
	if (edac_op_state == EDAC_OPSTATE_POLL)
		return 0;

	return atomic_read(&edac_handlers);
}
EXPORT_SYMBOL_GPL(edac_handler_set);

/*
 * handler for NMI type of interrupts to assert error
 */
void edac_atomic_assert_error(void)
{
	edac_err_assert++;
}
EXPORT_SYMBOL_GPL(edac_atomic_assert_error);

/*
 * sysfs object: /sys/devices/system/edac
 *	need to export to other files
 */
<<<<<<< HEAD
<<<<<<< HEAD
struct sysdev_class edac_class = {
	.name = "edac",
};
EXPORT_SYMBOL_GPL(edac_class);

/* return pointer to the 'edac' node in sysfs */
struct sysdev_class *edac_get_sysfs_class(void)
{
	int err = 0;

	if (atomic_read(&edac_class_valid))
		goto out;

	/* create the /sys/devices/system/edac directory */
	err = sysdev_class_register(&edac_class);
=======
=======
>>>>>>> refs/remotes/origin/master
struct bus_type edac_subsys = {
	.name = "edac",
	.dev_name = "edac",
};
EXPORT_SYMBOL_GPL(edac_subsys);

/* return pointer to the 'edac' node in sysfs */
struct bus_type *edac_get_sysfs_subsys(void)
{
	int err = 0;

	if (atomic_read(&edac_subsys_valid))
		goto out;

	/* create the /sys/devices/system/edac directory */
	err = subsys_system_register(&edac_subsys, NULL);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (err) {
		printk(KERN_ERR "Error registering toplevel EDAC sysfs dir\n");
		return NULL;
	}

out:
<<<<<<< HEAD
<<<<<<< HEAD
	atomic_inc(&edac_class_valid);
	return &edac_class;
}
EXPORT_SYMBOL_GPL(edac_get_sysfs_class);

void edac_put_sysfs_class(void)
{
	/* last user unregisters it */
	if (atomic_dec_and_test(&edac_class_valid))
		sysdev_class_unregister(&edac_class);
}
EXPORT_SYMBOL_GPL(edac_put_sysfs_class);
=======
=======
>>>>>>> refs/remotes/origin/master
	atomic_inc(&edac_subsys_valid);
	return &edac_subsys;
}
EXPORT_SYMBOL_GPL(edac_get_sysfs_subsys);

void edac_put_sysfs_subsys(void)
{
	/* last user unregisters it */
	if (atomic_dec_and_test(&edac_subsys_valid))
		bus_unregister(&edac_subsys);
}
EXPORT_SYMBOL_GPL(edac_put_sysfs_subsys);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
