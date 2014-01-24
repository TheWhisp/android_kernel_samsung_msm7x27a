/*
 * include/linux/node.h - generic node definition
 *
 * This is mainly for topological representation. We define the 
 * basic 'struct node' here, which can be embedded in per-arch 
 * definitions of processors.
 *
 * Basic handling of the devices is done in drivers/base/node.c
 * and system devices are handled in drivers/base/sys.c. 
 *
 * Nodes are exported via driverfs in the class/node/devices/
 * directory. 
 */
#ifndef _LINUX_NODE_H_
#define _LINUX_NODE_H_

<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/sysdev.h>
=======
#include <linux/device.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/device.h>
>>>>>>> refs/remotes/origin/master
#include <linux/cpumask.h>
#include <linux/workqueue.h>

struct node {
<<<<<<< HEAD
<<<<<<< HEAD
	struct sys_device	sysdev;
=======
	struct device	dev;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct device	dev;
>>>>>>> refs/remotes/origin/master

#if defined(CONFIG_MEMORY_HOTPLUG_SPARSE) && defined(CONFIG_HUGETLBFS)
	struct work_struct	node_work;
#endif
};

struct memory_block;
<<<<<<< HEAD
extern struct node node_devices[];
typedef  void (*node_registration_func_t)(struct node *);

extern int register_node(struct node *, int, struct node *);
=======
extern struct node *node_devices[];
typedef  void (*node_registration_func_t)(struct node *);

>>>>>>> refs/remotes/origin/master
extern void unregister_node(struct node *node);
#ifdef CONFIG_NUMA
extern int register_one_node(int nid);
extern void unregister_one_node(int nid);
extern int register_cpu_under_node(unsigned int cpu, unsigned int nid);
extern int unregister_cpu_under_node(unsigned int cpu, unsigned int nid);
extern int register_mem_sect_under_node(struct memory_block *mem_blk,
						int nid);
extern int unregister_mem_sect_under_nodes(struct memory_block *mem_blk,
					   unsigned long phys_index);

#ifdef CONFIG_HUGETLBFS
extern void register_hugetlbfs_with_node(node_registration_func_t doregister,
					 node_registration_func_t unregister);
#endif
#else
static inline int register_one_node(int nid)
{
	return 0;
}
static inline int unregister_one_node(int nid)
{
	return 0;
}
static inline int register_cpu_under_node(unsigned int cpu, unsigned int nid)
{
	return 0;
}
static inline int unregister_cpu_under_node(unsigned int cpu, unsigned int nid)
{
	return 0;
}
static inline int register_mem_sect_under_node(struct memory_block *mem_blk,
							int nid)
{
	return 0;
}
static inline int unregister_mem_sect_under_nodes(struct memory_block *mem_blk,
						  unsigned long phys_index)
{
	return 0;
}

static inline void register_hugetlbfs_with_node(node_registration_func_t reg,
						node_registration_func_t unreg)
{
}
#endif

<<<<<<< HEAD
<<<<<<< HEAD
#define to_node(sys_device) container_of(sys_device, struct node, sysdev)
=======
#define to_node(device) container_of(device, struct node, dev)
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define to_node(device) container_of(device, struct node, dev)
>>>>>>> refs/remotes/origin/master

#endif /* _LINUX_NODE_H_ */
