/*
 * Copyright (C) 2011 Richard Weinberger <richrd@nod.at>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/console.h>
#include <linux/init.h>
<<<<<<< HEAD
#include "os.h"
=======
#include <os.h>
>>>>>>> refs/remotes/origin/master

static void early_console_write(struct console *con, const char *s, unsigned int n)
{
	um_early_printk(s, n);
}

<<<<<<< HEAD
static struct console early_console = {
=======
static struct console early_console_dev = {
>>>>>>> refs/remotes/origin/master
	.name = "earlycon",
	.write = early_console_write,
	.flags = CON_BOOT,
	.index = -1,
};

static int __init setup_early_printk(char *buf)
{
<<<<<<< HEAD
	register_console(&early_console);

=======
	if (!early_console) {
		early_console = &early_console_dev;
		register_console(&early_console_dev);
	}
>>>>>>> refs/remotes/origin/master
	return 0;
}

early_param("earlyprintk", setup_early_printk);
