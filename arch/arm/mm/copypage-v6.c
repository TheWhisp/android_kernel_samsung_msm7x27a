/*
 *  linux/arch/arm/mm/copypage-v6.c
 *
 *  Copyright (C) 2002 Deep Blue Solutions Ltd, All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/mm.h>
#include <linux/highmem.h>

#include <asm/pgtable.h>
#include <asm/shmparam.h>
#include <asm/tlbflush.h>
#include <asm/cacheflush.h>
#include <asm/cachetype.h>

#include "mm.h"

#if SHMLBA > 16384
#error FIX ME
#endif

<<<<<<< HEAD
<<<<<<< HEAD
#define from_address	(0xffff8000)
#define to_address	(0xffffc000)

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static DEFINE_RAW_SPINLOCK(v6_lock);

/*
 * Copy the user page.  No aliasing to deal with so we can just
 * attack the kernel's existing mapping of these pages.
 */
static void v6_copy_user_highpage_nonaliasing(struct page *to,
	struct page *from, unsigned long vaddr, struct vm_area_struct *vma)
{
	void *kto, *kfrom;

<<<<<<< HEAD
<<<<<<< HEAD
	kfrom = kmap_atomic(from, KM_USER0);
	kto = kmap_atomic(to, KM_USER1);
	copy_page(kto, kfrom);
	__cpuc_flush_dcache_area(kto, PAGE_SIZE);
	kunmap_atomic(kto, KM_USER1);
	kunmap_atomic(kfrom, KM_USER0);
=======
=======
>>>>>>> refs/remotes/origin/master
	kfrom = kmap_atomic(from);
	kto = kmap_atomic(to);
	copy_page(kto, kfrom);
	kunmap_atomic(kto);
	kunmap_atomic(kfrom);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

/*
 * Clear the user page.  No aliasing to deal with so we can just
 * attack the kernel's existing mapping of this page.
 */
static void v6_clear_user_highpage_nonaliasing(struct page *page, unsigned long vaddr)
{
<<<<<<< HEAD
<<<<<<< HEAD
	void *kaddr = kmap_atomic(page, KM_USER0);
	clear_page(kaddr);
	kunmap_atomic(kaddr, KM_USER0);
=======
	void *kaddr = kmap_atomic(page);
	clear_page(kaddr);
	kunmap_atomic(kaddr);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	void *kaddr = kmap_atomic(page);
	clear_page(kaddr);
	kunmap_atomic(kaddr);
>>>>>>> refs/remotes/origin/master
}

/*
 * Discard data in the kernel mapping for the new page.
 * FIXME: needs this MCRR to be supported.
 */
static void discard_old_kernel_data(void *kto)
{
	__asm__("mcrr	p15, 0, %1, %0, c6	@ 0xec401f06"
	   :
	   : "r" (kto),
	     "r" ((unsigned long)kto + PAGE_SIZE - L1_CACHE_BYTES)
	   : "cc");
}

/*
 * Copy the page, taking account of the cache colour.
 */
static void v6_copy_user_highpage_aliasing(struct page *to,
	struct page *from, unsigned long vaddr, struct vm_area_struct *vma)
{
	unsigned int offset = CACHE_COLOUR(vaddr);
	unsigned long kfrom, kto;

	if (!test_and_set_bit(PG_dcache_clean, &from->flags))
		__flush_dcache_page(page_mapping(from), from);

	/* FIXME: not highmem safe */
	discard_old_kernel_data(page_address(to));

	/*
	 * Now copy the page using the same cache colour as the
	 * pages ultimate destination.
	 */
	raw_spin_lock(&v6_lock);

<<<<<<< HEAD
<<<<<<< HEAD
	set_pte_ext(TOP_PTE(from_address) + offset, pfn_pte(page_to_pfn(from), PAGE_KERNEL), 0);
	set_pte_ext(TOP_PTE(to_address) + offset, pfn_pte(page_to_pfn(to), PAGE_KERNEL), 0);

	kfrom = from_address + (offset << PAGE_SHIFT);
	kto   = to_address + (offset << PAGE_SHIFT);

	flush_tlb_kernel_page(kfrom);
	flush_tlb_kernel_page(kto);
=======
=======
>>>>>>> refs/remotes/origin/master
	kfrom = COPYPAGE_V6_FROM + (offset << PAGE_SHIFT);
	kto   = COPYPAGE_V6_TO + (offset << PAGE_SHIFT);

	set_top_pte(kfrom, mk_pte(from, PAGE_KERNEL));
	set_top_pte(kto, mk_pte(to, PAGE_KERNEL));
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	copy_page((void *)kto, (void *)kfrom);

	raw_spin_unlock(&v6_lock);
}

/*
 * Clear the user page.  We need to deal with the aliasing issues,
 * so remap the kernel page into the same cache colour as the user
 * page.
 */
static void v6_clear_user_highpage_aliasing(struct page *page, unsigned long vaddr)
{
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned int offset = CACHE_COLOUR(vaddr);
	unsigned long to = to_address + (offset << PAGE_SHIFT);
=======
	unsigned long to = COPYPAGE_V6_TO + (CACHE_COLOUR(vaddr) << PAGE_SHIFT);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	unsigned long to = COPYPAGE_V6_TO + (CACHE_COLOUR(vaddr) << PAGE_SHIFT);
>>>>>>> refs/remotes/origin/master

	/* FIXME: not highmem safe */
	discard_old_kernel_data(page_address(page));

	/*
	 * Now clear the page using the same cache colour as
	 * the pages ultimate destination.
	 */
	raw_spin_lock(&v6_lock);

<<<<<<< HEAD
<<<<<<< HEAD
	set_pte_ext(TOP_PTE(to_address) + offset, pfn_pte(page_to_pfn(page), PAGE_KERNEL), 0);
	flush_tlb_kernel_page(to);
=======
	set_top_pte(to, mk_pte(page, PAGE_KERNEL));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	set_top_pte(to, mk_pte(page, PAGE_KERNEL));
>>>>>>> refs/remotes/origin/master
	clear_page((void *)to);

	raw_spin_unlock(&v6_lock);
}

struct cpu_user_fns v6_user_fns __initdata = {
	.cpu_clear_user_highpage = v6_clear_user_highpage_nonaliasing,
	.cpu_copy_user_highpage	= v6_copy_user_highpage_nonaliasing,
};

static int __init v6_userpage_init(void)
{
	if (cache_is_vipt_aliasing()) {
		cpu_user.cpu_clear_user_highpage = v6_clear_user_highpage_aliasing;
		cpu_user.cpu_copy_user_highpage = v6_copy_user_highpage_aliasing;
	}

	return 0;
}

core_initcall(v6_userpage_init);
