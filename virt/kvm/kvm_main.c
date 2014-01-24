/*
 * Kernel-based Virtual Machine driver for Linux
 *
 * This module enables machines with Intel VT-x extensions to run virtual
 * machines without emulation or binary translation.
 *
 * Copyright (C) 2006 Qumranet, Inc.
 * Copyright 2010 Red Hat, Inc. and/or its affiliates.
 *
 * Authors:
 *   Avi Kivity   <avi@qumranet.com>
 *   Yaniv Kamay  <yaniv@qumranet.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2.  See
 * the COPYING file in the top-level directory.
 *
 */

#include "iodev.h"

#include <linux/kvm_host.h>
#include <linux/kvm.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/percpu.h>
#include <linux/mm.h>
#include <linux/miscdevice.h>
#include <linux/vmalloc.h>
#include <linux/reboot.h>
#include <linux/debugfs.h>
#include <linux/highmem.h>
#include <linux/file.h>
#include <linux/syscore_ops.h>
#include <linux/cpu.h>
#include <linux/sched.h>
#include <linux/cpumask.h>
#include <linux/smp.h>
#include <linux/anon_inodes.h>
#include <linux/profile.h>
#include <linux/kvm_para.h>
#include <linux/pagemap.h>
#include <linux/mman.h>
#include <linux/swap.h>
#include <linux/bitops.h>
#include <linux/spinlock.h>
#include <linux/compat.h>
#include <linux/srcu.h>
#include <linux/hugetlb.h>
#include <linux/slab.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/sort.h>
#include <linux/bsearch.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/sort.h>
#include <linux/bsearch.h>
>>>>>>> refs/remotes/origin/master

#include <asm/processor.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/pgtable.h>

#include "coalesced_mmio.h"
#include "async_pf.h"

#define CREATE_TRACE_POINTS
#include <trace/events/kvm.h>

MODULE_AUTHOR("Qumranet");
MODULE_LICENSE("GPL");

/*
 * Ordering of locks:
 *
 * 		kvm->lock --> kvm->slots_lock --> kvm->irq_lock
 */

<<<<<<< HEAD
DEFINE_RAW_SPINLOCK(kvm_lock);
=======
DEFINE_SPINLOCK(kvm_lock);
static DEFINE_RAW_SPINLOCK(kvm_count_lock);
>>>>>>> refs/remotes/origin/master
LIST_HEAD(vm_list);

static cpumask_var_t cpus_hardware_enabled;
static int kvm_usage_count = 0;
static atomic_t hardware_enable_failed;

struct kmem_cache *kvm_vcpu_cache;
EXPORT_SYMBOL_GPL(kvm_vcpu_cache);

static __read_mostly struct preempt_ops kvm_preempt_ops;

struct dentry *kvm_debugfs_dir;

static long kvm_vcpu_ioctl(struct file *file, unsigned int ioctl,
			   unsigned long arg);
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_COMPAT
static long kvm_vcpu_compat_ioctl(struct file *file, unsigned int ioctl,
				  unsigned long arg);
#endif
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static int hardware_enable_all(void);
static void hardware_disable_all(void);

static void kvm_io_bus_destroy(struct kvm_io_bus *bus);

bool kvm_rebooting;
EXPORT_SYMBOL_GPL(kvm_rebooting);

static bool largepages_enabled = true;

<<<<<<< HEAD
static struct page *hwpoison_page;
static pfn_t hwpoison_pfn;

<<<<<<< HEAD
static struct page *fault_page;
static pfn_t fault_pfn;
=======
struct page *fault_page;
pfn_t fault_pfn;
>>>>>>> refs/remotes/origin/cm-10.0

inline int kvm_is_mmio_pfn(pfn_t pfn)
{
	if (pfn_valid(pfn)) {
		int reserved;
		struct page *tail = pfn_to_page(pfn);
		struct page *head = compound_trans_head(tail);
		reserved = PageReserved(head);
		if (head != tail) {
			/*
			 * "head" is not a dangling pointer
			 * (compound_trans_head takes care of that)
			 * but the hugepage may have been splitted
			 * from under us (and we may not hold a
			 * reference count on the head page so it can
			 * be reused before we run PageReferenced), so
			 * we've to check PageTail before returning
			 * what we just read.
			 */
			smp_rmb();
			if (PageTail(tail))
				return reserved;
		}
		return PageReserved(tail);
	}
=======
bool kvm_is_mmio_pfn(pfn_t pfn)
{
	if (pfn_valid(pfn))
		return PageReserved(pfn_to_page(pfn));
>>>>>>> refs/remotes/origin/master

	return true;
}

/*
 * Switches to specified vcpu, until a matching vcpu_put()
 */
<<<<<<< HEAD
void vcpu_load(struct kvm_vcpu *vcpu)
{
	int cpu;

	mutex_lock(&vcpu->mutex);
=======
int vcpu_load(struct kvm_vcpu *vcpu)
{
	int cpu;

	if (mutex_lock_killable(&vcpu->mutex))
		return -EINTR;
>>>>>>> refs/remotes/origin/master
	if (unlikely(vcpu->pid != current->pids[PIDTYPE_PID].pid)) {
		/* The thread running this VCPU changed. */
		struct pid *oldpid = vcpu->pid;
		struct pid *newpid = get_task_pid(current, PIDTYPE_PID);
		rcu_assign_pointer(vcpu->pid, newpid);
		synchronize_rcu();
		put_pid(oldpid);
	}
	cpu = get_cpu();
	preempt_notifier_register(&vcpu->preempt_notifier);
	kvm_arch_vcpu_load(vcpu, cpu);
	put_cpu();
<<<<<<< HEAD
=======
	return 0;
>>>>>>> refs/remotes/origin/master
}

void vcpu_put(struct kvm_vcpu *vcpu)
{
	preempt_disable();
	kvm_arch_vcpu_put(vcpu);
	preempt_notifier_unregister(&vcpu->preempt_notifier);
	preempt_enable();
	mutex_unlock(&vcpu->mutex);
}

static void ack_flush(void *_completed)
{
}

static bool make_all_cpus_request(struct kvm *kvm, unsigned int req)
{
	int i, cpu, me;
	cpumask_var_t cpus;
	bool called = true;
	struct kvm_vcpu *vcpu;

	zalloc_cpumask_var(&cpus, GFP_ATOMIC);

	me = get_cpu();
	kvm_for_each_vcpu(i, vcpu, kvm) {
		kvm_make_request(req, vcpu);
		cpu = vcpu->cpu;

		/* Set ->requests bit before we read ->mode */
		smp_mb();

		if (cpus != NULL && cpu != -1 && cpu != me &&
		      kvm_vcpu_exiting_guest_mode(vcpu) != OUTSIDE_GUEST_MODE)
			cpumask_set_cpu(cpu, cpus);
	}
	if (unlikely(cpus == NULL))
		smp_call_function_many(cpu_online_mask, ack_flush, NULL, 1);
	else if (!cpumask_empty(cpus))
		smp_call_function_many(cpus, ack_flush, NULL, 1);
	else
		called = false;
	put_cpu();
	free_cpumask_var(cpus);
	return called;
}

void kvm_flush_remote_tlbs(struct kvm *kvm)
{
<<<<<<< HEAD
<<<<<<< HEAD
	int dirty_count = kvm->tlbs_dirty;
=======
	long dirty_count = kvm->tlbs_dirty;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	long dirty_count = kvm->tlbs_dirty;
>>>>>>> refs/remotes/origin/master

	smp_mb();
	if (make_all_cpus_request(kvm, KVM_REQ_TLB_FLUSH))
		++kvm->stat.remote_tlb_flush;
	cmpxchg(&kvm->tlbs_dirty, dirty_count, 0);
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(kvm_flush_remote_tlbs);
>>>>>>> refs/remotes/origin/master

void kvm_reload_remote_mmus(struct kvm *kvm)
{
	make_all_cpus_request(kvm, KVM_REQ_MMU_RELOAD);
}

<<<<<<< HEAD
=======
void kvm_make_mclock_inprogress_request(struct kvm *kvm)
{
	make_all_cpus_request(kvm, KVM_REQ_MCLOCK_INPROGRESS);
}

void kvm_make_scan_ioapic_request(struct kvm *kvm)
{
	make_all_cpus_request(kvm, KVM_REQ_SCAN_IOAPIC);
}

>>>>>>> refs/remotes/origin/master
int kvm_vcpu_init(struct kvm_vcpu *vcpu, struct kvm *kvm, unsigned id)
{
	struct page *page;
	int r;

	mutex_init(&vcpu->mutex);
	vcpu->cpu = -1;
	vcpu->kvm = kvm;
	vcpu->vcpu_id = id;
	vcpu->pid = NULL;
	init_waitqueue_head(&vcpu->wq);
	kvm_async_pf_vcpu_init(vcpu);

	page = alloc_page(GFP_KERNEL | __GFP_ZERO);
	if (!page) {
		r = -ENOMEM;
		goto fail;
	}
	vcpu->run = page_address(page);

<<<<<<< HEAD
=======
	kvm_vcpu_set_in_spin_loop(vcpu, false);
	kvm_vcpu_set_dy_eligible(vcpu, false);
	vcpu->preempted = false;

>>>>>>> refs/remotes/origin/master
	r = kvm_arch_vcpu_init(vcpu);
	if (r < 0)
		goto fail_free_run;
	return 0;

fail_free_run:
	free_page((unsigned long)vcpu->run);
fail:
	return r;
}
EXPORT_SYMBOL_GPL(kvm_vcpu_init);

void kvm_vcpu_uninit(struct kvm_vcpu *vcpu)
{
	put_pid(vcpu->pid);
	kvm_arch_vcpu_uninit(vcpu);
	free_page((unsigned long)vcpu->run);
}
EXPORT_SYMBOL_GPL(kvm_vcpu_uninit);

#if defined(CONFIG_MMU_NOTIFIER) && defined(KVM_ARCH_WANT_MMU_NOTIFIER)
static inline struct kvm *mmu_notifier_to_kvm(struct mmu_notifier *mn)
{
	return container_of(mn, struct kvm, mmu_notifier);
}

static void kvm_mmu_notifier_invalidate_page(struct mmu_notifier *mn,
					     struct mm_struct *mm,
					     unsigned long address)
{
	struct kvm *kvm = mmu_notifier_to_kvm(mn);
	int need_tlb_flush, idx;

	/*
	 * When ->invalidate_page runs, the linux pte has been zapped
	 * already but the page is still allocated until
	 * ->invalidate_page returns. So if we increase the sequence
	 * here the kvm page fault will notice if the spte can't be
	 * established because the page is going to be freed. If
	 * instead the kvm page fault establishes the spte before
	 * ->invalidate_page runs, kvm_unmap_hva will release it
	 * before returning.
	 *
	 * The sequence increase only need to be seen at spin_unlock
	 * time, and not at spin_lock time.
	 *
	 * Increasing the sequence after the spin_unlock would be
	 * unsafe because the kvm page fault could then establish the
	 * pte after kvm_unmap_hva returned, without noticing the page
	 * is going to be freed.
	 */
	idx = srcu_read_lock(&kvm->srcu);
	spin_lock(&kvm->mmu_lock);
<<<<<<< HEAD
<<<<<<< HEAD
	kvm->mmu_notifier_seq++;
	need_tlb_flush = kvm_unmap_hva(kvm, address) | kvm->tlbs_dirty;
	spin_unlock(&kvm->mmu_lock);
	srcu_read_unlock(&kvm->srcu, idx);

=======

	kvm->mmu_notifier_seq++;
	need_tlb_flush = kvm_unmap_hva(kvm, address) | kvm->tlbs_dirty;
>>>>>>> refs/remotes/origin/cm-10.0
=======

	kvm->mmu_notifier_seq++;
	need_tlb_flush = kvm_unmap_hva(kvm, address) | kvm->tlbs_dirty;
>>>>>>> refs/remotes/origin/master
	/* we've to flush the tlb before the pages can be freed */
	if (need_tlb_flush)
		kvm_flush_remote_tlbs(kvm);

<<<<<<< HEAD
<<<<<<< HEAD
=======
	spin_unlock(&kvm->mmu_lock);
	srcu_read_unlock(&kvm->srcu, idx);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	spin_unlock(&kvm->mmu_lock);
	srcu_read_unlock(&kvm->srcu, idx);
>>>>>>> refs/remotes/origin/master
}

static void kvm_mmu_notifier_change_pte(struct mmu_notifier *mn,
					struct mm_struct *mm,
					unsigned long address,
					pte_t pte)
{
	struct kvm *kvm = mmu_notifier_to_kvm(mn);
	int idx;

	idx = srcu_read_lock(&kvm->srcu);
	spin_lock(&kvm->mmu_lock);
	kvm->mmu_notifier_seq++;
	kvm_set_spte_hva(kvm, address, pte);
	spin_unlock(&kvm->mmu_lock);
	srcu_read_unlock(&kvm->srcu, idx);
}

static void kvm_mmu_notifier_invalidate_range_start(struct mmu_notifier *mn,
						    struct mm_struct *mm,
						    unsigned long start,
						    unsigned long end)
{
	struct kvm *kvm = mmu_notifier_to_kvm(mn);
	int need_tlb_flush = 0, idx;

	idx = srcu_read_lock(&kvm->srcu);
	spin_lock(&kvm->mmu_lock);
	/*
	 * The count increase must become visible at unlock time as no
	 * spte can be established without taking the mmu_lock and
	 * count is also read inside the mmu_lock critical section.
	 */
	kvm->mmu_notifier_count++;
<<<<<<< HEAD
	for (; start < end; start += PAGE_SIZE)
		need_tlb_flush |= kvm_unmap_hva(kvm, start);
	need_tlb_flush |= kvm->tlbs_dirty;
<<<<<<< HEAD
	spin_unlock(&kvm->mmu_lock);
	srcu_read_unlock(&kvm->srcu, idx);

	/* we've to flush the tlb before the pages can be freed */
	if (need_tlb_flush)
		kvm_flush_remote_tlbs(kvm);
=======
=======
	need_tlb_flush = kvm_unmap_hva_range(kvm, start, end);
	need_tlb_flush |= kvm->tlbs_dirty;
>>>>>>> refs/remotes/origin/master
	/* we've to flush the tlb before the pages can be freed */
	if (need_tlb_flush)
		kvm_flush_remote_tlbs(kvm);

	spin_unlock(&kvm->mmu_lock);
	srcu_read_unlock(&kvm->srcu, idx);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static void kvm_mmu_notifier_invalidate_range_end(struct mmu_notifier *mn,
						  struct mm_struct *mm,
						  unsigned long start,
						  unsigned long end)
{
	struct kvm *kvm = mmu_notifier_to_kvm(mn);

	spin_lock(&kvm->mmu_lock);
	/*
	 * This sequence increase will notify the kvm page fault that
	 * the page that is going to be mapped in the spte could have
	 * been freed.
	 */
	kvm->mmu_notifier_seq++;
<<<<<<< HEAD
<<<<<<< HEAD
	/*
	 * The above sequence increase must be visible before the
	 * below count decrease but both values are read by the kvm
	 * page fault under mmu_lock spinlock so we don't need to add
	 * a smb_wmb() here in between the two.
=======
=======
>>>>>>> refs/remotes/origin/master
	smp_wmb();
	/*
	 * The above sequence increase must be visible before the
	 * below count decrease, which is ensured by the smp_wmb above
	 * in conjunction with the smp_rmb in mmu_notifier_retry().
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	 */
	kvm->mmu_notifier_count--;
	spin_unlock(&kvm->mmu_lock);

	BUG_ON(kvm->mmu_notifier_count < 0);
}

static int kvm_mmu_notifier_clear_flush_young(struct mmu_notifier *mn,
					      struct mm_struct *mm,
					      unsigned long address)
{
	struct kvm *kvm = mmu_notifier_to_kvm(mn);
	int young, idx;

	idx = srcu_read_lock(&kvm->srcu);
	spin_lock(&kvm->mmu_lock);
<<<<<<< HEAD
<<<<<<< HEAD
	young = kvm_age_hva(kvm, address);
	spin_unlock(&kvm->mmu_lock);
	srcu_read_unlock(&kvm->srcu, idx);

	if (young)
		kvm_flush_remote_tlbs(kvm);

=======
=======
>>>>>>> refs/remotes/origin/master

	young = kvm_age_hva(kvm, address);
	if (young)
		kvm_flush_remote_tlbs(kvm);

	spin_unlock(&kvm->mmu_lock);
	srcu_read_unlock(&kvm->srcu, idx);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return young;
}

static int kvm_mmu_notifier_test_young(struct mmu_notifier *mn,
				       struct mm_struct *mm,
				       unsigned long address)
{
	struct kvm *kvm = mmu_notifier_to_kvm(mn);
	int young, idx;

	idx = srcu_read_lock(&kvm->srcu);
	spin_lock(&kvm->mmu_lock);
	young = kvm_test_age_hva(kvm, address);
	spin_unlock(&kvm->mmu_lock);
	srcu_read_unlock(&kvm->srcu, idx);

	return young;
}

static void kvm_mmu_notifier_release(struct mmu_notifier *mn,
				     struct mm_struct *mm)
{
	struct kvm *kvm = mmu_notifier_to_kvm(mn);
	int idx;

	idx = srcu_read_lock(&kvm->srcu);
<<<<<<< HEAD
	kvm_arch_flush_shadow(kvm);
=======
	kvm_arch_flush_shadow_all(kvm);
>>>>>>> refs/remotes/origin/master
	srcu_read_unlock(&kvm->srcu, idx);
}

static const struct mmu_notifier_ops kvm_mmu_notifier_ops = {
	.invalidate_page	= kvm_mmu_notifier_invalidate_page,
	.invalidate_range_start	= kvm_mmu_notifier_invalidate_range_start,
	.invalidate_range_end	= kvm_mmu_notifier_invalidate_range_end,
	.clear_flush_young	= kvm_mmu_notifier_clear_flush_young,
	.test_young		= kvm_mmu_notifier_test_young,
	.change_pte		= kvm_mmu_notifier_change_pte,
	.release		= kvm_mmu_notifier_release,
};

static int kvm_init_mmu_notifier(struct kvm *kvm)
{
	kvm->mmu_notifier.ops = &kvm_mmu_notifier_ops;
	return mmu_notifier_register(&kvm->mmu_notifier, current->mm);
}

#else  /* !(CONFIG_MMU_NOTIFIER && KVM_ARCH_WANT_MMU_NOTIFIER) */

static int kvm_init_mmu_notifier(struct kvm *kvm)
{
	return 0;
}

#endif /* CONFIG_MMU_NOTIFIER && KVM_ARCH_WANT_MMU_NOTIFIER */

<<<<<<< HEAD
<<<<<<< HEAD
static struct kvm *kvm_create_vm(void)
=======
=======
>>>>>>> refs/remotes/origin/master
static void kvm_init_memslots_id(struct kvm *kvm)
{
	int i;
	struct kvm_memslots *slots = kvm->memslots;

	for (i = 0; i < KVM_MEM_SLOTS_NUM; i++)
		slots->id_to_index[i] = slots->memslots[i].id = i;
}

static struct kvm *kvm_create_vm(unsigned long type)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	int r, i;
	struct kvm *kvm = kvm_arch_alloc_vm();

	if (!kvm)
		return ERR_PTR(-ENOMEM);

<<<<<<< HEAD
<<<<<<< HEAD
	r = kvm_arch_init_vm(kvm);
=======
	r = kvm_arch_init_vm(kvm, type);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	r = kvm_arch_init_vm(kvm, type);
>>>>>>> refs/remotes/origin/master
	if (r)
		goto out_err_nodisable;

	r = hardware_enable_all();
	if (r)
		goto out_err_nodisable;

#ifdef CONFIG_HAVE_KVM_IRQCHIP
	INIT_HLIST_HEAD(&kvm->mask_notifier_list);
	INIT_HLIST_HEAD(&kvm->irq_ack_notifier_list);
#endif

<<<<<<< HEAD
=======
	BUILD_BUG_ON(KVM_MEM_SLOTS_NUM > SHRT_MAX);

>>>>>>> refs/remotes/origin/master
	r = -ENOMEM;
	kvm->memslots = kzalloc(sizeof(struct kvm_memslots), GFP_KERNEL);
	if (!kvm->memslots)
		goto out_err_nosrcu;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	kvm_init_memslots_id(kvm);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	kvm_init_memslots_id(kvm);
>>>>>>> refs/remotes/origin/master
	if (init_srcu_struct(&kvm->srcu))
		goto out_err_nosrcu;
	for (i = 0; i < KVM_NR_BUSES; i++) {
		kvm->buses[i] = kzalloc(sizeof(struct kvm_io_bus),
					GFP_KERNEL);
		if (!kvm->buses[i])
			goto out_err;
	}

	spin_lock_init(&kvm->mmu_lock);
	kvm->mm = current->mm;
	atomic_inc(&kvm->mm->mm_count);
	kvm_eventfd_init(kvm);
	mutex_init(&kvm->lock);
	mutex_init(&kvm->irq_lock);
	mutex_init(&kvm->slots_lock);
	atomic_set(&kvm->users_count, 1);
<<<<<<< HEAD
=======
	INIT_LIST_HEAD(&kvm->devices);
>>>>>>> refs/remotes/origin/master

	r = kvm_init_mmu_notifier(kvm);
	if (r)
		goto out_err;

<<<<<<< HEAD
	raw_spin_lock(&kvm_lock);
	list_add(&kvm->vm_list, &vm_list);
	raw_spin_unlock(&kvm_lock);
=======
	spin_lock(&kvm_lock);
	list_add(&kvm->vm_list, &vm_list);
	spin_unlock(&kvm_lock);
>>>>>>> refs/remotes/origin/master

	return kvm;

out_err:
	cleanup_srcu_struct(&kvm->srcu);
out_err_nosrcu:
	hardware_disable_all();
out_err_nodisable:
	for (i = 0; i < KVM_NR_BUSES; i++)
		kfree(kvm->buses[i]);
	kfree(kvm->memslots);
	kvm_arch_free_vm(kvm);
	return ERR_PTR(r);
}

<<<<<<< HEAD
=======
/*
 * Avoid using vmalloc for a small buffer.
 * Should not be used when the size is statically known.
 */
void *kvm_kvzalloc(unsigned long size)
{
	if (size > PAGE_SIZE)
		return vzalloc(size);
	else
		return kzalloc(size, GFP_KERNEL);
}

void kvm_kvfree(const void *addr)
{
	if (is_vmalloc_addr(addr))
		vfree(addr);
	else
		kfree(addr);
}

>>>>>>> refs/remotes/origin/master
static void kvm_destroy_dirty_bitmap(struct kvm_memory_slot *memslot)
{
	if (!memslot->dirty_bitmap)
		return;

<<<<<<< HEAD
	if (2 * kvm_dirty_bitmap_bytes(memslot) > PAGE_SIZE)
		vfree(memslot->dirty_bitmap_head);
	else
		kfree(memslot->dirty_bitmap_head);

	memslot->dirty_bitmap = NULL;
	memslot->dirty_bitmap_head = NULL;
=======
	kvm_kvfree(memslot->dirty_bitmap);
	memslot->dirty_bitmap = NULL;
>>>>>>> refs/remotes/origin/master
}

/*
 * Free any memory in @free but not in @dont.
 */
<<<<<<< HEAD
static void kvm_free_physmem_slot(struct kvm_memory_slot *free,
				  struct kvm_memory_slot *dont)
{
<<<<<<< HEAD
	int i;

=======
>>>>>>> refs/remotes/origin/cm-10.0
	if (!dont || free->rmap != dont->rmap)
		vfree(free->rmap);

	if (!dont || free->dirty_bitmap != dont->dirty_bitmap)
		kvm_destroy_dirty_bitmap(free);

<<<<<<< HEAD

	for (i = 0; i < KVM_NR_PAGE_SIZES - 1; ++i) {
		if (!dont || free->lpage_info[i] != dont->lpage_info[i]) {
			vfree(free->lpage_info[i]);
			free->lpage_info[i] = NULL;
		}
	}
=======
	kvm_arch_free_memslot(free, dont);
>>>>>>> refs/remotes/origin/cm-10.0

	free->npages = 0;
	free->rmap = NULL;
=======
static void kvm_free_physmem_slot(struct kvm *kvm, struct kvm_memory_slot *free,
				  struct kvm_memory_slot *dont)
{
	if (!dont || free->dirty_bitmap != dont->dirty_bitmap)
		kvm_destroy_dirty_bitmap(free);

	kvm_arch_free_memslot(kvm, free, dont);

	free->npages = 0;
>>>>>>> refs/remotes/origin/master
}

void kvm_free_physmem(struct kvm *kvm)
{
<<<<<<< HEAD
<<<<<<< HEAD
	int i;
	struct kvm_memslots *slots = kvm->memslots;

	for (i = 0; i < slots->nmemslots; ++i)
		kvm_free_physmem_slot(&slots->memslots[i], NULL);
=======
=======
>>>>>>> refs/remotes/origin/master
	struct kvm_memslots *slots = kvm->memslots;
	struct kvm_memory_slot *memslot;

	kvm_for_each_memslot(memslot, slots)
<<<<<<< HEAD
		kvm_free_physmem_slot(memslot, NULL);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		kvm_free_physmem_slot(kvm, memslot, NULL);
>>>>>>> refs/remotes/origin/master

	kfree(kvm->memslots);
}

<<<<<<< HEAD
=======
static void kvm_destroy_devices(struct kvm *kvm)
{
	struct list_head *node, *tmp;

	list_for_each_safe(node, tmp, &kvm->devices) {
		struct kvm_device *dev =
			list_entry(node, struct kvm_device, vm_node);

		list_del(node);
		dev->ops->destroy(dev);
	}
}

>>>>>>> refs/remotes/origin/master
static void kvm_destroy_vm(struct kvm *kvm)
{
	int i;
	struct mm_struct *mm = kvm->mm;

	kvm_arch_sync_events(kvm);
<<<<<<< HEAD
	raw_spin_lock(&kvm_lock);
	list_del(&kvm->vm_list);
	raw_spin_unlock(&kvm_lock);
=======
	spin_lock(&kvm_lock);
	list_del(&kvm->vm_list);
	spin_unlock(&kvm_lock);
>>>>>>> refs/remotes/origin/master
	kvm_free_irq_routing(kvm);
	for (i = 0; i < KVM_NR_BUSES; i++)
		kvm_io_bus_destroy(kvm->buses[i]);
	kvm_coalesced_mmio_free(kvm);
#if defined(CONFIG_MMU_NOTIFIER) && defined(KVM_ARCH_WANT_MMU_NOTIFIER)
	mmu_notifier_unregister(&kvm->mmu_notifier, kvm->mm);
#else
<<<<<<< HEAD
	kvm_arch_flush_shadow(kvm);
#endif
	kvm_arch_destroy_vm(kvm);
=======
	kvm_arch_flush_shadow_all(kvm);
#endif
	kvm_arch_destroy_vm(kvm);
	kvm_destroy_devices(kvm);
>>>>>>> refs/remotes/origin/master
	kvm_free_physmem(kvm);
	cleanup_srcu_struct(&kvm->srcu);
	kvm_arch_free_vm(kvm);
	hardware_disable_all();
	mmdrop(mm);
}

void kvm_get_kvm(struct kvm *kvm)
{
	atomic_inc(&kvm->users_count);
}
EXPORT_SYMBOL_GPL(kvm_get_kvm);

void kvm_put_kvm(struct kvm *kvm)
{
	if (atomic_dec_and_test(&kvm->users_count))
		kvm_destroy_vm(kvm);
}
EXPORT_SYMBOL_GPL(kvm_put_kvm);


static int kvm_vm_release(struct inode *inode, struct file *filp)
{
	struct kvm *kvm = filp->private_data;

	kvm_irqfd_release(kvm);

	kvm_put_kvm(kvm);
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
#ifndef CONFIG_S390
=======
>>>>>>> refs/remotes/origin/cm-10.0
/*
 * Allocation size is twice as large as the actual dirty bitmap size.
 * This makes it possible to do double buffering: see x86's
 * kvm_vm_ioctl_get_dirty_log().
 */
static int kvm_create_dirty_bitmap(struct kvm_memory_slot *memslot)
{
<<<<<<< HEAD
=======
#ifndef CONFIG_S390
>>>>>>> refs/remotes/origin/cm-10.0
	unsigned long dirty_bytes = 2 * kvm_dirty_bitmap_bytes(memslot);

	if (dirty_bytes > PAGE_SIZE)
		memslot->dirty_bitmap = vzalloc(dirty_bytes);
	else
		memslot->dirty_bitmap = kzalloc(dirty_bytes, GFP_KERNEL);

	if (!memslot->dirty_bitmap)
		return -ENOMEM;

	memslot->dirty_bitmap_head = memslot->dirty_bitmap;
<<<<<<< HEAD
	return 0;
}
#endif /* !CONFIG_S390 */
=======
	memslot->nr_dirty_pages = 0;
=======
/*
 * Allocation size is twice as large as the actual dirty bitmap size.
 * See x86's kvm_vm_ioctl_get_dirty_log() why this is needed.
 */
static int kvm_create_dirty_bitmap(struct kvm_memory_slot *memslot)
{
#ifndef CONFIG_S390
	unsigned long dirty_bytes = 2 * kvm_dirty_bitmap_bytes(memslot);

	memslot->dirty_bitmap = kvm_kvzalloc(dirty_bytes);
	if (!memslot->dirty_bitmap)
		return -ENOMEM;

>>>>>>> refs/remotes/origin/master
#endif /* !CONFIG_S390 */
	return 0;
}

static int cmp_memslot(const void *slot1, const void *slot2)
{
	struct kvm_memory_slot *s1, *s2;

	s1 = (struct kvm_memory_slot *)slot1;
	s2 = (struct kvm_memory_slot *)slot2;

	if (s1->npages < s2->npages)
		return 1;
	if (s1->npages > s2->npages)
		return -1;

	return 0;
}

/*
 * Sort the memslots base on its size, so the larger slots
 * will get better fit.
 */
static void sort_memslots(struct kvm_memslots *slots)
{
	int i;

	sort(slots->memslots, KVM_MEM_SLOTS_NUM,
	      sizeof(struct kvm_memory_slot), cmp_memslot, NULL);

	for (i = 0; i < KVM_MEM_SLOTS_NUM; i++)
		slots->id_to_index[slots->memslots[i].id] = i;
}

<<<<<<< HEAD
void update_memslots(struct kvm_memslots *slots, struct kvm_memory_slot *new)
=======
void update_memslots(struct kvm_memslots *slots, struct kvm_memory_slot *new,
		     u64 last_generation)
>>>>>>> refs/remotes/origin/master
{
	if (new) {
		int id = new->id;
		struct kvm_memory_slot *old = id_to_memslot(slots, id);
		unsigned long npages = old->npages;

		*old = *new;
		if (new->npages != npages)
			sort_memslots(slots);
	}

<<<<<<< HEAD
	slots->generation++;
}
>>>>>>> refs/remotes/origin/cm-10.0
=======
	slots->generation = last_generation + 1;
}

static int check_memory_region_flags(struct kvm_userspace_memory_region *mem)
{
	u32 valid_flags = KVM_MEM_LOG_DIRTY_PAGES;

#ifdef KVM_CAP_READONLY_MEM
	valid_flags |= KVM_MEM_READONLY;
#endif

	if (mem->flags & ~valid_flags)
		return -EINVAL;

	return 0;
}

static struct kvm_memslots *install_new_memslots(struct kvm *kvm,
		struct kvm_memslots *slots, struct kvm_memory_slot *new)
{
	struct kvm_memslots *old_memslots = kvm->memslots;

	update_memslots(slots, new, kvm->memslots->generation);
	rcu_assign_pointer(kvm->memslots, slots);
	synchronize_srcu_expedited(&kvm->srcu);

	kvm_arch_memslots_updated(kvm);

	return old_memslots;
}
>>>>>>> refs/remotes/origin/master

/*
 * Allocate some memory and give it an address in the guest physical address
 * space.
 *
 * Discontiguous memory is allowed, mostly for framebuffers.
 *
 * Must be called holding mmap_sem for write.
 */
int __kvm_set_memory_region(struct kvm *kvm,
<<<<<<< HEAD
			    struct kvm_userspace_memory_region *mem,
			    int user_alloc)
=======
			    struct kvm_userspace_memory_region *mem)
>>>>>>> refs/remotes/origin/master
{
	int r;
	gfn_t base_gfn;
	unsigned long npages;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long i;
	struct kvm_memory_slot *memslot;
=======
	struct kvm_memory_slot *memslot, *slot;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct kvm_memory_slot *memslot, *slot;
>>>>>>> refs/remotes/origin/cm-11.0
	struct kvm_memory_slot old, new;
	struct kvm_memslots *slots, *old_memslots;
=======
	struct kvm_memory_slot *slot;
	struct kvm_memory_slot old, new;
	struct kvm_memslots *slots = NULL, *old_memslots;
	enum kvm_mr_change change;

	r = check_memory_region_flags(mem);
	if (r)
		goto out;
>>>>>>> refs/remotes/origin/master

	r = -EINVAL;
	/* General sanity checks */
	if (mem->memory_size & (PAGE_SIZE - 1))
		goto out;
	if (mem->guest_phys_addr & (PAGE_SIZE - 1))
		goto out;
	/* We can read the guest memory with __xxx_user() later on. */
<<<<<<< HEAD
	if (user_alloc &&
=======
	if ((mem->slot < KVM_USER_MEM_SLOTS) &&
>>>>>>> refs/remotes/origin/master
	    ((mem->userspace_addr & (PAGE_SIZE - 1)) ||
	     !access_ok(VERIFY_WRITE,
			(void __user *)(unsigned long)mem->userspace_addr,
			mem->memory_size)))
		goto out;
<<<<<<< HEAD
<<<<<<< HEAD
	if (mem->slot >= KVM_MEMORY_SLOTS + KVM_PRIVATE_MEM_SLOTS)
=======
	if (mem->slot >= KVM_MEM_SLOTS_NUM)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (mem->slot >= KVM_MEM_SLOTS_NUM)
>>>>>>> refs/remotes/origin/master
		goto out;
	if (mem->guest_phys_addr + mem->memory_size < mem->guest_phys_addr)
		goto out;

<<<<<<< HEAD
<<<<<<< HEAD
	memslot = &kvm->memslots->memslots[mem->slot];
=======
	memslot = id_to_memslot(kvm->memslots, mem->slot);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	slot = id_to_memslot(kvm->memslots, mem->slot);
>>>>>>> refs/remotes/origin/master
	base_gfn = mem->guest_phys_addr >> PAGE_SHIFT;
	npages = mem->memory_size >> PAGE_SHIFT;

	r = -EINVAL;
	if (npages > KVM_MEM_MAX_NR_PAGES)
		goto out;

	if (!npages)
		mem->flags &= ~KVM_MEM_LOG_DIRTY_PAGES;

<<<<<<< HEAD
	new = old = *memslot;
=======
	new = old = *slot;
>>>>>>> refs/remotes/origin/master

	new.id = mem->slot;
	new.base_gfn = base_gfn;
	new.npages = npages;
	new.flags = mem->flags;

<<<<<<< HEAD
	/* Disallow changing a memory slot's size. */
	r = -EINVAL;
	if (npages && old.npages && npages != old.npages)
		goto out_free;

	/* Check for overlaps */
	r = -EEXIST;
<<<<<<< HEAD
<<<<<<< HEAD
	for (i = 0; i < KVM_MEMORY_SLOTS; ++i) {
		struct kvm_memory_slot *s = &kvm->memslots->memslots[i];

		if (s == memslot || !s->npages)
			continue;
		if (!((base_gfn + npages <= s->base_gfn) ||
		      (base_gfn >= s->base_gfn + s->npages)))
=======
	kvm_for_each_memslot(slot, kvm->memslots) {
		if (slot->id >= KVM_MEMORY_SLOTS || slot == memslot)
			continue;
		if (!((base_gfn + npages <= slot->base_gfn) ||
		      (base_gfn >= slot->base_gfn + slot->npages)))
>>>>>>> refs/remotes/origin/cm-10.0
=======
	kvm_for_each_memslot(slot, kvm->memslots) {
		if (slot->id >= KVM_MEMORY_SLOTS || slot == memslot)
			continue;
		if (!((base_gfn + npages <= slot->base_gfn) ||
		      (base_gfn >= slot->base_gfn + slot->npages)))
>>>>>>> refs/remotes/origin/cm-11.0
			goto out_free;
=======
	r = -EINVAL;
	if (npages) {
		if (!old.npages)
			change = KVM_MR_CREATE;
		else { /* Modify an existing slot. */
			if ((mem->userspace_addr != old.userspace_addr) ||
			    (npages != old.npages) ||
			    ((new.flags ^ old.flags) & KVM_MEM_READONLY))
				goto out;

			if (base_gfn != old.base_gfn)
				change = KVM_MR_MOVE;
			else if (new.flags != old.flags)
				change = KVM_MR_FLAGS_ONLY;
			else { /* Nothing to change. */
				r = 0;
				goto out;
			}
		}
	} else if (old.npages) {
		change = KVM_MR_DELETE;
	} else /* Modify a non-existent slot: disallowed. */
		goto out;

	if ((change == KVM_MR_CREATE) || (change == KVM_MR_MOVE)) {
		/* Check for overlaps */
		r = -EEXIST;
		kvm_for_each_memslot(slot, kvm->memslots) {
			if ((slot->id >= KVM_USER_MEM_SLOTS) ||
			    (slot->id == mem->slot))
				continue;
			if (!((base_gfn + npages <= slot->base_gfn) ||
			      (base_gfn >= slot->base_gfn + slot->npages)))
				goto out;
		}
>>>>>>> refs/remotes/origin/master
	}

	/* Free page dirty bitmap if unneeded */
	if (!(new.flags & KVM_MEM_LOG_DIRTY_PAGES))
		new.dirty_bitmap = NULL;

	r = -ENOMEM;
<<<<<<< HEAD

	/* Allocate if a slot is being created */
<<<<<<< HEAD
#ifndef CONFIG_S390
	if (npages && !new.rmap) {
		new.rmap = vzalloc(npages * sizeof(*new.rmap));

		if (!new.rmap)
			goto out_free;

		new.user_alloc = user_alloc;
		new.userspace_addr = mem->userspace_addr;
	}
	if (!npages)
		goto skip_lpage;

	for (i = 0; i < KVM_NR_PAGE_SIZES - 1; ++i) {
		unsigned long ugfn;
		unsigned long j;
		int lpages;
		int level = i + 2;

		/* Avoid unused variable warning if no large pages */
		(void)level;

		if (new.lpage_info[i])
			continue;

		lpages = 1 + ((base_gfn + npages - 1)
			     >> KVM_HPAGE_GFN_SHIFT(level));
		lpages -= base_gfn >> KVM_HPAGE_GFN_SHIFT(level);

		new.lpage_info[i] = vzalloc(lpages * sizeof(*new.lpage_info[i]));

		if (!new.lpage_info[i])
			goto out_free;

		if (base_gfn & (KVM_PAGES_PER_HPAGE(level) - 1))
			new.lpage_info[i][0].write_count = 1;
		if ((base_gfn+npages) & (KVM_PAGES_PER_HPAGE(level) - 1))
			new.lpage_info[i][lpages - 1].write_count = 1;
		ugfn = new.userspace_addr >> PAGE_SHIFT;
		/*
		 * If the gfn and userspace address are not aligned wrt each
		 * other, or if explicitly asked to, disable large page
		 * support for this slot
		 */
		if ((base_gfn ^ ugfn) & (KVM_PAGES_PER_HPAGE(level) - 1) ||
		    !largepages_enabled)
			for (j = 0; j < lpages; ++j)
				new.lpage_info[i][j].write_count = 1;
	}

skip_lpage:

=======
	if (npages && !old.npages) {
		new.user_alloc = user_alloc;
		new.userspace_addr = mem->userspace_addr;
#ifndef CONFIG_S390
		new.rmap = vzalloc(npages * sizeof(*new.rmap));
		if (!new.rmap)
			goto out_free;
#endif /* not defined CONFIG_S390 */
		if (kvm_arch_create_memslot(&new, npages))
			goto out_free;
	}

>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (change == KVM_MR_CREATE) {
		new.userspace_addr = mem->userspace_addr;

		if (kvm_arch_create_memslot(kvm, &new, npages))
			goto out_free;
	}

>>>>>>> refs/remotes/origin/master
	/* Allocate page dirty bitmap if needed */
	if ((new.flags & KVM_MEM_LOG_DIRTY_PAGES) && !new.dirty_bitmap) {
		if (kvm_create_dirty_bitmap(&new) < 0)
			goto out_free;
<<<<<<< HEAD
		/* destroy any largepage mappings for dirty tracking */
	}
<<<<<<< HEAD
#else  /* not defined CONFIG_S390 */
	new.user_alloc = user_alloc;
	if (user_alloc)
		new.userspace_addr = mem->userspace_addr;
#endif /* not defined CONFIG_S390 */

	if (!npages) {
		r = -ENOMEM;
		slots = kzalloc(sizeof(struct kvm_memslots), GFP_KERNEL);
		if (!slots)
			goto out_free;
		memcpy(slots, kvm->memslots, sizeof(struct kvm_memslots));
		if (mem->slot >= slots->nmemslots)
			slots->nmemslots = mem->slot + 1;
		slots->generation++;
		slots->memslots[mem->slot].flags |= KVM_MEMSLOT_INVALID;
=======

	if (!npages || base_gfn != old.base_gfn) {
		struct kvm_memory_slot *slot;

=======
	}

	if ((change == KVM_MR_DELETE) || (change == KVM_MR_MOVE)) {
>>>>>>> refs/remotes/origin/master
		r = -ENOMEM;
		slots = kmemdup(kvm->memslots, sizeof(struct kvm_memslots),
				GFP_KERNEL);
		if (!slots)
			goto out_free;
		slot = id_to_memslot(slots, mem->slot);
		slot->flags |= KVM_MEMSLOT_INVALID;

<<<<<<< HEAD
		update_memslots(slots, NULL);
>>>>>>> refs/remotes/origin/cm-10.0

		old_memslots = kvm->memslots;
		rcu_assign_pointer(kvm->memslots, slots);
		synchronize_srcu_expedited(&kvm->srcu);
<<<<<<< HEAD
		/* From this point no new shadow pages pointing to a deleted
		 * memslot will be created.
=======
		old_memslots = install_new_memslots(kvm, slots, NULL);

=======
>>>>>>> refs/remotes/origin/cm-11.0
		/* slot was deleted or moved, clear iommu mapping */
		kvm_iommu_unmap_pages(kvm, &old);
		/* From this point no new shadow pages pointing to a deleted,
		 * or moved, memslot will be created.
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
		 *
		 * validation of sp->gfn happens in:
		 * 	- gfn_to_hva (kvm_read_guest, gfn_to_pfn)
		 * 	- kvm_is_visible_gfn (mmu_check_roots)
		 */
<<<<<<< HEAD
		kvm_arch_flush_shadow(kvm);
		kfree(old_memslots);
	}

	r = kvm_arch_prepare_memory_region(kvm, &new, old, mem, user_alloc);
	if (r)
		goto out_free;

	r = -ENOMEM;
<<<<<<< HEAD
	slots = kzalloc(sizeof(struct kvm_memslots), GFP_KERNEL);
	if (!slots)
		goto out_free;
	memcpy(slots, kvm->memslots, sizeof(struct kvm_memslots));
	if (mem->slot >= slots->nmemslots)
		slots->nmemslots = mem->slot + 1;
	slots->generation++;
=======
	slots = kmemdup(kvm->memslots, sizeof(struct kvm_memslots),
			GFP_KERNEL);
	if (!slots)
		goto out_free;
>>>>>>> refs/remotes/origin/cm-10.0

	/* map new memory slot into the iommu */
	if (npages) {
		r = kvm_iommu_map_pages(kvm, &new);
		if (r)
			goto out_slots;
	}

	/* actual memory is freed via old in kvm_free_physmem_slot below */
	if (!npages) {
		new.rmap = NULL;
		new.dirty_bitmap = NULL;
<<<<<<< HEAD
		for (i = 0; i < KVM_NR_PAGE_SIZES - 1; ++i)
			new.lpage_info[i] = NULL;
	}

	slots->memslots[mem->slot] = new;
=======
		memset(&new.arch, 0, sizeof(new.arch));
	}

	update_memslots(slots, &new);
>>>>>>> refs/remotes/origin/cm-10.0
	old_memslots = kvm->memslots;
	rcu_assign_pointer(kvm->memslots, slots);
	synchronize_srcu_expedited(&kvm->srcu);

	kvm_arch_commit_memory_region(kvm, mem, old, user_alloc);

<<<<<<< HEAD
=======
	/*
	 * If the new memory slot is created, we need to clear all
	 * mmio sptes.
	 */
	if (npages && old.base_gfn != mem->guest_phys_addr >> PAGE_SHIFT)
		kvm_arch_flush_shadow(kvm);

>>>>>>> refs/remotes/origin/cm-10.0
	kvm_free_physmem_slot(&old, &new);
	kfree(old_memslots);

	return 0;

out_slots:
	kfree(slots);
out_free:
	kvm_free_physmem_slot(&new, &old);
out:
	return r;

=======
		kvm_arch_flush_shadow_memslot(kvm, slot);
		slots = old_memslots;
	}

	r = kvm_arch_prepare_memory_region(kvm, &new, mem, change);
	if (r)
		goto out_slots;

	r = -ENOMEM;
	/*
	 * We can re-use the old_memslots from above, the only difference
	 * from the currently installed memslots is the invalid flag.  This
	 * will get overwritten by update_memslots anyway.
	 */
	if (!slots) {
		slots = kmemdup(kvm->memslots, sizeof(struct kvm_memslots),
				GFP_KERNEL);
		if (!slots)
			goto out_free;
	}

	/* actual memory is freed via old in kvm_free_physmem_slot below */
	if (change == KVM_MR_DELETE) {
		new.dirty_bitmap = NULL;
		memset(&new.arch, 0, sizeof(new.arch));
	}

	old_memslots = install_new_memslots(kvm, slots, &new);

	kvm_arch_commit_memory_region(kvm, mem, &old, change);

	kvm_free_physmem_slot(kvm, &old, &new);
	kfree(old_memslots);

	/*
	 * IOMMU mapping:  New slots need to be mapped.  Old slots need to be
	 * un-mapped and re-mapped if their base changes.  Since base change
	 * unmapping is handled above with slot deletion, mapping alone is
	 * needed here.  Anything else the iommu might care about for existing
	 * slots (size changes, userspace addr changes and read-only flag
	 * changes) is disallowed above, so any other attribute changes getting
	 * here can be skipped.
	 */
	if ((change == KVM_MR_CREATE) || (change == KVM_MR_MOVE)) {
		r = kvm_iommu_map_pages(kvm, &new);
		return r;
	}

	return 0;

out_slots:
	kfree(slots);
out_free:
	kvm_free_physmem_slot(kvm, &new, &old);
out:
	return r;
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL_GPL(__kvm_set_memory_region);

int kvm_set_memory_region(struct kvm *kvm,
<<<<<<< HEAD
			  struct kvm_userspace_memory_region *mem,
			  int user_alloc)
=======
			  struct kvm_userspace_memory_region *mem)
>>>>>>> refs/remotes/origin/master
{
	int r;

	mutex_lock(&kvm->slots_lock);
<<<<<<< HEAD
	r = __kvm_set_memory_region(kvm, mem, user_alloc);
=======
	r = __kvm_set_memory_region(kvm, mem);
>>>>>>> refs/remotes/origin/master
	mutex_unlock(&kvm->slots_lock);
	return r;
}
EXPORT_SYMBOL_GPL(kvm_set_memory_region);

int kvm_vm_ioctl_set_memory_region(struct kvm *kvm,
<<<<<<< HEAD
				   struct
				   kvm_userspace_memory_region *mem,
				   int user_alloc)
{
	if (mem->slot >= KVM_MEMORY_SLOTS)
		return -EINVAL;
	return kvm_set_memory_region(kvm, mem, user_alloc);
=======
				   struct kvm_userspace_memory_region *mem)
{
	if (mem->slot >= KVM_USER_MEM_SLOTS)
		return -EINVAL;
	return kvm_set_memory_region(kvm, mem);
>>>>>>> refs/remotes/origin/master
}

int kvm_get_dirty_log(struct kvm *kvm,
			struct kvm_dirty_log *log, int *is_dirty)
{
	struct kvm_memory_slot *memslot;
	int r, i;
	unsigned long n;
	unsigned long any = 0;

	r = -EINVAL;
<<<<<<< HEAD
	if (log->slot >= KVM_MEMORY_SLOTS)
		goto out;

<<<<<<< HEAD
	memslot = &kvm->memslots->memslots[log->slot];
=======
	memslot = id_to_memslot(kvm->memslots, log->slot);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (log->slot >= KVM_USER_MEM_SLOTS)
		goto out;

	memslot = id_to_memslot(kvm->memslots, log->slot);
>>>>>>> refs/remotes/origin/master
	r = -ENOENT;
	if (!memslot->dirty_bitmap)
		goto out;

	n = kvm_dirty_bitmap_bytes(memslot);

	for (i = 0; !any && i < n/sizeof(long); ++i)
		any = memslot->dirty_bitmap[i];

	r = -EFAULT;
	if (copy_to_user(log->dirty_bitmap, memslot->dirty_bitmap, n))
		goto out;

	if (any)
		*is_dirty = 1;

	r = 0;
out:
	return r;
}
<<<<<<< HEAD

<<<<<<< HEAD
=======
=======
EXPORT_SYMBOL_GPL(kvm_get_dirty_log);

>>>>>>> refs/remotes/origin/master
bool kvm_largepages_enabled(void)
{
	return largepages_enabled;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
void kvm_disable_largepages(void)
{
	largepages_enabled = false;
}
EXPORT_SYMBOL_GPL(kvm_disable_largepages);

<<<<<<< HEAD
int is_error_page(struct page *page)
{
	return page == bad_page || page == hwpoison_page || page == fault_page;
}
EXPORT_SYMBOL_GPL(is_error_page);

int is_error_pfn(pfn_t pfn)
{
	return pfn == bad_pfn || pfn == hwpoison_pfn || pfn == fault_pfn;
}
EXPORT_SYMBOL_GPL(is_error_pfn);

int is_hwpoison_pfn(pfn_t pfn)
{
	return pfn == hwpoison_pfn;
}
EXPORT_SYMBOL_GPL(is_hwpoison_pfn);

int is_fault_pfn(pfn_t pfn)
{
	return pfn == fault_pfn;
}
EXPORT_SYMBOL_GPL(is_fault_pfn);

<<<<<<< HEAD
=======
int is_noslot_pfn(pfn_t pfn)
{
	return pfn == bad_pfn;
}
EXPORT_SYMBOL_GPL(is_noslot_pfn);

int is_invalid_pfn(pfn_t pfn)
{
	return pfn == hwpoison_pfn || pfn == fault_pfn;
}
EXPORT_SYMBOL_GPL(is_invalid_pfn);

>>>>>>> refs/remotes/origin/cm-10.0
static inline unsigned long bad_hva(void)
{
	return PAGE_OFFSET;
}

int kvm_is_error_hva(unsigned long addr)
{
	return addr == bad_hva();
}
EXPORT_SYMBOL_GPL(kvm_is_error_hva);

<<<<<<< HEAD
static struct kvm_memory_slot *__gfn_to_memslot(struct kvm_memslots *slots,
						gfn_t gfn)
{
	int i;

	for (i = 0; i < slots->nmemslots; ++i) {
		struct kvm_memory_slot *memslot = &slots->memslots[i];

		if (gfn >= memslot->base_gfn
		    && gfn < memslot->base_gfn + memslot->npages)
			return memslot;
	}
	return NULL;
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
struct kvm_memory_slot *gfn_to_memslot(struct kvm *kvm, gfn_t gfn)
{
	return __gfn_to_memslot(kvm_memslots(kvm), gfn);
}
EXPORT_SYMBOL_GPL(gfn_to_memslot);

int kvm_is_visible_gfn(struct kvm *kvm, gfn_t gfn)
{
<<<<<<< HEAD
<<<<<<< HEAD
	int i;
	struct kvm_memslots *slots = kvm_memslots(kvm);

	for (i = 0; i < KVM_MEMORY_SLOTS; ++i) {
		struct kvm_memory_slot *memslot = &slots->memslots[i];

		if (memslot->flags & KVM_MEMSLOT_INVALID)
			continue;

		if (gfn >= memslot->base_gfn
		    && gfn < memslot->base_gfn + memslot->npages)
			return 1;
	}
	return 0;
=======
	struct kvm_memory_slot *memslot = gfn_to_memslot(kvm, gfn);

	if (!memslot || memslot->id >= KVM_MEMORY_SLOTS ||
=======
	struct kvm_memory_slot *memslot = gfn_to_memslot(kvm, gfn);

	if (!memslot || memslot->id >= KVM_USER_MEM_SLOTS ||
>>>>>>> refs/remotes/origin/master
	      memslot->flags & KVM_MEMSLOT_INVALID)
		return 0;

	return 1;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL_GPL(kvm_is_visible_gfn);

unsigned long kvm_host_page_size(struct kvm *kvm, gfn_t gfn)
{
	struct vm_area_struct *vma;
	unsigned long addr, size;

	size = PAGE_SIZE;

	addr = gfn_to_hva(kvm, gfn);
	if (kvm_is_error_hva(addr))
		return PAGE_SIZE;

	down_read(&current->mm->mmap_sem);
	vma = find_vma(current->mm, addr);
	if (!vma)
		goto out;

	size = vma_kernel_pagesize(vma);

out:
	up_read(&current->mm->mmap_sem);

	return size;
}

<<<<<<< HEAD
static unsigned long gfn_to_hva_many(struct kvm_memory_slot *slot, gfn_t gfn,
				     gfn_t *nr_pages)
{
	if (!slot || slot->flags & KVM_MEMSLOT_INVALID)
		return bad_hva();
=======
static bool memslot_is_readonly(struct kvm_memory_slot *slot)
{
	return slot->flags & KVM_MEM_READONLY;
}

static unsigned long __gfn_to_hva_many(struct kvm_memory_slot *slot, gfn_t gfn,
				       gfn_t *nr_pages, bool write)
{
	if (!slot || slot->flags & KVM_MEMSLOT_INVALID)
		return KVM_HVA_ERR_BAD;

	if (memslot_is_readonly(slot) && write)
		return KVM_HVA_ERR_RO_BAD;
>>>>>>> refs/remotes/origin/master

	if (nr_pages)
		*nr_pages = slot->npages - (gfn - slot->base_gfn);

<<<<<<< HEAD
	return gfn_to_hva_memslot(slot, gfn);
}
=======
	return __gfn_to_hva_memslot(slot, gfn);
}

static unsigned long gfn_to_hva_many(struct kvm_memory_slot *slot, gfn_t gfn,
				     gfn_t *nr_pages)
{
	return __gfn_to_hva_many(slot, gfn, nr_pages, true);
}

unsigned long gfn_to_hva_memslot(struct kvm_memory_slot *slot,
				 gfn_t gfn)
{
	return gfn_to_hva_many(slot, gfn, NULL);
}
EXPORT_SYMBOL_GPL(gfn_to_hva_memslot);
>>>>>>> refs/remotes/origin/master

unsigned long gfn_to_hva(struct kvm *kvm, gfn_t gfn)
{
	return gfn_to_hva_many(gfn_to_memslot(kvm, gfn), gfn, NULL);
}
EXPORT_SYMBOL_GPL(gfn_to_hva);

<<<<<<< HEAD
static pfn_t get_fault_pfn(void)
{
	get_page(fault_page);
	return fault_pfn;
}

int get_user_page_nowait(struct task_struct *tsk, struct mm_struct *mm,
=======
/*
 * If writable is set to false, the hva returned by this function is only
 * allowed to be read.
 */
unsigned long gfn_to_hva_prot(struct kvm *kvm, gfn_t gfn, bool *writable)
{
	struct kvm_memory_slot *slot = gfn_to_memslot(kvm, gfn);
	unsigned long hva = __gfn_to_hva_many(slot, gfn, NULL, false);

	if (!kvm_is_error_hva(hva) && writable)
		*writable = !memslot_is_readonly(slot);

	return hva;
}

static int kvm_read_hva(void *data, void __user *hva, int len)
{
	return __copy_from_user(data, hva, len);
}

static int kvm_read_hva_atomic(void *data, void __user *hva, int len)
{
	return __copy_from_user_inatomic(data, hva, len);
}

static int get_user_page_nowait(struct task_struct *tsk, struct mm_struct *mm,
>>>>>>> refs/remotes/origin/master
	unsigned long start, int write, struct page **page)
{
	int flags = FOLL_TOUCH | FOLL_NOWAIT | FOLL_HWPOISON | FOLL_GET;

	if (write)
		flags |= FOLL_WRITE;

	return __get_user_pages(tsk, mm, start, 1, flags, page, NULL, NULL);
}

static inline int check_user_page_hwpoison(unsigned long addr)
{
	int rc, flags = FOLL_TOUCH | FOLL_HWPOISON | FOLL_WRITE;

	rc = __get_user_pages(current, current->mm, addr, 1,
			      flags, NULL, NULL, NULL);
	return rc == -EHWPOISON;
}

<<<<<<< HEAD
static pfn_t hva_to_pfn(struct kvm *kvm, unsigned long addr, bool atomic,
			bool *async, bool write_fault, bool *writable)
{
	struct page *page[1];
	int npages = 0;
	pfn_t pfn;

	/* we can do it either atomically or asynchronously, not both */
	BUG_ON(atomic && async);

	BUG_ON(!write_fault && !writable);

	if (writable)
		*writable = true;

	if (atomic || async)
		npages = __get_user_pages_fast(addr, 1, 1, page);

	if (unlikely(npages != 1) && !atomic) {
		might_sleep();

		if (writable)
			*writable = write_fault;

		if (async) {
			down_read(&current->mm->mmap_sem);
			npages = get_user_page_nowait(current, current->mm,
						     addr, write_fault, page);
			up_read(&current->mm->mmap_sem);
		} else
			npages = get_user_pages_fast(addr, 1, write_fault,
						     page);

		/* map read fault as writable if possible */
		if (unlikely(!write_fault) && npages == 1) {
			struct page *wpage[1];

			npages = __get_user_pages_fast(addr, 1, 1, wpage);
			if (npages == 1) {
				*writable = true;
				put_page(page[0]);
				page[0] = wpage[0];
			}
			npages = 1;
		}
	}

	if (unlikely(npages != 1)) {
		struct vm_area_struct *vma;

		if (atomic)
			return get_fault_pfn();

		down_read(&current->mm->mmap_sem);
		if (npages == -EHWPOISON ||
			(!async && check_user_page_hwpoison(addr))) {
			up_read(&current->mm->mmap_sem);
			get_page(hwpoison_page);
			return page_to_pfn(hwpoison_page);
		}

		vma = find_vma_intersection(current->mm, addr, addr+1);

		if (vma == NULL)
			pfn = get_fault_pfn();
		else if ((vma->vm_flags & VM_PFNMAP)) {
			pfn = ((addr - vma->vm_start) >> PAGE_SHIFT) +
				vma->vm_pgoff;
			BUG_ON(!kvm_is_mmio_pfn(pfn));
		} else {
			if (async && (vma->vm_flags & VM_WRITE))
				*async = true;
			pfn = get_fault_pfn();
		}
		up_read(&current->mm->mmap_sem);
	} else
		pfn = page_to_pfn(page[0]);

	return pfn;
}

pfn_t hva_to_pfn_atomic(struct kvm *kvm, unsigned long addr)
{
	return hva_to_pfn(kvm, addr, true, NULL, true, NULL);
}
EXPORT_SYMBOL_GPL(hva_to_pfn_atomic);

static pfn_t __gfn_to_pfn(struct kvm *kvm, gfn_t gfn, bool atomic, bool *async,
			  bool write_fault, bool *writable)
{
	unsigned long addr;

	if (async)
		*async = false;

	addr = gfn_to_hva(kvm, gfn);
	if (kvm_is_error_hva(addr)) {
		get_page(bad_page);
		return page_to_pfn(bad_page);
	}

	return hva_to_pfn(kvm, addr, atomic, async, write_fault, writable);
=======
/*
 * The atomic path to get the writable pfn which will be stored in @pfn,
 * true indicates success, otherwise false is returned.
 */
static bool hva_to_pfn_fast(unsigned long addr, bool atomic, bool *async,
			    bool write_fault, bool *writable, pfn_t *pfn)
{
	struct page *page[1];
	int npages;

	if (!(async || atomic))
		return false;

	/*
	 * Fast pin a writable pfn only if it is a write fault request
	 * or the caller allows to map a writable pfn for a read fault
	 * request.
	 */
	if (!(write_fault || writable))
		return false;

	npages = __get_user_pages_fast(addr, 1, 1, page);
	if (npages == 1) {
		*pfn = page_to_pfn(page[0]);

		if (writable)
			*writable = true;
		return true;
	}

	return false;
}

/*
 * The slow path to get the pfn of the specified host virtual address,
 * 1 indicates success, -errno is returned if error is detected.
 */
static int hva_to_pfn_slow(unsigned long addr, bool *async, bool write_fault,
			   bool *writable, pfn_t *pfn)
{
	struct page *page[1];
	int npages = 0;

	might_sleep();

	if (writable)
		*writable = write_fault;

	if (async) {
		down_read(&current->mm->mmap_sem);
		npages = get_user_page_nowait(current, current->mm,
					      addr, write_fault, page);
		up_read(&current->mm->mmap_sem);
	} else
		npages = get_user_pages_fast(addr, 1, write_fault,
					     page);
	if (npages != 1)
		return npages;

	/* map read fault as writable if possible */
	if (unlikely(!write_fault) && writable) {
		struct page *wpage[1];

		npages = __get_user_pages_fast(addr, 1, 1, wpage);
		if (npages == 1) {
			*writable = true;
			put_page(page[0]);
			page[0] = wpage[0];
		}

		npages = 1;
	}
	*pfn = page_to_pfn(page[0]);
	return npages;
}

static bool vma_is_valid(struct vm_area_struct *vma, bool write_fault)
{
	if (unlikely(!(vma->vm_flags & VM_READ)))
		return false;

	if (write_fault && (unlikely(!(vma->vm_flags & VM_WRITE))))
		return false;

	return true;
}

/*
 * Pin guest page in memory and return its pfn.
 * @addr: host virtual address which maps memory to the guest
 * @atomic: whether this function can sleep
 * @async: whether this function need to wait IO complete if the
 *         host page is not in the memory
 * @write_fault: whether we should get a writable host page
 * @writable: whether it allows to map a writable host page for !@write_fault
 *
 * The function will map a writable host page for these two cases:
 * 1): @write_fault = true
 * 2): @write_fault = false && @writable, @writable will tell the caller
 *     whether the mapping is writable.
 */
static pfn_t hva_to_pfn(unsigned long addr, bool atomic, bool *async,
			bool write_fault, bool *writable)
{
	struct vm_area_struct *vma;
	pfn_t pfn = 0;
	int npages;

	/* we can do it either atomically or asynchronously, not both */
	BUG_ON(atomic && async);

	if (hva_to_pfn_fast(addr, atomic, async, write_fault, writable, &pfn))
		return pfn;

	if (atomic)
		return KVM_PFN_ERR_FAULT;

	npages = hva_to_pfn_slow(addr, async, write_fault, writable, &pfn);
	if (npages == 1)
		return pfn;

	down_read(&current->mm->mmap_sem);
	if (npages == -EHWPOISON ||
	      (!async && check_user_page_hwpoison(addr))) {
		pfn = KVM_PFN_ERR_HWPOISON;
		goto exit;
	}

	vma = find_vma_intersection(current->mm, addr, addr + 1);

	if (vma == NULL)
		pfn = KVM_PFN_ERR_FAULT;
	else if ((vma->vm_flags & VM_PFNMAP)) {
		pfn = ((addr - vma->vm_start) >> PAGE_SHIFT) +
			vma->vm_pgoff;
		BUG_ON(!kvm_is_mmio_pfn(pfn));
	} else {
		if (async && vma_is_valid(vma, write_fault))
			*async = true;
		pfn = KVM_PFN_ERR_FAULT;
	}
exit:
	up_read(&current->mm->mmap_sem);
	return pfn;
}

static pfn_t
__gfn_to_pfn_memslot(struct kvm_memory_slot *slot, gfn_t gfn, bool atomic,
		     bool *async, bool write_fault, bool *writable)
{
	unsigned long addr = __gfn_to_hva_many(slot, gfn, NULL, write_fault);

	if (addr == KVM_HVA_ERR_RO_BAD)
		return KVM_PFN_ERR_RO_FAULT;

	if (kvm_is_error_hva(addr))
		return KVM_PFN_NOSLOT;

	/* Do not map writable pfn in the readonly memslot. */
	if (writable && memslot_is_readonly(slot)) {
		*writable = false;
		writable = NULL;
	}

	return hva_to_pfn(addr, atomic, async, write_fault,
			  writable);
}

static pfn_t __gfn_to_pfn(struct kvm *kvm, gfn_t gfn, bool atomic, bool *async,
			  bool write_fault, bool *writable)
{
	struct kvm_memory_slot *slot;

	if (async)
		*async = false;

	slot = gfn_to_memslot(kvm, gfn);

	return __gfn_to_pfn_memslot(slot, gfn, atomic, async, write_fault,
				    writable);
>>>>>>> refs/remotes/origin/master
}

pfn_t gfn_to_pfn_atomic(struct kvm *kvm, gfn_t gfn)
{
	return __gfn_to_pfn(kvm, gfn, true, NULL, true, NULL);
}
EXPORT_SYMBOL_GPL(gfn_to_pfn_atomic);

pfn_t gfn_to_pfn_async(struct kvm *kvm, gfn_t gfn, bool *async,
		       bool write_fault, bool *writable)
{
	return __gfn_to_pfn(kvm, gfn, false, async, write_fault, writable);
}
EXPORT_SYMBOL_GPL(gfn_to_pfn_async);

pfn_t gfn_to_pfn(struct kvm *kvm, gfn_t gfn)
{
	return __gfn_to_pfn(kvm, gfn, false, NULL, true, NULL);
}
EXPORT_SYMBOL_GPL(gfn_to_pfn);

pfn_t gfn_to_pfn_prot(struct kvm *kvm, gfn_t gfn, bool write_fault,
		      bool *writable)
{
	return __gfn_to_pfn(kvm, gfn, false, NULL, write_fault, writable);
}
EXPORT_SYMBOL_GPL(gfn_to_pfn_prot);

<<<<<<< HEAD
pfn_t gfn_to_pfn_memslot(struct kvm *kvm,
			 struct kvm_memory_slot *slot, gfn_t gfn)
{
	unsigned long addr = gfn_to_hva_memslot(slot, gfn);
	return hva_to_pfn(kvm, addr, false, NULL, true, NULL);
}
=======
pfn_t gfn_to_pfn_memslot(struct kvm_memory_slot *slot, gfn_t gfn)
{
	return __gfn_to_pfn_memslot(slot, gfn, false, NULL, true, NULL);
}

pfn_t gfn_to_pfn_memslot_atomic(struct kvm_memory_slot *slot, gfn_t gfn)
{
	return __gfn_to_pfn_memslot(slot, gfn, true, NULL, true, NULL);
}
EXPORT_SYMBOL_GPL(gfn_to_pfn_memslot_atomic);
>>>>>>> refs/remotes/origin/master

int gfn_to_page_many_atomic(struct kvm *kvm, gfn_t gfn, struct page **pages,
								  int nr_pages)
{
	unsigned long addr;
	gfn_t entry;

	addr = gfn_to_hva_many(gfn_to_memslot(kvm, gfn), gfn, &entry);
	if (kvm_is_error_hva(addr))
		return -1;

	if (entry < nr_pages)
		return 0;

	return __get_user_pages_fast(addr, nr_pages, 1, pages);
}
EXPORT_SYMBOL_GPL(gfn_to_page_many_atomic);

<<<<<<< HEAD
=======
static struct page *kvm_pfn_to_page(pfn_t pfn)
{
	if (is_error_noslot_pfn(pfn))
		return KVM_ERR_PTR_BAD_PAGE;

	if (kvm_is_mmio_pfn(pfn)) {
		WARN_ON(1);
		return KVM_ERR_PTR_BAD_PAGE;
	}

	return pfn_to_page(pfn);
}

>>>>>>> refs/remotes/origin/master
struct page *gfn_to_page(struct kvm *kvm, gfn_t gfn)
{
	pfn_t pfn;

	pfn = gfn_to_pfn(kvm, gfn);
<<<<<<< HEAD
	if (!kvm_is_mmio_pfn(pfn))
		return pfn_to_page(pfn);

	WARN_ON(kvm_is_mmio_pfn(pfn));

	get_page(bad_page);
	return bad_page;
=======

	return kvm_pfn_to_page(pfn);
>>>>>>> refs/remotes/origin/master
}

EXPORT_SYMBOL_GPL(gfn_to_page);

void kvm_release_page_clean(struct page *page)
{
<<<<<<< HEAD
=======
	WARN_ON(is_error_page(page));

>>>>>>> refs/remotes/origin/master
	kvm_release_pfn_clean(page_to_pfn(page));
}
EXPORT_SYMBOL_GPL(kvm_release_page_clean);

void kvm_release_pfn_clean(pfn_t pfn)
{
<<<<<<< HEAD
	if (!kvm_is_mmio_pfn(pfn))
=======
	if (!is_error_noslot_pfn(pfn) && !kvm_is_mmio_pfn(pfn))
>>>>>>> refs/remotes/origin/master
		put_page(pfn_to_page(pfn));
}
EXPORT_SYMBOL_GPL(kvm_release_pfn_clean);

void kvm_release_page_dirty(struct page *page)
{
<<<<<<< HEAD
=======
	WARN_ON(is_error_page(page));

>>>>>>> refs/remotes/origin/master
	kvm_release_pfn_dirty(page_to_pfn(page));
}
EXPORT_SYMBOL_GPL(kvm_release_page_dirty);

void kvm_release_pfn_dirty(pfn_t pfn)
{
	kvm_set_pfn_dirty(pfn);
	kvm_release_pfn_clean(pfn);
}
EXPORT_SYMBOL_GPL(kvm_release_pfn_dirty);

void kvm_set_page_dirty(struct page *page)
{
	kvm_set_pfn_dirty(page_to_pfn(page));
}
EXPORT_SYMBOL_GPL(kvm_set_page_dirty);

void kvm_set_pfn_dirty(pfn_t pfn)
{
	if (!kvm_is_mmio_pfn(pfn)) {
		struct page *page = pfn_to_page(pfn);
		if (!PageReserved(page))
			SetPageDirty(page);
	}
}
EXPORT_SYMBOL_GPL(kvm_set_pfn_dirty);

void kvm_set_pfn_accessed(pfn_t pfn)
{
	if (!kvm_is_mmio_pfn(pfn))
		mark_page_accessed(pfn_to_page(pfn));
}
EXPORT_SYMBOL_GPL(kvm_set_pfn_accessed);

void kvm_get_pfn(pfn_t pfn)
{
	if (!kvm_is_mmio_pfn(pfn))
		get_page(pfn_to_page(pfn));
}
EXPORT_SYMBOL_GPL(kvm_get_pfn);

static int next_segment(unsigned long len, int offset)
{
	if (len > PAGE_SIZE - offset)
		return PAGE_SIZE - offset;
	else
		return len;
}

int kvm_read_guest_page(struct kvm *kvm, gfn_t gfn, void *data, int offset,
			int len)
{
	int r;
	unsigned long addr;

<<<<<<< HEAD
	addr = gfn_to_hva(kvm, gfn);
	if (kvm_is_error_hva(addr))
		return -EFAULT;
	r = __copy_from_user(data, (void __user *)addr + offset, len);
=======
	addr = gfn_to_hva_prot(kvm, gfn, NULL);
	if (kvm_is_error_hva(addr))
		return -EFAULT;
	r = kvm_read_hva(data, (void __user *)addr + offset, len);
>>>>>>> refs/remotes/origin/master
	if (r)
		return -EFAULT;
	return 0;
}
EXPORT_SYMBOL_GPL(kvm_read_guest_page);

int kvm_read_guest(struct kvm *kvm, gpa_t gpa, void *data, unsigned long len)
{
	gfn_t gfn = gpa >> PAGE_SHIFT;
	int seg;
	int offset = offset_in_page(gpa);
	int ret;

	while ((seg = next_segment(len, offset)) != 0) {
		ret = kvm_read_guest_page(kvm, gfn, data, offset, seg);
		if (ret < 0)
			return ret;
		offset = 0;
		len -= seg;
		data += seg;
		++gfn;
	}
	return 0;
}
EXPORT_SYMBOL_GPL(kvm_read_guest);

int kvm_read_guest_atomic(struct kvm *kvm, gpa_t gpa, void *data,
			  unsigned long len)
{
	int r;
	unsigned long addr;
	gfn_t gfn = gpa >> PAGE_SHIFT;
	int offset = offset_in_page(gpa);

<<<<<<< HEAD
	addr = gfn_to_hva(kvm, gfn);
	if (kvm_is_error_hva(addr))
		return -EFAULT;
	pagefault_disable();
	r = __copy_from_user_inatomic(data, (void __user *)addr + offset, len);
=======
	addr = gfn_to_hva_prot(kvm, gfn, NULL);
	if (kvm_is_error_hva(addr))
		return -EFAULT;
	pagefault_disable();
	r = kvm_read_hva_atomic(data, (void __user *)addr + offset, len);
>>>>>>> refs/remotes/origin/master
	pagefault_enable();
	if (r)
		return -EFAULT;
	return 0;
}
EXPORT_SYMBOL(kvm_read_guest_atomic);

int kvm_write_guest_page(struct kvm *kvm, gfn_t gfn, const void *data,
			 int offset, int len)
{
	int r;
	unsigned long addr;

	addr = gfn_to_hva(kvm, gfn);
	if (kvm_is_error_hva(addr))
		return -EFAULT;
<<<<<<< HEAD
<<<<<<< HEAD
	r = copy_to_user((void __user *)addr + offset, data, len);
=======
	r = __copy_to_user((void __user *)addr + offset, data, len);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	r = __copy_to_user((void __user *)addr + offset, data, len);
>>>>>>> refs/remotes/origin/master
	if (r)
		return -EFAULT;
	mark_page_dirty(kvm, gfn);
	return 0;
}
EXPORT_SYMBOL_GPL(kvm_write_guest_page);

int kvm_write_guest(struct kvm *kvm, gpa_t gpa, const void *data,
		    unsigned long len)
{
	gfn_t gfn = gpa >> PAGE_SHIFT;
	int seg;
	int offset = offset_in_page(gpa);
	int ret;

	while ((seg = next_segment(len, offset)) != 0) {
		ret = kvm_write_guest_page(kvm, gfn, data, offset, seg);
		if (ret < 0)
			return ret;
		offset = 0;
		len -= seg;
		data += seg;
		++gfn;
	}
	return 0;
}

int kvm_gfn_to_hva_cache_init(struct kvm *kvm, struct gfn_to_hva_cache *ghc,
			      gpa_t gpa, unsigned long len)
{
	struct kvm_memslots *slots = kvm_memslots(kvm);
	int offset = offset_in_page(gpa);
	gfn_t start_gfn = gpa >> PAGE_SHIFT;
	gfn_t end_gfn = (gpa + len - 1) >> PAGE_SHIFT;
	gfn_t nr_pages_needed = end_gfn - start_gfn + 1;
	gfn_t nr_pages_avail;

	ghc->gpa = gpa;
	ghc->generation = slots->generation;
	ghc->len = len;
	ghc->memslot = gfn_to_memslot(kvm, start_gfn);
	ghc->hva = gfn_to_hva_many(ghc->memslot, start_gfn, &nr_pages_avail);
	if (!kvm_is_error_hva(ghc->hva) && nr_pages_avail >= nr_pages_needed) {
		ghc->hva += offset;
	} else {
		/*
		 * If the requested region crosses two memslots, we still
		 * verify that the entire region is valid here.
		 */
		while (start_gfn <= end_gfn) {
			ghc->memslot = gfn_to_memslot(kvm, start_gfn);
			ghc->hva = gfn_to_hva_many(ghc->memslot, start_gfn,
						   &nr_pages_avail);
			if (kvm_is_error_hva(ghc->hva))
				return -EFAULT;
			start_gfn += nr_pages_avail;
		}
		/* Use the slow path for cross page reads and writes. */
		ghc->memslot = NULL;
	}
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	return 0;
}
EXPORT_SYMBOL_GPL(kvm_gfn_to_hva_cache_init);

int kvm_write_guest_cached(struct kvm *kvm, struct gfn_to_hva_cache *ghc,
			   void *data, unsigned long len)
{
	struct kvm_memslots *slots = kvm_memslots(kvm);
	int r;

	BUG_ON(len > ghc->len);

	if (slots->generation != ghc->generation)
		kvm_gfn_to_hva_cache_init(kvm, ghc, ghc->gpa, ghc->len);

	if (unlikely(!ghc->memslot))
		return kvm_write_guest(kvm, ghc->gpa, data, len);

	if (kvm_is_error_hva(ghc->hva))
		return -EFAULT;

<<<<<<< HEAD
<<<<<<< HEAD
	r = copy_to_user((void __user *)ghc->hva, data, len);
=======
	r = __copy_to_user((void __user *)ghc->hva, data, len);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	r = __copy_to_user((void __user *)ghc->hva, data, len);
>>>>>>> refs/remotes/origin/master
	if (r)
		return -EFAULT;
	mark_page_dirty_in_slot(kvm, ghc->memslot, ghc->gpa >> PAGE_SHIFT);

	return 0;
}
EXPORT_SYMBOL_GPL(kvm_write_guest_cached);

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
int kvm_read_guest_cached(struct kvm *kvm, struct gfn_to_hva_cache *ghc,
			   void *data, unsigned long len)
{
	struct kvm_memslots *slots = kvm_memslots(kvm);
	int r;

	BUG_ON(len > ghc->len);

	if (slots->generation != ghc->generation)
		kvm_gfn_to_hva_cache_init(kvm, ghc, ghc->gpa, ghc->len);

	if (unlikely(!ghc->memslot))
		return kvm_read_guest(kvm, ghc->gpa, data, len);

	if (kvm_is_error_hva(ghc->hva))
		return -EFAULT;

	r = __copy_from_user(data, (void __user *)ghc->hva, len);
	if (r)
		return -EFAULT;

	return 0;
}
EXPORT_SYMBOL_GPL(kvm_read_guest_cached);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
int kvm_clear_guest_page(struct kvm *kvm, gfn_t gfn, int offset, int len)
{
	return kvm_write_guest_page(kvm, gfn, (const void *) empty_zero_page,
				    offset, len);
=======
int kvm_clear_guest_page(struct kvm *kvm, gfn_t gfn, int offset, int len)
{
	const void *zero_page = (const void *) __va(page_to_phys(ZERO_PAGE(0)));

	return kvm_write_guest_page(kvm, gfn, zero_page, offset, len);
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL_GPL(kvm_clear_guest_page);

int kvm_clear_guest(struct kvm *kvm, gpa_t gpa, unsigned long len)
{
	gfn_t gfn = gpa >> PAGE_SHIFT;
	int seg;
	int offset = offset_in_page(gpa);
	int ret;

        while ((seg = next_segment(len, offset)) != 0) {
		ret = kvm_clear_guest_page(kvm, gfn, offset, seg);
		if (ret < 0)
			return ret;
		offset = 0;
		len -= seg;
		++gfn;
	}
	return 0;
}
EXPORT_SYMBOL_GPL(kvm_clear_guest);

void mark_page_dirty_in_slot(struct kvm *kvm, struct kvm_memory_slot *memslot,
			     gfn_t gfn)
{
	if (memslot && memslot->dirty_bitmap) {
		unsigned long rel_gfn = gfn - memslot->base_gfn;

<<<<<<< HEAD
<<<<<<< HEAD
		__set_bit_le(rel_gfn, memslot->dirty_bitmap);
=======
		if (!test_and_set_bit_le(rel_gfn, memslot->dirty_bitmap))
			memslot->nr_dirty_pages++;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		set_bit_le(rel_gfn, memslot->dirty_bitmap);
>>>>>>> refs/remotes/origin/master
	}
}

void mark_page_dirty(struct kvm *kvm, gfn_t gfn)
{
	struct kvm_memory_slot *memslot;

	memslot = gfn_to_memslot(kvm, gfn);
	mark_page_dirty_in_slot(kvm, memslot, gfn);
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(mark_page_dirty);
>>>>>>> refs/remotes/origin/master

/*
 * The vCPU has executed a HLT instruction with in-kernel mode enabled.
 */
void kvm_vcpu_block(struct kvm_vcpu *vcpu)
{
	DEFINE_WAIT(wait);

	for (;;) {
		prepare_to_wait(&vcpu->wq, &wait, TASK_INTERRUPTIBLE);

		if (kvm_arch_vcpu_runnable(vcpu)) {
			kvm_make_request(KVM_REQ_UNHALT, vcpu);
			break;
		}
		if (kvm_cpu_has_pending_timer(vcpu))
			break;
		if (signal_pending(current))
			break;

		schedule();
	}

	finish_wait(&vcpu->wq, &wait);
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(kvm_vcpu_block);

#ifndef CONFIG_S390
/*
 * Kick a sleeping VCPU, or a guest VCPU in guest mode, into host kernel mode.
 */
void kvm_vcpu_kick(struct kvm_vcpu *vcpu)
{
	int me;
	int cpu = vcpu->cpu;
	wait_queue_head_t *wqp;

	wqp = kvm_arch_vcpu_wq(vcpu);
	if (waitqueue_active(wqp)) {
		wake_up_interruptible(wqp);
		++vcpu->stat.halt_wakeup;
	}

	me = get_cpu();
	if (cpu != me && (unsigned)cpu < nr_cpu_ids && cpu_online(cpu))
		if (kvm_arch_vcpu_should_kick(vcpu))
			smp_send_reschedule(cpu);
	put_cpu();
}
EXPORT_SYMBOL_GPL(kvm_vcpu_kick);
#endif /* !CONFIG_S390 */
>>>>>>> refs/remotes/origin/master

void kvm_resched(struct kvm_vcpu *vcpu)
{
	if (!need_resched())
		return;
	cond_resched();
}
EXPORT_SYMBOL_GPL(kvm_resched);

<<<<<<< HEAD
=======
bool kvm_vcpu_yield_to(struct kvm_vcpu *target)
{
	struct pid *pid;
	struct task_struct *task = NULL;
	bool ret = false;

	rcu_read_lock();
	pid = rcu_dereference(target->pid);
	if (pid)
		task = get_pid_task(target->pid, PIDTYPE_PID);
	rcu_read_unlock();
	if (!task)
		return ret;
	if (task->flags & PF_VCPU) {
		put_task_struct(task);
		return ret;
	}
	ret = yield_to(task, 1);
	put_task_struct(task);

	return ret;
}
EXPORT_SYMBOL_GPL(kvm_vcpu_yield_to);

#ifdef CONFIG_HAVE_KVM_CPU_RELAX_INTERCEPT
/*
 * Helper that checks whether a VCPU is eligible for directed yield.
 * Most eligible candidate to yield is decided by following heuristics:
 *
 *  (a) VCPU which has not done pl-exit or cpu relax intercepted recently
 *  (preempted lock holder), indicated by @in_spin_loop.
 *  Set at the beiginning and cleared at the end of interception/PLE handler.
 *
 *  (b) VCPU which has done pl-exit/ cpu relax intercepted but did not get
 *  chance last time (mostly it has become eligible now since we have probably
 *  yielded to lockholder in last iteration. This is done by toggling
 *  @dy_eligible each time a VCPU checked for eligibility.)
 *
 *  Yielding to a recently pl-exited/cpu relax intercepted VCPU before yielding
 *  to preempted lock-holder could result in wrong VCPU selection and CPU
 *  burning. Giving priority for a potential lock-holder increases lock
 *  progress.
 *
 *  Since algorithm is based on heuristics, accessing another VCPU data without
 *  locking does not harm. It may result in trying to yield to  same VCPU, fail
 *  and continue with next VCPU and so on.
 */
bool kvm_vcpu_eligible_for_directed_yield(struct kvm_vcpu *vcpu)
{
	bool eligible;

	eligible = !vcpu->spin_loop.in_spin_loop ||
			(vcpu->spin_loop.in_spin_loop &&
			 vcpu->spin_loop.dy_eligible);

	if (vcpu->spin_loop.in_spin_loop)
		kvm_vcpu_set_dy_eligible(vcpu, !vcpu->spin_loop.dy_eligible);

	return eligible;
}
#endif

>>>>>>> refs/remotes/origin/master
void kvm_vcpu_on_spin(struct kvm_vcpu *me)
{
	struct kvm *kvm = me->kvm;
	struct kvm_vcpu *vcpu;
	int last_boosted_vcpu = me->kvm->last_boosted_vcpu;
	int yielded = 0;
<<<<<<< HEAD
	int pass;
	int i;

=======
	int try = 3;
	int pass;
	int i;

	kvm_vcpu_set_in_spin_loop(me, true);
>>>>>>> refs/remotes/origin/master
	/*
	 * We boost the priority of a VCPU that is runnable but not
	 * currently running, because it got preempted by something
	 * else and called schedule in __vcpu_run.  Hopefully that
	 * VCPU is holding the lock that we need and will release it.
	 * We approximate round-robin by starting at the last boosted VCPU.
	 */
<<<<<<< HEAD
	for (pass = 0; pass < 2 && !yielded; pass++) {
		kvm_for_each_vcpu(i, vcpu, kvm) {
			struct task_struct *task = NULL;
			struct pid *pid;
			if (!pass && i < last_boosted_vcpu) {
=======
	for (pass = 0; pass < 2 && !yielded && try; pass++) {
		kvm_for_each_vcpu(i, vcpu, kvm) {
			if (!pass && i <= last_boosted_vcpu) {
>>>>>>> refs/remotes/origin/master
				i = last_boosted_vcpu;
				continue;
			} else if (pass && i > last_boosted_vcpu)
				break;
<<<<<<< HEAD
=======
			if (!ACCESS_ONCE(vcpu->preempted))
				continue;
>>>>>>> refs/remotes/origin/master
			if (vcpu == me)
				continue;
			if (waitqueue_active(&vcpu->wq))
				continue;
<<<<<<< HEAD
			rcu_read_lock();
			pid = rcu_dereference(vcpu->pid);
			if (pid)
				task = get_pid_task(vcpu->pid, PIDTYPE_PID);
			rcu_read_unlock();
			if (!task)
				continue;
			if (task->flags & PF_VCPU) {
				put_task_struct(task);
				continue;
			}
			if (yield_to(task, 1)) {
				put_task_struct(task);
				kvm->last_boosted_vcpu = i;
				yielded = 1;
				break;
			}
			put_task_struct(task);
		}
	}
=======
			if (!kvm_vcpu_eligible_for_directed_yield(vcpu))
				continue;

			yielded = kvm_vcpu_yield_to(vcpu);
			if (yielded > 0) {
				kvm->last_boosted_vcpu = i;
				break;
			} else if (yielded < 0) {
				try--;
				if (!try)
					break;
			}
		}
	}
	kvm_vcpu_set_in_spin_loop(me, false);

	/* Ensure vcpu is not eligible during next spinloop */
	kvm_vcpu_set_dy_eligible(me, false);
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL_GPL(kvm_vcpu_on_spin);

static int kvm_vcpu_fault(struct vm_area_struct *vma, struct vm_fault *vmf)
{
	struct kvm_vcpu *vcpu = vma->vm_file->private_data;
	struct page *page;

	if (vmf->pgoff == 0)
		page = virt_to_page(vcpu->run);
#ifdef CONFIG_X86
	else if (vmf->pgoff == KVM_PIO_PAGE_OFFSET)
		page = virt_to_page(vcpu->arch.pio_data);
#endif
#ifdef KVM_COALESCED_MMIO_PAGE_OFFSET
	else if (vmf->pgoff == KVM_COALESCED_MMIO_PAGE_OFFSET)
		page = virt_to_page(vcpu->kvm->coalesced_mmio_ring);
#endif
	else
<<<<<<< HEAD
<<<<<<< HEAD
		return VM_FAULT_SIGBUS;
=======
		return kvm_arch_vcpu_fault(vcpu, vmf);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		return kvm_arch_vcpu_fault(vcpu, vmf);
>>>>>>> refs/remotes/origin/master
	get_page(page);
	vmf->page = page;
	return 0;
}

static const struct vm_operations_struct kvm_vcpu_vm_ops = {
	.fault = kvm_vcpu_fault,
};

static int kvm_vcpu_mmap(struct file *file, struct vm_area_struct *vma)
{
	vma->vm_ops = &kvm_vcpu_vm_ops;
	return 0;
}

static int kvm_vcpu_release(struct inode *inode, struct file *filp)
{
	struct kvm_vcpu *vcpu = filp->private_data;

	kvm_put_kvm(vcpu->kvm);
	return 0;
}

static struct file_operations kvm_vcpu_fops = {
	.release        = kvm_vcpu_release,
	.unlocked_ioctl = kvm_vcpu_ioctl,
<<<<<<< HEAD
<<<<<<< HEAD
	.compat_ioctl   = kvm_vcpu_ioctl,
=======
#ifdef CONFIG_COMPAT
	.compat_ioctl   = kvm_vcpu_compat_ioctl,
#endif
>>>>>>> refs/remotes/origin/cm-10.0
=======
#ifdef CONFIG_COMPAT
	.compat_ioctl   = kvm_vcpu_compat_ioctl,
#endif
>>>>>>> refs/remotes/origin/master
	.mmap           = kvm_vcpu_mmap,
	.llseek		= noop_llseek,
};

/*
 * Allocates an inode for the vcpu.
 */
static int create_vcpu_fd(struct kvm_vcpu *vcpu)
{
<<<<<<< HEAD
	return anon_inode_getfd("kvm-vcpu", &kvm_vcpu_fops, vcpu, O_RDWR);
=======
	return anon_inode_getfd("kvm-vcpu", &kvm_vcpu_fops, vcpu, O_RDWR | O_CLOEXEC);
>>>>>>> refs/remotes/origin/master
}

/*
 * Creates some virtual cpus.  Good luck creating more than one.
 */
static int kvm_vm_ioctl_create_vcpu(struct kvm *kvm, u32 id)
{
	int r;
	struct kvm_vcpu *vcpu, *v;

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	if (id >= KVM_MAX_VCPUS)
		return -EINVAL;

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (id >= KVM_MAX_VCPUS)
		return -EINVAL;

>>>>>>> refs/remotes/origin/master
=======
	if (id >= KVM_MAX_VCPUS)
		return -EINVAL;

>>>>>>> refs/remotes/origin/cm-11.0
	vcpu = kvm_arch_vcpu_create(kvm, id);
	if (IS_ERR(vcpu))
		return PTR_ERR(vcpu);

	preempt_notifier_init(&vcpu->preempt_notifier, &kvm_preempt_ops);

	r = kvm_arch_vcpu_setup(vcpu);
	if (r)
		goto vcpu_destroy;

	mutex_lock(&kvm->lock);
	if (!kvm_vcpu_compatible(vcpu)) {
		r = -EINVAL;
		goto unlock_vcpu_destroy;
	}
	if (atomic_read(&kvm->online_vcpus) == KVM_MAX_VCPUS) {
		r = -EINVAL;
		goto unlock_vcpu_destroy;
	}

	kvm_for_each_vcpu(r, v, kvm)
		if (v->vcpu_id == id) {
			r = -EEXIST;
			goto unlock_vcpu_destroy;
		}

	BUG_ON(kvm->vcpus[atomic_read(&kvm->online_vcpus)]);

	/* Now it's all set up, let userspace reach it */
	kvm_get_kvm(kvm);
	r = create_vcpu_fd(vcpu);
	if (r < 0) {
		kvm_put_kvm(kvm);
		goto unlock_vcpu_destroy;
	}

	kvm->vcpus[atomic_read(&kvm->online_vcpus)] = vcpu;
	smp_wmb();
	atomic_inc(&kvm->online_vcpus);

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_KVM_APIC_ARCHITECTURE
	if (kvm->bsp_vcpu_id == id)
		kvm->bsp_vcpu = vcpu;
#endif
=======
>>>>>>> refs/remotes/origin/cm-10.0
	mutex_unlock(&kvm->lock);
=======
	mutex_unlock(&kvm->lock);
	kvm_arch_vcpu_postcreate(vcpu);
>>>>>>> refs/remotes/origin/master
	return r;

unlock_vcpu_destroy:
	mutex_unlock(&kvm->lock);
vcpu_destroy:
	kvm_arch_vcpu_destroy(vcpu);
	return r;
}

static int kvm_vcpu_ioctl_set_sigmask(struct kvm_vcpu *vcpu, sigset_t *sigset)
{
	if (sigset) {
		sigdelsetmask(sigset, sigmask(SIGKILL)|sigmask(SIGSTOP));
		vcpu->sigset_active = 1;
		vcpu->sigset = *sigset;
	} else
		vcpu->sigset_active = 0;
	return 0;
}

static long kvm_vcpu_ioctl(struct file *filp,
			   unsigned int ioctl, unsigned long arg)
{
	struct kvm_vcpu *vcpu = filp->private_data;
	void __user *argp = (void __user *)arg;
	int r;
	struct kvm_fpu *fpu = NULL;
	struct kvm_sregs *kvm_sregs = NULL;

	if (vcpu->kvm->mm != current->mm)
		return -EIO;

<<<<<<< HEAD
#if defined(CONFIG_S390) || defined(CONFIG_PPC)
=======
#if defined(CONFIG_S390) || defined(CONFIG_PPC) || defined(CONFIG_MIPS)
>>>>>>> refs/remotes/origin/master
	/*
	 * Special cases: vcpu ioctls that are asynchronous to vcpu execution,
	 * so vcpu_load() would break it.
	 */
	if (ioctl == KVM_S390_INTERRUPT || ioctl == KVM_INTERRUPT)
		return kvm_arch_vcpu_ioctl(filp, ioctl, arg);
#endif


<<<<<<< HEAD
	vcpu_load(vcpu);
=======
	r = vcpu_load(vcpu);
	if (r)
		return r;
>>>>>>> refs/remotes/origin/master
	switch (ioctl) {
	case KVM_RUN:
		r = -EINVAL;
		if (arg)
			goto out;
		r = kvm_arch_vcpu_ioctl_run(vcpu, vcpu->run);
		trace_kvm_userspace_exit(vcpu->run->exit_reason, r);
		break;
	case KVM_GET_REGS: {
		struct kvm_regs *kvm_regs;

		r = -ENOMEM;
		kvm_regs = kzalloc(sizeof(struct kvm_regs), GFP_KERNEL);
		if (!kvm_regs)
			goto out;
		r = kvm_arch_vcpu_ioctl_get_regs(vcpu, kvm_regs);
		if (r)
			goto out_free1;
		r = -EFAULT;
		if (copy_to_user(argp, kvm_regs, sizeof(struct kvm_regs)))
			goto out_free1;
		r = 0;
out_free1:
		kfree(kvm_regs);
		break;
	}
	case KVM_SET_REGS: {
		struct kvm_regs *kvm_regs;

		r = -ENOMEM;
<<<<<<< HEAD
<<<<<<< HEAD
		kvm_regs = kzalloc(sizeof(struct kvm_regs), GFP_KERNEL);
		if (!kvm_regs)
			goto out;
		r = -EFAULT;
		if (copy_from_user(kvm_regs, argp, sizeof(struct kvm_regs)))
			goto out_free2;
=======
=======
>>>>>>> refs/remotes/origin/master
		kvm_regs = memdup_user(argp, sizeof(*kvm_regs));
		if (IS_ERR(kvm_regs)) {
			r = PTR_ERR(kvm_regs);
			goto out;
		}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		r = kvm_arch_vcpu_ioctl_set_regs(vcpu, kvm_regs);
		if (r)
			goto out_free2;
		r = 0;
out_free2:
=======
		r = kvm_arch_vcpu_ioctl_set_regs(vcpu, kvm_regs);
>>>>>>> refs/remotes/origin/master
		kfree(kvm_regs);
		break;
	}
	case KVM_GET_SREGS: {
		kvm_sregs = kzalloc(sizeof(struct kvm_sregs), GFP_KERNEL);
		r = -ENOMEM;
		if (!kvm_sregs)
			goto out;
		r = kvm_arch_vcpu_ioctl_get_sregs(vcpu, kvm_sregs);
		if (r)
			goto out;
		r = -EFAULT;
		if (copy_to_user(argp, kvm_sregs, sizeof(struct kvm_sregs)))
			goto out;
		r = 0;
		break;
	}
	case KVM_SET_SREGS: {
<<<<<<< HEAD
<<<<<<< HEAD
		kvm_sregs = kmalloc(sizeof(struct kvm_sregs), GFP_KERNEL);
		r = -ENOMEM;
		if (!kvm_sregs)
			goto out;
		r = -EFAULT;
		if (copy_from_user(kvm_sregs, argp, sizeof(struct kvm_sregs)))
			goto out;
=======
		kvm_sregs = memdup_user(argp, sizeof(*kvm_sregs));
		if (IS_ERR(kvm_sregs)) {
			r = PTR_ERR(kvm_sregs);
			goto out;
		}
>>>>>>> refs/remotes/origin/cm-10.0
		r = kvm_arch_vcpu_ioctl_set_sregs(vcpu, kvm_sregs);
		if (r)
			goto out;
		r = 0;
=======
		kvm_sregs = memdup_user(argp, sizeof(*kvm_sregs));
		if (IS_ERR(kvm_sregs)) {
			r = PTR_ERR(kvm_sregs);
			kvm_sregs = NULL;
			goto out;
		}
		r = kvm_arch_vcpu_ioctl_set_sregs(vcpu, kvm_sregs);
>>>>>>> refs/remotes/origin/master
		break;
	}
	case KVM_GET_MP_STATE: {
		struct kvm_mp_state mp_state;

		r = kvm_arch_vcpu_ioctl_get_mpstate(vcpu, &mp_state);
		if (r)
			goto out;
		r = -EFAULT;
		if (copy_to_user(argp, &mp_state, sizeof mp_state))
			goto out;
		r = 0;
		break;
	}
	case KVM_SET_MP_STATE: {
		struct kvm_mp_state mp_state;

		r = -EFAULT;
		if (copy_from_user(&mp_state, argp, sizeof mp_state))
			goto out;
		r = kvm_arch_vcpu_ioctl_set_mpstate(vcpu, &mp_state);
<<<<<<< HEAD
		if (r)
			goto out;
		r = 0;
=======
>>>>>>> refs/remotes/origin/master
		break;
	}
	case KVM_TRANSLATE: {
		struct kvm_translation tr;

		r = -EFAULT;
		if (copy_from_user(&tr, argp, sizeof tr))
			goto out;
		r = kvm_arch_vcpu_ioctl_translate(vcpu, &tr);
		if (r)
			goto out;
		r = -EFAULT;
		if (copy_to_user(argp, &tr, sizeof tr))
			goto out;
		r = 0;
		break;
	}
	case KVM_SET_GUEST_DEBUG: {
		struct kvm_guest_debug dbg;

		r = -EFAULT;
		if (copy_from_user(&dbg, argp, sizeof dbg))
			goto out;
		r = kvm_arch_vcpu_ioctl_set_guest_debug(vcpu, &dbg);
<<<<<<< HEAD
		if (r)
			goto out;
		r = 0;
=======
>>>>>>> refs/remotes/origin/master
		break;
	}
	case KVM_SET_SIGNAL_MASK: {
		struct kvm_signal_mask __user *sigmask_arg = argp;
		struct kvm_signal_mask kvm_sigmask;
		sigset_t sigset, *p;

		p = NULL;
		if (argp) {
			r = -EFAULT;
			if (copy_from_user(&kvm_sigmask, argp,
					   sizeof kvm_sigmask))
				goto out;
			r = -EINVAL;
			if (kvm_sigmask.len != sizeof sigset)
				goto out;
			r = -EFAULT;
			if (copy_from_user(&sigset, sigmask_arg->sigset,
					   sizeof sigset))
				goto out;
			p = &sigset;
		}
		r = kvm_vcpu_ioctl_set_sigmask(vcpu, p);
		break;
	}
	case KVM_GET_FPU: {
		fpu = kzalloc(sizeof(struct kvm_fpu), GFP_KERNEL);
		r = -ENOMEM;
		if (!fpu)
			goto out;
		r = kvm_arch_vcpu_ioctl_get_fpu(vcpu, fpu);
		if (r)
			goto out;
		r = -EFAULT;
		if (copy_to_user(argp, fpu, sizeof(struct kvm_fpu)))
			goto out;
		r = 0;
		break;
	}
	case KVM_SET_FPU: {
<<<<<<< HEAD
<<<<<<< HEAD
		fpu = kmalloc(sizeof(struct kvm_fpu), GFP_KERNEL);
		r = -ENOMEM;
		if (!fpu)
			goto out;
		r = -EFAULT;
		if (copy_from_user(fpu, argp, sizeof(struct kvm_fpu)))
			goto out;
=======
		fpu = memdup_user(argp, sizeof(*fpu));
		if (IS_ERR(fpu)) {
			r = PTR_ERR(fpu);
			goto out;
		}
>>>>>>> refs/remotes/origin/cm-10.0
		r = kvm_arch_vcpu_ioctl_set_fpu(vcpu, fpu);
		if (r)
			goto out;
		r = 0;
=======
		fpu = memdup_user(argp, sizeof(*fpu));
		if (IS_ERR(fpu)) {
			r = PTR_ERR(fpu);
			fpu = NULL;
			goto out;
		}
		r = kvm_arch_vcpu_ioctl_set_fpu(vcpu, fpu);
>>>>>>> refs/remotes/origin/master
		break;
	}
	default:
		r = kvm_arch_vcpu_ioctl(filp, ioctl, arg);
	}
out:
	vcpu_put(vcpu);
	kfree(fpu);
	kfree(kvm_sregs);
	return r;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_COMPAT
static long kvm_vcpu_compat_ioctl(struct file *filp,
				  unsigned int ioctl, unsigned long arg)
{
	struct kvm_vcpu *vcpu = filp->private_data;
	void __user *argp = compat_ptr(arg);
	int r;

	if (vcpu->kvm->mm != current->mm)
		return -EIO;

	switch (ioctl) {
	case KVM_SET_SIGNAL_MASK: {
		struct kvm_signal_mask __user *sigmask_arg = argp;
		struct kvm_signal_mask kvm_sigmask;
		compat_sigset_t csigset;
		sigset_t sigset;

		if (argp) {
			r = -EFAULT;
			if (copy_from_user(&kvm_sigmask, argp,
					   sizeof kvm_sigmask))
				goto out;
			r = -EINVAL;
			if (kvm_sigmask.len != sizeof csigset)
				goto out;
			r = -EFAULT;
			if (copy_from_user(&csigset, sigmask_arg->sigset,
					   sizeof csigset))
				goto out;
<<<<<<< HEAD
		}
		sigset_from_compat(&sigset, &csigset);
		r = kvm_vcpu_ioctl_set_sigmask(vcpu, &sigset);
=======
			sigset_from_compat(&sigset, &csigset);
			r = kvm_vcpu_ioctl_set_sigmask(vcpu, &sigset);
		} else
			r = kvm_vcpu_ioctl_set_sigmask(vcpu, NULL);
>>>>>>> refs/remotes/origin/master
		break;
	}
	default:
		r = kvm_vcpu_ioctl(filp, ioctl, arg);
	}

out:
	return r;
}
#endif

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int kvm_device_ioctl_attr(struct kvm_device *dev,
				 int (*accessor)(struct kvm_device *dev,
						 struct kvm_device_attr *attr),
				 unsigned long arg)
{
	struct kvm_device_attr attr;

	if (!accessor)
		return -EPERM;

	if (copy_from_user(&attr, (void __user *)arg, sizeof(attr)))
		return -EFAULT;

	return accessor(dev, &attr);
}

static long kvm_device_ioctl(struct file *filp, unsigned int ioctl,
			     unsigned long arg)
{
	struct kvm_device *dev = filp->private_data;

	switch (ioctl) {
	case KVM_SET_DEVICE_ATTR:
		return kvm_device_ioctl_attr(dev, dev->ops->set_attr, arg);
	case KVM_GET_DEVICE_ATTR:
		return kvm_device_ioctl_attr(dev, dev->ops->get_attr, arg);
	case KVM_HAS_DEVICE_ATTR:
		return kvm_device_ioctl_attr(dev, dev->ops->has_attr, arg);
	default:
		if (dev->ops->ioctl)
			return dev->ops->ioctl(dev, ioctl, arg);

		return -ENOTTY;
	}
}

static int kvm_device_release(struct inode *inode, struct file *filp)
{
	struct kvm_device *dev = filp->private_data;
	struct kvm *kvm = dev->kvm;

	kvm_put_kvm(kvm);
	return 0;
}

static const struct file_operations kvm_device_fops = {
	.unlocked_ioctl = kvm_device_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl = kvm_device_ioctl,
#endif
	.release = kvm_device_release,
};

struct kvm_device *kvm_device_from_filp(struct file *filp)
{
	if (filp->f_op != &kvm_device_fops)
		return NULL;

	return filp->private_data;
}

static int kvm_ioctl_create_device(struct kvm *kvm,
				   struct kvm_create_device *cd)
{
	struct kvm_device_ops *ops = NULL;
	struct kvm_device *dev;
	bool test = cd->flags & KVM_CREATE_DEVICE_TEST;
	int ret;

	switch (cd->type) {
#ifdef CONFIG_KVM_MPIC
	case KVM_DEV_TYPE_FSL_MPIC_20:
	case KVM_DEV_TYPE_FSL_MPIC_42:
		ops = &kvm_mpic_ops;
		break;
#endif
#ifdef CONFIG_KVM_XICS
	case KVM_DEV_TYPE_XICS:
		ops = &kvm_xics_ops;
		break;
#endif
#ifdef CONFIG_KVM_VFIO
	case KVM_DEV_TYPE_VFIO:
		ops = &kvm_vfio_ops;
		break;
#endif
	default:
		return -ENODEV;
	}

	if (test)
		return 0;

	dev = kzalloc(sizeof(*dev), GFP_KERNEL);
	if (!dev)
		return -ENOMEM;

	dev->ops = ops;
	dev->kvm = kvm;

	ret = ops->create(dev, cd->type);
	if (ret < 0) {
		kfree(dev);
		return ret;
	}

	ret = anon_inode_getfd(ops->name, &kvm_device_fops, dev, O_RDWR | O_CLOEXEC);
	if (ret < 0) {
		ops->destroy(dev);
		return ret;
	}

	list_add(&dev->vm_node, &kvm->devices);
	kvm_get_kvm(kvm);
	cd->fd = ret;
	return 0;
}

>>>>>>> refs/remotes/origin/master
static long kvm_vm_ioctl(struct file *filp,
			   unsigned int ioctl, unsigned long arg)
{
	struct kvm *kvm = filp->private_data;
	void __user *argp = (void __user *)arg;
	int r;

	if (kvm->mm != current->mm)
		return -EIO;
	switch (ioctl) {
	case KVM_CREATE_VCPU:
		r = kvm_vm_ioctl_create_vcpu(kvm, arg);
<<<<<<< HEAD
		if (r < 0)
			goto out;
=======
>>>>>>> refs/remotes/origin/master
		break;
	case KVM_SET_USER_MEMORY_REGION: {
		struct kvm_userspace_memory_region kvm_userspace_mem;

		r = -EFAULT;
		if (copy_from_user(&kvm_userspace_mem, argp,
						sizeof kvm_userspace_mem))
			goto out;

<<<<<<< HEAD
		r = kvm_vm_ioctl_set_memory_region(kvm, &kvm_userspace_mem, 1);
		if (r)
			goto out;
=======
		r = kvm_vm_ioctl_set_memory_region(kvm, &kvm_userspace_mem);
>>>>>>> refs/remotes/origin/master
		break;
	}
	case KVM_GET_DIRTY_LOG: {
		struct kvm_dirty_log log;

		r = -EFAULT;
		if (copy_from_user(&log, argp, sizeof log))
			goto out;
		r = kvm_vm_ioctl_get_dirty_log(kvm, &log);
<<<<<<< HEAD
		if (r)
			goto out;
=======
>>>>>>> refs/remotes/origin/master
		break;
	}
#ifdef KVM_COALESCED_MMIO_PAGE_OFFSET
	case KVM_REGISTER_COALESCED_MMIO: {
		struct kvm_coalesced_mmio_zone zone;
		r = -EFAULT;
		if (copy_from_user(&zone, argp, sizeof zone))
			goto out;
		r = kvm_vm_ioctl_register_coalesced_mmio(kvm, &zone);
<<<<<<< HEAD
		if (r)
			goto out;
		r = 0;
=======
>>>>>>> refs/remotes/origin/master
		break;
	}
	case KVM_UNREGISTER_COALESCED_MMIO: {
		struct kvm_coalesced_mmio_zone zone;
		r = -EFAULT;
		if (copy_from_user(&zone, argp, sizeof zone))
			goto out;
		r = kvm_vm_ioctl_unregister_coalesced_mmio(kvm, &zone);
<<<<<<< HEAD
		if (r)
			goto out;
		r = 0;
=======
>>>>>>> refs/remotes/origin/master
		break;
	}
#endif
	case KVM_IRQFD: {
		struct kvm_irqfd data;

		r = -EFAULT;
		if (copy_from_user(&data, argp, sizeof data))
			goto out;
<<<<<<< HEAD
		r = kvm_irqfd(kvm, data.fd, data.gsi, data.flags);
=======
		r = kvm_irqfd(kvm, &data);
>>>>>>> refs/remotes/origin/master
		break;
	}
	case KVM_IOEVENTFD: {
		struct kvm_ioeventfd data;

		r = -EFAULT;
		if (copy_from_user(&data, argp, sizeof data))
			goto out;
		r = kvm_ioeventfd(kvm, &data);
		break;
	}
#ifdef CONFIG_KVM_APIC_ARCHITECTURE
	case KVM_SET_BOOT_CPU_ID:
		r = 0;
		mutex_lock(&kvm->lock);
		if (atomic_read(&kvm->online_vcpus) != 0)
			r = -EBUSY;
		else
			kvm->bsp_vcpu_id = arg;
		mutex_unlock(&kvm->lock);
		break;
#endif
<<<<<<< HEAD
=======
#ifdef CONFIG_HAVE_KVM_MSI
	case KVM_SIGNAL_MSI: {
		struct kvm_msi msi;

		r = -EFAULT;
		if (copy_from_user(&msi, argp, sizeof msi))
			goto out;
		r = kvm_send_userspace_msi(kvm, &msi);
		break;
	}
#endif
#ifdef __KVM_HAVE_IRQ_LINE
	case KVM_IRQ_LINE_STATUS:
	case KVM_IRQ_LINE: {
		struct kvm_irq_level irq_event;

		r = -EFAULT;
		if (copy_from_user(&irq_event, argp, sizeof irq_event))
			goto out;

		r = kvm_vm_ioctl_irq_line(kvm, &irq_event,
					ioctl == KVM_IRQ_LINE_STATUS);
		if (r)
			goto out;

		r = -EFAULT;
		if (ioctl == KVM_IRQ_LINE_STATUS) {
			if (copy_to_user(argp, &irq_event, sizeof irq_event))
				goto out;
		}

		r = 0;
		break;
	}
#endif
#ifdef CONFIG_HAVE_KVM_IRQ_ROUTING
	case KVM_SET_GSI_ROUTING: {
		struct kvm_irq_routing routing;
		struct kvm_irq_routing __user *urouting;
		struct kvm_irq_routing_entry *entries;

		r = -EFAULT;
		if (copy_from_user(&routing, argp, sizeof(routing)))
			goto out;
		r = -EINVAL;
		if (routing.nr >= KVM_MAX_IRQ_ROUTES)
			goto out;
		if (routing.flags)
			goto out;
		r = -ENOMEM;
		entries = vmalloc(routing.nr * sizeof(*entries));
		if (!entries)
			goto out;
		r = -EFAULT;
		urouting = argp;
		if (copy_from_user(entries, urouting->entries,
				   routing.nr * sizeof(*entries)))
			goto out_free_irq_routing;
		r = kvm_set_irq_routing(kvm, entries, routing.nr,
					routing.flags);
	out_free_irq_routing:
		vfree(entries);
		break;
	}
#endif /* CONFIG_HAVE_KVM_IRQ_ROUTING */
	case KVM_CREATE_DEVICE: {
		struct kvm_create_device cd;

		r = -EFAULT;
		if (copy_from_user(&cd, argp, sizeof(cd)))
			goto out;

		r = kvm_ioctl_create_device(kvm, &cd);
		if (r)
			goto out;

		r = -EFAULT;
		if (copy_to_user(argp, &cd, sizeof(cd)))
			goto out;

		r = 0;
		break;
	}
>>>>>>> refs/remotes/origin/master
	default:
		r = kvm_arch_vm_ioctl(filp, ioctl, arg);
		if (r == -ENOTTY)
			r = kvm_vm_ioctl_assigned_device(kvm, ioctl, arg);
	}
out:
	return r;
}

#ifdef CONFIG_COMPAT
struct compat_kvm_dirty_log {
	__u32 slot;
	__u32 padding1;
	union {
		compat_uptr_t dirty_bitmap; /* one bit per page */
		__u64 padding2;
	};
};

static long kvm_vm_compat_ioctl(struct file *filp,
			   unsigned int ioctl, unsigned long arg)
{
	struct kvm *kvm = filp->private_data;
	int r;

	if (kvm->mm != current->mm)
		return -EIO;
	switch (ioctl) {
	case KVM_GET_DIRTY_LOG: {
		struct compat_kvm_dirty_log compat_log;
		struct kvm_dirty_log log;

		r = -EFAULT;
		if (copy_from_user(&compat_log, (void __user *)arg,
				   sizeof(compat_log)))
			goto out;
		log.slot	 = compat_log.slot;
		log.padding1	 = compat_log.padding1;
		log.padding2	 = compat_log.padding2;
		log.dirty_bitmap = compat_ptr(compat_log.dirty_bitmap);

		r = kvm_vm_ioctl_get_dirty_log(kvm, &log);
<<<<<<< HEAD
		if (r)
			goto out;
=======
>>>>>>> refs/remotes/origin/master
		break;
	}
	default:
		r = kvm_vm_ioctl(filp, ioctl, arg);
	}

out:
	return r;
}
#endif

<<<<<<< HEAD
static int kvm_vm_fault(struct vm_area_struct *vma, struct vm_fault *vmf)
{
	struct page *page[1];
	unsigned long addr;
	int npages;
	gfn_t gfn = vmf->pgoff;
	struct kvm *kvm = vma->vm_file->private_data;

	addr = gfn_to_hva(kvm, gfn);
	if (kvm_is_error_hva(addr))
		return VM_FAULT_SIGBUS;

	npages = get_user_pages(current, current->mm, addr, 1, 1, 0, page,
				NULL);
	if (unlikely(npages != 1))
		return VM_FAULT_SIGBUS;

	vmf->page = page[0];
	return 0;
}

static const struct vm_operations_struct kvm_vm_vm_ops = {
	.fault = kvm_vm_fault,
};

static int kvm_vm_mmap(struct file *file, struct vm_area_struct *vma)
{
	vma->vm_ops = &kvm_vm_vm_ops;
	return 0;
}

=======
>>>>>>> refs/remotes/origin/master
static struct file_operations kvm_vm_fops = {
	.release        = kvm_vm_release,
	.unlocked_ioctl = kvm_vm_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl   = kvm_vm_compat_ioctl,
#endif
<<<<<<< HEAD
	.mmap           = kvm_vm_mmap,
	.llseek		= noop_llseek,
};

<<<<<<< HEAD
static int kvm_dev_ioctl_create_vm(void)
=======
static int kvm_dev_ioctl_create_vm(unsigned long type)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.llseek		= noop_llseek,
};

static int kvm_dev_ioctl_create_vm(unsigned long type)
>>>>>>> refs/remotes/origin/master
{
	int r;
	struct kvm *kvm;

<<<<<<< HEAD
<<<<<<< HEAD
	kvm = kvm_create_vm();
=======
	kvm = kvm_create_vm(type);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	kvm = kvm_create_vm(type);
>>>>>>> refs/remotes/origin/master
	if (IS_ERR(kvm))
		return PTR_ERR(kvm);
#ifdef KVM_COALESCED_MMIO_PAGE_OFFSET
	r = kvm_coalesced_mmio_init(kvm);
	if (r < 0) {
		kvm_put_kvm(kvm);
		return r;
	}
#endif
<<<<<<< HEAD
	r = anon_inode_getfd("kvm-vm", &kvm_vm_fops, kvm, O_RDWR);
=======
	r = anon_inode_getfd("kvm-vm", &kvm_vm_fops, kvm, O_RDWR | O_CLOEXEC);
>>>>>>> refs/remotes/origin/master
	if (r < 0)
		kvm_put_kvm(kvm);

	return r;
}

static long kvm_dev_ioctl_check_extension_generic(long arg)
{
	switch (arg) {
	case KVM_CAP_USER_MEMORY:
	case KVM_CAP_DESTROY_MEMORY_REGION_WORKS:
	case KVM_CAP_JOIN_MEMORY_REGIONS_WORKS:
#ifdef CONFIG_KVM_APIC_ARCHITECTURE
	case KVM_CAP_SET_BOOT_CPU_ID:
#endif
	case KVM_CAP_INTERNAL_ERROR_DATA:
<<<<<<< HEAD
		return 1;
#ifdef CONFIG_HAVE_KVM_IRQCHIP
=======
#ifdef CONFIG_HAVE_KVM_MSI
	case KVM_CAP_SIGNAL_MSI:
#endif
#ifdef CONFIG_HAVE_KVM_IRQ_ROUTING
	case KVM_CAP_IRQFD_RESAMPLE:
#endif
		return 1;
#ifdef CONFIG_HAVE_KVM_IRQ_ROUTING
>>>>>>> refs/remotes/origin/master
	case KVM_CAP_IRQ_ROUTING:
		return KVM_MAX_IRQ_ROUTES;
#endif
	default:
		break;
	}
	return kvm_dev_ioctl_check_extension(arg);
}

static long kvm_dev_ioctl(struct file *filp,
			  unsigned int ioctl, unsigned long arg)
{
	long r = -EINVAL;

	switch (ioctl) {
	case KVM_GET_API_VERSION:
		r = -EINVAL;
		if (arg)
			goto out;
		r = KVM_API_VERSION;
		break;
	case KVM_CREATE_VM:
<<<<<<< HEAD
<<<<<<< HEAD
		r = -EINVAL;
		if (arg)
			goto out;
		r = kvm_dev_ioctl_create_vm();
=======
		r = kvm_dev_ioctl_create_vm(arg);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		r = kvm_dev_ioctl_create_vm(arg);
>>>>>>> refs/remotes/origin/master
		break;
	case KVM_CHECK_EXTENSION:
		r = kvm_dev_ioctl_check_extension_generic(arg);
		break;
	case KVM_GET_VCPU_MMAP_SIZE:
		r = -EINVAL;
		if (arg)
			goto out;
		r = PAGE_SIZE;     /* struct kvm_run */
#ifdef CONFIG_X86
		r += PAGE_SIZE;    /* pio data page */
#endif
#ifdef KVM_COALESCED_MMIO_PAGE_OFFSET
		r += PAGE_SIZE;    /* coalesced mmio ring page */
#endif
		break;
	case KVM_TRACE_ENABLE:
	case KVM_TRACE_PAUSE:
	case KVM_TRACE_DISABLE:
		r = -EOPNOTSUPP;
		break;
	default:
		return kvm_arch_dev_ioctl(filp, ioctl, arg);
	}
out:
	return r;
}

static struct file_operations kvm_chardev_ops = {
	.unlocked_ioctl = kvm_dev_ioctl,
	.compat_ioctl   = kvm_dev_ioctl,
	.llseek		= noop_llseek,
};

static struct miscdevice kvm_dev = {
	KVM_MINOR,
	"kvm",
	&kvm_chardev_ops,
};

static void hardware_enable_nolock(void *junk)
{
	int cpu = raw_smp_processor_id();
	int r;

	if (cpumask_test_cpu(cpu, cpus_hardware_enabled))
		return;

	cpumask_set_cpu(cpu, cpus_hardware_enabled);

	r = kvm_arch_hardware_enable(NULL);

	if (r) {
		cpumask_clear_cpu(cpu, cpus_hardware_enabled);
		atomic_inc(&hardware_enable_failed);
		printk(KERN_INFO "kvm: enabling virtualization on "
				 "CPU%d failed\n", cpu);
	}
}

<<<<<<< HEAD
static void hardware_enable(void *junk)
{
	raw_spin_lock(&kvm_lock);
	hardware_enable_nolock(junk);
	raw_spin_unlock(&kvm_lock);
=======
static void hardware_enable(void)
{
	raw_spin_lock(&kvm_count_lock);
	if (kvm_usage_count)
		hardware_enable_nolock(NULL);
	raw_spin_unlock(&kvm_count_lock);
>>>>>>> refs/remotes/origin/master
}

static void hardware_disable_nolock(void *junk)
{
	int cpu = raw_smp_processor_id();

	if (!cpumask_test_cpu(cpu, cpus_hardware_enabled))
		return;
	cpumask_clear_cpu(cpu, cpus_hardware_enabled);
	kvm_arch_hardware_disable(NULL);
}

<<<<<<< HEAD
static void hardware_disable(void *junk)
{
	raw_spin_lock(&kvm_lock);
	hardware_disable_nolock(junk);
	raw_spin_unlock(&kvm_lock);
=======
static void hardware_disable(void)
{
	raw_spin_lock(&kvm_count_lock);
	if (kvm_usage_count)
		hardware_disable_nolock(NULL);
	raw_spin_unlock(&kvm_count_lock);
>>>>>>> refs/remotes/origin/master
}

static void hardware_disable_all_nolock(void)
{
	BUG_ON(!kvm_usage_count);

	kvm_usage_count--;
	if (!kvm_usage_count)
		on_each_cpu(hardware_disable_nolock, NULL, 1);
}

static void hardware_disable_all(void)
{
<<<<<<< HEAD
	raw_spin_lock(&kvm_lock);
	hardware_disable_all_nolock();
	raw_spin_unlock(&kvm_lock);
=======
	raw_spin_lock(&kvm_count_lock);
	hardware_disable_all_nolock();
	raw_spin_unlock(&kvm_count_lock);
>>>>>>> refs/remotes/origin/master
}

static int hardware_enable_all(void)
{
	int r = 0;

<<<<<<< HEAD
	raw_spin_lock(&kvm_lock);
=======
	raw_spin_lock(&kvm_count_lock);
>>>>>>> refs/remotes/origin/master

	kvm_usage_count++;
	if (kvm_usage_count == 1) {
		atomic_set(&hardware_enable_failed, 0);
		on_each_cpu(hardware_enable_nolock, NULL, 1);

		if (atomic_read(&hardware_enable_failed)) {
			hardware_disable_all_nolock();
			r = -EBUSY;
		}
	}

<<<<<<< HEAD
	raw_spin_unlock(&kvm_lock);
=======
	raw_spin_unlock(&kvm_count_lock);
>>>>>>> refs/remotes/origin/master

	return r;
}

static int kvm_cpu_hotplug(struct notifier_block *notifier, unsigned long val,
			   void *v)
{
	int cpu = (long)v;

<<<<<<< HEAD
	if (!kvm_usage_count)
		return NOTIFY_OK;

=======
>>>>>>> refs/remotes/origin/master
	val &= ~CPU_TASKS_FROZEN;
	switch (val) {
	case CPU_DYING:
		printk(KERN_INFO "kvm: disabling virtualization on CPU%d\n",
		       cpu);
<<<<<<< HEAD
		hardware_disable(NULL);
=======
		hardware_disable();
>>>>>>> refs/remotes/origin/master
		break;
	case CPU_STARTING:
		printk(KERN_INFO "kvm: enabling virtualization on CPU%d\n",
		       cpu);
<<<<<<< HEAD
		hardware_enable(NULL);
=======
		hardware_enable();
>>>>>>> refs/remotes/origin/master
		break;
	}
	return NOTIFY_OK;
}

<<<<<<< HEAD

asmlinkage void kvm_spurious_fault(void)
{
	/* Fault while not rebooting.  We want the trace. */
	BUG();
}
EXPORT_SYMBOL_GPL(kvm_spurious_fault);

=======
>>>>>>> refs/remotes/origin/master
static int kvm_reboot(struct notifier_block *notifier, unsigned long val,
		      void *v)
{
	/*
	 * Some (well, at least mine) BIOSes hang on reboot if
	 * in vmx root mode.
	 *
	 * And Intel TXT required VMX off for all cpu when system shutdown.
	 */
	printk(KERN_INFO "kvm: exiting hardware virtualization\n");
	kvm_rebooting = true;
	on_each_cpu(hardware_disable_nolock, NULL, 1);
	return NOTIFY_OK;
}

static struct notifier_block kvm_reboot_notifier = {
	.notifier_call = kvm_reboot,
	.priority = 0,
};

static void kvm_io_bus_destroy(struct kvm_io_bus *bus)
{
	int i;

	for (i = 0; i < bus->dev_count; i++) {
<<<<<<< HEAD
<<<<<<< HEAD
		struct kvm_io_device *pos = bus->devs[i];
=======
		struct kvm_io_device *pos = bus->range[i].dev;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		struct kvm_io_device *pos = bus->range[i].dev;
>>>>>>> refs/remotes/origin/master

		kvm_iodevice_destructor(pos);
	}
	kfree(bus);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
int kvm_io_bus_sort_cmp(const void *p1, const void *p2)
{
	const struct kvm_io_range *r1 = p1;
	const struct kvm_io_range *r2 = p2;

=======
static inline int kvm_io_bus_cmp(const struct kvm_io_range *r1,
                                 const struct kvm_io_range *r2)
{
>>>>>>> refs/remotes/origin/master
	if (r1->addr < r2->addr)
		return -1;
	if (r1->addr + r1->len > r2->addr + r2->len)
		return 1;
	return 0;
}

<<<<<<< HEAD
int kvm_io_bus_insert_dev(struct kvm_io_bus *bus, struct kvm_io_device *dev,
			  gpa_t addr, int len)
{
	if (bus->dev_count == NR_IOBUS_DEVS)
		return -ENOSPC;

=======
static int kvm_io_bus_sort_cmp(const void *p1, const void *p2)
{
	return kvm_io_bus_cmp(p1, p2);
}

static int kvm_io_bus_insert_dev(struct kvm_io_bus *bus, struct kvm_io_device *dev,
			  gpa_t addr, int len)
{
>>>>>>> refs/remotes/origin/master
	bus->range[bus->dev_count++] = (struct kvm_io_range) {
		.addr = addr,
		.len = len,
		.dev = dev,
	};

	sort(bus->range, bus->dev_count, sizeof(struct kvm_io_range),
		kvm_io_bus_sort_cmp, NULL);

	return 0;
}

<<<<<<< HEAD
int kvm_io_bus_get_first_dev(struct kvm_io_bus *bus,
=======
static int kvm_io_bus_get_first_dev(struct kvm_io_bus *bus,
>>>>>>> refs/remotes/origin/master
			     gpa_t addr, int len)
{
	struct kvm_io_range *range, key;
	int off;

	key = (struct kvm_io_range) {
		.addr = addr,
		.len = len,
	};

	range = bsearch(&key, bus->range, bus->dev_count,
			sizeof(struct kvm_io_range), kvm_io_bus_sort_cmp);
	if (range == NULL)
		return -ENOENT;

	off = range - bus->range;

<<<<<<< HEAD
	while (off > 0 && kvm_io_bus_sort_cmp(&key, &bus->range[off-1]) == 0)
=======
	while (off > 0 && kvm_io_bus_cmp(&key, &bus->range[off-1]) == 0)
>>>>>>> refs/remotes/origin/master
		off--;

	return off;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int __kvm_io_bus_write(struct kvm_io_bus *bus,
			      struct kvm_io_range *range, const void *val)
{
	int idx;

	idx = kvm_io_bus_get_first_dev(bus, range->addr, range->len);
	if (idx < 0)
		return -EOPNOTSUPP;

	while (idx < bus->dev_count &&
		kvm_io_bus_cmp(range, &bus->range[idx]) == 0) {
		if (!kvm_iodevice_write(bus->range[idx].dev, range->addr,
					range->len, val))
			return idx;
		idx++;
	}

	return -EOPNOTSUPP;
}

>>>>>>> refs/remotes/origin/master
/* kvm_io_bus_write - called under kvm->slots_lock */
int kvm_io_bus_write(struct kvm *kvm, enum kvm_bus bus_idx, gpa_t addr,
		     int len, const void *val)
{
<<<<<<< HEAD
<<<<<<< HEAD
	int i;
	struct kvm_io_bus *bus;

	bus = srcu_dereference(kvm->buses[bus_idx], &kvm->srcu);
	for (i = 0; i < bus->dev_count; i++)
		if (!kvm_iodevice_write(bus->devs[i], addr, len, val))
			return 0;
=======
	int idx;
=======
	struct kvm_io_bus *bus;
	struct kvm_io_range range;
	int r;

	range = (struct kvm_io_range) {
		.addr = addr,
		.len = len,
	};

	bus = srcu_dereference(kvm->buses[bus_idx], &kvm->srcu);
	r = __kvm_io_bus_write(bus, &range, val);
	return r < 0 ? r : 0;
}

/* kvm_io_bus_write_cookie - called under kvm->slots_lock */
int kvm_io_bus_write_cookie(struct kvm *kvm, enum kvm_bus bus_idx, gpa_t addr,
			    int len, const void *val, long cookie)
{
>>>>>>> refs/remotes/origin/master
	struct kvm_io_bus *bus;
	struct kvm_io_range range;

	range = (struct kvm_io_range) {
		.addr = addr,
		.len = len,
	};

	bus = srcu_dereference(kvm->buses[bus_idx], &kvm->srcu);
<<<<<<< HEAD
	idx = kvm_io_bus_get_first_dev(bus, addr, len);
=======

	/* First try the device referenced by cookie. */
	if ((cookie >= 0) && (cookie < bus->dev_count) &&
	    (kvm_io_bus_cmp(&range, &bus->range[cookie]) == 0))
		if (!kvm_iodevice_write(bus->range[cookie].dev, addr, len,
					val))
			return cookie;

	/*
	 * cookie contained garbage; fall back to search and return the
	 * correct cookie value.
	 */
	return __kvm_io_bus_write(bus, &range, val);
}

static int __kvm_io_bus_read(struct kvm_io_bus *bus, struct kvm_io_range *range,
			     void *val)
{
	int idx;

	idx = kvm_io_bus_get_first_dev(bus, range->addr, range->len);
>>>>>>> refs/remotes/origin/master
	if (idx < 0)
		return -EOPNOTSUPP;

	while (idx < bus->dev_count &&
<<<<<<< HEAD
		kvm_io_bus_sort_cmp(&range, &bus->range[idx]) == 0) {
		if (!kvm_iodevice_write(bus->range[idx].dev, addr, len, val))
			return 0;
		idx++;
	}

>>>>>>> refs/remotes/origin/cm-10.0
=======
		kvm_io_bus_cmp(range, &bus->range[idx]) == 0) {
		if (!kvm_iodevice_read(bus->range[idx].dev, range->addr,
				       range->len, val))
			return idx;
		idx++;
	}

>>>>>>> refs/remotes/origin/master
	return -EOPNOTSUPP;
}

/* kvm_io_bus_read - called under kvm->slots_lock */
int kvm_io_bus_read(struct kvm *kvm, enum kvm_bus bus_idx, gpa_t addr,
		    int len, void *val)
{
<<<<<<< HEAD
<<<<<<< HEAD
	int i;
	struct kvm_io_bus *bus;

	bus = srcu_dereference(kvm->buses[bus_idx], &kvm->srcu);
	for (i = 0; i < bus->dev_count; i++)
		if (!kvm_iodevice_read(bus->devs[i], addr, len, val))
			return 0;
=======
	int idx;
=======
	struct kvm_io_bus *bus;
	struct kvm_io_range range;
	int r;

	range = (struct kvm_io_range) {
		.addr = addr,
		.len = len,
	};

	bus = srcu_dereference(kvm->buses[bus_idx], &kvm->srcu);
	r = __kvm_io_bus_read(bus, &range, val);
	return r < 0 ? r : 0;
}

/* kvm_io_bus_read_cookie - called under kvm->slots_lock */
int kvm_io_bus_read_cookie(struct kvm *kvm, enum kvm_bus bus_idx, gpa_t addr,
			   int len, void *val, long cookie)
{
>>>>>>> refs/remotes/origin/master
	struct kvm_io_bus *bus;
	struct kvm_io_range range;

	range = (struct kvm_io_range) {
		.addr = addr,
		.len = len,
	};

	bus = srcu_dereference(kvm->buses[bus_idx], &kvm->srcu);
<<<<<<< HEAD
	idx = kvm_io_bus_get_first_dev(bus, addr, len);
	if (idx < 0)
		return -EOPNOTSUPP;

	while (idx < bus->dev_count &&
		kvm_io_bus_sort_cmp(&range, &bus->range[idx]) == 0) {
		if (!kvm_iodevice_read(bus->range[idx].dev, addr, len, val))
			return 0;
		idx++;
	}

>>>>>>> refs/remotes/origin/cm-10.0
	return -EOPNOTSUPP;
}

/* Caller must hold slots_lock. */
<<<<<<< HEAD
int kvm_io_bus_register_dev(struct kvm *kvm, enum kvm_bus bus_idx,
			    struct kvm_io_device *dev)
=======
int kvm_io_bus_register_dev(struct kvm *kvm, enum kvm_bus bus_idx, gpa_t addr,
			    int len, struct kvm_io_device *dev)
>>>>>>> refs/remotes/origin/cm-10.0
=======

	/* First try the device referenced by cookie. */
	if ((cookie >= 0) && (cookie < bus->dev_count) &&
	    (kvm_io_bus_cmp(&range, &bus->range[cookie]) == 0))
		if (!kvm_iodevice_read(bus->range[cookie].dev, addr, len,
				       val))
			return cookie;

	/*
	 * cookie contained garbage; fall back to search and return the
	 * correct cookie value.
	 */
	return __kvm_io_bus_read(bus, &range, val);
}

/* Caller must hold slots_lock. */
int kvm_io_bus_register_dev(struct kvm *kvm, enum kvm_bus bus_idx, gpa_t addr,
			    int len, struct kvm_io_device *dev)
>>>>>>> refs/remotes/origin/master
{
	struct kvm_io_bus *new_bus, *bus;

	bus = kvm->buses[bus_idx];
<<<<<<< HEAD
	if (bus->dev_count > NR_IOBUS_DEVS-1)
		return -ENOSPC;

<<<<<<< HEAD
	new_bus = kzalloc(sizeof(struct kvm_io_bus), GFP_KERNEL);
	if (!new_bus)
		return -ENOMEM;
	memcpy(new_bus, bus, sizeof(struct kvm_io_bus));
	new_bus->devs[new_bus->dev_count++] = dev;
=======
	new_bus = kmemdup(bus, sizeof(struct kvm_io_bus), GFP_KERNEL);
	if (!new_bus)
		return -ENOMEM;
	kvm_io_bus_insert_dev(new_bus, dev, addr, len);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* exclude ioeventfd which is limited by maximum fd */
	if (bus->dev_count - bus->ioeventfd_count > NR_IOBUS_DEVS - 1)
		return -ENOSPC;

	new_bus = kzalloc(sizeof(*bus) + ((bus->dev_count + 1) *
			  sizeof(struct kvm_io_range)), GFP_KERNEL);
	if (!new_bus)
		return -ENOMEM;
	memcpy(new_bus, bus, sizeof(*bus) + (bus->dev_count *
	       sizeof(struct kvm_io_range)));
	kvm_io_bus_insert_dev(new_bus, dev, addr, len);
>>>>>>> refs/remotes/origin/master
	rcu_assign_pointer(kvm->buses[bus_idx], new_bus);
	synchronize_srcu_expedited(&kvm->srcu);
	kfree(bus);

	return 0;
}

/* Caller must hold slots_lock. */
int kvm_io_bus_unregister_dev(struct kvm *kvm, enum kvm_bus bus_idx,
			      struct kvm_io_device *dev)
{
	int i, r;
	struct kvm_io_bus *new_bus, *bus;

<<<<<<< HEAD
<<<<<<< HEAD
	new_bus = kzalloc(sizeof(struct kvm_io_bus), GFP_KERNEL);
	if (!new_bus)
		return -ENOMEM;

	bus = kvm->buses[bus_idx];
	memcpy(new_bus, bus, sizeof(struct kvm_io_bus));

	r = -ENOENT;
	for (i = 0; i < new_bus->dev_count; i++)
		if (new_bus->devs[i] == dev) {
			r = 0;
			new_bus->devs[i] = new_bus->devs[--new_bus->dev_count];
=======
	bus = kvm->buses[bus_idx];

	new_bus = kmemdup(bus, sizeof(*bus), GFP_KERNEL);
	if (!new_bus)
		return -ENOMEM;

	r = -ENOENT;
	for (i = 0; i < new_bus->dev_count; i++)
		if (new_bus->range[i].dev == dev) {
			r = 0;
			new_bus->dev_count--;
			new_bus->range[i] = new_bus->range[new_bus->dev_count];
			sort(new_bus->range, new_bus->dev_count,
			     sizeof(struct kvm_io_range),
			     kvm_io_bus_sort_cmp, NULL);
>>>>>>> refs/remotes/origin/cm-10.0
			break;
		}

	if (r) {
		kfree(new_bus);
		return r;
	}
=======
	bus = kvm->buses[bus_idx];
	r = -ENOENT;
	for (i = 0; i < bus->dev_count; i++)
		if (bus->range[i].dev == dev) {
			r = 0;
			break;
		}

	if (r)
		return r;

	new_bus = kzalloc(sizeof(*bus) + ((bus->dev_count - 1) *
			  sizeof(struct kvm_io_range)), GFP_KERNEL);
	if (!new_bus)
		return -ENOMEM;

	memcpy(new_bus, bus, sizeof(*bus) + i * sizeof(struct kvm_io_range));
	new_bus->dev_count--;
	memcpy(new_bus->range + i, bus->range + i + 1,
	       (new_bus->dev_count - i) * sizeof(struct kvm_io_range));
>>>>>>> refs/remotes/origin/master

	rcu_assign_pointer(kvm->buses[bus_idx], new_bus);
	synchronize_srcu_expedited(&kvm->srcu);
	kfree(bus);
	return r;
}

static struct notifier_block kvm_cpu_notifier = {
	.notifier_call = kvm_cpu_hotplug,
};

static int vm_stat_get(void *_offset, u64 *val)
{
	unsigned offset = (long)_offset;
	struct kvm *kvm;

	*val = 0;
<<<<<<< HEAD
	raw_spin_lock(&kvm_lock);
	list_for_each_entry(kvm, &vm_list, vm_list)
		*val += *(u32 *)((void *)kvm + offset);
	raw_spin_unlock(&kvm_lock);
=======
	spin_lock(&kvm_lock);
	list_for_each_entry(kvm, &vm_list, vm_list)
		*val += *(u32 *)((void *)kvm + offset);
	spin_unlock(&kvm_lock);
>>>>>>> refs/remotes/origin/master
	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(vm_stat_fops, vm_stat_get, NULL, "%llu\n");

static int vcpu_stat_get(void *_offset, u64 *val)
{
	unsigned offset = (long)_offset;
	struct kvm *kvm;
	struct kvm_vcpu *vcpu;
	int i;

	*val = 0;
<<<<<<< HEAD
	raw_spin_lock(&kvm_lock);
=======
	spin_lock(&kvm_lock);
>>>>>>> refs/remotes/origin/master
	list_for_each_entry(kvm, &vm_list, vm_list)
		kvm_for_each_vcpu(i, vcpu, kvm)
			*val += *(u32 *)((void *)vcpu + offset);

<<<<<<< HEAD
	raw_spin_unlock(&kvm_lock);
=======
	spin_unlock(&kvm_lock);
>>>>>>> refs/remotes/origin/master
	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(vcpu_stat_fops, vcpu_stat_get, NULL, "%llu\n");

static const struct file_operations *stat_fops[] = {
	[KVM_STAT_VCPU] = &vcpu_stat_fops,
	[KVM_STAT_VM]   = &vm_stat_fops,
};

<<<<<<< HEAD
<<<<<<< HEAD
static void kvm_init_debug(void)
{
	struct kvm_stats_debugfs_item *p;

	kvm_debugfs_dir = debugfs_create_dir("kvm", NULL);
	for (p = debugfs_entries; p->name; ++p)
		p->dentry = debugfs_create_file(p->name, 0444, kvm_debugfs_dir,
						(void *)(long)p->offset,
						stat_fops[p->kind]);
=======
static int kvm_init_debug(void)
{
	int r = -EFAULT;
=======
static int kvm_init_debug(void)
{
	int r = -EEXIST;
>>>>>>> refs/remotes/origin/master
	struct kvm_stats_debugfs_item *p;

	kvm_debugfs_dir = debugfs_create_dir("kvm", NULL);
	if (kvm_debugfs_dir == NULL)
		goto out;

	for (p = debugfs_entries; p->name; ++p) {
		p->dentry = debugfs_create_file(p->name, 0444, kvm_debugfs_dir,
						(void *)(long)p->offset,
						stat_fops[p->kind]);
		if (p->dentry == NULL)
			goto out_dir;
	}

	return 0;

out_dir:
	debugfs_remove_recursive(kvm_debugfs_dir);
out:
	return r;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static void kvm_exit_debug(void)
{
	struct kvm_stats_debugfs_item *p;

	for (p = debugfs_entries; p->name; ++p)
		debugfs_remove(p->dentry);
	debugfs_remove(kvm_debugfs_dir);
}

static int kvm_suspend(void)
{
	if (kvm_usage_count)
		hardware_disable_nolock(NULL);
	return 0;
}

static void kvm_resume(void)
{
	if (kvm_usage_count) {
<<<<<<< HEAD
		WARN_ON(raw_spin_is_locked(&kvm_lock));
=======
		WARN_ON(raw_spin_is_locked(&kvm_count_lock));
>>>>>>> refs/remotes/origin/master
		hardware_enable_nolock(NULL);
	}
}

static struct syscore_ops kvm_syscore_ops = {
	.suspend = kvm_suspend,
	.resume = kvm_resume,
};

<<<<<<< HEAD
struct page *bad_page;
pfn_t bad_pfn;

=======
>>>>>>> refs/remotes/origin/master
static inline
struct kvm_vcpu *preempt_notifier_to_vcpu(struct preempt_notifier *pn)
{
	return container_of(pn, struct kvm_vcpu, preempt_notifier);
}

static void kvm_sched_in(struct preempt_notifier *pn, int cpu)
{
	struct kvm_vcpu *vcpu = preempt_notifier_to_vcpu(pn);
<<<<<<< HEAD
=======
	if (vcpu->preempted)
		vcpu->preempted = false;
>>>>>>> refs/remotes/origin/master

	kvm_arch_vcpu_load(vcpu, cpu);
}

static void kvm_sched_out(struct preempt_notifier *pn,
			  struct task_struct *next)
{
	struct kvm_vcpu *vcpu = preempt_notifier_to_vcpu(pn);

<<<<<<< HEAD
=======
	if (current->state == TASK_RUNNING)
		vcpu->preempted = true;
>>>>>>> refs/remotes/origin/master
	kvm_arch_vcpu_put(vcpu);
}

int kvm_init(void *opaque, unsigned vcpu_size, unsigned vcpu_align,
		  struct module *module)
{
	int r;
	int cpu;

	r = kvm_arch_init(opaque);
	if (r)
		goto out_fail;

<<<<<<< HEAD
	bad_page = alloc_page(GFP_KERNEL | __GFP_ZERO);

	if (bad_page == NULL) {
		r = -ENOMEM;
		goto out;
	}

	bad_pfn = page_to_pfn(bad_page);

	hwpoison_page = alloc_page(GFP_KERNEL | __GFP_ZERO);

	if (hwpoison_page == NULL) {
		r = -ENOMEM;
		goto out_free_0;
	}

	hwpoison_pfn = page_to_pfn(hwpoison_page);

	fault_page = alloc_page(GFP_KERNEL | __GFP_ZERO);

	if (fault_page == NULL) {
		r = -ENOMEM;
		goto out_free_0;
	}

	fault_pfn = page_to_pfn(fault_page);
=======
	/*
	 * kvm_arch_init makes sure there's at most one caller
	 * for architectures that support multiple implementations,
	 * like intel and amd on x86.
	 * kvm_arch_init must be called before kvm_irqfd_init to avoid creating
	 * conflicts in case kvm is already setup for another implementation.
	 */
	r = kvm_irqfd_init();
	if (r)
		goto out_irqfd;
>>>>>>> refs/remotes/origin/master

	if (!zalloc_cpumask_var(&cpus_hardware_enabled, GFP_KERNEL)) {
		r = -ENOMEM;
		goto out_free_0;
	}

	r = kvm_arch_hardware_setup();
	if (r < 0)
		goto out_free_0a;

	for_each_online_cpu(cpu) {
		smp_call_function_single(cpu,
				kvm_arch_check_processor_compat,
				&r, 1);
		if (r < 0)
			goto out_free_1;
	}

	r = register_cpu_notifier(&kvm_cpu_notifier);
	if (r)
		goto out_free_2;
	register_reboot_notifier(&kvm_reboot_notifier);

	/* A kmem cache lets us meet the alignment requirements of fx_save. */
	if (!vcpu_align)
		vcpu_align = __alignof__(struct kvm_vcpu);
	kvm_vcpu_cache = kmem_cache_create("kvm_vcpu", vcpu_size, vcpu_align,
					   0, NULL);
	if (!kvm_vcpu_cache) {
		r = -ENOMEM;
		goto out_free_3;
	}

	r = kvm_async_pf_init();
	if (r)
		goto out_free;

	kvm_chardev_ops.owner = module;
	kvm_vm_fops.owner = module;
	kvm_vcpu_fops.owner = module;

	r = misc_register(&kvm_dev);
	if (r) {
		printk(KERN_ERR "kvm: misc device register failed\n");
		goto out_unreg;
	}

	register_syscore_ops(&kvm_syscore_ops);

	kvm_preempt_ops.sched_in = kvm_sched_in;
	kvm_preempt_ops.sched_out = kvm_sched_out;

<<<<<<< HEAD
<<<<<<< HEAD
	kvm_init_debug();

	return 0;

=======
=======
>>>>>>> refs/remotes/origin/master
	r = kvm_init_debug();
	if (r) {
		printk(KERN_ERR "kvm: create debugfs files failed\n");
		goto out_undebugfs;
	}

	return 0;

out_undebugfs:
	unregister_syscore_ops(&kvm_syscore_ops);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
	misc_deregister(&kvm_dev);
>>>>>>> refs/remotes/origin/master
out_unreg:
	kvm_async_pf_deinit();
out_free:
	kmem_cache_destroy(kvm_vcpu_cache);
out_free_3:
	unregister_reboot_notifier(&kvm_reboot_notifier);
	unregister_cpu_notifier(&kvm_cpu_notifier);
out_free_2:
out_free_1:
	kvm_arch_hardware_unsetup();
out_free_0a:
	free_cpumask_var(cpus_hardware_enabled);
out_free_0:
<<<<<<< HEAD
	if (fault_page)
		__free_page(fault_page);
	if (hwpoison_page)
		__free_page(hwpoison_page);
	__free_page(bad_page);
out:
=======
	kvm_irqfd_exit();
out_irqfd:
>>>>>>> refs/remotes/origin/master
	kvm_arch_exit();
out_fail:
	return r;
}
EXPORT_SYMBOL_GPL(kvm_init);

void kvm_exit(void)
{
	kvm_exit_debug();
	misc_deregister(&kvm_dev);
	kmem_cache_destroy(kvm_vcpu_cache);
	kvm_async_pf_deinit();
	unregister_syscore_ops(&kvm_syscore_ops);
	unregister_reboot_notifier(&kvm_reboot_notifier);
	unregister_cpu_notifier(&kvm_cpu_notifier);
	on_each_cpu(hardware_disable_nolock, NULL, 1);
	kvm_arch_hardware_unsetup();
	kvm_arch_exit();
<<<<<<< HEAD
	free_cpumask_var(cpus_hardware_enabled);
	__free_page(hwpoison_page);
	__free_page(bad_page);
=======
	kvm_irqfd_exit();
	free_cpumask_var(cpus_hardware_enabled);
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL_GPL(kvm_exit);
