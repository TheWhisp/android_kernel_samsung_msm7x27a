#ifndef _LINUX_TRACE_CLOCK_H
#define _LINUX_TRACE_CLOCK_H

/*
 * 3 trace clock variants, with differing scalability/precision
 * tradeoffs:
 *
 *  -   local: CPU-local trace clock
 *  -  medium: scalable global clock with some jitter
 *  -  global: globally monotonic, serialized clock
 */
#include <linux/compiler.h>
#include <linux/types.h>

<<<<<<< HEAD
extern u64 notrace trace_clock_local(void);
extern u64 notrace trace_clock(void);
extern u64 notrace trace_clock_global(void);
<<<<<<< HEAD
=======
extern u64 notrace trace_clock_counter(void);
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <asm/trace_clock.h>

extern u64 notrace trace_clock_local(void);
extern u64 notrace trace_clock(void);
extern u64 notrace trace_clock_jiffies(void);
extern u64 notrace trace_clock_global(void);
extern u64 notrace trace_clock_counter(void);
>>>>>>> refs/remotes/origin/master

#endif /* _LINUX_TRACE_CLOCK_H */
