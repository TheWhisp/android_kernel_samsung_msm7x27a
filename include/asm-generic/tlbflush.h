#ifndef __ASM_GENERIC_TLBFLUSH_H
#define __ASM_GENERIC_TLBFLUSH_H
/*
 * This is a dummy tlbflush implementation that can be used on all
 * nommu architectures.
 * If you have an MMU, you need to write your own functions.
 */
#ifdef CONFIG_MMU
#error need to implement an architecture specific asm/tlbflush.h
#endif

<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/bug.h>

>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/bug.h>

>>>>>>> refs/remotes/origin/master
static inline void flush_tlb_mm(struct mm_struct *mm)
{
	BUG();
}


#endif /* __ASM_GENERIC_TLBFLUSH_H */
