/*
 * omap-mcpdm.h
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright (C) 2009 Texas Instruments
 *
 * Contact: Misael Lopez Cruz <x0052729@ti.com>
=======
 * Copyright (C) 2009 - 2011 Texas Instruments
 *
 * Contact: Misael Lopez Cruz <misael.lopez@ti.com>
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Copyright (C) 2009 - 2011 Texas Instruments
 *
 * Contact: Misael Lopez Cruz <misael.lopez@ti.com>
>>>>>>> refs/remotes/origin/master
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

#ifndef __OMAP_MCPDM_H__
#define __OMAP_MCPDM_H__

<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/platform_device.h>

#define MCPDM_REVISION		0x00
#define MCPDM_SYSCONFIG		0x10
#define MCPDM_IRQSTATUS_RAW	0x24
#define MCPDM_IRQSTATUS		0x28
#define MCPDM_IRQENABLE_SET	0x2C
#define MCPDM_IRQENABLE_CLR	0x30
#define MCPDM_IRQWAKE_EN	0x34
#define MCPDM_DMAENABLE_SET	0x38
#define MCPDM_DMAENABLE_CLR	0x3C
#define MCPDM_DMAWAKEEN		0x40
#define MCPDM_CTRL		0x44
#define MCPDM_DN_DATA		0x48
#define MCPDM_UP_DATA		0x4C
#define MCPDM_FIFO_CTRL_DN	0x50
#define MCPDM_FIFO_CTRL_UP	0x54
#define MCPDM_DN_OFFSET		0x58
=======
=======
>>>>>>> refs/remotes/origin/master
#define MCPDM_REG_REVISION		0x00
#define MCPDM_REG_SYSCONFIG		0x10
#define MCPDM_REG_IRQSTATUS_RAW		0x24
#define MCPDM_REG_IRQSTATUS		0x28
#define MCPDM_REG_IRQENABLE_SET		0x2C
#define MCPDM_REG_IRQENABLE_CLR		0x30
#define MCPDM_REG_IRQWAKE_EN		0x34
#define MCPDM_REG_DMAENABLE_SET		0x38
#define MCPDM_REG_DMAENABLE_CLR		0x3C
#define MCPDM_REG_DMAWAKEEN		0x40
#define MCPDM_REG_CTRL			0x44
#define MCPDM_REG_DN_DATA		0x48
#define MCPDM_REG_UP_DATA		0x4C
#define MCPDM_REG_FIFO_CTRL_DN		0x50
#define MCPDM_REG_FIFO_CTRL_UP		0x54
#define MCPDM_REG_DN_OFFSET		0x58
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

/*
 * MCPDM_IRQ bit fields
 * IRQSTATUS_RAW, IRQSTATUS, IRQENABLE_SET, IRQENABLE_CLR
 */

#define MCPDM_DN_IRQ			(1 << 0)
#define MCPDM_DN_IRQ_EMPTY		(1 << 1)
#define MCPDM_DN_IRQ_ALMST_EMPTY	(1 << 2)
#define MCPDM_DN_IRQ_FULL		(1 << 3)

#define MCPDM_UP_IRQ			(1 << 8)
#define MCPDM_UP_IRQ_EMPTY		(1 << 9)
#define MCPDM_UP_IRQ_ALMST_FULL		(1 << 10)
#define MCPDM_UP_IRQ_FULL		(1 << 11)

#define MCPDM_DOWNLINK_IRQ_MASK		0x00F
#define MCPDM_UPLINK_IRQ_MASK		0xF00

/*
 * MCPDM_DMAENABLE bit fields
 */

<<<<<<< HEAD
<<<<<<< HEAD
#define DMA_DN_ENABLE		0x1
#define DMA_UP_ENABLE		0x2
=======
#define MCPDM_DMA_DN_ENABLE		(1 << 0)
#define MCPDM_DMA_UP_ENABLE		(1 << 1)
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define MCPDM_DMA_DN_ENABLE		(1 << 0)
#define MCPDM_DMA_UP_ENABLE		(1 << 1)
>>>>>>> refs/remotes/origin/master

/*
 * MCPDM_CTRL bit fields
 */

<<<<<<< HEAD
<<<<<<< HEAD
#define PDM_UP1_EN		0x0001
#define PDM_UP2_EN		0x0002
#define PDM_UP3_EN		0x0004
#define PDM_DN1_EN		0x0008
#define PDM_DN2_EN		0x0010
#define PDM_DN3_EN		0x0020
#define PDM_DN4_EN		0x0040
#define PDM_DN5_EN		0x0080
#define PDMOUTFORMAT		0x0100
#define CMD_INT			0x0200
#define STATUS_INT		0x0400
#define SW_UP_RST		0x0800
#define SW_DN_RST		0x1000
#define WD_EN			0x4000
#define PDM_UP_MASK		0x007
#define PDM_DN_MASK		0x0F8
#define PDM_CMD_MASK		0x200
#define PDM_STATUS_MASK		0x400


#define PDMOUTFORMAT_LJUST	(0 << 8)
#define PDMOUTFORMAT_RJUST	(1 << 8)
=======
=======
>>>>>>> refs/remotes/origin/master
#define MCPDM_PDM_UPLINK_EN(x)		(1 << (x - 1)) /* ch1 is at bit 0 */
#define MCPDM_PDM_DOWNLINK_EN(x)	(1 << (x + 2)) /* ch1 is at bit 3 */
#define MCPDM_PDMOUTFORMAT		(1 << 8)
#define MCPDM_CMD_INT			(1 << 9)
#define MCPDM_STATUS_INT		(1 << 10)
#define MCPDM_SW_UP_RST			(1 << 11)
#define MCPDM_SW_DN_RST			(1 << 12)
#define MCPDM_WD_EN			(1 << 14)
#define MCPDM_PDM_UP_MASK		0x7
#define MCPDM_PDM_DN_MASK		(0x1f << 3)


#define MCPDM_PDMOUTFORMAT_LJUST	(0 << 8)
#define MCPDM_PDMOUTFORMAT_RJUST	(1 << 8)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

/*
 * MCPDM_FIFO_CTRL bit fields
 */

<<<<<<< HEAD
<<<<<<< HEAD
#define UP_THRES_MAX		0xF
#define DN_THRES_MAX		0xF
=======
#define MCPDM_UP_THRES_MAX		0xF
#define MCPDM_DN_THRES_MAX		0xF
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define MCPDM_UP_THRES_MAX		0xF
#define MCPDM_DN_THRES_MAX		0xF
>>>>>>> refs/remotes/origin/master

/*
 * MCPDM_DN_OFFSET bit fields
 */

<<<<<<< HEAD
<<<<<<< HEAD
#define DN_OFST_RX1_EN		0x0001
#define DN_OFST_RX2_EN		0x0100

#define DN_OFST_RX1		1
#define DN_OFST_RX2		9
#define DN_OFST_MAX		0x1F

#define MCPDM_UPLINK		1
#define MCPDM_DOWNLINK		2
=======
=======
>>>>>>> refs/remotes/origin/master
#define MCPDM_DN_OFST_RX1_EN		(1 << 0)
#define MCPDM_DNOFST_RX1(x)		((x & 0x1f) << 1)
#define MCPDM_DN_OFST_RX2_EN		(1 << 8)
#define MCPDM_DNOFST_RX2(x)		((x & 0x1f) << 9)

void omap_mcpdm_configure_dn_offsets(struct snd_soc_pcm_runtime *rtd,
				    u8 rx1, u8 rx2);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#endif	/* End of __OMAP_MCPDM_H__ */
