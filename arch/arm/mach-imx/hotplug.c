/*
 * Copyright 2011 Freescale Semiconductor, Inc.
 * Copyright 2011 Linaro Ltd.
 *
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

#include <linux/errno.h>
<<<<<<< HEAD
#include <asm/cacheflush.h>
#include <asm/cp15.h>
#include <mach/common.h>

int platform_cpu_kill(unsigned int cpu)
{
	return 1;
}
=======
#include <linux/jiffies.h>
#include <asm/cp15.h>
#include <asm/proc-fns.h>

#include "common.h"
>>>>>>> refs/remotes/origin/master

static inline void cpu_enter_lowpower(void)
{
	unsigned int v;

<<<<<<< HEAD
	flush_cache_all();
=======
>>>>>>> refs/remotes/origin/master
	asm volatile(
		"mcr	p15, 0, %1, c7, c5, 0\n"
	"	mcr	p15, 0, %1, c7, c10, 4\n"
	/*
	 * Turn off coherency
	 */
	"	mrc	p15, 0, %0, c1, c0, 1\n"
	"	bic	%0, %0, %3\n"
	"	mcr	p15, 0, %0, c1, c0, 1\n"
	"	mrc	p15, 0, %0, c1, c0, 0\n"
	"	bic	%0, %0, %2\n"
	"	mcr	p15, 0, %0, c1, c0, 0\n"
	  : "=&r" (v)
	  : "r" (0), "Ir" (CR_C), "Ir" (0x40)
	  : "cc");
}

/*
 * platform-specific code to shutdown a CPU
 *
 * Called with IRQs disabled
 */
<<<<<<< HEAD
void platform_cpu_die(unsigned int cpu)
{
	cpu_enter_lowpower();
	imx_enable_cpu(cpu, false);

	/* spin here until hardware takes it down */
	while (1)
		;
}

int platform_cpu_disable(unsigned int cpu)
{
	/*
	 * we don't allow CPU 0 to be shutdown (it is still too special
	 * e.g. clock tick interrupts)
	 */
	return cpu == 0 ? -EPERM : 0;
=======
void imx_cpu_die(unsigned int cpu)
{
	cpu_enter_lowpower();
	/*
	 * We use the cpu jumping argument register to sync with
	 * imx_cpu_kill() which is running on cpu0 and waiting for
	 * the register being cleared to kill the cpu.
	 */
	imx_set_cpu_arg(cpu, ~0);

	while (1)
		cpu_do_idle();
}

int imx_cpu_kill(unsigned int cpu)
{
	unsigned long timeout = jiffies + msecs_to_jiffies(50);

	while (imx_get_cpu_arg(cpu) == 0)
		if (time_after(jiffies, timeout))
			return 0;
	imx_enable_cpu(cpu, false);
	imx_set_cpu_arg(cpu, 0);
	return 1;
>>>>>>> refs/remotes/origin/master
}
