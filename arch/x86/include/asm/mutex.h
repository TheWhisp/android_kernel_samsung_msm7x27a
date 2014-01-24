#ifdef CONFIG_X86_32
<<<<<<< HEAD
# include "mutex_32.h"
#else
# include "mutex_64.h"
=======
# include <asm/mutex_32.h>
#else
# include <asm/mutex_64.h>
>>>>>>> refs/remotes/origin/master
#endif
