/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Derived from IRIX <sys/SN/SN0/sn0_fru.h>
 *
 * Copyright (C) 1992 - 1997, 1999 Silcon Graphics, Inc.
 * Copyright (C) 1999, 2006 Ralf Baechle (ralf@linux-mips)
 */
#ifndef __ASM_SN_FRU_H
#define __ASM_SN_FRU_H

#define MAX_DIMMS			8	 /* max # of dimm banks */
#define MAX_PCIDEV			8	 /* max # of pci devices on a pci bus */

typedef unsigned char confidence_t;

typedef struct kf_mem_s {
	confidence_t km_confidence; /* confidence level that the memory is bad
				     * is this necessary ?
				     */
	confidence_t km_dimm[MAX_DIMMS];
<<<<<<< HEAD
	                            /* confidence level that dimm[i] is bad
=======
				    /* confidence level that dimm[i] is bad
>>>>>>> refs/remotes/origin/master
				     *I think this is the right number
				     */

} kf_mem_t;

typedef struct kf_cpu_s {
<<<<<<< HEAD
	confidence_t  	kc_confidence; /* confidence level that cpu is bad */
	confidence_t  	kc_icache; /* confidence level that instr. cache is bad */
	confidence_t  	kc_dcache; /* confidence level that data   cache is bad */
	confidence_t  	kc_scache; /* confidence level that sec.   cache is bad */
=======
	confidence_t	kc_confidence; /* confidence level that cpu is bad */
	confidence_t	kc_icache; /* confidence level that instr. cache is bad */
	confidence_t	kc_dcache; /* confidence level that data   cache is bad */
	confidence_t	kc_scache; /* confidence level that sec.   cache is bad */
>>>>>>> refs/remotes/origin/master
	confidence_t	kc_sysbus; /* confidence level that sysad/cmd/state bus is bad */
} kf_cpu_t;

typedef struct kf_pci_bus_s {
	confidence_t	kpb_belief;	/* confidence level  that the  pci bus is bad */
	confidence_t	kpb_pcidev_belief[MAX_PCIDEV];
<<<<<<< HEAD
	                                /* confidence level that the pci dev is bad */
=======
					/* confidence level that the pci dev is bad */
>>>>>>> refs/remotes/origin/master
} kf_pci_bus_t;

#endif /* __ASM_SN_FRU_H */
