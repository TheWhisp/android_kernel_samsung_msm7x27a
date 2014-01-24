/*
 * Copyright (C) 2007 Google, Inc.
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright (c) 2008-2011, The Linux Foundation. All rights reserved.
=======
 * Copyright (c) 2008-2012, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Copyright (c) 2008-2010, Code Aurora Forum. All rights reserved.
>>>>>>> refs/remotes/origin/master
=======
 * Copyright (c) 2008-2012, The Linux Foundation. All rights reserved.
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
 */

#ifndef __ASM_ARCH_MSM_IRQS_H
#define __ASM_ARCH_MSM_IRQS_H

#define MSM_IRQ_BIT(irq)     (1 << ((irq) & 31))

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include "irqs-8625.h"

>>>>>>> refs/remotes/origin/cm-10.0
=======
#include "irqs-8625.h"

>>>>>>> refs/remotes/origin/cm-11.0
#if defined(CONFIG_ARCH_MSM8960) || defined(CONFIG_ARCH_APQ8064) || \
	defined(CONFIG_ARCH_MSM8930)

#ifdef CONFIG_ARCH_MSM8960
#include "irqs-8960.h"
#endif

#ifdef CONFIG_ARCH_MSM8930
#include "irqs-8930.h"
#endif

#ifdef CONFIG_ARCH_APQ8064
#include "irqs-8064.h"
#endif

/* For now, use the maximum number of interrupts until a pending GIC issue
 * is sorted out */
#define NR_MSM_IRQS 288
#define NR_GPIO_IRQS 152
#define NR_PM8921_IRQS 256
<<<<<<< HEAD
<<<<<<< HEAD
#define NR_PM8821_IRQS 64
#define NR_TABLA_IRQS 49
#define NR_GPIO_EXPANDER_IRQS 8
#define NR_BOARD_IRQS (NR_PM8921_IRQS + NR_PM8821_IRQS + \
		NR_TABLA_IRQS + NR_GPIO_EXPANDER_IRQS)
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
#define NR_PM8821_IRQS 112
#define NR_WCD9XXX_IRQS 49
#define NR_TABLA_IRQS NR_WCD9XXX_IRQS
#define NR_GPIO_EXPANDER_IRQS 64
#ifdef CONFIG_PCI_MSI
#define NR_PCIE_MSI_IRQS 256
#define NR_BOARD_IRQS (NR_PM8921_IRQS + NR_PM8821_IRQS + \
		NR_WCD9XXX_IRQS + NR_GPIO_EXPANDER_IRQS + NR_PCIE_MSI_IRQS)
#else
#define NR_BOARD_IRQS (NR_PM8921_IRQS + NR_PM8821_IRQS + \
		NR_WCD9XXX_IRQS + NR_GPIO_EXPANDER_IRQS)
#endif
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
#define NR_TLMM_MSM_DIR_CONN_IRQ 8 /*Need to Verify this Count*/
#define NR_MSM_GPIOS NR_GPIO_IRQS

#else

<<<<<<< HEAD
<<<<<<< HEAD
#if defined(CONFIG_ARCH_MSMCOPPER)
#include "irqs-copper.h"
#elif defined(CONFIG_ARCH_MSM9615)
#include "irqs-9615.h"
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
#if defined(CONFIG_ARCH_MSM8974)
#include "irqs-8974.h"
#elif defined(CONFIG_ARCH_MSM9615)
#include "irqs-9615.h"
#elif defined(CONFIG_ARCH_MSM9625)
#include "irqs-9625.h"
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
#elif defined(CONFIG_ARCH_MSM7X30)
=======
#if defined(CONFIG_ARCH_MSM7X30)
>>>>>>> refs/remotes/origin/master
=======
#elif defined(CONFIG_ARCH_MSM7X30)
>>>>>>> refs/remotes/origin/cm-11.0
#include "irqs-7x30.h"
#elif defined(CONFIG_ARCH_QSD8X50)
#include "irqs-8x50.h"
#include "sirc.h"
<<<<<<< HEAD
#elif defined(CONFIG_ARCH_MSM8X60)
#include "irqs-8x60.h"
#elif defined(CONFIG_ARCH_MSM7X01A) || defined(CONFIG_ARCH_MSM7X25) \
	|| defined(CONFIG_ARCH_MSM7X27)
#include "irqs-7xxx.h"
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0

#define NR_GPIO_IRQS 133
#define NR_MSM_IRQS 256
#define NR_BOARD_IRQS 256
#define NR_MSM_GPIOS NR_GPIO_IRQS
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
#elif defined(CONFIG_ARCH_FSM9XXX)
#include "irqs-fsm9xxx.h"
#include "sirc.h"
=======
#elif defined(CONFIG_ARCH_MSM_ARM11)
#include "irqs-7x00.h"
>>>>>>> refs/remotes/origin/master
=======
#elif defined(CONFIG_ARCH_FSM9XXX)
#include "irqs-fsm9xxx.h"
#include "sirc.h"
>>>>>>> refs/remotes/origin/cm-11.0
#else
#error "Unknown architecture specification"
#endif

<<<<<<< HEAD
<<<<<<< HEAD
#endif

<<<<<<< HEAD
=======
#if !defined(CONFIG_SPARSE_IRQ)
>>>>>>> refs/remotes/origin/cm-10.0
#define NR_IRQS (NR_MSM_IRQS + NR_GPIO_IRQS + NR_BOARD_IRQS)
#define MSM_GPIO_TO_INT(n) (NR_MSM_IRQS + (n))
#define FIRST_GPIO_IRQ MSM_GPIO_TO_INT(0)
#define MSM_INT_TO_REG(base, irq) (base + irq / 32)
<<<<<<< HEAD
=======
#endif

#if defined(CONFIG_PCI_MSI) && defined(CONFIG_MSM_PCIE)
#define MSM_PCIE_MSI_INT(n) (NR_MSM_IRQS + NR_GPIO_IRQS + NR_PM8921_IRQS +  \
		NR_PM8821_IRQS + NR_TABLA_IRQS + NR_GPIO_EXPANDER_IRQS + (n))
#endif
>>>>>>> refs/remotes/origin/cm-10.0
=======
=======
#endif

#if !defined(CONFIG_SPARSE_IRQ)
>>>>>>> refs/remotes/origin/cm-11.0
#define NR_IRQS (NR_MSM_IRQS + NR_GPIO_IRQS + NR_BOARD_IRQS)
#define MSM_GPIO_TO_INT(n) (NR_MSM_IRQS + (n))
#define FIRST_GPIO_IRQ MSM_GPIO_TO_INT(0)
#define MSM_INT_TO_REG(base, irq) (base + irq / 32)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/master
=======
#endif

#if defined(CONFIG_PCI_MSI) && defined(CONFIG_MSM_PCIE)
#define MSM_PCIE_MSI_INT(n) (NR_MSM_IRQS + NR_GPIO_IRQS + NR_PM8921_IRQS +  \
		NR_PM8821_IRQS + NR_TABLA_IRQS + NR_GPIO_EXPANDER_IRQS + (n))
#endif
>>>>>>> refs/remotes/origin/cm-11.0

#endif
