#ifndef _LINUX_SLAB_DEF_H
#define	_LINUX_SLAB_DEF_H

/*
 * Definitions unique to the original Linux SLAB allocator.
<<<<<<< HEAD
 *
 * What we provide here is a way to optimize the frequent kmalloc
 * calls in the kernel by selecting the appropriate general cache
 * if kmalloc was called with a size that can be established at
 * compile time.
 */

#include <linux/init.h>
#include <asm/page.h>		/* kmalloc_sizes.h needs PAGE_SIZE */
#include <asm/cache.h>		/* kmalloc_sizes.h needs L1_CACHE_BYTES */
#include <linux/compiler.h>

<<<<<<< HEAD
#include <trace/events/kmem.h>

/*
 * Enforce a minimum alignment for the kmalloc caches.
 * Usually, the kmalloc caches are cache_line_size() aligned, except when
 * DEBUG and FORCED_DEBUG are enabled, then they are BYTES_PER_WORD aligned.
 * Some archs want to perform DMA into kmalloc caches and need a guaranteed
 * alignment larger than the alignment of a 64-bit integer.
 * ARCH_KMALLOC_MINALIGN allows that.
 * Note that increasing this value may disable some debug features.
 */
#ifdef ARCH_DMA_MINALIGN
#define ARCH_KMALLOC_MINALIGN ARCH_DMA_MINALIGN
#else
#define ARCH_KMALLOC_MINALIGN __alignof__(unsigned long long)
#endif

#ifndef ARCH_SLAB_MINALIGN
/*
 * Enforce a minimum alignment for all caches.
 * Intended for archs that get misalignment faults even for BYTES_PER_WORD
 * aligned buffers. Includes ARCH_KMALLOC_MINALIGN.
 * If possible: Do not enable this flag for CONFIG_DEBUG_SLAB, it disables
 * some debug features.
 */
#define ARCH_SLAB_MINALIGN 0
#endif

=======
>>>>>>> refs/remotes/origin/cm-10.0
/*
 * struct kmem_cache
 *
 * manages a cache.
 */

struct kmem_cache {
<<<<<<< HEAD
/* 1) per-cpu data, touched during every alloc/free */
	struct array_cache *array[NR_CPUS];
/* 2) Cache tunables. Protected by cache_chain_mutex */
=======
/* 1) Cache tunables. Protected by cache_chain_mutex */
>>>>>>> refs/remotes/origin/cm-10.0
=======
 */

struct kmem_cache {
/* 1) Cache tunables. Protected by cache_chain_mutex */
>>>>>>> refs/remotes/origin/master
	unsigned int batchcount;
	unsigned int limit;
	unsigned int shared;

<<<<<<< HEAD
	unsigned int buffer_size;
	u32 reciprocal_buffer_size;
<<<<<<< HEAD
/* 3) touched by every alloc & free from the backend */
=======
/* 2) touched by every alloc & free from the backend */
>>>>>>> refs/remotes/origin/cm-10.0
=======
	unsigned int size;
	u32 reciprocal_buffer_size;
/* 2) touched by every alloc & free from the backend */
>>>>>>> refs/remotes/origin/master

	unsigned int flags;		/* constant flags */
	unsigned int num;		/* # of objs per slab */

<<<<<<< HEAD
<<<<<<< HEAD
/* 4) cache_grow/shrink */
=======
/* 3) cache_grow/shrink */
>>>>>>> refs/remotes/origin/cm-10.0
=======
/* 3) cache_grow/shrink */
>>>>>>> refs/remotes/origin/master
	/* order of pgs per slab (2^n) */
	unsigned int gfporder;

	/* force GFP flags, e.g. GFP_DMA */
<<<<<<< HEAD
	gfp_t gfpflags;

	size_t colour;			/* cache colouring range */
	unsigned int colour_off;	/* colour offset */
	struct kmem_cache *slabp_cache;
	unsigned int slab_size;
	unsigned int dflags;		/* dynamic flags */
=======
	gfp_t allocflags;

	size_t colour;			/* cache colouring range */
	unsigned int colour_off;	/* colour offset */
	struct kmem_cache *freelist_cache;
	unsigned int freelist_size;
>>>>>>> refs/remotes/origin/master

	/* constructor func */
	void (*ctor)(void *obj);

<<<<<<< HEAD
<<<<<<< HEAD
/* 5) cache creation/removal */
	const char *name;
	struct list_head next;

/* 6) statistics */
=======
/* 4) cache creation/removal */
	const char *name;
	struct list_head next;

/* 5) statistics */
>>>>>>> refs/remotes/origin/cm-10.0
=======
/* 4) cache creation/removal */
	const char *name;
	struct list_head list;
	int refcount;
	int object_size;
	int align;

/* 5) statistics */
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_DEBUG_SLAB
	unsigned long num_active;
	unsigned long num_allocations;
	unsigned long high_mark;
	unsigned long grown;
	unsigned long reaped;
	unsigned long errors;
	unsigned long max_freeable;
	unsigned long node_allocs;
	unsigned long node_frees;
	unsigned long node_overflow;
	atomic_t allochit;
	atomic_t allocmiss;
	atomic_t freehit;
	atomic_t freemiss;

	/*
	 * If debugging is enabled, then the allocator can add additional
<<<<<<< HEAD
	 * fields and/or padding to every object. buffer_size contains the total
=======
	 * fields and/or padding to every object. size contains the total
>>>>>>> refs/remotes/origin/master
	 * object size including these internal fields, the following two
	 * variables contain the offset to the user object and its size.
	 */
	int obj_offset;
<<<<<<< HEAD
	int obj_size;
#endif /* CONFIG_DEBUG_SLAB */

<<<<<<< HEAD
	/*
	 * We put nodelists[] at the end of kmem_cache, because we want to size
	 * this array to nr_node_ids slots instead of MAX_NUMNODES
	 * (see kmem_cache_init())
	 * We still use [MAX_NUMNODES] and not [1] or [0] because cache_cache
	 * is statically defined, so we reserve the max number of nodes.
	 */
	struct kmem_list3 *nodelists[MAX_NUMNODES];
	/*
	 * Do not add fields after nodelists[]
=======
=======
#endif /* CONFIG_DEBUG_SLAB */
#ifdef CONFIG_MEMCG_KMEM
	struct memcg_cache_params *memcg_params;
#endif

>>>>>>> refs/remotes/origin/master
/* 6) per-cpu/per-node data, touched during every alloc/free */
	/*
	 * We put array[] at the end of kmem_cache, because we want to size
	 * this array to nr_cpu_ids slots instead of NR_CPUS
	 * (see kmem_cache_init())
	 * We still use [NR_CPUS] and not [1] or [0] because cache_cache
	 * is statically defined, so we reserve the max number of cpus.
<<<<<<< HEAD
	 */
	struct kmem_list3 **nodelists;
	struct array_cache *array[NR_CPUS];
	/*
	 * Do not add fields after array[]
>>>>>>> refs/remotes/origin/cm-10.0
	 */
};

/* Size description struct for general caches. */
struct cache_sizes {
	size_t		 	cs_size;
	struct kmem_cache	*cs_cachep;
#ifdef CONFIG_ZONE_DMA
	struct kmem_cache	*cs_dmacachep;
#endif
};
extern struct cache_sizes malloc_sizes[];

void *kmem_cache_alloc(struct kmem_cache *, gfp_t);
void *__kmalloc(size_t size, gfp_t flags);

#ifdef CONFIG_TRACING
extern void *kmem_cache_alloc_trace(size_t size,
				    struct kmem_cache *cachep, gfp_t flags);
extern size_t slab_buffer_size(struct kmem_cache *cachep);
#else
static __always_inline void *
kmem_cache_alloc_trace(size_t size, struct kmem_cache *cachep, gfp_t flags)
{
	return kmem_cache_alloc(cachep, flags);
}
static inline size_t slab_buffer_size(struct kmem_cache *cachep)
{
	return 0;
}
#endif

static __always_inline void *kmalloc(size_t size, gfp_t flags)
{
	struct kmem_cache *cachep;
	void *ret;

	if (__builtin_constant_p(size)) {
		int i = 0;

		if (!size)
			return ZERO_SIZE_PTR;

#define CACHE(x) \
		if (size <= x) \
			goto found; \
		else \
			i++;
#include <linux/kmalloc_sizes.h>
#undef CACHE
		return NULL;
found:
#ifdef CONFIG_ZONE_DMA
		if (flags & GFP_DMA)
			cachep = malloc_sizes[i].cs_dmacachep;
		else
#endif
			cachep = malloc_sizes[i].cs_cachep;

		ret = kmem_cache_alloc_trace(size, cachep, flags);

		return ret;
	}
	return __kmalloc(size, flags);
}

#ifdef CONFIG_NUMA
extern void *__kmalloc_node(size_t size, gfp_t flags, int node);
extern void *kmem_cache_alloc_node(struct kmem_cache *, gfp_t flags, int node);

#ifdef CONFIG_TRACING
extern void *kmem_cache_alloc_node_trace(size_t size,
					 struct kmem_cache *cachep,
					 gfp_t flags,
					 int nodeid);
#else
static __always_inline void *
kmem_cache_alloc_node_trace(size_t size,
			    struct kmem_cache *cachep,
			    gfp_t flags,
			    int nodeid)
{
	return kmem_cache_alloc_node(cachep, flags, nodeid);
}
#endif

static __always_inline void *kmalloc_node(size_t size, gfp_t flags, int node)
{
	struct kmem_cache *cachep;

	if (__builtin_constant_p(size)) {
		int i = 0;

		if (!size)
			return ZERO_SIZE_PTR;

#define CACHE(x) \
		if (size <= x) \
			goto found; \
		else \
			i++;
#include <linux/kmalloc_sizes.h>
#undef CACHE
		return NULL;
found:
#ifdef CONFIG_ZONE_DMA
		if (flags & GFP_DMA)
			cachep = malloc_sizes[i].cs_dmacachep;
		else
#endif
			cachep = malloc_sizes[i].cs_cachep;

		return kmem_cache_alloc_node_trace(size, cachep, flags, node);
	}
	return __kmalloc_node(size, flags, node);
}

#endif	/* CONFIG_NUMA */

=======
	 *
	 * We also need to guarantee that the list is able to accomodate a
	 * pointer for each node since "nodelists" uses the remainder of
	 * available pointers.
	 */
	struct kmem_cache_node **node;
	struct array_cache *array[NR_CPUS + MAX_NUMNODES];
	/*
	 * Do not add fields after array[]
	 */
};

>>>>>>> refs/remotes/origin/master
#endif	/* _LINUX_SLAB_DEF_H */
