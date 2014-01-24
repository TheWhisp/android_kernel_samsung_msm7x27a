/*
 * kernel/stacktrace.c
 *
 * Stack trace management functions
 *
 *  Copyright (C) 2006 Red Hat, Inc., Ingo Molnar <mingo@redhat.com>
 */
#include <linux/sched.h>
#include <linux/kernel.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/module.h>
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master
#include <linux/kallsyms.h>
#include <linux/stacktrace.h>

void print_stack_trace(struct stack_trace *trace, int spaces)
{
	int i;

	if (WARN_ON(!trace->entries))
		return;

	for (i = 0; i < trace->nr_entries; i++) {
		printk("%*c", 1 + spaces, ' ');
		print_ip_sym(trace->entries[i]);
	}
}
EXPORT_SYMBOL_GPL(print_stack_trace);

/*
<<<<<<< HEAD
<<<<<<< HEAD
 * Architectures that do not implement save_stack_trace_tsk get this
 * weak alias and a once-per-bootup warning (whenever this facility
 * is utilized - for example by procfs):
=======
 * Architectures that do not implement save_stack_trace_tsk or
 * save_stack_trace_regs get this weak alias and a once-per-bootup warning
 * (whenever this facility is utilized - for example by procfs):
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Architectures that do not implement save_stack_trace_tsk or
 * save_stack_trace_regs get this weak alias and a once-per-bootup warning
 * (whenever this facility is utilized - for example by procfs):
>>>>>>> refs/remotes/origin/master
 */
__weak void
save_stack_trace_tsk(struct task_struct *tsk, struct stack_trace *trace)
{
	WARN_ONCE(1, KERN_INFO "save_stack_trace_tsk() not implemented yet.\n");
}
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master

__weak void
save_stack_trace_regs(struct pt_regs *regs, struct stack_trace *trace)
{
	WARN_ONCE(1, KERN_INFO "save_stack_trace_regs() not implemented yet.\n");
}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
