/*
 * Dynamic function tracing support.
 *
 * Copyright (C) 2008 Abhishek Sagar <sagar.abhishek@gmail.com>
 * Copyright (C) 2010 Rabin Vincent <rabin@rab.in>
 *
 * For licencing details, see COPYING.
 *
 * Defines low-level handling of mcount calls when the kernel
 * is compiled with the -pg flag. When using dynamic ftrace, the
 * mcount call-sites get patched with NOP till they are enabled.
 * All code mutation routines here are called under stop_machine().
 */

#include <linux/ftrace.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/uaccess.h>

#include <asm/cacheflush.h>
#include <asm/ftrace.h>

#ifdef CONFIG_THUMB2_KERNEL
#define	NOP		0xeb04f85d	/* pop.w {lr} */
=======
#include <linux/module.h>
=======
>>>>>>> refs/remotes/origin/master
#include <linux/uaccess.h>

#include <asm/cacheflush.h>
#include <asm/opcodes.h>
#include <asm/ftrace.h>

#include "insn.h"

#ifdef CONFIG_THUMB2_KERNEL
#define	NOP		0xf85deb04	/* pop.w {lr} */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#else
#define	NOP		0xe8bd4000	/* pop {lr} */
#endif

#ifdef CONFIG_DYNAMIC_FTRACE
#ifdef CONFIG_OLD_MCOUNT
#define OLD_MCOUNT_ADDR	((unsigned long) mcount)
#define OLD_FTRACE_ADDR ((unsigned long) ftrace_caller_old)

#define	OLD_NOP		0xe1a00000	/* mov r0, r0 */

static unsigned long ftrace_nop_replace(struct dyn_ftrace *rec)
{
	return rec->arch.old_mcount ? OLD_NOP : NOP;
}

static unsigned long adjust_address(struct dyn_ftrace *rec, unsigned long addr)
{
	if (!rec->arch.old_mcount)
		return addr;

	if (addr == MCOUNT_ADDR)
		addr = OLD_MCOUNT_ADDR;
	else if (addr == FTRACE_ADDR)
		addr = OLD_FTRACE_ADDR;

	return addr;
}
#else
static unsigned long ftrace_nop_replace(struct dyn_ftrace *rec)
{
	return NOP;
}

static unsigned long adjust_address(struct dyn_ftrace *rec, unsigned long addr)
{
	return addr;
}
#endif

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_THUMB2_KERNEL
static unsigned long ftrace_gen_branch(unsigned long pc, unsigned long addr,
				       bool link)
{
	unsigned long s, j1, j2, i1, i2, imm10, imm11;
	unsigned long first, second;
	long offset;

	offset = (long)addr - (long)(pc + 4);
	if (offset < -16777216 || offset > 16777214) {
		WARN_ON_ONCE(1);
		return 0;
	}

	s	= (offset >> 24) & 0x1;
	i1	= (offset >> 23) & 0x1;
	i2	= (offset >> 22) & 0x1;
	imm10	= (offset >> 12) & 0x3ff;
	imm11	= (offset >>  1) & 0x7ff;

	j1 = (!i1) ^ s;
	j2 = (!i2) ^ s;

	first = 0xf000 | (s << 10) | imm10;
	second = 0x9000 | (j1 << 13) | (j2 << 11) | imm11;
	if (link)
		second |= 1 << 14;

	return (second << 16) | first;
}
#else
static unsigned long ftrace_gen_branch(unsigned long pc, unsigned long addr,
				       bool link)
{
	unsigned long opcode = 0xea000000;
	long offset;

	if (link)
		opcode |= 1 << 24;

	offset = (long)addr - (long)(pc + 8);
	if (unlikely(offset < -33554432 || offset > 33554428)) {
		/* Can't generate branches that far (from ARM ARM). Ftrace
		 * doesn't generate branches outside of kernel text.
		 */
		WARN_ON_ONCE(1);
		return 0;
	}

	offset = (offset >> 2) & 0x00ffffff;

	return opcode | offset;
}
#endif

static unsigned long ftrace_call_replace(unsigned long pc, unsigned long addr)
{
	return ftrace_gen_branch(pc, addr, true);
}

static int ftrace_modify_code(unsigned long pc, unsigned long old,
			      unsigned long new)
{
	unsigned long replaced;

	if (probe_kernel_read(&replaced, (void *)pc, MCOUNT_INSN_SIZE))
		return -EFAULT;

	if (replaced != old)
		return -EINVAL;
=======
int ftrace_arch_code_modify_prepare(void)
{
	set_kernel_text_rw();
	set_all_modules_text_rw();
	return 0;
}

int ftrace_arch_code_modify_post_process(void)
{
	set_all_modules_text_ro();
	set_kernel_text_ro();
	return 0;
}

int ftrace_arch_code_modify_prepare(void)
{
	set_kernel_text_rw();
	set_all_modules_text_rw();
	return 0;
}

int ftrace_arch_code_modify_post_process(void)
{
	set_all_modules_text_ro();
	set_kernel_text_ro();
	return 0;
}

=======
>>>>>>> refs/remotes/origin/master
static unsigned long ftrace_call_replace(unsigned long pc, unsigned long addr)
{
	return arm_gen_branch_link(pc, addr);
}

static int ftrace_modify_code(unsigned long pc, unsigned long old,
			      unsigned long new, bool validate)
{
	unsigned long replaced;

	if (IS_ENABLED(CONFIG_THUMB2_KERNEL)) {
		old = __opcode_to_mem_thumb32(old);
		new = __opcode_to_mem_thumb32(new);
	} else {
		old = __opcode_to_mem_arm(old);
		new = __opcode_to_mem_arm(new);
	}

	if (validate) {
		if (probe_kernel_read(&replaced, (void *)pc, MCOUNT_INSN_SIZE))
			return -EFAULT;

		if (replaced != old)
			return -EINVAL;
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	if (probe_kernel_write((void *)pc, &new, MCOUNT_INSN_SIZE))
		return -EPERM;

	flush_icache_range(pc, pc + MCOUNT_INSN_SIZE);

	return 0;
}

int ftrace_update_ftrace_func(ftrace_func_t func)
{
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long pc, old;
=======
	unsigned long pc;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	unsigned long pc;
>>>>>>> refs/remotes/origin/master
	unsigned long new;
	int ret;

	pc = (unsigned long)&ftrace_call;
<<<<<<< HEAD
<<<<<<< HEAD
	memcpy(&old, &ftrace_call, MCOUNT_INSN_SIZE);
	new = ftrace_call_replace(pc, (unsigned long)func);

	ret = ftrace_modify_code(pc, old, new);
=======
	new = ftrace_call_replace(pc, (unsigned long)func);

	ret = ftrace_modify_code(pc, 0, new, false);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	new = ftrace_call_replace(pc, (unsigned long)func);

	ret = ftrace_modify_code(pc, 0, new, false);
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_OLD_MCOUNT
	if (!ret) {
		pc = (unsigned long)&ftrace_call_old;
<<<<<<< HEAD
<<<<<<< HEAD
		memcpy(&old, &ftrace_call_old, MCOUNT_INSN_SIZE);
		new = ftrace_call_replace(pc, (unsigned long)func);

		ret = ftrace_modify_code(pc, old, new);
=======
		new = ftrace_call_replace(pc, (unsigned long)func);

		ret = ftrace_modify_code(pc, 0, new, false);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		new = ftrace_call_replace(pc, (unsigned long)func);

		ret = ftrace_modify_code(pc, 0, new, false);
>>>>>>> refs/remotes/origin/master
	}
#endif

	return ret;
}

int ftrace_make_call(struct dyn_ftrace *rec, unsigned long addr)
{
	unsigned long new, old;
	unsigned long ip = rec->ip;

	old = ftrace_nop_replace(rec);
	new = ftrace_call_replace(ip, adjust_address(rec, addr));

<<<<<<< HEAD
<<<<<<< HEAD
	return ftrace_modify_code(rec->ip, old, new);
=======
	return ftrace_modify_code(rec->ip, old, new, true);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return ftrace_modify_code(rec->ip, old, new, true);
>>>>>>> refs/remotes/origin/master
}

int ftrace_make_nop(struct module *mod,
		    struct dyn_ftrace *rec, unsigned long addr)
{
	unsigned long ip = rec->ip;
	unsigned long old;
	unsigned long new;
	int ret;

	old = ftrace_call_replace(ip, adjust_address(rec, addr));
	new = ftrace_nop_replace(rec);
<<<<<<< HEAD
<<<<<<< HEAD
	ret = ftrace_modify_code(ip, old, new);
=======
	ret = ftrace_modify_code(ip, old, new, true);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ret = ftrace_modify_code(ip, old, new, true);
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_OLD_MCOUNT
	if (ret == -EINVAL && addr == MCOUNT_ADDR) {
		rec->arch.old_mcount = true;

		old = ftrace_call_replace(ip, adjust_address(rec, addr));
		new = ftrace_nop_replace(rec);
<<<<<<< HEAD
<<<<<<< HEAD
		ret = ftrace_modify_code(ip, old, new);
=======
		ret = ftrace_modify_code(ip, old, new, true);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		ret = ftrace_modify_code(ip, old, new, true);
>>>>>>> refs/remotes/origin/master
	}
#endif

	return ret;
}

int __init ftrace_dyn_arch_init(void *data)
{
	*(unsigned long *)data = 0;

	return 0;
}
#endif /* CONFIG_DYNAMIC_FTRACE */

#ifdef CONFIG_FUNCTION_GRAPH_TRACER
void prepare_ftrace_return(unsigned long *parent, unsigned long self_addr,
			   unsigned long frame_pointer)
{
	unsigned long return_hooker = (unsigned long) &return_to_handler;
	struct ftrace_graph_ent trace;
	unsigned long old;
	int err;

	if (unlikely(atomic_read(&current->tracing_graph_pause)))
		return;

	old = *parent;
	*parent = return_hooker;

<<<<<<< HEAD
	err = ftrace_push_return_trace(old, self_addr, &trace.depth,
				       frame_pointer);
	if (err == -EBUSY) {
		*parent = old;
		return;
	}

	trace.func = self_addr;

	/* Only trace if the calling function expects to */
	if (!ftrace_graph_entry(&trace)) {
		current->curr_ret_stack--;
		*parent = old;
=======
	trace.func = self_addr;
	trace.depth = current->curr_ret_stack + 1;

	/* Only trace if the calling function expects to */
	if (!ftrace_graph_entry(&trace)) {
		*parent = old;
		return;
	}

	err = ftrace_push_return_trace(old, self_addr, &trace.depth,
				       frame_pointer);
	if (err == -EBUSY) {
		*parent = old;
		return;
>>>>>>> refs/remotes/origin/master
	}
}

#ifdef CONFIG_DYNAMIC_FTRACE
extern unsigned long ftrace_graph_call;
extern unsigned long ftrace_graph_call_old;
extern void ftrace_graph_caller_old(void);

static int __ftrace_modify_caller(unsigned long *callsite,
				  void (*func) (void), bool enable)
{
	unsigned long caller_fn = (unsigned long) func;
	unsigned long pc = (unsigned long) callsite;
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long branch = ftrace_gen_branch(pc, caller_fn, false);
=======
	unsigned long branch = arm_gen_branch(pc, caller_fn);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	unsigned long branch = arm_gen_branch(pc, caller_fn);
>>>>>>> refs/remotes/origin/master
	unsigned long nop = 0xe1a00000;	/* mov r0, r0 */
	unsigned long old = enable ? nop : branch;
	unsigned long new = enable ? branch : nop;

<<<<<<< HEAD
<<<<<<< HEAD
	return ftrace_modify_code(pc, old, new);
=======
	return ftrace_modify_code(pc, old, new, true);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return ftrace_modify_code(pc, old, new, true);
>>>>>>> refs/remotes/origin/master
}

static int ftrace_modify_graph_caller(bool enable)
{
	int ret;

	ret = __ftrace_modify_caller(&ftrace_graph_call,
				     ftrace_graph_caller,
				     enable);

#ifdef CONFIG_OLD_MCOUNT
	if (!ret)
		ret = __ftrace_modify_caller(&ftrace_graph_call_old,
					     ftrace_graph_caller_old,
					     enable);
#endif

	return ret;
}

int ftrace_enable_ftrace_graph_caller(void)
{
	return ftrace_modify_graph_caller(true);
}

int ftrace_disable_ftrace_graph_caller(void)
{
	return ftrace_modify_graph_caller(false);
}
#endif /* CONFIG_DYNAMIC_FTRACE */
#endif /* CONFIG_FUNCTION_GRAPH_TRACER */
