/*
 * Copyright (C) 1999 Cort Dougan <cort@cs.nmt.edu>
 */
#ifndef _ASM_POWERPC_DEBUG_H
#define _ASM_POWERPC_DEBUG_H

<<<<<<< HEAD
=======
#include <asm/hw_breakpoint.h>

>>>>>>> refs/remotes/origin/master
struct pt_regs;

extern struct dentry *powerpc_debugfs_root;

#if defined(CONFIG_DEBUGGER) || defined(CONFIG_KEXEC)

extern int (*__debugger)(struct pt_regs *regs);
extern int (*__debugger_ipi)(struct pt_regs *regs);
extern int (*__debugger_bpt)(struct pt_regs *regs);
extern int (*__debugger_sstep)(struct pt_regs *regs);
extern int (*__debugger_iabr_match)(struct pt_regs *regs);
<<<<<<< HEAD
extern int (*__debugger_dabr_match)(struct pt_regs *regs);
=======
extern int (*__debugger_break_match)(struct pt_regs *regs);
>>>>>>> refs/remotes/origin/master
extern int (*__debugger_fault_handler)(struct pt_regs *regs);

#define DEBUGGER_BOILERPLATE(__NAME) \
static inline int __NAME(struct pt_regs *regs) \
{ \
	if (unlikely(__ ## __NAME)) \
		return __ ## __NAME(regs); \
	return 0; \
}

DEBUGGER_BOILERPLATE(debugger)
DEBUGGER_BOILERPLATE(debugger_ipi)
DEBUGGER_BOILERPLATE(debugger_bpt)
DEBUGGER_BOILERPLATE(debugger_sstep)
DEBUGGER_BOILERPLATE(debugger_iabr_match)
<<<<<<< HEAD
DEBUGGER_BOILERPLATE(debugger_dabr_match)
=======
DEBUGGER_BOILERPLATE(debugger_break_match)
>>>>>>> refs/remotes/origin/master
DEBUGGER_BOILERPLATE(debugger_fault_handler)

#else
static inline int debugger(struct pt_regs *regs) { return 0; }
static inline int debugger_ipi(struct pt_regs *regs) { return 0; }
static inline int debugger_bpt(struct pt_regs *regs) { return 0; }
static inline int debugger_sstep(struct pt_regs *regs) { return 0; }
static inline int debugger_iabr_match(struct pt_regs *regs) { return 0; }
<<<<<<< HEAD
static inline int debugger_dabr_match(struct pt_regs *regs) { return 0; }
static inline int debugger_fault_handler(struct pt_regs *regs) { return 0; }
#endif

extern int set_dabr(unsigned long dabr);
=======
static inline int debugger_break_match(struct pt_regs *regs) { return 0; }
static inline int debugger_fault_handler(struct pt_regs *regs) { return 0; }
#endif

int set_breakpoint(struct arch_hw_breakpoint *brk);
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_PPC_ADV_DEBUG_REGS
extern void do_send_trap(struct pt_regs *regs, unsigned long address,
			 unsigned long error_code, int signal_code, int brkpt);
#else
<<<<<<< HEAD
extern void do_dabr(struct pt_regs *regs, unsigned long address,
		    unsigned long error_code);
=======

extern void do_break(struct pt_regs *regs, unsigned long address,
		     unsigned long error_code);
>>>>>>> refs/remotes/origin/master
#endif

#endif /* _ASM_POWERPC_DEBUG_H */
