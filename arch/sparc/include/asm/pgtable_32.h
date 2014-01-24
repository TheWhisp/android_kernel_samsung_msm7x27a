#ifndef _SPARC_PGTABLE_H
#define _SPARC_PGTABLE_H

/*  asm/pgtable.h:  Defines and functions used to work
 *                        with Sparc page tables.
 *
 *  Copyright (C) 1995 David S. Miller (davem@caip.rutgers.edu)
 *  Copyright (C) 1998 Jakub Jelinek (jj@sunsite.mff.cuni.cz)
 */

#include <linux/const.h>

#ifndef __ASSEMBLY__
#include <asm-generic/4level-fixup.h>

#include <linux/spinlock.h>
#include <linux/swap.h>
#include <asm/types.h>
<<<<<<< HEAD
#include <asm/pgtsun4c.h>
#include <asm/pgtsrmmu.h>
#include <asm/vac-ops.h>
#include <asm/oplib.h>
#include <asm/btfixup.h>
<<<<<<< HEAD
#include <asm/system.h>
=======
#include <asm/cpu_type.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <asm/pgtsrmmu.h>
#include <asm/vaddrs.h>
#include <asm/oplib.h>
#include <asm/cpu_type.h>
>>>>>>> refs/remotes/origin/master


struct vm_area_struct;
struct page;

extern void load_mmu(void);
extern unsigned long calc_highpages(void);

<<<<<<< HEAD
BTFIXUPDEF_SIMM13(pgdir_shift)
BTFIXUPDEF_SETHI(pgdir_size)
BTFIXUPDEF_SETHI(pgdir_mask)

BTFIXUPDEF_SIMM13(ptrs_per_pmd)
BTFIXUPDEF_SIMM13(ptrs_per_pgd)
BTFIXUPDEF_SIMM13(user_ptrs_per_pgd)

=======
>>>>>>> refs/remotes/origin/master
#define pte_ERROR(e)   __builtin_trap()
#define pmd_ERROR(e)   __builtin_trap()
#define pgd_ERROR(e)   __builtin_trap()

<<<<<<< HEAD
BTFIXUPDEF_INT(page_none)
BTFIXUPDEF_INT(page_copy)
BTFIXUPDEF_INT(page_readonly)
BTFIXUPDEF_INT(page_kernel)

#define PMD_SHIFT		SUN4C_PMD_SHIFT
#define PMD_SIZE        	(1UL << PMD_SHIFT)
#define PMD_MASK        	(~(PMD_SIZE-1))
#define PMD_ALIGN(__addr) 	(((__addr) + ~PMD_MASK) & PMD_MASK)
#define PGDIR_SHIFT     	BTFIXUP_SIMM13(pgdir_shift)
#define PGDIR_SIZE      	BTFIXUP_SETHI(pgdir_size)
#define PGDIR_MASK      	BTFIXUP_SETHI(pgdir_mask)
#define PTRS_PER_PTE    	1024
#define PTRS_PER_PMD    	BTFIXUP_SIMM13(ptrs_per_pmd)
#define PTRS_PER_PGD    	BTFIXUP_SIMM13(ptrs_per_pgd)
#define USER_PTRS_PER_PGD	BTFIXUP_SIMM13(user_ptrs_per_pgd)
#define FIRST_USER_ADDRESS	0
#define PTE_SIZE		(PTRS_PER_PTE*4)

#define PAGE_NONE      __pgprot(BTFIXUP_INT(page_none))
extern pgprot_t PAGE_SHARED;
#define PAGE_COPY      __pgprot(BTFIXUP_INT(page_copy))
#define PAGE_READONLY  __pgprot(BTFIXUP_INT(page_readonly))

extern unsigned long page_kernel;

#ifdef MODULE
#define PAGE_KERNEL	page_kernel
#else
#define PAGE_KERNEL    __pgprot(BTFIXUP_INT(page_kernel))
#endif

/* Top-level page directory */
extern pgd_t swapper_pg_dir[1024];

extern void paging_init(void);

/* Page table for 0-4MB for everybody, on the Sparc this
 * holds the same as on the i386.
 */
extern pte_t pg0[1024];
extern pte_t pg1[1024];
extern pte_t pg2[1024];
extern pte_t pg3[1024];

extern unsigned long ptr_in_current_pgd;

/* Here is a trick, since mmap.c need the initializer elements for
 * protection_map[] to be constant at compile time, I set the following
 * to all zeros.  I set it to the real values after I link in the
 * appropriate MMU page table routines at boot time.
 */
#define __P000  __pgprot(0)
#define __P001  __pgprot(0)
#define __P010  __pgprot(0)
#define __P011  __pgprot(0)
#define __P100  __pgprot(0)
#define __P101  __pgprot(0)
#define __P110  __pgprot(0)
#define __P111  __pgprot(0)

#define __S000	__pgprot(0)
#define __S001	__pgprot(0)
#define __S010	__pgprot(0)
#define __S011	__pgprot(0)
#define __S100	__pgprot(0)
#define __S101	__pgprot(0)
#define __S110	__pgprot(0)
#define __S111	__pgprot(0)

extern int num_contexts;
=======
#define PMD_SHIFT		22
#define PMD_SIZE        	(1UL << PMD_SHIFT)
#define PMD_MASK        	(~(PMD_SIZE-1))
#define PMD_ALIGN(__addr) 	(((__addr) + ~PMD_MASK) & PMD_MASK)
#define PGDIR_SHIFT     	SRMMU_PGDIR_SHIFT
#define PGDIR_SIZE      	SRMMU_PGDIR_SIZE
#define PGDIR_MASK      	SRMMU_PGDIR_MASK
#define PTRS_PER_PTE    	1024
#define PTRS_PER_PMD    	SRMMU_PTRS_PER_PMD
#define PTRS_PER_PGD    	SRMMU_PTRS_PER_PGD
#define USER_PTRS_PER_PGD	PAGE_OFFSET / SRMMU_PGDIR_SIZE
#define FIRST_USER_ADDRESS	0
#define PTE_SIZE		(PTRS_PER_PTE*4)

#define PAGE_NONE	SRMMU_PAGE_NONE
#define PAGE_SHARED	SRMMU_PAGE_SHARED
#define PAGE_COPY	SRMMU_PAGE_COPY
#define PAGE_READONLY	SRMMU_PAGE_RDONLY
#define PAGE_KERNEL	SRMMU_PAGE_KERNEL

/* Top-level page directory - dummy used by init-mm.
 * srmmu.c will assign the real one (which is dynamically sized) */
#define swapper_pg_dir NULL

extern void paging_init(void);

extern unsigned long ptr_in_current_pgd;

/*         xwr */
#define __P000  PAGE_NONE
#define __P001  PAGE_READONLY
#define __P010  PAGE_COPY
#define __P011  PAGE_COPY
#define __P100  PAGE_READONLY
#define __P101  PAGE_READONLY
#define __P110  PAGE_COPY
#define __P111  PAGE_COPY

#define __S000	PAGE_NONE
#define __S001	PAGE_READONLY
#define __S010	PAGE_SHARED
#define __S011	PAGE_SHARED
#define __S100	PAGE_READONLY
#define __S101	PAGE_READONLY
#define __S110	PAGE_SHARED
#define __S111	PAGE_SHARED
>>>>>>> refs/remotes/origin/master

/* First physical page can be anywhere, the following is needed so that
 * va-->pa and vice versa conversions work properly without performance
 * hit for all __pa()/__va() operations.
 */
extern unsigned long phys_base;
extern unsigned long pfn_base;

/*
<<<<<<< HEAD
 * BAD_PAGETABLE is used when we need a bogus page-table, while
 * BAD_PAGE is used for a bogus page.
 *
 * ZERO_PAGE is a global shared page that is always zero: used
 * for zero-mapped memory areas etc..
 */
extern pte_t * __bad_pagetable(void);
extern pte_t __bad_page(void);
extern unsigned long empty_zero_page;

#define BAD_PAGETABLE __bad_pagetable()
#define BAD_PAGE __bad_page()
#define ZERO_PAGE(vaddr) (virt_to_page(&empty_zero_page))

/*
 */
BTFIXUPDEF_CALL_CONST(struct page *, pmd_page, pmd_t)
BTFIXUPDEF_CALL_CONST(unsigned long, pgd_page_vaddr, pgd_t)

#define pmd_page(pmd) BTFIXUP_CALL(pmd_page)(pmd)
#define pgd_page_vaddr(pgd) BTFIXUP_CALL(pgd_page_vaddr)(pgd)

BTFIXUPDEF_CALL_CONST(int, pte_present, pte_t)
BTFIXUPDEF_CALL(void, pte_clear, pte_t *)
=======
 * ZERO_PAGE is a global shared page that is always zero: used
 * for zero-mapped memory areas etc..
 */
extern unsigned long empty_zero_page;

#define ZERO_PAGE(vaddr) (virt_to_page(&empty_zero_page))

/*
 * In general all page table modifications should use the V8 atomic
 * swap instruction.  This insures the mmu and the cpu are in sync
 * with respect to ref/mod bits in the page tables.
 */
static inline unsigned long srmmu_swap(unsigned long *addr, unsigned long value)
{
	__asm__ __volatile__("swap [%2], %0" : "=&r" (value) : "0" (value), "r" (addr));
	return value;
}

/* Certain architectures need to do special things when pte's
 * within a page table are directly modified.  Thus, the following
 * hook is made available.
 */

static inline void set_pte(pte_t *ptep, pte_t pteval)
{
	srmmu_swap((unsigned long *)ptep, pte_val(pteval));
}

#define set_pte_at(mm,addr,ptep,pteval) set_pte(ptep,pteval)

static inline int srmmu_device_memory(unsigned long x)
{
	return ((x & 0xF0000000) != 0);
}

static inline struct page *pmd_page(pmd_t pmd)
{
	if (srmmu_device_memory(pmd_val(pmd)))
		BUG();
	return pfn_to_page((pmd_val(pmd) & SRMMU_PTD_PMASK) >> (PAGE_SHIFT-4));
}

static inline unsigned long pgd_page_vaddr(pgd_t pgd)
{
	if (srmmu_device_memory(pgd_val(pgd))) {
		return ~0;
	} else {
		unsigned long v = pgd_val(pgd) & SRMMU_PTD_PMASK;
		return (unsigned long)__nocache_va(v << 4);
	}
}

static inline int pte_present(pte_t pte)
{
	return ((pte_val(pte) & SRMMU_ET_MASK) == SRMMU_ET_PTE);
}
>>>>>>> refs/remotes/origin/master

static inline int pte_none(pte_t pte)
{
	return !pte_val(pte);
}

<<<<<<< HEAD
#define pte_present(pte) BTFIXUP_CALL(pte_present)(pte)
#define pte_clear(mm,addr,pte) BTFIXUP_CALL(pte_clear)(pte)

BTFIXUPDEF_CALL_CONST(int, pmd_bad, pmd_t)
BTFIXUPDEF_CALL_CONST(int, pmd_present, pmd_t)
BTFIXUPDEF_CALL(void, pmd_clear, pmd_t *)
=======
static inline void __pte_clear(pte_t *ptep)
{
	set_pte(ptep, __pte(0));
}

static inline void pte_clear(struct mm_struct *mm, unsigned long addr, pte_t *ptep)
{
	__pte_clear(ptep);
}

static inline int pmd_bad(pmd_t pmd)
{
	return (pmd_val(pmd) & SRMMU_ET_MASK) != SRMMU_ET_PTD;
}

static inline int pmd_present(pmd_t pmd)
{
	return ((pmd_val(pmd) & SRMMU_ET_MASK) == SRMMU_ET_PTD);
}
>>>>>>> refs/remotes/origin/master

static inline int pmd_none(pmd_t pmd)
{
	return !pmd_val(pmd);
}

<<<<<<< HEAD
#define pmd_bad(pmd) BTFIXUP_CALL(pmd_bad)(pmd)
#define pmd_present(pmd) BTFIXUP_CALL(pmd_present)(pmd)
#define pmd_clear(pmd) BTFIXUP_CALL(pmd_clear)(pmd)

BTFIXUPDEF_CALL_CONST(int, pgd_none, pgd_t)
BTFIXUPDEF_CALL_CONST(int, pgd_bad, pgd_t)
BTFIXUPDEF_CALL_CONST(int, pgd_present, pgd_t)
BTFIXUPDEF_CALL(void, pgd_clear, pgd_t *)

#define pgd_none(pgd) BTFIXUP_CALL(pgd_none)(pgd)
#define pgd_bad(pgd) BTFIXUP_CALL(pgd_bad)(pgd)
#define pgd_present(pgd) BTFIXUP_CALL(pgd_present)(pgd)
#define pgd_clear(pgd) BTFIXUP_CALL(pgd_clear)(pgd)
=======
static inline void pmd_clear(pmd_t *pmdp)
{
	int i;
	for (i = 0; i < PTRS_PER_PTE/SRMMU_REAL_PTRS_PER_PTE; i++)
		set_pte((pte_t *)&pmdp->pmdv[i], __pte(0));
}

static inline int pgd_none(pgd_t pgd)          
{
	return !(pgd_val(pgd) & 0xFFFFFFF);
}

static inline int pgd_bad(pgd_t pgd)
{
	return (pgd_val(pgd) & SRMMU_ET_MASK) != SRMMU_ET_PTD;
}

static inline int pgd_present(pgd_t pgd)
{
	return ((pgd_val(pgd) & SRMMU_ET_MASK) == SRMMU_ET_PTD);
}

static inline void pgd_clear(pgd_t *pgdp)
{
	set_pte((pte_t *)pgdp, __pte(0));
}
>>>>>>> refs/remotes/origin/master

/*
 * The following only work if pte_present() is true.
 * Undefined behaviour if not..
 */
<<<<<<< HEAD
BTFIXUPDEF_HALF(pte_writei)
BTFIXUPDEF_HALF(pte_dirtyi)
BTFIXUPDEF_HALF(pte_youngi)

static int pte_write(pte_t pte) __attribute_const__;
static inline int pte_write(pte_t pte)
{
	return pte_val(pte) & BTFIXUP_HALF(pte_writei);
}

static int pte_dirty(pte_t pte) __attribute_const__;
static inline int pte_dirty(pte_t pte)
{
	return pte_val(pte) & BTFIXUP_HALF(pte_dirtyi);
}

static int pte_young(pte_t pte) __attribute_const__;
static inline int pte_young(pte_t pte)
{
	return pte_val(pte) & BTFIXUP_HALF(pte_youngi);
=======
static inline int pte_write(pte_t pte)
{
	return pte_val(pte) & SRMMU_WRITE;
}

static inline int pte_dirty(pte_t pte)
{
	return pte_val(pte) & SRMMU_DIRTY;
}

static inline int pte_young(pte_t pte)
{
	return pte_val(pte) & SRMMU_REF;
>>>>>>> refs/remotes/origin/master
}

/*
 * The following only work if pte_present() is not true.
 */
<<<<<<< HEAD
BTFIXUPDEF_HALF(pte_filei)

static int pte_file(pte_t pte) __attribute_const__;
static inline int pte_file(pte_t pte)
{
	return pte_val(pte) & BTFIXUP_HALF(pte_filei);
=======
static inline int pte_file(pte_t pte)
{
	return pte_val(pte) & SRMMU_FILE;
>>>>>>> refs/remotes/origin/master
}

static inline int pte_special(pte_t pte)
{
	return 0;
}

<<<<<<< HEAD
/*
 */
BTFIXUPDEF_HALF(pte_wrprotecti)
BTFIXUPDEF_HALF(pte_mkcleani)
BTFIXUPDEF_HALF(pte_mkoldi)

static pte_t pte_wrprotect(pte_t pte) __attribute_const__;
static inline pte_t pte_wrprotect(pte_t pte)
{
	return __pte(pte_val(pte) & ~BTFIXUP_HALF(pte_wrprotecti));
}

static pte_t pte_mkclean(pte_t pte) __attribute_const__;
static inline pte_t pte_mkclean(pte_t pte)
{
	return __pte(pte_val(pte) & ~BTFIXUP_HALF(pte_mkcleani));
}

static pte_t pte_mkold(pte_t pte) __attribute_const__;
static inline pte_t pte_mkold(pte_t pte)
{
	return __pte(pte_val(pte) & ~BTFIXUP_HALF(pte_mkoldi));
}

BTFIXUPDEF_CALL_CONST(pte_t, pte_mkwrite, pte_t)
BTFIXUPDEF_CALL_CONST(pte_t, pte_mkdirty, pte_t)
BTFIXUPDEF_CALL_CONST(pte_t, pte_mkyoung, pte_t)

#define pte_mkwrite(pte) BTFIXUP_CALL(pte_mkwrite)(pte)
#define pte_mkdirty(pte) BTFIXUP_CALL(pte_mkdirty)(pte)
#define pte_mkyoung(pte) BTFIXUP_CALL(pte_mkyoung)(pte)
=======
static inline pte_t pte_wrprotect(pte_t pte)
{
	return __pte(pte_val(pte) & ~SRMMU_WRITE);
}

static inline pte_t pte_mkclean(pte_t pte)
{
	return __pte(pte_val(pte) & ~SRMMU_DIRTY);
}

static inline pte_t pte_mkold(pte_t pte)
{
	return __pte(pte_val(pte) & ~SRMMU_REF);
}

static inline pte_t pte_mkwrite(pte_t pte)
{
	return __pte(pte_val(pte) | SRMMU_WRITE);
}

static inline pte_t pte_mkdirty(pte_t pte)
{
	return __pte(pte_val(pte) | SRMMU_DIRTY);
}

static inline pte_t pte_mkyoung(pte_t pte)
{
	return __pte(pte_val(pte) | SRMMU_REF);
}
>>>>>>> refs/remotes/origin/master

#define pte_mkspecial(pte)    (pte)

#define pfn_pte(pfn, prot)		mk_pte(pfn_to_page(pfn), prot)

<<<<<<< HEAD
BTFIXUPDEF_CALL(unsigned long,	 pte_pfn, pte_t)
#define pte_pfn(pte) BTFIXUP_CALL(pte_pfn)(pte)
=======
static inline unsigned long pte_pfn(pte_t pte)
{
	if (srmmu_device_memory(pte_val(pte))) {
		/* Just return something that will cause
		 * pfn_valid() to return false.  This makes
		 * copy_one_pte() to just directly copy to
		 * PTE over.
		 */
		return ~0UL;
	}
	return (pte_val(pte) & SRMMU_PTE_PMASK) >> (PAGE_SHIFT-4);
}

>>>>>>> refs/remotes/origin/master
#define pte_page(pte)	pfn_to_page(pte_pfn(pte))

/*
 * Conversion functions: convert a page and protection to a page entry,
 * and a page entry and page directory to the page they refer to.
 */
<<<<<<< HEAD
BTFIXUPDEF_CALL_CONST(pte_t, mk_pte, struct page *, pgprot_t)

BTFIXUPDEF_CALL_CONST(pte_t, mk_pte_phys, unsigned long, pgprot_t)
BTFIXUPDEF_CALL_CONST(pte_t, mk_pte_io, unsigned long, pgprot_t, int)
BTFIXUPDEF_CALL_CONST(pgprot_t, pgprot_noncached, pgprot_t)

#define mk_pte(page,pgprot) BTFIXUP_CALL(mk_pte)(page,pgprot)
#define mk_pte_phys(page,pgprot) BTFIXUP_CALL(mk_pte_phys)(page,pgprot)
#define mk_pte_io(page,pgprot,space) BTFIXUP_CALL(mk_pte_io)(page,pgprot,space)

#define pgprot_noncached(pgprot) BTFIXUP_CALL(pgprot_noncached)(pgprot)

BTFIXUPDEF_INT(pte_modify_mask)
=======
static inline pte_t mk_pte(struct page *page, pgprot_t pgprot)
{
	return __pte((page_to_pfn(page) << (PAGE_SHIFT-4)) | pgprot_val(pgprot));
}

static inline pte_t mk_pte_phys(unsigned long page, pgprot_t pgprot)
{
	return __pte(((page) >> 4) | pgprot_val(pgprot));
}

static inline pte_t mk_pte_io(unsigned long page, pgprot_t pgprot, int space)
{
	return __pte(((page) >> 4) | (space << 28) | pgprot_val(pgprot));
}

#define pgprot_noncached pgprot_noncached
static inline pgprot_t pgprot_noncached(pgprot_t prot)
{
	prot &= ~__pgprot(SRMMU_CACHE);
	return prot;
}
>>>>>>> refs/remotes/origin/master

static pte_t pte_modify(pte_t pte, pgprot_t newprot) __attribute_const__;
static inline pte_t pte_modify(pte_t pte, pgprot_t newprot)
{
<<<<<<< HEAD
	return __pte((pte_val(pte) & BTFIXUP_INT(pte_modify_mask)) |
=======
	return __pte((pte_val(pte) & SRMMU_CHG_MASK) |
>>>>>>> refs/remotes/origin/master
		pgprot_val(newprot));
}

#define pgd_index(address) ((address) >> PGDIR_SHIFT)

/* to find an entry in a page-table-directory */
#define pgd_offset(mm, address) ((mm)->pgd + pgd_index(address))

/* to find an entry in a kernel page-table-directory */
#define pgd_offset_k(address) pgd_offset(&init_mm, address)

/* Find an entry in the second-level page table.. */
<<<<<<< HEAD
BTFIXUPDEF_CALL(pmd_t *, pmd_offset, pgd_t *, unsigned long)
#define pmd_offset(dir,addr) BTFIXUP_CALL(pmd_offset)(dir,addr)

/* Find an entry in the third-level page table.. */
BTFIXUPDEF_CALL(pte_t *, pte_offset_kernel, pmd_t *, unsigned long)
#define pte_offset_kernel(dir,addr) BTFIXUP_CALL(pte_offset_kernel)(dir,addr)

/*
 * This shortcut works on sun4m (and sun4d) because the nocache area is static,
 * and sun4c is guaranteed to have no highmem anyway.
=======
static inline pmd_t *pmd_offset(pgd_t * dir, unsigned long address)
{
	return (pmd_t *) pgd_page_vaddr(*dir) +
		((address >> PMD_SHIFT) & (PTRS_PER_PMD - 1));
}

/* Find an entry in the third-level page table.. */
pte_t *pte_offset_kernel(pmd_t * dir, unsigned long address);

/*
 * This shortcut works on sun4m (and sun4d) because the nocache area is static.
>>>>>>> refs/remotes/origin/master
 */
#define pte_offset_map(d, a)		pte_offset_kernel(d,a)
#define pte_unmap(pte)		do{}while(0)

<<<<<<< HEAD
/* Certain architectures need to do special things when pte's
 * within a page table are directly modified.  Thus, the following
 * hook is made available.
 */

BTFIXUPDEF_CALL(void, set_pte, pte_t *, pte_t)

#define set_pte(ptep,pteval) BTFIXUP_CALL(set_pte)(ptep,pteval)
#define set_pte_at(mm,addr,ptep,pteval) set_pte(ptep,pteval)

struct seq_file;
BTFIXUPDEF_CALL(void, mmu_info, struct seq_file *)

#define mmu_info(p) BTFIXUP_CALL(mmu_info)(p)
=======
struct seq_file;
void mmu_info(struct seq_file *m);
>>>>>>> refs/remotes/origin/master

/* Fault handler stuff... */
#define FAULT_CODE_PROT     0x1
#define FAULT_CODE_WRITE    0x2
#define FAULT_CODE_USER     0x4

<<<<<<< HEAD
BTFIXUPDEF_CALL(void, update_mmu_cache, struct vm_area_struct *, unsigned long, pte_t *)

#define update_mmu_cache(vma,addr,ptep) BTFIXUP_CALL(update_mmu_cache)(vma,addr,ptep)

BTFIXUPDEF_CALL(void, sparc_mapiorange, unsigned int, unsigned long,
    unsigned long, unsigned int)
BTFIXUPDEF_CALL(void, sparc_unmapiorange, unsigned long, unsigned int)
#define sparc_mapiorange(bus,pa,va,len) BTFIXUP_CALL(sparc_mapiorange)(bus,pa,va,len)
#define sparc_unmapiorange(va,len) BTFIXUP_CALL(sparc_unmapiorange)(va,len)

extern int invalid_segment;

/* Encode and de-code a swap entry */
BTFIXUPDEF_CALL(unsigned long, __swp_type, swp_entry_t)
BTFIXUPDEF_CALL(unsigned long, __swp_offset, swp_entry_t)
BTFIXUPDEF_CALL(swp_entry_t, __swp_entry, unsigned long, unsigned long)

#define __swp_type(__x)			BTFIXUP_CALL(__swp_type)(__x)
#define __swp_offset(__x)		BTFIXUP_CALL(__swp_offset)(__x)
#define __swp_entry(__type,__off)	BTFIXUP_CALL(__swp_entry)(__type,__off)
=======
#define update_mmu_cache(vma, address, ptep) do { } while (0)

void srmmu_mapiorange(unsigned int bus, unsigned long xpa,
                      unsigned long xva, unsigned int len);
void srmmu_unmapiorange(unsigned long virt_addr, unsigned int len);

/* Encode and de-code a swap entry */
static inline unsigned long __swp_type(swp_entry_t entry)
{
	return (entry.val >> SRMMU_SWP_TYPE_SHIFT) & SRMMU_SWP_TYPE_MASK;
}

static inline unsigned long __swp_offset(swp_entry_t entry)
{
	return (entry.val >> SRMMU_SWP_OFF_SHIFT) & SRMMU_SWP_OFF_MASK;
}

static inline swp_entry_t __swp_entry(unsigned long type, unsigned long offset)
{
	return (swp_entry_t) {
		(type & SRMMU_SWP_TYPE_MASK) << SRMMU_SWP_TYPE_SHIFT
		| (offset & SRMMU_SWP_OFF_MASK) << SRMMU_SWP_OFF_SHIFT };
}
>>>>>>> refs/remotes/origin/master

#define __pte_to_swp_entry(pte)		((swp_entry_t) { pte_val(pte) })
#define __swp_entry_to_pte(x)		((pte_t) { (x).val })

/* file-offset-in-pte helpers */
<<<<<<< HEAD
BTFIXUPDEF_CALL(unsigned long, pte_to_pgoff, pte_t pte);
BTFIXUPDEF_CALL(pte_t, pgoff_to_pte, unsigned long pgoff);

#define pte_to_pgoff(pte) BTFIXUP_CALL(pte_to_pgoff)(pte)
#define pgoff_to_pte(off) BTFIXUP_CALL(pgoff_to_pte)(off)

/*
 * This is made a constant because mm/fremap.c required a constant.
 * Note that layout of these bits is different between sun4c.c and srmmu.c.
 */
#define PTE_FILE_MAX_BITS 24

/*
 */
struct ctx_list {
	struct ctx_list *next;
	struct ctx_list *prev;
	unsigned int ctx_number;
	struct mm_struct *ctx_mm;
};

extern struct ctx_list *ctx_list_pool;  /* Dynamically allocated */
extern struct ctx_list ctx_free;        /* Head of free list */
extern struct ctx_list ctx_used;        /* Head of used contexts list */

#define NO_CONTEXT     -1

static inline void remove_from_ctx_list(struct ctx_list *entry)
{
	entry->next->prev = entry->prev;
	entry->prev->next = entry->next;
}

static inline void add_to_ctx_list(struct ctx_list *head, struct ctx_list *entry)
{
	entry->next = head;
	(entry->prev = head->prev)->next = entry;
	head->prev = entry;
}
#define add_to_free_ctxlist(entry) add_to_ctx_list(&ctx_free, entry)
#define add_to_used_ctxlist(entry) add_to_ctx_list(&ctx_used, entry)

=======
static inline unsigned long pte_to_pgoff(pte_t pte)
{
	return pte_val(pte) >> SRMMU_PTE_FILE_SHIFT;
}

static inline pte_t pgoff_to_pte(unsigned long pgoff)
{
	return __pte((pgoff << SRMMU_PTE_FILE_SHIFT) | SRMMU_FILE);
}

/*
 * This is made a constant because mm/fremap.c required a constant.
 */
#define PTE_FILE_MAX_BITS 24

>>>>>>> refs/remotes/origin/master
static inline unsigned long
__get_phys (unsigned long addr)
{
	switch (sparc_cpu_model){
<<<<<<< HEAD
	case sun4:
	case sun4c:
		return sun4c_get_pte (addr) << PAGE_SHIFT;
=======
>>>>>>> refs/remotes/origin/master
	case sun4m:
	case sun4d:
		return ((srmmu_get_pte (addr) & 0xffffff00) << 4);
	default:
		return 0;
	}
}

static inline int
__get_iospace (unsigned long addr)
{
	switch (sparc_cpu_model){
<<<<<<< HEAD
	case sun4:
	case sun4c:
		return -1; /* Don't check iospace on sun4c */
=======
>>>>>>> refs/remotes/origin/master
	case sun4m:
	case sun4d:
		return (srmmu_get_pte (addr) >> 28);
	default:
		return -1;
	}
}

extern unsigned long *sparc_valid_addr_bitmap;

/* Needs to be defined here and not in linux/mm.h, as it is arch dependent */
#define kern_addr_valid(addr) \
	(test_bit(__pa((unsigned long)(addr))>>20, sparc_valid_addr_bitmap))

/*
 * For sparc32&64, the pfn in io_remap_pfn_range() carries <iospace> in
 * its high 4 bits.  These macros/functions put it there or get it from there.
 */
#define MK_IOSPACE_PFN(space, pfn)	(pfn | (space << (BITS_PER_LONG - 4)))
#define GET_IOSPACE(pfn)		(pfn >> (BITS_PER_LONG - 4))
#define GET_PFN(pfn)			(pfn & 0x0fffffffUL)

extern int remap_pfn_range(struct vm_area_struct *, unsigned long, unsigned long,
			   unsigned long, pgprot_t);

static inline int io_remap_pfn_range(struct vm_area_struct *vma,
				     unsigned long from, unsigned long pfn,
				     unsigned long size, pgprot_t prot)
{
	unsigned long long offset, space, phys_base;

	offset = ((unsigned long long) GET_PFN(pfn)) << PAGE_SHIFT;
	space = GET_IOSPACE(pfn);
	phys_base = offset | (space << 32ULL);

	return remap_pfn_range(vma, from, phys_base >> PAGE_SHIFT, size, prot);
}
<<<<<<< HEAD
=======
#define io_remap_pfn_range io_remap_pfn_range 
>>>>>>> refs/remotes/origin/master

#define __HAVE_ARCH_PTEP_SET_ACCESS_FLAGS
#define ptep_set_access_flags(__vma, __address, __ptep, __entry, __dirty) \
({									  \
	int __changed = !pte_same(*(__ptep), __entry);			  \
	if (__changed) {						  \
		set_pte_at((__vma)->vm_mm, (__address), __ptep, __entry); \
		flush_tlb_page(__vma, __address);			  \
	}								  \
<<<<<<< HEAD
	(sparc_cpu_model == sun4c) || __changed;			  \
=======
	__changed;							  \
>>>>>>> refs/remotes/origin/master
})

#include <asm-generic/pgtable.h>

#endif /* !(__ASSEMBLY__) */

#define VMALLOC_START           _AC(0xfe600000,UL)
<<<<<<< HEAD
/* XXX Alter this when I get around to fixing sun4c - Anton */
#define VMALLOC_END             _AC(0xffc00000,UL)


=======
#define VMALLOC_END             _AC(0xffc00000,UL)

>>>>>>> refs/remotes/origin/master
/* We provide our own get_unmapped_area to cope with VA holes for userland */
#define HAVE_ARCH_UNMAPPED_AREA

/*
 * No page table caches to initialise
 */
#define pgtable_cache_init()	do { } while (0)

#endif /* !(_SPARC_PGTABLE_H) */
