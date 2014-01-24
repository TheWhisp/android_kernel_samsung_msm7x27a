#include <asm/unistd.h>
#include <sys/mman.h>
#include <signal.h>
<<<<<<< HEAD
#include "as-layout.h"
#include "stub-data.h"
=======
#include <as-layout.h>
#include <stub-data.h>
>>>>>>> refs/remotes/origin/master

#ifdef __i386__
#include "stub_32.h"
#else
#include "stub_64.h"
#endif

extern void stub_segv_handler(int, siginfo_t *, void *);
extern void stub_clone_handler(void);
