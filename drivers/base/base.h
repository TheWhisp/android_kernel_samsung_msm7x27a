<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/notifier.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/notifier.h>
>>>>>>> refs/remotes/origin/master

/**
 * struct subsys_private - structure to hold the private to the driver core portions of the bus_type/class structure.
 *
 * @subsys - the struct kset that defines this subsystem
<<<<<<< HEAD
<<<<<<< HEAD
 * @devices_kset - the list of devices associated
=======
 * @devices_kset - the subsystem's 'devices' directory
 * @interfaces - list of subsystem interfaces associated
 * @mutex - protect the devices, and interfaces lists.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * @devices_kset - the subsystem's 'devices' directory
 * @interfaces - list of subsystem interfaces associated
 * @mutex - protect the devices, and interfaces lists.
>>>>>>> refs/remotes/origin/master
 *
 * @drivers_kset - the list of drivers associated
 * @klist_devices - the klist to iterate over the @devices_kset
 * @klist_drivers - the klist to iterate over the @drivers_kset
 * @bus_notifier - the bus notifier list for anything that cares about things
 *                 on this bus.
 * @bus - pointer back to the struct bus_type that this structure is associated
 *        with.
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * @class_interfaces - list of class_interfaces associated
 * @glue_dirs - "glue" directory to put in-between the parent device to
 *              avoid namespace conflicts
 * @class_mutex - mutex to protect the children, devices, and interfaces lists.
=======
 * @glue_dirs - "glue" directory to put in-between the parent device to
 *              avoid namespace conflicts
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * @glue_dirs - "glue" directory to put in-between the parent device to
 *              avoid namespace conflicts
>>>>>>> refs/remotes/origin/master
 * @class - pointer back to the struct class that this structure is associated
 *          with.
 *
 * This structure is the one that is the actual kobject allowing struct
 * bus_type/class to be statically allocated safely.  Nothing outside of the
 * driver core should ever touch these fields.
 */
struct subsys_private {
	struct kset subsys;
	struct kset *devices_kset;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct list_head interfaces;
	struct mutex mutex;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct list_head interfaces;
	struct mutex mutex;
>>>>>>> refs/remotes/origin/master

	struct kset *drivers_kset;
	struct klist klist_devices;
	struct klist klist_drivers;
	struct blocking_notifier_head bus_notifier;
	unsigned int drivers_autoprobe:1;
	struct bus_type *bus;

<<<<<<< HEAD
<<<<<<< HEAD
	struct list_head class_interfaces;
	struct kset glue_dirs;
	struct mutex class_mutex;
=======
	struct kset glue_dirs;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct kset glue_dirs;
>>>>>>> refs/remotes/origin/master
	struct class *class;
};
#define to_subsys_private(obj) container_of(obj, struct subsys_private, subsys.kobj)

struct driver_private {
	struct kobject kobj;
	struct klist klist_devices;
	struct klist_node knode_bus;
	struct module_kobject *mkobj;
	struct device_driver *driver;
};
#define to_driver(obj) container_of(obj, struct driver_private, kobj)

/**
 * struct device_private - structure to hold the private to the driver core portions of the device structure.
 *
 * @klist_children - klist containing all children of this device
 * @knode_parent - node in sibling list
 * @knode_driver - node in driver list
 * @knode_bus - node in bus list
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
 * @deferred_probe - entry in deferred_probe_list which is used to retry the
 *	binding of drivers which were unable to get all the resources needed by
 *	the device; typically because it depends on another driver getting
 *	probed first.
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 * @driver_data - private pointer for driver specific info.  Will turn into a
 * list soon.
 * @device - pointer back to the struct class that this structure is
 * associated with.
 *
 * Nothing outside of the driver core should ever touch these fields.
 */
struct device_private {
	struct klist klist_children;
	struct klist_node knode_parent;
	struct klist_node knode_driver;
	struct klist_node knode_bus;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct list_head deferred_probe;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct list_head deferred_probe;
>>>>>>> refs/remotes/origin/master
	void *driver_data;
	struct device *device;
};
#define to_device_private_parent(obj)	\
	container_of(obj, struct device_private, knode_parent)
#define to_device_private_driver(obj)	\
	container_of(obj, struct device_private, knode_driver)
#define to_device_private_bus(obj)	\
	container_of(obj, struct device_private, knode_bus)

extern int device_private_init(struct device *dev);

/* initialisation functions */
extern int devices_init(void);
extern int buses_init(void);
extern int classes_init(void);
extern int firmware_init(void);
#ifdef CONFIG_SYS_HYPERVISOR
extern int hypervisor_init(void);
#else
static inline int hypervisor_init(void) { return 0; }
#endif
extern int platform_bus_init(void);
<<<<<<< HEAD
extern int system_bus_init(void);
<<<<<<< HEAD
extern int cpu_dev_init(void);
=======
extern void cpu_dev_init(void);
>>>>>>> refs/remotes/origin/cm-10.0
=======
extern void cpu_dev_init(void);

struct kobject *virtual_device_parent(struct device *dev);
>>>>>>> refs/remotes/origin/master

extern int bus_add_device(struct device *dev);
extern void bus_probe_device(struct device *dev);
extern void bus_remove_device(struct device *dev);

extern int bus_add_driver(struct device_driver *drv);
extern void bus_remove_driver(struct device_driver *drv);

extern void driver_detach(struct device_driver *drv);
extern int driver_probe_device(struct device_driver *drv, struct device *dev);
<<<<<<< HEAD
<<<<<<< HEAD
=======
extern void driver_deferred_probe_del(struct device *dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
extern void driver_deferred_probe_del(struct device *dev);
>>>>>>> refs/remotes/origin/master
static inline int driver_match_device(struct device_driver *drv,
				      struct device *dev)
{
	return drv->bus->match ? drv->bus->match(dev, drv) : 1;
}

<<<<<<< HEAD
=======
extern int driver_add_groups(struct device_driver *drv,
			     const struct attribute_group **groups);
extern void driver_remove_groups(struct device_driver *drv,
				 const struct attribute_group **groups);

extern int device_add_groups(struct device *dev,
			     const struct attribute_group **groups);
extern void device_remove_groups(struct device *dev,
				 const struct attribute_group **groups);

>>>>>>> refs/remotes/origin/master
extern char *make_class_name(const char *name, struct kobject *kobj);

extern int devres_release_all(struct device *dev);

<<<<<<< HEAD
<<<<<<< HEAD
=======
/* /sys/devices directory */
>>>>>>> refs/remotes/origin/cm-10.0
=======
/* /sys/devices directory */
>>>>>>> refs/remotes/origin/master
extern struct kset *devices_kset;

#if defined(CONFIG_MODULES) && defined(CONFIG_SYSFS)
extern void module_add_driver(struct module *mod, struct device_driver *drv);
extern void module_remove_driver(struct device_driver *drv);
#else
static inline void module_add_driver(struct module *mod,
				     struct device_driver *drv) { }
static inline void module_remove_driver(struct device_driver *drv) { }
#endif

#ifdef CONFIG_DEVTMPFS
extern int devtmpfs_init(void);
#else
static inline int devtmpfs_init(void) { return 0; }
#endif
