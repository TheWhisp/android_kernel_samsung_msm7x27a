/*
 * Generic VM initialization for x86-64 NUMA setups.
 * Copyright 2002,2003 Andi Kleen, SuSE Labs.
 */
#include <linux/bootmem.h>

#include "numa_internal.h"

void __init initmem_init(void)
{
	x86_numa_init();
}
<<<<<<< HEAD

unsigned long __init numa_free_all_bootmem(void)
{
	unsigned long pages = 0;
	int i;

	for_each_online_node(i)
		pages += free_all_bootmem_node(NODE_DATA(i));

<<<<<<< HEAD
	pages += free_all_memory_core_early(MAX_NUMNODES);
=======
	pages += free_low_memory_core_early(MAX_NUMNODES);
>>>>>>> refs/remotes/origin/cm-10.0

	return pages;
}
=======
>>>>>>> refs/remotes/origin/master
