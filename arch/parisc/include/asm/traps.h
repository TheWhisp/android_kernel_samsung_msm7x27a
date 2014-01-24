#ifndef __ASM_TRAPS_H
#define __ASM_TRAPS_H

#ifdef __KERNEL__
struct pt_regs;

/* traps.c */
void parisc_terminate(char *msg, struct pt_regs *regs,
<<<<<<< HEAD
		int code, unsigned long offset);
=======
		int code, unsigned long offset) __noreturn __cold;
>>>>>>> refs/remotes/origin/master

/* mm/fault.c */
void do_page_fault(struct pt_regs *regs, unsigned long code,
		unsigned long address);
#endif

#endif
