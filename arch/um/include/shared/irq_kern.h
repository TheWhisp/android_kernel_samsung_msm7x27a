/*
 * Copyright (C) 2001, 2002 Jeff Dike (jdike@karaya.com)
 * Licensed under the GPL
 */

#ifndef __IRQ_KERN_H__
#define __IRQ_KERN_H__

<<<<<<< HEAD
#include "linux/interrupt.h"
#include "asm/ptrace.h"
=======
#include <linux/interrupt.h>
#include <asm/ptrace.h>
>>>>>>> refs/remotes/origin/master

extern int um_request_irq(unsigned int irq, int fd, int type,
			  irq_handler_t handler,
			  unsigned long irqflags,  const char * devname,
			  void *dev_id);
<<<<<<< HEAD

=======
void um_free_irq(unsigned int irq, void *dev);
>>>>>>> refs/remotes/origin/master
#endif

