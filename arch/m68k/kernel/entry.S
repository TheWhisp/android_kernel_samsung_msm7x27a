<<<<<<< HEAD
#ifdef CONFIG_MMU
=======
#if defined(CONFIG_MMU) && !defined(CONFIG_COLDFIRE)
>>>>>>> refs/remotes/origin/cm-10.0
#include "entry_mm.S"
#else
#include "entry_no.S"
#endif
