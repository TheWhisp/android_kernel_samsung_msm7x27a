#ifndef _RAID1_H
#define _RAID1_H

<<<<<<< HEAD
<<<<<<< HEAD
typedef struct mirror_info mirror_info_t;

struct mirror_info {
	mdk_rdev_t	*rdev;
=======
struct mirror_info {
	struct md_rdev	*rdev;
>>>>>>> refs/remotes/origin/cm-10.0
	sector_t	head_position;
=======
struct raid1_info {
	struct md_rdev	*rdev;
	sector_t	head_position;

	/* When choose the best device for a read (read_balance())
	 * we try to keep sequential reads one the same device
	 */
	sector_t	next_seq_sect;
	sector_t	seq_start;
>>>>>>> refs/remotes/origin/master
};

/*
 * memory pools need a pointer to the mddev, so they can force an unplug
 * when memory is tight, and a count of the number of drives that the
 * pool was allocated for, so they know how much to allocate and free.
 * mddev->raid_disks cannot be used, as it can change while a pool is active
 * These two datums are stored in a kmalloced struct.
<<<<<<< HEAD
<<<<<<< HEAD
 */

struct pool_info {
	mddev_t *mddev;
	int	raid_disks;
};


typedef struct r1bio_s r1bio_t;

struct r1_private_data_s {
	mddev_t			*mddev;
	mirror_info_t		*mirrors;
	int			raid_disks;
	int			last_used;
	sector_t		next_seq_sect;
	spinlock_t		device_lock;

	struct list_head	retry_list;
	/* queue pending writes and submit them on unplug */
	struct bio_list		pending_bio_list;

	/* for use when syncing mirrors: */

=======
=======
>>>>>>> refs/remotes/origin/master
 * The 'raid_disks' here is twice the raid_disks in r1conf.
 * This allows space for each 'real' device can have a replacement in the
 * second half of the array.
 */

struct pool_info {
	struct mddev *mddev;
	int	raid_disks;
};

struct r1conf {
	struct mddev		*mddev;
<<<<<<< HEAD
	struct mirror_info	*mirrors;	/* twice 'raid_disks' to
=======
	struct raid1_info	*mirrors;	/* twice 'raid_disks' to
>>>>>>> refs/remotes/origin/master
						 * allow for replacements.
						 */
	int			raid_disks;

<<<<<<< HEAD
	/* When choose the best device for a read (read_balance())
	 * we try to keep sequential reads one the same device
	 * using 'last_used' and 'next_seq_sect'
	 */
	int			last_used;
	sector_t		next_seq_sect;
=======
>>>>>>> refs/remotes/origin/master
	/* During resync, read_balancing is only allowed on the part
	 * of the array that has been resynced.  'next_resync' tells us
	 * where that is.
	 */
	sector_t		next_resync;

<<<<<<< HEAD
=======
	/* When raid1 starts resync, we divide array into four partitions
	 * |---------|--------------|---------------------|-------------|
	 *        next_resync   start_next_window       end_window
	 * start_next_window = next_resync + NEXT_NORMALIO_DISTANCE
	 * end_window = start_next_window + NEXT_NORMALIO_DISTANCE
	 * current_window_requests means the count of normalIO between
	 *   start_next_window and end_window.
	 * next_window_requests means the count of normalIO after end_window.
	 * */
	sector_t		start_next_window;
	int			current_window_requests;
	int			next_window_requests;

>>>>>>> refs/remotes/origin/master
	spinlock_t		device_lock;

	/* list of 'struct r1bio' that need to be processed by raid1d,
	 * whether to retry a read, writeout a resync or recovery
	 * block, or anything else.
	 */
	struct list_head	retry_list;

	/* queue pending writes to be submitted on unplug */
	struct bio_list		pending_bio_list;
	int			pending_count;

	/* for use when syncing mirrors:
	 * We don't allow both normal IO and resync/recovery IO at
	 * the same time - resync/recovery can only happen when there
	 * is no other IO.  So when either is active, the other has to wait.
	 * See more details description in raid1.c near raise_barrier().
	 */
	wait_queue_head_t	wait_barrier;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	spinlock_t		resync_lock;
	int			nr_pending;
	int			nr_waiting;
	int			nr_queued;
	int			barrier;
<<<<<<< HEAD
<<<<<<< HEAD
	sector_t		next_resync;
	int			fullsync;  /* set to 1 if a full sync is needed,
					    * (fresh device added).
					    * Cleared when a sync completes.
					    */

	wait_queue_head_t	wait_barrier;

	struct pool_info	*poolinfo;

	struct page		*tmppage;

	mempool_t *r1bio_pool;
	mempool_t *r1buf_pool;
=======
=======
	int			array_frozen;
>>>>>>> refs/remotes/origin/master

	/* Set to 1 if a full sync is needed, (fresh device added).
	 * Cleared when a sync completes.
	 */
	int			fullsync;

	/* When the same as mddev->recovery_disabled we don't allow
	 * recovery to be attempted as we expect a read error.
	 */
	int			recovery_disabled;


	/* poolinfo contains information about the content of the
	 * mempools - it changes when the array grows or shrinks
	 */
	struct pool_info	*poolinfo;
	mempool_t		*r1bio_pool;
	mempool_t		*r1buf_pool;

	/* temporary buffer to synchronous IO when attempting to repair
	 * a read error.
	 */
	struct page		*tmppage;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	/* When taking over an array from a different personality, we store
	 * the new thread here until we fully activate the array.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	struct mdk_thread_s	*thread;
};

typedef struct r1_private_data_s conf_t;

=======
	struct md_thread	*thread;
};

>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct md_thread	*thread;
};

>>>>>>> refs/remotes/origin/master
/*
 * this is our 'private' RAID1 bio.
 *
 * it contains information about what kind of IO operations were started
 * for this RAID1 operation, and about their status:
 */

<<<<<<< HEAD
<<<<<<< HEAD
struct r1bio_s {
=======
struct r1bio {
>>>>>>> refs/remotes/origin/cm-10.0
=======
struct r1bio {
>>>>>>> refs/remotes/origin/master
	atomic_t		remaining; /* 'have we finished' count,
					    * used from IRQ handlers
					    */
	atomic_t		behind_remaining; /* number of write-behind ios remaining
						 * in this BehindIO request
						 */
	sector_t		sector;
<<<<<<< HEAD
	int			sectors;
	unsigned long		state;
<<<<<<< HEAD
	mddev_t			*mddev;
=======
	struct mddev		*mddev;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	sector_t		start_next_window;
	int			sectors;
	unsigned long		state;
	struct mddev		*mddev;
>>>>>>> refs/remotes/origin/master
	/*
	 * original bio going to /dev/mdx
	 */
	struct bio		*master_bio;
	/*
	 * if the IO is in READ direction, then this is where we read
	 */
	int			read_disk;

	struct list_head	retry_list;
	/* Next two are only valid when R1BIO_BehindIO is set */
<<<<<<< HEAD
<<<<<<< HEAD
	struct page		**behind_pages;
=======
	struct bio_vec		*behind_bvecs;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct bio_vec		*behind_bvecs;
>>>>>>> refs/remotes/origin/master
	int			behind_page_count;
	/*
	 * if the IO is in WRITE direction, then multiple bios are used.
	 * We choose the number when they are allocated.
	 */
	struct bio		*bios[0];
	/* DO NOT PUT ANY NEW FIELDS HERE - bios array is contiguously alloced*/
};

<<<<<<< HEAD
/* when we get a read error on a read-only array, we redirect to another
 * device without failing the first device, or trying to over-write to
 * correct the read error.  To keep track of bad blocks on a per-bio
 * level, we store IO_BLOCKED in the appropriate 'bios' pointer
 */
<<<<<<< HEAD
#define IO_BLOCKED ((struct bio*)1)
=======
#define IO_BLOCKED ((struct bio *)1)
/* When we successfully write to a known bad-block, we need to remove the
 * bad-block marking which must be done from process context.  So we record
 * the success by setting bios[n] to IO_MADE_GOOD
 */
#define IO_MADE_GOOD ((struct bio *)2)

#define BIO_SPECIAL(bio) ((unsigned long)bio <= 2)
>>>>>>> refs/remotes/origin/cm-10.0

=======
>>>>>>> refs/remotes/origin/master
/* bits for r1bio.state */
#define	R1BIO_Uptodate	0
#define	R1BIO_IsSync	1
#define	R1BIO_Degraded	2
#define	R1BIO_BehindIO	3
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
/* Set ReadError on bios that experience a readerror so that
 * raid1d knows what to do with them.
 */
#define R1BIO_ReadError 4
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/* For write-behind requests, we call bi_end_io when
 * the last non-write-behind device completes, providing
 * any write was successful.  Otherwise we call when
 * any write-behind write succeeds, otherwise we call
 * with failure when last write completes (and all failed).
 * Record that bi_end_io was called with this flag...
 */
#define	R1BIO_Returned 6
<<<<<<< HEAD
<<<<<<< HEAD

extern int md_raid1_congested(mddev_t *mddev, int bits);
=======
=======
>>>>>>> refs/remotes/origin/master
/* If a write for this request means we can clear some
 * known-bad-block records, we set this flag
 */
#define	R1BIO_MadeGood 7
#define	R1BIO_WriteError 8

extern int md_raid1_congested(struct mddev *mddev, int bits);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#endif
