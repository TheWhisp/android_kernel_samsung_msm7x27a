#ifndef __ASM_GENERIC_MMU_H
#define __ASM_GENERIC_MMU_H

/*
 * This is the mmu.h header for nommu implementations.
 * Architectures with an MMU need something more complex.
 */
#ifndef __ASSEMBLY__
typedef struct {
<<<<<<< HEAD
	struct vm_list_struct	*vmlist;
	unsigned long		end_brk;
=======
	unsigned long		end_brk;

#ifdef CONFIG_BINFMT_ELF_FDPIC
	unsigned long		exec_fdpic_loadmap;
	unsigned long		interp_fdpic_loadmap;
#endif
>>>>>>> refs/remotes/origin/master
} mm_context_t;
#endif

#endif /* __ASM_GENERIC_MMU_H */
