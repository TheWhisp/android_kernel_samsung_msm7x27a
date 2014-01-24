#ifndef _MULTIPATH_H
#define _MULTIPATH_H

struct multipath_info {
<<<<<<< HEAD
	mdk_rdev_t	*rdev;
};

struct multipath_private_data {
	mddev_t			*mddev;
=======
	struct md_rdev	*rdev;
};

struct mpconf {
	struct mddev			*mddev;
>>>>>>> refs/remotes/origin/cm-10.0
	struct multipath_info	*multipaths;
	int			raid_disks;
	spinlock_t		device_lock;
	struct list_head	retry_list;

	mempool_t		*pool;
};

<<<<<<< HEAD
typedef struct multipath_private_data multipath_conf_t;

=======
>>>>>>> refs/remotes/origin/cm-10.0
/*
 * this is our 'private' 'collective' MULTIPATH buffer head.
 * it contains information about what kind of IO operations were started
 * for this MULTIPATH operation, and about their status:
 */

struct multipath_bh {
<<<<<<< HEAD
	mddev_t			*mddev;
=======
	struct mddev			*mddev;
>>>>>>> refs/remotes/origin/cm-10.0
	struct bio		*master_bio;
	struct bio		bio;
	int			path;
	struct list_head	retry_list;
};
#endif
