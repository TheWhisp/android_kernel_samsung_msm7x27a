/*
 * Copyright (C) 2007 Freescale Semiconductor, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 * Prototypes for MPC512x shared code
 */

#ifndef __MPC512X_H__
#define __MPC512X_H__
extern void __init mpc512x_init_IRQ(void);
<<<<<<< HEAD
extern void __init mpc512x_init(void);
extern int __init mpc5121_clk_init(void);
void __init mpc512x_declare_of_platform_devices(void);
extern void mpc512x_restart(char *cmd);
extern void mpc512x_init_diu(void);
extern void mpc512x_setup_diu(void);
=======
extern void __init mpc512x_init_early(void);
extern void __init mpc512x_init(void);
extern void __init mpc512x_setup_arch(void);
extern int __init mpc5121_clk_init(void);
extern const char *mpc512x_select_psc_compat(void);
extern const char *mpc512x_select_reset_compat(void);
extern void mpc512x_restart(char *cmd);

>>>>>>> refs/remotes/origin/master
#endif				/* __MPC512X_H__ */
