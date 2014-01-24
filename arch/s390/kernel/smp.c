/*
<<<<<<< HEAD
<<<<<<< HEAD
 *  arch/s390/kernel/smp.c
 *
 *    Copyright IBM Corp. 1999, 2009
 *    Author(s): Denis Joseph Barrow (djbarrow@de.ibm.com,barrow_dj@yahoo.com),
 *		 Martin Schwidefsky (schwidefsky@de.ibm.com)
 *		 Heiko Carstens (heiko.carstens@de.ibm.com)
=======
 *  SMP related functions
 *
 *    Copyright IBM Corp. 1999,2012
 *    Author(s): Denis Joseph Barrow,
 *		 Martin Schwidefsky <schwidefsky@de.ibm.com>,
 *		 Heiko Carstens <heiko.carstens@de.ibm.com>,
>>>>>>> refs/remotes/origin/cm-10.0
=======
 *  SMP related functions
 *
 *    Copyright IBM Corp. 1999, 2012
 *    Author(s): Denis Joseph Barrow,
 *		 Martin Schwidefsky <schwidefsky@de.ibm.com>,
 *		 Heiko Carstens <heiko.carstens@de.ibm.com>,
>>>>>>> refs/remotes/origin/master
 *
 *  based on other smp stuff by
 *    (c) 1995 Alan Cox, CymruNET Ltd  <alan@cymru.net>
 *    (c) 1998 Ingo Molnar
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * We work with logical cpu numbering everywhere we can. The only
 * functions using the real cpu address (got from STAP) are the sigp
 * functions. For all other functions we use the identity mapping.
 * That means that cpu_number_map[i] == i for every cpu. cpu_number_map is
 * used e.g. to find the idle task belonging to a logical cpu. Every array
 * in the kernel is sorted by the logical cpu number and not by the physical
 * one which is causing all the confusion with __cpu_logical_map and
 * cpu_number_map in other architectures.
=======
 * The code outside of smp.c uses logical cpu numbers, only smp.c does
 * the translation of logical to physical cpu ids. All new code that
 * operates on physical cpu numbers needs to go into smp.c.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * The code outside of smp.c uses logical cpu numbers, only smp.c does
 * the translation of logical to physical cpu ids. All new code that
 * operates on physical cpu numbers needs to go into smp.c.
>>>>>>> refs/remotes/origin/master
 */

#define KMSG_COMPONENT "cpu"
#define pr_fmt(fmt) KMSG_COMPONENT ": " fmt

#include <linux/workqueue.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/err.h>
#include <linux/spinlock.h>
#include <linux/kernel_stat.h>
#include <linux/delay.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/cache.h>
#include <linux/interrupt.h>
#include <linux/irqflags.h>
#include <linux/cpu.h>
#include <linux/timex.h>
#include <linux/bootmem.h>
#include <linux/slab.h>
#include <asm/asm-offsets.h>
#include <asm/ipl.h>
#include <asm/setup.h>
#include <asm/sigp.h>
#include <asm/pgalloc.h>
#include <asm/irq.h>
#include <asm/cpcmd.h>
=======
=======
>>>>>>> refs/remotes/origin/master
#include <linux/interrupt.h>
#include <linux/irqflags.h>
#include <linux/cpu.h>
#include <linux/slab.h>
#include <linux/crash_dump.h>
#include <asm/asm-offsets.h>
#include <asm/switch_to.h>
#include <asm/facility.h>
#include <asm/ipl.h>
#include <asm/setup.h>
#include <asm/irq.h>
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
#include <asm/tlbflush.h>
#include <asm/timer.h>
#include <asm/lowcore.h>
#include <asm/sclp.h>
<<<<<<< HEAD
#include <asm/cputime.h>
#include <asm/vdso.h>
#include <asm/cpu.h>
#include "entry.h"

/* logical cpu to cpu address */
unsigned short __cpu_logical_map[NR_CPUS];

static struct task_struct *current_set[NR_CPUS];

static u8 smp_cpu_type;
static int smp_use_sigp_detection;

enum s390_cpu_state {
=======
#include <asm/vdso.h>
#include <asm/debug.h>
#include <asm/os_info.h>
#include "entry.h"

enum {
	sigp_sense = 1,
	sigp_external_call = 2,
	sigp_emergency_signal = 3,
	sigp_start = 4,
	sigp_stop = 5,
	sigp_restart = 6,
	sigp_stop_and_store_status = 9,
	sigp_initial_cpu_reset = 11,
	sigp_cpu_reset = 12,
	sigp_set_prefix = 13,
	sigp_store_status_at_address = 14,
	sigp_store_extended_status_at_address = 15,
	sigp_set_architecture = 18,
	sigp_conditional_emergency_signal = 19,
	sigp_sense_running = 21,
};

enum {
	sigp_order_code_accepted = 0,
	sigp_status_stored = 1,
	sigp_busy = 2,
	sigp_not_operational = 3,
};

enum {
	ec_schedule = 0,
	ec_call_function,
=======
#include <asm/tlbflush.h>
#include <asm/vtimer.h>
#include <asm/lowcore.h>
#include <asm/sclp.h>
#include <asm/vdso.h>
#include <asm/debug.h>
#include <asm/os_info.h>
#include <asm/sigp.h>
#include "entry.h"

enum {
	ec_schedule = 0,
>>>>>>> refs/remotes/origin/master
	ec_call_function_single,
	ec_stop_cpu,
};

enum {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	CPU_STATE_STANDBY,
	CPU_STATE_CONFIGURED,
};

<<<<<<< HEAD
<<<<<<< HEAD
DEFINE_MUTEX(smp_cpu_state_mutex);
int smp_cpu_polarization[NR_CPUS];
static int smp_cpu_state[NR_CPUS];
static int cpu_management;

static DEFINE_PER_CPU(struct cpu, cpu_devices);

static void smp_ext_bitcall(int, int);

static int raw_cpu_stopped(int cpu)
{
	u32 status;

	switch (raw_sigp_ps(&status, 0, cpu, sigp_sense)) {
	case sigp_status_stored:
		/* Check for stopped and check stop state */
		if (status & 0x50)
			return 1;
		break;
	default:
		break;
	}
	return 0;
}

static inline int cpu_stopped(int cpu)
{
	return raw_cpu_stopped(cpu_logical_map(cpu));
}

void smp_switch_to_ipl_cpu(void (*func)(void *), void *data)
{
	struct _lowcore *lc, *current_lc;
	struct stack_frame *sf;
	struct pt_regs *regs;
	unsigned long sp;

	if (smp_processor_id() == 0)
		func(data);
	__load_psw_mask(PSW_BASE_BITS | PSW_DEFAULT_KEY);
	/* Disable lowcore protection */
	__ctl_clear_bit(0, 28);
	current_lc = lowcore_ptr[smp_processor_id()];
	lc = lowcore_ptr[0];
	if (!lc)
		lc = current_lc;
	lc->restart_psw.mask = PSW_BASE_BITS | PSW_DEFAULT_KEY;
	lc->restart_psw.addr = PSW_ADDR_AMODE | (unsigned long) smp_restart_cpu;
	if (!cpu_online(0))
		smp_switch_to_cpu(func, data, 0, stap(), __cpu_logical_map[0]);
	while (sigp(0, sigp_stop_and_store_status) == sigp_busy)
		cpu_relax();
	sp = lc->panic_stack;
	sp -= sizeof(struct pt_regs);
	regs = (struct pt_regs *) sp;
	memcpy(&regs->gprs, &current_lc->gpregs_save_area, sizeof(regs->gprs));
	regs->psw = lc->psw_save_area;
	sp -= STACK_FRAME_OVERHEAD;
	sf = (struct stack_frame *) sp;
	sf->back_chain = regs->gprs[15];
	smp_switch_to_cpu(func, data, sp, stap(), __cpu_logical_map[0]);
}

void smp_send_stop(void)
{
	int cpu, rc;

	/* Disable all interrupts/machine checks */
	__load_psw_mask(psw_kernel_bits & ~PSW_MASK_MCHECK);
	trace_hardirqs_off();

	/* stop all processors */
	for_each_online_cpu(cpu) {
		if (cpu == smp_processor_id())
			continue;
		do {
			rc = sigp(cpu, sigp_stop);
		} while (rc == sigp_busy);

		while (!cpu_stopped(cpu))
=======
struct pcpu {
	struct cpu cpu;
	struct task_struct *idle;	/* idle process for the cpu */
=======
struct pcpu {
	struct cpu *cpu;
>>>>>>> refs/remotes/origin/master
	struct _lowcore *lowcore;	/* lowcore page(s) for the cpu */
	unsigned long async_stack;	/* async stack for the cpu */
	unsigned long panic_stack;	/* panic stack for the cpu */
	unsigned long ec_mask;		/* bit mask for ec_xxx functions */
	int state;			/* physical cpu state */
<<<<<<< HEAD
	u32 status;			/* last status received via sigp */
=======
	int polarization;		/* physical polarization */
>>>>>>> refs/remotes/origin/master
	u16 address;			/* physical cpu address */
};

static u8 boot_cpu_type;
static u16 boot_cpu_address;
static struct pcpu pcpu_devices[NR_CPUS];

<<<<<<< HEAD
=======
/*
 * The smp_cpu_state_mutex must be held when changing the state or polarization
 * member of a pcpu data structure within the pcpu_devices arreay.
 */
>>>>>>> refs/remotes/origin/master
DEFINE_MUTEX(smp_cpu_state_mutex);

/*
 * Signal processor helper functions.
 */
static inline int __pcpu_sigp(u16 addr, u8 order, u32 parm, u32 *status)
{
	register unsigned int reg1 asm ("1") = parm;
	int cc;

	asm volatile(
		"	sigp	%1,%2,0(%3)\n"
		"	ipm	%0\n"
		"	srl	%0,28\n"
		: "=d" (cc), "+d" (reg1) : "d" (addr), "a" (order) : "cc");
	if (status && cc == 1)
		*status = reg1;
	return cc;
}

static inline int __pcpu_sigp_relax(u16 addr, u8 order, u32 parm, u32 *status)
{
	int cc;

	while (1) {
<<<<<<< HEAD
		cc = __pcpu_sigp(addr, order, parm, status);
		if (cc != sigp_busy)
=======
		cc = __pcpu_sigp(addr, order, parm, NULL);
		if (cc != SIGP_CC_BUSY)
>>>>>>> refs/remotes/origin/master
			return cc;
		cpu_relax();
	}
}

static int pcpu_sigp_retry(struct pcpu *pcpu, u8 order, u32 parm)
{
	int cc, retry;

	for (retry = 0; ; retry++) {
<<<<<<< HEAD
		cc = __pcpu_sigp(pcpu->address, order, parm, &pcpu->status);
		if (cc != sigp_busy)
=======
		cc = __pcpu_sigp(pcpu->address, order, parm, NULL);
		if (cc != SIGP_CC_BUSY)
>>>>>>> refs/remotes/origin/master
			break;
		if (retry >= 3)
			udelay(10);
	}
	return cc;
}

static inline int pcpu_stopped(struct pcpu *pcpu)
{
<<<<<<< HEAD
	if (__pcpu_sigp(pcpu->address, sigp_sense,
			0, &pcpu->status) != sigp_status_stored)
		return 0;
	/* Check for stopped and check stop state */
	return !!(pcpu->status & 0x50);
=======
	u32 uninitialized_var(status);

	if (__pcpu_sigp(pcpu->address, SIGP_SENSE,
			0, &status) != SIGP_CC_STATUS_STORED)
		return 0;
	return !!(status & (SIGP_STATUS_CHECK_STOP|SIGP_STATUS_STOPPED));
>>>>>>> refs/remotes/origin/master
}

static inline int pcpu_running(struct pcpu *pcpu)
{
<<<<<<< HEAD
	if (__pcpu_sigp(pcpu->address, sigp_sense_running,
			0, &pcpu->status) != sigp_status_stored)
		return 1;
	/* Check for running status */
	return !(pcpu->status & 0x400);
=======
	if (__pcpu_sigp(pcpu->address, SIGP_SENSE_RUNNING,
			0, NULL) != SIGP_CC_STATUS_STORED)
		return 1;
	/* Status stored condition code is equivalent to cpu not running. */
	return 0;
>>>>>>> refs/remotes/origin/master
}

/*
 * Find struct pcpu by cpu address.
 */
static struct pcpu *pcpu_find_address(const struct cpumask *mask, int address)
{
	int cpu;

	for_each_cpu(cpu, mask)
		if (pcpu_devices[cpu].address == address)
			return pcpu_devices + cpu;
	return NULL;
}

static void pcpu_ec_call(struct pcpu *pcpu, int ec_bit)
{
	int order;

<<<<<<< HEAD
	set_bit(ec_bit, &pcpu->ec_mask);
	order = pcpu_running(pcpu) ?
		sigp_external_call : sigp_emergency_signal;
	pcpu_sigp_retry(pcpu, order, 0);
}

static int __cpuinit pcpu_alloc_lowcore(struct pcpu *pcpu, int cpu)
=======
	if (test_and_set_bit(ec_bit, &pcpu->ec_mask))
		return;
	order = pcpu_running(pcpu) ? SIGP_EXTERNAL_CALL : SIGP_EMERGENCY_SIGNAL;
	pcpu_sigp_retry(pcpu, order, 0);
}

static int pcpu_alloc_lowcore(struct pcpu *pcpu, int cpu)
>>>>>>> refs/remotes/origin/master
{
	struct _lowcore *lc;

	if (pcpu != &pcpu_devices[0]) {
		pcpu->lowcore =	(struct _lowcore *)
			__get_free_pages(GFP_KERNEL | GFP_DMA, LC_ORDER);
		pcpu->async_stack = __get_free_pages(GFP_KERNEL, ASYNC_ORDER);
		pcpu->panic_stack = __get_free_page(GFP_KERNEL);
		if (!pcpu->lowcore || !pcpu->panic_stack || !pcpu->async_stack)
			goto out;
	}
	lc = pcpu->lowcore;
	memcpy(lc, &S390_lowcore, 512);
	memset((char *) lc + 512, 0, sizeof(*lc) - 512);
<<<<<<< HEAD
	lc->async_stack = pcpu->async_stack + ASYNC_SIZE;
	lc->panic_stack = pcpu->panic_stack + PAGE_SIZE;
=======
	lc->async_stack = pcpu->async_stack + ASYNC_SIZE
		- STACK_FRAME_OVERHEAD - sizeof(struct pt_regs);
	lc->panic_stack = pcpu->panic_stack + PAGE_SIZE
		- STACK_FRAME_OVERHEAD - sizeof(struct pt_regs);
>>>>>>> refs/remotes/origin/master
	lc->cpu_nr = cpu;
#ifndef CONFIG_64BIT
	if (MACHINE_HAS_IEEE) {
		lc->extended_save_area_addr = get_zeroed_page(GFP_KERNEL);
		if (!lc->extended_save_area_addr)
			goto out;
	}
#else
	if (vdso_alloc_per_cpu(lc))
		goto out;
#endif
	lowcore_ptr[cpu] = lc;
<<<<<<< HEAD
	pcpu_sigp_retry(pcpu, sigp_set_prefix, (u32)(unsigned long) lc);
=======
	pcpu_sigp_retry(pcpu, SIGP_SET_PREFIX, (u32)(unsigned long) lc);
>>>>>>> refs/remotes/origin/master
	return 0;
out:
	if (pcpu != &pcpu_devices[0]) {
		free_page(pcpu->panic_stack);
		free_pages(pcpu->async_stack, ASYNC_ORDER);
		free_pages((unsigned long) pcpu->lowcore, LC_ORDER);
	}
	return -ENOMEM;
}

<<<<<<< HEAD
static void pcpu_free_lowcore(struct pcpu *pcpu)
{
	pcpu_sigp_retry(pcpu, sigp_set_prefix, 0);
=======
#ifdef CONFIG_HOTPLUG_CPU

static void pcpu_free_lowcore(struct pcpu *pcpu)
{
	pcpu_sigp_retry(pcpu, SIGP_SET_PREFIX, 0);
>>>>>>> refs/remotes/origin/master
	lowcore_ptr[pcpu - pcpu_devices] = NULL;
#ifndef CONFIG_64BIT
	if (MACHINE_HAS_IEEE) {
		struct _lowcore *lc = pcpu->lowcore;

		free_page((unsigned long) lc->extended_save_area_addr);
		lc->extended_save_area_addr = 0;
	}
#else
	vdso_free_per_cpu(pcpu->lowcore);
#endif
	if (pcpu != &pcpu_devices[0]) {
		free_page(pcpu->panic_stack);
		free_pages(pcpu->async_stack, ASYNC_ORDER);
		free_pages((unsigned long) pcpu->lowcore, LC_ORDER);
	}
}

<<<<<<< HEAD
=======
#endif /* CONFIG_HOTPLUG_CPU */

>>>>>>> refs/remotes/origin/master
static void pcpu_prepare_secondary(struct pcpu *pcpu, int cpu)
{
	struct _lowcore *lc = pcpu->lowcore;

	atomic_inc(&init_mm.context.attach_count);
	lc->cpu_nr = cpu;
	lc->percpu_offset = __per_cpu_offset[cpu];
	lc->kernel_asce = S390_lowcore.kernel_asce;
	lc->machine_flags = S390_lowcore.machine_flags;
	lc->ftrace_func = S390_lowcore.ftrace_func;
	lc->user_timer = lc->system_timer = lc->steal_timer = 0;
	__ctl_store(lc->cregs_save_area, 0, 15);
	save_access_regs((unsigned int *) lc->access_regs_save_area);
	memcpy(lc->stfle_fac_list, S390_lowcore.stfle_fac_list,
	       MAX_FACILITY_BIT/8);
}

static void pcpu_attach_task(struct pcpu *pcpu, struct task_struct *tsk)
{
	struct _lowcore *lc = pcpu->lowcore;
	struct thread_info *ti = task_thread_info(tsk);

<<<<<<< HEAD
	lc->kernel_stack = (unsigned long) task_stack_page(tsk) + THREAD_SIZE;
=======
	lc->kernel_stack = (unsigned long) task_stack_page(tsk)
		+ THREAD_SIZE - STACK_FRAME_OVERHEAD - sizeof(struct pt_regs);
>>>>>>> refs/remotes/origin/master
	lc->thread_info = (unsigned long) task_thread_info(tsk);
	lc->current_task = (unsigned long) tsk;
	lc->user_timer = ti->user_timer;
	lc->system_timer = ti->system_timer;
	lc->steal_timer = 0;
}

static void pcpu_start_fn(struct pcpu *pcpu, void (*func)(void *), void *data)
{
	struct _lowcore *lc = pcpu->lowcore;

	lc->restart_stack = lc->kernel_stack;
	lc->restart_fn = (unsigned long) func;
	lc->restart_data = (unsigned long) data;
	lc->restart_source = -1UL;
<<<<<<< HEAD
	pcpu_sigp_retry(pcpu, sigp_restart, 0);
=======
	pcpu_sigp_retry(pcpu, SIGP_RESTART, 0);
>>>>>>> refs/remotes/origin/master
}

/*
 * Call function via PSW restart on pcpu and stop the current cpu.
 */
static void pcpu_delegate(struct pcpu *pcpu, void (*func)(void *),
			  void *data, unsigned long stack)
{
<<<<<<< HEAD
	struct _lowcore *lc = pcpu->lowcore;
	unsigned short this_cpu;

	__load_psw_mask(psw_kernel_bits);
	this_cpu = stap();
	if (pcpu->address == this_cpu)
		func(data);	/* should not return */
	/* Stop target cpu (if func returns this stops the current cpu). */
	pcpu_sigp_retry(pcpu, sigp_stop, 0);
	/* Restart func on the target cpu and stop the current cpu. */
	lc->restart_stack = stack;
	lc->restart_fn = (unsigned long) func;
	lc->restart_data = (unsigned long) data;
	lc->restart_source = (unsigned long) this_cpu;
	asm volatile(
		"0:	sigp	0,%0,6	# sigp restart to target cpu\n"
		"	brc	2,0b	# busy, try again\n"
		"1:	sigp	0,%1,5	# sigp stop to current cpu\n"
		"	brc	2,1b	# busy, try again\n"
		: : "d" (pcpu->address), "d" (this_cpu) : "0", "1", "cc");
=======
	struct _lowcore *lc = lowcore_ptr[pcpu - pcpu_devices];
	unsigned long source_cpu = stap();

	__load_psw_mask(PSW_KERNEL_BITS);
	if (pcpu->address == source_cpu)
		func(data);	/* should not return */
	/* Stop target cpu (if func returns this stops the current cpu). */
	pcpu_sigp_retry(pcpu, SIGP_STOP, 0);
	/* Restart func on the target cpu and stop the current cpu. */
	mem_assign_absolute(lc->restart_stack, stack);
	mem_assign_absolute(lc->restart_fn, (unsigned long) func);
	mem_assign_absolute(lc->restart_data, (unsigned long) data);
	mem_assign_absolute(lc->restart_source, source_cpu);
	asm volatile(
		"0:	sigp	0,%0,%2	# sigp restart to target cpu\n"
		"	brc	2,0b	# busy, try again\n"
		"1:	sigp	0,%1,%3	# sigp stop to current cpu\n"
		"	brc	2,1b	# busy, try again\n"
		: : "d" (pcpu->address), "d" (source_cpu),
		    "K" (SIGP_RESTART), "K" (SIGP_STOP)
		: "0", "1", "cc");
>>>>>>> refs/remotes/origin/master
	for (;;) ;
}

/*
 * Call function on an online CPU.
 */
void smp_call_online_cpu(void (*func)(void *), void *data)
{
	struct pcpu *pcpu;

	/* Use the current cpu if it is online. */
	pcpu = pcpu_find_address(cpu_online_mask, stap());
	if (!pcpu)
		/* Use the first online cpu. */
		pcpu = pcpu_devices + cpumask_first(cpu_online_mask);
	pcpu_delegate(pcpu, func, data, (unsigned long) restart_stack);
}

/*
 * Call function on the ipl CPU.
 */
void smp_call_ipl_cpu(void (*func)(void *), void *data)
{
	pcpu_delegate(&pcpu_devices[0], func, data,
		      pcpu_devices->panic_stack + PAGE_SIZE);
}

int smp_find_processor_id(u16 address)
{
	int cpu;

	for_each_present_cpu(cpu)
		if (pcpu_devices[cpu].address == address)
			return cpu;
	return -1;
}

int smp_vcpu_scheduled(int cpu)
{
	return pcpu_running(pcpu_devices + cpu);
}

void smp_yield(void)
{
	if (MACHINE_HAS_DIAG44)
		asm volatile("diag 0,0,0x44");
}

void smp_yield_cpu(int cpu)
{
	if (MACHINE_HAS_DIAG9C)
		asm volatile("diag %0,0,0x9c"
			     : : "d" (pcpu_devices[cpu].address));
	else if (MACHINE_HAS_DIAG44)
		asm volatile("diag 0,0,0x44");
}

/*
 * Send cpus emergency shutdown signal. This gives the cpus the
 * opportunity to complete outstanding interrupts.
 */
<<<<<<< HEAD
void smp_emergency_stop(cpumask_t *cpumask)
=======
static void smp_emergency_stop(cpumask_t *cpumask)
>>>>>>> refs/remotes/origin/master
{
	u64 end;
	int cpu;

<<<<<<< HEAD
	end = get_clock() + (1000000UL << 12);
	for_each_cpu(cpu, cpumask) {
		struct pcpu *pcpu = pcpu_devices + cpu;
		set_bit(ec_stop_cpu, &pcpu->ec_mask);
		while (__pcpu_sigp(pcpu->address, sigp_emergency_signal,
				   0, NULL) == sigp_busy &&
		       get_clock() < end)
			cpu_relax();
	}
	while (get_clock() < end) {
=======
	end = get_tod_clock() + (1000000UL << 12);
	for_each_cpu(cpu, cpumask) {
		struct pcpu *pcpu = pcpu_devices + cpu;
		set_bit(ec_stop_cpu, &pcpu->ec_mask);
		while (__pcpu_sigp(pcpu->address, SIGP_EMERGENCY_SIGNAL,
				   0, NULL) == SIGP_CC_BUSY &&
		       get_tod_clock() < end)
			cpu_relax();
	}
	while (get_tod_clock() < end) {
>>>>>>> refs/remotes/origin/master
		for_each_cpu(cpu, cpumask)
			if (pcpu_stopped(pcpu_devices + cpu))
				cpumask_clear_cpu(cpu, cpumask);
		if (cpumask_empty(cpumask))
			break;
		cpu_relax();
	}
}

/*
 * Stop all cpus but the current one.
 */
void smp_send_stop(void)
{
	cpumask_t cpumask;
	int cpu;

	/* Disable all interrupts/machine checks */
<<<<<<< HEAD
	__load_psw_mask(psw_kernel_bits | PSW_MASK_DAT);
=======
	__load_psw_mask(PSW_KERNEL_BITS | PSW_MASK_DAT);
>>>>>>> refs/remotes/origin/master
	trace_hardirqs_off();

	debug_set_critical();
	cpumask_copy(&cpumask, cpu_online_mask);
	cpumask_clear_cpu(smp_processor_id(), &cpumask);

	if (oops_in_progress)
		smp_emergency_stop(&cpumask);

	/* stop all processors */
	for_each_cpu(cpu, &cpumask) {
		struct pcpu *pcpu = pcpu_devices + cpu;
<<<<<<< HEAD
		pcpu_sigp_retry(pcpu, sigp_stop, 0);
		while (!pcpu_stopped(pcpu))
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pcpu_sigp_retry(pcpu, SIGP_STOP, 0);
		while (!pcpu_stopped(pcpu))
>>>>>>> refs/remotes/origin/master
			cpu_relax();
	}
}

/*
<<<<<<< HEAD
<<<<<<< HEAD
 * This is the main routine where commands issued by other
 * cpus are handled.
 */

static void do_ext_call_interrupt(unsigned int ext_int_code,
				  unsigned int param32, unsigned long param64)
{
	unsigned long bits;

	kstat_cpu(smp_processor_id()).irqs[EXTINT_IPI]++;
	/*
	 * handle bit signal external calls
	 */
	bits = xchg(&S390_lowcore.ext_call_fast, 0);
=======
=======
>>>>>>> refs/remotes/origin/master
 * Stop the current cpu.
 */
void smp_stop_cpu(void)
{
<<<<<<< HEAD
	pcpu_sigp_retry(pcpu_devices + smp_processor_id(), sigp_stop, 0);
=======
	pcpu_sigp_retry(pcpu_devices + smp_processor_id(), SIGP_STOP, 0);
>>>>>>> refs/remotes/origin/master
	for (;;) ;
}

/*
 * This is the main routine where commands issued by other
 * cpus are handled.
 */
<<<<<<< HEAD
static void do_ext_call_interrupt(struct ext_code ext_code,
				  unsigned int param32, unsigned long param64)
{
	unsigned long bits;
	int cpu;

	cpu = smp_processor_id();
	if (ext_code.code == 0x1202)
		kstat_cpu(cpu).irqs[EXTINT_EXC]++;
	else
		kstat_cpu(cpu).irqs[EXTINT_EMS]++;
	/*
	 * handle bit signal external calls
	 */
	bits = xchg(&pcpu_devices[cpu].ec_mask, 0);

	if (test_bit(ec_stop_cpu, &bits))
		smp_stop_cpu();
>>>>>>> refs/remotes/origin/cm-10.0

	if (test_bit(ec_schedule, &bits))
		scheduler_ipi();

	if (test_bit(ec_call_function, &bits))
		generic_smp_call_function_interrupt();

	if (test_bit(ec_call_function_single, &bits))
		generic_smp_call_function_single_interrupt();
<<<<<<< HEAD
}

/*
 * Send an external call sigp to another cpu and return without waiting
 * for its completion.
 */
static void smp_ext_bitcall(int cpu, int sig)
{
	/*
	 * Set signaling bit in lowcore of target cpu and kick it
	 */
	set_bit(sig, (unsigned long *) &lowcore_ptr[cpu]->ext_call_fast);
	while (sigp(cpu, sigp_emergency_signal) == sigp_busy)
		udelay(10);
=======

>>>>>>> refs/remotes/origin/cm-10.0
=======
static void smp_handle_ext_call(void)
{
	unsigned long bits;

	/* handle bit signal external calls */
	bits = xchg(&pcpu_devices[smp_processor_id()].ec_mask, 0);
	if (test_bit(ec_stop_cpu, &bits))
		smp_stop_cpu();
	if (test_bit(ec_schedule, &bits))
		scheduler_ipi();
	if (test_bit(ec_call_function_single, &bits))
		generic_smp_call_function_single_interrupt();
}

static void do_ext_call_interrupt(struct ext_code ext_code,
				  unsigned int param32, unsigned long param64)
{
	inc_irq_stat(ext_code.code == 0x1202 ? IRQEXT_EXC : IRQEXT_EMS);
	smp_handle_ext_call();
>>>>>>> refs/remotes/origin/master
}

void arch_send_call_function_ipi_mask(const struct cpumask *mask)
{
	int cpu;

	for_each_cpu(cpu, mask)
<<<<<<< HEAD
<<<<<<< HEAD
		smp_ext_bitcall(cpu, ec_call_function);
=======
		pcpu_ec_call(pcpu_devices + cpu, ec_call_function);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pcpu_ec_call(pcpu_devices + cpu, ec_call_function_single);
>>>>>>> refs/remotes/origin/master
}

void arch_send_call_function_single_ipi(int cpu)
{
<<<<<<< HEAD
<<<<<<< HEAD
	smp_ext_bitcall(cpu, ec_call_function_single);
=======
	pcpu_ec_call(pcpu_devices + cpu, ec_call_function_single);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pcpu_ec_call(pcpu_devices + cpu, ec_call_function_single);
>>>>>>> refs/remotes/origin/master
}

#ifndef CONFIG_64BIT
/*
 * this function sends a 'purge tlb' signal to another CPU.
 */
static void smp_ptlb_callback(void *info)
{
	__tlb_flush_local();
}

void smp_ptlb_all(void)
{
	on_each_cpu(smp_ptlb_callback, NULL, 1);
}
EXPORT_SYMBOL(smp_ptlb_all);
#endif /* ! CONFIG_64BIT */

/*
 * this function sends a 'reschedule' IPI to another CPU.
 * it goes straight through and wastes no time serializing
 * anything. Worst case is that we lose a reschedule ...
 */
void smp_send_reschedule(int cpu)
{
<<<<<<< HEAD
<<<<<<< HEAD
	smp_ext_bitcall(cpu, ec_schedule);
=======
	pcpu_ec_call(pcpu_devices + cpu, ec_schedule);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pcpu_ec_call(pcpu_devices + cpu, ec_schedule);
>>>>>>> refs/remotes/origin/master
}

/*
 * parameter area for the set/clear control bit callbacks
 */
struct ec_creg_mask_parms {
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long orvals[16];
	unsigned long andvals[16];
=======
	unsigned long orval;
	unsigned long andval;
	int cr;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	unsigned long orval;
	unsigned long andval;
	int cr;
>>>>>>> refs/remotes/origin/master
};

/*
 * callback for setting/clearing control bits
 */
static void smp_ctl_bit_callback(void *info)
{
	struct ec_creg_mask_parms *pp = info;
	unsigned long cregs[16];
<<<<<<< HEAD
<<<<<<< HEAD
	int i;

	__ctl_store(cregs, 0, 15);
	for (i = 0; i <= 15; i++)
		cregs[i] = (cregs[i] & pp->andvals[i]) | pp->orvals[i];
=======

	__ctl_store(cregs, 0, 15);
	cregs[pp->cr] = (cregs[pp->cr] & pp->andval) | pp->orval;
>>>>>>> refs/remotes/origin/cm-10.0
=======

	__ctl_store(cregs, 0, 15);
	cregs[pp->cr] = (cregs[pp->cr] & pp->andval) | pp->orval;
>>>>>>> refs/remotes/origin/master
	__ctl_load(cregs, 0, 15);
}

/*
 * Set a bit in a control register of all cpus
 */
void smp_ctl_set_bit(int cr, int bit)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct ec_creg_mask_parms parms;

	memset(&parms.orvals, 0, sizeof(parms.orvals));
	memset(&parms.andvals, 0xff, sizeof(parms.andvals));
	parms.orvals[cr] = 1UL << bit;
=======
	struct ec_creg_mask_parms parms = { 1UL << bit, -1UL, cr };

>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct ec_creg_mask_parms parms = { 1UL << bit, -1UL, cr };

>>>>>>> refs/remotes/origin/master
	on_each_cpu(smp_ctl_bit_callback, &parms, 1);
}
EXPORT_SYMBOL(smp_ctl_set_bit);

/*
 * Clear a bit in a control register of all cpus
 */
void smp_ctl_clear_bit(int cr, int bit)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct ec_creg_mask_parms parms;

	memset(&parms.orvals, 0, sizeof(parms.orvals));
	memset(&parms.andvals, 0xff, sizeof(parms.andvals));
	parms.andvals[cr] = ~(1UL << bit);
=======
	struct ec_creg_mask_parms parms = { 0, ~(1UL << bit), cr };

>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct ec_creg_mask_parms parms = { 0, ~(1UL << bit), cr };

>>>>>>> refs/remotes/origin/master
	on_each_cpu(smp_ctl_bit_callback, &parms, 1);
}
EXPORT_SYMBOL(smp_ctl_clear_bit);

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_ZFCPDUMP

static void __init smp_get_save_area(unsigned int cpu, unsigned int phy_cpu)
{
	if (ipl_info.type != IPL_TYPE_FCP_DUMP)
		return;
	if (cpu >= NR_CPUS) {
		pr_warning("CPU %i exceeds the maximum %i and is excluded from "
			   "the dump\n", cpu, NR_CPUS - 1);
		return;
	}
	zfcpdump_save_areas[cpu] = kmalloc(sizeof(struct save_area), GFP_KERNEL);
	while (raw_sigp(phy_cpu, sigp_stop_and_store_status) == sigp_busy)
		cpu_relax();
	memcpy_real(zfcpdump_save_areas[cpu],
		    (void *)(unsigned long) store_prefix() + SAVE_AREA_BASE,
		    sizeof(struct save_area));
}

struct save_area *zfcpdump_save_areas[NR_CPUS + 1];
EXPORT_SYMBOL_GPL(zfcpdump_save_areas);

#else

static inline void smp_get_save_area(unsigned int cpu, unsigned int phy_cpu) { }

#endif /* CONFIG_ZFCPDUMP */

static int cpu_known(int cpu_id)
{
	int cpu;

	for_each_present_cpu(cpu) {
		if (__cpu_logical_map[cpu] == cpu_id)
			return 1;
	}
	return 0;
}

static int smp_rescan_cpus_sigp(cpumask_t avail)
{
	int cpu_id, logical_cpu;

	logical_cpu = cpumask_first(&avail);
	if (logical_cpu >= nr_cpu_ids)
		return 0;
	for (cpu_id = 0; cpu_id <= MAX_CPU_ADDRESS; cpu_id++) {
		if (cpu_known(cpu_id))
			continue;
		__cpu_logical_map[logical_cpu] = cpu_id;
		smp_cpu_polarization[logical_cpu] = POLARIZATION_UNKNWN;
		if (!cpu_stopped(logical_cpu))
			continue;
		set_cpu_present(logical_cpu, true);
		smp_cpu_state[logical_cpu] = CPU_STATE_CONFIGURED;
		logical_cpu = cpumask_next(logical_cpu, &avail);
		if (logical_cpu >= nr_cpu_ids)
			break;
	}
	return 0;
}

static int smp_rescan_cpus_sclp(cpumask_t avail)
{
	struct sclp_cpu_info *info;
	int cpu_id, logical_cpu, cpu;
	int rc;

	logical_cpu = cpumask_first(&avail);
	if (logical_cpu >= nr_cpu_ids)
		return 0;
	info = kmalloc(sizeof(*info), GFP_KERNEL);
	if (!info)
		return -ENOMEM;
	rc = sclp_get_cpu_info(info);
	if (rc)
		goto out;
	for (cpu = 0; cpu < info->combined; cpu++) {
		if (info->has_cpu_type && info->cpu[cpu].type != smp_cpu_type)
			continue;
		cpu_id = info->cpu[cpu].address;
		if (cpu_known(cpu_id))
			continue;
		__cpu_logical_map[logical_cpu] = cpu_id;
		smp_cpu_polarization[logical_cpu] = POLARIZATION_UNKNWN;
		set_cpu_present(logical_cpu, true);
		if (cpu >= info->configured)
			smp_cpu_state[logical_cpu] = CPU_STATE_STANDBY;
		else
			smp_cpu_state[logical_cpu] = CPU_STATE_CONFIGURED;
		logical_cpu = cpumask_next(logical_cpu, &avail);
		if (logical_cpu >= nr_cpu_ids)
			break;
	}
out:
	kfree(info);
	return rc;
}

static int __smp_rescan_cpus(void)
{
	cpumask_t avail;

	cpumask_xor(&avail, cpu_possible_mask, cpu_present_mask);
	if (smp_use_sigp_detection)
		return smp_rescan_cpus_sigp(avail);
	else
		return smp_rescan_cpus_sclp(avail);
=======
#if defined(CONFIG_ZFCPDUMP) || defined(CONFIG_CRASH_DUMP)

struct save_area *zfcpdump_save_areas[NR_CPUS + 1];
EXPORT_SYMBOL_GPL(zfcpdump_save_areas);

=======
#if defined(CONFIG_ZFCPDUMP) || defined(CONFIG_CRASH_DUMP)

>>>>>>> refs/remotes/origin/master
static void __init smp_get_save_area(int cpu, u16 address)
{
	void *lc = pcpu_devices[0].lowcore;
	struct save_area *save_area;

	if (is_kdump_kernel())
		return;
	if (!OLDMEM_BASE && (address == boot_cpu_address ||
			     ipl_info.type != IPL_TYPE_FCP_DUMP))
		return;
<<<<<<< HEAD
	if (cpu >= NR_CPUS) {
		pr_warning("CPU %i exceeds the maximum %i and is excluded "
			   "from the dump\n", cpu, NR_CPUS - 1);
		return;
	}
	save_area = kmalloc(sizeof(struct save_area), GFP_KERNEL);
	if (!save_area)
		panic("could not allocate memory for save area\n");
	zfcpdump_save_areas[cpu] = save_area;
=======
	save_area = dump_save_area_create(cpu);
	if (!save_area)
		panic("could not allocate memory for save area\n");
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_CRASH_DUMP
	if (address == boot_cpu_address) {
		/* Copy the registers of the boot cpu. */
		copy_oldmem_page(1, (void *) save_area, sizeof(*save_area),
				 SAVE_AREA_BASE - PAGE_SIZE, 0);
		return;
	}
#endif
	/* Get the registers of a non-boot cpu. */
<<<<<<< HEAD
	__pcpu_sigp_relax(address, sigp_stop_and_store_status, 0, NULL);
=======
	__pcpu_sigp_relax(address, SIGP_STOP_AND_STORE_STATUS, 0, NULL);
>>>>>>> refs/remotes/origin/master
	memcpy_real(save_area, lc + SAVE_AREA_BASE, sizeof(*save_area));
}

int smp_store_status(int cpu)
{
	struct pcpu *pcpu;

	pcpu = pcpu_devices + cpu;
<<<<<<< HEAD
	if (__pcpu_sigp_relax(pcpu->address, sigp_stop_and_store_status,
			      0, NULL) != sigp_order_code_accepted)
=======
	if (__pcpu_sigp_relax(pcpu->address, SIGP_STOP_AND_STORE_STATUS,
			      0, NULL) != SIGP_CC_ORDER_CODE_ACCEPTED)
>>>>>>> refs/remotes/origin/master
		return -EIO;
	return 0;
}

#else /* CONFIG_ZFCPDUMP || CONFIG_CRASH_DUMP */

static inline void smp_get_save_area(int cpu, u16 address) { }

#endif /* CONFIG_ZFCPDUMP || CONFIG_CRASH_DUMP */

<<<<<<< HEAD
=======
void smp_cpu_set_polarization(int cpu, int val)
{
	pcpu_devices[cpu].polarization = val;
}

int smp_cpu_get_polarization(int cpu)
{
	return pcpu_devices[cpu].polarization;
}

>>>>>>> refs/remotes/origin/master
static struct sclp_cpu_info *smp_get_cpu_info(void)
{
	static int use_sigp_detection;
	struct sclp_cpu_info *info;
	int address;

	info = kzalloc(sizeof(*info), GFP_KERNEL);
	if (info && (use_sigp_detection || sclp_get_cpu_info(info))) {
		use_sigp_detection = 1;
		for (address = 0; address <= MAX_CPU_ADDRESS; address++) {
<<<<<<< HEAD
			if (__pcpu_sigp_relax(address, sigp_sense, 0, NULL) ==
			    sigp_not_operational)
=======
			if (__pcpu_sigp_relax(address, SIGP_SENSE, 0, NULL) ==
			    SIGP_CC_NOT_OPERATIONAL)
>>>>>>> refs/remotes/origin/master
				continue;
			info->cpu[info->configured].address = address;
			info->configured++;
		}
		info->combined = info->configured;
	}
	return info;
}

<<<<<<< HEAD
static int __devinit smp_add_present_cpu(int cpu);

static int __devinit __smp_rescan_cpus(struct sclp_cpu_info *info,
				       int sysfs_add)
=======
static int smp_add_present_cpu(int cpu);

static int __smp_rescan_cpus(struct sclp_cpu_info *info, int sysfs_add)
>>>>>>> refs/remotes/origin/master
{
	struct pcpu *pcpu;
	cpumask_t avail;
	int cpu, nr, i;

	nr = 0;
	cpumask_xor(&avail, cpu_possible_mask, cpu_present_mask);
	cpu = cpumask_first(&avail);
	for (i = 0; (i < info->combined) && (cpu < nr_cpu_ids); i++) {
		if (info->has_cpu_type && info->cpu[i].type != boot_cpu_type)
			continue;
		if (pcpu_find_address(cpu_present_mask, info->cpu[i].address))
			continue;
		pcpu = pcpu_devices + cpu;
		pcpu->address = info->cpu[i].address;
<<<<<<< HEAD
		pcpu->state = (cpu >= info->configured) ?
			CPU_STATE_STANDBY : CPU_STATE_CONFIGURED;
		cpu_set_polarization(cpu, POLARIZATION_UNKNOWN);
=======
		pcpu->state = (i >= info->configured) ?
			CPU_STATE_STANDBY : CPU_STATE_CONFIGURED;
		smp_cpu_set_polarization(cpu, POLARIZATION_UNKNOWN);
>>>>>>> refs/remotes/origin/master
		set_cpu_present(cpu, true);
		if (sysfs_add && smp_add_present_cpu(cpu) != 0)
			set_cpu_present(cpu, false);
		else
			nr++;
		cpu = cpumask_next(cpu, &avail);
	}
	return nr;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static void __init smp_detect_cpus(void)
{
	unsigned int cpu, c_cpus, s_cpus;
	struct sclp_cpu_info *info;
<<<<<<< HEAD
<<<<<<< HEAD
	u16 boot_cpu_addr, cpu_addr;

	c_cpus = 1;
	s_cpus = 0;
	boot_cpu_addr = __cpu_logical_map[0];
	info = kmalloc(sizeof(*info), GFP_KERNEL);
	if (!info)
		panic("smp_detect_cpus failed to allocate memory\n");
	/* Use sigp detection algorithm if sclp doesn't work. */
	if (sclp_get_cpu_info(info)) {
		smp_use_sigp_detection = 1;
		for (cpu = 0; cpu <= MAX_CPU_ADDRESS; cpu++) {
			if (cpu == boot_cpu_addr)
				continue;
			if (!raw_cpu_stopped(cpu))
				continue;
			smp_get_save_area(c_cpus, cpu);
			c_cpus++;
		}
		goto out;
	}

	if (info->has_cpu_type) {
		for (cpu = 0; cpu < info->combined; cpu++) {
			if (info->cpu[cpu].address == boot_cpu_addr) {
				smp_cpu_type = info->cpu[cpu].type;
				break;
			}
		}
	}

	for (cpu = 0; cpu < info->combined; cpu++) {
		if (info->has_cpu_type && info->cpu[cpu].type != smp_cpu_type)
			continue;
		cpu_addr = info->cpu[cpu].address;
		if (cpu_addr == boot_cpu_addr)
			continue;
		if (!raw_cpu_stopped(cpu_addr)) {
			s_cpus++;
			continue;
		}
		smp_get_save_area(c_cpus, cpu_addr);
		c_cpus++;
	}
out:
	kfree(info);
	pr_info("%d configured CPUs, %d standby CPUs\n", c_cpus, s_cpus);
	get_online_cpus();
	__smp_rescan_cpus();
	put_online_cpus();
=======
=======
>>>>>>> refs/remotes/origin/master

	info = smp_get_cpu_info();
	if (!info)
		panic("smp_detect_cpus failed to allocate memory\n");
	if (info->has_cpu_type) {
		for (cpu = 0; cpu < info->combined; cpu++) {
			if (info->cpu[cpu].address != boot_cpu_address)
				continue;
			/* The boot cpu dictates the cpu type. */
			boot_cpu_type = info->cpu[cpu].type;
			break;
		}
	}
	c_cpus = s_cpus = 0;
	for (cpu = 0; cpu < info->combined; cpu++) {
		if (info->has_cpu_type && info->cpu[cpu].type != boot_cpu_type)
			continue;
		if (cpu < info->configured) {
			smp_get_save_area(c_cpus, info->cpu[cpu].address);
			c_cpus++;
		} else
			s_cpus++;
	}
	pr_info("%d configured CPUs, %d standby CPUs\n", c_cpus, s_cpus);
	get_online_cpus();
	__smp_rescan_cpus(info, 0);
	put_online_cpus();
	kfree(info);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

/*
 *	Activate a secondary processor.
 */
<<<<<<< HEAD
<<<<<<< HEAD
int __cpuinit start_secondary(void *cpuvoid)
{
	/* Setup the cpu */
	cpu_init();
	preempt_disable();
	/* Enable TOD clock interrupts on the secondary cpu. */
	init_cpu_timer();
	/* Enable cpu timer interrupts on the secondary cpu. */
	init_cpu_vtimer();
	/* Enable pfault pseudo page faults on this cpu. */
	pfault_init();

	/* call cpu notifiers */
	notify_cpu_starting(smp_processor_id());
	/* Mark this cpu as online */
	ipi_call_lock();
	set_cpu_online(smp_processor_id(), true);
	ipi_call_unlock();
	/* Switch on interrupts */
	local_irq_enable();
	/* cpu_idle will call schedule for us */
	cpu_idle();
	return 0;
=======
static void __cpuinit smp_start_secondary(void *cpuvoid)
{
	S390_lowcore.last_update_clock = get_clock();
=======
static void smp_start_secondary(void *cpuvoid)
{
	S390_lowcore.last_update_clock = get_tod_clock();
>>>>>>> refs/remotes/origin/master
	S390_lowcore.restart_stack = (unsigned long) restart_stack;
	S390_lowcore.restart_fn = (unsigned long) do_restart;
	S390_lowcore.restart_data = 0;
	S390_lowcore.restart_source = -1UL;
	restore_access_regs(S390_lowcore.access_regs_save_area);
	__ctl_load(S390_lowcore.cregs_save_area, 0, 15);
<<<<<<< HEAD
	__load_psw_mask(psw_kernel_bits | PSW_MASK_DAT);
=======
	__load_psw_mask(PSW_KERNEL_BITS | PSW_MASK_DAT);
>>>>>>> refs/remotes/origin/master
	cpu_init();
	preempt_disable();
	init_cpu_timer();
	init_cpu_vtimer();
	pfault_init();
	notify_cpu_starting(smp_processor_id());
<<<<<<< HEAD
	ipi_call_lock();
	set_cpu_online(smp_processor_id(), true);
	ipi_call_unlock();
	local_irq_enable();
	/* cpu_idle will call schedule for us */
	cpu_idle();
>>>>>>> refs/remotes/origin/cm-10.0
}

struct create_idle {
	struct work_struct work;
	struct task_struct *idle;
	struct completion done;
	int cpu;
};

static void __cpuinit smp_fork_idle(struct work_struct *work)
{
	struct create_idle *c_idle;

	c_idle = container_of(work, struct create_idle, work);
	c_idle->idle = fork_idle(c_idle->cpu);
	complete(&c_idle->done);
}

<<<<<<< HEAD
static int __cpuinit smp_alloc_lowcore(int cpu)
{
	unsigned long async_stack, panic_stack;
	struct _lowcore *lowcore;

	lowcore = (void *) __get_free_pages(GFP_KERNEL | GFP_DMA, LC_ORDER);
	if (!lowcore)
		return -ENOMEM;
	async_stack = __get_free_pages(GFP_KERNEL, ASYNC_ORDER);
	panic_stack = __get_free_page(GFP_KERNEL);
	if (!panic_stack || !async_stack)
		goto out;
	memcpy(lowcore, &S390_lowcore, 512);
	memset((char *)lowcore + 512, 0, sizeof(*lowcore) - 512);
	lowcore->async_stack = async_stack + ASYNC_SIZE;
	lowcore->panic_stack = panic_stack + PAGE_SIZE;

#ifndef CONFIG_64BIT
	if (MACHINE_HAS_IEEE) {
		unsigned long save_area;

		save_area = get_zeroed_page(GFP_KERNEL);
		if (!save_area)
			goto out;
		lowcore->extended_save_area_addr = (u32) save_area;
	}
#else
	if (vdso_alloc_per_cpu(cpu, lowcore))
		goto out;
#endif
	lowcore_ptr[cpu] = lowcore;
	return 0;

out:
	free_page(panic_stack);
	free_pages(async_stack, ASYNC_ORDER);
	free_pages((unsigned long) lowcore, LC_ORDER);
	return -ENOMEM;
}

static void smp_free_lowcore(int cpu)
{
	struct _lowcore *lowcore;

	lowcore = lowcore_ptr[cpu];
#ifndef CONFIG_64BIT
	if (MACHINE_HAS_IEEE)
		free_page((unsigned long) lowcore->extended_save_area_addr);
#else
	vdso_free_per_cpu(cpu, lowcore);
#endif
	free_page(lowcore->panic_stack - PAGE_SIZE);
	free_pages(lowcore->async_stack - ASYNC_SIZE, ASYNC_ORDER);
	free_pages((unsigned long) lowcore, LC_ORDER);
	lowcore_ptr[cpu] = NULL;
}

/* Upping and downing of CPUs */
int __cpuinit __cpu_up(unsigned int cpu)
{
	struct _lowcore *cpu_lowcore;
	struct create_idle c_idle;
	struct task_struct *idle;
	struct stack_frame *sf;
	u32 lowcore;
	int ccode;

	if (smp_cpu_state[cpu] != CPU_STATE_CONFIGURED)
		return -EIO;
	idle = current_set[cpu];
	if (!idle) {
=======
/* Upping and downing of CPUs */
int __cpuinit __cpu_up(unsigned int cpu)
{
	struct create_idle c_idle;
=======
	set_cpu_online(smp_processor_id(), true);
	inc_irq_stat(CPU_RST);
	local_irq_enable();
	cpu_startup_entry(CPUHP_ONLINE);
}

/* Upping and downing of CPUs */
int __cpu_up(unsigned int cpu, struct task_struct *tidle)
{
>>>>>>> refs/remotes/origin/master
	struct pcpu *pcpu;
	int rc;

	pcpu = pcpu_devices + cpu;
	if (pcpu->state != CPU_STATE_CONFIGURED)
		return -EIO;
<<<<<<< HEAD
	if (pcpu_sigp_retry(pcpu, sigp_initial_cpu_reset, 0) !=
	    sigp_order_code_accepted)
		return -EIO;
	if (!pcpu->idle) {
>>>>>>> refs/remotes/origin/cm-10.0
		c_idle.done = COMPLETION_INITIALIZER_ONSTACK(c_idle.done);
		INIT_WORK_ONSTACK(&c_idle.work, smp_fork_idle);
		c_idle.cpu = cpu;
		schedule_work(&c_idle.work);
		wait_for_completion(&c_idle.done);
		if (IS_ERR(c_idle.idle))
			return PTR_ERR(c_idle.idle);
<<<<<<< HEAD
		idle = c_idle.idle;
		current_set[cpu] = c_idle.idle;
	}
	init_idle(idle, cpu);
	if (smp_alloc_lowcore(cpu))
		return -ENOMEM;
	do {
		ccode = sigp(cpu, sigp_initial_cpu_reset);
		if (ccode == sigp_busy)
			udelay(10);
		if (ccode == sigp_not_operational)
			goto err_out;
	} while (ccode == sigp_busy);

	lowcore = (u32)(unsigned long)lowcore_ptr[cpu];
	while (sigp_p(lowcore, cpu, sigp_set_prefix) == sigp_busy)
		udelay(10);

	cpu_lowcore = lowcore_ptr[cpu];
	cpu_lowcore->kernel_stack = (unsigned long)
		task_stack_page(idle) + THREAD_SIZE;
	cpu_lowcore->thread_info = (unsigned long) task_thread_info(idle);
	sf = (struct stack_frame *) (cpu_lowcore->kernel_stack
				     - sizeof(struct pt_regs)
				     - sizeof(struct stack_frame));
	memset(sf, 0, sizeof(struct stack_frame));
	sf->gprs[9] = (unsigned long) sf;
	cpu_lowcore->save_area[15] = (unsigned long) sf;
	__ctl_store(cpu_lowcore->cregs_save_area, 0, 15);
	atomic_inc(&init_mm.context.attach_count);
	asm volatile(
		"	stam	0,15,0(%0)"
		: : "a" (&cpu_lowcore->access_regs_save_area) : "memory");
	cpu_lowcore->percpu_offset = __per_cpu_offset[cpu];
	cpu_lowcore->current_task = (unsigned long) idle;
	cpu_lowcore->cpu_nr = cpu;
	cpu_lowcore->kernel_asce = S390_lowcore.kernel_asce;
	cpu_lowcore->machine_flags = S390_lowcore.machine_flags;
	cpu_lowcore->ftrace_func = S390_lowcore.ftrace_func;
	memcpy(cpu_lowcore->stfle_fac_list, S390_lowcore.stfle_fac_list,
	       MAX_FACILITY_BIT/8);
	eieio();

	while (sigp(cpu, sigp_restart) == sigp_busy)
		udelay(10);

	while (!cpu_online(cpu))
		cpu_relax();
	return 0;

err_out:
	smp_free_lowcore(cpu);
	return -EIO;
=======
		pcpu->idle = c_idle.idle;
	}
	init_idle(pcpu->idle, cpu);
=======
	if (pcpu_sigp_retry(pcpu, SIGP_INITIAL_CPU_RESET, 0) !=
	    SIGP_CC_ORDER_CODE_ACCEPTED)
		return -EIO;

>>>>>>> refs/remotes/origin/master
	rc = pcpu_alloc_lowcore(pcpu, cpu);
	if (rc)
		return rc;
	pcpu_prepare_secondary(pcpu, cpu);
<<<<<<< HEAD
	pcpu_attach_task(pcpu, pcpu->idle);
=======
	pcpu_attach_task(pcpu, tidle);
>>>>>>> refs/remotes/origin/master
	pcpu_start_fn(pcpu, smp_start_secondary, NULL);
	while (!cpu_online(cpu))
		cpu_relax();
	return 0;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
}

static int __init setup_possible_cpus(char *s)
{
<<<<<<< HEAD
	int pcpus, cpu;

	pcpus = simple_strtoul(s, NULL, 0);
	init_cpu_possible(cpumask_of(0));
	for (cpu = 1; cpu < pcpus && cpu < nr_cpu_ids; cpu++)
=======
	int max, cpu;

	if (kstrtoint(s, 0, &max) < 0)
		return 0;
	init_cpu_possible(cpumask_of(0));
	for (cpu = 1; cpu < max && cpu < nr_cpu_ids; cpu++)
>>>>>>> refs/remotes/origin/cm-10.0
		set_cpu_possible(cpu, true);
	return 0;
}
early_param("possible_cpus", setup_possible_cpus);
=======
}

static unsigned int setup_possible_cpus __initdata;

static int __init _setup_possible_cpus(char *s)
{
	get_option(&s, &setup_possible_cpus);
	return 0;
}
early_param("possible_cpus", _setup_possible_cpus);
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_HOTPLUG_CPU

int __cpu_disable(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct ec_creg_mask_parms cr_parms;
	int cpu = smp_processor_id();

	set_cpu_online(cpu, false);

	/* Disable pfault pseudo page faults on this cpu. */
	pfault_fini();

	memset(&cr_parms.orvals, 0, sizeof(cr_parms.orvals));
	memset(&cr_parms.andvals, 0xff, sizeof(cr_parms.andvals));

	/* disable all external interrupts */
	cr_parms.orvals[0] = 0;
	cr_parms.andvals[0] = ~(1 << 15 | 1 << 14 | 1 << 13 | 1 << 11 |
				1 << 10 | 1 <<	9 | 1 <<  6 | 1 <<  4);
	/* disable all I/O interrupts */
	cr_parms.orvals[6] = 0;
	cr_parms.andvals[6] = ~(1 << 31 | 1 << 30 | 1 << 29 | 1 << 28 |
				1 << 27 | 1 << 26 | 1 << 25 | 1 << 24);
	/* disable most machine checks */
	cr_parms.orvals[14] = 0;
	cr_parms.andvals[14] = ~(1 << 28 | 1 << 27 | 1 << 26 |
				 1 << 25 | 1 << 24);

	smp_ctl_bit_callback(&cr_parms);

=======
	unsigned long cregs[16];

=======
	unsigned long cregs[16];

	/* Handle possible pending IPIs */
	smp_handle_ext_call();
>>>>>>> refs/remotes/origin/master
	set_cpu_online(smp_processor_id(), false);
	/* Disable pseudo page faults on this cpu. */
	pfault_fini();
	/* Disable interrupt sources via control register. */
	__ctl_store(cregs, 0, 15);
	cregs[0]  &= ~0x0000ee70UL;	/* disable all external interrupts */
	cregs[6]  &= ~0xff000000UL;	/* disable all I/O interrupts */
	cregs[14] &= ~0x1f000000UL;	/* disable most machine checks */
	__ctl_load(cregs, 0, 15);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return 0;
}

void __cpu_die(unsigned int cpu)
{
<<<<<<< HEAD
<<<<<<< HEAD
	/* Wait until target cpu is down */
	while (!cpu_stopped(cpu))
		cpu_relax();
	while (sigp_p(0, cpu, sigp_set_prefix) == sigp_busy)
		udelay(10);
	smp_free_lowcore(cpu);
=======
=======
>>>>>>> refs/remotes/origin/master
	struct pcpu *pcpu;

	/* Wait until target cpu is down */
	pcpu = pcpu_devices + cpu;
	while (!pcpu_stopped(pcpu))
		cpu_relax();
	pcpu_free_lowcore(pcpu);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	atomic_dec(&init_mm.context.attach_count);
}

void __noreturn cpu_die(void)
{
	idle_task_exit();
<<<<<<< HEAD
<<<<<<< HEAD
	while (sigp(smp_processor_id(), sigp_stop) == sigp_busy)
		cpu_relax();
	for (;;);
=======
	pcpu_sigp_retry(pcpu_devices + smp_processor_id(), sigp_stop, 0);
	for (;;) ;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pcpu_sigp_retry(pcpu_devices + smp_processor_id(), SIGP_STOP, 0);
	for (;;) ;
>>>>>>> refs/remotes/origin/master
}

#endif /* CONFIG_HOTPLUG_CPU */

<<<<<<< HEAD
<<<<<<< HEAD
void __init smp_prepare_cpus(unsigned int max_cpus)
{
#ifndef CONFIG_64BIT
	unsigned long save_area = 0;
#endif
	unsigned long async_stack, panic_stack;
	struct _lowcore *lowcore;

	smp_detect_cpus();

	/* request the 0x1201 emergency signal external interrupt */
	if (register_external_interrupt(0x1201, do_ext_call_interrupt) != 0)
		panic("Couldn't request external interrupt 0x1201");

	/* Reallocate current lowcore, but keep its contents. */
	lowcore = (void *) __get_free_pages(GFP_KERNEL | GFP_DMA, LC_ORDER);
	panic_stack = __get_free_page(GFP_KERNEL);
	async_stack = __get_free_pages(GFP_KERNEL, ASYNC_ORDER);
	BUG_ON(!lowcore || !panic_stack || !async_stack);
#ifndef CONFIG_64BIT
	if (MACHINE_HAS_IEEE)
		save_area = get_zeroed_page(GFP_KERNEL);
#endif
	local_irq_disable();
	local_mcck_disable();
	lowcore_ptr[smp_processor_id()] = lowcore;
	*lowcore = S390_lowcore;
	lowcore->panic_stack = panic_stack + PAGE_SIZE;
	lowcore->async_stack = async_stack + ASYNC_SIZE;
#ifndef CONFIG_64BIT
	if (MACHINE_HAS_IEEE)
		lowcore->extended_save_area_addr = (u32) save_area;
#endif
	set_prefix((u32)(unsigned long) lowcore);
	local_mcck_enable();
	local_irq_enable();
#ifdef CONFIG_64BIT
	if (vdso_alloc_per_cpu(smp_processor_id(), &S390_lowcore))
		BUG();
#endif
=======
static void smp_call_os_info_init_fn(void)
{
	int (*init_fn)(void);
	unsigned long size;

	init_fn = os_info_old_entry(OS_INFO_INIT_FN, &size);
	if (!init_fn)
		return;
	init_fn();
=======
void __init smp_fill_possible_mask(void)
{
	unsigned int possible, cpu;

	possible = setup_possible_cpus;
	if (!possible)
		possible = MACHINE_IS_VM ? 64 : nr_cpu_ids;
	for (cpu = 0; cpu < possible && cpu < nr_cpu_ids; cpu++)
		set_cpu_possible(cpu, true);
>>>>>>> refs/remotes/origin/master
}

void __init smp_prepare_cpus(unsigned int max_cpus)
{
	/* request the 0x1201 emergency signal external interrupt */
	if (register_external_interrupt(0x1201, do_ext_call_interrupt) != 0)
		panic("Couldn't request external interrupt 0x1201");
	/* request the 0x1202 external call external interrupt */
	if (register_external_interrupt(0x1202, do_ext_call_interrupt) != 0)
		panic("Couldn't request external interrupt 0x1202");
<<<<<<< HEAD
	smp_call_os_info_init_fn();
	smp_detect_cpus();
>>>>>>> refs/remotes/origin/cm-10.0
=======
	smp_detect_cpus();
>>>>>>> refs/remotes/origin/master
}

void __init smp_prepare_boot_cpu(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	BUG_ON(smp_processor_id() != 0);

	current_thread_info()->cpu = 0;
	set_cpu_present(0, true);
	set_cpu_online(0, true);
	S390_lowcore.percpu_offset = __per_cpu_offset[0];
	current_set[0] = current;
	smp_cpu_state[0] = CPU_STATE_CONFIGURED;
	smp_cpu_polarization[0] = POLARIZATION_UNKNWN;
=======
	struct pcpu *pcpu = pcpu_devices;

	boot_cpu_address = stap();
	pcpu->idle = current;
	pcpu->state = CPU_STATE_CONFIGURED;
	pcpu->address = boot_cpu_address;
	pcpu->lowcore = (struct _lowcore *)(unsigned long) store_prefix();
	pcpu->async_stack = S390_lowcore.async_stack - ASYNC_SIZE;
	pcpu->panic_stack = S390_lowcore.panic_stack - PAGE_SIZE;
	S390_lowcore.percpu_offset = __per_cpu_offset[0];
	cpu_set_polarization(0, POLARIZATION_UNKNOWN);
	set_cpu_present(0, true);
	set_cpu_online(0, true);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct pcpu *pcpu = pcpu_devices;

	boot_cpu_address = stap();
	pcpu->state = CPU_STATE_CONFIGURED;
	pcpu->address = boot_cpu_address;
	pcpu->lowcore = (struct _lowcore *)(unsigned long) store_prefix();
	pcpu->async_stack = S390_lowcore.async_stack - ASYNC_SIZE
		+ STACK_FRAME_OVERHEAD + sizeof(struct pt_regs);
	pcpu->panic_stack = S390_lowcore.panic_stack - PAGE_SIZE
		+ STACK_FRAME_OVERHEAD + sizeof(struct pt_regs);
	S390_lowcore.percpu_offset = __per_cpu_offset[0];
	smp_cpu_set_polarization(0, POLARIZATION_UNKNOWN);
	set_cpu_present(0, true);
	set_cpu_online(0, true);
>>>>>>> refs/remotes/origin/master
}

void __init smp_cpus_done(unsigned int max_cpus)
{
}

void __init smp_setup_processor_id(void)
{
	S390_lowcore.cpu_nr = 0;
<<<<<<< HEAD
<<<<<<< HEAD
	__cpu_logical_map[0] = stap();
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

/*
 * the frequency of the profiling timer can be changed
 * by writing a multiplier value into /proc/profile.
 *
 * usually you want to run this on all CPUs ;)
 */
int setup_profiling_timer(unsigned int multiplier)
{
	return 0;
}

#ifdef CONFIG_HOTPLUG_CPU
<<<<<<< HEAD
<<<<<<< HEAD
static ssize_t cpu_configure_show(struct sys_device *dev,
				struct sysdev_attribute *attr, char *buf)
=======
static ssize_t cpu_configure_show(struct device *dev,
				  struct device_attribute *attr, char *buf)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static ssize_t cpu_configure_show(struct device *dev,
				  struct device_attribute *attr, char *buf)
>>>>>>> refs/remotes/origin/master
{
	ssize_t count;

	mutex_lock(&smp_cpu_state_mutex);
<<<<<<< HEAD
<<<<<<< HEAD
	count = sprintf(buf, "%d\n", smp_cpu_state[dev->id]);
=======
	count = sprintf(buf, "%d\n", pcpu_devices[dev->id].state);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	count = sprintf(buf, "%d\n", pcpu_devices[dev->id].state);
>>>>>>> refs/remotes/origin/master
	mutex_unlock(&smp_cpu_state_mutex);
	return count;
}

<<<<<<< HEAD
<<<<<<< HEAD
static ssize_t cpu_configure_store(struct sys_device *dev,
				  struct sysdev_attribute *attr,
				  const char *buf, size_t count)
{
	int cpu = dev->id;
	int val, rc;
=======
=======
>>>>>>> refs/remotes/origin/master
static ssize_t cpu_configure_store(struct device *dev,
				   struct device_attribute *attr,
				   const char *buf, size_t count)
{
	struct pcpu *pcpu;
	int cpu, val, rc;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	char delim;

	if (sscanf(buf, "%d %c", &val, &delim) != 1)
		return -EINVAL;
	if (val != 0 && val != 1)
		return -EINVAL;
<<<<<<< HEAD
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	get_online_cpus();
	mutex_lock(&smp_cpu_state_mutex);
	rc = -EBUSY;
	/* disallow configuration changes of online cpus and cpu 0 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (cpu_online(cpu) || cpu == 0)
		goto out;
	rc = 0;
	switch (val) {
	case 0:
		if (smp_cpu_state[cpu] == CPU_STATE_CONFIGURED) {
			rc = sclp_cpu_deconfigure(__cpu_logical_map[cpu]);
			if (!rc) {
				smp_cpu_state[cpu] = CPU_STATE_STANDBY;
				smp_cpu_polarization[cpu] = POLARIZATION_UNKNWN;
			}
		}
		break;
	case 1:
		if (smp_cpu_state[cpu] == CPU_STATE_STANDBY) {
			rc = sclp_cpu_configure(__cpu_logical_map[cpu]);
			if (!rc) {
				smp_cpu_state[cpu] = CPU_STATE_CONFIGURED;
				smp_cpu_polarization[cpu] = POLARIZATION_UNKNWN;
			}
		}
=======
=======
>>>>>>> refs/remotes/origin/master
	cpu = dev->id;
	if (cpu_online(cpu) || cpu == 0)
		goto out;
	pcpu = pcpu_devices + cpu;
	rc = 0;
	switch (val) {
	case 0:
		if (pcpu->state != CPU_STATE_CONFIGURED)
			break;
		rc = sclp_cpu_deconfigure(pcpu->address);
		if (rc)
			break;
		pcpu->state = CPU_STATE_STANDBY;
<<<<<<< HEAD
		cpu_set_polarization(cpu, POLARIZATION_UNKNOWN);
=======
		smp_cpu_set_polarization(cpu, POLARIZATION_UNKNOWN);
>>>>>>> refs/remotes/origin/master
		topology_expect_change();
		break;
	case 1:
		if (pcpu->state != CPU_STATE_STANDBY)
			break;
		rc = sclp_cpu_configure(pcpu->address);
		if (rc)
			break;
		pcpu->state = CPU_STATE_CONFIGURED;
<<<<<<< HEAD
		cpu_set_polarization(cpu, POLARIZATION_UNKNOWN);
		topology_expect_change();
>>>>>>> refs/remotes/origin/cm-10.0
=======
		smp_cpu_set_polarization(cpu, POLARIZATION_UNKNOWN);
		topology_expect_change();
>>>>>>> refs/remotes/origin/master
		break;
	default:
		break;
	}
out:
	mutex_unlock(&smp_cpu_state_mutex);
	put_online_cpus();
	return rc ? rc : count;
}
<<<<<<< HEAD
<<<<<<< HEAD
static SYSDEV_ATTR(configure, 0644, cpu_configure_show, cpu_configure_store);
#endif /* CONFIG_HOTPLUG_CPU */

static ssize_t cpu_polarization_show(struct sys_device *dev,
				     struct sysdev_attribute *attr, char *buf)
{
	int cpu = dev->id;
	ssize_t count;

	mutex_lock(&smp_cpu_state_mutex);
	switch (smp_cpu_polarization[cpu]) {
	case POLARIZATION_HRZ:
		count = sprintf(buf, "horizontal\n");
		break;
	case POLARIZATION_VL:
		count = sprintf(buf, "vertical:low\n");
		break;
	case POLARIZATION_VM:
		count = sprintf(buf, "vertical:medium\n");
		break;
	case POLARIZATION_VH:
		count = sprintf(buf, "vertical:high\n");
		break;
	default:
		count = sprintf(buf, "unknown\n");
		break;
	}
	mutex_unlock(&smp_cpu_state_mutex);
	return count;
}
static SYSDEV_ATTR(polarization, 0444, cpu_polarization_show, NULL);

static ssize_t show_cpu_address(struct sys_device *dev,
				struct sysdev_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", __cpu_logical_map[dev->id]);
}
static SYSDEV_ATTR(address, 0444, show_cpu_address, NULL);


static struct attribute *cpu_common_attrs[] = {
#ifdef CONFIG_HOTPLUG_CPU
	&attr_configure.attr,
#endif
	&attr_address.attr,
	&attr_polarization.attr,
=======
=======
>>>>>>> refs/remotes/origin/master
static DEVICE_ATTR(configure, 0644, cpu_configure_show, cpu_configure_store);
#endif /* CONFIG_HOTPLUG_CPU */

static ssize_t show_cpu_address(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", pcpu_devices[dev->id].address);
}
static DEVICE_ATTR(address, 0444, show_cpu_address, NULL);

static struct attribute *cpu_common_attrs[] = {
#ifdef CONFIG_HOTPLUG_CPU
	&dev_attr_configure.attr,
#endif
	&dev_attr_address.attr,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	NULL,
};

static struct attribute_group cpu_common_attr_group = {
	.attrs = cpu_common_attrs,
};

<<<<<<< HEAD
<<<<<<< HEAD
static ssize_t show_capability(struct sys_device *dev,
				struct sysdev_attribute *attr, char *buf)
=======
static ssize_t show_capability(struct device *dev,
				struct device_attribute *attr, char *buf)
>>>>>>> refs/remotes/origin/cm-10.0
{
	unsigned int capability;
	int rc;

	rc = get_cpu_capability(&capability);
	if (rc)
		return rc;
	return sprintf(buf, "%u\n", capability);
}
<<<<<<< HEAD
static SYSDEV_ATTR(capability, 0444, show_capability, NULL);

static ssize_t show_idle_count(struct sys_device *dev,
				struct sysdev_attribute *attr, char *buf)
{
	struct s390_idle_data *idle;
	unsigned long long idle_count;
	unsigned int sequence;

	idle = &per_cpu(s390_idle, dev->id);
repeat:
	sequence = idle->sequence;
	smp_rmb();
	if (sequence & 1)
		goto repeat;
	idle_count = idle->idle_count;
	if (idle->idle_enter)
		idle_count++;
	smp_rmb();
	if (idle->sequence != sequence)
		goto repeat;
	return sprintf(buf, "%llu\n", idle_count);
}
static SYSDEV_ATTR(idle_count, 0444, show_idle_count, NULL);

static ssize_t show_idle_time(struct sys_device *dev,
				struct sysdev_attribute *attr, char *buf)
{
	struct s390_idle_data *idle;
	unsigned long long now, idle_time, idle_enter;
	unsigned int sequence;

	idle = &per_cpu(s390_idle, dev->id);
	now = get_clock();
repeat:
	sequence = idle->sequence;
	smp_rmb();
	if (sequence & 1)
		goto repeat;
	idle_time = idle->idle_time;
	idle_enter = idle->idle_enter;
	if (idle_enter != 0ULL && idle_enter < now)
		idle_time += now - idle_enter;
	smp_rmb();
	if (idle->sequence != sequence)
		goto repeat;
	return sprintf(buf, "%llu\n", idle_time >> 12);
}
static SYSDEV_ATTR(idle_time_us, 0444, show_idle_time, NULL);

static struct attribute *cpu_online_attrs[] = {
	&attr_capability.attr,
	&attr_idle_count.attr,
	&attr_idle_time_us.attr,
=======
static DEVICE_ATTR(capability, 0444, show_capability, NULL);

=======
>>>>>>> refs/remotes/origin/master
static ssize_t show_idle_count(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct s390_idle_data *idle = &per_cpu(s390_idle, dev->id);
	unsigned long long idle_count;
	unsigned int sequence;

	do {
		sequence = ACCESS_ONCE(idle->sequence);
		idle_count = ACCESS_ONCE(idle->idle_count);
<<<<<<< HEAD
		if (ACCESS_ONCE(idle->idle_enter))
			idle_count++;
	} while ((sequence & 1) || (idle->sequence != sequence));
=======
		if (ACCESS_ONCE(idle->clock_idle_enter))
			idle_count++;
	} while ((sequence & 1) || (ACCESS_ONCE(idle->sequence) != sequence));
>>>>>>> refs/remotes/origin/master
	return sprintf(buf, "%llu\n", idle_count);
}
static DEVICE_ATTR(idle_count, 0444, show_idle_count, NULL);

static ssize_t show_idle_time(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct s390_idle_data *idle = &per_cpu(s390_idle, dev->id);
	unsigned long long now, idle_time, idle_enter, idle_exit;
	unsigned int sequence;

	do {
<<<<<<< HEAD
		now = get_clock();
		sequence = ACCESS_ONCE(idle->sequence);
		idle_time = ACCESS_ONCE(idle->idle_time);
		idle_enter = ACCESS_ONCE(idle->idle_enter);
		idle_exit = ACCESS_ONCE(idle->idle_exit);
	} while ((sequence & 1) || (idle->sequence != sequence));
=======
		now = get_tod_clock();
		sequence = ACCESS_ONCE(idle->sequence);
		idle_time = ACCESS_ONCE(idle->idle_time);
		idle_enter = ACCESS_ONCE(idle->clock_idle_enter);
		idle_exit = ACCESS_ONCE(idle->clock_idle_exit);
	} while ((sequence & 1) || (ACCESS_ONCE(idle->sequence) != sequence));
>>>>>>> refs/remotes/origin/master
	idle_time += idle_enter ? ((idle_exit ? : now) - idle_enter) : 0;
	return sprintf(buf, "%llu\n", idle_time >> 12);
}
static DEVICE_ATTR(idle_time_us, 0444, show_idle_time, NULL);

static struct attribute *cpu_online_attrs[] = {
<<<<<<< HEAD
	&dev_attr_capability.attr,
	&dev_attr_idle_count.attr,
	&dev_attr_idle_time_us.attr,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	&dev_attr_idle_count.attr,
	&dev_attr_idle_time_us.attr,
>>>>>>> refs/remotes/origin/master
	NULL,
};

static struct attribute_group cpu_online_attr_group = {
	.attrs = cpu_online_attrs,
};

<<<<<<< HEAD
static int __cpuinit smp_cpu_notify(struct notifier_block *self,
				    unsigned long action, void *hcpu)
{
	unsigned int cpu = (unsigned int)(long)hcpu;
<<<<<<< HEAD
	struct cpu *c = &per_cpu(cpu_devices, cpu);
	struct sys_device *s = &c->sysdev;
	struct s390_idle_data *idle;
=======
	struct cpu *c = &pcpu_devices[cpu].cpu;
	struct device *s = &c->dev;
>>>>>>> refs/remotes/origin/cm-10.0
	int err = 0;

	switch (action) {
	case CPU_ONLINE:
	case CPU_ONLINE_FROZEN:
<<<<<<< HEAD
		idle = &per_cpu(s390_idle, cpu);
		memset(idle, 0, sizeof(struct s390_idle_data));
=======
>>>>>>> refs/remotes/origin/cm-10.0
		err = sysfs_create_group(&s->kobj, &cpu_online_attr_group);
		break;
	case CPU_DEAD:
	case CPU_DEAD_FROZEN:
=======
static int smp_cpu_notify(struct notifier_block *self, unsigned long action,
			  void *hcpu)
{
	unsigned int cpu = (unsigned int)(long)hcpu;
	struct cpu *c = pcpu_devices[cpu].cpu;
	struct device *s = &c->dev;
	int err = 0;

	switch (action & ~CPU_TASKS_FROZEN) {
	case CPU_ONLINE:
		err = sysfs_create_group(&s->kobj, &cpu_online_attr_group);
		break;
	case CPU_DEAD:
>>>>>>> refs/remotes/origin/master
		sysfs_remove_group(&s->kobj, &cpu_online_attr_group);
		break;
	}
	return notifier_from_errno(err);
}

<<<<<<< HEAD
static struct notifier_block __cpuinitdata smp_cpu_nb = {
	.notifier_call = smp_cpu_notify,
};

static int __devinit smp_add_present_cpu(int cpu)
{
<<<<<<< HEAD
	struct cpu *c = &per_cpu(cpu_devices, cpu);
	struct sys_device *s = &c->sysdev;
=======
	struct cpu *c = &pcpu_devices[cpu].cpu;
	struct device *s = &c->dev;
>>>>>>> refs/remotes/origin/cm-10.0
	int rc;

=======
static int smp_add_present_cpu(int cpu)
{
	struct device *s;
	struct cpu *c;
	int rc;

	c = kzalloc(sizeof(*c), GFP_KERNEL);
	if (!c)
		return -ENOMEM;
	pcpu_devices[cpu].cpu = c;
	s = &c->dev;
>>>>>>> refs/remotes/origin/master
	c->hotpluggable = 1;
	rc = register_cpu(c, cpu);
	if (rc)
		goto out;
	rc = sysfs_create_group(&s->kobj, &cpu_common_attr_group);
	if (rc)
		goto out_cpu;
<<<<<<< HEAD
<<<<<<< HEAD
	if (!cpu_online(cpu))
		goto out;
	rc = sysfs_create_group(&s->kobj, &cpu_online_attr_group);
	if (!rc)
		return 0;
=======
=======
>>>>>>> refs/remotes/origin/master
	if (cpu_online(cpu)) {
		rc = sysfs_create_group(&s->kobj, &cpu_online_attr_group);
		if (rc)
			goto out_online;
	}
	rc = topology_cpu_init(c);
	if (rc)
		goto out_topology;
	return 0;

out_topology:
	if (cpu_online(cpu))
		sysfs_remove_group(&s->kobj, &cpu_online_attr_group);
out_online:
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	sysfs_remove_group(&s->kobj, &cpu_common_attr_group);
out_cpu:
#ifdef CONFIG_HOTPLUG_CPU
	unregister_cpu(c);
#endif
out:
	return rc;
}

#ifdef CONFIG_HOTPLUG_CPU

int __ref smp_rescan_cpus(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	cpumask_t newcpus;
	int cpu;
	int rc;

	get_online_cpus();
	mutex_lock(&smp_cpu_state_mutex);
	cpumask_copy(&newcpus, cpu_present_mask);
	rc = __smp_rescan_cpus();
	if (rc)
		goto out;
	cpumask_andnot(&newcpus, cpu_present_mask, &newcpus);
	for_each_cpu(cpu, &newcpus) {
		rc = smp_add_present_cpu(cpu);
		if (rc)
			set_cpu_present(cpu, false);
	}
	rc = 0;
out:
	mutex_unlock(&smp_cpu_state_mutex);
	put_online_cpus();
	if (!cpumask_empty(&newcpus))
		topology_schedule_update();
	return rc;
}

static ssize_t __ref rescan_store(struct sysdev_class *class,
				  struct sysdev_class_attribute *attr,
=======
=======
>>>>>>> refs/remotes/origin/master
	struct sclp_cpu_info *info;
	int nr;

	info = smp_get_cpu_info();
	if (!info)
		return -ENOMEM;
	get_online_cpus();
	mutex_lock(&smp_cpu_state_mutex);
	nr = __smp_rescan_cpus(info, 1);
	mutex_unlock(&smp_cpu_state_mutex);
	put_online_cpus();
	kfree(info);
	if (nr)
		topology_schedule_update();
	return 0;
}

static ssize_t __ref rescan_store(struct device *dev,
				  struct device_attribute *attr,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				  const char *buf,
				  size_t count)
{
	int rc;

	rc = smp_rescan_cpus();
	return rc ? rc : count;
}
<<<<<<< HEAD
<<<<<<< HEAD
static SYSDEV_CLASS_ATTR(rescan, 0200, NULL, rescan_store);
#endif /* CONFIG_HOTPLUG_CPU */

static ssize_t dispatching_show(struct sysdev_class *class,
				struct sysdev_class_attribute *attr,
				char *buf)
{
	ssize_t count;

	mutex_lock(&smp_cpu_state_mutex);
	count = sprintf(buf, "%d\n", cpu_management);
	mutex_unlock(&smp_cpu_state_mutex);
	return count;
}

static ssize_t dispatching_store(struct sysdev_class *dev,
				 struct sysdev_class_attribute *attr,
				 const char *buf,
				 size_t count)
{
	int val, rc;
	char delim;

	if (sscanf(buf, "%d %c", &val, &delim) != 1)
		return -EINVAL;
	if (val != 0 && val != 1)
		return -EINVAL;
	rc = 0;
	get_online_cpus();
	mutex_lock(&smp_cpu_state_mutex);
	if (cpu_management == val)
		goto out;
	rc = topology_set_cpu_management(val);
	if (!rc)
		cpu_management = val;
out:
	mutex_unlock(&smp_cpu_state_mutex);
	put_online_cpus();
	return rc ? rc : count;
}
static SYSDEV_CLASS_ATTR(dispatching, 0644, dispatching_show,
			 dispatching_store);

static int __init topology_init(void)
{
	int cpu;
	int rc;

	register_cpu_notifier(&smp_cpu_nb);

#ifdef CONFIG_HOTPLUG_CPU
	rc = sysdev_class_create_file(&cpu_sysdev_class, &attr_rescan);
	if (rc)
		return rc;
#endif
	rc = sysdev_class_create_file(&cpu_sysdev_class, &attr_dispatching);
	if (rc)
		return rc;
=======
=======
>>>>>>> refs/remotes/origin/master
static DEVICE_ATTR(rescan, 0200, NULL, rescan_store);
#endif /* CONFIG_HOTPLUG_CPU */

static int __init s390_smp_init(void)
{
	int cpu, rc;

<<<<<<< HEAD
	register_cpu_notifier(&smp_cpu_nb);
=======
	hotcpu_notifier(smp_cpu_notify, 0);
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_HOTPLUG_CPU
	rc = device_create_file(cpu_subsys.dev_root, &dev_attr_rescan);
	if (rc)
		return rc;
#endif
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	for_each_present_cpu(cpu) {
		rc = smp_add_present_cpu(cpu);
		if (rc)
			return rc;
	}
	return 0;
}
<<<<<<< HEAD
<<<<<<< HEAD
subsys_initcall(topology_init);
=======
subsys_initcall(s390_smp_init);
>>>>>>> refs/remotes/origin/cm-10.0
=======
subsys_initcall(s390_smp_init);
>>>>>>> refs/remotes/origin/master
