/*
 * linux/mm/page_isolation.c
 */

#include <linux/mm.h>
#include <linux/page-isolation.h>
#include <linux/pageblock-flags.h>
<<<<<<< HEAD
#include "internal.h"

=======
#include <linux/memory.h>
#include <linux/hugetlb.h>
#include "internal.h"

int set_migratetype_isolate(struct page *page, bool skip_hwpoisoned_pages)
{
	struct zone *zone;
	unsigned long flags, pfn;
	struct memory_isolate_notify arg;
	int notifier_ret;
	int ret = -EBUSY;

	zone = page_zone(page);

	spin_lock_irqsave(&zone->lock, flags);

	pfn = page_to_pfn(page);
	arg.start_pfn = pfn;
	arg.nr_pages = pageblock_nr_pages;
	arg.pages_found = 0;

	/*
	 * It may be possible to isolate a pageblock even if the
	 * migratetype is not MIGRATE_MOVABLE. The memory isolation
	 * notifier chain is used by balloon drivers to return the
	 * number of pages in a range that are held by the balloon
	 * driver to shrink memory. If all the pages are accounted for
	 * by balloons, are free, or on the LRU, isolation can continue.
	 * Later, for example, when memory hotplug notifier runs, these
	 * pages reported as "can be isolated" should be isolated(freed)
	 * by the balloon driver through the memory notifier chain.
	 */
	notifier_ret = memory_isolate_notify(MEM_ISOLATE_COUNT, &arg);
	notifier_ret = notifier_to_errno(notifier_ret);
	if (notifier_ret)
		goto out;
	/*
	 * FIXME: Now, memory hotplug doesn't call shrink_slab() by itself.
	 * We just check MOVABLE pages.
	 */
	if (!has_unmovable_pages(zone, page, arg.pages_found,
				 skip_hwpoisoned_pages))
		ret = 0;

	/*
	 * immobile means "not-on-lru" paes. If immobile is larger than
	 * removable-by-driver pages reported by notifier, we'll fail.
	 */

out:
	if (!ret) {
		unsigned long nr_pages;
		int migratetype = get_pageblock_migratetype(page);

		set_pageblock_migratetype(page, MIGRATE_ISOLATE);
		nr_pages = move_freepages_block(zone, page, MIGRATE_ISOLATE);

		__mod_zone_freepage_state(zone, -nr_pages, migratetype);
	}

	spin_unlock_irqrestore(&zone->lock, flags);
	if (!ret)
		drain_all_pages();
	return ret;
}

void unset_migratetype_isolate(struct page *page, unsigned migratetype)
{
	struct zone *zone;
	unsigned long flags, nr_pages;

	zone = page_zone(page);
	spin_lock_irqsave(&zone->lock, flags);
	if (get_pageblock_migratetype(page) != MIGRATE_ISOLATE)
		goto out;
	nr_pages = move_freepages_block(zone, page, migratetype);
	__mod_zone_freepage_state(zone, nr_pages, migratetype);
	set_pageblock_migratetype(page, migratetype);
out:
	spin_unlock_irqrestore(&zone->lock, flags);
}

>>>>>>> refs/remotes/origin/master
static inline struct page *
__first_valid_page(unsigned long pfn, unsigned long nr_pages)
{
	int i;
	for (i = 0; i < nr_pages; i++)
		if (pfn_valid_within(pfn + i))
			break;
	if (unlikely(i == nr_pages))
		return NULL;
	return pfn_to_page(pfn + i);
}

/*
 * start_isolate_page_range() -- make page-allocation-type of range of pages
 * to be MIGRATE_ISOLATE.
 * @start_pfn: The lower PFN of the range to be isolated.
 * @end_pfn: The upper PFN of the range to be isolated.
<<<<<<< HEAD
<<<<<<< HEAD
=======
 * @migratetype: migrate type to set in error recovery.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * @migratetype: migrate type to set in error recovery.
>>>>>>> refs/remotes/origin/master
 *
 * Making page-allocation-type to be MIGRATE_ISOLATE means free pages in
 * the range will never be allocated. Any free pages and pages freed in the
 * future will not be allocated again.
 *
 * start_pfn/end_pfn must be aligned to pageblock_order.
 * Returns 0 on success and -EBUSY if any part of range cannot be isolated.
 */
<<<<<<< HEAD
<<<<<<< HEAD
int
start_isolate_page_range(unsigned long start_pfn, unsigned long end_pfn)
=======
int start_isolate_page_range(unsigned long start_pfn, unsigned long end_pfn,
			     unsigned migratetype)
>>>>>>> refs/remotes/origin/cm-10.0
=======
int start_isolate_page_range(unsigned long start_pfn, unsigned long end_pfn,
			     unsigned migratetype, bool skip_hwpoisoned_pages)
>>>>>>> refs/remotes/origin/master
{
	unsigned long pfn;
	unsigned long undo_pfn;
	struct page *page;

	BUG_ON((start_pfn) & (pageblock_nr_pages - 1));
	BUG_ON((end_pfn) & (pageblock_nr_pages - 1));

	for (pfn = start_pfn;
	     pfn < end_pfn;
	     pfn += pageblock_nr_pages) {
		page = __first_valid_page(pfn, pageblock_nr_pages);
<<<<<<< HEAD
		if (page && set_migratetype_isolate(page)) {
=======
		if (page &&
		    set_migratetype_isolate(page, skip_hwpoisoned_pages)) {
>>>>>>> refs/remotes/origin/master
			undo_pfn = pfn;
			goto undo;
		}
	}
	return 0;
undo:
	for (pfn = start_pfn;
	     pfn < undo_pfn;
	     pfn += pageblock_nr_pages)
<<<<<<< HEAD
<<<<<<< HEAD
		unset_migratetype_isolate(pfn_to_page(pfn));
=======
		unset_migratetype_isolate(pfn_to_page(pfn), migratetype);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		unset_migratetype_isolate(pfn_to_page(pfn), migratetype);
>>>>>>> refs/remotes/origin/master

	return -EBUSY;
}

/*
 * Make isolated pages available again.
 */
<<<<<<< HEAD
<<<<<<< HEAD
int
undo_isolate_page_range(unsigned long start_pfn, unsigned long end_pfn)
=======
int undo_isolate_page_range(unsigned long start_pfn, unsigned long end_pfn,
			    unsigned migratetype)
>>>>>>> refs/remotes/origin/cm-10.0
=======
int undo_isolate_page_range(unsigned long start_pfn, unsigned long end_pfn,
			    unsigned migratetype)
>>>>>>> refs/remotes/origin/master
{
	unsigned long pfn;
	struct page *page;
	BUG_ON((start_pfn) & (pageblock_nr_pages - 1));
	BUG_ON((end_pfn) & (pageblock_nr_pages - 1));
	for (pfn = start_pfn;
	     pfn < end_pfn;
	     pfn += pageblock_nr_pages) {
		page = __first_valid_page(pfn, pageblock_nr_pages);
		if (!page || get_pageblock_migratetype(page) != MIGRATE_ISOLATE)
			continue;
<<<<<<< HEAD
<<<<<<< HEAD
		unset_migratetype_isolate(page);
=======
		unset_migratetype_isolate(page, migratetype);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		unset_migratetype_isolate(page, migratetype);
>>>>>>> refs/remotes/origin/master
	}
	return 0;
}
/*
 * Test all pages in the range is free(means isolated) or not.
 * all pages in [start_pfn...end_pfn) must be in the same zone.
 * zone->lock must be held before call this.
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * Returns 1 if all pages in the range is isolated.
=======
 * Returns 1 if all pages in the range are isolated.
>>>>>>> refs/remotes/origin/cm-10.0
 */
static int
__test_page_isolated_in_pageblock(unsigned long pfn, unsigned long end_pfn)
=======
 * Returns 1 if all pages in the range are isolated.
 */
static int
__test_page_isolated_in_pageblock(unsigned long pfn, unsigned long end_pfn,
				  bool skip_hwpoisoned_pages)
>>>>>>> refs/remotes/origin/master
{
	struct page *page;

	while (pfn < end_pfn) {
		if (!pfn_valid_within(pfn)) {
			pfn++;
			continue;
		}
		page = pfn_to_page(pfn);
<<<<<<< HEAD
		if (PageBuddy(page))
			pfn += 1 << page_order(page);
		else if (page_count(page) == 0 &&
				page_private(page) == MIGRATE_ISOLATE)
			pfn += 1;
=======
		if (PageBuddy(page)) {
			/*
			 * If race between isolatation and allocation happens,
			 * some free pages could be in MIGRATE_MOVABLE list
			 * although pageblock's migratation type of the page
			 * is MIGRATE_ISOLATE. Catch it and move the page into
			 * MIGRATE_ISOLATE list.
			 */
			if (get_freepage_migratetype(page) != MIGRATE_ISOLATE) {
				struct page *end_page;

				end_page = page + (1 << page_order(page)) - 1;
				move_freepages(page_zone(page), page, end_page,
						MIGRATE_ISOLATE);
			}
			pfn += 1 << page_order(page);
		}
		else if (page_count(page) == 0 &&
			get_freepage_migratetype(page) == MIGRATE_ISOLATE)
			pfn += 1;
		else if (skip_hwpoisoned_pages && PageHWPoison(page)) {
			/*
			 * The HWPoisoned page may be not in buddy
			 * system, and page_count() is not 0.
			 */
			pfn++;
			continue;
		}
>>>>>>> refs/remotes/origin/master
		else
			break;
	}
	if (pfn < end_pfn)
		return 0;
	return 1;
}

<<<<<<< HEAD
int test_pages_isolated(unsigned long start_pfn, unsigned long end_pfn)
=======
int test_pages_isolated(unsigned long start_pfn, unsigned long end_pfn,
			bool skip_hwpoisoned_pages)
>>>>>>> refs/remotes/origin/master
{
	unsigned long pfn, flags;
	struct page *page;
	struct zone *zone;
	int ret;

	/*
<<<<<<< HEAD
	 * Note: pageblock_nr_page != MAX_ORDER. Then, chunks of free page
	 * is not aligned to pageblock_nr_pages.
	 * Then we just check pagetype fist.
=======
	 * Note: pageblock_nr_pages != MAX_ORDER. Then, chunks of free pages
	 * are not aligned to pageblock_nr_pages.
	 * Then we just check migratetype first.
>>>>>>> refs/remotes/origin/master
	 */
	for (pfn = start_pfn; pfn < end_pfn; pfn += pageblock_nr_pages) {
		page = __first_valid_page(pfn, pageblock_nr_pages);
		if (page && get_pageblock_migratetype(page) != MIGRATE_ISOLATE)
			break;
	}
	page = __first_valid_page(start_pfn, end_pfn - start_pfn);
	if ((pfn < end_pfn) || !page)
		return -EBUSY;
<<<<<<< HEAD
	/* Check all pages are free or Marked as ISOLATED */
	zone = page_zone(page);
	spin_lock_irqsave(&zone->lock, flags);
	ret = __test_page_isolated_in_pageblock(start_pfn, end_pfn);
	spin_unlock_irqrestore(&zone->lock, flags);
	return ret ? 0 : -EBUSY;
}
=======
	/* Check all pages are free or marked as ISOLATED */
	zone = page_zone(page);
	spin_lock_irqsave(&zone->lock, flags);
	ret = __test_page_isolated_in_pageblock(start_pfn, end_pfn,
						skip_hwpoisoned_pages);
	spin_unlock_irqrestore(&zone->lock, flags);
	return ret ? 0 : -EBUSY;
}

struct page *alloc_migrate_target(struct page *page, unsigned long private,
				  int **resultp)
{
	gfp_t gfp_mask = GFP_USER | __GFP_MOVABLE;

	/*
	 * TODO: allocate a destination hugepage from a nearest neighbor node,
	 * accordance with memory policy of the user process if possible. For
	 * now as a simple work-around, we use the next node for destination.
	 */
	if (PageHuge(page)) {
		nodemask_t src = nodemask_of_node(page_to_nid(page));
		nodemask_t dst;
		nodes_complement(dst, src);
		return alloc_huge_page_node(page_hstate(compound_head(page)),
					    next_node(page_to_nid(page), dst));
	}

	if (PageHighMem(page))
		gfp_mask |= __GFP_HIGHMEM;

	return alloc_page(gfp_mask);
}
>>>>>>> refs/remotes/origin/master
