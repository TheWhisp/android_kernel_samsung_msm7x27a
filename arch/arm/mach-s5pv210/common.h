/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com
 *
 * Common Header for S5PV210 machines
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __ARCH_ARM_MACH_S5PV210_COMMON_H
#define __ARCH_ARM_MACH_S5PV210_COMMON_H

<<<<<<< HEAD
=======
#include <linux/reboot.h>

>>>>>>> refs/remotes/origin/master
void s5pv210_init_io(struct map_desc *mach_desc, int size);
void s5pv210_init_irq(void);

void s5pv210_register_clocks(void);
void s5pv210_setup_clocks(void);

<<<<<<< HEAD
void s5pv210_restart(char mode, const char *cmd);

#ifdef CONFIG_CPU_S5PV210
=======
void s5pv210_restart(enum reboot_mode mode, const char *cmd);
>>>>>>> refs/remotes/origin/master

extern  int s5pv210_init(void);
extern void s5pv210_map_io(void);
extern void s5pv210_init_clocks(int xtal);
extern void s5pv210_init_uarts(struct s3c2410_uartcfg *cfg, int no);

<<<<<<< HEAD
#else
#define s5pv210_init_clocks NULL
#define s5pv210_init_uarts NULL
#define s5pv210_map_io NULL
#define s5pv210_init NULL
#endif

=======
>>>>>>> refs/remotes/origin/master
#endif /* __ARCH_ARM_MACH_S5PV210_COMMON_H */
