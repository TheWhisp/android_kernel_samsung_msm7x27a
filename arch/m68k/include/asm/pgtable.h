#ifdef __uClinux__
<<<<<<< HEAD
#include "pgtable_no.h"
#else
#include "pgtable_mm.h"
=======
#include <asm/pgtable_no.h>
#else
#include <asm/pgtable_mm.h>
>>>>>>> refs/remotes/origin/master
#endif
