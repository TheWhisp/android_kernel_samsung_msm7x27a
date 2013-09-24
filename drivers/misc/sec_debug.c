/* sec_debug.c
 *
 * Exception handling in kernel by SEC
 *
 * Copyright (c) 2011 Samsung Electronics
 *                http://www.samsung.com/
 */

#ifdef CONFIG_SEC_DEBUG
#include <linux/errno.h>
#include <linux/ctype.h>
#include <linux/vmalloc.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/input.h>

#include <linux/file.h>
#include <mach/hardware.h>

#include <mach/msm_iomap.h>
#include "sec_debug.h"
#include "../arch/arm/mach-msm/smd_private.h"

/********************************
 *  Variable
 *********************************/
static int save_final_context;
__used t_sec_arm_core_regsiters sec_core_reg_dump;
__used t_sec_mmu_info           sec_mmu_reg_dump;
void *sec_dbg_buf_ptr;
unsigned int sec_dbg_buf_base;
unsigned int sec_dbg_buf_size;

#ifdef CONFIG_SEC_DEBUG_SCHED_LOG
struct sec_debug_nocache *sec_debug_nocache_log;
static struct task_info gExcpTaskLog[SCHED_LOG_MAX] __cacheline_aligned;
static struct irq_log gExcpIrqLog[SCHED_LOG_MAX] __cacheline_aligned;
static struct enterexit_log gExcpIrqEnterExitLog[SCHED_LOG_MAX]\
		__cacheline_aligned;
static struct timer_log gExcpTimerLog[SCHED_LOG_MAX] __cacheline_aligned;
atomic_t gExcpTaskLogIdx = {ATOMIC_INIT(-1)};
atomic_t gExcpIrqLogIdx = {ATOMIC_INIT(-1)};
atomic_t gExcpIrqEnterExitLogIdx = {ATOMIC_INIT(-1)};
atomic_t gExcpTimerLogIdx = {ATOMIC_INIT(-1)};
#endif /* CONFIG_SEC_DEBUG_SCHED_LOG */

/********************************
 *  Define
 *********************************/
#define LOCKUP_FIRST_KEY KEY_VOLUMEUP
#define LOCKUP_SECOND_KEY KEY_VOLUMEDOWN
#define LOCKUP_THIRD_KEY KEY_HOME

/********************************
 *  Function
 *********************************/
/* core reg dump function*/
void sec_get_core_reg_dump(t_sec_arm_core_regsiters *regs)
{
	asm(
		/* we will be in SVC mode when we enter this function.
		* Collect SVC registers along with cmn registers.
		*/
		"str r0, [%0,#0]\n\t"
		"str r1, [%0,#4]\n\t"
		"str r2, [%0,#8]\n\t"
		"str r3, [%0,#12]\n\t"
		"str r4, [%0,#16]\n\t"
		"str r5, [%0,#20]\n\t"
		"str r6, [%0,#24]\n\t"
		"str r7, [%0,#28]\n\t"
		"str r8, [%0,#32]\n\t"
		"str r9, [%0,#36]\n\t"
		"str r10, [%0,#40]\n\t"
		"str r11, [%0,#44]\n\t"
		"str r12, [%0,#48]\n\t"

		/* SVC */
		"str r13, [%0,#52]\n\t"
		"str r14, [%0,#56]\n\t"
		"mrs r1, spsr\n\t"
		"str r1, [%0,#60]\n\t"

		/* PC and CPSR */
		"sub r1, r15, #0x4\n\t"
		"str r1, [%0,#64]\n\t"
		"mrs r1, cpsr\n\t"
		"str r1, [%0,#68]\n\t"

		/* SYS/USR */
		"mrs r1, cpsr\n\t"
		"and r1, r1, #0xFFFFFFE0\n\t"
		"orr r1, r1, #0x1f\n\t"
		"msr cpsr,r1\n\t"

		"str r13, [%0,#72]\n\t"
		"str r14, [%0,#76]\n\t"

		/*FIQ*/
		"mrs r1, cpsr\n\t"
		"and r1,r1,#0xFFFFFFE0\n\t"
		"orr r1,r1,#0x11\n\t"
		"msr cpsr,r1\n\t"

		"str r8, [%0,#80]\n\t"
		"str r9, [%0,#84]\n\t"
		"str r10, [%0,#88]\n\t"
		"str r11, [%0,#92]\n\t"
		"str r12, [%0,#96]\n\t"
		"str r13, [%0,#100]\n\t"
		"str r14, [%0,#104]\n\t"
		"mrs r1, spsr\n\t"
		"str r1, [%0,#108]\n\t"

		/*IRQ*/
		"mrs r1, cpsr\n\t"
		"and r1, r1, #0xFFFFFFE0\n\t"
		"orr r1, r1, #0x12\n\t"
		"msr cpsr,r1\n\t"

		"str r13, [%0,#112]\n\t"
		"str r14, [%0,#116]\n\t"
		"mrs r1, spsr\n\t"
		"str r1, [%0,#120]\n\t"

		/*MON*/
		"mrs r1, cpsr\n\t"
		"and r1, r1, #0xFFFFFFE0\n\t"
		"orr r1, r1, #0x16\n\t"
		"msr cpsr,r1\n\t"

		"str r13, [%0,#124]\n\t"
		"str r14, [%0,#128]\n\t"
		"mrs r1, spsr\n\t"
		"str r1, [%0,#132]\n\t"

		/*ABT*/
		"mrs r1, cpsr\n\t"
		"and r1, r1, #0xFFFFFFE0\n\t"
		"orr r1, r1, #0x17\n\t"
		"msr cpsr,r1\n\t"

		"str r13, [%0,#136]\n\t"
		"str r14, [%0,#140]\n\t"
		"mrs r1, spsr\n\t"
		"str r1, [%0,#144]\n\t"

		/*UND*/
		"mrs r1, cpsr\n\t"
		"and r1, r1, #0xFFFFFFE0\n\t"
		"orr r1, r1, #0x1B\n\t"
		"msr cpsr,r1\n\t"

		"str r13, [%0,#148]\n\t"
		"str r14, [%0,#152]\n\t"
		"mrs r1, spsr\n\t"
		"str r1, [%0,#156]\n\t"

		/* restore to SVC mode */
		"mrs r1, cpsr\n\t"
		"and r1, r1, #0xFFFFFFE0\n\t"
		"orr r1, r1, #0x13\n\t"
		"msr cpsr,r1\n\t"

	:			/* output */
	: "r"(regs)		/* input */
	: "%r1"		/* clobbered register */
	);

	return;
}
EXPORT_SYMBOL(sec_get_core_reg_dump);

int sec_get_mmu_reg_dump(t_sec_mmu_info *mmu_info)
{
	asm("mrc    p15, 0, r1, c1, c0, 0\n\t"	/* SCTLR */
		"str r1, [%0]\n\t"
		"mrc    p15, 0, r1, c2, c0, 0\n\t"	/* TTBR0 */
		"str r1, [%0,#4]\n\t"
		"mrc    p15, 0, r1, c2, c0,1\n\t"	/* TTBR1 */
		"str r1, [%0,#8]\n\t"
		"mrc    p15, 0, r1, c2, c0,2\n\t"	/* TTBCR */
		"str r1, [%0,#12]\n\t"
		"mrc    p15, 0, r1, c3, c0,0\n\t"	/* DACR */
		"str r1, [%0,#16]\n\t"
		"mrc    p15, 0, r1, c5, c0,0\n\t"	/* DFSR */
		"str r1, [%0,#20]\n\t"
		"mrc    p15, 0, r1, c6, c0,0\n\t"	/* DFAR */
		"str r1, [%0,#24]\n\t"
		"mrc    p15, 0, r1, c5, c0,1\n\t"	/* IFSR */
		"str r1, [%0,#28]\n\t"
		"mrc    p15, 0, r1, c6, c0,2\n\t"	/* IFAR */
		"str r1, [%0,#32]\n\t"
		/*Dont populate DAFSR and RAFSR */
		"mrc    p15, 0, r1, c10, c2,0\n\t"	/* PMRRR */
		"str r1, [%0,#44]\n\t"
		"mrc    p15, 0, r1, c10, c2,1\n\t"	/* NMRRR */
		"str r1, [%0,#48]\n\t"
		"mrc    p15, 0, r1, c13, c0,0\n\t"	/* FCSEPID */
		"str r1, [%0,#52]\n\t"
		"mrc    p15, 0, r1, c13, c0,1\n\t"	/* CONTEXT */
		"str r1, [%0,#56]\n\t"
		"mrc    p15, 0, r1, c13, c0,2\n\t"	/* URWTPID */
		"str r1, [%0,#60]\n\t"
		"mrc    p15, 0, r1, c13, c0,3\n\t"	/* UROTPID */
		"str r1, [%0,#64]\n\t"
		"mrc    p15, 0, r1, c13, c0,4\n\t"	/* POTPIDR */
		"str r1, [%0,#68]\n\t"
		:			/* output */
	: "r"(mmu_info)		/* input */
	: "%r1", "memory"	/* clobbered register */
	);
	return 0;
}
EXPORT_SYMBOL(sec_get_mmu_reg_dump);

void sec_save_final_context(void)
{
	samsung_vendor1_id *smem_vendor1 = NULL;
	unsigned size;

	if (save_final_context) {
		printk(KERN_EMERG"(sec_save_final_context) already saved.\n");
		return;
	}

	sec_get_mmu_reg_dump(&sec_mmu_reg_dump);
	printk(KERN_EMERG"(sec_save_final_context) sec_get_mmu_reg_dump.\n");

	sec_get_core_reg_dump(&sec_core_reg_dump);
	printk(KERN_EMERG "(sec_save_final_context) sec_get_core_reg_dump.\n");

	smem_vendor1 = (samsung_vendor1_id *)smem_get_entry(SMEM_ID_VENDOR1,\
				&size);
	memcpy(&(smem_vendor1->apps_dump.apps_regs),
		&sec_mmu_reg_dump, sizeof(sec_mmu_reg_dump));

	save_final_context = 1;
}
EXPORT_SYMBOL(sec_save_final_context);

__init int sec_debug_init(void)
{
	pr_emerg("(%s)\n", __func__);

	sec_dbg_buf_ptr = ioremap_nocache(sec_dbg_buf_base, sec_dbg_buf_size);
	/* check sec_dbg_buf_ptr here */
	/* check magic code here */

	pr_emerg("(%s) sec_dbg_buf_ptr = 0x%x\n", __func__,\
		(unsigned int)sec_dbg_buf_ptr);

#ifdef CONFIG_SEC_DEBUG_SCHED_LOG
	sec_debug_nocache_log = (struct sec_debug_nocache *)sec_dbg_buf_ptr;

	if (sec_debug_nocache_log) {
		/* init sec_debug_nocache_log here */
		atomic_set(&(sec_debug_nocache_log->gExcpTaskLogIdx), -1);
		atomic_set(&(sec_debug_nocache_log->gExcpIrqLogIdx), -1);
		atomic_set(&(sec_debug_nocache_log->\
						gExcpIrqEnterExitLogIdx), -1);
		atomic_set(&(sec_debug_nocache_log->gExcpTimerLogIdx), -1);
	}
#endif /* CONFIG_SEC_DEBUG_SCHED_LOG */

	return 0;
}

static int __init sec_dbg_setup(char *str)
{
	unsigned size = memparse(str, &str);

	pr_emerg("(%s)\n", __func__);

	if (size && (size == roundup_pow_of_two(size)) && (*str == '@')) {
		unsigned long long base = 0;

		kstrtoull(++str, 0, &base);

		sec_dbg_buf_size = size;
		sec_dbg_buf_base = base;
	}

	pr_emerg("(%s) sec_dbg_buf_base = 0x%x\n", __func__, sec_dbg_buf_base);
	pr_emerg("(%s) sec_dbg_buf_size = 0x%x\n", __func__, sec_dbg_buf_size);
	return 1;
}
__setup("sec_dbg=", sec_dbg_setup);

#ifdef CONFIG_SEC_DEBUG_SCHED_LOG
void sec_debug_task_sched_log(int cpu, struct task_struct *task)
{
	unsigned i;

	if (sec_debug_nocache_log) {
		i = atomic_inc_return(&(sec_debug_nocache_log->\
				gExcpTaskLogIdx)) & (SCHED_LOG_MAX - 1);
		sec_debug_nocache_log->gExcpTaskLog[i].time = cpu_clock(cpu);
		strncpy(sec_debug_nocache_log->gExcpTaskLog[i].comm,\
			task->comm, TASK_COMM_LEN);
		sec_debug_nocache_log->gExcpTaskLog[i].pid = task->pid;
	} else {
		i = atomic_inc_return(&gExcpTaskLogIdx) & (SCHED_LOG_MAX - 1);
		gExcpTaskLog[i].time = cpu_clock(cpu);
		strncpy(gExcpTaskLog[i].comm, task->comm, TASK_COMM_LEN);
		gExcpTaskLog[i].pid = task->pid;
	}
}

void sec_debug_timer_log(unsigned int type, int int_lock, void *fn)
{
	int cpu = smp_processor_id();
	unsigned i;

	if (sec_debug_nocache_log) {
		i = atomic_inc_return(&(sec_debug_nocache_log->\
				gExcpTimerLogIdx)) & (SCHED_LOG_MAX - 1);
		sec_debug_nocache_log->gExcpTimerLog[i].time = cpu_clock(cpu);
		sec_debug_nocache_log->gExcpTimerLog[i].type = type;
		sec_debug_nocache_log->gExcpTimerLog[i].int_lock = int_lock;
		sec_debug_nocache_log->gExcpTimerLog[i].fn = (void *)fn;
	} else {
		i = atomic_inc_return(&gExcpTimerLogIdx) & (SCHED_LOG_MAX - 1);
		gExcpTimerLog[i].time = cpu_clock(cpu);
		gExcpTimerLog[i].type = type;
		gExcpTimerLog[i].int_lock = int_lock;
		gExcpTimerLog[i].fn = (void *)fn;
	}
}

void sec_debug_irq_sched_log(unsigned int irq, void *fn, int en)
{
	int cpu = smp_processor_id();
	unsigned i;

	if (sec_debug_nocache_log) {
		i = atomic_inc_return(&(sec_debug_nocache_log->gExcpIrqLogIdx))\
			& (SCHED_LOG_MAX - 1);
		sec_debug_nocache_log->gExcpIrqLog[i].time = cpu_clock(cpu);
		sec_debug_nocache_log->gExcpIrqLog[i].irq = irq;
		sec_debug_nocache_log->gExcpIrqLog[i].fn = (void *)fn;
		sec_debug_nocache_log->gExcpIrqLog[i].en = en;
	} else {
		i = atomic_inc_return(&gExcpIrqLogIdx) & (SCHED_LOG_MAX - 1);
		gExcpIrqLog[i].time = cpu_clock(cpu);
		gExcpIrqLog[i].irq = irq;
		gExcpIrqLog[i].fn = (void *)fn;
		gExcpIrqLog[i].en = en;
	}
}

void sec_debug_irq_sched_log_end(void)
{
	int cpu = smp_processor_id();
	unsigned i;

	if (sec_debug_nocache_log) {
		i = atomic_inc_return(&(sec_debug_nocache_log->gExcpIrqLogIdx))\
			& (SCHED_LOG_MAX - 1);
		sec_debug_nocache_log->gExcpIrqLog[i].elapsed_time =\
				cpu_clock(cpu)\
				- sec_debug_nocache_log->gExcpIrqLog[i].time;
	} else {
		i = atomic_read(&gExcpIrqLogIdx) & (SCHED_LOG_MAX - 1);
		gExcpIrqLog[i].elapsed_time =\
				cpu_clock(cpu) - gExcpIrqLog[i].time;
	}
}

#ifdef CONFIG_SEC_DEBUG_IRQ_EXIT_LOG
void sec_debug_irq_enterexit_log(unsigned int irq,\
						unsigned long long start_time)
{
	int cpu = smp_processor_id();
	unsigned i;

	if (sec_debug_nocache_log) {
		i = atomic_inc_return(&(sec_debug_nocache_log->\
						gExcpIrqEnterExitLogIdx))\
						& (SCHED_LOG_MAX - 1);
		sec_debug_nocache_log->gExcpIrqEnterExitLog[i].time =\
			start_time;
		sec_debug_nocache_log->gExcpIrqEnterExitLog[i].end_time =\
			cpu_clock(cpu);
		sec_debug_nocache_log->gExcpIrqEnterExitLog[i].irq = irq;
		sec_debug_nocache_log->gExcpIrqEnterExitLog[i].elapsed_time =\
			sec_debug_nocache_log->gExcpIrqEnterExitLog[i].end_time\
				 - start_time;
	} else {
		i = atomic_inc_return(&gExcpIrqEnterExitLogIdx)\
			& (SCHED_LOG_MAX - 1);
		gExcpIrqEnterExitLog[i].time = start_time;
		gExcpIrqEnterExitLog[i].end_time = cpu_clock(cpu);
		gExcpIrqEnterExitLog[i].irq = irq;
		gExcpIrqEnterExitLog[i].elapsed_time =\
			gExcpIrqEnterExitLog[i].end_time - start_time;
	}
}
#endif
#endif /* CONFIG_SEC_DEBUG_SCHED_LOG */


void sec_check_crash_key(int keycode, u8 keypress)
{
#if defined(CONFIG_MACH_TREBON)
	static enum {NONE, STEP1, STEP2, STEP3, STEP4, STEP5, STEP6,\
			STEP7, STEP8} state = NONE;

	switch (state) {
	case NONE:
		if ((keycode == LOCKUP_FIRST_KEY) && keypress)
			state = STEP1;
		else
			state = NONE;
		break;
	case STEP1:
		if ((keycode == LOCKUP_FIRST_KEY) && !keypress)
			state = STEP2;
		else
			state = NONE;
		break;
	case STEP2:
		if ((keycode == LOCKUP_SECOND_KEY) && keypress)
			state = STEP3;
		else
			state = NONE;
		break;
	case STEP3:
		if ((keycode == LOCKUP_SECOND_KEY) && !keypress)
			state = STEP4;
		else
			state = NONE;
		break;
	case STEP4:
		if ((keycode == LOCKUP_THIRD_KEY) && keypress)
			state = STEP5;
		else
			state = NONE;
		break;
	case STEP5:
		if ((keycode == LOCKUP_THIRD_KEY) && !keypress)
			state = STEP6;
		else
			state = NONE;
		break;
	case STEP6:
		if ((keycode == LOCKUP_FIRST_KEY) && keypress)
			state = STEP7;
		else
			state = NONE;
		break;
	case STEP7:
		if ((keycode == LOCKUP_FIRST_KEY) && !keypress)
			state = STEP8;
		else
			state = NONE;
		break;
	case STEP8:
		if ((keycode == LOCKUP_THIRD_KEY) && keypress)
			panic("[Crash Key] LOCKUP CAPTURED!!!");
		else
			state = NONE;
		break;
	default:
		break;
	}
#elif defined(CONFIG_MACH_GEIM)
	static enum {NONE, STEP1, STEP2, STEP3, STEP4, STEP5, STEP6,\
			STEP7, STEP8, STEP9, STEP10, STEP11, STEP12, STEP13,\
			STEP14, STEP15, STEP16} state = NONE;

	switch (state) {
	case NONE:
		if ((keycode == LOCKUP_FIRST_KEY) && keypress)
			state = STEP1;
		else
			state = NONE;
		break;
	case STEP1:
		if ((keycode == LOCKUP_FIRST_KEY) && !keypress)
			state = STEP2;
		else
			state = NONE;
		break;
	case STEP2:
		if ((keycode == LOCKUP_SECOND_KEY) && keypress)
			state = STEP3;
		else
			state = NONE;
		break;
	case STEP3:
		if ((keycode == LOCKUP_SECOND_KEY) && !keypress)
			state = STEP4;
		else
			state = NONE;
		break;
	case STEP4:
		if ((keycode == LOCKUP_FIRST_KEY) && keypress)
			state = STEP5;
		else
			state = NONE;
		break;
	case STEP5:
		if ((keycode == LOCKUP_FIRST_KEY) && !keypress)
			state = STEP6;
		else
			state = NONE;
		break;
	case STEP6:
		if ((keycode == LOCKUP_SECOND_KEY) && keypress)
			state = STEP7;
		else
			state = NONE;
		break;
	case STEP7:
		if ((keycode == LOCKUP_SECOND_KEY) && !keypress)
			state = STEP8;
		else
			state = NONE;
		break;
	case STEP8:
		if ((keycode == LOCKUP_SECOND_KEY) && keypress)
			state = STEP9;
		else
			state = NONE;
		break;
	case STEP9:
		if ((keycode == LOCKUP_SECOND_KEY) && !keypress)
			state = STEP10;
		else
			state = NONE;
		break;
	case STEP10:
		if ((keycode == LOCKUP_FIRST_KEY) && keypress)
			state = STEP11;
		else
			state = NONE;
		break;
	case STEP11:
		if ((keycode == LOCKUP_FIRST_KEY) && !keypress)
			state = STEP12;
		else
			state = NONE;
		break;
	case STEP12:
		if ((keycode == LOCKUP_SECOND_KEY) && keypress)
			state = STEP13;
		else
			state = NONE;
		break;
	case STEP13:
		if ((keycode == LOCKUP_SECOND_KEY) && !keypress)
			state = STEP14;
		else
			state = NONE;
		break;
	case STEP14:
		if ((keycode == LOCKUP_SECOND_KEY) && keypress)
			state = STEP15;
		else
			state = NONE;
		break;
	case STEP15:
		if ((keycode == LOCKUP_SECOND_KEY) && !keypress)
			state = STEP16;
		else
			state = NONE;
		break;
	case STEP16:
		if ((keycode == LOCKUP_SECOND_KEY) && keypress)
			panic("[Crash Key] LOCKUP CAPTURED!!!");
		else
			state = NONE;
		break;
	default:
		break;
	}
#elif defined(CONFIG_MACH_JENA)
	static enum {NONE, STEP1, STEP2, STEP3, STEP4, STEP5, STEP6,\
			STEP7, STEP8} state = NONE;

	switch (state) {
	case NONE:
		if ((keycode == LOCKUP_FIRST_KEY) && keypress)
			state = STEP1;
		else
			state = NONE;
		break;
	case STEP1:
		if ((keycode == LOCKUP_FIRST_KEY) && !keypress)
			state = STEP2;
		else
			state = NONE;
		break;
	case STEP2:
		if ((keycode == LOCKUP_SECOND_KEY) && keypress)
			state = STEP3;
		else
			state = NONE;
		break;
	case STEP3:
		if ((keycode == LOCKUP_SECOND_KEY) && !keypress)
			state = STEP4;
		else
			state = NONE;
		break;
	case STEP4:
		if ((keycode == LOCKUP_THIRD_KEY) && keypress)
			state = STEP5;
		else
			state = NONE;
		break;
	case STEP5:
		if ((keycode == LOCKUP_THIRD_KEY) && !keypress)
			state = STEP6;
		else
			state = NONE;
		break;
	case STEP6:
		if ((keycode == LOCKUP_FIRST_KEY) && keypress)
			state = STEP7;
		else
			state = NONE;
		break;
	case STEP7:
		if ((keycode == LOCKUP_FIRST_KEY) && !keypress)
			state = STEP8;
		else
			state = NONE;
		break;
	case STEP8:
		if ((keycode == LOCKUP_THIRD_KEY) && keypress)
			panic("[Crash Key] LOCKUP CAPTURED!!!");
		else
			state = NONE;
		break;
	default:
		break;
	}
#endif
}
EXPORT_SYMBOL(sec_check_crash_key);

#if defined(CONFIG_DEBUG_FS) && defined(CONFIG_DEBUG_PANIC_TEST)
#include <linux/debugfs.h>

static struct dentry *debug_panic_dent;
static spinlock_t debug_panic_spinlock;

struct debug_panic_type {
	void (*func)(void);
	char *desc;
};

void debug_panic_dabort(void)
{
	int *p = 0;
	*p = 0;
}

void debug_panic_pabort(void)
{
	void (*p)(void) = 0;
	p();
}
void debug_panic_lockup(void)
{
	unsigned long flags;
	spin_lock_irqsave(&debug_panic_spinlock, flags);
	while (1)
		;
	spin_unlock_irqrestore(&debug_panic_spinlock, flags);
}

void debug_panic_spinlock_bug(void)
{
	spin_lock(&debug_panic_spinlock);
	spin_lock(&debug_panic_spinlock);

	spin_unlock(&debug_panic_spinlock);
}

void debug_panic_sched_while_atomic(void)
{
	spin_lock(&debug_panic_spinlock);
	msleep(20);
	spin_unlock(&debug_panic_spinlock);
}

void debug_panic_sched_with_irqoff(void)
{
	unsigned long flags;

	raw_local_irq_save(flags);
	msleep(20);
}

struct debug_panic_type debug_panic_scenario[] = {
	[0] = {
		.func = debug_panic_dabort,
		.desc = "data abort\n"
	},
	[1] = {
		.func = debug_panic_pabort,
		.desc = "prefetch abort\n"
	},
	[2] = {
		.func = debug_panic_lockup,
		.desc = "lockup\n"
	},
	[3] = {
		.func = debug_panic_spinlock_bug,
		.desc = "spinlock bug\n"
	},
	[4] = {
		.func = debug_panic_sched_while_atomic,
		.desc = "schedule while atomic\n"
	},
	[5] = {
		.func = debug_panic_sched_with_irqoff,
		.desc = "schedule with irq disabled\n"
	},

};

static int debug_panic_open(struct inode *inode, struct file *file)
{
	file->private_data = inode->i_private;
	return 0;
}

static ssize_t debug_panic_read(struct file *file, char __user *buf,
			  size_t count, loff_t *ppos)
{
	struct debug_panic_type *panic_type = file->private_data;
	ssize_t rc;

	rc = simple_read_from_buffer((void __user *) buf, count,\
		ppos, (void *) panic_type->desc,\
		strlen(panic_type->desc));

	return rc;
}
static ssize_t debug_panic_write(struct file *file, const char __user *buf,
			   size_t count, loff_t *ppos)
{
	struct debug_panic_type *panic_type = file->private_data;

	pr_info("@@ %s %s\n", __func__, panic_type->desc);
	msleep(500);

	panic_type->func();

	return count;
}

static const struct file_operations debug_panic_ops = {
	.open =         debug_panic_open,
	.read =         debug_panic_read,
	.write =        debug_panic_write,
};

#define DEBUG_MAX_FNAME 16
void debug_panic_init(void)
{
	int i;
	char name[DEBUG_MAX_FNAME];

	spin_lock_init(&debug_panic_spinlock);

	debug_panic_dent = debugfs_create_dir("panic", NULL);
	if (IS_ERR(debug_panic_dent)) {
		pr_err("panic debugfs_create_dir fail, error %ld\n",
		       PTR_ERR(debug_panic_dent));
		return;
	}

	for (i = 0; i < ARRAY_SIZE(debug_panic_scenario); i++) {
		snprintf(name, DEBUG_MAX_FNAME-1, "panic-%d", i);
		if (debugfs_create_file(name, 0644, debug_panic_dent,\
			&debug_panic_scenario[i], &debug_panic_ops) == NULL) {
			pr_err("pmic8058 debugfs_create_file %s failed\n",\
				name);
		}
	}
}

static void debug_panic_exit(void)
{
	debugfs_remove_recursive(debug_panic_dent);
}

#else
static void debug_panic_init(void) { }
static void debug_panic_exit(void) { }
#endif
#endif /* CONFIG_SEC_DEBUG */
