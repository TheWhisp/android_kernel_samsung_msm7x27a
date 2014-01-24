#ifndef _NFS_FS_I
#define _NFS_FS_I

<<<<<<< HEAD
#include <asm/types.h>
#include <linux/list.h>
#include <linux/nfs.h>

=======
>>>>>>> refs/remotes/origin/cm-10.0
struct nlm_lockowner;

/*
 * NFS lock info
 */
struct nfs_lock_info {
	u32		state;
	struct nlm_lockowner *owner;
	struct list_head list;
};

struct nfs4_lock_state;
struct nfs4_lock_info {
	struct nfs4_lock_state *owner;
};

#endif
