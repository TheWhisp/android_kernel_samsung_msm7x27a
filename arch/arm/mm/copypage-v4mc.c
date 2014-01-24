/*
 *  linux/arch/arm/lib/copypage-armv4mc.S
 *
 *  Copyright (C) 1995-2005 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This handles the mini data cache, as found on SA11x0 and XScale
 * processors.  When we copy a user page page, we map it in such a way
 * that accesses to this page will not touch the main data cache, but
 * will be cached in the mini data cache.  This prevents us thrashing
 * the main data cache on page faults.
 */
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/highmem.h>

#include <asm/pgtable.h>
#include <asm/tlbflush.h>
#include <asm/cacheflush.h>

#include "mm.h"

<<<<<<< HEAD
<<<<<<< HEAD
/*
 * 0xffff8000 to 0xffffffff is reserved for any ARM architecture
 * specific hacks for copying pages efficiently.
 */
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#define minicache_pgprot __pgprot(L_PTE_PRESENT | L_PTE_YOUNG | \
				  L_PTE_MT_MINICACHE)

static DEFINE_RAW_SPINLOCK(minicache_lock);

/*
 * ARMv4 mini-dcache optimised copy_user_highpage
 *
 * We flush the destination cache lines just before we write the data into the
 * corresponding address.  Since the Dcache is read-allocate, this removes the
 * Dcache aliasing issue.  The writes will be forwarded to the write buffer,
 * and merged as appropriate.
 *
 * Note: We rely on all ARMv4 processors implementing the "invalidate D line"
 * instruction.  If your processor does not supply this, you have to write your
 * own copy_user_highpage that does the right thing.
 */
static void __naked
mc_copy_user_page(void *from, void *to)
{
	asm volatile(
	"stmfd	sp!, {r4, lr}			@ 2\n\
	mov	r4, %2				@ 1\n\
	ldmia	%0!, {r2, r3, ip, lr}		@ 4\n\
1:	mcr	p15, 0, %1, c7, c6, 1		@ 1   invalidate D line\n\
	stmia	%1!, {r2, r3, ip, lr}		@ 4\n\
	ldmia	%0!, {r2, r3, ip, lr}		@ 4+1\n\
	stmia	%1!, {r2, r3, ip, lr}		@ 4\n\
	ldmia	%0!, {r2, r3, ip, lr}		@ 4\n\
	mcr	p15, 0, %1, c7, c6, 1		@ 1   invalidate D line\n\
	stmia	%1!, {r2, r3, ip, lr}		@ 4\n\
	ldmia	%0!, {r2, r3, ip, lr}		@ 4\n\
	subs	r4, r4, #1			@ 1\n\
	stmia	%1!, {r2, r3, ip, lr}		@ 4\n\
	ldmneia	%0!, {r2, r3, ip, lr}		@ 4\n\
	bne	1b				@ 1\n\
	ldmfd	sp!, {r4, pc}			@ 3"
	:
	: "r" (from), "r" (to), "I" (PAGE_SIZE / 64));
}

void v4_mc_copy_user_highpage(struct page *to, struct page *from,
	unsigned long vaddr, struct vm_area_struct *vma)
{
<<<<<<< HEAD
<<<<<<< HEAD
	void *kto = kmap_atomic(to, KM_USER1);
=======
	void *kto = kmap_atomic(to);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	void *kto = kmap_atomic(to);
>>>>>>> refs/remotes/origin/master

	if (!test_and_set_bit(PG_dcache_clean, &from->flags))
		__flush_dcache_page(page_mapping(from), from);

	raw_spin_lock(&minicache_lock);

<<<<<<< HEAD
<<<<<<< HEAD
	set_pte_ext(TOP_PTE(0xffff8000), pfn_pte(page_to_pfn(from), minicache_pgprot), 0);
	flush_tlb_kernel_page(0xffff8000);

	mc_copy_user_page((void *)0xffff8000, kto);

	raw_spin_unlock(&minicache_lock);

	kunmap_atomic(kto, KM_USER1);
=======
=======
>>>>>>> refs/remotes/origin/master
	set_top_pte(COPYPAGE_MINICACHE, mk_pte(from, minicache_pgprot));

	mc_copy_user_page((void *)COPYPAGE_MINICACHE, kto);

	raw_spin_unlock(&minicache_lock);

	kunmap_atomic(kto);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

/*
 * ARMv4 optimised clear_user_page
 */
void v4_mc_clear_user_highpage(struct page *page, unsigned long vaddr)
{
<<<<<<< HEAD
<<<<<<< HEAD
	void *ptr, *kaddr = kmap_atomic(page, KM_USER0);
=======
	void *ptr, *kaddr = kmap_atomic(page);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	void *ptr, *kaddr = kmap_atomic(page);
>>>>>>> refs/remotes/origin/master
	asm volatile("\
	mov	r1, %2				@ 1\n\
	mov	r2, #0				@ 1\n\
	mov	r3, #0				@ 1\n\
	mov	ip, #0				@ 1\n\
	mov	lr, #0				@ 1\n\
1:	mcr	p15, 0, %0, c7, c6, 1		@ 1   invalidate D line\n\
	stmia	%0!, {r2, r3, ip, lr}		@ 4\n\
	stmia	%0!, {r2, r3, ip, lr}		@ 4\n\
	mcr	p15, 0, %0, c7, c6, 1		@ 1   invalidate D line\n\
	stmia	%0!, {r2, r3, ip, lr}		@ 4\n\
	stmia	%0!, {r2, r3, ip, lr}		@ 4\n\
	subs	r1, r1, #1			@ 1\n\
	bne	1b				@ 1"
	: "=r" (ptr)
	: "0" (kaddr), "I" (PAGE_SIZE / 64)
	: "r1", "r2", "r3", "ip", "lr");
<<<<<<< HEAD
<<<<<<< HEAD
	kunmap_atomic(kaddr, KM_USER0);
=======
	kunmap_atomic(kaddr);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	kunmap_atomic(kaddr);
>>>>>>> refs/remotes/origin/master
}

struct cpu_user_fns v4_mc_user_fns __initdata = {
	.cpu_clear_user_highpage = v4_mc_clear_user_highpage,
	.cpu_copy_user_highpage	= v4_mc_copy_user_highpage,
};
