/*
 * Copyright (C) 2001 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
<<<<<<< HEAD
 * Licensed under the GPL
=======
 * Copyright (C) 2013 Richard Weinberger <richrd@nod.at>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
>>>>>>> refs/remotes/origin/master
 */

#include <linux/kallsyms.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
<<<<<<< HEAD
#include "sysrq.h"

/* Catch non-i386 SUBARCH's. */
#if !defined(CONFIG_UML_X86) || defined(CONFIG_64BIT)
void show_trace(struct task_struct *task, unsigned long * stack)
{
	unsigned long addr;

	if (!stack) {
		stack = (unsigned long*) &stack;
		WARN_ON(1);
	}

	printk(KERN_INFO "Call Trace: \n");
	while (((long) stack & (THREAD_SIZE-1)) != 0) {
		addr = *stack;
		if (__kernel_text_address(addr)) {
			printk(KERN_INFO "%08lx:  [<%08lx>]",
			       (unsigned long) stack, addr);
			print_symbol(KERN_CONT " %s", addr);
			printk(KERN_CONT "\n");
		}
		stack++;
	}
	printk(KERN_INFO "\n");
}
#endif

/*
 * stack dumps generator - this is used by arch-independent code.
 * And this is identical to i386 currently.
 */
void dump_stack(void)
{
	unsigned long stack;

	show_trace(current, &stack);
}
EXPORT_SYMBOL(dump_stack);

/*Stolen from arch/i386/kernel/traps.c */
static const int kstack_depth_to_print = 24;

/* This recently started being used in arch-independent code too, as in
 * kernel/sched.c.*/
void show_stack(struct task_struct *task, unsigned long *esp)
{
	unsigned long *stack;
	int i;

	if (esp == NULL) {
		if (task != current && task != NULL) {
			esp = (unsigned long *) KSTK_ESP(task);
		} else {
			esp = (unsigned long *) &esp;
		}
	}

	stack = esp;
	for (i = 0; i < kstack_depth_to_print; i++) {
		if (kstack_end(stack))
			break;
		if (i && ((i % 8) == 0))
			printk(KERN_INFO "       ");
		printk(KERN_CONT "%08lx ", *stack++);
	}

	show_trace(task, esp);
=======
#include <asm/sysrq.h>
#include <os.h>

struct stack_frame {
	struct stack_frame *next_frame;
	unsigned long return_address;
};

static void do_stack_trace(unsigned long *sp, unsigned long bp)
{
	int reliable;
	unsigned long addr;
	struct stack_frame *frame = (struct stack_frame *)bp;

	printk(KERN_INFO "Call Trace:\n");
	while (((long) sp & (THREAD_SIZE-1)) != 0) {
		addr = *sp;
		if (__kernel_text_address(addr)) {
			reliable = 0;
			if ((unsigned long) sp == bp + sizeof(long)) {
				frame = frame ? frame->next_frame : NULL;
				bp = (unsigned long)frame;
				reliable = 1;
			}

			printk(KERN_INFO " [<%08lx>]", addr);
			printk(KERN_CONT " %s", reliable ? "" : "? ");
			print_symbol(KERN_CONT "%s", addr);
			printk(KERN_CONT "\n");
		}
		sp++;
	}
	printk(KERN_INFO "\n");
}

static unsigned long get_frame_pointer(struct task_struct *task,
				       struct pt_regs *segv_regs)
{
	if (!task || task == current)
		return segv_regs ? PT_REGS_BP(segv_regs) : current_bp();
	else
		return KSTK_EBP(task);
}

static unsigned long *get_stack_pointer(struct task_struct *task,
					struct pt_regs *segv_regs)
{
	if (!task || task == current)
		return segv_regs ? (unsigned long *)PT_REGS_SP(segv_regs) : current_sp();
	else
		return (unsigned long *)KSTK_ESP(task);
}

void show_stack(struct task_struct *task, unsigned long *stack)
{
	unsigned long *sp = stack, bp = 0;
	struct pt_regs *segv_regs = current->thread.segv_regs;
	int i;

	if (!segv_regs && os_is_signal_stack()) {
		printk(KERN_ERR "Received SIGSEGV in SIGSEGV handler,"
				" aborting stack trace!\n");
		return;
	}

#ifdef CONFIG_FRAME_POINTER
	bp = get_frame_pointer(task, segv_regs);
#endif

	if (!stack)
		sp = get_stack_pointer(task, segv_regs);

	printk(KERN_INFO "Stack:\n");
	stack = sp;
	for (i = 0; i < 3 * STACKSLOTS_PER_LINE; i++) {
		if (kstack_end(stack))
			break;
		if (i && ((i % STACKSLOTS_PER_LINE) == 0))
			printk(KERN_CONT "\n");
		printk(KERN_CONT " %08lx", *stack++);
	}
	printk(KERN_CONT "\n");

	do_stack_trace(sp, bp);
>>>>>>> refs/remotes/origin/master
}
