#ifndef ASMX86_DMA_CONTIGUOUS_H
#define ASMX86_DMA_CONTIGUOUS_H

#ifdef __KERNEL__

#include <linux/types.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <asm-generic/dma-contiguous.h>
=======
>>>>>>> refs/remotes/origin/master
=======
#include <asm-generic/dma-contiguous.h>
>>>>>>> refs/remotes/origin/cm-11.0

static inline void
dma_contiguous_early_fixup(phys_addr_t base, unsigned long size) { }

#endif
#endif
