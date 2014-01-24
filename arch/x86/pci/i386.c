/*
 *	Low-Level PCI Access for i386 machines
 *
 * Copyright 1993, 1994 Drew Eckhardt
 *      Visionary Computing
 *      (Unix and Linux consulting and custom programming)
 *      Drew@Colorado.EDU
 *      +1 (303) 786-7975
 *
 * Drew's work was sponsored by:
 *	iX Multiuser Multitasking Magazine
 *	Hannover, Germany
 *	hm@ix.de
 *
 * Copyright 1997--2000 Martin Mares <mj@ucw.cz>
 *
 * For more information, please consult the following manuals (look at
 * http://www.pcisig.com/ for how to get them):
 *
 * PCI BIOS Specification
 * PCI Local Bus Specification
 * PCI to PCI Bridge Specification
 * PCI System Design Guide
 *
 */

#include <linux/types.h>
#include <linux/kernel.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/errno.h>
#include <linux/bootmem.h>

#include <asm/pat.h>
#include <asm/e820.h>
#include <asm/pci_x86.h>
#include <asm/io_apic.h>


<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
/*
 * This list of dynamic mappings is for temporarily maintaining
 * original BIOS BAR addresses for possible reinstatement.
 */
struct pcibios_fwaddrmap {
	struct list_head list;
	struct pci_dev *dev;
	resource_size_t fw_addr[DEVICE_COUNT_RESOURCE];
};

static LIST_HEAD(pcibios_fwaddrmappings);
static DEFINE_SPINLOCK(pcibios_fwaddrmap_lock);
<<<<<<< HEAD
=======
static bool pcibios_fw_addr_done;
>>>>>>> refs/remotes/origin/master

/* Must be called with 'pcibios_fwaddrmap_lock' lock held. */
static struct pcibios_fwaddrmap *pcibios_fwaddrmap_lookup(struct pci_dev *dev)
{
	struct pcibios_fwaddrmap *map;

<<<<<<< HEAD
	WARN_ON(!spin_is_locked(&pcibios_fwaddrmap_lock));
=======
	WARN_ON_SMP(!spin_is_locked(&pcibios_fwaddrmap_lock));
>>>>>>> refs/remotes/origin/master

	list_for_each_entry(map, &pcibios_fwaddrmappings, list)
		if (map->dev == dev)
			return map;

	return NULL;
}

static void
pcibios_save_fw_addr(struct pci_dev *dev, int idx, resource_size_t fw_addr)
{
	unsigned long flags;
	struct pcibios_fwaddrmap *map;

<<<<<<< HEAD
=======
	if (pcibios_fw_addr_done)
		return;

>>>>>>> refs/remotes/origin/master
	spin_lock_irqsave(&pcibios_fwaddrmap_lock, flags);
	map = pcibios_fwaddrmap_lookup(dev);
	if (!map) {
		spin_unlock_irqrestore(&pcibios_fwaddrmap_lock, flags);
		map = kzalloc(sizeof(*map), GFP_KERNEL);
		if (!map)
			return;

		map->dev = pci_dev_get(dev);
		map->fw_addr[idx] = fw_addr;
		INIT_LIST_HEAD(&map->list);

		spin_lock_irqsave(&pcibios_fwaddrmap_lock, flags);
		list_add_tail(&map->list, &pcibios_fwaddrmappings);
	} else
		map->fw_addr[idx] = fw_addr;
	spin_unlock_irqrestore(&pcibios_fwaddrmap_lock, flags);
}

resource_size_t pcibios_retrieve_fw_addr(struct pci_dev *dev, int idx)
{
	unsigned long flags;
	struct pcibios_fwaddrmap *map;
	resource_size_t fw_addr = 0;

<<<<<<< HEAD
=======
	if (pcibios_fw_addr_done)
		return 0;

>>>>>>> refs/remotes/origin/master
	spin_lock_irqsave(&pcibios_fwaddrmap_lock, flags);
	map = pcibios_fwaddrmap_lookup(dev);
	if (map)
		fw_addr = map->fw_addr[idx];
	spin_unlock_irqrestore(&pcibios_fwaddrmap_lock, flags);

	return fw_addr;
}

<<<<<<< HEAD
static void pcibios_fw_addr_list_del(void)
=======
static void __init pcibios_fw_addr_list_del(void)
>>>>>>> refs/remotes/origin/master
{
	unsigned long flags;
	struct pcibios_fwaddrmap *entry, *next;

	spin_lock_irqsave(&pcibios_fwaddrmap_lock, flags);
	list_for_each_entry_safe(entry, next, &pcibios_fwaddrmappings, list) {
		list_del(&entry->list);
		pci_dev_put(entry->dev);
		kfree(entry);
	}
	spin_unlock_irqrestore(&pcibios_fwaddrmap_lock, flags);
<<<<<<< HEAD
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
	pcibios_fw_addr_done = true;
}

>>>>>>> refs/remotes/origin/master
static int
skip_isa_ioresource_align(struct pci_dev *dev) {

	if ((pci_probe & PCI_CAN_SKIP_ISA_ALIGN) &&
	    !(dev->bus->bridge_ctl & PCI_BRIDGE_CTL_ISA))
		return 1;
	return 0;
}

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
	resource_size_t start = res->start;

	if (res->flags & IORESOURCE_IO) {
		if (skip_isa_ioresource_align(dev))
			return start;
		if (start & 0x300)
			start = (start + 0x3ff) & ~0x3ff;
	}
	return start;
}
EXPORT_SYMBOL(pcibios_align_resource);

/*
 *  Handle resources of PCI devices.  If the world were perfect, we could
 *  just allocate all the resource regions and do nothing more.  It isn't.
 *  On the other hand, we cannot just re-allocate all devices, as it would
 *  require us to know lots of host bridge internals.  So we attempt to
 *  keep as much of the original configuration as possible, but tweak it
 *  when it's found to be wrong.
 *
 *  Known BIOS problems we have to work around:
 *	- I/O or memory regions not configured
 *	- regions configured, but not enabled in the command register
 *	- bogus I/O addresses above 64K used
 *	- expansion ROMs left enabled (this may sound harmless, but given
 *	  the fact the PCI specs explicitly allow address decoders to be
 *	  shared between expansion ROMs and other resource regions, it's
 *	  at least dangerous)
 *	- bad resource sizes or overlaps with other regions
 *
 *  Our solution:
 *	(1) Allocate resources for all buses behind PCI-to-PCI bridges.
 *	    This gives us fixed barriers on where we can allocate.
 *	(2) Allocate resources for all enabled devices.  If there is
 *	    a collision, just mark the resource as unallocated. Also
 *	    disable expansion ROMs during this step.
 *	(3) Try to allocate resources for disabled devices.  If the
 *	    resources were assigned correctly, everything goes well,
 *	    if they weren't, they won't disturb allocation of other
 *	    resources.
 *	(4) Assign new addresses to resources which were either
 *	    not configured at all or misconfigured.  If explicitly
 *	    requested by the user, configure expansion ROM address
 *	    as well.
 */

<<<<<<< HEAD
static void __init pcibios_allocate_bus_resources(struct list_head *bus_list)
{
	struct pci_bus *bus;
	struct pci_dev *dev;
	int idx;
	struct resource *r;

	/* Depth-First Search on bus tree */
	list_for_each_entry(bus, bus_list, node) {
		if ((dev = bus->self)) {
			for (idx = PCI_BRIDGE_RESOURCES;
			    idx < PCI_NUM_RESOURCES; idx++) {
				r = &dev->resource[idx];
				if (!r->flags)
					continue;
				if (!r->start ||
				    pci_claim_resource(dev, idx) < 0) {
					/*
					 * Something is wrong with the region.
					 * Invalidate the resource to prevent
					 * child resource allocations in this
					 * range.
					 */
					r->start = r->end = 0;
					r->flags = 0;
				}
			}
		}
		pcibios_allocate_bus_resources(&bus->children);
	}
}

=======
static void pcibios_allocate_bridge_resources(struct pci_dev *dev)
{
	int idx;
	struct resource *r;

	for (idx = PCI_BRIDGE_RESOURCES; idx < PCI_NUM_RESOURCES; idx++) {
		r = &dev->resource[idx];
		if (!r->flags)
			continue;
		if (r->parent)	/* Already allocated */
			continue;
		if (!r->start || pci_claim_resource(dev, idx) < 0) {
			/*
			 * Something is wrong with the region.
			 * Invalidate the resource to prevent
			 * child resource allocations in this
			 * range.
			 */
			r->start = r->end = 0;
			r->flags = 0;
		}
	}
}

static void pcibios_allocate_bus_resources(struct pci_bus *bus)
{
	struct pci_bus *child;

	/* Depth-First Search on bus tree */
	if (bus->self)
		pcibios_allocate_bridge_resources(bus->self);
	list_for_each_entry(child, &bus->children, node)
		pcibios_allocate_bus_resources(child);
}

>>>>>>> refs/remotes/origin/master
struct pci_check_idx_range {
	int start;
	int end;
};

<<<<<<< HEAD
static void __init pcibios_allocate_resources(int pass)
{
	struct pci_dev *dev = NULL;
=======
static void pcibios_allocate_dev_resources(struct pci_dev *dev, int pass)
{
>>>>>>> refs/remotes/origin/master
	int idx, disabled, i;
	u16 command;
	struct resource *r;

	struct pci_check_idx_range idx_range[] = {
		{ PCI_STD_RESOURCES, PCI_STD_RESOURCE_END },
#ifdef CONFIG_PCI_IOV
		{ PCI_IOV_RESOURCES, PCI_IOV_RESOURCE_END },
#endif
	};

<<<<<<< HEAD
	for_each_pci_dev(dev) {
		pci_read_config_word(dev, PCI_COMMAND, &command);
		for (i = 0; i < ARRAY_SIZE(idx_range); i++)
		for (idx = idx_range[i].start; idx <= idx_range[i].end; idx++) {
			r = &dev->resource[idx];
			if (r->parent)		/* Already allocated */
				continue;
			if (!r->start)		/* Address not assigned at all */
=======
	pci_read_config_word(dev, PCI_COMMAND, &command);
	for (i = 0; i < ARRAY_SIZE(idx_range); i++)
		for (idx = idx_range[i].start; idx <= idx_range[i].end; idx++) {
			r = &dev->resource[idx];
			if (r->parent)	/* Already allocated */
				continue;
			if (!r->start)	/* Address not assigned at all */
>>>>>>> refs/remotes/origin/master
				continue;
			if (r->flags & IORESOURCE_IO)
				disabled = !(command & PCI_COMMAND_IO);
			else
				disabled = !(command & PCI_COMMAND_MEMORY);
			if (pass == disabled) {
				dev_dbg(&dev->dev,
					"BAR %d: reserving %pr (d=%d, p=%d)\n",
					idx, r, disabled, pass);
				if (pci_claim_resource(dev, idx) < 0) {
					/* We'll assign a new address later */
<<<<<<< HEAD
<<<<<<< HEAD
					dev->fw_addr[idx] = r->start;
=======
					pcibios_save_fw_addr(dev,
							idx, r->start);
>>>>>>> refs/remotes/origin/cm-10.0
=======
					pcibios_save_fw_addr(dev,
							idx, r->start);
>>>>>>> refs/remotes/origin/master
					r->end -= r->start;
					r->start = 0;
				}
			}
		}
<<<<<<< HEAD
		if (!pass) {
			r = &dev->resource[PCI_ROM_RESOURCE];
			if (r->flags & IORESOURCE_ROM_ENABLE) {
				/* Turn the ROM off, leave the resource region,
				 * but keep it unregistered. */
				u32 reg;
				dev_dbg(&dev->dev, "disabling ROM %pR\n", r);
				r->flags &= ~IORESOURCE_ROM_ENABLE;
				pci_read_config_dword(dev,
						dev->rom_base_reg, &reg);
				pci_write_config_dword(dev, dev->rom_base_reg,
						reg & ~PCI_ROM_ADDRESS_ENABLE);
			}
=======
	if (!pass) {
		r = &dev->resource[PCI_ROM_RESOURCE];
		if (r->flags & IORESOURCE_ROM_ENABLE) {
			/* Turn the ROM off, leave the resource region,
			 * but keep it unregistered. */
			u32 reg;
			dev_dbg(&dev->dev, "disabling ROM %pR\n", r);
			r->flags &= ~IORESOURCE_ROM_ENABLE;
			pci_read_config_dword(dev, dev->rom_base_reg, &reg);
			pci_write_config_dword(dev, dev->rom_base_reg,
						reg & ~PCI_ROM_ADDRESS_ENABLE);
>>>>>>> refs/remotes/origin/master
		}
	}
}

<<<<<<< HEAD
static int __init pcibios_assign_resources(void)
{
	struct pci_dev *dev = NULL;
	struct resource *r;

	if (!(pci_probe & PCI_ASSIGN_ROMS)) {
		/*
		 * Try to use BIOS settings for ROMs, otherwise let
		 * pci_assign_unassigned_resources() allocate the new
		 * addresses.
		 */
		for_each_pci_dev(dev) {
			r = &dev->resource[PCI_ROM_RESOURCE];
			if (!r->flags || !r->start)
				continue;
			if (pci_claim_resource(dev, PCI_ROM_RESOURCE) < 0) {
				r->end -= r->start;
				r->start = 0;
			}
		}
	}

	pci_assign_unassigned_resources();
<<<<<<< HEAD
=======
	pcibios_fw_addr_list_del();
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void pcibios_allocate_resources(struct pci_bus *bus, int pass)
{
	struct pci_dev *dev;
	struct pci_bus *child;

	list_for_each_entry(dev, &bus->devices, bus_list) {
		pcibios_allocate_dev_resources(dev, pass);

		child = dev->subordinate;
		if (child)
			pcibios_allocate_resources(child, pass);
	}
}

static void pcibios_allocate_dev_rom_resource(struct pci_dev *dev)
{
	struct resource *r;

	/*
	 * Try to use BIOS settings for ROMs, otherwise let
	 * pci_assign_unassigned_resources() allocate the new
	 * addresses.
	 */
	r = &dev->resource[PCI_ROM_RESOURCE];
	if (!r->flags || !r->start)
		return;
	if (r->parent) /* Already allocated */
		return;

	if (pci_claim_resource(dev, PCI_ROM_RESOURCE) < 0) {
		r->end -= r->start;
		r->start = 0;
	}
}
static void pcibios_allocate_rom_resources(struct pci_bus *bus)
{
	struct pci_dev *dev;
	struct pci_bus *child;

	list_for_each_entry(dev, &bus->devices, bus_list) {
		pcibios_allocate_dev_rom_resource(dev);

		child = dev->subordinate;
		if (child)
			pcibios_allocate_rom_resources(child);
	}
}

static int __init pcibios_assign_resources(void)
{
	struct pci_bus *bus;

	if (!(pci_probe & PCI_ASSIGN_ROMS))
		list_for_each_entry(bus, &pci_root_buses, node)
			pcibios_allocate_rom_resources(bus);

	pci_assign_unassigned_resources();
	pcibios_fw_addr_list_del();
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
void __init pcibios_resource_survey(void)
{
	DBG("PCI: Allocating resources\n");
	pcibios_allocate_bus_resources(&pci_root_buses);
	pcibios_allocate_resources(0);
	pcibios_allocate_resources(1);
=======
void pcibios_resource_survey_bus(struct pci_bus *bus)
{
	dev_printk(KERN_DEBUG, &bus->dev, "Allocating resources\n");

	pcibios_allocate_bus_resources(bus);

	pcibios_allocate_resources(bus, 0);
	pcibios_allocate_resources(bus, 1);

	if (!(pci_probe & PCI_ASSIGN_ROMS))
		pcibios_allocate_rom_resources(bus);
}

void __init pcibios_resource_survey(void)
{
	struct pci_bus *bus;

	DBG("PCI: Allocating resources\n");

	list_for_each_entry(bus, &pci_root_buses, node)
		pcibios_allocate_bus_resources(bus);

	list_for_each_entry(bus, &pci_root_buses, node)
		pcibios_allocate_resources(bus, 0);
	list_for_each_entry(bus, &pci_root_buses, node)
		pcibios_allocate_resources(bus, 1);
>>>>>>> refs/remotes/origin/master

	e820_reserve_resources_late();
	/*
	 * Insert the IO APIC resources after PCI initialization has
	 * occurred to handle IO APICS that are mapped in on a BAR in
	 * PCI space, but before trying to assign unassigned pci res.
	 */
	ioapic_insert_resources();
}

/**
 * called in fs_initcall (one below subsys_initcall),
 * give a chance for motherboard reserve resources
 */
fs_initcall(pcibios_assign_resources);

<<<<<<< HEAD
<<<<<<< HEAD
/*
 *  If we set up a device for bus mastering, we need to check the latency
 *  timer as certain crappy BIOSes forget to set it properly.
 */
unsigned int pcibios_max_latency = 255;

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
	dev_printk(KERN_DEBUG, &dev->dev, "setting latency timer to %d\n", lat);
	pci_write_config_byte(dev, PCI_LATENCY_TIMER, lat);
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static const struct vm_operations_struct pci_mmap_ops = {
	.access = generic_access_phys,
};

int pci_mmap_page_range(struct pci_dev *dev, struct vm_area_struct *vma,
			enum pci_mmap_state mmap_state, int write_combine)
{
	unsigned long prot;

	/* I/O space cannot be accessed via normal processor loads and
	 * stores on this platform.
	 */
	if (mmap_state == pci_mmap_io)
		return -EINVAL;

	prot = pgprot_val(vma->vm_page_prot);

	/*
 	 * Return error if pat is not enabled and write_combine is requested.
 	 * Caller can followup with UC MINUS request and add a WC mtrr if there
 	 * is a free mtrr slot.
 	 */
	if (!pat_enabled && write_combine)
		return -EINVAL;

	if (pat_enabled && write_combine)
		prot |= _PAGE_CACHE_WC;
	else if (pat_enabled || boot_cpu_data.x86 > 3)
		/*
		 * ioremap() and ioremap_nocache() defaults to UC MINUS for now.
		 * To avoid attribute conflicts, request UC MINUS here
		 * as well.
		 */
		prot |= _PAGE_CACHE_UC_MINUS;

	prot |= _PAGE_IOMAP;	/* creating a mapping for IO */

	vma->vm_page_prot = __pgprot(prot);

	if (io_remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff,
			       vma->vm_end - vma->vm_start,
			       vma->vm_page_prot))
		return -EAGAIN;

	vma->vm_ops = &pci_mmap_ops;

	return 0;
}
