/*
 * Copyright 2011 Tilera Corporation. All Rights Reserved.
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation, version 2.
 *
 *   This program is distributed in the hope that it will be useful, but
 *   WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
 *   NON INFRINGEMENT.  See the GNU General Public License for
 *   more details.
 */

#ifndef _ASM_TILE_BITOPS_64_H
#define _ASM_TILE_BITOPS_64_H

#include <linux/compiler.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/atomic.h>
#include <asm/system.h>
=======
#include <linux/atomic.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <asm/cmpxchg.h>
>>>>>>> refs/remotes/origin/master

/* See <asm/bitops.h> for API comments. */

static inline void set_bit(unsigned nr, volatile unsigned long *addr)
{
	unsigned long mask = (1UL << (nr % BITS_PER_LONG));
	__insn_fetchor((void *)(addr + nr / BITS_PER_LONG), mask);
}

static inline void clear_bit(unsigned nr, volatile unsigned long *addr)
{
	unsigned long mask = (1UL << (nr % BITS_PER_LONG));
	__insn_fetchand((void *)(addr + nr / BITS_PER_LONG), ~mask);
}

#define smp_mb__before_clear_bit()	smp_mb()
#define smp_mb__after_clear_bit()	smp_mb()


static inline void change_bit(unsigned nr, volatile unsigned long *addr)
{
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long old, mask = (1UL << (nr % BITS_PER_LONG));
	long guess, oldval;
	addr += nr / BITS_PER_LONG;
	old = *addr;
=======
=======
>>>>>>> refs/remotes/origin/master
	unsigned long mask = (1UL << (nr % BITS_PER_LONG));
	unsigned long guess, oldval;
	addr += nr / BITS_PER_LONG;
	oldval = *addr;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	do {
		guess = oldval;
		oldval = atomic64_cmpxchg((atomic64_t *)addr,
					  guess, guess ^ mask);
=======
	do {
		guess = oldval;
		oldval = cmpxchg(addr, guess, guess ^ mask);
>>>>>>> refs/remotes/origin/master
	} while (guess != oldval);
}


/*
 * The test_and_xxx_bit() routines require a memory fence before we
 * start the operation, and after the operation completes.  We use
 * smp_mb() before, and rely on the "!= 0" comparison, plus a compiler
 * barrier(), to block until the atomic op is complete.
 */

static inline int test_and_set_bit(unsigned nr, volatile unsigned long *addr)
{
	int val;
	unsigned long mask = (1UL << (nr % BITS_PER_LONG));
	smp_mb();  /* barrier for proper semantics */
	val = (__insn_fetchor((void *)(addr + nr / BITS_PER_LONG), mask)
	       & mask) != 0;
	barrier();
	return val;
}


static inline int test_and_clear_bit(unsigned nr, volatile unsigned long *addr)
{
	int val;
	unsigned long mask = (1UL << (nr % BITS_PER_LONG));
	smp_mb();  /* barrier for proper semantics */
	val = (__insn_fetchand((void *)(addr + nr / BITS_PER_LONG), ~mask)
	       & mask) != 0;
	barrier();
	return val;
}


static inline int test_and_change_bit(unsigned nr,
				      volatile unsigned long *addr)
{
	unsigned long mask = (1UL << (nr % BITS_PER_LONG));
<<<<<<< HEAD
<<<<<<< HEAD
	long guess, oldval = *addr;
=======
	unsigned long guess, oldval;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	unsigned long guess, oldval;
>>>>>>> refs/remotes/origin/master
	addr += nr / BITS_PER_LONG;
	oldval = *addr;
	do {
		guess = oldval;
<<<<<<< HEAD
		oldval = atomic64_cmpxchg((atomic64_t *)addr,
					  guess, guess ^ mask);
=======
		oldval = cmpxchg(addr, guess, guess ^ mask);
>>>>>>> refs/remotes/origin/master
	} while (guess != oldval);
	return (oldval & mask) != 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
#define ext2_set_bit_atomic(lock, nr, addr)			\
	test_and_set_bit((nr), (unsigned long *)(addr))
#define ext2_clear_bit_atomic(lock, nr, addr)			\
	test_and_clear_bit((nr), (unsigned long *)(addr))
=======
#include <asm-generic/bitops/ext2-atomic-setbit.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <asm-generic/bitops/ext2-atomic-setbit.h>
>>>>>>> refs/remotes/origin/master

#endif /* _ASM_TILE_BITOPS_64_H */
