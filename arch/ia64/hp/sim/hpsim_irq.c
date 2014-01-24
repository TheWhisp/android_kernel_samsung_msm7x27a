/*
 * Platform dependent support for HP simulator.
 *
 * Copyright (C) 1998-2001 Hewlett-Packard Co
 * Copyright (C) 1998-2001 David Mosberger-Tang <davidm@hpl.hp.com>
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/irq.h>

<<<<<<< HEAD
<<<<<<< HEAD
=======
#include "hpsim_ssc.h"

>>>>>>> refs/remotes/origin/cm-10.0
=======
#include "hpsim_ssc.h"

>>>>>>> refs/remotes/origin/master
static unsigned int
hpsim_irq_startup(struct irq_data *data)
{
	return 0;
}

static void
hpsim_irq_noop(struct irq_data *data)
{
}

static int
hpsim_set_affinity_noop(struct irq_data *d, const struct cpumask *b, bool f)
{
	return 0;
}

static struct irq_chip irq_type_hp_sim = {
	.name =			"hpsim",
	.irq_startup =		hpsim_irq_startup,
	.irq_shutdown =		hpsim_irq_noop,
	.irq_enable =		hpsim_irq_noop,
	.irq_disable =		hpsim_irq_noop,
	.irq_ack =		hpsim_irq_noop,
	.irq_set_affinity =	hpsim_set_affinity_noop,
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static void hpsim_irq_set_chip(int irq)
{
	struct irq_chip *chip = irq_get_chip(irq);

	if (chip == &no_irq_chip)
		irq_set_chip(irq, &irq_type_hp_sim);
}

static void hpsim_connect_irq(int intr, int irq)
{
	ia64_ssc(intr, irq, 0, 0, SSC_CONNECT_INTERRUPT);
}

int hpsim_get_irq(int intr)
{
	int irq = assign_irq_vector(AUTO_ASSIGN);

	if (irq >= 0) {
		hpsim_irq_set_chip(irq);
		irq_set_handler(irq, handle_simple_irq);
		hpsim_connect_irq(intr, irq);
	}

	return irq;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
void __init
hpsim_irq_init (void)
{
	int i;

<<<<<<< HEAD
<<<<<<< HEAD
	for_each_active_irq(i) {
		struct irq_chip *chip = irq_get_chip(i);

		if (chip == &no_irq_chip)
			irq_set_chip(i, &irq_type_hp_sim);
	}
=======
	for_each_active_irq(i)
		hpsim_irq_set_chip(i);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	for_each_active_irq(i)
		hpsim_irq_set_chip(i);
>>>>>>> refs/remotes/origin/master
}
