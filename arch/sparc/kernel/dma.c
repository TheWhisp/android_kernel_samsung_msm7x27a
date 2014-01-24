#include <linux/kernel.h>
<<<<<<< HEAD
#include <linux/module.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/dma-mapping.h>
#include <linux/dma-debug.h>

#define PREALLOC_DMA_DEBUG_ENTRIES       (1 << 15)

static int __init dma_init(void)
{
	dma_debug_init(PREALLOC_DMA_DEBUG_ENTRIES);
	return 0;
}
fs_initcall(dma_init);
