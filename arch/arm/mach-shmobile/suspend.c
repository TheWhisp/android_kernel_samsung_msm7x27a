/*
 * Suspend-to-RAM support code for SH-Mobile ARM
 *
 *  Copyright (C) 2011 Magnus Damm
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#include <linux/pm.h>
#include <linux/suspend.h>
#include <linux/module.h>
#include <linux/err.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/system.h>
#include <asm/io.h>
=======
#include <asm/io.h>
#include <asm/system_misc.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/cpu.h>

#include <asm/io.h>
#include <asm/system_misc.h>
>>>>>>> refs/remotes/origin/master

static int shmobile_suspend_default_enter(suspend_state_t suspend_state)
{
	cpu_do_idle();
	return 0;
}

static int shmobile_suspend_begin(suspend_state_t state)
{
<<<<<<< HEAD
	disable_hlt();
=======
	cpu_idle_poll_ctrl(true);
>>>>>>> refs/remotes/origin/master
	return 0;
}

static void shmobile_suspend_end(void)
{
<<<<<<< HEAD
	enable_hlt();
=======
	cpu_idle_poll_ctrl(false);
>>>>>>> refs/remotes/origin/master
}

struct platform_suspend_ops shmobile_suspend_ops = {
	.begin		= shmobile_suspend_begin,
	.end		= shmobile_suspend_end,
	.enter		= shmobile_suspend_default_enter,
	.valid		= suspend_valid_only_mem,
};

<<<<<<< HEAD
static int __init shmobile_suspend_init(void)
=======
int __init shmobile_suspend_init(void)
>>>>>>> refs/remotes/origin/master
{
	suspend_set_ops(&shmobile_suspend_ops);
	return 0;
}
<<<<<<< HEAD
late_initcall(shmobile_suspend_init);
=======
>>>>>>> refs/remotes/origin/master
