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

#define KMSG_COMPONENT "zram"
#define pr_fmt(fmt) KMSG_COMPONENT ": " fmt

#ifdef CONFIG_ZRAM_DEBUG
#define DEBUG
#endif

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/bio.h>
#include <linux/bitops.h>
#include <linux/blkdev.h>
#include <linux/buffer_head.h>
#include <linux/device.h>
#include <linux/genhd.h>
#include <linux/highmem.h>
#include <linux/slab.h>
#include <linux/lzo.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <linux/ratelimit.h>

#include "zram_drv.h"

/* Globals */
static int zram_major;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
struct zram *devices;

/* Module params (documentation at end) */
unsigned int num_devices;
=======
struct zram *zram_devices;

/* Module params (documentation at end) */
static unsigned int num_devices;
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct zram *zram_devices;

/*
 * We don't need to see memory allocation errors more than once every 1
 * second to know that a problem is occurring.
 */
#define ALLOC_ERROR_LOG_RATE_MS 1000

/* Module params (documentation at end) */
static unsigned int num_devices = 1;

static inline struct zram *dev_to_zram(struct device *dev)
{
	return (struct zram *)dev_to_disk(dev)->private_data;
}

static ssize_t disksize_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct zram *zram = dev_to_zram(dev);

	return sprintf(buf, "%llu\n", zram->disksize);
}

static ssize_t initstate_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct zram *zram = dev_to_zram(dev);

	return sprintf(buf, "%u\n", zram->init_done);
}
>>>>>>> refs/remotes/origin/cm-11.0

static ssize_t num_reads_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct zram *zram = dev_to_zram(dev);

	return sprintf(buf, "%llu\n",
			(u64)atomic64_read(&zram->stats.num_reads));
}

static ssize_t num_writes_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct zram *zram = dev_to_zram(dev);

	return sprintf(buf, "%llu\n",
			(u64)atomic64_read(&zram->stats.num_writes));
}

static ssize_t invalid_io_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct zram *zram = dev_to_zram(dev);

	return sprintf(buf, "%llu\n",
			(u64)atomic64_read(&zram->stats.invalid_io));
}

static ssize_t notify_free_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct zram *zram = dev_to_zram(dev);

	return sprintf(buf, "%llu\n",
			(u64)atomic64_read(&zram->stats.notify_free));
}

static ssize_t zero_pages_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct zram *zram = dev_to_zram(dev);

	return sprintf(buf, "%u\n", zram->stats.pages_zero);
}

static ssize_t orig_data_size_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct zram *zram = dev_to_zram(dev);

	return sprintf(buf, "%llu\n",
		(u64)(zram->stats.pages_stored) << PAGE_SHIFT);
}

static ssize_t compr_data_size_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct zram *zram = dev_to_zram(dev);

	return sprintf(buf, "%llu\n",
			(u64)atomic64_read(&zram->stats.compr_size));
}

static ssize_t mem_used_total_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	u64 val = 0;
	struct zram *zram = dev_to_zram(dev);
	struct zram_meta *meta = zram->meta;

	down_read(&zram->init_lock);
	if (zram->init_done)
		val = zs_get_total_size_bytes(meta->mem_pool);
	up_read(&zram->init_lock);

	return sprintf(buf, "%llu\n", val);
}

static int zram_test_flag(struct zram_meta *meta, u32 index,
			enum zram_pageflags flag)
{
	return meta->table[index].flags & BIT(flag);
}

static void zram_set_flag(struct zram_meta *meta, u32 index,
			enum zram_pageflags flag)
{
	meta->table[index].flags |= BIT(flag);
}

static void zram_clear_flag(struct zram_meta *meta, u32 index,
			enum zram_pageflags flag)
{
	meta->table[index].flags &= ~BIT(flag);
}

static inline int is_partial_io(struct bio_vec *bvec)
{
	return bvec->bv_len != PAGE_SIZE;
}

/*
 * Check if request is within bounds and aligned on zram logical blocks.
 */
static inline int valid_io_request(struct zram *zram, struct bio *bio)
{
	u64 start, end, bound;

	/* unaligned request */
	if (unlikely(bio->bi_sector & (ZRAM_SECTOR_PER_LOGICAL_BLOCK - 1)))
		return 0;
	if (unlikely(bio->bi_size & (ZRAM_LOGICAL_BLOCK_SIZE - 1)))
		return 0;

	start = bio->bi_sector;
	end = start + (bio->bi_size >> SECTOR_SHIFT);
	bound = zram->disksize >> SECTOR_SHIFT;
	/* out of range range */
	if (unlikely(start >= bound || end > bound || start > end))
		return 0;

	/* I/O request is valid */
	return 1;
}

static void zram_meta_free(struct zram_meta *meta)
{
	zs_destroy_pool(meta->mem_pool);
	kfree(meta->compress_workmem);
	free_pages((unsigned long)meta->compress_buffer, 1);
	vfree(meta->table);
	kfree(meta);
}

static struct zram_meta *zram_meta_alloc(u64 disksize)
{
<<<<<<< HEAD
<<<<<<< HEAD
	u32 clen;
	void *obj;

	struct page *page = zram->table[index].page;
	u32 offset = zram->table[index].offset;

	if (unlikely(!page)) {
=======
	void *handle = zram->table[index].handle;

	if (unlikely(!handle)) {
>>>>>>> refs/remotes/origin/cm-10.0
		/*
		 * No memory is allocated for zero filled pages.
		 * Simply clear zero page flag.
		 */
		if (zram_test_flag(zram, index, ZRAM_ZERO)) {
			zram_clear_flag(zram, index, ZRAM_ZERO);
			zram_stat_dec(&zram->stats.pages_zero);
		}
		return;
	}

	if (unlikely(zram_test_flag(zram, index, ZRAM_UNCOMPRESSED))) {
<<<<<<< HEAD
		clen = PAGE_SIZE;
		__free_page(page);
=======
		__free_page(handle);
>>>>>>> refs/remotes/origin/cm-10.0
		zram_clear_flag(zram, index, ZRAM_UNCOMPRESSED);
		zram_stat_dec(&zram->stats.pages_expand);
		goto out;
	}

<<<<<<< HEAD
	obj = kmap_atomic(page, KM_USER0) + offset;
	clen = xv_get_object_size(obj) - sizeof(struct zobj_header);
	kunmap_atomic(obj, KM_USER0);

	xv_free(zram->mem_pool, page, offset);
	if (clen <= PAGE_SIZE / 2)
		zram_stat_dec(&zram->stats.good_compress);

out:
	zram_stat64_sub(zram, &zram->stats.compr_size, clen);
	zram_stat_dec(&zram->stats.pages_stored);

	zram->table[index].page = NULL;
	zram->table[index].offset = 0;
}

static void handle_zero_page(struct page *page)
{
	void *user_mem;

	user_mem = kmap_atomic(page, KM_USER0);
	memset(user_mem, 0, PAGE_SIZE);
	kunmap_atomic(user_mem, KM_USER0);
=======
	zs_free(zram->mem_pool, handle);
=======
	size_t num_pages;
	struct zram_meta *meta = kmalloc(sizeof(*meta), GFP_KERNEL);
	if (!meta)
		goto out;

	meta->compress_workmem = kzalloc(LZO1X_MEM_COMPRESS, GFP_KERNEL);
	if (!meta->compress_workmem)
		goto free_meta;

	meta->compress_buffer =
		(void *)__get_free_pages(GFP_KERNEL | __GFP_ZERO, 1);
	if (!meta->compress_buffer) {
		pr_err("Error allocating compressor buffer space\n");
		goto free_workmem;
	}

	num_pages = disksize >> PAGE_SHIFT;
	meta->table = vzalloc(num_pages * sizeof(*meta->table));
	if (!meta->table) {
		pr_err("Error allocating zram address table\n");
		goto free_buffer;
	}

	meta->mem_pool = zs_create_pool(GFP_NOIO | __GFP_HIGHMEM |
					__GFP_NOWARN);
	if (!meta->mem_pool) {
		pr_err("Error creating memory pool\n");
		goto free_table;
	}
>>>>>>> refs/remotes/origin/cm-11.0

	return meta;

free_table:
	vfree(meta->table);
free_buffer:
	free_pages((unsigned long)meta->compress_buffer, 1);
free_workmem:
	kfree(meta->compress_workmem);
free_meta:
	kfree(meta);
	meta = NULL;
out:
	return meta;
}

static void update_position(u32 *index, int *offset, struct bio_vec *bvec)
{
	if (*offset + bvec->bv_len >= PAGE_SIZE)
		(*index)++;
	*offset = (*offset + bvec->bv_len) % PAGE_SIZE;
}

static int page_zero_filled(void *ptr)
{
	unsigned int pos;
	unsigned long *page;

<<<<<<< HEAD
	user_mem = kmap_atomic(page);
	memset(user_mem + bvec->bv_offset, 0, bvec->bv_len);
	kunmap_atomic(user_mem);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	page = (unsigned long *)ptr;
>>>>>>> refs/remotes/origin/cm-11.0

	for (pos = 0; pos != PAGE_SIZE / sizeof(*page); pos++) {
		if (page[pos])
			return 0;
	}

	return 1;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void handle_uncompressed_page(struct zram *zram,
				struct page *page, u32 index)
{
	unsigned char *user_mem, *cmem;

	user_mem = kmap_atomic(page, KM_USER0);
	cmem = kmap_atomic(zram->table[index].page, KM_USER1) +
			zram->table[index].offset;

	memcpy(user_mem, cmem, PAGE_SIZE);
	kunmap_atomic(user_mem, KM_USER0);
	kunmap_atomic(cmem, KM_USER1);
=======
static void handle_uncompressed_page(struct zram *zram, struct bio_vec *bvec,
				     u32 index, int offset)
=======
static void handle_zero_page(struct bio_vec *bvec)
>>>>>>> refs/remotes/origin/cm-11.0
{
	struct page *page = bvec->bv_page;
	void *user_mem;

	user_mem = kmap_atomic(page);
	if (is_partial_io(bvec))
		memset(user_mem + bvec->bv_offset, 0, bvec->bv_len);
	else
		clear_page(user_mem);
	kunmap_atomic(user_mem);
>>>>>>> refs/remotes/origin/cm-10.0

	flush_dcache_page(page);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void zram_read(struct zram *zram, struct bio *bio)
{

	int i;
	u32 index;
	struct bio_vec *bvec;

	zram_stat64_inc(zram, &zram->stats.num_reads);
	index = bio->bi_sector >> SECTORS_PER_PAGE_SHIFT;

	bio_for_each_segment(bvec, bio, i) {
		int ret;
		size_t clen;
		struct page *page;
		struct zobj_header *zheader;
		unsigned char *user_mem, *cmem;

		page = bvec->bv_page;

		if (zram_test_flag(zram, index, ZRAM_ZERO)) {
			handle_zero_page(page);
			index++;
			continue;
		}

		/* Requested page is not present in compressed area */
		if (unlikely(!zram->table[index].page)) {
			pr_debug("Read before write: sector=%lu, size=%u",
				(ulong)(bio->bi_sector), bio->bi_size);
			handle_zero_page(page);
			index++;
			continue;
		}

		/* Page is stored uncompressed since it's incompressible */
		if (unlikely(zram_test_flag(zram, index, ZRAM_UNCOMPRESSED))) {
			handle_uncompressed_page(zram, page, index);
			index++;
			continue;
		}

		user_mem = kmap_atomic(page, KM_USER0);
		clen = PAGE_SIZE;

		cmem = kmap_atomic(zram->table[index].page, KM_USER1) +
				zram->table[index].offset;

		ret = lzo1x_decompress_safe(
			cmem + sizeof(*zheader),
			xv_get_object_size(cmem) - sizeof(*zheader),
			user_mem, &clen);

		kunmap_atomic(user_mem, KM_USER0);
		kunmap_atomic(cmem, KM_USER1);

		/* Should NEVER happen. Return bio error if it does. */
		if (unlikely(ret != LZO_E_OK)) {
			pr_err("Decompression failed! err=%d, page=%u\n",
				ret, index);
			zram_stat64_inc(zram, &zram->stats.failed_reads);
			goto out;
		}

		flush_dcache_page(page);
		index++;
	}

	set_bit(BIO_UPTODATE, &bio->bi_flags);
	bio_endio(bio, 0);
	return;

out:
	bio_io_error(bio);
}

static void zram_write(struct zram *zram, struct bio *bio)
{
	int i;
	u32 index;
	struct bio_vec *bvec;

	zram_stat64_inc(zram, &zram->stats.num_writes);
	index = bio->bi_sector >> SECTORS_PER_PAGE_SHIFT;

	bio_for_each_segment(bvec, bio, i) {
		int ret;
		u32 offset;
		size_t clen;
		struct zobj_header *zheader;
		struct page *page, *page_store;
		unsigned char *user_mem, *cmem, *src;

		page = bvec->bv_page;
		src = zram->compress_buffer;

		/*
		 * System overwrites unused sectors. Free memory associated
		 * with this sector now.
		 */
		if (zram->table[index].page ||
				zram_test_flag(zram, index, ZRAM_ZERO))
			zram_free_page(zram, index);

		mutex_lock(&zram->lock);

		user_mem = kmap_atomic(page, KM_USER0);
		if (page_zero_filled(user_mem)) {
			kunmap_atomic(user_mem, KM_USER0);
			mutex_unlock(&zram->lock);
			zram_stat_inc(&zram->stats.pages_zero);
			zram_set_flag(zram, index, ZRAM_ZERO);
			index++;
			continue;
		}

		ret = lzo1x_1_compress(user_mem, PAGE_SIZE, src, &clen,
					zram->compress_workmem);

		kunmap_atomic(user_mem, KM_USER0);

		if (unlikely(ret != LZO_E_OK)) {
			mutex_unlock(&zram->lock);
			pr_err("Compression failed! err=%d\n", ret);
			zram_stat64_inc(zram, &zram->stats.failed_writes);
			goto out;
		}

		/*
		 * Page is incompressible. Store it as-is (uncompressed)
		 * since we do not want to return too many disk write
		 * errors which has side effect of hanging the system.
		 */
		if (unlikely(clen > max_zpage_size)) {
			clen = PAGE_SIZE;
			page_store = alloc_page(GFP_NOIO | __GFP_HIGHMEM);
			if (unlikely(!page_store)) {
				mutex_unlock(&zram->lock);
				pr_info("Error allocating memory for "
					"incompressible page: %u\n", index);
				zram_stat64_inc(zram,
					&zram->stats.failed_writes);
				goto out;
			}

			offset = 0;
			zram_set_flag(zram, index, ZRAM_UNCOMPRESSED);
			zram_stat_inc(&zram->stats.pages_expand);
			zram->table[index].page = page_store;
			src = kmap_atomic(page, KM_USER0);
			goto memstore;
		}

		if (xv_malloc(zram->mem_pool, clen + sizeof(*zheader),
				&zram->table[index].page, &offset,
				GFP_NOIO | __GFP_HIGHMEM)) {
			mutex_unlock(&zram->lock);
			pr_info("Error allocating memory for compressed "
				"page: %u, size=%zu\n", index, clen);
			zram_stat64_inc(zram, &zram->stats.failed_writes);
			goto out;
		}

memstore:
		zram->table[index].offset = offset;

		cmem = kmap_atomic(zram->table[index].page, KM_USER1) +
				zram->table[index].offset;

#if 0
		/* Back-reference needed for memory defragmentation */
		if (!zram_test_flag(zram, index, ZRAM_UNCOMPRESSED)) {
			zheader = (struct zobj_header *)cmem;
			zheader->table_idx = index;
			cmem += sizeof(*zheader);
		}
#endif

		memcpy(cmem, src, clen);

		kunmap_atomic(cmem, KM_USER1);
		if (unlikely(zram_test_flag(zram, index, ZRAM_UNCOMPRESSED)))
			kunmap_atomic(src, KM_USER0);

		/* Update stats */
		zram_stat64_add(zram, &zram->stats.compr_size, clen);
		zram_stat_inc(&zram->stats.pages_stored);
		if (clen <= PAGE_SIZE / 2)
			zram_stat_inc(&zram->stats.good_compress);

		mutex_unlock(&zram->lock);
		index++;
=======
static inline int is_partial_io(struct bio_vec *bvec)
{
	return bvec->bv_len != PAGE_SIZE;
=======
static struct zram *zram_devices;

/* Module params (documentation at end) */
static unsigned int num_devices = 1;

static inline struct zram *dev_to_zram(struct device *dev)
{
	return (struct zram *)dev_to_disk(dev)->private_data;
}

static ssize_t disksize_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct zram *zram = dev_to_zram(dev);

	return sprintf(buf, "%llu\n", zram->disksize);
}

static ssize_t initstate_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct zram *zram = dev_to_zram(dev);

	return sprintf(buf, "%u\n", zram->init_done);
}

static ssize_t num_reads_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct zram *zram = dev_to_zram(dev);

	return sprintf(buf, "%llu\n",
			(u64)atomic64_read(&zram->stats.num_reads));
}

static ssize_t num_writes_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct zram *zram = dev_to_zram(dev);

	return sprintf(buf, "%llu\n",
			(u64)atomic64_read(&zram->stats.num_writes));
}

static ssize_t invalid_io_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct zram *zram = dev_to_zram(dev);

	return sprintf(buf, "%llu\n",
			(u64)atomic64_read(&zram->stats.invalid_io));
}

static ssize_t notify_free_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct zram *zram = dev_to_zram(dev);

	return sprintf(buf, "%llu\n",
			(u64)atomic64_read(&zram->stats.notify_free));
}

static ssize_t zero_pages_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct zram *zram = dev_to_zram(dev);

	return sprintf(buf, "%u\n", zram->stats.pages_zero);
}

static ssize_t orig_data_size_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct zram *zram = dev_to_zram(dev);

	return sprintf(buf, "%llu\n",
		(u64)(zram->stats.pages_stored) << PAGE_SHIFT);
}

static ssize_t compr_data_size_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct zram *zram = dev_to_zram(dev);

	return sprintf(buf, "%llu\n",
			(u64)atomic64_read(&zram->stats.compr_size));
}

static ssize_t mem_used_total_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	u64 val = 0;
	struct zram *zram = dev_to_zram(dev);
	struct zram_meta *meta = zram->meta;

	down_read(&zram->init_lock);
	if (zram->init_done)
		val = zs_get_total_size_bytes(meta->mem_pool);
	up_read(&zram->init_lock);

	return sprintf(buf, "%llu\n", val);
}

static int zram_test_flag(struct zram_meta *meta, u32 index,
			enum zram_pageflags flag)
{
	return meta->table[index].flags & BIT(flag);
}

static void zram_set_flag(struct zram_meta *meta, u32 index,
			enum zram_pageflags flag)
{
	meta->table[index].flags |= BIT(flag);
}

static void zram_clear_flag(struct zram_meta *meta, u32 index,
			enum zram_pageflags flag)
{
	meta->table[index].flags &= ~BIT(flag);
}

static inline int is_partial_io(struct bio_vec *bvec)
{
	return bvec->bv_len != PAGE_SIZE;
}

/*
 * Check if request is within bounds and aligned on zram logical blocks.
 */
static inline int valid_io_request(struct zram *zram, struct bio *bio)
{
	u64 start, end, bound;

	/* unaligned request */
	if (unlikely(bio->bi_sector & (ZRAM_SECTOR_PER_LOGICAL_BLOCK - 1)))
		return 0;
	if (unlikely(bio->bi_size & (ZRAM_LOGICAL_BLOCK_SIZE - 1)))
		return 0;

	start = bio->bi_sector;
	end = start + (bio->bi_size >> SECTOR_SHIFT);
	bound = zram->disksize >> SECTOR_SHIFT;
	/* out of range range */
	if (unlikely(start >= bound || end > bound || start > end))
		return 0;

	/* I/O request is valid */
	return 1;
}

static void zram_meta_free(struct zram_meta *meta)
{
	zs_destroy_pool(meta->mem_pool);
	kfree(meta->compress_workmem);
	free_pages((unsigned long)meta->compress_buffer, 1);
	vfree(meta->table);
	kfree(meta);
}

static struct zram_meta *zram_meta_alloc(u64 disksize)
{
	size_t num_pages;
	struct zram_meta *meta = kmalloc(sizeof(*meta), GFP_KERNEL);
	if (!meta)
		goto out;

	meta->compress_workmem = kzalloc(LZO1X_MEM_COMPRESS, GFP_KERNEL);
	if (!meta->compress_workmem)
		goto free_meta;

	meta->compress_buffer =
		(void *)__get_free_pages(GFP_KERNEL | __GFP_ZERO, 1);
	if (!meta->compress_buffer) {
		pr_err("Error allocating compressor buffer space\n");
		goto free_workmem;
	}

	num_pages = disksize >> PAGE_SHIFT;
	meta->table = vzalloc(num_pages * sizeof(*meta->table));
	if (!meta->table) {
		pr_err("Error allocating zram address table\n");
		goto free_buffer;
	}

	meta->mem_pool = zs_create_pool(GFP_NOIO | __GFP_HIGHMEM);
	if (!meta->mem_pool) {
		pr_err("Error creating memory pool\n");
		goto free_table;
	}

	return meta;

free_table:
	vfree(meta->table);
free_buffer:
	free_pages((unsigned long)meta->compress_buffer, 1);
free_workmem:
	kfree(meta->compress_workmem);
free_meta:
	kfree(meta);
	meta = NULL;
out:
	return meta;
}

static void update_position(u32 *index, int *offset, struct bio_vec *bvec)
{
	if (*offset + bvec->bv_len >= PAGE_SIZE)
		(*index)++;
	*offset = (*offset + bvec->bv_len) % PAGE_SIZE;
}

static int page_zero_filled(void *ptr)
{
	unsigned int pos;
	unsigned long *page;

	page = (unsigned long *)ptr;

	for (pos = 0; pos != PAGE_SIZE / sizeof(*page); pos++) {
		if (page[pos])
			return 0;
	}

	return 1;
}

static void handle_zero_page(struct bio_vec *bvec)
{
	struct page *page = bvec->bv_page;
	void *user_mem;

	user_mem = kmap_atomic(page);
	if (is_partial_io(bvec))
		memset(user_mem + bvec->bv_offset, 0, bvec->bv_len);
	else
		clear_page(user_mem);
	kunmap_atomic(user_mem);

	flush_dcache_page(page);
}

static void zram_free_page(struct zram *zram, size_t index)
{
	struct zram_meta *meta = zram->meta;
	unsigned long handle = meta->table[index].handle;
	u16 size = meta->table[index].size;

	if (unlikely(!handle)) {
		/*
		 * No memory is allocated for zero filled pages.
		 * Simply clear zero page flag.
		 */
		if (zram_test_flag(meta, index, ZRAM_ZERO)) {
			zram_clear_flag(meta, index, ZRAM_ZERO);
			zram->stats.pages_zero--;
		}
		return;
	}

	if (unlikely(size > max_zpage_size))
		zram->stats.bad_compress--;

	zs_free(meta->mem_pool, handle);

	if (size <= PAGE_SIZE / 2)
		zram->stats.good_compress--;

	atomic64_sub(meta->table[index].size, &zram->stats.compr_size);
	zram->stats.pages_stored--;

	meta->table[index].handle = 0;
	meta->table[index].size = 0;
}

static int zram_decompress_page(struct zram *zram, char *mem, u32 index)
{
	int ret = LZO_E_OK;
	size_t clen = PAGE_SIZE;
	unsigned char *cmem;
	struct zram_meta *meta = zram->meta;
	unsigned long handle = meta->table[index].handle;

	if (!handle || zram_test_flag(meta, index, ZRAM_ZERO)) {
		clear_page(mem);
		return 0;
	}

	cmem = zs_map_object(meta->mem_pool, handle, ZS_MM_RO);
	if (meta->table[index].size == PAGE_SIZE)
		copy_page(mem, cmem);
	else
		ret = lzo1x_decompress_safe(cmem, meta->table[index].size,
						mem, &clen);
	zs_unmap_object(meta->mem_pool, handle);

	/* Should NEVER happen. Return bio error if it does. */
	if (unlikely(ret != LZO_E_OK)) {
		pr_err("Decompression failed! err=%d, page=%u\n", ret, index);
		atomic64_inc(&zram->stats.failed_reads);
		return ret;
	}

	return 0;
>>>>>>> refs/remotes/origin/master
}

static int zram_bvec_read(struct zram *zram, struct bio_vec *bvec,
			  u32 index, int offset, struct bio *bio)
{
	int ret;
<<<<<<< HEAD
	size_t clen;
	struct page *page;
	struct zobj_header *zheader;
	unsigned char *user_mem, *cmem, *uncmem = NULL;

	page = bvec->bv_page;

	if (zram_test_flag(zram, index, ZRAM_ZERO)) {
		handle_zero_page(bvec);
		return 0;
	}

	/* Requested page is not present in compressed area */
	if (unlikely(!zram->table[index].handle)) {
		pr_debug("Read before write: sector=%lu, size=%u",
			 (ulong)(bio->bi_sector), bio->bi_size);
=======
	struct page *page;
	unsigned char *user_mem, *uncmem = NULL;
	struct zram_meta *meta = zram->meta;
	page = bvec->bv_page;

	if (unlikely(!meta->table[index].handle) ||
			zram_test_flag(meta, index, ZRAM_ZERO)) {
>>>>>>> refs/remotes/origin/master
		handle_zero_page(bvec);
		return 0;
	}

<<<<<<< HEAD
	/* Page is stored uncompressed since it's incompressible */
	if (unlikely(zram_test_flag(zram, index, ZRAM_UNCOMPRESSED))) {
		handle_uncompressed_page(zram, bvec, index, offset);
		return 0;
	}

	if (is_partial_io(bvec)) {
		/* Use  a temporary buffer to decompress the page */
		uncmem = kmalloc(PAGE_SIZE, GFP_KERNEL);
		if (!uncmem) {
			pr_info("Error allocating temp memory!\n");
			return -ENOMEM;
		}
	}
=======
	if (is_partial_io(bvec))
		/* Use  a temporary buffer to decompress the page */
		uncmem = kmalloc(PAGE_SIZE, GFP_NOIO);
>>>>>>> refs/remotes/origin/master

	user_mem = kmap_atomic(page);
	if (!is_partial_io(bvec))
		uncmem = user_mem;
<<<<<<< HEAD
	clen = PAGE_SIZE;
=======
static void zram_free_page(struct zram *zram, size_t index)
{
	struct zram_meta *meta = zram->meta;
	unsigned long handle = meta->table[index].handle;
	u16 size = meta->table[index].size;

	if (unlikely(!handle)) {
		/*
		 * No memory is allocated for zero filled pages.
		 * Simply clear zero page flag.
		 */
		if (zram_test_flag(meta, index, ZRAM_ZERO)) {
			zram_clear_flag(meta, index, ZRAM_ZERO);
			zram->stats.pages_zero--;
		}
		return;
	}

	if (unlikely(size > max_zpage_size))
		zram->stats.bad_compress--;

	zs_free(meta->mem_pool, handle);

	if (size <= PAGE_SIZE / 2)
		zram->stats.good_compress--;

	atomic64_sub(meta->table[index].size, &zram->stats.compr_size);
	zram->stats.pages_stored--;
>>>>>>> refs/remotes/origin/cm-11.0

	meta->table[index].handle = 0;
	meta->table[index].size = 0;
}

static int zram_decompress_page(struct zram *zram, char *mem, u32 index)
{
	int ret = LZO_E_OK;
	size_t clen = PAGE_SIZE;
	unsigned char *cmem;
	struct zram_meta *meta = zram->meta;
	unsigned long handle = meta->table[index].handle;

	if (!handle || zram_test_flag(meta, index, ZRAM_ZERO)) {
		clear_page(mem);
		return 0;
	}

	cmem = zs_map_object(meta->mem_pool, handle, ZS_MM_RO);
	if (meta->table[index].size == PAGE_SIZE)
		copy_page(mem, cmem);
	else
		ret = lzo1x_decompress_safe(cmem, meta->table[index].size,
						mem, &clen);
	zs_unmap_object(meta->mem_pool, handle);

	/* Should NEVER happen. Return bio error if it does. */
	if (unlikely(ret != LZO_E_OK)) {
		pr_err("Decompression failed! err=%d, page=%u\n", ret, index);
		atomic64_inc(&zram->stats.failed_reads);
		return ret;
	}

	return 0;
}

static int zram_bvec_read(struct zram *zram, struct bio_vec *bvec,
			  u32 index, int offset, struct bio *bio)
{
	int ret;
	struct page *page;
	unsigned char *user_mem, *uncmem = NULL;
	struct zram_meta *meta = zram->meta;
	page = bvec->bv_page;

	if (unlikely(!meta->table[index].handle) ||
			zram_test_flag(meta, index, ZRAM_ZERO)) {
		handle_zero_page(bvec);
		return 0;
	}

	if (is_partial_io(bvec))
		/* Use  a temporary buffer to decompress the page */
		uncmem = kmalloc(PAGE_SIZE, GFP_NOIO);

	user_mem = kmap_atomic(page);
	if (!is_partial_io(bvec))
		uncmem = user_mem;

	if (!uncmem) {
		pr_info("Unable to allocate temp memory\n");
		ret = -ENOMEM;
		goto out_cleanup;
	}

	ret = zram_decompress_page(zram, uncmem, index);
	/* Should NEVER happen. Return bio error if it does. */
	if (unlikely(ret != LZO_E_OK))
		goto out_cleanup;

<<<<<<< HEAD
	return 0;
=======

	if (!uncmem) {
		pr_info("Unable to allocate temp memory\n");
		ret = -ENOMEM;
		goto out_cleanup;
	}

	ret = zram_decompress_page(zram, uncmem, index);
	/* Should NEVER happen. Return bio error if it does. */
	if (unlikely(ret != LZO_E_OK))
		goto out_cleanup;

=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (is_partial_io(bvec))
		memcpy(user_mem + bvec->bv_offset, uncmem + offset,
				bvec->bv_len);

	flush_dcache_page(page);
	ret = 0;
out_cleanup:
	kunmap_atomic(user_mem);
	if (is_partial_io(bvec))
		kfree(uncmem);
	return ret;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
}

static int zram_bvec_write(struct zram *zram, struct bio_vec *bvec, u32 index,
			   int offset)
{
<<<<<<< HEAD
<<<<<<< HEAD
	int ret;
	u32 store_offset;
=======
	int ret = 0;
>>>>>>> refs/remotes/origin/cm-11.0
	size_t clen;
	unsigned long handle;
	struct page *page;
	unsigned char *user_mem, *cmem, *src, *uncmem = NULL;
	struct zram_meta *meta = zram->meta;
	static unsigned long zram_rs_time;

	page = bvec->bv_page;
<<<<<<< HEAD
	src = zram->compress_buffer;
=======
	int ret = 0;
	size_t clen;
	unsigned long handle;
	struct page *page;
	unsigned char *user_mem, *cmem, *src, *uncmem = NULL;
	struct zram_meta *meta = zram->meta;

	page = bvec->bv_page;
	src = meta->compress_buffer;
>>>>>>> refs/remotes/origin/master
=======
	src = meta->compress_buffer;
>>>>>>> refs/remotes/origin/cm-11.0

	if (is_partial_io(bvec)) {
		/*
		 * This is a partial IO. We need to read the full page
		 * before to write the changes.
		 */
<<<<<<< HEAD
<<<<<<< HEAD
		uncmem = kmalloc(PAGE_SIZE, GFP_KERNEL);
=======
		uncmem = kmalloc(PAGE_SIZE, GFP_NOIO);
>>>>>>> refs/remotes/origin/cm-11.0
		if (!uncmem) {
			ret = -ENOMEM;
			goto out;
		}
		ret = zram_decompress_page(zram, uncmem, index);
		if (ret)
			goto out;
	}

	user_mem = kmap_atomic(page);

<<<<<<< HEAD
	if (is_partial_io(bvec))
		memcpy(uncmem + offset, user_mem + bvec->bv_offset,
		       bvec->bv_len);
	else
		uncmem = user_mem;

	if (page_zero_filled(uncmem)) {
		kunmap_atomic(user_mem);
		if (is_partial_io(bvec))
			kfree(uncmem);
		zram_stat_inc(&zram->stats.pages_zero);
		zram_set_flag(zram, index, ZRAM_ZERO);
=======
		uncmem = kmalloc(PAGE_SIZE, GFP_NOIO);
		if (!uncmem) {
			ret = -ENOMEM;
			goto out;
		}
		ret = zram_decompress_page(zram, uncmem, index);
		if (ret)
			goto out;
	}

	user_mem = kmap_atomic(page);

=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (is_partial_io(bvec)) {
		memcpy(uncmem + offset, user_mem + bvec->bv_offset,
		       bvec->bv_len);
		kunmap_atomic(user_mem);
		user_mem = NULL;
	} else {
		uncmem = user_mem;
	}

	if (page_zero_filled(uncmem)) {
		kunmap_atomic(user_mem);
		/* Free memory associated with this sector now. */
		zram_free_page(zram, index);

		zram->stats.pages_zero++;
		zram_set_flag(meta, index, ZRAM_ZERO);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
		ret = 0;
		goto out;
	}

<<<<<<< HEAD
<<<<<<< HEAD
=======
	/*
	 * zram_slot_free_notify could miss free so that let's
	 * double check.
	 */
	if (unlikely(meta->table[index].handle ||
			zram_test_flag(meta, index, ZRAM_ZERO)))
		zram_free_page(zram, index);

>>>>>>> refs/remotes/origin/cm-11.0
	ret = lzo1x_1_compress(uncmem, PAGE_SIZE, src, &clen,
			       meta->compress_workmem);

<<<<<<< HEAD
	kunmap_atomic(user_mem);
	if (is_partial_io(bvec))
			kfree(uncmem);
=======
	/*
	 * zram_slot_free_notify could miss free so that let's
	 * double check.
	 */
	if (unlikely(meta->table[index].handle ||
			zram_test_flag(meta, index, ZRAM_ZERO)))
		zram_free_page(zram, index);

	ret = lzo1x_1_compress(uncmem, PAGE_SIZE, src, &clen,
			       meta->compress_workmem);

=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (!is_partial_io(bvec)) {
		kunmap_atomic(user_mem);
		user_mem = NULL;
		uncmem = NULL;
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

	if (unlikely(ret != LZO_E_OK)) {
		pr_err("Compression failed! err=%d\n", ret);
		goto out;
<<<<<<< HEAD
	}

	if (unlikely(clen > max_zpage_size)) {
		zram->stats.bad_compress++;
		clen = PAGE_SIZE;
		src = NULL;
		if (is_partial_io(bvec))
			src = uncmem;
	}

	handle = zs_malloc(meta->mem_pool, clen);
	if (!handle) {
		if (printk_timed_ratelimit(&zram_rs_time,
					   ALLOC_ERROR_LOG_RATE_MS))
			pr_info("Error allocating memory for compressed page: %u, size=%zu\n",
				index, clen);
		ret = -ENOMEM;
		goto out;
	}
	cmem = zs_map_object(meta->mem_pool, handle, ZS_MM_WO);

	if ((clen == PAGE_SIZE) && !is_partial_io(bvec)) {
		src = kmap_atomic(page);
		copy_page(cmem, src);
		kunmap_atomic(src);
	} else {
		memcpy(cmem, src, clen);
	}

	zs_unmap_object(meta->mem_pool, handle);

	/*
	 * Free memory associated with this sector
	 * before overwriting unused sectors.
	 */
	zram_free_page(zram, index);

	meta->table[index].handle = handle;
	meta->table[index].size = clen;

	/* Update stats */
	atomic64_add(clen, &zram->stats.compr_size);
	zram->stats.pages_stored++;
	if (clen <= PAGE_SIZE / 2)
		zram->stats.good_compress++;

out:
	if (is_partial_io(bvec))
		kfree(uncmem);

	if (ret)
		atomic64_inc(&zram->stats.failed_writes);
	return ret;
}

<<<<<<< HEAD
=======
	}

	if (unlikely(clen > max_zpage_size)) {
		zram->stats.bad_compress++;
		clen = PAGE_SIZE;
		src = NULL;
		if (is_partial_io(bvec))
			src = uncmem;
	}

	handle = zs_malloc(meta->mem_pool, clen);
	if (!handle) {
		pr_info("Error allocating memory for compressed page: %u, size=%zu\n",
			index, clen);
		ret = -ENOMEM;
		goto out;
	}
	cmem = zs_map_object(meta->mem_pool, handle, ZS_MM_WO);

	if ((clen == PAGE_SIZE) && !is_partial_io(bvec)) {
		src = kmap_atomic(page);
		copy_page(cmem, src);
		kunmap_atomic(src);
	} else {
		memcpy(cmem, src, clen);
	}

	zs_unmap_object(meta->mem_pool, handle);

	/*
	 * Free memory associated with this sector
	 * before overwriting unused sectors.
	 */
	zram_free_page(zram, index);

	meta->table[index].handle = handle;
	meta->table[index].size = clen;

	/* Update stats */
	atomic64_add(clen, &zram->stats.compr_size);
	zram->stats.pages_stored++;
	if (clen <= PAGE_SIZE / 2)
		zram->stats.good_compress++;

out:
	if (is_partial_io(bvec))
		kfree(uncmem);

	if (ret)
		atomic64_inc(&zram->stats.failed_writes);
	return ret;
}

=======
>>>>>>> refs/remotes/origin/cm-11.0
static void handle_pending_slot_free(struct zram *zram)
{
	struct zram_slot_free *free_rq;

	spin_lock(&zram->slot_free_lock);
	while (zram->slot_free_rq) {
		free_rq = zram->slot_free_rq;
		zram->slot_free_rq = free_rq->next;
		zram_free_page(zram, free_rq->index);
		kfree(free_rq);
	}
	spin_unlock(&zram->slot_free_lock);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
static int zram_bvec_rw(struct zram *zram, struct bio_vec *bvec, u32 index,
			int offset, struct bio *bio, int rw)
{
	int ret;

	if (rw == READ) {
		down_read(&zram->lock);
<<<<<<< HEAD
<<<<<<< HEAD
=======
		handle_pending_slot_free(zram);
>>>>>>> refs/remotes/origin/master
=======
		handle_pending_slot_free(zram);
>>>>>>> refs/remotes/origin/cm-11.0
		ret = zram_bvec_read(zram, bvec, index, offset, bio);
		up_read(&zram->lock);
	} else {
		down_write(&zram->lock);
<<<<<<< HEAD
<<<<<<< HEAD
=======
		handle_pending_slot_free(zram);
>>>>>>> refs/remotes/origin/master
=======
		handle_pending_slot_free(zram);
>>>>>>> refs/remotes/origin/cm-11.0
		ret = zram_bvec_write(zram, bvec, index, offset);
		up_write(&zram->lock);
	}

	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void update_position(u32 *index, int *offset, struct bio_vec *bvec)
{
	if (*offset + bvec->bv_len >= PAGE_SIZE)
		(*index)++;
	*offset = (*offset + bvec->bv_len) % PAGE_SIZE;
=======
static void zram_reset_device(struct zram *zram, bool reset_capacity)
{
=======
static void zram_reset_device(struct zram *zram, bool reset_capacity)
{
>>>>>>> refs/remotes/origin/cm-11.0
	size_t index;
	struct zram_meta *meta;

	flush_work(&zram->free_work);

	down_write(&zram->init_lock);
	if (!zram->init_done) {
		up_write(&zram->init_lock);
		return;
	}

	meta = zram->meta;
	zram->init_done = 0;

	/* Free all pages that are still in this zram device */
	for (index = 0; index < zram->disksize >> PAGE_SHIFT; index++) {
		unsigned long handle = meta->table[index].handle;
		if (!handle)
			continue;

		zs_free(meta->mem_pool, handle);
	}

	zram_meta_free(zram->meta);
	zram->meta = NULL;
	/* Reset stats */
	memset(&zram->stats, 0, sizeof(zram->stats));

	zram->disksize = 0;
	if (reset_capacity)
		set_capacity(zram->disk, 0);
	up_write(&zram->init_lock);
}

static void zram_init_device(struct zram *zram, struct zram_meta *meta)
{
	if (zram->disksize > 2 * (totalram_pages << PAGE_SHIFT)) {
		pr_info(
		"There is little point creating a zram of greater than "
		"twice the size of memory since we expect a 2:1 compression "
		"ratio. Note that zram uses about 0.1%% of the size of "
		"the disk when not in use so a huge zram is "
		"wasteful.\n"
		"\tMemory Size: %lu kB\n"
		"\tSize you selected: %llu kB\n"
		"Continuing anyway ...\n",
		(totalram_pages << PAGE_SHIFT) >> 10, zram->disksize >> 10
		);
	}

	/* zram devices sort of resembles non-rotational disks */
	queue_flag_set_unlocked(QUEUE_FLAG_NONROT, zram->disk->queue);

	zram->meta = meta;
	zram->init_done = 1;

	pr_debug("Initialization done!\n");
}

static ssize_t disksize_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t len)
{
	u64 disksize;
	struct zram_meta *meta;
	struct zram *zram = dev_to_zram(dev);

	disksize = memparse(buf, NULL);
	if (!disksize)
		return -EINVAL;

	disksize = PAGE_ALIGN(disksize);
	meta = zram_meta_alloc(disksize);
	down_write(&zram->init_lock);
	if (zram->init_done) {
		up_write(&zram->init_lock);
		zram_meta_free(meta);
		pr_info("Cannot change disksize for initialized device\n");
		return -EBUSY;
	}

	zram->disksize = disksize;
	set_capacity(zram->disk, zram->disksize >> SECTOR_SHIFT);
	zram_init_device(zram, meta);
	up_write(&zram->init_lock);

	return len;
}

static ssize_t reset_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t len)
{
	int ret;
	unsigned short do_reset;
	struct zram *zram;
	struct block_device *bdev;

	zram = dev_to_zram(dev);
	bdev = bdget_disk(zram->disk, 0);

<<<<<<< HEAD
	if (!bdev)
		return -ENOMEM;

	/* Do not reset an active device! */
	if (bdev->bd_holders) {
		ret = -EBUSY;
		goto out;
	}

	ret = kstrtou16(buf, 10, &do_reset);
	if (ret)
		goto out;

	if (!do_reset) {
		ret = -EINVAL;
		goto out;
	}

	/* Make sure all pending I/O is finished */
	fsync_bdev(bdev);
	bdput(bdev);

	zram_reset_device(zram, true);
	return len;

out:
	bdput(bdev);
	return ret;
>>>>>>> refs/remotes/origin/master
=======
	/* Do not reset an active device! */
	if (bdev->bd_holders)
		return -EBUSY;

	ret = kstrtou16(buf, 10, &do_reset);
	if (ret)
		return ret;

	if (!do_reset)
		return -EINVAL;

	/* Make sure all pending I/O is finished */
	if (bdev)
		fsync_bdev(bdev);

	zram_reset_device(zram, true);
	return len;
>>>>>>> refs/remotes/origin/cm-11.0
}

static void __zram_make_request(struct zram *zram, struct bio *bio, int rw)
{
	int i, offset;
	u32 index;
	struct bio_vec *bvec;

	switch (rw) {
	case READ:
<<<<<<< HEAD
<<<<<<< HEAD
		zram_stat64_inc(zram, &zram->stats.num_reads);
		break;
	case WRITE:
		zram_stat64_inc(zram, &zram->stats.num_writes);
=======
		atomic64_inc(&zram->stats.num_reads);
		break;
	case WRITE:
		atomic64_inc(&zram->stats.num_writes);
>>>>>>> refs/remotes/origin/master
=======
		atomic64_inc(&zram->stats.num_reads);
		break;
	case WRITE:
		atomic64_inc(&zram->stats.num_writes);
>>>>>>> refs/remotes/origin/cm-11.0
		break;
	}

	index = bio->bi_sector >> SECTORS_PER_PAGE_SHIFT;
	offset = (bio->bi_sector & (SECTORS_PER_PAGE - 1)) << SECTOR_SHIFT;

	bio_for_each_segment(bvec, bio, i) {
		int max_transfer_size = PAGE_SIZE - offset;

		if (bvec->bv_len > max_transfer_size) {
			/*
			 * zram_bvec_rw() can only make operation on a single
			 * zram page. Split the bio vector.
			 */
			struct bio_vec bv;

			bv.bv_page = bvec->bv_page;
			bv.bv_len = max_transfer_size;
			bv.bv_offset = bvec->bv_offset;

			if (zram_bvec_rw(zram, &bv, index, offset, bio, rw) < 0)
				goto out;

			bv.bv_len = bvec->bv_len - max_transfer_size;
			bv.bv_offset += max_transfer_size;
			if (zram_bvec_rw(zram, &bv, index+1, 0, bio, rw) < 0)
				goto out;
		} else
			if (zram_bvec_rw(zram, bvec, index, offset, bio, rw)
			    < 0)
				goto out;

		update_position(&index, &offset, bvec);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	set_bit(BIO_UPTODATE, &bio->bi_flags);
	bio_endio(bio, 0);
	return;

out:
	bio_io_error(bio);
}

/*
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
 * Check if request is within bounds and page aligned.
=======
 * Check if request is within bounds and aligned on zram logical blocks.
>>>>>>> refs/remotes/origin/cm-10.0
 */
static inline int valid_io_request(struct zram *zram, struct bio *bio)
{
	if (unlikely(
		(bio->bi_sector >= (zram->disksize >> SECTOR_SHIFT)) ||
<<<<<<< HEAD
		(bio->bi_sector & (SECTORS_PER_PAGE - 1)) ||
		(bio->bi_size & (PAGE_SIZE - 1)))) {
=======
		(bio->bi_sector & (ZRAM_SECTOR_PER_LOGICAL_BLOCK - 1)) ||
		(bio->bi_size & (ZRAM_LOGICAL_BLOCK_SIZE - 1)))) {
>>>>>>> refs/remotes/origin/cm-10.0

		return 0;
	}

	/* I/O request is valid */
	return 1;
}

/*
=======
>>>>>>> refs/remotes/origin/cm-11.0
 * Handler function for all zram I/O requests.
 */
<<<<<<< HEAD
static int zram_make_request(struct request_queue *queue, struct bio *bio)
{
	struct zram *zram = queue->queuedata;

	if (!valid_io_request(zram, bio)) {
		zram_stat64_inc(zram, &zram->stats.invalid_io);
		bio_io_error(bio);
		return 0;
	}

	if (unlikely(!zram->init_done) && zram_init_device(zram)) {
		bio_io_error(bio);
		return 0;
	}

	switch (bio_data_dir(bio)) {
	case READ:
		zram_read(zram, bio);
		break;

	case WRITE:
		zram_write(zram, bio);
		break;
	}

	return 0;
}

void zram_reset_device(struct zram *zram)
{
	size_t index;

	mutex_lock(&zram->init_lock);
=======
=======
 * Handler function for all zram I/O requests.
 */
>>>>>>> refs/remotes/origin/master
static void zram_make_request(struct request_queue *queue, struct bio *bio)
{
	struct zram *zram = queue->queuedata;

<<<<<<< HEAD
<<<<<<< HEAD
	if (unlikely(!zram->init_done) && zram_init_device(zram))
		goto error;

=======
>>>>>>> refs/remotes/origin/cm-11.0
	down_read(&zram->init_lock);
	if (unlikely(!zram->init_done))
		goto error;

	if (!valid_io_request(zram, bio)) {
<<<<<<< HEAD
		zram_stat64_inc(zram, &zram->stats.invalid_io);
		goto error_unlock;
=======
	down_read(&zram->init_lock);
	if (unlikely(!zram->init_done))
		goto error;

	if (!valid_io_request(zram, bio)) {
		atomic64_inc(&zram->stats.invalid_io);
		goto error;
>>>>>>> refs/remotes/origin/master
=======
		atomic64_inc(&zram->stats.invalid_io);
		goto error;
>>>>>>> refs/remotes/origin/cm-11.0
	}

	__zram_make_request(zram, bio, bio_data_dir(bio));
	up_read(&zram->init_lock);

	return;

<<<<<<< HEAD
<<<<<<< HEAD
error_unlock:
	up_read(&zram->init_lock);
=======
>>>>>>> refs/remotes/origin/cm-11.0
error:
	up_read(&zram->init_lock);
	bio_io_error(bio);
}

static void zram_slot_free(struct work_struct *work)
{
<<<<<<< HEAD
	size_t index;

>>>>>>> refs/remotes/origin/cm-10.0
	zram->init_done = 0;

	/* Free various per-device buffers */
	kfree(zram->compress_workmem);
	free_pages((unsigned long)zram->compress_buffer, 1);

	zram->compress_workmem = NULL;
	zram->compress_buffer = NULL;

	/* Free all pages that are still in this zram device */
	for (index = 0; index < zram->disksize >> PAGE_SHIFT; index++) {
<<<<<<< HEAD
		struct page *page;
		u16 offset;

		page = zram->table[index].page;
		offset = zram->table[index].offset;

		if (!page)
			continue;

		if (unlikely(zram_test_flag(zram, index, ZRAM_UNCOMPRESSED)))
			__free_page(page);
		else
			xv_free(zram->mem_pool, page, offset);
=======
		void *handle = zram->table[index].handle;
		if (!handle)
			continue;

		if (unlikely(zram_test_flag(zram, index, ZRAM_UNCOMPRESSED)))
			__free_page(handle);
		else
			zs_free(zram->mem_pool, handle);
>>>>>>> refs/remotes/origin/cm-10.0
	}

	vfree(zram->table);
	zram->table = NULL;

<<<<<<< HEAD
	xv_destroy_pool(zram->mem_pool);
=======
	zs_destroy_pool(zram->mem_pool);
>>>>>>> refs/remotes/origin/cm-10.0
	zram->mem_pool = NULL;

	/* Reset stats */
	memset(&zram->stats, 0, sizeof(zram->stats));

	zram->disksize = 0;
<<<<<<< HEAD
	mutex_unlock(&zram->init_lock);
=======
=======
	struct zram *zram;

	zram = container_of(work, struct zram, free_work);
	down_write(&zram->lock);
	handle_pending_slot_free(zram);
	up_write(&zram->lock);
>>>>>>> refs/remotes/origin/cm-11.0
}

static void add_slot_free(struct zram *zram, struct zram_slot_free *free_rq)
{
<<<<<<< HEAD
	down_write(&zram->init_lock);
	__zram_reset_device(zram);
	up_write(&zram->init_lock);
>>>>>>> refs/remotes/origin/cm-10.0
}

int zram_init_device(struct zram *zram)
{
	int ret;
	size_t num_pages;

<<<<<<< HEAD
	mutex_lock(&zram->init_lock);

	if (zram->init_done) {
		mutex_unlock(&zram->init_lock);
=======
	down_write(&zram->init_lock);

	if (zram->init_done) {
		up_write(&zram->init_lock);
>>>>>>> refs/remotes/origin/cm-10.0
		return 0;
	}

	zram_set_disksize(zram, totalram_pages << PAGE_SHIFT);

	zram->compress_workmem = kzalloc(LZO1X_MEM_COMPRESS, GFP_KERNEL);
	if (!zram->compress_workmem) {
		pr_err("Error allocating compressor working memory!\n");
		ret = -ENOMEM;
<<<<<<< HEAD
		goto fail;
	}

	zram->compress_buffer = (void *)__get_free_pages(__GFP_ZERO, 1);
	if (!zram->compress_buffer) {
		pr_err("Error allocating compressor buffer space\n");
		ret = -ENOMEM;
		goto fail;
=======
		goto fail_no_table;
	}

	zram->compress_buffer =
		(void *)__get_free_pages(GFP_KERNEL | __GFP_ZERO, 1);
	if (!zram->compress_buffer) {
		pr_err("Error allocating compressor buffer space\n");
		ret = -ENOMEM;
		goto fail_no_table;
>>>>>>> refs/remotes/origin/cm-10.0
	}

	num_pages = zram->disksize >> PAGE_SHIFT;
	zram->table = vzalloc(num_pages * sizeof(*zram->table));
	if (!zram->table) {
		pr_err("Error allocating zram address table\n");
<<<<<<< HEAD
		/* To prevent accessing table entries during cleanup */
		zram->disksize = 0;
		ret = -ENOMEM;
		goto fail;
=======
		ret = -ENOMEM;
		goto fail_no_table;
>>>>>>> refs/remotes/origin/cm-10.0
	}

	set_capacity(zram->disk, zram->disksize >> SECTOR_SHIFT);

	/* zram devices sort of resembles non-rotational disks */
	queue_flag_set_unlocked(QUEUE_FLAG_NONROT, zram->disk->queue);

<<<<<<< HEAD
	zram->mem_pool = xv_create_pool();
=======
	zram->mem_pool = zs_create_pool("zram", GFP_NOIO | __GFP_HIGHMEM);
>>>>>>> refs/remotes/origin/cm-10.0
	if (!zram->mem_pool) {
		pr_err("Error creating memory pool\n");
		ret = -ENOMEM;
		goto fail;
	}

	zram->init_done = 1;
<<<<<<< HEAD
	mutex_unlock(&zram->init_lock);
=======
	up_write(&zram->init_lock);
>>>>>>> refs/remotes/origin/cm-10.0

	pr_debug("Initialization done!\n");
	return 0;

<<<<<<< HEAD
fail:
	mutex_unlock(&zram->init_lock);
	zram_reset_device(zram);

=======
fail_no_table:
	/* To prevent accessing table entries during cleanup */
	zram->disksize = 0;
fail:
	__zram_reset_device(zram);
	up_write(&zram->init_lock);
>>>>>>> refs/remotes/origin/cm-10.0
	pr_err("Initialization failed: err=%d\n", ret);
	return ret;
=======
	spin_lock(&zram->slot_free_lock);
	free_rq->next = zram->slot_free_rq;
	zram->slot_free_rq = free_rq;
	spin_unlock(&zram->slot_free_lock);
>>>>>>> refs/remotes/origin/cm-11.0
}

<<<<<<< HEAD
void zram_slot_free_notify(struct block_device *bdev, unsigned long index)
=======
static void zram_slot_free_notify(struct block_device *bdev,
				unsigned long index)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct zram *zram;
	struct zram_slot_free *free_rq;

	zram = bdev->bd_disk->private_data;
<<<<<<< HEAD
	zram_free_page(zram, index);
	zram_stat64_inc(zram, &zram->stats.notify_free);
=======
error:
	up_read(&zram->init_lock);
	bio_io_error(bio);
}

static void zram_slot_free(struct work_struct *work)
{
	struct zram *zram;

	zram = container_of(work, struct zram, free_work);
	down_write(&zram->lock);
	handle_pending_slot_free(zram);
	up_write(&zram->lock);
}

static void add_slot_free(struct zram *zram, struct zram_slot_free *free_rq)
{
	spin_lock(&zram->slot_free_lock);
	free_rq->next = zram->slot_free_rq;
	zram->slot_free_rq = free_rq;
	spin_unlock(&zram->slot_free_lock);
}

static void zram_slot_free_notify(struct block_device *bdev,
				unsigned long index)
{
	struct zram *zram;
	struct zram_slot_free *free_rq;

	zram = bdev->bd_disk->private_data;
=======
>>>>>>> refs/remotes/origin/cm-11.0
	atomic64_inc(&zram->stats.notify_free);

	free_rq = kmalloc(sizeof(struct zram_slot_free), GFP_ATOMIC);
	if (!free_rq)
		return;

	free_rq->index = index;
	add_slot_free(zram, free_rq);
	schedule_work(&zram->free_work);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
}

static const struct block_device_operations zram_devops = {
	.swap_slot_free_notify = zram_slot_free_notify,
	.owner = THIS_MODULE
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
static DEVICE_ATTR(disksize, S_IRUGO | S_IWUSR,
		disksize_show, disksize_store);
static DEVICE_ATTR(initstate, S_IRUGO, initstate_show, NULL);
static DEVICE_ATTR(reset, S_IWUSR, NULL, reset_store);
static DEVICE_ATTR(num_reads, S_IRUGO, num_reads_show, NULL);
static DEVICE_ATTR(num_writes, S_IRUGO, num_writes_show, NULL);
static DEVICE_ATTR(invalid_io, S_IRUGO, invalid_io_show, NULL);
static DEVICE_ATTR(notify_free, S_IRUGO, notify_free_show, NULL);
static DEVICE_ATTR(zero_pages, S_IRUGO, zero_pages_show, NULL);
static DEVICE_ATTR(orig_data_size, S_IRUGO, orig_data_size_show, NULL);
static DEVICE_ATTR(compr_data_size, S_IRUGO, compr_data_size_show, NULL);
static DEVICE_ATTR(mem_used_total, S_IRUGO, mem_used_total_show, NULL);

static struct attribute *zram_disk_attrs[] = {
	&dev_attr_disksize.attr,
	&dev_attr_initstate.attr,
	&dev_attr_reset.attr,
	&dev_attr_num_reads.attr,
	&dev_attr_num_writes.attr,
	&dev_attr_invalid_io.attr,
	&dev_attr_notify_free.attr,
	&dev_attr_zero_pages.attr,
	&dev_attr_orig_data_size.attr,
	&dev_attr_compr_data_size.attr,
	&dev_attr_mem_used_total.attr,
	NULL,
};

static struct attribute_group zram_disk_attr_group = {
	.attrs = zram_disk_attrs,
};

>>>>>>> refs/remotes/origin/cm-11.0
static int create_device(struct zram *zram, int device_id)
{
	int ret = -ENOMEM;

<<<<<<< HEAD
	mutex_init(&zram->lock);
	mutex_init(&zram->init_lock);
=======
	init_rwsem(&zram->lock);
	init_rwsem(&zram->init_lock);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	spin_lock_init(&zram->stat64_lock);
=======
static DEVICE_ATTR(disksize, S_IRUGO | S_IWUSR,
		disksize_show, disksize_store);
static DEVICE_ATTR(initstate, S_IRUGO, initstate_show, NULL);
static DEVICE_ATTR(reset, S_IWUSR, NULL, reset_store);
static DEVICE_ATTR(num_reads, S_IRUGO, num_reads_show, NULL);
static DEVICE_ATTR(num_writes, S_IRUGO, num_writes_show, NULL);
static DEVICE_ATTR(invalid_io, S_IRUGO, invalid_io_show, NULL);
static DEVICE_ATTR(notify_free, S_IRUGO, notify_free_show, NULL);
static DEVICE_ATTR(zero_pages, S_IRUGO, zero_pages_show, NULL);
static DEVICE_ATTR(orig_data_size, S_IRUGO, orig_data_size_show, NULL);
static DEVICE_ATTR(compr_data_size, S_IRUGO, compr_data_size_show, NULL);
static DEVICE_ATTR(mem_used_total, S_IRUGO, mem_used_total_show, NULL);

static struct attribute *zram_disk_attrs[] = {
	&dev_attr_disksize.attr,
	&dev_attr_initstate.attr,
	&dev_attr_reset.attr,
	&dev_attr_num_reads.attr,
	&dev_attr_num_writes.attr,
	&dev_attr_invalid_io.attr,
	&dev_attr_notify_free.attr,
	&dev_attr_zero_pages.attr,
	&dev_attr_orig_data_size.attr,
	&dev_attr_compr_data_size.attr,
	&dev_attr_mem_used_total.attr,
	NULL,
};

static struct attribute_group zram_disk_attr_group = {
	.attrs = zram_disk_attrs,
};

static int create_device(struct zram *zram, int device_id)
{
	int ret = -ENOMEM;

	init_rwsem(&zram->lock);
	init_rwsem(&zram->init_lock);
=======
>>>>>>> refs/remotes/origin/cm-11.0

	INIT_WORK(&zram->free_work, zram_slot_free);
	spin_lock_init(&zram->slot_free_lock);
	zram->slot_free_rq = NULL;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

	zram->queue = blk_alloc_queue(GFP_KERNEL);
	if (!zram->queue) {
		pr_err("Error allocating disk queue for device %d\n",
			device_id);
<<<<<<< HEAD
<<<<<<< HEAD
		ret = -ENOMEM;
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
		goto out;
	}

	blk_queue_make_request(zram->queue, zram_make_request);
	zram->queue->queuedata = zram;

	 /* gendisk structure */
	zram->disk = alloc_disk(1);
	if (!zram->disk) {
<<<<<<< HEAD
<<<<<<< HEAD
		blk_cleanup_queue(zram->queue);
		pr_warning("Error allocating disk structure for device %d\n",
			device_id);
		ret = -ENOMEM;
		goto out;
=======
		pr_warn("Error allocating disk structure for device %d\n",
			device_id);
		goto out_free_queue;
>>>>>>> refs/remotes/origin/master
=======
		pr_warn("Error allocating disk structure for device %d\n",
			device_id);
		goto out_free_queue;
>>>>>>> refs/remotes/origin/cm-11.0
	}

	zram->disk->major = zram_major;
	zram->disk->first_minor = device_id;
	zram->disk->fops = &zram_devops;
	zram->disk->queue = zram->queue;
	zram->disk->private_data = zram;
	snprintf(zram->disk->disk_name, 16, "zram%d", device_id);

	/* Actual capacity set using syfs (/sys/block/zram<id>/disksize */
	set_capacity(zram->disk, 0);

	/*
	 * To ensure that we always get PAGE_SIZE aligned
	 * and n*PAGE_SIZED sized I/O requests.
	 */
	blk_queue_physical_block_size(zram->disk->queue, PAGE_SIZE);
	blk_queue_logical_block_size(zram->disk->queue,
					ZRAM_LOGICAL_BLOCK_SIZE);
	blk_queue_io_min(zram->disk->queue, PAGE_SIZE);
	blk_queue_io_opt(zram->disk->queue, PAGE_SIZE);

	add_disk(zram->disk);

	ret = sysfs_create_group(&disk_to_dev(zram->disk)->kobj,
				&zram_disk_attr_group);
	if (ret < 0) {
<<<<<<< HEAD
<<<<<<< HEAD
		pr_warning("Error creating sysfs group");
		goto out;
=======
		pr_warn("Error creating sysfs group");
		goto out_free_disk;
>>>>>>> refs/remotes/origin/cm-11.0
	}

	zram->init_done = 0;
	return 0;

<<<<<<< HEAD
=======
		pr_warn("Error creating sysfs group");
		goto out_free_disk;
	}

	zram->init_done = 0;
	return 0;

=======
>>>>>>> refs/remotes/origin/cm-11.0
out_free_disk:
	del_gendisk(zram->disk);
	put_disk(zram->disk);
out_free_queue:
	blk_cleanup_queue(zram->queue);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
out:
	return ret;
}

static void destroy_device(struct zram *zram)
{
	sysfs_remove_group(&disk_to_dev(zram->disk)->kobj,
			&zram_disk_attr_group);

<<<<<<< HEAD
	if (zram->disk) {
		del_gendisk(zram->disk);
		put_disk(zram->disk);
	}

	if (zram->queue)
		blk_cleanup_queue(zram->queue);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
unsigned int zram_get_num_devices(void)
{
	return num_devices;
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
	del_gendisk(zram->disk);
	put_disk(zram->disk);

	blk_cleanup_queue(zram->queue);
}

>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
static int __init zram_init(void)
{
	int ret, dev_id;

	if (num_devices > max_num_devices) {
<<<<<<< HEAD
<<<<<<< HEAD
		pr_warning("Invalid value for num_devices: %u\n",
=======
		pr_warn("Invalid value for num_devices: %u\n",
>>>>>>> refs/remotes/origin/master
=======
		pr_warn("Invalid value for num_devices: %u\n",
>>>>>>> refs/remotes/origin/cm-11.0
				num_devices);
		ret = -EINVAL;
		goto out;
	}

	zram_major = register_blkdev(0, "zram");
	if (zram_major <= 0) {
<<<<<<< HEAD
<<<<<<< HEAD
		pr_warning("Unable to get major number\n");
=======
		pr_warn("Unable to get major number\n");
>>>>>>> refs/remotes/origin/master
=======
		pr_warn("Unable to get major number\n");
>>>>>>> refs/remotes/origin/cm-11.0
		ret = -EBUSY;
		goto out;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	if (!num_devices) {
		pr_info("num_devices not specified. Using default: 1\n");
		num_devices = 1;
	}

	/* Allocate the device array and initialize each one */
	pr_info("Creating %u devices ...\n", num_devices);
<<<<<<< HEAD
	devices = kzalloc(num_devices * sizeof(struct zram), GFP_KERNEL);
	if (!devices) {
=======
	zram_devices = kzalloc(num_devices * sizeof(struct zram), GFP_KERNEL);
	if (!zram_devices) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* Allocate the device array and initialize each one */
=======
	/* Allocate the device array and initialize each one */
>>>>>>> refs/remotes/origin/cm-11.0
	zram_devices = kzalloc(num_devices * sizeof(struct zram), GFP_KERNEL);
	if (!zram_devices) {
>>>>>>> refs/remotes/origin/master
		ret = -ENOMEM;
		goto unregister;
	}

	for (dev_id = 0; dev_id < num_devices; dev_id++) {
<<<<<<< HEAD
<<<<<<< HEAD
		ret = create_device(&devices[dev_id], dev_id);
=======
		ret = create_device(&zram_devices[dev_id], dev_id);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		ret = create_device(&zram_devices[dev_id], dev_id);
>>>>>>> refs/remotes/origin/master
		if (ret)
			goto free_devices;
	}

<<<<<<< HEAD
<<<<<<< HEAD
=======
	pr_info("Created %u device(s) ...\n", num_devices);

>>>>>>> refs/remotes/origin/master
=======
	pr_info("Created %u device(s) ...\n", num_devices);

>>>>>>> refs/remotes/origin/cm-11.0
	return 0;

free_devices:
	while (dev_id)
<<<<<<< HEAD
<<<<<<< HEAD
		destroy_device(&devices[--dev_id]);
	kfree(devices);
=======
		destroy_device(&zram_devices[--dev_id]);
	kfree(zram_devices);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		destroy_device(&zram_devices[--dev_id]);
	kfree(zram_devices);
>>>>>>> refs/remotes/origin/master
unregister:
	unregister_blkdev(zram_major, "zram");
out:
	return ret;
}

static void __exit zram_exit(void)
{
	int i;
	struct zram *zram;

	for (i = 0; i < num_devices; i++) {
<<<<<<< HEAD
<<<<<<< HEAD
		zram = &devices[i];
=======
		zram = &zram_devices[i];
>>>>>>> refs/remotes/origin/cm-10.0

		destroy_device(zram);
<<<<<<< HEAD
		if (zram->init_done)
			zram_reset_device(zram);
=======
		zram = &zram_devices[i];

		destroy_device(zram);
=======
>>>>>>> refs/remotes/origin/cm-11.0
		/*
		 * Shouldn't access zram->disk after destroy_device
		 * because destroy_device already released zram->disk.
		 */
		zram_reset_device(zram, false);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	}

	unregister_blkdev(zram_major, "zram");

<<<<<<< HEAD
<<<<<<< HEAD
	kfree(devices);
=======
	kfree(zram_devices);
>>>>>>> refs/remotes/origin/cm-10.0
	pr_debug("Cleanup done!\n");
}

module_init(zram_init);
module_exit(zram_exit);

<<<<<<< HEAD
=======
	kfree(zram_devices);
	pr_debug("Cleanup done!\n");
}

module_init(zram_init);
module_exit(zram_exit);

module_param(num_devices, uint, 0);
MODULE_PARM_DESC(num_devices, "Number of zram devices");

>>>>>>> refs/remotes/origin/master
=======
module_param(num_devices, uint, 0);
MODULE_PARM_DESC(num_devices, "Number of zram devices");

>>>>>>> refs/remotes/origin/cm-11.0
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Nitin Gupta <ngupta@vflare.org>");
MODULE_DESCRIPTION("Compressed RAM Block Device");
MODULE_ALIAS("devname:zram");
