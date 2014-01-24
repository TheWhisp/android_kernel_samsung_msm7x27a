
<<<<<<< HEAD
#ifdef ARCH_X86_64
=======
#ifdef HAVE_ARCH_X86_64_SUPPORT
>>>>>>> refs/remotes/origin/master

#define MEMCPY_FN(fn, name, desc)		\
	extern void *fn(void *, const void *, size_t);

#include "mem-memcpy-x86-64-asm-def.h"

#undef MEMCPY_FN

#endif

