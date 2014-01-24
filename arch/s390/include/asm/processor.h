/*
<<<<<<< HEAD
 *  include/asm-s390/processor.h
 *
 *  S390 version
 *    Copyright (C) 1999 IBM Deutschland Entwicklung GmbH, IBM Corporation
=======
 *  S390 version
 *    Copyright IBM Corp. 1999
>>>>>>> refs/remotes/origin/master
 *    Author(s): Hartmut Penner (hp@de.ibm.com),
 *               Martin Schwidefsky (schwidefsky@de.ibm.com)
 *
 *  Derived from "include/asm-i386/processor.h"
 *    Copyright (C) 1994, Linus Torvalds
 */

#ifndef __ASM_S390_PROCESSOR_H
#define __ASM_S390_PROCESSOR_H

<<<<<<< HEAD
#include <linux/linkage.h>
<<<<<<< HEAD
=======
#include <linux/irqflags.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#ifndef __ASSEMBLY__

#include <linux/linkage.h>
#include <linux/irqflags.h>
>>>>>>> refs/remotes/origin/master
#include <asm/cpu.h>
#include <asm/page.h>
#include <asm/ptrace.h>
#include <asm/setup.h>
<<<<<<< HEAD

#ifdef __KERNEL__
=======
#include <asm/runtime_instr.h>

>>>>>>> refs/remotes/origin/master
/*
 * Default implementation of macro that returns current
 * instruction pointer ("program counter").
 */
#define current_text_addr() ({ void *pc; asm("basr %0,0" : "=a" (pc)); pc; })

static inline void get_cpu_id(struct cpuid *ptr)
{
	asm volatile("stidp %0" : "=Q" (*ptr));
}

extern void s390_adjust_jiffies(void);
<<<<<<< HEAD
extern int get_cpu_capability(unsigned int *);
<<<<<<< HEAD
=======
extern const struct seq_operations cpuinfo_op;
extern int sysctl_ieee_emulation_warnings;
>>>>>>> refs/remotes/origin/cm-10.0
=======
extern const struct seq_operations cpuinfo_op;
extern int sysctl_ieee_emulation_warnings;
extern void execve_tail(void);
>>>>>>> refs/remotes/origin/master

/*
 * User space process size: 2GB for 31 bit, 4TB or 8PT for 64 bit.
 */
<<<<<<< HEAD
#ifndef __s390x__

#define TASK_SIZE		(1UL << 31)
#define TASK_UNMAPPED_BASE	(1UL << 30)

#else /* __s390x__ */
=======
#ifndef CONFIG_64BIT

#define TASK_SIZE		(1UL << 31)
#define TASK_MAX_SIZE		(1UL << 31)
#define TASK_UNMAPPED_BASE	(1UL << 30)

#else /* CONFIG_64BIT */
>>>>>>> refs/remotes/origin/master

#define TASK_SIZE_OF(tsk)	((tsk)->mm->context.asce_limit)
#define TASK_UNMAPPED_BASE	(test_thread_flag(TIF_31BIT) ? \
					(1UL << 30) : (1UL << 41))
#define TASK_SIZE		TASK_SIZE_OF(current)
<<<<<<< HEAD

#endif /* __s390x__ */

#ifdef __KERNEL__

#ifndef __s390x__
#define STACK_TOP		(1UL << 31)
#define STACK_TOP_MAX		(1UL << 31)
#else /* __s390x__ */
#define STACK_TOP		(1UL << (test_thread_flag(TIF_31BIT) ? 31:42))
#define STACK_TOP_MAX		(1UL << 42)
#endif /* __s390x__ */


#endif
=======
#define TASK_MAX_SIZE		(1UL << 53)

#endif /* CONFIG_64BIT */

#ifndef CONFIG_64BIT
#define STACK_TOP		(1UL << 31)
#define STACK_TOP_MAX		(1UL << 31)
#else /* CONFIG_64BIT */
#define STACK_TOP		(1UL << (test_thread_flag(TIF_31BIT) ? 31:42))
#define STACK_TOP_MAX		(1UL << 42)
#endif /* CONFIG_64BIT */
>>>>>>> refs/remotes/origin/master

#define HAVE_ARCH_PICK_MMAP_LAYOUT

typedef struct {
        __u32 ar4;
} mm_segment_t;

/*
 * Thread structure
 */
struct thread_struct {
	s390_fp_regs fp_regs;
	unsigned int  acrs[NUM_ACRS];
        unsigned long ksp;              /* kernel stack pointer             */
	mm_segment_t mm_segment;
<<<<<<< HEAD
<<<<<<< HEAD
        unsigned long prot_addr;        /* address of protection-excep.     */
        unsigned int trap_no;
=======
	unsigned long gmap_addr;	/* address of last gmap fault. */
>>>>>>> refs/remotes/origin/cm-10.0
	struct per_regs per_user;	/* User specified PER registers */
	struct per_event per_event;	/* Cause of the last PER trap */
        /* pfault_wait is used to block the process on a pfault event */
	unsigned long pfault_wait;
	struct list_head list;
};

=======
	unsigned long gmap_addr;	/* address of last gmap fault. */
	struct per_regs per_user;	/* User specified PER registers */
	struct per_event per_event;	/* Cause of the last PER trap */
	unsigned long per_flags;	/* Flags to control debug behavior */
        /* pfault_wait is used to block the process on a pfault event */
	unsigned long pfault_wait;
	struct list_head list;
	/* cpu runtime instrumentation */
	struct runtime_instr_cb *ri_cb;
	int ri_signum;
#ifdef CONFIG_64BIT
	unsigned char trap_tdb[256];	/* Transaction abort diagnose block */
#endif
};

/* Flag to disable transactions. */
#define PER_FLAG_NO_TE			1UL
/* Flag to enable random transaction aborts. */
#define PER_FLAG_TE_ABORT_RAND		2UL
/* Flag to specify random transaction abort mode:
 * - abort each transaction at a random instruction before TEND if set.
 * - abort random transactions at a random instruction if cleared.
 */
#define PER_FLAG_TE_ABORT_RAND_TEND	4UL

>>>>>>> refs/remotes/origin/master
typedef struct thread_struct thread_struct;

/*
 * Stack layout of a C stack frame.
 */
#ifndef __PACK_STACK
struct stack_frame {
	unsigned long back_chain;
	unsigned long empty1[5];
	unsigned long gprs[10];
	unsigned int  empty2[8];
};
#else
struct stack_frame {
	unsigned long empty1[5];
	unsigned int  empty2[8];
	unsigned long gprs[10];
	unsigned long back_chain;
};
#endif

#define ARCH_MIN_TASKALIGN	8

#define INIT_THREAD {							\
	.ksp = sizeof(init_stack) + (unsigned long) &init_stack,	\
}

/*
 * Do necessary setup to start up a new thread.
 */
<<<<<<< HEAD
<<<<<<< HEAD
#define start_thread(regs, new_psw, new_stackp) do {		\
	set_fs(USER_DS);					\
	regs->psw.mask	= psw_user_bits;			\
	regs->psw.addr	= new_psw | PSW_ADDR_AMODE;		\
	regs->gprs[15]	= new_stackp;				\
} while (0)

#define start_thread31(regs, new_psw, new_stackp) do {		\
	set_fs(USER_DS);					\
	regs->psw.mask	= psw_user32_bits;			\
	regs->psw.addr	= new_psw | PSW_ADDR_AMODE;		\
	regs->gprs[15]	= new_stackp;				\
	crst_table_downgrade(current->mm, 1UL << 31);		\
=======
#define start_thread(regs, new_psw, new_stackp) do {			\
	regs->psw.mask	= psw_user_bits | PSW_MASK_EA | PSW_MASK_BA;	\
	regs->psw.addr	= new_psw | PSW_ADDR_AMODE;			\
	regs->gprs[15]	= new_stackp;					\
} while (0)

#define start_thread31(regs, new_psw, new_stackp) do {			\
	regs->psw.mask	= psw_user_bits | PSW_MASK_BA;			\
	regs->psw.addr	= new_psw | PSW_ADDR_AMODE;			\
	regs->gprs[15]	= new_stackp;					\
	__tlb_flush_mm(current->mm);					\
	crst_table_downgrade(current->mm, 1UL << 31);			\
	update_mm(current->mm, current);				\
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define start_thread(regs, new_psw, new_stackp) do {			\
	regs->psw.mask	= PSW_USER_BITS | PSW_MASK_EA | PSW_MASK_BA;	\
	regs->psw.addr	= new_psw | PSW_ADDR_AMODE;			\
	regs->gprs[15]	= new_stackp;					\
	execve_tail();							\
} while (0)

#define start_thread31(regs, new_psw, new_stackp) do {			\
	regs->psw.mask	= PSW_USER_BITS | PSW_MASK_BA;			\
	regs->psw.addr	= new_psw | PSW_ADDR_AMODE;			\
	regs->gprs[15]	= new_stackp;					\
	crst_table_downgrade(current->mm, 1UL << 31);			\
	execve_tail();							\
>>>>>>> refs/remotes/origin/master
} while (0)

/* Forward declaration, a strange C thing */
struct task_struct;
struct mm_struct;
struct seq_file;

<<<<<<< HEAD
/* Free all resources held by a thread. */
extern void release_thread(struct task_struct *);
extern int kernel_thread(int (*fn)(void *), void * arg, unsigned long flags);

/* Prepare to copy thread state - unlazy all lazy status */
#define prepare_to_copy(tsk)	do { } while (0)
=======
#ifdef CONFIG_64BIT
extern void show_cacheinfo(struct seq_file *m);
#else
static inline void show_cacheinfo(struct seq_file *m) { }
#endif

/* Free all resources held by a thread. */
extern void release_thread(struct task_struct *);
>>>>>>> refs/remotes/origin/master

/*
 * Return saved PC of a blocked thread.
 */
extern unsigned long thread_saved_pc(struct task_struct *t);

<<<<<<< HEAD
extern void show_code(struct pt_regs *regs);

=======
>>>>>>> refs/remotes/origin/master
unsigned long get_wchan(struct task_struct *p);
#define task_pt_regs(tsk) ((struct pt_regs *) \
        (task_stack_page(tsk) + THREAD_SIZE) - 1)
#define KSTK_EIP(tsk)	(task_pt_regs(tsk)->psw.addr)
#define KSTK_ESP(tsk)	(task_pt_regs(tsk)->gprs[15])

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
/* Has task runtime instrumentation enabled ? */
#define is_ri_task(tsk) (!!(tsk)->thread.ri_cb)

>>>>>>> refs/remotes/origin/master
static inline unsigned short stap(void)
{
	unsigned short cpu_address;

	asm volatile("stap %0" : "=m" (cpu_address));
	return cpu_address;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * Give up the time slice of the virtual PU.
 */
static inline void cpu_relax(void)
{
	if (MACHINE_HAS_DIAG44)
		asm volatile("diag 0,0,68");
	barrier();
}

<<<<<<< HEAD
=======
#define arch_mutex_cpu_relax()  barrier()

>>>>>>> refs/remotes/origin/master
static inline void psw_set_key(unsigned int key)
{
	asm volatile("spka 0(%0)" : : "d" (key));
}

/*
 * Set PSW to specified value.
 */
static inline void __load_psw(psw_t psw)
{
<<<<<<< HEAD
#ifndef __s390x__
=======
#ifndef CONFIG_64BIT
>>>>>>> refs/remotes/origin/master
	asm volatile("lpsw  %0" : : "Q" (psw) : "cc");
#else
	asm volatile("lpswe %0" : : "Q" (psw) : "cc");
#endif
}

/*
 * Set PSW mask to specified value, while leaving the
 * PSW addr pointing to the next instruction.
 */
<<<<<<< HEAD
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static inline void __load_psw_mask (unsigned long mask)
{
	unsigned long addr;
	psw_t psw;

	psw.mask = mask;

<<<<<<< HEAD
#ifndef __s390x__
=======
#ifndef CONFIG_64BIT
>>>>>>> refs/remotes/origin/master
	asm volatile(
		"	basr	%0,0\n"
		"0:	ahi	%0,1f-0b\n"
		"	st	%0,%O1+4(%R1)\n"
		"	lpsw	%1\n"
		"1:"
		: "=&d" (addr), "=Q" (psw) : "Q" (psw) : "memory", "cc");
<<<<<<< HEAD
#else /* __s390x__ */
=======
#else /* CONFIG_64BIT */
>>>>>>> refs/remotes/origin/master
	asm volatile(
		"	larl	%0,1f\n"
		"	stg	%0,%O1+8(%R1)\n"
		"	lpswe	%1\n"
		"1:"
		: "=&d" (addr), "=Q" (psw) : "Q" (psw) : "memory", "cc");
<<<<<<< HEAD
#endif /* __s390x__ */
}
<<<<<<< HEAD
 
/*
 * Function to stop a processor until an interruption occurred
 */
static inline void enabled_wait(void)
{
	__load_psw_mask(PSW_BASE_BITS | PSW_MASK_IO | PSW_MASK_EXT |
			PSW_MASK_MCHECK | PSW_MASK_WAIT | PSW_DEFAULT_KEY);
}

/*
 * Function to drop a processor into disabled wait state
 */

static inline void ATTRIB_NORET disabled_wait(unsigned long code)
=======
=======
#endif /* CONFIG_64BIT */
}
>>>>>>> refs/remotes/origin/master

/*
 * Rewind PSW instruction address by specified number of bytes.
 */
static inline unsigned long __rewind_psw(psw_t psw, unsigned long ilc)
{
<<<<<<< HEAD
#ifndef __s390x__
=======
#ifndef CONFIG_64BIT
>>>>>>> refs/remotes/origin/master
	if (psw.addr & PSW_ADDR_AMODE)
		/* 31 bit mode */
		return (psw.addr - ilc) | PSW_ADDR_AMODE;
	/* 24 bit mode */
	return (psw.addr - ilc) & ((1UL << 24) - 1);
#else
	unsigned long mask;

	mask = (psw.mask & PSW_MASK_EA) ? -1UL :
	       (psw.mask & PSW_MASK_BA) ? (1UL << 31) - 1 :
					  (1UL << 24) - 1;
	return (psw.addr - ilc) & mask;
#endif
}
 
/*
 * Function to drop a processor into disabled wait state
 */
static inline void __noreturn disabled_wait(unsigned long code)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
        unsigned long ctl_buf;
        psw_t dw_psw;

<<<<<<< HEAD
<<<<<<< HEAD
        dw_psw.mask = PSW_BASE_BITS | PSW_MASK_WAIT;
=======
	dw_psw.mask = PSW_MASK_BASE | PSW_MASK_WAIT | PSW_MASK_BA | PSW_MASK_EA;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	dw_psw.mask = PSW_MASK_BASE | PSW_MASK_WAIT | PSW_MASK_BA | PSW_MASK_EA;
>>>>>>> refs/remotes/origin/master
        dw_psw.addr = code;
        /* 
         * Store status and then load disabled wait psw,
         * the processor is dead afterwards
         */
<<<<<<< HEAD
#ifndef __s390x__
=======
#ifndef CONFIG_64BIT
>>>>>>> refs/remotes/origin/master
	asm volatile(
		"	stctl	0,0,0(%2)\n"
		"	ni	0(%2),0xef\n"	/* switch off protection */
		"	lctl	0,0,0(%2)\n"
		"	stpt	0xd8\n"		/* store timer */
		"	stckc	0xe0\n"		/* store clock comparator */
		"	stpx	0x108\n"	/* store prefix register */
		"	stam	0,15,0x120\n"	/* store access registers */
		"	std	0,0x160\n"	/* store f0 */
		"	std	2,0x168\n"	/* store f2 */
		"	std	4,0x170\n"	/* store f4 */
		"	std	6,0x178\n"	/* store f6 */
		"	stm	0,15,0x180\n"	/* store general registers */
		"	stctl	0,15,0x1c0\n"	/* store control registers */
		"	oi	0x1c0,0x10\n"	/* fake protection bit */
		"	lpsw	0(%1)"
		: "=m" (ctl_buf)
		: "a" (&dw_psw), "a" (&ctl_buf), "m" (dw_psw) : "cc");
<<<<<<< HEAD
#else /* __s390x__ */
=======
#else /* CONFIG_64BIT */
>>>>>>> refs/remotes/origin/master
	asm volatile(
		"	stctg	0,0,0(%2)\n"
		"	ni	4(%2),0xef\n"	/* switch off protection */
		"	lctlg	0,0,0(%2)\n"
		"	lghi	1,0x1000\n"
		"	stpt	0x328(1)\n"	/* store timer */
		"	stckc	0x330(1)\n"	/* store clock comparator */
		"	stpx	0x318(1)\n"	/* store prefix register */
		"	stam	0,15,0x340(1)\n"/* store access registers */
		"	stfpc	0x31c(1)\n"	/* store fpu control */
		"	std	0,0x200(1)\n"	/* store f0 */
		"	std	1,0x208(1)\n"	/* store f1 */
		"	std	2,0x210(1)\n"	/* store f2 */
		"	std	3,0x218(1)\n"	/* store f3 */
		"	std	4,0x220(1)\n"	/* store f4 */
		"	std	5,0x228(1)\n"	/* store f5 */
		"	std	6,0x230(1)\n"	/* store f6 */
		"	std	7,0x238(1)\n"	/* store f7 */
		"	std	8,0x240(1)\n"	/* store f8 */
		"	std	9,0x248(1)\n"	/* store f9 */
		"	std	10,0x250(1)\n"	/* store f10 */
		"	std	11,0x258(1)\n"	/* store f11 */
		"	std	12,0x260(1)\n"	/* store f12 */
		"	std	13,0x268(1)\n"	/* store f13 */
		"	std	14,0x270(1)\n"	/* store f14 */
		"	std	15,0x278(1)\n"	/* store f15 */
		"	stmg	0,15,0x280(1)\n"/* store general registers */
		"	stctg	0,15,0x380(1)\n"/* store control registers */
		"	oi	0x384(1),0x10\n"/* fake protection bit */
		"	lpswe	0(%1)"
		: "=m" (ctl_buf)
		: "a" (&dw_psw), "a" (&ctl_buf), "m" (dw_psw) : "cc", "0", "1");
<<<<<<< HEAD
#endif /* __s390x__ */
=======
#endif /* CONFIG_64BIT */
>>>>>>> refs/remotes/origin/master
	while (1);
}

/*
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
 * Use to set psw mask except for the first byte which
 * won't be changed by this function.
 */
static inline void
__set_psw_mask(unsigned long mask)
{
	__load_psw_mask(mask | (arch_local_save_flags() & ~(-1UL >> 8)));
}

#define local_mcck_enable() \
<<<<<<< HEAD
	__set_psw_mask(psw_kernel_bits | PSW_MASK_DAT | PSW_MASK_MCHECK)
#define local_mcck_disable() \
	__set_psw_mask(psw_kernel_bits | PSW_MASK_DAT)

/*
>>>>>>> refs/remotes/origin/cm-10.0
=======
	__set_psw_mask(PSW_KERNEL_BITS | PSW_MASK_DAT | PSW_MASK_MCHECK)
#define local_mcck_disable() \
	__set_psw_mask(PSW_KERNEL_BITS | PSW_MASK_DAT)

/*
>>>>>>> refs/remotes/origin/master
 * Basic Machine Check/Program Check Handler.
 */

extern void s390_base_mcck_handler(void);
extern void s390_base_pgm_handler(void);
extern void s390_base_ext_handler(void);

extern void (*s390_base_mcck_handler_fn)(void);
extern void (*s390_base_pgm_handler_fn)(void);
extern void (*s390_base_ext_handler_fn)(void);

#define ARCH_LOW_ADDRESS_LIMIT	0x7fffffffUL

<<<<<<< HEAD
#endif
=======
extern int memcpy_real(void *, void *, size_t);
extern void memcpy_absolute(void *, void *, size_t);

#define mem_assign_absolute(dest, val) {			\
	__typeof__(dest) __tmp = (val);				\
								\
	BUILD_BUG_ON(sizeof(__tmp) != sizeof(val));		\
	memcpy_absolute(&(dest), &__tmp, sizeof(__tmp));	\
}
>>>>>>> refs/remotes/origin/master

/*
 * Helper macro for exception table entries
 */
<<<<<<< HEAD
#ifndef __s390x__
#define EX_TABLE(_fault,_target)			\
	".section __ex_table,\"a\"\n"			\
	"	.align 4\n"				\
	"	.long  " #_fault "," #_target "\n"	\
	".previous\n"
#else
#define EX_TABLE(_fault,_target)			\
	".section __ex_table,\"a\"\n"			\
	"	.align 8\n"				\
	"	.quad  " #_fault "," #_target "\n"	\
	".previous\n"
#endif

#endif                                 /* __ASM_S390_PROCESSOR_H           */
=======
#define EX_TABLE(_fault, _target)	\
	".section __ex_table,\"a\"\n"	\
	".align	4\n"			\
	".long	(" #_fault ") - .\n"	\
	".long	(" #_target ") - .\n"	\
	".previous\n"

#else /* __ASSEMBLY__ */

#define EX_TABLE(_fault, _target)	\
	.section __ex_table,"a"	;	\
	.align	4 ;			\
	.long	(_fault) - . ;		\
	.long	(_target) - . ;		\
	.previous

#endif /* __ASSEMBLY__ */

#endif /* __ASM_S390_PROCESSOR_H */
>>>>>>> refs/remotes/origin/master
