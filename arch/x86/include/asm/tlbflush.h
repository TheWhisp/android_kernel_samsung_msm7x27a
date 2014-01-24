#ifndef _ASM_X86_TLBFLUSH_H
#define _ASM_X86_TLBFLUSH_H

#include <linux/mm.h>
#include <linux/sched.h>

#include <asm/processor.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/system.h>
=======
#include <asm/special_insns.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <asm/special_insns.h>
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_PARAVIRT
#include <asm/paravirt.h>
#else
#define __flush_tlb() __native_flush_tlb()
#define __flush_tlb_global() __native_flush_tlb_global()
#define __flush_tlb_single(addr) __native_flush_tlb_single(addr)
#endif

static inline void __native_flush_tlb(void)
{
	native_write_cr3(native_read_cr3());
}

<<<<<<< HEAD
static inline void __native_flush_tlb_global(void)
{
	unsigned long flags;
	unsigned long cr4;
=======
static inline void __native_flush_tlb_global_irq_disabled(void)
{
	unsigned long cr4;

	cr4 = native_read_cr4();
	/* clear PGE */
	native_write_cr4(cr4 & ~X86_CR4_PGE);
	/* write old PGE again and flush TLBs */
	native_write_cr4(cr4);
}

static inline void __native_flush_tlb_global(void)
{
	unsigned long flags;
>>>>>>> refs/remotes/origin/master

	/*
	 * Read-modify-write to CR4 - protect it from preemption and
	 * from interrupts. (Use the raw variant because this code can
	 * be called from deep inside debugging code.)
	 */
	raw_local_irq_save(flags);

<<<<<<< HEAD
	cr4 = native_read_cr4();
	/* clear PGE */
	native_write_cr4(cr4 & ~X86_CR4_PGE);
	/* write old PGE again and flush TLBs */
	native_write_cr4(cr4);
=======
	__native_flush_tlb_global_irq_disabled();
>>>>>>> refs/remotes/origin/master

	raw_local_irq_restore(flags);
}

static inline void __native_flush_tlb_single(unsigned long addr)
{
	asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
}

static inline void __flush_tlb_all(void)
{
	if (cpu_has_pge)
		__flush_tlb_global();
	else
		__flush_tlb();
}

static inline void __flush_tlb_one(unsigned long addr)
{
<<<<<<< HEAD
	if (cpu_has_invlpg)
		__flush_tlb_single(addr);
	else
		__flush_tlb();
}

#ifdef CONFIG_X86_32
# define TLB_FLUSH_ALL	0xffffffff
#else
# define TLB_FLUSH_ALL	-1ULL
#endif
=======
	count_vm_event(NR_TLB_LOCAL_FLUSH_ONE);
	__flush_tlb_single(addr);
}

#define TLB_FLUSH_ALL	-1UL
>>>>>>> refs/remotes/origin/master

/*
 * TLB flushing:
 *
 *  - flush_tlb() flushes the current mm struct TLBs
 *  - flush_tlb_all() flushes all processes TLBs
 *  - flush_tlb_mm(mm) flushes the specified mm context TLB's
 *  - flush_tlb_page(vma, vmaddr) flushes one page
 *  - flush_tlb_range(vma, start, end) flushes a range of pages
 *  - flush_tlb_kernel_range(start, end) flushes a range of kernel pages
<<<<<<< HEAD
 *  - flush_tlb_others(cpumask, mm, va) flushes TLBs on other cpus
 *
 * ..but the i386 has somewhat limited tlb flushing capabilities,
 * and page-granular flushes are available only on i486 and up.
 *
 * x86-64 can only flush individual pages or full VMs. For a range flush
 * we always do the full VM. Might be worth trying if for a small
 * range a few INVLPGs in a row are a win.
=======
 *  - flush_tlb_others(cpumask, mm, start, end) flushes TLBs on other cpus
 *
 * ..but the i386 has somewhat limited tlb flushing capabilities,
 * and page-granular flushes are available only on i486 and up.
>>>>>>> refs/remotes/origin/master
 */

#ifndef CONFIG_SMP

<<<<<<< HEAD
#define flush_tlb() __flush_tlb()
#define flush_tlb_all() __flush_tlb_all()
#define local_flush_tlb() __flush_tlb()
=======
/* "_up" is for UniProcessor.
 *
 * This is a helper for other header functions.  *Not* intended to be called
 * directly.  All global TLB flushes need to either call this, or to bump the
 * vm statistics themselves.
 */
static inline void __flush_tlb_up(void)
{
	count_vm_event(NR_TLB_LOCAL_FLUSH_ALL);
	__flush_tlb();
}

static inline void flush_tlb_all(void)
{
	count_vm_event(NR_TLB_LOCAL_FLUSH_ALL);
	__flush_tlb_all();
}

static inline void flush_tlb(void)
{
	__flush_tlb_up();
}

static inline void local_flush_tlb(void)
{
	__flush_tlb_up();
}
>>>>>>> refs/remotes/origin/master

static inline void flush_tlb_mm(struct mm_struct *mm)
{
	if (mm == current->active_mm)
<<<<<<< HEAD
		__flush_tlb();
=======
		__flush_tlb_up();
>>>>>>> refs/remotes/origin/master
}

static inline void flush_tlb_page(struct vm_area_struct *vma,
				  unsigned long addr)
{
	if (vma->vm_mm == current->active_mm)
		__flush_tlb_one(addr);
}

static inline void flush_tlb_range(struct vm_area_struct *vma,
				   unsigned long start, unsigned long end)
{
	if (vma->vm_mm == current->active_mm)
<<<<<<< HEAD
		__flush_tlb();
=======
		__flush_tlb_up();
}

static inline void flush_tlb_mm_range(struct mm_struct *mm,
	   unsigned long start, unsigned long end, unsigned long vmflag)
{
	if (mm == current->active_mm)
		__flush_tlb_up();
>>>>>>> refs/remotes/origin/master
}

static inline void native_flush_tlb_others(const struct cpumask *cpumask,
					   struct mm_struct *mm,
<<<<<<< HEAD
					   unsigned long va)
=======
					   unsigned long start,
					   unsigned long end)
>>>>>>> refs/remotes/origin/master
{
}

static inline void reset_lazy_tlbstate(void)
{
}

<<<<<<< HEAD
=======
static inline void flush_tlb_kernel_range(unsigned long start,
					  unsigned long end)
{
	flush_tlb_all();
}

>>>>>>> refs/remotes/origin/master
#else  /* SMP */

#include <asm/smp.h>

#define local_flush_tlb() __flush_tlb()

<<<<<<< HEAD
extern void flush_tlb_all(void);
extern void flush_tlb_current_task(void);
extern void flush_tlb_mm(struct mm_struct *);
extern void flush_tlb_page(struct vm_area_struct *, unsigned long);

#define flush_tlb()	flush_tlb_current_task()

static inline void flush_tlb_range(struct vm_area_struct *vma,
				   unsigned long start, unsigned long end)
{
	flush_tlb_mm(vma->vm_mm);
}

void native_flush_tlb_others(const struct cpumask *cpumask,
			     struct mm_struct *mm, unsigned long va);
=======
#define flush_tlb_mm(mm)	flush_tlb_mm_range(mm, 0UL, TLB_FLUSH_ALL, 0UL)

#define flush_tlb_range(vma, start, end)	\
		flush_tlb_mm_range(vma->vm_mm, start, end, vma->vm_flags)

extern void flush_tlb_all(void);
extern void flush_tlb_current_task(void);
extern void flush_tlb_page(struct vm_area_struct *, unsigned long);
extern void flush_tlb_mm_range(struct mm_struct *mm, unsigned long start,
				unsigned long end, unsigned long vmflag);
extern void flush_tlb_kernel_range(unsigned long start, unsigned long end);

#define flush_tlb()	flush_tlb_current_task()

void native_flush_tlb_others(const struct cpumask *cpumask,
				struct mm_struct *mm,
				unsigned long start, unsigned long end);
>>>>>>> refs/remotes/origin/master

#define TLBSTATE_OK	1
#define TLBSTATE_LAZY	2

struct tlb_state {
	struct mm_struct *active_mm;
	int state;
};
DECLARE_PER_CPU_SHARED_ALIGNED(struct tlb_state, cpu_tlbstate);

static inline void reset_lazy_tlbstate(void)
{
<<<<<<< HEAD
	percpu_write(cpu_tlbstate.state, 0);
	percpu_write(cpu_tlbstate.active_mm, &init_mm);
=======
	this_cpu_write(cpu_tlbstate.state, 0);
	this_cpu_write(cpu_tlbstate.active_mm, &init_mm);
>>>>>>> refs/remotes/origin/master
}

#endif	/* SMP */

#ifndef CONFIG_PARAVIRT
<<<<<<< HEAD
#define flush_tlb_others(mask, mm, va)	native_flush_tlb_others(mask, mm, va)
#endif

static inline void flush_tlb_kernel_range(unsigned long start,
					  unsigned long end)
{
	flush_tlb_all();
}

=======
#define flush_tlb_others(mask, mm, start, end)	\
	native_flush_tlb_others(mask, mm, start, end)
#endif

>>>>>>> refs/remotes/origin/master
#endif /* _ASM_X86_TLBFLUSH_H */
