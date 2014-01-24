<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/irq.h>
=======
=======
#define pr_fmt(fmt)  "irq: " fmt

>>>>>>> refs/remotes/origin/master
#include <linux/debugfs.h>
#include <linux/hardirq.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/irqdesc.h>
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#include <linux/irqdomain.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/of.h>
#include <linux/of_address.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/slab.h>
=======
=======
#include <linux/topology.h>
>>>>>>> refs/remotes/origin/master
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/smp.h>
#include <linux/fs.h>

<<<<<<< HEAD
#define IRQ_DOMAIN_MAP_LEGACY 0 /* driver allocated fixed range of irqs.
				 * ie. legacy 8259, gets irqs 1..15 */
#define IRQ_DOMAIN_MAP_NOMAP 1 /* no fast reverse mapping */
#define IRQ_DOMAIN_MAP_LINEAR 2 /* linear map of interrupts */
#define IRQ_DOMAIN_MAP_TREE 3 /* radix tree */
>>>>>>> refs/remotes/origin/cm-10.0

static LIST_HEAD(irq_domain_list);
static DEFINE_MUTEX(irq_domain_mutex);

<<<<<<< HEAD
/**
 * irq_domain_add() - Register an irq_domain
 * @domain: ptr to initialized irq_domain structure
 *
 * Registers an irq_domain structure.  The irq_domain must at a minimum be
 * initialized with an ops structure pointer, and either a ->to_irq hook or
 * a valid irq_base value.  Everything else is optional.
 */
void irq_domain_add(struct irq_domain *domain)
{
	struct irq_data *d;
	int hwirq, irq;

	/*
	 * This assumes that the irq_domain owner has already allocated
	 * the irq_descs.  This block will be removed when support for dynamic
	 * allocation of irq_descs is added to irq_domain.
	 */
	irq_domain_for_each_irq(domain, hwirq, irq) {
		d = irq_get_irq_data(irq);
		if (!d) {
			WARN(1, "error: assigning domain to non existant irq_desc");
			return;
		}
		if (d->domain) {
			/* things are broken; just report, don't clean up */
			WARN(1, "error: irq_desc already assigned to a domain");
			return;
		}
		d->domain = domain;
		d->hwirq = hwirq;
	}

	mutex_lock(&irq_domain_mutex);
	list_add(&domain->list, &irq_domain_list);
	mutex_unlock(&irq_domain_mutex);
}

/**
 * irq_domain_del() - Unregister an irq_domain
 * @domain: ptr to registered irq_domain.
 */
void irq_domain_del(struct irq_domain *domain)
{
	struct irq_data *d;
	int hwirq, irq;

	mutex_lock(&irq_domain_mutex);
	list_del(&domain->list);
	mutex_unlock(&irq_domain_mutex);

	/* Clear the irq_domain assignments */
	irq_domain_for_each_irq(domain, hwirq, irq) {
		d = irq_get_irq_data(irq);
		d->domain = NULL;
	}
}

#if defined(CONFIG_OF_IRQ)
/**
 * irq_create_of_mapping() - Map a linux irq number from a DT interrupt spec
 *
 * Used by the device tree interrupt mapping code to translate a device tree
 * interrupt specifier to a valid linux irq number.  Returns either a valid
 * linux IRQ number or 0.
 *
 * When the caller no longer need the irq number returned by this function it
 * should arrange to call irq_dispose_mapping().
 */
=======
=======
static LIST_HEAD(irq_domain_list);
static DEFINE_MUTEX(irq_domain_mutex);

>>>>>>> refs/remotes/origin/master
static DEFINE_MUTEX(revmap_trees_mutex);
static struct irq_domain *irq_default_domain;

/**
<<<<<<< HEAD
 * irq_domain_alloc() - Allocate a new irq_domain data structure
 * @of_node: optional device-tree node of the interrupt controller
 * @revmap_type: type of reverse mapping to use
=======
 * __irq_domain_add() - Allocate a new irq_domain data structure
 * @of_node: optional device-tree node of the interrupt controller
 * @size: Size of linear map; 0 for radix mapping only
 * @direct_max: Maximum value of direct maps; Use ~0 for no limit; 0 for no
 *              direct mapping
>>>>>>> refs/remotes/origin/master
 * @ops: map/unmap domain callbacks
 * @host_data: Controller private data pointer
 *
 * Allocates and initialize and irq_domain structure.  Caller is expected to
 * register allocated irq_domain with irq_domain_register().  Returns pointer
 * to IRQ domain, or NULL on failure.
 */
<<<<<<< HEAD
static struct irq_domain *irq_domain_alloc(struct device_node *of_node,
					   unsigned int revmap_type,
					   const struct irq_domain_ops *ops,
					   void *host_data)
{
	struct irq_domain *domain;

	domain = kzalloc(sizeof(*domain), GFP_KERNEL);
=======
struct irq_domain *__irq_domain_add(struct device_node *of_node, int size,
				    irq_hw_number_t hwirq_max, int direct_max,
				    const struct irq_domain_ops *ops,
				    void *host_data)
{
	struct irq_domain *domain;

	domain = kzalloc_node(sizeof(*domain) + (sizeof(unsigned int) * size),
			      GFP_KERNEL, of_node_to_nid(of_node));
>>>>>>> refs/remotes/origin/master
	if (WARN_ON(!domain))
		return NULL;

	/* Fill structure */
<<<<<<< HEAD
	domain->revmap_type = revmap_type;
	domain->ops = ops;
	domain->host_data = host_data;
	domain->of_node = of_node_get(of_node);

	return domain;
}

static void irq_domain_add(struct irq_domain *domain)
{
	mutex_lock(&irq_domain_mutex);
	list_add(&domain->link, &irq_domain_list);
	mutex_unlock(&irq_domain_mutex);
	pr_debug("irq: Allocated domain of type %d @0x%p\n",
		 domain->revmap_type, domain);
}

static unsigned int irq_domain_legacy_revmap(struct irq_domain *domain,
					     irq_hw_number_t hwirq)
{
	irq_hw_number_t first_hwirq = domain->revmap_data.legacy.first_hwirq;
	int size = domain->revmap_data.legacy.size;

	if (WARN_ON(hwirq < first_hwirq || hwirq >= first_hwirq + size))
		return 0;
	return hwirq - first_hwirq + domain->revmap_data.legacy.first_irq;
}

/**
 * irq_domain_add_legacy() - Allocate and register a legacy revmap irq_domain.
 * @of_node: pointer to interrupt controller's device tree node.
 * @size: total number of irqs in legacy mapping
 * @first_irq: first number of irq block assigned to the domain
 * @first_hwirq: first hwirq number to use for the translation. Should normally
 *               be '0', but a positive integer can be used if the effective
 *               hwirqs numbering does not begin at zero.
 * @ops: map/unmap domain callbacks
 * @host_data: Controller private data pointer
 *
 * Note: the map() callback will be called before this function returns
 * for all legacy interrupts except 0 (which is always the invalid irq for
 * a legacy controller).
 */
struct irq_domain *irq_domain_add_legacy(struct device_node *of_node,
					 unsigned int size,
					 unsigned int first_irq,
					 irq_hw_number_t first_hwirq,
=======
	INIT_RADIX_TREE(&domain->revmap_tree, GFP_KERNEL);
	domain->ops = ops;
	domain->host_data = host_data;
	domain->of_node = of_node_get(of_node);
	domain->hwirq_max = hwirq_max;
	domain->revmap_size = size;
	domain->revmap_direct_max_irq = direct_max;

	mutex_lock(&irq_domain_mutex);
	list_add(&domain->link, &irq_domain_list);
	mutex_unlock(&irq_domain_mutex);

	pr_debug("Added domain %s\n", domain->name);
	return domain;
}
EXPORT_SYMBOL_GPL(__irq_domain_add);

/**
 * irq_domain_remove() - Remove an irq domain.
 * @domain: domain to remove
 *
 * This routine is used to remove an irq domain. The caller must ensure
 * that all mappings within the domain have been disposed of prior to
 * use, depending on the revmap type.
 */
void irq_domain_remove(struct irq_domain *domain)
{
	mutex_lock(&irq_domain_mutex);

	/*
	 * radix_tree_delete() takes care of destroying the root
	 * node when all entries are removed. Shout if there are
	 * any mappings left.
	 */
	WARN_ON(domain->revmap_tree.height);

	list_del(&domain->link);

	/*
	 * If the going away domain is the default one, reset it.
	 */
	if (unlikely(irq_default_domain == domain))
		irq_set_default_host(NULL);

	mutex_unlock(&irq_domain_mutex);

	pr_debug("Removed domain %s\n", domain->name);

	of_node_put(domain->of_node);
	kfree(domain);
}
EXPORT_SYMBOL_GPL(irq_domain_remove);

/**
 * irq_domain_add_simple() - Register an irq_domain and optionally map a range of irqs
 * @of_node: pointer to interrupt controller's device tree node.
 * @size: total number of irqs in mapping
 * @first_irq: first number of irq block assigned to the domain,
 *	pass zero to assign irqs on-the-fly. If first_irq is non-zero, then
 *	pre-map all of the irqs in the domain to virqs starting at first_irq.
 * @ops: map/unmap domain callbacks
 * @host_data: Controller private data pointer
 *
 * Allocates an irq_domain, and optionally if first_irq is positive then also
 * allocate irq_descs and map all of the hwirqs to virqs starting at first_irq.
 *
 * This is intended to implement the expected behaviour for most
 * interrupt controllers. If device tree is used, then first_irq will be 0 and
 * irqs get mapped dynamically on the fly. However, if the controller requires
 * static virq assignments (non-DT boot) then it will set that up correctly.
 */
struct irq_domain *irq_domain_add_simple(struct device_node *of_node,
					 unsigned int size,
					 unsigned int first_irq,
>>>>>>> refs/remotes/origin/master
					 const struct irq_domain_ops *ops,
					 void *host_data)
{
	struct irq_domain *domain;
<<<<<<< HEAD
	unsigned int i;

	domain = irq_domain_alloc(of_node, IRQ_DOMAIN_MAP_LEGACY, ops, host_data);
	if (!domain)
		return NULL;

	domain->revmap_data.legacy.first_irq = first_irq;
	domain->revmap_data.legacy.first_hwirq = first_hwirq;
	domain->revmap_data.legacy.size = size;

	mutex_lock(&irq_domain_mutex);
	/* Verify that all the irqs are available */
	for (i = 0; i < size; i++) {
		int irq = first_irq + i;
		struct irq_data *irq_data = irq_get_irq_data(irq);

		if (WARN_ON(!irq_data || irq_data->domain)) {
			mutex_unlock(&irq_domain_mutex);
			of_node_put(domain->of_node);
			kfree(domain);
			return NULL;
		}
	}

	/* Claim all of the irqs before registering a legacy domain */
	for (i = 0; i < size; i++) {
		struct irq_data *irq_data = irq_get_irq_data(first_irq + i);
		irq_data->hwirq = first_hwirq + i;
		irq_data->domain = domain;
	}
	mutex_unlock(&irq_domain_mutex);

	for (i = 0; i < size; i++) {
		int irq = first_irq + i;
		int hwirq = first_hwirq + i;

		/* IRQ0 gets ignored */
		if (!irq)
			continue;

		/* Legacy flags are left to default at this point,
		 * one can then use irq_create_mapping() to
		 * explicitly change them
		 */
		ops->map(domain, irq, hwirq);

		/* Clear norequest flags */
		irq_clear_status_flags(irq, IRQ_NOREQUEST);
	}

	irq_domain_add(domain);
	return domain;
}

/**
 * irq_domain_add_linear() - Allocate and register a legacy revmap irq_domain.
 * @of_node: pointer to interrupt controller's device tree node.
 * @ops: map/unmap domain callbacks
 * @host_data: Controller private data pointer
 */
struct irq_domain *irq_domain_add_linear(struct device_node *of_node,
					 unsigned int size,
=======

	domain = __irq_domain_add(of_node, size, size, 0, ops, host_data);
	if (!domain)
		return NULL;

	if (first_irq > 0) {
		if (IS_ENABLED(CONFIG_SPARSE_IRQ)) {
			/* attempt to allocated irq_descs */
			int rc = irq_alloc_descs(first_irq, first_irq, size,
						 of_node_to_nid(of_node));
			if (rc < 0)
				pr_info("Cannot allocate irq_descs @ IRQ%d, assuming pre-allocated\n",
					first_irq);
		}
		irq_domain_associate_many(domain, first_irq, 0, size);
	}

	return domain;
}
EXPORT_SYMBOL_GPL(irq_domain_add_simple);

/**
 * irq_domain_add_legacy() - Allocate and register a legacy revmap irq_domain.
 * @of_node: pointer to interrupt controller's device tree node.
 * @size: total number of irqs in legacy mapping
 * @first_irq: first number of irq block assigned to the domain
 * @first_hwirq: first hwirq number to use for the translation. Should normally
 *               be '0', but a positive integer can be used if the effective
 *               hwirqs numbering does not begin at zero.
 * @ops: map/unmap domain callbacks
 * @host_data: Controller private data pointer
 *
 * Note: the map() callback will be called before this function returns
 * for all legacy interrupts except 0 (which is always the invalid irq for
 * a legacy controller).
 */
struct irq_domain *irq_domain_add_legacy(struct device_node *of_node,
					 unsigned int size,
					 unsigned int first_irq,
					 irq_hw_number_t first_hwirq,
>>>>>>> refs/remotes/origin/master
					 const struct irq_domain_ops *ops,
					 void *host_data)
{
	struct irq_domain *domain;
<<<<<<< HEAD
	unsigned int *revmap;

	revmap = kzalloc(sizeof(*revmap) * size, GFP_KERNEL);
	if (WARN_ON(!revmap))
		return NULL;

	domain = irq_domain_alloc(of_node, IRQ_DOMAIN_MAP_LINEAR, ops, host_data);
	if (!domain) {
		kfree(revmap);
		return NULL;
	}
	domain->revmap_data.linear.size = size;
	domain->revmap_data.linear.revmap = revmap;
	irq_domain_add(domain);
	return domain;
}

struct irq_domain *irq_domain_add_nomap(struct device_node *of_node,
					 unsigned int max_irq,
					 const struct irq_domain_ops *ops,
					 void *host_data)
{
	struct irq_domain *domain = irq_domain_alloc(of_node,
					IRQ_DOMAIN_MAP_NOMAP, ops, host_data);
	if (domain) {
		domain->revmap_data.nomap.max_irq = max_irq ? max_irq : ~0;
		irq_domain_add(domain);
	}
	return domain;
}

/**
 * irq_domain_add_tree()
 * @of_node: pointer to interrupt controller's device tree node.
 * @ops: map/unmap domain callbacks
 *
 * Note: The radix tree will be allocated later during boot automatically
 * (the reverse mapping will use the slow path until that happens).
 */
struct irq_domain *irq_domain_add_tree(struct device_node *of_node,
					 const struct irq_domain_ops *ops,
					 void *host_data)
{
	struct irq_domain *domain = irq_domain_alloc(of_node,
					IRQ_DOMAIN_MAP_TREE, ops, host_data);
	if (domain) {
		INIT_RADIX_TREE(&domain->revmap_data.tree, GFP_KERNEL);
		irq_domain_add(domain);
	}
	return domain;
}
=======

	domain = __irq_domain_add(of_node, first_hwirq + size,
				  first_hwirq + size, 0, ops, host_data);
	if (!domain)
		return NULL;

	irq_domain_associate_many(domain, first_irq, first_hwirq, size);

	return domain;
}
EXPORT_SYMBOL_GPL(irq_domain_add_legacy);
>>>>>>> refs/remotes/origin/master

/**
 * irq_find_host() - Locates a domain for a given device node
 * @node: device-tree node of the interrupt controller
 */
struct irq_domain *irq_find_host(struct device_node *node)
{
	struct irq_domain *h, *found = NULL;
	int rc;

	/* We might want to match the legacy controller last since
	 * it might potentially be set to match all interrupts in
	 * the absence of a device node. This isn't a problem so far
	 * yet though...
	 */
	mutex_lock(&irq_domain_mutex);
	list_for_each_entry(h, &irq_domain_list, link) {
		if (h->ops->match)
			rc = h->ops->match(h, node);
		else
			rc = (h->of_node != NULL) && (h->of_node == node);

		if (rc) {
			found = h;
			break;
		}
	}
	mutex_unlock(&irq_domain_mutex);
	return found;
}
EXPORT_SYMBOL_GPL(irq_find_host);

/**
 * irq_set_default_host() - Set a "default" irq domain
 * @domain: default domain pointer
 *
 * For convenience, it's possible to set a "default" domain that will be used
 * whenever NULL is passed to irq_create_mapping(). It makes life easier for
 * platforms that want to manipulate a few hard coded interrupt numbers that
 * aren't properly represented in the device-tree.
 */
void irq_set_default_host(struct irq_domain *domain)
{
<<<<<<< HEAD
	pr_debug("irq: Default domain set to @0x%p\n", domain);

	irq_default_domain = domain;
}

static int irq_setup_virq(struct irq_domain *domain, unsigned int virq,
			    irq_hw_number_t hwirq)
{
	struct irq_data *irq_data = irq_get_irq_data(virq);

	irq_data->hwirq = hwirq;
	irq_data->domain = domain;
	if (domain->ops->map(domain, virq, hwirq)) {
		pr_debug("irq: -> mapping failed, freeing\n");
		irq_data->domain = NULL;
		irq_data->hwirq = 0;
		return -1;
	}

=======
	pr_debug("Default domain set to @0x%p\n", domain);

	irq_default_domain = domain;
}
EXPORT_SYMBOL_GPL(irq_set_default_host);

static void irq_domain_disassociate(struct irq_domain *domain, unsigned int irq)
{
	struct irq_data *irq_data = irq_get_irq_data(irq);
	irq_hw_number_t hwirq;

	if (WARN(!irq_data || irq_data->domain != domain,
		 "virq%i doesn't exist; cannot disassociate\n", irq))
		return;

	hwirq = irq_data->hwirq;
	irq_set_status_flags(irq, IRQ_NOREQUEST);

	/* remove chip and handler */
	irq_set_chip_and_handler(irq, NULL, NULL);

	/* Make sure it's completed */
	synchronize_irq(irq);

	/* Tell the PIC about it */
	if (domain->ops->unmap)
		domain->ops->unmap(domain, irq);
	smp_mb();

	irq_data->domain = NULL;
	irq_data->hwirq = 0;

	/* Clear reverse map for this hwirq */
	if (hwirq < domain->revmap_size) {
		domain->linear_revmap[hwirq] = 0;
	} else {
		mutex_lock(&revmap_trees_mutex);
		radix_tree_delete(&domain->revmap_tree, hwirq);
		mutex_unlock(&revmap_trees_mutex);
	}
}

int irq_domain_associate(struct irq_domain *domain, unsigned int virq,
			 irq_hw_number_t hwirq)
{
	struct irq_data *irq_data = irq_get_irq_data(virq);
	int ret;

	if (WARN(hwirq >= domain->hwirq_max,
		 "error: hwirq 0x%x is too large for %s\n", (int)hwirq, domain->name))
		return -EINVAL;
	if (WARN(!irq_data, "error: virq%i is not allocated", virq))
		return -EINVAL;
	if (WARN(irq_data->domain, "error: virq%i is already associated", virq))
		return -EINVAL;

	mutex_lock(&irq_domain_mutex);
	irq_data->hwirq = hwirq;
	irq_data->domain = domain;
	if (domain->ops->map) {
		ret = domain->ops->map(domain, virq, hwirq);
		if (ret != 0) {
			/*
			 * If map() returns -EPERM, this interrupt is protected
			 * by the firmware or some other service and shall not
			 * be mapped. Don't bother telling the user about it.
			 */
			if (ret != -EPERM) {
				pr_info("%s didn't like hwirq-0x%lx to VIRQ%i mapping (rc=%d)\n",
				       domain->name, hwirq, virq, ret);
			}
			irq_data->domain = NULL;
			irq_data->hwirq = 0;
			mutex_unlock(&irq_domain_mutex);
			return ret;
		}

		/* If not already assigned, give the domain the chip's name */
		if (!domain->name && irq_data->chip)
			domain->name = irq_data->chip->name;
	}

	if (hwirq < domain->revmap_size) {
		domain->linear_revmap[hwirq] = virq;
	} else {
		mutex_lock(&revmap_trees_mutex);
		radix_tree_insert(&domain->revmap_tree, hwirq, irq_data);
		mutex_unlock(&revmap_trees_mutex);
	}
	mutex_unlock(&irq_domain_mutex);

>>>>>>> refs/remotes/origin/master
	irq_clear_status_flags(virq, IRQ_NOREQUEST);

	return 0;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(irq_domain_associate);

void irq_domain_associate_many(struct irq_domain *domain, unsigned int irq_base,
			       irq_hw_number_t hwirq_base, int count)
{
	int i;

	pr_debug("%s(%s, irqbase=%i, hwbase=%i, count=%i)\n", __func__,
		of_node_full_name(domain->of_node), irq_base, (int)hwirq_base, count);

	for (i = 0; i < count; i++) {
		irq_domain_associate(domain, irq_base + i, hwirq_base + i);
	}
}
EXPORT_SYMBOL_GPL(irq_domain_associate_many);
>>>>>>> refs/remotes/origin/master

/**
 * irq_create_direct_mapping() - Allocate an irq for direct mapping
 * @domain: domain to allocate the irq for or NULL for default domain
 *
 * This routine is used for irq controllers which can choose the hardware
 * interrupt numbers they generate. In such a case it's simplest to use
<<<<<<< HEAD
 * the linux irq as the hardware interrupt number.
=======
 * the linux irq as the hardware interrupt number. It still uses the linear
 * or radix tree to store the mapping, but the irq controller can optimize
 * the revmap path by using the hwirq directly.
>>>>>>> refs/remotes/origin/master
 */
unsigned int irq_create_direct_mapping(struct irq_domain *domain)
{
	unsigned int virq;

	if (domain == NULL)
		domain = irq_default_domain;

<<<<<<< HEAD
	BUG_ON(domain == NULL);
	WARN_ON(domain->revmap_type != IRQ_DOMAIN_MAP_NOMAP);

	virq = irq_alloc_desc_from(1, 0);
	if (!virq) {
		pr_debug("irq: create_direct virq allocation failed\n");
		return 0;
	}
	if (virq >= domain->revmap_data.nomap.max_irq) {
		pr_err("ERROR: no free irqs available below %i maximum\n",
			domain->revmap_data.nomap.max_irq);
		irq_free_desc(virq);
		return 0;
	}
	pr_debug("irq: create_direct obtained virq %d\n", virq);

	if (irq_setup_virq(domain, virq, virq)) {
=======
	virq = irq_alloc_desc_from(1, of_node_to_nid(domain->of_node));
	if (!virq) {
		pr_debug("create_direct virq allocation failed\n");
		return 0;
	}
	if (virq >= domain->revmap_direct_max_irq) {
		pr_err("ERROR: no free irqs available below %i maximum\n",
			domain->revmap_direct_max_irq);
		irq_free_desc(virq);
		return 0;
	}
	pr_debug("create_direct obtained virq %d\n", virq);

	if (irq_domain_associate(domain, virq, virq)) {
>>>>>>> refs/remotes/origin/master
		irq_free_desc(virq);
		return 0;
	}

	return virq;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(irq_create_direct_mapping);
>>>>>>> refs/remotes/origin/master

/**
 * irq_create_mapping() - Map a hardware interrupt into linux irq space
 * @domain: domain owning this hardware interrupt or NULL for default domain
 * @hwirq: hardware irq number in that domain space
 *
 * Only one mapping per hardware interrupt is permitted. Returns a linux
 * irq number.
 * If the sense/trigger is to be specified, set_irq_type() should be called
 * on the number returned from that call.
 */
unsigned int irq_create_mapping(struct irq_domain *domain,
				irq_hw_number_t hwirq)
{
	unsigned int hint;
	int virq;

<<<<<<< HEAD
	pr_debug("irq: irq_create_mapping(0x%p, 0x%lx)\n", domain, hwirq);
=======
	pr_debug("irq_create_mapping(0x%p, 0x%lx)\n", domain, hwirq);
>>>>>>> refs/remotes/origin/master

	/* Look for default domain if nececssary */
	if (domain == NULL)
		domain = irq_default_domain;
	if (domain == NULL) {
<<<<<<< HEAD
		printk(KERN_WARNING "irq_create_mapping called for"
		       " NULL domain, hwirq=%lx\n", hwirq);
		WARN_ON(1);
		return 0;
	}
	pr_debug("irq: -> using domain @%p\n", domain);
=======
		WARN(1, "%s(, %lx) called with NULL domain\n", __func__, hwirq);
		return 0;
	}
	pr_debug("-> using domain @%p\n", domain);
>>>>>>> refs/remotes/origin/master

	/* Check if mapping already exists */
	virq = irq_find_mapping(domain, hwirq);
	if (virq) {
<<<<<<< HEAD
		pr_debug("irq: -> existing mapping on virq %d\n", virq);
		return virq;
	}

	/* Get a virtual interrupt number */
	if (domain->revmap_type == IRQ_DOMAIN_MAP_LEGACY)
		return irq_domain_legacy_revmap(domain, hwirq);

=======
		pr_debug("-> existing mapping on virq %d\n", virq);
		return virq;
	}

>>>>>>> refs/remotes/origin/master
	/* Allocate a virtual interrupt number */
	hint = hwirq % nr_irqs;
	if (hint == 0)
		hint++;
<<<<<<< HEAD
	virq = irq_alloc_desc_from(hint, 0);
	if (virq <= 0)
		virq = irq_alloc_desc_from(1, 0);
	if (virq <= 0) {
		pr_debug("irq: -> virq allocation failed\n");
		return 0;
	}

	if (irq_setup_virq(domain, virq, hwirq)) {
		if (domain->revmap_type != IRQ_DOMAIN_MAP_LEGACY)
			irq_free_desc(virq);
		return 0;
	}

	pr_debug("irq: irq %lu on domain %s mapped to virtual irq %u\n",
		hwirq, domain->of_node ? domain->of_node->full_name : "null", virq);
=======
	virq = irq_alloc_desc_from(hint, of_node_to_nid(domain->of_node));
	if (virq <= 0)
		virq = irq_alloc_desc_from(1, of_node_to_nid(domain->of_node));
	if (virq <= 0) {
		pr_debug("-> virq allocation failed\n");
		return 0;
	}

	if (irq_domain_associate(domain, virq, hwirq)) {
		irq_free_desc(virq);
		return 0;
	}

	pr_debug("irq %lu on domain %s mapped to virtual irq %u\n",
		hwirq, of_node_full_name(domain->of_node), virq);
>>>>>>> refs/remotes/origin/master

	return virq;
}
EXPORT_SYMBOL_GPL(irq_create_mapping);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
unsigned int irq_create_of_mapping(struct device_node *controller,
				   const u32 *intspec, unsigned int intsize)
{
	struct irq_domain *domain;
<<<<<<< HEAD
	unsigned long hwirq;
	unsigned int irq, type;
	int rc = -EINVAL;

	/* Find a domain which can translate the irq spec */
	mutex_lock(&irq_domain_mutex);
	list_for_each_entry(domain, &irq_domain_list, list) {
		if (!domain->ops->dt_translate)
			continue;
		rc = domain->ops->dt_translate(domain, controller,
					intspec, intsize, &hwirq, &type);
		if (rc == 0)
			break;
	}
	mutex_unlock(&irq_domain_mutex);

	if (rc != 0)
		return 0;

	irq = irq_domain_to_irq(domain, hwirq);
	if (type != IRQ_TYPE_NONE)
		irq_set_irq_type(irq, type);
	pr_debug("%s: mapped hwirq=%i to irq=%i, flags=%x\n",
		 controller->full_name, (int)hwirq, irq, type);
	return irq;
=======
=======
/**
 * irq_create_strict_mappings() - Map a range of hw irqs to fixed linux irqs
 * @domain: domain owning the interrupt range
 * @irq_base: beginning of linux IRQ range
 * @hwirq_base: beginning of hardware IRQ range
 * @count: Number of interrupts to map
 *
 * This routine is used for allocating and mapping a range of hardware
 * irqs to linux irqs where the linux irq numbers are at pre-defined
 * locations. For use by controllers that already have static mappings
 * to insert in to the domain.
 *
 * Non-linear users can use irq_create_identity_mapping() for IRQ-at-a-time
 * domain insertion.
 *
 * 0 is returned upon success, while any failure to establish a static
 * mapping is treated as an error.
 */
int irq_create_strict_mappings(struct irq_domain *domain, unsigned int irq_base,
			       irq_hw_number_t hwirq_base, int count)
{
	int ret;

	ret = irq_alloc_descs(irq_base, irq_base, count,
			      of_node_to_nid(domain->of_node));
	if (unlikely(ret < 0))
		return ret;

	irq_domain_associate_many(domain, irq_base, hwirq_base, count);
	return 0;
}
EXPORT_SYMBOL_GPL(irq_create_strict_mappings);

unsigned int irq_create_of_mapping(struct of_phandle_args *irq_data)
{
	struct irq_domain *domain;
>>>>>>> refs/remotes/origin/master
	irq_hw_number_t hwirq;
	unsigned int type = IRQ_TYPE_NONE;
	unsigned int virq;

<<<<<<< HEAD
	domain = controller ? irq_find_host(controller) : irq_default_domain;
	if (!domain) {
#ifdef CONFIG_MIPS
		/*
		 * Workaround to avoid breaking interrupt controller drivers
		 * that don't yet register an irq_domain.  This is temporary
		 * code. ~~~gcl, Feb 24, 2012
		 *
		 * Scheduled for removal in Linux v3.6.  That should be enough
		 * time.
		 */
		if (intsize > 0)
			return intspec[0];
#endif
		printk(KERN_WARNING "irq: no irq domain found for %s !\n",
		       controller->full_name);
=======
	domain = irq_data->np ? irq_find_host(irq_data->np) : irq_default_domain;
	if (!domain) {
		pr_warn("no irq domain found for %s !\n",
			of_node_full_name(irq_data->np));
>>>>>>> refs/remotes/origin/master
		return 0;
	}

	/* If domain has no translation, then we assume interrupt line */
	if (domain->ops->xlate == NULL)
<<<<<<< HEAD
		hwirq = intspec[0];
	else {
		if (domain->ops->xlate(domain, controller, intspec, intsize,
				     &hwirq, &type))
=======
		hwirq = irq_data->args[0];
	else {
		if (domain->ops->xlate(domain, irq_data->np, irq_data->args,
					irq_data->args_count, &hwirq, &type))
>>>>>>> refs/remotes/origin/master
			return 0;
	}

	/* Create mapping */
	virq = irq_create_mapping(domain, hwirq);
	if (!virq)
		return virq;

	/* Set type if specified and different than the current one */
	if (type != IRQ_TYPE_NONE &&
<<<<<<< HEAD
	    type != (irqd_get_trigger_type(irq_get_irq_data(virq))))
		irq_set_irq_type(virq, type);
	return virq;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	    type != irq_get_trigger_type(virq))
		irq_set_irq_type(virq, type);
	return virq;
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL_GPL(irq_create_of_mapping);

/**
<<<<<<< HEAD
<<<<<<< HEAD
 * irq_dispose_mapping() - Discard a mapping created by irq_create_of_mapping()
 * @irq: linux irq number to be discarded
 *
 * Calling this function indicates the caller no longer needs a reference to
 * the linux irq number returned by a prior call to irq_create_of_mapping().
 */
void irq_dispose_mapping(unsigned int irq)
{
	/*
	 * nothing yet; will be filled when support for dynamic allocation of
	 * irq_descs is added to irq_domain
	 */
}
EXPORT_SYMBOL_GPL(irq_dispose_mapping);

int irq_domain_simple_dt_translate(struct irq_domain *d,
			    struct device_node *controller,
			    const u32 *intspec, unsigned int intsize,
			    unsigned long *out_hwirq, unsigned int *out_type)
{
	if (d->of_node != controller)
		return -EINVAL;
	if (intsize < 1)
		return -EINVAL;

	*out_hwirq = intspec[0];
	*out_type = IRQ_TYPE_NONE;
	if (intsize > 1)
		*out_type = intspec[1] & IRQ_TYPE_SENSE_MASK;
	return 0;
}

struct irq_domain_ops irq_domain_simple_ops = {
	.dt_translate = irq_domain_simple_dt_translate,
};
EXPORT_SYMBOL_GPL(irq_domain_simple_ops);

/**
 * irq_domain_create_simple() - Set up a 'simple' translation range
 */
void irq_domain_add_simple(struct device_node *controller, int irq_base)
{
	struct irq_domain *domain;

	domain = kzalloc(sizeof(*domain), GFP_KERNEL);
	if (!domain) {
		WARN_ON(1);
		return;
	}

	domain->irq_base = irq_base;
	domain->of_node = of_node_get(controller);
	domain->ops = &irq_domain_simple_ops;
	irq_domain_add(domain);
}
EXPORT_SYMBOL_GPL(irq_domain_add_simple);

=======
=======
>>>>>>> refs/remotes/origin/master
 * irq_dispose_mapping() - Unmap an interrupt
 * @virq: linux irq number of the interrupt to unmap
 */
void irq_dispose_mapping(unsigned int virq)
{
	struct irq_data *irq_data = irq_get_irq_data(virq);
	struct irq_domain *domain;
<<<<<<< HEAD
	irq_hw_number_t hwirq;
=======
>>>>>>> refs/remotes/origin/master

	if (!virq || !irq_data)
		return;

	domain = irq_data->domain;
	if (WARN_ON(domain == NULL))
		return;

<<<<<<< HEAD
	/* Never unmap legacy interrupts */
	if (domain->revmap_type == IRQ_DOMAIN_MAP_LEGACY)
		return;

	irq_set_status_flags(virq, IRQ_NOREQUEST);

	/* remove chip and handler */
	irq_set_chip_and_handler(virq, NULL, NULL);

	/* Make sure it's completed */
	synchronize_irq(virq);

	/* Tell the PIC about it */
	if (domain->ops->unmap)
		domain->ops->unmap(domain, virq);
	smp_mb();

	/* Clear reverse map */
	hwirq = irq_data->hwirq;
	switch(domain->revmap_type) {
	case IRQ_DOMAIN_MAP_LINEAR:
		if (hwirq < domain->revmap_data.linear.size)
			domain->revmap_data.linear.revmap[hwirq] = 0;
		break;
	case IRQ_DOMAIN_MAP_TREE:
		mutex_lock(&revmap_trees_mutex);
		radix_tree_delete(&domain->revmap_data.tree, hwirq);
		mutex_unlock(&revmap_trees_mutex);
		break;
	}

=======
	irq_domain_disassociate(domain, virq);
>>>>>>> refs/remotes/origin/master
	irq_free_desc(virq);
}
EXPORT_SYMBOL_GPL(irq_dispose_mapping);

/**
 * irq_find_mapping() - Find a linux irq from an hw irq number.
 * @domain: domain owning this hardware interrupt
 * @hwirq: hardware irq number in that domain space
<<<<<<< HEAD
 *
 * This is a slow path, for use by generic code. It's expected that an
 * irq controller implementation directly calls the appropriate low level
 * mapping function.
=======
>>>>>>> refs/remotes/origin/master
 */
unsigned int irq_find_mapping(struct irq_domain *domain,
			      irq_hw_number_t hwirq)
{
<<<<<<< HEAD
	unsigned int i;
	unsigned int hint = hwirq % nr_irqs;
=======
	struct irq_data *data;
>>>>>>> refs/remotes/origin/master

	/* Look for default domain if nececssary */
	if (domain == NULL)
		domain = irq_default_domain;
	if (domain == NULL)
		return 0;

<<<<<<< HEAD
	/* legacy -> bail early */
	if (domain->revmap_type == IRQ_DOMAIN_MAP_LEGACY)
		return irq_domain_legacy_revmap(domain, hwirq);

	/* Slow path does a linear search of the map */
	if (hint == 0)
		hint = 1;
	i = hint;
	do {
		struct irq_data *data = irq_get_irq_data(i);
		if (data && (data->domain == domain) && (data->hwirq == hwirq))
			return i;
		i++;
		if (i >= nr_irqs)
			i = 1;
	} while(i != hint);
	return 0;
}
EXPORT_SYMBOL_GPL(irq_find_mapping);

/**
 * irq_radix_revmap_lookup() - Find a linux irq from a hw irq number.
 * @domain: domain owning this hardware interrupt
 * @hwirq: hardware irq number in that domain space
 *
 * This is a fast path, for use by irq controller code that uses radix tree
 * revmaps
 */
unsigned int irq_radix_revmap_lookup(struct irq_domain *domain,
				     irq_hw_number_t hwirq)
{
	struct irq_data *irq_data;

	if (WARN_ON_ONCE(domain->revmap_type != IRQ_DOMAIN_MAP_TREE))
		return irq_find_mapping(domain, hwirq);

	/*
	 * Freeing an irq can delete nodes along the path to
	 * do the lookup via call_rcu.
	 */
	rcu_read_lock();
	irq_data = radix_tree_lookup(&domain->revmap_data.tree, hwirq);
	rcu_read_unlock();

	/*
	 * If found in radix tree, then fine.
	 * Else fallback to linear lookup - this should not happen in practice
	 * as it means that we failed to insert the node in the radix tree.
	 */
	return irq_data ? irq_data->irq : irq_find_mapping(domain, hwirq);
}

/**
 * irq_radix_revmap_insert() - Insert a hw irq to linux irq number mapping.
 * @domain: domain owning this hardware interrupt
 * @virq: linux irq number
 * @hwirq: hardware irq number in that domain space
 *
 * This is for use by irq controllers that use a radix tree reverse
 * mapping for fast lookup.
 */
void irq_radix_revmap_insert(struct irq_domain *domain, unsigned int virq,
			     irq_hw_number_t hwirq)
{
	struct irq_data *irq_data = irq_get_irq_data(virq);

	if (WARN_ON(domain->revmap_type != IRQ_DOMAIN_MAP_TREE))
		return;

	if (virq) {
		mutex_lock(&revmap_trees_mutex);
		radix_tree_insert(&domain->revmap_data.tree, hwirq, irq_data);
		mutex_unlock(&revmap_trees_mutex);
	}
}

/**
 * irq_linear_revmap() - Find a linux irq from a hw irq number.
 * @domain: domain owning this hardware interrupt
 * @hwirq: hardware irq number in that domain space
 *
 * This is a fast path, for use by irq controller code that uses linear
 * revmaps. It does fallback to the slow path if the revmap doesn't exist
 * yet and will create the revmap entry with appropriate locking
 */
unsigned int irq_linear_revmap(struct irq_domain *domain,
			       irq_hw_number_t hwirq)
{
	unsigned int *revmap;

	if (WARN_ON_ONCE(domain->revmap_type != IRQ_DOMAIN_MAP_LINEAR))
		return irq_find_mapping(domain, hwirq);

	/* Check revmap bounds */
	if (unlikely(hwirq >= domain->revmap_data.linear.size))
		return irq_find_mapping(domain, hwirq);

	/* Check if revmap was allocated */
	revmap = domain->revmap_data.linear.revmap;
	if (unlikely(revmap == NULL))
		return irq_find_mapping(domain, hwirq);

	/* Fill up revmap with slow path if no mapping found */
	if (unlikely(!revmap[hwirq]))
		revmap[hwirq] = irq_find_mapping(domain, hwirq);

	return revmap[hwirq];
}
=======
	if (hwirq < domain->revmap_direct_max_irq) {
		data = irq_get_irq_data(hwirq);
		if (data && (data->domain == domain) && (data->hwirq == hwirq))
			return hwirq;
	}

	/* Check if the hwirq is in the linear revmap. */
	if (hwirq < domain->revmap_size)
		return domain->linear_revmap[hwirq];

	rcu_read_lock();
	data = radix_tree_lookup(&domain->revmap_tree, hwirq);
	rcu_read_unlock();
	return data ? data->irq : 0;
}
EXPORT_SYMBOL_GPL(irq_find_mapping);
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_IRQ_DOMAIN_DEBUG
static int virq_debug_show(struct seq_file *m, void *private)
{
	unsigned long flags;
	struct irq_desc *desc;
<<<<<<< HEAD
	const char *p;
	static const char none[] = "none";
	void *data;
	int i;

	seq_printf(m, "%-5s  %-7s  %-15s  %-*s  %s\n", "irq", "hwirq",
		      "chip name", (int)(2 * sizeof(void *) + 2), "chip data",
		      "domain name");
=======
	struct irq_domain *domain;
	struct radix_tree_iter iter;
	void *data, **slot;
	int i;

	seq_printf(m, " %-16s  %-6s  %-10s  %-10s  %s\n",
		   "name", "mapped", "linear-max", "direct-max", "devtree-node");
	mutex_lock(&irq_domain_mutex);
	list_for_each_entry(domain, &irq_domain_list, link) {
		int count = 0;
		radix_tree_for_each_slot(slot, &domain->revmap_tree, &iter, 0)
			count++;
		seq_printf(m, "%c%-16s  %6u  %10u  %10u  %s\n",
			   domain == irq_default_domain ? '*' : ' ', domain->name,
			   domain->revmap_size + count, domain->revmap_size,
			   domain->revmap_direct_max_irq,
			   domain->of_node ? of_node_full_name(domain->of_node) : "");
	}
	mutex_unlock(&irq_domain_mutex);

	seq_printf(m, "%-5s  %-7s  %-15s  %-*s  %6s  %-14s  %s\n", "irq", "hwirq",
		      "chip name", (int)(2 * sizeof(void *) + 2), "chip data",
		      "active", "type", "domain");
>>>>>>> refs/remotes/origin/master

	for (i = 1; i < nr_irqs; i++) {
		desc = irq_to_desc(i);
		if (!desc)
			continue;

		raw_spin_lock_irqsave(&desc->lock, flags);
<<<<<<< HEAD

		if (desc->action && desc->action->handler) {
			struct irq_chip *chip;

			seq_printf(m, "%5d  ", i);
			seq_printf(m, "0x%05lx  ", desc->irq_data.hwirq);

			chip = irq_desc_get_chip(desc);
			if (chip && chip->name)
				p = chip->name;
			else
				p = none;
			seq_printf(m, "%-15s  ", p);
=======
		domain = desc->irq_data.domain;

		if (domain) {
			struct irq_chip *chip;
			int hwirq = desc->irq_data.hwirq;
			bool direct;

			seq_printf(m, "%5d  ", i);
			seq_printf(m, "0x%05x  ", hwirq);

			chip = irq_desc_get_chip(desc);
			seq_printf(m, "%-15s  ", (chip && chip->name) ? chip->name : "none");
>>>>>>> refs/remotes/origin/master

			data = irq_desc_get_chip_data(desc);
			seq_printf(m, data ? "0x%p  " : "  %p  ", data);

<<<<<<< HEAD
			if (desc->irq_data.domain && desc->irq_data.domain->of_node)
				p = desc->irq_data.domain->of_node->full_name;
			else
				p = none;
			seq_printf(m, "%s\n", p);
=======
			seq_printf(m, "   %c    ", (desc->action && desc->action->handler) ? '*' : ' ');
			direct = (i == hwirq) && (i < domain->revmap_direct_max_irq);
			seq_printf(m, "%6s%-8s  ",
				   (hwirq < domain->revmap_size) ? "LINEAR" : "RADIX",
				   direct ? "(DIRECT)" : "");
			seq_printf(m, "%s\n", desc->irq_data.domain->name);
>>>>>>> refs/remotes/origin/master
		}

		raw_spin_unlock_irqrestore(&desc->lock, flags);
	}

	return 0;
}

static int virq_debug_open(struct inode *inode, struct file *file)
{
	return single_open(file, virq_debug_show, inode->i_private);
}

static const struct file_operations virq_debug_fops = {
	.open = virq_debug_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

static int __init irq_debugfs_init(void)
{
	if (debugfs_create_file("irq_domain_mapping", S_IRUGO, NULL,
				 NULL, &virq_debug_fops) == NULL)
		return -ENOMEM;

	return 0;
}
__initcall(irq_debugfs_init);
#endif /* CONFIG_IRQ_DOMAIN_DEBUG */

<<<<<<< HEAD
int irq_domain_simple_map(struct irq_domain *d, unsigned int irq,
			  irq_hw_number_t hwirq)
{
	return 0;
}

=======
>>>>>>> refs/remotes/origin/master
/**
 * irq_domain_xlate_onecell() - Generic xlate for direct one cell bindings
 *
 * Device Tree IRQ specifier translation function which works with one cell
 * bindings where the cell value maps directly to the hwirq number.
 */
int irq_domain_xlate_onecell(struct irq_domain *d, struct device_node *ctrlr,
			     const u32 *intspec, unsigned int intsize,
			     unsigned long *out_hwirq, unsigned int *out_type)
{
	if (WARN_ON(intsize < 1))
		return -EINVAL;
	*out_hwirq = intspec[0];
	*out_type = IRQ_TYPE_NONE;
	return 0;
}
EXPORT_SYMBOL_GPL(irq_domain_xlate_onecell);

/**
 * irq_domain_xlate_twocell() - Generic xlate for direct two cell bindings
 *
 * Device Tree IRQ specifier translation function which works with two cell
 * bindings where the cell values map directly to the hwirq number
 * and linux irq flags.
 */
int irq_domain_xlate_twocell(struct irq_domain *d, struct device_node *ctrlr,
			const u32 *intspec, unsigned int intsize,
			irq_hw_number_t *out_hwirq, unsigned int *out_type)
{
	if (WARN_ON(intsize < 2))
		return -EINVAL;
	*out_hwirq = intspec[0];
	*out_type = intspec[1] & IRQ_TYPE_SENSE_MASK;
	return 0;
}
EXPORT_SYMBOL_GPL(irq_domain_xlate_twocell);

/**
 * irq_domain_xlate_onetwocell() - Generic xlate for one or two cell bindings
 *
 * Device Tree IRQ specifier translation function which works with either one
 * or two cell bindings where the cell values map directly to the hwirq number
 * and linux irq flags.
 *
 * Note: don't use this function unless your interrupt controller explicitly
 * supports both one and two cell bindings.  For the majority of controllers
 * the _onecell() or _twocell() variants above should be used.
 */
int irq_domain_xlate_onetwocell(struct irq_domain *d,
				struct device_node *ctrlr,
				const u32 *intspec, unsigned int intsize,
				unsigned long *out_hwirq, unsigned int *out_type)
{
	if (WARN_ON(intsize < 1))
		return -EINVAL;
	*out_hwirq = intspec[0];
	*out_type = (intsize > 1) ? intspec[1] : IRQ_TYPE_NONE;
	return 0;
}
EXPORT_SYMBOL_GPL(irq_domain_xlate_onetwocell);

const struct irq_domain_ops irq_domain_simple_ops = {
<<<<<<< HEAD
	.map = irq_domain_simple_map,
	.xlate = irq_domain_xlate_onetwocell,
};
EXPORT_SYMBOL_GPL(irq_domain_simple_ops);

#ifdef CONFIG_OF_IRQ
>>>>>>> refs/remotes/origin/cm-10.0
void irq_domain_generate_simple(const struct of_device_id *match,
				u64 phys_base, unsigned int irq_start)
{
	struct device_node *node;
<<<<<<< HEAD
	pr_info("looking for phys_base=%llx, irq_start=%i\n",
		(unsigned long long) phys_base, (int) irq_start);
	node = of_find_matching_node_by_address(NULL, match, phys_base);
	if (node)
		irq_domain_add_simple(node, irq_start);
	else
		pr_info("no node found\n");
}
EXPORT_SYMBOL_GPL(irq_domain_generate_simple);
#endif /* CONFIG_OF_IRQ */
=======
	pr_debug("looking for phys_base=%llx, irq_start=%i\n",
		(unsigned long long) phys_base, (int) irq_start);
	node = of_find_matching_node_by_address(NULL, match, phys_base);
	if (node)
		irq_domain_add_legacy(node, 32, irq_start, 0,
				      &irq_domain_simple_ops, NULL);
}
EXPORT_SYMBOL_GPL(irq_domain_generate_simple);
#endif
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.xlate = irq_domain_xlate_onetwocell,
};
EXPORT_SYMBOL_GPL(irq_domain_simple_ops);
>>>>>>> refs/remotes/origin/master
