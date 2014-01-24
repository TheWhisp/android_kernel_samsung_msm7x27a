/*
 * PCBIT-D module support
 *
 * Copyright (C) 1996 Universidade de Lisboa
<<<<<<< HEAD
 * 
 * Written by Pedro Roque Marques (roque@di.fc.ul.pt)
 *
 * This software may be used and distributed according to the terms of 
=======
 *
 * Written by Pedro Roque Marques (roque@di.fc.ul.pt)
 *
 * This software may be used and distributed according to the terms of
>>>>>>> refs/remotes/origin/cm-10.0
 * the GNU General Public License, incorporated herein by reference.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/skbuff.h>

#include <linux/isdnif.h>
#include "pcbit.h"

MODULE_DESCRIPTION("ISDN4Linux: Driver for PCBIT-T card");
MODULE_AUTHOR("Pedro Roque Marques");
MODULE_LICENSE("GPL");

static int mem[MAX_PCBIT_CARDS];
static int irq[MAX_PCBIT_CARDS];

module_param_array(mem, int, NULL, 0);
module_param_array(irq, int, NULL, 0);

static int num_boards;
<<<<<<< HEAD
struct pcbit_dev * dev_pcbit[MAX_PCBIT_CARDS];
=======
struct pcbit_dev *dev_pcbit[MAX_PCBIT_CARDS];
>>>>>>> refs/remotes/origin/cm-10.0

static int __init pcbit_init(void)
{
	int board;

	num_boards = 0;

<<<<<<< HEAD
	printk(KERN_NOTICE 
	       "PCBIT-D device driver v 0.5-fjpc0 19991204 - "
	       "Copyright (C) 1996 Universidade de Lisboa\n");

	if (mem[0] || irq[0]) 
	{
		for (board=0; board < MAX_PCBIT_CARDS && mem[board] && irq[board]; board++)
=======
	printk(KERN_NOTICE
	       "PCBIT-D device driver v 0.5-fjpc0 19991204 - "
	       "Copyright (C) 1996 Universidade de Lisboa\n");

	if (mem[0] || irq[0])
	{
		for (board = 0; board < MAX_PCBIT_CARDS && mem[board] && irq[board]; board++)
>>>>>>> refs/remotes/origin/cm-10.0
		{
			if (!mem[board])
				mem[board] = 0xD0000;
			if (!irq[board])
				irq[board] = 5;
<<<<<<< HEAD
			
			if (pcbit_init_dev(board, mem[board], irq[board]) == 0)
				num_boards++;
		
			else 
			{
				printk(KERN_WARNING 
				       "pcbit_init failed for dev %d", 
=======

			if (pcbit_init_dev(board, mem[board], irq[board]) == 0)
				num_boards++;

			else
			{
				printk(KERN_WARNING
				       "pcbit_init failed for dev %d",
>>>>>>> refs/remotes/origin/cm-10.0
				       board + 1);
				return -EIO;
			}
		}
	}

	/* Hardcoded default settings detection */

	if (!num_boards)
	{
<<<<<<< HEAD
		printk(KERN_INFO 
=======
		printk(KERN_INFO
>>>>>>> refs/remotes/origin/cm-10.0
		       "Trying to detect board using default settings\n");
		if (pcbit_init_dev(0, 0xD0000, 5) == 0)
			num_boards++;
		else
			return -EIO;
	}
	return 0;
}

static void __exit pcbit_exit(void)
{
#ifdef MODULE
	int board;

	for (board = 0; board < num_boards; board++)
		pcbit_terminate(board);
<<<<<<< HEAD
	printk(KERN_NOTICE 
=======
	printk(KERN_NOTICE
>>>>>>> refs/remotes/origin/cm-10.0
	       "PCBIT-D module unloaded\n");
#endif
}

#ifndef MODULE
#define MAX_PARA	(MAX_PCBIT_CARDS * 2)
static int __init pcbit_setup(char *line)
{
	int i, j, argc;
	char *str;
<<<<<<< HEAD
	int ints[MAX_PARA+1];
=======
	int ints[MAX_PARA + 1];
>>>>>>> refs/remotes/origin/cm-10.0

	str = get_options(line, MAX_PARA, ints);
	argc = ints[0];
	i = 0;
	j = 1;

<<<<<<< HEAD
	while (argc && (i<MAX_PCBIT_CARDS)) {
=======
	while (argc && (i < MAX_PCBIT_CARDS)) {
>>>>>>> refs/remotes/origin/cm-10.0

		if (argc) {
			mem[i]	= ints[j];
			j++; argc--;
		}
<<<<<<< HEAD
		
=======

>>>>>>> refs/remotes/origin/cm-10.0
		if (argc) {
			irq[i]	= ints[j];
			j++; argc--;
		}

		i++;
	}
<<<<<<< HEAD
	return(1);
=======
	return (1);
>>>>>>> refs/remotes/origin/cm-10.0
}
__setup("pcbit=", pcbit_setup);
#endif

module_init(pcbit_init);
module_exit(pcbit_exit);
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/cm-10.0
