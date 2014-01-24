/*
 * Idle daemon for PowerPC.  Idle daemon will handle any action
 * that needs to be taken when the system becomes idle.
 *
 * Originally written by Cort Dougan (cort@cs.nmt.edu).
 * Subsequent 32-bit hacking by Tom Rini, Armin Kuster,
 * Paul Mackerras and others.
 *
 * iSeries supported added by Mike Corrigan <mikejc@us.ibm.com>
 *
 * Additional shared processor, SMT, and firmware support
 *    Copyright (c) 2003 Dave Engebretsen <engebret@us.ibm.com>
 *
 * 32-bit and 64-bit versions merged by Paul Mackerras <paulus@samba.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/smp.h>
#include <linux/cpu.h>
#include <linux/sysctl.h>
#include <linux/tick.h>

<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/system.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#include <asm/processor.h>
#include <asm/cputable.h>
#include <asm/time.h>
#include <asm/machdep.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <asm/runlatch.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <asm/smp.h>

#ifdef CONFIG_HOTPLUG_CPU
#define cpu_should_die()	cpu_is_offline(smp_processor_id())
#else
#define cpu_should_die()	0
#endif

<<<<<<< HEAD
static int __init powersave_off(char *arg)
{
	ppc_md.power_save = NULL;
=======
=======
#include <asm/runlatch.h>
#include <asm/smp.h>


>>>>>>> refs/remotes/origin/master
unsigned long cpuidle_disable = IDLE_NO_OVERRIDE;
EXPORT_SYMBOL(cpuidle_disable);

static int __init powersave_off(char *arg)
{
	ppc_md.power_save = NULL;
	cpuidle_disable = IDLE_POWERSAVE_OFF;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return 0;
}
__setup("powersave=off", powersave_off);

<<<<<<< HEAD
/*
 * The body of the idle task.
 */
void cpu_idle(void)
{
	if (ppc_md.idle_loop)
		ppc_md.idle_loop();	/* doesn't return */

	set_thread_flag(TIF_POLLING_NRFLAG);
	while (1) {
<<<<<<< HEAD
		tick_nohz_stop_sched_tick(1);
=======
		tick_nohz_idle_enter();
		rcu_idle_enter();

>>>>>>> refs/remotes/origin/cm-10.0
		while (!need_resched() && !cpu_should_die()) {
			ppc64_runlatch_off();

			if (ppc_md.power_save) {
				clear_thread_flag(TIF_POLLING_NRFLAG);
				/*
				 * smp_mb is so clearing of TIF_POLLING_NRFLAG
				 * is ordered w.r.t. need_resched() test.
				 */
				smp_mb();
				local_irq_disable();

				/* Don't trace irqs off for idle */
				stop_critical_timings();

				/* check again after disabling irqs */
				if (!need_resched() && !cpu_should_die())
					ppc_md.power_save();

				start_critical_timings();

<<<<<<< HEAD
				local_irq_enable();
=======
				/* Some power_save functions return with
				 * interrupts enabled, some don't.
				 */
				if (irqs_disabled())
					local_irq_enable();
>>>>>>> refs/remotes/origin/cm-10.0
				set_thread_flag(TIF_POLLING_NRFLAG);

			} else {
				/*
				 * Go into low thread priority and possibly
				 * low power mode.
				 */
				HMT_low();
				HMT_very_low();
			}
		}

		HMT_medium();
		ppc64_runlatch_on();
<<<<<<< HEAD
		tick_nohz_restart_sched_tick();
		preempt_enable_no_resched();
		if (cpu_should_die())
			cpu_die();
		schedule();
		preempt_disable();
	}
}
=======
		rcu_idle_exit();
		tick_nohz_idle_exit();
		if (cpu_should_die()) {
			sched_preempt_enable_no_resched();
			cpu_die();
		}
		schedule_preempt_disabled();
	}
}


/*
 * cpu_idle_wait - Used to ensure that all the CPUs come out of the old
 * idle loop and start using the new idle loop.
 * Required while changing idle handler on SMP systems.
 * Caller must have changed idle handler to the new value before the call.
 * This window may be larger on shared systems.
 */
void cpu_idle_wait(void)
{
	int cpu;
	smp_mb();

	/* kick all the CPUs so that they exit out of old idle routine */
	get_online_cpus();
	for_each_online_cpu(cpu) {
		if (cpu != smp_processor_id())
			smp_send_reschedule(cpu);
	}
	put_online_cpus();
}
EXPORT_SYMBOL_GPL(cpu_idle_wait);
>>>>>>> refs/remotes/origin/cm-10.0
=======
#ifdef CONFIG_HOTPLUG_CPU
void arch_cpu_idle_dead(void)
{
	sched_preempt_enable_no_resched();
	cpu_die();
}
#endif

void arch_cpu_idle(void)
{
	ppc64_runlatch_off();

	if (ppc_md.power_save) {
		ppc_md.power_save();
		/*
		 * Some power_save functions return with
		 * interrupts enabled, some don't.
		 */
		if (irqs_disabled())
			local_irq_enable();
	} else {
		local_irq_enable();
		/*
		 * Go into low thread priority and possibly
		 * low power mode.
		 */
		HMT_low();
		HMT_very_low();
	}

	HMT_medium();
	ppc64_runlatch_on();
}
>>>>>>> refs/remotes/origin/master

int powersave_nap;

#ifdef CONFIG_SYSCTL
/*
 * Register the sysctl to set/clear powersave_nap.
 */
<<<<<<< HEAD
static ctl_table powersave_nap_ctl_table[]={
=======
static struct ctl_table powersave_nap_ctl_table[] = {
>>>>>>> refs/remotes/origin/master
	{
		.procname	= "powersave-nap",
		.data		= &powersave_nap,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{}
};
<<<<<<< HEAD
static ctl_table powersave_nap_sysctl_root[] = {
=======
static struct ctl_table powersave_nap_sysctl_root[] = {
>>>>>>> refs/remotes/origin/master
	{
		.procname	= "kernel",
		.mode		= 0555,
		.child		= powersave_nap_ctl_table,
	},
	{}
};

static int __init
register_powersave_nap_sysctl(void)
{
	register_sysctl_table(powersave_nap_sysctl_root);

	return 0;
}
__initcall(register_powersave_nap_sysctl);
#endif
