/*
<<<<<<< HEAD
 * drivers/base/memory.c - basic Memory class support
=======
 * Memory subsystem support
>>>>>>> refs/remotes/origin/cm-10.0
 *
 * Written by Matt Tolentino <matthew.e.tolentino@intel.com>
 *            Dave Hansen <haveblue@us.ibm.com>
 *
 * This file provides the necessary infrastructure to represent
 * a SPARSEMEM-memory-model system's physical memory in /sysfs.
 * All arch-independent code that assumes MEMORY_HOTPLUG requires
 * SPARSEMEM should be contained here, or in mm/memory_hotplug.c.
 */

<<<<<<< HEAD
#include <linux/sysdev.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/module.h>
#include <linux/init.h>
#include <linux/topology.h>
#include <linux/capability.h>
#include <linux/device.h>
#include <linux/memory.h>
#include <linux/kobject.h>
#include <linux/memory_hotplug.h>
#include <linux/mm.h>
#include <linux/mutex.h>
#include <linux/stat.h>
#include <linux/slab.h>

<<<<<<< HEAD
#include <asm/atomic.h>
=======
#include <linux/atomic.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <asm/uaccess.h>

static DEFINE_MUTEX(mem_sysfs_mutex);

#define MEMORY_CLASS_NAME	"memory"

static int sections_per_block;

static inline int base_memory_block_id(int section_nr)
{
	return section_nr / sections_per_block;
}

<<<<<<< HEAD
static struct sysdev_class memory_sysdev_class = {
	.name = MEMORY_CLASS_NAME,
};

static const char *memory_uevent_name(struct kset *kset, struct kobject *kobj)
{
	return MEMORY_CLASS_NAME;
}

static int memory_uevent(struct kset *kset, struct kobject *obj,
			struct kobj_uevent_env *env)
{
	int retval = 0;

	return retval;
}

static const struct kset_uevent_ops memory_uevent_ops = {
	.name		= memory_uevent_name,
	.uevent		= memory_uevent,
=======
static struct bus_type memory_subsys = {
	.name = MEMORY_CLASS_NAME,
	.dev_name = MEMORY_CLASS_NAME,
>>>>>>> refs/remotes/origin/cm-10.0
};

static BLOCKING_NOTIFIER_HEAD(memory_chain);

<<<<<<< HEAD
unsigned long movable_reserved_start, movable_reserved_size;
unsigned long low_power_memory_start, low_power_memory_size;

=======
>>>>>>> refs/remotes/origin/cm-10.0
int register_memory_notifier(struct notifier_block *nb)
{
        return blocking_notifier_chain_register(&memory_chain, nb);
}
EXPORT_SYMBOL(register_memory_notifier);

void unregister_memory_notifier(struct notifier_block *nb)
{
        blocking_notifier_chain_unregister(&memory_chain, nb);
}
EXPORT_SYMBOL(unregister_memory_notifier);

static ATOMIC_NOTIFIER_HEAD(memory_isolate_chain);

int register_memory_isolate_notifier(struct notifier_block *nb)
{
	return atomic_notifier_chain_register(&memory_isolate_chain, nb);
}
EXPORT_SYMBOL(register_memory_isolate_notifier);

void unregister_memory_isolate_notifier(struct notifier_block *nb)
{
	atomic_notifier_chain_unregister(&memory_isolate_chain, nb);
}
EXPORT_SYMBOL(unregister_memory_isolate_notifier);

/*
 * register_memory - Setup a sysfs device for a memory block
 */
static
int register_memory(struct memory_block *memory)
{
	int error;

<<<<<<< HEAD
	memory->sysdev.cls = &memory_sysdev_class;
	memory->sysdev.id = memory->start_section_nr / sections_per_block;

	error = sysdev_register(&memory->sysdev);
=======
	memory->dev.bus = &memory_subsys;
	memory->dev.id = memory->start_section_nr / sections_per_block;

	error = device_register(&memory->dev);
>>>>>>> refs/remotes/origin/cm-10.0
	return error;
}

static void
unregister_memory(struct memory_block *memory)
{
<<<<<<< HEAD
	BUG_ON(memory->sysdev.cls != &memory_sysdev_class);

	/* drop the ref. we got in remove_memory_block() */
	kobject_put(&memory->sysdev.kobj);
	sysdev_unregister(&memory->sysdev);
=======
	BUG_ON(memory->dev.bus != &memory_subsys);

	/* drop the ref. we got in remove_memory_block() */
	kobject_put(&memory->dev.kobj);
	device_unregister(&memory->dev);
>>>>>>> refs/remotes/origin/cm-10.0
}

unsigned long __weak memory_block_size_bytes(void)
{
	return MIN_MEMORY_BLOCK_SIZE;
}

static unsigned long get_memory_block_size(void)
{
	unsigned long block_sz;

	block_sz = memory_block_size_bytes();

	/* Validate blk_sz is a power of 2 and not less than section size */
	if ((block_sz & (block_sz - 1)) || (block_sz < MIN_MEMORY_BLOCK_SIZE)) {
		WARN_ON(1);
		block_sz = MIN_MEMORY_BLOCK_SIZE;
	}

	return block_sz;
}

/*
 * use this as the physical section index that this memsection
 * uses.
 */

<<<<<<< HEAD
static ssize_t show_mem_start_phys_index(struct sys_device *dev,
			struct sysdev_attribute *attr, char *buf)
{
	struct memory_block *mem =
		container_of(dev, struct memory_block, sysdev);
=======
static ssize_t show_mem_start_phys_index(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	struct memory_block *mem =
		container_of(dev, struct memory_block, dev);
>>>>>>> refs/remotes/origin/cm-10.0
	unsigned long phys_index;

	phys_index = mem->start_section_nr / sections_per_block;
	return sprintf(buf, "%08lx\n", phys_index);
}

<<<<<<< HEAD
static ssize_t show_mem_end_phys_index(struct sys_device *dev,
			struct sysdev_attribute *attr, char *buf)
{
	struct memory_block *mem =
		container_of(dev, struct memory_block, sysdev);
=======
static ssize_t show_mem_end_phys_index(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	struct memory_block *mem =
		container_of(dev, struct memory_block, dev);
>>>>>>> refs/remotes/origin/cm-10.0
	unsigned long phys_index;

	phys_index = mem->end_section_nr / sections_per_block;
	return sprintf(buf, "%08lx\n", phys_index);
}

/*
 * Show whether the section of memory is likely to be hot-removable
 */
<<<<<<< HEAD
static ssize_t show_mem_removable(struct sys_device *dev,
			struct sysdev_attribute *attr, char *buf)
=======
static ssize_t show_mem_removable(struct device *dev,
			struct device_attribute *attr, char *buf)
>>>>>>> refs/remotes/origin/cm-10.0
{
	unsigned long i, pfn;
	int ret = 1;
	struct memory_block *mem =
<<<<<<< HEAD
		container_of(dev, struct memory_block, sysdev);
=======
		container_of(dev, struct memory_block, dev);
>>>>>>> refs/remotes/origin/cm-10.0

	for (i = 0; i < sections_per_block; i++) {
		if (!present_section_nr(mem->start_section_nr + i))
			continue;
		pfn = section_nr_to_pfn(mem->start_section_nr + i);
		ret &= is_mem_section_removable(pfn, PAGES_PER_SECTION);
	}

	return sprintf(buf, "%d\n", ret);
}

/*
 * online, offline, going offline, etc.
 */
<<<<<<< HEAD
static ssize_t show_mem_state(struct sys_device *dev,
			struct sysdev_attribute *attr, char *buf)
{
	struct memory_block *mem =
		container_of(dev, struct memory_block, sysdev);
=======
static ssize_t show_mem_state(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	struct memory_block *mem =
		container_of(dev, struct memory_block, dev);
>>>>>>> refs/remotes/origin/cm-10.0
	ssize_t len = 0;

	/*
	 * We can probably put these states in a nice little array
	 * so that they're not open-coded
	 */
	switch (mem->state) {
		case MEM_ONLINE:
			len = sprintf(buf, "online\n");
			break;
		case MEM_OFFLINE:
			len = sprintf(buf, "offline\n");
			break;
		case MEM_GOING_OFFLINE:
			len = sprintf(buf, "going-offline\n");
			break;
		default:
			len = sprintf(buf, "ERROR-UNKNOWN-%ld\n",
					mem->state);
			WARN_ON(1);
			break;
	}

	return len;
}

int memory_notify(unsigned long val, void *v)
{
	return blocking_notifier_call_chain(&memory_chain, val, v);
}

int memory_isolate_notify(unsigned long val, void *v)
{
	return atomic_notifier_call_chain(&memory_isolate_chain, val, v);
}

/*
 * The probe routines leave the pages reserved, just as the bootmem code does.
 * Make sure they're still that way.
 */
static bool pages_correctly_reserved(unsigned long start_pfn,
					unsigned long nr_pages)
{
	int i, j;
	struct page *page;
	unsigned long pfn = start_pfn;

	/*
	 * memmap between sections is not contiguous except with
	 * SPARSEMEM_VMEMMAP. We lookup the page once per section
	 * and assume memmap is contiguous within each section
	 */
	for (i = 0; i < sections_per_block; i++, pfn += PAGES_PER_SECTION) {
		if (WARN_ON_ONCE(!pfn_valid(pfn)))
			return false;
		page = pfn_to_page(pfn);

		for (j = 0; j < PAGES_PER_SECTION; j++) {
			if (PageReserved(page + j))
				continue;

			printk(KERN_WARNING "section number %ld page number %d "
				"not reserved, was it already online?\n",
				pfn_to_section_nr(pfn), j);

			return false;
		}
	}

	return true;
}

/*
 * MEMORY_HOTPLUG depends on SPARSEMEM in mm/Kconfig, so it is
 * OK to have direct references to sparsemem variables in here.
 */
static int
memory_block_action(unsigned long phys_index, unsigned long action)
{
	unsigned long start_pfn, start_paddr;
	unsigned long nr_pages = PAGES_PER_SECTION * sections_per_block;
	struct page *first_page;
	int ret;

	first_page = pfn_to_page(phys_index << PFN_SECTION_SHIFT);

	switch (action) {
		case MEM_ONLINE:
			start_pfn = page_to_pfn(first_page);

			if (!pages_correctly_reserved(start_pfn, nr_pages))
				return -EBUSY;

			ret = online_pages(start_pfn, nr_pages);
			break;
		case MEM_OFFLINE:
			start_paddr = page_to_pfn(first_page) << PAGE_SHIFT;
			ret = remove_memory(start_paddr,
					    nr_pages << PAGE_SHIFT);
			break;
		default:
			WARN(1, KERN_WARNING "%s(%ld, %ld) unknown action: "
			     "%ld\n", __func__, phys_index, action, action);
			ret = -EINVAL;
	}

	return ret;
}

static int memory_block_change_state(struct memory_block *mem,
		unsigned long to_state, unsigned long from_state_req)
{
	int ret = 0;

	mutex_lock(&mem->state_mutex);

	if (mem->state != from_state_req) {
		ret = -EINVAL;
		goto out;
	}

	if (to_state == MEM_OFFLINE)
		mem->state = MEM_GOING_OFFLINE;

	ret = memory_block_action(mem->start_section_nr, to_state);

<<<<<<< HEAD
	if (ret)
		mem->state = from_state_req;
	else
		mem->state = to_state;

=======
	if (ret) {
		mem->state = from_state_req;
		goto out;
	}

	mem->state = to_state;
	switch (mem->state) {
	case MEM_OFFLINE:
		kobject_uevent(&mem->dev.kobj, KOBJ_OFFLINE);
		break;
	case MEM_ONLINE:
		kobject_uevent(&mem->dev.kobj, KOBJ_ONLINE);
		break;
	default:
		break;
	}
>>>>>>> refs/remotes/origin/cm-10.0
out:
	mutex_unlock(&mem->state_mutex);
	return ret;
}

static ssize_t
<<<<<<< HEAD
store_mem_state(struct sys_device *dev,
		struct sysdev_attribute *attr, const char *buf, size_t count)
=======
store_mem_state(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct memory_block *mem;
	int ret = -EINVAL;

<<<<<<< HEAD
	mem = container_of(dev, struct memory_block, sysdev);
=======
	mem = container_of(dev, struct memory_block, dev);
>>>>>>> refs/remotes/origin/cm-10.0

	if (!strncmp(buf, "online", min((int)count, 6)))
		ret = memory_block_change_state(mem, MEM_ONLINE, MEM_OFFLINE);
	else if(!strncmp(buf, "offline", min((int)count, 7)))
		ret = memory_block_change_state(mem, MEM_OFFLINE, MEM_ONLINE);

	if (ret)
		return ret;
	return count;
}

/*
 * phys_device is a bad name for this.  What I really want
 * is a way to differentiate between memory ranges that
 * are part of physical devices that constitute
 * a complete removable unit or fru.
 * i.e. do these ranges belong to the same physical device,
 * s.t. if I offline all of these sections I can then
 * remove the physical device?
 */
<<<<<<< HEAD
static ssize_t show_phys_device(struct sys_device *dev,
				struct sysdev_attribute *attr, char *buf)
{
	struct memory_block *mem =
		container_of(dev, struct memory_block, sysdev);
	return sprintf(buf, "%d\n", mem->phys_device);
}

static SYSDEV_ATTR(phys_index, 0444, show_mem_start_phys_index, NULL);
static SYSDEV_ATTR(end_phys_index, 0444, show_mem_end_phys_index, NULL);
static SYSDEV_ATTR(state, 0644, show_mem_state, store_mem_state);
static SYSDEV_ATTR(phys_device, 0444, show_phys_device, NULL);
static SYSDEV_ATTR(removable, 0444, show_mem_removable, NULL);

#define mem_create_simple_file(mem, attr_name)	\
	sysdev_create_file(&mem->sysdev, &attr_##attr_name)
#define mem_remove_simple_file(mem, attr_name)	\
	sysdev_remove_file(&mem->sysdev, &attr_##attr_name)
=======
static ssize_t show_phys_device(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct memory_block *mem =
		container_of(dev, struct memory_block, dev);
	return sprintf(buf, "%d\n", mem->phys_device);
}

static DEVICE_ATTR(phys_index, 0444, show_mem_start_phys_index, NULL);
static DEVICE_ATTR(end_phys_index, 0444, show_mem_end_phys_index, NULL);
static DEVICE_ATTR(state, 0644, show_mem_state, store_mem_state);
static DEVICE_ATTR(phys_device, 0444, show_phys_device, NULL);
static DEVICE_ATTR(removable, 0444, show_mem_removable, NULL);

#define mem_create_simple_file(mem, attr_name)	\
	device_create_file(&mem->dev, &dev_attr_##attr_name)
#define mem_remove_simple_file(mem, attr_name)	\
	device_remove_file(&mem->dev, &dev_attr_##attr_name)
>>>>>>> refs/remotes/origin/cm-10.0

/*
 * Block size attribute stuff
 */
static ssize_t
<<<<<<< HEAD
print_block_size(struct sysdev_class *class, struct sysdev_class_attribute *attr,
=======
print_block_size(struct device *dev, struct device_attribute *attr,
>>>>>>> refs/remotes/origin/cm-10.0
		 char *buf)
{
	return sprintf(buf, "%lx\n", get_memory_block_size());
}

<<<<<<< HEAD
static SYSDEV_CLASS_ATTR(block_size_bytes, 0444, print_block_size, NULL);

static int block_size_init(void)
{
	return sysfs_create_file(&memory_sysdev_class.kset.kobj,
				&attr_block_size_bytes.attr);
}

static ssize_t
print_movable_size(struct class *class, struct class_attribute *attr, char *buf)
{
	return sprintf(buf, "%lx\n", movable_reserved_size);
}

static CLASS_ATTR(movable_size_bytes, 0444, print_movable_size, NULL);

static int movable_size_init(void)
{
	return sysfs_create_file(&memory_sysdev_class.kset.kobj,
				&class_attr_movable_size_bytes.attr);
}

static ssize_t
print_movable_start(struct class *class, struct class_attribute *attr, char *buf)
{
	return sprintf(buf, "%lx\n", movable_reserved_start);
}

static CLASS_ATTR(movable_start_bytes, 0444, print_movable_start, NULL);

static int movable_start_init(void)
{
	return sysfs_create_file(&memory_sysdev_class.kset.kobj,
				&class_attr_movable_start_bytes.attr);
}

static ssize_t
print_low_power_memory_size(struct class *class, struct class_attribute *attr, char *buf)
{
	return sprintf(buf, "%lx\n", low_power_memory_size);
}

static CLASS_ATTR(low_power_memory_size_bytes, 0444,
	print_low_power_memory_size, NULL);

static int low_power_memory_size_init(void)
{
	return sysfs_create_file(&memory_sysdev_class.kset.kobj,
				&class_attr_low_power_memory_size_bytes.attr);
}

static ssize_t
print_low_power_memory_start(struct class *class, struct class_attribute *attr, char *buf)
{
	return sprintf(buf, "%lx\n", low_power_memory_start);
}

static CLASS_ATTR(low_power_memory_start_bytes, 0444,
	print_low_power_memory_start, NULL);

static int low_power_memory_start_init(void)
{
	return sysfs_create_file(&memory_sysdev_class.kset.kobj,
				&class_attr_low_power_memory_start_bytes.attr);
=======
static DEVICE_ATTR(block_size_bytes, 0444, print_block_size, NULL);

static int block_size_init(void)
{
	return device_create_file(memory_subsys.dev_root,
				  &dev_attr_block_size_bytes);
>>>>>>> refs/remotes/origin/cm-10.0
}

/*
 * Some architectures will have custom drivers to do this, and
 * will not need to do it from userspace.  The fake hot-add code
 * as well as ppc64 will do all of their discovery in userspace
 * and will require this interface.
 */
#ifdef CONFIG_ARCH_MEMORY_PROBE
static ssize_t
<<<<<<< HEAD
memory_probe_store(struct class *class, struct class_attribute *attr,
=======
memory_probe_store(struct device *dev, struct device_attribute *attr,
>>>>>>> refs/remotes/origin/cm-10.0
		   const char *buf, size_t count)
{
	u64 phys_addr;
	int nid;
	int i, ret;
<<<<<<< HEAD

	phys_addr = simple_strtoull(buf, NULL, 0);

=======
	unsigned long pages_per_block = PAGES_PER_SECTION * sections_per_block;

	phys_addr = simple_strtoull(buf, NULL, 0);

	if (phys_addr & ((pages_per_block << PAGE_SHIFT) - 1))
		return -EINVAL;

>>>>>>> refs/remotes/origin/cm-10.0
	for (i = 0; i < sections_per_block; i++) {
		nid = memory_add_physaddr_to_nid(phys_addr);
		ret = add_memory(nid, phys_addr,
				 PAGES_PER_SECTION << PAGE_SHIFT);
		if (ret)
			goto out;

		phys_addr += MIN_MEMORY_BLOCK_SIZE;
	}

	ret = count;
out:
	return ret;
}
<<<<<<< HEAD
static CLASS_ATTR(probe, S_IWUSR, NULL, memory_probe_store);

static int memory_probe_init(void)
{
	return sysfs_create_file(&memory_sysdev_class.kset.kobj,
				&class_attr_probe.attr);
=======
static DEVICE_ATTR(probe, S_IWUSR, NULL, memory_probe_store);

static int memory_probe_init(void)
{
	return device_create_file(memory_subsys.dev_root, &dev_attr_probe);
>>>>>>> refs/remotes/origin/cm-10.0
}
#else
static inline int memory_probe_init(void)
{
	return 0;
}
#endif

#ifdef CONFIG_MEMORY_FAILURE
/*
 * Support for offlining pages of memory
 */

/* Soft offline a page */
static ssize_t
<<<<<<< HEAD
store_soft_offline_page(struct class *class,
			struct class_attribute *attr,
=======
store_soft_offline_page(struct device *dev,
			struct device_attribute *attr,
>>>>>>> refs/remotes/origin/cm-10.0
			const char *buf, size_t count)
{
	int ret;
	u64 pfn;
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	if (strict_strtoull(buf, 0, &pfn) < 0)
		return -EINVAL;
	pfn >>= PAGE_SHIFT;
	if (!pfn_valid(pfn))
		return -ENXIO;
	ret = soft_offline_page(pfn_to_page(pfn), 0);
	return ret == 0 ? count : ret;
}

/* Forcibly offline a page, including killing processes. */
static ssize_t
<<<<<<< HEAD
store_hard_offline_page(struct class *class,
			struct class_attribute *attr,
=======
store_hard_offline_page(struct device *dev,
			struct device_attribute *attr,
>>>>>>> refs/remotes/origin/cm-10.0
			const char *buf, size_t count)
{
	int ret;
	u64 pfn;
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	if (strict_strtoull(buf, 0, &pfn) < 0)
		return -EINVAL;
	pfn >>= PAGE_SHIFT;
<<<<<<< HEAD
	ret = __memory_failure(pfn, 0, 0);
	return ret ? ret : count;
}

static CLASS_ATTR(soft_offline_page, 0644, NULL, store_soft_offline_page);
static CLASS_ATTR(hard_offline_page, 0644, NULL, store_hard_offline_page);
=======
	ret = memory_failure(pfn, 0, 0);
	return ret ? ret : count;
}

static DEVICE_ATTR(soft_offline_page, 0644, NULL, store_soft_offline_page);
static DEVICE_ATTR(hard_offline_page, 0644, NULL, store_hard_offline_page);
>>>>>>> refs/remotes/origin/cm-10.0

static __init int memory_fail_init(void)
{
	int err;

<<<<<<< HEAD
	err = sysfs_create_file(&memory_sysdev_class.kset.kobj,
				&class_attr_soft_offline_page.attr);
	if (!err)
		err = sysfs_create_file(&memory_sysdev_class.kset.kobj,
				&class_attr_hard_offline_page.attr);
=======
	err = device_create_file(memory_subsys.dev_root,
				&dev_attr_soft_offline_page);
	if (!err)
		err = device_create_file(memory_subsys.dev_root,
				&dev_attr_hard_offline_page);
>>>>>>> refs/remotes/origin/cm-10.0
	return err;
}
#else
static inline int memory_fail_init(void)
{
	return 0;
}
#endif

<<<<<<< HEAD
#ifdef CONFIG_ARCH_MEMORY_REMOVE
static ssize_t
memory_remove_store(struct class *class, struct class_attribute *attr,
		    const char *buf, size_t count)
{
	u64 phys_addr;
	int ret;

	phys_addr = simple_strtoull(buf, NULL, 0);

	ret = physical_remove_memory(phys_addr,
		PAGES_PER_SECTION << PAGE_SHIFT);

	if (ret)
		count = ret;

	return count;
}
static CLASS_ATTR(remove, S_IWUSR, NULL, memory_remove_store);

static int memory_remove_init(void)
{
	return sysfs_create_file(&memory_sysdev_class.kset.kobj,
				&class_attr_remove.attr);
}

static ssize_t
memory_active_store(struct class *class, struct class_attribute *attr,
		    const char *buf, size_t count)
{
	u64 phys_addr;
	int ret;

	phys_addr = simple_strtoull(buf, NULL, 0);

	ret = physical_active_memory(phys_addr,
		PAGES_PER_SECTION << PAGE_SHIFT);

	if (ret)
		count = ret;

	return count;
}
static CLASS_ATTR(active, S_IWUSR, NULL, memory_active_store);

static int memory_active_init(void)
{
	return sysfs_create_file(&memory_sysdev_class.kset.kobj,
				&class_attr_active.attr);
}

static ssize_t
memory_low_power_store(struct class *class, struct class_attribute *attr,
		       const char *buf, size_t count)
{
	u64 phys_addr;
	int ret;

	phys_addr = simple_strtoull(buf, NULL, 0);

	ret = physical_low_power_memory(phys_addr,
		PAGES_PER_SECTION << PAGE_SHIFT);

	if (ret)
		count = ret;

	return count;
}
static CLASS_ATTR(low_power, S_IWUSR, NULL, memory_low_power_store);

static int memory_low_power_init(void)
{
	return sysfs_create_file(&memory_sysdev_class.kset.kobj,
				&class_attr_low_power.attr);
}
#else
static inline int memory_remove_init(void)
{
	return 0;
}
static inline int memory_active_init(void)
{
	return 0;
}
static inline int memory_low_power_init(void)
{
	return 0;
}
#endif

=======
>>>>>>> refs/remotes/origin/cm-10.0
/*
 * Note that phys_device is optional.  It is here to allow for
 * differentiation between which *physical* devices each
 * section belongs to...
 */
int __weak arch_get_memory_phys_device(unsigned long start_pfn)
{
	return 0;
}

<<<<<<< HEAD
struct memory_block *find_memory_block_hinted(struct mem_section *section,
					      struct memory_block *hint)
{
	struct kobject *kobj;
	struct sys_device *sysdev;
	struct memory_block *mem;
	char name[sizeof(MEMORY_CLASS_NAME) + 9 + 1];
	int block_id = base_memory_block_id(__section_nr(section));

	kobj = hint ? &hint->sysdev.kobj : NULL;

	/*
	 * This only works because we know that section == sysdev->id
	 * slightly redundant with sysdev_register()
	 */
	sprintf(&name[0], "%s%d", MEMORY_CLASS_NAME, block_id);

	kobj = kset_find_obj_hinted(&memory_sysdev_class.kset, name, kobj);
	if (!kobj)
		return NULL;

	sysdev = container_of(kobj, struct sys_device, kobj);
	mem = container_of(sysdev, struct memory_block, sysdev);

	return mem;
=======
/*
 * A reference for the returned object is held and the reference for the
 * hinted object is released.
 */
struct memory_block *find_memory_block_hinted(struct mem_section *section,
					      struct memory_block *hint)
{
	int block_id = base_memory_block_id(__section_nr(section));
	struct device *hintdev = hint ? &hint->dev : NULL;
	struct device *dev;

	dev = subsys_find_device_by_id(&memory_subsys, block_id, hintdev);
	if (hint)
		put_device(&hint->dev);
	if (!dev)
		return NULL;
	return container_of(dev, struct memory_block, dev);
>>>>>>> refs/remotes/origin/cm-10.0
}

/*
 * For now, we have a linear search to go find the appropriate
 * memory_block corresponding to a particular phys_index. If
 * this gets to be a real problem, we can always use a radix
 * tree or something here.
 *
<<<<<<< HEAD
 * This could be made generic for all sysdev classes.
=======
 * This could be made generic for all device subsystems.
>>>>>>> refs/remotes/origin/cm-10.0
 */
struct memory_block *find_memory_block(struct mem_section *section)
{
	return find_memory_block_hinted(section, NULL);
}

static int init_memory_block(struct memory_block **memory,
			     struct mem_section *section, unsigned long state)
{
	struct memory_block *mem;
	unsigned long start_pfn;
	int scn_nr;
	int ret = 0;

	mem = kzalloc(sizeof(*mem), GFP_KERNEL);
	if (!mem)
		return -ENOMEM;

	scn_nr = __section_nr(section);
	mem->start_section_nr =
			base_memory_block_id(scn_nr) * sections_per_block;
	mem->end_section_nr = mem->start_section_nr + sections_per_block - 1;
	mem->state = state;
	mem->section_count++;
	mutex_init(&mem->state_mutex);
	start_pfn = section_nr_to_pfn(mem->start_section_nr);
	mem->phys_device = arch_get_memory_phys_device(start_pfn);

	ret = register_memory(mem);
	if (!ret)
		ret = mem_create_simple_file(mem, phys_index);
	if (!ret)
		ret = mem_create_simple_file(mem, end_phys_index);
	if (!ret)
		ret = mem_create_simple_file(mem, state);
	if (!ret)
		ret = mem_create_simple_file(mem, phys_device);
	if (!ret)
		ret = mem_create_simple_file(mem, removable);

	*memory = mem;
	return ret;
}

static int add_memory_section(int nid, struct mem_section *section,
<<<<<<< HEAD
			unsigned long state, enum mem_add_context context)
{
	struct memory_block *mem;
=======
			struct memory_block **mem_p,
			unsigned long state, enum mem_add_context context)
{
	struct memory_block *mem = NULL;
	int scn_nr = __section_nr(section);
>>>>>>> refs/remotes/origin/cm-10.0
	int ret = 0;

	mutex_lock(&mem_sysfs_mutex);

<<<<<<< HEAD
	mem = find_memory_block(section);
	if (mem) {
		mem->section_count++;
		kobject_put(&mem->sysdev.kobj);
	} else
		ret = init_memory_block(&mem, section, state);
=======
	if (context == BOOT) {
		/* same memory block ? */
		if (mem_p && *mem_p)
			if (scn_nr >= (*mem_p)->start_section_nr &&
			    scn_nr <= (*mem_p)->end_section_nr) {
				mem = *mem_p;
				kobject_get(&mem->dev.kobj);
			}
	} else
		mem = find_memory_block(section);

	if (mem) {
		mem->section_count++;
		kobject_put(&mem->dev.kobj);
	} else {
		ret = init_memory_block(&mem, section, state);
		/* store memory_block pointer for next loop */
		if (!ret && context == BOOT)
			if (mem_p)
				*mem_p = mem;
	}
>>>>>>> refs/remotes/origin/cm-10.0

	if (!ret) {
		if (context == HOTPLUG &&
		    mem->section_count == sections_per_block)
			ret = register_mem_sect_under_node(mem, nid);
	}

	mutex_unlock(&mem_sysfs_mutex);
	return ret;
}

int remove_memory_block(unsigned long node_id, struct mem_section *section,
		int phys_device)
{
	struct memory_block *mem;

	mutex_lock(&mem_sysfs_mutex);
	mem = find_memory_block(section);
	unregister_mem_sect_under_nodes(mem, __section_nr(section));

	mem->section_count--;
	if (mem->section_count == 0) {
		mem_remove_simple_file(mem, phys_index);
		mem_remove_simple_file(mem, end_phys_index);
		mem_remove_simple_file(mem, state);
		mem_remove_simple_file(mem, phys_device);
		mem_remove_simple_file(mem, removable);
		unregister_memory(mem);
		kfree(mem);
	} else
<<<<<<< HEAD
		kobject_put(&mem->sysdev.kobj);
=======
		kobject_put(&mem->dev.kobj);
>>>>>>> refs/remotes/origin/cm-10.0

	mutex_unlock(&mem_sysfs_mutex);
	return 0;
}

/*
 * need an interface for the VM to add new memory regions,
 * but without onlining it.
 */
int register_new_memory(int nid, struct mem_section *section)
{
<<<<<<< HEAD
	return add_memory_section(nid, section, MEM_OFFLINE, HOTPLUG);
=======
	return add_memory_section(nid, section, NULL, MEM_OFFLINE, HOTPLUG);
>>>>>>> refs/remotes/origin/cm-10.0
}

int unregister_memory_section(struct mem_section *section)
{
	if (!present_section(section))
		return -EINVAL;

	return remove_memory_block(0, section, 0);
}

/*
 * Initialize the sysfs support for memory devices...
 */
int __init memory_dev_init(void)
{
	unsigned int i;
	int ret;
	int err;
	unsigned long block_sz;
<<<<<<< HEAD

	memory_sysdev_class.kset.uevent_ops = &memory_uevent_ops;
	ret = sysdev_class_register(&memory_sysdev_class);
=======
	struct memory_block *mem = NULL;

	ret = subsys_system_register(&memory_subsys, NULL);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret)
		goto out;

	block_sz = get_memory_block_size();
	sections_per_block = block_sz / MIN_MEMORY_BLOCK_SIZE;

	/*
	 * Create entries for memory sections that were found
	 * during boot and have been initialized
	 */
	for (i = 0; i < NR_MEM_SECTIONS; i++) {
		if (!present_section_nr(i))
			continue;
<<<<<<< HEAD
		err = add_memory_section(0, __nr_to_section(i), MEM_ONLINE,
=======
		/* don't need to reuse memory_block if only one per block */
		err = add_memory_section(0, __nr_to_section(i),
				 (sections_per_block == 1) ? NULL : &mem,
					 MEM_ONLINE,
>>>>>>> refs/remotes/origin/cm-10.0
					 BOOT);
		if (!ret)
			ret = err;
	}

	err = memory_probe_init();
	if (!ret)
		ret = err;
	err = memory_fail_init();
	if (!ret)
		ret = err;
<<<<<<< HEAD
	err = memory_remove_init();
	if (!ret)
		ret = err;
	err = memory_active_init();
	if (!ret)
		ret = err;
	err = memory_low_power_init();
	if (!ret)
		ret = err;
	err = block_size_init();
	if (!ret)
		ret = err;
	err = movable_size_init();
	if (!ret)
		ret = err;
	err = movable_start_init();
	if (!ret)
		ret = err;
	err = low_power_memory_size_init();
	if (!ret)
		ret = err;
	err = low_power_memory_start_init();
	if (!ret)
		ret = err;
=======
	err = block_size_init();
	if (!ret)
		ret = err;
>>>>>>> refs/remotes/origin/cm-10.0
out:
	if (ret)
		printk(KERN_ERR "%s() failed: %d\n", __func__, ret);
	return ret;
}
