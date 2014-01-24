#ifdef __uClinux__
<<<<<<< HEAD
#include "cacheflush_no.h"
#else
#include "cacheflush_mm.h"
=======
#include <asm/cacheflush_no.h>
#else
#include <asm/cacheflush_mm.h>
>>>>>>> refs/remotes/origin/master
#endif
