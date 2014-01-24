/*
 * Copyright 2010 Tilera Corporation. All Rights Reserved.
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation, version 2.
 *
 *   This program is distributed in the hope that it will be useful, but
 *   WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
 *   NON INFRINGEMENT.  See the GNU General Public License for
 *   more details.
 */

#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/kprobes.h>
#include <linux/module.h>
#include <linux/pfn.h>
#include <linux/kallsyms.h>
#include <linux/stacktrace.h>
#include <linux/uaccess.h>
#include <linux/mmzone.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/backtrace.h>
#include <asm/page.h>
#include <asm/tlbflush.h>
#include <asm/ucontext.h>
=======
#include <linux/dcache.h>
#include <linux/fs.h>
=======
#include <linux/dcache.h>
#include <linux/fs.h>
#include <linux/string.h>
>>>>>>> refs/remotes/origin/master
#include <asm/backtrace.h>
#include <asm/page.h>
#include <asm/ucontext.h>
#include <asm/switch_to.h>
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
#include <asm/sigframe.h>
#include <asm/stack.h>
=======
#include <asm/sigframe.h>
#include <asm/stack.h>
#include <asm/vdso.h>
>>>>>>> refs/remotes/origin/master
#include <arch/abi.h>
#include <arch/interrupts.h>

#define KBT_ONGOING	0  /* Backtrace still ongoing */
#define KBT_DONE	1  /* Backtrace cleanly completed */
#define KBT_RUNNING	2  /* Can't run backtrace on a running task */
#define KBT_LOOP	3  /* Backtrace entered a loop */

/* Is address on the specified kernel stack? */
static int in_kernel_stack(struct KBacktraceIterator *kbt, unsigned long sp)
{
	ulong kstack_base = (ulong) kbt->task->stack;
	if (kstack_base == 0)  /* corrupt task pointer; just follow stack... */
		return sp >= PAGE_OFFSET && sp < (unsigned long)high_memory;
	return sp >= kstack_base && sp < kstack_base + THREAD_SIZE;
}

<<<<<<< HEAD
<<<<<<< HEAD
/* Is address valid for reading? */
static int valid_address(struct KBacktraceIterator *kbt, unsigned long address)
{
	HV_PTE *l1_pgtable = kbt->pgtable;
	HV_PTE *l2_pgtable;
	unsigned long pfn;
	HV_PTE pte;
	struct page *page;

	if (l1_pgtable == NULL)
		return 0;	/* can't read user space in other tasks */

#ifdef CONFIG_64BIT
	/* Find the real l1_pgtable by looking in the l0_pgtable. */
	pte = l1_pgtable[HV_L0_INDEX(address)];
	if (!hv_pte_get_present(pte))
		return 0;
	pfn = hv_pte_get_pfn(pte);
	if (pte_huge(pte)) {
		if (!pfn_valid(pfn)) {
			pr_err("L0 huge page has bad pfn %#lx\n", pfn);
			return 0;
		}
		return hv_pte_get_present(pte) && hv_pte_get_readable(pte);
	}
	page = pfn_to_page(pfn);
	BUG_ON(PageHighMem(page));  /* No HIGHMEM on 64-bit. */
	l1_pgtable = (HV_PTE *)pfn_to_kaddr(pfn);
#endif
	pte = l1_pgtable[HV_L1_INDEX(address)];
	if (!hv_pte_get_present(pte))
		return 0;
	pfn = hv_pte_get_pfn(pte);
	if (pte_huge(pte)) {
		if (!pfn_valid(pfn)) {
			pr_err("huge page has bad pfn %#lx\n", pfn);
			return 0;
		}
		return hv_pte_get_present(pte) && hv_pte_get_readable(pte);
	}

	page = pfn_to_page(pfn);
	if (PageHighMem(page)) {
		pr_err("L2 page table not in LOWMEM (%#llx)\n",
		       HV_PFN_TO_CPA(pfn));
		return 0;
	}
	l2_pgtable = (HV_PTE *)pfn_to_kaddr(pfn);
	pte = l2_pgtable[HV_L2_INDEX(address)];
	return hv_pte_get_present(pte) && hv_pte_get_readable(pte);
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/* Callback for backtracer; basically a glorified memcpy */
static bool read_memory_func(void *result, unsigned long address,
			     unsigned int size, void *vkbt)
{
	int retval;
	struct KBacktraceIterator *kbt = (struct KBacktraceIterator *)vkbt;
<<<<<<< HEAD
<<<<<<< HEAD
=======

	if (address == 0)
		return 0;
>>>>>>> refs/remotes/origin/cm-10.0
=======

	if (address == 0)
		return 0;
>>>>>>> refs/remotes/origin/master
	if (__kernel_text_address(address)) {
		/* OK to read kernel code. */
	} else if (address >= PAGE_OFFSET) {
		/* We only tolerate kernel-space reads of this task's stack */
		if (!in_kernel_stack(kbt, address))
			return 0;
<<<<<<< HEAD
<<<<<<< HEAD
	} else if (!valid_address(kbt, address)) {
		return 0;	/* invalid user-space address */
=======
	} else if (!kbt->is_current) {
		return 0;	/* can't read from other user address spaces */
>>>>>>> refs/remotes/origin/cm-10.0
=======
	} else if (!kbt->is_current) {
		return 0;	/* can't read from other user address spaces */
>>>>>>> refs/remotes/origin/master
	}
	pagefault_disable();
	retval = __copy_from_user_inatomic(result,
					   (void __user __force *)address,
					   size);
	pagefault_enable();
	return (retval == 0);
}

/* Return a pt_regs pointer for a valid fault handler frame */
static struct pt_regs *valid_fault_handler(struct KBacktraceIterator* kbt)
{
	const char *fault = NULL;  /* happy compiler */
	char fault_buf[64];
	unsigned long sp = kbt->it.sp;
	struct pt_regs *p;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (sp % sizeof(long) != 0)
		return NULL;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (sp % sizeof(long) != 0)
		return NULL;
>>>>>>> refs/remotes/origin/master
	if (!in_kernel_stack(kbt, sp))
		return NULL;
	if (!in_kernel_stack(kbt, sp + C_ABI_SAVE_AREA_SIZE + PTREGS_SIZE-1))
		return NULL;
	p = (struct pt_regs *)(sp + C_ABI_SAVE_AREA_SIZE);
	if (p->faultnum == INT_SWINT_1 || p->faultnum == INT_SWINT_1_SIGRETURN)
		fault = "syscall";
	else {
		if (kbt->verbose) {     /* else we aren't going to use it */
			snprintf(fault_buf, sizeof(fault_buf),
				 "interrupt %ld", p->faultnum);
			fault = fault_buf;
		}
	}
	if (EX1_PL(p->ex1) == KERNEL_PL &&
	    __kernel_text_address(p->pc) &&
	    in_kernel_stack(kbt, p->sp) &&
	    p->sp >= sp) {
		if (kbt->verbose)
			pr_err("  <%s while in kernel mode>\n", fault);
<<<<<<< HEAD
	} else if (EX1_PL(p->ex1) == USER_PL &&
	    p->pc < PAGE_OFFSET &&
	    p->sp < PAGE_OFFSET) {
=======
	} else if (user_mode(p) &&
		   p->sp < PAGE_OFFSET && p->sp != 0) {
>>>>>>> refs/remotes/origin/master
		if (kbt->verbose)
			pr_err("  <%s while in user mode>\n", fault);
	} else if (kbt->verbose) {
		pr_err("  (odd fault: pc %#lx, sp %#lx, ex1 %#lx?)\n",
		       p->pc, p->sp, p->ex1);
		p = NULL;
	}
<<<<<<< HEAD
	if (!kbt->profile || (INT_MASK(p->faultnum) & QUEUED_INTERRUPTS) == 0)
=======
	if (!kbt->profile || ((1ULL << p->faultnum) & QUEUED_INTERRUPTS) == 0)
>>>>>>> refs/remotes/origin/master
		return p;
	return NULL;
}

/* Is the pc pointing to a sigreturn trampoline? */
static int is_sigreturn(unsigned long pc)
{
<<<<<<< HEAD
	return (pc == VDSO_BASE);
}

/* Return a pt_regs pointer for a valid signal handler frame */
<<<<<<< HEAD
static struct pt_regs *valid_sigframe(struct KBacktraceIterator* kbt)
{
	BacktraceIterator *b = &kbt->it;

	if (b->pc == VDSO_BASE) {
		struct rt_sigframe *frame;
		unsigned long sigframe_top =
			b->sp + sizeof(struct rt_sigframe) - 1;
		if (!valid_address(kbt, b->sp) ||
		    !valid_address(kbt, sigframe_top)) {
			if (kbt->verbose)
				pr_err("  (odd signal: sp %#lx?)\n",
				       (unsigned long)(b->sp));
			return NULL;
		}
		frame = (struct rt_sigframe *)b->sp;
		if (kbt->verbose) {
			pr_err("  <received signal %d>\n",
			       frame->info.si_signo);
		}
		return (struct pt_regs *)&frame->uc.uc_mcontext;
=======
=======
	return current->mm && (pc == VDSO_SYM(&__vdso_rt_sigreturn));
}

/* Return a pt_regs pointer for a valid signal handler frame */
>>>>>>> refs/remotes/origin/master
static struct pt_regs *valid_sigframe(struct KBacktraceIterator* kbt,
				      struct rt_sigframe* kframe)
{
	BacktraceIterator *b = &kbt->it;

<<<<<<< HEAD
	if (b->pc == VDSO_BASE && b->sp < PAGE_OFFSET &&
=======
	if (is_sigreturn(b->pc) && b->sp < PAGE_OFFSET &&
>>>>>>> refs/remotes/origin/master
	    b->sp % sizeof(long) == 0) {
		int retval;
		pagefault_disable();
		retval = __copy_from_user_inatomic(
			kframe, (void __user __force *)b->sp,
			sizeof(*kframe));
		pagefault_enable();
		if (retval != 0 ||
		    (unsigned int)(kframe->info.si_signo) >= _NSIG)
			return NULL;
		if (kbt->verbose) {
			pr_err("  <received signal %d>\n",
			       kframe->info.si_signo);
		}
		return (struct pt_regs *)&kframe->uc.uc_mcontext;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}
	return NULL;
}

static int KBacktraceIterator_is_sigreturn(struct KBacktraceIterator *kbt)
{
	return is_sigreturn(kbt->it.pc);
}

static int KBacktraceIterator_restart(struct KBacktraceIterator *kbt)
{
	struct pt_regs *p;
<<<<<<< HEAD
<<<<<<< HEAD

	p = valid_fault_handler(kbt);
	if (p == NULL)
		p = valid_sigframe(kbt);
=======
=======
>>>>>>> refs/remotes/origin/master
	struct rt_sigframe kframe;

	p = valid_fault_handler(kbt);
	if (p == NULL)
		p = valid_sigframe(kbt, &kframe);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (p == NULL)
		return 0;
	backtrace_init(&kbt->it, read_memory_func, kbt,
		       p->pc, p->lr, p->sp, p->regs[52]);
	kbt->new_context = 1;
	return 1;
}

/* Find a frame that isn't a sigreturn, if there is one. */
static int KBacktraceIterator_next_item_inclusive(
	struct KBacktraceIterator *kbt)
{
	for (;;) {
		do {
			if (!KBacktraceIterator_is_sigreturn(kbt))
				return KBT_ONGOING;
		} while (backtrace_next(&kbt->it));

		if (!KBacktraceIterator_restart(kbt))
			return KBT_DONE;
	}
}

/*
 * If the current sp is on a page different than what we recorded
 * as the top-of-kernel-stack last time we context switched, we have
 * probably blown the stack, and nothing is going to work out well.
 * If we can at least get out a warning, that may help the debug,
 * though we probably won't be able to backtrace into the code that
 * actually did the recursive damage.
 */
static void validate_stack(struct pt_regs *regs)
{
<<<<<<< HEAD
	int cpu = smp_processor_id();
	unsigned long ksp0 = get_current_ksp0();
	unsigned long ksp0_base = ksp0 - THREAD_SIZE;
	unsigned long sp = stack_pointer;

	if (EX1_PL(regs->ex1) == KERNEL_PL && regs->sp >= ksp0) {
		pr_err("WARNING: cpu %d: kernel stack page %#lx underrun!\n"
		       "  sp %#lx (%#lx in caller), caller pc %#lx, lr %#lx\n",
		       cpu, ksp0_base, sp, regs->sp, regs->pc, regs->lr);
	}

	else if (sp < ksp0_base + sizeof(struct thread_info)) {
		pr_err("WARNING: cpu %d: kernel stack page %#lx overrun!\n"
		       "  sp %#lx (%#lx in caller), caller pc %#lx, lr %#lx\n",
		       cpu, ksp0_base, sp, regs->sp, regs->pc, regs->lr);
=======
	int cpu = raw_smp_processor_id();
	unsigned long ksp0 = get_current_ksp0();
	unsigned long ksp0_base = ksp0 & -THREAD_SIZE;
	unsigned long sp = stack_pointer;

	if (EX1_PL(regs->ex1) == KERNEL_PL && regs->sp >= ksp0) {
		pr_err("WARNING: cpu %d: kernel stack %#lx..%#lx underrun!\n"
		       "  sp %#lx (%#lx in caller), caller pc %#lx, lr %#lx\n",
		       cpu, ksp0_base, ksp0, sp, regs->sp, regs->pc, regs->lr);
	}

	else if (sp < ksp0_base + sizeof(struct thread_info)) {
		pr_err("WARNING: cpu %d: kernel stack %#lx..%#lx overrun!\n"
		       "  sp %#lx (%#lx in caller), caller pc %#lx, lr %#lx\n",
		       cpu, ksp0_base, ksp0, sp, regs->sp, regs->pc, regs->lr);
>>>>>>> refs/remotes/origin/master
	}
}

void KBacktraceIterator_init(struct KBacktraceIterator *kbt,
			     struct task_struct *t, struct pt_regs *regs)
{
	unsigned long pc, lr, sp, r52;
	int is_current;

	/*
	 * Set up callback information.  We grab the kernel stack base
<<<<<<< HEAD
<<<<<<< HEAD
	 * so we will allow reads of that address range, and if we're
	 * asking about the current process we grab the page table
	 * so we can check user accesses before trying to read them.
	 * We flush the TLB to avoid any weird skew issues.
	 */
	is_current = (t == NULL);
=======
	 * so we will allow reads of that address range.
	 */
	is_current = (t == NULL || t == current);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	 * so we will allow reads of that address range.
	 */
	is_current = (t == NULL || t == current);
>>>>>>> refs/remotes/origin/master
	kbt->is_current = is_current;
	if (is_current)
		t = validate_current();
	kbt->task = t;
<<<<<<< HEAD
<<<<<<< HEAD
	kbt->pgtable = NULL;
	kbt->verbose = 0;   /* override in caller if desired */
	kbt->profile = 0;   /* override in caller if desired */
	kbt->end = KBT_ONGOING;
	kbt->new_context = 0;
	if (is_current) {
		HV_PhysAddr pgdir_pa = hv_inquire_context().page_table;
		if (pgdir_pa == (unsigned long)swapper_pg_dir - PAGE_OFFSET) {
			/*
			 * Not just an optimization: this also allows
			 * this to work at all before va/pa mappings
			 * are set up.
			 */
			kbt->pgtable = swapper_pg_dir;
		} else {
			struct page *page = pfn_to_page(PFN_DOWN(pgdir_pa));
			if (!PageHighMem(page))
				kbt->pgtable = __va(pgdir_pa);
			else
				pr_err("page table not in LOWMEM"
				       " (%#llx)\n", pgdir_pa);
		}
		local_flush_tlb_all();
		validate_stack(regs);
	}
=======
=======
>>>>>>> refs/remotes/origin/master
	kbt->verbose = 0;   /* override in caller if desired */
	kbt->profile = 0;   /* override in caller if desired */
	kbt->end = KBT_ONGOING;
	kbt->new_context = 1;
	if (is_current)
		validate_stack(regs);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	if (regs == NULL) {
		if (is_current || t->state == TASK_RUNNING) {
			/* Can't do this; we need registers */
			kbt->end = KBT_RUNNING;
			return;
		}
		pc = get_switch_to_pc();
		lr = t->thread.pc;
		sp = t->thread.ksp;
		r52 = 0;
	} else {
		pc = regs->pc;
		lr = regs->lr;
		sp = regs->sp;
		r52 = regs->regs[52];
	}

	backtrace_init(&kbt->it, read_memory_func, kbt, pc, lr, sp, r52);
	kbt->end = KBacktraceIterator_next_item_inclusive(kbt);
}
EXPORT_SYMBOL(KBacktraceIterator_init);

int KBacktraceIterator_end(struct KBacktraceIterator *kbt)
{
	return kbt->end != KBT_ONGOING;
}
EXPORT_SYMBOL(KBacktraceIterator_end);

void KBacktraceIterator_next(struct KBacktraceIterator *kbt)
{
	unsigned long old_pc = kbt->it.pc, old_sp = kbt->it.sp;
	kbt->new_context = 0;
	if (!backtrace_next(&kbt->it) && !KBacktraceIterator_restart(kbt)) {
		kbt->end = KBT_DONE;
		return;
	}
	kbt->end = KBacktraceIterator_next_item_inclusive(kbt);
	if (old_pc == kbt->it.pc && old_sp == kbt->it.sp) {
		/* Trapped in a loop; give up. */
		kbt->end = KBT_LOOP;
	}
}
EXPORT_SYMBOL(KBacktraceIterator_next);

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static void describe_addr(struct KBacktraceIterator *kbt,
			  unsigned long address,
			  int have_mmap_sem, char *buf, size_t bufsize)
{
	struct vm_area_struct *vma;
	size_t namelen, remaining;
	unsigned long size, offset, adjust;
	char *p, *modname;
	const char *name;
	int rc;

	/*
	 * Look one byte back for every caller frame (i.e. those that
	 * aren't a new context) so we look up symbol data for the
	 * call itself, not the following instruction, which may be on
	 * a different line (or in a different function).
	 */
	adjust = !kbt->new_context;
	address -= adjust;

	if (address >= PAGE_OFFSET) {
		/* Handle kernel symbols. */
		BUG_ON(bufsize < KSYM_NAME_LEN);
		name = kallsyms_lookup(address, &size, &offset,
				       &modname, buf);
		if (name == NULL) {
			buf[0] = '\0';
			return;
		}
		namelen = strlen(buf);
		remaining = (bufsize - 1) - namelen;
		p = buf + namelen;
		rc = snprintf(p, remaining, "+%#lx/%#lx ",
			      offset + adjust, size);
		if (modname && rc < remaining)
			snprintf(p + rc, remaining - rc, "[%s] ", modname);
		buf[bufsize-1] = '\0';
		return;
	}

	/* If we don't have the mmap_sem, we can't show any more info. */
	buf[0] = '\0';
	if (!have_mmap_sem)
		return;

	/* Find vma info. */
	vma = find_vma(kbt->task->mm, address);
	if (vma == NULL || address < vma->vm_start) {
		snprintf(buf, bufsize, "[unmapped address] ");
		return;
	}

	if (vma->vm_file) {
<<<<<<< HEAD
		char *s;
		p = d_path(&vma->vm_file->f_path, buf, bufsize);
		if (IS_ERR(p))
			p = "?";
		s = strrchr(p, '/');
		if (s)
			p = s+1;
	} else {
		p = "anon";
	}

	/* Generate a string description of the vma info. */
	namelen = strlen(p);
	remaining = (bufsize - 1) - namelen;
	memmove(buf, p, namelen);
=======
		p = d_path(&vma->vm_file->f_path, buf, bufsize);
		if (IS_ERR(p))
			p = "?";
		name = kbasename(p);
	} else {
		name = "anon";
	}

	/* Generate a string description of the vma info. */
	namelen = strlen(name);
	remaining = (bufsize - 1) - namelen;
	memmove(buf, name, namelen);
>>>>>>> refs/remotes/origin/master
	snprintf(buf + namelen, remaining, "[%lx+%lx] ",
		 vma->vm_start, vma->vm_end - vma->vm_start);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
/*
 * Avoid possible crash recursion during backtrace.  If it happens, it
 * makes it easy to lose the actual root cause of the failure, so we
 * put a simple guard on all the backtrace loops.
 */
static bool start_backtrace(void)
{
	if (current->thread.in_backtrace) {
		pr_err("Backtrace requested while in backtrace!\n");
		return false;
	}
	current->thread.in_backtrace = true;
	return true;
}

static void end_backtrace(void)
{
	current->thread.in_backtrace = false;
}

>>>>>>> refs/remotes/origin/master
/*
 * This method wraps the backtracer's more generic support.
 * It is only invoked from the architecture-specific code; show_stack()
 * and dump_stack() (in entry.S) are architecture-independent entry points.
 */
void tile_show_stack(struct KBacktraceIterator *kbt, int headers)
{
	int i;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	int have_mmap_sem = 0;
>>>>>>> refs/remotes/origin/cm-10.0

=======
	int have_mmap_sem = 0;

	if (!start_backtrace())
		return;
>>>>>>> refs/remotes/origin/master
	if (headers) {
		/*
		 * Add a blank line since if we are called from panic(),
		 * then bust_spinlocks() spit out a space in front of us
		 * and it will mess up our KERN_ERR.
		 */
		pr_err("\n");
		pr_err("Starting stack dump of tid %d, pid %d (%s)"
		       " on cpu %d at cycle %lld\n",
		       kbt->task->pid, kbt->task->tgid, kbt->task->comm,
<<<<<<< HEAD
		       smp_processor_id(), get_cycles());
=======
		       raw_smp_processor_id(), get_cycles());
>>>>>>> refs/remotes/origin/master
	}
	kbt->verbose = 1;
	i = 0;
	for (; !KBacktraceIterator_end(kbt); KBacktraceIterator_next(kbt)) {
<<<<<<< HEAD
<<<<<<< HEAD
		char *modname;
		const char *name;
		unsigned long address = kbt->it.pc;
		unsigned long offset, size;
		char namebuf[KSYM_NAME_LEN+100];

		if (address >= PAGE_OFFSET)
			name = kallsyms_lookup(address, &size, &offset,
					       &modname, namebuf);
		else
			name = NULL;

		if (!name)
			namebuf[0] = '\0';
		else {
			size_t namelen = strlen(namebuf);
			size_t remaining = (sizeof(namebuf) - 1) - namelen;
			char *p = namebuf + namelen;
			int rc = snprintf(p, remaining, "+%#lx/%#lx ",
					  offset, size);
			if (modname && rc < remaining)
				snprintf(p + rc, remaining - rc,
					 "[%s] ", modname);
			namebuf[sizeof(namebuf)-1] = '\0';
		}
=======
=======
>>>>>>> refs/remotes/origin/master
		char namebuf[KSYM_NAME_LEN+100];
		unsigned long address = kbt->it.pc;

		/* Try to acquire the mmap_sem as we pass into userspace. */
		if (address < PAGE_OFFSET && !have_mmap_sem && kbt->task->mm)
			have_mmap_sem =
				down_read_trylock(&kbt->task->mm->mmap_sem);

		describe_addr(kbt, address, have_mmap_sem,
			      namebuf, sizeof(namebuf));
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

		pr_err("  frame %d: 0x%lx %s(sp 0x%lx)\n",
		       i++, address, namebuf, (unsigned long)(kbt->it.sp));

		if (i >= 100) {
			pr_err("Stack dump truncated"
			       " (%d frames)\n", i);
			break;
		}
	}
	if (kbt->end == KBT_LOOP)
		pr_err("Stack dump stopped; next frame identical to this one\n");
	if (headers)
		pr_err("Stack dump complete\n");
<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (have_mmap_sem)
		up_read(&kbt->task->mm->mmap_sem);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (have_mmap_sem)
		up_read(&kbt->task->mm->mmap_sem);
	end_backtrace();
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL(tile_show_stack);


/* This is called from show_regs() and _dump_stack() */
void dump_stack_regs(struct pt_regs *regs)
{
	struct KBacktraceIterator kbt;
	KBacktraceIterator_init(&kbt, NULL, regs);
	tile_show_stack(&kbt, 1);
}
EXPORT_SYMBOL(dump_stack_regs);

static struct pt_regs *regs_to_pt_regs(struct pt_regs *regs,
				       ulong pc, ulong lr, ulong sp, ulong r52)
{
	memset(regs, 0, sizeof(struct pt_regs));
	regs->pc = pc;
	regs->lr = lr;
	regs->sp = sp;
	regs->regs[52] = r52;
	return regs;
}

/* This is called from dump_stack() and just converts to pt_regs */
void _dump_stack(int dummy, ulong pc, ulong lr, ulong sp, ulong r52)
{
	struct pt_regs regs;
	dump_stack_regs(regs_to_pt_regs(&regs, pc, lr, sp, r52));
}

/* This is called from KBacktraceIterator_init_current() */
void _KBacktraceIterator_init_current(struct KBacktraceIterator *kbt, ulong pc,
				      ulong lr, ulong sp, ulong r52)
{
	struct pt_regs regs;
	KBacktraceIterator_init(kbt, NULL,
				regs_to_pt_regs(&regs, pc, lr, sp, r52));
}

<<<<<<< HEAD
/* This is called only from kernel/sched.c, with esp == NULL */
=======
/* This is called only from kernel/sched/core.c, with esp == NULL */
>>>>>>> refs/remotes/origin/master
void show_stack(struct task_struct *task, unsigned long *esp)
{
	struct KBacktraceIterator kbt;
	if (task == NULL || task == current)
		KBacktraceIterator_init_current(&kbt);
	else
		KBacktraceIterator_init(&kbt, task, NULL);
	tile_show_stack(&kbt, 0);
}

#ifdef CONFIG_STACKTRACE

/* Support generic Linux stack API too */

void save_stack_trace_tsk(struct task_struct *task, struct stack_trace *trace)
{
	struct KBacktraceIterator kbt;
	int skip = trace->skip;
	int i = 0;

<<<<<<< HEAD
=======
	if (!start_backtrace())
		goto done;
>>>>>>> refs/remotes/origin/master
	if (task == NULL || task == current)
		KBacktraceIterator_init_current(&kbt);
	else
		KBacktraceIterator_init(&kbt, task, NULL);
	for (; !KBacktraceIterator_end(&kbt); KBacktraceIterator_next(&kbt)) {
		if (skip) {
			--skip;
			continue;
		}
		if (i >= trace->max_entries || kbt.it.pc < PAGE_OFFSET)
			break;
		trace->entries[i++] = kbt.it.pc;
	}
<<<<<<< HEAD
=======
	end_backtrace();
done:
>>>>>>> refs/remotes/origin/master
	trace->nr_entries = i;
}
EXPORT_SYMBOL(save_stack_trace_tsk);

void save_stack_trace(struct stack_trace *trace)
{
	save_stack_trace_tsk(NULL, trace);
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(save_stack_trace);
>>>>>>> refs/remotes/origin/master

#endif

/* In entry.S */
EXPORT_SYMBOL(KBacktraceIterator_init_current);
