/*
 * Driver for the Synopsys DesignWare DMA Controller (aka DMACA on
 * AVR32 systems.)
 *
 * Copyright (C) 2007 Atmel Corporation
 * Copyright (C) 2010-2011 ST Microelectronics
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef DW_DMAC_H
#define DW_DMAC_H

#include <linux/dmaengine.h>

/**
<<<<<<< HEAD
=======
 * struct dw_dma_slave - Controller-specific information about a slave
 *
 * @dma_dev: required DMA master device. Depricated.
 * @bus_id: name of this device channel, not just a device name since
 *          devices may have more than one channel e.g. "foo_tx"
 * @cfg_hi: Platform-specific initializer for the CFG_HI register
 * @cfg_lo: Platform-specific initializer for the CFG_LO register
 * @src_master: src master for transfers on allocated channel.
 * @dst_master: dest master for transfers on allocated channel.
 */
struct dw_dma_slave {
	struct device		*dma_dev;
	u32			cfg_hi;
	u32			cfg_lo;
	u8			src_master;
	u8			dst_master;
};

/**
>>>>>>> refs/remotes/origin/master
 * struct dw_dma_platform_data - Controller configuration parameters
 * @nr_channels: Number of channels supported by hardware (max 8)
 * @is_private: The device channels should be marked as private and not for
 *	by the general purpose DMA channel allocator.
<<<<<<< HEAD
=======
 * @chan_allocation_order: Allocate channels starting from 0 or 7
 * @chan_priority: Set channel priority increasing from 0 to 7 or 7 to 0.
 * @block_size: Maximum block size supported by the controller
 * @nr_masters: Number of AHB masters supported by the controller
 * @data_width: Maximum data width supported by hardware per AHB master
 *		(0 - 8bits, 1 - 16bits, ..., 5 - 256bits)
 * @sd: slave specific data. Used for configuring channels
 * @sd_count: count of slave data structures passed.
>>>>>>> refs/remotes/origin/master
 */
struct dw_dma_platform_data {
	unsigned int	nr_channels;
	bool		is_private;
#define CHAN_ALLOCATION_ASCENDING	0	/* zero to seven */
#define CHAN_ALLOCATION_DESCENDING	1	/* seven to zero */
	unsigned char	chan_allocation_order;
#define CHAN_PRIORITY_ASCENDING		0	/* chan0 highest */
#define CHAN_PRIORITY_DESCENDING	1	/* chan7 highest */
	unsigned char	chan_priority;
<<<<<<< HEAD
};

<<<<<<< HEAD
/**
 * enum dw_dma_slave_width - DMA slave register access width.
 * @DMA_SLAVE_WIDTH_8BIT: Do 8-bit slave register accesses
 * @DMA_SLAVE_WIDTH_16BIT: Do 16-bit slave register accesses
 * @DMA_SLAVE_WIDTH_32BIT: Do 32-bit slave register accesses
 */
enum dw_dma_slave_width {
	DW_DMA_SLAVE_WIDTH_8BIT,
	DW_DMA_SLAVE_WIDTH_16BIT,
	DW_DMA_SLAVE_WIDTH_32BIT,
};

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	unsigned short	block_size;
	unsigned char	nr_masters;
	unsigned char	data_width[4];
};

>>>>>>> refs/remotes/origin/master
/* bursts size */
enum dw_dma_msize {
	DW_DMA_MSIZE_1,
	DW_DMA_MSIZE_4,
	DW_DMA_MSIZE_8,
	DW_DMA_MSIZE_16,
	DW_DMA_MSIZE_32,
	DW_DMA_MSIZE_64,
	DW_DMA_MSIZE_128,
	DW_DMA_MSIZE_256,
};

<<<<<<< HEAD
<<<<<<< HEAD
/* flow controller */
enum dw_dma_fc {
	DW_DMA_FC_D_M2M,
	DW_DMA_FC_D_M2P,
	DW_DMA_FC_D_P2M,
	DW_DMA_FC_D_P2P,
	DW_DMA_FC_P_P2M,
	DW_DMA_FC_SP_P2P,
	DW_DMA_FC_P_M2P,
	DW_DMA_FC_DP_P2P,
};

=======
>>>>>>> refs/remotes/origin/cm-10.0
/**
 * struct dw_dma_slave - Controller-specific information about a slave
 *
 * @dma_dev: required DMA master device
<<<<<<< HEAD
 * @tx_reg: physical address of data register used for
 *	memory-to-peripheral transfers
 * @rx_reg: physical address of data register used for
 *	peripheral-to-memory transfers
 * @reg_width: peripheral register width
=======
>>>>>>> refs/remotes/origin/cm-10.0
 * @cfg_hi: Platform-specific initializer for the CFG_HI register
 * @cfg_lo: Platform-specific initializer for the CFG_LO register
 * @src_master: src master for transfers on allocated channel.
 * @dst_master: dest master for transfers on allocated channel.
<<<<<<< HEAD
 * @src_msize: src burst size.
 * @dst_msize: dest burst size.
 * @fc: flow controller for DMA transfer
 */
struct dw_dma_slave {
	struct device		*dma_dev;
	dma_addr_t		tx_reg;
	dma_addr_t		rx_reg;
	enum dw_dma_slave_width	reg_width;
=======
 */
struct dw_dma_slave {
	struct device		*dma_dev;
>>>>>>> refs/remotes/origin/cm-10.0
	u32			cfg_hi;
	u32			cfg_lo;
	u8			src_master;
	u8			dst_master;
<<<<<<< HEAD
	u8			src_msize;
	u8			dst_msize;
	u8			fc;
=======
>>>>>>> refs/remotes/origin/cm-10.0
};

=======
>>>>>>> refs/remotes/origin/master
/* Platform-configurable bits in CFG_HI */
#define DWC_CFGH_FCMODE		(1 << 0)
#define DWC_CFGH_FIFO_MODE	(1 << 1)
#define DWC_CFGH_PROTCTL(x)	((x) << 2)
#define DWC_CFGH_SRC_PER(x)	((x) << 7)
#define DWC_CFGH_DST_PER(x)	((x) << 11)

/* Platform-configurable bits in CFG_LO */
#define DWC_CFGL_LOCK_CH_XFER	(0 << 12)	/* scope of LOCK_CH */
#define DWC_CFGL_LOCK_CH_BLOCK	(1 << 12)
#define DWC_CFGL_LOCK_CH_XACT	(2 << 12)
#define DWC_CFGL_LOCK_BUS_XFER	(0 << 14)	/* scope of LOCK_BUS */
#define DWC_CFGL_LOCK_BUS_BLOCK	(1 << 14)
#define DWC_CFGL_LOCK_BUS_XACT	(2 << 14)
#define DWC_CFGL_LOCK_CH	(1 << 15)	/* channel lockout */
#define DWC_CFGL_LOCK_BUS	(1 << 16)	/* busmaster lockout */
#define DWC_CFGL_HS_DST_POL	(1 << 18)	/* dst handshake active low */
#define DWC_CFGL_HS_SRC_POL	(1 << 19)	/* src handshake active low */

/* DMA API extensions */
struct dw_cyclic_desc {
	struct dw_desc	**desc;
	unsigned long	periods;
	void		(*period_callback)(void *param);
	void		*period_callback_param;
};

struct dw_cyclic_desc *dw_dma_cyclic_prep(struct dma_chan *chan,
		dma_addr_t buf_addr, size_t buf_len, size_t period_len,
<<<<<<< HEAD
<<<<<<< HEAD
		enum dma_data_direction direction);
=======
		enum dma_transfer_direction direction);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		enum dma_transfer_direction direction);
>>>>>>> refs/remotes/origin/master
void dw_dma_cyclic_free(struct dma_chan *chan);
int dw_dma_cyclic_start(struct dma_chan *chan);
void dw_dma_cyclic_stop(struct dma_chan *chan);

dma_addr_t dw_dma_get_src_addr(struct dma_chan *chan);

dma_addr_t dw_dma_get_dst_addr(struct dma_chan *chan);

#endif /* DW_DMAC_H */
