#ifndef __ASM_ARCH_SYSTEM_H
#define __ASM_ARCH_SYSTEM_H

<<<<<<< HEAD
<<<<<<< HEAD
static inline void arch_idle(void)
{
	cpu_do_idle();
}

static inline void arch_reset(char mode, const char *cmd)
{
	cpu_reset(0);
=======
=======
>>>>>>> refs/remotes/origin/master
#include <asm/system_misc.h>

static inline void arch_reset(char mode, const char *cmd)
{
	soft_restart(0);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

#endif
