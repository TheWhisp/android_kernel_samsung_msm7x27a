#ifndef __LINUX_PAGEISOLATION_H
#define __LINUX_PAGEISOLATION_H

/*
 * Changes migrate type in [start_pfn, end_pfn) to be MIGRATE_ISOLATE.
<<<<<<< HEAD
 * If specified range includes migrate types other than MOVABLE,
=======
 * If specified range includes migrate types other than MOVABLE or CMA,
>>>>>>> refs/remotes/origin/cm-10.0
 * this will fail with -EBUSY.
 *
 * For isolating all pages in the range finally, the caller have to
 * free all pages in the range. test_page_isolated() can be used for
 * test it.
 */
extern int
<<<<<<< HEAD
start_isolate_page_range(unsigned long start_pfn, unsigned long end_pfn);
=======
start_isolate_page_range(unsigned long start_pfn, unsigned long end_pfn,
			 unsigned migratetype);
>>>>>>> refs/remotes/origin/cm-10.0

/*
 * Changes MIGRATE_ISOLATE to MIGRATE_MOVABLE.
 * target range is [start_pfn, end_pfn)
 */
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
undo_isolate_page_range(unsigned long start_pfn, unsigned long end_pfn,
			unsigned migratetype);

/*
 * Test all pages in [start_pfn, end_pfn) are isolated or not.
 */
int test_pages_isolated(unsigned long start_pfn, unsigned long end_pfn);

/*
 * Internal functions. Changes pageblock's migrate type.
 */
extern int set_migratetype_isolate(struct page *page);
extern void unset_migratetype_isolate(struct page *page, unsigned migratetype);
>>>>>>> refs/remotes/origin/cm-10.0


#endif
