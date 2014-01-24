/*
 * Copyright (C) 2000 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
 */

<<<<<<< HEAD
<<<<<<< HEAD
#include "linux/stddef.h"
#include "linux/fs.h"
#include "linux/ptrace.h"
#include "linux/sched.h"
#include "linux/slab.h"
#include "asm/current.h"
#include "asm/processor.h"
#include "asm/uaccess.h"
=======
=======
>>>>>>> refs/remotes/origin/master
#include <linux/stddef.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/ptrace.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <asm/current.h>
#include <asm/processor.h>
#include <asm/uaccess.h>
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
#include "as-layout.h"
#include "mem_user.h"
#include "skas.h"
#include "os.h"
#include "internal.h"
=======
#include <as-layout.h>
#include <mem_user.h>
#include <skas.h>
#include <os.h>
>>>>>>> refs/remotes/origin/master

void flush_thread(void)
{
	void *data = NULL;
	int ret;

	arch_flush_thread(&current->thread.arch);

	ret = unmap(&current->mm->context.id, 0, STUB_START, 0, &data);
	ret = ret || unmap(&current->mm->context.id, STUB_END,
			   host_task_size - STUB_END, 1, &data);
	if (ret) {
		printk(KERN_ERR "flush_thread - clearing address space failed, "
		       "err = %d\n", ret);
		force_sig(SIGKILL, current);
	}
<<<<<<< HEAD
=======
	get_safe_registers(current_pt_regs()->regs.gp,
			   current_pt_regs()->regs.fp);
>>>>>>> refs/remotes/origin/master

	__switch_mm(&current->mm->context.id);
}

void start_thread(struct pt_regs *regs, unsigned long eip, unsigned long esp)
{
<<<<<<< HEAD
<<<<<<< HEAD
	set_fs(USER_DS);
	PT_REGS_IP(regs) = eip;
	PT_REGS_SP(regs) = esp;
}
=======
	PT_REGS_IP(regs) = eip;
	PT_REGS_SP(regs) = esp;
}
EXPORT_SYMBOL(start_thread);
>>>>>>> refs/remotes/origin/cm-10.0

static long execve1(const char *file,
		    const char __user *const __user *argv,
		    const char __user *const __user *env)
{
	long error;

	error = do_execve(file, argv, env, &current->thread.regs);
	if (error == 0) {
		task_lock(current);
		current->ptrace &= ~PT_DTRACE;
#ifdef SUBARCH_EXECVE1
		SUBARCH_EXECVE1(&current->thread.regs.regs);
#endif
		task_unlock(current);
	}
	return error;
}

long um_execve(const char *file, const char __user *const __user *argv, const char __user *const __user *env)
{
	long err;

	err = execve1(file, argv, env);
	if (!err)
		UML_LONGJMP(current->thread.exec_buf, 1);
	return err;
}

long sys_execve(const char __user *file, const char __user *const __user *argv,
		const char __user *const __user *env)
{
	long error;
	char *filename;

	filename = getname(file);
	error = PTR_ERR(filename);
	if (IS_ERR(filename)) goto out;
	error = execve1(filename, argv, env);
	putname(filename);
 out:
	return error;
}
=======
	PT_REGS_IP(regs) = eip;
	PT_REGS_SP(regs) = esp;
	current->ptrace &= ~PT_DTRACE;
#ifdef SUBARCH_EXECVE1
	SUBARCH_EXECVE1(regs->regs);
#endif
}
EXPORT_SYMBOL(start_thread);
>>>>>>> refs/remotes/origin/master
