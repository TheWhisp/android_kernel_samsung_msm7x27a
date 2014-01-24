#ifndef _PARISC_MMZONE_H
#define _PARISC_MMZONE_H

<<<<<<< HEAD
#ifdef CONFIG_DISCONTIGMEM

#define MAX_PHYSMEM_RANGES 8 /* Fix the size for now (current known max is 3) */
=======
#define MAX_PHYSMEM_RANGES 8 /* Fix the size for now (current known max is 3) */

#ifdef CONFIG_DISCONTIGMEM

>>>>>>> refs/remotes/origin/master
extern int npmem_ranges;

struct node_map_data {
    pg_data_t pg_data;
};

extern struct node_map_data node_data[];

#define NODE_DATA(nid)          (&node_data[nid].pg_data)

/* We have these possible memory map layouts:
 * Astro: 0-3.75, 67.75-68, 4-64
 * zx1: 0-1, 257-260, 4-256
 * Stretch (N-class): 0-2, 4-32, 34-xxx
 */

/* Since each 1GB can only belong to one region (node), we can create
 * an index table for pfn to nid lookup; each entry in pfnnid_map 
 * represents 1GB, and contains the node that the memory belongs to. */

#define PFNNID_SHIFT (30 - PAGE_SHIFT)
#define PFNNID_MAP_MAX  512     /* support 512GB */
<<<<<<< HEAD
extern unsigned char pfnnid_map[PFNNID_MAP_MAX];
=======
extern signed char pfnnid_map[PFNNID_MAP_MAX];
>>>>>>> refs/remotes/origin/master

#ifndef CONFIG_64BIT
#define pfn_is_io(pfn) ((pfn & (0xf0000000UL >> PAGE_SHIFT)) == (0xf0000000UL >> PAGE_SHIFT))
#else
/* io can be 0xf0f0f0f0f0xxxxxx or 0xfffffffff0000000 */
#define pfn_is_io(pfn) ((pfn & (0xf000000000000000UL >> PAGE_SHIFT)) == (0xf000000000000000UL >> PAGE_SHIFT))
#endif

static inline int pfn_to_nid(unsigned long pfn)
{
	unsigned int i;
<<<<<<< HEAD
	unsigned char r;
=======
>>>>>>> refs/remotes/origin/master

	if (unlikely(pfn_is_io(pfn)))
		return 0;

	i = pfn >> PFNNID_SHIFT;
<<<<<<< HEAD
	BUG_ON(i >= sizeof(pfnnid_map) / sizeof(pfnnid_map[0]));
	r = pfnnid_map[i];
	BUG_ON(r == 0xff);

	return (int)r;
=======
	BUG_ON(i >= ARRAY_SIZE(pfnnid_map));

	return pfnnid_map[i];
>>>>>>> refs/remotes/origin/master
}

static inline int pfn_valid(int pfn)
{
	int nid = pfn_to_nid(pfn);

	if (nid >= 0)
		return (pfn < node_end_pfn(nid));
	return 0;
}

<<<<<<< HEAD
#else /* !CONFIG_DISCONTIGMEM */
#define MAX_PHYSMEM_RANGES 	1 
=======
>>>>>>> refs/remotes/origin/master
#endif
#endif /* _PARISC_MMZONE_H */
