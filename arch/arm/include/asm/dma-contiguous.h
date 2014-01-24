#ifndef ASMARM_DMA_CONTIGUOUS_H
#define ASMARM_DMA_CONTIGUOUS_H

#ifdef __KERNEL__
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
#ifdef CONFIG_CMA

#include <linux/types.h>
#include <asm-generic/dma-contiguous.h>
<<<<<<< HEAD
=======
#ifdef CONFIG_DMA_CMA

#include <linux/types.h>
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

void dma_contiguous_early_fixup(phys_addr_t base, unsigned long size);

#endif
#endif

#endif
