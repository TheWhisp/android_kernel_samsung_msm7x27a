/*
 * async.c: Asynchronous function calls for boot performance
 *
 * (C) Copyright 2009 Intel Corporation
 * Author: Arjan van de Ven <arjan@linux.intel.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 */


/*

Goals and Theory of Operation

The primary goal of this feature is to reduce the kernel boot time,
by doing various independent hardware delays and discovery operations
decoupled and not strictly serialized.

More specifically, the asynchronous function call concept allows
certain operations (primarily during system boot) to happen
asynchronously, out of order, while these operations still
have their externally visible parts happen sequentially and in-order.
(not unlike how out-of-order CPUs retire their instructions in order)

Key to the asynchronous function call implementation is the concept of
a "sequence cookie" (which, although it has an abstracted type, can be
thought of as a monotonically incrementing number).

The async core will assign each scheduled event such a sequence cookie and
pass this to the called functions.

The asynchronously called function should before doing a globally visible
operation, such as registering device numbers, call the
async_synchronize_cookie() function and pass in its own cookie. The
async_synchronize_cookie() function will make sure that all asynchronous
operations that were scheduled prior to the operation corresponding with the
cookie have completed.

Subsystem/driver initialization code that scheduled asynchronous probe
functions, but which shares global resources with other drivers/subsystems
that do not use the asynchronous call feature, need to do a full
synchronization with the async_synchronize_full() function, before returning
from their init function. This is to maintain strict ordering between the
asynchronous and synchronous parts of the kernel.

*/

#include <linux/async.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/module.h>
=======
#include <linux/atomic.h>
#include <linux/ktime.h>
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/atomic.h>
#include <linux/ktime.h>
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/atomic.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0

static async_cookie_t next_cookie = 1;

#define MAX_WORK	32768

static LIST_HEAD(async_pending);
static LIST_HEAD(async_running);
static DEFINE_SPINLOCK(async_lock);

struct async_entry {
	struct list_head	list;
	struct work_struct	work;
	async_cookie_t		cookie;
	async_func_ptr		*func;
	void			*data;
	struct list_head	*running;
=======

#include "workqueue_internal.h"

static async_cookie_t next_cookie = 1;

#define MAX_WORK		32768
#define ASYNC_COOKIE_MAX	ULLONG_MAX	/* infinity cookie */

static LIST_HEAD(async_global_pending);	/* pending from all registered doms */
static ASYNC_DOMAIN(async_dfl_domain);
static DEFINE_SPINLOCK(async_lock);

struct async_entry {
	struct list_head	domain_list;
	struct list_head	global_list;
	struct work_struct	work;
	async_cookie_t		cookie;
	async_func_t		func;
	void			*data;
	struct async_domain	*domain;
>>>>>>> refs/remotes/origin/master
};

static DECLARE_WAIT_QUEUE_HEAD(async_done);

static atomic_t entry_count;

<<<<<<< HEAD
<<<<<<< HEAD
extern int initcall_debug;

=======
>>>>>>> refs/remotes/origin/cm-10.0

/*
 * MUST be called with the lock held!
 */
static async_cookie_t  __lowest_in_progress(struct list_head *running)
{
	struct async_entry *entry;

	if (!running) { /* just check the entry count */
		if (atomic_read(&entry_count))
			return 0; /* smaller than any cookie */
		else
			return next_cookie;
	}

	if (!list_empty(running)) {
		entry = list_first_entry(running,
			struct async_entry, list);
		return entry->cookie;
	}

	list_for_each_entry(entry, &async_pending, list)
		if (entry->running == running)
			return entry->cookie;

	return next_cookie;	/* "infinity" value */
}

static async_cookie_t  lowest_in_progress(struct list_head *running)
{
	unsigned long flags;
	async_cookie_t ret;

	spin_lock_irqsave(&async_lock, flags);
	ret = __lowest_in_progress(running);
=======
static async_cookie_t lowest_in_progress(struct async_domain *domain)
{
	struct list_head *pending;
	async_cookie_t ret = ASYNC_COOKIE_MAX;
	unsigned long flags;

	spin_lock_irqsave(&async_lock, flags);

	if (domain)
		pending = &domain->pending;
	else
		pending = &async_global_pending;

	if (!list_empty(pending))
		ret = list_first_entry(pending, struct async_entry,
				       domain_list)->cookie;

>>>>>>> refs/remotes/origin/master
	spin_unlock_irqrestore(&async_lock, flags);
	return ret;
}

/*
 * pick the first pending entry and run it
 */
static void async_run_entry_fn(struct work_struct *work)
{
	struct async_entry *entry =
		container_of(work, struct async_entry, work);
	unsigned long flags;
<<<<<<< HEAD
<<<<<<< HEAD
	ktime_t calltime, delta, rettime;
=======
	ktime_t uninitialized_var(calltime), delta, rettime;
>>>>>>> refs/remotes/origin/cm-10.0

	/* 1) move self to the running queue */
	spin_lock_irqsave(&async_lock, flags);
	list_move_tail(&entry->list, entry->running);
	spin_unlock_irqrestore(&async_lock, flags);

	/* 2) run (and print duration) */
	if (initcall_debug && system_state == SYSTEM_BOOTING) {
<<<<<<< HEAD
		printk("calling  %lli_%pF @ %i\n", (long long)entry->cookie,
=======
		printk(KERN_DEBUG "calling  %lli_%pF @ %i\n",
			(long long)entry->cookie,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ktime_t uninitialized_var(calltime), delta, rettime;

	/* 1) run (and print duration) */
	if (initcall_debug && system_state == SYSTEM_BOOTING) {
		printk(KERN_DEBUG "calling  %lli_%pF @ %i\n",
			(long long)entry->cookie,
>>>>>>> refs/remotes/origin/master
			entry->func, task_pid_nr(current));
		calltime = ktime_get();
	}
	entry->func(entry->data, entry->cookie);
	if (initcall_debug && system_state == SYSTEM_BOOTING) {
		rettime = ktime_get();
		delta = ktime_sub(rettime, calltime);
<<<<<<< HEAD
<<<<<<< HEAD
		printk("initcall %lli_%pF returned 0 after %lld usecs\n",
=======
		printk(KERN_DEBUG "initcall %lli_%pF returned 0 after %lld usecs\n",
>>>>>>> refs/remotes/origin/cm-10.0
=======
		printk(KERN_DEBUG "initcall %lli_%pF returned 0 after %lld usecs\n",
>>>>>>> refs/remotes/origin/master
			(long long)entry->cookie,
			entry->func,
			(long long)ktime_to_ns(delta) >> 10);
	}

<<<<<<< HEAD
	/* 3) remove self from the running queue */
	spin_lock_irqsave(&async_lock, flags);
	list_del(&entry->list);

	/* 4) free the entry */
=======
	/* 2) remove self from the pending queues */
	spin_lock_irqsave(&async_lock, flags);
	list_del_init(&entry->domain_list);
	list_del_init(&entry->global_list);

	/* 3) free the entry */
>>>>>>> refs/remotes/origin/master
	kfree(entry);
	atomic_dec(&entry_count);

	spin_unlock_irqrestore(&async_lock, flags);

<<<<<<< HEAD
	/* 5) wake up any waiters */
	wake_up(&async_done);
}

static async_cookie_t __async_schedule(async_func_ptr *ptr, void *data, struct list_head *running)
=======
	/* 4) wake up any waiters */
	wake_up(&async_done);
}

static async_cookie_t __async_schedule(async_func_t func, void *data, struct async_domain *domain)
>>>>>>> refs/remotes/origin/master
{
	struct async_entry *entry;
	unsigned long flags;
	async_cookie_t newcookie;

	/* allow irq-off callers */
	entry = kzalloc(sizeof(struct async_entry), GFP_ATOMIC);

	/*
	 * If we're out of memory or if there's too much work
	 * pending already, we execute synchronously.
	 */
	if (!entry || atomic_read(&entry_count) > MAX_WORK) {
		kfree(entry);
		spin_lock_irqsave(&async_lock, flags);
		newcookie = next_cookie++;
		spin_unlock_irqrestore(&async_lock, flags);

		/* low on memory.. run synchronously */
<<<<<<< HEAD
		ptr(data, newcookie);
		return newcookie;
	}
	INIT_WORK(&entry->work, async_run_entry_fn);
	entry->func = ptr;
	entry->data = data;
	entry->running = running;

	spin_lock_irqsave(&async_lock, flags);
	newcookie = entry->cookie = next_cookie++;
	list_add_tail(&entry->list, &async_pending);
	atomic_inc(&entry_count);
	spin_unlock_irqrestore(&async_lock, flags);

=======
		func(data, newcookie);
		return newcookie;
	}
	INIT_LIST_HEAD(&entry->domain_list);
	INIT_LIST_HEAD(&entry->global_list);
	INIT_WORK(&entry->work, async_run_entry_fn);
	entry->func = func;
	entry->data = data;
	entry->domain = domain;

	spin_lock_irqsave(&async_lock, flags);

	/* allocate cookie and queue */
	newcookie = entry->cookie = next_cookie++;

	list_add_tail(&entry->domain_list, &domain->pending);
	if (domain->registered)
		list_add_tail(&entry->global_list, &async_global_pending);

	atomic_inc(&entry_count);
	spin_unlock_irqrestore(&async_lock, flags);

	/* mark that this task has queued an async job, used by module init */
	current->flags |= PF_USED_ASYNC;

>>>>>>> refs/remotes/origin/master
	/* schedule for execution */
	queue_work(system_unbound_wq, &entry->work);

	return newcookie;
}

/**
 * async_schedule - schedule a function for asynchronous execution
<<<<<<< HEAD
 * @ptr: function to execute asynchronously
=======
 * @func: function to execute asynchronously
>>>>>>> refs/remotes/origin/master
 * @data: data pointer to pass to the function
 *
 * Returns an async_cookie_t that may be used for checkpointing later.
 * Note: This function may be called from atomic or non-atomic contexts.
 */
<<<<<<< HEAD
async_cookie_t async_schedule(async_func_ptr *ptr, void *data)
{
	return __async_schedule(ptr, data, &async_running);
=======
async_cookie_t async_schedule(async_func_t func, void *data)
{
	return __async_schedule(func, data, &async_dfl_domain);
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL_GPL(async_schedule);

/**
 * async_schedule_domain - schedule a function for asynchronous execution within a certain domain
<<<<<<< HEAD
 * @ptr: function to execute asynchronously
 * @data: data pointer to pass to the function
 * @running: running list for the domain
 *
 * Returns an async_cookie_t that may be used for checkpointing later.
 * @running may be used in the async_synchronize_*_domain() functions
 * to wait within a certain synchronization domain rather than globally.
 * A synchronization domain is specified via the running queue @running to use.
 * Note: This function may be called from atomic or non-atomic contexts.
 */
async_cookie_t async_schedule_domain(async_func_ptr *ptr, void *data,
				     struct list_head *running)
{
	return __async_schedule(ptr, data, running);
=======
 * @func: function to execute asynchronously
 * @data: data pointer to pass to the function
 * @domain: the domain
 *
 * Returns an async_cookie_t that may be used for checkpointing later.
 * @domain may be used in the async_synchronize_*_domain() functions to
 * wait within a certain synchronization domain rather than globally.  A
 * synchronization domain is specified via @domain.  Note: This function
 * may be called from atomic or non-atomic contexts.
 */
async_cookie_t async_schedule_domain(async_func_t func, void *data,
				     struct async_domain *domain)
{
	return __async_schedule(func, data, domain);
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL_GPL(async_schedule_domain);

/**
 * async_synchronize_full - synchronize all asynchronous function calls
 *
 * This function waits until all asynchronous function calls have been done.
 */
void async_synchronize_full(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	async_synchronize_cookie_domain(next_cookie, NULL);
=======
	async_synchronize_full_domain(NULL);
>>>>>>> refs/remotes/origin/master
=======
	async_synchronize_cookie_domain(next_cookie, NULL);
>>>>>>> refs/remotes/origin/cm-11.0
}
EXPORT_SYMBOL_GPL(async_synchronize_full);

/**
<<<<<<< HEAD
 * async_synchronize_full_domain - synchronize all asynchronous function within a certain domain
 * @list: running list to synchronize on
 *
 * This function waits until all asynchronous function calls for the
 * synchronization domain specified by the running list @list have been done.
 */
void async_synchronize_full_domain(struct list_head *list)
{
	async_synchronize_cookie_domain(next_cookie, list);
=======
 * async_unregister_domain - ensure no more anonymous waiters on this domain
 * @domain: idle domain to flush out of any async_synchronize_full instances
 *
 * async_synchronize_{cookie|full}_domain() are not flushed since callers
 * of these routines should know the lifetime of @domain
 *
 * Prefer ASYNC_DOMAIN_EXCLUSIVE() declarations over flushing
 */
void async_unregister_domain(struct async_domain *domain)
{
	spin_lock_irq(&async_lock);
	WARN_ON(!domain->registered || !list_empty(&domain->pending));
	domain->registered = 0;
	spin_unlock_irq(&async_lock);
}
EXPORT_SYMBOL_GPL(async_unregister_domain);

/**
 * async_synchronize_full_domain - synchronize all asynchronous function within a certain domain
 * @domain: the domain to synchronize
 *
 * This function waits until all asynchronous function calls for the
 * synchronization domain specified by @domain have been done.
 */
void async_synchronize_full_domain(struct async_domain *domain)
{
	async_synchronize_cookie_domain(ASYNC_COOKIE_MAX, domain);
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL_GPL(async_synchronize_full_domain);

/**
 * async_synchronize_cookie_domain - synchronize asynchronous function calls within a certain domain with cookie checkpointing
 * @cookie: async_cookie_t to use as checkpoint
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
 * @running: running list to synchronize on, NULL indicates all lists
 *
 * This function waits until all asynchronous function calls for the
 * synchronization domain specified by the running list @list submitted
 * prior to @cookie have been done.
 */
void async_synchronize_cookie_domain(async_cookie_t cookie,
				     struct list_head *running)
{
<<<<<<< HEAD
	ktime_t starttime, delta, endtime;

	if (initcall_debug && system_state == SYSTEM_BOOTING) {
		printk("async_waiting @ %i\n", task_pid_nr(current));
=======
=======
 * @domain: the domain to synchronize (%NULL for all registered domains)
 *
 * This function waits until all asynchronous function calls for the
 * synchronization domain specified by @domain submitted prior to @cookie
 * have been done.
 */
void async_synchronize_cookie_domain(async_cookie_t cookie, struct async_domain *domain)
{
>>>>>>> refs/remotes/origin/master
	ktime_t uninitialized_var(starttime), delta, endtime;

	if (initcall_debug && system_state == SYSTEM_BOOTING) {
		printk(KERN_DEBUG "async_waiting @ %i\n", task_pid_nr(current));
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		starttime = ktime_get();
	}

	wait_event(async_done, lowest_in_progress(running) >= cookie);
=======
		starttime = ktime_get();
	}

	wait_event(async_done, lowest_in_progress(domain) >= cookie);
>>>>>>> refs/remotes/origin/master

	if (initcall_debug && system_state == SYSTEM_BOOTING) {
		endtime = ktime_get();
		delta = ktime_sub(endtime, starttime);

<<<<<<< HEAD
<<<<<<< HEAD
		printk("async_continuing @ %i after %lli usec\n",
=======
		printk(KERN_DEBUG "async_continuing @ %i after %lli usec\n",
>>>>>>> refs/remotes/origin/cm-10.0
=======
		printk(KERN_DEBUG "async_continuing @ %i after %lli usec\n",
>>>>>>> refs/remotes/origin/master
			task_pid_nr(current),
			(long long)ktime_to_ns(delta) >> 10);
	}
}
EXPORT_SYMBOL_GPL(async_synchronize_cookie_domain);

/**
 * async_synchronize_cookie - synchronize asynchronous function calls with cookie checkpointing
 * @cookie: async_cookie_t to use as checkpoint
 *
 * This function waits until all asynchronous function calls prior to @cookie
 * have been done.
 */
void async_synchronize_cookie(async_cookie_t cookie)
{
<<<<<<< HEAD
	async_synchronize_cookie_domain(cookie, &async_running);
}
EXPORT_SYMBOL_GPL(async_synchronize_cookie);
=======
	async_synchronize_cookie_domain(cookie, &async_dfl_domain);
}
EXPORT_SYMBOL_GPL(async_synchronize_cookie);

/**
 * current_is_async - is %current an async worker task?
 *
 * Returns %true if %current is an async worker task.
 */
bool current_is_async(void)
{
	struct worker *worker = current_wq_worker();

	return worker && worker->current_func == async_run_entry_fn;
}
>>>>>>> refs/remotes/origin/master
