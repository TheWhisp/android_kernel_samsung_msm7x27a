#ifdef CONFIG_X86_32
<<<<<<< HEAD
# include "mmzone_32.h"
#else
# include "mmzone_64.h"
=======
# include <asm/mmzone_32.h>
#else
# include <asm/mmzone_64.h>
>>>>>>> refs/remotes/origin/master
#endif
