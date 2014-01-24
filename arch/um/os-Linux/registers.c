/*
 * Copyright (C) 2004 PathScale, Inc
 * Copyright (C) 2004 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
 */

#include <errno.h>
#include <string.h>
#include <sys/ptrace.h>
<<<<<<< HEAD
#include "sysdep/ptrace.h"
<<<<<<< HEAD
=======
#include "sysdep/ptrace_user.h"
#include "registers.h"
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <sysdep/ptrace.h>
#include <sysdep/ptrace_user.h>
#include <registers.h>
>>>>>>> refs/remotes/origin/master

int save_registers(int pid, struct uml_pt_regs *regs)
{
	int err;

	err = ptrace(PTRACE_GETREGS, pid, 0, regs->gp);
	if (err < 0)
		return -errno;
	return 0;
}

int restore_registers(int pid, struct uml_pt_regs *regs)
{
	int err;

	err = ptrace(PTRACE_SETREGS, pid, 0, regs->gp);
	if (err < 0)
		return -errno;
	return 0;
}

/* This is set once at boot time and not changed thereafter */

static unsigned long exec_regs[MAX_REG_NR];
<<<<<<< HEAD
<<<<<<< HEAD
=======
static unsigned long exec_fp_regs[FP_SIZE];
>>>>>>> refs/remotes/origin/cm-10.0
=======
static unsigned long exec_fp_regs[FP_SIZE];
>>>>>>> refs/remotes/origin/master

int init_registers(int pid)
{
	int err;

	err = ptrace(PTRACE_GETREGS, pid, 0, exec_regs);
	if (err < 0)
		return -errno;

	arch_init_registers(pid);
<<<<<<< HEAD
<<<<<<< HEAD
	return 0;
}

void get_safe_registers(unsigned long *regs)
{
	memcpy(regs, exec_regs, sizeof(exec_regs));
=======
=======
>>>>>>> refs/remotes/origin/master
	get_fp_registers(pid, exec_fp_regs);
	return 0;
}

void get_safe_registers(unsigned long *regs, unsigned long *fp_regs)
{
	memcpy(regs, exec_regs, sizeof(exec_regs));

	if (fp_regs)
		memcpy(fp_regs, exec_fp_regs, sizeof(exec_fp_regs));
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}
