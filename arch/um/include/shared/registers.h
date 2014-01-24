/*
 * Copyright (C) 2004 PathScale, Inc
 * Licensed under the GPL
 */

#ifndef __REGISTERS_H
#define __REGISTERS_H

<<<<<<< HEAD
#include "sysdep/ptrace.h"
#include "sysdep/archsetjmp.h"
=======
#include <sysdep/ptrace.h>
#include <sysdep/archsetjmp.h>
>>>>>>> refs/remotes/origin/master

extern int save_fp_registers(int pid, unsigned long *fp_regs);
extern int restore_fp_registers(int pid, unsigned long *fp_regs);
extern int save_fpx_registers(int pid, unsigned long *fp_regs);
extern int restore_fpx_registers(int pid, unsigned long *fp_regs);
extern int save_registers(int pid, struct uml_pt_regs *regs);
extern int restore_registers(int pid, struct uml_pt_regs *regs);
extern int init_registers(int pid);
<<<<<<< HEAD
<<<<<<< HEAD
extern void get_safe_registers(unsigned long *regs);
=======
extern void get_safe_registers(unsigned long *regs, unsigned long *fp_regs);
>>>>>>> refs/remotes/origin/cm-10.0
=======
extern void get_safe_registers(unsigned long *regs, unsigned long *fp_regs);
>>>>>>> refs/remotes/origin/master
extern unsigned long get_thread_reg(int reg, jmp_buf *buf);
extern int get_fp_registers(int pid, unsigned long *regs);
extern int put_fp_registers(int pid, unsigned long *regs);

#endif
