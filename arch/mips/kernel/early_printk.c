/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2002, 2003, 06, 07 Ralf Baechle (ralf@linux-mips.org)
 * Copyright (C) 2007 MIPS Technologies, Inc.
 *   written by Ralf Baechle (ralf@linux-mips.org)
 */
<<<<<<< HEAD
#include <linux/console.h>
=======
#include <linux/kernel.h>
#include <linux/console.h>
#include <linux/printk.h>
>>>>>>> refs/remotes/origin/master
#include <linux/init.h>

#include <asm/setup.h>

extern void prom_putchar(char);

<<<<<<< HEAD
static void __init
early_console_write(struct console *con, const char *s, unsigned n)
=======
static void early_console_write(struct console *con, const char *s, unsigned n)
>>>>>>> refs/remotes/origin/master
{
	while (n-- && *s) {
		if (*s == '\n')
			prom_putchar('\r');
		prom_putchar(*s);
		s++;
	}
}

<<<<<<< HEAD
static struct console early_console __initdata = {
=======
static struct console early_console_prom = {
>>>>>>> refs/remotes/origin/master
	.name	= "early",
	.write	= early_console_write,
	.flags	= CON_PRINTBUFFER | CON_BOOT,
	.index	= -1
};

<<<<<<< HEAD
static int early_console_initialized __initdata;

void __init setup_early_printk(void)
{
	if (early_console_initialized)
		return;
	early_console_initialized = 1;

	register_console(&early_console);
=======
void __init setup_early_printk(void)
{
	if (early_console)
		return;
	early_console = &early_console_prom;

	register_console(&early_console_prom);
>>>>>>> refs/remotes/origin/master
}
