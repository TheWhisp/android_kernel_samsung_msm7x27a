/*
 *    Precise Delay Loops for S390
 *
<<<<<<< HEAD
 *    Copyright IBM Corp. 1999,2008
=======
 *    Copyright IBM Corp. 1999, 2008
>>>>>>> refs/remotes/origin/master
 *    Author(s): Martin Schwidefsky <schwidefsky@de.ibm.com>,
 *		 Heiko Carstens <heiko.carstens@de.ibm.com>,
 */

#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/timex.h>
#include <linux/module.h>
#include <linux/irqflags.h>
#include <linux/interrupt.h>
<<<<<<< HEAD
#include <asm/div64.h>
<<<<<<< HEAD
=======
#include <asm/timer.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <asm/vtimer.h>
#include <asm/div64.h>
>>>>>>> refs/remotes/origin/master

void __delay(unsigned long loops)
{
        /*
         * To end the bloody studid and useless discussion about the
         * BogoMips number I took the liberty to define the __delay
         * function in a way that that resulting BogoMips number will
         * yield the megahertz number of the cpu. The important function
         * is udelay and that is done using the tod clock. -- martin.
         */
	asm volatile("0: brct %0,0b" : : "d" ((loops/2) + 1));
}

static void __udelay_disabled(unsigned long long usecs)
{
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long mask, cr0, cr0_saved;
	u64 clock_saved;
	u64 end;

	mask = psw_kernel_bits | PSW_MASK_WAIT | PSW_MASK_EXT;
	end = get_clock() + (usecs << 12);
	clock_saved = local_tick_disable();
	__ctl_store(cr0_saved, 0, 0);
	cr0 = (cr0_saved & 0xffff00e0) | 0x00000800;
	__ctl_load(cr0 , 0, 0);
	lockdep_off();
	do {
		set_clock_comparator(end);
		trace_hardirqs_on();
		__load_psw_mask(mask);
		local_irq_disable();
	} while (get_clock() < end);
	lockdep_on();
	__ctl_load(cr0_saved, 0, 0);
=======
	unsigned long cr0, cr6, new;
	u64 clock_saved, end;

	end = get_clock() + (usecs << 12);
=======
	unsigned long cr0, cr6, new;
	u64 clock_saved, end;

	end = get_tod_clock() + (usecs << 12);
>>>>>>> refs/remotes/origin/master
	clock_saved = local_tick_disable();
	__ctl_store(cr0, 0, 0);
	__ctl_store(cr6, 6, 6);
	new = (cr0 &  0xffff00e0) | 0x00000800;
	__ctl_load(new , 0, 0);
	new = 0;
	__ctl_load(new, 6, 6);
	lockdep_off();
	do {
		set_clock_comparator(end);
		vtime_stop_cpu();
<<<<<<< HEAD
		local_irq_disable();
	} while (get_clock() < end);
	lockdep_on();
	__ctl_load(cr0, 0, 0);
	__ctl_load(cr6, 6, 6);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	} while (get_tod_clock_fast() < end);
	lockdep_on();
	__ctl_load(cr0, 0, 0);
	__ctl_load(cr6, 6, 6);
>>>>>>> refs/remotes/origin/master
	local_tick_enable(clock_saved);
}

static void __udelay_enabled(unsigned long long usecs)
{
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long mask;
	u64 clock_saved;
	u64 end;

	mask = psw_kernel_bits | PSW_MASK_WAIT | PSW_MASK_EXT | PSW_MASK_IO;
=======
	u64 clock_saved, end;

>>>>>>> refs/remotes/origin/cm-10.0
	end = get_clock() + (usecs << 12);
=======
	u64 clock_saved, end;

	end = get_tod_clock_fast() + (usecs << 12);
>>>>>>> refs/remotes/origin/master
	do {
		clock_saved = 0;
		if (end < S390_lowcore.clock_comparator) {
			clock_saved = local_tick_disable();
			set_clock_comparator(end);
		}
<<<<<<< HEAD
<<<<<<< HEAD
		trace_hardirqs_on();
		__load_psw_mask(mask);
=======
		vtime_stop_cpu();
>>>>>>> refs/remotes/origin/cm-10.0
		local_irq_disable();
		if (clock_saved)
			local_tick_enable(clock_saved);
	} while (get_clock() < end);
=======
		vtime_stop_cpu();
		if (clock_saved)
			local_tick_enable(clock_saved);
	} while (get_tod_clock_fast() < end);
>>>>>>> refs/remotes/origin/master
}

/*
 * Waits for 'usecs' microseconds using the TOD clock comparator.
 */
void __udelay(unsigned long long usecs)
{
	unsigned long flags;

	preempt_disable();
	local_irq_save(flags);
	if (in_irq()) {
		__udelay_disabled(usecs);
		goto out;
	}
	if (in_softirq()) {
		if (raw_irqs_disabled_flags(flags))
			__udelay_disabled(usecs);
		else
			__udelay_enabled(usecs);
		goto out;
	}
	if (raw_irqs_disabled_flags(flags)) {
		local_bh_disable();
		__udelay_disabled(usecs);
		_local_bh_enable();
		goto out;
	}
	__udelay_enabled(usecs);
out:
	local_irq_restore(flags);
	preempt_enable();
}
EXPORT_SYMBOL(__udelay);

/*
 * Simple udelay variant. To be used on startup and reboot
 * when the interrupt handler isn't working.
 */
void udelay_simple(unsigned long long usecs)
{
	u64 end;

<<<<<<< HEAD
	end = get_clock() + (usecs << 12);
	while (get_clock() < end)
=======
	end = get_tod_clock_fast() + (usecs << 12);
	while (get_tod_clock_fast() < end)
>>>>>>> refs/remotes/origin/master
		cpu_relax();
}

void __ndelay(unsigned long long nsecs)
{
	u64 end;

	nsecs <<= 9;
	do_div(nsecs, 125);
<<<<<<< HEAD
	end = get_clock() + nsecs;
	if (nsecs & ~0xfffUL)
		__udelay(nsecs >> 12);
	while (get_clock() < end)
=======
	end = get_tod_clock_fast() + nsecs;
	if (nsecs & ~0xfffUL)
		__udelay(nsecs >> 12);
	while (get_tod_clock_fast() < end)
>>>>>>> refs/remotes/origin/master
		barrier();
}
EXPORT_SYMBOL(__ndelay);
