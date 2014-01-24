#ifndef __LOCKD_NETNS_H__
#define __LOCKD_NETNS_H__

<<<<<<< HEAD
=======
#include <linux/fs.h>
>>>>>>> refs/remotes/origin/master
#include <net/netns/generic.h>

struct lockd_net {
	unsigned int nlmsvc_users;
<<<<<<< HEAD
=======
	unsigned long next_gc;
	unsigned long nrhosts;

	struct delayed_work grace_period_end;
	struct lock_manager lockd_manager;
	struct list_head grace_list;

	spinlock_t nsm_clnt_lock;
	unsigned int nsm_users;
	struct rpc_clnt *nsm_clnt;
>>>>>>> refs/remotes/origin/master
};

extern int lockd_net_id;

#endif
