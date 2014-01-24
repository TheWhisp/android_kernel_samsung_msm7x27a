/*
<<<<<<< HEAD
 *    Copyright IBM Corp. 1999,2009
=======
 *    Copyright IBM Corp. 1999,2012
>>>>>>> refs/remotes/origin/cm-10.0
 *    Author(s): Denis Joseph Barrow,
 *		 Martin Schwidefsky <schwidefsky@de.ibm.com>,
 *		 Heiko Carstens <heiko.carstens@de.ibm.com>,
 */
#ifndef __ASM_SMP_H
#define __ASM_SMP_H

#ifdef CONFIG_SMP

<<<<<<< HEAD
#include <asm/system.h>
#include <asm/sigp.h>

extern void machine_restart_smp(char *);
extern void machine_halt_smp(void);
extern void machine_power_off_smp(void);

#define raw_smp_processor_id()	(S390_lowcore.cpu_nr)

extern int __cpu_disable (void);
extern void __cpu_die (unsigned int cpu);
extern int __cpu_up (unsigned int cpu);

extern struct mutex smp_cpu_state_mutex;
extern int smp_cpu_polarization[];
=======
#include <asm/lowcore.h>

#define raw_smp_processor_id()	(S390_lowcore.cpu_nr)

extern struct mutex smp_cpu_state_mutex;
extern struct save_area *zfcpdump_save_areas[NR_CPUS + 1];

extern int __cpu_up(unsigned int cpu);
>>>>>>> refs/remotes/origin/cm-10.0

extern void arch_send_call_function_single_ipi(int cpu);
extern void arch_send_call_function_ipi_mask(const struct cpumask *mask);

<<<<<<< HEAD
extern struct save_area *zfcpdump_save_areas[NR_CPUS + 1];

extern void smp_switch_to_ipl_cpu(void (*func)(void *), void *);
extern void smp_switch_to_cpu(void (*)(void *), void *, unsigned long sp,
			      int from, int to);
extern void smp_restart_cpu(void);

/*
 * returns 1 if (virtual) cpu is scheduled
 * returns 0 otherwise
 */
static inline int smp_vcpu_scheduled(int cpu)
{
	u32 status;

	switch (sigp_ps(&status, 0, cpu, sigp_sense_running)) {
	case sigp_status_stored:
		/* Check for running status */
		if (status & 0x400)
			return 0;
		break;
	case sigp_not_operational:
		return 0;
	default:
		break;
	}
	return 1;
}

#else /* CONFIG_SMP */

static inline void smp_switch_to_ipl_cpu(void (*func)(void *), void *data)
=======
extern void smp_call_online_cpu(void (*func)(void *), void *);
extern void smp_call_ipl_cpu(void (*func)(void *), void *);

extern int smp_find_processor_id(u16 address);
extern int smp_store_status(int cpu);
extern int smp_vcpu_scheduled(int cpu);
extern void smp_yield_cpu(int cpu);
extern void smp_yield(void);
extern void smp_stop_cpu(void);

#else /* CONFIG_SMP */

static inline void smp_call_ipl_cpu(void (*func)(void *), void *data)
{
	func(data);
}

static inline void smp_call_online_cpu(void (*func)(void *), void *data)
>>>>>>> refs/remotes/origin/cm-10.0
{
	func(data);
}

<<<<<<< HEAD
#define smp_vcpu_scheduled	(1)
=======
static inline int smp_find_processor_id(int address) { return 0; }
static inline int smp_vcpu_scheduled(int cpu) { return 1; }
static inline void smp_yield_cpu(int cpu) { }
static inline void smp_yield(void) { }
static inline void smp_stop_cpu(void) { }
>>>>>>> refs/remotes/origin/cm-10.0

#endif /* CONFIG_SMP */

#ifdef CONFIG_HOTPLUG_CPU
extern int smp_rescan_cpus(void);
extern void __noreturn cpu_die(void);
<<<<<<< HEAD
=======
extern void __cpu_die(unsigned int cpu);
extern int __cpu_disable(void);
>>>>>>> refs/remotes/origin/cm-10.0
#else
static inline int smp_rescan_cpus(void) { return 0; }
static inline void cpu_die(void) { }
#endif

#endif /* __ASM_SMP_H */
