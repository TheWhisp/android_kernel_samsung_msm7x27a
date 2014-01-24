#ifdef __KERNEL__
# ifdef CONFIG_X86_32
#  include "posix_types_32.h"
# else
#  include "posix_types_64.h"
# endif
#else
# ifdef __i386__
#  include "posix_types_32.h"
<<<<<<< HEAD
=======
# elif defined(__ILP32__)
#  include "posix_types_x32.h"
>>>>>>> refs/remotes/origin/cm-10.0
# else
#  include "posix_types_64.h"
# endif
#endif
