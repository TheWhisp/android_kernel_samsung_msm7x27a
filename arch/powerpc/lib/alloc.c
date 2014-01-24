#include <linux/types.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/bootmem.h>
#include <linux/string.h>
<<<<<<< HEAD
<<<<<<< HEAD

#include <asm/system.h>
=======
#include <asm/setup.h>

>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <asm/setup.h>

>>>>>>> refs/remotes/origin/master

void * __init_refok zalloc_maybe_bootmem(size_t size, gfp_t mask)
{
	void *p;

	if (mem_init_done)
		p = kzalloc(size, mask);
	else {
		p = alloc_bootmem(size);
		if (p)
			memset(p, 0, size);
	}
	return p;
}
