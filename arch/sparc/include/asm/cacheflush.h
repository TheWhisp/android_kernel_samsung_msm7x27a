#ifndef ___ASM_SPARC_CACHEFLUSH_H
#define ___ASM_SPARC_CACHEFLUSH_H
<<<<<<< HEAD
=======

/* flush addr - to allow use of self-modifying code */
#define flushi(addr)	__asm__ __volatile__ ("flush %0" : : "r" (addr) : "memory")

>>>>>>> refs/remotes/origin/master
#if defined(__sparc__) && defined(__arch64__)
#include <asm/cacheflush_64.h>
#else
#include <asm/cacheflush_32.h>
#endif
#endif
