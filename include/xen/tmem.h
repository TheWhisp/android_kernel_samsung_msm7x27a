#ifndef _XEN_TMEM_H
#define _XEN_TMEM_H

#include <linux/types.h>

<<<<<<< HEAD
/* defined in drivers/xen/tmem.c */
extern bool tmem_enabled;
=======
#ifdef CONFIG_XEN_TMEM_MODULE
#define tmem_enabled true
#else
/* defined in drivers/xen/tmem.c */
extern bool tmem_enabled;
#endif

#ifdef CONFIG_XEN_SELFBALLOONING
extern int xen_selfballoon_init(bool, bool);
#endif
>>>>>>> refs/remotes/origin/master

#endif /* _XEN_TMEM_H */
