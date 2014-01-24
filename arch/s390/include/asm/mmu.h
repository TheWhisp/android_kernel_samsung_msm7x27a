#ifndef __MMU_H
#define __MMU_H

<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/errno.h>

>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/errno.h>

>>>>>>> refs/remotes/origin/master
typedef struct {
	atomic_t attach_count;
	unsigned int flush_mm;
	spinlock_t list_lock;
	struct list_head pgtable_list;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct list_head gmap_list;
>>>>>>> refs/remotes/origin/cm-10.0
	unsigned long asce_bits;
	unsigned long asce_limit;
	unsigned long vdso_base;
	/* Cloned contexts will be created with extended page tables. */
	unsigned int alloc_pgste:1;
=======
	struct list_head gmap_list;
	unsigned long asce_bits;
	unsigned long asce_limit;
	unsigned long vdso_base;
>>>>>>> refs/remotes/origin/master
	/* The mmu context has extended page tables. */
	unsigned int has_pgste:1;
} mm_context_t;

#define INIT_MM_CONTEXT(name)						      \
	.context.list_lock    = __SPIN_LOCK_UNLOCKED(name.context.list_lock), \
<<<<<<< HEAD
<<<<<<< HEAD
	.context.pgtable_list = LIST_HEAD_INIT(name.context.pgtable_list),
=======
=======
>>>>>>> refs/remotes/origin/master
	.context.pgtable_list = LIST_HEAD_INIT(name.context.pgtable_list),    \
	.context.gmap_list = LIST_HEAD_INIT(name.context.gmap_list),

static inline int tprot(unsigned long addr)
{
	int rc = -EFAULT;

	asm volatile(
		"	tprot	0(%1),0\n"
		"0:	ipm	%0\n"
		"	srl	%0,28\n"
		"1:\n"
		EX_TABLE(0b,1b)
		: "+d" (rc) : "a" (addr) : "cc");
	return rc;
}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#endif
