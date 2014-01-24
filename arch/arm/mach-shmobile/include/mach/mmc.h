#ifndef MMC_H
#define MMC_H

/**************************************************
 *
 *		board specific settings
 *
 **************************************************/

<<<<<<< HEAD
#ifdef CONFIG_MACH_AP4EVB
#include "mach/mmc-ap4eb.h"
#elif defined(CONFIG_MACH_MACKEREL)
=======
#ifdef CONFIG_MACH_MACKEREL
>>>>>>> refs/remotes/origin/master
#include "mach/mmc-mackerel.h"
#else
#error "unsupported board."
#endif

#endif /* MMC_H */
