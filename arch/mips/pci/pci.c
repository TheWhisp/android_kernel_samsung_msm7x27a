/*
<<<<<<< HEAD
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
<<<<<<< HEAD
 * Copyright (C) 2003, 04 Ralf Baechle (ralf@linux-mips.org)
 */
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/bootmem.h>
=======
=======
 * This program is free software; you can redistribute	it and/or modify it
 * under  the terms of	the GNU General	 Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
>>>>>>> refs/remotes/origin/master
 * Copyright (C) 2003, 04, 11 Ralf Baechle (ralf@linux-mips.org)
 * Copyright (C) 2011 Wind River Systems,
 *   written by Ralf Baechle (ralf@linux-mips.org)
 */
#include <linux/bug.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/bootmem.h>
#include <linux/export.h>
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/init.h>
#include <linux/types.h>
#include <linux/pci.h>

<<<<<<< HEAD
/*
 * Indicate whether we respect the PCI setup left by the firmware.
 *
 * Make this long-lived  so that we know when shutting down
 * whether we probed only or not.
 */
int pci_probe_only;

#define PCI_ASSIGN_ALL_BUSSES	1

unsigned int pci_probe = PCI_ASSIGN_ALL_BUSSES;
=======
=======
#include <linux/init.h>
#include <linux/types.h>
#include <linux/pci.h>
#include <linux/of_address.h>

>>>>>>> refs/remotes/origin/master
#include <asm/cpu-info.h>

/*
 * If PCI_PROBE_ONLY in pci_flags is set, we don't change any PCI resource
 * assignments.
 */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

/*
 * The PCI controller list.
 */

static struct pci_controller *hose_head, **hose_tail = &hose_head;

unsigned long PCIBIOS_MIN_IO;
unsigned long PCIBIOS_MIN_MEM;

static int pci_initialized;

/*
 * We need to avoid collisions with `mirrored' VGA ports
 * and other strange ISA hardware, so we always want the
 * addresses to be allocated in the 0x000-0x0ff region
 * modulo 0x400.
 *
 * Why? Because some silly external IO cards only decode
 * the low 10 bits of the IO address. The 0x00-0xff region
 * is reserved for motherboard devices that decode all 16
 * bits, so it's ok to allocate at, say, 0x2800-0x28ff,
 * but we want to try to avoid allocating at 0x2900-0x2bff
 * which might have be mirrored at 0x0100-0x03ff..
 */
resource_size_t
pcibios_align_resource(void *data, const struct resource *res,
		       resource_size_t size, resource_size_t align)
{
	struct pci_dev *dev = data;
	struct pci_controller *hose = dev->sysdata;
	resource_size_t start = res->start;

	if (res->flags & IORESOURCE_IO) {
		/* Make sure we start at our min on all hoses */
		if (start < PCIBIOS_MIN_IO + hose->io_resource->start)
			start = PCIBIOS_MIN_IO + hose->io_resource->start;

		/*
		 * Put everything into 0x00-0xff region modulo 0x400
		 */
		if (start & 0x300)
			start = (start + 0x3ff) & ~0x3ff;
	} else if (res->flags & IORESOURCE_MEM) {
		/* Make sure we start at our min on all hoses */
		if (start < PCIBIOS_MIN_MEM + hose->mem_resource->start)
			start = PCIBIOS_MIN_MEM + hose->mem_resource->start;
	}

	return start;
}

<<<<<<< HEAD
static void __devinit pcibios_scanbus(struct pci_controller *hose)
{
	static int next_busno;
	static int need_domain_info;
<<<<<<< HEAD
=======
	LIST_HEAD(resources);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void pcibios_scanbus(struct pci_controller *hose)
{
	static int next_busno;
	static int need_domain_info;
	LIST_HEAD(resources);
>>>>>>> refs/remotes/origin/master
	struct pci_bus *bus;

	if (!hose->iommu)
		PCI_DMA_BUS_IS_PHYS = 1;

<<<<<<< HEAD
<<<<<<< HEAD
	if (hose->get_busno && pci_probe_only)
		next_busno = (*hose->get_busno)();

	bus = pci_scan_bus(next_busno, hose->pci_ops, hose);
=======
=======
>>>>>>> refs/remotes/origin/master
	if (hose->get_busno && pci_has_flag(PCI_PROBE_ONLY))
		next_busno = (*hose->get_busno)();

	pci_add_resource_offset(&resources,
				hose->mem_resource, hose->mem_offset);
	pci_add_resource_offset(&resources, hose->io_resource, hose->io_offset);
	bus = pci_scan_root_bus(NULL, next_busno, hose->pci_ops, hose,
				&resources);
	if (!bus)
		pci_free_resource_list(&resources);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	hose->bus = bus;

	need_domain_info = need_domain_info || hose->index;
	hose->need_domain_info = need_domain_info;
	if (bus) {
<<<<<<< HEAD
		next_busno = bus->subordinate + 1;
=======
		next_busno = bus->busn_res.end + 1;
>>>>>>> refs/remotes/origin/master
		/* Don't allow 8-bit bus number overflow inside the hose -
		   reserve some space for bridges. */
		if (next_busno > 224) {
			next_busno = 0;
			need_domain_info = 1;
		}

<<<<<<< HEAD
<<<<<<< HEAD
		if (!pci_probe_only) {
=======
		if (!pci_has_flag(PCI_PROBE_ONLY)) {
>>>>>>> refs/remotes/origin/cm-10.0
			pci_bus_size_bridges(bus);
			pci_bus_assign_resources(bus);
			pci_enable_bridges(bus);
=======
		if (!pci_has_flag(PCI_PROBE_ONLY)) {
			pci_bus_size_bridges(bus);
			pci_bus_assign_resources(bus);
>>>>>>> refs/remotes/origin/master
		}
	}
}

<<<<<<< HEAD
static DEFINE_MUTEX(pci_scan_mutex);

void __devinit register_pci_controller(struct pci_controller *hose)
{
	if (request_resource(&iomem_resource, hose->mem_resource) < 0)
		goto out;
	if (request_resource(&ioport_resource, hose->io_resource) < 0) {
=======
#ifdef CONFIG_OF
void pci_load_of_ranges(struct pci_controller *hose, struct device_node *node)
{
	struct of_pci_range range;
	struct of_pci_range_parser parser;

	pr_info("PCI host bridge %s ranges:\n", node->full_name);
	hose->of_node = node;

	if (of_pci_range_parser_init(&parser, node))
		return;

	for_each_of_pci_range(&parser, &range) {
		struct resource *res = NULL;

		switch (range.flags & IORESOURCE_TYPE_BITS) {
		case IORESOURCE_IO:
			pr_info("  IO 0x%016llx..0x%016llx\n",
				range.cpu_addr,
				range.cpu_addr + range.size - 1);
			hose->io_map_base =
				(unsigned long)ioremap(range.cpu_addr,
						       range.size);
			res = hose->io_resource;
			break;
		case IORESOURCE_MEM:
			pr_info(" MEM 0x%016llx..0x%016llx\n",
				range.cpu_addr,
				range.cpu_addr + range.size - 1);
			res = hose->mem_resource;
			break;
		}
		if (res != NULL)
			of_pci_range_to_resource(&range, node, res);
	}
}

struct device_node *pcibios_get_phb_of_node(struct pci_bus *bus)
{
	struct pci_controller *hose = bus->sysdata;

	return of_node_get(hose->of_node);
}
#endif

static DEFINE_MUTEX(pci_scan_mutex);

void register_pci_controller(struct pci_controller *hose)
{
	struct resource *parent;

	parent = hose->mem_resource->parent;
	if (!parent)
		parent = &iomem_resource;

	if (request_resource(parent, hose->mem_resource) < 0)
		goto out;

	parent = hose->io_resource->parent;
	if (!parent)
		parent = &ioport_resource;

	if (request_resource(parent, hose->io_resource) < 0) {
>>>>>>> refs/remotes/origin/master
		release_resource(hose->mem_resource);
		goto out;
	}

	*hose_tail = hose;
	hose_tail = &hose->next;

	/*
	 * Do not panic here but later - this might happen before console init.
	 */
	if (!hose->io_map_base) {
		printk(KERN_WARNING
		       "registering PCI controller with io_map_base unset\n");
	}

	/*
	 * Scan the bus if it is register after the PCI subsystem
	 * initialization.
	 */
	if (pci_initialized) {
		mutex_lock(&pci_scan_mutex);
		pcibios_scanbus(hose);
		mutex_unlock(&pci_scan_mutex);
	}

	return;

out:
	printk(KERN_WARNING
	       "Skipping PCI bus scan due to resource conflict\n");
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static void __init pcibios_set_cache_line_size(void)
{
	struct cpuinfo_mips *c = &current_cpu_data;
	unsigned int lsize;

	/*
	 * Set PCI cacheline size to that of the highest level in the
	 * cache hierarchy.
	 */
	lsize = c->dcache.linesz;
	lsize = c->scache.linesz ? : lsize;
	lsize = c->tcache.linesz ? : lsize;

	BUG_ON(!lsize);

	pci_dfl_cache_line_size = lsize >> 2;

	pr_debug("PCI: pci_cache_line_size set to %d bytes\n", lsize);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static int __init pcibios_init(void)
{
	struct pci_controller *hose;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	pcibios_set_cache_line_size();

>>>>>>> refs/remotes/origin/cm-10.0
=======
	pcibios_set_cache_line_size();

>>>>>>> refs/remotes/origin/master
	/* Scan all of the recorded PCI controllers.  */
	for (hose = hose_head; hose; hose = hose->next)
		pcibios_scanbus(hose);

	pci_fixup_irqs(pci_common_swizzle, pcibios_map_irq);

	pci_initialized = 1;

	return 0;
}

subsys_initcall(pcibios_init);

static int pcibios_enable_resources(struct pci_dev *dev, int mask)
{
	u16 cmd, old_cmd;
	int idx;
	struct resource *r;

	pci_read_config_word(dev, PCI_COMMAND, &cmd);
	old_cmd = cmd;
	for (idx=0; idx < PCI_NUM_RESOURCES; idx++) {
		/* Only set up the requested stuff */
		if (!(mask & (1<<idx)))
			continue;

		r = &dev->resource[idx];
		if (!(r->flags & (IORESOURCE_IO | IORESOURCE_MEM)))
			continue;
		if ((idx == PCI_ROM_RESOURCE) &&
				(!(r->flags & IORESOURCE_ROM_ENABLE)))
			continue;
		if (!r->start && r->end) {
			printk(KERN_ERR "PCI: Device %s not available "
			       "because of resource collisions\n",
			       pci_name(dev));
			return -EINVAL;
		}
		if (r->flags & IORESOURCE_IO)
			cmd |= PCI_COMMAND_IO;
		if (r->flags & IORESOURCE_MEM)
			cmd |= PCI_COMMAND_MEMORY;
	}
	if (cmd != old_cmd) {
		printk("PCI: Enabling device %s (%04x -> %04x)\n",
		       pci_name(dev), old_cmd, cmd);
		pci_write_config_word(dev, PCI_COMMAND, cmd);
	}
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
/*
 *  If we set up a device for bus mastering, we need to check the latency
 *  timer as certain crappy BIOSes forget to set it properly.
 */
static unsigned int pcibios_max_latency = 255;

void pcibios_set_master(struct pci_dev *dev)
{
	u8 lat;
	pci_read_config_byte(dev, PCI_LATENCY_TIMER, &lat);
	if (lat < 16)
		lat = (64 <= pcibios_max_latency) ? 64 : pcibios_max_latency;
	else if (lat > pcibios_max_latency)
		lat = pcibios_max_latency;
	else
		return;
	printk(KERN_DEBUG "PCI: Setting latency timer of device %s to %d\n",
	       pci_name(dev), lat);
	pci_write_config_byte(dev, PCI_LATENCY_TIMER, lat);
}

unsigned int pcibios_assign_all_busses(void)
{
	return (pci_probe & PCI_ASSIGN_ALL_BUSSES) ? 1 : 0;
=======
unsigned int pcibios_assign_all_busses(void)
{
	return 1;
>>>>>>> refs/remotes/origin/cm-10.0
=======
unsigned int pcibios_assign_all_busses(void)
{
	return 1;
>>>>>>> refs/remotes/origin/master
}

int pcibios_enable_device(struct pci_dev *dev, int mask)
{
	int err;

	if ((err = pcibios_enable_resources(dev, mask)) < 0)
		return err;

	return pcibios_plat_dev_init(dev);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void pcibios_fixup_device_resources(struct pci_dev *dev,
	struct pci_bus *bus)
{
	/* Update device resources.  */
	struct pci_controller *hose = (struct pci_controller *)bus->sysdata;
	unsigned long offset = 0;
	int i;

	for (i = 0; i < PCI_NUM_RESOURCES; i++) {
		if (!dev->resource[i].start)
			continue;
		if (dev->resource[i].flags & IORESOURCE_IO)
			offset = hose->io_offset;
		else if (dev->resource[i].flags & IORESOURCE_MEM)
			offset = hose->mem_offset;

		dev->resource[i].start += offset;
		dev->resource[i].end += offset;
	}
}

void __devinit pcibios_fixup_bus(struct pci_bus *bus)
{
	/* Propagate hose info into the subordinate devices.  */

	struct pci_controller *hose = bus->sysdata;
	struct list_head *ln;
	struct pci_dev *dev = bus->self;

	if (!dev) {
		bus->resource[0] = hose->io_resource;
		bus->resource[1] = hose->mem_resource;
	} else if (pci_probe_only &&
		   (dev->class >> 8) == PCI_CLASS_BRIDGE_PCI) {
		pci_read_bridge_bases(bus);
		pcibios_fixup_device_resources(dev, bus);
	}

	for (ln = bus->devices.next; ln != &bus->devices; ln = ln->next) {
		dev = pci_dev_b(ln);

		if ((dev->class >> 8) != PCI_CLASS_BRIDGE_PCI)
			pcibios_fixup_device_resources(dev, bus);
=======
void __devinit pcibios_fixup_bus(struct pci_bus *bus)
=======
void pcibios_fixup_bus(struct pci_bus *bus)
>>>>>>> refs/remotes/origin/master
{
	struct pci_dev *dev = bus->self;

	if (pci_has_flag(PCI_PROBE_ONLY) && dev &&
	    (dev->class >> 8) == PCI_CLASS_BRIDGE_PCI) {
		pci_read_bridge_bases(bus);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	}
}

void __init
pcibios_update_irq(struct pci_dev *dev, int irq)
{
	pci_write_config_byte(dev, PCI_INTERRUPT_LINE, irq);
}

<<<<<<< HEAD
void pcibios_resource_to_bus(struct pci_dev *dev, struct pci_bus_region *region,
			 struct resource *res)
{
	struct pci_controller *hose = (struct pci_controller *)dev->sysdata;
	unsigned long offset = 0;

	if (res->flags & IORESOURCE_IO)
		offset = hose->io_offset;
	else if (res->flags & IORESOURCE_MEM)
		offset = hose->mem_offset;

	region->start = res->start - offset;
	region->end = res->end - offset;
}

void __devinit
pcibios_bus_to_resource(struct pci_dev *dev, struct resource *res,
			struct pci_bus_region *region)
{
	struct pci_controller *hose = (struct pci_controller *)dev->sysdata;
	unsigned long offset = 0;

	if (res->flags & IORESOURCE_IO)
		offset = hose->io_offset;
	else if (res->flags & IORESOURCE_MEM)
		offset = hose->mem_offset;

	res->start = region->start + offset;
	res->end = region->end + offset;
}

#ifdef CONFIG_HOTPLUG
EXPORT_SYMBOL(pcibios_resource_to_bus);
EXPORT_SYMBOL(pcibios_bus_to_resource);
=======
#ifdef CONFIG_HOTPLUG
>>>>>>> refs/remotes/origin/cm-10.0
EXPORT_SYMBOL(PCIBIOS_MIN_IO);
EXPORT_SYMBOL(PCIBIOS_MIN_MEM);
#endif
=======
	}
}

EXPORT_SYMBOL(PCIBIOS_MIN_IO);
EXPORT_SYMBOL(PCIBIOS_MIN_MEM);
>>>>>>> refs/remotes/origin/master

int pci_mmap_page_range(struct pci_dev *dev, struct vm_area_struct *vma,
			enum pci_mmap_state mmap_state, int write_combine)
{
	unsigned long prot;

	/*
	 * I/O space can be accessed via normal processor loads and stores on
	 * this platform but for now we elect not to do this and portable
	 * drivers should not do this anyway.
	 */
	if (mmap_state == pci_mmap_io)
		return -EINVAL;

	/*
	 * Ignore write-combine; for now only return uncached mappings.
	 */
	prot = pgprot_val(vma->vm_page_prot);
	prot = (prot & ~_CACHE_MASK) | _CACHE_UNCACHED;
	vma->vm_page_prot = __pgprot(prot);

	return remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff,
		vma->vm_end - vma->vm_start, vma->vm_page_prot);
}

<<<<<<< HEAD
char * (*pcibios_plat_setup)(char *str) __devinitdata;

char *__devinit pcibios_setup(char *str)
=======
char * (*pcibios_plat_setup)(char *str) __initdata;

char *__init pcibios_setup(char *str)
>>>>>>> refs/remotes/origin/master
{
	if (pcibios_plat_setup)
		return pcibios_plat_setup(str);
	return str;
}
