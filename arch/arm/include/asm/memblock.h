#ifndef _ASM_ARM_MEMBLOCK_H
#define _ASM_ARM_MEMBLOCK_H

struct meminfo;
struct machine_desc;

extern void arm_memblock_init(struct meminfo *, struct machine_desc *);

<<<<<<< HEAD
=======
phys_addr_t arm_memblock_steal(phys_addr_t size, phys_addr_t align);

>>>>>>> refs/remotes/origin/cm-10.0
#endif
