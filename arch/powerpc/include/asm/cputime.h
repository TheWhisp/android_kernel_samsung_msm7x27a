/*
 * Definitions for measuring cputime on powerpc machines.
 *
 * Copyright (C) 2006 Paul Mackerras, IBM Corp.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 *
 * If we have CONFIG_VIRT_CPU_ACCOUNTING, we measure cpu time in
 * the same units as the timebase.  Otherwise we measure cpu time
 * in jiffies using the generic definitions.
 */

#ifndef __POWERPC_CPUTIME_H
#define __POWERPC_CPUTIME_H

#ifndef CONFIG_VIRT_CPU_ACCOUNTING
#include <asm-generic/cputime.h>
#ifdef __KERNEL__
static inline void setup_cputime_one_jiffy(void) { }
#endif
#else

#include <linux/types.h>
#include <linux/time.h>
#include <asm/div64.h>
#include <asm/time.h>
#include <asm/param.h>

<<<<<<< HEAD
typedef u64 cputime_t;
typedef u64 cputime64_t;

#define cputime_zero			((cputime_t)0)
#define cputime_max			((~((cputime_t)0) >> 1) - 1)
#define cputime_add(__a, __b)		((__a) +  (__b))
#define cputime_sub(__a, __b)		((__a) -  (__b))
#define cputime_div(__a, __n)		((__a) /  (__n))
#define cputime_halve(__a)		((__a) >> 1)
#define cputime_eq(__a, __b)		((__a) == (__b))
#define cputime_gt(__a, __b)		((__a) >  (__b))
#define cputime_ge(__a, __b)		((__a) >= (__b))
#define cputime_lt(__a, __b)		((__a) <  (__b))
#define cputime_le(__a, __b)		((__a) <= (__b))

#define cputime64_zero			((cputime64_t)0)
#define cputime64_add(__a, __b)		((__a) + (__b))
#define cputime64_sub(__a, __b)		((__a) - (__b))
#define cputime_to_cputime64(__ct)	(__ct)
=======
typedef u64 __nocast cputime_t;
typedef u64 __nocast cputime64_t;
>>>>>>> refs/remotes/origin/cm-10.0

#ifdef __KERNEL__

/*
 * One jiffy in timebase units computed during initialization
 */
extern cputime_t cputime_one_jiffy;

/*
 * Convert cputime <-> jiffies
 */
extern u64 __cputime_jiffies_factor;
DECLARE_PER_CPU(unsigned long, cputime_last_delta);
DECLARE_PER_CPU(unsigned long, cputime_scaled_last_delta);

static inline unsigned long cputime_to_jiffies(const cputime_t ct)
{
<<<<<<< HEAD
	return mulhdu(ct, __cputime_jiffies_factor);
=======
	return mulhdu((__force u64) ct, __cputime_jiffies_factor);
>>>>>>> refs/remotes/origin/cm-10.0
}

/* Estimate the scaled cputime by scaling the real cputime based on
 * the last scaled to real ratio */
static inline cputime_t cputime_to_scaled(const cputime_t ct)
{
	if (cpu_has_feature(CPU_FTR_SPURR) &&
	    __get_cpu_var(cputime_last_delta))
<<<<<<< HEAD
		return ct * __get_cpu_var(cputime_scaled_last_delta) /
			    __get_cpu_var(cputime_last_delta);
=======
		return (__force u64) ct *
			__get_cpu_var(cputime_scaled_last_delta) /
			__get_cpu_var(cputime_last_delta);
>>>>>>> refs/remotes/origin/cm-10.0
	return ct;
}

static inline cputime_t jiffies_to_cputime(const unsigned long jif)
{
<<<<<<< HEAD
	cputime_t ct;
=======
	u64 ct;
>>>>>>> refs/remotes/origin/cm-10.0
	unsigned long sec;

	/* have to be a little careful about overflow */
	ct = jif % HZ;
	sec = jif / HZ;
	if (ct) {
		ct *= tb_ticks_per_sec;
		do_div(ct, HZ);
	}
	if (sec)
		ct += (cputime_t) sec * tb_ticks_per_sec;
<<<<<<< HEAD
	return ct;
=======
	return (__force cputime_t) ct;
>>>>>>> refs/remotes/origin/cm-10.0
}

static inline void setup_cputime_one_jiffy(void)
{
	cputime_one_jiffy = jiffies_to_cputime(1);
}

static inline cputime64_t jiffies64_to_cputime64(const u64 jif)
{
<<<<<<< HEAD
	cputime_t ct;
=======
	u64 ct;
>>>>>>> refs/remotes/origin/cm-10.0
	u64 sec;

	/* have to be a little careful about overflow */
	ct = jif % HZ;
	sec = jif / HZ;
	if (ct) {
		ct *= tb_ticks_per_sec;
		do_div(ct, HZ);
	}
	if (sec)
<<<<<<< HEAD
		ct += (cputime_t) sec * tb_ticks_per_sec;
	return ct;
=======
		ct += (u64) sec * tb_ticks_per_sec;
	return (__force cputime64_t) ct;
>>>>>>> refs/remotes/origin/cm-10.0
}

static inline u64 cputime64_to_jiffies64(const cputime_t ct)
{
<<<<<<< HEAD
	return mulhdu(ct, __cputime_jiffies_factor);
=======
	return mulhdu((__force u64) ct, __cputime_jiffies_factor);
>>>>>>> refs/remotes/origin/cm-10.0
}

/*
 * Convert cputime <-> microseconds
 */
extern u64 __cputime_usec_factor;

static inline unsigned long cputime_to_usecs(const cputime_t ct)
{
<<<<<<< HEAD
	return mulhdu(ct, __cputime_usec_factor);
=======
	return mulhdu((__force u64) ct, __cputime_usec_factor);
>>>>>>> refs/remotes/origin/cm-10.0
}

static inline cputime_t usecs_to_cputime(const unsigned long us)
{
<<<<<<< HEAD
	cputime_t ct;
=======
	u64 ct;
>>>>>>> refs/remotes/origin/cm-10.0
	unsigned long sec;

	/* have to be a little careful about overflow */
	ct = us % 1000000;
	sec = us / 1000000;
	if (ct) {
		ct *= tb_ticks_per_sec;
		do_div(ct, 1000000);
	}
	if (sec)
		ct += (cputime_t) sec * tb_ticks_per_sec;
<<<<<<< HEAD
	return ct;
}

=======
	return (__force cputime_t) ct;
}

#define usecs_to_cputime64(us)		usecs_to_cputime(us)

>>>>>>> refs/remotes/origin/cm-10.0
/*
 * Convert cputime <-> seconds
 */
extern u64 __cputime_sec_factor;

static inline unsigned long cputime_to_secs(const cputime_t ct)
{
<<<<<<< HEAD
	return mulhdu(ct, __cputime_sec_factor);
=======
	return mulhdu((__force u64) ct, __cputime_sec_factor);
>>>>>>> refs/remotes/origin/cm-10.0
}

static inline cputime_t secs_to_cputime(const unsigned long sec)
{
<<<<<<< HEAD
	return (cputime_t) sec * tb_ticks_per_sec;
=======
	return (__force cputime_t)((u64) sec * tb_ticks_per_sec);
>>>>>>> refs/remotes/origin/cm-10.0
}

/*
 * Convert cputime <-> timespec
 */
static inline void cputime_to_timespec(const cputime_t ct, struct timespec *p)
{
<<<<<<< HEAD
	u64 x = ct;
=======
	u64 x = (__force u64) ct;
>>>>>>> refs/remotes/origin/cm-10.0
	unsigned int frac;

	frac = do_div(x, tb_ticks_per_sec);
	p->tv_sec = x;
	x = (u64) frac * 1000000000;
	do_div(x, tb_ticks_per_sec);
	p->tv_nsec = x;
}

static inline cputime_t timespec_to_cputime(const struct timespec *p)
{
<<<<<<< HEAD
	cputime_t ct;

	ct = (u64) p->tv_nsec * tb_ticks_per_sec;
	do_div(ct, 1000000000);
	return ct + (u64) p->tv_sec * tb_ticks_per_sec;
=======
	u64 ct;

	ct = (u64) p->tv_nsec * tb_ticks_per_sec;
	do_div(ct, 1000000000);
	return (__force cputime_t)(ct + (u64) p->tv_sec * tb_ticks_per_sec);
>>>>>>> refs/remotes/origin/cm-10.0
}

/*
 * Convert cputime <-> timeval
 */
static inline void cputime_to_timeval(const cputime_t ct, struct timeval *p)
{
<<<<<<< HEAD
	u64 x = ct;
=======
	u64 x = (__force u64) ct;
>>>>>>> refs/remotes/origin/cm-10.0
	unsigned int frac;

	frac = do_div(x, tb_ticks_per_sec);
	p->tv_sec = x;
	x = (u64) frac * 1000000;
	do_div(x, tb_ticks_per_sec);
	p->tv_usec = x;
}

static inline cputime_t timeval_to_cputime(const struct timeval *p)
{
<<<<<<< HEAD
	cputime_t ct;

	ct = (u64) p->tv_usec * tb_ticks_per_sec;
	do_div(ct, 1000000);
	return ct + (u64) p->tv_sec * tb_ticks_per_sec;
=======
	u64 ct;

	ct = (u64) p->tv_usec * tb_ticks_per_sec;
	do_div(ct, 1000000);
	return (__force cputime_t)(ct + (u64) p->tv_sec * tb_ticks_per_sec);
>>>>>>> refs/remotes/origin/cm-10.0
}

/*
 * Convert cputime <-> clock_t (units of 1/USER_HZ seconds)
 */
extern u64 __cputime_clockt_factor;

static inline unsigned long cputime_to_clock_t(const cputime_t ct)
{
<<<<<<< HEAD
	return mulhdu(ct, __cputime_clockt_factor);
=======
	return mulhdu((__force u64) ct, __cputime_clockt_factor);
>>>>>>> refs/remotes/origin/cm-10.0
}

static inline cputime_t clock_t_to_cputime(const unsigned long clk)
{
<<<<<<< HEAD
	cputime_t ct;
=======
	u64 ct;
>>>>>>> refs/remotes/origin/cm-10.0
	unsigned long sec;

	/* have to be a little careful about overflow */
	ct = clk % USER_HZ;
	sec = clk / USER_HZ;
	if (ct) {
		ct *= tb_ticks_per_sec;
		do_div(ct, USER_HZ);
	}
	if (sec)
<<<<<<< HEAD
		ct += (cputime_t) sec * tb_ticks_per_sec;
	return ct;
=======
		ct += (u64) sec * tb_ticks_per_sec;
	return (__force cputime_t) ct;
>>>>>>> refs/remotes/origin/cm-10.0
}

#define cputime64_to_clock_t(ct)	cputime_to_clock_t((cputime_t)(ct))

#endif /* __KERNEL__ */
#endif /* CONFIG_VIRT_CPU_ACCOUNTING */
#endif /* __POWERPC_CPUTIME_H */
