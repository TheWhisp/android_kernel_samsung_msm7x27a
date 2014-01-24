/*
 * IPVS         An implementation of the IP virtual server support for the
 *              LINUX operating system.  IPVS is now implemented as a module
 *              over the Netfilter framework. IPVS can be used to build a
 *              high-performance and highly available server based on a
 *              cluster of servers.
 *
 * Authors:     Wensong Zhang <wensong@linuxvirtualserver.org>
 *              Peter Kese <peter.kese@ijs.si>
 *
 *              This program is free software; you can redistribute it and/or
 *              modify it under the terms of the GNU General Public License
 *              as published by the Free Software Foundation; either version
 *              2 of the License, or (at your option) any later version.
 *
 * Changes:
 *
 */

#define KMSG_COMPONENT "IPVS"
#define pr_fmt(fmt) KMSG_COMPONENT ": " fmt

#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <asm/string.h>
#include <linux/kmod.h>
#include <linux/sysctl.h>

#include <net/ip_vs.h>

EXPORT_SYMBOL(ip_vs_scheduler_err);
/*
 *  IPVS scheduler list
 */
static LIST_HEAD(ip_vs_schedulers);

<<<<<<< HEAD
/* lock for service table */
static DEFINE_SPINLOCK(ip_vs_sched_lock);
=======
/* semaphore for schedulers */
static DEFINE_MUTEX(ip_vs_sched_mutex);
>>>>>>> refs/remotes/origin/master


/*
 *  Bind a service with a scheduler
 */
int ip_vs_bind_scheduler(struct ip_vs_service *svc,
			 struct ip_vs_scheduler *scheduler)
{
	int ret;

<<<<<<< HEAD
	svc->scheduler = scheduler;

=======
>>>>>>> refs/remotes/origin/master
	if (scheduler->init_service) {
		ret = scheduler->init_service(svc);
		if (ret) {
			pr_err("%s(): init error\n", __func__);
			return ret;
		}
	}
<<<<<<< HEAD

=======
	rcu_assign_pointer(svc->scheduler, scheduler);
>>>>>>> refs/remotes/origin/master
	return 0;
}


/*
 *  Unbind a service with its scheduler
 */
<<<<<<< HEAD
int ip_vs_unbind_scheduler(struct ip_vs_service *svc)
{
	struct ip_vs_scheduler *sched = svc->scheduler;

	if (!sched)
		return 0;

	if (sched->done_service) {
		if (sched->done_service(svc) != 0) {
			pr_err("%s(): done error\n", __func__);
			return -EINVAL;
		}
	}

	svc->scheduler = NULL;
	return 0;
=======
void ip_vs_unbind_scheduler(struct ip_vs_service *svc,
			    struct ip_vs_scheduler *sched)
{
	struct ip_vs_scheduler *cur_sched;

	cur_sched = rcu_dereference_protected(svc->scheduler, 1);
	/* This check proves that old 'sched' was installed */
	if (!cur_sched)
		return;

	if (sched->done_service)
		sched->done_service(svc);
	/* svc->scheduler can not be set to NULL */
>>>>>>> refs/remotes/origin/master
}


/*
 *  Get scheduler in the scheduler list by name
 */
static struct ip_vs_scheduler *ip_vs_sched_getbyname(const char *sched_name)
{
	struct ip_vs_scheduler *sched;

	IP_VS_DBG(2, "%s(): sched_name \"%s\"\n", __func__, sched_name);

<<<<<<< HEAD
	spin_lock_bh(&ip_vs_sched_lock);
=======
	mutex_lock(&ip_vs_sched_mutex);
>>>>>>> refs/remotes/origin/master

	list_for_each_entry(sched, &ip_vs_schedulers, n_list) {
		/*
		 * Test and get the modules atomically
		 */
		if (sched->module && !try_module_get(sched->module)) {
			/*
			 * This scheduler is just deleted
			 */
			continue;
		}
		if (strcmp(sched_name, sched->name)==0) {
			/* HIT */
<<<<<<< HEAD
			spin_unlock_bh(&ip_vs_sched_lock);
=======
			mutex_unlock(&ip_vs_sched_mutex);
>>>>>>> refs/remotes/origin/master
			return sched;
		}
		if (sched->module)
			module_put(sched->module);
	}

<<<<<<< HEAD
	spin_unlock_bh(&ip_vs_sched_lock);
=======
	mutex_unlock(&ip_vs_sched_mutex);
>>>>>>> refs/remotes/origin/master
	return NULL;
}


/*
 *  Lookup scheduler and try to load it if it doesn't exist
 */
struct ip_vs_scheduler *ip_vs_scheduler_get(const char *sched_name)
{
	struct ip_vs_scheduler *sched;

	/*
	 *  Search for the scheduler by sched_name
	 */
	sched = ip_vs_sched_getbyname(sched_name);

	/*
	 *  If scheduler not found, load the module and search again
	 */
	if (sched == NULL) {
		request_module("ip_vs_%s", sched_name);
		sched = ip_vs_sched_getbyname(sched_name);
	}

	return sched;
}

void ip_vs_scheduler_put(struct ip_vs_scheduler *scheduler)
{
	if (scheduler && scheduler->module)
		module_put(scheduler->module);
}

/*
 * Common error output helper for schedulers
 */

void ip_vs_scheduler_err(struct ip_vs_service *svc, const char *msg)
{
<<<<<<< HEAD
	if (svc->fwmark) {
		IP_VS_ERR_RL("%s: FWM %u 0x%08X - %s\n",
			     svc->scheduler->name, svc->fwmark,
			     svc->fwmark, msg);
#ifdef CONFIG_IP_VS_IPV6
	} else if (svc->af == AF_INET6) {
		IP_VS_ERR_RL("%s: %s [%pI6]:%d - %s\n",
			     svc->scheduler->name,
			     ip_vs_proto_name(svc->protocol),
=======
	struct ip_vs_scheduler *sched;

	sched = rcu_dereference(svc->scheduler);
	if (svc->fwmark) {
		IP_VS_ERR_RL("%s: FWM %u 0x%08X - %s\n",
			     sched->name, svc->fwmark, svc->fwmark, msg);
#ifdef CONFIG_IP_VS_IPV6
	} else if (svc->af == AF_INET6) {
		IP_VS_ERR_RL("%s: %s [%pI6c]:%d - %s\n",
			     sched->name, ip_vs_proto_name(svc->protocol),
>>>>>>> refs/remotes/origin/master
			     &svc->addr.in6, ntohs(svc->port), msg);
#endif
	} else {
		IP_VS_ERR_RL("%s: %s %pI4:%d - %s\n",
<<<<<<< HEAD
			     svc->scheduler->name,
			     ip_vs_proto_name(svc->protocol),
=======
			     sched->name, ip_vs_proto_name(svc->protocol),
>>>>>>> refs/remotes/origin/master
			     &svc->addr.ip, ntohs(svc->port), msg);
	}
}

/*
 *  Register a scheduler in the scheduler list
 */
int register_ip_vs_scheduler(struct ip_vs_scheduler *scheduler)
{
	struct ip_vs_scheduler *sched;

	if (!scheduler) {
		pr_err("%s(): NULL arg\n", __func__);
		return -EINVAL;
	}

	if (!scheduler->name) {
		pr_err("%s(): NULL scheduler_name\n", __func__);
		return -EINVAL;
	}

	/* increase the module use count */
	ip_vs_use_count_inc();

<<<<<<< HEAD
	spin_lock_bh(&ip_vs_sched_lock);

	if (!list_empty(&scheduler->n_list)) {
		spin_unlock_bh(&ip_vs_sched_lock);
=======
	mutex_lock(&ip_vs_sched_mutex);

	if (!list_empty(&scheduler->n_list)) {
		mutex_unlock(&ip_vs_sched_mutex);
>>>>>>> refs/remotes/origin/master
		ip_vs_use_count_dec();
		pr_err("%s(): [%s] scheduler already linked\n",
		       __func__, scheduler->name);
		return -EINVAL;
	}

	/*
	 *  Make sure that the scheduler with this name doesn't exist
	 *  in the scheduler list.
	 */
	list_for_each_entry(sched, &ip_vs_schedulers, n_list) {
		if (strcmp(scheduler->name, sched->name) == 0) {
<<<<<<< HEAD
			spin_unlock_bh(&ip_vs_sched_lock);
=======
			mutex_unlock(&ip_vs_sched_mutex);
>>>>>>> refs/remotes/origin/master
			ip_vs_use_count_dec();
			pr_err("%s(): [%s] scheduler already existed "
			       "in the system\n", __func__, scheduler->name);
			return -EINVAL;
		}
	}
	/*
	 *	Add it into the d-linked scheduler list
	 */
	list_add(&scheduler->n_list, &ip_vs_schedulers);
<<<<<<< HEAD
	spin_unlock_bh(&ip_vs_sched_lock);
=======
	mutex_unlock(&ip_vs_sched_mutex);
>>>>>>> refs/remotes/origin/master

	pr_info("[%s] scheduler registered.\n", scheduler->name);

	return 0;
}


/*
 *  Unregister a scheduler from the scheduler list
 */
int unregister_ip_vs_scheduler(struct ip_vs_scheduler *scheduler)
{
	if (!scheduler) {
		pr_err("%s(): NULL arg\n", __func__);
		return -EINVAL;
	}

<<<<<<< HEAD
	spin_lock_bh(&ip_vs_sched_lock);
	if (list_empty(&scheduler->n_list)) {
		spin_unlock_bh(&ip_vs_sched_lock);
=======
	mutex_lock(&ip_vs_sched_mutex);
	if (list_empty(&scheduler->n_list)) {
		mutex_unlock(&ip_vs_sched_mutex);
>>>>>>> refs/remotes/origin/master
		pr_err("%s(): [%s] scheduler is not in the list. failed\n",
		       __func__, scheduler->name);
		return -EINVAL;
	}

	/*
	 *	Remove it from the d-linked scheduler list
	 */
	list_del(&scheduler->n_list);
<<<<<<< HEAD
	spin_unlock_bh(&ip_vs_sched_lock);
=======
	mutex_unlock(&ip_vs_sched_mutex);
>>>>>>> refs/remotes/origin/master

	/* decrease the module use count */
	ip_vs_use_count_dec();

	pr_info("[%s] scheduler unregistered.\n", scheduler->name);

	return 0;
}
