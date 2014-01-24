#ifndef _FS_CEPH_MSGPOOL
#define _FS_CEPH_MSGPOOL

#include <linux/mempool.h>
<<<<<<< HEAD
#include "messenger.h"
=======
#include <linux/ceph/messenger.h>
>>>>>>> refs/remotes/origin/master

/*
 * we use memory pools for preallocating messages we may receive, to
 * avoid unexpected OOM conditions.
 */
struct ceph_msgpool {
	const char *name;
	mempool_t *pool;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	int front_len;          /* preallocated payload size */
};

extern int ceph_msgpool_init(struct ceph_msgpool *pool,
=======
=======
>>>>>>> refs/remotes/origin/master
	int type;               /* preallocated message type */
	int front_len;          /* preallocated payload size */
};

extern int ceph_msgpool_init(struct ceph_msgpool *pool, int type,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
	int type;               /* preallocated message type */
	int front_len;          /* preallocated payload size */
};

extern int ceph_msgpool_init(struct ceph_msgpool *pool, int type,
>>>>>>> refs/remotes/origin/cm-11.0
			     int front_len, int size, bool blocking,
			     const char *name);
extern void ceph_msgpool_destroy(struct ceph_msgpool *pool);
extern struct ceph_msg *ceph_msgpool_get(struct ceph_msgpool *,
					 int front_len);
extern void ceph_msgpool_put(struct ceph_msgpool *, struct ceph_msg *);

#endif
