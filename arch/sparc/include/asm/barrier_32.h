#ifndef __SPARC_BARRIER_H
#define __SPARC_BARRIER_H

<<<<<<< HEAD
/* XXX Change this if we ever use a PSO mode kernel. */
#define mb()	__asm__ __volatile__ ("" : : : "memory")
#define rmb()	mb()
#define wmb()	mb()
#define read_barrier_depends()	do { } while(0)
#define set_mb(__var, __value)  do { __var = __value; mb(); } while(0)
#define smp_mb()	__asm__ __volatile__("":::"memory")
#define smp_rmb()	__asm__ __volatile__("":::"memory")
#define smp_wmb()	__asm__ __volatile__("":::"memory")
#define smp_read_barrier_depends()	do { } while(0)
=======
#include <asm/processor.h> /* for nop() */
#include <asm-generic/barrier.h>
>>>>>>> refs/remotes/origin/master

#endif /* !(__SPARC_BARRIER_H) */
