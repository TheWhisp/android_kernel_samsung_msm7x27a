/*
 * Copyright (C) 2010 Red Hat, Inc., Peter Zijlstra <pzijlstr@redhat.com>
 *
 * Provides a framework for enqueueing and running callbacks from hardirq
 * context. The enqueueing is NMI-safe.
 */

<<<<<<< HEAD
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/irq_work.h>
#include <linux/hardirq.h>
=======
#include <linux/bug.h>
#include <linux/kernel.h>
#include <linux/export.h>
#include <linux/irq_work.h>
#include <linux/percpu.h>
#include <linux/hardirq.h>
#include <linux/irqflags.h>
#include <asm/processor.h>
>>>>>>> refs/remotes/origin/cm-10.0

/*
 * An entry can be in one of four states:
 *
 * free	     NULL, 0 -> {claimed}       : free to be used
 * claimed   NULL, 3 -> {pending}       : claimed to be enqueued
 * pending   next, 3 -> {busy}          : queued, pending callback
 * busy      NULL, 2 -> {free, claimed} : callback in progress, can be claimed
<<<<<<< HEAD
 *
 * We use the lower two bits of the next pointer to keep PENDING and BUSY
 * flags.
=======
>>>>>>> refs/remotes/origin/cm-10.0
 */

#define IRQ_WORK_PENDING	1UL
#define IRQ_WORK_BUSY		2UL
#define IRQ_WORK_FLAGS		3UL

<<<<<<< HEAD
static inline bool irq_work_is_set(struct irq_work *entry, int flags)
{
	return (unsigned long)entry->next & flags;
}

static inline struct irq_work *irq_work_next(struct irq_work *entry)
{
	unsigned long next = (unsigned long)entry->next;
	next &= ~IRQ_WORK_FLAGS;
	return (struct irq_work *)next;
}

static inline struct irq_work *next_flags(struct irq_work *entry, int flags)
{
	unsigned long next = (unsigned long)entry;
	next |= flags;
	return (struct irq_work *)next;
}

static DEFINE_PER_CPU(struct irq_work *, irq_work_list);
=======
static DEFINE_PER_CPU(struct llist_head, irq_work_list);
>>>>>>> refs/remotes/origin/cm-10.0

/*
 * Claim the entry so that no one else will poke at it.
 */
<<<<<<< HEAD
static bool irq_work_claim(struct irq_work *entry)
{
	struct irq_work *next, *nflags;

	do {
		next = entry->next;
		if ((unsigned long)next & IRQ_WORK_PENDING)
			return false;
		nflags = next_flags(next, IRQ_WORK_FLAGS);
	} while (cmpxchg(&entry->next, next, nflags) != next);
=======
static bool irq_work_claim(struct irq_work *work)
{
	unsigned long flags, nflags;

	for (;;) {
		flags = work->flags;
		if (flags & IRQ_WORK_PENDING)
			return false;
		nflags = flags | IRQ_WORK_FLAGS;
		if (cmpxchg(&work->flags, flags, nflags) == flags)
			break;
		cpu_relax();
	}
>>>>>>> refs/remotes/origin/cm-10.0

	return true;
}

<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/cm-10.0
void __weak arch_irq_work_raise(void)
{
	/*
	 * Lame architectures will get the timer tick callback
	 */
}

/*
 * Queue the entry and raise the IPI if needed.
 */
<<<<<<< HEAD
static void __irq_work_queue(struct irq_work *entry)
{
	struct irq_work *next;

	preempt_disable();

	do {
		next = __this_cpu_read(irq_work_list);
		/* Can assign non-atomic because we keep the flags set. */
		entry->next = next_flags(next, IRQ_WORK_FLAGS);
	} while (this_cpu_cmpxchg(irq_work_list, next, entry) != next);

	/* The list was empty, raise self-interrupt to start processing. */
	if (!irq_work_next(entry))
=======
static void __irq_work_queue(struct irq_work *work)
{
	bool empty;

	preempt_disable();

	empty = llist_add(&work->llnode, &__get_cpu_var(irq_work_list));
	/* The list was empty, raise self-interrupt to start processing. */
	if (empty)
>>>>>>> refs/remotes/origin/cm-10.0
		arch_irq_work_raise();

	preempt_enable();
}

/*
 * Enqueue the irq_work @entry, returns true on success, failure when the
 * @entry was already enqueued by someone else.
 *
 * Can be re-enqueued while the callback is still in progress.
 */
<<<<<<< HEAD
bool irq_work_queue(struct irq_work *entry)
{
	if (!irq_work_claim(entry)) {
=======
bool irq_work_queue(struct irq_work *work)
{
	if (!irq_work_claim(work)) {
>>>>>>> refs/remotes/origin/cm-10.0
		/*
		 * Already enqueued, can't do!
		 */
		return false;
	}

<<<<<<< HEAD
	__irq_work_queue(entry);
=======
	__irq_work_queue(work);
>>>>>>> refs/remotes/origin/cm-10.0
	return true;
}
EXPORT_SYMBOL_GPL(irq_work_queue);

/*
 * Run the irq_work entries on this cpu. Requires to be ran from hardirq
 * context with local IRQs disabled.
 */
void irq_work_run(void)
{
<<<<<<< HEAD
	struct irq_work *list;

	if (this_cpu_read(irq_work_list) == NULL)
=======
	struct irq_work *work;
	struct llist_head *this_list;
	struct llist_node *llnode;

	this_list = &__get_cpu_var(irq_work_list);
	if (llist_empty(this_list))
>>>>>>> refs/remotes/origin/cm-10.0
		return;

	BUG_ON(!in_irq());
	BUG_ON(!irqs_disabled());

<<<<<<< HEAD
	list = this_cpu_xchg(irq_work_list, NULL);

	while (list != NULL) {
		struct irq_work *entry = list;

		list = irq_work_next(list);

		/*
		 * Clear the PENDING bit, after this point the @entry
		 * can be re-used.
		 */
		entry->next = next_flags(NULL, IRQ_WORK_BUSY);
		entry->func(entry);
=======
	llnode = llist_del_all(this_list);
	while (llnode != NULL) {
		work = llist_entry(llnode, struct irq_work, llnode);

		llnode = llist_next(llnode);

		/*
		 * Clear the PENDING bit, after this point the @work
		 * can be re-used.
		 */
		work->flags = IRQ_WORK_BUSY;
		work->func(work);
>>>>>>> refs/remotes/origin/cm-10.0
		/*
		 * Clear the BUSY bit and return to the free state if
		 * no-one else claimed it meanwhile.
		 */
<<<<<<< HEAD
		(void)cmpxchg(&entry->next,
			      next_flags(NULL, IRQ_WORK_BUSY),
			      NULL);
=======
		(void)cmpxchg(&work->flags, IRQ_WORK_BUSY, 0);
>>>>>>> refs/remotes/origin/cm-10.0
	}
}
EXPORT_SYMBOL_GPL(irq_work_run);

/*
 * Synchronize against the irq_work @entry, ensures the entry is not
 * currently in use.
 */
<<<<<<< HEAD
void irq_work_sync(struct irq_work *entry)
{
	WARN_ON_ONCE(irqs_disabled());

	while (irq_work_is_set(entry, IRQ_WORK_BUSY))
=======
void irq_work_sync(struct irq_work *work)
{
	WARN_ON_ONCE(irqs_disabled());

	while (work->flags & IRQ_WORK_BUSY)
>>>>>>> refs/remotes/origin/cm-10.0
		cpu_relax();
}
EXPORT_SYMBOL_GPL(irq_work_sync);
