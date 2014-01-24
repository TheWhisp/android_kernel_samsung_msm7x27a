/*
 * turbostat -- show CPU frequency and C-state residency
 * on modern Intel turbo-capable processors.
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright (c) 2010, Intel Corporation.
=======
 * Copyright (c) 2012 Intel Corporation.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Copyright (c) 2013 Intel Corporation.
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

<<<<<<< HEAD
<<<<<<< HEAD
=======
#define _GNU_SOURCE
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define _GNU_SOURCE
#include <asm/msr.h>
>>>>>>> refs/remotes/origin/master
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/time.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <sched.h>
>>>>>>> refs/remotes/origin/cm-10.0

#define MSR_TSC	0x10
#define MSR_NEHALEM_PLATFORM_INFO	0xCE
#define MSR_NEHALEM_TURBO_RATIO_LIMIT	0x1AD
#define MSR_APERF	0xE8
#define MSR_MPERF	0xE7
#define MSR_PKG_C2_RESIDENCY	0x60D	/* SNB only */
#define MSR_PKG_C3_RESIDENCY	0x3F8
#define MSR_PKG_C6_RESIDENCY	0x3F9
#define MSR_PKG_C7_RESIDENCY	0x3FA	/* SNB only */
#define MSR_CORE_C3_RESIDENCY	0x3FC
#define MSR_CORE_C6_RESIDENCY	0x3FD
#define MSR_CORE_C7_RESIDENCY	0x3FE	/* SNB only */
=======
#include <sched.h>
>>>>>>> refs/remotes/origin/master

char *proc_stat = "/proc/stat";
unsigned int interval_sec = 5;	/* set with -i interval_sec */
unsigned int verbose;		/* set with -v */
<<<<<<< HEAD
<<<<<<< HEAD
=======
unsigned int summary_only;	/* set with -s */
>>>>>>> refs/remotes/origin/cm-10.0
=======
unsigned int rapl_verbose;	/* set with -R */
unsigned int thermal_verbose;	/* set with -T */
unsigned int summary_only;	/* set with -s */
>>>>>>> refs/remotes/origin/master
unsigned int skip_c0;
unsigned int skip_c1;
unsigned int do_nhm_cstates;
unsigned int do_snb_cstates;
<<<<<<< HEAD
unsigned int has_aperf;
=======
unsigned int do_c8_c9_c10;
unsigned int do_slm_cstates;
unsigned int use_c1_residency_msr;
unsigned int has_aperf;
unsigned int has_epb;
>>>>>>> refs/remotes/origin/master
unsigned int units = 1000000000;	/* Ghz etc */
unsigned int genuine_intel;
unsigned int has_invariant_tsc;
unsigned int do_nehalem_platform_info;
unsigned int do_nehalem_turbo_ratio_limit;
<<<<<<< HEAD
unsigned int extra_msr_offset;
=======
unsigned int do_ivt_turbo_ratio_limit;
unsigned int extra_msr_offset32;
unsigned int extra_msr_offset64;
unsigned int extra_delta_offset32;
unsigned int extra_delta_offset64;
int do_smi;
>>>>>>> refs/remotes/origin/master
double bclk;
unsigned int show_pkg;
unsigned int show_core;
unsigned int show_cpu;
<<<<<<< HEAD
=======
unsigned int show_pkg_only;
unsigned int show_core_only;
char *output_buffer, *outp;
unsigned int do_rapl;
unsigned int do_dts;
unsigned int do_ptm;
unsigned int tcc_activation_temp;
unsigned int tcc_activation_temp_override;
double rapl_power_units, rapl_energy_units, rapl_time_units;
double rapl_joule_counter_range;

#define RAPL_PKG	(1 << 0)
#define RAPL_CORES	(1 << 1)
#define RAPL_GFX	(1 << 2)
#define RAPL_DRAM	(1 << 3)
#define RAPL_PKG_PERF_STATUS	(1 << 4)
#define RAPL_DRAM_PERF_STATUS	(1 << 5)
#define RAPL_PKG_POWER_INFO	(1 << 6)
#define RAPL_CORE_POLICY	(1 << 7)
#define	TJMAX_DEFAULT	100

#define MAX(a, b) ((a) > (b) ? (a) : (b))
>>>>>>> refs/remotes/origin/master

int aperf_mperf_unstable;
int backwards_count;
char *progname;
<<<<<<< HEAD
<<<<<<< HEAD
int need_reinitialize;

int num_cpus;
=======

int num_cpus;
cpu_set_t *cpu_mask;
size_t cpu_mask_size;
>>>>>>> refs/remotes/origin/cm-10.0

struct counters {
	unsigned long long tsc;		/* per thread */
	unsigned long long aperf;	/* per thread */
	unsigned long long mperf;	/* per thread */
	unsigned long long c1;	/* per thread (calculated) */
	unsigned long long c3;	/* per core */
	unsigned long long c6;	/* per core */
	unsigned long long c7;	/* per core */
	unsigned long long pc2;	/* per package */
	unsigned long long pc3;	/* per package */
	unsigned long long pc6;	/* per package */
	unsigned long long pc7;	/* per package */
	unsigned long long extra_msr;	/* per thread */
	int pkg;
	int core;
	int cpu;
	struct counters *next;
};

struct counters *cnt_even;
struct counters *cnt_odd;
struct counters *cnt_delta;
struct counters *cnt_average;
struct timeval tv_even;
struct timeval tv_odd;
struct timeval tv_delta;

<<<<<<< HEAD
unsigned long long get_msr(int cpu, off_t offset)
{
	ssize_t retval;
	unsigned long long msr;
=======
/*
 * cpu_mask_init(ncpus)
 *
 * allocate and clear cpu_mask
 * set cpu_mask_size
 */
void cpu_mask_init(int ncpus)
{
	cpu_mask = CPU_ALLOC(ncpus);
	if (cpu_mask == NULL) {
		perror("CPU_ALLOC");
		exit(3);
	}
	cpu_mask_size = CPU_ALLOC_SIZE(ncpus);
	CPU_ZERO_S(cpu_mask_size, cpu_mask);
}

void cpu_mask_uninit()
{
	CPU_FREE(cpu_mask);
	cpu_mask = NULL;
	cpu_mask_size = 0;
=======

cpu_set_t *cpu_present_set, *cpu_affinity_set;
size_t cpu_present_setsize, cpu_affinity_setsize;

struct thread_data {
	unsigned long long tsc;
	unsigned long long aperf;
	unsigned long long mperf;
	unsigned long long c1;
	unsigned long long extra_msr64;
	unsigned long long extra_delta64;
	unsigned long long extra_msr32;
	unsigned long long extra_delta32;
	unsigned int smi_count;
	unsigned int cpu_id;
	unsigned int flags;
#define CPU_IS_FIRST_THREAD_IN_CORE	0x2
#define CPU_IS_FIRST_CORE_IN_PACKAGE	0x4
} *thread_even, *thread_odd;

struct core_data {
	unsigned long long c3;
	unsigned long long c6;
	unsigned long long c7;
	unsigned int core_temp_c;
	unsigned int core_id;
} *core_even, *core_odd;

struct pkg_data {
	unsigned long long pc2;
	unsigned long long pc3;
	unsigned long long pc6;
	unsigned long long pc7;
	unsigned long long pc8;
	unsigned long long pc9;
	unsigned long long pc10;
	unsigned int package_id;
	unsigned int energy_pkg;	/* MSR_PKG_ENERGY_STATUS */
	unsigned int energy_dram;	/* MSR_DRAM_ENERGY_STATUS */
	unsigned int energy_cores;	/* MSR_PP0_ENERGY_STATUS */
	unsigned int energy_gfx;	/* MSR_PP1_ENERGY_STATUS */
	unsigned int rapl_pkg_perf_status;	/* MSR_PKG_PERF_STATUS */
	unsigned int rapl_dram_perf_status;	/* MSR_DRAM_PERF_STATUS */
	unsigned int pkg_temp_c;

} *package_even, *package_odd;

#define ODD_COUNTERS thread_odd, core_odd, package_odd
#define EVEN_COUNTERS thread_even, core_even, package_even

#define GET_THREAD(thread_base, thread_no, core_no, pkg_no) \
	(thread_base + (pkg_no) * topo.num_cores_per_pkg * \
		topo.num_threads_per_core + \
		(core_no) * topo.num_threads_per_core + (thread_no))
#define GET_CORE(core_base, core_no, pkg_no) \
	(core_base + (pkg_no) * topo.num_cores_per_pkg + (core_no))
#define GET_PKG(pkg_base, pkg_no) (pkg_base + pkg_no)

struct system_summary {
	struct thread_data threads;
	struct core_data cores;
	struct pkg_data packages;
} sum, average;


struct topo_params {
	int num_packages;
	int num_cpus;
	int num_cores;
	int max_cpu_num;
	int num_cores_per_pkg;
	int num_threads_per_core;
} topo;

struct timeval tv_even, tv_odd, tv_delta;

void setup_all_buffers(void);

int cpu_is_not_present(int cpu)
{
	return !CPU_ISSET_S(cpu, cpu_present_setsize, cpu_present_set);
}
/*
 * run func(thread, core, package) in topology order
 * skip non-present cpus
 */

int for_all_cpus(int (func)(struct thread_data *, struct core_data *, struct pkg_data *),
	struct thread_data *thread_base, struct core_data *core_base, struct pkg_data *pkg_base)
{
	int retval, pkg_no, core_no, thread_no;

	for (pkg_no = 0; pkg_no < topo.num_packages; ++pkg_no) {
		for (core_no = 0; core_no < topo.num_cores_per_pkg; ++core_no) {
			for (thread_no = 0; thread_no <
				topo.num_threads_per_core; ++thread_no) {
				struct thread_data *t;
				struct core_data *c;
				struct pkg_data *p;

				t = GET_THREAD(thread_base, thread_no, core_no, pkg_no);

				if (cpu_is_not_present(t->cpu_id))
					continue;

				c = GET_CORE(core_base, core_no, pkg_no);
				p = GET_PKG(pkg_base, pkg_no);

				retval = func(t, c, p);
				if (retval)
					return retval;
			}
		}
	}
	return 0;
>>>>>>> refs/remotes/origin/master
}

int cpu_migrate(int cpu)
{
<<<<<<< HEAD
	CPU_ZERO_S(cpu_mask_size, cpu_mask);
	CPU_SET_S(cpu, cpu_mask_size, cpu_mask);
	if (sched_setaffinity(0, cpu_mask_size, cpu_mask) == -1)
=======
	CPU_ZERO_S(cpu_affinity_setsize, cpu_affinity_set);
	CPU_SET_S(cpu, cpu_affinity_setsize, cpu_affinity_set);
	if (sched_setaffinity(0, cpu_affinity_setsize, cpu_affinity_set) == -1)
>>>>>>> refs/remotes/origin/master
		return -1;
	else
		return 0;
}

int get_msr(int cpu, off_t offset, unsigned long long *msr)
{
	ssize_t retval;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	char pathname[32];
	int fd;

	sprintf(pathname, "/dev/cpu/%d/msr", cpu);
	fd = open(pathname, O_RDONLY);
<<<<<<< HEAD
<<<<<<< HEAD
	if (fd < 0) {
		perror(pathname);
		need_reinitialize = 1;
		return 0;
	}

	retval = pread(fd, &msr, sizeof msr, offset);
	if (retval != sizeof msr) {
		fprintf(stderr, "cpu%d pread(..., 0x%zx) = %jd\n",
			cpu, offset, retval);
		exit(-2);
	}

	close(fd);
	return msr;
=======
=======
>>>>>>> refs/remotes/origin/master
	if (fd < 0)
		return -1;

	retval = pread(fd, msr, sizeof *msr, offset);
	close(fd);

<<<<<<< HEAD
	if (retval != sizeof *msr)
		return -1;

	return 0;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (retval != sizeof *msr) {
		fprintf(stderr, "%s offset 0x%zx read failed\n", pathname, offset);
		return -1;
	}

	return 0;
>>>>>>> refs/remotes/origin/master
}

void print_header(void)
{
	if (show_pkg)
<<<<<<< HEAD
<<<<<<< HEAD
		fprintf(stderr, "pkg ");
	if (show_core)
		fprintf(stderr, "core");
	if (show_cpu)
		fprintf(stderr, " CPU");
	if (do_nhm_cstates)
		fprintf(stderr, "   %%c0 ");
=======
		fprintf(stderr, "pk");
	if (show_pkg)
		fprintf(stderr, " ");
	if (show_core)
		fprintf(stderr, "cor");
	if (show_cpu)
		fprintf(stderr, " CPU");
	if (show_pkg || show_core || show_cpu)
		fprintf(stderr, " ");
	if (do_nhm_cstates)
		fprintf(stderr, "   %%c0");
>>>>>>> refs/remotes/origin/cm-10.0
	if (has_aperf)
		fprintf(stderr, "  GHz");
	fprintf(stderr, "  TSC");
	if (do_nhm_cstates)
<<<<<<< HEAD
		fprintf(stderr, "   %%c1 ");
	if (do_nhm_cstates)
		fprintf(stderr, "   %%c3 ");
	if (do_nhm_cstates)
		fprintf(stderr, "   %%c6 ");
	if (do_snb_cstates)
		fprintf(stderr, "   %%c7 ");
	if (do_snb_cstates)
		fprintf(stderr, "  %%pc2 ");
	if (do_nhm_cstates)
		fprintf(stderr, "  %%pc3 ");
	if (do_nhm_cstates)
		fprintf(stderr, "  %%pc6 ");
	if (do_snb_cstates)
		fprintf(stderr, "  %%pc7 ");
	if (extra_msr_offset)
		fprintf(stderr, "       MSR 0x%x ", extra_msr_offset);
=======
		fprintf(stderr, "    %%c1");
	if (do_nhm_cstates)
		fprintf(stderr, "    %%c3");
	if (do_nhm_cstates)
		fprintf(stderr, "    %%c6");
	if (do_snb_cstates)
		fprintf(stderr, "    %%c7");
	if (do_snb_cstates)
		fprintf(stderr, "   %%pc2");
	if (do_nhm_cstates)
		fprintf(stderr, "   %%pc3");
	if (do_nhm_cstates)
		fprintf(stderr, "   %%pc6");
	if (do_snb_cstates)
		fprintf(stderr, "   %%pc7");
	if (extra_msr_offset)
		fprintf(stderr, "        MSR 0x%x ", extra_msr_offset);
>>>>>>> refs/remotes/origin/cm-10.0

	putc('\n', stderr);
}

void dump_cnt(struct counters *cnt)
{
<<<<<<< HEAD
	fprintf(stderr, "package: %d ", cnt->pkg);
	fprintf(stderr, "core:: %d ", cnt->core);
	fprintf(stderr, "CPU: %d ", cnt->cpu);
	fprintf(stderr, "TSC: %016llX\n", cnt->tsc);
	fprintf(stderr, "c3: %016llX\n", cnt->c3);
	fprintf(stderr, "c6: %016llX\n", cnt->c6);
	fprintf(stderr, "c7: %016llX\n", cnt->c7);
	fprintf(stderr, "aperf: %016llX\n", cnt->aperf);
	fprintf(stderr, "pc2: %016llX\n", cnt->pc2);
	fprintf(stderr, "pc3: %016llX\n", cnt->pc3);
	fprintf(stderr, "pc6: %016llX\n", cnt->pc6);
	fprintf(stderr, "pc7: %016llX\n", cnt->pc7);
	fprintf(stderr, "msr0x%x: %016llX\n", extra_msr_offset, cnt->extra_msr);
=======
	if (!cnt)
		return;
	if (cnt->pkg) fprintf(stderr, "package: %d ", cnt->pkg);
	if (cnt->core) fprintf(stderr, "core:: %d ", cnt->core);
	if (cnt->cpu) fprintf(stderr, "CPU: %d ", cnt->cpu);
	if (cnt->tsc) fprintf(stderr, "TSC: %016llX\n", cnt->tsc);
	if (cnt->c3) fprintf(stderr, "c3: %016llX\n", cnt->c3);
	if (cnt->c6) fprintf(stderr, "c6: %016llX\n", cnt->c6);
	if (cnt->c7) fprintf(stderr, "c7: %016llX\n", cnt->c7);
	if (cnt->aperf) fprintf(stderr, "aperf: %016llX\n", cnt->aperf);
	if (cnt->pc2) fprintf(stderr, "pc2: %016llX\n", cnt->pc2);
	if (cnt->pc3) fprintf(stderr, "pc3: %016llX\n", cnt->pc3);
	if (cnt->pc6) fprintf(stderr, "pc6: %016llX\n", cnt->pc6);
	if (cnt->pc7) fprintf(stderr, "pc7: %016llX\n", cnt->pc7);
	if (cnt->extra_msr) fprintf(stderr, "msr0x%x: %016llX\n", extra_msr_offset, cnt->extra_msr);
>>>>>>> refs/remotes/origin/cm-10.0
}

void dump_list(struct counters *cnt)
{
	printf("dump_list 0x%p\n", cnt);

	for (; cnt; cnt = cnt->next)
		dump_cnt(cnt);
}

<<<<<<< HEAD
=======
=======
		outp += sprintf(outp, "pk");
	if (show_pkg)
		outp += sprintf(outp, " ");
	if (show_core)
		outp += sprintf(outp, "cor");
	if (show_cpu)
		outp += sprintf(outp, " CPU");
	if (show_pkg || show_core || show_cpu)
		outp += sprintf(outp, " ");
	if (do_nhm_cstates)
		outp += sprintf(outp, "   %%c0");
	if (has_aperf)
		outp += sprintf(outp, "  GHz");
	outp += sprintf(outp, "  TSC");
	if (do_smi)
		outp += sprintf(outp, " SMI");
	if (extra_delta_offset32)
		outp += sprintf(outp, "  count 0x%03X", extra_delta_offset32);
	if (extra_delta_offset64)
		outp += sprintf(outp, "  COUNT 0x%03X", extra_delta_offset64);
	if (extra_msr_offset32)
		outp += sprintf(outp, "   MSR 0x%03X", extra_msr_offset32);
	if (extra_msr_offset64)
		outp += sprintf(outp, "           MSR 0x%03X", extra_msr_offset64);
	if (do_nhm_cstates)
		outp += sprintf(outp, "    %%c1");
	if (do_nhm_cstates && !do_slm_cstates)
		outp += sprintf(outp, "    %%c3");
	if (do_nhm_cstates)
		outp += sprintf(outp, "    %%c6");
	if (do_snb_cstates)
		outp += sprintf(outp, "    %%c7");

	if (do_dts)
		outp += sprintf(outp, " CTMP");
	if (do_ptm)
		outp += sprintf(outp, " PTMP");

	if (do_snb_cstates)
		outp += sprintf(outp, "   %%pc2");
	if (do_nhm_cstates && !do_slm_cstates)
		outp += sprintf(outp, "   %%pc3");
	if (do_nhm_cstates && !do_slm_cstates)
		outp += sprintf(outp, "   %%pc6");
	if (do_snb_cstates)
		outp += sprintf(outp, "   %%pc7");
	if (do_c8_c9_c10) {
		outp += sprintf(outp, "   %%pc8");
		outp += sprintf(outp, "   %%pc9");
		outp += sprintf(outp, "  %%pc10");
	}

	if (do_rapl & RAPL_PKG)
		outp += sprintf(outp, "  Pkg_W");
	if (do_rapl & RAPL_CORES)
		outp += sprintf(outp, "  Cor_W");
	if (do_rapl & RAPL_GFX)
		outp += sprintf(outp, " GFX_W");
	if (do_rapl & RAPL_DRAM)
		outp += sprintf(outp, " RAM_W");
	if (do_rapl & RAPL_PKG_PERF_STATUS)
		outp += sprintf(outp, " PKG_%%");
	if (do_rapl & RAPL_DRAM_PERF_STATUS)
		outp += sprintf(outp, " RAM_%%");

	outp += sprintf(outp, "\n");
}

int dump_counters(struct thread_data *t, struct core_data *c,
	struct pkg_data *p)
{
	fprintf(stderr, "t %p, c %p, p %p\n", t, c, p);

	if (t) {
		fprintf(stderr, "CPU: %d flags 0x%x\n", t->cpu_id, t->flags);
		fprintf(stderr, "TSC: %016llX\n", t->tsc);
		fprintf(stderr, "aperf: %016llX\n", t->aperf);
		fprintf(stderr, "mperf: %016llX\n", t->mperf);
		fprintf(stderr, "c1: %016llX\n", t->c1);
		fprintf(stderr, "msr0x%x: %08llX\n",
			extra_delta_offset32, t->extra_delta32);
		fprintf(stderr, "msr0x%x: %016llX\n",
			extra_delta_offset64, t->extra_delta64);
		fprintf(stderr, "msr0x%x: %08llX\n",
			extra_msr_offset32, t->extra_msr32);
		fprintf(stderr, "msr0x%x: %016llX\n",
			extra_msr_offset64, t->extra_msr64);
		if (do_smi)
			fprintf(stderr, "SMI: %08X\n", t->smi_count);
	}

	if (c) {
		fprintf(stderr, "core: %d\n", c->core_id);
		fprintf(stderr, "c3: %016llX\n", c->c3);
		fprintf(stderr, "c6: %016llX\n", c->c6);
		fprintf(stderr, "c7: %016llX\n", c->c7);
		fprintf(stderr, "DTS: %dC\n", c->core_temp_c);
	}

	if (p) {
		fprintf(stderr, "package: %d\n", p->package_id);
		fprintf(stderr, "pc2: %016llX\n", p->pc2);
		fprintf(stderr, "pc3: %016llX\n", p->pc3);
		fprintf(stderr, "pc6: %016llX\n", p->pc6);
		fprintf(stderr, "pc7: %016llX\n", p->pc7);
		fprintf(stderr, "pc8: %016llX\n", p->pc8);
		fprintf(stderr, "pc9: %016llX\n", p->pc9);
		fprintf(stderr, "pc10: %016llX\n", p->pc10);
		fprintf(stderr, "Joules PKG: %0X\n", p->energy_pkg);
		fprintf(stderr, "Joules COR: %0X\n", p->energy_cores);
		fprintf(stderr, "Joules GFX: %0X\n", p->energy_gfx);
		fprintf(stderr, "Joules RAM: %0X\n", p->energy_dram);
		fprintf(stderr, "Throttle PKG: %0X\n", p->rapl_pkg_perf_status);
		fprintf(stderr, "Throttle RAM: %0X\n", p->rapl_dram_perf_status);
		fprintf(stderr, "PTM: %dC\n", p->pkg_temp_c);
	}
	return 0;
}

>>>>>>> refs/remotes/origin/master
/*
 * column formatting convention & formats
 * package: "pk" 2 columns %2d
 * core: "cor" 3 columns %3d
 * CPU: "CPU" 3 columns %3d
<<<<<<< HEAD
 * GHz: "GHz" 3 columns %3.2
 * TSC: "TSC" 3 columns %3.2
 * percentage " %pc3" %6.2
 */
>>>>>>> refs/remotes/origin/cm-10.0
void print_cnt(struct counters *p)
{
	double interval_float;

	interval_float = tv_delta.tv_sec + tv_delta.tv_usec/1000000.0;

	/* topology columns, print blanks on 1st (average) line */
	if (p == cnt_average) {
		if (show_pkg)
<<<<<<< HEAD
			fprintf(stderr, "    ");
		if (show_core)
			fprintf(stderr, "    ");
		if (show_cpu)
			fprintf(stderr, "    ");
	} else {
		if (show_pkg)
			fprintf(stderr, "%4d", p->pkg);
		if (show_core)
			fprintf(stderr, "%4d", p->core);
		if (show_cpu)
			fprintf(stderr, "%4d", p->cpu);
=======
			fprintf(stderr, "  ");
		if (show_pkg && show_core)
			fprintf(stderr, " ");
		if (show_core)
			fprintf(stderr, "   ");
		if (show_cpu)
			fprintf(stderr, " " "   ");
	} else {
		if (show_pkg)
			fprintf(stderr, "%2d", p->pkg);
		if (show_pkg && show_core)
			fprintf(stderr, " ");
		if (show_core)
			fprintf(stderr, "%3d", p->core);
		if (show_cpu)
			fprintf(stderr, " %3d", p->cpu);
>>>>>>> refs/remotes/origin/cm-10.0
	}

	/* %c0 */
	if (do_nhm_cstates) {
<<<<<<< HEAD
		if (!skip_c0)
			fprintf(stderr, "%7.2f", 100.0 * p->mperf/p->tsc);
		else
			fprintf(stderr, "   ****");
=======
		if (show_pkg || show_core || show_cpu)
			fprintf(stderr, " ");
		if (!skip_c0)
			fprintf(stderr, "%6.2f", 100.0 * p->mperf/p->tsc);
		else
			fprintf(stderr, "  ****");
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Pkg_W: %6.2
 * Cor_W: %6.2
 * GFX_W: %5.2
 * RAM_W: %5.2
 * GHz: "GHz" 3 columns %3.2
 * TSC: "TSC" 3 columns %3.2
 * SMI: "SMI" 4 columns %4d
 * percentage " %pc3" %6.2
 * Perf Status percentage: %5.2
 * "CTMP" 4 columns %4d
 */
int format_counters(struct thread_data *t, struct core_data *c,
	struct pkg_data *p)
{
	double interval_float;
	char *fmt5, *fmt6;

	 /* if showing only 1st thread in core and this isn't one, bail out */
	if (show_core_only && !(t->flags & CPU_IS_FIRST_THREAD_IN_CORE))
		return 0;

	 /* if showing only 1st thread in pkg and this isn't one, bail out */
	if (show_pkg_only && !(t->flags & CPU_IS_FIRST_CORE_IN_PACKAGE))
		return 0;

	interval_float = tv_delta.tv_sec + tv_delta.tv_usec/1000000.0;

	/* topo columns, print blanks on 1st (average) line */
	if (t == &average.threads) {
		if (show_pkg)
			outp += sprintf(outp, "  ");
		if (show_pkg && show_core)
			outp += sprintf(outp, " ");
		if (show_core)
			outp += sprintf(outp, "   ");
		if (show_cpu)
			outp += sprintf(outp, " " "   ");
	} else {
		if (show_pkg) {
			if (p)
				outp += sprintf(outp, "%2d", p->package_id);
			else
				outp += sprintf(outp, "  ");
		}
		if (show_pkg && show_core)
			outp += sprintf(outp, " ");
		if (show_core) {
			if (c)
				outp += sprintf(outp, "%3d", c->core_id);
			else
				outp += sprintf(outp, "   ");
		}
		if (show_cpu)
			outp += sprintf(outp, " %3d", t->cpu_id);
	}
	/* %c0 */
	if (do_nhm_cstates) {
		if (show_pkg || show_core || show_cpu)
			outp += sprintf(outp, " ");
		if (!skip_c0)
			outp += sprintf(outp, "%6.2f", 100.0 * t->mperf/t->tsc);
		else
			outp += sprintf(outp, "  ****");
>>>>>>> refs/remotes/origin/master
	}

	/* GHz */
	if (has_aperf) {
		if (!aperf_mperf_unstable) {
<<<<<<< HEAD
<<<<<<< HEAD
			fprintf(stderr, "%5.2f",
=======
			fprintf(stderr, " %3.2f",
>>>>>>> refs/remotes/origin/cm-10.0
				1.0 * p->tsc / units * p->aperf /
				p->mperf / interval_float);
		} else {
			if (p->aperf > p->tsc || p->mperf > p->tsc) {
<<<<<<< HEAD
				fprintf(stderr, " ****");
			} else {
				fprintf(stderr, "%4.1f*",
=======
				fprintf(stderr, " ***");
			} else {
				fprintf(stderr, "%3.1f*",
>>>>>>> refs/remotes/origin/cm-10.0
					1.0 * p->tsc /
					units * p->aperf /
					p->mperf / interval_float);
=======
			outp += sprintf(outp, " %3.2f",
				1.0 * t->tsc / units * t->aperf /
				t->mperf / interval_float);
		} else {
			if (t->aperf > t->tsc || t->mperf > t->tsc) {
				outp += sprintf(outp, " ***");
			} else {
				outp += sprintf(outp, "%3.1f*",
					1.0 * t->tsc /
					units * t->aperf /
					t->mperf / interval_float);
>>>>>>> refs/remotes/origin/master
			}
		}
	}

	/* TSC */
<<<<<<< HEAD
	fprintf(stderr, "%5.2f", 1.0 * p->tsc/units/interval_float);

	if (do_nhm_cstates) {
		if (!skip_c1)
<<<<<<< HEAD
			fprintf(stderr, "%7.2f", 100.0 * p->c1/p->tsc);
		else
			fprintf(stderr, "   ****");
	}
	if (do_nhm_cstates)
		fprintf(stderr, "%7.2f", 100.0 * p->c3/p->tsc);
	if (do_nhm_cstates)
		fprintf(stderr, "%7.2f", 100.0 * p->c6/p->tsc);
	if (do_snb_cstates)
		fprintf(stderr, "%7.2f", 100.0 * p->c7/p->tsc);
	if (do_snb_cstates)
		fprintf(stderr, "%7.2f", 100.0 * p->pc2/p->tsc);
	if (do_nhm_cstates)
		fprintf(stderr, "%7.2f", 100.0 * p->pc3/p->tsc);
	if (do_nhm_cstates)
		fprintf(stderr, "%7.2f", 100.0 * p->pc6/p->tsc);
	if (do_snb_cstates)
		fprintf(stderr, "%7.2f", 100.0 * p->pc7/p->tsc);
=======
			fprintf(stderr, " %6.2f", 100.0 * p->c1/p->tsc);
		else
			fprintf(stderr, "  ****");
	}
	if (do_nhm_cstates)
		fprintf(stderr, " %6.2f", 100.0 * p->c3/p->tsc);
	if (do_nhm_cstates)
		fprintf(stderr, " %6.2f", 100.0 * p->c6/p->tsc);
	if (do_snb_cstates)
		fprintf(stderr, " %6.2f", 100.0 * p->c7/p->tsc);
	if (do_snb_cstates)
		fprintf(stderr, " %6.2f", 100.0 * p->pc2/p->tsc);
	if (do_nhm_cstates)
		fprintf(stderr, " %6.2f", 100.0 * p->pc3/p->tsc);
	if (do_nhm_cstates)
		fprintf(stderr, " %6.2f", 100.0 * p->pc6/p->tsc);
	if (do_snb_cstates)
		fprintf(stderr, " %6.2f", 100.0 * p->pc7/p->tsc);
>>>>>>> refs/remotes/origin/cm-10.0
	if (extra_msr_offset)
		fprintf(stderr, "  0x%016llx", p->extra_msr);
	putc('\n', stderr);
}

void print_counters(struct counters *counters)
{
	struct counters *cnt;
<<<<<<< HEAD

	print_header();
=======
	static int printed;


	if (!printed || !summary_only)
		print_header();
>>>>>>> refs/remotes/origin/cm-10.0

	if (num_cpus > 1)
		print_cnt(cnt_average);

<<<<<<< HEAD
=======
=======
	outp += sprintf(outp, "%5.2f", 1.0 * t->tsc/units/interval_float);

	/* SMI */
	if (do_smi)
		outp += sprintf(outp, "%4d", t->smi_count);

	/* delta */
	if (extra_delta_offset32)
		outp += sprintf(outp, "  %11llu", t->extra_delta32);

	/* DELTA */
	if (extra_delta_offset64)
		outp += sprintf(outp, "  %11llu", t->extra_delta64);
	/* msr */
	if (extra_msr_offset32)
		outp += sprintf(outp, "  0x%08llx", t->extra_msr32);

	/* MSR */
	if (extra_msr_offset64)
		outp += sprintf(outp, "  0x%016llx", t->extra_msr64);

	if (do_nhm_cstates) {
		if (!skip_c1)
			outp += sprintf(outp, " %6.2f", 100.0 * t->c1/t->tsc);
		else
			outp += sprintf(outp, "  ****");
	}

	/* print per-core data only for 1st thread in core */
	if (!(t->flags & CPU_IS_FIRST_THREAD_IN_CORE))
		goto done;

	if (do_nhm_cstates && !do_slm_cstates)
		outp += sprintf(outp, " %6.2f", 100.0 * c->c3/t->tsc);
	if (do_nhm_cstates)
		outp += sprintf(outp, " %6.2f", 100.0 * c->c6/t->tsc);
	if (do_snb_cstates)
		outp += sprintf(outp, " %6.2f", 100.0 * c->c7/t->tsc);

	if (do_dts)
		outp += sprintf(outp, " %4d", c->core_temp_c);

	/* print per-package data only for 1st core in package */
	if (!(t->flags & CPU_IS_FIRST_CORE_IN_PACKAGE))
		goto done;

	if (do_ptm)
		outp += sprintf(outp, " %4d", p->pkg_temp_c);

	if (do_snb_cstates)
		outp += sprintf(outp, " %6.2f", 100.0 * p->pc2/t->tsc);
	if (do_nhm_cstates && !do_slm_cstates)
		outp += sprintf(outp, " %6.2f", 100.0 * p->pc3/t->tsc);
	if (do_nhm_cstates && !do_slm_cstates)
		outp += sprintf(outp, " %6.2f", 100.0 * p->pc6/t->tsc);
	if (do_snb_cstates)
		outp += sprintf(outp, " %6.2f", 100.0 * p->pc7/t->tsc);
	if (do_c8_c9_c10) {
		outp += sprintf(outp, " %6.2f", 100.0 * p->pc8/t->tsc);
		outp += sprintf(outp, " %6.2f", 100.0 * p->pc9/t->tsc);
		outp += sprintf(outp, " %6.2f", 100.0 * p->pc10/t->tsc);
	}

	/*
 	 * If measurement interval exceeds minimum RAPL Joule Counter range,
 	 * indicate that results are suspect by printing "**" in fraction place.
 	 */
	if (interval_float < rapl_joule_counter_range) {
		fmt5 = " %5.2f";
		fmt6 = " %6.2f";
	} else {
		fmt5 = " %3.0f**";
		fmt6 = " %4.0f**";
	}

	if (do_rapl & RAPL_PKG)
		outp += sprintf(outp, fmt6, p->energy_pkg * rapl_energy_units / interval_float);
	if (do_rapl & RAPL_CORES)
		outp += sprintf(outp, fmt6, p->energy_cores * rapl_energy_units / interval_float);
	if (do_rapl & RAPL_GFX)
		outp += sprintf(outp, fmt5, p->energy_gfx * rapl_energy_units / interval_float); 
	if (do_rapl & RAPL_DRAM)
		outp += sprintf(outp, fmt5, p->energy_dram * rapl_energy_units / interval_float);
	if (do_rapl & RAPL_PKG_PERF_STATUS )
		outp += sprintf(outp, fmt5, 100.0 * p->rapl_pkg_perf_status * rapl_time_units / interval_float);
	if (do_rapl & RAPL_DRAM_PERF_STATUS )
		outp += sprintf(outp, fmt5, 100.0 * p->rapl_dram_perf_status * rapl_time_units / interval_float);

done:
	outp += sprintf(outp, "\n");

	return 0;
}

void flush_stdout()
{
	fputs(output_buffer, stdout);
	fflush(stdout);
	outp = output_buffer;
}
void flush_stderr()
{
	fputs(output_buffer, stderr);
	outp = output_buffer;
}
void format_all_counters(struct thread_data *t, struct core_data *c, struct pkg_data *p)
{
	static int printed;

	if (!printed || !summary_only)
		print_header();

	if (topo.num_cpus > 1)
		format_counters(&average.threads, &average.cores,
			&average.packages);

>>>>>>> refs/remotes/origin/master
	printed = 1;

	if (summary_only)
		return;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	for (cnt = counters; cnt != NULL; cnt = cnt->next)
		print_cnt(cnt);

}

#define SUBTRACT_COUNTER(after, before, delta) (delta = (after - before), (before > after))

int compute_delta(struct counters *after,
	struct counters *before, struct counters *delta)
{
	int errors = 0;
	int perf_err = 0;

	skip_c0 = skip_c1 = 0;

	for ( ; after && before && delta;
		after = after->next, before = before->next, delta = delta->next) {
		if (before->cpu != after->cpu) {
			printf("cpu configuration changed: %d != %d\n",
				before->cpu, after->cpu);
			return -1;
		}

		if (SUBTRACT_COUNTER(after->tsc, before->tsc, delta->tsc)) {
			fprintf(stderr, "cpu%d TSC went backwards %llX to %llX\n",
				before->cpu, before->tsc, after->tsc);
			errors++;
		}
		/* check for TSC < 1 Mcycles over interval */
		if (delta->tsc < (1000 * 1000)) {
			fprintf(stderr, "Insanely slow TSC rate,"
				" TSC stops in idle?\n");
			fprintf(stderr, "You can disable all c-states"
				" by booting with \"idle=poll\"\n");
			fprintf(stderr, "or just the deep ones with"
				" \"processor.max_cstate=1\"\n");
			exit(-3);
		}
		if (SUBTRACT_COUNTER(after->c3, before->c3, delta->c3)) {
			fprintf(stderr, "cpu%d c3 counter went backwards %llX to %llX\n",
				before->cpu, before->c3, after->c3);
			errors++;
		}
		if (SUBTRACT_COUNTER(after->c6, before->c6, delta->c6)) {
			fprintf(stderr, "cpu%d c6 counter went backwards %llX to %llX\n",
				before->cpu, before->c6, after->c6);
			errors++;
		}
		if (SUBTRACT_COUNTER(after->c7, before->c7, delta->c7)) {
			fprintf(stderr, "cpu%d c7 counter went backwards %llX to %llX\n",
				before->cpu, before->c7, after->c7);
			errors++;
		}
		if (SUBTRACT_COUNTER(after->pc2, before->pc2, delta->pc2)) {
			fprintf(stderr, "cpu%d pc2 counter went backwards %llX to %llX\n",
				before->cpu, before->pc2, after->pc2);
			errors++;
		}
		if (SUBTRACT_COUNTER(after->pc3, before->pc3, delta->pc3)) {
			fprintf(stderr, "cpu%d pc3 counter went backwards %llX to %llX\n",
				before->cpu, before->pc3, after->pc3);
			errors++;
		}
		if (SUBTRACT_COUNTER(after->pc6, before->pc6, delta->pc6)) {
			fprintf(stderr, "cpu%d pc6 counter went backwards %llX to %llX\n",
				before->cpu, before->pc6, after->pc6);
			errors++;
		}
		if (SUBTRACT_COUNTER(after->pc7, before->pc7, delta->pc7)) {
			fprintf(stderr, "cpu%d pc7 counter went backwards %llX to %llX\n",
				before->cpu, before->pc7, after->pc7);
			errors++;
		}

		perf_err = SUBTRACT_COUNTER(after->aperf, before->aperf, delta->aperf);
		if (perf_err) {
			fprintf(stderr, "cpu%d aperf counter went backwards %llX to %llX\n",
				before->cpu, before->aperf, after->aperf);
		}
		perf_err |= SUBTRACT_COUNTER(after->mperf, before->mperf, delta->mperf);
		if (perf_err) {
			fprintf(stderr, "cpu%d mperf counter went backwards %llX to %llX\n",
				before->cpu, before->mperf, after->mperf);
		}
		if (perf_err) {
			if (!aperf_mperf_unstable) {
				fprintf(stderr, "%s: APERF or MPERF went backwards *\n", progname);
				fprintf(stderr, "* Frequency results do not cover entire interval *\n");
				fprintf(stderr, "* fix this by running Linux-2.6.30 or later *\n");

				aperf_mperf_unstable = 1;
			}
			/*
			 * mperf delta is likely a huge "positive" number
			 * can not use it for calculating c0 time
			 */
			skip_c0 = 1;
			skip_c1 = 1;
		}

		/*
		 * As mperf and tsc collection are not atomic,
		 * it is possible for mperf's non-halted cycles
		 * to exceed TSC's all cycles: show c1 = 0% in that case.
		 */
		if (delta->mperf > delta->tsc)
			delta->c1 = 0;
		else /* normal case, derive c1 */
			delta->c1 = delta->tsc - delta->mperf
				- delta->c3 - delta->c6 - delta->c7;

		if (delta->mperf == 0)
			delta->mperf = 1;	/* divide by 0 protection */

		/*
		 * for "extra msr", just copy the latest w/o subtracting
		 */
		delta->extra_msr = after->extra_msr;
		if (errors) {
			fprintf(stderr, "ERROR cpu%d before:\n", before->cpu);
			dump_cnt(before);
			fprintf(stderr, "ERROR cpu%d after:\n", before->cpu);
			dump_cnt(after);
			errors = 0;
		}
	}
	return 0;
}

void compute_average(struct counters *delta, struct counters *avg)
{
	struct counters *sum;

	sum = calloc(1, sizeof(struct counters));
	if (sum == NULL) {
		perror("calloc sum");
		exit(1);
	}

	for (; delta; delta = delta->next) {
		sum->tsc += delta->tsc;
		sum->c1 += delta->c1;
		sum->c3 += delta->c3;
		sum->c6 += delta->c6;
		sum->c7 += delta->c7;
		sum->aperf += delta->aperf;
		sum->mperf += delta->mperf;
		sum->pc2 += delta->pc2;
		sum->pc3 += delta->pc3;
		sum->pc6 += delta->pc6;
		sum->pc7 += delta->pc7;
	}
	avg->tsc = sum->tsc/num_cpus;
	avg->c1 = sum->c1/num_cpus;
	avg->c3 = sum->c3/num_cpus;
	avg->c6 = sum->c6/num_cpus;
	avg->c7 = sum->c7/num_cpus;
	avg->aperf = sum->aperf/num_cpus;
	avg->mperf = sum->mperf/num_cpus;
	avg->pc2 = sum->pc2/num_cpus;
	avg->pc3 = sum->pc3/num_cpus;
	avg->pc6 = sum->pc6/num_cpus;
	avg->pc7 = sum->pc7/num_cpus;

	free(sum);
}

<<<<<<< HEAD
void get_counters(struct counters *cnt)
{
	for ( ; cnt; cnt = cnt->next) {
		cnt->tsc = get_msr(cnt->cpu, MSR_TSC);
		if (do_nhm_cstates)
			cnt->c3 = get_msr(cnt->cpu, MSR_CORE_C3_RESIDENCY);
		if (do_nhm_cstates)
			cnt->c6 = get_msr(cnt->cpu, MSR_CORE_C6_RESIDENCY);
		if (do_snb_cstates)
			cnt->c7 = get_msr(cnt->cpu, MSR_CORE_C7_RESIDENCY);
		if (has_aperf)
			cnt->aperf = get_msr(cnt->cpu, MSR_APERF);
		if (has_aperf)
			cnt->mperf = get_msr(cnt->cpu, MSR_MPERF);
		if (do_snb_cstates)
			cnt->pc2 = get_msr(cnt->cpu, MSR_PKG_C2_RESIDENCY);
		if (do_nhm_cstates)
			cnt->pc3 = get_msr(cnt->cpu, MSR_PKG_C3_RESIDENCY);
		if (do_nhm_cstates)
			cnt->pc6 = get_msr(cnt->cpu, MSR_PKG_C6_RESIDENCY);
		if (do_snb_cstates)
			cnt->pc7 = get_msr(cnt->cpu, MSR_PKG_C7_RESIDENCY);
		if (extra_msr_offset)
			cnt->extra_msr = get_msr(cnt->cpu, extra_msr_offset);
	}
=======
int get_counters(struct counters *cnt)
{
	for ( ; cnt; cnt = cnt->next) {

		if (cpu_migrate(cnt->cpu))
			return -1;

		if (get_msr(cnt->cpu, MSR_TSC, &cnt->tsc))
			return -1;

		if (has_aperf) {
			if (get_msr(cnt->cpu, MSR_APERF, &cnt->aperf))
				return -1;
			if (get_msr(cnt->cpu, MSR_MPERF, &cnt->mperf))
				return -1;
		}

		if (do_nhm_cstates) {
			if (get_msr(cnt->cpu, MSR_CORE_C3_RESIDENCY, &cnt->c3))
				return -1;
			if (get_msr(cnt->cpu, MSR_CORE_C6_RESIDENCY, &cnt->c6))
				return -1;
		}

		if (do_snb_cstates)
			if (get_msr(cnt->cpu, MSR_CORE_C7_RESIDENCY, &cnt->c7))
				return -1;

		if (do_nhm_cstates) {
			if (get_msr(cnt->cpu, MSR_PKG_C3_RESIDENCY, &cnt->pc3))
				return -1;
			if (get_msr(cnt->cpu, MSR_PKG_C6_RESIDENCY, &cnt->pc6))
				return -1;
		}
		if (do_snb_cstates) {
			if (get_msr(cnt->cpu, MSR_PKG_C2_RESIDENCY, &cnt->pc2))
				return -1;
			if (get_msr(cnt->cpu, MSR_PKG_C7_RESIDENCY, &cnt->pc7))
				return -1;
		}
		if (extra_msr_offset)
			if (get_msr(cnt->cpu, extra_msr_offset, &cnt->extra_msr))
				return -1;
	}
	return 0;
>>>>>>> refs/remotes/origin/cm-10.0
}

void print_nehalem_info(void)
{
	unsigned long long msr;
	unsigned int ratio;

	if (!do_nehalem_platform_info)
		return;

<<<<<<< HEAD
	msr = get_msr(0, MSR_NEHALEM_PLATFORM_INFO);
=======
	get_msr(0, MSR_NEHALEM_PLATFORM_INFO, &msr);
>>>>>>> refs/remotes/origin/cm-10.0

	ratio = (msr >> 40) & 0xFF;
	fprintf(stderr, "%d * %.0f = %.0f MHz max efficiency\n",
		ratio, bclk, ratio * bclk);

	ratio = (msr >> 8) & 0xFF;
	fprintf(stderr, "%d * %.0f = %.0f MHz TSC frequency\n",
		ratio, bclk, ratio * bclk);

	if (verbose > 1)
		fprintf(stderr, "MSR_NEHALEM_PLATFORM_INFO: 0x%llx\n", msr);

	if (!do_nehalem_turbo_ratio_limit)
		return;

<<<<<<< HEAD
	msr = get_msr(0, MSR_NEHALEM_TURBO_RATIO_LIMIT);
=======
	get_msr(0, MSR_NEHALEM_TURBO_RATIO_LIMIT, &msr);
>>>>>>> refs/remotes/origin/cm-10.0

	ratio = (msr >> 24) & 0xFF;
	if (ratio)
		fprintf(stderr, "%d * %.0f = %.0f MHz max turbo 4 active cores\n",
			ratio, bclk, ratio * bclk);

	ratio = (msr >> 16) & 0xFF;
	if (ratio)
		fprintf(stderr, "%d * %.0f = %.0f MHz max turbo 3 active cores\n",
			ratio, bclk, ratio * bclk);

	ratio = (msr >> 8) & 0xFF;
	if (ratio)
		fprintf(stderr, "%d * %.0f = %.0f MHz max turbo 2 active cores\n",
			ratio, bclk, ratio * bclk);

	ratio = (msr >> 0) & 0xFF;
	if (ratio)
		fprintf(stderr, "%d * %.0f = %.0f MHz max turbo 1 active cores\n",
			ratio, bclk, ratio * bclk);

}

void free_counter_list(struct counters *list)
{
	struct counters *p;

	for (p = list; p; ) {
		struct counters *free_me;

		free_me = p;
		p = p->next;
		free(free_me);
	}
}

void free_all_counters(void)
{
	free_counter_list(cnt_even);
	cnt_even = NULL;

	free_counter_list(cnt_odd);
	cnt_odd = NULL;

	free_counter_list(cnt_delta);
	cnt_delta = NULL;

	free_counter_list(cnt_average);
	cnt_average = NULL;
}

void insert_counters(struct counters **list,
	struct counters *new)
{
	struct counters *prev;

	/*
	 * list was empty
	 */
	if (*list == NULL) {
		new->next = *list;
		*list = new;
		return;
	}

<<<<<<< HEAD
	show_cpu = 1;	/* there is more than one CPU */
=======
	if (!summary_only)
		show_cpu = 1;	/* there is more than one CPU */
>>>>>>> refs/remotes/origin/cm-10.0

	/*
	 * insert on front of list.
	 * It is sorted by ascending package#, core#, cpu#
	 */
	if (((*list)->pkg > new->pkg) ||
	    (((*list)->pkg == new->pkg) && ((*list)->core > new->core)) ||
	    (((*list)->pkg == new->pkg) && ((*list)->core == new->core) && ((*list)->cpu > new->cpu))) {
		new->next = *list;
		*list = new;
		return;
	}

	prev = *list;

	while (prev->next && (prev->next->pkg < new->pkg)) {
		prev = prev->next;
<<<<<<< HEAD
		show_pkg = 1;	/* there is more than 1 package */
=======
		if (!summary_only)
			show_pkg = 1;	/* there is more than 1 package */
>>>>>>> refs/remotes/origin/cm-10.0
	}

	while (prev->next && (prev->next->pkg == new->pkg)
		&& (prev->next->core < new->core)) {
		prev = prev->next;
<<<<<<< HEAD
		show_core = 1;	/* there is more than 1 core */
=======
		if (!summary_only)
			show_core = 1;	/* there is more than 1 core */
>>>>>>> refs/remotes/origin/cm-10.0
	}

	while (prev->next && (prev->next->pkg == new->pkg)
		&& (prev->next->core == new->core)
		&& (prev->next->cpu < new->cpu)) {
		prev = prev->next;
	}

	/*
	 * insert after "prev"
	 */
	new->next = prev->next;
	prev->next = new;
}

void alloc_new_counters(int pkg, int core, int cpu)
{
	struct counters *new;

	if (verbose > 1)
		printf("pkg%d core%d, cpu%d\n", pkg, core, cpu);

	new = (struct counters *)calloc(1, sizeof(struct counters));
	if (new == NULL) {
		perror("calloc");
		exit(1);
	}
	new->pkg = pkg;
	new->core = core;
	new->cpu = cpu;
	insert_counters(&cnt_odd, new);

	new = (struct counters *)calloc(1,
		sizeof(struct counters));
	if (new == NULL) {
		perror("calloc");
		exit(1);
	}
	new->pkg = pkg;
	new->core = core;
	new->cpu = cpu;
	insert_counters(&cnt_even, new);

	new = (struct counters *)calloc(1, sizeof(struct counters));
	if (new == NULL) {
		perror("calloc");
		exit(1);
	}
	new->pkg = pkg;
	new->core = core;
	new->cpu = cpu;
	insert_counters(&cnt_delta, new);

	new = (struct counters *)calloc(1, sizeof(struct counters));
	if (new == NULL) {
		perror("calloc");
		exit(1);
	}
	new->pkg = pkg;
	new->core = core;
	new->cpu = cpu;
	cnt_average = new;
}

int get_physical_package_id(int cpu)
{
	char path[64];
	FILE *filep;
	int pkg;

	sprintf(path, "/sys/devices/system/cpu/cpu%d/topology/physical_package_id", cpu);
	filep = fopen(path, "r");
	if (filep == NULL) {
		perror(path);
		exit(1);
	}
	fscanf(filep, "%d", &pkg);
	fclose(filep);
	return pkg;
}

int get_core_id(int cpu)
{
	char path[64];
	FILE *filep;
=======
	for_all_cpus(format_counters, t, c, p);
}

#define DELTA_WRAP32(new, old)			\
	if (new > old) {			\
		old = new - old;		\
	} else {				\
		old = 0x100000000 + new - old;	\
	}

void
delta_package(struct pkg_data *new, struct pkg_data *old)
{
	old->pc2 = new->pc2 - old->pc2;
	old->pc3 = new->pc3 - old->pc3;
	old->pc6 = new->pc6 - old->pc6;
	old->pc7 = new->pc7 - old->pc7;
	old->pc8 = new->pc8 - old->pc8;
	old->pc9 = new->pc9 - old->pc9;
	old->pc10 = new->pc10 - old->pc10;
	old->pkg_temp_c = new->pkg_temp_c;

	DELTA_WRAP32(new->energy_pkg, old->energy_pkg);
	DELTA_WRAP32(new->energy_cores, old->energy_cores);
	DELTA_WRAP32(new->energy_gfx, old->energy_gfx);
	DELTA_WRAP32(new->energy_dram, old->energy_dram);
	DELTA_WRAP32(new->rapl_pkg_perf_status, old->rapl_pkg_perf_status);
	DELTA_WRAP32(new->rapl_dram_perf_status, old->rapl_dram_perf_status);
}

void
delta_core(struct core_data *new, struct core_data *old)
{
	old->c3 = new->c3 - old->c3;
	old->c6 = new->c6 - old->c6;
	old->c7 = new->c7 - old->c7;
	old->core_temp_c = new->core_temp_c;
}

/*
 * old = new - old
 */
void
delta_thread(struct thread_data *new, struct thread_data *old,
	struct core_data *core_delta)
{
	old->tsc = new->tsc - old->tsc;

	/* check for TSC < 1 Mcycles over interval */
	if (old->tsc < (1000 * 1000)) {
		fprintf(stderr, "Insanely slow TSC rate, TSC stops in idle?\n");
		fprintf(stderr, "You can disable all c-states by booting with \"idle=poll\"\n");
		fprintf(stderr, "or just the deep ones with \"processor.max_cstate=1\"\n");
		exit(-3);
	}

	old->c1 = new->c1 - old->c1;

	if ((new->aperf > old->aperf) && (new->mperf > old->mperf)) {
		old->aperf = new->aperf - old->aperf;
		old->mperf = new->mperf - old->mperf;
	} else {

		if (!aperf_mperf_unstable) {
			fprintf(stderr, "%s: APERF or MPERF went backwards *\n", progname);
			fprintf(stderr, "* Frequency results do not cover entire interval *\n");
			fprintf(stderr, "* fix this by running Linux-2.6.30 or later *\n");

			aperf_mperf_unstable = 1;
		}
		/*
		 * mperf delta is likely a huge "positive" number
		 * can not use it for calculating c0 time
		 */
		skip_c0 = 1;
		skip_c1 = 1;
	}


	if (use_c1_residency_msr) {
		/*
		 * Some models have a dedicated C1 residency MSR,
		 * which should be more accurate than the derivation below.
		 */
	} else {
		/*
		 * As counter collection is not atomic,
		 * it is possible for mperf's non-halted cycles + idle states
		 * to exceed TSC's all cycles: show c1 = 0% in that case.
		 */
		if ((old->mperf + core_delta->c3 + core_delta->c6 + core_delta->c7) > old->tsc)
			old->c1 = 0;
		else {
			/* normal case, derive c1 */
			old->c1 = old->tsc - old->mperf - core_delta->c3
				- core_delta->c6 - core_delta->c7;
		}
	}

	if (old->mperf == 0) {
		if (verbose > 1) fprintf(stderr, "cpu%d MPERF 0!\n", old->cpu_id);
		old->mperf = 1;	/* divide by 0 protection */
	}

	old->extra_delta32 = new->extra_delta32 - old->extra_delta32;
	old->extra_delta32 &= 0xFFFFFFFF;

	old->extra_delta64 = new->extra_delta64 - old->extra_delta64;

	/*
	 * Extra MSR is just a snapshot, simply copy latest w/o subtracting
	 */
	old->extra_msr32 = new->extra_msr32;
	old->extra_msr64 = new->extra_msr64;

	if (do_smi)
		old->smi_count = new->smi_count - old->smi_count;
}

int delta_cpu(struct thread_data *t, struct core_data *c,
	struct pkg_data *p, struct thread_data *t2,
	struct core_data *c2, struct pkg_data *p2)
{
	/* calculate core delta only for 1st thread in core */
	if (t->flags & CPU_IS_FIRST_THREAD_IN_CORE)
		delta_core(c, c2);

	/* always calculate thread delta */
	delta_thread(t, t2, c2);	/* c2 is core delta */

	/* calculate package delta only for 1st core in package */
	if (t->flags & CPU_IS_FIRST_CORE_IN_PACKAGE)
		delta_package(p, p2);

	return 0;
}

void clear_counters(struct thread_data *t, struct core_data *c, struct pkg_data *p)
{
	t->tsc = 0;
	t->aperf = 0;
	t->mperf = 0;
	t->c1 = 0;

	t->smi_count = 0;
	t->extra_delta32 = 0;
	t->extra_delta64 = 0;

	/* tells format_counters to dump all fields from this set */
	t->flags = CPU_IS_FIRST_THREAD_IN_CORE | CPU_IS_FIRST_CORE_IN_PACKAGE;

	c->c3 = 0;
	c->c6 = 0;
	c->c7 = 0;
	c->core_temp_c = 0;

	p->pc2 = 0;
	p->pc3 = 0;
	p->pc6 = 0;
	p->pc7 = 0;
	p->pc8 = 0;
	p->pc9 = 0;
	p->pc10 = 0;

	p->energy_pkg = 0;
	p->energy_dram = 0;
	p->energy_cores = 0;
	p->energy_gfx = 0;
	p->rapl_pkg_perf_status = 0;
	p->rapl_dram_perf_status = 0;
	p->pkg_temp_c = 0;
}
int sum_counters(struct thread_data *t, struct core_data *c,
	struct pkg_data *p)
{
	average.threads.tsc += t->tsc;
	average.threads.aperf += t->aperf;
	average.threads.mperf += t->mperf;
	average.threads.c1 += t->c1;

	average.threads.extra_delta32 += t->extra_delta32;
	average.threads.extra_delta64 += t->extra_delta64;

	/* sum per-core values only for 1st thread in core */
	if (!(t->flags & CPU_IS_FIRST_THREAD_IN_CORE))
		return 0;

	average.cores.c3 += c->c3;
	average.cores.c6 += c->c6;
	average.cores.c7 += c->c7;

	average.cores.core_temp_c = MAX(average.cores.core_temp_c, c->core_temp_c);

	/* sum per-pkg values only for 1st core in pkg */
	if (!(t->flags & CPU_IS_FIRST_CORE_IN_PACKAGE))
		return 0;

	average.packages.pc2 += p->pc2;
	average.packages.pc3 += p->pc3;
	average.packages.pc6 += p->pc6;
	average.packages.pc7 += p->pc7;
	average.packages.pc8 += p->pc8;
	average.packages.pc9 += p->pc9;
	average.packages.pc10 += p->pc10;

	average.packages.energy_pkg += p->energy_pkg;
	average.packages.energy_dram += p->energy_dram;
	average.packages.energy_cores += p->energy_cores;
	average.packages.energy_gfx += p->energy_gfx;

	average.packages.pkg_temp_c = MAX(average.packages.pkg_temp_c, p->pkg_temp_c);

	average.packages.rapl_pkg_perf_status += p->rapl_pkg_perf_status;
	average.packages.rapl_dram_perf_status += p->rapl_dram_perf_status;
	return 0;
}
/*
 * sum the counters for all cpus in the system
 * compute the weighted average
 */
void compute_average(struct thread_data *t, struct core_data *c,
	struct pkg_data *p)
{
	clear_counters(&average.threads, &average.cores, &average.packages);

	for_all_cpus(sum_counters, t, c, p);

	average.threads.tsc /= topo.num_cpus;
	average.threads.aperf /= topo.num_cpus;
	average.threads.mperf /= topo.num_cpus;
	average.threads.c1 /= topo.num_cpus;

	average.threads.extra_delta32 /= topo.num_cpus;
	average.threads.extra_delta32 &= 0xFFFFFFFF;

	average.threads.extra_delta64 /= topo.num_cpus;

	average.cores.c3 /= topo.num_cores;
	average.cores.c6 /= topo.num_cores;
	average.cores.c7 /= topo.num_cores;

	average.packages.pc2 /= topo.num_packages;
	average.packages.pc3 /= topo.num_packages;
	average.packages.pc6 /= topo.num_packages;
	average.packages.pc7 /= topo.num_packages;

	average.packages.pc8 /= topo.num_packages;
	average.packages.pc9 /= topo.num_packages;
	average.packages.pc10 /= topo.num_packages;
}

static unsigned long long rdtsc(void)
{
	unsigned int low, high;

	asm volatile("rdtsc" : "=a" (low), "=d" (high));

	return low | ((unsigned long long)high) << 32;
}


/*
 * get_counters(...)
 * migrate to cpu
 * acquire and record local counters for that cpu
 */
int get_counters(struct thread_data *t, struct core_data *c, struct pkg_data *p)
{
	int cpu = t->cpu_id;
	unsigned long long msr;

	if (cpu_migrate(cpu)) {
		fprintf(stderr, "Could not migrate to CPU %d\n", cpu);
		return -1;
	}

	t->tsc = rdtsc();	/* we are running on local CPU of interest */

	if (has_aperf) {
		if (get_msr(cpu, MSR_IA32_APERF, &t->aperf))
			return -3;
		if (get_msr(cpu, MSR_IA32_MPERF, &t->mperf))
			return -4;
	}

	if (do_smi) {
		if (get_msr(cpu, MSR_SMI_COUNT, &msr))
			return -5;
		t->smi_count = msr & 0xFFFFFFFF;
	}
	if (extra_delta_offset32) {
		if (get_msr(cpu, extra_delta_offset32, &msr))
			return -5;
		t->extra_delta32 = msr & 0xFFFFFFFF;
	}

	if (extra_delta_offset64)
		if (get_msr(cpu, extra_delta_offset64, &t->extra_delta64))
			return -5;

	if (extra_msr_offset32) {
		if (get_msr(cpu, extra_msr_offset32, &msr))
			return -5;
		t->extra_msr32 = msr & 0xFFFFFFFF;
	}

	if (extra_msr_offset64)
		if (get_msr(cpu, extra_msr_offset64, &t->extra_msr64))
			return -5;

	if (use_c1_residency_msr) {
		if (get_msr(cpu, MSR_CORE_C1_RES, &t->c1))
			return -6;
	}

	/* collect core counters only for 1st thread in core */
	if (!(t->flags & CPU_IS_FIRST_THREAD_IN_CORE))
		return 0;

	if (do_nhm_cstates && !do_slm_cstates) {
		if (get_msr(cpu, MSR_CORE_C3_RESIDENCY, &c->c3))
			return -6;
	}

	if (do_nhm_cstates) {
		if (get_msr(cpu, MSR_CORE_C6_RESIDENCY, &c->c6))
			return -7;
	}

	if (do_snb_cstates)
		if (get_msr(cpu, MSR_CORE_C7_RESIDENCY, &c->c7))
			return -8;

	if (do_dts) {
		if (get_msr(cpu, MSR_IA32_THERM_STATUS, &msr))
			return -9;
		c->core_temp_c = tcc_activation_temp - ((msr >> 16) & 0x7F);
	}


	/* collect package counters only for 1st core in package */
	if (!(t->flags & CPU_IS_FIRST_CORE_IN_PACKAGE))
		return 0;

	if (do_nhm_cstates && !do_slm_cstates) {
		if (get_msr(cpu, MSR_PKG_C3_RESIDENCY, &p->pc3))
			return -9;
		if (get_msr(cpu, MSR_PKG_C6_RESIDENCY, &p->pc6))
			return -10;
	}
	if (do_snb_cstates) {
		if (get_msr(cpu, MSR_PKG_C2_RESIDENCY, &p->pc2))
			return -11;
		if (get_msr(cpu, MSR_PKG_C7_RESIDENCY, &p->pc7))
			return -12;
	}
	if (do_c8_c9_c10) {
		if (get_msr(cpu, MSR_PKG_C8_RESIDENCY, &p->pc8))
			return -13;
		if (get_msr(cpu, MSR_PKG_C9_RESIDENCY, &p->pc9))
			return -13;
		if (get_msr(cpu, MSR_PKG_C10_RESIDENCY, &p->pc10))
			return -13;
	}
	if (do_rapl & RAPL_PKG) {
		if (get_msr(cpu, MSR_PKG_ENERGY_STATUS, &msr))
			return -13;
		p->energy_pkg = msr & 0xFFFFFFFF;
	}
	if (do_rapl & RAPL_CORES) {
		if (get_msr(cpu, MSR_PP0_ENERGY_STATUS, &msr))
			return -14;
		p->energy_cores = msr & 0xFFFFFFFF;
	}
	if (do_rapl & RAPL_DRAM) {
		if (get_msr(cpu, MSR_DRAM_ENERGY_STATUS, &msr))
			return -15;
		p->energy_dram = msr & 0xFFFFFFFF;
	}
	if (do_rapl & RAPL_GFX) {
		if (get_msr(cpu, MSR_PP1_ENERGY_STATUS, &msr))
			return -16;
		p->energy_gfx = msr & 0xFFFFFFFF;
	}
	if (do_rapl & RAPL_PKG_PERF_STATUS) {
		if (get_msr(cpu, MSR_PKG_PERF_STATUS, &msr))
			return -16;
		p->rapl_pkg_perf_status = msr & 0xFFFFFFFF;
	}
	if (do_rapl & RAPL_DRAM_PERF_STATUS) {
		if (get_msr(cpu, MSR_DRAM_PERF_STATUS, &msr))
			return -16;
		p->rapl_dram_perf_status = msr & 0xFFFFFFFF;
	}
	if (do_ptm) {
		if (get_msr(cpu, MSR_IA32_PACKAGE_THERM_STATUS, &msr))
			return -17;
		p->pkg_temp_c = tcc_activation_temp - ((msr >> 16) & 0x7F);
	}
	return 0;
}

void print_verbose_header(void)
{
	unsigned long long msr;
	unsigned int ratio;

	if (!do_nehalem_platform_info)
		return;

	get_msr(0, MSR_NHM_PLATFORM_INFO, &msr);

	fprintf(stderr, "cpu0: MSR_NHM_PLATFORM_INFO: 0x%08llx\n", msr);

	ratio = (msr >> 40) & 0xFF;
	fprintf(stderr, "%d * %.0f = %.0f MHz max efficiency\n",
		ratio, bclk, ratio * bclk);

	ratio = (msr >> 8) & 0xFF;
	fprintf(stderr, "%d * %.0f = %.0f MHz TSC frequency\n",
		ratio, bclk, ratio * bclk);

	get_msr(0, MSR_IA32_POWER_CTL, &msr);
	fprintf(stderr, "cpu0: MSR_IA32_POWER_CTL: 0x%08llx (C1E auto-promotion: %sabled)\n",
		msr, msr & 0x2 ? "EN" : "DIS");

	if (!do_ivt_turbo_ratio_limit)
		goto print_nhm_turbo_ratio_limits;

	get_msr(0, MSR_IVT_TURBO_RATIO_LIMIT, &msr);

	fprintf(stderr, "cpu0: MSR_IVT_TURBO_RATIO_LIMIT: 0x%08llx\n", msr);

	ratio = (msr >> 56) & 0xFF;
	if (ratio)
		fprintf(stderr, "%d * %.0f = %.0f MHz max turbo 16 active cores\n",
			ratio, bclk, ratio * bclk);

	ratio = (msr >> 48) & 0xFF;
	if (ratio)
		fprintf(stderr, "%d * %.0f = %.0f MHz max turbo 15 active cores\n",
			ratio, bclk, ratio * bclk);

	ratio = (msr >> 40) & 0xFF;
	if (ratio)
		fprintf(stderr, "%d * %.0f = %.0f MHz max turbo 14 active cores\n",
			ratio, bclk, ratio * bclk);

	ratio = (msr >> 32) & 0xFF;
	if (ratio)
		fprintf(stderr, "%d * %.0f = %.0f MHz max turbo 13 active cores\n",
			ratio, bclk, ratio * bclk);

	ratio = (msr >> 24) & 0xFF;
	if (ratio)
		fprintf(stderr, "%d * %.0f = %.0f MHz max turbo 12 active cores\n",
			ratio, bclk, ratio * bclk);

	ratio = (msr >> 16) & 0xFF;
	if (ratio)
		fprintf(stderr, "%d * %.0f = %.0f MHz max turbo 11 active cores\n",
			ratio, bclk, ratio * bclk);

	ratio = (msr >> 8) & 0xFF;
	if (ratio)
		fprintf(stderr, "%d * %.0f = %.0f MHz max turbo 10 active cores\n",
			ratio, bclk, ratio * bclk);

	ratio = (msr >> 0) & 0xFF;
	if (ratio)
		fprintf(stderr, "%d * %.0f = %.0f MHz max turbo 9 active cores\n",
			ratio, bclk, ratio * bclk);

print_nhm_turbo_ratio_limits:
	get_msr(0, MSR_NHM_SNB_PKG_CST_CFG_CTL, &msr);

#define SNB_C1_AUTO_UNDEMOTE              (1UL << 27)
#define SNB_C3_AUTO_UNDEMOTE              (1UL << 28)

	fprintf(stderr, "cpu0: MSR_NHM_SNB_PKG_CST_CFG_CTL: 0x%08llx", msr);

	fprintf(stderr, " (%s%s%s%s%slocked: pkg-cstate-limit=%d: ",
		(msr & SNB_C3_AUTO_UNDEMOTE) ? "UNdemote-C3, " : "",
		(msr & SNB_C1_AUTO_UNDEMOTE) ? "UNdemote-C1, " : "",
		(msr & NHM_C3_AUTO_DEMOTE) ? "demote-C3, " : "",
		(msr & NHM_C1_AUTO_DEMOTE) ? "demote-C1, " : "",
		(msr & (1 << 15)) ? "" : "UN",
		(unsigned int)msr & 7);


	switch(msr & 0x7) {
	case 0:
		fprintf(stderr, do_slm_cstates ? "no pkg states" : "pc0");
		break;
	case 1:
		fprintf(stderr, do_slm_cstates ? "no pkg states" : do_snb_cstates ? "pc2" : "pc0");
		break;
	case 2:
		fprintf(stderr, do_slm_cstates ? "invalid" : do_snb_cstates ? "pc6-noret" : "pc3");
		break;
	case 3:
		fprintf(stderr, do_slm_cstates ? "invalid" : "pc6");
		break;
	case 4:
		fprintf(stderr, do_slm_cstates ? "pc4" : "pc7");
		break;
	case 5:
		fprintf(stderr, do_slm_cstates ? "invalid" : do_snb_cstates ? "pc7s" : "invalid");
		break;
	case 6:
		fprintf(stderr, do_slm_cstates ? "pc6" : "invalid");
		break;
	case 7:
		fprintf(stderr, do_slm_cstates ? "pc7" : "unlimited");
		break;
	default:
		fprintf(stderr, "invalid");
	}
	fprintf(stderr, ")\n");

	if (!do_nehalem_turbo_ratio_limit)
		return;

	get_msr(0, MSR_NHM_TURBO_RATIO_LIMIT, &msr);

	fprintf(stderr, "cpu0: MSR_NHM_TURBO_RATIO_LIMIT: 0x%08llx\n", msr);

	ratio = (msr >> 56) & 0xFF;
	if (ratio)
		fprintf(stderr, "%d * %.0f = %.0f MHz max turbo 8 active cores\n",
			ratio, bclk, ratio * bclk);

	ratio = (msr >> 48) & 0xFF;
	if (ratio)
		fprintf(stderr, "%d * %.0f = %.0f MHz max turbo 7 active cores\n",
			ratio, bclk, ratio * bclk);

	ratio = (msr >> 40) & 0xFF;
	if (ratio)
		fprintf(stderr, "%d * %.0f = %.0f MHz max turbo 6 active cores\n",
			ratio, bclk, ratio * bclk);

	ratio = (msr >> 32) & 0xFF;
	if (ratio)
		fprintf(stderr, "%d * %.0f = %.0f MHz max turbo 5 active cores\n",
			ratio, bclk, ratio * bclk);

	ratio = (msr >> 24) & 0xFF;
	if (ratio)
		fprintf(stderr, "%d * %.0f = %.0f MHz max turbo 4 active cores\n",
			ratio, bclk, ratio * bclk);

	ratio = (msr >> 16) & 0xFF;
	if (ratio)
		fprintf(stderr, "%d * %.0f = %.0f MHz max turbo 3 active cores\n",
			ratio, bclk, ratio * bclk);

	ratio = (msr >> 8) & 0xFF;
	if (ratio)
		fprintf(stderr, "%d * %.0f = %.0f MHz max turbo 2 active cores\n",
			ratio, bclk, ratio * bclk);

	ratio = (msr >> 0) & 0xFF;
	if (ratio)
		fprintf(stderr, "%d * %.0f = %.0f MHz max turbo 1 active cores\n",
			ratio, bclk, ratio * bclk);
}

void free_all_buffers(void)
{
	CPU_FREE(cpu_present_set);
	cpu_present_set = NULL;
	cpu_present_set = 0;

	CPU_FREE(cpu_affinity_set);
	cpu_affinity_set = NULL;
	cpu_affinity_setsize = 0;

	free(thread_even);
	free(core_even);
	free(package_even);

	thread_even = NULL;
	core_even = NULL;
	package_even = NULL;

	free(thread_odd);
	free(core_odd);
	free(package_odd);

	thread_odd = NULL;
	core_odd = NULL;
	package_odd = NULL;

	free(output_buffer);
	output_buffer = NULL;
	outp = NULL;
}

/*
 * cpu_is_first_sibling_in_core(cpu)
 * return 1 if given CPU is 1st HT sibling in the core
 */
int cpu_is_first_sibling_in_core(int cpu)
{
	char path[64];
	FILE *filep;
	int first_cpu;

	sprintf(path, "/sys/devices/system/cpu/cpu%d/topology/thread_siblings_list", cpu);
	filep = fopen(path, "r");
	if (filep == NULL) {
		perror(path);
		exit(1);
	}
	fscanf(filep, "%d", &first_cpu);
	fclose(filep);
	return (cpu == first_cpu);
}

/*
 * cpu_is_first_core_in_package(cpu)
 * return 1 if given CPU is 1st core in package
 */
int cpu_is_first_core_in_package(int cpu)
{
	char path[64];
	FILE *filep;
	int first_cpu;

	sprintf(path, "/sys/devices/system/cpu/cpu%d/topology/core_siblings_list", cpu);
	filep = fopen(path, "r");
	if (filep == NULL) {
		perror(path);
		exit(1);
	}
	fscanf(filep, "%d", &first_cpu);
	fclose(filep);
	return (cpu == first_cpu);
}

int get_physical_package_id(int cpu)
{
	char path[80];
	FILE *filep;
	int pkg;

	sprintf(path, "/sys/devices/system/cpu/cpu%d/topology/physical_package_id", cpu);
	filep = fopen(path, "r");
	if (filep == NULL) {
		perror(path);
		exit(1);
	}
	fscanf(filep, "%d", &pkg);
	fclose(filep);
	return pkg;
}

int get_core_id(int cpu)
{
	char path[80];
	FILE *filep;
>>>>>>> refs/remotes/origin/master
	int core;

	sprintf(path, "/sys/devices/system/cpu/cpu%d/topology/core_id", cpu);
	filep = fopen(path, "r");
	if (filep == NULL) {
		perror(path);
		exit(1);
	}
	fscanf(filep, "%d", &core);
	fclose(filep);
	return core;
}

<<<<<<< HEAD
/*
<<<<<<< HEAD
 * run func(index, cpu) on every cpu in /proc/stat
=======
 * run func(pkg, core, cpu) on every cpu in /proc/stat
>>>>>>> refs/remotes/origin/cm-10.0
 */

int for_all_cpus(void (func)(int, int, int))
{
	FILE *fp;
	int cpu_count;
=======
int get_num_ht_siblings(int cpu)
{
	char path[80];
	FILE *filep;
	int sib1, sib2;
	int matches;
	char character;

	sprintf(path, "/sys/devices/system/cpu/cpu%d/topology/thread_siblings_list", cpu);
	filep = fopen(path, "r");
	if (filep == NULL) {
		perror(path);
		exit(1);
	}
	/*
	 * file format:
	 * if a pair of number with a character between: 2 siblings (eg. 1-2, or 1,4)
	 * otherwinse 1 sibling (self).
	 */
	matches = fscanf(filep, "%d%c%d\n", &sib1, &character, &sib2);

	fclose(filep);

	if (matches == 3)
		return 2;
	else
		return 1;
}

/*
 * run func(thread, core, package) in topology order
 * skip non-present cpus
 */

int for_all_cpus_2(int (func)(struct thread_data *, struct core_data *,
	struct pkg_data *, struct thread_data *, struct core_data *,
	struct pkg_data *), struct thread_data *thread_base,
	struct core_data *core_base, struct pkg_data *pkg_base,
	struct thread_data *thread_base2, struct core_data *core_base2,
	struct pkg_data *pkg_base2)
{
	int retval, pkg_no, core_no, thread_no;

	for (pkg_no = 0; pkg_no < topo.num_packages; ++pkg_no) {
		for (core_no = 0; core_no < topo.num_cores_per_pkg; ++core_no) {
			for (thread_no = 0; thread_no <
				topo.num_threads_per_core; ++thread_no) {
				struct thread_data *t, *t2;
				struct core_data *c, *c2;
				struct pkg_data *p, *p2;

				t = GET_THREAD(thread_base, thread_no, core_no, pkg_no);

				if (cpu_is_not_present(t->cpu_id))
					continue;

				t2 = GET_THREAD(thread_base2, thread_no, core_no, pkg_no);

				c = GET_CORE(core_base, core_no, pkg_no);
				c2 = GET_CORE(core_base2, core_no, pkg_no);

				p = GET_PKG(pkg_base, pkg_no);
				p2 = GET_PKG(pkg_base2, pkg_no);

				retval = func(t, c, p, t2, c2, p2);
				if (retval)
					return retval;
			}
		}
	}
	return 0;
}

/*
 * run func(cpu) on every cpu in /proc/stat
 * return max_cpu number
 */
int for_all_proc_cpus(int (func)(int))
{
	FILE *fp;
	int cpu_num;
>>>>>>> refs/remotes/origin/master
	int retval;

	fp = fopen(proc_stat, "r");
	if (fp == NULL) {
		perror(proc_stat);
		exit(1);
	}

<<<<<<< HEAD
	retval = fscanf(fp, "cpu %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d\n");
	if (retval != 0) {
		perror("/proc/stat format");
		exit(1);
	}

	for (cpu_count = 0; ; cpu_count++) {
		int cpu;

		retval = fscanf(fp, "cpu%u %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d\n", &cpu);
		if (retval != 1)
			break;

		func(get_physical_package_id(cpu), get_core_id(cpu), cpu);
	}
	fclose(fp);
	return cpu_count;
}

void re_initialize(void)
{
<<<<<<< HEAD
	printf("turbostat: topology changed, re-initializing.\n");
	free_all_counters();
	num_cpus = for_all_cpus(alloc_new_counters);
	need_reinitialize = 0;
	printf("num_cpus is now %d\n", num_cpus);
=======
	free_all_counters();
	num_cpus = for_all_cpus(alloc_new_counters);
	cpu_mask_uninit();
	cpu_mask_init(num_cpus);
	printf("turbostat: re-initialized with num_cpus %d\n", num_cpus);
>>>>>>> refs/remotes/origin/cm-10.0
}

void dummy(int pkg, int core, int cpu) { return; }
/*
 * check to see if a cpu came on-line
 */
<<<<<<< HEAD
void verify_num_cpus(void)
=======
int verify_num_cpus(void)
>>>>>>> refs/remotes/origin/cm-10.0
{
	int new_num_cpus;

	new_num_cpus = for_all_cpus(dummy);

	if (new_num_cpus != num_cpus) {
		if (verbose)
			printf("num_cpus was %d, is now  %d\n",
				num_cpus, new_num_cpus);
<<<<<<< HEAD
		need_reinitialize = 1;
	}
=======
		return -1;
	}
	return 0;
>>>>>>> refs/remotes/origin/cm-10.0
}

void turbostat_loop()
{
restart:
	get_counters(cnt_even);
	gettimeofday(&tv_even, (struct timezone *)NULL);

	while (1) {
<<<<<<< HEAD
		verify_num_cpus();
		if (need_reinitialize) {
=======
		if (verify_num_cpus()) {
>>>>>>> refs/remotes/origin/cm-10.0
			re_initialize();
			goto restart;
		}
		sleep(interval_sec);
<<<<<<< HEAD
		get_counters(cnt_odd);
		gettimeofday(&tv_odd, (struct timezone *)NULL);

=======
		if (get_counters(cnt_odd)) {
			re_initialize();
			goto restart;
		}
		gettimeofday(&tv_odd, (struct timezone *)NULL);
>>>>>>> refs/remotes/origin/cm-10.0
		compute_delta(cnt_odd, cnt_even, cnt_delta);
		timersub(&tv_odd, &tv_even, &tv_delta);
		compute_average(cnt_delta, cnt_average);
		print_counters(cnt_delta);
<<<<<<< HEAD
		if (need_reinitialize) {
			re_initialize();
			goto restart;
		}
		sleep(interval_sec);
		get_counters(cnt_even);
=======
		sleep(interval_sec);
		if (get_counters(cnt_even)) {
			re_initialize();
			goto restart;
		}
>>>>>>> refs/remotes/origin/cm-10.0
		gettimeofday(&tv_even, (struct timezone *)NULL);
		compute_delta(cnt_even, cnt_odd, cnt_delta);
		timersub(&tv_even, &tv_odd, &tv_delta);
		compute_average(cnt_delta, cnt_average);
		print_counters(cnt_delta);
	}
}

void check_dev_msr()
{
	struct stat sb;

	if (stat("/dev/cpu/0/msr", &sb)) {
		fprintf(stderr, "no /dev/cpu/0/msr\n");
		fprintf(stderr, "Try \"# modprobe msr\"\n");
		exit(-5);
	}
}

void check_super_user()
{
	if (getuid() != 0) {
		fprintf(stderr, "must be root\n");
		exit(-6);
	}
}

int has_nehalem_turbo_ratio_limit(unsigned int family, unsigned int model)
=======
	retval = fscanf(fp, "cpu %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d\n");
	if (retval != 0) {
		perror("/proc/stat format");
		exit(1);
	}

	while (1) {
		retval = fscanf(fp, "cpu%u %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d\n", &cpu_num);
		if (retval != 1)
			break;

		retval = func(cpu_num);
		if (retval) {
			fclose(fp);
			return(retval);
		}
	}
	fclose(fp);
	return 0;
}

void re_initialize(void)
{
	free_all_buffers();
	setup_all_buffers();
	printf("turbostat: re-initialized with num_cpus %d\n", topo.num_cpus);
}


/*
 * count_cpus()
 * remember the last one seen, it will be the max
 */
int count_cpus(int cpu)
{
	if (topo.max_cpu_num < cpu)
		topo.max_cpu_num = cpu;

	topo.num_cpus += 1;
	return 0;
}
int mark_cpu_present(int cpu)
{
	CPU_SET_S(cpu, cpu_present_setsize, cpu_present_set);
	return 0;
}

void turbostat_loop()
{
	int retval;
	int restarted = 0;

restart:
	restarted++;

	retval = for_all_cpus(get_counters, EVEN_COUNTERS);
	if (retval < -1) {
		exit(retval);
	} else if (retval == -1) {
		if (restarted > 1) {
			exit(retval);
		}
		re_initialize();
		goto restart;
	}
	restarted = 0;
	gettimeofday(&tv_even, (struct timezone *)NULL);

	while (1) {
		if (for_all_proc_cpus(cpu_is_not_present)) {
			re_initialize();
			goto restart;
		}
		sleep(interval_sec);
		retval = for_all_cpus(get_counters, ODD_COUNTERS);
		if (retval < -1) {
			exit(retval);
		} else if (retval == -1) {
			re_initialize();
			goto restart;
		}
		gettimeofday(&tv_odd, (struct timezone *)NULL);
		timersub(&tv_odd, &tv_even, &tv_delta);
		for_all_cpus_2(delta_cpu, ODD_COUNTERS, EVEN_COUNTERS);
		compute_average(EVEN_COUNTERS);
		format_all_counters(EVEN_COUNTERS);
		flush_stdout();
		sleep(interval_sec);
		retval = for_all_cpus(get_counters, EVEN_COUNTERS);
		if (retval < -1) {
			exit(retval);
		} else if (retval == -1) {
			re_initialize();
			goto restart;
		}
		gettimeofday(&tv_even, (struct timezone *)NULL);
		timersub(&tv_even, &tv_odd, &tv_delta);
		for_all_cpus_2(delta_cpu, EVEN_COUNTERS, ODD_COUNTERS);
		compute_average(ODD_COUNTERS);
		format_all_counters(ODD_COUNTERS);
		flush_stdout();
	}
}

void check_dev_msr()
{
	struct stat sb;

	if (stat("/dev/cpu/0/msr", &sb)) {
		fprintf(stderr, "no /dev/cpu/0/msr\n");
		fprintf(stderr, "Try \"# modprobe msr\"\n");
		exit(-5);
	}
}

void check_super_user()
{
	if (getuid() != 0) {
		fprintf(stderr, "must be root\n");
		exit(-6);
	}
}

int has_nehalem_turbo_ratio_limit(unsigned int family, unsigned int model)
{
	if (!genuine_intel)
		return 0;

	if (family != 6)
		return 0;

	switch (model) {
	case 0x1A:	/* Core i7, Xeon 5500 series - Bloomfield, Gainstown NHM-EP */
	case 0x1E:	/* Core i7 and i5 Processor - Clarksfield, Lynnfield, Jasper Forest */
	case 0x1F:	/* Core i7 and i5 Processor - Nehalem */
	case 0x25:	/* Westmere Client - Clarkdale, Arrandale */
	case 0x2C:	/* Westmere EP - Gulftown */
	case 0x2A:	/* SNB */
	case 0x2D:	/* SNB Xeon */
	case 0x3A:	/* IVB */
	case 0x3E:	/* IVB Xeon */
	case 0x3C:	/* HSW */
	case 0x3F:	/* HSW */
	case 0x45:	/* HSW */
	case 0x46:	/* HSW */
	case 0x37:	/* BYT */
	case 0x4D:	/* AVN */
		return 1;
	case 0x2E:	/* Nehalem-EX Xeon - Beckton */
	case 0x2F:	/* Westmere-EX Xeon - Eagleton */
	default:
		return 0;
	}
}
int has_ivt_turbo_ratio_limit(unsigned int family, unsigned int model)
{
	if (!genuine_intel)
		return 0;

	if (family != 6)
		return 0;

	switch (model) {
	case 0x3E:	/* IVB Xeon */
		return 1;
	default:
		return 0;
	}
}

/*
 * print_epb()
 * Decode the ENERGY_PERF_BIAS MSR
 */
int print_epb(struct thread_data *t, struct core_data *c, struct pkg_data *p)
{
	unsigned long long msr;
	char *epb_string;
	int cpu;

	if (!has_epb)
		return 0;

	cpu = t->cpu_id;

	/* EPB is per-package */
	if (!(t->flags & CPU_IS_FIRST_THREAD_IN_CORE) || !(t->flags & CPU_IS_FIRST_CORE_IN_PACKAGE))
		return 0;

	if (cpu_migrate(cpu)) {
		fprintf(stderr, "Could not migrate to CPU %d\n", cpu);
		return -1;
	}

	if (get_msr(cpu, MSR_IA32_ENERGY_PERF_BIAS, &msr))
		return 0;

	switch (msr & 0x7) {
	case ENERGY_PERF_BIAS_PERFORMANCE:
		epb_string = "performance";
		break;
	case ENERGY_PERF_BIAS_NORMAL:
		epb_string = "balanced";
		break;
	case ENERGY_PERF_BIAS_POWERSAVE:
		epb_string = "powersave";
		break;
	default:
		epb_string = "custom";
		break;
	}
	fprintf(stderr, "cpu%d: MSR_IA32_ENERGY_PERF_BIAS: 0x%08llx (%s)\n", cpu, msr, epb_string);

	return 0;
}

#define	RAPL_POWER_GRANULARITY	0x7FFF	/* 15 bit power granularity */
#define	RAPL_TIME_GRANULARITY	0x3F /* 6 bit time granularity */

double get_tdp(model)
{
	unsigned long long msr;

	if (do_rapl & RAPL_PKG_POWER_INFO)
		if (!get_msr(0, MSR_PKG_POWER_INFO, &msr))
			return ((msr >> 0) & RAPL_POWER_GRANULARITY) * rapl_power_units;

	switch (model) {
	case 0x37:
	case 0x4D:
		return 30.0;
	default:
		return 135.0;
	}
}


/*
 * rapl_probe()
 *
 * sets do_rapl, rapl_power_units, rapl_energy_units, rapl_time_units
 */
void rapl_probe(unsigned int family, unsigned int model)
{
	unsigned long long msr;
	unsigned int time_unit;
	double tdp;

	if (!genuine_intel)
		return;

	if (family != 6)
		return;

	switch (model) {
	case 0x2A:
	case 0x3A:
	case 0x3C:	/* HSW */
	case 0x3F:	/* HSW */
	case 0x45:	/* HSW */
	case 0x46:	/* HSW */
		do_rapl = RAPL_PKG | RAPL_CORES | RAPL_CORE_POLICY | RAPL_GFX | RAPL_PKG_POWER_INFO;
		break;
	case 0x2D:
	case 0x3E:
		do_rapl = RAPL_PKG | RAPL_CORES | RAPL_CORE_POLICY | RAPL_DRAM | RAPL_PKG_PERF_STATUS | RAPL_DRAM_PERF_STATUS | RAPL_PKG_POWER_INFO;
		break;
	case 0x37:	/* BYT */
	case 0x4D:	/* AVN */
		do_rapl = RAPL_PKG | RAPL_CORES ;
		break;
	default:
		return;
	}

	/* units on package 0, verify later other packages match */
	if (get_msr(0, MSR_RAPL_POWER_UNIT, &msr))
		return;

	rapl_power_units = 1.0 / (1 << (msr & 0xF));
	if (model == 0x37)
		rapl_energy_units = 1.0 * (1 << (msr >> 8 & 0x1F)) / 1000000;
	else
		rapl_energy_units = 1.0 / (1 << (msr >> 8 & 0x1F));

	time_unit = msr >> 16 & 0xF;
	if (time_unit == 0)
		time_unit = 0xA;

	rapl_time_units = 1.0 / (1 << (time_unit));

	tdp = get_tdp(model);

	rapl_joule_counter_range = 0xFFFFFFFF * rapl_energy_units / tdp;
	if (verbose)
		fprintf(stderr, "RAPL: %.0f sec. Joule Counter Range, at %.0f Watts\n", rapl_joule_counter_range, tdp);

	return;
}

int print_thermal(struct thread_data *t, struct core_data *c, struct pkg_data *p)
{
	unsigned long long msr;
	unsigned int dts;
	int cpu;

	if (!(do_dts || do_ptm))
		return 0;

	cpu = t->cpu_id;

	/* DTS is per-core, no need to print for each thread */
	if (!(t->flags & CPU_IS_FIRST_THREAD_IN_CORE)) 
		return 0;

	if (cpu_migrate(cpu)) {
		fprintf(stderr, "Could not migrate to CPU %d\n", cpu);
		return -1;
	}

	if (do_ptm && (t->flags & CPU_IS_FIRST_CORE_IN_PACKAGE)) {
		if (get_msr(cpu, MSR_IA32_PACKAGE_THERM_STATUS, &msr))
			return 0;

		dts = (msr >> 16) & 0x7F;
		fprintf(stderr, "cpu%d: MSR_IA32_PACKAGE_THERM_STATUS: 0x%08llx (%d C)\n",
			cpu, msr, tcc_activation_temp - dts);

#ifdef	THERM_DEBUG
		if (get_msr(cpu, MSR_IA32_PACKAGE_THERM_INTERRUPT, &msr))
			return 0;

		dts = (msr >> 16) & 0x7F;
		dts2 = (msr >> 8) & 0x7F;
		fprintf(stderr, "cpu%d: MSR_IA32_PACKAGE_THERM_INTERRUPT: 0x%08llx (%d C, %d C)\n",
			cpu, msr, tcc_activation_temp - dts, tcc_activation_temp - dts2);
#endif
	}


	if (do_dts) {
		unsigned int resolution;

		if (get_msr(cpu, MSR_IA32_THERM_STATUS, &msr))
			return 0;

		dts = (msr >> 16) & 0x7F;
		resolution = (msr >> 27) & 0xF;
		fprintf(stderr, "cpu%d: MSR_IA32_THERM_STATUS: 0x%08llx (%d C +/- %d)\n",
			cpu, msr, tcc_activation_temp - dts, resolution);

#ifdef THERM_DEBUG
		if (get_msr(cpu, MSR_IA32_THERM_INTERRUPT, &msr))
			return 0;

		dts = (msr >> 16) & 0x7F;
		dts2 = (msr >> 8) & 0x7F;
		fprintf(stderr, "cpu%d: MSR_IA32_THERM_INTERRUPT: 0x%08llx (%d C, %d C)\n",
			cpu, msr, tcc_activation_temp - dts, tcc_activation_temp - dts2);
#endif
	}

	return 0;
}
	
void print_power_limit_msr(int cpu, unsigned long long msr, char *label)
{
	fprintf(stderr, "cpu%d: %s: %sabled (%f Watts, %f sec, clamp %sabled)\n",
		cpu, label,
		((msr >> 15) & 1) ? "EN" : "DIS",
		((msr >> 0) & 0x7FFF) * rapl_power_units,
		(1.0 + (((msr >> 22) & 0x3)/4.0)) * (1 << ((msr >> 17) & 0x1F)) * rapl_time_units,
		(((msr >> 16) & 1) ? "EN" : "DIS"));

	return;
}

int print_rapl(struct thread_data *t, struct core_data *c, struct pkg_data *p)
{
	unsigned long long msr;
	int cpu;

	if (!do_rapl)
		return 0;

	/* RAPL counters are per package, so print only for 1st thread/package */
	if (!(t->flags & CPU_IS_FIRST_THREAD_IN_CORE) || !(t->flags & CPU_IS_FIRST_CORE_IN_PACKAGE))
		return 0;

	cpu = t->cpu_id;
	if (cpu_migrate(cpu)) {
		fprintf(stderr, "Could not migrate to CPU %d\n", cpu);
		return -1;
	}

	if (get_msr(cpu, MSR_RAPL_POWER_UNIT, &msr))
		return -1;

	if (verbose) {
		fprintf(stderr, "cpu%d: MSR_RAPL_POWER_UNIT: 0x%08llx "
			"(%f Watts, %f Joules, %f sec.)\n", cpu, msr,
			rapl_power_units, rapl_energy_units, rapl_time_units);
	}
	if (do_rapl & RAPL_PKG_POWER_INFO) {

		if (get_msr(cpu, MSR_PKG_POWER_INFO, &msr))
                	return -5;


		fprintf(stderr, "cpu%d: MSR_PKG_POWER_INFO: 0x%08llx (%.0f W TDP, RAPL %.0f - %.0f W, %f sec.)\n",
			cpu, msr,
			((msr >>  0) & RAPL_POWER_GRANULARITY) * rapl_power_units,
			((msr >> 16) & RAPL_POWER_GRANULARITY) * rapl_power_units,
			((msr >> 32) & RAPL_POWER_GRANULARITY) * rapl_power_units,
			((msr >> 48) & RAPL_TIME_GRANULARITY) * rapl_time_units);

	}
	if (do_rapl & RAPL_PKG) {

		if (get_msr(cpu, MSR_PKG_POWER_LIMIT, &msr))
			return -9;

		fprintf(stderr, "cpu%d: MSR_PKG_POWER_LIMIT: 0x%08llx (%slocked)\n",
			cpu, msr, (msr >> 63) & 1 ? "": "UN");

		print_power_limit_msr(cpu, msr, "PKG Limit #1");
		fprintf(stderr, "cpu%d: PKG Limit #2: %sabled (%f Watts, %f* sec, clamp %sabled)\n",
			cpu,
			((msr >> 47) & 1) ? "EN" : "DIS",
			((msr >> 32) & 0x7FFF) * rapl_power_units,
			(1.0 + (((msr >> 54) & 0x3)/4.0)) * (1 << ((msr >> 49) & 0x1F)) * rapl_time_units,
			((msr >> 48) & 1) ? "EN" : "DIS");
	}

	if (do_rapl & RAPL_DRAM) {
		if (get_msr(cpu, MSR_DRAM_POWER_INFO, &msr))
                	return -6;


		fprintf(stderr, "cpu%d: MSR_DRAM_POWER_INFO,: 0x%08llx (%.0f W TDP, RAPL %.0f - %.0f W, %f sec.)\n",
			cpu, msr,
			((msr >>  0) & RAPL_POWER_GRANULARITY) * rapl_power_units,
			((msr >> 16) & RAPL_POWER_GRANULARITY) * rapl_power_units,
			((msr >> 32) & RAPL_POWER_GRANULARITY) * rapl_power_units,
			((msr >> 48) & RAPL_TIME_GRANULARITY) * rapl_time_units);


		if (get_msr(cpu, MSR_DRAM_POWER_LIMIT, &msr))
			return -9;
		fprintf(stderr, "cpu%d: MSR_DRAM_POWER_LIMIT: 0x%08llx (%slocked)\n",
				cpu, msr, (msr >> 31) & 1 ? "": "UN");

		print_power_limit_msr(cpu, msr, "DRAM Limit");
	}
	if (do_rapl & RAPL_CORE_POLICY) {
		if (verbose) {
			if (get_msr(cpu, MSR_PP0_POLICY, &msr))
				return -7;

			fprintf(stderr, "cpu%d: MSR_PP0_POLICY: %lld\n", cpu, msr & 0xF);
		}
	}
	if (do_rapl & RAPL_CORES) {
		if (verbose) {

			if (get_msr(cpu, MSR_PP0_POWER_LIMIT, &msr))
				return -9;
			fprintf(stderr, "cpu%d: MSR_PP0_POWER_LIMIT: 0x%08llx (%slocked)\n",
					cpu, msr, (msr >> 31) & 1 ? "": "UN");
			print_power_limit_msr(cpu, msr, "Cores Limit");
		}
	}
	if (do_rapl & RAPL_GFX) {
		if (verbose) {
			if (get_msr(cpu, MSR_PP1_POLICY, &msr))
				return -8;

			fprintf(stderr, "cpu%d: MSR_PP1_POLICY: %lld\n", cpu, msr & 0xF);

			if (get_msr(cpu, MSR_PP1_POWER_LIMIT, &msr))
				return -9;
			fprintf(stderr, "cpu%d: MSR_PP1_POWER_LIMIT: 0x%08llx (%slocked)\n",
					cpu, msr, (msr >> 31) & 1 ? "": "UN");
			print_power_limit_msr(cpu, msr, "GFX Limit");
		}
	}
	return 0;
}


int is_snb(unsigned int family, unsigned int model)
{
	if (!genuine_intel)
		return 0;

	switch (model) {
	case 0x2A:
	case 0x2D:
	case 0x3A:	/* IVB */
	case 0x3E:	/* IVB Xeon */
	case 0x3C:	/* HSW */
	case 0x3F:	/* HSW */
	case 0x45:	/* HSW */
	case 0x46:	/* HSW */
		return 1;
	}
	return 0;
}

int has_c8_c9_c10(unsigned int family, unsigned int model)
>>>>>>> refs/remotes/origin/master
{
	if (!genuine_intel)
		return 0;

<<<<<<< HEAD
	if (family != 6)
		return 0;

	switch (model) {
	case 0x1A:	/* Core i7, Xeon 5500 series - Bloomfield, Gainstown NHM-EP */
	case 0x1E:	/* Core i7 and i5 Processor - Clarksfield, Lynnfield, Jasper Forest */
	case 0x1F:	/* Core i7 and i5 Processor - Nehalem */
	case 0x25:	/* Westmere Client - Clarkdale, Arrandale */
	case 0x2C:	/* Westmere EP - Gulftown */
	case 0x2A:	/* SNB */
	case 0x2D:	/* SNB Xeon */
<<<<<<< HEAD
=======
	case 0x3A:	/* IVB */
	case 0x3D:	/* IVB Xeon */
>>>>>>> refs/remotes/origin/cm-10.0
		return 1;
	case 0x2E:	/* Nehalem-EX Xeon - Beckton */
	case 0x2F:	/* Westmere-EX Xeon - Eagleton */
	default:
		return 0;
	}
}

int is_snb(unsigned int family, unsigned int model)
{
	if (!genuine_intel)
		return 0;

	switch (model) {
	case 0x2A:
	case 0x2D:
=======
	switch (model) {
	case 0x45:
		return 1;
	}
	return 0;
}


int is_slm(unsigned int family, unsigned int model)
{
	if (!genuine_intel)
		return 0;
	switch (model) {
	case 0x37:	/* BYT */
	case 0x4D:	/* AVN */
>>>>>>> refs/remotes/origin/master
		return 1;
	}
	return 0;
}

<<<<<<< HEAD
=======
#define SLM_BCLK_FREQS 5
double slm_freq_table[SLM_BCLK_FREQS] = { 83.3, 100.0, 133.3, 116.7, 80.0};

double slm_bclk(void)
{
	unsigned long long msr = 3;
	unsigned int i;
	double freq;

	if (get_msr(0, MSR_FSB_FREQ, &msr))
		fprintf(stderr, "SLM BCLK: unknown\n");

	i = msr & 0xf;
	if (i >= SLM_BCLK_FREQS) {
		fprintf(stderr, "SLM BCLK[%d] invalid\n", i);
		msr = 3;
	}
	freq = slm_freq_table[i];

	fprintf(stderr, "SLM BCLK: %.1f Mhz\n", freq);

	return freq;
}

>>>>>>> refs/remotes/origin/master
double discover_bclk(unsigned int family, unsigned int model)
{
	if (is_snb(family, model))
		return 100.00;
<<<<<<< HEAD
=======
	else if (is_slm(family, model))
		return slm_bclk();
>>>>>>> refs/remotes/origin/master
	else
		return 133.33;
}

<<<<<<< HEAD
=======
/*
 * MSR_IA32_TEMPERATURE_TARGET indicates the temperature where
 * the Thermal Control Circuit (TCC) activates.
 * This is usually equal to tjMax.
 *
 * Older processors do not have this MSR, so there we guess,
 * but also allow cmdline over-ride with -T.
 *
 * Several MSR temperature values are in units of degrees-C
 * below this value, including the Digital Thermal Sensor (DTS),
 * Package Thermal Management Sensor (PTM), and thermal event thresholds.
 */
int set_temperature_target(struct thread_data *t, struct core_data *c, struct pkg_data *p)
{
	unsigned long long msr;
	unsigned int target_c_local;
	int cpu;

	/* tcc_activation_temp is used only for dts or ptm */
	if (!(do_dts || do_ptm))
		return 0;

	/* this is a per-package concept */
	if (!(t->flags & CPU_IS_FIRST_THREAD_IN_CORE) || !(t->flags & CPU_IS_FIRST_CORE_IN_PACKAGE))
		return 0;

	cpu = t->cpu_id;
	if (cpu_migrate(cpu)) {
		fprintf(stderr, "Could not migrate to CPU %d\n", cpu);
		return -1;
	}

	if (tcc_activation_temp_override != 0) {
		tcc_activation_temp = tcc_activation_temp_override;
		fprintf(stderr, "cpu%d: Using cmdline TCC Target (%d C)\n",
			cpu, tcc_activation_temp);
		return 0;
	}

	/* Temperature Target MSR is Nehalem and newer only */
	if (!do_nehalem_platform_info)
		goto guess;

	if (get_msr(0, MSR_IA32_TEMPERATURE_TARGET, &msr))
		goto guess;

	target_c_local = (msr >> 16) & 0x7F;

	if (verbose)
		fprintf(stderr, "cpu%d: MSR_IA32_TEMPERATURE_TARGET: 0x%08llx (%d C)\n",
			cpu, msr, target_c_local);

	if (target_c_local < 85 || target_c_local > 127)
		goto guess;

	tcc_activation_temp = target_c_local;

	return 0;

guess:
	tcc_activation_temp = TJMAX_DEFAULT;
	fprintf(stderr, "cpu%d: Guessing tjMax %d C, Please use -T to specify\n",
		cpu, tcc_activation_temp);

	return 0;
}
>>>>>>> refs/remotes/origin/master
void check_cpuid()
{
	unsigned int eax, ebx, ecx, edx, max_level;
	unsigned int fms, family, model, stepping;

	eax = ebx = ecx = edx = 0;

	asm("cpuid" : "=a" (max_level), "=b" (ebx), "=c" (ecx), "=d" (edx) : "a" (0));

	if (ebx == 0x756e6547 && edx == 0x49656e69 && ecx == 0x6c65746e)
		genuine_intel = 1;

	if (verbose)
<<<<<<< HEAD
		fprintf(stderr, "%.4s%.4s%.4s ",
=======
		fprintf(stderr, "CPUID(0): %.4s%.4s%.4s ",
>>>>>>> refs/remotes/origin/master
			(char *)&ebx, (char *)&edx, (char *)&ecx);

	asm("cpuid" : "=a" (fms), "=c" (ecx), "=d" (edx) : "a" (1) : "ebx");
	family = (fms >> 8) & 0xf;
	model = (fms >> 4) & 0xf;
	stepping = fms & 0xf;
	if (family == 6 || family == 0xf)
		model += ((fms >> 16) & 0xf) << 4;

	if (verbose)
		fprintf(stderr, "%d CPUID levels; family:model:stepping 0x%x:%x:%x (%d:%d:%d)\n",
			max_level, family, model, stepping, family, model, stepping);

	if (!(edx & (1 << 5))) {
		fprintf(stderr, "CPUID: no MSR\n");
		exit(1);
	}

	/*
	 * check max extended function levels of CPUID.
	 * This is needed to check for invariant TSC.
	 * This check is valid for both Intel and AMD.
	 */
	ebx = ecx = edx = 0;
	asm("cpuid" : "=a" (max_level), "=b" (ebx), "=c" (ecx), "=d" (edx) : "a" (0x80000000));

	if (max_level < 0x80000007) {
		fprintf(stderr, "CPUID: no invariant TSC (max_level 0x%x)\n", max_level);
		exit(1);
	}

	/*
	 * Non-Stop TSC is advertised by CPUID.EAX=0x80000007: EDX.bit8
	 * this check is valid for both Intel and AMD
	 */
	asm("cpuid" : "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx) : "a" (0x80000007));
	has_invariant_tsc = edx & (1 << 8);

	if (!has_invariant_tsc) {
		fprintf(stderr, "No invariant TSC\n");
		exit(1);
	}

	/*
	 * APERF/MPERF is advertised by CPUID.EAX=0x6: ECX.bit0
	 * this check is valid for both Intel and AMD
	 */

	asm("cpuid" : "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx) : "a" (0x6));
	has_aperf = ecx & (1 << 0);
<<<<<<< HEAD
	if (!has_aperf) {
		fprintf(stderr, "No APERF MSR\n");
		exit(1);
	}

	do_nehalem_platform_info = genuine_intel && has_invariant_tsc;
	do_nhm_cstates = genuine_intel;	/* all Intel w/ non-stop TSC have NHM counters */
	do_snb_cstates = is_snb(family, model);
	bclk = discover_bclk(family, model);

	do_nehalem_turbo_ratio_limit = has_nehalem_turbo_ratio_limit(family, model);
=======
	do_dts = eax & (1 << 0);
	do_ptm = eax & (1 << 6);
	has_epb = ecx & (1 << 3);

	if (verbose)
		fprintf(stderr, "CPUID(6): %s%s%s%s\n",
			has_aperf ? "APERF" : "No APERF!",
			do_dts ? ", DTS" : "",
			do_ptm ? ", PTM": "",
			has_epb ? ", EPB": "");

	if (!has_aperf)
		exit(-1);

	do_nehalem_platform_info = genuine_intel && has_invariant_tsc;
	do_nhm_cstates = genuine_intel;	/* all Intel w/ non-stop TSC have NHM counters */
	do_smi = do_nhm_cstates;
	do_snb_cstates = is_snb(family, model);
	do_c8_c9_c10 = has_c8_c9_c10(family, model);
	do_slm_cstates = is_slm(family, model);
	bclk = discover_bclk(family, model);

	do_nehalem_turbo_ratio_limit = has_nehalem_turbo_ratio_limit(family, model);
	do_ivt_turbo_ratio_limit = has_ivt_turbo_ratio_limit(family, model);
	rapl_probe(family, model);

	return;
>>>>>>> refs/remotes/origin/master
}


void usage()
{
<<<<<<< HEAD
	fprintf(stderr, "%s: [-v] [-M MSR#] [-i interval_sec | command ...]\n",
=======
	fprintf(stderr, "%s: [-v][-R][-T][-p|-P|-S][-c MSR# | -s]][-C MSR#][-m MSR#][-M MSR#][-i interval_sec | command ...]\n",
>>>>>>> refs/remotes/origin/master
		progname);
	exit(1);
}


/*
 * in /dev/cpu/ return success for names that are numbers
 * ie. filter out ".", "..", "microcode".
 */
int dir_filter(const struct dirent *dirp)
{
	if (isdigit(dirp->d_name[0]))
		return 1;
	else
		return 0;
}

int open_dev_cpu_msr(int dummy1)
{
	return 0;
}

<<<<<<< HEAD
=======
void topology_probe()
{
	int i;
	int max_core_id = 0;
	int max_package_id = 0;
	int max_siblings = 0;
	struct cpu_topology {
		int core_id;
		int physical_package_id;
	} *cpus;

	/* Initialize num_cpus, max_cpu_num */
	topo.num_cpus = 0;
	topo.max_cpu_num = 0;
	for_all_proc_cpus(count_cpus);
	if (!summary_only && topo.num_cpus > 1)
		show_cpu = 1;

	if (verbose > 1)
		fprintf(stderr, "num_cpus %d max_cpu_num %d\n", topo.num_cpus, topo.max_cpu_num);

	cpus = calloc(1, (topo.max_cpu_num  + 1) * sizeof(struct cpu_topology));
	if (cpus == NULL) {
		perror("calloc cpus");
		exit(1);
	}

	/*
	 * Allocate and initialize cpu_present_set
	 */
	cpu_present_set = CPU_ALLOC((topo.max_cpu_num + 1));
	if (cpu_present_set == NULL) {
		perror("CPU_ALLOC");
		exit(3);
	}
	cpu_present_setsize = CPU_ALLOC_SIZE((topo.max_cpu_num + 1));
	CPU_ZERO_S(cpu_present_setsize, cpu_present_set);
	for_all_proc_cpus(mark_cpu_present);

	/*
	 * Allocate and initialize cpu_affinity_set
	 */
	cpu_affinity_set = CPU_ALLOC((topo.max_cpu_num + 1));
	if (cpu_affinity_set == NULL) {
		perror("CPU_ALLOC");
		exit(3);
	}
	cpu_affinity_setsize = CPU_ALLOC_SIZE((topo.max_cpu_num + 1));
	CPU_ZERO_S(cpu_affinity_setsize, cpu_affinity_set);


	/*
	 * For online cpus
	 * find max_core_id, max_package_id
	 */
	for (i = 0; i <= topo.max_cpu_num; ++i) {
		int siblings;

		if (cpu_is_not_present(i)) {
			if (verbose > 1)
				fprintf(stderr, "cpu%d NOT PRESENT\n", i);
			continue;
		}
		cpus[i].core_id = get_core_id(i);
		if (cpus[i].core_id > max_core_id)
			max_core_id = cpus[i].core_id;

		cpus[i].physical_package_id = get_physical_package_id(i);
		if (cpus[i].physical_package_id > max_package_id)
			max_package_id = cpus[i].physical_package_id;

		siblings = get_num_ht_siblings(i);
		if (siblings > max_siblings)
			max_siblings = siblings;
		if (verbose > 1)
			fprintf(stderr, "cpu %d pkg %d core %d\n",
				i, cpus[i].physical_package_id, cpus[i].core_id);
	}
	topo.num_cores_per_pkg = max_core_id + 1;
	if (verbose > 1)
		fprintf(stderr, "max_core_id %d, sizing for %d cores per package\n",
			max_core_id, topo.num_cores_per_pkg);
	if (!summary_only && topo.num_cores_per_pkg > 1)
		show_core = 1;

	topo.num_packages = max_package_id + 1;
	if (verbose > 1)
		fprintf(stderr, "max_package_id %d, sizing for %d packages\n",
			max_package_id, topo.num_packages);
	if (!summary_only && topo.num_packages > 1)
		show_pkg = 1;

	topo.num_threads_per_core = max_siblings;
	if (verbose > 1)
		fprintf(stderr, "max_siblings %d\n", max_siblings);

	free(cpus);
}

void
allocate_counters(struct thread_data **t, struct core_data **c, struct pkg_data **p)
{
	int i;

	*t = calloc(topo.num_threads_per_core * topo.num_cores_per_pkg *
		topo.num_packages, sizeof(struct thread_data));
	if (*t == NULL)
		goto error;

	for (i = 0; i < topo.num_threads_per_core *
		topo.num_cores_per_pkg * topo.num_packages; i++)
		(*t)[i].cpu_id = -1;

	*c = calloc(topo.num_cores_per_pkg * topo.num_packages,
		sizeof(struct core_data));
	if (*c == NULL)
		goto error;

	for (i = 0; i < topo.num_cores_per_pkg * topo.num_packages; i++)
		(*c)[i].core_id = -1;

	*p = calloc(topo.num_packages, sizeof(struct pkg_data));
	if (*p == NULL)
		goto error;

	for (i = 0; i < topo.num_packages; i++)
		(*p)[i].package_id = i;

	return;
error:
	perror("calloc counters");
	exit(1);
}
/*
 * init_counter()
 *
 * set cpu_id, core_num, pkg_num
 * set FIRST_THREAD_IN_CORE and FIRST_CORE_IN_PACKAGE
 *
 * increment topo.num_cores when 1st core in pkg seen
 */
void init_counter(struct thread_data *thread_base, struct core_data *core_base,
	struct pkg_data *pkg_base, int thread_num, int core_num,
	int pkg_num, int cpu_id)
{
	struct thread_data *t;
	struct core_data *c;
	struct pkg_data *p;

	t = GET_THREAD(thread_base, thread_num, core_num, pkg_num);
	c = GET_CORE(core_base, core_num, pkg_num);
	p = GET_PKG(pkg_base, pkg_num);

	t->cpu_id = cpu_id;
	if (thread_num == 0) {
		t->flags |= CPU_IS_FIRST_THREAD_IN_CORE;
		if (cpu_is_first_core_in_package(cpu_id))
			t->flags |= CPU_IS_FIRST_CORE_IN_PACKAGE;
	}

	c->core_id = core_num;
	p->package_id = pkg_num;
}


int initialize_counters(int cpu_id)
{
	int my_thread_id, my_core_id, my_package_id;

	my_package_id = get_physical_package_id(cpu_id);
	my_core_id = get_core_id(cpu_id);

	if (cpu_is_first_sibling_in_core(cpu_id)) {
		my_thread_id = 0;
		topo.num_cores++;
	} else {
		my_thread_id = 1;
	}

	init_counter(EVEN_COUNTERS, my_thread_id, my_core_id, my_package_id, cpu_id);
	init_counter(ODD_COUNTERS, my_thread_id, my_core_id, my_package_id, cpu_id);
	return 0;
}

void allocate_output_buffer()
{
	output_buffer = calloc(1, (1 + topo.num_cpus) * 256);
	outp = output_buffer;
	if (outp == NULL) {
		perror("calloc");
		exit(-1);
	}
}

void setup_all_buffers(void)
{
	topology_probe();
	allocate_counters(&thread_even, &core_even, &package_even);
	allocate_counters(&thread_odd, &core_odd, &package_odd);
	allocate_output_buffer();
	for_all_proc_cpus(initialize_counters);
}
>>>>>>> refs/remotes/origin/master
void turbostat_init()
{
	check_cpuid();

	check_dev_msr();
	check_super_user();

<<<<<<< HEAD
	num_cpus = for_all_cpus(alloc_new_counters);
<<<<<<< HEAD
=======
	cpu_mask_init(num_cpus);
>>>>>>> refs/remotes/origin/cm-10.0

	if (verbose)
		print_nehalem_info();
=======
	setup_all_buffers();

	if (verbose)
		print_verbose_header();

	if (verbose)
		for_all_cpus(print_epb, ODD_COUNTERS);

	if (verbose)
		for_all_cpus(print_rapl, ODD_COUNTERS);

	for_all_cpus(set_temperature_target, ODD_COUNTERS);

	if (verbose)
		for_all_cpus(print_thermal, ODD_COUNTERS);
>>>>>>> refs/remotes/origin/master
}

int fork_it(char **argv)
{
<<<<<<< HEAD
	int retval;
	pid_t child_pid;
	get_counters(cnt_even);
=======
	pid_t child_pid;
	int status;

	status = for_all_cpus(get_counters, EVEN_COUNTERS);
	if (status)
		exit(status);
	/* clear affinity side-effect of get_counters() */
	sched_setaffinity(0, cpu_present_setsize, cpu_present_set);
>>>>>>> refs/remotes/origin/master
	gettimeofday(&tv_even, (struct timezone *)NULL);

	child_pid = fork();
	if (!child_pid) {
		/* child */
		execvp(argv[0], argv);
	} else {
<<<<<<< HEAD
		int status;
=======
>>>>>>> refs/remotes/origin/master

		/* parent */
		if (child_pid == -1) {
			perror("fork");
			exit(1);
		}

		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		if (waitpid(child_pid, &status, 0) == -1) {
			perror("wait");
<<<<<<< HEAD
			exit(1);
		}
	}
	get_counters(cnt_odd);
	gettimeofday(&tv_odd, (struct timezone *)NULL);
	retval = compute_delta(cnt_odd, cnt_even, cnt_delta);

	timersub(&tv_odd, &tv_even, &tv_delta);
	compute_average(cnt_delta, cnt_average);
	if (!retval)
		print_counters(cnt_delta);

	fprintf(stderr, "%.6f sec\n", tv_delta.tv_sec + tv_delta.tv_usec/1000000.0);

	return 0;
=======
			exit(status);
		}
	}
	/*
	 * n.b. fork_it() does not check for errors from for_all_cpus()
	 * because re-starting is problematic when forking
	 */
	for_all_cpus(get_counters, ODD_COUNTERS);
	gettimeofday(&tv_odd, (struct timezone *)NULL);
	timersub(&tv_odd, &tv_even, &tv_delta);
	for_all_cpus_2(delta_cpu, ODD_COUNTERS, EVEN_COUNTERS);
	compute_average(EVEN_COUNTERS);
	format_all_counters(EVEN_COUNTERS);
	flush_stderr();

	fprintf(stderr, "%.6f sec\n", tv_delta.tv_sec + tv_delta.tv_usec/1000000.0);

	return status;
>>>>>>> refs/remotes/origin/master
}

void cmdline(int argc, char **argv)
{
	int opt;

	progname = argv[0];

<<<<<<< HEAD
<<<<<<< HEAD
	while ((opt = getopt(argc, argv, "+vi:M:")) != -1) {
		switch (opt) {
=======
	while ((opt = getopt(argc, argv, "+svi:M:")) != -1) {
		switch (opt) {
		case 's':
			summary_only++;
			break;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	while ((opt = getopt(argc, argv, "+pPSvi:sc:sC:m:M:RT:")) != -1) {
		switch (opt) {
		case 'p':
			show_core_only++;
			break;
		case 'P':
			show_pkg_only++;
			break;
		case 'S':
			summary_only++;
			break;
>>>>>>> refs/remotes/origin/master
		case 'v':
			verbose++;
			break;
		case 'i':
			interval_sec = atoi(optarg);
			break;
<<<<<<< HEAD
		case 'M':
			sscanf(optarg, "%x", &extra_msr_offset);
			if (verbose > 1)
				fprintf(stderr, "MSR 0x%X\n", extra_msr_offset);
=======
		case 'c':
			sscanf(optarg, "%x", &extra_delta_offset32);
			break;
		case 'C':
			sscanf(optarg, "%x", &extra_delta_offset64);
			break;
		case 'm':
			sscanf(optarg, "%x", &extra_msr_offset32);
			break;
		case 'M':
			sscanf(optarg, "%x", &extra_msr_offset64);
			break;
		case 'R':
			rapl_verbose++;
			break;
		case 'T':
			tcc_activation_temp_override = atoi(optarg);
>>>>>>> refs/remotes/origin/master
			break;
		default:
			usage();
		}
	}
}

int main(int argc, char **argv)
{
	cmdline(argc, argv);

<<<<<<< HEAD
	if (verbose > 1)
		fprintf(stderr, "turbostat Dec 6, 2010"
			" - Len Brown <lenb@kernel.org>\n");
	if (verbose > 1)
		fprintf(stderr, "http://userweb.kernel.org/~lenb/acpi/utils/pmtools/turbostat/\n");
=======
	if (verbose)
		fprintf(stderr, "turbostat v3.5 April 26, 2013"
			" - Len Brown <lenb@kernel.org>\n");
>>>>>>> refs/remotes/origin/master

	turbostat_init();

	/*
	 * if any params left, it must be a command to fork
	 */
	if (argc - optind)
		return fork_it(argv + optind);
	else
		turbostat_loop();

	return 0;
}
