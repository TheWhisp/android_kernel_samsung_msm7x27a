#ifndef __LINUX_PAGEISOLATION_H
#define __LINUX_PAGEISOLATION_H

<<<<<<< HEAD
/*
 * Changes migrate type in [start_pfn, end_pfn) to be MIGRATE_ISOLATE.
<<<<<<< HEAD
 * If specified range includes migrate types other than MOVABLE,
=======
 * If specified range includes migrate types other than MOVABLE or CMA,
>>>>>>> refs/remotes/origin/cm-10.0
=======
#ifdef CONFIG_MEMORY_ISOLATION
static inline bool is_migrate_isolate_page(struct page *page)
{
	return get_pageblock_migratetype(page) == MIGRATE_ISOLATE;
}
static inline bool is_migrate_isolate(int migratetype)
{
	return migratetype == MIGRATE_ISOLATE;
}
#else
static inline bool is_migrate_isolate_page(struct page *page)
{
	return false;
}
static inline bool is_migrate_isolate(int migratetype)
{
	return false;
}
#endif

bool has_unmovable_pages(struct zone *zone, struct page *page, int count,
			 bool skip_hwpoisoned_pages);
void set_pageblock_migratetype(struct page *page, int migratetype);
int move_freepages_block(struct zone *zone, struct page *page,
				int migratetype);
int move_freepages(struct zone *zone,
			  struct page *start_page, struct page *end_page,
			  int migratetype);

/*
 * Changes migrate type in [start_pfn, end_pfn) to be MIGRATE_ISOLATE.
 * If specified range includes migrate types other than MOVABLE or CMA,
>>>>>>> refs/remotes/origin/master
 * this will fail with -EBUSY.
 *
 * For isolating all pages in the range finally, the caller have to
 * free all pages in the range. test_page_isolated() can be used for
 * test it.
 */
<<<<<<< HEAD
extern int
<<<<<<< HEAD
start_isolate_page_range(unsigned long start_pfn, unsigned long end_pfn);
=======
start_isolate_page_range(unsigned long start_pfn, unsigned long end_pfn,
			 unsigned migratetype);
>>>>>>> refs/remotes/origin/cm-10.0
=======
int
start_isolate_page_range(unsigned long start_pfn, unsigned long end_pfn,
			 unsigned migratetype, bool skip_hwpoisoned_pages);
>>>>>>> refs/remotes/origin/master

/*
 * Changes MIGRATE_ISOLATE to MIGRATE_MOVABLE.
 * target range is [start_pfn, end_pfn)
 */
<<<<<<< HEAD
extern int
<<<<<<< HEAD
undo_isolate_page_range(unsigned long start_pfn, unsigned long end_pfn);

/*
 * test all pages in [start_pfn, end_pfn)are isolated or not.
 */
extern int
test_pages_isolated(unsigned long start_pfn, unsigned long end_pfn);

/*
 * Internal funcs.Changes pageblock's migrate type.
 * Please use make_pagetype_isolated()/make_pagetype_movable().
 */
extern int set_migratetype_isolate(struct page *page);
extern void unset_migratetype_isolate(struct page *page);
=======
=======
int
>>>>>>> refs/remotes/origin/master
undo_isolate_page_range(unsigned long start_pfn, unsigned long end_pfn,
			unsigned migratetype);

/*
 * Test all pages in [start_pfn, end_pfn) are isolated or not.
 */
<<<<<<< HEAD
int test_pages_isolated(unsigned long start_pfn, unsigned long end_pfn);
=======
int test_pages_isolated(unsigned long start_pfn, unsigned long end_pfn,
			bool skip_hwpoisoned_pages);
>>>>>>> refs/remotes/origin/master

/*
 * Internal functions. Changes pageblock's migrate type.
 */
<<<<<<< HEAD
extern int set_migratetype_isolate(struct page *page);
extern void unset_migratetype_isolate(struct page *page, unsigned migratetype);
>>>>>>> refs/remotes/origin/cm-10.0

=======
int set_migratetype_isolate(struct page *page, bool skip_hwpoisoned_pages);
void unset_migratetype_isolate(struct page *page, unsigned migratetype);
struct page *alloc_migrate_target(struct page *page, unsigned long private,
				int **resultp);
>>>>>>> refs/remotes/origin/master

#endif
