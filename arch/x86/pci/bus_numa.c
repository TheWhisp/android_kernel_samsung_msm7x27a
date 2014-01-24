#include <linux/init.h>
#include <linux/pci.h>
#include <linux/range.h>

#include "bus_numa.h"

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
		goto default_resources;

	printk(KERN_DEBUG "PCI: root bus %02x: hardware-probed resources\n",
	       bus);

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
		if (res->flags & IORESOURCE_IO)
			root = &ioport_resource;
		else
			root = &iomem_resource;
		insert_resource(root, res);
	}
<<<<<<< HEAD
=======
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
>>>>>>> refs/remotes/origin/cm-10.0
}

void __devinit update_res(struct pci_root_info *info, resource_size_t start,
			  resource_size_t end, unsigned long flags, int merge)
{
	int i;
	struct resource *res;

	if (start > end)
		return;

	if (start == MAX_RESOURCE)
		return;

	if (!merge)
		goto addit;

	/* try to merge it with old one */
	for (i = 0; i < info->res_num; i++) {
		resource_size_t final_start, final_end;
		resource_size_t common_start, common_end;

		res = &info->res[i];
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
	if (info->res_num >= RES_NUM)
		return;

	res = &info->res[info->res_num];
	res->name = info->name;
	res->flags = flags;
	res->start = start;
	res->end = end;
	res->child = NULL;
	info->res_num++;
}
