#ifndef _LINUX_HUGE_MM_H
#define _LINUX_HUGE_MM_H

extern int do_huge_pmd_anonymous_page(struct mm_struct *mm,
				      struct vm_area_struct *vma,
				      unsigned long address, pmd_t *pmd,
				      unsigned int flags);
extern int copy_huge_pmd(struct mm_struct *dst_mm, struct mm_struct *src_mm,
			 pmd_t *dst_pmd, pmd_t *src_pmd, unsigned long addr,
			 struct vm_area_struct *vma);
<<<<<<< HEAD
extern int do_huge_pmd_wp_page(struct mm_struct *mm, struct vm_area_struct *vma,
			       unsigned long address, pmd_t *pmd,
			       pmd_t orig_pmd);
extern pgtable_t get_pmd_huge_pte(struct mm_struct *mm);
extern struct page *follow_trans_huge_pmd(struct mm_struct *mm,
=======
extern void huge_pmd_set_accessed(struct mm_struct *mm,
				  struct vm_area_struct *vma,
				  unsigned long address, pmd_t *pmd,
				  pmd_t orig_pmd, int dirty);
extern int do_huge_pmd_wp_page(struct mm_struct *mm, struct vm_area_struct *vma,
			       unsigned long address, pmd_t *pmd,
			       pmd_t orig_pmd);
extern struct page *follow_trans_huge_pmd(struct vm_area_struct *vma,
>>>>>>> refs/remotes/origin/master
					  unsigned long addr,
					  pmd_t *pmd,
					  unsigned int flags);
extern int zap_huge_pmd(struct mmu_gather *tlb,
			struct vm_area_struct *vma,
<<<<<<< HEAD
<<<<<<< HEAD
			pmd_t *pmd);
extern int mincore_huge_pmd(struct vm_area_struct *vma, pmd_t *pmd,
			unsigned long addr, unsigned long end,
			unsigned char *vec);
=======
=======
>>>>>>> refs/remotes/origin/master
			pmd_t *pmd, unsigned long addr);
extern int mincore_huge_pmd(struct vm_area_struct *vma, pmd_t *pmd,
			unsigned long addr, unsigned long end,
			unsigned char *vec);
extern int move_huge_pmd(struct vm_area_struct *vma,
			 struct vm_area_struct *new_vma,
			 unsigned long old_addr,
			 unsigned long new_addr, unsigned long old_end,
			 pmd_t *old_pmd, pmd_t *new_pmd);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
extern int change_huge_pmd(struct vm_area_struct *vma, pmd_t *pmd,
			unsigned long addr, pgprot_t newprot);
=======
extern int change_huge_pmd(struct vm_area_struct *vma, pmd_t *pmd,
			unsigned long addr, pgprot_t newprot,
			int prot_numa);
>>>>>>> refs/remotes/origin/master

enum transparent_hugepage_flag {
	TRANSPARENT_HUGEPAGE_FLAG,
	TRANSPARENT_HUGEPAGE_REQ_MADV_FLAG,
	TRANSPARENT_HUGEPAGE_DEFRAG_FLAG,
	TRANSPARENT_HUGEPAGE_DEFRAG_REQ_MADV_FLAG,
	TRANSPARENT_HUGEPAGE_DEFRAG_KHUGEPAGED_FLAG,
<<<<<<< HEAD
=======
	TRANSPARENT_HUGEPAGE_USE_ZERO_PAGE_FLAG,
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_DEBUG_VM
	TRANSPARENT_HUGEPAGE_DEBUG_COW_FLAG,
#endif
};

enum page_check_address_pmd_flag {
	PAGE_CHECK_ADDRESS_PMD_FLAG,
	PAGE_CHECK_ADDRESS_PMD_NOTSPLITTING_FLAG,
	PAGE_CHECK_ADDRESS_PMD_SPLITTING_FLAG,
};
extern pmd_t *page_check_address_pmd(struct page *page,
				     struct mm_struct *mm,
				     unsigned long address,
<<<<<<< HEAD
				     enum page_check_address_pmd_flag flag);

<<<<<<< HEAD
=======
#define HPAGE_PMD_ORDER (HPAGE_PMD_SHIFT-PAGE_SHIFT)
#define HPAGE_PMD_NR (1<<HPAGE_PMD_ORDER)

>>>>>>> refs/remotes/origin/cm-10.0
#ifdef CONFIG_TRANSPARENT_HUGEPAGE
#define HPAGE_PMD_SHIFT HPAGE_SHIFT
#define HPAGE_PMD_MASK HPAGE_MASK
#define HPAGE_PMD_SIZE HPAGE_SIZE
=======
				     enum page_check_address_pmd_flag flag,
				     spinlock_t **ptl);

#define HPAGE_PMD_ORDER (HPAGE_PMD_SHIFT-PAGE_SHIFT)
#define HPAGE_PMD_NR (1<<HPAGE_PMD_ORDER)

#ifdef CONFIG_TRANSPARENT_HUGEPAGE
#define HPAGE_PMD_SHIFT PMD_SHIFT
#define HPAGE_PMD_SIZE	((1UL) << HPAGE_PMD_SHIFT)
#define HPAGE_PMD_MASK	(~(HPAGE_PMD_SIZE - 1))

extern bool is_vma_temporary_stack(struct vm_area_struct *vma);
>>>>>>> refs/remotes/origin/master

#define transparent_hugepage_enabled(__vma)				\
	((transparent_hugepage_flags &					\
	  (1<<TRANSPARENT_HUGEPAGE_FLAG) ||				\
	  (transparent_hugepage_flags &					\
	   (1<<TRANSPARENT_HUGEPAGE_REQ_MADV_FLAG) &&			\
	   ((__vma)->vm_flags & VM_HUGEPAGE))) &&			\
	 !((__vma)->vm_flags & VM_NOHUGEPAGE) &&			\
	 !is_vma_temporary_stack(__vma))
#define transparent_hugepage_defrag(__vma)				\
	((transparent_hugepage_flags &					\
	  (1<<TRANSPARENT_HUGEPAGE_DEFRAG_FLAG)) ||			\
	 (transparent_hugepage_flags &					\
	  (1<<TRANSPARENT_HUGEPAGE_DEFRAG_REQ_MADV_FLAG) &&		\
	  (__vma)->vm_flags & VM_HUGEPAGE))
<<<<<<< HEAD
=======
#define transparent_hugepage_use_zero_page()				\
	(transparent_hugepage_flags &					\
	 (1<<TRANSPARENT_HUGEPAGE_USE_ZERO_PAGE_FLAG))
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_DEBUG_VM
#define transparent_hugepage_debug_cow()				\
	(transparent_hugepage_flags &					\
	 (1<<TRANSPARENT_HUGEPAGE_DEBUG_COW_FLAG))
#else /* CONFIG_DEBUG_VM */
#define transparent_hugepage_debug_cow() 0
#endif /* CONFIG_DEBUG_VM */

extern unsigned long transparent_hugepage_flags;
extern int copy_pte_range(struct mm_struct *dst_mm, struct mm_struct *src_mm,
			  pmd_t *dst_pmd, pmd_t *src_pmd,
			  struct vm_area_struct *vma,
			  unsigned long addr, unsigned long end);
<<<<<<< HEAD
extern int handle_pte_fault(struct mm_struct *mm,
			    struct vm_area_struct *vma, unsigned long address,
			    pte_t *pte, pmd_t *pmd, unsigned int flags);
extern int split_huge_page(struct page *page);
extern void __split_huge_page_pmd(struct mm_struct *mm, pmd_t *pmd);
#define split_huge_page_pmd(__mm, __pmd)				\
	do {								\
		pmd_t *____pmd = (__pmd);				\
		if (unlikely(pmd_trans_huge(*____pmd)))			\
			__split_huge_page_pmd(__mm, ____pmd);		\
=======
extern int split_huge_page_to_list(struct page *page, struct list_head *list);
static inline int split_huge_page(struct page *page)
{
	return split_huge_page_to_list(page, NULL);
}
extern void __split_huge_page_pmd(struct vm_area_struct *vma,
		unsigned long address, pmd_t *pmd);
#define split_huge_page_pmd(__vma, __address, __pmd)			\
	do {								\
		pmd_t *____pmd = (__pmd);				\
		if (unlikely(pmd_trans_huge(*____pmd)))			\
			__split_huge_page_pmd(__vma, __address,		\
					____pmd);			\
>>>>>>> refs/remotes/origin/master
	}  while (0)
#define wait_split_huge_page(__anon_vma, __pmd)				\
	do {								\
		pmd_t *____pmd = (__pmd);				\
<<<<<<< HEAD
		anon_vma_lock(__anon_vma);				\
		anon_vma_unlock(__anon_vma);				\
		BUG_ON(pmd_trans_splitting(*____pmd) ||			\
		       pmd_trans_huge(*____pmd));			\
	} while (0)
<<<<<<< HEAD
#define HPAGE_PMD_ORDER (HPAGE_PMD_SHIFT-PAGE_SHIFT)
#define HPAGE_PMD_NR (1<<HPAGE_PMD_ORDER)
=======
>>>>>>> refs/remotes/origin/cm-10.0
#if HPAGE_PMD_ORDER > MAX_ORDER
=======
		anon_vma_lock_write(__anon_vma);			\
		anon_vma_unlock_write(__anon_vma);			\
		BUG_ON(pmd_trans_splitting(*____pmd) ||			\
		       pmd_trans_huge(*____pmd));			\
	} while (0)
extern void split_huge_page_pmd_mm(struct mm_struct *mm, unsigned long address,
		pmd_t *pmd);
#if HPAGE_PMD_ORDER >= MAX_ORDER
>>>>>>> refs/remotes/origin/master
#error "hugepages can't be allocated by the buddy allocator"
#endif
extern int hugepage_madvise(struct vm_area_struct *vma,
			    unsigned long *vm_flags, int advice);
extern void __vma_adjust_trans_huge(struct vm_area_struct *vma,
				    unsigned long start,
				    unsigned long end,
				    long adjust_next);
<<<<<<< HEAD
<<<<<<< HEAD
=======
extern int __pmd_trans_huge_lock(pmd_t *pmd,
				 struct vm_area_struct *vma);
/* mmap_sem must be held on entry */
static inline int pmd_trans_huge_lock(pmd_t *pmd,
				      struct vm_area_struct *vma)
{
	VM_BUG_ON(!rwsem_is_locked(&vma->vm_mm->mmap_sem));
	if (pmd_trans_huge(*pmd))
		return __pmd_trans_huge_lock(pmd, vma);
	else
		return 0;
}
>>>>>>> refs/remotes/origin/cm-10.0
=======
extern int __pmd_trans_huge_lock(pmd_t *pmd, struct vm_area_struct *vma,
		spinlock_t **ptl);
/* mmap_sem must be held on entry */
static inline int pmd_trans_huge_lock(pmd_t *pmd, struct vm_area_struct *vma,
		spinlock_t **ptl)
{
	VM_BUG_ON(!rwsem_is_locked(&vma->vm_mm->mmap_sem));
	if (pmd_trans_huge(*pmd))
		return __pmd_trans_huge_lock(pmd, vma, ptl);
	else
		return 0;
}
>>>>>>> refs/remotes/origin/master
static inline void vma_adjust_trans_huge(struct vm_area_struct *vma,
					 unsigned long start,
					 unsigned long end,
					 long adjust_next)
{
	if (!vma->anon_vma || vma->vm_ops)
		return;
	__vma_adjust_trans_huge(vma, start, end, adjust_next);
}
static inline int hpage_nr_pages(struct page *page)
{
	if (unlikely(PageTransHuge(page)))
		return HPAGE_PMD_NR;
	return 1;
}
<<<<<<< HEAD
=======
/*
 * compound_trans_head() should be used instead of compound_head(),
 * whenever the "page" passed as parameter could be the tail of a
 * transparent hugepage that could be undergoing a
 * __split_huge_page_refcount(). The page structure layout often
 * changes across releases and it makes extensive use of unions. So if
 * the page structure layout will change in a way that
 * page->first_page gets clobbered by __split_huge_page_refcount, the
 * implementation making use of smp_rmb() will be required.
 *
 * Currently we define compound_trans_head as compound_head, because
 * page->private is in the same union with page->first_page, and
 * page->private isn't clobbered. However this also means we're
 * currently leaving dirt into the page->private field of anonymous
 * pages resulting from a THP split, instead of setting page->private
 * to zero like for every other page that has PG_private not set. But
 * anonymous pages don't use page->private so this is not a problem.
 */
#if 0
/* This will be needed if page->private will be clobbered in split_huge_page */
>>>>>>> refs/remotes/origin/master
static inline struct page *compound_trans_head(struct page *page)
{
	if (PageTail(page)) {
		struct page *head;
		head = page->first_page;
		smp_rmb();
		/*
		 * head may be a dangling pointer.
		 * __split_huge_page_refcount clears PageTail before
		 * overwriting first_page, so if PageTail is still
		 * there it means the head pointer isn't dangling.
		 */
		if (PageTail(page))
			return head;
	}
	return page;
}
<<<<<<< HEAD
#else /* CONFIG_TRANSPARENT_HUGEPAGE */
<<<<<<< HEAD
#define HPAGE_PMD_SHIFT ({ BUG(); 0; })
#define HPAGE_PMD_MASK ({ BUG(); 0; })
#define HPAGE_PMD_SIZE ({ BUG(); 0; })
=======
#define HPAGE_PMD_SHIFT ({ BUILD_BUG(); 0; })
#define HPAGE_PMD_MASK ({ BUILD_BUG(); 0; })
#define HPAGE_PMD_SIZE ({ BUILD_BUG(); 0; })
>>>>>>> refs/remotes/origin/cm-10.0
=======
#else
#define compound_trans_head(page) compound_head(page)
#endif

extern int do_huge_pmd_numa_page(struct mm_struct *mm, struct vm_area_struct *vma,
				unsigned long addr, pmd_t pmd, pmd_t *pmdp);

#else /* CONFIG_TRANSPARENT_HUGEPAGE */
#define HPAGE_PMD_SHIFT ({ BUILD_BUG(); 0; })
#define HPAGE_PMD_MASK ({ BUILD_BUG(); 0; })
#define HPAGE_PMD_SIZE ({ BUILD_BUG(); 0; })
>>>>>>> refs/remotes/origin/master

#define hpage_nr_pages(x) 1

#define transparent_hugepage_enabled(__vma) 0

#define transparent_hugepage_flags 0UL
<<<<<<< HEAD
=======
static inline int
split_huge_page_to_list(struct page *page, struct list_head *list)
{
	return 0;
}
>>>>>>> refs/remotes/origin/master
static inline int split_huge_page(struct page *page)
{
	return 0;
}
<<<<<<< HEAD
#define split_huge_page_pmd(__mm, __pmd)	\
	do { } while (0)
#define wait_split_huge_page(__anon_vma, __pmd)	\
	do { } while (0)
=======
#define split_huge_page_pmd(__vma, __address, __pmd)	\
	do { } while (0)
#define wait_split_huge_page(__anon_vma, __pmd)	\
	do { } while (0)
#define split_huge_page_pmd_mm(__mm, __address, __pmd)	\
	do { } while (0)
>>>>>>> refs/remotes/origin/master
#define compound_trans_head(page) compound_head(page)
static inline int hugepage_madvise(struct vm_area_struct *vma,
				   unsigned long *vm_flags, int advice)
{
	BUG();
	return 0;
}
static inline void vma_adjust_trans_huge(struct vm_area_struct *vma,
					 unsigned long start,
					 unsigned long end,
					 long adjust_next)
{
}
<<<<<<< HEAD
<<<<<<< HEAD
=======
static inline int pmd_trans_huge_lock(pmd_t *pmd,
				      struct vm_area_struct *vma)
{
	return 0;
}
>>>>>>> refs/remotes/origin/cm-10.0
=======
static inline int pmd_trans_huge_lock(pmd_t *pmd, struct vm_area_struct *vma,
		spinlock_t **ptl)
{
	return 0;
}

static inline int do_huge_pmd_numa_page(struct mm_struct *mm, struct vm_area_struct *vma,
					unsigned long addr, pmd_t pmd, pmd_t *pmdp)
{
	return 0;
}

>>>>>>> refs/remotes/origin/master
#endif /* CONFIG_TRANSPARENT_HUGEPAGE */

#endif /* _LINUX_HUGE_MM_H */
