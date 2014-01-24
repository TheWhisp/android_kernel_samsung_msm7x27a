/*
 * Declare directives for structure packing. No padding will be provided
 * between the members of packed structures, and therefore, there is no
 * guarantee that structure members will be aligned.
 *
 * Declaring packed structures is compiler specific. In order to handle all
 * cases, packed structures should be delared as:
 *
 * #include <packed_section_start.h>
 *
 * typedef BWL_PRE_PACKED_STRUCT struct foobar_t {
 *    some_struct_members;
 * } BWL_POST_PACKED_STRUCT foobar_t;
 *
 * #include <packed_section_end.h>
 *
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright (C) 1999-2011, Broadcom Corporation
 * 
 *         Unless you and Broadcom execute a separate written software license
=======
 * Copyright (C) 1999-2012, Broadcom Corporation
 * 
 *      Unless you and Broadcom execute a separate written software license
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Copyright (C) 1999-2012, Broadcom Corporation
 * 
 *      Unless you and Broadcom execute a separate written software license
>>>>>>> refs/remotes/origin/cm-11.0
 * agreement governing use of this software, this software is licensed to you
 * under the terms of the GNU General Public License version 2 (the "GPL"),
 * available at http://www.broadcom.com/licenses/GPLv2.php, with the
 * following added to such license:
 * 
 *      As a special exception, the copyright holders of this software give you
 * permission to link this software with independent modules, and to copy and
 * distribute the resulting executable under terms of your choice, provided that
 * you also meet, for each linked independent module, the terms and conditions of
 * the license of that module.  An independent module is a module which is not
 * derived from this software.  The special exception does not apply to any
 * modifications of the software.
 * 
 *      Notwithstanding the above, under no circumstances may you combine this
 * software in any way with any other Broadcom software provided under a license
 * other than the GPL, without Broadcom's express prior written consent.
<<<<<<< HEAD
<<<<<<< HEAD
 * $Id: packed_section_start.h 277737 2011-08-16 17:54:59Z $
=======
 * $Id: packed_section_start.h 286783 2011-09-29 06:18:57Z $
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * $Id: packed_section_start.h 286783 2011-09-29 06:18:57Z $
>>>>>>> refs/remotes/origin/cm-11.0
 */



<<<<<<< HEAD
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
#ifdef BWL_PACKED_SECTION
	#error "BWL_PACKED_SECTION is already defined!"
#else
	#define BWL_PACKED_SECTION
#endif





<<<<<<< HEAD
<<<<<<< HEAD
#if defined(__GNUC__)
=======
#if defined(__GNUC__) || defined(__lint)
>>>>>>> refs/remotes/origin/cm-10.0
=======
#if defined(__GNUC__) || defined(__lint)
>>>>>>> refs/remotes/origin/cm-11.0
	#define	BWL_PRE_PACKED_STRUCT
	#define	BWL_POST_PACKED_STRUCT	__attribute__ ((packed))
#elif defined(__CC_ARM)
	#define	BWL_PRE_PACKED_STRUCT	__packed
	#define	BWL_POST_PACKED_STRUCT
#else
	#error "Unknown compiler!"
#endif
