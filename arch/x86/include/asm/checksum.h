#ifdef CONFIG_X86_32
<<<<<<< HEAD
# include "checksum_32.h"
#else
# include "checksum_64.h"
=======
# include <asm/checksum_32.h>
#else
# include <asm/checksum_64.h>
>>>>>>> refs/remotes/origin/master
#endif
