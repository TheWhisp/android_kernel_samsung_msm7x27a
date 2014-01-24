#ifdef __uClinux__
<<<<<<< HEAD
#include "io_no.h"
#else
#include "io_mm.h"
=======
#include <asm/io_no.h>
#else
#include <asm/io_mm.h>
>>>>>>> refs/remotes/origin/master
#endif
