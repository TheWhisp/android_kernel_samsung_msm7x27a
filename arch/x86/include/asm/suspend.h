#ifdef CONFIG_X86_32
<<<<<<< HEAD
# include "suspend_32.h"
#else
# include "suspend_64.h"
=======
# include <asm/suspend_32.h>
#else
# include <asm/suspend_64.h>
>>>>>>> refs/remotes/origin/master
#endif
