/*
<<<<<<< HEAD
<<<<<<< HEAD
 * drivers/base/cpu.c - basic CPU class support
 */

#include <linux/sysdev.h>
=======
=======
>>>>>>> refs/remotes/origin/master
 * CPU subsystem support
 */

#include <linux/kernel.h>
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/cpu.h>
#include <linux/topology.h>
#include <linux/device.h>
#include <linux/node.h>
#include <linux/gfp.h>
<<<<<<< HEAD
<<<<<<< HEAD

#include "base.h"

static struct sysdev_class_attribute *cpu_sysdev_class_attrs[];

struct sysdev_class cpu_sysdev_class = {
	.name = "cpu",
	.attrs = cpu_sysdev_class_attrs,
};
EXPORT_SYMBOL(cpu_sysdev_class);

static DEFINE_PER_CPU(struct sys_device *, cpu_sys_devices);

#ifdef CONFIG_HOTPLUG_CPU
static ssize_t show_online(struct sys_device *dev, struct sysdev_attribute *attr,
			   char *buf)
{
	struct cpu *cpu = container_of(dev, struct cpu, sysdev);

	return sprintf(buf, "%u\n", !!cpu_online(cpu->sysdev.id));
}

static ssize_t __ref store_online(struct sys_device *dev, struct sysdev_attribute *attr,
				 const char *buf, size_t count)
{
	struct cpu *cpu = container_of(dev, struct cpu, sysdev);
=======
#include <linux/slab.h>
#include <linux/percpu.h>

#include "base.h"

struct bus_type cpu_subsys = {
	.name = "cpu",
	.dev_name = "cpu",
};
EXPORT_SYMBOL_GPL(cpu_subsys);

static DEFINE_PER_CPU(struct device *, cpu_sys_devices);

#ifdef CONFIG_HOTPLUG_CPU
static ssize_t show_online(struct device *dev,
			   struct device_attribute *attr,
			   char *buf)
{
	struct cpu *cpu = container_of(dev, struct cpu, dev);

	return sprintf(buf, "%u\n", !!cpu_online(cpu->dev.id));
}

static ssize_t __ref store_online(struct device *dev,
				  struct device_attribute *attr,
				  const char *buf, size_t count)
{
	struct cpu *cpu = container_of(dev, struct cpu, dev);
>>>>>>> refs/remotes/origin/cm-10.0
	ssize_t ret;

	cpu_hotplug_driver_lock();
	switch (buf[0]) {
	case '0':
<<<<<<< HEAD
		ret = cpu_down(cpu->sysdev.id);
=======
		ret = cpu_down(cpu->dev.id);
>>>>>>> refs/remotes/origin/cm-10.0
		if (!ret)
			kobject_uevent(&dev->kobj, KOBJ_OFFLINE);
		break;
	case '1':
<<<<<<< HEAD
		ret = cpu_up(cpu->sysdev.id);
=======
		ret = cpu_up(cpu->dev.id);
>>>>>>> refs/remotes/origin/cm-10.0
		if (!ret)
			kobject_uevent(&dev->kobj, KOBJ_ONLINE);
		break;
	default:
		ret = -EINVAL;
	}
	cpu_hotplug_driver_unlock();

	if (ret >= 0)
		ret = count;
	return ret;
}
<<<<<<< HEAD
static SYSDEV_ATTR(online, 0644, show_online, store_online);

static void __cpuinit register_cpu_control(struct cpu *cpu)
{
	sysdev_create_file(&cpu->sysdev, &attr_online);
}
void unregister_cpu(struct cpu *cpu)
{
	int logical_cpu = cpu->sysdev.id;

	unregister_cpu_under_node(logical_cpu, cpu_to_node(logical_cpu));

	sysdev_remove_file(&cpu->sysdev, &attr_online);

	sysdev_unregister(&cpu->sysdev);
=======
static DEVICE_ATTR(online, 0644, show_online, store_online);

static void __cpuinit register_cpu_control(struct cpu *cpu)
{
	device_create_file(&cpu->dev, &dev_attr_online);
}
=======
#include <linux/slab.h>
#include <linux/percpu.h>
#include <linux/acpi.h>
#include <linux/of.h>

#include "base.h"

static DEFINE_PER_CPU(struct device *, cpu_sys_devices);

static int cpu_subsys_match(struct device *dev, struct device_driver *drv)
{
	/* ACPI style match is the only one that may succeed. */
	if (acpi_driver_match_device(dev, drv))
		return 1;

	return 0;
}

#ifdef CONFIG_HOTPLUG_CPU
static void change_cpu_under_node(struct cpu *cpu,
			unsigned int from_nid, unsigned int to_nid)
{
	int cpuid = cpu->dev.id;
	unregister_cpu_under_node(cpuid, from_nid);
	register_cpu_under_node(cpuid, to_nid);
	cpu->node_id = to_nid;
}

static int __ref cpu_subsys_online(struct device *dev)
{
	struct cpu *cpu = container_of(dev, struct cpu, dev);
	int cpuid = dev->id;
	int from_nid, to_nid;
	int ret;

	from_nid = cpu_to_node(cpuid);
	if (from_nid == NUMA_NO_NODE)
		return -ENODEV;

	ret = cpu_up(cpuid);
	/*
	 * When hot adding memory to memoryless node and enabling a cpu
	 * on the node, node number of the cpu may internally change.
	 */
	to_nid = cpu_to_node(cpuid);
	if (from_nid != to_nid)
		change_cpu_under_node(cpu, from_nid, to_nid);

	return ret;
}

static int cpu_subsys_offline(struct device *dev)
{
	return cpu_down(dev->id);
}

>>>>>>> refs/remotes/origin/master
void unregister_cpu(struct cpu *cpu)
{
	int logical_cpu = cpu->dev.id;

	unregister_cpu_under_node(logical_cpu, cpu_to_node(logical_cpu));

<<<<<<< HEAD
	device_remove_file(&cpu->dev, &dev_attr_online);

	device_unregister(&cpu->dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	device_unregister(&cpu->dev);
>>>>>>> refs/remotes/origin/master
	per_cpu(cpu_sys_devices, logical_cpu) = NULL;
	return;
}

#ifdef CONFIG_ARCH_CPU_PROBE_RELEASE
<<<<<<< HEAD
<<<<<<< HEAD
static ssize_t cpu_probe_store(struct sysdev_class *class,
			       struct sysdev_class_attribute *attr,
=======
static ssize_t cpu_probe_store(struct device *dev,
			       struct device_attribute *attr,
>>>>>>> refs/remotes/origin/cm-10.0
			       const char *buf,
			       size_t count)
{
	return arch_cpu_probe(buf, count);
}

<<<<<<< HEAD
static ssize_t cpu_release_store(struct sysdev_class *class,
				 struct sysdev_class_attribute *attr,
=======
static ssize_t cpu_release_store(struct device *dev,
				 struct device_attribute *attr,
>>>>>>> refs/remotes/origin/cm-10.0
				 const char *buf,
				 size_t count)
{
	return arch_cpu_release(buf, count);
}

<<<<<<< HEAD
static SYSDEV_CLASS_ATTR(probe, S_IWUSR, NULL, cpu_probe_store);
static SYSDEV_CLASS_ATTR(release, S_IWUSR, NULL, cpu_release_store);
=======
static DEVICE_ATTR(probe, S_IWUSR, NULL, cpu_probe_store);
static DEVICE_ATTR(release, S_IWUSR, NULL, cpu_release_store);
>>>>>>> refs/remotes/origin/cm-10.0
#endif /* CONFIG_ARCH_CPU_PROBE_RELEASE */

#else /* ... !CONFIG_HOTPLUG_CPU */
static inline void register_cpu_control(struct cpu *cpu)
{
}
#endif /* CONFIG_HOTPLUG_CPU */

#ifdef CONFIG_KEXEC
#include <linux/kexec.h>

<<<<<<< HEAD
static ssize_t show_crash_notes(struct sys_device *dev, struct sysdev_attribute *attr,
				char *buf)
{
	struct cpu *cpu = container_of(dev, struct cpu, sysdev);
=======
=======
static ssize_t cpu_probe_store(struct device *dev,
			       struct device_attribute *attr,
			       const char *buf,
			       size_t count)
{
	ssize_t cnt;
	int ret;

	ret = lock_device_hotplug_sysfs();
	if (ret)
		return ret;

	cnt = arch_cpu_probe(buf, count);

	unlock_device_hotplug();
	return cnt;
}

static ssize_t cpu_release_store(struct device *dev,
				 struct device_attribute *attr,
				 const char *buf,
				 size_t count)
{
	ssize_t cnt;
	int ret;

	ret = lock_device_hotplug_sysfs();
	if (ret)
		return ret;

	cnt = arch_cpu_release(buf, count);

	unlock_device_hotplug();
	return cnt;
}

static DEVICE_ATTR(probe, S_IWUSR, NULL, cpu_probe_store);
static DEVICE_ATTR(release, S_IWUSR, NULL, cpu_release_store);
#endif /* CONFIG_ARCH_CPU_PROBE_RELEASE */
#endif /* CONFIG_HOTPLUG_CPU */

struct bus_type cpu_subsys = {
	.name = "cpu",
	.dev_name = "cpu",
	.match = cpu_subsys_match,
#ifdef CONFIG_HOTPLUG_CPU
	.online = cpu_subsys_online,
	.offline = cpu_subsys_offline,
#endif
};
EXPORT_SYMBOL_GPL(cpu_subsys);

#ifdef CONFIG_KEXEC
#include <linux/kexec.h>

>>>>>>> refs/remotes/origin/master
static ssize_t show_crash_notes(struct device *dev, struct device_attribute *attr,
				char *buf)
{
	struct cpu *cpu = container_of(dev, struct cpu, dev);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	ssize_t rc;
	unsigned long long addr;
	int cpunum;

<<<<<<< HEAD
<<<<<<< HEAD
	cpunum = cpu->sysdev.id;
=======
	cpunum = cpu->dev.id;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	cpunum = cpu->dev.id;
>>>>>>> refs/remotes/origin/master

	/*
	 * Might be reading other cpu's data based on which cpu read thread
	 * has been scheduled. But cpu data (memory) is allocated once during
	 * boot up and this data does not change there after. Hence this
	 * operation should be safe. No locking required.
	 */
	addr = per_cpu_ptr_to_phys(per_cpu_ptr(crash_notes, cpunum));
	rc = sprintf(buf, "%Lx\n", addr);
	return rc;
}
<<<<<<< HEAD
<<<<<<< HEAD
static SYSDEV_ATTR(crash_notes, 0400, show_crash_notes, NULL);
=======
static DEVICE_ATTR(crash_notes, 0400, show_crash_notes, NULL);
>>>>>>> refs/remotes/origin/cm-10.0
#endif
=======
static DEVICE_ATTR(crash_notes, 0400, show_crash_notes, NULL);

static ssize_t show_crash_notes_size(struct device *dev,
				     struct device_attribute *attr,
				     char *buf)
{
	ssize_t rc;

	rc = sprintf(buf, "%zu\n", sizeof(note_buf_t));
	return rc;
}
static DEVICE_ATTR(crash_notes_size, 0400, show_crash_notes_size, NULL);

static struct attribute *crash_note_cpu_attrs[] = {
	&dev_attr_crash_notes.attr,
	&dev_attr_crash_notes_size.attr,
	NULL
};

static struct attribute_group crash_note_cpu_attr_group = {
	.attrs = crash_note_cpu_attrs,
};
#endif

static const struct attribute_group *common_cpu_attr_groups[] = {
#ifdef CONFIG_KEXEC
	&crash_note_cpu_attr_group,
#endif
	NULL
};

static const struct attribute_group *hotplugable_cpu_attr_groups[] = {
#ifdef CONFIG_KEXEC
	&crash_note_cpu_attr_group,
#endif
	NULL
};
>>>>>>> refs/remotes/origin/master

/*
 * Print cpu online, possible, present, and system maps
 */

struct cpu_attr {
<<<<<<< HEAD
<<<<<<< HEAD
	struct sysdev_class_attribute attr;
	const struct cpumask *const * const map;
};

static ssize_t show_cpus_attr(struct sysdev_class *class,
			      struct sysdev_class_attribute *attr,
=======
=======
>>>>>>> refs/remotes/origin/master
	struct device_attribute attr;
	const struct cpumask *const * const map;
};

static ssize_t show_cpus_attr(struct device *dev,
			      struct device_attribute *attr,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			      char *buf)
{
	struct cpu_attr *ca = container_of(attr, struct cpu_attr, attr);
	int n = cpulist_scnprintf(buf, PAGE_SIZE-2, *(ca->map));

	buf[n++] = '\n';
	buf[n] = '\0';
	return n;
}

<<<<<<< HEAD
<<<<<<< HEAD
#define _CPU_ATTR(name, map)						\
	{ _SYSDEV_CLASS_ATTR(name, 0444, show_cpus_attr, NULL), map }

/* Keep in sync with cpu_sysdev_class_attrs */
=======
=======
>>>>>>> refs/remotes/origin/master
#define _CPU_ATTR(name, map) \
	{ __ATTR(name, 0444, show_cpus_attr, NULL), map }

/* Keep in sync with cpu_subsys_attrs */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static struct cpu_attr cpu_attrs[] = {
	_CPU_ATTR(online, &cpu_online_mask),
	_CPU_ATTR(possible, &cpu_possible_mask),
	_CPU_ATTR(present, &cpu_present_mask),
};

/*
 * Print values for NR_CPUS and offlined cpus
 */
<<<<<<< HEAD
<<<<<<< HEAD
static ssize_t print_cpus_kernel_max(struct sysdev_class *class,
				     struct sysdev_class_attribute *attr, char *buf)
=======
static ssize_t print_cpus_kernel_max(struct device *dev,
				     struct device_attribute *attr, char *buf)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static ssize_t print_cpus_kernel_max(struct device *dev,
				     struct device_attribute *attr, char *buf)
>>>>>>> refs/remotes/origin/master
{
	int n = snprintf(buf, PAGE_SIZE-2, "%d\n", NR_CPUS - 1);
	return n;
}
<<<<<<< HEAD
<<<<<<< HEAD
static SYSDEV_CLASS_ATTR(kernel_max, 0444, print_cpus_kernel_max, NULL);
=======
static DEVICE_ATTR(kernel_max, 0444, print_cpus_kernel_max, NULL);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static DEVICE_ATTR(kernel_max, 0444, print_cpus_kernel_max, NULL);
>>>>>>> refs/remotes/origin/master

/* arch-optional setting to enable display of offline cpus >= nr_cpu_ids */
unsigned int total_cpus;

<<<<<<< HEAD
<<<<<<< HEAD
static ssize_t print_cpus_offline(struct sysdev_class *class,
				  struct sysdev_class_attribute *attr, char *buf)
=======
static ssize_t print_cpus_offline(struct device *dev,
				  struct device_attribute *attr, char *buf)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static ssize_t print_cpus_offline(struct device *dev,
				  struct device_attribute *attr, char *buf)
>>>>>>> refs/remotes/origin/master
{
	int n = 0, len = PAGE_SIZE-2;
	cpumask_var_t offline;

	/* display offline cpus < nr_cpu_ids */
	if (!alloc_cpumask_var(&offline, GFP_KERNEL))
		return -ENOMEM;
	cpumask_andnot(offline, cpu_possible_mask, cpu_online_mask);
	n = cpulist_scnprintf(buf, len, offline);
	free_cpumask_var(offline);

	/* display offline cpus >= nr_cpu_ids */
	if (total_cpus && nr_cpu_ids < total_cpus) {
		if (n && n < len)
			buf[n++] = ',';

		if (nr_cpu_ids == total_cpus-1)
			n += snprintf(&buf[n], len - n, "%d", nr_cpu_ids);
		else
			n += snprintf(&buf[n], len - n, "%d-%d",
						      nr_cpu_ids, total_cpus-1);
	}

	n += snprintf(&buf[n], len - n, "\n");
	return n;
}
<<<<<<< HEAD
<<<<<<< HEAD
static SYSDEV_CLASS_ATTR(offline, 0444, print_cpus_offline, NULL);
=======
=======
>>>>>>> refs/remotes/origin/master
static DEVICE_ATTR(offline, 0444, print_cpus_offline, NULL);

static void cpu_device_release(struct device *dev)
{
	/*
	 * This is an empty function to prevent the driver core from spitting a
	 * warning at us.  Yes, I know this is directly opposite of what the
	 * documentation for the driver core and kobjects say, and the author
	 * of this code has already been publically ridiculed for doing
	 * something as foolish as this.  However, at this point in time, it is
	 * the only way to handle the issue of statically allocated cpu
	 * devices.  The different architectures will have their cpu device
	 * code reworked to properly handle this in the near future, so this
	 * function will then be changed to correctly free up the memory held
	 * by the cpu device.
	 *
	 * Never copy this way of doing things, or you too will be made fun of
<<<<<<< HEAD
	 * on the linux-kerenl list, you have been warned.
	 */
}
>>>>>>> refs/remotes/origin/cm-10.0
=======
	 * on the linux-kernel list, you have been warned.
	 */
}
>>>>>>> refs/remotes/origin/master

/*
 * register_cpu - Setup a sysfs device for a CPU.
 * @cpu - cpu->hotpluggable field set to 1 will generate a control file in
 *	  sysfs for this CPU.
 * @num - CPU number to use when creating the device.
 *
 * Initialize and register the CPU device.
 */
<<<<<<< HEAD
int __cpuinit register_cpu(struct cpu *cpu, int num)
{
	int error;
<<<<<<< HEAD
	cpu->node_id = cpu_to_node(num);
	cpu->sysdev.id = num;
	cpu->sysdev.cls = &cpu_sysdev_class;

	error = sysdev_register(&cpu->sysdev);

	if (!error && cpu->hotpluggable)
		register_cpu_control(cpu);
	if (!error)
		per_cpu(cpu_sys_devices, num) = &cpu->sysdev;
=======
=======
int register_cpu(struct cpu *cpu, int num)
{
	int error;
>>>>>>> refs/remotes/origin/master

	cpu->node_id = cpu_to_node(num);
	memset(&cpu->dev, 0x00, sizeof(struct device));
	cpu->dev.id = num;
	cpu->dev.bus = &cpu_subsys;
	cpu->dev.release = cpu_device_release;
<<<<<<< HEAD
#ifdef CONFIG_ARCH_HAS_CPU_AUTOPROBE
	cpu->dev.bus->uevent = arch_cpu_uevent;
#endif
	error = device_register(&cpu->dev);
	if (!error && cpu->hotpluggable)
		register_cpu_control(cpu);
	if (!error)
		per_cpu(cpu_sys_devices, num) = &cpu->dev;
>>>>>>> refs/remotes/origin/cm-10.0
	if (!error)
		register_cpu_under_node(num, cpu_to_node(num));

#ifdef CONFIG_KEXEC
	if (!error)
<<<<<<< HEAD
		error = sysdev_create_file(&cpu->sysdev, &attr_crash_notes);
=======
		error = device_create_file(&cpu->dev, &dev_attr_crash_notes);
>>>>>>> refs/remotes/origin/cm-10.0
#endif
	return error;
}

<<<<<<< HEAD
struct sys_device *get_cpu_sysdev(unsigned cpu)
=======
struct device *get_cpu_device(unsigned cpu)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	cpu->dev.offline_disabled = !cpu->hotpluggable;
	cpu->dev.offline = !cpu_online(num);
	cpu->dev.of_node = of_get_cpu_node(num, NULL);
#ifdef CONFIG_ARCH_HAS_CPU_AUTOPROBE
	cpu->dev.bus->uevent = arch_cpu_uevent;
#endif
	cpu->dev.groups = common_cpu_attr_groups;
	if (cpu->hotpluggable)
		cpu->dev.groups = hotplugable_cpu_attr_groups;
	error = device_register(&cpu->dev);
	if (!error)
		per_cpu(cpu_sys_devices, num) = &cpu->dev;
	if (!error)
		register_cpu_under_node(num, cpu_to_node(num));

	return error;
}

struct device *get_cpu_device(unsigned cpu)
>>>>>>> refs/remotes/origin/master
{
	if (cpu < nr_cpu_ids && cpu_possible(cpu))
		return per_cpu(cpu_sys_devices, cpu);
	else
		return NULL;
}
<<<<<<< HEAD
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(get_cpu_sysdev);

int __init cpu_dev_init(void)
{
	int err;

	err = sysdev_class_register(&cpu_sysdev_class);
#if defined(CONFIG_SCHED_MC) || defined(CONFIG_SCHED_SMT)
	if (!err)
		err = sched_create_sysfs_power_savings_entries(&cpu_sysdev_class);
#endif

	return err;
}

static struct sysdev_class_attribute *cpu_sysdev_class_attrs[] = {
#ifdef CONFIG_ARCH_CPU_PROBE_RELEASE
	&attr_probe,
	&attr_release,
#endif
	&cpu_attrs[0].attr,
	&cpu_attrs[1].attr,
	&cpu_attrs[2].attr,
	&attr_kernel_max,
	&attr_offline,
	NULL
};
=======
=======
>>>>>>> refs/remotes/origin/master
EXPORT_SYMBOL_GPL(get_cpu_device);

#ifdef CONFIG_ARCH_HAS_CPU_AUTOPROBE
static DEVICE_ATTR(modalias, 0444, arch_print_cpu_modalias, NULL);
#endif

static struct attribute *cpu_root_attrs[] = {
#ifdef CONFIG_ARCH_CPU_PROBE_RELEASE
	&dev_attr_probe.attr,
	&dev_attr_release.attr,
#endif
	&cpu_attrs[0].attr.attr,
	&cpu_attrs[1].attr.attr,
	&cpu_attrs[2].attr.attr,
	&dev_attr_kernel_max.attr,
	&dev_attr_offline.attr,
#ifdef CONFIG_ARCH_HAS_CPU_AUTOPROBE
	&dev_attr_modalias.attr,
#endif
	NULL
};

static struct attribute_group cpu_root_attr_group = {
	.attrs = cpu_root_attrs,
};

static const struct attribute_group *cpu_root_attr_groups[] = {
	&cpu_root_attr_group,
	NULL,
};

bool cpu_is_hotpluggable(unsigned cpu)
{
	struct device *dev = get_cpu_device(cpu);
	return dev && container_of(dev, struct cpu, dev)->hotpluggable;
}
EXPORT_SYMBOL_GPL(cpu_is_hotpluggable);

#ifdef CONFIG_GENERIC_CPU_DEVICES
static DEFINE_PER_CPU(struct cpu, cpu_devices);
#endif

static void __init cpu_dev_register_generic(void)
{
#ifdef CONFIG_GENERIC_CPU_DEVICES
	int i;

	for_each_possible_cpu(i) {
		if (register_cpu(&per_cpu(cpu_devices, i), i))
			panic("Failed to register CPU device");
	}
#endif
}

void __init cpu_dev_init(void)
{
	if (subsys_system_register(&cpu_subsys, cpu_root_attr_groups))
		panic("Failed to register CPU subsystem");

	cpu_dev_register_generic();
<<<<<<< HEAD

#if defined(CONFIG_SCHED_MC) || defined(CONFIG_SCHED_SMT)
	sched_create_sysfs_power_savings_entries(cpu_subsys.dev_root);
#endif
}
>>>>>>> refs/remotes/origin/cm-10.0
=======
}
>>>>>>> refs/remotes/origin/master
