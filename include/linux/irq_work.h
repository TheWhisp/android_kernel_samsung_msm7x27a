#ifndef _LINUX_IRQ_WORK_H
#define _LINUX_IRQ_WORK_H

<<<<<<< HEAD
<<<<<<< HEAD
struct irq_work {
	struct irq_work *next;
=======
#include <linux/llist.h>

struct irq_work {
	unsigned long flags;
	struct llist_node llnode;
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/llist.h>

/*
 * An entry can be in one of four states:
 *
 * free	     NULL, 0 -> {claimed}       : free to be used
 * claimed   NULL, 3 -> {pending}       : claimed to be enqueued
 * pending   next, 3 -> {busy}          : queued, pending callback
 * busy      NULL, 2 -> {free, claimed} : callback in progress, can be claimed
 */

#define IRQ_WORK_PENDING	1UL
#define IRQ_WORK_BUSY		2UL
#define IRQ_WORK_FLAGS		3UL
#define IRQ_WORK_LAZY		4UL /* Doesn't want IPI, wait for tick */

struct irq_work {
	unsigned long flags;
	struct llist_node llnode;
>>>>>>> refs/remotes/origin/master
	void (*func)(struct irq_work *);
};

static inline
<<<<<<< HEAD
<<<<<<< HEAD
void init_irq_work(struct irq_work *entry, void (*func)(struct irq_work *))
{
	entry->next = NULL;
	entry->func = func;
}

bool irq_work_queue(struct irq_work *entry);
void irq_work_run(void);
void irq_work_sync(struct irq_work *entry);
=======
=======
>>>>>>> refs/remotes/origin/master
void init_irq_work(struct irq_work *work, void (*func)(struct irq_work *))
{
	work->flags = 0;
	work->func = func;
}

<<<<<<< HEAD
bool irq_work_queue(struct irq_work *work);
void irq_work_run(void);
void irq_work_sync(struct irq_work *work);
>>>>>>> refs/remotes/origin/cm-10.0
=======
void irq_work_queue(struct irq_work *work);
void irq_work_run(void);
void irq_work_sync(struct irq_work *work);

#ifdef CONFIG_IRQ_WORK
bool irq_work_needs_cpu(void);
#else
static inline bool irq_work_needs_cpu(void) { return false; }
#endif
>>>>>>> refs/remotes/origin/master

#endif /* _LINUX_IRQ_WORK_H */
