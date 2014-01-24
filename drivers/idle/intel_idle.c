/*
 * intel_idle.c - native hardware idle loop for modern Intel processors
 *
<<<<<<< HEAD
 * Copyright (c) 2010, Intel Corporation.
=======
 * Copyright (c) 2013, Intel Corporation.
>>>>>>> refs/remotes/origin/master
 * Len Brown <len.brown@intel.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 */

/*
 * intel_idle is a cpuidle driver that loads on specific Intel processors
 * in lieu of the legacy ACPI processor_idle driver.  The intent is to
 * make Linux more efficient on these processors, as intel_idle knows
 * more than ACPI, as well as make Linux more immune to ACPI BIOS bugs.
 */

/*
 * Design Assumptions
 *
 * All CPUs have same idle states as boot CPU
 *
 * Chipset BM_STS (bus master status) bit is a NOP
 *	for preventing entry into deep C-stats
 */

/*
 * Known limitations
 *
 * The driver currently initializes for_each_online_cpu() upon modprobe.
 * It it unaware of subsequent processors hot-added to the system.
 * This means that if you boot with maxcpus=n and later online
 * processors above n, those processors will use C1 only.
 *
 * ACPI has a .suspend hack to turn off deep c-statees during suspend
 * to avoid complications with the lapic timer workaround.
 * Have not seen issues with suspend, but may need same workaround here.
 *
 * There is currently no kernel-based automatic probing/loading mechanism
 * if the driver is built as a module.
 */

/* un-comment DEBUG to enable pr_debug() statements */
#define DEBUG

#include <linux/kernel.h>
#include <linux/cpuidle.h>
#include <linux/clockchips.h>
<<<<<<< HEAD
#include <linux/hrtimer.h>	/* ktime_get_real() */
=======
>>>>>>> refs/remotes/origin/master
#include <trace/events/power.h>
#include <linux/sched.h>
#include <linux/notifier.h>
#include <linux/cpu.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/module.h>
#include <asm/cpu_device_id.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/module.h>
#include <asm/cpu_device_id.h>
>>>>>>> refs/remotes/origin/master
#include <asm/mwait.h>
#include <asm/msr.h>

#define INTEL_IDLE_VERSION "0.4"
#define PREFIX "intel_idle: "

static struct cpuidle_driver intel_idle_driver = {
	.name = "intel_idle",
	.owner = THIS_MODULE,
};
/* intel_idle.max_cstate=0 disables driver */
<<<<<<< HEAD
static int max_cstate = MWAIT_MAX_NUM_CSTATES - 1;
=======
static int max_cstate = CPUIDLE_STATE_MAX - 1;
>>>>>>> refs/remotes/origin/master

static unsigned int mwait_substates;

#define LAPIC_TIMER_ALWAYS_RELIABLE 0xFFFFFFFF
/* Reliable LAPIC Timer States, bit 1 for C1 etc.  */
static unsigned int lapic_timer_reliable_states = (1 << 1);	 /* Default to only C1 */

<<<<<<< HEAD
<<<<<<< HEAD
static struct cpuidle_device __percpu *intel_idle_cpuidle_devices;
static int intel_idle(struct cpuidle_device *dev, struct cpuidle_state *state);
=======
=======
>>>>>>> refs/remotes/origin/master
struct idle_cpu {
	struct cpuidle_state *state_table;

	/*
	 * Hardware C-state auto-demotion may not always be optimal.
	 * Indicate which enable bits to clear here.
	 */
	unsigned long auto_demotion_disable_flags;
<<<<<<< HEAD
=======
	bool disable_promotion_to_c1e;
>>>>>>> refs/remotes/origin/master
};

static const struct idle_cpu *icpu;
static struct cpuidle_device __percpu *intel_idle_cpuidle_devices;
static int intel_idle(struct cpuidle_device *dev,
			struct cpuidle_driver *drv, int index);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int intel_idle_cpu_init(int cpu);
>>>>>>> refs/remotes/origin/master

static struct cpuidle_state *cpuidle_state_table;

/*
<<<<<<< HEAD
<<<<<<< HEAD
 * Hardware C-state auto-demotion may not always be optimal.
 * Indicate which enable bits to clear here.
 */
static unsigned long long auto_demotion_disable_flags;

/*
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 * Set this flag for states where the HW flushes the TLB for us
 * and so we don't need cross-calls to keep it consistent.
 * If this flag is set, SW flushes the TLB, so even if the
 * HW doesn't do the flushing, this flag is safe to use.
 */
#define CPUIDLE_FLAG_TLB_FLUSHED	0x10000

/*
<<<<<<< HEAD
=======
 * MWAIT takes an 8-bit "hint" in EAX "suggesting"
 * the C-state (top nibble) and sub-state (bottom nibble)
 * 0x00 means "MWAIT(C1)", 0x10 means "MWAIT(C2)" etc.
 *
 * We store the hint at the top of our "flags" for each state.
 */
#define flg2MWAIT(flags) (((flags) >> 24) & 0xFF)
#define MWAIT2flg(eax) ((eax & 0xFF) << 24)

/*
>>>>>>> refs/remotes/origin/master
 * States are indexed by the cstate number,
 * which is also the index into the MWAIT hint array.
 * Thus C0 is a dummy.
 */
<<<<<<< HEAD
static struct cpuidle_state nehalem_cstates[MWAIT_MAX_NUM_CSTATES] = {
	{ /* MWAIT C0 */ },
	{ /* MWAIT C1 */
		.name = "C1-NHM",
		.desc = "MWAIT 0x00",
<<<<<<< HEAD
		.driver_data = (void *) 0x00,
=======
>>>>>>> refs/remotes/origin/cm-10.0
		.flags = CPUIDLE_FLAG_TIME_VALID,
		.exit_latency = 3,
		.target_residency = 6,
		.enter = &intel_idle },
	{ /* MWAIT C2 */
		.name = "C3-NHM",
		.desc = "MWAIT 0x10",
<<<<<<< HEAD
		.driver_data = (void *) 0x10,
=======
>>>>>>> refs/remotes/origin/cm-10.0
		.flags = CPUIDLE_FLAG_TIME_VALID | CPUIDLE_FLAG_TLB_FLUSHED,
		.exit_latency = 20,
		.target_residency = 80,
		.enter = &intel_idle },
	{ /* MWAIT C3 */
		.name = "C6-NHM",
		.desc = "MWAIT 0x20",
<<<<<<< HEAD
		.driver_data = (void *) 0x20,
=======
>>>>>>> refs/remotes/origin/cm-10.0
		.flags = CPUIDLE_FLAG_TIME_VALID | CPUIDLE_FLAG_TLB_FLUSHED,
		.exit_latency = 200,
		.target_residency = 800,
		.enter = &intel_idle },
};

static struct cpuidle_state snb_cstates[MWAIT_MAX_NUM_CSTATES] = {
	{ /* MWAIT C0 */ },
	{ /* MWAIT C1 */
		.name = "C1-SNB",
		.desc = "MWAIT 0x00",
<<<<<<< HEAD
		.driver_data = (void *) 0x00,
=======
>>>>>>> refs/remotes/origin/cm-10.0
		.flags = CPUIDLE_FLAG_TIME_VALID,
		.exit_latency = 1,
		.target_residency = 1,
		.enter = &intel_idle },
	{ /* MWAIT C2 */
		.name = "C3-SNB",
		.desc = "MWAIT 0x10",
<<<<<<< HEAD
		.driver_data = (void *) 0x10,
=======
>>>>>>> refs/remotes/origin/cm-10.0
		.flags = CPUIDLE_FLAG_TIME_VALID | CPUIDLE_FLAG_TLB_FLUSHED,
		.exit_latency = 80,
		.target_residency = 211,
		.enter = &intel_idle },
	{ /* MWAIT C3 */
		.name = "C6-SNB",
		.desc = "MWAIT 0x20",
<<<<<<< HEAD
		.driver_data = (void *) 0x20,
=======
>>>>>>> refs/remotes/origin/cm-10.0
		.flags = CPUIDLE_FLAG_TIME_VALID | CPUIDLE_FLAG_TLB_FLUSHED,
		.exit_latency = 104,
		.target_residency = 345,
		.enter = &intel_idle },
	{ /* MWAIT C4 */
		.name = "C7-SNB",
		.desc = "MWAIT 0x30",
<<<<<<< HEAD
		.driver_data = (void *) 0x30,
=======
>>>>>>> refs/remotes/origin/cm-10.0
		.flags = CPUIDLE_FLAG_TIME_VALID | CPUIDLE_FLAG_TLB_FLUSHED,
		.exit_latency = 109,
		.target_residency = 345,
		.enter = &intel_idle },
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
static struct cpuidle_state ivb_cstates[MWAIT_MAX_NUM_CSTATES] = {
	{ /* MWAIT C0 */ },
	{ /* MWAIT C1 */
		.name = "C1-IVB",
		.desc = "MWAIT 0x00",
		.flags = CPUIDLE_FLAG_TIME_VALID,
		.exit_latency = 1,
		.target_residency = 1,
		.enter = &intel_idle },
	{ /* MWAIT C2 */
		.name = "C3-IVB",
		.desc = "MWAIT 0x10",
		.flags = CPUIDLE_FLAG_TIME_VALID | CPUIDLE_FLAG_TLB_FLUSHED,
		.exit_latency = 59,
		.target_residency = 156,
		.enter = &intel_idle },
	{ /* MWAIT C3 */
		.name = "C6-IVB",
		.desc = "MWAIT 0x20",
		.flags = CPUIDLE_FLAG_TIME_VALID | CPUIDLE_FLAG_TLB_FLUSHED,
		.exit_latency = 80,
		.target_residency = 300,
		.enter = &intel_idle },
	{ /* MWAIT C4 */
		.name = "C7-IVB",
		.desc = "MWAIT 0x30",
		.flags = CPUIDLE_FLAG_TIME_VALID | CPUIDLE_FLAG_TLB_FLUSHED,
		.exit_latency = 87,
		.target_residency = 300,
		.enter = &intel_idle },
};

<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
static struct cpuidle_state atom_cstates[MWAIT_MAX_NUM_CSTATES] = {
	{ /* MWAIT C0 */ },
	{ /* MWAIT C1 */
		.name = "C1-ATM",
		.desc = "MWAIT 0x00",
<<<<<<< HEAD
		.driver_data = (void *) 0x00,
=======
>>>>>>> refs/remotes/origin/cm-10.0
		.flags = CPUIDLE_FLAG_TIME_VALID,
		.exit_latency = 1,
		.target_residency = 4,
		.enter = &intel_idle },
	{ /* MWAIT C2 */
		.name = "C2-ATM",
		.desc = "MWAIT 0x10",
<<<<<<< HEAD
		.driver_data = (void *) 0x10,
=======
>>>>>>> refs/remotes/origin/cm-10.0
		.flags = CPUIDLE_FLAG_TIME_VALID,
		.exit_latency = 20,
		.target_residency = 80,
		.enter = &intel_idle },
	{ /* MWAIT C3 */ },
	{ /* MWAIT C4 */
		.name = "C4-ATM",
		.desc = "MWAIT 0x30",
<<<<<<< HEAD
		.driver_data = (void *) 0x30,
=======
>>>>>>> refs/remotes/origin/cm-10.0
		.flags = CPUIDLE_FLAG_TIME_VALID | CPUIDLE_FLAG_TLB_FLUSHED,
		.exit_latency = 100,
		.target_residency = 400,
		.enter = &intel_idle },
	{ /* MWAIT C5 */ },
	{ /* MWAIT C6 */
		.name = "C6-ATM",
		.desc = "MWAIT 0x52",
<<<<<<< HEAD
		.driver_data = (void *) 0x52,
=======
>>>>>>> refs/remotes/origin/cm-10.0
		.flags = CPUIDLE_FLAG_TIME_VALID | CPUIDLE_FLAG_TLB_FLUSHED,
		.exit_latency = 140,
		.target_residency = 560,
		.enter = &intel_idle },
};

<<<<<<< HEAD
/**
 * intel_idle
 * @dev: cpuidle_device
 * @state: cpuidle state
 *
 */
static int intel_idle(struct cpuidle_device *dev, struct cpuidle_state *state)
{
	unsigned long ecx = 1; /* break on interrupt flag */
	unsigned long eax = (unsigned long)cpuidle_get_statedata(state);
=======
static long get_driver_data(int cstate)
{
	int driver_data;
	switch (cstate) {

	case 1:	/* MWAIT C1 */
		driver_data = 0x00;
		break;
	case 2:	/* MWAIT C2 */
		driver_data = 0x10;
		break;
	case 3:	/* MWAIT C3 */
		driver_data = 0x20;
		break;
	case 4:	/* MWAIT C4 */
		driver_data = 0x30;
		break;
	case 5:	/* MWAIT C5 */
		driver_data = 0x40;
		break;
	case 6:	/* MWAIT C6 */
		driver_data = 0x52;
		break;
	default:
		driver_data = 0x00;
	}
	return driver_data;
}
=======
static struct cpuidle_state nehalem_cstates[] = {
	{
		.name = "C1-NHM",
		.desc = "MWAIT 0x00",
		.flags = MWAIT2flg(0x00) | CPUIDLE_FLAG_TIME_VALID,
		.exit_latency = 3,
		.target_residency = 6,
		.enter = &intel_idle },
	{
		.name = "C1E-NHM",
		.desc = "MWAIT 0x01",
		.flags = MWAIT2flg(0x01) | CPUIDLE_FLAG_TIME_VALID,
		.exit_latency = 10,
		.target_residency = 20,
		.enter = &intel_idle },
	{
		.name = "C3-NHM",
		.desc = "MWAIT 0x10",
		.flags = MWAIT2flg(0x10) | CPUIDLE_FLAG_TIME_VALID | CPUIDLE_FLAG_TLB_FLUSHED,
		.exit_latency = 20,
		.target_residency = 80,
		.enter = &intel_idle },
	{
		.name = "C6-NHM",
		.desc = "MWAIT 0x20",
		.flags = MWAIT2flg(0x20) | CPUIDLE_FLAG_TIME_VALID | CPUIDLE_FLAG_TLB_FLUSHED,
		.exit_latency = 200,
		.target_residency = 800,
		.enter = &intel_idle },
	{
		.enter = NULL }
};

static struct cpuidle_state snb_cstates[] = {
	{
		.name = "C1-SNB",
		.desc = "MWAIT 0x00",
		.flags = MWAIT2flg(0x00) | CPUIDLE_FLAG_TIME_VALID,
		.exit_latency = 2,
		.target_residency = 2,
		.enter = &intel_idle },
	{
		.name = "C1E-SNB",
		.desc = "MWAIT 0x01",
		.flags = MWAIT2flg(0x01) | CPUIDLE_FLAG_TIME_VALID,
		.exit_latency = 10,
		.target_residency = 20,
		.enter = &intel_idle },
	{
		.name = "C3-SNB",
		.desc = "MWAIT 0x10",
		.flags = MWAIT2flg(0x10) | CPUIDLE_FLAG_TIME_VALID | CPUIDLE_FLAG_TLB_FLUSHED,
		.exit_latency = 80,
		.target_residency = 211,
		.enter = &intel_idle },
	{
		.name = "C6-SNB",
		.desc = "MWAIT 0x20",
		.flags = MWAIT2flg(0x20) | CPUIDLE_FLAG_TIME_VALID | CPUIDLE_FLAG_TLB_FLUSHED,
		.exit_latency = 104,
		.target_residency = 345,
		.enter = &intel_idle },
	{
		.name = "C7-SNB",
		.desc = "MWAIT 0x30",
		.flags = MWAIT2flg(0x30) | CPUIDLE_FLAG_TIME_VALID | CPUIDLE_FLAG_TLB_FLUSHED,
		.exit_latency = 109,
		.target_residency = 345,
		.enter = &intel_idle },
	{
		.enter = NULL }
};

static struct cpuidle_state ivb_cstates[] = {
	{
		.name = "C1-IVB",
		.desc = "MWAIT 0x00",
		.flags = MWAIT2flg(0x00) | CPUIDLE_FLAG_TIME_VALID,
		.exit_latency = 1,
		.target_residency = 1,
		.enter = &intel_idle },
	{
		.name = "C1E-IVB",
		.desc = "MWAIT 0x01",
		.flags = MWAIT2flg(0x01) | CPUIDLE_FLAG_TIME_VALID,
		.exit_latency = 10,
		.target_residency = 20,
		.enter = &intel_idle },
	{
		.name = "C3-IVB",
		.desc = "MWAIT 0x10",
		.flags = MWAIT2flg(0x10) | CPUIDLE_FLAG_TIME_VALID | CPUIDLE_FLAG_TLB_FLUSHED,
		.exit_latency = 59,
		.target_residency = 156,
		.enter = &intel_idle },
	{
		.name = "C6-IVB",
		.desc = "MWAIT 0x20",
		.flags = MWAIT2flg(0x20) | CPUIDLE_FLAG_TIME_VALID | CPUIDLE_FLAG_TLB_FLUSHED,
		.exit_latency = 80,
		.target_residency = 300,
		.enter = &intel_idle },
	{
		.name = "C7-IVB",
		.desc = "MWAIT 0x30",
		.flags = MWAIT2flg(0x30) | CPUIDLE_FLAG_TIME_VALID | CPUIDLE_FLAG_TLB_FLUSHED,
		.exit_latency = 87,
		.target_residency = 300,
		.enter = &intel_idle },
	{
		.enter = NULL }
};

static struct cpuidle_state hsw_cstates[] = {
	{
		.name = "C1-HSW",
		.desc = "MWAIT 0x00",
		.flags = MWAIT2flg(0x00) | CPUIDLE_FLAG_TIME_VALID,
		.exit_latency = 2,
		.target_residency = 2,
		.enter = &intel_idle },
	{
		.name = "C1E-HSW",
		.desc = "MWAIT 0x01",
		.flags = MWAIT2flg(0x01) | CPUIDLE_FLAG_TIME_VALID,
		.exit_latency = 10,
		.target_residency = 20,
		.enter = &intel_idle },
	{
		.name = "C3-HSW",
		.desc = "MWAIT 0x10",
		.flags = MWAIT2flg(0x10) | CPUIDLE_FLAG_TIME_VALID | CPUIDLE_FLAG_TLB_FLUSHED,
		.exit_latency = 33,
		.target_residency = 100,
		.enter = &intel_idle },
	{
		.name = "C6-HSW",
		.desc = "MWAIT 0x20",
		.flags = MWAIT2flg(0x20) | CPUIDLE_FLAG_TIME_VALID | CPUIDLE_FLAG_TLB_FLUSHED,
		.exit_latency = 133,
		.target_residency = 400,
		.enter = &intel_idle },
	{
		.name = "C7s-HSW",
		.desc = "MWAIT 0x32",
		.flags = MWAIT2flg(0x32) | CPUIDLE_FLAG_TIME_VALID | CPUIDLE_FLAG_TLB_FLUSHED,
		.exit_latency = 166,
		.target_residency = 500,
		.enter = &intel_idle },
	{
		.name = "C8-HSW",
		.desc = "MWAIT 0x40",
		.flags = MWAIT2flg(0x40) | CPUIDLE_FLAG_TIME_VALID | CPUIDLE_FLAG_TLB_FLUSHED,
		.exit_latency = 300,
		.target_residency = 900,
		.enter = &intel_idle },
	{
		.name = "C9-HSW",
		.desc = "MWAIT 0x50",
		.flags = MWAIT2flg(0x50) | CPUIDLE_FLAG_TIME_VALID | CPUIDLE_FLAG_TLB_FLUSHED,
		.exit_latency = 600,
		.target_residency = 1800,
		.enter = &intel_idle },
	{
		.name = "C10-HSW",
		.desc = "MWAIT 0x60",
		.flags = MWAIT2flg(0x60) | CPUIDLE_FLAG_TIME_VALID | CPUIDLE_FLAG_TLB_FLUSHED,
		.exit_latency = 2600,
		.target_residency = 7700,
		.enter = &intel_idle },
	{
		.enter = NULL }
};

static struct cpuidle_state atom_cstates[] = {
	{
		.name = "C1E-ATM",
		.desc = "MWAIT 0x00",
		.flags = MWAIT2flg(0x00) | CPUIDLE_FLAG_TIME_VALID,
		.exit_latency = 10,
		.target_residency = 20,
		.enter = &intel_idle },
	{
		.name = "C2-ATM",
		.desc = "MWAIT 0x10",
		.flags = MWAIT2flg(0x10) | CPUIDLE_FLAG_TIME_VALID,
		.exit_latency = 20,
		.target_residency = 80,
		.enter = &intel_idle },
	{
		.name = "C4-ATM",
		.desc = "MWAIT 0x30",
		.flags = MWAIT2flg(0x30) | CPUIDLE_FLAG_TIME_VALID | CPUIDLE_FLAG_TLB_FLUSHED,
		.exit_latency = 100,
		.target_residency = 400,
		.enter = &intel_idle },
	{
		.name = "C6-ATM",
		.desc = "MWAIT 0x52",
		.flags = MWAIT2flg(0x52) | CPUIDLE_FLAG_TIME_VALID | CPUIDLE_FLAG_TLB_FLUSHED,
		.exit_latency = 140,
		.target_residency = 560,
		.enter = &intel_idle },
	{
		.enter = NULL }
};
static struct cpuidle_state avn_cstates[] = {
	{
		.name = "C1-AVN",
		.desc = "MWAIT 0x00",
		.flags = MWAIT2flg(0x00) | CPUIDLE_FLAG_TIME_VALID,
		.exit_latency = 2,
		.target_residency = 2,
		.enter = &intel_idle },
	{
		.name = "C6-AVN",
		.desc = "MWAIT 0x51",
		.flags = MWAIT2flg(0x51) | CPUIDLE_FLAG_TIME_VALID | CPUIDLE_FLAG_TLB_FLUSHED,
		.exit_latency = 15,
		.target_residency = 45,
		.enter = &intel_idle },
	{
		.enter = NULL }
};
>>>>>>> refs/remotes/origin/master

/**
 * intel_idle
 * @dev: cpuidle_device
 * @drv: cpuidle driver
 * @index: index of cpuidle state
 *
 * Must be called under local_irq_disable().
 */
static int intel_idle(struct cpuidle_device *dev,
		struct cpuidle_driver *drv, int index)
{
	unsigned long ecx = 1; /* break on interrupt flag */
	struct cpuidle_state *state = &drv->states[index];
<<<<<<< HEAD
	struct cpuidle_state_usage *state_usage = &dev->states_usage[index];
	unsigned long eax = (unsigned long)cpuidle_get_statedata(state_usage);
>>>>>>> refs/remotes/origin/cm-10.0
	unsigned int cstate;
	ktime_t kt_before, kt_after;
	s64 usec_delta;
=======
	unsigned long eax = flg2MWAIT(state->flags);
	unsigned int cstate;
>>>>>>> refs/remotes/origin/master
	int cpu = smp_processor_id();

	cstate = (((eax) >> MWAIT_SUBSTATE_SIZE) & MWAIT_CSTATE_MASK) + 1;

<<<<<<< HEAD
<<<<<<< HEAD
	local_irq_disable();

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * leave_mm() to avoid costly and often unnecessary wakeups
	 * for flushing the user TLB's associated with the active mm.
	 */
	if (state->flags & CPUIDLE_FLAG_TLB_FLUSHED)
		leave_mm(cpu);

	if (!(lapic_timer_reliable_states & (1 << (cstate))))
		clockevents_notify(CLOCK_EVT_NOTIFY_BROADCAST_ENTER, &cpu);

<<<<<<< HEAD
	kt_before = ktime_get_real();

	stop_critical_timings();
	if (!need_resched()) {

		__monitor((void *)&current_thread_info()->flags, 0, 0);
		smp_mb();
		if (!need_resched())
			__mwait(eax, ecx);
	}

	start_critical_timings();

	kt_after = ktime_get_real();
	usec_delta = ktime_to_us(ktime_sub(kt_after, kt_before));

	local_irq_enable();
=======
	mwait_idle_with_hints(eax, ecx);
>>>>>>> refs/remotes/origin/master

	if (!(lapic_timer_reliable_states & (1 << (cstate))))
		clockevents_notify(CLOCK_EVT_NOTIFY_BROADCAST_EXIT, &cpu);

<<<<<<< HEAD
<<<<<<< HEAD
	return usec_delta;
=======
	/* Update cpuidle counters */
	dev->last_residency = (int)usec_delta;

	return index;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return index;
>>>>>>> refs/remotes/origin/master
}

static void __setup_broadcast_timer(void *arg)
{
	unsigned long reason = (unsigned long)arg;
	int cpu = smp_processor_id();

	reason = reason ?
		CLOCK_EVT_NOTIFY_BROADCAST_ON : CLOCK_EVT_NOTIFY_BROADCAST_OFF;

	clockevents_notify(reason, &cpu);
}

<<<<<<< HEAD
static int setup_broadcast_cpuhp_notify(struct notifier_block *n,
		unsigned long action, void *hcpu)
{
	int hotcpu = (unsigned long)hcpu;

	switch (action & 0xf) {
	case CPU_ONLINE:
		smp_call_function_single(hotcpu, __setup_broadcast_timer,
			(void *)true, 1);
=======
static int cpu_hotplug_notify(struct notifier_block *n,
			      unsigned long action, void *hcpu)
{
	int hotcpu = (unsigned long)hcpu;
	struct cpuidle_device *dev;

	switch (action & ~CPU_TASKS_FROZEN) {
	case CPU_ONLINE:

		if (lapic_timer_reliable_states != LAPIC_TIMER_ALWAYS_RELIABLE)
			smp_call_function_single(hotcpu, __setup_broadcast_timer,
						 (void *)true, 1);

		/*
		 * Some systems can hotplug a cpu at runtime after
		 * the kernel has booted, we have to initialize the
		 * driver in this case
		 */
		dev = per_cpu_ptr(intel_idle_cpuidle_devices, hotcpu);
		if (!dev->registered)
			intel_idle_cpu_init(hotcpu);

>>>>>>> refs/remotes/origin/master
		break;
	}
	return NOTIFY_OK;
}

<<<<<<< HEAD
static struct notifier_block setup_broadcast_notifier = {
	.notifier_call = setup_broadcast_cpuhp_notify,
=======
static struct notifier_block cpu_hotplug_notifier = {
	.notifier_call = cpu_hotplug_notify,
>>>>>>> refs/remotes/origin/master
};

static void auto_demotion_disable(void *dummy)
{
	unsigned long long msr_bits;

	rdmsrl(MSR_NHM_SNB_PKG_CST_CFG_CTL, msr_bits);
<<<<<<< HEAD
<<<<<<< HEAD
	msr_bits &= ~auto_demotion_disable_flags;
	wrmsrl(MSR_NHM_SNB_PKG_CST_CFG_CTL, msr_bits);
}

<<<<<<< HEAD
=======
=======
	msr_bits &= ~(icpu->auto_demotion_disable_flags);
	wrmsrl(MSR_NHM_SNB_PKG_CST_CFG_CTL, msr_bits);
}

>>>>>>> refs/remotes/origin/cm-10.0
static const struct idle_cpu idle_cpu_nehalem = {
	.state_table = nehalem_cstates,
	.auto_demotion_disable_flags = NHM_C1_AUTO_DEMOTE | NHM_C3_AUTO_DEMOTE,
=======
	msr_bits &= ~(icpu->auto_demotion_disable_flags);
	wrmsrl(MSR_NHM_SNB_PKG_CST_CFG_CTL, msr_bits);
}
static void c1e_promotion_disable(void *dummy)
{
	unsigned long long msr_bits;

	rdmsrl(MSR_IA32_POWER_CTL, msr_bits);
	msr_bits &= ~0x2;
	wrmsrl(MSR_IA32_POWER_CTL, msr_bits);
}

static const struct idle_cpu idle_cpu_nehalem = {
	.state_table = nehalem_cstates,
	.auto_demotion_disable_flags = NHM_C1_AUTO_DEMOTE | NHM_C3_AUTO_DEMOTE,
	.disable_promotion_to_c1e = true,
>>>>>>> refs/remotes/origin/master
};

static const struct idle_cpu idle_cpu_atom = {
	.state_table = atom_cstates,
};

static const struct idle_cpu idle_cpu_lincroft = {
	.state_table = atom_cstates,
	.auto_demotion_disable_flags = ATM_LNC_C6_AUTO_DEMOTE,
};

static const struct idle_cpu idle_cpu_snb = {
	.state_table = snb_cstates,
<<<<<<< HEAD
};

<<<<<<< HEAD
static const struct idle_cpu idle_cpu_ivb = {
	.state_table = ivb_cstates,
};

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.disable_promotion_to_c1e = true,
};

static const struct idle_cpu idle_cpu_ivb = {
	.state_table = ivb_cstates,
<<<<<<< HEAD
	.disable_promotion_to_c1e = true,
};

static const struct idle_cpu idle_cpu_hsw = {
	.state_table = hsw_cstates,
	.disable_promotion_to_c1e = true,
};

static const struct idle_cpu idle_cpu_avn = {
	.state_table = avn_cstates,
	.disable_promotion_to_c1e = true,
};

>>>>>>> refs/remotes/origin/master
=======
};

>>>>>>> refs/remotes/origin/cm-11.0
#define ICPU(model, cpu) \
	{ X86_VENDOR_INTEL, 6, model, X86_FEATURE_MWAIT, (unsigned long)&cpu }

static const struct x86_cpu_id intel_idle_ids[] = {
	ICPU(0x1a, idle_cpu_nehalem),
	ICPU(0x1e, idle_cpu_nehalem),
	ICPU(0x1f, idle_cpu_nehalem),
	ICPU(0x25, idle_cpu_nehalem),
	ICPU(0x2c, idle_cpu_nehalem),
	ICPU(0x2e, idle_cpu_nehalem),
	ICPU(0x1c, idle_cpu_atom),
	ICPU(0x26, idle_cpu_lincroft),
	ICPU(0x2f, idle_cpu_nehalem),
	ICPU(0x2a, idle_cpu_snb),
	ICPU(0x2d, idle_cpu_snb),
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	ICPU(0x3a, idle_cpu_ivb),
	ICPU(0x3e, idle_cpu_ivb),
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ICPU(0x3a, idle_cpu_ivb),
	ICPU(0x3e, idle_cpu_ivb),
	ICPU(0x3c, idle_cpu_hsw),
	ICPU(0x3f, idle_cpu_hsw),
	ICPU(0x45, idle_cpu_hsw),
	ICPU(0x46, idle_cpu_hsw),
	ICPU(0x4D, idle_cpu_avn),
>>>>>>> refs/remotes/origin/master
=======
	ICPU(0x3a, idle_cpu_ivb),
	ICPU(0x3e, idle_cpu_ivb),
>>>>>>> refs/remotes/origin/cm-11.0
	{}
};
MODULE_DEVICE_TABLE(x86cpu, intel_idle_ids);

<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> fcfe545... Squashed update of kernel from 3.4.75 to 3.4.76
=======
>>>>>>> refs/remotes/origin/cm-10.0
/*
 * intel_idle_probe()
 */
static int intel_idle_probe(void)
{
	unsigned int eax, ebx, ecx;
<<<<<<< HEAD
=======
	const struct x86_cpu_id *id;
>>>>>>> refs/remotes/origin/cm-10.0
=======
/*
 * intel_idle_probe()
 */
static int __init intel_idle_probe(void)
{
	unsigned int eax, ebx, ecx;
	const struct x86_cpu_id *id;
>>>>>>> refs/remotes/origin/master

	if (max_cstate == 0) {
		pr_debug(PREFIX "disabled\n");
		return -EPERM;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	if (boot_cpu_data.x86_vendor != X86_VENDOR_INTEL)
		return -ENODEV;

	if (!boot_cpu_has(X86_FEATURE_MWAIT))
		return -ENODEV;
=======
=======
>>>>>>> refs/remotes/origin/master
	id = x86_match_cpu(intel_idle_ids);
	if (!id) {
		if (boot_cpu_data.x86_vendor == X86_VENDOR_INTEL &&
		    boot_cpu_data.x86 == 6)
			pr_debug(PREFIX "does not run on family %d model %d\n",
				boot_cpu_data.x86, boot_cpu_data.x86_model);
		return -ENODEV;
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	if (boot_cpu_data.cpuid_level < CPUID_MWAIT_LEAF)
		return -ENODEV;

	cpuid(CPUID_MWAIT_LEAF, &eax, &ebx, &ecx, &mwait_substates);

	if (!(ecx & CPUID5_ECX_EXTENSIONS_SUPPORTED) ||
	    !(ecx & CPUID5_ECX_INTERRUPT_BREAK) ||
	    !mwait_substates)
			return -ENODEV;

	pr_debug(PREFIX "MWAIT substates: 0x%x\n", mwait_substates);

<<<<<<< HEAD
<<<<<<< HEAD

	if (boot_cpu_data.x86 != 6)	/* family 6 */
		return -ENODEV;

	switch (boot_cpu_data.x86_model) {

	case 0x1A:	/* Core i7, Xeon 5500 series */
	case 0x1E:	/* Core i7 and i5 Processor - Lynnfield Jasper Forest */
	case 0x1F:	/* Core i7 and i5 Processor - Nehalem */
	case 0x2E:	/* Nehalem-EX Xeon */
	case 0x2F:	/* Westmere-EX Xeon */
	case 0x25:	/* Westmere */
	case 0x2C:	/* Westmere */
		cpuidle_state_table = nehalem_cstates;
		auto_demotion_disable_flags =
			(NHM_C1_AUTO_DEMOTE | NHM_C3_AUTO_DEMOTE);
		break;

	case 0x1C:	/* 28 - Atom Processor */
		cpuidle_state_table = atom_cstates;
		break;

	case 0x26:	/* 38 - Lincroft Atom Processor */
		cpuidle_state_table = atom_cstates;
		auto_demotion_disable_flags = ATM_LNC_C6_AUTO_DEMOTE;
		break;

	case 0x2A:	/* SNB */
	case 0x2D:	/* SNB Xeon */
		cpuidle_state_table = snb_cstates;
		break;

	default:
		pr_debug(PREFIX "does not run on family %d model %d\n",
			boot_cpu_data.x86, boot_cpu_data.x86_model);
		return -ENODEV;
	}
=======
	icpu = (const struct idle_cpu *)id->driver_data;
	cpuidle_state_table = icpu->state_table;
>>>>>>> refs/remotes/origin/cm-10.0

	if (boot_cpu_has(X86_FEATURE_ARAT))	/* Always Reliable APIC Timer */
		lapic_timer_reliable_states = LAPIC_TIMER_ALWAYS_RELIABLE;
	else {
		on_each_cpu(__setup_broadcast_timer, (void *)true, 1);
		register_cpu_notifier(&setup_broadcast_notifier);
	}
=======
	icpu = (const struct idle_cpu *)id->driver_data;
	cpuidle_state_table = icpu->state_table;

	if (boot_cpu_has(X86_FEATURE_ARAT))	/* Always Reliable APIC Timer */
		lapic_timer_reliable_states = LAPIC_TIMER_ALWAYS_RELIABLE;
	else
		on_each_cpu(__setup_broadcast_timer, (void *)true, 1);
>>>>>>> refs/remotes/origin/master

	pr_debug(PREFIX "v" INTEL_IDLE_VERSION
		" model 0x%X\n", boot_cpu_data.x86_model);

	pr_debug(PREFIX "lapic_timer_reliable_states 0x%x\n",
		lapic_timer_reliable_states);
	return 0;
}

/*
 * intel_idle_cpuidle_devices_uninit()
 * unregister, free cpuidle_devices
 */
static void intel_idle_cpuidle_devices_uninit(void)
{
	int i;
	struct cpuidle_device *dev;

	for_each_online_cpu(i) {
		dev = per_cpu_ptr(intel_idle_cpuidle_devices, i);
		cpuidle_unregister_device(dev);
	}

	free_percpu(intel_idle_cpuidle_devices);
	return;
}
/*
<<<<<<< HEAD
<<<<<<< HEAD
 * intel_idle_cpuidle_devices_init()
 * allocate, initialize, register cpuidle_devices
 */
static int intel_idle_cpuidle_devices_init(void)
{
	int i, cstate;
	struct cpuidle_device *dev;

	intel_idle_cpuidle_devices = alloc_percpu(struct cpuidle_device);
	if (intel_idle_cpuidle_devices == NULL)
		return -ENOMEM;

	for_each_online_cpu(i) {
		dev = per_cpu_ptr(intel_idle_cpuidle_devices, i);

		dev->state_count = 1;

		for (cstate = 1; cstate < MWAIT_MAX_NUM_CSTATES; ++cstate) {
			int num_substates;

			if (cstate > max_cstate) {
				printk(PREFIX "max_cstate %d reached\n",
					max_cstate);
				break;
			}

			/* does the state exist in CPUID.MWAIT? */
			num_substates = (mwait_substates >> ((cstate) * 4))
						& MWAIT_SUBSTATE_MASK;
			if (num_substates == 0)
				continue;
			/* is the state not enabled? */
			if (cpuidle_state_table[cstate].enter == NULL) {
				/* does the driver not know about the state? */
				if (*cpuidle_state_table[cstate].name == '\0')
					pr_debug(PREFIX "unaware of model 0x%x"
						" MWAIT %d please"
						" contact lenb@kernel.org",
					boot_cpu_data.x86_model, cstate);
				continue;
			}

			if ((cstate > 2) &&
				!boot_cpu_has(X86_FEATURE_NONSTOP_TSC))
				mark_tsc_unstable("TSC halts in idle"
					" states deeper than C2");

			dev->states[dev->state_count] =	/* structure copy */
				cpuidle_state_table[cstate];

			dev->state_count += 1;
		}

		dev->cpu = i;
		if (cpuidle_register_device(dev)) {
			pr_debug(PREFIX "cpuidle_register_device %d failed!\n",
				 i);
			intel_idle_cpuidle_devices_uninit();
			return -EIO;
		}
	}
	if (auto_demotion_disable_flags)
=======
 * intel_idle_cpuidle_driver_init()
 * allocate, initialize cpuidle_states
 */
static int intel_idle_cpuidle_driver_init(void)
=======
 * intel_idle_cpuidle_driver_init()
 * allocate, initialize cpuidle_states
 */
static int __init intel_idle_cpuidle_driver_init(void)
>>>>>>> refs/remotes/origin/master
{
	int cstate;
	struct cpuidle_driver *drv = &intel_idle_driver;

	drv->state_count = 1;

<<<<<<< HEAD
	for (cstate = 1; cstate < MWAIT_MAX_NUM_CSTATES; ++cstate) {
		int num_substates;

		if (cstate > max_cstate) {
=======
	for (cstate = 0; cstate < CPUIDLE_STATE_MAX; ++cstate) {
		int num_substates, mwait_hint, mwait_cstate, mwait_substate;

		if (cpuidle_state_table[cstate].enter == NULL)
			break;

		if (cstate + 1 > max_cstate) {
>>>>>>> refs/remotes/origin/master
			printk(PREFIX "max_cstate %d reached\n",
				max_cstate);
			break;
		}

<<<<<<< HEAD
		/* does the state exist in CPUID.MWAIT? */
		num_substates = (mwait_substates >> ((cstate) * 4))
					& MWAIT_SUBSTATE_MASK;
		if (num_substates == 0)
			continue;
		/* is the state not enabled? */
		if (cpuidle_state_table[cstate].enter == NULL) {
			/* does the driver not know about the state? */
			if (*cpuidle_state_table[cstate].name == '\0')
				pr_debug(PREFIX "unaware of model 0x%x"
					" MWAIT %d please"
					" contact lenb@kernel.org",
				boot_cpu_data.x86_model, cstate);
			continue;
		}

		if ((cstate > 2) &&
=======
		mwait_hint = flg2MWAIT(cpuidle_state_table[cstate].flags);
		mwait_cstate = MWAIT_HINT2CSTATE(mwait_hint);
		mwait_substate = MWAIT_HINT2SUBSTATE(mwait_hint);

		/* does the state exist in CPUID.MWAIT? */
		num_substates = (mwait_substates >> ((mwait_cstate + 1) * 4))
					& MWAIT_SUBSTATE_MASK;

		/* if sub-state in table is not enumerated by CPUID */
		if ((mwait_substate + 1) > num_substates)
			continue;

		if (((mwait_cstate + 1) > 2) &&
>>>>>>> refs/remotes/origin/master
			!boot_cpu_has(X86_FEATURE_NONSTOP_TSC))
			mark_tsc_unstable("TSC halts in idle"
					" states deeper than C2");

		drv->states[drv->state_count] =	/* structure copy */
			cpuidle_state_table[cstate];

		drv->state_count += 1;
	}

	if (icpu->auto_demotion_disable_flags)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		on_each_cpu(auto_demotion_disable, NULL, 1);

=======
		on_each_cpu(auto_demotion_disable, NULL, 1);

	if (icpu->disable_promotion_to_c1e)	/* each-cpu is redundant */
		on_each_cpu(c1e_promotion_disable, NULL, 1);

>>>>>>> refs/remotes/origin/master
	return 0;
}


<<<<<<< HEAD
<<<<<<< HEAD
static int __init intel_idle_init(void)
{
	int retval;
=======
=======
>>>>>>> refs/remotes/origin/master
/*
 * intel_idle_cpu_init()
 * allocate, initialize, register cpuidle_devices
 * @cpu: cpu/core to initialize
 */
<<<<<<< HEAD
int intel_idle_cpu_init(int cpu)
=======
static int intel_idle_cpu_init(int cpu)
>>>>>>> refs/remotes/origin/master
{
	int cstate;
	struct cpuidle_device *dev;

	dev = per_cpu_ptr(intel_idle_cpuidle_devices, cpu);

	dev->state_count = 1;

<<<<<<< HEAD
	for (cstate = 1; cstate < MWAIT_MAX_NUM_CSTATES; ++cstate) {
		int num_substates;

		if (cstate > max_cstate) {
=======
	for (cstate = 0; cstate < CPUIDLE_STATE_MAX; ++cstate) {
		int num_substates, mwait_hint, mwait_cstate, mwait_substate;

		if (cpuidle_state_table[cstate].enter == NULL)
			break;

		if (cstate + 1 > max_cstate) {
>>>>>>> refs/remotes/origin/master
			printk(PREFIX "max_cstate %d reached\n", max_cstate);
			break;
		}

<<<<<<< HEAD
		/* does the state exist in CPUID.MWAIT? */
		num_substates = (mwait_substates >> ((cstate) * 4))
			& MWAIT_SUBSTATE_MASK;
		if (num_substates == 0)
			continue;
		/* is the state not enabled? */
		if (cpuidle_state_table[cstate].enter == NULL)
			continue;

		dev->states_usage[dev->state_count].driver_data =
			(void *)get_driver_data(cstate);
=======
		mwait_hint = flg2MWAIT(cpuidle_state_table[cstate].flags);
		mwait_cstate = MWAIT_HINT2CSTATE(mwait_hint);
		mwait_substate = MWAIT_HINT2SUBSTATE(mwait_hint);

		/* does the state exist in CPUID.MWAIT? */
		num_substates = (mwait_substates >> ((mwait_cstate + 1) * 4))
					& MWAIT_SUBSTATE_MASK;

		/* if sub-state in table is not enumerated by CPUID */
		if ((mwait_substate + 1) > num_substates)
			continue;
>>>>>>> refs/remotes/origin/master

		dev->state_count += 1;
	}

	dev->cpu = cpu;

	if (cpuidle_register_device(dev)) {
		pr_debug(PREFIX "cpuidle_register_device %d failed!\n", cpu);
		intel_idle_cpuidle_devices_uninit();
		return -EIO;
	}

	if (icpu->auto_demotion_disable_flags)
		smp_call_function_single(cpu, auto_demotion_disable, NULL, 1);

	return 0;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(intel_idle_cpu_init);
=======
>>>>>>> refs/remotes/origin/master

static int __init intel_idle_init(void)
{
	int retval, i;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	/* Do not load intel_idle at all for now if idle= is passed */
	if (boot_option_idle_override != IDLE_NO_OVERRIDE)
		return -ENODEV;

	retval = intel_idle_probe();
	if (retval)
		return retval;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	intel_idle_cpuidle_driver_init();
>>>>>>> refs/remotes/origin/cm-10.0
	retval = cpuidle_register_driver(&intel_idle_driver);
	if (retval) {
		printk(KERN_DEBUG PREFIX "intel_idle yielding to %s",
			cpuidle_get_driver()->name);
		return retval;
	}

<<<<<<< HEAD
	retval = intel_idle_cpuidle_devices_init();
	if (retval) {
		cpuidle_unregister_driver(&intel_idle_driver);
		return retval;
=======
=======
	intel_idle_cpuidle_driver_init();
	retval = cpuidle_register_driver(&intel_idle_driver);
	if (retval) {
		struct cpuidle_driver *drv = cpuidle_get_driver();
		printk(KERN_DEBUG PREFIX "intel_idle yielding to %s",
			drv ? drv->name : "none");
		return retval;
	}

>>>>>>> refs/remotes/origin/master
	intel_idle_cpuidle_devices = alloc_percpu(struct cpuidle_device);
	if (intel_idle_cpuidle_devices == NULL)
		return -ENOMEM;

	for_each_online_cpu(i) {
		retval = intel_idle_cpu_init(i);
		if (retval) {
			cpuidle_unregister_driver(&intel_idle_driver);
			return retval;
		}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	}
=======
	}
	register_cpu_notifier(&cpu_hotplug_notifier);
>>>>>>> refs/remotes/origin/master

	return 0;
}

static void __exit intel_idle_exit(void)
{
	intel_idle_cpuidle_devices_uninit();
	cpuidle_unregister_driver(&intel_idle_driver);

<<<<<<< HEAD
	if (lapic_timer_reliable_states != LAPIC_TIMER_ALWAYS_RELIABLE) {
		on_each_cpu(__setup_broadcast_timer, (void *)false, 1);
		unregister_cpu_notifier(&setup_broadcast_notifier);
	}
=======

	if (lapic_timer_reliable_states != LAPIC_TIMER_ALWAYS_RELIABLE)
		on_each_cpu(__setup_broadcast_timer, (void *)false, 1);
	unregister_cpu_notifier(&cpu_hotplug_notifier);
>>>>>>> refs/remotes/origin/master

	return;
}

module_init(intel_idle_init);
module_exit(intel_idle_exit);

module_param(max_cstate, int, 0444);

MODULE_AUTHOR("Len Brown <len.brown@intel.com>");
MODULE_DESCRIPTION("Cpuidle driver for Intel Hardware v" INTEL_IDLE_VERSION);
MODULE_LICENSE("GPL");
