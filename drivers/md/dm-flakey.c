/*
 * Copyright (C) 2003 Sistina Software (UK) Limited.
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright (C) 2004, 2010 Red Hat, Inc. All rights reserved.
=======
 * Copyright (C) 2004, 2010-2011 Red Hat, Inc. All rights reserved.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Copyright (C) 2004, 2010-2011 Red Hat, Inc. All rights reserved.
>>>>>>> refs/remotes/origin/master
 *
 * This file is released under the GPL.
 */

#include <linux/device-mapper.h>

#include <linux/module.h>
#include <linux/init.h>
#include <linux/blkdev.h>
#include <linux/bio.h>
#include <linux/slab.h>

#define DM_MSG_PREFIX "flakey"

<<<<<<< HEAD
<<<<<<< HEAD
=======
#define all_corrupt_bio_flags_match(bio, fc)	\
	(((bio)->bi_rw & (fc)->corrupt_bio_flags) == (fc)->corrupt_bio_flags)

>>>>>>> refs/remotes/origin/cm-10.0
=======
#define all_corrupt_bio_flags_match(bio, fc)	\
	(((bio)->bi_rw & (fc)->corrupt_bio_flags) == (fc)->corrupt_bio_flags)

>>>>>>> refs/remotes/origin/master
/*
 * Flakey: Used for testing only, simulates intermittent,
 * catastrophic device failure.
 */
struct flakey_c {
	struct dm_dev *dev;
	unsigned long start_time;
	sector_t start;
	unsigned up_interval;
	unsigned down_interval;
<<<<<<< HEAD
<<<<<<< HEAD
};

/*
 * Construct a flakey mapping: <dev_path> <offset> <up interval> <down interval>
 */
static int flakey_ctr(struct dm_target *ti, unsigned int argc, char **argv)
{
	struct flakey_c *fc;
	unsigned long long tmp;

	if (argc != 4) {
		ti->error = "dm-flakey: Invalid argument count";
		return -EINVAL;
	}

	fc = kmalloc(sizeof(*fc), GFP_KERNEL);
	if (!fc) {
		ti->error = "dm-flakey: Cannot allocate linear context";
=======
=======
>>>>>>> refs/remotes/origin/master
	unsigned long flags;
	unsigned corrupt_bio_byte;
	unsigned corrupt_bio_rw;
	unsigned corrupt_bio_value;
	unsigned corrupt_bio_flags;
};

enum feature_flag_bits {
	DROP_WRITES
};

<<<<<<< HEAD
=======
struct per_bio_data {
	bool bio_submitted;
};

>>>>>>> refs/remotes/origin/master
static int parse_features(struct dm_arg_set *as, struct flakey_c *fc,
			  struct dm_target *ti)
{
	int r;
	unsigned argc;
	const char *arg_name;

	static struct dm_arg _args[] = {
		{0, 6, "Invalid number of feature args"},
		{1, UINT_MAX, "Invalid corrupt bio byte"},
		{0, 255, "Invalid corrupt value to write into bio byte (0-255)"},
		{0, UINT_MAX, "Invalid corrupt bio flags mask"},
	};

	/* No feature arguments supplied. */
	if (!as->argc)
		return 0;

	r = dm_read_arg_group(_args, as, &argc, &ti->error);
	if (r)
		return r;

	while (argc) {
		arg_name = dm_shift_arg(as);
		argc--;

		/*
		 * drop_writes
		 */
		if (!strcasecmp(arg_name, "drop_writes")) {
			if (test_and_set_bit(DROP_WRITES, &fc->flags)) {
				ti->error = "Feature drop_writes duplicated";
				return -EINVAL;
			}

			continue;
		}

		/*
		 * corrupt_bio_byte <Nth_byte> <direction> <value> <bio_flags>
		 */
		if (!strcasecmp(arg_name, "corrupt_bio_byte")) {
			if (!argc) {
				ti->error = "Feature corrupt_bio_byte requires parameters";
				return -EINVAL;
			}

			r = dm_read_arg(_args + 1, as, &fc->corrupt_bio_byte, &ti->error);
			if (r)
				return r;
			argc--;

			/*
			 * Direction r or w?
			 */
			arg_name = dm_shift_arg(as);
			if (!strcasecmp(arg_name, "w"))
				fc->corrupt_bio_rw = WRITE;
			else if (!strcasecmp(arg_name, "r"))
				fc->corrupt_bio_rw = READ;
			else {
				ti->error = "Invalid corrupt bio direction (r or w)";
				return -EINVAL;
			}
			argc--;

			/*
			 * Value of byte (0-255) to write in place of correct one.
			 */
			r = dm_read_arg(_args + 2, as, &fc->corrupt_bio_value, &ti->error);
			if (r)
				return r;
			argc--;

			/*
			 * Only corrupt bios with these flags set.
			 */
			r = dm_read_arg(_args + 3, as, &fc->corrupt_bio_flags, &ti->error);
			if (r)
				return r;
			argc--;

			continue;
		}

		ti->error = "Unrecognised flakey feature requested";
		return -EINVAL;
	}

	if (test_bit(DROP_WRITES, &fc->flags) && (fc->corrupt_bio_rw == WRITE)) {
		ti->error = "drop_writes is incompatible with corrupt_bio_byte with the WRITE flag set";
		return -EINVAL;
	}

	return 0;
}

/*
 * Construct a flakey mapping:
 * <dev_path> <offset> <up interval> <down interval> [<#feature args> [<arg>]*]
 *
 *   Feature args:
 *     [drop_writes]
 *     [corrupt_bio_byte <Nth_byte> <direction> <value> <bio_flags>]
 *
 *   Nth_byte starts from 1 for the first byte.
 *   Direction is r for READ or w for WRITE.
 *   bio_flags is ignored if 0.
 */
static int flakey_ctr(struct dm_target *ti, unsigned int argc, char **argv)
{
	static struct dm_arg _args[] = {
		{0, UINT_MAX, "Invalid up interval"},
		{0, UINT_MAX, "Invalid down interval"},
	};

	int r;
	struct flakey_c *fc;
	unsigned long long tmpll;
	struct dm_arg_set as;
	const char *devname;
	char dummy;

	as.argc = argc;
	as.argv = argv;

	if (argc < 4) {
		ti->error = "Invalid argument count";
		return -EINVAL;
	}

	fc = kzalloc(sizeof(*fc), GFP_KERNEL);
	if (!fc) {
<<<<<<< HEAD
		ti->error = "Cannot allocate linear context";
>>>>>>> refs/remotes/origin/cm-10.0
=======
		ti->error = "Cannot allocate context";
>>>>>>> refs/remotes/origin/master
		return -ENOMEM;
	}
	fc->start_time = jiffies;

<<<<<<< HEAD
<<<<<<< HEAD
	if (sscanf(argv[1], "%llu", &tmp) != 1) {
		ti->error = "dm-flakey: Invalid device sector";
		goto bad;
	}
	fc->start = tmp;

	if (sscanf(argv[2], "%u", &fc->up_interval) != 1) {
		ti->error = "dm-flakey: Invalid up interval";
		goto bad;
	}

	if (sscanf(argv[3], "%u", &fc->down_interval) != 1) {
		ti->error = "dm-flakey: Invalid down interval";
		goto bad;
	}

	if (!(fc->up_interval + fc->down_interval)) {
		ti->error = "dm-flakey: Total (up + down) interval is zero";
=======
=======
>>>>>>> refs/remotes/origin/master
	devname = dm_shift_arg(&as);

	if (sscanf(dm_shift_arg(&as), "%llu%c", &tmpll, &dummy) != 1) {
		ti->error = "Invalid device sector";
		goto bad;
	}
	fc->start = tmpll;

	r = dm_read_arg(_args, &as, &fc->up_interval, &ti->error);
	if (r)
		goto bad;

	r = dm_read_arg(_args, &as, &fc->down_interval, &ti->error);
	if (r)
		goto bad;

	if (!(fc->up_interval + fc->down_interval)) {
		ti->error = "Total (up + down) interval is zero";
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		goto bad;
	}

	if (fc->up_interval + fc->down_interval < fc->up_interval) {
<<<<<<< HEAD
<<<<<<< HEAD
		ti->error = "dm-flakey: Interval overflow";
		goto bad;
	}

	if (dm_get_device(ti, argv[0], dm_table_get_mode(ti->table), &fc->dev)) {
		ti->error = "dm-flakey: Device lookup failed";
=======
=======
>>>>>>> refs/remotes/origin/master
		ti->error = "Interval overflow";
		goto bad;
	}

	r = parse_features(&as, fc, ti);
	if (r)
		goto bad;

	if (dm_get_device(ti, devname, dm_table_get_mode(ti->table), &fc->dev)) {
		ti->error = "Device lookup failed";
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		goto bad;
	}

	ti->num_flush_requests = 1;
<<<<<<< HEAD
=======
	ti->num_discard_requests = 1;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		goto bad;
	}

	ti->num_flush_bios = 1;
	ti->num_discard_bios = 1;
	ti->per_bio_data_size = sizeof(struct per_bio_data);
>>>>>>> refs/remotes/origin/master
	ti->private = fc;
	return 0;

bad:
	kfree(fc);
	return -EINVAL;
}

static void flakey_dtr(struct dm_target *ti)
{
	struct flakey_c *fc = ti->private;

	dm_put_device(ti, fc->dev);
	kfree(fc);
}

static sector_t flakey_map_sector(struct dm_target *ti, sector_t bi_sector)
{
	struct flakey_c *fc = ti->private;

<<<<<<< HEAD
<<<<<<< HEAD
	return fc->start + (bi_sector - ti->begin);
=======
	return fc->start + dm_target_offset(ti, bi_sector);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return fc->start + dm_target_offset(ti, bi_sector);
>>>>>>> refs/remotes/origin/master
}

static void flakey_map_bio(struct dm_target *ti, struct bio *bio)
{
	struct flakey_c *fc = ti->private;

	bio->bi_bdev = fc->dev->bdev;
	if (bio_sectors(bio))
		bio->bi_sector = flakey_map_sector(ti, bio->bi_sector);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static void corrupt_bio_data(struct bio *bio, struct flakey_c *fc)
{
	unsigned bio_bytes = bio_cur_bytes(bio);
	char *data = bio_data(bio);

	/*
	 * Overwrite the Nth byte of the data returned.
	 */
	if (data && bio_bytes >= fc->corrupt_bio_byte) {
		data[fc->corrupt_bio_byte - 1] = fc->corrupt_bio_value;

		DMDEBUG("Corrupting data bio=%p by writing %u to byte %u "
			"(rw=%c bi_rw=%lu bi_sector=%llu cur_bytes=%u)\n",
			bio, fc->corrupt_bio_value, fc->corrupt_bio_byte,
			(bio_data_dir(bio) == WRITE) ? 'w' : 'r',
			bio->bi_rw, (unsigned long long)bio->bi_sector, bio_bytes);
	}
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
static int flakey_map(struct dm_target *ti, struct bio *bio,
		      union map_info *map_context)
{
	struct flakey_c *fc = ti->private;
	unsigned elapsed;

	/* Are we alive ? */
	elapsed = (jiffies - fc->start_time) / HZ;
<<<<<<< HEAD
	if (elapsed % (fc->up_interval + fc->down_interval) >= fc->up_interval)
		return -EIO;

=======
=======
static int flakey_map(struct dm_target *ti, struct bio *bio)
{
	struct flakey_c *fc = ti->private;
	unsigned elapsed;
	struct per_bio_data *pb = dm_per_bio_data(bio, sizeof(struct per_bio_data));
	pb->bio_submitted = false;

	/* Are we alive ? */
	elapsed = (jiffies - fc->start_time) / HZ;
>>>>>>> refs/remotes/origin/master
	if (elapsed % (fc->up_interval + fc->down_interval) >= fc->up_interval) {
		/*
		 * Flag this bio as submitted while down.
		 */
<<<<<<< HEAD
		map_context->ll = 1;
=======
		pb->bio_submitted = true;
>>>>>>> refs/remotes/origin/master

		/*
		 * Map reads as normal.
		 */
		if (bio_data_dir(bio) == READ)
			goto map_bio;

		/*
		 * Drop writes?
		 */
		if (test_bit(DROP_WRITES, &fc->flags)) {
			bio_endio(bio, 0);
			return DM_MAPIO_SUBMITTED;
		}

		/*
		 * Corrupt matching writes.
		 */
		if (fc->corrupt_bio_byte && (fc->corrupt_bio_rw == WRITE)) {
			if (all_corrupt_bio_flags_match(bio, fc))
				corrupt_bio_data(bio, fc);
			goto map_bio;
		}

		/*
		 * By default, error all I/O.
		 */
		return -EIO;
	}

map_bio:
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	flakey_map_bio(ti, bio);

	return DM_MAPIO_REMAPPED;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int flakey_status(struct dm_target *ti, status_type_t type,
			 char *result, unsigned int maxlen)
{
	struct flakey_c *fc = ti->private;
=======
static int flakey_end_io(struct dm_target *ti, struct bio *bio,
			 int error, union map_info *map_context)
{
	struct flakey_c *fc = ti->private;
	unsigned bio_submitted_while_down = map_context->ll;
=======
static int flakey_end_io(struct dm_target *ti, struct bio *bio, int error)
{
	struct flakey_c *fc = ti->private;
	struct per_bio_data *pb = dm_per_bio_data(bio, sizeof(struct per_bio_data));
>>>>>>> refs/remotes/origin/master

	/*
	 * Corrupt successful READs while in down state.
	 * If flags were specified, only corrupt those that match.
	 */
<<<<<<< HEAD
	if (fc->corrupt_bio_byte && !error && bio_submitted_while_down &&
=======
	if (fc->corrupt_bio_byte && !error && pb->bio_submitted &&
>>>>>>> refs/remotes/origin/master
	    (bio_data_dir(bio) == READ) && (fc->corrupt_bio_rw == READ) &&
	    all_corrupt_bio_flags_match(bio, fc))
		corrupt_bio_data(bio, fc);

	return error;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int flakey_status(struct dm_target *ti, status_type_t type,
			 char *result, unsigned int maxlen)
=======
static void flakey_status(struct dm_target *ti, status_type_t type,
			  unsigned status_flags, char *result, unsigned maxlen)
>>>>>>> refs/remotes/origin/master
=======
static void flakey_status(struct dm_target *ti, status_type_t type,
			  char *result, unsigned int maxlen)
>>>>>>> refs/remotes/origin/cm-11.0
{
	unsigned sz = 0;
	struct flakey_c *fc = ti->private;
	unsigned drop_writes;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	switch (type) {
	case STATUSTYPE_INFO:
		result[0] = '\0';
		break;

	case STATUSTYPE_TABLE:
<<<<<<< HEAD
<<<<<<< HEAD
		snprintf(result, maxlen, "%s %llu %u %u", fc->dev->name,
			 (unsigned long long)fc->start, fc->up_interval,
			 fc->down_interval);
=======
=======
>>>>>>> refs/remotes/origin/master
		DMEMIT("%s %llu %u %u ", fc->dev->name,
		       (unsigned long long)fc->start, fc->up_interval,
		       fc->down_interval);

		drop_writes = test_bit(DROP_WRITES, &fc->flags);
		DMEMIT("%u ", drop_writes + (fc->corrupt_bio_byte > 0) * 5);

		if (drop_writes)
			DMEMIT("drop_writes ");

		if (fc->corrupt_bio_byte)
			DMEMIT("corrupt_bio_byte %u %c %u %u ",
			       fc->corrupt_bio_byte,
			       (fc->corrupt_bio_rw == WRITE) ? 'w' : 'r',
			       fc->corrupt_bio_value, fc->corrupt_bio_flags);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		break;
	}
<<<<<<< HEAD
	return 0;
=======
		break;
	}
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
}

static int flakey_ioctl(struct dm_target *ti, unsigned int cmd, unsigned long arg)
{
	struct flakey_c *fc = ti->private;
	struct dm_dev *dev = fc->dev;
	int r = 0;

	/*
	 * Only pass ioctls through if the device sizes match exactly.
	 */
	if (fc->start ||
	    ti->len != i_size_read(dev->bdev->bd_inode) >> SECTOR_SHIFT)
		r = scsi_verify_blk_ioctl(NULL, cmd);

	return r ? : __blkdev_driver_ioctl(dev->bdev, dev->mode, cmd, arg);
}

static int flakey_merge(struct dm_target *ti, struct bvec_merge_data *bvm,
			struct bio_vec *biovec, int max_size)
{
	struct flakey_c *fc = ti->private;
	struct request_queue *q = bdev_get_queue(fc->dev->bdev);

	if (!q->merge_bvec_fn)
		return max_size;

	bvm->bi_bdev = fc->dev->bdev;
	bvm->bi_sector = flakey_map_sector(ti, bvm->bi_sector);

	return min(max_size, q->merge_bvec_fn(q, bvm, biovec));
}

static int flakey_iterate_devices(struct dm_target *ti, iterate_devices_callout_fn fn, void *data)
{
	struct flakey_c *fc = ti->private;

	return fn(ti, fc->dev, fc->start, ti->len, data);
}

static struct target_type flakey_target = {
	.name   = "flakey",
<<<<<<< HEAD
<<<<<<< HEAD
	.version = {1, 1, 0},
=======
	.version = {1, 2, 0},
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.version = {1, 3, 1},
>>>>>>> refs/remotes/origin/master
	.module = THIS_MODULE,
	.ctr    = flakey_ctr,
	.dtr    = flakey_dtr,
	.map    = flakey_map,
<<<<<<< HEAD
<<<<<<< HEAD
=======
	.end_io = flakey_end_io,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.end_io = flakey_end_io,
>>>>>>> refs/remotes/origin/master
	.status = flakey_status,
	.ioctl	= flakey_ioctl,
	.merge	= flakey_merge,
	.iterate_devices = flakey_iterate_devices,
};

static int __init dm_flakey_init(void)
{
	int r = dm_register_target(&flakey_target);

	if (r < 0)
		DMERR("register failed %d", r);

	return r;
}

static void __exit dm_flakey_exit(void)
{
	dm_unregister_target(&flakey_target);
}

/* Module hooks */
module_init(dm_flakey_init);
module_exit(dm_flakey_exit);

MODULE_DESCRIPTION(DM_NAME " flakey target");
MODULE_AUTHOR("Joe Thornber <dm-devel@redhat.com>");
MODULE_LICENSE("GPL");
