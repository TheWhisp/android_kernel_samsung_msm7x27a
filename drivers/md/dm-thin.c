/*
<<<<<<< HEAD
 * Copyright (C) 2011 Red Hat UK.
=======
 * Copyright (C) 2011-2012 Red Hat UK.
>>>>>>> refs/remotes/origin/master
 *
 * This file is released under the GPL.
 */

#include "dm-thin-metadata.h"
<<<<<<< HEAD
=======
#include "dm-bio-prison.h"
#include "dm.h"
>>>>>>> refs/remotes/origin/master

#include <linux/device-mapper.h>
#include <linux/dm-io.h>
#include <linux/dm-kcopyd.h>
#include <linux/list.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>

#define	DM_MSG_PREFIX	"thin"

/*
 * Tunable constants
 */
#define ENDIO_HOOK_POOL_SIZE 1024
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
#define DEFERRED_SET_SIZE 64
=======
>>>>>>> refs/remotes/origin/master
#define MAPPING_POOL_SIZE 1024
#define PRISON_CELLS 1024
#define COMMIT_PERIOD HZ

<<<<<<< HEAD
=======
DECLARE_DM_KCOPYD_THROTTLE_WITH_MODULE_PARM(snapshot_copy_throttle,
		"A percentage of time allocated for copy on write");

>>>>>>> refs/remotes/origin/master
/*
 * The block size of the device holding pool data must be
 * between 64KB and 1GB.
 */
#define DATA_DEV_BLOCK_SIZE_MIN_SECTORS (64 * 1024 >> SECTOR_SHIFT)
#define DATA_DEV_BLOCK_SIZE_MAX_SECTORS (1024 * 1024 * 1024 >> SECTOR_SHIFT)

/*
 * Device id is restricted to 24 bits.
 */
#define MAX_DEV_ID ((1 << 24) - 1)

/*
 * How do we handle breaking sharing of data blocks?
 * =================================================
 *
 * We use a standard copy-on-write btree to store the mappings for the
 * devices (note I'm talking about copy-on-write of the metadata here, not
 * the data).  When you take an internal snapshot you clone the root node
 * of the origin btree.  After this there is no concept of an origin or a
 * snapshot.  They are just two device trees that happen to point to the
 * same data blocks.
 *
 * When we get a write in we decide if it's to a shared data block using
 * some timestamp magic.  If it is, we have to break sharing.
 *
 * Let's say we write to a shared block in what was the origin.  The
 * steps are:
 *
 * i) plug io further to this physical block. (see bio_prison code).
 *
 * ii) quiesce any read io to that shared data block.  Obviously
<<<<<<< HEAD
 * including all devices that share this block.  (see deferred_set code)
=======
 * including all devices that share this block.  (see dm_deferred_set code)
>>>>>>> refs/remotes/origin/master
 *
 * iii) copy the data block to a newly allocate block.  This step can be
 * missed out if the io covers the block. (schedule_copy).
 *
 * iv) insert the new mapping into the origin's btree
 * (process_prepared_mapping).  This act of inserting breaks some
 * sharing of btree nodes between the two devices.  Breaking sharing only
 * effects the btree of that specific device.  Btrees for the other
 * devices that share the block never change.  The btree for the origin
 * device as it was after the last commit is untouched, ie. we're using
 * persistent data structures in the functional programming sense.
 *
 * v) unplug io to this physical block, including the io that triggered
 * the breaking of sharing.
 *
 * Steps (ii) and (iii) occur in parallel.
 *
 * The metadata _doesn't_ need to be committed before the io continues.  We
 * get away with this because the io is always written to a _new_ block.
 * If there's a crash, then:
 *
 * - The origin mapping will point to the old origin block (the shared
 * one).  This will contain the data as it was before the io that triggered
 * the breaking of sharing came in.
 *
 * - The snap mapping still points to the old block.  As it would after
 * the commit.
 *
 * The downside of this scheme is the timestamp magic isn't perfect, and
 * will continue to think that data block in the snapshot device is shared
 * even after the write to the origin has broken sharing.  I suspect data
 * blocks will typically be shared by many different devices, so we're
 * breaking sharing n + 1 times, rather than n, where n is the number of
 * devices that reference this data block.  At the moment I think the
 * benefits far, far outweigh the disadvantages.
 */

/*----------------------------------------------------------------*/

/*
<<<<<<< HEAD
 * Sometimes we can't deal with a bio straight away.  We put them in prison
 * where they can't cause any mischief.  Bios are put in a cell identified
 * by a key, multiple bios can be in the same cell.  When the cell is
 * subsequently unlocked the bios become available.
 */
struct bio_prison;

struct cell_key {
	int virtual;
	dm_thin_id dev;
	dm_block_t block;
};

struct cell {
	struct hlist_node list;
	struct bio_prison *prison;
	struct cell_key key;
	struct bio *holder;
	struct bio_list bios;
};

struct bio_prison {
	spinlock_t lock;
	mempool_t *cell_pool;

	unsigned nr_buckets;
	unsigned hash_mask;
	struct hlist_head *cells;
};

static uint32_t calc_nr_buckets(unsigned nr_cells)
{
	uint32_t n = 128;

	nr_cells /= 4;
	nr_cells = min(nr_cells, 8192u);

	while (n < nr_cells)
		n <<= 1;

	return n;
}

/*
 * @nr_cells should be the number of cells you want in use _concurrently_.
 * Don't confuse it with the number of distinct keys.
 */
static struct bio_prison *prison_create(unsigned nr_cells)
{
	unsigned i;
	uint32_t nr_buckets = calc_nr_buckets(nr_cells);
	size_t len = sizeof(struct bio_prison) +
		(sizeof(struct hlist_head) * nr_buckets);
	struct bio_prison *prison = kmalloc(len, GFP_KERNEL);

	if (!prison)
		return NULL;

	spin_lock_init(&prison->lock);
	prison->cell_pool = mempool_create_kmalloc_pool(nr_cells,
							sizeof(struct cell));
	if (!prison->cell_pool) {
		kfree(prison);
		return NULL;
	}

	prison->nr_buckets = nr_buckets;
	prison->hash_mask = nr_buckets - 1;
	prison->cells = (struct hlist_head *) (prison + 1);
	for (i = 0; i < nr_buckets; i++)
		INIT_HLIST_HEAD(prison->cells + i);

	return prison;
}

static void prison_destroy(struct bio_prison *prison)
{
	mempool_destroy(prison->cell_pool);
	kfree(prison);
}

static uint32_t hash_key(struct bio_prison *prison, struct cell_key *key)
{
	const unsigned long BIG_PRIME = 4294967291UL;
	uint64_t hash = key->block * BIG_PRIME;

	return (uint32_t) (hash & prison->hash_mask);
}

static int keys_equal(struct cell_key *lhs, struct cell_key *rhs)
{
	       return (lhs->virtual == rhs->virtual) &&
		       (lhs->dev == rhs->dev) &&
		       (lhs->block == rhs->block);
}

static struct cell *__search_bucket(struct hlist_head *bucket,
				    struct cell_key *key)
{
	struct cell *cell;
	struct hlist_node *tmp;

	hlist_for_each_entry(cell, tmp, bucket, list)
		if (keys_equal(&cell->key, key))
			return cell;

	return NULL;
}

/*
 * This may block if a new cell needs allocating.  You must ensure that
 * cells will be unlocked even if the calling thread is blocked.
 *
 * Returns 1 if the cell was already held, 0 if @inmate is the new holder.
 */
static int bio_detain(struct bio_prison *prison, struct cell_key *key,
		      struct bio *inmate, struct cell **ref)
{
	int r = 1;
	unsigned long flags;
	uint32_t hash = hash_key(prison, key);
	struct cell *cell, *cell2;

	BUG_ON(hash > prison->nr_buckets);

	spin_lock_irqsave(&prison->lock, flags);

	cell = __search_bucket(prison->cells + hash, key);
	if (cell) {
		bio_list_add(&cell->bios, inmate);
		goto out;
	}

	/*
	 * Allocate a new cell
	 */
	spin_unlock_irqrestore(&prison->lock, flags);
	cell2 = mempool_alloc(prison->cell_pool, GFP_NOIO);
	spin_lock_irqsave(&prison->lock, flags);

	/*
	 * We've been unlocked, so we have to double check that
	 * nobody else has inserted this cell in the meantime.
	 */
	cell = __search_bucket(prison->cells + hash, key);
	if (cell) {
		mempool_free(cell2, prison->cell_pool);
		bio_list_add(&cell->bios, inmate);
		goto out;
	}

	/*
	 * Use new cell.
	 */
	cell = cell2;

	cell->prison = prison;
	memcpy(&cell->key, key, sizeof(cell->key));
	cell->holder = inmate;
	bio_list_init(&cell->bios);
	hlist_add_head(&cell->list, prison->cells + hash);

	r = 0;

out:
	spin_unlock_irqrestore(&prison->lock, flags);

	*ref = cell;

	return r;
}

/*
 * @inmates must have been initialised prior to this call
 */
static void __cell_release(struct cell *cell, struct bio_list *inmates)
{
	struct bio_prison *prison = cell->prison;

	hlist_del(&cell->list);

	if (inmates) {
		bio_list_add(inmates, cell->holder);
		bio_list_merge(inmates, &cell->bios);
	}

	mempool_free(cell, prison->cell_pool);
}

static void cell_release(struct cell *cell, struct bio_list *bios)
{
	unsigned long flags;
	struct bio_prison *prison = cell->prison;

	spin_lock_irqsave(&prison->lock, flags);
	__cell_release(cell, bios);
	spin_unlock_irqrestore(&prison->lock, flags);
}

/*
 * There are a couple of places where we put a bio into a cell briefly
 * before taking it out again.  In these situations we know that no other
 * bio may be in the cell.  This function releases the cell, and also does
 * a sanity check.
 */
static void __cell_release_singleton(struct cell *cell, struct bio *bio)
{
	BUG_ON(cell->holder != bio);
	BUG_ON(!bio_list_empty(&cell->bios));

	__cell_release(cell, NULL);
}

static void cell_release_singleton(struct cell *cell, struct bio *bio)
{
	unsigned long flags;
	struct bio_prison *prison = cell->prison;

	spin_lock_irqsave(&prison->lock, flags);
	__cell_release_singleton(cell, bio);
	spin_unlock_irqrestore(&prison->lock, flags);
}

/*
 * Sometimes we don't want the holder, just the additional bios.
 */
static void __cell_release_no_holder(struct cell *cell, struct bio_list *inmates)
{
	struct bio_prison *prison = cell->prison;

	hlist_del(&cell->list);
	bio_list_merge(inmates, &cell->bios);

	mempool_free(cell, prison->cell_pool);
}

static void cell_release_no_holder(struct cell *cell, struct bio_list *inmates)
{
	unsigned long flags;
	struct bio_prison *prison = cell->prison;

	spin_lock_irqsave(&prison->lock, flags);
	__cell_release_no_holder(cell, inmates);
	spin_unlock_irqrestore(&prison->lock, flags);
}

static void cell_error(struct cell *cell)
{
	struct bio_prison *prison = cell->prison;
	struct bio_list bios;
	struct bio *bio;
	unsigned long flags;

	bio_list_init(&bios);

	spin_lock_irqsave(&prison->lock, flags);
	__cell_release(cell, &bios);
	spin_unlock_irqrestore(&prison->lock, flags);

	while ((bio = bio_list_pop(&bios)))
		bio_io_error(bio);
}

/*----------------------------------------------------------------*/

/*
 * We use the deferred set to keep track of pending reads to shared blocks.
 * We do this to ensure the new mapping caused by a write isn't performed
 * until these prior reads have completed.  Otherwise the insertion of the
 * new mapping could free the old block that the read bios are mapped to.
 */

struct deferred_set;
struct deferred_entry {
	struct deferred_set *ds;
	unsigned count;
	struct list_head work_items;
};

struct deferred_set {
	spinlock_t lock;
	unsigned current_entry;
	unsigned sweeper;
	struct deferred_entry entries[DEFERRED_SET_SIZE];
};

static void ds_init(struct deferred_set *ds)
{
	int i;

	spin_lock_init(&ds->lock);
	ds->current_entry = 0;
	ds->sweeper = 0;
	for (i = 0; i < DEFERRED_SET_SIZE; i++) {
		ds->entries[i].ds = ds;
		ds->entries[i].count = 0;
		INIT_LIST_HEAD(&ds->entries[i].work_items);
	}
}

static struct deferred_entry *ds_inc(struct deferred_set *ds)
{
	unsigned long flags;
	struct deferred_entry *entry;

	spin_lock_irqsave(&ds->lock, flags);
	entry = ds->entries + ds->current_entry;
	entry->count++;
	spin_unlock_irqrestore(&ds->lock, flags);

	return entry;
}

static unsigned ds_next(unsigned index)
{
	return (index + 1) % DEFERRED_SET_SIZE;
}

static void __sweep(struct deferred_set *ds, struct list_head *head)
{
	while ((ds->sweeper != ds->current_entry) &&
	       !ds->entries[ds->sweeper].count) {
		list_splice_init(&ds->entries[ds->sweeper].work_items, head);
		ds->sweeper = ds_next(ds->sweeper);
	}

	if ((ds->sweeper == ds->current_entry) && !ds->entries[ds->sweeper].count)
		list_splice_init(&ds->entries[ds->sweeper].work_items, head);
}

static void ds_dec(struct deferred_entry *entry, struct list_head *head)
{
	unsigned long flags;

	spin_lock_irqsave(&entry->ds->lock, flags);
	BUG_ON(!entry->count);
	--entry->count;
	__sweep(entry->ds, head);
	spin_unlock_irqrestore(&entry->ds->lock, flags);
}

/*
 * Returns 1 if deferred or 0 if no pending items to delay job.
 */
static int ds_add_work(struct deferred_set *ds, struct list_head *work)
{
	int r = 1;
	unsigned long flags;
	unsigned next_entry;

	spin_lock_irqsave(&ds->lock, flags);
	if ((ds->sweeper == ds->current_entry) &&
	    !ds->entries[ds->current_entry].count)
		r = 0;
	else {
		list_add(work, &ds->entries[ds->current_entry].work_items);
		next_entry = ds_next(ds->current_entry);
		if (!ds->entries[next_entry].count)
			ds->current_entry = next_entry;
	}
	spin_unlock_irqrestore(&ds->lock, flags);

	return r;
}

/*----------------------------------------------------------------*/

/*
 * Key building.
 */
static void build_data_key(struct dm_thin_device *td,
			   dm_block_t b, struct cell_key *key)
=======
 * Key building.
 */
static void build_data_key(struct dm_thin_device *td,
			   dm_block_t b, struct dm_cell_key *key)
>>>>>>> refs/remotes/origin/master
{
	key->virtual = 0;
	key->dev = dm_thin_dev_id(td);
	key->block = b;
}

static void build_virtual_key(struct dm_thin_device *td, dm_block_t b,
<<<<<<< HEAD
			      struct cell_key *key)
=======
			      struct dm_cell_key *key)
>>>>>>> refs/remotes/origin/master
{
	key->virtual = 1;
	key->dev = dm_thin_dev_id(td);
	key->block = b;
}

/*----------------------------------------------------------------*/

/*
 * A pool device ties together a metadata device and a data device.  It
 * also provides the interface for creating and destroying internal
 * devices.
 */
<<<<<<< HEAD
struct new_mapping;

struct pool_features {
	unsigned zero_new_blocks:1;
	unsigned discard_enabled:1;
	unsigned discard_passdown:1;
};

=======
struct dm_thin_new_mapping;

/*
 * The pool runs in 3 modes.  Ordered in degraded order for comparisons.
 */
enum pool_mode {
	PM_WRITE,		/* metadata may be changed */
	PM_READ_ONLY,		/* metadata may not be changed */
	PM_FAIL,		/* all I/O fails */
};

struct pool_features {
	enum pool_mode mode;

	bool zero_new_blocks:1;
	bool discard_enabled:1;
	bool discard_passdown:1;
};

struct thin_c;
typedef void (*process_bio_fn)(struct thin_c *tc, struct bio *bio);
typedef void (*process_mapping_fn)(struct dm_thin_new_mapping *m);

>>>>>>> refs/remotes/origin/master
struct pool {
	struct list_head list;
	struct dm_target *ti;	/* Only set if a pool target is bound */

	struct mapped_device *pool_md;
	struct block_device *md_dev;
	struct dm_pool_metadata *pmd;

<<<<<<< HEAD
	uint32_t sectors_per_block;
	unsigned block_shift;
	dm_block_t offset_mask;
	dm_block_t low_water_blocks;
=======
	dm_block_t low_water_blocks;
	uint32_t sectors_per_block;
	int sectors_per_block_shift;
>>>>>>> refs/remotes/origin/master

	struct pool_features pf;
	unsigned low_water_triggered:1;	/* A dm event has been sent */
	unsigned no_free_space:1;	/* A -ENOSPC warning has been issued */

<<<<<<< HEAD
	struct bio_prison *prison;
=======
	struct dm_bio_prison *prison;
>>>>>>> refs/remotes/origin/master
	struct dm_kcopyd_client *copier;

	struct workqueue_struct *wq;
	struct work_struct worker;
	struct delayed_work waker;

<<<<<<< HEAD
	unsigned ref_count;
	unsigned long last_commit_jiffies;
=======
	unsigned long last_commit_jiffies;
	unsigned ref_count;
>>>>>>> refs/remotes/origin/master

	spinlock_t lock;
	struct bio_list deferred_bios;
	struct bio_list deferred_flush_bios;
	struct list_head prepared_mappings;
	struct list_head prepared_discards;

	struct bio_list retry_on_resume_list;

<<<<<<< HEAD
	struct deferred_set shared_read_ds;
	struct deferred_set all_io_ds;

	struct new_mapping *next_mapping;
	mempool_t *mapping_pool;
	mempool_t *endio_hook_pool;
};

=======
	struct dm_deferred_set *shared_read_ds;
	struct dm_deferred_set *all_io_ds;

	struct dm_thin_new_mapping *next_mapping;
	mempool_t *mapping_pool;

	process_bio_fn process_bio;
	process_bio_fn process_discard;

	process_mapping_fn process_prepared_mapping;
	process_mapping_fn process_prepared_discard;
};

static enum pool_mode get_pool_mode(struct pool *pool);
static void set_pool_mode(struct pool *pool, enum pool_mode mode);

>>>>>>> refs/remotes/origin/master
/*
 * Target context for a pool.
 */
struct pool_c {
	struct dm_target *ti;
	struct pool *pool;
	struct dm_dev *data_dev;
	struct dm_dev *metadata_dev;
	struct dm_target_callbacks callbacks;

	dm_block_t low_water_blocks;
<<<<<<< HEAD
	struct pool_features pf;
=======
	struct pool_features requested_pf; /* Features requested during table load */
	struct pool_features adjusted_pf;  /* Features used after adjusting for constituent devices */
>>>>>>> refs/remotes/origin/master
};

/*
 * Target context for a thin.
 */
struct thin_c {
	struct dm_dev *pool_dev;
	struct dm_dev *origin_dev;
	dm_thin_id dev_id;

	struct pool *pool;
	struct dm_thin_device *td;
};

/*----------------------------------------------------------------*/

/*
<<<<<<< HEAD
=======
 * wake_worker() is used when new work is queued and when pool_resume is
 * ready to continue deferred IO processing.
 */
static void wake_worker(struct pool *pool)
{
	queue_work(pool->wq, &pool->worker);
}

/*----------------------------------------------------------------*/

static int bio_detain(struct pool *pool, struct dm_cell_key *key, struct bio *bio,
		      struct dm_bio_prison_cell **cell_result)
{
	int r;
	struct dm_bio_prison_cell *cell_prealloc;

	/*
	 * Allocate a cell from the prison's mempool.
	 * This might block but it can't fail.
	 */
	cell_prealloc = dm_bio_prison_alloc_cell(pool->prison, GFP_NOIO);

	r = dm_bio_detain(pool->prison, key, bio, cell_prealloc, cell_result);
	if (r)
		/*
		 * We reused an old cell; we can get rid of
		 * the new one.
		 */
		dm_bio_prison_free_cell(pool->prison, cell_prealloc);

	return r;
}

static void cell_release(struct pool *pool,
			 struct dm_bio_prison_cell *cell,
			 struct bio_list *bios)
{
	dm_cell_release(pool->prison, cell, bios);
	dm_bio_prison_free_cell(pool->prison, cell);
}

static void cell_release_no_holder(struct pool *pool,
				   struct dm_bio_prison_cell *cell,
				   struct bio_list *bios)
{
	dm_cell_release_no_holder(pool->prison, cell, bios);
	dm_bio_prison_free_cell(pool->prison, cell);
}

static void cell_defer_no_holder_no_free(struct thin_c *tc,
					 struct dm_bio_prison_cell *cell)
{
	struct pool *pool = tc->pool;
	unsigned long flags;

	spin_lock_irqsave(&pool->lock, flags);
	dm_cell_release_no_holder(pool->prison, cell, &pool->deferred_bios);
	spin_unlock_irqrestore(&pool->lock, flags);

	wake_worker(pool);
}

static void cell_error(struct pool *pool,
		       struct dm_bio_prison_cell *cell)
{
	dm_cell_error(pool->prison, cell);
	dm_bio_prison_free_cell(pool->prison, cell);
}

/*----------------------------------------------------------------*/

/*
>>>>>>> refs/remotes/origin/master
 * A global list of pools that uses a struct mapped_device as a key.
 */
static struct dm_thin_pool_table {
	struct mutex mutex;
	struct list_head pools;
} dm_thin_pool_table;

static void pool_table_init(void)
{
	mutex_init(&dm_thin_pool_table.mutex);
	INIT_LIST_HEAD(&dm_thin_pool_table.pools);
}

static void __pool_table_insert(struct pool *pool)
{
	BUG_ON(!mutex_is_locked(&dm_thin_pool_table.mutex));
	list_add(&pool->list, &dm_thin_pool_table.pools);
}

static void __pool_table_remove(struct pool *pool)
{
	BUG_ON(!mutex_is_locked(&dm_thin_pool_table.mutex));
	list_del(&pool->list);
}

static struct pool *__pool_table_lookup(struct mapped_device *md)
{
	struct pool *pool = NULL, *tmp;

	BUG_ON(!mutex_is_locked(&dm_thin_pool_table.mutex));

	list_for_each_entry(tmp, &dm_thin_pool_table.pools, list) {
		if (tmp->pool_md == md) {
			pool = tmp;
			break;
		}
	}

	return pool;
}

static struct pool *__pool_table_lookup_metadata_dev(struct block_device *md_dev)
{
	struct pool *pool = NULL, *tmp;

	BUG_ON(!mutex_is_locked(&dm_thin_pool_table.mutex));

	list_for_each_entry(tmp, &dm_thin_pool_table.pools, list) {
		if (tmp->md_dev == md_dev) {
			pool = tmp;
			break;
		}
	}

	return pool;
}

/*----------------------------------------------------------------*/

<<<<<<< HEAD
struct endio_hook {
	struct thin_c *tc;
	struct deferred_entry *shared_read_entry;
	struct deferred_entry *all_io_entry;
	struct new_mapping *overwrite_mapping;
=======
struct dm_thin_endio_hook {
	struct thin_c *tc;
	struct dm_deferred_entry *shared_read_entry;
	struct dm_deferred_entry *all_io_entry;
	struct dm_thin_new_mapping *overwrite_mapping;
>>>>>>> refs/remotes/origin/master
};

static void __requeue_bio_list(struct thin_c *tc, struct bio_list *master)
{
	struct bio *bio;
	struct bio_list bios;

	bio_list_init(&bios);
	bio_list_merge(&bios, master);
	bio_list_init(master);

	while ((bio = bio_list_pop(&bios))) {
<<<<<<< HEAD
		struct endio_hook *h = dm_get_mapinfo(bio)->ptr;
=======
		struct dm_thin_endio_hook *h = dm_per_bio_data(bio, sizeof(struct dm_thin_endio_hook));

>>>>>>> refs/remotes/origin/master
		if (h->tc == tc)
			bio_endio(bio, DM_ENDIO_REQUEUE);
		else
			bio_list_add(master, bio);
	}
}

static void requeue_io(struct thin_c *tc)
{
	struct pool *pool = tc->pool;
	unsigned long flags;

	spin_lock_irqsave(&pool->lock, flags);
	__requeue_bio_list(tc, &pool->deferred_bios);
	__requeue_bio_list(tc, &pool->retry_on_resume_list);
	spin_unlock_irqrestore(&pool->lock, flags);
}

/*
 * This section of code contains the logic for processing a thin device's IO.
 * Much of the code depends on pool object resources (lists, workqueues, etc)
 * but most is exclusively called from the thin target rather than the thin-pool
 * target.
 */

<<<<<<< HEAD
static dm_block_t get_bio_block(struct thin_c *tc, struct bio *bio)
{
	return bio->bi_sector >> tc->pool->block_shift;
=======
static bool block_size_is_power_of_two(struct pool *pool)
{
	return pool->sectors_per_block_shift >= 0;
}

static dm_block_t get_bio_block(struct thin_c *tc, struct bio *bio)
{
	struct pool *pool = tc->pool;
	sector_t block_nr = bio->bi_sector;

	if (block_size_is_power_of_two(pool))
		block_nr >>= pool->sectors_per_block_shift;
	else
		(void) sector_div(block_nr, pool->sectors_per_block);

	return block_nr;
>>>>>>> refs/remotes/origin/master
}

static void remap(struct thin_c *tc, struct bio *bio, dm_block_t block)
{
	struct pool *pool = tc->pool;
<<<<<<< HEAD

	bio->bi_bdev = tc->pool_dev->bdev;
	bio->bi_sector = (block << pool->block_shift) +
		(bio->bi_sector & pool->offset_mask);
=======
	sector_t bi_sector = bio->bi_sector;

	bio->bi_bdev = tc->pool_dev->bdev;
	if (block_size_is_power_of_two(pool))
		bio->bi_sector = (block << pool->sectors_per_block_shift) |
				(bi_sector & (pool->sectors_per_block - 1));
	else
		bio->bi_sector = (block * pool->sectors_per_block) +
				 sector_div(bi_sector, pool->sectors_per_block);
>>>>>>> refs/remotes/origin/master
}

static void remap_to_origin(struct thin_c *tc, struct bio *bio)
{
	bio->bi_bdev = tc->origin_dev->bdev;
}

<<<<<<< HEAD
=======
static int bio_triggers_commit(struct thin_c *tc, struct bio *bio)
{
	return (bio->bi_rw & (REQ_FLUSH | REQ_FUA)) &&
		dm_thin_changed_this_transaction(tc->td);
}

static void inc_all_io_entry(struct pool *pool, struct bio *bio)
{
	struct dm_thin_endio_hook *h;

	if (bio->bi_rw & REQ_DISCARD)
		return;

	h = dm_per_bio_data(bio, sizeof(struct dm_thin_endio_hook));
	h->all_io_entry = dm_deferred_entry_inc(pool->all_io_ds);
}

>>>>>>> refs/remotes/origin/master
static void issue(struct thin_c *tc, struct bio *bio)
{
	struct pool *pool = tc->pool;
	unsigned long flags;

<<<<<<< HEAD
	/*
	 * Batch together any FUA/FLUSH bios we find and then issue
	 * a single commit for them in process_deferred_bios().
	 */
	if (bio->bi_rw & (REQ_FLUSH | REQ_FUA)) {
		spin_lock_irqsave(&pool->lock, flags);
		bio_list_add(&pool->deferred_flush_bios, bio);
		spin_unlock_irqrestore(&pool->lock, flags);
	} else
		generic_make_request(bio);
=======
	if (!bio_triggers_commit(tc, bio)) {
		generic_make_request(bio);
		return;
	}

	/*
	 * Complete bio with an error if earlier I/O caused changes to
	 * the metadata that can't be committed e.g, due to I/O errors
	 * on the metadata device.
	 */
	if (dm_thin_aborted_changes(tc->td)) {
		bio_io_error(bio);
		return;
	}

	/*
	 * Batch together any bios that trigger commits and then issue a
	 * single commit for them in process_deferred_bios().
	 */
	spin_lock_irqsave(&pool->lock, flags);
	bio_list_add(&pool->deferred_flush_bios, bio);
	spin_unlock_irqrestore(&pool->lock, flags);
>>>>>>> refs/remotes/origin/master
}

static void remap_to_origin_and_issue(struct thin_c *tc, struct bio *bio)
{
	remap_to_origin(tc, bio);
<<<<<<< HEAD
	issue(tc, bio);
}

static void remap_and_issue(struct thin_c *tc, struct bio *bio,
			    dm_block_t block)
{
	remap(tc, bio, block);
	issue(tc, bio);
}

/*
 * wake_worker() is used when new work is queued and when pool_resume is
 * ready to continue deferred IO processing.
 */
static void wake_worker(struct pool *pool)
{
	queue_work(pool->wq, &pool->worker);
=======
	issue(tc, bio);
}

static void remap_and_issue(struct thin_c *tc, struct bio *bio,
			    dm_block_t block)
{
	remap(tc, bio, block);
	issue(tc, bio);
>>>>>>> refs/remotes/origin/master
}

/*----------------------------------------------------------------*/

/*
 * Bio endio functions.
 */
<<<<<<< HEAD
struct new_mapping {
=======
struct dm_thin_new_mapping {
>>>>>>> refs/remotes/origin/master
	struct list_head list;

	unsigned quiesced:1;
	unsigned prepared:1;
	unsigned pass_discard:1;

	struct thin_c *tc;
	dm_block_t virt_block;
	dm_block_t data_block;
<<<<<<< HEAD
	struct cell *cell, *cell2;
=======
	struct dm_bio_prison_cell *cell, *cell2;
>>>>>>> refs/remotes/origin/master
	int err;

	/*
	 * If the bio covers the whole area of a block then we can avoid
	 * zeroing or copying.  Instead this bio is hooked.  The bio will
	 * still be in the cell, so care has to be taken to avoid issuing
	 * the bio twice.
	 */
	struct bio *bio;
	bio_end_io_t *saved_bi_end_io;
};

<<<<<<< HEAD
static void __maybe_add_mapping(struct new_mapping *m)
=======
static void __maybe_add_mapping(struct dm_thin_new_mapping *m)
>>>>>>> refs/remotes/origin/master
{
	struct pool *pool = m->tc->pool;

	if (m->quiesced && m->prepared) {
		list_add(&m->list, &pool->prepared_mappings);
		wake_worker(pool);
	}
}

static void copy_complete(int read_err, unsigned long write_err, void *context)
{
	unsigned long flags;
<<<<<<< HEAD
	struct new_mapping *m = context;
=======
	struct dm_thin_new_mapping *m = context;
>>>>>>> refs/remotes/origin/master
	struct pool *pool = m->tc->pool;

	m->err = read_err || write_err ? -EIO : 0;

	spin_lock_irqsave(&pool->lock, flags);
	m->prepared = 1;
	__maybe_add_mapping(m);
	spin_unlock_irqrestore(&pool->lock, flags);
}

static void overwrite_endio(struct bio *bio, int err)
{
	unsigned long flags;
<<<<<<< HEAD
	struct endio_hook *h = dm_get_mapinfo(bio)->ptr;
	struct new_mapping *m = h->overwrite_mapping;
=======
	struct dm_thin_endio_hook *h = dm_per_bio_data(bio, sizeof(struct dm_thin_endio_hook));
	struct dm_thin_new_mapping *m = h->overwrite_mapping;
>>>>>>> refs/remotes/origin/master
	struct pool *pool = m->tc->pool;

	m->err = err;

	spin_lock_irqsave(&pool->lock, flags);
	m->prepared = 1;
	__maybe_add_mapping(m);
	spin_unlock_irqrestore(&pool->lock, flags);
}

/*----------------------------------------------------------------*/

/*
 * Workqueue.
 */

/*
 * Prepared mapping jobs.
 */

/*
 * This sends the bios in the cell back to the deferred_bios list.
 */
<<<<<<< HEAD
static void cell_defer(struct thin_c *tc, struct cell *cell,
		       dm_block_t data_block)
=======
static void cell_defer(struct thin_c *tc, struct dm_bio_prison_cell *cell)
>>>>>>> refs/remotes/origin/master
{
	struct pool *pool = tc->pool;
	unsigned long flags;

	spin_lock_irqsave(&pool->lock, flags);
<<<<<<< HEAD
	cell_release(cell, &pool->deferred_bios);
=======
	cell_release(pool, cell, &pool->deferred_bios);
>>>>>>> refs/remotes/origin/master
	spin_unlock_irqrestore(&tc->pool->lock, flags);

	wake_worker(pool);
}

/*
<<<<<<< HEAD
 * Same as cell_defer above, except it omits one particular detainee,
 * a write bio that covers the block and has already been processed.
 */
static void cell_defer_except(struct thin_c *tc, struct cell *cell)
{
	struct bio_list bios;
	struct pool *pool = tc->pool;
	unsigned long flags;

	bio_list_init(&bios);

	spin_lock_irqsave(&pool->lock, flags);
	cell_release_no_holder(cell, &pool->deferred_bios);
=======
 * Same as cell_defer above, except it omits the original holder of the cell.
 */
static void cell_defer_no_holder(struct thin_c *tc, struct dm_bio_prison_cell *cell)
{
	struct pool *pool = tc->pool;
	unsigned long flags;

	spin_lock_irqsave(&pool->lock, flags);
	cell_release_no_holder(pool, cell, &pool->deferred_bios);
>>>>>>> refs/remotes/origin/master
	spin_unlock_irqrestore(&pool->lock, flags);

	wake_worker(pool);
}

<<<<<<< HEAD
static void process_prepared_mapping(struct new_mapping *m)
{
	struct thin_c *tc = m->tc;
=======
static void process_prepared_mapping_fail(struct dm_thin_new_mapping *m)
{
	if (m->bio)
		m->bio->bi_end_io = m->saved_bi_end_io;
	cell_error(m->tc->pool, m->cell);
	list_del(&m->list);
	mempool_free(m, m->tc->pool->mapping_pool);
}

static void process_prepared_mapping(struct dm_thin_new_mapping *m)
{
	struct thin_c *tc = m->tc;
	struct pool *pool = tc->pool;
>>>>>>> refs/remotes/origin/master
	struct bio *bio;
	int r;

	bio = m->bio;
	if (bio)
		bio->bi_end_io = m->saved_bi_end_io;

	if (m->err) {
<<<<<<< HEAD
		cell_error(m->cell);
<<<<<<< HEAD
=======
		cell_error(pool, m->cell);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
		goto out;
	}

	/*
	 * Commit the prepared block into the mapping btree.
	 * Any I/O for this block arriving after this point will get
	 * remapped to it directly.
	 */
	r = dm_thin_insert_block(tc->td, m->virt_block, m->data_block);
	if (r) {
<<<<<<< HEAD
		DMERR("dm_thin_insert_block() failed");
		cell_error(m->cell);
<<<<<<< HEAD
=======
		DMERR_LIMIT("%s: dm_thin_insert_block() failed: error = %d",
			    dm_device_name(pool->pool_md), r);
		set_pool_mode(pool, PM_READ_ONLY);
		cell_error(pool, m->cell);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
		goto out;
	}

	/*
	 * Release any bios held while the block was being provisioned.
	 * If we are processing a write bio that completely covers the block,
	 * we already processed it so can ignore it now when processing
	 * the bios in the cell.
	 */
	if (bio) {
<<<<<<< HEAD
		cell_defer_except(tc, m->cell);
		bio_endio(bio, 0);
	} else
		cell_defer(tc, m->cell, m->data_block);

out:
	list_del(&m->list);
	mempool_free(m, tc->pool->mapping_pool);
}

static void process_prepared_discard(struct new_mapping *m)
{
	int r;
	struct thin_c *tc = m->tc;

	r = dm_thin_remove_block(tc->td, m->virt_block);
	if (r)
		DMERR("dm_thin_remove_block() failed");

	/*
	 * Pass the discard down to the underlying device?
	 */
=======
		cell_defer_no_holder(tc, m->cell);
		bio_endio(bio, 0);
	} else
		cell_defer(tc, m->cell);

out:
	list_del(&m->list);
	mempool_free(m, pool->mapping_pool);
}

static void process_prepared_discard_fail(struct dm_thin_new_mapping *m)
{
	struct thin_c *tc = m->tc;

	bio_io_error(m->bio);
	cell_defer_no_holder(tc, m->cell);
	cell_defer_no_holder(tc, m->cell2);
	mempool_free(m, tc->pool->mapping_pool);
}

static void process_prepared_discard_passdown(struct dm_thin_new_mapping *m)
{
	struct thin_c *tc = m->tc;

	inc_all_io_entry(tc->pool, m->bio);
	cell_defer_no_holder(tc, m->cell);
	cell_defer_no_holder(tc, m->cell2);

>>>>>>> refs/remotes/origin/master
	if (m->pass_discard)
		remap_and_issue(tc, m->bio, m->data_block);
	else
		bio_endio(m->bio, 0);

<<<<<<< HEAD
	cell_defer_except(tc, m->cell);
	cell_defer_except(tc, m->cell2);
	mempool_free(m, tc->pool->mapping_pool);
}

static void process_prepared(struct pool *pool, struct list_head *head,
			     void (*fn)(struct new_mapping *))
{
	unsigned long flags;
	struct list_head maps;
	struct new_mapping *m, *tmp;
=======
	mempool_free(m, tc->pool->mapping_pool);
}

static void process_prepared_discard(struct dm_thin_new_mapping *m)
{
	int r;
	struct thin_c *tc = m->tc;

	r = dm_thin_remove_block(tc->td, m->virt_block);
	if (r)
		DMERR_LIMIT("dm_thin_remove_block() failed");

	process_prepared_discard_passdown(m);
}

static void process_prepared(struct pool *pool, struct list_head *head,
			     process_mapping_fn *fn)
{
	unsigned long flags;
	struct list_head maps;
	struct dm_thin_new_mapping *m, *tmp;
>>>>>>> refs/remotes/origin/master

	INIT_LIST_HEAD(&maps);
	spin_lock_irqsave(&pool->lock, flags);
	list_splice_init(head, &maps);
	spin_unlock_irqrestore(&pool->lock, flags);

	list_for_each_entry_safe(m, tmp, &maps, list)
<<<<<<< HEAD
		fn(m);
=======
		(*fn)(m);
>>>>>>> refs/remotes/origin/master
}

/*
 * Deferred bio jobs.
 */
static int io_overlaps_block(struct pool *pool, struct bio *bio)
{
<<<<<<< HEAD
	return !(bio->bi_sector & pool->offset_mask) &&
		(bio->bi_size == (pool->sectors_per_block << SECTOR_SHIFT));

=======
	return bio->bi_size == (pool->sectors_per_block << SECTOR_SHIFT);
>>>>>>> refs/remotes/origin/master
}

static int io_overwrites_block(struct pool *pool, struct bio *bio)
{
	return (bio_data_dir(bio) == WRITE) &&
		io_overlaps_block(pool, bio);
}

static void save_and_set_endio(struct bio *bio, bio_end_io_t **save,
			       bio_end_io_t *fn)
{
	*save = bio->bi_end_io;
	bio->bi_end_io = fn;
}

static int ensure_next_mapping(struct pool *pool)
{
	if (pool->next_mapping)
		return 0;

	pool->next_mapping = mempool_alloc(pool->mapping_pool, GFP_ATOMIC);

	return pool->next_mapping ? 0 : -ENOMEM;
}

<<<<<<< HEAD
static struct new_mapping *get_next_mapping(struct pool *pool)
{
	struct new_mapping *r = pool->next_mapping;
=======
static struct dm_thin_new_mapping *get_next_mapping(struct pool *pool)
{
	struct dm_thin_new_mapping *r = pool->next_mapping;
>>>>>>> refs/remotes/origin/master

	BUG_ON(!pool->next_mapping);

	pool->next_mapping = NULL;

	return r;
}

static void schedule_copy(struct thin_c *tc, dm_block_t virt_block,
			  struct dm_dev *origin, dm_block_t data_origin,
			  dm_block_t data_dest,
<<<<<<< HEAD
			  struct cell *cell, struct bio *bio)
{
	int r;
	struct pool *pool = tc->pool;
	struct new_mapping *m = get_next_mapping(pool);
=======
			  struct dm_bio_prison_cell *cell, struct bio *bio)
{
	int r;
	struct pool *pool = tc->pool;
	struct dm_thin_new_mapping *m = get_next_mapping(pool);
>>>>>>> refs/remotes/origin/master

	INIT_LIST_HEAD(&m->list);
	m->quiesced = 0;
	m->prepared = 0;
	m->tc = tc;
	m->virt_block = virt_block;
	m->data_block = data_dest;
	m->cell = cell;
	m->err = 0;
	m->bio = NULL;

<<<<<<< HEAD
	if (!ds_add_work(&pool->shared_read_ds, &m->list))
=======
	if (!dm_deferred_set_add_work(pool->shared_read_ds, &m->list))
>>>>>>> refs/remotes/origin/master
		m->quiesced = 1;

	/*
	 * IO to pool_dev remaps to the pool target's data_dev.
	 *
	 * If the whole block of data is being overwritten, we can issue the
	 * bio immediately. Otherwise we use kcopyd to clone the data first.
	 */
	if (io_overwrites_block(pool, bio)) {
<<<<<<< HEAD
		struct endio_hook *h = dm_get_mapinfo(bio)->ptr;
		h->overwrite_mapping = m;
		m->bio = bio;
		save_and_set_endio(bio, &m->saved_bi_end_io, overwrite_endio);
=======
		struct dm_thin_endio_hook *h = dm_per_bio_data(bio, sizeof(struct dm_thin_endio_hook));

		h->overwrite_mapping = m;
		m->bio = bio;
		save_and_set_endio(bio, &m->saved_bi_end_io, overwrite_endio);
		inc_all_io_entry(pool, bio);
>>>>>>> refs/remotes/origin/master
		remap_and_issue(tc, bio, data_dest);
	} else {
		struct dm_io_region from, to;

		from.bdev = origin->bdev;
		from.sector = data_origin * pool->sectors_per_block;
		from.count = pool->sectors_per_block;

		to.bdev = tc->pool_dev->bdev;
		to.sector = data_dest * pool->sectors_per_block;
		to.count = pool->sectors_per_block;

		r = dm_kcopyd_copy(pool->copier, &from, 1, &to,
				   0, copy_complete, m);
		if (r < 0) {
			mempool_free(m, pool->mapping_pool);
<<<<<<< HEAD
			DMERR("dm_kcopyd_copy() failed");
			cell_error(cell);
=======
			DMERR_LIMIT("dm_kcopyd_copy() failed");
			cell_error(pool, cell);
>>>>>>> refs/remotes/origin/master
		}
	}
}

static void schedule_internal_copy(struct thin_c *tc, dm_block_t virt_block,
				   dm_block_t data_origin, dm_block_t data_dest,
<<<<<<< HEAD
				   struct cell *cell, struct bio *bio)
=======
				   struct dm_bio_prison_cell *cell, struct bio *bio)
>>>>>>> refs/remotes/origin/master
{
	schedule_copy(tc, virt_block, tc->pool_dev,
		      data_origin, data_dest, cell, bio);
}

static void schedule_external_copy(struct thin_c *tc, dm_block_t virt_block,
				   dm_block_t data_dest,
<<<<<<< HEAD
				   struct cell *cell, struct bio *bio)
=======
				   struct dm_bio_prison_cell *cell, struct bio *bio)
>>>>>>> refs/remotes/origin/master
{
	schedule_copy(tc, virt_block, tc->origin_dev,
		      virt_block, data_dest, cell, bio);
}

static void schedule_zero(struct thin_c *tc, dm_block_t virt_block,
<<<<<<< HEAD
			  dm_block_t data_block, struct cell *cell,
			  struct bio *bio)
{
	struct pool *pool = tc->pool;
	struct new_mapping *m = get_next_mapping(pool);
=======
			  dm_block_t data_block, struct dm_bio_prison_cell *cell,
			  struct bio *bio)
{
	struct pool *pool = tc->pool;
	struct dm_thin_new_mapping *m = get_next_mapping(pool);
>>>>>>> refs/remotes/origin/master

	INIT_LIST_HEAD(&m->list);
	m->quiesced = 1;
	m->prepared = 0;
	m->tc = tc;
	m->virt_block = virt_block;
	m->data_block = data_block;
	m->cell = cell;
	m->err = 0;
	m->bio = NULL;

	/*
	 * If the whole block of data is being overwritten or we are not
	 * zeroing pre-existing data, we can issue the bio immediately.
	 * Otherwise we use kcopyd to zero the data first.
	 */
	if (!pool->pf.zero_new_blocks)
		process_prepared_mapping(m);

	else if (io_overwrites_block(pool, bio)) {
<<<<<<< HEAD
		struct endio_hook *h = dm_get_mapinfo(bio)->ptr;
		h->overwrite_mapping = m;
		m->bio = bio;
		save_and_set_endio(bio, &m->saved_bi_end_io, overwrite_endio);
		remap_and_issue(tc, bio, data_block);

=======
		struct dm_thin_endio_hook *h = dm_per_bio_data(bio, sizeof(struct dm_thin_endio_hook));

		h->overwrite_mapping = m;
		m->bio = bio;
		save_and_set_endio(bio, &m->saved_bi_end_io, overwrite_endio);
		inc_all_io_entry(pool, bio);
		remap_and_issue(tc, bio, data_block);
>>>>>>> refs/remotes/origin/master
	} else {
		int r;
		struct dm_io_region to;

		to.bdev = tc->pool_dev->bdev;
		to.sector = data_block * pool->sectors_per_block;
		to.count = pool->sectors_per_block;

		r = dm_kcopyd_zero(pool->copier, 1, &to, 0, copy_complete, m);
		if (r < 0) {
			mempool_free(m, pool->mapping_pool);
<<<<<<< HEAD
			DMERR("dm_kcopyd_zero() failed");
			cell_error(cell);
=======
			DMERR_LIMIT("dm_kcopyd_zero() failed");
			cell_error(pool, cell);
>>>>>>> refs/remotes/origin/master
		}
	}
}

<<<<<<< HEAD
=======
/*
 * A non-zero return indicates read_only or fail_io mode.
 * Many callers don't care about the return value.
 */
static int commit(struct pool *pool)
{
	int r;

	if (get_pool_mode(pool) != PM_WRITE)
		return -EINVAL;

	r = dm_pool_commit_metadata(pool->pmd);
	if (r) {
		DMERR_LIMIT("%s: dm_pool_commit_metadata failed: error = %d",
			    dm_device_name(pool->pool_md), r);
		set_pool_mode(pool, PM_READ_ONLY);
	}

	return r;
}

>>>>>>> refs/remotes/origin/master
static int alloc_data_block(struct thin_c *tc, dm_block_t *result)
{
	int r;
	dm_block_t free_blocks;
	unsigned long flags;
	struct pool *pool = tc->pool;

<<<<<<< HEAD
=======
	/*
	 * Once no_free_space is set we must not allow allocation to succeed.
	 * Otherwise it is difficult to explain, debug, test and support.
	 */
	if (pool->no_free_space)
		return -ENOSPC;

>>>>>>> refs/remotes/origin/master
	r = dm_pool_get_free_block_count(pool->pmd, &free_blocks);
	if (r)
		return r;

	if (free_blocks <= pool->low_water_blocks && !pool->low_water_triggered) {
<<<<<<< HEAD
		DMWARN("%s: reached low water mark, sending event.",
=======
		DMWARN("%s: reached low water mark for data device: sending event.",
>>>>>>> refs/remotes/origin/master
		       dm_device_name(pool->pool_md));
		spin_lock_irqsave(&pool->lock, flags);
		pool->low_water_triggered = 1;
		spin_unlock_irqrestore(&pool->lock, flags);
		dm_table_event(pool->ti->table);
	}

	if (!free_blocks) {
<<<<<<< HEAD
		if (pool->no_free_space)
			return -ENOSPC;
		else {
			/*
			 * Try to commit to see if that will free up some
			 * more space.
			 */
			r = dm_pool_commit_metadata(pool->pmd);
			if (r) {
				DMERR("%s: dm_pool_commit_metadata() failed, error = %d",
				      __func__, r);
				return r;
			}

			r = dm_pool_get_free_block_count(pool->pmd, &free_blocks);
			if (r)
				return r;

			/*
			 * If we still have no space we set a flag to avoid
			 * doing all this checking and return -ENOSPC.
			 */
			if (!free_blocks) {
				DMWARN("%s: no free space available.",
				       dm_device_name(pool->pool_md));
				spin_lock_irqsave(&pool->lock, flags);
				pool->no_free_space = 1;
				spin_unlock_irqrestore(&pool->lock, flags);
				return -ENOSPC;
			}
=======
		/*
		 * Try to commit to see if that will free up some
		 * more space.
		 */
		r = commit(pool);
		if (r)
			return r;

		r = dm_pool_get_free_block_count(pool->pmd, &free_blocks);
		if (r)
			return r;

		/*
		 * If we still have no space we set a flag to avoid
		 * doing all this checking and return -ENOSPC.  This
		 * flag serves as a latch that disallows allocations from
		 * this pool until the admin takes action (e.g. resize or
		 * table reload).
		 */
		if (!free_blocks) {
			DMWARN("%s: no free data space available.",
			       dm_device_name(pool->pool_md));
			spin_lock_irqsave(&pool->lock, flags);
			pool->no_free_space = 1;
			spin_unlock_irqrestore(&pool->lock, flags);
			return -ENOSPC;
>>>>>>> refs/remotes/origin/master
		}
	}

	r = dm_pool_alloc_data_block(pool->pmd, result);
<<<<<<< HEAD
	if (r)
		return r;
=======
	if (r) {
		if (r == -ENOSPC &&
		    !dm_pool_get_free_metadata_block_count(pool->pmd, &free_blocks) &&
		    !free_blocks) {
			DMWARN("%s: no free metadata space available.",
			       dm_device_name(pool->pool_md));
			set_pool_mode(pool, PM_READ_ONLY);
		}
		return r;
	}
>>>>>>> refs/remotes/origin/master

	return 0;
}

/*
 * If we have run out of space, queue bios until the device is
 * resumed, presumably after having been reloaded with more space.
 */
static void retry_on_resume(struct bio *bio)
{
<<<<<<< HEAD
	struct endio_hook *h = dm_get_mapinfo(bio)->ptr;
=======
	struct dm_thin_endio_hook *h = dm_per_bio_data(bio, sizeof(struct dm_thin_endio_hook));
>>>>>>> refs/remotes/origin/master
	struct thin_c *tc = h->tc;
	struct pool *pool = tc->pool;
	unsigned long flags;

	spin_lock_irqsave(&pool->lock, flags);
	bio_list_add(&pool->retry_on_resume_list, bio);
	spin_unlock_irqrestore(&pool->lock, flags);
}

<<<<<<< HEAD
static void no_space(struct cell *cell)
=======
static void no_space(struct pool *pool, struct dm_bio_prison_cell *cell)
>>>>>>> refs/remotes/origin/master
{
	struct bio *bio;
	struct bio_list bios;

	bio_list_init(&bios);
<<<<<<< HEAD
	cell_release(cell, &bios);
=======
	cell_release(pool, cell, &bios);
>>>>>>> refs/remotes/origin/master

	while ((bio = bio_list_pop(&bios)))
		retry_on_resume(bio);
}

static void process_discard(struct thin_c *tc, struct bio *bio)
{
	int r;
	unsigned long flags;
	struct pool *pool = tc->pool;
<<<<<<< HEAD
	struct cell *cell, *cell2;
	struct cell_key key, key2;
	dm_block_t block = get_bio_block(tc, bio);
	struct dm_thin_lookup_result lookup_result;
	struct new_mapping *m;

	build_virtual_key(tc->td, block, &key);
	if (bio_detain(tc->pool->prison, &key, bio, &cell))
=======
	struct dm_bio_prison_cell *cell, *cell2;
	struct dm_cell_key key, key2;
	dm_block_t block = get_bio_block(tc, bio);
	struct dm_thin_lookup_result lookup_result;
	struct dm_thin_new_mapping *m;

	build_virtual_key(tc->td, block, &key);
	if (bio_detain(tc->pool, &key, bio, &cell))
>>>>>>> refs/remotes/origin/master
		return;

	r = dm_thin_find_block(tc->td, block, 1, &lookup_result);
	switch (r) {
	case 0:
		/*
		 * Check nobody is fiddling with this pool block.  This can
		 * happen if someone's in the process of breaking sharing
		 * on this block.
		 */
		build_data_key(tc->td, lookup_result.block, &key2);
<<<<<<< HEAD
		if (bio_detain(tc->pool->prison, &key2, bio, &cell2)) {
			cell_release_singleton(cell, bio);
=======
		if (bio_detain(tc->pool, &key2, bio, &cell2)) {
			cell_defer_no_holder(tc, cell);
>>>>>>> refs/remotes/origin/master
			break;
		}

		if (io_overlaps_block(pool, bio)) {
			/*
			 * IO may still be going to the destination block.  We must
			 * quiesce before we can do the removal.
			 */
			m = get_next_mapping(pool);
			m->tc = tc;
<<<<<<< HEAD
			m->pass_discard = (!lookup_result.shared) & pool->pf.discard_passdown;
=======
			m->pass_discard = (!lookup_result.shared) && pool->pf.discard_passdown;
>>>>>>> refs/remotes/origin/master
			m->virt_block = block;
			m->data_block = lookup_result.block;
			m->cell = cell;
			m->cell2 = cell2;
			m->err = 0;
			m->bio = bio;

<<<<<<< HEAD
			if (!ds_add_work(&pool->all_io_ds, &m->list)) {
=======
			if (!dm_deferred_set_add_work(pool->all_io_ds, &m->list)) {
>>>>>>> refs/remotes/origin/master
				spin_lock_irqsave(&pool->lock, flags);
				list_add(&m->list, &pool->prepared_discards);
				spin_unlock_irqrestore(&pool->lock, flags);
				wake_worker(pool);
			}
		} else {
<<<<<<< HEAD
			/*
			 * This path is hit if people are ignoring
			 * limits->discard_granularity.  It ignores any
			 * part of the discard that is in a subsequent
			 * block.
			 */
			sector_t offset = bio->bi_sector - (block << pool->block_shift);
			unsigned remaining = (pool->sectors_per_block - offset) << 9;
			bio->bi_size = min(bio->bi_size, remaining);

			cell_release_singleton(cell, bio);
			cell_release_singleton(cell2, bio);
<<<<<<< HEAD
=======
			inc_all_io_entry(pool, bio);
			cell_defer_no_holder(tc, cell);
			cell_defer_no_holder(tc, cell2);

			/*
			 * The DM core makes sure that the discard doesn't span
			 * a block boundary.  So we submit the discard of a
			 * partial block appropriately.
			 */
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
			if ((!lookup_result.shared) && pool->pf.discard_passdown)
				remap_and_issue(tc, bio, lookup_result.block);
			else
				bio_endio(bio, 0);
		}
		break;

	case -ENODATA:
		/*
		 * It isn't provisioned, just forget it.
		 */
<<<<<<< HEAD
		cell_release_singleton(cell, bio);
=======
		cell_defer_no_holder(tc, cell);
>>>>>>> refs/remotes/origin/master
		bio_endio(bio, 0);
		break;

	default:
<<<<<<< HEAD
		DMERR("discard: find block unexpectedly returned %d", r);
		cell_release_singleton(cell, bio);
=======
		DMERR_LIMIT("%s: dm_thin_find_block() failed: error = %d",
			    __func__, r);
		cell_defer_no_holder(tc, cell);
>>>>>>> refs/remotes/origin/master
		bio_io_error(bio);
		break;
	}
}

static void break_sharing(struct thin_c *tc, struct bio *bio, dm_block_t block,
<<<<<<< HEAD
			  struct cell_key *key,
			  struct dm_thin_lookup_result *lookup_result,
			  struct cell *cell)
{
	int r;
	dm_block_t data_block;
=======
			  struct dm_cell_key *key,
			  struct dm_thin_lookup_result *lookup_result,
			  struct dm_bio_prison_cell *cell)
{
	int r;
	dm_block_t data_block;
	struct pool *pool = tc->pool;
>>>>>>> refs/remotes/origin/master

	r = alloc_data_block(tc, &data_block);
	switch (r) {
	case 0:
		schedule_internal_copy(tc, block, lookup_result->block,
				       data_block, cell, bio);
		break;

	case -ENOSPC:
<<<<<<< HEAD
		no_space(cell);
		break;

	default:
		DMERR("%s: alloc_data_block() failed, error = %d", __func__, r);
		cell_error(cell);
=======
		no_space(pool, cell);
		break;

	default:
		DMERR_LIMIT("%s: alloc_data_block() failed: error = %d",
			    __func__, r);
		set_pool_mode(pool, PM_READ_ONLY);
		cell_error(pool, cell);
>>>>>>> refs/remotes/origin/master
		break;
	}
}

static void process_shared_bio(struct thin_c *tc, struct bio *bio,
			       dm_block_t block,
			       struct dm_thin_lookup_result *lookup_result)
{
<<<<<<< HEAD
	struct cell *cell;
	struct pool *pool = tc->pool;
	struct cell_key key;
=======
	struct dm_bio_prison_cell *cell;
	struct pool *pool = tc->pool;
	struct dm_cell_key key;
>>>>>>> refs/remotes/origin/master

	/*
	 * If cell is already occupied, then sharing is already in the process
	 * of being broken so we have nothing further to do here.
	 */
	build_data_key(tc->td, lookup_result->block, &key);
<<<<<<< HEAD
	if (bio_detain(pool->prison, &key, bio, &cell))
		return;

	if (bio_data_dir(bio) == WRITE)
		break_sharing(tc, bio, block, &key, lookup_result, cell);
	else {
		struct endio_hook *h = dm_get_mapinfo(bio)->ptr;

		h->shared_read_entry = ds_inc(&pool->shared_read_ds);

		cell_release_singleton(cell, bio);
=======
	if (bio_detain(pool, &key, bio, &cell))
		return;

	if (bio_data_dir(bio) == WRITE && bio->bi_size)
		break_sharing(tc, bio, block, &key, lookup_result, cell);
	else {
		struct dm_thin_endio_hook *h = dm_per_bio_data(bio, sizeof(struct dm_thin_endio_hook));

		h->shared_read_entry = dm_deferred_entry_inc(pool->shared_read_ds);
		inc_all_io_entry(pool, bio);
		cell_defer_no_holder(tc, cell);

>>>>>>> refs/remotes/origin/master
		remap_and_issue(tc, bio, lookup_result->block);
	}
}

static void provision_block(struct thin_c *tc, struct bio *bio, dm_block_t block,
<<<<<<< HEAD
			    struct cell *cell)
{
	int r;
	dm_block_t data_block;
=======
			    struct dm_bio_prison_cell *cell)
{
	int r;
	dm_block_t data_block;
	struct pool *pool = tc->pool;
>>>>>>> refs/remotes/origin/master

	/*
	 * Remap empty bios (flushes) immediately, without provisioning.
	 */
	if (!bio->bi_size) {
<<<<<<< HEAD
		cell_release_singleton(cell, bio);
=======
		inc_all_io_entry(pool, bio);
		cell_defer_no_holder(tc, cell);

>>>>>>> refs/remotes/origin/master
		remap_and_issue(tc, bio, 0);
		return;
	}

	/*
	 * Fill read bios with zeroes and complete them immediately.
	 */
	if (bio_data_dir(bio) == READ) {
		zero_fill_bio(bio);
<<<<<<< HEAD
		cell_release_singleton(cell, bio);
=======
		cell_defer_no_holder(tc, cell);
>>>>>>> refs/remotes/origin/master
		bio_endio(bio, 0);
		return;
	}

	r = alloc_data_block(tc, &data_block);
	switch (r) {
	case 0:
		if (tc->origin_dev)
			schedule_external_copy(tc, block, data_block, cell, bio);
		else
			schedule_zero(tc, block, data_block, cell, bio);
		break;

	case -ENOSPC:
<<<<<<< HEAD
		no_space(cell);
		break;

	default:
		DMERR("%s: alloc_data_block() failed, error = %d", __func__, r);
		cell_error(cell);
=======
		no_space(pool, cell);
		break;

	default:
		DMERR_LIMIT("%s: alloc_data_block() failed: error = %d",
			    __func__, r);
		set_pool_mode(pool, PM_READ_ONLY);
		cell_error(pool, cell);
>>>>>>> refs/remotes/origin/master
		break;
	}
}

static void process_bio(struct thin_c *tc, struct bio *bio)
{
	int r;
<<<<<<< HEAD
	dm_block_t block = get_bio_block(tc, bio);
	struct cell *cell;
	struct cell_key key;
=======
	struct pool *pool = tc->pool;
	dm_block_t block = get_bio_block(tc, bio);
	struct dm_bio_prison_cell *cell;
	struct dm_cell_key key;
>>>>>>> refs/remotes/origin/master
	struct dm_thin_lookup_result lookup_result;

	/*
	 * If cell is already occupied, then the block is already
	 * being provisioned so we have nothing further to do here.
	 */
	build_virtual_key(tc->td, block, &key);
<<<<<<< HEAD
	if (bio_detain(tc->pool->prison, &key, bio, &cell))
=======
	if (bio_detain(pool, &key, bio, &cell))
>>>>>>> refs/remotes/origin/master
		return;

	r = dm_thin_find_block(tc->td, block, 1, &lookup_result);
	switch (r) {
	case 0:
<<<<<<< HEAD
		/*
		 * We can release this cell now.  This thread is the only
		 * one that puts bios into a cell, and we know there were
		 * no preceding bios.
		 */
		/*
		 * TODO: this will probably have to change when discard goes
		 * back in.
		 */
		cell_release_singleton(cell, bio);

		if (lookup_result.shared)
			process_shared_bio(tc, bio, block, &lookup_result);
		else
			remap_and_issue(tc, bio, lookup_result.block);
=======
		if (lookup_result.shared) {
			process_shared_bio(tc, bio, block, &lookup_result);
			cell_defer_no_holder(tc, cell); /* FIXME: pass this cell into process_shared? */
		} else {
			inc_all_io_entry(pool, bio);
			cell_defer_no_holder(tc, cell);

			remap_and_issue(tc, bio, lookup_result.block);
		}
>>>>>>> refs/remotes/origin/master
		break;

	case -ENODATA:
		if (bio_data_dir(bio) == READ && tc->origin_dev) {
<<<<<<< HEAD
			cell_release_singleton(cell, bio);
=======
			inc_all_io_entry(pool, bio);
			cell_defer_no_holder(tc, cell);

>>>>>>> refs/remotes/origin/master
			remap_to_origin_and_issue(tc, bio);
		} else
			provision_block(tc, bio, block, cell);
		break;

	default:
<<<<<<< HEAD
		DMERR("dm_thin_find_block() failed, error = %d", r);
		cell_release_singleton(cell, bio);
=======
		DMERR_LIMIT("%s: dm_thin_find_block() failed: error = %d",
			    __func__, r);
		cell_defer_no_holder(tc, cell);
		bio_io_error(bio);
		break;
	}
}

static void process_bio_read_only(struct thin_c *tc, struct bio *bio)
{
	int r;
	int rw = bio_data_dir(bio);
	dm_block_t block = get_bio_block(tc, bio);
	struct dm_thin_lookup_result lookup_result;

	r = dm_thin_find_block(tc->td, block, 1, &lookup_result);
	switch (r) {
	case 0:
		if (lookup_result.shared && (rw == WRITE) && bio->bi_size)
			bio_io_error(bio);
		else {
			inc_all_io_entry(tc->pool, bio);
			remap_and_issue(tc, bio, lookup_result.block);
		}
		break;

	case -ENODATA:
		if (rw != READ) {
			bio_io_error(bio);
			break;
		}

		if (tc->origin_dev) {
			inc_all_io_entry(tc->pool, bio);
			remap_to_origin_and_issue(tc, bio);
			break;
		}

		zero_fill_bio(bio);
		bio_endio(bio, 0);
		break;

	default:
		DMERR_LIMIT("%s: dm_thin_find_block() failed: error = %d",
			    __func__, r);
>>>>>>> refs/remotes/origin/master
		bio_io_error(bio);
		break;
	}
}

<<<<<<< HEAD
=======
static void process_bio_fail(struct thin_c *tc, struct bio *bio)
{
	bio_io_error(bio);
}

/*
 * FIXME: should we also commit due to size of transaction, measured in
 * metadata blocks?
 */
>>>>>>> refs/remotes/origin/master
static int need_commit_due_to_time(struct pool *pool)
{
	return jiffies < pool->last_commit_jiffies ||
	       jiffies > pool->last_commit_jiffies + COMMIT_PERIOD;
}

static void process_deferred_bios(struct pool *pool)
{
	unsigned long flags;
	struct bio *bio;
	struct bio_list bios;
<<<<<<< HEAD
	int r;
=======
>>>>>>> refs/remotes/origin/master

	bio_list_init(&bios);

	spin_lock_irqsave(&pool->lock, flags);
	bio_list_merge(&bios, &pool->deferred_bios);
	bio_list_init(&pool->deferred_bios);
	spin_unlock_irqrestore(&pool->lock, flags);

	while ((bio = bio_list_pop(&bios))) {
<<<<<<< HEAD
		struct endio_hook *h = dm_get_mapinfo(bio)->ptr;
=======
		struct dm_thin_endio_hook *h = dm_per_bio_data(bio, sizeof(struct dm_thin_endio_hook));
>>>>>>> refs/remotes/origin/master
		struct thin_c *tc = h->tc;

		/*
		 * If we've got no free new_mapping structs, and processing
		 * this bio might require one, we pause until there are some
		 * prepared mappings to process.
		 */
		if (ensure_next_mapping(pool)) {
			spin_lock_irqsave(&pool->lock, flags);
			bio_list_merge(&pool->deferred_bios, &bios);
			spin_unlock_irqrestore(&pool->lock, flags);

			break;
		}

		if (bio->bi_rw & REQ_DISCARD)
<<<<<<< HEAD
			process_discard(tc, bio);
		else
			process_bio(tc, bio);
=======
			pool->process_discard(tc, bio);
		else
			pool->process_bio(tc, bio);
>>>>>>> refs/remotes/origin/master
	}

	/*
	 * If there are any deferred flush bios, we must commit
	 * the metadata before issuing them.
	 */
	bio_list_init(&bios);
	spin_lock_irqsave(&pool->lock, flags);
	bio_list_merge(&bios, &pool->deferred_flush_bios);
	bio_list_init(&pool->deferred_flush_bios);
	spin_unlock_irqrestore(&pool->lock, flags);

	if (bio_list_empty(&bios) && !need_commit_due_to_time(pool))
		return;

<<<<<<< HEAD
	r = dm_pool_commit_metadata(pool->pmd);
	if (r) {
		DMERR("%s: dm_pool_commit_metadata() failed, error = %d",
		      __func__, r);
=======
	if (commit(pool)) {
>>>>>>> refs/remotes/origin/master
		while ((bio = bio_list_pop(&bios)))
			bio_io_error(bio);
		return;
	}
	pool->last_commit_jiffies = jiffies;

	while ((bio = bio_list_pop(&bios)))
		generic_make_request(bio);
}

static void do_worker(struct work_struct *ws)
{
	struct pool *pool = container_of(ws, struct pool, worker);

<<<<<<< HEAD
	process_prepared(pool, &pool->prepared_mappings, process_prepared_mapping);
	process_prepared(pool, &pool->prepared_discards, process_prepared_discard);
=======
	process_prepared(pool, &pool->prepared_mappings, &pool->process_prepared_mapping);
	process_prepared(pool, &pool->prepared_discards, &pool->process_prepared_discard);
>>>>>>> refs/remotes/origin/master
	process_deferred_bios(pool);
}

/*
 * We want to commit periodically so that not too much
 * unwritten data builds up.
 */
static void do_waker(struct work_struct *ws)
{
	struct pool *pool = container_of(to_delayed_work(ws), struct pool, waker);
	wake_worker(pool);
	queue_delayed_work(pool->wq, &pool->waker, COMMIT_PERIOD);
}

/*----------------------------------------------------------------*/

<<<<<<< HEAD
=======
static enum pool_mode get_pool_mode(struct pool *pool)
{
	return pool->pf.mode;
}

static void set_pool_mode(struct pool *pool, enum pool_mode mode)
{
	int r;

	pool->pf.mode = mode;

	switch (mode) {
	case PM_FAIL:
		DMERR("%s: switching pool to failure mode",
		      dm_device_name(pool->pool_md));
		dm_pool_metadata_read_only(pool->pmd);
		pool->process_bio = process_bio_fail;
		pool->process_discard = process_bio_fail;
		pool->process_prepared_mapping = process_prepared_mapping_fail;
		pool->process_prepared_discard = process_prepared_discard_fail;
		break;

	case PM_READ_ONLY:
		DMERR("%s: switching pool to read-only mode",
		      dm_device_name(pool->pool_md));
		r = dm_pool_abort_metadata(pool->pmd);
		if (r) {
			DMERR("%s: aborting transaction failed",
			      dm_device_name(pool->pool_md));
			set_pool_mode(pool, PM_FAIL);
		} else {
			dm_pool_metadata_read_only(pool->pmd);
			pool->process_bio = process_bio_read_only;
			pool->process_discard = process_discard;
			pool->process_prepared_mapping = process_prepared_mapping_fail;
			pool->process_prepared_discard = process_prepared_discard_passdown;
		}
		break;

	case PM_WRITE:
		dm_pool_metadata_read_write(pool->pmd);
		pool->process_bio = process_bio;
		pool->process_discard = process_discard;
		pool->process_prepared_mapping = process_prepared_mapping;
		pool->process_prepared_discard = process_prepared_discard;
		break;
	}
}

/*----------------------------------------------------------------*/

>>>>>>> refs/remotes/origin/master
/*
 * Mapping functions.
 */

/*
 * Called only while mapping a thin bio to hand it over to the workqueue.
 */
static void thin_defer_bio(struct thin_c *tc, struct bio *bio)
{
	unsigned long flags;
	struct pool *pool = tc->pool;

	spin_lock_irqsave(&pool->lock, flags);
	bio_list_add(&pool->deferred_bios, bio);
	spin_unlock_irqrestore(&pool->lock, flags);

	wake_worker(pool);
}

<<<<<<< HEAD
static struct endio_hook *thin_hook_bio(struct thin_c *tc, struct bio *bio)
{
	struct pool *pool = tc->pool;
	struct endio_hook *h = mempool_alloc(pool->endio_hook_pool, GFP_NOIO);

	h->tc = tc;
	h->shared_read_entry = NULL;
	h->all_io_entry = bio->bi_rw & REQ_DISCARD ? NULL : ds_inc(&pool->all_io_ds);
	h->overwrite_mapping = NULL;

	return h;
=======
static void thin_hook_bio(struct thin_c *tc, struct bio *bio)
{
	struct dm_thin_endio_hook *h = dm_per_bio_data(bio, sizeof(struct dm_thin_endio_hook));

	h->tc = tc;
	h->shared_read_entry = NULL;
	h->all_io_entry = NULL;
	h->overwrite_mapping = NULL;
>>>>>>> refs/remotes/origin/master
}

/*
 * Non-blocking function called from the thin target's map function.
 */
<<<<<<< HEAD
static int thin_bio_map(struct dm_target *ti, struct bio *bio,
			union map_info *map_context)
=======
static int thin_bio_map(struct dm_target *ti, struct bio *bio)
>>>>>>> refs/remotes/origin/master
{
	int r;
	struct thin_c *tc = ti->private;
	dm_block_t block = get_bio_block(tc, bio);
	struct dm_thin_device *td = tc->td;
	struct dm_thin_lookup_result result;
<<<<<<< HEAD

	map_context->ptr = thin_hook_bio(tc, bio);
=======
	struct dm_bio_prison_cell cell1, cell2;
	struct dm_bio_prison_cell *cell_result;
	struct dm_cell_key key;

	thin_hook_bio(tc, bio);

	if (get_pool_mode(tc->pool) == PM_FAIL) {
		bio_io_error(bio);
		return DM_MAPIO_SUBMITTED;
	}

>>>>>>> refs/remotes/origin/master
	if (bio->bi_rw & (REQ_DISCARD | REQ_FLUSH | REQ_FUA)) {
		thin_defer_bio(tc, bio);
		return DM_MAPIO_SUBMITTED;
	}

	r = dm_thin_find_block(td, block, 0, &result);

	/*
	 * Note that we defer readahead too.
	 */
	switch (r) {
	case 0:
		if (unlikely(result.shared)) {
			/*
			 * We have a race condition here between the
			 * result.shared value returned by the lookup and
			 * snapshot creation, which may cause new
			 * sharing.
			 *
			 * To avoid this always quiesce the origin before
			 * taking the snap.  You want to do this anyway to
			 * ensure a consistent application view
			 * (i.e. lockfs).
			 *
			 * More distant ancestors are irrelevant. The
			 * shared flag will be set in their case.
			 */
			thin_defer_bio(tc, bio);
<<<<<<< HEAD
			r = DM_MAPIO_SUBMITTED;
		} else {
			remap(tc, bio, result.block);
			r = DM_MAPIO_REMAPPED;
		}
		break;

	case -ENODATA:
=======
			return DM_MAPIO_SUBMITTED;
		}

		build_virtual_key(tc->td, block, &key);
		if (dm_bio_detain(tc->pool->prison, &key, bio, &cell1, &cell_result))
			return DM_MAPIO_SUBMITTED;

		build_data_key(tc->td, result.block, &key);
		if (dm_bio_detain(tc->pool->prison, &key, bio, &cell2, &cell_result)) {
			cell_defer_no_holder_no_free(tc, &cell1);
			return DM_MAPIO_SUBMITTED;
		}

		inc_all_io_entry(tc->pool, bio);
		cell_defer_no_holder_no_free(tc, &cell2);
		cell_defer_no_holder_no_free(tc, &cell1);

		remap(tc, bio, result.block);
		return DM_MAPIO_REMAPPED;

	case -ENODATA:
		if (get_pool_mode(tc->pool) == PM_READ_ONLY) {
			/*
			 * This block isn't provisioned, and we have no way
			 * of doing so.  Just error it.
			 */
			bio_io_error(bio);
			return DM_MAPIO_SUBMITTED;
		}
		/* fall through */

	case -EWOULDBLOCK:
>>>>>>> refs/remotes/origin/master
		/*
		 * In future, the failed dm_thin_find_block above could
		 * provide the hint to load the metadata into cache.
		 */
<<<<<<< HEAD
	case -EWOULDBLOCK:
		thin_defer_bio(tc, bio);
		r = DM_MAPIO_SUBMITTED;
		break;
	}

	return r;
=======
		thin_defer_bio(tc, bio);
		return DM_MAPIO_SUBMITTED;

	default:
		/*
		 * Must always call bio_io_error on failure.
		 * dm_thin_find_block can fail with -EINVAL if the
		 * pool is switched to fail-io mode.
		 */
		bio_io_error(bio);
		return DM_MAPIO_SUBMITTED;
	}
>>>>>>> refs/remotes/origin/master
}

static int pool_is_congested(struct dm_target_callbacks *cb, int bdi_bits)
{
	int r;
	unsigned long flags;
	struct pool_c *pt = container_of(cb, struct pool_c, callbacks);

	spin_lock_irqsave(&pt->pool->lock, flags);
	r = !bio_list_empty(&pt->pool->retry_on_resume_list);
	spin_unlock_irqrestore(&pt->pool->lock, flags);

	if (!r) {
		struct request_queue *q = bdev_get_queue(pt->data_dev->bdev);
		r = bdi_congested(&q->backing_dev_info, bdi_bits);
	}

	return r;
}

static void __requeue_bios(struct pool *pool)
{
	bio_list_merge(&pool->deferred_bios, &pool->retry_on_resume_list);
	bio_list_init(&pool->retry_on_resume_list);
}

/*----------------------------------------------------------------
 * Binding of control targets to a pool object
 *--------------------------------------------------------------*/
<<<<<<< HEAD
=======
static bool data_dev_supports_discard(struct pool_c *pt)
{
	struct request_queue *q = bdev_get_queue(pt->data_dev->bdev);

	return q && blk_queue_discard(q);
}

static bool is_factor(sector_t block_size, uint32_t n)
{
	return !sector_div(block_size, n);
}

/*
 * If discard_passdown was enabled verify that the data device
 * supports discards.  Disable discard_passdown if not.
 */
static void disable_passdown_if_not_supported(struct pool_c *pt)
{
	struct pool *pool = pt->pool;
	struct block_device *data_bdev = pt->data_dev->bdev;
	struct queue_limits *data_limits = &bdev_get_queue(data_bdev)->limits;
	sector_t block_size = pool->sectors_per_block << SECTOR_SHIFT;
	const char *reason = NULL;
	char buf[BDEVNAME_SIZE];

	if (!pt->adjusted_pf.discard_passdown)
		return;

	if (!data_dev_supports_discard(pt))
		reason = "discard unsupported";

	else if (data_limits->max_discard_sectors < pool->sectors_per_block)
		reason = "max discard sectors smaller than a block";

	else if (data_limits->discard_granularity > block_size)
		reason = "discard granularity larger than a block";

	else if (!is_factor(block_size, data_limits->discard_granularity))
		reason = "discard granularity not a factor of block size";

	if (reason) {
		DMWARN("Data device (%s) %s: Disabling discard passdown.", bdevname(data_bdev, buf), reason);
		pt->adjusted_pf.discard_passdown = false;
	}
}

>>>>>>> refs/remotes/origin/master
static int bind_control_target(struct pool *pool, struct dm_target *ti)
{
	struct pool_c *pt = ti->private;

<<<<<<< HEAD
	pool->ti = ti;
	pool->low_water_blocks = pt->low_water_blocks;
	pool->pf = pt->pf;

	/*
	 * If discard_passdown was enabled verify that the data device
	 * supports discards.  Disable discard_passdown if not; otherwise
	 * -EOPNOTSUPP will be returned.
	 */
	if (pt->pf.discard_passdown) {
		struct request_queue *q = bdev_get_queue(pt->data_dev->bdev);
		if (!q || !blk_queue_discard(q)) {
			char buf[BDEVNAME_SIZE];
			DMWARN("Discard unsupported by data device (%s): Disabling discard passdown.",
			       bdevname(pt->data_dev->bdev, buf));
			pool->pf.discard_passdown = 0;
		}
	}
=======
	/*
	 * We want to make sure that a pool in PM_FAIL mode is never upgraded.
	 */
	enum pool_mode old_mode = pool->pf.mode;
	enum pool_mode new_mode = pt->adjusted_pf.mode;

	/*
	 * If we were in PM_FAIL mode, rollback of metadata failed.  We're
	 * not going to recover without a thin_repair.  So we never let the
	 * pool move out of the old mode.  On the other hand a PM_READ_ONLY
	 * may have been due to a lack of metadata or data space, and may
	 * now work (ie. if the underlying devices have been resized).
	 */
	if (old_mode == PM_FAIL)
		new_mode = old_mode;

	pool->ti = ti;
	pool->low_water_blocks = pt->low_water_blocks;
	pool->pf = pt->adjusted_pf;

	set_pool_mode(pool, new_mode);
>>>>>>> refs/remotes/origin/master

	return 0;
}

static void unbind_control_target(struct pool *pool, struct dm_target *ti)
{
	if (pool->ti == ti)
		pool->ti = NULL;
}

/*----------------------------------------------------------------
 * Pool creation
 *--------------------------------------------------------------*/
/* Initialize pool features. */
static void pool_features_init(struct pool_features *pf)
{
<<<<<<< HEAD
	pf->zero_new_blocks = 1;
	pf->discard_enabled = 1;
	pf->discard_passdown = 1;
=======
	pf->mode = PM_WRITE;
	pf->zero_new_blocks = true;
	pf->discard_enabled = true;
	pf->discard_passdown = true;
>>>>>>> refs/remotes/origin/master
}

static void __pool_destroy(struct pool *pool)
{
	__pool_table_remove(pool);

	if (dm_pool_metadata_close(pool->pmd) < 0)
		DMWARN("%s: dm_pool_metadata_close() failed.", __func__);

<<<<<<< HEAD
	prison_destroy(pool->prison);
=======
	dm_bio_prison_destroy(pool->prison);
>>>>>>> refs/remotes/origin/master
	dm_kcopyd_client_destroy(pool->copier);

	if (pool->wq)
		destroy_workqueue(pool->wq);

	if (pool->next_mapping)
		mempool_free(pool->next_mapping, pool->mapping_pool);
	mempool_destroy(pool->mapping_pool);
<<<<<<< HEAD
	mempool_destroy(pool->endio_hook_pool);
	kfree(pool);
}

static struct pool *pool_create(struct mapped_device *pool_md,
				struct block_device *metadata_dev,
				unsigned long block_size, char **error)
=======
	dm_deferred_set_destroy(pool->shared_read_ds);
	dm_deferred_set_destroy(pool->all_io_ds);
	kfree(pool);
}

static struct kmem_cache *_new_mapping_cache;

static struct pool *pool_create(struct mapped_device *pool_md,
				struct block_device *metadata_dev,
				unsigned long block_size,
				int read_only, char **error)
>>>>>>> refs/remotes/origin/master
{
	int r;
	void *err_p;
	struct pool *pool;
	struct dm_pool_metadata *pmd;
<<<<<<< HEAD

	pmd = dm_pool_metadata_open(metadata_dev, block_size);
=======
	bool format_device = read_only ? false : true;

	pmd = dm_pool_metadata_open(metadata_dev, block_size, format_device);
>>>>>>> refs/remotes/origin/master
	if (IS_ERR(pmd)) {
		*error = "Error creating metadata object";
		return (struct pool *)pmd;
	}

	pool = kmalloc(sizeof(*pool), GFP_KERNEL);
	if (!pool) {
		*error = "Error allocating memory for pool";
		err_p = ERR_PTR(-ENOMEM);
		goto bad_pool;
	}

	pool->pmd = pmd;
	pool->sectors_per_block = block_size;
<<<<<<< HEAD
	pool->block_shift = ffs(block_size) - 1;
	pool->offset_mask = block_size - 1;
	pool->low_water_blocks = 0;
	pool_features_init(&pool->pf);
	pool->prison = prison_create(PRISON_CELLS);
=======
	if (block_size & (block_size - 1))
		pool->sectors_per_block_shift = -1;
	else
		pool->sectors_per_block_shift = __ffs(block_size);
	pool->low_water_blocks = 0;
	pool_features_init(&pool->pf);
	pool->prison = dm_bio_prison_create(PRISON_CELLS);
>>>>>>> refs/remotes/origin/master
	if (!pool->prison) {
		*error = "Error creating pool's bio prison";
		err_p = ERR_PTR(-ENOMEM);
		goto bad_prison;
	}

<<<<<<< HEAD
	pool->copier = dm_kcopyd_client_create();
=======
	pool->copier = dm_kcopyd_client_create(&dm_kcopyd_throttle);
>>>>>>> refs/remotes/origin/master
	if (IS_ERR(pool->copier)) {
		r = PTR_ERR(pool->copier);
		*error = "Error creating pool's kcopyd client";
		err_p = ERR_PTR(r);
		goto bad_kcopyd_client;
	}

	/*
	 * Create singlethreaded workqueue that will service all devices
	 * that use this metadata.
	 */
	pool->wq = alloc_ordered_workqueue("dm-" DM_MSG_PREFIX, WQ_MEM_RECLAIM);
	if (!pool->wq) {
		*error = "Error creating pool's workqueue";
		err_p = ERR_PTR(-ENOMEM);
		goto bad_wq;
	}

	INIT_WORK(&pool->worker, do_worker);
	INIT_DELAYED_WORK(&pool->waker, do_waker);
	spin_lock_init(&pool->lock);
	bio_list_init(&pool->deferred_bios);
	bio_list_init(&pool->deferred_flush_bios);
	INIT_LIST_HEAD(&pool->prepared_mappings);
	INIT_LIST_HEAD(&pool->prepared_discards);
	pool->low_water_triggered = 0;
	pool->no_free_space = 0;
	bio_list_init(&pool->retry_on_resume_list);
<<<<<<< HEAD
	ds_init(&pool->shared_read_ds);
	ds_init(&pool->all_io_ds);

	pool->next_mapping = NULL;
	pool->mapping_pool =
		mempool_create_kmalloc_pool(MAPPING_POOL_SIZE, sizeof(struct new_mapping));
=======

	pool->shared_read_ds = dm_deferred_set_create();
	if (!pool->shared_read_ds) {
		*error = "Error creating pool's shared read deferred set";
		err_p = ERR_PTR(-ENOMEM);
		goto bad_shared_read_ds;
	}

	pool->all_io_ds = dm_deferred_set_create();
	if (!pool->all_io_ds) {
		*error = "Error creating pool's all io deferred set";
		err_p = ERR_PTR(-ENOMEM);
		goto bad_all_io_ds;
	}

	pool->next_mapping = NULL;
	pool->mapping_pool = mempool_create_slab_pool(MAPPING_POOL_SIZE,
						      _new_mapping_cache);
>>>>>>> refs/remotes/origin/master
	if (!pool->mapping_pool) {
		*error = "Error creating pool's mapping mempool";
		err_p = ERR_PTR(-ENOMEM);
		goto bad_mapping_pool;
	}

<<<<<<< HEAD
	pool->endio_hook_pool =
		mempool_create_kmalloc_pool(ENDIO_HOOK_POOL_SIZE, sizeof(struct endio_hook));
	if (!pool->endio_hook_pool) {
		*error = "Error creating pool's endio_hook mempool";
		err_p = ERR_PTR(-ENOMEM);
		goto bad_endio_hook_pool;
	}
=======
>>>>>>> refs/remotes/origin/master
	pool->ref_count = 1;
	pool->last_commit_jiffies = jiffies;
	pool->pool_md = pool_md;
	pool->md_dev = metadata_dev;
	__pool_table_insert(pool);

	return pool;

<<<<<<< HEAD
bad_endio_hook_pool:
	mempool_destroy(pool->mapping_pool);
bad_mapping_pool:
=======
bad_mapping_pool:
	dm_deferred_set_destroy(pool->all_io_ds);
bad_all_io_ds:
	dm_deferred_set_destroy(pool->shared_read_ds);
bad_shared_read_ds:
>>>>>>> refs/remotes/origin/master
	destroy_workqueue(pool->wq);
bad_wq:
	dm_kcopyd_client_destroy(pool->copier);
bad_kcopyd_client:
<<<<<<< HEAD
	prison_destroy(pool->prison);
=======
	dm_bio_prison_destroy(pool->prison);
>>>>>>> refs/remotes/origin/master
bad_prison:
	kfree(pool);
bad_pool:
	if (dm_pool_metadata_close(pmd))
		DMWARN("%s: dm_pool_metadata_close() failed.", __func__);

	return err_p;
}

static void __pool_inc(struct pool *pool)
{
	BUG_ON(!mutex_is_locked(&dm_thin_pool_table.mutex));
	pool->ref_count++;
}

static void __pool_dec(struct pool *pool)
{
	BUG_ON(!mutex_is_locked(&dm_thin_pool_table.mutex));
	BUG_ON(!pool->ref_count);
	if (!--pool->ref_count)
		__pool_destroy(pool);
}

static struct pool *__pool_find(struct mapped_device *pool_md,
				struct block_device *metadata_dev,
<<<<<<< HEAD
				unsigned long block_size, char **error,
				int *created)
=======
				unsigned long block_size, int read_only,
				char **error, int *created)
>>>>>>> refs/remotes/origin/master
{
	struct pool *pool = __pool_table_lookup_metadata_dev(metadata_dev);

	if (pool) {
<<<<<<< HEAD
		if (pool->pool_md != pool_md)
			return ERR_PTR(-EBUSY);
=======
		if (pool->pool_md != pool_md) {
			*error = "metadata device already in use by a pool";
			return ERR_PTR(-EBUSY);
		}
>>>>>>> refs/remotes/origin/master
		__pool_inc(pool);

	} else {
		pool = __pool_table_lookup(pool_md);
		if (pool) {
<<<<<<< HEAD
			if (pool->md_dev != metadata_dev)
				return ERR_PTR(-EINVAL);
			__pool_inc(pool);

		} else {
			pool = pool_create(pool_md, metadata_dev, block_size, error);
=======
			if (pool->md_dev != metadata_dev) {
				*error = "different pool cannot replace a pool";
				return ERR_PTR(-EINVAL);
			}
			__pool_inc(pool);

		} else {
			pool = pool_create(pool_md, metadata_dev, block_size, read_only, error);
>>>>>>> refs/remotes/origin/master
			*created = 1;
		}
	}

	return pool;
}

/*----------------------------------------------------------------
 * Pool target methods
 *--------------------------------------------------------------*/
static void pool_dtr(struct dm_target *ti)
{
	struct pool_c *pt = ti->private;

	mutex_lock(&dm_thin_pool_table.mutex);

	unbind_control_target(pt->pool, ti);
	__pool_dec(pt->pool);
	dm_put_device(ti, pt->metadata_dev);
	dm_put_device(ti, pt->data_dev);
	kfree(pt);

	mutex_unlock(&dm_thin_pool_table.mutex);
}

static int parse_pool_features(struct dm_arg_set *as, struct pool_features *pf,
			       struct dm_target *ti)
{
	int r;
	unsigned argc;
	const char *arg_name;

	static struct dm_arg _args[] = {
		{0, 3, "Invalid number of pool feature arguments"},
	};

	/*
	 * No feature arguments supplied.
	 */
	if (!as->argc)
		return 0;

<<<<<<< HEAD
	r = dm_read_arg_group(_args, as, &argc, &ti->error);
	if (r)
		return -EINVAL;

	while (argc && !r) {
		arg_name = dm_shift_arg(as);
		argc--;

		if (!strcasecmp(arg_name, "skip_block_zeroing")) {
			pf->zero_new_blocks = 0;
			continue;
		} else if (!strcasecmp(arg_name, "ignore_discard")) {
			pf->discard_enabled = 0;
			continue;
		} else if (!strcasecmp(arg_name, "no_discard_passdown")) {
			pf->discard_passdown = 0;
			continue;
		}

		ti->error = "Unrecognised pool feature requested";
		r = -EINVAL;
	}

	return r;
=======
	r = dm_read_arg_group(_args, as, &argc, &ti->error);
	if (r)
		return -EINVAL;

	while (argc && !r) {
		arg_name = dm_shift_arg(as);
		argc--;

		if (!strcasecmp(arg_name, "skip_block_zeroing"))
			pf->zero_new_blocks = false;

		else if (!strcasecmp(arg_name, "ignore_discard"))
			pf->discard_enabled = false;

		else if (!strcasecmp(arg_name, "no_discard_passdown"))
			pf->discard_passdown = false;

		else if (!strcasecmp(arg_name, "read_only"))
			pf->mode = PM_READ_ONLY;

		else {
			ti->error = "Unrecognised pool feature requested";
			r = -EINVAL;
			break;
		}
	}

	return r;
}

static void metadata_low_callback(void *context)
{
	struct pool *pool = context;

	DMWARN("%s: reached low water mark for metadata device: sending event.",
	       dm_device_name(pool->pool_md));

	dm_table_event(pool->ti->table);
}

static sector_t get_metadata_dev_size(struct block_device *bdev)
{
	sector_t metadata_dev_size = i_size_read(bdev->bd_inode) >> SECTOR_SHIFT;
	char buffer[BDEVNAME_SIZE];

	if (metadata_dev_size > THIN_METADATA_MAX_SECTORS_WARNING) {
		DMWARN("Metadata device %s is larger than %u sectors: excess space will not be used.",
		       bdevname(bdev, buffer), THIN_METADATA_MAX_SECTORS);
		metadata_dev_size = THIN_METADATA_MAX_SECTORS_WARNING;
	}

	return metadata_dev_size;
}

static dm_block_t get_metadata_dev_size_in_blocks(struct block_device *bdev)
{
	sector_t metadata_dev_size = get_metadata_dev_size(bdev);

	sector_div(metadata_dev_size, THIN_METADATA_BLOCK_SIZE >> SECTOR_SHIFT);

	return metadata_dev_size;
}

/*
 * When a metadata threshold is crossed a dm event is triggered, and
 * userland should respond by growing the metadata device.  We could let
 * userland set the threshold, like we do with the data threshold, but I'm
 * not sure they know enough to do this well.
 */
static dm_block_t calc_metadata_threshold(struct pool_c *pt)
{
	/*
	 * 4M is ample for all ops with the possible exception of thin
	 * device deletion which is harmless if it fails (just retry the
	 * delete after you've grown the device).
	 */
	dm_block_t quarter = get_metadata_dev_size_in_blocks(pt->metadata_dev->bdev) / 4;
	return min((dm_block_t)1024ULL /* 4M */, quarter);
>>>>>>> refs/remotes/origin/master
}

/*
 * thin-pool <metadata dev> <data dev>
 *	     <data block size (sectors)>
 *	     <low water mark (blocks)>
 *	     [<#feature args> [<arg>]*]
 *
 * Optional feature arguments are:
 *	     skip_block_zeroing: skips the zeroing of newly-provisioned blocks.
 *	     ignore_discard: disable discard
 *	     no_discard_passdown: don't pass discards down to the data device
 */
static int pool_ctr(struct dm_target *ti, unsigned argc, char **argv)
{
	int r, pool_created = 0;
	struct pool_c *pt;
	struct pool *pool;
	struct pool_features pf;
	struct dm_arg_set as;
	struct dm_dev *data_dev;
	unsigned long block_size;
	dm_block_t low_water_blocks;
	struct dm_dev *metadata_dev;
<<<<<<< HEAD
	sector_t metadata_dev_size;
	char b[BDEVNAME_SIZE];
=======
	fmode_t metadata_mode;
>>>>>>> refs/remotes/origin/master

	/*
	 * FIXME Remove validation from scope of lock.
	 */
	mutex_lock(&dm_thin_pool_table.mutex);

	if (argc < 4) {
		ti->error = "Invalid argument count";
		r = -EINVAL;
		goto out_unlock;
	}
<<<<<<< HEAD
	as.argc = argc;
	as.argv = argv;

	r = dm_get_device(ti, argv[0], FMODE_READ | FMODE_WRITE, &metadata_dev);
=======

	as.argc = argc;
	as.argv = argv;

	/*
	 * Set default pool features.
	 */
	pool_features_init(&pf);

	dm_consume_args(&as, 4);
	r = parse_pool_features(&as, &pf, ti);
	if (r)
		goto out_unlock;

	metadata_mode = FMODE_READ | ((pf.mode == PM_READ_ONLY) ? 0 : FMODE_WRITE);
	r = dm_get_device(ti, argv[0], metadata_mode, &metadata_dev);
>>>>>>> refs/remotes/origin/master
	if (r) {
		ti->error = "Error opening metadata block device";
		goto out_unlock;
	}

<<<<<<< HEAD
	metadata_dev_size = i_size_read(metadata_dev->bdev->bd_inode) >> SECTOR_SHIFT;
	if (metadata_dev_size > THIN_METADATA_MAX_SECTORS_WARNING)
		DMWARN("Metadata device %s is larger than %u sectors: excess space will not be used.",
		       bdevname(metadata_dev->bdev, b), THIN_METADATA_MAX_SECTORS);
=======
	/*
	 * Run for the side-effect of possibly issuing a warning if the
	 * device is too big.
	 */
	(void) get_metadata_dev_size(metadata_dev->bdev);
>>>>>>> refs/remotes/origin/master

	r = dm_get_device(ti, argv[1], FMODE_READ | FMODE_WRITE, &data_dev);
	if (r) {
		ti->error = "Error getting data device";
		goto out_metadata;
	}

	if (kstrtoul(argv[2], 10, &block_size) || !block_size ||
	    block_size < DATA_DEV_BLOCK_SIZE_MIN_SECTORS ||
	    block_size > DATA_DEV_BLOCK_SIZE_MAX_SECTORS ||
<<<<<<< HEAD
	    !is_power_of_2(block_size)) {
=======
	    block_size & (DATA_DEV_BLOCK_SIZE_MIN_SECTORS - 1)) {
>>>>>>> refs/remotes/origin/master
		ti->error = "Invalid block size";
		r = -EINVAL;
		goto out;
	}

	if (kstrtoull(argv[3], 10, (unsigned long long *)&low_water_blocks)) {
		ti->error = "Invalid low water mark";
		r = -EINVAL;
		goto out;
	}

<<<<<<< HEAD
	/*
	 * Set default pool features.
	 */
	pool_features_init(&pf);

	dm_consume_args(&as, 4);
	r = parse_pool_features(&as, &pf, ti);
	if (r)
		goto out;

=======
>>>>>>> refs/remotes/origin/master
	pt = kzalloc(sizeof(*pt), GFP_KERNEL);
	if (!pt) {
		r = -ENOMEM;
		goto out;
	}

	pool = __pool_find(dm_table_get_md(ti->table), metadata_dev->bdev,
<<<<<<< HEAD
			   block_size, &ti->error, &pool_created);
=======
			   block_size, pf.mode == PM_READ_ONLY, &ti->error, &pool_created);
>>>>>>> refs/remotes/origin/master
	if (IS_ERR(pool)) {
		r = PTR_ERR(pool);
		goto out_free_pt;
	}

	/*
	 * 'pool_created' reflects whether this is the first table load.
	 * Top level discard support is not allowed to be changed after
	 * initial load.  This would require a pool reload to trigger thin
	 * device changes.
	 */
	if (!pool_created && pf.discard_enabled != pool->pf.discard_enabled) {
		ti->error = "Discard support cannot be disabled once enabled";
		r = -EINVAL;
		goto out_flags_changed;
	}

	pt->pool = pool;
	pt->ti = ti;
	pt->metadata_dev = metadata_dev;
	pt->data_dev = data_dev;
	pt->low_water_blocks = low_water_blocks;
<<<<<<< HEAD
	pt->pf = pf;
	ti->num_flush_requests = 1;
=======
	pt->adjusted_pf = pt->requested_pf = pf;
	ti->num_flush_bios = 1;

>>>>>>> refs/remotes/origin/master
	/*
	 * Only need to enable discards if the pool should pass
	 * them down to the data device.  The thin device's discard
	 * processing will cause mappings to be removed from the btree.
	 */
<<<<<<< HEAD
	if (pf.discard_enabled && pf.discard_passdown) {
		ti->num_discard_requests = 1;
=======
	ti->discard_zeroes_data_unsupported = true;
	if (pf.discard_enabled && pf.discard_passdown) {
		ti->num_discard_bios = 1;

>>>>>>> refs/remotes/origin/master
		/*
		 * Setting 'discards_supported' circumvents the normal
		 * stacking of discard limits (this keeps the pool and
		 * thin devices' discard limits consistent).
		 */
<<<<<<< HEAD
		ti->discards_supported = 1;
		ti->discard_zeroes_data_unsupported = 1;
	}
	ti->private = pt;

=======
		ti->discards_supported = true;
	}
	ti->private = pt;

	r = dm_pool_register_metadata_threshold(pt->pool->pmd,
						calc_metadata_threshold(pt),
						metadata_low_callback,
						pool);
	if (r)
		goto out_free_pt;

>>>>>>> refs/remotes/origin/master
	pt->callbacks.congested_fn = pool_is_congested;
	dm_table_add_target_callbacks(ti->table, &pt->callbacks);

	mutex_unlock(&dm_thin_pool_table.mutex);

	return 0;

out_flags_changed:
	__pool_dec(pool);
out_free_pt:
	kfree(pt);
out:
	dm_put_device(ti, data_dev);
out_metadata:
	dm_put_device(ti, metadata_dev);
out_unlock:
	mutex_unlock(&dm_thin_pool_table.mutex);

	return r;
}

<<<<<<< HEAD
static int pool_map(struct dm_target *ti, struct bio *bio,
		    union map_info *map_context)
=======
static int pool_map(struct dm_target *ti, struct bio *bio)
>>>>>>> refs/remotes/origin/master
{
	int r;
	struct pool_c *pt = ti->private;
	struct pool *pool = pt->pool;
	unsigned long flags;

	/*
	 * As this is a singleton target, ti->begin is always zero.
	 */
	spin_lock_irqsave(&pool->lock, flags);
	bio->bi_bdev = pt->data_dev->bdev;
	r = DM_MAPIO_REMAPPED;
	spin_unlock_irqrestore(&pool->lock, flags);

	return r;
}

<<<<<<< HEAD
=======
static int maybe_resize_data_dev(struct dm_target *ti, bool *need_commit)
{
	int r;
	struct pool_c *pt = ti->private;
	struct pool *pool = pt->pool;
	sector_t data_size = ti->len;
	dm_block_t sb_data_size;

	*need_commit = false;

	(void) sector_div(data_size, pool->sectors_per_block);

	r = dm_pool_get_data_dev_size(pool->pmd, &sb_data_size);
	if (r) {
		DMERR("%s: failed to retrieve data device size",
		      dm_device_name(pool->pool_md));
		return r;
	}

	if (data_size < sb_data_size) {
		DMERR("%s: pool target (%llu blocks) too small: expected %llu",
		      dm_device_name(pool->pool_md),
		      (unsigned long long)data_size, sb_data_size);
		return -EINVAL;

	} else if (data_size > sb_data_size) {
		r = dm_pool_resize_data_dev(pool->pmd, data_size);
		if (r) {
			DMERR("%s: failed to resize data device",
			      dm_device_name(pool->pool_md));
			set_pool_mode(pool, PM_READ_ONLY);
			return r;
		}

		*need_commit = true;
	}

	return 0;
}

static int maybe_resize_metadata_dev(struct dm_target *ti, bool *need_commit)
{
	int r;
	struct pool_c *pt = ti->private;
	struct pool *pool = pt->pool;
	dm_block_t metadata_dev_size, sb_metadata_dev_size;

	*need_commit = false;

	metadata_dev_size = get_metadata_dev_size_in_blocks(pool->md_dev);

	r = dm_pool_get_metadata_dev_size(pool->pmd, &sb_metadata_dev_size);
	if (r) {
		DMERR("%s: failed to retrieve metadata device size",
		      dm_device_name(pool->pool_md));
		return r;
	}

	if (metadata_dev_size < sb_metadata_dev_size) {
		DMERR("%s: metadata device (%llu blocks) too small: expected %llu",
		      dm_device_name(pool->pool_md),
		      metadata_dev_size, sb_metadata_dev_size);
		return -EINVAL;

	} else if (metadata_dev_size > sb_metadata_dev_size) {
		r = dm_pool_resize_metadata_dev(pool->pmd, metadata_dev_size);
		if (r) {
			DMERR("%s: failed to resize metadata device",
			      dm_device_name(pool->pool_md));
			return r;
		}

		*need_commit = true;
	}

	return 0;
}

>>>>>>> refs/remotes/origin/master
/*
 * Retrieves the number of blocks of the data device from
 * the superblock and compares it to the actual device size,
 * thus resizing the data device in case it has grown.
 *
 * This both copes with opening preallocated data devices in the ctr
 * being followed by a resume
 * -and-
 * calling the resume method individually after userspace has
 * grown the data device in reaction to a table event.
 */
static int pool_preresume(struct dm_target *ti)
{
	int r;
<<<<<<< HEAD
	struct pool_c *pt = ti->private;
	struct pool *pool = pt->pool;
	dm_block_t data_size, sb_data_size;
=======
	bool need_commit1, need_commit2;
	struct pool_c *pt = ti->private;
	struct pool *pool = pt->pool;
>>>>>>> refs/remotes/origin/master

	/*
	 * Take control of the pool object.
	 */
	r = bind_control_target(pool, ti);
	if (r)
		return r;

<<<<<<< HEAD
	data_size = ti->len >> pool->block_shift;
	r = dm_pool_get_data_dev_size(pool->pmd, &sb_data_size);
	if (r) {
		DMERR("failed to retrieve data device size");
		return r;
	}

	if (data_size < sb_data_size) {
		DMERR("pool target too small, is %llu blocks (expected %llu)",
		      data_size, sb_data_size);
		return -EINVAL;

	} else if (data_size > sb_data_size) {
		r = dm_pool_resize_data_dev(pool->pmd, data_size);
		if (r) {
			DMERR("failed to resize data device");
			return r;
		}

		r = dm_pool_commit_metadata(pool->pmd);
		if (r) {
			DMERR("%s: dm_pool_commit_metadata() failed, error = %d",
			      __func__, r);
			return r;
		}
	}
=======
	r = maybe_resize_data_dev(ti, &need_commit1);
	if (r)
		return r;

	r = maybe_resize_metadata_dev(ti, &need_commit2);
	if (r)
		return r;

	if (need_commit1 || need_commit2)
		(void) commit(pool);
>>>>>>> refs/remotes/origin/master

	return 0;
}

static void pool_resume(struct dm_target *ti)
{
	struct pool_c *pt = ti->private;
	struct pool *pool = pt->pool;
	unsigned long flags;

	spin_lock_irqsave(&pool->lock, flags);
	pool->low_water_triggered = 0;
	pool->no_free_space = 0;
	__requeue_bios(pool);
	spin_unlock_irqrestore(&pool->lock, flags);

	do_waker(&pool->waker.work);
}

static void pool_postsuspend(struct dm_target *ti)
{
<<<<<<< HEAD
	int r;
=======
>>>>>>> refs/remotes/origin/master
	struct pool_c *pt = ti->private;
	struct pool *pool = pt->pool;

	cancel_delayed_work(&pool->waker);
	flush_workqueue(pool->wq);
<<<<<<< HEAD

	r = dm_pool_commit_metadata(pool->pmd);
	if (r < 0) {
		DMERR("%s: dm_pool_commit_metadata() failed, error = %d",
		      __func__, r);
		/* FIXME: invalidate device? error the next FUA or FLUSH bio ?*/
	}
=======
	(void) commit(pool);
>>>>>>> refs/remotes/origin/master
}

static int check_arg_count(unsigned argc, unsigned args_required)
{
	if (argc != args_required) {
		DMWARN("Message received with %u arguments instead of %u.",
		       argc, args_required);
		return -EINVAL;
	}

	return 0;
}

static int read_dev_id(char *arg, dm_thin_id *dev_id, int warning)
{
	if (!kstrtoull(arg, 10, (unsigned long long *)dev_id) &&
	    *dev_id <= MAX_DEV_ID)
		return 0;

	if (warning)
		DMWARN("Message received with invalid device id: %s", arg);

	return -EINVAL;
}

static int process_create_thin_mesg(unsigned argc, char **argv, struct pool *pool)
{
	dm_thin_id dev_id;
	int r;

	r = check_arg_count(argc, 2);
	if (r)
		return r;

	r = read_dev_id(argv[1], &dev_id, 1);
	if (r)
		return r;

	r = dm_pool_create_thin(pool->pmd, dev_id);
	if (r) {
		DMWARN("Creation of new thinly-provisioned device with id %s failed.",
		       argv[1]);
		return r;
	}

	return 0;
}

static int process_create_snap_mesg(unsigned argc, char **argv, struct pool *pool)
{
	dm_thin_id dev_id;
	dm_thin_id origin_dev_id;
	int r;

	r = check_arg_count(argc, 3);
	if (r)
		return r;

	r = read_dev_id(argv[1], &dev_id, 1);
	if (r)
		return r;

	r = read_dev_id(argv[2], &origin_dev_id, 1);
	if (r)
		return r;

	r = dm_pool_create_snap(pool->pmd, dev_id, origin_dev_id);
	if (r) {
		DMWARN("Creation of new snapshot %s of device %s failed.",
		       argv[1], argv[2]);
		return r;
	}

	return 0;
}

static int process_delete_mesg(unsigned argc, char **argv, struct pool *pool)
{
	dm_thin_id dev_id;
	int r;

	r = check_arg_count(argc, 2);
	if (r)
		return r;

	r = read_dev_id(argv[1], &dev_id, 1);
	if (r)
		return r;

	r = dm_pool_delete_thin_device(pool->pmd, dev_id);
	if (r)
		DMWARN("Deletion of thin device %s failed.", argv[1]);

	return r;
}

static int process_set_transaction_id_mesg(unsigned argc, char **argv, struct pool *pool)
{
	dm_thin_id old_id, new_id;
	int r;

	r = check_arg_count(argc, 3);
	if (r)
		return r;

	if (kstrtoull(argv[1], 10, (unsigned long long *)&old_id)) {
		DMWARN("set_transaction_id message: Unrecognised id %s.", argv[1]);
		return -EINVAL;
	}

	if (kstrtoull(argv[2], 10, (unsigned long long *)&new_id)) {
		DMWARN("set_transaction_id message: Unrecognised new id %s.", argv[2]);
		return -EINVAL;
	}

	r = dm_pool_set_metadata_transaction_id(pool->pmd, old_id, new_id);
	if (r) {
		DMWARN("Failed to change transaction id from %s to %s.",
		       argv[1], argv[2]);
		return r;
	}

	return 0;
}

<<<<<<< HEAD
=======
static int process_reserve_metadata_snap_mesg(unsigned argc, char **argv, struct pool *pool)
{
	int r;

	r = check_arg_count(argc, 1);
	if (r)
		return r;

	(void) commit(pool);

	r = dm_pool_reserve_metadata_snap(pool->pmd);
	if (r)
		DMWARN("reserve_metadata_snap message failed.");

	return r;
}

static int process_release_metadata_snap_mesg(unsigned argc, char **argv, struct pool *pool)
{
	int r;

	r = check_arg_count(argc, 1);
	if (r)
		return r;

	r = dm_pool_release_metadata_snap(pool->pmd);
	if (r)
		DMWARN("release_metadata_snap message failed.");

	return r;
}

>>>>>>> refs/remotes/origin/master
/*
 * Messages supported:
 *   create_thin	<dev_id>
 *   create_snap	<dev_id> <origin_id>
 *   delete		<dev_id>
 *   trim		<dev_id> <new_size_in_sectors>
 *   set_transaction_id <current_trans_id> <new_trans_id>
<<<<<<< HEAD
=======
 *   reserve_metadata_snap
 *   release_metadata_snap
>>>>>>> refs/remotes/origin/master
 */
static int pool_message(struct dm_target *ti, unsigned argc, char **argv)
{
	int r = -EINVAL;
	struct pool_c *pt = ti->private;
	struct pool *pool = pt->pool;

	if (!strcasecmp(argv[0], "create_thin"))
		r = process_create_thin_mesg(argc, argv, pool);

	else if (!strcasecmp(argv[0], "create_snap"))
		r = process_create_snap_mesg(argc, argv, pool);

	else if (!strcasecmp(argv[0], "delete"))
		r = process_delete_mesg(argc, argv, pool);

	else if (!strcasecmp(argv[0], "set_transaction_id"))
		r = process_set_transaction_id_mesg(argc, argv, pool);

<<<<<<< HEAD
	else
		DMWARN("Unrecognised thin pool target message received: %s", argv[0]);

	if (!r) {
		r = dm_pool_commit_metadata(pool->pmd);
		if (r)
			DMERR("%s message: dm_pool_commit_metadata() failed, error = %d",
			      argv[0], r);
	}
=======
	else if (!strcasecmp(argv[0], "reserve_metadata_snap"))
		r = process_reserve_metadata_snap_mesg(argc, argv, pool);

	else if (!strcasecmp(argv[0], "release_metadata_snap"))
		r = process_release_metadata_snap_mesg(argc, argv, pool);

	else
		DMWARN("Unrecognised thin pool target message received: %s", argv[0]);

	if (!r)
		(void) commit(pool);
>>>>>>> refs/remotes/origin/master

	return r;
}

<<<<<<< HEAD
=======
static void emit_flags(struct pool_features *pf, char *result,
		       unsigned sz, unsigned maxlen)
{
	unsigned count = !pf->zero_new_blocks + !pf->discard_enabled +
		!pf->discard_passdown + (pf->mode == PM_READ_ONLY);
	DMEMIT("%u ", count);

	if (!pf->zero_new_blocks)
		DMEMIT("skip_block_zeroing ");

	if (!pf->discard_enabled)
		DMEMIT("ignore_discard ");

	if (!pf->discard_passdown)
		DMEMIT("no_discard_passdown ");

	if (pf->mode == PM_READ_ONLY)
		DMEMIT("read_only ");
}

>>>>>>> refs/remotes/origin/master
/*
 * Status line is:
 *    <transaction id> <used metadata sectors>/<total metadata sectors>
 *    <used data sectors>/<total data sectors> <held metadata root>
 */
<<<<<<< HEAD
<<<<<<< HEAD
static int pool_status(struct dm_target *ti, status_type_t type,
		       char *result, unsigned maxlen)
=======
static void pool_status(struct dm_target *ti, status_type_t type,
			char *result, unsigned maxlen)
>>>>>>> refs/remotes/origin/cm-11.0
{
	int r, count;
=======
static void pool_status(struct dm_target *ti, status_type_t type,
			unsigned status_flags, char *result, unsigned maxlen)
{
	int r;
>>>>>>> refs/remotes/origin/master
	unsigned sz = 0;
	uint64_t transaction_id;
	dm_block_t nr_free_blocks_data;
	dm_block_t nr_free_blocks_metadata;
	dm_block_t nr_blocks_data;
	dm_block_t nr_blocks_metadata;
	dm_block_t held_root;
	char buf[BDEVNAME_SIZE];
	char buf2[BDEVNAME_SIZE];
	struct pool_c *pt = ti->private;
	struct pool *pool = pt->pool;

	switch (type) {
	case STATUSTYPE_INFO:
<<<<<<< HEAD
<<<<<<< HEAD
		r = dm_pool_get_metadata_transaction_id(pool->pmd,
							&transaction_id);
		if (r)
			return r;
=======
		r = dm_pool_get_metadata_transaction_id(pool->pmd, &transaction_id);
		if (r) {
			DMERR("dm_pool_get_metadata_transaction_id returned %d", r);
			goto err;
		}
>>>>>>> refs/remotes/origin/cm-11.0

		r = dm_pool_get_free_metadata_block_count(pool->pmd, &nr_free_blocks_metadata);
		if (r) {
			DMERR("dm_pool_get_free_metadata_block_count returned %d", r);
			goto err;
		}

		r = dm_pool_get_metadata_dev_size(pool->pmd, &nr_blocks_metadata);
		if (r) {
			DMERR("dm_pool_get_metadata_dev_size returned %d", r);
			goto err;
		}

		r = dm_pool_get_free_block_count(pool->pmd, &nr_free_blocks_data);
		if (r) {
			DMERR("dm_pool_get_free_block_count returned %d", r);
			goto err;
		}

		r = dm_pool_get_data_dev_size(pool->pmd, &nr_blocks_data);
		if (r) {
			DMERR("dm_pool_get_data_dev_size returned %d", r);
			goto err;
		}

		r = dm_pool_get_held_metadata_root(pool->pmd, &held_root);
<<<<<<< HEAD
		if (r)
			return r;
=======
		if (get_pool_mode(pool) == PM_FAIL) {
			DMEMIT("Fail");
			break;
		}

		/* Commit to ensure statistics aren't out-of-date */
		if (!(status_flags & DM_STATUS_NOFLUSH_FLAG) && !dm_suspended(ti))
			(void) commit(pool);

		r = dm_pool_get_metadata_transaction_id(pool->pmd, &transaction_id);
		if (r) {
			DMERR("%s: dm_pool_get_metadata_transaction_id returned %d",
			      dm_device_name(pool->pool_md), r);
			goto err;
		}

		r = dm_pool_get_free_metadata_block_count(pool->pmd, &nr_free_blocks_metadata);
		if (r) {
			DMERR("%s: dm_pool_get_free_metadata_block_count returned %d",
			      dm_device_name(pool->pool_md), r);
			goto err;
		}

		r = dm_pool_get_metadata_dev_size(pool->pmd, &nr_blocks_metadata);
		if (r) {
			DMERR("%s: dm_pool_get_metadata_dev_size returned %d",
			      dm_device_name(pool->pool_md), r);
			goto err;
		}

		r = dm_pool_get_free_block_count(pool->pmd, &nr_free_blocks_data);
		if (r) {
			DMERR("%s: dm_pool_get_free_block_count returned %d",
			      dm_device_name(pool->pool_md), r);
			goto err;
		}

		r = dm_pool_get_data_dev_size(pool->pmd, &nr_blocks_data);
		if (r) {
			DMERR("%s: dm_pool_get_data_dev_size returned %d",
			      dm_device_name(pool->pool_md), r);
			goto err;
		}

		r = dm_pool_get_metadata_snap(pool->pmd, &held_root);
		if (r) {
			DMERR("%s: dm_pool_get_metadata_snap returned %d",
			      dm_device_name(pool->pool_md), r);
			goto err;
		}
>>>>>>> refs/remotes/origin/master
=======
		if (r) {
			DMERR("dm_pool_get_metadata_snap returned %d", r);
			goto err;
		}
>>>>>>> refs/remotes/origin/cm-11.0

		DMEMIT("%llu %llu/%llu %llu/%llu ",
		       (unsigned long long)transaction_id,
		       (unsigned long long)(nr_blocks_metadata - nr_free_blocks_metadata),
		       (unsigned long long)nr_blocks_metadata,
		       (unsigned long long)(nr_blocks_data - nr_free_blocks_data),
		       (unsigned long long)nr_blocks_data);

		if (held_root)
<<<<<<< HEAD
			DMEMIT("%llu", held_root);
		else
			DMEMIT("-");
=======
			DMEMIT("%llu ", held_root);
		else
			DMEMIT("- ");

		if (pool->pf.mode == PM_READ_ONLY)
			DMEMIT("ro ");
		else
			DMEMIT("rw ");

		if (!pool->pf.discard_enabled)
			DMEMIT("ignore_discard");
		else if (pool->pf.discard_passdown)
			DMEMIT("discard_passdown");
		else
			DMEMIT("no_discard_passdown");
>>>>>>> refs/remotes/origin/master

		break;

	case STATUSTYPE_TABLE:
		DMEMIT("%s %s %lu %llu ",
		       format_dev_t(buf, pt->metadata_dev->bdev->bd_dev),
		       format_dev_t(buf2, pt->data_dev->bdev->bd_dev),
		       (unsigned long)pool->sectors_per_block,
		       (unsigned long long)pt->low_water_blocks);
<<<<<<< HEAD

		count = !pool->pf.zero_new_blocks + !pool->pf.discard_enabled +
			!pt->pf.discard_passdown;
		DMEMIT("%u ", count);

		if (!pool->pf.zero_new_blocks)
			DMEMIT("skip_block_zeroing ");

		if (!pool->pf.discard_enabled)
			DMEMIT("ignore_discard ");

		if (!pt->pf.discard_passdown)
			DMEMIT("no_discard_passdown ");

		break;
	}
	return;

<<<<<<< HEAD
	return 0;
=======
		emit_flags(&pt->requested_pf, result, sz, maxlen);
		break;
	}
	return;

err:
	DMEMIT("Error");
>>>>>>> refs/remotes/origin/master
=======
err:
	DMEMIT("Error");
>>>>>>> refs/remotes/origin/cm-11.0
}

static int pool_iterate_devices(struct dm_target *ti,
				iterate_devices_callout_fn fn, void *data)
{
	struct pool_c *pt = ti->private;

	return fn(ti, pt->data_dev, 0, ti->len, data);
}

static int pool_merge(struct dm_target *ti, struct bvec_merge_data *bvm,
		      struct bio_vec *biovec, int max_size)
{
	struct pool_c *pt = ti->private;
	struct request_queue *q = bdev_get_queue(pt->data_dev->bdev);

	if (!q->merge_bvec_fn)
		return max_size;

	bvm->bi_bdev = pt->data_dev->bdev;

	return min(max_size, q->merge_bvec_fn(q, bvm, biovec));
}

<<<<<<< HEAD
static void set_discard_limits(struct pool *pool, struct queue_limits *limits)
{
	/*
	 * FIXME: these limits may be incompatible with the pool's data device
	 */
	limits->max_discard_sectors = pool->sectors_per_block;

	/*
	 * This is just a hint, and not enforced.  We have to cope with
	 * bios that overlap 2 blocks.
	 */
	limits->discard_granularity = pool->sectors_per_block << SECTOR_SHIFT;
<<<<<<< HEAD
=======
static void set_discard_limits(struct pool_c *pt, struct queue_limits *limits)
{
	struct pool *pool = pt->pool;
	struct queue_limits *data_limits;

	limits->max_discard_sectors = pool->sectors_per_block;

	/*
	 * discard_granularity is just a hint, and not enforced.
	 */
	if (pt->adjusted_pf.discard_passdown) {
		data_limits = &bdev_get_queue(pt->data_dev->bdev)->limits;
		limits->discard_granularity = data_limits->discard_granularity;
	} else
		limits->discard_granularity = pool->sectors_per_block << SECTOR_SHIFT;
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
}

static void pool_io_hints(struct dm_target *ti, struct queue_limits *limits)
{
	struct pool_c *pt = ti->private;
	struct pool *pool = pt->pool;
<<<<<<< HEAD

	blk_limits_io_min(limits, 0);
	blk_limits_io_opt(limits, pool->sectors_per_block << SECTOR_SHIFT);
	if (pool->pf.discard_enabled)
		set_discard_limits(pool, limits);
=======
	uint64_t io_opt_sectors = limits->io_opt >> SECTOR_SHIFT;

	/*
	 * If the system-determined stacked limits are compatible with the
	 * pool's blocksize (io_opt is a factor) do not override them.
	 */
	if (io_opt_sectors < pool->sectors_per_block ||
	    do_div(io_opt_sectors, pool->sectors_per_block)) {
		blk_limits_io_min(limits, 0);
		blk_limits_io_opt(limits, pool->sectors_per_block << SECTOR_SHIFT);
	}

	/*
	 * pt->adjusted_pf is a staging area for the actual features to use.
	 * They get transferred to the live pool in bind_control_target()
	 * called from pool_preresume().
	 */
	if (!pt->adjusted_pf.discard_enabled) {
		/*
		 * Must explicitly disallow stacking discard limits otherwise the
		 * block layer will stack them if pool's data device has support.
		 * QUEUE_FLAG_DISCARD wouldn't be set but there is no way for the
		 * user to see that, so make sure to set all discard limits to 0.
		 */
		limits->discard_granularity = 0;
		return;
	}

	disable_passdown_if_not_supported(pt);

	set_discard_limits(pt, limits);
>>>>>>> refs/remotes/origin/master
}

static struct target_type pool_target = {
	.name = "thin-pool",
	.features = DM_TARGET_SINGLETON | DM_TARGET_ALWAYS_WRITEABLE |
		    DM_TARGET_IMMUTABLE,
<<<<<<< HEAD
	.version = {1, 1, 0},
=======
	.version = {1, 9, 0},
>>>>>>> refs/remotes/origin/master
	.module = THIS_MODULE,
	.ctr = pool_ctr,
	.dtr = pool_dtr,
	.map = pool_map,
	.postsuspend = pool_postsuspend,
	.preresume = pool_preresume,
	.resume = pool_resume,
	.message = pool_message,
	.status = pool_status,
	.merge = pool_merge,
	.iterate_devices = pool_iterate_devices,
	.io_hints = pool_io_hints,
};

/*----------------------------------------------------------------
 * Thin target methods
 *--------------------------------------------------------------*/
static void thin_dtr(struct dm_target *ti)
{
	struct thin_c *tc = ti->private;

	mutex_lock(&dm_thin_pool_table.mutex);

	__pool_dec(tc->pool);
	dm_pool_close_thin_device(tc->td);
	dm_put_device(ti, tc->pool_dev);
	if (tc->origin_dev)
		dm_put_device(ti, tc->origin_dev);
	kfree(tc);

	mutex_unlock(&dm_thin_pool_table.mutex);
}

/*
 * Thin target parameters:
 *
 * <pool_dev> <dev_id> [origin_dev]
 *
 * pool_dev: the path to the pool (eg, /dev/mapper/my_pool)
 * dev_id: the internal device identifier
 * origin_dev: a device external to the pool that should act as the origin
 *
 * If the pool device has discards disabled, they get disabled for the thin
 * device as well.
 */
static int thin_ctr(struct dm_target *ti, unsigned argc, char **argv)
{
	int r;
	struct thin_c *tc;
	struct dm_dev *pool_dev, *origin_dev;
	struct mapped_device *pool_md;

	mutex_lock(&dm_thin_pool_table.mutex);

	if (argc != 2 && argc != 3) {
		ti->error = "Invalid argument count";
		r = -EINVAL;
		goto out_unlock;
	}

	tc = ti->private = kzalloc(sizeof(*tc), GFP_KERNEL);
	if (!tc) {
		ti->error = "Out of memory";
		r = -ENOMEM;
		goto out_unlock;
	}

	if (argc == 3) {
		r = dm_get_device(ti, argv[2], FMODE_READ, &origin_dev);
		if (r) {
			ti->error = "Error opening origin device";
			goto bad_origin_dev;
		}
		tc->origin_dev = origin_dev;
	}

	r = dm_get_device(ti, argv[0], dm_table_get_mode(ti->table), &pool_dev);
	if (r) {
		ti->error = "Error opening pool device";
		goto bad_pool_dev;
	}
	tc->pool_dev = pool_dev;

	if (read_dev_id(argv[1], (unsigned long long *)&tc->dev_id, 0)) {
		ti->error = "Invalid device id";
		r = -EINVAL;
		goto bad_common;
	}

	pool_md = dm_get_md(tc->pool_dev->bdev->bd_dev);
	if (!pool_md) {
		ti->error = "Couldn't get pool mapped device";
		r = -EINVAL;
		goto bad_common;
	}

	tc->pool = __pool_table_lookup(pool_md);
	if (!tc->pool) {
		ti->error = "Couldn't find pool object";
		r = -EINVAL;
		goto bad_pool_lookup;
	}
	__pool_inc(tc->pool);

<<<<<<< HEAD
=======
	if (get_pool_mode(tc->pool) == PM_FAIL) {
		ti->error = "Couldn't open thin device, Pool is in fail mode";
		goto bad_thin_open;
	}

>>>>>>> refs/remotes/origin/master
	r = dm_pool_open_thin_device(tc->pool->pmd, tc->dev_id, &tc->td);
	if (r) {
		ti->error = "Couldn't open thin internal device";
		goto bad_thin_open;
	}

<<<<<<< HEAD
	ti->split_io = tc->pool->sectors_per_block;
	ti->num_flush_requests = 1;

	/* In case the pool supports discards, pass them on. */
	if (tc->pool->pf.discard_enabled) {
		ti->discards_supported = 1;
		ti->num_discard_requests = 1;
		ti->discard_zeroes_data_unsupported = 1;
<<<<<<< HEAD
=======
	r = dm_set_target_max_io_len(ti, tc->pool->sectors_per_block);
	if (r)
		goto bad_thin_open;

	ti->num_flush_bios = 1;
	ti->flush_supported = true;
	ti->per_bio_data_size = sizeof(struct dm_thin_endio_hook);

	/* In case the pool supports discards, pass them on. */
	ti->discard_zeroes_data_unsupported = true;
	if (tc->pool->pf.discard_enabled) {
		ti->discards_supported = true;
		ti->num_discard_bios = 1;
		/* Discard bios must be split on a block boundary */
		ti->split_discard_bios = true;
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	}

	dm_put(pool_md);

	mutex_unlock(&dm_thin_pool_table.mutex);

	return 0;

bad_thin_open:
	__pool_dec(tc->pool);
bad_pool_lookup:
	dm_put(pool_md);
bad_common:
	dm_put_device(ti, tc->pool_dev);
bad_pool_dev:
	if (tc->origin_dev)
		dm_put_device(ti, tc->origin_dev);
bad_origin_dev:
	kfree(tc);
out_unlock:
	mutex_unlock(&dm_thin_pool_table.mutex);

	return r;
}

<<<<<<< HEAD
static int thin_map(struct dm_target *ti, struct bio *bio,
		    union map_info *map_context)
{
	bio->bi_sector = dm_target_offset(ti, bio->bi_sector);

	return thin_bio_map(ti, bio, map_context);
}

static int thin_endio(struct dm_target *ti,
		      struct bio *bio, int err,
		      union map_info *map_context)
{
	unsigned long flags;
	struct endio_hook *h = map_context->ptr;
	struct list_head work;
	struct new_mapping *m, *tmp;
=======
static int thin_map(struct dm_target *ti, struct bio *bio)
{
	bio->bi_sector = dm_target_offset(ti, bio->bi_sector);

	return thin_bio_map(ti, bio);
}

static int thin_endio(struct dm_target *ti, struct bio *bio, int err)
{
	unsigned long flags;
	struct dm_thin_endio_hook *h = dm_per_bio_data(bio, sizeof(struct dm_thin_endio_hook));
	struct list_head work;
	struct dm_thin_new_mapping *m, *tmp;
>>>>>>> refs/remotes/origin/master
	struct pool *pool = h->tc->pool;

	if (h->shared_read_entry) {
		INIT_LIST_HEAD(&work);
<<<<<<< HEAD
		ds_dec(h->shared_read_entry, &work);
=======
		dm_deferred_entry_dec(h->shared_read_entry, &work);
>>>>>>> refs/remotes/origin/master

		spin_lock_irqsave(&pool->lock, flags);
		list_for_each_entry_safe(m, tmp, &work, list) {
			list_del(&m->list);
			m->quiesced = 1;
			__maybe_add_mapping(m);
		}
		spin_unlock_irqrestore(&pool->lock, flags);
	}

	if (h->all_io_entry) {
		INIT_LIST_HEAD(&work);
<<<<<<< HEAD
		ds_dec(h->all_io_entry, &work);
		spin_lock_irqsave(&pool->lock, flags);
		list_for_each_entry_safe(m, tmp, &work, list)
			list_add(&m->list, &pool->prepared_discards);
		spin_unlock_irqrestore(&pool->lock, flags);
	}

	mempool_free(h, pool->endio_hook_pool);

=======
		dm_deferred_entry_dec(h->all_io_entry, &work);
		if (!list_empty(&work)) {
			spin_lock_irqsave(&pool->lock, flags);
			list_for_each_entry_safe(m, tmp, &work, list)
				list_add(&m->list, &pool->prepared_discards);
			spin_unlock_irqrestore(&pool->lock, flags);
			wake_worker(pool);
		}
	}

>>>>>>> refs/remotes/origin/master
	return 0;
}

static void thin_postsuspend(struct dm_target *ti)
{
	if (dm_noflush_suspending(ti))
		requeue_io((struct thin_c *)ti->private);
}

/*
 * <nr mapped sectors> <highest mapped sector>
 */
<<<<<<< HEAD
<<<<<<< HEAD
static int thin_status(struct dm_target *ti, status_type_t type,
		       char *result, unsigned maxlen)
=======
static void thin_status(struct dm_target *ti, status_type_t type,
			unsigned status_flags, char *result, unsigned maxlen)
>>>>>>> refs/remotes/origin/master
=======
static void thin_status(struct dm_target *ti, status_type_t type,
			char *result, unsigned maxlen)
>>>>>>> refs/remotes/origin/cm-11.0
{
	int r;
	ssize_t sz = 0;
	dm_block_t mapped, highest;
	char buf[BDEVNAME_SIZE];
	struct thin_c *tc = ti->private;

<<<<<<< HEAD
=======
	if (get_pool_mode(tc->pool) == PM_FAIL) {
		DMEMIT("Fail");
		return;
	}

>>>>>>> refs/remotes/origin/master
	if (!tc->td)
		DMEMIT("-");
	else {
		switch (type) {
		case STATUSTYPE_INFO:
			r = dm_thin_get_mapped_count(tc->td, &mapped);
<<<<<<< HEAD
<<<<<<< HEAD
			if (r)
				return r;

			r = dm_thin_get_highest_mapped_block(tc->td, &highest);
			if (r < 0)
				return r;
=======
			if (r) {
				DMERR("dm_thin_get_mapped_count returned %d", r);
				goto err;
			}

			r = dm_thin_get_highest_mapped_block(tc->td, &highest);
=======
			if (r) {
				DMERR("dm_thin_get_mapped_count returned %d", r);
				goto err;
			}

			r = dm_thin_get_highest_mapped_block(tc->td, &highest);
>>>>>>> refs/remotes/origin/cm-11.0
			if (r < 0) {
				DMERR("dm_thin_get_highest_mapped_block returned %d", r);
				goto err;
			}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

			DMEMIT("%llu ", mapped * tc->pool->sectors_per_block);
			if (r)
				DMEMIT("%llu", ((highest + 1) *
						tc->pool->sectors_per_block) - 1);
			else
				DMEMIT("-");
			break;

		case STATUSTYPE_TABLE:
			DMEMIT("%s %lu",
			       format_dev_t(buf, tc->pool_dev->bdev->bd_dev),
			       (unsigned long) tc->dev_id);
			if (tc->origin_dev)
				DMEMIT(" %s", format_dev_t(buf, tc->origin_dev->bdev->bd_dev));
			break;
		}
	}

<<<<<<< HEAD
<<<<<<< HEAD
	return 0;
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	return;

err:
	DMEMIT("Error");
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
}

static int thin_iterate_devices(struct dm_target *ti,
				iterate_devices_callout_fn fn, void *data)
{
<<<<<<< HEAD
	dm_block_t blocks;
	struct thin_c *tc = ti->private;
=======
	sector_t blocks;
	struct thin_c *tc = ti->private;
	struct pool *pool = tc->pool;
>>>>>>> refs/remotes/origin/master

	/*
	 * We can't call dm_pool_get_data_dev_size() since that blocks.  So
	 * we follow a more convoluted path through to the pool's target.
	 */
<<<<<<< HEAD
	if (!tc->pool->ti)
		return 0;	/* nothing is bound */

	blocks = tc->pool->ti->len >> tc->pool->block_shift;
	if (blocks)
		return fn(ti, tc->pool_dev, 0, tc->pool->sectors_per_block * blocks, data);
=======
	if (!pool->ti)
		return 0;	/* nothing is bound */

	blocks = pool->ti->len;
	(void) sector_div(blocks, pool->sectors_per_block);
	if (blocks)
		return fn(ti, tc->pool_dev, 0, pool->sectors_per_block * blocks, data);
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
static void thin_io_hints(struct dm_target *ti, struct queue_limits *limits)
{
	struct thin_c *tc = ti->private;
	struct pool *pool = tc->pool;

	blk_limits_io_min(limits, 0);
	blk_limits_io_opt(limits, pool->sectors_per_block << SECTOR_SHIFT);
	set_discard_limits(pool, limits);
}

static struct target_type thin_target = {
	.name = "thin",
	.version = {1, 1, 0},
=======
static struct target_type thin_target = {
	.name = "thin",
	.version = {1, 9, 0},
>>>>>>> refs/remotes/origin/master
	.module	= THIS_MODULE,
	.ctr = thin_ctr,
	.dtr = thin_dtr,
	.map = thin_map,
	.end_io = thin_endio,
	.postsuspend = thin_postsuspend,
	.status = thin_status,
	.iterate_devices = thin_iterate_devices,
<<<<<<< HEAD
	.io_hints = thin_io_hints,
=======
>>>>>>> refs/remotes/origin/master
};

/*----------------------------------------------------------------*/

static int __init dm_thin_init(void)
{
	int r;

	pool_table_init();

	r = dm_register_target(&thin_target);
	if (r)
		return r;

	r = dm_register_target(&pool_target);
	if (r)
<<<<<<< HEAD
		dm_unregister_target(&thin_target);
=======
		goto bad_pool_target;

	r = -ENOMEM;

	_new_mapping_cache = KMEM_CACHE(dm_thin_new_mapping, 0);
	if (!_new_mapping_cache)
		goto bad_new_mapping_cache;

	return 0;

bad_new_mapping_cache:
	dm_unregister_target(&pool_target);
bad_pool_target:
	dm_unregister_target(&thin_target);
>>>>>>> refs/remotes/origin/master

	return r;
}

static void dm_thin_exit(void)
{
	dm_unregister_target(&thin_target);
	dm_unregister_target(&pool_target);
<<<<<<< HEAD
=======

	kmem_cache_destroy(_new_mapping_cache);
>>>>>>> refs/remotes/origin/master
}

module_init(dm_thin_init);
module_exit(dm_thin_exit);

MODULE_DESCRIPTION(DM_NAME " thin provisioning target");
MODULE_AUTHOR("Joe Thornber <dm-devel@redhat.com>");
MODULE_LICENSE("GPL");
