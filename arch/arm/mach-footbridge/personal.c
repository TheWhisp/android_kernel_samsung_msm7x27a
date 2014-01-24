/*
 * linux/arch/arm/mach-footbridge/personal.c
 *
 * Personal server (Skiff) machine fixup
 */
#include <linux/init.h>
#include <linux/spinlock.h>

#include <asm/hardware/dec21285.h>
#include <asm/mach-types.h>

#include <asm/mach/arch.h>

#include "common.h"

MACHINE_START(PERSONAL_SERVER, "Compaq-PersonalServer")
	/* Maintainer: Jamey Hicks / George France */
<<<<<<< HEAD
<<<<<<< HEAD
	.boot_params	= 0x00000100,
	.map_io		= footbridge_map_io,
	.init_irq	= footbridge_init_irq,
	.timer		= &footbridge_timer,
=======
	.atag_offset	= 0x100,
	.map_io		= footbridge_map_io,
	.init_irq	= footbridge_init_irq,
	.timer		= &footbridge_timer,
	.restart	= footbridge_restart,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.atag_offset	= 0x100,
	.map_io		= footbridge_map_io,
	.init_irq	= footbridge_init_irq,
	.init_time	= footbridge_timer_init,
	.restart	= footbridge_restart,
>>>>>>> refs/remotes/origin/master
MACHINE_END

