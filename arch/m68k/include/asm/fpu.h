#ifndef __M68K_FPU_H
#define __M68K_FPU_H


/*
 * MAX floating point unit state size (FSAVE/FRESTORE)
 */

#if defined(CONFIG_M68020) || defined(CONFIG_M68030)
#define FPSTATESIZE (216)
#elif defined(CONFIG_M68040)
#define FPSTATESIZE (96)
#elif defined(CONFIG_M68KFPU_EMU)
#define FPSTATESIZE (28)
<<<<<<< HEAD
<<<<<<< HEAD
=======
#elif defined(CONFIG_COLDFIRE) && defined(CONFIG_MMU)
#define FPSTATESIZE (16)
>>>>>>> refs/remotes/origin/cm-10.0
=======
#elif defined(CONFIG_COLDFIRE) && defined(CONFIG_MMU)
#define FPSTATESIZE (16)
>>>>>>> refs/remotes/origin/master
#elif defined(CONFIG_M68060)
#define FPSTATESIZE (12)
#else
#define FPSTATESIZE (0)
#endif

#endif /* __M68K_FPU_H */
