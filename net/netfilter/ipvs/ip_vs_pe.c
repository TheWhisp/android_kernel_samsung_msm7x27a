#define KMSG_COMPONENT "IPVS"
#define pr_fmt(fmt) KMSG_COMPONENT ": " fmt

#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <asm/string.h>
#include <linux/kmod.h>
#include <linux/sysctl.h>

#include <net/ip_vs.h>

/* IPVS pe list */
static LIST_HEAD(ip_vs_pe);

<<<<<<< HEAD
/* lock for service table */
static DEFINE_SPINLOCK(ip_vs_pe_lock);

/* Bind a service with a pe */
void ip_vs_bind_pe(struct ip_vs_service *svc, struct ip_vs_pe *pe)
{
	svc->pe = pe;
}

/* Unbind a service from its pe */
void ip_vs_unbind_pe(struct ip_vs_service *svc)
{
	svc->pe = NULL;
}
=======
/* semaphore for IPVS PEs. */
static DEFINE_MUTEX(ip_vs_pe_mutex);
>>>>>>> refs/remotes/origin/master

/* Get pe in the pe list by name */
struct ip_vs_pe *__ip_vs_pe_getbyname(const char *pe_name)
{
	struct ip_vs_pe *pe;

	IP_VS_DBG(10, "%s(): pe_name \"%s\"\n", __func__,
		  pe_name);

<<<<<<< HEAD
	spin_lock_bh(&ip_vs_pe_lock);

	list_for_each_entry(pe, &ip_vs_pe, n_list) {
=======
	rcu_read_lock();
	list_for_each_entry_rcu(pe, &ip_vs_pe, n_list) {
>>>>>>> refs/remotes/origin/master
		/* Test and get the modules atomically */
		if (pe->module &&
		    !try_module_get(pe->module)) {
			/* This pe is just deleted */
			continue;
		}
		if (strcmp(pe_name, pe->name)==0) {
			/* HIT */
<<<<<<< HEAD
			spin_unlock_bh(&ip_vs_pe_lock);
=======
			rcu_read_unlock();
>>>>>>> refs/remotes/origin/master
			return pe;
		}
		if (pe->module)
			module_put(pe->module);
	}
<<<<<<< HEAD

	spin_unlock_bh(&ip_vs_pe_lock);
=======
	rcu_read_unlock();

>>>>>>> refs/remotes/origin/master
	return NULL;
}

/* Lookup pe and try to load it if it doesn't exist */
struct ip_vs_pe *ip_vs_pe_getbyname(const char *name)
{
	struct ip_vs_pe *pe;

	/* Search for the pe by name */
	pe = __ip_vs_pe_getbyname(name);

	/* If pe not found, load the module and search again */
	if (!pe) {
		request_module("ip_vs_pe_%s", name);
		pe = __ip_vs_pe_getbyname(name);
	}

	return pe;
}

/* Register a pe in the pe list */
int register_ip_vs_pe(struct ip_vs_pe *pe)
{
	struct ip_vs_pe *tmp;

	/* increase the module use count */
	ip_vs_use_count_inc();

<<<<<<< HEAD
	spin_lock_bh(&ip_vs_pe_lock);

	if (!list_empty(&pe->n_list)) {
		spin_unlock_bh(&ip_vs_pe_lock);
		ip_vs_use_count_dec();
		pr_err("%s(): [%s] pe already linked\n",
		       __func__, pe->name);
		return -EINVAL;
	}

=======
	mutex_lock(&ip_vs_pe_mutex);
>>>>>>> refs/remotes/origin/master
	/* Make sure that the pe with this name doesn't exist
	 * in the pe list.
	 */
	list_for_each_entry(tmp, &ip_vs_pe, n_list) {
		if (strcmp(tmp->name, pe->name) == 0) {
<<<<<<< HEAD
			spin_unlock_bh(&ip_vs_pe_lock);
=======
			mutex_unlock(&ip_vs_pe_mutex);
>>>>>>> refs/remotes/origin/master
			ip_vs_use_count_dec();
			pr_err("%s(): [%s] pe already existed "
			       "in the system\n", __func__, pe->name);
			return -EINVAL;
		}
	}
	/* Add it into the d-linked pe list */
<<<<<<< HEAD
	list_add(&pe->n_list, &ip_vs_pe);
	spin_unlock_bh(&ip_vs_pe_lock);
=======
	list_add_rcu(&pe->n_list, &ip_vs_pe);
	mutex_unlock(&ip_vs_pe_mutex);
>>>>>>> refs/remotes/origin/master

	pr_info("[%s] pe registered.\n", pe->name);

	return 0;
}
EXPORT_SYMBOL_GPL(register_ip_vs_pe);

/* Unregister a pe from the pe list */
int unregister_ip_vs_pe(struct ip_vs_pe *pe)
{
<<<<<<< HEAD
	spin_lock_bh(&ip_vs_pe_lock);
	if (list_empty(&pe->n_list)) {
		spin_unlock_bh(&ip_vs_pe_lock);
		pr_err("%s(): [%s] pe is not in the list. failed\n",
		       __func__, pe->name);
		return -EINVAL;
	}

	/* Remove it from the d-linked pe list */
	list_del(&pe->n_list);
	spin_unlock_bh(&ip_vs_pe_lock);
=======
	mutex_lock(&ip_vs_pe_mutex);
	/* Remove it from the d-linked pe list */
	list_del_rcu(&pe->n_list);
	mutex_unlock(&ip_vs_pe_mutex);
>>>>>>> refs/remotes/origin/master

	/* decrease the module use count */
	ip_vs_use_count_dec();

	pr_info("[%s] pe unregistered.\n", pe->name);

	return 0;
}
EXPORT_SYMBOL_GPL(unregister_ip_vs_pe);
