/*
 * Compressed RAM block device
 *
 * Copyright (C) 2008, 2009, 2010  Nitin Gupta
 *
 * This code is released using a dual license strategy: BSD/GPL
 * You can choose the licence that better fits your requirements.
 *
 * Released under the terms of 3-clause BSD License
 * Released under the terms of GNU General Public License Version 2.0
 *
 * Project home: http://compcache.googlecode.com
 */

#ifndef _ZRAM_DRV_H_
#define _ZRAM_DRV_H_

#include <linux/spinlock.h>
#include <linux/mutex.h>

<<<<<<< HEAD
<<<<<<< HEAD
#include "xvmalloc.h"
=======
#include "../zsmalloc/zsmalloc.h"
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include "../zsmalloc/zsmalloc.h"
>>>>>>> refs/remotes/origin/master

/*
 * Some arbitrary value. This is just to catch
 * invalid value for num_devices module parameter.
 */
static const unsigned max_num_devices = 32;

<<<<<<< HEAD
<<<<<<< HEAD
/*
 * Stored at beginning of each compressed object.
 *
 * It stores back-reference to table entry which points to this
 * object. This is required to support memory defragmentation.
 */
struct zobj_header {
#if 0
	u32 table_idx;
#endif
};

/*-- Configurable parameters */

/* Default zram disk size: 25% of total RAM */
static const unsigned default_disksize_perc_ram = 25;

=======
/*-- Configurable parameters */

>>>>>>> refs/remotes/origin/master
=======
/*-- Configurable parameters */

>>>>>>> refs/remotes/origin/cm-11.0
/*
 * Pages that compress to size greater than this are stored
 * uncompressed in memory.
 */
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
static const unsigned max_zpage_size = PAGE_SIZE / 4 * 3;

/*
 * NOTE: max_zpage_size must be less than or equal to:
 *   XV_MAX_ALLOC_SIZE - sizeof(struct zobj_header)
=======
=======
>>>>>>> refs/remotes/origin/master
static const size_t max_zpage_size = PAGE_SIZE / 4 * 3;

/*
 * NOTE: max_zpage_size must be less than or equal to:
<<<<<<< HEAD
 *   ZS_MAX_ALLOC_SIZE - sizeof(struct zobj_header)
>>>>>>> refs/remotes/origin/cm-10.0
 * otherwise, xv_malloc() would always return failure.
=======
 *   ZS_MAX_ALLOC_SIZE. Otherwise, zs_malloc() would
 * always return failure.
>>>>>>> refs/remotes/origin/master
=======
static const size_t max_zpage_size = PAGE_SIZE / 10 * 9;

/*
 * NOTE: max_zpage_size must be less than or equal to:
 *   ZS_MAX_ALLOC_SIZE. Otherwise, zs_malloc() would
 * always return failure.
>>>>>>> refs/remotes/origin/cm-11.0
 */

/*-- End of configurable params */

#define SECTOR_SHIFT		9
#define SECTOR_SIZE		(1 << SECTOR_SHIFT)
#define SECTORS_PER_PAGE_SHIFT	(PAGE_SHIFT - SECTOR_SHIFT)
#define SECTORS_PER_PAGE	(1 << SECTORS_PER_PAGE_SHIFT)
<<<<<<< HEAD
<<<<<<< HEAD
#define ZRAM_LOGICAL_BLOCK_SIZE	4096
=======
=======
>>>>>>> refs/remotes/origin/master
#define ZRAM_LOGICAL_BLOCK_SHIFT 12
#define ZRAM_LOGICAL_BLOCK_SIZE	(1 << ZRAM_LOGICAL_BLOCK_SHIFT)
#define ZRAM_SECTOR_PER_LOGICAL_BLOCK	\
	(1 << (ZRAM_LOGICAL_BLOCK_SHIFT - SECTOR_SHIFT))
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

/* Flags for zram pages (table[page_no].flags) */
enum zram_pageflags {
<<<<<<< HEAD
	/* Page is stored uncompressed */
	ZRAM_UNCOMPRESSED,

=======

/* Flags for zram pages (table[page_no].flags) */
enum zram_pageflags {
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	/* Page consists entirely of zeros */
	ZRAM_ZERO,

	__NR_ZRAM_PAGEFLAGS,
};

/*-- Data structures */

/* Allocated for each disk page */
struct table {
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	struct page *page;
	u16 offset;
=======
	void *handle;
=======
	unsigned long handle;
>>>>>>> refs/remotes/origin/cm-11.0
	u16 size;	/* object size (excluding header) */
>>>>>>> refs/remotes/origin/cm-10.0
	u8 count;	/* object ref count (not yet used) */
	u8 flags;
} __aligned(4);

/*
 * All 64bit fields should only be manipulated by 64bit atomic accessors.
 * All modifications to 32bit counter should be protected by zram->lock.
 */
struct zram_stats {
	atomic64_t compr_size;	/* compressed size of pages stored */
	atomic64_t num_reads;	/* failed + successful */
	atomic64_t num_writes;	/* --do-- */
	atomic64_t failed_reads;	/* should NEVER! happen */
	atomic64_t failed_writes;	/* can happen when memory is too low */
	atomic64_t invalid_io;	/* non-page-aligned I/O requests */
	atomic64_t notify_free;	/* no. of swap slot free notifications */
	u32 pages_zero;		/* no. of zero filled pages */
	u32 pages_stored;	/* no. of pages currently stored */
	u32 good_compress;	/* % of pages with compression ratio<=50% */
	u32 bad_compress;	/* % of pages with compression ratio>=75% */
};

<<<<<<< HEAD
struct zram {
<<<<<<< HEAD
	struct xv_pool *mem_pool;
=======
	struct zs_pool *mem_pool;
>>>>>>> refs/remotes/origin/cm-10.0
	void *compress_workmem;
	void *compress_buffer;
	struct table *table;
	spinlock_t stat64_lock;	/* protect 64-bit stats */
<<<<<<< HEAD
	struct mutex lock;	/* protect compression buffers against
				 * concurrent writes */
	struct request_queue *queue;
	struct gendisk *disk;
	int init_done;
	/* Prevent concurrent execution of device init and reset */
	struct mutex init_lock;
=======
	struct rw_semaphore lock; /* protect compression buffers and table
				   * against concurrent read and writes */
=======
	unsigned long handle;
	u16 size;	/* object size (excluding header) */
	u8 count;	/* object ref count (not yet used) */
	u8 flags;
} __aligned(4);

/*
 * All 64bit fields should only be manipulated by 64bit atomic accessors.
 * All modifications to 32bit counter should be protected by zram->lock.
 */
struct zram_stats {
	atomic64_t compr_size;	/* compressed size of pages stored */
	atomic64_t num_reads;	/* failed + successful */
	atomic64_t num_writes;	/* --do-- */
	atomic64_t failed_reads;	/* should NEVER! happen */
	atomic64_t failed_writes;	/* can happen when memory is too low */
	atomic64_t invalid_io;	/* non-page-aligned I/O requests */
	atomic64_t notify_free;	/* no. of swap slot free notifications */
	u32 pages_zero;		/* no. of zero filled pages */
	u32 pages_stored;	/* no. of pages currently stored */
	u32 good_compress;	/* % of pages with compression ratio<=50% */
	u32 bad_compress;	/* % of pages with compression ratio>=75% */
};

struct zram_meta {
	void *compress_workmem;
	void *compress_buffer;
	struct table *table;
=======
struct zram_meta {
	void *compress_workmem;
	void *compress_buffer;
	struct table *table;
>>>>>>> refs/remotes/origin/cm-11.0
	struct zs_pool *mem_pool;
};

struct zram_slot_free {
	unsigned long index;
	struct zram_slot_free *next;
};

struct zram {
	struct zram_meta *meta;
	struct rw_semaphore lock; /* protect compression buffers, table,
				   * 32bit stat counters against concurrent
				   * notifications, reads and writes */

	struct work_struct free_work;  /* handle pending free request */
	struct zram_slot_free *slot_free_rq; /* list head of free request */

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	struct request_queue *queue;
	struct gendisk *disk;
	int init_done;
	/* Prevent concurrent execution of device init, reset and R/W request */
	struct rw_semaphore init_lock;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * This is the limit on amount of *uncompressed* worth of data
	 * we can store in a disk.
	 */
	u64 disksize;	/* bytes */
<<<<<<< HEAD
<<<<<<< HEAD

	struct zram_stats stats;
};

<<<<<<< HEAD
extern struct zram *devices;
extern unsigned int num_devices;
=======
extern struct zram *zram_devices;
unsigned int zram_get_num_devices(void);
>>>>>>> refs/remotes/origin/cm-10.0
#ifdef CONFIG_SYSFS
extern struct attribute_group zram_disk_attr_group;
#endif

extern int zram_init_device(struct zram *zram);
<<<<<<< HEAD
extern void zram_reset_device(struct zram *zram);
=======
extern void __zram_reset_device(struct zram *zram);
>>>>>>> refs/remotes/origin/cm-10.0

=======
	spinlock_t slot_free_lock;

	struct zram_stats stats;
};
>>>>>>> refs/remotes/origin/master
=======
	spinlock_t slot_free_lock;

	struct zram_stats stats;
};
>>>>>>> refs/remotes/origin/cm-11.0
#endif
