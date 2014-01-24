#ifndef __ASM_ARCH_HARDWARE_H
#define __ASM_ARCH_HARDWARE_H
#include <asm/types.h>

<<<<<<< HEAD
#define pcibios_assign_all_busses() 1

#ifndef __ASSEMBLY__
extern unsigned long iop13xx_pcibios_min_io;
extern unsigned long iop13xx_pcibios_min_mem;
=======
#ifndef __ASSEMBLY__
>>>>>>> refs/remotes/origin/cm-10.0
extern u16 iop13xx_dev_id(void);
extern void iop13xx_set_atu_mmr_bases(void);
#endif

<<<<<<< HEAD
#define PCIBIOS_MIN_IO      (iop13xx_pcibios_min_io)
#define PCIBIOS_MIN_MEM     (iop13xx_pcibios_min_mem)

=======
>>>>>>> refs/remotes/origin/cm-10.0
/*
 * Generic chipset bits
 *
 */
#include "iop13xx.h"

/*
 * Board specific bits
 */
#include "iq81340.h"

#endif  /* _ASM_ARCH_HARDWARE_H */
