/*
 *
 * arch/arm/mach-u300/include/mach/irqs.h
 *
 *
 * Copyright (C) 2006-2009 ST-Ericsson AB
 * License terms: GNU General Public License (GPL) version 2
 * IRQ channel definitions for the U300 platforms.
 * Author: Linus Walleij <linus.walleij@stericsson.com>
 */

#ifndef __MACH_IRQS_H
#define __MACH_IRQS_H

<<<<<<< HEAD
#define IRQ_U300_INTCON0_START		0
#define IRQ_U300_INTCON1_START		32
/* These are on INTCON0 - 30 lines */
#define IRQ_U300_IRQ0_EXT		0
#define IRQ_U300_IRQ1_EXT		1
#define IRQ_U300_DMA			2
#define IRQ_U300_VIDEO_ENC_0		3
#define IRQ_U300_VIDEO_ENC_1		4
#define IRQ_U300_AAIF_RX		5
#define IRQ_U300_AAIF_TX		6
#define IRQ_U300_AAIF_VGPIO		7
#define IRQ_U300_AAIF_WAKEUP		8
#define IRQ_U300_PCM_I2S0_FRAME		9
#define IRQ_U300_PCM_I2S0_FIFO		10
#define IRQ_U300_PCM_I2S1_FRAME		11
#define IRQ_U300_PCM_I2S1_FIFO		12
#define IRQ_U300_XGAM_GAMCON		13
#define IRQ_U300_XGAM_CDI		14
#define IRQ_U300_XGAM_CDICON		15
#if defined(CONFIG_MACH_U300_BS2X) || defined(CONFIG_MACH_U300_BS330)
/* MMIACC not used on the DB3210 or DB3350 chips */
#define IRQ_U300_XGAM_MMIACC		16
#endif
#define IRQ_U300_XGAM_PDI		17
#define IRQ_U300_XGAM_PDICON		18
#define IRQ_U300_XGAM_GAMEACC		19
#define IRQ_U300_XGAM_MCIDCT		20
#define IRQ_U300_APEX			21
#define IRQ_U300_UART0			22
#define IRQ_U300_SPI			23
#define IRQ_U300_TIMER_APP_OS		24
#define IRQ_U300_TIMER_APP_DD		25
#define IRQ_U300_TIMER_APP_GP1		26
#define IRQ_U300_TIMER_APP_GP2		27
#define IRQ_U300_TIMER_OS		28
#define IRQ_U300_TIMER_MS		29
#define IRQ_U300_KEYPAD_KEYBF		30
#define IRQ_U300_KEYPAD_KEYBR		31
/* These are on INTCON1 - 32 lines */
#define IRQ_U300_GPIO_PORT0		32
#define IRQ_U300_GPIO_PORT1		33
#define IRQ_U300_GPIO_PORT2		34
=======
#define IRQ_U300_INTCON0_START		1
#define IRQ_U300_INTCON1_START		33
/* These are on INTCON0 - 30 lines */
#define IRQ_U300_IRQ0_EXT		1
#define IRQ_U300_IRQ1_EXT		2
#define IRQ_U300_DMA			3
#define IRQ_U300_VIDEO_ENC_0		4
#define IRQ_U300_VIDEO_ENC_1		5
#define IRQ_U300_AAIF_RX		6
#define IRQ_U300_AAIF_TX		7
#define IRQ_U300_AAIF_VGPIO		8
#define IRQ_U300_AAIF_WAKEUP		9
#define IRQ_U300_PCM_I2S0_FRAME		10
#define IRQ_U300_PCM_I2S0_FIFO		11
#define IRQ_U300_PCM_I2S1_FRAME		12
#define IRQ_U300_PCM_I2S1_FIFO		13
#define IRQ_U300_XGAM_GAMCON		14
#define IRQ_U300_XGAM_CDI		15
#define IRQ_U300_XGAM_CDICON		16
#if defined(CONFIG_MACH_U300_BS2X) || defined(CONFIG_MACH_U300_BS330)
/* MMIACC not used on the DB3210 or DB3350 chips */
#define IRQ_U300_XGAM_MMIACC		17
#endif
#define IRQ_U300_XGAM_PDI		18
#define IRQ_U300_XGAM_PDICON		19
#define IRQ_U300_XGAM_GAMEACC		20
#define IRQ_U300_XGAM_MCIDCT		21
#define IRQ_U300_APEX			22
#define IRQ_U300_UART0			23
#define IRQ_U300_SPI			24
#define IRQ_U300_TIMER_APP_OS		25
#define IRQ_U300_TIMER_APP_DD		26
#define IRQ_U300_TIMER_APP_GP1		27
#define IRQ_U300_TIMER_APP_GP2		28
#define IRQ_U300_TIMER_OS		29
#define IRQ_U300_TIMER_MS		30
#define IRQ_U300_KEYPAD_KEYBF		31
#define IRQ_U300_KEYPAD_KEYBR		32
/* These are on INTCON1 - 32 lines */
#define IRQ_U300_GPIO_PORT0		33
#define IRQ_U300_GPIO_PORT1		34
#define IRQ_U300_GPIO_PORT2		35
>>>>>>> refs/remotes/origin/cm-10.0

#if defined(CONFIG_MACH_U300_BS2X) || defined(CONFIG_MACH_U300_BS330) || \
    defined(CONFIG_MACH_U300_BS335)
/* These are for DB3150, DB3200 and DB3350 */
<<<<<<< HEAD
#define IRQ_U300_WDOG			35
#define IRQ_U300_EVHIST			36
#define IRQ_U300_MSPRO			37
#define IRQ_U300_MMCSD_MCIINTR0		38
#define IRQ_U300_MMCSD_MCIINTR1		39
#define IRQ_U300_I2C0			40
#define IRQ_U300_I2C1			41
#define IRQ_U300_RTC			42
#define IRQ_U300_NFIF			43
#define IRQ_U300_NFIF2			44
=======
#define IRQ_U300_WDOG			36
#define IRQ_U300_EVHIST			37
#define IRQ_U300_MSPRO			38
#define IRQ_U300_MMCSD_MCIINTR0		39
#define IRQ_U300_MMCSD_MCIINTR1		40
#define IRQ_U300_I2C0			41
#define IRQ_U300_I2C1			42
#define IRQ_U300_RTC			43
#define IRQ_U300_NFIF			44
#define IRQ_U300_NFIF2			45
>>>>>>> refs/remotes/origin/cm-10.0
#endif

/* DB3150 and DB3200 have only 45 IRQs */
#if defined(CONFIG_MACH_U300_BS2X) || defined(CONFIG_MACH_U300_BS330)
<<<<<<< HEAD
#define U300_NR_IRQS			45
=======
#define U300_VIC_IRQS_END		46
>>>>>>> refs/remotes/origin/cm-10.0
#endif

/* The DB3350-specific interrupt lines */
#ifdef CONFIG_MACH_U300_BS335
<<<<<<< HEAD
#define IRQ_U300_ISP_F0			45
#define IRQ_U300_ISP_F1			46
#define IRQ_U300_ISP_F2			47
#define IRQ_U300_ISP_F3			48
#define IRQ_U300_ISP_F4			49
#define IRQ_U300_GPIO_PORT3		50
#define IRQ_U300_SYSCON_PLL_LOCK	51
#define IRQ_U300_UART1			52
#define IRQ_U300_GPIO_PORT4		53
#define IRQ_U300_GPIO_PORT5		54
#define IRQ_U300_GPIO_PORT6		55
#define U300_NR_IRQS			56
=======
#define IRQ_U300_ISP_F0			46
#define IRQ_U300_ISP_F1			47
#define IRQ_U300_ISP_F2			48
#define IRQ_U300_ISP_F3			49
#define IRQ_U300_ISP_F4			50
#define IRQ_U300_GPIO_PORT3		51
#define IRQ_U300_SYSCON_PLL_LOCK	52
#define IRQ_U300_UART1			53
#define IRQ_U300_GPIO_PORT4		54
#define IRQ_U300_GPIO_PORT5		55
#define IRQ_U300_GPIO_PORT6		56
#define U300_VIC_IRQS_END		57
>>>>>>> refs/remotes/origin/cm-10.0
#endif

/* The DB3210-specific interrupt lines */
#ifdef CONFIG_MACH_U300_BS365
<<<<<<< HEAD
#define IRQ_U300_GPIO_PORT3		35
#define IRQ_U300_GPIO_PORT4		36
#define IRQ_U300_WDOG			37
#define IRQ_U300_EVHIST			38
#define IRQ_U300_MSPRO			39
#define IRQ_U300_MMCSD_MCIINTR0		40
#define IRQ_U300_MMCSD_MCIINTR1		41
#define IRQ_U300_I2C0			42
#define IRQ_U300_I2C1			43
#define IRQ_U300_RTC			44
#define IRQ_U300_NFIF			45
#define IRQ_U300_NFIF2			46
#define IRQ_U300_SYSCON_PLL_LOCK	47
#define U300_NR_IRQS			48
#endif

#ifdef CONFIG_AB3550_CORE
#define IRQ_AB3550_BASE			(U300_NR_IRQS)
#define IRQ_AB3550_END			(IRQ_AB3550_BASE + 37)

#define NR_IRQS				(IRQ_AB3550_END + 1)
#else
#define NR_IRQS U300_NR_IRQS
#endif

=======
#define IRQ_U300_GPIO_PORT3		36
#define IRQ_U300_GPIO_PORT4		37
#define IRQ_U300_WDOG			38
#define IRQ_U300_EVHIST			39
#define IRQ_U300_MSPRO			40
#define IRQ_U300_MMCSD_MCIINTR0		41
#define IRQ_U300_MMCSD_MCIINTR1		42
#define IRQ_U300_I2C0			43
#define IRQ_U300_I2C1			44
#define IRQ_U300_RTC			45
#define IRQ_U300_NFIF			46
#define IRQ_U300_NFIF2			47
#define IRQ_U300_SYSCON_PLL_LOCK	48
#define U300_VIC_IRQS_END		49
#endif

/* Maximum 8*7 GPIO lines */
#ifdef CONFIG_PINCTRL_COH901
#define IRQ_U300_GPIO_BASE		(U300_VIC_IRQS_END)
#define IRQ_U300_GPIO_END		(IRQ_U300_GPIO_BASE + 56)
#else
#define IRQ_U300_GPIO_END		(U300_VIC_IRQS_END)
#endif

#define NR_IRQS				(IRQ_U300_GPIO_END - IRQ_U300_INTCON0_START)

>>>>>>> refs/remotes/origin/cm-10.0
#endif
