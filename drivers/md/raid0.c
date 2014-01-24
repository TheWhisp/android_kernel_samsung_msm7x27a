/*
   raid0.c : Multiple Devices driver for Linux
             Copyright (C) 1994-96 Marc ZYNGIER
	     <zyngier@ufr-info-p7.ibp.fr> or
	     <maz@gloups.fdn.fr>
             Copyright (C) 1999, 2000 Ingo Molnar, Red Hat


   RAID-0 management functions.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   You should have received a copy of the GNU General Public License
   (for example /usr/src/linux/COPYING); if not, write to the Free
   Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  
*/

#include <linux/blkdev.h>
#include <linux/seq_file.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/master
#include <linux/slab.h>
#include "md.h"
#include "raid0.h"
#include "raid5.h"

static int raid0_congested(void *data, int bits)
{
<<<<<<< HEAD
<<<<<<< HEAD
	mddev_t *mddev = data;
	raid0_conf_t *conf = mddev->private;
	mdk_rdev_t **devlist = conf->devlist;
=======
	struct mddev *mddev = data;
	struct r0conf *conf = mddev->private;
	struct md_rdev **devlist = conf->devlist;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct mddev *mddev = data;
	struct r0conf *conf = mddev->private;
	struct md_rdev **devlist = conf->devlist;
>>>>>>> refs/remotes/origin/master
	int raid_disks = conf->strip_zone[0].nb_dev;
	int i, ret = 0;

	if (mddev_congested(mddev, bits))
		return 1;

	for (i = 0; i < raid_disks && !ret ; i++) {
		struct request_queue *q = bdev_get_queue(devlist[i]->bdev);

		ret |= bdi_congested(&q->backing_dev_info, bits);
	}
	return ret;
}

/*
 * inform the user of the raid configuration
*/
<<<<<<< HEAD
<<<<<<< HEAD
static void dump_zones(mddev_t *mddev)
{
	int j, k, h;
	sector_t zone_size = 0;
	sector_t zone_start = 0;
	char b[BDEVNAME_SIZE];
	raid0_conf_t *conf = mddev->private;
	int raid_disks = conf->strip_zone[0].nb_dev;
	printk(KERN_INFO "******* %s configuration *********\n",
		mdname(mddev));
	h = 0;
	for (j = 0; j < conf->nr_strip_zones; j++) {
		printk(KERN_INFO "zone%d=[", j);
		for (k = 0; k < conf->strip_zone[j].nb_dev; k++)
			printk(KERN_CONT "%s/",
=======
=======
>>>>>>> refs/remotes/origin/master
static void dump_zones(struct mddev *mddev)
{
	int j, k;
	sector_t zone_size = 0;
	sector_t zone_start = 0;
	char b[BDEVNAME_SIZE];
	struct r0conf *conf = mddev->private;
	int raid_disks = conf->strip_zone[0].nb_dev;
	printk(KERN_INFO "md: RAID0 configuration for %s - %d zone%s\n",
	       mdname(mddev),
	       conf->nr_strip_zones, conf->nr_strip_zones==1?"":"s");
	for (j = 0; j < conf->nr_strip_zones; j++) {
		printk(KERN_INFO "md: zone%d=[", j);
		for (k = 0; k < conf->strip_zone[j].nb_dev; k++)
			printk(KERN_CONT "%s%s", k?"/":"",
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			bdevname(conf->devlist[j*raid_disks
						+ k]->bdev, b));
		printk(KERN_CONT "]\n");

		zone_size  = conf->strip_zone[j].zone_end - zone_start;
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_INFO "        zone offset=%llukb "
				"device offset=%llukb size=%llukb\n",
=======
		printk(KERN_INFO "      zone-offset=%10lluKB, "
				"device-offset=%10lluKB, size=%10lluKB\n",
>>>>>>> refs/remotes/origin/cm-10.0
=======
		printk(KERN_INFO "      zone-offset=%10lluKB, "
				"device-offset=%10lluKB, size=%10lluKB\n",
>>>>>>> refs/remotes/origin/master
			(unsigned long long)zone_start>>1,
			(unsigned long long)conf->strip_zone[j].dev_start>>1,
			(unsigned long long)zone_size>>1);
		zone_start = conf->strip_zone[j].zone_end;
	}
<<<<<<< HEAD
<<<<<<< HEAD
	printk(KERN_INFO "**********************************\n\n");
}

static int create_strip_zones(mddev_t *mddev, raid0_conf_t **private_conf)
{
	int i, c, err;
	sector_t curr_zone_end, sectors;
	mdk_rdev_t *smallest, *rdev1, *rdev2, *rdev, **dev;
	struct strip_zone *zone;
	int cnt;
	char b[BDEVNAME_SIZE];
	raid0_conf_t *conf = kzalloc(sizeof(*conf), GFP_KERNEL);

	if (!conf)
		return -ENOMEM;
	list_for_each_entry(rdev1, &mddev->disks, same_set) {
		printk(KERN_INFO "md/raid0:%s: looking at %s\n",
		       mdname(mddev),
		       bdevname(rdev1->bdev, b));
=======
=======
>>>>>>> refs/remotes/origin/master
	printk(KERN_INFO "\n");
}

static int create_strip_zones(struct mddev *mddev, struct r0conf **private_conf)
{
	int i, c, err;
	sector_t curr_zone_end, sectors;
	struct md_rdev *smallest, *rdev1, *rdev2, *rdev, **dev;
	struct strip_zone *zone;
	int cnt;
	char b[BDEVNAME_SIZE];
	char b2[BDEVNAME_SIZE];
	struct r0conf *conf = kzalloc(sizeof(*conf), GFP_KERNEL);
<<<<<<< HEAD
=======
	bool discard_supported = false;
>>>>>>> refs/remotes/origin/master

	if (!conf)
		return -ENOMEM;
	rdev_for_each(rdev1, mddev) {
		pr_debug("md/raid0:%s: looking at %s\n",
			 mdname(mddev),
			 bdevname(rdev1->bdev, b));
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		c = 0;

		/* round size to chunk_size */
		sectors = rdev1->sectors;
		sector_div(sectors, mddev->chunk_sectors);
		rdev1->sectors = sectors * mddev->chunk_sectors;

<<<<<<< HEAD
<<<<<<< HEAD
		list_for_each_entry(rdev2, &mddev->disks, same_set) {
			printk(KERN_INFO "md/raid0:%s:   comparing %s(%llu)",
			       mdname(mddev),
			       bdevname(rdev1->bdev,b),
			       (unsigned long long)rdev1->sectors);
			printk(KERN_CONT " with %s(%llu)\n",
			       bdevname(rdev2->bdev,b),
			       (unsigned long long)rdev2->sectors);
			if (rdev2 == rdev1) {
				printk(KERN_INFO "md/raid0:%s:   END\n",
				       mdname(mddev));
=======
=======
>>>>>>> refs/remotes/origin/master
		rdev_for_each(rdev2, mddev) {
			pr_debug("md/raid0:%s:   comparing %s(%llu)"
				 " with %s(%llu)\n",
				 mdname(mddev),
				 bdevname(rdev1->bdev,b),
				 (unsigned long long)rdev1->sectors,
				 bdevname(rdev2->bdev,b2),
				 (unsigned long long)rdev2->sectors);
			if (rdev2 == rdev1) {
				pr_debug("md/raid0:%s:   END\n",
					 mdname(mddev));
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				break;
			}
			if (rdev2->sectors == rdev1->sectors) {
				/*
				 * Not unique, don't count it as a new
				 * group
				 */
<<<<<<< HEAD
<<<<<<< HEAD
				printk(KERN_INFO "md/raid0:%s:   EQUAL\n",
				       mdname(mddev));
				c = 1;
				break;
			}
			printk(KERN_INFO "md/raid0:%s:   NOT EQUAL\n",
			       mdname(mddev));
		}
		if (!c) {
			printk(KERN_INFO "md/raid0:%s:   ==> UNIQUE\n",
			       mdname(mddev));
			conf->nr_strip_zones++;
			printk(KERN_INFO "md/raid0:%s: %d zones\n",
			       mdname(mddev), conf->nr_strip_zones);
		}
	}
	printk(KERN_INFO "md/raid0:%s: FINAL %d zones\n",
	       mdname(mddev), conf->nr_strip_zones);
=======
=======
>>>>>>> refs/remotes/origin/master
				pr_debug("md/raid0:%s:   EQUAL\n",
					 mdname(mddev));
				c = 1;
				break;
			}
			pr_debug("md/raid0:%s:   NOT EQUAL\n",
				 mdname(mddev));
		}
		if (!c) {
			pr_debug("md/raid0:%s:   ==> UNIQUE\n",
				 mdname(mddev));
			conf->nr_strip_zones++;
			pr_debug("md/raid0:%s: %d zones\n",
				 mdname(mddev), conf->nr_strip_zones);
		}
	}
	pr_debug("md/raid0:%s: FINAL %d zones\n",
		 mdname(mddev), conf->nr_strip_zones);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	err = -ENOMEM;
	conf->strip_zone = kzalloc(sizeof(struct strip_zone)*
				conf->nr_strip_zones, GFP_KERNEL);
	if (!conf->strip_zone)
		goto abort;
<<<<<<< HEAD
<<<<<<< HEAD
	conf->devlist = kzalloc(sizeof(mdk_rdev_t*)*
=======
	conf->devlist = kzalloc(sizeof(struct md_rdev*)*
>>>>>>> refs/remotes/origin/cm-10.0
=======
	conf->devlist = kzalloc(sizeof(struct md_rdev*)*
>>>>>>> refs/remotes/origin/master
				conf->nr_strip_zones*mddev->raid_disks,
				GFP_KERNEL);
	if (!conf->devlist)
		goto abort;

	/* The first zone must contain all devices, so here we check that
	 * there is a proper alignment of slots to devices and find them all
	 */
	zone = &conf->strip_zone[0];
	cnt = 0;
	smallest = NULL;
	dev = conf->devlist;
	err = -EINVAL;
<<<<<<< HEAD
<<<<<<< HEAD
	list_for_each_entry(rdev1, &mddev->disks, same_set) {
=======
	rdev_for_each(rdev1, mddev) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	rdev_for_each(rdev1, mddev) {
>>>>>>> refs/remotes/origin/master
		int j = rdev1->raid_disk;

		if (mddev->level == 10) {
			/* taking over a raid10-n2 array */
			j /= 2;
			rdev1->new_raid_disk = j;
		}

		if (mddev->level == 1) {
			/* taiking over a raid1 array-
			 * we have only one active disk
			 */
			j = 0;
			rdev1->new_raid_disk = j;
		}

<<<<<<< HEAD
		if (j < 0 || j >= mddev->raid_disks) {
=======
		if (j < 0) {
			printk(KERN_ERR
			       "md/raid0:%s: remove inactive devices before converting to RAID0\n",
			       mdname(mddev));
			goto abort;
		}
		if (j >= mddev->raid_disks) {
>>>>>>> refs/remotes/origin/master
			printk(KERN_ERR "md/raid0:%s: bad disk number %d - "
			       "aborting!\n", mdname(mddev), j);
			goto abort;
		}
		if (dev[j]) {
			printk(KERN_ERR "md/raid0:%s: multiple devices for %d - "
			       "aborting!\n", mdname(mddev), j);
			goto abort;
		}
		dev[j] = rdev1;

		disk_stack_limits(mddev->gendisk, rdev1->bdev,
				  rdev1->data_offset << 9);
<<<<<<< HEAD
<<<<<<< HEAD
		/* as we don't honour merge_bvec_fn, we must never risk
		 * violating it, so limit ->max_segments to 1, lying within
		 * a single page.
		 */

		if (rdev1->bdev->bd_disk->queue->merge_bvec_fn) {
			blk_queue_max_segments(mddev->queue, 1);
			blk_queue_segment_boundary(mddev->queue,
						   PAGE_CACHE_SIZE - 1);
		}
=======
=======
>>>>>>> refs/remotes/origin/master

		if (rdev1->bdev->bd_disk->queue->merge_bvec_fn)
			conf->has_merge_bvec = 1;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		if (!smallest || (rdev1->sectors < smallest->sectors))
			smallest = rdev1;
		cnt++;
=======
		if (!smallest || (rdev1->sectors < smallest->sectors))
			smallest = rdev1;
		cnt++;

		if (blk_queue_discard(bdev_get_queue(rdev1->bdev)))
			discard_supported = true;
>>>>>>> refs/remotes/origin/master
	}
	if (cnt != mddev->raid_disks) {
		printk(KERN_ERR "md/raid0:%s: too few disks (%d of %d) - "
		       "aborting!\n", mdname(mddev), cnt, mddev->raid_disks);
		goto abort;
	}
	zone->nb_dev = cnt;
	zone->zone_end = smallest->sectors * cnt;

	curr_zone_end = zone->zone_end;

	/* now do the other zones */
	for (i = 1; i < conf->nr_strip_zones; i++)
	{
		int j;

		zone = conf->strip_zone + i;
		dev = conf->devlist + i * mddev->raid_disks;

<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_INFO "md/raid0:%s: zone %d\n",
		       mdname(mddev), i);
=======
		pr_debug("md/raid0:%s: zone %d\n", mdname(mddev), i);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_debug("md/raid0:%s: zone %d\n", mdname(mddev), i);
>>>>>>> refs/remotes/origin/master
		zone->dev_start = smallest->sectors;
		smallest = NULL;
		c = 0;

		for (j=0; j<cnt; j++) {
			rdev = conf->devlist[j];
<<<<<<< HEAD
<<<<<<< HEAD
			printk(KERN_INFO "md/raid0:%s: checking %s ...",
			       mdname(mddev),
			       bdevname(rdev->bdev, b));
			if (rdev->sectors <= zone->dev_start) {
				printk(KERN_CONT " nope.\n");
				continue;
			}
			printk(KERN_CONT " contained as device %d\n", c);
=======
=======
>>>>>>> refs/remotes/origin/master
			if (rdev->sectors <= zone->dev_start) {
				pr_debug("md/raid0:%s: checking %s ... nope\n",
					 mdname(mddev),
					 bdevname(rdev->bdev, b));
				continue;
			}
			pr_debug("md/raid0:%s: checking %s ..."
				 " contained as device %d\n",
				 mdname(mddev),
				 bdevname(rdev->bdev, b), c);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			dev[c] = rdev;
			c++;
			if (!smallest || rdev->sectors < smallest->sectors) {
				smallest = rdev;
<<<<<<< HEAD
<<<<<<< HEAD
				printk(KERN_INFO "md/raid0:%s:  (%llu) is smallest!.\n",
				       mdname(mddev),
				       (unsigned long long)rdev->sectors);
=======
				pr_debug("md/raid0:%s:  (%llu) is smallest!.\n",
					 mdname(mddev),
					 (unsigned long long)rdev->sectors);
>>>>>>> refs/remotes/origin/cm-10.0
=======
				pr_debug("md/raid0:%s:  (%llu) is smallest!.\n",
					 mdname(mddev),
					 (unsigned long long)rdev->sectors);
>>>>>>> refs/remotes/origin/master
			}
		}

		zone->nb_dev = c;
		sectors = (smallest->sectors - zone->dev_start) * c;
<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_INFO "md/raid0:%s: zone->nb_dev: %d, sectors: %llu\n",
		       mdname(mddev),
		       zone->nb_dev, (unsigned long long)sectors);
=======
		pr_debug("md/raid0:%s: zone->nb_dev: %d, sectors: %llu\n",
			 mdname(mddev),
			 zone->nb_dev, (unsigned long long)sectors);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_debug("md/raid0:%s: zone->nb_dev: %d, sectors: %llu\n",
			 mdname(mddev),
			 zone->nb_dev, (unsigned long long)sectors);
>>>>>>> refs/remotes/origin/master

		curr_zone_end += sectors;
		zone->zone_end = curr_zone_end;

<<<<<<< HEAD
<<<<<<< HEAD
		printk(KERN_INFO "md/raid0:%s: current zone start: %llu\n",
		       mdname(mddev),
		       (unsigned long long)smallest->sectors);
=======
		pr_debug("md/raid0:%s: current zone start: %llu\n",
			 mdname(mddev),
			 (unsigned long long)smallest->sectors);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_debug("md/raid0:%s: current zone start: %llu\n",
			 mdname(mddev),
			 (unsigned long long)smallest->sectors);
>>>>>>> refs/remotes/origin/master
	}
	mddev->queue->backing_dev_info.congested_fn = raid0_congested;
	mddev->queue->backing_dev_info.congested_data = mddev;

	/*
	 * now since we have the hard sector sizes, we can make sure
	 * chunk size is a multiple of that sector size
	 */
	if ((mddev->chunk_sectors << 9) % queue_logical_block_size(mddev->queue)) {
		printk(KERN_ERR "md/raid0:%s: chunk_size of %d not valid\n",
		       mdname(mddev),
		       mddev->chunk_sectors << 9);
		goto abort;
	}

	blk_queue_io_min(mddev->queue, mddev->chunk_sectors << 9);
	blk_queue_io_opt(mddev->queue,
			 (mddev->chunk_sectors << 9) * mddev->raid_disks);

<<<<<<< HEAD
<<<<<<< HEAD
	printk(KERN_INFO "md/raid0:%s: done.\n", mdname(mddev));
=======
	pr_debug("md/raid0:%s: done.\n", mdname(mddev));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!discard_supported)
		queue_flag_clear_unlocked(QUEUE_FLAG_DISCARD, mddev->queue);
	else
		queue_flag_set_unlocked(QUEUE_FLAG_DISCARD, mddev->queue);

	pr_debug("md/raid0:%s: done.\n", mdname(mddev));
>>>>>>> refs/remotes/origin/master
	*private_conf = conf;

	return 0;
abort:
	kfree(conf->strip_zone);
	kfree(conf->devlist);
	kfree(conf);
	*private_conf = ERR_PTR(err);
	return err;
}

<<<<<<< HEAD
<<<<<<< HEAD
/**
 *	raid0_mergeable_bvec -- tell bio layer if a two requests can be merged
=======
=======
>>>>>>> refs/remotes/origin/master
/* Find the zone which holds a particular offset
 * Update *sectorp to be an offset in that zone
 */
static struct strip_zone *find_zone(struct r0conf *conf,
				    sector_t *sectorp)
{
	int i;
	struct strip_zone *z = conf->strip_zone;
	sector_t sector = *sectorp;

	for (i = 0; i < conf->nr_strip_zones; i++)
		if (sector < z[i].zone_end) {
			if (i)
				*sectorp = sector - z[i-1].zone_end;
			return z + i;
		}
	BUG();
}

/*
 * remaps the bio to the target device. we separate two flows.
 * power 2 flow and a general flow for the sake of perfromance
*/
static struct md_rdev *map_sector(struct mddev *mddev, struct strip_zone *zone,
				sector_t sector, sector_t *sector_offset)
{
	unsigned int sect_in_chunk;
	sector_t chunk;
	struct r0conf *conf = mddev->private;
	int raid_disks = conf->strip_zone[0].nb_dev;
	unsigned int chunk_sects = mddev->chunk_sectors;

	if (is_power_of_2(chunk_sects)) {
		int chunksect_bits = ffz(~chunk_sects);
		/* find the sector offset inside the chunk */
		sect_in_chunk  = sector & (chunk_sects - 1);
		sector >>= chunksect_bits;
		/* chunk in zone */
		chunk = *sector_offset;
		/* quotient is the chunk in real device*/
		sector_div(chunk, zone->nb_dev << chunksect_bits);
	} else{
		sect_in_chunk = sector_div(sector, chunk_sects);
		chunk = *sector_offset;
		sector_div(chunk, chunk_sects * zone->nb_dev);
	}
	/*
	*  position the bio over the real device
	*  real sector = chunk in device + starting of zone
	*	+ the position in the chunk
	*/
	*sector_offset = (chunk * chunk_sects) + sect_in_chunk;
	return conf->devlist[(zone - conf->strip_zone)*raid_disks
			     + sector_div(sector, zone->nb_dev)];
}

/**
 *	raid0_mergeable_bvec -- tell bio layer if two requests can be merged
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 *	@q: request queue
 *	@bvm: properties of new bio
 *	@biovec: the request that could be merged to it.
 *
 *	Return amount of bytes we can accept at this offset
 */
static int raid0_mergeable_bvec(struct request_queue *q,
				struct bvec_merge_data *bvm,
				struct bio_vec *biovec)
{
<<<<<<< HEAD
<<<<<<< HEAD
	mddev_t *mddev = q->queuedata;
	sector_t sector = bvm->bi_sector + get_start_sect(bvm->bi_bdev);
	int max;
	unsigned int chunk_sectors = mddev->chunk_sectors;
	unsigned int bio_sectors = bvm->bi_size >> 9;
=======
=======
>>>>>>> refs/remotes/origin/master
	struct mddev *mddev = q->queuedata;
	struct r0conf *conf = mddev->private;
	sector_t sector = bvm->bi_sector + get_start_sect(bvm->bi_bdev);
	sector_t sector_offset = sector;
	int max;
	unsigned int chunk_sectors = mddev->chunk_sectors;
	unsigned int bio_sectors = bvm->bi_size >> 9;
	struct strip_zone *zone;
	struct md_rdev *rdev;
	struct request_queue *subq;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	if (is_power_of_2(chunk_sectors))
		max =  (chunk_sectors - ((sector & (chunk_sectors-1))
						+ bio_sectors)) << 9;
	else
		max =  (chunk_sectors - (sector_div(sector, chunk_sectors)
						+ bio_sectors)) << 9;
<<<<<<< HEAD
<<<<<<< HEAD
	if (max < 0) max = 0; /* bio_add cannot handle a negative return */
	if (max <= biovec->bv_len && bio_sectors == 0)
		return biovec->bv_len;
	else 
		return max;
}

static sector_t raid0_size(mddev_t *mddev, sector_t sectors, int raid_disks)
{
	sector_t array_sectors = 0;
	mdk_rdev_t *rdev;
=======
=======
>>>>>>> refs/remotes/origin/master
	if (max < 0)
		max = 0; /* bio_add cannot handle a negative return */
	if (max <= biovec->bv_len && bio_sectors == 0)
		return biovec->bv_len;
	if (max < biovec->bv_len)
		/* too small already, no need to check further */
		return max;
	if (!conf->has_merge_bvec)
		return max;

	/* May need to check subordinate device */
	sector = sector_offset;
	zone = find_zone(mddev->private, &sector_offset);
	rdev = map_sector(mddev, zone, sector, &sector_offset);
	subq = bdev_get_queue(rdev->bdev);
	if (subq->merge_bvec_fn) {
		bvm->bi_bdev = rdev->bdev;
		bvm->bi_sector = sector_offset + zone->dev_start +
			rdev->data_offset;
		return min(max, subq->merge_bvec_fn(subq, bvm, biovec));
	} else
		return max;
}

static sector_t raid0_size(struct mddev *mddev, sector_t sectors, int raid_disks)
{
	sector_t array_sectors = 0;
	struct md_rdev *rdev;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	WARN_ONCE(sectors || raid_disks,
		  "%s does not support generic reshape\n", __func__);

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	list_for_each_entry(rdev, &mddev->disks, same_set)
		array_sectors += rdev->sectors;
=======
	rdev_for_each(rdev, mddev)
		array_sectors += (rdev->sectors &
				  ~(sector_t)(mddev->chunk_sectors-1));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	rdev_for_each(rdev, mddev)
		array_sectors += (rdev->sectors &
				  ~(sector_t)(mddev->chunk_sectors-1));
>>>>>>> refs/remotes/origin/master
=======
	rdev_for_each(rdev, mddev)
		array_sectors += (rdev->sectors &
				  ~(sector_t)(mddev->chunk_sectors-1));
>>>>>>> refs/remotes/origin/cm-11.0

	return array_sectors;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int raid0_run(mddev_t *mddev)
{
	raid0_conf_t *conf;
=======
=======
>>>>>>> refs/remotes/origin/master
static int raid0_stop(struct mddev *mddev);

static int raid0_run(struct mddev *mddev)
{
	struct r0conf *conf;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	int ret;

	if (mddev->chunk_sectors == 0) {
		printk(KERN_ERR "md/raid0:%s: chunk size must be set.\n",
		       mdname(mddev));
		return -EINVAL;
	}
	if (md_check_no_bitmap(mddev))
		return -EINVAL;
	blk_queue_max_hw_sectors(mddev->queue, mddev->chunk_sectors);
<<<<<<< HEAD
=======
	blk_queue_max_write_same_sectors(mddev->queue, mddev->chunk_sectors);
	blk_queue_max_discard_sectors(mddev->queue, mddev->chunk_sectors);
>>>>>>> refs/remotes/origin/master

	/* if private is not null, we are here after takeover */
	if (mddev->private == NULL) {
		ret = create_strip_zones(mddev, &conf);
		if (ret < 0)
			return ret;
		mddev->private = conf;
	}
	conf = mddev->private;

	/* calculate array device size */
	md_set_array_sectors(mddev, raid0_size(mddev, 0, 0));

	printk(KERN_INFO "md/raid0:%s: md_size is %llu sectors.\n",
	       mdname(mddev),
	       (unsigned long long)mddev->array_sectors);
	/* calculate the max read-ahead size.
	 * For read-ahead of large files to be effective, we need to
	 * readahead at least twice a whole stripe. i.e. number of devices
	 * multiplied by chunk size times 2.
	 * If an individual device has an ra_pages greater than the
	 * chunk size, then we will not drive that device as hard as it
	 * wants.  We consider this a configuration error: a larger
	 * chunksize should be used in that case.
	 */
	{
		int stripe = mddev->raid_disks *
			(mddev->chunk_sectors << 9) / PAGE_SIZE;
		if (mddev->queue->backing_dev_info.ra_pages < 2* stripe)
			mddev->queue->backing_dev_info.ra_pages = 2* stripe;
	}

	blk_queue_merge_bvec(mddev->queue, raid0_mergeable_bvec);
	dump_zones(mddev);
<<<<<<< HEAD
<<<<<<< HEAD
	return md_integrity_register(mddev);
}

static int raid0_stop(mddev_t *mddev)
{
	raid0_conf_t *conf = mddev->private;
=======
=======
>>>>>>> refs/remotes/origin/master

	ret = md_integrity_register(mddev);
	if (ret)
		raid0_stop(mddev);

	return ret;
}

static int raid0_stop(struct mddev *mddev)
{
	struct r0conf *conf = mddev->private;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	blk_sync_queue(mddev->queue); /* the unplug fn references 'conf'*/
	kfree(conf->strip_zone);
	kfree(conf->devlist);
	kfree(conf);
	mddev->private = NULL;
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
/* Find the zone which holds a particular offset
 * Update *sectorp to be an offset in that zone
 */
static struct strip_zone *find_zone(struct raid0_private_data *conf,
				    sector_t *sectorp)
{
	int i;
	struct strip_zone *z = conf->strip_zone;
	sector_t sector = *sectorp;

	for (i = 0; i < conf->nr_strip_zones; i++)
		if (sector < z[i].zone_end) {
			if (i)
				*sectorp = sector - z[i-1].zone_end;
			return z + i;
		}
	BUG();
}

/*
 * remaps the bio to the target device. we separate two flows.
 * power 2 flow and a general flow for the sake of perfromance
*/
static mdk_rdev_t *map_sector(mddev_t *mddev, struct strip_zone *zone,
				sector_t sector, sector_t *sector_offset)
{
	unsigned int sect_in_chunk;
	sector_t chunk;
	raid0_conf_t *conf = mddev->private;
	int raid_disks = conf->strip_zone[0].nb_dev;
	unsigned int chunk_sects = mddev->chunk_sectors;

	if (is_power_of_2(chunk_sects)) {
		int chunksect_bits = ffz(~chunk_sects);
		/* find the sector offset inside the chunk */
		sect_in_chunk  = sector & (chunk_sects - 1);
		sector >>= chunksect_bits;
		/* chunk in zone */
		chunk = *sector_offset;
		/* quotient is the chunk in real device*/
		sector_div(chunk, zone->nb_dev << chunksect_bits);
	} else{
		sect_in_chunk = sector_div(sector, chunk_sects);
		chunk = *sector_offset;
		sector_div(chunk, chunk_sects * zone->nb_dev);
	}
	/*
	*  position the bio over the real device
	*  real sector = chunk in device + starting of zone
	*	+ the position in the chunk
	*/
	*sector_offset = (chunk * chunk_sects) + sect_in_chunk;
	return conf->devlist[(zone - conf->strip_zone)*raid_disks
			     + sector_div(sector, zone->nb_dev)];
}

/*
 * Is io distribute over 1 or more chunks ?
*/
static inline int is_io_in_chunk_boundary(mddev_t *mddev,
=======
=======
>>>>>>> refs/remotes/origin/master
/*
 * Is io distribute over 1 or more chunks ?
*/
static inline int is_io_in_chunk_boundary(struct mddev *mddev,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			unsigned int chunk_sects, struct bio *bio)
{
	if (likely(is_power_of_2(chunk_sects))) {
		return chunk_sects >= ((bio->bi_sector & (chunk_sects-1))
<<<<<<< HEAD
					+ (bio->bi_size >> 9));
	} else{
		sector_t sector = bio->bi_sector;
		return chunk_sects >= (sector_div(sector, chunk_sects)
						+ (bio->bi_size >> 9));
	}
}

<<<<<<< HEAD
static int raid0_make_request(mddev_t *mddev, struct bio *bio)
=======
static void raid0_make_request(struct mddev *mddev, struct bio *bio)
>>>>>>> refs/remotes/origin/cm-10.0
=======
					+ bio_sectors(bio));
	} else{
		sector_t sector = bio->bi_sector;
		return chunk_sects >= (sector_div(sector, chunk_sects)
						+ bio_sectors(bio));
	}
}

static void raid0_make_request(struct mddev *mddev, struct bio *bio)
>>>>>>> refs/remotes/origin/master
{
	unsigned int chunk_sects;
	sector_t sector_offset;
	struct strip_zone *zone;
<<<<<<< HEAD
<<<<<<< HEAD
	mdk_rdev_t *tmp_dev;

	if (unlikely(bio->bi_rw & REQ_FLUSH)) {
		md_flush_request(mddev, bio);
		return 0;
=======
=======
>>>>>>> refs/remotes/origin/master
	struct md_rdev *tmp_dev;

	if (unlikely(bio->bi_rw & REQ_FLUSH)) {
		md_flush_request(mddev, bio);
		return;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	chunk_sects = mddev->chunk_sectors;
	if (unlikely(!is_io_in_chunk_boundary(mddev, chunk_sects, bio))) {
		sector_t sector = bio->bi_sector;
		struct bio_pair *bp;
		/* Sanity check -- queue functions should prevent this happening */
<<<<<<< HEAD
		if (bio->bi_vcnt != 1 ||
		    bio->bi_idx != 0)
=======
		if (bio_segments(bio) > 1)
>>>>>>> refs/remotes/origin/master
			goto bad_map;
		/* This is a one page bio that upper layers
		 * refuse to split for us, so we need to split it.
		 */
		if (likely(is_power_of_2(chunk_sects)))
			bp = bio_split(bio, chunk_sects - (sector &
							   (chunk_sects-1)));
		else
			bp = bio_split(bio, chunk_sects -
				       sector_div(sector, chunk_sects));
<<<<<<< HEAD
<<<<<<< HEAD
		if (raid0_make_request(mddev, &bp->bio1))
			generic_make_request(&bp->bio1);
		if (raid0_make_request(mddev, &bp->bio2))
			generic_make_request(&bp->bio2);

		bio_pair_release(bp);
		return 0;
	}

	sector_offset = bio->bi_sector;
	zone =  find_zone(mddev->private, &sector_offset);
=======
=======
>>>>>>> refs/remotes/origin/master
		raid0_make_request(mddev, &bp->bio1);
		raid0_make_request(mddev, &bp->bio2);
		bio_pair_release(bp);
		return;
	}

	sector_offset = bio->bi_sector;
	zone = find_zone(mddev->private, &sector_offset);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	tmp_dev = map_sector(mddev, zone, bio->bi_sector,
			     &sector_offset);
	bio->bi_bdev = tmp_dev->bdev;
	bio->bi_sector = sector_offset + zone->dev_start +
		tmp_dev->data_offset;
<<<<<<< HEAD
<<<<<<< HEAD
	/*
	 * Let the main block layer submit the IO and resolve recursion:
	 */
	return 1;
=======

	generic_make_request(bio);
	return;
>>>>>>> refs/remotes/origin/cm-10.0
=======

	if (unlikely((bio->bi_rw & REQ_DISCARD) &&
		     !blk_queue_discard(bdev_get_queue(bio->bi_bdev)))) {
		/* Just ignore it */
		bio_endio(bio, 0);
		return;
	}

	generic_make_request(bio);
	return;
>>>>>>> refs/remotes/origin/master

bad_map:
	printk("md/raid0:%s: make_request bug: can't convert block across chunks"
	       " or bigger than %dk %llu %d\n",
	       mdname(mddev), chunk_sects / 2,
<<<<<<< HEAD
	       (unsigned long long)bio->bi_sector, bio->bi_size >> 10);

	bio_io_error(bio);
<<<<<<< HEAD
	return 0;
}

static void raid0_status(struct seq_file *seq, mddev_t *mddev)
{
#undef MD_DEBUG
#ifdef MD_DEBUG
	int j, k, h;
	char b[BDEVNAME_SIZE];
	raid0_conf_t *conf = mddev->private;
	int raid_disks = conf->strip_zone[0].nb_dev;

	sector_t zone_size;
	sector_t zone_start = 0;
	h = 0;

	for (j = 0; j < conf->nr_strip_zones; j++) {
		seq_printf(seq, "      z%d", j);
		seq_printf(seq, "=[");
		for (k = 0; k < conf->strip_zone[j].nb_dev; k++)
			seq_printf(seq, "%s/", bdevname(
				conf->devlist[j*raid_disks + k]
						->bdev, b));

		zone_size  = conf->strip_zone[j].zone_end - zone_start;
		seq_printf(seq, "] ze=%lld ds=%lld s=%lld\n",
			(unsigned long long)zone_start>>1,
			(unsigned long long)conf->strip_zone[j].dev_start>>1,
			(unsigned long long)zone_size>>1);
		zone_start = conf->strip_zone[j].zone_end;
	}
#endif
=======
=======
	       (unsigned long long)bio->bi_sector, bio_sectors(bio) / 2);

	bio_io_error(bio);
>>>>>>> refs/remotes/origin/master
	return;
}

static void raid0_status(struct seq_file *seq, struct mddev *mddev)
{
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	seq_printf(seq, " %dk chunks", mddev->chunk_sectors / 2);
	return;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void *raid0_takeover_raid45(mddev_t *mddev)
{
	mdk_rdev_t *rdev;
	raid0_conf_t *priv_conf;
=======
=======
>>>>>>> refs/remotes/origin/master
static void *raid0_takeover_raid45(struct mddev *mddev)
{
	struct md_rdev *rdev;
	struct r0conf *priv_conf;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	if (mddev->degraded != 1) {
		printk(KERN_ERR "md/raid0:%s: raid5 must be degraded! Degraded disks: %d\n",
		       mdname(mddev),
		       mddev->degraded);
		return ERR_PTR(-EINVAL);
	}

<<<<<<< HEAD
<<<<<<< HEAD
	list_for_each_entry(rdev, &mddev->disks, same_set) {
=======
	rdev_for_each(rdev, mddev) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	rdev_for_each(rdev, mddev) {
>>>>>>> refs/remotes/origin/master
		/* check slot number for a disk */
		if (rdev->raid_disk == mddev->raid_disks-1) {
			printk(KERN_ERR "md/raid0:%s: raid5 must have missing parity disk!\n",
			       mdname(mddev));
			return ERR_PTR(-EINVAL);
		}
<<<<<<< HEAD
=======
		rdev->sectors = mddev->dev_sectors;
>>>>>>> refs/remotes/origin/master
	}

	/* Set new parameters */
	mddev->new_level = 0;
	mddev->new_layout = 0;
	mddev->new_chunk_sectors = mddev->chunk_sectors;
	mddev->raid_disks--;
	mddev->delta_disks = -1;
	/* make sure it will be not marked as dirty */
	mddev->recovery_cp = MaxSector;

	create_strip_zones(mddev, &priv_conf);
	return priv_conf;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void *raid0_takeover_raid10(mddev_t *mddev)
{
	raid0_conf_t *priv_conf;
=======
static void *raid0_takeover_raid10(struct mddev *mddev)
{
	struct r0conf *priv_conf;
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void *raid0_takeover_raid10(struct mddev *mddev)
{
	struct r0conf *priv_conf;
>>>>>>> refs/remotes/origin/master

	/* Check layout:
	 *  - far_copies must be 1
	 *  - near_copies must be 2
	 *  - disks number must be even
	 *  - all mirrors must be already degraded
	 */
	if (mddev->layout != ((1 << 8) + 2)) {
		printk(KERN_ERR "md/raid0:%s:: Raid0 cannot takover layout: 0x%x\n",
		       mdname(mddev),
		       mddev->layout);
		return ERR_PTR(-EINVAL);
	}
	if (mddev->raid_disks & 1) {
		printk(KERN_ERR "md/raid0:%s: Raid0 cannot takover Raid10 with odd disk number.\n",
		       mdname(mddev));
		return ERR_PTR(-EINVAL);
	}
	if (mddev->degraded != (mddev->raid_disks>>1)) {
		printk(KERN_ERR "md/raid0:%s: All mirrors must be already degraded!\n",
		       mdname(mddev));
		return ERR_PTR(-EINVAL);
	}

	/* Set new parameters */
	mddev->new_level = 0;
	mddev->new_layout = 0;
	mddev->new_chunk_sectors = mddev->chunk_sectors;
	mddev->delta_disks = - mddev->raid_disks / 2;
	mddev->raid_disks += mddev->delta_disks;
	mddev->degraded = 0;
	/* make sure it will be not marked as dirty */
	mddev->recovery_cp = MaxSector;

	create_strip_zones(mddev, &priv_conf);
	return priv_conf;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void *raid0_takeover_raid1(mddev_t *mddev)
{
	raid0_conf_t *priv_conf;
=======
=======
>>>>>>> refs/remotes/origin/master
static void *raid0_takeover_raid1(struct mddev *mddev)
{
	struct r0conf *priv_conf;
	int chunksect;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	/* Check layout:
	 *  - (N - 1) mirror drives must be already faulty
	 */
	if ((mddev->raid_disks - 1) != mddev->degraded) {
		printk(KERN_ERR "md/raid0:%s: (N - 1) mirrors drives must be already faulty!\n",
		       mdname(mddev));
		return ERR_PTR(-EINVAL);
	}

<<<<<<< HEAD
<<<<<<< HEAD
	/* Set new parameters */
	mddev->new_level = 0;
	mddev->new_layout = 0;
	mddev->new_chunk_sectors = 128; /* by default set chunk size to 64k */
=======
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * a raid1 doesn't have the notion of chunk size, so
	 * figure out the largest suitable size we can use.
	 */
	chunksect = 64 * 2; /* 64K by default */

	/* The array must be an exact multiple of chunksize */
	while (chunksect && (mddev->array_sectors & (chunksect - 1)))
		chunksect >>= 1;

	if ((chunksect << 9) < PAGE_SIZE)
		/* array size does not allow a suitable chunk size */
		return ERR_PTR(-EINVAL);

	/* Set new parameters */
	mddev->new_level = 0;
	mddev->new_layout = 0;
	mddev->new_chunk_sectors = chunksect;
	mddev->chunk_sectors = chunksect;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	mddev->delta_disks = 1 - mddev->raid_disks;
	mddev->raid_disks = 1;
	/* make sure it will be not marked as dirty */
	mddev->recovery_cp = MaxSector;

	create_strip_zones(mddev, &priv_conf);
	return priv_conf;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void *raid0_takeover(mddev_t *mddev)
=======
static void *raid0_takeover(struct mddev *mddev)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void *raid0_takeover(struct mddev *mddev)
>>>>>>> refs/remotes/origin/master
{
	/* raid0 can take over:
	 *  raid4 - if all data disks are active.
	 *  raid5 - providing it is Raid4 layout and one disk is faulty
	 *  raid10 - assuming we have all necessary active disks
	 *  raid1 - with (N -1) mirror drives faulty
	 */
	if (mddev->level == 4)
		return raid0_takeover_raid45(mddev);

	if (mddev->level == 5) {
		if (mddev->layout == ALGORITHM_PARITY_N)
			return raid0_takeover_raid45(mddev);

		printk(KERN_ERR "md/raid0:%s: Raid can only takeover Raid5 with layout: %d\n",
		       mdname(mddev), ALGORITHM_PARITY_N);
	}

	if (mddev->level == 10)
		return raid0_takeover_raid10(mddev);

	if (mddev->level == 1)
		return raid0_takeover_raid1(mddev);

	printk(KERN_ERR "Takeover from raid%i to raid0 not supported\n",
		mddev->level);

	return ERR_PTR(-EINVAL);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void raid0_quiesce(mddev_t *mddev, int state)
{
}

static struct mdk_personality raid0_personality=
=======
=======
>>>>>>> refs/remotes/origin/master
static void raid0_quiesce(struct mddev *mddev, int state)
{
}

static struct md_personality raid0_personality=
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	.name		= "raid0",
	.level		= 0,
	.owner		= THIS_MODULE,
	.make_request	= raid0_make_request,
	.run		= raid0_run,
	.stop		= raid0_stop,
	.status		= raid0_status,
	.size		= raid0_size,
	.takeover	= raid0_takeover,
	.quiesce	= raid0_quiesce,
};

static int __init raid0_init (void)
{
	return register_md_personality (&raid0_personality);
}

static void raid0_exit (void)
{
	unregister_md_personality (&raid0_personality);
}

module_init(raid0_init);
module_exit(raid0_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("RAID0 (striping) personality for MD");
MODULE_ALIAS("md-personality-2"); /* RAID0 */
MODULE_ALIAS("md-raid0");
MODULE_ALIAS("md-level-0");
