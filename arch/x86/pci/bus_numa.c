#include <linux/init.h>
#include <linux/pci.h>
#include <linux/range.h>

#include "bus_numa.h"

<<<<<<< HEAD
int pci_root_num;
struct pci_root_info pci_root_info[PCI_ROOT_NR];

<<<<<<< HEAD
void x86_pci_root_bus_res_quirks(struct pci_bus *b)
=======
void x86_pci_root_bus_resources(int bus, struct list_head *resources)
>>>>>>> refs/remotes/origin/cm-10.0
{
	int i;
	int j;
	struct pci_root_info *info;

<<<<<<< HEAD
	/* don't go for it if _CRS is used already */
	if (b->resource[0] != &ioport_resource ||
	    b->resource[1] != &iomem_resource)
		return;

	if (!pci_root_num)
		return;

	for (i = 0; i < pci_root_num; i++) {
		if (pci_root_info[i].bus_min == b->number)
=======
	if (!pci_root_num)
		goto default_resources;

	for (i = 0; i < pci_root_num; i++) {
		if (pci_root_info[i].bus_min == bus)
>>>>>>> refs/remotes/origin/cm-10.0
			break;
	}

	if (i == pci_root_num)
<<<<<<< HEAD
		return;

	printk(KERN_DEBUG "PCI: peer root bus %02x res updated from pci conf\n",
			b->number);

	pci_bus_remove_resources(b);
=======
=======
LIST_HEAD(pci_root_infos);

static struct pci_root_info *x86_find_pci_root_info(int bus)
{
	struct pci_root_info *info;

	if (list_empty(&pci_root_infos))
		return NULL;

	list_for_each_entry(info, &pci_root_infos, list)
		if (info->busn.start == bus)
			return info;

	return NULL;
}

void x86_pci_root_bus_resources(int bus, struct list_head *resources)
{
	struct pci_root_info *info = x86_find_pci_root_info(bus);
	struct pci_root_res *root_res;
	struct pci_host_bridge_window *window;
	bool found = false;

	if (!info)
>>>>>>> refs/remotes/origin/master
		goto default_resources;

	printk(KERN_DEBUG "PCI: root bus %02x: hardware-probed resources\n",
	       bus);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	info = &pci_root_info[i];
	for (j = 0; j < info->res_num; j++) {
		struct resource *res;
		struct resource *root;

		res = &info->res[j];
<<<<<<< HEAD
		pci_bus_add_resource(b, res, 0);
=======
		pci_add_resource(resources, res);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* already added by acpi ? */
	list_for_each_entry(window, resources, list)
		if (window->res->flags & IORESOURCE_BUS) {
			found = true;
			break;
		}

	if (!found)
		pci_add_resource(resources, &info->busn);

	list_for_each_entry(root_res, &info->resources, list) {
		struct resource *res;
		struct resource *root;

		res = &root_res->res;
		pci_add_resource(resources, res);
>>>>>>> refs/remotes/origin/master
		if (res->flags & IORESOURCE_IO)
			root = &ioport_resource;
		else
			root = &iomem_resource;
		insert_resource(root, res);
	}
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	return;

default_resources:
	/*
	 * We don't have any host bridge aperture information from the
	 * "native host bridge drivers," e.g., amd_bus or broadcom_bus,
	 * so fall back to the defaults historically used by pci_create_bus().
	 */
	printk(KERN_DEBUG "PCI: root bus %02x: using default resources\n", bus);
	pci_add_resource(resources, &ioport_resource);
	pci_add_resource(resources, &iomem_resource);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
}

void __devinit update_res(struct pci_root_info *info, resource_size_t start,
			  resource_size_t end, unsigned long flags, int merge)
{
	int i;
	struct resource *res;
=======
}

struct pci_root_info __init *alloc_pci_root_info(int bus_min, int bus_max,
						 int node, int link)
{
	struct pci_root_info *info;

	info = kzalloc(sizeof(*info), GFP_KERNEL);

	if (!info)
		return info;

	sprintf(info->name, "PCI Bus #%02x", bus_min);

	INIT_LIST_HEAD(&info->resources);
	info->busn.name  = info->name;
	info->busn.start = bus_min;
	info->busn.end   = bus_max;
	info->busn.flags = IORESOURCE_BUS;
	info->node = node;
	info->link = link;

	list_add_tail(&info->list, &pci_root_infos);

	return info;
}

void update_res(struct pci_root_info *info, resource_size_t start,
		resource_size_t end, unsigned long flags, int merge)
{
	struct resource *res;
	struct pci_root_res *root_res;
>>>>>>> refs/remotes/origin/master

	if (start > end)
		return;

	if (start == MAX_RESOURCE)
		return;

	if (!merge)
		goto addit;

	/* try to merge it with old one */
<<<<<<< HEAD
	for (i = 0; i < info->res_num; i++) {
		resource_size_t final_start, final_end;
		resource_size_t common_start, common_end;

		res = &info->res[i];
=======
	list_for_each_entry(root_res, &info->resources, list) {
		resource_size_t final_start, final_end;
		resource_size_t common_start, common_end;

		res = &root_res->res;
>>>>>>> refs/remotes/origin/master
		if (res->flags != flags)
			continue;

		common_start = max(res->start, start);
		common_end = min(res->end, end);
		if (common_start > common_end + 1)
			continue;

		final_start = min(res->start, start);
		final_end = max(res->end, end);

		res->start = final_start;
		res->end = final_end;
		return;
	}

addit:

	/* need to add that */
<<<<<<< HEAD
	if (info->res_num >= RES_NUM)
		return;

	res = &info->res[info->res_num];
=======
	root_res = kzalloc(sizeof(*root_res), GFP_KERNEL);
	if (!root_res)
		return;

	res = &root_res->res;
>>>>>>> refs/remotes/origin/master
	res->name = info->name;
	res->flags = flags;
	res->start = start;
	res->end = end;
<<<<<<< HEAD
	res->child = NULL;
	info->res_num++;
=======

	list_add_tail(&root_res->list, &info->resources);
>>>>>>> refs/remotes/origin/master
}
