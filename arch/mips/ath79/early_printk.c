/*
<<<<<<< HEAD
<<<<<<< HEAD
 *  Atheros AR71XX/AR724X/AR913X SoC early printk support
 *
 *  Copyright (C) 2008-2010 Gabor Juhos <juhosg@openwrt.org>
=======
 *  Atheros AR7XXX/AR9XXX SoC early printk support
 *
 *  Copyright (C) 2008-2011 Gabor Juhos <juhosg@openwrt.org>
>>>>>>> refs/remotes/origin/cm-10.0
=======
 *  Atheros AR7XXX/AR9XXX SoC early printk support
 *
 *  Copyright (C) 2008-2011 Gabor Juhos <juhosg@openwrt.org>
>>>>>>> refs/remotes/origin/master
 *  Copyright (C) 2008 Imre Kaloz <kaloz@openwrt.org>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <linux/io.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/serial_reg.h>
#include <asm/addrspace.h>

#include <asm/mach-ath79/ar71xx_regs.h>

static inline void prom_wait_thre(void __iomem *base)
{
	u32 lsr;

	do {
		lsr = __raw_readl(base + UART_LSR * 4);
		if (lsr & UART_LSR_THRE)
=======
=======
>>>>>>> refs/remotes/origin/master
#include <linux/errno.h>
#include <linux/serial_reg.h>
#include <asm/addrspace.h>

#include <asm/mach-ath79/ath79.h>
#include <asm/mach-ath79/ar71xx_regs.h>
#include <asm/mach-ath79/ar933x_uart.h>

static void (*_prom_putchar) (unsigned char);

static inline void prom_putchar_wait(void __iomem *reg, u32 mask, u32 val)
{
	u32 t;

	do {
		t = __raw_readl(reg);
		if ((t & mask) == val)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			break;
	} while (1);
}

<<<<<<< HEAD
<<<<<<< HEAD
void prom_putchar(unsigned char ch)
{
	void __iomem *base = (void __iomem *)(KSEG1ADDR(AR71XX_UART_BASE));

	prom_wait_thre(base);
	__raw_writel(ch, base + UART_TX * 4);
	prom_wait_thre(base);
=======
=======
>>>>>>> refs/remotes/origin/master
static void prom_putchar_ar71xx(unsigned char ch)
{
	void __iomem *base = (void __iomem *)(KSEG1ADDR(AR71XX_UART_BASE));

	prom_putchar_wait(base + UART_LSR * 4, UART_LSR_THRE, UART_LSR_THRE);
	__raw_writel(ch, base + UART_TX * 4);
	prom_putchar_wait(base + UART_LSR * 4, UART_LSR_THRE, UART_LSR_THRE);
}

static void prom_putchar_ar933x(unsigned char ch)
{
	void __iomem *base = (void __iomem *)(KSEG1ADDR(AR933X_UART_BASE));

	prom_putchar_wait(base + AR933X_UART_DATA_REG, AR933X_UART_DATA_TX_CSR,
			  AR933X_UART_DATA_TX_CSR);
	__raw_writel(AR933X_UART_DATA_TX_CSR | ch, base + AR933X_UART_DATA_REG);
	prom_putchar_wait(base + AR933X_UART_DATA_REG, AR933X_UART_DATA_TX_CSR,
			  AR933X_UART_DATA_TX_CSR);
}

static void prom_putchar_dummy(unsigned char ch)
{
	/* nothing to do */
}

static void prom_putchar_init(void)
{
	void __iomem *base;
	u32 id;

	base = (void __iomem *)(KSEG1ADDR(AR71XX_RESET_BASE));
	id = __raw_readl(base + AR71XX_RESET_REG_REV_ID);
	id &= REV_ID_MAJOR_MASK;

	switch (id) {
	case REV_ID_MAJOR_AR71XX:
	case REV_ID_MAJOR_AR7240:
	case REV_ID_MAJOR_AR7241:
	case REV_ID_MAJOR_AR7242:
	case REV_ID_MAJOR_AR913X:
<<<<<<< HEAD
=======
	case REV_ID_MAJOR_AR9341:
	case REV_ID_MAJOR_AR9342:
	case REV_ID_MAJOR_AR9344:
	case REV_ID_MAJOR_QCA9556:
	case REV_ID_MAJOR_QCA9558:
>>>>>>> refs/remotes/origin/master
		_prom_putchar = prom_putchar_ar71xx;
		break;

	case REV_ID_MAJOR_AR9330:
	case REV_ID_MAJOR_AR9331:
		_prom_putchar = prom_putchar_ar933x;
		break;

	default:
		_prom_putchar = prom_putchar_dummy;
		break;
	}
}

void prom_putchar(unsigned char ch)
{
	if (!_prom_putchar)
		prom_putchar_init();

	_prom_putchar(ch);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}
