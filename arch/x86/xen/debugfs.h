#ifndef _XEN_DEBUGFS_H
#define _XEN_DEBUGFS_H

struct dentry * __init xen_init_debugfs(void);

<<<<<<< HEAD
<<<<<<< HEAD
struct dentry *xen_debugfs_create_u32_array(const char *name, mode_t mode,
=======
struct dentry *xen_debugfs_create_u32_array(const char *name, umode_t mode,
>>>>>>> refs/remotes/origin/cm-10.0
					    struct dentry *parent,
					    u32 *array, unsigned elements);

=======
>>>>>>> refs/remotes/origin/master
#endif /* _XEN_DEBUGFS_H */
