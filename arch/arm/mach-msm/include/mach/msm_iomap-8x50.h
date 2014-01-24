/*
 * Copyright (C) 2007 Google, Inc.
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright (c) 2008-2010, The Linux Foundation. All rights reserved.
=======
 * Copyright (c) 2008-2011 Code Aurora Forum. All rights reserved.
>>>>>>> refs/remotes/origin/master
=======
 * Copyright (c) 2008-2010, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-11.0
 * Author: Brian Swetland <swetland@google.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *
 * The MSM peripherals are spread all over across 768MB of physical
 * space, which makes just having a simple IO_ADDRESS macro to slide
 * them into the right virtual location rough.  Instead, we will
 * provide a master phys->virt mapping for peripherals here.
 *
 */

#ifndef __ASM_ARCH_MSM_IOMAP_8X50_H
#define __ASM_ARCH_MSM_IOMAP_8X50_H

/* Physical base address and size of peripherals.
 * Ordered by the virtual base addresses they will be mapped at.
 *
 * MSM_VIC_BASE must be an value that can be loaded via a "mov"
 * instruction, otherwise entry-macro.S will not compile.
 *
 * If you add or remove entries here, you'll want to edit the
 * msm_io_desc array in arch/arm/mach-msm/io.c to reflect your
 * changes.
 *
 */

<<<<<<< HEAD
<<<<<<< HEAD
#define MSM_VIC_BASE          IOMEM(0xFA000000)
#define MSM_VIC_PHYS          0xAC000000
#define MSM_VIC_SIZE          SZ_4K

#define MSM_CSR_BASE          IOMEM(0xFA001000)
#define MSM_CSR_PHYS          0xAC100000
#define MSM_CSR_SIZE          SZ_4K

#define MSM_TMR_PHYS          MSM_CSR_PHYS
#define MSM_TMR_BASE          MSM_CSR_BASE
#define MSM_TMR_SIZE          SZ_4K

#define MSM_GPIO1_BASE        IOMEM(0xFA003000)
#define MSM_GPIO1_PHYS        0xA9000000
#define MSM_GPIO1_SIZE        SZ_4K

#define MSM_GPIO2_BASE        IOMEM(0xFA004000)
#define MSM_GPIO2_PHYS        0xA9100000
#define MSM_GPIO2_SIZE        SZ_4K

#define MSM_CLK_CTL_BASE      IOMEM(0xFA005000)
#define MSM_CLK_CTL_PHYS      0xA8600000
#define MSM_CLK_CTL_SIZE      SZ_4K

#define MSM_SIRC_BASE         IOMEM(0xFB006000)
#define MSM_SIRC_PHYS         0xAC200000
#define MSM_SIRC_SIZE         SZ_4K

#define MSM_SCPLL_BASE        IOMEM(0xFB007000)
#define MSM_SCPLL_PHYS        0xA8800000
#define MSM_SCPLL_SIZE        SZ_4K

#define MSM_TCSR_BASE         IOMEM(0xFB008000)
#define MSM_TCSR_PHYS         0xA8700000
#define MSM_TCSR_SIZE         SZ_4K

#define MSM_SHARED_RAM_BASE   IOMEM(0xFA100000)
=======
#define MSM_VIC_BASE          IOMEM(0xE0000000)
=======
#define MSM_VIC_BASE          IOMEM(0xFA000000)
>>>>>>> refs/remotes/origin/cm-11.0
#define MSM_VIC_PHYS          0xAC000000
#define MSM_VIC_SIZE          SZ_4K

#define MSM_CSR_BASE          IOMEM(0xFA001000)
#define MSM_CSR_PHYS          0xAC100000
#define MSM_CSR_SIZE          SZ_4K

#define MSM_TMR_PHYS          MSM_CSR_PHYS
#define MSM_TMR_BASE          MSM_CSR_BASE
#define MSM_TMR_SIZE          SZ_4K

#define MSM_GPIO1_BASE        IOMEM(0xFA003000)
#define MSM_GPIO1_PHYS        0xA9000000
#define MSM_GPIO1_SIZE        SZ_4K

#define MSM_GPIO2_BASE        IOMEM(0xFA004000)
#define MSM_GPIO2_PHYS        0xA9100000
#define MSM_GPIO2_SIZE        SZ_4K

#define MSM_CLK_CTL_BASE      IOMEM(0xFA005000)
#define MSM_CLK_CTL_PHYS      0xA8600000
#define MSM_CLK_CTL_SIZE      SZ_4K

#define MSM_SIRC_BASE         IOMEM(0xFB006000)
#define MSM_SIRC_PHYS         0xAC200000
#define MSM_SIRC_SIZE         SZ_4K

#define MSM_SCPLL_BASE        IOMEM(0xFB007000)
#define MSM_SCPLL_PHYS        0xA8800000
#define MSM_SCPLL_SIZE        SZ_4K

#define MSM_TCSR_BASE         IOMEM(0xFB008000)
#define MSM_TCSR_PHYS         0xA8700000
#define MSM_TCSR_SIZE         SZ_4K

<<<<<<< HEAD
#define MSM_SHARED_RAM_BASE   IOMEM(0xE0100000)
#define MSM_SHARED_RAM_PHYS (MSM_SMI_BASE + 0x00100000)
>>>>>>> refs/remotes/origin/master
=======
#define MSM_SHARED_RAM_BASE   IOMEM(0xFA100000)
>>>>>>> refs/remotes/origin/cm-11.0
#define MSM_SHARED_RAM_SIZE   SZ_1M

#define MSM_UART1_PHYS        0xA9A00000
#define MSM_UART1_SIZE        SZ_4K

#define MSM_UART2_PHYS        0xA9B00000
#define MSM_UART2_SIZE        SZ_4K

#define MSM_UART3_PHYS        0xA9C00000
#define MSM_UART3_SIZE        SZ_4K

<<<<<<< HEAD
<<<<<<< HEAD
#define MSM_MDC_BASE	      IOMEM(0xFA200000)
#define MSM_MDC_PHYS	      0xAA500000
#define MSM_MDC_SIZE	      SZ_1M

#define MSM_AD5_BASE          IOMEM(0xFA300000)
#define MSM_AD5_PHYS          0xAC000000
#define MSM_AD5_SIZE          (SZ_1M*13)

=======
#define MSM_MDC_BASE	      IOMEM(0xE0200000)
=======
#define MSM_MDC_BASE	      IOMEM(0xFA200000)
>>>>>>> refs/remotes/origin/cm-11.0
#define MSM_MDC_PHYS	      0xAA500000
#define MSM_MDC_SIZE	      SZ_1M

#define MSM_AD5_BASE          IOMEM(0xFA300000)
#define MSM_AD5_PHYS          0xAC000000
#define MSM_AD5_SIZE          (SZ_1M*13)

<<<<<<< HEAD

#define MSM_I2C_SIZE          SZ_4K
#define MSM_I2C_PHYS          0xA9900000

#define MSM_HSUSB_PHYS        0xA0800000
#define MSM_HSUSB_SIZE        SZ_1K

#define MSM_NAND_PHYS           0xA0A00000


#define MSM_TSIF_PHYS        (0xa0100000)
#define MSM_TSIF_SIZE        (0x200)

#define MSM_TSSC_PHYS         0xAA300000

#define MSM_UART1DM_PHYS      0xA0200000
#define MSM_UART2DM_PHYS      0xA0900000


#define MSM_SDC1_PHYS          0xA0300000
#define MSM_SDC1_SIZE          SZ_4K

#define MSM_SDC2_PHYS          0xA0400000
#define MSM_SDC2_SIZE          SZ_4K

#define MSM_SDC3_PHYS          0xA0500000
#define MSM_SDC3_SIZE           SZ_4K

#define MSM_SDC4_PHYS          0xA0600000
#define MSM_SDC4_SIZE          SZ_4K

>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
#endif
