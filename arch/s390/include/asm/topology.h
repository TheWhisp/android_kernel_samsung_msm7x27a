#ifndef _ASM_S390_TOPOLOGY_H
#define _ASM_S390_TOPOLOGY_H

#include <linux/cpumask.h>
<<<<<<< HEAD
#include <asm/sysinfo.h>

<<<<<<< HEAD
=======
=======

struct sysinfo_15_1_x;
>>>>>>> refs/remotes/origin/master
struct cpu;

#ifdef CONFIG_SCHED_BOOK

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
extern unsigned char cpu_core_id[NR_CPUS];
extern cpumask_t cpu_core_map[NR_CPUS];

static inline const struct cpumask *cpu_coregroup_mask(int cpu)
{
	return &cpu_core_map[cpu];
}

#define topology_core_id(cpu)		(cpu_core_id[cpu])
#define topology_core_cpumask(cpu)	(&cpu_core_map[cpu])
#define mc_capable()			(1)

<<<<<<< HEAD
#ifdef CONFIG_SCHED_BOOK

=======
>>>>>>> refs/remotes/origin/cm-10.0
extern unsigned char cpu_book_id[NR_CPUS];
extern cpumask_t cpu_book_map[NR_CPUS];

static inline const struct cpumask *cpu_book_mask(int cpu)
{
	return &cpu_book_map[cpu];
}

#define topology_book_id(cpu)		(cpu_book_id[cpu])
#define topology_book_cpumask(cpu)	(&cpu_book_map[cpu])

<<<<<<< HEAD
#endif /* CONFIG_SCHED_BOOK */

int topology_set_cpu_management(int fc);
void topology_schedule_update(void);
void store_topology(struct sysinfo_15_1_x *info);

#define POLARIZATION_UNKNWN	(-1)
=======
=======
struct cpu_topology_s390 {
	unsigned short core_id;
	unsigned short socket_id;
	unsigned short book_id;
	cpumask_t core_mask;
	cpumask_t book_mask;
};

extern struct cpu_topology_s390 cpu_topology[NR_CPUS];

#define topology_physical_package_id(cpu)	(cpu_topology[cpu].socket_id)
#define topology_core_id(cpu)			(cpu_topology[cpu].core_id)
#define topology_core_cpumask(cpu)		(&cpu_topology[cpu].core_mask)
#define topology_book_id(cpu)			(cpu_topology[cpu].book_id)
#define topology_book_cpumask(cpu)		(&cpu_topology[cpu].book_mask)

#define mc_capable() 1

static inline const struct cpumask *cpu_coregroup_mask(int cpu)
{
	return &cpu_topology[cpu].core_mask;
}

static inline const struct cpumask *cpu_book_mask(int cpu)
{
	return &cpu_topology[cpu].book_mask;
}

>>>>>>> refs/remotes/origin/master
int topology_cpu_init(struct cpu *);
int topology_set_cpu_management(int fc);
void topology_schedule_update(void);
void store_topology(struct sysinfo_15_1_x *info);
void topology_expect_change(void);

#else /* CONFIG_SCHED_BOOK */

static inline void topology_schedule_update(void) { }
static inline int topology_cpu_init(struct cpu *cpu) { return 0; }
static inline void topology_expect_change(void) { }

#endif /* CONFIG_SCHED_BOOK */

#define POLARIZATION_UNKNOWN	(-1)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#define POLARIZATION_HRZ	(0)
#define POLARIZATION_VL		(1)
#define POLARIZATION_VM		(2)
#define POLARIZATION_VH		(3)

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_SMP
=======
extern int cpu_polarization[];

static inline void cpu_set_polarization(int cpu, int val)
{
#ifdef CONFIG_SCHED_BOOK
	cpu_polarization[cpu] = val;
#endif
}

static inline int cpu_read_polarization(int cpu)
{
#ifdef CONFIG_SCHED_BOOK
	return cpu_polarization[cpu];
#else
	return POLARIZATION_HRZ;
#endif
}

#ifdef CONFIG_SCHED_BOOK
>>>>>>> refs/remotes/origin/cm-10.0
=======
#ifdef CONFIG_SCHED_BOOK
>>>>>>> refs/remotes/origin/master
void s390_init_cpu_topology(void);
#else
static inline void s390_init_cpu_topology(void)
{
};
#endif

#define SD_BOOK_INIT	SD_CPU_INIT

#include <asm-generic/topology.h>

#endif /* _ASM_S390_TOPOLOGY_H */
