#ifdef CONFIG_X86_32
<<<<<<< HEAD
# include "seccomp_32.h"
#else
# include "seccomp_64.h"
=======
# include <asm/seccomp_32.h>
#else
# include <asm/seccomp_64.h>
>>>>>>> refs/remotes/origin/master
#endif
