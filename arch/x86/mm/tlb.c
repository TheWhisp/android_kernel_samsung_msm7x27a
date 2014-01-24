#include <linux/init.h>

#include <linux/mm.h>
#include <linux/spinlock.h>
#include <linux/smp.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/cpu.h>

#include <asm/tlbflush.h>
#include <asm/mmu_context.h>
#include <asm/cache.h>
#include <asm/apic.h>
#include <asm/uv/uv.h>
<<<<<<< HEAD
=======
#include <linux/debugfs.h>
>>>>>>> refs/remotes/origin/master

DEFINE_PER_CPU_SHARED_ALIGNED(struct tlb_state, cpu_tlbstate)
			= { &init_mm, 0, };

/*
 *	Smarter SMP flushing macros.
 *		c/o Linus Torvalds.
 *
 *	These mean you can really definitely utterly forget about
 *	writing to user space from interrupts. (Its not allowed anyway).
 *
 *	Optimizations Manfred Spraul <manfred@colorfullife.com>
 *
 *	More scalable flush, from Andi Kleen
 *
<<<<<<< HEAD
 *	To avoid global state use 8 different call vectors.
 *	Each CPU uses a specific vector to trigger flushes on other
 *	CPUs. Depending on the received vector the target CPUs look into
 *	the right array slot for the flush data.
 *
 *	With more than 8 CPUs they are hashed to the 8 available
 *	vectors. The limited global vector space forces us to this right now.
 *	In future when interrupts are split into per CPU domains this could be
 *	fixed, at the cost of triggering multiple IPIs in some cases.
 */

union smp_flush_state {
	struct {
		struct mm_struct *flush_mm;
		unsigned long flush_va;
		raw_spinlock_t tlbstate_lock;
		DECLARE_BITMAP(flush_cpumask, NR_CPUS);
	};
	char pad[INTERNODE_CACHE_BYTES];
} ____cacheline_internodealigned_in_smp;

/* State is put into the per CPU data section, but padded
   to a full cache line because other CPUs can access it and we don't
   want false sharing in the per cpu data segment. */
static union smp_flush_state flush_state[NUM_INVALIDATE_TLB_VECTORS];

static DEFINE_PER_CPU_READ_MOSTLY(int, tlb_vector_offset);
=======
 *	Implement flush IPI by CALL_FUNCTION_VECTOR, Alex Shi
 */

struct flush_tlb_info {
	struct mm_struct *flush_mm;
	unsigned long flush_start;
	unsigned long flush_end;
};
>>>>>>> refs/remotes/origin/master

/*
 * We cannot call mmdrop() because we are in interrupt context,
 * instead update mm->cpu_vm_mask.
 */
void leave_mm(int cpu)
{
<<<<<<< HEAD
	if (percpu_read(cpu_tlbstate.state) == TLBSTATE_OK)
		BUG();
	cpumask_clear_cpu(cpu,
			  mm_cpumask(percpu_read(cpu_tlbstate.active_mm)));
	load_cr3(swapper_pg_dir);
=======
	struct mm_struct *active_mm = this_cpu_read(cpu_tlbstate.active_mm);
	if (this_cpu_read(cpu_tlbstate.state) == TLBSTATE_OK)
		BUG();
	if (cpumask_test_cpu(cpu, mm_cpumask(active_mm))) {
		cpumask_clear_cpu(cpu, mm_cpumask(active_mm));
		load_cr3(swapper_pg_dir);
	}
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL_GPL(leave_mm);

/*
<<<<<<< HEAD
 *
=======
>>>>>>> refs/remotes/origin/master
 * The flush IPI assumes that a thread switch happens in this order:
 * [cpu0: the cpu that switches]
 * 1) switch_mm() either 1a) or 1b)
 * 1a) thread switch to a different mm
<<<<<<< HEAD
 * 1a1) cpu_clear(cpu, old_mm->cpu_vm_mask);
 *	Stop ipi delivery for the old mm. This is not synchronized with
 *	the other cpus, but smp_invalidate_interrupt ignore flush ipis
 *	for the wrong mm, and in the worst case we perform a superfluous
 *	tlb flush.
 * 1a2) set cpu mmu_state to TLBSTATE_OK
 *	Now the smp_invalidate_interrupt won't call leave_mm if cpu0
 *	was in lazy tlb mode.
 * 1a3) update cpu active_mm
 *	Now cpu0 accepts tlb flushes for the new mm.
 * 1a4) cpu_set(cpu, new_mm->cpu_vm_mask);
 *	Now the other cpus will send tlb flush ipis.
 * 1a4) change cr3.
 * 1b) thread switch without mm change
 *	cpu active_mm is correct, cpu0 already handles
 *	flush ipis.
 * 1b1) set cpu mmu_state to TLBSTATE_OK
=======
 * 1a1) set cpu_tlbstate to TLBSTATE_OK
 *	Now the tlb flush NMI handler flush_tlb_func won't call leave_mm
 *	if cpu0 was in lazy tlb mode.
 * 1a2) update cpu active_mm
 *	Now cpu0 accepts tlb flushes for the new mm.
 * 1a3) cpu_set(cpu, new_mm->cpu_vm_mask);
 *	Now the other cpus will send tlb flush ipis.
 * 1a4) change cr3.
 * 1a5) cpu_clear(cpu, old_mm->cpu_vm_mask);
 *	Stop ipi delivery for the old mm. This is not synchronized with
 *	the other cpus, but flush_tlb_func ignore flush ipis for the wrong
 *	mm, and in the worst case we perform a superfluous tlb flush.
 * 1b) thread switch without mm change
 *	cpu active_mm is correct, cpu0 already handles flush ipis.
 * 1b1) set cpu_tlbstate to TLBSTATE_OK
>>>>>>> refs/remotes/origin/master
 * 1b2) test_and_set the cpu bit in cpu_vm_mask.
 *	Atomically set the bit [other cpus will start sending flush ipis],
 *	and test the bit.
 * 1b3) if the bit was 0: leave_mm was called, flush the tlb.
 * 2) switch %%esp, ie current
 *
 * The interrupt must handle 2 special cases:
 * - cr3 is changed before %%esp, ie. it cannot use current->{active_,}mm.
 * - the cpu performs speculative tlb reads, i.e. even if the cpu only
 *   runs in kernel space, the cpu could load tlb entries for user space
 *   pages.
 *
<<<<<<< HEAD
 * The good news is that cpu mmu_state is local to each cpu, no
=======
 * The good news is that cpu_tlbstate is local to each cpu, no
>>>>>>> refs/remotes/origin/master
 * write/read ordering problems.
 */

/*
<<<<<<< HEAD
 * TLB flush IPI:
 *
 * 1) Flush the tlb entries if the cpu uses the mm that's being flushed.
 * 2) Leave the mm if we are in the lazy tlb mode.
 *
 * Interrupts are disabled.
 */

/*
 * FIXME: use of asmlinkage is not consistent.  On x86_64 it's noop
 * but still used for documentation purpose but the usage is slightly
 * inconsistent.  On x86_32, asmlinkage is regparm(0) but interrupt
 * entry calls in with the first parameter in %eax.  Maybe define
 * intrlinkage?
 */
#ifdef CONFIG_X86_64
asmlinkage
#endif
void smp_invalidate_interrupt(struct pt_regs *regs)
{
	unsigned int cpu;
	unsigned int sender;
	union smp_flush_state *f;

	cpu = smp_processor_id();
	/*
	 * orig_rax contains the negated interrupt vector.
	 * Use that to determine where the sender put the data.
	 */
	sender = ~regs->orig_ax - INVALIDATE_TLB_VECTOR_START;
	f = &flush_state[sender];

	if (!cpumask_test_cpu(cpu, to_cpumask(f->flush_cpumask)))
		goto out;
		/*
		 * This was a BUG() but until someone can quote me the
		 * line from the intel manual that guarantees an IPI to
		 * multiple CPUs is retried _only_ on the erroring CPUs
		 * its staying as a return
		 *
		 * BUG();
		 */

	if (f->flush_mm == percpu_read(cpu_tlbstate.active_mm)) {
		if (percpu_read(cpu_tlbstate.state) == TLBSTATE_OK) {
			if (f->flush_va == TLB_FLUSH_ALL)
				local_flush_tlb();
			else
				__flush_tlb_one(f->flush_va);
		} else
			leave_mm(cpu);
	}
out:
	ack_APIC_irq();
	smp_mb__before_clear_bit();
	cpumask_clear_cpu(cpu, to_cpumask(f->flush_cpumask));
	smp_mb__after_clear_bit();
	inc_irq_stat(irq_tlb_count);
}

static void flush_tlb_others_ipi(const struct cpumask *cpumask,
				 struct mm_struct *mm, unsigned long va)
{
	unsigned int sender;
	union smp_flush_state *f;

	/* Caller has disabled preemption */
	sender = this_cpu_read(tlb_vector_offset);
	f = &flush_state[sender];

	if (nr_cpu_ids > NUM_INVALIDATE_TLB_VECTORS)
		raw_spin_lock(&f->tlbstate_lock);

	f->flush_mm = mm;
	f->flush_va = va;
	if (cpumask_andnot(to_cpumask(f->flush_cpumask), cpumask, cpumask_of(smp_processor_id()))) {
		/*
		 * We have to send the IPI only to
		 * CPUs affected.
		 */
		apic->send_IPI_mask(to_cpumask(f->flush_cpumask),
			      INVALIDATE_TLB_VECTOR_START + sender);

		while (!cpumask_empty(to_cpumask(f->flush_cpumask)))
			cpu_relax();
	}

	f->flush_mm = NULL;
	f->flush_va = 0;
	if (nr_cpu_ids > NUM_INVALIDATE_TLB_VECTORS)
		raw_spin_unlock(&f->tlbstate_lock);
}

void native_flush_tlb_others(const struct cpumask *cpumask,
			     struct mm_struct *mm, unsigned long va)
{
=======
 * TLB flush funcation:
 * 1) Flush the tlb entries if the cpu uses the mm that's being flushed.
 * 2) Leave the mm if we are in the lazy tlb mode.
 */
static void flush_tlb_func(void *info)
{
	struct flush_tlb_info *f = info;

	inc_irq_stat(irq_tlb_count);

	if (f->flush_mm != this_cpu_read(cpu_tlbstate.active_mm))
		return;

	count_vm_event(NR_TLB_REMOTE_FLUSH_RECEIVED);
	if (this_cpu_read(cpu_tlbstate.state) == TLBSTATE_OK) {
		if (f->flush_end == TLB_FLUSH_ALL)
			local_flush_tlb();
		else if (!f->flush_end)
			__flush_tlb_single(f->flush_start);
		else {
			unsigned long addr;
			addr = f->flush_start;
			while (addr < f->flush_end) {
				__flush_tlb_single(addr);
				addr += PAGE_SIZE;
			}
		}
	} else
		leave_mm(smp_processor_id());

}

void native_flush_tlb_others(const struct cpumask *cpumask,
				 struct mm_struct *mm, unsigned long start,
				 unsigned long end)
{
	struct flush_tlb_info info;
	info.flush_mm = mm;
	info.flush_start = start;
	info.flush_end = end;

	count_vm_event(NR_TLB_REMOTE_FLUSH);
>>>>>>> refs/remotes/origin/master
	if (is_uv_system()) {
		unsigned int cpu;

		cpu = smp_processor_id();
<<<<<<< HEAD
		cpumask = uv_flush_tlb_others(cpumask, mm, va, cpu);
		if (cpumask)
			flush_tlb_others_ipi(cpumask, mm, va);
		return;
	}
	flush_tlb_others_ipi(cpumask, mm, va);
}

static void __cpuinit calculate_tlb_offset(void)
{
	int cpu, node, nr_node_vecs, idx = 0;
	/*
	 * we are changing tlb_vector_offset for each CPU in runtime, but this
	 * will not cause inconsistency, as the write is atomic under X86. we
	 * might see more lock contentions in a short time, but after all CPU's
	 * tlb_vector_offset are changed, everything should go normal
	 *
	 * Note: if NUM_INVALIDATE_TLB_VECTORS % nr_online_nodes !=0, we might
	 * waste some vectors.
	 **/
	if (nr_online_nodes > NUM_INVALIDATE_TLB_VECTORS)
		nr_node_vecs = 1;
	else
		nr_node_vecs = NUM_INVALIDATE_TLB_VECTORS/nr_online_nodes;

	for_each_online_node(node) {
		int node_offset = (idx % NUM_INVALIDATE_TLB_VECTORS) *
			nr_node_vecs;
		int cpu_offset = 0;
		for_each_cpu(cpu, cpumask_of_node(node)) {
			per_cpu(tlb_vector_offset, cpu) = node_offset +
				cpu_offset;
			cpu_offset++;
			cpu_offset = cpu_offset % nr_node_vecs;
		}
		idx++;
	}
}

static int __cpuinit tlb_cpuhp_notify(struct notifier_block *n,
		unsigned long action, void *hcpu)
{
	switch (action & 0xf) {
	case CPU_ONLINE:
	case CPU_DEAD:
		calculate_tlb_offset();
	}
	return NOTIFY_OK;
}

static int __cpuinit init_smp_flush(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(flush_state); i++)
		raw_spin_lock_init(&flush_state[i].tlbstate_lock);

	calculate_tlb_offset();
	hotcpu_notifier(tlb_cpuhp_notify, 0);
	return 0;
}
core_initcall(init_smp_flush);

=======
		cpumask = uv_flush_tlb_others(cpumask, mm, start, end, cpu);
		if (cpumask)
			smp_call_function_many(cpumask, flush_tlb_func,
								&info, 1);
		return;
	}
	smp_call_function_many(cpumask, flush_tlb_func, &info, 1);
}

>>>>>>> refs/remotes/origin/master
void flush_tlb_current_task(void)
{
	struct mm_struct *mm = current->mm;

	preempt_disable();

<<<<<<< HEAD
	local_flush_tlb();
	if (cpumask_any_but(mm_cpumask(mm), smp_processor_id()) < nr_cpu_ids)
		flush_tlb_others(mm_cpumask(mm), mm, TLB_FLUSH_ALL);
	preempt_enable();
}

void flush_tlb_mm(struct mm_struct *mm)
{
	preempt_disable();

	if (current->active_mm == mm) {
		if (current->mm)
			local_flush_tlb();
		else
			leave_mm(smp_processor_id());
	}
	if (cpumask_any_but(mm_cpumask(mm), smp_processor_id()) < nr_cpu_ids)
		flush_tlb_others(mm_cpumask(mm), mm, TLB_FLUSH_ALL);

	preempt_enable();
}

void flush_tlb_page(struct vm_area_struct *vma, unsigned long va)
=======
	count_vm_event(NR_TLB_LOCAL_FLUSH_ALL);
	local_flush_tlb();
	if (cpumask_any_but(mm_cpumask(mm), smp_processor_id()) < nr_cpu_ids)
		flush_tlb_others(mm_cpumask(mm), mm, 0UL, TLB_FLUSH_ALL);
	preempt_enable();
}

/*
 * It can find out the THP large page, or
 * HUGETLB page in tlb_flush when THP disabled
 */
static inline unsigned long has_large_page(struct mm_struct *mm,
				 unsigned long start, unsigned long end)
{
	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;
	unsigned long addr = ALIGN(start, HPAGE_SIZE);
	for (; addr < end; addr += HPAGE_SIZE) {
		pgd = pgd_offset(mm, addr);
		if (likely(!pgd_none(*pgd))) {
			pud = pud_offset(pgd, addr);
			if (likely(!pud_none(*pud))) {
				pmd = pmd_offset(pud, addr);
				if (likely(!pmd_none(*pmd)))
					if (pmd_large(*pmd))
						return addr;
			}
		}
	}
	return 0;
}

void flush_tlb_mm_range(struct mm_struct *mm, unsigned long start,
				unsigned long end, unsigned long vmflag)
{
	unsigned long addr;
	unsigned act_entries, tlb_entries = 0;

	preempt_disable();
	if (current->active_mm != mm)
		goto flush_all;

	if (!current->mm) {
		leave_mm(smp_processor_id());
		goto flush_all;
	}

	if (end == TLB_FLUSH_ALL || tlb_flushall_shift == -1
					|| vmflag & VM_HUGETLB) {
		local_flush_tlb();
		goto flush_all;
	}

	/* In modern CPU, last level tlb used for both data/ins */
	if (vmflag & VM_EXEC)
		tlb_entries = tlb_lli_4k[ENTRIES];
	else
		tlb_entries = tlb_lld_4k[ENTRIES];
	/* Assume all of TLB entries was occupied by this task */
	act_entries = mm->total_vm > tlb_entries ? tlb_entries : mm->total_vm;

	/* tlb_flushall_shift is on balance point, details in commit log */
	if ((end - start) >> PAGE_SHIFT > act_entries >> tlb_flushall_shift) {
		count_vm_event(NR_TLB_LOCAL_FLUSH_ALL);
		local_flush_tlb();
	} else {
		if (has_large_page(mm, start, end)) {
			local_flush_tlb();
			goto flush_all;
		}
		/* flush range by one by one 'invlpg' */
		for (addr = start; addr < end;	addr += PAGE_SIZE) {
			count_vm_event(NR_TLB_LOCAL_FLUSH_ONE);
			__flush_tlb_single(addr);
		}

		if (cpumask_any_but(mm_cpumask(mm),
				smp_processor_id()) < nr_cpu_ids)
			flush_tlb_others(mm_cpumask(mm), mm, start, end);
		preempt_enable();
		return;
	}

flush_all:
	if (cpumask_any_but(mm_cpumask(mm), smp_processor_id()) < nr_cpu_ids)
		flush_tlb_others(mm_cpumask(mm), mm, 0UL, TLB_FLUSH_ALL);
	preempt_enable();
}

void flush_tlb_page(struct vm_area_struct *vma, unsigned long start)
>>>>>>> refs/remotes/origin/master
{
	struct mm_struct *mm = vma->vm_mm;

	preempt_disable();

	if (current->active_mm == mm) {
		if (current->mm)
<<<<<<< HEAD
			__flush_tlb_one(va);
=======
			__flush_tlb_one(start);
>>>>>>> refs/remotes/origin/master
		else
			leave_mm(smp_processor_id());
	}

	if (cpumask_any_but(mm_cpumask(mm), smp_processor_id()) < nr_cpu_ids)
<<<<<<< HEAD
		flush_tlb_others(mm_cpumask(mm), mm, va);
=======
		flush_tlb_others(mm_cpumask(mm), mm, start, 0UL);
>>>>>>> refs/remotes/origin/master

	preempt_enable();
}

static void do_flush_tlb_all(void *info)
{
<<<<<<< HEAD
	__flush_tlb_all();
	if (percpu_read(cpu_tlbstate.state) == TLBSTATE_LAZY)
=======
	count_vm_event(NR_TLB_REMOTE_FLUSH_RECEIVED);
	__flush_tlb_all();
	if (this_cpu_read(cpu_tlbstate.state) == TLBSTATE_LAZY)
>>>>>>> refs/remotes/origin/master
		leave_mm(smp_processor_id());
}

void flush_tlb_all(void)
{
<<<<<<< HEAD
	on_each_cpu(do_flush_tlb_all, NULL, 1);
}
=======
	count_vm_event(NR_TLB_REMOTE_FLUSH);
	on_each_cpu(do_flush_tlb_all, NULL, 1);
}

static void do_kernel_range_flush(void *info)
{
	struct flush_tlb_info *f = info;
	unsigned long addr;

	/* flush range by one by one 'invlpg' */
	for (addr = f->flush_start; addr < f->flush_end; addr += PAGE_SIZE)
		__flush_tlb_single(addr);
}

void flush_tlb_kernel_range(unsigned long start, unsigned long end)
{
	unsigned act_entries;
	struct flush_tlb_info info;

	/* In modern CPU, last level tlb used for both data/ins */
	act_entries = tlb_lld_4k[ENTRIES];

	/* Balance as user space task's flush, a bit conservative */
	if (end == TLB_FLUSH_ALL || tlb_flushall_shift == -1 ||
		(end - start) >> PAGE_SHIFT > act_entries >> tlb_flushall_shift)

		on_each_cpu(do_flush_tlb_all, NULL, 1);
	else {
		info.flush_start = start;
		info.flush_end = end;
		on_each_cpu(do_kernel_range_flush, &info, 1);
	}
}

#ifdef CONFIG_DEBUG_TLBFLUSH
static ssize_t tlbflush_read_file(struct file *file, char __user *user_buf,
			     size_t count, loff_t *ppos)
{
	char buf[32];
	unsigned int len;

	len = sprintf(buf, "%hd\n", tlb_flushall_shift);
	return simple_read_from_buffer(user_buf, count, ppos, buf, len);
}

static ssize_t tlbflush_write_file(struct file *file,
		 const char __user *user_buf, size_t count, loff_t *ppos)
{
	char buf[32];
	ssize_t len;
	s8 shift;

	len = min(count, sizeof(buf) - 1);
	if (copy_from_user(buf, user_buf, len))
		return -EFAULT;

	buf[len] = '\0';
	if (kstrtos8(buf, 0, &shift))
		return -EINVAL;

	if (shift < -1 || shift >= BITS_PER_LONG)
		return -EINVAL;

	tlb_flushall_shift = shift;
	return count;
}

static const struct file_operations fops_tlbflush = {
	.read = tlbflush_read_file,
	.write = tlbflush_write_file,
	.llseek = default_llseek,
};

static int __init create_tlb_flushall_shift(void)
{
	debugfs_create_file("tlb_flushall_shift", S_IRUSR | S_IWUSR,
			    arch_debugfs_dir, NULL, &fops_tlbflush);
	return 0;
}
late_initcall(create_tlb_flushall_shift);
#endif
>>>>>>> refs/remotes/origin/master
