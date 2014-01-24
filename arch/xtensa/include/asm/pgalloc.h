/*
 * include/asm-xtensa/pgalloc.h
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Copyright (C) 2001-2007 Tensilica Inc.
 */

#ifndef _XTENSA_PGALLOC_H
#define _XTENSA_PGALLOC_H

#ifdef __KERNEL__

#include <linux/highmem.h>
#include <linux/slab.h>

/*
 * Allocating and freeing a pmd is trivial: the 1-entry pmd is
 * inside the pgd, so has no extra memory associated with it.
 */

#define pmd_populate_kernel(mm, pmdp, ptep)				     \
	(pmd_val(*(pmdp)) = ((unsigned long)ptep))
#define pmd_populate(mm, pmdp, page)					     \
	(pmd_val(*(pmdp)) = ((unsigned long)page_to_virt(page)))
#define pmd_pgtable(pmd) pmd_page(pmd)

static inline pgd_t*
pgd_alloc(struct mm_struct *mm)
{
	return (pgd_t*) __get_free_pages(GFP_KERNEL | __GFP_ZERO, PGD_ORDER);
}

static inline void pgd_free(struct mm_struct *mm, pgd_t *pgd)
{
	free_page((unsigned long)pgd);
}

<<<<<<< HEAD
/* Use a slab cache for the pte pages (see also sparc64 implementation) */

extern struct kmem_cache *pgtable_cache;

static inline pte_t *pte_alloc_one_kernel(struct mm_struct *mm, 
					 unsigned long address)
{
	return kmem_cache_alloc(pgtable_cache, GFP_KERNEL|__GFP_REPEAT);
=======
static inline pte_t *pte_alloc_one_kernel(struct mm_struct *mm,
					 unsigned long address)
{
	pte_t *ptep;
	int i;

	ptep = (pte_t *)__get_free_page(GFP_KERNEL|__GFP_REPEAT);
	if (!ptep)
		return NULL;
	for (i = 0; i < 1024; i++)
		pte_clear(NULL, 0, ptep + i);
	return ptep;
>>>>>>> refs/remotes/origin/master
}

static inline pgtable_t pte_alloc_one(struct mm_struct *mm,
					unsigned long addr)
{
<<<<<<< HEAD
	struct page *page;

	page = virt_to_page(pte_alloc_one_kernel(mm, addr));
	pgtable_page_ctor(page);
=======
	pte_t *pte;
	struct page *page;

	pte = pte_alloc_one_kernel(mm, addr);
	if (!pte)
		return NULL;
	page = virt_to_page(pte);
	if (!pgtable_page_ctor(page)) {
		__free_page(page);
		return NULL;
	}
>>>>>>> refs/remotes/origin/master
	return page;
}

static inline void pte_free_kernel(struct mm_struct *mm, pte_t *pte)
{
<<<<<<< HEAD
	kmem_cache_free(pgtable_cache, pte);
=======
	free_page((unsigned long)pte);
>>>>>>> refs/remotes/origin/master
}

static inline void pte_free(struct mm_struct *mm, pgtable_t pte)
{
	pgtable_page_dtor(pte);
<<<<<<< HEAD
	kmem_cache_free(pgtable_cache, page_address(pte));
=======
	__free_page(pte);
>>>>>>> refs/remotes/origin/master
}
#define pmd_pgtable(pmd) pmd_page(pmd)

#endif /* __KERNEL__ */
#endif /* _XTENSA_PGALLOC_H */
