#ifndef _LINUX_OF_DEVICE_H
#define _LINUX_OF_DEVICE_H

<<<<<<< HEAD
#include <linux/platform_device.h>
#include <linux/of_platform.h> /* temporary until merge */

#ifdef CONFIG_OF_DEVICE
<<<<<<< HEAD
#include <linux/device.h>
#include <linux/of.h>
#include <linux/mod_devicetable.h>

=======
=======
#include <linux/cpu.h>
#include <linux/platform_device.h>
#include <linux/of_platform.h> /* temporary until merge */

>>>>>>> refs/remotes/origin/master
#include <linux/of.h>
#include <linux/mod_devicetable.h>

struct device;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
#ifdef CONFIG_OF
>>>>>>> refs/remotes/origin/master
extern const struct of_device_id *of_match_device(
	const struct of_device_id *matches, const struct device *dev);
extern void of_device_make_bus_id(struct device *dev);

/**
 * of_driver_match_device - Tell if a driver's of_match_table matches a device.
 * @drv: the device_driver structure to test
 * @dev: the device structure to match against
 */
static inline int of_driver_match_device(struct device *dev,
					 const struct device_driver *drv)
{
	return of_match_device(drv->of_match_table, dev) != NULL;
}

extern struct platform_device *of_dev_get(struct platform_device *dev);
extern void of_dev_put(struct platform_device *dev);

extern int of_device_add(struct platform_device *pdev);
extern int of_device_register(struct platform_device *ofdev);
extern void of_device_unregister(struct platform_device *ofdev);

extern ssize_t of_device_get_modalias(struct device *dev,
					char *str, ssize_t len);

<<<<<<< HEAD
<<<<<<< HEAD
extern int of_device_uevent(struct device *dev, struct kobj_uevent_env *env);
=======
extern void of_device_uevent(struct device *dev, struct kobj_uevent_env *env);
extern int of_device_uevent_modalias(struct device *dev, struct kobj_uevent_env *env);
>>>>>>> refs/remotes/origin/cm-10.0
=======
extern void of_device_uevent(struct device *dev, struct kobj_uevent_env *env);
extern int of_device_uevent_modalias(struct device *dev, struct kobj_uevent_env *env);
>>>>>>> refs/remotes/origin/master

static inline void of_device_node_put(struct device *dev)
{
	of_node_put(dev->of_node);
}

<<<<<<< HEAD
#else /* CONFIG_OF_DEVICE */
=======
static inline struct device_node *of_cpu_device_node_get(int cpu)
{
	struct device *cpu_dev;
	cpu_dev = get_cpu_device(cpu);
	if (!cpu_dev)
		return NULL;
	return of_node_get(cpu_dev->of_node);
}

#else /* CONFIG_OF */
>>>>>>> refs/remotes/origin/master

static inline int of_driver_match_device(struct device *dev,
					 struct device_driver *drv)
{
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline int of_device_uevent(struct device *dev,
=======
=======
>>>>>>> refs/remotes/origin/master
static inline void of_device_uevent(struct device *dev,
			struct kobj_uevent_env *env) { }

static inline int of_device_uevent_modalias(struct device *dev,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				   struct kobj_uevent_env *env)
{
	return -ENODEV;
}

static inline void of_device_node_put(struct device *dev) { }

static inline const struct of_device_id *of_match_device(
		const struct of_device_id *matches, const struct device *dev)
{
	return NULL;
}
<<<<<<< HEAD
#endif /* CONFIG_OF_DEVICE */
=======

static inline struct device_node *of_cpu_device_node_get(int cpu)
{
	return NULL;
}
#endif /* CONFIG_OF */
>>>>>>> refs/remotes/origin/master

#endif /* _LINUX_OF_DEVICE_H */
