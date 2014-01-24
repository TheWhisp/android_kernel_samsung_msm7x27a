#ifndef _RAID0_H
#define _RAID0_H

<<<<<<< HEAD
struct strip_zone
{
	sector_t zone_end;	/* Start of the next zone (in sectors) */
	sector_t dev_start;	/* Zone offset in real dev (in sectors) */
	int nb_dev;		/* # of devices attached to the zone */
};

struct raid0_private_data
{
	struct strip_zone *strip_zone;
	mdk_rdev_t **devlist; /* lists of rdevs, pointed to by strip_zone->dev */
	int nr_strip_zones;
};

typedef struct raid0_private_data raid0_conf_t;

=======
struct strip_zone {
	sector_t zone_end;	/* Start of the next zone (in sectors) */
	sector_t dev_start;	/* Zone offset in real dev (in sectors) */
	int	 nb_dev;	/* # of devices attached to the zone */
};

struct r0conf {
	struct strip_zone	*strip_zone;
	struct md_rdev		**devlist; /* lists of rdevs, pointed to
					    * by strip_zone->dev */
	int			nr_strip_zones;
	int			has_merge_bvec;	/* at least one member has
						 * a merge_bvec_fn */
};

>>>>>>> refs/remotes/origin/cm-10.0
#endif
