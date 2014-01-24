#ifndef ___ASM_SPARC_PROCESSOR_H
#define ___ASM_SPARC_PROCESSOR_H
#if defined(__sparc__) && defined(__arch64__)
#include <asm/processor_64.h>
#else
#include <asm/processor_32.h>
#endif
<<<<<<< HEAD
<<<<<<< HEAD
=======

#define nop() 		__asm__ __volatile__ ("nop")

>>>>>>> refs/remotes/origin/cm-10.0
=======

#define nop() 		__asm__ __volatile__ ("nop")

>>>>>>> refs/remotes/origin/master
#endif
