/*
 * Kernel-based Virtual Machine driver for Linux
 *
 * This module enables machines with Intel VT-x extensions to run virtual
 * machines without emulation or binary translation.
 *
 * MMU support
 *
 * Copyright (C) 2006 Qumranet, Inc.
 * Copyright 2010 Red Hat, Inc. and/or its affiliates.
 *
 * Authors:
 *   Yaniv Kamay  <yaniv@qumranet.com>
 *   Avi Kivity   <avi@qumranet.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2.  See
 * the COPYING file in the top-level directory.
 *
 */

/*
 * We need the mmu code to access both 32-bit and 64-bit guest ptes,
 * so the code in this file is compiled twice, once per pte size.
 */

<<<<<<< HEAD
=======
/*
 * This is used to catch non optimized PT_GUEST_(DIRTY|ACCESS)_SHIFT macro
 * uses for EPT without A/D paging type.
 */
extern u64 __pure __using_nonexistent_pte_bit(void)
	       __compiletime_error("wrong use of PT_GUEST_(DIRTY|ACCESS)_SHIFT");

>>>>>>> refs/remotes/origin/master
#if PTTYPE == 64
	#define pt_element_t u64
	#define guest_walker guest_walker64
	#define FNAME(name) paging##64_##name
	#define PT_BASE_ADDR_MASK PT64_BASE_ADDR_MASK
	#define PT_LVL_ADDR_MASK(lvl) PT64_LVL_ADDR_MASK(lvl)
	#define PT_LVL_OFFSET_MASK(lvl) PT64_LVL_OFFSET_MASK(lvl)
	#define PT_INDEX(addr, level) PT64_INDEX(addr, level)
	#define PT_LEVEL_BITS PT64_LEVEL_BITS
<<<<<<< HEAD
=======
	#define PT_GUEST_ACCESSED_MASK PT_ACCESSED_MASK
	#define PT_GUEST_DIRTY_MASK PT_DIRTY_MASK
	#define PT_GUEST_DIRTY_SHIFT PT_DIRTY_SHIFT
	#define PT_GUEST_ACCESSED_SHIFT PT_ACCESSED_SHIFT
>>>>>>> refs/remotes/origin/master
	#ifdef CONFIG_X86_64
	#define PT_MAX_FULL_LEVELS 4
	#define CMPXCHG cmpxchg
	#else
	#define CMPXCHG cmpxchg64
	#define PT_MAX_FULL_LEVELS 2
	#endif
#elif PTTYPE == 32
	#define pt_element_t u32
	#define guest_walker guest_walker32
	#define FNAME(name) paging##32_##name
	#define PT_BASE_ADDR_MASK PT32_BASE_ADDR_MASK
	#define PT_LVL_ADDR_MASK(lvl) PT32_LVL_ADDR_MASK(lvl)
	#define PT_LVL_OFFSET_MASK(lvl) PT32_LVL_OFFSET_MASK(lvl)
	#define PT_INDEX(addr, level) PT32_INDEX(addr, level)
	#define PT_LEVEL_BITS PT32_LEVEL_BITS
	#define PT_MAX_FULL_LEVELS 2
<<<<<<< HEAD
	#define CMPXCHG cmpxchg
=======
	#define PT_GUEST_ACCESSED_MASK PT_ACCESSED_MASK
	#define PT_GUEST_DIRTY_MASK PT_DIRTY_MASK
	#define PT_GUEST_DIRTY_SHIFT PT_DIRTY_SHIFT
	#define PT_GUEST_ACCESSED_SHIFT PT_ACCESSED_SHIFT
	#define CMPXCHG cmpxchg
#elif PTTYPE == PTTYPE_EPT
	#define pt_element_t u64
	#define guest_walker guest_walkerEPT
	#define FNAME(name) ept_##name
	#define PT_BASE_ADDR_MASK PT64_BASE_ADDR_MASK
	#define PT_LVL_ADDR_MASK(lvl) PT64_LVL_ADDR_MASK(lvl)
	#define PT_LVL_OFFSET_MASK(lvl) PT64_LVL_OFFSET_MASK(lvl)
	#define PT_INDEX(addr, level) PT64_INDEX(addr, level)
	#define PT_LEVEL_BITS PT64_LEVEL_BITS
	#define PT_GUEST_ACCESSED_MASK 0
	#define PT_GUEST_DIRTY_MASK 0
	#define PT_GUEST_DIRTY_SHIFT __using_nonexistent_pte_bit()
	#define PT_GUEST_ACCESSED_SHIFT __using_nonexistent_pte_bit()
	#define CMPXCHG cmpxchg64
	#define PT_MAX_FULL_LEVELS 4
>>>>>>> refs/remotes/origin/master
#else
	#error Invalid PTTYPE value
#endif

#define gpte_to_gfn_lvl FNAME(gpte_to_gfn_lvl)
#define gpte_to_gfn(pte) gpte_to_gfn_lvl((pte), PT_PAGE_TABLE_LEVEL)

/*
 * The guest_walker structure emulates the behavior of the hardware page
 * table walker.
 */
struct guest_walker {
	int level;
<<<<<<< HEAD
=======
	unsigned max_level;
>>>>>>> refs/remotes/origin/master
	gfn_t table_gfn[PT_MAX_FULL_LEVELS];
	pt_element_t ptes[PT_MAX_FULL_LEVELS];
	pt_element_t prefetch_ptes[PTE_PREFETCH_NUM];
	gpa_t pte_gpa[PT_MAX_FULL_LEVELS];
<<<<<<< HEAD
=======
	pt_element_t __user *ptep_user[PT_MAX_FULL_LEVELS];
	bool pte_writable[PT_MAX_FULL_LEVELS];
>>>>>>> refs/remotes/origin/master
	unsigned pt_access;
	unsigned pte_access;
	gfn_t gfn;
	struct x86_exception fault;
};

static gfn_t gpte_to_gfn_lvl(pt_element_t gpte, int lvl)
{
	return (gpte & PT_LVL_ADDR_MASK(lvl)) >> PAGE_SHIFT;
}

<<<<<<< HEAD
=======
static inline void FNAME(protect_clean_gpte)(unsigned *access, unsigned gpte)
{
	unsigned mask;

	/* dirty bit is not supported, so no need to track it */
	if (!PT_GUEST_DIRTY_MASK)
		return;

	BUILD_BUG_ON(PT_WRITABLE_MASK != ACC_WRITE_MASK);

	mask = (unsigned)~ACC_WRITE_MASK;
	/* Allow write access to dirty gptes */
	mask |= (gpte >> (PT_GUEST_DIRTY_SHIFT - PT_WRITABLE_SHIFT)) &
		PT_WRITABLE_MASK;
	*access &= mask;
}

static bool FNAME(is_rsvd_bits_set)(struct kvm_mmu *mmu, u64 gpte, int level)
{
	int bit7 = (gpte >> 7) & 1, low6 = gpte & 0x3f;

	return (gpte & mmu->rsvd_bits_mask[bit7][level-1]) |
		((mmu->bad_mt_xwr & (1ull << low6)) != 0);
}

static inline int FNAME(is_present_gpte)(unsigned long pte)
{
#if PTTYPE != PTTYPE_EPT
	return is_present_gpte(pte);
#else
	return pte & 7;
#endif
}

>>>>>>> refs/remotes/origin/master
static int FNAME(cmpxchg_gpte)(struct kvm_vcpu *vcpu, struct kvm_mmu *mmu,
			       pt_element_t __user *ptep_user, unsigned index,
			       pt_element_t orig_pte, pt_element_t new_pte)
{
	int npages;
	pt_element_t ret;
	pt_element_t *table;
	struct page *page;

	npages = get_user_pages_fast((unsigned long)ptep_user, 1, 1, &page);
	/* Check if the user is doing something meaningless. */
	if (unlikely(npages != 1))
		return -EFAULT;

<<<<<<< HEAD
<<<<<<< HEAD
	table = kmap_atomic(page, KM_USER0);
	ret = CMPXCHG(&table[index], orig_pte, new_pte);
	kunmap_atomic(table, KM_USER0);
=======
	table = kmap_atomic(page);
	ret = CMPXCHG(&table[index], orig_pte, new_pte);
	kunmap_atomic(table);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	table = kmap_atomic(page);
	ret = CMPXCHG(&table[index], orig_pte, new_pte);
	kunmap_atomic(table);
>>>>>>> refs/remotes/origin/master

	kvm_release_page_dirty(page);

	return (ret != orig_pte);
}

<<<<<<< HEAD
<<<<<<< HEAD
static unsigned FNAME(gpte_access)(struct kvm_vcpu *vcpu, pt_element_t gpte)
=======
static unsigned FNAME(gpte_access)(struct kvm_vcpu *vcpu, pt_element_t gpte,
				   bool last)
>>>>>>> refs/remotes/origin/cm-10.0
{
	unsigned access;

	access = (gpte & (PT_WRITABLE_MASK | PT_USER_MASK)) | ACC_EXEC_MASK;
<<<<<<< HEAD
=======
	if (last && !is_dirty_gpte(gpte))
		access &= ~ACC_WRITE_MASK;

>>>>>>> refs/remotes/origin/cm-10.0
#if PTTYPE == 64
	if (vcpu->arch.mmu.nx)
		access &= ~(gpte >> PT64_NX_SHIFT);
#endif
	return access;
}

<<<<<<< HEAD
=======
static bool FNAME(is_last_gpte)(struct guest_walker *walker,
				struct kvm_vcpu *vcpu, struct kvm_mmu *mmu,
				pt_element_t gpte)
{
	if (walker->level == PT_PAGE_TABLE_LEVEL)
		return true;

	if ((walker->level == PT_DIRECTORY_LEVEL) && is_large_pte(gpte) &&
	    (PTTYPE == 64 || is_pse(vcpu)))
		return true;

	if ((walker->level == PT_PDPE_LEVEL) && is_large_pte(gpte) &&
	    (mmu->root_level == PT64_ROOT_LEVEL))
		return true;

	return false;
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
static bool FNAME(prefetch_invalid_gpte)(struct kvm_vcpu *vcpu,
				  struct kvm_mmu_page *sp, u64 *spte,
				  u64 gpte)
{
	if (FNAME(is_rsvd_bits_set)(&vcpu->arch.mmu, gpte, PT_PAGE_TABLE_LEVEL))
		goto no_present;

	if (!FNAME(is_present_gpte)(gpte))
		goto no_present;

	/* if accessed bit is not supported prefetch non accessed gpte */
	if (PT_GUEST_ACCESSED_MASK && !(gpte & PT_GUEST_ACCESSED_MASK))
		goto no_present;

	return false;

no_present:
	drop_spte(vcpu->kvm, spte);
	return true;
}

static inline unsigned FNAME(gpte_access)(struct kvm_vcpu *vcpu, u64 gpte)
{
	unsigned access;
#if PTTYPE == PTTYPE_EPT
	access = ((gpte & VMX_EPT_WRITABLE_MASK) ? ACC_WRITE_MASK : 0) |
		((gpte & VMX_EPT_EXECUTABLE_MASK) ? ACC_EXEC_MASK : 0) |
		ACC_USER_MASK;
#else
	access = (gpte & (PT_WRITABLE_MASK | PT_USER_MASK)) | ACC_EXEC_MASK;
	access &= ~(gpte >> PT64_NX_SHIFT);
#endif

	return access;
}

static int FNAME(update_accessed_dirty_bits)(struct kvm_vcpu *vcpu,
					     struct kvm_mmu *mmu,
					     struct guest_walker *walker,
					     int write_fault)
{
	unsigned level, index;
	pt_element_t pte, orig_pte;
	pt_element_t __user *ptep_user;
	gfn_t table_gfn;
	int ret;

	/* dirty/accessed bits are not supported, so no need to update them */
	if (!PT_GUEST_DIRTY_MASK)
		return 0;

	for (level = walker->max_level; level >= walker->level; --level) {
		pte = orig_pte = walker->ptes[level - 1];
		table_gfn = walker->table_gfn[level - 1];
		ptep_user = walker->ptep_user[level - 1];
		index = offset_in_page(ptep_user) / sizeof(pt_element_t);
		if (!(pte & PT_GUEST_ACCESSED_MASK)) {
			trace_kvm_mmu_set_accessed_bit(table_gfn, index, sizeof(pte));
			pte |= PT_GUEST_ACCESSED_MASK;
		}
		if (level == walker->level && write_fault &&
				!(pte & PT_GUEST_DIRTY_MASK)) {
			trace_kvm_mmu_set_dirty_bit(table_gfn, index, sizeof(pte));
			pte |= PT_GUEST_DIRTY_MASK;
		}
		if (pte == orig_pte)
			continue;

		/*
		 * If the slot is read-only, simply do not process the accessed
		 * and dirty bits.  This is the correct thing to do if the slot
		 * is ROM, and page tables in read-as-ROM/write-as-MMIO slots
		 * are only supported if the accessed and dirty bits are already
		 * set in the ROM (so that MMIO writes are never needed).
		 *
		 * Note that NPT does not allow this at all and faults, since
		 * it always wants nested page table entries for the guest
		 * page tables to be writable.  And EPT works but will simply
		 * overwrite the read-only memory to set the accessed and dirty
		 * bits.
		 */
		if (unlikely(!walker->pte_writable[level - 1]))
			continue;

		ret = FNAME(cmpxchg_gpte)(vcpu, mmu, ptep_user, index, orig_pte, pte);
		if (ret)
			return ret;

		mark_page_dirty(vcpu->kvm, table_gfn);
		walker->ptes[level] = pte;
	}
	return 0;
}

>>>>>>> refs/remotes/origin/master
/*
 * Fetch a guest pte for a guest virtual address
 */
static int FNAME(walk_addr_generic)(struct guest_walker *walker,
				    struct kvm_vcpu *vcpu, struct kvm_mmu *mmu,
				    gva_t addr, u32 access)
{
<<<<<<< HEAD
	pt_element_t pte;
	pt_element_t __user *uninitialized_var(ptep_user);
	gfn_t table_gfn;
	unsigned index, pt_access, uninitialized_var(pte_access);
	gpa_t pte_gpa;
<<<<<<< HEAD
	bool eperm, present, rsvd_fault;
	int offset, write_fault, user_fault, fetch_fault;

	write_fault = access & PFERR_WRITE_MASK;
	user_fault = access & PFERR_USER_MASK;
	fetch_fault = access & PFERR_FETCH_MASK;

	trace_kvm_mmu_pagetable_walk(addr, write_fault, user_fault,
				     fetch_fault);
walk:
	present = true;
	eperm = rsvd_fault = false;
=======
	bool eperm, last_gpte;
=======
	int ret;
	pt_element_t pte;
	pt_element_t __user *uninitialized_var(ptep_user);
	gfn_t table_gfn;
	unsigned index, pt_access, pte_access, accessed_dirty;
	gpa_t pte_gpa;
>>>>>>> refs/remotes/origin/master
	int offset;
	const int write_fault = access & PFERR_WRITE_MASK;
	const int user_fault  = access & PFERR_USER_MASK;
	const int fetch_fault = access & PFERR_FETCH_MASK;
	u16 errcode = 0;
<<<<<<< HEAD

	trace_kvm_mmu_pagetable_walk(addr, write_fault, user_fault,
				     fetch_fault);
retry_walk:
	eperm = false;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	gpa_t real_gpa;
	gfn_t gfn;

	trace_kvm_mmu_pagetable_walk(addr, access);
retry_walk:
>>>>>>> refs/remotes/origin/master
	walker->level = mmu->root_level;
	pte           = mmu->get_cr3(vcpu);

#if PTTYPE == 64
	if (walker->level == PT32E_ROOT_LEVEL) {
<<<<<<< HEAD
<<<<<<< HEAD
		pte = kvm_pdptr_read_mmu(vcpu, mmu, (addr >> 30) & 3);
		trace_kvm_mmu_paging_element(pte, walker->level);
		if (!is_present_gpte(pte)) {
			present = false;
			goto error;
		}
=======
		pte = mmu->get_pdptr(vcpu, (addr >> 30) & 3);
		trace_kvm_mmu_paging_element(pte, walker->level);
		if (!is_present_gpte(pte))
			goto error;
>>>>>>> refs/remotes/origin/cm-10.0
		--walker->level;
	}
#endif
	ASSERT((!is_long_mode(vcpu) && is_pae(vcpu)) ||
	       (mmu->get_cr3(vcpu) & CR3_NONPAE_RESERVED_BITS) == 0);

	pt_access = ACC_ALL;

	for (;;) {
		gfn_t real_gfn;
		unsigned long host_addr;

=======
		pte = mmu->get_pdptr(vcpu, (addr >> 30) & 3);
		trace_kvm_mmu_paging_element(pte, walker->level);
		if (!FNAME(is_present_gpte)(pte))
			goto error;
		--walker->level;
	}
#endif
	walker->max_level = walker->level;
	ASSERT((!is_long_mode(vcpu) && is_pae(vcpu)) ||
	       (mmu->get_cr3(vcpu) & CR3_NONPAE_RESERVED_BITS) == 0);

	accessed_dirty = PT_GUEST_ACCESSED_MASK;
	pt_access = pte_access = ACC_ALL;
	++walker->level;

	do {
		gfn_t real_gfn;
		unsigned long host_addr;

		pt_access &= pte_access;
		--walker->level;

>>>>>>> refs/remotes/origin/master
		index = PT_INDEX(addr, walker->level);

		table_gfn = gpte_to_gfn(pte);
		offset    = index * sizeof(pt_element_t);
		pte_gpa   = gfn_to_gpa(table_gfn) + offset;
		walker->table_gfn[walker->level - 1] = table_gfn;
		walker->pte_gpa[walker->level - 1] = pte_gpa;

		real_gfn = mmu->translate_gpa(vcpu, gfn_to_gpa(table_gfn),
					      PFERR_USER_MASK|PFERR_WRITE_MASK);
<<<<<<< HEAD
<<<<<<< HEAD
		if (unlikely(real_gfn == UNMAPPED_GVA)) {
			present = false;
			break;
		}
		real_gfn = gpa_to_gfn(real_gfn);

		host_addr = gfn_to_hva(vcpu->kvm, real_gfn);
		if (unlikely(kvm_is_error_hva(host_addr))) {
			present = false;
			break;
		}

		ptep_user = (pt_element_t __user *)((void *)host_addr + offset);
		if (unlikely(__copy_from_user(&pte, ptep_user, sizeof(pte)))) {
			present = false;
			break;
		}

		trace_kvm_mmu_paging_element(pte, walker->level);

		if (unlikely(!is_present_gpte(pte))) {
			present = false;
			break;
		}

		if (unlikely(is_rsvd_bits_set(&vcpu->arch.mmu, pte,
					      walker->level))) {
			rsvd_fault = true;
			break;
		}

		if (unlikely(write_fault && !is_writable_pte(pte)
			     && (user_fault || is_write_protection(vcpu))))
			eperm = true;

		if (unlikely(user_fault && !(pte & PT_USER_MASK)))
=======
=======
>>>>>>> refs/remotes/origin/master
		if (unlikely(real_gfn == UNMAPPED_GVA))
			goto error;
		real_gfn = gpa_to_gfn(real_gfn);

<<<<<<< HEAD
		host_addr = gfn_to_hva(vcpu->kvm, real_gfn);
=======
		host_addr = gfn_to_hva_prot(vcpu->kvm, real_gfn,
					    &walker->pte_writable[walker->level - 1]);
>>>>>>> refs/remotes/origin/master
		if (unlikely(kvm_is_error_hva(host_addr)))
			goto error;

		ptep_user = (pt_element_t __user *)((void *)host_addr + offset);
		if (unlikely(__copy_from_user(&pte, ptep_user, sizeof(pte))))
			goto error;
<<<<<<< HEAD

		trace_kvm_mmu_paging_element(pte, walker->level);

		if (unlikely(!is_present_gpte(pte)))
			goto error;

		if (unlikely(is_rsvd_bits_set(&vcpu->arch.mmu, pte,
					      walker->level))) {
=======
		walker->ptep_user[walker->level - 1] = ptep_user;

		trace_kvm_mmu_paging_element(pte, walker->level);

		if (unlikely(!FNAME(is_present_gpte)(pte)))
			goto error;

		if (unlikely(FNAME(is_rsvd_bits_set)(mmu, pte,
					             walker->level))) {
>>>>>>> refs/remotes/origin/master
			errcode |= PFERR_RSVD_MASK | PFERR_PRESENT_MASK;
			goto error;
		}

<<<<<<< HEAD
		if (!check_write_user_access(vcpu, write_fault, user_fault,
					  pte))
>>>>>>> refs/remotes/origin/cm-10.0
			eperm = true;

#if PTTYPE == 64
		if (unlikely(fetch_fault && (pte & PT64_NX_MASK)))
			eperm = true;
#endif

<<<<<<< HEAD
		if (!eperm && !rsvd_fault
		    && unlikely(!(pte & PT_ACCESSED_MASK))) {
=======
		last_gpte = FNAME(is_last_gpte)(walker, vcpu, mmu, pte);
		if (last_gpte) {
			pte_access = pt_access &
				     FNAME(gpte_access)(vcpu, pte, true);
			/* check if the kernel is fetching from user page */
			if (unlikely(pte_access & PT_USER_MASK) &&
			    kvm_read_cr4_bits(vcpu, X86_CR4_SMEP))
				if (fetch_fault && !user_fault)
					eperm = true;
		}

		if (!eperm && unlikely(!(pte & PT_ACCESSED_MASK))) {
>>>>>>> refs/remotes/origin/cm-10.0
			int ret;
			trace_kvm_mmu_set_accessed_bit(table_gfn, index,
						       sizeof(pte));
			ret = FNAME(cmpxchg_gpte)(vcpu, mmu, ptep_user, index,
						  pte, pte|PT_ACCESSED_MASK);
<<<<<<< HEAD
			if (unlikely(ret < 0)) {
				present = false;
				break;
			} else if (ret)
				goto walk;
=======
			if (unlikely(ret < 0))
				goto error;
			else if (ret)
				goto retry_walk;
>>>>>>> refs/remotes/origin/cm-10.0

			mark_page_dirty(vcpu->kvm, table_gfn);
			pte |= PT_ACCESSED_MASK;
		}

<<<<<<< HEAD
		pte_access = pt_access & FNAME(gpte_access)(vcpu, pte);

		walker->ptes[walker->level - 1] = pte;

		if ((walker->level == PT_PAGE_TABLE_LEVEL) ||
		    ((walker->level == PT_DIRECTORY_LEVEL) &&
				is_large_pte(pte) &&
				(PTTYPE == 64 || is_pse(vcpu))) ||
		    ((walker->level == PT_PDPE_LEVEL) &&
				is_large_pte(pte) &&
				mmu->root_level == PT64_ROOT_LEVEL)) {
=======
		walker->ptes[walker->level - 1] = pte;

		if (last_gpte) {
>>>>>>> refs/remotes/origin/cm-10.0
			int lvl = walker->level;
			gpa_t real_gpa;
			gfn_t gfn;
			u32 ac;

			gfn = gpte_to_gfn_lvl(pte, lvl);
			gfn += (addr & PT_LVL_OFFSET_MASK(lvl)) >> PAGE_SHIFT;

			if (PTTYPE == 32 &&
			    walker->level == PT_DIRECTORY_LEVEL &&
			    is_cpuid_PSE36())
				gfn += pse36_gfn_delta(pte);

			ac = write_fault | fetch_fault | user_fault;

			real_gpa = mmu->translate_gpa(vcpu, gfn_to_gpa(gfn),
						      ac);
			if (real_gpa == UNMAPPED_GVA)
				return 0;

			walker->gfn = real_gpa >> PAGE_SHIFT;

			break;
		}

<<<<<<< HEAD
		pt_access = pte_access;
		--walker->level;
	}

	if (unlikely(!present || eperm || rsvd_fault))
		goto error;
=======
		pt_access &= FNAME(gpte_access)(vcpu, pte, false);
		--walker->level;
	}

	if (unlikely(eperm)) {
		errcode |= PFERR_PRESENT_MASK;
		goto error;
	}
>>>>>>> refs/remotes/origin/cm-10.0

	if (write_fault && unlikely(!is_dirty_gpte(pte))) {
		int ret;

		trace_kvm_mmu_set_dirty_bit(table_gfn, index, sizeof(pte));
		ret = FNAME(cmpxchg_gpte)(vcpu, mmu, ptep_user, index,
					  pte, pte|PT_DIRTY_MASK);
<<<<<<< HEAD
		if (unlikely(ret < 0)) {
			present = false;
			goto error;
		} else if (ret)
			goto walk;
=======
=======
		accessed_dirty &= pte;
		pte_access = pt_access & FNAME(gpte_access)(vcpu, pte);

		walker->ptes[walker->level - 1] = pte;
	} while (!is_last_gpte(mmu, walker->level, pte));

	if (unlikely(permission_fault(mmu, pte_access, access))) {
		errcode |= PFERR_PRESENT_MASK;
		goto error;
	}

	gfn = gpte_to_gfn_lvl(pte, walker->level);
	gfn += (addr & PT_LVL_OFFSET_MASK(walker->level)) >> PAGE_SHIFT;

	if (PTTYPE == 32 && walker->level == PT_DIRECTORY_LEVEL && is_cpuid_PSE36())
		gfn += pse36_gfn_delta(pte);

	real_gpa = mmu->translate_gpa(vcpu, gfn_to_gpa(gfn), access);
	if (real_gpa == UNMAPPED_GVA)
		return 0;

	walker->gfn = real_gpa >> PAGE_SHIFT;

	if (!write_fault)
		FNAME(protect_clean_gpte)(&pte_access, pte);
	else
		/*
		 * On a write fault, fold the dirty bit into accessed_dirty.
		 * For modes without A/D bits support accessed_dirty will be
		 * always clear.
		 */
		accessed_dirty &= pte >>
			(PT_GUEST_DIRTY_SHIFT - PT_GUEST_ACCESSED_SHIFT);

	if (unlikely(!accessed_dirty)) {
		ret = FNAME(update_accessed_dirty_bits)(vcpu, mmu, walker, write_fault);
>>>>>>> refs/remotes/origin/master
		if (unlikely(ret < 0))
			goto error;
		else if (ret)
			goto retry_walk;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

		mark_page_dirty(vcpu->kvm, table_gfn);
		pte |= PT_DIRTY_MASK;
		walker->ptes[walker->level - 1] = pte;
=======
>>>>>>> refs/remotes/origin/master
	}

	walker->pt_access = pt_access;
	walker->pte_access = pte_access;
	pgprintk("%s: pte %llx pte_access %x pt_access %x\n",
		 __func__, (u64)pte, pte_access, pt_access);
	return 1;

error:
<<<<<<< HEAD
<<<<<<< HEAD
	walker->fault.vector = PF_VECTOR;
	walker->fault.error_code_valid = true;
	walker->fault.error_code = 0;
	if (present)
		walker->fault.error_code |= PFERR_PRESENT_MASK;

	walker->fault.error_code |= write_fault | user_fault;

	if (fetch_fault && mmu->nx)
		walker->fault.error_code |= PFERR_FETCH_MASK;
	if (rsvd_fault)
		walker->fault.error_code |= PFERR_RSVD_MASK;

=======
=======
>>>>>>> refs/remotes/origin/master
	errcode |= write_fault | user_fault;
	if (fetch_fault && (mmu->nx ||
			    kvm_read_cr4_bits(vcpu, X86_CR4_SMEP)))
		errcode |= PFERR_FETCH_MASK;

	walker->fault.vector = PF_VECTOR;
	walker->fault.error_code_valid = true;
	walker->fault.error_code = errcode;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======

#if PTTYPE == PTTYPE_EPT
	/*
	 * Use PFERR_RSVD_MASK in error_code to to tell if EPT
	 * misconfiguration requires to be injected. The detection is
	 * done by is_rsvd_bits_set() above.
	 *
	 * We set up the value of exit_qualification to inject:
	 * [2:0] - Derive from [2:0] of real exit_qualification at EPT violation
	 * [5:3] - Calculated by the page walk of the guest EPT page tables
	 * [7:8] - Derived from [7:8] of real exit_qualification
	 *
	 * The other bits are set to 0.
	 */
	if (!(errcode & PFERR_RSVD_MASK)) {
		vcpu->arch.exit_qualification &= 0x187;
		vcpu->arch.exit_qualification |= ((pt_access & pte) & 0x7) << 3;
	}
#endif
>>>>>>> refs/remotes/origin/master
	walker->fault.address = addr;
	walker->fault.nested_page_fault = mmu != vcpu->arch.walk_mmu;

	trace_kvm_mmu_walker_error(walker->fault.error_code);
	return 0;
}

static int FNAME(walk_addr)(struct guest_walker *walker,
			    struct kvm_vcpu *vcpu, gva_t addr, u32 access)
{
	return FNAME(walk_addr_generic)(walker, vcpu, &vcpu->arch.mmu, addr,
					access);
}

<<<<<<< HEAD
=======
#if PTTYPE != PTTYPE_EPT
>>>>>>> refs/remotes/origin/master
static int FNAME(walk_addr_nested)(struct guest_walker *walker,
				   struct kvm_vcpu *vcpu, gva_t addr,
				   u32 access)
{
	return FNAME(walk_addr_generic)(walker, vcpu, &vcpu->arch.nested_mmu,
					addr, access);
}
<<<<<<< HEAD

static bool FNAME(prefetch_invalid_gpte)(struct kvm_vcpu *vcpu,
				    struct kvm_mmu_page *sp, u64 *spte,
				    pt_element_t gpte)
{
<<<<<<< HEAD
	u64 nonpresent = shadow_trap_nonpresent_pte;

	if (is_rsvd_bits_set(&vcpu->arch.mmu, gpte, PT_PAGE_TABLE_LEVEL))
		goto no_present;

	if (!is_present_gpte(gpte)) {
		if (!sp->unsync)
			nonpresent = shadow_notrap_nonpresent_pte;
		goto no_present;
	}
=======
	if (is_rsvd_bits_set(&vcpu->arch.mmu, gpte, PT_PAGE_TABLE_LEVEL))
		goto no_present;

	if (!is_present_gpte(gpte))
		goto no_present;
>>>>>>> refs/remotes/origin/cm-10.0

	if (!(gpte & PT_ACCESSED_MASK))
		goto no_present;

	return false;

no_present:
<<<<<<< HEAD
	drop_spte(vcpu->kvm, spte, nonpresent);
=======
	drop_spte(vcpu->kvm, spte);
>>>>>>> refs/remotes/origin/cm-10.0
=======
#endif

static bool
FNAME(prefetch_gpte)(struct kvm_vcpu *vcpu, struct kvm_mmu_page *sp,
		     u64 *spte, pt_element_t gpte, bool no_dirty_log)
{
	unsigned pte_access;
	gfn_t gfn;
	pfn_t pfn;

	if (FNAME(prefetch_invalid_gpte)(vcpu, sp, spte, gpte))
		return false;

	pgprintk("%s: gpte %llx spte %p\n", __func__, (u64)gpte, spte);

	gfn = gpte_to_gfn(gpte);
	pte_access = sp->role.access & FNAME(gpte_access)(vcpu, gpte);
	FNAME(protect_clean_gpte)(&pte_access, gpte);
	pfn = pte_prefetch_gfn_to_pfn(vcpu, gfn,
			no_dirty_log && (pte_access & ACC_WRITE_MASK));
	if (is_error_pfn(pfn))
		return false;

	/*
	 * we call mmu_set_spte() with host_writable = true because
	 * pte_prefetch_gfn_to_pfn always gets a writable pfn.
	 */
	mmu_set_spte(vcpu, spte, pte_access, 0, NULL, PT_PAGE_TABLE_LEVEL,
		     gfn, pfn, true, true);

>>>>>>> refs/remotes/origin/master
	return true;
}

static void FNAME(update_pte)(struct kvm_vcpu *vcpu, struct kvm_mmu_page *sp,
			      u64 *spte, const void *pte)
{
<<<<<<< HEAD
	pt_element_t gpte;
	unsigned pte_access;
	pfn_t pfn;

	gpte = *(const pt_element_t *)pte;
	if (FNAME(prefetch_invalid_gpte)(vcpu, sp, spte, gpte))
		return;

	pgprintk("%s: gpte %llx spte %p\n", __func__, (u64)gpte, spte);
<<<<<<< HEAD
	pte_access = sp->role.access & FNAME(gpte_access)(vcpu, gpte);
	pfn = gfn_to_pfn_atomic(vcpu->kvm, gpte_to_gfn(gpte));
	if (is_error_pfn(pfn)) {
=======
	pte_access = sp->role.access & FNAME(gpte_access)(vcpu, gpte, true);
	pfn = gfn_to_pfn_atomic(vcpu->kvm, gpte_to_gfn(gpte));
	if (mmu_invalid_pfn(pfn)) {
>>>>>>> refs/remotes/origin/cm-10.0
		kvm_release_pfn_clean(pfn);
		return;
	}

	/*
	 * we call mmu_set_spte() with host_writable = true because that
	 * vcpu->arch.update_pte.pfn was fetched from get_user_pages(write = 1).
	 */
	mmu_set_spte(vcpu, spte, sp->role.access, pte_access, 0, 0,
<<<<<<< HEAD
		     is_dirty_gpte(gpte), NULL, PT_PAGE_TABLE_LEVEL,
=======
		     NULL, PT_PAGE_TABLE_LEVEL,
>>>>>>> refs/remotes/origin/cm-10.0
		     gpte_to_gfn(gpte), pfn, true, true);
=======
	pt_element_t gpte = *(const pt_element_t *)pte;

	FNAME(prefetch_gpte)(vcpu, sp, spte, gpte, false);
>>>>>>> refs/remotes/origin/master
}

static bool FNAME(gpte_changed)(struct kvm_vcpu *vcpu,
				struct guest_walker *gw, int level)
{
	pt_element_t curr_pte;
	gpa_t base_gpa, pte_gpa = gw->pte_gpa[level - 1];
	u64 mask;
	int r, index;

	if (level == PT_PAGE_TABLE_LEVEL) {
		mask = PTE_PREFETCH_NUM * sizeof(pt_element_t) - 1;
		base_gpa = pte_gpa & ~mask;
		index = (pte_gpa - base_gpa) / sizeof(pt_element_t);

		r = kvm_read_guest_atomic(vcpu->kvm, base_gpa,
				gw->prefetch_ptes, sizeof(gw->prefetch_ptes));
		curr_pte = gw->prefetch_ptes[index];
	} else
		r = kvm_read_guest_atomic(vcpu->kvm, pte_gpa,
				  &curr_pte, sizeof(curr_pte));

	return r || curr_pte != gw->ptes[level - 1];
}

static void FNAME(pte_prefetch)(struct kvm_vcpu *vcpu, struct guest_walker *gw,
				u64 *sptep)
{
	struct kvm_mmu_page *sp;
	pt_element_t *gptep = gw->prefetch_ptes;
	u64 *spte;
	int i;

	sp = page_header(__pa(sptep));

	if (sp->role.level > PT_PAGE_TABLE_LEVEL)
		return;

	if (sp->role.direct)
		return __direct_pte_prefetch(vcpu, sp, sptep);

	i = (sptep - sp->spt) & ~(PTE_PREFETCH_NUM - 1);
	spte = sp->spt + i;

	for (i = 0; i < PTE_PREFETCH_NUM; i++, spte++) {
<<<<<<< HEAD
		pt_element_t gpte;
		unsigned pte_access;
		gfn_t gfn;
		pfn_t pfn;
<<<<<<< HEAD
		bool dirty;
=======
>>>>>>> refs/remotes/origin/cm-10.0

		if (spte == sptep)
			continue;

<<<<<<< HEAD
		if (*spte != shadow_trap_nonpresent_pte)
=======
		if (is_shadow_present_pte(*spte))
>>>>>>> refs/remotes/origin/cm-10.0
			continue;

		gpte = gptep[i];

		if (FNAME(prefetch_invalid_gpte)(vcpu, sp, spte, gpte))
			continue;

<<<<<<< HEAD
		pte_access = sp->role.access & FNAME(gpte_access)(vcpu, gpte);
		gfn = gpte_to_gfn(gpte);
		dirty = is_dirty_gpte(gpte);
		pfn = pte_prefetch_gfn_to_pfn(vcpu, gfn,
				      (pte_access & ACC_WRITE_MASK) && dirty);
		if (is_error_pfn(pfn)) {
=======
		pte_access = sp->role.access & FNAME(gpte_access)(vcpu, gpte,
								  true);
		gfn = gpte_to_gfn(gpte);
		pfn = pte_prefetch_gfn_to_pfn(vcpu, gfn,
				      pte_access & ACC_WRITE_MASK);
		if (mmu_invalid_pfn(pfn)) {
>>>>>>> refs/remotes/origin/cm-10.0
			kvm_release_pfn_clean(pfn);
			break;
		}

		mmu_set_spte(vcpu, spte, sp->role.access, pte_access, 0, 0,
<<<<<<< HEAD
			     dirty, NULL, PT_PAGE_TABLE_LEVEL, gfn,
=======
			     NULL, PT_PAGE_TABLE_LEVEL, gfn,
>>>>>>> refs/remotes/origin/cm-10.0
			     pfn, true, true);
=======
		if (spte == sptep)
			continue;

		if (is_shadow_present_pte(*spte))
			continue;

		if (!FNAME(prefetch_gpte)(vcpu, sp, spte, gptep[i], true))
			break;
>>>>>>> refs/remotes/origin/master
	}
}

/*
 * Fetch a shadow pte for a specific level in the paging hierarchy.
<<<<<<< HEAD
 */
static u64 *FNAME(fetch)(struct kvm_vcpu *vcpu, gva_t addr,
			 struct guest_walker *gw,
			 int user_fault, int write_fault, int hlevel,
<<<<<<< HEAD
			 int *ptwrite, pfn_t pfn, bool map_writable,
=======
			 int *emulate, pfn_t pfn, bool map_writable,
>>>>>>> refs/remotes/origin/cm-10.0
			 bool prefault)
{
	unsigned access = gw->pt_access;
	struct kvm_mmu_page *sp = NULL;
<<<<<<< HEAD
	bool dirty = is_dirty_gpte(gw->ptes[gw->level - 1]);
=======
>>>>>>> refs/remotes/origin/cm-10.0
	int top_level;
	unsigned direct_access;
	struct kvm_shadow_walk_iterator it;

	if (!is_present_gpte(gw->ptes[gw->level - 1]))
		return NULL;

<<<<<<< HEAD
	direct_access = gw->pt_access & gw->pte_access;
	if (!dirty)
		direct_access &= ~ACC_WRITE_MASK;
=======
	direct_access = gw->pte_access;
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * If the guest tries to write a write-protected page, we need to
 * emulate this operation, return 1 to indicate this case.
 */
static int FNAME(fetch)(struct kvm_vcpu *vcpu, gva_t addr,
			 struct guest_walker *gw,
			 int write_fault, int hlevel,
			 pfn_t pfn, bool map_writable, bool prefault)
{
	struct kvm_mmu_page *sp = NULL;
	struct kvm_shadow_walk_iterator it;
	unsigned direct_access, access = gw->pt_access;
	int top_level, emulate = 0;

	direct_access = gw->pte_access;
>>>>>>> refs/remotes/origin/master

	top_level = vcpu->arch.mmu.root_level;
	if (top_level == PT32E_ROOT_LEVEL)
		top_level = PT32_ROOT_LEVEL;
	/*
	 * Verify that the top-level gpte is still there.  Since the page
	 * is a root page, it is either write protected (and cannot be
	 * changed from now on) or it is invalid (in which case, we don't
	 * really care if it changes underneath us after this point).
	 */
	if (FNAME(gpte_changed)(vcpu, gw, top_level))
		goto out_gpte_changed;

	for (shadow_walk_init(&it, vcpu, addr);
	     shadow_walk_okay(&it) && it.level > gw->level;
	     shadow_walk_next(&it)) {
		gfn_t table_gfn;

<<<<<<< HEAD
<<<<<<< HEAD
=======
		clear_sp_write_flooding_count(it.sptep);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		clear_sp_write_flooding_count(it.sptep);
>>>>>>> refs/remotes/origin/master
		drop_large_spte(vcpu, it.sptep);

		sp = NULL;
		if (!is_shadow_present_pte(*it.sptep)) {
			table_gfn = gw->table_gfn[it.level - 2];
			sp = kvm_mmu_get_page(vcpu, table_gfn, addr, it.level-1,
					      false, access, it.sptep);
		}

		/*
		 * Verify that the gpte in the page we've just write
		 * protected is still there.
		 */
		if (FNAME(gpte_changed)(vcpu, gw, it.level - 1))
			goto out_gpte_changed;

		if (sp)
<<<<<<< HEAD
			link_shadow_page(it.sptep, sp);
=======
			link_shadow_page(it.sptep, sp, PT_GUEST_ACCESSED_MASK);
>>>>>>> refs/remotes/origin/master
	}

	for (;
	     shadow_walk_okay(&it) && it.level > hlevel;
	     shadow_walk_next(&it)) {
		gfn_t direct_gfn;

<<<<<<< HEAD
<<<<<<< HEAD
=======
		clear_sp_write_flooding_count(it.sptep);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		clear_sp_write_flooding_count(it.sptep);
>>>>>>> refs/remotes/origin/master
		validate_direct_spte(vcpu, it.sptep, direct_access);

		drop_large_spte(vcpu, it.sptep);

		if (is_shadow_present_pte(*it.sptep))
			continue;

		direct_gfn = gw->gfn & ~(KVM_PAGES_PER_HPAGE(it.level) - 1);

		sp = kvm_mmu_get_page(vcpu, direct_gfn, addr, it.level-1,
				      true, direct_access, it.sptep);
<<<<<<< HEAD
		link_shadow_page(it.sptep, sp);
	}

<<<<<<< HEAD
	mmu_set_spte(vcpu, it.sptep, access, gw->pte_access & access,
		     user_fault, write_fault, dirty, ptwrite, it.level,
=======
	clear_sp_write_flooding_count(it.sptep);
	mmu_set_spte(vcpu, it.sptep, access, gw->pte_access,
		     user_fault, write_fault, emulate, it.level,
>>>>>>> refs/remotes/origin/cm-10.0
		     gw->gfn, pfn, prefault, map_writable);
	FNAME(pte_prefetch)(vcpu, gw, it.sptep);

	return it.sptep;
=======
		link_shadow_page(it.sptep, sp, PT_GUEST_ACCESSED_MASK);
	}

	clear_sp_write_flooding_count(it.sptep);
	mmu_set_spte(vcpu, it.sptep, gw->pte_access, write_fault, &emulate,
		     it.level, gw->gfn, pfn, prefault, map_writable);
	FNAME(pte_prefetch)(vcpu, gw, it.sptep);

	return emulate;
>>>>>>> refs/remotes/origin/master

out_gpte_changed:
	if (sp)
		kvm_mmu_put_page(sp, it.sptep);
	kvm_release_pfn_clean(pfn);
<<<<<<< HEAD
	return NULL;
=======
	return 0;
}

 /*
 * To see whether the mapped gfn can write its page table in the current
 * mapping.
 *
 * It is the helper function of FNAME(page_fault). When guest uses large page
 * size to map the writable gfn which is used as current page table, we should
 * force kvm to use small page size to map it because new shadow page will be
 * created when kvm establishes shadow page table that stop kvm using large
 * page size. Do it early can avoid unnecessary #PF and emulation.
 *
 * @write_fault_to_shadow_pgtable will return true if the fault gfn is
 * currently used as its page table.
 *
 * Note: the PDPT page table is not checked for PAE-32 bit guest. It is ok
 * since the PDPT is always shadowed, that means, we can not use large page
 * size to map the gfn which is used as PDPT.
 */
static bool
FNAME(is_self_change_mapping)(struct kvm_vcpu *vcpu,
			      struct guest_walker *walker, int user_fault,
			      bool *write_fault_to_shadow_pgtable)
{
	int level;
	gfn_t mask = ~(KVM_PAGES_PER_HPAGE(walker->level) - 1);
	bool self_changed = false;

	if (!(walker->pte_access & ACC_WRITE_MASK ||
	      (!is_write_protection(vcpu) && !user_fault)))
		return false;

	for (level = walker->level; level <= walker->max_level; level++) {
		gfn_t gfn = walker->gfn ^ walker->table_gfn[level - 1];

		self_changed |= !(gfn & mask);
		*write_fault_to_shadow_pgtable |= !gfn;
	}

	return self_changed;
>>>>>>> refs/remotes/origin/master
}

/*
 * Page fault handler.  There are several causes for a page fault:
 *   - there is no shadow pte for the guest pte
 *   - write access through a shadow pte marked read only so that we can set
 *     the dirty bit
 *   - write access to a shadow pte marked read only so we can update the page
 *     dirty bitmap, when userspace requests it
 *   - mmio access; in this case we will never install a present shadow pte
 *   - normal guest page fault due to the guest pte marked not present, not
 *     writable, or not executable
 *
 *  Returns: 1 if we need to emulate the instruction, 0 otherwise, or
 *           a negative value on error.
 */
static int FNAME(page_fault)(struct kvm_vcpu *vcpu, gva_t addr, u32 error_code,
			     bool prefault)
{
	int write_fault = error_code & PFERR_WRITE_MASK;
	int user_fault = error_code & PFERR_USER_MASK;
	struct guest_walker walker;
<<<<<<< HEAD
	u64 *sptep;
<<<<<<< HEAD
	int write_pt = 0;
=======
	int emulate = 0;
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	int r;
	pfn_t pfn;
	int level = PT_PAGE_TABLE_LEVEL;
	int force_pt_level;
	unsigned long mmu_seq;
<<<<<<< HEAD
	bool map_writable;

	pgprintk("%s: addr %lx err %x\n", __func__, addr, error_code);

<<<<<<< HEAD
=======
	if (unlikely(error_code & PFERR_RSVD_MASK))
		return handle_mmio_page_fault(vcpu, addr, error_code,
					      mmu_is_nested(vcpu));

>>>>>>> refs/remotes/origin/cm-10.0
=======
	bool map_writable, is_self_change_mapping;

	pgprintk("%s: addr %lx err %x\n", __func__, addr, error_code);

	if (unlikely(error_code & PFERR_RSVD_MASK)) {
		r = handle_mmio_page_fault(vcpu, addr, error_code,
					      mmu_is_nested(vcpu));
		if (likely(r != RET_MMIO_PF_INVALID))
			return r;
	};

>>>>>>> refs/remotes/origin/master
	r = mmu_topup_memory_caches(vcpu);
	if (r)
		return r;

	/*
	 * Look up the guest pte for the faulting address.
	 */
	r = FNAME(walk_addr)(&walker, vcpu, addr, error_code);

	/*
	 * The page is not mapped by the guest.  Let the guest handle it.
	 */
	if (!r) {
		pgprintk("%s: guest page fault\n", __func__);
<<<<<<< HEAD
<<<<<<< HEAD
		if (!prefault) {
			inject_page_fault(vcpu, &walker.fault);
			/* reset fork detector */
			vcpu->arch.last_pt_write_count = 0;
		}
=======
		if (!prefault)
			inject_page_fault(vcpu, &walker.fault);

>>>>>>> refs/remotes/origin/cm-10.0
		return 0;
	}

	if (walker.level >= PT_DIRECTORY_LEVEL)
		force_pt_level = mapping_level_dirty_bitmap(vcpu, walker.gfn);
=======
		if (!prefault)
			inject_page_fault(vcpu, &walker.fault);

		return 0;
	}

	vcpu->arch.write_fault_to_shadow_pgtable = false;

	is_self_change_mapping = FNAME(is_self_change_mapping)(vcpu,
	      &walker, user_fault, &vcpu->arch.write_fault_to_shadow_pgtable);

	if (walker.level >= PT_DIRECTORY_LEVEL)
		force_pt_level = mapping_level_dirty_bitmap(vcpu, walker.gfn)
		   || is_self_change_mapping;
>>>>>>> refs/remotes/origin/master
	else
		force_pt_level = 1;
	if (!force_pt_level) {
		level = min(walker.level, mapping_level(vcpu, walker.gfn));
		walker.gfn = walker.gfn & ~(KVM_PAGES_PER_HPAGE(level) - 1);
	}

	mmu_seq = vcpu->kvm->mmu_notifier_seq;
	smp_rmb();

	if (try_async_pf(vcpu, prefault, walker.gfn, addr, &pfn, write_fault,
			 &map_writable))
		return 0;

<<<<<<< HEAD
<<<<<<< HEAD
	/* mmio */
	if (is_error_pfn(pfn))
		return kvm_handle_bad_page(vcpu->kvm, walker.gfn, pfn);
=======
	if (handle_abnormal_pfn(vcpu, mmu_is_nested(vcpu) ? 0 : addr,
				walker.gfn, pfn, walker.pte_access, &r))
		return r;
>>>>>>> refs/remotes/origin/cm-10.0

	spin_lock(&vcpu->kvm->mmu_lock);
	if (mmu_notifier_retry(vcpu, mmu_seq))
		goto out_unlock;

<<<<<<< HEAD
	trace_kvm_mmu_audit(vcpu, AUDIT_PRE_PAGE_FAULT);
=======
	kvm_mmu_audit(vcpu, AUDIT_PRE_PAGE_FAULT);
>>>>>>> refs/remotes/origin/cm-10.0
	kvm_mmu_free_some_pages(vcpu);
	if (!force_pt_level)
		transparent_hugepage_adjust(vcpu, &walker.gfn, &pfn, &level);
	sptep = FNAME(fetch)(vcpu, addr, &walker, user_fault, write_fault,
<<<<<<< HEAD
			     level, &write_pt, pfn, map_writable, prefault);
	(void)sptep;
	pgprintk("%s: shadow pte %p %llx ptwrite %d\n", __func__,
		 sptep, *sptep, write_pt);

	if (!write_pt)
		vcpu->arch.last_pt_write_count = 0; /* reset fork detector */

	++vcpu->stat.pf_fixed;
	trace_kvm_mmu_audit(vcpu, AUDIT_POST_PAGE_FAULT);
	spin_unlock(&vcpu->kvm->mmu_lock);

	return write_pt;
=======
			     level, &emulate, pfn, map_writable, prefault);
	(void)sptep;
	pgprintk("%s: shadow pte %p %llx emulate %d\n", __func__,
		 sptep, *sptep, emulate);

=======
	if (handle_abnormal_pfn(vcpu, mmu_is_nested(vcpu) ? 0 : addr,
				walker.gfn, pfn, walker.pte_access, &r))
		return r;

	/*
	 * Do not change pte_access if the pfn is a mmio page, otherwise
	 * we will cache the incorrect access into mmio spte.
	 */
	if (write_fault && !(walker.pte_access & ACC_WRITE_MASK) &&
	     !is_write_protection(vcpu) && !user_fault &&
	      !is_noslot_pfn(pfn)) {
		walker.pte_access |= ACC_WRITE_MASK;
		walker.pte_access &= ~ACC_USER_MASK;

		/*
		 * If we converted a user page to a kernel page,
		 * so that the kernel can write to it when cr0.wp=0,
		 * then we should prevent the kernel from executing it
		 * if SMEP is enabled.
		 */
		if (kvm_read_cr4_bits(vcpu, X86_CR4_SMEP))
			walker.pte_access &= ~ACC_EXEC_MASK;
	}

	spin_lock(&vcpu->kvm->mmu_lock);
	if (mmu_notifier_retry(vcpu->kvm, mmu_seq))
		goto out_unlock;

	kvm_mmu_audit(vcpu, AUDIT_PRE_PAGE_FAULT);
	make_mmu_pages_available(vcpu);
	if (!force_pt_level)
		transparent_hugepage_adjust(vcpu, &walker.gfn, &pfn, &level);
	r = FNAME(fetch)(vcpu, addr, &walker, write_fault,
			 level, pfn, map_writable, prefault);
>>>>>>> refs/remotes/origin/master
	++vcpu->stat.pf_fixed;
	kvm_mmu_audit(vcpu, AUDIT_POST_PAGE_FAULT);
	spin_unlock(&vcpu->kvm->mmu_lock);

<<<<<<< HEAD
	return emulate;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return r;
>>>>>>> refs/remotes/origin/master

out_unlock:
	spin_unlock(&vcpu->kvm->mmu_lock);
	kvm_release_pfn_clean(pfn);
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static gpa_t FNAME(get_level1_sp_gpa)(struct kvm_mmu_page *sp)
{
	int offset = 0;

<<<<<<< HEAD
	WARN_ON(sp->role.level != 1);
=======
	WARN_ON(sp->role.level != PT_PAGE_TABLE_LEVEL);
>>>>>>> refs/remotes/origin/master

	if (PTTYPE == 32)
		offset = sp->role.quadrant << PT64_LEVEL_BITS;

	return gfn_to_gpa(sp->gfn) + offset * sizeof(pt_element_t);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static void FNAME(invlpg)(struct kvm_vcpu *vcpu, gva_t gva)
{
	struct kvm_shadow_walk_iterator iterator;
	struct kvm_mmu_page *sp;
<<<<<<< HEAD
<<<<<<< HEAD
	gpa_t pte_gpa = -1;
	int level;
	u64 *sptep;
	int need_flush = 0;

	spin_lock(&vcpu->kvm->mmu_lock);

=======
=======
>>>>>>> refs/remotes/origin/master
	int level;
	u64 *sptep;

	vcpu_clear_mmio_info(vcpu, gva);

	/*
	 * No need to check return value here, rmap_can_add() can
	 * help us to skip pte prefetch later.
	 */
	mmu_topup_memory_caches(vcpu);

	spin_lock(&vcpu->kvm->mmu_lock);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	for_each_shadow_entry(vcpu, gva, iterator) {
		level = iterator.level;
		sptep = iterator.sptep;

		sp = page_header(__pa(sptep));
		if (is_last_spte(*sptep, level)) {
<<<<<<< HEAD
<<<<<<< HEAD
			int offset, shift;
=======
			pt_element_t gpte;
			gpa_t pte_gpa;
>>>>>>> refs/remotes/origin/cm-10.0
=======
			pt_element_t gpte;
			gpa_t pte_gpa;
>>>>>>> refs/remotes/origin/master

			if (!sp->unsync)
				break;

<<<<<<< HEAD
<<<<<<< HEAD
			shift = PAGE_SHIFT -
				  (PT_LEVEL_BITS - PT64_LEVEL_BITS) * level;
			offset = sp->role.quadrant << shift;

			pte_gpa = (sp->gfn << PAGE_SHIFT) + offset;
			pte_gpa += (sptep - sp->spt) * sizeof(pt_element_t);

			if (is_shadow_present_pte(*sptep)) {
				if (is_large_pte(*sptep))
					--vcpu->kvm->stat.lpages;
				drop_spte(vcpu->kvm, sptep,
					  shadow_trap_nonpresent_pte);
				need_flush = 1;
			} else
				__set_spte(sptep, shadow_trap_nonpresent_pte);
			break;
=======
=======
>>>>>>> refs/remotes/origin/master
			pte_gpa = FNAME(get_level1_sp_gpa)(sp);
			pte_gpa += (sptep - sp->spt) * sizeof(pt_element_t);

			if (mmu_page_zap_pte(vcpu->kvm, sp, sptep))
				kvm_flush_remote_tlbs(vcpu->kvm);

			if (!rmap_can_add(vcpu))
				break;

			if (kvm_read_guest_atomic(vcpu->kvm, pte_gpa, &gpte,
						  sizeof(pt_element_t)))
				break;

			FNAME(update_pte)(vcpu, sp, sptep, &gpte);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		}

		if (!is_shadow_present_pte(*sptep) || !sp->unsync_children)
			break;
	}
<<<<<<< HEAD
<<<<<<< HEAD

	if (need_flush)
		kvm_flush_remote_tlbs(vcpu->kvm);

	atomic_inc(&vcpu->kvm->arch.invlpg_counter);

	spin_unlock(&vcpu->kvm->mmu_lock);

	if (pte_gpa == -1)
		return;

	if (mmu_topup_memory_caches(vcpu))
		return;
	kvm_mmu_pte_write(vcpu, pte_gpa, NULL, sizeof(pt_element_t), 0);
=======
	spin_unlock(&vcpu->kvm->mmu_lock);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	spin_unlock(&vcpu->kvm->mmu_lock);
>>>>>>> refs/remotes/origin/master
}

static gpa_t FNAME(gva_to_gpa)(struct kvm_vcpu *vcpu, gva_t vaddr, u32 access,
			       struct x86_exception *exception)
{
	struct guest_walker walker;
	gpa_t gpa = UNMAPPED_GVA;
	int r;

	r = FNAME(walk_addr)(&walker, vcpu, vaddr, access);

	if (r) {
		gpa = gfn_to_gpa(walker.gfn);
		gpa |= vaddr & ~PAGE_MASK;
	} else if (exception)
		*exception = walker.fault;

	return gpa;
}

<<<<<<< HEAD
=======
#if PTTYPE != PTTYPE_EPT
>>>>>>> refs/remotes/origin/master
static gpa_t FNAME(gva_to_gpa_nested)(struct kvm_vcpu *vcpu, gva_t vaddr,
				      u32 access,
				      struct x86_exception *exception)
{
	struct guest_walker walker;
	gpa_t gpa = UNMAPPED_GVA;
	int r;

	r = FNAME(walk_addr_nested)(&walker, vcpu, vaddr, access);

	if (r) {
		gpa = gfn_to_gpa(walker.gfn);
		gpa |= vaddr & ~PAGE_MASK;
	} else if (exception)
		*exception = walker.fault;

	return gpa;
}
<<<<<<< HEAD

<<<<<<< HEAD
static void FNAME(prefetch_page)(struct kvm_vcpu *vcpu,
				 struct kvm_mmu_page *sp)
{
	int i, j, offset, r;
	pt_element_t pt[256 / sizeof(pt_element_t)];
	gpa_t pte_gpa;

	if (sp->role.direct
	    || (PTTYPE == 32 && sp->role.level > PT_PAGE_TABLE_LEVEL)) {
		nonpaging_prefetch_page(vcpu, sp);
		return;
	}

	pte_gpa = gfn_to_gpa(sp->gfn);
	if (PTTYPE == 32) {
		offset = sp->role.quadrant << PT64_LEVEL_BITS;
		pte_gpa += offset * sizeof(pt_element_t);
	}

	for (i = 0; i < PT64_ENT_PER_PAGE; i += ARRAY_SIZE(pt)) {
		r = kvm_read_guest_atomic(vcpu->kvm, pte_gpa, pt, sizeof pt);
		pte_gpa += ARRAY_SIZE(pt) * sizeof(pt_element_t);
		for (j = 0; j < ARRAY_SIZE(pt); ++j)
			if (r || is_present_gpte(pt[j]))
				sp->spt[i+j] = shadow_trap_nonpresent_pte;
			else
				sp->spt[i+j] = shadow_notrap_nonpresent_pte;
	}
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
#endif

>>>>>>> refs/remotes/origin/master
/*
 * Using the cached information from sp->gfns is safe because:
 * - The spte has a reference to the struct page, so the pfn for a given gfn
 *   can't change unless all sptes pointing to it are nuked first.
 *
 * Note:
 *   We should flush all tlbs if spte is dropped even though guest is
 *   responsible for it. Since if we don't, kvm_mmu_notifier_invalidate_page
 *   and kvm_mmu_notifier_invalidate_range_start detect the mapping page isn't
 *   used by guest then tlbs are not flushed, so guest is allowed to access the
 *   freed pages.
 *   And we increase kvm->tlbs_dirty to delay tlbs flush in this case.
 */
static int FNAME(sync_page)(struct kvm_vcpu *vcpu, struct kvm_mmu_page *sp)
{
<<<<<<< HEAD
<<<<<<< HEAD
	int i, offset, nr_present;
	bool host_writable;
	gpa_t first_pte_gpa;

	offset = nr_present = 0;

	/* direct kvm_mmu_page can not be unsync. */
	BUG_ON(sp->role.direct);

	if (PTTYPE == 32)
		offset = sp->role.quadrant << PT64_LEVEL_BITS;

	first_pte_gpa = gfn_to_gpa(sp->gfn) + offset * sizeof(pt_element_t);
=======
=======
>>>>>>> refs/remotes/origin/master
	int i, nr_present = 0;
	bool host_writable;
	gpa_t first_pte_gpa;

	/* direct kvm_mmu_page can not be unsync. */
	BUG_ON(sp->role.direct);

	first_pte_gpa = FNAME(get_level1_sp_gpa)(sp);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	for (i = 0; i < PT64_ENT_PER_PAGE; i++) {
		unsigned pte_access;
		pt_element_t gpte;
		gpa_t pte_gpa;
		gfn_t gfn;

<<<<<<< HEAD
<<<<<<< HEAD
		if (!is_shadow_present_pte(sp->spt[i]))
=======
		if (!sp->spt[i])
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (!sp->spt[i])
>>>>>>> refs/remotes/origin/master
			continue;

		pte_gpa = first_pte_gpa + i * sizeof(pt_element_t);

		if (kvm_read_guest_atomic(vcpu->kvm, pte_gpa, &gpte,
					  sizeof(pt_element_t)))
			return -EINVAL;

<<<<<<< HEAD
<<<<<<< HEAD
		gfn = gpte_to_gfn(gpte);

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		if (FNAME(prefetch_invalid_gpte)(vcpu, sp, &sp->spt[i], gpte)) {
			vcpu->kvm->tlbs_dirty++;
			continue;
		}

<<<<<<< HEAD
<<<<<<< HEAD
		if (gfn != sp->gfns[i]) {
			drop_spte(vcpu->kvm, &sp->spt[i],
				      shadow_trap_nonpresent_pte);
=======
		gfn = gpte_to_gfn(gpte);
		pte_access = sp->role.access;
		pte_access &= FNAME(gpte_access)(vcpu, gpte, true);

		if (sync_mmio_spte(&sp->spt[i], gfn, pte_access, &nr_present))
=======
		gfn = gpte_to_gfn(gpte);
		pte_access = sp->role.access;
		pte_access &= FNAME(gpte_access)(vcpu, gpte);
		FNAME(protect_clean_gpte)(&pte_access, gpte);

		if (sync_mmio_spte(vcpu->kvm, &sp->spt[i], gfn, pte_access,
		      &nr_present))
>>>>>>> refs/remotes/origin/master
			continue;

		if (gfn != sp->gfns[i]) {
			drop_spte(vcpu->kvm, &sp->spt[i]);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			vcpu->kvm->tlbs_dirty++;
			continue;
		}

		nr_present++;
<<<<<<< HEAD
<<<<<<< HEAD
		pte_access = sp->role.access & FNAME(gpte_access)(vcpu, gpte);
		host_writable = sp->spt[i] & SPTE_HOST_WRITEABLE;

		set_spte(vcpu, &sp->spt[i], pte_access, 0, 0,
			 is_dirty_gpte(gpte), PT_PAGE_TABLE_LEVEL, gfn,
=======

		host_writable = sp->spt[i] & SPTE_HOST_WRITEABLE;

		set_spte(vcpu, &sp->spt[i], pte_access, 0, 0,
			 PT_PAGE_TABLE_LEVEL, gfn,
>>>>>>> refs/remotes/origin/cm-10.0
=======

		host_writable = sp->spt[i] & SPTE_HOST_WRITEABLE;

		set_spte(vcpu, &sp->spt[i], pte_access,
			 PT_PAGE_TABLE_LEVEL, gfn,
>>>>>>> refs/remotes/origin/master
			 spte_to_pfn(sp->spt[i]), true, false,
			 host_writable);
	}

	return !nr_present;
}

#undef pt_element_t
#undef guest_walker
#undef FNAME
#undef PT_BASE_ADDR_MASK
#undef PT_INDEX
#undef PT_LVL_ADDR_MASK
#undef PT_LVL_OFFSET_MASK
#undef PT_LEVEL_BITS
#undef PT_MAX_FULL_LEVELS
#undef gpte_to_gfn
#undef gpte_to_gfn_lvl
#undef CMPXCHG
<<<<<<< HEAD
=======
#undef PT_GUEST_ACCESSED_MASK
#undef PT_GUEST_DIRTY_MASK
#undef PT_GUEST_DIRTY_SHIFT
#undef PT_GUEST_ACCESSED_SHIFT
>>>>>>> refs/remotes/origin/master
