#ifndef _LINUX_IRQ_WORK_H
#define _LINUX_IRQ_WORK_H

<<<<<<< HEAD
struct irq_work {
	struct irq_work *next;
=======
#include <linux/llist.h>

struct irq_work {
	unsigned long flags;
	struct llist_node llnode;
>>>>>>> refs/remotes/origin/cm-10.0
	void (*func)(struct irq_work *);
};

static inline
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
void init_irq_work(struct irq_work *work, void (*func)(struct irq_work *))
{
	work->flags = 0;
	work->func = func;
}

bool irq_work_queue(struct irq_work *work);
void irq_work_run(void);
void irq_work_sync(struct irq_work *work);
>>>>>>> refs/remotes/origin/cm-10.0

#endif /* _LINUX_IRQ_WORK_H */
