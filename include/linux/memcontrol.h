/* memcontrol.h - Memory Controller
 *
 * Copyright IBM Corporation, 2007
 * Author Balbir Singh <balbir@linux.vnet.ibm.com>
 *
 * Copyright 2007 OpenVZ SWsoft Inc
 * Author: Pavel Emelianov <xemul@openvz.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _LINUX_MEMCONTROL_H
#define _LINUX_MEMCONTROL_H
#include <linux/cgroup.h>
#include <linux/vm_event_item.h>
<<<<<<< HEAD
=======
#include <linux/hardirq.h>
#include <linux/jump_label.h>
>>>>>>> refs/remotes/origin/master

struct mem_cgroup;
struct page_cgroup;
struct page;
struct mm_struct;
<<<<<<< HEAD

/* Stats that can be updated by kernel. */
enum mem_cgroup_page_stat_item {
	MEMCG_NR_FILE_MAPPED, /* # of pages charged as file rss */
};

<<<<<<< HEAD
extern unsigned long mem_cgroup_isolate_pages(unsigned long nr_to_scan,
					struct list_head *dst,
					unsigned long *scanned, int order,
					isolate_mode_t mode,
					struct zone *z,
					struct mem_cgroup *mem_cont,
					int active, int file);
=======
=======
struct kmem_cache;

/*
 * The corresponding mem_cgroup_stat_names is defined in mm/memcontrol.c,
 * These two lists should keep in accord with each other.
 */
enum mem_cgroup_stat_index {
	/*
	 * For MEM_CONTAINER_TYPE_ALL, usage = pagecache + rss.
	 */
	MEM_CGROUP_STAT_CACHE,		/* # of pages charged as cache */
	MEM_CGROUP_STAT_RSS,		/* # of pages charged as anon rss */
	MEM_CGROUP_STAT_RSS_HUGE,	/* # of pages charged as anon huge */
	MEM_CGROUP_STAT_FILE_MAPPED,	/* # of pages charged as file rss */
	MEM_CGROUP_STAT_WRITEBACK,	/* # of pages under writeback */
	MEM_CGROUP_STAT_SWAP,		/* # of pages, swapped out */
	MEM_CGROUP_STAT_NSTATS,
};

>>>>>>> refs/remotes/origin/master
struct mem_cgroup_reclaim_cookie {
	struct zone *zone;
	int priority;
	unsigned int generation;
};
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

#ifdef CONFIG_CGROUP_MEM_RES_CTLR
=======

#ifdef CONFIG_MEMCG
>>>>>>> refs/remotes/origin/master
/*
 * All "charge" functions with gfp_mask should use GFP_KERNEL or
 * (gfp_mask & GFP_RECLAIM_MASK). In current implementatin, memcg doesn't
 * alloc memory but reclaims memory from all available zones. So, "where I want
 * memory from" bits of gfp_mask has no meaning. So any bits of that field is
 * available but adding a rule is better. charge functions' gfp_mask should
 * be set to GFP_KERNEL or gfp_mask & GFP_RECLAIM_MASK for avoiding ambiguous
 * codes.
 * (Of course, if memcg does memory allocation in future, GFP_KERNEL is sane.)
 */

extern int mem_cgroup_newpage_charge(struct page *page, struct mm_struct *mm,
				gfp_t gfp_mask);
/* for swap handling */
extern int mem_cgroup_try_charge_swapin(struct mm_struct *mm,
<<<<<<< HEAD
<<<<<<< HEAD
		struct page *page, gfp_t mask, struct mem_cgroup **ptr);
extern void mem_cgroup_commit_charge_swapin(struct page *page,
					struct mem_cgroup *ptr);
extern void mem_cgroup_cancel_charge_swapin(struct mem_cgroup *ptr);

extern int mem_cgroup_cache_charge(struct page *page, struct mm_struct *mm,
					gfp_t gfp_mask);
extern void mem_cgroup_add_lru_list(struct page *page, enum lru_list lru);
extern void mem_cgroup_del_lru_list(struct page *page, enum lru_list lru);
extern void mem_cgroup_rotate_reclaimable_page(struct page *page);
extern void mem_cgroup_rotate_lru_list(struct page *page, enum lru_list lru);
extern void mem_cgroup_del_lru(struct page *page);
extern void mem_cgroup_move_lists(struct page *page,
				  enum lru_list from, enum lru_list to);
=======
=======
>>>>>>> refs/remotes/origin/master
		struct page *page, gfp_t mask, struct mem_cgroup **memcgp);
extern void mem_cgroup_commit_charge_swapin(struct page *page,
					struct mem_cgroup *memcg);
extern void mem_cgroup_cancel_charge_swapin(struct mem_cgroup *memcg);

extern int mem_cgroup_cache_charge(struct page *page, struct mm_struct *mm,
					gfp_t gfp_mask);

struct lruvec *mem_cgroup_zone_lruvec(struct zone *, struct mem_cgroup *);
<<<<<<< HEAD
struct lruvec *mem_cgroup_lru_add_list(struct zone *, struct page *,
				       enum lru_list);
void mem_cgroup_lru_del_list(struct page *, enum lru_list);
void mem_cgroup_lru_del(struct page *);
struct lruvec *mem_cgroup_lru_move_lists(struct zone *, struct page *,
					 enum lru_list, enum lru_list);
>>>>>>> refs/remotes/origin/cm-10.0
=======
struct lruvec *mem_cgroup_page_lruvec(struct page *, struct zone *);
>>>>>>> refs/remotes/origin/master

/* For coalescing uncharge for reducing memcg' overhead*/
extern void mem_cgroup_uncharge_start(void);
extern void mem_cgroup_uncharge_end(void);

extern void mem_cgroup_uncharge_page(struct page *page);
extern void mem_cgroup_uncharge_cache_page(struct page *page);
<<<<<<< HEAD
<<<<<<< HEAD
extern int mem_cgroup_shmem_charge_fallback(struct page *page,
			struct mm_struct *mm, gfp_t gfp_mask);

extern void mem_cgroup_out_of_memory(struct mem_cgroup *mem, gfp_t gfp_mask);
int task_in_mem_cgroup(struct task_struct *task, const struct mem_cgroup *mem);
=======

extern void mem_cgroup_out_of_memory(struct mem_cgroup *memcg, gfp_t gfp_mask,
				     int order);
int task_in_mem_cgroup(struct task_struct *task, const struct mem_cgroup *memcg);
>>>>>>> refs/remotes/origin/cm-10.0
=======

bool __mem_cgroup_same_or_subtree(const struct mem_cgroup *root_memcg,
				  struct mem_cgroup *memcg);
bool task_in_mem_cgroup(struct task_struct *task,
			const struct mem_cgroup *memcg);
>>>>>>> refs/remotes/origin/master

extern struct mem_cgroup *try_get_mem_cgroup_from_page(struct page *page);
extern struct mem_cgroup *mem_cgroup_from_task(struct task_struct *p);
extern struct mem_cgroup *try_get_mem_cgroup_from_mm(struct mm_struct *mm);

<<<<<<< HEAD
<<<<<<< HEAD
static inline
int mm_match_cgroup(const struct mm_struct *mm, const struct mem_cgroup *cgroup)
{
	struct mem_cgroup *mem;
	rcu_read_lock();
	mem = mem_cgroup_from_task(rcu_dereference((mm)->owner));
	rcu_read_unlock();
	return cgroup == mem;
}

extern struct cgroup_subsys_state *mem_cgroup_css(struct mem_cgroup *mem);

extern int
mem_cgroup_prepare_migration(struct page *page,
	struct page *newpage, struct mem_cgroup **ptr, gfp_t gfp_mask);
extern void mem_cgroup_end_migration(struct mem_cgroup *mem,
	struct page *oldpage, struct page *newpage, bool migration_ok);

/*
 * For memory reclaim.
 */
int mem_cgroup_inactive_anon_is_low(struct mem_cgroup *memcg);
int mem_cgroup_inactive_file_is_low(struct mem_cgroup *memcg);
int mem_cgroup_select_victim_node(struct mem_cgroup *memcg);
unsigned long mem_cgroup_zone_nr_lru_pages(struct mem_cgroup *memcg,
						struct zone *zone,
						enum lru_list lru);
=======
extern struct mem_cgroup *parent_mem_cgroup(struct mem_cgroup *memcg);
extern struct mem_cgroup *mem_cgroup_from_cont(struct cgroup *cont);

static inline
int mm_match_cgroup(const struct mm_struct *mm, const struct mem_cgroup *cgroup)
{
	struct mem_cgroup *memcg;
	rcu_read_lock();
	memcg = mem_cgroup_from_task(rcu_dereference((mm)->owner));
	rcu_read_unlock();
	return cgroup == memcg;
=======
extern struct mem_cgroup *parent_mem_cgroup(struct mem_cgroup *memcg);
extern struct mem_cgroup *mem_cgroup_from_css(struct cgroup_subsys_state *css);

static inline
bool mm_match_cgroup(const struct mm_struct *mm, const struct mem_cgroup *memcg)
{
	struct mem_cgroup *task_memcg;
	bool match;

	rcu_read_lock();
	task_memcg = mem_cgroup_from_task(rcu_dereference(mm->owner));
	match = __mem_cgroup_same_or_subtree(memcg, task_memcg);
	rcu_read_unlock();
	return match;
>>>>>>> refs/remotes/origin/master
}

extern struct cgroup_subsys_state *mem_cgroup_css(struct mem_cgroup *memcg);

<<<<<<< HEAD
extern int
mem_cgroup_prepare_migration(struct page *page,
	struct page *newpage, struct mem_cgroup **memcgp, gfp_t gfp_mask);
=======
extern void
mem_cgroup_prepare_migration(struct page *page, struct page *newpage,
			     struct mem_cgroup **memcgp);
>>>>>>> refs/remotes/origin/master
extern void mem_cgroup_end_migration(struct mem_cgroup *memcg,
	struct page *oldpage, struct page *newpage, bool migration_ok);

struct mem_cgroup *mem_cgroup_iter(struct mem_cgroup *,
				   struct mem_cgroup *,
				   struct mem_cgroup_reclaim_cookie *);
void mem_cgroup_iter_break(struct mem_cgroup *, struct mem_cgroup *);

/*
 * For memory reclaim.
 */
<<<<<<< HEAD
int mem_cgroup_inactive_anon_is_low(struct mem_cgroup *memcg,
				    struct zone *zone);
int mem_cgroup_inactive_file_is_low(struct mem_cgroup *memcg,
				    struct zone *zone);
int mem_cgroup_select_victim_node(struct mem_cgroup *memcg);
unsigned long mem_cgroup_zone_nr_lru_pages(struct mem_cgroup *memcg,
					int nid, int zid, unsigned int lrumask);
>>>>>>> refs/remotes/origin/cm-10.0
struct zone_reclaim_stat *mem_cgroup_get_reclaim_stat(struct mem_cgroup *memcg,
						      struct zone *zone);
struct zone_reclaim_stat*
mem_cgroup_get_reclaim_stat_from_page(struct page *page);
=======
int mem_cgroup_inactive_anon_is_low(struct lruvec *lruvec);
int mem_cgroup_select_victim_node(struct mem_cgroup *memcg);
unsigned long mem_cgroup_get_lru_size(struct lruvec *lruvec, enum lru_list);
void mem_cgroup_update_lru_size(struct lruvec *, enum lru_list, int);
>>>>>>> refs/remotes/origin/master
extern void mem_cgroup_print_oom_info(struct mem_cgroup *memcg,
					struct task_struct *p);
extern void mem_cgroup_replace_page_cache(struct page *oldpage,
					struct page *newpage);

<<<<<<< HEAD
#ifdef CONFIG_CGROUP_MEM_RES_CTLR_SWAP
=======
static inline void mem_cgroup_oom_enable(void)
{
	WARN_ON(current->memcg_oom.may_oom);
	current->memcg_oom.may_oom = 1;
}

static inline void mem_cgroup_oom_disable(void)
{
	WARN_ON(!current->memcg_oom.may_oom);
	current->memcg_oom.may_oom = 0;
}

static inline bool task_in_memcg_oom(struct task_struct *p)
{
	return p->memcg_oom.memcg;
}

bool mem_cgroup_oom_synchronize(bool wait);

#ifdef CONFIG_MEMCG_SWAP
>>>>>>> refs/remotes/origin/master
extern int do_swap_account;
#endif

static inline bool mem_cgroup_disabled(void)
{
	if (mem_cgroup_subsys.disabled)
		return true;
	return false;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
void __mem_cgroup_begin_update_page_stat(struct page *page, bool *locked,
					 unsigned long *flags);

extern atomic_t memcg_moving;

static inline void mem_cgroup_begin_update_page_stat(struct page *page,
					bool *locked, unsigned long *flags)
{
	if (mem_cgroup_disabled())
		return;
	rcu_read_lock();
	*locked = false;
	if (atomic_read(&memcg_moving))
		__mem_cgroup_begin_update_page_stat(page, locked, flags);
}

void __mem_cgroup_end_update_page_stat(struct page *page,
				unsigned long *flags);
static inline void mem_cgroup_end_update_page_stat(struct page *page,
					bool *locked, unsigned long *flags)
{
	if (mem_cgroup_disabled())
		return;
	if (*locked)
		__mem_cgroup_end_update_page_stat(page, flags);
	rcu_read_unlock();
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
void mem_cgroup_update_page_stat(struct page *page,
				 enum mem_cgroup_page_stat_item idx,
				 int val);

static inline void mem_cgroup_inc_page_stat(struct page *page,
					    enum mem_cgroup_page_stat_item idx)
=======
void mem_cgroup_update_page_stat(struct page *page,
				 enum mem_cgroup_stat_index idx,
				 int val);

static inline void mem_cgroup_inc_page_stat(struct page *page,
					    enum mem_cgroup_stat_index idx)
>>>>>>> refs/remotes/origin/master
{
	mem_cgroup_update_page_stat(page, idx, 1);
}

static inline void mem_cgroup_dec_page_stat(struct page *page,
<<<<<<< HEAD
					    enum mem_cgroup_page_stat_item idx)
=======
					    enum mem_cgroup_stat_index idx)
>>>>>>> refs/remotes/origin/master
{
	mem_cgroup_update_page_stat(page, idx, -1);
}

unsigned long mem_cgroup_soft_limit_reclaim(struct zone *zone, int order,
						gfp_t gfp_mask,
						unsigned long *total_scanned);
<<<<<<< HEAD
<<<<<<< HEAD
u64 mem_cgroup_get_limit(struct mem_cgroup *mem);

void mem_cgroup_count_vm_event(struct mm_struct *mm, enum vm_event_item idx);
#ifdef CONFIG_TRANSPARENT_HUGEPAGE
void mem_cgroup_split_huge_fixup(struct page *head, struct page *tail);
=======
u64 mem_cgroup_get_limit(struct mem_cgroup *memcg);

void mem_cgroup_count_vm_event(struct mm_struct *mm, enum vm_event_item idx);
#ifdef CONFIG_TRANSPARENT_HUGEPAGE
void mem_cgroup_split_huge_fixup(struct page *head);
>>>>>>> refs/remotes/origin/cm-10.0
=======

void __mem_cgroup_count_vm_event(struct mm_struct *mm, enum vm_event_item idx);
static inline void mem_cgroup_count_vm_event(struct mm_struct *mm,
					     enum vm_event_item idx)
{
	if (mem_cgroup_disabled())
		return;
	__mem_cgroup_count_vm_event(mm, idx);
}
#ifdef CONFIG_TRANSPARENT_HUGEPAGE
void mem_cgroup_split_huge_fixup(struct page *head);
>>>>>>> refs/remotes/origin/master
#endif

#ifdef CONFIG_DEBUG_VM
bool mem_cgroup_bad_page_check(struct page *page);
void mem_cgroup_print_bad_page(struct page *page);
#endif
<<<<<<< HEAD
#else /* CONFIG_CGROUP_MEM_RES_CTLR */
=======
#else /* CONFIG_MEMCG */
>>>>>>> refs/remotes/origin/master
struct mem_cgroup;

static inline int mem_cgroup_newpage_charge(struct page *page,
					struct mm_struct *mm, gfp_t gfp_mask)
{
	return 0;
}

static inline int mem_cgroup_cache_charge(struct page *page,
					struct mm_struct *mm, gfp_t gfp_mask)
{
	return 0;
}

static inline int mem_cgroup_try_charge_swapin(struct mm_struct *mm,
<<<<<<< HEAD
<<<<<<< HEAD
		struct page *page, gfp_t gfp_mask, struct mem_cgroup **ptr)
=======
		struct page *page, gfp_t gfp_mask, struct mem_cgroup **memcgp)
>>>>>>> refs/remotes/origin/cm-10.0
=======
		struct page *page, gfp_t gfp_mask, struct mem_cgroup **memcgp)
>>>>>>> refs/remotes/origin/master
{
	return 0;
}

static inline void mem_cgroup_commit_charge_swapin(struct page *page,
<<<<<<< HEAD
<<<<<<< HEAD
					  struct mem_cgroup *ptr)
{
}

static inline void mem_cgroup_cancel_charge_swapin(struct mem_cgroup *ptr)
=======
=======
>>>>>>> refs/remotes/origin/master
					  struct mem_cgroup *memcg)
{
}

static inline void mem_cgroup_cancel_charge_swapin(struct mem_cgroup *memcg)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
}

static inline void mem_cgroup_uncharge_start(void)
{
}

static inline void mem_cgroup_uncharge_end(void)
{
}

static inline void mem_cgroup_uncharge_page(struct page *page)
{
}

static inline void mem_cgroup_uncharge_cache_page(struct page *page)
{
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline int mem_cgroup_shmem_charge_fallback(struct page *page,
			struct mm_struct *mm, gfp_t gfp_mask)
{
	return 0;
}

static inline void mem_cgroup_add_lru_list(struct page *page, int lru)
{
}

static inline void mem_cgroup_del_lru_list(struct page *page, int lru)
{
	return ;
}

static inline void mem_cgroup_rotate_reclaimable_page(struct page *page)
{
	return ;
}

static inline void mem_cgroup_rotate_lru_list(struct page *page, int lru)
{
	return ;
}

static inline void mem_cgroup_del_lru(struct page *page)
{
	return ;
}

static inline void
mem_cgroup_move_lists(struct page *page, enum lru_list from, enum lru_list to)
{
=======
=======
>>>>>>> refs/remotes/origin/master
static inline struct lruvec *mem_cgroup_zone_lruvec(struct zone *zone,
						    struct mem_cgroup *memcg)
{
	return &zone->lruvec;
}

<<<<<<< HEAD
static inline struct lruvec *mem_cgroup_lru_add_list(struct zone *zone,
						     struct page *page,
						     enum lru_list lru)
=======
static inline struct lruvec *mem_cgroup_page_lruvec(struct page *page,
						    struct zone *zone)
>>>>>>> refs/remotes/origin/master
{
	return &zone->lruvec;
}

<<<<<<< HEAD
static inline void mem_cgroup_lru_del_list(struct page *page, enum lru_list lru)
{
}

static inline void mem_cgroup_lru_del(struct page *page)
{
}

static inline struct lruvec *mem_cgroup_lru_move_lists(struct zone *zone,
						       struct page *page,
						       enum lru_list from,
						       enum lru_list to)
{
	return &zone->lruvec;
>>>>>>> refs/remotes/origin/cm-10.0
}

=======
>>>>>>> refs/remotes/origin/master
static inline struct mem_cgroup *try_get_mem_cgroup_from_page(struct page *page)
{
	return NULL;
}

static inline struct mem_cgroup *try_get_mem_cgroup_from_mm(struct mm_struct *mm)
{
	return NULL;
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline int mm_match_cgroup(struct mm_struct *mm, struct mem_cgroup *mem)
=======
static inline int mm_match_cgroup(struct mm_struct *mm,
		struct mem_cgroup *memcg)
>>>>>>> refs/remotes/origin/cm-10.0
{
	return 1;
}

static inline int task_in_mem_cgroup(struct task_struct *task,
<<<<<<< HEAD
				     const struct mem_cgroup *mem)
=======
				     const struct mem_cgroup *memcg)
>>>>>>> refs/remotes/origin/cm-10.0
{
	return 1;
}

<<<<<<< HEAD
static inline struct cgroup_subsys_state *mem_cgroup_css(struct mem_cgroup *mem)
=======
static inline struct cgroup_subsys_state
		*mem_cgroup_css(struct mem_cgroup *memcg)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static inline bool mm_match_cgroup(struct mm_struct *mm,
		struct mem_cgroup *memcg)
{
	return true;
}

static inline bool task_in_mem_cgroup(struct task_struct *task,
				      const struct mem_cgroup *memcg)
{
	return true;
}

static inline struct cgroup_subsys_state
		*mem_cgroup_css(struct mem_cgroup *memcg)
>>>>>>> refs/remotes/origin/master
{
	return NULL;
}

<<<<<<< HEAD
static inline int
mem_cgroup_prepare_migration(struct page *page, struct page *newpage,
<<<<<<< HEAD
	struct mem_cgroup **ptr, gfp_t gfp_mask)
=======
	struct mem_cgroup **memcgp, gfp_t gfp_mask)
>>>>>>> refs/remotes/origin/cm-10.0
{
	return 0;
}

<<<<<<< HEAD
static inline void mem_cgroup_end_migration(struct mem_cgroup *mem,
=======
static inline void mem_cgroup_end_migration(struct mem_cgroup *memcg,
>>>>>>> refs/remotes/origin/cm-10.0
=======
static inline void
mem_cgroup_prepare_migration(struct page *page, struct page *newpage,
			     struct mem_cgroup **memcgp)
{
}

static inline void mem_cgroup_end_migration(struct mem_cgroup *memcg,
>>>>>>> refs/remotes/origin/master
		struct page *oldpage, struct page *newpage, bool migration_ok)
{
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline int mem_cgroup_get_reclaim_priority(struct mem_cgroup *mem)
{
	return 0;
}

static inline void mem_cgroup_note_reclaim_priority(struct mem_cgroup *mem,
						int priority)
{
}

static inline void mem_cgroup_record_reclaim_priority(struct mem_cgroup *mem,
						int priority)
=======
=======
>>>>>>> refs/remotes/origin/master
static inline struct mem_cgroup *
mem_cgroup_iter(struct mem_cgroup *root,
		struct mem_cgroup *prev,
		struct mem_cgroup_reclaim_cookie *reclaim)
{
	return NULL;
}

static inline void mem_cgroup_iter_break(struct mem_cgroup *root,
					 struct mem_cgroup *prev)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
}

static inline bool mem_cgroup_disabled(void)
{
	return true;
}

static inline int
<<<<<<< HEAD
<<<<<<< HEAD
mem_cgroup_inactive_anon_is_low(struct mem_cgroup *memcg)
=======
mem_cgroup_inactive_anon_is_low(struct mem_cgroup *memcg, struct zone *zone)
>>>>>>> refs/remotes/origin/cm-10.0
{
	return 1;
}

static inline int
<<<<<<< HEAD
mem_cgroup_inactive_file_is_low(struct mem_cgroup *memcg)
=======
mem_cgroup_inactive_file_is_low(struct mem_cgroup *memcg, struct zone *zone)
>>>>>>> refs/remotes/origin/cm-10.0
=======
mem_cgroup_inactive_anon_is_low(struct lruvec *lruvec)
>>>>>>> refs/remotes/origin/master
{
	return 1;
}

static inline unsigned long
<<<<<<< HEAD
<<<<<<< HEAD
mem_cgroup_zone_nr_lru_pages(struct mem_cgroup *memcg, struct zone *zone,
			     enum lru_list lru)
=======
mem_cgroup_zone_nr_lru_pages(struct mem_cgroup *memcg, int nid, int zid,
				unsigned int lru_mask)
>>>>>>> refs/remotes/origin/cm-10.0
=======
mem_cgroup_get_lru_size(struct lruvec *lruvec, enum lru_list lru)
>>>>>>> refs/remotes/origin/master
{
	return 0;
}

<<<<<<< HEAD

static inline struct zone_reclaim_stat*
mem_cgroup_get_reclaim_stat(struct mem_cgroup *memcg, struct zone *zone)
{
	return NULL;
}

static inline struct zone_reclaim_stat*
mem_cgroup_get_reclaim_stat_from_page(struct page *page)
{
	return NULL;
=======
static inline void
mem_cgroup_update_lru_size(struct lruvec *lruvec, enum lru_list lru,
			      int increment)
{
>>>>>>> refs/remotes/origin/master
}

static inline void
mem_cgroup_print_oom_info(struct mem_cgroup *memcg, struct task_struct *p)
{
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static inline void mem_cgroup_begin_update_page_stat(struct page *page,
					bool *locked, unsigned long *flags)
{
}

static inline void mem_cgroup_end_update_page_stat(struct page *page,
					bool *locked, unsigned long *flags)
{
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
static inline void mem_cgroup_inc_page_stat(struct page *page,
					    enum mem_cgroup_page_stat_item idx)
=======
static inline void mem_cgroup_oom_enable(void)
{
}

static inline void mem_cgroup_oom_disable(void)
{
}

static inline bool task_in_memcg_oom(struct task_struct *p)
{
	return false;
}

static inline bool mem_cgroup_oom_synchronize(bool wait)
{
	return false;
}

static inline void mem_cgroup_inc_page_stat(struct page *page,
					    enum mem_cgroup_stat_index idx)
>>>>>>> refs/remotes/origin/master
{
}

static inline void mem_cgroup_dec_page_stat(struct page *page,
<<<<<<< HEAD
					    enum mem_cgroup_page_stat_item idx)
=======
					    enum mem_cgroup_stat_index idx)
>>>>>>> refs/remotes/origin/master
{
}

static inline
unsigned long mem_cgroup_soft_limit_reclaim(struct zone *zone, int order,
					    gfp_t gfp_mask,
					    unsigned long *total_scanned)
{
	return 0;
}

<<<<<<< HEAD
static inline
<<<<<<< HEAD
u64 mem_cgroup_get_limit(struct mem_cgroup *mem)
=======
u64 mem_cgroup_get_limit(struct mem_cgroup *memcg)
>>>>>>> refs/remotes/origin/cm-10.0
{
	return 0;
}

<<<<<<< HEAD
static inline void mem_cgroup_split_huge_fixup(struct page *head,
						struct page *tail)
=======
static inline void mem_cgroup_split_huge_fixup(struct page *head)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static inline void mem_cgroup_split_huge_fixup(struct page *head)
>>>>>>> refs/remotes/origin/master
{
}

static inline
void mem_cgroup_count_vm_event(struct mm_struct *mm, enum vm_event_item idx)
{
}
static inline void mem_cgroup_replace_page_cache(struct page *oldpage,
				struct page *newpage)
{
}
<<<<<<< HEAD
<<<<<<< HEAD
#endif /* CONFIG_CGROUP_MEM_CONT */
=======
#endif /* CONFIG_CGROUP_MEM_RES_CTLR */
>>>>>>> refs/remotes/origin/cm-10.0

#if !defined(CONFIG_CGROUP_MEM_RES_CTLR) || !defined(CONFIG_DEBUG_VM)
=======
#endif /* CONFIG_MEMCG */

#if !defined(CONFIG_MEMCG) || !defined(CONFIG_DEBUG_VM)
>>>>>>> refs/remotes/origin/master
static inline bool
mem_cgroup_bad_page_check(struct page *page)
{
	return false;
}

static inline void
mem_cgroup_print_bad_page(struct page *page)
{
}
#endif

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
enum {
	UNDER_LIMIT,
	SOFT_LIMIT,
	OVER_LIMIT,
};

struct sock;
<<<<<<< HEAD
#ifdef CONFIG_CGROUP_MEM_RES_CTLR_KMEM
=======
#if defined(CONFIG_INET) && defined(CONFIG_MEMCG_KMEM)
>>>>>>> refs/remotes/origin/master
void sock_update_memcg(struct sock *sk);
void sock_release_memcg(struct sock *sk);
#else
static inline void sock_update_memcg(struct sock *sk)
{
}
static inline void sock_release_memcg(struct sock *sk)
{
}
<<<<<<< HEAD
#endif /* CONFIG_CGROUP_MEM_RES_CTLR_KMEM */
>>>>>>> refs/remotes/origin/cm-10.0
=======
#endif /* CONFIG_INET && CONFIG_MEMCG_KMEM */

#ifdef CONFIG_MEMCG_KMEM
extern struct static_key memcg_kmem_enabled_key;

extern int memcg_limited_groups_array_size;

/*
 * Helper macro to loop through all memcg-specific caches. Callers must still
 * check if the cache is valid (it is either valid or NULL).
 * the slab_mutex must be held when looping through those caches
 */
#define for_each_memcg_cache_index(_idx)	\
	for ((_idx) = 0; (_idx) < memcg_limited_groups_array_size; (_idx)++)

static inline bool memcg_kmem_enabled(void)
{
	return static_key_false(&memcg_kmem_enabled_key);
}

/*
 * In general, we'll do everything in our power to not incur in any overhead
 * for non-memcg users for the kmem functions. Not even a function call, if we
 * can avoid it.
 *
 * Therefore, we'll inline all those functions so that in the best case, we'll
 * see that kmemcg is off for everybody and proceed quickly.  If it is on,
 * we'll still do most of the flag checking inline. We check a lot of
 * conditions, but because they are pretty simple, they are expected to be
 * fast.
 */
bool __memcg_kmem_newpage_charge(gfp_t gfp, struct mem_cgroup **memcg,
					int order);
void __memcg_kmem_commit_charge(struct page *page,
				       struct mem_cgroup *memcg, int order);
void __memcg_kmem_uncharge_pages(struct page *page, int order);

int memcg_cache_id(struct mem_cgroup *memcg);
int memcg_register_cache(struct mem_cgroup *memcg, struct kmem_cache *s,
			 struct kmem_cache *root_cache);
void memcg_release_cache(struct kmem_cache *cachep);
void memcg_cache_list_add(struct mem_cgroup *memcg, struct kmem_cache *cachep);

int memcg_update_cache_size(struct kmem_cache *s, int num_groups);
void memcg_update_array_size(int num_groups);

struct kmem_cache *
__memcg_kmem_get_cache(struct kmem_cache *cachep, gfp_t gfp);

void mem_cgroup_destroy_cache(struct kmem_cache *cachep);
void kmem_cache_destroy_memcg_children(struct kmem_cache *s);

/**
 * memcg_kmem_newpage_charge: verify if a new kmem allocation is allowed.
 * @gfp: the gfp allocation flags.
 * @memcg: a pointer to the memcg this was charged against.
 * @order: allocation order.
 *
 * returns true if the memcg where the current task belongs can hold this
 * allocation.
 *
 * We return true automatically if this allocation is not to be accounted to
 * any memcg.
 */
static inline bool
memcg_kmem_newpage_charge(gfp_t gfp, struct mem_cgroup **memcg, int order)
{
	if (!memcg_kmem_enabled())
		return true;

	/*
	 * __GFP_NOFAIL allocations will move on even if charging is not
	 * possible. Therefore we don't even try, and have this allocation
	 * unaccounted. We could in theory charge it with
	 * res_counter_charge_nofail, but we hope those allocations are rare,
	 * and won't be worth the trouble.
	 */
	if (!(gfp & __GFP_KMEMCG) || (gfp & __GFP_NOFAIL))
		return true;
	if (in_interrupt() || (!current->mm) || (current->flags & PF_KTHREAD))
		return true;

	/* If the test is dying, just let it go. */
	if (unlikely(fatal_signal_pending(current)))
		return true;

	return __memcg_kmem_newpage_charge(gfp, memcg, order);
}

/**
 * memcg_kmem_uncharge_pages: uncharge pages from memcg
 * @page: pointer to struct page being freed
 * @order: allocation order.
 *
 * there is no need to specify memcg here, since it is embedded in page_cgroup
 */
static inline void
memcg_kmem_uncharge_pages(struct page *page, int order)
{
	if (memcg_kmem_enabled())
		__memcg_kmem_uncharge_pages(page, order);
}

/**
 * memcg_kmem_commit_charge: embeds correct memcg in a page
 * @page: pointer to struct page recently allocated
 * @memcg: the memcg structure we charged against
 * @order: allocation order.
 *
 * Needs to be called after memcg_kmem_newpage_charge, regardless of success or
 * failure of the allocation. if @page is NULL, this function will revert the
 * charges. Otherwise, it will commit the memcg given by @memcg to the
 * corresponding page_cgroup.
 */
static inline void
memcg_kmem_commit_charge(struct page *page, struct mem_cgroup *memcg, int order)
{
	if (memcg_kmem_enabled() && memcg)
		__memcg_kmem_commit_charge(page, memcg, order);
}

/**
 * memcg_kmem_get_cache: selects the correct per-memcg cache for allocation
 * @cachep: the original global kmem cache
 * @gfp: allocation flags.
 *
 * This function assumes that the task allocating, which determines the memcg
 * in the page allocator, belongs to the same cgroup throughout the whole
 * process.  Misacounting can happen if the task calls memcg_kmem_get_cache()
 * while belonging to a cgroup, and later on changes. This is considered
 * acceptable, and should only happen upon task migration.
 *
 * Before the cache is created by the memcg core, there is also a possible
 * imbalance: the task belongs to a memcg, but the cache being allocated from
 * is the global cache, since the child cache is not yet guaranteed to be
 * ready. This case is also fine, since in this case the GFP_KMEMCG will not be
 * passed and the page allocator will not attempt any cgroup accounting.
 */
static __always_inline struct kmem_cache *
memcg_kmem_get_cache(struct kmem_cache *cachep, gfp_t gfp)
{
	if (!memcg_kmem_enabled())
		return cachep;
	if (gfp & __GFP_NOFAIL)
		return cachep;
	if (in_interrupt() || (!current->mm) || (current->flags & PF_KTHREAD))
		return cachep;
	if (unlikely(fatal_signal_pending(current)))
		return cachep;

	return __memcg_kmem_get_cache(cachep, gfp);
}
#else
#define for_each_memcg_cache_index(_idx)	\
	for (; NULL; )

static inline bool memcg_kmem_enabled(void)
{
	return false;
}

static inline bool
memcg_kmem_newpage_charge(gfp_t gfp, struct mem_cgroup **memcg, int order)
{
	return true;
}

static inline void memcg_kmem_uncharge_pages(struct page *page, int order)
{
}

static inline void
memcg_kmem_commit_charge(struct page *page, struct mem_cgroup *memcg, int order)
{
}

static inline int memcg_cache_id(struct mem_cgroup *memcg)
{
	return -1;
}

static inline int
memcg_register_cache(struct mem_cgroup *memcg, struct kmem_cache *s,
		     struct kmem_cache *root_cache)
{
	return 0;
}

static inline void memcg_release_cache(struct kmem_cache *cachep)
{
}

static inline void memcg_cache_list_add(struct mem_cgroup *memcg,
					struct kmem_cache *s)
{
}

static inline struct kmem_cache *
memcg_kmem_get_cache(struct kmem_cache *cachep, gfp_t gfp)
{
	return cachep;
}

static inline void kmem_cache_destroy_memcg_children(struct kmem_cache *s)
{
}
#endif /* CONFIG_MEMCG_KMEM */
>>>>>>> refs/remotes/origin/master
#endif /* _LINUX_MEMCONTROL_H */

