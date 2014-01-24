/*
**  linux/amiga/chipram.c
**
**      Modified 03-May-94 by Geert Uytterhoeven <geert@linux-m68k.org>
**          - 64-bit aligned allocations for full AGA compatibility
**
**	Rewritten 15/9/2000 by Geert to use resource management
*/

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/module.h>

<<<<<<< HEAD
=======
#include <asm/atomic.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <asm/page.h>
#include <asm/amigahw.h>

unsigned long amiga_chip_size;
EXPORT_SYMBOL(amiga_chip_size);

static struct resource chipram_res = {
<<<<<<< HEAD
    .name = "Chip RAM", .start = CHIP_PHYSADDR
};
static unsigned long chipavail;
=======
	.name = "Chip RAM", .start = CHIP_PHYSADDR
};
static atomic_t chipavail;
>>>>>>> refs/remotes/origin/cm-10.0


void __init amiga_chip_init(void)
{
<<<<<<< HEAD
    if (!AMIGAHW_PRESENT(CHIP_RAM))
	return;

    chipram_res.end = amiga_chip_size-1;
    request_resource(&iomem_resource, &chipram_res);

    chipavail = amiga_chip_size;
=======
	if (!AMIGAHW_PRESENT(CHIP_RAM))
		return;

	chipram_res.end = CHIP_PHYSADDR + amiga_chip_size - 1;
	request_resource(&iomem_resource, &chipram_res);

	atomic_set(&chipavail, amiga_chip_size);
>>>>>>> refs/remotes/origin/cm-10.0
}


void *amiga_chip_alloc(unsigned long size, const char *name)
{
<<<<<<< HEAD
    struct resource *res;

    /* round up */
    size = PAGE_ALIGN(size);

#ifdef DEBUG
    printk("amiga_chip_alloc: allocate %ld bytes\n", size);
#endif
    res = kzalloc(sizeof(struct resource), GFP_KERNEL);
    if (!res)
	return NULL;
    res->name = name;

    if (allocate_resource(&chipram_res, res, size, 0, UINT_MAX, PAGE_SIZE, NULL, NULL) < 0) {
	kfree(res);
	return NULL;
    }
    chipavail -= size;
#ifdef DEBUG
    printk("amiga_chip_alloc: returning %lx\n", res->start);
#endif
    return (void *)ZTWO_VADDR(res->start);
=======
	struct resource *res;
	void *p;

	res = kzalloc(sizeof(struct resource), GFP_KERNEL);
	if (!res)
		return NULL;

	res->name = name;
	p = amiga_chip_alloc_res(size, res);
	if (!p) {
		kfree(res);
		return NULL;
	}

	return p;
>>>>>>> refs/remotes/origin/cm-10.0
}
EXPORT_SYMBOL(amiga_chip_alloc);


<<<<<<< HEAD
    /*
     *  Warning:
     *  amiga_chip_alloc_res is meant only for drivers that need to allocate
     *  Chip RAM before kmalloc() is functional. As a consequence, those
     *  drivers must not free that Chip RAM afterwards.
     */

void * __init amiga_chip_alloc_res(unsigned long size, struct resource *res)
{
    unsigned long start;

    /* round up */
    size = PAGE_ALIGN(size);
    /* dmesg into chipmem prefers memory at the safe end */
    start = CHIP_PHYSADDR + chipavail - size;

#ifdef DEBUG
    printk("amiga_chip_alloc_res: allocate %ld bytes\n", size);
#endif
    if (allocate_resource(&chipram_res, res, size, start, UINT_MAX, PAGE_SIZE, NULL, NULL) < 0) {
	printk("amiga_chip_alloc_res: first alloc failed!\n");
	if (allocate_resource(&chipram_res, res, size, 0, UINT_MAX, PAGE_SIZE, NULL, NULL) < 0)
	    return NULL;
    }
    chipavail -= size;
#ifdef DEBUG
    printk("amiga_chip_alloc_res: returning %lx\n", res->start);
#endif
    return (void *)ZTWO_VADDR(res->start);
=======
	/*
	 *  Warning:
	 *  amiga_chip_alloc_res is meant only for drivers that need to
	 *  allocate Chip RAM before kmalloc() is functional. As a consequence,
	 *  those drivers must not free that Chip RAM afterwards.
	 */

void *amiga_chip_alloc_res(unsigned long size, struct resource *res)
{
	int error;

	/* round up */
	size = PAGE_ALIGN(size);

	pr_debug("amiga_chip_alloc_res: allocate %lu bytes\n", size);
	error = allocate_resource(&chipram_res, res, size, 0, UINT_MAX,
				  PAGE_SIZE, NULL, NULL);
	if (error < 0) {
		pr_err("amiga_chip_alloc_res: allocate_resource() failed %d!\n",
		       error);
		return NULL;
	}

	atomic_sub(size, &chipavail);
	pr_debug("amiga_chip_alloc_res: returning %pR\n", res);
	return (void *)ZTWO_VADDR(res->start);
>>>>>>> refs/remotes/origin/cm-10.0
}

void amiga_chip_free(void *ptr)
{
<<<<<<< HEAD
    unsigned long start = ZTWO_PADDR(ptr);
    struct resource **p, *res;
    unsigned long size;

    for (p = &chipram_res.child; (res = *p); p = &res->sibling) {
	if (res->start != start)
	    continue;
	*p = res->sibling;
	size = res->end-start;
#ifdef DEBUG
	printk("amiga_chip_free: free %ld bytes at %p\n", size, ptr);
#endif
	chipavail += size;
	kfree(res);
	return;
    }
    printk("amiga_chip_free: trying to free nonexistent region at %p\n", ptr);
=======
	unsigned long start = ZTWO_PADDR(ptr);
	struct resource *res;
	unsigned long size;

	res = lookup_resource(&chipram_res, start);
	if (!res) {
		pr_err("amiga_chip_free: trying to free nonexistent region at "
		       "%p\n", ptr);
		return;
	}

	size = resource_size(res);
	pr_debug("amiga_chip_free: free %lu bytes at %p\n", size, ptr);
	atomic_add(size, &chipavail);
	release_resource(res);
	kfree(res);
>>>>>>> refs/remotes/origin/cm-10.0
}
EXPORT_SYMBOL(amiga_chip_free);


unsigned long amiga_chip_avail(void)
{
<<<<<<< HEAD
#ifdef DEBUG
	printk("amiga_chip_avail : %ld bytes\n", chipavail);
#endif
	return chipavail;
=======
	unsigned long n = atomic_read(&chipavail);

	pr_debug("amiga_chip_avail : %lu bytes\n", n);
	return n;
>>>>>>> refs/remotes/origin/cm-10.0
}
EXPORT_SYMBOL(amiga_chip_avail);

