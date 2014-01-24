/*
 * mem-memcpy.c
 *
 * memcpy: Simple memory copy in various ways
 *
 * Written by Hitoshi Mitake <mitake@dcl.info.waseda.ac.jp>
 */
<<<<<<< HEAD
<<<<<<< HEAD
#include <ctype.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#include "../perf.h"
#include "../util/util.h"
#include "../util/parse-options.h"
#include "../util/header.h"
#include "bench.h"
#include "mem-memcpy-arch.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>

#define K 1024

static const char	*length_str	= "1MB";
static const char	*routine	= "default";
<<<<<<< HEAD
<<<<<<< HEAD
=======
static int		iterations	= 1;
>>>>>>> refs/remotes/origin/cm-10.0
static bool		use_clock;
static int		clock_fd;
=======
static int		iterations	= 1;
static bool		use_cycle;
static int		cycle_fd;
>>>>>>> refs/remotes/origin/master
static bool		only_prefault;
static bool		no_prefault;

static const struct option options[] = {
	OPT_STRING('l', "length", &length_str, "1MB",
		    "Specify length of memory to copy. "
<<<<<<< HEAD
		    "available unit: B, MB, GB (upper and lower)"),
	OPT_STRING('r', "routine", &routine, "default",
		    "Specify routine to copy"),
<<<<<<< HEAD
=======
	OPT_INTEGER('i', "iterations", &iterations,
		    "repeat memcpy() invocation this number of times"),
>>>>>>> refs/remotes/origin/cm-10.0
	OPT_BOOLEAN('c', "clock", &use_clock,
		    "Use CPU clock for measuring"),
=======
		    "Available units: B, KB, MB, GB and TB (upper and lower)"),
	OPT_STRING('r', "routine", &routine, "default",
		    "Specify routine to copy"),
	OPT_INTEGER('i', "iterations", &iterations,
		    "repeat memcpy() invocation this number of times"),
	OPT_BOOLEAN('c', "cycle", &use_cycle,
		    "Use cycles event instead of gettimeofday() for measuring"),
>>>>>>> refs/remotes/origin/master
	OPT_BOOLEAN('o', "only-prefault", &only_prefault,
		    "Show only the result with page faults before memcpy()"),
	OPT_BOOLEAN('n', "no-prefault", &no_prefault,
		    "Show only the result without page faults before memcpy()"),
	OPT_END()
};

typedef void *(*memcpy_t)(void *, const void *, size_t);

struct routine {
	const char *name;
	const char *desc;
	memcpy_t fn;
};

struct routine routines[] = {
	{ "default",
	  "Default memcpy() provided by glibc",
	  memcpy },
<<<<<<< HEAD
#ifdef ARCH_X86_64
=======
#ifdef HAVE_ARCH_X86_64_SUPPORT
>>>>>>> refs/remotes/origin/master

#define MEMCPY_FN(fn, name, desc) { name, desc, fn },
#include "mem-memcpy-x86-64-asm-def.h"
#undef MEMCPY_FN

#endif

	{ NULL,
	  NULL,
	  NULL   }
};

static const char * const bench_mem_memcpy_usage[] = {
	"perf bench mem memcpy <options>",
	NULL
};

<<<<<<< HEAD
static struct perf_event_attr clock_attr = {
=======
static struct perf_event_attr cycle_attr = {
>>>>>>> refs/remotes/origin/master
	.type		= PERF_TYPE_HARDWARE,
	.config		= PERF_COUNT_HW_CPU_CYCLES
};

<<<<<<< HEAD
static void init_clock(void)
{
	clock_fd = sys_perf_event_open(&clock_attr, getpid(), -1, -1, 0);

	if (clock_fd < 0 && errno == ENOSYS)
		die("No CONFIG_PERF_EVENTS=y kernel support configured?\n");
	else
		BUG_ON(clock_fd < 0);
}

static u64 get_clock(void)
=======
static void init_cycle(void)
{
	cycle_fd = sys_perf_event_open(&cycle_attr, getpid(), -1, -1, 0);

	if (cycle_fd < 0 && errno == ENOSYS)
		die("No CONFIG_PERF_EVENTS=y kernel support configured?\n");
	else
		BUG_ON(cycle_fd < 0);
}

static u64 get_cycle(void)
>>>>>>> refs/remotes/origin/master
{
	int ret;
	u64 clk;

<<<<<<< HEAD
	ret = read(clock_fd, &clk, sizeof(u64));
=======
	ret = read(cycle_fd, &clk, sizeof(u64));
>>>>>>> refs/remotes/origin/master
	BUG_ON(ret != sizeof(u64));

	return clk;
}

static double timeval2double(struct timeval *ts)
{
	return (double)ts->tv_sec +
		(double)ts->tv_usec / (double)1000000;
}

static void alloc_mem(void **dst, void **src, size_t length)
{
	*dst = zalloc(length);
<<<<<<< HEAD
	if (!dst)
		die("memory allocation failed - maybe length is too large?\n");

	*src = zalloc(length);
	if (!src)
		die("memory allocation failed - maybe length is too large?\n");
}

static u64 do_memcpy_clock(memcpy_t fn, size_t len, bool prefault)
{
	u64 clock_start = 0ULL, clock_end = 0ULL;
	void *src = NULL, *dst = NULL;
<<<<<<< HEAD
=======
	int i;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!*dst)
		die("memory allocation failed - maybe length is too large?\n");

	*src = zalloc(length);
	if (!*src)
		die("memory allocation failed - maybe length is too large?\n");
	/* Make sure to always replace the zero pages even if MMAP_THRESH is crossed */
	memset(*src, 0, length);
}

static u64 do_memcpy_cycle(memcpy_t fn, size_t len, bool prefault)
{
	u64 cycle_start = 0ULL, cycle_end = 0ULL;
	void *src = NULL, *dst = NULL;
	int i;
>>>>>>> refs/remotes/origin/master

	alloc_mem(&src, &dst, len);

	if (prefault)
		fn(dst, src, len);

<<<<<<< HEAD
	clock_start = get_clock();
<<<<<<< HEAD
	fn(dst, src, len);
=======
	for (i = 0; i < iterations; ++i)
		fn(dst, src, len);
>>>>>>> refs/remotes/origin/cm-10.0
	clock_end = get_clock();

	free(src);
	free(dst);
	return clock_end - clock_start;
=======
	cycle_start = get_cycle();
	for (i = 0; i < iterations; ++i)
		fn(dst, src, len);
	cycle_end = get_cycle();

	free(src);
	free(dst);
	return cycle_end - cycle_start;
>>>>>>> refs/remotes/origin/master
}

static double do_memcpy_gettimeofday(memcpy_t fn, size_t len, bool prefault)
{
	struct timeval tv_start, tv_end, tv_diff;
	void *src = NULL, *dst = NULL;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	int i;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	int i;
>>>>>>> refs/remotes/origin/master

	alloc_mem(&src, &dst, len);

	if (prefault)
		fn(dst, src, len);

	BUG_ON(gettimeofday(&tv_start, NULL));
<<<<<<< HEAD
<<<<<<< HEAD
	fn(dst, src, len);
=======
	for (i = 0; i < iterations; ++i)
		fn(dst, src, len);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	for (i = 0; i < iterations; ++i)
		fn(dst, src, len);
>>>>>>> refs/remotes/origin/master
	BUG_ON(gettimeofday(&tv_end, NULL));

	timersub(&tv_end, &tv_start, &tv_diff);

	free(src);
	free(dst);
	return (double)((double)len / timeval2double(&tv_diff));
}

#define pf (no_prefault ? 0 : 1)

#define print_bps(x) do {					\
		if (x < K)					\
			printf(" %14lf B/Sec", x);		\
		else if (x < K * K)				\
			printf(" %14lfd KB/Sec", x / K);	\
		else if (x < K * K * K)				\
			printf(" %14lf MB/Sec", x / K / K);	\
		else						\
			printf(" %14lf GB/Sec", x / K / K / K); \
	} while (0)

int bench_mem_memcpy(int argc, const char **argv,
<<<<<<< HEAD
		     const char *prefix __used)
=======
		     const char *prefix __maybe_unused)
>>>>>>> refs/remotes/origin/master
{
	int i;
	size_t len;
	double result_bps[2];
<<<<<<< HEAD
	u64 result_clock[2];
=======
	u64 result_cycle[2];
>>>>>>> refs/remotes/origin/master

	argc = parse_options(argc, argv, options,
			     bench_mem_memcpy_usage, 0);

<<<<<<< HEAD
	if (use_clock)
		init_clock();

	len = (size_t)perf_atoll((char *)length_str);

	result_clock[0] = result_clock[1] = 0ULL;
=======
	if (use_cycle)
		init_cycle();

	len = (size_t)perf_atoll((char *)length_str);

	result_cycle[0] = result_cycle[1] = 0ULL;
>>>>>>> refs/remotes/origin/master
	result_bps[0] = result_bps[1] = 0.0;

	if ((s64)len <= 0) {
		fprintf(stderr, "Invalid length:%s\n", length_str);
		return 1;
	}

	/* same to without specifying either of prefault and no-prefault */
	if (only_prefault && no_prefault)
		only_prefault = no_prefault = false;

	for (i = 0; routines[i].name; i++) {
		if (!strcmp(routines[i].name, routine))
			break;
	}
	if (!routines[i].name) {
		printf("Unknown routine:%s\n", routine);
		printf("Available routines...\n");
		for (i = 0; routines[i].name; i++) {
			printf("\t%s ... %s\n",
			       routines[i].name, routines[i].desc);
		}
		return 1;
	}

	if (bench_format == BENCH_FORMAT_DEFAULT)
		printf("# Copying %s Bytes ...\n\n", length_str);

	if (!only_prefault && !no_prefault) {
		/* show both of results */
<<<<<<< HEAD
		if (use_clock) {
			result_clock[0] =
				do_memcpy_clock(routines[i].fn, len, false);
			result_clock[1] =
				do_memcpy_clock(routines[i].fn, len, true);
=======
		if (use_cycle) {
			result_cycle[0] =
				do_memcpy_cycle(routines[i].fn, len, false);
			result_cycle[1] =
				do_memcpy_cycle(routines[i].fn, len, true);
>>>>>>> refs/remotes/origin/master
		} else {
			result_bps[0] =
				do_memcpy_gettimeofday(routines[i].fn,
						len, false);
			result_bps[1] =
				do_memcpy_gettimeofday(routines[i].fn,
						len, true);
		}
	} else {
<<<<<<< HEAD
		if (use_clock) {
			result_clock[pf] =
				do_memcpy_clock(routines[i].fn,
=======
		if (use_cycle) {
			result_cycle[pf] =
				do_memcpy_cycle(routines[i].fn,
>>>>>>> refs/remotes/origin/master
						len, only_prefault);
		} else {
			result_bps[pf] =
				do_memcpy_gettimeofday(routines[i].fn,
						len, only_prefault);
		}
	}

	switch (bench_format) {
	case BENCH_FORMAT_DEFAULT:
		if (!only_prefault && !no_prefault) {
<<<<<<< HEAD
			if (use_clock) {
				printf(" %14lf Clock/Byte\n",
					(double)result_clock[0]
					/ (double)len);
				printf(" %14lf Clock/Byte (with prefault)\n",
					(double)result_clock[1]
=======
			if (use_cycle) {
				printf(" %14lf Cycle/Byte\n",
					(double)result_cycle[0]
					/ (double)len);
				printf(" %14lf Cycle/Byte (with prefault)\n",
					(double)result_cycle[1]
>>>>>>> refs/remotes/origin/master
					/ (double)len);
			} else {
				print_bps(result_bps[0]);
				printf("\n");
				print_bps(result_bps[1]);
				printf(" (with prefault)\n");
			}
		} else {
<<<<<<< HEAD
			if (use_clock) {
				printf(" %14lf Clock/Byte",
					(double)result_clock[pf]
=======
			if (use_cycle) {
				printf(" %14lf Cycle/Byte",
					(double)result_cycle[pf]
>>>>>>> refs/remotes/origin/master
					/ (double)len);
			} else
				print_bps(result_bps[pf]);

			printf("%s\n", only_prefault ? " (with prefault)" : "");
		}
		break;
	case BENCH_FORMAT_SIMPLE:
		if (!only_prefault && !no_prefault) {
<<<<<<< HEAD
			if (use_clock) {
				printf("%lf %lf\n",
					(double)result_clock[0] / (double)len,
					(double)result_clock[1] / (double)len);
=======
			if (use_cycle) {
				printf("%lf %lf\n",
					(double)result_cycle[0] / (double)len,
					(double)result_cycle[1] / (double)len);
>>>>>>> refs/remotes/origin/master
			} else {
				printf("%lf %lf\n",
					result_bps[0], result_bps[1]);
			}
		} else {
<<<<<<< HEAD
			if (use_clock) {
				printf("%lf\n", (double)result_clock[pf]
=======
			if (use_cycle) {
				printf("%lf\n", (double)result_cycle[pf]
>>>>>>> refs/remotes/origin/master
					/ (double)len);
			} else
				printf("%lf\n", result_bps[pf]);
		}
		break;
	default:
		/* reaching this means there's some disaster: */
		die("unknown format: %d\n", bench_format);
		break;
	}

	return 0;
}
