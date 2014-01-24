/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1999, 2000 by Silicon Graphics
 * Copyright (C) 2003 by Ralf Baechle
 */
#include <linux/init.h>
#include <linux/mm.h>
#include <asm/fixmap.h>
#include <asm/pgtable.h>
#include <asm/pgalloc.h>
<<<<<<< HEAD
=======
#include <asm/tlbflush.h>
>>>>>>> refs/remotes/origin/master

void pgd_init(unsigned long page)
{
	unsigned long *p, *end;
	unsigned long entry;

#ifdef __PAGETABLE_PMD_FOLDED
	entry = (unsigned long)invalid_pte_table;
#else
	entry = (unsigned long)invalid_pmd_table;
#endif

<<<<<<< HEAD
 	p = (unsigned long *) page;
	end = p + PTRS_PER_PGD;

	while (p < end) {
=======
	p = (unsigned long *) page;
	end = p + PTRS_PER_PGD;

	do {
>>>>>>> refs/remotes/origin/master
		p[0] = entry;
		p[1] = entry;
		p[2] = entry;
		p[3] = entry;
		p[4] = entry;
<<<<<<< HEAD
		p[5] = entry;
		p[6] = entry;
		p[7] = entry;
		p += 8;
	}
=======
		p += 8;
		p[-3] = entry;
		p[-2] = entry;
		p[-1] = entry;
	} while (p != end);
>>>>>>> refs/remotes/origin/master
}

#ifndef __PAGETABLE_PMD_FOLDED
void pmd_init(unsigned long addr, unsigned long pagetable)
{
	unsigned long *p, *end;

<<<<<<< HEAD
 	p = (unsigned long *) addr;
	end = p + PTRS_PER_PMD;

	while (p < end) {
=======
	p = (unsigned long *) addr;
	end = p + PTRS_PER_PMD;

	do {
>>>>>>> refs/remotes/origin/master
		p[0] = pagetable;
		p[1] = pagetable;
		p[2] = pagetable;
		p[3] = pagetable;
		p[4] = pagetable;
<<<<<<< HEAD
		p[5] = pagetable;
		p[6] = pagetable;
		p[7] = pagetable;
		p += 8;
	}
}
#endif

=======
		p += 8;
		p[-3] = pagetable;
		p[-2] = pagetable;
		p[-1] = pagetable;
	} while (p != end);
}
#endif

#ifdef CONFIG_TRANSPARENT_HUGEPAGE

void pmdp_splitting_flush(struct vm_area_struct *vma,
			 unsigned long address,
			 pmd_t *pmdp)
{
	if (!pmd_trans_splitting(*pmdp)) {
		pmd_t pmd = pmd_mksplitting(*pmdp);
		set_pmd_at(vma->vm_mm, address, pmdp, pmd);
	}
}

#endif

pmd_t mk_pmd(struct page *page, pgprot_t prot)
{
	pmd_t pmd;

	pmd_val(pmd) = (page_to_pfn(page) << _PFN_SHIFT) | pgprot_val(prot);

	return pmd;
}

void set_pmd_at(struct mm_struct *mm, unsigned long addr,
		pmd_t *pmdp, pmd_t pmd)
{
	*pmdp = pmd;
	flush_tlb_all();
}

>>>>>>> refs/remotes/origin/master
void __init pagetable_init(void)
{
	unsigned long vaddr;
	pgd_t *pgd_base;

	/* Initialize the entire pgd.  */
	pgd_init((unsigned long)swapper_pg_dir);
#ifndef __PAGETABLE_PMD_FOLDED
	pmd_init((unsigned long)invalid_pmd_table, (unsigned long)invalid_pte_table);
#endif
	pgd_base = swapper_pg_dir;
	/*
	 * Fixed mappings:
	 */
	vaddr = __fix_to_virt(__end_of_fixed_addresses - 1) & PMD_MASK;
<<<<<<< HEAD
<<<<<<< HEAD
	fixrange_init(vaddr, 0, pgd_base);
=======
	fixrange_init(vaddr, vaddr + FIXADDR_SIZE, pgd_base);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	fixrange_init(vaddr, vaddr + FIXADDR_SIZE, pgd_base);
>>>>>>> refs/remotes/origin/master
}
