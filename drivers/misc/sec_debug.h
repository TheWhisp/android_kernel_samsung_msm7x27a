#ifndef _SEC_DEBUG_H_
#define _SEC_DEBUG_H_

#include <linux/io.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/sched.h>

typedef struct tag_mmu_info {
	int SCTLR;
	int TTBR0;
	int TTBR1;
	int TTBCR;
	int DACR;
	int DFSR;
	int DFAR;
	int IFSR;
	int IFAR;
	int DAFSR;
	int IAFSR;
	int PMRRR;
	int NMRRR;
	int FCSEPID;
	int CONTEXT;
	int URWTPID;
	int UROTPID;
	int POTPIDR;
} t_sec_mmu_info;

/*ARM CORE regs mapping structure*/
typedef struct {
	/* COMMON */
	unsigned int r0;
	unsigned int r1;
	unsigned int r2;
	unsigned int r3;
	unsigned int r4;
	unsigned int r5;
	unsigned int r6;
	unsigned int r7;
	unsigned int r8;
	unsigned int r9;
	unsigned int r10;
	unsigned int r11;
	unsigned int r12;

	/* SVC */
	unsigned int r13_svc;
	unsigned int r14_svc;
	unsigned int spsr_svc;

	/* PC & CPSR */
	unsigned int pc;
	unsigned int cpsr;

	/* USR/SYS */
	unsigned int r13_usr;
	unsigned int r14_usr;

	/* FIQ */
	unsigned int r8_fiq;
	unsigned int r9_fiq;
	unsigned int r10_fiq;
	unsigned int r11_fiq;
	unsigned int r12_fiq;
	unsigned int r13_fiq;
	unsigned int r14_fiq;
	unsigned int spsr_fiq;

	/* IRQ */
	unsigned int r13_irq;
	unsigned int r14_irq;
	unsigned int spsr_irq;

	/* MON */
	unsigned int r13_mon;
	unsigned int r14_mon;
	unsigned int spsr_mon;

	/* ABT */
	unsigned int r13_abt;
	unsigned int r14_abt;
	unsigned int spsr_abt;

	/* UNDEF */
	unsigned int r13_und;
	unsigned int r14_und;
	unsigned int spsr_und;

} t_sec_arm_core_regsiters;

#ifdef CONFIG_SEC_DEBUG_SCHED_LOG
#define SCHED_LOG_MAX 1024

struct irq_log {
	unsigned long long time;
	int irq;
	void *fn;
	int en;
	unsigned long long elapsed_time;
};

struct softirq_log {
	unsigned long long time;
	void *fn;
	unsigned long long elapsed_time;
};

struct enterexit_log {
	unsigned int irq;
	unsigned long long time;
	unsigned long long end_time;
	unsigned long long elapsed_time;
};

struct task_info {
	unsigned long long time;
	char comm[TASK_COMM_LEN];
	pid_t pid;
};

union task_log {
	struct task_info task;
	struct irq_log irq;
};

struct sched_log {
	unsigned long long time;
	union task_log log;
};

struct timer_log {
	unsigned long long time;
	unsigned int type;
	int int_lock;
	void *fn;
};

struct sec_debug_nocache {
	struct task_info gExcpTaskLog[SCHED_LOG_MAX] ;
	struct irq_log gExcpIrqLog[SCHED_LOG_MAX] ;
	struct enterexit_log gExcpIrqEnterExitLog[SCHED_LOG_MAX] ;
	struct timer_log gExcpTimerLog[SCHED_LOG_MAX] ;
	atomic_t gExcpTaskLogIdx;
	atomic_t gExcpIrqLogIdx;
	atomic_t gExcpIrqEnterExitLogIdx;
	atomic_t gExcpTimerLogIdx;
};

extern void sec_debug_task_sched_log(int cpu, struct task_struct *task);
extern void sec_debug_irq_sched_log(unsigned int irq, void *fn, int en);
extern void sec_debug_irq_sched_log_end(void);
extern void sec_debug_timer_log(unsigned int type, int int_lock, void *fn);
#ifdef CONFIG_SEC_DEBUG_IRQ_EXIT_LOG
extern void sec_debug_irq_enterexit_log(unsigned int irq,\
					unsigned long long start_time);
#endif /* CONFIG_SEC_DEBUG_IRQ_EXIT_LOG */
#endif /* CONFIG_SEC_DEBUG_SCHED_LOG */

extern void sec_get_core_reg_dump(t_sec_arm_core_regsiters *regs);
extern int  sec_get_mmu_reg_dump(t_sec_mmu_info *mmu_info);
extern void sec_save_final_context(void);
extern int sec_debug_init(void);
extern void sec_check_crash_key(int keycode, u8 keypress);
#endif /* _SEC_DEBUG_H_ */
