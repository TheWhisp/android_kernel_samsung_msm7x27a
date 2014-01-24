/***********************license start***************
 * Author: Cavium Networks
 *
 * Contact: support@caviumnetworks.com
 * This file is part of the OCTEON SDK
 *
<<<<<<< HEAD
 * Copyright (c) 2003-2010 Cavium Networks
=======
 * Copyright (c) 2003-2012 Cavium Networks
>>>>>>> refs/remotes/origin/master
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, Version 2, as
 * published by the Free Software Foundation.
 *
 * This file is distributed in the hope that it will be useful, but
 * AS-IS and WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE, TITLE, or
 * NONINFRINGEMENT.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this file; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 * or visit http://www.gnu.org/licenses/.
 *
 * This file may also be available under a different license from Cavium.
 * Contact Cavium Networks for more information
 ***********************license end**************************************/

#ifndef __CVMX_L2D_DEFS_H__
#define __CVMX_L2D_DEFS_H__

#define CVMX_L2D_BST0 (CVMX_ADD_IO_SEG(0x0001180080000780ull))
#define CVMX_L2D_BST1 (CVMX_ADD_IO_SEG(0x0001180080000788ull))
#define CVMX_L2D_BST2 (CVMX_ADD_IO_SEG(0x0001180080000790ull))
#define CVMX_L2D_BST3 (CVMX_ADD_IO_SEG(0x0001180080000798ull))
#define CVMX_L2D_ERR (CVMX_ADD_IO_SEG(0x0001180080000010ull))
#define CVMX_L2D_FADR (CVMX_ADD_IO_SEG(0x0001180080000018ull))
#define CVMX_L2D_FSYN0 (CVMX_ADD_IO_SEG(0x0001180080000020ull))
#define CVMX_L2D_FSYN1 (CVMX_ADD_IO_SEG(0x0001180080000028ull))
#define CVMX_L2D_FUS0 (CVMX_ADD_IO_SEG(0x00011800800007A0ull))
#define CVMX_L2D_FUS1 (CVMX_ADD_IO_SEG(0x00011800800007A8ull))
#define CVMX_L2D_FUS2 (CVMX_ADD_IO_SEG(0x00011800800007B0ull))
#define CVMX_L2D_FUS3 (CVMX_ADD_IO_SEG(0x00011800800007B8ull))

union cvmx_l2d_bst0 {
	uint64_t u64;
	struct cvmx_l2d_bst0_s {
<<<<<<< HEAD
		uint64_t reserved_35_63:29;
		uint64_t ftl:1;
		uint64_t q0stat:34;
=======
#ifdef __BIG_ENDIAN_BITFIELD
		uint64_t reserved_35_63:29;
		uint64_t ftl:1;
		uint64_t q0stat:34;
#else
		uint64_t q0stat:34;
		uint64_t ftl:1;
		uint64_t reserved_35_63:29;
#endif
>>>>>>> refs/remotes/origin/master
	} s;
	struct cvmx_l2d_bst0_s cn30xx;
	struct cvmx_l2d_bst0_s cn31xx;
	struct cvmx_l2d_bst0_s cn38xx;
	struct cvmx_l2d_bst0_s cn38xxp2;
	struct cvmx_l2d_bst0_s cn50xx;
	struct cvmx_l2d_bst0_s cn52xx;
	struct cvmx_l2d_bst0_s cn52xxp1;
	struct cvmx_l2d_bst0_s cn56xx;
	struct cvmx_l2d_bst0_s cn56xxp1;
	struct cvmx_l2d_bst0_s cn58xx;
	struct cvmx_l2d_bst0_s cn58xxp1;
};

union cvmx_l2d_bst1 {
	uint64_t u64;
	struct cvmx_l2d_bst1_s {
<<<<<<< HEAD
		uint64_t reserved_34_63:30;
		uint64_t q1stat:34;
=======
#ifdef __BIG_ENDIAN_BITFIELD
		uint64_t reserved_34_63:30;
		uint64_t q1stat:34;
#else
		uint64_t q1stat:34;
		uint64_t reserved_34_63:30;
#endif
>>>>>>> refs/remotes/origin/master
	} s;
	struct cvmx_l2d_bst1_s cn30xx;
	struct cvmx_l2d_bst1_s cn31xx;
	struct cvmx_l2d_bst1_s cn38xx;
	struct cvmx_l2d_bst1_s cn38xxp2;
	struct cvmx_l2d_bst1_s cn50xx;
	struct cvmx_l2d_bst1_s cn52xx;
	struct cvmx_l2d_bst1_s cn52xxp1;
	struct cvmx_l2d_bst1_s cn56xx;
	struct cvmx_l2d_bst1_s cn56xxp1;
	struct cvmx_l2d_bst1_s cn58xx;
	struct cvmx_l2d_bst1_s cn58xxp1;
};

union cvmx_l2d_bst2 {
	uint64_t u64;
	struct cvmx_l2d_bst2_s {
<<<<<<< HEAD
		uint64_t reserved_34_63:30;
		uint64_t q2stat:34;
=======
#ifdef __BIG_ENDIAN_BITFIELD
		uint64_t reserved_34_63:30;
		uint64_t q2stat:34;
#else
		uint64_t q2stat:34;
		uint64_t reserved_34_63:30;
#endif
>>>>>>> refs/remotes/origin/master
	} s;
	struct cvmx_l2d_bst2_s cn30xx;
	struct cvmx_l2d_bst2_s cn31xx;
	struct cvmx_l2d_bst2_s cn38xx;
	struct cvmx_l2d_bst2_s cn38xxp2;
	struct cvmx_l2d_bst2_s cn50xx;
	struct cvmx_l2d_bst2_s cn52xx;
	struct cvmx_l2d_bst2_s cn52xxp1;
	struct cvmx_l2d_bst2_s cn56xx;
	struct cvmx_l2d_bst2_s cn56xxp1;
	struct cvmx_l2d_bst2_s cn58xx;
	struct cvmx_l2d_bst2_s cn58xxp1;
};

union cvmx_l2d_bst3 {
	uint64_t u64;
	struct cvmx_l2d_bst3_s {
<<<<<<< HEAD
		uint64_t reserved_34_63:30;
		uint64_t q3stat:34;
=======
#ifdef __BIG_ENDIAN_BITFIELD
		uint64_t reserved_34_63:30;
		uint64_t q3stat:34;
#else
		uint64_t q3stat:34;
		uint64_t reserved_34_63:30;
#endif
>>>>>>> refs/remotes/origin/master
	} s;
	struct cvmx_l2d_bst3_s cn30xx;
	struct cvmx_l2d_bst3_s cn31xx;
	struct cvmx_l2d_bst3_s cn38xx;
	struct cvmx_l2d_bst3_s cn38xxp2;
	struct cvmx_l2d_bst3_s cn50xx;
	struct cvmx_l2d_bst3_s cn52xx;
	struct cvmx_l2d_bst3_s cn52xxp1;
	struct cvmx_l2d_bst3_s cn56xx;
	struct cvmx_l2d_bst3_s cn56xxp1;
	struct cvmx_l2d_bst3_s cn58xx;
	struct cvmx_l2d_bst3_s cn58xxp1;
};

union cvmx_l2d_err {
	uint64_t u64;
	struct cvmx_l2d_err_s {
<<<<<<< HEAD
=======
#ifdef __BIG_ENDIAN_BITFIELD
>>>>>>> refs/remotes/origin/master
		uint64_t reserved_6_63:58;
		uint64_t bmhclsel:1;
		uint64_t ded_err:1;
		uint64_t sec_err:1;
		uint64_t ded_intena:1;
		uint64_t sec_intena:1;
		uint64_t ecc_ena:1;
<<<<<<< HEAD
=======
#else
		uint64_t ecc_ena:1;
		uint64_t sec_intena:1;
		uint64_t ded_intena:1;
		uint64_t sec_err:1;
		uint64_t ded_err:1;
		uint64_t bmhclsel:1;
		uint64_t reserved_6_63:58;
#endif
>>>>>>> refs/remotes/origin/master
	} s;
	struct cvmx_l2d_err_s cn30xx;
	struct cvmx_l2d_err_s cn31xx;
	struct cvmx_l2d_err_s cn38xx;
	struct cvmx_l2d_err_s cn38xxp2;
	struct cvmx_l2d_err_s cn50xx;
	struct cvmx_l2d_err_s cn52xx;
	struct cvmx_l2d_err_s cn52xxp1;
	struct cvmx_l2d_err_s cn56xx;
	struct cvmx_l2d_err_s cn56xxp1;
	struct cvmx_l2d_err_s cn58xx;
	struct cvmx_l2d_err_s cn58xxp1;
};

union cvmx_l2d_fadr {
	uint64_t u64;
	struct cvmx_l2d_fadr_s {
<<<<<<< HEAD
=======
#ifdef __BIG_ENDIAN_BITFIELD
>>>>>>> refs/remotes/origin/master
		uint64_t reserved_19_63:45;
		uint64_t fadru:1;
		uint64_t fowmsk:4;
		uint64_t fset:3;
		uint64_t fadr:11;
<<<<<<< HEAD
	} s;
	struct cvmx_l2d_fadr_cn30xx {
=======
#else
		uint64_t fadr:11;
		uint64_t fset:3;
		uint64_t fowmsk:4;
		uint64_t fadru:1;
		uint64_t reserved_19_63:45;
#endif
	} s;
	struct cvmx_l2d_fadr_cn30xx {
#ifdef __BIG_ENDIAN_BITFIELD
>>>>>>> refs/remotes/origin/master
		uint64_t reserved_18_63:46;
		uint64_t fowmsk:4;
		uint64_t reserved_13_13:1;
		uint64_t fset:2;
		uint64_t reserved_9_10:2;
		uint64_t fadr:9;
<<<<<<< HEAD
	} cn30xx;
	struct cvmx_l2d_fadr_cn31xx {
=======
#else
		uint64_t fadr:9;
		uint64_t reserved_9_10:2;
		uint64_t fset:2;
		uint64_t reserved_13_13:1;
		uint64_t fowmsk:4;
		uint64_t reserved_18_63:46;
#endif
	} cn30xx;
	struct cvmx_l2d_fadr_cn31xx {
#ifdef __BIG_ENDIAN_BITFIELD
>>>>>>> refs/remotes/origin/master
		uint64_t reserved_18_63:46;
		uint64_t fowmsk:4;
		uint64_t reserved_13_13:1;
		uint64_t fset:2;
		uint64_t reserved_10_10:1;
		uint64_t fadr:10;
<<<<<<< HEAD
	} cn31xx;
	struct cvmx_l2d_fadr_cn38xx {
=======
#else
		uint64_t fadr:10;
		uint64_t reserved_10_10:1;
		uint64_t fset:2;
		uint64_t reserved_13_13:1;
		uint64_t fowmsk:4;
		uint64_t reserved_18_63:46;
#endif
	} cn31xx;
	struct cvmx_l2d_fadr_cn38xx {
#ifdef __BIG_ENDIAN_BITFIELD
>>>>>>> refs/remotes/origin/master
		uint64_t reserved_18_63:46;
		uint64_t fowmsk:4;
		uint64_t fset:3;
		uint64_t fadr:11;
<<<<<<< HEAD
	} cn38xx;
	struct cvmx_l2d_fadr_cn38xx cn38xxp2;
	struct cvmx_l2d_fadr_cn50xx {
=======
#else
		uint64_t fadr:11;
		uint64_t fset:3;
		uint64_t fowmsk:4;
		uint64_t reserved_18_63:46;
#endif
	} cn38xx;
	struct cvmx_l2d_fadr_cn38xx cn38xxp2;
	struct cvmx_l2d_fadr_cn50xx {
#ifdef __BIG_ENDIAN_BITFIELD
>>>>>>> refs/remotes/origin/master
		uint64_t reserved_18_63:46;
		uint64_t fowmsk:4;
		uint64_t fset:3;
		uint64_t reserved_8_10:3;
		uint64_t fadr:8;
<<<<<<< HEAD
	} cn50xx;
	struct cvmx_l2d_fadr_cn52xx {
=======
#else
		uint64_t fadr:8;
		uint64_t reserved_8_10:3;
		uint64_t fset:3;
		uint64_t fowmsk:4;
		uint64_t reserved_18_63:46;
#endif
	} cn50xx;
	struct cvmx_l2d_fadr_cn52xx {
#ifdef __BIG_ENDIAN_BITFIELD
>>>>>>> refs/remotes/origin/master
		uint64_t reserved_18_63:46;
		uint64_t fowmsk:4;
		uint64_t fset:3;
		uint64_t reserved_10_10:1;
		uint64_t fadr:10;
<<<<<<< HEAD
=======
#else
		uint64_t fadr:10;
		uint64_t reserved_10_10:1;
		uint64_t fset:3;
		uint64_t fowmsk:4;
		uint64_t reserved_18_63:46;
#endif
>>>>>>> refs/remotes/origin/master
	} cn52xx;
	struct cvmx_l2d_fadr_cn52xx cn52xxp1;
	struct cvmx_l2d_fadr_s cn56xx;
	struct cvmx_l2d_fadr_s cn56xxp1;
	struct cvmx_l2d_fadr_s cn58xx;
	struct cvmx_l2d_fadr_s cn58xxp1;
};

union cvmx_l2d_fsyn0 {
	uint64_t u64;
	struct cvmx_l2d_fsyn0_s {
<<<<<<< HEAD
		uint64_t reserved_20_63:44;
		uint64_t fsyn_ow1:10;
		uint64_t fsyn_ow0:10;
=======
#ifdef __BIG_ENDIAN_BITFIELD
		uint64_t reserved_20_63:44;
		uint64_t fsyn_ow1:10;
		uint64_t fsyn_ow0:10;
#else
		uint64_t fsyn_ow0:10;
		uint64_t fsyn_ow1:10;
		uint64_t reserved_20_63:44;
#endif
>>>>>>> refs/remotes/origin/master
	} s;
	struct cvmx_l2d_fsyn0_s cn30xx;
	struct cvmx_l2d_fsyn0_s cn31xx;
	struct cvmx_l2d_fsyn0_s cn38xx;
	struct cvmx_l2d_fsyn0_s cn38xxp2;
	struct cvmx_l2d_fsyn0_s cn50xx;
	struct cvmx_l2d_fsyn0_s cn52xx;
	struct cvmx_l2d_fsyn0_s cn52xxp1;
	struct cvmx_l2d_fsyn0_s cn56xx;
	struct cvmx_l2d_fsyn0_s cn56xxp1;
	struct cvmx_l2d_fsyn0_s cn58xx;
	struct cvmx_l2d_fsyn0_s cn58xxp1;
};

union cvmx_l2d_fsyn1 {
	uint64_t u64;
	struct cvmx_l2d_fsyn1_s {
<<<<<<< HEAD
		uint64_t reserved_20_63:44;
		uint64_t fsyn_ow3:10;
		uint64_t fsyn_ow2:10;
=======
#ifdef __BIG_ENDIAN_BITFIELD
		uint64_t reserved_20_63:44;
		uint64_t fsyn_ow3:10;
		uint64_t fsyn_ow2:10;
#else
		uint64_t fsyn_ow2:10;
		uint64_t fsyn_ow3:10;
		uint64_t reserved_20_63:44;
#endif
>>>>>>> refs/remotes/origin/master
	} s;
	struct cvmx_l2d_fsyn1_s cn30xx;
	struct cvmx_l2d_fsyn1_s cn31xx;
	struct cvmx_l2d_fsyn1_s cn38xx;
	struct cvmx_l2d_fsyn1_s cn38xxp2;
	struct cvmx_l2d_fsyn1_s cn50xx;
	struct cvmx_l2d_fsyn1_s cn52xx;
	struct cvmx_l2d_fsyn1_s cn52xxp1;
	struct cvmx_l2d_fsyn1_s cn56xx;
	struct cvmx_l2d_fsyn1_s cn56xxp1;
	struct cvmx_l2d_fsyn1_s cn58xx;
	struct cvmx_l2d_fsyn1_s cn58xxp1;
};

union cvmx_l2d_fus0 {
	uint64_t u64;
	struct cvmx_l2d_fus0_s {
<<<<<<< HEAD
		uint64_t reserved_34_63:30;
		uint64_t q0fus:34;
=======
#ifdef __BIG_ENDIAN_BITFIELD
		uint64_t reserved_34_63:30;
		uint64_t q0fus:34;
#else
		uint64_t q0fus:34;
		uint64_t reserved_34_63:30;
#endif
>>>>>>> refs/remotes/origin/master
	} s;
	struct cvmx_l2d_fus0_s cn30xx;
	struct cvmx_l2d_fus0_s cn31xx;
	struct cvmx_l2d_fus0_s cn38xx;
	struct cvmx_l2d_fus0_s cn38xxp2;
	struct cvmx_l2d_fus0_s cn50xx;
	struct cvmx_l2d_fus0_s cn52xx;
	struct cvmx_l2d_fus0_s cn52xxp1;
	struct cvmx_l2d_fus0_s cn56xx;
	struct cvmx_l2d_fus0_s cn56xxp1;
	struct cvmx_l2d_fus0_s cn58xx;
	struct cvmx_l2d_fus0_s cn58xxp1;
};

union cvmx_l2d_fus1 {
	uint64_t u64;
	struct cvmx_l2d_fus1_s {
<<<<<<< HEAD
		uint64_t reserved_34_63:30;
		uint64_t q1fus:34;
=======
#ifdef __BIG_ENDIAN_BITFIELD
		uint64_t reserved_34_63:30;
		uint64_t q1fus:34;
#else
		uint64_t q1fus:34;
		uint64_t reserved_34_63:30;
#endif
>>>>>>> refs/remotes/origin/master
	} s;
	struct cvmx_l2d_fus1_s cn30xx;
	struct cvmx_l2d_fus1_s cn31xx;
	struct cvmx_l2d_fus1_s cn38xx;
	struct cvmx_l2d_fus1_s cn38xxp2;
	struct cvmx_l2d_fus1_s cn50xx;
	struct cvmx_l2d_fus1_s cn52xx;
	struct cvmx_l2d_fus1_s cn52xxp1;
	struct cvmx_l2d_fus1_s cn56xx;
	struct cvmx_l2d_fus1_s cn56xxp1;
	struct cvmx_l2d_fus1_s cn58xx;
	struct cvmx_l2d_fus1_s cn58xxp1;
};

union cvmx_l2d_fus2 {
	uint64_t u64;
	struct cvmx_l2d_fus2_s {
<<<<<<< HEAD
		uint64_t reserved_34_63:30;
		uint64_t q2fus:34;
=======
#ifdef __BIG_ENDIAN_BITFIELD
		uint64_t reserved_34_63:30;
		uint64_t q2fus:34;
#else
		uint64_t q2fus:34;
		uint64_t reserved_34_63:30;
#endif
>>>>>>> refs/remotes/origin/master
	} s;
	struct cvmx_l2d_fus2_s cn30xx;
	struct cvmx_l2d_fus2_s cn31xx;
	struct cvmx_l2d_fus2_s cn38xx;
	struct cvmx_l2d_fus2_s cn38xxp2;
	struct cvmx_l2d_fus2_s cn50xx;
	struct cvmx_l2d_fus2_s cn52xx;
	struct cvmx_l2d_fus2_s cn52xxp1;
	struct cvmx_l2d_fus2_s cn56xx;
	struct cvmx_l2d_fus2_s cn56xxp1;
	struct cvmx_l2d_fus2_s cn58xx;
	struct cvmx_l2d_fus2_s cn58xxp1;
};

union cvmx_l2d_fus3 {
	uint64_t u64;
	struct cvmx_l2d_fus3_s {
<<<<<<< HEAD
=======
#ifdef __BIG_ENDIAN_BITFIELD
>>>>>>> refs/remotes/origin/master
		uint64_t reserved_40_63:24;
		uint64_t ema_ctl:3;
		uint64_t reserved_34_36:3;
		uint64_t q3fus:34;
<<<<<<< HEAD
	} s;
	struct cvmx_l2d_fus3_cn30xx {
		uint64_t reserved_35_63:29;
		uint64_t crip_64k:1;
		uint64_t q3fus:34;
	} cn30xx;
	struct cvmx_l2d_fus3_cn31xx {
		uint64_t reserved_35_63:29;
		uint64_t crip_128k:1;
		uint64_t q3fus:34;
	} cn31xx;
	struct cvmx_l2d_fus3_cn38xx {
=======
#else
		uint64_t q3fus:34;
		uint64_t reserved_34_36:3;
		uint64_t ema_ctl:3;
		uint64_t reserved_40_63:24;
#endif
	} s;
	struct cvmx_l2d_fus3_cn30xx {
#ifdef __BIG_ENDIAN_BITFIELD
		uint64_t reserved_35_63:29;
		uint64_t crip_64k:1;
		uint64_t q3fus:34;
#else
		uint64_t q3fus:34;
		uint64_t crip_64k:1;
		uint64_t reserved_35_63:29;
#endif
	} cn30xx;
	struct cvmx_l2d_fus3_cn31xx {
#ifdef __BIG_ENDIAN_BITFIELD
		uint64_t reserved_35_63:29;
		uint64_t crip_128k:1;
		uint64_t q3fus:34;
#else
		uint64_t q3fus:34;
		uint64_t crip_128k:1;
		uint64_t reserved_35_63:29;
#endif
	} cn31xx;
	struct cvmx_l2d_fus3_cn38xx {
#ifdef __BIG_ENDIAN_BITFIELD
>>>>>>> refs/remotes/origin/master
		uint64_t reserved_36_63:28;
		uint64_t crip_256k:1;
		uint64_t crip_512k:1;
		uint64_t q3fus:34;
<<<<<<< HEAD
	} cn38xx;
	struct cvmx_l2d_fus3_cn38xx cn38xxp2;
	struct cvmx_l2d_fus3_cn50xx {
=======
#else
		uint64_t q3fus:34;
		uint64_t crip_512k:1;
		uint64_t crip_256k:1;
		uint64_t reserved_36_63:28;
#endif
	} cn38xx;
	struct cvmx_l2d_fus3_cn38xx cn38xxp2;
	struct cvmx_l2d_fus3_cn50xx {
#ifdef __BIG_ENDIAN_BITFIELD
>>>>>>> refs/remotes/origin/master
		uint64_t reserved_40_63:24;
		uint64_t ema_ctl:3;
		uint64_t reserved_36_36:1;
		uint64_t crip_32k:1;
		uint64_t crip_64k:1;
		uint64_t q3fus:34;
<<<<<<< HEAD
	} cn50xx;
	struct cvmx_l2d_fus3_cn52xx {
=======
#else
		uint64_t q3fus:34;
		uint64_t crip_64k:1;
		uint64_t crip_32k:1;
		uint64_t reserved_36_36:1;
		uint64_t ema_ctl:3;
		uint64_t reserved_40_63:24;
#endif
	} cn50xx;
	struct cvmx_l2d_fus3_cn52xx {
#ifdef __BIG_ENDIAN_BITFIELD
>>>>>>> refs/remotes/origin/master
		uint64_t reserved_40_63:24;
		uint64_t ema_ctl:3;
		uint64_t reserved_36_36:1;
		uint64_t crip_128k:1;
		uint64_t crip_256k:1;
		uint64_t q3fus:34;
<<<<<<< HEAD
	} cn52xx;
	struct cvmx_l2d_fus3_cn52xx cn52xxp1;
	struct cvmx_l2d_fus3_cn56xx {
=======
#else
		uint64_t q3fus:34;
		uint64_t crip_256k:1;
		uint64_t crip_128k:1;
		uint64_t reserved_36_36:1;
		uint64_t ema_ctl:3;
		uint64_t reserved_40_63:24;
#endif
	} cn52xx;
	struct cvmx_l2d_fus3_cn52xx cn52xxp1;
	struct cvmx_l2d_fus3_cn56xx {
#ifdef __BIG_ENDIAN_BITFIELD
>>>>>>> refs/remotes/origin/master
		uint64_t reserved_40_63:24;
		uint64_t ema_ctl:3;
		uint64_t reserved_36_36:1;
		uint64_t crip_512k:1;
		uint64_t crip_1024k:1;
		uint64_t q3fus:34;
<<<<<<< HEAD
	} cn56xx;
	struct cvmx_l2d_fus3_cn56xx cn56xxp1;
	struct cvmx_l2d_fus3_cn58xx {
=======
#else
		uint64_t q3fus:34;
		uint64_t crip_1024k:1;
		uint64_t crip_512k:1;
		uint64_t reserved_36_36:1;
		uint64_t ema_ctl:3;
		uint64_t reserved_40_63:24;
#endif
	} cn56xx;
	struct cvmx_l2d_fus3_cn56xx cn56xxp1;
	struct cvmx_l2d_fus3_cn58xx {
#ifdef __BIG_ENDIAN_BITFIELD
>>>>>>> refs/remotes/origin/master
		uint64_t reserved_39_63:25;
		uint64_t ema_ctl:2;
		uint64_t reserved_36_36:1;
		uint64_t crip_512k:1;
		uint64_t crip_1024k:1;
		uint64_t q3fus:34;
<<<<<<< HEAD
=======
#else
		uint64_t q3fus:34;
		uint64_t crip_1024k:1;
		uint64_t crip_512k:1;
		uint64_t reserved_36_36:1;
		uint64_t ema_ctl:2;
		uint64_t reserved_39_63:25;
#endif
>>>>>>> refs/remotes/origin/master
	} cn58xx;
	struct cvmx_l2d_fus3_cn58xx cn58xxp1;
};

#endif
