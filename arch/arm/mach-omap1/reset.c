/*
 * OMAP1 reset support
 */
#include <linux/kernel.h>
#include <linux/io.h>

<<<<<<< HEAD
#include <mach/hardware.h>
#include <mach/system.h>
#include <plat/prcm.h>

void omap1_arch_reset(char mode, const char *cmd)
=======
#include <plat/prcm.h>

#include <mach/hardware.h>

void omap1_restart(char mode, const char *cmd)
>>>>>>> refs/remotes/origin/cm-10.0
{
	/*
	 * Workaround for 5912/1611b bug mentioned in sprz209d.pdf p. 28
	 * "Global Software Reset Affects Traffic Controller Frequency".
	 */
	if (cpu_is_omap5912()) {
		omap_writew(omap_readw(DPLL_CTL) & ~(1 << 4), DPLL_CTL);
		omap_writew(0x8, ARM_RSTCT1);
	}

	omap_writew(1, ARM_RSTCT1);
}
<<<<<<< HEAD

void (*arch_reset)(char, const char *) = omap1_arch_reset;
=======
>>>>>>> refs/remotes/origin/cm-10.0
