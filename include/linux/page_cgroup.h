#ifndef __LINUX_PAGE_CGROUP_H
#define __LINUX_PAGE_CGROUP_H

enum {
	/* flags for mem_cgroup */
	PCG_LOCK,  /* Lock for pc->mem_cgroup and following bits. */
<<<<<<< HEAD
<<<<<<< HEAD
	PCG_CACHE, /* charged as cache */
	PCG_USED, /* this object is in use. */
	PCG_MIGRATION, /* under page migration */
	/* flags for mem_cgroup and file and I/O status */
	PCG_MOVE_LOCK, /* For race between move_account v.s. following bits */
	PCG_FILE_MAPPED, /* page is accounted as "mapped" */
	/* No lock in page_cgroup */
	PCG_ACCT_LRU, /* page has been accounted for (under lru_lock) */
=======
	PCG_USED, /* this object is in use. */
	PCG_MIGRATION, /* under page migration */
>>>>>>> refs/remotes/origin/cm-10.0
=======
	PCG_USED, /* this object is in use. */
	PCG_MIGRATION, /* under page migration */
>>>>>>> refs/remotes/origin/master
	__NR_PCG_FLAGS,
};

#ifndef __GENERATING_BOUNDS_H
#include <generated/bounds.h>

<<<<<<< HEAD
#ifdef CONFIG_CGROUP_MEM_RES_CTLR
=======
#ifdef CONFIG_MEMCG
>>>>>>> refs/remotes/origin/master
#include <linux/bit_spinlock.h>

/*
 * Page Cgroup can be considered as an extended mem_map.
 * A page_cgroup page is associated with every page descriptor. The
 * page_cgroup helps us identify information about the cgroup
 * All page cgroups are allocated at boot or memory hotplug event,
 * then the page cgroup for pfn always exists.
 */
struct page_cgroup {
	unsigned long flags;
	struct mem_cgroup *mem_cgroup;
<<<<<<< HEAD
<<<<<<< HEAD
	struct list_head lru;		/* per cgroup LRU list */
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};

void __meminit pgdat_page_cgroup_init(struct pglist_data *pgdat);

#ifdef CONFIG_SPARSEMEM
static inline void __init page_cgroup_init_flatmem(void)
{
}
extern void __init page_cgroup_init(void);
#else
void __init page_cgroup_init_flatmem(void);
static inline void __init page_cgroup_init(void)
{
}
#endif

struct page_cgroup *lookup_page_cgroup(struct page *page);
struct page *lookup_cgroup_page(struct page_cgroup *pc);

#define TESTPCGFLAG(uname, lname)			\
static inline int PageCgroup##uname(struct page_cgroup *pc)	\
	{ return test_bit(PCG_##lname, &pc->flags); }

#define SETPCGFLAG(uname, lname)			\
static inline void SetPageCgroup##uname(struct page_cgroup *pc)\
	{ set_bit(PCG_##lname, &pc->flags);  }

#define CLEARPCGFLAG(uname, lname)			\
static inline void ClearPageCgroup##uname(struct page_cgroup *pc)	\
	{ clear_bit(PCG_##lname, &pc->flags);  }

#define TESTCLEARPCGFLAG(uname, lname)			\
static inline int TestClearPageCgroup##uname(struct page_cgroup *pc)	\
	{ return test_and_clear_bit(PCG_##lname, &pc->flags);  }

<<<<<<< HEAD
<<<<<<< HEAD
/* Cache flag is set only once (at allocation) */
TESTPCGFLAG(Cache, CACHE)
CLEARPCGFLAG(Cache, CACHE)
SETPCGFLAG(Cache, CACHE)

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
TESTPCGFLAG(Used, USED)
CLEARPCGFLAG(Used, USED)
SETPCGFLAG(Used, USED)

<<<<<<< HEAD
<<<<<<< HEAD
SETPCGFLAG(AcctLRU, ACCT_LRU)
CLEARPCGFLAG(AcctLRU, ACCT_LRU)
TESTPCGFLAG(AcctLRU, ACCT_LRU)
TESTCLEARPCGFLAG(AcctLRU, ACCT_LRU)


SETPCGFLAG(FileMapped, FILE_MAPPED)
CLEARPCGFLAG(FileMapped, FILE_MAPPED)
TESTPCGFLAG(FileMapped, FILE_MAPPED)

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
SETPCGFLAG(Migration, MIGRATION)
CLEARPCGFLAG(Migration, MIGRATION)
TESTPCGFLAG(Migration, MIGRATION)

static inline void lock_page_cgroup(struct page_cgroup *pc)
{
	/*
	 * Don't take this lock in IRQ context.
<<<<<<< HEAD
<<<<<<< HEAD
	 * This lock is for pc->mem_cgroup, USED, CACHE, MIGRATION
=======
	 * This lock is for pc->mem_cgroup, USED, MIGRATION
>>>>>>> refs/remotes/origin/cm-10.0
=======
	 * This lock is for pc->mem_cgroup, USED, MIGRATION
>>>>>>> refs/remotes/origin/master
	 */
	bit_spin_lock(PCG_LOCK, &pc->flags);
}

static inline void unlock_page_cgroup(struct page_cgroup *pc)
{
	bit_spin_unlock(PCG_LOCK, &pc->flags);
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline void move_lock_page_cgroup(struct page_cgroup *pc,
	unsigned long *flags)
{
	/*
	 * We know updates to pc->flags of page cache's stats are from both of
	 * usual context or IRQ context. Disable IRQ to avoid deadlock.
	 */
	local_irq_save(*flags);
	bit_spin_lock(PCG_MOVE_LOCK, &pc->flags);
}

static inline void move_unlock_page_cgroup(struct page_cgroup *pc,
	unsigned long *flags)
{
	bit_spin_unlock(PCG_MOVE_LOCK, &pc->flags);
	local_irq_restore(*flags);
}

#ifdef CONFIG_SPARSEMEM
#define PCG_ARRAYID_WIDTH	SECTIONS_SHIFT
#else
#define PCG_ARRAYID_WIDTH	NODES_SHIFT
#endif

#if (PCG_ARRAYID_WIDTH > BITS_PER_LONG - NR_PCG_FLAGS)
#error Not enough space left in pc->flags to store page_cgroup array IDs
#endif

/* pc->flags: ARRAY-ID | FLAGS */

#define PCG_ARRAYID_MASK	((1UL << PCG_ARRAYID_WIDTH) - 1)

#define PCG_ARRAYID_OFFSET	(BITS_PER_LONG - PCG_ARRAYID_WIDTH)
/*
 * Zero the shift count for non-existent fields, to prevent compiler
 * warnings and ensure references are optimized away.
 */
#define PCG_ARRAYID_SHIFT	(PCG_ARRAYID_OFFSET * (PCG_ARRAYID_WIDTH != 0))

static inline void set_page_cgroup_array_id(struct page_cgroup *pc,
					    unsigned long id)
{
	pc->flags &= ~(PCG_ARRAYID_MASK << PCG_ARRAYID_SHIFT);
	pc->flags |= (id & PCG_ARRAYID_MASK) << PCG_ARRAYID_SHIFT;
}

static inline unsigned long page_cgroup_array_id(struct page_cgroup *pc)
{
	return (pc->flags >> PCG_ARRAYID_SHIFT) & PCG_ARRAYID_MASK;
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
#else /* CONFIG_CGROUP_MEM_RES_CTLR */
=======
#else /* CONFIG_MEMCG */
>>>>>>> refs/remotes/origin/master
struct page_cgroup;

static inline void __meminit pgdat_page_cgroup_init(struct pglist_data *pgdat)
{
}

static inline struct page_cgroup *lookup_page_cgroup(struct page *page)
{
	return NULL;
}

static inline void page_cgroup_init(void)
{
}

static inline void __init page_cgroup_init_flatmem(void)
{
}

<<<<<<< HEAD
#endif /* CONFIG_CGROUP_MEM_RES_CTLR */

#include <linux/swap.h>

#ifdef CONFIG_CGROUP_MEM_RES_CTLR_SWAP
extern unsigned short swap_cgroup_cmpxchg(swp_entry_t ent,
					unsigned short old, unsigned short new);
extern unsigned short swap_cgroup_record(swp_entry_t ent, unsigned short id);
<<<<<<< HEAD
extern unsigned short lookup_swap_cgroup(swp_entry_t ent);
=======
extern unsigned short lookup_swap_cgroup_id(swp_entry_t ent);
>>>>>>> refs/remotes/origin/cm-10.0
=======
#endif /* CONFIG_MEMCG */

#include <linux/swap.h>

#ifdef CONFIG_MEMCG_SWAP
extern unsigned short swap_cgroup_cmpxchg(swp_entry_t ent,
					unsigned short old, unsigned short new);
extern unsigned short swap_cgroup_record(swp_entry_t ent, unsigned short id);
extern unsigned short lookup_swap_cgroup_id(swp_entry_t ent);
>>>>>>> refs/remotes/origin/master
extern int swap_cgroup_swapon(int type, unsigned long max_pages);
extern void swap_cgroup_swapoff(int type);
#else

static inline
unsigned short swap_cgroup_record(swp_entry_t ent, unsigned short id)
{
	return 0;
}

static inline
<<<<<<< HEAD
<<<<<<< HEAD
unsigned short lookup_swap_cgroup(swp_entry_t ent)
=======
unsigned short lookup_swap_cgroup_id(swp_entry_t ent)
>>>>>>> refs/remotes/origin/cm-10.0
=======
unsigned short lookup_swap_cgroup_id(swp_entry_t ent)
>>>>>>> refs/remotes/origin/master
{
	return 0;
}

static inline int
swap_cgroup_swapon(int type, unsigned long max_pages)
{
	return 0;
}

static inline void swap_cgroup_swapoff(int type)
{
	return;
}

<<<<<<< HEAD
#endif /* CONFIG_CGROUP_MEM_RES_CTLR_SWAP */
=======
#endif /* CONFIG_MEMCG_SWAP */
>>>>>>> refs/remotes/origin/master

#endif /* !__GENERATING_BOUNDS_H */

#endif /* __LINUX_PAGE_CGROUP_H */
