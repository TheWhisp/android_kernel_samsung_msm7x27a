/*
 * Copyright 2000 Deep Blue Solutions Ltd
 * Copyright 2004 ARM Limited
 * Copyright 2008 Cavium Networks
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, Version 2, as
 * published by the Free Software Foundation.
 */

#ifndef __CNS3XXX_CORE_H
#define __CNS3XXX_CORE_H

<<<<<<< HEAD
extern struct sys_timer cns3xxx_timer;

<<<<<<< HEAD
void __init cns3xxx_map_io(void);
void __init cns3xxx_init_irq(void);
void cns3xxx_power_off(void);
=======
=======
#include <linux/reboot.h>

extern void cns3xxx_timer_init(void);

>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_CACHE_L2X0
void __init cns3xxx_l2x0_init(void);
#else
static inline void cns3xxx_l2x0_init(void) {}
#endif /* CONFIG_CACHE_L2X0 */

void __init cns3xxx_map_io(void);
void __init cns3xxx_init_irq(void);
void cns3xxx_power_off(void);
<<<<<<< HEAD
void cns3xxx_restart(char, const char *);
>>>>>>> refs/remotes/origin/cm-10.0
=======
void cns3xxx_restart(enum reboot_mode, const char *);
>>>>>>> refs/remotes/origin/master

#endif /* __CNS3XXX_CORE_H */
