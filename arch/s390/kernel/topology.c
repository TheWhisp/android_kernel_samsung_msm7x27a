/*
<<<<<<< HEAD
<<<<<<< HEAD
 *    Copyright IBM Corp. 2007
=======
 *    Copyright IBM Corp. 2007,2011
>>>>>>> refs/remotes/origin/cm-10.0
=======
 *    Copyright IBM Corp. 2007, 2011
>>>>>>> refs/remotes/origin/master
 *    Author(s): Heiko Carstens <heiko.carstens@de.ibm.com>
 */

#define KMSG_COMPONENT "cpu"
#define pr_fmt(fmt) KMSG_COMPONENT ": " fmt

<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/bootmem.h>
#include <linux/sched.h>
#include <linux/workqueue.h>
#include <linux/cpu.h>
#include <linux/smp.h>
#include <linux/cpuset.h>
#include <asm/delay.h>
=======
=======
>>>>>>> refs/remotes/origin/master
#include <linux/workqueue.h>
#include <linux/bootmem.h>
#include <linux/cpuset.h>
#include <linux/device.h>
<<<<<<< HEAD
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/cpu.h>
#include <linux/smp.h>
#include <linux/mm.h>
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <asm/sysinfo.h>
>>>>>>> refs/remotes/origin/master

#define PTF_HORIZONTAL	(0UL)
#define PTF_VERTICAL	(1UL)
#define PTF_CHECK	(2UL)

struct mask_info {
	struct mask_info *next;
	unsigned char id;
	cpumask_t mask;
};

<<<<<<< HEAD
static int topology_enabled = 1;
static void topology_work_fn(struct work_struct *work);
static struct sysinfo_15_1_x *tl_info;
<<<<<<< HEAD
static struct timer_list topology_timer;
=======
>>>>>>> refs/remotes/origin/cm-10.0
static void set_topology_timer(void);
static DECLARE_WORK(topology_work, topology_work_fn);
/* topology_lock protects the core linked list */
static DEFINE_SPINLOCK(topology_lock);

static struct mask_info core_info;
cpumask_t cpu_core_map[NR_CPUS];
unsigned char cpu_core_id[NR_CPUS];

<<<<<<< HEAD
#ifdef CONFIG_SCHED_BOOK
static struct mask_info book_info;
cpumask_t cpu_book_map[NR_CPUS];
unsigned char cpu_book_id[NR_CPUS];
#endif
=======
static struct mask_info book_info;
cpumask_t cpu_book_map[NR_CPUS];
unsigned char cpu_book_id[NR_CPUS];

/* smp_cpu_state_mutex must be held when accessing this array */
int cpu_polarization[NR_CPUS];
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void set_topology_timer(void);
static void topology_work_fn(struct work_struct *work);
static struct sysinfo_15_1_x *tl_info;

static int topology_enabled = 1;
static DECLARE_WORK(topology_work, topology_work_fn);

/* topology_lock protects the socket and book linked lists */
static DEFINE_SPINLOCK(topology_lock);
static struct mask_info socket_info;
static struct mask_info book_info;

struct cpu_topology_s390 cpu_topology[NR_CPUS];
EXPORT_SYMBOL_GPL(cpu_topology);
>>>>>>> refs/remotes/origin/master

static cpumask_t cpu_group_map(struct mask_info *info, unsigned int cpu)
{
	cpumask_t mask;

<<<<<<< HEAD
	cpumask_clear(&mask);
	if (!topology_enabled || !MACHINE_HAS_TOPOLOGY) {
		cpumask_copy(&mask, cpumask_of(cpu));
		return mask;
	}
	while (info) {
		if (cpumask_test_cpu(cpu, &info->mask)) {
			mask = info->mask;
			break;
		}
		info = info->next;
	}
	if (cpumask_empty(&mask))
		cpumask_copy(&mask, cpumask_of(cpu));
	return mask;
}

<<<<<<< HEAD
static void add_cpus_to_mask(struct topology_cpu *tl_cpu,
			     struct mask_info *book, struct mask_info *core)
=======
static struct mask_info *add_cpus_to_mask(struct topology_cpu *tl_cpu,
					  struct mask_info *book,
					  struct mask_info *core,
					  int one_core_per_cpu)
>>>>>>> refs/remotes/origin/cm-10.0
{
	unsigned int cpu;

	for (cpu = find_first_bit(&tl_cpu->mask[0], TOPOLOGY_CPU_BITS);
	     cpu < TOPOLOGY_CPU_BITS;
	     cpu = find_next_bit(&tl_cpu->mask[0], TOPOLOGY_CPU_BITS, cpu + 1))
	{
<<<<<<< HEAD
		unsigned int rcpu, lcpu;

		rcpu = TOPOLOGY_CPU_BITS - 1 - cpu + tl_cpu->origin;
		for_each_present_cpu(lcpu) {
			if (cpu_logical_map(lcpu) != rcpu)
				continue;
#ifdef CONFIG_SCHED_BOOK
			cpumask_set_cpu(lcpu, &book->mask);
			cpu_book_id[lcpu] = book->id;
#endif
			cpumask_set_cpu(lcpu, &core->mask);
			cpu_core_id[lcpu] = core->id;
			smp_cpu_polarization[lcpu] = tl_cpu->pp;
		}
	}
=======
=======
	cpumask_copy(&mask, cpumask_of(cpu));
	if (!topology_enabled || !MACHINE_HAS_TOPOLOGY)
		return mask;
	for (; info; info = info->next) {
		if (cpumask_test_cpu(cpu, &info->mask))
			return info->mask;
	}
	return mask;
}

static struct mask_info *add_cpus_to_mask(struct topology_cpu *tl_cpu,
					  struct mask_info *book,
					  struct mask_info *socket,
					  int one_socket_per_cpu)
{
	unsigned int cpu;

	for_each_set_bit(cpu, &tl_cpu->mask[0], TOPOLOGY_CPU_BITS) {
>>>>>>> refs/remotes/origin/master
		unsigned int rcpu;
		int lcpu;

		rcpu = TOPOLOGY_CPU_BITS - 1 - cpu + tl_cpu->origin;
		lcpu = smp_find_processor_id(rcpu);
<<<<<<< HEAD
		if (lcpu >= 0) {
			cpumask_set_cpu(lcpu, &book->mask);
			cpu_book_id[lcpu] = book->id;
			cpumask_set_cpu(lcpu, &core->mask);
			if (one_core_per_cpu) {
				cpu_core_id[lcpu] = rcpu;
				core = core->next;
			} else {
				cpu_core_id[lcpu] = core->id;
			}
			cpu_set_polarization(lcpu, tl_cpu->pp);
		}
	}
	return core;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (lcpu < 0)
			continue;
		cpumask_set_cpu(lcpu, &book->mask);
		cpu_topology[lcpu].book_id = book->id;
		cpumask_set_cpu(lcpu, &socket->mask);
		cpu_topology[lcpu].core_id = rcpu;
		if (one_socket_per_cpu) {
			cpu_topology[lcpu].socket_id = rcpu;
			socket = socket->next;
		} else {
			cpu_topology[lcpu].socket_id = socket->id;
		}
		smp_cpu_set_polarization(lcpu, tl_cpu->pp);
	}
	return socket;
>>>>>>> refs/remotes/origin/master
}

static void clear_masks(void)
{
	struct mask_info *info;

<<<<<<< HEAD
	info = &core_info;
=======
	info = &socket_info;
>>>>>>> refs/remotes/origin/master
	while (info) {
		cpumask_clear(&info->mask);
		info = info->next;
	}
<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_SCHED_BOOK
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	info = &book_info;
	while (info) {
		cpumask_clear(&info->mask);
		info = info->next;
	}
<<<<<<< HEAD
<<<<<<< HEAD
#endif
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static union topology_entry *next_tle(union topology_entry *tle)
{
	if (!tle->nl)
		return (union topology_entry *)((struct topology_cpu *)tle + 1);
	return (union topology_entry *)((struct topology_container *)tle + 1);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void tl_to_cores(struct sysinfo_15_1_x *info)
{
#ifdef CONFIG_SCHED_BOOK
	struct mask_info *book = &book_info;
#else
	struct mask_info *book = NULL;
#endif
	struct mask_info *core = &core_info;
	union topology_entry *tle, *end;


	spin_lock_irq(&topology_lock);
	clear_masks();
=======
static void __tl_to_cores_generic(struct sysinfo_15_1_x *info)
{
	struct mask_info *core = &core_info;
	struct mask_info *book = &book_info;
	union topology_entry *tle, *end;

>>>>>>> refs/remotes/origin/cm-10.0
=======
static void __tl_to_masks_generic(struct sysinfo_15_1_x *info)
{
	struct mask_info *socket = &socket_info;
	struct mask_info *book = &book_info;
	union topology_entry *tle, *end;

>>>>>>> refs/remotes/origin/master
	tle = info->tle;
	end = (union topology_entry *)((unsigned long)info + info->length);
	while (tle < end) {
		switch (tle->nl) {
<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_SCHED_BOOK
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		case 2:
			book = book->next;
			book->id = tle->container.id;
			break;
<<<<<<< HEAD
<<<<<<< HEAD
#endif
=======
>>>>>>> refs/remotes/origin/cm-10.0
		case 1:
			core = core->next;
			core->id = tle->container.id;
			break;
		case 0:
<<<<<<< HEAD
			add_cpus_to_mask(&tle->cpu, book, core);
			break;
		default:
			clear_masks();
			goto out;
		}
		tle = next_tle(tle);
	}
out:
=======
			add_cpus_to_mask(&tle->cpu, book, core, 0);
=======
		case 1:
			socket = socket->next;
			socket->id = tle->container.id;
			break;
		case 0:
			add_cpus_to_mask(&tle->cpu, book, socket, 0);
>>>>>>> refs/remotes/origin/master
			break;
		default:
			clear_masks();
			return;
		}
		tle = next_tle(tle);
	}
}

<<<<<<< HEAD
static void __tl_to_cores_z10(struct sysinfo_15_1_x *info)
{
	struct mask_info *core = &core_info;
=======
static void __tl_to_masks_z10(struct sysinfo_15_1_x *info)
{
	struct mask_info *socket = &socket_info;
>>>>>>> refs/remotes/origin/master
	struct mask_info *book = &book_info;
	union topology_entry *tle, *end;

	tle = info->tle;
	end = (union topology_entry *)((unsigned long)info + info->length);
	while (tle < end) {
		switch (tle->nl) {
		case 1:
			book = book->next;
			book->id = tle->container.id;
			break;
		case 0:
<<<<<<< HEAD
			core = add_cpus_to_mask(&tle->cpu, book, core, 1);
=======
			socket = add_cpus_to_mask(&tle->cpu, book, socket, 1);
>>>>>>> refs/remotes/origin/master
			break;
		default:
			clear_masks();
			return;
		}
		tle = next_tle(tle);
	}
}

<<<<<<< HEAD
static void tl_to_cores(struct sysinfo_15_1_x *info)
{
	struct cpuid cpu_id;

	get_cpu_id(&cpu_id);
	spin_lock_irq(&topology_lock);
=======
static void tl_to_masks(struct sysinfo_15_1_x *info)
{
	struct cpuid cpu_id;

	spin_lock_irq(&topology_lock);
	get_cpu_id(&cpu_id);
>>>>>>> refs/remotes/origin/master
	clear_masks();
	switch (cpu_id.machine) {
	case 0x2097:
	case 0x2098:
<<<<<<< HEAD
		__tl_to_cores_z10(info);
		break;
	default:
		__tl_to_cores_generic(info);
	}
>>>>>>> refs/remotes/origin/cm-10.0
=======
		__tl_to_masks_z10(info);
		break;
	default:
		__tl_to_masks_generic(info);
	}
>>>>>>> refs/remotes/origin/master
	spin_unlock_irq(&topology_lock);
}

static void topology_update_polarization_simple(void)
{
	int cpu;

	mutex_lock(&smp_cpu_state_mutex);
	for_each_possible_cpu(cpu)
<<<<<<< HEAD
<<<<<<< HEAD
		smp_cpu_polarization[cpu] = POLARIZATION_HRZ;
=======
		cpu_set_polarization(cpu, POLARIZATION_HRZ);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		smp_cpu_set_polarization(cpu, POLARIZATION_HRZ);
>>>>>>> refs/remotes/origin/master
	mutex_unlock(&smp_cpu_state_mutex);
}

static int ptf(unsigned long fc)
{
	int rc;

	asm volatile(
		"	.insn	rre,0xb9a20000,%1,%1\n"
		"	ipm	%0\n"
		"	srl	%0,28\n"
		: "=d" (rc)
		: "d" (fc)  : "cc");
	return rc;
}

int topology_set_cpu_management(int fc)
{
<<<<<<< HEAD
<<<<<<< HEAD
	int cpu;
	int rc;
=======
	int cpu, rc;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	int cpu, rc;
>>>>>>> refs/remotes/origin/master

	if (!MACHINE_HAS_TOPOLOGY)
		return -EOPNOTSUPP;
	if (fc)
		rc = ptf(PTF_VERTICAL);
	else
		rc = ptf(PTF_HORIZONTAL);
	if (rc)
		return -EBUSY;
	for_each_possible_cpu(cpu)
<<<<<<< HEAD
<<<<<<< HEAD
		smp_cpu_polarization[cpu] = POLARIZATION_UNKNWN;
=======
		cpu_set_polarization(cpu, POLARIZATION_UNKNOWN);
>>>>>>> refs/remotes/origin/cm-10.0
	return rc;
}

static void update_cpu_core_map(void)
=======
		smp_cpu_set_polarization(cpu, POLARIZATION_UNKNOWN);
	return rc;
}

static void update_cpu_masks(void)
>>>>>>> refs/remotes/origin/master
{
	unsigned long flags;
	int cpu;

	spin_lock_irqsave(&topology_lock, flags);
	for_each_possible_cpu(cpu) {
<<<<<<< HEAD
		cpu_core_map[cpu] = cpu_group_map(&core_info, cpu);
<<<<<<< HEAD
#ifdef CONFIG_SCHED_BOOK
		cpu_book_map[cpu] = cpu_group_map(&book_info, cpu);
#endif
=======
		cpu_book_map[cpu] = cpu_group_map(&book_info, cpu);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		cpu_topology[cpu].core_mask = cpu_group_map(&socket_info, cpu);
		cpu_topology[cpu].book_mask = cpu_group_map(&book_info, cpu);
		if (!MACHINE_HAS_TOPOLOGY) {
			cpu_topology[cpu].core_id = cpu;
			cpu_topology[cpu].socket_id = cpu;
			cpu_topology[cpu].book_id = cpu;
		}
>>>>>>> refs/remotes/origin/master
	}
	spin_unlock_irqrestore(&topology_lock, flags);
}

void store_topology(struct sysinfo_15_1_x *info)
{
<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_SCHED_BOOK
=======
>>>>>>> refs/remotes/origin/cm-10.0
	int rc;

	rc = stsi(info, 15, 1, 3);
	if (rc != -ENOSYS)
		return;
<<<<<<< HEAD
#endif
=======
>>>>>>> refs/remotes/origin/cm-10.0
	stsi(info, 15, 1, 2);
=======
	if (topology_max_mnest >= 3)
		stsi(info, 15, 1, 3);
	else
		stsi(info, 15, 1, 2);
>>>>>>> refs/remotes/origin/master
}

int arch_update_cpu_topology(void)
{
	struct sysinfo_15_1_x *info = tl_info;
<<<<<<< HEAD
<<<<<<< HEAD
	struct sys_device *sysdev;
=======
	struct device *dev;
>>>>>>> refs/remotes/origin/cm-10.0
	int cpu;

	if (!MACHINE_HAS_TOPOLOGY) {
		update_cpu_core_map();
=======
	struct device *dev;
	int cpu;

	if (!MACHINE_HAS_TOPOLOGY) {
		update_cpu_masks();
>>>>>>> refs/remotes/origin/master
		topology_update_polarization_simple();
		return 0;
	}
	store_topology(info);
<<<<<<< HEAD
	tl_to_cores(info);
	update_cpu_core_map();
	for_each_online_cpu(cpu) {
<<<<<<< HEAD
		sysdev = get_cpu_sysdev(cpu);
		kobject_uevent(&sysdev->kobj, KOBJ_CHANGE);
=======
		dev = get_cpu_device(cpu);
		kobject_uevent(&dev->kobj, KOBJ_CHANGE);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	tl_to_masks(info);
	update_cpu_masks();
	for_each_online_cpu(cpu) {
		dev = get_cpu_device(cpu);
		kobject_uevent(&dev->kobj, KOBJ_CHANGE);
>>>>>>> refs/remotes/origin/master
	}
	return 1;
}

static void topology_work_fn(struct work_struct *work)
{
	rebuild_sched_domains();
}

void topology_schedule_update(void)
{
	schedule_work(&topology_work);
}

static void topology_timer_fn(unsigned long ignored)
{
	if (ptf(PTF_CHECK))
		topology_schedule_update();
	set_topology_timer();
}

<<<<<<< HEAD
<<<<<<< HEAD
static void set_topology_timer(void)
{
	topology_timer.function = topology_timer_fn;
	topology_timer.data = 0;
	topology_timer.expires = jiffies + 60 * HZ;
	add_timer(&topology_timer);
=======
=======
>>>>>>> refs/remotes/origin/master
static struct timer_list topology_timer =
	TIMER_DEFERRED_INITIALIZER(topology_timer_fn, 0, 0);

static atomic_t topology_poll = ATOMIC_INIT(0);

static void set_topology_timer(void)
{
	if (atomic_add_unless(&topology_poll, -1, 0))
		mod_timer(&topology_timer, jiffies + HZ / 10);
	else
		mod_timer(&topology_timer, jiffies + HZ * 60);
}

void topology_expect_change(void)
{
	if (!MACHINE_HAS_TOPOLOGY)
		return;
	/* This is racy, but it doesn't matter since it is just a heuristic.
	 * Worst case is that we poll in a higher frequency for a bit longer.
	 */
	if (atomic_read(&topology_poll) > 60)
		return;
	atomic_add(60, &topology_poll);
	set_topology_timer();
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static int __init early_parse_topology(char *p)
{
	if (strncmp(p, "off", 3))
		return 0;
	topology_enabled = 0;
	return 0;
}
early_param("topology", early_parse_topology);

<<<<<<< HEAD
<<<<<<< HEAD
static int __init init_topology_update(void)
{
	int rc;

	rc = 0;
	if (!MACHINE_HAS_TOPOLOGY) {
		topology_update_polarization_simple();
		goto out;
	}
	init_timer_deferrable(&topology_timer);
	set_topology_timer();
out:
	update_cpu_core_map();
	return rc;
}
__initcall(init_topology_update);

static void alloc_masks(struct sysinfo_15_1_x *info, struct mask_info *mask,
			int offset)
=======
static void __init alloc_masks(struct sysinfo_15_1_x *info,
			       struct mask_info *mask, int offset)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void __init alloc_masks(struct sysinfo_15_1_x *info,
			       struct mask_info *mask, int offset)
>>>>>>> refs/remotes/origin/master
{
	int i, nr_masks;

	nr_masks = info->mag[TOPOLOGY_NR_MAG - offset];
	for (i = 0; i < info->mnest - offset; i++)
		nr_masks *= info->mag[TOPOLOGY_NR_MAG - offset - 1 - i];
	nr_masks = max(nr_masks, 1);
	for (i = 0; i < nr_masks; i++) {
		mask->next = alloc_bootmem(sizeof(struct mask_info));
		mask = mask->next;
	}
}

void __init s390_init_cpu_topology(void)
{
	struct sysinfo_15_1_x *info;
	int i;

	if (!MACHINE_HAS_TOPOLOGY)
		return;
	tl_info = alloc_bootmem_pages(PAGE_SIZE);
	info = tl_info;
	store_topology(info);
	pr_info("The CPU configuration topology of the machine is:");
	for (i = 0; i < TOPOLOGY_NR_MAG; i++)
<<<<<<< HEAD
<<<<<<< HEAD
		printk(" %d", info->mag[i]);
	printk(" / %d\n", info->mnest);
	alloc_masks(info, &core_info, 2);
#ifdef CONFIG_SCHED_BOOK
	alloc_masks(info, &book_info, 3);
#endif
}
=======
		printk(KERN_CONT " %d", info->mag[i]);
	printk(KERN_CONT " / %d\n", info->mnest);
	alloc_masks(info, &core_info, 1);
=======
		printk(KERN_CONT " %d", info->mag[i]);
	printk(KERN_CONT " / %d\n", info->mnest);
	alloc_masks(info, &socket_info, 1);
>>>>>>> refs/remotes/origin/master
	alloc_masks(info, &book_info, 2);
}

static int cpu_management;

static ssize_t dispatching_show(struct device *dev,
				struct device_attribute *attr,
				char *buf)
{
	ssize_t count;

	mutex_lock(&smp_cpu_state_mutex);
	count = sprintf(buf, "%d\n", cpu_management);
	mutex_unlock(&smp_cpu_state_mutex);
	return count;
}

static ssize_t dispatching_store(struct device *dev,
				 struct device_attribute *attr,
				 const char *buf,
				 size_t count)
{
	int val, rc;
	char delim;

	if (sscanf(buf, "%d %c", &val, &delim) != 1)
		return -EINVAL;
	if (val != 0 && val != 1)
		return -EINVAL;
	rc = 0;
	get_online_cpus();
	mutex_lock(&smp_cpu_state_mutex);
	if (cpu_management == val)
		goto out;
	rc = topology_set_cpu_management(val);
	if (rc)
		goto out;
	cpu_management = val;
	topology_expect_change();
out:
	mutex_unlock(&smp_cpu_state_mutex);
	put_online_cpus();
	return rc ? rc : count;
}
static DEVICE_ATTR(dispatching, 0644, dispatching_show,
			 dispatching_store);

static ssize_t cpu_polarization_show(struct device *dev,
				     struct device_attribute *attr, char *buf)
{
	int cpu = dev->id;
	ssize_t count;

	mutex_lock(&smp_cpu_state_mutex);
<<<<<<< HEAD
	switch (cpu_read_polarization(cpu)) {
=======
	switch (smp_cpu_get_polarization(cpu)) {
>>>>>>> refs/remotes/origin/master
	case POLARIZATION_HRZ:
		count = sprintf(buf, "horizontal\n");
		break;
	case POLARIZATION_VL:
		count = sprintf(buf, "vertical:low\n");
		break;
	case POLARIZATION_VM:
		count = sprintf(buf, "vertical:medium\n");
		break;
	case POLARIZATION_VH:
		count = sprintf(buf, "vertical:high\n");
		break;
	default:
		count = sprintf(buf, "unknown\n");
		break;
	}
	mutex_unlock(&smp_cpu_state_mutex);
	return count;
}
static DEVICE_ATTR(polarization, 0444, cpu_polarization_show, NULL);

static struct attribute *topology_cpu_attrs[] = {
	&dev_attr_polarization.attr,
	NULL,
};

static struct attribute_group topology_cpu_attr_group = {
	.attrs = topology_cpu_attrs,
};

int topology_cpu_init(struct cpu *cpu)
{
	return sysfs_create_group(&cpu->dev.kobj, &topology_cpu_attr_group);
}

static int __init topology_init(void)
{
	if (!MACHINE_HAS_TOPOLOGY) {
		topology_update_polarization_simple();
		goto out;
	}
	set_topology_timer();
out:
<<<<<<< HEAD
	update_cpu_core_map();
	return device_create_file(cpu_subsys.dev_root, &dev_attr_dispatching);
}
device_initcall(topology_init);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	update_cpu_masks();
	return device_create_file(cpu_subsys.dev_root, &dev_attr_dispatching);
}
device_initcall(topology_init);
>>>>>>> refs/remotes/origin/master
