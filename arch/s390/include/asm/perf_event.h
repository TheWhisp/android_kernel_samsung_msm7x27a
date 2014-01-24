/*
 * Performance event support - s390 specific definitions.
 *
<<<<<<< HEAD
 * Copyright 2009 Martin Schwidefsky, IBM Corporation.
 */

/* Empty, just to avoid compiling error */

#define PERF_EVENT_INDEX_OFFSET 0
=======
 * Copyright IBM Corp. 2009, 2012
 * Author(s): Martin Schwidefsky <schwidefsky@de.ibm.com>
 *	      Hendrik Brueckner <brueckner@linux.vnet.ibm.com>
 */

#include <asm/cpu_mf.h>

/* CPU-measurement counter facility */
#define PERF_CPUM_CF_MAX_CTR		160

/* Per-CPU flags for PMU states */
#define PMU_F_RESERVED			0x1000
#define PMU_F_ENABLED			0x2000
>>>>>>> refs/remotes/origin/cm-10.0
