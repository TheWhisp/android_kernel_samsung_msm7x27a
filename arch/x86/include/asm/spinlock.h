#ifndef _ASM_X86_SPINLOCK_H
#define _ASM_X86_SPINLOCK_H

<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/atomic.h>
#include <asm/rwlock.h>
=======
#include <linux/atomic.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/jump_label.h>
#include <linux/atomic.h>
>>>>>>> refs/remotes/origin/master
#include <asm/page.h>
#include <asm/processor.h>
#include <linux/compiler.h>
#include <asm/paravirt.h>
<<<<<<< HEAD
=======
#include <asm/bitops.h>

>>>>>>> refs/remotes/origin/master
/*
 * Your basic SMP spinlocks, allowing only a single CPU anywhere
 *
 * Simple spin lock operations.  There are two variants, one clears IRQ's
 * on the local processor, one does not.
 *
<<<<<<< HEAD
 * These are fair FIFO ticket locks, which are currently limited to 256
 * CPUs.
=======
 * These are fair FIFO ticket locks, which support up to 2^16 CPUs.
>>>>>>> refs/remotes/origin/master
 *
 * (the type definitions are in asm/spinlock_types.h)
 */

#ifdef CONFIG_X86_32
# define LOCK_PTR_REG "a"
<<<<<<< HEAD
# define REG_PTR_MODE "k"
#else
# define LOCK_PTR_REG "D"
# define REG_PTR_MODE "q"
=======
#else
# define LOCK_PTR_REG "D"
>>>>>>> refs/remotes/origin/master
#endif

#if defined(CONFIG_X86_32) && \
	(defined(CONFIG_X86_OOSTORE) || defined(CONFIG_X86_PPRO_FENCE))
/*
 * On PPro SMP or if we are using OOSTORE, we use a locked operation to unlock
 * (PPro errata 66, 92)
 */
# define UNLOCK_LOCK_PREFIX LOCK_PREFIX
#else
# define UNLOCK_LOCK_PREFIX
#endif

<<<<<<< HEAD
=======
/* How long a lock should spin before we consider blocking */
#define SPIN_THRESHOLD	(1 << 15)

extern struct static_key paravirt_ticketlocks_enabled;
static __always_inline bool static_key_false(struct static_key *key);

#ifdef CONFIG_PARAVIRT_SPINLOCKS

static inline void __ticket_enter_slowpath(arch_spinlock_t *lock)
{
	set_bit(0, (volatile unsigned long *)&lock->tickets.tail);
}

#else  /* !CONFIG_PARAVIRT_SPINLOCKS */
static __always_inline void __ticket_lock_spinning(arch_spinlock_t *lock,
							__ticket_t ticket)
{
}
static inline void __ticket_unlock_kick(arch_spinlock_t *lock,
							__ticket_t ticket)
{
}

#endif /* CONFIG_PARAVIRT_SPINLOCKS */

static __always_inline int arch_spin_value_unlocked(arch_spinlock_t lock)
{
	return lock.tickets.head == lock.tickets.tail;
}

>>>>>>> refs/remotes/origin/master
/*
 * Ticket locks are conceptually two parts, one indicating the current head of
 * the queue, and the other indicating the current tail. The lock is acquired
 * by atomically noting the tail and incrementing it by one (thus adding
 * ourself to the queue and noting our position), then waiting until the head
 * becomes equal to the the initial value of the tail.
 *
 * We use an xadd covering *both* parts of the lock, to increment the tail and
 * also load the position of the head, which takes care of memory ordering
 * issues and should be optimal for the uncontended case. Note the tail must be
 * in the high part, because a wide xadd increment of the low part would carry
 * up and contaminate the high part.
<<<<<<< HEAD
<<<<<<< HEAD
 *
 * With fewer than 2^8 possible CPUs, we can use x86's partial registers to
 * save some instructions and make the code more elegant. There really isn't
 * much between them in performance though, especially as locks are out of line.
 */
#if (NR_CPUS < 256)
#define TICKET_SHIFT 8

static __always_inline void __ticket_spin_lock(arch_spinlock_t *lock)
{
	short inc = 0x0100;

	asm volatile (
		LOCK_PREFIX "xaddw %w0, %1\n"
		"1:\t"
		"cmpb %h0, %b0\n\t"
		"je 2f\n\t"
		"rep ; nop\n\t"
		"movb %1, %b0\n\t"
		/* don't need lfence here, because loads are in-order */
		"jmp 1b\n"
		"2:"
		: "+Q" (inc), "+m" (lock->slock)
		:
		: "memory", "cc");
=======
 */
static __always_inline void __ticket_spin_lock(arch_spinlock_t *lock)
{
	register struct __raw_tickets inc = { .tail = 1 };

	inc = xadd(&lock->tickets, inc);

	for (;;) {
		if (inc.head == inc.tail)
			break;
		cpu_relax();
		inc.head = ACCESS_ONCE(lock->tickets.head);
	}
	barrier();		/* make sure nothing creeps before the lock is taken */
>>>>>>> refs/remotes/origin/cm-10.0
}

static __always_inline int __ticket_spin_trylock(arch_spinlock_t *lock)
{
<<<<<<< HEAD
	int tmp, new;

	asm volatile("movzwl %2, %0\n\t"
		     "cmpb %h0,%b0\n\t"
		     "leal 0x100(%" REG_PTR_MODE "0), %1\n\t"
		     "jne 1f\n\t"
		     LOCK_PREFIX "cmpxchgw %w1,%2\n\t"
		     "1:"
		     "sete %b1\n\t"
		     "movzbl %b1,%0\n\t"
		     : "=&a" (tmp), "=&q" (new), "+m" (lock->slock)
		     :
		     : "memory", "cc");

	return tmp;
}

static __always_inline void __ticket_spin_unlock(arch_spinlock_t *lock)
{
	asm volatile(UNLOCK_LOCK_PREFIX "incb %0"
		     : "+m" (lock->slock)
		     :
		     : "memory", "cc");
}
#else
#define TICKET_SHIFT 16

static __always_inline void __ticket_spin_lock(arch_spinlock_t *lock)
{
	int inc = 0x00010000;
	int tmp;

	asm volatile(LOCK_PREFIX "xaddl %0, %1\n"
		     "movzwl %w0, %2\n\t"
		     "shrl $16, %0\n\t"
		     "1:\t"
		     "cmpl %0, %2\n\t"
		     "je 2f\n\t"
		     "rep ; nop\n\t"
		     "movzwl %1, %2\n\t"
		     /* don't need lfence here, because loads are in-order */
		     "jmp 1b\n"
		     "2:"
		     : "+r" (inc), "+m" (lock->slock), "=&r" (tmp)
		     :
		     : "memory", "cc");
}

static __always_inline int __ticket_spin_trylock(arch_spinlock_t *lock)
{
	int tmp;
	int new;

	asm volatile("movl %2,%0\n\t"
		     "movl %0,%1\n\t"
		     "roll $16, %0\n\t"
		     "cmpl %0,%1\n\t"
		     "leal 0x00010000(%" REG_PTR_MODE "0), %1\n\t"
		     "jne 1f\n\t"
		     LOCK_PREFIX "cmpxchgl %1,%2\n\t"
		     "1:"
		     "sete %b1\n\t"
		     "movzbl %b1,%0\n\t"
		     : "=&a" (tmp), "=&q" (new), "+m" (lock->slock)
		     :
		     : "memory", "cc");

	return tmp;
=======
	arch_spinlock_t old, new;

	old.tickets = ACCESS_ONCE(lock->tickets);
	if (old.tickets.head != old.tickets.tail)
		return 0;

	new.head_tail = old.head_tail + (1 << TICKET_SHIFT);

	/* cmpxchg is a full barrier, so nothing can move before it */
	return cmpxchg(&lock->head_tail, old.head_tail, new.head_tail) == old.head_tail;
>>>>>>> refs/remotes/origin/cm-10.0
}

static __always_inline void __ticket_spin_unlock(arch_spinlock_t *lock)
{
<<<<<<< HEAD
	asm volatile(UNLOCK_LOCK_PREFIX "incw %0"
		     : "+m" (lock->slock)
		     :
		     : "memory", "cc");
}
#endif

static inline int __ticket_spin_is_locked(arch_spinlock_t *lock)
{
	int tmp = ACCESS_ONCE(lock->slock);

	return !!(((tmp >> TICKET_SHIFT) ^ tmp) & ((1 << TICKET_SHIFT) - 1));
=======
	__add(&lock->tickets.head, 1, UNLOCK_LOCK_PREFIX);
}

static inline int __ticket_spin_is_locked(arch_spinlock_t *lock)
{
	struct __raw_tickets tmp = ACCESS_ONCE(lock->tickets);

	return tmp.tail != tmp.head;
>>>>>>> refs/remotes/origin/cm-10.0
}

static inline int __ticket_spin_is_contended(arch_spinlock_t *lock)
{
<<<<<<< HEAD
	int tmp = ACCESS_ONCE(lock->slock);

	return (((tmp >> TICKET_SHIFT) - tmp) & ((1 << TICKET_SHIFT) - 1)) > 1;
=======
	struct __raw_tickets tmp = ACCESS_ONCE(lock->tickets);

	return (__ticket_t)(tmp.tail - tmp.head) > 1;
>>>>>>> refs/remotes/origin/cm-10.0
}

#ifndef CONFIG_PARAVIRT_SPINLOCKS

static inline int arch_spin_is_locked(arch_spinlock_t *lock)
{
	return __ticket_spin_is_locked(lock);
=======
 */
static __always_inline void arch_spin_lock(arch_spinlock_t *lock)
{
	register struct __raw_tickets inc = { .tail = TICKET_LOCK_INC };

	inc = xadd(&lock->tickets, inc);
	if (likely(inc.head == inc.tail))
		goto out;

	inc.tail &= ~TICKET_SLOWPATH_FLAG;
	for (;;) {
		unsigned count = SPIN_THRESHOLD;

		do {
			if (ACCESS_ONCE(lock->tickets.head) == inc.tail)
				goto out;
			cpu_relax();
		} while (--count);
		__ticket_lock_spinning(lock, inc.tail);
	}
out:	barrier();	/* make sure nothing creeps before the lock is taken */
}

static __always_inline int arch_spin_trylock(arch_spinlock_t *lock)
{
	arch_spinlock_t old, new;

	old.tickets = ACCESS_ONCE(lock->tickets);
	if (old.tickets.head != (old.tickets.tail & ~TICKET_SLOWPATH_FLAG))
		return 0;

	new.head_tail = old.head_tail + (TICKET_LOCK_INC << TICKET_SHIFT);

	/* cmpxchg is a full barrier, so nothing can move before it */
	return cmpxchg(&lock->head_tail, old.head_tail, new.head_tail) == old.head_tail;
}

static inline void __ticket_unlock_slowpath(arch_spinlock_t *lock,
					    arch_spinlock_t old)
{
	arch_spinlock_t new;

	BUILD_BUG_ON(((__ticket_t)NR_CPUS) != NR_CPUS);

	/* Perform the unlock on the "before" copy */
	old.tickets.head += TICKET_LOCK_INC;

	/* Clear the slowpath flag */
	new.head_tail = old.head_tail & ~(TICKET_SLOWPATH_FLAG << TICKET_SHIFT);

	/*
	 * If the lock is uncontended, clear the flag - use cmpxchg in
	 * case it changes behind our back though.
	 */
	if (new.tickets.head != new.tickets.tail ||
	    cmpxchg(&lock->head_tail, old.head_tail,
					new.head_tail) != old.head_tail) {
		/*
		 * Lock still has someone queued for it, so wake up an
		 * appropriate waiter.
		 */
		__ticket_unlock_kick(lock, old.tickets.head);
	}
}

static __always_inline void arch_spin_unlock(arch_spinlock_t *lock)
{
	if (TICKET_SLOWPATH_FLAG &&
	    static_key_false(&paravirt_ticketlocks_enabled)) {
		arch_spinlock_t prev;

		prev = *lock;
		add_smp(&lock->tickets.head, TICKET_LOCK_INC);

		/* add_smp() is a full mb() */

		if (unlikely(lock->tickets.tail & TICKET_SLOWPATH_FLAG))
			__ticket_unlock_slowpath(lock, prev);
	} else
		__add(&lock->tickets.head, TICKET_LOCK_INC, UNLOCK_LOCK_PREFIX);
}

static inline int arch_spin_is_locked(arch_spinlock_t *lock)
{
	struct __raw_tickets tmp = ACCESS_ONCE(lock->tickets);

	return tmp.tail != tmp.head;
>>>>>>> refs/remotes/origin/master
}

static inline int arch_spin_is_contended(arch_spinlock_t *lock)
{
<<<<<<< HEAD
	return __ticket_spin_is_contended(lock);
}
#define arch_spin_is_contended	arch_spin_is_contended

static __always_inline void arch_spin_lock(arch_spinlock_t *lock)
{
	__ticket_spin_lock(lock);
}

static __always_inline int arch_spin_trylock(arch_spinlock_t *lock)
{
	return __ticket_spin_trylock(lock);
}

static __always_inline void arch_spin_unlock(arch_spinlock_t *lock)
{
	__ticket_spin_unlock(lock);
}
=======
	struct __raw_tickets tmp = ACCESS_ONCE(lock->tickets);

	return (__ticket_t)(tmp.tail - tmp.head) > TICKET_LOCK_INC;
}
#define arch_spin_is_contended	arch_spin_is_contended
>>>>>>> refs/remotes/origin/master

static __always_inline void arch_spin_lock_flags(arch_spinlock_t *lock,
						  unsigned long flags)
{
	arch_spin_lock(lock);
}

<<<<<<< HEAD
#endif	/* CONFIG_PARAVIRT_SPINLOCKS */

=======
>>>>>>> refs/remotes/origin/master
static inline void arch_spin_unlock_wait(arch_spinlock_t *lock)
{
	while (arch_spin_is_locked(lock))
		cpu_relax();
}

/*
 * Read-write spinlocks, allowing multiple readers
 * but only one writer.
 *
 * NOTE! it is quite common to have readers in interrupts
 * but no interrupt writers. For those circumstances we
 * can "mix" irq-safe locks - any writer needs to get a
 * irq-safe write-lock, but readers can get non-irqsafe
 * read-locks.
 *
 * On x86, we implement read-write locks as a 32-bit counter
 * with the high bit (sign) being the "contended" bit.
 */

/**
 * read_can_lock - would read_trylock() succeed?
 * @lock: the rwlock in question.
 */
static inline int arch_read_can_lock(arch_rwlock_t *lock)
{
<<<<<<< HEAD
<<<<<<< HEAD
	return (int)(lock)->lock > 0;
=======
	return lock->lock > 0;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return lock->lock > 0;
>>>>>>> refs/remotes/origin/master
}

/**
 * write_can_lock - would write_trylock() succeed?
 * @lock: the rwlock in question.
 */
static inline int arch_write_can_lock(arch_rwlock_t *lock)
{
<<<<<<< HEAD
<<<<<<< HEAD
	return (lock)->lock == RW_LOCK_BIAS;
=======
	return lock->write == WRITE_LOCK_CMP;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return lock->write == WRITE_LOCK_CMP;
>>>>>>> refs/remotes/origin/master
}

static inline void arch_read_lock(arch_rwlock_t *rw)
{
<<<<<<< HEAD
<<<<<<< HEAD
	asm volatile(LOCK_PREFIX " subl $1,(%0)\n\t"
=======
	asm volatile(LOCK_PREFIX READ_LOCK_SIZE(dec) " (%0)\n\t"
>>>>>>> refs/remotes/origin/cm-10.0
=======
	asm volatile(LOCK_PREFIX READ_LOCK_SIZE(dec) " (%0)\n\t"
>>>>>>> refs/remotes/origin/master
		     "jns 1f\n"
		     "call __read_lock_failed\n\t"
		     "1:\n"
		     ::LOCK_PTR_REG (rw) : "memory");
}

static inline void arch_write_lock(arch_rwlock_t *rw)
{
<<<<<<< HEAD
<<<<<<< HEAD
	asm volatile(LOCK_PREFIX " subl %1,(%0)\n\t"
		     "jz 1f\n"
		     "call __write_lock_failed\n\t"
		     "1:\n"
		     ::LOCK_PTR_REG (rw), "i" (RW_LOCK_BIAS) : "memory");
=======
=======
>>>>>>> refs/remotes/origin/master
	asm volatile(LOCK_PREFIX WRITE_LOCK_SUB(%1) "(%0)\n\t"
		     "jz 1f\n"
		     "call __write_lock_failed\n\t"
		     "1:\n"
		     ::LOCK_PTR_REG (&rw->write), "i" (RW_LOCK_BIAS)
		     : "memory");
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static inline int arch_read_trylock(arch_rwlock_t *lock)
{
<<<<<<< HEAD
<<<<<<< HEAD
	atomic_t *count = (atomic_t *)lock;

	if (atomic_dec_return(count) >= 0)
		return 1;
	atomic_inc(count);
=======
=======
>>>>>>> refs/remotes/origin/master
	READ_LOCK_ATOMIC(t) *count = (READ_LOCK_ATOMIC(t) *)lock;

	if (READ_LOCK_ATOMIC(dec_return)(count) >= 0)
		return 1;
	READ_LOCK_ATOMIC(inc)(count);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return 0;
}

static inline int arch_write_trylock(arch_rwlock_t *lock)
{
<<<<<<< HEAD
<<<<<<< HEAD
	atomic_t *count = (atomic_t *)lock;

	if (atomic_sub_and_test(RW_LOCK_BIAS, count))
		return 1;
	atomic_add(RW_LOCK_BIAS, count);
=======
=======
>>>>>>> refs/remotes/origin/master
	atomic_t *count = (atomic_t *)&lock->write;

	if (atomic_sub_and_test(WRITE_LOCK_CMP, count))
		return 1;
	atomic_add(WRITE_LOCK_CMP, count);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return 0;
}

static inline void arch_read_unlock(arch_rwlock_t *rw)
{
<<<<<<< HEAD
<<<<<<< HEAD
	asm volatile(LOCK_PREFIX "incl %0" :"+m" (rw->lock) : : "memory");
=======
	asm volatile(LOCK_PREFIX READ_LOCK_SIZE(inc) " %0"
		     :"+m" (rw->lock) : : "memory");
>>>>>>> refs/remotes/origin/cm-10.0
=======
	asm volatile(LOCK_PREFIX READ_LOCK_SIZE(inc) " %0"
		     :"+m" (rw->lock) : : "memory");
>>>>>>> refs/remotes/origin/master
}

static inline void arch_write_unlock(arch_rwlock_t *rw)
{
<<<<<<< HEAD
<<<<<<< HEAD
	asm volatile(LOCK_PREFIX "addl %1, %0"
		     : "+m" (rw->lock) : "i" (RW_LOCK_BIAS) : "memory");
=======
	asm volatile(LOCK_PREFIX WRITE_LOCK_ADD(%1) "%0"
		     : "+m" (rw->write) : "i" (RW_LOCK_BIAS) : "memory");
>>>>>>> refs/remotes/origin/cm-10.0
=======
	asm volatile(LOCK_PREFIX WRITE_LOCK_ADD(%1) "%0"
		     : "+m" (rw->write) : "i" (RW_LOCK_BIAS) : "memory");
>>>>>>> refs/remotes/origin/master
}

#define arch_read_lock_flags(lock, flags) arch_read_lock(lock)
#define arch_write_lock_flags(lock, flags) arch_write_lock(lock)

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
#undef READ_LOCK_SIZE
#undef READ_LOCK_ATOMIC
#undef WRITE_LOCK_ADD
#undef WRITE_LOCK_SUB
#undef WRITE_LOCK_CMP

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#define arch_spin_relax(lock)	cpu_relax()
#define arch_read_relax(lock)	cpu_relax()
#define arch_write_relax(lock)	cpu_relax()

<<<<<<< HEAD
/* The {read|write|spin}_lock() on x86 are full memory barriers. */
static inline void smp_mb__after_lock(void) { }
#define ARCH_HAS_SMP_MB_AFTER_LOCK

=======
>>>>>>> refs/remotes/origin/master
#endif /* _ASM_X86_SPINLOCK_H */
