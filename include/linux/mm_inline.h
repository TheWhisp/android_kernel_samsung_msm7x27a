#ifndef LINUX_MM_INLINE_H
#define LINUX_MM_INLINE_H

#include <linux/huge_mm.h>
<<<<<<< HEAD
=======
#include <linux/swap.h>
>>>>>>> refs/remotes/origin/master

/**
 * page_is_file_cache - should the page be on a file LRU or anon LRU?
 * @page: the page to test
 *
 * Returns 1 if @page is page cache page backed by a regular filesystem,
 * or 0 if @page is anonymous, tmpfs or otherwise ram or swap backed.
 * Used by functions that manipulate the LRU lists, to sort a page
 * onto the right LRU list.
 *
 * We would like to get this info without a page flag, but the state
 * needs to survive until the page is last deleted from the LRU, which
 * could be as far down as __page_cache_release.
 */
static inline int page_is_file_cache(struct page *page)
{
	return !PageSwapBacked(page);
}

<<<<<<< HEAD
static inline void
<<<<<<< HEAD
__add_page_to_lru_list(struct zone *zone, struct page *page, enum lru_list l,
		       struct list_head *head)
{
	list_add(&page->lru, head);
	__mod_zone_page_state(zone, NR_LRU_BASE + l, hpage_nr_pages(page));
	mem_cgroup_add_lru_list(page, l);
}

static inline void
add_page_to_lru_list(struct zone *zone, struct page *page, enum lru_list l)
{
	__add_page_to_lru_list(zone, page, l, &zone->lru[l].list);
}

static inline void
del_page_from_lru_list(struct zone *zone, struct page *page, enum lru_list l)
{
	list_del(&page->lru);
	__mod_zone_page_state(zone, NR_LRU_BASE + l, -hpage_nr_pages(page));
	mem_cgroup_del_lru_list(page, l);
=======
add_page_to_lru_list(struct zone *zone, struct page *page, enum lru_list lru)
{
	struct lruvec *lruvec;

	lruvec = mem_cgroup_lru_add_list(zone, page, lru);
	list_add(&page->lru, &lruvec->lists[lru]);
	__mod_zone_page_state(zone, NR_LRU_BASE + lru, hpage_nr_pages(page));
}

static inline void
del_page_from_lru_list(struct zone *zone, struct page *page, enum lru_list lru)
{
	mem_cgroup_lru_del_list(page, lru);
	list_del(&page->lru);
	__mod_zone_page_state(zone, NR_LRU_BASE + lru, -hpage_nr_pages(page));
>>>>>>> refs/remotes/origin/cm-10.0
=======
static __always_inline void add_page_to_lru_list(struct page *page,
				struct lruvec *lruvec, enum lru_list lru)
{
	int nr_pages = hpage_nr_pages(page);
	mem_cgroup_update_lru_size(lruvec, lru, nr_pages);
	list_add(&page->lru, &lruvec->lists[lru]);
	__mod_zone_page_state(lruvec_zone(lruvec), NR_LRU_BASE + lru, nr_pages);
}

static __always_inline void del_page_from_lru_list(struct page *page,
				struct lruvec *lruvec, enum lru_list lru)
{
	int nr_pages = hpage_nr_pages(page);
	mem_cgroup_update_lru_size(lruvec, lru, -nr_pages);
	list_del(&page->lru);
	__mod_zone_page_state(lruvec_zone(lruvec), NR_LRU_BASE + lru, -nr_pages);
>>>>>>> refs/remotes/origin/master
}

/**
 * page_lru_base_type - which LRU list type should a page be on?
 * @page: the page to test
 *
 * Used for LRU list index arithmetic.
 *
 * Returns the base LRU type - file or anon - @page should be on.
 */
static inline enum lru_list page_lru_base_type(struct page *page)
{
	if (page_is_file_cache(page))
		return LRU_INACTIVE_FILE;
	return LRU_INACTIVE_ANON;
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline void
del_page_from_lru(struct zone *zone, struct page *page)
{
	enum lru_list l;

	list_del(&page->lru);
	if (PageUnevictable(page)) {
		__ClearPageUnevictable(page);
		l = LRU_UNEVICTABLE;
	} else {
		l = page_lru_base_type(page);
		if (PageActive(page)) {
			__ClearPageActive(page);
			l += LRU_ACTIVE;
		}
	}
	__mod_zone_page_state(zone, NR_LRU_BASE + l, -hpage_nr_pages(page));
	mem_cgroup_del_lru_list(page, l);
=======
=======
>>>>>>> refs/remotes/origin/master
/**
 * page_off_lru - which LRU list was page on? clearing its lru flags.
 * @page: the page to test
 *
 * Returns the LRU list a page was on, as an index into the array of LRU
 * lists; and clears its Unevictable or Active flags, ready for freeing.
 */
<<<<<<< HEAD
static inline enum lru_list page_off_lru(struct page *page)
=======
static __always_inline enum lru_list page_off_lru(struct page *page)
>>>>>>> refs/remotes/origin/master
{
	enum lru_list lru;

	if (PageUnevictable(page)) {
		__ClearPageUnevictable(page);
		lru = LRU_UNEVICTABLE;
	} else {
		lru = page_lru_base_type(page);
		if (PageActive(page)) {
			__ClearPageActive(page);
			lru += LRU_ACTIVE;
		}
	}
	return lru;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

/**
 * page_lru - which LRU list should a page be on?
 * @page: the page to test
 *
 * Returns the LRU list a page should be on, as an index
 * into the array of LRU lists.
 */
<<<<<<< HEAD
static inline enum lru_list page_lru(struct page *page)
=======
static __always_inline enum lru_list page_lru(struct page *page)
>>>>>>> refs/remotes/origin/master
{
	enum lru_list lru;

	if (PageUnevictable(page))
		lru = LRU_UNEVICTABLE;
	else {
		lru = page_lru_base_type(page);
		if (PageActive(page))
			lru += LRU_ACTIVE;
	}
<<<<<<< HEAD
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return lru;
}

#endif
