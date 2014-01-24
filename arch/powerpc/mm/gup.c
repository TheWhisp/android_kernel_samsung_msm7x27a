/*
 * Lockless get_user_pages_fast for powerpc
 *
 * Copyright (C) 2008 Nick Piggin
 * Copyright (C) 2008 Novell Inc.
 */
#undef DEBUG

#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/hugetlb.h>
#include <linux/vmstat.h>
#include <linux/pagemap.h>
#include <linux/rwsem.h>
#include <asm/pgtable.h>

#ifdef __HAVE_ARCH_PTE_SPECIAL

/*
 * The performance critical leaf functions are made noinline otherwise gcc
 * inlines everything into a single function which results in too much
 * register pressure.
 */
static noinline int gup_pte_range(pmd_t pmd, unsigned long addr,
		unsigned long end, int write, struct page **pages, int *nr)
{
	unsigned long mask, result;
	pte_t *ptep;

	result = _PAGE_PRESENT|_PAGE_USER;
	if (write)
		result |= _PAGE_RW;
	mask = result | _PAGE_SPECIAL;

	ptep = pte_offset_kernel(&pmd, addr);
	do {
<<<<<<< HEAD
		pte_t pte = *ptep;
=======
		pte_t pte = ACCESS_ONCE(*ptep);
>>>>>>> refs/remotes/origin/master
		struct page *page;

		if ((pte_val(pte) & mask) != result)
			return 0;
		VM_BUG_ON(!pfn_valid(pte_pfn(pte)));
		page = pte_page(pte);
		if (!page_cache_get_speculative(page))
			return 0;
		if (unlikely(pte_val(pte) != pte_val(*ptep))) {
			put_page(page);
			return 0;
		}
		pages[*nr] = page;
		(*nr)++;

	} while (ptep++, addr += PAGE_SIZE, addr != end);

	return 1;
}

static int gup_pmd_range(pud_t pud, unsigned long addr, unsigned long end,
		int write, struct page **pages, int *nr)
{
	unsigned long next;
	pmd_t *pmdp;

	pmdp = pmd_offset(&pud, addr);
	do {
<<<<<<< HEAD
		pmd_t pmd = *pmdp;

		next = pmd_addr_end(addr, end);
		if (pmd_none(pmd))
			return 0;
		if (is_hugepd(pmdp)) {
=======
		pmd_t pmd = ACCESS_ONCE(*pmdp);

		next = pmd_addr_end(addr, end);
		/*
		 * If we find a splitting transparent hugepage we
		 * return zero. That will result in taking the slow
		 * path which will call wait_split_huge_page()
		 * if the pmd is still in splitting state
		 */
		if (pmd_none(pmd) || pmd_trans_splitting(pmd))
			return 0;
		if (pmd_huge(pmd) || pmd_large(pmd)) {
			if (!gup_hugepte((pte_t *)pmdp, PMD_SIZE, addr, next,
					 write, pages, nr))
				return 0;
		} else if (is_hugepd(pmdp)) {
>>>>>>> refs/remotes/origin/master
			if (!gup_hugepd((hugepd_t *)pmdp, PMD_SHIFT,
					addr, next, write, pages, nr))
				return 0;
		} else if (!gup_pte_range(pmd, addr, next, write, pages, nr))
			return 0;
	} while (pmdp++, addr = next, addr != end);

	return 1;
}

static int gup_pud_range(pgd_t pgd, unsigned long addr, unsigned long end,
		int write, struct page **pages, int *nr)
{
	unsigned long next;
	pud_t *pudp;

	pudp = pud_offset(&pgd, addr);
	do {
<<<<<<< HEAD
		pud_t pud = *pudp;
=======
		pud_t pud = ACCESS_ONCE(*pudp);
>>>>>>> refs/remotes/origin/master

		next = pud_addr_end(addr, end);
		if (pud_none(pud))
			return 0;
<<<<<<< HEAD
		if (is_hugepd(pudp)) {
=======
		if (pud_huge(pud)) {
			if (!gup_hugepte((pte_t *)pudp, PUD_SIZE, addr, next,
					 write, pages, nr))
				return 0;
		} else if (is_hugepd(pudp)) {
>>>>>>> refs/remotes/origin/master
			if (!gup_hugepd((hugepd_t *)pudp, PUD_SHIFT,
					addr, next, write, pages, nr))
				return 0;
		} else if (!gup_pmd_range(pud, addr, next, write, pages, nr))
			return 0;
	} while (pudp++, addr = next, addr != end);

	return 1;
}

<<<<<<< HEAD
int get_user_pages_fast(unsigned long start, int nr_pages, int write,
			struct page **pages)
=======
int __get_user_pages_fast(unsigned long start, int nr_pages, int write,
			  struct page **pages)
>>>>>>> refs/remotes/origin/master
{
	struct mm_struct *mm = current->mm;
	unsigned long addr, len, end;
	unsigned long next;
<<<<<<< HEAD
=======
	unsigned long flags;
>>>>>>> refs/remotes/origin/master
	pgd_t *pgdp;
	int nr = 0;

	pr_devel("%s(%lx,%x,%s)\n", __func__, start, nr_pages, write ? "write" : "read");

	start &= PAGE_MASK;
	addr = start;
	len = (unsigned long) nr_pages << PAGE_SHIFT;
	end = start + len;

	if (unlikely(!access_ok(write ? VERIFY_WRITE : VERIFY_READ,
					start, len)))
<<<<<<< HEAD
		goto slow_irqon;
=======
		return 0;
>>>>>>> refs/remotes/origin/master

	pr_devel("  aligned: %lx .. %lx\n", start, end);

	/*
	 * XXX: batch / limit 'nr', to avoid large irq off latency
	 * needs some instrumenting to determine the common sizes used by
	 * important workloads (eg. DB2), and whether limiting the batch size
	 * will decrease performance.
	 *
	 * It seems like we're in the clear for the moment. Direct-IO is
	 * the main guy that batches up lots of get_user_pages, and even
	 * they are limited to 64-at-a-time which is not so many.
	 */
	/*
	 * This doesn't prevent pagetable teardown, but does prevent
	 * the pagetables from being freed on powerpc.
	 *
	 * So long as we atomically load page table pointers versus teardown,
	 * we can follow the address down to the the page and take a ref on it.
	 */
<<<<<<< HEAD
	local_irq_disable();

	pgdp = pgd_offset(mm, addr);
	do {
		pgd_t pgd = *pgdp;
=======
	local_irq_save(flags);

	pgdp = pgd_offset(mm, addr);
	do {
		pgd_t pgd = ACCESS_ONCE(*pgdp);
>>>>>>> refs/remotes/origin/master

		pr_devel("  %016lx: normal pgd %p\n", addr,
			 (void *)pgd_val(pgd));
		next = pgd_addr_end(addr, end);
		if (pgd_none(pgd))
<<<<<<< HEAD
			goto slow;
		if (is_hugepd(pgdp)) {
			if (!gup_hugepd((hugepd_t *)pgdp, PGDIR_SHIFT,
					addr, next, write, pages, &nr))
				goto slow;
		} else if (!gup_pud_range(pgd, addr, next, write, pages, &nr))
			goto slow;
	} while (pgdp++, addr = next, addr != end);

	local_irq_enable();

	VM_BUG_ON(nr != (end - start) >> PAGE_SHIFT);
	return nr;

	{
		int ret;

slow:
		local_irq_enable();
slow_irqon:
=======
			break;
		if (pgd_huge(pgd)) {
			if (!gup_hugepte((pte_t *)pgdp, PGDIR_SIZE, addr, next,
					 write, pages, &nr))
				break;
		} else if (is_hugepd(pgdp)) {
			if (!gup_hugepd((hugepd_t *)pgdp, PGDIR_SHIFT,
					addr, next, write, pages, &nr))
				break;
		} else if (!gup_pud_range(pgd, addr, next, write, pages, &nr))
			break;
	} while (pgdp++, addr = next, addr != end);

	local_irq_restore(flags);

	return nr;
}

int get_user_pages_fast(unsigned long start, int nr_pages, int write,
			struct page **pages)
{
	struct mm_struct *mm = current->mm;
	int nr, ret;

	start &= PAGE_MASK;
	nr = __get_user_pages_fast(start, nr_pages, write, pages);
	ret = nr;

	if (nr < nr_pages) {
>>>>>>> refs/remotes/origin/master
		pr_devel("  slow path ! nr = %d\n", nr);

		/* Try to get the remaining pages with get_user_pages */
		start += nr << PAGE_SHIFT;
		pages += nr;

		down_read(&mm->mmap_sem);
		ret = get_user_pages(current, mm, start,
<<<<<<< HEAD
			(end - start) >> PAGE_SHIFT, write, 0, pages, NULL);
=======
				     nr_pages - nr, write, 0, pages, NULL);
>>>>>>> refs/remotes/origin/master
		up_read(&mm->mmap_sem);

		/* Have to be a bit careful with return values */
		if (nr > 0) {
			if (ret < 0)
				ret = nr;
			else
				ret += nr;
		}
<<<<<<< HEAD

		return ret;
	}
=======
	}

	return ret;
>>>>>>> refs/remotes/origin/master
}

#endif /* __HAVE_ARCH_PTE_SPECIAL */
