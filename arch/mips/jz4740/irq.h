/*
 *  Copyright (C) 2010, Lars-Peter Clausen <lars@metafoo.de>
 *
 *  This program is free software; you can redistribute it and/or modify it
<<<<<<< HEAD
 *  under  the terms of the GNU General  Public License as published by the
=======
 *  under  the terms of the GNU General	 Public License as published by the
>>>>>>> refs/remotes/origin/master
 *  Free Software Foundation;  either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifndef __MIPS_JZ4740_IRQ_H__
#define __MIPS_JZ4740_IRQ_H__

<<<<<<< HEAD
<<<<<<< HEAD
extern void jz4740_intc_suspend(void);
extern void jz4740_intc_resume(void);
=======
=======
>>>>>>> refs/remotes/origin/master
#include <linux/irq.h>

extern void jz4740_irq_suspend(struct irq_data *data);
extern void jz4740_irq_resume(struct irq_data *data);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#endif
