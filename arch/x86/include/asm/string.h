#ifdef CONFIG_X86_32
<<<<<<< HEAD
# include "string_32.h"
#else
# include "string_64.h"
=======
# include <asm/string_32.h>
#else
# include <asm/string_64.h>
>>>>>>> refs/remotes/origin/master
#endif
