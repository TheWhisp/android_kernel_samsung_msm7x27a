/* bounce buffer handling for block devices
 *
 * - Split from highmem.c
 */

#include <linux/mm.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/module.h>
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master
#include <linux/swap.h>
#include <linux/gfp.h>
#include <linux/bio.h>
#include <linux/pagemap.h>
#include <linux/mempool.h>
#include <linux/blkdev.h>
#include <linux/init.h>
#include <linux/hash.h>
#include <linux/highmem.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/bootmem.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/bootmem.h>
>>>>>>> refs/remotes/origin/master
#include <asm/tlbflush.h>

#include <trace/events/block.h>

#define POOL_SIZE	64
#define ISA_POOL_SIZE	16

static mempool_t *page_pool, *isa_page_pool;

<<<<<<< HEAD
#ifdef CONFIG_HIGHMEM
static __init int init_emergency_pool(void)
{
<<<<<<< HEAD
	struct sysinfo i;
	si_meminfo(&i);
	si_swapinfo(&i);

	if (!i.totalhigh)
		return 0;
=======
#ifndef CONFIG_MEMORY_HOTPLUG
	if (max_pfn <= max_low_pfn)
		return 0;
#endif
>>>>>>> refs/remotes/origin/cm-10.0

	page_pool = mempool_create_page_pool(POOL_SIZE, 0);
	BUG_ON(!page_pool);
	printk("highmem bounce pool size: %d pages\n", POOL_SIZE);
=======
#if defined(CONFIG_HIGHMEM) || defined(CONFIG_NEED_BOUNCE_POOL)
static __init int init_emergency_pool(void)
{
#if defined(CONFIG_HIGHMEM) && !defined(CONFIG_MEMORY_HOTPLUG)
	if (max_pfn <= max_low_pfn)
		return 0;
#endif

	page_pool = mempool_create_page_pool(POOL_SIZE, 0);
	BUG_ON(!page_pool);
	printk("bounce pool size: %d pages\n", POOL_SIZE);
>>>>>>> refs/remotes/origin/master

	return 0;
}

__initcall(init_emergency_pool);
<<<<<<< HEAD

=======
#endif

#ifdef CONFIG_HIGHMEM
>>>>>>> refs/remotes/origin/master
/*
 * highmem version, map in to vec
 */
static void bounce_copy_vec(struct bio_vec *to, unsigned char *vfrom)
{
	unsigned long flags;
	unsigned char *vto;

	local_irq_save(flags);
<<<<<<< HEAD
<<<<<<< HEAD
	vto = kmap_atomic(to->bv_page, KM_BOUNCE_READ);
	memcpy(vto + to->bv_offset, vfrom, to->bv_len);
	kunmap_atomic(vto, KM_BOUNCE_READ);
=======
	vto = kmap_atomic(to->bv_page);
	memcpy(vto + to->bv_offset, vfrom, to->bv_len);
	kunmap_atomic(vto);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	vto = kmap_atomic(to->bv_page);
	memcpy(vto + to->bv_offset, vfrom, to->bv_len);
	kunmap_atomic(vto);
>>>>>>> refs/remotes/origin/master
	local_irq_restore(flags);
}

#else /* CONFIG_HIGHMEM */

#define bounce_copy_vec(to, vfrom)	\
	memcpy(page_address((to)->bv_page) + (to)->bv_offset, vfrom, (to)->bv_len)

#endif /* CONFIG_HIGHMEM */

/*
 * allocate pages in the DMA region for the ISA pool
 */
static void *mempool_alloc_pages_isa(gfp_t gfp_mask, void *data)
{
	return mempool_alloc_pages(gfp_mask | GFP_DMA, data);
}

/*
 * gets called "every" time someone init's a queue with BLK_BOUNCE_ISA
 * as the max address, so check if the pool has already been created.
 */
int init_emergency_isa_pool(void)
{
	if (isa_page_pool)
		return 0;

	isa_page_pool = mempool_create(ISA_POOL_SIZE, mempool_alloc_pages_isa,
				       mempool_free_pages, (void *) 0);
	BUG_ON(!isa_page_pool);

	printk("isa bounce pool size: %d pages\n", ISA_POOL_SIZE);
	return 0;
}

/*
 * Simple bounce buffer support for highmem pages. Depending on the
 * queue gfp mask set, *to may or may not be a highmem page. kmap it
 * always, it will do the Right Thing
 */
static void copy_to_high_bio_irq(struct bio *to, struct bio *from)
{
	unsigned char *vfrom;
	struct bio_vec *tovec, *fromvec;
	int i;

<<<<<<< HEAD
	__bio_for_each_segment(tovec, to, i, 0) {
=======
	bio_for_each_segment(tovec, to, i) {
>>>>>>> refs/remotes/origin/master
		fromvec = from->bi_io_vec + i;

		/*
		 * not bounced
		 */
		if (tovec->bv_page == fromvec->bv_page)
			continue;

		/*
		 * fromvec->bv_offset and fromvec->bv_len might have been
		 * modified by the block layer, so use the original copy,
		 * bounce_copy_vec already uses tovec->bv_len
		 */
		vfrom = page_address(fromvec->bv_page) + tovec->bv_offset;

		bounce_copy_vec(tovec, vfrom);
		flush_dcache_page(tovec->bv_page);
	}
}

static void bounce_end_io(struct bio *bio, mempool_t *pool, int err)
{
	struct bio *bio_orig = bio->bi_private;
	struct bio_vec *bvec, *org_vec;
	int i;

	if (test_bit(BIO_EOPNOTSUPP, &bio->bi_flags))
		set_bit(BIO_EOPNOTSUPP, &bio_orig->bi_flags);

	/*
	 * free up bounce indirect pages used
	 */
<<<<<<< HEAD
	__bio_for_each_segment(bvec, bio, i, 0) {
=======
	bio_for_each_segment_all(bvec, bio, i) {
>>>>>>> refs/remotes/origin/master
		org_vec = bio_orig->bi_io_vec + i;
		if (bvec->bv_page == org_vec->bv_page)
			continue;

		dec_zone_page_state(bvec->bv_page, NR_BOUNCE);
		mempool_free(bvec->bv_page, pool);
	}

	bio_endio(bio_orig, err);
	bio_put(bio);
}

static void bounce_end_io_write(struct bio *bio, int err)
{
	bounce_end_io(bio, page_pool, err);
}

static void bounce_end_io_write_isa(struct bio *bio, int err)
{

	bounce_end_io(bio, isa_page_pool, err);
}

static void __bounce_end_io_read(struct bio *bio, mempool_t *pool, int err)
{
	struct bio *bio_orig = bio->bi_private;

	if (test_bit(BIO_UPTODATE, &bio->bi_flags))
		copy_to_high_bio_irq(bio_orig, bio);

	bounce_end_io(bio, pool, err);
}

static void bounce_end_io_read(struct bio *bio, int err)
{
	__bounce_end_io_read(bio, page_pool, err);
}

static void bounce_end_io_read_isa(struct bio *bio, int err)
{
	__bounce_end_io_read(bio, isa_page_pool, err);
}

<<<<<<< HEAD
static void __blk_queue_bounce(struct request_queue *q, struct bio **bio_orig,
			       mempool_t *pool)
{
	struct page *page;
	struct bio *bio = NULL;
	int i, rw = bio_data_dir(*bio_orig);
	struct bio_vec *to, *from;

	bio_for_each_segment(from, *bio_orig, i) {
		page = from->bv_page;

		/*
		 * is destination page below bounce pfn?
		 */
		if (page_to_pfn(page) <= queue_bounce_pfn(q))
			continue;

		/*
		 * irk, bounce it
		 */
		if (!bio) {
			unsigned int cnt = (*bio_orig)->bi_vcnt;

			bio = bio_alloc(GFP_NOIO, cnt);
			memset(bio->bi_io_vec, 0, cnt * sizeof(struct bio_vec));
		}
			

		to = bio->bi_io_vec + i;

		to->bv_page = mempool_alloc(pool, q->bounce_gfp);
		to->bv_len = from->bv_len;
		to->bv_offset = from->bv_offset;
		inc_zone_page_state(to->bv_page, NR_BOUNCE);
=======
#ifdef CONFIG_NEED_BOUNCE_POOL
static int must_snapshot_stable_pages(struct request_queue *q, struct bio *bio)
{
	if (bio_data_dir(bio) != WRITE)
		return 0;

	if (!bdi_cap_stable_pages_required(&q->backing_dev_info))
		return 0;

	return test_bit(BIO_SNAP_STABLE, &bio->bi_flags);
}
#else
static int must_snapshot_stable_pages(struct request_queue *q, struct bio *bio)
{
	return 0;
}
#endif /* CONFIG_NEED_BOUNCE_POOL */

static void __blk_queue_bounce(struct request_queue *q, struct bio **bio_orig,
			       mempool_t *pool, int force)
{
	struct bio *bio;
	int rw = bio_data_dir(*bio_orig);
	struct bio_vec *to, *from;
	unsigned i;

	if (force)
		goto bounce;
	bio_for_each_segment(from, *bio_orig, i)
		if (page_to_pfn(from->bv_page) > queue_bounce_pfn(q))
			goto bounce;

	return;
bounce:
	bio = bio_clone_bioset(*bio_orig, GFP_NOIO, fs_bio_set);

	bio_for_each_segment_all(to, bio, i) {
		struct page *page = to->bv_page;

		if (page_to_pfn(page) <= queue_bounce_pfn(q) && !force)
			continue;

		inc_zone_page_state(to->bv_page, NR_BOUNCE);
		to->bv_page = mempool_alloc(pool, q->bounce_gfp);
>>>>>>> refs/remotes/origin/master

		if (rw == WRITE) {
			char *vto, *vfrom;

<<<<<<< HEAD
			flush_dcache_page(from->bv_page);
			vto = page_address(to->bv_page) + to->bv_offset;
			vfrom = kmap(from->bv_page) + from->bv_offset;
			memcpy(vto, vfrom, to->bv_len);
			kunmap(from->bv_page);
		}
	}

	/*
	 * no pages bounced
	 */
	if (!bio)
		return;

	trace_block_bio_bounce(q, *bio_orig);

	/*
	 * at least one page was bounced, fill in possible non-highmem
	 * pages
	 */
	__bio_for_each_segment(from, *bio_orig, i, 0) {
		to = bio_iovec_idx(bio, i);
		if (!to->bv_page) {
			to->bv_page = from->bv_page;
			to->bv_len = from->bv_len;
			to->bv_offset = from->bv_offset;
		}
	}

	bio->bi_bdev = (*bio_orig)->bi_bdev;
	bio->bi_flags |= (1 << BIO_BOUNCED);
	bio->bi_sector = (*bio_orig)->bi_sector;
	bio->bi_rw = (*bio_orig)->bi_rw;

	bio->bi_vcnt = (*bio_orig)->bi_vcnt;
	bio->bi_idx = (*bio_orig)->bi_idx;
	bio->bi_size = (*bio_orig)->bi_size;
=======
			flush_dcache_page(page);

			vto = page_address(to->bv_page) + to->bv_offset;
			vfrom = kmap_atomic(page) + to->bv_offset;
			memcpy(vto, vfrom, to->bv_len);
			kunmap_atomic(vfrom);
		}
	}

	trace_block_bio_bounce(q, *bio_orig);

	bio->bi_flags |= (1 << BIO_BOUNCED);
>>>>>>> refs/remotes/origin/master

	if (pool == page_pool) {
		bio->bi_end_io = bounce_end_io_write;
		if (rw == READ)
			bio->bi_end_io = bounce_end_io_read;
	} else {
		bio->bi_end_io = bounce_end_io_write_isa;
		if (rw == READ)
			bio->bi_end_io = bounce_end_io_read_isa;
	}

	bio->bi_private = *bio_orig;
	*bio_orig = bio;
}

void blk_queue_bounce(struct request_queue *q, struct bio **bio_orig)
{
<<<<<<< HEAD
=======
	int must_bounce;
>>>>>>> refs/remotes/origin/master
	mempool_t *pool;

	/*
	 * Data-less bio, nothing to bounce
	 */
	if (!bio_has_data(*bio_orig))
		return;

<<<<<<< HEAD
=======
	must_bounce = must_snapshot_stable_pages(q, *bio_orig);

>>>>>>> refs/remotes/origin/master
	/*
	 * for non-isa bounce case, just check if the bounce pfn is equal
	 * to or bigger than the highest pfn in the system -- in that case,
	 * don't waste time iterating over bio segments
	 */
	if (!(q->bounce_gfp & GFP_DMA)) {
<<<<<<< HEAD
		if (queue_bounce_pfn(q) >= blk_max_pfn)
=======
		if (queue_bounce_pfn(q) >= blk_max_pfn && !must_bounce)
>>>>>>> refs/remotes/origin/master
			return;
		pool = page_pool;
	} else {
		BUG_ON(!isa_page_pool);
		pool = isa_page_pool;
	}

	/*
	 * slow path
	 */
<<<<<<< HEAD
	__blk_queue_bounce(q, bio_orig, pool);
=======
	__blk_queue_bounce(q, bio_orig, pool, must_bounce);
>>>>>>> refs/remotes/origin/master
}

EXPORT_SYMBOL(blk_queue_bounce);
