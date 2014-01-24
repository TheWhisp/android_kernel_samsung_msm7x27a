#ifndef __MACH_ATMEL_MCI_H
#define __MACH_ATMEL_MCI_H

<<<<<<< HEAD
#include <mach/at_hdmac.h>
=======
#include <linux/platform_data/dma-atmel.h>
>>>>>>> refs/remotes/origin/master

/**
 * struct mci_dma_data - DMA data for MCI interface
 */
struct mci_dma_data {
	struct at_dma_slave	sdata;
};

/* accessor macros */
#define	slave_data_ptr(s)	(&(s)->sdata)
#define find_slave_dev(s)	((s)->sdata.dma_dev)

<<<<<<< HEAD
#define	setup_dma_addr(s, t, r)	do {		\
	if (s) {				\
		(s)->sdata.tx_reg = (t);	\
		(s)->sdata.rx_reg = (r);	\
	}					\
} while (0)

=======
>>>>>>> refs/remotes/origin/master
#endif /* __MACH_ATMEL_MCI_H */
