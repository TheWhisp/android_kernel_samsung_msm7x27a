#ifndef _ASM_S390_SPARSEMEM_H
#define _ASM_S390_SPARSEMEM_H

#ifdef CONFIG_64BIT

#define SECTION_SIZE_BITS	28
<<<<<<< HEAD
<<<<<<< HEAD
#define MAX_PHYSADDR_BITS	42
#define MAX_PHYSMEM_BITS	42
=======
#define MAX_PHYSADDR_BITS	46
#define MAX_PHYSMEM_BITS	46
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define MAX_PHYSMEM_BITS	46
>>>>>>> refs/remotes/origin/master

#else

#define SECTION_SIZE_BITS	25
<<<<<<< HEAD
#define MAX_PHYSADDR_BITS	31
=======
>>>>>>> refs/remotes/origin/master
#define MAX_PHYSMEM_BITS	31

#endif /* CONFIG_64BIT */

#endif /* _ASM_S390_SPARSEMEM_H */
