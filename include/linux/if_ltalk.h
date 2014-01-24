#ifndef __LINUX_LTALK_H
#define __LINUX_LTALK_H

<<<<<<< HEAD
#define LTALK_HLEN		1
#define LTALK_MTU		600
#define LTALK_ALEN		1

#ifdef __KERNEL__
extern struct net_device *alloc_ltalkdev(int sizeof_priv);
#endif

#endif
=======
#include <uapi/linux/if_ltalk.h>

extern struct net_device *alloc_ltalkdev(int sizeof_priv);
#endif
>>>>>>> refs/remotes/origin/master
