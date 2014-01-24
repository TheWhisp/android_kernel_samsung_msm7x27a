#ifndef _LINEAR_H
#define _LINEAR_H

struct dev_info {
<<<<<<< HEAD
<<<<<<< HEAD
	mdk_rdev_t	*rdev;
	sector_t	end_sector;
};

typedef struct dev_info dev_info_t;

struct linear_private_data
{
	struct rcu_head		rcu;
	sector_t		array_sectors;
	dev_info_t		disks[0];
};


typedef struct linear_private_data linear_conf_t;

=======
=======
>>>>>>> refs/remotes/origin/master
	struct md_rdev	*rdev;
	sector_t	end_sector;
};

struct linear_conf
{
	struct rcu_head		rcu;
	sector_t		array_sectors;
	struct dev_info		disks[0];
};
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#endif
