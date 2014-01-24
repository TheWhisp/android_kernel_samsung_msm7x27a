/*
 * Count register synchronisation.
 *
 * All CPUs will have their count registers synchronised to the CPU0 next time
 * value. This can cause a small timewarp for CPU0. All other CPU's should
 * not have done anything significant (but they may have had interrupts
 * enabled briefly - prom_smp_finish() should not be responsible for enabling
 * interrupts...)
 *
 * FIXME: broken for SMTC
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/irqflags.h>
#include <linux/cpumask.h>

#include <asm/r4k-timer.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/atomic.h>
=======
#include <linux/atomic.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <asm/barrier.h>
#include <asm/mipsregs.h>

static atomic_t __cpuinitdata count_start_flag = ATOMIC_INIT(0);
static atomic_t __cpuinitdata count_count_start = ATOMIC_INIT(0);
static atomic_t __cpuinitdata count_count_stop = ATOMIC_INIT(0);
static atomic_t __cpuinitdata count_reference = ATOMIC_INIT(0);

#define COUNTON	100
#define NR_LOOPS 5

void __cpuinit synchronise_count_master(void)
=======
#include <linux/atomic.h>
#include <asm/barrier.h>
#include <asm/mipsregs.h>

static atomic_t count_start_flag = ATOMIC_INIT(0);
static atomic_t count_count_start = ATOMIC_INIT(0);
static atomic_t count_count_stop = ATOMIC_INIT(0);
static atomic_t count_reference = ATOMIC_INIT(0);

#define COUNTON 100
#define NR_LOOPS 5

void synchronise_count_master(int cpu)
>>>>>>> refs/remotes/origin/master
{
	int i;
	unsigned long flags;
	unsigned int initcount;
<<<<<<< HEAD
	int nslaves;
=======
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_MIPS_MT_SMTC
	/*
	 * SMTC needs to synchronise per VPE, not per CPU
	 * ignore for now
	 */
	return;
#endif

<<<<<<< HEAD
	printk(KERN_INFO "Synchronize counters across %u CPUs: ",
	       num_online_cpus());
=======
	printk(KERN_INFO "Synchronize counters for CPU %u: ", cpu);
>>>>>>> refs/remotes/origin/master

	local_irq_save(flags);

	/*
	 * Notify the slaves that it's time to start
	 */
	atomic_set(&count_reference, read_c0_count());
<<<<<<< HEAD
	atomic_set(&count_start_flag, 1);
=======
	atomic_set(&count_start_flag, cpu);
>>>>>>> refs/remotes/origin/master
	smp_wmb();

	/* Count will be initialised to current timer for all CPU's */
	initcount = read_c0_count();

	/*
	 * We loop a few times to get a primed instruction cache,
	 * then the last pass is more or less synchronised and
	 * the master and slaves each set their cycle counters to a known
	 * value all at once. This reduces the chance of having random offsets
	 * between the processors, and guarantees that the maximum
	 * delay between the cycle counters is never bigger than
	 * the latency of information-passing (cachelines) between
	 * two CPUs.
	 */

<<<<<<< HEAD
	nslaves = num_online_cpus()-1;
	for (i = 0; i < NR_LOOPS; i++) {
		/* slaves loop on '!= ncpus' */
		while (atomic_read(&count_count_start) != nslaves)
=======
	for (i = 0; i < NR_LOOPS; i++) {
		/* slaves loop on '!= 2' */
		while (atomic_read(&count_count_start) != 1)
>>>>>>> refs/remotes/origin/master
			mb();
		atomic_set(&count_count_stop, 0);
		smp_wmb();

		/* this lets the slaves write their count register */
		atomic_inc(&count_count_start);

		/*
		 * Everyone initialises count in the last loop:
		 */
		if (i == NR_LOOPS-1)
			write_c0_count(initcount);

		/*
		 * Wait for all slaves to leave the synchronization point:
		 */
<<<<<<< HEAD
		while (atomic_read(&count_count_stop) != nslaves)
=======
		while (atomic_read(&count_count_stop) != 1)
>>>>>>> refs/remotes/origin/master
			mb();
		atomic_set(&count_count_start, 0);
		smp_wmb();
		atomic_inc(&count_count_stop);
	}
	/* Arrange for an interrupt in a short while */
	write_c0_compare(read_c0_count() + COUNTON);
<<<<<<< HEAD
=======
	atomic_set(&count_start_flag, 0);
>>>>>>> refs/remotes/origin/master

	local_irq_restore(flags);

	/*
	 * i386 code reported the skew here, but the
	 * count registers were almost certainly out of sync
	 * so no point in alarming people
	 */
	printk("done.\n");
}

<<<<<<< HEAD
void __cpuinit synchronise_count_slave(void)
{
	int i;
	unsigned long flags;
	unsigned int initcount;
	int ncpus;
=======
void synchronise_count_slave(int cpu)
{
	int i;
	unsigned int initcount;
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_MIPS_MT_SMTC
	/*
	 * SMTC needs to synchronise per VPE, not per CPU
	 * ignore for now
	 */
	return;
#endif

<<<<<<< HEAD
	local_irq_save(flags);

=======
>>>>>>> refs/remotes/origin/master
	/*
	 * Not every cpu is online at the time this gets called,
	 * so we first wait for the master to say everyone is ready
	 */

<<<<<<< HEAD
	while (!atomic_read(&count_start_flag))
=======
	while (atomic_read(&count_start_flag) != cpu)
>>>>>>> refs/remotes/origin/master
		mb();

	/* Count will be initialised to next expire for all CPU's */
	initcount = atomic_read(&count_reference);

<<<<<<< HEAD
	ncpus = num_online_cpus();
	for (i = 0; i < NR_LOOPS; i++) {
		atomic_inc(&count_count_start);
		while (atomic_read(&count_count_start) != ncpus)
=======
	for (i = 0; i < NR_LOOPS; i++) {
		atomic_inc(&count_count_start);
		while (atomic_read(&count_count_start) != 2)
>>>>>>> refs/remotes/origin/master
			mb();

		/*
		 * Everyone initialises count in the last loop:
		 */
		if (i == NR_LOOPS-1)
			write_c0_count(initcount);

		atomic_inc(&count_count_stop);
<<<<<<< HEAD
		while (atomic_read(&count_count_stop) != ncpus)
=======
		while (atomic_read(&count_count_stop) != 2)
>>>>>>> refs/remotes/origin/master
			mb();
	}
	/* Arrange for an interrupt in a short while */
	write_c0_compare(read_c0_count() + COUNTON);
<<<<<<< HEAD

	local_irq_restore(flags);
=======
>>>>>>> refs/remotes/origin/master
}
#undef NR_LOOPS
