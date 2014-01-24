/*
 * Written by Pat Gaughen (gone@us.ibm.com) Mar 2002
 *
 */

#ifndef _ASM_X86_MMZONE_32_H
#define _ASM_X86_MMZONE_32_H

#include <asm/smp.h>

#ifdef CONFIG_NUMA
extern struct pglist_data *node_data[];
#define NODE_DATA(nid)	(node_data[nid])

#include <asm/numaq.h>

#endif /* CONFIG_NUMA */

#ifdef CONFIG_DISCONTIGMEM

/*
 * generic node memory support, the following assumptions apply:
 *
 * 1) memory comes in 64Mb contiguous chunks which are either present or not
 * 2) we will not have more than 64Gb in total
 *
 * for now assume that 64Gb is max amount of RAM for whole system
 *    64Gb / 4096bytes/page = 16777216 pages
 */
#define MAX_NR_PAGES 16777216
<<<<<<< HEAD
<<<<<<< HEAD
#define MAX_ELEMENTS 1024
#define PAGES_PER_ELEMENT (MAX_NR_PAGES/MAX_ELEMENTS)
=======
#define MAX_SECTIONS 1024
#define PAGES_PER_SECTION (MAX_NR_PAGES/MAX_SECTIONS)
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define MAX_SECTIONS 1024
#define PAGES_PER_SECTION (MAX_NR_PAGES/MAX_SECTIONS)
>>>>>>> refs/remotes/origin/master

extern s8 physnode_map[];

static inline int pfn_to_nid(unsigned long pfn)
{
#ifdef CONFIG_NUMA
<<<<<<< HEAD
<<<<<<< HEAD
	return((int) physnode_map[(pfn) / PAGES_PER_ELEMENT]);
=======
	return((int) physnode_map[(pfn) / PAGES_PER_SECTION]);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return((int) physnode_map[(pfn) / PAGES_PER_SECTION]);
>>>>>>> refs/remotes/origin/master
#else
	return 0;
#endif
}

static inline int pfn_valid(int pfn)
{
	int nid = pfn_to_nid(pfn);

	if (nid >= 0)
		return (pfn < node_end_pfn(nid));
	return 0;
}

#define early_pfn_valid(pfn)	pfn_valid((pfn))

#endif /* CONFIG_DISCONTIGMEM */

<<<<<<< HEAD
#ifdef CONFIG_NEED_MULTIPLE_NODES
/* always use node 0 for bootmem on this numa platform */
#define bootmem_arch_preferred_node(__bdata, size, align, goal, limit)	\
	(NODE_DATA(0)->bdata)
#endif /* CONFIG_NEED_MULTIPLE_NODES */

=======
>>>>>>> refs/remotes/origin/master
#endif /* _ASM_X86_MMZONE_32_H */
