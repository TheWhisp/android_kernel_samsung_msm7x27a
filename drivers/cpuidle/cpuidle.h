/*
 * cpuidle.h - The internal header file
 */

#ifndef __DRIVER_CPUIDLE_H
#define __DRIVER_CPUIDLE_H

<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/sysdev.h>
=======
#include <linux/device.h>
>>>>>>> refs/remotes/origin/cm-10.0

=======
>>>>>>> refs/remotes/origin/master
/* For internal use only */
extern struct cpuidle_governor *cpuidle_curr_governor;
extern struct list_head cpuidle_governors;
extern struct list_head cpuidle_detected_devices;
extern struct mutex cpuidle_lock;
extern spinlock_t cpuidle_driver_lock;
<<<<<<< HEAD
<<<<<<< HEAD
=======
extern int cpuidle_disabled(void);
>>>>>>> refs/remotes/origin/cm-10.0
=======
extern int cpuidle_disabled(void);
extern int cpuidle_enter_state(struct cpuidle_device *dev,
		struct cpuidle_driver *drv, int next_state);
>>>>>>> refs/remotes/origin/master

/* idle loop */
extern void cpuidle_install_idle_handler(void);
extern void cpuidle_uninstall_idle_handler(void);

/* governors */
extern int cpuidle_switch_governor(struct cpuidle_governor *gov);

/* sysfs */
<<<<<<< HEAD
<<<<<<< HEAD
extern int cpuidle_add_class_sysfs(struct sysdev_class *cls);
extern void cpuidle_remove_class_sysfs(struct sysdev_class *cls);
extern int cpuidle_add_state_sysfs(struct cpuidle_device *device);
extern void cpuidle_remove_state_sysfs(struct cpuidle_device *device);
extern int cpuidle_add_sysfs(struct sys_device *sysdev);
extern void cpuidle_remove_sysfs(struct sys_device *sysdev);
=======
extern int cpuidle_add_interface(struct device *dev);
extern void cpuidle_remove_interface(struct device *dev);
extern int cpuidle_add_state_sysfs(struct cpuidle_device *device);
extern void cpuidle_remove_state_sysfs(struct cpuidle_device *device);
extern int cpuidle_add_sysfs(struct device *dev);
extern void cpuidle_remove_sysfs(struct device *dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======

struct device;

extern int cpuidle_add_interface(struct device *dev);
extern void cpuidle_remove_interface(struct device *dev);
extern int cpuidle_add_device_sysfs(struct cpuidle_device *device);
extern void cpuidle_remove_device_sysfs(struct cpuidle_device *device);
extern int cpuidle_add_sysfs(struct cpuidle_device *dev);
extern void cpuidle_remove_sysfs(struct cpuidle_device *dev);

#ifdef CONFIG_ARCH_NEEDS_CPU_IDLE_COUPLED
bool cpuidle_state_is_coupled(struct cpuidle_device *dev,
		struct cpuidle_driver *drv, int state);
int cpuidle_enter_state_coupled(struct cpuidle_device *dev,
		struct cpuidle_driver *drv, int next_state);
int cpuidle_coupled_register_device(struct cpuidle_device *dev);
void cpuidle_coupled_unregister_device(struct cpuidle_device *dev);
#else
static inline bool cpuidle_state_is_coupled(struct cpuidle_device *dev,
		struct cpuidle_driver *drv, int state)
{
	return false;
}

static inline int cpuidle_enter_state_coupled(struct cpuidle_device *dev,
		struct cpuidle_driver *drv, int next_state)
{
	return -1;
}

static inline int cpuidle_coupled_register_device(struct cpuidle_device *dev)
{
	return 0;
}

static inline void cpuidle_coupled_unregister_device(struct cpuidle_device *dev)
{
}
#endif
>>>>>>> refs/remotes/origin/master

#endif /* __DRIVER_CPUIDLE_H */
