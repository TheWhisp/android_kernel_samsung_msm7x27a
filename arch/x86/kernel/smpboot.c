<<<<<<< HEAD
/*
=======
 /*
>>>>>>> refs/remotes/origin/master
 *	x86 SMP booting functions
 *
 *	(c) 1995 Alan Cox, Building #3 <alan@lxorguk.ukuu.org.uk>
 *	(c) 1998, 1999, 2000, 2009 Ingo Molnar <mingo@redhat.com>
 *	Copyright 2001 Andi Kleen, SuSE Labs.
 *
 *	Much of the core SMP work is based on previous work by Thomas Radke, to
 *	whom a great many thanks are extended.
 *
 *	Thanks to Intel for making available several different Pentium,
 *	Pentium Pro and Pentium-II/Xeon MP machines.
 *	Original development of Linux SMP code supported by Caldera.
 *
 *	This code is released under the GNU General Public License version 2 or
 *	later.
 *
 *	Fixes
 *		Felix Koop	:	NR_CPUS used properly
 *		Jose Renau	:	Handle single CPU case.
 *		Alan Cox	:	By repeated request 8) - Total BogoMIPS report.
 *		Greg Wright	:	Fix for kernel stacks panic.
 *		Erich Boleyn	:	MP v1.4 and additional changes.
 *	Matthias Sattler	:	Changes for 2.1 kernel map.
 *	Michel Lespinasse	:	Changes for 2.1 kernel map.
 *	Michael Chastain	:	Change trampoline.S to gnu as.
 *		Alan Cox	:	Dumb bug: 'B' step PPro's are fine
 *		Ingo Molnar	:	Added APIC timers, based on code
 *					from Jose Renau
 *		Ingo Molnar	:	various cleanups and rewrites
 *		Tigran Aivazian	:	fixed "0.00 in /proc/uptime on SMP" bug.
 *	Maciej W. Rozycki	:	Bits for genuine 82489DX APICs
 *	Andi Kleen		:	Changed for SMP boot into long mode.
 *		Martin J. Bligh	: 	Added support for multi-quad systems
 *		Dave Jones	:	Report invalid combinations of Athlon CPUs.
 *		Rusty Russell	:	Hacked into shape for new "hotplug" boot process.
 *      Andi Kleen              :       Converted to new state machine.
 *	Ashok Raj		: 	CPU hotplug support
 *	Glauber Costa		:	i386 and x86_64 integration
 */

<<<<<<< HEAD
=======
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

>>>>>>> refs/remotes/origin/master
#include <linux/init.h>
#include <linux/smp.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/percpu.h>
#include <linux/bootmem.h>
#include <linux/err.h>
#include <linux/nmi.h>
#include <linux/tboot.h>
#include <linux/stackprotector.h>
#include <linux/gfp.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/cpuidle.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/cpuidle.h>
>>>>>>> refs/remotes/origin/master

#include <asm/acpi.h>
#include <asm/desc.h>
#include <asm/nmi.h>
#include <asm/irq.h>
#include <asm/idle.h>
<<<<<<< HEAD
#include <asm/trampoline.h>
=======
#include <asm/realmode.h>
>>>>>>> refs/remotes/origin/master
#include <asm/cpu.h>
#include <asm/numa.h>
#include <asm/pgtable.h>
#include <asm/tlbflush.h>
#include <asm/mtrr.h>
#include <asm/mwait.h>
#include <asm/apic.h>
#include <asm/io_apic.h>
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <asm/i387.h>
#include <asm/fpu-internal.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <asm/i387.h>
#include <asm/fpu-internal.h>
>>>>>>> refs/remotes/origin/cm-11.0
#include <asm/setup.h>
#include <asm/uv/uv.h>
#include <linux/mc146818rtc.h>

#include <asm/smpboot_hooks.h>
#include <asm/i8259.h>
=======
#include <asm/i387.h>
#include <asm/fpu-internal.h>
#include <asm/setup.h>
#include <asm/uv/uv.h>
#include <linux/mc146818rtc.h>
#include <asm/smpboot_hooks.h>
#include <asm/i8259.h>
#include <asm/realmode.h>
#include <asm/misc.h>
>>>>>>> refs/remotes/origin/master

/* State of each CPU */
DEFINE_PER_CPU(int, cpu_state) = { 0 };

<<<<<<< HEAD
/* Store all idle threads, this can be reused instead of creating
* a new thread. Also avoids complicated thread destroy functionality
* for idle threads.
*/
#ifdef CONFIG_HOTPLUG_CPU
/*
 * Needed only for CONFIG_HOTPLUG_CPU because __cpuinitdata is
 * removed after init for !CONFIG_HOTPLUG_CPU.
 */
static DEFINE_PER_CPU(struct task_struct *, idle_thread_array);
#define get_idle_for_cpu(x)      (per_cpu(idle_thread_array, x))
#define set_idle_for_cpu(x, p)   (per_cpu(idle_thread_array, x) = (p))

/*
 * We need this for trampoline_base protection from concurrent accesses when
 * off- and onlining cores wildly.
 */
static DEFINE_MUTEX(x86_cpu_hotplug_driver_mutex);

void cpu_hotplug_driver_lock(void)
{
        mutex_lock(&x86_cpu_hotplug_driver_mutex);
}

void cpu_hotplug_driver_unlock(void)
{
        mutex_unlock(&x86_cpu_hotplug_driver_mutex);
}

ssize_t arch_cpu_probe(const char *buf, size_t count) { return -1; }
ssize_t arch_cpu_release(const char *buf, size_t count) { return -1; }
#else
static struct task_struct *idle_thread_array[NR_CPUS] __cpuinitdata ;
#define get_idle_for_cpu(x)      (idle_thread_array[(x)])
#define set_idle_for_cpu(x, p)   (idle_thread_array[(x)] = (p))
#endif

=======
>>>>>>> refs/remotes/origin/master
/* Number of siblings per CPU package */
int smp_num_siblings = 1;
EXPORT_SYMBOL(smp_num_siblings);

/* Last level cache ID of each logical CPU */
<<<<<<< HEAD
DEFINE_PER_CPU(u16, cpu_llc_id) = BAD_APICID;

/* representing HT siblings of each logical CPU */
DEFINE_PER_CPU(cpumask_var_t, cpu_sibling_map);
EXPORT_PER_CPU_SYMBOL(cpu_sibling_map);

/* representing HT and core siblings of each logical CPU */
DEFINE_PER_CPU(cpumask_var_t, cpu_core_map);
EXPORT_PER_CPU_SYMBOL(cpu_core_map);

DEFINE_PER_CPU(cpumask_var_t, cpu_llc_shared_map);
=======
DEFINE_PER_CPU_READ_MOSTLY(u16, cpu_llc_id) = BAD_APICID;

/* representing HT siblings of each logical CPU */
DEFINE_PER_CPU_READ_MOSTLY(cpumask_var_t, cpu_sibling_map);
EXPORT_PER_CPU_SYMBOL(cpu_sibling_map);

/* representing HT and core siblings of each logical CPU */
DEFINE_PER_CPU_READ_MOSTLY(cpumask_var_t, cpu_core_map);
EXPORT_PER_CPU_SYMBOL(cpu_core_map);

DEFINE_PER_CPU_READ_MOSTLY(cpumask_var_t, cpu_llc_shared_map);
>>>>>>> refs/remotes/origin/master

/* Per CPU bogomips and other parameters */
DEFINE_PER_CPU_SHARED_ALIGNED(struct cpuinfo_x86, cpu_info);
EXPORT_PER_CPU_SYMBOL(cpu_info);

atomic_t init_deasserted;

/*
<<<<<<< HEAD
 * Report back to the Boot Processor.
 * Running on AP.
 */
static void __cpuinit smp_callin(void)
=======
 * Report back to the Boot Processor during boot time or to the caller processor
 * during CPU online.
 */
static void smp_callin(void)
>>>>>>> refs/remotes/origin/master
{
	int cpuid, phys_id;
	unsigned long timeout;

	/*
	 * If waken up by an INIT in an 82489DX configuration
	 * we may get here before an INIT-deassert IPI reaches
	 * our local APIC.  We have to wait for the IPI or we'll
	 * lock up on an APIC access.
<<<<<<< HEAD
	 */
	if (apic->wait_for_init_deassert)
=======
	 *
	 * Since CPU0 is not wakened up by INIT, it doesn't wait for the IPI.
	 */
	cpuid = smp_processor_id();
	if (apic->wait_for_init_deassert && cpuid != 0)
>>>>>>> refs/remotes/origin/master
		apic->wait_for_init_deassert(&init_deasserted);

	/*
	 * (This works even if the APIC is not enabled.)
	 */
	phys_id = read_apic_id();
<<<<<<< HEAD
	cpuid = smp_processor_id();
=======
>>>>>>> refs/remotes/origin/master
	if (cpumask_test_cpu(cpuid, cpu_callin_mask)) {
		panic("%s: phys CPU#%d, CPU#%d already present??\n", __func__,
					phys_id, cpuid);
	}
	pr_debug("CPU#%d (phys ID: %d) waiting for CALLOUT\n", cpuid, phys_id);

	/*
	 * STARTUP IPIs are fragile beasts as they might sometimes
	 * trigger some glue motherboard logic. Complete APIC bus
	 * silence for 1 second, this overestimates the time the
	 * boot CPU is spending to send the up to 2 STARTUP IPIs
	 * by a factor of two. This should be enough.
	 */

	/*
	 * Waiting 2s total for startup (udelay is not yet working)
	 */
	timeout = jiffies + 2*HZ;
	while (time_before(jiffies, timeout)) {
		/*
		 * Has the boot CPU finished it's STARTUP sequence?
		 */
		if (cpumask_test_cpu(cpuid, cpu_callout_mask))
			break;
		cpu_relax();
	}

	if (!time_before(jiffies, timeout)) {
		panic("%s: CPU%d started up but did not get a callout!\n",
		      __func__, cpuid);
	}

	/*
	 * the boot CPU has finished the init stage and is spinning
	 * on callin_map until we finish. We are free to set up this
	 * CPU, first the APIC. (this is probably redundant on most
	 * boards)
	 */

<<<<<<< HEAD
	pr_debug("CALLIN, before setup_local_APIC().\n");
=======
	pr_debug("CALLIN, before setup_local_APIC()\n");
>>>>>>> refs/remotes/origin/master
	if (apic->smp_callin_clear_local_apic)
		apic->smp_callin_clear_local_apic();
	setup_local_APIC();
	end_local_APIC_setup();

	/*
	 * Need to setup vector mappings before we enable interrupts.
	 */
	setup_vector_irq(smp_processor_id());
<<<<<<< HEAD
<<<<<<< HEAD
	/*
	 * Get our bogomips.
	 *
	 * Need to enable IRQs because it can take longer and then
	 * the NMI watchdog might kill us.
	 */
	local_irq_enable();
	calibrate_delay();
	local_irq_disable();
	pr_debug("Stack at about %p\n", &cpuid);

	/*
	 * Save our processor parameters
	 */
	smp_store_cpu_info(cpuid);
=======
=======
>>>>>>> refs/remotes/origin/master

	/*
	 * Save our processor parameters. Note: this information
	 * is needed for clock calibration.
	 */
	smp_store_cpu_info(cpuid);

	/*
	 * Get our bogomips.
	 * Update loops_per_jiffy in cpu_data. Previous call to
	 * smp_store_cpu_info() stored a value that is close but not as
	 * accurate as the value just calculated.
	 */
	calibrate_delay();
	cpu_data(cpuid).loops_per_jiffy = loops_per_jiffy;
	pr_debug("Stack at about %p\n", &cpuid);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	/*
	 * This must be done before setting cpu_online_mask
	 * or calling notify_cpu_starting.
	 */
	set_cpu_sibling_map(raw_smp_processor_id());
	wmb();

	notify_cpu_starting(cpuid);

	/*
	 * Allow the master to continue.
	 */
	cpumask_set_cpu(cpuid, cpu_callin_mask);
}

<<<<<<< HEAD
/*
 * Activate a secondary processor.
 */
notrace static void __cpuinit start_secondary(void *unused)
=======
static int cpu0_logical_apicid;
static int enable_start_cpu0;
/*
 * Activate a secondary processor.
 */
static void notrace start_secondary(void *unused)
>>>>>>> refs/remotes/origin/master
{
	/*
	 * Don't put *anything* before cpu_init(), SMP booting is too
	 * fragile that we want to limit the things done here to the
	 * most necessary things.
	 */
	cpu_init();
<<<<<<< HEAD
<<<<<<< HEAD
=======
	x86_cpuinit.early_percpu_clock_init();
>>>>>>> refs/remotes/origin/cm-10.0
	preempt_disable();
	smp_callin();

=======
	x86_cpuinit.early_percpu_clock_init();
	preempt_disable();
	smp_callin();

	enable_start_cpu0 = 0;

>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_X86_32
	/* switch away from the initial page table */
	load_cr3(swapper_pg_dir);
	__flush_tlb_all();
#endif

	/* otherwise gcc will move up smp_processor_id before the cpu_init */
	barrier();
	/*
	 * Check TSC synchronization with the BP:
	 */
	check_tsc_sync_target();

	/*
<<<<<<< HEAD
	 * We need to hold call_lock, so there is no inconsistency
	 * between the time smp_call_function() determines number of
	 * IPI recipients, and the time when the determination is made
	 * for which cpus receive the IPI. Holding this
	 * lock helps us to not include this cpu in a currently in progress
	 * smp_call_function().
	 *
=======
>>>>>>> refs/remotes/origin/master
	 * We need to hold vector_lock so there the set of online cpus
	 * does not change while we are assigning vectors to cpus.  Holding
	 * this lock ensures we don't half assign or remove an irq from a cpu.
	 */
<<<<<<< HEAD
	ipi_call_lock();
	lock_vector_lock();
	set_cpu_online(smp_processor_id(), true);
	unlock_vector_lock();
	ipi_call_unlock();
	per_cpu(cpu_state, smp_processor_id()) = CPU_ONLINE;
	x86_platform.nmi_init();

<<<<<<< HEAD
	/*
	 * Wait until the cpu which brought this one up marked it
	 * online before enabling interrupts. If we don't do that then
	 * we can end up waking up the softirq thread before this cpu
	 * reached the active state, which makes the scheduler unhappy
	 * and schedule the softirq thread on the wrong cpu. This is
	 * only observable with forced threaded interrupts, but in
	 * theory it could also happen w/o them. It's just way harder
	 * to achieve.
	 */
	while (!cpumask_test_cpu(smp_processor_id(), cpu_active_mask))
		cpu_relax();

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	lock_vector_lock();
	set_cpu_online(smp_processor_id(), true);
	unlock_vector_lock();
	per_cpu(cpu_state, smp_processor_id()) = CPU_ONLINE;
	x86_platform.nmi_init();

>>>>>>> refs/remotes/origin/master
	/* enable local interrupts */
	local_irq_enable();

	/* to prevent fake stack check failure in clock setup */
	boot_init_stack_canary();

	x86_cpuinit.setup_percpu_clockev();

	wmb();
<<<<<<< HEAD
	cpu_idle();
=======
	cpu_startup_entry(CPUHP_ONLINE);
}

void __init smp_store_boot_cpu_info(void)
{
	int id = 0; /* CPU 0 */
	struct cpuinfo_x86 *c = &cpu_data(id);

	*c = boot_cpu_data;
	c->cpu_index = id;
>>>>>>> refs/remotes/origin/master
}

/*
 * The bootstrap kernel entry code has set these up. Save them for
 * a given CPU
 */
<<<<<<< HEAD

void __cpuinit smp_store_cpu_info(int id)
=======
void smp_store_cpu_info(int id)
>>>>>>> refs/remotes/origin/master
{
	struct cpuinfo_x86 *c = &cpu_data(id);

	*c = boot_cpu_data;
	c->cpu_index = id;
<<<<<<< HEAD
	if (id != 0)
		identify_secondary_cpu(c);
}

static void __cpuinit link_thread_siblings(int cpu1, int cpu2)
{
	cpumask_set_cpu(cpu1, cpu_sibling_mask(cpu2));
	cpumask_set_cpu(cpu2, cpu_sibling_mask(cpu1));
	cpumask_set_cpu(cpu1, cpu_core_mask(cpu2));
	cpumask_set_cpu(cpu2, cpu_core_mask(cpu1));
	cpumask_set_cpu(cpu1, cpu_llc_shared_mask(cpu2));
	cpumask_set_cpu(cpu2, cpu_llc_shared_mask(cpu1));
}


void __cpuinit set_cpu_sibling_map(int cpu)
{
	int i;
	struct cpuinfo_x86 *c = &cpu_data(cpu);

	cpumask_set_cpu(cpu, cpu_sibling_setup_mask);

	if (smp_num_siblings > 1) {
		for_each_cpu(i, cpu_sibling_setup_mask) {
			struct cpuinfo_x86 *o = &cpu_data(i);

			if (cpu_has(c, X86_FEATURE_TOPOEXT)) {
				if (c->phys_proc_id == o->phys_proc_id &&
				    per_cpu(cpu_llc_id, cpu) == per_cpu(cpu_llc_id, i) &&
				    c->compute_unit_id == o->compute_unit_id)
					link_thread_siblings(cpu, i);
			} else if (c->phys_proc_id == o->phys_proc_id &&
				   c->cpu_core_id == o->cpu_core_id) {
				link_thread_siblings(cpu, i);
			}
		}
	} else {
		cpumask_set_cpu(cpu, cpu_sibling_mask(cpu));
	}

	cpumask_set_cpu(cpu, cpu_llc_shared_mask(cpu));

	if (__this_cpu_read(cpu_info.x86_max_cores) == 1) {
		cpumask_copy(cpu_core_mask(cpu), cpu_sibling_mask(cpu));
		c->booted_cores = 1;
		return;
	}

	for_each_cpu(i, cpu_sibling_setup_mask) {
		if (per_cpu(cpu_llc_id, cpu) != BAD_APICID &&
		    per_cpu(cpu_llc_id, cpu) == per_cpu(cpu_llc_id, i)) {
			cpumask_set_cpu(i, cpu_llc_shared_mask(cpu));
			cpumask_set_cpu(cpu, cpu_llc_shared_mask(i));
		}
		if (c->phys_proc_id == cpu_data(i).phys_proc_id) {
			cpumask_set_cpu(i, cpu_core_mask(cpu));
			cpumask_set_cpu(cpu, cpu_core_mask(i));
=======
	/*
	 * During boot time, CPU0 has this setup already. Save the info when
	 * bringing up AP or offlined CPU0.
	 */
	identify_secondary_cpu(c);
}

static bool
topology_sane(struct cpuinfo_x86 *c, struct cpuinfo_x86 *o, const char *name)
{
	int cpu1 = c->cpu_index, cpu2 = o->cpu_index;

	return !WARN_ONCE(cpu_to_node(cpu1) != cpu_to_node(cpu2),
		"sched: CPU #%d's %s-sibling CPU #%d is not on the same node! "
		"[node: %d != %d]. Ignoring dependency.\n",
		cpu1, name, cpu2, cpu_to_node(cpu1), cpu_to_node(cpu2));
}

#define link_mask(_m, c1, c2)						\
do {									\
	cpumask_set_cpu((c1), cpu_##_m##_mask(c2));			\
	cpumask_set_cpu((c2), cpu_##_m##_mask(c1));			\
} while (0)

static bool match_smt(struct cpuinfo_x86 *c, struct cpuinfo_x86 *o)
{
	if (cpu_has_topoext) {
		int cpu1 = c->cpu_index, cpu2 = o->cpu_index;

		if (c->phys_proc_id == o->phys_proc_id &&
		    per_cpu(cpu_llc_id, cpu1) == per_cpu(cpu_llc_id, cpu2) &&
		    c->compute_unit_id == o->compute_unit_id)
			return topology_sane(c, o, "smt");

	} else if (c->phys_proc_id == o->phys_proc_id &&
		   c->cpu_core_id == o->cpu_core_id) {
		return topology_sane(c, o, "smt");
	}

	return false;
}

static bool match_llc(struct cpuinfo_x86 *c, struct cpuinfo_x86 *o)
{
	int cpu1 = c->cpu_index, cpu2 = o->cpu_index;

	if (per_cpu(cpu_llc_id, cpu1) != BAD_APICID &&
	    per_cpu(cpu_llc_id, cpu1) == per_cpu(cpu_llc_id, cpu2))
		return topology_sane(c, o, "llc");

	return false;
}

static bool match_mc(struct cpuinfo_x86 *c, struct cpuinfo_x86 *o)
{
	if (c->phys_proc_id == o->phys_proc_id) {
		if (cpu_has(c, X86_FEATURE_AMD_DCM))
			return true;

		return topology_sane(c, o, "mc");
	}
	return false;
}

void set_cpu_sibling_map(int cpu)
{
	bool has_smt = smp_num_siblings > 1;
	bool has_mp = has_smt || boot_cpu_data.x86_max_cores > 1;
	struct cpuinfo_x86 *c = &cpu_data(cpu);
	struct cpuinfo_x86 *o;
	int i;

	cpumask_set_cpu(cpu, cpu_sibling_setup_mask);

	if (!has_mp) {
		cpumask_set_cpu(cpu, cpu_sibling_mask(cpu));
		cpumask_set_cpu(cpu, cpu_llc_shared_mask(cpu));
		cpumask_set_cpu(cpu, cpu_core_mask(cpu));
		c->booted_cores = 1;
		return;
	}

	for_each_cpu(i, cpu_sibling_setup_mask) {
		o = &cpu_data(i);

		if ((i == cpu) || (has_smt && match_smt(c, o)))
			link_mask(sibling, cpu, i);

		if ((i == cpu) || (has_mp && match_llc(c, o)))
			link_mask(llc_shared, cpu, i);

	}

	/*
	 * This needs a separate iteration over the cpus because we rely on all
	 * cpu_sibling_mask links to be set-up.
	 */
	for_each_cpu(i, cpu_sibling_setup_mask) {
		o = &cpu_data(i);

		if ((i == cpu) || (has_mp && match_mc(c, o))) {
			link_mask(core, cpu, i);

>>>>>>> refs/remotes/origin/master
			/*
			 *  Does this new cpu bringup a new core?
			 */
			if (cpumask_weight(cpu_sibling_mask(cpu)) == 1) {
				/*
				 * for each core in package, increment
				 * the booted_cores for this new cpu
				 */
				if (cpumask_first(cpu_sibling_mask(i)) == i)
					c->booted_cores++;
				/*
				 * increment the core count for all
				 * the other cpus in this package
				 */
				if (i != cpu)
					cpu_data(i).booted_cores++;
			} else if (i != cpu && !c->booted_cores)
				c->booted_cores = cpu_data(i).booted_cores;
		}
	}
}

/* maps the cpu to the sched domain representing multi-core */
const struct cpumask *cpu_coregroup_mask(int cpu)
{
<<<<<<< HEAD
	struct cpuinfo_x86 *c = &cpu_data(cpu);
	/*
	 * For perf, we return last level cache shared map.
	 * And for power savings, we return cpu_core_map
	 */
	if ((sched_mc_power_savings || sched_smt_power_savings) &&
	    !(cpu_has(c, X86_FEATURE_AMD_DCM)))
		return cpu_core_mask(cpu);
	else
		return cpu_llc_shared_mask(cpu);
=======
	return cpu_llc_shared_mask(cpu);
>>>>>>> refs/remotes/origin/master
}

static void impress_friends(void)
{
	int cpu;
	unsigned long bogosum = 0;
	/*
	 * Allow the user to impress friends.
	 */
<<<<<<< HEAD
	pr_debug("Before bogomips.\n");
	for_each_possible_cpu(cpu)
		if (cpumask_test_cpu(cpu, cpu_callout_mask))
			bogosum += cpu_data(cpu).loops_per_jiffy;
	printk(KERN_INFO
		"Total of %d processors activated (%lu.%02lu BogoMIPS).\n",
=======
	pr_debug("Before bogomips\n");
	for_each_possible_cpu(cpu)
		if (cpumask_test_cpu(cpu, cpu_callout_mask))
			bogosum += cpu_data(cpu).loops_per_jiffy;
	pr_info("Total of %d processors activated (%lu.%02lu BogoMIPS)\n",
>>>>>>> refs/remotes/origin/master
		num_online_cpus(),
		bogosum/(500000/HZ),
		(bogosum/(5000/HZ))%100);

<<<<<<< HEAD
	pr_debug("Before bogocount - setting activated=1.\n");
=======
	pr_debug("Before bogocount - setting activated=1\n");
>>>>>>> refs/remotes/origin/master
}

void __inquire_remote_apic(int apicid)
{
	unsigned i, regs[] = { APIC_ID >> 4, APIC_LVR >> 4, APIC_SPIV >> 4 };
<<<<<<< HEAD
<<<<<<< HEAD
	char *names[] = { "ID", "VERSION", "SPIV" };
=======
	const char * const names[] = { "ID", "VERSION", "SPIV" };
>>>>>>> refs/remotes/origin/cm-10.0
	int timeout;
	u32 status;

	printk(KERN_INFO "Inquiring remote APIC 0x%x...\n", apicid);

	for (i = 0; i < ARRAY_SIZE(regs); i++) {
		printk(KERN_INFO "... APIC 0x%x %s: ", apicid, names[i]);
=======
	const char * const names[] = { "ID", "VERSION", "SPIV" };
	int timeout;
	u32 status;

	pr_info("Inquiring remote APIC 0x%x...\n", apicid);

	for (i = 0; i < ARRAY_SIZE(regs); i++) {
		pr_info("... APIC 0x%x %s: ", apicid, names[i]);
>>>>>>> refs/remotes/origin/master

		/*
		 * Wait for idle.
		 */
		status = safe_apic_wait_icr_idle();
		if (status)
<<<<<<< HEAD
			printk(KERN_CONT
			       "a previous APIC delivery may have failed\n");
=======
			pr_cont("a previous APIC delivery may have failed\n");
>>>>>>> refs/remotes/origin/master

		apic_icr_write(APIC_DM_REMRD | regs[i], apicid);

		timeout = 0;
		do {
			udelay(100);
			status = apic_read(APIC_ICR) & APIC_ICR_RR_MASK;
		} while (status == APIC_ICR_RR_INPROG && timeout++ < 1000);

		switch (status) {
		case APIC_ICR_RR_VALID:
			status = apic_read(APIC_RRR);
<<<<<<< HEAD
			printk(KERN_CONT "%08x\n", status);
			break;
		default:
			printk(KERN_CONT "failed\n");
=======
			pr_cont("%08x\n", status);
			break;
		default:
			pr_cont("failed\n");
>>>>>>> refs/remotes/origin/master
		}
	}
}

/*
 * Poke the other CPU in the eye via NMI to wake it up. Remember that the normal
 * INIT, INIT, STARTUP sequence will reset the chip hard for us, and this
 * won't ... remember to clear down the APIC, etc later.
 */
<<<<<<< HEAD
int __cpuinit
wakeup_secondary_cpu_via_nmi(int logical_apicid, unsigned long start_eip)
=======
int
wakeup_secondary_cpu_via_nmi(int apicid, unsigned long start_eip)
>>>>>>> refs/remotes/origin/master
{
	unsigned long send_status, accept_status = 0;
	int maxlvt;

	/* Target chip */
	/* Boot on the stack */
	/* Kick the second */
<<<<<<< HEAD
	apic_icr_write(APIC_DM_NMI | apic->dest_logical, logical_apicid);
=======
	apic_icr_write(APIC_DM_NMI | apic->dest_logical, apicid);
>>>>>>> refs/remotes/origin/master

	pr_debug("Waiting for send to finish...\n");
	send_status = safe_apic_wait_icr_idle();

	/*
	 * Give the other CPU some time to accept the IPI.
	 */
	udelay(200);
	if (APIC_INTEGRATED(apic_version[boot_cpu_physical_apicid])) {
		maxlvt = lapic_get_maxlvt();
		if (maxlvt > 3)			/* Due to the Pentium erratum 3AP.  */
			apic_write(APIC_ESR, 0);
		accept_status = (apic_read(APIC_ESR) & 0xEF);
	}
<<<<<<< HEAD
	pr_debug("NMI sent.\n");

	if (send_status)
		printk(KERN_ERR "APIC never delivered???\n");
	if (accept_status)
		printk(KERN_ERR "APIC delivery error (%lx).\n", accept_status);
=======
	pr_debug("NMI sent\n");

	if (send_status)
		pr_err("APIC never delivered???\n");
	if (accept_status)
		pr_err("APIC delivery error (%lx)\n", accept_status);
>>>>>>> refs/remotes/origin/master

	return (send_status | accept_status);
}

<<<<<<< HEAD
static int __cpuinit
=======
static int
>>>>>>> refs/remotes/origin/master
wakeup_secondary_cpu_via_init(int phys_apicid, unsigned long start_eip)
{
	unsigned long send_status, accept_status = 0;
	int maxlvt, num_starts, j;

	maxlvt = lapic_get_maxlvt();

	/*
	 * Be paranoid about clearing APIC errors.
	 */
	if (APIC_INTEGRATED(apic_version[phys_apicid])) {
		if (maxlvt > 3)		/* Due to the Pentium erratum 3AP.  */
			apic_write(APIC_ESR, 0);
		apic_read(APIC_ESR);
	}

<<<<<<< HEAD
	pr_debug("Asserting INIT.\n");
=======
	pr_debug("Asserting INIT\n");
>>>>>>> refs/remotes/origin/master

	/*
	 * Turn INIT on target chip
	 */
	/*
	 * Send IPI
	 */
	apic_icr_write(APIC_INT_LEVELTRIG | APIC_INT_ASSERT | APIC_DM_INIT,
		       phys_apicid);

	pr_debug("Waiting for send to finish...\n");
	send_status = safe_apic_wait_icr_idle();

	mdelay(10);

<<<<<<< HEAD
	pr_debug("Deasserting INIT.\n");
=======
	pr_debug("Deasserting INIT\n");
>>>>>>> refs/remotes/origin/master

	/* Target chip */
	/* Send IPI */
	apic_icr_write(APIC_INT_LEVELTRIG | APIC_DM_INIT, phys_apicid);

	pr_debug("Waiting for send to finish...\n");
	send_status = safe_apic_wait_icr_idle();

	mb();
	atomic_set(&init_deasserted, 1);

	/*
	 * Should we send STARTUP IPIs ?
	 *
	 * Determine this based on the APIC version.
	 * If we don't have an integrated APIC, don't send the STARTUP IPIs.
	 */
	if (APIC_INTEGRATED(apic_version[phys_apicid]))
		num_starts = 2;
	else
		num_starts = 0;

	/*
	 * Paravirt / VMI wants a startup IPI hook here to set up the
	 * target processor state.
	 */
	startup_ipi_hook(phys_apicid, (unsigned long) start_secondary,
			 stack_start);

	/*
	 * Run STARTUP IPI loop.
	 */
<<<<<<< HEAD
	pr_debug("#startup loops: %d.\n", num_starts);

	for (j = 1; j <= num_starts; j++) {
		pr_debug("Sending STARTUP #%d.\n", j);
		if (maxlvt > 3)		/* Due to the Pentium erratum 3AP.  */
			apic_write(APIC_ESR, 0);
		apic_read(APIC_ESR);
		pr_debug("After apic_write.\n");
=======
	pr_debug("#startup loops: %d\n", num_starts);

	for (j = 1; j <= num_starts; j++) {
		pr_debug("Sending STARTUP #%d\n", j);
		if (maxlvt > 3)		/* Due to the Pentium erratum 3AP.  */
			apic_write(APIC_ESR, 0);
		apic_read(APIC_ESR);
		pr_debug("After apic_write\n");
>>>>>>> refs/remotes/origin/master

		/*
		 * STARTUP IPI
		 */

		/* Target chip */
		/* Boot on the stack */
		/* Kick the second */
		apic_icr_write(APIC_DM_STARTUP | (start_eip >> 12),
			       phys_apicid);

		/*
		 * Give the other CPU some time to accept the IPI.
		 */
		udelay(300);

<<<<<<< HEAD
		pr_debug("Startup point 1.\n");
=======
		pr_debug("Startup point 1\n");
>>>>>>> refs/remotes/origin/master

		pr_debug("Waiting for send to finish...\n");
		send_status = safe_apic_wait_icr_idle();

		/*
		 * Give the other CPU some time to accept the IPI.
		 */
		udelay(200);
		if (maxlvt > 3)		/* Due to the Pentium erratum 3AP.  */
			apic_write(APIC_ESR, 0);
		accept_status = (apic_read(APIC_ESR) & 0xEF);
		if (send_status || accept_status)
			break;
	}
<<<<<<< HEAD
	pr_debug("After Startup.\n");

	if (send_status)
		printk(KERN_ERR "APIC never delivered???\n");
	if (accept_status)
		printk(KERN_ERR "APIC delivery error (%lx).\n", accept_status);
=======
	pr_debug("After Startup\n");

	if (send_status)
		pr_err("APIC never delivered???\n");
	if (accept_status)
		pr_err("APIC delivery error (%lx)\n", accept_status);
>>>>>>> refs/remotes/origin/master

	return (send_status | accept_status);
}

<<<<<<< HEAD
struct create_idle {
	struct work_struct work;
	struct task_struct *idle;
	struct completion done;
	int cpu;
};

static void __cpuinit do_fork_idle(struct work_struct *work)
{
	struct create_idle *c_idle =
		container_of(work, struct create_idle, work);

	c_idle->idle = fork_idle(c_idle->cpu);
	complete(&c_idle->done);
}

/* reduce the number of lines printed when booting a large cpu count system */
static void __cpuinit announce_cpu(int cpu, int apicid)
{
	static int current_node = -1;
	int node = early_cpu_to_node(cpu);
=======
void smp_announce(void)
{
	int num_nodes = num_online_nodes();

	printk(KERN_INFO "x86: Booted up %d node%s, %d CPUs\n",
	       num_nodes, (num_nodes > 1 ? "s" : ""), num_online_cpus());
}

/* reduce the number of lines printed when booting a large cpu count system */
static void announce_cpu(int cpu, int apicid)
{
	static int current_node = -1;
	int node = early_cpu_to_node(cpu);
	static int width, node_width;

	if (!width)
		width = num_digits(num_possible_cpus()) + 1; /* + '#' sign */

	if (!node_width)
		node_width = num_digits(num_possible_nodes()) + 1; /* + '#' */

	if (cpu == 1)
		printk(KERN_INFO "x86: Booting SMP configuration:\n");
>>>>>>> refs/remotes/origin/master

	if (system_state == SYSTEM_BOOTING) {
		if (node != current_node) {
			if (current_node > (-1))
<<<<<<< HEAD
				pr_cont(" Ok.\n");
			current_node = node;
			pr_info("Booting Node %3d, Processors ", node);
		}
		pr_cont(" #%d%s", cpu, cpu == (nr_cpu_ids - 1) ? " Ok.\n" : "");
		return;
=======
				pr_cont("\n");
			current_node = node;

			printk(KERN_INFO ".... node %*s#%d, CPUs:  ",
			       node_width - num_digits(node), " ", node);
		}

		/* Add padding for the BSP */
		if (cpu == 1)
			pr_cont("%*s", width + 1, " ");

		pr_cont("%*s#%d", width - num_digits(cpu), " ", cpu);

>>>>>>> refs/remotes/origin/master
	} else
		pr_info("Booting Node %d Processor %d APIC 0x%x\n",
			node, cpu, apicid);
}

<<<<<<< HEAD
=======
static int wakeup_cpu0_nmi(unsigned int cmd, struct pt_regs *regs)
{
	int cpu;

	cpu = smp_processor_id();
	if (cpu == 0 && !cpu_online(cpu) && enable_start_cpu0)
		return NMI_HANDLED;

	return NMI_DONE;
}

/*
 * Wake up AP by INIT, INIT, STARTUP sequence.
 *
 * Instead of waiting for STARTUP after INITs, BSP will execute the BIOS
 * boot-strap code which is not a desired behavior for waking up BSP. To
 * void the boot-strap code, wake up CPU0 by NMI instead.
 *
 * This works to wake up soft offlined CPU0 only. If CPU0 is hard offlined
 * (i.e. physically hot removed and then hot added), NMI won't wake it up.
 * We'll change this code in the future to wake up hard offlined CPU0 if
 * real platform and request are available.
 */
static int
wakeup_cpu_via_init_nmi(int cpu, unsigned long start_ip, int apicid,
	       int *cpu0_nmi_registered)
{
	int id;
	int boot_error;

	/*
	 * Wake up AP by INIT, INIT, STARTUP sequence.
	 */
	if (cpu)
		return wakeup_secondary_cpu_via_init(apicid, start_ip);

	/*
	 * Wake up BSP by nmi.
	 *
	 * Register a NMI handler to help wake up CPU0.
	 */
	boot_error = register_nmi_handler(NMI_LOCAL,
					  wakeup_cpu0_nmi, 0, "wake_cpu0");

	if (!boot_error) {
		enable_start_cpu0 = 1;
		*cpu0_nmi_registered = 1;
		if (apic->dest_logical == APIC_DEST_LOGICAL)
			id = cpu0_logical_apicid;
		else
			id = apicid;
		boot_error = wakeup_secondary_cpu_via_nmi(id, start_ip);
	}

	return boot_error;
}

>>>>>>> refs/remotes/origin/master
/*
 * NOTE - on most systems this is a PHYSICAL apic ID, but on multiquad
 * (ie clustered apic addressing mode), this is a LOGICAL apic ID.
 * Returns zero if CPU booted OK, else error code from
 * ->wakeup_secondary_cpu.
 */
<<<<<<< HEAD
static int __cpuinit do_boot_cpu(int apicid, int cpu)
{
	unsigned long boot_error = 0;
	unsigned long start_ip;
	int timeout;
	struct create_idle c_idle = {
		.cpu	= cpu,
		.done	= COMPLETION_INITIALIZER_ONSTACK(c_idle.done),
	};

	INIT_WORK_ONSTACK(&c_idle.work, do_fork_idle);

	alternatives_smp_switch(1);

	c_idle.idle = get_idle_for_cpu(cpu);

	/*
	 * We can't use kernel_thread since we must avoid to
	 * reschedule the child.
	 */
	if (c_idle.idle) {
		c_idle.idle->thread.sp = (unsigned long) (((struct pt_regs *)
			(THREAD_SIZE +  task_stack_page(c_idle.idle))) - 1);
		init_idle(c_idle.idle, cpu);
		goto do_rest;
	}

	schedule_work(&c_idle.work);
	wait_for_completion(&c_idle.done);

	if (IS_ERR(c_idle.idle)) {
		printk("failed fork for CPU %d\n", cpu);
		destroy_work_on_stack(&c_idle.work);
		return PTR_ERR(c_idle.idle);
	}

	set_idle_for_cpu(cpu, c_idle.idle);
do_rest:
	per_cpu(current_task, cpu) = c_idle.idle;
=======
static int do_boot_cpu(int apicid, int cpu, struct task_struct *idle)
{
	volatile u32 *trampoline_status =
		(volatile u32 *) __va(real_mode_header->trampoline_status);
	/* start_ip had better be page-aligned! */
	unsigned long start_ip = real_mode_header->trampoline_start;

	unsigned long boot_error = 0;
	int timeout;
	int cpu0_nmi_registered = 0;

	/* Just in case we booted with a single CPU. */
	alternatives_enable_smp();

	idle->thread.sp = (unsigned long) (((struct pt_regs *)
			  (THREAD_SIZE +  task_stack_page(idle))) - 1);
	per_cpu(current_task, cpu) = idle;

>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_X86_32
	/* Stack for startup_32 can be just as for start_secondary onwards */
	irq_ctx_init(cpu);
#else
<<<<<<< HEAD
	clear_tsk_thread_flag(c_idle.idle, TIF_FORK);
	initial_gs = per_cpu_offset(cpu);
	per_cpu(kernel_stack, cpu) =
		(unsigned long)task_stack_page(c_idle.idle) -
=======
	clear_tsk_thread_flag(idle, TIF_FORK);
	initial_gs = per_cpu_offset(cpu);
	per_cpu(kernel_stack, cpu) =
		(unsigned long)task_stack_page(idle) -
>>>>>>> refs/remotes/origin/master
		KERNEL_STACK_OFFSET + THREAD_SIZE;
#endif
	early_gdt_descr.address = (unsigned long)get_cpu_gdt_table(cpu);
	initial_code = (unsigned long)start_secondary;
<<<<<<< HEAD
	stack_start  = c_idle.idle->thread.sp;

	/* start_ip had better be page-aligned! */
	start_ip = trampoline_address();
=======
	stack_start  = idle->thread.sp;
>>>>>>> refs/remotes/origin/master

	/* So we see what's up */
	announce_cpu(cpu, apicid);

	/*
	 * This grunge runs the startup process for
	 * the targeted processor.
	 */

<<<<<<< HEAD
<<<<<<< HEAD
	printk(KERN_DEBUG "smpboot cpu %d: start_ip = %lx\n", cpu, start_ip);

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	atomic_set(&init_deasserted, 0);

	if (get_uv_system_type() != UV_NON_UNIQUE_APIC) {

		pr_debug("Setting warm reset code and vector.\n");

		smpboot_setup_warm_reset_vector(start_ip);
		/*
		 * Be paranoid about clearing APIC errors.
		*/
		if (APIC_INTEGRATED(apic_version[boot_cpu_physical_apicid])) {
			apic_write(APIC_ESR, 0);
			apic_read(APIC_ESR);
		}
	}

	/*
<<<<<<< HEAD
	 * Kick the secondary CPU. Use the method in the APIC driver
	 * if it's defined - or use an INIT boot APIC message otherwise:
=======
	 * Wake up a CPU in difference cases:
	 * - Use the method in the APIC driver if it's defined
	 * Otherwise,
	 * - Use an INIT boot APIC message for APs or NMI for BSP.
>>>>>>> refs/remotes/origin/master
	 */
	if (apic->wakeup_secondary_cpu)
		boot_error = apic->wakeup_secondary_cpu(apicid, start_ip);
	else
<<<<<<< HEAD
		boot_error = wakeup_secondary_cpu_via_init(apicid, start_ip);
=======
		boot_error = wakeup_cpu_via_init_nmi(cpu, start_ip, apicid,
						     &cpu0_nmi_registered);
>>>>>>> refs/remotes/origin/master

	if (!boot_error) {
		/*
		 * allow APs to start initializing.
		 */
<<<<<<< HEAD
		pr_debug("Before Callout %d.\n", cpu);
		cpumask_set_cpu(cpu, cpu_callout_mask);
		pr_debug("After Callout %d.\n", cpu);
=======
		pr_debug("Before Callout %d\n", cpu);
		cpumask_set_cpu(cpu, cpu_callout_mask);
		pr_debug("After Callout %d\n", cpu);
>>>>>>> refs/remotes/origin/master

		/*
		 * Wait 5s total for a response
		 */
		for (timeout = 0; timeout < 50000; timeout++) {
			if (cpumask_test_cpu(cpu, cpu_callin_mask))
				break;	/* It has booted */
			udelay(100);
			/*
			 * Allow other tasks to run while we wait for the
			 * AP to come online. This also gives a chance
			 * for the MTRR work(triggered by the AP coming online)
			 * to be completed in the stop machine context.
			 */
			schedule();
		}

<<<<<<< HEAD
<<<<<<< HEAD
		if (cpumask_test_cpu(cpu, cpu_callin_mask))
			pr_debug("CPU%d: has booted.\n", cpu);
		else {
=======
=======
>>>>>>> refs/remotes/origin/master
		if (cpumask_test_cpu(cpu, cpu_callin_mask)) {
			print_cpu_msr(&cpu_data(cpu));
			pr_debug("CPU%d: has booted.\n", cpu);
		} else {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
			boot_error = 1;
			if (*(volatile u32 *)TRAMPOLINE_SYM(trampoline_status)
			    == 0xA5A5A5A5)
=======
			boot_error = 1;
			if (*trampoline_status == 0xA5A5A5A5)
>>>>>>> refs/remotes/origin/master
				/* trampoline started but...? */
				pr_err("CPU%d: Stuck ??\n", cpu);
			else
				/* trampoline code not run */
<<<<<<< HEAD
				pr_err("CPU%d: Not responding.\n", cpu);
=======
				pr_err("CPU%d: Not responding\n", cpu);
>>>>>>> refs/remotes/origin/master
			if (apic->inquire_remote_apic)
				apic->inquire_remote_apic(apicid);
		}
	}

	if (boot_error) {
		/* Try to put things back the way they were before ... */
		numa_remove_cpu(cpu); /* was set by numa_add_cpu */

		/* was set by do_boot_cpu() */
		cpumask_clear_cpu(cpu, cpu_callout_mask);

		/* was set by cpu_init() */
		cpumask_clear_cpu(cpu, cpu_initialized_mask);

		set_cpu_present(cpu, false);
		per_cpu(x86_cpu_to_apicid, cpu) = BAD_APICID;
	}

	/* mark "stuck" area as not stuck */
<<<<<<< HEAD
	*(volatile u32 *)TRAMPOLINE_SYM(trampoline_status) = 0;
=======
	*trampoline_status = 0;
>>>>>>> refs/remotes/origin/master

	if (get_uv_system_type() != UV_NON_UNIQUE_APIC) {
		/*
		 * Cleanup possible dangling ends...
		 */
		smpboot_restore_warm_reset_vector();
	}
<<<<<<< HEAD

	destroy_work_on_stack(&c_idle.work);
	return boot_error;
}

int __cpuinit native_cpu_up(unsigned int cpu)
=======
	/*
	 * Clean up the nmi handler. Do this after the callin and callout sync
	 * to avoid impact of possible long unregister time.
	 */
	if (cpu0_nmi_registered)
		unregister_nmi_handler(NMI_LOCAL, "wake_cpu0");

	return boot_error;
}

int native_cpu_up(unsigned int cpu, struct task_struct *tidle)
>>>>>>> refs/remotes/origin/master
{
	int apicid = apic->cpu_present_to_apicid(cpu);
	unsigned long flags;
	int err;

	WARN_ON(irqs_disabled());

	pr_debug("++++++++++++++++++++=_---CPU UP  %u\n", cpu);

<<<<<<< HEAD
	if (apicid == BAD_APICID || apicid == boot_cpu_physical_apicid ||
<<<<<<< HEAD
	    !physid_isset(apicid, phys_cpu_present_map)) {
=======
	    !physid_isset(apicid, phys_cpu_present_map) ||
	    !apic->apic_id_valid(apicid)) {
>>>>>>> refs/remotes/origin/cm-10.0
		printk(KERN_ERR "%s: bad cpu %d\n", __func__, cpu);
=======
	if (apicid == BAD_APICID ||
	    !physid_isset(apicid, phys_cpu_present_map) ||
	    !apic->apic_id_valid(apicid)) {
		pr_err("%s: bad cpu %d\n", __func__, cpu);
>>>>>>> refs/remotes/origin/master
		return -EINVAL;
	}

	/*
	 * Already booted CPU?
	 */
	if (cpumask_test_cpu(cpu, cpu_callin_mask)) {
		pr_debug("do_boot_cpu %d Already started\n", cpu);
		return -ENOSYS;
	}

	/*
	 * Save current MTRR state in case it was changed since early boot
	 * (e.g. by the ACPI SMI) to initialize new CPUs with MTRRs in sync:
	 */
	mtrr_save_state();

	per_cpu(cpu_state, cpu) = CPU_UP_PREPARE;

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
	/* the FPU context is blank, nobody can own it */
	__cpu_disable_lazy_restore(cpu);

>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* the FPU context is blank, nobody can own it */
	__cpu_disable_lazy_restore(cpu);

>>>>>>> refs/remotes/origin/cm-11.0
	err = do_boot_cpu(apicid, cpu);
=======
	/* the FPU context is blank, nobody can own it */
	__cpu_disable_lazy_restore(cpu);

	err = do_boot_cpu(apicid, cpu, tidle);
>>>>>>> refs/remotes/origin/master
	if (err) {
		pr_debug("do_boot_cpu failed %d\n", err);
		return -EIO;
	}

	/*
	 * Check TSC synchronization with the AP (keep irqs disabled
	 * while doing so):
	 */
	local_irq_save(flags);
	check_tsc_sync_source(cpu);
	local_irq_restore(flags);

	while (!cpu_online(cpu)) {
		cpu_relax();
		touch_nmi_watchdog();
	}

	return 0;
}

/**
 * arch_disable_smp_support() - disables SMP support for x86 at runtime
 */
void arch_disable_smp_support(void)
{
	disable_ioapic_support();
}

/*
 * Fall back to non SMP mode after errors.
 *
 * RED-PEN audit/test this more. I bet there is more state messed up here.
 */
static __init void disable_smp(void)
{
	init_cpu_present(cpumask_of(0));
	init_cpu_possible(cpumask_of(0));
	smpboot_clear_io_apic_irqs();

	if (smp_found_config)
		physid_set_mask_of_physid(boot_cpu_physical_apicid, &phys_cpu_present_map);
	else
		physid_set_mask_of_physid(0, &phys_cpu_present_map);
	cpumask_set_cpu(0, cpu_sibling_mask(0));
	cpumask_set_cpu(0, cpu_core_mask(0));
}

/*
 * Various sanity checks.
 */
static int __init smp_sanity_check(unsigned max_cpus)
{
	preempt_disable();

#if !defined(CONFIG_X86_BIGSMP) && defined(CONFIG_X86_32)
	if (def_to_bigsmp && nr_cpu_ids > 8) {
		unsigned int cpu;
		unsigned nr;

<<<<<<< HEAD
		printk(KERN_WARNING
		       "More than 8 CPUs detected - skipping them.\n"
		       "Use CONFIG_X86_BIGSMP.\n");
=======
		pr_warn("More than 8 CPUs detected - skipping them\n"
			"Use CONFIG_X86_BIGSMP\n");
>>>>>>> refs/remotes/origin/master

		nr = 0;
		for_each_present_cpu(cpu) {
			if (nr >= 8)
				set_cpu_present(cpu, false);
			nr++;
		}

		nr = 0;
		for_each_possible_cpu(cpu) {
			if (nr >= 8)
				set_cpu_possible(cpu, false);
			nr++;
		}

		nr_cpu_ids = 8;
	}
#endif

	if (!physid_isset(hard_smp_processor_id(), phys_cpu_present_map)) {
<<<<<<< HEAD
		printk(KERN_WARNING
			"weird, boot CPU (#%d) not listed by the BIOS.\n",
=======
		pr_warn("weird, boot CPU (#%d) not listed by the BIOS\n",
>>>>>>> refs/remotes/origin/master
			hard_smp_processor_id());

		physid_set(hard_smp_processor_id(), phys_cpu_present_map);
	}

	/*
	 * If we couldn't find an SMP configuration at boot time,
	 * get out of here now!
	 */
	if (!smp_found_config && !acpi_lapic) {
		preempt_enable();
<<<<<<< HEAD
		printk(KERN_NOTICE "SMP motherboard not detected.\n");
		disable_smp();
		if (APIC_init_uniprocessor())
			printk(KERN_NOTICE "Local APIC not detected."
					   " Using dummy APIC emulation.\n");
=======
		pr_notice("SMP motherboard not detected\n");
		disable_smp();
		if (APIC_init_uniprocessor())
			pr_notice("Local APIC not detected. Using dummy APIC emulation.\n");
>>>>>>> refs/remotes/origin/master
		return -1;
	}

	/*
	 * Should not be necessary because the MP table should list the boot
	 * CPU too, but we do it for the sake of robustness anyway.
	 */
	if (!apic->check_phys_apicid_present(boot_cpu_physical_apicid)) {
<<<<<<< HEAD
		printk(KERN_NOTICE
			"weird, boot CPU (#%d) not listed by the BIOS.\n",
			boot_cpu_physical_apicid);
=======
		pr_notice("weird, boot CPU (#%d) not listed by the BIOS\n",
			  boot_cpu_physical_apicid);
>>>>>>> refs/remotes/origin/master
		physid_set(hard_smp_processor_id(), phys_cpu_present_map);
	}
	preempt_enable();

	/*
	 * If we couldn't find a local APIC, then get out of here now!
	 */
	if (APIC_INTEGRATED(apic_version[boot_cpu_physical_apicid]) &&
	    !cpu_has_apic) {
		if (!disable_apic) {
			pr_err("BIOS bug, local APIC #%d not detected!...\n",
				boot_cpu_physical_apicid);
<<<<<<< HEAD
			pr_err("... forcing use of dummy APIC emulation."
				"(tell your hw vendor)\n");
=======
			pr_err("... forcing use of dummy APIC emulation (tell your hw vendor)\n");
>>>>>>> refs/remotes/origin/master
		}
		smpboot_clear_io_apic();
		disable_ioapic_support();
		return -1;
	}

	verify_local_APIC();

	/*
	 * If SMP should be disabled, then really disable it!
	 */
	if (!max_cpus) {
<<<<<<< HEAD
		printk(KERN_INFO "SMP mode deactivated.\n");
=======
		pr_info("SMP mode deactivated\n");
>>>>>>> refs/remotes/origin/master
		smpboot_clear_io_apic();

		connect_bsp_APIC();
		setup_local_APIC();
		bsp_end_local_APIC_setup();
		return -1;
	}

	return 0;
}

static void __init smp_cpu_index_default(void)
{
	int i;
	struct cpuinfo_x86 *c;

	for_each_possible_cpu(i) {
		c = &cpu_data(i);
		/* mark all to hotplug */
		c->cpu_index = nr_cpu_ids;
	}
}

/*
 * Prepare for SMP bootup.  The MP table or ACPI has been read
 * earlier.  Just do some sanity checking here and enable APIC mode.
 */
void __init native_smp_prepare_cpus(unsigned int max_cpus)
{
	unsigned int i;

	preempt_disable();
	smp_cpu_index_default();

	/*
	 * Setup boot CPU information
	 */
<<<<<<< HEAD
	smp_store_cpu_info(0); /* Final full version of the data */
=======
	smp_store_boot_cpu_info(); /* Final full version of the data */
>>>>>>> refs/remotes/origin/master
	cpumask_copy(cpu_callin_mask, cpumask_of(0));
	mb();

	current_thread_info()->cpu = 0;  /* needed? */
	for_each_possible_cpu(i) {
		zalloc_cpumask_var(&per_cpu(cpu_sibling_map, i), GFP_KERNEL);
		zalloc_cpumask_var(&per_cpu(cpu_core_map, i), GFP_KERNEL);
		zalloc_cpumask_var(&per_cpu(cpu_llc_shared_map, i), GFP_KERNEL);
	}
	set_cpu_sibling_map(0);


	if (smp_sanity_check(max_cpus) < 0) {
<<<<<<< HEAD
		printk(KERN_INFO "SMP disabled\n");
=======
		pr_info("SMP disabled\n");
>>>>>>> refs/remotes/origin/master
		disable_smp();
		goto out;
	}

	default_setup_apic_routing();

	preempt_disable();
	if (read_apic_id() != boot_cpu_physical_apicid) {
		panic("Boot APIC ID in local APIC unexpected (%d vs %d)",
		     read_apic_id(), boot_cpu_physical_apicid);
		/* Or can we switch back to PIC here? */
	}
	preempt_enable();

	connect_bsp_APIC();

	/*
	 * Switch from PIC to APIC mode.
	 */
	setup_local_APIC();

<<<<<<< HEAD
=======
	if (x2apic_mode)
		cpu0_logical_apicid = apic_read(APIC_LDR);
	else
		cpu0_logical_apicid = GET_APIC_LOGICAL_ID(apic_read(APIC_LDR));

>>>>>>> refs/remotes/origin/master
	/*
	 * Enable IO APIC before setting up error vector
	 */
	if (!skip_ioapic_setup && nr_ioapics)
		enable_IO_APIC();

	bsp_end_local_APIC_setup();

	if (apic->setup_portio_remap)
		apic->setup_portio_remap();

	smpboot_setup_io_apic();
	/*
	 * Set up local APIC timer on boot CPU.
	 */

<<<<<<< HEAD
	printk(KERN_INFO "CPU%d: ", 0);
=======
	pr_info("CPU%d: ", 0);
>>>>>>> refs/remotes/origin/master
	print_cpu_info(&cpu_data(0));
	x86_init.timers.setup_percpu_clockev();

	if (is_uv_system())
		uv_system_init();

	set_mtrr_aps_delayed_init();
out:
	preempt_enable();
}

<<<<<<< HEAD
void arch_disable_nonboot_cpus_begin(void)
{
	/*
	 * Avoid the smp alternatives switch during the disable_nonboot_cpus().
	 * In the suspend path, we will be back in the SMP mode shortly anyways.
	 */
	skip_smp_alternatives = true;
}

void arch_disable_nonboot_cpus_end(void)
{
	skip_smp_alternatives = false;
}

=======
>>>>>>> refs/remotes/origin/master
void arch_enable_nonboot_cpus_begin(void)
{
	set_mtrr_aps_delayed_init();
}

void arch_enable_nonboot_cpus_end(void)
{
	mtrr_aps_init();
}

/*
 * Early setup to make printk work.
 */
void __init native_smp_prepare_boot_cpu(void)
{
	int me = smp_processor_id();
	switch_to_new_gdt(me);
	/* already set me in cpu_online_mask in boot_cpu_init() */
	cpumask_set_cpu(me, cpu_callout_mask);
	per_cpu(cpu_state, me) = CPU_ONLINE;
}

void __init native_smp_cpus_done(unsigned int max_cpus)
{
<<<<<<< HEAD
	pr_debug("Boot done.\n");

<<<<<<< HEAD
=======
	nmi_selftest();
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pr_debug("Boot done\n");

	nmi_selftest();
>>>>>>> refs/remotes/origin/master
	impress_friends();
#ifdef CONFIG_X86_IO_APIC
	setup_ioapic_dest();
#endif
	mtrr_aps_init();
}

static int __initdata setup_possible_cpus = -1;
static int __init _setup_possible_cpus(char *str)
{
	get_option(&str, &setup_possible_cpus);
	return 0;
}
early_param("possible_cpus", _setup_possible_cpus);


/*
 * cpu_possible_mask should be static, it cannot change as cpu's
 * are onlined, or offlined. The reason is per-cpu data-structures
 * are allocated by some modules at init time, and dont expect to
 * do this dynamically on cpu arrival/departure.
 * cpu_present_mask on the other hand can change dynamically.
 * In case when cpu_hotplug is not compiled, then we resort to current
 * behaviour, which is cpu_possible == cpu_present.
 * - Ashok Raj
 *
 * Three ways to find out the number of additional hotplug CPUs:
 * - If the BIOS specified disabled CPUs in ACPI/mptables use that.
 * - The user can overwrite it with possible_cpus=NUM
 * - Otherwise don't reserve additional CPUs.
 * We do this because additional CPUs waste a lot of memory.
 * -AK
 */
__init void prefill_possible_map(void)
{
	int i, possible;

	/* no processor from mptable or madt */
	if (!num_processors)
		num_processors = 1;

	i = setup_max_cpus ?: 1;
	if (setup_possible_cpus == -1) {
		possible = num_processors;
#ifdef CONFIG_HOTPLUG_CPU
		if (setup_max_cpus)
			possible += disabled_cpus;
#else
		if (possible > i)
			possible = i;
#endif
	} else
		possible = setup_possible_cpus;

	total_cpus = max_t(int, possible, num_processors + disabled_cpus);

	/* nr_cpu_ids could be reduced via nr_cpus= */
	if (possible > nr_cpu_ids) {
<<<<<<< HEAD
		printk(KERN_WARNING
			"%d Processors exceeds NR_CPUS limit of %d\n",
=======
		pr_warn("%d Processors exceeds NR_CPUS limit of %d\n",
>>>>>>> refs/remotes/origin/master
			possible, nr_cpu_ids);
		possible = nr_cpu_ids;
	}

#ifdef CONFIG_HOTPLUG_CPU
	if (!setup_max_cpus)
#endif
	if (possible > i) {
<<<<<<< HEAD
		printk(KERN_WARNING
			"%d Processors exceeds max_cpus limit of %u\n",
=======
		pr_warn("%d Processors exceeds max_cpus limit of %u\n",
>>>>>>> refs/remotes/origin/master
			possible, setup_max_cpus);
		possible = i;
	}

<<<<<<< HEAD
	printk(KERN_INFO "SMP: Allowing %d CPUs, %d hotplug CPUs\n",
=======
	pr_info("Allowing %d CPUs, %d hotplug CPUs\n",
>>>>>>> refs/remotes/origin/master
		possible, max_t(int, possible - num_processors, 0));

	for (i = 0; i < possible; i++)
		set_cpu_possible(i, true);
	for (; i < NR_CPUS; i++)
		set_cpu_possible(i, false);

	nr_cpu_ids = possible;
}

#ifdef CONFIG_HOTPLUG_CPU

static void remove_siblinginfo(int cpu)
{
	int sibling;
	struct cpuinfo_x86 *c = &cpu_data(cpu);

	for_each_cpu(sibling, cpu_core_mask(cpu)) {
		cpumask_clear_cpu(cpu, cpu_core_mask(sibling));
		/*/
		 * last thread sibling in this cpu core going down
		 */
		if (cpumask_weight(cpu_sibling_mask(cpu)) == 1)
			cpu_data(sibling).booted_cores--;
	}

	for_each_cpu(sibling, cpu_sibling_mask(cpu))
		cpumask_clear_cpu(cpu, cpu_sibling_mask(sibling));
	cpumask_clear(cpu_sibling_mask(cpu));
	cpumask_clear(cpu_core_mask(cpu));
	c->phys_proc_id = 0;
	c->cpu_core_id = 0;
	cpumask_clear_cpu(cpu, cpu_sibling_setup_mask);
}

static void __ref remove_cpu_from_maps(int cpu)
{
	set_cpu_online(cpu, false);
	cpumask_clear_cpu(cpu, cpu_callout_mask);
	cpumask_clear_cpu(cpu, cpu_callin_mask);
	/* was set by cpu_init() */
	cpumask_clear_cpu(cpu, cpu_initialized_mask);
	numa_remove_cpu(cpu);
}

void cpu_disable_common(void)
{
	int cpu = smp_processor_id();

	remove_siblinginfo(cpu);

	/* It's now safe to remove this processor from the online map */
	lock_vector_lock();
	remove_cpu_from_maps(cpu);
	unlock_vector_lock();
	fixup_irqs();
}

int native_cpu_disable(void)
{
<<<<<<< HEAD
	int cpu = smp_processor_id();

	/*
	 * Perhaps use cpufreq to drop frequency, but that could go
	 * into generic code.
	 *
	 * We won't take down the boot processor on i386 due to some
	 * interrupts only being able to be serviced by the BSP.
	 * Especially so if we're not using an IOAPIC	-zwane
	 */
	if (cpu == 0)
		return -EBUSY;
=======
	int ret;

	ret = check_irq_vectors_for_cpu_disable();
	if (ret)
		return ret;
>>>>>>> refs/remotes/origin/master

	clear_local_APIC();

	cpu_disable_common();
	return 0;
}

void native_cpu_die(unsigned int cpu)
{
	/* We don't do anything here: idle task is faking death itself. */
	unsigned int i;

	for (i = 0; i < 10; i++) {
		/* They ack this in play_dead by setting CPU_DEAD */
		if (per_cpu(cpu_state, cpu) == CPU_DEAD) {
			if (system_state == SYSTEM_RUNNING)
				pr_info("CPU %u is now offline\n", cpu);
<<<<<<< HEAD

			if (1 == num_online_cpus())
				alternatives_smp_switch(0);
=======
>>>>>>> refs/remotes/origin/master
			return;
		}
		msleep(100);
	}
	pr_err("CPU %u didn't die...\n", cpu);
}

void play_dead_common(void)
{
	idle_task_exit();
	reset_lazy_tlbstate();
	amd_e400_remove_cpu(raw_smp_processor_id());

	mb();
	/* Ack it */
	__this_cpu_write(cpu_state, CPU_DEAD);

	/*
	 * With physical CPU hotplug, we should halt the cpu
	 */
	local_irq_disable();
}

<<<<<<< HEAD
=======
static bool wakeup_cpu0(void)
{
	if (smp_processor_id() == 0 && enable_start_cpu0)
		return true;

	return false;
}

>>>>>>> refs/remotes/origin/master
/*
 * We need to flush the caches before going to sleep, lest we have
 * dirty data in our caches when we come back up.
 */
static inline void mwait_play_dead(void)
{
	unsigned int eax, ebx, ecx, edx;
	unsigned int highest_cstate = 0;
	unsigned int highest_subcstate = 0;
<<<<<<< HEAD
	int i;
	void *mwait_ptr;
	struct cpuinfo_x86 *c = __this_cpu_ptr(&cpu_info);

	if (!(this_cpu_has(X86_FEATURE_MWAIT) && mwait_usable(c)))
=======
	void *mwait_ptr;
	int i;

	if (!this_cpu_has(X86_FEATURE_MWAIT))
>>>>>>> refs/remotes/origin/master
		return;
	if (!this_cpu_has(X86_FEATURE_CLFLSH))
		return;
	if (__this_cpu_read(cpu_info.cpuid_level) < CPUID_MWAIT_LEAF)
		return;

	eax = CPUID_MWAIT_LEAF;
	ecx = 0;
	native_cpuid(&eax, &ebx, &ecx, &edx);

	/*
	 * eax will be 0 if EDX enumeration is not valid.
	 * Initialized below to cstate, sub_cstate value when EDX is valid.
	 */
	if (!(ecx & CPUID5_ECX_EXTENSIONS_SUPPORTED)) {
		eax = 0;
	} else {
		edx >>= MWAIT_SUBSTATE_SIZE;
		for (i = 0; i < 7 && edx; i++, edx >>= MWAIT_SUBSTATE_SIZE) {
			if (edx & MWAIT_SUBSTATE_MASK) {
				highest_cstate = i;
				highest_subcstate = edx & MWAIT_SUBSTATE_MASK;
			}
		}
		eax = (highest_cstate << MWAIT_SUBSTATE_SIZE) |
			(highest_subcstate - 1);
	}

	/*
	 * This should be a memory location in a cache line which is
	 * unlikely to be touched by other processors.  The actual
	 * content is immaterial as it is not actually modified in any way.
	 */
	mwait_ptr = &current_thread_info()->flags;

	wbinvd();

	while (1) {
		/*
		 * The CLFLUSH is a workaround for erratum AAI65 for
		 * the Xeon 7400 series.  It's not clear it is actually
		 * needed, but it should be harmless in either case.
		 * The WBINVD is insufficient due to the spurious-wakeup
		 * case where we return around the loop.
		 */
<<<<<<< HEAD
		clflush(mwait_ptr);
		__monitor(mwait_ptr, 0, 0);
		mb();
		__mwait(eax, 0);
=======
		mb();
		clflush(mwait_ptr);
		mb();
		__monitor(mwait_ptr, 0, 0);
		mb();
		__mwait(eax, 0);
		/*
		 * If NMI wants to wake up CPU0, start CPU0.
		 */
		if (wakeup_cpu0())
			start_cpu0();
>>>>>>> refs/remotes/origin/master
	}
}

static inline void hlt_play_dead(void)
{
	if (__this_cpu_read(cpu_info.x86) >= 4)
		wbinvd();

	while (1) {
		native_halt();
<<<<<<< HEAD
=======
		/*
		 * If NMI wants to wake up CPU0, start CPU0.
		 */
		if (wakeup_cpu0())
			start_cpu0();
>>>>>>> refs/remotes/origin/master
	}
}

void native_play_dead(void)
{
	play_dead_common();
	tboot_shutdown(TB_SHUTDOWN_WFS);

	mwait_play_dead();	/* Only returns on failure */
<<<<<<< HEAD
<<<<<<< HEAD
	hlt_play_dead();
=======
	if (cpuidle_play_dead())
		hlt_play_dead();
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (cpuidle_play_dead())
		hlt_play_dead();
>>>>>>> refs/remotes/origin/master
}

#else /* ... !CONFIG_HOTPLUG_CPU */
int native_cpu_disable(void)
{
	return -ENOSYS;
}

void native_cpu_die(unsigned int cpu)
{
	/* We said "no" in __cpu_disable */
	BUG();
}

void native_play_dead(void)
{
	BUG();
}

#endif
